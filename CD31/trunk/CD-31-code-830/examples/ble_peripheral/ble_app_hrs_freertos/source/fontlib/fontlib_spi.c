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
#include "ext_flash.h"

#include "stdio.h"

//#define FONT_SIMULATION_SPI      //Using a simulation SPI

//#define GT24_CS_INIT    PIN_LOW(GT24_CS_PIN)
//#define GT24_CS_HIGH    PIN_HIGH(GT24_CS_PIN)
//#define GT24_CS_LOW     PIN_LOW(GT24_CS_PIN)



/****************************************************************
** Function name:		    Font_CsInit	
** Descriptions:            �ֿ�Spi  CS��ʼ��
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ             
** Created Date:            2016.12.28          
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Font_CsInit(void)
{
//    GT24_CS_HIGH;
}

/****************************************************************
** Function name:           Fontlib_CsSelect
** Descriptions:            �ֿ�оƬSPIʹ��
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ             
** Created Date:            2016.11.14       
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Fontlib_CsSelect(void)
{   
//    GT24_CS_LOW;
//    nrf_delay_us(5);  
//    Bps_Spi2SelectGt24(1); 
}

/****************************************************************
** Function name:           Fontlib_CsNoSelect
** Descriptions:            �ֿ�оƬSPIʧ��
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ             
** Created Date:            2016.11.14    
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Fontlib_CsNoSelect(void)
{  
//    GT24_CS_HIGH;
//    nrf_delay_us(5);  
//    Bps_Spi2SelectGt24(0);
}

/****************************************************************
** Function name:           Fontlib_SpiWrite
** Descriptions:            SPI Write
** input parameters:        data��дȡ���ݣ�length:��ȡ����
** output parameters:       ��
** Returned value:          ����0��ʧ�ܣ�1�ɹ�
** Created by:              WJ             
** Created Date:            2016.11.14       
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/
//static uint8_t Fontlib_SpiWrite(const uint8_t *data, uint16_t length)
//{
//    return Bps_Spi2Write(data, length);
//}

/****************************************************************
** Function name:           Fontlib_SpiRead
** Descriptions:            SPI read 
** input parameters:        length:��ȡ����
** output parameters:       data����ȡ����
** Returned value:          ����0��ʧ�ܣ�1�ɹ�
** Created by:              WJ             
** Created Date:            2016.11.14     
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/
//static uint8_t Fontlib_SpiRead(uint8_t *data, uint16_t length)
//{
//    return Bps_Spi2Read(data, length); 
//}

/****************************************************************
** Function name:           r_dat_bat
** Descriptions:            �ֿ�ص�����ʹ��spi
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ             
** Created Date:            2016.12.28          
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
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
** Function name:           Fontlib_Sleep
** Descriptions:            �ֿ�����
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ             
** Created Date:            2016.12.28          
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Fontlib_Sleep(void)
{
//    static uint8_t data = 0;
//    
//    data = 0xB9;//˯��
//    Fontlib_CsSelect();//ѡ�� low

//    Fontlib_SpiWrite(&data, 1);

//    Fontlib_CsNoSelect(); //CS���� high
//    nrf_delay_us(30);
}

/****************************************************************
** Function name:           Fontlib_Wakeup
** Descriptions:            �ֿ⻽��
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ             
** Created Date:            2016.12.28          
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Fontlib_Wakeup(void)
{
//    static uint8_t data = 0;
//    
//    data = 0xAB;//����   
//    Fontlib_CsSelect();//ѡ�� low

//    Fontlib_SpiWrite(&data, 1);

//    Fontlib_CsNoSelect(); //CS���� high
//    nrf_delay_us(30);
}

/****************************************************************
** Function name:		    Fontlib_Init	
** Descriptions:            �ֿ��ʼ��
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ             
** Created Date:            2016.12.28          
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Fontlib_Init(void)
{
//    Font_CsInit();
//    Fontlib_Sleep();
}

/****************************************************************
** Function name:           Fontlib_Test
** Descriptions:            The test case 
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ      
** Created Date:            2016.11.14   
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Fontlib_Test(void)
{
    static uint8_t p[32]={0};
    static uint8_t getTData[32]={0};
    
    p[0] = 'A';//0x41;
    Fontlib_Wakeup();
    Fontlib_AsciiLattice(getTData, ASCII_8X16,p[0]);
    
    Fontlib_Sleep();   
}

/****************************************************************
** Function name:           Font_Utf8ToGbk
** Descriptions:            Utf8 to GBK
** input parameters:        utf8��ԭʼ����
** output parameters:       ��
** Returned value:          ����Gbk
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
** input parameters:        utf8��ԭʼ����
** output parameters:       ��
** Returned value:          ����Unicode
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
** Descriptions:            ��ȡ�������ݣ�ASIIC�� 
** input parameters:        p_data��ԭʼ���ݣ�
** output parameters:       p_lattice����������
** Returned value:          ��
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
** Descriptions:            ��ȡ�������ݣ����ģ�
** input parameters:        p_data��ԭʼ���ݣ�
** output parameters:       p_lattice����������
** Returned value:          ��
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
** Descriptions:            ��ȡ�������ݣ����
** input parameters:        p_data��ԭʼ���ݣ�
** output parameters:       p_lattice����������
** Returned value:          ��
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
** Descriptions:            ��ȡ�������ݣ����ģ�
** input parameters:        p_data��ԭʼ���ݣ�
** output parameters:       p_lattice����������
** Returned value:          ��
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
** Descriptions:            ��ȡ�������ݣ��������ģ�
** input parameters:        p_data��ԭʼ���ݣ�
** output parameters:       p_lattice����������
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2016-09-16
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Fontlib_CyrillicLattice(uint8_t *p_data, uint8_t *p_lattice)
{
    CYRILLIC_GetData(Fontlib_Utf8ToUnicode(p_data), p_lattice);///������ģ����
}

/****************************************************************
** Function name:           Fontlib_LattinLattice
** Descriptions:            ��ȡ�������ݣ������ģ�
** input parameters:        p_data��ԭʼ���ݣ�
** output parameters:       p_lattice����������
** Returned value:          ��
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

