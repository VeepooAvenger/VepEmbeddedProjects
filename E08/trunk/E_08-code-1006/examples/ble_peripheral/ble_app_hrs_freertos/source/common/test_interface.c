/*******************Copyright(c)*********************************
** Veepoo Tech
** COPYRIGHT (C) 2016-2019
**----------------------FileInfo---------------------------------
** File name:                  test_interface.h
** Last modified Date:         2018-11-14
** Last SYS_VERSION:           0.0.1
** Descriptions:
**---------------------------------------------------------------
** Createdby:                  WJ
** Createddate:                2018-11-14
** SYS_VERSION:                0.0.1
** Descriptions:               测试接口
**---------------------------------------------------------------
** Modified by:				
** Modified date:			
** SYS_VERSION:				
** Modified Descriptions:	
****************************************************************/
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include "test_interface.h"

#include "ext_flash.h"
#include "gui_thread.h"
#include "data_manage.h"
#include "api_flash.h"
#include "flash_data_type.h"
#include "SystemTime.h"
#include "api_ext_flash.h"

#include "ble_thread.h"
#include "communication_thread.h"

#include "Acculate_SleepAlgorithm.h"

/****************************************************************
** Function name:           Test_SetEcgResultData
** Descriptions:            设置ECG手动测量结果，并保存
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2018-11-14
*****************************************************************/ 
void Test_SetEcgResultData(void)
{
    uint8_t   set_data_buf[14] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13};
   
    p_data_manage->ecg_result_data_s.mark = 0xAB;
    p_data_manage->ecg_result_data_s.year = clock->year;
    p_data_manage->ecg_result_data_s.month = clock->month;
    p_data_manage->ecg_result_data_s.day = clock->day;
    p_data_manage->ecg_result_data_s.hour = clock->hour;
    p_data_manage->ecg_result_data_s.minute = clock->minutes;
    p_data_manage->ecg_result_data_s.second = clock->seconds;
    
    memcpy(p_data_manage->ecg_result_data_s.ecg_result_data, set_data_buf, 14);
    
    p_data_manage->ecg_result_data_s.ecg_res[0] = 0xF1;
    p_data_manage->ecg_result_data_s.ecg_res[1] = 0xF2;
    
    
    BleThread_SaveEcgResultDataSem();
    
}

/****************************************************************
** Function name:           Test_SetAutomaticEcgResultData
** Descriptions:            设置ECG自动测量异常结果，并保存
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2018-11-14
*****************************************************************/ 
void Test_SetAutomaticEcgResultData(void)
{
    uint8_t buf[25] = {0};
    
    for(uint8_t i = 0; i < 25; i++)
    {
        buf[i] = i;
    }
    memcpy((uint8_t *)&p_data_manage->ecg_ppg_s.ecg_measure_s.ecg_test_result_s, buf, 25);
    for(uint16_t i = 0; i < 250*6; i++)
    {
        p_data_manage->data_pool.ecg_data[i] = i;
    }
 
    Extflash_EcgWriteSet((uint8_t *)p_data_manage->data_pool.ecg_data, 250*6*4, 
                         (uint8_t *)&p_data_manage->ecg_ppg_s.ecg_measure_s.ecg_test_result_s, 25);
    
    BleThread_ExtFlashOperationSem();
    
}

/****************************************************************
** Function name:           Test_SendEcgResultId
** Descriptions:            发送ecg测量结果id
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2018-11-15
*****************************************************************/ 
void Test_SendEcgResultId(void)
{
   // uint8_t time_data[7] = {0xE2, 0x07, 0x0B, 0x0F, 0x0A, 0x00, 0x00};
    uint8_t time_data[7] = {0xE2, 0x07, 0x0B, 0x0F, 0x0A, 0x25, 0x1E};
        
    ComThread_SendEcgIdData(0x02, time_data);
}

/****************************************************************
** Function name:           Test_SendEcgResultDataById
** Descriptions:            发送ecg测量结果,通过id
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2018-11-15
*****************************************************************/ 
void Test_SendEcgResultDataById(void)
{
    ComThread_SendEcgDataById(0x02, 0x0A, NULL);
}

/****************************************************************
** Function name:           Test_SendEcgAutomaticResultId
** Descriptions:            发送ecg自动测量结果id
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2018-11-15
*****************************************************************/ 
void Test_SendEcgAutomaticResultId(void)
{
    uint8_t time_data[7] = {0xE2, 0x07, 0x0B, 0x0F, 0x0A, 0x00, 0x00};
        
    ComThread_SendEcgIdData(0x03, time_data);
}

/****************************************************************
** Function name:           Test_SendEcgAutomaticResultDataById
** Descriptions:            发送ecg自动测量结果,通过id
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2018-11-15
*****************************************************************/ 
void Test_SendEcgAutomaticResultDataById(void)
{
    ComThread_SendEcgDataById(3, 0, NULL);
}

/****************************************************************
** Function name:           Test_SetSleepData
** Descriptions:            设置精准睡眠的数据
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2018-11-14
*****************************************************************/ 
uint8_t *Test_SetSleepData(uint8_t *p_sleep_info)
{
    static uint8_t sleep_cnt = 0;
	acculate_sleep_quality_t *p_asq = NULL;
	
	p_asq = (acculate_sleep_quality_t *)p_sleep_info;
	
    sleep_cnt = 3;
    if(sleep_cnt == 0)
    {
        // First Part
        p_asq->enter_time.month = clock->month; //入睡时间点
        p_asq->enter_time.date = clock->day;
        p_asq->enter_time.hour = 0;
        p_asq->enter_time.minute = 8;
        
        p_asq->quit_time.month = clock->month; //退出时间点
        p_asq->quit_time.date = clock->day;
        p_asq->quit_time.hour = 12;
        p_asq->quit_time.minute = 8;
        
        p_asq->sleep_flag = 1;//睡眠标志位
        p_asq->startup_quality = 1; //起夜得分
        p_asq->deepsleep_quality = 1; //深睡得分
        p_asq->averstarttodeep_quality = 1; //睡眠效率得分
        p_asq->firstdeeptime_quality = 1; //入睡效率得分
        p_asq->all_sleep_len_quality = 1; //睡眠时长得分
        p_asq->quit_mode = 1; //退出睡眠模式
        p_asq->quanlity_val = 1; //睡眠质量
        p_asq->startup_times = 1;//起夜次数
        p_asq->deep_shallow_len_mode = 1; // 深浅睡眠模式
        p_asq->deep_sleep_len = 450; //深睡时长
        p_asq->shallow_sleep_len = 270; //浅睡时长
        p_asq->else_sleep_len = 1; //其他时长
        p_asq->all_sleep_len = 720; //睡眠总时长
        p_asq->first_deep_time = 1; //第一次进入深睡时间
        p_asq->startup_len = 1; //起夜总时长
        p_asq->aver_start_to_deep = 1; //起夜到深睡均值
        p_asq->one_point_how_many_seconds = 1; //一个点代表多少秒
        
        // Second Part
        p_asq->loss_sleep_flag = 1; //失眠标志
        p_asq->loss_sleep_degree = 1; //失眠得分
        p_asq->loss_sleep_times = 1; //失眠次数
        p_asq->loss_sleep_length = 1; //失眠长度
        //p_asq->loss_sleep_start_time[10]; //失眠开始时间
        for(uint32_t i = 0; i < 10; i++)
        {
            p_asq->loss_sleep_start_time[i] = i + 1;
        }
        //p_asq->loss_sleep_end_time[10]; //失眠结束时间
        for(uint32_t i = 0; i < 10; i++)
        {
            p_asq->loss_sleep_end_time[i] = i + 1;
        }
        
        // Third Part
        p_asq->temp_len = 1440;//深浅睡眠数组有效长度
        //p_asq->deep_shallow[2880]; //深浅睡眠数组
        for(uint32_t i = 0; i < 2880; i++)
        {
            p_asq->deep_shallow[i] = i & 0xff;
        }

        // Forth Part
        p_asq->CRC_value1 = 0xAA;//基本睡眠信息校验CRC1
        p_asq->CRC_value2 = 0x55;//失眠信息校验CRC2
        p_asq->CRC_value3 = 0xAA;//睡眠曲线信息校验CRC3
	
    }
    else if(sleep_cnt == 1)
    {
       // First Part
        p_asq->enter_time.month = clock->month; //入睡时间点
        p_asq->enter_time.date = clock->day;
        p_asq->enter_time.hour = 13;
        p_asq->enter_time.minute = 8;
        
        p_asq->quit_time.month = clock->month; //退出时间点
        p_asq->quit_time.date = clock->day;
        p_asq->quit_time.hour = 15;
        p_asq->quit_time.minute = 8;
        
        p_asq->sleep_flag = 1;//睡眠标志位
        p_asq->startup_quality = 1; //起夜得分
        p_asq->deepsleep_quality = 1; //深睡得分
        p_asq->averstarttodeep_quality = 1; //睡眠效率得分
        p_asq->firstdeeptime_quality = 1; //入睡效率得分
        p_asq->all_sleep_len_quality = 1; //睡眠时长得分
        p_asq->quit_mode = 1; //退出睡眠模式
        p_asq->quanlity_val = 1; //睡眠质量
        p_asq->startup_times = 1;//起夜次数
        p_asq->deep_shallow_len_mode = 1; // 深浅睡眠模式
        p_asq->deep_sleep_len = 100; //深睡时长
        p_asq->shallow_sleep_len = 20; //浅睡时长
        p_asq->else_sleep_len = 1; //其他时长
        p_asq->all_sleep_len = 120; //睡眠总时长
        p_asq->first_deep_time = 1; //第一次进入深睡时间
        p_asq->startup_len = 1; //起夜总时长
        p_asq->aver_start_to_deep = 1; //起夜到深睡均值
        p_asq->one_point_how_many_seconds = 1; //一个点代表多少秒
        
        // Second Part
        p_asq->loss_sleep_flag = 1; //失眠标志
        p_asq->loss_sleep_degree = 1; //失眠得分
        p_asq->loss_sleep_times = 1; //失眠次数
        p_asq->loss_sleep_length = 1; //失眠长度
        //p_asq->loss_sleep_start_time[10]; //失眠开始时间
        for(uint32_t i = 0; i < 10; i++)
        {
            p_asq->loss_sleep_start_time[i] = i + 1;
        }
        //p_asq->loss_sleep_end_time[10]; //失眠结束时间
        for(uint32_t i = 0; i < 10; i++)
        {
            p_asq->loss_sleep_end_time[i] = i + 1;
        }
        
        // Third Part
        p_asq->temp_len = 240;//深浅睡眠数组有效长度
        //p_asq->deep_shallow[2880]; //深浅睡眠数组
        for(uint32_t i = 0; i < 2880; i++)
        {
            p_asq->deep_shallow[i] = i & 0xff;
        }

        // Forth Part
        p_asq->CRC_value1 = 0xFF;//基本睡眠信息校验CRC1
        p_asq->CRC_value2 = 0xCC;//失眠信息校验CRC2
        p_asq->CRC_value3 = 0xDD;//睡眠曲线信息校验CRC3        
    }
    else 
    {
       // First Part
        p_asq->enter_time.month = clock->month; //入睡时间点
        p_asq->enter_time.date = clock->day;
        p_asq->enter_time.hour = 16;
        p_asq->enter_time.minute = 8;
        
        p_asq->quit_time.month = clock->month; //退出时间点
        p_asq->quit_time.date = clock->day;
        p_asq->quit_time.hour = 19;
        p_asq->quit_time.minute = 8;
        
        p_asq->sleep_flag = 1;//睡眠标志位
        p_asq->startup_quality = 1; //起夜得分
        p_asq->deepsleep_quality = 1; //深睡得分
        p_asq->averstarttodeep_quality = 1; //睡眠效率得分
        p_asq->firstdeeptime_quality = 1; //入睡效率得分
        p_asq->all_sleep_len_quality = 1; //睡眠时长得分
        p_asq->quit_mode = 1; //退出睡眠模式
        p_asq->quanlity_val = 1; //睡眠质量
        p_asq->startup_times = 1;//起夜次数
        p_asq->deep_shallow_len_mode = 1; // 深浅睡眠模式
        p_asq->deep_sleep_len = 100; //深睡时长
        p_asq->shallow_sleep_len = 180; //浅睡时长
        p_asq->else_sleep_len = 1; //其他时长
        p_asq->all_sleep_len = 180; //睡眠总时长
        p_asq->first_deep_time = 1; //第一次进入深睡时间
        p_asq->startup_len = 1; //起夜总时长
        p_asq->aver_start_to_deep = 1; //起夜到深睡均值
        p_asq->one_point_how_many_seconds = 1; //一个点代表多少秒
        
        // Second Part
        p_asq->loss_sleep_flag = 1; //失眠标志
        p_asq->loss_sleep_degree = 1; //失眠得分
        p_asq->loss_sleep_times = 1; //失眠次数
        p_asq->loss_sleep_length = 1; //失眠长度
        //p_asq->loss_sleep_start_time[10]; //失眠开始时间
        for(uint32_t i = 0; i < 10; i++)
        {
            p_asq->loss_sleep_start_time[i] = i + 1;
        }
        //p_asq->loss_sleep_end_time[10]; //失眠结束时间
        for(uint32_t i = 0; i < 10; i++)
        {
            p_asq->loss_sleep_end_time[i] = i + 1;
        }
        
        // Third Part
        p_asq->temp_len = 180*2;//深浅睡眠数组有效长度
        //p_asq->deep_shallow[2880]; //深浅睡眠数组
        for(uint32_t i = 0; i < 2880; i++)
        {
            p_asq->deep_shallow[i] = i & 0xff;
        }

        // Forth Part
        p_asq->CRC_value1 = 0xEE;//基本睡眠信息校验CRC1
        p_asq->CRC_value2 = 0xDD;//失眠信息校验CRC2
        p_asq->CRC_value3 = 0xCC;//睡眠曲线信息校验CRC3        
    }
	return p_sleep_info;
}


/****************************************************************
** Function name:           Test_SendSleepDataById
** Descriptions:            发送睡眠数据
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2018-11-20
*****************************************************************/ 
void Test_SendSleepDataById(void)
{
    uint8_t data_buf[2] = {0xFF, 0x01};
    
    ComThread_SendSleepData(data_buf);
}

