/***************************Copyright(c)***********************************
**                       Veepoo Tech
**                  COPYRIGHT (C) 2016-2019
**-----------File Info-----------------------------------------------------
** File        name:     alarm_event.h
** Last modified Date:   2017-08-24
** Last SYS_VERSION:     0.0.0
** Descriptions:            
**-------------------------------------------------------------------------
** Created  by:          WJ
** Created  date:        2017-08-24
** SYS_VERSION:          0.0.0
** Descriptions:         
**-------------------------------------------------------------------------
** Modified by:				
** Modified date:  			          
** SYS_VERSION:                 
** Descriptions:                        
***************************************************************************/

#ifndef _ALARM_CLOCK_EVENT_H_
#define _ALARM_CLOCK_EVENT_H_

#include <stdint.h>
#include <stdbool.h>

#define COUNTDOWN_MAX_NUM       1

typedef enum
{
    STOPWATCH_IDE,          //准备
    STOPWATCH_READY,        //就绪
    STOPWATCH_START,        //开始
    STOPWATCH_STOP,         //停止
}stopwatch_e;

typedef enum
{
    COUNTDOWN_STATE_IDE,          //准备
    COUNTDOWN_STATE_RUN,        //就绪
    COUNTDOWN_STATE_PAUSE,        //暂停
}COUNTDOWN_STATE_E;

#pragma pack(1)

typedef struct{   
    uint8_t id;                 
    uint8_t state;               //倒计时状态    
    uint8_t type;                //倒计时类型
    uint32_t countdown_second;   //总时间 
}countdown_config_t;             //倒计时


typedef struct{ 
    uint8_t stopwatch_100millisecond;
    stopwatch_e state;   
    uint8_t init_start;
    uint16_t total_second;
}stopwatch_config_t;             //秒表

#pragma pack()

//用于秒表计时的100毫秒定时器事件
void Watch_100msEvent(void);

//秒表初始化,(关机、按键开启)
void Watch_Init(void);

//秒表从就绪、停止状态开启
void Watch_Start(void);

//停止秒表,按键停止或者秒表计时到达上限
void Watch_Stop(void);

//秒表已经准备就绪,处于忙碌状态
uint8_t Watch_IsBusy(void);

//获取秒表的当前状态
stopwatch_e Watch_GetState(void);

//获取秒表的总秒数,用于ＵＩ显示
uint16_t Watch_GetTotalSecond(void);

//获取秒表当前的100毫秒数
uint8_t Watch_Get100Millisecond(void);

//设置秒表为就绪状态,(3s长按,初始化完成，并设置秒表为就绪状态)
void Watch_SetReady(void);

//暂停、继续秒表,亮屏状态下短按按键切换处理
void Watch_Toggle(void);

//秒表已经开始，秒表运转
uint8_t Watch_IsRun(void);

void Countdown_SendDataToApp(uint8_t ack, uint8_t operate);

#endif

