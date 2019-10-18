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

//����������
void Wear_DectInit(void);

//�������
uint8_t Wear_DectCalculate(void);

//��ȡ�����־
uint8_t Wear_GetWearFlag(void);

//�ر�������
void Wear_Close(void);

//����ADC��APP
uint8_t Wear_BleSendADC(void);

//�Ƿ��ڽ���������
uint8_t Wear_DectGetState(void);

//��������־
void Wear_ClearWearFlag(void);

// �����⿪��״̬�仯��������������
void Wear_SetWearSwitchState(void);
#endif
