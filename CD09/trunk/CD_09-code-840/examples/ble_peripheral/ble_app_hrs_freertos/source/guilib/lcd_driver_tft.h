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
#ifndef __LCD_DRIVER_TFT_H
#define __LCD_DRIVER_TFT_H
#include "board.h"

#define LCD_SPI_CS_LOW      PIN_LOW(LCD_CS_PIN);
#define LCD_SPI_CS_HIGH     PIN_HIGH(LCD_CS_PIN);

#define LCD_MAX_ROW                 160
#define LCD_MAX_COLUMN              80

//初始化屏幕
void LCD_DriverInit(void);

//初始化全屏显示区域
void LCD_SetPositionInit(void);	

//设置显示区域
void LCD_SetPosition(uint8_t s_column, uint8_t s_page, uint8_t column_size, uint8_t page_size);

//写数据内容
uint8_t LCD_DriverWriteDataBuf(const uint8_t *data, const uint32_t data_len);

//从外部flash获取UI数据填充显示;address:flash数据地址,data_len:数据长度
void LCD_DriverWriteDataFromExtFlash(const uint32_t address, const uint32_t data_len);

//写数据内容带背景色
uint8_t LCD_DriverWriteDataBufBackground(const uint8_t *data, const uint16_t data_len, uint16_t color);

//从外部flash获取UI数据填充显示，并填充背景颜色;address:flash数据地址,data_len:数据长度；color：背景颜色
void LCD_DriverWriteDataFromExtFlashAndBackground(const uint32_t address, const uint16_t data_len, uint16_t color);

//从外部flash获取UI数据填充显示，并填充背景颜色,更换主色;address:flash数据地址,data_len:数据长度；
//back_groundcolor：背景颜色;main_color：主色;sub_color：辅助颜色
void LCD_DriverWriteDataFromExtFlashAndChangeBackground(const uint32_t address, const uint16_t data_len, 
                                                        uint16_t back_groundcolor, uint16_t main_color, uint16_t sub_color);

//从外部flash获取UI数据填充数组
void LCD_DriverReadFromExtFlash(const uint32_t address, uint8_t *data, const uint16_t data_len);
    
//清全屏
void LCD_OpenScreenAll(void);

//区域清屏
void LCD_OpenScreenByPosition(uint8_t s_column, uint8_t s_page, uint8_t column_size, uint8_t page_size);

//区域设置颜色
void LCD_SetColorScreenByPosition(uint8_t s_column, uint8_t s_page, uint8_t column_size, uint8_t page_size, uint16_t color);

//关屏,并清全屏
void LCD_CloseScreen(void);

//开屏
void LCD_OpenScreen(void);

//关闭背光
void LCD_CloseLed(void);

//唤醒LCD
void LCD_WakeUp(void);

//测试使用
void LCD_ChangePageColorTest(void);


#endif


