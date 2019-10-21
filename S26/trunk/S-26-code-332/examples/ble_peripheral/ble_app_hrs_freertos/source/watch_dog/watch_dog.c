/*******************Copyright(c)*********************************
** Veepoo Tech
** COPYRIGHT (C) 2016-2019
**----------------------FileInfo---------------------------------
** File name:                  watch_dog.c
** Last modified Date:         2016-09-12
** Last SYS_VERSION:           0.0.1
** Descriptions:
**---------------------------------------------------------------
** Createdby:                  WJ
** Createddate:                2016-11-29
** SYS_VERSION:                0.0.1
** Descriptions:               看门狗
**---------------------------------------------------------------
** Modified by:				
** Modified date:			
** SYS_VERSION:				
** Modified Descriptions:	
****************************************************************/

#ifdef NRF51
#include "nrf51.h"
#else
#include "nrf52.h"
#endif
#include "app_error.h"
#include "nrf_drv_wdt.h"


#define APP_WDT_TIMEOUT     10000   //10s


static nrf_drv_wdt_channel_id m_channel_id;

/****************************************************************
** Function name:             App_Wdt_Event_Handler
** Descriptions:              看门狗事件处理
** input parameters:          无
** output parameters:         无
** Returned value:            无
** Created by:                WJ
** Created Date:              2016-11-29
****************************************************************/
void App_Wdt_Event_Handler(void)
{
    NVIC_SystemReset();     //reset
    //NOTE: The max amount of time we can spend in WDT interrupt is two cycles of 32768[Hz] clock - after that, reset occurs
}

/****************************************************************
** Function name:             App_Watch_Dog_Init
** Descriptions:              看门狗初始化
** input parameters:          无
** output parameters:         无
** Returned value:            无
** Created by:                WJ
** Created Date:              2016-11-29
****************************************************************/
void App_Watch_Dog_Init(void)
{
    uint32_t err_code = NRF_SUCCESS;
    
    //Configure WDT.
    nrf_drv_wdt_config_t config = NRF_DRV_WDT_DEAFULT_CONFIG;
    config.reload_value = APP_WDT_TIMEOUT;
    
    err_code = nrf_drv_wdt_init(&config, App_Wdt_Event_Handler);
    APP_ERROR_CHECK(err_code);
    err_code = nrf_drv_wdt_channel_alloc(&m_channel_id);
    APP_ERROR_CHECK(err_code);
    nrf_drv_wdt_enable();
}

/****************************************************************
** Function name:             App_Watch_Dog_Reload
** Descriptions:              喂狗
** input parameters:          无
** output parameters:         无
** Returned value:            无
** Created by:                WJ
** Created Date:              2016-11-29
****************************************************************/
void App_Watch_Dog_Reload(void)
{
    nrf_drv_wdt_channel_feed(m_channel_id);
}
