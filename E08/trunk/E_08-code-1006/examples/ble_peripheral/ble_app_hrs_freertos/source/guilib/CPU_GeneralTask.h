#ifndef _CPU_GENERAL_TASK_H_
#define _CPU_GENERAL_TASK_H_

#include "cpu_task_data_type.h"

void Cpu_ScheduleTask(cpu_task_tcb_t* p_task, cpu_device_tcb_t* p_device, CPU_TASK_TYPE task_id);

void Cpu_ClearSechduleTask(cpu_task_tcb_t* p_task);

void Cpu_IdelToTimerTask(cpu_task_tcb_t* p_task, cpu_device_tcb_t* p_device);

void Cpu_IdelToRunTask(cpu_task_tcb_t* p_task, cpu_device_tcb_t* p_device);

void Cpu_MainTask(cpu_task_tcb_t* p_task, cpu_device_tcb_t* p_device);

void Cpu_ChooseMainTask(cpu_task_tcb_t* p_task, cpu_device_tcb_t* p_device);

void Cpu_MacAdd(cpu_task_tcb_t* p_task, cpu_device_tcb_t* p_device);

void Cpu_TaskRun(cpu_task_tcb_t* p_task, movement_cont_t* p_movement, cpu_device_tcb_t* p_device);

void Cpu_TaskRunSelect(cpu_task_tcb_t* p_task, cpu_device_tcb_t* p_device, movement_cont_t* p_movement);

void Cpu_TaskRunData(cpu_task_tcb_t* p_task, cpu_device_tcb_t* p_device, movement_cont_t* p_movement);

void Cpu_TaskRunDataResult(cpu_task_tcb_t* p_task, cpu_device_tcb_t* p_device, movement_cont_t* p_movement);

void Cpu_StepTask(cpu_task_tcb_t* p_task, cpu_device_tcb_t* p_device);

void Cpu_DistanTask(cpu_task_tcb_t* p_task, cpu_device_tcb_t* p_device);

void Cpu_CalorTask(cpu_task_tcb_t* p_task, cpu_device_tcb_t* p_device);

void Cpu_HeartTask(cpu_task_tcb_t* p_task, cpu_device_tcb_t* p_device);

void Cpu_SleepTask(cpu_task_tcb_t* p_task, cpu_device_tcb_t* p_device);

void Cpu_FindPhoneTask(cpu_task_tcb_t* p_task, cpu_device_tcb_t* p_device);

void Cpu_PowerOffTask(cpu_task_tcb_t* p_task, cpu_event_tcb_t *p_event, cpu_device_tcb_t* p_device);

void Cpu_TimerTask(cpu_task_tcb_t* p_task, watch_timer_t* p_timer, cpu_device_tcb_t* p_device);

void Cpu_HourglassTask(cpu_task_tcb_t* p_task, cpu_device_tcb_t* p_device);

void Cpu_TraverseInTask(cpu_task_tcb_t* p_task, CPU_TASK_TYPE task_type, uint8_t dis_time);

void Cpu_BpTask(cpu_task_tcb_t* p_task, cpu_device_tcb_t* p_device);

void Cpu_PttTask(cpu_task_tcb_t* p_task, cpu_device_tcb_t* p_device);

void Cpu_PttItemTask(cpu_task_tcb_t* p_task, cpu_device_tcb_t* p_device);

void Cpu_EcgTask(cpu_task_tcb_t* p_task, cpu_device_tcb_t* p_device);
void Cpu_EcgItem1Task(cpu_task_tcb_t* p_task, cpu_device_tcb_t* p_device);
void Cpu_EcgItem2Task(cpu_task_tcb_t* p_task, cpu_device_tcb_t* p_device);

void Cpu_OxygenTask(cpu_task_tcb_t* p_task, cpu_device_tcb_t* p_device);

//更多功能界面处理
void Cpu_FunctTask(cpu_task_tcb_t* p_task, cpu_device_tcb_t* p_device);

//更多功能界面处理
void Cpu_Funct2Task(cpu_task_tcb_t* p_task, cpu_device_tcb_t* p_device);

//开关设置界面处理
void Cpu_MessageSwitchTask(cpu_task_tcb_t* p_task, cpu_device_tcb_t* p_device);

//开关设置界面处理
void Cpu_SwitchTask(cpu_task_tcb_t* p_task, cpu_device_tcb_t* p_device);

//亮度调节界面
void Cpu_LightLevelSetTask(cpu_task_tcb_t* p_task, cpu_device_tcb_t* p_device);

//亮屏时间设置界面
void Cpu_LightTimeSetTask(cpu_task_tcb_t* p_task, cpu_device_tcb_t* p_device);

//清除数据界面
void Cpu_ClearDataTask(cpu_task_tcb_t* p_task, cpu_device_tcb_t* p_device);

//显示信息推送
void Cpu_DispMessage3Task(cpu_task_tcb_t* p_task, cpu_device_tcb_t* p_device);

//显示信息推送
void Cpu_DispMessageTask(cpu_task_tcb_t* p_task, cpu_device_tcb_t* p_device);

//显示信息推送2
void Cpu_DispMessage2Task(cpu_task_tcb_t* p_task, cpu_device_tcb_t* p_device);

#endif
