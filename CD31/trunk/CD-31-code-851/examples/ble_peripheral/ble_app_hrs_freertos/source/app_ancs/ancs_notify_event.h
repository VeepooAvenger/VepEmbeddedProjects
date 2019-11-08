/***************************Copyright(c)***********************************
**                       Veepoo Tech
**                  COPYRIGHT (C) 2016-2019
**-----------File Info-----------------------------------------------------
** File        name:     ancs_notify_event.h
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

#ifndef _ANCS_NOTIFY_EVENT_H_
#define _ANCS_NOTIFY_EVENT_H_

#include "ble_ancs_c.h"

#define ATTR_DATA_SIZE            64//BLE_ANCS_ATTR_DATA_MAX                      /**< Allocated size for attribute data. */		
#define ATTR_MESSAGE_DATA_SIZE    300

extern uint8_t m_attr_appID[ATTR_DATA_SIZE];
extern uint8_t m_attr_title[ATTR_DATA_SIZE];
extern uint8_t m_attr_subtitle[ATTR_DATA_SIZE]; 
extern uint8_t m_attr_message[ATTR_MESSAGE_DATA_SIZE];  
extern uint8_t m_attr_message_size[4]; 
extern uint8_t m_attr_date[ATTR_DATA_SIZE];    
extern uint8_t m_attr_posaction[ATTR_DATA_SIZE];  
extern uint8_t m_attr_negaction[ATTR_DATA_SIZE]; 

//消息属性处理
void Notify_AttrPrint(ble_ancs_c_evt_notif_attr_t * p_attr,
                      ble_ancs_c_attr_list_t * ancs_attr_list);

#endif
