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


//求Uint8 类型的数据平均值
uint8_t Util_AverageByUint8(uint8_t *input_data, uint16_t input_len);

//检测数组是否全为0；返回0:数组不为0，1：数组全为0
uint8_t Util_CheckBufIsZero(uint8_t *input_data, uint16_t input_len);

//排序
void Util_CommonSort(uint8_t *data, uint8_t len);

//数值转字符串
uint16_t Util_Itoa(int n, char s[]);

#endif  //_UTIL_H
