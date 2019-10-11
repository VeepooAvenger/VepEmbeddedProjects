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

#define BP_CALC_MAX_CNT    19 //血压计算的最大次数


typedef enum
{
    BP_WORK_MODE_IDLE = 0,
    BP_WORK_MODE_WAITE,
    BP_WORK_MODE_BUSY
}bp_work_mode_t;

// 获取测试状态
bp_work_mode_t Bp_GetWorkMode(void);

//设置测试状态
void Bp_SetWorkMode(bp_work_mode_t work_mode);

//测量状态忙碌中
uint8_t Bp_IsWorkBusy(void);

//获取血压
test_mode_t *Bp_GetBpMode(void);

//设置血压测量模式
uint8_t Bp_SetBpMode(uint8_t test_mode);

//打开血压测量
void Bp_Open(void);

//关闭血压测量
void Bp_Close(void);

//计算血压
void Bp_CalculateProcess(void);

//测量无效发送数据到app
void Bp_BleSendTestInvalid(void);

//发送测量结果到app
void Bp_BleSendTestResultValue(void);

//获取测量状态
uint8_t Bp_GetFlagState(void);

//获取测量血压值
void Bp_GetTestResult(uint8_t *sp, uint8_t *dp);

#endif
