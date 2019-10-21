/*******************Copyright(c)*********************************
** Veepoo Tech
** COPYRIGHT (C) 2016-2019
**----------------------FileInfo---------------------------------
** File name:                  app_rtc.c
** Last modified Date:         2016-11-30
** Last SYS_VERSION:           0.0.1
** Descriptions:
**---------------------------------------------------------------
** Createdby:                  WJ
** Createddate:                2016-11-30
** SYS_VERSION:                0.0.1
** Descriptions:               定时器初始化
**---------------------------------------------------------------
** Modified by:				
** Modified date:			
** SYS_VERSION:				
** Modified Descriptions:	
****************************************************************/

#include <string.h>
#include "nordic_common.h"
#include "nrf_drv_rtc.h"
#include "nrf_drv_clock.h"
#include "app_type.h"
#include "SystemTime.h"
#include "bsp_btn_ble.h"

// Get Compare event COMPARE_TIME seconds after the counter starts from 0. 
#define COMPARE_COUNTERTIME  (3UL)     

// Declaring an instance of nrf_drv_rtc for RTC1.
const nrf_drv_rtc_t rtc = NRF_DRV_RTC_INSTANCE(2); 

static uint16_t rct_c_cnt = 0;
/****************************************************************
** Function name:               Rtc_Handler
** Descriptions:                rtc 中断处理
** input parameters:            无
** output parameters:           无
** Returned value:              无
** Created by:                  WJ   
** Created Date:                2016-11-30
**---------------------------------------------------------------
** Modified by:                  
** Modified date:              
** Modified Descriptions:
**---------------------------------------------------------------
****************************************************************/
static void Rtc_Handler(nrf_drv_rtc_int_type_t int_type)
{
    static uint8_t rct_cnt = 0;
    
    if(++rct_cnt == 8)
    {
        //30分钟少一次
        if(++rct_c_cnt >= 1800)
        {
            rct_cnt = 7;
            rct_c_cnt = 0;
            return;
        }
        rct_cnt = 0;
        SystemTime_Update_Second();
    }
    // WJ.20190508. 按键时间精确
    Buttons_PressCalcTime();
}

/****************************************************************
** Function name:               Rtc_Config
** Descriptions:                rtc 配置
** input parameters:            无
** output parameters:           无
** Returned value:              无
** Created by:                  WJ   
** Created Date:                2016-11-30
**---------------------------------------------------------------
** Modified by:                  
** Modified date:              
** Modified Descriptions:
**---------------------------------------------------------------
****************************************************************/
static void Rtc_Config(void)
{
    uint32_t err_code;

    //Initialize RTC instance
    nrf_drv_rtc_config_t config = NRF_DRV_RTC_DEFAULT_CONFIG;
    config.prescaler = 4095;
    err_code = nrf_drv_rtc_init(&rtc, &config, Rtc_Handler);
    APP_ERROR_CHECK(err_code);

    //Enable tick event & interrupt
    nrf_drv_rtc_tick_enable(&rtc,true);

    //Set compare channel to trigger interrupt after COMPARE_COUNTERTIME seconds
    err_code = nrf_drv_rtc_cc_set(&rtc,0,COMPARE_COUNTERTIME*8 ,true);
    APP_ERROR_CHECK(err_code);

    rct_c_cnt = 0;
    //Power on RTC instance
    nrf_drv_rtc_enable(&rtc);
}

/****************************************************************
** Function name:               App_RTC_Init
** Descriptions:                RTC 配置
** input parameters:            无
** output parameters:           无
** Returned value:              无
** Created by:                  WJ   
** Created Date:                2016-11-30
**---------------------------------------------------------------
** Modified by:                  
** Modified date:              
** Modified Descriptions:
**---------------------------------------------------------------
****************************************************************/
void App_RTC_Init(void)
{
    Rtc_Config();
}






