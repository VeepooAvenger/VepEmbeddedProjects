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

/****************************************************************
** Function name:             Sleep_SaveData
** Descriptions:              睡眠存储数据
** input parameters:          无
** output parameters:         无
** Returned value:            无
** Created by:                WJ
** Created Date:              2016-11-29
****************************************************************/ 
static void Sleep_SaveData(const vph_sleep_info_t* p_sleep_info)
{
    uint8_t curr_month = 0;
    uint8_t curr_day = 0;
    uint8_t yester_day = 0;
    flash_find_index_t*  p_find_index = &p_flash_param->flash_index_s;
    vph_sleep_quality_info_t vsis_sleep_quality_info;
    
    memset(&vsis_sleep_quality_info, 0, sizeof(vph_sleep_quality_info_t));
    curr_day = clock->day;
    curr_month = clock->month; 

    vsis_sleep_quality_info.head = 0xA1;        //数据标志
    /*---------------------------- 保存苏醒时间  -------------------------*/
    vsis_sleep_quality_info.sleep_out_time[0] = curr_month;//clock->month;        //月
    vsis_sleep_quality_info.sleep_out_time[1] = p_sleep_info->quit_time.date;
    vsis_sleep_quality_info.sleep_out_time[2] = p_sleep_info->quit_time.hour;
    vsis_sleep_quality_info.sleep_out_time[3] = p_sleep_info->quit_time.minute;
    
    //入睡时间处理
    if(p_sleep_info->quit_time.date != p_sleep_info->enter_time.date)//十二点之后入睡
    {
        //入睡时间在前一天
        //WJ.20180104.睡眠在1月份，入睡和退出睡眠不在同一天的bug
        //if(vsis_sleep_quality_info.sleep_out_time[0] == 1)
        if(p_sleep_info->quit_time.date == 1 && vsis_sleep_quality_info.sleep_out_time[0] == 1)//苏醒月份是一月
        {
            vsis_sleep_quality_info.sleep_enter_time[0] = 12;//入睡月份12月(苏醒的前一个月月份)
        }
        else
        {
            Sclock_GetPreviousDayDate(1, &curr_month, &yester_day);
            vsis_sleep_quality_info.sleep_enter_time[0] = curr_month;//clock->month;   //WJ.20170303.修正入睡月份错误的BUG
        }    //苏醒时间当前月份 
    }
    else
    {
        if(p_sleep_info->quit_time.date != curr_day)
        {
            Sclock_GetPreviousDayDate(1, &curr_month, &yester_day);
//            vsis_sleep_quality_info.sleep_enter_time[0] = curr_month;   //WJ.20170619.修正退出睡眠月份错误的BUG            
        }
//        else
//        {
        vsis_sleep_quality_info.sleep_enter_time[0] = curr_month;//clock->month;//入睡时间当前月份(苏醒时候的月份)
//        }
    }
    
    /*-------------- 保存入睡时  --------------*/
    vsis_sleep_quality_info.sleep_enter_time[1] = p_sleep_info->enter_time.date;
    vsis_sleep_quality_info.sleep_enter_time[2] = p_sleep_info->enter_time.hour;
    vsis_sleep_quality_info.sleep_enter_time[3] = p_sleep_info->enter_time.minute;
    /*---------------------------- 保存睡眠时间及质量  -------------------------*/
    vsis_sleep_quality_info.sleep_quality[0] = p_sleep_info->deep_time;                //深睡值
    vsis_sleep_quality_info.sleep_quality[1] = p_sleep_info->shallow_time;            //浅睡值
    vsis_sleep_quality_info.sleep_quality[2] = p_sleep_info->quanlity_val;            //睡眠质量
    vsis_sleep_quality_info.total_time = p_sleep_info->deep_time + p_sleep_info->shallow_time;        //总睡眠时间长度
    /*------------------------------- 保存睡眠曲线 -----------------------------*/
    memcpy(vsis_sleep_quality_info.sleep_shallow, p_sleep_info->deep_shallow, 31);        //拷贝睡眠曲线， 长度为洗衣支持长度31个byte，可存储20个小时数据
    
    /*------------------------------- 保存中途觉醒时刻 -------------------------------*/
    vsis_sleep_quality_info.start_time[0] = 209;
    vsis_sleep_quality_info.end_time[0] = 210;
    memcpy(&vsis_sleep_quality_info.start_time[1], p_sleep_info->start_time, 11);
    memcpy(&vsis_sleep_quality_info.end_time[1], p_sleep_info->end_time, 11);    

    // 注意:当天8点以后入睡存在当天块
    if((p_sleep_info->enter_time.hour >= 8)&&(p_sleep_info->enter_time.date == clock->day))
    {
        Api_SaveSleepResultData(p_find_index->block_index,(uint8_t*)&vsis_sleep_quality_info);
    }
    else
    {
        // 否则存在前一天
        Api_SaveSleepResultData((p_find_index->block_index+2)%3,(uint8_t*)&vsis_sleep_quality_info);
    }
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
    vph_sleep_info_t  m_sleep_info = {0};
    const vph_sleep_info_t* p_sleep_info = Vs_GetSleepingInfo();
    uint8_t daily_id = p_flash_param->flash_index_s.block_index%3;
    uint16_t current_package = p_flash_param->flash_index_s.daily.offset[daily_id];
        
//    if(p_sleep_info->sleep_flag  != 0x01) 
//    {
//        Vs_ClearInfoMsg();      //清楚睡眠缓存数据
//        return;
//    }
    
    //调用睡眠计算，并作相关处理
    Api_SetSleepBlack(daily_id);
    Vph_SleepLowBattery(current_package);
    if(p_sleep_info->sleep_flag  == 3) //退出睡眠       
    {
        memcpy(&m_sleep_info,p_sleep_info,sizeof(vph_sleep_info_t));
        
        // 更新退出时间
        m_sleep_info.quit_time.date = clock->day;
        m_sleep_info.quit_time.hour = clock->hour;
        m_sleep_info.quit_time.minute = clock->minutes;
        
        Sleep_SaveData(&m_sleep_info);
        Vs_ClearInfoMsg();        
    } 
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
    const vph_sleep_info_t* p_sleep_info = Vs_GetSleepingInfo();//得到睡眠数据
    uint8_t daily_id = p_flash_param->flash_index_s.block_index%3;//天序号
    uint16_t current_package = p_flash_param->flash_index_s.daily.offset[daily_id];//五分钟数据偏移量
    
    //调用睡眠计算，并作相关处理
    Api_SetSleepBlack(daily_id);
    p_data_manage->daily_attr.sleep_err = Vph_SleepDetectEntery(current_package);//日常数据
    if(p_sleep_info->sleep_flag  == 3)//已经退出睡眠进行存储       
    {
        Sleep_SaveData(Vs_GetSleepingInfo());//
        Vs_ClearInfoMsg();        
    }     
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
    const vph_sleep_info_t* p_sleep_info = Vs_GetSleepingInfo();//
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
    
    // 1.清空睡眠缓存数据
    Vs_ClearInfoMsg();  
    
    do{
        // 2.调用睡眠计算，遍历处理(前一天20:00---当前时间)
        for(; daily_pkg <= daily_max_pkg; daily_pkg++)
        {
            Vph_SleepDetectEntery(daily_pkg);
            if(p_sleep_info->sleep_flag  == 3)        
            {
                Sleep_SaveData(Vs_GetSleepingInfo());
                Vs_ClearInfoMsg();        
            } 
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
    
    Api_SetSleepBlack(daily_id);
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
    Vph_SleepingInit(Api_SleepCallback);
    Vs_ClearInfoMsg();   
}
