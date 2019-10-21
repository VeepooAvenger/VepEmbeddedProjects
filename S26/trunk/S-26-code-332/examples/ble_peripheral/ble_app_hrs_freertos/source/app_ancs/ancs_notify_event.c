/*******************Copyright(c)*********************************
** Veepoo Tech
** COPYRIGHT (C) 2016-2019
**----------------------FileInfo---------------------------------
** File name:                  ancs_notify_event.c
** Last modified Date:         2017-07-15
** Last SYS_VERSION:           0.0.1
** Descriptions:
**---------------------------------------------------------------
** Createdby:                  GT
** Createddate:                2017-07-15
** SYS_VERSION:                0.0.1
** Descriptions:               ANCS 通知事件
**---------------------------------------------------------------
** Modified by:				
** Modified date:			
** SYS_VERSION:				
** Modified Descriptions:	
****************************************************************/

#include <stdint.h>
#include <string.h>
#include "ancs_notify_event.h"
#include "data_manage.h"
#include "app_ancs.h"
#include "api_flash.h"
#include "cpu_task_data_type.h"
#include "gui_thread.h"
#include "motor_event.h"
#include "gui_font_lattice.h"

static uint8_t recv_package_flag = 0;

uint8_t m_attr_appID[32];
uint8_t m_attr_title[32];                       // < Buffer to store attribute data.
uint8_t m_attr_subtitle[32];                    // < Buffer to store attribute data. 
uint8_t m_attr_message[ATTR_DATA_SIZE];         // < Buffer to store attribute data.
uint8_t m_attr_message_size[32];                // < Buffer to store attribute data.
uint8_t m_attr_date[32];                        // < Buffer to store attribute data. 
uint8_t m_attr_posaction[32];                   // < Buffer to store attribute data.  
uint8_t m_attr_negaction[32];                   // < Buffer to store attribute data. 

uint8_t call_infor_type = 0;
/****************************************************************
** Function name:               Get_Message_Size
** Descriptions:                获取信息长度
** input parameters:            无
** output parameters:           无
** Returned value:              无
** Created by:                  GT    
** Created Date:                2017-07-15
**---------------------------------------------------------------
** Modified by:                  
** Modified date:              
** Modified Descriptions:
**---------------------------------------------------------------
****************************************************************/
static uint8_t Get_Message_Size(void)
{        
    uint16_t message_len = 0x00;
    //三个字节组合成一个长度，mattr_message_size[0]以ASCII码表示
//    if(m_attr_message_size[0] >= 0x30 && m_attr_message_size[1] < 0x30 && m_attr_message_size[2]< 0x30) 
//    {
//        message_len = m_attr_message_size[0]-0x30;
//    }
//    else if(m_attr_message_size[0] >= 0x30 && m_attr_message_size[1] >= 0x30 && m_attr_message_size[2]< 0x30)
//    {
//        message_len = (m_attr_message_size[0]-0x30)*10 + m_attr_message_size[1] - 0x30;
//    }
//    else if(m_attr_message_size[0] >= 0x30 && m_attr_message_size[1] >= 0x30 && m_attr_message_size[2]>=0x30)
//    {
//        message_len = (m_attr_message_size[0]-0x30)*100 + (m_attr_message_size[1] - 0x30)*10 + m_attr_message_size[2] - 0x30;
//    }

    for(uint8_t i = 0; i < sizeof(m_attr_message); i++)
    {
        if(m_attr_message[i] == 0x00)
        {
            break;
        }
        message_len++;
    }
    
    if(message_len >= sizeof(m_attr_message))
    {
        message_len = sizeof(m_attr_message);
    }
    
    return message_len;
}

/****************************************************************
** Function name:               Message_Filter
** Descriptions:                信息过滤
** input parameters:            无
** output parameters:           无
** Returned value:              无
** Created by:                  GT    
** Created Date:                2017-07-15
**---------------------------------------------------------------
** Modified by:                  
** Modified date:              
** Modified Descriptions:
**---------------------------------------------------------------
****************************************************************/
static void Ancs_GetMessageContent(message_notify_content_t *p_notify, uint8_t message_type)
{    
    uint16_t tile_msg_len = 0;
    uint16_t attr_msg_len = 0;
    uint8_t index_start = 0;
        
    if(message_type == MSG_NOTIFY_QQ_ID)
    {
        index_start = 3;
    }
    // 这里需要修正 修正原因:IOS Version10.3.3版本 微信备注放在了title结构体中 
    // 遍历title结构体,取出有效数据
    for(uint8_t index = index_start; index < sizeof(m_attr_title); index++)
    {
        if(m_attr_title[index] != 0)
        {
            p_notify->content[index] = m_attr_title[index];
        }
        else
        {
            tile_msg_len = index - index_start;
            break;
        }
    }
    
    // 如果title中有内容,此处需要人为添加冒号ASCII:0x3A
    if(tile_msg_len > 0)
    {
        tile_msg_len = Font_FullScreenTotalActiveLenght(p_notify->content, tile_msg_len);
        p_notify->content[tile_msg_len] = 0x3A;
        tile_msg_len += 1;
    }
    
    // 2.取出Message中有效内容
    attr_msg_len = Get_Message_Size();
    if(tile_msg_len + attr_msg_len > sizeof(p_notify->content))
    {
        attr_msg_len = sizeof(p_notify->content) - tile_msg_len;
    }
    
    // 3.更新内容长度
    p_notify->len = tile_msg_len + attr_msg_len;
    memcpy(&p_notify->content[tile_msg_len], m_attr_message, attr_msg_len);
}

/****************************************************************
** Function name:               ancs_phone_number_filter
** Descriptions:                电话号码过滤
** input parameters:            无
** output parameters:           无
** Returned value:              无
** Created by:                  GT    
** Created Date:                2017-07-15
**---------------------------------------------------------------
** Modified by:                  
** Modified date:              
** Modified Descriptions:
**---------------------------------------------------------------
****************************************************************/
static void ancs_phone_number_filter(call_notify_content_t* p_call_notify, uint8_t* p_data,uint8_t len)
{    
    p_call_notify->len = 0;

    for(uint8_t i = 0; i < len; i++)
    {
        // 如果出现分隔符,则过滤掉 
        if(p_data[i] == 0x2D)
        {
            continue;
        }
        
        p_call_notify->content[p_call_notify->len++] = p_data[i];
    }
}

/****************************************************************
** Function name:               ancs_phone_number_identify
** Descriptions:                电话号码识别
** input parameters:            无
** output parameters:           无
** Returned value:              无
** Created by:                  GT    
** Created Date:                2017-07-15
**---------------------------------------------------------------
** Modified by:                  
** Modified date:              
** Modified Descriptions:
**---------------------------------------------------------------
****************************************************************/
static void ancs_phone_number_identify(call_notify_content_t *p_call_notify)
{    
    // 电话号码分析 
    if((m_attr_title[0]  == 0xE2)&&(m_attr_title[1] == 0x80)&&(m_attr_title[2] == 0xAD)
      &&(m_attr_title[6] == 0x2D)&&(m_attr_title[11] == 0x2D))
    {
        // iphone6plus出现 
        ancs_phone_number_filter(p_call_notify, &m_attr_title[3],21);
    }
    else if((m_attr_title[3] == 0x2D)&&(m_attr_title[8] == 0x2D))
    {
        ancs_phone_number_filter(p_call_notify, &m_attr_title[0],21);
    }
    else if((m_attr_title[0] ==  0xE2)&&(m_attr_title[1] == 0x80)&&(m_attr_title[2] == 0xAD)
           &&(m_attr_title[7]  ==  0x2D))
    {
        // iphone6plus出现座机号码 
        ancs_phone_number_filter(p_call_notify, &m_attr_title[3],21);
    }
    else if((m_attr_title[4] ==  0x2D)&&(0x30 <= m_attr_title[3])&&(m_attr_title[3] <= 0x39))
    {
        // 显示座机号码  
        ancs_phone_number_filter(p_call_notify, &m_attr_title[0],21);
    }
    else
    {
        p_call_notify->len = 24 ;
        // 找到空格 
        for(int i = 0; i < 24; i++)
        {
            if(m_attr_title[i]  == 0x00)
            {
                p_call_notify->len = i;
                break;
            }
        }

        memcpy(p_call_notify->content, (uint8_t *)m_attr_title, p_call_notify->len);
    }
}

/****************************************************************
** Function name:               ancs_call_event
** Descriptions:                
** input parameters:            无
** output parameters:           无
** Returned value:              无
** Created by:                  GT    
** Created Date:                2017-07-15
**---------------------------------------------------------------
** Modified by:                  
** Modified date:              
** Modified Descriptions:
**---------------------------------------------------------------
****************************************************************/
static void ancs_call_event(void)
{
    call_notify_content_t *p_call_notify = &p_data_manage->app_notify_s.call_content_s;
    
    if(call_infor_type == 0)
    {
        //YX.2017.12.18.屏蔽推送消息为未接电话时，出现电话事件提醒的问题
        return ;
    }
    
    if(App_Ancs_BlePhoneGet())//接听标志位
    {
        memset(p_call_notify, 0, sizeof(call_notify_content_t));
            
        ancs_phone_number_identify(p_call_notify);

        GuiThread_AddEvent(CPU_EVENT_TASK_CALL);
    }
//    else
//    {	   
//        MotorDelEvent();//关闭马达     
//        // 如果是手环端拒接,则不能响应挂断 Modify by GT 2017-11-1?
//        if(p_data_manage->app_notify_s.call_content_s.reject_call == 0)
//        {
//            p_data_manage->app_notify_s.call_content_s.hang_up_call = 1;
//        }
//    }	    
}

/****************************************************************
** Function name:               ancs_message_event
** Descriptions:                信息事件
** input parameters:            无
** output parameters:           无
** Returned value:              无
** Created by:                  GT    
** Created Date:                2017-07-15
**---------------------------------------------------------------
** Modified by:                  
** Modified date:              
** Modified Descriptions:
**---------------------------------------------------------------
****************************************************************/
static void ancs_message_event(message_notify_content_t *p_message_notify)
{
    call_notify_content_t  call_notify = {0};
    uint8_t content_len = Get_Message_Size();

    /* 拷贝数据 */
    ancs_phone_number_identify(&call_notify);

    if(call_notify.len + content_len >= sizeof(p_message_notify->content))
    {
        content_len = sizeof(p_message_notify->content) - call_notify.len;
    }
    
    // 将电话号码拷贝到content内容中
    memcpy(p_message_notify->content, call_notify.content, call_notify.len);
    call_notify.len = Font_FullScreenTotalActiveLenght(p_message_notify->content, call_notify.len);
    p_message_notify->len = call_notify.len;
//    p_data_manage->app_notify_s.msg_title_len = call_notify.len;
    p_message_notify->content[p_message_notify->len] = 0x3A;
    p_message_notify->len++;
    
    if(p_message_notify->len + content_len > sizeof(p_message_notify->content))
    {
        content_len = sizeof(p_message_notify->content) - p_message_notify->len;
    }
    // 拼接内容
    memcpy((uint8_t*)&p_message_notify->content[p_message_notify->len], (uint8_t *)m_attr_message, content_len);
    p_message_notify->len += content_len;
    
}

/****************************************************************
** Function name:               notif_attr_print
** Descriptions:                通知
** input parameters:            p_attr：    
                                ancs_attr_list：    
** output parameters:           无
** Returned value:              无
** Created by:                  GT    
** Created Date:                2017-07-15
**---------------------------------------------------------------
** Modified by:                  
** Modified date:              
** Modified Descriptions:
**---------------------------------------------------------------
****************************************************************/
void notif_attr_print(ble_ancs_c_evt_notif_attr_t* p_attr,
                      ble_ancs_c_attr_list_t* ancs_attr_list)
{
    uint8_t attr_type = MSG_NOTIFY_NULL_ID;
    switch(p_attr->attr_id)
    {
        case BLE_ANCS_NOTIF_ATTR_ID_APP_IDENTIFIER:
            recv_package_flag |= 0x01; break;
        case BLE_ANCS_NOTIF_ATTR_ID_TITLE:
            recv_package_flag |= 0x02; break;
        case BLE_ANCS_NOTIF_ATTR_ID_SUBTITLE:
            recv_package_flag |= 0x04; break;
        case BLE_ANCS_NOTIF_ATTR_ID_MESSAGE:
            recv_package_flag |= 0x08; break;
        case BLE_ANCS_NOTIF_ATTR_ID_MESSAGE_SIZE:
            recv_package_flag |= 0x10; break;
        case BLE_ANCS_NOTIF_ATTR_ID_DATE:
            recv_package_flag |= 0x20; break;
        case BLE_ANCS_NOTIF_ATTR_ID_POSITIVE_ACTION_LABEL:
            recv_package_flag |= 0x40; break;
        case BLE_ANCS_NOTIF_ATTR_ID_NEGATIVE_ACTION_LABEL:
            recv_package_flag |= 0x80; break;
        default:
            break;
    }
    // 当所有的属性包接收完成时,处理数据
    if((recv_package_flag & 0x0F) == 0x0F)
    {
        if(recv_package_flag == 0x0F)
        {
            if(!strcmp((const char*)m_attr_appID,(const char*)"com.apple.MobileSMS"))
            {
                if(p_flash_param->message_enable_s.sms  == 1)
                {
                    attr_type = MSG_NOTIFY_SMS_ID;
                }
            }
            else if(!strcmp((const char*)m_attr_appID,(const char *)"com.apple.mobilephone"))
            {
                if(p_flash_param->message_enable_s.call  == 1)
                {
                    ancs_call_event();
                }
            }
            else
            {
                return;
            }
        }
        recv_package_flag = 0x00;
                
        // 1.处理来电
        if(!strcmp((const char*)m_attr_appID,(const char *)"com.apple.mobilephone"))
        {
            if(p_flash_param->message_enable_s.call  == 1)
            {
                ancs_call_event();
            }
        }
        else
        { 
            if(!strcmp((const char*)m_attr_appID,(const char*)"com.apple.MobileSMS"))
            {
                if(p_flash_param->message_enable_s.sms  == 1)
                {
                    // 2.处理短信
                    attr_type = MSG_NOTIFY_SMS_ID;
                }
            }
            else
            {    
                // 3.处理消息推送
                if(!strcmp((const char*)m_attr_appID,(const char*)"com.tencent.xin"))
                {
                    if(p_flash_param->message_enable_s.wechat  == 1)
                    {
                        attr_type = MSG_NOTIFY_WECHAT_ID;
                    }
                }
                else if((!strcmp((const char*)m_attr_appID,"com.tencent.mqq"))||(!strcmp((const char*)m_attr_appID,"com.tencent.tim"))||(!strcmp((const char*)m_attr_appID,"com.tencent.mQQi")))
                {
                    if(p_flash_param->message_enable_s.qq == 1)
                    {
                        attr_type = MSG_NOTIFY_QQ_ID;
                    }
                }
                else if(!strcmp((const char*)m_attr_appID,(const char*)"com.facebook.Facebook"))
                {
                    if(p_flash_param->message_enable_s.facebook  == 1)
                    {
                        attr_type = MSG_NOTIFY_FACEBOOK_ID;
                    }
                }
                else if(!strcmp((const char*)m_attr_appID,(const char*)"com.atebits.Tweetie2"))
                {
                    if(p_flash_param->message_enable_s.twitter ==  1)
                    {
                        attr_type = MSG_NOTIFY_TWITTER_ID;
                    }
                }
                else if(!strcmp((const char*)m_attr_appID,(const char*)"net.whatsapp.WhatsApp"))
                {
                    if(p_flash_param->message_enable_s.whatsapp  == 1)
                    {
                        attr_type = MSG_NOTIFY_WHATSAPP_ID;
                    }
                }
                else if(!strcmp((const char*)m_attr_appID,"com.linkedin.Zephyr"))
                {
                    if(p_flash_param->message_enable_s.linkedln  == 1)
                    {
                        attr_type = MSG_NOTIFY_LINKEDLN_ID;
                    }
                }
                else if(!strcmp((const char*)m_attr_appID,"jp.naver.line"))
                {
                    if(p_flash_param->message_enable_s.line  == 1)
                    {
                        attr_type = MSG_NOTIFY_LINE_ID;
                    }
                }
                else if(!strcmp((const char*)m_attr_appID,"com.burbn.instagram"))
                {
                    if(p_flash_param->message_enable_s.instagram  == 1)
                    {
                        attr_type = MSG_NOTIFY_INSTAGRAM_ID;
                    }
                }
                else if(!strcmp((const char*)m_attr_appID,"com.toyopagroup.picaboo"))
                {
                    if(p_flash_param->message_enable_s.snapchat  == 1)
                    {
                        attr_type = MSG_NOTIFY_SNAPCHAT_ID;
                    }
                }
                else if((!strcmp((const char*)m_attr_appID,"com.skype.tomskype"))
                       //YX.20180508.修复Skype应用推送的问题
                       ||(!strcmp((const char*)m_attr_appID,"com.skype.skype"))
                       ||(!strcmp((const char*)m_attr_appID,"com.microsoft.lync2013.iphone")))
                {
                    if(p_flash_param->message_enable_s.skype  == 1)
                    {
                        attr_type = MSG_NOTIFY_SKYPE_ID;
                    }
                }
                else if(!strcmp((const char*)m_attr_appID,"com.google.Gmail"))
                {
                    if(p_flash_param->message_enable_s.gmail == 1)
                    {
                        attr_type = MSG_NOTIFY_GMAIL_ID;
                    }
                }
                else if(attr_type == MSG_NOTIFY_NULL_ID)
                {
                    if(p_flash_param->message_enable_s.other == 1)
                    {
                        attr_type = MSG_NOTIFY_OTHER_ID;
                    }
                }
            }
            
            if(attr_type != MSG_NOTIFY_NULL_ID)
            {
                p_data_manage->app_notify_s.update_index = (p_data_manage->app_notify_s.update_index+1)%2;
                message_notify_content_t*  p_message_notify = &p_data_manage->app_notify_s.message_content_s[p_data_manage->app_notify_s.update_index];
                memset(p_message_notify, 0, sizeof(message_notify_content_t));
                p_message_notify->type = (MSG_NOTIFY_TYPE_E)attr_type;
                if(attr_type == MSG_NOTIFY_SMS_ID)
                {
                    // 2.处理短信
                    ancs_message_event(p_message_notify);
                }
                else
                {
                    // 3.处理消息推送
                    Ancs_GetMessageContent(p_message_notify, attr_type);
                }
                p_data_manage->app_notify_s.unread_msg_count = 1;
                GuiThread_AddEvent(CPU_EVENT_TASK_MESSAGE);
            }
        }
            
        // 数据处理完成,重新初始化 
        memset(m_attr_appID, 0, sizeof(m_attr_appID));
        memset(m_attr_title, 0, sizeof(m_attr_title));
        memset(m_attr_message, 0, sizeof(m_attr_message));
    }
    
    if(recv_package_flag & 0xf0)
    {
        recv_package_flag = 0;
        // 数据处理完成,重新初始化 
        memset(m_attr_appID, 0, sizeof(m_attr_appID));
        memset(m_attr_title, 0, sizeof(m_attr_title));
        memset(m_attr_message, 0, sizeof(m_attr_message));
    }
}
