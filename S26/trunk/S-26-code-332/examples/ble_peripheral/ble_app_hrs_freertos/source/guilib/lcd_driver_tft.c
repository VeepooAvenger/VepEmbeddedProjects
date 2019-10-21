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
#include "ext_flash.h"
#include "bsp_spi.h"

#define UI_START_ADDR     0x000000

#define DRIVER_HANCAI       1

/*********************************************************************
* Macro Definition
*/

#define LCD_DC_DATA                 PIN_HIGH(LCD_DC_PIN)
#define LCD_DC_CMD                  PIN_LOW(LCD_DC_PIN)
#define LCD_EN_ON                   PIN_HIGH(LCD_EN_PIN)
#define LCD_EN_OFF                  PIN_LOW(LCD_EN_PIN)
#define LCD_RES_HIGH                PIN_HIGH(LCD_RES_PIN)
#define LCD_RES_LOW                 PIN_LOW(LCD_RES_PIN)

#define LCD_SPI_MAX_DATA            254     //SPIһ�η������ݵ�������,��ƽ̨��أ�52832��
/*********************************************************************
* Global Variables
*/
static uint8_t lcd_buf[LCD_MAX_COLUMN*2] = {0x00, };
static uint8_t lcd_wakeup_flag = 0;

/*********************************************************************
* local functions
*/
/*
static void LCD_DisWhite(void);
static void LCD_DisRed(void);
static void LCD_DisGreen(void);
static void LCD_DisBlue(void);
static void LCD_DisYellow(void);
static void LCD_DisCyan(void);
static void LCD_DisPurple(void);
*/
static void LCD_DisBlack(void);

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
uint8_t LCD_DriverWriteSPI(const uint8_t *data, const uint32_t data_len)
{
    return Bps_Spi2Write(data, data_len);
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
    Bps_Spi2SelectLcd(1);
    LCD_DriverWriteSPI(&data, 1);
    Bps_Spi2SelectLcd(0);
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
    Bps_Spi2SelectLcd(1);
    ret = LCD_DriverWriteSPI(&data, 1); 
    Bps_Spi2SelectLcd(0);
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
    Bps_Spi2SelectLcd(1);  
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
    Bps_Spi2SelectLcd(0); 
    return ret;	
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
    __ALIGN(4) uint8_t display_buf[256] = {0x00,};
    uint32_t send_len = 0;
    uint32_t send_index = 0;
    uint32_t ui_address = 0;
    
    ui_address = address + UI_START_ADDR;
   
    while(send_index < data_len)
    {
        send_len = data_len - send_index;
        if(send_len > LCD_SPI_MAX_DATA)
        {   //note: 52832 SPIһ�������255���ֽ�
            send_len = LCD_SPI_MAX_DATA;
        }       
        Extflash_Read(ui_address + send_index, send_len, display_buf);       
        LCD_DriverWriteDataBuf(display_buf, send_len);
        send_index += send_len;
    }    
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
uint8_t LCD_DriverWriteDataBufBackground(const uint8_t *data, const uint32_t data_len, uint16_t color)
{
    uint8_t ret = 1;
    uint16_t send_num = 0;
    uint16_t send_len = 0;
    uint16_t i = 0;
    uint16_t j = 0;
    
    send_num = data_len / (LCD_MAX_COLUMN*2) + (data_len % (LCD_MAX_COLUMN * 2) == 0 ? 0:1);
    
    LCD_DC_DATA; 
    Bps_Spi2SelectLcd(1);
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
    Bps_Spi2SelectLcd(0); 
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
void LCD_DriverWriteDataFromExtFlashAndBackground(const uint32_t address, const uint32_t data_len, uint16_t color)
{
    __ALIGN(4) uint8_t display_buf[256] = {0x00,};
    uint8_t background_color_l = 0;
    uint8_t background_color_h = 0;
    uint16_t send_len = 0;
    uint16_t send_index = 0;
    uint16_t j = 0;
    uint16_t display_color = 0;
    uint32_t ui_address = 0;
    
    ui_address = address + UI_START_ADDR;
    
    background_color_l = color & 0x00ff;
    background_color_h = (color & 0xff00) >> 8;
    while(send_index < data_len)
    {
        send_len = data_len - send_index;
        if(send_len > 252)
        {   //note: 52832 SPIһ�������255���ֽ�
            send_len = 252;
        }       
        Extflash_Read(ui_address + send_index, send_len, display_buf);
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
void LCD_DriverWriteDataFromExtFlashAndChangeBackground(const uint32_t address, const uint32_t data_len, 
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
        Extflash_Read(UI_START_ADDR + address + send_index, send_len, display_buf);
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
void LCD_DriverReadFromExtFlash(const uint32_t address, uint8_t *data, const uint32_t data_len)
{
    uint8_t display_buf[255] = {0x00,};
    uint16_t read_len = 0;
    uint16_t read_index = 0;
    
    uint32_t address_tmp = 0;
    
    address_tmp = address + UI_START_ADDR;
    
    while(read_index < data_len)
    {
        read_len = data_len - read_index;
        if(read_len > LCD_SPI_MAX_DATA)
        {   //note: 52832 SPIһ�������255���ֽ�
            read_len = LCD_SPI_MAX_DATA;
        }       
        Extflash_Read(address_tmp + read_index, read_len, display_buf);       
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
    LCD_RES_LOW;    
    LCD_DriverDelay(10);          //Delay 10ms   
    LCD_RES_HIGH;                     
    LCD_DriverDelay(10);          //Delay 10ms   
    
    LCD_DriverWriteCmd(0x11);     //Sleep out
    LCD_DriverDelay(120);          //Delay 120ms
    lcd_wakeup_flag = 1;

#if !(DRIVER_HANCAI)
    LCD_DriverWriteCmd(0xB1);     
    LCD_DriverWriteDataByte(0x01);   
    LCD_DriverWriteDataByte(0x08);   
    LCD_DriverWriteDataByte(0x05);   

    LCD_DriverWriteCmd(0xB2);     
    LCD_DriverWriteDataByte(0x01);   
    LCD_DriverWriteDataByte(0x08);   
    LCD_DriverWriteDataByte(0x05);   

    LCD_DriverWriteCmd(0xB3);     
    LCD_DriverWriteDataByte(0x01);   
    LCD_DriverWriteDataByte(0x08);   
    LCD_DriverWriteDataByte(0x05);   
    LCD_DriverWriteDataByte(0x05);   
    LCD_DriverWriteDataByte(0x08);   
    LCD_DriverWriteDataByte(0x05);   

    LCD_DriverWriteCmd(0xB4);     //Dot inversion
    LCD_DriverWriteDataByte(0x00);   

    LCD_DriverWriteCmd(0xC0);     
    LCD_DriverWriteDataByte(0x28);   
    LCD_DriverWriteDataByte(0x08);   
    LCD_DriverWriteDataByte(0x04);   

    LCD_DriverWriteCmd(0xC1);     
    LCD_DriverWriteDataByte(0xC0);   

    LCD_DriverWriteCmd(0xC2);     
    LCD_DriverWriteDataByte(0x0D);   
    LCD_DriverWriteDataByte(0x00);   

    LCD_DriverWriteCmd(0xC3);     
    LCD_DriverWriteDataByte(0x8D);   
    LCD_DriverWriteDataByte(0x2A);   

    LCD_DriverWriteCmd(0xC4);     
    LCD_DriverWriteDataByte(0x8D);   
    LCD_DriverWriteDataByte(0xEE);   

    LCD_DriverWriteCmd(0xC5);     //VCOM
    LCD_DriverWriteDataByte(0x06);     //1D  

    LCD_DriverWriteCmd(0x36);     //MX, MY, RGB mode
    LCD_DriverWriteDataByte(0xC8);     //0xc8:��Ļ��ʾ��ˮƽ�ҵ��󣬴�ֱ�µ��ϣ�,BGR
    //LCD_DriverWriteDataByte(0x08);     //0x08:��Ļ��ʾ(ˮƽ���ң���ֱ�ϵ���),BGR

    LCD_DriverWriteCmd(0xE0);     
    LCD_DriverWriteDataByte(0x07);   
    LCD_DriverWriteDataByte(0x17);   
    LCD_DriverWriteDataByte(0x0C);   
    LCD_DriverWriteDataByte(0x15);   
    LCD_DriverWriteDataByte(0x2E);   
    LCD_DriverWriteDataByte(0x2A);   
    LCD_DriverWriteDataByte(0x23);   
    LCD_DriverWriteDataByte(0x28);   
    LCD_DriverWriteDataByte(0x28);   
    LCD_DriverWriteDataByte(0x28);   
    LCD_DriverWriteDataByte(0x2E);   
    LCD_DriverWriteDataByte(0x39);   
    LCD_DriverWriteDataByte(0x00);   
    LCD_DriverWriteDataByte(0x03);   
    LCD_DriverWriteDataByte(0x02);   
    LCD_DriverWriteDataByte(0x10);   

    LCD_DriverWriteCmd(0xE1);     
    LCD_DriverWriteDataByte(0x06);   
    LCD_DriverWriteDataByte(0x21);   
    LCD_DriverWriteDataByte(0x0D);   
    LCD_DriverWriteDataByte(0x17);   
    LCD_DriverWriteDataByte(0x35);   
    LCD_DriverWriteDataByte(0x30);   
    LCD_DriverWriteDataByte(0x2A);   
    LCD_DriverWriteDataByte(0x2D);   
    LCD_DriverWriteDataByte(0x2C);   
    LCD_DriverWriteDataByte(0x29);   
    LCD_DriverWriteDataByte(0x31);   
    LCD_DriverWriteDataByte(0x3B);   
    LCD_DriverWriteDataByte(0x00);   
    LCD_DriverWriteDataByte(0x02);   
    LCD_DriverWriteDataByte(0x03);   
    LCD_DriverWriteDataByte(0x12);   
    
    LCD_DriverWriteCmd(0x3A);     //65k mode RGB 565
    LCD_DriverWriteDataByte(0x05);  
#else
    LCD_DriverWriteCmd(0x21);   
    
    LCD_DriverWriteCmd(0xB1);     
    LCD_DriverWriteDataByte(0x05);   
    LCD_DriverWriteDataByte(0x3A);   
    LCD_DriverWriteDataByte(0x3A);   

    LCD_DriverWriteCmd(0xB2);     
    LCD_DriverWriteDataByte(0x05);   
    LCD_DriverWriteDataByte(0x3A);   
    LCD_DriverWriteDataByte(0x3A);   

    LCD_DriverWriteCmd(0xB3);     
    LCD_DriverWriteDataByte(0x05);   
    LCD_DriverWriteDataByte(0x3A);   
    LCD_DriverWriteDataByte(0x3A);   
    LCD_DriverWriteDataByte(0x05);   
    LCD_DriverWriteDataByte(0x3A);   
    LCD_DriverWriteDataByte(0x3A);   

    LCD_DriverWriteCmd(0xB4);     //Dot inversion
    LCD_DriverWriteDataByte(0x03);   

    LCD_DriverWriteCmd(0xC0);     
    LCD_DriverWriteDataByte(0x62);   
    LCD_DriverWriteDataByte(0x02);   
    LCD_DriverWriteDataByte(0x04);   

    LCD_DriverWriteCmd(0xC1);     
    LCD_DriverWriteDataByte(0xC0);   

    LCD_DriverWriteCmd(0xC2);     
    LCD_DriverWriteDataByte(0x0D);   
    LCD_DriverWriteDataByte(0x00);   

    LCD_DriverWriteCmd(0xC3);     
    LCD_DriverWriteDataByte(0x8D);   
    LCD_DriverWriteDataByte(0x6A);   

    LCD_DriverWriteCmd(0xC4);     
    LCD_DriverWriteDataByte(0x8D);   
    LCD_DriverWriteDataByte(0xEE);   

    LCD_DriverWriteCmd(0xC5);     //VCOM
    LCD_DriverWriteDataByte(0x0E);     //1D  

    LCD_DriverWriteCmd(0x36);     //MX, MY, RGB mode
    LCD_DriverWriteDataByte(0xC8);     //0xc8:��Ļ��ʾ��ˮƽ�ҵ��󣬴�ֱ�µ��ϣ�,BGR
    //LCD_DriverWriteDataByte(0x08);     //0x08:��Ļ��ʾ(ˮƽ���ң���ֱ�ϵ���),BGR

    LCD_DriverWriteCmd(0xE0);     
    LCD_DriverWriteDataByte(0x10);   
    LCD_DriverWriteDataByte(0x0E);   
    LCD_DriverWriteDataByte(0x02);   
    LCD_DriverWriteDataByte(0x03);   
    LCD_DriverWriteDataByte(0x0E);   
    LCD_DriverWriteDataByte(0x07);   
    LCD_DriverWriteDataByte(0x02);   
    LCD_DriverWriteDataByte(0x07);   
    LCD_DriverWriteDataByte(0x0A);   
    LCD_DriverWriteDataByte(0x12);   
    LCD_DriverWriteDataByte(0x27);   
    LCD_DriverWriteDataByte(0x37);   
    LCD_DriverWriteDataByte(0x00);   
    LCD_DriverWriteDataByte(0x0D);   
    LCD_DriverWriteDataByte(0x0E);   
    LCD_DriverWriteDataByte(0x10);   

    LCD_DriverWriteCmd(0xE1);     
    LCD_DriverWriteDataByte(0x10);   
    LCD_DriverWriteDataByte(0x0E);   
    LCD_DriverWriteDataByte(0x03);   
    LCD_DriverWriteDataByte(0x03);   
    LCD_DriverWriteDataByte(0x0F);   
    LCD_DriverWriteDataByte(0x06);   
    LCD_DriverWriteDataByte(0x02);   
    LCD_DriverWriteDataByte(0x08);   
    LCD_DriverWriteDataByte(0x0A);   
    LCD_DriverWriteDataByte(0x13);   
    LCD_DriverWriteDataByte(0x26);   
    LCD_DriverWriteDataByte(0x36);   
    LCD_DriverWriteDataByte(0x00);   
    LCD_DriverWriteDataByte(0x0D);   
    LCD_DriverWriteDataByte(0x0E);   
    LCD_DriverWriteDataByte(0x10);   
    
    LCD_DriverWriteCmd(0x3A);     //65k mode RGB 565
    LCD_DriverWriteDataByte(0x05);     
#endif

    LCD_DisBlack();
//    LCD_DriverWriteCmd(0x29);     //Display on    
//    LCD_OpenScreen();
//    LCD_DisWhite();
//    LCD_DisGreen();
//    LCD_DisBlue();
//    LCD_DriverDelay(5000);
//    LCD_DisYellow();
//    LCD_DisCyan();
//    LCD_DisPurple();

//    LCD_EN_ON;    
    
    LCD_CloseScreen();
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
#if !(DRIVER_HANCAI)    
    LCD_DriverWriteCmd(0x2A); //Set Column Address
    LCD_DriverWriteDataByte(0x00);
    LCD_DriverWriteDataByte(0x18);
    LCD_DriverWriteDataByte(0x00);
    LCD_DriverWriteDataByte(0x67);  

    LCD_DriverWriteCmd(0x2B); //Set Page Address
    LCD_DriverWriteDataByte(0x00);
    LCD_DriverWriteDataByte(0x00);
    LCD_DriverWriteDataByte(0x00);
    LCD_DriverWriteDataByte(0x9F);//9F

    LCD_DriverWriteCmd(0x2C);//Memory Write
#else
    LCD_DriverWriteCmd(0x2A); //Set Column Address
    LCD_DriverWriteDataByte(0x00);
    LCD_DriverWriteDataByte(0x1A);
    LCD_DriverWriteDataByte(0x00);
    LCD_DriverWriteDataByte(0x69);  

    LCD_DriverWriteCmd(0x2B); //Set Page Address
    LCD_DriverWriteDataByte(0x00);
    LCD_DriverWriteDataByte(0x01);
    LCD_DriverWriteDataByte(0x00);
    LCD_DriverWriteDataByte(0xA0);//9F

    LCD_DriverWriteCmd(0x2C);//Memory Write
#endif    
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
#if !(DRIVER_HANCAI)    
    LCD_DriverWriteCmd(0x2A); //Set Column Address
    LCD_DriverWriteDataByte(0x00);
    LCD_DriverWriteDataByte(0x18 + s_column);
    LCD_DriverWriteDataByte(0x00);
    LCD_DriverWriteDataByte(0x18 + s_column + (column_size - 1));  

    LCD_DriverWriteCmd(0x2B); //Set Page Address
    LCD_DriverWriteDataByte(0x00);
    LCD_DriverWriteDataByte(s_page);
    LCD_DriverWriteDataByte(0x00);
    LCD_DriverWriteDataByte(s_page + (page_size - 1));//9F

    LCD_DriverWriteCmd(0x2C);//Memory Write
#else
    LCD_DriverWriteCmd(0x2A); //Set Column Address
    LCD_DriverWriteDataByte(0x00);
    LCD_DriverWriteDataByte(0x1A + s_column);
    LCD_DriverWriteDataByte(0x00);
    LCD_DriverWriteDataByte(0x1A + s_column + (column_size - 1));  

    LCD_DriverWriteCmd(0x2B); //Set Page Address
    LCD_DriverWriteDataByte(0x00);
    LCD_DriverWriteDataByte(1+s_page);
    LCD_DriverWriteDataByte(0x00);
    LCD_DriverWriteDataByte(1+s_page + (page_size - 1));//9F

    LCD_DriverWriteCmd(0x2C);//Memory Write
#endif
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
    LCD_EN_OFF;
//    LCD_OpenScreenAll();   
    LCD_DriverWriteCmd(0x28);     //Display off   
    
    LCD_DriverWriteCmd(0x10);     //Sleep in   
    lcd_wakeup_flag = 0;
    LCD_DriverDelay(120);          //Delay 120ms
}

/****************************************************************
** Function name:           LCD_CloseLed
** Descriptions:            �ص�����
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
    LCD_EN_OFF;
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
    LCD_EN_ON;
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
}

/*******************************************����************************************************/

/*//��ɫ	
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

//��ɫ
static void LCD_DisBlue(void)	
{
    uint8_t j,i;
    LCD_SetPositionInit();
        
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
*/
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


