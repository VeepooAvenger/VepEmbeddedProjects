#ifndef _EVENT_PRIORITY_H_
#define _EVENT_PRIORITY_H_

#include "cpu_task_data_type.h"

#define EVENT_ATTR_NODE_MAX_CNT         10

// 事件优先级枚举类型
typedef enum
{
    EVENT_PRIORITY_NULL = 0,
    CHARGE_EVENT_PRIORITY = 1,
    CALL_HINT_PRIORITY = 2,
    BLE_DISC_EVENT_PRIORITY = 3,
	HOURGLASS_HINT_EVENT_PRIORITY = 4,
    PHYS_HINT_EVENT_PRIORITY = 5,
	SPORT_TARGET_ALERT_PRIORITY = 6,
    HEART_ALERT_EVENT_PRIORITY = 7,	
    LOW_BO_ALERT_EVENT_PRIORITY = 8,	
    ALARM_HINT_EVENT_PRIORITY = 9,
    MESSAGE_HINT_EVENT_PRIORITY = 10,
    SENDETRY_HINT_EVENT_PRIORITY = 11,
    CAMERAY_EVENT_PRIORITY = 12,
    EVENT_PRIORITY_MAX_ID,
}EVENT_PRIORITY_ID;

typedef struct
{
    uint8_t            count;                                // 事件数量
    CPU_EVENT_TYPE     max_priority_ready_event;             // 最大就绪优先级事件
    cpu_event_tcb_t    event_tcb[EVENT_ATTR_NODE_MAX_CNT];   // 时间任务块参数
}event_queue_t;

#endif
