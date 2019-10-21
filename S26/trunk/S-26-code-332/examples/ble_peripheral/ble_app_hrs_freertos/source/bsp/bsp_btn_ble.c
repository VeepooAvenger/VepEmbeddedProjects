#include <stdint.h>
#include <stdbool.h>
#include "nrf_delay.h"
#include "bsp_btn_ble.h"
//#include "app_timer.h"
//#include "time_manage.h"
#include "nrf_drv_gpiote.h"
//#include "SystemTime.h"
#include "data_manage.h"
#include "cpu_task_data_type.h"
#include "gui_thread.h"
#include "board.h"

#define APP_TIME_DEF_BSP 0          //不需要定义，定义会死机问题还没有找到

#define APP_BUTTON_KEY_PIN_0        KEY_PIN

static uint8_t      key_press_flag = 0;
static uint16_t     key_press_time = 0;

#if (BUTTON_ENABLE_2)

#define APP_BUTTON_KEY_PIN_1    KEY2_PIN
    
static uint8_t      key1_press_flag = 0;
static uint16_t     key1_press_time = 0;
#endif

uint32_t  key_rtc_cnt = 0;

void Bsp_Key_Init(void);


/**@brief Function for initializing buttons and leds.
 *
 * @param[out] p_erase_bonds  Will be true if the clear bonding button was pressed to wake the application up.
 */
void Buttons_Init(void)
{  
    Bsp_Key_Init();
}

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
    if(pin != APP_BUTTON_KEY_PIN_0 || action != NRF_GPIOTE_POLARITY_TOGGLE)
    {
        return;
    }

    if(!nrf_gpio_pin_read(APP_BUTTON_KEY_PIN_0))//&& p_data_manage->device_trace_param_s.font_test_on == 0)    //按下
    {
        key_press_flag = 1;
        key_press_time = 0;
    }
    else                                //释放
    {
        if(key_press_time < 3*8)//3)
        {        
            GuiThread_AddKeyEvent(BUTTON_EVENT_CLICK, key_press_time);
        }
        
        key_press_flag = 0;
        key_press_time = 0;
    }  
}

#if (BUTTON_ENABLE_2)
uint8_t Buttons_GetKey1PressState(void)
{
    return key1_press_flag;
}

uint16_t Buttons_GetKey1PressTime(void)
{
    return key1_press_time;
}

static void Bsp_Key1_Handler(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action)
{
    if(pin != APP_BUTTON_KEY_PIN_1 || action != NRF_GPIOTE_POLARITY_TOGGLE)
    {
        return;
    }

    if(!nrf_gpio_pin_read(APP_BUTTON_KEY_PIN_1))//&& p_data_manage->device_trace_param_s.font_test_on == 0)    //按下
    {
        key1_press_flag = 1;
        key1_press_time = 0;
    }
    else                                //释放
    {
        if(key1_press_time < 3*8)//3)
        {        
            GuiThread_AddKeyEvent(BUTTON1_EVENT_CLICK, key1_press_time);
        }
        
        key1_press_flag = 0;
        key1_press_time = 0;
    }  
}
#endif

void Buttons_PressCalcTime(void)
{
    key_rtc_cnt++;
    if(key_press_flag)
    {
        key_press_time++;       // 单位为125ms
        if(key_press_time == 3*8)
        {
            // 触发按键事件
            GuiThread_AddKeyEvent(BUTTON_EVENT_LONGPRESS, key_press_time);         
        }

    } 

#if (BUTTON_ENABLE_2)    
    if(key1_press_flag)
    {
        key1_press_time++;       // 单位为125ms
        if(key1_press_time == 5*8)
        {
             // 触发按键事件
            GuiThread_AddKeyEvent(BUTTON1_EVENT_LONGPRESS_5S, key1_press_time);         
        }

    } 
#endif      
}

void Bsp_Key_Init(void)
{
    ret_code_t err_code;

    if(!nrf_drv_gpiote_is_init())
    {
        err_code = nrf_drv_gpiote_init();    
        APP_ERROR_CHECK(err_code);
    }
    
    nrf_drv_gpiote_in_config_t in_config = GPIOTE_CONFIG_IN_SENSE_TOGGLE(true);    
    in_config.pull = NRF_GPIO_PIN_NOPULL;//NRF_GPIO_PIN_PULLUP;            //PULL UP
    
    err_code = nrf_drv_gpiote_in_init(APP_BUTTON_KEY_PIN_0, &in_config, Bsp_Key_Handler);
    APP_ERROR_CHECK(err_code);

    nrf_drv_gpiote_in_event_enable(APP_BUTTON_KEY_PIN_0, true);  


#if (BUTTON_ENABLE_2)
    nrf_drv_gpiote_in_config_t in_config1 = GPIOTE_CONFIG_IN_SENSE_TOGGLE(true);    
    in_config1.pull = NRF_GPIO_PIN_NOPULL;//NRF_GPIO_PIN_PULLUP;            //PULL UP
    
    err_code = nrf_drv_gpiote_in_init(APP_BUTTON_KEY_PIN_1, &in_config1, Bsp_Key1_Handler);
    APP_ERROR_CHECK(err_code);

    nrf_drv_gpiote_in_event_enable(APP_BUTTON_KEY_PIN_1, true);  
#endif    
    
    
}

