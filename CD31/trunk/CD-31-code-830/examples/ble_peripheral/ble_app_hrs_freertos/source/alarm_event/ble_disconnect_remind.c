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
//20180612.�޸����������߼�����������ֻ����һ�Σ����ٶ�ʱ����
***************************************************************************/
#include <string.h>
#include "ble_disconnect_remind.h"
#include "gui_thread.h"
#include "app_type.h"

#define REMIND_BLE_DISC_TIME    (600)   //10����

static ble_disconnect_remind_t  ble_disconnect_remind_s = {0x00,};

extern system_info_t system_info_s;            //ϵͳ��Ϣ
extern system_switch_t system_switch_s;        //ϵͳ����

/****************************************************************
** Function name:           Remind_BleDisconnectDisPlay
** Descriptions:            ����������������UI
** input parameters:        remind_switch:0�����ѡ�1���ѣ�
                            boot:0�ػ���1������
                            ble_connet��0�����Ͽ���1�������ӡ�
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2018-04-03
*****************************************************************/ 
void Remind_BleDisconnectDisPlay(uint8_t remind_switch, uint8_t boot, uint8_t ble_connet)
{
    if(remind_switch != 1 || boot != 1 || ble_connet != 0)
    {   //���ѿ��عرա��ػ�������������״̬��
        return;
    }
    //����UI����ʱ��ɹ���������
    if(Gui_ThreadCanAcceptEvent(CPU_EVENT_TASK_BLE_DISC_REMIND) == 1)
    {
        Gui_ThreadEventTaskWakeUp(CPU_EVENT_TASK_BLE_DISC_REMIND);
//        ble_disconnect_remind_s.remdind_failed = 0;
//        ble_disconnect_remind_s.remdind_flag = 1;
//        ble_disconnect_remind_s.remdind_time = 0;
        //WJ.20180612.�޸������������ѣ�ֻ����һ��
        memset(&ble_disconnect_remind_s, 0, sizeof(ble_disconnect_remind_s));
    }
    else
    {   //����ʧ�ܣ���һ����������
        ble_disconnect_remind_s.remdind_failed = 1;
    }
}

/****************************************************************
** Function name:           Remind_BleDisconnectMonitor
** Descriptions:            ���������������Ѽ���
** input parameters:        remind_switch:0�����ѡ�1���ѣ�
                            boot:0�ػ���1������
                            ble_connet��0�����Ͽ���1�������ӡ�
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2018-04-03
*****************************************************************/ 
void Remind_BleDisconnectMonitor(uint8_t remind_switch, uint8_t boot, uint8_t ble_connet)
{
    if(remind_switch != 1 || boot != 1 || ble_connet != 0)
    {   //���ѿ��عرա��ػ�������������״̬��
        return;
    }    
    //����ʧ���ٴ�����
    if(ble_disconnect_remind_s.remdind_failed == 1)
    {
        Remind_BleDisconnectDisPlay(remind_switch, boot, ble_connet);
        return;
    }
    //WJ.20180612.�޸������������ѣ�ֻ����һ��
//    //��ʱ����
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
** Descriptions:            ���ò�����ɾ������
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2018-04-03
*****************************************************************/ 
void Remind_BleDisconnectRemove(void)
{
    //���ò���
    memset(&ble_disconnect_remind_s, 0, sizeof(ble_disconnect_remind_s)); 
//    //�����ǰ����������������״̬�£���Ҫ�ر�UI��ʾ    
//    if(Gui_GetThreadJob() == CPU_EVENT_TASK_BLE_DISC_REMIND)
//    {
//        Gui_ThreadEventTaskWakeUp(CPU_EVENT_TASK_ALL_EXIT);
//    }
}


