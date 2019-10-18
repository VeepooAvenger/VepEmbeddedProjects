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

//��ʼ����Ļ
void LCD_DriverInit(void);

//��ʼ��ȫ����ʾ����
void LCD_SetPositionInit(void);	

//������ʾ����
void LCD_SetPosition(uint8_t s_column, uint8_t s_page, uint8_t column_size, uint8_t page_size);

//д��������
uint8_t LCD_DriverWriteDataBuf(const uint8_t *data, const uint32_t data_len);

//���ⲿflash��ȡUI���������ʾ;address:flash���ݵ�ַ,data_len:���ݳ���
void LCD_DriverWriteDataFromExtFlash(const uint32_t address, const uint32_t data_len);

//д�������ݴ�����ɫ
uint8_t LCD_DriverWriteDataBufBackground(const uint8_t *data, const uint16_t data_len, uint16_t color);

//���ⲿflash��ȡUI���������ʾ������䱳����ɫ;address:flash���ݵ�ַ,data_len:���ݳ��ȣ�color��������ɫ
void LCD_DriverWriteDataFromExtFlashAndBackground(const uint32_t address, const uint16_t data_len, uint16_t color);

//���ⲿflash��ȡUI���������ʾ������䱳����ɫ,������ɫ;address:flash���ݵ�ַ,data_len:���ݳ��ȣ�
//back_groundcolor��������ɫ;main_color����ɫ;sub_color��������ɫ
void LCD_DriverWriteDataFromExtFlashAndChangeBackground(const uint32_t address, const uint16_t data_len, 
                                                        uint16_t back_groundcolor, uint16_t main_color, uint16_t sub_color);

//���ⲿflash��ȡUI�����������
void LCD_DriverReadFromExtFlash(const uint32_t address, uint8_t *data, const uint16_t data_len);
    
//��ȫ��
void LCD_OpenScreenAll(void);

//��������
void LCD_OpenScreenByPosition(uint8_t s_column, uint8_t s_page, uint8_t column_size, uint8_t page_size);

//����������ɫ
void LCD_SetColorScreenByPosition(uint8_t s_column, uint8_t s_page, uint8_t column_size, uint8_t page_size, uint16_t color);

//����,����ȫ��
void LCD_CloseScreen(void);

//����
void LCD_OpenScreen(void);

//�رձ���
void LCD_CloseLed(void);

//����LCD
void LCD_WakeUp(void);

//����ʹ��
void LCD_ChangePageColorTest(void);


#endif


