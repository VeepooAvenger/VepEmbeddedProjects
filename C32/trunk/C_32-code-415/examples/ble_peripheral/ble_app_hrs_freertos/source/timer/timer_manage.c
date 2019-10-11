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


extern system_flag_t system_flag_s;            //ϵͳ��־
/*********************************************************************
* local functions
*/

/*******************************************************************************
** Function name:           Timer_BleSendDataStart
** Descriptions:            ����flash���ݶ�ʱ������
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
void Timer_BleSendDataStart(void)
{
    if (pdPASS != xTimerStart(m_send_flash_data_timer, OSTIMER_WAIT_FOR_QUEUE))
    {
 //       APP_ERROR_HANDLER(NRF_ERROR_NO_MEM);
    }
}

/*******************************************************************************
** Function name:           Timer_BleSendDataStop
** Descriptions:            ����flash���ݶ�ʱ��ֹͣ
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
void Timer_BleSendDataStop(void)
{
    xTimerStop(m_send_flash_data_timer, 0);
}


/*******************************************************************************
** Function name:           Timer_BleSendDataStop
** Descriptions:            ��ʱ����adc���ݶ�ʱ������
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
void Timer_BleSendAdcDataStart(void)
{
    if (pdPASS != xTimerStart(m_send_adc_data_timer, OSTIMER_WAIT_FOR_QUEUE))
    {
//        APP_ERROR_HANDLER(NRF_ERROR_NO_MEM);
    }
}


/*******************************************************************************
** Function name:           Timer_BleSendADCDataStop
** Descriptions:            ��ʱ����adc���ݶ�ʱ��ֹͣ
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
void Timer_BleSendADCDataStop(void)
{
    xTimerStop(m_send_adc_data_timer, 0);
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
** Function name:           Timer_BleSendDataTimeoutHandler
** Descriptions:            ����flash���ݶ�ʱ���ж�
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
static void Timer_BleSendDataTimeoutHandler(TimerHandle_t xTimer)
{
    UNUSED_PARAMETER(xTimer);    
    App_AddEvent(APP_SEND_FLASH_DATA_EVENT);      
}

/*******************************************************************************
** Function name:           Timer_BleSendAdcDataTimeoutHandler
** Descriptions:            Adc���Ͷ�ʱ���ж�
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
static void Timer_BleSendAdcDataTimeoutHandler(TimerHandle_t xTimer)
{
    UNUSED_PARAMETER(xTimer);   
    App_AddEvent(APP_SEND_ADC_DATA_EVENT);      
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

/*******************************************************************************
** Function name:           Timer_MotorStart
** Descriptions:            ����ʱ��ʱ����ʼ
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
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
** Descriptions:            ����ʱ��ʱ���ر�
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
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
** Descriptions:            ��ﶨʱ���ж�
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
static void Timer_MotorTimeoutHandler(TimerHandle_t xTimer)
{
    UNUSED_PARAMETER(xTimer);
#if APP_MOTOR_ENABLED    
    if(Battery_IsLow() && system_flag_s.sign_test == 0x00)     
    {  
        //ֹͣ�� 
        Motor_DelEvent();      
    }  
    //����Ƿ��е�����¼����ж��Ƿ�ʼ��    
    Motor_EventRun();    
#endif  // APP_MOTOR_ENABLED  
}
/*******************************************************************************
** Function name:           Timers_Init
** Descriptions:            ��ʱ����ʼ��
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
