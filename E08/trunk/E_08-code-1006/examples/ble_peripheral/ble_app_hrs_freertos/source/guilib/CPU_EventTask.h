#ifndef _CPU_EVENT_TASK_H_
#define _CPU_EVENT_TASK_H_

#include "cpu_task_data_type.h"

// �����¼�����
void Cpu_ScheduleEvent(cpu_event_tcb_t* p_event, cpu_device_tcb_t* p_device, CPU_EVENT_TYPE event_id);

// �����ǰ�¼�
void Cpu_ClearCurrentEvent(cpu_event_tcb_t* p_event, cpu_device_tcb_t* p_device);

//  �͵������¼�����
void Cpu_LowPowerHintEvent(cpu_task_tcb_t* p_task, cpu_event_tcb_t* p_event, cpu_device_tcb_t* p_device);

//  ���������¼�����
void Cpu_TurnwristStartEvent(cpu_task_tcb_t* p_task, cpu_event_tcb_t* p_event, movement_cont_t *p_movement, watch_timer_t *p_timer, cpu_device_tcb_t* p_device);

//  ����ֹͣ�¼�����
void Cpu_TurnwristStopEvent(cpu_task_tcb_t* p_task, cpu_event_tcb_t* p_event, watch_timer_t *p_timer, cpu_device_tcb_t* p_device);

//  ��������¼�����
void Cpu_IdelChargeEvent(cpu_task_tcb_t* p_task, cpu_event_tcb_t* p_event, cpu_device_tcb_t* p_device);

//  ���������¼�����
void Cpu_AlarmHintEvent(cpu_event_tcb_t* p_event, cpu_device_tcb_t* p_device);

// ���������¼�����
void Cpu_SedentaryHintEvent(cpu_event_tcb_t* p_event, cpu_device_tcb_t* p_device);

// �绰�����¼�����
void Cpu_CallHintEvent(cpu_event_tcb_t* p_event, cpu_device_tcb_t* p_device);

//Ů����Ŀ����
//void Cpu_PhysRemindEvent(cpu_task_tcb_t* p_task, cpu_event_tcb_t* p_event, watch_timer_t *p_timer, movement_cont_t *p_movement,cpu_device_tcb_t* p_device);
    
// ��Ϣ�����¼�����
//void Cpu_MessageHintEvent(cpu_task_tcb_t* p_task, cpu_event_tcb_t* p_event, watch_timer_t *p_timer,cpu_device_tcb_t* p_device);
void Cpu_MessageHintEvent(cpu_task_tcb_t* p_task, cpu_event_tcb_t* p_event, watch_timer_t *p_timer, movement_cont_t *p_movement,cpu_device_tcb_t* p_device);

// ���ʸ澯�¼�����
void Cpu_HeartAlertHintEvent(cpu_event_tcb_t* p_event, cpu_device_tcb_t* p_device);

// �����澯�¼�����
void Cpu_LowOxygenAlertHintEvent(cpu_event_tcb_t* p_event, cpu_device_tcb_t* p_device);

// ���վ����¼�����
void Cpu_CameraEvent(cpu_event_tcb_t* p_event, cpu_device_tcb_t* p_device);

// �ֿ�����¼�����
void Cpu_FontTestEvent(cpu_task_tcb_t* p_task, cpu_event_tcb_t* p_event, cpu_device_tcb_t* p_device);

// ����ʱApp������������
void Cpu_HourglassAppHintEvent(cpu_event_tcb_t* p_event, cpu_device_tcb_t* p_device);

// ����ʱʱ�䵽����������
void Cpu_HourglassTimeOutEvent(cpu_event_tcb_t* p_event, cpu_device_tcb_t* p_device);

// ������������
void Cpu_BleDiscRemindEvent(cpu_event_tcb_t* p_event, cpu_device_tcb_t* p_device);

// �豸�ػ��¼�(��Ҫ�ŵ������ж���,�ᷴӦ��,ԭ��δ֪)
void Cpu_DevicePowerOnEvent(cpu_task_tcb_t* p_task, cpu_event_tcb_t* p_event, cpu_device_tcb_t* p_device);

// �豸�ػ��¼�(��Ҫ�ŵ������ж���,�ᷴӦ��,ԭ��δ֪)
void Cpu_DevicePowerOffEvent(cpu_task_tcb_t* p_task, cpu_event_tcb_t* p_event, cpu_device_tcb_t* p_device);

void Cpu_DeviceSportTargetEvent(cpu_task_tcb_t* p_task, cpu_event_tcb_t* p_event, cpu_device_tcb_t* p_device);

#endif
