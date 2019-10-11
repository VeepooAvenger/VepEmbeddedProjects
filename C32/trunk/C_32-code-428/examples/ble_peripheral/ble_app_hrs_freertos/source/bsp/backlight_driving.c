/***************************Copyright(c)***********************************
**                       Veepoo Tech
**                  COPYRIGHT (C) 2016-2019
**-----------File Info-----------------------------------------------------
** File        name:     backlight_driving.h
** Last modified Date:   2018-08-07
** Last SYS_VERSION:     0.0.0
** Descriptions:            
**-------------------------------------------------------------------------
** Created  by:          WJ
** Created  date:        2018-08-07
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
#include "app_error.h"
#include "nrf_delay.h"
#include "board.h"
#include "backlight_driving.h"

#define BACKLIGHT_PWM_PIN               LCD_EN_PIN         //背光控制管脚
#define BACKLIGHT_MAX_SUPPORT_LEVEL     16                 //背光IC可支持最大调光等级

static backlight_driving_info_t backlight_driving_info_s;

//调级次数需要根据实际情况设置
const uint8_t backlight_level_cnt[BACKLIGHT_MAX_SUPPORT_LEVEL] = {15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0};

/****************************************************************
** Function name:           Backlight_SetPWM
** Descriptions:            背光调级
** input parameters:        set_level:设置背光等级
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2018-08-07
*****************************************************************/ 
void Backlight_SetPWM(uint8_t set_level)
{
    uint8_t pwm_cnt = 0;
    
    uint8_t c_index = 0;
    uint8_t s_index = 0;
    
    s_index = BACKLIGHT_MAX_SUPPORT_LEVEL - BACKLIGHT_MAX_CHANAGE_LEVEL + set_level - 1;
    c_index = BACKLIGHT_MAX_SUPPORT_LEVEL - BACKLIGHT_MAX_CHANAGE_LEVEL + backlight_driving_info_s.level - 1;
    
    if(set_level < backlight_driving_info_s.level)
    {
        pwm_cnt = backlight_level_cnt[s_index] - backlight_level_cnt[c_index];
    }
    else if(set_level > backlight_driving_info_s.level)
    {
        pwm_cnt = backlight_level_cnt[s_index] 
                  + BACKLIGHT_MAX_SUPPORT_LEVEL - backlight_level_cnt[c_index];
    }
    else
    {   //相同等级不再调整
        return;
    }
    
    for(uint8_t i = 0; i < pwm_cnt; i++)
    {
        PIN_LOW(BACKLIGHT_PWM_PIN);
        nrf_delay_us(1);
        PIN_HIGH(BACKLIGHT_PWM_PIN);
        nrf_delay_us(1);
    }
    backlight_driving_info_s.level = set_level;
}

/****************************************************************
** Function name:           Backlight_Close
** Descriptions:            关闭背光
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2018-08-07
*****************************************************************/ 
void Backlight_Close(void)
{
    PIN_LOW(BACKLIGHT_PWM_PIN);
    //3ms的延时确保关闭，防止连续开启导致等级出错
    nrf_delay_ms(3);    
    
    //初始化参数
    backlight_driving_info_s.state = 0;
    backlight_driving_info_s.level = 0;
}

/****************************************************************
** Function name:           Backlight_Close
** Descriptions:            关闭背光
** input parameters:        set_level:设置背光等级
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2018-08-07
*****************************************************************/ 
void Backlight_SetLeve(uint8_t set_level)
{
    if(set_level == 0)
    {
        Backlight_Close();
        return;
    }
    if(backlight_driving_info_s.state == 0)
    {   //背光未开启
        PIN_HIGH(BACKLIGHT_PWM_PIN);
        nrf_delay_us(20);
        backlight_driving_info_s.state = 1;
        backlight_driving_info_s.level = BACKLIGHT_MAX_CHANAGE_LEVEL;
    }
    if(set_level > BACKLIGHT_MAX_CHANAGE_LEVEL)
    {
        set_level = BACKLIGHT_MAX_CHANAGE_LEVEL;
    }
    Backlight_SetPWM(set_level); 
    
}

/****************************************************************
** Function name:           Backlight_TestSetLeve
** Descriptions:            背光调级测试
** input parameters:        set_level:设置背光等级
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2018-08-13
*****************************************************************/ 
void Backlight_TestSetLeve(uint8_t set_level)
{
    uint8_t pwm_set_cnt = 0;
    
    PIN_LOW(BACKLIGHT_PWM_PIN);
    //3ms的延时确保关闭，防止连续开启导致等级出错
    nrf_delay_ms(3);  

    if(set_level == 0)
    {
        return;
    }
    else if(set_level > BACKLIGHT_MAX_SUPPORT_LEVEL)
    {
        set_level = BACKLIGHT_MAX_SUPPORT_LEVEL;
    }   

    //背光未开启
    PIN_HIGH(BACKLIGHT_PWM_PIN);
    nrf_delay_us(20);
    
    pwm_set_cnt = BACKLIGHT_MAX_SUPPORT_LEVEL - set_level;
    
    for(uint8_t i = 0; i < pwm_set_cnt; i++)
    {
        PIN_LOW(BACKLIGHT_PWM_PIN);
        nrf_delay_us(1);
        PIN_HIGH(BACKLIGHT_PWM_PIN);
        nrf_delay_us(1);
    }
  
    backlight_driving_info_s.state = 1;  
    backlight_driving_info_s.level = set_level;
}


