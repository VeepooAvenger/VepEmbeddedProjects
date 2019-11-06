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
** Descriptions:               ��ʱ����ʼ��
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
                                        //����һ��ռ��24Byte
#define APP_TIMER_OP_QUEUE_SIZE         10//20//15      // Size of timer operation queues.
#define APP_TIME_STOPWATCH_INTERVAL     100                         /**< System clock process interval (ms). */

static TimerHandle_t m_stopwatch_timer;

static void Timer_StopWatchTimeoutHandler(TimerHandle_t xTimer);

/****************************************************************
** Function name:               timers_init
** Descriptions:                ��ʱ����ʼ��
** input parameters:            ��
** output parameters:           ��
** Returned value:              ��
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
** Descriptions:            ���ʱ������
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
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
** Descriptions:            ���ʱ��ֹͣ
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
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
** Descriptions:            ���ʱ���ж�
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
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
