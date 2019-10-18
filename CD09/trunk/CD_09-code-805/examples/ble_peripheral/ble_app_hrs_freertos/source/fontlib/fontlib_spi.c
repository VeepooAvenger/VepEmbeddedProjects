/***************************Copyright(c)***********************************
**                       Veepoo Tech
**                  COPYRIGHT (C) 2016-2019
**-----------File Info-----------------------------------------------------
** File        name:     fondlib_spi.c
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

#include "fontlib_spi.h"
#include "nrf_delay.h"
#include "bsp_spi.h"
#include "HFMA2Ylib.h"
#include "board.h"

#include "stdio.h"
#include "nrf_gpio.h"
//#define FONT_SIMULATION_SPI      //Using a simulation SPI
#include "ext_flash.h"



/****************************************************************
** Function name:		    Font_CsInit	
** Descriptions:            字库Spi  CS初始化
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ             
** Created Date:            2016.12.28          
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void SPIWriteData(uint32_t tData)
{
    Bps_Spi2Write((uint8_t *)&tData, 4);
//    GT24_CS_HIGH;
}

/****************************************************************
** Function name:           Fontlib_CsSelect
** Descriptions:            字库芯片SPI使能
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ             
** Created Date:            2016.11.14       
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void SPIWriteComm(uint8_t tData)
{
    Bps_Spi2Write((uint8_t *)&tData, 1);
//    GT24_CS_LOW;
//    nrf_delay_us(5);  
//    Bps_Spi0SelectGt24(1); 
}

/****************************************************************
** Function name:           Fontlib_CsNoSelect
** Descriptions:            字库芯片SPI失能
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ             
** Created Date:            2016.11.14    
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void  Fontlib_Sleep(void)
{  
//    GT24_CS_HIGH;
//    nrf_delay_us(5);  
//    Bps_Spi0SelectGt24(0);
}

/****************************************************************
** Function name:           Fontlib_SpiWrite
** Descriptions:            SPI Write
** input parameters:        data：写取数据；length:读取长度
** output parameters:       无
** Returned value:          返回0：失败；1成功
** Created by:              WJ             
** Created Date:            2016.11.14       
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/
void  Fontlib_Wakeup(void)
{
//    SPIWriteComm(0xAB);
//    nrf_delay_us(50);
}

///****************************************************************
//** Function name:               SPIRead_data_bytes
//** Descriptions:                spi读取字节
//** input parameters:            无
//** output parameters:           无
//** Returned value:              数值
//** Created by:                  WJ    
//** Created Date:                2016-11-14
//**---------------------------------------------------------------
//** Modified by:                  
//** Modified date:              
//** Modified Descriptions:
//**---------------------------------------------------------------
//****************************************************************/
//static uint8_t SPIRead_data_bytes()
//{
//    static uint8_t Data,tData;
//    
//	nrf_gpio_pin_set(FONT_SPI_SCK);
//	
//    for(uint8_t i = 0; i < 8; i++)
//    {
//		nrf_delay_us(1);
//        nrf_gpio_pin_clear(FONT_SPI_SCK);
//        nrf_delay_us(1);
//        if(nrf_gpio_pin_read(FONT_SPI_MISO))
//        {
//            Data=Data|1;
//        }
//        tData    =Data;
//        Data = Data << 1;
//        
//        nrf_gpio_pin_set(FONT_SPI_SCK);	
//    }
//    return tData;
//}

/****************************************************************
** Function name:               r_dat_bat
** Descriptions:                读多个数据
** input parameters:            TAB_addr：        地址
                                Num：             数据个数
                                p_arr：           数据指针
** output parameters:           无
** Returned value:              无
** Created by:                  WJ    
** Created Date:                2016-11-14
**---------------------------------------------------------------
** Modified by:                  
** Modified date:              
** Modified Descriptions:
**---------------------------------------------------------------
****************************************************************/
void r_dat_bat(uint32_t TAB_addr,uint8_t Num,uint8_t *p_arr)
{
//    uint8_t ret = 1;
    static uint32_t addr = 0;
//    static uint8_t buf[5] = {0x00,0x00,0x00,0x00};
    
    addr = TAB_addr; //+ FLASH_FONT_ADDR; 
//    Extflash_SetCsSelect(1);
//    
//    SPIWriteData(TAB_addr);
//    
//    for(uint8_t i = 0; i < Num; i++ )
//    {
//        *(p_arr+i) = SPIRead_data_bytes();
//    }
//    
//    SPI_CSO_HIGH();
    
//    Extflash_SetCsSelect(0);
    Extflash_Read(addr, Num, p_arr);
//    Extflash_SetCsSelect(0);
}


/****************************************************************
** Function name:               Font_Init
** Descriptions:                字库初始化
** input parameters:            无
** output parameters:           无
** Returned value:              无
** Created by:                  WJ    
** Created Date:                2016-11-14
**---------------------------------------------------------------
** Modified by:                  
** Modified date:              
** Modified Descriptions:
**---------------------------------------------------------------
****************************************************************/
void Fontlib_Init(void)
{
//    // 管脚初始化 
//    nrf_gpio_cfg_output(FONT_SPI_CSO);
//    nrf_gpio_pin_clear(FONT_SPI_CSO);
//    nrf_gpio_cfg_output(FONT_SPI_SCK);
//    nrf_gpio_pin_clear(FONT_SPI_SCK);
//    nrf_gpio_cfg_output(FONT_SPI_MOSI);
//    nrf_gpio_pin_clear(FONT_SPI_MOSI);
//    nrf_gpio_cfg_input(FONT_SPI_MISO,NRF_GPIO_PIN_PULLUP);
//    nrf_gpio_pin_clear(FONT_SPI_MISO);
//    
//    nrf_delay_us(25);

//    // 进入睡眠模式 
//    SPIWriteComm(0xB9);
//    nrf_delay_us(25);
    Fontlib_Sleep();
}

/****************************************************************
** Function name:           Font_Utf8ToGbk
** Descriptions:            Utf8 to GBK
** input parameters:        utf8：原始数据
** output parameters:       无
** Returned value:          返回Gbk
** Created by:              WJ      
** Created Date:            2016.11.14   
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/
uint16_t Font_Utf8ToGbk (uint8_t *utf8)   
{
    uint8_t gbk_buff[2] = {0x00, };
    uint16_t gbk_key = 0;
    
	gbk_buff[0] = (*utf8 & 0x0F) << 4 | (((*(utf8+1)) >> 2) & 0x0F);
	gbk_buff[1] = (((*(utf8 + 1))&0x03) << 6 | ((*(utf8 + 2)) & 0x3F));
	gbk_key = (uint16_t)gbk_buff[0] << 8 | gbk_buff[1]; 
	
	return gbk_key;
}

/****************************************************************
** Function name:           Fontlib_Utf8ToUnicode
** Descriptions:            Utf8 to Unicode, Utf8 range around 0x80 to 0x7ff, to change Unicode
** input parameters:        utf8：原始数据
** output parameters:       无
** Returned value:          返回Unicode
** Created by:              WJ      
** Created Date:            2016.11.14   
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/
uint16_t Fontlib_Utf8ToUnicode(uint8_t *utf8) 
{
    uint8_t unicode_buff[2] = {0x00,};
    uint16_t unicode_key = 0;
    
    unicode_buff[0] = *utf8 & 0x1F;
    unicode_buff[1] = (*(utf8 + 1)) & 0x3F;
    unicode_key = (uint16_t)unicode_buff[0] << 6 | unicode_buff[1]; 
    
    return unicode_key;
}

/****************************************************************
** Function name:           Fontlib_AsciiLattice
** Descriptions:            获取点阵数据（ASIIC） 
** input parameters:        p_data：原始数据；
** output parameters:       p_lattice：点阵数据
** Returned value:          无
** Created by:              WJ    
** Created Date:            2016.11.14   
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Fontlib_AsciiLattice(uint8_t *p_lattice, uint32_t type, uint8_t ascii)
{
    ASCII_GetData(ascii, type, p_lattice);	
}

/****************************************************************
** Function name:           Fontlib_ChineseLattice
** Descriptions:            获取点阵数据（中文）
** input parameters:        p_data：原始数据；
** output parameters:       p_lattice：点阵数据
** Returned value:          无
** Created by:              WJ              
** Created Date:            2016-09-16
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/
uint8_t Fontlib_ChineseLattice(uint8_t *p_data, uint8_t *p_lattice)
{	
    uint32_t gbk_code = 0;
    
    gbk_code = U2G(Font_Utf8ToGbk(p_data));
    if(gbk_code == 0x00)
    {
        return 0;
    }

    hzbmp16(SEL_GB, gbk_code, 0, 16, p_lattice);

    return 1;
}

/****************************************************************
** Function name:           Fontlib_KoreanLattice
** Descriptions:            获取点阵数据（韩语）
** input parameters:        p_data：原始数据；
** output parameters:       p_lattice：点阵数据
** Returned value:          无
** Created by:              WJ              
** Created Date:            2016-09-16
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Fontlib_KoreanLattice(uint8_t *p_data, uint8_t *p_lattice)
{
    uint32_t gbk_code = 0;
    
    gbk_code=U2K(Font_Utf8ToGbk(p_data));
    hzbmp16(SEL_KSC, gbk_code, 0, 16, p_lattice);
}

/****************************************************************
** Function name:           Fontlib_JapaneseLattice
** Descriptions:            获取点阵数据（日文）
** input parameters:        p_data：原始数据；
** output parameters:       p_lattice：点阵数据
** Returned value:          无
** Created by:              WJ              
** Created Date:            2016-09-16
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Fontlib_JapaneseLattice(uint8_t *p_data, uint8_t *p_lattice)
{
    uint32_t gbk_code = 0;

    gbk_code=U2J(Font_Utf8ToGbk(p_data));
    hzbmp16(SEL_JIS, gbk_code, 0, 16,p_lattice);
}

/****************************************************************
** Function name:           Fontlib_LattinLattice
** Descriptions:            获取点阵数据（西里拉文）
** input parameters:        p_data：原始数据；
** output parameters:       p_lattice：点阵数据
** Returned value:          无
** Created by:              WJ              
** Created Date:            2016-09-16
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Fontlib_CyrillicLattice(uint8_t *p_data, uint8_t *p_lattice)
{
    CYRILLIC_GetData(Fontlib_Utf8ToUnicode(p_data), p_lattice);///西里尔文（俄语）
}

/****************************************************************
** Function name:           Fontlib_LattinLattice
** Descriptions:            获取点阵数据（拉丁文）
** input parameters:        p_data：原始数据；
** output parameters:       p_lattice：点阵数据
** Returned value:          无
** Created by:              WJ              
** Created Date:            2016-09-16
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Fontlib_LattinLattice(uint8_t *p_data, uint8_t *p_lattice)
{
    if((p_data[0] & 0xE0) == 0xE0)
    {
        LATIN_GetData(Font_Utf8ToGbk(p_data), p_lattice);
    }
    else
    {
        LATIN_GetData(Fontlib_Utf8ToUnicode(p_data), p_lattice);
    }
}

