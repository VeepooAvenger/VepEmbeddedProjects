/***************************Copyright(c)***********************************
**                       Veepoo Tech
**                  COPYRIGHT (C) 2016-2019
**-----------File Info-----------------------------------------------------
** File        name:     phone_notify_event.h
** Last modified Date:   2017-08-24
** Last SYS_VERSION:     0.0.0
** Descriptions:            
**-------------------------------------------------------------------------
** Created  by:          WJ
** Created  date:        2017-08-24
** SYS_VERSION:          0.0.0
** Descriptions:         
**-------------------------------------------------------------------------
** Modified by:				
** Modified date:  			          
** SYS_VERSION:                 
** Descriptions:                        
***************************************************************************/
#ifndef _PHONE_NOTIFY_EVENT_H_
#define _PHONE_NOTIFY_EVENT_H_

#include <stdint.h>
#include <stdbool.h>
#include "ancs_notify_event.h"

#define TLTLE_DATA_MAX_LEN      ATTR_DATA_SIZE//32
#define CONTENT_DATA_MAX_LEN    ATTR_MESSAGE_DATA_SIZE//220//128

#define PHONE_CALL_IDLE             0   //就绪
#define PHONE_CALL_INCOMING         1   //来电
#define PHONE_CALL_MUTE             2   //静音
#define PHONE_CALL_READYREJECT      4   //拒接中
#define PHONE_CALL_REJECTSUC        3   //拒接完成

#pragma pack(1)
typedef struct
{
		uint8_t recv_done:1;
		uint8_t resv:7;
}notify_control_t;

typedef struct
{
    uint8_t call:1;
    uint8_t message:1;
    uint8_t wechat:1;
    uint8_t qq:1;
    
    uint8_t facebook:1;
    uint8_t twitter:1;
    uint8_t whatsapp:1;   
    uint8_t linkedin:1;
    
    uint8_t line:1;
    uint8_t instagram:1;
    uint8_t snapchat:1;
    uint8_t skype:1;
    
    uint8_t gmail:1;
    uint8_t other:1;
    uint8_t resv:2;
}notify_hint_t;

typedef struct
{
    uint16_t title_len;
    uint8_t title[TLTLE_DATA_MAX_LEN];
    uint16_t content_len;
    uint8_t content[CONTENT_DATA_MAX_LEN];
    uint16_t package_index;
}notify_content_t;

typedef struct
{
		uint8_t ext_phone_event;
		notify_control_t  control;
		notify_hint_t     hint;
		notify_content_t  msg;
}phone_notify_event_t;

#pragma pack()

extern phone_notify_event_t phone_notify_s;

//设置来电状态
void Phone_NotifySetCallingState(uint8_t state);

//获取来电状态
uint8_t Phone_NotifyGetCallingState(void);

//消息控制初始化
void Phone_NotifyInit(void);

//消息处理
void Phone_NotifyCore(void);

//判断开关状态，消息是否能提醒
uint8_t Phone_NotifyIsCanRemind(void);

#endif
