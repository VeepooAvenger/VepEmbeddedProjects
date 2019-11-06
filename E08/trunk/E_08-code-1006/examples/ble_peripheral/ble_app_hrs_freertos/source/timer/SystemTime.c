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
** Descriptions:               ϵͳʱ��
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
#include "ble_disconnect_remind.h"
//#include "gui_update.h"

#ifndef BUILD_UINT16
#define BUILD_UINT16(loByte, hiByte) \
          ((uint16_t)(((loByte) & 0x00FF) + (((hiByte) & 0x00FF) << 8)))
#endif

extern uint8_t camera_delay_cnt;

static system_clock_time_t  system_clock_time;                      //ϵͳʱ��

//ϵͳʱ��ָ��
const system_clock_time_t* clock = (const system_clock_time_t*)&system_clock_time;


/****************************************************************
** Function name:               Seek_Days
** Descriptions:                ����һ�µ�����
** input parameters:            ��
** output parameters:           ��
** Returned value:              ��
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
    
    /* �����´��� */
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
** Descriptions:                ������
** input parameters:            ��
** output parameters:           ��
** Returned value:              ��
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
     *  ���չ�ʽ:
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
    
    /* �˴������޸���ֵ */
    if(week == 0 )
    {
        week = 7;
    }
    
    system_clock_time.week = week;
}

/****************************************************************
** Function name:               RTC_TimeEvent
** Descriptions:                RTCʱ���¼�
** input parameters:            ��
** output parameters:           ��
** Returned value:              ��
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
                
                // �������ڼ�������
                Seek_Week();
            }
        }
    }    
}

/****************************************************************
** Function name:               SystemTime_Update_Second
** Descriptions:                ÿ�����ϵͳʱ��
** input parameters:            ��
** output parameters:           ��
** Returned value:              ��
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
    
    //App_Watch_Dog_Reload(); //1��ι��1�� 
   
    // 
    ////Buttons_PressTime();
    	
    // �����̴߳�������������
    BleThread_PeriodCycleTask(clock->hour,clock->minutes,clock->seconds);
	
    // ͨ�Ŵ�������������
    ComThread_PeriodCycleTask(clock->hour,clock->minutes,clock->seconds);
	
    // �㷨��������������
    AlgThread_PeriodCycleTask(clock->hour,clock->minutes,clock->seconds); 
    
    if(p_data_manage->device_trace_param_s.sign_on == 1)
    {      
        //Gui_UpdateTimeoutHandler();
        //YX.20180408.������������,����״̬�£����ѿ��ؿ���   
        Remind_BleDisconnectMonitor(p_flash_param->dev_config_s.ble_disconnect_enable, 1, 
                                    p_data_manage->device_trace_param_s.ble_connect);
        camera_delay_cnt++;

    }

}

/****************************************************************
** Function name:               SystemTime_Init
** Descriptions:                ϵͳʱ���ʼ��
** input parameters:            ��
** output parameters:           ��
** Returned value:              ��
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
    // ��ʼ��ϵͳʱ�����
    memset(&system_clock_time,0,sizeof(system_clock_time));

    // ʱ��Ĭ�ϳ�ʼֵ
    system_clock_time.seconds = 0;
    system_clock_time.minutes = 0;
    system_clock_time.hour = 8;
    system_clock_time.day = 1;
    system_clock_time.month = 1;
    system_clock_time.year = 2019;
    system_clock_time.week = 2;
}

/****************************************************************
** Function name:               SystemTime_Enable
** Descriptions:                ʹ��ϵͳʱ��
** input parameters:            ��
** output parameters:           ��
** Returned value:              ��
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
** Descriptions:                ��ȡϵͳʱ��
** input parameters:            p_data��         ����ָ��
                                len��            ���ݴ�С
** output parameters:           ��
** Returned value:              ��
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
** Descriptions:                ϵͳʱ�����
** input parameters:            p_data��         ����ָ��
                                len��            ���ݴ�С
** output parameters:           ��
** Returned value:              ��
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
    
        // ע��:�˴�ʱ���޸ĺ�,��Ҫ���¼������� 
        Seek_Week();
    }
}

/****************************************************************
** Function name:               SystemTime_Check
** Descriptions:                ϵͳʱ����
** input parameters:            p_data��         ����ָ��
                                len��            ���ݴ�С
** output parameters:           ��
** Returned value:              �Ƿ���ȷ
** Created by:                  GT     
** Created Date:                2016-09-23
**---------------------------------------------------------------
** Modified by:                  
** Modified date:              
** Modified Descriptions:
**---------------------------------------------------------------
****************************************************************/
//���
uint8_t SystemTime_Check(uint8_t* p_data, uint8_t len)    
{
    uint16_t year_tmp = p_data[0]*256 + p_data[1];
    
    if((year_tmp >= 2015) && (year_tmp <= 2050))   //2015<= �� <= 2050
    {
        if((p_data[2] >= 1) && (p_data[2] <= 12))     //   1<= �� <= 12
        {
            if((p_data[3] >= 1) && (p_data[3]) <= 31)        //   1<= �� <= 31
            {
                if((p_data[4] <= 23) && ( p_data[5] <= 59) && (p_data[6] <= 59))    //   0<= ʱ <= 23  
                {
                    return NRF_SUCCESS;  
                }
            }
        }       
    }       
    return NRF_ERROR_INVALID_DATA;
}

//С��
uint8_t SystemTime_Check1(uint8_t *p_data, uint8_t len)    
{
    static uint16_t year_tmp = 0;
    year_tmp =p_data[0] + (p_data[1]<<8) ;
    
    if((year_tmp >= 2015) && (year_tmp <= 2050))   //2015<= �� <= 2050
    {
        if((p_data[2] >= 1) && (p_data[2] <= 12))     //   1<= �� <= 12
        {
            if((p_data[3] >= 1) && (p_data[3]) <= 31)        //   1<= �� <= 31
            {
                if((p_data[4] <= 23) && ( p_data[5] <= 59) && (p_data[6] <= 59))    //   0<= ʱ <= 23  
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
            day_temp = Seek_Days(IsLeapYear(year), month_temp); 
        }            
    }
    
    *day = day_temp;
    *month = month_temp;
}


//С�� p_time1 < p_time2
uint8_t SystemTime_Compare(uint8_t *p_time1, uint8_t *p_time2)    
{
    uint16_t    year1 = 0;
    uint16_t    year2 = 0;
    
    year1 = p_time1[0] + (p_time1[1]<<8);
    year2 = p_time2[0] + (p_time2[1]<<8);
    
    if(year1 < year2 
      || ((year1 == year2) && (memcmp(&p_time1[2], &p_time2[2], 5) < 0)))
    {
        return 1;
    }
    
    return 0;
}
