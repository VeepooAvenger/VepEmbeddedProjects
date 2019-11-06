/***************************Copyright(c)***********************************
**                       Veepoo Tech
**                  COPYRIGHT (C) 2016-2019
**-----------File Info-----------------------------------------------------
** File        name:     ble_disconnect_remind.h
** Last modified Date:   2018-04-03
** Last SYS_VERSION:     0.0.0
** Descriptions:            
**-------------------------------------------------------------------------
** Created  by:          WJ
** Created  date:        2018-04-03
** SYS_VERSION:          0.0.0
** Descriptions:         
**-------------------------------------------------------------------------
** Modified by:				
** Modified date:  			          
** SYS_VERSION:                 
** Descriptions:                        
***************************************************************************/

#ifndef _BLE_DISCONNECT_REMIND_H_
#define _BLE_DISCONNECT_REMIND_H_

#include <stdint.h>

typedef struct 
{
    uint8_t     remdind_flag;       //蓝牙断开提醒标志
    uint8_t     remdind_failed;     //提醒失败，下一秒提醒
    uint16_t    remdind_time;       //上次断连时间计时，计时单位1s    
}ble_disconnect_remind_t;


//加入蓝牙断连提醒UI
//remind_switch:0不提醒、1提醒；
//boot:0关机、1开机；
//ble_connet：0蓝牙断开、1蓝牙连接
void Remind_BleDisconnectDisPlay(uint8_t remind_switch, uint8_t boot, uint8_t ble_connet);

//加入蓝牙断连提醒监听 
//remind_switch:0不提醒、1提醒；
//boot:0关机、1开机；
//ble_connet：0蓝牙断开、1蓝牙连接
void Remind_BleDisconnectMonitor(uint8_t remind_switch, uint8_t boot, uint8_t ble_connet);

//重置参数，删除提醒 
void Remind_BleDisconnectRemove(void); 

#endif


