/***************************Copyright(c)***********************************
**                       Veepoo Tech
**                  COPYRIGHT (C) 2016-2019
**-----------File Info-----------------------------------------------------
** File        name:     app_ancs.h
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
#ifndef __ANCS_DOUP_H
#define __ANCS_DOUP_H

#define APP_ANCS_ENABLED 1

#if APP_ANCS_ENABLED
#include "ble_ancs_c.h"
#include "peer_manager.h"


//ancs ��ʼ��
void Ancs_Init(void);

//�����¼�����
void Ancs_OnBleEvt(ble_evt_t * p_ble_evt);

//���ַ���
void Ancs_BleDbDiscoveryStart(pm_evt_t const * p_evt);

//�����¼��ظ�
void Ancs_BleDbDiscoveryOnBleEvt(ble_evt_t * p_ble_evt);

//��
void Ancs_Bonded(pm_evt_t const * p_evt);

//ɾ����
void Ancs_DisBonded(void);

//����peer id
void Ancs_SetPeerId(pm_peer_id_t peer_id);

//ancs ���ӶϿ�
void Ancs_Disconnected(ble_evt_t * p_ble_evt);

//ancs���ù㲥
void Ancs_SetAdvertising(void);

//����ancs��ʱ��
void Ancs_TimerCreate(void);

//ancs ��ʼ
void Ancs_Start(void);

//��������״̬��־
uint8_t Ancs_GetBlePhone(void);

//��������״̬��־
void Ancs_SetBlePhone(uint8_t phone_flag);

//����Ҷϲ���
uint8_t Ancs_OperationCalls(uint8_t type);

//�����Ϣid��¼
void App_AncsClearLastCategoryId(void);
#endif  //APP_ANCS_ENABLED

#endif




