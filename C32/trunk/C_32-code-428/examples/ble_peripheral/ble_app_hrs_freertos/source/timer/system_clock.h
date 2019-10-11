/***************************Copyright(c)***********************************
**                       Veepoo Tech
**                  COPYRIGHT (C) 2016-2019
**-----------File Info-----------------------------------------------------
** File        name:     system_clock.h
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

#ifndef _SYSTEM_TIME_H_
#define _SYSTEM_TIME_H_

#include <stdint.h>
#include <stdbool.h>

#pragma pack(1)
typedef struct
{
    uint16_t  year;     // 2000+
    uint8_t   month;    // 1-12
    uint8_t   day;      // 0-31
    uint8_t   hour;     // 0-23
    uint8_t   minutes;  // 0-59
    uint8_t   seconds;  // 0-59
    uint8_t   week;
}system_clock_time_t;

#pragma pack()

extern const system_clock_time_t * clock;

//一秒钟时间中断事件,处理系统时间,其他app事件
extern void Sclock_OneSecondEventHandler(void);

/* 系统时间初始化 */
void Sclock_Init(void);

//获取系统时间
void Sclock_ReadTimeToBuf(uint8_t *time_buf,uint8_t len);

//同步时间
void Sclock_SynchronizationTime(const uint8_t *time_buf,uint8_t len);

//对时间数组自检
uint8_t Sclock_TimeCheck(uint8_t *p_data, uint8_t len);

//时间补偿
void Sclock_CompensationTime(const uint8_t m_seconds);

//是不是0点0分
bool Sclock_IsZero(void);

//获取前几天的日期
void Sclock_GetPreviousDayDate (uint16_t p_day, uint8_t *month, uint8_t *day);

//
void Sclock_Enable(void);

//求某年到某月某日有多少天
uint32_t Sclock_SeekYearDays(uint16_t year,uint8_t month,uint8_t day);

//根据年和天数求月日
uint8_t Sclock_SeekYearMonthAndDayByDays(uint16_t year, uint16_t days, uint8_t *month, uint8_t *day);

//获取当前系统日期
void Sclock_GetYearMonthDay(uint16_t *year, uint8_t *month, uint8_t *day);

//获取当前系统日期；year/month/day:小的年月日；year1/month1/day1:大的年月日；
//all_days：总天水；返回时间是否有效
uint8_t Sclock_GetsDaysByTwoDate(uint16_t year, uint8_t month, uint8_t day, uint16_t year1, 
                                  uint8_t month1, uint8_t day1, uint32_t *all_days);

void Sclock_GetTime(uint8_t *hours, uint8_t *minute, uint8_t *second);

//测试设置系统时间                      
void Sclock_SetTestTime(void);
    

#endif
