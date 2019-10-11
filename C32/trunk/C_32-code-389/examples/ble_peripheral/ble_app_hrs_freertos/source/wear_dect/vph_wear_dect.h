/***************************Copyright(c)***********************************
**                       Veepoo Tech
**                  COPYRIGHT (C) 2016-2019
**-----------File Info-----------------------------------------------------
** File        name:     vph_wear_dect.hs
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
#ifndef __VPH_WEAR_DECT_H_
#define __VPH_WEAR_DECT_H_

#include "stdint.h"

//开启佩戴检测
void Wear_DectInit(void);

//佩戴计算
uint8_t Wear_DectCalculate(void);

//获取佩戴标志
uint8_t Wear_GetWearFlag(void);

//关闭佩戴检测
void Wear_Close(void);

//发送ADC到APP
uint8_t Wear_BleSendADC(void);

//是否在进行佩戴检测
uint8_t Wear_DectGetState(void);

//清除佩戴标志
void Wear_ClearWearFlag(void);

// 佩戴检测开关状态变化，不再做佩戴检测
void Wear_SetWearSwitchState(void);
#endif
