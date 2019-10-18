/***************************Copyright(c)***********************************
**                       Veepoo Tech
**                  COPYRIGHT (C) 2016-2019
**-----------File Info-----------------------------------------------------
** File        name:     motor_event.c
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
#include <string.h>
#include <stdbool.h>
#include "motor_event.h"
#include "board.h"
#include "app_type.h"
#include "app_config_data.h"

#if APP_MOTOR_ENABLED

#define MOTOR_EN_OFF()  PIN_LOW(MOTOR_EN_PIN)
#define MOTOR_EN_ON()   PIN_HIGH(MOTOR_EN_PIN)


extern system_flag_t system_flag_s;

static motor_event_t motor_event_s = {0x00,};
static motor_event_t motor_event_ctr_s = {0x00,};
static uint8_t motor_event_updat = 0; 


static uint8_t motor_on_flag = 0;
extern power_statistics_t power_statistics_s;

/****************************************************************
** Function name:           Motor_Init
** Descriptions:            马达初始化
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2016-11-14
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Motor_Init(void)
{
    MOTOR_EN_OFF();
    memset((uint8_t *)&motor_event_s, 0, sizeof(motor_event_t));
    memset((uint8_t *)&motor_event_ctr_s, 0, sizeof(motor_event_t));
}

/****************************************************************
** Function name:           Motor_AddEvent
** Descriptions:            增加马达事件
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2016-11-14
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Motor_AddEvent(uint8_t group,uint8_t continuous,uint8_t sustain_time,uint8_t interval)//增加电机事件 
{  
    motor_event_s.continuous   = continuous;
    motor_event_s.group        = group;
    motor_event_s.sustain      = sustain_time;
    motor_event_s.sustain_cnt  = sustain_time;
    motor_event_s.interval     = interval; 
    motor_event_s.interval_cnt = interval;    
    motor_event_s.flag         = true;   
    motor_event_s.update       = true;   
    motor_event_updat = 1;
}

/****************************************************************
** Function name:           Motor_DelEvent
** Descriptions:            删除马达事件
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2016-11-14
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Motor_DelEvent(void)
{
    memset((uint8_t *)&motor_event_s, 0, sizeof(motor_event_t));
    motor_event_s.update = true;
    motor_event_updat = 1;
    
    
    motor_on_flag = 0;
}

/****************************************************************
** Function name:           Motor_EventRun
** Descriptions:            马达事件监听（一秒执行）
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2016-11-14
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Motor_EventRun(void)//
{
    if( motor_event_updat == 1)
    {
        motor_event_updat = 0;
        memcpy(&motor_event_ctr_s, &motor_event_s, sizeof(motor_event_t));
    }
    
    if(motor_event_ctr_s.flag)
    { 
        
        if(motor_on_flag == 1)
        {
            // 马达震动次数
            power_statistics_s.motor_vibration_time++;     
        }
        
        
        if(motor_event_ctr_s.sustain_cnt==0x00)
        {
            MOTOR_EN_OFF();
            motor_on_flag = 0; 
        }

        if(motor_event_ctr_s.sustain_cnt > 0)
        {
            motor_event_ctr_s.sustain_cnt--;
            MOTOR_EN_ON(); 
            
            // 马达震动次数
            if(motor_on_flag == 0)
            {
                power_statistics_s.motor_vibration_num++;   
            }        
            motor_on_flag = 1;  
        }
        else
        {
            if(motor_event_ctr_s.interval_cnt>0)
            {
                motor_event_ctr_s.interval_cnt--;

                if(motor_event_ctr_s.interval_cnt==0x00)
                {
                    motor_event_ctr_s.interval_cnt = motor_event_ctr_s.interval;
                    motor_event_ctr_s.sustain_cnt  = motor_event_ctr_s.sustain;

                    if(motor_event_ctr_s.continuous) //如果是1 分组振动 如果是0 持续振动
                    {            
                        if(motor_event_ctr_s.group > 0)
                        {
                            motor_event_ctr_s.group--;
                            if(motor_event_ctr_s.group == 0x00)
                            {
                                Motor_DelEvent();    
                                motor_on_flag = 0;                
                            }
                        }
                    }
                }
            }
            else
            {
                Motor_DelEvent();
                motor_on_flag = 0;   
            }
        }
    }
    else
    {
        if(motor_event_ctr_s.update)
        {
            MOTOR_EN_OFF();
            motor_event_ctr_s.update = false;
            system_flag_s.motor_type = 0;
            motor_on_flag = 0; 
        }
    }  
}

/****************************************************************
** Function name:           Sleep_GetDailyData
** Descriptions:            开启马达
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2016-11-14
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Motor_PowerOn(void)
{
    memset((uint8_t *)&motor_event_s, 0, sizeof(motor_event_t));
    memset((uint8_t *)&motor_event_ctr_s, 0, sizeof(motor_event_t));
    MOTOR_EN_ON();   
}

/****************************************************************
** Function name:           Sleep_GetDailyData
** Descriptions:            关闭马达
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2016-11-14
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Motor_PowerOFF(void)
{    
    memset((uint8_t *)&motor_event_s, 0, sizeof(motor_event_t));    
    memset((uint8_t *)&motor_event_ctr_s, 0, sizeof(motor_event_t));
    MOTOR_EN_OFF();    
}

/****************************************************************
** Function name:           Motor_IsWork
** Descriptions:            马达是否工作
** input parameters:        无
** output parameters:       无
** Returned value:          返还马达状态
** Created by:              WJ              
** Created Date:            2016-11-14
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
uint8_t Motor_IsWork(void)
{
    return motor_event_s.flag;
}


#endif //APP_MOTOR_ENABLED

