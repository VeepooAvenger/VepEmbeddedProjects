/***************************Copyright(c)***********************************
**                       Veepoo Tech
**                  COPYRIGHT (C) 2016-2019
**-----------File Info-----------------------------------------------------
** File        name:     ancs_notify_event.c
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
#include <stdint.h>
#include <string.h>
#include "ancs_notify_event.h"
#include "phone_notify_event.h"
#include "app_ancs.h"
#include "gui_thread.h"
#include "app_config_data.h"

static uint8_t recv_package_flag = 0;
const char*  mobileSMS    = "com.apple.MobileSMS";      //短信
const char*  mobilePhone  = "com.apple.mobilephone";    //来电
const char*  mobilexin    = "com.tencent.xin";          //微信
const char*  mobileqq     = "com.tencent.mqq" ;         //QQ   
const char*  mobileqq2     = "com.tencent.tim" ;        //QQ轻聊版 
const char*  mobileqq3     = "com.tencent.mQQi" ;       //QQ国际版
const char*  mobileTwitter = "com.atebits.Tweetie2";    //推特
const char*  mobileFacebook = "com.facebook.Facebook";  //Facebook
const char*  mobileWhatsApp = "net.whatsapp.WhatsApp";  //whatsapp
const char*  mobileLinkedin  = "com.linkedin.Zephyr";   //linkedin
const char*  mobileSnapchat    = "com.toyopagroup.picaboo";  //Snapchat
const char*  mobileSkype     = "com.skype.tomskype" ;   //skype
const char*  mobileSkype_1     = "com.skype.skype" ;   //skype
const char*  mobileSkype_2     = "com.microsoft.lync2013.iphone" ;   //skype
const char*  mobileInstagram = "com.burbn.instagram";   //instagram
const char*  mobileLine = "jp.naver.line";              //line
const char*  mobileGmail = "com.google.Gmail";          //Gmail

uint8_t m_attr_appID[ATTR_DATA_SIZE] = {0x00,};
uint8_t m_attr_title[ATTR_DATA_SIZE] = {0x00,};           /**< Buffer to store attribute data. */
uint8_t m_attr_subtitle[ATTR_DATA_SIZE] = {0x00,};        /**< Buffer to store attribute data. */
uint8_t m_attr_message[ATTR_MESSAGE_DATA_SIZE] = {0x00,}; /**< Buffer to store attribute data. */
uint8_t m_attr_message_size[4] = {0x00,};                 /**< Buffer to store attribute data. */
uint8_t m_attr_date[ATTR_DATA_SIZE] = {0x00,};            /**< Buffer to store attribute data. */
uint8_t m_attr_posaction[ATTR_DATA_SIZE] = {0x00,};       /**< Buffer to store attribute data. */
uint8_t m_attr_negaction[ATTR_DATA_SIZE] = {0x00,};       /**< Buffer to store attribute data. */

extern system_info_t system_info_s;

uint8_t Notify_ArrayContentSize(uint8_t *p_data, uint8_t len);

/****************************************************************
** Function name:           Notify_MessageFilter
** Descriptions:            Message内容过滤
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-08-24
*****************************************************************/ 
static void Notify_MessageFilter(void)
{
//    uint16_t i = 0;
//    /* 找到消息的冒号 */
//    for( i = 0; i < ATTR_MESSAGE_DATA_SIZE; i++)
//    {
//        if(m_attr_message[i] == 0x3A)
//        {
//            phone_notify_s.msg.title_len = i;
//            break;
//        }
//    }

//    /* 如果找到冒号,则需要处理 */
//    if(phone_notify_s.msg.title_len > 0)
//    {
//        memcpy(phone_notify_s.msg.title,(uint8_t *)m_attr_message,(phone_notify_s.msg.title_len>TLTLE_DATA_MAX_LEN)?TLTLE_DATA_MAX_LEN:phone_notify_s.msg.title_len); 
//        memcpy(phone_notify_s.msg.content,(uint8_t *)&m_attr_message[i+1], ATTR_MESSAGE_DATA_SIZE - i - 1);
//        phone_notify_s.msg.content_len = ATTR_MESSAGE_DATA_SIZE - i - 1;
//    }
//    else
//    {
        phone_notify_s.msg.title_len = 0;
        phone_notify_s.msg.content_len = ATTR_MESSAGE_DATA_SIZE;
        memcpy(phone_notify_s.msg.content,m_attr_message,ATTR_MESSAGE_DATA_SIZE);
//    }
}

/****************************************************************
** Function name:           Notify_TwitterFilter
** Descriptions:            twitter内容过滤
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-08-24
*****************************************************************/ 
static void Notify_TwitterFilter(void)
{
    uint16_t i = 0;
    /* 找到消息的冒号 */
    for( i = 0; i < ATTR_MESSAGE_DATA_SIZE; i++)
    {
        if(m_attr_message[i] == 0x3A)
        {
            phone_notify_s.msg.title_len = i;
            break;
        }
    }

    if(phone_notify_s.msg.title_len > 0)
    {
        memcpy(phone_notify_s.msg.title,(uint8_t *)m_attr_message,(phone_notify_s.msg.title_len>TLTLE_DATA_MAX_LEN)?TLTLE_DATA_MAX_LEN:phone_notify_s.msg.title_len); 
        memcpy(phone_notify_s.msg.content,(uint8_t *)&m_attr_message[i + 1], ATTR_MESSAGE_DATA_SIZE - i - 1);       
        phone_notify_s.msg.content_len = ATTR_MESSAGE_DATA_SIZE - i - 1;
    }
    else
    {
        phone_notify_s.msg.title_len = 0;
        phone_notify_s.msg.content_len = ATTR_MESSAGE_DATA_SIZE;
        memcpy(phone_notify_s.msg.content,m_attr_message, ATTR_MESSAGE_DATA_SIZE);
    }
}

/****************************************************************
** Function name:           Notify_GeneralFilter
** Descriptions:            通用信息过滤
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-08-24
*****************************************************************/ 
static void Notify_GeneralFilter(void)
{
    //WJ.20171125.解决部分存在标题不提示的问题
    phone_notify_s.msg.title_len = Notify_ArrayContentSize(m_attr_title, ATTR_DATA_SIZE);
    if(phone_notify_s.msg.title_len != 0)
    {
        memcpy(phone_notify_s.msg.title,(uint8_t *)&m_attr_title, ATTR_DATA_SIZE);
    }
    phone_notify_s.msg.content_len = ATTR_MESSAGE_DATA_SIZE;
    memcpy(phone_notify_s.msg.content,(uint8_t *)&m_attr_message, ATTR_MESSAGE_DATA_SIZE);
}

/****************************************************************
** Function name:           Notify_ArrayContentSize
** Descriptions:            获取内容长度
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-08-24
*****************************************************************/ 
uint8_t Notify_ArrayContentSize(uint8_t *p_data, uint8_t len)
{
    uint8_t size = 0;

    for(int i = 0; i < len; i++)
    {
        if(p_data[i] == 0)
        {
            size = i + 1;
            break;
        }
    }

    return size;
}

/****************************************************************
** Function name:           Notify_PhoneNumberFilter
** Descriptions:            数字过滤
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-08-24
*****************************************************************/ 
static void Notify_PhoneNumberFilter(uint8_t *p_data,uint8_t len)
{
    for(int i = 0; i < len; i++)
    {
        /* 如果出现分隔符,则过滤掉 */
        if(p_data[i] == 0x2D)
        {
            continue;
        }
        phone_notify_s.msg.title[phone_notify_s.msg.title_len++] = p_data[i];
    }
}

/****************************************************************
** Function name:           Notify_PhoneNumberIdentify
** Descriptions:            电话号码处理
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-08-24
*****************************************************************/ 
static void Notify_PhoneNumberIdentify(uint8_t type)
{	
    phone_notify_s.msg.title_len = 0 ;

    /* 电话号码分析 */
    if((m_attr_title[0] == 0xE2) && (m_attr_title[1] == 0x80) && (m_attr_title[2] == 0xAD)
      &&(m_attr_title[6] == 0x2D)&&(m_attr_title[11] == 0x2D))
    {
        /* iphone6plus出现 */
        Notify_PhoneNumberFilter(&m_attr_title[3], 21);
    }
    else if((m_attr_title[3] == 0x2D) && (m_attr_title[8] == 0x2D))
    {
        Notify_PhoneNumberFilter(&m_attr_title[0], 21);
    }
    else if((m_attr_title[0] == 0xE2) && (m_attr_title[1] == 0x80) && (m_attr_title[2] == 0xAD)
           &&(m_attr_title[7] == 0x2D))
    {
        /* iphone6plus出现座机号码 */
        Notify_PhoneNumberFilter(&m_attr_title[3], 21);
    }
    else if(( m_attr_title[4] == 0x2D) && (m_attr_title[3] >= 0x30 ) && (m_attr_title[3] <= 0x39))
    {
        /* 显示座机号码 */
        Notify_PhoneNumberFilter(&m_attr_title[0], 21);
    }
    else
    {//YX.20170603.让短信名称达到主题的最大长度
        if(type == 1) // call
        {
            phone_notify_s.msg.title_len = 24 ;
        }
        else // message
        {
            phone_notify_s.msg.title_len = ATTR_DATA_SIZE ;
        }        
        /* 找到空格 */
        for(int i = 0; i < phone_notify_s.msg.title_len; i++)
        {
            if(m_attr_title[i] == 0x00)
            {
                phone_notify_s.msg.title_len = i;
                break;
            }
        }

        memcpy(phone_notify_s.msg.title, (uint8_t *)m_attr_title, phone_notify_s.msg.title_len);

    }
	
	//JSH.20181025.修复ios陌生来电号码不居中的bug
	for(uint32_t i = 0; i < 24; i++)
	{
		if(phone_notify_s.msg.title[i] == 0)
		{
			phone_notify_s.msg.title_len = i;
			
			break;
		}
	}
	//JSH.20181025.修复ios陌生来电号码不居中的bug
	if((phone_notify_s.msg.title[0] == 0xE2 && phone_notify_s.msg.title[1] == 0x80 && phone_notify_s.msg.title[2] == 0xAD)
      && (phone_notify_s.msg.title[phone_notify_s.msg.title_len - 3] == 0xE2 
	   && phone_notify_s.msg.title[phone_notify_s.msg.title_len - 2] == 0x80 
	   && phone_notify_s.msg.title[phone_notify_s.msg.title_len - 1] == 0xAC))
    {
        phone_notify_s.msg.title_len -= 6;
        memcpy(phone_notify_s.msg.title, &phone_notify_s.msg.title[3], phone_notify_s.msg.title_len);
        phone_notify_s.msg.title[phone_notify_s.msg.title_len] = 0;
    }
}

/****************************************************************
** Function name:           Notify_AncsCallEvent
** Descriptions:            call消息处理
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-08-24
*****************************************************************/ 
static void Notify_AncsCallEvent(void)
{
    if(Ancs_GetBlePhone())//接听标志位
    {
        phone_notify_s.msg.content_len = 0;
            
        Notify_PhoneNumberIdentify(1);

        /* 拷贝数据 */
        phone_notify_s.control.recv_done = 1;
        phone_notify_s.hint.call = 1;
    } 
}

/* 短信处理 */

/****************************************************************
** Function name:           Notify_AncsMessageEvent
** Descriptions:            message消息处理
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-08-24
*****************************************************************/ 
static void Notify_AncsMessageEvent(void)
{
    /* 拷贝数据 */
    phone_notify_s.msg.content_len = ATTR_MESSAGE_DATA_SIZE;

    Notify_PhoneNumberIdentify(2);
    memcpy(phone_notify_s.msg.content, (uint8_t *)m_attr_message, ATTR_MESSAGE_DATA_SIZE);//message

    phone_notify_s.control.recv_done = 1;
    phone_notify_s.hint.message = 1;
}

/****************************************************************
** Function name:           Notify_MessageFilter
** Descriptions:            wechat消息处理
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-08-24
*****************************************************************/ 
static void Notify_AncsWechatEvent(void)
{	
    uint16_t wechat_title_len = 0;
	const uint8_t weixin_buf[7] = {0xE5, 0xBE, 0xAE, 0xE4, 0xBF, 0xA1,0x00}; //“微信”编码
    
    /* 拷贝数据 */
    Notify_MessageFilter();
                        
    for(uint16_t i = 0; i < ATTR_DATA_SIZE; i++)
    {
        if(m_attr_title[i] == 0x00)
        {
            wechat_title_len = i;
            wechat_title_len++;
            break;
        }
    } 
    //兼容旧版本微信，旧版本标题中存在“微信”两个字
    if(memcmp(m_attr_title, weixin_buf, 7) == 0)
    {
        wechat_title_len = 0;
    }
    if(wechat_title_len != 0)
    {
        memcpy(&phone_notify_s.msg.title[phone_notify_s.msg.title_len], m_attr_title, 
              wechat_title_len);
        phone_notify_s.msg.title_len += wechat_title_len;
    }
    
    phone_notify_s.control.recv_done = 1;
    phone_notify_s.hint.wechat = 1;
    if(phone_notify_s.msg.title_len > 1)//&& phone_notify_s.msg.title_len < TLTLE_DATA_MAX_LEN)  
    {
       phone_notify_s.msg.title_len--;
    } 
//    if(phone_notify_s.msg.content_len > 1 && phone_notify_s.msg.content[0] == 0x20)        //WJ.20170517.解决微信信息前面的空格
//    {
//       memcpy(phone_notify_s.msg.content, phone_notify_s.msg.content+1, (phone_notify_s.msg.content_len-1)); 
//    }
}

/****************************************************************
** Function name:           Notify_MessageFilter
** Descriptions:            qq消息处理
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-08-24
*****************************************************************/ 
static void Notify_AncsQQEvent(void)
{
    /* 读取*/
    Notify_MessageFilter();

    /* 拷贝数据 */
    phone_notify_s.control.recv_done = 1;
    phone_notify_s.hint.qq = 1;
}

/****************************************************************
** Function name:           Notify_MessageFilter
** Descriptions:            facebook消息处理
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-08-24
*****************************************************************/ 
static void Notify_AncsFacebookEvent(void)
{
    /* 读取*/
    Notify_GeneralFilter();

    /* 拷贝数据 */
    phone_notify_s.control.recv_done = 1;
    phone_notify_s.hint.facebook = 1;
}

/****************************************************************
** Function name:           Notify_AncsTwitterEvent
** Descriptions:            twitter消息处理
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-08-24
*****************************************************************/ 
static void Notify_AncsTwitterEvent(void)
{
    /* 读取*/
    Notify_TwitterFilter();

    /* 拷贝数据 */
    phone_notify_s.control.recv_done = 1;
    phone_notify_s.hint.twitter = 1;
}

/****************************************************************
** Function name:           Notify_AncsWhatsappEvent
** Descriptions:            whatsapp消息处理
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-08-24
*****************************************************************/ 
static void Notify_AncsWhatsappEvent(void)
{
    /* 读取*/
    Notify_GeneralFilter();

    /* 拷贝数据 */
    phone_notify_s.control.recv_done = 1;
    phone_notify_s.hint.whatsapp = 1;
}

/****************************************************************
** Function name:           Notify_AncsLinkedinEvent
** Descriptions:            linkedin消息处理
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-08-24
*****************************************************************/ 
static void Notify_AncsLinkedinEvent(void)
{
    /* 读取*/
    Notify_GeneralFilter();
    
    /* 拷贝数据 */
    phone_notify_s.control.recv_done = 1;
    phone_notify_s.hint.linkedin = 1;
}

/****************************************************************
** Function name:           Notify_AncsSnapchatEvent
** Descriptions:            snapchat消息处理
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-08-24
*****************************************************************/ 
static void Notify_AncsSnapchatEvent(void)
{
    /* 读取*/
    Notify_GeneralFilter();
    
    /* 拷贝数据 */
    phone_notify_s.control.recv_done = 1;
    phone_notify_s.hint.snapchat = 1;
}

/****************************************************************
** Function name:           Notify_AncsSkypeEvent
** Descriptions:            skype消息处理
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-08-24
*****************************************************************/ 
static void Notify_AncsSkypeEvent(void)
{
    /* 读取*/
    Notify_GeneralFilter();
    
    /* 拷贝数据 */
    phone_notify_s.control.recv_done = 1;
    phone_notify_s.hint.skype = 1;
}

static void Notify_AncsInstagramEvent(void)
{
    /* 读取*/
    Notify_GeneralFilter();
    
    /* 拷贝数据 */
    phone_notify_s.control.recv_done = 1;
    phone_notify_s.hint.instagram = 1;
}

/****************************************************************
** Function name:           Notify_AncsLineEvent
** Descriptions:            line消息处理
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-08-24
*****************************************************************/ 
static void Notify_AncsLineEvent(void)
{
    /* 读取*/
    Notify_GeneralFilter();
    
    /* 拷贝数据 */
    phone_notify_s.control.recv_done = 1;
    phone_notify_s.hint.line = 1;
}

/****************************************************************
** Function name:           Notify_AncsGmailEvent
** Descriptions:            gmail消息处理
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-08-24
*****************************************************************/ 
static void Notify_AncsGmailEvent(void)
{
    /* 读取*/
    Notify_GeneralFilter();
    
    /* 拷贝数据 */
    phone_notify_s.control.recv_done = 1;
    phone_notify_s.hint.gmail = 1;
}

/****************************************************************
** Function name:           Notify_AncsOtherEvent
** Descriptions:            其他消息处理
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-08-24
*****************************************************************/ 
static void Notify_AncsOtherEvent(void)
{
    /* 读取*/
    Notify_GeneralFilter();
    
    /* 拷贝数据 */
    phone_notify_s.control.recv_done = 1;
    phone_notify_s.hint.other = 1;
}

/****************************************************************
** Function name:           Notify_AttrPrint
** Descriptions:            消息属性处理
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-08-24
*****************************************************************/ 
void Notify_AttrPrint(ble_ancs_c_evt_notif_attr_t * p_attr,
                      ble_ancs_c_attr_list_t * ancs_attr_list)
{	
    uint8_t notif_flag = 0;
    const message_enable_t *message_p = &(system_info_s.message_enable);    
    
    switch(p_attr->attr_id)
    {
    case BLE_ANCS_NOTIF_ATTR_ID_APP_IDENTIFIER:
        recv_package_flag = 0x01; break; // recv_package_flag |= 0x01; break;
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
    
//    if(recv_package_flag == 0xFF)
//    {
//        recv_package_flag = 0x00;
//    }

    /* 当所有的属性包接收完成时,处理数据 */
    if((recv_package_flag & 0x0F) == 0x0F)
    {     
        recv_package_flag = 0x00;      
        if(strcmp((const char*)m_attr_appID,(const char *)mobilePhone) == 0)
        {   //来电显示    
            if(message_p->call_enable == 0x01 && Ancs_GetBlePhone())
            {               
                Phone_NotifyInit();
                Notify_AncsCallEvent();
                notif_flag = 1;

                //数据处理完成,重新初始化
                memset(m_attr_title, 0, sizeof(m_attr_title));
                memset(m_attr_message, 0, sizeof(m_attr_message));
                 
                //唤醒UI事件处理UI推送显示
                //WJ.20180129.增加不能加入来电提醒时清除数据
                if(Gui_ThreadCanAcceptEvent(CPU_EVENT_TASK_CALL_REMIND) == 0)
                {
                    Phone_NotifyInit();
                    return;
                }
                Gui_ThreadEventTaskWakeUp(CPU_EVENT_TASK_CALL_REMIND);      
            }
            else
            {

                //数据处理完成,重新初始化
                memset(m_attr_title, 0, sizeof(m_attr_title));
                memset(m_attr_message, 0, sizeof(m_attr_message));
            }                
            return;
        }
//        else if(!Ancs_GetBlePhone())
        {    
            Phone_NotifyInit();        
            if(strcmp((const char*)m_attr_appID,(const char*)mobileSMS) == 0)
            {   
                //短信显示
                if(message_p->sms_enable == 0x01)
                {
                    Notify_AncsMessageEvent();
                    notif_flag = 1;
                }
            }
            else if(strcmp((const char*)m_attr_appID,(const char*)mobilexin) == 0)
            {
                //微信显示
                if(message_p->wechat_enable == 0x01)
                {
                    Notify_AncsWechatEvent();
                    notif_flag = 1;
                }
            }
            else if(strcmp((const char*)m_attr_appID,(const char*)mobileqq) == 0 
                   || strcmp((const char*)m_attr_appID,(const char*)mobileqq2) == 0 
                   ||strcmp((const char*)m_attr_appID,(const char*)mobileqq3) == 0)
            {
                //QQ显示
                if(message_p->qq_enable == 0x01)
                {
                    Notify_AncsQQEvent();
                    notif_flag = 1;
                }
            }
            else if(strcmp((const char*)m_attr_appID,(const char*)mobileTwitter) == 0)
            {
                //Twitter显示
                if(message_p->twitter_enable == 0x01)
                {
                    Notify_AncsTwitterEvent();
                    notif_flag = 1;
                }
            }
            else if(strcmp((const char*)m_attr_appID,(const char*)mobileFacebook) == 0)
            {   
                //Facebook显示
                if(message_p->facebook_enable == 0x01)
                {
                    Notify_AncsFacebookEvent();
                    notif_flag = 1;
                }
            }
            else if(strcmp((const char*)m_attr_appID,(const char*)mobileWhatsApp) == 0)
            {
                //Whatsapp显示
                if(message_p->whatsapp_enable == 0x01)
                {
                    Notify_AncsWhatsappEvent();
                    notif_flag = 1;
                }
            }
            else if(strcmp((const char*)m_attr_appID,(const char*)mobileLinkedin) == 0)
            {
                //linkedin显示
                if(message_p->linkedin_enable == 0x01)
                {
                    Notify_AncsLinkedinEvent();
                    notif_flag = 1;
                }
            }
            else if(strcmp((const char*)m_attr_appID,(const char*)mobileSnapchat) == 0)
            {
                //snapchat显示
                if(message_p->snapchat_enable == 0x01)
                {
                    Notify_AncsSnapchatEvent();
                    notif_flag = 1;
                }
            }
            else if(strcmp((const char*)m_attr_appID,(const char*)mobileSkype) == 0
                   || strcmp((const char*)m_attr_appID,(const char*)mobileSkype_1) == 0
                   || strcmp((const char*)m_attr_appID,(const char*)mobileSkype_2) == 0)
            {       //WJ.20180508.增加
                //skype显示
                if(message_p->skype_enable == 0x01)
                {
                    Notify_AncsSkypeEvent();
                    notif_flag = 1;
                }
            }
            else if(strcmp((const char*)m_attr_appID,(const char*)mobileInstagram) == 0)
            {
                //instagram显示
                if(message_p->instagram_enable == 0x01)
                {
                    Notify_AncsInstagramEvent();
                    notif_flag = 1;
                }
            }
            else if(strcmp((const char*)m_attr_appID,(const char*)mobileLine) == 0)
            {
                //line显示
                if(message_p->line_enable == 0x01)
                {
                    Notify_AncsLineEvent();
                    notif_flag = 1;
                }
            }
            else if(strcmp((const char*)m_attr_appID,(const char*)mobileGmail) == 0)
            {
                //Gmail显示
                if(message_p->gmail_enable == 0x01)
                {
                    Notify_AncsGmailEvent();
                    notif_flag = 1;
                }
            }
            else
            {
                //other显示
                if(message_p->other_enable == 0x01)
                {
                    Notify_AncsOtherEvent();
                    notif_flag = 1;
                }
            }
        }        
        //数据处理完成,重新初始化
        memset(m_attr_title, 0, sizeof(m_attr_title));
        memset(m_attr_message, 0, sizeof(m_attr_message));
         
        if(notif_flag == 1)
        {
            //唤醒UI事件处理UI推送显示
            //WJ.20180129.增加不能加入消息提醒时清除数据
            if(Gui_ThreadCanAcceptEvent(CPU_EVENT_TASK_MESSAGE_REMIND) == 0)
            {
                Phone_NotifyInit();
                return;
            }
            Gui_ThreadEventTaskWakeUp(CPU_EVENT_TASK_MESSAGE_REMIND);
        }
    }
}
