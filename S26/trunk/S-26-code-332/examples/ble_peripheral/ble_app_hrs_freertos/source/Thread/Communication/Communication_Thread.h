/******************************************************************************
 * Copyright (c) 2017 - 2018, Veepoo
 * 
 * All rights reserved.
 * 
** 文件名称: communication_thread.h
** 创建时间: 2017-06-06
** 创 建 人: GT
** 描    述: 文件提供与APP通信线程的基本功能接口及完成相应的任务,任务描述如下:
**           1.完成五分钟数据的发送;
**           2.完成血氧数据的发送;
**           3.完成睡眠数据的发送;
**           4.完成心率实时数据的发送;
**           5.完成血氧实时数据的发送;
**           6.完成疲劳度实时数据的发送;
**
*******************************************************************************/
#ifndef _COMMUNICATION_THREAD_H_
#define _COMMUNICATION_THREAD_H_

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

// 通信线程资源管理结构体
typedef struct
{
    SemaphoreHandle_t  m_upload_adc_sem;          // 记步数据实时发送信号量(二值)
    SemaphoreHandle_t  m_all_flash_sem;           // FLASH区所有数据发送信号量(二值)
    SemaphoreHandle_t  m_gensor_data_sem;         // 发送心率实时数据信号量(二值)
    SemaphoreHandle_t  m_pedometer_data_sem;      // 记步数据实时发送信号量(二值)
    SemaphoreHandle_t  m_wechat_data_sem;         // 发送微信数据信号量(二值)
    SemaphoreHandle_t  m_daily_data_sem;          // 发送五分钟数据信号量(二值)
    SemaphoreHandle_t  m_sleep_data_sem;          // 发送五分钟数据信号量(二值)
    SemaphoreHandle_t  m_oxygen_data_sem;         // 发送血氧数据信号量(二值)
    SemaphoreHandle_t  m_scatter_heart_sem;       // 发送散点图心率数据信号量(二值)
    SemaphoreHandle_t  m_alarm_data_sem;          // 发送闹铃数据信号量(二值)
    SemaphoreHandle_t  m_movement_sem;            // 发送运动模式数据信号量(二值)
    SemaphoreHandle_t  m_heart_app;               // 发送心率实时数据信号量(二值)
    SemaphoreHandle_t  m_oxygen_app;              // 发送血氧实时数据信号量(二值)
    SemaphoreHandle_t  m_fatigue_app;             // 发送疲劳度实时数据信号量(二值)
    SemaphoreHandle_t  m_resp_app;                // 发送疲劳度实时数据信号量(二值)
    SemaphoreHandle_t  m_hourglass_app;           // 发送倒计时实时数据信号量(二值)
    SemaphoreHandle_t  m_bp_app;                  // 发送血压实时数据信号量(二值)
        
    SemaphoreHandle_t  m_task_count;              // 通信数据任务计数信号量

    TaskHandle_t       m_thread_handle;           // 线程句柄
}communication_resource_t;

// 日常数据上传控制参数
typedef struct
{
    uint16_t  start;      // 起始包数
    uint16_t  total;      // 总包数
    uint16_t  split;      // 分割包数
    uint8_t   day_id;     // 序号
    uint16_t  last_index;   // 记录上传最后的位置，
}daily_upload_param_t;

// 睡眠数据上传控制参数
typedef struct
{
    uint8_t  total_package_cnt;  //总包数
    uint8_t  sleep_wake_cnt;     //睡眠苏醒次数
    uint8_t  day_id;             //睡眠天数ID
}sleep_upload_param_t;

typedef struct
{
    uint8_t  send_data[80];         // 发送数据
    sleep_upload_param_t  sleep;    // 睡眠上传参数
    daily_upload_param_t  daily;    // 日常数据上传参数
    daily_upload_param_t  oxygen;   // 血氧数据上传参数
    daily_upload_param_t  sheart;   // HRV散点图上传参数
    daily_upload_param_t  movement; // 运动模式上传参数
}com_upload_param_t;

// 通信线程初始化
void ComThread_Init(void);

//获取发送FLASH原始数据的状态
uint8_t ComThread_GetSendFlashDataState(void);
    
// 发送ADC数据
void ComThread_SendADCData(void);

// 发送FLASH数据
void ComThread_SendAllFlashData(void);

// 发送五分钟存储数据
void ComThread_SendDailyData(uint8_t* p_data);

// 发送HRV散点图心率存储数据
void ComThread_SendScatterHeartData(uint8_t* p_data);

//发送运动模式数据
void ComThread_SendMovementData(void);

// 通信线程唤醒发送睡眠数据任务(全天)
void ComThread_SendSleepData(uint8_t* p_data);

// 通信线程唤醒发送血氧数据任务(全天)
void ComThread_SendOxygenData(uint8_t* p_data);

// 发送闹铃数据到APP端(最多20组)
void ComThread_SendAlarmData(void);

// 通信线程周期性任务
void ComThread_PeriodCycleTask(uint8_t hour, uint8_t minute, uint8_t second);

#endif
