/***************************Copyright(c)***********************************
**                       Veepoo Tech
**                  COPYRIGHT (C) 2016-2019
**-----------File Info-----------------------------------------------------
** File        name:     util.h
** Last modified Date:   2017-08-01
** Last SYS_VERSION:     0.0.0
** Descriptions:            
**-------------------------------------------------------------------------
** Created  by:          WJ
** Created  date:        2017-08-01
** SYS_VERSION:          0.0.0
** Descriptions:         
**-------------------------------------------------------------------------
** Modified by:				
** Modified date:  			          
** SYS_VERSION:                 
** Descriptions:                        
***************************************************************************/

#ifndef _UTIL_H
#define _UTIL_H


//��Uint8 ���͵�����ƽ��ֵ
uint8_t Util_AverageByUint8(uint8_t *input_data, uint16_t input_len);

//��������Ƿ�ȫΪ0������0:���鲻Ϊ0��1������ȫΪ0
uint8_t Util_CheckBufIsZero(uint8_t *input_data, uint16_t input_len);

//����
void Util_CommonSort(uint8_t *data, uint8_t len);

//��ֵת�ַ���
uint16_t Util_Itoa(int n, char s[]);

#endif  //_UTIL_H
