/***************************Copyright(c)***********************************
**                       Veepoo Tech
**                  COPYRIGHT (C) 2016-2019
**-----------File Info-----------------------------------------------------
** File        name:     pwm.c
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
#include <string.h>
#include "nrf.h"
#include "app_error.h"
#include "app_pwm.h"
#include "board.h"
#include "bsp_led.h"

#define PWM_GA1     GC1_PIN

APP_PWM_INSTANCE(PWM1, 2);                   // Create the instance "PWM1" using TIMER1.

static volatile bool ready_flag;            // A flag indicating PWM status.
static uint8_t pwm_flag = 0;                //pwm开启标志

/****************************************************************
** Function name:           Pwm_ReadyCallback
** Descriptions:            回调函数
** input parameters:        p_event：状态；p_context：回调执行函数
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2016-09-16
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Pwm_ReadyCallback(uint32_t pwm_id)    // PWM callback function
{
    ready_flag = true;
}

/****************************************************************
** Function name:           Pwm_Start
** Descriptions:            开始PWN
** input parameters:        frequency：频率；duty_cycle：占空比
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2016-09-16
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Pwm_Start(uint16_t frequency, uint8_t duty_cycle)
{
    ret_code_t err_code;

    if(pwm_flag)
    {
        return;
    }
    pwm_flag = 1;
    if(frequency == 0)
    {
        frequency = 20;
    }
    
    Led_SetGear(0);
    
    /* 2-channel PWM, 200Hz, output on DK LED pins. */
    app_pwm_config_t pwm1_cfg = APP_PWM_DEFAULT_CONFIG_1CH((1000000L/frequency), PWM_GA1);

    /* Switch the polarity of the second channel. */
    pwm1_cfg.pin_polarity[0] = APP_PWM_POLARITY_ACTIVE_HIGH;

    /* Initialize and enable PWM. */
    err_code = app_pwm_init(&PWM1, &pwm1_cfg, Pwm_ReadyCallback);
    APP_ERROR_CHECK(err_code);
    
    app_pwm_enable(&PWM1);

    while (app_pwm_channel_duty_set(&PWM1, 0, duty_cycle) == NRF_ERROR_BUSY);        
}

/****************************************************************
** Function name:           Pwm_Stop
** Descriptions:            停止pwm
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2016-09-16
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Pwm_Stop(void)
{
    if(pwm_flag)
    {
        app_pwm_disable(&PWM1);
        app_pwm_uninit(&PWM1);
        pwm_flag = 0;
    }
    Led_SetGear(0);
}
