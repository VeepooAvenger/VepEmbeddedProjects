/***************************Copyright(c)***********************************
**                       Veepoo Tech
**                  COPYRIGHT (C) 2016-2019
**-----------File Info-----------------------------------------------------
** File        name:     system_clock.c
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
#include <string.h>
#include "system_clock.h"
#include "nordic_common.h"
#include "app_type.h"
#include "app_rtc.h"
#include "daily_test.h"

/*********************************************************************
* �궨��
*/
#ifndef BUILD_UINT16
#define BUILD_UINT16(loByte, hiByte) \
          ((uint16_t)(((loByte) & 0x00FF) + (((hiByte) & 0x00FF) << 8)))
#endif

// �ж��ǲ�������
#define	IS_LEAP_YEAR(year)	(!((year) % 400) || (((year) % 100) && !((year) % 4)))

/*********************************************************************
* ȫ�ֱ���
*/
static system_clock_time_t  system_clock_time_s;        //ϵͳʱ��
const system_clock_time_t *clock = (const system_clock_time_t*)&system_clock_time_s;

/*********************************************************************
* ���ؾ�̬����
*/
						
/*********************************************************************
* ���غ�������
*/

/****************************************************************
** Function name:           Sclock_SeekDays
** Descriptions:            ����ĳ�����ж�����
** input parameters:        is_leap_year���ǲ������ꣻmoth:�Ǹ���
** output parameters:       ��
** Returned value:          ����������ж�����
** Created by:              WJ              
** Created Date:            2017-08-11
*****************************************************************/ 
static uint8_t Sclock_SeekDays(bool is_leap_year, uint8_t month)
{
    uint8_t days = 31;
	
    if((month == 4) || ( month == 6) || (month == 9) || ( month == 11))
    {
        days = 30;
    }
    
    /* �����´��� */
    if(month == 2)
    {
        if(is_leap_year)
        {
            days = 29;
        }
        else 
        {
            days = 28;
        }
    }
    return days;
}

/****************************************************************
** Function name:           Sclock_SeekWeek
** Descriptions:            ��������
** input parameters:        ��
** output parameters:       ��
** Returned value:          �������ڣ�1~7��
** Created by:              WJ              
** Created Date:            2017-08-11
*****************************************************************/ 
static void Sclock_SeekWeek(void)
{
    uint16_t year = clock->year;
    uint8_t month = clock->month;
    uint8_t day = clock->day;
    uint8_t week = 0; 
    
    if(month == 1)
    {
        year--;
        month = 12;
        day += 3;
    }
    else if(month == 2)
    {
        year--;
        month = 12;
        day--;
    }
    week = (day + 2 * month + 3 * (month+1)/5 + year + year/4- year/100 + year/400) % 7 ;

    system_clock_time_s.week = ++week;        //1~7
}

/****************************************************************
** Function name:           Sclock_OneSecondDeal
** Descriptions:            һ����ʱ���¼�����ϵͳʱ��
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-08-11
*****************************************************************/ 
static void Sclock_OneSecondDeal(void)
{
    uint8_t days = 0;
    
#if DAILY_TEST_SLEEP
#else    
//    system_clock_time_s.seconds += 4;     //test
//   system_clock_time_s.seconds += 9;
    if(++system_clock_time_s.seconds >= 60)
#endif         
    {
        system_clock_time_s.seconds = 0;
#if DAILY_TEST_SLEEP
    if(system_clock_time_s.minutes %5 == 0)
    {
        system_clock_time_s.minutes += 3;
    }               
#endif     
        if(++system_clock_time_s.minutes >= 60)
        {
            system_clock_time_s.minutes = 0;
            if(++system_clock_time_s.hour >= 24)
            {
                system_clock_time_s.hour = 0;
                system_clock_time_s.day++;
                days = Sclock_SeekDays(IS_LEAP_YEAR(system_clock_time_s.year),system_clock_time_s.month);
                if(system_clock_time_s.day > days)
                {
                    system_clock_time_s.day = 1;
                    if(++system_clock_time_s.month > 12)
                    {
                        system_clock_time_s.month = 1;
                        system_clock_time_s.year++;											 
                    }
                }             
                /* �������ڼ������� */
                Sclock_SeekWeek();
            }
        }
    }	
}

/****************************************************************
** Function name:           Sclock_OneSecondEventHandler
** Descriptions:            һ����ʱ���ж��¼�,����ϵͳʱ��,����app�¼�
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-08-11
*****************************************************************/ 
void Sclock_OneSecondEventHandler(void)
{   
    Sclock_OneSecondDeal();
    App_OneSecondTrigger();   // 
}

/****************************************************************
** Function name:           Sclock_Init
** Descriptions:            ϵͳʱ�Ӳ�����ʼ��
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-08-11
*****************************************************************/ 
void Sclock_Init(void)
{
    /* ��ʼ��ϵͳʱ����� */
    memset(&system_clock_time_s, 0 ,sizeof(system_clock_time_s));

    /* ʱ��Ĭ�ϳ�ʼֵ */
    system_clock_time_s.seconds = 50;
    system_clock_time_s.minutes = 0;
    system_clock_time_s.hour = 1;
    system_clock_time_s.day = 1;
    system_clock_time_s.month = 1;
    system_clock_time_s.year = 2017;
    system_clock_time_s.week = 5;
}


/****************************************************************
** Function name:           Sclock_Enable
** Descriptions:            ʹ��ϵͳʱ��
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-08-11
*****************************************************************/ 
void Sclock_Enable(void)
{
    Rtc_Init();   
}

/****************************************************************
** Function name:           Sclock_ReadTimeToBuf
** Descriptions:            ��ȡϵͳʱ��
** input parameters:        time_buf������ָ�룬len�����鳤��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-08-11
*****************************************************************/ 
void Sclock_ReadTimeToBuf(uint8_t *time_buf, uint8_t len)
{
    if(len >= 7)
    {
        time_buf[0] = (uint8_t)((system_clock_time_s.year >> 8) & 0xff);
        time_buf[1] = (uint8_t)(system_clock_time_s.year & 0xFF);
        time_buf[2] = system_clock_time_s.month;
        time_buf[3] = system_clock_time_s.day;
        time_buf[4] = system_clock_time_s.hour;
        time_buf[5] = system_clock_time_s.minutes;
        time_buf[6] = system_clock_time_s.seconds;
    }
}

/****************************************************************
** Function name:           Sclock_SynchronizationTime
** Descriptions:            ͬ��ʱ��
** input parameters:        ��
** output parameters:       ��
** Returned value:          ����ƽ��ֵ
** Created by:              WJ              
** Created Date:            2017-08-11
*****************************************************************/ 
void Sclock_SynchronizationTime(const uint8_t *time_buf, uint8_t len)
{
    if(len >= 7)
    {
        system_clock_time_s.year = BUILD_UINT16(time_buf[1], time_buf[0]);
        system_clock_time_s.month = time_buf[2];
        system_clock_time_s.day = time_buf[3];
        system_clock_time_s.hour = time_buf[4];
        system_clock_time_s.minutes = time_buf[5];
        system_clock_time_s.seconds = time_buf[6];
    
        /* ��ע��:�˴�ʱ���޸ĺ�,��Ҫ���¼������� */
        Sclock_SeekWeek();
    }
}

/****************************************************************
** Function name:           Sclock_TimeCheck
** Descriptions:            ��ʱ�������Լ�
** input parameters:        time_buf������ָ�룬len�����鳤��
** output parameters:       ��
** Returned value:          ����ƽ��ֵ
** Created by:              WJ              
** Created Date:            2017-08-11
*****************************************************************/ 
uint8_t Sclock_TimeCheck(uint8_t *time_buf, uint8_t len)	//ʱ����Ϣ������
{
    uint16_t year_tmp;
    
    year_tmp = time_buf[0] * 256 + time_buf[1];
    
    if((2015 <= year_tmp) && (year_tmp <= 2050))   //2015<= �� <= 2050
    {
        if((1 <= time_buf[2]) && (time_buf[2] <= 12))	 //   1<= �� <= 12
        {
            if((1 <= time_buf[3]) && (time_buf[3]) <= 31)		//   1<= �� <= 31
            {
                if((time_buf[4] <= 23) && ( time_buf[5] <= 59) && (time_buf[6] <= 59))    //   0<= ʱ <= 23  
                {
                    return(1);  //   0<=  �֣���  <= 59
                }
            }
        }       
    }   	
	return(0);
} 

/****************************************************************
** Function name:           Sclock_CompensationTime
** Descriptions:            ʱ�䲹��
** input parameters:        m_seconds������������
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-08-11
*****************************************************************/ 
void Sclock_CompensationTime(const uint8_t m_seconds)
{
    uint8_t days = 0;
    
    system_clock_time_s.seconds += m_seconds;
    if(system_clock_time_s.seconds >= 60)
    {
        system_clock_time_s.minutes += system_clock_time_s.seconds / 60;
        system_clock_time_s.seconds %= 60; 
        
        if( system_clock_time_s.minutes >= 60)
        {
            system_clock_time_s.hour += system_clock_time_s.minutes / 60;
            system_clock_time_s.minutes %= 60;
            if(system_clock_time_s.hour >= 24)
            {
                system_clock_time_s.hour = 0;
                system_clock_time_s.day++;
                days = Sclock_SeekDays(IS_LEAP_YEAR(system_clock_time_s.year), system_clock_time_s.month);
                if(system_clock_time_s.day > days)
                {
                    system_clock_time_s.day = 1;
                    if(++system_clock_time_s.month > 12)
                    {
                        system_clock_time_s.month = 1;
                        system_clock_time_s.year++;											 
                    }
                }             
                /* �������ڼ������� */
                Sclock_SeekWeek();
            }
        }       
    }  
}

/****************************************************************
** Function name:           Sclock_IsZero
** Descriptions:            �ǲ���0��0��
** input parameters:        ��
** output parameters:       ��
** Returned value:          ����0�����ǣ�1��
** Created by:              WJ              
** Created Date:            2017-08-11
*****************************************************************/ 
bool Sclock_IsZero(void)
{
    if(clock->hour == 0 && clock->minutes == 0)
    {
        return true;
    }
    return false;
}

/****************************************************************
** Function name:           Sclock_GetPreviousDayDate
** Descriptions:            ��ȡǰ���������
** input parameters:        p_day��ǰ����
** output parameters:       month���£�day��
** Returned value:          ��
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
            day_temp = Sclock_SeekDays(IS_LEAP_YEAR(year), month_temp); 
        }            
    }
    
    *day = day_temp;
    *month = month_temp;
}

/****************************************************************
** Function name:           Sclock_SeekYearDays
** Descriptions:            ��ĳ�굽ĳ��ĳ���ж�����
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��������
** Created by:              WJ              
** Created Date:            2017-08-11
*****************************************************************/ 
uint32_t Sclock_SeekYearDays(uint16_t year, uint8_t month, uint8_t day)
{
    uint32_t year_day = 0;

    if(month > 12 || day > 31)
    {
        return 0;
    }
    
    for(uint8_t index = 1; index <= month; index++)
    {
        if(index == month)
        {
            year_day += day;
        }
        else
        {
            year_day += Sclock_SeekDays(IS_LEAP_YEAR(year), index);
        }
    }
    return year_day;
}

/****************************************************************
** Function name:           Sclock_SeekYearMonthAndDayByDays
** Descriptions:            �����������������
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��������
** Created by:              WJ              
** Created Date:            2017-08-11
*****************************************************************/ 
uint8_t Sclock_SeekYearMonthAndDayByDays(uint16_t year, uint16_t days, uint8_t *month, uint8_t *day)
{
    uint32_t year_day = 0;
//    uint8_t temp_month = 0;
//    uint8_t temp_day = 0;
    uint8_t month_days = 0;
    
    if(month == NULL || day == NULL)
    {
        return 0;
    }
    for(uint8_t index = 1; index <= 12; index++)
    {
        month_days = Sclock_SeekDays(IS_LEAP_YEAR(year), index);
        
        if((year_day + month_days) > days)
        {
            *month = index;
            *day = days - year_day;
            return 1;
        }
        year_day += month_days;
        
    }
    return 0;
}

/****************************************************************
** Function name:           Sclock_GetYearMonthDay
** Descriptions:            ��ȡ��ǰϵͳ����
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��������
** Created by:              WJ              
** Created Date:            2017-08-11
*****************************************************************/ 
void Sclock_GetYearMonthDay(uint16_t *year, uint8_t *month, uint8_t *day)
{
    if(year != NULL)
    {
        *year = clock->year;
    }
    if(month != NULL)
    {
        *month = clock->month;
    }
    if(day != NULL)
    {
        *day = clock->day;
    }
    
}

/****************************************************************
** Function name:           Sclock_GetsDaysByTwoDate
** Descriptions:            ��ȡ��ǰϵͳ����
** input parameters:        year/month/day:С��������
                            year1/month1/day1:���������                        
** output parameters:       all_days������ˮ
** Returned value:          ����ʱ���Ƿ���Ч
** Created by:              WJ              
** Created Date:            2017-10-23
*****************************************************************/ 
uint8_t Sclock_GetsDaysByTwoDate(uint16_t year, uint8_t month, uint8_t day, uint16_t year1, 
                                  uint8_t month1, uint8_t day1, uint32_t *all_days)
{
    uint16_t i = 0;
    uint32_t total_days = 0;
    uint32_t days = 0;
    uint32_t days1 = 0;
    
    if(year > year1)
    {
        //ʱ�䲻��
        *all_days = 0;
        return 0;
    }
    days = Sclock_SeekYearDays(year, month, day);
    days1 = Sclock_SeekYearDays(year1, month1, day1);
    
    //WJ.20171107.û�бȽ����
    if(year == year1 && days > days1)
    {
        //ʱ�䲻��
        *all_days = 0;
        return 0;
    }
    
    if(year == year1)
    {
        total_days = days1 - days;
    }
    else
    {
        total_days = Sclock_SeekYearDays(year1, month1, day1);
        for(i = year; i < year1; i++)
        {
            total_days += Sclock_SeekYearDays(i, 12, 31);
        }
        total_days = total_days - Sclock_SeekYearDays(year, month, day);
    }
    
    *all_days = total_days;
    return 1;
}

/****************************************************************
** Function name:           Sclock_GetTime
** Descriptions:            ��ȡ��ǰϵͳʱ��
** input parameters:        year/month/day:С��������
                            year1/month1/day1:���������                        
** output parameters:       all_days������ˮ
** Returned value:          ����ʱ���Ƿ���Ч
** Created by:              WJ              
** Created Date:            2017-10-23
*****************************************************************/ 
void Sclock_GetTime(uint8_t *hours, uint8_t *minutes, uint8_t *second)
{
    if(hours != NULL)
    {
        *hours = clock->hour;
    }
    if(minutes != NULL)
    {
        *minutes = clock->minutes;
    }
    if(second != NULL)
    {
        *second = clock->seconds;
    } 
}

/****************************************************************
** Function name:           Sclock_SetTestTime
** Descriptions:            ��������ϵͳʱ��                      
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-10-27
*****************************************************************/ 
void Sclock_SetTestTime(void)
{
    system_clock_time_s.year =2018;
    system_clock_time_s.month = 12;
    system_clock_time_s.day =  2;
    system_clock_time_s.hour = 9;
    system_clock_time_s.minutes = 29;
    system_clock_time_s.seconds = 58;   
}

