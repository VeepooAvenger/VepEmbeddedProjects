#ifndef _EVENT_MANAGE_H_
#define _EVENT_MANAGE_H_

#include "cpu_task_data_type.h"
#include "event_priority.h"

// �����µ��¼�
void EventManage_CollectNewEvent(CPU_EVENT_TYPE  event);

// ����¼��б����¼�
void EventManage_ClearEvent(CPU_EVENT_TYPE  event);

// ��ȡ���������ȼ��¼�
bool EventManage_IsHigerPriorityEvent(CPU_EVENT_TYPE  current_event);

// ��������ȼ���������
bool EventManage_PopMaxPriorityEvent(cpu_event_tcb_t* p_event_tcb);

// ���¼�ѹ���¼�����
void EventManage_PushEventTcb(const cpu_event_tcb_t *p_tcb);

// ����¼��б��������¼�
void EventManage_ClearEventList(void);

#endif
