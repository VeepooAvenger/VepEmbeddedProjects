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

#define APP_TIME_DEF_BSP 0          //不需要定义，定义会死机问题还没有找到

#define BUTTON_ENABLE_2   1

#define APP_BUTTON_KEY_PIN_0    KEY_PIN
#if (BUTTON_ENABLE_2)
    #define APP_BUTTON_KEY_PIN_1    KEY1_PIN
#endif    

#define KEY_PRESS_INTERVAL              50   
#define KEY_PRESS_LONG_PRESS            (3 * 20)
#define KEY_PRESS_LONG_PRESS_6          (6 * 20)
#define KEY_PRESS_LONG_PRESS_R          (7 * 20)

static TimerHandle_t m_key_interval_timer;

static uint8_t bsp_btn_start = 0;
static bsp_key_info_t key_info_s;
static uint8_t key_clock_cnt = 0;

#if (BUTTON_ENABLE_2)
static TimerHandle_t m_key1_interval_timer;

static uint8_t bsp_btn1_start = 0;
static bsp_key_info_t key1_info_s;
static uint8_t key1_clock_cnt = 0;
#endif

extern system_flag_t system_flag_s;

void Button_TimerStart(void);
void Button_TimerStop(void);

#if (BUTTON_ENABLE_2)
void Button1_TimerStart(void);
void Button1_TimerStop(void);
#endif

/****************************************************************
** Function name:           Button_GetButtonInfo
** Descriptions:            获取按键信息
** input parameters:        无
** output parameters:       无
** Returned value:          无
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
** Descriptions:            获取当前按键是否按下
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ             
** Created Date:            2017-01-18
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
uint8_t Button_GetPressState(void)
{
    return bsp_btn1_start;
}

/****************************************************************
** Function name:           Button_Handler
** Descriptions:            按键管脚中断
** input parameters:        无
** output parameters:       无
** Returned value:          无
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
    if(!nrf_gpio_pin_read(APP_BUTTON_KEY_PIN_0))    //按下
    {
        if(key_clock_cnt == 0)
        {
            bsp_btn_start = 1;
            key_info_s.key_state = APP_BSP_KEY_NULL_PRESS;
        
            Button_TimerStart();
        }
    }
    else                                //释放
    {
        if(key_clock_cnt == 0)
        {
            /* 防震动处理（一个定时器周期） */
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
            // 触发按键事件
            if(!system_flag_s.sign_off)
            {   
                Gui_ThreadAddKeyEvent(BUTTON_EVENT_CLICK, BUTTON_TYPE_KEY);
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
** Descriptions:            检测按键释放（长按）
** input parameters:        无
** output parameters:       无
** Returned value:          无
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
    
    if(nrf_gpio_pin_read(APP_BUTTON_KEY_PIN_0) && key_clock_cnt >= KEY_PRESS_LONG_PRESS_R) //按键释放
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
** Descriptions:            定时器超时处理
** input parameters:        无
** output parameters:       无
** Returned value:          无
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
            //加入长按事件监听，WJ.20170215 
            Gui_ThreadAddKeyEvent(BUTTON_EVENT_LONGPRESS, BUTTON_TYPE_KEY);       
            key_info_s.key_state = APP_BSP_KEY_NULL_PRESS;            
        }
        else if(key_clock_cnt == KEY_PRESS_LONG_PRESS_6)
        {
            key_info_s.key_press_time = APP_BSP_PRESS_LONG_6S;
            key_info_s.key_state = APP_BSP_KEY_LONG_PRESS;  
            //加入长按事件监听，WJ.20170215
            Gui_ThreadAddKeyEvent(BUTTON_EVENT_LONGPRESS_6S, BUTTON_TYPE_KEY);            
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
** Descriptions:            创建定时器
** input parameters:        无
** output parameters:       无
** Returned value:          无
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
** input parameters:        无
** output parameters:       无
** Returned value:          无
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
** input parameters:        无
** output parameters:       无
** Returned value:          无
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

#if (BUTTON_ENABLE_2)
/*************************Button1*********************************/
/****************************************************************
** Function name:           Button_GetButtonInfo
** Descriptions:            获取按键信息
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ             
** Created Date:            2016-11-28
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
bsp_key_info_t *Button1_GetButtonInfo(void)
{
    return &key1_info_s;
}

/****************************************************************
** Function name:           Button_GetPressState
** Descriptions:            获取当前按键是否按下
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ             
** Created Date:            2017-01-18
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
uint8_t Button1_GetPressState(void)
{
    return bsp_btn1_start;
}

/****************************************************************
** Function name:           Button_Handler
** Descriptions:            按键管脚中断
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ             
** Created Date:            2017-01-18
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
static void Button1_Handler(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action)
{
    if(pin != APP_BUTTON_KEY_PIN_1 || action != NRF_GPIOTE_POLARITY_TOGGLE)
    {
#if BUTTON_TEST        
        uint8_t ble_test_buf[20] = {0x22,0x00,0x00,};
        Ble_SendDataViaBat(ble_test_buf, 20);
#endif          
        return;
    }
    key1_info_s.key_id = APP_BSP_KEY_ID_2;
    if(!nrf_gpio_pin_read(APP_BUTTON_KEY_PIN_1))    //按下
    {
#if BUTTON_TEST        
        uint8_t ble_test_buf[20] = {0x22,0x00,0x11,};
        Ble_SendDataViaBat(ble_test_buf, 20);
#endif        
        if(key1_clock_cnt == 0)
        {
            bsp_btn1_start = 1;
            key1_info_s.key_state = APP_BSP_KEY_NULL_PRESS;
        
            Button1_TimerStart();
        }
    }
    else                                //释放
    {      
#if BUTTON_TEST        
        uint8_t ble_test_buf[20] = {0x22,0x00,0x22,};
        Ble_SendDataViaBat(ble_test_buf, 20);
#endif          
        if(key1_clock_cnt < KEY_PRESS_LONG_PRESS)// && key_clock_cnt > 0)
        {
            Button1_TimerStop();
            key1_info_s.key_press_time = 0;
            key1_info_s.key_state = APP_BSP_KEY_SHORT_PRESS;   
            key1_clock_cnt = 0;        
            // 触发按键事件
            if(!system_flag_s.sign_off)
            {   
                Gui_ThreadAddKeyEvent(BUTTON1_EVENT_CLICK,BUTTON_TYPE_KEY);
            }
        }
        else if(key1_clock_cnt < KEY_PRESS_LONG_PRESS_R )
        {           
            key1_clock_cnt = 0; 
        }
               
        bsp_btn1_start = 0;
    }    
}


/*******************************************************************************
** Function name:           App_HalAcc_Timeout_Handler
** Descriptions:            定时器超时处理
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ             
** Created Date:            2016-11-28
**------------------------------------------------------------------------------
** Modified by:
** Modified date:
**------------------------------------------------------------------------------
*******************************************************************************/
static void Button1_TimerOutHandler(TimerHandle_t xTimer)
{
    static uint8_t stop_cnt = 0;
    
    UNUSED_PARAMETER(xTimer);
    if(!nrf_gpio_pin_read(APP_BUTTON_KEY_PIN_1))
    {  
        stop_cnt = 0;
        if(key1_clock_cnt > KEY_PRESS_LONG_PRESS_R) return;
                    
        key1_clock_cnt++;        
        if(key1_clock_cnt == KEY_PRESS_LONG_PRESS)
        {
            key1_info_s.key_press_time = APP_BSP_PRESS_LONG_3S;
            key1_info_s.key_state = APP_BSP_KEY_LONG_PRESS;     
            //加入长按事件监听，WJ.20170215 
            Gui_ThreadAddKeyEvent(BUTTON1_EVENT_LONGPRESS, BUTTON_TYPE_KEY);       
            key1_info_s.key_state = APP_BSP_KEY_NULL_PRESS;            
        }
        else if(key1_clock_cnt == KEY_PRESS_LONG_PRESS_6)
        {
            key1_info_s.key_press_time = APP_BSP_PRESS_LONG_6S;
            key1_info_s.key_state = APP_BSP_KEY_LONG_PRESS;  
            //加入长按事件监听，WJ.20170215
            Gui_ThreadAddKeyEvent(BUTTON1_EVENT_LONGPRESS_6S, BUTTON_TYPE_KEY);            
            key1_info_s.key_state = APP_BSP_KEY_NULL_PRESS;
        } 
    }
    else
    {
        if(stop_cnt++ >= 12)    //60ms
        {
            stop_cnt = 0;        
            key1_clock_cnt = 0; 
            Button1_TimerStop();  
        }       
    }
}

/****************************************************************
** Function name:           App_HalAcc_Create_Time
** Descriptions:            创建定时器
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ             
** Created Date:            2016-11-28
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Button1_TimerCreate(void)
{ 
    m_key1_interval_timer = xTimerCreate("KEY1_BUTTON" , KEY_PRESS_INTERVAL, 
                                       pdTRUE, NULL, Button1_TimerOutHandler);
}

/****************************************************************
** Function name:           App_HalAcc_Time_Start
** Descriptions:            
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ             
** Created Date:            2016-11-28
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Button1_TimerStart(void)
{
    if (pdPASS != xTimerStart(m_key1_interval_timer, OSTIMER_WAIT_FOR_QUEUE))
    {
//        APP_ERROR_HANDLER(NRF_ERROR_NO_MEM);
    }   
}

/****************************************************************
** Function name:           App_HalAcc_Time_Start
** Descriptions:            
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ             
** Created Date:            2016-11-28
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Button1_TimerStop(void)
{
    xTimerStop(m_key1_interval_timer, 0);   
}
#endif

/****************************************************************
** Function name:           Button_Init
** Descriptions:            按键初始化
** input parameters:        无
** output parameters:       无
** Returned value:          无
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
#if (BUTTON_ENABLE_2)    
    Button1_TimerCreate();
#endif    
	nrf_drv_gpiote_in_config_t in_config = GPIOTE_CONFIG_IN_SENSE_TOGGLE(false);	
	in_config.pull = NRF_GPIO_PIN_NOPULL;//NRF_GPIO_PIN_PULLUP;			//PULL UP

	err_code = nrf_drv_gpiote_in_init(APP_BUTTON_KEY_PIN_0, &in_config, Button_Handler);
	APP_ERROR_CHECK(err_code);

	nrf_drv_gpiote_in_event_enable(APP_BUTTON_KEY_PIN_0, true);    

#if (BUTTON_ENABLE_2)    
	nrf_drv_gpiote_in_config_t in_config1 = GPIOTE_CONFIG_IN_SENSE_TOGGLE(false);	
	in_config1.pull = NRF_GPIO_PIN_NOPULL;//NRF_GPIO_PIN_PULLUP;			//PULL UP

	err_code = nrf_drv_gpiote_in_init(APP_BUTTON_KEY_PIN_1, &in_config1, Button1_Handler);
	APP_ERROR_CHECK(err_code);

	nrf_drv_gpiote_in_event_enable(APP_BUTTON_KEY_PIN_1, true);   
#endif    
    
}
