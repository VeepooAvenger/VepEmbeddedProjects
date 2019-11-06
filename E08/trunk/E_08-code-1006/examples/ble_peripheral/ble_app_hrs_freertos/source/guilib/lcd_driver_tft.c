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

#define UI_START_ADDR           0x200000

#define UI_DISPLAY_180          1//0   //旋转180°


/*********************************************************************
* Macro Definition
*/

#define LCD_DC_DATA                 PIN_HIGH(LCD_DC_PIN)
#define LCD_DC_CMD                  PIN_LOW(LCD_DC_PIN)
#define LCD_EN_ON                   //PIN_HIGH(LCD_EN_PIN)
#define LCD_EN_OFF                  //PIN_LOW(LCD_EN_PIN)
#define LCD_RES_HIGH                PIN_HIGH(LCD_RES_PIN)
#define LCD_RES_LOW                 PIN_LOW(LCD_RES_PIN)

#define LCD_SPI_MAX_DATA            SPI_MAX_DATA     //SPI一次发送数据的最大个数,与平台相关（52832）
/*********************************************************************
* Global Variables
*/
static uint8_t lcd_buf[LCD_MAX_COLUMN*2] = {0x00, };
static uint8_t lcd_wakeup_flag = 0;

/*********************************************************************
* local functions
*/
//static void LCD_DisWhite(void);
//static void LCD_DisRed(void);
//static void LCD_DisGreen(void);
//static void LCD_DisBlue(void);
//static void LCD_DisYellow(void);
//static void LCD_DisCyan(void);
//static void LCD_DisPurple(void);
static void LCD_DisBlack(void);

/****************************************************************
** Function name:			LCD_DriverDelay
** Descriptions:            延时函数，单位毫秒
** input parameters:        ms:延时多少毫秒
** output parameters:
** Returned value:          
** Created by:				WJ              
** Created Date:           	2017-07-25
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
** Function name:			LCD_DriverWriteSPI
** Descriptions:            使用SPI发送数据
** input parameters:        data:数据地址,data_len:数据长度
** output parameters:
** Returned value:          
** Created by:				WJ              
** Created Date:           	2017-07-25
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
uint8_t LCD_DriverWriteSPI(const uint8_t *data, const uint32_t data_len)
{
    return Bps_Spi1Write(data, data_len);
}

/****************************************************************
** Function name:			LCD_DriverWriteCmd
** Descriptions:            一次发送一个命令
** input parameters:        data:数据地址,data_len:数据长度
** output parameters:
** Returned value:          
** Created by:				WJ              
** Created Date:           	2017-07-25
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
** Function name:			LCD_DriverWriteDataByte
** Descriptions:            一次发送一个数据
** input parameters:        data:数据地址,data_len:数据长度
** output parameters:
** Returned value:          返回0:发送失败,1:发送成功
** Created by:				WJ              
** Created Date:           	2017-07-25
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
    LCD_SPI_CS_LOW;  
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
    LCD_SPI_CS_HIGH; 
    return ret;	
}

/****************************************************************
** Function name:           LCD_DriverPrepareWriteData
** Descriptions:            准备写数据
** input parameters:        无
** output parameters:       无
** Returned value:          无
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
    #if SPI_1_2_USE_DMA
    
    Spi12_Operation(address + UI_START_ADDR, data_len);
    #else
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
    #endif //SPI_1_2_USE_DMA
}

//////void LCD_DriverWriteDataFromExtFlashType2(uint16_t index, const uint32_t address, const uint32_t data_len)
//////{
//////    if(index == 0 && write_data_s.flg == 1)
//////    {
//////        if(write_data_s.data_len > 0)
//////        {
//////            LCD_DriverWriteDataFromExtFlash(address + write_data_s.offset_len, write_data_s.data_len);
//////        }
//////    }
//////    else
//////    {
//////        //YX.20190304.解决显示区域越界的时候，出现超出部分不显示的问题
//////        if(write_data_s.flg2 == 1)
//////        {
//////            write_data_s.flg2 = 0;
//////            if(write_data_s.offset_len > 0)
//////            {
//////                LCD_DriverWriteDataFromExtFlash(address, write_data_s.offset_len);
//////            }
//////            if(write_data_s.data_len > 0)
//////            {
//////                LCD_SetPositionOffset(write_data_s.x, write_data_s.y, write_data_s.x_size, write_data_s.y_size, write_data_s.y_offset);
//////                LCD_DriverWriteDataFromExtFlash(address + write_data_s.offset_len, write_data_s.data_len);
//////            }
//////        }
//////        else
//////        {
//////            LCD_DriverWriteDataFromExtFlash(address, data_len);
//////        }
//////    }
//////    
//////    //ZJH.20190422.确保write_data_s.flg2标志位使用完就初始化
//////    write_data_s.flg2 = 0;
//////}


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
    
    while(read_index < data_len)
    {
        read_len = data_len - read_index;
        if(read_len > LCD_SPI_MAX_DATA)
        {   //note: 52832 SPI一次最大发送255个字节
            read_len = LCD_SPI_MAX_DATA;
        }       
        Extflash_Read(address + read_index, read_len, display_buf);       
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
** Created by:				WJ              
** Created Date:           	2017-07-25
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void LCD_DriverInit(void)
{   
    Backlight_SetLeve(0);
    
    LCD_RES_LOW;    
    LCD_DriverDelay(10);          //Delay 10ms 
    LCD_RES_HIGH;                     
    LCD_DriverDelay(10);          //Delay 10ms   
    LCD_DriverWriteCmd(0x11);     //Sleep out
    LCD_DriverDelay(120);          //Delay 120ms
    lcd_wakeup_flag = 1;

#if LCD_BL_V3  //芭乐V3屏
    LCD_DriverWriteCmd(0x36);
    
#if UI_DISPLAY_180    
    LCD_DriverWriteDataByte (0xC0);//(0x00);
#else
    LCD_DriverWriteDataByte (0x00);
#endif    
    LCD_DriverWriteCmd(0x3a);
    LCD_DriverWriteDataByte (0x05);
    LCD_DriverWriteCmd(0x21);
    LCD_DriverWriteCmd(0xE7);
    LCD_DriverWriteDataByte (0x00);	 //10h-2 data;00h-1data
    LCD_DriverWriteCmd(0x2a);
    LCD_DriverWriteDataByte (0x00);
    LCD_DriverWriteDataByte (0x00);
    LCD_DriverWriteDataByte (0x00);
    LCD_DriverWriteDataByte (0xef);
    LCD_DriverWriteCmd(0x2b);
    LCD_DriverWriteDataByte (0x00);
    LCD_DriverWriteDataByte (0x00);
    LCD_DriverWriteDataByte (0x00);
    LCD_DriverWriteDataByte (0xef);
            
    //---ST7789V Frame rate setting------//
    LCD_DriverWriteCmd(0xb2);
    LCD_DriverWriteDataByte (0x0c);
    LCD_DriverWriteDataByte (0x0c);
    LCD_DriverWriteDataByte (0x00);
    LCD_DriverWriteDataByte (0x33);
    LCD_DriverWriteDataByte (0x33);
    LCD_DriverWriteCmd(0xb7);
    LCD_DriverWriteDataByte (0x35);
    
    //--------ST7789V Power setting--------//
    LCD_DriverWriteCmd(0xbb);
    LCD_DriverWriteDataByte (0x2a);
    LCD_DriverWriteCmd(0xc0);
    LCD_DriverWriteDataByte (0x2c);
    LCD_DriverWriteCmd(0xc2);
    LCD_DriverWriteDataByte (0x01);
    LCD_DriverWriteCmd(0xc3);
    LCD_DriverWriteDataByte (0x0b);
    LCD_DriverWriteCmd(0xc4);
    LCD_DriverWriteDataByte (0x20);
    LCD_DriverWriteCmd(0xc6);
    LCD_DriverWriteDataByte (0x0f);
    LCD_DriverWriteCmd(0xd0);
    LCD_DriverWriteDataByte (0xa4);
    LCD_DriverWriteDataByte (0xa1);
    LCD_DriverWriteCmd(0xe9);
    LCD_DriverWriteDataByte (0x11);
    LCD_DriverWriteDataByte (0x11);
    LCD_DriverWriteDataByte (0x03);	

    //--------------ST7789V
    LCD_DriverWriteCmd(0xe0);
    LCD_DriverWriteDataByte (0xf0);
    LCD_DriverWriteDataByte (0x09);
    LCD_DriverWriteDataByte (0x13);
    LCD_DriverWriteDataByte (0x0a);
    LCD_DriverWriteDataByte (0x0b);
    LCD_DriverWriteDataByte (0x06);
    LCD_DriverWriteDataByte (0x38);
    LCD_DriverWriteDataByte (0x33);
    LCD_DriverWriteDataByte (0x4f);
    LCD_DriverWriteDataByte (0x04);
    LCD_DriverWriteDataByte (0x0d);
    LCD_DriverWriteDataByte (0x19);
    LCD_DriverWriteDataByte (0x2e);
    LCD_DriverWriteDataByte (0x2f);
    LCD_DriverWriteCmd(0xe1);
    LCD_DriverWriteDataByte (0xf0);
    LCD_DriverWriteDataByte (0x09);
    LCD_DriverWriteDataByte (0x13);
    LCD_DriverWriteDataByte (0x0a);
    LCD_DriverWriteDataByte (0x0b);
    LCD_DriverWriteDataByte (0x06);
    LCD_DriverWriteDataByte (0x38);
    LCD_DriverWriteDataByte (0x33);
    LCD_DriverWriteDataByte (0x4f);
    LCD_DriverWriteDataByte (0x04);
    LCD_DriverWriteDataByte (0x0d);
    LCD_DriverWriteDataByte (0x19);
    LCD_DriverWriteDataByte (0x2e);
    LCD_DriverWriteDataByte (0x2f);
#endif

    LCD_DisBlack();
    LCD_CloseScreen();
    
//    LCD_DriverWriteCmd(0x29);     //Display on    
//    LCD_OpenScreen();
//    LCD_DisWhite();
//    LCD_DisGreen();
//    LCD_DisBlue();
//    LCD_DriverDelay(5000);
//    LCD_DisYellow();
//    LCD_DisCyan();
//    LCD_DisPurple();
 
    
    
}

/****************************************************************
** Function name:			LCD_SetPositionInit
** Descriptions:            屏显示区域初始化(全屏)
** input parameters:        
** output parameters:
** Returned value:          
** Created by:				WJ              
** Created Date:           	2017-07-25
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void LCD_SetPositionInit(void)
{ 
    LCD_SetPosition(0, 0, LCD_MAX_COLUMN, LCD_MAX_ROW);
}



void LCD_DispOffset(uint16_t num)
{
    LCD_DriverWriteCmd(0x37);
    LCD_DriverWriteDataByte ((num&0xff00)>>8);
    LCD_DriverWriteDataByte (num&0xff);
}


/****************************************************************
** Function name:			LCD_SetPositionInit
** Descriptions:            屏显示区域初始化(局部)
** input parameters:        
** output parameters:       s_column:开始列,s_page:开始页,column_size:多少列,page_size:多少页
** Returned value:          
** Created by:				WJ              
** Created Date:           	2017-07-25
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void LCD_SetPosition(uint8_t s_column, uint8_t s_page, uint8_t column_size, uint8_t page_size)
{ 

#if UI_DISPLAY_180        
    uint8_t     a = 80;
    uint16_t    b = 0;
    LCD_DriverWriteCmd(0x2A); //Set Column Address
    LCD_DriverWriteDataByte(0x00);
    //LCD_DriverWriteDataByte(0x18 + s_column);
    LCD_DriverWriteDataByte(s_column);
    LCD_DriverWriteDataByte(0x00);
    //LCD_DriverWriteDataByte(0x18 + s_column + (column_size - 1));  
    LCD_DriverWriteDataByte(s_column + (column_size - 1)); 
    
    b = a+s_page;
    LCD_DriverWriteCmd(0x2B); //Set Page Address
    LCD_DriverWriteDataByte((b&0xff00)>>8);
    LCD_DriverWriteDataByte(b&0x00ff);
    
    b = a+s_page + (page_size - 1);
    LCD_DriverWriteDataByte((b&0xff00)>>8);
    LCD_DriverWriteDataByte(b&0x00ff); 
#else
    LCD_DriverWriteCmd(0x2A); //Set Column Address
    LCD_DriverWriteDataByte(0x00);
    LCD_DriverWriteDataByte(s_column);
    LCD_DriverWriteDataByte(0x00);
    LCD_DriverWriteDataByte(s_column + (column_size - 1)); 

    LCD_DriverWriteCmd(0x2B); //Set Page Address
    LCD_DriverWriteDataByte(0x00);
    LCD_DriverWriteDataByte(s_page);
    
    LCD_DriverWriteDataByte(0x00);
    LCD_DriverWriteDataByte(s_page + (page_size - 1)); 
#endif
  
    LCD_DriverWriteCmd(0x2C);//Memory Write 
}


//uint16_t test_page_tmp = 0;
void LCD_SetPositionOffset(uint8_t s_column, uint8_t s_page, uint8_t column_size, uint8_t page_size, uint16_t y_offset)
{ 
//    uint16_t page_start = 0x28+y_offset;
//    uint16_t page_tmp = 0;
//	
//	LCD_DriverWriteCmd(0x2A); //Set Column Address
//    LCD_DriverWriteDataByte(0x00);
//    LCD_DriverWriteDataByte(s_column);
//    LCD_DriverWriteDataByte(0x00);
//    LCD_DriverWriteDataByte(s_column + (column_size - 1)); 
//    LCD_DriverWriteCmd(0x2B); //Set Page Address
//    page_tmp = page_start + s_page;
//    page_tmp %= 320;
//    LCD_DriverWriteDataByte((page_tmp&0xff00)>>8);
//    LCD_DriverWriteDataByte(page_tmp&0xff);
//    page_tmp = page_start + s_page + (page_size - 1);
//    page_tmp %= 320;
//    LCD_DriverWriteDataByte((page_tmp&0xff00)>>8);
//    LCD_DriverWriteDataByte(page_tmp&0xff);
//    LCD_DriverWriteCmd(0x2C);//Memory Write 
    
    
    
    
    uint16_t page_start = y_offset;
    uint16_t page_tmp = 0;
	
	LCD_DriverWriteCmd(0x2A); //Set Column Address
    LCD_DriverWriteDataByte(0x00);
    //LCD_DriverWriteDataByte(0x18 + s_column);
    LCD_DriverWriteDataByte(s_column);
    LCD_DriverWriteDataByte(0x00);
    //LCD_DriverWriteDataByte(0x18 + s_column + (column_size - 1));  
    LCD_DriverWriteDataByte(s_column + (column_size - 1)); 
    
    LCD_DriverWriteCmd(0x2B); //Set Page Address
    page_tmp = page_start + s_page;
    page_tmp %= 320;
//    test_page_tmp = page_tmp;
    LCD_DriverWriteDataByte((page_tmp&0xff00)>>8);
    LCD_DriverWriteDataByte(page_tmp&0xff);
    
    page_tmp = page_start + s_page + (page_size - 1);
    page_tmp %= 320;
//    test_page_tmp = page_tmp;
    LCD_DriverWriteDataByte((page_tmp&0xff00)>>8);
    LCD_DriverWriteDataByte(page_tmp&0xff);
    
//    LCD_DriverWriteDataByte(0x00);
//    LCD_DriverWriteDataByte(s_page + page_tmp + (page_size - 1));//9F  
  
    LCD_DriverWriteCmd(0x2C);//Memory Write 
}

//////uint8_t LCD_SetPositionType2(uint16_t index, uint8_t s_column, uint8_t s_page, uint8_t column_size, uint8_t page_size)
//////{
//////    if(index == 0 && write_data_s.flg == 1)
//////    {
//////        if(s_page > write_data_s.y)
//////        {
//////            if(s_page < write_data_s.y + write_data_s.y_size)
//////            {
//////                if(s_page + page_size > write_data_s.y + write_data_s.y_size)
//////                {
//////                    write_data_s.offset_len = 0;
//////                    write_data_s.data_len = (write_data_s.y + write_data_s.y_size - s_page)*column_size*2;
//////                    LCD_SetPositionOffset(s_column, s_page, column_size, write_data_s.y + write_data_s.y_size - s_page, write_data_s.y_offset);
//////                }
//////                else
//////                {
//////                    write_data_s.offset_len = 0;
//////                    write_data_s.data_len = page_size*column_size*2;
//////                    LCD_SetPositionOffset(s_column, s_page, column_size, page_size, write_data_s.y_offset);
//////                }
//////            }
//////            else
//////            {
//////                write_data_s.offset_len = 0;
//////                write_data_s.data_len = 0;
//////                return 0;
//////            }
//////        }
//////        else if(s_page < write_data_s.y)
//////        {
//////            if(s_page + page_size > write_data_s.y)
//////            {
//////                if(s_page + page_size > write_data_s.y + write_data_s.y_size)
//////                {
//////                    write_data_s.offset_len = (write_data_s.y-s_page)*column_size*2;
//////                    write_data_s.data_len = write_data_s.y_size*column_size*2;
//////                    LCD_SetPositionOffset(s_column, write_data_s.y, column_size, write_data_s.y_size, write_data_s.y_offset);
//////                }
//////                else
//////                {
//////                    write_data_s.offset_len = (write_data_s.y-s_page)*column_size*2;
//////                    write_data_s.data_len = (s_page + page_size - write_data_s.y)*column_size*2;
//////                    LCD_SetPositionOffset(s_column, write_data_s.y, column_size, s_page + page_size - write_data_s.y, write_data_s.y_offset);
//////                }
//////            }
//////            else
//////            {
//////                write_data_s.offset_len = 0;
//////                write_data_s.data_len = 0;
//////                return 0;
//////            }
//////        }
//////        else
//////        {
//////            if(s_page + page_size > write_data_s.y + write_data_s.y_size)
//////            {
//////                write_data_s.offset_len = 0;
//////                write_data_s.data_len = (write_data_s.y + write_data_s.y_size - s_page)*column_size*2;
//////                LCD_SetPositionOffset(s_column, s_page, column_size, write_data_s.y + write_data_s.y_size - s_page, write_data_s.y_offset);
//////            }
//////            else
//////            {
//////                write_data_s.offset_len = 0;
//////                write_data_s.data_len = page_size*column_size*2;
//////                LCD_SetPositionOffset(s_column, s_page, column_size, page_size, write_data_s.y_offset);
//////            }
//////        }
//////    }
//////    else
//////    {
//////        //YX.20190304.解决显示区域越界的时候，出现超出部分不显示的问题
//////        if((s_page + write_data_s.y_offset < 320) && (s_page + page_size + write_data_s.y_offset > 320))
//////        {
//////            uint16_t page_tmp = 0;
//////            page_tmp = 320 - (s_page + write_data_s.y_offset);
//////            write_data_s.flg2 = 1;
//////            write_data_s.x = s_column;
//////            write_data_s.x_size = column_size;
//////            write_data_s.y = s_page+page_tmp;
//////            write_data_s.y_size = page_size - page_tmp;
//////            write_data_s.offset_len = page_tmp*column_size*2;
//////            write_data_s.data_len = (page_size-page_tmp)*column_size*2;
//////            
//////            page_size = page_tmp;
//////            
//////            //LCD_SetPositionOffset(s_column, s_page, column_size, page_tmp, write_data_s.y_offset);
//////        }

//////        LCD_SetPositionOffset(s_column, s_page, column_size, page_size, write_data_s.y_offset);      
//////    }
//////    return 1;
//////}




/****************************************************************
** Function name:			LCD_OpenScreenAll
** Descriptions:            清除屏幕,黑色背景
** input parameters:        
** output parameters:       
** Returned value:          
** Created by:				WJ              
** Created Date:           	2017-07-27
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
** Function name:			LCD_SetColorScreenByPosition
** Descriptions:            设置某块区域的颜色（单一）
** input parameters:        s_column：横向坐标
**                          s_page：纵向坐标
**                          column_size：横向大小
**                          page_size：纵向大小
**                          color：颜色数据
** output parameters:       
** Returned value:          
** Created by:				YX              
** Created Date:           	2017-08-07
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





//////void LCD_OpenScreenByPositionType2(uint16_t index, uint8_t s_column, uint8_t s_page, uint8_t column_size, uint8_t page_size)
//////{   
//////    uint8_t i = 0;
//////    uint16_t column_size_tmp = 0;
//////    uint16_t page_size_tmp = 0;
//////    
//////    //WJ.20180427.防止越界导致不知道的异常
//////    if(column_size > LCD_MAX_COLUMN)
//////    {
//////        return;
//////    }
//////      
////// 
//////    LCD_SetPositionType2(index, s_column, s_page, column_size, page_size);
//////    
//////    if(index == 0 && write_data_s.flg == 1)
//////    {
//////        if(write_data_s.data_len > 0)
//////        {
//////            page_size_tmp = write_data_s.data_len/column_size/2;
//////            column_size_tmp = write_data_s.data_len/page_size_tmp;
//////            for(i = 0; i < page_size_tmp; i++)
//////            {
//////                memset(lcd_buf, 0x00, column_size_tmp);
//////                LCD_DriverWriteDataBuf(lcd_buf, column_size_tmp);
//////            }
//////        }
//////    }
//////    else
//////    {
//////        //ZJH.20190422.解决显示区域越界，一块数据被分为头尾两块时，清屏出现异常的问题
//////        //ZJH.20190422.具体解决方法就是将其分为两块区域进行两次清屏
//////        if(write_data_s.flg2 == 1)
//////        {
//////            write_data_s.flg2 = 0;
//////            if(write_data_s.offset_len > 0)
//////            {
//////                
//////                page_size_tmp = write_data_s.offset_len/column_size/2;
//////                column_size_tmp = write_data_s.offset_len/page_size_tmp;
//////                for(i = 0; i < page_size_tmp; i++)
//////                {
//////                    memset(lcd_buf, 0x00, column_size_tmp);
//////                    LCD_DriverWriteDataBuf(lcd_buf, column_size_tmp);
//////                }
//////                
//////            }
//////            if(write_data_s.data_len > 0)
//////            {
//////                LCD_SetPositionOffset(write_data_s.x, write_data_s.y, write_data_s.x_size, write_data_s.y_size, write_data_s.y_offset);
//////                page_size_tmp = write_data_s.data_len/write_data_s.x_size/2;
//////                column_size_tmp = write_data_s.data_len/page_size_tmp;
//////                for(i = 0; i < page_size_tmp; i++)
//////                {
//////                    memset(lcd_buf, 0x00, column_size_tmp);
//////                    LCD_DriverWriteDataBuf(lcd_buf, column_size_tmp);
//////                }

//////            }
//////        }
//////        else
//////        {         
//////            for(i = 0; i < page_size; i++)									     
//////            {
//////                memset(lcd_buf, 0x00, column_size * 2);
//////                LCD_DriverWriteDataBuf(lcd_buf, column_size * 2);
//////            }
//////            
//////        }
//////    }

//////    //ZJH.20190422.因清屏函数调LCD_SetPositionType2()函数，而可能将write_data_s.flg2标志位激活，所以在此将其重新初始化为0
//////    write_data_s.flg2 = 0;       
//////}

/****************************************************************
** Function name:           LCD_DispClockData
** Descriptions:            用来实现时钟界面的数据
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              YX              
** Created Date:            2017-08-29
*****************************************************************/     
void LCD_DispClockData(uint8_t s_column, uint8_t s_page, uint8_t column_size, uint8_t page_size, uint8_t* data, uint32_t data_size)
{
    LCD_SetPosition(s_column, s_page, column_size, page_size);
    LCD_DriverWriteDataBuf(data, data_size);
}

/****************************************************************
** Function name:           LCD_DispClockData
** Descriptions:            用来实现时钟界面的数据
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              YX              
** Created Date:            2017-08-29
*****************************************************************/     
//////void LCD_DispClockDataType2(uint16_t index, uint8_t s_column, uint8_t s_page, uint8_t column_size, uint8_t page_size, uint8_t* data, uint32_t data_size)
//////{
//////    uint8_t ret = 1;
//////    ret = LCD_SetPositionType2(index, s_column, s_page, column_size, page_size);
//////    if(ret == 1)
//////    {
//////        LCD_DriverWriteDataBuf(data, data_size);
//////    }
//////}




/****************************************************************
** Function name:           LCD_DispTextDataType2
** Descriptions:            用来实现字库的数据
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              YX              
** Created Date:            2017-08-29
*****************************************************************/     
//////void LCD_DispTextDataType2(uint16_t index, uint8_t s_column, uint8_t s_page, uint8_t column_size, uint8_t page_size, uint8_t* data, uint32_t data_size)
//////{
//////    if(index == 0 && write_data_s.flg == 1)
//////    {
//////        if(s_page > write_data_s.y)
//////        {
//////            if(s_page < write_data_s.y + write_data_s.y_size)
//////            {
//////                if(s_page + page_size > write_data_s.y + write_data_s.y_size)
//////                {
//////                    write_data_s.offset_len = 0;
//////                    write_data_s.data_len = (write_data_s.y + write_data_s.y_size - s_page)*column_size*2;
//////                    LCD_SetPositionOffset(s_column, s_page, column_size, write_data_s.y + write_data_s.y_size - s_page, write_data_s.y_offset);
//////                    LCD_DriverWriteDataBuf(data, write_data_s.data_len);
//////                }
//////                else
//////                {
//////                    write_data_s.offset_len = 0;
//////                    write_data_s.data_len = page_size*column_size*2;
//////                    LCD_SetPositionOffset(s_column, s_page, column_size, page_size, write_data_s.y_offset);
//////                    LCD_DriverWriteDataBuf(data, write_data_s.data_len);
//////                }
//////            }
//////            else
//////            {
//////                write_data_s.offset_len = 0;
//////                write_data_s.data_len = 0;
//////                return ;
//////            }
//////        }
//////        else if(s_page < write_data_s.y)
//////        {
//////            if(s_page + page_size > write_data_s.y)
//////            {
//////                if(s_page + page_size > write_data_s.y + write_data_s.y_size)
//////                {
//////                    write_data_s.offset_len = (write_data_s.y-s_page)*column_size*2;
//////                    write_data_s.data_len = write_data_s.y_size*column_size*2;
//////                    LCD_SetPositionOffset(s_column, write_data_s.y, column_size, write_data_s.y_size, write_data_s.y_offset);
//////                    LCD_DriverWriteDataBuf(data+write_data_s.offset_len, write_data_s.data_len);
//////                }
//////                else
//////                {
//////                    write_data_s.offset_len = (write_data_s.y-s_page)*column_size*2;
//////                    write_data_s.data_len = (s_page + page_size - write_data_s.y)*column_size*2;
//////                    LCD_SetPositionOffset(s_column, write_data_s.y, column_size, s_page + page_size - write_data_s.y, write_data_s.y_offset);
//////                    LCD_DriverWriteDataBuf(data+write_data_s.offset_len, write_data_s.data_len);
//////                }
//////            }
//////            else
//////            {
//////                write_data_s.offset_len = 0;
//////                write_data_s.data_len = 0;
//////                return ;
//////            }
//////        }
//////        else
//////        {
//////            if(s_page + page_size > write_data_s.y + write_data_s.y_size)
//////            {
//////                write_data_s.offset_len = 0;
//////                write_data_s.data_len = (write_data_s.y + write_data_s.y_size - s_page)*column_size*2;
//////                LCD_SetPositionOffset(s_column, s_page, column_size, write_data_s.y + write_data_s.y_size - s_page, write_data_s.y_offset);
//////                LCD_DriverWriteDataBuf(data+write_data_s.offset_len, write_data_s.data_len);
//////            }
//////            else
//////            {
//////                write_data_s.offset_len = 0;
//////                write_data_s.data_len = page_size*column_size*2;
//////                LCD_SetPositionOffset(s_column, s_page, column_size, page_size, write_data_s.y_offset);
//////                LCD_DriverWriteDataBuf(data+write_data_s.offset_len, write_data_s.data_len);
//////            }
//////        }
//////    }
//////    else
//////    {
//////        //YX.20190304.解决显示区域越界的时候，出现超出部分不显示的问题
//////        if((s_page + write_data_s.y_offset < 320) && (s_page + page_size + write_data_s.y_offset > 320))
//////        {
//////            uint16_t page_tmp = 0;
//////            page_tmp = 320 - (s_page + write_data_s.y_offset);
//////            write_data_s.flg2 = 1;
//////            write_data_s.x = s_column;
//////            write_data_s.x_size = column_size;
//////            write_data_s.y = s_page+page_tmp;
//////            write_data_s.y_size = page_size - page_tmp;
//////            write_data_s.offset_len = page_tmp*column_size*2;
//////            write_data_s.data_len = (page_size-page_tmp)*column_size*2;
//////            LCD_SetPositionOffset(s_column, s_page, column_size, page_tmp, write_data_s.y_offset);
//////            LCD_DriverWriteDataBuf(data, write_data_s.offset_len);
//////            LCD_SetPositionOffset(write_data_s.x, write_data_s.y, write_data_s.x_size, write_data_s.y_size, write_data_s.y_offset);
//////            LCD_DriverWriteDataBuf(data + write_data_s.offset_len, write_data_s.data_len);
//////        }
//////        else
//////        {
//////            LCD_SetPositionOffset(s_column, s_page, column_size, page_size, write_data_s.y_offset);
//////            LCD_DriverWriteDataBuf(data, data_size);
//////        }
//////    }
//////}

/****************************************************************
** Function name:			LCD_CloseScreen
** Descriptions:            关屏,清除内容,关闭背景灯,进入休眠模式
** input parameters:        
** output parameters:       
** Returned value:          
** Created by:				WJ              
** Created Date:           	2017-07-27
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
** Function name:			LCD_OpenScreen
** Descriptions:            开屏退出休眠模式
** input parameters:        
** output parameters:       
** Returned value:          
** Created by:				WJ              
** Created Date:           	2017-07-27
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
    
    //LCD_EN_ON;   
    Backlight_SetLeve(BACKLIGHT_MAX_CHANAGE_LEVEL);
}

/****************************************************************
** Function name:           LCD_CloseLed
** Descriptions:            关屏,关闭背景灯
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
    //LCD_EN_ON;
    //Backlight_SetLeve(lcd_backlight_level);
}

/*******************************************测试************************************************/
/*
//白色	
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


