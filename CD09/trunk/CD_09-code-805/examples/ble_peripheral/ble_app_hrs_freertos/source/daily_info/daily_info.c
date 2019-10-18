/***************************Copyright(c)***********************************
**                       Veepoo Tech
**                  COPYRIGHT (C) 2016-2019
**-----------File Info-----------------------------------------------------
** File        name:     daily_info.c
** Last modified Date:   2017-08-01
** Last SYS_VERSION:     0.0.0
** Descriptions:            
**-------------------------------------------------------------------------
** Created  by:          WJ
** Created  date:        2017-08-01
** SYS_VERSION:          0.0.0
** Descriptions:         
**-------------------------------------------------------------------------
** Modified by:				
** Modified date:  			          
** SYS_VERSION:                 
** Descriptions:                        
***************************************************************************/
#include <string.h>
#include "app_type.h"
#include "app_flash.h"
#include "daily_info.h"
#include "fstorage.h"
#include "nowear_detection.h"
#include "vph_wear_dect.h"
#include "daily_test.h"
#include "posture_get.h"

/*********************************************************************
* Macro Definition
*/
#define DAILY_DATA_PER_SIZE     20
#define DAILY_BOTTOM_TIME       5

/*********************************************************************
* Global Variables
*/
static daily_pkg_info_t cur_pkg_info;                   //当前数据包所在位置
static daily_data_format_t recent_daily_data;           //5分钟保存数据结构体
static daily_last_info_t daily_last_info_s = {0x00, };
sport_history_t sc_sport_info = {0x00, };
static uint8_t sleep_code = 0;

extern system_flag_t system_flag_s;
extern system_info_t system_info_s;
extern system_switch_t system_switch_s;
extern uint32_t reset_value;
extern uint32_t reset_time_minutes; 
extern uint32_t reset_time_seconds; 

/****************************************************************
** Function name:           Daily_OperationFlashCallback
** Descriptions:            保存日常数据返回函数
** input parameters:
** output parameters:
** Returned value:
** Created by:              WJ             
** Created Date:            2016-12-07
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Daily_OperationFlashCallback(uint8_t result)
{
    if(result == FS_SUCCESS)
    {
        system_flag_s.daily_save_state = 0x01;
    }
    else
    {
        system_flag_s.daily_save_state = 0x00;
    }
}

/****************************************************************
** Function name:           Daily_SetIntervalSteps5m
** Descriptions:            设置最后5分钟的记步
** input parameters:
** output parameters:
** Returned value:
** Created by:              WJ             
** Created Date:            2016-12-07
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Daily_SetIntervalSteps5m(uint32_t setp_num )
{
    daily_last_info_s.last_setp_num = setp_num;
}

/****************************************************************
** Function name:           Daily_GetIntervalSteps5m
** Descriptions:            获取最后5分钟的记步
** input parameters:
** output parameters:
** Returned value:
** Created by:              WJ             
** Created Date:            2016-12-07
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
uint32_t Daily_GetIntervalSteps5m(void)
{
    return daily_last_info_s.last_setp_num_5m;
}

/****************************************************************
** Function name:           Daily_GetIntervalSteps10m
** Descriptions:            获取最后10分钟的记步
** input parameters:
** output parameters:
** Returned value:
** Created by:              WJ             
** Created Date:            2016-12-07
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
uint32_t Daily_GetIntervalSteps10m(void)
{
    return daily_last_info_s.last_setp_num_10m;
}

/****************************************************************
** Function name:           Daily_GetLastMomentum
** Descriptions:            获取最后五分钟的运动量
** input parameters:
** output parameters:
** Returned value:
** Created by:              WJ             
** Created Date:            2016-12-07
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
uint32_t Daily_GetLastMomentum(void)
{
    return daily_last_info_s.last_sport_momentum ;
}

/****************************************************************
** Function name:           Daily_SetDailyPkgInfo
** Descriptions:            设置块号和记录数
** input parameters:        block：块号；pkg_cnt：记录数
** output parameters:
** Returned value:
** Created by:              WJ             
** Created Date:            2016-12-07
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Daily_SetDailyPkgInfo(uint8_t block, uint16_t pkg_cnt)
{
    cur_pkg_info.block = block;
    cur_pkg_info.pkg_cnt = pkg_cnt;
}

/****************************************************************
** Function name:           Daily_OperationFlashCallback
** Descriptions:            获取指定天的块号信息
** input parameters:        previous_num:前几天
** output parameters:
** Returned value:
** Created by:              WJ             
** Created Date:            2016-12-07
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
uint8_t Daily_GetPreviousBlock(uint8_t previous_num)
{
    uint8_t i = 0;
    uint8_t previous_block = 0;

    previous_block = cur_pkg_info.block ;
    for(i = 0; i < previous_num; i++ )
    {
        if(previous_block == 0)     //if reach the end of the block section, jump to the first block
        {
            previous_block = DAILY_DATA_BLOCK_MAX - 1;
        }
        else
        {
            previous_block = previous_block - 1;
        }
    }

    return previous_block;
}

/****************************************************************
** Function name:           Daily_Init
** Descriptions:            初始化
** input parameters:
** output parameters:
** Returned value:
** Created by:              SQH              
** Created Date:            2016-11-24
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Daily_Init(void)
{
    memset(&cur_pkg_info, 0, sizeof(daily_pkg_info_t));
    memset(&recent_daily_data, 0, sizeof(daily_data_format_t));

    memset(&daily_last_info_s, 0, sizeof(daily_last_info_t));  
}

/****************************************************************
** Function name:           Daily_Clear
** Descriptions:            清除五分钟数据记录变量
** input parameters:
** output parameters:
** Returned value:
** Created by:              SQH              
** Created Date:            2016-11-14
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Daily_Clear(void)
{
    memset(&recent_daily_data, 0, sizeof(daily_data_format_t));     //清除已经保存了的日常数据
}

/****************************************************************
** Function name:           Sleep_GetDailyData
** Descriptions:            设置五分钟数据计步、运动量、时间等
** input parameters:
** output parameters:
** Returned value:
** Created by:              SQH              
** Created Date:            2016-11-14
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Daily_GetDailyData(void)
{
    static uint8_t reste_flag = 0;
    uint16_t temp = 0;
    sport_information_info_t gsii_sport_info_s = {0x00,};
    
#if DAILY_TEST_SLEEP    
    Daily_Clear();
    DailyTest_setData((void *)&recent_daily_data);
    
    return;
#else  
    uint8_t time_buff[7] = {0,};
    uint32_t s_momentum_temp = 0;
    uint32_t s_step_temp = 0;
    
    Get_SportInformationInfo(&gsii_sport_info_s);
    
    Daily_Clear();    
    /*------ get time ------*/
    Sclock_ReadTimeToBuf(time_buff, 7);
    recent_daily_data.month = time_buff[2];
    recent_daily_data.day = time_buff[3];
    recent_daily_data.hour = time_buff[4];
    recent_daily_data.minute = time_buff[5];
 
    //计步、运动量  ->      可直接计算出结果
    s_momentum_temp = Work_MeasureGet();    
//    Work_MeasrueCln();
    if(daily_last_info_s.last_sport_momentum > s_momentum_temp)
    {
        s_momentum_temp = 0;
    }
    else
    {
        s_momentum_temp = s_momentum_temp - daily_last_info_s.last_sport_momentum;    
    }
    daily_last_info_s.last_sport_momentum = Work_MeasureGet(); 
    s_momentum_temp = s_momentum_temp >> 12;
    recent_daily_data.movement_h = (s_momentum_temp >> 8) & 0xFF;
    recent_daily_data.movement_l = (s_momentum_temp >> 0) & 0xFF;

    s_step_temp = gsii_sport_info_s.posi_steps;//All_StepGet();
    daily_last_info_s.last_setp_num_10m = daily_last_info_s.last_setp_num_5m + (s_step_temp - daily_last_info_s.last_setp_num);   //WJ.20170517.修复心率提醒条件
    if(daily_last_info_s.last_setp_num_5m > s_step_temp)
    {
        daily_last_info_s.last_setp_num_5m = 0;
    }
    else
    {
        daily_last_info_s.last_setp_num_5m = s_step_temp - daily_last_info_s.last_setp_num;
    }
    daily_last_info_s.last_setp_num = s_step_temp;
    recent_daily_data.steps_h = (daily_last_info_s.last_setp_num_5m >> 8) & 0xFF;
    recent_daily_data.steps_l = (daily_last_info_s.last_setp_num_5m >> 0) & 0xFF;
    
    //心率获取、血压获取 ->  需要等待采集完成再获取结果
         
    sc_sport_info = Vnwd_HistoryMomentum(s_momentum_temp, Daily_GetIntervalSteps5m());		     

    //获取姿态以及次数，获取后初始化姿态统计
    Posture_StatisticalGet(&recent_daily_data.sleep_posture, &recent_daily_data.sleep_posture_cnt);
    Posture_StatisticalInit();
    
    recent_daily_data.sleep_code = sleep_code;
    if(!reste_flag)
    {
        reste_flag = 1;
        recent_daily_data.reserve6 = ((reset_value >> 12) & 0xF0) + (reset_value & 0x0F);
        recent_daily_data.reserve7 = reset_time_minutes;
    } 

    if(gsii_sport_info_s.posi_cal < daily_last_info_s.last_calories)
    {
        temp = 0;        
        daily_last_info_s.last_calories = gsii_sport_info_s.posi_cal;
    }
    else
    {       
        temp = gsii_sport_info_s.posi_cal - daily_last_info_s.last_calories;
        temp = temp/100;
        daily_last_info_s.last_calories += temp * 100;
    }
    
    recent_daily_data.calories_h = (temp >> 8) & 0xFF;
    recent_daily_data.calories_l = (temp     ) & 0xFF;
    
    if(gsii_sport_info_s.posi_distance < daily_last_info_s.last_distance)
    {
        temp = 0;
    }
    else
    {
        temp = gsii_sport_info_s.posi_distance - daily_last_info_s.last_distance;
    }
    daily_last_info_s.last_distance = gsii_sport_info_s.posi_distance;
    recent_daily_data.distance_h = (temp >> 8) & 0xFF;
    recent_daily_data.distance_l = (temp     ) & 0xFF;
   
#endif         
}

/****************************************************************
** Function name:           Daily_SetDailyData
** Descriptions:            设置日常数据中的测量结果
** input parameters:
** output parameters:
** Returned value:
** Created by:              WJ             
** Created Date:            2016-12-07
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Daily_SetDailyData(uint8_t oxgen, uint8_t heart, uint8_t bp_dp, uint8_t bp_sp)
{
    uint8_t wear_flag = 0;
    uint8_t heart_flag = 0;
    recent_daily_data.bp_h = bp_sp;
    recent_daily_data.bp_l = bp_dp;

    wear_flag = Wear_GetWearFlag();
    if(system_info_s.wear_detect != 1 || system_switch_s.led_gear != 1
      || (system_switch_s.blood_pressure != 1 && system_switch_s.heart != 1))
    {
        wear_flag = 0;
    }
    
    if(heart > 30)
    {
        recent_daily_data.heart = heart;     
        Wear_ClearWearFlag();     //WJ.20170714.修改清除佩戴标志的位置
        wear_flag = 0;
        heart_flag = 1;
    }
    else if(bp_sp > 30)
    {
        Wear_ClearWearFlag();     //WJ.20170824.修改清除佩戴标志的位置
        wear_flag = 0;
        recent_daily_data.heart = 0;
        heart_flag = 1;
    }

    //WJ.20171221.算法要求修改为代入脱手算法的姿态为统计姿态
    Vnwd_NoWearDect(sc_sport_info, wear_flag, recent_daily_data.sleep_posture, 8);//Gesture_GetMode16bit(), 8);
    wear_flag = Get_NoWearStation();
    if(wear_flag == 6 || heart == 6)
    {
        wear_flag = 6;  //WJ.20170829.临时添加，后续跟随睡眠算法修改
        heart_flag = 0;
    }
    if(heart_flag == 0)
    {
        recent_daily_data.heart = wear_flag;      
    }        
}

/****************************************************************
** Function name:           Daily_SaveDailyData
** Descriptions:            保存五分钟数据
** input parameters:
** output parameters:
** Returned value:
** Created by:              SQH              
** Created Date:            2016-11-14
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Daily_SaveDailyData(void)
{
    if(cur_pkg_info.pkg_cnt < DAILY_PKG_MAX_NUM - 2)
    {
        App_FlashWriteDailyData(cur_pkg_info.block, cur_pkg_info.pkg_cnt++, 
                               (uint8_t *)&recent_daily_data, sizeof(daily_data_format_t),
                               Daily_OperationFlashCallback);
    }    
    Daily_Clear();      //数据存储到flash后，将中间状态数据清0         
}

// 功耗数据统计结果存储
extern power_statistics_t power_statistics_s;
void Daily_SaveDailyPowerData(void)
{
    uint8_t save_buf[20] = {0};
    
    memcpy(save_buf, &power_statistics_s, sizeof(power_statistics_s));

    App_FlashEraseCheck();
    
    App_FlashWriteDailyData(cur_pkg_info.block, DAILY_PKG_MAX_NUM - 2, 
                           (uint8_t *)save_buf, 20,
                            NULL);
    App_FlashEraseCheck();
    
    memset(&power_statistics_s, 0, sizeof(power_statistics_s));
    
}


/****************************************************************
** Function name:           Daily_SeHeadtUsing
** Descriptions:            设置当前块号为正在使用
** input parameters:
** output parameters:
** Returned value:
** Created by:              WJ             
** Created Date:            2016-12-07
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Daily_SeHeadtUsing(void)
{
    daily_block_info_t daily_block_info_c;
    
    memset(&daily_block_info_c, 0xFF, sizeof(daily_block_info_t));
    daily_block_info_c.block_flag = 0xAA;
    daily_block_info_c.month = clock->month;
    daily_block_info_c.day = clock->day;
    App_FlashWriteDailyHead(cur_pkg_info.block, (void *)&daily_block_info_c, sizeof(daily_block_info_t));    
}

/****************************************************************
** Function name:           Daily_NextDay
** Descriptions:            跨天处理换块存储
** input parameters:
** output parameters:
** Returned value:
** Created by:              WJ             
** Created Date:            2016-12-07
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Daily_NextDay(void)
{
    daily_block_info_t daily_block_info;
    sport_information_info_t gsii_sport_info_s = {0x00,};
    uint32_t all_step = 0;
    
#if DAILY_TEST_SLEEP && !(DAILY_TEST_SLEEP_WRITE_DATA)
    
#else    
    
    Get_SportInformationInfo(&gsii_sport_info_s);
    all_step = gsii_sport_info_s.posi_steps;//All_StepGet();
    
    memset(&daily_block_info, 0xFF, sizeof(daily_block_info_t));
    
    daily_block_info.block_flag = 0x00;
    daily_block_info.all_step[0] = (all_step >> 16) & 0xFF;  
    daily_block_info.all_step[1] = (all_step >>  8) & 0xFF; 
    daily_block_info.all_step[2] = (all_step >>  0) & 0xFF; 
    daily_block_info.pack_num = cur_pkg_info.pkg_cnt;
    
    daily_block_info.all_calories = gsii_sport_info_s.posi_cal;
    daily_block_info.all_distance = gsii_sport_info_s.posi_distance;    
    
    App_FlashWriteDailyHead(cur_pkg_info.block, (void *)&daily_block_info, sizeof(daily_block_info_t));
    
    // 存储功耗统计
    Daily_SaveDailyPowerData();
    
    
#endif  
    
    cur_pkg_info.block++;
    cur_pkg_info.pkg_cnt = 0;
    if(cur_pkg_info.block >= DAILY_DATA_BLOCK_MAX)     //if reach the end of the block section, jump to the first block
    {
        cur_pkg_info.block = DAILY_DATA_BLOCK_0;
    }
    
#if DAILY_TEST_SLEEP && !(DAILY_TEST_SLEEP_WRITE_DATA)

#else    
    App_FlashEraseDailyDataPage(cur_pkg_info.block);    
    
    nrf_delay_ms(100); 
    Daily_SeHeadtUsing();   
    nrf_delay_ms(100); 
#endif   
    
    memset(&daily_last_info_s, 0, sizeof(daily_last_info_t));   
    Ped_SetSportsInfo(0, 0, 0);
    // 计步重置
    All_StepCln();
    Work_MeasrueCln();
}

/****************************************************************
** Function name:           Daily_DailyHeadSetUsed
** Descriptions:            设置当前块为完成使用保存信息，换块
** input parameters:
** output parameters:
** Returned value:
** Created by:              WJ             
** Created Date:            2016-12-07
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Daily_DailyHeadSetUsed(const uint8_t month, const uint8_t day)
{
    daily_block_info_t daily_block_info;

    uint32_t all_step = 0;
    
    all_step = All_StepGet();

    memset(&daily_block_info, 0xFF, sizeof(daily_block_info_t));
//    daily_block_info.month = month;
//    daily_block_info.day = day;
    daily_block_info.block_flag = 0x00;
    daily_block_info.all_step[0] = (all_step >> 16) & 0xFF;  
    daily_block_info.all_step[1] = (all_step >>  8) & 0xFF; 
    daily_block_info.all_step[2] = (all_step >>  0) & 0xFF; 
    daily_block_info.pack_num = cur_pkg_info.pkg_cnt;
    
    App_FlashWriteDailyHead(cur_pkg_info.block, (void *)&daily_block_info, sizeof(daily_block_info_t));
    
    
    cur_pkg_info.block++;
    cur_pkg_info.pkg_cnt = 0;
    if(cur_pkg_info.block >= DAILY_DATA_BLOCK_MAX)     //if reach the end of the block section, jump to the first block
    {
        cur_pkg_info.block = DAILY_DATA_BLOCK_0;
    }
    App_FlashEraseDailyDataPage(cur_pkg_info.block);    
 
    nrf_delay_ms(100);    
    Daily_SeHeadtUsing();
    
    memset(&daily_last_info_s, 0, sizeof(daily_last_info_t)); 
    Ped_SetSportsInfo(0, 0, 0);
    Work_MeasrueCln();
}

/****************************************************************
** Function name:           Daily_ClearDailyByBlcak
** Descriptions:            清除指定块号的信息并作为当天存储块
** input parameters:
** output parameters:
** Returned value:
** Created by:              WJ             
** Created Date:            2016-12-07
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Daily_ClearDailyByBlcak(uint8_t black)
{
    Daily_SetDailyPkgInfo(black, 0);
    All_StepSet(0);
    App_FlashEraseDailyDataPage(black); 
    nrf_delay_ms(100);  
    Daily_SeHeadtUsing();   
    nrf_delay_ms(100);       
}

/****************************************************************
** Function name:           Daily_ClearDailyCurrentBlcak
** Descriptions:            清除当前块的数据
** input parameters:
** output parameters:
** Returned value:
** Created by:              WJ             
** Created Date:            2016-12-12
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Daily_ClearDailyCurrentBlcak(void)
{
    Daily_SetDailyPkgInfo(cur_pkg_info.block, 0);
    All_StepSet(0);
    App_FlashEraseDailyDataPage(cur_pkg_info.block); 
    nrf_delay_ms(100);  
    Daily_SeHeadtUsing();    
    nrf_delay_ms(100); 
    
    //重置数据
    memset(&daily_last_info_s, 0, sizeof(daily_last_info_t)); 
    Ped_SetSportsInfo(0, 0, 0);
    Work_MeasrueCln(); 
    Daily_Clear();      //数据存储到flash后，将中间状态数据清0    
}

/****************************************************************
** Function name:           Daily_SleepGetDailyDataCallback
** Descriptions:            睡眠算法获取日常数据接口
** input parameters:
** output parameters:
** Returned value:
** Created by:              WJ             
** Created Date:            2016-12-07
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
//current_pkg = cur_pkg_info.pkg_cnt - 1;
uint32_t Daily_SleepGetDailyDataCallback(uint8_t *data, uint16_t current_pkg, uint16_t pre_offset)
{	
    uint8_t ret = 0;
    uint8_t temp_data[DAILY_DATA_PER_SIZE] = {0}; 
    uint16_t temp_block_index = 0;
    uint16_t temp_offset_index = 0;
    daily_block_info_t daily_block_info;
    daily_data_format_t daily_data_tmp;           //5分钟保存数据结构体
    
    temp_block_index = cur_pkg_info.block;
    temp_offset_index = cur_pkg_info.pkg_cnt;

    /* current_pkg的范围【0,287】,pre_offset 的范围【0,287】*/
    
    if(current_pkg >= temp_offset_index || data == NULL)
    {
        return NRF_ERROR_NOT_FOUND;
    }
    

    if(pre_offset <= current_pkg)
    {
        /* 条件一:读取当天数据 */
        temp_offset_index = current_pkg - pre_offset;
    }
    else
    {
        /* 条件二:读取跨天数据 */
        if(temp_block_index == 0)
        {
            temp_block_index = DAILY_DATA_BLOCK_MAX - 1; 
        }
        else
        {
            temp_block_index--;
        }
        App_FlashReadDailyHead(temp_block_index, (void *)&daily_block_info, sizeof(daily_block_info_t)); 
        
        if(daily_block_info.block_flag == DAILY_HEAD_FLAG_USED)
        {
            if(daily_block_info.pack_num > 0 && ((daily_block_info.pack_num ) >= (pre_offset - current_pkg)))
            {
                temp_offset_index = daily_block_info.pack_num - (pre_offset - current_pkg);
            }
            else
            {                
                return NRF_ERROR_NOT_FOUND;
            }
        }
        else
        {
            return NRF_ERROR_NOT_FOUND;
        }
    }
    
    /* 查找天的位置索引 */

    ret = App_FlashReadDailyData(temp_block_index, temp_offset_index, temp_data);
    if(ret != APP_FLASH_SUCCESS)
    {   
        return NRF_ERROR_NOT_FOUND;
    }       
    
    if((temp_data[1] > 31)||(temp_data[2] > 23)||(temp_data[3] > 59))
    {
            /* 这里需要判断每条数据的日、时、分为0xFF时,则为无效数据 */
            //Sleep_Callback_Trace(5,current_pkg,pre_offset,temp_day_index,temp_block_index,temp_offset_index);
            return NRF_ERROR_NOT_FOUND;
    }
    else
    {
        memcpy(&daily_data_tmp, temp_data, sizeof(daily_data_format_t));
        data[0] = daily_data_tmp.day;
        data[1] = daily_data_tmp.hour;
        data[2] = daily_data_tmp.minute;
        data[3] = daily_data_tmp.steps_h;
        data[4] = daily_data_tmp.steps_l;
        data[5] = daily_data_tmp.movement_h;
        data[6] = daily_data_tmp.movement_l;
        data[7] = daily_data_tmp.heart;        
    }
    
    return NRF_SUCCESS;
}

/****************************************************************
** Function name:           Daily_GetPkgInfo
** Descriptions:            获取存储块号和记录条数信息
** input parameters:
** output parameters:
** Returned value:
** Created by:              WJ             
** Created Date:            2016-12-07
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
daily_pkg_info_t *Daily_GetPkgInfo(void)
{
    return &cur_pkg_info;
}

/****************************************************************
** Function name:           Daily_OperationFlashCallback
** Descriptions:            无
** input parameters:        
** output parameters:       无
** Returned value:          获取是否成功
** Created by:              WJ             
** Created Date:            2016-12-07
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
uint8_t Daily_GetBlockHeadInfo(daily_block_info_t *daily_block_info, uint8_t block_num)
{
    uint8_t ret = 0;
    
    ret = App_FlashReadDailyHead(block_num, (void *)daily_block_info, sizeof(daily_block_info_t)); 
    if(ret == APP_FLASH_SUCCESS)
    {
        return 1;
    }
    return 0; 
}

/****************************************************************
** Function name:           Daily_GetAllWalkCntByDays
** Descriptions:            获取指定天数的记步总数
** input parameters:        几天前
** output parameters:       无
** Returned value:          返回计步数
** Created by:              WJ             
** Created Date:            2017-02-16
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
uint32_t Daily_GetAllWalkCntByDays(uint8_t ago_days)
{
    uint8_t block = 0;
    uint8_t month = 0;
    uint8_t day = 0;
    uint32_t all_walk = 0;
    daily_block_info_t daily_block_info;
    
    if(ago_days == 0)
    {
        all_walk = All_StepGet();
        return all_walk; 
    }
    
    block = Daily_GetPreviousBlock( ago_days);
    App_FlashReadDailyHead(block, (void *)&daily_block_info, sizeof(daily_block_info_t));   
    Sclock_GetPreviousDayDate (ago_days, &month, &day);
    if(daily_block_info.block_flag == 0 && daily_block_info.month == month && daily_block_info.day == day)
    {    
        all_walk = daily_block_info.all_step[2] 
                + (daily_block_info.all_step[1]>>8) + (daily_block_info.all_step[0]>>16);
    }
    return all_walk;
}

/****************************************************************
** Function name:           Daily_TestSleepAddPkg
** Descriptions:            睡眠仿真使用
** input parameters:
** output parameters:
** Returned value:
** Created by:              WJ             
** Created Date:            2016-12-07
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Daily_TestSleepAddPkg(void)
{
    cur_pkg_info.pkg_cnt++;
}

/****************************************************************
** Function name:           Daily_SetSleepCode
** Descriptions:            设置的睡眠计算返回代码
** input parameters:        无
** output parameters:       无
** Returned value:
** Created by:              WJ             
** Created Date:            2016-12-07
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Daily_SetSleepCode(uint8_t code)
{   
    sleep_code = code;
}

/****************************************************************
** Function name:           Daily_GetSleepCode
** Descriptions:            获取最后设置的睡眠计算返回代码
** input parameters:        无
** output parameters:       无
** Returned value:          返回代码
** Created by:              WJ             
** Created Date:            2016-12-07
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
uint8_t Daily_GetSleepCode(void)
{   
    return sleep_code;
}

/****************************************************************
** Function name:           Daily_InitFromFlash
** Descriptions:            
** input parameters:
** output parameters:
** Returned value:
** Created by:              WJ              
** Created Date:            2016-12-05
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Daily_InitFromFlash(void)
{   
    uint8_t i = 0;
    uint16_t j = 0;
    uint8_t error_cnt = 0;
    uint8_t month = 0;
    uint8_t day = 0;
    uint8_t c_month = 0;
    uint8_t c_day = 0;    
    uint32_t all_step = 0;    
    uint32_t all_calories = 0;
    uint32_t all_distance = 0;
    daily_data_format_t daily_data_temp;  
    daily_block_info_t daily_block_info;
    
    Daily_Init();
    for(i = DAILY_DATA_BLOCK_0; i < DAILY_DATA_BLOCK_MAX; i++)
    {
        App_FlashReadDailyHead(i, (void *)&daily_block_info, sizeof(daily_block_info_t)); 
        if(daily_block_info.block_flag == DAILY_HEAD_FLAG_USING)
        {           
            for(j = 0; j < DAILY_PKG_MAX_NUM - 2; )
            {
                App_FlashReadDailyData(i, j, (void *)&daily_data_temp); 
                if(daily_data_temp.day != 0xFF && daily_data_temp.month != 0xFF)
                {
                    if(daily_data_temp.steps_l != 0xFF && daily_data_temp.steps_h != 0xFF)
                    {
                        all_step += daily_data_temp.steps_l + (daily_data_temp.steps_h << 8);    
                        c_month = daily_data_temp.month;
                        c_day = daily_data_temp.day;
                        all_calories += daily_data_temp.calories_l + (daily_data_temp.calories_h << 8); 
                        all_distance += daily_data_temp.distance_l + (daily_data_temp.distance_h << 8); 
                        
                        if(daily_data_temp.reserve6 || daily_data_temp.reserve7)
                        {
                            power_statistics_s.rest_cnt++;
                        }
                    }
                    error_cnt = 0;
                }
                else
                {
                    error_cnt++;
                }
                j++;
                if(error_cnt >= 3)
                {
                    break;                        
                }
            }
            Daily_SetDailyPkgInfo(i, j - error_cnt);
            
            all_calories = all_calories * 100;
            Ped_SetSportsInfo(all_step, all_calories, all_distance);
            
            daily_last_info_s.last_calories = all_calories;
            daily_last_info_s.last_distance = all_distance;
            daily_last_info_s.last_setp_num = all_step;
            
            if((daily_block_info.month != clock->month || daily_block_info.day != clock->day))
            {
                if(c_month != clock->month || c_day != clock->day)
                {
                    Sclock_GetPreviousDayDate(1, &month, &day);
                    Daily_DailyHeadSetUsed(month, day);
                }
            }            
            break;
        }
        else if(daily_block_info.block_flag == 0xFF)
        {
            Daily_ClearDailyByBlcak(i);
            break;
        }
    }
    
    if(i >= DAILY_DATA_BLOCK_MAX)
    {
        Daily_ClearDailyByBlcak(0);
    }    
}

