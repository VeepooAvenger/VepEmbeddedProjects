/*****************************************************************
Copyright: 2012-2016, Veepoo Tech. Co., Ltd.
File name:         bsp_iic.c
Description: 
Author:             WJ
Version: 
Date:             2016-09-26
History: 
*****************************************************************/
#include "board.h"
#include "nrf_drv_twi.h"
#include "app_util_platform.h"
#include "app_error.h"
#include <string.h>

/*********************************************************************
* Macro Definition
*/
#define BSP_IIC0_SDA        I2C_DAT_PIN//G_SDA_PIN
#define BSP_IIC0_SCL        I2C_CLK_PIN//G_SCL_PIN

#define BSP_IIC1_SCL        0xFF//I2C_CLK_PIN
#define BSP_IIC1_SDA        0xFF//I2C_DAT_PIN

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
** Function name:            twi_iic0_handler
** Descriptions:            
** input parameters:
** output parameters:
** Returned value:
** Created by:                WJ              
** Created Date:               2016-09-16
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Twi_IIC00_handler(nrf_drv_twi_evt_t const * p_event, void * p_context)
{

}


/****************************************************************
** Function name:            bsp_iic_init
** Descriptions:            
** input parameters:
** output parameters:
** Returned value:
** Created by:                WJ              
** Created Date:               2016-09-16
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Bsp_IIC0_Init(void)
{
    ret_code_t err_code;
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
       .frequency          = NRF_TWI_FREQ_400K,
       .interrupt_priority = APP_IRQ_PRIORITY_HIGH,
       .clear_bus_init     = true
    };

    err_code = nrf_drv_twi_init(&m_twi0, &twi_iic0_config, NULL, NULL);
    APP_ERROR_CHECK(err_code);

    nrf_drv_twi_enable(&m_twi0);
}


/****************************************************************
** Function name:            Bsp_IIC0_Write
** Descriptions:            
** input parameters:
** output parameters:
** Returned value:
** Created by:                WJ              
** Created Date:               2016-09-16
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
uint8_t Bsp_IIC0_Write(const uint8_t address, const uint8_t *data, const uint16_t length)
{
    ret_code_t err_code = nrf_drv_twi_tx(&m_twi0, address, data, length, false);
    APP_ERROR_CHECK(err_code);   
    return (err_code == 0) ? 1:0;
}

/****************************************************************
** Function name:            Bsp_IIC0_Read
** Descriptions:            
** input parameters:
** output parameters:
** Returned value:
** Created by:                WJ              
** Created Date:               2016-09-16
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
uint8_t Bsp_IIC0_Read(const uint8_t address, uint8_t *data, uint16_t length)
{
    /* Read 1 byte from the specified address - skip 3 bits dedicated for fractional part of temperature. */
    ret_code_t err_code = nrf_drv_twi_rx(&m_twi0, address, data, length);
    APP_ERROR_CHECK(err_code);   
    return (err_code == 0) ? 1:0; 
}
#endif      //TWI0_ENABLED


#if TWI1_ENABLED
/****************************************************************
** Function name:            twi_iic1_handler
** Descriptions:            
** input parameters:
** output parameters:
** Returned value:
** Created by:                WJ              
** Created Date:               2016-09-16
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Twi_IIC01_handler(nrf_drv_twi_evt_t const * p_event, void * p_context)
{

}


/****************************************************************
** Function name:            Bsp_IIC1_Init
** Descriptions:            
** input parameters:
** output parameters:
** Returned value:
** Created by:                WJ              
** Created Date:               2016-09-16
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Bsp_IIC1_Init(void)
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
** Function name:            Bsp_IIC0_Write
** Descriptions:            
** input parameters:
** output parameters:
** Returned value:
** Created by:                WJ              
** Created Date:               2016-09-16
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
uint8_t Bsp_IIC1_Write(const uint8_t address, const uint8_t *data, const uint16_t length)
{     
    ret_code_t err_code = nrf_drv_twi_tx(&m_twi1, address, data, length, false);
    APP_ERROR_CHECK(err_code);   
    return (err_code == 0) ? 1:0;
}


/****************************************************************
** Function name:            Bsp_IIC0_Read
** Descriptions:            
** input parameters:
** output parameters:
** Returned value:
** Created by:                WJ              
** Created Date:               2016-09-16
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
uint8_t Bsp_IIC1_Read(const uint8_t address, uint8_t *data, uint16_t length)
{
    /* Read 1 byte from the specified address - skip 3 bits dedicated for fractional part of temperature. */
    ret_code_t err_code = nrf_drv_twi_rx(&m_twi1, address, data, length);
    APP_ERROR_CHECK(err_code);   
    return (err_code == 0) ? 1:0; 
}

#endif      //TWI1_ENABLED

