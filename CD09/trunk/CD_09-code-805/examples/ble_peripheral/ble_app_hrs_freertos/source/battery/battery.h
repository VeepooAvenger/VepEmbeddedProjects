/***************************Copyright(c)***********************************
**                       Veepoo Tech
**                  COPYRIGHT (C) 2016-2019
**-----------File Info-----------------------------------------------------
** File        name:     battery.h
** Last modified Date:   2017-08-21
** Last SYS_VERSION:     0.0.0
** Descriptions:            
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
#ifndef BATTERY_H__
#define BATTERY_H__

#include "stdint.h"
#include "board.h"

#define APP_BATTERY_ENABLED     1


#define POWER_NORMAL     0x00              //放电                              
#define POWER_CHARGE     0x01              //充电
#define POWER_LOW        0x02              //低电 
#define POWER_FULL       0x03              //充满

#define BATTERY_LEVEL_LOW       5          //低电等级
#define BATTERY_LEVEL_CHARGE    6          //充电等级


#define POWER_CHARGING_PIN   CHARGING_PIN       //充电中  

#define POWER_CHG_PIN        CHG_PIN            //充电完成 

//获取充满状态
uint8_t Battery_GetChargingFullState(void);
    
//设置充满状态
void Battery_SetChargingFullState(uint8_t flg);
    
//开始电池电量检测
void Battery_TestingStart(void);

//停止电池电量检测
void Battery_TestingStop(void);

//电池电量计算
void Battery_TestingCalculate(void);

//获取电池电量等级
uint8_t Battery_GetGrade(void);

//获取电池电量采样值（平均）
uint16_t Battery_GetAdcValue(void);

//电池充电状态管脚初始化
void Battery_InitPin(void);

//电池正在充电
uint8_t Battery_IsCharging(void);

//电池是否充满
uint8_t Battery_IsChargingFull(void);

//进入低电关机状态;返回0：没有，1:低电需要关机
uint8_t Battery_IsLowToShut(void);

//进入低电状态;返回0：没有，1:低电
uint8_t Battery_IsLow(void);

//用于充电清除低电关机标志 
void Battery_ClearLowFlag(void);

void Battery_SetFull(void);

#endif // BATTERY_H__

