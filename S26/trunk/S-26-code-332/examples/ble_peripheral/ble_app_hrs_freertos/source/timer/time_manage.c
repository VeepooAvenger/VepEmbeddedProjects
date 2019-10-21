/*******************Copyright(c)*********************************
** Veepoo Tech
** COPYRIGHT (C) 2016-2019
**----------------------FileInfo---------------------------------
** File name:                  time_manage.c
** Last modified Date:         2017-07-15
** Last SYS_VERSION:           0.0.1
** Descriptions:
**---------------------------------------------------------------
** Createdby:                  GT
** Createddate:                2017-07-15
** SYS_VERSION:                0.0.1
** Descriptions:               定时器初始化
**---------------------------------------------------------------
** Modified by:				
** Modified date:			
** SYS_VERSION:				
** Modified Descriptions:	
****************************************************************/

#include "app_type.h"
#include "time_manage.h"
#include "bsp_btn_ble.h"
/*********************************************************************
* Macro Definition
*/

#define APP_TIMER_OP_QUEUE_SIZE       20//15      // Size of timer operation queues.

/****************************************************************
** Function name:               timers_init
** Descriptions:                定时器初始化
** input parameters:            无
** output parameters:           无
** Returned value:              无
** Created by:                  GT     
** Created Date:                2017-07-15
**---------------------------------------------------------------
** Modified by:                  
** Modified date:              
** Modified Descriptions:
**---------------------------------------------------------------
****************************************************************/
void timers_init(void)
{   //app_timer works with RTC1 and nrv_drv_timer uses configurable TIMER0 and/or TIMER1 and/or TIMER2
   
    // Initialize timer module.
    APP_TIMER_INIT(APP_TIMER_PRESCALER, APP_TIMER_OP_QUEUE_SIZE, false);
    
    // Create timers.                                                        

    App_Ancs_Create_Time();
}
