/***************************Copyright(c)***********************************
**                       Veepoo Tech
**                  COPYRIGHT (C) 2016-2019
**-----------File Info-----------------------------------------------------
** File        name:     phone_notify_event.c
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
#include <string.h>
#include "phone_notify_event.h"
#include "app_type.h"
#include "app_task.h"
#include "gui_page_display.h"
#include "phone_notify_roll_event.h"
#include "gui_page_display.h"

phone_notify_event_t  phone_notify_s ;
uint8_t phone_calling_state = 0;   //����״̬

extern system_info_t system_info_s;
extern system_flag_t system_flag_s;

/****************************************************************
** Function name:           Phone_NotifySetCallingState
** Descriptions:            ����״̬����
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-08-24
*****************************************************************/ 
void Phone_NotifySetCallingState(uint8_t state)
{
    phone_calling_state = state;
}

/****************************************************************
** Function name:           Phone_NotifyGetCallingState
** Descriptions:            ��ȡ����״̬
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-08-24
*****************************************************************/ 
uint8_t Phone_NotifyGetCallingState(void)
{
    return phone_calling_state;
}

/****************************************************************
** Function name:           Phone_NotifyInit
** Descriptions:            ��Ϣ���Ƴ�ʼ��
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-08-24
*****************************************************************/ 
void Phone_NotifyInit(void)
{
    memset(&phone_notify_s, 0, sizeof(phone_notify_s));
}

/****************************************************************
** Function name:           Phone_NotifyCore
** Descriptions:            ��Ϣ����
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-08-24
*****************************************************************/ 
void Phone_NotifyCore(void)
{
    const message_enable_t *message_p = &(system_info_s.message_enable);
    
    if(phone_notify_s.control.recv_done == 0)
    {
        return;
    }
       
    phone_notify_s.control.recv_done = 0;
            
    if((message_p->call_enable == 0x01) && (phone_notify_s.hint.call == 0x01))
    {
        /* ������Ϣ������ʾ */
        phone_notify_s.hint.call = 0;
        Phone_NotifySetCallingState(PHONE_CALL_INCOMING);//������
        //���ڲ鿴��Ϣ�����ǰ��Ϣ����ʾ����
        if(Message_IsReading()) 
        {
            Message_Clear();       
        }
        Message_Mark(MSG_TYPE_CALL);
        Motor_AddEvent(20, 1, 1 * 20, 2 * 20);//����20�� �ǳ����� ��1�� ���2��            
        return;
    }   
    if(Message_IsReading())
    {
        return;
    }
    if((message_p->sms_enable == 0x01) && (phone_notify_s.hint.message == 0x01))
    {
        /* ������Ϣ������ʾ */
        phone_notify_s.hint.message = 0;               
        Message_Mark(MSG_TYPE_MESSAGE);
    }
    else if((message_p->wechat_enable == 0x01) && (phone_notify_s.hint.wechat == 0x01))
    {
        /* ΢����Ϣ������ʾ */
        phone_notify_s.hint.wechat = 0;
        
        if(phone_notify_s.msg.content_len > 1  &&  phone_notify_s.msg.content[0] == 0x20)//WJ.20170517.���΢����Ϣǰ��Ŀո�
        {
           phone_notify_s.msg.content_len = phone_notify_s.msg.content_len - 1;
           memcpy(phone_notify_s.msg.content, phone_notify_s.msg.content+1, phone_notify_s.msg.content_len); 
        }       
        Message_Mark(MSG_TYPE_WECHAT);
    }
    else if((message_p->qq_enable == 0x01) && (phone_notify_s.hint.qq == 0x01))
    {   
        /* QQ��Ϣ������ʾ */
        phone_notify_s.hint.qq = 0;    
        Message_Mark(MSG_TYPE_QQ);
    }
    else if((message_p->facebook_enable == 0x01) && (phone_notify_s.hint.facebook == 0x01))
    {   
        /* Facebook��Ϣ���� */
        phone_notify_s.hint.facebook = 0;   
        Message_Mark(MSG_TYPE_FACEBOOK);
    }    
    else if((message_p->twitter_enable == 0x01) && (phone_notify_s.hint.twitter == 0x01))
    {
        /* twitter��Ϣ���� */
        phone_notify_s.hint.twitter = 0;
        Message_Mark(MSG_TYPE_TWITTER);
    }
    else if((message_p->whatsapp_enable == 0x01) && (phone_notify_s.hint.whatsapp == 0x01))
    {
        /* whatsapp��Ϣ���� */
        phone_notify_s.hint.whatsapp = 0;    
        Message_Mark(MSG_TYPE_WHATSAPP);
    }    
    else if((message_p->linkedin_enable == 0x01) && (phone_notify_s.hint.linkedin == 0x01))
    {
        /* linkedin��Ϣ���� */
        phone_notify_s.hint.linkedin = 0;   
        Message_Mark(MSG_TYPE_LINKEDIN);
    }   
    else if((message_p->snapchat_enable == 0x01) && (phone_notify_s.hint.snapchat == 0x01))
    {
        /* snapchat��Ϣ���� */
        phone_notify_s.hint.snapchat = 0;    
        Message_Mark(MSG_TYPE_SNAPCHAT);
    }
    else if((message_p->skype_enable == 0x01) && (phone_notify_s.hint.skype == 0x01))
    {
        /* skype��Ϣ���� */
        phone_notify_s.hint.skype = 0;   
        Message_Mark(MSG_TYPE_SKYPE);
    }
    else if((message_p->instagram_enable == 0x01) && (phone_notify_s.hint.instagram == 0x01))
    {
        /* instagram��Ϣ���� */
        phone_notify_s.hint.instagram = 0;    
        Message_Mark(MSG_TYPE_INSTAGRAM);
    }
    else if((message_p->line_enable == 0x01) && (phone_notify_s.hint.line == 0x01))
    {
        /* line��Ϣ���� */
        phone_notify_s.hint.line = 0;    
        Message_Mark(MSG_TYPE_LINE);
    }    
    else if((message_p->gmail_enable == 0x01) && (phone_notify_s.hint.gmail == 0x01))
    {   
        /* gmail��Ϣ���� */
        phone_notify_s.hint.gmail = 0; 
        Message_Mark(MSG_TYPE_GMAIL);
    }
    else if((message_p->other_enable == 0x01) && (phone_notify_s.hint.other == 0x01))
    {   
        /* other��Ϣ���� */
        phone_notify_s.hint.other = 0;    
        Message_Mark(MSG_TYPE_OTHER);
    } 
    else
    {
        return;
    }    
    Motor_AddEvent(1, 0, 2 * 20, 0);//����1�� �ǳ����� ��1�� 
}


/****************************************************************
** Function name:           Phone_NotifyIsCanRemind
** Descriptions:            �жϿ���״̬����Ϣ�Ƿ�������
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2018-01-11
*****************************************************************/ 
uint8_t Phone_NotifyIsCanRemind(void)
{      
    const message_enable_t *message_p = &(system_info_s.message_enable);    
    
    if((message_p->call_enable == 0x01) && (phone_notify_s.hint.call == 0x01))
    {
        /* ������Ϣ������ʾ */       
        return 1;
    }   
    if((message_p->sms_enable == 0x01) && (phone_notify_s.hint.message == 0x01))
    {
        /* ������Ϣ������ʾ */     
        return 1;
    }
    else if((message_p->wechat_enable == 0x01) && (phone_notify_s.hint.wechat == 0x01))
    {
        /* ΢����Ϣ������ʾ */     
        return 1;
    }
    else if((message_p->qq_enable == 0x01) && (phone_notify_s.hint.qq == 0x01))
    {   
        /* QQ��Ϣ������ʾ */     
        return 1;
    }
    else if((message_p->facebook_enable == 0x01) && (phone_notify_s.hint.facebook == 0x01))
    {   
        /* Facebook��Ϣ���� */     
        return 1;
    }    
    else if((message_p->twitter_enable == 0x01) && (phone_notify_s.hint.twitter == 0x01))
    {
        /* twitter��Ϣ���� */     
        return 1;
    }
    else if((message_p->whatsapp_enable == 0x01) && (phone_notify_s.hint.whatsapp == 0x01))
    {
        /* whatsapp��Ϣ���� */     
        return 1;
    }    
    else if((message_p->linkedin_enable == 0x01) && (phone_notify_s.hint.linkedin == 0x01))
    {
        /* linkedin��Ϣ���� */     
        return 1;
    }   
    else if((message_p->snapchat_enable == 0x01) && (phone_notify_s.hint.snapchat == 0x01))
    {
        /* snapchat��Ϣ���� */     
        return 1;
    }
    else if((message_p->skype_enable == 0x01) && (phone_notify_s.hint.skype == 0x01))
    {
        /* skype��Ϣ���� */     
        return 1;
    }
    else if((message_p->instagram_enable == 0x01) && (phone_notify_s.hint.instagram == 0x01))
    {
        /* instagram��Ϣ���� */     
        return 1;
    }
    else if((message_p->line_enable == 0x01) && (phone_notify_s.hint.line == 0x01))
    {
        /* line��Ϣ���� */     
        return 1;
    }    
    else if((message_p->gmail_enable == 0x01) && (phone_notify_s.hint.gmail == 0x01))
    {   
        /* gmail��Ϣ���� */     
        return 1;
    }
    else if((message_p->other_enable == 0x01) && (phone_notify_s.hint.other == 0x01))
    {   
        /* other��Ϣ���� */     
        return 1;
    } 
    return 0;
}
