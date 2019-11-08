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


//ancs 初始化
void Ancs_Init(void);

//蓝牙事件处理
void Ancs_OnBleEvt(ble_evt_t * p_ble_evt);

//发现服务
void Ancs_BleDbDiscoveryStart(pm_evt_t const * p_evt);

//服务事件回复
void Ancs_BleDbDiscoveryOnBleEvt(ble_evt_t * p_ble_evt);

//绑定
void Ancs_Bonded(pm_evt_t const * p_evt);

//删除绑定
void Ancs_DisBonded(void);

//设置peer id
void Ancs_SetPeerId(pm_peer_id_t peer_id);

//ancs 连接断开
void Ancs_Disconnected(ble_evt_t * p_ble_evt);

//ancs设置广播
void Ancs_SetAdvertising(void);

//创建ancs定时器
void Ancs_TimerCreate(void);

//ancs 开始
void Ancs_Start(void);

//设置来电状态标志
uint8_t Ancs_GetBlePhone(void);

//设置来电状态标志
void Ancs_SetBlePhone(uint8_t phone_flag);

//来电挂断操作
uint8_t Ancs_OperationCalls(uint8_t type);

//清除消息id记录
void App_AncsClearLastCategoryId(void);
#endif  //APP_ANCS_ENABLED

#endif




