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
    SemaphoreHandle_t  m_battery_sem;               // 电池电量计算信号量
    SemaphoreHandle_t  m_hourglass_sem;             // 倒计时信号量
    SemaphoreHandle_t  m_sedentary_sem;             // 久坐计算信号量
    SemaphoreHandle_t  m_step_sport_trace_sem;      // 记步运动量跟踪信号量
    SemaphoreHandle_t  m_alarm_sem;                 // 闹铃计算信号量
    SemaphoreHandle_t  m_brightness_sem;            // 亮度计算信号量
    
    SemaphoreHandle_t  m_ecg_measure_sem;           // ECG算法信号量
    SemaphoreHandle_t  m_ppg_sem;                   // PPG算法信号量

    SemaphoreHandle_t  m_wear_sem;                  // 佩戴算法信号量
    SemaphoreHandle_t  m_no_wear_sem;               // 脱手佩戴算法信号量
    
    SemaphoreHandle_t  m_pedometer_sem;             // 记步算法信号量
    SemaphoreHandle_t  m_gesture_sem;               // 姿态算法信号量
    //SemaphoreHandle_t  m_fatigue_sem;               // 疲劳度算法信号量
    SemaphoreHandle_t  m_sleep_sem;                 // 睡眠算法信号量
    SemaphoreHandle_t  m_sleep_traversing_sem;      // 睡眠算法信号量
    SemaphoreHandle_t  m_task_count;                // 任务数量计数信号量
    SemaphoreHandle_t  m_blood_oxygen_measure_sem;  // 血氧算法信号量
    SemaphoreHandle_t  m_osahs_sem;                 // 呼吸暂停算法信号量
    SemaphoreHandle_t  m_one_seconds_sem;           // 一秒时间，处理秒事件算法
    SemaphoreHandle_t  m_exit_sleep_sem;            // 强制退出睡眠
    
    
    TaskHandle_t       m_thread_handle;             // 线程句柄
}algorithm_thread_resource_t;

// 算法线程初始化
void AlgThread_Init(void);

// 算法信号量处理
void AlgThread_CalcSignal(void);

// 算法线程启动睡眠遍历任务
void AlgThread_SleepTraversing(void);

// GSensor事务提交
void AlgThread_GsensorEventCommit(void);
	
// 算法线程周期性任务
void AlgThread_PeriodCycleTask(uint8_t hour, uint8_t minute, uint8_t second);

void Algorithm_AlarmValidCheck(void);

uint8_t Alarm_GetValidConfigNum(void);

void Algorithm_OledBrightnessAdjustTask(void);

void AlgThread_Osahs(void);

// 加入强制退出睡眠信号量
void AlgThread_ExitSleepSem(void);

#endif
