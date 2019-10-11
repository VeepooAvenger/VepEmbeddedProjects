/***************************Copyright(c)***********************************
**                       Veepoo Tech
**                  COPYRIGHT (C) 2016-2019
**-----------File Info-----------------------------------------------------
** File        name:     lcd_driver_tft.h
** Last modified Date:   2017-07-25
** Last SYS_VERSION:     0.0.0
** Descriptions:            
**-------------------------------------------------------------------------
** Created  by:          WJ
** Created  date:        2017-07-25
** SYS_VERSION:          0.0.0
** Descriptions:         
**-------------------------------------------------------------------------
** Modified by:				
** Modified date:  			          
** SYS_VERSION:                 
** Descriptions:                        
***************************************************************************/
#include <stdint.h>
#include <string.h>
#include "nrf_delay.h"
#include "bsp_spi.h"
#include "board.h"
#include "lcd_driver_tft.h"
#include "tft_icon_const.h"

#include "ext_flash.h"
#include "backlight_driving.h"

#define PAGE_ADDRESS_1      0x00
#define PAGE_ADDRESS_2      28*1024



/*********************************************************************
* Macro Definition
*/

#define LCD_DC_DATA                 PIN_HIGH(LCD_DC_PIN)
#define LCD_DC_CMD                  PIN_LOW(LCD_DC_PIN)
#define LCD_EN_ON                   //PIN_HIGH(LCD_EN_PIN)
#define LCD_EN_OFF                  //PIN_LOW(LCD_EN_PIN)
#define LCD_RES_HIGH                PIN_HIGH(LCD_RES_PIN)
#define LCD_RES_LOW                 PIN_LOW(LCD_RES_PIN)

#define LCD_SPI_MAX_DATA            SPI_MAX_DATA     //SPIһ�η������ݵ�������,��ƽ̨��أ�52832��
/*********************************************************************
* Global Variables
*/
static uint8_t lcd_buf[LCD_MAX_COLUMN*2] = {0x00, };
static uint8_t lcd_wakeup_flag = 0;

/*********************************************************************
* local functions
*/
static void LCD_DisWhite(void);
static void LCD_DisRed(void);
static void LCD_DisGreen(void);
static void LCD_DisBlue(void);
static void LCD_DisYellow(void);
static void LCD_DisCyan(void);
static void LCD_DisPurple(void);
static void LCD_DisBlack(void);
static void LCD_DisBlue1(void);

/****************************************************************
** Function name:           LCD_DriverDelay
** Descriptions:            ��ʱ��������λ����
** input parameters:        ms:��ʱ���ٺ���
** output parameters:
** Returned value:          
** Created by:              WJ              
** Created Date:            2017-07-25
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void LCD_DriverDelay(const uint32_t ms)
{
    nrf_delay_ms(ms);
}
void  LCD_DriverDeselect(void)
{
	LCD_SPI_CS_HIGH;
}
/****************************************************************
** Function name:           LCD_DriverWriteSPI
** Descriptions:            ʹ��SPI��������
** input parameters:        data:���ݵ�ַ,data_len:���ݳ���
** output parameters:
** Returned value:          
** Created by:              WJ              
** Created Date:            2017-07-25
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
uint8_t LCD_DriverWriteSPI(const uint8_t *data, const uint16_t data_len)
{
    return Bps_Spi1Write(data, data_len);
}

/****************************************************************
** Function name:           LCD_DriverWriteCmd
** Descriptions:            һ�η���һ������
** input parameters:        data:���ݵ�ַ,data_len:���ݳ���
** output parameters:
** Returned value:          
** Created by:              WJ              
** Created Date:            2017-07-25
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void LCD_DriverWriteCmd(const uint8_t data)
{
    LCD_DC_CMD;
    LCD_SPI_CS_LOW;
    LCD_DriverWriteSPI(&data, 1);
    LCD_SPI_CS_HIGH;
}

/****************************************************************
** Function name:           LCD_DriverWriteDataByte
** Descriptions:            һ�η���һ������
** input parameters:        data:���ݵ�ַ,data_len:���ݳ���
** output parameters:
** Returned value:          ����0:����ʧ��,1:���ͳɹ�
** Created by:              WJ              
** Created Date:            2017-07-25
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
uint8_t LCD_DriverWriteDataByte(const uint8_t data)
{
    uint8_t ret = 0;
    LCD_DC_DATA;
    LCD_SPI_CS_LOW;
    ret = LCD_DriverWriteSPI(&data, 1); 
    LCD_SPI_CS_HIGH;
    return ret;
}

/****************************************************************
** Function name:           LCD_DriverWriteDataBuf
** Descriptions:            һ�η��Ͷ������
** input parameters:        data:���ݵ�ַ,data_len:���ݳ���
** output parameters:
** Returned value:          ����0:����ʧ��,1:���ͳɹ�
** Created by:              WJ              
** Created Date:            2017-07-25
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
uint8_t LCD_DriverWriteDataBuf(const uint8_t *data, const uint32_t data_len)
{
    uint8_t ret = 1;
    uint32_t send_len = 0;
    uint32_t send_index = 0;
    
    LCD_DC_DATA; 
    LCD_SPI_CS_LOW;  
    while(send_index < data_len)
    {
        send_len = data_len - send_index;
        if(send_len > LCD_SPI_MAX_DATA)
        {   //note: 52832 SPIһ�������255���ֽ�
            send_len = LCD_SPI_MAX_DATA;
        }       
        ret &= LCD_DriverWriteSPI(&data[send_index], send_len);       
        send_index += send_len;
    }      
    LCD_SPI_CS_HIGH; 
    return ret;	
}
/****************************************************************
** Function name:           LCD_DriverPrepareWriteData
** Descriptions:            ׼��д����
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              YX              
** Created Date:            2019-01-05
*****************************************************************/ 
void LCD_DriverPrepareWriteData(void)
{
    LCD_DC_DATA; 
    LCD_SPI_CS_LOW;  
}
/****************************************************************
** Function name:           LCD_DriverWriteDataFromExtFlash
** Descriptions:            ���ⲿflash��ȡUI���������ʾ
** input parameters:        address:flash���ݵ�ַ,data_len:���ݳ���
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-08-09
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void LCD_DriverWriteDataFromExtFlash(const uint32_t address, const uint32_t data_len)
{
	#if SPI_1_2_USE_DMA
    
    Spi12_Operation(address + FLASH_GT_SIZE, data_len);
    #else
    uint8_t display_buf[255] = {0x00,};
    uint32_t send_len = 0;
    uint32_t send_index = 0;
    uint32_t address_tmp = 0;
    
    //HYQ.20180623.�����ֿ��UI����һ��FLASH����˶�ȡUI���ݵ�ʱ����Ҫ�����ֿ�Ĵ�С
    address_tmp = address + FLASH_GT_SIZE;
    
    while(send_index < data_len)
    {
        send_len = data_len - send_index;
        if(send_len > LCD_SPI_MAX_DATA)
        {   //note: 52832 SPIһ�������255���ֽ�
            send_len = LCD_SPI_MAX_DATA;
        }       
        Extflash_Read(address_tmp + send_index, send_len, display_buf);       
        LCD_DriverWriteDataBuf(display_buf, send_len);
        send_index += send_len;
    }   
    #endif //SPI_1_2_USE_DMA    
}

/****************************************************************
** Function name:           LCD_DriverWriteDataBufBackground
** Descriptions:            д���ֵ�ʱ�����Ҫ�õ���������ֵı���ɫ��������ֵ�����Ϊ0x0000������ʾcolor��
** input parameters:        data:���ݵ�ַ
**                          data_len:���ݳ���
**                          color:���ݱ���ɫ
** output parameters:
** Returned value:          ����0:����ʧ��,1:���ͳɹ�
** Created by:              YX              
** Created Date:            2017-08-07
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
uint8_t LCD_DriverWriteDataBufBackground(const uint8_t *data, const uint16_t data_len, uint16_t color)
{
    uint8_t ret = 1;
    uint16_t send_num = 0;
    uint16_t send_len = 0;
    uint16_t i = 0;
    uint16_t j = 0;
    
    send_num = data_len / (LCD_MAX_COLUMN*2) + (data_len % (LCD_MAX_COLUMN * 2) == 0 ? 0:1);
    
    LCD_DC_DATA; 
    LCD_SPI_CS_LOW;
    for(i = 0; i < send_num; i++)
    {
        if(i >= send_num - 1)
        {
            send_len = data_len % (LCD_MAX_COLUMN * 2);
        }
        else
        {
            send_len = (LCD_MAX_COLUMN * 2);
        }
        for(j = 0; j < send_len/2; j++)
        {
            if(*(uint16_t*)data == 0x0000)
            {
                lcd_buf[j * 2] = color & 0x00ff;
                lcd_buf[j * 2 + 1] = (color & 0xff00) >> 8;
            }
            else
            {
                lcd_buf[j * 2] = (*(uint16_t*)data) & 0x00ff;
                lcd_buf[j * 2 + 1] = (*(uint16_t*)data & 0xff00) >> 8;
            }
            data += 2;
        }       
        ret &= LCD_DriverWriteSPI(lcd_buf, send_len);        
    }
    LCD_SPI_CS_HIGH; 
    return ret;	
}

/****************************************************************
** Function name:           LCD_DriverWriteDataFromExtFlashAndBackground
** Descriptions:            ���ⲿflash��ȡUI���������ʾ������䱳����ɫ
** input parameters:        address:flash���ݵ�ַ,data_len:���ݳ��ȣ�color��������ɫ
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-08-09
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void LCD_DriverWriteDataFromExtFlashAndBackground(const uint32_t address, const uint16_t data_len, uint16_t color)
{
    uint8_t display_buf[255] = {0x00,};
    uint8_t background_color_l = 0;
    uint8_t background_color_h = 0;
    uint16_t send_len = 0;
    uint16_t send_index = 0;
    uint16_t j = 0;
    uint16_t display_color = 0;
    
    background_color_l = color & 0x00ff;
    background_color_h = (color & 0xff00) >> 8;
    while(send_index < data_len)
    {
        send_len = data_len - send_index;
        if(send_len > 252)
        {   //note: 52832 SPIһ�������255���ֽ�
            send_len = 252;
        }       
        Extflash_Read(address + send_index, send_len, display_buf);
        for(j = 0; j < send_len/2; j++)
        {
            display_color = *((uint16_t *)&display_buf[j * 2]);
            if(display_color == 0x0000)
            {
                display_buf[j * 2] = background_color_l;
                display_buf[j * 2 + 1] = background_color_h;
            }
        }         
        LCD_DriverWriteDataBuf(display_buf, send_len);
        send_index += send_len;
    }      
}


/****************************************************************
** Function name:           LCD_DriverWriteDataFromExtFlashAndChangeBackground
** Descriptions:            ���ⲿflash��ȡUI���������ʾ������䱳����ɫ,������ɫ
** input parameters:        address:flash���ݵ�ַ,data_len:���ݳ��ȣ�back_groundcolor��������ɫ
                            main_color����ɫ
                            sub_color��������ɫ
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-08-09
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void LCD_DriverWriteDataFromExtFlashAndChangeBackground(const uint32_t address, const uint16_t data_len, 
                                                        uint16_t back_groundcolor, uint16_t main_color, uint16_t sub_color)
{
    uint8_t display_buf[255] = {0x00,};
    uint8_t background_color_l = 0;
    uint8_t background_color_h = 0;
    uint8_t mian_color_l = 0;
    uint8_t mian_color_h = 0;
    uint8_t sub_color_l = 0;
    uint8_t sub_color_h = 0;
    uint16_t send_len = 0;
    uint16_t send_index = 0;
    uint16_t j = 0;
    uint16_t display_color = 0;
    
    background_color_l = back_groundcolor & 0x00ff;
    background_color_h = (back_groundcolor & 0xff00) >> 8;
    
    mian_color_l = main_color & 0x00ff;
    mian_color_h = (main_color & 0xff00) >> 8;
    
    sub_color_l = sub_color & 0x00ff;
    sub_color_h = (sub_color & 0xff00) >> 8;
    while(send_index < data_len)
    {
        send_len = data_len - send_index;
        if(send_len > 252)
        {   //note: 52832 SPIһ�������255���ֽ�
            send_len = 252;
        }       
        Extflash_Read(address + send_index, send_len, display_buf);
        for(j = 0; j < send_len/2; j++)
        {
            display_color = (uint16_t)(display_buf[j * 2] << 8) + display_buf[j * 2 + 1];//*((uint16_t *)&display_buf[j * 2]);
            if(display_color == 0xFFFF)
            {               
                display_buf[j * 2 ] =  mian_color_l;
                display_buf[j * 2 + 1] = mian_color_h; 
            }
            else if(display_color >= 0x7DEF)
            {  
                display_buf[j * 2 ] = sub_color_l;  
                display_buf[j * 2 + 1] = sub_color_h; 
            }
            else 
            {      
                display_buf[j * 2 ] = background_color_l;   
                display_buf[j * 2 + 1] = background_color_h;  
            }
        }         
        LCD_DriverWriteDataBuf(display_buf, send_len);
        send_index += send_len;
    }      
}


/****************************************************************
** Function name:			LCD_DriverReadFromExtFlash
** Descriptions:            ���ⲿflash��ȡUI�����������
** input parameters:        address:flash���ݵ�ַ,data_len:���ݳ���
** output parameters:       data������
** Returned value:          ��
** Created by:				WJ              
** Created Date:           	2017-09-12
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void LCD_DriverReadFromExtFlash(const uint32_t address, uint8_t *data, const uint16_t data_len)
{
    uint8_t display_buf[255] = {0x00,};
    uint16_t read_len = 0;
    uint16_t read_index = 0;
    
    while(read_index < data_len)
    {
        read_len = data_len - read_index;
        if(read_len > LCD_SPI_MAX_DATA)
        {   //note: 52832 SPIһ�������255���ֽ�
            read_len = LCD_SPI_MAX_DATA;
        }       
        Extflash_Read(address + read_index, read_len, display_buf);       
        memcpy(data + read_index,  display_buf, read_len);
        read_index += read_len;
    }      
}

/****************************************************************
** Function name:			LCD_DriverInit
** Descriptions:            ����ʼ��
** input parameters:        
** output parameters:
** Returned value:          
** Created by:              WJ              
** Created Date:            2017-07-25
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void LCD_DriverInit(void)
{   
    //LCD_EN_OFF; 
    Backlight_SetLeve(0);
    
    LCD_RES_LOW;    
    LCD_DriverDelay(10);          //Delay 10ms   
    LCD_RES_HIGH;                     
    LCD_DriverDelay(10);          //Delay 10ms   
    
    LCD_DriverWriteCmd(0x11);     //Sleep out
    LCD_DriverDelay(120);          //Delay 120ms
    lcd_wakeup_flag = 1;

    
    LCD_DriverDelay(1000);          //Delay 120ms
    LCD_RES_LOW;    
    LCD_DriverDelay(10);          //Delay 10ms   
    LCD_RES_HIGH;                     
    LCD_DriverDelay(10);          //Delay 10ms   
    

	LCD_DriverWriteCmd(0x11);
	LCD_DriverDelay(600);
	LCD_DriverWriteCmd(0x36);
	LCD_DriverWriteDataByte(0x10); ////////////

	LCD_DriverWriteCmd(0x3a);
	LCD_DriverWriteDataByte(0x05);
	LCD_DriverWriteCmd(0x21);
	LCD_DriverWriteCmd(0xE7);
	LCD_DriverWriteDataByte(0x00);	 // 2 data;00-1data
	LCD_DriverWriteCmd(0x2a);
	LCD_DriverWriteDataByte(0x00);
	LCD_DriverWriteDataByte(0x00);
	LCD_DriverWriteDataByte(0x00);
	LCD_DriverWriteDataByte(0xef);
	LCD_DriverWriteCmd(0x2b);
	LCD_DriverWriteDataByte(0x00);
	LCD_DriverWriteDataByte(0x00);
	LCD_DriverWriteDataByte(0x00);
	LCD_DriverWriteDataByte(0xef);
	//--------------------------------ST7789V Frame rate setting----------------------------------//
	LCD_DriverWriteCmd(0xb2);
	LCD_DriverWriteDataByte(0x0c);
	LCD_DriverWriteDataByte(0x0c);
	LCD_DriverWriteDataByte(0x00);
	LCD_DriverWriteDataByte(0x33);
	LCD_DriverWriteDataByte(0x33);
	LCD_DriverWriteCmd(0xb7);
	LCD_DriverWriteDataByte(0x35);
	//---------------------------------ST7789V Power setting--------------------------------------//
	LCD_DriverWriteCmd(0xbb);
	LCD_DriverWriteDataByte(0x1f);
	LCD_DriverWriteCmd(0xc0);
	LCD_DriverWriteDataByte(0x2c);
	LCD_DriverWriteCmd(0xc2);
	LCD_DriverWriteDataByte(0x01);
	LCD_DriverWriteCmd(0xc3);
	LCD_DriverWriteDataByte(0x12);
	LCD_DriverWriteCmd(0xc4);
	LCD_DriverWriteDataByte(0x20);
	LCD_DriverWriteCmd(0xc6);
	LCD_DriverWriteDataByte(0x0f);
	LCD_DriverWriteCmd(0xd0);
	LCD_DriverWriteDataByte(0xa4);
	LCD_DriverWriteDataByte(0xa1);
	//--------------------------------ST7789V
	LCD_DriverWriteCmd(0xe0);
	LCD_DriverWriteDataByte(0xd0);
	LCD_DriverWriteDataByte(0x08);
	LCD_DriverWriteDataByte(0x11);
	LCD_DriverWriteDataByte(0x08);
	LCD_DriverWriteDataByte(0x0c);
	LCD_DriverWriteDataByte(0x15);
	LCD_DriverWriteDataByte(0x39);
	LCD_DriverWriteDataByte(0x33);
	LCD_DriverWriteDataByte(0x50);
	LCD_DriverWriteDataByte(0x36);
	LCD_DriverWriteDataByte(0x13);
	LCD_DriverWriteDataByte(0x14);
	LCD_DriverWriteDataByte(0x29);
	LCD_DriverWriteDataByte(0x2d);
	LCD_DriverWriteCmd(0xe1);
	LCD_DriverWriteDataByte(0xd0);
	LCD_DriverWriteDataByte(0x08);
	LCD_DriverWriteDataByte(0x10);
	LCD_DriverWriteDataByte(0x08);
	LCD_DriverWriteDataByte(0x06);
	LCD_DriverWriteDataByte(0x06);
	LCD_DriverWriteDataByte(0x39);
	LCD_DriverWriteDataByte(0x44);
	LCD_DriverWriteDataByte(0x51);
	LCD_DriverWriteDataByte(0x0b);
	LCD_DriverWriteDataByte(0x16);
	LCD_DriverWriteDataByte(0x14);
	LCD_DriverWriteDataByte(0x2f);
	LCD_DriverWriteDataByte(0x31);    
 
 
//    LCD_DisWhite();
//    LCD_DisRed();
//    LCD_DisYellow();
//    LCD_DisCyan();
//    LCD_DisGreen();
//    LCD_DisBlue();
//    LCD_DisPurple();
    LCD_DisBlack();
      
//	LCD_DriverDelay(120);
//	LCD_DriverWriteCmd(0x29);
//    
//    LCD_EN_ON; 
    Backlight_SetLeve(0);
}

/****************************************************************
** Function name:           LCD_SetPositionInit
** Descriptions:            ����ʾ�����ʼ��(ȫ��)
** input parameters:        
** output parameters:
** Returned value:          
** Created by:              WJ              
** Created Date:            2017-07-25
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void LCD_SetPositionInit(void)
{ 
    LCD_DriverWriteCmd(0x2A); //Set Column Address
    LCD_DriverWriteDataByte(0x00);
    LCD_DriverWriteDataByte(0);
    LCD_DriverWriteDataByte(0x00);
    LCD_DriverWriteDataByte(LCD_MAX_COLUMN - 1); 
    
    LCD_DriverWriteCmd(0x2B); //Set Page Address
    LCD_DriverWriteDataByte(0x00);
    LCD_DriverWriteDataByte(0);
    
    LCD_DriverWriteDataByte(0x00);
    LCD_DriverWriteDataByte(LCD_MAX_ROW - 1);//9F  
  
    LCD_DriverWriteCmd(0x2C);//Memory Write   
}

/****************************************************************
** Function name:           LCD_SetPositionInit
** Descriptions:            ����ʾ�����ʼ��(�ֲ�)
** input parameters:        
** output parameters:       s_column:��ʼ��,s_page:��ʼҳ,column_size:������,page_size:����ҳ
** Returned value:          
** Created by:              WJ              
** Created Date:            2017-07-25
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void LCD_SetPosition(uint8_t s_column, uint8_t s_page, uint8_t column_size, uint8_t page_size)
{ 

    LCD_DriverWriteCmd(0x2A); //Set Column Address
    LCD_DriverWriteDataByte(0x00);
    //LCD_DriverWriteDataByte(0x18 + s_column);
    LCD_DriverWriteDataByte(s_column);
    LCD_DriverWriteDataByte(0x00);
    //LCD_DriverWriteDataByte(0x18 + s_column + (column_size - 1));  
    LCD_DriverWriteDataByte(s_column + (column_size - 1)); 
    
    LCD_DriverWriteCmd(0x2B); //Set Page Address
    LCD_DriverWriteDataByte(0x00);
    LCD_DriverWriteDataByte(s_page);
    LCD_DriverWriteDataByte(0x00);
    LCD_DriverWriteDataByte(s_page + (page_size - 1));//9F  
  
    LCD_DriverWriteCmd(0x2C);//Memory Write   

}

/****************************************************************
** Function name:           LCD_OpenScreenAll
** Descriptions:            �����Ļ,��ɫ����
** input parameters:        
** output parameters:       
** Returned value:          
** Created by:              WJ              
** Created Date:            2017-07-27
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void LCD_OpenScreenAll(void)
{
    LCD_DisBlack();
}

void LCD_OpenScreenByPosition(uint8_t s_column, uint8_t s_page, uint8_t column_size, uint8_t page_size)
{   
    uint8_t i = 0;
    
    //WJ.20180427.��ֹԽ�絼�²�֪�����쳣
    if(column_size > LCD_MAX_COLUMN)
    {
        return;
    }
    
    LCD_SetPosition(s_column, s_page, column_size, page_size);         
    for(i = 0; i < page_size; i++)									     
    {
        memset(lcd_buf, 0x00, column_size * 2);
        LCD_DriverWriteDataBuf(lcd_buf, column_size * 2);
    }
}

/****************************************************************
** Function name:           LCD_SetColorScreenByPosition
** Descriptions:            ����ĳ���������ɫ����һ��
** input parameters:        s_column����������
**                          s_page����������
**                          column_size�������С
**                          page_size�������С
**                          color����ɫ����
** output parameters:       
** Returned value:          
** Created by:              YX              
** Created Date:            2017-08-07
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void LCD_SetColorScreenByPosition(uint8_t s_column, uint8_t s_page, uint8_t column_size, uint8_t page_size, uint16_t color)
{   
    uint8_t i = 0;
    //WJ.20180427.��ֹԽ�絼�²�֪�����쳣
    if(column_size > LCD_MAX_COLUMN)
    {
        return;
    }   
    LCD_SetPosition(s_column, s_page, column_size, page_size);         
    for(i = 0; i < page_size; i++)									     
    {
        for(uint16_t j = 0; j < column_size; j++)
        {
            lcd_buf[j*2] = color&0x00ff;
            lcd_buf[j*2+1] = (color&0xff00)>>8;
        }
        LCD_DriverWriteDataBuf(lcd_buf, column_size * 2);
    }
}

/****************************************************************
** Function name:           LCD_CloseScreen
** Descriptions:            ����,�������,�رձ�����,��������ģʽ
** input parameters:        
** output parameters:       
** Returned value:          
** Created by:              WJ              
** Created Date:            2017-07-27
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void LCD_CloseScreen(void)
{
    //LCD_EN_OFF;
    Backlight_SetLeve(0);
//    LCD_OpenScreenAll();   
    LCD_DriverWriteCmd(0x28);     //Display off   
    
    LCD_DriverWriteCmd(0x10);     //Sleep in  
    lcd_wakeup_flag = 0;
    LCD_DriverDelay(120);          //Delay 120ms 
}

/****************************************************************
** Function name:           LCD_OpenScreen
** Descriptions:            �����˳�����ģʽ
** input parameters:        
** output parameters:       
** Returned value:          
** Created by:              WJ              
** Created Date:            2017-07-27
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void LCD_OpenScreen(void)
{  
    if(lcd_wakeup_flag == 0)
    {       
        LCD_DriverWriteCmd(0x11);     //Sleep out  
        LCD_DriverDelay(120);          //Delay 120ms
        lcd_wakeup_flag = 1;
    }
    LCD_DriverWriteCmd(0x29);     //Display on 
//    LCD_EN_ON;
}

/****************************************************************
** Function name:           LCD_CloseLed
** Descriptions:            ����,�رձ�����
** input parameters:        
** output parameters:       
** Returned value:          
** Created by:              WJ              
** Created Date:            2017-07-27
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void LCD_CloseLed(void)
{
    //LCD_EN_OFF;
    Backlight_SetLeve(0);
}

/****************************************************************
** Function name:           LCD_OpenLed
** Descriptions:            ����,�رձ�����
** input parameters:        
** output parameters:       
** Returned value:          
** Created by:              WJ              
** Created Date:            2017-07-27
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void LCD_OpenLed(void)
{
//    LCD_EN_ON;
}

/****************************************************************
** Function name:           LCD_WakeUp
** Descriptions:            LCD���ѣ��ܼӿ�LCD�������ݴ���
** input parameters:        
** output parameters:       
** Returned value:          
** Created by:              WJ              
** Created Date:            2017-07-27
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void LCD_WakeUp(void)
{    
    if(lcd_wakeup_flag == 0)
    {       
        LCD_DriverWriteCmd(0x11);     //Sleep out  
        LCD_DriverDelay(120);          //Delay 120ms
        lcd_wakeup_flag = 1;
    }
//    LCD_EN_ON;
}

/*******************************************����************************************************/
//��ɫ	
static void LCD_DisWhite(void)		 
{
    uint8_t j,i;
    LCD_SetPositionInit();
    for(i = 0; i < LCD_MAX_ROW; i++)																								      
    {
        for(j = 0; j < LCD_MAX_COLUMN; j++)
        {
            lcd_buf[j*2 ] = 0xff;
            lcd_buf[j*2 + 1] = 0xff;
        }
        LCD_DriverWriteDataBuf(lcd_buf, LCD_MAX_COLUMN*2);
    }
}	

//��ɫ
static void LCD_DisRed(void)	
{
    uint8_t j,i;
    LCD_SetPositionInit();
    for(i = 0; i < LCD_MAX_ROW; i++)	
    {    
        for(j = 0; j < LCD_MAX_COLUMN; j++)
        {
            lcd_buf[j*2 ] = 0xf8;
            lcd_buf[j*2 + 1] = 0x00;
        }
        LCD_DriverWriteDataBuf(lcd_buf, LCD_MAX_COLUMN*2);
    }
}	

//��ɫ
static void LCD_DisYellow(void)		 
{
    uint8_t j,i;
    LCD_SetPositionInit();
    for(i = 0; i < LCD_MAX_ROW; i++)		
    {    
        for(j = 0; j < LCD_MAX_COLUMN; j++)
        {
            lcd_buf[ j*2 ] = 0xFF;
            lcd_buf[ j*2 + 1] = 0xE0;
        }
        LCD_DriverWriteDataBuf(lcd_buf, LCD_MAX_COLUMN*2);
    }
}

//��ɫ
static void LCD_DisCyan(void)		 
{
    uint8_t j,i;
    LCD_SetPositionInit();
    for(i = 0; i < LCD_MAX_ROW; i++)		
    {    
        for(j = 0; j < LCD_MAX_COLUMN; j++)
        {
            lcd_buf[ j*2 ] = 0x07;
            lcd_buf[ j*2 + 1] = 0xFF;
        }
        LCD_DriverWriteDataBuf(lcd_buf, LCD_MAX_COLUMN*2);
    }
}

//��ɫ
static void LCD_DisPurple(void)		 
{
    uint8_t j,i;
    LCD_SetPositionInit();
    for(i = 0; i < LCD_MAX_ROW; i++)		
    {    
        for(j = 0; j < LCD_MAX_COLUMN; j++)
        {
            lcd_buf[ j*2 ] = 0xF8;
            lcd_buf[ j*2 + 1] = 0x1F;
        }
        LCD_DriverWriteDataBuf(lcd_buf, LCD_MAX_COLUMN*2);
    }
}

//��ɫ
static void LCD_DisGreen(void)
{
    uint8_t j,i;
    LCD_SetPositionInit();
    for(i = 0; i < LCD_MAX_ROW; i++)		
    {    
        for(j = 0; j < LCD_MAX_COLUMN; j++)
        {
            lcd_buf[ j*2 ] = 0x07;
            lcd_buf[ j*2 + 1] = 0xE0;
        }
        LCD_DriverWriteDataBuf(lcd_buf, LCD_MAX_COLUMN*2);
    }
}

////��ɫ
//static void LCD_DisBlue(void)	
//{
//    uint8_t j,i;
//   // LCD_SetPositionInit();
//        
//    LCD_SetPosition(29, 160, 182, 38);
//    for(i = 0; i < 38; i++)									     
//    {
//        for(j = 0; j < 182; j++)
//        {
//            lcd_buf[ j*2 ] = 0x00;
//            lcd_buf[ j*2 + 1] = 0x1f;
//        }
//        LCD_DriverWriteDataBuf(lcd_buf, 182*2);
//    }
//}

//��ɫ
static void LCD_DisBlue(void)	
{
    uint8_t j,i;
    LCD_SetPositionInit();
        
//    LCD_SetPosition(29, 160, 182, 38);
    for(i = 0; i < LCD_MAX_ROW; i++)									     
    {
        for(j = 0; j < LCD_MAX_COLUMN; j++)
        {
            lcd_buf[ j*2 ] = 0x00;
            lcd_buf[ j*2 + 1] = 0x1f;
        }
        LCD_DriverWriteDataBuf(lcd_buf, LCD_MAX_COLUMN*2);
    }
}

//��ɫ
static void LCD_DisBlack(void)	
{
    uint8_t j,i;
    LCD_SetPositionInit();
        
    for(i = 0; i < LCD_MAX_ROW; i++)									     
    {
        for(j = 0; j < LCD_MAX_COLUMN; j++)
        {
            lcd_buf[ j*2 ] = 0x00;
            lcd_buf[ j*2 + 1] = 0x00;
        }
        LCD_DriverWriteDataBuf(lcd_buf, LCD_MAX_COLUMN*2);
    }
}

//����ʹ��,ҳ���л�,��ʾЧ��
void LCD_ChangePageColorTest(void)
{
    static uint8_t cnt = 0;
    uint16_t i = 0;
    
    if(cnt <= 1 )
    {        
        LCD_SetPositionInit();        
        for(i = 0; i < 160; i++)
        {
            switch(cnt)
            {
            case 0:                                  
                Extflash_Read(PAGE_ADDRESS_1 + i*80*2, 80*2, lcd_buf);break;            
            default:            
                Extflash_Read(PAGE_ADDRESS_2 + i*80*2, 80*2, lcd_buf);break;          
            }
            LCD_DriverWriteDataBuf(lcd_buf, 80*2);      
        }
        cnt++;
        return;
    }
    
    switch(cnt)
    {    
        case 11:
            LCD_DisWhite();
            break;
        case 12:
            LCD_DisRed();
            break;
        case 13:
            LCD_DisGreen();
            break;
        case 14:
            LCD_DisBlue();
            break;
        case 15:
            LCD_DisYellow();
            break;
        case 16:
            LCD_DisCyan();
            break;
        case 17:
            LCD_DisPurple();
            break;
        case 18:
            LCD_DisBlack();
        default:
            cnt = 0;
            LCD_DisWhite();    
            break;
    }
}



//��ɫ
void LCD_DisBlue12Bit(void)	
{
    uint8_t j,i;
    
    //LCD_SetPositionInit();
      LCD_SetPosition(0,0,240,240);  
    for(i = 0; i < LCD_MAX_ROW; i++)									     
    {
        for(j = 0; j < LCD_MAX_COLUMN/2; j++)
        {
            lcd_buf[ j*3 ] = 0x00;
            lcd_buf[ j*3 + 1] = 0xF0;
            lcd_buf[ j*3 + 2] = 0x0F;
        }
        
        LCD_DriverWriteDataBuf(lcd_buf, LCD_MAX_COLUMN*3/2);
    }
}

//��ɫ
void LCD_DisRed12Bit(void)	
{
    uint8_t j,i;
    
    //LCD_SetPositionInit();
      LCD_SetPosition(0,0,240,240);  
    for(i = 0; i < LCD_MAX_ROW; i++)									     
    {
        for(j = 0; j < LCD_MAX_COLUMN/2; j++)
        {
            lcd_buf[ j*3 ] = 0xF0;
            lcd_buf[ j*3 + 1] = 0x0F;
            lcd_buf[ j*3 + 2] = 0x00;
        }
        
        LCD_DriverWriteDataBuf(lcd_buf, LCD_MAX_COLUMN*3/2);
    }
}
