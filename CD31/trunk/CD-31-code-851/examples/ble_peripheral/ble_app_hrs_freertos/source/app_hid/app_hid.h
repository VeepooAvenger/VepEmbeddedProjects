/***************************Copyright(c)***********************************
**                       Veepoo Tech
**                  COPYRIGHT (C) 2016-2019
**-----------File Info-----------------------------------------------------
** File        name:     app_hid.h
** Last modified Date:   2017-08-01
** Last SYS_VERSION:     0.0.0
** Descriptions:            
**-------------------------------------------------------------------------
** Created  by:          YX
** Created  date:        2017-08-01
** SYS_VERSION:          0.0.0
** Descriptions:         
**-------------------------------------------------------------------------
** Modified by:				
** Modified date:  			          
** SYS_VERSION:                 
** Descriptions:                        
***************************************************************************/
#ifndef APP_HID_H__
#define APP_HID_H__

#include "peer_manager.h"

#define APP_HID_ENABLED 1

#if APP_HID_ENABLED

//Function for receiving the Application's HID BLE Stack events.
void Hid_OnBleEvt(ble_evt_t * p_ble_evt);
//Function for initializing HID Service
void Hid_Init(void);
//hid开始暂停命令
void Hid_AudioStartOrStop(void);
//hid下一首歌命令
void Hid_AudioNext(void);
//hid上一首歌命令
void Hid_AudioLast(void);
//hid音量+命令
void Hid_AudioVolumeAdd(void);
//hid音量-命令
void Hid_AudioVolumeSub(void);

#endif
 
#endif  //APP_HID_H__
