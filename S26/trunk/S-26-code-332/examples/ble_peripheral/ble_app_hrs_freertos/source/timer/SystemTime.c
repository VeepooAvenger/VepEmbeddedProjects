/*******************Copyright(c)*********************************
** Veepoo Tech
** COPYRIGHT (C) 2016-2019
**----------------------FileInfo---------------------------------
** File name:                  system_time.c
** Last modified Date:         2016-09-23
** Last SYS_VERSION:           0.0.1
** Descriptions:
**---------------------------------------------------------------
** Createdby:                  GT
** Createddate:                2016-09-23
** SYS_VERSION:                0.0.1
** Descriptions:               系统时间
**---------------------------------------------------------------
** Modified by:				
** Modified date:			
** SYS_VERSION:				
** Modified Descriptions:	
****************************************************************/

#include <string.h>
#include "SystemTime.h"
#include "nordic_common.h"
#include "app_type.h"
#include "app_rtc.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "algorithm_thread.h"
#include "communication_thread.h"
#include "ble_thread.h"
#include "gui_thread.h"
#include "data_manage.h"
#include "bsp_btn_ble.h"
#include "configuration.h"
#include "log.h"
#include "util.h"

#ifndef BUILD_UINT16
#define BUILD_UINT16(loByte, hiByte) \
          ((uint16_t)(((loByte) & 0x00FF) + (((hiByte) & 0x00FF) << 8)))
#endif


static system_clock_time_t  system_clock_time;                      //系统时间
//系统时间指针
const system_clock_time_t* clock = (const system_clock_time_t*)&system_clock_time;

/****************************************************************
** Function name:               Seek_Days
** Descriptions:                查找一月的天数
** input parameters:            无
** output parameters:           无
** Returned value:              无
** Created by:                  GT     
** Created Date:                2016-09-23
**---------------------------------------------------------------
** Modified by:                  
** Modified date:              
** Modified Descriptions:
**---------------------------------------------------------------
****************************************************************/
static uint8_t Seek_Days(bool is_leap_year, uint8_t month)
{
    uint8_t days = 31;
    
    if((4 == month)||(6 == month)||(9 == month)||(11 == month))
    {
        days = 30;
    }
    
    /* 闰年月处理 */
    if(is_leap_year&&(2 == month))
    {
        days = 29;
    }
    else if((!is_leap_year)&&(2 == month))
    {	    
        days = 28;
    }
    
    return days;
}

/****************************************************************
** Function name:               Seek_Week
** Descriptions:                查找周
** input parameters:            无
** output parameters:           无
** Returned value:              无
** Created by:                  GT     
** Created Date:                2016-09-23
**---------------------------------------------------------------
** Modified by:                  
** Modified date:              
** Modified Descriptions:
**---------------------------------------------------------------
****************************************************************/
static void Seek_Week(void)
{
    /* 
     *  蔡勒公式:
     *   week = y + [y/4]+[c/4]-2c+[26(m+1)/10]+d-1    
     */
    uint8_t year = clock->year%100;
    uint8_t month = clock->month;
    uint8_t day = clock->day;
    uint8_t century = 0;
    uint8_t week = 0;

    if((1 == month) || (2 == month))
    {
        year--;
        month = 12 + month;
        century = year/100;
    }

    week = (year+year/4+century/4-2*century+26*(month+1)/10+day-1)%7 ;
    
    /* 此处重新修改数值 */
    if(week == 0 )
    {
        week = 7;
    }
    
    system_clock_time.week = week;
}

/****************************************************************
** Function name:               RTC_TimeEvent
** Descriptions:                RTC时间事件
** input parameters:            无
** output parameters:           无
** Returned value:              无
** Created by:                  GT     
** Created Date:                2016-09-23
**---------------------------------------------------------------
** Modified by:                  
** Modified date:              
** Modified Descriptions:
**---------------------------------------------------------------
****************************************************************/
static void RTC_TimeEvent(void)
{
    uint8_t days = 0;
    
    if(++system_clock_time.seconds >= 60)       
    {
        system_clock_time.seconds = 0;
        if(++system_clock_time.minutes >= 60)
        {
            system_clock_time.minutes = 0;
            if(++system_clock_time.hour >= 24)
            {
                system_clock_time.hour = 0;
                system_clock_time.day++;
                days = Seek_Days(IsLeapYear(system_clock_time.year),system_clock_time.month);
                if(system_clock_time.day > days)
                {
                    system_clock_time.day = 1;
                    if(++system_clock_time.month > 12)
                    {
                        system_clock_time.month = 1;
                        system_clock_time.year++;                                             
                    }
                }             
                
                // 根据日期计算星期
                Seek_Week();
            }
        }
    }    
}

/****************************************************************
** Function name:               SystemTime_Update_Second
** Descriptions:                每秒更新系统时间
** input parameters:            无
** output parameters:           无
** Returned value:              无
** Created by:                  GT     
** Created Date:                2016-09-23
**---------------------------------------------------------------
** Modified by:                  
** Modified date:              
** Modified Descriptions:
**---------------------------------------------------------------
****************************************************************/
void SystemTime_Update_Second(void)
{   
    RTC_TimeEvent();
    	
    // 蓝牙线程处理周期性任务
    BleThread_PeriodCycleTask(clock->hour,clock->minutes,clock->seconds);
	
    // 通信处理周期性任务
    ComThread_PeriodCycleTask(clock->hour,clock->minutes,clock->seconds);
	
    // 算法处理周期性任务
    AlgThread_PeriodCycleTask(clock->hour,clock->minutes,clock->seconds);    
}

/****************************************************************
** Function name:               SystemTime_Init
** Descriptions:                系统时间初始化
** input parameters:            无
** output parameters:           无
** Returned value:              无
** Created by:                  GT     
** Created Date:                2016-09-23
**---------------------------------------------------------------
** Modified by:                  
** Modified date:              
** Modified Descriptions:
**---------------------------------------------------------------
****************************************************************/
void SystemTime_Init(void)
{
    // 初始化系统时间参数
    memset(&system_clock_time,0,sizeof(system_clock_time));

    // 时间默认初始值
    system_clock_time.seconds = 0;
    system_clock_time.minutes = 0;
    system_clock_time.hour = 8;
    system_clock_time.day = 11;
    system_clock_time.month = 9;
    system_clock_time.year = 2017;
    system_clock_time.week = 1;
}

/****************************************************************
** Function name:               SystemTime_Enable
** Descriptions:                使能系统时间
** input parameters:            无
** output parameters:           无
** Returned value:              无
** Created by:                  GT     
** Created Date:                2016-09-23
**---------------------------------------------------------------
** Modified by:                  
** Modified date:              
** Modified Descriptions:
**---------------------------------------------------------------
****************************************************************/
void SystemTime_Enable(void)
{
    App_RTC_Init();   
}

/****************************************************************
** Function name:               SystemTime_Read
** Descriptions:                读取系统时间
** input parameters:            p_data：         数据指针
                                len：            数据大小
** output parameters:           无
** Returned value:              无
** Created by:                  GT     
** Created Date:                2016-09-23
**---------------------------------------------------------------
** Modified by:                  
** Modified date:              
** Modified Descriptions:
**---------------------------------------------------------------
****************************************************************/
void SystemTime_Read(uint8_t* pData,uint8_t len)
{
    if(len >= 7)
    {
        pData[0] = (uint8_t)((system_clock_time.year>>8)&0xff);
        pData[1] = (uint8_t)(system_clock_time.year&0xFF);
        pData[2] = system_clock_time.month;
        pData[3] = system_clock_time.day;
        pData[4] = system_clock_time.hour;
        pData[5] = system_clock_time.minutes;
        pData[6] = system_clock_time.seconds;
    }
}

/****************************************************************
** Function name:               SystemTime_Adjust
** Descriptions:                系统时间调整
** input parameters:            p_data：         数据指针
                                len：            数据大小
** output parameters:           无
** Returned value:              无
** Created by:                  GT     
** Created Date:                2016-09-23
**---------------------------------------------------------------
** Modified by:                  
** Modified date:              
** Modified Descriptions:
**---------------------------------------------------------------
****************************************************************/
void SystemTime_Adjust(const uint8_t* pData,uint8_t len)
{
    if(len >= 7)
    {
        system_clock_time.year = BUILD_UINT16(pData[1], pData[0]);
        system_clock_time.month = pData[2];
        system_clock_time.day = pData[3];
        system_clock_time.hour = pData[4];
        system_clock_time.minutes = pData[5];
        system_clock_time.seconds = pData[6];
    
        // 注意:此处时间修改后,需要重新计算星期 
        Seek_Week();
    }
}

/****************************************************************
** Function name:               SystemTime_Check
** Descriptions:                系统时间检查
** input parameters:            p_data：         数据指针
                                len：            数据大小
** output parameters:           无
** Returned value:              是否正确
** Created by:                  GT     
** Created Date:                2016-09-23
**---------------------------------------------------------------
** Modified by:                  
** Modified date:              
** Modified Descriptions:
**---------------------------------------------------------------
****************************************************************/
uint8_t SystemTime_Check(uint8_t* p_data, uint8_t len)    
{
    uint16_t year_tmp = p_data[0]*256 + p_data[1];
    
    if((2015 <= year_tmp) && (year_tmp <= 2050))   //2015<= 年 <= 2050
    {
        if((1 <= p_data[2]) && (p_data[2] <= 12))     //   1<= 月 <= 12
        {
            if((1 <= p_data[3]) && (p_data[3]) <= 31)        //   1<= 日 <= 31
            {
                if((p_data[4] <= 23) && ( p_data[5] <= 59) && (p_data[6] <= 59))    //   0<= 时 <= 23  
                {
                    return NRF_SUCCESS;  
                }
            }
        }       
    }       
    return NRF_ERROR_INVALID_DATA;
}

/****************************************************************
** Function name:           Sclock_GetPreviousDayDate
** Descriptions:            获取前几天的日期
** input parameters:        p_day：前几天
** output parameters:       month：月；day日
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-08-11
*****************************************************************/ 
void Sclock_GetPreviousDayDate(uint16_t p_day, uint8_t *month, uint8_t *day)
{
    uint16_t i = 0;
    uint8_t month_temp = 0;
    uint8_t day_temp = 0;
    uint16_t year = 0;
    
    month_temp = clock->month;
    day_temp = clock->day;
    year = clock->year;
    
    if(p_day > 0xFFFF)
    {
        return;
    }
    
    for(i = 0; i < p_day; i++)
    {
        if(day_temp > 1)
        {
            day_temp = day_temp - 1;
        }
        else
        {
            if(month_temp > 1)
            {
                month_temp = month_temp - 1;       //WJ.20170322
            }
            else
            {
                month_temp = 12;
                year--;
            }
            day_temp = Seek_Days(IsLeapYear(year), month_temp); 
        }            
    }
    
    *day = day_temp;
    *month = month_temp;
}

