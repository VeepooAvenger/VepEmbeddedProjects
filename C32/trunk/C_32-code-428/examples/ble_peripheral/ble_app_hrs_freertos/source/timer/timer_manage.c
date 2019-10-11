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
#include "app_type.h"
#include "timer_manage.h"
#include "bsp_button.h"
#include "hal_acc_calculate.h"
#include "gui_update.h"
/*********************************************************************
* Macro Definition
*/


/*********************************************************************
* Global Variables
*/

static TimerHandle_t m_send_flash_data_timer;
static TimerHandle_t m_send_adc_data_timer;
static TimerHandle_t m_stopwatch_timer;
static TimerHandle_t m_motor_timer;


extern system_flag_t system_flag_s;            //系统标志
/*********************************************************************
* local functions
*/

/*******************************************************************************
** Function name:           Timer_BleSendDataStart
** Descriptions:            发送flash数据定时器开启
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
void Timer_BleSendDataStart(void)
{
    if (pdPASS != xTimerStart(m_send_flash_data_timer, OSTIMER_WAIT_FOR_QUEUE))
    {
 //       APP_ERROR_HANDLER(NRF_ERROR_NO_MEM);
    }
}

/*******************************************************************************
** Function name:           Timer_BleSendDataStop
** Descriptions:            发送flash数据定时器停止
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
void Timer_BleSendDataStop(void)
{
    xTimerStop(m_send_flash_data_timer, 0);
}


/*******************************************************************************
** Function name:           Timer_BleSendDataStop
** Descriptions:            定时发送adc数据定时器开启
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
void Timer_BleSendAdcDataStart(void)
{
    if (pdPASS != xTimerStart(m_send_adc_data_timer, OSTIMER_WAIT_FOR_QUEUE))
    {
//        APP_ERROR_HANDLER(NRF_ERROR_NO_MEM);
    }
}


/*******************************************************************************
** Function name:           Timer_BleSendADCDataStop
** Descriptions:            定时发送adc数据定时器停止
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
void Timer_BleSendADCDataStop(void)
{
    xTimerStop(m_send_adc_data_timer, 0);
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
** Function name:           Timer_BleSendDataTimeoutHandler
** Descriptions:            发送flash数据定时器中断
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
static void Timer_BleSendDataTimeoutHandler(TimerHandle_t xTimer)
{
    UNUSED_PARAMETER(xTimer);    
    App_AddEvent(APP_SEND_FLASH_DATA_EVENT);      
}

/*******************************************************************************
** Function name:           Timer_BleSendAdcDataTimeoutHandler
** Descriptions:            Adc发送定时器中断
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
static void Timer_BleSendAdcDataTimeoutHandler(TimerHandle_t xTimer)
{
    UNUSED_PARAMETER(xTimer);   
    App_AddEvent(APP_SEND_ADC_DATA_EVENT);      
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

/*******************************************************************************
** Function name:           Timer_MotorStart
** Descriptions:            马达计时定时器开始
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              YX             
** Created Date:            2016-10-30
**------------------------------------------------------------------------------
** Modified by:
** Modified date:
**------------------------------------------------------------------------------
*******************************************************************************/
void Timer_MotorStart(void)
{
    if (pdPASS != xTimerStart(m_motor_timer, OSTIMER_WAIT_FOR_QUEUE))
    {
//        APP_ERROR_HANDLER(NRF_ERROR_NO_MEM);
    }
}

/*******************************************************************************
** Function name:           Timer_MotorStop
** Descriptions:            马达计时定时器关闭
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              YX  
** Created Date:            2016-10-30
**------------------------------------------------------------------------------
** Modified by:
** Modified date:
**------------------------------------------------------------------------------
*******************************************************************************/
void Timer_MotorStop(void)
{
    xTimerStop(m_motor_timer, 0);
}
/*******************************************************************************
** Function name:           Timer_MotorTimeoutHandler
** Descriptions:            马达定时器中断
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
static void Timer_MotorTimeoutHandler(TimerHandle_t xTimer)
{
    UNUSED_PARAMETER(xTimer);
#if APP_MOTOR_ENABLED    
    if(Battery_IsLow() && system_flag_s.sign_test == 0x00)     
    {  
        //停止震动 
        Motor_DelEvent();      
    }  
    //检测是否有电机震动事件，判断是否开始震动    
    Motor_EventRun();    
#endif  // APP_MOTOR_ENABLED  
}
/*******************************************************************************
** Function name:           Timers_Init
** Descriptions:            定时器初始化
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
void Timers_Init(void)
{   
    //app_timer works with RTC1 and nrv_drv_timer uses configurable TIMER0 and/or TIMER1 and/or TIMER2
 
    // Initialize timer module.
    APP_TIMER_INIT(APP_TIMER_PRESCALER, APP_TIMER_OP_QUEUE_SIZE, false);
	
    // Create timers.														

#if APP_ANCS_ENABLED 
    Ancs_TimerCreate();
#endif
 
#if APP_HALACC_ENABLED 
    Acc_TimerCreate();
#endif   
    
    Gui_UpdateTimeoutInit();

    m_send_flash_data_timer = xTimerCreate("SEND_FLASH_DATA" , APP_TIME_SEND_DATA_INTERVAL	,pdFALSE, NULL, Timer_BleSendDataTimeoutHandler);
    
    m_send_adc_data_timer = xTimerCreate("SEND_ADC_DATA" , APP_TIME_SEND_DATA_INTERVAL	,pdFALSE, NULL, Timer_BleSendAdcDataTimeoutHandler);
    m_stopwatch_timer = xTimerCreate("STOPWATCH" , APP_TIME_STOPWATCH_INTERVAL	,pdTRUE, NULL, Timer_StopWatchTimeoutHandler);
    m_motor_timer = xTimerCreate("MOTOR" , APP_TIME_MOTOR_INTERVAL	,pdTRUE, NULL, Timer_MotorTimeoutHandler);
}
