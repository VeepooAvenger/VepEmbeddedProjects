/***************************Copyright(c)***********************************
**                       Veepoo Tech
**                  COPYRIGHT (C) 2016-2019
**-----------File Info-----------------------------------------------------
** File        name:     alarm_event.c
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
#include <string.h>
#include "nrf_delay.h"
#include "alarm_event.h"
#include "bracelet_protocol.h"
#include "motor_event.h"
#include "gui_data_inteface.h"
#include "time_manage.h"
#include "api_flash.h"


static stopwatch_config_t stopwatch_config = {0x00, };  //秒表
uint8_t stopwatch_calibration_cnt = 0;


/*****************************************************秒表处理********************************************/

/****************************************************************
** Function name:           Watch_100ms_Start
** Descriptions:            用于秒表计时的100毫秒定时器开启
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-05-15
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------*/
void Watch_100ms_Start(void)
{
    Timer_StopWatchStart();
}

/****************************************************************
** Function name:           Watch_100ms_Stop
** Descriptions:            用于秒表计时的100毫秒定时器关闭
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-05-15
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------*/
void Watch_100ms_Stop(void)
{
    Timer_StopWatchStop();
}
 
/****************************************************************
** Function name:           Watch_100msEvent
** Descriptions:            用于秒表计时的100毫秒定时器事件
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-05-15
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------*/
void Watch_100msEvent(void)
{    
    if(++stopwatch_calibration_cnt >= 18)
    {
        stopwatch_calibration_cnt = 0;
        return;
    }
    
    if(++stopwatch_config.stopwatch_100millisecond >=10)
    {
        stopwatch_config.stopwatch_100millisecond = 0;
        stopwatch_config.total_second++ ;
        
        if(stopwatch_config.total_second > 5999 || 
            (stopwatch_config.total_second == 5999 && stopwatch_config.stopwatch_100millisecond >= 9))
        {
            stopwatch_config.total_second = 5999;
            stopwatch_config.stopwatch_100millisecond = 9; 
            stopwatch_config.state = STOPWATCH_STOP;
            Watch_100ms_Stop();
        }
    }
//    App_AddEvent(APP_NULL_EVENT);
}

/****************************************************************
** Function name:           Watch_Init
** Descriptions:            秒表初始化,(关机、按键开启)
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-05-15
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------*/
void Watch_Init(void)
{
    memset(&stopwatch_config, 0x00, sizeof(stopwatch_config_t));
    stopwatch_config.state = STOPWATCH_IDE;
    Timer_StopWatchStop();
    stopwatch_calibration_cnt = 0;
}
 
/****************************************************************
** Function name:           Watch_SetReady
** Descriptions:            设置秒表为就绪状态,(3s长按,初始化完成，并设置秒表为就绪状态)
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-05-15
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------*/
void Watch_SetReady(void)
{
    stopwatch_config.state = STOPWATCH_READY;        //就绪
}
 
/****************************************************************
** Function name:           Watch_Start
** Descriptions:            秒表从就绪、停止状态开启
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-05-15
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------*/
void Watch_Start(void)
{
    if(stopwatch_config.total_second >= 5999 
        && stopwatch_config.stopwatch_100millisecond == 9)
    {           
        return;
    }
    Watch_100ms_Start();
    stopwatch_config.state = STOPWATCH_START;
    stopwatch_config.init_start = 1;
}
 
/****************************************************************
** Function name:           Watch_Stop
** Descriptions:            停止秒表,按键停止或者秒表计时到达上限
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-05-15
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------*/
void Watch_Stop(void)
{
    Timer_StopWatchStop();
    stopwatch_config.state = STOPWATCH_STOP;
    stopwatch_config.init_start = 0;
}
 
/****************************************************************
** Function name:           Watch_GetState
** Descriptions:            获取秒表的当前状态
** input parameters:        无
** output parameters:       无
** Returned value:          无
                            STOPWATCH_IDE,          //准备
                            STOPWATCH_READY,        //就绪
                            STOPWATCH_START,        //开始
                            STOPWATCH_STOP,         //停止
** Created by:              WJ              
** Created Date:            2017-05-15
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------*/
stopwatch_e Watch_GetState(void)
{
    return stopwatch_config.state;
}

/****************************************************************
** Function name:           Watch_IsBusy
** Descriptions:            秒表已经准备就绪,处于忙碌状态
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-05-15
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------*/
uint8_t Watch_IsBusy(void)
{
    return stopwatch_config.state >= STOPWATCH_READY;
}
 
/****************************************************************
** Function name:           Watch_Toggle
** Descriptions:            暂停、继续秒表,亮屏状态下短按按键切换处理
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-05-15
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------*/
void Watch_Toggle(void)
{
    if(stopwatch_config.state == STOPWATCH_STOP 
        || stopwatch_config.state == STOPWATCH_READY)
    {
        Watch_Start();
    }
    else if(stopwatch_config.state == STOPWATCH_START)
    {
        Watch_Stop();
    }
}

/****************************************************************
** Function name:           Watch_GetTotalSecond
** Descriptions:            获取秒表的总秒数,用于ＵＩ显示
** input parameters:        无
** output parameters:       无
** Returned value:          返回:秒表当前的总秒数  0~5999                      
** Created by:              WJ              
** Created Date:            2017-05-15
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------*/
uint16_t Watch_GetTotalSecond(void)
{
    return stopwatch_config.total_second;
}

/****************************************************************
** Function name:           Watch_Get100Millisecond
** Descriptions:            获取秒表当前的100毫秒数
** input parameters:        无
** output parameters:       无
** Returned value:          返回:秒表当前的100毫秒数 0~9                     
** Created by:              WJ              
** Created Date:            2017-05-15
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------*/
uint8_t Watch_Get100Millisecond(void)
{
    return stopwatch_config.stopwatch_100millisecond;
}

/****************************************************************
** Function name:           Watch_IsRun
** Descriptions:            秒表已经开始，秒表运转
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              JSH              
** Created Date:            2018-05-29
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------*/
uint8_t Watch_IsRun(void)
{
    if(stopwatch_config.state == STOPWATCH_START)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/***************************************************秒表处理结束******************************************/


/****************************************************************
** Function name:           Countdown_SendDataToApp
** Descriptions:            发送响应数据到APP,倒计时进行的时候发送
** input parameters:        无
** output parameters:       无
** Returned value:          无              
** Created by:              WJ              
** Created Date:            2017-05-15
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------*/
void Countdown_SendDataToApp(uint8_t ack, uint8_t operate)
{    
    uint8_t ble_ack[20] = {0x00, };
    
    ble_ack[0] = 0xB2;   
    
    if((p_flash_param->hourglass_s.app_trigger == 1)||(p_flash_param->hourglass_s.btn_trigger == 1))
    {
        ble_ack[1] = 3;
    }
    else
    {
        ble_ack[1] = 4;
        p_flash_param->hourglass_s.app_send = 0;
    }
    
    ble_ack[2] = 1;
    
    if(p_flash_param->hourglass_s.ui_enable == 1)
    {
        ble_ack[7] = 1;
        if(p_flash_param->hourglass_s.btn_trigger == 1)
        {
            ble_ack[11] = 1;
        }
    }
    ble_ack[4] = p_flash_param->hourglass_s.dev_remain_time&0xFF;
    ble_ack[5] = (p_flash_param->hourglass_s.dev_remain_time>>8)&0xFF;
    ble_ack[6] = (p_flash_param->hourglass_s.dev_remain_time>>16)&0xFF;
    ble_ack[8] = p_flash_param->hourglass_s.run_remain_time&0xFF;
    ble_ack[9] = (p_flash_param->hourglass_s.run_remain_time>>8)&0xFF;
    ble_ack[10] = (p_flash_param->hourglass_s.run_remain_time>>16)&0xFF;
    
    Ble_SendDataViaBat(ble_ack, 20);    
}
