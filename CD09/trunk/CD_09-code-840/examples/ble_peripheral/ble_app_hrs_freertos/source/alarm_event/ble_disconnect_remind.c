/***************************Copyright(c)***********************************
**                       Veepoo Tech
**                  COPYRIGHT (C) 2016-2019
**-----------File Info-----------------------------------------------------
** File        name:     ble_disconnect_remind.c
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
//20180612.修改蓝牙断连逻辑，蓝牙断连只提醒一次，不再定时提醒
***************************************************************************/
#include <string.h>
#include "ble_disconnect_remind.h"
#include "gui_thread.h"
#include "app_type.h"

#define REMIND_BLE_DISC_TIME    (600)   //10分钟

static ble_disconnect_remind_t  ble_disconnect_remind_s = {0x00,};

extern system_info_t system_info_s;            //系统信息
extern system_switch_t system_switch_s;        //系统开关

/****************************************************************
** Function name:           Remind_BleDisconnectDisPlay
** Descriptions:            加入蓝牙断连提醒UI
** input parameters:        remind_switch:0不提醒、1提醒；
                            boot:0关机、1开机；
                            ble_connet：0蓝牙断开、1蓝牙连接、
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2018-04-03
*****************************************************************/ 
void Remind_BleDisconnectDisPlay(uint8_t remind_switch, uint8_t boot, uint8_t ble_connet)
{
    if(remind_switch != 1 || boot != 1 || ble_connet != 0)
    {   //提醒开关关闭、关机或者蓝牙连接状态下
        return;
    }
    //加入UI提醒时间成功，则提醒
    if(Gui_ThreadCanAcceptEvent(CPU_EVENT_TASK_BLE_DISC_REMIND) == 1)
    {
        Gui_ThreadEventTaskWakeUp(CPU_EVENT_TASK_BLE_DISC_REMIND);
//        ble_disconnect_remind_s.remdind_failed = 0;
//        ble_disconnect_remind_s.remdind_flag = 1;
//        ble_disconnect_remind_s.remdind_time = 0;
        //WJ.20180612.修改蓝牙断连提醒，只提醒一次
        memset(&ble_disconnect_remind_s, 0, sizeof(ble_disconnect_remind_s));
    }
    else
    {   //提醒失败，下一秒重新提醒
        ble_disconnect_remind_s.remdind_failed = 1;
    }
}

/****************************************************************
** Function name:           Remind_BleDisconnectMonitor
** Descriptions:            加入蓝牙断连提醒监听
** input parameters:        remind_switch:0不提醒、1提醒；
                            boot:0关机、1开机；
                            ble_connet：0蓝牙断开、1蓝牙连接、
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2018-04-03
*****************************************************************/ 
void Remind_BleDisconnectMonitor(uint8_t remind_switch, uint8_t boot, uint8_t ble_connet)
{
    if(remind_switch != 1 || boot != 1 || ble_connet != 0)
    {   //提醒开关关闭、关机或者蓝牙连接状态下
        return;
    }    
    //提醒失败再次提醒
    if(ble_disconnect_remind_s.remdind_failed == 1)
    {
        Remind_BleDisconnectDisPlay(remind_switch, boot, ble_connet);
        return;
    }
    //WJ.20180612.修改蓝牙断连提醒，只提醒一次
//    //定时提醒
//    if(ble_disconnect_remind_s.remdind_flag == 1)
//    {
//        ble_disconnect_remind_s.remdind_time++;
//        if(ble_disconnect_remind_s.remdind_time >= REMIND_BLE_DISC_TIME)
//        {
//            Remind_BleDisconnectDisPlay(remind_switch, boot, ble_connet);
//            return;
//        }
//    } 
}

/****************************************************************
** Function name:           Remind_BleDisconnectRemove
** Descriptions:            重置参数，删除提醒
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2018-04-03
*****************************************************************/ 
void Remind_BleDisconnectRemove(void)
{
    //重置参数
    memset(&ble_disconnect_remind_s, 0, sizeof(ble_disconnect_remind_s)); 
//    //如果当前处于蓝牙断连提醒状态下，需要关闭UI显示    
//    if(Gui_GetThreadJob() == CPU_EVENT_TASK_BLE_DISC_REMIND)
//    {
//        Gui_ThreadEventTaskWakeUp(CPU_EVENT_TASK_ALL_EXIT);
//    }
}


