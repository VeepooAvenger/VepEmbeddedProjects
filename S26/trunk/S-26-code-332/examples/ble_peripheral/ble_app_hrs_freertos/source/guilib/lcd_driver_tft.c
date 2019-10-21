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

#define LCD_SPI_MAX_DATA            254     //SPI一次发送数据的最大个数,与平台相关（52832）
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
** Descriptions:            延时函数，单位毫秒
** input parameters:        ms:延时多少毫秒
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
** Descriptions:            使用SPI发送数据
** input parameters:        data:数据地址,data_len:数据长度
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
** Descriptions:            一次发送一个命令
** input parameters:        data:数据地址,data_len:数据长度
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
** Descriptions:            一次发送一个数据
** input parameters:        data:数据地址,data_len:数据长度
** output parameters:
** Returned value:          返回0:发送失败,1:发送成功
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
** Descriptions:            一次发送多个数据
** input parameters:        data:数据地址,data_len:数据长度
** output parameters:
** Returned value:          返回0:发送失败,1:发送成功
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
        {   //note: 52832 SPI一次最大发送255个字节
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
** Descriptions:            从外部flash获取UI数据填充显示
** input parameters:        address:flash数据地址,data_len:数据长度
** output parameters:       无
** Returned value:          无
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
        {   //note: 52832 SPI一次最大发送255个字节
            send_len = LCD_SPI_MAX_DATA;
        }       
        Extflash_Read(ui_address + send_index, send_len, display_buf);       
        LCD_DriverWriteDataBuf(display_buf, send_len);
        send_index += send_len;
    }    
}

/****************************************************************
** Function name:           LCD_DriverWriteDataBufBackground
** Descriptions:            写数字的时候可能要用到，添加数字的背景色（如果数字的数据为0x0000，则显示color）
** input parameters:        data:数据地址
**                          data_len:数据长度
**                          color:数据背景色
** output parameters:
** Returned value:          返回0:发送失败,1:发送成功
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
** Descriptions:            从外部flash获取UI数据填充显示，并填充背景颜色
** input parameters:        address:flash数据地址,data_len:数据长度；color：背景颜色
** output parameters:       无
** Returned value:          无
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
        {   //note: 52832 SPI一次最大发送255个字节
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
** Descriptions:            从外部flash获取UI数据填充显示，并填充背景颜色,更换主色
** input parameters:        address:flash数据地址,data_len:数据长度；back_groundcolor：背景颜色
                            main_color：主色
                            sub_color：辅助颜色
** output parameters:       无
** Returned value:          无
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
        {   //note: 52832 SPI一次最大发送255个字节
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
** Descriptions:            从外部flash获取UI数据填充数组
** input parameters:        address:flash数据地址,data_len:数据长度
** output parameters:       data：数组
** Returned value:          无
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
        {   //note: 52832 SPI一次最大发送255个字节
            read_len = LCD_SPI_MAX_DATA;
        }       
        Extflash_Read(address_tmp + read_index, read_len, display_buf);       
        memcpy(data + read_index,  display_buf, read_len);
        read_index += read_len;
    }      
}

/****************************************************************
** Function name:			LCD_DriverInit
** Descriptions:            屏初始化
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
    LCD_DriverWriteDataByte(0xC8);     //0xc8:屏幕显示（水平右到左，垂直下到上）,BGR
    //LCD_DriverWriteDataByte(0x08);     //0x08:屏幕显示(水平左到右，垂直上到下),BGR

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
    LCD_DriverWriteDataByte(0xC8);     //0xc8:屏幕显示（水平右到左，垂直下到上）,BGR
    //LCD_DriverWriteDataByte(0x08);     //0x08:屏幕显示(水平左到右，垂直上到下),BGR

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
** Descriptions:            屏显示区域初始化(全屏)
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
** Descriptions:            屏显示区域初始化(局部)
** input parameters:        
** output parameters:       s_column:开始列,s_page:开始页,column_size:多少列,page_size:多少页
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
** Descriptions:            清除屏幕,黑色背景
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
    
    //WJ.20180427.防止越界导致不知道的异常
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
** Descriptions:            设置某块区域的颜色（单一）
** input parameters:        s_column：横向坐标
**                          s_page：纵向坐标
**                          column_size：横向大小
**                          page_size：纵向大小
**                          color：颜色数据
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
    //WJ.20180427.防止越界导致不知道的异常
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
** Descriptions:            关屏,清除内容,关闭背景灯,进入休眠模式
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
** Descriptions:            关掉背灯
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
** Descriptions:            开屏退出休眠模式
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
** Descriptions:            LCD唤醒，能加快LCD本身数据处理
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

/*******************************************测试************************************************/

/*//白色	
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

//红色
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

//黄色
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

//青色
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

//紫色
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

//绿色
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

//蓝色
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
//黑色
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


