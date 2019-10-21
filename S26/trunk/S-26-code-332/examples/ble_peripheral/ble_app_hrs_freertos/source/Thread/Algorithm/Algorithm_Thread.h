/******************************************************************************
 * Copyright (c) 2017 - 2018, Veepoo
 * 
 * All rights reserved.
 * 
** 文件名称: algorithm_thread.h
** 创建时间: 2017-06-02
** 创 建 人: GT
** 描    述: 文件完成系统算法的任务,任务描述如下:
**           1.完成血氧计算算法;
**           2.完成睡眠时间计算算法;
**           3.完成佩戴检测计算算法;
**           4.完成记步计算算法;
**           5.完成姿态计算算法;
**
*******************************************************************************/
#ifndef _ALGORITHM_THREAD_H_
#define _ALGORITHM_THREAD_H_

#include <stdbool.h>
#include "flash_data_type.h"
#include "cpu_task_data_type.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "data_manage.h"

//-----------------------------------
//        算法线程结构体
//-----------------------------------
typedef struct
{
    //SemaphoreHandle_t  m_battery_sem;               // 电池电量计算信号量
    //SemaphoreHandle_t  m_hourglass_sem;             // 倒计时信号量
    //SemaphoreHandle_t  m_sedentary_sem;             // 久坐计算信号量
    SemaphoreHandle_t  m_step_sport_trace_sem;      // 记步运动量跟踪信号量
    //SemaphoreHandle_t  m_alarm_sem;                 // 闹铃计算信号量
    //SemaphoreHandle_t  m_brightness_sem;            // 亮度计算信号量
    SemaphoreHandle_t  m_blood_oxygen_measure_sem;  // 血氧算法信号量
    SemaphoreHandle_t  m_osahs_sem;                 // 呼吸暂停算法信号量
    SemaphoreHandle_t  m_hrv_sem;                   // HRV算法信号量
    SemaphoreHandle_t  m_br_sem;                    // 呼吸率算法信号量
    SemaphoreHandle_t  m_wear_sem;                  // 佩戴算法信号量
    SemaphoreHandle_t  m_no_wear_sem;               // 脱手佩戴算法信号量
    SemaphoreHandle_t  m_heart_sem;                 // 心率算法信号量
    SemaphoreHandle_t  m_pedometer_sem;             // 记步算法信号量
    SemaphoreHandle_t  m_gesture_sem;               // 姿态算法信号量
    SemaphoreHandle_t  m_fatigue_sem;               // 疲劳度算法信号量
    SemaphoreHandle_t  m_sleep_sem;                 // 睡眠算法信号量
    SemaphoreHandle_t  m_sleep_traversing_sem;      // 睡眠算法信号量

    SemaphoreHandle_t  m_bp_sem;                    // 血压
    SemaphoreHandle_t  m_one_seconds_sem;           // 一秒事件  
    
    SemaphoreHandle_t  m_task_count;                // 任务数量计数信号量
    
    TaskHandle_t       m_thread_handle;             // 线程句柄
}algorithm_thread_resource_t;


extern detection_char_info_t oxygen_detection;

// 算法线程初始化
void AlgThread_Init(void);

// AFE4404设备任务
void AlgThread_AFE4404Task(void);

// 算法线程启动呼吸暂停任务
void AlgThread_Osahs(void);

// 算法线程启动睡眠遍历任务
void AlgThread_SleepTraversing(void);

// GSensor事务提交
void AlgThread_GsensorEventCommit(void);
	
// 算法线程周期性任务
void AlgThread_PeriodCycleTask(uint8_t hour, uint8_t minute, uint8_t second);

void Algorithm_AlarmValidCheck(void);

uint8_t Alarm_GetValidConfigNum(void);

void Algorithm_OledBrightnessAdjustTask(void);

// 心率校准
//void Algorithm_HeartCalibTask(void);

void Algorithm_Spo2CalibTask(uint8_t mode, uint8_t set_red_cur, uint8_t set_ird_cur);

// 关闭自动测量
void AlgThread_OffTimeMeasurement(void);

// 血压测量模式设置
void Algorithm_BpSetModeParam(void);

#endif
