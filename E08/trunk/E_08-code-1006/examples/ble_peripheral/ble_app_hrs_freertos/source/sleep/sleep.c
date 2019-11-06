/*******************Copyright(c)*********************************
** Veepoo Tech
** COPYRIGHT (C) 2016-2019
**----------------------FileInfo---------------------------------
** File name:                 sleep.c
** Last modified Date:         2017-08-12
** Last SYS_VERSION:           0.0.1
** Descriptions:
**---------------------------------------------------------------
** Createdby:                  WJ
** Createddate:                2017-01-04
** SYS_VERSION:                0.0.1
** Descriptions:               睡眠
**---------------------------------------------------------------
** Modified by:				
** Modified date:			
** SYS_VERSION:				
** Modified Descriptions:	
****************************************************************/

#include "sleep.h"
#include "SystemTime.h"
#include "api_flash.h"
#include "app_type.h"
#include "log.h"
#include "flash_common.h"
#include "app_crc16.h"

#include "test_interface.h"
#include "watch_dog.h"
                                                        //测试阶段不开启
#define   SLEE_RESULT_CHECK_ENABLED         0           //出睡眠结果后，存储前对睡眠结果数据检查过滤
                                                        

static uint8_t sleep_type = 0x01;

#if SLEEP_CALC_ENABLED 

static bool Sleep_ValidityCheck(uint8_t *curr_sleep_time, uint8_t *last_sleep_time);
    
/****************************************************************
** Function name:             Sleep_ResultCheck
** Descriptions:              睡眠结果数据检查
** input parameters:          无
** output parameters:         无
** Returned value:            无
** Created by:                WJ
** Created Date:              2018-12-14
****************************************************************/ 
bool Sleep_ResultCheck(const acculate_sleep_quality_t* p_sleep_info)
{
#if SLEE_RESULT_CHECK_ENABLED    
    //入睡时间和退出睡眠时间
    if(p_sleep_info->enter_time.month == p_sleep_info->quit_time.month)
    {   //同一个月份
        if(p_sleep_info->enter_time.date == p_sleep_info->quit_time.date)
        {   //入睡和退出睡眠时间在同一天
            if(p_sleep_info->enter_time.hour*60 + p_sleep_info->enter_time.minute 
               >= p_sleep_info->quit_time.hour*60 + p_sleep_info->quit_time.minute)
            {   //退出睡眠时间小于入睡时间-时分
                return false;
            }
        }
        else if(p_sleep_info->enter_time.date > p_sleep_info->quit_time.date)
        {   //退出睡眠时间小于入睡时间-日
            return false; 
        }
    }
    else if(p_sleep_info->enter_time.month > p_sleep_info->quit_time.month)
    {   // 跨年
        if(p_sleep_info->enter_time.month != 12 || p_sleep_info->quit_time.month != 1)
        {
            return false; 
        }
    }
    else
    {   //跨月
        if(p_sleep_info->enter_time.month + 1 != p_sleep_info->quit_time.month)
        {   //月份不连续
            return false; 
        }
        if(p_sleep_info->quit_time.date != 1) 
        {   
            return false;  
        }            
    }
    
    //睡眠总时长
    if(p_sleep_info->all_sleep_len == 0)
    {
        return false;
    }
    //睡眠曲线长度
    if(p_sleep_info->temp_len == 0)
    {
        return false;
    }

#endif  //SLEE_RESULT_CHECK_ENABLED
    
    return true;
}

/****************************************************************
** Function name:             Sleep_SaveData
** Descriptions:              睡眠存储数据
** input parameters:          无
** output parameters:         无
** Returned value:            无
** Created by:                WJ
** Created Date:              2016-11-29
****************************************************************/ 
static void Sleep_SaveData(const acculate_sleep_quality_t* p_sleep_info)
{
    uint16_t            sleep_data_len = 0;                                     //睡眠数据保存长度
    uint16_t            crc_total_len = 0;                                      //睡眠数据crc校验总长度
    uint16_t            crc_value1 = 0xFFFF;                                    //基本信息校验
    uint16_t            crc_value2 = 0xFFFF;                                    //失眠信息校验
    uint16_t            crc_value3 = 0xFFFF;                                    //曲线校验
    uint16_t            crc_value4 = 0xFFFF;                                    //有效信息校验
    __ALIGN(4) uint8_t  sleep_data_buf[SLEEP_PER_DATA_MAX_SIZE/4*4 + 40] = {0};      //睡眠数据保存缓存    
    flash_find_index_t  *p_find_index = &p_flash_param->flash_index_s;
 
    uint8_t             day_id = 0;                 //数据存储块
    uint8_t             sleep_wake_cnt = 0;         //睡眠次数
    uint16_t            sleep_curve_len = 0;        //睡眠曲线长度和
    
    if(Sleep_ResultCheck(p_sleep_info) == false)
    {
        return;
    }
    
    //基本信息
    sleep_data_buf[SLEEP_BASIC_INFO_OFFSET] = 0xA3;
    sleep_data_buf[SLEEP_BASIC_INFO_OFFSET + 1] = SLEEP_BASIC_INFO_LEN;
    memcpy(&sleep_data_buf[SLEEP_BASIC_INFO_OFFSET + 2], &p_sleep_info->enter_time, SLEEP_BASIC_INFO_LEN);
    //基本信息CRC校验
    crc_value1 = App_Crc16(&sleep_data_buf[SLEEP_BASIC_INFO_OFFSET + 2], SLEEP_BASIC_INFO_LEN);

    //睡眠类型：普通睡眠、精准睡眠  
    sleep_data_buf[50] = sleep_type;
    
    //失眠信息
    sleep_data_buf[SLEEP_INSOMINA_OFFSET] = 0xA4;
    sleep_data_buf[SLEEP_INSOMINA_OFFSET + 1] = SLEEP_INSOMINA_LEN;
    memcpy(&sleep_data_buf[SLEEP_INSOMINA_OFFSET + 2], &p_sleep_info->loss_sleep_flag, SLEEP_INSOMINA_LEN);   
    //失眠信息CRC校验
    crc_value2 = App_Crc16(&sleep_data_buf[SLEEP_INSOMINA_OFFSET + 2], SLEEP_INSOMINA_LEN);
    
    //睡眠曲线
    sleep_data_buf[SLEEP_CURVE_MIG_OFFSET] = 0xA5;
    sleep_data_buf[SLEEP_CURVE_MIG_OFFSET + 1] = p_sleep_info->temp_len & 0xFF;
    sleep_data_buf[SLEEP_CURVE_MIG_OFFSET + 2] = (p_sleep_info->temp_len >> 8) & 0xFF;
    memcpy(&sleep_data_buf[SLEEP_CURVE_MIG_OFFSET + 3], p_sleep_info->deep_shallow, p_sleep_info->temp_len);      
    //睡眠曲线CRC校验,
    crc_value3 = App_Crc16(&sleep_data_buf[SLEEP_CURVE_MIG_OFFSET + 3], p_sleep_info->temp_len);
  
    //有效内容的总长度，长度必须是4字节对齐
    sleep_data_len = SLEEP_CURVE_MIG_OFFSET + 3 + p_sleep_info->temp_len;
    
    {   //WJ.20190122.增加睡眠连接处理
        //睡眠连接标志
        sleep_data_buf[sleep_data_len++] = 0xA6;
        sleep_data_buf[sleep_data_len++] = 0x02;
        if(p_sleep_info->as_continue_flag == 0x01)
        {
            sleep_data_buf[sleep_data_len++] = 1;
        }
        else
        {
            sleep_data_buf[sleep_data_len++] = 0;
        }
        sleep_data_buf[sleep_data_len++] = 0xFF;
    }
    
    //有效内容的总长度，长度必须是4字节对齐
    if(sleep_data_len & 0x03)
    {
        sleep_data_len = (sleep_data_len/4) * 4 + 4;
    }
    //有效数据CRC总校验,包含填充字节
    crc_total_len = sleep_data_len - SLEEP_BASIC_INFO_OFFSET;    
    crc_value4 = App_Crc16(&sleep_data_buf[SLEEP_BASIC_INFO_OFFSET], crc_total_len);

    //头部信息
    sleep_data_buf[0] = 0xA1;
    sleep_data_buf[1] = sleep_data_len & 0xFF;
    sleep_data_buf[2] = (sleep_data_len >> 8) & 0xFF;
     
    //校验信息
    sleep_data_buf[SLEEP_BASIC_CHECK_OFFSET] = 0xA2;
    sleep_data_buf[SLEEP_BASIC_CHECK_OFFSET + 1] = SLEEP_BASIC_CHECK_LEN;
    
    sleep_data_buf[SLEEP_BASIC_CHECK_OFFSET + 2] = crc_value1 & 0xFF;
    sleep_data_buf[SLEEP_BASIC_CHECK_OFFSET + 3] = (crc_value1>>8) & 0xFF;
    sleep_data_buf[SLEEP_BASIC_CHECK_OFFSET + 4] = crc_value2 & 0xFF;
    sleep_data_buf[SLEEP_BASIC_CHECK_OFFSET + 5] = (crc_value2>>8) & 0xFF;
    sleep_data_buf[SLEEP_BASIC_CHECK_OFFSET + 6] = crc_value3 & 0xFF;
    sleep_data_buf[SLEEP_BASIC_CHECK_OFFSET + 7] = (crc_value3>>8) & 0xFF;
    sleep_data_buf[SLEEP_BASIC_CHECK_OFFSET + 8] = crc_value4 & 0xFF;
    sleep_data_buf[SLEEP_BASIC_CHECK_OFFSET + 9] = (crc_value4>>8) & 0xFF;

    //CRC对比,当前CRC校验和算法校验对比

    
    // 注意:当天11点以后入睡存在当天块
    if((p_sleep_info->enter_time.hour >= 11)&&(p_sleep_info->enter_time.date == clock->day))
    {
        day_id = p_find_index->block_index;
    }
    else
    {   // 否则存在前一天
        day_id = (p_find_index->block_index+2)%3;
    }
    
    //获取当天已存储的睡眠曲线长度
    Api_GetSleepCurveLength(day_id, &sleep_wake_cnt, &sleep_curve_len);
    sleep_curve_len += p_sleep_info->temp_len;    
    
    if(sleep_wake_cnt >= SLEEP_ONE_DAY_MAX_TIMES 
      || (p_sleep_info->temp_len % 2 == 0x01) 
      || sleep_curve_len > 1440 * 2)
    {   // 不能存储，曲线超出24小时之和，当前曲线长度不对
        sleep_wake_cnt++;
        return;
    }
    
    uint8_t error_code = 0;
    error_code = Api_SaveSleepResultData(day_id, sleep_data_buf, sleep_data_len);

    // WJ.20181229.时间过长
    App_Watch_Dog_Reload();
    
    if(error_code == 0)
    { //WJ.20190122.存储成功，设置睡眠下链接标志
        if(p_sleep_info->as_continue_flag == 0x01)
        {
            Api_AlterSleepContinueFlag(day_id, sleep_wake_cnt);
        }        
    }

    // WJ.20181229.时间过长
    App_Watch_Dog_Reload();
}

/****************************************************************
** Function name:             Sleep_LowBateeryProcess
** Descriptions:               
** input parameters:          无
** output parameters:         无
** Returned value:            无
** Created by:                WJ
** Created Date:              2016-11-29
****************************************************************/ 
void Sleep_LowBateeryProcess(void)
{
    uint8_t     daily_id = p_flash_param->flash_index_s.block_index%3;
    uint16_t    current_package = p_flash_param->flash_index_s.daily.offset[daily_id];

    if(p_flash_param->dev_config_s.ppg_detect_enable == 1 /*&& p_flash_param->dev_config_s.precise_sleep == 1*/)
    {   //精准睡眠
        sleep_type = 1;
    }
    else
    {
        sleep_type = 0;
    } 
    // WJ.20190515.分配空间
    acculate_sleep_quality_t *p_sleep_info = NULL;
    p_sleep_info = (acculate_sleep_quality_t *)malloc(sizeof(acculate_sleep_quality_t));
    if(p_sleep_info == NULL)
    {
        return;
    }
    Get_WriteRawPos(p_sleep_info, sizeof(acculate_sleep_quality_t));
    
    //调用睡眠计算，并作相关处理    
    Api_SetSleepBlack(daily_id);
    Acculate_SleepLowBattery(current_package, sleep_type);
    
    // WJ.20190701.算法内部有赋值处理，需要在算法调用完成后再次获取
    Vs_GetSleepingInfo();
    
    if(p_sleep_info->sleep_flag == 3) //退出睡眠       
    {

#if SLEE_RESULT_CHECK_ENABLED           
        uint8_t last_sleep_time[4] = {0};   //最后一次退出睡眠时间-日月时分
        Api_GetSleepTotalTimeNew(last_sleep_time);
        if(Sleep_ValidityCheck((void *)&p_sleep_info->enter_time, last_sleep_time))
        {
#endif            
            Sleep_SaveData(p_sleep_info);//Vs_GetSleepingInfo());

#if SLEE_RESULT_CHECK_ENABLED               
        }
#endif        
        Vs_ClearInfoMsg();        
    } 
    
    p_data_manage->sleep_flag = p_sleep_info->sleep_flag;
    
    // WJ.20190515.释放空间 
    if(p_sleep_info != NULL)
    {
        free(p_sleep_info);
        p_sleep_info = NULL;
    } 
    p_data_manage->sleep_result_s.sleep_all_time = Api_GetSleepTotalTime(&p_data_manage->sleep_result_s.sleep_quality, 
                               &p_data_manage->sleep_result_s.sleep_shallow_time, 
                               &p_data_manage->sleep_result_s.sleep_deep_time);
}

/****************************************************************
** Function name:             Sleep_NormalProcess
** Descriptions:              正常情况下的睡眠处理 
** input parameters:          无
** output parameters:         无
** Returned value:            无
** Created by:                WJ
** Created Date:              2016-11-29
****************************************************************/ 
void Sleep_NormalProcess(void)
{
    uint8_t     daily_id = 0;                           //天序号
    uint16_t    current_package = 0;                    //五分钟数据偏移量  
    
    daily_id = p_flash_param->flash_index_s.block_index % 3; 
    current_package = p_flash_param->flash_index_s.daily.offset[daily_id];
    
    if(p_flash_param->dev_config_s.ppg_detect_enable == 1 /*&& p_flash_param->dev_config_s.precise_sleep == 1*/)
    {   //精准睡眠
        sleep_type = 1;
    }
    else
    {
        sleep_type = 0;
    }
    // WJ.20190515.分配空间
    acculate_sleep_quality_t *p_sleep_info = NULL;
    p_sleep_info = (acculate_sleep_quality_t *)malloc(sizeof(acculate_sleep_quality_t));
    if(p_sleep_info == NULL)
    {
        return;
    }
    Get_WriteRawPos(p_sleep_info, sizeof(acculate_sleep_quality_t));    
    
    //调用睡眠计算，并作相关处理
    Api_SetSleepBlack(daily_id);
    //p_data_manage->daily_attr.sleep_err = 
    Acculate_SleepDetectEntery(current_package, 0, sleep_type);
    
    // WJ.20190701.算法内部有赋值处理，需要在算法调用完成后再次获取
    Vs_GetSleepingInfo();
    
    if(p_sleep_info->sleep_flag == 3)//已经退出睡眠进行存储       
    {
#if SLEE_RESULT_CHECK_ENABLED           
        uint8_t last_sleep_time[4] = {0};   //最后一次退出睡眠时间-日月时分
        Api_GetSleepTotalTimeNew(last_sleep_time);
        if(Sleep_ValidityCheck((void *)&p_sleep_info->enter_time, last_sleep_time))
        {
#endif            
            Sleep_SaveData(p_sleep_info);//Vs_GetSleepingInfo());

#if SLEE_RESULT_CHECK_ENABLED               
        }
#endif        
        Vs_ClearInfoMsg();        
    } 
    
    p_data_manage->sleep_flag = p_sleep_info->sleep_flag;
    
    // WJ.20190515.释放空间    
    if(p_sleep_info != NULL)
    {
        free(p_sleep_info);
        p_sleep_info = NULL;
    }
    
    p_data_manage->sleep_result_s.sleep_all_time = Api_GetSleepTotalTime(&p_data_manage->sleep_result_s.sleep_quality, 
                               &p_data_manage->sleep_result_s.sleep_shallow_time, 
                               &p_data_manage->sleep_result_s.sleep_deep_time);
}

/****************************************************************
** Function name:           Sleep_ValidityCheck
** Descriptions:            入睡时间需要大于最后一次的退出睡眠时间
** input parameters:        curr_sleep_time：当前入睡时间；last_sleep_time：最后一次退出睡眠时间
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2018-12-11
*****************************************************************/ 
static bool Sleep_ValidityCheck(uint8_t *curr_sleep_time, uint8_t *last_sleep_time)
{
    if(last_sleep_time[0] == 0 && last_sleep_time[1] == 0 
       && last_sleep_time[2] == 0 && last_sleep_time[3] == 0)
    {
        return true;
    }
    
    if(   curr_sleep_time[0] > last_sleep_time[0]
      || (curr_sleep_time[0] == last_sleep_time[0] && curr_sleep_time[1] > last_sleep_time[1]) 
      || (curr_sleep_time[0] == last_sleep_time[0] && curr_sleep_time[1] == last_sleep_time[1]
         && curr_sleep_time[2]*60 + curr_sleep_time[3] >= last_sleep_time[2]*60 + last_sleep_time[3]))
    {
        return true;
    }
    
    if(  curr_sleep_time[0] < last_sleep_time[0] 
      && curr_sleep_time[0] == 1 && last_sleep_time[0] == 12
      && curr_sleep_time[1] == 1 && last_sleep_time[2] == 31)
    {
        return true;
    }
    
    return false;
}


/****************************************************************
** Function name:           Sleep_TraversingOperationByRest
** Descriptions:            睡眠遍历，
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2018-12-11
*****************************************************************/ 
void Sleep_TraversingOperationByRest(void)
{
    uint8_t daily_id = p_flash_param->flash_index_s.block_index%3;
    uint16_t current_package = p_flash_param->flash_index_s.daily.offset[daily_id];
    
    uint32_t daily_data[3] = {0};
    uint8_t* p_daily_data = (uint8_t*)daily_data;
    
    uint8_t traver_sleep_block = 0;
    uint16_t daily_pkg = 0;
    uint16_t daily_max_pkg = 0;
    uint8_t y_month = 0;
    uint8_t y_day = 0; 
    
    traver_sleep_block = (daily_id + 2)%3;  //得到昨天的快号
    daily_max_pkg = p_flash_param->flash_index_s.daily.offset[traver_sleep_block];
    Sclock_GetPreviousDayDate(1, &y_month, &y_day); //获取昨天的月份和日期
    
    if(daily_max_pkg != 0)
    {
        //读取昨天的快号中数据
        Flash_CommonRead(daily_data, RECORD_DAILY_DATA_ADDR(traver_sleep_block,0), 9);
    }
    
    if(daily_max_pkg == 0 || p_daily_data[0] != y_month || p_daily_data[1] != y_day)
    {
        //昨天不存在数据，睡眠遍历从今天的数据开始
        daily_pkg = 0;
        traver_sleep_block = daily_id;
        daily_max_pkg = p_flash_param->flash_index_s.daily.offset[traver_sleep_block];
        Api_SetSleepBlack(traver_sleep_block);
    }
    else
    {
        if(daily_max_pkg >= 48)
        {
            //48 = 12 * 4,代表从昨天晚上8点开始遍历
            daily_pkg = daily_max_pkg - 48 + 1;
        }
        else
        {
            daily_pkg = 1;
        }
        Api_SetSleepBlack(traver_sleep_block);
    }
    // WJ.20190515.分配空间
    acculate_sleep_quality_t *p_sleep_info = NULL;
    p_sleep_info = (acculate_sleep_quality_t *)malloc(sizeof(acculate_sleep_quality_t));
    if(p_sleep_info == NULL)
    {
        return;
    }
    Get_WriteRawPos(p_sleep_info, sizeof(acculate_sleep_quality_t));
    
    // 1.清空睡眠缓存数据
    Vs_ClearInfoMsg();  

    if(p_flash_param->dev_config_s.ppg_detect_enable == 1 /*&& p_flash_param->dev_config_s.precise_sleep == 1*/)
    {   //精准睡眠
        sleep_type = 1;
    }
    else
    {
        sleep_type = 0;
    }    
    do{
        // 2.调用睡眠计算，遍历处理(前一天20:00---当前时间)
        for(; daily_pkg <= daily_max_pkg; daily_pkg++)
        {
            Acculate_SleepDetectEntery(daily_pkg, 0, sleep_type);
    
            // WJ.20190701.算法内部有赋值处理，需要在算法调用完成后再次获取
            Vs_GetSleepingInfo();
    
            if(p_sleep_info->sleep_flag == 3)        
            {
                uint8_t last_sleep_time[4] = {0};   //最后一次退出睡眠时间-日月时分
                Api_GetSleepTotalTimeNew(last_sleep_time, NULL, NULL, NULL);
                if(Sleep_ValidityCheck((void *)&p_sleep_info->enter_time, last_sleep_time))
                {
                    Sleep_SaveData(p_sleep_info);//Vs_GetSleepingInfo());
                }
                Vs_ClearInfoMsg();        
            } 
            // WJ.20181229.时间过长
            App_Watch_Dog_Reload();
        }
        if(traver_sleep_block != daily_id)
        {
            traver_sleep_block = daily_id;
            Api_SetSleepBlack(traver_sleep_block);
            daily_max_pkg = current_package;
            daily_pkg = 1;
        }
        else
        {
            break;
        }
    }while(1);
      
    p_data_manage->sleep_flag = p_sleep_info->sleep_flag;
    
    // WJ.20190515.释放空间    
    if(p_sleep_info != NULL)
    {
        free(p_sleep_info);
        p_sleep_info = NULL;
    }
    
    Api_SetSleepBlack(daily_id);
    
    
    p_data_manage->sleep_result_s.sleep_all_time = Api_GetSleepTotalTime(&p_data_manage->sleep_result_s.sleep_quality, 
                               &p_data_manage->sleep_result_s.sleep_shallow_time, 
                               &p_data_manage->sleep_result_s.sleep_deep_time);
    
}

/****************************************************************
** Function name:             Sleep_TraversingOperation
** Descriptions:              睡眠遍历操作 
** input parameters:          无
** output parameters:         无
** Returned value:            无
** Created by:                GT
** Created Date:              2017-10-11
****************************************************************/ 
void Sleep_TraversingOperation(void)
{
    uint8_t daily_id = p_flash_param->flash_index_s.block_index%3;
    uint16_t current_package = p_flash_param->flash_index_s.daily.offset[daily_id];
    
    uint32_t daily_data[3] = {0};
    uint8_t* p_daily_data = (uint8_t*)daily_data;
    
    uint8_t traver_sleep_block = 0;
    uint16_t daily_pkg = 0;
    uint16_t daily_max_pkg = 0;
    uint8_t y_month = 0;
    uint8_t y_day = 0; 
    
    traver_sleep_block = (daily_id + 2)%3;  //得到昨天的快号
    daily_max_pkg = p_flash_param->flash_index_s.daily.offset[traver_sleep_block];
    Sclock_GetPreviousDayDate(1, &y_month, &y_day); //获取昨天的月份和日期
    
    if(daily_max_pkg != 0)
    {
        //读取昨天的快号中数据
        Flash_CommonRead(daily_data, RECORD_DAILY_DATA_ADDR(traver_sleep_block,0), 9);
    }
    
    if(daily_max_pkg == 0 || p_daily_data[0] != y_month || p_daily_data[1] != y_day)
    {
        //昨天不存在数据，睡眠遍历从今天的数据开始
        daily_pkg = 0;
        traver_sleep_block = daily_id;
        daily_max_pkg = p_flash_param->flash_index_s.daily.offset[traver_sleep_block];
        Api_SetSleepBlack(traver_sleep_block);
    }
    else
    {
        if(daily_max_pkg >= 48)
        {
            //48 = 12 * 4,代表从昨天晚上8点开始遍历
            daily_pkg = daily_max_pkg - 48 + 1;
        }
        else
        {
            daily_pkg = 1;
        }
        Api_SetSleepBlack(traver_sleep_block);
    }
    // WJ.20190515.分配空间
    acculate_sleep_quality_t *p_sleep_info = NULL;
    p_sleep_info = (acculate_sleep_quality_t *)malloc(sizeof(acculate_sleep_quality_t));
    if(p_sleep_info == NULL)
    {
        return;
    }
    Get_WriteRawPos(p_sleep_info, sizeof(acculate_sleep_quality_t));
    
    // 1.清空睡眠缓存数据
    Vs_ClearInfoMsg();  

    if(p_flash_param->dev_config_s.ppg_detect_enable == 1 /*&& p_flash_param->dev_config_s.precise_sleep == 1*/)
    {   //精准睡眠
        sleep_type = 1;
    }
    else
    {
        sleep_type = 0;
    }    
    do{
        // 2.调用睡眠计算，遍历处理(前一天20:00---当前时间)
        for(; daily_pkg <= daily_max_pkg; daily_pkg++)
        {
            Acculate_SleepDetectEntery(daily_pkg, 0, sleep_type);
    
            // WJ.20190701.算法内部有赋值处理，需要在算法调用完成后再次获取
            Vs_GetSleepingInfo();
    
            if(p_sleep_info->sleep_flag == 3)        
            {
                Sleep_SaveData(p_sleep_info);//Vs_GetSleepingInfo());
                Vs_ClearInfoMsg();        
            } 
            // WJ.20181229.时间过长
            App_Watch_Dog_Reload();
        }
        if(traver_sleep_block != daily_id)
        {
            traver_sleep_block = daily_id;
            Api_SetSleepBlack(traver_sleep_block);
            daily_max_pkg = current_package;
            daily_pkg = 1;
        }
        else
        {
            break;
        }
    }while(1);
    
    p_data_manage->sleep_flag = p_sleep_info->sleep_flag;
    
    // WJ.20190515.释放空间    
    if(p_sleep_info != NULL)
    {
        free(p_sleep_info);
        p_sleep_info = NULL;
    }
    
    Api_SetSleepBlack(daily_id);
    
    p_data_manage->sleep_result_s.sleep_all_time = Api_GetSleepTotalTime(&p_data_manage->sleep_result_s.sleep_quality, 
                               &p_data_manage->sleep_result_s.sleep_shallow_time, 
                               &p_data_manage->sleep_result_s.sleep_deep_time);
}


void Sleep_DebugOverloadData(void)
{
//    //1.擦除现有数据
//    Api_FlashFatoryRestore(); 
//    
//    //2.写入虚拟数据
//    Sleep_OverloadVirtualData();

//    //3.遍历
//    Sleep_TraversingOperation();
};

/****************************************************************
** Function name:             Sleep_Init
** Descriptions:              睡眠初始化 
** input parameters:          无
** output parameters:         无
** Returned value:            无
** Created by:                WJ
** Created Date:              2016-11-29
****************************************************************/ 
void Sleep_Init(void)
{    
    Acculate_SleepingInit(Api_SleepCallback);
    Vs_ClearInfoMsg();       
}



#if SLEEP_DEBUG 


const uint8_t sleep_test_data[431][20] = {
 7, 3, 15, 0, 0, 62, 0, 37, 0, 53, 128, 80, 112, 80, 128, 0, 0, 0, 0, 0
, 7, 3, 15, 5, 0, 42, 0, 45, 0, 64, 98, 96, 96, 96, 96, 0, 0, 0, 0, 0
, 7, 3, 15, 10, 0, 103, 0, 73, 0, 64, 112, 96, 96, 96, 96, 0, 0, 0, 0, 0
, 7, 3, 15, 15, 0, 61, 0, 34, 0, 74, 116, 96, 96, 96, 128, 0, 0, 0, 0, 0
, 7, 3, 15, 20, 0, 25, 0, 42, 0, 64, 128, 128, 100, 128, 96, 0, 0, 0, 0, 0
, 7, 3, 15, 25, 0, 0, 0, 22, 0, 64, 128, 130, 98, 96, 128, 0, 0, 0, 0, 0
, 7, 3, 15, 30, 0, 14, 0, 18, 0, 53, 128, 116, 100, 96, 96, 0, 0, 0, 0, 0
, 7, 3, 15, 35, 0, 0, 0, 5, 0, 64, 98, 104, 80, 116, 112, 0, 0, 0, 0, 0
, 7, 3, 15, 40, 0, 49, 0, 47, 0, 64, 108, 96, 128, 112, 96, 0, 0, 0, 0, 0
, 7, 3, 15, 45, 0, 0, 0, 19, 0, 64, 96, 98, 96, 98, 98, 0, 0, 0, 0, 0
, 7, 3, 15, 50, 0, 10, 0, 39, 0, 64, 96, 102, 102, 96, 128, 0, 0, 0, 0, 0
, 7, 3, 15, 55, 0, 0, 0, 11, 0, 53, 128, 104, 112, 28, 100, 0, 0, 0, 0, 0
, 7, 3, 16, 0, 0, 0, 0, 27, 0, 64, 134, 100, 100, 112, 96, 4, 0, 0, 0, 0
, 7, 3, 16, 5, 0, 0, 0, 8, 0, 53, 96, 80, 96, 80, 16, 0, 0, 0, 0, 0
, 7, 3, 16, 10, 0, 0, 0, 14, 0, 64, 96, 102, 96, 128, 0, 16, 0, 0, 0, 0
, 7, 3, 16, 15, 0, 10, 0, 10, 0, 53, 96, 100, 80, 96, 104, 16, 0, 0, 0, 0
, 7, 3, 16, 20, 0, 12, 0, 12, 0, 53, 80, 102, 112, 96, 112, 0, 0, 0, 0, 0
, 7, 3, 16, 25, 0, 0, 0, 13, 0, 53, 96, 132, 112, 80, 96, 0, 0, 0, 0, 0
, 7, 3, 16, 30, 0, 0, 0, 13, 0, 53, 96, 104, 84, 96, 96, 0, 0, 0, 0, 0
, 7, 3, 16, 35, 0, 12, 0, 19, 0, 73, 96, 116, 96, 100, 96, 8, 0, 0, 0, 0
, 7, 3, 16, 40, 0, 0, 0, 8, 0, 74, 96, 96, 96, 104, 96, 0, 0, 0, 0, 0
, 7, 3, 16, 45, 0, 0, 0, 13, 0, 74, 96, 128, 108, 96, 96, 0, 0, 0, 0, 0
, 7, 3, 16, 50, 0, 0, 0, 10, 0, 74, 96, 96, 112, 112, 96, 0, 0, 0, 0, 0
, 7, 3, 16, 55, 0, 10, 0, 16, 0, 74, 96, 96, 96, 98, 96, 0, 0, 0, 0, 0
, 7, 3, 17, 0, 0, 0, 0, 13, 0, 74, 96, 96, 104, 112, 132, 0, 0, 0, 0, 0
, 7, 3, 17, 5, 0, 0, 0, 19, 0, 64, 130, 130, 96, 128, 128, 0, 0, 0, 0, 0
, 7, 3, 17, 10, 0, 11, 0, 20, 0, 64, 48, 96, 128, 96, 100, 0, 0, 0, 0, 0
, 7, 3, 17, 15, 0, 0, 0, 29, 0, 64, 134, 96, 132, 130, 98, 0, 0, 0, 0, 0
, 7, 3, 17, 20, 0, 10, 0, 33, 0, 64, 128, 130, 128, 96, 100, 0, 0, 0, 0, 0
, 7, 3, 17, 25, 0, 0, 0, 20, 0, 64, 128, 96, 112, 96, 128, 0, 0, 0, 0, 0
, 7, 3, 17, 30, 0, 0, 0, 8, 0, 53, 128, 96, 128, 128, 128, 0, 0, 0, 0, 0
, 7, 3, 17, 35, 0, 22, 0, 42, 0, 53, 128, 108, 128, 128, 96, 0, 0, 0, 0, 0
, 7, 3, 17, 40, 0, 14, 0, 44, 0, 64, 128, 130, 128, 130, 130, 0, 0, 0, 0, 0
, 7, 3, 17, 45, 0, 40, 0, 54, 0, 64, 128, 128, 128, 96, 130, 0, 0, 0, 0, 0
, 7, 3, 17, 50, 0, 0, 0, 10, 0, 74, 112, 96, 112, 98, 80, 0, 0, 0, 0, 0
, 7, 3, 17, 55, 0, 46, 0, 31, 0, 53, 112, 104, 80, 128, 102, 0, 0, 0, 0, 0
, 7, 3, 18, 0, 0, 0, 0, 9, 0, 54, 112, 96, 98, 96, 112, 0, 0, 0, 0, 0
, 7, 3, 18, 5, 0, 0, 0, 15, 0, 64, 96, 96, 144, 102, 96, 0, 0, 0, 0, 0
, 7, 3, 18, 10, 0, 14, 0, 14, 0, 64, 112, 132, 144, 112, 130, 0, 0, 0, 0, 0
, 7, 3, 18, 15, 0, 33, 0, 65, 0, 64, 128, 96, 128, 96, 128, 0, 0, 0, 0, 0
, 7, 3, 18, 20, 0, 0, 0, 5, 0, 62, 98, 80, 80, 112, 80, 0, 0, 0, 0, 0
, 7, 3, 18, 25, 0, 0, 0, 2, 0, 64, 112, 112, 80, 116, 112, 0, 0, 0, 0, 0
, 7, 3, 18, 30, 0, 0, 0, 9, 0, 62, 112, 112, 100, 96, 96, 0, 0, 0, 0, 0
, 7, 3, 18, 35, 0, 0, 0, 11, 0, 74, 116, 134, 0, 96, 100, 4, 0, 0, 0, 0
, 7, 3, 18, 40, 0, 0, 0, 8, 0, 74, 144, 98, 98, 80, 80, 0, 0, 0, 0, 0
, 7, 3, 18, 45, 0, 38, 0, 53, 0, 64, 128, 130, 112, 130, 96, 0, 0, 0, 0, 0
, 7, 3, 18, 50, 0, 0, 0, 10, 6, 74, 0, 0, 0, 0, 96, 255, 0, 0, 0, 0
, 7, 3, 18, 55, 0, 0, 0, 0, 6, 74, 0, 0, 0, 0, 0, 255, 0, 0, 0, 0
, 7, 3, 19, 0, 0, 0, 0, 0, 6, 74, 0, 0, 0, 0, 0, 255, 0, 0, 0, 0
, 7, 3, 19, 5, 0, 0, 0, 3, 6, 74, 0, 0, 0, 0, 0, 255, 0, 0, 0, 0
, 7, 3, 19, 10, 0, 0, 0, 12, 0, 74, 0, 0, 0, 0, 0, 255, 0, 0, 0, 0
, 7, 3, 19, 15, 0, 0, 0, 18, 1, 74, 96, 112, 112, 100, 96, 0, 0, 0, 0, 0
, 7, 3, 19, 20, 0, 14, 0, 21, 1, 72, 128, 96, 104, 128, 134, 0, 0, 0, 0, 0
, 7, 3, 19, 25, 0, 145, 0, 85, 0, 64, 96, 130, 100, 96, 96, 0, 0, 0, 0, 0
, 7, 3, 19, 30, 1, 156, 0, 233, 0, 54, 96, 96, 96, 112, 96, 0, 0, 0, 0, 0
, 7, 3, 19, 35, 1, 227, 0, 201, 0, 64, 96, 96, 96, 112, 116, 0, 0, 0, 0, 0
, 7, 3, 19, 40, 0, 88, 0, 47, 0, 54, 112, 128, 96, 130, 96, 0, 0, 0, 0, 0
, 7, 3, 19, 45, 0, 0, 0, 9, 0, 54, 112, 112, 80, 100, 130, 0, 0, 0, 0, 0
, 7, 3, 19, 50, 0, 0, 0, 28, 0, 52, 114, 112, 100, 98, 102, 0, 0, 0, 0, 0
, 7, 3, 19, 55, 0, 33, 0, 68, 0, 64, 98, 96, 96, 130, 112, 0, 0, 0, 0, 0
, 7, 3, 20, 0, 0, 0, 0, 20, 0, 64, 104, 96, 130, 96, 130, 0, 0, 0, 0, 0
, 7, 3, 20, 5, 0, 0, 0, 15, 0, 64, 96, 130, 96, 98, 98, 0, 0, 0, 0, 0
, 7, 3, 20, 10, 0, 0, 0, 7, 0, 64, 98, 102, 96, 98, 98, 0, 0, 0, 0, 0
, 7, 3, 20, 15, 0, 0, 0, 18, 0, 64, 102, 96, 102, 98, 96, 0, 0, 0, 0, 0
, 7, 3, 20, 20, 0, 0, 0, 28, 0, 64, 98, 98, 84, 96, 96, 0, 0, 0, 0, 0
, 7, 3, 20, 25, 0, 0, 0, 25, 0, 64, 96, 100, 102, 96, 100, 0, 0, 0, 0, 0
, 7, 3, 20, 30, 0, 0, 0, 22, 0, 64, 96, 100, 98, 96, 100, 0, 0, 0, 0, 0
, 7, 3, 20, 35, 0, 0, 0, 18, 0, 64, 96, 98, 96, 100, 98, 0, 0, 0, 0, 0
, 7, 3, 20, 40, 0, 0, 0, 28, 0, 64, 102, 100, 98, 118, 100, 0, 0, 0, 0, 0
, 7, 3, 20, 45, 0, 238, 0, 154, 0, 64, 112, 96, 96, 114, 80, 0, 0, 0, 0, 0
, 7, 3, 20, 50, 0, 123, 0, 103, 0, 54, 96, 112, 96, 102, 130, 0, 0, 0, 0, 0
, 7, 3, 20, 55, 0, 22, 0, 51, 0, 52, 128, 112, 0, 84, 130, 4, 0, 0, 0, 0
, 7, 3, 21, 0, 0, 0, 0, 0, 1, 72, 0, 0, 0, 0, 0, 255, 0, 0, 0, 0
, 7, 3, 21, 5, 0, 0, 0, 0, 1, 72, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
, 7, 3, 21, 10, 0, 0, 0, 0, 1, 72, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
, 7, 3, 21, 15, 0, 0, 0, 0, 1, 72, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
, 7, 3, 21, 20, 0, 0, 0, 0, 1, 72, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
, 7, 3, 21, 25, 0, 0, 0, 0, 1, 72, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
, 7, 3, 21, 30, 0, 0, 0, 0, 2, 72, 0, 0, 0, 0, 0, 255, 0, 0, 0, 0
, 7, 3, 21, 35, 0, 0, 0, 0, 2, 72, 0, 0, 0, 0, 0, 255, 0, 0, 0, 0
, 7, 3, 21, 40, 0, 0, 0, 0, 2, 72, 0, 0, 0, 0, 0, 255, 0, 0, 0, 0
, 7, 3, 21, 45, 0, 0, 0, 0, 2, 72, 0, 0, 0, 0, 0, 255, 0, 0, 0, 0
, 7, 3, 21, 50, 0, 0, 0, 0, 2, 72, 0, 0, 0, 0, 0, 255, 0, 0, 0, 0
, 7, 3, 21, 55, 0, 0, 0, 0, 2, 72, 0, 0, 0, 0, 0, 255, 0, 0, 0, 0
, 7, 3, 22, 0, 0, 0, 0, 0, 2, 72, 0, 0, 0, 0, 0, 255, 0, 0, 0, 0
, 7, 3, 22, 5, 0, 0, 0, 0, 2, 72, 0, 0, 0, 0, 0, 255, 0, 0, 0, 0
, 7, 3, 22, 10, 0, 0, 0, 0, 2, 72, 0, 0, 0, 0, 0, 255, 0, 0, 0, 0
, 7, 3, 22, 15, 0, 0, 0, 0, 2, 72, 0, 0, 0, 0, 0, 255, 0, 0, 0, 0
, 7, 3, 22, 20, 0, 0, 0, 0, 2, 72, 0, 0, 0, 0, 0, 255, 0, 0, 0, 0
, 7, 3, 22, 25, 0, 0, 0, 0, 2, 72, 0, 0, 0, 0, 0, 255, 0, 0, 0, 0
, 7, 3, 22, 30, 0, 0, 0, 0, 2, 72, 0, 0, 0, 0, 0, 255, 0, 0, 0, 0
, 7, 3, 22, 35, 0, 0, 0, 0, 2, 72, 0, 0, 0, 0, 0, 255, 0, 0, 0, 0
, 7, 3, 22, 40, 0, 0, 0, 0, 2, 72, 0, 0, 0, 0, 0, 255, 0, 0, 0, 0
, 7, 3, 22, 45, 0, 0, 0, 0, 2, 72, 0, 0, 0, 0, 0, 255, 0, 0, 0, 0
, 7, 3, 22, 50, 0, 0, 0, 0, 2, 72, 0, 0, 0, 0, 0, 255, 0, 0, 0, 0
, 7, 3, 22, 55, 0, 0, 0, 0, 2, 72, 0, 0, 0, 0, 0, 255, 0, 0, 0, 0
, 7, 3, 23, 0, 0, 0, 0, 0, 2, 72, 0, 0, 0, 0, 0, 255, 0, 0, 0, 0
, 7, 3, 23, 5, 0, 0, 0, 2, 2, 72, 0, 0, 0, 0, 0, 255, 0, 0, 0, 0
, 7, 3, 23, 10, 0, 0, 0, 0, 2, 72, 0, 0, 0, 0, 0, 255, 0, 0, 0, 0
, 7, 3, 23, 15, 0, 0, 0, 0, 2, 72, 0, 0, 0, 0, 0, 255, 0, 0, 0, 0
, 7, 3, 23, 20, 0, 0, 0, 0, 2, 72, 0, 0, 0, 0, 0, 255, 0, 0, 0, 0
, 7, 3, 23, 25, 0, 0, 0, 0, 2, 72, 0, 0, 0, 0, 0, 255, 0, 0, 0, 0
, 7, 3, 23, 30, 0, 0, 0, 0, 2, 72, 0, 0, 0, 0, 0, 255, 0, 0, 0, 0
, 7, 3, 23, 35, 0, 0, 0, 0, 2, 72, 0, 0, 0, 0, 0, 255, 0, 0, 0, 0
, 7, 3, 23, 40, 0, 0, 0, 0, 2, 72, 0, 0, 0, 0, 0, 255, 0, 0, 0, 0
, 7, 3, 23, 45, 0, 0, 0, 0, 2, 72, 0, 0, 0, 0, 0, 255, 0, 0, 0, 0
, 7, 3, 23, 50, 0, 0, 0, 0, 2, 72, 0, 0, 0, 0, 0, 255, 0, 0, 0, 0
, 7, 3, 23, 55, 0, 0, 0, 0, 2, 72, 0, 0, 0, 0, 0, 255, 0, 0, 0, 0
, 7, 4, 0, 0, 0, 0, 0, 0, 2, 72, 0, 0, 0, 0, 0, 255, 0, 0, 0, 0
, 7, 4, 0, 5, 0, 0, 0, 0, 2, 72, 0, 0, 0, 0, 0, 255, 0, 0, 0, 0
, 7, 4, 0, 10, 0, 0, 0, 0, 2, 72, 0, 0, 0, 0, 0, 255, 0, 0, 0, 0
, 7, 4, 0, 15, 0, 0, 0, 2, 2, 72, 0, 0, 0, 0, 0, 255, 0, 0, 0, 0
, 7, 4, 0, 20, 0, 0, 0, 0, 2, 72, 0, 0, 0, 0, 0, 255, 0, 0, 0, 0
, 7, 4, 0, 25, 0, 0, 0, 0, 2, 72, 0, 0, 0, 0, 0, 255, 0, 0, 0, 0
, 7, 4, 0, 30, 0, 0, 0, 0, 2, 72, 0, 0, 0, 0, 0, 255, 0, 0, 0, 0
, 7, 4, 0, 35, 0, 0, 0, 0, 2, 72, 0, 0, 0, 0, 0, 255, 0, 0, 0, 0
, 7, 4, 0, 40, 0, 0, 0, 0, 2, 72, 0, 0, 0, 0, 0, 255, 0, 0, 0, 0
, 7, 4, 0, 45, 0, 0, 0, 0, 2, 72, 0, 0, 0, 0, 0, 255, 0, 0, 0, 0
, 7, 4, 0, 50, 0, 0, 0, 0, 2, 72, 0, 0, 0, 0, 0, 255, 0, 0, 0, 0
, 7, 4, 0, 55, 0, 0, 0, 0, 2, 72, 0, 0, 0, 0, 0, 255, 0, 0, 0, 0
, 7, 4, 1, 0, 0, 0, 0, 0, 2, 72, 0, 0, 0, 0, 0, 255, 0, 0, 0, 0
, 7, 4, 1, 5, 0, 0, 0, 0, 2, 72, 0, 0, 0, 0, 0, 255, 0, 0, 0, 0
, 7, 4, 1, 10, 0, 0, 0, 0, 2, 72, 0, 0, 0, 0, 0, 255, 0, 0, 0, 0
, 7, 4, 1, 15, 0, 0, 0, 0, 2, 72, 0, 0, 0, 0, 0, 255, 0, 0, 0, 0
, 7, 4, 1, 20, 0, 0, 0, 0, 2, 72, 0, 0, 0, 0, 0, 255, 0, 0, 0, 0
, 7, 4, 1, 25, 0, 0, 0, 0, 2, 72, 0, 0, 0, 0, 0, 255, 0, 0, 0, 0
, 7, 4, 1, 30, 0, 0, 0, 0, 2, 72, 0, 0, 0, 0, 0, 255, 0, 0, 0, 0
, 7, 4, 1, 35, 0, 0, 0, 0, 2, 72, 0, 0, 0, 0, 0, 255, 0, 0, 0, 0
, 7, 4, 1, 40, 0, 0, 0, 0, 2, 72, 0, 0, 0, 0, 0, 255, 0, 0, 0, 0
, 7, 4, 1, 45, 0, 0, 0, 0, 2, 72, 0, 0, 0, 0, 0, 255, 0, 0, 0, 0
, 7, 4, 1, 50, 0, 17, 0, 28, 2, 64, 0, 0, 0, 0, 0, 255, 0, 0, 0, 0
, 7, 4, 1, 55, 0, 0, 0, 3, 0, 53, 0, 0, 0, 0, 0, 255, 0, 0, 0, 0
, 7, 4, 2, 0, 0, 10, 0, 13, 0, 53, 96, 80, 104, 128, 128, 0, 0, 0, 0, 0
, 7, 4, 2, 5, 0, 0, 0, 0, 0, 53, 130, 96, 152, 92, 128, 0, 0, 0, 0, 0
, 7, 4, 2, 10, 0, 0, 0, 5, 0, 64, 112, 80, 24, 24, 24, 0, 0, 0, 0, 0
, 7, 4, 2, 15, 0, 0, 0, 0, 0, 64, 24, 24, 16, 24, 8, 0, 0, 0, 0, 0
, 7, 4, 2, 20, 0, 0, 0, 0, 0, 64, 16, 16, 24, 16, 28, 0, 0, 0, 0, 0
, 7, 4, 2, 25, 0, 0, 0, 1, 0, 64, 24, 24, 112, 24, 24, 0, 0, 0, 0, 0
, 7, 4, 2, 30, 0, 0, 0, 0, 0, 64, 24, 16, 24, 24, 24, 0, 0, 0, 0, 0
, 7, 4, 2, 35, 0, 0, 0, 1, 0, 64, 24, 24, 24, 8, 24, 0, 0, 0, 0, 0
, 7, 4, 2, 40, 0, 0, 0, 0, 0, 64, 16, 24, 24, 24, 24, 0, 0, 0, 0, 0
, 7, 4, 2, 45, 0, 0, 0, 0, 0, 64, 24, 24, 24, 24, 24, 0, 0, 0, 0, 0
, 7, 4, 2, 50, 0, 0, 0, 0, 0, 64, 24, 24, 16, 24, 24, 0, 0, 0, 0, 0
, 7, 4, 2, 55, 0, 0, 0, 0, 0, 64, 24, 48, 24, 16, 24, 0, 0, 0, 0, 0
, 7, 4, 3, 0, 0, 0, 0, 0, 0, 64, 16, 16, 16, 24, 16, 0, 0, 0, 0, 0
, 7, 4, 3, 5, 0, 0, 0, 0, 0, 64, 24, 16, 24, 24, 84, 0, 0, 0, 0, 0
, 7, 4, 3, 10, 0, 0, 0, 0, 0, 64, 20, 24, 16, 24, 16, 0, 0, 0, 0, 0
, 7, 4, 3, 15, 0, 0, 0, 0, 0, 64, 16, 80, 16, 24, 24, 0, 0, 0, 0, 0
, 7, 4, 3, 20, 0, 0, 0, 0, 0, 64, 24, 24, 24, 28, 24, 0, 0, 0, 0, 0
, 7, 4, 3, 25, 0, 0, 0, 0, 0, 64, 16, 24, 24, 24, 16, 0, 0, 0, 0, 0
, 7, 4, 3, 30, 0, 0, 0, 0, 0, 53, 112, 24, 16, 16, 16, 0, 0, 0, 0, 0
, 7, 4, 3, 35, 0, 0, 0, 0, 0, 64, 16, 112, 24, 24, 16, 0, 0, 0, 0, 0
, 7, 4, 3, 40, 0, 0, 0, 0, 0, 64, 24, 24, 24, 24, 16, 0, 0, 0, 0, 0
, 7, 4, 3, 45, 0, 0, 0, 0, 0, 64, 16, 24, 24, 24, 16, 0, 0, 0, 0, 0
, 7, 4, 3, 50, 0, 0, 0, 0, 0, 64, 16, 24, 16, 16, 16, 0, 0, 0, 0, 0
, 7, 4, 3, 55, 0, 0, 0, 0, 0, 64, 16, 24, 24, 16, 48, 0, 0, 0, 0, 0
, 7, 4, 4, 0, 0, 0, 0, 0, 0, 64, 16, 16, 24, 16, 16, 0, 0, 0, 0, 0
, 7, 4, 4, 5, 0, 0, 0, 0, 0, 64, 16, 20, 16, 16, 16, 0, 0, 0, 0, 0
, 7, 4, 4, 10, 0, 0, 0, 0, 0, 64, 24, 8, 24, 16, 116, 0, 0, 0, 0, 0
, 7, 4, 4, 15, 0, 0, 0, 0, 0, 64, 84, 8, 16, 80, 16, 0, 0, 0, 0, 0
, 7, 4, 4, 20, 0, 0, 0, 0, 0, 64, 16, 16, 16, 16, 16, 0, 0, 0, 0, 0
, 7, 4, 4, 25, 0, 0, 0, 0, 0, 64, 80, 16, 24, 16, 16, 0, 0, 0, 0, 0
, 7, 4, 4, 30, 0, 0, 0, 0, 0, 64, 80, 16, 16, 16, 112, 0, 0, 0, 0, 0
, 7, 4, 4, 35, 0, 0, 0, 0, 0, 64, 80, 80, 80, 16, 80, 0, 0, 0, 0, 0
, 7, 4, 4, 40, 0, 0, 0, 0, 0, 64, 80, 80, 80, 16, 80, 0, 0, 0, 0, 0
, 7, 4, 4, 45, 0, 0, 0, 0, 0, 64, 80, 80, 80, 80, 16, 0, 0, 0, 0, 0
, 7, 4, 4, 50, 0, 0, 0, 0, 0, 64, 16, 80, 80, 16, 80, 0, 0, 0, 0, 0
, 7, 4, 4, 55, 0, 0, 0, 0, 0, 64, 80, 80, 80, 16, 80, 0, 0, 0, 0, 0
, 7, 4, 5, 0, 0, 0, 0, 0, 0, 64, 16, 16, 16, 16, 16, 0, 0, 0, 0, 0
, 7, 4, 5, 5, 0, 0, 0, 0, 0, 64, 16, 16, 112, 16, 16, 0, 0, 0, 0, 0
, 7, 4, 5, 10, 0, 0, 0, 0, 0, 64, 16, 80, 16, 16, 80, 0, 0, 0, 0, 0
, 7, 4, 5, 15, 0, 0, 0, 0, 0, 64, 20, 20, 16, 80, 16, 0, 0, 0, 0, 0
, 7, 4, 5, 20, 0, 0, 0, 0, 0, 64, 16, 80, 16, 8, 96, 0, 0, 0, 0, 0
, 7, 4, 5, 25, 0, 0, 0, 0, 0, 64, 80, 80, 112, 80, 80, 0, 0, 0, 0, 0
, 7, 4, 5, 30, 0, 0, 0, 2, 0, 64, 84, 80, 128, 16, 80, 0, 0, 0, 0, 0
, 7, 4, 5, 35, 0, 0, 0, 0, 0, 64, 80, 8, 16, 16, 16, 0, 0, 0, 0, 0
, 7, 4, 5, 40, 0, 0, 0, 0, 0, 64, 24, 88, 80, 80, 16, 0, 0, 0, 0, 0
, 7, 4, 5, 45, 0, 0, 0, 0, 0, 64, 16, 80, 80, 48, 80, 0, 0, 0, 0, 0
, 7, 4, 5, 50, 0, 0, 0, 0, 0, 64, 112, 146, 80, 16, 16, 0, 0, 0, 0, 0
, 7, 4, 5, 55, 0, 0, 0, 0, 0, 64, 16, 80, 16, 24, 16, 0, 0, 0, 0, 0
, 7, 4, 6, 0, 0, 0, 0, 0, 0, 64, 16, 16, 80, 16, 16, 0, 0, 0, 0, 0
, 7, 4, 6, 5, 0, 0, 0, 0, 0, 64, 16, 8, 16, 16, 16, 0, 0, 0, 0, 0
, 7, 4, 6, 10, 0, 0, 0, 0, 0, 64, 16, 24, 16, 24, 24, 0, 0, 0, 0, 0
, 7, 4, 6, 15, 0, 0, 0, 0, 0, 64, 16, 24, 16, 16, 16, 0, 0, 0, 0, 0
, 7, 4, 6, 20, 0, 0, 0, 0, 0, 64, 16, 24, 16, 24, 24, 0, 0, 0, 0, 0
, 7, 4, 6, 25, 0, 0, 0, 0, 0, 64, 16, 24, 16, 16, 16, 0, 0, 0, 0, 0
, 7, 4, 6, 30, 0, 0, 0, 0, 0, 64, 16, 16, 16, 16, 16, 0, 0, 0, 0, 0
, 7, 4, 6, 35, 0, 0, 0, 0, 0, 62, 16, 80, 16, 16, 16, 0, 0, 0, 0, 0
, 7, 4, 6, 40, 0, 0, 0, 27, 0, 72, 100, 98, 112, 28, 24, 0, 0, 0, 0, 0
, 7, 4, 6, 45, 0, 0, 0, 0, 0, 72, 24, 24, 28, 16, 24, 0, 0, 0, 0, 0
, 7, 4, 6, 50, 0, 0, 0, 2, 0, 74, 52, 116, 16, 28, 24, 0, 0, 0, 0, 0
, 7, 4, 6, 55, 0, 0, 0, 1, 0, 74, 52, 16, 8, 112, 24, 0, 0, 0, 0, 0
, 7, 4, 7, 0, 0, 0, 0, 0, 0, 53, 16, 16, 20, 16, 0, 0, 0, 0, 0, 0
, 7, 4, 7, 5, 0, 15, 0, 4, 0, 74, 112, 24, 16, 20, 24, 0, 0, 0, 0, 0
, 7, 4, 7, 10, 0, 0, 0, 0, 0, 74, 24, 24, 24, 16, 24, 0, 0, 0, 0, 0
, 7, 4, 7, 15, 0, 93, 0, 105, 0, 73, 104, 98, 98, 96, 100, 0, 0, 0, 0, 0
, 7, 4, 7, 20, 0, 146, 0, 106, 0, 64, 96, 96, 96, 128, 116, 0, 0, 0, 0, 0
, 7, 4, 7, 25, 0, 81, 0, 61, 0, 54, 128, 128, 96, 80, 112, 0, 0, 0, 0, 0
, 7, 4, 7, 30, 1, 87, 0, 246, 0, 54, 112, 96, 112, 80, 98, 0, 0, 0, 0, 0
, 7, 4, 7, 35, 0, 37, 0, 38, 0, 64, 112, 96, 96, 80, 98, 0, 0, 0, 0, 0
, 7, 4, 7, 40, 0, 21, 0, 15, 0, 64, 80, 100, 80, 112, 24, 0, 0, 0, 0, 0
, 7, 4, 7, 45, 0, 0, 0, 3, 0, 64, 16, 20, 16, 24, 48, 0, 0, 0, 0, 0
, 7, 4, 7, 50, 0, 0, 0, 15, 0, 62, 16, 16, 8, 16, 16, 0, 0, 0, 0, 0
, 7, 4, 7, 55, 0, 0, 0, 1, 0, 62, 16, 56, 80, 16, 16, 0, 0, 0, 0, 0
, 7, 4, 8, 0, 0, 0, 0, 2, 0, 62, 16, 56, 80, 16, 16, 0, 0, 0, 0, 0
, 7, 4, 8, 5, 0, 0, 0, 14, 0, 62, 72, 56, 48, 16, 80, 0, 0, 0, 0, 0
, 7, 4, 8, 10, 0, 0, 0, 6, 0, 62, 48, 52, 16, 24, 48, 0, 0, 0, 0, 0
, 7, 4, 8, 15, 0, 0, 0, 2, 0, 62, 48, 80, 48, 84, 48, 0, 0, 0, 0, 0
, 7, 4, 8, 20, 0, 0, 0, 0, 0, 62, 52, 48, 16, 48, 80, 0, 0, 0, 0, 0
, 7, 4, 8, 25, 0, 0, 0, 9, 0, 62, 80, 48, 80, 52, 80, 0, 0, 0, 0, 0
, 7, 4, 8, 30, 0, 0, 0, 3, 0, 64, 80, 112, 16, 56, 48, 0, 0, 0, 0, 0
, 7, 4, 8, 35, 0, 0, 0, 1, 0, 64, 48, 52, 48, 80, 80, 0, 0, 0, 0, 0
, 7, 4, 8, 40, 0, 40, 0, 44, 0, 64, 92, 116, 96, 96, 96, 0, 0, 0, 0, 0
, 7, 4, 8, 45, 1, 238, 1, 0, 0, 54, 96, 96, 96, 96, 96, 0, 0, 0, 0, 0
, 7, 4, 8, 50, 0, 153, 0, 90, 0, 54, 112, 116, 82, 112, 96, 0, 0, 0, 0, 0
, 7, 4, 8, 55, 0, 1, 0, 8, 0, 53, 112, 112, 96, 112, 112, 0, 0, 0, 0, 0
, 7, 4, 9, 0, 0, 137, 0, 117, 0, 63, 80, 128, 98, 0, 0, 4, 0, 0, 0, 0
, 7, 4, 9, 5, 0, 0, 0, 0, 1, 74, 24, 28, 24, 24, 24, 0, 0, 0, 0, 0
, 7, 4, 9, 10, 0, 0, 0, 0, 1, 74, 24, 24, 80, 24, 24, 0, 0, 0, 0, 0
, 7, 4, 9, 15, 0, 0, 0, 0, 0, 74, 24, 28, 28, 16, 56, 0, 0, 0, 0, 0
, 7, 4, 9, 20, 0, 0, 0, 0, 0, 74, 16, 16, 24, 16, 28, 0, 0, 0, 0, 0
, 7, 4, 9, 25, 0, 0, 0, 0, 0, 74, 24, 24, 16, 120, 124, 0, 0, 0, 0, 0

// 2:10~7:10  
// 2:06~7:05
};



//month(1 byte) 
//date(1 byte) 
//hour(1 byte) 
//miniute(1 byte) 
//steps(2 byte)
//movement(2 byte)
//pulse rate(1 byte)
//gesture(1 byte)
//sleep state 1(1 byte)
//sleep state 2(1 byte)
//sleep state 3(1 byte)
//sleep state 4(1 byte)
//sleep state 5(1 byte)
//sleep state 6(1 byte)
//NULL(position 17~20)

uint32_t Api_SleepCallbackTest(uint8_t *data, uint16_t current_pkg, uint16_t pre_offset)
{
    if(current_pkg == 0 || current_pkg <= pre_offset)
    {
        return NRF_ERROR_NOT_FOUND;
    }
  
    memcpy(data, sleep_test_data[current_pkg - pre_offset - 1], 20);
    
    
    return 0;
}
#endif

//uint32_t calc_error[179] = {0}; 
void Sleep_Test(void)
{
#if SLEEP_DEBUG    

    // WJ.20190515.分配空间
    acculate_sleep_quality_t *p_sleep_info = NULL;
    /* //为了保证仿真过程和实际一致，把这部分移到下面处理，每次调用申请
    p_sleep_info = (acculate_sleep_quality_t *)malloc(sizeof(acculate_sleep_quality_t));
    if(p_sleep_info == NULL)
    {
        return;
    }
    Get_WriteRawPos(p_sleep_info, sizeof(acculate_sleep_quality_t));
    */
    Acculate_SleepingInit(Api_SleepCallbackTest);
    Vs_ClearInfoMsg();  
    
    
    // WJ.20181229.时间过长
    App_Watch_Dog_Reload();
    //删除外部flash数据
    //
    
    for(uint16_t i = 1; i <= 222; i++)
    {
        
        p_sleep_info = (acculate_sleep_quality_t *)malloc(sizeof(acculate_sleep_quality_t));
        if(p_sleep_info == NULL)
        {
            return;
        }
        Get_WriteRawPos(p_sleep_info, sizeof(acculate_sleep_quality_t));
        
        
        //calc_error[i - 1] = 
        Acculate_SleepDetectEntery(i, 0, 1);

        // WJ.20190701.算法内部有赋值处理，需要在算法调用完成后再次获取
        Vs_GetSleepingInfo();
    
        if(p_sleep_info->sleep_flag == 3)        
        { 
            uint8_t last_sleep_time[4] = {0};   //最后一次退出睡眠时间-日月时分
            Api_GetSleepTotalTimeNew(last_sleep_time, NULL, NULL, NULL);
            if(Sleep_ValidityCheck((void *)&p_sleep_info->enter_time, last_sleep_time))
            {
//                Sleep_SaveData(p_sleep_info);//Vs_GetSleepingInfo());
            }
            Vs_ClearInfoMsg();      
            // WJ.20181229.时间过长
            App_Watch_Dog_Reload();  
        } 
        
        // WJ.20190515.释放空间 
        if(p_sleep_info != NULL)
        {
            free(p_sleep_info);
            p_sleep_info = NULL;
        } 
            
    }
    // WJ.20190515.释放空间 
    if(p_sleep_info != NULL)
    {
        free(p_sleep_info);
        p_sleep_info = NULL;
    } 
    
    // WJ.20181229.时间过长
    App_Watch_Dog_Reload();
    Sleep_Init();
#endif    
}


#if SLEEP_DEBUG_1


#if SLEEP_DEBUG_1_1 
const uint8_t daily_data_buf0[288][92] = {
12,8,0,0,0,0,0,0,0,0,0,0,74,0,17,17,137,113,49,0,65,72,67,67,67,0,0,0,0,0,0,0,0,0,0,0,91,88,90,92,90,89,91,93,87,89,87,88,91,90,90,87,66,80,98,86,92,94,89,92,91,87,87,84,83,80,78,84,87,92,91,93,92,88,84,88,91,90,88,86,87,87,82,77,82,90,0,0,201,147,0,0,
12,8,0,5,0,0,0,16,0,0,0,0,74,0,113,17,145,129,77,0,65,66,75,81,74,0,0,0,0,0,0,0,0,0,0,0,91,93,88,90,87,89,91,90,87,89,90,90,89,87,91,91,87,88,87,86,87,87,87,87,87,87,78,71,72,109,112,111,106,102,102,102,102,102,102,102,102,78,63,71,73,73,73,73,73,73,0,0,201,147,0,0,
12,8,0,10,0,0,0,0,0,0,0,0,63,0,105,109,133,77,9,0,76,73,66,65,67,0,0,0,0,0,0,0,0,0,0,0,75,90,94,94,97,98,82,76,76,76,75,79,81,93,98,98,98,98,98,98,95,93,93,92,82,82,82,82,82,82,82,82,82,82,82,82,82,84,84,87,87,87,87,87,87,87,87,87,88,88,0,0,201,109,0,0,
12,8,0,15,0,0,0,4,0,0,0,0,74,0,145,137,141,133,77,0,70,71,64,64,65,0,0,0,0,0,0,0,0,0,0,0,87,98,111,110,110,110,110,110,110,108,102,100,100,92,87,86,76,69,70,69,69,71,74,75,76,79,91,92,90,88,79,72,75,75,83,85,91,83,87,87,87,88,83,79,83,88,88,74,76,81,0,0,201,144,0,0,
12,8,0,20,0,0,0,18,0,0,0,0,74,0,77,77,141,131,81,0,67,65,70,66,69,0,0,0,0,0,0,0,0,0,0,0,93,90,83,79,77,82,82,81,79,79,79,82,77,83,83,82,89,87,82,87,85,85,80,82,82,76,70,99,107,98,93,88,93,91,92,90,87,78,77,92,97,92,93,98,91,82,81,80,76,78,0,0,201,135,0,0,
12,8,0,25,0,0,0,1,0,0,0,0,73,0,77,17,113,49,9,0,64,63,71,72,77,0,0,0,0,0,0,0,0,0,0,0,102,100,94,91,85,85,86,87,87,94,92,90,104,99,97,100,93,94,84,81,78,80,83,80,84,88,88,84,79,75,75,78,87,84,84,79,81,86,85,79,77,75,76,68,69,69,71,79,83,90,0,0,201,135,0,0,
12,8,0,30,0,0,0,5,0,0,0,0,73,0,105,97,105,113,113,0,73,72,64,70,71,0,0,0,0,0,0,0,0,0,0,0,87,87,83,81,79,73,72,71,87,98,87,85,90,89,83,79,76,76,78,82,93,105,99,93,90,91,82,85,88,84,84,83,84,84,79,83,85,97,95,92,91,90,88,79,79,82,79,78,81,82,0,0,201,132,0,0,
12,8,0,35,0,0,0,1,0,0,0,0,73,0,41,105,109,137,129,16,73,73,72,60,41,0,0,0,0,0,0,0,0,0,0,0,82,66,71,71,76,84,81,82,84,82,85,82,83,83,83,80,81,85,81,82,82,80,80,84,81,77,80,81,81,79,79,78,64,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,0,0,201,98,0,0,
12,8,0,40,0,0,0,10,0,0,0,0,74,0,129,145,17,17,17,17,70,69,68,65,71,0,0,0,0,0,0,0,0,0,0,0,60,60,60,60,62,63,78,93,93,90,89,84,80,79,91,88,89,84,83,80,82,82,83,83,85,96,91,95,88,83,78,92,93,91,90,94,92,93,91,93,93,91,89,86,84,77,72,67,75,83,0,0,201,126,0,0,
12,8,0,45,0,0,0,10,0,0,0,0,63,0,113,17,17,105,77,0,64,66,69,78,80,0,0,0,0,0,0,0,0,0,0,0,95,91,91,91,89,94,95,93,88,87,89,94,85,85,89,89,89,92,92,91,89,91,88,86,85,82,82,82,80,82,82,78,77,79,75,78,74,72,72,61,53,60,67,66,67,76,95,94,88,85,0,0,201,113,0,0,
12,8,0,50,0,0,0,0,0,0,0,0,72,0,117,49,17,17,17,0,68,66,64,64,64,0,0,0,0,0,0,0,0,0,0,0,87,83,83,94,92,91,84,81,86,89,87,86,82,83,93,92,90,88,96,92,93,95,91,89,85,84,91,85,95,95,93,92,92,94,89,91,93,91,93,89,84,89,93,91,92,95,91,93,91,89,0,0,201,150,0,0,
12,8,0,55,0,0,0,5,0,0,0,0,72,0,17,17,17,17,81,0,62,62,62,63,62,0,0,0,0,0,0,0,0,0,0,0,96,102,99,98,95,92,92,94,87,86,97,92,95,90,100,97,94,92,89,92,92,92,94,95,91,91,91,98,99,95,95,91,94,95,90,93,105,98,93,85,83,81,78,86,110,109,105,105,104,101,0,0,201,119,0,0,
12,8,1,0,0,0,0,8,0,0,0,0,61,0,49,81,113,81,113,0,59,62,62,65,66,0,0,0,0,0,0,0,0,0,0,0,95,101,100,94,98,95,99,95,98,100,97,89,103,100,93,93,94,97,94,94,93,79,93,106,103,96,95,93,94,94,96,91,90,92,89,90,88,92,93,83,69,67,77,96,99,101,95,94,89,90,0,0,201,58,0,0,
12,8,1,5,0,0,0,4,0,0,0,0,74,0,17,113,17,81,17,0,64,66,61,59,59,0,0,0,0,0,0,0,0,0,0,0,93,90,86,90,97,93,92,97,92,92,88,78,57,61,79,104,106,98,94,95,100,100,99,95,98,95,92,91,98,96,82,82,98,100,103,105,93,95,106,103,101,101,96,93,98,101,102,85,100,106,0,0,201,113,0,0,
12,8,1,10,0,0,0,0,0,0,0,0,74,0,17,17,17,81,21,0,59,60,60,61,59,0,0,0,0,0,0,0,0,0,0,0,105,102,96,87,103,105,102,96,99,98,94,95,98,98,95,99,99,100,96,99,93,95,99,97,93,97,98,94,89,102,94,83,83,99,105,100,96,102,98,100,100,98,96,95,96,97,101,98,97,96,0,0,201,150,0,0,
12,8,1,15,0,0,0,0,0,0,0,0,74,0,49,49,81,17,17,0,61,60,60,60,62,0,0,0,0,0,0,0,0,0,0,0,94,95,95,91,92,94,101,94,91,98,99,99,93,97,96,96,96,96,98,94,95,96,91,101,105,101,97,98,96,98,100,102,99,98,90,98,94,94,95,96,90,90,96,97,98,97,92,97,92,93,0,0,201,150,0,0,
12,8,1,20,0,0,0,5,0,0,0,0,63,0,17,49,113,49,17,0,61,61,61,58,60,0,0,0,0,0,0,0,0,0,0,0,94,92,95,99,98,94,91,94,102,98,102,98,99,95,96,93,96,93,97,92,92,93,91,83,69,87,106,114,109,105,105,105,101,97,98,100,102,95,94,98,93,98,93,95,98,101,99,97,101,102,0,0,201,82,0,0,
12,8,1,25,0,0,0,1,0,0,0,0,63,0,17,17,17,49,17,0,61,60,60,60,63,0,0,0,0,0,0,0,0,0,0,0,99,97,93,94,96,93,100,99,95,93,96,96,99,94,96,99,97,96,93,94,87,92,101,96,94,97,96,100,99,95,98,97,98,99,94,96,95,94,95,93,92,96,93,91,93,91,92,93,92,92,0,0,201,145,0,0,
12,8,1,30,0,0,0,0,0,0,0,0,72,0,113,17,17,17,49,0,61,60,60,58,58,0,0,0,0,0,0,0,0,0,0,0,84,87,101,108,100,96,93,94,97,97,97,97,101,103,99,99,96,97,96,99,95,93,95,97,101,101,96,98,96,104,103,102,100,101,99,100,101,102,99,100,94,95,98,97,90,93,102,104,104,102,0,0,201,150,0,0,
12,8,1,35,0,0,0,0,0,0,0,0,72,0,17,17,21,17,17,0,58,58,58,58,58,0,0,0,0,0,0,0,0,0,0,0,95,101,101,101,101,101,98,101,101,94,104,99,104,106,102,99,98,93,101,100,100,100,101,101,100,99,103,100,98,102,100,98,98,94,99,100,102,104,102,100,97,95,96,100,99,103,103,99,99,100,0,0,201,150,0,0,
12,8,1,40,0,0,0,8,0,0,0,0,72,0,17,49,17,17,21,0,58,57,57,57,62,0,0,0,0,0,0,0,0,0,0,0,100,97,97,109,103,98,101,94,97,103,104,103,98,103,112,105,103,100,100,103,99,99,102,99,99,99,104,118,108,104,103,100,104,102,105,107,100,96,100,101,101,101,88,84,86,110,111,110,107,101,0,0,201,121,0,0,
12,8,1,45,0,0,0,2,0,0,0,0,74,0,17,17,113,113,17,0,59,59,61,59,59,0,0,0,0,0,0,0,0,0,0,0,95,105,104,103,103,103,98,95,102,101,98,94,95,96,98,95,96,95,105,108,101,96,95,97,97,94,100,89,77,78,90,84,100,99,107,108,106,104,100,98,103,99,96,100,98,100,99,100,96,98,0,0,201,83,0,0,
12,8,1,50,0,0,0,0,0,0,0,0,72,0,49,17,17,17,49,0,58,59,60,60,60,0,0,0,0,0,0,0,0,0,0,0,100,97,99,95,103,105,102,95,95,101,99,99,100,99,99,100,95,99,99,99,99,94,98,92,96,97,98,99,97,93,93,94,100,97,95,93,102,100,96,99,94,95,96,100,96,94,96,94,93,97,0,0,201,150,0,0,
12,8,1,55,0,0,0,14,0,0,0,0,72,0,17,17,17,17,113,0,60,60,61,59,61,0,0,0,0,0,0,0,0,0,0,0,94,96,95,97,95,92,93,94,96,101,103,98,95,92,95,96,95,100,100,96,97,94,94,96,97,99,97,98,94,92,93,102,100,103,105,99,101,97,100,96,99,96,95,96,96,100,100,97,92,84,0,0,201,148,0,0,
12,8,2,0,0,0,0,0,0,0,0,0,72,0,113,17,17,17,49,0,63,59,58,60,59,0,0,0,0,0,0,0,0,0,0,0,74,69,91,110,99,107,108,108,101,98,96,97,97,92,95,100,105,99,95,101,99,102,101,97,96,96,98,96,102,96,102,101,96,93,98,100,97,94,99,104,100,101,99,95,98,94,103,104,99,100,0,0,201,150,0,0,
12,8,2,5,0,0,0,18,0,0,0,0,72,0,49,17,77,49,17,0,58,58,66,60,60,0,0,0,0,0,0,0,0,0,0,0,98,107,103,101,95,92,95,103,99,98,101,99,100,102,107,99,97,102,102,97,101,97,96,92,92,86,71,86,90,89,99,103,102,97,103,99,96,97,93,94,96,96,101,96,93,91,101,102,100,101,0,0,201,143,0,0,
12,8,2,10,0,0,0,17,0,0,0,0,72,0,81,145,145,145,131,0,60,60,64,63,63,0,0,0,0,0,0,0,0,0,0,0,96,93,97,91,82,94,103,97,106,110,101,102,99,99,88,98,94,98,97,102,60,53,78,98,102,109,99,99,102,99,101,94,97,93,94,99,99,99,99,99,98,94,94,94,94,94,94,93,96,96,0,0,201,57,0,0,
12,8,2,15,0,0,0,0,0,0,0,0,63,0,145,129,107,97,139,0,62,70,73,71,68,0,0,0,0,0,0,0,0,0,0,0,96,96,110,119,125,132,108,108,108,127,141,141,136,136,140,149,147,147,150,147,125,109,109,113,135,152,166,157,157,153,142,121,147,172,161,141,139,124,117,117,96,72,90,105,105,112,124,122,129,131,0,0,201,150,0,0,
12,8,2,20,0,0,0,24,0,0,0,0,74,0,129,129,109,21,81,3,78,41,82,65,64,0,0,0,0,0,0,0,0,0,0,0,111,117,115,88,82,75,72,72,72,72,72,72,72,72,72,72,72,72,71,67,67,65,66,69,62,59,63,65,68,75,73,87,99,93,73,91,102,101,97,98,98,92,83,85,97,95,95,93,95,94,0,0,201,69,0,0,
12,8,2,25,0,0,0,10,0,0,0,0,74,0,17,81,117,17,21,0,62,64,61,64,62,0,0,0,0,0,0,0,0,0,0,0,95,94,94,94,91,92,87,94,102,102,96,93,91,92,81,93,105,98,98,102,106,105,98,96,96,96,105,94,81,69,66,80,102,99,98,97,102,98,94,97,98,103,99,96,96,94,93,91,87,86,0,0,201,146,0,0,
12,8,2,30,0,0,0,3,0,0,0,0,72,0,17,113,113,113,113,0,64,63,62,62,62,0,0,0,0,0,0,0,0,0,0,0,87,99,96,100,95,94,93,91,90,82,80,78,82,100,91,107,105,102,97,98,98,96,94,83,84,95,98,96,91,95,95,99,94,95,94,90,78,82,97,93,96,96,96,97,94,92,93,89,88,90,0,0,201,82,0,0,
12,8,2,35,0,0,0,20,0,0,0,0,63,0,113,97,113,17,113,0,78,65,62,62,64,0,0,0,0,0,0,0,0,0,0,0,88,82,79,65,69,72,80,75,76,77,81,100,106,102,102,81,82,106,107,98,93,96,97,93,95,92,92,90,90,95,92,96,94,90,92,96,93,96,95,96,94,91,87,71,71,96,100,100,95,95,0,0,201,110,0,0,
12,8,2,40,0,0,0,5,0,0,0,0,63,0,113,85,17,21,17,0,64,59,60,60,62,0,0,0,0,0,0,0,0,0,0,0,99,99,96,93,95,94,92,88,70,59,84,107,113,104,96,95,97,97,96,95,98,103,100,98,98,96,93,98,106,100,99,101,103,107,100,97,97,96,100,97,92,95,94,95,94,95,94,91,90,94,0,0,201,125,0,0,
12,8,2,45,0,0,0,0,0,0,0,0,63,0,17,17,17,17,113,0,62,63,62,65,62,0,0,0,0,0,0,0,0,0,0,0,92,96,98,94,101,99,92,90,94,93,95,94,93,91,92,95,95,92,92,93,95,93,96,99,94,93,91,92,94,93,88,92,90,90,82,98,105,103,97,94,95,91,93,95,96,94,93,93,92,93,0,0,201,150,0,0,
12,8,2,50,0,0,0,1,0,0,0,0,74,0,17,17,113,17,21,0,63,62,62,59,60,0,0,0,0,0,0,0,0,0,0,0,93,92,92,91,88,93,94,95,87,101,92,90,94,98,93,95,97,96,97,96,96,90,73,88,105,105,104,104,102,101,101,103,100,98,100,99,97,97,95,94,98,97,102,95,95,97,100,95,93,95,0,0,201,89,0,0,
12,8,2,55,0,0,0,9,0,0,0,0,74,0,17,17,17,105,17,0,60,61,72,59,59,0,0,0,0,0,0,0,0,0,0,0,101,97,97,92,94,95,93,98,100,94,95,97,97,98,98,96,88,94,99,96,97,96,93,92,80,80,70,60,67,98,104,95,94,104,98,101,99,101,98,97,99,109,106,101,99,95,97,96,94,95,0,0,201,147,0,0,
12,8,3,0,0,0,0,0,0,0,0,0,74,0,21,17,49,17,49,0,61,61,61,59,61,0,0,0,0,0,0,0,0,0,0,0,93,96,97,100,97,89,88,97,102,98,99,100,99,97,93,90,98,93,96,97,89,103,99,96,94,94,97,97,96,81,84,99,103,95,95,99,101,108,99,98,97,95,88,95,105,97,99,96,96,94,0,0,201,150,0,0,
12,8,3,5,0,0,0,0,0,0,0,0,74,0,17,17,113,17,17,0,61,62,61,62,61,0,0,0,0,0,0,0,0,0,0,0,95,95,98,96,96,101,95,94,93,97,94,93,95,92,93,96,95,94,96,95,95,89,89,95,98,98,95,94,94,93,94,94,94,95,94,95,95,97,95,93,96,93,98,97,93,93,96,94,95,93,0,0,201,150,0,0,
12,8,3,10,0,0,0,0,0,0,0,0,74,0,17,17,17,49,17,0,61,62,61,61,60,0,0,0,0,0,0,0,0,0,0,0,94,95,99,93,95,90,95,94,95,96,96,97,90,92,89,94,93,96,94,95,95,94,93,94,97,94,98,93,93,99,95,93,93,99,98,95,94,94,92,99,97,92,98,97,104,96,98,96,93,93,0,0,201,150,0,0,
12,8,3,15,0,0,0,13,0,0,0,0,74,0,17,17,143,143,113,0,60,62,77,69,64,0,0,0,0,0,0,0,0,0,0,0,93,95,94,102,96,95,102,98,98,99,96,93,95,94,97,99,95,93,86,85,91,93,89,85,109,106,100,102,102,102,109,119,120,124,124,137,138,129,128,128,115,112,102,94,98,97,100,97,94,95,0,0,201,139,0,0,
12,8,3,20,0,0,0,5,0,0,0,0,72,0,17,141,133,81,17,0,66,72,62,60,60,0,0,0,0,0,0,0,0,0,0,0,93,92,94,92,90,91,91,91,91,97,98,94,94,91,105,106,90,90,85,75,71,86,102,102,102,97,83,92,95,101,101,96,96,98,99,94,94,94,95,98,100,99,97,97,90,101,102,98,100,99,0,0,201,103,0,0,
12,8,3,25,0,0,0,8,0,0,0,0,74,0,49,81,149,145,17,0,60,62,58,58,57,0,0,0,0,0,0,0,0,0,0,0,100,97,93,102,101,86,100,102,97,99,98,98,102,102,99,101,97,99,95,76,94,100,105,95,94,94,106,109,109,106,101,100,105,102,99,100,103,94,97,105,107,104,102,99,98,98,96,100,103,102,0,0,201,94,0,0,
12,8,3,30,0,0,0,50,0,0,0,0,53,0,17,129,141,73,77,0,60,94,73,68,64,0,0,0,0,0,0,0,0,0,0,0,100,103,100,100,101,99,107,108,104,96,94,76,77,73,76,76,80,82,83,83,76,76,77,82,94,96,90,85,81,81,85,81,89,88,84,90,89,87,83,84,98,96,84,82,99,95,93,95,93,91,0,0,201,97,0,0,
12,8,3,35,0,0,0,10,0,0,0,0,53,0,17,81,113,113,117,0,64,68,64,63,69,0,0,0,0,0,0,0,0,0,0,0,90,92,93,89,90,87,97,95,88,89,87,81,90,98,93,92,92,81,71,85,107,98,92,90,90,85,85,84,93,90,84,79,74,90,106,102,98,99,96,96,95,95,92,96,97,91,91,90,69,71,0,0,201,135,0,0,
12,8,3,40,0,0,0,4,0,0,0,0,74,0,77,109,113,113,17,0,64,63,61,60,61,0,0,0,0,0,0,0,0,0,0,0,94,96,94,83,84,107,110,110,102,99,95,85,91,90,109,96,95,97,92,92,90,94,93,92,94,99,96,97,103,97,95,93,97,92,87,101,104,102,103,99,95,90,95,87,83,99,101,101,98,96,0,0,201,149,0,0,
12,8,3,45,0,0,0,0,0,0,0,0,74,0,85,81,113,81,17,0,60,62,65,65,68,0,0,0,0,0,0,0,0,0,0,0,90,91,103,101,98,99,106,97,98,96,98,101,100,100,96,83,88,93,92,95,90,88,85,82,79,87,101,97,100,94,80,82,101,93,95,93,94,90,89,89,93,90,89,84,83,82,83,84,81,80,0,0,201,150,0,0,
12,8,3,50,0,0,0,6,0,0,0,0,74,0,105,113,113,113,129,0,75,66,67,73,71,0,0,0,0,0,0,0,0,0,0,0,84,84,81,81,79,69,67,68,70,76,73,83,87,97,93,89,88,90,91,89,88,86,83,89,72,73,88,95,99,96,88,89,94,90,80,76,76,76,79,96,99,96,90,90,104,99,100,93,91,90,0,0,201,146,0,0,
12,8,3,55,0,0,0,0,0,0,0,0,74,0,21,17,21,113,147,0,66,62,61,64,69,0,0,0,0,0,0,0,0,0,0,0,90,92,92,91,84,87,98,94,98,92,96,93,93,92,98,101,98,94,92,93,98,90,98,90,79,66,88,109,102,98,95,92,96,96,94,94,92,89,87,84,86,101,107,141,144,177,170,138,137,136,0,0,201,87,0,0,
12,8,4,0,0,0,0,1,0,0,0,0,64,0,139,145,85,17,77,0,64,65,64,67,68,0,0,0,0,0,0,0,0,0,0,0,136,136,136,129,116,100,97,80,64,109,109,86,84,102,104,94,95,93,91,95,91,87,86,99,101,97,92,93,92,94,95,90,91,87,82,89,87,86,83,88,90,83,74,83,103,102,95,91,89,92,0,0,201,86,0,0,
12,8,4,5,0,0,0,0,0,0,0,0,74,0,113,113,17,113,97,0,61,69,62,63,71,0,0,0,0,0,0,0,0,0,0,0,99,97,103,97,98,101,96,93,89,88,91,88,91,83,87,88,84,74,90,102,97,101,99,95,96,92,91,92,92,94,92,92,89,91,92,86,85,86,92,102,88,85,81,79,78,81,82,82,83,80,0,0,201,150,0,0,
12,8,4,10,0,0,0,3,0,0,0,0,73,0,137,147,81,81,49,0,74,70,66,67,67,0,0,0,0,0,0,0,0,0,0,0,74,89,91,89,85,83,74,73,73,73,76,88,92,89,88,92,92,92,95,94,91,88,87,88,89,86,86,89,89,89,90,89,88,93,87,91,87,83,82,86,87,86,80,88,94,94,91,89,85,83,0,0,201,130,0,0,
12,8,4,15,0,0,0,4,0,0,0,0,63,0,105,81,117,17,17,0,69,64,67,62,64,0,0,0,0,0,0,0,0,0,0,0,83,78,76,80,83,84,89,87,87,90,89,87,89,89,94,97,94,93,92,88,86,68,69,98,108,104,95,91,94,92,90,91,95,96,93,100,95,95,95,95,94,92,93,95,92,91,91,90,87,84,0,0,201,87,0,0,
12,8,4,20,0,0,0,20,0,0,0,0,74,0,117,145,49,141,17,0,70,68,68,70,64,0,0,0,0,0,0,0,0,0,0,0,74,68,70,70,90,100,96,96,95,80,77,83,84,94,94,90,84,91,96,88,91,88,88,92,90,87,84,84,80,89,87,85,84,74,69,75,80,81,97,100,98,94,91,88,92,91,89,91,90,88,0,0,201,104,0,0,
12,8,4,25,0,0,0,0,0,0,0,0,72,0,17,17,17,17,17,0,67,65,65,65,66,0,0,0,0,0,0,0,0,0,0,0,88,85,88,84,88,87,89,86,88,87,82,89,89,89,92,94,87,91,92,89,91,92,92,91,92,88,84,87,90,90,91,89,93,91,90,92,91,90,90,88,91,83,87,86,88,87,94,89,87,88,0,0,201,150,0,0,
12,8,4,30,0,0,0,7,0,0,0,0,63,0,17,145,17,17,17,0,66,67,66,69,68,0,0,0,0,0,0,0,0,0,0,0,90,93,86,93,87,84,85,88,98,90,85,77,76,83,94,93,97,96,93,91,89,88,89,89,88,88,87,88,86,85,85,85,83,85,92,88,85,84,80,85,90,86,86,89,90,82,85,84,86,86,0,0,201,116,0,0,
12,8,4,35,0,0,0,0,0,0,0,0,63,0,17,17,17,17,17,0,68,66,67,68,65,0,0,0,0,0,0,0,0,0,0,0,89,85,87,83,88,92,84,87,88,89,88,85,86,87,88,89,90,93,88,85,91,85,89,90,89,85,85,85,86,83,85,89,85,84,88,88,88,84,83,86,84,84,83,91,95,92,90,84,83,85,0,0,201,150,0,0,
12,8,4,40,0,0,0,0,0,0,0,0,63,0,81,17,17,17,49,0,65,65,67,65,66,0,0,0,0,0,0,0,0,0,0,0,90,91,91,90,89,87,87,90,89,88,89,88,91,84,89,90,92,91,87,89,88,91,87,89,87,87,87,84,84,83,88,92,89,88,89,95,89,89,90,91,90,85,87,84,88,89,88,88,89,92,0,0,201,150,0,0,
12,8,4,45,0,0,0,0,0,0,0,0,63,0,73,17,17,17,17,0,65,65,67,66,66,0,0,0,0,0,0,0,0,0,0,0,94,93,92,91,91,91,89,85,80,84,92,87,88,89,90,89,89,93,91,87,85,86,87,88,90,89,87,85,84,85,86,90,88,87,83,86,83,94,91,86,88,88,88,95,91,86,88,92,89,87,0,0,201,150,0,0,
12,8,4,50,0,0,0,0,0,0,0,0,63,0,17,17,49,17,17,0,65,62,62,63,64,0,0,0,0,0,0,0,0,0,0,0,89,90,93,88,89,92,90,88,91,89,94,93,91,87,92,92,94,95,93,97,89,84,90,94,91,96,93,92,91,90,90,93,88,90,94,92,94,93,93,97,95,93,91,94,91,88,91,89,90,90,0,0,201,150,0,0,
12,8,4,55,0,0,0,0,0,0,0,0,63,0,17,17,17,49,17,0,64,64,64,63,63,0,0,0,0,0,0,0,0,0,0,0,92,92,91,91,90,95,89,93,92,91,91,92,87,89,96,95,93,93,90,91,91,92,90,91,92,92,85,90,85,90,91,92,90,91,92,90,90,92,98,96,95,92,92,90,92,91,95,91,92,94,0,0,201,150,0,0,
12,8,5,0,0,0,0,0,0,0,0,0,63,0,17,113,17,49,113,0,63,63,63,63,63,0,0,0,0,0,0,0,0,0,0,0,88,89,96,97,92,89,94,92,94,90,92,88,87,93,87,86,92,94,95,96,99,94,92,94,90,91,93,87,84,100,93,93,91,92,92,91,90,93,91,95,92,90,94,92,94,93,93,92,90,91,0,0,201,150,0,0,
12,8,5,5,0,0,0,13,0,0,0,0,74,0,17,17,113,49,81,0,63,63,67,58,60,0,0,0,0,0,0,0,0,0,0,0,89,91,93,94,88,91,93,93,92,90,92,91,87,91,91,94,96,92,92,90,85,75,71,54,69,102,110,98,91,92,94,96,88,89,93,95,95,100,102,99,95,94,94,99,98,99,93,97,98,92,0,0,201,79,0,0,
12,8,5,10,0,0,0,4,0,0,0,0,63,0,145,117,49,81,117,0,62,60,60,61,60,0,0,0,0,0,0,0,0,0,0,0,83,104,106,93,89,89,89,97,104,96,101,103,105,100,98,93,92,90,93,91,94,97,95,96,97,95,105,100,99,103,99,102,98,101,93,91,80,82,95,96,100,100,101,98,98,93,92,87,83,90,0,0,201,92,0,0,
12,8,5,15,0,0,0,0,0,0,0,0,54,0,81,17,17,49,17,0,60,61,59,60,60,0,0,0,0,0,0,0,0,0,0,0,102,97,101,97,94,93,96,96,94,93,96,97,97,95,95,100,97,93,93,93,102,104,101,98,94,97,98,99,93,95,97,95,96,103,102,99,97,96,93,94,97,97,97,97,99,101,95,97,103,100,0,0,201,150,0,0,
12,8,5,20,0,0,0,2,0,0,0,0,54,0,17,113,49,97,105,0,58,65,64,72,63,0,0,0,0,0,0,0,0,0,0,0,101,102,97,96,100,100,102,100,97,97,94,92,86,90,85,83,94,90,89,85,90,87,93,99,91,94,90,94,90,85,89,84,81,81,81,78,74,83,95,96,88,95,91,89,96,95,92,93,93,94,0,0,201,141,0,0,
12,8,5,25,0,0,0,1,0,0,0,0,54,0,113,113,77,17,143,0,64,71,67,73,64,0,0,0,0,0,0,0,0,0,0,0,96,89,93,96,92,93,91,90,90,88,82,80,77,77,79,75,74,75,74,74,74,77,82,86,88,88,92,87,86,85,85,87,87,86,86,84,69,67,82,87,86,86,85,94,100,109,100,96,93,92,0,0,201,107,0,0,
12,8,5,30,0,0,0,2,0,0,0,0,73,0,113,113,105,17,105,0,67,64,72,70,72,0,0,0,0,0,0,0,0,0,0,0,95,93,87,89,88,91,90,89,99,90,85,99,93,95,91,94,90,90,89,88,88,85,78,77,74,76,84,84,87,86,85,84,85,87,84,84,82,81,81,82,83,83,79,83,82,80,80,80,84,88,0,0,201,124,0,0,
12,8,5,35,0,0,0,9,0,0,0,0,74,0,109,17,113,17,105,0,73,64,66,68,69,0,0,0,0,0,0,0,0,0,0,0,80,77,78,75,80,74,69,93,94,95,89,87,98,91,92,93,95,93,89,83,81,78,79,81,92,92,100,95,93,90,90,91,88,89,85,81,82,85,84,72,70,71,69,73,78,91,89,101,89,87,0,0,201,135,0,0,
12,8,5,40,0,0,0,7,0,0,0,0,74,0,97,113,17,113,17,0,66,65,67,63,60,0,0,0,0,0,0,0,0,0,0,0,86,85,78,83,95,96,92,93,92,90,89,85,90,88,90,89,90,88,89,90,83,92,91,92,90,88,85,80,72,88,102,99,93,91,92,88,87,80,98,98,98,98,96,98,97,101,100,97,100,98,0,0,201,81,0,0,
12,8,5,45,0,0,0,13,0,0,0,0,74,0,21,131,129,129,137,0,66,61,64,65,65,0,0,0,0,0,0,0,0,0,0,0,93,88,85,94,89,70,66,72,73,103,87,86,84,77,78,83,84,94,94,93,93,92,84,83,78,74,76,87,97,82,76,73,96,101,93,95,90,92,96,96,92,89,81,82,102,97,85,86,99,96,0,0,201,116,0,0,
12,8,5,50,0,0,0,1,0,0,0,0,74,0,137,113,49,145,129,0,64,63,64,68,61,0,0,0,0,0,0,0,0,0,0,0,93,88,83,70,82,99,99,94,89,89,90,96,96,90,95,96,92,90,89,89,87,90,92,94,94,92,89,89,90,91,89,92,90,78,71,70,73,102,107,103,103,101,98,95,92,77,79,103,104,104,0,0,201,103,0,0,
12,8,5,55,0,0,0,0,0,0,0,0,73,0,109,77,145,137,137,8,63,59,63,63,61,0,0,0,0,0,0,0,0,0,0,0,104,104,100,100,88,83,75,72,72,74,82,82,85,85,83,83,90,96,98,96,90,90,96,93,91,93,108,111,102,96,90,90,89,88,88,87,90,90,104,101,96,98,98,98,97,94,93,96,96,97,0,0,201,150,0,0,
12,8,6,0,0,0,0,12,0,0,0,0,74,0,129,129,145,49,81,0,64,68,59,62,60,0,0,0,0,0,0,0,0,0,0,0,96,92,94,95,95,96,93,93,87,87,87,86,87,94,113,77,73,94,104,97,95,97,93,92,101,98,96,104,107,103,94,95,91,86,90,91,96,89,95,101,102,100,101,95,93,107,94,93,96,95,0,0,201,103,0,0,
12,8,6,5,0,0,0,5,0,0,0,0,74,0,81,113,139,113,17,0,61,63,58,58,64,0,0,0,0,0,0,0,0,0,0,0,87,89,97,103,96,99,97,98,98,93,86,83,86,102,99,84,80,84,101,100,98,85,92,95,110,108,91,89,98,100,104,106,103,105,100,103,102,98,92,94,93,86,93,97,91,90,92,90,85,84,0,0,201,149,0,0,
12,8,6,10,0,15,0,70,0,13,0,8,74,0,17,113,105,129,129,0,65,71,76,94,78,0,0,0,0,0,0,0,0,0,0,0,93,90,92,88,89,89,86,89,90,89,91,88,82,66,67,79,76,66,72,79,81,83,80,82,81,73,69,69,72,73,74,74,78,81,74,76,78,78,78,74,71,71,71,87,81,81,81,89,86,84,0,0,201,78,0,0,
12,8,6,15,0,0,0,0,0,0,0,0,53,0,49,17,17,113,17,1,67,63,64,62,64,0,0,0,0,0,0,0,0,0,0,0,83,80,83,91,93,91,88,88,88,91,93,90,86,91,93,93,92,90,94,93,97,95,92,93,90,89,88,90,94,92,88,88,88,82,96,105,106,100,100,95,98,92,95,92,88,90,90,89,88,89,0,0,201,150,0,0,
12,8,6,20,0,0,0,0,0,0,0,0,53,0,145,17,17,113,17,0,62,63,62,63,61,0,0,0,0,0,0,0,0,0,0,0,92,92,101,101,99,97,99,97,93,94,92,94,91,96,93,96,92,93,90,88,92,91,91,88,93,97,93,95,96,97,94,105,105,107,99,103,105,103,101,98,97,97,101,96,93,93,91,94,95,92,0,0,201,150,0,0,
12,8,6,25,0,0,0,8,0,0,0,0,53,0,151,81,49,17,113,0,64,59,60,61,61,0,0,0,0,0,0,0,0,0,0,0,96,99,91,101,106,103,108,104,103,98,100,105,102,99,97,100,101,94,92,99,101,96,92,96,97,94,95,94,95,97,93,93,94,94,92,96,97,96,96,92,92,93,89,99,100,99,94,99,97,95,0,0,201,150,0,0,
12,8,6,30,0,0,0,1,0,0,0,0,53,0,113,113,113,17,85,0,60,61,61,60,62,0,0,0,0,0,0,0,0,0,0,0,102,98,93,96,94,100,99,96,95,94,96,97,96,99,97,96,93,95,95,94,96,94,92,95,94,98,100,95,97,95,96,101,96,94,96,99,97,97,95,95,93,89,88,84,111,109,108,105,103,100,0,0,201,121,0,0,
12,8,6,35,0,0,0,0,0,0,0,0,74,0,17,17,17,113,113,0,59,59,59,57,59,0,0,0,0,0,0,0,0,0,0,0,96,101,101,102,101,99,97,98,96,93,95,96,97,99,98,96,99,98,104,101,97,99,100,93,95,97,98,93,95,102,103,101,100,100,96,96,105,104,105,101,101,97,98,98,99,99,104,99,96,97,0,0,201,150,0,0,
12,8,6,40,0,0,0,0,0,0,0,0,53,0,17,149,17,113,17,0,60,58,60,60,58,0,0,0,0,0,0,0,0,0,0,0,104,93,96,103,95,100,99,96,102,99,106,103,97,100,103,101,95,96,97,99,100,100,92,92,96,93,94,95,95,96,97,98,99,99,99,99,96,103,102,101,101,99,94,96,99,105,102,99,96,100,0,0,201,143,0,0,
12,8,6,45,0,0,0,0,0,0,0,0,53,0,81,17,49,17,17,0,57,59,58,58,59,0,0,0,0,0,0,0,0,0,0,0,104,101,103,102,101,106,105,99,98,99,98,99,98,98,101,103,99,95,81,89,102,104,102,97,108,104,94,100,101,98,102,99,98,96,95,102,99,98,99,97,98,98,100,95,93,97,96,94,97,97,0,0,201,149,0,0,
12,8,6,50,0,0,0,5,0,0,0,0,74,0,145,113,131,49,113,0,61,57,60,58,58,0,0,0,0,0,0,0,0,0,0,0,94,80,69,99,110,109,107,107,99,94,100,96,98,92,90,108,107,98,99,107,103,100,98,93,97,103,103,97,105,103,95,94,95,96,111,106,107,102,103,99,99,98,96,85,88,96,103,101,99,98,0,0,201,149,0,0,
12,8,6,55,0,0,0,4,0,0,0,0,74,0,113,113,85,49,17,0,57,59,62,58,57,0,0,0,0,0,0,0,0,0,0,0,102,99,105,97,98,104,97,102,104,104,100,97,96,95,87,95,101,99,98,107,102,102,98,94,92,81,91,87,100,107,103,101,97,93,98,96,94,89,75,83,107,110,103,99,93,99,97,99,104,105,0,0,201,150,0,0,
12,8,7,0,0,0,0,6,0,0,0,0,74,0,73,17,117,49,17,0,58,61,65,60,63,0,0,0,0,0,0,0,0,0,0,0,93,90,97,86,76,88,97,96,101,97,96,92,95,94,94,98,84,84,100,99,96,97,96,98,93,85,66,64,91,102,97,96,96,104,96,94,96,100,99,96,99,96,93,97,91,88,88,89,94,99,0,0,201,146,0,0,
12,8,7,5,0,0,0,1,0,0,0,0,74,0,81,17,113,113,73,0,65,67,68,69,73,0,0,0,0,0,0,0,0,0,0,0,98,92,87,85,95,82,77,96,94,94,97,91,91,86,87,83,86,86,83,85,82,79,78,84,86,85,83,91,86,90,91,86,86,88,87,87,88,87,73,69,69,66,70,87,88,87,86,87,89,93,0,0,201,119,0,0,
12,8,7,10,0,0,0,5,0,0,0,0,72,0,17,109,137,129,17,0,63,65,62,66,64,0,0,0,0,0,0,0,0,0,0,0,90,91,91,94,95,96,93,92,91,87,85,82,71,72,79,83,84,82,82,82,84,79,80,80,85,87,86,83,81,85,85,85,89,83,74,88,95,93,89,89,89,91,91,92,90,93,96,89,86,86,0,0,201,99,0,0,
12,8,7,15,0,0,0,7,0,0,0,0,54,0,77,17,97,49,17,0,63,65,75,60,61,0,0,0,0,0,0,0,0,0,0,0,73,75,87,88,99,99,97,93,94,95,94,92,89,89,88,91,93,96,91,89,88,89,89,85,83,82,84,84,79,73,106,103,97,99,91,96,96,93,94,101,97,96,97,93,92,93,93,89,97,101,0,0,201,79,0,0,
12,8,7,20,0,0,0,34,0,0,0,0,64,0,117,17,149,141,133,0,64,62,66,76,98,0,0,0,0,0,0,0,0,0,0,0,94,96,95,96,96,90,76,98,97,100,102,97,92,94,95,94,91,90,89,88,86,87,89,88,92,82,58,78,93,93,93,100,106,93,76,73,67,68,68,70,74,67,56,72,61,66,106,108,108,108,0,0,201,61,0,0,
12,8,7,25,0,0,0,0,0,0,0,0,53,0,77,73,137,77,131,16,66,68,73,63,72,0,0,0,0,0,0,0,0,0,0,0,91,90,90,92,91,78,86,90,91,88,90,91,74,63,73,88,94,95,96,96,92,78,84,87,87,84,67,65,81,93,91,95,94,96,96,96,97,97,97,93,92,92,92,92,92,90,86,86,86,86,0,0,201,150,0,0,
12,8,7,30,0,0,0,0,0,0,0,0,53,0,133,129,145,145,145,16,65,69,61,62,93,0,0,0,0,0,0,0,0,0,0,0,86,86,80,75,70,58,83,94,94,93,84,85,80,79,80,82,82,82,101,123,124,100,100,98,98,98,98,104,100,90,87,88,84,95,98,100,99,94,92,96,88,68,65,65,78,115,114,112,88,79,0,0,201,150,0,0,
12,8,7,35,0,0,0,6,0,0,0,0,53,0,109,141,141,133,131,0,92,77,81,84,92,0,0,0,0,0,0,0,0,0,0,0,49,50,54,80,95,102,102,68,58,52,50,53,55,55,56,69,78,78,72,63,61,75,75,58,58,58,59,67,87,86,86,86,86,125,158,158,103,67,63,59,68,93,87,86,82,75,66,63,63,63,0,0,201,119,0,0,
12,8,7,40,0,0,0,93,0,0,0,0,64,0,129,133,129,131,131,8,75,90,91,91,87,0,0,0,0,0,0,0,0,0,0,0,62,66,66,66,75,89,95,85,79,79,61,65,60,60,54,58,85,92,76,76,69,69,69,72,60,61,74,80,80,66,67,65,72,72,72,70,69,72,72,72,72,53,69,85,86,87,87,89,91,91,0,0,201,47,0,0,
12,8,7,45,0,139,1,93,0,124,0,81,64,0,139,97,131,141,109,0,88,91,94,102,109,0,0,0,0,0,0,0,0,0,0,0,85,74,74,74,74,74,74,74,74,76,62,62,62,62,62,62,56,53,52,67,95,85,85,85,95,103,103,103,106,108,108,80,59,59,59,59,68,71,71,73,76,74,68,85,91,91,65,48,52,52,0,0,201,47,0,0,
12,8,7,50,0,80,0,182,0,66,0,44,73,0,141,139,131,137,99,0,74,81,87,92,97,0,0,0,0,0,0,0,0,0,0,0,52,52,52,52,60,69,68,68,68,68,60,59,64,66,66,68,79,77,74,73,65,65,65,65,65,65,65,65,65,62,60,59,59,59,59,62,63,66,83,87,88,74,71,64,56,56,56,56,56,56,0,0,81,59,0,0,
12,8,7,55,0,147,1,22,0,123,0,80,64,0,133,101,133,129,129,0,106,113,99,97,96,0,0,0,0,0,0,0,0,0,0,0,56,56,56,56,56,56,56,57,57,57,58,53,51,51,49,43,43,41,66,76,81,80,84,88,83,82,82,82,82,82,80,54,51,51,51,52,55,55,54,57,54,54,54,50,49,50,51,80,85,84,0,0,21,61,0,0,
12,8,8,0,1,141,1,109,1,85,0,223,64,0,131,129,129,133,129,0,94,95,91,104,112,0,0,0,0,0,0,0,0,0,0,0,76,60,60,64,70,69,69,78,81,85,87,80,70,66,66,62,54,53,58,60,76,94,93,94,94,99,98,95,93,67,67,98,84,63,66,82,68,65,85,75,66,80,86,85,63,59,55,75,83,89,0,0,21,70,0,0,
12,8,8,5,2,80,2,46,2,27,1,97,64,0,133,129,129,133,137,0,116,118,112,110,110,0,0,0,0,0,0,0,0,0,0,0,81,80,81,85,74,49,61,96,90,89,90,100,83,76,90,66,52,52,52,75,84,59,73,93,89,95,98,90,80,78,84,83,83,90,87,84,77,81,81,81,85,89,84,78,78,78,87,97,95,97,0,0,93,101,0,0,
12,8,8,10,2,82,2,4,2,16,1,89,64,0,105,129,41,77,13,0,102,106,99,104,105,0,0,0,0,0,0,0,0,0,0,0,99,97,81,92,101,99,102,97,66,57,60,60,60,71,87,99,100,92,92,88,87,84,88,85,88,98,97,87,92,96,96,93,90,94,90,84,90,92,90,86,83,86,88,88,88,91,100,97,99,100,0,0,93,129,0,0,
12,8,8,15,2,59,1,170,1,231,1,62,64,0,77,77,109,143,131,0,93,102,107,111,111,0,0,0,0,0,0,0,0,0,0,0,100,99,88,98,102,99,97,98,93,88,86,86,86,86,78,78,78,76,74,84,85,85,86,87,83,75,63,52,75,80,80,80,80,98,105,107,107,107,107,103,88,81,77,73,73,73,71,60,59,63,0,0,93,82,0,0,
12,8,8,20,2,66,1,106,1,214,1,52,54,0,129,131,129,141,133,0,109,105,106,104,105,0,0,0,0,0,0,0,0,0,0,0,66,66,67,84,99,97,97,97,94,92,92,92,88,86,80,76,83,70,68,60,58,57,56,58,72,74,74,74,75,71,69,64,60,57,57,57,70,79,90,91,92,74,83,92,90,63,59,58,61,64,0,0,93,94,0,0,
12,8,8,25,2,36,1,100,1,193,1,37,64,0,109,133,77,141,129,0,102,106,103,106,104,0,0,0,0,0,0,0,0,0,0,0,67,67,71,70,54,52,52,53,62,65,65,66,67,65,69,72,54,50,50,50,50,54,54,54,54,57,62,58,58,58,54,70,74,62,57,55,54,54,55,57,62,62,62,66,80,80,78,81,67,57,0,0,93,78,0,0,
12,8,8,30,2,60,1,106,1,206,1,47,54,0,141,101,109,105,109,0,109,107,98,97,100,0,0,0,0,0,0,0,0,0,0,0,55,67,97,100,79,68,63,64,62,59,54,54,54,54,56,56,56,56,56,57,55,60,66,66,66,73,90,88,87,101,99,93,93,93,93,101,101,97,93,90,90,91,91,91,91,90,86,86,69,55,0,0,21,125,0,0,
12,8,8,35,1,205,1,64,1,119,2,99,54,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,103,0,0,
12,8,8,40,0,146,0,196,0,119,0,78,64,0,133,131,133,129,129,0,101,94,94,87,93,0,0,0,0,0,0,0,0,0,0,0,50,49,48,48,48,50,60,61,69,71,71,71,71,71,71,69,61,52,53,58,62,76,92,102,100,93,93,93,99,112,106,96,96,96,96,96,87,73,73,73,48,44,60,73,66,77,89,89,90,90,0,0,5,91,0,0,
12,8,8,45,0,0,0,105,0,0,0,0,64,0,131,133,131,135,129,0,98,87,83,85,84,0,0,0,0,0,0,0,0,0,0,0,78,60,57,57,63,84,76,75,67,88,88,83,90,90,90,87,60,52,49,47,60,80,78,78,78,78,78,78,76,56,46,46,46,51,60,60,60,60,74,94,94,101,86,78,94,96,103,104,86,80,0,0,21,108,0,0,
12,8,8,50,0,76,0,95,0,63,0,41,54,0,77,131,105,135,133,0,89,93,99,100,96,0,0,0,0,0,0,0,0,0,0,0,80,80,80,80,80,75,74,86,102,111,113,113,113,113,113,105,105,101,100,100,100,100,100,100,100,97,86,86,81,51,51,52,53,53,53,56,62,77,73,74,81,88,66,62,60,55,51,51,51,51,0,0,21,91,0,0,
12,8,8,55,0,181,0,146,0,143,0,94,54,0,109,131,131,131,105,0,99,97,95,92,87,0,0,0,0,0,0,0,0,0,0,0,76,76,76,86,89,97,88,103,99,90,90,90,90,92,97,97,97,97,97,97,95,94,88,63,59,61,58,57,57,59,61,65,65,73,74,83,85,86,95,96,96,91,73,84,79,75,70,72,72,72,0,0,21,71,0,0,
12,8,9,0,0,0,0,67,0,0,0,0,64,0,105,131,101,111,105,0,83,85,86,84,91,0,0,0,0,0,0,0,0,0,0,0,72,72,72,69,59,56,55,55,56,60,72,76,76,76,81,86,87,87,88,88,80,79,82,82,82,83,87,80,62,68,72,71,72,72,68,69,68,61,62,66,66,80,80,91,95,103,107,99,100,105,0,0,21,74,0,0,
12,8,9,5,0,0,0,23,0,0,0,0,53,0,101,99,105,109,109,0,83,87,88,88,89,0,0,0,0,0,0,0,0,0,0,0,75,73,72,74,88,88,88,99,107,75,66,65,70,76,76,80,81,75,72,68,67,68,86,83,80,80,77,70,71,69,81,74,60,63,63,65,65,64,61,59,59,63,66,66,66,65,65,63,61,64,0,0,21,87,0,0,
12,8,9,10,0,0,0,11,0,0,0,0,53,0,109,109,97,109,109,0,85,89,87,87,89,0,0,0,0,0,0,0,0,0,0,0,81,81,81,81,81,82,82,82,82,78,60,60,60,60,66,68,64,64,59,58,60,60,61,64,64,66,66,65,65,65,63,61,63,68,75,70,70,62,61,67,71,69,80,92,95,107,117,117,110,99,0,0,21,77,0,0,
12,8,9,15,0,14,0,122,0,11,0,7,51,0,99,97,129,0,0,0,86,88,97,0,89,0,0,0,0,0,0,0,0,0,0,0,80,80,70,60,67,69,90,104,104,104,100,76,76,60,58,60,67,74,71,72,67,71,95,89,102,82,72,85,85,85,85,81,80,80,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,80,0,0,21,50,0,0,
12,8,9,20,0,0,0,0,0,0,0,0,51,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,81,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,88,0,0,21,150,0,0,
12,8,9,25,0,0,0,76,0,0,0,0,53,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,88,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,88,0,0,5,105,0,0,
12,8,9,30,0,0,0,65,0,0,2,21,52,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,122,0,0,
12,8,9,35,0,0,0,0,0,0,0,0,52,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,150,0,0,
12,8,9,40,0,0,0,33,0,0,0,0,51,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,135,0,0,0,0,138,138,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,138,0,0,5,128,0,0,
12,8,9,45,0,0,0,0,0,0,0,0,51,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,138,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,138,0,0,5,150,0,0,
12,8,9,50,0,0,0,0,0,0,0,0,51,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,138,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,138,0,0,5,150,0,0,
12,8,9,55,0,0,0,0,0,0,0,0,51,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,138,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,138,0,0,5,150,0,0,
12,8,10,0,0,0,0,1,0,0,0,0,51,1,0,0,0,0,0,0,0,0,0,0,41,0,0,0,0,0,0,0,0,0,0,0,138,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,138,0,0,5,150,0,0,
12,8,10,5,0,0,0,2,0,0,0,0,51,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,138,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,138,0,0,5,150,0,0,
12,8,10,10,0,0,0,0,0,0,0,0,51,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,150,0,0,
12,8,10,15,0,17,0,102,0,15,0,10,51,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,88,0,0,
12,8,10,20,0,0,0,186,0,0,0,0,64,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,138,0,0,5,91,0,0,
12,8,10,25,0,0,0,102,0,0,0,0,64,0,109,103,77,101,109,0,85,89,87,88,88,0,0,0,0,0,0,0,0,0,0,0,138,138,119,64,58,57,53,61,73,76,66,47,48,67,68,72,72,73,85,67,53,56,61,61,61,74,82,82,79,68,68,68,68,68,67,68,69,69,69,80,76,70,66,74,59,63,62,62,59,58,0,0,5,108,0,0,
12,8,10,30,0,10,0,118,0,8,0,5,64,0,109,101,109,109,97,0,90,91,90,90,90,0,0,0,0,0,0,0,0,0,0,0,58,63,74,73,70,75,81,81,81,81,81,76,72,72,72,72,72,72,72,71,69,67,66,66,66,63,63,62,57,53,55,58,60,60,60,60,60,60,60,60,60,60,61,61,65,70,69,69,73,80,0,0,28,94,0,0,
12,8,10,35,0,0,0,82,0,0,0,0,64,0,131,77,77,109,109,0,89,88,85,89,87,0,0,0,0,0,0,0,0,0,0,0,85,86,86,79,72,69,62,63,62,62,61,59,59,60,60,60,61,61,61,63,77,75,76,76,76,74,74,74,74,80,86,88,86,80,80,69,67,67,57,55,55,55,55,55,53,57,64,70,73,73,0,0,28,83,0,0,
12,8,10,40,0,0,0,197,0,0,0,0,64,0,109,109,141,143,131,0,88,90,94,94,91,0,0,0,0,0,0,0,0,0,0,0,76,80,75,73,73,64,62,62,58,63,66,66,66,67,70,70,70,69,65,65,66,87,96,96,96,94,85,79,66,66,66,68,74,74,74,74,74,72,68,67,67,65,65,63,69,74,74,78,87,88,0,0,28,51,0,0,
12,8,10,45,0,0,0,121,0,0,0,0,74,0,101,101,109,109,73,0,92,85,89,90,94,0,0,0,0,0,0,0,0,0,0,0,63,56,70,74,75,76,74,69,65,69,74,72,69,70,69,68,75,72,69,64,62,62,62,61,60,60,73,83,82,77,68,68,66,59,59,59,59,59,65,66,70,70,70,67,66,64,63,65,67,72,0,0,28,67,0,0,
12,8,10,50,0,12,0,69,0,9,0,6,54,0,97,111,135,141,105,0,93,87,89,83,81,0,0,0,0,0,0,0,0,0,0,0,81,81,81,63,66,70,70,70,70,71,77,108,130,81,77,77,77,77,77,72,70,66,67,66,66,71,74,74,74,74,74,74,74,63,67,72,77,80,79,76,73,73,75,78,75,73,72,72,71,71,0,0,28,105,0,0,
12,8,10,55,0,25,0,158,0,20,0,13,64,1,0,0,0,0,0,0,0,0,0,0,81,0,0,0,0,0,0,0,0,0,0,0,73,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,77,0,0,28,53,0,0,
12,8,11,0,0,0,0,45,0,0,0,0,74,1,133,129,129,133,111,0,80,81,94,97,88,0,0,0,0,0,0,0,0,0,0,0,73,71,71,71,79,98,85,78,77,87,96,98,94,84,75,64,60,68,86,86,84,84,86,87,87,87,87,87,87,86,82,78,77,68,64,64,63,70,72,71,70,75,77,77,77,75,74,74,74,74,0,0,5,90,0,0,
12,8,11,5,0,11,0,196,0,10,0,7,54,0,141,101,135,97,129,0,88,87,88,92,92,0,0,0,0,0,0,0,0,0,0,0,71,59,59,65,68,68,68,68,67,70,70,70,70,70,73,87,72,68,68,68,67,66,65,65,66,67,74,74,74,74,74,76,82,84,84,84,84,84,80,84,87,86,76,75,75,75,85,90,89,86,0,0,28,70,0,0,
12,8,11,10,0,0,0,182,0,0,0,0,64,0,97,99,101,105,77,0,94,91,88,88,85,0,0,0,0,0,0,0,0,0,0,0,85,86,84,79,78,68,68,76,100,111,111,97,82,82,82,82,75,66,66,71,75,74,72,71,71,71,74,77,76,74,79,81,80,80,92,90,91,91,91,80,55,53,53,56,67,68,68,69,76,80,0,0,28,65,0,0,
12,8,11,15,0,0,0,99,0,0,0,0,54,0,105,135,141,109,135,0,90,92,80,79,77,0,0,0,0,0,0,0,0,0,0,0,86,86,86,84,69,70,73,73,79,67,67,67,64,62,62,61,72,100,92,80,80,69,70,70,71,86,85,80,83,95,94,75,89,99,99,100,107,95,78,78,79,76,75,75,73,72,72,72,72,70,0,0,28,100,0,0,
12,8,11,20,0,19,0,133,0,15,0,9,64,0,97,97,111,133,109,0,81,91,85,85,83,0,0,0,0,0,0,0,0,0,0,0,67,67,67,69,75,73,73,75,76,76,79,85,87,90,90,90,90,90,90,89,86,85,85,85,85,75,78,80,80,80,69,64,60,60,66,78,78,78,79,86,86,76,72,78,81,89,84,79,92,101,0,0,28,89,0,0,
12,8,11,25,0,0,0,196,0,0,0,0,64,0,77,111,129,129,135,0,84,84,91,94,91,0,0,0,0,0,0,0,0,0,0,0,91,86,84,84,74,66,64,67,76,85,85,85,85,85,83,59,58,67,74,86,84,73,73,78,83,83,83,85,88,88,89,86,83,83,97,100,98,98,90,86,78,73,73,73,73,72,72,73,87,89,0,0,28,87,0,0,
12,8,11,30,0,10,0,224,0,8,0,6,64,0,109,109,77,105,99,0,91,93,93,89,88,0,0,0,0,0,0,0,0,0,0,0,86,86,86,86,88,94,96,116,118,118,118,118,118,122,127,127,125,107,91,65,66,66,66,68,68,68,64,61,61,61,65,70,70,75,86,89,89,89,89,89,102,102,102,106,105,104,104,93,93,102,0,0,28,74,0,0,
12,8,11,35,0,0,0,157,0,0,0,0,64,0,73,77,131,101,129,0,76,86,88,92,92,0,0,0,0,0,0,0,0,0,0,0,101,93,93,93,94,90,89,84,72,85,90,87,97,105,99,99,81,68,80,79,91,91,90,84,78,78,78,78,78,81,94,79,78,78,78,85,86,86,84,80,80,80,85,93,72,71,74,74,74,74,0,0,28,94,0,0,
12,8,11,40,0,0,0,130,0,0,0,0,54,0,135,129,129,109,129,0,80,81,80,84,78,0,0,0,0,0,0,0,0,0,0,0,74,74,74,82,90,90,91,89,89,86,78,78,97,96,102,102,102,102,80,83,78,83,92,105,110,100,89,76,87,96,127,127,125,108,112,112,112,112,112,107,88,86,80,77,77,76,76,79,85,111,0,0,28,49,0,0,
12,8,11,45,0,27,0,167,0,22,0,14,64,0,129,139,111,133,111,0,85,82,80,81,90,0,0,0,0,0,0,0,0,0,0,0,149,149,139,124,118,101,81,76,76,79,79,75,66,66,67,74,90,92,92,92,84,88,98,90,90,90,92,85,82,82,71,78,76,73,73,73,80,82,82,81,76,71,59,56,56,57,89,95,85,80,0,0,28,90,0,0,
12,8,11,50,0,0,0,114,0,0,0,0,54,0,131,131,97,135,131,0,84,92,83,76,79,0,0,0,0,0,0,0,0,0,0,0,78,76,76,77,76,81,80,77,78,80,91,86,76,76,74,52,55,56,56,56,61,95,101,101,141,157,146,146,146,146,146,146,146,126,81,87,91,95,95,102,104,104,104,104,104,104,104,104,103,103,0,0,28,107,0,0,
12,8,11,55,0,0,0,83,0,0,0,0,54,0,129,131,141,109,97,0,74,80,82,70,80,0,0,0,0,0,0,0,0,0,0,0,90,85,85,86,77,84,70,66,48,49,49,49,49,49,50,50,50,55,61,68,84,116,118,108,89,85,71,82,83,83,84,86,88,85,87,86,84,84,80,79,79,79,79,79,78,81,75,75,79,79,0,0,28,56,0,0,
12,8,12,0,0,0,0,145,0,0,0,0,64,0,77,109,77,105,103,0,81,85,81,80,86,0,0,0,0,0,0,0,0,0,0,0,81,82,81,79,76,72,68,67,67,70,70,70,70,70,70,70,65,75,79,79,79,77,72,74,79,79,93,102,88,89,90,95,97,103,122,126,116,97,96,93,59,55,55,50,50,50,50,59,62,62,0,0,28,82,0,0,
12,8,12,5,1,102,0,247,1,63,0,209,64,0,101,131,109,77,77,0,88,92,96,102,100,0,0,0,0,0,0,0,0,0,0,0,75,87,87,87,87,87,87,91,91,99,103,102,92,79,77,69,66,62,62,59,53,49,47,47,47,48,49,49,57,58,62,55,54,53,59,61,61,63,78,81,64,62,62,62,61,58,58,59,75,78,0,0,28,92,0,0,
12,8,12,10,2,17,1,204,1,219,1,54,54,0,77,133,133,133,101,0,101,105,111,107,110,0,0,0,0,0,0,0,0,0,0,0,80,80,80,80,81,82,82,82,82,74,67,85,84,74,79,98,62,55,54,60,72,90,92,72,56,57,51,47,56,64,69,104,105,93,80,64,63,64,68,71,71,72,73,73,77,91,99,85,69,73,0,0,21,123,0,0,
12,8,12,15,2,54,1,89,1,204,1,45,54,0,133,109,77,73,73,0,110,103,100,105,103,0,0,0,0,0,0,0,0,0,0,0,59,56,60,63,67,71,69,70,66,58,60,60,60,60,67,67,67,84,86,85,85,87,94,76,69,74,71,66,75,62,59,65,68,69,68,65,63,51,51,54,56,70,63,73,68,64,64,67,71,71,0,0,21,118,0,0,
12,8,12,20,2,25,0,219,1,169,1,22,64,0,77,97,105,109,77,0,103,103,101,111,99,0,0,0,0,0,0,0,0,0,0,0,71,79,81,90,90,85,59,52,52,52,52,52,52,59,57,47,55,58,68,69,72,72,72,72,72,70,73,80,80,80,72,52,54,63,62,56,56,56,56,56,63,70,70,70,68,68,68,64,62,62,0,0,21,104,0,0,
12,8,12,25,0,55,0,216,0,44,0,29,64,0,105,111,109,99,109,0,91,91,92,89,78,0,0,0,0,0,0,0,0,0,0,0,67,79,79,79,81,85,88,87,68,62,67,67,65,64,64,64,64,64,62,60,62,63,63,63,63,68,68,68,67,67,67,67,66,64,64,63,63,63,63,70,78,81,71,68,68,114,127,115,95,85,0,0,21,102,0,0,
12,8,12,30,0,0,0,66,0,0,0,0,53,0,109,109,109,143,105,0,72,77,74,75,76,0,0,0,0,0,0,0,0,0,0,0,76,75,76,86,86,99,101,104,142,154,145,90,84,83,88,90,90,89,84,77,72,70,72,76,78,78,80,84,80,76,81,75,74,74,74,74,73,78,82,81,77,76,74,73,70,73,76,76,75,75,0,0,21,92,0,0,
12,8,12,35,0,0,0,22,0,0,0,0,53,0,109,111,133,143,103,0,71,86,85,87,88,0,0,0,0,0,0,0,0,0,0,0,75,78,80,76,75,83,88,94,86,89,87,77,96,94,70,56,56,57,58,62,62,61,59,66,73,74,73,70,70,70,70,70,69,60,58,56,57,67,77,79,79,78,74,66,66,65,64,61,60,60,0,0,21,92,0,0,
12,8,12,40,0,0,0,42,0,0,0,0,74,0,99,109,41,105,105,0,87,78,72,75,77,0,0,0,0,0,0,0,0,0,0,0,72,72,72,72,72,72,76,129,129,129,130,131,131,125,84,91,92,93,84,84,83,79,81,83,80,78,79,82,85,83,82,79,78,78,77,68,70,82,82,83,81,78,74,74,76,75,77,74,74,75,0,0,21,52,0,0,
12,8,12,45,0,58,0,110,0,47,0,30,64,0,105,105,109,109,133,0,76,79,76,89,93,0,0,0,0,0,0,0,0,0,0,0,77,81,80,73,74,75,76,76,75,79,74,72,73,71,74,75,73,73,69,72,76,83,81,81,81,79,79,78,74,74,73,73,71,71,73,76,76,76,77,80,94,101,102,102,101,76,66,66,66,66,0,0,21,55,0,0,
12,8,12,50,0,34,0,103,0,27,0,18,74,0,77,143,133,97,129,2,88,90,78,90,89,0,0,0,0,0,0,0,0,0,0,0,66,66,67,84,85,85,85,85,85,78,57,46,57,61,62,63,63,59,59,73,79,79,79,79,78,70,65,57,58,58,58,58,58,58,67,72,60,51,51,54,75,75,75,78,69,62,64,64,63,63,0,0,21,49,0,0,
12,8,12,55,0,0,0,57,0,0,0,0,74,0,97,141,141,135,111,0,96,98,97,95,97,0,0,0,0,0,0,0,0,0,0,0,56,56,56,55,52,53,53,53,65,70,79,74,74,74,65,60,60,60,60,60,59,59,59,60,54,47,50,52,53,55,56,58,58,58,58,58,57,58,59,59,59,61,59,62,71,72,71,57,56,56,0,0,21,77,0,0,
12,8,13,0,0,0,0,50,0,0,0,0,74,0,103,101,101,101,143,0,102,99,99,97,90,0,0,0,0,0,0,0,0,0,0,0,53,51,49,49,49,51,78,64,51,50,58,64,64,62,55,55,55,55,57,59,61,62,62,58,60,59,58,58,59,60,60,62,69,63,58,59,59,59,63,62,62,62,65,65,68,69,69,65,63,62,0,0,21,96,0,0,
12,8,13,5,0,0,0,39,0,0,0,0,74,0,109,109,97,105,137,0,87,89,79,80,92,0,0,0,0,0,0,0,0,0,0,0,64,65,66,68,68,68,68,67,66,65,66,68,68,67,62,61,60,72,75,78,66,67,77,80,76,72,72,73,75,74,72,76,73,73,72,73,71,72,72,72,72,70,67,70,71,76,86,85,86,85,0,0,21,60,0,0,
12,8,13,10,0,0,0,56,0,0,0,0,74,0,0,129,107,131,109,0,0,82,83,87,84,0,0,0,0,0,0,0,0,0,0,0,65,93,91,93,0,0,0,0,0,0,0,0,0,72,68,71,71,71,72,73,71,71,71,70,67,68,76,77,77,78,78,79,74,71,73,88,95,93,91,91,91,85,87,98,93,82,82,69,71,71,0,0,21,56,0,0,
12,8,13,15,0,27,0,67,0,21,0,14,64,0,99,143,109,141,97,0,82,80,84,81,86,0,0,0,0,0,0,0,0,0,0,0,70,70,70,72,70,69,70,71,72,71,66,65,60,66,71,67,64,60,72,78,75,63,62,66,61,60,70,72,72,72,73,72,72,76,77,77,83,84,83,83,82,77,75,98,117,115,116,88,56,56,0,0,21,54,0,0,
12,8,13,20,0,237,0,203,0,191,0,125,64,0,141,109,129,129,109,0,77,85,92,98,101,0,0,0,0,0,0,0,0,0,0,0,59,71,76,76,73,72,100,128,97,97,89,74,73,65,65,58,60,60,60,60,60,60,61,62,62,62,62,62,62,62,73,76,76,93,78,70,66,63,62,58,58,59,60,60,66,82,79,75,71,74,0,0,0,107,0,0,
12,8,13,25,2,18,1,161,1,184,1,31,54,0,129,73,77,105,73,0,107,109,108,106,105,0,0,0,0,0,0,0,0,0,0,0,94,92,79,70,71,71,71,71,71,71,72,73,74,67,73,95,81,74,71,70,77,82,79,88,89,93,94,96,98,108,101,97,99,98,94,92,92,94,98,98,98,92,101,96,87,92,95,98,104,104,0,0,21,150,0,0,
12,8,13,30,2,28,1,176,1,217,1,54,64,0,77,109,101,77,141,0,103,108,107,107,112,0,0,0,0,0,0,0,0,0,0,0,105,99,92,95,91,87,88,92,81,59,57,57,57,79,90,75,62,62,62,62,55,55,55,55,58,58,70,70,78,93,93,93,93,95,80,72,72,75,75,75,81,84,110,110,85,48,59,60,60,60,0,0,21,126,0,0,
12,8,13,35,0,197,0,115,0,155,1,247,54,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,21,50,14,3,
12,8,13,40,0,147,0,125,0,117,0,77,64,0,129,133,135,103,141,0,101,100,96,94,88,0,0,0,0,0,0,0,0,0,0,0,0,110,87,86,86,86,86,73,62,56,52,57,58,55,56,56,68,74,67,63,64,64,64,85,65,65,78,80,80,79,79,66,58,60,66,66,67,63,77,77,79,80,80,77,74,71,66,83,89,89,0,0,5,56,0,0,
12,8,13,45,0,38,0,86,0,30,0,19,64,0,129,133,131,131,101,0,91,94,91,94,88,0,0,0,0,0,0,0,0,0,0,0,79,76,70,67,64,64,64,79,74,71,73,82,95,84,69,60,50,63,63,67,72,72,71,73,85,85,81,78,76,58,57,58,69,78,86,101,82,68,69,63,57,76,78,78,81,84,84,84,96,102,0,0,21,80,0,0,
12,8,13,50,0,39,0,62,0,31,0,21,64,0,109,109,109,109,109,0,82,78,82,85,97,0,0,0,0,0,0,0,0,0,0,0,92,76,76,76,75,78,79,84,78,71,75,79,78,77,70,71,77,96,94,87,85,85,84,83,83,82,80,83,86,86,82,72,71,76,84,86,86,85,74,70,65,57,51,59,64,69,69,69,72,77,0,0,21,75,0,0,
12,8,13,55,0,27,0,77,0,21,0,14,62,0,141,143,101,109,101,0,93,91,88,84,76,0,0,0,0,0,0,0,0,0,0,0,80,80,75,74,71,69,67,53,53,56,64,79,82,83,87,90,106,105,91,89,81,70,63,66,67,70,70,70,70,70,71,70,70,71,78,78,78,77,76,76,79,89,90,71,79,80,81,88,78,78,0,0,21,73,0,0,
12,8,14,0,0,0,0,63,0,0,0,0,62,0,105,105,77,109,77,0,81,78,76,77,80,0,0,0,0,0,0,0,0,0,0,0,70,73,75,71,67,75,77,78,75,76,74,73,69,75,77,77,89,102,100,94,77,83,81,80,76,76,75,76,76,79,86,82,76,77,80,81,77,73,73,70,78,73,76,79,77,74,72,70,72,72,0,0,21,137,0,0,
12,8,14,5,0,21,0,79,0,17,0,10,74,0,109,109,133,141,133,0,82,93,83,84,87,0,0,0,0,0,0,0,0,0,0,0,74,79,79,78,78,89,90,90,72,64,79,87,86,76,63,62,93,107,78,76,77,78,79,78,75,64,68,68,68,74,73,69,68,75,75,72,67,78,90,87,76,67,89,96,96,96,96,95,74,74,0,0,21,87,0,0,
12,8,14,10,0,36,0,94,0,28,0,19,64,0,133,141,137,97,97,0,91,81,77,79,87,0,0,0,0,0,0,0,0,0,0,0,74,55,61,68,80,77,75,75,65,62,62,65,66,66,66,79,96,100,100,100,100,102,108,104,105,97,59,52,56,57,58,58,63,65,74,76,76,76,79,74,68,66,66,66,66,66,63,63,64,63,0,0,21,122,0,0,
12,8,14,15,0,0,0,17,0,0,0,0,64,0,97,97,109,109,109,0,83,80,80,77,84,0,0,0,0,0,0,0,0,0,0,0,75,82,81,76,76,76,76,72,88,87,76,76,75,74,74,76,78,75,72,71,70,72,70,69,64,66,71,82,86,84,83,83,86,86,85,78,72,72,72,72,72,71,71,74,75,71,71,71,71,70,0,0,0,145,0,0,
12,8,14,20,0,0,0,18,0,0,0,0,64,0,109,105,105,73,105,0,78,78,80,83,83,0,0,0,0,0,0,0,0,0,0,0,70,70,71,68,67,73,86,87,86,80,79,75,80,81,82,84,79,79,80,80,79,76,70,73,77,81,84,80,80,74,75,76,78,77,77,77,72,68,81,77,74,65,73,76,78,78,78,78,91,98,0,0,0,73,0,0,
12,8,14,25,0,59,0,74,0,47,0,31,64,0,97,109,97,131,131,0,78,88,90,89,93,0,0,0,0,0,0,0,0,0,0,0,98,101,76,70,74,77,72,68,70,67,71,73,76,94,96,89,65,52,61,86,86,87,87,87,85,84,84,79,80,94,126,122,75,71,71,84,84,84,87,90,91,91,91,91,91,76,82,85,78,78,0,0,0,67,0,0,
12,8,14,30,0,104,0,121,0,86,0,56,54,0,133,105,129,135,109,4,96,85,86,81,76,0,0,0,0,0,0,0,0,0,0,0,75,75,70,60,60,60,61,68,69,78,78,80,84,85,87,87,87,87,87,87,86,83,77,77,66,71,79,85,77,66,66,66,66,66,66,66,68,71,75,77,74,72,76,77,73,79,80,79,80,82,0,0,21,57,0,0,
12,8,14,35,0,0,0,110,0,0,0,0,62,0,9,109,109,105,109,0,78,80,77,79,87,0,0,0,0,0,0,0,0,0,0,0,79,78,67,73,75,80,81,83,76,60,58,58,66,79,87,88,86,74,70,74,78,78,66,66,79,82,82,85,88,93,84,81,78,76,83,98,98,98,92,65,74,73,59,57,61,61,61,60,61,61,0,0,21,68,0,0,
12,8,14,40,0,0,0,92,0,0,0,0,64,0,135,131,141,141,129,1,92,73,74,73,81,0,0,0,0,0,0,0,0,0,0,0,61,62,61,60,60,60,64,88,102,106,107,107,107,106,104,96,83,73,73,73,74,75,79,83,78,78,78,76,76,77,83,90,87,85,77,81,82,82,84,87,87,87,91,82,82,81,79,78,85,88,0,0,21,70,0,0,
12,8,14,45,0,0,0,53,0,0,0,0,53,0,133,101,97,131,79,0,84,75,79,81,76,0,0,0,0,0,0,0,0,0,0,0,67,66,66,76,83,75,70,70,75,103,102,98,84,84,84,84,84,84,84,84,86,90,91,90,81,71,68,69,69,63,62,62,62,62,65,72,79,77,72,71,73,75,75,76,76,71,71,73,75,75,0,0,21,56,0,0,
12,8,14,50,0,0,0,23,0,0,0,0,64,0,97,103,139,141,105,0,79,75,78,77,74,0,0,0,0,0,0,0,0,0,0,0,73,76,77,76,75,74,71,69,69,77,83,82,83,79,79,79,74,75,89,92,94,87,77,73,73,73,73,70,66,66,67,81,86,119,126,111,108,108,108,105,127,131,130,130,93,80,78,78,77,75,0,0,21,73,0,0,
12,8,14,55,0,0,0,78,0,0,0,0,64,0,109,109,111,101,111,0,84,77,84,83,85,0,0,0,0,0,0,0,0,0,0,0,75,72,70,64,53,53,59,70,73,71,72,86,91,77,81,88,88,87,79,79,79,79,79,64,68,80,77,83,78,78,76,72,67,67,67,63,63,63,58,54,58,62,62,93,119,127,122,111,98,89,0,0,21,121,0,0,
12,8,15,0,0,41,0,101,0,33,0,21,64,0,101,109,141,129,97,0,85,83,89,91,86,0,0,0,0,0,0,0,0,0,0,0,88,94,97,96,96,90,85,76,76,73,70,70,74,87,91,101,102,107,115,114,114,112,108,114,112,102,86,86,89,102,99,98,119,114,114,98,92,74,75,77,77,73,73,72,75,78,75,73,72,71,0,0,21,65,0,0,
12,8,15,5,0,0,0,45,0,0,0,0,62,0,109,141,111,109,141,0,86,86,84,79,77,0,0,0,0,0,0,0,0,0,0,0,67,86,86,86,86,87,74,70,70,69,70,71,71,71,71,66,61,87,80,76,67,50,50,52,52,52,66,70,77,71,73,73,74,74,75,79,80,80,80,91,90,85,89,109,114,135,122,75,86,94,0,0,21,50,0,0,
12,8,15,10,0,0,0,109,0,0,0,0,62,0,109,97,131,77,101,0,81,79,82,83,80,0,0,0,0,0,0,0,0,0,0,0,102,95,83,78,78,78,70,69,67,67,81,68,79,86,87,113,116,114,122,115,103,94,110,126,123,98,93,103,108,108,106,105,105,103,98,94,94,80,74,74,76,86,88,88,78,62,66,73,76,77,0,0,21,69,0,0,
12,8,15,15,0,0,0,76,0,0,0,0,62,0,101,109,109,97,109,0,80,79,81,78,76,0,0,0,0,0,0,0,0,0,0,0,77,77,77,71,76,74,69,63,59,58,58,70,77,77,77,77,89,101,100,99,98,98,78,71,68,68,67,72,77,83,91,108,109,107,107,105,99,93,72,77,81,82,76,72,62,65,66,73,79,78,0,0,21,65,0,0,
12,8,15,20,0,0,0,57,0,0,0,0,62,0,109,101,109,109,77,0,79,80,73,79,73,0,0,0,0,0,0,0,0,0,0,0,78,78,76,78,91,92,92,107,138,136,89,77,84,89,89,84,78,77,76,79,82,83,84,86,86,87,86,86,83,81,79,79,73,70,90,92,90,90,90,90,94,86,79,80,78,78,85,85,87,82,0,0,21,78,0,0,
12,8,15,25,0,0,0,74,0,0,0,0,62,0,77,143,73,109,77,0,75,70,72,75,79,0,0,0,0,0,0,0,0,0,0,0,80,79,77,77,77,84,82,85,84,82,85,88,86,81,86,86,89,87,78,75,78,80,83,76,76,74,71,71,70,77,82,81,80,80,80,80,79,79,80,82,87,85,80,84,86,81,78,78,77,75,0,0,21,73,0,0,
12,8,15,30,0,137,1,46,0,113,0,74,64,0,109,129,109,109,129,0,86,97,98,93,89,0,0,0,0,0,0,0,0,0,0,0,73,72,68,74,70,63,63,63,63,63,63,63,61,58,74,80,80,75,75,75,81,88,88,82,80,80,81,82,89,101,94,93,88,88,88,111,115,115,113,110,96,98,118,120,114,97,91,91,87,88,0,0,21,70,0,0,
12,8,15,35,0,55,0,237,0,43,0,28,64,0,109,105,109,141,135,0,94,94,88,92,90,0,0,0,0,0,0,0,0,0,0,0,115,139,154,119,104,85,66,63,63,62,62,62,62,68,73,73,72,72,72,80,97,97,87,81,81,83,88,88,102,99,79,78,78,78,83,95,95,90,83,89,83,78,63,63,59,57,66,89,118,118,0,0,21,70,0,0,
12,8,15,40,0,0,0,62,0,0,0,0,53,0,109,131,131,103,101,0,81,84,83,82,82,0,0,0,0,0,0,0,0,0,0,0,118,118,118,99,75,74,87,93,93,93,92,85,65,61,66,68,65,65,63,58,55,54,54,54,54,69,111,98,98,79,77,85,85,91,96,95,88,87,82,77,75,73,69,69,70,62,60,68,76,76,0,0,21,83,0,0,
12,8,15,45,0,0,0,3,0,0,2,98,53,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,122,0,0,
12,8,15,50,0,0,0,60,0,0,0,0,73,0,79,109,109,105,97,0,76,76,77,81,83,0,0,0,0,0,0,0,0,0,0,0,0,82,74,70,66,67,73,73,78,72,67,68,68,77,86,73,78,83,79,77,76,76,79,79,76,78,83,79,76,76,78,74,72,69,67,74,70,71,72,73,73,71,70,66,71,72,72,72,73,73,0,0,5,91,0,0,
12,8,15,55,0,0,0,37,0,0,0,0,73,0,129,129,109,73,137,0,79,83,83,78,82,0,0,0,0,0,0,0,0,0,0,0,65,63,64,52,65,78,100,104,104,106,102,84,66,68,69,70,70,68,64,64,64,62,63,65,67,74,77,77,74,71,69,69,69,71,76,76,75,72,75,74,74,74,74,74,75,73,66,68,69,69,0,0,21,71,0,0,
12,8,16,0,0,0,0,103,0,0,0,0,73,0,105,73,133,77,129,0,85,76,84,89,86,0,0,0,0,0,0,0,0,0,0,0,70,70,70,70,69,69,71,71,73,73,74,76,75,74,74,73,70,71,81,80,76,73,68,67,68,72,72,72,72,57,49,49,78,92,92,92,90,72,73,64,62,60,60,79,75,95,115,107,107,107,0,0,21,73,0,0,
12,8,16,5,0,0,0,111,0,0,0,0,64,0,133,105,139,99,77,0,88,79,76,83,83,0,0,0,0,0,0,0,0,0,0,0,80,78,77,77,72,60,64,69,80,78,75,72,74,73,74,74,74,73,74,75,84,84,78,76,78,80,80,80,80,80,80,80,80,76,71,72,73,73,75,75,74,72,67,66,66,73,76,79,79,79,0,0,0,58,0,0,
12,8,16,10,0,0,0,0,0,0,0,0,64,0,105,73,77,73,109,0,82,80,81,79,83,0,0,0,0,0,0,0,0,0,0,0,73,69,68,73,72,70,69,68,67,71,82,73,71,71,72,73,73,74,73,74,74,74,71,70,69,64,62,70,79,85,80,80,73,74,74,71,70,72,72,72,74,71,69,69,71,68,68,68,67,67,0,0,0,135,0,0,
12,8,16,15,0,49,0,84,0,38,0,25,73,0,109,77,77,109,107,0,76,81,78,80,92,0,0,0,0,0,0,0,0,0,0,0,67,68,68,74,74,75,73,74,76,75,74,68,60,67,71,67,69,76,74,73,73,73,73,75,77,74,69,71,74,76,81,82,82,82,86,66,55,64,64,69,70,101,119,119,119,116,109,109,106,93,0,0,0,79,0,0,
12,8,16,20,0,11,0,208,0,9,0,6,64,0,141,109,131,137,131,0,93,91,94,97,81,0,0,0,0,0,0,0,0,0,0,0,77,79,91,81,79,79,75,73,73,76,76,73,70,77,97,96,96,100,100,85,69,59,55,55,61,60,60,60,62,64,65,69,69,69,69,68,67,67,67,67,67,68,71,66,67,67,67,67,67,70,0,0,21,76,0,0,
12,8,16,25,0,0,0,214,0,0,0,0,64,0,133,131,99,101,129,0,89,93,93,91,85,0,0,0,0,0,0,0,0,0,0,0,75,65,60,60,60,60,61,60,63,69,72,72,72,72,76,78,70,62,61,58,59,77,81,81,85,91,89,67,67,67,67,67,67,67,75,68,66,66,79,83,82,84,86,86,86,88,93,93,93,93,0,0,21,115,0,0,
12,8,16,30,0,10,0,128,0,8,0,5,54,0,129,107,137,111,133,1,88,88,88,87,90,0,0,0,0,0,0,0,0,0,0,0,73,76,117,147,108,89,77,77,87,92,92,92,92,122,132,130,126,102,76,74,69,106,112,114,114,123,133,133,87,65,67,94,101,101,101,107,111,83,89,90,90,90,86,103,110,104,97,102,81,81,0,0,28,89,0,0,
12,8,16,35,0,0,0,168,0,0,0,0,54,0,133,129,129,129,77,0,86,87,91,93,93,0,0,0,0,0,0,0,0,0,0,0,80,75,75,75,81,85,85,85,85,85,86,90,81,81,81,74,69,80,80,80,78,76,76,76,76,72,96,98,109,112,106,109,91,85,77,77,77,79,86,79,64,64,58,57,72,87,94,94,94,94,0,0,28,80,0,0,
12,8,16,40,0,0,0,183,0,0,0,0,64,0,131,137,129,133,129,0,89,92,93,86,97,0,0,0,0,0,0,0,0,0,0,0,95,95,89,65,63,71,72,72,64,63,68,68,83,106,106,105,104,113,112,112,101,81,64,94,104,100,96,88,83,83,84,84,87,87,87,87,85,78,78,78,74,69,101,106,73,75,93,98,120,129,0,0,28,86,0,0,
12,8,16,45,0,0,0,123,0,0,0,0,64,0,133,131,129,137,131,0,88,95,91,87,81,0,0,0,0,0,0,0,0,0,0,0,137,137,136,136,121,109,106,86,94,90,90,90,90,90,87,93,75,62,63,72,105,104,96,96,95,94,79,82,82,82,82,85,81,77,77,78,82,95,109,82,52,48,62,75,74,73,74,76,74,73,0,0,28,98,0,0,
12,8,16,50,0,0,0,135,0,0,0,0,64,0,129,133,101,131,131,8,84,80,81,87,86,0,0,0,0,0,0,0,0,0,0,0,73,73,90,94,96,106,106,107,107,107,103,70,75,100,95,80,69,69,69,66,65,84,88,86,88,84,79,85,96,96,95,77,62,72,74,74,77,79,84,89,113,112,94,90,90,90,90,90,90,90,0,0,28,65,0,0,
12,8,16,55,0,0,0,134,0,0,0,0,64,0,137,129,77,109,77,0,86,88,83,74,78,0,0,0,0,0,0,0,0,0,0,0,82,67,68,67,65,65,65,69,69,69,69,69,77,81,81,82,82,74,73,79,92,92,90,75,69,69,83,85,74,64,74,78,80,81,81,76,76,75,75,76,77,76,71,67,68,70,69,69,69,69,0,0,28,98,0,0,
12,8,17,0,0,0,0,138,0,0,0,0,54,0,97,97,133,131,131,0,83,88,83,83,84,0,0,0,0,0,0,0,0,0,0,0,70,70,69,69,70,79,91,92,92,92,92,92,92,106,110,127,131,131,131,128,126,124,130,130,85,77,84,82,90,97,97,94,68,65,54,54,54,54,55,67,69,69,70,77,77,77,77,79,80,80,0,0,28,94,0,0,
12,8,17,5,0,17,0,134,0,13,0,9,64,0,105,133,133,109,109,1,85,80,78,84,76,0,0,0,0,0,0,0,0,0,0,0,80,79,77,63,52,52,52,52,52,56,66,66,65,64,70,70,71,90,96,100,122,122,122,122,100,80,79,83,84,69,62,68,72,80,80,79,76,74,73,79,98,101,105,110,97,68,67,89,104,104,0,0,28,77,0,0,
12,8,17,10,0,0,0,215,0,0,0,0,64,0,109,105,109,75,109,0,81,86,85,83,76,0,0,0,0,0,0,0,0,0,0,0,101,94,94,87,86,86,86,88,88,83,82,77,76,76,74,69,69,69,69,68,66,66,64,63,63,68,69,87,90,107,116,112,111,89,89,87,91,88,86,86,79,83,89,89,89,89,110,112,86,73,0,0,28,73,0,0,
12,8,17,15,0,10,0,104,0,8,0,5,64,0,143,103,111,107,135,0,75,73,80,82,88,0,0,0,0,0,0,0,0,0,0,0,75,75,75,93,99,103,112,112,104,82,66,72,75,71,77,83,116,101,78,84,86,90,90,88,74,66,55,53,67,74,74,74,73,73,83,83,83,75,67,74,74,74,74,77,72,68,68,68,68,67,0,0,28,61,0,0,
12,8,17,20,0,43,0,155,0,34,0,22,74,0,135,129,109,141,109,0,92,89,86,82,83,0,0,0,0,0,0,0,0,0,0,0,66,64,64,63,63,63,63,63,70,70,69,63,63,60,90,127,91,70,70,70,73,70,69,70,80,72,53,59,73,81,81,80,79,75,71,67,71,73,75,75,74,75,75,74,73,72,70,70,70,70,0,0,28,48,0,0,
12,8,17,25,0,124,0,169,0,99,0,64,54,0,77,101,133,133,111,0,82,87,91,95,82,0,0,0,0,0,0,0,0,0,0,0,70,70,70,70,70,68,68,77,89,99,108,104,92,88,88,88,76,72,75,83,83,83,76,73,73,73,73,72,72,70,68,72,76,76,71,63,63,58,55,66,68,69,67,54,65,89,87,84,86,87,0,0,21,53,0,0,
12,8,17,30,0,23,0,83,0,18,0,12,64,0,133,97,77,73,101,0,84,87,82,79,85,0,0,0,0,0,0,0,0,0,0,0,86,82,82,112,99,66,66,66,66,63,63,68,77,89,89,89,89,89,89,89,91,95,90,64,66,94,80,80,80,79,77,76,76,75,73,73,78,81,81,82,78,78,71,72,91,90,81,79,79,79,0,0,21,86,0,0,
12,8,17,35,0,0,0,108,0,0,0,0,64,0,131,105,105,135,137,0,85,86,85,87,76,0,0,0,0,0,0,0,0,0,0,0,75,74,80,83,70,65,74,74,75,79,78,77,77,86,88,112,112,112,112,113,112,67,64,65,73,79,76,62,57,64,73,71,89,89,89,89,124,133,133,134,79,76,80,81,80,77,77,77,78,79,0,0,21,53,0,0,
12,8,17,40,0,0,0,25,0,0,0,0,73,0,105,109,105,111,105,0,75,75,77,75,77,0,0,0,0,0,0,0,0,0,0,0,80,74,78,78,74,73,72,75,77,74,74,75,77,78,83,80,82,83,75,75,78,78,83,81,76,75,70,69,69,70,79,80,80,80,79,80,80,81,75,76,77,81,80,78,74,74,74,73,71,71,0,0,21,134,0,0,
12,8,17,45,0,0,0,38,0,0,0,0,73,0,109,133,77,105,73,0,82,78,74,76,75,0,0,0,0,0,0,0,0,0,0,0,72,72,72,76,82,84,85,92,94,94,85,82,81,81,83,78,80,87,85,81,78,78,79,80,83,84,84,83,83,83,79,74,74,73,75,77,77,80,81,77,80,77,81,80,76,76,75,77,105,108,0,0,21,105,0,0,
12,8,17,50,0,0,0,97,0,0,0,0,53,0,109,77,109,103,73,0,81,78,80,81,75,0,0,0,0,78,0,0,0,0,0,16,131,133,133,114,83,109,109,77,81,84,81,78,78,79,80,70,75,75,62,64,73,77,78,78,79,83,83,83,83,83,83,84,84,84,77,70,68,68,72,77,69,71,83,84,71,72,73,86,84,85,0,0,21,58,0,0,
12,8,17,55,0,0,0,159,0,0,0,0,74,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,77,0,0,0,0,0,31,92,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,88,96,94,95,74,73,0,0,0,0,0,85,0,0,21,36,0,0,
12,8,18,0,0,50,0,149,0,42,0,28,73,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,84,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,85,0,0,0,99,0,0,
12,8,18,5,0,0,0,23,0,0,0,0,73,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,144,0,0,
12,8,18,10,0,0,0,0,0,0,0,77,74,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6,14,6,
12,8,18,15,0,45,0,106,0,35,0,23,63,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,91,0,0,
12,8,18,20,0,33,0,108,0,26,0,17,64,0,109,79,111,137,143,0,84,78,82,82,81,0,0,0,0,0,0,0,0,0,0,0,86,86,115,118,118,94,79,81,79,78,76,77,77,79,80,80,77,69,67,66,72,100,114,114,114,112,112,112,112,114,116,116,108,89,67,80,86,79,79,79,79,67,62,63,64,62,78,80,78,78,0,0,5,104,0,0,
12,8,18,25,0,112,0,131,0,90,0,59,64,0,109,133,141,111,141,4,92,94,91,86,88,0,0,0,0,0,0,0,0,0,0,0,81,81,81,79,69,69,69,69,69,61,63,66,74,80,83,83,76,74,70,70,70,70,77,77,74,70,66,64,72,77,77,82,82,82,82,81,65,61,57,56,56,69,83,85,86,86,77,68,70,75,0,0,21,71,0,0,
12,8,18,30,0,0,0,27,0,0,0,0,74,0,131,139,131,77,77,0,92,89,94,86,85,0,0,0,0,0,0,0,0,0,0,0,85,91,90,93,105,105,105,91,61,61,61,59,57,57,57,61,64,64,64,67,71,63,62,62,62,62,62,62,62,62,62,62,59,58,57,59,59,59,60,62,62,71,71,71,71,71,71,68,66,66,0,0,21,109,0,0,
12,8,18,35,0,18,0,91,0,14,0,9,74,0,141,141,141,137,109,0,87,88,86,76,92,0,0,0,0,0,0,0,0,0,0,0,66,66,66,65,65,65,65,65,65,65,65,65,59,41,41,42,68,77,77,77,96,98,97,92,72,67,67,67,67,67,67,67,70,89,97,93,93,93,94,94,94,95,93,97,67,64,74,79,74,74,0,0,21,98,0,0,
12,8,18,40,0,175,0,241,0,144,0,94,64,0,109,101,133,109,97,0,87,89,96,90,90,0,0,0,0,0,0,0,0,0,0,0,60,71,73,76,78,77,77,70,63,65,66,63,64,64,66,55,55,57,57,58,58,63,63,63,58,54,54,55,68,73,73,73,73,77,80,79,73,68,67,71,81,83,84,86,86,86,85,80,71,70,0,0,21,93,0,0,
12,8,18,45,0,33,0,52,0,26,0,17,64,0,143,105,105,109,109,0,86,86,90,90,89,0,0,0,0,0,0,0,0,0,0,0,70,68,68,68,70,72,68,70,71,71,69,67,65,56,66,70,72,72,74,73,67,66,66,73,72,67,64,62,62,62,62,59,58,57,58,58,64,65,65,65,75,84,80,80,80,77,83,90,102,109,0,0,21,121,0,0,
12,8,18,50,0,44,0,122,0,35,0,23,64,0,109,103,111,109,109,0,87,93,88,84,90,0,0,0,0,0,0,0,0,0,0,0,110,104,88,85,74,60,88,103,106,101,101,101,74,70,80,86,84,89,114,114,114,100,84,83,83,83,83,83,83,83,71,73,77,77,80,80,96,105,105,105,105,105,105,105,105,105,104,105,105,105,0,0,21,99,0,0,
12,8,18,55,0,0,0,42,0,0,0,0,64,0,119,109,109,77,141,0,75,77,76,79,74,0,0,0,0,0,0,0,0,0,0,0,72,70,72,83,85,88,84,80,81,82,85,81,83,78,85,89,79,78,74,79,87,84,90,87,85,85,85,80,94,94,94,91,91,91,83,81,82,82,86,91,82,80,80,77,91,95,80,78,83,85,0,0,21,63,0,0,
12,8,19,0,0,0,0,8,0,0,0,0,53,0,137,109,111,105,105,0,76,81,86,80,91,0,0,0,0,0,0,0,0,0,0,0,86,81,77,75,79,82,81,73,83,90,85,77,82,83,79,74,70,72,71,70,68,66,65,66,68,68,64,64,74,75,74,78,76,77,78,75,74,72,68,69,70,69,68,64,63,63,59,59,56,60,0,0,21,115,0,0,
12,8,19,5,0,11,0,27,0,9,0,5,53,0,109,139,141,107,141,0,86,86,86,91,85,0,0,0,0,0,0,0,0,0,0,0,65,68,71,72,71,70,69,68,65,65,64,63,63,63,64,73,75,75,71,69,69,70,70,69,72,71,72,61,63,63,65,70,67,67,64,61,59,58,57,57,58,61,69,69,77,89,75,82,83,86,0,0,21,129,0,0,
12,8,19,10,0,141,0,108,0,111,0,73,64,0,141,129,141,109,109,0,91,89,86,78,83,0,0,0,0,0,0,0,0,0,0,0,87,87,81,78,78,78,78,78,52,52,71,73,79,85,85,85,68,55,81,87,87,87,87,117,117,122,122,0,116,115,115,115,101,93,90,89,95,91,79,80,80,81,76,82,79,75,74,71,79,79,0,0,21,107,0,0,
12,8,19,15,0,16,0,77,0,13,0,9,64,0,141,141,131,131,139,0,81,84,90,89,87,0,0,0,0,0,0,0,0,0,0,0,79,73,73,73,69,68,68,68,68,69,66,66,66,66,66,66,66,66,66,66,66,71,112,128,130,130,131,96,63,67,64,64,64,64,63,82,85,89,111,111,110,82,67,69,69,70,70,70,71,71,0,0,21,111,0,0,
12,8,19,20,0,34,0,41,0,27,0,17,53,0,129,109,77,139,137,0,91,76,74,81,75,0,0,0,0,0,0,0,0,0,0,0,67,65,62,62,60,55,56,58,78,77,76,69,69,76,74,83,84,83,84,88,80,78,80,86,85,82,78,78,74,74,71,68,70,72,74,75,80,80,79,80,80,69,69,82,80,80,77,82,78,78,0,0,21,104,0,0,
12,8,19,25,0,0,0,10,0,0,1,240,53,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,21,2,14,2,
12,8,19,30,0,0,0,89,0,0,0,0,53,0,137,139,109,109,109,0,77,84,84,86,85,0,0,0,0,0,0,0,0,0,0,0,0,128,128,128,128,96,74,74,74,74,73,70,70,74,84,84,84,89,89,89,94,87,82,82,82,91,95,94,78,68,60,59,61,62,62,62,75,80,79,72,71,70,69,67,67,69,71,59,66,71,0,0,5,54,0,0,
12,8,19,35,0,71,0,114,0,60,0,39,53,0,109,139,141,105,141,0,91,87,84,85,92,0,0,0,0,0,0,0,0,0,0,0,82,84,79,73,72,75,75,75,75,72,60,57,71,79,79,75,70,73,76,60,64,73,71,73,72,71,67,63,68,78,78,72,71,74,70,68,68,64,61,61,66,68,69,70,70,70,70,70,69,67,0,0,21,88,0,0,
12,8,19,40,0,116,0,192,0,95,0,62,73,0,129,135,131,131,141,0,102,103,99,90,82,0,0,0,0,0,0,0,0,0,0,0,62,59,57,57,56,62,68,68,90,85,79,79,64,60,62,64,64,76,76,76,76,82,82,82,82,90,100,85,80,80,80,77,77,77,77,82,82,82,82,79,78,78,78,78,78,78,82,92,112,136,0,0,21,69,0,0,
12,8,19,45,0,0,0,43,0,0,0,0,64,0,141,75,77,73,137,0,86,87,87,84,85,0,0,0,0,0,0,0,0,0,0,0,146,152,152,152,121,94,90,83,76,76,72,68,68,68,68,68,69,69,72,70,66,63,65,62,62,68,66,66,66,65,62,62,65,65,65,65,75,72,60,60,67,66,61,64,68,67,76,76,73,74,0,0,21,89,0,0,
12,8,19,50,0,51,0,100,0,40,0,26,64,0,137,105,79,133,135,16,89,90,100,96,95,0,0,0,0,0,0,0,0,0,0,0,74,69,63,65,64,63,62,64,65,65,65,65,64,64,64,57,50,50,50,50,62,64,71,75,75,75,76,76,76,69,54,54,67,102,93,86,77,77,77,77,71,71,71,71,74,93,93,92,91,94,0,0,21,66,0,0,
12,8,19,55,0,0,0,221,0,0,0,0,64,0,131,129,129,135,131,24,95,96,96,101,98,0,0,0,0,0,0,0,0,0,0,0,75,75,81,110,110,110,110,110,110,107,81,87,93,93,89,70,66,66,70,72,67,58,58,58,58,58,49,48,73,78,100,114,79,58,59,59,60,69,69,69,85,97,97,97,110,152,152,152,152,152,0,0,21,63,0,0,
12,8,20,0,0,36,0,173,0,32,0,21,74,0,131,131,131,129,129,0,100,99,95,101,109,0,0,0,0,0,0,0,0,0,0,0,145,135,118,78,78,73,93,92,91,114,107,104,85,83,85,85,90,77,56,56,61,64,64,64,64,64,64,78,48,53,86,82,73,73,73,73,74,88,92,58,38,38,51,54,54,61,65,71,71,71,0,0,21,54,0,0,
12,8,20,5,0,0,0,220,0,0,0,0,64,0,131,131,131,129,129,0,95,99,97,96,95,0,0,0,0,0,0,0,0,0,0,0,87,88,88,88,84,78,100,88,78,76,76,63,53,60,111,116,81,74,85,87,87,87,87,86,82,68,65,64,50,48,49,83,95,95,95,97,101,92,87,87,87,78,67,67,74,76,91,110,86,79,0,0,28,59,0,0,
12,8,20,10,0,62,0,213,0,56,0,37,54,0,135,135,135,131,135,8,101,93,103,109,100,0,0,0,0,0,0,0,0,0,0,0,80,80,78,76,70,57,49,52,52,55,66,76,76,76,76,76,76,76,80,89,92,98,88,88,88,81,70,65,65,65,61,54,47,44,43,44,44,44,45,45,67,73,75,75,62,52,47,56,61,64,0,0,28,70,0,0,
12,8,20,15,1,30,1,82,0,241,0,158,64,0,135,99,141,109,77,0,93,106,106,102,106,0,0,0,0,0,0,0,0,0,0,0,84,84,84,95,95,84,61,62,57,57,57,56,50,53,64,72,76,77,81,84,88,88,97,103,105,113,117,116,115,103,98,55,55,65,89,105,112,111,103,101,89,84,94,92,71,71,81,104,101,98,0,0,21,72,0,0,
12,8,20,20,1,218,1,169,1,161,1,16,54,0,141,101,105,41,97,0,113,103,106,113,112,0,0,0,0,0,0,0,0,0,0,0,86,80,77,67,64,63,62,63,71,85,96,92,99,102,112,108,115,107,105,97,103,105,103,71,67,65,75,81,87,82,79,86,92,99,74,79,84,77,68,78,98,89,102,97,93,97,107,99,99,99,0,0,21,121,0,0,
12,8,20,25,2,16,1,188,1,200,1,43,64,0,77,141,105,105,77,0,108,108,103,103,109,0,0,0,0,0,0,0,0,0,0,0,99,101,108,108,109,107,101,98,103,118,120,123,112,109,106,107,106,82,77,67,66,71,77,79,93,101,96,92,100,105,104,104,103,101,104,103,101,102,95,95,89,87,94,105,102,100,103,104,104,103,0,0,21,92,0,0,
12,8,20,30,2,13,1,220,1,212,1,50,54,0,109,97,109,109,109,0,112,114,114,115,112,0,0,0,0,0,0,0,0,0,0,0,94,84,85,85,82,76,67,58,56,55,55,55,55,55,50,50,50,56,59,66,75,75,75,77,73,81,105,103,106,102,98,98,98,90,74,75,76,76,70,70,72,98,101,100,88,60,64,69,70,70,0,0,21,89,0,0,
12,8,20,35,1,213,1,112,1,171,1,23,64,0,131,109,129,129,129,0,113,109,111,103,109,0,0,0,0,0,0,0,0,0,0,0,70,74,84,86,107,111,108,85,82,75,75,75,76,75,65,65,61,63,69,92,95,86,75,74,89,96,95,92,94,88,91,106,102,104,110,105,92,83,87,62,79,111,107,99,90,76,89,97,96,96,0,0,21,110,0,0,
12,8,20,40,0,17,0,50,0,13,0,9,62,0,129,137,109,133,141,0,96,94,97,101,90,0,0,0,0,0,0,0,0,0,0,0,96,72,57,68,77,101,114,119,116,115,102,92,92,92,89,82,77,69,61,59,57,55,55,54,67,67,67,68,69,68,67,70,76,80,81,89,89,104,108,107,97,90,90,97,100,103,103,100,76,76,0,0,21,68,0,0,
12,8,20,45,0,162,0,110,0,128,0,84,64,0,141,131,105,133,129,0,99,102,75,91,97,0,0,0,0,0,0,0,0,0,0,0,66,63,63,62,59,56,59,57,56,56,57,59,58,58,73,98,97,70,56,55,54,58,69,77,83,87,88,117,147,147,147,147,147,147,147,135,103,103,95,82,82,73,72,56,50,59,64,86,84,82,0,0,21,127,0,0,
12,8,20,50,1,194,0,183,1,101,0,232,64,0,129,77,109,77,141,0,96,91,82,93,95,0,0,0,0,0,0,0,0,0,0,0,89,98,93,92,97,100,99,91,91,91,91,63,56,62,75,74,74,76,74,74,74,74,74,76,91,93,77,67,68,69,70,69,69,69,69,69,69,73,74,72,77,72,67,67,67,67,67,67,82,91,0,0,21,134,0,0,
12,8,20,55,1,154,0,166,1,68,0,213,64,0,77,77,109,129,131,0,99,97,92,96,88,0,0,0,0,0,0,0,0,0,0,0,96,98,98,90,87,82,78,77,68,68,68,68,68,68,68,69,79,79,79,79,80,92,93,89,60,55,56,58,58,59,92,109,99,99,99,88,88,77,106,137,132,110,101,83,83,83,82,78,78,78,0,0,21,139,0,0,
12,8,21,0,0,149,0,180,0,121,0,78,64,0,131,131,131,131,131,0,95,100,91,94,101,0,0,0,0,0,0,0,0,0,0,0,69,72,103,100,82,80,70,62,62,64,80,106,111,106,89,68,71,71,71,71,87,98,96,94,93,85,64,66,69,66,66,75,103,99,85,83,68,63,72,73,78,87,117,117,81,72,67,79,82,89,0,0,21,80,0,0,
12,8,21,5,2,10,1,61,1,157,1,14,54,0,129,129,129,129,129,0,105,106,107,107,107,0,0,0,0,0,0,0,0,0,0,0,95,89,95,79,76,92,91,108,115,95,100,70,80,88,88,71,95,112,120,110,66,80,57,57,58,70,70,56,84,82,73,90,88,84,75,64,70,62,62,78,95,96,103,95,76,86,95,105,79,64,0,0,21,150,0,0,
12,8,21,10,1,203,1,21,1,108,0,238,54,0,129,129,129,133,129,0,105,100,101,96,102,0,0,0,0,0,0,0,0,0,0,0,90,88,90,87,87,86,85,91,90,88,90,89,86,86,86,86,80,76,75,74,117,100,86,86,89,87,92,84,93,100,104,86,80,70,83,84,84,88,91,96,100,106,107,0,105,99,81,88,90,92,0,0,21,131,0,0,
12,8,21,15,1,7,0,237,0,208,0,136,54,0,129,129,129,131,129,0,104,89,95,99,101,0,0,0,0,0,0,0,0,0,0,0,72,78,81,91,82,80,76,81,81,76,76,76,74,86,83,84,92,91,91,90,76,71,65,66,81,79,65,76,91,106,106,106,93,92,81,88,103,90,83,89,89,92,97,98,83,81,86,88,75,84,0,0,21,86,0,0,
12,8,21,20,0,0,0,117,0,0,0,0,64,0,131,133,141,141,131,0,96,93,93,90,90,0,0,0,0,0,0,0,0,0,0,0,77,73,63,71,73,78,77,74,64,62,61,62,63,64,85,96,92,79,60,57,57,58,62,62,80,84,85,91,78,49,51,51,55,57,57,58,60,61,76,75,65,64,62,84,107,106,107,80,70,66,0,0,21,101,0,0,
12,8,21,25,0,0,0,21,0,0,0,0,64,0,141,137,137,105,137,2,83,77,81,81,78,0,0,0,0,0,0,0,0,0,0,0,52,60,97,116,104,73,74,76,78,77,78,78,78,102,115,112,107,87,70,72,73,74,73,71,71,70,68,69,71,72,75,71,70,69,69,69,73,73,70,71,72,80,86,86,86,83,77,75,74,74,0,0,21,115,0,0,
12,8,21,30,0,0,0,0,0,0,0,0,64,0,73,77,105,137,105,0,79,79,79,79,73,0,0,0,0,0,0,0,0,0,0,0,74,74,73,73,74,76,78,81,75,74,73,69,72,76,76,74,73,73,74,74,73,74,75,73,72,73,74,74,76,73,74,73,76,76,77,77,73,69,69,69,72,80,84,83,82,81,79,77,76,77,0,0,21,150,0,0,
12,8,21,35,0,0,0,3,0,0,0,0,64,0,109,109,13,77,141,0,77,75,75,75,73,0,0,0,0,0,0,0,0,0,0,0,74,74,71,77,89,92,79,90,103,104,101,94,91,91,91,91,91,88,88,79,77,78,83,83,81,86,97,82,79,77,73,74,80,80,79,79,76,76,76,78,77,76,76,73,75,77,79,79,79,79,0,0,21,148,0,0,
12,8,21,40,0,0,0,88,0,0,0,0,73,0,137,129,129,137,139,0,79,96,102,91,83,0,0,0,0,0,0,0,0,0,0,0,79,84,84,118,107,89,89,99,100,100,95,114,96,80,81,88,87,88,88,75,62,62,60,61,62,66,79,90,90,90,86,85,79,72,90,117,113,70,73,72,72,71,72,72,68,60,60,70,84,87,0,0,21,81,0,0,
12,8,21,45,0,33,0,212,0,26,0,17,64,0,131,137,129,143,137,0,83,88,91,92,87,0,0,0,0,0,0,0,0,0,0,0,94,97,95,80,87,94,90,78,78,78,78,75,73,71,65,68,68,62,58,78,84,73,70,58,58,66,70,70,78,92,87,88,86,74,63,61,62,66,73,71,74,74,74,68,66,66,66,66,65,68,0,0,21,67,0,0,
12,8,21,50,0,66,0,194,0,56,0,37,64,0,137,129,141,131,137,0,73,87,95,93,82,0,0,0,0,0,0,0,0,0,0,0,72,72,76,87,96,95,90,80,78,79,79,81,61,67,80,81,82,96,106,109,74,72,71,69,69,69,69,74,96,96,96,102,109,117,121,121,91,76,81,107,107,79,76,87,92,122,138,138,148,145,0,0,28,94,0,0,
12,8,21,55,0,30,0,121,0,29,0,19,64,0,105,75,137,77,109,0,73,70,75,83,85,0,0,0,0,0,0,0,0,0,0,0,129,122,74,71,77,84,85,87,95,104,103,103,103,103,98,85,85,84,82,83,82,80,80,83,98,92,90,99,92,94,83,84,84,83,78,67,71,71,73,79,82,85,77,77,77,77,75,74,74,80,0,0,21,68,0,0,
12,8,22,0,0,23,0,90,0,18,0,12,74,0,131,141,131,137,105,0,89,85,95,93,86,0,0,0,0,0,0,0,0,0,0,0,92,86,82,78,73,69,64,60,60,60,60,60,59,61,71,82,81,82,85,91,82,82,75,77,89,97,94,94,113,113,115,121,130,139,139,121,114,113,118,122,122,119,119,119,106,93,89,89,81,78,0,0,21,67,0,0,
12,8,22,5,0,24,0,66,0,19,0,12,64,0,107,107,105,141,141,0,75,71,76,79,79,0,0,0,0,0,0,0,0,0,0,0,83,88,87,79,79,78,70,70,70,80,87,87,87,87,87,87,86,86,88,95,91,85,85,76,69,69,86,85,85,86,86,86,74,73,73,73,73,73,74,73,73,73,73,73,73,73,73,73,77,80,0,0,21,102,0,0,
12,8,22,10,0,0,0,86,0,0,0,0,53,0,139,109,109,111,137,0,76,73,72,74,74,0,0,0,0,0,0,0,0,0,0,0,80,78,83,86,86,86,86,84,84,82,80,83,84,89,82,85,86,81,76,74,74,79,80,81,82,85,85,83,70,69,74,75,77,79,73,75,75,75,76,77,79,76,72,63,61,73,86,88,88,88,0,0,21,87,0,0,
12,8,22,15,0,0,0,75,0,0,0,0,53,0,141,141,137,137,109,0,74,72,72,76,76,0,0,0,0,0,0,0,0,0,0,0,84,84,84,88,89,90,95,94,93,93,91,87,84,82,86,87,89,94,77,80,80,83,86,86,83,83,82,78,77,107,125,125,125,127,124,125,128,110,110,103,76,78,86,90,90,89,69,66,78,80,0,0,21,70,0,0,
12,8,22,20,0,0,0,69,0,0,0,0,53,0,109,109,105,111,111,0,77,79,78,74,75,0,0,0,0,0,0,0,0,0,0,0,75,75,71,67,66,66,70,76,72,70,70,70,70,69,69,69,73,81,81,84,82,96,96,91,86,84,77,73,71,70,70,73,78,91,90,90,79,78,78,76,77,79,79,91,85,81,81,81,90,88,0,0,21,56,0,0,
12,8,22,25,0,35,1,104,0,36,0,24,64,0,137,109,109,141,133,0,78,89,80,83,104,0,0,0,0,0,0,0,0,0,0,0,82,82,82,83,86,74,68,69,65,65,71,71,74,78,78,78,73,66,66,66,69,70,73,77,82,83,83,83,84,84,83,77,75,79,79,80,81,81,81,81,81,79,76,66,65,57,56,56,56,56,0,0,0,99,0,0,
12,8,22,30,0,11,0,94,0,9,0,5,64,0,143,135,137,109,105,0,106,93,79,82,82,0,0,0,0,0,0,0,0,0,0,0,58,71,77,83,84,85,85,84,81,74,65,70,66,65,73,74,72,81,90,90,90,90,93,87,87,72,69,80,86,90,107,107,92,94,97,107,107,113,133,136,127,119,113,91,71,63,62,60,60,60,0,0,23,90,0,0,
12,8,22,35,0,38,0,94,0,30,0,20,64,0,105,79,139,139,141,0,78,83,81,78,79,0,0,0,0,0,0,0,0,0,0,0,63,82,92,94,95,106,116,118,118,109,109,98,95,101,101,98,76,76,57,56,57,73,75,75,81,93,95,84,81,86,88,82,76,62,62,62,62,74,84,86,90,90,90,90,96,92,89,87,92,101,0,0,23,117,0,0,
12,8,22,40,0,54,0,145,0,43,0,28,53,0,109,143,129,129,29,0,72,75,87,92,45,0,0,0,0,0,0,0,0,0,0,0,109,95,77,77,78,74,72,69,69,76,80,80,80,79,78,78,79,87,89,89,86,86,88,92,98,84,79,60,67,61,63,71,71,74,82,80,77,82,95,138,152,150,149,151,149,151,153,150,148,149,0,0,23,65,0,0,
12,8,22,45,0,0,0,8,0,0,0,0,53,0,137,105,0,0,0,0,55,73,0,0,43,0,0,0,0,0,0,0,0,0,0,0,150,150,150,150,151,153,151,152,153,151,148,147,146,147,149,150,147,146,146,148,147,147,153,150,148,147,145,130,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,121,0,0,21,83,0,0,
12,8,22,50,0,0,0,0,0,0,0,0,51,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,121,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,120,0,0,21,150,0,0,
12,8,22,55,0,196,0,212,0,159,0,104,64,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,122,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,133,0,0,5,53,0,0,
12,8,23,0,0,18,0,199,0,14,0,9,64,1,0,139,137,141,109,0,88,87,85,92,75,0,0,0,0,0,0,0,0,0,0,0,133,133,133,138,116,101,101,96,96,96,96,78,78,78,85,84,65,59,58,73,83,79,79,75,78,78,78,76,72,72,72,71,65,63,65,65,64,60,62,65,70,97,100,100,108,118,112,86,87,88,0,0,5,97,0,0,
12,8,23,5,0,0,0,46,0,0,0,0,64,0,145,109,109,109,75,0,68,69,77,77,72,0,0,0,0,0,0,0,0,0,0,0,87,88,88,91,97,96,87,83,85,87,87,91,84,88,88,90,89,85,85,85,85,85,84,84,84,88,92,87,72,62,64,69,70,76,74,71,70,66,71,77,69,69,72,74,86,86,85,82,78,78,0,0,21,119,0,0,
12,8,23,10,0,0,0,25,0,0,0,0,64,0,109,109,109,143,141,0,70,73,77,74,71,0,0,0,0,0,0,0,0,0,0,0,86,96,96,97,97,96,82,76,72,75,75,79,82,80,80,96,114,131,81,80,83,85,84,79,79,77,72,55,54,54,57,68,78,76,75,82,88,88,89,91,91,91,91,84,84,85,83,84,90,89,0,0,21,111,0,0,
12,8,23,15,0,0,0,8,0,0,0,0,64,0,143,143,143,145,17,16,78,79,76,67,66,0,0,0,0,0,0,0,0,0,0,0,89,89,87,85,83,83,83,83,74,72,72,72,92,97,124,124,120,100,100,100,100,105,105,106,115,105,101,96,93,93,93,93,87,87,91,94,88,80,89,103,108,108,94,87,89,89,89,88,88,86,0,0,21,87,0,0,
12,8,23,20,0,0,0,9,0,0,0,0,62,0,105,109,25,137,151,16,68,68,66,76,69,0,0,0,0,0,0,0,0,0,0,0,85,85,86,86,87,88,85,84,88,92,88,78,79,80,81,82,87,86,85,90,87,88,82,87,86,90,89,89,89,90,89,91,91,91,91,91,89,77,76,78,76,72,72,72,72,72,72,86,98,95,0,0,21,126,0,0,
12,8,23,25,0,0,0,22,0,0,0,0,62,0,115,109,113,137,137,0,65,69,68,71,71,0,0,0,0,0,0,0,0,0,0,0,91,90,90,90,85,85,89,91,93,92,92,91,92,91,91,80,74,74,69,72,84,94,94,94,97,91,91,91,91,93,86,85,87,88,89,89,89,89,85,80,80,77,79,84,91,70,53,53,55,55,0,0,21,125,0,0,
12,8,23,30,0,76,0,128,0,66,0,43,53,0,141,137,141,113,105,0,83,91,75,70,72,0,0,0,0,0,0,0,0,0,0,0,55,55,67,67,67,76,112,107,73,62,58,58,57,56,57,62,62,64,65,67,78,80,85,87,81,73,73,78,79,64,56,56,66,68,83,84,77,76,83,88,88,88,81,83,84,87,89,89,89,90,0,0,0,103,0,0,
12,8,23,35,0,0,0,0,0,0,0,0,53,0,139,141,81,25,57,0,71,72,66,64,65,0,0,0,0,0,0,0,0,0,0,0,78,77,74,72,75,75,75,90,90,89,88,89,90,92,92,92,92,99,109,99,90,88,92,88,88,86,88,89,88,87,91,93,92,94,96,92,88,84,89,92,93,91,96,89,84,89,88,91,92,91,0,0,21,150,0,0,
12,8,23,40,0,0,0,14,0,0,0,0,53,0,149,137,25,113,109,0,67,66,62,61,68,0,0,0,0,0,0,0,0,0,0,0,91,86,87,90,91,90,92,95,95,89,84,84,83,82,76,73,104,126,96,93,100,93,98,100,97,95,97,92,93,93,94,92,95,97,93,96,96,96,98,94,83,79,73,73,73,73,65,58,76,92,0,0,21,110,0,0,
12,8,23,45,0,0,0,20,0,0,0,0,74,0,113,117,121,81,25,0,64,63,63,68,62,0,0,0,0,0,0,0,0,0,0,0,96,96,93,93,91,93,94,93,79,77,88,95,92,93,93,93,90,92,93,93,94,95,91,90,88,91,89,92,95,95,95,90,89,94,91,84,86,86,76,94,96,93,92,93,92,90,95,93,94,94,0,0,21,99,0,0,
12,8,23,50,0,0,0,0,0,0,0,0,53,0,25,25,81,25,57,0,60,60,62,61,59,0,0,0,0,0,0,0,0,0,0,0,90,100,99,99,103,103,99,94,99,97,99,95,101,100,97,92,96,96,94,93,93,88,90,95,101,102,98,98,96,95,91,98,96,92,100,98,94,91,92,92,94,95,96,92,91,98,95,102,96,95,0,0,21,150,0,0,
12,8,23,55,0,0,0,0,0,0,0,0,53,0,25,25,57,89,57,0,61,60,60,62,61,0,0,0,0,0,0,0,0,0,0,0,95,99,94,93,95,98,99,99,103,96,94,93,99,101,97,92,94,104,93,92,97,93,94,102,98,91,91,100,102,95,94,94,97,104,98,93,97,88,89,93,96,94,96,95,96,91,97,97,95,90,0,0,21,150,0,0,


};
#endif //SLEEP_DEBUG_1_1 

#if SLEEP_DEBUG_1_2 
const uint8_t daily_data_buf1[192][92] = {
12,9,0,0,0,0,0,0,0,0,0,0,53,0,57,121,89,89,81,0,60,61,60,59,60,0,0,0,0,0,0,0,0,0,0,0,107,96,93,93,92,96,93,96,95,93,90,98,101,88,105,103,88,96,100,101,96,103,96,97,97,97,100,96,90,96,103,100,99,101,101,99,101,98,90,92,105,92,95,97,102,90,96,106,96,92,0,0,21,150,0,0,
12,9,0,5,0,0,0,0,0,0,0,0,53,0,57,57,57,57,89,0,59,59,61,61,61,0,0,0,0,0,0,0,0,0,0,0,97,101,98,96,96,101,94,101,101,91,91,104,99,91,98,101,98,98,93,99,95,97,99,91,95,96,93,101,95,96,93,95,95,95,92,94,96,97,94,97,97,94,92,99,100,100,92,88,100,93,0,0,39,150,0,0,
12,9,0,10,0,0,0,0,0,0,0,0,53,0,113,57,81,121,57,0,59,60,59,58,60,0,0,0,0,0,0,0,0,0,0,0,80,98,101,95,100,94,90,88,82,99,97,101,100,98,99,96,98,97,90,101,95,93,101,102,102,97,94,95,98,99,96,97,98,101,93,102,103,101,97,99,96,93,108,98,98,93,99,95,93,95,0,0,201,150,0,0,
12,9,0,15,0,0,0,0,0,0,0,0,53,0,121,57,25,25,57,0,59,59,59,59,59,0,0,0,0,0,0,0,0,0,0,0,99,93,103,101,95,84,102,102,98,96,85,85,96,97,95,95,95,99,97,101,103,100,95,92,98,101,101,98,98,98,93,109,103,100,97,99,101,97,96,100,97,97,94,93,101,99,106,101,99,97,0,0,201,150,0,0,
12,9,0,20,0,0,0,22,0,0,0,0,53,0,57,57,89,121,139,0,59,59,59,59,73,0,0,0,0,0,0,0,0,0,0,0,106,101,96,89,102,100,97,96,95,96,94,104,96,96,96,98,96,99,101,98,95,99,93,93,103,99,102,100,99,98,97,99,97,93,93,103,100,103,99,91,80,80,80,80,81,84,83,73,73,74,0,0,201,131,0,0,
12,9,0,25,0,0,0,4,0,0,0,0,74,0,81,49,117,109,25,0,60,61,58,69,56,0,0,0,0,0,0,0,0,0,0,0,85,92,102,103,101,101,105,100,96,82,70,85,90,110,105,104,92,100,104,102,99,101,98,86,83,102,99,102,100,97,89,100,94,89,87,84,84,84,81,94,110,104,102,103,106,104,101,103,99,99,0,0,201,81,0,0,
12,9,0,30,0,0,0,0,0,0,0,0,74,0,85,57,85,25,49,0,64,56,60,56,56,0,0,0,0,0,0,0,0,0,0,0,95,94,83,77,75,97,113,104,103,98,106,102,96,106,106,100,97,105,104,96,103,104,107,109,109,105,102,91,86,84,103,111,110,110,103,96,102,105,103,93,109,106,101,90,108,112,105,107,99,96,0,0,201,138,0,0,
12,9,0,35,0,0,0,13,0,0,0,0,74,0,25,21,137,109,109,0,57,61,71,83,78,0,0,0,0,0,0,0,0,0,0,0,107,108,109,102,106,103,101,93,102,98,103,107,106,103,104,103,89,77,76,79,80,87,95,95,90,87,82,73,82,82,82,76,72,68,70,70,76,99,108,56,56,56,49,53,69,72,77,83,98,102,0,0,201,77,0,0,
12,9,0,40,0,0,0,9,0,0,0,0,74,0,109,117,109,23,25,0,66,65,66,68,59,0,0,0,0,0,0,0,0,0,0,0,102,102,97,90,92,94,93,90,94,79,86,91,90,90,93,92,99,98,98,96,101,103,102,100,98,102,97,80,80,80,81,87,92,79,90,95,96,91,95,95,99,98,93,94,96,104,102,98,96,96,0,0,201,72,0,0,
12,9,0,45,0,0,0,7,0,0,0,0,74,0,107,87,105,117,81,0,70,65,70,64,67,0,0,0,0,0,0,0,0,0,0,0,91,87,87,86,81,81,77,65,68,77,83,94,97,95,93,97,93,87,83,77,71,77,85,73,74,81,96,97,98,94,93,91,99,100,101,98,99,102,74,70,71,75,78,80,82,96,93,96,102,103,0,0,201,134,0,0,
12,9,0,50,0,12,0,42,0,9,0,6,74,0,117,141,77,105,129,0,67,70,73,81,87,0,0,0,0,0,0,0,0,0,0,0,90,74,84,89,94,95,95,93,90,90,90,94,112,112,112,111,110,112,112,108,98,98,100,90,79,80,81,78,75,68,68,67,56,54,53,54,56,73,94,97,102,97,96,96,96,88,85,72,50,47,0,0,201,148,0,0,
12,9,0,55,0,0,0,107,0,0,0,0,54,0,141,139,139,137,143,12,88,84,79,73,77,0,0,0,0,0,0,0,0,0,0,0,47,49,82,97,87,78,72,66,64,63,61,59,59,62,65,101,121,89,75,75,75,75,75,75,75,75,79,79,79,79,79,79,84,86,86,98,95,95,95,92,80,80,80,80,86,88,82,96,96,96,0,0,201,101,0,0,
12,9,1,0,0,46,0,122,0,36,0,24,74,0,131,109,121,121,25,0,91,86,63,60,59,0,0,0,0,0,0,0,0,0,0,0,101,98,89,79,85,81,81,81,81,81,78,66,64,62,64,74,82,69,64,74,85,87,94,90,89,90,90,89,88,94,105,100,98,96,91,93,93,97,97,98,98,96,97,99,99,97,102,100,97,94,0,0,201,113,0,0,
12,9,1,5,0,0,0,0,0,0,0,0,74,0,25,57,57,57,113,0,59,59,56,56,57,0,0,0,0,0,0,0,0,0,0,0,87,93,105,103,102,101,99,101,101,96,97,99,104,102,93,100,102,95,95,96,100,104,98,99,105,105,107,106,107,104,103,107,106,100,105,100,108,101,103,100,96,98,91,92,101,107,104,104,103,102,0,0,201,150,0,0,
12,9,1,10,0,0,0,0,0,0,0,0,74,0,121,121,57,121,121,0,58,58,58,57,59,0,0,0,0,0,0,0,0,0,0,0,94,100,100,90,89,99,98,101,97,99,105,100,97,103,99,99,94,90,99,100,100,99,100,99,97,101,101,97,100,100,101,99,107,110,103,96,97,104,101,86,95,94,97,95,91,96,98,103,97,97,0,0,201,150,0,0,
12,9,1,15,0,0,0,16,0,0,0,0,64,0,121,105,49,121,25,0,59,64,59,59,59,0,0,0,0,0,0,0,0,0,0,0,101,99,99,100,96,93,94,93,94,101,99,100,83,96,103,103,102,99,93,79,86,105,106,100,102,96,99,93,107,105,98,89,88,92,103,104,103,98,96,96,96,95,101,99,99,99,100,105,98,97,0,0,201,96,0,0,
12,9,1,20,0,0,0,11,0,0,0,0,64,0,25,25,25,113,81,0,60,61,61,63,63,0,0,0,0,0,0,0,0,0,0,0,94,91,92,100,96,97,97,99,98,100,100,95,93,93,96,98,98,97,92,93,95,91,96,95,96,99,99,97,94,94,94,98,96,94,93,95,94,96,95,94,92,84,97,102,106,106,105,98,93,90,0,0,201,113,0,0,
12,9,1,25,0,0,0,0,0,0,0,0,54,0,57,57,145,25,85,0,62,62,64,62,63,0,0,0,0,0,0,0,0,0,0,0,94,92,89,92,93,97,95,94,94,88,88,91,98,92,93,91,96,94,96,93,95,96,93,93,93,93,93,94,94,93,96,97,96,93,93,93,94,94,95,92,93,94,88,93,92,90,91,93,89,89,0,0,201,150,0,0,
12,9,1,30,0,0,0,0,0,0,0,0,54,0,57,25,25,89,25,0,63,64,63,63,63,0,0,0,0,0,0,0,0,0,0,0,86,91,94,91,92,95,90,90,91,92,89,91,93,95,92,91,91,90,90,91,93,92,93,91,94,92,93,92,88,93,90,92,91,90,92,90,91,92,93,91,91,93,92,92,91,96,93,88,93,93,0,0,201,150,0,0,
12,9,1,35,0,0,0,13,0,0,0,0,54,0,121,25,57,25,113,0,64,64,64,65,68,0,0,0,0,0,0,0,0,0,0,0,94,94,86,88,90,96,94,91,90,91,93,90,90,90,91,91,93,93,93,90,88,91,86,86,85,89,89,91,90,89,90,91,87,89,88,91,90,88,89,91,90,88,89,89,86,89,91,91,91,91,0,0,201,140,0,0,
12,9,1,40,0,0,0,0,0,0,0,0,74,0,25,57,113,57,25,0,61,63,62,62,62,0,0,0,0,0,0,0,0,0,0,0,92,105,97,95,93,96,95,93,93,96,95,93,96,93,89,94,93,92,90,91,93,96,93,88,81,106,100,98,98,93,92,95,94,93,96,91,95,94,98,96,99,96,96,94,93,91,93,93,95,93,0,0,201,150,0,0,
12,9,1,45,0,0,0,0,0,0,0,0,74,0,25,25,25,121,121,0,62,62,62,62,62,0,0,0,0,0,0,0,0,0,0,0,93,95,94,95,98,97,93,97,94,91,94,99,94,94,92,96,93,93,90,96,95,98,97,93,90,94,91,96,90,93,91,94,85,89,93,93,101,95,94,93,93,96,93,95,93,96,95,94,91,93,0,0,201,150,0,0,
12,9,1,50,0,0,0,12,0,0,0,0,74,0,25,25,85,121,81,0,61,61,63,58,57,0,0,0,0,0,0,0,0,0,0,0,91,98,94,96,93,92,91,90,95,99,96,93,94,95,93,93,95,97,97,92,93,97,95,94,95,91,95,88,85,85,97,112,109,111,112,102,91,87,94,102,91,97,107,101,101,96,100,96,107,106,0,0,201,81,0,0,
12,9,1,55,0,0,0,0,0,0,0,0,72,0,121,57,81,25,113,0,59,59,57,60,62,0,0,0,0,0,0,0,0,0,0,0,100,97,96,100,99,103,96,98,85,94,99,98,96,94,98,99,99,96,94,104,106,105,95,94,105,103,103,98,91,101,100,95,95,98,97,97,99,99,98,88,86,85,91,100,97,93,98,100,95,92,0,0,201,149,0,0,
12,9,2,0,0,0,0,4,0,0,0,0,72,0,57,57,113,9,139,0,63,62,66,68,77,0,0,0,0,0,0,0,0,0,0,0,91,92,96,90,85,96,98,91,93,93,88,85,101,95,93,98,98,93,91,90,96,95,91,91,88,85,86,85,78,73,73,83,83,86,85,91,91,88,84,80,80,79,76,75,75,74,69,66,60,60,0,0,201,141,0,0,
12,9,2,5,0,0,0,6,0,0,0,0,72,0,111,121,105,17,113,0,79,64,66,63,66,0,0,0,0,0,0,0,0,0,0,0,66,66,65,64,71,74,80,75,67,89,97,90,89,90,91,95,95,91,88,87,82,88,91,91,92,88,85,90,95,88,87,93,100,98,93,92,95,94,90,87,85,91,89,89,91,89,90,87,79,78,0,0,201,146,0,0,
12,9,2,10,0,0,0,0,0,0,0,0,72,0,121,113,17,121,113,0,64,62,67,68,71,0,0,0,0,0,0,0,0,0,0,0,91,95,91,92,91,88,91,89,86,91,84,83,99,96,90,90,88,86,96,93,96,90,90,87,91,89,87,83,82,83,83,86,92,89,88,88,85,82,83,81,77,78,77,81,81,78,85,85,85,85,0,0,201,150,0,0,
12,9,2,15,0,0,0,4,0,0,0,0,72,0,121,17,17,105,81,0,66,69,73,78,68,0,0,0,0,0,0,0,0,0,0,0,78,79,74,82,93,99,97,93,90,91,93,89,85,87,81,86,83,83,84,84,83,82,84,79,78,78,78,76,77,76,76,78,77,75,78,77,77,75,73,71,69,78,92,95,91,90,92,92,88,88,0,0,201,109,0,0,
12,9,2,20,0,0,0,4,0,0,0,0,74,0,25,113,21,21,25,0,65,65,65,62,63,0,0,0,0,0,0,0,0,0,0,0,93,94,91,90,86,89,86,90,90,93,95,96,91,90,89,88,92,91,87,83,78,75,91,102,102,98,95,93,90,83,69,88,95,99,98,99,90,93,90,91,95,94,92,93,91,95,93,89,94,97,0,0,201,93,0,0,
12,9,2,25,0,0,0,4,0,0,0,0,74,0,25,85,113,25,121,0,62,64,63,61,62,0,0,0,0,0,0,0,0,0,0,0,90,98,97,96,97,95,96,93,94,87,84,79,83,103,100,101,100,93,88,88,83,81,96,96,95,95,93,86,87,86,90,98,96,96,94,96,97,100,95,93,90,83,92,94,90,94,102,97,94,88,0,0,201,150,0,0,
12,9,2,30,0,0,0,8,0,0,0,0,74,0,145,121,107,81,57,0,61,63,68,61,64,0,0,0,0,0,0,0,0,0,0,0,58,65,69,77,99,99,93,93,93,91,92,91,93,92,92,93,92,92,92,88,83,88,89,90,91,86,80,79,75,101,97,95,95,98,96,93,88,88,93,94,93,91,93,90,86,94,90,90,88,93,0,0,201,149,0,0,
12,9,2,35,0,0,0,7,0,0,0,0,74,0,85,57,25,25,51,0,63,62,63,63,63,0,0,0,0,0,0,0,0,0,0,0,90,92,91,92,89,80,82,94,100,92,97,95,90,98,95,92,91,90,97,94,91,87,91,94,91,91,97,94,90,93,95,94,92,90,96,91,89,86,80,79,79,74,83,94,97,100,102,98,93,90,0,0,201,120,0,0,
12,9,2,40,0,0,0,0,0,0,0,0,54,0,25,25,25,25,25,0,62,62,63,63,63,0,0,0,0,0,0,0,0,0,0,0,101,99,94,89,90,92,92,87,95,95,97,93,90,88,89,94,94,92,93,97,95,93,91,93,91,90,91,89,91,92,91,93,97,91,90,92,92,91,90,92,89,92,91,91,89,92,98,91,98,99,0,0,201,150,0,0,
12,9,2,45,0,0,0,6,0,0,0,0,74,0,25,113,121,17,121,0,63,67,63,64,65,0,0,0,0,0,0,0,0,0,0,0,94,93,94,91,92,92,89,91,93,93,91,92,92,92,86,81,74,77,90,96,94,96,90,93,95,91,93,93,88,82,81,89,91,93,91,94,95,94,88,93,91,89,89,89,87,89,90,89,86,88,0,0,201,106,0,0,
12,9,2,50,0,0,0,0,0,0,0,0,74,0,57,25,25,25,121,0,66,66,67,67,66,0,0,0,0,0,0,0,0,0,0,0,88,88,85,82,82,82,88,85,86,86,89,90,87,85,91,87,86,88,90,89,89,88,90,90,88,87,85,87,88,82,83,84,85,88,91,89,89,89,87,86,88,89,88,83,82,91,94,90,85,86,0,0,201,150,0,0,
12,9,2,55,0,0,0,14,0,0,0,0,74,0,113,85,121,25,121,0,68,68,65,62,64,0,0,0,0,0,0,0,0,0,0,0,86,85,84,85,83,86,85,86,84,75,68,65,84,100,99,98,91,86,92,94,91,91,93,94,88,90,91,85,91,92,92,99,94,93,94,91,94,95,91,91,90,92,94,88,85,86,94,90,95,93,0,0,201,150,0,0,
12,9,3,0,0,0,0,0,0,0,0,0,74,0,25,25,25,121,25,0,64,65,67,64,63,0,0,0,0,0,0,0,0,0,0,0,81,77,89,94,94,93,94,91,90,88,92,91,91,93,88,83,89,89,90,90,92,86,86,88,87,86,90,87,85,84,90,88,92,94,88,89,92,96,94,93,92,83,93,94,91,91,89,90,89,88,0,0,201,150,0,0,
12,9,3,5,0,0,0,16,0,0,0,0,74,0,121,121,117,81,25,0,64,64,66,67,66,0,0,0,0,0,0,0,0,0,0,0,89,88,93,88,91,91,90,89,95,91,90,90,85,91,88,83,90,93,89,91,85,84,94,93,94,91,89,71,65,65,66,82,94,97,94,93,90,90,86,87,90,88,91,92,86,88,89,86,85,88,0,0,201,82,0,0,
12,9,3,10,0,0,0,6,0,0,0,0,53,0,25,25,25,109,25,0,66,65,67,71,65,0,0,0,0,0,0,0,0,0,0,0,90,91,87,84,87,94,88,85,88,88,89,86,90,86,91,93,91,89,87,87,85,88,85,85,89,90,88,89,83,84,82,84,85,84,81,81,82,90,90,91,92,91,89,87,91,91,87,88,87,88,0,0,201,149,0,0,
12,9,3,15,0,0,0,6,0,0,0,0,53,0,25,25,145,147,81,0,65,65,65,66,61,0,0,0,0,0,0,0,0,0,0,0,88,87,89,90,85,95,93,89,87,91,92,90,89,86,91,87,86,95,88,91,88,87,87,82,82,89,92,89,93,89,87,87,86,87,90,90,90,95,97,97,95,98,97,93,91,91,96,96,93,94,0,0,201,150,0,0,
12,9,3,20,0,0,0,5,0,0,0,0,53,0,25,113,85,137,121,0,62,63,64,76,66,0,0,0,0,0,0,0,0,0,0,0,96,93,87,93,95,90,92,95,98,97,94,93,93,91,94,92,92,96,97,96,94,92,91,94,89,90,91,92,90,80,70,63,73,85,80,82,81,79,72,86,89,92,81,88,87,86,91,90,87,87,0,0,201,132,0,0,
12,9,3,25,0,0,0,0,0,0,0,0,53,0,25,25,113,113,49,0,65,64,65,64,64,0,0,0,0,0,0,0,0,0,0,0,92,91,90,88,86,86,90,91,92,92,93,91,91,90,93,89,94,91,91,90,88,89,88,87,93,94,94,92,89,89,86,90,99,97,94,93,93,98,82,79,79,85,90,93,99,96,94,92,88,83,0,0,201,150,0,0,
12,9,3,30,0,0,0,0,0,0,0,0,53,0,25,119,17,121,25,0,62,66,63,64,65,0,0,0,0,0,0,0,0,0,0,0,93,95,94,93,92,95,91,89,88,91,88,94,91,95,97,94,80,80,86,94,93,97,91,90,93,95,91,89,90,90,92,90,95,90,93,90,91,92,87,90,86,89,90,90,94,93,91,90,91,92,0,0,201,150,0,0,
12,9,3,35,0,0,0,11,0,0,0,0,53,0,25,57,121,57,141,0,66,66,65,65,69,0,0,0,0,0,0,0,0,0,0,0,92,87,83,86,85,87,93,88,92,90,85,88,86,85,90,92,86,86,92,96,93,95,87,92,86,85,89,91,90,88,86,88,91,89,90,87,88,90,93,90,87,89,86,94,90,65,54,59,68,68,0,0,201,143,0,0,
12,9,3,40,0,0,0,5,0,0,0,0,74,0,49,25,145,117,121,0,67,62,65,63,60,0,0,0,0,0,0,0,0,0,0,0,70,92,95,89,86,93,90,95,96,93,96,92,94,91,93,91,94,90,95,99,98,90,97,94,88,88,87,84,81,94,97,96,95,90,86,90,89,87,88,94,102,99,105,99,100,94,94,92,95,92,0,0,201,149,0,0,
12,9,3,45,0,0,0,2,0,0,0,0,74,0,121,113,21,117,89,0,62,62,61,59,59,0,0,0,0,0,0,0,0,0,0,0,94,92,96,90,95,99,90,94,91,90,94,95,95,94,92,102,107,99,94,91,91,100,99,101,102,101,94,89,90,90,96,105,104,101,101,99,96,102,104,88,83,103,99,106,105,98,96,94,94,98,0,0,201,150,0,0,
12,9,3,50,0,0,0,4,0,0,0,0,74,0,121,113,113,111,113,0,63,68,78,75,66,0,0,0,0,0,0,0,0,0,0,0,97,103,100,94,89,92,92,89,84,79,76,77,81,80,84,90,84,86,93,86,83,85,74,70,67,70,69,71,72,72,72,72,73,76,87,90,92,100,98,93,92,92,88,88,79,72,89,95,89,85,0,0,201,150,0,0,
12,9,3,55,0,37,0,107,0,31,0,19,74,0,109,113,111,117,89,0,73,68,83,81,66,0,0,0,0,0,0,0,0,0,0,0,81,81,84,78,88,78,72,71,72,72,76,87,83,77,77,90,90,84,82,79,79,69,58,51,50,56,62,65,72,72,72,72,81,84,75,89,90,90,89,85,87,84,85,87,91,89,94,89,98,104,0,0,201,56,0,0,
12,9,4,0,0,0,0,11,0,0,0,0,54,0,85,121,25,25,25,0,67,65,63,66,63,0,0,0,0,0,0,0,0,0,0,0,96,92,89,90,87,92,102,102,97,94,93,88,90,92,91,86,86,90,90,88,90,91,92,87,88,94,98,94,94,94,93,91,90,88,92,92,90,82,75,88,93,97,94,92,91,92,88,91,95,92,0,0,201,140,0,0,
12,9,4,5,0,0,0,10,0,0,0,0,54,0,105,25,81,141,9,0,68,64,64,75,70,0,0,0,0,0,0,0,0,0,0,0,88,82,82,83,94,87,83,88,97,93,91,97,89,88,92,91,94,91,93,92,88,97,94,89,94,91,96,93,86,86,87,87,85,82,94,100,78,77,77,73,72,83,86,84,84,82,83,81,85,92,0,0,201,104,0,0,
12,9,4,10,0,0,0,16,0,0,0,0,74,0,57,85,85,137,25,0,67,68,67,63,63,0,0,0,0,0,0,0,0,0,0,0,90,92,86,83,85,84,88,92,86,82,76,75,81,90,95,98,91,89,87,89,92,91,88,82,90,88,85,83,82,94,96,102,99,98,92,97,99,95,90,90,95,96,93,91,83,92,93,96,97,97,0,0,201,149,0,0,
12,9,4,15,0,13,0,55,0,10,0,7,74,0,145,121,145,111,109,0,63,64,66,90,83,0,0,0,0,0,0,0,0,0,0,0,94,92,90,92,95,97,95,93,97,86,85,98,99,92,89,90,95,94,93,93,91,91,90,87,92,92,85,86,88,86,83,83,83,83,83,66,62,78,80,81,81,71,65,65,68,83,84,82,85,85,0,0,201,95,0,0,
12,9,4,20,0,0,0,6,0,0,0,0,72,0,25,121,77,25,25,0,66,66,70,64,65,0,0,0,0,0,0,0,0,0,0,0,86,87,88,86,90,86,90,91,89,89,88,87,88,91,90,89,90,86,86,86,92,85,80,75,68,71,85,96,101,96,93,90,91,96,97,95,89,88,88,86,80,85,85,90,97,95,96,90,93,92,0,0,201,76,0,0,
12,9,4,25,0,0,0,52,0,0,0,0,74,0,85,145,141,109,109,0,67,65,69,65,77,0,0,0,0,0,0,0,0,0,0,0,97,96,96,84,87,99,96,93,91,95,92,91,89,93,96,95,94,93,94,91,75,78,78,72,65,61,60,65,80,79,79,81,89,96,93,88,82,86,93,92,89,75,75,81,82,73,89,94,89,111,0,0,201,117,0,0,
12,9,4,30,0,0,0,49,0,0,0,0,54,0,109,137,25,25,137,2,85,72,65,61,65,0,0,0,0,0,0,0,0,0,0,0,130,133,102,89,90,91,91,86,83,80,70,72,78,79,79,79,79,76,83,91,88,89,89,89,91,95,96,95,89,91,95,96,98,98,96,96,94,94,94,92,97,98,98,99,103,92,99,103,99,99,0,0,201,111,0,0,
12,9,4,35,0,0,0,0,0,0,0,0,74,0,25,25,25,25,25,0,64,63,62,63,63,0,0,0,0,0,0,0,0,0,0,0,94,88,92,93,93,96,98,94,96,96,95,90,93,91,94,95,90,100,99,93,96,94,95,95,96,95,92,97,92,92,96,98,93,95,96,94,95,91,95,96,96,95,92,93,94,93,92,88,92,96,0,0,201,150,0,0,
12,9,4,40,0,0,0,18,0,0,0,0,53,0,145,25,117,113,81,0,70,63,72,65,64,0,0,0,0,0,0,0,0,0,0,0,89,85,79,75,72,91,92,96,99,95,92,97,94,93,91,91,90,92,91,89,88,86,81,81,78,82,87,90,88,86,88,94,95,94,95,90,88,86,83,84,88,100,96,91,88,84,91,96,97,96,0,0,201,86,0,0,
12,9,4,45,0,0,0,0,0,0,0,0,53,0,105,113,25,25,25,0,67,64,61,60,61,0,0,0,0,0,0,0,0,0,0,0,92,91,88,86,87,86,87,91,95,93,92,91,90,90,87,85,100,99,93,95,95,95,95,93,94,94,97,96,93,94,94,98,98,96,98,96,92,97,95,101,98,94,97,93,94,95,93,94,93,94,0,0,201,150,0,0,
12,9,4,50,0,0,0,0,0,0,0,0,53,0,25,25,25,25,25,0,62,62,62,63,62,0,0,0,0,0,0,0,0,0,0,0,91,95,95,96,92,97,96,96,96,91,94,96,92,96,94,96,94,91,93,93,96,95,97,95,93,92,94,93,95,94,93,90,92,95,94,96,93,92,98,93,93,93,94,97,95,90,96,93,93,95,0,0,201,150,0,0,
12,9,4,55,0,0,0,0,0,0,0,0,53,0,121,113,121,25,121,0,62,60,61,61,61,0,0,0,0,0,0,0,0,0,0,0,97,95,97,94,93,93,91,96,91,92,94,95,100,97,97,98,95,95,93,95,98,99,97,92,93,92,98,98,95,94,95,98,93,94,95,95,94,92,94,97,96,94,95,91,99,99,93,95,92,93,0,0,201,150,0,0,
12,9,5,0,0,0,0,0,0,0,0,0,53,0,25,121,25,25,25,0,61,61,61,61,61,0,0,0,0,0,0,0,0,0,0,0,97,93,98,96,92,95,100,95,93,100,92,97,96,90,101,97,96,103,101,98,94,95,96,96,96,93,93,95,97,94,100,95,92,93,100,97,93,96,99,95,95,94,95,96,96,98,101,97,98,97,0,0,201,150,0,0,
12,9,5,5,0,0,0,0,0,0,0,0,53,0,113,121,121,25,25,0,61,61,61,61,62,0,0,0,0,0,0,0,0,0,0,0,94,93,93,93,94,91,95,98,96,88,100,93,94,96,98,95,93,95,91,92,93,100,98,93,94,92,95,99,96,95,92,94,93,95,101,94,95,92,97,98,95,93,94,94,96,89,94,98,94,95,0,0,201,150,0,0,
12,9,5,10,0,0,0,21,0,0,0,0,74,0,141,121,25,25,21,0,70,61,59,59,59,0,0,0,0,0,0,0,0,0,0,0,91,89,89,87,87,74,62,72,98,103,90,92,90,95,95,94,99,94,94,94,100,94,97,100,102,107,100,98,98,98,91,101,101,102,99,100,96,96,96,98,95,89,89,103,106,105,99,102,100,100,0,0,201,136,0,0,
12,9,5,15,0,0,0,0,0,0,0,0,74,0,121,25,89,25,25,0,60,58,58,60,59,0,0,0,0,0,0,0,0,0,0,0,96,96,98,98,101,99,92,96,96,93,98,102,102,98,97,98,95,108,103,96,92,94,95,98,101,98,92,99,106,105,101,97,94,98,98,101,97,93,94,93,95,96,96,98,97,96,95,99,102,102,0,0,201,150,0,0,
12,9,5,20,0,0,0,1,0,0,0,0,74,0,117,81,113,121,113,0,64,60,62,59,63,0,0,0,0,0,0,0,0,0,0,0,101,101,98,96,94,96,96,96,89,67,88,103,99,104,104,105,101,94,95,95,100,95,90,93,96,90,88,97,94,91,97,102,105,100,100,95,87,90,104,99,94,97,95,93,92,91,89,82,75,76,0,0,201,135,0,0,
12,9,5,25,0,0,0,1,0,0,0,0,74,0,17,113,105,121,73,0,59,61,64,60,65,0,0,0,0,0,0,0,0,0,0,0,100,103,98,95,83,69,89,103,101,98,98,87,82,80,91,103,98,96,92,96,94,91,85,72,66,75,101,99,102,95,98,99,92,94,93,88,93,99,103,103,105,104,98,93,88,82,83,74,74,74,0,0,201,150,0,0,
12,9,5,30,0,0,0,0,0,0,0,0,74,0,113,113,113,105,17,0,65,70,69,89,80,0,0,0,0,0,0,0,0,0,0,0,90,97,95,93,92,85,68,64,84,95,92,88,86,84,87,79,79,76,80,81,75,84,94,94,87,90,86,83,82,76,74,75,73,70,61,63,63,66,61,59,62,71,75,73,74,73,78,81,81,80,0,0,201,150,0,0,
12,9,5,35,0,0,0,2,0,0,0,0,74,0,77,41,105,105,25,0,76,76,82,70,68,0,0,0,0,0,0,0,0,0,0,0,76,77,76,75,76,77,76,78,74,73,70,72,78,81,86,91,81,81,71,71,69,70,71,71,69,73,71,73,73,65,73,76,87,88,92,93,91,89,85,78,81,89,92,92,88,86,89,83,85,85,0,0,201,149,0,0,
12,9,5,40,0,0,0,1,0,0,0,0,74,0,49,121,105,105,49,0,67,65,69,69,66,0,0,0,0,0,0,0,0,0,0,0,89,91,86,84,84,85,88,90,90,86,92,88,89,90,90,88,82,84,96,87,90,81,82,82,74,72,86,87,86,85,81,80,81,83,92,90,92,92,88,79,91,99,100,90,88,87,88,88,90,92,0,0,201,150,0,0,
12,9,5,45,0,0,0,1,0,0,0,0,74,0,89,121,89,149,147,0,65,67,65,65,71,0,0,0,0,0,0,0,0,0,0,0,98,93,90,87,85,89,82,82,91,92,85,88,85,87,86,85,85,85,85,82,87,88,85,88,93,89,96,93,91,90,87,91,92,95,94,90,90,89,124,143,143,137,132,115,81,85,94,95,92,90,0,0,201,149,0,0,
12,9,5,50,0,0,0,0,0,0,0,0,74,0,117,121,105,77,89,0,63,65,61,69,62,0,0,0,0,0,0,0,0,0,0,0,98,88,88,86,88,101,97,93,93,88,87,92,88,90,88,73,88,99,103,94,92,92,98,98,107,97,92,88,91,94,92,91,90,90,84,75,93,98,96,84,92,102,102,102,93,91,89,86,96,96,0,0,201,150,0,0,
12,9,5,55,0,0,0,18,0,0,0,0,74,0,113,109,113,25,25,0,67,68,63,66,64,0,0,0,0,0,0,0,0,0,0,0,88,90,75,73,87,91,91,88,85,83,79,72,63,86,91,74,78,98,96,98,97,87,92,93,97,99,93,87,89,86,88,89,90,88,90,88,88,92,88,88,88,88,89,97,94,93,92,90,91,90,0,0,201,132,0,0,
12,9,6,0,0,0,0,0,0,0,0,0,74,0,25,113,85,25,109,0,65,69,69,67,69,0,0,0,0,0,0,0,0,0,0,0,88,88,87,93,94,91,94,87,86,87,87,84,83,83,80,83,78,84,90,91,91,87,81,80,80,86,84,89,87,87,84,84,84,92,89,90,88,90,88,82,75,82,84,88,88,86,88,88,88,93,0,0,201,150,0,0,
12,9,6,5,0,0,0,0,0,0,0,0,74,0,25,17,113,137,81,0,67,70,67,69,68,0,0,0,0,0,0,0,0,0,0,0,90,92,87,88,86,85,83,83,87,92,84,82,83,87,87,87,82,81,78,75,75,79,90,92,92,91,87,89,91,82,83,84,82,84,82,79,76,86,84,89,86,86,92,89,87,79,79,83,90,93,0,0,201,150,0,0,
12,9,6,10,0,0,0,8,0,0,0,0,74,0,25,113,117,119,113,0,68,65,68,63,63,0,0,0,0,0,0,0,0,0,0,0,86,80,91,88,91,90,83,83,83,88,89,87,85,88,96,98,96,93,94,91,90,96,91,89,89,89,89,89,96,96,86,85,80,73,73,74,79,78,77,81,90,90,82,80,75,75,77,71,71,73,0,0,201,149,0,0,
12,9,6,15,0,0,0,9,0,0,0,0,74,0,117,21,115,137,147,0,63,66,64,64,68,0,0,0,0,0,0,0,0,0,0,0,69,72,77,77,80,85,86,79,75,71,74,77,74,75,73,69,64,66,64,63,64,64,64,64,64,64,64,65,69,69,67,65,64,61,59,60,64,61,62,61,60,60,60,59,55,54,53,53,53,58,0,0,201,143,0,0,
12,9,6,20,0,0,0,9,0,0,0,0,74,0,89,137,81,137,21,0,68,66,68,81,71,0,0,0,0,0,0,0,0,0,0,0,92,89,91,91,87,88,82,80,83,91,91,85,89,89,88,88,87,86,90,95,92,88,83,79,82,85,87,85,82,78,78,78,80,80,77,75,76,75,87,99,108,90,92,96,89,86,87,89,88,88,0,0,201,115,0,0,
12,9,6,25,0,0,0,17,0,0,0,0,63,0,137,139,113,25,25,0,74,78,64,64,63,0,0,0,0,0,0,0,0,0,0,0,86,88,85,80,76,73,76,76,76,86,93,81,78,78,78,83,93,91,91,93,89,86,88,95,93,92,89,91,86,87,86,95,93,89,95,93,91,90,89,88,88,90,92,97,98,92,97,96,93,93,0,0,201,89,0,0,
12,9,6,30,0,0,0,11,0,0,0,0,63,0,25,25,25,121,113,0,64,63,62,61,65,0,0,0,0,0,0,0,0,0,0,0,92,91,91,92,92,93,90,87,88,89,91,92,94,93,95,96,91,95,92,91,96,91,95,94,94,99,93,82,88,99,95,96,89,84,88,94,94,99,99,101,95,78,67,88,96,94,94,93,89,89,0,0,201,119,0,0,
12,9,6,35,0,0,0,0,0,0,0,0,53,0,121,25,25,113,121,0,65,65,62,65,63,0,0,0,0,0,0,0,0,0,0,0,88,89,87,86,87,89,89,91,91,90,91,91,93,89,95,90,86,87,89,92,89,93,93,97,93,97,94,96,91,89,87,94,88,85,92,84,85,88,88,91,90,91,90,91,91,88,91,92,96,97,0,0,201,150,0,0,
12,9,6,40,0,0,0,0,0,0,0,0,53,0,121,25,25,25,25,0,65,65,65,65,65,0,0,0,0,0,0,0,0,0,0,0,92,92,88,90,90,89,85,91,89,87,90,90,89,89,92,93,88,91,88,94,93,93,91,93,89,86,90,88,88,87,92,90,91,89,87,90,95,89,88,88,86,92,92,88,91,88,92,91,88,88,0,0,201,150,0,0,
12,9,6,45,0,0,0,0,0,0,0,0,53,0,25,25,25,25,25,0,63,65,66,65,65,0,0,0,0,0,0,0,0,0,0,0,77,91,101,93,93,91,88,91,90,93,90,91,90,89,90,90,90,89,87,95,90,87,88,88,86,91,89,90,89,87,92,89,91,91,92,91,89,88,89,91,87,87,88,90,90,91,90,87,89,92,0,0,201,150,0,0,
12,9,6,50,0,0,0,0,0,0,0,0,53,0,25,121,121,121,25,0,65,65,65,65,65,0,0,0,0,0,0,0,0,0,0,0,89,91,87,92,92,89,90,90,88,86,95,93,87,86,82,84,92,91,93,89,87,89,89,88,90,89,88,89,86,85,85,89,87,88,89,89,89,86,86,91,90,90,90,88,91,91,90,88,88,88,0,0,201,150,0,0,
12,9,6,55,0,0,0,0,0,0,0,0,53,0,113,25,25,25,25,0,66,65,66,65,65,0,0,0,0,0,0,0,0,0,0,0,86,90,90,86,80,77,84,92,91,89,88,89,89,96,91,87,89,87,89,88,89,89,88,90,89,92,89,91,88,87,89,90,90,91,88,94,89,88,89,88,92,90,91,91,91,88,88,88,88,88,0,0,201,150,0,0,
12,9,7,0,0,0,0,0,0,0,0,0,53,0,25,25,25,25,25,0,66,65,66,67,66,0,0,0,0,0,0,0,0,0,0,0,89,92,90,88,88,88,88,89,88,90,89,88,90,90,91,90,91,90,88,88,91,92,88,89,89,88,90,88,88,85,88,90,89,89,85,92,90,83,87,88,88,90,89,85,92,92,86,84,91,90,0,0,201,150,0,0,
12,9,7,5,0,0,0,0,0,0,0,0,53,0,25,25,25,25,17,0,67,67,66,67,65,0,0,0,0,0,0,0,0,0,0,0,89,88,87,88,85,86,86,89,87,83,83,85,88,93,91,86,87,86,88,84,88,89,85,84,86,90,88,90,93,89,87,88,88,87,87,86,86,89,89,85,98,93,88,90,87,88,88,90,87,86,0,0,201,150,0,0,
12,9,7,10,0,0,0,6,0,0,0,0,53,0,25,25,125,25,137,0,66,66,66,66,66,0,0,0,0,0,0,0,0,0,0,0,87,88,90,87,88,89,87,88,88,91,90,86,89,88,86,88,88,97,92,88,86,88,90,89,86,91,88,89,86,86,89,86,87,86,89,89,90,89,89,91,89,87,90,88,87,89,86,88,86,80,0,0,201,146,0,0,
12,9,7,15,0,0,0,6,0,0,0,0,74,0,145,25,25,25,113,0,73,60,62,61,66,0,0,0,0,0,0,0,0,0,0,0,84,65,60,85,101,97,94,89,91,78,70,88,102,106,98,95,97,94,91,93,90,90,88,94,98,98,101,97,95,97,99,99,93,95,91,94,94,97,97,91,88,92,96,99,96,95,72,75,85,88,0,0,201,123,0,0,
12,9,7,20,0,0,0,3,0,0,0,0,74,0,85,117,85,17,25,0,61,62,65,60,61,0,0,0,0,0,0,0,0,0,0,0,94,94,93,103,106,99,96,97,97,90,85,80,92,102,100,91,92,90,87,101,97,96,93,91,83,66,73,94,106,99,99,100,99,98,97,92,93,96,90,102,101,99,94,92,91,98,96,93,90,88,0,0,201,150,0,0,
12,9,7,25,0,0,0,0,0,0,0,0,74,0,117,25,113,117,25,0,61,64,65,65,67,0,0,0,0,0,0,0,0,0,0,0,88,98,96,98,94,101,95,92,91,90,88,95,93,92,91,91,89,88,95,93,93,91,89,89,93,88,88,90,90,99,94,93,92,90,90,86,81,85,89,86,80,76,84,91,89,90,89,88,85,85,0,0,201,148,0,0,
12,9,7,30,0,0,0,5,0,0,0,0,74,0,17,105,25,105,147,0,70,75,66,70,67,0,0,0,0,0,0,0,0,0,0,0,87,83,85,81,84,83,83,85,79,79,77,79,78,75,76,72,70,68,80,91,88,92,89,86,87,87,87,90,89,88,86,89,88,77,86,85,81,72,76,98,96,90,94,93,88,97,97,92,96,95,0,0,201,99,0,0,
12,9,7,35,0,0,0,1,0,0,0,0,74,0,85,113,25,57,147,0,66,64,64,65,68,0,0,0,0,0,0,0,0,0,0,0,74,89,100,95,93,91,95,92,95,91,89,92,88,89,87,89,76,78,94,90,95,93,93,92,92,91,94,91,87,91,85,88,95,90,91,92,86,90,93,90,91,90,89,89,91,86,82,76,75,73,0,0,201,126,0,0,
12,9,7,40,0,0,0,4,0,0,0,0,74,0,151,113,25,137,137,0,65,67,66,84,68,0,0,0,0,0,0,0,0,0,0,0,68,69,77,83,76,64,71,94,95,96,92,90,90,94,89,79,65,69,94,95,94,93,90,90,91,88,90,85,87,85,87,84,77,63,71,72,74,73,73,73,77,79,78,80,85,123,154,154,154,154,0,0,201,150,0,0,
12,9,7,45,0,0,0,3,0,0,0,0,74,0,113,121,145,147,117,0,64,63,65,65,61,0,0,0,0,0,0,0,0,0,0,0,154,154,154,141,91,93,92,89,86,93,91,88,91,97,92,90,91,91,91,89,75,74,70,82,88,89,90,88,86,77,80,74,87,89,92,95,93,93,96,95,94,94,96,98,94,91,91,97,97,98,0,0,201,92,0,0,
12,9,7,50,0,0,0,1,0,0,0,0,74,0,147,121,137,25,57,0,63,62,65,61,61,0,0,0,0,0,0,0,0,0,0,0,98,97,92,92,90,94,93,93,90,92,92,91,88,87,94,91,88,94,100,95,86,89,92,93,84,86,77,70,77,87,96,98,93,90,99,97,97,93,93,92,92,96,98,88,86,97,100,93,91,85,0,0,201,141,0,0,
12,9,7,55,0,0,0,6,0,0,0,0,74,0,137,117,113,113,25,0,66,62,64,63,62,0,0,0,0,0,0,0,0,0,0,0,71,58,79,93,99,99,101,99,94,90,90,87,90,103,93,93,94,92,92,94,96,88,94,89,92,92,91,86,91,99,93,91,87,77,75,75,83,95,106,101,93,86,100,97,96,93,92,92,92,95,0,0,201,149,0,0,
12,9,8,0,0,0,0,4,0,0,0,0,63,0,25,137,57,25,25,0,64,68,62,64,64,0,0,0,0,0,0,0,0,0,0,0,92,92,92,92,92,90,90,87,94,91,84,84,91,86,76,68,63,84,95,99,97,91,95,95,89,90,100,92,94,90,92,95,92,93,93,89,90,87,90,95,92,90,92,92,92,89,93,92,90,88,0,0,201,103,0,0,
12,9,8,5,0,0,0,11,0,0,0,0,63,0,145,25,121,121,137,0,65,64,64,62,70,0,0,0,0,0,0,0,0,0,0,0,95,93,91,91,90,92,90,93,89,84,79,91,100,95,92,93,92,95,90,93,90,94,91,92,91,87,84,84,91,92,93,97,96,92,90,95,99,91,91,90,82,76,86,82,81,69,74,76,86,85,0,0,201,127,0,0,
12,9,8,10,0,0,0,6,0,0,0,0,63,0,81,145,25,25,121,0,64,66,62,62,64,0,0,0,0,0,0,0,0,0,0,0,85,85,87,89,90,89,89,89,88,89,87,89,95,92,90,84,92,103,99,94,98,93,92,96,89,83,89,95,93,96,92,96,93,97,95,92,93,92,92,93,92,91,86,87,78,92,99,98,97,96,0,0,201,100,0,0,
12,9,8,15,0,0,0,0,0,0,0,0,63,0,25,25,25,29,25,0,62,62,63,63,63,0,0,0,0,0,0,0,0,0,0,0,93,92,91,95,92,93,92,94,95,94,94,93,94,91,97,94,94,96,93,90,90,92,94,94,94,97,94,91,89,93,93,91,92,91,93,93,96,92,93,90,96,96,94,92,95,95,94,91,91,90,0,0,201,150,0,0,
12,9,8,20,0,0,0,5,0,0,0,0,53,0,145,25,81,29,25,0,67,64,66,62,63,0,0,0,0,0,0,0,0,0,0,0,90,91,90,90,90,88,88,87,82,91,88,90,93,94,95,90,87,92,89,90,87,99,93,90,87,88,87,87,92,91,93,95,95,92,90,93,94,93,97,97,95,92,90,90,93,92,92,93,84,83,0,0,201,128,0,0,
12,9,8,25,0,0,0,0,0,0,0,0,53,0,25,57,25,25,29,0,62,65,64,64,64,0,0,0,0,0,0,0,0,0,0,0,91,92,94,96,97,93,92,89,92,93,88,89,92,90,90,89,86,85,92,93,91,90,90,93,92,90,87,90,88,91,90,88,91,93,91,91,92,91,88,93,91,90,89,90,96,89,92,90,88,88,0,0,201,150,0,0,
12,9,8,30,0,0,0,0,0,0,0,0,53,0,121,25,25,25,17,0,66,65,66,66,67,0,0,0,0,0,0,0,0,0,0,0,90,91,89,90,88,87,87,85,92,86,85,92,92,89,89,90,89,87,87,86,87,88,88,89,92,91,87,88,91,83,89,85,85,84,87,91,92,89,88,90,89,89,88,88,85,89,86,97,98,94,0,0,201,150,0,0,
12,9,8,35,0,0,0,10,0,0,0,0,74,0,57,145,57,25,25,0,64,74,66,66,65,0,0,0,0,0,0,0,0,0,0,0,85,93,97,91,87,89,91,89,91,91,90,89,89,95,90,87,87,59,56,71,90,85,91,89,88,89,89,89,87,91,91,92,89,89,86,87,91,97,85,77,85,94,93,93,91,88,88,87,85,83,0,0,201,102,0,0,
12,9,8,40,0,0,0,4,0,0,0,0,74,0,89,147,25,113,89,0,66,68,64,64,64,0,0,0,0,0,0,0,0,0,0,0,89,82,80,95,90,91,88,89,93,87,87,76,87,94,92,90,90,88,79,81,92,91,96,94,90,83,93,91,93,88,98,95,90,84,94,94,95,90,92,87,83,88,95,94,95,99,93,89,93,96,0,0,201,150,0,0,
12,9,8,45,0,0,0,2,0,0,0,0,74,0,109,81,117,113,17,0,68,71,66,65,67,0,0,0,0,0,0,0,0,0,0,0,88,79,75,83,94,92,93,89,84,78,68,70,90,91,89,87,87,88,86,84,91,86,88,93,82,86,77,68,80,90,93,94,92,92,89,88,88,83,82,83,92,91,83,76,84,93,93,93,93,93,0,0,201,149,0,0,
12,9,8,50,0,0,0,21,0,0,0,0,74,0,85,25,139,81,81,0,65,69,85,67,70,0,0,0,0,0,0,0,0,0,0,0,92,89,89,97,94,87,86,88,88,93,89,88,82,89,91,88,87,84,78,82,98,89,83,77,71,75,73,78,93,75,88,93,97,83,85,89,88,86,81,80,83,85,82,83,89,90,85,82,77,74,0,0,201,143,0,0,
12,9,8,55,0,0,0,2,0,0,0,0,74,0,25,21,113,141,141,0,67,70,70,81,69,0,0,0,0,0,0,0,0,0,0,0,85,89,92,95,92,85,87,89,84,83,93,92,89,89,84,84,84,86,75,68,69,78,82,80,89,91,86,88,86,87,82,83,86,73,60,69,83,85,87,90,90,90,89,90,90,86,97,88,84,84,0,0,201,150,0,0,
12,9,9,0,0,0,0,1,0,0,0,0,74,0,85,141,113,25,81,0,68,72,68,66,71,0,0,0,0,0,0,0,0,0,0,0,76,76,80,81,93,96,94,95,97,97,95,85,85,84,80,63,64,69,76,89,89,87,90,86,83,84,81,76,93,90,93,94,92,91,91,87,80,90,89,86,85,83,72,68,80,86,84,83,79,78,0,0,201,78,0,0,
12,9,9,5,0,0,0,3,0,0,0,0,74,0,137,81,25,57,137,0,67,66,66,72,81,0,0,0,0,0,0,0,0,0,0,0,77,89,90,88,89,89,85,91,91,84,92,93,91,84,79,81,90,91,90,90,85,91,96,90,90,88,87,86,89,85,79,85,73,78,78,85,89,86,84,86,81,82,69,60,64,63,61,66,86,91,0,0,201,106,0,0,
12,9,9,10,0,0,0,8,0,0,0,0,53,0,25,145,17,25,49,0,67,71,66,64,71,0,0,0,0,0,0,0,0,0,0,0,90,92,85,87,86,91,90,86,81,81,77,81,91,84,86,88,75,82,83,98,97,91,88,87,85,83,89,86,91,91,92,92,91,89,90,92,94,91,89,89,87,85,87,84,81,78,80,81,91,85,0,0,201,104,0,0,
12,9,9,15,0,0,0,3,0,0,0,0,53,0,17,17,113,113,77,0,71,74,67,68,73,0,0,0,0,0,0,0,0,0,0,0,83,80,81,78,79,85,85,85,82,83,88,84,82,80,78,77,77,74,72,86,89,90,90,85,88,91,85,88,87,84,84,91,89,93,93,90,89,86,85,84,84,82,89,84,79,83,84,81,79,79,0,0,201,147,0,0,
12,9,9,20,0,0,0,8,0,0,0,0,53,0,141,25,17,49,17,0,77,64,70,70,70,0,0,0,0,0,0,0,0,0,0,0,68,66,82,89,90,79,63,72,80,90,91,92,93,92,95,92,92,89,89,85,83,86,84,87,83,82,79,77,93,92,88,84,84,84,81,83,80,77,83,90,84,84,79,82,82,87,86,84,82,81,0,0,201,135,0,0,
12,9,9,25,0,0,0,58,0,0,0,0,63,0,113,81,49,141,109,0,74,68,69,78,73,0,0,0,0,0,0,0,0,0,0,0,71,81,81,85,86,87,88,65,46,53,67,85,91,95,94,97,96,88,86,87,91,95,91,90,85,82,88,84,84,74,80,90,93,95,95,91,82,73,72,72,75,79,81,68,76,83,88,86,84,84,0,0,201,81,0,0,
12,9,9,30,0,0,0,4,0,0,0,0,63,0,137,141,105,137,133,0,78,72,74,82,92,0,0,0,0,0,0,0,0,0,0,0,88,85,85,82,70,75,75,74,83,82,82,84,87,84,81,80,78,86,79,67,73,77,84,83,86,86,89,89,89,89,85,84,76,74,74,74,72,66,64,62,62,62,62,62,62,60,60,60,62,66,0,0,201,93,0,0,
12,9,9,35,0,0,0,45,0,0,0,0,53,0,141,109,109,109,109,0,93,82,77,74,77,0,0,0,0,0,0,0,0,0,0,0,78,80,80,79,65,62,58,58,78,84,85,84,84,84,84,84,84,84,84,84,86,91,90,90,87,85,74,66,63,71,72,76,80,82,82,77,77,83,82,82,73,74,85,81,79,79,79,73,45,42,0,0,201,92,0,0,
12,9,9,40,0,14,0,150,0,11,0,7,64,0,131,129,131,131,133,0,99,97,103,95,100,0,0,0,0,0,0,0,0,0,0,0,52,55,61,58,62,62,61,61,61,59,52,53,61,61,61,62,58,54,54,54,54,50,49,51,52,56,79,79,79,79,79,79,79,77,77,78,61,57,57,57,57,65,67,47,47,45,43,79,84,84,0,0,201,72,0,0,
12,9,9,45,0,145,1,56,0,122,0,80,64,0,143,141,129,129,109,0,100,112,113,104,103,0,0,0,0,0,0,0,0,0,0,0,84,84,84,84,85,87,87,87,74,49,53,63,65,66,66,66,66,66,66,58,87,70,72,73,72,90,82,79,79,83,96,96,85,60,60,60,60,58,60,64,64,58,56,56,54,59,67,65,52,45,0,0,201,56,0,0,
12,9,9,50,0,251,1,172,0,214,0,140,64,0,129,133,129,129,129,0,102,106,113,116,109,0,0,0,0,0,0,0,0,0,0,0,61,69,99,95,94,95,93,90,78,78,78,75,60,55,57,58,58,57,64,76,77,77,77,77,77,77,78,80,77,77,73,73,89,93,85,82,82,80,76,64,59,67,91,98,127,129,116,68,65,69,0,0,21,67,0,0,
12,9,9,55,0,45,0,97,0,35,0,23,64,0,129,129,131,137,137,0,107,103,95,84,79,0,0,0,0,0,0,0,0,0,0,0,69,80,75,82,86,125,125,79,58,88,89,76,74,78,74,61,72,95,97,97,88,90,99,67,75,80,85,73,61,65,65,65,65,72,71,70,70,86,94,93,72,54,55,66,92,104,99,75,75,75,0,0,21,47,0,0,
12,9,10,0,0,0,0,49,0,0,0,0,64,0,131,131,77,129,141,0,91,101,76,87,90,0,0,0,0,0,0,0,0,0,0,0,66,71,71,71,71,71,71,67,86,110,108,114,105,105,90,89,86,84,79,79,79,71,66,73,85,86,83,84,87,87,85,81,81,75,69,72,77,73,64,62,56,63,89,98,99,95,104,104,104,104,0,0,21,95,0,0,
12,9,10,5,0,43,0,107,0,34,0,22,64,0,137,139,109,109,129,0,91,87,88,89,95,0,0,0,0,0,0,0,0,0,0,0,92,92,90,72,66,66,70,70,72,74,75,75,79,80,87,91,91,91,91,93,93,93,97,93,84,68,68,65,60,61,64,76,84,84,81,68,63,59,60,63,66,80,78,79,78,69,56,54,62,64,0,0,93,66,0,0,
12,9,10,10,0,30,0,51,0,24,0,16,64,0,131,137,143,131,131,24,89,84,89,87,92,0,0,0,0,0,0,0,0,0,0,0,78,88,87,78,76,76,76,76,73,66,65,54,53,53,53,60,62,62,62,62,62,62,60,63,65,65,63,63,73,76,78,80,80,80,82,91,92,92,97,76,55,61,62,70,74,73,72,72,63,60,0,0,93,105,0,0,
12,9,10,15,0,0,0,21,0,0,0,0,64,0,143,143,143,143,143,24,89,77,73,73,75,0,0,0,0,0,0,0,0,0,0,0,60,59,64,73,74,77,77,77,79,82,74,66,64,64,64,71,75,81,84,82,81,81,89,93,105,121,110,80,61,56,56,56,56,56,57,58,58,58,58,77,116,118,117,80,78,84,95,122,125,125,0,0,93,106,0,0,
12,9,10,20,0,32,0,30,0,26,0,17,64,0,143,137,141,141,137,0,75,73,75,75,80,0,0,0,0,0,0,0,0,0,0,0,118,113,98,98,105,110,110,111,100,82,80,72,68,66,67,69,79,84,83,83,76,90,92,82,82,82,82,77,77,77,77,79,83,83,85,98,98,92,92,92,79,78,92,97,97,91,87,82,82,82,0,0,93,131,0,0,
12,9,10,25,0,0,0,61,0,0,0,0,73,0,109,139,135,141,139,0,85,87,85,83,88,0,0,0,0,0,0,0,0,0,0,0,73,73,71,71,70,68,68,68,68,64,61,61,60,66,69,69,69,69,69,69,69,69,69,69,69,69,69,68,66,66,66,66,66,66,65,63,63,64,66,69,71,71,71,71,71,71,71,71,68,68,0,0,93,69,0,0,
12,9,10,30,0,0,0,47,0,0,0,0,64,0,139,143,143,73,109,0,88,89,91,93,90,0,0,0,0,0,0,0,0,0,0,0,66,60,60,60,60,60,58,58,58,66,78,79,83,91,93,71,58,66,76,96,101,101,93,88,88,79,61,65,79,82,82,81,80,80,73,64,64,64,61,66,68,68,66,60,64,64,64,77,82,82,0,0,93,105,0,0,
12,9,10,35,0,0,0,21,0,0,0,0,64,0,129,129,129,143,131,0,95,93,95,97,93,0,0,0,0,0,0,0,0,0,0,0,82,82,82,82,82,82,76,71,57,56,54,54,58,60,60,60,83,79,80,60,53,56,59,58,60,60,60,60,60,59,59,59,59,59,71,81,81,81,74,71,71,71,68,68,72,77,77,77,77,77,0,0,93,136,0,0,
12,9,10,40,0,23,0,53,0,18,0,11,54,0,139,107,129,143,131,0,88,94,94,87,89,0,0,0,0,0,0,0,0,0,0,0,77,81,85,85,81,69,57,67,101,77,67,67,67,67,67,68,69,63,64,64,64,66,80,80,80,80,80,80,80,80,83,90,87,79,69,70,84,85,81,80,80,80,80,80,80,80,80,77,72,68,0,0,0,75,0,0,
12,9,10,45,0,18,0,32,0,14,0,10,73,0,143,129,131,111,109,2,98,90,93,85,85,0,0,0,0,0,0,0,0,0,0,0,77,79,80,67,55,52,55,62,70,70,70,70,70,70,78,86,86,94,96,85,74,74,74,73,101,108,116,121,106,96,96,91,83,79,72,71,72,74,74,74,74,66,66,69,70,70,70,70,69,69,0,0,0,82,0,0,
12,9,10,50,0,0,0,7,0,0,0,0,64,0,109,109,141,73,109,0,83,87,84,83,85,0,0,0,0,0,0,0,0,0,0,0,70,70,70,70,70,70,69,70,72,68,68,67,67,70,70,69,67,66,69,72,68,67,67,68,70,66,66,65,65,61,65,69,69,72,71,70,70,70,68,69,72,72,73,76,76,74,70,68,64,63,0,0,0,147,0,0,
12,9,10,55,0,0,0,25,0,0,0,0,73,0,109,109,109,109,109,0,85,85,85,87,83,0,0,0,0,0,0,0,0,0,0,0,80,80,82,82,84,78,67,67,68,66,60,62,67,66,67,68,66,68,60,61,72,74,72,65,65,68,72,70,71,72,72,54,52,64,75,70,70,71,64,62,62,62,62,62,62,62,68,71,76,74,0,0,0,88,0,0,
12,9,11,0,0,0,0,0,0,0,0,0,64,0,109,109,111,73,105,0,84,83,83,83,81,0,0,0,0,0,0,0,0,0,0,0,67,66,79,85,80,69,68,70,75,70,72,75,72,67,60,67,65,80,88,88,89,90,79,82,82,82,75,68,69,69,66,75,77,76,75,73,74,73,69,71,72,71,72,72,72,71,71,71,71,72,0,0,0,150,0,0,
12,9,11,5,0,12,0,31,0,9,0,6,64,0,143,77,109,141,105,0,81,81,78,79,81,0,0,0,0,0,0,0,0,0,0,0,72,79,82,82,82,82,81,92,107,108,91,73,66,66,66,69,71,72,72,72,75,76,82,89,89,75,76,80,77,90,90,95,87,84,79,73,74,78,77,77,77,80,97,94,93,92,92,86,86,86,0,0,93,132,0,0,
12,9,11,10,0,0,0,78,0,0,0,0,62,0,139,129,135,139,137,0,93,91,88,79,72,0,0,0,0,0,0,0,0,0,0,0,86,86,80,63,63,63,68,69,69,69,69,67,71,75,76,74,80,86,86,82,78,64,44,44,52,69,99,87,75,79,79,94,88,88,88,86,83,78,78,78,78,95,106,107,107,107,92,82,81,80,0,0,93,62,0,0,
12,9,11,15,0,0,0,35,0,0,0,0,62,0,105,143,141,129,107,0,73,73,72,86,75,0,0,0,0,0,0,0,0,0,0,0,75,77,78,78,77,76,73,67,67,67,67,70,70,70,70,70,95,124,115,106,88,82,83,78,77,78,79,80,80,80,80,79,82,82,74,89,104,97,86,86,85,79,77,76,76,74,74,75,74,74,0,0,93,131,0,0,
12,9,11,20,0,0,0,29,0,0,0,0,62,0,109,139,139,137,105,0,77,77,82,83,80,0,0,0,0,0,0,0,0,0,0,0,74,73,72,73,73,75,75,75,75,77,91,135,148,148,130,104,93,93,93,93,93,93,93,93,93,93,89,73,74,74,74,87,88,87,80,69,76,78,78,77,76,76,76,76,75,74,74,70,70,70,0,0,93,113,0,0,
12,9,11,25,0,0,0,41,0,0,0,0,64,0,137,139,141,139,137,0,76,87,73,76,77,0,0,0,0,0,0,0,0,0,0,0,72,72,72,72,72,72,72,76,79,100,100,100,94,71,69,93,93,91,84,84,65,65,65,65,64,64,70,70,72,78,84,86,86,86,88,89,94,104,104,103,98,92,92,83,66,72,74,76,74,74,0,0,93,105,0,0,
12,9,11,30,0,0,0,32,0,0,0,0,54,0,143,109,137,77,143,0,83,75,77,78,88,0,0,0,0,0,0,0,0,0,0,0,89,91,69,61,61,61,59,59,59,59,60,64,73,80,87,99,100,85,83,71,71,71,66,65,70,76,74,72,74,79,85,87,88,87,85,85,87,85,80,80,80,79,81,77,68,65,68,69,73,77,0,0,93,129,0,0,
12,9,11,35,0,0,0,10,0,0,0,0,54,0,141,131,141,137,137,2,82,64,77,79,86,0,0,0,0,0,0,0,0,0,0,0,79,79,79,78,74,69,67,76,71,71,68,69,69,69,69,69,72,78,80,85,84,81,84,85,84,89,90,72,68,68,67,61,68,76,78,80,80,75,76,80,76,77,75,76,73,77,106,88,78,78,0,0,93,86,0,0,
12,9,11,40,0,0,0,79,0,0,0,0,53,0,137,141,141,137,137,0,88,81,91,87,89,0,0,0,0,0,0,0,0,0,0,0,78,78,71,69,75,79,69,65,65,65,65,68,71,72,72,74,74,74,73,73,89,92,97,98,99,102,86,64,61,68,68,68,67,62,60,60,61,61,61,62,62,63,61,60,57,54,54,64,76,76,0,0,0,81,0,0,
12,9,11,45,0,42,0,95,0,34,0,22,54,0,137,141,111,141,143,0,85,91,86,76,77,0,0,0,0,0,0,0,0,0,0,0,82,82,82,72,63,63,74,83,70,67,67,67,69,74,84,79,75,70,70,70,70,70,70,70,72,79,79,98,101,101,101,93,93,92,92,92,92,92,98,101,101,101,101,101,101,101,101,101,101,101,0,0,0,66,0,0,
12,9,11,50,0,0,0,94,0,0,0,0,64,0,109,139,131,139,143,0,85,83,82,75,72,0,0,0,0,0,0,0,0,0,0,0,109,78,77,75,73,68,68,68,67,70,72,74,74,71,70,73,74,79,106,106,107,107,91,85,85,85,85,85,66,50,50,56,58,58,58,58,66,83,100,100,100,100,147,154,135,114,93,80,73,70,0,0,21,146,0,0,
12,9,11,55,0,12,0,54,0,9,0,6,64,0,107,97,139,109,105,2,82,92,84,80,76,0,0,0,0,0,0,0,0,0,0,0,70,70,70,70,70,70,69,68,69,70,72,72,72,72,72,72,72,72,72,67,73,82,82,82,82,82,82,87,104,104,104,98,85,71,70,66,67,69,69,69,70,71,78,76,75,72,69,68,61,60,0,0,21,76,0,0,
12,9,12,0,0,0,0,15,0,0,0,0,64,0,109,129,139,141,139,0,78,93,92,83,77,0,0,0,0,0,0,0,0,0,0,0,63,68,72,73,73,73,74,75,75,76,77,69,67,63,61,62,59,62,64,69,61,60,60,60,60,60,52,56,60,69,68,66,82,82,78,81,79,79,79,78,76,73,71,76,79,86,75,70,76,76,0,0,21,135,0,0,
12,9,12,5,0,0,0,28,0,0,0,0,62,0,137,77,109,139,137,0,76,71,77,80,77,0,0,0,0,0,0,0,0,0,0,0,73,71,71,70,69,65,70,69,69,71,73,73,78,109,115,126,107,106,113,113,113,130,130,119,93,87,80,73,64,52,54,79,83,81,72,72,83,80,71,71,80,80,80,77,82,84,79,73,73,73,0,0,93,56,0,0,
12,9,12,10,0,0,0,0,0,0,0,0,73,0,137,141,141,141,137,0,77,80,90,85,82,0,0,0,0,0,0,0,0,0,0,0,72,87,89,115,117,95,70,60,61,69,70,71,71,69,68,68,69,70,70,68,68,66,65,67,60,60,60,60,71,75,76,83,90,89,89,89,89,88,82,64,61,61,61,61,61,65,70,70,65,65,0,0,93,150,0,0,
12,9,12,15,0,0,0,0,0,0,0,0,73,0,143,137,143,141,143,0,85,80,83,80,82,0,0,0,0,0,0,0,0,0,0,0,69,69,69,74,69,60,57,57,58,61,61,62,71,73,71,69,67,66,70,78,78,72,72,72,72,70,66,68,67,68,70,70,70,69,66,72,72,77,81,76,76,90,90,91,91,90,66,62,62,62,0,0,93,150,0,0,
12,9,12,20,0,0,0,0,0,0,0,0,73,0,143,137,137,107,141,0,83,84,80,80,84,0,0,0,0,0,0,0,0,0,0,0,63,64,65,66,66,60,60,61,65,70,73,77,72,69,68,82,92,92,92,84,72,67,71,77,77,77,76,77,79,73,85,93,94,98,84,68,69,71,63,67,72,71,66,66,66,66,66,87,92,86,0,0,93,150,0,0,
12,9,12,25,0,46,0,38,0,37,0,24,73,0,141,105,137,77,129,16,82,82,83,97,89,0,0,0,0,0,0,0,0,0,0,0,71,72,69,67,68,80,77,75,76,76,83,110,90,67,68,68,72,72,77,80,80,80,78,73,59,58,59,59,59,60,60,60,61,62,62,62,62,61,62,62,62,62,62,62,62,62,62,62,78,84,0,0,93,77,0,0,
12,9,12,30,0,0,0,25,0,0,0,0,64,0,129,143,105,105,141,17,91,86,89,91,88,0,0,0,0,0,0,0,0,0,0,0,85,86,87,87,87,87,85,80,81,98,97,81,59,60,75,80,80,80,80,80,83,86,86,86,86,86,93,85,80,82,99,97,96,93,92,92,76,53,54,61,63,68,69,81,84,84,84,83,85,92,0,0,93,134,0,0,
12,9,12,35,0,0,0,30,0,0,0,0,64,0,139,139,137,133,133,0,74,87,101,97,99,0,0,0,0,0,0,0,0,0,0,0,96,107,108,120,125,125,123,109,90,75,72,69,69,69,69,71,108,115,115,129,143,119,103,91,86,76,55,55,78,97,100,74,63,63,63,63,62,61,61,60,60,83,101,84,84,67,60,51,51,51,0,0,93,121,0,0,
12,9,12,40,0,13,0,38,0,10,0,6,64,0,143,79,129,131,143,4,100,104,96,99,98,0,0,0,0,0,0,0,0,0,0,0,52,52,52,52,55,57,57,57,57,58,58,58,58,59,61,64,64,62,60,60,60,58,59,59,61,61,62,63,63,63,63,70,70,68,68,70,70,70,72,73,78,78,72,72,72,72,70,66,66,66,0,0,21,89,0,0,
12,9,12,45,0,27,0,91,0,21,0,14,54,0,133,129,139,137,129,0,96,93,83,92,92,0,0,0,0,0,0,0,0,0,0,0,78,85,97,100,111,111,105,104,104,104,104,101,103,101,77,70,70,70,70,70,76,78,78,90,109,106,103,79,76,64,63,63,63,69,58,53,54,68,83,83,81,88,103,103,103,103,103,94,94,94,0,0,21,102,0,0,
12,9,12,50,0,22,0,50,0,18,0,12,73,0,79,139,139,141,141,2,94,81,84,87,85,0,0,0,0,0,0,0,0,0,0,0,102,102,95,90,99,108,108,108,108,107,87,81,81,81,85,87,85,77,77,77,75,70,70,70,70,70,68,67,67,68,67,64,64,62,62,100,116,119,84,73,68,68,69,70,70,70,71,70,69,69,0,0,21,127,0,0,
12,9,12,55,0,0,0,1,0,0,0,0,73,0,141,137,107,141,109,0,85,86,84,84,83,0,0,0,0,0,0,0,0,0,0,0,74,70,58,58,58,58,58,64,67,76,75,73,67,63,66,67,68,71,71,71,68,66,66,66,74,74,74,72,65,68,89,84,67,68,67,67,70,72,75,71,71,87,90,74,66,70,71,71,70,69,0,0,21,150,0,0,
12,9,13,0,0,24,0,48,0,19,0,13,74,0,143,105,105,109,77,0,94,79,74,75,78,0,0,0,0,0,0,0,0,0,0,0,73,83,89,88,88,90,90,90,78,70,76,72,76,71,74,72,71,71,73,74,79,80,81,83,78,77,77,79,80,80,81,79,75,70,81,83,83,79,80,82,78,78,78,75,70,79,79,78,82,82,0,0,0,118,0,0,
12,9,13,5,0,0,0,0,0,0,0,0,74,0,113,105,113,113,113,0,71,72,71,71,72,0,0,0,0,0,0,0,0,0,0,0,84,82,83,83,81,79,80,78,81,80,81,75,71,74,80,83,79,78,79,79,80,80,84,82,82,81,80,80,83,80,82,84,80,81,83,80,82,77,80,82,75,73,79,85,78,81,84,82,77,76,0,0,93,150,0,0,
12,9,13,10,0,0,0,0,0,0,0,0,74,0,49,113,113,113,113,0,71,71,70,72,70,0,0,0,0,0,0,0,0,0,0,0,82,79,74,80,78,81,78,83,87,89,84,84,81,80,83,77,80,80,77,79,75,72,81,82,86,87,82,88,88,82,81,85,81,77,80,75,76,81,81,79,84,78,81,81,85,83,83,87,76,81,0,0,93,150,0,0,
12,9,13,15,0,0,0,0,0,0,0,0,74,0,113,113,77,113,113,0,69,71,72,68,71,0,0,0,0,0,0,0,0,0,0,0,79,69,81,81,84,80,81,85,84,89,86,83,82,80,82,78,83,77,78,87,83,89,84,83,86,78,78,73,71,78,83,89,83,79,81,86,83,85,89,83,79,90,84,82,79,75,83,82,78,77,0,0,93,150,0,0,
12,9,13,20,0,0,0,0,0,0,0,0,74,0,113,17,113,49,113,0,69,73,69,70,71,0,0,0,0,0,0,0,0,0,0,0,85,86,84,81,80,85,76,71,84,86,83,83,79,80,77,78,79,76,80,83,83,81,84,84,81,92,89,84,85,81,87,91,82,75,83,83,78,76,83,82,82,77,82,84,85,76,74,86,87,87,0,0,93,150,0,0,
12,9,13,25,0,0,0,0,0,0,0,0,74,0,85,121,105,105,25,0,73,67,67,67,66,0,0,0,0,0,0,0,0,0,0,0,82,74,73,73,79,86,91,87,89,86,87,86,82,84,84,87,94,87,91,91,88,78,73,92,90,86,86,90,85,80,79,83,90,88,87,86,87,84,87,88,89,87,90,93,84,89,86,82,91,90,0,0,93,150,0,0,
12,9,13,30,0,0,0,0,0,0,0,0,74,0,85,25,113,113,121,0,69,67,67,66,67,0,0,0,0,0,0,0,0,0,0,0,86,89,82,78,78,84,86,83,84,88,85,90,84,88,86,92,86,86,87,88,89,89,85,83,82,86,87,87,85,89,94,96,91,85,86,75,82,93,88,86,89,86,87,89,86,87,88,83,89,87,0,0,93,150,0,0,
12,9,13,35,0,0,0,0,0,0,1,224,51,0,85,25,113,113,121,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,14,32,
12,9,13,40,0,0,0,1,0,0,0,0,72,0,24,24,120,107,113,0,68,68,67,72,65,0,0,0,0,0,0,0,0,0,0,0,85,86,87,84,86,87,91,86,87,84,84,85,86,86,83,89,85,88,86,84,85,91,88,88,88,84,87,85,88,84,87,84,86,84,85,85,80,80,79,80,92,87,79,87,95,92,91,90,93,90,0,0,5,41,0,0,
12,9,13,45,0,0,0,1,0,0,0,0,72,0,113,113,57,105,105,0,65,65,67,70,68,0,0,0,0,0,0,0,0,0,0,0,90,86,81,86,91,88,89,96,95,95,93,91,84,82,91,90,93,89,92,90,93,88,90,87,84,74,75,84,95,91,93,90,84,80,86,95,89,71,64,72,91,93,87,88,90,84,79,72,90,96,0,0,0,149,0,0,
12,9,13,50,0,0,0,5,0,0,0,0,74,0,57,105,23,17,113,0,65,70,71,66,66,0,0,0,0,0,0,0,0,0,0,0,92,88,90,94,90,95,87,85,86,85,82,90,91,95,89,85,78,84,87,86,67,57,59,71,93,94,95,92,91,87,84,79,92,92,85,82,85,91,91,92,90,89,85,88,88,83,80,90,99,98,0,0,0,91,0,0,
12,9,13,55,0,0,0,0,0,0,0,0,74,0,57,113,117,113,121,0,64,64,64,64,64,0,0,0,0,0,0,0,0,0,0,0,92,93,92,89,84,91,93,92,86,90,81,87,96,97,91,90,88,84,93,89,88,83,84,96,95,92,89,80,89,100,94,90,88,89,81,72,87,95,88,96,94,93,84,93,89,101,90,88,93,95,0,0,0,150,0,0,
12,9,14,0,0,0,0,19,0,0,0,0,63,0,117,121,113,105,77,0,68,63,65,74,78,0,0,0,0,0,0,0,0,0,0,0,97,104,90,64,68,91,99,99,89,95,89,86,91,97,86,75,85,100,96,90,83,82,98,89,91,82,82,98,98,94,89,85,92,90,86,82,82,87,79,67,65,73,72,76,77,81,80,77,73,75,0,0,0,98,0,0,
12,9,14,5,0,0,0,5,0,0,0,0,53,0,105,113,17,77,109,0,82,71,74,77,79,0,0,0,0,0,0,0,0,0,0,0,73,73,76,74,74,68,64,63,65,71,79,81,82,81,81,81,84,85,79,81,79,78,79,75,78,79,80,80,80,78,71,66,69,77,82,84,81,81,77,84,82,81,75,70,68,67,68,65,71,76,0,0,0,136,0,0,
12,9,14,10,0,0,0,5,0,0,0,0,53,0,113,113,111,105,109,0,73,74,71,65,66,0,0,0,0,0,0,0,0,0,0,0,85,82,81,80,77,75,79,80,80,76,77,71,74,79,79,80,85,81,82,82,89,83,85,84,104,110,92,91,84,79,87,91,87,87,91,90,85,82,85,89,92,89,87,84,90,89,88,87,91,95,0,0,0,77,0,0,
12,9,14,15,0,0,0,0,0,0,0,0,51,0,25,105,105,137,105,0,65,74,68,74,71,0,0,0,0,0,0,0,0,0,0,0,92,88,89,91,91,92,91,91,88,89,89,87,83,78,78,76,83,74,72,71,88,93,88,93,83,67,66,74,78,80,70,75,67,81,84,84,81,78,83,83,81,78,81,81,86,83,83,80,89,93,0,0,0,86,0,0,
12,9,14,20,0,0,0,0,0,0,0,0,51,0,113,109,105,137,113,0,72,70,69,70,64,0,0,0,0,0,0,0,0,0,0,0,73,76,80,83,86,83,86,84,82,81,80,82,84,89,87,86,80,77,95,79,82,84,78,82,84,87,84,85,87,85,85,90,81,87,82,82,81,79,78,83,90,93,88,89,89,90,95,92,91,91,0,0,0,107,0,0,
12,9,14,25,0,0,0,0,0,0,0,0,53,0,113,57,81,121,115,0,63,64,64,63,66,0,0,0,0,0,0,0,0,0,0,0,91,90,89,89,89,86,78,85,94,91,97,95,94,92,87,86,95,90,93,98,98,94,93,86,82,93,87,89,89,90,91,90,95,90,92,92,95,96,94,89,61,74,83,88,95,86,86,86,86,87,0,0,0,150,0,0,
12,9,14,30,0,98,0,185,0,77,0,50,54,0,137,129,135,137,137,0,73,90,92,86,84,0,0,0,0,0,0,0,0,0,0,0,72,69,69,73,77,77,77,76,85,88,91,92,83,91,80,66,66,73,81,93,94,89,87,58,41,43,56,57,57,60,73,72,73,79,79,79,83,85,87,83,74,74,74,74,74,74,74,79,76,83,0,0,0,48,0,0,
12,9,14,35,0,0,0,28,0,0,0,0,54,0,131,139,143,139,79,0,88,81,76,80,83,0,0,0,0,0,0,0,0,0,0,0,90,80,84,84,78,77,77,78,71,68,68,67,64,64,64,64,80,99,99,100,110,110,113,122,123,123,123,103,95,95,95,95,93,88,85,85,84,76,76,76,67,60,69,77,77,77,77,77,77,77,0,0,21,101,0,0,
12,9,14,40,0,0,0,26,0,0,0,0,74,0,143,105,141,141,79,2,80,86,75,80,84,0,0,0,0,0,0,0,0,0,0,0,70,71,77,77,77,80,89,88,88,88,88,77,73,65,65,65,65,65,65,65,63,63,66,73,72,72,73,68,71,80,80,80,80,78,64,56,59,60,60,62,65,66,67,67,67,67,70,70,70,70,0,0,21,141,0,0,
12,9,14,45,0,0,0,0,0,0,0,0,74,0,111,111,111,107,109,0,80,81,80,71,73,0,0,0,0,0,0,0,0,0,0,0,71,72,63,64,64,64,64,64,74,84,83,86,87,74,71,70,68,68,68,68,68,70,81,82,75,68,68,68,68,68,70,80,80,80,79,80,80,80,80,80,80,76,77,77,81,84,84,89,89,89,0,0,21,150,0,0,
12,9,14,50,0,0,0,20,0,0,0,0,74,0,143,139,139,105,107,0,73,76,75,76,75,0,0,0,0,0,0,0,0,0,0,0,89,86,81,75,74,76,76,73,72,72,75,76,76,76,75,74,74,73,73,73,73,94,94,98,99,106,106,106,106,107,97,81,81,90,106,122,126,97,87,58,58,58,62,63,63,63,63,63,66,67,0,0,21,85,0,0,
12,9,14,55,0,0,0,17,0,0,0,0,64,0,141,77,143,109,109,0,79,79,76,70,74,0,0,0,0,0,0,0,0,0,0,0,54,54,55,57,63,0,67,67,67,72,68,80,97,89,81,76,69,68,68,68,66,64,64,62,60,61,73,87,77,77,77,81,85,85,85,79,79,87,87,87,87,93,91,90,88,86,88,88,88,88,0,0,21,130,0,0,
12,9,15,0,0,0,0,23,0,0,0,0,74,0,131,141,141,141,141,0,78,75,78,77,68,0,0,0,0,0,0,0,0,0,0,0,82,59,55,51,51,51,51,53,89,92,93,82,71,71,66,61,62,68,77,82,86,87,84,85,72,69,84,99,100,99,103,107,107,107,106,102,98,90,98,88,85,88,89,89,89,99,100,99,101,104,0,0,21,84,0,0,
12,9,15,5,0,14,0,85,0,11,0,7,54,0,0,0,0,0,0,0,0,0,0,0,68,0,0,0,0,0,0,0,0,0,0,0,98,95,95,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,95,0,0,0,108,0,0,
12,9,15,10,0,10,0,34,0,8,0,6,64,1,109,141,137,139,139,0,80,80,80,81,77,0,0,0,0,0,0,0,0,0,0,0,92,90,82,82,82,74,71,68,71,76,84,82,80,81,82,74,69,70,70,70,70,72,74,83,109,120,120,118,114,107,90,74,73,73,73,71,80,80,80,80,80,83,84,83,75,70,73,73,77,80,0,0,5,89,0,0,
12,9,15,15,0,27,0,69,0,21,0,14,73,0,137,139,139,139,139,10,87,72,69,71,77,0,0,0,0,0,0,0,0,0,0,0,69,64,63,87,84,110,93,92,78,78,78,76,77,87,87,87,87,91,93,88,77,73,73,73,103,126,139,157,155,149,149,149,146,91,77,91,91,91,91,91,92,84,88,123,123,123,122,119,82,81,0,0,0,84,0,0,
12,9,15,20,0,74,0,225,0,66,0,42,73,0,143,139,129,129,129,0,70,75,90,103,100,0,0,0,0,0,0,0,0,0,0,0,82,84,82,110,123,123,100,86,83,82,82,82,82,82,73,60,56,63,65,65,65,64,64,74,76,70,64,69,74,84,75,89,121,125,106,67,70,88,91,94,95,99,112,80,84,83,76,74,59,58,0,0,0,60,0,0,
12,9,15,25,0,0,0,46,0,0,0,0,64,0,129,139,129,141,129,0,96,86,90,86,94,0,0,0,0,0,0,0,0,0,0,0,105,122,94,55,66,70,58,51,48,48,50,55,73,78,88,83,84,84,88,130,129,122,98,98,97,98,88,74,71,61,63,64,63,58,64,76,84,84,87,72,59,53,67,71,74,72,78,93,95,95,0,0,21,91,0,0,
12,9,15,30,0,0,0,9,0,0,1,161,64,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,21,39,14,25,
12,9,15,35,0,0,0,46,0,0,0,0,64,0,141,141,133,133,141,0,78,90,98,97,88,0,0,0,0,0,0,0,0,0,0,0,75,69,69,69,69,82,88,86,83,90,83,79,87,89,89,88,79,82,79,76,75,71,71,72,74,74,74,74,76,76,76,76,75,81,94,74,62,62,62,62,64,70,77,80,80,81,74,74,74,74,0,0,5,79,0,0,
12,9,15,40,0,0,0,74,0,0,0,0,54,0,101,101,129,109,129,0,102,96,101,98,95,0,0,0,0,0,0,0,0,0,0,0,67,53,58,64,67,65,64,58,58,58,62,72,81,77,77,74,72,75,74,70,65,62,62,62,64,69,70,70,73,73,73,73,68,64,64,73,69,69,69,69,72,72,73,75,99,101,73,85,92,88,0,0,21,111,0,0,
12,9,15,45,0,0,0,77,0,0,0,0,64,0,131,133,129,137,137,0,95,89,103,87,71,0,0,0,0,0,0,0,0,0,0,0,82,83,88,90,90,92,102,91,92,102,93,80,75,74,76,78,78,77,77,67,65,68,68,66,66,66,71,86,86,76,57,71,76,81,81,81,81,79,76,72,77,80,83,83,83,85,85,84,78,68,0,0,21,100,0,0,
12,9,15,50,0,0,0,95,0,0,0,0,54,0,141,105,129,129,137,0,83,79,92,92,88,0,0,0,0,0,0,0,0,0,0,0,64,65,96,102,107,90,70,68,68,68,68,68,83,91,91,92,92,88,94,94,94,85,85,88,85,83,106,103,70,65,68,70,62,66,76,76,76,77,83,84,84,82,77,84,75,83,90,83,62,62,0,0,21,88,0,0,
12,9,15,55,0,0,0,74,0,0,0,0,64,0,105,107,107,129,0,0,73,78,91,101,87,0,0,0,0,0,0,0,0,0,0,0,62,62,77,79,83,84,84,91,70,73,76,76,76,74,68,68,69,109,99,89,60,66,78,95,64,76,95,103,73,64,60,63,59,57,73,77,56,64,78,78,105,94,0,0,0,0,0,0,0,74,0,0,0,114,0,0,


};
#endif //SLEEP_DEBUG_1_2 
#endif //SLEEP_DEBUG_1

void Sleep_TestWriteDailyData(void)
{
#if SLEEP_DEBUG_1 
    uint8_t             day_id = 0;
    flash_find_index_t  *p_find_index = &p_flash_param->flash_index_s;
    day_id = (p_find_index->block_index+2)%3;

#if SLEEP_DEBUG_1_1     
    Flash_CommonErase((uint32_t*)RECORD_DAILY_DATA_ADDR(day_id, 0), RECORD_DAILY_ONE_DAY_SIZE/4096);
    Flash_CommonErase((uint32_t*)RECORD_SLEEP_DATA_ADDR(day_id, 0), RECORD_SLEEP_DATA_SIZE/4096); 
    Flash_CommonStore((uint32_t*)RECORD_DAILY_DATA_ADDR(day_id, 0), (uint32_t *)daily_data_buf0, 288*92/4);
    
#endif
    
#if SLEEP_DEBUG_1_2 
    day_id = (p_find_index->block_index)%3;    
    Flash_CommonErase((uint32_t*)RECORD_DAILY_DATA_ADDR(day_id, 0), RECORD_DAILY_ONE_DAY_SIZE/4096);
    Flash_CommonErase((uint32_t*)RECORD_SLEEP_DATA_ADDR(day_id, 0), RECORD_SLEEP_DATA_SIZE/4096); 
    Flash_CommonStore((uint32_t*)RECORD_DAILY_DATA_ADDR(day_id, 0), (uint32_t *)daily_data_buf1, 192*92/4);
#endif   
    
#endif //SLEEP_DEBUG_1
}

#if SLEEP_DEBUG_2

const uint8_t sleep_info_data[900] = 
{
161,132,3,162,8,35,182,21
,241,138,170,153,68,163,36,1
,5,1,0,1,5,7,40,3
,0,0,0,0,0,0,5,0
,0,0,9,1,135,0,0,0
,144,1,0,0,0,0,0,0
,60,0,0,164,44,0,0,0
,0,0,0,0,0,0,0,0
,0,0,0,0,0,0,0,0
,0,0,0,0,0,0,0,0
,0,0,0,0,0,0,0,0
,0,0,0,0,0,0,0,0
,0,165,32,3,32,0,32,0
,32,0,32,0,32,0,32,0
,32,0,32,0,32,0,32,0
,32,0,32,0,32,0,32,0
,32,0,32,0,32,0,32,0
,32,0,32,0,32,0,32,0
,32,0,32,0,32,0,32,0
,32,0,32,0,32,0,32,0
,32,0,32,0,32,0,32,0
,32,0,32,0,32,0,32,0
,32,0,32,0,0,0,0,0
,0,0,0,0,0,0,0,0
,0,0,0,0,0,0,0,0
,0,0,0,0,0,0,0,0
,0,0,0,0,0,0,0,0
,0,0,0,0,0,0,0,0
,0,0,0,0,0,0,32,0
,32,0,32,0,32,0,32,0
,32,0,32,0,32,0,32,0
,32,0,32,0,32,0,32,0
,32,0,32,0,32,0,32,0
,32,0,32,0,32,0,0,0
,0,0,0,0,0,0,0,0
,0,0,0,0,0,0,0,0
,0,0,0,0,0,0,0,0
,0,0,0,0,0,0,0,0
,0,0,0,0,0,0,0,0
,0,0,0,0,0,0,0,0
,0,0,0,0,0,0,0,0
,0,0,32,0,32,0,32,0
,32,0,32,0,0,0,0,0
,0,0,0,0,0,0,32,0
,32,0,32,0,32,0,32,0
,0,0,0,0,0,0,0,0
,0,0,32,0,32,0,32,0
,32,0,32,0,32,0,32,0
,32,0,32,0,32,0,32,0
,32,0,32,0,32,0,32,0
,0,0,0,0,0,0,0,0
,0,0,0,0,0,0,0,0
,0,0,0,0,0,0,0,0
,0,0,0,0,0,0,0,0
,0,0,0,0,0,0,0,0
,0,0,0,0,0,0,0,0
,0,0,0,0,0,0,0,0
,0,0,0,0,0,0,0,0
,0,0,0,0,0,0,0,0
,0,0,0,0,0,0,0,0
,0,0,0,0,0,0,0,0
,0,0,0,0,0,0,0,0
,0,0,0,0,0,0,0,0
,0,0,0,0,0,0,32,0
,32,0,32,0,32,0,32,0
,0,0,0,0,0,0,0,0
,0,0,0,0,0,0,0,0
,0,0,0,0,0,0,0,0
,0,0,0,0,0,0,0,0
,0,0,0,0,0,0,0,0
,0,0,0,0,0,0,0,0
,0,0,0,0,0,0,0,0
,0,0,0,0,0,0,0,0
,0,0,0,0,0,0,32,0
,32,0,32,0,32,0,32,0
,0,0,0,0,0,0,0,0
,0,0,0,0,0,0,0,0
,0,0,0,0,0,0,0,0
,0,0,0,0,0,0,0,0
,0,0,0,0,0,0,0,0
,0,0,0,0,0,0,0,0
,0,0,32,0,32,0,32,0
,32,0,32,0,0,0,0,0
,0,0,0,0,0,0,0,0
,0,0,0,0,0,0,0,0
,0,0,0,0,0,0,0,0
,0,0,0,0,0,0,0,0
,0,0,0,0,32,0,32,0
,32,0,32,0,32,0,0,0
,0,0,0,0,0,0,0,0
,0,0,0,0,0,0,0,0
,0,0,32,0,32,0,32,0
,32,0,32,0,0,0,0,0
,0,0,0,0,0,0,0,0
,0,0,0,0,0,0,0,0
,32,0,32,0,32,0,32,0
,32,0,0,0,0,0,0,0
,0,0,0,0,0,0,0,0
,0,0,0,0,0,0,0,0
,0,0,0,0,0,0,0,0
,32,0,32,0,32,0,32,0
,32,0,32,0,32,0,32,0
,32,0,32,0,0,0,0,0
,0,0,0,0,0,0,32,0
,32,0,32,0,32,0,32,0
,0,0,0,0,0,0,0,0
,0,0,32,0,32,0,32,0
,32,0,32,0,0,0,0,0
,0,0,0,0,0,0,0,0
,0,0,0,0,0,0,0,0
,0,0,0,0,0,0,0,0
,0,0,0,0,0,0,0,0
,0,0,0,0,

};

#endif

#include "ext_flash.h"
void Sleep_TestWriteSleepInfoData(void)
{
    
#if SLEEP_DEBUG_2
    
    uint16_t start_sector = 0;
    uint16_t block_num = 0;
    
    uint8_t             day_id = 0;
    flash_find_index_t  *p_find_index = &p_flash_param->flash_index_s;
    day_id = (p_find_index->block_index+2)%3;
    //睡眠数据一起擦除
    //Flash_CommonErase((uint32_t*)RECORD_SLEEP_DATA_ADDR(day_id, 0), RECORD_SLEEP_DATA_SIZE/4096);     
    //Flash_CommonStore((uint32_t*)RECORD_SLEEP_DATA_ADDR(day_id, 0), (uint32_t *)sleep_info_data, 1600/4);
    
    start_sector = RECORD_SLEEP_DATA_ADDR(day_id, 0)/4096;
    block_num = RECORD_SLEEP_DATA_SIZE/4096;
    for(uint8_t i = 0; i < block_num; i++)
    {
        Extflash_DataSectorErase4K(start_sector + i);
    }
    
    Api_SaveSleepResultData(day_id, (uint8_t *)sleep_info_data, 900);
    
#endif    
}


#endif
