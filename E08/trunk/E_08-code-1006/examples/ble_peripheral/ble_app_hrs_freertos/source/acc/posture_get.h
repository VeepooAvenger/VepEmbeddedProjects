/***************************Copyright(c)***********************************
**                       Veepoo Tech
**                  COPYRIGHT (C) 2016-2019
**-----------File Info-----------------------------------------------------
** File        name:     posture_get.h
** Last modified Date:   2017-08-21
** Last SYS_VERSION:     0.0.0
** Descriptions:         ͳ��ÿ�������̬���ݣ�����˯�߷���   
**-------------------------------------------------------------------------
** Created  by:          WJ
** Created  date:        2017-08-21
** SYS_VERSION:          0.0.0
** Descriptions:         
**-------------------------------------------------------------------------
** Modified by:				
** Modified date:  			          
** SYS_VERSION:                 
** Descriptions:                        
***************************************************************************/
#ifndef APP_POSTURE_GET_H__
#define APP_POSTURE_GET_H__

#include "string.h"

//��̬ͳ�ƣ�ÿ����ͳ��һ��
void Posture_Statistical(void);

//��ȡ��ǰͳ�Ƶĳ��ִ��������̬�����ͺʹ��������浽�����������
void Posture_StatisticalGet(uint8_t *max_type, uint8_t *max_cnt);

//�����ǰͳ�Ƶ�����
void Posture_StatisticalInit(void);

//��ȡÿ�����ֵ̬
uint8_t Posture_GetSecPosture(void);

#endif //APP_POSTURE_GET_H__

