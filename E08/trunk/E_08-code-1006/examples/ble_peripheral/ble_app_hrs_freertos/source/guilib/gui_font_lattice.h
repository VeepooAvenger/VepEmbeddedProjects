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

//显示行
#define FIRST_PAGE_ROWS     4
#define PER_PAGE_MAX_ROWS   7//每一页最大行数
#define MAX_PAGE_LEN        20//10

//字间间隔
#define LCD_FONT_INTERVAL           0
//字宽
#define LCD_FONT_8_16_WIDTH         8
#define LCD_FONT_8_16_HIGH          16
#define LCD_FONT_16_16_WIDTH        16
#define LCD_FONT_16_16_HIGH         16

//用于处理字体颜色
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
    FONT_ERROR_TYPE_ID = 0,     //不能识别
    FONT_ASCII_TYPE_ID = 0x01, /* ASCII类型 */
    FONT_CHINESE_TYPE_ID,      /* 中文汉字 */
    FONT_KOREAN_TYPE_ID,       /* 韩语 */
    FONT_JAPANESE_TYPE_ID,     /* 日语 */
    FONT_CYRILLIC_TYPE_ID,     /* 西里拉文 */
    FONT_LATTIN_TYPE_ID,       /* 拉丁文 */
}FONT_TYPE_ID_E;

//生产测试显示字库数据
void Font_DisplayFontTestText(void);

//void Font_DisplayLatticeText(uint8_t *p_content, uint16_t len, uint8_t page);

//计算信息内容分页信息; p_title：标题内容；title_len：标题长度;
//p_content：内容；content_len：内容长度;
//start_page_index：分页起始内容数组下标;
//返回总共有多少页信息
uint8_t Font_FullScreenTotalCount(uint8_t *p_title, uint16_t title_len,
                uint8_t *p_content, uint16_t content_len, uint16_t *start_page_index);

//显示字库内容;p_content：文字数组；content_len:内容长度；
//start_page：开始显示行数;total_rows：显示行数
uint8_t Font_DisplayContentText(uint8_t *p_content, uint16_t content_len, uint8_t start_page, uint8_t total_rows);


//显示字库内容加间隔;p_content：文字数组；content_len:内容长度；
//start_page：开始显示行数;total_rows：显示行数 
//x_interval:纵向间隔;y_interval：横向间隔
void Font_DisContentText(uint8_t *p_content, uint16_t content_len, uint8_t start_page, 
                         uint8_t total_rows, uint8_t x_interval, uint8_t y_interval);

void Font_DisplayTitleText(uint8_t *p_title, uint16_t title_len, uint8_t start_page, uint8_t total_rows);

//识别文字类型返回占位宽度
//p_title：文字数组；title_len：长度
//返回消息内容的有效长度
uint16_t Font_FullScreenTotalActiveLenght(uint8_t *p_title, uint16_t title_len);

uint8_t Font_FullScreenTotalCountByStr(uint8_t *p_content, uint16_t content_len, uint16_t *start_page_index);

uint8_t Font_DisUiTextOneLine2X(uint32_t index, uint8_t *p_content, uint16_t content_len, uint8_t start_x, uint8_t start_y, 
                                uint8_t is_middle, uint8_t x_interval, uint8_t y_interval, uint16_t backcolor);


// 消息第一页显示
uint16_t Font_DisplayContentTextFirstPage2X(uint8_t *p_content, uint16_t content_len, uint8_t start_y, uint8_t total_rows);
#endif
