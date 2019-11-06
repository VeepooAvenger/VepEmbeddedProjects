/*******************Copyright(c)*********************************
** Veepoo Tech
** COPYRIGHT (C) 2016-2019
**----------------------FileInfo---------------------------------
** File name:                  time_manage.h
** Last modified Date:         2017-07-15
** Last SYS_VERSION:           0.0.1
** Descriptions:
**---------------------------------------------------------------
** Createdby:                  GT
** Createddate:                2017-07-15
** SYS_VERSION:                0.0.1
** Descriptions:               定时器初始化
**---------------------------------------------------------------
** Modified by:				
** Modified date:			
** SYS_VERSION:				
** Modified Descriptions:	
****************************************************************/

#include "app_type.h"
#include "time_manage.h"
#include "bsp_btn_ble.h"
#include "alarm_event.h"

/*********************************************************************
* Macro Definition
*/
                                        //增加一个占用24Byte
#define APP_TIMER_OP_QUEUE_SIZE         10//20//15      // Size of timer operation queues.
#define APP_TIME_STOPWATCH_INTERVAL     100                         /**< System clock process interval (ms). */

static TimerHandle_t m_stopwatch_timer;

static void Timer_StopWatchTimeoutHandler(TimerHandle_t xTimer);

/****************************************************************
** Function name:               timers_init
** Descriptions:                定时器初始化
** input parameters:            无
** output parameters:           无
** Returned value:              无
** Created by:                  GT     
** Created Date:                2017-07-15
**---------------------------------------------------------------
** Modified by:                  
** Modified date:              
** Modified Descriptions:
**---------------------------------------------------------------
****************************************************************/
void timers_init(void)
{   //app_timer works with RTC1 and nrv_drv_timer uses configurable TIMER0 and/or TIMER1 and/or TIMER2
   
    // Initialize timer module.
    APP_TIMER_INIT(APP_TIMER_PRESCALER, APP_TIMER_OP_QUEUE_SIZE, false);
    
    // Create timers.                                                        

    App_Ancs_Create_Time();
    
    m_stopwatch_timer = xTimerCreate("STOPWATCH" , APP_TIME_STOPWATCH_INTERVAL	,pdTRUE, NULL, Timer_StopWatchTimeoutHandler);
 
}

/*******************************************************************************
** Function name:           Timer_BleSendADCDataStop
** Descriptions:            秒表定时器开启
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2016-10-12
**------------------------------------------------------------------------------
** Modified by:
** Modified date:
**------------------------------------------------------------------------------
*******************************************************************************/
void Timer_StopWatchStart(void)
{
    if (pdPASS != xTimerStart(m_stopwatch_timer, OSTIMER_WAIT_FOR_QUEUE))
    {
//        APP_ERROR_HANDLER(NRF_ERROR_NO_MEM);
    }
}

/*******************************************************************************
** Function name:           Timer_StopWatchStop
** Descriptions:            秒表定时器停止
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2016-10-12
**------------------------------------------------------------------------------
** Modified by:
** Modified date:
**------------------------------------------------------------------------------
*******************************************************************************/
void Timer_StopWatchStop(void)
{
    xTimerStop(m_stopwatch_timer, 0);
}

/*******************************************************************************
** Function name:           Timer_StopWatchTimeoutHandler
** Descriptions:            秒表定时器中断
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2016-10-12
**------------------------------------------------------------------------------
** Modified by:
** Modified date:
**------------------------------------------------------------------------------
*******************************************************************************/
static void Timer_StopWatchTimeoutHandler(TimerHandle_t xTimer)
{
    UNUSED_PARAMETER(xTimer);   
    Watch_100msEvent();
}
