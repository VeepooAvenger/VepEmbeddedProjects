/***************************Copyright(c)***********************************
**                       Veepoo Tech
**                  COPYRIGHT (C) 2016-2019
**-----------File Info-----------------------------------------------------
** File        name:     timer_manage.h
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
#ifndef TIMER_MANAGE_H
#define TIMER_MANAGE_H

#include "FreeRTOS.h"
#include "timers.h"
#include "app_timer.h"

#include "app_type.h"


#define APP_TIMER_PRESCALER              0                           /**< Value of the RTC1 PRESCALER register. */
#define OSTIMER_WAIT_FOR_QUEUE           15 

#define APP_TIMER_OP_QUEUE_SIZE          20                          /**< Size of timer operation queues. */

#define APP_TIME_SEND_DATA_INTERVAL      45                         /**< System clock process interval (ms). */
#define APP_TIME_STOPWATCH_INTERVAL      100                         /**< System clock process interval (ms). */
#define APP_TIME_MOTOR_INTERVAL          50                          /**< System clock process interval (ms). */

//定时器初始化
void Timers_Init(void);

//发送flash数据定时器开启
void Timer_BleSendDataStart(void);

//发送flash数据定时器停止
void Timer_BleSendDataStop(void);

//定时发送adc数据定时器开启
void Timer_BleSendAdcDataStart(void);

//定时发送adc数据定时器停止
void Timer_BleSendADCDataStop(void);

//秒表定时器开启
void Timer_StopWatchStop(void);

//秒表定时器停止
void Timer_StopWatchStart(void);

//马达计时定时器开始
void Timer_MotorStart(void);

//马达计时定时器关闭
void Timer_MotorStop(void);

#endif

