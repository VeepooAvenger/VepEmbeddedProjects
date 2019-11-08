/***************************Copyright(c)***********************************
**                       Veepoo Tech
**                  COPYRIGHT (C) 2016-2019
**-----------File Info-----------------------------------------------------
** File        name:     phone_notify_roll_event.h
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
#ifndef _PHONE_NOTIFY_ROLL_EVENT_H_
#define _PHONE_NOTIFY_ROLL_EVENT_H_

#include <stdint.h>
#include <stdbool.h>
#include "phone_notify_event.h"

typedef enum
{
    MSG_TYPE_NULL = 0,
    MSG_TYPE_CALL = 1,
    MSG_TYPE_MESSAGE,
    MSG_TYPE_WECHAT,
    MSG_TYPE_QQ,
    MSG_TYPE_FACEBOOK,
    MSG_TYPE_TWITTER,
    MSG_TYPE_WHATSAPP,
    MSG_TYPE_LINKEDIN,
    MSG_TYPE_SNAPCHAT,
    MSG_TYPE_SKYPE,
    MSG_TYPE_INSTAGRAM,
    MSG_TYPE_LINE,
    MSG_TYPE_GMAIL,
    MSG_TYPE_OTHER
}msg_type_e;

#pragma pack(1)
typedef struct
{
    uint16_t title_len;
    uint8_t *title;
    uint16_t content_len;
    uint8_t *content;
}show_msg_t;

/* δ����Ϣ�ṹ�� */
typedef struct
{
		uint8_t enable:1;  
		uint8_t single_page_hint:1;     //��������
		uint8_t roll_mul_page_hint:1;   //������������
		uint8_t reading:1;
		uint8_t resv:4;
	
		uint16_t title_read_index;      //title������
		uint16_t content_read_index;    //���ݶ�����
	
		uint16_t current_page_index;    //��ǰҳ����
		uint16_t total_page_count;      //��ҳ��	
	        
        msg_type_e msg_type;            //��Ϣ����
        notify_content_t msg;        
        uint16_t read_page_len[10];     //����ÿһҳ����ʼ��ַ
        show_msg_t show_msg;
}unread_message_t;

#pragma pack()

extern unread_message_t  unread_message_s;

//�Ƿ����δ��������Ϣ
bool Message_ExistUnread(void);

//�Ƿ����ڶ�ȡ������Ϣ
bool Message_IsReading(void);

//�����Ϣ
void Message_Clear(void);

//������һҳδ����Ϣ
uint8_t Message_NextUnreadMessageHint(uint8_t reading_flag);

//����δ����Ϣ
void Message_Mark(msg_type_e type);

#endif
