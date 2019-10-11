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
//#include "app_config_data.h"

#define ALARM_CLOCK_MAX_NUM      20

#define COUNTDOWN_MAX_NUM       1

typedef enum
{
    STOPWATCH_IDE,          //准备
    STOPWATCH_READY,        //就绪
    STOPWATCH_START,        //开始
    STOPWATCH_STOP,         //停止
}stopwatch_e;

#pragma pack(1)

//闹铃参数结构体
typedef struct
{
	uint8_t valid;          //闹钟--有效/无效(1bit,节省内存空间)
	uint8_t hour;           //闹钟--时	
	uint8_t minute;         //闹钟--分    
	uint8_t state;          //闹钟--状态(开启关闭)
	uint8_t repeat;         //闹钟--重复
    uint8_t scene_mode;     //情景模式   
    uint16_t year;          //年    
    uint8_t month;          //月
    uint8_t day;            //日
    uint8_t res[2];         //保留4字节对齐！！！！！
}alarm_clock_config_t;

typedef struct{ 
    uint8_t alarm_index;
    uint8_t alarm_num_start;
    uint8_t alarm_num_total;
    uint16_t alarm_crc16;
}alarm_clock_data_t;

typedef struct{ 
    uint8_t alarm_hour;
    uint8_t alarm_minute;
    uint8_t alarm_mode;
}alarm_clock_do_t;

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

//闹钟

//获取最后执行的闹钟数据
alarm_clock_do_t *Alarm_GetAlarmClockDo(void);

//设置闹钟数据发送的详情
void Alarm_SetSendData(uint8_t start_index, uint8_t total_index, uint16_t crc16);

//发送闹钟数据到APP，每次发送一组，需要延时循环发送
uint8_t Alarm_SendDataHandler(void);

//通过ID清除闹钟，需要保存
uint8_t Alarm_ClearByID(uint8_t id);

//重置所有闹钟,但不需要保存，用于恢复出厂设置，
//闹钟数据在清除整个用户数据块里清除，根据实际情况使用
uint8_t Alarm_InitAll(void);

//重置所有闹钟,需要保存
uint8_t Alarm_ClearAll(void);

//监听闹钟，到时间执行闹钟处理，整分钟处理
void Alarm_Monitor(void);

//单个闹钟数据自检，判断数据有效区间
uint8_t Alarm_Check(alarm_clock_config_t *alarm_clock_temp);

//获取闹钟的有效个数
uint8_t Alarm_GetValidConfigNum(void);

// 设置闹钟,数据来自协议(APP设置),数据为闹钟的有效部分,
uint8_t Alarm_Set(uint8_t *data, uint8_t len);

//计算闹钟的CRC
uint16_t Alarm_GetCRC(void);

//把闹钟写入到flash，目前52端设置了10s的保存延时,在1秒事件里执行
uint8_t Alarm_SetToFlashCheck(void);

//从flash获取闹钟数据，注意这里与平台、底层设计相关
uint8_t Alarm_getFromFlash(void);

// 自检单次闹钟，过时的需要重置状态
void Alarm_ValidCheck(void);

//闹钟自检，清除无效闹钟，上电、或者定时自检
void Alarm_InitCheck(void);

//闹钟初始化，自检、过时自检
void Alarm_Init(void);

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
//uint8_t Watch_Toggle(void);

//倒计时
//倒计时初始化,用于执行的数据
void Countdown_Init(void );

//倒计时设置，来自通讯协议
uint8_t Countdown_SetTime(uint32_t second, uint8_t state, uint8_t id, uint8_t type);

//从多次开始倒计时,按键开启
uint8_t Countdown_SetTimeBySystemStart(void);

//获取当前正在执行的倒计时配置
countdown_config_t *Countdown_Get(void);

//获取正在运行的倒计时状态
uint8_t Countdown_GetState(void);

//发送响应数据到APP,倒计时进行的时候发送
void Countdown_SendDataToApp(uint8_t ack, uint8_t operate);

//倒计时跑秒处理,放在一秒定时器中断中
void Countdown_ToRunSecond(void);

#endif

