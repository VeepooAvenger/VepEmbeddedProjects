/******************************************************************************
 * Copyright (c) 2017 - 2018, Veepoo
 * 
 * All rights reserved.
 * 
** 文件名称: gui_thread.h
** 创建时间: 2017-06-07
** 创 建 人: GT
** 描    述: 文件提供UI线程的控制和接口设计
**
*******************************************************************************/
#ifndef _GUI_THREAD_H_
#define _GUI_THREAD_H_

#include <stdint.h>
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "cpu_task_data_type.h"





typedef struct
{    
    uint8_t            init_complete;             //线程初始化标志
    uint8_t            sleep_flag;                //线程是否信号量阻塞
    uint8_t            sleep_time;                //线程阻塞休眠时间
    SemaphoreHandle_t  m_thread_sleep;            // UI线程休眠
    
    SemaphoreHandle_t  m_task_count;              // 任务数量计数信号量
    TaskHandle_t       m_thread_handle;           // 线程句柄
}gui_resource_t;

//任务参数结构体
typedef struct
{
    uint32_t          pid;             // 描述当前GUI的实时ID
    watch_timer_t     watch;           // 秒表
    movement_cont_t   movement;        // 运动模式
    cpu_task_tcb_t    task_tcb;
    cpu_event_tcb_t   event_tcb;	
    cpu_device_tcb_t  device_tcb;
}cpu_param_t;

extern uint8_t display_language;

// GUI线程初始化
void GuiThread_Init(void);

// GUI线程页面是否激活
bool GuiThread_PageActive(void);

// GUI线程添加按键事件
void GuiThread_AddKeyEvent(BUTTON_EVENT_TYPE event, uint8_t times);

// GUI线程添加事件
uint8_t GuiThread_AddEvent(CPU_EVENT_TYPE event);

// 清除事件
void GuiThread_ClearEvent(CPU_EVENT_TYPE event);

// 设备是否正在查找手机
bool GuiThread_IsDuringFindPhone(void);

// GUI线程获取任务类型
CPU_TASK_TYPE GuiThread_GetTaskType(void);

// GUI线程获取事件类型
CPU_EVENT_TYPE GuiThread_GetEventType(void);

// GUI线程获取秒表结构体指针
const watch_timer_t* GuiThread_GetWatchTimer(void);

uint8_t GuiThread_TimerIsBusy(void);

// UI线程亮度调节任务唤醒
void GuiThread_BrightAdjust(void);

// GUI线程充电事件处理
void GuiThread_ChargingCommit(void);

// GUI线程充满事务提交
void GuiThread_ChargFullCommit(void);

// GUI线程清除所有参数
void GuiThread_ClearAllParam(void);

// 进去主界面
void GuiThread_InMainTask(void);

void GuiThread_ClearMovementParm(void);

//进入运动模式
void GuiThread_InMovementTask(void);

//设置当前显示区域，用于下次显示清屏
void Gui_ThreadSetDisPlayArea(uint8_t start_column, uint8_t start_page, uint8_t column_size, uint8_t page_size);

//更新主界面
void Gui_ThreadUpdateMian(void);

//清除任务的一些参数
void GuiThread_ClearTaskParam(void);


//spi 传输数据时UI线程休眠
void GuiThread_SpiSleep(void);

//spi 传输数据完成时UI线程唤醒
void GuiThread_SpiWeakUp(void);
#endif
