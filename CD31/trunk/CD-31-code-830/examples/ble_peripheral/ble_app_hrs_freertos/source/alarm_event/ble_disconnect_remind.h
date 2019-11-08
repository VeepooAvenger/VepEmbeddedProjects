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
    uint8_t     remdind_flag;       //�����Ͽ����ѱ�־
    uint8_t     remdind_failed;     //����ʧ�ܣ���һ������
    uint16_t    remdind_time;       //�ϴζ���ʱ���ʱ����ʱ��λ1s    
}ble_disconnect_remind_t;


//����������������UI
//remind_switch:0�����ѡ�1���ѣ�
//boot:0�ػ���1������
//ble_connet��0�����Ͽ���1��������
void Remind_BleDisconnectDisPlay(uint8_t remind_switch, uint8_t boot, uint8_t ble_connet);

//���������������Ѽ��� 
//remind_switch:0�����ѡ�1���ѣ�
//boot:0�ػ���1������
//ble_connet��0�����Ͽ���1��������
void Remind_BleDisconnectMonitor(uint8_t remind_switch, uint8_t boot, uint8_t ble_connet);

//���ò�����ɾ������ 
void Remind_BleDisconnectRemove(void); 

#endif


