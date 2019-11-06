/***************************Copyright(c)***********************************
**                       Veepoo Tech
**                  COPYRIGHT (C) 2016-2019
**-----------File Info-----------------------------------------------------
** File        name:     bsp_spi.c
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
#define SPI0_SS_PIN         SI1182_CS
#define SPI0_MISO_PIN       SI1182_SO
#define SPI0_MOSI_PIN       SI1182_SI
#define SPI0_SCK_PIN        SI1182_SCLK
#endif //SPI0_ENABLED
        
#if  SPI1_ENABLED
// SPI1
#define SPI1_SS_PIN         LCD_CS_PIN
#define SPI1_MISO_PIN       LCD_SO_PIN
#define SPI1_MOSI_PIN       LCD_SI_PIN
#define SPI1_SCK_PIN        LCD_SCLK_PIN
#endif //SPI1_ENABLED

#if  SPI2_ENABLED
// SPI2
#define SPI2_SS_PIN         FLASH_DATA_CS_PIN
#define SPI2_MISO_PIN       FLASH_DATA_SO_PIN
#define SPI2_MOSI_PIN       FLASH_DATA_SI_PIN
#define SPI2_SCK_PIN        FLASH_DATA_SCLK_PIN

bool data_flash_busy = false;
bool ui_flash_busy = false;
bool gsensor_flash_busy = false;

#endif //SPI2_ENABLED   

/*********************************************************************
* 静态变量
*/
#if  SPI0_ENABLED
static const nrf_drv_spi_t spi0 = NRF_DRV_SPI_INSTANCE(SPI0_INSTANCE);  /**< SPI instance. */
#endif //SPI0_ENABLED

#if  SPI1_ENABLED
static uint8_t spi1_finished = true;
static const nrf_drv_spi_t spi1 = NRF_DRV_SPI_INSTANCE(SPI1_INSTANCE);  /**< SPI instance. */
#endif //SPI1_ENABLED

#if  SPI2_ENABLED
static uint8_t spi2_finished = true;
static const nrf_drv_spi_t spi2 = NRF_DRV_SPI_INSTANCE(SPI2_INSTANCE);  /**< SPI instance. */
uint8_t spi2_cs_flash = 0;
uint8_t spi2_cs_gt24 = 0;

#endif //SPI2_ENABLED

#if SPI_1_2_USE_DMA
spi_dma_param_t spi_dma_param_s;
#endif //SPI_1_2_USE_DMA


/*********************************************************************
* 函数声明
*/
    
//准备写数据
extern void LCD_DriverPrepareWriteData(void);
//准备读数据
extern uint8_t Extflash_PrepareRead(uint32_t offset);

extern void Extflash_Deselect(void);

extern void LCD_DriverDeselect(void);

uint8_t Spi12_Operation(const uint32_t address, const uint32_t data_len)
{
#if SPI_1_2_USE_DMA
    memset((void*)&spi_dma_param_s, 0, sizeof(spi_dma_param_t));
    
    spi_dma_param_s.flash_addr = address;
    spi_dma_param_s.data_len = data_len;
    if(spi_dma_param_s.data_len >= SPI_MAX_DATA)
    {
        spi_dma_param_s.now_read_len = SPI_MAX_DATA;
    }
    else
    {
        spi_dma_param_s.now_read_len = spi_dma_param_s.data_len;
    }
    
    if(Extflash_PrepareRead(spi_dma_param_s.flash_addr) == 1)
    {
        LCD_DriverPrepareWriteData();
        spi_dma_param_s.start_flg = 1;
        spi_dma_param_s.read_flg = 1;
        spi_dma_param_s.read_buf_flg = 0;
        Bps_Spi2Read(spi_dma_param_s.buf[spi_dma_param_s.read_buf_flg], spi_dma_param_s.now_read_len);
        while(spi_dma_param_s.start_flg == 1)GuiThread_SpiSleep();
    }
    else
    {
        return 0;
    }
#endif //SPI_1_2_USE_DMA
    return 1;
}



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
    spi_config.frequency = NRF_DRV_SPI_FREQ_4M;
    APP_ERROR_CHECK(nrf_drv_spi_init(&spi0, &spi_config, NULL));
    //nrf_gpio_cfg_input(spi_config.miso_pin, NRF_GPIO_PIN_PULLUP);//WJ.20170725.flash驱动能力不够
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
    uint32_t error_code = 0;
    
    error_code = nrf_drv_spi_transfer(&spi0, data, length, NULL, 0);
    APP_ERROR_CHECK(error_code);
    
    return (error_code == 0) ? 1:0;
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
    uint32_t error_code = 0;

    error_code = nrf_drv_spi_transfer(&spi0, NULL, 0, data, length);
    APP_ERROR_CHECK(error_code);
    
    return (error_code == 0) ? 1:0;
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
    
    Bps_Spi0Init();
    
    return nrf_drv_spi_transfer(&spi0, input, input_len, output, output_len);
}


/****************************************************************
** Function name:            Bps_Spi0UnInit
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
void Bps_Spi0UnInit(void)
{
    nrf_drv_spi_uninit(&spi0);
    
    *(volatile uint32_t *)0x40003FFC = 0;
    *(volatile uint32_t *)0x40003FFC;
    *(volatile uint32_t *)0x40003FFC = 1;
                   
    PIN_HIGH(SPI0_SS_PIN);
    PIN_HIGH(SPI0_MISO_PIN);
    PIN_HIGH(SPI0_MOSI_PIN);
    PIN_HIGH(SPI0_SCK_PIN);
}


#endif  //SPI0_ENABLED

#if  SPI1_ENABLED

static uint8_t spi1_init_flag = 0;


/****************************************************************
** Function name:           Spim3_EventHandler
** Descriptions:            Spim3回调中断
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2018-07-20
*****************************************************************/ 
static void Bps_Spi1EventHandler(nrf_drv_spi_evt_t const * p_event)
{
    spi1_finished = true;
#if SPI_1_2_USE_DMA
    if(spi_dma_param_s.start_flg == 0)
    {
        GuiThread_SpiWeakUp();
        return;
    }

    spi_dma_param_s.write_flg = 0;
    spi_dma_param_s.buf_len[spi_dma_param_s.write_buf_flg] = 0;
    
    if(spi2_finished == true && spi_dma_param_s.read_flg == 0)
    {
        //读取数据判断
        for(uint8_t i = 0; i < 2; i++)
        {
            if(spi_dma_param_s.buf_len[i] == 0)
            {
                //判断是否还有数据未读取完成
                if(spi_dma_param_s.now_read_pos < spi_dma_param_s.data_len)
                {
                    spi_dma_param_s.read_flg = 1;
                    spi_dma_param_s.read_buf_flg = i;
                    if(spi_dma_param_s.data_len - spi_dma_param_s.now_read_pos >= SPI_MAX_DATA)
                    {
                        spi_dma_param_s.now_read_len = SPI_MAX_DATA;
                    }
                    else
                    {
                        spi_dma_param_s.now_read_len = spi_dma_param_s.data_len - spi_dma_param_s.now_read_pos;
                    }
                    
                    Bps_Spi2Read(spi_dma_param_s.buf[i], spi_dma_param_s.now_read_len);
                }
                break;
            }
        }
    }
    
    //写数据判断
    for(uint8_t i = 0; i < 2; i++)
    {
        if(spi_dma_param_s.buf_len[i] != 0)
        {
            spi_dma_param_s.write_flg = 1;
            spi_dma_param_s.write_buf_flg = i;
            Bps_Spi1Write(spi_dma_param_s.buf[i] , spi_dma_param_s.buf_len[i]);
            break;
        }
    }
    
    if(spi_dma_param_s.write_flg == 0 && spi_dma_param_s.read_flg == 0)
       //&& spi_dma_param_s.now_read_pos == spi_dma_param_s.data_len)
    {
        memset((void*)&spi_dma_param_s, 0, sizeof(spi_dma_param_t));
        Extflash_Deselect();
        LCD_DriverDeselect();
        GuiThread_SpiWeakUp();
    }
#else
    GuiThread_SpiWeakUp();
#endif //SPI_1_2_USE_DMA
}


/****************************************************************
** Function name:           Bsp_Spim1IsBuys
** Descriptions:            Spim3是否忙碌
** input parameters:        无
** output parameters:       无
** Returned value:          0：空闲；1：忙碌
** Created by:              WJ              
** Created Date:            2018-07-20
*****************************************************************/ 
uint8_t Bsp_Spim1IsBuys(void)
{
    if(spi1_finished == true)
    {
        return 0;
    }
    return 1;
}

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
    if(spi1_init_flag == 0)
    {        
        *(volatile uint32_t *)0x40004FFC = 0;
        *(volatile uint32_t *)0x40004FFC;
        *(volatile uint32_t *)0x40004FFC = 1;
        
        nrf_drv_spi_config_t spi_config = NRF_DRV_SPI_DEFAULT_CONFIG;;
        spi_config.miso_pin = SPI1_MISO_PIN;
        spi_config.mosi_pin = SPI1_MOSI_PIN;
        spi_config.sck_pin  = SPI1_SCK_PIN;
        spi_config.frequency = NRF_DRV_SPI_FREQ_8M;
        APP_ERROR_CHECK(nrf_drv_spi_init(&spi1, &spi_config, Bps_Spi1EventHandler));
    
        spi1_init_flag = 1;
        
        spi1_finished = true;
    }
}


void Bps_Spi1UnInit(void)
{
    if(spi1_init_flag == 1)
    {
        nrf_drv_spi_uninit(&spi1);
        spi1_init_flag = 0;
        
        *(volatile uint32_t *)0x40004FFC = 0;
        *(volatile uint32_t *)0x40004FFC;
        *(volatile uint32_t *)0x40004FFC = 1;
                       
        PIN_HIGH(SPI1_SS_PIN);
        PIN_HIGH(SPI1_MISO_PIN);
        PIN_HIGH(SPI1_MOSI_PIN);
        PIN_HIGH(SPI1_SCK_PIN);
    }
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
    
	Bps_Spi1Init();
	
    while(Bsp_Spim1IsBuys());
    spi1_finished = false;
    
    error_code = nrf_drv_spi_transfer(&spi1, data, length, NULL, 0);
    APP_ERROR_CHECK(error_code);
#if SPI_1_2_USE_DMA
    if(spi_dma_param_s.start_flg == 0)
    {
        while(Bsp_Spim1IsBuys());
    }
#else
    GuiThread_SpiSleep();
    while(Bsp_Spim1IsBuys());
#endif //SPI_1_2_USE_DMA
    
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
	
	Bps_Spi1Init();
    
    while(Bsp_Spim1IsBuys());
    spi1_finished = false;
    
    error_code = nrf_drv_spi_transfer(&spi1, NULL, 0, data, length);
    APP_ERROR_CHECK(error_code);
#if SPI_1_2_USE_DMA
    if(spi_dma_param_s.start_flg == 0)
    {
        while(Bsp_Spim1IsBuys());
    }
#else
    GuiThread_SpiSleep();
    while(Bsp_Spim1IsBuys());
#endif //SPI_1_2_USE_DMA
    
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
    
#if SPI_1_2_USE_DMA  
    if(spi_dma_param_s.start_flg == 0)
    {
        GuiThread_SpiWeakUp();
        return;
    }
    
    spi_dma_param_s.read_flg = 0;
    spi_dma_param_s.buf_len[spi_dma_param_s.read_buf_flg] = spi_dma_param_s.now_read_len;
    spi_dma_param_s.now_read_pos += spi_dma_param_s.now_read_len;
    
    if(spi1_finished == true && spi_dma_param_s.write_flg == 0)
    {
        //写数据判断
        for(uint8_t i = 0; i < 2; i++)
        {
            if(spi_dma_param_s.buf_len[i] != 0)
            {
                spi_dma_param_s.write_flg = 1;
                spi_dma_param_s.write_buf_flg = i;
                Bps_Spi1Write(spi_dma_param_s.buf[i] , spi_dma_param_s.buf_len[i]);
                break;
            }
        }
    }
    
    //读取数据判断
    for(uint8_t i = 0; i < 2; i++)
    {
        if(spi_dma_param_s.buf_len[i] == 0)
        {
            //判断是否还有数据未读取完成
            if(spi_dma_param_s.now_read_pos < spi_dma_param_s.data_len)
            {
                spi_dma_param_s.read_flg = 1;
                spi_dma_param_s.read_buf_flg = i;
                if(spi_dma_param_s.data_len - spi_dma_param_s.now_read_pos >= SPI_MAX_DATA)
                {
                    spi_dma_param_s.now_read_len = SPI_MAX_DATA;
                }
                else
                {
                    spi_dma_param_s.now_read_len = spi_dma_param_s.data_len - spi_dma_param_s.now_read_pos;
                }
                
                Bps_Spi2Read(spi_dma_param_s.buf[i], spi_dma_param_s.now_read_len);
            }
            break;
        }
    }
#else
    GuiThread_SpiWeakUp();
#endif //SPI_1_2_USE_DMA
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
        data_flash_busy = false;
        ui_flash_busy = false;
        gsensor_flash_busy = false;
        
        *(volatile uint32_t *)0x40023FFC = 0;
        *(volatile uint32_t *)0x40023FFC;
        *(volatile uint32_t *)0x40023FFC = 1;
        nrf_drv_spi_config_t spi_config = NRF_DRV_SPI_DEFAULT_CONFIG;;
        spi_config.miso_pin = SPI2_MISO_PIN;
        spi_config.mosi_pin = SPI2_MOSI_PIN;
        spi_config.sck_pin  = SPI2_SCK_PIN;
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
        PIN_HIGH(FLASH_DATA_CS_PIN);  
        PIN_HIGH(G_CS_PIN);
        
        PIN_HIGH(SPI2_SS_PIN);
        PIN_HIGH(SPI2_MISO_PIN);
        PIN_HIGH(SPI2_MOSI_PIN);
        PIN_HIGH(SPI2_SCK_PIN);
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
#if SPI_1_2_USE_DMA
    if(spi_dma_param_s.start_flg == 0)
    {
        while(Bsp_Spim2IsBuys());
    }
#else
    GuiThread_SpiSleep();
    while(Bsp_Spim2IsBuys());
#endif //SPI_1_2_USE_DMA
    
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
#if SPI_1_2_USE_DMA
    if(spi_dma_param_s.start_flg == 0)
    {
        while(Bsp_Spim2IsBuys());
    }
#else
    GuiThread_SpiSleep();
    while(Bsp_Spim2IsBuys());
#endif //SPI_1_2_USE_DMA
    
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

/************************************************************************
** Function name:        Bsp_DataFlashCsLow
** Descriptions:         数据flash选中
** input parameter:      void     
** Returned Value:       void
** Created  by:          JSH
** Created  Datas:       2019-02-13
**************************************************************************/
void Bsp_DataFlashCsLow(void)
{
	data_flash_busy = true;
	PIN_LOW(FLASH_DATA_CS_PIN);
}

/************************************************************************
** Function name:        Bsp_DataFlashCsHigh
** Descriptions:         数据flash不选中
** input parameter:      void     
** Returned Value:       void
** Created  by:          JSH
** Created  Datas:       2019-02-13
**************************************************************************/
void Bsp_DataFlashCsHigh(void)
{
	PIN_HIGH(FLASH_DATA_CS_PIN);
	data_flash_busy = false;
}

/************************************************************************
** Function name:        Bsp_UiFlashCsLow
** Descriptions:         UI flash选中
** input parameter:      void     
** Returned Value:       void
** Created  by:          WJ
** Created  Datas:       2019-04-29
**************************************************************************/
void Bsp_UiFlashCsLow(void)
{
	ui_flash_busy = true;  
	PIN_LOW(FLASH_CS_PIN);  
}

void Bsp_UiFlashCsHigh(void)
{
	PIN_HIGH(FLASH_CS_PIN);
	ui_flash_busy = false;    
}


/************************************************************************
** Function name:        Bsp_UiFlashCsLow
** Descriptions:         Gsensor 选中
** input parameter:      void     
** Returned Value:       void
** Created  by:          WJ
** Created  Datas:       2019-04-29
**************************************************************************/
void Bsp_GsensorFlashCsLow(void)
{
	gsensor_flash_busy = true;  
	PIN_LOW(G_CS_PIN);  
}

void Bsp_GsensorFlashCsHigh(void)
{
	PIN_HIGH(G_CS_PIN);
	gsensor_flash_busy = false;    
}


/************************************************************************
** Function name:        Bsp_Spi2IsBusy
** Descriptions:         SPI2是否忙碌
** input parameter:      void     
** Returned Value:       void
** Created  by:          JSH
** Created  Datas:       2019-02-13
**************************************************************************/
bool Bsp_Spi2IsBusy(void)
{
	if(data_flash_busy == true 
      || ui_flash_busy == true 
      || gsensor_flash_busy == true )
	{
		return true;
	}
	else
	{
		return false;
	}
}

#endif  //SPI2_ENABLED

