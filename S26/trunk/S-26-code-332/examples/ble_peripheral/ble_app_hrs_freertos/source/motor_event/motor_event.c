/*******************Copyright(c)*********************************
** Veepoo Tech
** COPYRIGHT (C) 2016-2019
**----------------------FileInfo---------------------------------
** File name:                  motor_event.c
** Last modified Date:         2016-09-12
** Last SYS_VERSION:           0.0.1
** Descriptions:
**---------------------------------------------------------------
** Createdby:                  GT
** Createddate:                2016-11-29
** SYS_VERSION:                0.0.1
** Descriptions:               震动事件
**---------------------------------------------------------------
** Modified by:				
** Modified date:			
** SYS_VERSION:				
** Modified Descriptions:	
****************************************************************/

#include "motor_event.h"
#include <string.h>
#include <stdbool.h>
#include "board.h"
#include "app_type.h"
#include "time_manage.h"
#include "battery_adc.h"

#define MOTOR_EN_OFF()  do{nrf_gpio_cfg_output(MOTOR_EN_PIN);nrf_gpio_pin_clear(MOTOR_EN_PIN);}while(0)
#define MOTOR_EN_ON()   do{nrf_gpio_cfg_output(MOTOR_EN_PIN);nrf_gpio_pin_set(MOTOR_EN_PIN);}while(0)

static motor_event_t motor_event;


/****************************************************************
** Function name:             MotorGetData
** Descriptions:              获取震动数据
** input parameters:          无
** output parameters:         无
** Returned value:            无
** Created by:                GT
** Created Date:              2016-11-29
****************************************************************/
motor_event_t *MotorGetData(void)//获取马达结构体
{
    return &motor_event;
}

/****************************************************************
** Function name:             MotorGetData
** Descriptions:              添加震动
** input parameters:          group           振动的组数
                              continuous      是否持续振动 若为1 则分组振动（参数group才起作用） 若为0，则持续振动
                              sustain_time    一次振动的持续时间
                              interval        两次振动之间的时间间隔
** output parameters:         无
** Returned value:            无
** Created by:                GT
** Created Date:              2016-11-29
****************************************************************/
void MotorAddEvent(uint8_t group,uint8_t continuous,uint8_t sustain_time,uint8_t interval)//增加电机事件 
{  
	if(Battery_Get_Grade() == 0)
	{
		MotorDelEvent();
		
		return ;
	}
	
    motor_event.flag         = true;
    motor_event.continuous   = continuous;
    motor_event.update       = true;
    motor_event.group        = group;
    motor_event.sustain      = sustain_time;
    motor_event.sustain_cnt  = sustain_time;
    motor_event.interval     = interval; 
    motor_event.interval_cnt = interval;
}

/****************************************************************
** Function name:             MotorDelEvent
** Descriptions:              删除震动
** input parameters:          无
** output parameters:         无
** Returned value:            无
** Created by:                GT
** Created Date:              2016-11-29
****************************************************************/
void MotorDelEvent(void)//删除
{
    memset((uint8_t *)&motor_event,0,sizeof(motor_event_t));
    motor_event.update   = true;
    MOTOR_EN_OFF();
}

/****************************************************************
** Function name:             MotorEventRun
** Descriptions:              震动运行事件
** input parameters:          无
** output parameters:         无
** Returned value:            无
** Created by:                GT
** Created Date:              2016-11-29
****************************************************************/
void MotorEventRun(void)//一秒执行
{
    if(motor_event.flag)
    { 
        if(motor_event.sustain_cnt==0x00)
        {
            MOTOR_EN_OFF();
        }

        if(motor_event.sustain_cnt > 0)
        {
            motor_event.sustain_cnt--;

            MOTOR_EN_ON(); 
        }
        else
        {
            if(motor_event.interval_cnt>0)
            {
                motor_event.interval_cnt--;

                if(motor_event.interval_cnt==0x00)
                {
                    motor_event.interval_cnt = motor_event.interval;
                    motor_event.sustain_cnt  = motor_event.sustain;

                    if(motor_event.continuous) //如果是1 分组振动 如果是0 持续振动
                    {            
                        if(motor_event.group > 0)
                        {
                            motor_event.group--;
                            if(motor_event.group == 0x00)
                            {
                                MotorDelEvent();                 
                            }
                        }
                    }
                }
            }
        }
    }
    else
    {
        if(motor_event.update)
        {
            MOTOR_EN_OFF();
            motor_event.update = false;
        }
    }  
}

/****************************************************************
** Function name:             MotorPowerOn
** Descriptions:              震动上电
** input parameters:          无
** output parameters:         无
** Returned value:            无
** Created by:                GT
** Created Date:              2016-11-29
****************************************************************/
void MotorPowerOn(void)
{
    memset((uint8_t *)&motor_event,0,sizeof(motor_event_t));
    MOTOR_EN_ON();   
}

/****************************************************************
** Function name:             MotorPowerOFF
** Descriptions:              震动掉电
** input parameters:          无
** output parameters:         无
** Returned value:            无
** Created by:                GT
** Created Date:              2016-11-29
****************************************************************/
void MotorPowerOFF(void)
{    
    memset((uint8_t *)&motor_event,0,sizeof(motor_event_t));
    MOTOR_EN_OFF();    
}

