/***************************Copyright(c)***********************************
**                       Veepoo Tech
**                  COPYRIGHT (C) 2016-2019
**-----------File Info-----------------------------------------------------
** File        name:     afe4900_driver.h
** Last modified Date:   2018-10-18
** Last SYS_VERSION:     0.0.0
** Descriptions:            
**-------------------------------------------------------------------------
** Created  by:          JSH
** Created  date:        2018-10-18
** SYS_VERSION:          0.0.0
** Descriptions:         
**-------------------------------------------------------------------------
** Modified by:				
** Modified date:  			          
** SYS_VERSION:                 
** Descriptions:                        
***************************************************************************/

#ifndef __AFE4900_DRIVER_H
#define __AFE4900_DRIVER_H

#include<stdint.h>


// afe4900 fifo��ȡ��ת��
void Afe4900_FifoReadConvert(uint8_t *data, uint32_t sample_cnt);

// afe4900 fifo���ݻ�ȡ
void Afe4900_FifoDateGet(int32_t *data_src, uint32_t sample_types, uint32_t sample_index, int32_t *data_get, uint32_t sample_cnt);

// afe4900 ���Ĵ�����ת��
int32_t Afe4900_RegisterReadConvert(uint8_t reg_address);

// afe4900 ��ʼ����
void Afe4900_StartSampling(void);

// afe4900 ֹͣ����
void Afe4900_StopSampling(void);

// afe4900 ����PPGģʽ-����
void Afe4900_Ppg1Config(void);

// afe4900 ����PPGģʽ-Ѫ��
void Afe4900_Ppg2Config(void);

void Afe4900_PpgRestConfig(void);

// afe4900 �ϵ��ʼ��
void Afe4900_PowerOnInit(void);

#endif

