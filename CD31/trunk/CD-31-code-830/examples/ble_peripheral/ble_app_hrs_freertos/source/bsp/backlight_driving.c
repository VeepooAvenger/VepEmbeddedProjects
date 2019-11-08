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

#define BACKLIGHT_PWM_PIN               LCD_EN_PIN         //������ƹܽ�
#define BACKLIGHT_MAX_SUPPORT_LEVEL     16                 //����IC��֧��������ȼ�

static backlight_driving_info_t backlight_driving_info_s;

//����������Ҫ����ʵ���������
const uint8_t backlight_level_cnt[BACKLIGHT_MAX_SUPPORT_LEVEL] = {15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0};

/****************************************************************
** Function name:           Backlight_SetPWM
** Descriptions:            �������
** input parameters:        set_level:���ñ���ȼ�
** output parameters:       ��
** Returned value:          ��
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
    {   //��ͬ�ȼ����ٵ���
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
** Descriptions:            �رձ���
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2018-08-07
*****************************************************************/ 
void Backlight_Close(void)
{
    PIN_LOW(BACKLIGHT_PWM_PIN);
    //3ms����ʱȷ���رգ���ֹ�����������µȼ�����
    nrf_delay_ms(3);    
    
    //��ʼ������
    backlight_driving_info_s.state = 0;
    backlight_driving_info_s.level = 0;
}

/****************************************************************
** Function name:           Backlight_Close
** Descriptions:            �رձ���
** input parameters:        set_level:���ñ���ȼ�
** output parameters:       ��
** Returned value:          ��
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
    {   //����δ����
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
** Descriptions:            �����������
** input parameters:        set_level:���ñ���ȼ�
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2018-08-13
*****************************************************************/ 
void Backlight_TestSetLeve(uint8_t set_level)
{
    uint8_t pwm_set_cnt = 0;
    
    PIN_LOW(BACKLIGHT_PWM_PIN);
    //3ms����ʱȷ���رգ���ֹ�����������µȼ�����
    nrf_delay_ms(3);  

    if(set_level == 0)
    {
        return;
    }
    else if(set_level > BACKLIGHT_MAX_SUPPORT_LEVEL)
    {
        set_level = BACKLIGHT_MAX_SUPPORT_LEVEL;
    }   

    //����δ����
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


