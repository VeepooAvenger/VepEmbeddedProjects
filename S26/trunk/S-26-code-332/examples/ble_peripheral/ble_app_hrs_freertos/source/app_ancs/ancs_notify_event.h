#ifndef _ANCS_NOTIFY_EVENT_H_
#define _ANCS_NOTIFY_EVENT_H_

#include "ble_ancs_c.h"

#define ATTR_DATA_SIZE            BLE_ANCS_ATTR_DATA_MAX                      /**< Allocated size for attribute data. */        

extern uint8_t m_attr_appID[32];
extern uint8_t m_attr_title[32];
extern uint8_t m_attr_subtitle[32]; 
extern uint8_t m_attr_message[ATTR_DATA_SIZE];  
extern uint8_t m_attr_message_size[32]; 
extern uint8_t m_attr_date[32];    
extern uint8_t m_attr_posaction[32];  
extern uint8_t m_attr_negaction[32]; 

void notif_attr_print(ble_ancs_c_evt_notif_attr_t*  p_attr,
                      ble_ancs_c_attr_list_t*  ancs_attr_list);

#endif
