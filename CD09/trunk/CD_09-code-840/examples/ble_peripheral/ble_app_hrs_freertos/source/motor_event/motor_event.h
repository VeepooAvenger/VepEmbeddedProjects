/***************************Copyright(c)***********************************
**                       Veepoo Tech
**                  COPYRIGHT (C) 2016-2019
**-----------File Info-----------------------------------------------------
** File        name:     motor_event.h
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
#ifndef MOTOR_EVENT_H
#define MOTOR_EVENT_H

#include "stdint.h"

#define APP_MOTOR_ENABLED   1
 
#pragma pack(1) 
//振动两组 每组振动两秒 每组间隔两秒
typedef struct
{ 
  uint8_t flag;//电机开始标志
  uint8_t continuous;//持续振动还是分组振动
  uint8_t sustain;//电机持续时间
  uint8_t sustain_cnt;
  uint8_t interval;//电机时间间隔
  uint8_t interval_cnt;
  uint8_t group;//电机振动组数
  uint8_t update;//电机更新
}motor_event_t;  
#pragma pack() 

//初始化
void Motor_Init(void);

//马达事件监听
void Motor_EventRun(void);

//删除马达事件
void Motor_DelEvent(void);

//添加马达事件
void Motor_AddEvent(uint8_t group,uint8_t continuous,uint8_t sustain_time,uint8_t interval);

//马达开启
void Motor_PowerOn(void);

//马达关闭
void Motor_PowerOFF(void);

//马达是否工作
uint8_t Motor_IsWork(void);

#endif

