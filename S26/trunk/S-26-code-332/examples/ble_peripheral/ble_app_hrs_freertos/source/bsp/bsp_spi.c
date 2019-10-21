/*****************************************************************
Copyright: 2012-2016, Veepoo Tech. Co., Ltd.
File name:         bsp_spi.c
Description: 
Author:             WJ
Version: 
Date:             2016-09-14
History: 
*****************************************************************/
#include "board.h"
#include "nrf_drv_spi.h"
#include "app_util_platform.h"
#include "app_error.h"
#include <string.h>
#include "bsp_spi.h"
#include "gui_thread.h"

/*********************************************************************
* 宏定义
*/
#if  SPI0_ENABLED
#define SPI0_INSTANCE  0 /**< SPI instance index. */
#endif  //SPI0_ENABLED

#if  SPI1_ENABLED
#define SPI1_INSTANCE  1 /**< SPI instance index. */
#endif  //SPI1_ENABLED

#if  SPI2_ENABLED
#define SPI2_INSTANCE  2 /**< SPI instance index. */
#endif  //SPI2_ENABLED


#if  SPI0_ENABLED
// SPI0
#define SPI0_SS_PIN         G_CS_PIN
#define SPI0_MISO_PIN       G_SO_PIN
#define SPI0_MOSI_PIN       G_SI_PIN
#define SPI0_SCK_PIN        G_SCLK_PIN
#endif //SPI0_ENABLED
        
#if  SPI1_ENABLED
// SPI1
#define SPI1_SS_PIN         AFE_SEN
#define SPI1_MISO_PIN       AFE_SO
#define SPI1_MOSI_PIN       AFE_SI
#define SPI1_SCK_PIN        AFE_SCLK
#endif //SPI0_ENABLED

#if  SPI3_ENABLED
// SPI1
#define SPI1_SS_PIN         LCD_CS_PIN
#define SPI1_MISO_PIN       LCD_SDOUT_PIN
#define SPI1_MOSI_PIN       LCD_SDIN_PIN
#define SPI1_SCK_PIN        LCD_SCLK_PIN
#endif //SPI0_ENABLED   

/*********************************************************************
* 静态变量
*/
#if  SPI0_ENABLED
static const nrf_drv_spi_t spi0 = NRF_DRV_SPI_INSTANCE(SPI0_INSTANCE);  /**< SPI instance. */
#endif //SPI0_ENABLED

#if  SPI1_ENABLED
static const nrf_drv_spi_t spi1 = NRF_DRV_SPI_INSTANCE(SPI1_INSTANCE);  /**< SPI instance. */
#endif //SPI1_ENABLED

#if  SPI2_ENABLED
static uint8_t spi2_finished = true;
static const nrf_drv_spi_t spi2 = NRF_DRV_SPI_INSTANCE(SPI2_INSTANCE);  /**< SPI instance. */
uint8_t spi2_cs_flash = 0;
uint8_t spi2_cs_gt24 = 0;
uint8_t spi2_cs_lcd = 0;
#endif //SPI2_ENABLED

/****************************************************************
** Function name:            spi_event_handler
** Descriptions:            
** input parameters:
** output parameters:
** Returned value:
** Created by:                WJ              
** Created Date:               2016-09-12
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void spi_event_handler(nrf_drv_spi_evt_t const * p_event)
{
    
}
#if  SPI0_ENABLED
/****************************************************************
** Function name:            Bps_Spi0Init
** Descriptions:            
** input parameters:
** output parameters:
** Returned value:
** Created by:                WJ              
** Created Date:               2016-09-12
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Bps_Spi0Init(void)
{
    nrf_drv_spi_config_t spi_config = NRF_DRV_SPI_DEFAULT_CONFIG;
    spi_config.miso_pin = SPI0_MISO_PIN;
    spi_config.mosi_pin = SPI0_MOSI_PIN;
    spi_config.sck_pin  = SPI0_SCK_PIN;
    spi_config.frequency = NRF_DRV_SPI_FREQ_8M;
    APP_ERROR_CHECK(nrf_drv_spi_init(&spi0, &spi_config, NULL));
//    nrf_gpio_cfg_input(spi_config.miso_pin, NRF_GPIO_PIN_PULLUP);//WJ.20170725.flash驱动能力不够
}

/****************************************************************
** Function name:            Bps_Spi0Write
** Descriptions:            
** input parameters:
** output parameters:
** Returned value:          1:true,0:false
** Created by:                WJ              
** Created Date:               2016-09-12
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
uint8_t Bps_Spi0Write(const uint8_t *data, uint16_t length)
{
    return nrf_drv_spi_transfer(&spi0, data, length, NULL, 0);
}

/****************************************************************
** Function name:            Bps_Spi0Read
** Descriptions:            
** input parameters:
** output parameters:
** Returned value:          1:true,0:false
** Created by:                WJ              
** Created Date:               2016-09-12
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
uint8_t Bps_Spi0Read(uint8_t *data, uint16_t length)
{
    return nrf_drv_spi_transfer(&spi0, NULL, 0, data, length);
}

/****************************************************************
** Function name:            Bps_Spi0ReadWrite
** Descriptions:            spi write and read
** input parameters:
** output parameters:
** Returned value:          1:true,0:false
** Created by:                WJ              
** Created Date:               2016-09-12
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/
uint8_t Bps_Spi0ReadWrite(const uint8_t *input, uint16_t input_len, uint8_t *output, uint16_t output_len)
{
    return nrf_drv_spi_transfer(&spi0, input, input_len, output, output_len);
}
#endif  //SPI0_ENABLED

#if  SPI1_ENABLED
/****************************************************************
** Function name:           Bps_Spi1Init
** Descriptions:            初始化
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2016-09-12
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Bps_Spi1Init(void)
{
    nrf_drv_spi_config_t spi_config = NRF_DRV_SPI_DEFAULT_CONFIG;;
    spi_config.miso_pin = SPI1_MISO_PIN;
    spi_config.mosi_pin = SPI1_MOSI_PIN;
    spi_config.sck_pin  = SPI1_SCK_PIN;
    spi_config.frequency = NRF_DRV_SPI_FREQ_8M;
    APP_ERROR_CHECK(nrf_drv_spi_init(&spi1, &spi_config, NULL));
}

/****************************************************************
** Function name:           Bps_Spi1Write
** Descriptions:            写
** input parameters:        data:数据；length：长度
** output parameters:       无
** Returned value:          1:true,0:false
** Created by:              WJ              
** Created Date:            2016-09-12
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
uint8_t Bps_Spi1Write(const uint8_t *data, uint16_t length)
{
    uint32_t error_code = 0;
    
    error_code = nrf_drv_spi_transfer(&spi1, data, length, NULL, 0);
    APP_ERROR_CHECK(error_code);
    return (error_code == 0) ? 1:0;
}

/****************************************************************
** Function name:           Bps_Spi1Read
** Descriptions:            读
** input parameters:        data:数据；length：长度
** output parameters:       无
** Returned value:          1:true,0:false
** Created by:              WJ              
** Created Date:            2016-09-12
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
uint8_t Bps_Spi1Read(uint8_t *data, uint16_t length)
{
    uint32_t error_code = 0;
    
    error_code = nrf_drv_spi_transfer(&spi1, NULL, 0, data, length);
    APP_ERROR_CHECK(error_code);
    return (error_code == 0) ? 1:0;
}

/****************************************************************
** Function name:           Bps_Spi1ReadWrite
** Descriptions:            读写
** input parameters:        input:写数据；input_len：写长度
** output parameters:       output:读数据；output_len：读长度
** Returned value:          1:true,0:false
** Created by:              WJ              
** Created Date:            2016-09-12
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/
uint8_t Bps_Spi1ReadWrite(const uint8_t *input, uint16_t input_len, uint8_t *output, uint16_t output_len)
{
    uint32_t error_code = 0;
    
    error_code = nrf_drv_spi_transfer(&spi1, input, input_len, output, output_len);
    APP_ERROR_CHECK(error_code);
    return (error_code == 0) ? 1:0;
}

#endif  //SPI1_ENABLED

#if  SPI2_ENABLED

static uint8_t spi2_init_flag = 0;

/****************************************************************
** Function name:           Bps_Spi2EventHandler
** Descriptions:            Spim2回调中断
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2018-07-20
*****************************************************************/ 
static void Bps_Spi2EventHandler(nrf_drv_spi_evt_t const * p_event)
{
    spi2_finished = true;
    
    GuiThread_SpiWeakUp();
}

/****************************************************************
** Function name:           Bsp_Spim2IsBuys
** Descriptions:            Spim2是否忙碌
** input parameters:        无
** output parameters:       无
** Returned value:          0：空闲；1：忙碌
** Created by:              WJ              
** Created Date:            2018-07-20
*****************************************************************/ 
uint8_t Bsp_Spim2IsBuys(void)
{
    if(spi2_finished == true)
    {
        return 0;
    }
    return 1;
}


/****************************************************************
** Function name:           Bps_Spi2Init
** Descriptions:            初始化
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2016-09-12
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Bps_Spi2Init(void)
{   
    if(spi2_init_flag == 0)
    {    
        *(volatile uint32_t *)0x40023FFC = 0;
        *(volatile uint32_t *)0x40023FFC;
        *(volatile uint32_t *)0x40023FFC = 1;
        nrf_drv_spi_config_t spi_config = NRF_DRV_SPI_DEFAULT_CONFIG;;
        spi_config.miso_pin = FLASH_SO_PIN;
        spi_config.mosi_pin = FLASH_SI_PIN;
        spi_config.sck_pin  = FLASH_SCLK_PIN;
        spi_config.frequency = NRF_DRV_SPI_FREQ_8M;
        APP_ERROR_CHECK(nrf_drv_spi_init(&spi2, &spi_config, Bps_Spi2EventHandler));  
        nrf_gpio_cfg_input(spi_config.miso_pin, NRF_GPIO_PIN_PULLUP);//WJ.20170725.flash驱动能力不够  
            
        spi2_init_flag = 1;
        
        spi2_finished = true;
    }
}

void Bps_Spi2UnInit(void)
{
    if(spi2_init_flag == 1)
    {
        nrf_drv_spi_uninit(&spi2);
        spi2_init_flag = 0;
        
        *(volatile uint32_t *)0x40023FFC = 0;
        *(volatile uint32_t *)0x40023FFC;
        *(volatile uint32_t *)0x40023FFC = 1;
            
        PIN_HIGH(FLASH_CS_PIN);
        PIN_HIGH(LCD_CS_PIN);
        
        PIN_HIGH(FLASH_SO_PIN);
        PIN_HIGH(FLASH_SI_PIN);
        PIN_HIGH(FLASH_SCLK_PIN);
    }
}

/****************************************************************
** Function name:           Bps_Spi2Write
** Descriptions:            写
** input parameters:        data:数据；length：长度
** output parameters:       无
** Returned value:          1:true,0:false
** Created by:              WJ              
** Created Date:            2016-09-12
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
uint8_t Bps_Spi2Write(const uint8_t *data, uint16_t length)
{
    uint32_t error_code = 0;
    
    Bps_Spi2Init();
    
    while(Bsp_Spim2IsBuys());
    spi2_finished = false;
    
    error_code = nrf_drv_spi_transfer(&spi2, data, length, NULL, 0);
    APP_ERROR_CHECK(error_code);

    GuiThread_SpiSleep();
    while(Bsp_Spim2IsBuys());
    
    return (error_code == 0) ? 1:0;
}

/****************************************************************
** Function name:           Bps_Spi2Read
** Descriptions:            读
** input parameters:        data:数据；length：长度
** output parameters:       无
** Returned value:          1:true,0:false
** Created by:              WJ              
** Created Date:            2016-09-12
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
uint8_t Bps_Spi2Read(uint8_t *data, uint16_t length)
{
    uint32_t error_code = 0;
    
    Bps_Spi2Init();
    
    while(Bsp_Spim2IsBuys());
    spi2_finished = false;
        
    error_code = nrf_drv_spi_transfer(&spi2, NULL, 0, data, length);
    APP_ERROR_CHECK(error_code);

    GuiThread_SpiSleep();
    while(Bsp_Spim2IsBuys());
    
    return (error_code == 0) ? 1:0;
}

/****************************************************************
** Function name:           Bps_Spi2ReadWrite
** Descriptions:            读写
** input parameters:        input:写数据；input_len：写长度
** output parameters:       output:读数据；output_len：读长度
** Returned value:          1:true,0:false
** Created by:              WJ              
** Created Date:            2016-09-12
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/
uint8_t Bps_Spi2ReadWrite(const uint8_t *input, uint16_t input_len, uint8_t *output, uint16_t output_len)
{
    uint32_t error_code = 0;
    
    error_code = nrf_drv_spi_transfer(&spi2, input, input_len, output, output_len);
    APP_ERROR_CHECK(error_code);
    return (error_code == 0) ? 1:0;
}

/****************************************************************
** Function name:           Bps_Spi2SelectGt24
** Descriptions:            选择和释放字库SPI
** input parameters:        flag：0，释放；1：占用
** output parameters:       无
** Returned value:          1:true,0:false
** Created by:              WJ              
** Created Date:            2016-09-12
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/
void Bps_Spi2SelectGt24(uint8_t flag)
{
    if(flag)
    {
        PIN_HIGH(FLASH_CS_PIN);
        PIN_HIGH(LCD_CS_PIN);
        spi2_cs_flash = 0;
        spi2_cs_lcd = 0;
        spi2_cs_gt24 = 1;
        PIN_LOW(GT24_CS_PIN);
    }
    else
    {
        PIN_HIGH(GT24_CS_PIN);
        spi2_cs_gt24 = 0;
    }
}

/****************************************************************
** Function name:           Bps_Spi2SelectFlash
** Descriptions:            选择和释放FLASH SPI
** input parameters:        flag：0，释放；1：占用
** output parameters:       无
** Returned value:          1:true,0:false
** Created by:              WJ              
** Created Date:            2016-09-12
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/
void Bps_Spi2SelectFlash(uint8_t flag)
{
    if(flag)
    {
        PIN_HIGH(GT24_CS_PIN);
        PIN_HIGH(LCD_CS_PIN);
        spi2_cs_lcd  = 0;
        spi2_cs_gt24 = 0;
        spi2_cs_flash = 1;
        PIN_LOW(FLASH_CS_PIN);
    }
    else
    {
        spi2_cs_flash = 0;
        PIN_HIGH(FLASH_CS_PIN);
    }
}

void Bps_Spi2SelectLcd(uint8_t flag)
{
    if(flag)
    {
        PIN_HIGH(FLASH_CS_PIN);
        PIN_HIGH(GT24_CS_PIN);
        spi2_cs_flash = 0;
        spi2_cs_gt24 = 0;
        spi2_cs_lcd = 1;
        PIN_LOW(LCD_CS_PIN);
    }
    else
    {
        PIN_HIGH(LCD_CS_PIN);
        spi2_cs_lcd = 0;
    }
}

/****************************************************************
** Function name:           Bps_Spi0SelectIsBusy
** Descriptions:            SPI是否忙碌
** input parameters:        无
** output parameters:       无
** Returned value:          1:true,0:false
** Created by:              WJ              
** Created Date:            2016-09-12
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/
uint8_t Bps_Spi2SelectIsBusy(void)
{    
    return spi2_cs_flash || spi2_cs_gt24 || spi2_cs_lcd;
}

#endif  //SPI2_ENABLED

