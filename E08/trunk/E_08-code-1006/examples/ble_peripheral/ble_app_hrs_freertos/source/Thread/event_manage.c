#include <string.h>
#include "event_manage.h"
#include "data_manage.h"

static EVENT_PRIORITY_ID Get_EventPriority(CPU_EVENT_TYPE event)
{
    EVENT_PRIORITY_ID  event_priority_id = EVENT_PRIORITY_NULL;
	
    switch(event)
    {
        case CPU_EVENT_TASK_CHARGE:
            event_priority_id = CHARGE_EVENT_PRIORITY;
            break;
        case CPU_EVENT_TASK_CALL:
            event_priority_id = CALL_HINT_PRIORITY;
            break;
        case CPU_EVENT_TASK_ALARM:
            event_priority_id = ALARM_HINT_EVENT_PRIORITY;
            break;
        case CPU_EVENT_TASK_SIT:
            event_priority_id = SENDETRY_HINT_EVENT_PRIORITY;		 
            break;
//        case CPU_EVENT_TASK_PHYS_REMIND:
//            event_priority_id = PHYS_HINT_EVENT_PRIORITY;		 
//            break;
        case CPU_EVENT_TASK_MESSAGE:
            event_priority_id = MESSAGE_HINT_EVENT_PRIORITY;		 
            break;
        case CPU_EVENT_TASK_HEARTALERT:
            event_priority_id = HEART_ALERT_EVENT_PRIORITY;		 
            break;	
        case CPU_EVENT_TASK_LOW_SPO2:
            event_priority_id = LOW_BO_ALERT_EVENT_PRIORITY;		 
            break;	
		case CPU_EVENT_TASK_SPORT_TARGET:
		    event_priority_id = SPORT_TARGET_ALERT_PRIORITY; 
		    break;
        case CPU_EVENT_TASK_HOURGLASS_HINT:        
        case CPU_EVENT_TASK_HOURGLASS_TIMEOUT:
            event_priority_id = HOURGLASS_HINT_EVENT_PRIORITY;		 
            break;
        case CPU_EVENT_TASK_CAMERA:
            event_priority_id = CAMERAY_EVENT_PRIORITY;
            break;
        case CPU_EVENT_TASK_BLE_DISC_REMIND:
            event_priority_id = BLE_DISC_EVENT_PRIORITY;
            break;
        default:
            event_priority_id = EVENT_PRIORITY_NULL;
            break;
    }
    
    return event_priority_id;
}

static void EventManage_ScanEventCount(void)
{    
    p_data_manage->event_list.count = 0;
    for(uint8_t index = 0; index < EVENT_ATTR_NODE_MAX_CNT; index++)
    {
        if(p_data_manage->event_list.event_tcb[index].event_id != CPU_EVENT_TASK_IDEL)
        {
            p_data_manage->event_list.count++;  		 
        }
    }	
}

static void EventManage_ResetMaxPriorityEvent(void)
{
    EVENT_PRIORITY_ID  max_event_priority = EVENT_PRIORITY_MAX_ID;
    EVENT_PRIORITY_ID  current_event_priority = EVENT_PRIORITY_NULL;
    CPU_EVENT_TYPE  current_event = CPU_EVENT_TASK_IDEL;
    
    // 遍历链表,查找最大优先级事件
    p_data_manage->event_list.max_priority_ready_event = CPU_EVENT_TASK_IDEL;
    for(uint8_t index = 0; index < EVENT_ATTR_NODE_MAX_CNT; index++)
    {
        current_event = p_data_manage->event_list.event_tcb[index].event_id;
        current_event_priority = Get_EventPriority(current_event);
        
        if((max_event_priority > current_event_priority)&&(current_event_priority != EVENT_PRIORITY_NULL))
        {
            max_event_priority = current_event_priority;
            p_data_manage->event_list.max_priority_ready_event = current_event;
        }
       
    }
}

void EventManage_CollectNewEvent(CPU_EVENT_TYPE  event)
{
    // 1.遍历当前事件列表,如果事件列表已经存在,则直接返回
    for(uint8_t index = 0; index < EVENT_ATTR_NODE_MAX_CNT; index++)
    {
        if(p_data_manage->event_list.event_tcb[index].event_id == event)
        {
            return;  		 
        }		
    }
    
    // 2.插入新的事件
    for(uint8_t index = 0; index < EVENT_ATTR_NODE_MAX_CNT; index++)
    {
        if(p_data_manage->event_list.event_tcb[index].event_id == CPU_EVENT_TASK_IDEL)
        {
            // 并且将新的事件参数初始化
            memset(&p_data_manage->event_list.event_tcb[index], 0, sizeof(cpu_event_tcb_t));
            p_data_manage->event_list.event_tcb[index].task_controller.init = 1;
            p_data_manage->event_list.event_tcb[index].event_id = event;
            break; 		 
        }
    }

    EventManage_ScanEventCount();
    EventManage_ResetMaxPriorityEvent();
}

void EventManage_ClearEvent(CPU_EVENT_TYPE  event)
{
    for(uint8_t index = 0; index < EVENT_ATTR_NODE_MAX_CNT; index++)
    {
        if(p_data_manage->event_list.event_tcb[index].event_id == event)
        {
            // 并且将新的事件参数初始化
            memset(&p_data_manage->event_list.event_tcb[index], 0, sizeof(cpu_event_tcb_t));
        }
    }
    
    EventManage_ScanEventCount();
    EventManage_ResetMaxPriorityEvent();
}

bool EventManage_IsHigerPriorityEvent(CPU_EVENT_TYPE  current_event)
{
    EventManage_ResetMaxPriorityEvent();
    
    return (Get_EventPriority(p_data_manage->event_list.max_priority_ready_event) <= Get_EventPriority(current_event))||
           (current_event == CPU_EVENT_TASK_IDEL);
}

bool EventManage_PopMaxPriorityEvent(cpu_event_tcb_t* p_event_tcb)
{
    int8_t  max_index = 0;
    EVENT_PRIORITY_ID  max_event_priority = EVENT_PRIORITY_MAX_ID;
    EVENT_PRIORITY_ID  current_event_priority = EVENT_PRIORITY_NULL;
    CPU_EVENT_TYPE  current_event = CPU_EVENT_TASK_IDEL;
    
    // 遍历链表,查找最大优先级事件
    for(uint8_t index = 0; index < EVENT_ATTR_NODE_MAX_CNT; index++)
    {
        current_event = p_data_manage->event_list.event_tcb[index].event_id;
        current_event_priority = Get_EventPriority(current_event);
        if((max_event_priority > current_event_priority)&&(current_event_priority != EVENT_PRIORITY_NULL))
        {
            max_index = index;
            max_event_priority = current_event_priority;
        }
    }
    
    // 如果事件查找成功,则需要取出事件
    if(max_event_priority != EVENT_PRIORITY_MAX_ID)
    {
        // 取出当前最大优先级事件
        memcpy(p_event_tcb, &p_data_manage->event_list.event_tcb[max_index], sizeof(cpu_event_tcb_t)); 
        memset(&p_data_manage->event_list.event_tcb[max_index], 0, sizeof(cpu_event_tcb_t));
    }
    
    EventManage_ScanEventCount();
    EventManage_ResetMaxPriorityEvent();
    
    return (max_event_priority != EVENT_PRIORITY_MAX_ID);
}

void EventManage_PushEventTcb(const cpu_event_tcb_t *p_tcb)
{
    for(uint8_t index = 0; index < EVENT_ATTR_NODE_MAX_CNT; index++)
    {
        if(p_data_manage->event_list.event_tcb[index].event_id == CPU_EVENT_TASK_IDEL)
        {
            memcpy(&p_data_manage->event_list.event_tcb[index], p_tcb, sizeof(cpu_event_tcb_t));
            break;
        }
    }
    
    EventManage_ScanEventCount();
    EventManage_ResetMaxPriorityEvent();
}

void EventManage_ClearEventList(void)
{
    memset(&p_data_manage->event_list,0,sizeof(event_queue_t));
}

