#ifndef _EVENT_MANAGE_H_
#define _EVENT_MANAGE_H_

#include "cpu_task_data_type.h"
#include "event_priority.h"

// 接收新的事件
void EventManage_CollectNewEvent(CPU_EVENT_TYPE  event);

// 清除事件列表中事件
void EventManage_ClearEvent(CPU_EVENT_TYPE  event);

// 获取最大就绪优先级事件
bool EventManage_IsHigerPriorityEvent(CPU_EVENT_TYPE  current_event);

// 最最大优先级参数弹出
bool EventManage_PopMaxPriorityEvent(cpu_event_tcb_t* p_event_tcb);

// 将事件压入事件队列
void EventManage_PushEventTcb(const cpu_event_tcb_t *p_tcb);

// 清空事件列表中所有事件
void EventManage_ClearEventList(void);

#endif
