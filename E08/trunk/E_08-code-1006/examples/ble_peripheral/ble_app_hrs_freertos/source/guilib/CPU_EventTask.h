#ifndef _CPU_EVENT_TASK_H_
#define _CPU_EVENT_TASK_H_

#include "cpu_task_data_type.h"

// 调度事件任务
void Cpu_ScheduleEvent(cpu_event_tcb_t* p_event, cpu_device_tcb_t* p_device, CPU_EVENT_TYPE event_id);

// 清除当前事件
void Cpu_ClearCurrentEvent(cpu_event_tcb_t* p_event, cpu_device_tcb_t* p_device);

//  低电提醒事件任务
void Cpu_LowPowerHintEvent(cpu_task_tcb_t* p_task, cpu_event_tcb_t* p_event, cpu_device_tcb_t* p_device);

//  翻腕启动事件任务
void Cpu_TurnwristStartEvent(cpu_task_tcb_t* p_task, cpu_event_tcb_t* p_event, movement_cont_t *p_movement, watch_timer_t *p_timer, cpu_device_tcb_t* p_device);

//  翻腕停止事件任务
void Cpu_TurnwristStopEvent(cpu_task_tcb_t* p_task, cpu_event_tcb_t* p_event, watch_timer_t *p_timer, cpu_device_tcb_t* p_device);

//  待机充电事件任务
void Cpu_IdelChargeEvent(cpu_task_tcb_t* p_task, cpu_event_tcb_t* p_event, cpu_device_tcb_t* p_device);

//  闹钟提醒事件任务
void Cpu_AlarmHintEvent(cpu_event_tcb_t* p_event, cpu_device_tcb_t* p_device);

// 久坐提醒事件任务
void Cpu_SedentaryHintEvent(cpu_event_tcb_t* p_event, cpu_device_tcb_t* p_device);

// 电话提醒事件任务
void Cpu_CallHintEvent(cpu_event_tcb_t* p_event, cpu_device_tcb_t* p_device);

//女性项目提醒
//void Cpu_PhysRemindEvent(cpu_task_tcb_t* p_task, cpu_event_tcb_t* p_event, watch_timer_t *p_timer, movement_cont_t *p_movement,cpu_device_tcb_t* p_device);
    
// 消息提醒事件任务
//void Cpu_MessageHintEvent(cpu_task_tcb_t* p_task, cpu_event_tcb_t* p_event, watch_timer_t *p_timer,cpu_device_tcb_t* p_device);
void Cpu_MessageHintEvent(cpu_task_tcb_t* p_task, cpu_event_tcb_t* p_event, watch_timer_t *p_timer, movement_cont_t *p_movement,cpu_device_tcb_t* p_device);

// 心率告警事件任务
void Cpu_HeartAlertHintEvent(cpu_event_tcb_t* p_event, cpu_device_tcb_t* p_device);

// 低氧告警事件任务
void Cpu_LowOxygenAlertHintEvent(cpu_event_tcb_t* p_event, cpu_device_tcb_t* p_device);

// 拍照就绪事件任务
void Cpu_CameraEvent(cpu_event_tcb_t* p_event, cpu_device_tcb_t* p_device);

// 字库测试事件任务
void Cpu_FontTestEvent(cpu_task_tcb_t* p_task, cpu_event_tcb_t* p_event, cpu_device_tcb_t* p_device);

// 倒计时App设置提醒任务
void Cpu_HourglassAppHintEvent(cpu_event_tcb_t* p_event, cpu_device_tcb_t* p_device);

// 倒计时时间到了提醒任务
void Cpu_HourglassTimeOutEvent(cpu_event_tcb_t* p_event, cpu_device_tcb_t* p_device);

// 蓝牙断连任务
void Cpu_BleDiscRemindEvent(cpu_event_tcb_t* p_event, cpu_device_tcb_t* p_device);

// 设备关机事件(不要放到按键中断中,会反应慢,原因未知)
void Cpu_DevicePowerOnEvent(cpu_task_tcb_t* p_task, cpu_event_tcb_t* p_event, cpu_device_tcb_t* p_device);

// 设备关机事件(不要放到按键中断中,会反应慢,原因未知)
void Cpu_DevicePowerOffEvent(cpu_task_tcb_t* p_task, cpu_event_tcb_t* p_event, cpu_device_tcb_t* p_device);

void Cpu_DeviceSportTargetEvent(cpu_task_tcb_t* p_task, cpu_event_tcb_t* p_event, cpu_device_tcb_t* p_device);

#endif
