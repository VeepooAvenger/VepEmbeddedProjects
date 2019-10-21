/*******************Copyright(c)*********************************
** Veepoo Tech
** COPYRIGHT (C) 2016-2019
**----------------------FileInfo---------------------------------
** File name:                  fontlib_spi.c
** Last modified Date:         2016-11-14
** Last SYS_VERSION:           0.0.1
** Descriptions:
**---------------------------------------------------------------
** Createdby:                  WJ
** Createddate:                2016-11-14
** SYS_VERSION:                0.0.1
** Descriptions:               字库库
**---------------------------------------------------------------
** Modified by:				
** Modified date:			
** SYS_VERSION:				
** Modified Descriptions:	
****************************************************************/

#include "fontlib_spi.h"
#include "nrf_delay.h"
#include "bsp_spi.h"
#include "HFMA2Ylib.h"
#include "board.h"
#include "stdio.h"
#include "nrf_gpio.h"
#include "ext_flash.h"


/****************************************************************
** Function name:               SPIWriteData
** Descriptions:                SPI写数据
** input parameters:            tData：       数据
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
void SPIWriteData(uint32_t tData)
{
    Bps_Spi2Write((uint8_t *)&tData, 4);
//    tData=tData|0x03000000;
//    
//    for(uint8_t i=0; i<32; i++) 
//    {
//        nrf_gpio_pin_clear(FONT_SPI_SCK);
//        
//        if(tData & (0x80000000 >> i))
//        {
//            SPI_SMO_OUT(0x01);
//        }
//        else
//        {
//            SPI_SMO_OUT(0x00);
//        }

//        nrf_gpio_pin_set(FONT_SPI_SCK);
//    }
}

/****************************************************************
** Function name:               SPIWriteComm
** Descriptions:                spi 写数据
** input parameters:            tData:           数据
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
void SPIWriteComm(uint8_t tData)
{
    Bps_Spi2Write((uint8_t *)&tData, 1);
//    uint8_t i;

//    nrf_gpio_pin_clear(FONT_SPI_CSO);
//    
//    for(i=0; i<8; i++) 
//    {
//        nrf_gpio_pin_clear(FONT_SPI_SCK);
//        
//        if(tData & (0x80 >> i))
//        {
//            SPI_SMO_OUT(0x01);
//        }
//        else
//        {
//            SPI_SMO_OUT(0x00);
//        }
//        
//        nrf_gpio_pin_set(FONT_SPI_SCK);
//    }

//    nrf_gpio_pin_set(FONT_SPI_CSO);
//    nrf_delay_us(25);    
}

/****************************************************************
** Function name:               Font_Sleep
** Descriptions:                字库睡眠
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
void  Font_Sleep(void)
{
//    SPIWriteComm(0xB9);
//    nrf_delay_us(50);    
}

/****************************************************************
** Function name:               Font_Wakeup
** Descriptions:                字库唤醒
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
void  Font_Wakeup(void)
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
    Extflash_SetCsSelect(1);
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
    Extflash_SetCsSelect(0);
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
void Font_Init(void)
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
    Font_Sleep();
}

/****************************************************************
** Function name:               Font_AsciiLattice
** Descriptions:                获取字型
** input parameters:            ascii：         字符
                                p_lattice：     字型
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
void Font_AsciiLattice(uint8_t ascii,uint8_t* p_lattice)
{
    ASCII_GetData(ascii,ASCII_8X16,p_lattice);
}

/****************************************************************
** Function name:               Font_ChineseLattice
** Descriptions:                中文字型
** input parameters:            p_data：        字符
                                p_lattice：     字型
** output parameters:           无
** Returned value:              是否成功
** Created by:                  WJ    
** Created Date:                2016-11-14
**---------------------------------------------------------------
** Modified by:                  
** Modified date:              
** Modified Descriptions:
**---------------------------------------------------------------
****************************************************************/
bool Font_ChineseLattice(uint8_t* p_data, uint8_t* p_lattice)
{    
    uint32_t gbk_code = 0;
	
    gbk_code=U2G(UTF8_TO_GBK(p_data));
	
    if(gbk_code  == 0x00)
    {
        return false;
    }
    
    hzbmp16(SEL_GB, gbk_code, 0, 16,p_lattice);
    
    return true;
}

/****************************************************************
** Function name:               Font_KoreanLattice
** Descriptions:                韩语字型
** input parameters:            p_data：        字符
                                p_lattice：     字型
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
void Font_KoreanLattice(uint8_t* p_data, uint8_t* p_lattice)
{
     uint32_t gbk_code = 0;
     gbk_code=U2K(UTF8_TO_GBK(p_data));
     hzbmp16(SEL_KSC, gbk_code, 0, 16,p_lattice);
}

/****************************************************************
** Function name:               Font_JapaneseLattice
** Descriptions:                日语字型
** input parameters:            p_data：        字符
                                p_lattice：     字型
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
void Font_JapaneseLattice(uint8_t* p_data, uint8_t* p_lattice)
{
    uint32_t gbk_code = 0;
        
    gbk_code=U2J(UTF8_TO_GBK(p_data));
    hzbmp16(SEL_JIS, gbk_code, 0, 16,p_lattice);
}

/****************************************************************
** Function name:               Font_CyrillicLattice
** Descriptions:                西里尔字型（俄语）
** input parameters:            p_data：        字符
                                p_lattice：     字型
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
void Font_CyrillicLattice(uint8_t* p_data, uint8_t* p_lattice)
{
     CYRILLIC_GetData(UTF8_TO_UNICODE(p_data),p_lattice);
}

/****************************************************************
** Function name:               Font_LattinLattice
** Descriptions:                拉丁字库
** input parameters:            p_data：        字符
                                p_lattice：     字型
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
void Font_LattinLattice(uint8_t* p_data, uint8_t* p_lattice)
{
    if((p_data[0] & 0xE0) == 0xE0)
    {
        //增加部分
        LATIN_GetData(UTF8_TO_GBK(p_data), p_lattice);
    }
    else
    {
        LATIN_GetData(UTF8_TO_UNICODE(p_data),p_lattice);
    }
}

