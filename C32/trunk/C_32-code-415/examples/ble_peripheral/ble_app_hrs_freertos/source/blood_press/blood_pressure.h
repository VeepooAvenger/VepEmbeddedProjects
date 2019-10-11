/***************************Copyright(c)***********************************
**                       Veepoo Tech
**                  COPYRIGHT (C) 2016-2019
**-----------File Info-----------------------------------------------------
** File        name:     blood_pressure.h
** Last modified Date:   2017-07-28
** Last SYS_VERSION:     0.0.0
** Descriptions:            
**-------------------------------------------------------------------------
** Created  by:          WJ
** Created  date:        2017-07-28
** SYS_VERSION:          0.0.0
** Descriptions:         
**-------------------------------------------------------------------------
** Modified by:				
** Modified date:  			          
** SYS_VERSION:                 
** Descriptions:                        
***************************************************************************/
#ifndef __BLOOD_PRESSURE_H
#define __BLOOD_PRESSURE_H

#include "vph_bp.h"

#define BP_CALC_MAX_CNT    19 //Ѫѹ�����������


typedef enum
{
    BP_WORK_MODE_IDLE = 0,
    BP_WORK_MODE_WAITE,
    BP_WORK_MODE_BUSY
}bp_work_mode_t;

// ��ȡ����״̬
bp_work_mode_t Bp_GetWorkMode(void);

//���ò���״̬
void Bp_SetWorkMode(bp_work_mode_t work_mode);

//����״̬æµ��
uint8_t Bp_IsWorkBusy(void);

//��ȡѪѹ
test_mode_t *Bp_GetBpMode(void);

//����Ѫѹ����ģʽ
uint8_t Bp_SetBpMode(uint8_t test_mode);

//��Ѫѹ����
void Bp_Open(void);

//�ر�Ѫѹ����
void Bp_Close(void);

//����Ѫѹ
void Bp_CalculateProcess(void);

//������Ч�������ݵ�app
void Bp_BleSendTestInvalid(void);

//���Ͳ��������app
void Bp_BleSendTestResultValue(void);

//��ȡ����״̬
uint8_t Bp_GetFlagState(void);

//��ȡ����Ѫѹֵ
void Bp_GetTestResult(uint8_t *sp, uint8_t *dp);

#endif
