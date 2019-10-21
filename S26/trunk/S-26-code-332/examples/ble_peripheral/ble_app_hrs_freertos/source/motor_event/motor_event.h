#ifndef MOTOR_EVENT_H
#define MOTOR_EVENT_H

#include "stdint.h"
 
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
void MotorEventRun(void);//一秒执行
void MotorDelEvent(void);//删除
void MotorAddEvent(uint8_t group,uint8_t continuous,uint8_t sustain_time,uint8_t interval);//增加电机事件 
motor_event_t *MotorGetData(void);//获取马达结构体

void MotorPowerOn(void);
void MotorPowerOFF(void);
void MotorDelEvent(void);

#endif

