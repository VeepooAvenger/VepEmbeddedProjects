/***************************Copyright(c)***********************************
**                       Veepoo Tech
**                  COPYRIGHT (C) 2016-2019
**-----------File Info-----------------------------------------------------
** File        name:     gui_font_lattice.h
** Last modified Date:   2017-08-01
** Last SYS_VERSION:     0.0.0
** Descriptions:            
**-------------------------------------------------------------------------
** Created  by:          WJ
** Created  date:        2017-08-01
** SYS_VERSION:          0.0.0
** Descriptions:         
**-------------------------------------------------------------------------
** Modified by:				
** Modified date:  			          
** SYS_VERSION:                 
** Descriptions:                        
***************************************************************************/
#ifndef _GUI_FONT_LATTICE_H_
#define _GUI_FONT_LATTICE_H_

#include <stdint.h>

#define DIS_MAX_X_SIZE      (240 - 8)
#define DIS_MAX_Y_SIZE      240

#define DIS_START_X         (8)
#define DIS_FIRST_Y_SIZE    240

//��ʾ��
#define FIRST_PAGE_ROWS     4
#define PER_PAGE_MAX_ROWS   7//ÿһҳ�������
#define MAX_PAGE_LEN        20//10

//�ּ���
#define LCD_FONT_INTERVAL           0
//�ֿ�
#define LCD_FONT_8_16_WIDTH         8
#define LCD_FONT_8_16_HIGH          16
#define LCD_FONT_16_16_WIDTH        16
#define LCD_FONT_16_16_HIGH         16

//���ڴ���������ɫ
#define LCD_COLOR_BLACK		(0x0000)
#define LCD_COLOR_WHITE		(0xFFFF)
#define LCD_COLOR_RED		(0x00F8)
#define LCD_COLOR_YELLOW	(0xE0FF)
#define LCD_COLOR_CYAN		(0xFF07)
#define LCD_COLOR_PURPLE	(0x1FF8)
#define LCD_COLOR_DISGREEN	(0xE007)
#define LCD_COLOR_BLUE		(0x1F00)

typedef enum
{
    FONT_ERROR_TYPE_ID = 0,     //����ʶ��
    FONT_ASCII_TYPE_ID = 0x01, /* ASCII���� */
    FONT_CHINESE_TYPE_ID,      /* ���ĺ��� */
    FONT_KOREAN_TYPE_ID,       /* ���� */
    FONT_JAPANESE_TYPE_ID,     /* ���� */
    FONT_CYRILLIC_TYPE_ID,     /* �������� */
    FONT_LATTIN_TYPE_ID,       /* ������ */
}FONT_TYPE_ID_E;

//����������ʾ�ֿ�����
void Font_DisplayFontTestText(void);

//void Font_DisplayLatticeText(uint8_t *p_content, uint16_t len, uint8_t page);

//������Ϣ���ݷ�ҳ��Ϣ; p_title���������ݣ�title_len�����ⳤ��;
//p_content�����ݣ�content_len�����ݳ���;
//start_page_index����ҳ��ʼ���������±�;
//�����ܹ��ж���ҳ��Ϣ
uint8_t Font_FullScreenTotalCount(uint8_t *p_title, uint16_t title_len,
                uint8_t *p_content, uint16_t content_len, uint16_t *start_page_index);

//��ʾ�ֿ�����;p_content���������飻content_len:���ݳ��ȣ�
//start_page����ʼ��ʾ����;total_rows����ʾ����
uint8_t Font_DisplayContentText(uint8_t *p_content, uint16_t content_len, uint8_t start_page, uint8_t total_rows);


//��ʾ�ֿ����ݼӼ��;p_content���������飻content_len:���ݳ��ȣ�
//start_page����ʼ��ʾ����;total_rows����ʾ���� 
//x_interval:������;y_interval��������
void Font_DisContentText(uint8_t *p_content, uint16_t content_len, uint8_t start_page, 
                         uint8_t total_rows, uint8_t x_interval, uint8_t y_interval);

void Font_DisplayTitleText(uint8_t *p_title, uint16_t title_len, uint8_t start_page, uint8_t total_rows);

//ʶ���������ͷ���ռλ���
//p_title���������飻title_len������
//������Ϣ���ݵ���Ч����
uint16_t Font_FullScreenTotalActiveLenght(uint8_t *p_title, uint16_t title_len);

uint8_t Font_FullScreenTotalCountByStr(uint8_t *p_content, uint16_t content_len, uint16_t *start_page_index);

uint8_t Font_DisUiTextOneLine2X(uint32_t index, uint8_t *p_content, uint16_t content_len, uint8_t start_x, uint8_t start_y, 
                                uint8_t is_middle, uint8_t x_interval, uint8_t y_interval, uint16_t backcolor);


// ��Ϣ��һҳ��ʾ
uint16_t Font_DisplayContentTextFirstPage2X(uint8_t *p_content, uint16_t content_len, uint8_t start_y, uint8_t total_rows);
#endif
