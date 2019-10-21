#ifndef _CPU_GENERAL_TASK_H_
#define _CPU_GENERAL_TASK_H_

#include "cpu_task_data_type.h"
#include "gui_page_display.h"

void Cpu_ScheduleTask(cpu_task_tcb_t* p_task, cpu_device_tcb_t* p_device, CPU_TASK_TYPE task_id);

void Cpu_ClearSechduleTask(cpu_task_tcb_t* p_task);

void Cpu_IdelToTimerTask(cpu_task_tcb_t* p_task, cpu_device_tcb_t* p_device);

void Cpu_IdelToRunTask(cpu_task_tcb_t* p_task, cpu_device_tcb_t* p_device);

void Cpu_MainTask(cpu_task_tcb_t* p_task, cpu_device_tcb_t* p_device);

void Cpu_MacAdd(cpu_task_tcb_t* p_task, cpu_device_tcb_t* p_device);

void Cpu_TaskRun(cpu_task_tcb_t* p_task, movement_cont_t* p_movement, cpu_device_tcb_t* p_device);

void Cpu_StepTask(cpu_task_tcb_t* p_task, cpu_device_tcb_t* p_device);

void Cpu_DistanTask(cpu_task_tcb_t* p_task, cpu_device_tcb_t* p_device);

void Cpu_CalorTask(cpu_task_tcb_t* p_task, cpu_device_tcb_t* p_device);

void Cpu_DrinkTask(cpu_task_tcb_t* p_task, cpu_device_tcb_t* p_device);
	
void Cpu_OxygenTask(cpu_task_tcb_t* p_task, cpu_device_tcb_t* p_device);

void Cpu_HeartTask(cpu_task_tcb_t* p_task, cpu_device_tcb_t* p_device);

void Cpu_SleepTask(cpu_task_tcb_t* p_task, cpu_device_tcb_t* p_device);

void Cpu_FindPhoneTask(cpu_task_tcb_t* p_task, cpu_device_tcb_t* p_device);

void Cpu_PowerOffTask(cpu_task_tcb_t* p_task, cpu_event_tcb_t *p_event, cpu_device_tcb_t* p_device);

void Cpu_TimerTask(cpu_task_tcb_t* p_task, watch_timer_t* p_timer, cpu_device_tcb_t* p_device);

void Cpu_HourglassTask(cpu_task_tcb_t* p_task, cpu_device_tcb_t* p_device);

void Cpu_TraverseInTask(cpu_task_tcb_t* p_task, CPU_TASK_TYPE task_type, uint8_t dis_time);

void Cpu_BootUpTask(cpu_task_tcb_t* p_task, cpu_event_tcb_t *p_event, cpu_device_tcb_t* p_device);

void Cpu_BpTask(cpu_task_tcb_t* p_task, cpu_device_tcb_t* p_device);

#endif
