#include <stdint.h>
#include <stdbool.h>
#include "nrf_delay.h"
#include "bsp_btn_ble.h"
#include "app_timer.h"
#include "time_manage.h"
#include "nrf_drv_gpiote.h"
#include "SystemTime.h"
#include "data_manage.h"
#include "cpu_task_data_type.h"
#include "gui_thread.h"

#define APP_TIME_DEF_BSP 0          //不需要定义，定义会死机问题还没有找到

#define APP_BUTTON_KEY_PIN_0    KEY_PIN

static uint8_t  key_press_flag = 0;
static uint16_t  key_press_time = 0;

uint32_t  key_rtc_cnt = 0;

static uint8_t  button_enabled_flag = 0;

/**@brief Function for initializing buttons and leds.
 *
 * @param[out] p_erase_bonds  Will be true if the clear bonding button was pressed to wake the application up.
 */
uint8_t Buttons_GetPressState(void)
{
    return key_press_flag;
}

uint16_t Buttons_GetPressTime(void)
{
    return key_press_time;
}

static void Bsp_Key_Handler(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action)
{
    if(pin != APP_BUTTON_KEY_PIN_0 || action != NRF_GPIOTE_POLARITY_TOGGLE
      || button_enabled_flag != 1)
    {
        return;
    }

    if(!nrf_gpio_pin_read(APP_BUTTON_KEY_PIN_0))//&& p_data_manage->device_trace_param_s.font_test_on == 0)    //按下
    {
        key_press_flag = 1;
        key_press_time = 0;
        if(p_data_manage->device_trace_param_s.factory_mode == 0)
        {
            MotorDelEvent();//关闭马达事件
        }
    }
    else                                //释放
    {
        if(key_press_time < 3*8)//3)
        {        
            GuiThread_AddKeyEvent(BUTTON_EVENT_CLICK);//, BUTTON_TYPE_KEY);
        }
        
        key_press_flag = 0;
        key_press_time = 0;
    }  
}

void Buttons_PressCalcTime(void)
{
    key_rtc_cnt++;
    if(key_press_flag)
    {
        key_press_time++;       // 单位为125ms
        if(key_press_time == 3*8)
        {
            // 触发按键事件
            GuiThread_AddKeyEvent(BUTTON_EVENT_LONGPRESS);//, BUTTON_TYPE_KEY);         
        }
    }
}

void Buttons_Init(void)
{
    ret_code_t err_code;

    
    if(!nrf_drv_gpiote_is_init())
    {
        err_code = nrf_drv_gpiote_init();    
        APP_ERROR_CHECK(err_code);
    }
    //WJ.20181024.增加管脚定义检查
    if(APP_BUTTON_KEY_PIN_0 == 0xFF)
    {   //管脚未定义
        return;
    }
    else
    {
        nrf_drv_gpiote_in_config_t in_config = GPIOTE_CONFIG_IN_SENSE_TOGGLE(false);//true);    
        in_config.pull = NRF_GPIO_PIN_NOPULL;//NRF_GPIO_PIN_PULLUP;            //PULL UP
        
        err_code = nrf_drv_gpiote_in_init(APP_BUTTON_KEY_PIN_0, &in_config, Bsp_Key_Handler);
        APP_ERROR_CHECK(err_code);

        //nrf_drv_gpiote_in_event_enable(APP_BUTTON_KEY_PIN_0, true);    
    }
    button_enabled_flag = 0;
    Buttons_Enable();
}

void Buttons_Disable(void)
{
    if(button_enabled_flag == 1)
    {
        PIN_HIGH(SGM_IN1_PIN); 
        PIN_HIGH(SGM_IN2_PIN); 
        button_enabled_flag = 0;
        nrf_drv_gpiote_in_event_disable(APP_BUTTON_KEY_PIN_0);
        
        key_press_flag = 0;
    }
}

void Buttons_Enable(void)
{
    if(button_enabled_flag == 0)
    {
        key_press_flag = 0;
        
        PIN_LOW(SGM_IN1_PIN); 
        PIN_LOW(SGM_IN2_PIN);      
        nrf_drv_gpiote_in_event_enable(APP_BUTTON_KEY_PIN_0, true); 
        button_enabled_flag = 1;  
    }
}

bool Buttons_IsEnable(void)
{
    return button_enabled_flag == 1?1:0;
}

