/***************************Copyright(c)***********************************
**                       Veepoo Tech
**                  COPYRIGHT (C) 2016-2019
**-----------File Info-----------------------------------------------------
** File        name:     posture_get.h
** Last modified Date:   2017-08-21
** Last SYS_VERSION:     0.0.0
** Descriptions:         统计每两秒的姿态数据，用于睡眠分析   
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

//姿态统计，每两秒统计一次
void Posture_Statistical(void);

//获取当前统计的出现次数最多姿态的类型和次数，保存到五分钟数据中
void Posture_StatisticalGet(uint8_t *max_type, uint8_t *max_cnt);

//清除当前统计的数据
void Posture_StatisticalInit(void);

//获取每秒的姿态值
uint8_t Posture_GetSecPosture(void);

#endif //APP_POSTURE_GET_H__

