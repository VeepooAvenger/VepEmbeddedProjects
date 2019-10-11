/***************************Copyright(c)***********************************
**                       Veepoo Tech
**                  COPYRIGHT (C) 2016-2019
**-----------File Info-----------------------------------------------------
** File        name:     gui_thread.h
** Last modified Date:   2017-07-31
** Last SYS_VERSION:     0.0.0
** Descriptions:            
**-------------------------------------------------------------------------
** Created  by:          WJ
** Created  date:        2017-07-31
** SYS_VERSION:          0.0.0
** Descriptions:         
**-------------------------------------------------------------------------
** Modified by:				
** Modified date:  			          
** SYS_VERSION:                 
** Descriptions:                        
***************************************************************************/
#ifndef _GUI_THREAD_H_
#define _GUI_THREAD_H_

#include <stdint.h>
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "cpu_common.h"



#pragma pack(1)
typedef struct
{
	SemaphoreHandle_t  m_repaint_sem;             //GUI重绘信号量(二值)
	SemaphoreHandle_t  m_Task_count;              //通信数据任务计数信号量
	TaskHandle_t       m_thread_handle;           //线程句柄
}gui_thread_resource_t;

//任务参数结构体
typedef struct
{
	oled_driver_t          oled;         //OLED控制参数
	gui_controller_t       gui;          //静态页面参数
	
	CPU_EVENT_TYPE         event;        //事件
	CPU_TASK_TYPE          task;         //任务
	
	cpu_task_controller_t  event_control;    //运动任务参数
	cpu_task_controller_t  job_control;      //任务参数
}cpu_task_t;


typedef struct
{
    uint8_t button_id;
    uint8_t button_event;
}cpu_button_t;

#pragma pack()

extern uint8_t display_language;

extern uint8_t disconnet_keep_flg;
extern uint8_t disconnet_motor_flg;

//UI线程初始化
void Gui_ThreadInit(void);

// 秒表计时跟踪功能
void Gui_ThreadWatchTimerTrace(void);

bool Gui_ThreadPageActive(void);

bool Gui_ThreadTaskPageActive(void);

//是否处于运动界面
bool Gui_ThreadIsTaskRun(void);

// GUI线程添加按键事件
void Gui_ThreadAddKeyEvent(BUTTON_EVENT_TYPE event);

//GUI线程内容刷新任务唤醒  
void Gui_ThreadRepaintTaskWakeUp(void);

//任务唤醒
void Gui_ThreadTaskWakeUp(CPU_TASK_TYPE task);

//外部事件唤醒
void Gui_ThreadEventTaskWakeUp(CPU_EVENT_TYPE event);

// UI线程亮度调节任务唤醒
void Gui_ThreadBrightAdjustTaskWakeUp(void);

//姿态亮屏，返回0:亮屏事件添加失败, 1:亮屏事件添加成功
uint8_t Gui_ThreadPostureToBrightScreen(void);

//姿态关屏，返回0:关屏事件添加失败, 1:关屏事件添加成功
uint8_t Gui_ThreadPostureToCloseScreen(void);

//设置当前显示区域，用于下次显示清屏
void Gui_ThreadSetDisPlayArea(uint8_t start_column, uint8_t start_page, uint8_t column_size, uint8_t page_size);

//关机UI处理
void Gui_ThreadPowerOff(void);
//开机UI处理
void Gui_ThreadPowerOn(void);

//清除外部事件
void Gui_ThreadEventTaskClear(void);

//清除当前事件事件
void Gui_ThreadTaskClear(void);

//事件优先级处理，判断当前事件是否响应
uint8_t Gui_ThreadCanAcceptEvent(uint8_t event);

//更新秒表界面
void Gui_ThreadUpdateStopwatch(void);

//更新主界面
void Gui_ThreadUpdateMian(void);

//是否在查找手机界面
uint8_t Gui_ThreadIsFindPhone(void);

//任务界面进入空闲
void Gui_ThreadTaskToIdle(void);

//获取当前任务ID
uint8_t Gui_GetThreadTask(void);

//获取当前事件ID
uint8_t Gui_GetThreadJob(void);

void Gui_CheckEvent(void);

void Gui_SetChargingState(uint8_t state);
    
#endif
