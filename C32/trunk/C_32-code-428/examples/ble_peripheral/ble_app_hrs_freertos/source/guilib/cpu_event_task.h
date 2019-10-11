/***************************Copyright(c)***********************************
* *                      Veepoo Tech
* *                 COPYRIGHT (C) 2016-2019
**-----------File Info-----------------------------------------------------
* *File        name:     cpu_event_task.h
* *Last modified Date:   2017-08-01
* *Last SYS_VERSION:     0.0.0
* *Descriptions:            
**-------------------------------------------------------------------------
* *Created  by:          WJ
* *Created  date:        2017-08-01
* *SYS_VERSION:          0.0.0
* *Descriptions:         
**-------------------------------------------------------------------------
* *Modified by:				
* *Modified date:  			          
* *SYS_VERSION:                 
* *Descriptions:                        
***************************************************************************/
#ifndef _CPU_EVENT_TASK_H_
#define _CPU_EVENT_TASK_H_

#include "cpu_common.h"

//  低电提醒事件任务
void Cpu_EventLowPowerHint(cpu_task_controller_t *p_controller, gui_controller_t *p_gui);

//  待机充电事件任务
void Cpu_EventIdelCharge(cpu_task_controller_t *p_controller, gui_controller_t *p_gui);

//  函数功能:充电事件任务
void Cpu_EventAlarm(cpu_task_controller_t *p_controller, gui_controller_t *p_gui);

// 久坐提醒事件任务
void Cpu_EventSedentary(cpu_task_controller_t *p_controller, gui_controller_t *p_gui);

// 消息提醒事件任务
void Cpu_EventCallRemind(cpu_task_controller_t *p_controller, gui_controller_t *p_gui);

// 消息提醒事件任务
void Cpu_EventMessageRemind(cpu_task_controller_t *p_controller, gui_controller_t *p_gui);

// 心率告警事件任务
void Cpu_EventHeartAlert(cpu_task_controller_t *p_controller, gui_controller_t *p_gui);

// 拍照就绪事件任务
void Cpu_EventCameraReady(cpu_task_controller_t *p_controller, gui_controller_t *p_gui);

// 拍照OK事件任务
void Cpu_EventCameraOK(cpu_task_controller_t *p_controller, gui_controller_t *p_gui);

// 字库测试事件任务
void Cpu_EventFontTest(cpu_task_controller_t *p_controller, gui_controller_t *p_gui);

// 倒计时时间到了提醒任务
void Cpu_EvenCountDownTimeOut(cpu_task_controller_t *p_controller, gui_controller_t *p_gui);

//倒计时开始界面事件处理
void Cpu_EvenCountDownTimeStart(cpu_task_controller_t *p_controller, gui_controller_t *p_gui);

//运动达标界面事件处理
void Cpu_EvenStarget(cpu_task_controller_t *p_controller, gui_controller_t *p_gui);

//显示MAC地址和版本号
void Cpu_EvenDisMacVer(cpu_task_controller_t *p_controller, gui_controller_t *p_gui);

//女性功能提醒
void Cpu_EvenPhysRemind(cpu_task_controller_t *p_controller, gui_controller_t *p_gui);


void Cpu_EvenCartoon(cpu_task_controller_t *p_controller, gui_controller_t *p_gui);

void Cpu_EvenCartoon1(cpu_task_controller_t *p_controller, gui_controller_t *p_gui);

#endif
