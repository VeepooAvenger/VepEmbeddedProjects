/***************************Copyright(c)***********************************
**                       Veepoo Tech
**                  COPYRIGHT (C) 2016-2019
**-----------File Info-----------------------------------------------------
** File        name:     cpu_general_task.h
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
#ifndef _CPU_GENERAL_TASK_H_
#define _CPU_GENERAL_TASK_H_

#include "cpu_common.h"

//页面跳转
void Cpu_TaskSchedule(cpu_task_controller_t *p_controller, gui_controller_t *p_gui, CPU_TASK_TYPE task_id);

//重置刷新间隔计数器
void Cpu_TaskClearIntervalCnt(void);

//主界面处理
void Cpu_TaskMain(cpu_task_controller_t *p_controller, gui_controller_t *p_gui);

//计步界面处理
void Cpu_TaskStep(cpu_task_controller_t *p_controller, gui_controller_t *p_gui);

//距离界面处理
void Cpu_TaskDistance(cpu_task_controller_t *p_controller, gui_controller_t *p_gui);

//卡路里界面处理
void Cpu_TaskCalories(cpu_task_controller_t *p_controller, gui_controller_t *p_gui);

//运动界面处理
void Cpu_TaskRun(cpu_task_controller_t *p_controller, gui_controller_t *p_gui);

//运动模式操作界面
void Cpu_TaskRunButton(cpu_task_controller_t *p_controller, gui_controller_t *p_gui);

//运动模式数据界面
void Cpu_TaskRunData(cpu_task_controller_t *p_controller, gui_controller_t *p_gui);

//血压界面处理
void Cpu_TaskBP(cpu_task_controller_t *p_controller, gui_controller_t *p_gui);

//心率界面处理
void Cpu_TaskHeart(cpu_task_controller_t *p_controller, gui_controller_t *p_gui);

//睡眠界面处理
void Cpu_TaskSleep(cpu_task_controller_t *p_controller, gui_controller_t *p_gui);

//找手机界面处理
void Cpu_TaskFindPhone(cpu_task_controller_t *p_controller, gui_controller_t *p_gui);
// 带图片确认的关机界面
void Cpu_TaskTurnOff(cpu_task_controller_t *p_controller, gui_controller_t *p_gui);
//关机界面处理
void Cpu_TaskPowerOff(cpu_task_controller_t *p_controller, gui_controller_t *p_gui);
//开机界面处理
void Cpu_TaskPowerOn(cpu_task_controller_t *p_controller, gui_controller_t *p_gui);

//秒表界面处理
void Cpu_TaskStopWatch(cpu_task_controller_t *p_controller, gui_controller_t *p_gui);

//倒计时界面处理
void Gui_TaskCountDown(cpu_task_controller_t *p_controller, gui_controller_t *p_gui);

// 运动模式暂停
void Cpu_TaskRunModePause(cpu_task_controller_t *p_controller, gui_controller_t *p_gui);

// 运动模式退出
void Cpu_TaskRunModeExit(cpu_task_controller_t *p_controller, gui_controller_t *p_gui);

//MAC and VERSION
void Cpu_TaskDisMacVer(cpu_task_controller_t *p_controller, gui_controller_t *p_gui);


#endif
