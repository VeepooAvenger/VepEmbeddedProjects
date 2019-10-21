/***************************Copyright(c)***********************************
**                       Veepoo Tech
**                  COPYRIGHT (C) 2016-2019
**-----------File Info-----------------------------------------------------
** File        name:     gui_font_lattice.c
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
#include <string.h>
#include <stdbool.h>
#include "gui_font_lattice.h"
#include "fontlib_spi.h"
#include "lcd_driver_tft.h"
//#include "phone_notify_event.h"

#define ATTR_DATA_SIZE            64//BLE_ANCS_ATTR_DATA_MAX                      /**< Allocated size for attribute data. */		
#define ATTR_MESSAGE_DATA_SIZE    300

#define TLTLE_DATA_MAX_LEN      ATTR_DATA_SIZE//32
#define CONTENT_DATA_MAX_LEN    ATTR_MESSAGE_DATA_SIZE//220//128

//const uint8_t icon_test_zifu[5][16*2] = {
//    {
//    0x20,0x30,0xAC,0x63,0x20,0x58,0x20,0xF8,0x4F,0x48,0x49,0xFA,0x48,0x48,0x08,0x00,
//    0x22,0x67,0x22,0x12,0x12,0x12,0x00,0xFF,0x22,0x22,0x22,0x3F,0x22,0x22,0x20,0x00,
//    },
//    {
//    0x00,0x80,0x60,0xF8,0x07,0x00,0x04,0x04,0x84,0x44,0x24,0x14,0x0C,0x04,0x00,0x00,
//    0x01,0x00,0x00,0xFF,0x00,0x00,0x38,0x46,0x41,0x40,0x40,0x40,0x40,0x40,0x78,0x00,
//    },
//    {
//    0x00,0xF8,0x8C,0x8B,0x88,0xF8,0x00,0xFC,0x24,0x26,0xFD,0x24,0x24,0xFC,0x00,0x00,
//    0x00,0x3F,0x10,0x10,0x10,0x3F,0x80,0x61,0x19,0x07,0x7D,0x99,0x95,0x91,0xD8,0x00,
//    },
//    {
//    0x24,0x24,0xA4,0xFE,0xA3,0x22,0x00,0x22,0xCC,0x00,0x00,0xFF,0x00,0x00,0x00,0x00,
//    0x08,0x06,0x01,0xFF,0x00,0x01,0x04,0x04,0x04,0x04,0x04,0xFF,0x02,0x02,0x02,0x00,
//    },
//    {
//    0x10,0x10,0x10,0xFF,0x10,0x90,0x08,0x88,0x88,0x88,0xFF,0x88,0x88,0x88,0x08,0x00,
//    0x04,0x44,0x82,0x7F,0x01,0x80,0x80,0x40,0x43,0x2C,0x10,0x28,0x46,0x81,0x80,0x00,
//    }
//};

uint8_t Font_FullScreenTotalWidth(uint8_t *p_title, uint16_t title_len, uint8_t *font_num);


/*******************************************************************************
** Function name:		GUIFont_8X16FontChange
** Descriptions:        ���ֿ���ȡ��8*16�����ݿ��������������ʾ
** input parameters:    start_x��x����
**                      start_y��y����
**                      data������
**                      font_color_data��������ɫ����
**                      back_color_data:������ɫ����
** output parameters:
** Returned value:
** Created by:			YX              
** Created Date:        2017-07-21
**------------------------------------------------------------------------------
** Modified by:
** Modified date:
**------------------------------------------------------------------------------
*******************************************************************************/
void Font_8X16FontChangeToColorScreen(uint8_t start_colunm, uint8_t start_page, 
                                     const uint8_t *data, uint16_t font_color_data, uint16_t back_color_data)
{
    uint8_t change_buf[LCD_FONT_8_16_HIGH * LCD_FONT_8_16_WIDTH * 2 + 1] = {0x00,};
    uint8_t font_color_data_l = 0;
    uint8_t font_color_data_h = 0;
    uint16_t buf_index = 0;
    uint16_t i = 0, j = 0, m = 0, n = 0;
    
    font_color_data_l = font_color_data & 0xff;
    font_color_data_h = (font_color_data >>8 ) & 0xff;
    
    for(j = 0; j < LCD_FONT_8_16_HIGH; j++)
    {      
        m = j % 8;     
        for(i = 0; i < LCD_FONT_8_16_WIDTH; i++)
        {
            n = i + (j / 8) * LCD_FONT_8_16_WIDTH;
             if(((data[n] >> m) & 0x01) == 0x01)
             {
                 change_buf[buf_index++] = font_color_data_l;
                 change_buf[buf_index++] = font_color_data_h;
             }
             else
             {
                 change_buf[buf_index++] = 0x00;
                 change_buf[buf_index++] = 0x00;
             }
        }
    }
    LCD_SetPosition(start_colunm, start_page, LCD_FONT_8_16_WIDTH, LCD_FONT_8_16_HIGH);
    LCD_DriverWriteDataBuf(change_buf, LCD_FONT_8_16_HIGH * LCD_FONT_8_16_WIDTH * 2);
}

/*******************************************************************************
** Function name:	    LCD_Driver_Write_16_16Font
** Descriptions:        ���ֿ���ȡ��16*16�����ݿ��������������ʾ
** input parameters:    start_x��x����
**                      start_y��y����
**                      data������
**                      font_color_data��������ɫ����
**                      back_color_data:������ɫ����
** output parameters:
** Returned value:
** Created by:			YX              
** Created Date:        2017-07-21
**------------------------------------------------------------------------------
** Modified by:
** Modified date:
**------------------------------------------------------------------------------
*******************************************************************************/
void GUIFont_16X16FontChangeToColorScreen(uint8_t start_colunm, uint8_t start_page, 
                                         const uint8_t *data, uint16_t font_color_data, uint16_t back_color_data)
{
    uint8_t change_buf[LCD_FONT_16_16_HIGH * LCD_FONT_16_16_WIDTH*2+1] = {0x00,};
    uint8_t font_color_data_l = 0;
    uint8_t font_color_data_h = 0;
    uint16_t buf_index = 0;
    uint16_t i = 0, j = 0, m = 0, n = 0;
    
    font_color_data_l = font_color_data & 0xff;
    font_color_data_h = (font_color_data >>8 ) & 0xff;

    for(j = 0; j < LCD_FONT_16_16_HIGH; j++)
    {
        m = j % 8;
        for(i = 0; i < LCD_FONT_16_16_WIDTH; i++)
        {
             n = i + (j / 8) * LCD_FONT_16_16_WIDTH;
             if(((data[n] >> m) & 0x01) == 0x01)
             {
                 change_buf[buf_index++] = font_color_data_l;
                 change_buf[buf_index++] = font_color_data_h;
             }
             else
             {
                 change_buf[buf_index++] = 0x00;
                 change_buf[buf_index++] = 0x00;
             }
        }
    }
    LCD_SetPosition(start_colunm, start_page, LCD_FONT_16_16_WIDTH, LCD_FONT_16_16_HIGH);
    LCD_DriverWriteDataBuf(change_buf, LCD_FONT_16_16_HIGH * LCD_FONT_16_16_WIDTH * 2);
}

/*******************************************************************************
** Function name:	    Font_ImageLeftRotate
** Descriptions:        ͼƬ����ת���������ڵ���
** input parameters:    pInput���������飻width��ͼƬ�Ŀ�high��ͼƬ�ĸ�

** output parameters:   pOutput:����������
** Returned value:
** Created by:		    WJ             
** Created Date:        2017-07-21
**------------------------------------------------------------------------------
** Modified by:
** Modified date:
**------------------------------------------------------------------------------
*******************************************************************************/
void Font_ImageLeftRotate(uint8_t *pInput, uint8_t *pOutput, uint8_t width, uint8_t high) 
{
	//����ת Add by GT 2017-04-05
	for(uint16_t i = 0; i < width; i++)
	{
		for(int j = 0; j < high; j++)
		{
			if(pInput[i + width * (int)(j / 8)] & (0x01<<( j % 8)))
            {
				pOutput[high * ((int)(width - 1 - i) / 8) + j] |= (0x01<<((width - 1 - i)%8));
            }
			else
            {
				pOutput[high * ((int)(width - 1 - i) / 8) + j] &= ~(0x01<<((width - 1 - i)%8));
            }
		}
	}
}

/*******************************************************************************
** Function name:	    Font_DisplayFontTestText
** Descriptions:        ����������ʾ�ֿ�����
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
** Created by:			WJ             
** Created Date:        2017-07-21
**------------------------------------------------------------------------------
** Modified by:
** Modified date:
**------------------------------------------------------------------------------
*******************************************************************************/
//uint8_t code[32] = {0};
//#define  BLOCK_SIZE      128
//#define  BLOCK_NUM       (2 * 1024 * 1024 / BLOCK_SIZE)
//static uint16_t crc_font = 0xffff;
//static uint16_t crc_flash = 0xffff;
//static uint8_t data_buff[BLOCK_SIZE];
void Font_DisplayFontTestText(void)
{
     /* ��Ļ����ʾ"Ψ���ǿƼ�"������ */
    uint8_t temp[15] = {0xE7,0xBB,0xB4,0xE4,0xBA,0xBF,0xE9,0xAD,0x84,0xE7,0xA7,0x91,0xE6,0x8A,0x80};
    uint8_t code[32] = {0}; 
    uint8_t start_page = 0;
    uint8_t start_colum = 0;
    
    start_page = 14;
    start_colum = 32;
 


//    uint32_t block_i = 0;
//    uint32_t addr = 0;
//    uint8_t  buf[4];
//     
//    crc_font = 0xffff;
//    crc_flash = 0xffff;  
//    for(block_i = 0; block_i < BLOCK_NUM; ++block_i)
//    {
//        addr = block_i * BLOCK_SIZE;
//        Bps_Spi2SelectGt24(1);
//        Extflash_Read(addr, BLOCK_SIZE, data_buff);
//        crc_flash = Crc16(crc_flash, data_buff, BLOCK_SIZE);
//    }
//    
//    //0xBF01:ȫ0xFF //�ֿ⣺0x387C // flash�ֿ⣺0x3D18
//    if(crc_flash != crc_font)
//    {
//        uint8_t array[32] = {0xff};
//        
//    }
//    else
//    {
//        crc_flash = 0;
//        crc_font = 0;
//    }
    
    Font_Wakeup();  
    for(uint8_t i = 0; i < 5; i++)
    {
        Font_ChineseLattice(&temp[i*3], code);               
//        Font_ImageLeftRotate((void*)code, rotate_code, 16, 16);          
        GUIFont_16X16FontChangeToColorScreen(start_colum, start_page, code, LCD_COLOR_WHITE, LCD_COLOR_BLACK);   
//        GUIFont_16X16FontChangeToColorScreen(start_page, start_colum, &icon_test_zifu[i][0], LCD_COLOR_WHITE, LCD_COLOR_BLACK);  
        start_page += (16 + 13);
    }  
    Font_Sleep();    
}

/*******************************************************************************
** Function name:	    Font_GetType
** Descriptions:        ʶ����������
** input parameters:    p_data����������
** output parameters:   ��
** Returned value:      ��
** Created by:			WJ             
** Created Date:        2017-07-21
**------------------------------------------------------------------------------
** Modified by:
** Modified date:
**------------------------------------------------------------------------------
*******************************************************************************/
static FONT_TYPE_ID_E Font_GetType(uint8_t *p_data)
{
    FONT_TYPE_ID_E	font_type = FONT_ASCII_TYPE_ID;
    uint16_t gbk_code = 0;
    
    /* UTF8���� */
    if((p_data[0] & 0xE0) == 0xE0)
    {          
        gbk_code = UTF8_TO_GBK(p_data);        

        //WJ.20171227.�������ķ��Ų���ʾ�����⣬�������κ󲿷��ַ�����ʾ�ո�(��ĳЩͼƬ)
//        if(((gbk_code > 0x202D) && (gbk_code <= 0x206F)) //20170626.YX �޸������ַ�ʶ�𲻳���
//          || ((gbk_code >= 0x3000) && (gbk_code <= 0x303F))
//          || ((gbk_code >= 0xFF00 ) && (gbk_code <= 0xFFFF)))
        {
            /* ������ */
            font_type = FONT_CHINESE_TYPE_ID;
        }
        if((gbk_code >= 0x4E00) && (gbk_code <= 0x9FA5))
        {
            /* �������� */
            font_type = FONT_CHINESE_TYPE_ID;	
        }
        if((gbk_code >= 0xAC00) && (gbk_code <= 0xD7AF))
        {
            /* ��������(һ) */
            font_type = FONT_KOREAN_TYPE_ID;
        }        
        if((gbk_code >= 0x1100) && (gbk_code <= 0x11FF))
        {
            /* ��������(��) */
            font_type = FONT_KOREAN_TYPE_ID;
        }
        if((gbk_code >= 0x3130) && (gbk_code <= 0x318F))
        {
            /* ��������(��) */
            font_type = FONT_KOREAN_TYPE_ID;
        }
        if((gbk_code >= 0x3040) && (gbk_code <= 0x30FF))
        {
            /* ���� */
            font_type = FONT_JAPANESE_TYPE_ID;
        }

        if(((gbk_code >= 0x2C60) && (gbk_code <= 0x2C7F)))
        {
            /* ������ϵ��չ�ַ� */
            font_type = FONT_LATTIN_TYPE_ID;
        }
        if((gbk_code >= 0xA720) && (gbk_code <= 0xA7FF))
        {
            font_type = FONT_LATTIN_TYPE_ID;
        }

        if((gbk_code >= 0x1E00) && (gbk_code <= 0x1EFF))
        {
            font_type = FONT_LATTIN_TYPE_ID;
        }
        
        if(font_type == FONT_ASCII_TYPE_ID)
        {
            font_type = FONT_ERROR_TYPE_ID;     //20170626.YX ����ĳЩͼƬ���ո������
        }

    }
    else if((p_data[0] & 0xC0) == 0xC0)
    {
        uint32_t unicode = UTF8_TO_UNICODE(p_data);
        if(( unicode >=0x400) && (unicode <= 0x4FF))
        {
            font_type = FONT_CYRILLIC_TYPE_ID;
        }
        else if((unicode >= 0x80) && (unicode <= 0x24F))
        {
            font_type = FONT_LATTIN_TYPE_ID;
        }
    }
    else if((p_data[0] & 0x80) == 0x00)
    {
        font_type = FONT_ASCII_TYPE_ID; 
    }
    else
    {
        font_type = FONT_ERROR_TYPE_ID;     //WJ.20170418.����ĳЩͼƬ���ո������
    }
    return font_type;
}

/*******************************************************************************
** Function name:	    Font_FontWidth
** Descriptions:        ʶ���������ͷ���ռλ���
** input parameters:    p_data����������
** output parameters:   ��
** Returned value:      ��
** Created by:			WJ             
** Created Date:        2017-07-21
**------------------------------------------------------------------------------
** Modified by:
** Modified date:
**------------------------------------------------------------------------------
*******************************************************************************/
static uint8_t Font_FontWidth(uint8_t *p_data)
{
    uint8_t font_width = 8;
    uint16_t gbk_code = 0;
    
    /* UTF8���� */
    if((p_data[0] & 0xE0) == 0xE0)
    {
        gbk_code = UTF8_TO_GBK(p_data); 
        
        if(((gbk_code >= 0x2C60) && (gbk_code <= 0x2C7F))
          || ((gbk_code >= 0xA720) && (gbk_code <= 0xA7FF))
          || ((gbk_code >= 0x1E00) && (gbk_code <= 0x1EFF)))
        {
            font_width = 8;
        }
        else
        {
            font_width = 16;
        }
    }
    return font_width;
}
static uint8_t Font_FontWidth16(uint16_t *p_data)
{
    uint8_t font_width = 8;
    uint16_t gbk_code = 0;
    
    /* UTF8���� */
    if((p_data[0] & 0xE0) == 0xE0)
    {
        gbk_code = UTF8_TO_GBK(p_data); 
        
        if(((gbk_code >= 0x2C60) && (gbk_code <= 0x2C7F))
          || ((gbk_code >= 0xA720) && (gbk_code <= 0xA7FF))
          || ((gbk_code >= 0x1E00) && (gbk_code <= 0x1EFF)))
        {
            font_width = 8;
        }
        else
        {
            font_width = 16;
        }
    }
    return font_width;
}
/*******************************************************************************
** Function name:	    Font_CodeByteNum
** Descriptions:        ʶ������������ռ���鳤��
** input parameters:    p_data����������
** output parameters:   ��
** Returned value:      ��
** Created by:			WJ             
** Created Date:        2017-07-21
**------------------------------------------------------------------------------
** Modified by:
** Modified date:
**------------------------------------------------------------------------------
*******************************************************************************/
static uint8_t Font_CodeByteNum(uint8_t *p_data)
{
    uint8_t code_byte = 1;

    /* UTF8���� */
    //YX.20180514.��ע�����޷���ʾ������ʱ����ȡ��������Ϣ�������ʾ����UI�����֣���BUG��2577��
    if((0xF0 & p_data[0]) == 0xF0)
    {
        code_byte = 4;
    }
    else if((p_data[0] & 0xE0) == 0xE0)
    {
        code_byte = 3;
    }
    else if((p_data[0] & 0xC0) == 0xC0)
    {
        code_byte = 2;
    }
    else if((p_data[0] & 0x80)== 0x00 )
    {
        code_byte = 1;
    }

    return code_byte;
}
static uint8_t Font_CodeByteNum16(uint16_t *p_data)
{
    uint8_t code_byte = 1;

    /* UTF8���� */
    //YX.20180514.��ע�����޷���ʾ������ʱ����ȡ��������Ϣ�������ʾ����UI�����֣���BUG��2577��
    if((0xF0 & p_data[0]) == 0xF0)
    {
        code_byte = 4;
    }
    else if((p_data[0] & 0xE0) == 0xE0)
    {
        code_byte = 3;
    }
    else if((p_data[0] & 0xC0) == 0xC0)
    {
        code_byte = 2;
    }
    else if((p_data[0] & 0x80)== 0x00 )
    {
        code_byte = 1;
    }

    return code_byte;
}
//
/*******************************************************************************
** Function name:	    Font_IsDispSuc
** Descriptions:        �жϵ�ǰ�����Ƿ��������Ļ����ʾ����
** input parameters:    p_data����������
** output parameters:   ��
** Returned value:      ��
** Created by:			WJ             
** Created Date:        2017-07-21
**------------------------------------------------------------------------------
** Modified by:
** Modified date:
**------------------------------------------------------------------------------
*******************************************************************************/
static bool Font_IsDispSuc(uint8_t *p_data)
{
    bool Result = true;
    
    FONT_TYPE_ID_E font_type = Font_GetType(p_data);
    switch(font_type)
    {
        case FONT_ASCII_TYPE_ID:
        case FONT_LATTIN_TYPE_ID:
        case FONT_CHINESE_TYPE_ID:
        case FONT_KOREAN_TYPE_ID:
        case FONT_JAPANESE_TYPE_ID:
        case FONT_CYRILLIC_TYPE_ID:    
            break;
        default:
            Result = false;
            break;
    }
    return Result;
}

/*******************************************************************************
** Function name:	    Font_TranslateLattice
** Descriptions:        ʶ���������Ͳ�ת������ʾ
** input parameters:    p_data��������������:page:��Ļ��ʼ�У�column:��ʼ��
** output parameters:   ��
** Returned value:      ��
** Created by:			WJ             
** Created Date:        2017-07-21
**------------------------------------------------------------------------------
** Modified by:
** Modified date:
**------------------------------------------------------------------------------
*******************************************************************************/
static bool Font_TranslateLattice(uint8_t *p_data, uint8_t page, uint8_t column)
{
    bool bRet = true;
    uint8_t code[32] = {0};
    uint8_t width = 0;
    FONT_TYPE_ID_E font_type = Font_GetType(p_data);
//    uint8_t rotate_code[32] = {0};  

    switch(font_type)
    {
        case FONT_ASCII_TYPE_ID:
        {
            Font_AsciiLattice(p_data[0], code);
            width = 8;
            break;
        }
        case FONT_CHINESE_TYPE_ID:
        {
            bRet = Font_ChineseLattice(p_data,code);
            width = 16;
            break;
        }
        case FONT_KOREAN_TYPE_ID:
        {
            Font_KoreanLattice(p_data,code);  
            width = 16;
            break;
        }
        case FONT_JAPANESE_TYPE_ID:
        {
            Font_JapaneseLattice(p_data,code);  
            width = 16;
            break;
        }
        case FONT_CYRILLIC_TYPE_ID:
        {
            Font_CyrillicLattice(p_data,code);  
            width = 8;
            break;
        }
        case FONT_LATTIN_TYPE_ID:
        {
            Font_LattinLattice(p_data,code);  
            width = 8;
            break;
        }
        default:
            bRet = false;
            break;
    }

    if(true == bRet)
    {
        /* �������ݿ��������ݻ����� */    
//        page -= width / 8;       
//        Font_ImageLeftRotate((void*)code, rotate_code, 16, width);
        if(column + width > 80)
        {
            page += 16 + 5;
            column = LCD_FONT_INTERVAL;
        } 
        //WJ.20171229.������ʾ��Χ        
        if(page > (160 - 16))
        {
            return bRet;
        }            
        if(width == 16)
        {
            GUIFont_16X16FontChangeToColorScreen(column, page, code, LCD_COLOR_WHITE, LCD_COLOR_BLACK);   
        }
        else
        {
            Font_8X16FontChangeToColorScreen(column, page, code, LCD_COLOR_WHITE, LCD_COLOR_BLACK);               
        }       
    }

    return bRet;
}

/*******************************************************************************
** Function name:	    Font_DisplayTitleText
** Descriptions:        ��ʾ�ֿ�����
** input parameters:    p_content���������飻content_len:���ݳ��ȣ�
                        start_page����ʼ��ʾ����
                        total_rows����ʾ����
** output parameters:   ��
** Returned value:      ��
** Created by:			WJ             
** Created Date:        2017-07-21
**------------------------------------------------------------------------------
** Modified by:
** Modified date:
**------------------------------------------------------------------------------
*******************************************************************************/
void Font_DisplayTitleText(uint8_t *p_content, uint16_t content_len, uint8_t start_page, uint8_t total_rows)
{       
    uint8_t page = 0;
    uint8_t pos = 0;     
    uint8_t max_width = 0;
    uint8_t font_num = 0;
    uint16_t i = 0;
    uint16_t max_len = 0;  
    uint8_t rows_cnt = 0;
    
    if(p_content == NULL || content_len == 0) 
    {
        return;
    }   
    max_len = content_len;   
    
    max_width = Font_FullScreenTotalWidth(p_content, max_len, &font_num);
    //��ʼҳ   
    page = start_page;   
    if((max_width * 8 + (font_num - 1) * LCD_FONT_INTERVAL) <= 80)
    {
        //��ʼ��
        pos = (80 - (max_width * 8 + (font_num - 1) * LCD_FONT_INTERVAL)) / 2;
    }
    else
    { 
        //��ʼ��
        pos = LCD_FONT_INTERVAL;   
    }
    
    /* �����ֿ�оƬ */
    Font_Wakeup();
    
    for(i = 0; (i+Font_CodeByteNum(&p_content[i])) <= max_len; i += Font_CodeByteNum(&p_content[i]))
    {
        if((p_content[i] == 0x00) || (page >= (160 - 16) && pos > (80 - Font_FontWidth(&p_content[i])))
          || page > (160 - 16) )
        {
            break;
        }
        //WJ.20171229.��ʾԽ������
        if(!Font_IsDispSuc(&p_content[i]))
        {
            continue;
        }        
        
        if(pos + Font_FontWidth(&p_content[i]) > 80)
        {
            page += 16 + 5;
            pos = LCD_FONT_INTERVAL;
            if(page > (160 - 16))
            {
                break;
            }
        }
        /* ������Ҫ�ж����Ƿ���ʶ�� */
        if(Font_TranslateLattice(&p_content[i], page, pos))
        {
            pos += Font_FontWidth(&p_content[i]);
            if(pos == 80)
            {
                page += 16 + 5;
                pos = LCD_FONT_INTERVAL;
                rows_cnt++;
            }
            else if(pos > 80)
            {
                page += 16 + 5;
                pos = Font_FontWidth(&p_content[i]);
                rows_cnt++;
            }
            if(rows_cnt >= total_rows)
            {
                break;
            }
        }             
    }
     
    Font_Sleep();
}

/*******************************************************************************
** Function name:	    Font_DisplayContentText
** Descriptions:        ��ʾ�ֿ�����
** input parameters:    p_content���������飻content_len:���ݳ��ȣ�
                        start_page����ʼ��ʾ����
                        total_rows����ʾ����
** output parameters:   ��
** Returned value:      ��
** Created by:			WJ             
** Created Date:        2017-07-21
**------------------------------------------------------------------------------
** Modified by:
** Modified date:
**------------------------------------------------------------------------------
*******************************************************************************/
uint8_t Font_DisplayContentText(uint8_t *p_content, uint16_t content_len, uint8_t start_page, uint8_t total_rows)
{
    uint8_t read_len = 0;
    uint16_t i = 0;
    uint16_t max_len = 0;   
  
    uint8_t page = 0;
    uint8_t pos = 0;     
 
    if(p_content == NULL || content_len == 0) 
    {
        return 0;
    }
    
    max_len = content_len;
    
    page = start_page;   //��ʼҳ
    pos = LCD_FONT_INTERVAL;    //��ʼ��
    
    /* �����ֿ�оƬ */
    Font_Wakeup();
    
    for(i = 0; (i+Font_CodeByteNum(&p_content[i])) <= max_len; i += Font_CodeByteNum(&p_content[i]))
    {
        if((p_content[i] == 0x00) || (page >= (160 - 16) && pos > (80 - Font_FontWidth(&p_content[i])))
          || page > (160 - 16) )
        {
            break;
        }
         
        //WJ.20171229.��ʾԽ������
        if(!Font_IsDispSuc(&p_content[i]))
        {
            continue;
        }        
        
        if(pos + Font_FontWidth(&p_content[i]) > 80)
        {
            page += 16 + 5;
            pos = LCD_FONT_INTERVAL;
            if(page > (160 - 16))
            {
                break;
            }
        }
        
        /* ������Ҫ�ж����Ƿ���ʶ�� */
        if(Font_TranslateLattice(&p_content[i], page, pos))
        {
            pos += Font_FontWidth(&p_content[i]);
            if(pos == 80)
            {
                page += 16 + 5;
                pos = LCD_FONT_INTERVAL;
            }
            else if(pos > 80)
            {
                page += 16 + 5;
                pos = Font_FontWidth(&p_content[i]);
            }
        }
        read_len += Font_CodeByteNum(&p_content[i]);
    }
     
    Font_Sleep();
    
    return read_len;
}

/*******************************************************************************
** Function name:	    Font_DisContentText
** Descriptions:        ��ʾ�ֿ����ݼӼ��
** input parameters:    p_content���������飻content_len:���ݳ��ȣ�
                        start_page����ʼ��ʾ����
                        total_rows����ʾ����
                        x_interval:������
                        y_interval��������
** output parameters:   ��
** Returned value:      ��
** Created by:			WJ             
** Created Date:        2017-07-21
**------------------------------------------------------------------------------
** Modified by:
** Modified date:
**------------------------------------------------------------------------------
*******************************************************************************/
void Font_DisContentText(uint8_t *p_content, uint16_t content_len, uint8_t start_page, uint8_t total_rows, uint8_t x_interval, uint8_t y_interval)
{
    uint16_t i = 0;
    uint16_t max_len = 0;   
  
    uint8_t page = 0;
    uint8_t pos = 0;     
 
    if(p_content == NULL || content_len == 0) 
    {
        return;
    }
    
    max_len = content_len;
    
    page = start_page;   //��ʼҳ
    pos = 0;//LCD_FONT_INTERVAL;    //��ʼ��
    
    /* �����ֿ�оƬ */
    Font_Wakeup();
    
    for(i = 0; (i+Font_CodeByteNum(&p_content[i])) <= max_len; i += Font_CodeByteNum(&p_content[i]))
    {
        if((p_content[i] == 0x00) || (page >= (160 - 16) && pos > (80 - Font_FontWidth(&p_content[i])))
          || page > (160 - 16) )
        {
            break;
        }
         
        //WJ.20171229.��ʾԽ������
        if(!Font_IsDispSuc(&p_content[i]))
        {
            continue;
        }        
        
        if(pos + Font_FontWidth(&p_content[i]) > 80)
        {
            page += 16 + 5;
            pos = LCD_FONT_INTERVAL;
            if(page > (160 - 16))
            {
                break;
            }
        }
        
        /* ������Ҫ�ж����Ƿ���ʶ�� */
        if(Font_TranslateLattice(&p_content[i], page, pos))
        {
            pos += Font_FontWidth(&p_content[i]) + x_interval;
            if(pos == 80)
            {
                page += 16 + y_interval;
                pos = LCD_FONT_INTERVAL;
            }
            else if(pos > 80)
            {
                page += 16 + y_interval;
                pos = Font_FontWidth(&p_content[i]) + x_interval;
            }
        }             
    }
     
    Font_Sleep();
}

/*******************************************************************************
** Function name:	    Font_FullScreenTotalActiveLenght
** Descriptions:        ʶ���������ͷ���ռλ���
** input parameters:    p_title���������飻title_len������
** output parameters:   ��
** Returned value:      ��Ϣ���ݵ���Ч����
** Created by:			WJ             
** Created Date:        2017-07-21
**------------------------------------------------------------------------------
** Modified by:
** Modified date:
**------------------------------------------------------------------------------
*******************************************************************************/
uint16_t Font_FullScreenTotalActiveLenght(uint8_t *p_title, uint16_t title_len)
{
//    uint8_t total_width = 0x00;
    uint16_t i = 0;
    
    if(p_title == NULL || title_len == 0) 
    {
        return 0;
    }
    
    for(i = 0; i < title_len; i += Font_CodeByteNum(&p_title[i]))
    {
        if( p_title[i] == 0x00 || (i + Font_CodeByteNum(&p_title[i]) > title_len))
        {
            break;
        }        
    }    
     
    return i;
}

/*******************************************************************************
** Function name:	    Font_FullScreenTotalWidth
** Descriptions:        ʶ������ռλ���
** input parameters:    p_title���������飻title_len������
** output parameters:   ��
** Returned value:      ��
** Created by:			WJ             
** Created Date:        2017-07-21
**------------------------------------------------------------------------------
** Modified by:
** Modified date:
**------------------------------------------------------------------------------
*******************************************************************************/
uint8_t Font_FullScreenTotalWidth(uint8_t *p_title, uint16_t title_len, uint8_t *font_num)
{
    uint8_t total_width = 0x00;
    uint16_t i = 0;
    
    *font_num = 0;
    if(p_title == NULL || title_len == 0)
    {       
        return 0;
    }
    
    for(i = 0; i < title_len; i += Font_CodeByteNum(&p_title[i]))
    {
        if(p_title[i] == 0x00 || (i + Font_CodeByteNum(&p_title[i]) > title_len))
        {
            break;
        }
        if(Font_FontWidth(&p_title[i]) == 8)
        {
            total_width += 1;
            font_num++;
        }
        else
        {
            total_width += 2;
            font_num++;
        }
    }        
    return total_width;
}

/*******************************************************************************
** Function name:	    Font_FullScreenTotalCount
** Descriptions:        ������Ϣ���ݷ�ҳ��Ϣ
** input parameters:    p_title���������ݣ�title_len�����ⳤ��
                        p_content�����ݣ�content_len�����ݳ���                        
** output parameters:   start_page_index����ҳ��ʼ���������±�
** Returned value:      �����ܹ��ж���ҳ��Ϣ
** Created by:			WJ             
** Created Date:        2017-07-21
**------------------------------------------------------------------------------
** Modified by:
** Modified date:
**------------------------------------------------------------------------------
*******************************************************************************/
uint8_t Font_FullScreenTotalCount(uint8_t *p_title, uint16_t title_len,
                                 uint8_t *p_content, uint16_t content_len, uint16_t *start_page_index)
{
    uint8_t total_page = 0x01;
    uint8_t total_rows = 0x01;    
    uint16_t content_len_max = 0;
    uint16_t i = 0;  
    uint8_t start_pos = LCD_FONT_INTERVAL;
    
    if(p_content == NULL || content_len == 0)
    {
        return 0;
    }
    
    content_len_max = content_len;
    if(content_len_max > CONTENT_DATA_MAX_LEN)
    {
        content_len_max = CONTENT_DATA_MAX_LEN;
    }
   
    for(i = 0; i < content_len_max; i += Font_CodeByteNum(&p_content[i]))
    {
        if(p_content[i] == 0x00)
        {
            break;
        }
        if(Font_IsDispSuc(&p_content[i]))
        {
            //20170626.YX �޸������ַ���ʾ������Ļ��䲻��������
            if((start_pos + Font_FontWidth(&p_content[i])) > 80)
            {
                total_rows++;
                start_pos = LCD_FONT_INTERVAL + Font_FontWidth(&p_content[i]);
                if((total_rows >  FIRST_PAGE_ROWS) && ((total_rows - FIRST_PAGE_ROWS) % PER_PAGE_MAX_ROWS == 1)
                  && (total_page < MAX_PAGE_LEN))
                {
                    start_page_index[total_page++] = i;// - Font_CodeByteNum(&p_content[i]);
                }                
            }
            else
            {
                start_pos += Font_FontWidth(&p_content[i]) + LCD_FONT_INTERVAL;
            }
        }
    }
    if( i > 0)
    {
        if(i > content_len_max)
        {
            i = content_len_max;
        }
        start_page_index[total_page] = i;
    }
    
    return total_page;
}


/*******************************************************************************
** Function name:	    Font_FullScreenTotalCount
** Descriptions:        ������Ϣ���ݷ�ҳ��Ϣ
** input parameters:    p_content�����ݣ�content_len�����ݳ���                        
** output parameters:   start_page_index����ҳ��ʼ���������±�
** Returned value:      �����ܹ��ж���ҳ��Ϣ
** Created by:			WJ             
** Created Date:        2017-07-21
**------------------------------------------------------------------------------
** Modified by:
** Modified date:
**------------------------------------------------------------------------------
*******************************************************************************/
uint8_t Font_FullScreenTotalCountByStr(uint8_t *p_content, uint16_t content_len, uint16_t *start_page_index)
{
    uint8_t total_page = 0x01;
    uint8_t total_rows = 0x01;    
    uint16_t content_len_max = 0;
    uint16_t i = 0;  
    uint8_t start_pos = LCD_FONT_INTERVAL;
    
    if(p_content == NULL || content_len == 0)
    {
        return 0;
    }
    
    content_len_max = content_len;
    if(content_len_max > CONTENT_DATA_MAX_LEN)
    {
        content_len_max = CONTENT_DATA_MAX_LEN;
    }
   
    for(i = 0; i < content_len_max; i += Font_CodeByteNum(&p_content[i]))
    {
        if(p_content[i] == 0x00)
        {
            break;
        }
        if(Font_IsDispSuc(&p_content[i]))
        {
            //20170626.YX �޸������ַ���ʾ������Ļ��䲻��������
            if((start_pos + Font_FontWidth(&p_content[i])) > 80)
            {
                total_rows++;
                start_pos = LCD_FONT_INTERVAL + Font_FontWidth(&p_content[i]);
                //if((total_rows % PER_PAGE_MAX_ROWS == 1) && (total_page < MAX_PAGE_LEN))
                if((total_rows >  FIRST_PAGE_ROWS) && ((total_rows - FIRST_PAGE_ROWS) % PER_PAGE_MAX_ROWS == 1)
                  && (total_page < MAX_PAGE_LEN))
                {
                    start_page_index[total_page++] = i;
                }                
            }
            else
            {
                start_pos += Font_FontWidth(&p_content[i]) + LCD_FONT_INTERVAL;
            }
        }
    }
    if( i > 0)
    {
        if(i > content_len_max)
        {
            i = content_len_max;
        }
        start_page_index[total_page] = i;
    }
    
    return total_page;
}

