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
uint8_t phone_calling_state = 0;   //来电状态

extern system_info_t system_info_s;
extern system_flag_t system_flag_s;

/****************************************************************
** Function name:           Phone_NotifySetCallingState
** Descriptions:            来电状态设置
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-08-24
*****************************************************************/ 
void Phone_NotifySetCallingState(uint8_t state)
{
    phone_calling_state = state;
}

/****************************************************************
** Function name:           Phone_NotifyGetCallingState
** Descriptions:            获取来电状态
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-08-24
*****************************************************************/ 
uint8_t Phone_NotifyGetCallingState(void)
{
    return phone_calling_state;
}

/****************************************************************
** Function name:           Phone_NotifyInit
** Descriptions:            消息控制初始化
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-08-24
*****************************************************************/ 
void Phone_NotifyInit(void)
{
    memset(&phone_notify_s, 0, sizeof(phone_notify_s));
}

/****************************************************************
** Function name:           Phone_NotifyCore
** Descriptions:            消息处理
** input parameters:        无
** output parameters:       无
** Returned value:          无
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
        /* 来电消息推送显示 */
        phone_notify_s.hint.call = 0;
        Phone_NotifySetCallingState(PHONE_CALL_INCOMING);//来电中
        //正在查看信息清除当前信息，提示来电
        if(Message_IsReading()) 
        {
            Message_Clear();       
        }
        Message_Mark(MSG_TYPE_CALL);
        Motor_AddEvent(20, 1, 1 * 20, 2 * 20);//提醒20组 非持续振动 振动1秒 间隔2秒            
        return;
    }   
    if(Message_IsReading())
    {
        return;
    }
    if((message_p->sms_enable == 0x01) && (phone_notify_s.hint.message == 0x01))
    {
        /* 短信消息推送显示 */
        phone_notify_s.hint.message = 0;               
        Message_Mark(MSG_TYPE_MESSAGE);
    }
    else if((message_p->wechat_enable == 0x01) && (phone_notify_s.hint.wechat == 0x01))
    {
        /* 微信消息推送显示 */
        phone_notify_s.hint.wechat = 0;
        
        if(phone_notify_s.msg.content_len > 1  &&  phone_notify_s.msg.content[0] == 0x20)//WJ.20170517.解决微信信息前面的空格
        {
           phone_notify_s.msg.content_len = phone_notify_s.msg.content_len - 1;
           memcpy(phone_notify_s.msg.content, phone_notify_s.msg.content+1, phone_notify_s.msg.content_len); 
        }       
        Message_Mark(MSG_TYPE_WECHAT);
    }
    else if((message_p->qq_enable == 0x01) && (phone_notify_s.hint.qq == 0x01))
    {   
        /* QQ消息推送显示 */
        phone_notify_s.hint.qq = 0;    
        Message_Mark(MSG_TYPE_QQ);
    }
    else if((message_p->facebook_enable == 0x01) && (phone_notify_s.hint.facebook == 0x01))
    {   
        /* Facebook消息推送 */
        phone_notify_s.hint.facebook = 0;   
        Message_Mark(MSG_TYPE_FACEBOOK);
    }    
    else if((message_p->twitter_enable == 0x01) && (phone_notify_s.hint.twitter == 0x01))
    {
        /* twitter消息推送 */
        phone_notify_s.hint.twitter = 0;
        Message_Mark(MSG_TYPE_TWITTER);
    }
    else if((message_p->whatsapp_enable == 0x01) && (phone_notify_s.hint.whatsapp == 0x01))
    {
        /* whatsapp消息推送 */
        phone_notify_s.hint.whatsapp = 0;    
        Message_Mark(MSG_TYPE_WHATSAPP);
    }    
    else if((message_p->linkedin_enable == 0x01) && (phone_notify_s.hint.linkedin == 0x01))
    {
        /* linkedin消息推送 */
        phone_notify_s.hint.linkedin = 0;   
        Message_Mark(MSG_TYPE_LINKEDIN);
    }   
    else if((message_p->snapchat_enable == 0x01) && (phone_notify_s.hint.snapchat == 0x01))
    {
        /* snapchat消息推送 */
        phone_notify_s.hint.snapchat = 0;    
        Message_Mark(MSG_TYPE_SNAPCHAT);
    }
    else if((message_p->skype_enable == 0x01) && (phone_notify_s.hint.skype == 0x01))
    {
        /* skype消息推送 */
        phone_notify_s.hint.skype = 0;   
        Message_Mark(MSG_TYPE_SKYPE);
    }
    else if((message_p->instagram_enable == 0x01) && (phone_notify_s.hint.instagram == 0x01))
    {
        /* instagram消息推送 */
        phone_notify_s.hint.instagram = 0;    
        Message_Mark(MSG_TYPE_INSTAGRAM);
    }
    else if((message_p->line_enable == 0x01) && (phone_notify_s.hint.line == 0x01))
    {
        /* line消息推送 */
        phone_notify_s.hint.line = 0;    
        Message_Mark(MSG_TYPE_LINE);
    }    
    else if((message_p->gmail_enable == 0x01) && (phone_notify_s.hint.gmail == 0x01))
    {   
        /* gmail消息推送 */
        phone_notify_s.hint.gmail = 0; 
        Message_Mark(MSG_TYPE_GMAIL);
    }
    else if((message_p->other_enable == 0x01) && (phone_notify_s.hint.other == 0x01))
    {   
        /* other消息推送 */
        phone_notify_s.hint.other = 0;    
        Message_Mark(MSG_TYPE_OTHER);
    } 
    else
    {
        return;
    }    
    Motor_AddEvent(1, 0, 2 * 20, 0);//提醒1组 非持续振动 振动1秒 
}


/****************************************************************
** Function name:           Phone_NotifyIsCanRemind
** Descriptions:            判断开关状态，消息是否能提醒
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2018-01-11
*****************************************************************/ 
uint8_t Phone_NotifyIsCanRemind(void)
{      
    const message_enable_t *message_p = &(system_info_s.message_enable);    
    
    if((message_p->call_enable == 0x01) && (phone_notify_s.hint.call == 0x01))
    {
        /* 来电消息推送显示 */       
        return 1;
    }   
    if((message_p->sms_enable == 0x01) && (phone_notify_s.hint.message == 0x01))
    {
        /* 短信消息推送显示 */     
        return 1;
    }
    else if((message_p->wechat_enable == 0x01) && (phone_notify_s.hint.wechat == 0x01))
    {
        /* 微信消息推送显示 */     
        return 1;
    }
    else if((message_p->qq_enable == 0x01) && (phone_notify_s.hint.qq == 0x01))
    {   
        /* QQ消息推送显示 */     
        return 1;
    }
    else if((message_p->facebook_enable == 0x01) && (phone_notify_s.hint.facebook == 0x01))
    {   
        /* Facebook消息推送 */     
        return 1;
    }    
    else if((message_p->twitter_enable == 0x01) && (phone_notify_s.hint.twitter == 0x01))
    {
        /* twitter消息推送 */     
        return 1;
    }
    else if((message_p->whatsapp_enable == 0x01) && (phone_notify_s.hint.whatsapp == 0x01))
    {
        /* whatsapp消息推送 */     
        return 1;
    }    
    else if((message_p->linkedin_enable == 0x01) && (phone_notify_s.hint.linkedin == 0x01))
    {
        /* linkedin消息推送 */     
        return 1;
    }   
    else if((message_p->snapchat_enable == 0x01) && (phone_notify_s.hint.snapchat == 0x01))
    {
        /* snapchat消息推送 */     
        return 1;
    }
    else if((message_p->skype_enable == 0x01) && (phone_notify_s.hint.skype == 0x01))
    {
        /* skype消息推送 */     
        return 1;
    }
    else if((message_p->instagram_enable == 0x01) && (phone_notify_s.hint.instagram == 0x01))
    {
        /* instagram消息推送 */     
        return 1;
    }
    else if((message_p->line_enable == 0x01) && (phone_notify_s.hint.line == 0x01))
    {
        /* line消息推送 */     
        return 1;
    }    
    else if((message_p->gmail_enable == 0x01) && (phone_notify_s.hint.gmail == 0x01))
    {   
        /* gmail消息推送 */     
        return 1;
    }
    else if((message_p->other_enable == 0x01) && (phone_notify_s.hint.other == 0x01))
    {   
        /* other消息推送 */     
        return 1;
    } 
    return 0;
}
