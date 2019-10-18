/***************************Copyright(c)***********************************
**                       Veepoo Tech
**                  COPYRIGHT (C) 2016-2019
**-----------File Info-----------------------------------------------------
** File        name:     outsit.c
** Last modified Date:   2017-08-21
** Last SYS_VERSION:     0.0.0
** Descriptions:            
**-------------------------------------------------------------------------
** Created  by:          WJ
** Created  date:        2017-08-21
** SYS_VERSION:          0.0.0
** Descriptions:         
**-------------------------------------------------------------------------
** Modified by:				
** Modified date:  			          
** SYS_VERSION:                 
** Descriptions:                        
***************************************************************************/
#include <string.h>

#include "outsit.h"
#include "system_clock.h"
#include "app_type.h"
#include "gui_thread.h"
#include "nowear_detection.h"
#include "battery.h"


static uint16_t outsit_step[5] = {0x00, };  //记录最后五分钟计步数据
static uint8_t outsit_step_cnt = 0;         //最后五分钟计步分钟次数
static uint32_t outsit_last_step = 0;

static uint8_t t_outsit_cnt = 0;

extern system_flag_t system_flag_s;
extern system_info_t system_info_s;

/****************************************************************
** Function name:           Outsit_Init
** Descriptions:            久坐初始化
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ             
** Created Date:            2017-08-21
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Outsit_Init(void)
{
    t_outsit_cnt = 0x00;
    memset(outsit_step, 0x00, sizeof(outsit_step));
    outsit_last_step = All_StepGet();
    outsit_step_cnt = 0;
}

/****************************************************************
** Function name:           Outsit_Steps_5m
** Descriptions:            最后五分钟计步处理
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ             
** Created Date:            2017-08-21
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
uint16_t Outsit_Steps_5m(void)
{
    uint16_t step_5m = 0;
    uint8_t i = 0;
    
    if(outsit_step_cnt < 5)
    {
        outsit_step[outsit_step_cnt++] = (uint16_t)(All_StepGet() - outsit_last_step);
    }
    else
    {
        for(i = 0; i < 4; i++)
        {
            outsit_step[i] = outsit_step[i + 1];
        }
        outsit_step[i] = All_StepGet() - outsit_last_step;
    }
       
    outsit_last_step = All_StepGet();        
    step_5m = outsit_step[0] + outsit_step[1] + outsit_step[2] +outsit_step[3] + outsit_step[4];
    
    return step_5m;
}
/****************************************************************
** Function name:           Outsit_Monitor
** Descriptions:            久坐监听
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ             
** Created Date:            2017-08-21
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Outsit_Monitor(void)
{
    outsit_t *outsit = NULL;
    uint8_t time[8] = {0x00,};
    
    Sclock_ReadTimeToBuf(time, 7);
    outsit = &system_info_s.outsit_cfg;
    
    if((!system_flag_s.sign_off ) && (!system_flag_s.sleep_enter) && (system_flag_s.power_state == POWER_NORMAL )
      && (Get_NoWearStation() < 2 || Get_NoWearStation() == 5))//不充电
    {
        if(outsit->on_off_flag) // 0x00 关闭 0x01 打开
        {
            if((memcmp(&time[4], outsit->start_time,2) >= 0)
                &&(memcmp(outsit->end_time, &time[4],2) >= 0)
                && Outsit_Steps_5m() < 13)
            {
                if(t_outsit_cnt < 245)
                {
                    t_outsit_cnt++;//计数
                }
                               
                if(t_outsit_cnt >= outsit->threshold_value)//计数到指定的阈值 触发动作
                {  
                    t_outsit_cnt = 0x00;
                    Gui_ThreadEventTaskWakeUp(CPU_EVENT_TASK_SIT); 
                }
            }
            else
            {
                t_outsit_cnt = 0x00;
            }
        }
        else
        {
            t_outsit_cnt = 0x00;
        }
    }
    else
    {
        t_outsit_cnt = 0x00;
    }
}

/****************************************************************
** Function name:           Outsit_ConfigCheck
** Descriptions:            久坐配置自检
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ             
** Created Date:            2017-08-21
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
uint8_t Outsit_ConfigCheck(outsit_t *outsit_t)
{
    uint16_t start_time = 0;
    uint16_t end_time = 0;
    
    start_time = outsit_t->start_time[0]*60 + outsit_t->start_time[1];
    end_time = outsit_t->end_time[0]*60 + outsit_t->end_time[1];   
    
    //WJ.20171227.修改不能设置240分钟的阈值
    if((outsit_t->on_off_flag > 1) ||
       (outsit_t->threshold_value < 30) ||
       (outsit_t->threshold_value > 240) || 
       (start_time > 17 * 60 + 30) ||    
       (start_time < 8 * 60) ||
       (end_time > 18 * 60) ||    
       (end_time < 8 * 60 + 30) ||
       (start_time + outsit_t->threshold_value > end_time))
    {
        return 0;       
    }    
    
    return 1;
}
