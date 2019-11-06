/*****************************************************************
Copyright:      2012-2016, Veepoo Tech. Co., Ltd.
File name:         app_ancs.h
Description: 
Author:         SQH
Version: 
Date:           2016-11-03
History: 
*****************************************************************/
#ifndef __ANCS_DOUP_H
#define __ANCS_DOUP_H

#include "ble_ancs_c.h"
#include "peer_manager.h"

void App_Ancs_Init(void);
void App_Ancs_On_Ble_Evt(ble_evt_t * p_ble_evt);

void App_Ancs_Ble_Db_Discovery_Start(pm_evt_t const * p_evt);
void App_Ancs_Ble_Db_Discovery_On_Ble_Evt(ble_evt_t * p_ble_evt);
void App_Ancs_Bonded(pm_evt_t const * p_evt);
void App_Ancs_DisBonded(void);

void App_Ancs_Set_Peer_Id(pm_peer_id_t peer_id);
void App_Ancs_Disconnected(ble_evt_t * p_ble_evt);
uint8_t App_AncsOperationCalls(void);
void App_Ancs_Advertising_Set(void);

void App_Ancs_Create_Time(void);
void App_Ancs_Start(void);

uint8_t App_Ancs_BlePhoneGet(void);
void App_Ancs_BlePhoneSet(uint8_t phone_flag);

#endif




