/***************************Copyright(c)***********************************
**                       Veepoo Tech
**                  COPYRIGHT (C) 2016-2019
**-----------File Info-----------------------------------------------------
** File        name:     phone_notify_roll_event.c
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
#include "phone_notify_roll_event.h"
#include "phone_notify_event.h"
#include "gui_font_lattice.h"
#include "gui_page_display.h"


unread_message_t  unread_message_s;

/****************************************************************
** Function name:           Message_ExistUnread
** Descriptions:            是否存在未读信息
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-08-24
*****************************************************************/ 
bool Message_ExistUnread(void)
{
    if(Message_IsReading() &&  unread_message_s.current_page_index >= unread_message_s.total_page_count )
    {
        Message_Clear();
    }
    return (unread_message_s.current_page_index < unread_message_s.total_page_count );
}

/****************************************************************
** Function name:           Message_IsReading
** Descriptions:            正在查看信息
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-08-24
*****************************************************************/ 
bool Message_IsReading(void)
{
    return (unread_message_s.reading == 0x01);
}

/****************************************************************
** Function name:           Message_Clear
** Descriptions:            清除消息
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-08-24
*****************************************************************/ 
void Message_Clear(void)
{
    memset(&unread_message_s, 0x00, sizeof(unread_message_t));    
}

/****************************************************************
** Function name:           Message_NextUnreadMessageHint
** Descriptions:            消息翻页
** input parameters:        reading_flag：是否正在读取
** output parameters:       无
** Returned value:          0：没有消息了；1：还有消息
** Created by:              WJ              
** Created Date:            2017-08-24
*****************************************************************/ 
uint8_t Message_NextUnreadMessageHint(uint8_t reading_flag)
{    
    if(!reading_flag && unread_message_s.reading == 0)
    {
        unread_message_s.current_page_index = 1;
        
        unread_message_s.content_read_index = 0;  
    }
    else
    {
        if(unread_message_s.current_page_index == 0) 
        {
            unread_message_s.current_page_index = 1;
        }
        if(unread_message_s.total_page_count <= 1)
        {
            return 0;
        }
        unread_message_s.show_msg.title = NULL;
        unread_message_s.show_msg.title_len = 0; 
        
        unread_message_s.content_read_index = unread_message_s.read_page_len[unread_message_s.current_page_index];  
        unread_message_s.show_msg.content = &unread_message_s.msg.content[unread_message_s.content_read_index];
        unread_message_s.show_msg.content_len  = unread_message_s.read_page_len[unread_message_s.current_page_index+1] - 
                    unread_message_s.read_page_len[unread_message_s.current_page_index];
        
        unread_message_s.current_page_index++;
    }
    unread_message_s.enable = 0x01; 
    unread_message_s.reading = 0x01;
    return 1;
}

/****************************************************************
** Function name:           Message_Mark
** Descriptions:            消息解析
** input parameters:        type：消息类型
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-08-24
*****************************************************************/ 
void Message_Mark(msg_type_e type)
{
    //清除数据
    Message_Clear();
    
    /* 分析数据 */
    unread_message_s.enable = 0x00;
    unread_message_s.reading = 0x00;
    unread_message_s.title_read_index = 0x00;
    unread_message_s.content_read_index = 0x00;
    unread_message_s.current_page_index = 0x00;
    unread_message_s.total_page_count = 0;    
    if(type != MSG_TYPE_CALL )
    {    //YX.20170603.让电话通知之外的其他手机通知的title可以显示到定义的最大长度
         //将title加入到content中
        phone_notify_s.msg.title_len = 
                (phone_notify_s.msg.title_len>TLTLE_DATA_MAX_LEN)?TLTLE_DATA_MAX_LEN:phone_notify_s.msg.title_len;
        phone_notify_s.msg.title_len = Font_FullScreenTotalActiveLenght(phone_notify_s.msg.title , phone_notify_s.msg.title_len);
        memcpy(unread_message_s.msg.content, phone_notify_s.msg.title, phone_notify_s.msg.title_len);
        if(phone_notify_s.msg.title_len != 0)
        {
            unread_message_s.msg.content[phone_notify_s.msg.title_len++] = 0x3A;//0x3A：冒号
        }
        phone_notify_s.msg.content_len = phone_notify_s.msg.title_len + phone_notify_s.msg.content_len;
        unread_message_s.msg.content_len = 
                (phone_notify_s.msg.content_len>CONTENT_DATA_MAX_LEN)?CONTENT_DATA_MAX_LEN:phone_notify_s.msg.content_len;
        memcpy(&unread_message_s.msg.content[phone_notify_s.msg.title_len], phone_notify_s.msg.content, unread_message_s.msg.content_len-phone_notify_s.msg.title_len);       
    }
    else
    {
        unread_message_s.msg.content_len = 
                (phone_notify_s.msg.content_len>CONTENT_DATA_MAX_LEN)?CONTENT_DATA_MAX_LEN:phone_notify_s.msg.content_len;
        memcpy(unread_message_s.msg.content, phone_notify_s.msg.content, unread_message_s.msg.content_len);
        unread_message_s.msg.title_len = 
                (phone_notify_s.msg.title_len>TLTLE_DATA_MAX_LEN)?TLTLE_DATA_MAX_LEN:phone_notify_s.msg.title_len;    
        memcpy(unread_message_s.msg.title, phone_notify_s.msg.title, unread_message_s.msg.title_len);
    }
    unread_message_s.msg_type = type;
    unread_message_s.read_page_len[0] = 0;
    unread_message_s.total_page_count = Font_FullScreenTotalCount2X(unread_message_s.msg.title,
            unread_message_s.msg.title_len,
            unread_message_s.msg.content,
            unread_message_s.msg.content_len,        
            unread_message_s.read_page_len);
            
    unread_message_s.show_msg.content = unread_message_s.msg.content;

    unread_message_s.show_msg.content_len = unread_message_s.read_page_len[1];

    unread_message_s.show_msg.title = unread_message_s.msg.title;
    unread_message_s.show_msg.title_len = unread_message_s.msg.title_len;    
    
    if(MSG_TYPE_CALL == type)
    {
        unread_message_s.current_page_index = unread_message_s.total_page_count;
    }
}
