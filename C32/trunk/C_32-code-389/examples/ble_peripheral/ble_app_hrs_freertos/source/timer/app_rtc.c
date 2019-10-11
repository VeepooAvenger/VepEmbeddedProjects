/***************************Copyright(c)***********************************
**                       Veepoo Tech
**                  COPYRIGHT (C) 2016-2019
**-----------File Info-----------------------------------------------------
** File        name:     app_rtc.c
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
#include "nordic_common.h"
#include "nrf_drv_rtc.h"
#include "nrf_drv_clock.h"
#include "app_type.h"
#include "system_clock.h"
#include "hal_acc_calculate.h"

/*********************************************************************
* Macro Definition
*/
#define COMPARE_COUNTERTIME  (3UL)      /**< Get Compare event COMPARE_TIME seconds after the counter starts from 0. */

/*********************************************************************
* Global Variables
*/
const nrf_drv_rtc_t rtc = NRF_DRV_RTC_INSTANCE(2); /**< Declaring an instance of nrf_drv_rtc for RTC1. */

static uint16_t rct_c_cnt = 0;
/*********************************************************************
* local functions
*/

/****************************************************************
** Function name:           Rtc_Handler
** Descriptions:            rtc中断回调函数
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
        Sclock_OneSecondEventHandler();
    }  
}

/****************************************************************
** Function name:           Rtc_Config
** Descriptions:            rtc配置
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
** Function name:           Rtc_Init
** Descriptions:            初始化rtc
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
void Rtc_Init(void)
{   
    Rtc_Config();
}

/****************************************************************
** Function name:           Rtc_Close
** Descriptions:            关闭RTC时钟
** input parameters:
** output parameters:
** Returned value:
** Created by:              WJ              
** Created Date:            2017-05-24
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Rtc_Close(void)
{
    nrf_drv_rtc_disable(&rtc);
}




