/***************************Copyright(c)***********************************
**                       Veepoo Tech
**                  COPYRIGHT (C) 2016-2019
**-----------File Info-----------------------------------------------------
** File        name:     bsp_button.c
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
#include <stdint.h>
#include <stdbool.h>
#include "nrf_delay.h"
#include "app_timer.h"
#include "timer_manage.h"
#include "nrf_drv_gpiote.h"
#include "bsp_button.h"
#include "gui_thread.h"

#define APP_TIME_DEF_BSP 0          //����Ҫ���壬������������⻹û���ҵ�

#define APP_BUTTON_KEY_PIN_0    KEY_PIN

#define KEY_PRESS_INTERVAL              50   
#define KEY_PRESS_LONG_PRESS            (3 * 20)
#define KEY_PRESS_LONG_PRESS_6          (6 * 20)
#define KEY_PRESS_LONG_PRESS_R          (7 * 20)

static TimerHandle_t m_key_interval_timer;

static uint8_t bsp_btn_start = 0;
static bsp_key_info_t key_info_s;
static uint8_t key_clock_cnt = 0;

extern system_flag_t system_flag_s;

void Button_TimerStart(void);
void Button_TimerStop(void);

/****************************************************************
** Function name:           Button_GetButtonInfo
** Descriptions:            ��ȡ������Ϣ
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ             
** Created Date:            2016-11-28
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
bsp_key_info_t *Button_GetButtonInfo(void)
{
    return &key_info_s;
}

/****************************************************************
** Function name:           Button_GetPressState
** Descriptions:            ��ȡ��ǰ�����Ƿ���
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ             
** Created Date:            2017-01-18
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
uint8_t Button_GetPressState(void)
{
    return bsp_btn_start;
}

/****************************************************************
** Function name:           Button_Handler
** Descriptions:            �����ܽ��ж�
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ             
** Created Date:            2017-01-18
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
static void Button_Handler(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action)
{
    if(pin != APP_BUTTON_KEY_PIN_0 || action != NRF_GPIOTE_POLARITY_TOGGLE)
    {
        return;
    }
    key_info_s.key_id = APP_BSP_KEY_ID_1;
    if(!nrf_gpio_pin_read(APP_BUTTON_KEY_PIN_0))    //����
    {
        if(key_clock_cnt == 0)
        {
            bsp_btn_start = 1;
            key_info_s.key_state = APP_BSP_KEY_NULL_PRESS;
        
            Button_TimerStart();
        }
    }
    else                                //�ͷ�
    {
        if(key_clock_cnt == 0)
        {
            /* ���𶯴���һ����ʱ�����ڣ� */
            Button_TimerStop();
            key_info_s.key_press_time = 0;
            key_clock_cnt = 0;
        }
        else if(key_clock_cnt < KEY_PRESS_LONG_PRESS && key_clock_cnt > 0)
        {
            Button_TimerStop();
            key_info_s.key_press_time = 0;
            key_info_s.key_state = APP_BSP_KEY_SHORT_PRESS;   
            key_clock_cnt = 0;        
            // ���������¼�
            if(!system_flag_s.sign_off)
            {   
                Gui_ThreadAddKeyEvent(BUTTON_EVENT_CLICK);
            }
        }
        else if(key_clock_cnt < KEY_PRESS_LONG_PRESS_R )
        {           
            key_clock_cnt = 0; 
        }
               
        bsp_btn_start = 0;
    }    
}

/****************************************************************
** Function name:           Button_PressTime
** Descriptions:            ��ⰴ���ͷţ�������
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ             
** Created Date:            2017-01-18
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Button_PressTime(void)
{   
    static uint8_t key_no_press_cnt = 0;
    
    if(nrf_gpio_pin_read(APP_BUTTON_KEY_PIN_0) && key_clock_cnt >= KEY_PRESS_LONG_PRESS_R) //�����ͷ�
    {            
        if(++key_no_press_cnt >= 1)
        {
            key_clock_cnt = 0;    
            key_no_press_cnt = 0;              
        }     
    }
}

/*******************************************************************************
** Function name:           App_HalAcc_Timeout_Handler
** Descriptions:            ��ʱ����ʱ����
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ             
** Created Date:            2016-11-28
**------------------------------------------------------------------------------
** Modified by:
** Modified date:
**------------------------------------------------------------------------------
*******************************************************************************/
static void Button_TimerOutHandler(TimerHandle_t xTimer)
{
    static uint8_t stop_cnt = 0;
    
    UNUSED_PARAMETER(xTimer);
    if(!nrf_gpio_pin_read(APP_BUTTON_KEY_PIN_0))
    {  
        stop_cnt = 0;
        if(key_clock_cnt > KEY_PRESS_LONG_PRESS_R) return;
                    
        key_clock_cnt++;        
        if(key_clock_cnt == KEY_PRESS_LONG_PRESS)
        {
            key_info_s.key_press_time = APP_BSP_PRESS_LONG_3S;
            key_info_s.key_state = APP_BSP_KEY_LONG_PRESS;     
            //���볤���¼�������WJ.20170215 
            Gui_ThreadAddKeyEvent(BUTTON_EVENT_LONGPRESS);       
            key_info_s.key_state = APP_BSP_KEY_NULL_PRESS;            
        }
        else if(key_clock_cnt == KEY_PRESS_LONG_PRESS_6)
        {
            key_info_s.key_press_time = APP_BSP_PRESS_LONG_6S;
            key_info_s.key_state = APP_BSP_KEY_LONG_PRESS;  
            //���볤���¼�������WJ.20170215
            Gui_ThreadAddKeyEvent(BUTTON_EVENT_LONGPRESS_6S);            
            key_info_s.key_state = APP_BSP_KEY_NULL_PRESS;
        } 
    }
    else
    {
        if(stop_cnt++ >= 12)    //60ms
        {
            stop_cnt = 0;        
            key_clock_cnt = 0; 
            Button_TimerStop();  
        }       
    }
}

/****************************************************************
** Function name:           App_HalAcc_Create_Time
** Descriptions:            ������ʱ��
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ             
** Created Date:            2016-11-28
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Button_TimerCreate(void)
{ 
    m_key_interval_timer = xTimerCreate("KEY_BUTTON" , KEY_PRESS_INTERVAL, 
                                       pdTRUE, NULL, Button_TimerOutHandler);
}

/****************************************************************
** Function name:           App_HalAcc_Time_Start
** Descriptions:            
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ             
** Created Date:            2016-11-28
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Button_TimerStart(void)
{
    if (pdPASS != xTimerStart(m_key_interval_timer, OSTIMER_WAIT_FOR_QUEUE))
    {
//        APP_ERROR_HANDLER(NRF_ERROR_NO_MEM);
    }   
}

/****************************************************************
** Function name:           App_HalAcc_Time_Start
** Descriptions:            
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ             
** Created Date:            2016-11-28
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Button_TimerStop(void)
{
    xTimerStop(m_key_interval_timer, 0);   
}

/****************************************************************
** Function name:           Button_Init
** Descriptions:            ������ʼ��
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ             
** Created Date:            2016-11-28
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Button_Init(void)
{    
	ret_code_t err_code;
	//if not initlize gpiote module, initlize it
	if(!nrf_drv_gpiote_is_init())
	{
		err_code = nrf_drv_gpiote_init();	
		APP_ERROR_CHECK(err_code);
	}

    Button_TimerCreate();
    
	nrf_drv_gpiote_in_config_t in_config = GPIOTE_CONFIG_IN_SENSE_TOGGLE(false);	
	in_config.pull = NRF_GPIO_PIN_NOPULL;//NRF_GPIO_PIN_PULLUP;			//PULL UP

	err_code = nrf_drv_gpiote_in_init(APP_BUTTON_KEY_PIN_0, &in_config, Button_Handler);
	APP_ERROR_CHECK(err_code);

	nrf_drv_gpiote_in_event_enable(APP_BUTTON_KEY_PIN_0, true);    
    
}
