/***************************Copyright(c)***********************************
**                       Veepoo Tech
**                  COPYRIGHT (C) 2016-2019
**-----------File Info-----------------------------------------------------
** File        name:     bsp_iic.c
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
#include "board.h"
#include "nrf_drv_twi.h"
#include "app_util_platform.h"
#include "app_error.h"
#include <string.h>

/*********************************************************************
* Macro Definition
*/
#define BSP_IIC0_SDA        TP_SDA_PIN
#define BSP_IIC0_SCL        TP_SCL_PIN

#define BSP_IIC1_SCL        I2C_CLK_PIN
#define BSP_IIC1_SDA        I2C_DAT_PIN

/* TWI instance ID. */
#define TWI_INSTANCE_ID0     0
#define TWI_INSTANCE_ID1     1 

/*********************************************************************
* Global Variables
*/
/* TWI instance. */
#if TWI0_ENABLED
static const nrf_drv_twi_t m_twi0 = NRF_DRV_TWI_INSTANCE(TWI_INSTANCE_ID0);
#endif

#if TWI1_ENABLED
static const nrf_drv_twi_t m_twi1 = NRF_DRV_TWI_INSTANCE(TWI_INSTANCE_ID1);
#endif
/*********************************************************************
* local functions
*/

#if TWI0_ENABLED
/****************************************************************
** Function name:           Twi_IIC0Handler
** Descriptions:            回调函数
** input parameters:        p_event：状态；p_context：回调执行函数
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2016-09-16
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Twi_IIC0Handler(nrf_drv_twi_evt_t const * p_event, void * p_context)
{

}

/****************************************************************
** Function name:           Bsp_IIC0Init
** Descriptions:            初始化
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2016-09-16
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Bsp_IIC0Init(void)
{
    ret_code_t err_code = 0;
    
    *(volatile uint32_t *)0x40003FFC = 0;
    *(volatile uint32_t *)0x40003FFC;
    *(volatile uint32_t *)0x40003FFC = 1;
 
//*(volatile uint32_t *)0x40004FFC = 0;
//*(volatile uint32_t *)0x40004FFC;
//*(volatile uint32_t *)0x40004FFC = 1;
    
    //set sensing   WJ.20170309     IIC导致死机的问题
    nrf_gpio_cfg_input(BSP_IIC0_SCL, NRF_GPIO_PIN_PULLUP);
    nrf_gpio_cfg_input(BSP_IIC0_SDA, NRF_GPIO_PIN_PULLUP);
    
    const nrf_drv_twi_config_t twi_iic0_config = {
       .scl                = BSP_IIC0_SCL,
       .sda                = BSP_IIC0_SDA,
       .frequency          = NRF_TWI_FREQ_100K,
       .interrupt_priority = APP_IRQ_PRIORITY_HIGH,
       .clear_bus_init     = true
    };

    err_code = nrf_drv_twi_init(&m_twi0, &twi_iic0_config, NULL, NULL);
    APP_ERROR_CHECK(err_code);

    nrf_drv_twi_enable(&m_twi0);
}


/****************************************************************
** Function name:           Bsp_IIC0Write
** Descriptions:            写数据
** input parameters:        address:地址；data：写数组；length：写长度
** output parameters:       无
** Returned value:          1:true,0:false
** Created by:              WJ              
** Created Date:            2016-09-16
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
uint8_t Bsp_IIC0Write(const uint8_t address, const uint8_t *data, const uint16_t length)
{
    ret_code_t err_code = nrf_drv_twi_tx(&m_twi0, address, data, length, false);
    //APP_ERROR_CHECK(err_code);   
    return (err_code == 0) ? 1:0;
}

/****************************************************************
** Function name:           Bsp_IIC0Read      
** Descriptions:            读数据
** input parameters:        address:地址；length：读长度
** output parameters:       data：读取数组
** Returned value:          1:true,0:false
** Created by:              WJ              
** Created Date:            2016-09-16
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
uint8_t Bsp_IIC0Read(const uint8_t address, uint8_t *data, uint16_t length)
{
    /* Read 1 byte from the specified address - skip 3 bits dedicated for fractional part of temperature. */
    ret_code_t err_code = nrf_drv_twi_rx(&m_twi0, address, data, length);
    //APP_ERROR_CHECK(err_code);   
    return (err_code == 0) ? 1:0; 
}
#endif      //TWI0_ENABLED


#if TWI1_ENABLED
/****************************************************************
** Function name:           twi_iic1_handler  
** Descriptions:            回调函数
** input parameters:        p_event：状态；p_context：回调执行函数
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2016-09-16
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Twi_IIC1Handler(nrf_drv_twi_evt_t const * p_event, void * p_context)
{

}


/****************************************************************
** Function name:           Bsp_IIC1Init
** Descriptions:            初始化
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2016-09-16
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Bsp_IIC1Init(void)
{
    ret_code_t err_code;

    const nrf_drv_twi_config_t twi_iic1_config = {
       .scl                = BSP_IIC1_SCL,
       .sda                = BSP_IIC1_SDA,
       .frequency          = NRF_TWI_FREQ_100K,
       .interrupt_priority = APP_IRQ_PRIORITY_HIGH,
       .clear_bus_init     = false
    };

    err_code = nrf_drv_twi_init(&m_twi1, &twi_iic1_config, NULL, NULL);
    APP_ERROR_CHECK(err_code);

    nrf_drv_twi_enable(&m_twi1);
}


/****************************************************************
** Function name:           Bsp_IIC0Write
** Descriptions:            写数据
** input parameters:        address:地址；data：写数组；length：写长度
** output parameters:       无
** Returned value:          1:true,0:false
** Created by:              WJ              
** Created Date:            2016-09-16
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
uint8_t Bsp_IIC1Write(const uint8_t address, const uint8_t *data, const uint16_t length)
{     
    ret_code_t err_code = nrf_drv_twi_tx(&m_twi1, address, data, length, false);
    APP_ERROR_CHECK(err_code);   
    return (err_code == 0) ? 1:0;
}

/****************************************************************
** Function name:           Bsp_IIC1Read
** Descriptions:            读数据
** input parameters:        address:地址；length：读长度
** output parameters:       data：读取数组
** Returned value:          1:true,0:false
** Created by:              WJ              
** Created Date:            2016-09-16
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
uint8_t Bsp_IIC1Read(const uint8_t address, uint8_t *data, uint16_t length)
{
    /* Read 1 byte from the specified address - skip 3 bits dedicated for fractional part of temperature. */
    ret_code_t err_code = nrf_drv_twi_rx(&m_twi1, address, data, length);
    APP_ERROR_CHECK(err_code);   
    return (err_code == 0) ? 1:0; 
}

#endif      //TWI1_ENABLED

