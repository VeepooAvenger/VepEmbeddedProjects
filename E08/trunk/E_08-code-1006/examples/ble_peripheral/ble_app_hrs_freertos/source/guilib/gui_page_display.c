/***************************Copyright(c)***********************************
**                       Veepoo Tech
**                  COPYRIGHT (C) 2016-2019
**-----------File Info-----------------------------------------------------
** File        name:     gui_page_display.c
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
#include "gui_page_display.h"
#include "gui_data_inteface.h"
#include "lcd_driver_tft.h"
#include "gui_font_lattice.h"
#include "tft_icon_const.h"
#include "ext_flash.h"
#include "gui_flash_addr.h"
#include "gui_flash_param.h"
#include "configuration.h"
#include "util.h"
#include "common_data_type.h"
#include "backlight_driving.h"
#include "FreeRTOS.h"
#include "task.h"

//ˢ�µ�ʱ����ʱһ��ʱ��30ms
const uint8_t disp_delay_time = 1;//20;

extern uint8_t ui_style_type; 

// �������Ա���
static uint8_t language_save = 0xff;
// ���湫Ӣ�Ʊ���
static uint8_t unit_type_save = 0xff;

// ����֡�����������ڶ�����ͣ����
static uint16_t animation_cnt = 0;

extern uint8_t display_language; 
extern uint8_t lcd_backlight_level;
/****************************************************************
** Function name:           Gui_LcdDriverInit
** Descriptions:            LCD��ʼ��
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              YX              
** Created Date:            2017-08-07
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Gui_LcdDriverInit(void)
{ 	
    LCD_DriverInit();   //������ʼ��
}

/****************************************************************
** Function name:           Gui_LcdDriverOn
** Descriptions:            LCD����Ļ
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              YX              
** Created Date:            2017-08-07
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Gui_LcdDriverOn(void)
{
    LCD_OpenScreen();
}

/****************************************************************
** Function name:           Gui_LcdDriverOff
** Descriptions:            LCD�ر���Ļ
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              YX              
** Created Date:            2017-08-07
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Gui_LcdDriverOff(void)
{
    LCD_CloseScreen();
}


/****************************************************************
** Function name:           Gui_LcdLedOff
** Descriptions:            LCD�ر���Ļ����
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2018-02-05
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Gui_LcdLedOff(void)
{
    LCD_CloseLed();
}

/****************************************************************
** Function name:           Gui_LcdLedOn
** Descriptions:            LCD����Ļ����
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2018-02-05
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Gui_LcdLedOn(void)
{
//    vTaskDelay(disp_delay_time);
    Backlight_SetLeve(lcd_backlight_level);
//    vTaskDelay(30);
} 

/****************************************************************
** Function name:           Gui_LcdWakeUp
** Descriptions:            LCD����
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2018-03-03
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Gui_LcdWakeUp(void)
{
    LCD_WakeUp();
}

/****************************************************************
** Function name:           Gui_PageClearBackground
** Descriptions:            ��������
** input parameters:        s_column����������
**                          s_page����������
**                          column_size�������С
**                          page_size�������С
** output parameters:       ��
** Returned value:          ��
** Created by:              YX              
** Created Date:            2017-08-07
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Gui_PageClearBackground(uint8_t s_column, uint8_t s_page, uint8_t column_size, uint8_t page_size)
{   
    //WJ.20180208.��ֹԽ�紦��
    if(s_column >= LCD_MAX_COLUMN ||  s_page >= LCD_MAX_ROW
      || column_size > LCD_MAX_COLUMN || page_size > LCD_MAX_ROW)
    {
        return;
    }
    
    if(column_size * page_size == 0)
    {
        return ;
    }
    
    //��������
    LCD_OpenScreenByPosition(s_column, s_page, column_size, page_size); 
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
void Gui_PageRefreshBackground(uint8_t s_column, uint8_t s_page, uint8_t column_size, uint8_t page_size, uint16_t color)
{     
    //WJ.20180427.��ֹԽ�紦��
    if(s_column >= LCD_MAX_COLUMN ||  s_page >= LCD_MAX_ROW
      || column_size > LCD_MAX_COLUMN || page_size > LCD_MAX_ROW)
    {
        return;
    }
    if(page_size * column_size == 0)
    {
        return;
    } 
    /*����ˢ��Ϊ������ɫ*/
    LCD_SetColorScreenByPosition(s_column, s_page, column_size, page_size, color); 
}

/****************************************************************
** Function name:           Gui_DisDbugUi
** Descriptions:            ��ʾUI���Խ���
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              YX              
** Created Date:            2017-09-06
*****************************************************************/  
void Gui_DisDbugUi(void)
{
//    LCD_SetPositionInit();
//    LCD_DriverWriteDataFromExtFlash(LCD_MAX_ROW * LCD_MAX_COLUMN * 2 * dbug_dis_ui_num, 
//                                    LCD_MAX_ROW * LCD_MAX_COLUMN * 2);
//    if(dbug_dis_ui_flg == 1)
//    {
//        if(++dbug_dis_ui_num >= TEST_DBUG_UI_MAX_NUM)
//        {
//            dbug_dis_ui_num = 0;
//        }    
//    }    
}

/****************************************************************
** Function name:           Gui_DisXIcon
** Descriptions:            ��ʾX
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              YX              
** Created Date:            2017-08-29
*****************************************************************/  
void Gui_DisXIcon(void)
{
    uint8_t dis_x_data[64] = {0};
    for(uint8_t i = 0; i < 32; i++)
    {
        for(uint8_t j = 0; j < 32; j++)
        {
            if(i == j || i + j == 32)
            {
                dis_x_data[j * 2] = 0xFF;
                dis_x_data[j * 2 + 1] = 0xFF;
            }
            else
            {            
                dis_x_data[j * 2] = 0;
                dis_x_data[j * 2 + 1] = 0;
            }
        }
        LCD_DriverWriteDataBuf(dis_x_data, 64);
    }
    
}

/****************************************************************
** Function name:           Gui_PageDisXxxIcon
** Descriptions:            ��ʾXXX
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              YX              
** Created Date:            2017-08-29
*****************************************************************/     
void Gui_PageDisXxxIcon(void)
{
    LCD_SetPosition(24, 22, 32, 32);
    Gui_DisXIcon();
    LCD_SetPosition(24, 64, 32, 32);
    Gui_DisXIcon();
    LCD_SetPosition(24, 106, 32, 32);
    Gui_DisXIcon();
}

/****************************************************************
** Function name:           Gui_PageClearBigToSmallBackground
** Descriptions:            �����ˢС����ı���
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              YX              
** Created Date:            2018-12-29
*****************************************************************/ 
void Gui_PageClearBigToSmallBackground(void)
{
    
//////    Gui_PageClearBackground(0, 0, 
//////                            icon_bigtosmall_coord[0][0]+icon_bigtosmall_size[0][0], icon_bigtosmall_coord[0][1]);
//////    Gui_PageClearBackground(0, icon_bigtosmall_coord[0][1], 
//////                            icon_bigtosmall_coord[0][0], icon_bigtosmall_size[0][1]);
//////    
//////    Gui_PageClearBackground(icon_bigtosmall_coord[0][0]+icon_bigtosmall_size[0][0], 0, 
//////                            MAX_X_SIZE - (icon_bigtosmall_coord[0][0]+icon_bigtosmall_size[0][0]), icon_bigtosmall_coord[1][1]);
//////    Gui_PageClearBackground(icon_bigtosmall_coord[0][0]+icon_bigtosmall_size[0][0], icon_bigtosmall_coord[1][1], 
//////                            icon_bigtosmall_coord[1][0] - (icon_bigtosmall_coord[0][0]+icon_bigtosmall_size[0][0]),
//////                            icon_bigtosmall_coord[0][1]+icon_bigtosmall_size[0][1] - icon_bigtosmall_coord[1][1]);
//////    Gui_PageClearBackground(icon_bigtosmall_coord[1][0]+icon_bigtosmall_size[1][0], icon_bigtosmall_coord[1][1], 
//////                            MAX_X_SIZE - (icon_bigtosmall_coord[1][0]+icon_bigtosmall_size[1][0]),
//////                            icon_bigtosmall_coord[0][1]+icon_bigtosmall_size[0][1] - icon_bigtosmall_coord[1][1]);
//////    Gui_PageClearBackground(icon_bigtosmall_coord[1][0], icon_bigtosmall_coord[1][1]+icon_bigtosmall_size[1][1], 
//////                            MAX_X_SIZE - (icon_bigtosmall_coord[1][0]),
//////                            icon_bigtosmall_coord[0][1]+icon_bigtosmall_size[0][1] - (icon_bigtosmall_coord[1][1]+icon_bigtosmall_size[1][1]));
//////    
//////    
//////    Gui_PageClearBackground(0, icon_bigtosmall_coord[0][1]+icon_bigtosmall_size[0][1], 
//////                            MAX_X_SIZE, icon_bigtosmall_coord[3][1] - (icon_bigtosmall_coord[0][1]+icon_bigtosmall_size[0][1]));                         
//////    Gui_PageClearBackground(0, icon_bigtosmall_coord[3][1], 
//////                            icon_bigtosmall_coord[3][0], icon_bigtosmall_coord[2][1]-icon_bigtosmall_coord[3][1]);
////////    Gui_PageClearBackground(icon_bigtosmall_coord[3][0]+icon_bigtosmall_size[3][0], icon_bigtosmall_coord[3][1], 
////////                            MAX_X_SIZE-(icon_bigtosmall_coord[3][0]+icon_bigtosmall_size[3][0]), icon_bigtosmall_coord[2][1]-icon_bigtosmall_coord[3][1]);
//////    Gui_PageClearBackground(0, icon_bigtosmall_coord[2][1], 
//////                            icon_bigtosmall_coord[2][0],icon_bigtosmall_size[2][1]);
//////    
//////    Gui_PageClearBackground(icon_bigtosmall_coord[2][0]+icon_bigtosmall_size[2][0],
//////                            icon_bigtosmall_coord[2][1],
//////                            icon_bigtosmall_coord[3][0]-(icon_bigtosmall_coord[2][0]+icon_bigtosmall_size[2][0]),
//////                            icon_bigtosmall_size[2][1]);
//////                            
////////    Gui_PageClearBackground(icon_bigtosmall_coord[3][0]+icon_bigtosmall_size[3][0], icon_bigtosmall_coord[4][1], 
////////                            icon_bigtosmall_coord[4][0]-(icon_bigtosmall_coord[3][0]+icon_bigtosmall_size[3][0]),icon_bigtosmall_size[4][1]);
////////                            
////////    Gui_PageClearBackground(icon_bigtosmall_coord[4][0]+icon_bigtosmall_size[4][0],
////////                            icon_bigtosmall_coord[4][1],
////////                            MAX_X_SIZE-(icon_bigtosmall_coord[4][0]+icon_bigtosmall_size[4][0]),
////////                            icon_bigtosmall_size[4][1]);
//////                            
//////    Gui_PageClearBackground(0, icon_bigtosmall_coord[2][1]+icon_bigtosmall_size[2][1], 
//////                            icon_bigtosmall_coord[3][0],
//////                            icon_bigtosmall_coord[3][1]+icon_bigtosmall_size[3][1]-(icon_bigtosmall_coord[2][1]+icon_bigtosmall_size[2][1]));
//////                            
//////    Gui_PageClearBackground(icon_bigtosmall_coord[3][0]+icon_bigtosmall_size[3][0],
//////                            icon_bigtosmall_coord[3][1], 
//////                            MAX_X_SIZE - (icon_bigtosmall_coord[3][0]+icon_bigtosmall_size[3][0]),
//////                            icon_bigtosmall_size[3][1]);
//////                            
////////    Gui_PageClearBackground(icon_bigtosmall_coord[3][0]+icon_bigtosmall_size[3][0],
////////                            icon_bigtosmall_coord[4][1]+icon_bigtosmall_size[4][1],
////////                            MAX_X_SIZE-(icon_bigtosmall_coord[3][0]+icon_bigtosmall_size[3][0]),
////////                            icon_bigtosmall_coord[3][1]+icon_bigtosmall_size[3][1]-(icon_bigtosmall_coord[4][1]+icon_bigtosmall_size[4][1]));

//////    Gui_PageClearBackground(0,
//////                            icon_bigtosmall_coord[3][1]+icon_bigtosmall_size[3][1],
//////                            MAX_X_SIZE,
//////                            icon_bigtosmall_coord[5][1]-(icon_bigtosmall_coord[3][1]+icon_bigtosmall_size[3][1]));
//////    
//////    Gui_PageClearBackground(0,
//////                            icon_bigtosmall_coord[5][1],
//////                            icon_bigtosmall_coord[5][0],
//////                            icon_bigtosmall_size[5][1]);
//////    Gui_PageClearBackground(icon_bigtosmall_coord[5][0] + icon_bigtosmall_size[5][0],
//////                            icon_bigtosmall_coord[5][1],
//////                            MAX_X_SIZE-(icon_bigtosmall_coord[5][0] + icon_bigtosmall_size[5][0]),
//////                            icon_bigtosmall_size[5][1]);
//////    
//////    Gui_PageClearBackground(0,
//////                            icon_bigtosmall_coord[6][1],
//////                            icon_bigtosmall_coord[6][0],
//////                            MAX_Y_SIZE - icon_bigtosmall_coord[6][1]);                       
//////    Gui_PageClearBackground(icon_bigtosmall_coord[8][0] + icon_bigtosmall_size[8][0],
//////                            icon_bigtosmall_coord[8][1],
//////                            MAX_X_SIZE - (icon_bigtosmall_coord[8][0] + icon_bigtosmall_size[8][0]),
//////                            MAX_Y_SIZE - icon_bigtosmall_coord[8][1]);
//////                            
//////    Gui_PageClearBackground(icon_bigtosmall_coord[6][0] + icon_bigtosmall_size[6][0],
//////                            icon_bigtosmall_coord[6][1],
//////                            icon_bigtosmall_coord[7][0]-(icon_bigtosmall_coord[6][0] + icon_bigtosmall_size[6][0]),
//////                            MAX_Y_SIZE - icon_bigtosmall_coord[6][1]);
//////                            
//////    Gui_PageClearBackground(icon_bigtosmall_coord[6][0] + icon_bigtosmall_size[6][0],
//////                            icon_bigtosmall_coord[6][1],
//////                            icon_bigtosmall_coord[9][0]-(icon_bigtosmall_coord[6][0] + icon_bigtosmall_size[6][0]),
//////                            MAX_Y_SIZE - icon_bigtosmall_coord[6][1]);
//////                            
//////    Gui_PageClearBackground(icon_bigtosmall_coord[9][0] + icon_bigtosmall_size[9][0],
//////                            icon_bigtosmall_coord[6][1],
//////                            icon_bigtosmall_coord[8][0]-(icon_bigtosmall_coord[9][0] + icon_bigtosmall_size[9][0]),
//////                            MAX_Y_SIZE - icon_bigtosmall_coord[6][1]);
//////                            
//////    Gui_PageClearBackground(icon_bigtosmall_coord[6][0],
//////                            icon_bigtosmall_coord[6][1]+icon_bigtosmall_size[6][1],
//////                            icon_bigtosmall_size[6][0],
//////                            MAX_Y_SIZE - (icon_bigtosmall_coord[6][1]+icon_bigtosmall_size[6][1])); 
//////    Gui_PageClearBackground(icon_bigtosmall_coord[8][0],
//////                            icon_bigtosmall_coord[8][1]+icon_bigtosmall_size[8][1],
//////                            icon_bigtosmall_size[8][0],
//////                            MAX_Y_SIZE - (icon_bigtosmall_coord[8][1]+icon_bigtosmall_size[8][1]));


//////    Gui_PageClearBackground(icon_bigtosmall_coord[9][0],
//////                            icon_bigtosmall_coord[5][1]+icon_bigtosmall_size[5][1],
//////                            icon_bigtosmall_size[9][0],
//////                            icon_bigtosmall_coord[7][1] - (icon_bigtosmall_coord[5][1]+icon_bigtosmall_size[5][1])); 
//////    Gui_PageClearBackground(icon_bigtosmall_coord[9][0],
//////                            icon_bigtosmall_coord[7][1]+icon_bigtosmall_size[7][1],
//////                            icon_bigtosmall_size[9][0],
//////                            icon_bigtosmall_coord[9][1] - (icon_bigtosmall_coord[7][1]+icon_bigtosmall_size[7][1]));
//////    
//////    Gui_PageClearBackground(icon_bigtosmall_coord[9][0],
//////                            icon_bigtosmall_coord[9][1]+icon_bigtosmall_size[9][1],
//////                            icon_bigtosmall_size[9][0],
//////                            MAX_Y_SIZE - (icon_bigtosmall_coord[9][1]+icon_bigtosmall_size[9][1]));
}

/****************************************************************
** Function name:           Gui_PageClearBigToSmallArea
** Descriptions:            �����ˢС�������ʾ����
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              YX              
** Created Date:            2018-12-29
*****************************************************************/ 
void Gui_PageClearBigToSmallArea(void)
{
//////    for(uint8_t i = 0; i < 10; i++)
//////    {
//////        Gui_PageClearBackground(icon_bigtosmall_coord[i][0], icon_bigtosmall_coord[i][1],
//////                                icon_bigtosmall_size[i][0], icon_bigtosmall_size[i][1]);
//////    }
}

/****************************************************************
** Function name:           Gui_PageMainAssistant
** Descriptions:            ��ʾ�������ʱ�Ӻ�����
** input parameters:        month����
**                          day����
**                          hour��ʱ
**                          minute������
**                          week������
**                          language������
**                          am_pm_format��AM,PM��״̬
** output parameters:       ��
** Returned value:          ��
** Created by:              YX              
** Created Date:            2017-08-07
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/     
static void Gui_PageMainAssistant(uint16_t index, uint8_t week, uint8_t language, uint16_t am_pm, 
                                    uint8_t ble_status, uint8_t charge, uint8_t battery_level, 
                                    uint8_t menstrual, uint8_t y_coord)
{
    if(language)
    {
        language = 1;
    }
    
    
    // ����
    LCD_SetPosition(main3_main1_icon3_coord[0][0], main3_main1_icon3_coord[0][1],
                    ICON_MAIN1_ICON3_WIDE, ICON_MAIN1_ICON3_HIGH);
    LCD_DriverWriteDataFromExtFlash(main3_main1_icon3_addr[ble_status % 2], ICON_MAIN1_ICON3_SIZE);

    // ������
    LCD_SetPosition(main3_main1_icon1_coord[0][0], main3_main1_icon1_coord[0][1], 
                    ICON_MAIN1_ICON1_WIDE, ICON_MAIN1_ICON1_HIGH);
    if(am_pm == GUI_TIME_FORMAT_AM)
    {
        if(language == 0)
        {
            LCD_DriverWriteDataFromExtFlash(main3_main1_icon1_addr[1], ICON_MAIN1_ICON1_SIZE);
        }
        else
        {
            LCD_DriverWriteDataFromExtFlash(main3_main1_icon1_addr[3], ICON_MAIN1_ICON1_SIZE);
        }
    }
    else if(am_pm == GUI_TIME_FORMAT_PM)
    {
        if(language == 0)
        {
            LCD_DriverWriteDataFromExtFlash(main3_main1_icon1_addr[0], ICON_MAIN1_ICON1_SIZE);
        }
        else
        {
            LCD_DriverWriteDataFromExtFlash(main3_main1_icon1_addr[2], ICON_MAIN1_ICON1_SIZE);
        }
    }
    else
    { 
        //��Ҫ�滻
        Gui_PageClearBackground(main3_main1_icon1_coord[0][0], main3_main1_icon1_coord[0][1], 
                                ICON_MAIN1_ICON1_WIDE, ICON_MAIN1_ICON1_HIGH);
    }
    
    // ����
    if(week)
    {
        week -= 1;
        week %= 7;
    }
    LCD_SetPosition(main3_main1_week_coord[0][0], main3_main1_week_coord[0][1],
                    ICON_MAIN1_WEEK_WIDE, ICON_MAIN1_WEEK_HIGH);
    LCD_DriverWriteDataFromExtFlash(main3_main1_week_addr[language %2 ][week], ICON_MAIN1_WEEK_SIZE);

////    //Ů��
////    LCD_SetPosition(icon_16_main1_menstrual_coord[0][0], icon_16_main1_menstrual_coord[0][1], 
////                    ICON_SHARE2_WEMON_WIDE, ICON_SHARE2_WEMON_HIGH);
////    LCD_DriverWriteDataFromExtFlash(icon_16_main1_menstrual_addr[0][menstrual % 5], ICON_SHARE2_WEMON_SIZE);
        
    // ���
    if(charge == BATTERRY_IS_CHARGING)
    {
        // ����� 
        LCD_SetPosition(main3_main1_icon2_coord[0][0], main3_main1_icon2_coord[0][1],
                        ICON_MAIN1_ICON2_WIDE, ICON_MAIN1_ICON2_HIGH);
        LCD_DriverWriteDataFromExtFlash(main3_main1_icon2_addr[index % 5], ICON_MAIN1_ICON2_SIZE);

    }
    else
    {
        if(battery_level == 0)
        {
            if((index % 4 == 1)||(index % 4 == 3))
            {
                // ��ؿ�
                LCD_SetPosition(main3_main1_icon2_coord[0][0], main3_main1_icon2_coord[0][1],
                                ICON_MAIN1_ICON2_WIDE, ICON_MAIN1_ICON2_HIGH);
                LCD_DriverWriteDataFromExtFlash(main3_main1_icon2_addr[4], ICON_MAIN1_ICON2_SIZE);
            }
            else if((index % 4 == 0)||(index % 4 == 2))
            {
                // ��ؿ�
                LCD_SetPosition(main3_main1_icon2_coord[0][0], main3_main1_icon2_coord[0][1],
                                ICON_MAIN1_ICON2_WIDE, ICON_MAIN1_ICON2_HIGH);
                LCD_DriverWriteDataFromExtFlash(main3_main1_icon2_addr[0], ICON_MAIN1_ICON2_SIZE);
            }
        }
        else
        {
            // ������� --- buff��Ҫ����˳��
            battery_level %= 5;
            
            LCD_SetPosition(main3_main1_icon2_coord[0][0], main3_main1_icon2_coord[0][1],
                            ICON_MAIN1_ICON2_WIDE, ICON_MAIN1_ICON2_HIGH);
            LCD_DriverWriteDataFromExtFlash(main3_main1_icon2_addr[battery_level], ICON_MAIN1_ICON2_SIZE);
        }
    }
}



static void Gui_PageMainAssistant1(uint16_t index, uint8_t week, uint8_t language, uint16_t am_pm, 
                                    uint8_t ble_status, uint8_t charge, uint8_t battery_level, 
                                    uint8_t menstrual, uint8_t y_coord)
{
    if(language)
    {
        language = 1;
    }
    
    
    // ����
    LCD_SetPosition(main4_main1_icon3_coord[0][0], main4_main1_icon3_coord[0][1],
                    ICON_MAIN1_ICON3_WIDE, ICON_MAIN1_ICON3_HIGH);
    LCD_DriverWriteDataFromExtFlash(main4_main1_icon3_addr[ble_status % 2], ICON_MAIN1_ICON3_SIZE);

    // ������
    LCD_SetPosition(main4_main1_icon1_coord[0][0], main4_main1_icon1_coord[0][1], 
                    ICON_MAIN1_ICON1_WIDE, ICON_MAIN1_ICON1_HIGH);
    if(am_pm == GUI_TIME_FORMAT_AM)
    {
        if(language == 0)
        {
            LCD_DriverWriteDataFromExtFlash(main4_main1_icon1_addr[1], ICON_MAIN1_ICON1_SIZE);
        }
        else
        {
            LCD_DriverWriteDataFromExtFlash(main4_main1_icon1_addr[3], ICON_MAIN1_ICON1_SIZE);
        }
    }
    else if(am_pm == GUI_TIME_FORMAT_PM)
    {
        if(language == 0)
        {
            LCD_DriverWriteDataFromExtFlash(main4_main1_icon1_addr[0], ICON_MAIN1_ICON1_SIZE);
        }
        else
        {
            LCD_DriverWriteDataFromExtFlash(main4_main1_icon1_addr[2], ICON_MAIN1_ICON1_SIZE);
        }
    }
    else
    { 
        //��Ҫ�滻
        Gui_PageClearBackground(main4_main1_icon1_coord[0][0], main4_main1_icon1_coord[0][1], 
                                ICON_MAIN1_ICON1_WIDE, ICON_MAIN1_ICON1_HIGH);
    }
    
    // ����
    if(week)
    {
        week -= 1;
        week %= 7;
    }
    LCD_SetPosition(main4_main1_week_coord[0][0], main4_main1_week_coord[0][1],
                    ICON_MAIN1_WEEK_WIDE, ICON_MAIN1_WEEK_HIGH);
    LCD_DriverWriteDataFromExtFlash(main4_main1_week_addr[language%2][week], ICON_MAIN1_WEEK_SIZE);

////    //Ů��
////    LCD_SetPosition(icon_16_main1_menstrual_coord[0][0], icon_16_main1_menstrual_coord[0][1], 
////                    ICON_SHARE2_WEMON_WIDE, ICON_SHARE2_WEMON_HIGH);
////    LCD_DriverWriteDataFromExtFlash(icon_16_main1_menstrual_addr[0][menstrual % 5], ICON_SHARE2_WEMON_SIZE);
        
    // ���
    if(charge == BATTERRY_IS_CHARGING)
    {
        // ����� 
        LCD_SetPosition(main4_main1_icon2_coord[0][0], main4_main1_icon2_coord[0][1],
                        ICON_MAIN1_ICON2_WIDE, ICON_MAIN1_WEEK_HIGH);
        LCD_DriverWriteDataFromExtFlash(main4_main1_icon2_addr[index % 5], ICON_MAIN1_ICON2_SIZE);

    }
    else
    {
        if(battery_level == 0)
        {
            if((index % 4 == 1)||(index % 4 == 3))
            {
                // ��ؿ�
                LCD_SetPosition(main4_main1_icon2_coord[0][0], main4_main1_icon2_coord[0][1],
                                ICON_MAIN1_ICON2_WIDE, ICON_MAIN1_ICON2_HIGH);
                LCD_DriverWriteDataFromExtFlash(main4_main1_icon2_addr[4], ICON_MAIN1_ICON2_SIZE);
            }
            else if((index % 4 == 0)||(index % 4 == 2))
            {
                // ��ؿ�
                LCD_SetPosition(main4_main1_icon2_coord[0][0], main4_main1_icon2_coord[0][1],
                                ICON_MAIN1_ICON2_WIDE, ICON_MAIN1_ICON2_HIGH);
                LCD_DriverWriteDataFromExtFlash(main4_main1_icon2_addr[0], ICON_MAIN1_ICON2_SIZE);
            }
        }
        else
        {
            // ������� --- buff��Ҫ����˳��
            battery_level %= 5;
            
            LCD_SetPosition(main4_main1_icon2_coord[0][0], main4_main1_icon2_coord[0][1],
                            ICON_MAIN1_ICON2_WIDE, ICON_MAIN1_ICON2_HIGH);
            LCD_DriverWriteDataFromExtFlash(main4_main1_icon2_addr[battery_level], ICON_MAIN1_ICON2_SIZE);
        }
    }
}

/****************************************************************
** Function name:           Gui_PageMainStye01
** Descriptions:            ��ʾ��������1
** input parameters:        
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ             
** Created Date:            2018-05-21
*****************************************************************/ 
void Gui_PageMainStye01(uint16_t index, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, float distance_cnt, uint8_t week, 
                        uint16_t am_pm, uint32_t step_cnt, uint8_t heart,
                        uint8_t ble_status, uint8_t charge, uint8_t battery_level, uint8_t menstrual,
                        uint16_t calories_cnt, uint32_t step_target, uint16_t sleep_time, uint8_t language, uint8_t *cls_flag)
{
    // ��������
    if(index == 0)
    {
        // �����ϲ�����
        Gui_PageClearBackground(0, 
                                0, 
                                MAX_X_SIZE, 
                                main3_main3_bg_coord[0][1]); 
        
        // ����������
        Gui_PageClearBackground(0, 
                                0, 
                                main3_main3_bg_coord[0][0], 
                                MAX_Y_SIZE); 


        // �����Ҳ�����
        Gui_PageClearBackground(main3_main3_bg_coord[0][0]+ICON_MAIN3_BG_WIDE, 
                                0, 
                                MAX_X_SIZE - (main3_main3_bg_coord[0][0]+ICON_MAIN1_ICON1_WIDE), 
                                MAX_Y_SIZE); 

//////        // �����²�����
//////        Gui_PageClearBackground(0, 
//////                                main3_main3_bg_coord[0][1]+ICON_MAIN1_ICON1_HIGH, 
//////                                main3_main3_bg_coord[0][0], 
//////                                MAX_Y_SIZE-(main3_main3_bg_coord[0][1]+ICON_MAIN1_ICON1_HIGH));         
    }
    // ����--����
    if(index == 0)
    {
        LCD_SetPosition(main3_main3_bg_coord[0][0], main3_main3_bg_coord[0][1],
                        ICON_MAIN3_BG_WIDE, ICON_MAIN3_BG_HIGH);
        LCD_DriverWriteDataFromExtFlash(main3_main3_bg_addr[0], ICON_MAIN3_BG_SIZE);
    }    

    Gui_PageMainAssistant( index, week, language, am_pm, ble_status, charge, battery_level, menstrual, main6_main6_data1_coord[0][1]);

    // ʱ��
    // ����ʱ��--ʱ
    LCD_SetPosition(main3_main4_data1_coord[0][0], main3_main4_data1_coord[0][1],
                    ICON_MAIN4_DATA1_WIDE, ICON_MAIN4_DATA1_HIGH);
    LCD_DriverWriteDataFromExtFlash(main3_main4_data1_addr[(hour / 10) % 10], ICON_MAIN4_DATA1_SIZE);
    LCD_SetPosition(main4_main4_data1_coord[1][0], main3_main4_data1_coord[1][1],
                    ICON_MAIN4_DATA1_WIDE, ICON_MAIN4_DATA1_HIGH);
    LCD_DriverWriteDataFromExtFlash(main3_main4_data1_addr[hour % 10], ICON_MAIN4_DATA1_SIZE);

    // ����ʱ��: 
     
    LCD_SetPosition(main3_main4_data1dot_coord[0][0], main3_main4_data1dot_coord[0][1],
                    ICON_MAIN4_DATA1DOT_WIDE, ICON_MAIN4_DATA1DOT_HIGH);
    LCD_DriverWriteDataFromExtFlash(main3_main4_data1dot_addr[0], ICON_MAIN4_DATA1DOT_SIZE); 
    
    // ����ʱ��--�� 
    LCD_SetPosition(main4_main4_data1_coord[2][0], main3_main4_data1_coord[2][1],
                    ICON_MAIN4_DATA1_WIDE, ICON_MAIN4_DATA1_HIGH);
    LCD_DriverWriteDataFromExtFlash(main3_main4_data1_addr[(minute / 10) % 10], ICON_MAIN4_DATA1_SIZE);
    LCD_SetPosition(main4_main4_data1_coord[3][0], main3_main4_data1_coord[3][1],
                    ICON_MAIN4_DATA1_WIDE, ICON_MAIN4_DATA1_HIGH);
    LCD_DriverWriteDataFromExtFlash(main3_main4_data1_addr[minute % 10], ICON_MAIN4_DATA1_SIZE); 

    // ����
    // ������ 
    LCD_SetPosition(main3_main4_data2_coord[0][0], main3_main4_data2_coord[0][1], 
                    ICON_MAIN8_DATA1_WIDE, ICON_MAIN8_DATA1_HIGH);
    LCD_DriverWriteDataFromExtFlash(main8_main8_data1_addr[month / 10 % 10], ICON_MAIN8_DATA1_SIZE);
    LCD_SetPosition(main3_main4_data2_coord[1][0], main3_main4_data2_coord[1][1], 
                    ICON_MAIN8_DATA1_WIDE, ICON_MAIN8_DATA1_HIGH);
    LCD_DriverWriteDataFromExtFlash(main8_main8_data1_addr[month % 10], ICON_MAIN8_DATA1_SIZE);
    
    // ������
    LCD_SetPosition(main3_main4_data2_coord[2][0], main3_main4_data2_coord[2][1], 
                    ICON_MAIN8_DATA1_WIDE, ICON_MAIN8_DATA1_HIGH);
    LCD_DriverWriteDataFromExtFlash(main8_main8_data1_addr[day / 10 % 10], ICON_MAIN8_DATA1_SIZE);
    LCD_SetPosition(main3_main4_data2_coord[3][0], main3_main4_data2_coord[3][1], 
                    ICON_MAIN8_DATA1_WIDE, ICON_MAIN8_DATA1_HIGH);
    LCD_DriverWriteDataFromExtFlash(main8_main8_data1_addr[day % 10], ICON_MAIN8_DATA1_SIZE);
    
    // ���� ��
    LCD_SetPosition(main6_main6_data2dot_coord[0][0], main6_main6_data2dot_coord[0][1], 
                    ICON_MAIN8_DATA1DOT_WIDE, ICON_MAIN8_DATA1DOT_HIGH);
    LCD_DriverWriteDataFromExtFlash(main8_main8_data1dot_addr[0], ICON_MAIN8_DATA1DOT_SIZE);
    
}

/****************************************************************
** Function name:           Gui_PageMainStye02
** Descriptions:            ��ʾ��������1
** input parameters:        
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ             
** Created Date:            2018-05-21
*****************************************************************/ 
 void Gui_PageMainStye02(uint16_t index, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, float distance_cnt, uint8_t week, 
                         uint16_t am_pm, uint32_t step_cnt, uint8_t heart,
                         uint8_t ble_status, uint8_t charge, uint8_t battery_level, uint8_t menstrual,
                         uint16_t calories_cnt, uint32_t step_target, uint16_t sleep_time, uint8_t language, uint8_t *cls_flag)
{
    // ��������
    if(index == 0 && *cls_flag != 0)
    {
        //����������
        Gui_PageClearBackground(0, 
                                0, 
                                main8_main8_icon1_coord[0][0], 
                                main8_main8_icon1_coord[0][1]+ICON_MAIN8_ICON1_HIGH);        
        //�����Ҳ�����
        Gui_PageClearBackground(main8_main8_icon1_coord[0][0]+ICON_MAIN8_ICON1_WIDE, 
                                0, 
                                MAX_X_SIZE-(main8_main8_icon1_coord[0][0]+ICON_MAIN8_ICON1_WIDE), 
                                main8_main8_icon1_coord[0][1]+ICON_MAIN8_ICON1_HIGH);

        //�����²������� ��ʱ����ϲ�
        Gui_PageClearBackground(0, 
                                main8_main8_icon1_coord[0][1]+ICON_MAIN8_ICON1_HIGH, 
                                MAX_X_SIZE, 
                                main8_main8_data2_coord[0][1]); 
        
        //ʱ��������
        Gui_PageClearBackground(0, 
                                main8_main8_data2_coord[0][1], 
                                main8_main8_data2_coord[0][0], 
                                MAX_Y_SIZE - main8_main8_data2_coord[0][1]);        
        //ʱ���Ҳ�����
        Gui_PageClearBackground(main8_main8_data2_coord[3][0]+ICON_MAIN8_DATA2_WIDE, 
                                main8_main8_data2_coord[3][1], 
                                MAX_X_SIZE-(main8_main8_data2_coord[0][0]+ICON_MAIN8_DATA2_WIDE), 
                                MAX_Y_SIZE - main8_main8_data2_coord[3][1]);

        //ʱ���²�������
        Gui_PageClearBackground(0, 
                                main8_main8_data2_coord[0][1] + ICON_MAIN8_DATA2_HIGH, 
                                MAX_X_SIZE, 
                                MAX_Y_SIZE - (main8_main8_data2_coord[0][1] + ICON_MAIN8_DATA2_HIGH)); 
                                
                                
    }
    
    // ����
    if(index == 0)
    {
        //����
        LCD_SetPosition(main8_main8_icon1_coord[0][0], main8_main8_icon1_coord[0][1], 
                        ICON_MAIN8_ICON1_WIDE, ICON_MAIN8_ICON1_HIGH);
        LCD_DriverWriteDataFromExtFlash(main8_main8_icon1_addr[0], ICON_MAIN8_ICON1_SIZE);
        //��������
        LCD_SetPosition(main8_main8_icon5_coord[0][0], main8_main8_icon5_coord[0][1], 
                        ICON_MAIN8_ICON5_WIDE, ICON_MAIN8_ICON5_HIGH);
        LCD_DriverWriteDataFromExtFlash(main8_main8_icon5_addr[0], ICON_MAIN8_ICON5_SIZE);    

        //��̱���
        LCD_SetPosition(main8_main8_icon2_coord[0][0], main8_main8_icon2_coord[0][1], 
                        ICON_MAIN8_ICON2_WIDE, ICON_MAIN8_ICON2_HIGH);
        LCD_DriverWriteDataFromExtFlash(main8_main8_icon2_addr[0], ICON_MAIN8_ICON2_SIZE);   

        //�Ʋ�����
        LCD_SetPosition(main8_main8_icon3_coord[0][0], main8_main8_icon3_coord[0][1], 
                        ICON_MAIN8_ICON3_WIDE, ICON_MAIN8_ICON3_HIGH);
        LCD_DriverWriteDataFromExtFlash(main8_main8_icon3_addr[0], ICON_MAIN8_ICON3_SIZE);

        //��·�ﱳ��
        LCD_SetPosition(main8_main8_icon4_coord[0][0], main8_main8_icon4_coord[0][1], 
                        ICON_MAIN8_ICON4_WIDE, ICON_MAIN8_ICON4_HIGH);
        LCD_DriverWriteDataFromExtFlash(main8_main8_icon4_addr[0], ICON_MAIN8_ICON4_SIZE);    


        //��̣�.
        LCD_SetPosition(main8_main8_data3dot_coord[0][0], main8_main8_data3dot_coord[0][1], 
                        ICON_MAIN8_DATA3DOT_WIDE, ICON_MAIN8_DATA3DOT_HIGH);
        LCD_DriverWriteDataFromExtFlash(main8_main8_data3dot_addr[0], ICON_MAIN8_DATA3DOT_SIZE);  
        
        //���ڣ�.
        LCD_SetPosition(main8_main8_data1dot_coord[0][0], main8_main8_data1dot_coord[0][1], 
                        ICON_MAIN8_DATA1DOT_WIDE, ICON_MAIN8_DATA1DOT_HIGH);
        LCD_DriverWriteDataFromExtFlash(main8_main8_data1dot_addr[0], ICON_MAIN8_DATA1DOT_SIZE); 
        
        
        //ʱ��ð��
        LCD_SetPosition(main8_main8_data2dot_coord[0][0], main8_main8_data2dot_coord[0][1], 
                        ICON_MAIN8_DATA2DOT_WIDE, ICON_MAIN8_DATA2DOT_HIGH);
        LCD_DriverWriteDataFromExtFlash(main8_main8_data2dot_addr[0], ICON_MAIN8_DATA2DOT_SIZE);         
        
    }
    

    // ʱ��
    // ����ʱ��--ʱ
    {
        LCD_SetPosition(main8_main8_data2_coord[0][0], main8_main8_data2_coord[0][1],
                        ICON_MAIN8_DATA2_WIDE, ICON_MAIN8_DATA2_HIGH);
        LCD_DriverWriteDataFromExtFlash(main8_main8_data2_addr[(hour / 10) % 10], ICON_MAIN8_DATA2_SIZE);
        LCD_SetPosition(main8_main8_data2_coord[1][0], main8_main8_data2_coord[1][1],
                        ICON_MAIN8_DATA2_WIDE, ICON_MAIN8_DATA2_HIGH);
        LCD_DriverWriteDataFromExtFlash(main8_main8_data2_addr[hour % 10], ICON_MAIN8_DATA2_SIZE);
    }
    
    
    // ����ʱ��--�� 
    {
        LCD_SetPosition(main8_main8_data2_coord[2][0], main8_main8_data2_coord[2][1],
                        ICON_MAIN8_DATA2_WIDE, ICON_MAIN8_DATA2_HIGH);
        LCD_DriverWriteDataFromExtFlash(main8_main8_data2_addr[(minute / 10) % 10], ICON_MAIN8_DATA2_SIZE);
        LCD_SetPosition(main8_main8_data2_coord[3][0], main8_main8_data2_coord[3][1],
                        ICON_MAIN8_DATA2_WIDE, ICON_MAIN8_DATA2_HIGH);
        LCD_DriverWriteDataFromExtFlash(main8_main8_data2_addr[minute % 10], ICON_MAIN8_DATA2_SIZE); 
    }    
    
    // ����
    // ��������--��
    LCD_SetPosition(main8_main8_data1_coord[0][0], main8_main8_data1_coord[0][1],
                         ICON_MAIN8_DATA1_WIDE, ICON_MAIN8_DATA1_HIGH);
    LCD_DriverWriteDataFromExtFlash(main8_main8_data1_addr[(month / 10) % 10], ICON_MAIN8_DATA1_SIZE);
    LCD_SetPosition(main8_main8_data1_coord[1][0], main8_main8_data1_coord[1][1],
                         ICON_MAIN8_DATA1_WIDE, ICON_MAIN8_DATA1_HIGH);
    LCD_DriverWriteDataFromExtFlash(main8_main8_data1_addr[month % 10], ICON_MAIN8_DATA1_SIZE);
    
    
    // ��������--�� 
    LCD_SetPosition(main8_main8_data1_coord[2][0], main8_main8_data1_coord[2][1],
                         ICON_MAIN8_DATA1_WIDE, ICON_MAIN8_DATA1_HIGH);
    LCD_DriverWriteDataFromExtFlash(main8_main8_data1_addr[(day / 10) % 10], ICON_MAIN8_DATA1_SIZE);
    LCD_SetPosition(main8_main8_data1_coord[3][0], main8_main8_data1_coord[3][1],
                         ICON_MAIN8_DATA1_WIDE, ICON_MAIN8_DATA1_HIGH);
    LCD_DriverWriteDataFromExtFlash(main8_main8_data1_addr[day % 10], ICON_MAIN8_DATA1_SIZE); 
    
    
    // ����
    {
        if(week)
        {
            week -= 1;
            week %= 7;
        }
        LCD_SetPosition(main8_main8_week_coord[0][0], main8_main8_week_coord[0][1],
                        ICON_MAIN8_WEEK_WIDE, ICON_MAIN8_WEEK_HIGH);
        LCD_DriverWriteDataFromExtFlash(main8_main8_week_addr[language][week], ICON_MAIN8_WEEK_SIZE);
    }
    

    // �Ʋ�����
    if(step_cnt > 99999)
    {
        step_cnt = 99999;
    }

    LCD_SetPosition(main8_main8_step_data3_coord[0][0], main8_main8_step_data3_coord[0][1], 
                    ICON_MAIN8_DATA3_WIDE, ICON_MAIN8_DATA3_HIGH);
    if(step_cnt >= 10000)
    {    
        LCD_DriverWriteDataFromExtFlash(main8_main8_data3_addr[(step_cnt / 10000) %10], ICON_MAIN8_DATA3_SIZE);        
    }  
    else
    {    
        LCD_DriverWriteDataFromExtFlash(main8_main8_data3_addr[0], ICON_MAIN8_DATA3_SIZE);        
    } 
    
    LCD_SetPosition(main8_main8_step_data3_coord[1][0], main8_main8_step_data3_coord[1][1], 
                    ICON_MAIN8_DATA3_WIDE, ICON_MAIN8_DATA3_HIGH);
    if(step_cnt >= 1000)
    {    
        LCD_DriverWriteDataFromExtFlash(main8_main8_data3_addr[(step_cnt / 1000) %10], ICON_MAIN8_DATA3_SIZE);        
    }  
    else
    {    
        LCD_DriverWriteDataFromExtFlash(main8_main8_data3_addr[0], ICON_MAIN8_DATA3_SIZE);        
    }
    
    LCD_SetPosition(main8_main8_step_data3_coord[2][0], main8_main8_step_data3_coord[2][1], 
                    ICON_MAIN8_DATA3_WIDE, ICON_MAIN8_DATA3_HIGH);
    if(step_cnt >= 100)
    {    
        LCD_DriverWriteDataFromExtFlash(main8_main8_data3_addr[(step_cnt / 100) %10], ICON_MAIN8_DATA3_SIZE);        
    }  
    else
    {    
        LCD_DriverWriteDataFromExtFlash(main8_main8_data3_addr[0], ICON_MAIN8_DATA3_SIZE);        
    }
    LCD_SetPosition(main8_main8_step_data3_coord[3][0], main8_main8_step_data3_coord[3][1], 
                    ICON_MAIN8_DATA3_WIDE, ICON_MAIN8_DATA3_HIGH);
    if(step_cnt >= 10)
    {    
        LCD_DriverWriteDataFromExtFlash(main8_main8_data3_addr[(step_cnt / 10) %10], ICON_MAIN8_DATA3_SIZE);        
    }  
    else
    {    
        LCD_DriverWriteDataFromExtFlash(main8_main8_data3_addr[0], ICON_MAIN8_DATA3_SIZE);        
    }
    
    LCD_SetPosition(main8_main8_step_data3_coord[4][0], main8_main8_step_data3_coord[4][1], 
                    ICON_MAIN8_DATA3_WIDE, ICON_MAIN8_DATA3_HIGH);
    if(step_cnt > 0)
    {    
        LCD_DriverWriteDataFromExtFlash(main8_main8_data3_addr[step_cnt %10], ICON_MAIN8_DATA3_SIZE);        
    }  
    else
    {    
        LCD_DriverWriteDataFromExtFlash(main8_main8_data3_addr[0], ICON_MAIN8_DATA3_SIZE);        
    }

    // ��·����ֵ
    {
        if(calories_cnt > 9999)
        {
            calories_cnt = 9999;
        }

        LCD_SetPosition(main8_main8_calor_data3_coord[0][0], main8_main8_calor_data3_coord[1][1], 
                        ICON_MAIN8_DATA3_WIDE, ICON_MAIN8_DATA3_HIGH);
        if(calories_cnt >= 1000)
        {    
            LCD_DriverWriteDataFromExtFlash(main8_main8_data3_addr[(calories_cnt / 1000) %10], ICON_MAIN8_DATA3_SIZE);        
        }  
        else
        {    
            LCD_DriverWriteDataFromExtFlash(main8_main8_data3_addr[0], ICON_MAIN8_DATA3_SIZE);        
        }
        
        LCD_SetPosition(main8_main8_calor_data3_coord[1][0], main8_main8_calor_data3_coord[1][1], 
                        ICON_MAIN8_DATA3_WIDE, ICON_MAIN8_DATA3_HIGH);
        if(calories_cnt >= 100)
        {    
            LCD_DriverWriteDataFromExtFlash(main8_main8_data3_addr[(calories_cnt / 100) %10], ICON_MAIN8_DATA3_SIZE);        
        }  
        else
        {    
            LCD_DriverWriteDataFromExtFlash(main8_main8_data3_addr[0], ICON_MAIN8_DATA3_SIZE);        
        }
        
        LCD_SetPosition(main8_main8_calor_data3_coord[2][0], main8_main8_calor_data3_coord[2][1], 
                        ICON_MAIN8_DATA3_WIDE, ICON_MAIN8_DATA3_HIGH);
        if(calories_cnt >= 10)
        {    
            LCD_DriverWriteDataFromExtFlash(main8_main8_data3_addr[(calories_cnt / 10) %10], ICON_MAIN8_DATA3_SIZE);        
        }  
        else
        {    
            LCD_DriverWriteDataFromExtFlash(main8_main8_data3_addr[0], ICON_MAIN8_DATA3_SIZE);        
        }
        
        LCD_SetPosition(main8_main8_calor_data3_coord[3][0], main8_main8_calor_data3_coord[3][1], 
                        ICON_MAIN8_DATA3_WIDE, ICON_MAIN8_DATA3_HIGH);
        if(calories_cnt > 0)
        {    
            LCD_DriverWriteDataFromExtFlash(main8_main8_data3_addr[calories_cnt %10], ICON_MAIN8_DATA3_SIZE);        
        }  
        else
        {    
            LCD_DriverWriteDataFromExtFlash(main8_main8_data3_addr[0], ICON_MAIN8_DATA3_SIZE);        
        }               
    }    

    // ������ֵ
    {
        uint32_t dis_count_tmp = 0;
        
        dis_count_tmp = (uint32_t)(distance_cnt* 10);
        if(dis_count_tmp > 999)
        {
            dis_count_tmp = 999;
        }
        
        LCD_SetPosition(main8_main8_lc_data3_coord[0][0], main8_main8_lc_data3_coord[0][1], 
                        ICON_MAIN8_DATA3_WIDE, ICON_MAIN8_DATA3_HIGH);
        if(dis_count_tmp >= 100)
        {    
            LCD_DriverWriteDataFromExtFlash(main8_main8_data3_addr[(dis_count_tmp / 100) %10], ICON_MAIN8_DATA3_SIZE);        
        }  
        else
        {    
            LCD_DriverWriteDataFromExtFlash(main8_main8_data3_addr[0], ICON_MAIN8_DATA3_SIZE);        
        }
        
        LCD_SetPosition(main8_main8_lc_data3_coord[1][0], main8_main8_lc_data3_coord[1][1], 
                        ICON_MAIN8_DATA3_WIDE, ICON_MAIN8_DATA3_HIGH);
        if(dis_count_tmp >= 10)
        {    
            LCD_DriverWriteDataFromExtFlash(main8_main8_data3_addr[(dis_count_tmp / 10) %10], ICON_MAIN8_DATA3_SIZE);        
        }  
        else
        {    
            LCD_DriverWriteDataFromExtFlash(main8_main8_data3_addr[0], ICON_MAIN8_DATA3_SIZE);        
        }
              

        LCD_SetPosition(main8_main8_lc_data3_coord[2][0], main8_main8_lc_data3_coord[2][1], 
                        ICON_MAIN8_DATA3_WIDE, ICON_MAIN8_DATA3_HIGH);
        if(dis_count_tmp > 0)
        {    
            LCD_DriverWriteDataFromExtFlash(main8_main8_data3_addr[dis_count_tmp %10], ICON_MAIN8_DATA3_SIZE);        
        }  
        else
        {    
            LCD_DriverWriteDataFromExtFlash(main8_main8_data3_addr[0], ICON_MAIN8_DATA3_SIZE);        
        }                    
    }
}

/****************************************************************
** Function name:           Gui_PageMainStye03
** Descriptions:            ��ʾ��������3
** input parameters:        
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ             
** Created Date:            2018-05-21
*****************************************************************/ 
void Gui_PageMainStye03(uint16_t index, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, float distance_cnt, uint8_t week, 
                        uint16_t am_pm, uint32_t step_cnt, uint8_t heart,
                        uint8_t ble_status, uint8_t charge, uint8_t battery_level, uint8_t menstrual,
                        uint16_t calories_cnt, uint32_t step_target, uint16_t sleep_time, uint8_t language, uint8_t *cls_flag)
{    
    
//	static uint8_t hour_save = 0;
	// ��������
    if(index == 0 && *cls_flag != 0)
    {
        // �����ϲ�����
        Gui_PageClearBackground(0, 
                                0, 
                                MAX_X_SIZE, 
                                main6_main6_bg_coord[0][1]); 
        
        
        // ����������
        Gui_PageClearBackground(0, 
                                0, 
                                main6_main6_bg_coord[0][0], 
                                MAX_Y_SIZE); 


        // �����Ҳ�����
        Gui_PageClearBackground(main6_main6_bg_coord[0][0]+ICON_MAIN6_BG_WIDE, 
                                0, 
                                MAX_X_SIZE - (main6_main6_bg_coord[0][0]+ICON_MAIN6_BG_WIDE), 
                                MAX_Y_SIZE); 

        
        // �����²�����
        Gui_PageClearBackground(0, 
                                main6_main6_bg_coord[0][1]+ICON_MAIN6_BG_HIGH, 
                                MAX_X_SIZE, 
                                MAX_Y_SIZE-(main6_main6_bg_coord[0][1]+ICON_MAIN6_BG_HIGH));         
    }
	// ������ʾ 
	if(index == 0)
	{
		LCD_SetPosition(main6_main6_bg_coord[0][0], main6_main6_bg_coord[0][1], 
						ICON_MAIN6_BG_WIDE, ICON_MAIN6_BG_HIGH);
		LCD_DriverWriteDataFromExtFlash(main6_main6_bg_addr[0], ICON_MAIN6_BG_SIZE);
        
	}
	
    // ʱ��
    // ����ʱ��--ʱ
    LCD_SetPosition(main6_main6_data1_coord[0][0], main6_main6_data1_coord[0][1],
                    ICON_MAIN6_DATA1_WIDE, ICON_MAIN6_DATA1_HIGH);
    LCD_DriverWriteDataFromExtFlash(main6_main6_data1_addr[(hour / 10) % 10], ICON_MAIN6_DATA1_SIZE);
    LCD_SetPosition(main6_main6_data1_coord[1][0], main6_main6_data1_coord[1][1],
                    ICON_MAIN6_DATA1_WIDE, ICON_MAIN6_DATA1_HIGH);
    LCD_DriverWriteDataFromExtFlash(main6_main6_data1_addr[hour % 10], ICON_MAIN6_DATA1_SIZE);
    

    // ����ʱ��--�� 
    LCD_SetPosition(main6_main6_data1_coord[2][0], main6_main6_data1_coord[2][1],
                    ICON_MAIN6_DATA1_WIDE, ICON_MAIN6_DATA1_HIGH);
    LCD_DriverWriteDataFromExtFlash(main6_main6_data1_addr[(minute / 10) % 10], ICON_MAIN6_DATA1_SIZE);
    LCD_SetPosition(main6_main6_data1_coord[3][0], main6_main6_data1_coord[3][1],
                    ICON_MAIN6_DATA1_WIDE, ICON_MAIN6_DATA1_HIGH);
    LCD_DriverWriteDataFromExtFlash(main6_main6_data1_addr[minute % 10], ICON_MAIN6_DATA1_SIZE); 

    // ����ʱ��: 
    LCD_SetPosition(main6_main6_data1dot_coord[0][0], main6_main6_data1dot_coord[0][1],
                    ICON_MAIN6_DATA1DOT_WIDE, ICON_MAIN6_DATA1DOT_HIGH);
    LCD_DriverWriteDataFromExtFlash(main6_main6_data1dot_addr[0], ICON_MAIN6_DATA1DOT_SIZE);

    
    // ����
    // ������ 
    LCD_SetPosition(main6_main6_data2_coord[0][0], main6_main6_data2_coord[0][1], 
                    ICON_MAIN6_DATA2_WIDE, ICON_MAIN6_DATA2_HIGH);
    LCD_DriverWriteDataFromExtFlash(main6_main6_data2_addr[month / 10 % 10], ICON_MAIN6_DATA2_SIZE);
    LCD_SetPosition(main6_main6_data2_coord[1][0], main6_main6_data2_coord[1][1], 
                    ICON_MAIN6_DATA2_WIDE, ICON_MAIN6_DATA2_HIGH);
    LCD_DriverWriteDataFromExtFlash(main6_main6_data2_addr[month % 10], ICON_MAIN6_DATA2_SIZE);
    
    // ������
    LCD_SetPosition(main6_main6_data2_coord[2][0], main6_main6_data2_coord[2][1], 
                    ICON_MAIN6_DATA2_WIDE, ICON_MAIN6_DATA2_HIGH);
    LCD_DriverWriteDataFromExtFlash(main6_main6_data2_addr[day / 10 % 10], ICON_MAIN6_DATA2_SIZE);
    LCD_SetPosition(main6_main6_data2_coord[3][0], main6_main6_data2_coord[3][1], 
                    ICON_MAIN6_DATA2_WIDE, ICON_MAIN6_DATA2_HIGH);
    LCD_DriverWriteDataFromExtFlash(main6_main6_data2_addr[day % 10], ICON_MAIN6_DATA2_SIZE);
    
    // ���� ��
    LCD_SetPosition(main6_main6_data2dot1_coord[0][0], main6_main6_data2dot1_coord[0][1], 
                    ICON_MAIN6_DATA2DOT_WIDE, ICON_MAIN6_DATA2DOT_HIGH);
    LCD_DriverWriteDataFromExtFlash(main6_main6_data2dot_addr[0], ICON_MAIN6_DATA2DOT_SIZE);
	  
    // ����
    LCD_SetPosition(main6_main1_icon3_coord[0][0], main6_main1_icon3_coord[0][1],
                    ICON_MAIN1_ICON3_WIDE, ICON_MAIN1_ICON3_HIGH);
    LCD_DriverWriteDataFromExtFlash(main6_main1_icon3_addr[ble_status % 2], ICON_MAIN1_ICON3_SIZE);

    // ������
    LCD_SetPosition(main6_main1_icon1_coord[0][0], main6_main1_icon1_coord[0][1], 
                    ICON_MAIN1_ICON1_WIDE, ICON_MAIN1_ICON1_HIGH);
    if(am_pm == GUI_TIME_FORMAT_AM)
    {
        if(language == 0)
        {
            LCD_DriverWriteDataFromExtFlash(main6_main1_icon1_addr[1], ICON_MAIN1_ICON1_SIZE);
        }
        else
        {
            LCD_DriverWriteDataFromExtFlash(main6_main1_icon1_addr[3], ICON_MAIN1_ICON1_SIZE);
        }
    }
    else if(am_pm == GUI_TIME_FORMAT_PM)
    {
        if(language == 0)
        {
            LCD_DriverWriteDataFromExtFlash(main6_main1_icon1_addr[0], ICON_MAIN1_ICON1_SIZE);
        }
        else
        {
            LCD_DriverWriteDataFromExtFlash(main6_main1_icon1_addr[2], ICON_MAIN1_ICON1_SIZE);
        }
    }
    else
    { 
        //��Ҫ�滻
        Gui_PageClearBackground(main6_main1_icon1_coord[0][0], main6_main1_icon1_coord[0][1], 
                                ICON_MAIN1_ICON1_WIDE, ICON_MAIN1_ICON1_HIGH);
    }
    
    // ����
    if(week)
    {
        week -= 1;
        week %= 7;
    }
    LCD_SetPosition(main6_main1_week_coord[0][0], main6_main1_week_coord[0][1],
                    ICON_MAIN1_WEEK_WIDE, ICON_MAIN1_WEEK_HIGH);
    LCD_DriverWriteDataFromExtFlash(main6_main1_week_addr[language % 2][week], ICON_MAIN1_WEEK_SIZE);

////    //Ů��
////    LCD_SetPosition(icon_16_main1_menstrual_coord[0][0], icon_16_main1_menstrual_coord[0][1], 
////                    ICON_SHARE2_WEMON_WIDE, ICON_SHARE2_WEMON_HIGH);
////    LCD_DriverWriteDataFromExtFlash(icon_16_main1_menstrual_addr[0][menstrual % 5], ICON_SHARE2_WEMON_SIZE);
        
    // ���
    if(charge == BATTERRY_IS_CHARGING)
    {
        // ����� 
        LCD_SetPosition(main6_main1_icon2_coord[0][0], main6_main1_icon2_coord[0][1],
                        ICON_MAIN1_ICON2_WIDE, ICON_MAIN1_ICON2_HIGH);
        LCD_DriverWriteDataFromExtFlash(main6_main1_icon2_addr[index % 5], ICON_MAIN1_ICON2_SIZE);

    }
    else
    {
        if(battery_level == 0)
        {
            if((index % 4 == 1)||(index % 4 == 3))
            {
                // ��ؿ�
                LCD_SetPosition(main6_main1_icon2_coord[0][0], main6_main1_icon2_coord[0][1],
                                ICON_MAIN1_ICON2_WIDE, ICON_MAIN1_ICON2_HIGH);
                LCD_DriverWriteDataFromExtFlash(main6_main1_icon2_addr[4], ICON_MAIN1_ICON2_SIZE);
            }
            else if((index % 4 == 0)||(index % 4 == 2))
            {
                // ��ؿ�
                LCD_SetPosition(main6_main1_icon2_coord[0][0], main6_main1_icon2_coord[0][1],
                                ICON_MAIN1_ICON2_WIDE, ICON_MAIN1_ICON2_HIGH);
                LCD_DriverWriteDataFromExtFlash(main6_main1_icon2_addr[0], ICON_MAIN1_ICON2_SIZE);
            }
        }
        else
        {
            // �������  --- buff��Ҫ����˳��
            battery_level %= 5;
            
            LCD_SetPosition(main6_main1_icon2_coord[0][0], main6_main1_icon2_coord[0][1],
                            ICON_MAIN1_ICON2_WIDE, ICON_MAIN1_ICON2_HIGH);
            LCD_DriverWriteDataFromExtFlash(main6_main1_icon2_addr[battery_level], ICON_MAIN1_ICON2_SIZE);
        }
    }
	
    
}

/****************************************************************
** Function name:           Gui_PageMainStye04
** Descriptions:            ��ʾ��������4
** input parameters:        
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ             
** Created Date:            2018-05-21
*****************************************************************/ 
void Gui_PageMainStye04(uint16_t index, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, float distance_cnt, uint8_t week, 
                        uint16_t am_pm, uint32_t step_cnt, uint8_t heart,
                        uint8_t ble_status, uint8_t charge, uint8_t battery_level, uint8_t menstrual,
                        uint16_t calories_cnt, uint32_t step_target, uint16_t sleep_time, uint8_t language, uint8_t *cls_flag)
{
    
	// ������ʾ 
	if(index == 0)
	{
                // �����ϲ�����
        Gui_PageClearBackground(0, 
                                0, 
                                MAX_X_SIZE, 
                                main4_main4_bg_coord[0][1]); 
        
        
        // ����������
        Gui_PageClearBackground(0, 
                                0, 
                                main4_main4_bg_coord[0][0], 
                                MAX_Y_SIZE); 


        // �����Ҳ�����
        Gui_PageClearBackground(main4_main4_bg_coord[0][0]+ICON_MAIN4_BG_WIDE, 
                                0, 
                                MAX_X_SIZE - (main4_main4_bg_coord[0][0]+ICON_MAIN4_BG_WIDE), 
                                MAX_Y_SIZE); 

        
        // �����²�����
        Gui_PageClearBackground(0, 
                                main4_main4_bg_coord[0][1]+ICON_MAIN4_BG_HIGH, 
                                MAX_X_SIZE, 
                                MAX_Y_SIZE-(main4_main4_bg_coord[0][1]+ICON_MAIN4_BG_HIGH));
                                
                                
        //�󱳾�
		LCD_SetPosition(main4_main4_bg_coord[0][0], main4_main4_bg_coord[0][1], 
						ICON_MAIN4_BG_WIDE, ICON_MAIN4_BG_HIGH);
		LCD_DriverWriteDataFromExtFlash(main4_main4_bg_addr[0], ICON_MAIN4_BG_SIZE);
                
	}    

    Gui_PageMainAssistant1( index, week, language, am_pm, ble_status, charge, battery_level, menstrual, 0);
	


    // ʱ��
    // ����ʱ��--ʱ
    LCD_SetPosition(main4_main4_data1_coord[0][0], main4_main4_data1_coord[0][1],
                    ICON_MAIN4_DATA1_WIDE, ICON_MAIN4_DATA1_HIGH);
    LCD_DriverWriteDataFromExtFlash(main4_main4_data1_addr[(hour / 10) % 10], ICON_MAIN4_DATA1_SIZE);
    LCD_SetPosition(main4_main4_data1_coord[1][0], main4_main4_data1_coord[1][1],
                    ICON_MAIN4_DATA1_WIDE, ICON_MAIN4_DATA1_HIGH);
    LCD_DriverWriteDataFromExtFlash(main4_main4_data1_addr[hour % 10], ICON_MAIN4_DATA1_SIZE);
    

    
    
    // ����ʱ��--�� 
    LCD_SetPosition(main4_main4_data1_coord[2][0], main4_main4_data1_coord[2][1],
                    ICON_MAIN4_DATA1_WIDE, ICON_MAIN4_DATA1_HIGH);
    LCD_DriverWriteDataFromExtFlash(main4_main4_data1_addr[(minute / 10) % 10], ICON_MAIN4_DATA1_SIZE);
    LCD_SetPosition(main4_main4_data1_coord[3][0], main4_main4_data1_coord[3][1],
                    ICON_MAIN4_DATA1_WIDE, ICON_MAIN4_DATA1_HIGH);
    LCD_DriverWriteDataFromExtFlash(main4_main4_data1_addr[minute % 10], ICON_MAIN4_DATA1_SIZE);
    
    // ����ʱ��: 
     
    LCD_SetPosition(main4_main4_data1dot_coord[0][0], main4_main4_data1dot_coord[0][1],
                    ICON_MAIN4_DATA1DOT_WIDE, ICON_MAIN4_DATA1DOT_HIGH);
    LCD_DriverWriteDataFromExtFlash(main4_main4_data1dot_addr[0], ICON_MAIN4_DATA1DOT_SIZE); 
  
}


/****************************************************************
** Function name:           Gui_PageMainStye05
** Descriptions:            ��ʾ��������5
** input parameters:        
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ             
** Created Date:            2018-05-21
*****************************************************************/ 
void Gui_PageChooseMain(uint16_t index, uint8_t Choose, uint8_t *cls_flag)
{    
    #if TEST_CLEAR_SCREEN_PARAM
    if(index == 0)
    {
        Gui_PageRefreshBackground(0,0,LCD_MAX_COLUMN,LCD_MAX_ROW,0x00F8);
    }
    #endif //TEST_CLEAR_SCREEN_PARAM
    
	// ��������
	//if(index == 0)
    if(Choose >= 4)
    {
        Choose = 0;
    }
    LCD_SetPosition(mainswitch6_interfaces_icon1_coord[0][0], mainswitch6_interfaces_icon1_coord[0][1],
                    ICON_INTERFACES_ICON1_WIDE, ICON_INTERFACES_ICON1_HIGH);
    LCD_DriverWriteDataFromExtFlash(mainswitch6_interfaces_icon1_addr[Choose % 4], ICON_INTERFACES_ICON1_SIZE); 
    *cls_flag = 1;
}

/****************************************************************
** Function name:           Gui_PageMain
** Descriptions:            ��ʾ������
** input parameters:        
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2018-05-22
*****************************************************************/     
void Gui_PageMain(uint16_t index, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, float distance_cnt, uint8_t week, 
                  uint16_t am_pm, uint32_t step_cnt, uint8_t heart,
                  uint8_t ble_status, uint8_t charge, uint8_t battery_level, uint8_t menstrual,
                  uint16_t calories_cnt, uint32_t step_target, uint16_t sleep_time, uint8_t language, uint8_t *cls_flag)
{
    #if TEST_CLEAR_SCREEN_PARAM
    if(index == 0)
    {
        Gui_PageRefreshBackground(0,0,LCD_MAX_COLUMN,LCD_MAX_ROW,0x00F8);
    }
    #endif //TEST_CLEAR_SCREEN_PARAM
	
    if(ui_style_type == 0)
    {
        //��ע��������3
        Gui_PageMainStye01(index, month, day, hour, minute, distance_cnt, week, 
                          am_pm, step_cnt, heart,
                          ble_status, charge, battery_level, menstrual,
                          calories_cnt, step_target, sleep_time, language, cls_flag);  
    }
    else if(ui_style_type == 1)
    {
        //��ע��������8
        Gui_PageMainStye02(index, month, day, hour, minute, distance_cnt, week, 
                          am_pm, step_cnt, heart,
                          ble_status, charge, battery_level, menstrual,
                          calories_cnt, step_target, sleep_time, language, cls_flag);        
    }
    else if(ui_style_type == 2)
    {
        //��ע��������6
        Gui_PageMainStye03(index, month, day, hour, minute, distance_cnt, week, 
                          am_pm, step_cnt, heart,
                          ble_status, charge, battery_level, menstrual,
                          calories_cnt, step_target, sleep_time, language, cls_flag);        
    } 
	else if(ui_style_type == 3)
    {
        //��ע��������4
        Gui_PageMainStye04(index, month, day, hour, minute, distance_cnt, week, 
                          am_pm, step_cnt, heart,
                          ble_status, charge, battery_level, menstrual,
                          calories_cnt, step_target, sleep_time, language, cls_flag);        
    }
    
    *cls_flag = 1;
}

/****************************************************************
** Function name:           Gui_PageStep
** Descriptions:            ��ʾ�Ʋ�����
** input parameters:        index����̬ͼ��ļ���
**                          step_cnt������
**                          target_cnt��Ŀ��
                            language: ����
** output parameters:       ��
** Returned value:          ��
** Created by:              YX              
** Created Date:            2017-08-07
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Gui_PageStep(uint32_t step_cnt, uint8_t step_prg, uint16_t index, uint8_t hour, uint8_t minute, uint8_t language, 
                  uint8_t *cls_flag, float distance_cnt, uint8_t uint_type, uint16_t calories_cnt)
{
    #if TEST_CLEAR_SCREEN_PARAM
    if(index == 0)
    {
        Gui_PageRefreshBackground(0,0,LCD_MAX_COLUMN,LCD_MAX_ROW,0x00F8);
    }
    #endif //TEST_CLEAR_SCREEN_PARAM
    
    // �Ʋ�����
    if(index == 0 && *cls_flag == 1)
    {
        
    }
    

    uint16_t target_location = 0;
    
    #if TEST_CLEAR_SCREEN_PARAM
    if(index == 0)
    {
        Gui_PageClearBackground(0,0,LCD_MAX_COLUMN,LCD_MAX_ROW,0x00F8);
    }
    #endif //TEST_CLEAR_SCREEN_PARAM
    
//    step_prg = 100;
    
    target_location = (uint16_t )step_prg * 10 / 100;
    
    if(language)
    {
        language = 1;
    }
    if(index == 0)
    {
        // ����
        LCD_SetPosition(step_step_bg1_coord[0][0], step_step_bg1_coord[0][1], 
                        ICON_STEP_BG1_WIDE, ICON_STEP_BG1_HIGH);
        LCD_DriverWriteDataFromExtFlash(step_step_bg1_addr[language%2], ICON_STEP_BG1_SIZE);      
//    }
//    //Ĭ�ϵ�������ʾ
//    if(index == 0)
//    {
        //��̬ͼ��
        LCD_SetPosition(step_step_icon5_coord[0][0], step_step_icon5_coord[0][1], 
                             ICON_STEP_ICON5_WIDE, ICON_STEP_ICON5_HIGH);
        LCD_DriverWriteDataFromExtFlash(step_step_icon5_addr[0], ICON_STEP_ICON5_SIZE);
        
        // �Ʋ���ֵ
        for(uint8_t i = 0; i < 5; i++)
        {
            LCD_SetPosition(step_step_data1_coord[i][0], step_step_data1_coord[i][1], 
                                 ICON_STEP_DATA1_WIDE, ICON_STEP_DATA1_HIGH);
            LCD_DriverWriteDataFromExtFlash(step_step_data1_addr[0], ICON_STEP_DATA1_SIZE);
        }
        
        // ��·����ֵ
        for(uint8_t i = 0; i < 4; i++)
        {
            LCD_SetPosition(step_calories_data2_coord[i][0], step_calories_data2_coord[i][1], 
                                 ICON_STEP_DATA2_WIDE, ICON_STEP_DATA2_HIGH);
            LCD_DriverWriteDataFromExtFlash(step_step_data2_addr[0], ICON_STEP_DATA2_SIZE);
        }
        
        // ������ֵ
        for(uint8_t i = 0; i < 3; i++)
        {
            LCD_SetPosition(step_distance_data2_coord[i][0], step_distance_data2_coord[i][1], 
                                 ICON_STEP_DATA2_WIDE, ICON_STEP_DATA2_HIGH);
            LCD_DriverWriteDataFromExtFlash(step_step_data2_addr[0], ICON_STEP_DATA2_SIZE);
        }
        // ��������-��
        LCD_SetPosition(step_step_data2dot_coord[0][0], step_step_data2dot_coord[0][1], 
                             ICON_STEP_DATA2DOT_WIDE, ICON_STEP_DATA2DOT_HIGH);
        LCD_DriverWriteDataFromExtFlash(step_step_data2dot_addr[0], ICON_STEP_DATA2DOT_SIZE);
        
    }
    
    // ��̵�λ
    LCD_SetPosition(step_step_icon4_coord[0][0], step_step_icon4_coord[0][1], 
                    ICON_STEP_ICON4_WIDE, ICON_STEP_ICON4_HIGH);
    LCD_DriverWriteDataFromExtFlash(step_step_icon4_addr[uint_type%2][language%2], ICON_STEP_ICON4_SIZE);     

    if(calories_cnt >= 9999)calories_cnt = 9999;
//    uint32_t calories_cnt2 = 0;
    uint32_t calories_cnt_tmp = calories_cnt/(10*2 - target_location);
    if(calories_cnt_tmp == 0)
    {
        calories_cnt_tmp = 1;
    }
        
    uint32_t distance_cnt1 = 0;
    if(distance_cnt > 999)
    {
        distance_cnt1 = 999;
    }
    if(distance_cnt < 100)
    {
        distance_cnt1 = (uint32_t)(distance_cnt* 10);
    }
//    uint32_t distance_cnt2 = 0;
    uint32_t distance_cnt_tmp = distance_cnt1/(10*2 - target_location);
    if(distance_cnt_tmp == 0)
    {
        distance_cnt_tmp = 1;
    }
    
    if(step_cnt > 99999)
    {
        step_cnt = 99999;
    }
//    uint32_t step_cnt2 = 0;
    uint32_t step_cnt_tmp = step_cnt/(10*2 - target_location);
    if(step_cnt_tmp == 0)
    {
        step_cnt_tmp = 1;
    }
        
    if(index == 1)
    {
        uint8_t flg1 = 0;
//        uint8_t flg2 = 0;
//        uint8_t flg3 = 0;
//        uint8_t flg4 = 0;
        uint8_t tmp1 = 0;
        uint8_t tmp2 = 0;
        uint8_t distance_disp[3] = {0};
        uint8_t calories_disp[4] = {0};
        uint8_t step_disp[5] = {0};
        while(1)
        {
            //�˶�Բ��ͼ��
            if(target_location != 0)
            {
                if(flg1 == 0)
                {
                    if(tmp1++ < 40)
                    {
                        LCD_SetPosition(step_step_icon5_coord[0][0], step_step_icon5_coord[0][1], 
                                             ICON_STEP_ICON5_WIDE, ICON_STEP_ICON5_HIGH);
                        LCD_DriverWriteDataFromExtFlash(step_step_icon5_addr[tmp1 % 11], ICON_STEP_ICON5_SIZE);
                    }
                    else
                    {
                        flg1 = 1;
                        tmp2 = 40;
                    }
                }
                else if(flg1 == 1)
                {
                    if(tmp2-- >= target_location)
                    {
                        LCD_SetPosition(step_step_icon5_coord[0][0], step_step_icon5_coord[0][1], 
                                             ICON_STEP_ICON5_WIDE, ICON_STEP_ICON5_HIGH);
                        LCD_DriverWriteDataFromExtFlash(step_step_icon5_addr[tmp2 % 11], ICON_STEP_ICON5_SIZE);
                    }
                    else
                    {
                        flg1 = 2;
                    }
                }
            }
            else
            {
                flg1 = 2;
            }
            
            // �Ʋ���ֵ
            //if(flg2 == 0)
            {
                step_disp[0] = step_cnt/10000%10;
                step_disp[1] = step_cnt/1000%10;
                step_disp[2] = step_cnt/100%10;
                step_disp[3] = step_cnt/10%10;
                step_disp[4] = step_cnt%10;
                for(uint8_t i = 0; i < 5; i++)
                {
                    LCD_SetPosition(step_step_data1_coord[i][0], step_step_data1_coord[i][1], 
                                         ICON_STEP_DATA1_WIDE, ICON_STEP_DATA1_HIGH);
                    LCD_DriverWriteDataFromExtFlash(step_step_data1_addr[step_disp[i]%10], ICON_STEP_DATA1_SIZE);
                }
//////                if(step_cnt2 == step_cnt)
//////                {
//////                    flg2 = 1;
//////                }
//////                
//////                step_cnt2 += step_cnt_tmp;
//////                if(step_cnt2 >= step_cnt)step_cnt2 = step_cnt;
            }
            
            // ��·����ֵ
            //if(flg3 == 0)
            {
                calories_disp[0] = calories_cnt/1000%10;
                calories_disp[1] = calories_cnt/100%10;
                calories_disp[2] = calories_cnt/10%10;
                calories_disp[3] = calories_cnt%10;
                for(uint8_t i = 0; i < 4; i++)
                {
                    LCD_SetPosition(step_calories_data2_coord[i][0], step_calories_data2_coord[i][1], 
                                    ICON_STEP_DATA2_WIDE, ICON_STEP_DATA2_HIGH);
                    LCD_DriverWriteDataFromExtFlash(step_step_data2_addr[calories_disp[i]%10], ICON_STEP_DATA2_SIZE);
                }
//////                if(calories_cnt2 == calories_cnt)
//////                {
//////                    flg3 = 1;
//////                }
//////                
//////                calories_cnt2 += calories_cnt_tmp;
//////                if(calories_cnt2 >= calories_cnt)calories_cnt2 = calories_cnt;
            }
            
            // ������ֵ
            //if(flg4 == 0)
            {
                distance_disp[0] = (uint32_t)distance_cnt1/100%10;
                distance_disp[1] = (uint32_t)distance_cnt1/10%10;
                distance_disp[2] = (uint32_t)distance_cnt1%10;
                for(uint8_t i = 0; i < 3; i++)
                {
                    LCD_SetPosition(step_distance_data2_coord[i][0], step_distance_data2_coord[i][1], 
                                         ICON_STEP_DATA2_WIDE, ICON_STEP_DATA2_HIGH);
                    LCD_DriverWriteDataFromExtFlash(step_step_data2_addr[distance_disp[i]%10], ICON_STEP_DATA2_SIZE);
                }
//////                if(distance_cnt2 == distance_cnt1)
//////                {
//////                    flg4 = 1;
//////                }
//////                
//////                distance_cnt2 += distance_cnt_tmp;
//////                if(distance_cnt2 >= distance_cnt1)distance_cnt2 = distance_cnt1;
            }

            //if(flg1 == 2 && flg2 == 1 && flg3 == 1 && flg4 == 1)
            {
                break;
            }
        }
    }
    language_save = language;
	unit_type_save = uint_type;
    
    *cls_flag = 2;
}

/****************************************************************
** Function name:           Gui_PageDistance
** Descriptions:            ��ʾ��̽���
** input parameters:        index����̬ͼ��ļ���
**                          dis_count���������
                            uint_type: ��λ���ͣ�km\mi
                            language: ���ԣ�0���ġ�1Ӣ��
** output parameters:       
** Returned value:          
** Created by:              YX              
** Created Date:            2017-08-10
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Gui_PageDistance(float distance_cnt, uint16_t index, uint8_t uint_type, uint8_t hour, uint8_t minute, uint8_t language, uint8_t *cls_flag)
{
//////    #if TEST_CLEAR_SCREEN_PARAM
//////    if(index == 0)
//////    {
//////        Gui_PageRefreshBackground(0,0,LCD_MAX_COLUMN,LCD_MAX_ROW,0x00F8);
//////    }
//////    #endif //TEST_CLEAR_SCREEN_PARAM
//////    
//////    // ����������
//////    if(index == 0 && *cls_flag == 1)
//////    {
//////        
//////    }

//////	// ʱ��
//////	{
//////		// ����ʱ��--ʱ
//////        LCD_SetPosition(icon_16_scd_step_time_coord[0][0], icon_16_scd_step_time_coord[0][1],
//////                        ICON_SHARE_DATA1_WIDE, ICON_SHARE_DATA1_HIGH);
//////        LCD_DriverWriteDataFromExtFlash(icon_16_scd_step_time_addr[(hour / 10) % 10], ICON_SHARE_DATA1_SIZE);
//////        
//////        LCD_SetPosition(icon_16_scd_step_time_coord[1][0], icon_16_scd_step_time_coord[1][1],
//////                        ICON_SHARE_DATA1_WIDE, ICON_SHARE_DATA1_HIGH);
//////        LCD_DriverWriteDataFromExtFlash(icon_16_scd_step_time_addr[hour % 10], ICON_SHARE_DATA1_SIZE);
//////        
//////    
//////        // ����ʱ��--�� 
//////        LCD_SetPosition(icon_16_scd_step_time_coord[2][0], icon_16_scd_step_time_coord[2][1],
//////                        ICON_SHARE_DATA1_WIDE, ICON_SHARE_DATA1_HIGH);
//////        LCD_DriverWriteDataFromExtFlash(icon_16_scd_step_time_addr[(minute / 10) % 10], ICON_SHARE_DATA1_SIZE);
//////        
//////        LCD_SetPosition(icon_16_scd_step_time_coord[3][0], icon_16_scd_step_time_coord[3][1],
//////                        ICON_SHARE_DATA1_WIDE, ICON_SHARE_DATA1_HIGH);
//////        LCD_DriverWriteDataFromExtFlash(icon_16_scd_step_time_addr[minute % 10], ICON_SHARE_DATA1_SIZE); 
//////        
//////        
//////        // ����ʱ��--: 
//////        LCD_SetPosition(icon_16_scd_step_time_mh_coord[0][0], icon_16_scd_step_time_mh_coord[0][1],
//////                        ICON_SHARE_DATA1DOT_WIDE, ICON_SHARE_DATA1DOT_HIGH);
//////        LCD_DriverWriteDataFromExtFlash(icon_16_scd_step_mh_addr[0], ICON_SHARE_DATA1DOT_SIZE);        
//////	}
//////    
//////    
//////    // ���뵥λ
//////    if((index == 0)||(language_save != language))
//////    {
//////        LCD_SetPosition(icon_16_scd_distance_dw_coord[0][0], icon_16_scd_distance_dw_coord[0][1], 
//////                        ICON_DISTANCE_ICON1_WIDE, ICON_DISTANCE_ICON1_HIGH);
//////        LCD_DriverWriteDataFromExtFlash(icon_16_scd_distance_dw_addr[0][language % 2], ICON_DISTANCE_ICON1_SIZE);
//////        
//////        
//////        // ���뵥λ
//////        if((index == 0)||(language_save != language)||(unit_type_save != uint_type))
//////        {
//////            LCD_SetPosition(icon_16_scd_distance_dw1_coord[0][0], icon_16_scd_distance_dw1_coord[0][1], 
//////                            ICON_DISTANCE_ICON6_WIDE, ICON_DISTANCE_ICON6_HIGH);
//////            LCD_DriverWriteDataFromExtFlash(icon_16_scd_distance_dw1_addr[language%2][uint_type%2], ICON_DISTANCE_ICON6_SIZE);
//////        }       
//////    }
//////    
//////    
//////    
//////    // ���붯ͼ
//////    if(index %5 == 0)
//////    {
//////        LCD_SetPosition(icon_16_scd_distance_dt1_coord[0][0], icon_16_scd_distance_dt1_coord[0][1], 
//////                        ICON_DISTANCE_ICON2_WIDE, ICON_DISTANCE_ICON2_HIGH);
//////        LCD_DriverWriteDataFromExtFlash(icon_16_scd_distance_dt1_addr[index/5 % 2], ICON_DISTANCE_ICON2_SIZE);
//////    }
//////    
////// 
//////    // ��ͼ
//////    {
//////        if(index == 0)
//////        {
//////            LCD_SetPosition(icon_16_scd_step_qiu_coord[1][0], icon_16_scd_step_qiu_coord[1][1], 
//////                            ICON_SHARE_ICON3_WIDE, ICON_SHARE_ICON3_HIGH);
//////            LCD_DriverWriteDataFromExtFlash(icon_16_scd_step_qiu_addr[1][3], ICON_SHARE_ICON3_SIZE);
//////        }
//////        
//////        if(index %8 < 4)
//////        {
//////            LCD_SetPosition(icon_16_scd_step_qiu_coord[0][0], icon_16_scd_step_qiu_coord[0][1], 
//////                            ICON_SHARE_ICON2_WIDE, ICON_SHARE_ICON2_HIGH);
//////            LCD_DriverWriteDataFromExtFlash(icon_16_scd_step_qiu_addr[0][index % 4], ICON_SHARE_ICON2_SIZE);
//////        
//////        }
//////        else
//////        {
//////            LCD_SetPosition(icon_16_scd_step_qiu_coord[1][0], icon_16_scd_step_qiu_coord[1][1], 
//////                            ICON_SHARE_ICON3_WIDE, ICON_SHARE_ICON3_HIGH);
//////            LCD_DriverWriteDataFromExtFlash(icon_16_scd_step_qiu_addr[1][index % 4], ICON_SHARE_ICON3_SIZE);        
//////        }
//////                
//////    }

//////    
//////    // ��ͷ��ͼ
//////    if(index %2 == 0)
//////    {
//////        LCD_SetPosition(icon_16_scd_distance_dt_coord[0][0], icon_16_scd_distance_dt_coord[0][1], 
//////                        ICON_CALORIES_ICON4_WIDE, ICON_CALORIES_ICON4_HIGH);
//////        LCD_DriverWriteDataFromExtFlash(icon_16_scd_distance_dt_addr[0][index/2 % 2], ICON_CALORIES_ICON4_SIZE);
//////        
//////        LCD_SetPosition(icon_16_scd_distance_dt_coord[1][0], icon_16_scd_distance_dt_coord[1][1], 
//////                        ICON_CALORIES_ICON4_WIDE, ICON_CALORIES_ICON4_HIGH);
//////        LCD_DriverWriteDataFromExtFlash(icon_16_scd_distance_dt_addr[1][index/2 % 2], ICON_CALORIES_ICON4_SIZE);        
//////    }
//////    

//////    // ���
//////    {
//////        LCD_SetPosition(icon_16_scd_step_bg_coord[0][0], icon_16_scd_step_bg_coord[0][1], 
//////                        ICON_SHARE_ICON4_WIDE, ICON_SHARE_ICON4_HIGH);
//////        LCD_DriverWriteDataFromExtFlash(icon_16_scd_step_bg_addr[0], ICON_SHARE_ICON4_SIZE);
//////    }
//////    
//////    if(index == 0)
//////    {
//////        // ��ֵ
//////        Gui_PageClearBackground(0, 
//////                                icon_bigtosmall_coord[5][1], 
//////                                MAX_X_SIZE, 
//////                                icon_16_scd_distance_num_coord[0][1] - icon_bigtosmall_coord[5][1]); 
//////        
//////        // ��ֵ
//////        Gui_PageClearBackground(0, 
//////                                icon_16_scd_distance_num_coord[0][1], 
//////                                icon_16_scd_distance_num_coord[0][0], 
//////                                icon_bigtosmall_coord[5][1]+ icon_bigtosmall_size[5][1]-(icon_16_scd_distance_num_coord[0][1])); 

//////        // ��ֵ
//////        Gui_PageClearBackground(icon_16_scd_distance_num_coord[2][0]+ICON_SHARE_DATA2_WIDE, 
//////                                icon_16_scd_distance_num_coord[0][1], 
//////                                MAX_X_SIZE-(icon_16_scd_distance_num_coord[2][0]+ICON_SHARE_DATA2_WIDE), 
//////                                icon_bigtosmall_coord[5][1]+ icon_bigtosmall_size[5][1]-(icon_16_scd_distance_num_coord[0][1])); 

//////        // ��ֵ
//////        Gui_PageClearBackground(0, 
//////                                icon_16_scd_distance_num_coord[0][1]+ICON_SHARE_DATA2_HIGH, 
//////                                MAX_X_SIZE, 
//////                                icon_bigtosmall_coord[5][1]+ icon_bigtosmall_size[5][1]-(icon_16_scd_distance_num_coord[0][1]+ICON_SHARE_DATA2_HIGH)); 
//////    }
//////    
//////    // ������ֵ
//////    {
//////        uint32_t dis_count_tmp = 0;
//////        
//////        dis_count_tmp = (uint32_t)(distance_cnt* 10);
//////        if(dis_count_tmp > 999)
//////        {
//////            dis_count_tmp = 999;
//////        }
//////        
//////        LCD_SetPosition(icon_16_scd_distance_num_coord[0][0], icon_16_scd_distance_num_coord[0][1], 
//////                        ICON_SHARE_DATA2_WIDE, ICON_SHARE_DATA2_HIGH);
//////        if(dis_count_tmp >= 100)
//////        {    
//////            LCD_DriverWriteDataFromExtFlash(icon_16_scd_distance_num_addr[(dis_count_tmp / 100) %10], ICON_SHARE_DATA2_SIZE);        
//////        }  
//////        else
//////        {    
//////            LCD_DriverWriteDataFromExtFlash(icon_16_scd_distance_num_addr[0], ICON_SHARE_DATA2_SIZE);        
//////        }
//////        
//////        LCD_SetPosition(icon_16_scd_distance_num_coord[1][0], icon_16_scd_distance_num_coord[1][1], 
//////                        ICON_SHARE_DATA2_WIDE, ICON_SHARE_DATA2_HIGH);
//////        if(dis_count_tmp >= 10)
//////        {    
//////            LCD_DriverWriteDataFromExtFlash(icon_16_scd_distance_num_addr[(dis_count_tmp / 10) %10], ICON_SHARE_DATA2_SIZE);        
//////        }  
//////        else
//////        {    
//////            LCD_DriverWriteDataFromExtFlash(icon_16_scd_distance_num_addr[0], ICON_SHARE_DATA2_SIZE);        
//////        }
//////        
//////        // ��������-��
//////        LCD_SetPosition(icon_16_scd_distance_d_coord[0][0], icon_16_scd_distance_d_coord[0][1], 
//////                        ICON_DISTANCE_DATA1DOT_WIDE, ICON_DISTANCE_DATA1DOT_HIGH);
//////        LCD_DriverWriteDataFromExtFlash(icon_16_scd_distance_d_addr[0], ICON_DISTANCE_DATA1DOT_SIZE);  
//////        

//////        LCD_SetPosition(icon_16_scd_distance_num_coord[2][0], icon_16_scd_distance_num_coord[2][1], 
//////                        ICON_SHARE_DATA2_WIDE, ICON_SHARE_DATA2_HIGH);
//////        if(dis_count_tmp > 0)
//////        {    
//////            LCD_DriverWriteDataFromExtFlash(icon_16_scd_distance_num_addr[dis_count_tmp %10], ICON_SHARE_DATA2_SIZE);        
//////        }  
//////        else
//////        {    
//////            LCD_DriverWriteDataFromExtFlash(icon_16_scd_distance_num_addr[0], ICON_SHARE_DATA2_SIZE);        
//////        }                    
//////    }
//////    
//////    language_save = language;
//////	unit_type_save = uint_type;
//////    *cls_flag = 2;
}

/****************************************************************
** Function name:           Gui_PageCalories
** Descriptions:            ��ʾ��·��
** input parameters:        index����̬ͼ��ļ���
                            calorie����·��󿨣�
                            language: ���ԣ�0���ġ�1Ӣ��
** output parameters:       
** Returned value:          
** Created by:              WJ              
** Created Date:            2017-08-11
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Gui_PageCalories(uint16_t calories_cnt, uint16_t index, uint8_t hour, uint8_t minute, uint8_t language, uint8_t *cls_flag)
{
//////    #if TEST_CLEAR_SCREEN_PARAM
//////    if(index == 0)
//////    {
//////        Gui_PageRefreshBackground(0,0,LCD_MAX_COLUMN,LCD_MAX_ROW,0x00F8);
//////    }
//////    #endif //TEST_CLEAR_SCREEN_PARAM
//////    
//////    // ��·������
//////    if(index == 0 && *cls_flag == 1)
//////    {
//////         
//////    }



//////	// ʱ��
//////	{
//////		// ����ʱ��--ʱ
//////        LCD_SetPosition(icon_16_scd_step_time_coord[0][0], icon_16_scd_step_time_coord[0][1],
//////                        ICON_SHARE_DATA1_WIDE, ICON_SHARE_DATA1_HIGH);
//////        LCD_DriverWriteDataFromExtFlash(icon_16_scd_step_time_addr[(hour / 10) % 10], ICON_SHARE_DATA1_SIZE);
//////        
//////        LCD_SetPosition(icon_16_scd_step_time_coord[1][0], icon_16_scd_step_time_coord[1][1],
//////                        ICON_SHARE_DATA1_WIDE, ICON_SHARE_DATA1_HIGH);
//////        LCD_DriverWriteDataFromExtFlash(icon_16_scd_step_time_addr[hour % 10], ICON_SHARE_DATA1_SIZE);
//////        
//////    
//////        // ����ʱ��--�� 
//////        LCD_SetPosition(icon_16_scd_step_time_coord[2][0], icon_16_scd_step_time_coord[2][1],
//////                        ICON_SHARE_DATA1_WIDE, ICON_SHARE_DATA1_HIGH);
//////        LCD_DriverWriteDataFromExtFlash(icon_16_scd_step_time_addr[(minute / 10) % 10], ICON_SHARE_DATA1_SIZE);
//////        
//////        LCD_SetPosition(icon_16_scd_step_time_coord[3][0], icon_16_scd_step_time_coord[3][1],
//////                        ICON_SHARE_DATA1_WIDE, ICON_SHARE_DATA1_HIGH);
//////        LCD_DriverWriteDataFromExtFlash(icon_16_scd_step_time_addr[minute % 10], ICON_SHARE_DATA1_SIZE); 
//////        
//////        
//////        // ����ʱ��--: 
//////        LCD_SetPosition(icon_16_scd_step_time_mh_coord[0][0], icon_16_scd_step_time_mh_coord[0][1],
//////                        ICON_SHARE_DATA1DOT_WIDE, ICON_SHARE_DATA1DOT_HIGH);
//////        LCD_DriverWriteDataFromExtFlash(icon_16_scd_step_mh_addr[0], ICON_SHARE_DATA1DOT_SIZE);        
//////	}
//////    
//////    
//////    // ��·�ﵥλ
//////    if((index == 0)||(language_save != language))
//////    {
//////        LCD_SetPosition(icon_16_scd_calories_dw_coord[0][0], icon_16_scd_calories_dw_coord[0][1], 
//////                        ICON_CALORIES_ICON1_WIDE, ICON_CALORIES_ICON1_HIGH);
//////        LCD_DriverWriteDataFromExtFlash(icon_16_scd_calories_dw_addr[0][language % 2], ICON_CALORIES_ICON1_SIZE);
//////        
//////        
//////        LCD_SetPosition(icon_16_scd_calories_dw_coord[1][0], icon_16_scd_calories_dw_coord[1][1], 
//////                        ICON_CALORIES_ICON6_WIDE, ICON_CALORIES_ICON6_HIGH);
//////        LCD_DriverWriteDataFromExtFlash(icon_16_scd_calories_dw_addr[1][language % 2], ICON_CALORIES_ICON6_SIZE);       
//////    }
//////    
//////    
//////    
//////    // ��·�ﶯͼ
//////    if(index % 5 == 0)
//////    {
//////        LCD_SetPosition(icon_16_scd_calories_dt1_coord[0][0], icon_16_scd_calories_dt1_coord[0][1], 
//////                        ICON_CALORIES_ICON2_WIDE, ICON_CALORIES_ICON2_HIGH);
//////        LCD_DriverWriteDataFromExtFlash(icon_16_scd_calories_dt1_addr[index/5 % 3], ICON_CALORIES_ICON2_SIZE);
//////    }
//////    
////// 
//////    // ��ͼ
//////    {
//////        if(index == 0)
//////        {
//////            LCD_SetPosition(icon_16_scd_step_qiu_coord[1][0], icon_16_scd_step_qiu_coord[1][1], 
//////                            ICON_SHARE_ICON3_WIDE, ICON_SHARE_ICON3_HIGH);
//////            LCD_DriverWriteDataFromExtFlash(icon_16_scd_step_qiu_addr[1][3], ICON_SHARE_ICON3_SIZE);
//////        }
//////        
//////        if(index % 8 < 4)
//////        {
//////            LCD_SetPosition(icon_16_scd_step_qiu_coord[0][0], icon_16_scd_step_qiu_coord[0][1], 
//////                            ICON_SHARE_ICON2_WIDE, ICON_SHARE_ICON2_HIGH);
//////            LCD_DriverWriteDataFromExtFlash(icon_16_scd_step_qiu_addr[0][index % 4], ICON_SHARE_ICON2_SIZE);
//////        
//////        }
//////        else
//////        {
//////            LCD_SetPosition(icon_16_scd_step_qiu_coord[1][0], icon_16_scd_step_qiu_coord[1][1], 
//////                            ICON_SHARE_ICON3_WIDE, ICON_SHARE_ICON3_HIGH);
//////            LCD_DriverWriteDataFromExtFlash(icon_16_scd_step_qiu_addr[1][index % 4], ICON_SHARE_ICON3_SIZE);        
//////        }
//////                
//////    }

//////    
//////    // ��ͷ��ͼ
//////    if(index %2 == 0)
//////    {
//////        LCD_SetPosition(icon_16_scd_calories_dt_coord[0][0], icon_16_scd_calories_dt_coord[0][1], 
//////                        ICON_CALORIES_ICON4_WIDE, ICON_CALORIES_ICON4_HIGH);
//////        LCD_DriverWriteDataFromExtFlash(icon_16_scd_calories_dt_addr[0][index/2 % 2], ICON_CALORIES_ICON4_SIZE);
//////        
//////        LCD_SetPosition(icon_16_scd_calories_dt_coord[1][0], icon_16_scd_calories_dt_coord[1][1], 
//////                        ICON_CALORIES_ICON4_WIDE, ICON_CALORIES_ICON4_HIGH);
//////        LCD_DriverWriteDataFromExtFlash(icon_16_scd_calories_dt_addr[1][index/2 % 2], ICON_CALORIES_ICON4_SIZE);        
//////    }
//////    

//////    // ���
//////    {
//////        LCD_SetPosition(icon_16_scd_step_bg_coord[0][0], icon_16_scd_step_bg_coord[0][1], 
//////                        ICON_SHARE_ICON4_WIDE, ICON_SHARE_ICON4_HIGH);
//////        LCD_DriverWriteDataFromExtFlash(icon_16_scd_step_bg_addr[0], ICON_SHARE_ICON4_SIZE);
//////    }


//////    if(index == 0)
//////    {
//////        // ��ֵ
//////        Gui_PageClearBackground(0, 
//////                                icon_bigtosmall_coord[5][1], 
//////                                MAX_X_SIZE, 
//////                                icon_16_scd_calories_num_coord[0][1] - icon_bigtosmall_coord[5][1]); 
//////        
//////        // ��ֵ
//////        Gui_PageClearBackground(0, 
//////                                icon_16_scd_calories_num_coord[0][1], 
//////                                icon_16_scd_calories_num_coord[0][0], 
//////                                icon_bigtosmall_coord[5][1]+ icon_bigtosmall_size[5][1]-(icon_16_scd_calories_num_coord[0][1])); 

//////        // ��ֵ
//////        Gui_PageClearBackground(icon_16_scd_calories_num_coord[2][0]+ICON_SHARE_DATA2_WIDE, 
//////                                icon_16_scd_calories_num_coord[0][1], 
//////                                MAX_X_SIZE-(icon_16_scd_calories_num_coord[2][0]+ICON_SHARE_DATA2_WIDE), 
//////                                icon_bigtosmall_coord[5][1]+ icon_bigtosmall_size[5][1]-(icon_16_scd_calories_num_coord[0][1])); 

//////        // ��ֵ
//////        Gui_PageClearBackground(0, 
//////                                icon_16_scd_calories_num_coord[0][1]+ICON_SHARE_DATA2_HIGH, 
//////                                MAX_X_SIZE, 
//////                                icon_bigtosmall_coord[5][1]+ icon_bigtosmall_size[5][1]-(icon_16_scd_calories_num_coord[0][1]+ICON_SHARE_DATA2_HIGH)); 
//////    }
//////    
//////    // ��·����ֵ
//////    {
//////        if(calories_cnt > 9999)
//////        {
//////            calories_cnt = 9999;
//////        }

//////        LCD_SetPosition(icon_16_scd_calories_num_coord[0][0], icon_16_scd_calories_num_coord[1][1], 
//////                        ICON_SHARE_DATA2_WIDE, ICON_SHARE_DATA2_HIGH);
//////        if(calories_cnt >= 1000)
//////        {    
//////            LCD_DriverWriteDataFromExtFlash(icon_16_scd_calories_num_addr[(calories_cnt / 1000) %10], ICON_SHARE_DATA2_SIZE);        
//////        }  
//////        else
//////        {    
//////            LCD_DriverWriteDataFromExtFlash(icon_16_scd_calories_num_addr[0], ICON_SHARE_DATA2_SIZE);        
//////        }
//////        
//////        LCD_SetPosition(icon_16_scd_calories_num_coord[1][0], icon_16_scd_calories_num_coord[1][1], 
//////                        ICON_SHARE_DATA2_WIDE, ICON_SHARE_DATA2_HIGH);
//////        if(calories_cnt >= 100)
//////        {    
//////            LCD_DriverWriteDataFromExtFlash(icon_16_scd_calories_num_addr[(calories_cnt / 100) %10], ICON_SHARE_DATA2_SIZE);        
//////        }  
//////        else
//////        {    
//////            LCD_DriverWriteDataFromExtFlash(icon_16_scd_calories_num_addr[0], ICON_SHARE_DATA2_SIZE);        
//////        }
//////        
//////        LCD_SetPosition(icon_16_scd_calories_num_coord[2][0], icon_16_scd_calories_num_coord[2][1], 
//////                        ICON_SHARE_DATA2_WIDE, ICON_SHARE_DATA2_HIGH);
//////        if(calories_cnt >= 10)
//////        {    
//////            LCD_DriverWriteDataFromExtFlash(icon_16_scd_calories_num_addr[(calories_cnt / 10) %10], ICON_SHARE_DATA2_SIZE);        
//////        }  
//////        else
//////        {    
//////            LCD_DriverWriteDataFromExtFlash(icon_16_scd_calories_num_addr[0], ICON_SHARE_DATA2_SIZE);        
//////        }
//////        
//////        LCD_SetPosition(icon_16_scd_calories_num_coord[3][0], icon_16_scd_calories_num_coord[3][1], 
//////                        ICON_SHARE_DATA2_WIDE, ICON_SHARE_DATA2_HIGH);
//////        if(calories_cnt > 0)
//////        {    
//////            LCD_DriverWriteDataFromExtFlash(icon_16_scd_calories_num_addr[calories_cnt %10], ICON_SHARE_DATA2_SIZE);        
//////        }  
//////        else
//////        {    
//////            LCD_DriverWriteDataFromExtFlash(icon_16_scd_calories_num_addr[0], ICON_SHARE_DATA2_SIZE);        
//////        }               
//////    }
//////    
//////    language_save = language;
//////    *cls_flag = 2;
}

/****************************************************************
** Function name:           Gui_PageRunMode
** Descriptions:            ��ʾ�˶�ģʽ
** input parameters:        index����̬ͼ��ļ���
                            language: ���ԣ�0���ġ�1Ӣ��
** output parameters:       
** Returned value:          
** Created by:              WJ              
** Created Date:            2017-08-11
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Gui_PageRunMode(uint16_t index, uint8_t hour, uint8_t minute, uint8_t language, uint8_t *cls_flag)
{	
    #if TEST_CLEAR_SCREEN_PARAM
    if(index == 0)
    {
        Gui_PageRefreshBackground(0,0,LCD_MAX_COLUMN,LCD_MAX_ROW,0x00F8);
    }
    #endif //TEST_CLEAR_SCREEN_PARAM
    
    if(language)
    {
        language = 1;
    }
    
    // �˶�ģʽ����
    if(index == 0)
    {
        // �����ϲ�����
        Gui_PageClearBackground(0, 
                                0, 
                                MAX_X_SIZE, 
                                sports_sports_icon1_coord[0][1]); 
        
        
        // ����������
        Gui_PageClearBackground(0, 
                                0, 
                                sports_sports_icon1_coord[0][0], 
                                MAX_Y_SIZE); 


        // �����Ҳ�����
        Gui_PageClearBackground(sports_sports_icon1_coord[0][0]+ICON_SPORTS_ICON1_WIDE, 
                                0, 
                                MAX_X_SIZE - (sports_sports_icon1_coord[0][0]+ICON_SPORTS_ICON1_WIDE), 
                                MAX_Y_SIZE); 

        
        // �����²�����
        Gui_PageClearBackground(0, 
                                sports_sports_icon1_coord[0][1]+ICON_SPORTS_ICON1_HIGH, 
                                MAX_X_SIZE, 
                                MAX_Y_SIZE-(sports_sports_icon1_coord[0][1]+ICON_SPORTS_ICON1_HIGH));    
    }

     //����
    LCD_SetPosition(sports_sports_icon1_coord[0][0], sports_sports_icon1_coord[0][1], 
                    ICON_SPORTS_ICON1_WIDE, ICON_SPORTS_ICON1_HIGH);
    LCD_DriverWriteDataFromExtFlash(sports_sports_icon1_addr[index % 2], ICON_SPORTS_ICON1_SIZE);  
    
    // �˶�ģʽ��λ
    if((index == 0)||(language_save != language))
    {        
        //����
        LCD_SetPosition(sports_sports_icon2_coord[0][0], sports_sports_icon2_coord[0][1], 
                        ICON_SPORTS_ICON2_WIDE, ICON_SPORTS_ICON2_HIGH);
        LCD_DriverWriteDataFromExtFlash(sports_sports_icon2_addr[language % 2], ICON_SPORTS_ICON2_SIZE);       
    }
    
    *cls_flag = 2;
}

/****************************************************************
** Function name:           Gui_PageRunModeSelect
** Descriptions:            ���˶�ѡ��ģʽ
** input parameters:        index����̬ͼ��ļ���
                            language: ���ԣ�0���ġ�1Ӣ��
** output parameters:       
** Returned value:          
** Created by:              WJ              
** Created Date:            2017-08-11
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/
void Gui_PageRunModeSelect(uint16_t index, uint8_t page, uint8_t num, uint8_t* switch_tmp, uint8_t language)
{
    #if TEST_CLEAR_SCREEN_PARAM
    if(index == 0)
    {
        Gui_PageClearBackground(0,0,LCD_MAX_COLUMN,LCD_MAX_ROW,0x00F8);
    }
    #endif //TEST_CLEAR_SCREEN_PARAM
    
    if(index == 0)
    {
        Gui_PageClearBackground(0, 0, MAX_X_SIZE, MAX_Y_SIZE);
        //�ұ߻�����
        LCD_SetPosition(sports1_icon4_coord[0][0], sports1_icon4_coord[0][1], 
                             ICON_SPORTS1_ICON4_WIDE, ICON_SPORTS1_ICON4_HIGH);
        LCD_DriverWriteDataFromExtFlash(sports1_icon4_addr[page % 4], ICON_SPORTS1_ICON4_SIZE);
        
        for(uint8_t i = 0; i < 4; i++)
        {
            if(page % 4 == 3 && i == 2)
            {
                break;
            }
            //���˶�UI
            LCD_SetPosition(sports1_icon1_coord[i][0], sports1_icon1_coord[i][1], 
                                ICON_SPORTS1_ICON1_WIDE, ICON_SPORTS1_ICON1_HIGH);
            LCD_DriverWriteDataFromExtFlash(sports1_icon1_addr[(page % 4)*4 + i], ICON_SPORTS1_ICON1_SIZE);
            //���˶�����
            LCD_SetPosition(sports1_icon2_coord[i][0], sports1_icon2_coord[i][1], 
                                ICON_SPORTS1_ICON2_WIDE, ICON_SPORTS1_ICON2_HIGH);
            LCD_DriverWriteDataFromExtFlash(sports1_icon2_addr[language % 2][(page % 4)*4 + i], ICON_SPORTS1_ICON2_SIZE);
        }
    } 
    
    if(num > 0)
    {
        for(uint8_t i = 0; i < num; i++)
        {
            Gui_PageClearBackground(sports1_icon3_coord[i % 4][0], sports1_icon3_coord[i % 4][1], 
                                    ICON_SPORTS1_ICON3_WIDE, ICON_SPORTS1_ICON3_HIGH);
        }
    }
    
    LCD_SetPosition(sports1_icon3_coord[num % 4][0], sports1_icon3_coord[num % 4][1], 
                        ICON_SPORTS1_ICON3_WIDE, ICON_SPORTS1_ICON3_HIGH);
    LCD_DriverWriteDataFromExtFlash(sports1_icon3_addr[0], ICON_MORE2_ICON1_SIZE);
    
//    for(uint8_t i = 0; i < 3; i++)
//    {
//        if(page % 3 == 2 && i == 2)
//        {
//            break;
//        }
//        LCD_SetPosition(icon_16_switch_icon2_coord[i][0], icon_16_switch_icon2_coord[i][1], 
//                             ICON_MORE2_ICON2_WIDE, ICON_MORE2_ICON2_HIGH);
//        LCD_DriverWriteDataFromExtFlash(icon_16_switch_icon2_addr[switch_tmp[i]%2], ICON_MORE2_ICON2_SIZE);
//    }
}
/****************************************************************
** Function name:           Gui_PageRunModeData
** Descriptions:            ��ʾ�˶�ģʽ���ݽ���
** input parameters:        seconds���˶�ʱ�䣻heart�����ʣ�calories����·�
                            distance�����룻step���Ʋ���
                            uint_type��0,km��1��mi
                            language: ���ԣ�0���ġ�1Ӣ��
** output parameters:       
** Returned value:          
** Created by:              WJ              
** Created Date:            2017-08-11
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Gui_PageRunModeData(uint16_t index, uint32_t seconds, uint8_t heart, float distance, float calories, 
                         uint32_t step_cnt, uint8_t uint_type, uint8_t animation_flag, uint8_t language, uint8_t *cls_flag, 
                         uint8_t runmode_type_show, uint8_t face_num, uint8_t runmode_type, uint8_t hour, uint8_t minute,
                         uint8_t Operation_state, uint8_t run_state, uint32_t Speed_distance)
{	
    #if TEST_CLEAR_SCREEN_PARAM
    if(index == 0)
    {
        Gui_PageRefreshBackground(0,0,LCD_MAX_COLUMN,LCD_MAX_ROW,0x00F8);
    }
    #endif //TEST_CLEAR_SCREEN_PARAM
    
    static uint8_t last_language = 0xFF;
    static uint8_t last_heart = 0;
    static uint32_t last_seconds = 0;
    static float last_calories = 0;
    static uint32_t last_step_cnt = 0;   
    static uint16_t heart_index = 0;
    static uint8_t last_run_state = 0xFF;
    static uint8_t last_Operation_state = 0xFF;
    static uint8_t last_face_num = 0XFF;
    static float  last_distance = 0.0f;
    static  uint32_t  last_Speed_distance = 0;
    if(index == 0 || last_face_num != face_num || last_language != language)
    {
        Gui_PageClearBackground(0, 0, MAX_X_SIZE, MAX_Y_SIZE);
    }
    if(face_num == 1 || ( runmode_type_show == 1 && face_num == 2))
    {
        if(last_language != language || index == 0)
        {
            //�˶��ı���
            LCD_SetPosition(sports2_icon1_coord[0][0], sports2_icon1_coord[0][1], 
                            ICON_SPORTS2_ICON1_WIDE, ICON_SPORTS2_ICON1_HIGH);
            LCD_DriverWriteDataFromExtFlash(sports2_icon1_addr[0], ICON_SPORTS2_ICON1_SIZE);
            
            //�˶�������������
            LCD_SetPosition(sports2_icon2_coord[0][0], sports2_icon2_coord[0][1], 
                            ICON_SPORTS2_ICON2_WIDE, ICON_SPORTS2_ICON2_HIGH);
            LCD_DriverWriteDataFromExtFlash(sports2_icon2_addr[runmode_type - 1], ICON_SPORTS2_ICON2_SIZE);
            
            LCD_SetPosition(sports2_icon3_coord[0][0], sports2_icon3_coord[0][1], 
                            ICON_SPORTS2_ICON3_WIDE, ICON_SPORTS2_ICON3_HIGH);
            LCD_DriverWriteDataFromExtFlash(sports2_icon3_addr[language%2][runmode_type - 1], ICON_SPORTS2_ICON3_SIZE);
        }
        
        uint8_t system_data[4] = {0};
           
        system_data[0] = hour / 10;
        system_data[1] = hour % 10;
        system_data[2] = minute  / 10;
        system_data[3] = minute % 10;

            
        // ϵͳʱ��
        for(uint32_t i = 0; i < 4; i++)
        {
            LCD_SetPosition(sports2_data1_coord[i][0], sports2_data1_coord[i][1], 
                            ICON_SPORTS2_DATA1_WIDE, ICON_SPORTS2_DATA1_HIGH);
            LCD_DriverWriteDataFromExtFlash(sports2_data1_addr[system_data[i]], ICON_SPORTS2_DATA1_SIZE);
        }
        LCD_SetPosition(sports2_data1dot_coord[0][0], sports2_data1dot_coord[0][1], 
                        ICON_SPORTS2_DATA1DOT_WIDE, ICON_SPORTS2_DATA1DOT_HIGH);
        LCD_DriverWriteDataFromExtFlash(sports2_data1dot_addr[0], ICON_SPORTS2_DATA1DOT_SIZE);
        // �˶�ʱ��
        if(last_seconds != seconds || index == 0 || last_face_num != face_num) 
        {
            uint8_t hour1 = 0;
            uint8_t minute1 = 0;
            uint8_t second1 = 0;
            uint8_t timing_data[6] = {0};
            
            last_seconds = seconds;
            
            hour1   = seconds / 60 / 60;
            minute1 = (seconds / 60 ) % 60;
            second1 = seconds % 60;
            timing_data[0] = (hour1 % 100) / 10;
            timing_data[1] = hour1 % 10;
            timing_data[2] = (minute1 % 100) / 10;
            timing_data[3] = minute1 % 10;
            timing_data[4] = (second1 % 100) / 10;
            timing_data[5] = second1 % 10;
            
            if(face_num == 1)
            {
                // �˶�ʱ��
                for(uint32_t i = 0; i < 6; i++)
                {
                    // ��ʾ������������ 
                    LCD_SetPosition(sports2_data2_coord[i][0], sports2_data2_coord[i][1], 
                                    ICON_SPORTS2_DATA2_WIDE, ICON_SPORTS2_DATA2_HIGH);
                    LCD_DriverWriteDataFromExtFlash(sports2_data2_addr[timing_data[i]], ICON_SPORTS2_DATA2_SIZE);
                }
                
                // �˶�ʱ��ð��
                for(uint32_t i = 0; i < 2; i++)
                {
                    // ��ʾ�����������ֵĵ� 
                    LCD_SetPosition(sports2_data2dot_coord[i][0], sports2_data2dot_coord[i][1], 
                                    ICON_SPORTS2_DATA2DOT_WIDE, ICON_SPORTS2_DATA2DOT_HIGH);
                    LCD_DriverWriteDataFromExtFlash(sports2_data2dot_addr[0], ICON_SPORTS2_DATA2DOT_SIZE);
                } 
            }
            else if(runmode_type_show == 1 && face_num == 2)
            {
                 // �˶�ʱ��
                for(uint32_t i = 0; i < 6; i++)
                {
                    // ��ʾ������������ 
                    LCD_SetPosition(sports2_data2_page2_coord[i][0], sports2_data2_page2_coord[i][1], 
                                    ICON_SPORTS2_DATA2_WIDE, ICON_SPORTS2_DATA2_HIGH);
                    LCD_DriverWriteDataFromExtFlash(sports2_data2_addr[timing_data[i]], ICON_SPORTS2_DATA2_SIZE);
                }
                
                // �˶�ʱ��ð��
                for(uint32_t i = 0; i < 2; i++)
                {
                    // ��ʾ�����������ֵĵ� 
                    LCD_SetPosition(sports2_data2dot_page2_coord[i][0], sports2_data2dot_page2_coord[i][1], 
                                    ICON_SPORTS2_DATA2DOT_WIDE, ICON_SPORTS2_DATA2DOT_HIGH);
                    LCD_DriverWriteDataFromExtFlash(sports2_data2dot_addr[0], ICON_SPORTS2_DATA2DOT_SIZE);
                }
            }

        }
    }
    if(face_num == 1)
    {
        if(runmode_type_show == 0)
        {
            //�ұ߻����� 
            LCD_SetPosition(sports2_icon4_coord[0][0], sports2_icon4_coord[0][1], 
                            ICON_SPORTS2_ICON4_WIDE, ICON_SPORTS2_ICON4_HIGH);
            LCD_DriverWriteDataFromExtFlash(sports2_icon4_addr[0], ICON_SPORTS2_ICON4_SIZE);
        }
        else
        {
            //�ұ߻����� 
            LCD_SetPosition(sports2_icon4_coord[0][0], sports2_icon4_coord[0][1], 
                            ICON_SPORTS2_ICON5_WIDE, ICON_SPORTS2_ICON5_HIGH);
            LCD_DriverWriteDataFromExtFlash(sports2_icon5_addr[0], ICON_SPORTS2_ICON5_SIZE); 
        }
        if(last_language != language || index == 0)
        {
            //����UI 
            LCD_SetPosition(sports2_icon6_coord[0][0], sports2_icon6_coord[0][1], 
                            ICON_SPORTS2_ICON6_WIDE, ICON_SPORTS2_ICON6_HIGH);
            LCD_DriverWriteDataFromExtFlash(sports2_icon6_addr[0], ICON_SPORTS2_ICON6_SIZE);
            //����������ʾ 
            LCD_SetPosition(sports2_icon6_coord[0][0], sports2_icon6_coord[0][1], 
                            ICON_SPORTS2_ICON7_WIDE, ICON_SPORTS2_ICON7_HIGH);
            LCD_DriverWriteDataFromExtFlash(sports2_icon7_addr[language%2][0], ICON_SPORTS2_ICON7_SIZE);
            //���� | 
            LCD_SetPosition(sports2_icon10_coord[0][0], sports2_icon10_coord[0][1], 
                            ICON_SPORTS2_ICON10_WIDE, ICON_SPORTS2_ICON10_HIGH);
            LCD_DriverWriteDataFromExtFlash(sports2_icon10_addr[0], ICON_SPORTS2_ICON10_SIZE);
            
            //���� ��λ 
            LCD_SetPosition(sports2_unit1_coord[0][0], sports2_unit1_coord[0][1], 
                            ICON_SPORTS2_UNIT1_WIDE, ICON_SPORTS2_UNIT1_HIGH);
            LCD_DriverWriteDataFromExtFlash(sports2_unit1_addr[language%2][0], ICON_SPORTS2_UNIT1_SIZE); 
        }
        if(last_heart != heart || index == 0)
        {
            if(heart > 255)
            {
                heart = 255;
            }
            if(heart > 0)
            {
                // ������ֵ                  
                uint8_t  heart_buff[3] = {0};
                if(heart > 255)
                {
                    heart = 255;
                }
                heart_buff[0] = (heart / 100) %10;
                heart_buff[1] = (heart / 10) %10;
                heart_buff[2] = (heart / 10) %10;
                for(uint8_t i = 0;i < 3;i++)
                {     
                     LCD_SetPosition(sports2_hear_coord[i][0], sports2_hear_coord[i][1], 
                                    ICON_SPORTS2_DATA3_WIDE, ICON_SPORTS2_DATA3_HIGH);
                     LCD_DriverWriteDataFromExtFlash(sports2_data3_addr[heart_buff[i]%10], ICON_SPORTS2_DATA3_SIZE);
                } 
            }
            else
            {
                for(uint8_t i = 0;i < 3;i++)
                {
                     LCD_SetPosition(sports2_hear_coord[i][0], sports2_hear_coord[i][1], 
                                    ICON_SPORTS2_DATA3DOT1_WIDE, ICON_SPORTS2_DATA3DOT1_HIGH);
                     LCD_DriverWriteDataFromExtFlash(sports2_data3dot1_addr[0], ICON_SPORTS2_DATA3DOT1_SIZE);
                }
            }

        }
        if(last_language != language || index == 0)
        {
            //��·��UI 
            LCD_SetPosition(sports2_icon6_coord[1][0], sports2_icon6_coord[1][1], 
                            ICON_SPORTS2_ICON6_WIDE, ICON_SPORTS2_ICON6_HIGH);
            LCD_DriverWriteDataFromExtFlash(sports2_icon6_addr[1], ICON_SPORTS2_ICON6_SIZE);
            //��·��������ʾ 
            LCD_SetPosition(sports2_icon6_coord[1][0], sports2_icon6_coord[1][1], 
                            ICON_SPORTS2_ICON7_WIDE, ICON_SPORTS2_ICON7_HIGH);
            LCD_DriverWriteDataFromExtFlash(sports2_icon7_addr[language%2][1], ICON_SPORTS2_ICON7_SIZE);
            //��·�� | 
            LCD_SetPosition(sports2_icon10_coord[1][0], sports2_icon10_coord[1][1], 
                            ICON_SPORTS2_ICON10_WIDE, ICON_SPORTS2_ICON10_HIGH);
            LCD_DriverWriteDataFromExtFlash(sports2_icon10_addr[1], ICON_SPORTS2_ICON10_SIZE);
            
            //��·�� ��λ 
            LCD_SetPosition(sports2_unit1_coord[1][0], sports2_unit1_coord[1][1], 
                            ICON_SPORTS2_UNIT2_WIDE, ICON_SPORTS2_UNIT2_HIGH);
            LCD_DriverWriteDataFromExtFlash(sports2_unit1_addr[language%2][1], ICON_SPORTS2_UNIT2_SIZE); 
        }
        // ��·����ֵ
        if(last_calories != calories || index == 0)
        {
            uint32_t calories_tmp = 0;
            uint32_t calories_buff[4]  = {0};
            calories_tmp = (uint32_t)calories;
            if(calories_tmp > 9999)
            {
                calories = 9999;
            }
            calories_buff[0] = ((uint32_t)calories / 1000) %10;
            calories_buff[1] = ((uint32_t)calories / 100) %10;
            calories_buff[2] = ((uint32_t)calories / 10) %10;
            calories_buff[3] = ((uint32_t)calories / 1) %10;
            for(uint8_t i = 0;i < 4;i++)
            {
                LCD_SetPosition(sports2_calories_coord[i][0], sports2_calories_coord[i][1], 
                                ICON_SPORTS2_DATA3_WIDE, ICON_SPORTS2_DATA3_HIGH);
                LCD_DriverWriteDataFromExtFlash(sports2_data3_addr[calories_buff[i]%10], ICON_SPORTS2_DATA3_SIZE);                
            }
        }
        
    }
    if((runmode_type_show == 0 && face_num == 2) || (runmode_type_show == 1 && face_num == 3))
    {
        if((index == 0 || last_language != language || last_run_state != run_state || last_Operation_state != Operation_state)
            && runmode_type_show == 0)
        {
            //�ұ߻����� 
            LCD_SetPosition(sports2_icon4_coord[0][0], sports2_icon4_coord[0][1], 
                            ICON_SPORTS2_ICON4_WIDE, ICON_SPORTS2_ICON4_HIGH);
            LCD_DriverWriteDataFromExtFlash(sports2_icon4_addr[1], ICON_SPORTS2_ICON4_SIZE);
            //��������
            for(uint8_t i = 0; i < 3; i++)
            {
                LCD_SetPosition(sports3_icon2_coord[i][0], sports3_icon2_coord[i][1], 
                            ICON_SPORTS3_ICON2_WIDE, ICON_SPORTS3_ICON2_HIGH);                 
                LCD_DriverWriteDataFromExtFlash(sports3_icon2_addr[language%2][run_state%2][Operation_state%3][i], ICON_SPORTS3_ICON2_SIZE); 
            }  
        }
        else if((index == 0 || last_language != language || last_run_state != run_state || last_Operation_state != Operation_state)
            && runmode_type_show == 1)
        {
            //�ұ߻����� 
            LCD_SetPosition(sports2_icon4_coord[0][0], sports2_icon4_coord[0][1], 
                            ICON_SPORTS2_ICON5_WIDE, ICON_SPORTS2_ICON5_HIGH);
            LCD_DriverWriteDataFromExtFlash(sports2_icon5_addr[2], ICON_SPORTS2_ICON5_SIZE); 
            //��������
            for(uint8_t i = 0; i < 3; i++)
            {
                LCD_SetPosition(sports3_icon2_coord[i][0], sports3_icon2_coord[i][1], 
                            ICON_SPORTS3_ICON2_WIDE, ICON_SPORTS3_ICON2_HIGH);                 
                LCD_DriverWriteDataFromExtFlash(sports3_icon2_addr[language%2][run_state%2][Operation_state%3][i], ICON_SPORTS3_ICON2_SIZE); 
            }  
        }
            
    }
    if(runmode_type_show == 1 && face_num == 2)
    {
       if(index == 0)
       {
            //�ұ߻����� 
            LCD_SetPosition(sports2_icon4_coord[0][0], sports2_icon4_coord[0][1], 
                            ICON_SPORTS2_ICON5_WIDE, ICON_SPORTS2_ICON5_HIGH);
            LCD_DriverWriteDataFromExtFlash(sports2_icon5_addr[1], ICON_SPORTS2_ICON5_SIZE); 
       }
       if(index == 0 || last_language != language)
       {
           for(uint8_t i = 0;i < 3;i++)
           {
                //�Ʋ�����̡�����UI
                LCD_SetPosition(sports2_icon8_coord[i][0], sports2_icon8_coord[i][1], 
                                ICON_SPORTS2_ICON8_WIDE, ICON_SPORTS2_ICON8_HIGH);
                LCD_DriverWriteDataFromExtFlash(sports2_icon8_addr[i], ICON_SPORTS2_ICON8_SIZE);
               
                //�Ʋ�����̡���������
                LCD_SetPosition(sports2_icon9_coord[i][0], sports2_icon9_coord[i][1], 
                                ICON_SPORTS2_ICON9_WIDE, ICON_SPORTS2_ICON9_HIGH);
                LCD_DriverWriteDataFromExtFlash(sports2_icon9_addr[language%2][i], ICON_SPORTS2_ICON9_SIZE);
           }
           
            //�Ʋ�����̡����ٵ�λ
            LCD_SetPosition(sports2_unit3_coord[0][0], sports2_unit3_coord[0][1], 
                            ICON_SPORTS2_UNIT3_WIDE, ICON_SPORTS2_UNIT3_HIGH);
            LCD_DriverWriteDataFromExtFlash(sports2_unit3_addr[language%2], ICON_SPORTS2_UNIT3_SIZE);
           
            LCD_SetPosition(sports2_unit4_coord[0][0], sports2_unit4_coord[0][1], 
                            ICON_SPORTS2_UNIT4_WIDE, ICON_SPORTS2_UNIT4_HIGH);
            LCD_DriverWriteDataFromExtFlash(sports2_unit4_addr[language%2][uint_type%2], ICON_SPORTS2_UNIT4_SIZE);
           
            LCD_SetPosition(sports2_unit5_coord[0][0], sports2_unit5_coord[0][1], 
                            ICON_SPORTS2_UNIT5_WIDE, ICON_SPORTS2_UNIT5_HIGH);
            LCD_DriverWriteDataFromExtFlash(sports2_unit5_addr[language%2], ICON_SPORTS2_UNIT5_SIZE);
  
       }
        if( last_step_cnt != step_cnt || index == 0 || last_language != language)
        {
            //�Ʋ�
            uint32_t  step_cnt_buff[5] = {0};
            if(step_cnt > 99999)
            {
                step_cnt = 99999;
            }
            step_cnt_buff[0] =  (step_cnt / 10000) %10;
            step_cnt_buff[1] =  (step_cnt / 1000) %10;
            step_cnt_buff[2] =  (step_cnt / 100) %10;
            step_cnt_buff[3] =  (step_cnt / 10) %10;
            step_cnt_buff[4] =  (step_cnt /1) %10;
            for(uint8_t i = 0; i < 5; i++)
            {
                LCD_SetPosition(sports2_step_coord[i][0], sports2_step_coord[i][1], 
                                ICON_SPORTS2_DATA3_WIDE, ICON_SPORTS2_DATA3_HIGH);
                LCD_DriverWriteDataFromExtFlash(sports2_data3_addr[step_cnt_buff[i]%10], ICON_SPORTS2_DATA3_SIZE);
            }
        }
        //���    
        if( last_distance != distance || index == 0)
        {
            uint32_t  distance_disp[3] = {0};
            if(distance > 999)
            {
                distance = 999;
            }
            if(distance < 100)
            {
                distance = (uint32_t)(distance* 10);
            }
            distance_disp[0] = (uint32_t)distance/100%10;
            distance_disp[1] = (uint32_t)distance/10%10;
            distance_disp[2] = (uint32_t)distance%10;
            for(uint8_t i = 0; i < 3; i++)
            {
                LCD_SetPosition(sports2_mileage_coord[i][0], sports2_mileage_coord[i][1], 
                                ICON_SPORTS2_DATA3_WIDE, ICON_SPORTS2_DATA3_HIGH);
                LCD_DriverWriteDataFromExtFlash(sports2_data3_addr[distance_disp[i]%10], ICON_SPORTS2_DATA3_SIZE);
            }
            LCD_SetPosition(sports2_data3dot2_coord[0][0], sports2_data3dot2_coord[0][1], 
                            ICON_SPORTS2_DATA3DOT2_WIDE, ICON_SPORTS2_DATA3DOT2_HIGH);
            LCD_DriverWriteDataFromExtFlash(sports2_data3dot2_addr[0], ICON_SPORTS2_DATA3DOT2_SIZE);
        }
        if( last_Speed_distance != Speed_distance || index == 0)
        {
            last_Speed_distance = Speed_distance;
            uint8_t Speed_min = 0;
            uint8_t Speed_seconds = 0;
            uint8_t Speed_distance_buff[4] = {0};
            
            Speed_min = (seconds / 60 / Speed_distance);
            Speed_seconds = (seconds / Speed_distance);
            if(Speed_min > 99)
            {
               Speed_min = 99;
            }
            if(Speed_seconds > 99)
            {
               Speed_seconds = 99;
            }
            Speed_distance_buff[0] = Speed_min/ 10;   // ���ӵ��ٶ�
            Speed_distance_buff[1] = Speed_min % 10; 

            Speed_distance_buff[2] = Speed_seconds/ 10;        // ����ٶ�
            Speed_distance_buff[3] = Speed_seconds % 10; 
            for(uint8_t i = 0; i < 4; i++)
            {
                LCD_SetPosition(sports2_speed_coord[i][0], sports2_speed_coord[i][1], 
                                ICON_SPORTS2_DATA3_WIDE, ICON_SPORTS2_DATA3_HIGH);
                LCD_DriverWriteDataFromExtFlash(sports2_data3_addr[Speed_distance_buff[i]%10], ICON_SPORTS2_DATA3_SIZE);
            }
            
            LCD_SetPosition(sports2_data3dot3_coord[0][0], sports2_data3dot3_coord[0][1], 
                            ICON_SPORTS2_DATA3DOT3_WIDE, ICON_SPORTS2_DATA3DOT3_HIGH);
            LCD_DriverWriteDataFromExtFlash(sports2_data3dot3_addr[0], ICON_SPORTS2_DATA3DOT3_SIZE);
            LCD_SetPosition(sports2_data3dot3_coord[1][0], sports2_data3dot3_coord[1][1], 
                            ICON_SPORTS2_DATA3DOT4_WIDE, ICON_SPORTS2_DATA3DOT4_HIGH);
            LCD_DriverWriteDataFromExtFlash(sports2_data3dot4_addr[0], ICON_SPORTS2_DATA3DOT4_SIZE);
        }
        
    }
    last_heart = heart;                                                         
    last_calories = calories;
    last_face_num = face_num;
    last_language = language;
    last_run_state = run_state;
    last_step_cnt = step_cnt;
    last_distance = distance;
    last_Operation_state = Operation_state; 
}
/****************************************************************
** Function name:           Gui_PageRunModeDataResult
** Descriptions:            ��ʾ�˶�ģʽ��������
** input parameters:        seconds���˶�ʱ�䣻heart�����ʣ�calories����·�
                            distance�����룻step���Ʋ���
                            uint_type��0,km��1��mi
                            language: ���ԣ�0���ġ�1Ӣ��
** output parameters:       
** Returned value:          
** Created by:              WJ              
** Created Date:            2017-08-11
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
// �˶�ģ��������
void Gui_PageRunModeDataResult(uint16_t index, uint8_t data_valid, uint32_t seconds, uint8_t heart, float distance, float calories, 
                                uint32_t step_cnt, uint8_t uint_type, bool lock, uint8_t hour, uint8_t minute, uint8_t animation_flag, 
                                uint8_t language, uint8_t runmode_type, uint8_t runmode_show_type, uint8_t page, uint32_t Speed_distance)
{
    static uint8_t last_language = 0XFF;
    if(index == 0 || last_language != language)
    {
        Gui_PageClearBackground(0, 0, MAX_X_SIZE, MAX_Y_SIZE);
    }
    
	if(data_valid)
	{
        if(index == 0)
        {
            uint8_t hour = 0;
            uint8_t minute = 0;
            uint8_t second = 0;
            uint8_t timing_data[6] = {0};

            hour   = seconds / 60 / 60;
            minute = (seconds / 60 ) % 60;
            second = seconds % 60;
            timing_data[0] = (hour % 100) / 10;
            timing_data[1] = hour % 10;
            timing_data[2] = (minute % 100) / 10;
            timing_data[3] = minute % 10;
            timing_data[4] = (second % 100) / 10;
            timing_data[5] = second % 10;
            
            if(heart > 255)
            {
                heart = 255;
            }
            // ������ֵ                  
            uint8_t  heart_buff[3] = {0};
            heart_buff[0] = (heart / 100) %10;
            heart_buff[1] = (heart / 10) %10;
            heart_buff[2] = (heart / 10) %10;
       
            uint32_t calories_tmp = 0;
            uint32_t calories_buff[4]  = {0};
            calories_tmp = (uint32_t)calories;
            if(calories_tmp > 9999)
            {
                calories = 9999;
            }
            calories_buff[0] = ((uint32_t)calories / 1000) %10;
            calories_buff[1] = ((uint32_t)calories / 100) %10;
            calories_buff[2] = ((uint32_t)calories / 10) %10;
            calories_buff[3] = ((uint32_t)calories / 1) %10;
            
            if(runmode_show_type == 0)
            {
                /**********************2�������˶�չʾ���� -- static -- sports4_unit**********************/
                for(uint8_t i = 0; i < 3; i++)    //UI
                {
                    LCD_SetPosition(sports4_icon1_page1_coord[i][0], sports4_icon1_page1_coord[i][1], 
                                ICON_SPORTS4_ICON1_WIDE, ICON_SPORTS4_ICON1_HIGH);
                    LCD_DriverWriteDataFromExtFlash(sports4_icon1_addr[i], ICON_SPORTS4_ICON1_SIZE); 
                }
                //��λ - ���ʵ�λ
                LCD_SetPosition(sports4_unit1_page1_coord[0][0], sports4_unit1_page1_coord[0][1], 
                                ICON_SPORTS4_UNIT1_WIDE, ICON_SPORTS4_UNIT1_HIGH);
                LCD_DriverWriteDataFromExtFlash(sports4_unit1_page1_addr[0][language%2], ICON_SPORTS4_UNIT1_SIZE);
                //��λ - ��·��
                LCD_SetPosition(sports4_unit1_page1_coord[1][0], sports4_unit1_page1_coord[1][1], 
                                ICON_SPORTS4_UNIT2_WIDE, ICON_SPORTS4_UNIT2_HIGH);
                LCD_DriverWriteDataFromExtFlash(sports4_unit1_page1_addr[1][language%2], ICON_SPORTS4_UNIT2_SIZE);
                for(uint8_t i = 0; i < 6; i++)    //ʱ��
                {
                    LCD_SetPosition(sports4_data1_page1_coord[i][0], sports4_data1_page1_coord[i][1], 
                                ICON_SPORTS4_DATA1_WIDE, ICON_SPORTS4_DATA1_HIGH);
                    LCD_DriverWriteDataFromExtFlash(sports4_data1_addr[timing_data[i] % 10], ICON_SPORTS4_DATA1_SIZE); 
                }
                for(uint8_t i = 0; i < 2; i++)    //ʱ�� :
                {
                    LCD_SetPosition(sports4_data1dot1_page1_coord[i][0], sports4_data1dot1_page1_coord[i][1], 
                                ICON_SPORTS4_DATA1DOT1_WIDE, ICON_SPORTS4_DATA1DOT1_HIGH);
                    LCD_DriverWriteDataFromExtFlash(sports4_data1dot1_addr[0], ICON_SPORTS4_DATA1DOT1_SIZE); 
                }
                for(uint8_t i = 0; i < 3; i++)    //����
                {
                    LCD_SetPosition(sports4_hear_page1_coord[i][0], sports4_hear_page1_coord[i][1], 
                                ICON_SPORTS4_DATA1_WIDE, ICON_SPORTS4_DATA1_HIGH);
                    LCD_DriverWriteDataFromExtFlash(sports4_data1_addr[heart_buff[i] % 10], ICON_SPORTS4_DATA1_SIZE); 
                }
                for(uint8_t i = 0; i < 4; i++)    //��·��
                {
                    LCD_SetPosition(sports4_Calorie_page1_coord[i][0], sports4_Calorie_page1_coord[i][1], 
                                ICON_SPORTS4_DATA1_WIDE, ICON_SPORTS4_DATA1_HIGH);
                    LCD_DriverWriteDataFromExtFlash(sports4_data1_addr[calories_buff[i] % 10], ICON_SPORTS4_DATA1_SIZE); 
                }
                for(uint8_t i = 0; i < 3; i++)    //UI����ĺ���
                {
                    LCD_SetPosition(sports4_icon2_page1_coord[i][0], sports4_icon2_page1_coord[i][1], 
                                ICON_SPORTS4_ICON2_WIDE, ICON_SPORTS4_ICON2_HIGH);
                    LCD_DriverWriteDataFromExtFlash(sports4_icon2_addr[0], ICON_SPORTS4_ICON2_SIZE); 
                }
                
               
            }
            else if(runmode_show_type == 1)
            {
                 uint32_t  distance_disp[3] = {0};
                if(distance > 999)
                {
                    distance = 999;
                }
                if(distance < 100)
                {
                    distance = (uint32_t)(distance* 10);
                }
                distance_disp[0] = (uint32_t)distance/100%10;
                distance_disp[1] = (uint32_t)distance/10%10;
                distance_disp[2] = (uint32_t)distance%10;
                
                uint32_t step_cnt_buff[5]  = {0};
                if(step_cnt > 99999)
                {
                    step_cnt = 99999;
                }
                step_cnt_buff[0] = ((uint32_t)step_cnt / 10000) %10;
                step_cnt_buff[1] = ((uint32_t)step_cnt / 1000) %10;
                step_cnt_buff[2] = ((uint32_t)step_cnt / 100) %10;
                step_cnt_buff[3] = ((uint32_t)step_cnt / 10) %10;
                step_cnt_buff[4] = ((uint32_t)step_cnt / 1) %10;
                
                int8_t Speed_min = 0;
                uint8_t Speed_seconds = 0;
                uint8_t Speed_distance_buff[4] = {0};
                
                Speed_min = (seconds / 60 / Speed_distance);
                Speed_seconds = (seconds / Speed_distance);
                if(Speed_min > 99)
                {
                   Speed_min = 99;
                }
                if(Speed_seconds > 99)
                {
                   Speed_seconds = 99;
                }
                Speed_distance_buff[0] = Speed_min/ 10;   // ���ӵ��ٶ�
                Speed_distance_buff[1] = Speed_min % 10; 

                Speed_distance_buff[2] = Speed_seconds/ 10;        // ����ٶ�
                Speed_distance_buff[3] = Speed_seconds % 10; 
                
                /**********************5�������˶�չʾ���� -- static -- sports4_unit**********************/
                for(uint8_t i = 0; i < 6; i++)    //UI
                {
                    LCD_SetPosition(sports4_icon1_page2_coord[i][0], sports4_icon1_page2_coord[i][1], 
                                ICON_SPORTS4_ICON1_WIDE, ICON_SPORTS4_ICON1_HIGH);
                    LCD_DriverWriteDataFromExtFlash(sports4_icon1_addr[i], ICON_SPORTS4_ICON1_SIZE); 
                }
                //��λ - ���ʵ�λ
                LCD_SetPosition(sports4_unit1_page2_coord[0][0], sports4_unit1_page2_coord[0][1], 
                                ICON_SPORTS4_UNIT1_WIDE, ICON_SPORTS4_UNIT1_HIGH);
                LCD_DriverWriteDataFromExtFlash(sports4_unit1_page2_addr[0][language%2], ICON_SPORTS4_UNIT1_SIZE);
                //��λ - ��·��
                LCD_SetPosition(sports4_unit1_page2_coord[1][0], sports4_unit1_page2_coord[1][1], 
                                ICON_SPORTS4_UNIT2_WIDE, ICON_SPORTS4_UNIT2_HIGH);
                LCD_DriverWriteDataFromExtFlash(sports4_unit1_page2_addr[1][language%2], ICON_SPORTS4_UNIT2_SIZE);
                //��λ - �Ʋ�
                LCD_SetPosition(sports4_unit1_page2_coord[2][0], sports4_unit1_page2_coord[2][1], 
                                ICON_SPORTS4_UNIT3_WIDE, ICON_SPORTS4_UNIT3_HIGH);
                LCD_DriverWriteDataFromExtFlash(sports4_unit1_page2_addr[2][language%2], ICON_SPORTS4_UNIT3_SIZE);
                //��λ - ����
                LCD_SetPosition(sports4_unit1_page2_coord[3][0], sports4_unit1_page2_coord[3][1], 
                                ICON_SPORTS4_UNIT4_WIDE, ICON_SPORTS4_UNIT4_HIGH);
                LCD_DriverWriteDataFromExtFlash(sports4_unit4_page2_addr[uint_type%2][language%2], ICON_SPORTS4_UNIT4_SIZE);            
                //��λ - ����
                LCD_SetPosition(sports4_unit1_page2_coord[4][0], sports4_unit1_page2_coord[4][1], 
                                ICON_SPORTS4_UNIT5_WIDE, ICON_SPORTS4_UNIT5_HIGH);
                LCD_DriverWriteDataFromExtFlash(sports4_unit5_page2_addr[language%2], ICON_SPORTS4_UNIT5_SIZE);
                for(uint8_t i = 0; i < 6; i++)    //ʱ��
                {
                    LCD_SetPosition(sports4_data1_page2_coord[i][0], sports4_data1_page2_coord[i][1], 
                                ICON_SPORTS4_DATA1_WIDE, ICON_SPORTS4_DATA1_HIGH);
                    LCD_DriverWriteDataFromExtFlash(sports4_data1_addr[timing_data[i] % 10], ICON_SPORTS4_DATA1_SIZE); 
                }
                for(uint8_t i = 0; i < 2; i++)    //ʱ�� :
                {
                    LCD_SetPosition(sports4_data1dot1_page2_coord[i][0], sports4_data1dot1_page2_coord[i][1], 
                                ICON_SPORTS4_DATA1DOT1_WIDE, ICON_SPORTS4_DATA1DOT1_HIGH);
                    LCD_DriverWriteDataFromExtFlash(sports4_data1dot1_addr[0], ICON_SPORTS4_DATA1DOT1_SIZE); 
                }
                for(uint8_t i = 0; i < 3; i++)    //����
                {
                    LCD_SetPosition(sports4_hear_page2_coord[i][0], sports4_hear_page2_coord[i][1], 
                                ICON_SPORTS4_DATA1_WIDE, ICON_SPORTS4_DATA1_HIGH);
                    LCD_DriverWriteDataFromExtFlash(sports4_data1_addr[heart_buff[i] % 10], ICON_SPORTS4_DATA1_SIZE); 
                }
                for(uint8_t i = 0; i < 4; i++)    //��·��
                {
                    LCD_SetPosition(sports4_Calorie_page2_coord[i][0], sports4_Calorie_page2_coord[i][1], 
                                ICON_SPORTS4_DATA1_WIDE, ICON_SPORTS4_DATA1_HIGH);
                    LCD_DriverWriteDataFromExtFlash(sports4_data1_addr[calories_buff[i] % 10], ICON_SPORTS4_DATA1_SIZE); 
                }
                for(uint8_t i = 0; i < 5; i++)    //����
                {
                    LCD_SetPosition(sports4_step_page1_coord[i][0], sports4_step_page1_coord[i][1], 
                                ICON_SPORTS4_DATA1_WIDE, ICON_SPORTS4_DATA1_HIGH);
                    LCD_DriverWriteDataFromExtFlash(sports4_data1_addr[step_cnt_buff[i] % 10], ICON_SPORTS4_DATA1_SIZE); 
                }
                for(uint8_t i = 0; i < 3; i++)    //���
                {
                    LCD_SetPosition(sports4_mileage_page1_coord[i][0], sports4_mileage_page1_coord[i][1], 
                                ICON_SPORTS4_DATA1_WIDE, ICON_SPORTS4_DATA1_HIGH);
                    LCD_DriverWriteDataFromExtFlash(sports4_data1_addr[distance_disp[i] % 10], ICON_SPORTS4_DATA1_SIZE); 
                }
                //
                LCD_SetPosition(sports4_data1dot2_coord[0][0], sports4_data1dot2_coord[0][1], 
                            ICON_SPORTS4_DATA1DOT2_WIDE, ICON_SPORTS4_DATA1DOT2_HIGH);
                LCD_DriverWriteDataFromExtFlash(sports4_data1dot2_addr[0], ICON_SPORTS4_DATA1DOT2_SIZE);
                for(uint8_t i = 0; i < 4; i++)    //����
                {
                    LCD_SetPosition(sports4_speed_page1_coord[i][0], sports4_speed_page1_coord[i][1], 
                                ICON_SPORTS4_DATA1_WIDE, ICON_SPORTS4_DATA1_HIGH);
                    LCD_DriverWriteDataFromExtFlash(sports4_data1_addr[Speed_distance_buff[i] % 10], ICON_SPORTS4_DATA1_SIZE); 
                }
                //����' ''
                LCD_SetPosition(sports4_data1dot3_coord[0][0], sports4_data1dot3_coord[0][1], 
                            ICON_SPORTS4_DATA1DOT3_WIDE, ICON_SPORTS4_DATA1DOT3_HIGH);
                LCD_DriverWriteDataFromExtFlash(sports4_data1dot3_addr[0], ICON_SPORTS4_DATA1DOT3_SIZE);
                LCD_SetPosition(sports4_data1dot3_coord[1][0], sports4_data1dot3_coord[1][1], 
                            ICON_SPORTS4_DATA1DOT4_WIDE, ICON_SPORTS4_DATA1DOT4_HIGH);
                LCD_DriverWriteDataFromExtFlash(sports4_data1dot3_addr[1], ICON_SPORTS4_DATA1DOT4_SIZE); 
                for(uint8_t i = 0; i < 5; i++)    //UI����ĺ���
                {
                    LCD_SetPosition(sports4_icon2_page2_coord[i][0], sports4_icon2_page2_coord[i][1], 
                                ICON_SPORTS4_ICON2_WIDE, ICON_SPORTS4_ICON2_HIGH);
                    LCD_DriverWriteDataFromExtFlash(sports4_icon2_addr[0], ICON_SPORTS4_ICON2_SIZE); 
                }
            }
        }
	}
	else
	{
		/**********************���ݲ��� -- static -- sports4_icon3**********************/ 
        if(index == 0)
        {
            LCD_SetPosition(sports4_icon3_coord[0][0], sports4_icon3_coord[0][1], 
                        ICON_SPORTS4_ICON3_WIDE, ICON_SPORTS4_ICON3_HIGH);
            LCD_DriverWriteDataFromExtFlash(sports4_icon3_addr[0], ICON_SPORTS4_ICON3_SIZE);
            LCD_SetPosition(sports4_icon4_coord[0][0], sports4_icon4_coord[0][1], 
                        ICON_SPORTS4_ICON4_WIDE, ICON_SPORTS4_ICON4_HIGH);
            LCD_DriverWriteDataFromExtFlash(sports4_icon4_addr[language%2], ICON_SPORTS4_ICON4_SIZE);
        }
	}





	last_language = language;
	if(index == 0)
    {
        
    }
}
/****************************************************************
** Function name:           Gui_PageRunModePause
** Descriptions:            �˶�ģʽ��ͣ��ʼ����
** input parameters:        pause_state��1��ͣ��0����
                            language: ���ԣ�0���ġ�1Ӣ��
** output parameters:       
** Returned value:          
** Created by:              WJ              
** Created Date:            2017-08-11
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Gui_PageRunModePause(uint16_t index, uint8_t pause_state, uint8_t language, uint8_t *cls_flag)
{	
//    #if TEST_CLEAR_SCREEN_PARAM
//    if(index == 0)
//    {
//        Gui_PageRefreshBackground(0,0,LCD_MAX_COLUMN,LCD_MAX_ROW,0x00F8);
//    }
//    #endif //TEST_CLEAR_SCREEN_PARAM
//    
//    // ͼ���ϲ�����
//    if(index == 0 && *cls_flag != 0)
//    {
//        // �����ϲ�����
//        Gui_PageClearBackground(0, 
//                                0, 
//                                MAX_X_SIZE, 
//                                sport3_move2_icon1_coord[0][1]); 
//        
//        
//        // ����������
//        Gui_PageClearBackground(0, 
//                                0, 
//                                sport3_move2_icon1_coord[0][0], 
//                                MAX_Y_SIZE); 


//        // �����Ҳ�����
//        Gui_PageClearBackground(sport3_move2_icon1_coord[0][0]+ICON_MOVE2_ICON1_WIDE, 
//                                0, 
//                                MAX_X_SIZE - (sport3_move2_icon1_coord[0][0]+ICON_MOVE2_ICON1_WIDE), 
//                                MAX_Y_SIZE); 

//        
//        // �����²�����
//        Gui_PageClearBackground(0, 
//                                sport3_move2_icon1_coord[0][1]+ICON_MOVE2_ICON1_HIGH, 
//                                MAX_X_SIZE, 
//                                MAX_Y_SIZE-(sport3_move2_icon1_coord[0][1]+ICON_MOVE2_ICON1_HIGH));                                  
//    }
//    
//    // ��ǩ��ͼ��
//    if(index == 0)
//    {
//        // ��ߺ���
//        LCD_SetPosition(sport3_move2_icon2_coord[0][0], sport3_move2_icon2_coord[0][1], 
//                        ICON_MOVE2_ICON2_WIDE, ICON_MOVE2_ICON2_HIGH);
//        LCD_DriverWriteDataFromExtFlash(sport3_move2_icon2_addr[0], ICON_MOVE2_ICON2_SIZE);        
//        
//        //��ͣ
//        if(pause_state)
//        {
//            // ͼ��
//            LCD_SetPosition(sport3_move2_icon1_coord[0][0], sport3_move2_icon1_coord[0][1], 
//                            ICON_MOVE2_ICON1_WIDE, ICON_MOVE2_ICON1_HIGH);
//            LCD_DriverWriteDataFromExtFlash(sport3_move2_icon1_addr[0], ICON_MOVE2_ICON1_SIZE);
//            
//            // ���� ��ʼ
//            LCD_SetPosition(sport3_move2_icon3_coord[0][0], sport3_move2_icon3_coord[0][1], 
//                            ICON_MOVE2_ICON3_WIDE, ICON_MOVE2_ICON3_HIGH);
//            LCD_DriverWriteDataFromExtFlash(sport3_move2_icon3_addr[language%2], ICON_MOVE2_ICON3_SIZE);
//            
//        }            
//        else
//        {
//            // ͼ��
//            LCD_SetPosition(sport3_move2_icon1_coord[0][0], sport3_move2_icon1_coord[0][1], 
//                            ICON_MOVE2_ICON1_WIDE, ICON_MOVE2_ICON1_HIGH);
//            LCD_DriverWriteDataFromExtFlash(sport2_move2_icon1_addr[0], ICON_MOVE2_ICON1_SIZE);
//            // ���� �ݶ�
//            LCD_SetPosition(sport3_move2_icon3_coord[0][0], sport3_move2_icon3_coord[0][1], 
//                            ICON_MOVE2_ICON3_WIDE, ICON_MOVE2_ICON3_HIGH);
//            LCD_DriverWriteDataFromExtFlash(sport2_move2_icon3_addr[language%2], ICON_MOVE2_ICON3_SIZE);
//        }
//    }
//    
//    *cls_flag = 1;
}

/****************************************************************
** Function name:           Gui_PageRunModeExit
** Descriptions:            �˶�ģʽ�˳�����
** input parameters:        language: ���ԣ�0���ġ�1Ӣ��
** output parameters:       
** Returned value:          
** Created by:              WJ              
** Created Date:            2017-08-11
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Gui_PageRunModeExit(uint16_t index, uint8_t language, uint8_t *cls_flag)
{
////    #if TEST_CLEAR_SCREEN_PARAM
////    if(index == 0)
////    {
////        Gui_PageRefreshBackground(0,0,LCD_MAX_COLUMN,LCD_MAX_ROW,0x00F8);
////    }
////    #endif //TEST_CLEAR_SCREEN_PARAM
////    
////    // ͼ���ϲ�����
////    if(index == 0 && *cls_flag != 0)
////    {
////        // �����ϲ�����
////        Gui_PageClearBackground(0, 
////                                0, 
////                                MAX_X_SIZE, 
////                                sport4_move2_icon1_coord[0][1]); 
////        
////        
////        // ����������
////        Gui_PageClearBackground(0, 
////                                0, 
////                                sport4_move2_icon1_coord[0][0], 
////                                MAX_Y_SIZE); 


////        // �����Ҳ�����
////        Gui_PageClearBackground(sport4_move2_icon1_coord[0][0]+ICON_MOVE2_ICON1_WIDE, 
////                                0, 
////                                MAX_X_SIZE - (sport4_move2_icon1_coord[0][0]+ICON_MOVE2_ICON1_WIDE), 
////                                MAX_Y_SIZE); 

////        
////        // �����²�����
////        Gui_PageClearBackground(0, 
////                                sport4_move2_icon1_coord[0][1]+ICON_MOVE2_ICON1_HIGH, 
////                                MAX_X_SIZE, 
////                                MAX_Y_SIZE-(sport4_move2_icon1_coord[0][1]+ICON_MOVE2_ICON1_HIGH));                                 
////    }

////    // ��ǩ��ͼ��
////    if(index == 0)
////    {
////        
////        // ��ߺ���
////        LCD_SetPosition(sport4_move2_icon2_coord[0][0], sport4_move2_icon2_coord[0][1], 
////                        ICON_MOVE2_ICON2_WIDE, ICON_MOVE2_ICON2_HIGH);
////        LCD_DriverWriteDataFromExtFlash(sport4_move2_icon2_addr[0], ICON_MOVE2_ICON2_SIZE);        
////        
////        // ͼ��
////        LCD_SetPosition(sport4_move2_icon1_coord[0][0], sport4_move2_icon1_coord[0][1], 
////                        ICON_MOVE2_ICON1_WIDE, ICON_MOVE2_ICON1_HIGH);
////        LCD_DriverWriteDataFromExtFlash(sport4_move2_icon1_addr[0], ICON_MOVE2_ICON1_SIZE);
////        
////        // ���� �˳�
////        LCD_SetPosition(sport4_move2_icon3_coord[0][0], sport4_move2_icon3_coord[0][1], 
////                        ICON_MOVE2_ICON3_WIDE, ICON_MOVE2_ICON3_HIGH);
////        LCD_DriverWriteDataFromExtFlash(sport4_move2_icon3_addr[language%2], ICON_MOVE2_ICON3_SIZE);

////    }
////    *cls_flag = 1;
}


/****************************************************************
** Function name:           Gui_PageBP
** Descriptions:            ��ʾѪѹ����
** input parameters:        index����̬ͼ����  bp_sp: ��ѹ bp_dp����ѹ
**                          test_mode������ģʽ��1˽��ģʽ��0ͨ��ģʽ
**                          test_state�����Ե�״̬��1�����У�2��������3���Գ���
** output parameters:       
** Returned value:          
** Created by:              YX              
** Created Date:            2017-08-11
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Gui_PageBP(uint16_t index, uint8_t bp_sp, uint8_t bp_dp, uint8_t test_mode, uint8_t test_state, uint8_t hour, uint8_t minute, uint8_t *cls_flag, uint8_t language)
{
    #if TEST_CLEAR_SCREEN_PARAM
    if(index == 0)
    {
        Gui_PageRefreshBackground(0,0,LCD_MAX_COLUMN,LCD_MAX_ROW,0x00F8);
    }
    #endif //TEST_CLEAR_SCREEN_PARAM
    
    // Ѫѹ����
    if(index == 0 && *cls_flag == 1)
    {
        
    }    
    if(index == 0)
    {
        //����
        LCD_SetPosition(heart_share_bg_coord[0][0], heart_share_bg_coord[0][1], 
                             ICON_SHARE_BG_WIDE, ICON_SHARE_BG_HIGH);
        LCD_DriverWriteDataFromExtFlash(heart_share_bg_addr[0], ICON_SHARE_BG_SIZE);
             
    }     
  
    // Ѫѹ��ͼ��ʾ
    {
        LCD_SetPosition(blood_bp_icon1_coord[0][0], blood_bp_icon1_coord[0][1], 
                       ICON_BP_ICON1_WIDE, ICON_BP_ICON1_HIGH);
        LCD_DriverWriteDataFromExtFlash(blood_bp_icon1_addr[index % 2], ICON_BP_ICON1_SIZE);
    }    
    
    
    if(test_mode == 1)
    {
        //p
        LCD_SetPosition(blood_bp_icon2_coord[0][0], blood_bp_icon2_coord[0][1], 
                       ICON_BP_ICON2_WIDE, ICON_BP_ICON2_HIGH);
        LCD_DriverWriteDataFromExtFlash(blood_bp_icon2_addr[0], ICON_BP_ICON2_SIZE);
             
    }
    else
    {
       Gui_PageClearBackground(blood_bp_icon2_coord[0][0], blood_bp_icon2_coord[0][1], 
                                ICON_BP_ICON2_WIDE, ICON_BP_ICON2_HIGH);
    }    
    
    
    // ѪѹС��λ
    if(index == 0)
    {
        LCD_SetPosition(blood_bp_unit_coord[0][0], blood_bp_unit_coord[0][1], 
                        ICON_BP_UNIT_WIDE, ICON_BP_UNIT_HIGH);
        LCD_DriverWriteDataFromExtFlash(blood_bp_unit_addr[0], ICON_BP_UNIT_SIZE);
    }

    
    // Ѫѹ��λ
    LCD_SetPosition(blood_bp_bg1_coord[0][0], blood_bp_bg1_coord[0][1], 
                    ICON_BP_BG1_WIDE, ICON_BP_BG1_HIGH);
    LCD_DriverWriteDataFromExtFlash(blood_bp_bg1_addr[ language%2], ICON_BP_BG1_SIZE);
   if(index == 0)
   {
        // б��
        LCD_SetPosition(blood_bp_datadot_coord[0][0], blood_bp_datadot_coord[0][1], 
                        ICON_BP_DATADOT_WIDE, ICON_BP_DATADOT_HIGH);
        LCD_DriverWriteDataFromExtFlash(blood_bp_datadot_addr[0], ICON_BP_DATADOT_SIZE);   
   }
  
    if(test_state == 0)
    {    
            
        if(index == 0)
        {
            for(uint8_t i = 0; i < 6; i++)
            {
                // ����--- 
                LCD_SetPosition(blood_bp_datadot2_coord[i][0], blood_bp_datadot2_coord[i][1], 
                                ICON_BP_DATADOT2_WIDE, ICON_BP_DATADOT2_HIGH);
                LCD_DriverWriteDataFromExtFlash(blood_bp_datadot2_addr[0], ICON_BP_DATADOT2_SIZE);
            }

        }

    }
    else if(test_state == 1)
    {     
        // ��ѹ  
        if(bp_sp >= 100)
        {
            LCD_SetPosition(blood_bp_datadot2_coord[0][0], blood_bp_datadot2_coord[0][1], 
                            ICON_BP_DATA_WIDE, ICON_BP_DATA_HIGH);
            LCD_DriverWriteDataFromExtFlash(blood_bp_data_addr[(bp_sp / 100)% 10], ICON_BP_DATA_SIZE);          
        }
        else
        {
            LCD_SetPosition(blood_bp_datadot2_coord[0][0], blood_bp_datadot2_coord[0][1], 
                            ICON_BP_DATA_WIDE, ICON_BP_DATA_HIGH);
            LCD_DriverWriteDataFromExtFlash(blood_bp_data_addr[0], ICON_BP_DATA_SIZE); 
        }
        
        LCD_SetPosition(blood_bp_datadot2_coord[1][0], blood_bp_datadot2_coord[1][1], 
                            ICON_BP_DATA_WIDE, ICON_BP_DATA_HIGH);
        LCD_DriverWriteDataFromExtFlash(blood_bp_data_addr[(bp_sp / 10)% 10], ICON_BP_DATA_SIZE); 

        LCD_SetPosition(blood_bp_datadot2_coord[2][0], blood_bp_datadot2_coord[2][1], 
                            ICON_BP_DATA_WIDE, ICON_BP_DATA_HIGH);
        LCD_DriverWriteDataFromExtFlash(blood_bp_data_addr[bp_sp % 10], ICON_BP_DATA_SIZE);          

              
        // ��ѹ  
        if(bp_dp >= 100)
        {
            LCD_SetPosition(blood_bp_datadot2_coord[3][0], blood_bp_datadot2_coord[3][1], 
                            ICON_BP_DATA_WIDE, ICON_BP_DATA_HIGH);
            LCD_DriverWriteDataFromExtFlash(blood_bp_data_addr[(bp_dp % 1000) / 100], ICON_BP_DATA_SIZE); 
        }
        else
        {
            LCD_SetPosition(blood_bp_datadot2_coord[3][0], blood_bp_datadot2_coord[3][1], 
                            ICON_BP_DATA_WIDE, ICON_BP_DATA_HIGH);
            LCD_DriverWriteDataFromExtFlash(blood_bp_data_addr[0], ICON_BP_DATA_SIZE); 
        }
        
        LCD_SetPosition(blood_bp_datadot2_coord[4][0], blood_bp_datadot2_coord[4][1], 
                            ICON_BP_DATA_WIDE, ICON_BP_DATA_HIGH);
        LCD_DriverWriteDataFromExtFlash(blood_bp_data_addr[(bp_dp % 100) / 10], ICON_BP_DATA_SIZE); 

        LCD_SetPosition(blood_bp_datadot2_coord[5][0], blood_bp_datadot2_coord[5][1], 
                            ICON_BP_DATA_WIDE, ICON_BP_DATA_HIGH);
        LCD_DriverWriteDataFromExtFlash(blood_bp_data_addr[bp_dp % 10], ICON_BP_DATA_SIZE);  
		

    }
    else if(test_state >= 2)
    {
		 
        if(index == 0)
        {
            for(uint8_t i = 0; i < 6; i++)
            {
            
                // ����XXX 
                LCD_SetPosition(blood_bp_datadot2_coord[i][0], blood_bp_datadot2_coord[i][1], 
                                ICON_BP_DATA_WIDE, ICON_BP_DATA_HIGH);
                LCD_DriverWriteDataFromExtFlash(blood_bp_data_addr[10], ICON_BP_DATA_SIZE);
            }
                            
        }

    }
    
    *cls_flag = 2;
}

/****************************************************************
** Function name:           Gui_PageHeart
** Descriptions:            ��ʾ���ʽ���
** input parameters:        index����̬ͼ����  heart: ����ֵ
** output parameters:       
** Returned value:          
** Created by:              YX              
** Created Date:            2017-08-11
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Gui_PageHeart(uint16_t index, uint8_t heart, uint8_t last_heart, uint8_t hour, uint8_t minute, uint8_t language, uint8_t *cls_flag)
{
    #if TEST_CLEAR_SCREEN_PARAM
    if(index == 0)
    {
        Gui_PageRefreshBackground(0,0,LCD_MAX_COLUMN,LCD_MAX_ROW,0x00F8);
    }
    #endif //TEST_CLEAR_SCREEN_PARAM
    
    if(language)
    {
        language = 1;
    }
    
    // ��������
    if(index == 0 && *cls_flag == 1)
    {
        ;
    }

    
    if(index == 0)
    {
        //����
        LCD_SetPosition(heart_share_bg_coord[0][0], heart_share_bg_coord[0][1], 
                             ICON_SHARE_BG_WIDE, ICON_SHARE_BG_HIGH);
        LCD_DriverWriteDataFromExtFlash(heart_share_bg_addr[0], ICON_SHARE_BG_SIZE);
             
    }    
       
    
    // ����С��λ
    {
        LCD_SetPosition(heart_heart_unit1_coord[0][0], heart_heart_unit1_coord[0][1], 
                       ICON_HEART_UNIT1_WIDE, ICON_HEART_UNIT1_HIGH);
        LCD_DriverWriteDataFromExtFlash(heart_heart_unit1_addr[language % 2], ICON_HEART_UNIT1_SIZE);
    }
    

    // ���ʴ�λ
    {
        LCD_SetPosition(heart_heart_bg1_coord[0][0], heart_heart_bg1_coord[0][1], 
                       ICON_HEART_BG1_WIDE, ICON_HEART_BG1_HIGH);
        LCD_DriverWriteDataFromExtFlash(heart_heart_bg1_addr[language % 2], ICON_HEART_BG1_SIZE);
    }
 
    // ���ʶ�ͼ��ʾ
    {
        LCD_SetPosition(heart_heart_icon1_coord[0][0], heart_heart_icon1_coord[0][1], 
                       ICON_HEART_ICON1_WIDE, ICON_HEART_ICON1_HIGH);
        LCD_DriverWriteDataFromExtFlash(heart_heart_icon1_addr[index % 2], ICON_HEART_ICON1_SIZE);
    }
    
    //����ֵ
    if(heart != 0)
    {
        LCD_SetPosition(heart_share_data1_coord[0][0], heart_share_data1_coord[0][1], 
                        ICON_SHARE_DATA1_WIDE, ICON_SHARE_DATA1_HIGH);
        if(heart >= 100)
        {    
            LCD_DriverWriteDataFromExtFlash(heart_share_data1_addr[(heart / 100) %10], ICON_SHARE_DATA1_SIZE);        
        }  
        else
        {    
            LCD_DriverWriteDataFromExtFlash(heart_share_data1_addr[0], ICON_SHARE_DATA1_SIZE);        
        }
        
        LCD_SetPosition(heart_share_data1_coord[1][0], heart_share_data1_coord[1][1], 
                        ICON_SHARE_DATA1_WIDE, ICON_SHARE_DATA1_HIGH);
        if(heart >= 10)
        {    
            LCD_DriverWriteDataFromExtFlash(heart_share_data1_addr[(heart / 10) %10], ICON_SHARE_DATA1_SIZE);        
        }  
        else
        {    
            LCD_DriverWriteDataFromExtFlash(heart_share_data1_addr[0], ICON_SHARE_DATA1_SIZE);        
        }
        
        LCD_SetPosition(heart_share_data1_coord[2][0], heart_share_data1_coord[2][1], 
                        ICON_SHARE_DATA1_WIDE, ICON_SHARE_DATA1_HIGH);
//        if(heart > 0)
//        {
            LCD_DriverWriteDataFromExtFlash(heart_share_data1_addr[heart %10], ICON_SHARE_DATA1_SIZE);        
//        }
//        else
//        {    
//            LCD_DriverWriteDataFromExtFlash(icon_16_heart_data_addr[0], ICON_SHARE_DATA1_SIZE);        
//        }
    }
    else
    {
        for(uint8_t i = 0; i < 3; i++)
        {
            LCD_SetPosition(heart_share_data1_coord[i][0], heart_share_data1_coord[i][1], 
                                 ICON_SHARE_DATA1DOT_WIDE, ICON_SHARE_DATA1DOT_HIGH);
            LCD_DriverWriteDataFromExtFlash(heart_share_data1dot_addr[0], ICON_SHARE_DATA1DOT_SIZE);
        }
    }
    
    *cls_flag = 2;
    
}

/****************************************************************
** Function name:           Gui_PageSleep
** Descriptions:            ˯�߽���
** input parameters:        index����̬ͼ���� 
**                          time��˯�ߵ�ʱ��
**                          language����λ��ʾ������
** output parameters:       ��
** Returned value:          ��
** Created by:              YX              
** Created Date:            2017-08-11
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Gui_PageSleep(uint16_t index, uint16_t time, uint8_t hour, uint8_t minute, uint8_t language, uint8_t *cls_flag,
                    uint16_t deep_time,uint16_t light_time, uint8_t quality)
{  
    #if TEST_CLEAR_SCREEN_PARAM
    if(index == 0)
    {
        Gui_PageRefreshBackground(0,0,LCD_MAX_COLUMN,LCD_MAX_ROW,0x00F8);
    }
    #endif //TEST_CLEAR_SCREEN_PARAM
     
    
        if(language)
    {
        language = 1;
    }

    if(index == 0)
    {    
        Gui_PageClearBackground(0, 0, LCD_MAX_COLUMN, LCD_MAX_ROW);
        
        //����
        LCD_SetPosition(sleep_sleep_bg1_coord[0][0], sleep_sleep_bg1_coord[0][1], 
                             ICON_SLEEP_BG1_WIDE, ICON_SLEEP_BG1_HIGH);
        LCD_DriverWriteDataFromExtFlash(sleep_sleep_bg1_addr[language % 2], ICON_SLEEP_BG1_SIZE);
        
        //˯��ʱ��
        for(uint8_t i = 0; i < 3;i++)
        {
            LCD_SetPosition(icon_16_sleep_num_coord[i][0], icon_16_sleep_num_coord[i][1], 
                                 ICON_STEP_DATA1_WIDE, ICON_STEP_DATA1_HIGH);
            LCD_DriverWriteDataFromExtFlash(step_step_data1_addr[0], ICON_STEP_DATA1_SIZE);
        }
        
        //˯��С����
        LCD_SetPosition(sleep_sleep_datadot_coord[0][0], sleep_sleep_datadot_coord[0][1], 
                       ICON_SLEEP_DATADOT_WIDE, ICON_SLEEP_DATADOT_HIGH);
        LCD_DriverWriteDataFromExtFlash(sleep_sleep_datadot_addr[0], ICON_SLEEP_DATADOT_SIZE);        
        
        //��˯ʱ��
        for(uint8_t i = 0; i < 3;i++)
        {
            LCD_SetPosition(icon_16_sleep_deep_num_coord[i][0], icon_16_sleep_deep_num_coord[i][1], 
                                 ICON_STEP_DATA2_WIDE, ICON_STEP_DATA2_HIGH);
            LCD_DriverWriteDataFromExtFlash(step_step_data2_addr[0], ICON_STEP_DATA2_SIZE);
        }
        
        //��˯С����
        LCD_SetPosition(icon_16_sleep_d_icon_coord[1][0], icon_16_sleep_d_icon_coord[1][1], 
                       ICON_STEP_DATA2DOT_WIDE, ICON_STEP_DATA2DOT_HIGH);
        LCD_DriverWriteDataFromExtFlash(step_step_data2dot_addr[0], ICON_STEP_DATA2DOT_SIZE);    

        //ǳ˯ʱ��
        for(uint8_t i = 0; i < 3;i++)
        {
            LCD_SetPosition(icon_16_sleep_light_num_coord[i][0], icon_16_sleep_light_num_coord[i][1], 
                                 ICON_STEP_DATA2_WIDE, ICON_STEP_DATA2_HIGH);
            LCD_DriverWriteDataFromExtFlash(step_step_data2_addr[0], ICON_STEP_DATA2_SIZE);
        }
        //ǳ˯С����
        LCD_SetPosition(icon_16_sleep_d_icon_coord[2][0], icon_16_sleep_d_icon_coord[2][1], 
                       ICON_STEP_DATA2DOT_WIDE, ICON_STEP_DATA2DOT_HIGH);
        LCD_DriverWriteDataFromExtFlash(step_step_data2dot_addr[0], ICON_STEP_DATA2DOT_SIZE);      

        //˯������
        LCD_SetPosition(sleep_sleep_icon7_coord[0][0], sleep_sleep_icon7_coord[0][1], 
                       ICON_SLEEP_ICON7_WIDE, ICON_SLEEP_ICON7_HIGH);
        LCD_DriverWriteDataFromExtFlash(sleep_sleep_icon7_addr[quality%6][language % 2], ICON_SLEEP_ICON7_SIZE);         
    }    
    
   
    uint8_t sleep_hours = time / 60;
    uint8_t sleep_minutes = 10 * (time % 60 / 60.0f);
    uint32_t disp_sleep_buf1 = sleep_hours*10 + sleep_minutes;
////    uint32_t disp_sleep_buf2 = 0;
////    uint32_t disp_sleep_tmp = disp_sleep_buf1/DYNAMIC_NUM_MAX;
////    if(disp_sleep_tmp == 0) disp_sleep_tmp = 1;
////    uint32_t disp_sleep_cnt = 0;
    
    uint8_t deep_sleep_hours = deep_time / 60;
    uint8_t deep_sleep_minutes = 10 * (deep_time % 60 / 60.0f);
    uint32_t disp_deep_sleep_buf1 = deep_sleep_hours*10 + deep_sleep_minutes;
////    uint32_t disp_deep_sleep_buf2 = 0;
////    uint32_t disp_deep_sleep_tmp = disp_deep_sleep_buf1/DYNAMIC_NUM_MAX;
////    if(disp_deep_sleep_tmp == 0) disp_deep_sleep_tmp = 1;
////    uint32_t disp_deep_sleep_cnt = 0;
    
    uint8_t light_sleep_hours = light_time / 60;
    uint8_t light_sleep_minutes = 10 * (light_time % 60 / 60.0f);
    uint32_t disp_light_sleep_buf1 = light_sleep_hours*10 + light_sleep_minutes;
////    uint32_t disp_light_sleep_buf2 = 0;
////    uint32_t disp_light_sleep_tmp = disp_light_sleep_buf1/DYNAMIC_NUM_MAX;
////    if(disp_light_sleep_tmp == 0) disp_light_sleep_tmp = 1;
////    uint32_t disp_light_sleep_cnt = 0;
    
    if(index == 1)
    {
//        uint8_t flg1 = 0;
//        uint8_t flg2 = 0;
//        uint8_t flg3 = 0;
        uint8_t sleep_disp[3] = {0};
        uint8_t deep_sleep_disp[3] = {0};
        uint8_t light_sleep_disp[3] = {0};
        while(1)
        {
            //˯��ʱ��
            //if(flg1 == 0)
            {
                sleep_disp[0] = disp_sleep_buf1/100%10;
                sleep_disp[1] = disp_sleep_buf1/10%10;
                sleep_disp[2] = disp_sleep_buf1%10;
                for(uint8_t i = 0; i < 3; i++)
                {
                    LCD_SetPosition(icon_16_sleep_num_coord[i][0], icon_16_sleep_num_coord[i][1], 
                                         ICON_STEP_DATA1_WIDE, ICON_STEP_DATA1_HIGH);
                    LCD_DriverWriteDataFromExtFlash(step_step_data1_addr[sleep_disp[i]], ICON_STEP_DATA1_SIZE);
                }
//////                if(disp_sleep_buf2 == disp_sleep_buf1)
//////                {
//////                    flg1 = 1;
//////                }
//////                
//////                disp_sleep_buf2 += disp_sleep_tmp;
//////                if(disp_sleep_buf2 >= disp_sleep_buf1)disp_sleep_buf2 = disp_sleep_buf1;
            }
            
            //��˯ʱ��
            //if(flg2 == 0)
            {
                deep_sleep_disp[0] = disp_deep_sleep_buf1/100%10;
                deep_sleep_disp[1] = disp_deep_sleep_buf1/10%10;
                deep_sleep_disp[2] = disp_deep_sleep_buf1%10;
                for(uint8_t i = 0; i < 3; i++)
                {
                    LCD_SetPosition(icon_16_sleep_deep_num_coord[i][0], icon_16_sleep_deep_num_coord[i][1], 
                                         ICON_STEP_DATA2_WIDE, ICON_STEP_DATA2_HIGH);
                    LCD_DriverWriteDataFromExtFlash(step_step_data2_addr[deep_sleep_disp[i]], ICON_STEP_DATA2_SIZE);
                }
//////                if(disp_deep_sleep_buf2 == disp_deep_sleep_buf1)
//////                {
//////                    flg2 = 1;
//////                }
//////                
//////                disp_deep_sleep_buf2 += disp_deep_sleep_tmp;
//////                if(disp_deep_sleep_buf2 >= disp_deep_sleep_buf1)disp_deep_sleep_buf2 = disp_deep_sleep_buf1;
            }
            
            //ǳ˯ʱ��
            //if(flg3 == 0)
            {
                light_sleep_disp[0] = disp_light_sleep_buf1/100%10;
                light_sleep_disp[1] = disp_light_sleep_buf1/10%10;
                light_sleep_disp[2] = disp_light_sleep_buf1%10;
                for(uint8_t i = 0; i < 3; i++)
                {
                    LCD_SetPosition(icon_16_sleep_light_num_coord[i][0], icon_16_sleep_light_num_coord[i][1], 
                                         ICON_STEP_DATA2_WIDE, ICON_STEP_DATA2_HIGH);
                    LCD_DriverWriteDataFromExtFlash(step_step_data2_addr[light_sleep_disp[i]], ICON_STEP_DATA2_SIZE);
                }
////                if(disp_light_sleep_buf2 == disp_light_sleep_buf1)
////                {
////                    flg3 = 1;
////                }
////                
////                disp_light_sleep_buf2 += disp_light_sleep_tmp;
////                if(disp_light_sleep_buf2 >= disp_light_sleep_buf1)disp_light_sleep_buf2 = disp_light_sleep_buf1;
            }
            
            //if(flg1 == 1 && flg2 == 1 && flg3 == 1)
            {
                break;
            }
        }
    }
    *cls_flag = 2;
}

/****************************************************************
** Function name:           Gui_PagePower
** Descriptions:            �ػ�����
** input parameters:        
** output parameters:       
** Returned value:          
** Created by:              YX              
** Created Date:            2017-08-11
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Gui_PagePower(uint16_t index, uint8_t *mac_data, uint8_t *ver_data, uint8_t hour, uint8_t minute, uint8_t language, uint8_t *cls_flag,uint8_t *ble_name)
{
    uint8_t dis_data_buf[50] = {0};
    uint8_t i = 0;
    uint8_t temp_data = 0;
        
    #if TEST_CLEAR_SCREEN_PARAM
    if(index == 0)
    {
        Gui_PageRefreshBackground(0,0,LCD_MAX_COLUMN,LCD_MAX_ROW,0x00F8);
    }
    #endif //TEST_CLEAR_SCREEN_PARAM
    
	if(language )
    {
        language = 1;
    }
    
	// �ػ�ͼ���ϲ�����
	if(index == 0)
	{
		//������
        Gui_PageClearBackground(0, 
                                0, 
                                MAX_X_SIZE,
                                MAX_Y_SIZE);
////        //������
////        Gui_PageClearBackground(0, 
////                                0, 
////                                poweroff_poweroff_icon1_coord[0][0],
////                                MAX_Y_SIZE);
////        //������
////        Gui_PageClearBackground(0, 
////                                poweroff_poweroff_icon1_coord[0][1] + ICON_POWEROFF_ICON1_HIGH, 
////                                MAX_X_SIZE,
////                                MAX_Y_SIZE - (poweroff_poweroff_icon1_coord[0][1] + ICON_POWEROFF_ICON1_HIGH));
////        //������
////        Gui_PageClearBackground(poweroff_poweroff_icon1_coord[0][0] + ICON_POWEROFF_ICON1_WIDE, 
////                                0, 
////                                MAX_X_SIZE - (poweroff_poweroff_icon1_coord[0][0] + ICON_POWEROFF_ICON1_WIDE),
////                                MAX_Y_SIZE);
	}
	
	
	// �ػ�ͼ��
	{
		LCD_SetPosition(poweroff_poweroff_icon1_coord[0][0], poweroff_poweroff_icon1_coord[0][1], 
						ICON_POWEROFF_ICON1_WIDE, ICON_POWEROFF_ICON1_HIGH);
		LCD_DriverWriteDataFromExtFlash(poweroff_poweroff_icon1_addr[0], ICON_POWEROFF_ICON1_SIZE);
	}
    if(index == 0)
    {
        char buf_tmp[40];
		
		uint8_t buf_index = 0;
		uint8_t temp_data = 0;
        
        buf_index = 0;
        
        if(language == 0)
        {
            char name[20] = {0xE8, 0xAE, 0xBE, 0xE5, 0xA4, 0x87, 0xE5, 0x90, 0x8D, 0xE7, 0xA7, 0xB0,0}; 
            strcpy(buf_tmp, name);
            buf_index = strlen(buf_tmp);
        }
        else
        {
            char name[10] = "NAME"; 
            strcpy(buf_tmp, name);
            buf_index = strlen(buf_tmp);
        }
        
        buf_tmp[buf_index++] = ':';
        buf_tmp[buf_index++] = ' ';
        buf_tmp[buf_index++] = 0;
        strcat(buf_tmp,(char *)ble_name);
        buf_index = strlen(buf_tmp);
        
        Font_DisplayTitleText((uint8_t *)buf_tmp, buf_index,147, 1);
//        Font_DisUiTextOneLine2X(0, (uint8_t *)buf_tmp, buf_index, 48, 140, 1, 0, 0, 0X00);    
        
        i = 0;
        dis_data_buf[i++] = 'M';
        dis_data_buf[i++] = 'A';
        dis_data_buf[i++] = 'C';
        dis_data_buf[i++] = ':';
        dis_data_buf[i++] = ' ';
        
        for(uint8_t j = 4; j < 6; j++)
        {
            temp_data = mac_data[5 - j] / 16;
            if(temp_data >= 0x0A && temp_data <= 0x0F)
            {
                dis_data_buf[i++] = temp_data + 'A' - 10;
            }
            else if(temp_data <= 0x09)
            {
                dis_data_buf[i++] = temp_data + '0';
            }
            
            temp_data = mac_data[5 - j] % 16;
            if(temp_data >= 0x0A && temp_data <= 0x0F)
            {
                dis_data_buf[i++] = temp_data + 'A' - 10;
            }
            else if(temp_data <= 0x09)
            {
                dis_data_buf[i++] += temp_data +'0';
            }           
            dis_data_buf[i++] = ':';
        }
        i--;
        dis_data_buf[i] = 0;
        Font_DisplayTitleText(dis_data_buf, i, 175, 1);
//        Font_DisplayContentText(dis_data_buf, i, 175, 1);
//        Font_DisUiTextOneLine2X(0, dis_data_buf, i, 120, 175, 1, 0, 0, 0X00);
        i = 0;
        dis_data_buf[i++] = 'V';
        dis_data_buf[i++] = 'e';
        dis_data_buf[i++] = 'r';
        dis_data_buf[i++] = ':';
        dis_data_buf[i++] = ' ';
        for(uint8_t j = 0; j < 2; j++)
        {
            temp_data = ver_data[j] / 16 ;
            if(temp_data >= 0x0A && temp_data <= 0x0F)
            {
                dis_data_buf[i++] = temp_data + 'a' - 10;
            }
            else if(temp_data <= 0x09)
            {
                dis_data_buf[i++] = temp_data + '0';
            }
            
            temp_data = ver_data[j] % 16;
            if(temp_data >= 0x0A && temp_data <= 0x0F)
            {
                dis_data_buf[i++] = temp_data + 'a' - 10;
            }
            else if(temp_data <= 0x09)
            {
                dis_data_buf[i++] = temp_data + '0';
            }           
            dis_data_buf[i++] = '.';
        }
        i--;
        dis_data_buf[i] = 0;
        Font_DisplayTitleText(dis_data_buf, i, 204, 1);
//        Font_DisplayContentText(dis_data_buf, i, 204, 1);
//        Font_DisUiTextOneLine2X(0, dis_data_buf, i, 120, 204, 1, 0, 0, 0X00);
    }
        
    *cls_flag = 2;
}

/****************************************************************
** Function name:           Gui_PageFindPhone
** Descriptions:            ���ֻ�����
** input parameters:        dynamic_find���Ƿ�����̬ͼ
**                          index����̬ͼ����
** output parameters:       
** Returned value:          
** Created by:              YX              
** Created Date:            2017-08-11
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Gui_PageFindPhone(bool dynamic_find, uint16_t index, uint8_t hour, uint8_t minute, uint8_t language, uint8_t *cls_flag)
{ 
    #if TEST_CLEAR_SCREEN_PARAM
    if(index == 0)
    {
        Gui_PageRefreshBackground(0,0,LCD_MAX_COLUMN,LCD_MAX_ROW,0x00F8);
    }
    #endif //TEST_CLEAR_SCREEN_PARAM
    
    if(language)
    {
        language = 1;
    }    
    if(dynamic_find == 1)
    {
         
        if(index == 0)
        {
            Gui_PageClearBackground(0, 0, 
                                240, 240);
            //����
            LCD_SetPosition(heart_share_bg_coord[0][0], heart_share_bg_coord[0][1], 
                                 ICON_SHARE_BG_WIDE, ICON_SHARE_BG_HIGH);
            LCD_DriverWriteDataFromExtFlash(heart_share_bg_addr[0], ICON_SHARE_BG_SIZE);
            
            //�ֻ��·�����
            LCD_SetPosition(findphone_findphone_icon3_coord[0][0], findphone_findphone_icon3_coord[0][1], 
                                 ICON_FINDPHONE_ICON3_WIDE, ICON_FINDPHONE_ICON3_HIGH);
            LCD_DriverWriteDataFromExtFlash(findphone_findphone_icon3_addr[0], ICON_FINDPHONE_ICON3_SIZE);               
        }

         //�Ϸ�
        LCD_SetPosition(findphone_findphone_icon1_coord[0][0], findphone_findphone_icon1_coord[0][1], 
                       ICON_FINDPHONE_ICON1_WIDE, ICON_FINDPHONE_ICON1_HIGH);
        LCD_DriverWriteDataFromExtFlash(findphone_findphone_icon1_addr[0], ICON_FINDPHONE_ICON1_SIZE); 
        //��λ
        LCD_SetPosition(findphone_findphone_icon4_coord[0][0], findphone_findphone_icon4_coord[0][1], 
                             ICON_FINDPHONE_ICON4_WIDE, ICON_FINDPHONE_ICON4_HIGH);
        LCD_DriverWriteDataFromExtFlash(findphone_findphone_icon4_addr[language%2], ICON_FINDPHONE_ICON4_SIZE);
        //��ͼ1
        if(index % 2 == 0)
        {
            LCD_SetPosition(findphone_findphone_icon2_coord[0][0], findphone_findphone_icon2_coord[0][1], 
                           ICON_FINDPHONE_ICON2_WIDE, ICON_FINDPHONE_ICON2_HIGH);
            LCD_DriverWriteDataFromExtFlash(findphone_findphone_icon2_addr[(index/2)%3], ICON_FINDPHONE_ICON2_SIZE); 
        }
        *cls_flag = 2;
    }
    else
    {
//        if(index == 0)
//        {
//            //����
//            LCD_SetPosition(heart_share_bg_coord[0][0], heart_share_bg_coord[0][1], 
//                                 ICON_SHARE_BG_WIDE, ICON_SHARE_BG_HIGH);
//            LCD_DriverWriteDataFromExtFlash(heart_share_bg_addr[0], ICON_SHARE_BG_SIZE);
//            
//            //�ֻ��·�����
//            LCD_SetPosition(findphone_findphone_icon3_coord[0][0], findphone_findphone_icon3_coord[0][1], 
//                                 ICON_FINDPHONE_ICON3_WIDE, ICON_FINDPHONE_ICON3_HIGH);
//            LCD_DriverWriteDataFromExtFlash(findphone_findphone_icon3_addr[0], ICON_FINDPHONE_ICON3_SIZE);               
//        }
//        Gui_PageClearBackground(findphone_findphone_icon2_coord[0][0], findphone_findphone_icon2_coord[0][1], 
//                                ICON_FINDPHONE_ICON2_WIDE, ICON_FINDPHONE_ICON2_HIGH);
//         //�Ϸ�
//        LCD_SetPosition(findphone_findphone_icon1_coord[0][0], findphone_findphone_icon1_coord[0][1], 
//                       ICON_FINDPHONE_ICON1_WIDE, ICON_FINDPHONE_ICON1_HIGH);
//        LCD_DriverWriteDataFromExtFlash(findphone_findphone_icon1_addr[0], ICON_FINDPHONE_ICON1_SIZE); 
//        //��λ
//        LCD_SetPosition(findphone_findphone_icon4_coord[0][0], findphone_findphone_icon4_coord[0][1], 
//                             ICON_FINDPHONE_ICON4_WIDE, ICON_FINDPHONE_ICON4_HIGH);
//        LCD_DriverWriteDataFromExtFlash(findphone_findphone_icon4_addr[language%2], ICON_FINDPHONE_ICON4_SIZE);              
        *cls_flag = 1;
    }   
    
}

/****************************************************************
** Function name:           Gui_PageBleUnconnected
** Descriptions:            ����δ����
** input parameters:        
**                          index����̬ͼ����
** output parameters:       
** Returned value:          
** Created by:              YX              
** Created Date:            2017-08-11
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Gui_PageBleUnconnected(uint16_t index,uint8_t language)
{ 
    #if TEST_CLEAR_SCREEN_PARAM
    if(index == 0)
    {
        Gui_PageRefreshBackground(0,0,LCD_MAX_COLUMN,LCD_MAX_ROW,0x00F8);
    }
    #endif //TEST_CLEAR_SCREEN_PARAM
    
    if(language)
    {
        language = 1;
    }    

    if(index == 0)
    {
        Gui_PageClearBackground(0,0,240,240);
        LCD_SetPosition(73, 29, 
                       ICON_DISCONNECTED2_ICON1_WIDE, ICON_DISCONNECTED2_ICON1_HIGH);
        LCD_DriverWriteDataFromExtFlash(ICON_DISCONNECTED2_ICON1_ADDR, ICON_DISCONNECTED2_ICON1_SIZE);

        if(language)
        {
            LCD_SetPosition(16, 148, 
                       ICON_DISCONNECTED2_ICON2_WIDE, ICON_DISCONNECTED2_ICON2_HIGH);
            LCD_DriverWriteDataFromExtFlash(ICON_DISCONNECTED2_ICON2_2_ADDR, ICON_DISCONNECTED2_ICON2_SIZE);  
        }
        else
        {
            LCD_SetPosition(16, 148, 
                       ICON_DISCONNECTED2_ICON2_WIDE, ICON_DISCONNECTED2_ICON2_HIGH);
            LCD_DriverWriteDataFromExtFlash(ICON_DISCONNECTED2_ICON2_1_ADDR, ICON_DISCONNECTED2_ICON2_SIZE); 
        }

    }
    
}

/****************************************************************
** Function name:           GUI_StopWatchPage
** Descriptions:            ������
** input parameters:        entry: �Ƿ��Ѿ��������
**                          total_second��������
**                          one_hundred_ms��100������
** output parameters:       
** Returned value:          
** Created by:              YX              
** Created Date:            2017-08-11
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void GUI_StopWatchPage(uint16_t index, uint8_t entry_flag, uint8_t run_flag, uint16_t total_second, uint8_t one_hundred_ms,
						uint8_t sys_hour, uint8_t sys_minute, uint8_t language, uint8_t *cls_flag)
{
    #if TEST_CLEAR_SCREEN_PARAM
    if(index == 0)
    {
        Gui_PageRefreshBackground(0,0,LCD_MAX_COLUMN,LCD_MAX_ROW,0x00F8);
    }
    #endif //TEST_CLEAR_SCREEN_PARAM
    
    uint8_t minute = 0;
    uint8_t second = 0;
    uint8_t rand_num = 0;
    uint8_t data_tmp[6] = {0,};
	
	if(language)
	{
		language = 1;
	}
	
    if((index == 0)||(run_flag != 1))
	{
		animation_cnt = 0;
	}
	else
	{
		animation_cnt++;
	}
    
    if(total_second == 0 && one_hundred_ms == 0)
    {
        rand_num = 0;
    }
    else if((total_second == 5999 && one_hundred_ms >= 9) 
           || total_second > 5999)
    {
        rand_num = 9;
		total_second = 5999;
    }
    else
    {
        rand_num = one_hundred_ms + 5; 
    }
	second = total_second % 60;
    minute = total_second / 60;
	
    data_tmp[0] = (minute / 10) % 10;
    data_tmp[1] = minute % 10;
    data_tmp[2] = (second / 10) % 10;
    data_tmp[3] = second % 10;
    data_tmp[4] = one_hundred_ms % 10;
    data_tmp[5] = rand_num % 10;
    
    // �󱳾�
    if(index == 0)
    {
        LCD_SetPosition(heart_share_bg_coord[0][0], heart_share_bg_coord[0][1], 
                             ICON_SHARE_BG_WIDE, ICON_SHARE_BG_HIGH);
        LCD_DriverWriteDataFromExtFlash(heart_share_bg_addr[0], ICON_SHARE_BG_SIZE);
    }
    
    //��־ͼ��
    LCD_SetPosition(stopwacth_stopwatch_icon1_coord[0][0], stopwacth_stopwatch_icon1_coord[0][1], 
                    ICON_STOPWATCH_ICON1_WIDE, ICON_STOPWATCH_ICON1_HIGH);
    LCD_DriverWriteDataFromExtFlash(stopwacth_stopwatch_icon1_addr[animation_cnt % 2], ICON_STOPWATCH_ICON1_SIZE);    
    
    // ʱ����ʾ
    {
        for(uint32_t i = 0; i < 6; i++)
        {
            LCD_SetPosition(stopwacth_stopwatch_data1_coord[i][0], stopwacth_stopwatch_data1_coord[i][1], 
                            ICON_STOPWATCH_DATA1_WIDE, ICON_STOPWATCH_DATA1_HIGH);
            LCD_DriverWriteDataFromExtFlash(stopwacth_stopwatch_data1_addr[data_tmp[0 + i]], ICON_STOPWATCH_DATA1_SIZE);
        }

        // ʱ��ð��
        for(uint32_t i = 0; i < 2; i++)
        {
            LCD_SetPosition(stopwacth_stopwatch_data1dot1_coord[i][0], stopwacth_stopwatch_data1dot1_coord[i][1], 
                            ICON_STOPWATCH_DATA1DOT1_WIDE, ICON_STOPWATCH_DATA1DOT1_HIGH);
            LCD_DriverWriteDataFromExtFlash(stopwacth_stopwatch_data1dot1_addr[i], ICON_STOPWATCH_DATA1DOT1_SIZE);
        }
    }
    
    // ��ͣ����ͼ��
    if(run_flag)
    {
        LCD_SetPosition(icon_16_stopwatch_resume_coord[0][0], icon_16_stopwatch_resume_coord[0][1], 
                             ICON_COUNTDOWN_ICON3_WIDE, ICON_COUNTDOWN_ICON3_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_stopwatch_pause_addr[language% 2], ICON_COUNTDOWN_ICON3_SIZE);
        *cls_flag = 2;
    }
    else
    {
        if(total_second == 0)
        {
            // ����ͼ��
            LCD_SetPosition(icon_16_stopwatch_start_coord[0][0], icon_16_stopwatch_start_coord[0][1], 
                            ICON_COUNTDOWN_ICON3_WIDE, ICON_COUNTDOWN_ICON3_HIGH);
            LCD_DriverWriteDataFromExtFlash(icon_16_stopwatch_start_addr[language % 2], ICON_COUNTDOWN_ICON3_SIZE);
        }
        else
        {
            LCD_SetPosition(icon_16_stopwatch_pause_coord[0][0], icon_16_stopwatch_pause_coord[0][1], 
                            ICON_COUNTDOWN_ICON3_WIDE, ICON_COUNTDOWN_ICON3_HIGH);
            LCD_DriverWriteDataFromExtFlash(icon_16_stopwatch_resume_addr[language %2], ICON_COUNTDOWN_ICON3_SIZE);
        }
        *cls_flag = 1;
    }
    
    if(index == 0)
    {
        LCD_SetPosition(icon_16_stopwatch_start_coord[0][0], icon_16_stopwatch_start_coord[0][1], 
                        ICON_COUNTDOWN_ICON3_WIDE, ICON_COUNTDOWN_ICON3_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_stopwatch_start_addr[language % 2], ICON_COUNTDOWN_ICON3_SIZE);
    }

    
    //�²����ֱ���
    if(index == 0 || language != language_save)
    {
        LCD_SetPosition(stopwacth_stopwatch_bg1_coord[0][0], stopwacth_stopwatch_bg1_coord[0][1], 
                        ICON_STOPWATCH_BG1_WIDE, ICON_STOPWATCH_BG1_HIGH);
        LCD_DriverWriteDataFromExtFlash(stopwacth_stopwatch_bg1_addr[language % 2], ICON_STOPWATCH_BG1_SIZE);    
        
    }
  
}

/****************************************************************
** Function name:           Gui_PageCountDown
** Descriptions:            ����ʱ����
** input parameters:        dynamic���Ƿ�����̬ͼ
**                          index����̬ͼ���� 
** output parameters:       
** Returned value:          
** Created by:              YX              
** Created Date:            2017-08-11
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Gui_PageCountDown(uint16_t index, uint8_t dynamic, uint32_t total_second, uint8_t hour, uint8_t minute, uint8_t language, uint8_t *cls_flag, uint8_t run_state)
{
    #if TEST_CLEAR_SCREEN_PARAM
    if(index == 0)
    {
        Gui_PageRefreshBackground(0,0,LCD_MAX_COLUMN,LCD_MAX_ROW,0x00F8);
    }
    #endif //TEST_CLEAR_SCREEN_PARAM
    
	if(language)
	{
		language = 1;
	}
	
	if((index == 0)||(run_state != 1))
	{
		animation_cnt = 0;
	}
	else
	{
		animation_cnt++;
	}
	
     // �󱳾�
    if(index == 0)
    {
        LCD_SetPosition(heart_share_bg_coord[0][0], heart_share_bg_coord[0][1], 
                             ICON_SHARE_BG_WIDE, ICON_SHARE_BG_HIGH);
        LCD_DriverWriteDataFromExtFlash(heart_share_bg_addr[0], ICON_SHARE_BG_SIZE);
    }
    
    //����ͼ��
    LCD_SetPosition(countdown_countdown_icon1_coord[0][0], countdown_countdown_icon1_coord[0][1], 
                    ICON_COUNTDOWN_ICON1_WIDE, ICON_COUNTDOWN_ICON1_HIGH);
    LCD_DriverWriteDataFromExtFlash(countdown_countdown_icon1_addr[animation_cnt % 2], ICON_COUNTDOWN_ICON1_SIZE);    
        
    // ʱ����ʾ
    {
        uint32_t hour = 0;
        uint32_t minute = 0;
        uint32_t second = 0;
        uint8_t  data_tmp[6];
        
        hour = total_second / (60 * 60);
        minute = (total_second / 60) % 60;
        second = total_second % 60;
        
        data_tmp[0] = hour / 10;
        data_tmp[1] = hour % 10;
        data_tmp[2] = minute / 10;
        data_tmp[3] = minute % 10;
        data_tmp[4] = second / 10;
        data_tmp[5] = second % 10;
		
		// ������ʾ
        for(uint32_t i = 0; i < 6; i++) 
        {
            LCD_SetPosition(stopwacth_stopwatch_data1_coord[i][0], stopwacth_stopwatch_data1_coord[i][1], 
                            ICON_STOPWATCH_DATA1_WIDE, ICON_STOPWATCH_DATA1_HIGH);
            LCD_DriverWriteDataFromExtFlash(stopwacth_stopwatch_data1_addr[data_tmp[i]], ICON_STOPWATCH_DATA1_SIZE);
        }
		
        // ����ð��
        if(index == 0)
        {
            LCD_SetPosition(countdown_countdown_data1dot_coord[0][0], countdown_countdown_data1dot_coord[0][1], 
                            ICON_COUNTDOWN_DATA1DOT_WIDE, ICON_COUNTDOWN_DATA1DOT_HIGH);
            LCD_DriverWriteDataFromExtFlash(countdown_countdown_data1dot_addr[0], ICON_COUNTDOWN_DATA1DOT_SIZE);
        }
		
        // ð��
        if(index == 0)
        {
            LCD_SetPosition(countdown_countdown_data1dot_coord[1][0], countdown_countdown_data1dot_coord[1][1], 
                            ICON_COUNTDOWN_DATA1DOT_WIDE, ICON_COUNTDOWN_DATA1DOT_HIGH);
            LCD_DriverWriteDataFromExtFlash(countdown_countdown_data1dot_addr[0], ICON_COUNTDOWN_DATA1DOT_SIZE);
        }
    }
        
    LCD_SetPosition(icon_16_stopwatch_start_coord[0][0], icon_16_stopwatch_start_coord[0][1], 
                    ICON_COUNTDOWN_ICON3_WIDE, ICON_COUNTDOWN_ICON3_HIGH);
    if(dynamic == 0)
    {   //��ʼ
        LCD_DriverWriteDataFromExtFlash(icon_16_stopwatch_start_addr[language % 2], ICON_COUNTDOWN_ICON3_SIZE);
        *cls_flag = 2;
    }
    else
    {
        if(run_state == 1)
        {   //��ͣ
            LCD_DriverWriteDataFromExtFlash(icon_16_stopwatch_pause_addr[language % 2], ICON_COUNTDOWN_ICON3_SIZE);
        }
        else
        {
            //����
            LCD_DriverWriteDataFromExtFlash(icon_16_stopwatch_resume_addr[language % 2], ICON_COUNTDOWN_ICON3_SIZE);
        }
        *cls_flag = 1;
    }
 
    //�²����ֱ���
    if(index == 0 || language != language_save)
    {
        LCD_SetPosition(countdown_countdown_bg1_coord[0][0], countdown_countdown_bg1_coord[0][1], 
                        ICON_COUNTDOWN_BG1_WIDE, ICON_COUNTDOWN_BG1_HIGH);
        LCD_DriverWriteDataFromExtFlash(countdown_countdown_bg1_addr[language%2], ICON_COUNTDOWN_BG1_SIZE);    
        
    }
}


/****************************************************************
** Function name:           Gui_PageCountDownTimeOut
** Descriptions:            ����ʱ��������
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2018-05-25
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Gui_PageCountDownTimeOut(uint16_t index,  uint8_t language, uint8_t *cls_flag)
{
//////    #if TEST_CLEAR_SCREEN_PARAM
//////    if(index == 0)
//////    {
//////        Gui_PageRefreshBackground(0,0,LCD_MAX_COLUMN,LCD_MAX_ROW,0x00F8);
//////    }
//////    #endif //TEST_CLEAR_SCREEN_PARAM
//////    
//////    if(language)
//////    {
//////        language = 1;
//////    }
//////    
//////    //����ʱ�����ϲ�����
//////    if(index == 0 && *cls_flag != 0)
//////    {
//////        Gui_PageClearBackground(0, 
//////                                0, 
//////                                MAX_X_SIZE,
//////                               icon_16_countdown3_counter_coord[0][1]);
//////    }
//////    
//////    //����ʱ����������
//////    if(index == 0 && *cls_flag != 0)
//////    {
//////        Gui_PageClearBackground(0, 
//////                                icon_16_countdown3_counter_coord[0][1], 
//////                                icon_16_countdown3_counter_coord[0][0],
//////                                ICON_COUNTDOWN3_ICON1_HIGH);
//////    }
//////    
//////    
//////    //����
//////    LCD_SetPosition(icon_16_countdown3_counter_coord[0][0], icon_16_countdown3_counter_coord[0][1], 
//////							ICON_COUNTDOWN3_ICON1_WIDE, ICON_COUNTDOWN3_ICON1_HIGH);
//////    LCD_DriverWriteDataFromExtFlash(icon_16_countdown3_counter_addr[index %8], ICON_COUNTDOWN3_ICON1_SIZE);
//////    
//////    //����ʱ�����Ҳ�����
//////    if(index == 0 && *cls_flag != 0)
//////    {
//////        Gui_PageClearBackground(icon_16_countdown3_counter_coord[0][0] + ICON_COUNTDOWN3_ICON1_WIDE, 
//////                                icon_16_countdown3_counter_coord[0][1], 
//////                                MAX_X_SIZE - (icon_16_countdown3_counter_coord[0][0] + ICON_COUNTDOWN3_ICON1_WIDE),
//////                                ICON_COUNTDOWN3_ICON1_HIGH);
//////    }
//////    
//////    //����ʱ�����²�����
//////    if(index == 0 && *cls_flag != 0)
//////    {
//////        Gui_PageClearBackground(0, 
//////                                icon_16_countdown3_counter_coord[0][1] + ICON_COUNTDOWN3_ICON1_HIGH, 
//////                                MAX_X_SIZE,
//////                                MAX_Y_SIZE - (icon_16_countdown3_counter_coord[0][1] + ICON_COUNTDOWN3_ICON1_HIGH));
//////    }
//////    *cls_flag = 1;
}

/****************************************************************
** Function name:           Gui_PageCameraReady
** Descriptions:            ���׼������
** input parameters:        index����̬ͼ���� 
** output parameters:       
** Returned value:          
** Created by:              YX              
** Created Date:            2017-08-11
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Gui_PageCameraReady(uint16_t index, uint8_t camera_state, uint8_t *cls_flag)
{
    #if TEST_CLEAR_SCREEN_PARAM
    if(index == 0)
    {
        Gui_PageRefreshBackground(0,0,LCD_MAX_COLUMN,LCD_MAX_ROW,0x00F8);
    }
    #endif //TEST_CLEAR_SCREEN_PARAM
    
    // ͼ���ϲ�����
    if(index == 0 && *cls_flag != 0)
    {
		//������
        Gui_PageClearBackground(0, 
                                0, 
                                MAX_X_SIZE,
                                icon_16_camera_icon1_coord[0][1]);
        //������
        Gui_PageClearBackground(0, 
                                0, 
                                icon_16_camera_icon1_coord[0][0],
                                MAX_Y_SIZE);
        //������
        Gui_PageClearBackground(0, 
                                icon_16_camera_icon1_coord[0][1] + ICON_CAM_ICON1_HIGH, 
                                MAX_X_SIZE,
                                MAX_Y_SIZE - (icon_16_camera_icon1_coord[0][1] + ICON_CAM_ICON1_HIGH));
        //������
        Gui_PageClearBackground(icon_16_camera_icon1_coord[0][0] + ICON_CAM_ICON1_WIDE, 
                                0, 
                                MAX_X_SIZE - (icon_16_camera_icon1_coord[0][0] + ICON_CAM_ICON1_WIDE),
                                MAX_Y_SIZE);
    }
    
    if(camera_state == 1)
    {
        // ��ͼ
        LCD_SetPosition(icon_16_camera_icon1_coord[0][0], icon_16_camera_icon1_coord[0][1], 
                       ICON_CAM_ICON1_WIDE, ICON_CAM_ICON1_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_camera_icon1_addr[1], ICON_CAM_ICON1_SIZE);
        
        // С��ͼ
        LCD_SetPosition(icon_16_camera_icon2_coord[0][0], icon_16_camera_icon2_coord[0][1], 
                       ICON_CAM_ICON2_WIDE, ICON_CAM_ICON2_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_camera_icon2_addr[1], ICON_CAM_ICON2_SIZE);
    }
    else
    {
        // ��ͼ
        LCD_SetPosition(icon_16_camera_icon1_coord[0][0], icon_16_camera_icon1_coord[0][1], 
                       ICON_CAM_ICON1_WIDE, ICON_CAM_ICON1_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_camera_icon1_addr[0], ICON_CAM_ICON1_SIZE);
        
        // С��ͼ
        LCD_SetPosition(icon_16_camera_icon2_coord[0][0], icon_16_camera_icon2_coord[0][1], 
                       ICON_CAM_ICON2_WIDE, ICON_CAM_ICON2_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_camera_icon2_addr[0], ICON_CAM_ICON2_SIZE);
    }
   
    
     *cls_flag = 1;
}

/****************************************************************
** Function name:           Gui_PageDisMacVer
** Descriptions:            ��ʾMac�Ͱ汾��
** input parameters:        index����̬ͼ���� 
** output parameters:       
** Returned value:          
** Created by:              YX              
** Created Date:            2017-08-11
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Gui_PageDisMacVer(uint16_t index, uint8_t *mac_data, uint8_t *ver_data, uint8_t language, uint8_t *cls_flag)
{
    uint8_t buf_index = 0;
    uint8_t temp_data = 0;
    uint8_t dis_mac_data_buf[10] = {0};
    uint8_t dis_ver_data_buf[10] = {0};
    
//    if(index == 0)
//    {
//        Gui_PageClearBackground(0, 
//                                0, 
//                                MAX_X_SIZE,
//                                MAX_Y_SIZE);
//    }
        
    if(index == 0)
    {
        dis_mac_data_buf[buf_index++] = 'M';
        dis_mac_data_buf[buf_index++] = 'A';
        dis_mac_data_buf[buf_index++] = 'C';
        dis_mac_data_buf[buf_index++] = ':';
        temp_data = (mac_data[1] / 16) % 16;
        if(temp_data >= 0x0A && temp_data <= 0x0F)
        {
            dis_mac_data_buf[buf_index++] = temp_data + 'A' - 10;
        }
        else if(temp_data <= 0x09)
        {
            dis_mac_data_buf[buf_index++] = temp_data +'0';
        }
        temp_data = mac_data[1] % 16;
        if(temp_data >= 0x0A && temp_data <= 0x0F)
        {
            dis_mac_data_buf[buf_index++] = temp_data + 'A' - 10;
        }
        else if(temp_data <= 0x09)
        {
            dis_mac_data_buf[buf_index++] = temp_data +'0';
        }
        dis_mac_data_buf[buf_index++] = '.';
        temp_data = (mac_data[0] / 16) % 16;
        if(temp_data >= 0x0A && temp_data <= 0x0F)
        {
            dis_mac_data_buf[buf_index++] = temp_data + 'A' - 10;
        }
        else if(temp_data <= 0x09)
        {
            dis_mac_data_buf[buf_index++] = temp_data +'0';
        }
        temp_data = mac_data[0] % 16;
        if(temp_data >= 0x0A && temp_data <= 0x0F)
        {
            dis_mac_data_buf[buf_index++] = temp_data + 'A' - 10;
        }
        else if(temp_data <= 0x09)
        {
            dis_mac_data_buf[buf_index++] = temp_data +'0';
        } 
//        Font_DisUiTextOneLine2X(0,dis_mac_data_buf, buf_index, 120, 80, 1, 0, 0, 0x0000);
        Font_DisContentText(dis_mac_data_buf, buf_index, 139, 0, 0, 0);
        
        buf_index = 0;
        dis_ver_data_buf[buf_index++] = 'V';
        dis_ver_data_buf[buf_index++] = 'e';
        dis_ver_data_buf[buf_index++] = 'r';
        dis_ver_data_buf[buf_index++] = ':';
        dis_ver_data_buf[buf_index++] = ver_data[0] / 16 + '0';
        dis_ver_data_buf[buf_index++] = ver_data[0] % 16 + '0';
        dis_ver_data_buf[buf_index++] = '.';
        dis_ver_data_buf[buf_index++] = ver_data[1] / 16 + '0';
        dis_ver_data_buf[buf_index++] = ver_data[1] % 16 + '0'; 
//        Font_DisUiTextOneLine2X(0,dis_ver_data_buf, buf_index, 120, 128, 1, 0, 0, 0x0000);
        Font_DisContentText(dis_ver_data_buf, buf_index, 163, 0, 0, 0);
    }
}

/****************************************************************
** Function name:           Gui_PageAlarm
** Descriptions:            ���ӽ���
** input parameters:        am_pm_format��am��pm״̬     scene�����ӵ����� 
**                          hour�����ӵ�ʱ   minute�����ӵķ�
**                          language�����ӵ�λ������  index����̬ͼ����
** output parameters:       
** Returned value:          
** Created by:              YX              
** Created Date:            2017-08-11
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Gui_PageAlarm(uint16_t index, uint8_t am_pm_format, uint8_t scene, uint8_t hour, uint8_t minute, uint8_t language, uint8_t *cls_flag)
{
    #if TEST_CLEAR_SCREEN_PARAM
    if(index == 0)
    {
        Gui_PageRefreshBackground(0,0,LCD_MAX_COLUMN,LCD_MAX_ROW,0x00F8);
    }
    #endif //TEST_CLEAR_SCREEN_PARAM
    
    if(scene == 0)
    {
        // ͼ���ϲ�����
        if(index == 0)
        {
            //������
            Gui_PageClearBackground(0, 
                                    0, 
                                    MAX_X_SIZE,
                                    charging_chg_icon1_coord[0][1]);
            //������
            Gui_PageClearBackground(0, 
                                    0, 
                                    charging_chg_icon1_coord[0][0],
                                    MAX_Y_SIZE);
            //������
            Gui_PageClearBackground(0, 
                                    charging_chg_icon1_coord[0][1] + ICON_CHG_ICON1_HIGH, 
                                    MAX_X_SIZE,
                                    MAX_Y_SIZE - (charging_chg_icon1_coord[0][1] + ICON_CHG_ICON1_HIGH));
            //������
            Gui_PageClearBackground(charging_chg_icon1_coord[0][0] + ICON_CHG_ICON1_WIDE, 
                                    0, 
                                    MAX_X_SIZE - (charging_chg_icon1_coord[0][0] + ICON_CHG_ICON1_WIDE),
                                    MAX_Y_SIZE);
            
        }
        
        // ʱ����ʾ
        if(index%2 == 0)
        {
            // ����ʱ��--ʱ
            LCD_SetPosition(clock_clock_data2_coord[0][0], clock_clock_data2_coord[0][1],
                            ICON_CLOCK_DATA2_WIDE, ICON_CLOCK_DATA2_HIGH);
            LCD_DriverWriteDataFromExtFlash(clock_clock_data2_addr[(hour / 10) % 10], ICON_CLOCK_DATA2_SIZE);
            LCD_SetPosition(clock_clock_data2_coord[1][0], clock_clock_data2_coord[1][1],
                            ICON_CLOCK_DATA2_WIDE, ICON_CLOCK_DATA2_HIGH);
            LCD_DriverWriteDataFromExtFlash(clock_clock_data2_addr[hour % 10], ICON_CLOCK_DATA2_SIZE);
            
            // ����ʱ��--�� 
            LCD_SetPosition(clock_clock_data2_coord[2][0], clock_clock_data2_coord[2][1],
                            ICON_CLOCK_DATA2_WIDE, ICON_CLOCK_DATA2_HIGH);
            LCD_DriverWriteDataFromExtFlash(clock_clock_data2_addr[(minute / 10) % 10], ICON_CLOCK_DATA2_SIZE);
            LCD_SetPosition(clock_clock_data2_coord[3][0], clock_clock_data2_coord[3][1],
                            ICON_CLOCK_DATA2_WIDE, ICON_CLOCK_DATA2_HIGH);
            LCD_DriverWriteDataFromExtFlash(clock_clock_data2_addr[minute % 10], ICON_CLOCK_DATA2_SIZE); 
            
            // ����ʱ��--ð��
            LCD_SetPosition(clock_clock_data2dot_coord[0][0], clock_clock_data2dot_coord[0][1],
                            ICON_CLOCK_DATA2DOT_WIDE, ICON_CLOCK_DATA2DOT_HIGH);
            LCD_DriverWriteDataFromExtFlash(clock_clock_data2dot_addr[0], ICON_CLOCK_DATA2DOT_SIZE);
        }
        else
        {
            // ���ӽ���-ͼ��
            for(uint8_t i = 0; i < 4; i++)
            {
                LCD_SetPosition(clock_clock_data2_coord[i][0], clock_clock_data2_coord[i][1],
                            ICON_CLOCK_DATA2_WIDE, ICON_CLOCK_DATA2_HIGH);
                LCD_DriverWriteDataFromExtFlash(clock_clock_data1dot2_addr[0], ICON_CLOCK_DATA2_SIZE);   
            }
            // ����ʱ��--ð��
            LCD_SetPosition(clock_clock_data1dot_coord[0][0], clock_clock_data1dot_coord[0][1],
                            ICON_CLOCK_DATA1DOT_WIDE, ICON_CLOCK_DATA1DOT_HIGH);
            LCD_DriverWriteDataFromExtFlash(clock_clock_data1dot_addr[0], ICON_CLOCK_DATA1DOT_SIZE);
       
        }

        // ��ʾ������
        if(index == 0)
        {
            LCD_SetPosition(clock_clock_icon1_coord[0][0], clock_clock_icon1_coord[0][1], 
                            ICON_CLOCK_ICON1_WIDE, ICON_CLOCK_ICON1_HIGH);
            if(am_pm_format == GUI_TIME_FORMAT_AM)
            {
                if(language == 0)
                {
                    LCD_DriverWriteDataFromExtFlash(clock_clock_icon1_addr[0], ICON_CLOCK_ICON1_SIZE);
                }
                else
                {
                    LCD_DriverWriteDataFromExtFlash(clock_clock_icon1_addr[2], ICON_CLOCK_ICON1_SIZE);
                }
            }
            else if(am_pm_format == GUI_TIME_FORMAT_PM)
            {
                if(language == 0)
                {
                    LCD_DriverWriteDataFromExtFlash(clock_clock_icon1_addr[1], ICON_CLOCK_ICON1_SIZE);
                }
                else
                {
                    LCD_DriverWriteDataFromExtFlash(clock_clock_icon1_addr[3], ICON_CLOCK_ICON1_SIZE);
                }
            }
            else
            { 
                //��Ҫ�滻
                LCD_DriverWriteDataFromExtFlash(clock_clock_icon1_addr[4], ICON_CLOCK_ICON1_SIZE);
            }
        }

    }
    else
    {
////        // �龰����ͼ��
////        
////        scene -= 1;
////        scene %= 20;
////        
////        // ͼ���ϲ�����
////        if(index == 0)
////        {
////            //������
////            Gui_PageClearBackground(0, 
////                                    0, 
////                                    MAX_X_SIZE,
////                                    scenecloc_remindclock_icon_coord[0][1]);
////            //������
////            Gui_PageClearBackground(0, 
////                                    0, 
////                                    scenecloc_remindclock_icon_coord[0][0],
////                                    MAX_Y_SIZE);
////            //������
////            Gui_PageClearBackground(0, 
////                                    scenecloc_remindclock_icon_coord[0][1] + ICON_REMINDCLOCK_ICON_HIGH, 
////                                    MAX_X_SIZE,
////                                    MAX_Y_SIZE - (scenecloc_remindclock_icon_coord[0][1] + ICON_REMINDCLOCK_ICON_HIGH));
////            //������
////            Gui_PageClearBackground(scenecloc_remindclock_icon_coord[0][0] + ICON_REMINDCLOCK_ICON_WIDE, 
////                                    0, 
////                                    MAX_X_SIZE - (scenecloc_remindclock_icon_coord[0][0] + ICON_REMINDCLOCK_ICON_WIDE),
////                                    MAX_Y_SIZE);
////                        
////        }
////        
////        // ͼ����ʾ
////        if(index == 0)
////        {
////            LCD_SetPosition(scenecloc_remindclock_icon_coord[0][0], scenecloc_remindclock_icon_coord[0][1], 
////                            ICON_REMINDCLOCK_ICON_WIDE, ICON_REMINDCLOCK_ICON_HIGH);
////            LCD_DriverWriteDataFromExtFlash(scenecloc_remindclock_icon_addr[scene % 20], ICON_REMINDCLOCK_ICON_SIZE); 
////        }
    }
    *cls_flag = 1;
}

/****************************************************************
** Function name:           Gui_PageSit
** Descriptions:            ��������
** input parameters:        index����̬ͼ����
** output parameters:       
** Returned value:          
** Created by:              YX              
** Created Date:            2017-08-11
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Gui_PageSit(uint16_t index, uint8_t language, uint8_t *cls_flag)
{
    #if TEST_CLEAR_SCREEN_PARAM
    if(index == 0)
    {
        Gui_PageRefreshBackground(0,0,LCD_MAX_COLUMN,LCD_MAX_ROW,0x00F8);
    }
    #endif //TEST_CLEAR_SCREEN_PARAM
    
    if(language)
    {
        language = 1;
    }
    
    // ͼ���ϲ�����
    if(index == 0)
    {
		//������
        Gui_PageClearBackground(0, 
                                0, 
                                MAX_X_SIZE,
                                icon_16_sit_icon_coord[0][1]);
        //������
        Gui_PageClearBackground(0, 
                                0, 
                                icon_16_sit_icon_coord[0][0],
                                MAX_Y_SIZE);
        //������
        Gui_PageClearBackground(0, 
                                icon_16_sit_icon_coord[0][1] + ICON_SIT_ICON1_HIGH, 
                                MAX_X_SIZE,
                                MAX_Y_SIZE - (icon_16_sit_icon_coord[0][1] + ICON_SIT_ICON1_HIGH));
        //������
        Gui_PageClearBackground(icon_16_sit_icon_coord[0][0] + ICON_SIT_ICON1_WIDE, 
                                0, 
                                MAX_X_SIZE - (icon_16_sit_icon_coord[0][0] + ICON_SIT_ICON1_WIDE),
                                MAX_Y_SIZE);
                    
    }
    
    
    if(index == 0)
    {
        //����
        LCD_SetPosition(icon_16_sit_unit_coord[0][0], icon_16_sit_unit_coord[0][1], 
                        ICON_SIT_ICON2_WIDE, ICON_SIT_ICON2_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_sit_unit_addr[0], ICON_SIT_ICON2_SIZE); 
    }
    
    //��ͼ
    LCD_SetPosition(icon_16_sit_icon_coord[0][0], icon_16_sit_icon_coord[0][1], 
                    ICON_SIT_ICON1_WIDE, ICON_SIT_ICON1_HIGH);
    LCD_DriverWriteDataFromExtFlash(icon_16_sit_icon_addr[index%7], ICON_SIT_ICON1_SIZE);
    *cls_flag = 1;
}

/****************************************************************
** Function name:           Gui_PageLowPower
** Descriptions:            �͵����ѽ���
** input parameters:        index����̬ͼ����
** output parameters:       
** Returned value:          
** Created by:              YX              
** Created Date:            2017-08-11
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Gui_PageLowPower(uint16_t index, uint8_t *cls_flag)
{   
    #if TEST_CLEAR_SCREEN_PARAM
    if(index == 0)
    {
        Gui_PageRefreshBackground(0,0,LCD_MAX_COLUMN,LCD_MAX_ROW,0x00F8);
    }
    #endif //TEST_CLEAR_SCREEN_PARAM
    
    // ͼ���ϲ�����
    if(index == 0)
    {
		//������
        Gui_PageClearBackground(0, 
                                0, 
                                MAX_X_SIZE,
                                icon_16_sit_icon_coord[0][1]);
        //������
        Gui_PageClearBackground(0, 
                                0, 
                                icon_16_sit_icon_coord[0][0],
                                MAX_Y_SIZE);
        //������
        Gui_PageClearBackground(0, 
                                icon_16_sit_icon_coord[0][1] + ICON_SIT_ICON1_HIGH, 
                                MAX_X_SIZE,
                                MAX_Y_SIZE - (icon_16_sit_icon_coord[0][1] + ICON_SIT_ICON1_HIGH));
        //������
        Gui_PageClearBackground(icon_16_sit_icon_coord[0][0] + ICON_SIT_ICON1_WIDE, 
                                0, 
                                MAX_X_SIZE - (icon_16_sit_icon_coord[0][0] + ICON_SIT_ICON1_WIDE),
                                MAX_Y_SIZE);  
             
    }
    
    LCD_SetPosition(lowchg_chg_icon2_coord[0][0], lowchg_chg_icon2_coord[0][1], 
                    ICON_CHG_ICON2_WIDE, ICON_CHG_ICON2_HIGH);
    LCD_DriverWriteDataFromExtFlash(lowchg_chg_icon2_addr[index % 2], ICON_CHG_ICON2_SIZE);
	
    *cls_flag = 1;
}

/****************************************************************
** Function name:           Gui_PageBatteryCharg
** Descriptions:            ��س�����
** input parameters:        index����̬ͼ����
** output parameters:       
** Returned value:          
** Created by:              YX              
** Created Date:            2017-08-11
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Gui_PageBatteryCharg(uint16_t index, uint8_t state, uint8_t *cls_flag)
{	
    #if TEST_CLEAR_SCREEN_PARAM
    if(index == 0)
    {
        Gui_PageRefreshBackground(0,0,LCD_MAX_COLUMN,LCD_MAX_ROW,0x00F8);
    }
    #endif //TEST_CLEAR_SCREEN_PARAM
    
    // ͼ���ϲ�����
    if(index == 0 && *cls_flag != 0)
    {
        //������
        Gui_PageClearBackground(0, 
                                0, 
                                MAX_X_SIZE,
                                charging_chg_icon1_coord[0][1]);
        //������
        Gui_PageClearBackground(0, 
                                0, 
                                charging_chg_icon1_coord[0][0],
                                MAX_Y_SIZE);
        //������
        Gui_PageClearBackground(0, 
                                charging_chg_icon1_coord[0][1] + ICON_CHG_ICON1_HIGH, 
                                MAX_X_SIZE,
                                MAX_Y_SIZE - (charging_chg_icon1_coord[0][1] + ICON_CHG_ICON1_HIGH));
        //������
        Gui_PageClearBackground(charging_chg_icon1_coord[0][0] + ICON_CHG_ICON1_WIDE, 
                                0, 
                                MAX_X_SIZE - (charging_chg_icon1_coord[0][0] + ICON_CHG_ICON1_WIDE),
                                MAX_Y_SIZE);
    }
    
    
    // ��ͼ��ʾ
    if(state == 0)
    {
         // ��صȼ���ʾ
		{		
			LCD_SetPosition(charging_chg_icon1_coord[0][0], charging_chg_icon1_coord[0][1], 
						   ICON_CHG_ICON1_WIDE, ICON_CHG_ICON1_HIGH);
			LCD_DriverWriteDataFromExtFlash(charging_chg_icon1_addr[index % 5], ICON_CHG_ICON1_SIZE);
		}
	}
    else
    {
        LCD_SetPosition(charging_chg_icon1_coord[0][0], charging_chg_icon1_coord[0][1], 
                        ICON_CHG_ICON1_WIDE, ICON_CHG_ICON1_HIGH);
        LCD_DriverWriteDataFromExtFlash(charging_chg_icon1_addr[4], ICON_CHG_ICON1_SIZE);
        
    }
    

    *cls_flag = 1;
}

/****************************************************************
** Function name:           Gui_PageHeartAlert
** Descriptions:            ���ʱ�������
** input parameters:        index����̬ͼ����
** output parameters:       
** Returned value:          
** Created by:              YX              
** Created Date:            2017-08-11
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/
void Gui_PageHeartAlert(uint16_t index, uint8_t *cls_flag)
{
    #if TEST_CLEAR_SCREEN_PARAM
    if(index == 0)
    {
        Gui_PageRefreshBackground(0,0,LCD_MAX_COLUMN,LCD_MAX_ROW,0x00F8);
    }
    #endif //TEST_CLEAR_SCREEN_PARAM
    
    // ͼ���ϲ�����
    if(index == 0 && *cls_flag != 0)
    {
		//������
        Gui_PageClearBackground(0, 
                                0, 
                                MAX_X_SIZE,
                                heartalert_heartalert_icon1_coord[0][1]);
        //������
        Gui_PageClearBackground(0, 
                                0, 
                                heartalert_heartalert_icon1_coord[0][0],
                                MAX_Y_SIZE);
        //������
        Gui_PageClearBackground(0, 
                                heartalert_heartalert_icon1_coord[0][1] + ICON_HEARTALERT_ICON1_HIGH, 
                                MAX_X_SIZE,
                                MAX_Y_SIZE - (heartalert_heartalert_icon1_coord[0][1] + ICON_HEARTALERT_ICON1_HIGH));
        //������
        Gui_PageClearBackground(heartalert_heartalert_icon1_coord[0][0] + ICON_HEARTALERT_ICON1_WIDE, 
                                0, 
                                MAX_X_SIZE - (heartalert_heartalert_icon1_coord[0][0] + ICON_HEARTALERT_ICON1_WIDE),
                                MAX_Y_SIZE);
    }
    // ͼ���ϲ�����
    if(index == 0)
    { 
        // ��ͼ
        LCD_SetPosition(heartalert_heartalert_icon1_coord[0][0], heartalert_heartalert_icon1_coord[0][1], 
                             ICON_HEARTALERT_ICON1_WIDE, ICON_HEARTALERT_ICON1_HIGH);
        LCD_DriverWriteDataFromExtFlash(heartalert_heartalert_icon1_addr[0], ICON_HEARTALERT_ICON1_SIZE);
    }
    
    // ��
    LCD_SetPosition(heartalert_heartalert_icon2_coord[0][0], heartalert_heartalert_icon2_coord[0][1], 
                         ICON_HEARTALERT_ICON2_WIDE, ICON_HEARTALERT_ICON2_HIGH);
    LCD_DriverWriteDataFromExtFlash(heartalert_heartalert_icon2_addr[index%2], ICON_HEARTALERT_ICON2_SIZE);
    *cls_flag = 1;
}
/****************************************************************
** Function name:           Gui_LowOxygenAlertPage
** Descriptions:            ������������
** input parameters:        index����̬ͼ����
** output parameters:       
** Returned value:          
** Created by:              YX              
** Created Date:            2017-08-11
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/
void Gui_LowOxygenAlertPage(uint16_t index, uint8_t *cls_flag)
{
    #if TEST_CLEAR_SCREEN_PARAM
    if(index == 0)
    {
        Gui_PageRefreshBackground(0,0,LCD_MAX_COLUMN,LCD_MAX_ROW,0x00F8);
    }
    #endif //TEST_CLEAR_SCREEN_PARAM
    
    // ͼ���ϲ�����
    if(index == 0 && *cls_flag != 0)
    {
		//������
        Gui_PageClearBackground(0, 
                                0, 
                                MAX_X_SIZE,
                                heartalert_heartalert_icon1_coord[0][1]);
        //������
        Gui_PageClearBackground(0, 
                                0, 
                                heartalert_heartalert_icon1_coord[0][0],
                                MAX_Y_SIZE);
        //������
        Gui_PageClearBackground(0, 
                                heartalert_heartalert_icon1_coord[0][1] + ICON_HEARTALERT_ICON1_HIGH, 
                                MAX_X_SIZE,
                                MAX_Y_SIZE - (heartalert_heartalert_icon1_coord[0][1] + ICON_HEARTALERT_ICON1_HIGH));
        //������
        Gui_PageClearBackground(heartalert_heartalert_icon1_coord[0][0] + ICON_HEARTALERT_ICON1_WIDE, 
                                0, 
                                MAX_X_SIZE - (heartalert_heartalert_icon1_coord[0][0] + ICON_HEARTALERT_ICON1_WIDE),
                                MAX_Y_SIZE) ;
    }
    //��������
    if(index == 0)
    {
        // ��ͼ
        LCD_SetPosition(icon_16_spo2_icon1_coord[0][0], icon_16_spo2_icon1_coord[0][1], 
                             ICON_SPO2_ICON4_WIDE, ICON_SPO2_ICON4_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_spo2_icon1_addr[0], ICON_SPO2_ICON4_SIZE);
    }
    
    // ��
    LCD_SetPosition(icon_16_spo2_label1_coord[0][0], icon_16_spo2_label1_coord[0][1], 
                         ICON_SPO2_ICON5_WIDE, ICON_SPO2_ICON5_HIGH);
    LCD_DriverWriteDataFromExtFlash(icon_16_spo2_label1_addr[index%2], ICON_SPO2_ICON5_SIZE);
    *cls_flag = 1;
}
/****************************************************************
** Function name:           Gui_PageFontTest
** Descriptions:            ��ʾ��ά���ǿƼ�����������
** input parameters:        
** output parameters:       
** Returned value:          
** Created by:              YX              
** Created Date:            2017-08-11
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Gui_PageFontTest(uint16_t index, uint8_t *cls_flag)
{
    // ����
    if(index == 0)
    {
        Gui_PageClearBackground(0, 
                                0, 
                                MAX_X_SIZE,
                                MAX_Y_SIZE);
    }
	
    Font_DisplayFontTestText(); //��������ʹ��
}


/****************************************************************
** Function name:           Gui_PageCallNotificationIcon
** Descriptions:            ��ʾ�������
** input parameters:        call_state�������״̬
**                          index����̬ͼ����
** output parameters:       
** Returned value:          
** Created by:              YX              
** Created Date:            2017-08-11
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Gui_PageCallNotificationIcon(uint16_t index, uint8_t call_state, uint8_t language, uint8_t *cls_flag)
{	
    #if TEST_CLEAR_SCREEN_PARAM
    if(index == 0)
    {
        Gui_PageRefreshBackground(0,0,LCD_MAX_COLUMN,LCD_MAX_ROW,0x00F8);
    }
    #endif //TEST_CLEAR_SCREEN_PARAM
    
    if(language)
    {
        language = 1;
    }
    
    // ����
    if(index == 0 )
    {
        Gui_PageClearBackground(0, 
                                0, 
                                MAX_X_SIZE,
                                MAX_Y_SIZE);
    } 
    
	// ����״̬��ʾ��
	if(call_state == GUI_CALL_MUTE)
    {
        LCD_SetPosition(call_call_icon5_coord[0][0], call_call_icon5_coord[0][1], 
                        ICON_CALL_ICON5_WIDE, ICON_CALL_ICON5_HIGH);
        LCD_DriverWriteDataFromExtFlash(call_call_icon5_addr[language % 2], ICON_CALL_ICON5_SIZE);
    }
    else if(call_state == GUI_CALL_REJECTSUC)
    {
        LCD_SetPosition(call_call_icon6_coord[0][0], call_call_icon6_coord[0][1], 
                        ICON_CALL_ICON5_WIDE, ICON_CALL_ICON5_HIGH);
        LCD_DriverWriteDataFromExtFlash(call_call_icon6_addr[language % 2], ICON_CALL_ICON5_SIZE);
    }
    
    // ����Ҷ�ͼ��
    {
        LCD_SetPosition(call_call_icon2_coord[0][0], call_call_icon2_coord[0][1], 
                        ICON_CALL_ICON2_WIDE, ICON_CALL_ICON2_HIGH);
        if((call_state == GUI_CALL_INCOMING)||(call_state == GUI_CALL_MUTE))
        {
            
            LCD_DriverWriteDataFromExtFlash(call_call_icon2_addr[0], ICON_CALL_ICON2_SIZE);
        }
        else if(call_state == GUI_CALL_REJECTSUC)
        {
            LCD_DriverWriteDataFromExtFlash(call_call_icon2_addr[1], ICON_CALL_ICON2_SIZE);
        }
    }
	
    // ���羲��ͼ��
    {
        LCD_SetPosition(call_call_icon3_coord[0][0], call_call_icon3_coord[0][1], 
                        ICON_CALL_ICON3_WIDE, ICON_CALL_ICON3_HIGH);
        if(call_state == GUI_CALL_INCOMING)
        {
            LCD_DriverWriteDataFromExtFlash(call_call_icon3_addr[0], ICON_CALL_ICON3_SIZE);
        }
        else if(call_state == GUI_CALL_MUTE)
        {
            LCD_DriverWriteDataFromExtFlash(call_call_icon3_addr[1], ICON_CALL_ICON3_SIZE);
        }
    }

	// �Ҷ���ʾ��
	{
        LCD_SetPosition(call_call_icon7_coord[0][0], call_call_icon7_coord[0][1], 
                        ICON_CALL_ICON7_WIDE, ICON_CALL_ICON7_HIGH);
        LCD_DriverWriteDataFromExtFlash(call_call_icon7_addr[language % 2], ICON_CALL_ICON7_SIZE);
	}
	
	// ������ʾ��
	{
        LCD_SetPosition(call_call_icon8_coord[0][0], call_call_icon8_coord[0][1], 
                        ICON_CALL_ICON8_WIDE, ICON_CALL_ICON8_HIGH);
        LCD_DriverWriteDataFromExtFlash(call_call_icon8_addr[language % 2], ICON_CALL_ICON8_SIZE);
	}
    *cls_flag = 1;
}

/****************************************************************
** Function name:           Gui_PageCallNotificationContent


** Descriptions:            ��ʾ������Ϣ���ͽ���
** input parameters:        display_buf����Ϣ����ָ��
                            buf_len�����ݳ���
** output parameters:       
** Returned value:          
** Created by:              WJ             
** Created Date:            2017-08-11
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Gui_PageCallNotificationContent(uint16_t index, uint8_t *display_buf, uint16_t buf_len, uint8_t *cls_flag)
{
    //��Ҫ���
    Font_DisUiTextOneLine2X(index, display_buf, buf_len, 120, 25, 1, 0, 0, 0X00);
}

/****************************************************************
** Function name:           Gui_PageMessageNotificationIcon
** Descriptions:            ��ʾ��Ϣ���ͽ���
** input parameters:        msg_type����Ϣ������
**                          page_index����̬ͼ����
** output parameters:       
** Returned value:          
** Created by:              YX              
** Created Date:            2017-08-11
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Gui_PageMessageNotificationIcon(uint16_t index, uint8_t msg_type, uint8_t page_index, uint8_t hour, uint8_t minute, uint8_t *cls_flag)
{   
    #if TEST_CLEAR_SCREEN_PARAM
    if(index == 0)
    {
        Gui_PageRefreshBackground(0,0,LCD_MAX_COLUMN,LCD_MAX_ROW,0x00F8);
    }
    #endif //TEST_CLEAR_SCREEN_PARAM
        
    // ��Ļ��ȫ��
    if(index == 0 && *cls_flag != 0)
    {
        Gui_PageClearBackground(0, 
                                0, 
                                MAX_X_SIZE,
                                MAX_Y_SIZE);
    }
    // ��Ϣ����ͼ��
    if(msg_type >= 1)
    {
        LCD_SetPosition(push_inform_icon1_coord[0][0], push_inform_icon1_coord[0][1], 
                        ICON_INFORM_ICON1_WIDE, ICON_INFORM_ICON1_HIGH);
        LCD_DriverWriteDataFromExtFlash(push_inform_icon1_addr[msg_type - 1], ICON_INFORM_ICON1_SIZE); 
    }

    // ʱ��
    {
        // ����ʱ��--ʱ
        LCD_SetPosition(push_inform_data_coord[0][0], push_inform_data_coord[0][1],
                        ICON_INFORM_DATA_WIDE, ICON_INFORM_DATA_HIGH);
        LCD_DriverWriteDataFromExtFlash(push_inform_data_addr[(hour / 10) % 10], ICON_INFORM_DATA_SIZE);
        LCD_SetPosition(push_inform_data_coord[1][0], push_inform_data_coord[1][1],
                        ICON_INFORM_DATA_WIDE, ICON_INFORM_DATA_HIGH);
        LCD_DriverWriteDataFromExtFlash(push_inform_data_addr[hour % 10], ICON_INFORM_DATA_SIZE);
        
        // ����ʱ��--�� 
        LCD_SetPosition(push_inform_data_coord[2][0], push_inform_data_coord[2][1],
                        ICON_INFORM_DATA_WIDE, ICON_INFORM_DATA_HIGH);
        LCD_DriverWriteDataFromExtFlash(push_inform_data_addr[(minute / 10) % 10], ICON_INFORM_DATA_SIZE);
        LCD_SetPosition(push_inform_data_coord[3][0], push_inform_data_coord[3][1],
                        ICON_INFORM_DATA_WIDE, ICON_INFORM_DATA_HIGH);
        LCD_DriverWriteDataFromExtFlash(push_inform_data_addr[minute % 10], ICON_INFORM_DATA_SIZE); 
        
        // ����ʱ��--ð��
        LCD_SetPosition(push_inform_datadot_coord[0][0], push_inform_datadot_coord[0][1],
                        ICON_INFORM_DATADOT_WIDE, ICON_INFORM_DATADOT_HIGH);
        LCD_DriverWriteDataFromExtFlash(push_inform_datadot_addr[0], ICON_INFORM_DATADOT_SIZE);
    }
    
    *cls_flag = 1;
}
 
/****************************************************************
** Function name:           Gui_PageMessageNotificationContent
** Descriptions:            ��ʾ��Ϣ���ͽ���
** input parameters:        display_buf����Ϣ����ָ��
                            buf_len�����ݳ���
                            page_index:�ڼ�ҳ
** output parameters:       
** Returned value:          
** Created by:              WJ             
** Created Date:            2017-08-11
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
uint8_t Gui_PageMessageNotificationContent(uint16_t index, uint8_t *display_buf, uint16_t buf_len, uint8_t *cls_flag)
{
 	// ��Ļ�ײ�-����
    if(index == 0)
    {
//        Gui_PageClearBackground(0, 
//                                0, 
//                                MAX_X_SIZE,
//                                MAX_Y_SIZE);
    }   
    *cls_flag = 1;
    
    return Font_DisplayContentTextFirstPage2X(display_buf, buf_len, 90, 3);
}

/****************************************************************
** Function name:           Gui_PageStarget
** Descriptions:            �˶�Ŀ�����ѽ���
** input parameters:        index����̬ͼ����
** output parameters:       ��
** Returned value:          ��
** Created by:              YX
** Created Date:            2017-09-11
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/
void Gui_PageStarget(uint16_t index, uint32_t step_target_cnt, uint8_t language, uint8_t *cls_flag)
{
    #if TEST_CLEAR_SCREEN_PARAM
    if(index == 0)
    {
        Gui_PageRefreshBackground(0,0,LCD_MAX_COLUMN,LCD_MAX_ROW,0x00F8);
    }
    #endif //TEST_CLEAR_SCREEN_PARAM
    
    if(language)
    {
        language = 1;
    }
    
    // ͼ���ϲ�����
    if(index == 0)
    {
		//������
        Gui_PageClearBackground(0, 
                                0, 
                                MAX_X_SIZE,
                                sport1_standard_icon1_coord[0][1]);
        //������
        Gui_PageClearBackground(0, 
                                0, 
                                sport1_standard_icon1_coord[0][0],
                                MAX_Y_SIZE);
        //������
        Gui_PageClearBackground(0, 
                                sport1_standard_icon1_coord[0][1] + ICON_STANDARD_ICON1_HIGH, 
                                MAX_X_SIZE,
                                MAX_Y_SIZE - (sport1_standard_icon1_coord[0][1] + ICON_STANDARD_ICON1_HIGH));
        //������
        Gui_PageClearBackground(sport1_standard_icon1_coord[0][0] + ICON_STANDARD_ICON1_WIDE, 
                                0, 
                                MAX_X_SIZE - (sport1_standard_icon1_coord[0][0] + ICON_STANDARD_ICON1_WIDE),
                                MAX_Y_SIZE);;   


    }
     
    // ��ͼ
    LCD_SetPosition(sport1_standard_icon1_coord[0][0], sport1_standard_icon1_coord[0][1], 
                   ICON_STANDARD_ICON1_WIDE, ICON_STANDARD_ICON1_HIGH);
    LCD_DriverWriteDataFromExtFlash(sport1_standard_icon1_addr[index%5], ICON_STANDARD_ICON1_SIZE);
   
    // ��λ
    LCD_SetPosition(sport1_standard_icon2_coord[0][0], sport1_standard_icon2_coord[0][1], 
                   ICON_STANDARD_ICON2_WIDE, ICON_STANDARD_ICON2_HIGH);
    LCD_DriverWriteDataFromExtFlash(sport1_standard_icon2_addr[language%2], ICON_STANDARD_ICON2_SIZE);  
    
    *cls_flag = 1;
}

/****************************************************************
** Function name:           Gui_PageBleDiscRemind
** Descriptions:            ������������
** input parameters:        index����̬ͼ����
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ
** Created Date:            2018-04-03
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/
void Gui_PageBleDiscRemind(uint16_t index, uint8_t *cls_flag)
{
    #if TEST_CLEAR_SCREEN_PARAM
    if(index == 0)
    {
        Gui_PageRefreshBackground(0,0,LCD_MAX_COLUMN,LCD_MAX_ROW,0x00F8);
    }
    #endif //TEST_CLEAR_SCREEN_PARAM
    
    // ͼ���ϲ�����
    if(index == 0 && *cls_flag != 0)
    {
      	//������
        Gui_PageClearBackground(0, 
                                0, 
                                MAX_X_SIZE,
                                MAX_Y_SIZE);
////		//������
////        Gui_PageClearBackground(0, 
////                                0, 
////                                MAX_X_SIZE,
////                                disconnected_disconnected_icon1_coord[0][1]);
////        //������
////        Gui_PageClearBackground(0, 
////                                0, 
////                                disconnected_disconnected_icon1_coord[0][0],
////                                MAX_Y_SIZE);
////        //������
////        Gui_PageClearBackground(0, 
////                                disconnected_disconnected_icon1_coord[0][1] + ICON_DISCONNECTED_ICON1_HIGH, 
////                                MAX_X_SIZE,
////                                MAX_Y_SIZE - (disconnected_disconnected_icon1_coord[0][1] + ICON_DISCONNECTED_ICON1_HIGH));
////        //������
////        Gui_PageClearBackground(disconnected_disconnected_icon1_coord[0][0] + ICON_DISCONNECTED_ICON1_WIDE, 
////                                0, 
////                                MAX_X_SIZE - (disconnected_disconnected_icon1_coord[0][0] + ICON_DISCONNECTED_ICON1_WIDE),
////                                MAX_Y_SIZE);
    }
         
    if(index == 0)
    {
        // ����
        LCD_SetPosition(disconnected_disconnected_icon1_coord[0][0], disconnected_disconnected_icon1_coord[0][1], 
                       ICON_DISCONNECTED_ICON1_WIDE, ICON_DISCONNECTED_ICON1_HIGH);
        LCD_DriverWriteDataFromExtFlash(disconnected_disconnected_icon1_addr[0], ICON_DISCONNECTED_ICON1_SIZE);
        
    }
    
    // ��ͼ
    LCD_SetPosition(disconnected_disconnected_icon2_coord[0][0], disconnected_disconnected_icon2_coord[0][1], 
                   ICON_DISCONNECTED_ICON2_WIDE, ICON_DISCONNECTED_ICON2_HIGH);
    LCD_DriverWriteDataFromExtFlash(disconnected_disconnected_icon2_addr[index%5], ICON_DISCONNECTED_ICON2_SIZE);  
    *cls_flag = 1;
}

/****************************************************************
** Function name:           Gui_PagePtt
** Descriptions:            ��ʾPtt����
** input parameters:        index����̬ͼ����
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2018-11-09
*****************************************************************/ 
void Gui_PagePtt(uint16_t index, uint8_t *cls_flag)
{
//    LCD_SetPosition(icon_ptt_dynamic_coord[0][0], icon_ptt_dynamic_coord[0][1],
//                    ICON_PTT_ICON1_WIDE, ICON_PTT_ICON1_HIGH);
//    LCD_DriverWriteDataFromExtFlash(icon_ptt_dynamic_addr[index % 4], ICON_PTT_ICON1_SIZE);
//    
//    
//    LCD_SetPosition(icon_ptt_ring_coord[0][0], icon_ptt_ring_coord[0][1], 
//                    ICON_FINDPHONE_ICON4_WIDE, ICON_FINDPHONE_ICON4_HIGH);
//    LCD_DriverWriteDataFromExtFlash(icon_ptt_ring_addr[0], ICON_FINDPHONE_ICON4_SIZE); 
//    
//    
//    
//    LCD_SetPosition(icon_ptt_text_coord[0][0], icon_ptt_text_coord[0][1],
//                    ICON_MOVE1_ICON5_WIDE, ICON_MOVE1_ICON5_HIGH);   
//    if(display_language == 0)
//    {
//        LCD_DriverWriteDataFromExtFlash(icon_ptt_text_addr[0], ICON_MOVE1_ICON5_SIZE);;
//    }
//    else
//    {
//        LCD_DriverWriteDataFromExtFlash(icon_ptt_text_addr[1], ICON_MOVE1_ICON5_SIZE);
//    } 
    *cls_flag = 1;
}

/****************************************************************
** Function name:           Gui_PagePttItem1
** Descriptions:            ��ʾPtt����-�ӽ���1
** input parameters:        index����̬ͼ����
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2018-11-09
*****************************************************************/ 
void Gui_PagePttItem1(uint16_t index, uint8_t state, uint8_t *cls_flag)
{
//    LCD_SetPosition(icon_ptt2_dynamic_coord[0][0], icon_ptt2_dynamic_coord[0][1],
//                    ICON_PTT2_ICON1_WIDE, ICON_PTT2_ICON1_HIGH);
//    LCD_DriverWriteDataFromExtFlash(icon_ptt2_dynamic_addr[index % 5], ICON_PTT2_ICON1_SIZE);
//    
//    LCD_SetPosition(icon_ptt2_text_coord[0][0], icon_ptt2_text_coord[0][1],
//                    ICON_PTT2_ICON2_WIDE, ICON_PTT2_ICON2_HIGH);
//    if(display_language == 0)
//    {
//        LCD_DriverWriteDataFromExtFlash(icon_ptt2_text_addr[0], ICON_PTT2_ICON2_SIZE);
//    }
//    else
//    {
//        LCD_DriverWriteDataFromExtFlash(icon_ptt2_text_addr[1], ICON_PTT2_ICON2_SIZE);
//    }
    *cls_flag = 1;
}

/****************************************************************
** Function name:           Gui_PageEcg
** Descriptions:            ��ʾEcg����
** input parameters:        index����̬ͼ����
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2018-11-09
*****************************************************************/ 
void Gui_PageEcg(uint16_t index, uint8_t hour, uint8_t minute, uint8_t language, uint8_t *cls_flag)
{
    #if TEST_CLEAR_SCREEN_PARAM
    if(index == 0)
    {
        Gui_PageRefreshBackground(0,0,LCD_MAX_COLUMN,LCD_MAX_ROW,0x00F8);
    }
    #endif //TEST_CLEAR_SCREEN_PARAM
    
    if(language)
    {
        language = 1;
    }
    
    
    if(index == 0 )
    {    
        //ECG��ͼ��        
        Gui_PageClearBackground(0, 
                                0, 
                                MAX_X_SIZE,
                                icon_16_ecg1_icon_coord[0][1]);
        //ECG��ͼ�� 
        Gui_PageClearBackground(0, 
                                0, 
                                icon_16_ecg1_icon_coord[0][0],
                                MAX_Y_SIZE);
        //ECG��ͼ�� 
        Gui_PageClearBackground(0, 
                                icon_16_ecg1_icon_coord[0][1] + ICON_ECG_ICON5_HIGH, 
                                MAX_X_SIZE,
                                MAX_Y_SIZE - (icon_16_ecg1_icon_coord[0][1] + ICON_ECG_ICON5_HIGH));
        //ECG��ͼ�� 
        Gui_PageClearBackground(icon_16_ecg1_icon_coord[0][0] + ICON_ECG_ICON5_WIDE, 
                                0, 
                                MAX_X_SIZE - (icon_16_ecg1_icon_coord[0][0] + ICON_ECG_ICON5_WIDE),
                                MAX_Y_SIZE);

    }
    
    // ECG��ͼ
    {
        LCD_SetPosition(icon_16_ecg1_icon_coord[0][0], icon_16_ecg1_icon_coord[0][1], 
                        ICON_ECG_ICON5_WIDE, ICON_ECG_ICON5_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_ecg1_icon_addr[index % 2], ICON_ECG_ICON5_SIZE);
        
        
        LCD_SetPosition(icon_16_ecg1_label_coord[0][0], icon_16_ecg1_label_coord[0][1], 
                        ICON_ECG_ICON6_WIDE, ICON_ECG_ICON6_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_ecg1_label_addr[0], ICON_ECG_ICON6_SIZE);        
    } 
    *cls_flag = 2;
}

/****************************************************************
** Function name:           Gui_PagePttItem1
** Descriptions:            ��ʾECG����-�ӽ���1
** input parameters:        index����̬ͼ����
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2018-11-09
*****************************************************************/ 
void Gui_PageEcgItem1(uint16_t index, uint8_t state, uint8_t heart, uint8_t language, uint8_t *cls_flag)
{
#if TEST_CLEAR_SCREEN_PARAM
    if(index == 0)
    {
        Gui_PageRefreshBackground(0,0,LCD_MAX_COLUMN,LCD_MAX_ROW,0x00F8);
    }
#endif //TEST_CLEAR_SCREEN_PARAM
    
    if(index == 0 )
    {
        //����
        LCD_SetPosition(heart_share_bg_coord[0][0], heart_share_bg_coord[0][1], 
                             ICON_SHARE_BG_WIDE, ICON_SHARE_BG_HIGH);
        LCD_DriverWriteDataFromExtFlash(heart_share_bg_addr[0], ICON_SHARE_BG_SIZE);    
    }

    
    // ECG��ͼ
    LCD_SetPosition(ecg_ECG_icon1_coord[0][0], ecg_ECG_icon1_coord[0][1], 
                    ICON_ECG_ICON1_WIDE, ICON_ECG_ICON1_HIGH);
    LCD_DriverWriteDataFromExtFlash(ecg_ECG_icon1_addr[index % 2], ICON_ECG_ICON1_SIZE);

 
    // ���ʵ�λ
    LCD_SetPosition(ecg_heart_unit1_coord[0][0], ecg_heart_unit1_coord[0][1], 
                    ICON_HEART_UNIT1_WIDE, ICON_HEART_UNIT1_HIGH);
    LCD_DriverWriteDataFromExtFlash(ecg_heart_unit1_addr[language % 2], ICON_HEART_UNIT1_SIZE);
    
    
    // ECG��λ   
    LCD_SetPosition(ecg_ECG_bg1_coord[0][0], ecg_ECG_bg1_coord[0][1], 
                    ICON_ECG_BG1_WIDE, ICON_ECG_BG1_HIGH);
    LCD_DriverWriteDataFromExtFlash(ecg_ECG_bg1_addr[0], ICON_ECG_BG1_SIZE);        
 
    
	//ECG��ֵ	
    if(heart == 0)
    {
        for(uint8_t i = 0; i < 3; i++)
        {
            LCD_SetPosition(ecg_share_data1dot_coord[i][0], ecg_share_data1dot_coord[i][1], 
                            ICON_SHARE_DATA1DOT_WIDE, ICON_SHARE_DATA1DOT_HIGH);
            LCD_DriverWriteDataFromExtFlash(ecg_share_data1dot_addr[0], ICON_SHARE_DATA1DOT_SIZE);
                
        }          
    }
    else
    {
        
        LCD_SetPosition(ecg_share_data1dot_coord[0][0], ecg_share_data1dot_coord[0][1], 
                        ICON_SHARE_DATA1_WIDE, ICON_SHARE_DATA1_HIGH);
        if(heart >= 100)
        {    
            LCD_DriverWriteDataFromExtFlash(ecg_share_data1_addr[(heart / 100) %10], ICON_SHARE_DATA1_SIZE);        
        }  
        else
        {    
            LCD_DriverWriteDataFromExtFlash(ecg_share_data1_addr[0], ICON_SHARE_DATA1_SIZE);        
        }        
        
        
        LCD_SetPosition(ecg_share_data1dot_coord[0][0], ecg_share_data1dot_coord[1][1], 
                        ICON_SHARE_DATA1_WIDE, ICON_SHARE_DATA1_HIGH);
        if(heart >= 10)
        {    
            LCD_DriverWriteDataFromExtFlash(ecg_share_data1_addr[(heart / 10) %10], ICON_SHARE_DATA1_SIZE);        
        }  
        else
        {    
            LCD_DriverWriteDataFromExtFlash(ecg_share_data1_addr[0], ICON_SHARE_DATA1_SIZE);        
        }
        
        LCD_SetPosition(ecg_share_data1dot_coord[2][0], ecg_share_data1dot_coord[2][1], 
                        ICON_SHARE_DATA1_WIDE, ICON_SHARE_DATA1_HIGH);
        if(heart > 0)
        {    
            LCD_DriverWriteDataFromExtFlash(ecg_share_data1_addr[heart %10], ICON_SHARE_DATA1_SIZE);        
        }  
        else
        {    
            LCD_DriverWriteDataFromExtFlash(ecg_share_data1_addr[0], ICON_SHARE_DATA1_SIZE);        
        }
        
    }
    *cls_flag = 1;
}

/****************************************************************
** Function name:           Gui_PageEcgItem2
** Descriptions:            ��ʾPtt����-�ӽ���2
** input parameters:        index����̬ͼ����
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2018-11-09
*****************************************************************/ 
void Gui_PageEcgItem2(uint16_t index, uint8_t state, uint8_t language, uint8_t *cls_flag)
{
    #if TEST_CLEAR_SCREEN_PARAM
    if(index == 0)
    {
        Gui_PageRefreshBackground(0,0,LCD_MAX_COLUMN,LCD_MAX_ROW,0x00F8);
    }
    #endif //TEST_CLEAR_SCREEN_PARAM
    
    if(index == 0 )
    {
        //����
        LCD_SetPosition(heart_share_bg_coord[0][0], heart_share_bg_coord[0][1], 
                             ICON_SHARE_BG_WIDE, ICON_SHARE_BG_HIGH);
        LCD_DriverWriteDataFromExtFlash(heart_share_bg_addr[0], ICON_SHARE_BG_SIZE); 
    }
    
    //����ͼ������
    LCD_SetPosition(ecg_ECG_icon1_coord[0][0], ecg_ECG_icon1_coord[0][1],
                    ICON_ECG_ICON1_WIDE, ICON_ECG_ICON1_HIGH);
    LCD_DriverWriteDataFromExtFlash(ecg_ECG_icon1_addr[0], ICON_ECG_ICON1_SIZE);

    //���Խ��ͼ��  state : 0 ���Գ��� 1���ɹ� 2����������
    LCD_SetPosition(ecg_ECG_icon3_coord[0][0], ecg_ECG_icon3_coord[0][1],
                    ICON_ECG_ICON3_WIDE, ICON_ECG_ICON3_HIGH);
    LCD_DriverWriteDataFromExtFlash(ecg_ECG_icon3_addr[state%3], ICON_ECG_ICON3_SIZE);
    
    //���Խ��������ʾ
    LCD_SetPosition(ecg_ECG_icon2_coord[0][0], ecg_ECG_icon2_coord[0][1],
                    ICON_ECG_ICON2_WIDE, ICON_ECG_ICON2_HIGH);
    LCD_DriverWriteDataFromExtFlash(ecg_ECG_icon2_addr[state%3][language%2], ICON_ECG_ICON2_SIZE);
    *cls_flag = 1;
}


/****************************************************************
** Function name:           Gui_PageOxygen
** Descriptions:            Ѫ������
** input parameters:        index����̬ͼ����
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2018-05-26
****************************************************************/
void Gui_PageOxygen(uint16_t index, uint8_t progress, uint8_t oxygen_cnt, 
                    uint8_t hour, uint8_t minute, uint8_t language, uint8_t *cls_flag)
{
    #if TEST_CLEAR_SCREEN_PARAM
    if(index == 0)
    {
        Gui_PageRefreshBackground(0,0,LCD_MAX_COLUMN,LCD_MAX_ROW,0x00F8);
    }
    #endif //TEST_CLEAR_SCREEN_PARAM
    
    //Ѫ��ֵoxygen_cntΪ0ʱ,����progress��Ч
    static uint8_t last_oxygen = 0;
    
    if(language)
    {
        language = 1;
    }
    
    if(index == 0)
    {
        // Ѫ������
        LCD_SetPosition(heart_share_bg_coord[0][0], heart_share_bg_coord[0][1], 
                             ICON_SHARE_BG_WIDE, ICON_SHARE_BG_HIGH);
        LCD_DriverWriteDataFromExtFlash(heart_share_bg_addr[0], ICON_SHARE_BG_SIZE);    
    }

    // Ѫ����ͼ
    LCD_SetPosition(oxygen_spo2_icon1_coord[0][0], oxygen_spo2_icon1_coord[0][1], 
                    ICON_SPO2_ICON1_WIDE, ICON_SPO2_ICON1_HIGH);
    LCD_DriverWriteDataFromExtFlash(oxygen_spo2_icon1_addr[index % 2], ICON_SPO2_ICON1_SIZE);

    // Ѫ����λ
    LCD_SetPosition(oxygen_spo2_icon3_coord[0][0], oxygen_spo2_icon3_coord[0][1], 
                    ICON_SPO2_ICON3_WIDE, ICON_SPO2_ICON3_HIGH);
    LCD_DriverWriteDataFromExtFlash(oxygen_spo2_icon3_addr[language % 2], ICON_SPO2_ICON3_SIZE);
        
	//Ѫ����ֵ	
    if(oxygen_cnt == 0)
    {
        
        for(uint8_t i = 0; i < 2; i++)
        {
            LCD_SetPosition(icon_16_spo2_data_coord[i][0], icon_16_spo2_data_coord[i][1], 
                            ICON_SHARE_DATA1DOT_WIDE, ICON_SHARE_DATA1DOT_HIGH);
            LCD_DriverWriteDataFromExtFlash(heart_share_data1dot_addr[0], ICON_SHARE_DATA1DOT_SIZE);
                
        }      
    }
    else
    {
        
        //Ѫ����ֵ
        LCD_SetPosition(icon_16_spo2_data_coord[0][0], icon_16_spo2_data_coord[0][1], 
                        ICON_SHARE_DATA1_WIDE, ICON_SHARE_DATA1_HIGH);
        if(oxygen_cnt >= 10)
        {    
            LCD_DriverWriteDataFromExtFlash(ecg_share_data1_addr[(oxygen_cnt / 10) %10], ICON_SHARE_DATA1_SIZE);        
        }  
        else
        {    
            LCD_DriverWriteDataFromExtFlash(ecg_share_data1_addr[0], ICON_SHARE_DATA1_SIZE);        
        }
        
        LCD_SetPosition(icon_16_spo2_data_coord[1][0], icon_16_spo2_data_coord[1][1], 
                        ICON_SHARE_DATA1_WIDE, ICON_SHARE_DATA1_HIGH);
        if(oxygen_cnt > 0)
        {    
            LCD_DriverWriteDataFromExtFlash(ecg_share_data1_addr[oxygen_cnt %10], ICON_SHARE_DATA1_SIZE);        
        }  
        else
        {    
            LCD_DriverWriteDataFromExtFlash(ecg_share_data1_addr[0], ICON_SHARE_DATA1_SIZE);        
        }
        
    }
    
    //�ٷֺ�
    LCD_SetPosition(oxygen_spo2_icon2_coord[0][0], oxygen_spo2_icon2_coord[0][1], 
                    ICON_SPO2_ICON2_WIDE, ICON_SPO2_ICON2_HIGH);
    LCD_DriverWriteDataFromExtFlash(oxygen_spo2_icon2_addr[0], ICON_SPO2_ICON2_SIZE); 
       
    last_oxygen = oxygen_cnt;
    *cls_flag = 2;
}
 /****************************************************************
** Function name:           Gui_PageFunct
** Descriptions:            ��ʾ���๦�ܽ���
** input parameters:        index����Ϣ����ָ��
                            language�����ݳ���
** output parameters:       ��
** Returned value:          ��
** Created by:              JSH              
** Created Date:            2018-09-30
**----------------------------------------------------------------
*****************************************************************/
void Gui_PageFunct(uint16_t index, uint8_t language)
{
    #if TEST_CLEAR_SCREEN_PARAM
    if(index == 0)
    {
        Gui_PageRefreshBackground(0,0,LCD_MAX_COLUMN,LCD_MAX_ROW,0x00F8);
    }
    #endif //TEST_CLEAR_SCREEN_PARAM
    
    if(index == 0)
    {
        //������
        Gui_PageClearBackground(0, 
                                0, 
                                MAX_X_SIZE,
                                more_more_icon1_coord[0][1]);
        //������
        Gui_PageClearBackground(0, 
                                0, 
                                more_more_icon1_coord[0][0],
                                MAX_Y_SIZE);
        //������
        Gui_PageClearBackground(0, 
                                more_more_icon1_coord[0][1] + ICON_MORE_ICON1_HIGH, 
                                MAX_X_SIZE,
                                MAX_Y_SIZE - (more_more_icon1_coord[0][1] + ICON_MORE_ICON1_HIGH));
        //������
        Gui_PageClearBackground(more_more_icon1_coord[0][0] + ICON_MORE_ICON1_WIDE, 
                                0, 
                                MAX_X_SIZE - (more_more_icon1_coord[0][0] + ICON_MORE_ICON1_WIDE),
                                MAX_Y_SIZE);

    }
    
    if(index == 0)
    {
                //����
        LCD_SetPosition(more_more_icon1_coord[0][0], more_more_icon1_coord[0][1], 
                             ICON_MORE_ICON1_WIDE, ICON_MORE_ICON1_HIGH);
        LCD_DriverWriteDataFromExtFlash(more_more_icon1_addr[0], ICON_MORE_ICON1_SIZE);
        
    }
    
    LCD_SetPosition(more_more_icon2_coord[0][0], more_more_icon2_coord[0][1], 
                             ICON_MORE_ICON2_WIDE, ICON_MORE_ICON2_HIGH);
    LCD_DriverWriteDataFromExtFlash(more_more_icon2_addr[language % 2], ICON_MORE_ICON2_SIZE);
    
}

/****************************************************************
** Function name:           Gui_PageFunct2
** Descriptions:            ��ʾ���๦�ܽ���
** input parameters:        index����Ϣ����ָ��
                            language�����ݳ���
** output parameters:       ��
** Returned value:          ��
** Created by:              JSH              
** Created Date:            2018-09-30
**----------------------------------------------------------------
*****************************************************************/
void Gui_PageFunct2(uint16_t index, uint8_t page, uint8_t num, uint8_t language)
{
    static uint8_t page_num_bk = 0xFF;
    uint8_t page_num = 0;
    page_num = page*16 + num;
    
    #if TEST_CLEAR_SCREEN_PARAM
    if(index == 0)
    {
        Gui_PageRefreshBackground(0,0,LCD_MAX_COLUMN,LCD_MAX_ROW,0x00F8);
    }
    #endif //TEST_CLEAR_SCREEN_PARAM
    
    
    if(index == 0)
    {
        Gui_PageClearBackground(0, 0, MAX_X_SIZE, MAX_Y_SIZE);
        
        LCD_SetPosition(moresetup_more1_icon1_coord[0][0], moresetup_more1_icon1_coord[0][1], 
                            ICON_MORE1_ICON1_WIDE, ICON_MORE1_ICON1_HIGH);
        LCD_DriverWriteDataFromExtFlash(moresetup_more1_icon1_addr[page % 3], ICON_MORE1_ICON1_SIZE);
        
        for(uint8_t i = 0; i < 4; i++)
        {
            if(page % 3 == 2 && i == 3)
            {
                break;
            }
            LCD_SetPosition(moresetup_more1_icon3_coord[i][0], moresetup_more1_icon3_coord[i][1], 
                                 ICON_MORE1_ICON3_WIDE, ICON_MORE1_ICON3_HIGH);
            LCD_DriverWriteDataFromExtFlash(moresetup_more1_icon3_addr[(page % 3)*4 + i], ICON_MORE1_ICON3_SIZE);
            
            LCD_SetPosition(moresetup_more1_icon4_coord[i][0], moresetup_more1_icon4_coord[i][1], 
                                 ICON_MORE1_ICON4_WIDE, ICON_MORE1_ICON4_HIGH);
            LCD_DriverWriteDataFromExtFlash(moresetup_more1_icon4_addr[language % 2][(page % 3)*4 + i], ICON_MORE1_ICON4_SIZE);
        }
    } 
    
    if(page_num_bk != page_num || index == 0)
    {
        //if((index == 0 && slid_param_s.slid_flag == 0) || (index >= 1))
        {
            page_num_bk = page_num;
        }
        for(uint8_t i = 0; i < 4; i++)
        {
            if(i != num)
            {
                Gui_PageClearBackground(moresetup_more1_icon2_coord[i % 4][0], moresetup_more1_icon2_coord[i % 4][1], 
                                            ICON_MORE1_ICON2_WIDE, ICON_MORE1_ICON2_HIGH);
            }
            else
            {
                LCD_SetPosition(moresetup_more1_icon2_coord[num % 4][0], moresetup_more1_icon2_coord[num % 4][1], 
                                     ICON_MORE1_ICON2_WIDE, ICON_MORE1_ICON2_HIGH);
                LCD_DriverWriteDataFromExtFlash(moresetup_more1_icon2_addr[0], ICON_MORE1_ICON2_SIZE);
            }
        }
        
        
    }
}

/****************************************************************
** Function name:           Gui_PageMessageSwitch
** Descriptions:            ��Ϣ�������ý���
** input parameters:        index����Ϣ����ָ��
                            language�����ݳ���
** output parameters:       ��
** Returned value:          ��
** Created by:              JSH              
** Created Date:            2018-09-30
**----------------------------------------------------------------
*****************************************************************/
void Gui_PageMessageSwitch(uint16_t index, uint8_t page, uint8_t num, uint8_t* switch_tmp, uint8_t language)
{
    #if TEST_CLEAR_SCREEN_PARAM
    if(index == 0)
    {
        Gui_PageClearBackground(0,0,LCD_MAX_COLUMN,LCD_MAX_ROW,0x00F8);
    }
    #endif //TEST_CLEAR_SCREEN_PARAM
    
    if(index == 0)
    {
        Gui_PageClearBackground(0, 0, MAX_X_SIZE, MAX_Y_SIZE);
        
        LCD_SetPosition(icon_16_message3_icon1_coord[0][0], icon_16_message3_icon1_coord[0][1], 
                             ICON_MESSAGE3_ICON1_WIDE, ICON_MESSAGE3_ICON1_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_message3_icon1_addr[page % 5], ICON_MESSAGE3_ICON1_SIZE);
        
        for(uint8_t i = 0; i < 4; i++)
        {
            if(page % 5 == 4 && i == 3)
            {
                break;
            }
            LCD_SetPosition(icon_16_message3_icon2_coord[i][0], icon_16_message3_icon2_coord[i][1], 
                                 ICON_MESSAGE3_ICON2_WIDE, ICON_MESSAGE3_ICON2_HIGH);
            LCD_DriverWriteDataFromExtFlash(icon_16_message3_icon2_addr[(page % 5)*4 + i], ICON_MESSAGE3_ICON2_SIZE);
            
            LCD_SetPosition(icon_16_message3_icon4_coord[i][0], icon_16_message3_icon4_coord[i][1], 
                                 ICON_MESSAGE3_ICON4_WIDE, ICON_MESSAGE3_ICON4_HIGH);
            LCD_DriverWriteDataFromExtFlash(icon_16_message3_icon4_addr[language % 2][(page % 5)*4 + i], ICON_MESSAGE3_ICON4_SIZE);
            
        }
    } 
    
    if(num > 0)
    {
        for(uint8_t i = 0; i < num; i++)
        {
            Gui_PageClearBackground(icon_16_message3_icon3_coord[i % 4][0], icon_16_message3_icon3_coord[i % 4][1], 
                                         ICON_MESSAGE3_ICON3_WIDE, ICON_MESSAGE3_ICON3_HIGH);
        }
    }
    
    LCD_SetPosition(icon_16_message3_icon3_coord[num % 4][0], icon_16_message3_icon3_coord[num % 4][1], 
                         ICON_MESSAGE3_ICON3_WIDE, ICON_MESSAGE3_ICON3_HIGH);
    LCD_DriverWriteDataFromExtFlash(icon_16_message3_icon3_addr[0], ICON_MESSAGE3_ICON3_SIZE);
    
    for(uint8_t i = 0; i < 3; i++)
    {
        if(page % 5 == 4 && i == 2)
        {
            break;
        }
        LCD_SetPosition(icon_16_switch_icon2_coord[i][0], icon_16_switch_icon2_coord[i][1], 
                             ICON_MORE2_ICON2_WIDE, ICON_MORE2_ICON2_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_switch_icon2_addr[switch_tmp[i]%2], ICON_MORE2_ICON2_SIZE);
    }

} 
/****************************************************************
** Function name:           Gui_PageSwitch
** Descriptions:            �������ý���
** input parameters:        index����Ϣ����ָ��
                            language�����ݳ���
** output parameters:       ��
** Returned value:          ��
** Created by:              JSH              
** Created Date:            2018-09-30
**----------------------------------------------------------------
*****************************************************************/
void Gui_PageSwitch(uint16_t index, uint8_t page, uint8_t num, uint8_t* switch_tmp, uint8_t language)
{
    #if TEST_CLEAR_SCREEN_PARAM
    if(index == 0)
    {
        Gui_PageClearBackground(0,0,LCD_MAX_COLUMN,LCD_MAX_ROW,0x00F8);
    }
    #endif //TEST_CLEAR_SCREEN_PARAM
    
    if(index == 0)
    {
        Gui_PageClearBackground(0, 0, MAX_X_SIZE, MAX_Y_SIZE);
        //��߻�����
        LCD_SetPosition(setupswitch1_more2_icon5_coord[0][0], setupswitch1_more2_icon5_coord[0][1], 
                             ICON_MORE1_ICON1_WIDE, ICON_MORE1_ICON1_HIGH);
        LCD_DriverWriteDataFromExtFlash(setupswitch1_more2_icon5_addr[page % 3], ICON_MORE1_ICON1_SIZE);
        
        for(uint8_t i = 0; i < 4; i++)
        {
//            if(page % 3 == 2 && i == 3)
//            {
//                break;
//            }
            LCD_SetPosition(setupswitch1_more2_icon3_coord[i][0], setupswitch1_more2_icon3_coord[i][1], 
                                ICON_MORE2_ICON3_WIDE, ICON_MORE2_ICON3_HIGH);
            LCD_DriverWriteDataFromExtFlash(setupswitch1_more2_icon3_addr[(page % 3)*4 + i], ICON_MORE2_ICON3_SIZE);
            
            LCD_SetPosition(setupswitch1_more2_icon4_coord[i][0], setupswitch1_more2_icon4_coord[i][1], 
                                ICON_MORE2_ICON4_WIDE, ICON_MORE2_ICON4_HIGH);
            LCD_DriverWriteDataFromExtFlash(setupswitch1_more2_icon4_addr[language % 2][(page % 3)*4 + i], ICON_MORE2_ICON4_SIZE);
        }
    } 
    
    if(num > 0)
    {
        for(uint8_t i = 0; i < num; i++)
        {
            Gui_PageClearBackground(icon_16_message3_icon3_coord[i % 4][0], icon_16_message3_icon3_coord[i % 4][1], 
                                    ICON_MORE2_ICON1_WIDE, ICON_MORE2_ICON1_HIGH);
        }
    }
    
    LCD_SetPosition(icon_16_message3_icon3_coord[num % 4][0], icon_16_message3_icon3_coord[num % 4][1], 
                        ICON_MORE2_ICON1_WIDE, ICON_MORE2_ICON1_HIGH);
    LCD_DriverWriteDataFromExtFlash(icon_16_message3_icon3_addr[0], ICON_MORE2_ICON1_SIZE);
    
    for(uint8_t i = 0; i < 3; i++)
    {
//        if(page % 3 == 2 && i == 2)
//        {
//            break;
//        }
        LCD_SetPosition(icon_16_switch_icon2_coord[i][0], icon_16_switch_icon2_coord[i][1], 
                             ICON_MORE2_ICON2_WIDE, ICON_MORE2_ICON2_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_switch_icon2_addr[switch_tmp[i]%2], ICON_MORE2_ICON2_SIZE);
    }
    
} 
/****************************************************************
** Function name:           Gui_PageLightLevelSet
** Descriptions:            ���ȵ������ý���
** input parameters:        index����̬ͼ��ļ���
                            language: ���ԣ�0���ġ�1Ӣ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2018-05-24
*****************************************************************/ 
void Gui_PageLightLevelSet(uint16_t index, uint8_t light_level, uint8_t language)
{	
    static uint8_t light_level_bk = 0;
    uint8_t light_level_temp = 0;
    float one_level_tmp = 80.0 / 12;
    uint8_t disp_level_num = 0;
    
    #if TEST_CLEAR_SCREEN_PARAM
    if(index == 0)
    {
        Gui_PageClearBackground(0,0,LCD_MAX_COLUMN,LCD_MAX_ROW,0x00F8);
    }
    #endif //TEST_CLEAR_SCREEN_PARAM
    
    if(language)
    {
        language = 1;
    }
    
     if(light_level >= 12)
    {
        light_level = 12;
    }
    
    light_level_temp = light_level;
    
    disp_level_num = one_level_tmp * light_level_temp;
    if(index == 0)
    {
        Gui_PageClearBackground(0, 0, MAX_X_SIZE, MAX_Y_SIZE);
    }
    if(index == 0 )
    {
        
        //������
        LCD_SetPosition(setupbrightness_set2_bg1_coord[0][0], setupbrightness_set2_bg1_coord[0][1],
                         ICON_SET2_BG1_WIDE, ICON_SET2_BG1_HIGH);
        LCD_DriverWriteDataFromExtFlash(setupbrightness_set2_bg1_addr[0], ICON_SET2_BG1_SIZE);
        //����
        LCD_SetPosition(setupbrightness_set2_icon1_coord[0][0], setupbrightness_set2_icon1_coord[0][1],
                         ICON_SET2_ICON1_WIDE, ICON_SET2_ICON1_HIGH);
        LCD_DriverWriteDataFromExtFlash(setupbrightness_set2_icon1_addr[0], ICON_SET2_ICON1_SIZE);
    }
    
    
    if(index == 0 || language != language_save)
    {
        LCD_SetPosition(setupbrightness_set2_icon3_coord[0][0], setupbrightness_set2_icon3_coord[0][1],
                         ICON_SET2_ICON3_WIDE, ICON_SET2_ICON3_HIGH);
        LCD_DriverWriteDataFromExtFlash(setupbrightness_set2_icon3_addr[language%2], ICON_SET2_ICON3_SIZE);
    }
    
    if(index == 0)
    {
        LCD_SetPosition(setupbrightness_set2_icon_coord[0][0], setupbrightness_set2_icon_coord[0][1],
                        ICON_SET2_ICON4_WIDE, ICON_SET2_ICON4_HIGH);
        LCD_DriverWriteDataFromExtFlash(setupbrightness_set2_icon_addr[1][0], ICON_SET2_ICON4_SIZE); 
    }
    
    if(light_level_bk != light_level || index == 0)
    {
        light_level_bk = light_level;
        for(uint8_t i = 0; i < disp_level_num; i++)
        {
            LCD_SetPosition(setupbrightness_set2_icon_coord[0][0] + 2 + 2*i, setupbrightness_set2_icon_coord[0][1],
                            ICON_SET2_ICON4_WIDE, ICON_SET2_ICON4_HIGH);
            LCD_DriverWriteDataFromExtFlash(setupbrightness_set2_icon_addr[1][1], ICON_SET2_ICON4_SIZE);         
        }
        
        for(uint8_t i = disp_level_num; i < 80; i++)
        {
            LCD_SetPosition(setupbrightness_set2_icon_coord[0][0] + 2 + 2*i, setupbrightness_set2_icon_coord[0][1],
                            ICON_SET2_ICON4_WIDE, ICON_SET2_ICON4_HIGH);
            LCD_DriverWriteDataFromExtFlash(setupbrightness_set2_icon_addr[0][1], ICON_SET2_ICON4_SIZE);         
        }
         
        if(disp_level_num >= 80)
        {
            LCD_SetPosition(setupbrightness_set2_icon_coord[1][0], setupbrightness_set2_icon_coord[1][1],
                            ICON_SET2_ICON4_WIDE, ICON_SET2_ICON4_HIGH);
            LCD_DriverWriteDataFromExtFlash(setupbrightness_set2_icon_addr[1][2], ICON_SET2_ICON4_SIZE); 
        }
        else
        {
            LCD_SetPosition(setupbrightness_set2_icon_coord[1][0], setupbrightness_set2_icon_coord[1][1],
                            ICON_SET2_ICON4_WIDE, ICON_SET2_ICON4_HIGH);
            LCD_DriverWriteDataFromExtFlash(setupbrightness_set2_icon_addr[0][2], ICON_SET2_ICON4_SIZE);
        }
        
        for(uint8_t i = 0; i < 80; i++)
        {
            LCD_SetPosition(setupbrightness_set2_icon5_coord[0][0] + 2*i, setupbrightness_set2_icon5_coord[0][1],
                                 ICON_SET2_ICON5_WIDE, ICON_SET2_ICON5_HIGH);
            LCD_DriverWriteDataFromExtFlash(setupbrightness_set2_icon5_addr[1], ICON_SET2_ICON5_SIZE); 
        }
        
        LCD_SetPosition(setupbrightness_set2_icon5_coord[0][0] + 2 + disp_level_num*2-10, setupbrightness_set2_icon5_coord[0][1],
                             ICON_SET2_ICON5_WIDE, ICON_SET2_ICON5_HIGH);
        LCD_DriverWriteDataFromExtFlash(setupbrightness_set2_icon5_addr[0], ICON_SET2_ICON5_SIZE);
        
        LCD_SetPosition(setupbrightness_set2_icon6_coord[0][0] + 2 + disp_level_num*2-10, setupbrightness_set2_icon6_coord[0][1],
                             ICON_SET2_ICON6_WIDE, ICON_SET2_ICON6_HIGH);
        LCD_DriverWriteDataFromExtFlash(setupbrightness_set2_icon6_addr[0], ICON_SET2_ICON6_SIZE);
        
        //����
        LCD_SetPosition(setuplocktime_set1_data1_coord[0][0], setuplocktime_set1_data1_coord[0][1],
                            ICON_SET1_DATA1_WIDE, ICON_SET1_DATA1_HIGH);
        LCD_DriverWriteDataFromExtFlash(setuplocktime_set1_data1_addr[light_level /10 % 10], ICON_SET1_DATA1_SIZE); 
        
        LCD_SetPosition(setuplocktime_set1_data1_coord[1][0], setuplocktime_set1_data1_coord[1][1],
                            ICON_SET1_DATA1_WIDE, ICON_SET1_DATA1_HIGH);
        LCD_DriverWriteDataFromExtFlash(setuplocktime_set1_data1_addr[light_level % 10], ICON_SET1_DATA1_SIZE);
    }
    if(index == 0)
    {
////        LCD_DispOffset(slid_param_s.slid_area_s.y_offset);
    }
}
/****************************************************************
** Function name:           Gui_PageLightTimeSet
** Descriptions:            ����ʱ�����ý���
** input parameters:        index����̬ͼ��ļ���
                            language: ���ԣ�0���ġ�1Ӣ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2018-05-24
*****************************************************************/ 
void Gui_PageLightTimeSet(uint16_t index, uint8_t light_time ,uint8_t language)
{
    static uint8_t light_time_bk = 0;
    uint8_t light_time_tmp = 0;
    float one_time_tmp = 80.0 / 30;
    uint8_t disp_time_num = 0;
    
    #if TEST_CLEAR_SCREEN_PARAM
    if(index == 0)
    {
        Gui_PageClearBackground(0,0,LCD_MAX_COLUMN,LCD_MAX_ROW,0x00F8);
    }
    #endif //TEST_CLEAR_SCREEN_PARAM
    
    if(language)
    {
        language = 1;
    }
    
    if(light_time >= 3)
    {
        light_time_tmp = light_time;
    }
    disp_time_num = one_time_tmp * light_time_tmp;
    if(index == 0)
    {
////                //������
////        Gui_PageClearBackground(0, 
////                                0, 
////                                MAX_X_SIZE,
////                                cleardata_cleardata_icon1_coord[0][1]);
////        //������
////        Gui_PageClearBackground(0, 
////                                0, 
////                                cleardata_cleardata_icon1_coord[0][0],
////                                MAX_Y_SIZE);
////        //������
////        Gui_PageClearBackground(0, 
////                                cleardata_cleardata_icon1_coord[0][1] + ICON_CLEARDATA_ICON1_HIGH, 
////                                MAX_X_SIZE,
////                                MAX_Y_SIZE - (cleardata_cleardata_icon1_coord[0][1] + ICON_CLEARDATA_ICON1_HIGH));
////        //������
////        Gui_PageClearBackground(cleardata_cleardata_icon1_coord[0][0] + ICON_CLEARDATA_ICON1_WIDE, 
////                                0, 
////                                MAX_X_SIZE - (cleardata_cleardata_icon1_coord[0][0] + ICON_CLEARDATA_ICON1_WIDE),
////                                MAX_Y_SIZE);
        Gui_PageClearBackground(0, 0, MAX_X_SIZE, MAX_Y_SIZE);
    }
    if(index == 0 )
    {
    
        //������
        LCD_SetPosition(setupbrightness_set2_bg1_coord[0][0], setupbrightness_set2_bg1_coord[0][1],
                         ICON_SET2_BG1_WIDE, ICON_SET2_BG1_HIGH);
        LCD_DriverWriteDataFromExtFlash(setupbrightness_set2_bg1_addr[0], ICON_SET2_BG1_SIZE);
        //���� 
        LCD_SetPosition(setuplocktime_set1_icon1_coord[0][0], setuplocktime_set1_icon1_coord[0][1],
                         ICON_SET1_ICON1_WIDE, ICON_SET1_ICON1_HIGH);
        LCD_DriverWriteDataFromExtFlash(setuplocktime_set1_icon1_addr[0], ICON_SET1_ICON1_SIZE);
    }
    
    
    if(index == 0 || language != language_save)
    {
        LCD_SetPosition(setuplocktime_set1_icon2_coord[0][0], setuplocktime_set1_icon2_coord[0][1],
                         ICON_SET1_ICON2_WIDE, ICON_SET1_ICON2_HIGH);
        LCD_DriverWriteDataFromExtFlash(setuplocktime_set1_icon2_addr[language%2], ICON_SET1_ICON2_SIZE);
    }
    
    if(index == 0)
    {
        LCD_SetPosition(setupbrightness_set2_icon_coord[0][0], setupbrightness_set2_icon_coord[0][1],
                        ICON_SET2_ICON4_WIDE, ICON_SET2_ICON4_HIGH);
        LCD_DriverWriteDataFromExtFlash(setupbrightness_set2_icon_addr[1][0], ICON_SET2_ICON4_SIZE); 
    }
    
    if(light_time_bk != light_time || index == 0)
    {
        light_time_bk = light_time ;
        for(uint8_t i = 0; i < disp_time_num; i++)
        {
            LCD_SetPosition(setupbrightness_set2_icon_coord[0][0] + 2 + 2*i, setupbrightness_set2_icon_coord[0][1],
                            ICON_SET2_ICON4_WIDE, ICON_SET2_ICON4_HIGH);
            LCD_DriverWriteDataFromExtFlash(setupbrightness_set2_icon_addr[1][1], ICON_SET2_ICON4_SIZE);         
        }
        

        for(uint8_t i = disp_time_num; i < 80; i++)
        {
            LCD_SetPosition(setupbrightness_set2_icon_coord[0][0] + 2 + 2*i, setupbrightness_set2_icon_coord[0][1],
                            ICON_SET2_ICON4_WIDE, ICON_SET2_ICON4_HIGH);
            LCD_DriverWriteDataFromExtFlash(setupbrightness_set2_icon_addr[0][1], ICON_SET2_ICON4_SIZE);         
        }
        
        //����
        LCD_SetPosition(103, setuplocktime_set1_data1_coord[0][1],
                            ICON_SET1_DATA1_WIDE, ICON_SET1_DATA1_HIGH);
        LCD_DriverWriteDataFromExtFlash(setuplocktime_set1_data1_addr[light_time /10 % 10], ICON_SET1_DATA1_SIZE); 
        
        LCD_SetPosition(103 + ICON_SET1_DATA1_WIDE, setuplocktime_set1_data1_coord[1][1],
                            ICON_SET1_DATA1_WIDE, ICON_SET1_DATA1_HIGH);
        LCD_DriverWriteDataFromExtFlash(setuplocktime_set1_data1_addr[light_time % 10], ICON_SET1_DATA1_SIZE); 
        
        LCD_SetPosition(setuplocktime_set1_data1dot_coord[0][0], setuplocktime_set1_data1dot_coord[0][1],
                            ICON_SET1_DATA1DOT_WIDE, ICON_SET1_DATA1DOT_HIGH);
        LCD_DriverWriteDataFromExtFlash(setuplocktime_set1_data1dot_addr[0], ICON_SET1_DATA1DOT_SIZE); 
        
        
        if(disp_time_num >= 80)
        {
            LCD_SetPosition(setupbrightness_set2_icon_coord[1][0], setupbrightness_set2_icon_coord[1][1],
                            ICON_SET2_ICON4_WIDE, ICON_SET2_ICON4_HIGH);
            LCD_DriverWriteDataFromExtFlash(setupbrightness_set2_icon_addr[1][2], ICON_SET2_ICON4_SIZE); 
        }
        else
        {
            LCD_SetPosition(setupbrightness_set2_icon_coord[1][0], setupbrightness_set2_icon_coord[1][1],
                            ICON_SET2_ICON4_WIDE, ICON_SET2_ICON4_HIGH);
            LCD_DriverWriteDataFromExtFlash(setupbrightness_set2_icon_addr[0][2], ICON_SET2_ICON4_SIZE);
        }
        
        for(uint8_t i = 0; i < 80; i++)
        {
            LCD_SetPosition(setupbrightness_set2_icon5_coord[0][0] + 2*i, setupbrightness_set2_icon5_coord[0][1],
                                 ICON_SET2_ICON5_WIDE, ICON_SET2_ICON5_HIGH);
            LCD_DriverWriteDataFromExtFlash(setupbrightness_set2_icon5_addr[1], ICON_SET2_ICON5_SIZE); 
        }
        
        LCD_SetPosition(setupbrightness_set2_icon5_coord[0][0] + 2 + disp_time_num*2-10, setupbrightness_set2_icon5_coord[0][1],
                             ICON_SET2_ICON5_WIDE, ICON_SET2_ICON5_HIGH);
        LCD_DriverWriteDataFromExtFlash(setupbrightness_set2_icon5_addr[0], ICON_SET2_ICON5_SIZE);
        
        LCD_SetPosition(setupbrightness_set2_icon6_coord[0][0] + 2 + disp_time_num*2-10, setupbrightness_set2_icon6_coord[0][1],
                             ICON_SET2_ICON6_WIDE, ICON_SET2_ICON6_HIGH);
        LCD_DriverWriteDataFromExtFlash(setupbrightness_set2_icon6_addr[0], ICON_SET2_ICON6_SIZE);
    }
    if(index == 0)
    {
//////        LCD_DispOffset(slid_param_s.slid_area_s.y_offset);
    }
} 
/****************************************************************
** Function name:           Gui_PageClearData
** Descriptions:            ������ݽ���
** input parameters:        index����Ϣ����ָ��
                            language�����ݳ���
** output parameters:       ��
** Returned value:          ��
** Created by:              JSH              
** Created Date:            2018-09-30
**----------------------------------------------------------------
*****************************************************************/
void Gui_PageClearData(uint16_t index, uint8_t flg, uint8_t language)
{
    #if TEST_CLEAR_SCREEN_PARAM
    if(index == 0)
    {
        Gui_PageClearBackground(0,0,LCD_MAX_COLUMN,LCD_MAX_ROW,0x00F8);
    }
    #endif //TEST_CLEAR_SCREEN_PARAM
    
    if(index == 0)
    {
        //������
        Gui_PageClearBackground(0, 
                                0, 
                                MAX_X_SIZE,
                                cleardata_cleardata_icon1_coord[0][1]);
        //������
        Gui_PageClearBackground(0, 
                                0, 
                                cleardata_cleardata_icon1_coord[0][0],
                                MAX_Y_SIZE);
        //������
        Gui_PageClearBackground(0, 
                                cleardata_cleardata_icon1_coord[0][1] + ICON_CLEARDATA_ICON1_HIGH, 
                                MAX_X_SIZE,
                                MAX_Y_SIZE - (cleardata_cleardata_icon1_coord[0][1] + ICON_CLEARDATA_ICON1_HIGH));
        //������
        Gui_PageClearBackground(cleardata_cleardata_icon1_coord[0][0] + ICON_CLEARDATA_ICON1_WIDE, 
                                0, 
                                MAX_X_SIZE - (cleardata_cleardata_icon1_coord[0][0] + ICON_CLEARDATA_ICON1_WIDE),
                                MAX_Y_SIZE);
    } 
    
    LCD_SetPosition(cleardata_cleardata_icon1_coord[0][0], cleardata_cleardata_icon1_coord[0][1], 
                    ICON_CLEARDATA_ICON1_WIDE, ICON_CLEARDATA_ICON1_HIGH);
    LCD_DriverWriteDataFromExtFlash(cleardata_cleardata_icon1_addr[language%2], ICON_CLEARDATA_ICON1_SIZE);
    
    LCD_SetPosition(cleardata_cleardata_icon2_coord[0][0], cleardata_cleardata_icon2_coord[0][1], 
                    ICON_CLEARDATA_ICON2_WIDE, ICON_CLEARDATA_ICON2_HIGH);
    LCD_DriverWriteDataFromExtFlash(cleardata_cleardata_icon2_addr[0][flg%2][language%2], ICON_CLEARDATA_ICON2_SIZE);
    
    LCD_SetPosition(cleardata_cleardata_icon2_coord[1][0], cleardata_cleardata_icon2_coord[1][1], 
                    ICON_CLEARDATA_ICON2_WIDE, ICON_CLEARDATA_ICON2_HIGH);
    LCD_DriverWriteDataFromExtFlash(cleardata_cleardata_icon2_addr[1][flg%2][language%2], ICON_CLEARDATA_ICON2_SIZE);
    if(index == 0)
    {
//////        LCD_DispOffset(slid_param_s.slid_area_s.y_offset);
    }
} 
/****************************************************************
** Function name:           Gui_PageMessage3
** Descriptions:            ��Ϣ����
** input parameters:        index����Ϣ����ָ��
                            language�����ݳ���
** output parameters:       ��
** Returned value:          ��
** Created by:              JSH              
** Created Date:            2018-09-30
**----------------------------------------------------------------
*****************************************************************/
void Gui_PageMessage3(uint16_t index, uint8_t language)
{
    #if TEST_CLEAR_SCREEN_PARAM
    if(index == 0)
    {
        Gui_PageClearBackground(0,0,LCD_MAX_COLUMN,LCD_MAX_ROW,0x00F8);
    }
    #endif //TEST_CLEAR_SCREEN_PARAM
    
    if(index == 0)
    {
        //������
        Gui_PageClearBackground(0, 
                                0, 
                                MAX_X_SIZE,
                                messagequery_message4_icon1_coord[0][1]);
        //������
        Gui_PageClearBackground(0, 
                                0, 
                                messagequery_message4_icon1_coord[0][0],
                                MAX_Y_SIZE);
        //������
        Gui_PageClearBackground(0, 
                                messagequery_message4_icon1_coord[0][1] + ICON_MESSAGE4_ICON1_HIGH, 
                                MAX_X_SIZE,
                                MAX_Y_SIZE - (messagequery_message4_icon1_coord[0][1] + ICON_MESSAGE4_ICON1_HIGH));
        //������
        Gui_PageClearBackground(messagequery_message4_icon1_coord[0][0] + ICON_MESSAGE4_ICON1_WIDE, 
                                0, 
                                MAX_X_SIZE - (messagequery_message4_icon1_coord[0][0] + ICON_MESSAGE4_ICON1_WIDE),
                                MAX_Y_SIZE);
    } 
    
    LCD_SetPosition(messagequery_message4_icon1_coord[0][0], messagequery_message4_icon1_coord[0][1], 
                    ICON_MESSAGE4_ICON1_WIDE, ICON_MESSAGE4_ICON1_HIGH);
    LCD_DriverWriteDataFromExtFlash(messagequery_message4_icon1_addr[0], ICON_MESSAGE4_ICON1_SIZE);
    
    LCD_SetPosition(messagequery_message4_icon2_coord[0][0], messagequery_message4_icon2_coord[0][1], 
                    ICON_MESSAGE4_ICON2_WIDE, ICON_MESSAGE4_ICON2_HIGH);
    LCD_DriverWriteDataFromExtFlash(messagequery_message4_icon2_addr[language%2], ICON_MESSAGE4_ICON2_SIZE);
    

    if(index == 0)
    {
//////        LCD_DispOffset(slid_param_s.slid_area_s.y_offset);
    }
} 
/****************************************************************
** Function name:           Gui_PageDispMessage
** Descriptions:            ��ʾ��Ϣ����
** input parameters:        index����Ϣ����ָ��
                            language�����ݳ���
** output parameters:       ��
** Returned value:          ��
** Created by:              JSH              
** Created Date:            2018-09-30
**----------------------------------------------------------------
*****************************************************************/
void Gui_PageDispMessage(uint16_t index, page_message_param_t message_para, uint8_t language)
{
    uint8_t disp_choose[5] = {0};
    uint8_t choose_tmp = message_para.message_touch_num;
    uint8_t message_type = 0;
    uint16_t disp_color[2] ={0x718c, 0xffff};
    uint8_t disp_time[20] = {0};
    
    #if TEST_CLEAR_SCREEN_PARAM
    if(index == 0)
    {
        Gui_PageClearBackground(0,0,LCD_MAX_COLUMN,LCD_MAX_ROW,0x00F8);
    }
    #endif //TEST_CLEAR_SCREEN_PARAM
    
    if(index == 0)
    {
        Gui_PageClearBackground(0, 0, MAX_X_SIZE, MAX_Y_SIZE);
    }
    
    if(message_para.disp_data_num == 0)
    {
        if(index == 0)
        {   
            LCD_SetPosition(messageno_message4_icon3_coord[0][0], messageno_message4_icon3_coord[0][1], 
                                 ICON_MESSAGE4_ICON3_WIDE, ICON_MESSAGE4_ICON3_HIGH);
            LCD_DriverWriteDataFromExtFlash(messageno_message4_icon3_addr[0], ICON_MESSAGE4_ICON3_SIZE);
        }
        
        LCD_SetPosition(messageno_message4_icon4_coord[0][0], messageno_message4_icon4_coord[0][1], 
                             ICON_MESSAGE4_ICON4_WIDE, ICON_MESSAGE4_ICON4_HIGH);
        LCD_DriverWriteDataFromExtFlash(messageno_message4_icon4_addr[language%2], ICON_MESSAGE4_ICON4_SIZE);
    }
    else
    {
//        for(uint8_t i = 0; i < 5; i++)
//        {
//            if(i == choose_tmp)
//            {
//                disp_choose[i] = 1;
//            }
//        }
//        
//        if(index == 0)
//        {
//            for(uint8_t i = 0; i < 2; i++)
//            {
//                if(i >= message_para.disp_data_num)
//                {
//                    break;
//                }
//                LCD_SetPosition(icon_16_message_bg1_coord[i][0], icon_16_message_bg1_coord[i][1], 
//                                     ICON_MESSAGE_ICON7_WIDE, ICON_MESSAGE_ICON7_HIGH);
//                LCD_DriverWriteDataFromExtFlash(icon_16_message_bg1_addr[0], ICON_MESSAGE_ICON7_SIZE);
//            }
//        } 
//        
//        LCD_SetPosition(icon_16_message_icon2_coord[0][0], icon_16_message_icon2_coord[0][1], 
//                             ICON_MESSAGE_ICON5_WIDE, ICON_MESSAGE_ICON5_HIGH);
//        LCD_DriverWriteDataFromExtFlash(icon_16_message_icon2_addr[1][disp_choose[0]%2][language%2], ICON_MESSAGE_ICON5_SIZE);
//        
//        LCD_SetPosition(icon_16_message_icon2_coord[1][0], icon_16_message_icon2_coord[1][1], 
//                             ICON_MESSAGE_ICON4_WIDE, ICON_MESSAGE_ICON4_HIGH);
//        LCD_DriverWriteDataFromExtFlash(icon_16_message_icon2_addr[0][disp_choose[1]%2][language%2], ICON_MESSAGE_ICON4_SIZE);
//        
//        for(uint8_t i = 0; i < message_para.disp_data_num; i++)
//        {
//            message_type = message_para.message_type[i];
//            if(message_type >= 2)
//            {
//                message_type -= 2;
//            }
//            LCD_SetPosition(icon_16_message_icon1_coord[i][0], icon_16_message_icon1_coord[i][1], 
//                                 ICON_MESSAGE_ICON3_WIDE, ICON_MESSAGE_ICON3_HIGH);
//            LCD_DriverWriteDataFromExtFlash(icon_16_message_icon1_addr[disp_choose[i+2]%2][message_type%13], ICON_MESSAGE_ICON3_SIZE);

//            //��Ϣ��������
//            Font_DisUiTextOneLine(message_para.p_message_content[i], message_para.content_len[i], icon_16_message_data_coord[i][0], 
//                                  icon_16_message_data_coord[i][1], 0, 0, 0, disp_color[disp_choose[i+2]%2], 0);
//            disp_time[0] = message_para.message_time_s[i].hour/10%10+'0';
//            disp_time[1] = message_para.message_time_s[i].hour%10+'0';
//            disp_time[2] = ':';
//            disp_time[3] = message_para.message_time_s[i].minutes/10%10+'0';
//            disp_time[4] = message_para.message_time_s[i].minutes%10+'0';
//            disp_time[5] = ' ';
//            disp_time[6] = ' ';
//            disp_time[7] = message_para.message_time_s[i].year/1000%10+'0';
//            disp_time[8] = message_para.message_time_s[i].year/100%10+'0';
//            disp_time[9] = message_para.message_time_s[i].year/10%10+'0';
//            disp_time[10] = message_para.message_time_s[i].year%10+'0';
//            disp_time[11] = '/';
//            disp_time[12] = message_para.message_time_s[i].month/10%10+'0';
//            disp_time[13] = message_para.message_time_s[i].month%10+'0';
//            disp_time[14] = '/';
//            disp_time[15] = message_para.message_time_s[i].day/10%10+'0';
//            disp_time[16] = message_para.message_time_s[i].day%10+'0';
//            //��ʾʱ��
//            Font_DisUiTextOneLine(disp_time, 17, icon_16_message_data_coord[i][0], 
//                                  icon_16_message_data_coord[i][1]+27, 0, 0, 0, disp_color[disp_choose[i+2]%2], 0);
//        }
    }
    
    if(index == 0)
    {
////////        LCD_DispOffset(slid_param_s.slid_area_s.y_offset);
    }
}