/***************************Copyright(c)***********************************
**                       Veepoo Tech
**                  COPYRIGHT (C) 2016-2019
**-----------File Info-----------------------------------------------------
** File        name:     watch_dog.c
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
#ifdef NRF51
    #include "nrf51.h"
#else
    #include "nrf52.h"
#endif
#include "app_error.h"
#include "nrf_drv_wdt.h"

/*********************************************************************
* Macro Definition
*/
#define APP_WDT_TIMEOUT     5000   //5s

/*********************************************************************
* Global Variables
*/
static nrf_drv_wdt_channel_id m_channel_id;

/*********************************************************************
* local functions
*/



/****************************************************************
** Function name:           Wdt_EventHandler
** Descriptions:            
** input parameters:
** output parameters:
** Returned value:
** Created by:              WJ              
** Created Date:            2016-09-12
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Wdt_EventHandler(void)
{
    NVIC_SystemReset();     //reset
    //NOTE: The max amount of time we can spend in WDT interrupt is two cycles of 32768[Hz] clock - after that, reset occurs
}

/****************************************************************
** Function name:           Wdt_Init
** Descriptions:            
** input parameters:
** output parameters:
** Returned value:
** Created by:              WJ              
** Created Date:            2016-09-12
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Wdt_Init(void)
{
    uint32_t err_code = NRF_SUCCESS;
    
        //Configure WDT.
    nrf_drv_wdt_config_t config = NRF_DRV_WDT_DEAFULT_CONFIG;
    config.reload_value = APP_WDT_TIMEOUT;
    
    err_code = nrf_drv_wdt_init(&config, Wdt_EventHandler);
    APP_ERROR_CHECK(err_code);
    err_code = nrf_drv_wdt_channel_alloc(&m_channel_id);
    APP_ERROR_CHECK(err_code);
    nrf_drv_wdt_enable();
}

/****************************************************************
** Function name:           Wdt_Reload
** Descriptions:            ø¥√≈π∑Œππ∑
** input parameters:
** output parameters:
** Returned value:
** Created by:              WJ              
** Created Date:            2016-09-12
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Wdt_Reload(void)
{
    nrf_drv_wdt_channel_feed(m_channel_id);
}
