/***************************Copyright(c)***********************************
**                       Veepoo Tech
**                  COPYRIGHT (C) 2016-2019
**-----------File Info-----------------------------------------------------
** File        name:     tp_cal_data.h
** Last modified Date:   2018-03-23
** Last SYS_VERSION:     0.0.0
** Descriptions:            
**-------------------------------------------------------------------------
** Created  by:          YX
** Created  date:        2018-03-23
** SYS_VERSION:          0.0.0
** Descriptions:         
**-------------------------------------------------------------------------
** Modified by:				
** Modified date:  			          
** SYS_VERSION:                 
** Descriptions:                        
***************************************************************************/

#ifndef TP_CAL_DATA_H__
#define TP_CAL_DATA_H__

#include "tp_gesture.h"

//����������ľ���ֵ
uint32_t Tp_SubAbs(uint32_t a, uint32_t b);

//���������ֱ�߾���
uint32_t Tp_CalTwoPointDist(coord_info_t a, coord_info_t b);

//�����������е��ֱ�߾���
uint32_t Tp_CalTwoDataDist(TP_COORD_DATA_T a, TP_COORD_DATA_T b);

//�Ƚ�����Ĵ�С
uint8_t Tp_CompareTwoDataSize(uint32_t a, uint32_t b);

//�Ƚϵ��Ƿ��ڷ���������
uint8_t Tp_ComparePointInSquareArea(TP_COORD_DATA_T point, area_square_info_t area_s);

//�Ƚϵ��Ƿ���Բ��������
uint8_t Tp_ComparePointInCircleArea(TP_COORD_DATA_T point, area_circle_info_t area_s);

#endif
