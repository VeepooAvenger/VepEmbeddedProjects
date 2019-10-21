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
#include "common_data_type.h"

#include "bsp_btn_ble.h"

extern uint8_t ui_style_type;  //GUI_MAIN_WHITE;//GUI_MAIN_RED;//GUI_MAIN_YELLOW;//GUI_MAIN_GREEN;

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
    if(column_size * page_size == 0)
    {
        return ;
    }
    /*����ˢ��Ϊ������ɫ*/
    LCD_SetColorScreenByPosition(s_column, s_page, column_size, page_size, color); 
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
** Function name:           Gui_PageMainBackGround
** Descriptions:            ��ʾ������ı�������
** input parameters:        color_type��������ɫ
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ             
** Created Date:            2017-08-09
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Gui_PageMainBackGround(void)//uint8_t color_type)
{   

}

/****************************************************************
** Function name:           Gui_PageMainDateHour
** Descriptions:            ��ʾ�������ʱ�Ӻ�����
** input parameters:        month����
**                          day����
**                          hour��ʱ
**                          minute������
**                          week������
**                          language������
** output parameters:       
** Returned value:          
** Created by:              YX              
** Created Date:            2017-08-07
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/     
void Gui_PageMainDateHour(uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t week, 
                          uint8_t language, uint16_t am_pm_format, uint8_t phys_menstrual, uint16_t step_prg)
{   
    uint8_t hour_index = 0;
    uint8_t bg_index = 1;
    uint8_t prg_index = 0;
    uint8_t prg_cnt = 0;
    uint32_t prg_addr = 0;
    
    uint8_t start_column = 0;
    uint8_t start_page = 0;
    
    if(ui_style_type <= 2 && ui_style_type >= 1)
    {   //���һ
        bg_index = (ui_style_type - 1) % 2;
        /* ����*/
        LCD_SetPosition(icon_main1_bar_coord[0][0], icon_main1_bar_coord[0][1], 
                        ICON_MAIN1GREEN_BAR_WIDE, ICON_MAIN1GREEN_BAR_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_main1_bar_addr[bg_index][0], ICON_MAIN1GREEN_BAR_SIZE);

        /* ����AM��PM��ʽ */
        /* ����ʱ��--ʱ */
        LCD_SetPosition(icon_main1_hour_coord[0][0], icon_main1_hour_coord[0][1],
                        ICON_MAIN1GREEN_TIME_WIDE, ICON_MAIN1GREEN_TIME_HIGH);
        if(am_pm_format == GUI_TIME_FORMAT_AM)
        {
            hour_index = hour / 10 % 10 % 2;			

            if(language == 0)
            {
                 /* ����ʱ��--ʱ*/
                LCD_DriverWriteDataFromExtFlash(icon_main1_hour_addr[bg_index][10 + hour_index], ICON_MAIN1GREEN_TIME_SIZE);
            }
            else
            {
                LCD_DriverWriteDataFromExtFlash(icon_main1_hour_addr[bg_index][10 + 4 + hour_index], ICON_MAIN1GREEN_TIME_SIZE);
            }
        }
        else if(am_pm_format == GUI_TIME_FORMAT_PM)
        {
            hour_index = hour / 10 % 10 % 2;

            if(language == 0)
            {
                LCD_DriverWriteDataFromExtFlash(icon_main1_hour_addr[bg_index][10 + hour_index + 2], ICON_MAIN1GREEN_TIME_SIZE);
            }
            else
            {
                LCD_DriverWriteDataFromExtFlash(icon_main1_hour_addr[bg_index][10 + 4 + hour_index + 2], ICON_MAIN1GREEN_TIME_SIZE);
            }
        }
        else
        {          
            LCD_DriverWriteDataFromExtFlash(icon_main1_hour_addr[bg_index][hour / 10 % 10 % 3], ICON_MAIN1GREEN_TIME_SIZE);
        }
        LCD_SetPosition(icon_main1_hour_coord[1][0], icon_main1_hour_coord[1][1],
                        ICON_MAIN1GREEN_TIME_WIDE, ICON_MAIN1GREEN_TIME_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_main1_hour_addr[bg_index][hour % 10], ICON_MAIN1GREEN_TIME_SIZE);
        
        /* ����ʱ��--�� */
        LCD_SetPosition(icon_main1_hour_coord[2][0], icon_main1_hour_coord[2][1],
                        ICON_MAIN1GREEN_TIME_WIDE, ICON_MAIN1GREEN_TIME_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_main1_hour_addr[bg_index][minute / 10 % 6], ICON_MAIN1GREEN_TIME_SIZE);
        LCD_SetPosition(icon_main1_hour_coord[3][0], icon_main1_hour_coord[3][1],
                        ICON_MAIN1GREEN_TIME_WIDE, ICON_MAIN1GREEN_TIME_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_main1_hour_addr[bg_index][minute % 10], ICON_MAIN1GREEN_TIME_SIZE);  

        /* ������ */
        LCD_SetPosition(icon_main1_date_coord[0][0], icon_main1_date_coord[0][1], 
                        ICON_MAIN1GREEN_DATE_WIDE, ICON_MAIN1GREEN_DATE_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_main1_date_addr[bg_index][day / 10 % 4], ICON_MAIN1GREEN_DATE_SIZE);
        LCD_SetPosition(icon_main1_date_coord[1][0], icon_main1_date_coord[1][1], 
                        ICON_MAIN1GREEN_DATE_WIDE, ICON_MAIN1GREEN_DATE_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_main1_date_addr[bg_index][day % 10], ICON_MAIN1GREEN_DATE_SIZE);  
        
        /* �������� */
        LCD_SetPosition(icon_main1_week_coord[0][0], icon_main1_week_coord[0][1],
                        ICON_MAIN1GREEN_WEEK_WIDE, ICON_MAIN1GREEN_WEEK_HIGH);
        if(language == 0)
        {
            LCD_DriverWriteDataFromExtFlash(icon_main1_week_addr[bg_index][week - 1], ICON_MAIN1GREEN_WEEK_SIZE);
        }
        else
        {
            LCD_DriverWriteDataFromExtFlash(icon_main1_week_addr[bg_index][week - 1 + 7], ICON_MAIN1GREEN_WEEK_SIZE);		
        }
   
        return;
    }
    else if(ui_style_type >= 3 && ui_style_type <= 5)
    {   //����   //��ɫ����ɫ����ɫ
        bg_index = (ui_style_type - 3) % 3;
        
        /* ����AM��PM��ʽ */
        //���AM\PM���⴦����
        Gui_PageClearBackground(icon_main2_ampm_coord[0][0], icon_main2_ampm_coord[0][1],
                                ICON_MAIN2GREEN_AMPM_WIDE, ICON_MAIN2GREEN_AMPM_HIGH);
        /* ����ʱ��--ʱ */
        LCD_SetPosition(icon_main2_hour_coord[0][0], icon_main2_hour_coord[0][1],
                        ICON_MAIN2GREEN_TIME_WIDE, ICON_MAIN2GREEN_TIME_HIGH);
        if(am_pm_format == GUI_TIME_FORMAT_AM)
        {
            hour_index = hour / 10 % 10 % 2;			
		
            if(language == 0)
            {
                 /* ����ʱ��--ʱ*/
                LCD_DriverWriteDataFromExtFlash(icon_main2_hour_addr[bg_index][10 + hour_index], ICON_MAIN2GREEN_TIME_SIZE);
            }
            else
            {
                LCD_DriverWriteDataFromExtFlash(icon_main2_hour_addr[bg_index][10 + 4 + hour_index], ICON_MAIN2GREEN_TIME_SIZE);
                if(hour_index == 1)
                {
                    LCD_SetPosition(icon_main2_ampm_coord[0][0], icon_main2_ampm_coord[0][1],
                                    ICON_MAIN2GREEN_AMPM_WIDE, ICON_MAIN2GREEN_AMPM_HIGH);
                    LCD_DriverWriteDataFromExtFlash(icon_main2_ampm_addr[bg_index][0], ICON_MAIN2GREEN_AMPM_SIZE);
                }
            }
        }
        else if(am_pm_format == GUI_TIME_FORMAT_PM)
        {
            hour_index = hour / 10 % 10 % 2;     			     
	
            if(language == 0)
            {
                LCD_DriverWriteDataFromExtFlash(icon_main2_hour_addr[bg_index][10 + hour_index +2], ICON_MAIN2GREEN_TIME_SIZE);
            }
            else
            {
                LCD_DriverWriteDataFromExtFlash(icon_main2_hour_addr[bg_index][10 + 4 + hour_index + 2], ICON_MAIN2GREEN_TIME_SIZE);
                if(hour_index == 1)
                {
                    LCD_SetPosition(icon_main2_ampm_coord[0][0], icon_main2_ampm_coord[0][1],
                                    ICON_MAIN2GREEN_AMPM_WIDE, ICON_MAIN2GREEN_AMPM_HIGH);
                    LCD_DriverWriteDataFromExtFlash(icon_main2_ampm_addr[bg_index][1], ICON_MAIN2GREEN_AMPM_SIZE);
                }
            }
        }
        else
        {
            LCD_DriverWriteDataFromExtFlash(icon_main2_hour_addr[bg_index][hour / 10 % 10 % 3], ICON_MAIN2GREEN_TIME_SIZE);
        }
        LCD_SetPosition(icon_main2_hour_coord[1][0], icon_main2_hour_coord[1][1],
                        ICON_MAIN2GREEN_TIME_WIDE, ICON_MAIN2GREEN_TIME_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_main2_hour_addr[bg_index][hour % 10], ICON_MAIN2GREEN_TIME_SIZE);
        
        /* ����ʱ��--�� */
        LCD_SetPosition(icon_main2_hour_coord[2][0], icon_main2_hour_coord[2][1],
                        ICON_MAIN2GREEN_TIME_WIDE, ICON_MAIN2GREEN_TIME_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_main2_hour_addr[bg_index == 0 ? 1:2][minute / 10 % 6], ICON_MAIN2GREEN_TIME_SIZE);
        LCD_SetPosition(icon_main2_hour_coord[3][0], icon_main2_hour_coord[3][1],
                        ICON_MAIN2GREEN_TIME_WIDE, ICON_MAIN2GREEN_TIME_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_main2_hour_addr[bg_index == 0 ? 1:2][minute % 10], ICON_MAIN2GREEN_TIME_SIZE);  
   
        /* ������ */
        LCD_SetPosition(icon_main2_date_coord[0][0], icon_main2_date_coord[0][1], 
                        ICON_MAIN2G_NUMBER_WIDE, ICON_MAIN2G_NUMBER_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_main2_date_addr[bg_index][month / 10 % 2], ICON_MAIN2G_NUMBER_SIZE);
        LCD_SetPosition(icon_main2_date_coord[1][0], icon_main2_date_coord[1][1], 
                        ICON_MAIN2G_NUMBER_WIDE, ICON_MAIN2G_NUMBER_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_main2_date_addr[bg_index][month % 10], ICON_MAIN2G_NUMBER_SIZE); 
        
        /* б�� */
        LCD_SetPosition(icon_main2_date_line_coord[0][0], icon_main2_date_line_coord[0][1], 
                        ICON_MAIN2G_DATE_WIDE, ICON_MAIN2G_DATE_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_main2_date_line_addr[bg_index][0], ICON_MAIN2G_DATE_SIZE);
        
        /* ������ */
        LCD_SetPosition(icon_main2_date_coord[2][0], icon_main2_date_coord[2][1], 
                        ICON_MAIN2G_NUMBER_WIDE, ICON_MAIN2G_NUMBER_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_main2_date_addr[bg_index][day / 10 % 4], ICON_MAIN2G_NUMBER_SIZE);
        LCD_SetPosition(icon_main2_date_coord[3][0], icon_main2_date_coord[3][1], 
                        ICON_MAIN2G_NUMBER_WIDE, ICON_MAIN2G_NUMBER_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_main2_date_addr[bg_index][day % 10], ICON_MAIN2G_NUMBER_SIZE); 
   
        /* �������� */
        LCD_SetPosition(icon_main2_week_coord[0][0], icon_main2_week_coord[0][1],
                        ICON_MAIN2Y_WEEK_WIDE, ICON_MAIN2Y_WEEK_HIGH);
        if(language == 0)
        {
            LCD_DriverWriteDataFromExtFlash(icon_main2_week_addr[bg_index == 2 ? 0:1][week - 1], ICON_MAIN2Y_WEEK_SIZE); 
        }
        else        
        {
            LCD_DriverWriteDataFromExtFlash(icon_main2_week_addr[bg_index == 2 ? 0:1][week - 1 + 7], ICON_MAIN2Y_WEEK_SIZE); 
        }
        
        //�˶������� step_prg
        //Բ����ɫ
        if(step_prg >= 100)
        {
            prg_index = 1;
            prg_addr = icon_main2_progress_ring0_addr[bg_index == 2 ? 0:1][1];
        }
        else
        {
            prg_index = 0;
            prg_addr = icon_main2_progress_ring0_addr[bg_index == 2 ? 0:1][0]; 
        }
        LCD_SetPosition(icon_main2_progress_ring_coord[0][0], icon_main2_progress_ring_coord[0][1],
                        ICON_MAIN2G_PROGRESS0_WIDE, ICON_MAIN2G_PROGRESS0_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_main2_progress_ring_addr[bg_index == 2 ? 0:1][prg_index], ICON_MAIN2G_PROGRESS0_SIZE); 
     
        prg_cnt = step_prg % 100 + (step_prg % 101) / 100;        
        if(prg_cnt != 100)
        {
            for(uint8_t i = 0; i < prg_cnt / 5; i++)
            {
                LCD_SetPosition(icon_main2_progress_ring0_coord[i][0], icon_main2_progress_ring0_coord[i][1],
                                ICON_MAIN2G_PROGRESS_WIDE, ICON_MAIN2G_PROGRESS_HIGH);
                LCD_DriverWriteDataFromExtFlash(prg_addr + i * ICON_MAIN2G_PROGRESS_SIZE, ICON_MAIN2G_PROGRESS_SIZE);
            }
        }
        
        //�ٷֱ�����
        uint16_t bg_color_value = 0;
        uint32_t bg_addr = 0;

        bg_addr = icon_main2_progress_number_addr[bg_index == 2 ? 0:1][0];
        LCD_DriverReadFromExtFlash(bg_addr, (uint8_t *)&bg_color_value, 2); 

        prg_cnt = 1; //�����жϰٷֺŵ�λ��
        start_page = icon_main2_progress_number_coord[0][1];
        if(step_prg >= 100)
        {
            start_column = icon_main2_progress_number_coord[2][0];
            LCD_SetPosition(start_column, start_page,
                            ICON_MAIN2G_PERCENTAGE_WIDE, ICON_MAIN2G_PERCENTAGE_HIGH);
            LCD_DriverWriteDataFromExtFlash(icon_main2_progress_number_addr[bg_index == 2 ? 0:1][step_prg / 100 % 10],
                                            ICON_MAIN2G_PERCENTAGE_SIZE); 
            start_column += ICON_MAIN2G_PERCENTAGE_WIDE + 1;
            prg_cnt = 2;
        }
        else
        {
            start_column = icon_main2_progress_number_coord[1][0];   
            Gui_PageRefreshBackground(icon_main2_progress_number_coord[2][0], start_page,
                                     start_column - icon_main2_progress_number_coord[2][0], ICON_MAIN2G_PERCENTAGE_HIGH, 
                                     bg_color_value);
        }
        if(step_prg >= 10)
        {
            LCD_SetPosition(start_column, start_page,
                            ICON_MAIN2G_PERCENTAGE_WIDE, ICON_MAIN2G_PERCENTAGE_HIGH);
            LCD_DriverWriteDataFromExtFlash(icon_main2_progress_number_addr[bg_index == 2 ? 0:1][step_prg / 10 % 10],
                                            ICON_MAIN2G_PERCENTAGE_SIZE); 
            start_column += ICON_MAIN2G_PERCENTAGE_WIDE + 1;
        }
        else
        {
            start_column = icon_main2_progress_number_coord[0][0]; 
            Gui_PageRefreshBackground(icon_main2_progress_number_coord[2][0], start_page,
                                     start_column - icon_main2_progress_number_coord[2][0], ICON_MAIN2G_PERCENTAGE_HIGH, 
                                     bg_color_value);
            prg_cnt = 0;
        }
        
        LCD_SetPosition(start_column, start_page,
                        ICON_MAIN2G_PERCENTAGE_WIDE, ICON_MAIN2G_PERCENTAGE_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_main2_progress_number_addr[bg_index == 2 ? 0:1][step_prg % 10],
                                        ICON_MAIN2G_PERCENTAGE_SIZE); 
        start_column += ICON_MAIN2G_PERCENTAGE_WIDE;
        
        Gui_PageRefreshBackground(start_column, start_page,
                                 icon_main2_percent_sign_coord[prg_cnt][0] - start_column, ICON_MAIN2G_PERCENTAGE_HIGH,
                                 bg_color_value);
        
        // "%"
        LCD_SetPosition(icon_main2_percent_sign_coord[prg_cnt][0], icon_main2_percent_sign_coord[prg_cnt][1],
                        ICON_MAIN2G_PERCENTAGE0_WIDE, ICON_MAIN2G_PERCENTAGE0_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_main2_percent_sign_addr[bg_index == 2 ? 0:1][0],
                                        ICON_MAIN2G_PERCENTAGE0_SIZE);  
        return;
    }
    else if(ui_style_type == 0 || ui_style_type >= 7)//< 6)
    {   //�����
        /* ����ʱ��--ʱ */
        LCD_SetPosition(icon_main3_hour_coord[0][0], icon_main3_hour_coord[0][1],
                        ICON_MAIN3_TIME_WIDE, ICON_MAIN3_TIME_HIGH);
   
        LCD_DriverWriteDataFromExtFlash(icon_main3_hour_addr[0][hour / 10 % 10 % 3], ICON_MAIN3_TIME_SIZE);
        
        LCD_SetPosition(icon_main3_hour_coord[1][0], icon_main3_hour_coord[1][1],
                        ICON_MAIN3_TIME_WIDE, ICON_MAIN3_TIME_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_main3_hour_addr[0][hour % 10], ICON_MAIN3_TIME_SIZE);
        /* ����ʱ��--: */
        LCD_SetPosition(icon_main3_colon_coord[0][0], icon_main3_colon_coord[0][1],
                        ICON_MAIN3_TIME1_WIDE, ICON_MAIN3_TIME1_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_main3_colon_addr[0][0], ICON_MAIN3_TIME1_SIZE);
        /* ����ʱ��--�� */
        LCD_SetPosition(icon_main3_hour_coord[2][0], icon_main3_hour_coord[2][1],
                        ICON_MAIN3_TIME_WIDE, ICON_MAIN3_TIME_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_main3_hour_addr[0][minute / 10 % 6], ICON_MAIN3_TIME_SIZE);
        LCD_SetPosition(icon_main3_hour_coord[3][0], icon_main3_hour_coord[3][1],
                        ICON_MAIN3_TIME_WIDE, ICON_MAIN3_TIME_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_main3_hour_addr[0][minute % 10], ICON_MAIN3_TIME_SIZE);  
   
        /* ������ */
        LCD_SetPosition(icon_main3_date_coord[0][0], icon_main3_date_coord[0][1], 
                        ICON_MAIN3_DATE_WIDE, ICON_MAIN3_DATE_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_main3_date_addr[0][month / 10 % 2], ICON_MAIN3_DATE_SIZE);
        LCD_SetPosition(icon_main3_date_coord[1][0], icon_main3_date_coord[1][1], 
                        ICON_MAIN3_DATE_WIDE, ICON_MAIN3_DATE_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_main3_date_addr[0][month % 10], ICON_MAIN3_DATE_SIZE); 
        
        /* б�� */
        LCD_SetPosition(icon_main3_date_line_coord[0][0], icon_main3_date_line_coord[0][1], 
                        ICON_MAIN3_DATE1_WIDE, ICON_MAIN3_DATE1_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_main3_date_line_addr[0][0], ICON_MAIN3_DATE1_SIZE);
        
        /* ������ */
        LCD_SetPosition(icon_main3_date_coord[2][0], icon_main3_date_coord[2][1], 
                        ICON_MAIN3_DATE_WIDE, ICON_MAIN3_DATE_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_main3_date_addr[0][day / 10 % 4], ICON_MAIN3_DATE_SIZE);
        LCD_SetPosition(icon_main3_date_coord[3][0], icon_main3_date_coord[3][1], 
                        ICON_MAIN3_DATE_WIDE, ICON_MAIN3_DATE_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_main3_date_addr[0][day % 10], ICON_MAIN3_DATE_SIZE); 
   
        /* �������� */
        LCD_SetPosition(icon_main3_week_coord[0][0], icon_main3_week_coord[0][1],
                        ICON_MAIN3_WEEK_WIDE, ICON_MAIN3_WEEK_HIGH);
        if(language == 0)
        {
            LCD_DriverWriteDataFromExtFlash(icon_main3_week_addr[0][week - 1], ICON_MAIN3_WEEK_SIZE); 
        }
        else        
        {
            LCD_DriverWriteDataFromExtFlash(icon_main3_week_addr[0][week - 1 + 7], ICON_MAIN3_WEEK_SIZE); 
        }
        
        //�˶������� step_prg
        //Բ����ɫ
        if(step_prg >= 100)
        {
            prg_index = 1;
            prg_addr = icon_main3_progress_ring0_addr[0][1];
        }
        else
        {
            prg_index = 0;
            prg_addr = icon_main3_progress_ring0_addr[0][0]; 
        }
        LCD_SetPosition(icon_main3_progress_ring_coord[0][0], icon_main3_progress_ring_coord[0][1],
                        ICON_MAIN3_PROGRESS0_WIDE, ICON_MAIN3_PROGRESS0_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_main3_progress_ring_addr[0][prg_index], ICON_MAIN3_PROGRESS0_SIZE); 
     
        prg_cnt = step_prg % 100 + (step_prg % 101) / 100;        
        if(prg_cnt != 100)
        {
            for(uint8_t i = 0; i < prg_cnt / 5; i++)
            {
                LCD_SetPosition(icon_main3_progress_ring0_coord[i][0], icon_main3_progress_ring0_coord[i][1],
                                ICON_MAIN3_PROGRESS_WIDE, ICON_MAIN3_PROGRESS_HIGH);
                LCD_DriverWriteDataFromExtFlash(prg_addr + i * ICON_MAIN3_PROGRESS_SIZE, ICON_MAIN3_PROGRESS_SIZE);
            }
        }
        
        //�ٷֱ�����
        uint16_t bg_color_value = 0;
        uint32_t bg_addr = 0;

        bg_addr = icon_main3_progress_number_addr[0][0];
        LCD_DriverReadFromExtFlash(bg_addr, (uint8_t *)&bg_color_value, 2); 

        prg_cnt = 1; //�����жϰٷֺŵ�λ��
        start_page = icon_main3_progress_number_coord[0][1];
        if(step_prg >= 100)
        {
            start_column = icon_main3_progress_number_coord[2][0];
            LCD_SetPosition(start_column, start_page,
                            ICON_MAIN3_PERCENTAGE_WIDE, ICON_MAIN3_PERCENTAGE_HIGH);
            LCD_DriverWriteDataFromExtFlash(icon_main3_progress_number_addr[0][step_prg / 100 % 10],
                                            ICON_MAIN3_PERCENTAGE_SIZE); 
            start_column += ICON_MAIN3_PERCENTAGE_WIDE + 1;
            prg_cnt = 2;
        }
        else
        {
            start_column = icon_main3_progress_number_coord[1][0];   
            Gui_PageRefreshBackground(icon_main3_progress_number_coord[2][0], start_page,
                                     start_column - icon_main3_progress_number_coord[2][0], ICON_MAIN3_PERCENTAGE_HIGH, 
                                     bg_color_value);
        }
        if(step_prg >= 10)
        {
            LCD_SetPosition(start_column, start_page,
                            ICON_MAIN3_PERCENTAGE_WIDE, ICON_MAIN3_PERCENTAGE_HIGH);
            LCD_DriverWriteDataFromExtFlash(icon_main3_progress_number_addr[0][step_prg / 10 % 10],
                                            ICON_MAIN3_PERCENTAGE_SIZE); 
            start_column += ICON_MAIN3_PERCENTAGE_WIDE + 1;
        }
        else
        {
            start_column = icon_main3_progress_number_coord[0][0]; 
            Gui_PageRefreshBackground(icon_main3_progress_number_coord[2][0], start_page,
                                     start_column - icon_main3_progress_number_coord[2][0], ICON_MAIN3_PERCENTAGE_HIGH, 
                                     bg_color_value);
            prg_cnt = 0;
        }
        
        LCD_SetPosition(start_column, start_page,
                        ICON_MAIN3_PERCENTAGE_WIDE, ICON_MAIN3_PERCENTAGE_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_main3_progress_number_addr[0][step_prg % 10],
                                        ICON_MAIN3_PERCENTAGE_SIZE); 
        start_column += ICON_MAIN3_PERCENTAGE_WIDE;
        
        Gui_PageRefreshBackground(start_column, start_page,
                                 icon_main3_percent_sign_coord[prg_cnt][0] - start_column, ICON_MAIN3_PERCENTAGE_HIGH,
                                 bg_color_value);
        
        // "%"
        LCD_SetPosition(icon_main3_percent_sign_coord[prg_cnt][0], icon_main3_percent_sign_coord[prg_cnt][1],
                        ICON_MAIN3_PERCENTAGE0_WIDE, ICON_MAIN3_PERCENTAGE0_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_main3_percent_sign_addr[0][0], ICON_MAIN3_PERCENTAGE0_SIZE);  
   
        return;
    }
    else if(ui_style_type == 6)
    {   //�����
        /* ����ʱ��--ʱ */
        LCD_SetPosition(icon_main4_hour_coord[0][0], icon_main4_hour_coord[0][1],
                        ICON_MAIN4_TIME_WIDE, ICON_MAIN4_TIME_HIGH);
   
        LCD_DriverWriteDataFromExtFlash(icon_main4_hour_addr[0][hour / 10 % 10 % 3], ICON_MAIN4_TIME_SIZE);
        
        LCD_SetPosition(icon_main4_hour_coord[1][0], icon_main4_hour_coord[1][1],
                        ICON_MAIN4_TIME_WIDE, ICON_MAIN4_TIME_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_main4_hour_addr[0][hour % 10], ICON_MAIN4_TIME_SIZE);

        /* ����ʱ��--�� */ //ʱ�Ӻͷ��ӵ����ִ�С��һ��
        LCD_SetPosition(icon_main4_hour_coord[2][0], icon_main4_hour_coord[2][1],
                        ICON_MAIN4_TIME1_WIDE, ICON_MAIN4_TIME1_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_main4_hour_addr[1][minute / 10 % 6], ICON_MAIN4_TIME1_SIZE);
        LCD_SetPosition(icon_main4_hour_coord[3][0], icon_main4_hour_coord[3][1],
                        ICON_MAIN4_TIME1_WIDE, ICON_MAIN4_TIME1_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_main4_hour_addr[1][minute % 10], ICON_MAIN4_TIME1_SIZE);  
  
        /* ������ */
        LCD_SetPosition(icon_main4_date_coord[0][0], icon_main4_date_coord[0][1], 
                        ICON_MAIN4_DATE_WIDE, ICON_MAIN4_DATE_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_main4_date_addr[0][day / 10 % 4], ICON_MAIN4_DATE_SIZE);
        LCD_SetPosition(icon_main4_date_coord[1][0], icon_main4_date_coord[1][1], 
                        ICON_MAIN4_DATE_WIDE, ICON_MAIN4_DATE_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_main4_date_addr[0][day % 10], ICON_MAIN4_DATE_SIZE); 
   
        /* �������� */
        LCD_SetPosition(icon_main4_week_coord[0][0], icon_main4_week_coord[0][1],
                        ICON_MAIN4_WEEK_WIDE, ICON_MAIN4_WEEK_HIGH);
        if(language == 0)
        {
            LCD_DriverWriteDataFromExtFlash(icon_main4_week_addr[0][week - 1], ICON_MAIN4_WEEK_SIZE); 
        }
        else        
        {
            LCD_DriverWriteDataFromExtFlash(icon_main4_week_addr[0][week - 1 + 7], ICON_MAIN4_WEEK_SIZE); 
        }
        
        //�˶������� step_prg
        //Բ����ɫ
        if(step_prg >= 100)
        {
            prg_index = 1;
            prg_addr = icon_main4_progress_ring0_addr[0][1];
        }
        else
        {
            prg_index = 0;
            prg_addr = icon_main4_progress_ring0_addr[0][0]; 
        }
        LCD_SetPosition(icon_main4_progress_ring_coord[0][0], icon_main4_progress_ring_coord[0][1],
                        ICON_MAIN4_PROGRESS0_WIDE, ICON_MAIN4_PROGRESS0_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_main4_progress_ring_addr[0][prg_index], ICON_MAIN4_PROGRESS0_SIZE); 
     
        prg_cnt = step_prg % 100 + (step_prg % 101) / 100;        
        if(prg_cnt != 100)
        {
            for(uint8_t i = 0; i < prg_cnt / 5; i++)
            {
                LCD_SetPosition(icon_main4_progress_ring0_coord[i][0], icon_main4_progress_ring0_coord[i][1],
                                ICON_MAIN4_PROGRESS_WIDE, ICON_MAIN4_PROGRESS_HIGH);
                LCD_DriverWriteDataFromExtFlash(prg_addr + i * ICON_MAIN4_PROGRESS_SIZE, ICON_MAIN4_PROGRESS_SIZE);
            }
        }
        
        //�ٷֱ�����
        uint16_t bg_color_value = 0;
        uint32_t bg_addr = 0;

        bg_addr = icon_main4_progress_number_addr[0][0];
        LCD_DriverReadFromExtFlash(bg_addr, (uint8_t *)&bg_color_value, 2); 

        prg_cnt = 1; //�����жϰٷֺŵ�λ��
        start_page = icon_main4_progress_number_coord[0][1];
        if(step_prg >= 100)
        {
            start_column = icon_main4_progress_number_coord[2][0];
            LCD_SetPosition(start_column, start_page,
                            ICON_MAIN4_PERCENTAGE_WIDE, ICON_MAIN4_PERCENTAGE_HIGH);
            LCD_DriverWriteDataFromExtFlash(icon_main4_progress_number_addr[0][step_prg / 100 % 10],
                                            ICON_MAIN4_PERCENTAGE_SIZE); 
            start_column += ICON_MAIN4_PERCENTAGE_WIDE + 1;
            prg_cnt = 2;
        }
        else
        {
            start_column = icon_main4_progress_number_coord[1][0];   
            Gui_PageRefreshBackground(icon_main4_progress_number_coord[2][0], start_page,
                                     start_column - icon_main4_progress_number_coord[2][0], ICON_MAIN4_PERCENTAGE_HIGH, 
                                     bg_color_value);
        }
        if(step_prg >= 10)
        {
            LCD_SetPosition(start_column, start_page,
                            ICON_MAIN4_PERCENTAGE_WIDE, ICON_MAIN4_PERCENTAGE_HIGH);
            LCD_DriverWriteDataFromExtFlash(icon_main4_progress_number_addr[0][step_prg / 10 % 10],
                                            ICON_MAIN4_PERCENTAGE_SIZE); 
            start_column += ICON_MAIN4_PERCENTAGE_WIDE + 1;
        }
        else
        {
            start_column = icon_main4_progress_number_coord[0][0]; 
            Gui_PageRefreshBackground(icon_main4_progress_number_coord[2][0], start_page,
                                     start_column - icon_main4_progress_number_coord[2][0], ICON_MAIN4_PERCENTAGE_HIGH, 
                                     bg_color_value);
            prg_cnt = 0;
        }
        
        LCD_SetPosition(start_column, start_page,
                        ICON_MAIN4_PERCENTAGE_WIDE, ICON_MAIN4_PERCENTAGE_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_main4_progress_number_addr[0][step_prg % 10],
                                        ICON_MAIN4_PERCENTAGE_SIZE); 
        start_column += ICON_MAIN4_PERCENTAGE_WIDE;
        
        Gui_PageRefreshBackground(start_column, start_page,
                                 icon_main4_percent_sign_coord[prg_cnt][0] - start_column, ICON_MAIN4_PERCENTAGE_HIGH,
                                 bg_color_value);
        // "%"
        LCD_SetPosition(icon_main4_percent_sign_coord[prg_cnt][0], icon_main4_percent_sign_coord[prg_cnt][1],
                        ICON_MAIN4_PERCENTAGE0_WIDE, ICON_MAIN4_PERCENTAGE0_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_main4_percent_sign_addr[0][0], ICON_MAIN4_PERCENTAGE0_SIZE);  
        
        return;
    }
//    else 
//    {   //�����-����
//        bg_index = (ui_style_type - 7) % 2;
//        //���̱���
//        LCD_SetPosition(icon_main5_dial_bg_coord[0][0], icon_main5_dial_bg_coord[0][1], 
//                        ICON_MAIN5_BG_WIDE, ICON_MAIN5_BG_HIGH);
//        LCD_DriverWriteDataFromExtFlash(icon_main5_dial_bg_addr[bg_index][0], ICON_MAIN5_BG_SIZE);
//        //����ָ��
////        Clock_DisplayDial(hour, minute, bg_index);  

//        /* ������ */
//        LCD_SetPosition(icon_main5_date_coord[0][0], icon_main5_date_coord[0][1], 
//                        ICON_MAIN5_DATE_WIDE, ICON_MAIN5_DATE_HIGH);
//        LCD_DriverWriteDataFromExtFlash(icon_main5_date_addr[bg_index][month / 10 % 2], ICON_MAIN5_DATE_SIZE);
//        LCD_SetPosition(icon_main5_date_coord[1][0], icon_main5_date_coord[1][1], 
//                        ICON_MAIN5_DATE_WIDE, ICON_MAIN5_DATE_HIGH);
//        LCD_DriverWriteDataFromExtFlash(icon_main5_date_addr[bg_index][month % 10], ICON_MAIN5_DATE_SIZE); 
//        
//        /* б�� */
//        LCD_SetPosition(icon_main5_date_line_coord[0][0], icon_main5_date_line_coord[0][1], 
//                        ICON_MAIN5_DATE1_WIDE, ICON_MAIN5_DATE1_HIGH);
//        LCD_DriverWriteDataFromExtFlash(icon_main5_date_line_addr[bg_index][0], ICON_MAIN5_DATE1_SIZE);
//        
//        /* ������ */
//        LCD_SetPosition(icon_main5_date_coord[2][0], icon_main5_date_coord[2][1], 
//                        ICON_MAIN5_DATE_WIDE, ICON_MAIN5_DATE_HIGH);
//        LCD_DriverWriteDataFromExtFlash(icon_main5_date_addr[bg_index][day / 10 % 4], ICON_MAIN5_DATE_SIZE);
//        LCD_SetPosition(icon_main5_date_coord[3][0], icon_main5_date_coord[3][1], 
//                        ICON_MAIN5_DATE_WIDE, ICON_MAIN5_DATE_HIGH);
//        LCD_DriverWriteDataFromExtFlash(icon_main5_date_addr[bg_index][day % 10], ICON_MAIN5_DATE_SIZE); 
//   
//        /* �������� */
//        LCD_SetPosition(icon_main5_week_coord[0][0], icon_main5_week_coord[0][1],
//                        ICON_MAIN5_WEEK_WIDE, ICON_MAIN5_WEEK_HIGH);
//        if(language == 0)
//        {
//            LCD_DriverWriteDataFromExtFlash(icon_main5_week_addr[bg_index][week - 1], ICON_MAIN5_WEEK_SIZE); 
//        }
//        else        
//        {
//            LCD_DriverWriteDataFromExtFlash(icon_main5_week_addr[bg_index][week - 1 + 7], ICON_MAIN5_WEEK_SIZE); 
//        }

//        //Ů�Թ���
//        if((bg_index == 1)&&(phys_menstrual > 0 && phys_menstrual < 5))
//        {
//            LCD_SetPosition(icon_main5_menstrual_coord[0][0], icon_main5_menstrual_coord[0][1],
//                            ICON_MAIN5_ICON1_WIDE, ICON_MAIN5_ICON1_HIGH);
//            LCD_DriverWriteDataFromExtFlash(icon_main5_menstrual_addr[0][phys_menstrual - 1], ICON_MAIN5_ICON1_SIZE);
//        }
//        else
//        {
//            Gui_PageClearBackground(icon_main5_menstrual_coord[0][0], icon_main5_menstrual_coord[0][1], 
//                                    ICON_MAIN5_ICON1_WIDE, ICON_MAIN5_ICON1_HIGH);
//        }
//    }
}

/****************************************************************
** Function name:           Gui_PageMainAssisant
** Descriptions:            ��ʾ����������ͼ��
** input parameters:        index����̬ͼ��ļ���
**                          ble_status��������״̬
**                          am_pm_format��AM,PM��״̬
**                          charge�����״̬
**                          battery_level����صĵȼ�
** output parameters:       
** Returned value:          
** Created by:              YX              
** Created Date:            2017-08-07
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Gui_PageMainAssisant(uint8_t index, uint8_t ble_status, uint8_t am_pm_format, 
                          uint8_t charge, uint8_t battery_level, uint8_t language, uint32_t step_all, uint8_t menstrual)
{
    static uint8_t last_bttery_state = 0xFF;
    static uint8_t last_battery_level = 0xFF;
    uint8_t bg_index = 1;
    uint8_t i = 0;
    uint8_t j = 0;
    uint32_t step_cnt = 0;
    
    uint8_t start_column = 0;
    uint8_t start_page = 0;
    
    if(ui_style_type <= 2 && ui_style_type >= 1)
    {   //���һ
        bg_index = (ui_style_type - 1) % 2;
        /* ��������ͼ�� */
        LCD_SetPosition(icon_main1_ble_coord[0][0], icon_main1_ble_coord[0][1], 
                        ICON_MAIN1GREEN_BLE_WIDE, ICON_MAIN1GREEN_BLE_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_main1_ble_addr[bg_index][ble_status % 2], ICON_MAIN1GREEN_BLE_SIZE);
  
        /* ���ͼ�� */
        if(charge == BATTERRY_IS_CHARGING)
        {
            last_bttery_state = 1;
            /* �������ͼ�� */
            LCD_SetPosition(icon_main1_battery_coord[0][0], icon_main1_battery_coord[0][1], 
                            ICON_MAIN1GREEN_BATTERY_WIDE, ICON_MAIN1GREEN_BATTERY_HIGH);
            LCD_DriverWriteDataFromExtFlash(icon_main1_battery_addr[bg_index][1], ICON_MAIN1GREEN_BATTERY_SIZE);
        }
        else
        {
            uint16_t bg_color_value = 0;
            uint32_t bg_addr = 0;

            bg_addr = icon_main1_bar_addr[bg_index][0];
            LCD_DriverReadFromExtFlash(bg_addr, (uint8_t *)&bg_color_value, 2); 
            if(battery_level == 0)
            {
                if((index % 2) >= 1)
                {
                    Gui_PageRefreshBackground(icon_main1_battery_coord[0][0], icon_main1_battery_coord[0][1], 
                                             ICON_MAIN1GREEN_BATTERY_WIDE, ICON_MAIN1GREEN_BATTERY_HIGH, bg_color_value);
                }
                else
                {
                    //* ���ͼ�� */backgroud_color_battery
                    LCD_SetPosition(icon_main1_battery_coord[0][0], icon_main1_battery_coord[0][1], 
                                    ICON_MAIN1GREEN_BATTERY_WIDE, ICON_MAIN1GREEN_BATTERY_HIGH);
                    LCD_DriverWriteDataFromExtFlash(icon_main1_battery_addr[bg_index][0], ICON_MAIN1GREEN_BATTERY_SIZE);

                    /* ������ͼ�� */
                    Gui_PageRefreshBackground(icon_main1_battery_level_coord[0][0], icon_main1_battery_level_coord[0][1], 
                                             icon_main1_battery_level_coord[4][0] - icon_main1_battery_level_coord[0][0], 4, 
                                             bg_color_value);
                }
            }
            else 
            {
                if(index == 0 || last_bttery_state == 1 || last_battery_level != battery_level)
                {
                    last_bttery_state = 0;
                    //����ϵ���UI���ܼ�ʱˢ�µ�bug
                    last_battery_level = battery_level;
                    ///* ���ͼ�� */backgroud_color_battery
                    LCD_SetPosition(icon_main1_battery_coord[0][0], icon_main1_battery_coord[0][1], 
                                    ICON_MAIN1GREEN_BATTERY_WIDE, ICON_MAIN1GREEN_BATTERY_HIGH);
                    LCD_DriverWriteDataFromExtFlash(icon_main1_battery_addr[bg_index][0], ICON_MAIN1GREEN_BATTERY_SIZE);

                }
                if(battery_level < 4)
                {
                    /* ������ͼ�� */
                    Gui_PageRefreshBackground(icon_main1_battery_level_coord[battery_level][0], icon_main1_battery_level_coord[battery_level][1], 
                                             icon_main1_battery_level_coord[4][0] - icon_main1_battery_level_coord[battery_level][0], 4, 
                                             bg_color_value);
                } 
            }
        } 
       
        /* ����CSR ����״̬,�����ڹ���λ�� */
           
        return; 
    }
    else if(ui_style_type >= 3 && ui_style_type <= 5)
    {   //����
        bg_index = (ui_style_type - 3) % 3;
        bg_index = (bg_index == 2 ? 0:1);
        /* ��������ͼ�� */
        LCD_SetPosition(icon_main2_ble_coord[0][0], icon_main2_ble_coord[0][1], 
                        ICON_MAIN2G_BLE_WIDE, ICON_MAIN2G_BLE_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_main2_ble_addr[bg_index][ble_status % 2], ICON_MAIN2G_BLE_SIZE);
  
        /* ���ͼ�� */
        if(charge == BATTERRY_IS_CHARGING)
        {
            last_bttery_state = 1;
            /* �������ͼ�� */
            LCD_SetPosition(icon_main2_battery_coord[0][0], icon_main2_battery_coord[0][1], 
                            ICON_MAIN2G_BATTERY_WIDE, ICON_MAIN2G_BATTERY_HIGH);
            LCD_DriverWriteDataFromExtFlash(icon_main2_battery_addr[bg_index][1], ICON_MAIN2G_BATTERY_SIZE);
        }
        else
        {
            uint16_t bg_color_value = 0;
            uint32_t bg_addr = 0;

            bg_addr = icon_main2_battery_addr[bg_index][0];
            LCD_DriverReadFromExtFlash(bg_addr, (uint8_t *)&bg_color_value, 2); 
            
            if(battery_level == 0)
            {
                if((index % 2) >= 1)
                {
                    Gui_PageRefreshBackground(icon_main2_battery_coord[0][0], icon_main2_battery_coord[0][1], 
                                             ICON_MAIN2G_BATTERY_WIDE, ICON_MAIN2G_BATTERY_HIGH, bg_color_value);
                }
                else
                {
                    //* ���ͼ�� */backgroud_color_battery
                    LCD_SetPosition(icon_main2_battery_coord[0][0], icon_main2_battery_coord[0][1], 
                                    ICON_MAIN2G_BATTERY_WIDE, ICON_MAIN2G_BATTERY_HIGH);
                    LCD_DriverWriteDataFromExtFlash(icon_main2_battery_addr[bg_index][0], ICON_MAIN2G_BATTERY_SIZE);

                    /* ������ͼ�� */
                    Gui_PageRefreshBackground(icon_main2_battery_level_coord[0][0], icon_main2_battery_level_coord[0][1], 
                                             icon_main2_battery_level_coord[4][0] - icon_main2_battery_level_coord[0][0], 5, 
                                             bg_color_value);
                }
            }
            else 
            {
                if(index == 0 || last_bttery_state == 1 || last_battery_level != battery_level)
                {
                    last_bttery_state = 0;
                    //����ϵ���UI���ܼ�ʱˢ�µ�bug
                    last_battery_level = battery_level;
                    ///* ���ͼ�� */backgroud_color_battery
                    LCD_SetPosition(icon_main2_battery_coord[0][0], icon_main2_battery_coord[0][1], 
                                    ICON_MAIN2G_BATTERY_WIDE, ICON_MAIN2G_BATTERY_HIGH);
                    LCD_DriverWriteDataFromExtFlash(icon_main2_battery_addr[bg_index][0], ICON_MAIN2G_BATTERY_SIZE);

                }
                if(battery_level < 4)
                {
                    /* ������ͼ�� */
                    Gui_PageRefreshBackground(icon_main2_battery_level_coord[battery_level][0], icon_main2_battery_level_coord[battery_level][1], 
                                             icon_main2_battery_level_coord[4][0] - icon_main2_battery_level_coord[battery_level][0], 5, 
                                             bg_color_value);
                } 
            }
        } 
       
        /* ����CSR ����״̬,�����ڹ���λ�� */

        
        /* ����ѥ�ӺͼƲ�*/
        LCD_SetPosition(icon_main2_step_shoes_coord[0][0], icon_main2_step_shoes_coord[0][1],
                        ICON_MAIN2_SHOES_WIDE, ICON_MAIN2_SHOES_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_main2_step_shoes_addr[0][0], ICON_MAIN2_SHOES_SIZE);
        step_cnt = step_all;
        if(step_cnt > 99999)
        {
            step_cnt = 99999;
        }
        for(j = 5; j > 0; j--)
        {
            if(step_all / 10 == 0)
            {
                break;
            }
            step_all = step_all / 10;
        }
        i = 0;
        do{
            LCD_SetPosition(icon_main2_step_number_coord[5 - j - i][0], icon_main2_step_number_coord[5 - j - i][1],
                            ICON_MAIN2_STEP_WIDE, ICON_MAIN2_STEP_HIGH);
            LCD_DriverWriteDataFromExtFlash(icon_main2_step_number_addr[0][step_cnt % 10], ICON_MAIN2_STEP_SIZE);
            i++;
            step_cnt = step_cnt / 10;
        }while(step_cnt > 0);

        if(i < 5)
        {
            Gui_PageClearBackground(icon_main2_step_number_coord[i][0], icon_main2_step_number_coord[i][1],
                                    ICON_MAIN2_STEP_WIDE, icon_main2_step_number_coord[5][1] - icon_main2_step_number_coord[i][1]);
        }
        
        return;
    }
    else if(ui_style_type == 0 || ui_style_type >= 7)
    {   //����� 
        /* ��������ͼ�� */
        LCD_SetPosition(icon_main3_ble_coord[0][0], icon_main3_ble_coord[0][1], 
                        ICON_MAIN3_BLE_WIDE, ICON_MAIN3_BLE_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_main3_ble_addr[0][ble_status % 2], ICON_MAIN3_BLE_SIZE);
  
        /* ����AM��PM��ʽ */
        uint8_t am_pm_index = 0;
        if(language != 0)
        {
            am_pm_index = 1;
        }
        if(am_pm_format == GUI_TIME_FORMAT_AM)
        {
            LCD_SetPosition(icon_main3_ampm_coord[0][0], icon_main3_ampm_coord[0][1],
                            ICON_MAIN3_AMPM_WIDE, ICON_MAIN3_AMPM_HIGH);
            LCD_DriverWriteDataFromExtFlash(icon_main3_ampm_addr[0][am_pm_index * 2], ICON_MAIN3_AMPM_SIZE);
        }
        else if(am_pm_format == GUI_TIME_FORMAT_PM)
        {
            LCD_SetPosition(icon_main3_ampm_coord[0][0], icon_main3_ampm_coord[0][1],
                            ICON_MAIN3_AMPM_WIDE, ICON_MAIN3_AMPM_HIGH);
            LCD_DriverWriteDataFromExtFlash(icon_main3_ampm_addr[0][am_pm_index * 2 + 1], ICON_MAIN3_AMPM_SIZE);
        }
        else
        {
            Gui_PageClearBackground(icon_main3_ampm_coord[0][0], icon_main3_ampm_coord[0][1], 
                                   ICON_MAIN3_AMPM_WIDE, ICON_MAIN3_AMPM_HIGH);
        }
        
        /* ���ͼ�� */
        if(charge == BATTERRY_IS_CHARGING)
        {
            last_bttery_state = 1;
            /* �������ͼ�� */
            LCD_SetPosition(icon_main3_battery_coord[0][0], icon_main3_battery_coord[0][1], 
                            ICON_MAIN3_BATTERY_WIDE, ICON_MAIN3_BATTERY_HIGH);
            LCD_DriverWriteDataFromExtFlash(icon_main3_battery_addr[0][1], ICON_MAIN3_BATTERY_SIZE);
        }
        else
        {     
            if(battery_level == 0)
            {
                if((index % 2) >= 1)
                {
                    Gui_PageClearBackground(icon_main3_battery_coord[0][0], icon_main3_battery_coord[0][1], 
                                           ICON_MAIN3_BATTERY_WIDE, ICON_MAIN3_BATTERY_HIGH);
                }
                else
                {
                    //* ���ͼ�� */backgroud_color_battery
                    LCD_SetPosition(icon_main3_battery_coord[0][0], icon_main3_battery_coord[0][1], 
                                    ICON_MAIN3_BATTERY_WIDE, ICON_MAIN3_BATTERY_HIGH);
                    LCD_DriverWriteDataFromExtFlash(icon_main3_battery_addr[0][0], ICON_MAIN3_BATTERY_SIZE);

                    /* ������ͼ�� */
                    Gui_PageClearBackground(icon_main3_battery_level_coord[0][0], icon_main3_battery_level_coord[0][1], 
                                           icon_main3_battery_level_coord[4][0] - icon_main3_battery_level_coord[0][0], 5);
                }
            }
            else 
            {
                if(index == 0 || last_bttery_state == 1 || last_battery_level != battery_level)
                {
                    last_bttery_state = 0;
                    //����ϵ���UI���ܼ�ʱˢ�µ�bug
                    last_battery_level = battery_level;
                    ///* ���ͼ�� */backgroud_color_battery
                    LCD_SetPosition(icon_main3_battery_coord[0][0], icon_main3_battery_coord[0][1], 
                                    ICON_MAIN3_BATTERY_WIDE, ICON_MAIN3_BATTERY_HIGH);
                    LCD_DriverWriteDataFromExtFlash(icon_main3_battery_addr[0][0], ICON_MAIN3_BATTERY_SIZE);

                }
                if(battery_level < 4)
                {
                    /* ������ͼ�� */
                    Gui_PageClearBackground(icon_main3_battery_level_coord[battery_level][0], icon_main3_battery_level_coord[battery_level][1], 
                                           icon_main3_battery_level_coord[4][0] - icon_main3_battery_level_coord[battery_level][0], 5);
                } 
            }
        } 
       
        /* ����CSR ����״̬,�����ڹ���λ�� */
 
        
        /* ���ƼƲ���*/
        uint32_t step_num = 1;
        step_cnt = step_all;
        if(step_cnt > 99999)
        {
            step_cnt = 99999;
        }
        for(j = 0; j < 5; j++)
        {
            if(step_all / 10 == 0)
            {
                break;
            }
            step_all = step_all / 10;
            step_num = step_num * 10;
        }
        
        if(j == 5)
        {
            j = j - 1;
            step_num = step_num / 10;
        }
        
        start_column = icon_main3_step_number_coord[j][0];
        start_page = icon_main3_step_number_coord[j][1];
        
        Gui_PageClearBackground(icon_main3_step_number_coord[4][0], start_page, 
                                icon_main3_step_number_coord[j][0] - icon_main3_step_number_coord[4][0],  ICON_MAIN3_STEP_HIGH);
        do{
            LCD_SetPosition(start_column, start_page,
                            ICON_MAIN3_STEP_WIDE, ICON_MAIN3_STEP_HIGH);
            LCD_DriverWriteDataFromExtFlash(icon_main3_step_number_addr[0][step_cnt / step_num % 10], ICON_MAIN3_STEP_SIZE);
            start_column += ICON_MAIN3_STEP_WIDE + 1;
            Gui_PageClearBackground(start_column - 1, start_page,1, ICON_MAIN3_STEP_HIGH);
            step_num = step_num / 10;
        }while(step_num > 0);
        Gui_PageClearBackground(start_column, start_page, 
                                icon_main3_step_number_coord[5][0] - start_column, ICON_MAIN3_STEP_HIGH);
        if(i < 5)
        {
            Gui_PageClearBackground(icon_main3_step_number_coord[i][0], icon_main3_step_number_coord[i][1],
                                    ICON_MAIN3_STEP_WIDE, icon_main3_step_number_coord[5][1] - icon_main3_step_number_coord[i][1]);
        }
        return;
    }
    else if(ui_style_type  == 6)
    {   //����� 
        /* ��������ͼ�� */
        LCD_SetPosition(icon_main4_ble_coord[0][0], icon_main4_ble_coord[0][1], 
                        ICON_MAIN4_BLE_WIDE, ICON_MAIN4_BLE_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_main4_ble_addr[0][ble_status % 2], ICON_MAIN4_BLE_SIZE);
  
        /* ����AM��PM��ʽ */
        uint8_t am_pm_index = 0;
        if(language != 0)
        {
            am_pm_index = 1;
        }
        if(am_pm_format == GUI_TIME_FORMAT_AM)
        {
            LCD_SetPosition(icon_main4_ampm_coord[0][0], icon_main4_ampm_coord[0][1],
                            ICON_MAIN4_AMPM_WIDE, ICON_MAIN4_AMPM_HIGH);
            LCD_DriverWriteDataFromExtFlash(icon_main4_ampm_addr[0][am_pm_index * 2], ICON_MAIN4_AMPM_SIZE);
        }
        else if(am_pm_format == GUI_TIME_FORMAT_PM)
        {
            LCD_SetPosition(icon_main4_ampm_coord[0][0], icon_main4_ampm_coord[0][1],
                            ICON_MAIN4_AMPM_WIDE, ICON_MAIN4_AMPM_HIGH);
            LCD_DriverWriteDataFromExtFlash(icon_main4_ampm_addr[0][am_pm_index * 2 + 1], ICON_MAIN4_AMPM_SIZE);
        }
        else
        {
            Gui_PageClearBackground(icon_main4_ampm_coord[0][0], icon_main4_ampm_coord[0][1], 
                                   ICON_MAIN4_AMPM_WIDE, ICON_MAIN4_AMPM_HIGH);
        }
        
        /* ���ͼ�� */
        if(charge == BATTERRY_IS_CHARGING)
        {
            last_bttery_state = 1;
            /* �������ͼ�� */
            LCD_SetPosition(icon_main4_battery_coord[0][0], icon_main4_battery_coord[0][1], 
                            ICON_MAIN4_BATTERY_WIDE, ICON_MAIN4_BATTERY_HIGH);
            LCD_DriverWriteDataFromExtFlash(icon_main4_battery_addr[0][1], ICON_MAIN4_BATTERY_SIZE);
        }
        else
        {     
            if(battery_level == 0)
            {
                if((index % 2) >= 1)
                {
                    Gui_PageClearBackground(icon_main4_battery_coord[0][0], icon_main4_battery_coord[0][1], 
                                           ICON_MAIN4_BATTERY_WIDE, ICON_MAIN4_BATTERY_HIGH);
                }
                else
                {
                    //* ���ͼ�� */backgroud_color_battery
                    LCD_SetPosition(icon_main4_battery_coord[0][0], icon_main4_battery_coord[0][1], 
                                    ICON_MAIN4_BATTERY_WIDE, ICON_MAIN4_BATTERY_HIGH);
                    LCD_DriverWriteDataFromExtFlash(icon_main4_battery_addr[0][0], ICON_MAIN4_BATTERY_SIZE);

                    /* ������ͼ�� */
                    Gui_PageClearBackground(icon_main4_battery_level_coord[0][0], icon_main4_battery_level_coord[0][1], 
                                           icon_main4_battery_level_coord[4][0] - icon_main4_battery_level_coord[0][0], 5);
                }
            }
            else 
            {
                if(index == 0 || last_bttery_state == 1 || last_battery_level != battery_level)
                {
                    last_bttery_state = 0;
                    //����ϵ���UI���ܼ�ʱˢ�µ�bug
                    last_battery_level = battery_level;
                    ///* ���ͼ�� */backgroud_color_battery
                    LCD_SetPosition(icon_main4_battery_coord[0][0], icon_main4_battery_coord[0][1], 
                                    ICON_MAIN4_BATTERY_WIDE, ICON_MAIN4_BATTERY_HIGH);
                    LCD_DriverWriteDataFromExtFlash(icon_main4_battery_addr[0][0], ICON_MAIN4_BATTERY_SIZE);

                }
                if(battery_level < 4)
                {
                    /* ������ͼ�� */
                    Gui_PageClearBackground(icon_main4_battery_level_coord[battery_level][0], icon_main4_battery_level_coord[battery_level][1], 
                                           icon_main4_battery_level_coord[4][0] - icon_main4_battery_level_coord[battery_level][0], 5);
                } 
            }
        } 
       
        /* ����CSR ����״̬,�����ڹ���λ�� */

        
        /* ���ƼƲ���*/
        uint32_t step_num = 1;
        step_cnt = step_all;
        if(step_cnt > 99999)
        {
            step_cnt = 99999;
        }
        for(j = 0; j < 5; j++)
        {
            if(step_all / 10 == 0)
            {
                break;
            }
            step_all = step_all / 10;
            step_num = step_num * 10;
        }
        
        if(j == 5)
        {
            j = j - 1;
            step_num = step_num / 10;
        }
        
        start_column = icon_main4_step_number_coord[j][0];
        start_page = icon_main4_step_number_coord[j][1];
        
        //������ͷ���ĵļƲ����ֹ���
        Gui_PageClearBackground(icon_main4_step_number_coord[4][0], start_page, 
                                icon_main4_step_number_coord[j][0] - icon_main4_step_number_coord[4][0],  ICON_MAIN3_STEP_HIGH);
        do{
            LCD_SetPosition(start_column, start_page,
                            ICON_MAIN3_STEP_WIDE, ICON_MAIN3_STEP_HIGH);
            LCD_DriverWriteDataFromExtFlash(icon_main4_step_number_addr[0][step_cnt / step_num % 10], ICON_MAIN3_STEP_SIZE);
            start_column += ICON_MAIN3_STEP_WIDE + 1;
            Gui_PageClearBackground(start_column - 1, start_page,1, ICON_MAIN3_STEP_HIGH);
            step_num = step_num / 10;
        }while(step_num > 0);
        Gui_PageClearBackground(start_column, start_page, 
                                icon_main4_step_number_coord[5][0] - start_column, ICON_MAIN3_STEP_HIGH);
        if(i < 5)
        {
            Gui_PageClearBackground(icon_main4_step_number_coord[i][0], icon_main4_step_number_coord[i][1],
                                    ICON_MAIN3_STEP_WIDE, icon_main4_step_number_coord[5][1] - icon_main4_step_number_coord[i][1]);
        }
        return;
    }
//    else 
//    {   //�����-����
//        bg_index = (ui_style_type - 7) % 2;
//        
//        /* ��������ͼ�� */
//        LCD_SetPosition(icon_main5_ble_coord[0][0], icon_main5_ble_coord[0][1], 
//                        ICON_MAIN5_BLE_WIDE, ICON_MAIN5_BLE_HIGH);
//        LCD_DriverWriteDataFromExtFlash(icon_main5_ble_addr[bg_index][ble_status % 2], ICON_MAIN5_BLE_SIZE);
//  
//        /* ���ͼ�� */
//        if(charge == BATTERRY_IS_CHARGING)
//        {
//            last_bttery_state = 1;
//            /* �������ͼ�� */
//            LCD_SetPosition(icon_main5_battery_coord[0][0], icon_main5_battery_coord[0][1], 
//                            ICON_MAIN5_BATTERY_WIDE, ICON_MAIN5_BATTERY_HIGH);
//            LCD_DriverWriteDataFromExtFlash(icon_main5_battery_addr[bg_index][1], ICON_MAIN5_BATTERY_SIZE);
//        }
//        else
//        {     
//            if(battery_level == 0)
//            {
//                if((index % 2) >= 1)
//                {
//                    Gui_PageClearBackground(icon_main5_battery_coord[0][0], icon_main5_battery_coord[0][1], 
//                                           ICON_MAIN5_BATTERY_WIDE, ICON_MAIN5_BATTERY_HIGH);
//                }
//                else
//                {
//                    //* ���ͼ�� */backgroud_color_battery
//                    LCD_SetPosition(icon_main5_battery_coord[0][0], icon_main5_battery_coord[0][1], 
//                                    ICON_MAIN5_BATTERY_WIDE, ICON_MAIN5_BATTERY_HIGH);
//                    LCD_DriverWriteDataFromExtFlash(icon_main5_battery_addr[bg_index][0], ICON_MAIN5_BATTERY_SIZE);

//                    /* ������ͼ�� */
//                    Gui_PageClearBackground(icon_main5_battery_level_coord[0][0], icon_main5_battery_level_coord[0][1], 
//                                           icon_main5_battery_level_coord[4][0] - icon_main5_battery_level_coord[0][0], 5);
//                }
//            }
//            else 
//            {
//                if(index == 0 || last_bttery_state == 1 || last_battery_level != battery_level)
//                {
//                    last_bttery_state = 0;
//                    //����ϵ���UI���ܼ�ʱˢ�µ�bug
//                    last_battery_level = battery_level;
//                    ///* ���ͼ�� */backgroud_color_battery
//                    LCD_SetPosition(icon_main5_battery_coord[0][0], icon_main5_battery_coord[0][1], 
//                                    ICON_MAIN5_BATTERY_WIDE, ICON_MAIN5_BATTERY_HIGH);
//                    LCD_DriverWriteDataFromExtFlash(icon_main5_battery_addr[bg_index][0], ICON_MAIN5_BATTERY_SIZE);

//                }
//                if(battery_level < 4)
//                {
//                    /* ������ͼ�� */
//                    Gui_PageClearBackground(icon_main5_battery_level_coord[battery_level][0], icon_main5_battery_level_coord[battery_level][1], 
//                                           icon_main5_battery_level_coord[4][0] - icon_main5_battery_level_coord[battery_level][0], 5);
//                } 
//            }
//        } 
//       
//        /* ����CSR ����״̬,�����ڹ���λ�� */
//       
//    }
}

/****************************************************************
** Function name:           Gui_PageStep
** Descriptions:            ��ʾ�Ʋ�����
** input parameters:        index����̬ͼ��ļ���
**                          step_count������
**                          step_target��Ŀ��
                            language: ����
** output parameters:       
** Returned value:          
** Created by:              YX              
** Created Date:            2017-08-07
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/  
void Gui_PageStep(uint32_t step_cnt, uint16_t target_cnt, uint8_t index, uint8_t language)
{
    uint8_t start_column = 0;
    uint8_t start_page = 0;
    
    if(step_cnt > 99999)
    {
        step_cnt = 99999;
    }
     /* ��沽�� */  
    if(step_cnt >= 1000)
    {
        start_page = icon_16_step_data_coord[0][1];        
        start_column = 38;//24;
        if(step_cnt >= 10000)
        {
            start_column = 15;
            LCD_SetPosition(start_column, start_page, ICON_FUNCTION_DATA_WIDE, ICON_FUNCTION_DATA_HIGH);
            LCD_DriverWriteDataFromExtFlash(icon_16_function_digital_addr[(step_cnt / 10000) % 10], ICON_FUNCTION_DATA_SIZE);
            start_column += ICON_FUNCTION_DATA_WIDE;
            Gui_PageClearBackground(start_column, start_page, 3, ICON_FUNCTION_DATA_HIGH);
            start_column += 3;
        }
        else
        {
            Gui_PageClearBackground(15, start_page, 38 - 15, ICON_FUNCTION_DATA_HIGH);
        }
        
        LCD_SetPosition(start_column, start_page, ICON_FUNCTION_DATA_WIDE, ICON_FUNCTION_DATA_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_function_digital_addr[(step_cnt / 1000) % 10], ICON_FUNCTION_DATA_SIZE);
        start_column += ICON_FUNCTION_DATA_WIDE;
        Gui_PageClearBackground(start_column, start_page, 3, ICON_FUNCTION_DATA_HIGH);
        start_column += 3;
        
        //ǧλ�ָ���
        LCD_SetPosition(icon_16_step_data_coord[1][0], icon_16_step_data_coord[1][1], ICON_FUNCTION_SEPARATOR_WIDE, ICON_FUNCTION_SEPARATOR_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_function_digital_addr[11], ICON_FUNCTION_SEPARATOR_SIZE);      
    }
    
    start_page = icon_16_step_data_coord[2][1];
    if(step_cnt >= 100)
    {     
        start_column = 10;        
        LCD_SetPosition(start_column, start_page, ICON_FUNCTION_DATA_WIDE, ICON_FUNCTION_DATA_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_function_digital_addr[(step_cnt / 100) % 10], ICON_FUNCTION_DATA_SIZE);
        start_column += ICON_FUNCTION_DATA_WIDE;
        Gui_PageClearBackground(start_column, start_page, 2, ICON_FUNCTION_DATA_HIGH);
        start_column += 2;
    }
    else
    {
        Gui_PageClearBackground(0, icon_16_step_data_coord[0][1], LCD_MAX_COLUMN, ICON_FUNCTION_DATA_HIGH);
        Gui_PageClearBackground(0, start_page, 18, ICON_FUNCTION_DATA_HIGH);
        start_column = 18; 
    }
    if(step_cnt >= 10)
    {            
        LCD_SetPosition(start_column, start_page, ICON_FUNCTION_DATA_WIDE, ICON_FUNCTION_DATA_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_function_digital_addr[(step_cnt / 10) % 10], ICON_FUNCTION_DATA_SIZE);
        start_column += ICON_FUNCTION_DATA_WIDE;
        Gui_PageClearBackground(start_column, start_page, 2, ICON_FUNCTION_DATA_HIGH);
        start_column += 2;
    }
    else
    {
        Gui_PageClearBackground(0, icon_16_step_data_coord[0][1], LCD_MAX_COLUMN, ICON_FUNCTION_DATA_HIGH);
        Gui_PageClearBackground(0, start_page, 29, ICON_FUNCTION_DATA_HIGH);
        start_column = 29;   
    }
    LCD_SetPosition(start_column, start_page, ICON_FUNCTION_DATA_WIDE, ICON_FUNCTION_DATA_HIGH);
    LCD_DriverWriteDataFromExtFlash(icon_16_function_digital_addr[step_cnt % 10], ICON_FUNCTION_DATA_SIZE);
    start_column += ICON_FUNCTION_DATA_WIDE;
    Gui_PageClearBackground(start_column, start_page, LCD_MAX_COLUMN - start_column, ICON_FUNCTION_DATA_HIGH);

    /* ���STEP */
    LCD_SetPosition(icon_16_step_unit_coord[0][0], icon_16_step_unit_coord[0][1],
                    ICON_STEP_UNIT_WIDE, ICON_STEP_UNIT_HIGH);
    LCD_DriverWriteDataFromExtFlash(icon_16_step_unit_addr[language % 2], ICON_STEP_UNIT_SIZE);

    /* ���Ʋ����� */
    if(index == 0)
    {
        LCD_SetPosition(icon_16_step_ring_coord[0][0], icon_16_step_ring_coord[0][1],
                        ICON_STEP_RINGS_WIDE, ICON_STEP_RINGS_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_step_ring_addr[0], ICON_STEP_RINGS_SIZE);
    }
    //WJ.20180226.����ֵ������
    if(target_cnt > 100)
    {
        target_cnt = 100;
    }
    if(target_cnt <= 100)
    {
        for(uint8_t i = 0; i < target_cnt / 5; i++)//((step_count*100)/step_target)/5
        {
            LCD_SetPosition(icon_16_step_prog_coord[i][0], icon_16_step_prog_coord[i][1],
                            ICON_STEP_PROGRESS_WIDE, ICON_STEP_PROGRESS_HIGH);
            LCD_DriverWriteDataFromExtFlash(icon_16_step_prog_addr[i], ICON_STEP_PROGRESS_SIZE);
        }
    }
    /* ���Ʋ�UI */
    /* ������ܻỨ���Ĳ��� */
    Gui_PageClearBackground(icon_16_step_icon_coord[3][0], icon_16_step_icon_coord[3][1], 
                            ICON_STEP_SHOES_WIDE, icon_16_step_icon_coord[index % 4][1] - icon_16_step_icon_coord[3][1]);
    Gui_PageClearBackground(icon_16_step_icon_coord[index % 4][0], icon_16_step_icon_coord[index % 4][1] + ICON_STEP_SHOES_HIGH, 
                            ICON_STEP_SHOES_WIDE, icon_16_step_icon_coord[1][1] - icon_16_step_icon_coord[index % 4][1]);
    /* ��ʾͼ�� */
    LCD_SetPosition(icon_16_step_icon_coord[index % 4][0], icon_16_step_icon_coord[index % 4][1], 
                    ICON_STEP_SHOES_WIDE, ICON_STEP_SHOES_HIGH);
    LCD_DriverWriteDataFromExtFlash(icon_16_step_icon_addr[0], ICON_STEP_SHOES_SIZE);
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
void Gui_PageDistance(float dis_count, uint8_t index, uint8_t uint_type, uint8_t language)
{
    static uint8_t start_column = 0;
    uint8_t start_page = 0;
    uint16_t dis_count_tmp = 0;
    
    /* ��̾�̬ͼ�� */
    if(index == 0)
    {
        LCD_SetPosition(icon_16_distance_ring_coord[0][0], icon_16_distance_ring_coord[0][1], 
                        ICON_DISTANCE_RING_WIDE, ICON_DISTANCE_RING_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_distance_ring_addr[0], ICON_DISTANCE_RING_SIZE);
    }
    /* Ϊ��̬ͼ������� */
    Gui_PageClearBackground(icon_16_distance_map1_coord[3][0], icon_16_distance_map1_coord[3][1], 
                           ICON_DISTANCE_MAP1_WIDE, 
                           icon_16_distance_map1_coord[index % 4][1] - icon_16_distance_map1_coord[3][1]);
    /* ��̶�̬ͼ�� */
    LCD_SetPosition(icon_16_distance_map1_coord[index % 4][0], icon_16_distance_map1_coord[index % 4][1],
                    ICON_DISTANCE_MAP1_WIDE, ICON_DISTANCE_MAP1_HIGH);
    LCD_DriverWriteDataFromExtFlash(icon_16_distance_map1_addr[0], ICON_DISTANCE_MAP1_SIZE);
    
    LCD_SetPosition(icon_16_distance_map2_coord[index % 4][0], icon_16_distance_map2_coord[index % 4][1],
                    ICON_DISTANCE_MAP2_WIDE, ICON_DISTANCE_MAP2_HIGH);
    LCD_DriverWriteDataFromExtFlash(icon_16_distance_map2_addr[0], ICON_DISTANCE_MAP2_SIZE);

    /* Ϊ��̬ͼ������� */
    //�ײ�
    Gui_PageClearBackground(icon_16_distance_map2_coord[index % 4][0], icon_16_distance_map2_coord[index % 4][1] + ICON_DISTANCE_MAP2_HIGH,
                            ICON_DISTANCE_MAP2_WIDE, 
                            icon_16_distance_map2_coord[1][1] - icon_16_distance_map2_coord[index % 4][1]);
    //���  
    Gui_PageClearBackground(icon_16_distance_map1_coord[index % 4][0], icon_16_distance_map1_coord[index % 4][1] + ICON_DISTANCE_MAP1_HIGH,
                            icon_16_distance_map2_coord[index % 4][0] - icon_16_distance_map1_coord[index % 4][0], 
                            6 - (icon_16_distance_map1_coord[index % 4][1] - icon_16_distance_map1_coord[3][1]));
    //�ұ�
    Gui_PageClearBackground(icon_16_distance_map2_coord[index % 4][0] + ICON_DISTANCE_MAP2_WIDE, icon_16_distance_map1_coord[index % 4][1] + ICON_DISTANCE_MAP1_HIGH,
                            icon_16_distance_map1_coord[index % 4][0] + ICON_DISTANCE_MAP1_WIDE - (icon_16_distance_map2_coord[index % 4][0] + ICON_DISTANCE_MAP2_WIDE), 
                            6 - (icon_16_distance_map1_coord[index % 4][1] - icon_16_distance_map1_coord[3][1]));     
    /* ��ʾ�������� */
    if(dis_count >=100)
    {
        dis_count = 99.9;
    }
    
    dis_count_tmp = dis_count;
    dis_count_tmp = dis_count * 10;
    
    start_page = icon_16_distance_data_coord[0][1];
    if(dis_count_tmp >= 100)
    {
        start_column = icon_16_distance_data_coord[0][0];
        LCD_SetPosition(start_column, start_page, ICON_FUNCTION_DATA_WIDE, ICON_FUNCTION_DATA_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_function_digital_addr[(dis_count_tmp / 100) % 10], ICON_FUNCTION_DATA_SIZE);
        start_column += ICON_FUNCTION_DATA_WIDE;
        Gui_PageClearBackground(start_column, start_page, 2, ICON_FUNCTION_DATA_HIGH);
        start_column += 2;
    }
    else
    {
        start_column = icon_16_distance_data_coord[1][0];
        Gui_PageClearBackground(0, start_page, start_column, ICON_FUNCTION_DATA_HIGH);
    }
    LCD_SetPosition(start_column, start_page, ICON_FUNCTION_DATA_WIDE, ICON_FUNCTION_DATA_HIGH);
    LCD_DriverWriteDataFromExtFlash(icon_16_function_digital_addr[(dis_count_tmp / 10) % 10], ICON_FUNCTION_DATA_SIZE);
    start_column += ICON_FUNCTION_DATA_WIDE;
    Gui_PageClearBackground(start_column, start_page, 2, ICON_FUNCTION_DATA_HIGH);
    start_column += 2;
    //"."
    LCD_SetPosition(start_column, icon_16_distance_point_coord[0][1],
                    ICON_FUNCTION_POINT_WIDE, ICON_FUNCTION_POINT_HIGH);
    LCD_DriverWriteDataFromExtFlash(icon_16_distance_point_addr[0], ICON_FUNCTION_POINT_SIZE);
    Gui_PageClearBackground(start_column, start_page, ICON_FUNCTION_POINT_WIDE, 
                           ICON_FUNCTION_DATA_HIGH - ICON_FUNCTION_POINT_HIGH);
    start_column += ICON_FUNCTION_POINT_WIDE;
    
    Gui_PageClearBackground(start_column, start_page, 2, ICON_FUNCTION_DATA_HIGH);
    start_column += 2;
    
    LCD_SetPosition(start_column, start_page, ICON_FUNCTION_DATA_WIDE, ICON_FUNCTION_DATA_HIGH);
    LCD_DriverWriteDataFromExtFlash(icon_16_function_digital_addr[dis_count_tmp % 10], ICON_FUNCTION_DATA_SIZE);
    start_column += ICON_FUNCTION_DATA_WIDE;
    
    Gui_PageClearBackground(start_column, start_page, LCD_MAX_COLUMN - start_column, ICON_FUNCTION_DATA_HIGH);

    /* ���뵥λ */
    if(language == 0)
    {
		LCD_SetPosition(icon_16_distance_unit_coord[0][0], icon_16_distance_unit_coord[0][1], 
                        ICON_DISTANCE_UNIT_WIDE, ICON_DISTANCE_UNIT_HIGH);
		if(uint_type == 0)
		{
			LCD_DriverWriteDataFromExtFlash(icon_16_distance_unit_addr[0], ICON_DISTANCE_UNIT_SIZE);
		}
		else
		{
			LCD_DriverWriteDataFromExtFlash(icon_16_distance_unit_addr[3], ICON_DISTANCE_UNIT_SIZE);
		}
    }
    else
    {
        LCD_SetPosition(icon_16_distance_unit_coord[0][0], icon_16_distance_unit_coord[0][1], 
                        ICON_DISTANCE_UNIT_WIDE, ICON_DISTANCE_UNIT_HIGH);
        if(uint_type == 0)
        {
            LCD_DriverWriteDataFromExtFlash(icon_16_distance_unit_addr[1], ICON_DISTANCE_UNIT_SIZE);
        }
        else
        {
            LCD_DriverWriteDataFromExtFlash(icon_16_distance_unit_addr[2], ICON_DISTANCE_UNIT_SIZE);
        }
    }
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
void Gui_PageCalories(float calorie, uint8_t index, uint8_t language)
{
    uint8_t start_column = 0;
    uint8_t start_page = 0;
    uint32_t calorie_tmp = 0;
    
    if(index == 0)
    {
        /* ��·�ﾲ̬ͼ�� */
        LCD_SetPosition(icon_16_calories_contour_coord[0][0], icon_16_calories_contour_coord[0][1],
                        ICON_CALORIES_ICON1_WIDE, ICON_CALORIES_ICON1_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_calories_contour_addr[0], ICON_CALORIES_ICON1_SIZE);
    } 

    /* ��·�ﶯ̬ͼ�� */
    LCD_SetPosition(icon_16_calories_flame_coord[0][0], icon_16_calories_flame_coord[0][1], 
                    ICON_CALORIES_ICON_WIDE, ICON_CALORIES_ICON_HIGH);
    LCD_DriverWriteDataFromExtFlash(icon_16_calories_flame_addr[index % 3 + 1], ICON_CALORIES_ICON_SIZE);

    /* ��ʾ��·������ */
    calorie_tmp = calorie;
    if(calorie_tmp > 9999)
    {
        calorie_tmp = 9999;
    }
    
    if(calorie_tmp >= 1000)
    {    
        start_page = icon_16_calories_data_coord[3][1];
        start_column = icon_16_calories_data_coord[3][0];
        LCD_SetPosition(start_column, start_page, ICON_CLOCK_NUMBER_WIDE, ICON_CLOCK_NUMBER_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_clock_number_addr[calorie_tmp / 1000 % 10], ICON_CLOCK_NUMBER_SIZE);        
        start_column += ICON_CLOCK_NUMBER_WIDE;
        Gui_PageClearBackground(start_column, start_page, 2, ICON_CLOCK_NUMBER_HIGH);
        start_column += 2;
        
        LCD_SetPosition(start_column, start_page, ICON_CLOCK_NUMBER_WIDE, ICON_CLOCK_NUMBER_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_clock_number_addr[calorie_tmp / 100 % 10], ICON_CLOCK_NUMBER_SIZE);        
        start_column += ICON_CLOCK_NUMBER_WIDE;
        Gui_PageClearBackground(start_column, start_page, 2, ICON_CLOCK_NUMBER_HIGH);
        start_column += 2;
        
        LCD_SetPosition(start_column, start_page, ICON_CLOCK_NUMBER_WIDE, ICON_CLOCK_NUMBER_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_clock_number_addr[calorie_tmp / 10 % 10], ICON_CLOCK_NUMBER_SIZE);        
        start_column += ICON_CLOCK_NUMBER_WIDE;
        Gui_PageClearBackground(start_column, start_page, 2, ICON_CLOCK_NUMBER_HIGH);
        start_column += 2;
        
        LCD_SetPosition(start_column, start_page, ICON_CLOCK_NUMBER_WIDE, ICON_CLOCK_NUMBER_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_clock_number_addr[calorie_tmp % 10], ICON_CLOCK_NUMBER_SIZE);        
        start_column += ICON_CLOCK_NUMBER_WIDE;
        Gui_PageClearBackground(start_column, start_page, 2, ICON_CLOCK_NUMBER_HIGH);

        Gui_PageClearBackground(0, icon_16_calories_data_coord[0][1], LCD_MAX_COLUMN, 
                               icon_16_calories_data_coord[3][1] - icon_16_calories_data_coord[0][1]);                               
    }  
    else
    {    
        start_page = icon_16_calories_data_coord[0][1];  
        if(calorie_tmp >= 100)
        {     
            start_column = icon_16_calories_data_coord[0][0];        
            LCD_SetPosition(start_column, start_page, ICON_FUNCTION_DATA_WIDE, ICON_FUNCTION_DATA_HIGH);
            LCD_DriverWriteDataFromExtFlash(icon_16_function_digital_addr[(calorie_tmp / 100) % 10], ICON_FUNCTION_DATA_SIZE);
            start_column += ICON_FUNCTION_DATA_WIDE;
            Gui_PageClearBackground(start_column, start_page, 2, ICON_FUNCTION_DATA_HIGH);
            start_column += 2;
        }
        else
        {         
            start_column = icon_16_calories_data_coord[1][0]; 
            Gui_PageClearBackground(0, start_page, start_column, ICON_FUNCTION_DATA_HIGH);
        }
        if(calorie_tmp >= 10)
        {            
            LCD_SetPosition(start_column, start_page, ICON_FUNCTION_DATA_WIDE, ICON_FUNCTION_DATA_HIGH);
            LCD_DriverWriteDataFromExtFlash(icon_16_function_digital_addr[(calorie_tmp / 10) % 10], ICON_FUNCTION_DATA_SIZE);
            start_column += ICON_FUNCTION_DATA_WIDE;
            Gui_PageClearBackground(start_column, start_page, 2, ICON_FUNCTION_DATA_HIGH);
            start_column += 2;
        }
        else
        {         
            start_column = icon_16_calories_data_coord[2][0]; 
            Gui_PageClearBackground(0, start_page, start_column, ICON_FUNCTION_DATA_HIGH);
  
        }
        LCD_SetPosition(start_column, start_page, ICON_FUNCTION_DATA_WIDE, ICON_FUNCTION_DATA_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_function_digital_addr[calorie_tmp % 10], ICON_FUNCTION_DATA_SIZE);
        start_column += ICON_FUNCTION_DATA_WIDE;
        Gui_PageClearBackground(start_column, start_page, LCD_MAX_COLUMN - start_column, ICON_FUNCTION_DATA_HIGH);
    }

    /* ��·�ﵥλ */
    LCD_SetPosition(icon_16_calories_unit_coord[0][0], icon_16_calories_unit_coord[0][1], 
                    ICON_CALORIES_UNIT_WIDE, ICON_CALORIES_UNIT_HIGH);
    LCD_DriverWriteDataFromExtFlash(icon_16_calories_unit_addr[language % 2], ICON_CALORIES_UNIT_SIZE);
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
void Gui_PageHeart(uint8_t index, uint8_t heart, uint8_t language)
{
    uint8_t start_column = 0;
    uint8_t start_page = 0;
    
    if(index == 0)
    {
        /* ��������ͼ�꾲̬ */
        LCD_SetPosition(icon_16_heart_contour_coord[0][0], icon_16_heart_contour_coord[0][1], 
                        ICON_HEARTRATE_GRAPHICS_WIDE, ICON_HEARTRATE_GRAPHICS_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_heart_contour_addr[0], ICON_HEARTRATE_GRAPHICS_SIZE);
    }
    /* ��������ͼ�궯̬ */
    LCD_SetPosition(icon_16_heart_line_coord[0][0], icon_16_heart_line_coord[0][1], 
                    ICON_HEARTRATE_LINE_WIDE, ICON_HEARTRATE_LINE_HIGH);
    LCD_DriverWriteDataFromExtFlash(icon_16_heart_line_addr[index % 7], ICON_HEARTRATE_LINE_SIZE);
	/* ��ʾ����ֵ */
    start_page = icon_16_heart_data_coord[0][1];
    if(heart >= 100)
    {
        start_column = 6;
        LCD_SetPosition(start_column, start_page, ICON_FUNCTION_DATA_WIDE, ICON_FUNCTION_DATA_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_function_digital_addr[(heart / 100) % 10], ICON_FUNCTION_DATA_SIZE);
        start_column += ICON_FUNCTION_DATA_WIDE;
        Gui_PageClearBackground(start_column, start_page, 2, ICON_FUNCTION_DATA_HIGH);
        start_column += 2;
    }
    else
    {
        start_column = 18;
        Gui_PageClearBackground(6, start_page, start_column - 6, ICON_FUNCTION_DATA_HIGH);
    }
    
    if(heart >= 10)
    {
        LCD_SetPosition(start_column, start_page, ICON_FUNCTION_DATA_WIDE, ICON_FUNCTION_DATA_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_function_digital_addr[(heart / 10) % 10], ICON_FUNCTION_DATA_SIZE);
        start_column += ICON_FUNCTION_DATA_WIDE;
        Gui_PageClearBackground(start_column, start_page, 2, ICON_FUNCTION_DATA_HIGH);
        start_column += 2;
    }
    else
    {
        start_column = 29;
        Gui_PageClearBackground(6, start_page, start_column - 6, ICON_FUNCTION_DATA_HIGH);
    }
    LCD_SetPosition(start_column, start_page, ICON_FUNCTION_DATA_WIDE, ICON_FUNCTION_DATA_HIGH);
    LCD_DriverWriteDataFromExtFlash(icon_16_function_digital_addr[heart % 10], ICON_FUNCTION_DATA_SIZE);
    start_column += ICON_FUNCTION_DATA_WIDE;
    Gui_PageClearBackground(start_column, start_page, 
                            LCD_MAX_COLUMN - start_column, ICON_FUNCTION_DATA_HIGH);

    /* bmp */
    LCD_SetPosition(icon_16_heart_unit_coord[0][0], icon_16_heart_unit_coord[0][1], 
                    ICON_HEARTRATE_UNIT_WIDE, ICON_HEARTRATE_UNIT_HIGH);
    LCD_DriverWriteDataFromExtFlash(icon_16_heart_unit_addr[language % 2], ICON_HEARTRATE_UNIT_SIZE);
}

/****************************************************************
** Function name:           Gui_PageSleep
** Descriptions:            ˯�߽���
** input parameters:        index����̬ͼ���� 
**                          time��˯�ߵ�ʱ��
**                          language����λ��ʾ������
** output parameters:       
** Returned value:          
** Created by:              YX              
** Created Date:            2017-08-11
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Gui_PageSleep(uint8_t index, uint16_t time, uint8_t language)
{
    uint8_t start_column = 0;
    uint8_t start_page = 0; 
    uint8_t sleep_hours = time / 60;
    uint8_t sleep_minutes = 10 * (time % 60 / 60.0f);
	/* ˯�߶�̬ͼ */
    LCD_SetPosition(icon_16_sleep_icon_coord[0][0], icon_16_sleep_icon_coord[0][1], 
                    ICON_SLEEP_ICON_WIDE, ICON_SLEEP_ICON_HIGH);
    LCD_DriverWriteDataFromExtFlash(icon_16_sleep_icon_addr[index % 4], ICON_SLEEP_ICON_SIZE);
    if(index == 0)
    {
        /* ˯��ʱ�� */
        start_page = icon_16_sleep_data_coord[0][1];
        Gui_PageClearBackground(start_column, start_page, LCD_MAX_COLUMN, ICON_FUNCTION_DATA_HIGH);
        
        if(sleep_hours > 99)
        {
            sleep_hours = 99;
            sleep_minutes = 9;
        }		
        if(sleep_hours > 9)
        {
            start_column = icon_16_sleep_data_coord[0][0];
            LCD_SetPosition(start_column, start_page, ICON_FUNCTION_DATA_WIDE, ICON_FUNCTION_DATA_HIGH);
            LCD_DriverWriteDataFromExtFlash(icon_16_function_digital_addr[(sleep_hours % 100) / 10], ICON_FUNCTION_DATA_SIZE);
            start_column += ICON_FUNCTION_DATA_WIDE + 2;
        }
        else
        {
            start_column = icon_16_sleep_data_coord[1][0];
        }
        LCD_SetPosition(start_column, start_page, ICON_FUNCTION_DATA_WIDE, ICON_FUNCTION_DATA_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_function_digital_addr[sleep_hours % 10], ICON_FUNCTION_DATA_SIZE);
        start_column += ICON_FUNCTION_DATA_WIDE + 2;
        
        /* �� */
        LCD_SetPosition(start_column, icon_16_sleep_point_coord[0][1], ICON_FUNCTION_POINT_WIDE, ICON_FUNCTION_POINT_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_sleep_point_addr[0], ICON_FUNCTION_POINT_SIZE);
        start_column += ICON_FUNCTION_POINT_WIDE + 2;
        
        LCD_SetPosition(start_column, start_page, ICON_FUNCTION_DATA_WIDE, ICON_FUNCTION_DATA_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_function_digital_addr[sleep_minutes % 10], ICON_FUNCTION_DATA_SIZE);        
    }
    /* unit */
    LCD_SetPosition(icon_16_sleep_unit_coord[0][0], icon_16_sleep_unit_coord[0][1], 
                    ICON_SLEEP_UNIT_WIDE, ICON_SLEEP_UNIT_HIGH);
    LCD_DriverWriteDataFromExtFlash(icon_16_sleep_unit_addr[language % 2], ICON_SLEEP_UNIT_SIZE);
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
void Gui_PagePower(void)
{
    /* �ػ�ͼ�� */
    LCD_SetPosition(icon_16_poweroff_icon_coord[0][0], icon_16_poweroff_icon_coord[0][1], 
                    ICON_OFF_WIDE, ICON_OFF_HIGH);
    LCD_DriverWriteDataFromExtFlash(icon_16_poweroff_icon_addr[0], ICON_OFF_SIZE);
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
void Gui_PageFindPhone(bool dynamic_find, uint8_t index)
{
    if(index == 0)
    {
        /* �ֻ�ͼ�� */
        LCD_SetPosition(icon_16_findphone_phone_coord[0][0], icon_16_findphone_phone_coord[0][1], 
                        ICON_FIND_PHONES_WIDE, ICON_FIND_PHONES_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_findphone_phone_addr[0], ICON_FIND_PHONES_SIZE);
        
    }
    
	if(!dynamic_find)
    {
        /* �رն�̬ͼ */
		index = 0;
        /* �Ŵ󾵶�̬ͼ�� */
        LCD_SetPosition(icon_16_findphone_magnifier_coord[0][0], icon_16_findphone_magnifier_coord[0][1], 
                        ICON_FIND_PHONES1_WIDE, ICON_FIND_PHONES1_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_findphone_magnifier_addr[4], ICON_FIND_PHONES1_SIZE);
        return;
    }
    /* �Ŵ󾵶�̬ͼ�� */
    LCD_SetPosition(icon_16_findphone_magnifier_coord[0][0], icon_16_findphone_magnifier_coord[0][1], 
                    ICON_FIND_PHONES1_WIDE, ICON_FIND_PHONES1_HIGH);
    LCD_DriverWriteDataFromExtFlash(icon_16_findphone_magnifier_addr[index % 4], ICON_FIND_PHONES1_SIZE);
    
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
void GUI_StopWatchPage(bool entry, uint16_t total_second, uint8_t one_hundred_ms, uint8_t index)
{
    uint8_t minute = 0;
    uint8_t second = 0;
    
    if(entry == 0)
    {
        if(index == 0)
        {
            LCD_SetPosition(icon_16_stopwatch_contour_coord[0][0], icon_16_stopwatch_contour_coord[0][1], 
                            ICON_STOPWATCH_CONTOUR_WIDE, ICON_STOPWATCH_CONTOUR_HIGH);
            LCD_DriverWriteDataFromExtFlash(icon_16_stopwatch_contour_addr[0], ICON_STOPWATCH_CONTOUR_SIZE);
        }
        LCD_SetPosition(icon_16_stopwatch_pointer_coord[0][0], icon_16_stopwatch_pointer_coord[0][1], 
                        ICON_STOPWATCH_POINTER_WIDE, ICON_STOPWATCH_POINTER_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_stopwatch_pointer_addr[index % 8], ICON_STOPWATCH_POINTER_SIZE);
    }
    else
    {
        second = total_second % 60;
        minute = total_second / 60 % 100;
        if((total_second == 5999 && one_hundred_ms >= 9) 
           || total_second > 5999)
        {
            one_hundred_ms = 99;
        }
        
        //��
        LCD_SetPosition(icon_16_stopwatch_data_coord[0][0], icon_16_stopwatch_data_coord[0][1], 
                        ICON_TIMING_MIN_WIDE, ICON_TIMING_MIN_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_stopwatch_number1_addr[minute / 10 % 10], ICON_TIMING_MIN_SIZE);
        LCD_SetPosition(icon_16_stopwatch_data_coord[1][0], icon_16_stopwatch_data_coord[1][1], 
                        ICON_TIMING_MIN_WIDE, ICON_TIMING_MIN_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_stopwatch_number1_addr[minute % 10], ICON_TIMING_MIN_SIZE);
        //��
        LCD_SetPosition(icon_16_stopwatch_data_coord[2][0], icon_16_stopwatch_data_coord[2][1], 
                        ICON_TIMING_MIN_WIDE, ICON_TIMING_MIN_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_stopwatch_number2_addr[second / 10 % 10], ICON_TIMING_MIN_SIZE);
        LCD_SetPosition(icon_16_stopwatch_data_coord[3][0], icon_16_stopwatch_data_coord[3][1], 
                        ICON_TIMING_MIN_WIDE, ICON_TIMING_MIN_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_stopwatch_number2_addr[second % 10], ICON_TIMING_MIN_SIZE);
        //����
        LCD_SetPosition(icon_16_stopwatch_data_coord[4][0], icon_16_stopwatch_data_coord[4][1], 
                        ICON_TIMING_MIN_WIDE, ICON_TIMING_MIN_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_stopwatch_number3_addr[one_hundred_ms /10 % 10], ICON_TIMING_MIN_SIZE);
        LCD_SetPosition(icon_16_stopwatch_data_coord[5][0], icon_16_stopwatch_data_coord[5][1], 
                        ICON_TIMING_MIN_WIDE, ICON_TIMING_MIN_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_stopwatch_number3_addr[one_hundred_ms % 10], ICON_TIMING_MIN_SIZE); 
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
void Gui_PageCountDown(bool dynamic, uint8_t index)
{
	if(!dynamic)
    {
        /* �رն�̬ͼ */
		index = 0;	
    }
    LCD_SetPosition(icon_16_countdown_icon_coord[0][0], icon_16_countdown_icon_coord[0][1], 
                    ICON_COUNTDOWN_ICON_WIDE, ICON_COUNTDOWN_ICON_HIGH);
    LCD_DriverWriteDataFromExtFlash(icon_16_countdown_icon_addr[index % 6], ICON_COUNTDOWN_ICON_SIZE);
    		
}

/****************************************************************
** Function name:           Gui_PageCameraReady
** Descriptions:            �������
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
void Gui_PageCameraReady(uint8_t index)
{
    if(index == 0)
    {
        /* ��ʾ��̬ͼ�� */
        LCD_SetPosition(icon_16_camera_contour_coord[0][0], icon_16_camera_contour_coord[0][1],
                        ICON_CAMERA_CONTOUR_WIDE, ICON_CAMERA_CONTOUR_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_camera_contour_addr[0], ICON_CAMERA_CONTOUR_SIZE);
    }
    
    /* ��ʾ��̬ͼ�� */
    LCD_SetPosition(icon_16_camera_round_coord[0][0], icon_16_camera_round_coord[0][1],
                    ICON_CAMERA_ROUND_WIDE, ICON_CAMERA_ROUND_HIGH);
    LCD_DriverWriteDataFromExtFlash(icon_16_camera_round_addr[index % 3], ICON_CAMERA_ROUND_SIZE);
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
void Gui_PageDisMacVer(uint8_t index, uint8_t *mac_data, uint8_t *ver_data)
{
    uint8_t dis_mac_data_buf[10] = {0};
    uint8_t dis_ver_data_buf[10] = {0};
    if(index == 0)
    {
        dis_mac_data_buf[0] = 'M';
        dis_mac_data_buf[1] = ':';
        dis_mac_data_buf[2] = mac_data[1] / 16;
        if(dis_mac_data_buf[2] >= 0x0A && dis_mac_data_buf[2] <= 0x0F)
        {
            dis_mac_data_buf[2] += 'A' - 10;
        }
        else if(dis_mac_data_buf[2] <= 0x09)
        {
            dis_mac_data_buf[2] += '0';
        }
        dis_mac_data_buf[3] = mac_data[1] % 16;
        if(dis_mac_data_buf[3] >= 0x0A && dis_mac_data_buf[3] <= 0x0F)
        {
            dis_mac_data_buf[3] += 'A' - 10;
        }
        else if(dis_mac_data_buf[3] <= 0x09)
        {
            dis_mac_data_buf[3] += '0';
        }
        dis_mac_data_buf[4] = ':';
        dis_mac_data_buf[5] = mac_data[0] / 16;
        if(dis_mac_data_buf[5] >= 0x0A && dis_mac_data_buf[5] <= 0x0F)
        {
            dis_mac_data_buf[5] += 'A' - 10;
        }
        else if(dis_mac_data_buf[5] <= 0x09)
        {
            dis_mac_data_buf[5] += '0';
        }
        dis_mac_data_buf[6] = mac_data[0] % 16;
        if(dis_mac_data_buf[6] >= 0x0A && dis_mac_data_buf[6] <= 0x0F)
        {
            dis_mac_data_buf[6] += 'A' - 10;
        }
        else if(dis_mac_data_buf[6] <= 0x09)
        {
            dis_mac_data_buf[6] += '0';
        }
        Font_DisContentText(dis_mac_data_buf, 7, 49, 1, 2, 0);
//        Font_DisplayContentText(dis_mac_data_buf, 7, 49, 1);
        
        dis_ver_data_buf[0] = 'V';
        dis_ver_data_buf[1] = ':';
        dis_ver_data_buf[2] = ver_data[0] / 16 + '0';
        dis_ver_data_buf[3] = ver_data[0] % 16 + '0';
        dis_ver_data_buf[4] = '.';
        dis_ver_data_buf[5] = ver_data[1] / 16 + '0';
        dis_ver_data_buf[6] = ver_data[1] % 16 + '0';
        Font_DisContentText(dis_ver_data_buf, 7, 95, 1, 2, 0);
//        Font_DisplayContentText(dis_ver_data_buf, 7, 95, 1);
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
void Gui_PageAlarm(uint8_t am_pm_format, uint8_t scene, uint8_t hour, uint8_t minute, uint8_t language, uint8_t index)
{
    uint8_t time_data[4] = {0};
    // ������Ҫ��������������
	if(scene == 0)
    {
        if(index == 0)
        {
            /* ��̬ͼ�� */
            LCD_SetPosition(icon_16_clock_contour_coord[0][0], icon_16_clock_contour_coord[0][1], 
                            ICON_CLOCK_CONTOUR_WIDE, ICON_CLOCK_CONTOUR_HIGH);
            LCD_DriverWriteDataFromExtFlash(icon_16_clock_contour_addr[0], ICON_CLOCK_CONTOUR_SIZE);
            
            /* AM,PM,����,���� */
            LCD_SetPosition(icon_16_clock_ampm_coord[0][0], icon_16_clock_ampm_coord[0][1],
                            ICON_CLOCK_ICON_WIDE, ICON_CLOCK_ICON_HIGH);
            if(am_pm_format == GUI_TIME_FORMAT_STANDARD)
            {
                Gui_PageClearBackground(icon_16_clock_ampm_coord[0][0], icon_16_clock_ampm_coord[0][1], 
                                        ICON_CLOCK_ICON_WIDE, ICON_CLOCK_ICON_HIGH);
            }
            else
            {
                if(language == 0)
                {
                    LCD_DriverWriteDataFromExtFlash(icon_16_clock_ampm_addr[am_pm_format - 1], ICON_CLOCK_ICON_SIZE);
                }
                else
                {
                    LCD_DriverWriteDataFromExtFlash(icon_16_clock_ampm_addr[am_pm_format + 1], ICON_CLOCK_ICON_SIZE);
                }
            }
            time_data[0] = hour / 10 % 3;
            time_data[1] = hour % 10;
            time_data[2] = minute / 10 % 6;
            time_data[3] = minute % 10;
            /* ʱ�� */
            for(uint8_t i = 0; i < 4; i++)
            {
                LCD_SetPosition(icon_16_clock_data_coord[i][0], icon_16_clock_data_coord[i][1],
                                ICON_CLOCK_NUMBER_WIDE, ICON_CLOCK_NUMBER_HIGH);
                LCD_DriverWriteDataFromExtFlash(icon_16_clock_number_addr[time_data[i]], ICON_CLOCK_NUMBER_SIZE);
            }
            /* ð�� */
            LCD_SetPosition(icon_16_clock_colon_coord[0][0], icon_16_clock_colon_coord[0][1], 
                            ICON_CLOCK_COLON_WIDE, ICON_CLOCK_COLON_HIGH);
            LCD_DriverWriteDataFromExtFlash(icon_16_clock_colon_addr[0], ICON_CLOCK_COLON_SIZE);
        }
        /* ��̬ͼ�� */
        LCD_SetPosition(icon_16_clock_pointer_coord[0][0], icon_16_clock_pointer_coord[0][1], 
                        ICON_CLOCK_POINTER_WIDE, ICON_CLOCK_POINTER_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_clock_pointer_addr[index % 7], ICON_CLOCK_POINTER_SIZE);
    }
	else
    {
        /* ����������� */
        if(index % 2 == 1)
        {
            Gui_PageClearBackground(icon_16_remindclock_icon_coord[0][0], icon_16_remindclock_icon_coord[0][1], 
                                    ICON_REMINDCLOCK_ICON_WIDE, icon_16_remindclock_icon_coord[1][1] - icon_16_remindclock_icon_coord[0][1]);
        }
        /* ��ʾͼ�� */
        LCD_SetPosition(icon_16_remindclock_icon_coord[index % 2][0], icon_16_remindclock_icon_coord[index % 2][1], 
                        ICON_REMINDCLOCK_ICON_WIDE, ICON_REMINDCLOCK_ICON_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_remindclock_icon_addr[(scene - 1) % 20], ICON_REMINDCLOCK_ICON_SIZE);

        /* ����ײ����� */
        if(index % 2 == 0)
        {
            Gui_PageClearBackground(icon_16_remindclock_icon_coord[0][0], icon_16_remindclock_icon_coord[0][1] + ICON_REMINDCLOCK_ICON_HIGH, 
                                    ICON_REMINDCLOCK_ICON_WIDE, icon_16_remindclock_icon_coord[1][1] - icon_16_remindclock_icon_coord[0][1]);
        }
    }		
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
void Gui_PageSit(uint8_t index)
{		
    /* ��ʾͼ�� */
    LCD_SetPosition(icon_16_sit_icon_coord[0][0], icon_16_sit_icon_coord[0][1],
                    ICON_SEDENTARY_ICON_WIDE, ICON_SEDENTARY_ICON_HIGH);
    LCD_DriverWriteDataFromExtFlash(icon_16_sit_icon_addr[index % 2], ICON_SEDENTARY_ICON_SIZE);
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
void Gui_PageLowPower(uint8_t index)
{
    /* ��ʾ��̬ͼ�� */
    LCD_SetPosition(icon_16_lowpower_contour_coord[0][0], icon_16_lowpower_contour_coord[0][1],
                    ICON_LOWELECTRICITY_CONTOUR_WIDE, ICON_LOWELECTRICITY_CONTOUR_HIGH);
    LCD_DriverWriteDataFromExtFlash(icon_16_lowpower_contour_addr[0], ICON_LOWELECTRICITY_CONTOUR_SIZE);
    
    /* ��ʾ��̬ͼ�� */
    if(index % 2 == 0)
    {
        LCD_SetPosition(icon_16_lowpower_lightning_coord[0][0], icon_16_lowpower_lightning_coord[0][1],
                        ICON_LOWELECTRICITY_LIGHTNING_WIDE, ICON_LOWELECTRICITY_LIGHTNING_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_lowpower_lightning_addr[0], ICON_LOWELECTRICITY_LIGHTNING_SIZE);
    }
    else
    {
        Gui_PageClearBackground(icon_16_lowpower_lightning_coord[0][0], icon_16_lowpower_lightning_coord[0][1],
                                ICON_LOWELECTRICITY_LIGHTNING_WIDE, ICON_LOWELECTRICITY_LIGHTNING_HIGH);
    }
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
void Gui_PageBatteryCharg(uint8_t index, uint8_t state)
{	
    if(index == 0)
    {
        /* ��ʾ��̬ͼ�� */
        LCD_SetPosition(icon_16_chg_contour_coord[0][0], icon_16_chg_contour_coord[0][1],
                        ICON_CHARGE_CONTOUR_WIDE, ICON_CHARGE_CONTOUR_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_chg_contour_addr[0], ICON_CHARGE_CONTOUR_SIZE);
    }
    
    if(state == 1)
    {
        index = 6;
    }
    
    /* ��ʾ��̬ͼ�� */
    LCD_SetPosition(icon_16_chg_bar_coord[0][0], icon_16_chg_bar_coord[0][1],
                    ICON_CHARGE_BAR_WIDE, ICON_CHARGE_BAR_HIGH);
    LCD_DriverWriteDataFromExtFlash(icon_16_chg_bar_addr[index % 7], ICON_CHARGE_BAR_SIZE);
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
void Gui_PageHeartAlert(uint8_t index)
{
    if(index == 0)
    {
        /* ��ʾ��̬ͼ�� */
        LCD_SetPosition(icon_16_heartalert_contour_coord[0][0], icon_16_heartalert_contour_coord[0][1],
                        ICON_ABNORMALITY_HEART_WIDE, ICON_ABNORMALITY_HEART_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_heartalert_contour_addr[0], ICON_ABNORMALITY_HEART_SIZE);
    }
    
    /* ��ʾ��̬ͼ�� */
    LCD_SetPosition(icon_16_heartalert_round_coord[0][0], icon_16_heartalert_round_coord[0][1],
                    ICON_ABNORMALITY_ROUND_WIDE, ICON_ABNORMALITY_ROUND_HIGH);
    LCD_DriverWriteDataFromExtFlash(icon_16_heartalert_round_addr[index % 5], ICON_ABNORMALITY_ROUND_SIZE);
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
void Gui_PageFontTest(void)
{
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
void Gui_PageCallNotificationIcon(uint8_t call_state, uint8_t index)
{
    static uint8_t call_state_bk = 0;   //�ϴν����ĵ绰״̬
    
    if(call_state_bk != call_state)
    {
        /* �������� */
        Gui_PageClearBackground(icon_16_call_arr_coord[0][0], icon_16_call_arr_coord[0][1],
                                ICON_CALL_ARC_WIDE, ICON_CALL_ARC_HIGH + 1);
        call_state_bk = call_state;
    }
    
    if(call_state == GUI_CALL_MUTE)
    {
        /* ���� */
        LCD_SetPosition(icon_16_call_mute_coord[0][0], icon_16_call_mute_coord[0][1],
                        ICON_CALL_MUTE_WIDE, ICON_CALL_MUTE_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_call_mute_addr[0], ICON_CALL_MUTE_SIZE);
    }
    else if(call_state == GUI_CALL_REJECTSUC)
    {
        /* �������� */
        Gui_PageClearBackground(6, 8, 2, 78);
        Gui_PageClearBackground(icon_16_call_hangup_coord[0][0], icon_16_call_hangup_coord[0][1] + ICON_CALL_HANGUP_HIGH, 
                                ICON_CALL_HANGUP_WIDE, 5);
        /* �ܽ� */
        LCD_SetPosition(icon_16_call_hangup_coord[0][0], icon_16_call_hangup_coord[0][1],
                        ICON_CALL_HANGUP_WIDE, ICON_CALL_HANGUP_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_call_hangup_addr[0], ICON_CALL_HANGUP_SIZE);
    }
    else
    {
        if(index == 0)
        {
            /* ���羲̬ͼ�� */
            LCD_SetPosition(icon_16_call_phone_coord[0][0], icon_16_call_phone_coord[0][1],
                            ICON_CALL_PHONE_WIDE, ICON_CALL_PHONE_HIGH);
            LCD_DriverWriteDataFromExtFlash(icon_16_call_phone_addr[0], ICON_CALL_PHONE_SIZE);
        }
        /* ���綯̬ͼ�� */
        LCD_SetPosition(icon_16_call_arr_coord[0][0], icon_16_call_arr_coord[0][1],
                        ICON_CALL_ARC_WIDE, ICON_CALL_ARC_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_call_arr_addr[index % 3], ICON_CALL_ARC_SIZE);
    }        
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
void Gui_PageCallNotificationContent(uint8_t *display_buf, uint16_t buf_len)
{
    Font_DisplayTitleText(display_buf, buf_len, icon_16_call_text_coord[0][1], 2);
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
void Gui_PageMessageNotificationIcon(uint8_t msg_type, uint8_t page_index)
{      
    if(msg_type <= MSG_NOTIFY_MAX_ID && msg_type > MSG_NOTIFY_NULL_ID)
    {
        /* ������ܻỨ���Ĳ��� */
        Gui_PageClearBackground(icon_16_inform_icon_coord[0][0], icon_16_inform_icon_coord[0][1], 
                                ICON_INFORM_ICON_WIDE, icon_16_inform_icon_coord[page_index % 2][1] - icon_16_inform_icon_coord[0][1]);
        /* ��ʾͼ�� */
        LCD_SetPosition(icon_16_inform_icon_coord[page_index % 2][0], icon_16_inform_icon_coord[page_index % 2][1],
                        ICON_INFORM_ICON_WIDE, ICON_INFORM_ICON_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_inform_icon_addr[msg_type - 1], ICON_INFORM_ICON_SIZE);
        
        Gui_PageClearBackground(icon_16_inform_icon_coord[0][0], icon_16_inform_icon_coord[0][1] + ICON_INFORM_ICON_HIGH, 
                                ICON_INFORM_ICON_WIDE, icon_16_inform_icon_coord[1][1] - icon_16_inform_icon_coord[page_index % 2][1]);
    } 
    else
    {
        return;
    }
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
uint8_t Gui_PageMessageNotificationContent(uint8_t *display_buf, uint16_t buf_len, uint8_t page_index)
{          
    uint8_t read_len = 0;
    if(page_index <= 1)
    {
        read_len = Font_DisplayContentText(display_buf, buf_len, 72, FIRST_PAGE_ROWS);
    }
    else
    {
        read_len = Font_DisplayContentText(display_buf, buf_len, 2, PER_PAGE_MAX_ROWS);
    }
    return read_len;
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
void Gui_PageStarget(uint8_t index, uint32_t target_step, uint8_t language)
{
    uint8_t start_column = 0;
    uint8_t start_page = 0;
    uint32_t step_cnt = 0;
    
    /* ͼ����ʾ */
    if(index % 2 == 1)
    {       
        Gui_PageClearBackground(5, 6, 15, 15);
        Gui_PageClearBackground(0, 60, 18, 18);
        Gui_PageClearBackground(41, 0, 17, 17);
        Gui_PageClearBackground(68, 17, 11, 11);
        Gui_PageClearBackground(60, 57, 13, 13);
//        LCD_SetPosition(icon_16_target_icon_coord[1][0], icon_16_target_icon_coord[1][1],
//                        ICON_GOAL_2_WIDE, ICON_GOAL_2_HIGH);
//        LCD_DriverWriteDataFromExtFlash(icon_16_target_icon_addr[1], ICON_GOAL_2_SIZE);
    }
    else
    {
        LCD_SetPosition(icon_16_target_icon_coord[0][0], icon_16_target_icon_coord[0][1],
                        ICON_GOAL_1_WIDE, ICON_GOAL_1_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_target_icon_addr[0], ICON_GOAL_1_SIZE); 
    }
    
    /* ��沽�� */  
    step_cnt = target_step;
    if(step_cnt > 99999)
    {
        step_cnt = 99999;
    }
     /* ��沽�� */  
    if(step_cnt >= 1000)
    {
        
        start_page = icon_16_step_data_coord[0][1];        
        start_column = 38;//24;
        if(step_cnt >= 10000)
        {
            start_column = 15;
            LCD_SetPosition(start_column, start_page, ICON_FUNCTION_DATA_WIDE, ICON_FUNCTION_DATA_HIGH);
            LCD_DriverWriteDataFromExtFlash(icon_16_function_digital_addr[(step_cnt / 10000) % 10], ICON_FUNCTION_DATA_SIZE);
            start_column += ICON_FUNCTION_DATA_WIDE;
            Gui_PageClearBackground(start_column, start_page, 3, ICON_FUNCTION_DATA_HIGH);
            start_column += 3;
        }
        else
        {
            Gui_PageClearBackground(15, start_page, 38 - 15, ICON_FUNCTION_DATA_HIGH);
        }
        
        LCD_SetPosition(start_column, start_page, ICON_FUNCTION_DATA_WIDE, ICON_FUNCTION_DATA_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_function_digital_addr[(step_cnt / 1000) % 10], ICON_FUNCTION_DATA_SIZE);
        start_column += ICON_FUNCTION_DATA_WIDE;
        Gui_PageClearBackground(start_column, start_page, 3, ICON_FUNCTION_DATA_HIGH);
        start_column += 3;
        
        //ǧλ�ָ���
        LCD_SetPosition(icon_16_step_data_coord[1][0], icon_16_step_data_coord[1][1], ICON_FUNCTION_SEPARATOR_WIDE, ICON_FUNCTION_SEPARATOR_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_function_digital_addr[11], ICON_FUNCTION_SEPARATOR_SIZE);      
    }
    
    start_page = icon_16_step_data_coord[2][1];
    if(step_cnt >= 100)
    {     
        start_column = 10;        
        LCD_SetPosition(start_column, start_page, ICON_FUNCTION_DATA_WIDE, ICON_FUNCTION_DATA_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_function_digital_addr[(step_cnt / 100) % 10], ICON_FUNCTION_DATA_SIZE);
        start_column += ICON_FUNCTION_DATA_WIDE;
        Gui_PageClearBackground(start_column, start_page, 2, ICON_FUNCTION_DATA_HIGH);
        start_column += 2;
    }
    else
    {
        Gui_PageClearBackground(0, icon_16_step_data_coord[0][1], LCD_MAX_COLUMN, ICON_FUNCTION_DATA_HIGH);
        Gui_PageClearBackground(0, start_page, 18, ICON_FUNCTION_DATA_HIGH);
        start_column = 18; 
    }
    if(step_cnt >= 10)
    {            
        LCD_SetPosition(start_column, start_page, ICON_FUNCTION_DATA_WIDE, ICON_FUNCTION_DATA_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_function_digital_addr[(step_cnt / 10) % 10], ICON_FUNCTION_DATA_SIZE);
        start_column += ICON_FUNCTION_DATA_WIDE;
        Gui_PageClearBackground(start_column, start_page, 2, ICON_FUNCTION_DATA_HIGH);
        start_column += 2;
    }
    else
    {
        Gui_PageClearBackground(0, icon_16_step_data_coord[0][1], LCD_MAX_COLUMN, ICON_FUNCTION_DATA_HIGH);
        Gui_PageClearBackground(0, start_page, 29, ICON_FUNCTION_DATA_HIGH);
        start_column = 29;   
    }
    LCD_SetPosition(start_column, start_page, ICON_FUNCTION_DATA_WIDE, ICON_FUNCTION_DATA_HIGH);
    LCD_DriverWriteDataFromExtFlash(icon_16_function_digital_addr[step_cnt % 10], ICON_FUNCTION_DATA_SIZE);
    start_column += ICON_FUNCTION_DATA_WIDE;
    Gui_PageClearBackground(start_column, start_page, LCD_MAX_COLUMN - start_column, ICON_FUNCTION_DATA_HIGH); 
}



/****************************************************************
** Function name:           Gui_PageCsr
** Descriptions:            3.0�����󶨽���
** input parameters:        language: ���ԣ�0���ġ�1Ӣ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-08-11
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Gui_PageCsr(uint8_t csr_state, uint8_t index)
{	
  
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
void Gui_PageRunMode(uint8_t index, uint8_t language)
{	
    //�˶�ģʽ������
    //��
    if(index == 0)
    {
        LCD_SetPosition(icon_16_move1_run_man_coord[0][0], icon_16_move1_run_man_coord[0][1],
                        ICON_MOVE1_ICON1_WIDE, ICON_MOVE1_ICON1_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_move1_run_man_addr[0], ICON_MOVE1_ICON1_SIZE);
    }
    //������
    Gui_PageClearBackground(icon_16_move1_run_line_coord[0][0], icon_16_move1_run_line_coord[0][1],
                           icon_16_move1_run_line_coord[index % 3][0] - icon_16_move1_run_line_coord[0][0],
                           ICON_MOVE1_ICON2_HIGH);
    //��̬
    LCD_SetPosition(icon_16_move1_run_line_coord[index % 3][0], icon_16_move1_run_line_coord[index % 3][1],
                    ICON_MOVE1_ICON2_WIDE, ICON_MOVE1_ICON2_HIGH);
    LCD_DriverWriteDataFromExtFlash(icon_16_move1_run_line_addr[0], ICON_MOVE1_ICON2_SIZE);
    //����ұ�
    Gui_PageClearBackground(icon_16_move1_run_line_coord[index % 3][0] + ICON_MOVE1_ICON2_WIDE, icon_16_move1_run_line_coord[index % 3][1],
                           icon_16_move1_run_line_coord[2][0] - icon_16_move1_run_line_coord[index % 3][0],
                           ICON_MOVE1_ICON2_HIGH);
    //s˵��   
    LCD_SetPosition(icon_16_move1_run_text_coord[0][0], icon_16_move1_run_text_coord[0][1], 
                    ICON_MOVE1_TEXT_WIDE, ICON_MOVE1_TEXT_HIGH);
    LCD_DriverWriteDataFromExtFlash(icon_16_move1_run_text_addr[language % 2], ICON_MOVE1_TEXT_SIZE);
}

/****************************************************************
** Function name:           Gui_PageRunStaticIcon
** Descriptions:            ��ʾ�˶�ģʽ�ľ�̬ͼ��
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
void Gui_PageRunStaticIcon(void)
{
    for(uint8_t i = 0; i < 2; i++)
    {
        /* ʱ���ð�� */
        LCD_SetPosition(icon_16_move2_timing_colon_coord[i][0], icon_16_move2_timing_colon_coord[i][1], 
                        ICON_MOVE_TIME1_WIDE, ICON_MOVE_TIME1_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_move2_timing_colon_addr[0], ICON_MOVE_TIME1_SIZE);  
    }
         
    for(uint8_t i = 0; i < 4; i++)
    {
        /* �ָ��� */
        LCD_SetPosition(icon_16_move2_line_coord[i][0], icon_16_move2_line_coord[i][1], 
                        ICON_MOVE_ICON5_WIDE, ICON_MOVE_ICON5_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_move2_line_addr[0], ICON_MOVE_ICON5_SIZE);
    }
    /* ����ͼ�� */
    LCD_SetPosition(icon_16_move2_heart_icon_coord[0][0], icon_16_move2_heart_icon_coord[0][1], 
                    ICON_MOVE_ICON3_WIDE, ICON_MOVE_ICON3_HIGH);
    LCD_DriverWriteDataFromExtFlash(icon_16_move2_heart_icon_addr[0], ICON_MOVE_ICON3_SIZE);
    /* ����ͼ�� */
    LCD_SetPosition(icon_16_move2_distance_icon_coord[0][0], icon_16_move2_distance_icon_coord[0][1],
                    ICON_MOVE_ICON2_WIDE, ICON_MOVE_ICON2_HIGH);
    LCD_DriverWriteDataFromExtFlash(icon_16_move2_distance_icon_addr[0], ICON_MOVE_ICON2_SIZE);
    /* ��·��ͼ�� */
    LCD_SetPosition(icon_16_move2_calories_icon_coord[0][0], icon_16_move2_calories_icon_coord[0][1],
                    ICON_MOVE_ICON1_WIDE, ICON_MOVE_ICON1_HIGH);
    LCD_DriverWriteDataFromExtFlash(icon_16_move2_calories_icon_addr[0], ICON_MOVE_ICON1_SIZE);
    /* �Ʋ�ͼ�� */
    LCD_SetPosition(icon_16_move2_step_icon_coord[0][0], icon_16_move2_step_icon_coord[0][1],
                    ICON_MOVE_ICON4_WIDE, ICON_MOVE_ICON4_HIGH);
    LCD_DriverWriteDataFromExtFlash(icon_16_move2_step_icon_addr[0], ICON_MOVE_ICON4_SIZE); 
}
/****************************************************************
** Function name:           Gui_UpdateRunModeTime
** Descriptions:            ��ʾ�˶�ģʽ���ݽ����ʱ��
** input parameters:        seconds���˶�ʱ��
** output parameters:       
** Returned value:          
** Created by:              WJ              
** Created Date:            2017-08-11
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
static void Gui_UpdateRunModeTime(uint32_t seconds)    
{
    uint8_t hour = 0;
    uint8_t minute = 0;
    uint8_t second = 0;
    uint8_t timing_data[6] = {0};
    
    hour   = seconds / 60 / 60 % 10;
    minute = (seconds / 60) % 60;
    second = seconds % 60;
    timing_data[0] = (hour % 100) / 10;
    timing_data[1] = hour % 10;
    timing_data[2] = (minute % 100) / 10;
    timing_data[3] = minute % 10;
    timing_data[4] = (second % 100) / 10;
    timing_data[5] = second % 10;
    
    for(uint8_t i = 0; i < 6; i++)
    {
        /* ��ʾ������������ */
        LCD_SetPosition(icon_16_move2_timing_coord[i][0], icon_16_move2_timing_coord[i][1], 
                        ICON_MOVE_TIME_WIDE, ICON_MOVE_TIME_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_move2_timing_addr[timing_data[i]], ICON_MOVE_TIME_SIZE);
    }
}

/****************************************************************
** Function name:           Gui_UpdateRunModeHeart
** Descriptions:            ��ʾ�˶�ģʽ���ݽ��������
** input parameters:        heart������
** output parameters:       
** Returned value:          
** Created by:              WJ              
** Created Date:            2017-08-11
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
static void Gui_UpdateRunModeHeart(uint8_t heart, uint8_t language)    
{
    uint8_t stop_xcood = 0;
    uint8_t start_column = 0;
    uint8_t start_page = 0;
    uint8_t heart_tmp = 0;
    
    heart_tmp = heart;
    start_page = icon_16_move2_data_coord[0][1];
    start_column = 2;

    if(heart_tmp == 0)
    {
        
        Gui_PageClearBackground(21, start_page, 1, ICON_MOVE_DATE1_HIGH);
        
        start_column = 22;
        LCD_SetPosition(start_column, icon_16_move2_heart_nodata_coord[0][1], 
                        ICON_MOVE_DATA2_WIDE, ICON_MOVE_DATA2_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_move2_heart_nodata_addr[0], ICON_MOVE_DATA2_SIZE);
        
        Gui_PageClearBackground(start_column, start_page, ICON_MOVE_DATA2_WIDE, 6);
        Gui_PageClearBackground(start_column, icon_16_move2_heart_nodata_coord[0][1] + ICON_MOVE_DATA2_HIGH, ICON_MOVE_DATA2_WIDE, 6);
        start_column += ICON_MOVE_DATA2_WIDE;
        Gui_PageClearBackground(start_column, start_page, 2, ICON_MOVE_DATE1_HIGH + 2);
        start_column += 2;   

        
    }
    else    
    {
        start_column = 21;
        if(heart_tmp > 99)
        {
            LCD_SetPosition(start_column, start_page, ICON_MOVE_DATE1_WIDE, ICON_MOVE_DATE1_HIGH);
            LCD_DriverWriteDataFromExtFlash(icon_16_move2_data_addr[(heart_tmp % 1000) / 100], ICON_MOVE_DATE1_SIZE);
            Gui_PageClearBackground(start_column, start_page + ICON_MOVE_DATE1_HIGH, ICON_MOVE_DATE1_WIDE, 2);
            start_column += ICON_MOVE_DATE1_WIDE;
            Gui_PageClearBackground(start_column, start_page, 1, ICON_MOVE_DATE1_HIGH + 2);
            start_column += 1; 
        }
        
        if(heart_tmp > 9)
        {
            LCD_SetPosition(start_column, start_page, ICON_MOVE_DATE1_WIDE, ICON_MOVE_DATE1_HIGH);
            LCD_DriverWriteDataFromExtFlash(icon_16_move2_data_addr[(heart_tmp % 100) / 10], ICON_MOVE_DATE1_SIZE);
            Gui_PageClearBackground(start_column, start_page + ICON_MOVE_DATE1_HIGH, ICON_MOVE_DATE1_WIDE, 2);
            start_column += ICON_MOVE_DATE1_WIDE;
            Gui_PageClearBackground(start_column, start_page, 1, ICON_MOVE_DATE1_HIGH + 2);
            start_column += 1;
        }  
        LCD_SetPosition(start_column, start_page, 
                        ICON_MOVE_DATE1_WIDE, ICON_MOVE_DATE1_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_move2_data_addr[heart_tmp % 10], ICON_MOVE_DATE1_SIZE);
        Gui_PageClearBackground(start_column, start_page + ICON_MOVE_DATE1_HIGH, ICON_MOVE_DATE1_WIDE, 2);        
        start_column += ICON_MOVE_DATE1_WIDE;
        Gui_PageClearBackground(start_column, start_page, 2, ICON_MOVE_DATE1_HIGH + 2);
        start_column += 2;
    }
    LCD_SetPosition(start_column, icon_16_move2_heart_unit_coord[0][1],
                    ICON_MOVE_UNIT1_WIDE, ICON_MOVE_UNIT1_HIGH);
    if(language == 0)
    {
        LCD_DriverWriteDataFromExtFlash(icon_16_move2_heart_unit_addr[0], ICON_MOVE_UNIT1_SIZE);
    }
    else
    {       
        LCD_DriverWriteDataFromExtFlash(icon_16_move2_heart_unit_addr[1], ICON_MOVE_UNIT1_SIZE);
    }
    Gui_PageClearBackground(start_column, start_page, ICON_MOVE_UNIT1_WIDE, 2);
    stop_xcood = start_column + ICON_MOVE_UNIT1_WIDE;
    Gui_PageClearBackground(stop_xcood, start_page, LCD_MAX_COLUMN - stop_xcood, ICON_MOVE_UNIT1_HIGH);
}

/****************************************************************
** Function name:           Gui_UpdateRunModeDistance
** Descriptions:            ��ʾ�˶�ģʽ���ݽ���ľ���
** input parameters:        distance������
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
static void Gui_UpdateRunModeDistance(float distance, uint8_t uint_type, uint8_t language)    
{
    uint8_t  stop_xcood = 0;
    uint8_t  start_column = 0;
    uint8_t  start_page = 0;  
    uint16_t distance_temp = 0;
    
    start_column = 2;
    start_page = icon_16_move2_data_coord[1][1];
    
    if(distance >=100)
    {
        distance = 99.9f;
    }

    distance_temp = distance * 10;
    
    start_column = 21;
    if(distance_temp > 99)
    {
        LCD_SetPosition(start_column, start_page, 
                       ICON_MOVE_DATE1_WIDE, ICON_MOVE_DATE1_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_move2_data_addr[(distance_temp % 1000) / 100], ICON_MOVE_DATE1_SIZE);  
        start_column += ICON_MOVE_DATE1_WIDE;
        Gui_PageClearBackground(start_column, start_page, 1, ICON_MOVE_DATE1_HIGH);
        start_column += 1;
    }
    
    if(distance_temp > 9)
    {
        LCD_SetPosition(start_column, start_page, 
                       ICON_MOVE_DATE1_WIDE, ICON_MOVE_DATE1_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_move2_data_addr[(distance_temp % 100) / 10], ICON_MOVE_DATE1_SIZE);  
        start_column += ICON_MOVE_DATE1_WIDE;
        Gui_PageClearBackground(start_column, start_page, 1, ICON_MOVE_DATE1_HIGH);
        start_column += 1;
    }  
    if(distance < 1)
    {
        LCD_SetPosition(start_column, start_page, 
                        ICON_MOVE_DATE1_WIDE, ICON_MOVE_DATE1_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_move2_data_addr[0], ICON_MOVE_DATE1_SIZE);
        start_column += ICON_MOVE_DATE1_WIDE;
        Gui_PageClearBackground(start_column, start_page, 2, ICON_MOVE_DATE1_HIGH);
        start_column += 2;        
    }
    //��ʾС����
    if(distance < 100)
    {
        Gui_PageClearBackground(start_column, start_page, 1, ICON_MOVE_DATE1_HIGH);
        start_column += 1;
        LCD_SetPosition(start_column, icon_16_move2_data_point_coord[0][1], 
                       ICON_MOVE_DATE3_WIDE, ICON_MOVE_DATE3_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_move2_data_point_addr[0], ICON_MOVE_DATE3_SIZE); 
        Gui_PageClearBackground(start_column, start_page, ICON_MOVE_DATE3_WIDE, ICON_MOVE_DATE1_HIGH - ICON_MOVE_DATE3_HIGH);
        start_column += ICON_MOVE_DATE3_WIDE;
        Gui_PageClearBackground(start_column, start_page, 2, ICON_MOVE_DATE1_HIGH);
        start_column += 2;        
    }
    
    LCD_SetPosition(start_column, start_page, 
                    ICON_MOVE_DATE1_WIDE, ICON_MOVE_DATE1_HIGH);
    LCD_DriverWriteDataFromExtFlash(icon_16_move2_data_addr[distance_temp % 10], ICON_MOVE_DATE1_SIZE);
    start_column += ICON_MOVE_DATE1_WIDE;
    Gui_PageClearBackground(start_column, start_page, 2, ICON_MOVE_DATE1_HIGH);
    start_column += 2;
    
    //��λ   
    LCD_SetPosition(start_column, icon_16_move2_distance_unit_coord[0][1],
                    ICON_MOVE_UINT_WIDE, ICON_MOVE_UINT_HIGH);
    if(language == 0)
    {
		// JSH.20181102.�����£�����ʾӢ��
		if(uint_type == 0)
		{
			LCD_DriverWriteDataFromExtFlash(icon_16_move2_distance_unit_addr[0], ICON_MOVE_UINT_SIZE); 
		}
		else
		{
			LCD_DriverWriteDataFromExtFlash(icon_16_move2_distance_unit_addr[3], ICON_MOVE_UINT_SIZE);
		}
    }
    else
    {
        if(uint_type == 0)
        {
            LCD_DriverWriteDataFromExtFlash(icon_16_move2_distance_unit_addr[1], ICON_MOVE_UINT_SIZE);
        }   
        else
        {
            LCD_DriverWriteDataFromExtFlash(icon_16_move2_distance_unit_addr[2], ICON_MOVE_UINT_SIZE);
        }
    }
    Gui_PageClearBackground(start_column, start_page, ICON_MOVE_UINT_WIDE, 1);
    stop_xcood = start_column + ICON_MOVE_UINT_WIDE;
    Gui_PageClearBackground(stop_xcood, start_page, LCD_MAX_COLUMN - stop_xcood, ICON_MOVE_DATE1_HIGH);
       
}

/****************************************************************
** Function name:           Gui_UpdateRunModeCalories
** Descriptions:            ��ʾ�˶�ģʽ���ݽ���Ŀ�·��
** input parameters:        calories����·��
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
static void Gui_UpdateRunModeCalories(float calories, uint8_t language)    
{
    uint8_t  start_column = 0;
    uint8_t  start_page = 0;  
    uint16_t calories_temp = 0;
    
    start_column = 2;
    start_page = icon_16_move2_data_coord[2][1];

    calories_temp = calories;
    if(calories_temp > 9999)
    {
        calories_temp = 9999;
    }
    else
    {
        calories_temp = calories;
    }
            
    start_column = 21;
    if(calories_temp > 999)
    {
        LCD_SetPosition(start_column, start_page, ICON_MOVE_DATE1_WIDE, ICON_MOVE_DATE1_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_move2_data_addr[(calories_temp % 10000) / 1000], ICON_MOVE_DATE1_SIZE);  
        start_column += ICON_MOVE_DATE1_WIDE;
        Gui_PageClearBackground(start_column, start_page, 1, ICON_MOVE_DATE1_HIGH);
        start_column += 1;
    }
    
    if(calories_temp > 99)
    {
        LCD_SetPosition(start_column, start_page, ICON_MOVE_DATE1_WIDE, ICON_MOVE_DATE1_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_move2_data_addr[(calories_temp % 1000) / 100], ICON_MOVE_DATE1_SIZE);  
        start_column += ICON_MOVE_DATE1_WIDE;
        Gui_PageClearBackground(start_column, start_page, 1, ICON_MOVE_DATE1_HIGH);
        start_column += 1;
    }   
    if(calories_temp > 9)
    {
        LCD_SetPosition(start_column, start_page, ICON_MOVE_DATE1_WIDE, ICON_MOVE_DATE1_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_move2_data_addr[(calories_temp % 100) / 10], ICON_MOVE_DATE1_SIZE);  
        start_column += ICON_MOVE_DATE1_WIDE;
        Gui_PageClearBackground(start_column, start_page, 1, ICON_MOVE_DATE1_HIGH);
        start_column += 1;
    }     
    LCD_SetPosition(start_column, start_page, ICON_MOVE_DATE1_WIDE, ICON_MOVE_DATE1_HIGH);
    LCD_DriverWriteDataFromExtFlash(icon_16_move2_data_addr[calories_temp % 10], ICON_MOVE_DATE1_SIZE);
    start_column += ICON_MOVE_DATE1_WIDE;
    Gui_PageClearBackground(start_column, start_page, 2, ICON_MOVE_DATE1_HIGH);
    start_column += 2;

    //��λ ,��λ��ʱ����ʾ��λ   
    if(calories_temp < 1000)
    {   
        LCD_SetPosition(start_column, icon_16_move2_calories_unit_coord[0][1], 
                        ICON_MOVE_UNIT2_WIDE, ICON_MOVE_UNIT2_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_move2_calories_unit_addr[language % 2], ICON_MOVE_UNIT2_SIZE);
        Gui_PageClearBackground(start_column, icon_16_move2_calories_unit_coord[0][1] + ICON_MOVE_UNIT2_HIGH, ICON_MOVE_UNIT2_WIDE, 1);
        start_column = start_column + ICON_MOVE_UNIT2_WIDE;
    }
    if(start_column < LCD_MAX_COLUMN)
    {
        Gui_PageClearBackground(start_column, start_page, LCD_MAX_COLUMN - start_column, ICON_MOVE_DATE1_HIGH);
    }
}

/****************************************************************
** Function name:           Gui_UpdateRunModeStep
** Descriptions:            ��ʾ�˶�ģʽ���ݽ���ļƲ�
** input parameters:        step���Ʋ���
** output parameters:       
** Returned value:          
** Created by:              WJ              
** Created Date:            2017-08-11
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
static void Gui_UpdateRunModeStep(uint32_t step)    
{
    uint8_t  stop_xcood = 0;
    uint8_t  start_column = 0;
    uint8_t  start_page = 0;  
    uint32_t step_temp = 0;
    
    start_column = 2;
    start_page = icon_16_move2_data_coord[3][1];
    step_temp = step;
    if(step_temp > 99999)
    {
        step_temp = 99999;
    }
    
    start_column = 21;
    if(step_temp > 9999)
    {
        LCD_SetPosition(start_column, start_page, ICON_MOVE_DATE1_WIDE, ICON_MOVE_DATE1_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_move2_data_addr[(step_temp % 100000) / 10000], ICON_MOVE_DATE1_SIZE);  
        start_column += ICON_MOVE_DATE1_WIDE;
        Gui_PageClearBackground(start_column, start_page, 1, ICON_MOVE_DATE1_HIGH);
        start_column += 1;
    }
    if(step_temp > 999)
    {
        LCD_SetPosition(start_column, start_page, ICON_MOVE_DATE1_WIDE, ICON_MOVE_DATE1_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_move2_data_addr[(step_temp % 10000) / 1000], ICON_MOVE_DATE1_SIZE);  
        start_column += ICON_MOVE_DATE1_WIDE;
        Gui_PageClearBackground(start_column, start_page, 1, ICON_MOVE_DATE1_HIGH);
        start_column += 1;
    }    
    if(step_temp > 99)
    {
        LCD_SetPosition(start_column, start_page, ICON_MOVE_DATE1_WIDE, ICON_MOVE_DATE1_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_move2_data_addr[(step_temp % 1000) / 100], ICON_MOVE_DATE1_SIZE);  
        start_column += ICON_MOVE_DATE1_WIDE;
        Gui_PageClearBackground(start_column, start_page, 1, ICON_MOVE_DATE1_HIGH);
        start_column += 1;
    }   
    if(step_temp > 9)
    {
        LCD_SetPosition(start_column, start_page, ICON_MOVE_DATE1_WIDE, ICON_MOVE_DATE1_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_move2_data_addr[(step_temp % 100) / 10], ICON_MOVE_DATE1_SIZE);  
        start_column += ICON_MOVE_DATE1_WIDE;
        Gui_PageClearBackground(start_column, start_page, 1, ICON_MOVE_DATE1_HIGH);
        start_column += 1;
    }     
    LCD_SetPosition(start_column, start_page, ICON_MOVE_DATE1_WIDE, ICON_MOVE_DATE1_HIGH);
    LCD_DriverWriteDataFromExtFlash(icon_16_move2_data_addr[step_temp % 10], ICON_MOVE_DATE1_SIZE);
    stop_xcood = start_column + ICON_MOVE_DATE1_WIDE;
    Gui_PageClearBackground(stop_xcood, start_page, LCD_MAX_COLUMN - stop_xcood, ICON_MOVE_DATE1_HIGH);
  
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
void Gui_PageRunModeData(uint32_t seconds, uint8_t heart, float distance, float calories, 
                         uint32_t step, uint8_t uint_type, uint8_t language)
{	

    //��̬ͼ��
    Gui_PageRunStaticIcon();
	// ʱ��
    Gui_UpdateRunModeTime(seconds);
	//����
    Gui_UpdateRunModeHeart(heart, language);	    
    //����
    Gui_UpdateRunModeDistance(distance, uint_type, language);   
    //��·��
    Gui_UpdateRunModeCalories(calories, language);  
    //�Ʋ�
    Gui_UpdateRunModeStep(step); 
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
void Gui_PageRunModePause(uint8_t pause_state, uint8_t language)
{	
	// ��ͣ��ʼ 
    if(pause_state)
    {        
        LCD_SetPosition(icon_16_move4_pause_icon_coord[0][0], icon_16_move4_pause_icon_coord[0][1], 
                        ICON_MOVE3_ICON_WIDE, ICON_MOVE3_ICON_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_move4_pause_icon_addr[0], ICON_MOVE3_ICON_SIZE);
        LCD_SetPosition(icon_16_move4_pause_text_coord[0][0], icon_16_move4_pause_text_coord[0][1], 
                        ICON_MOVE3_TEXT_WIDE, ICON_MOVE3_TEXT_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_move4_pause_text_addr[language % 2], ICON_MOVE3_TEXT_SIZE);
    }
    else        
    {
        LCD_SetPosition(icon_16_move3_goon_icon_coord[0][0], icon_16_move3_goon_icon_coord[0][1],  
                        ICON_MOVE2_ICON_WIDE, ICON_MOVE2_ICON_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_move3_goon_icon_addr[0], ICON_MOVE2_ICON_SIZE);
        LCD_SetPosition(icon_16_move3_goon_text_coord[0][0], icon_16_move3_goon_text_coord[0][1],  
                        ICON_MOVE2_TEXT_WIDE, ICON_MOVE2_TEXT_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_move3_goon_text_addr[language % 2], ICON_MOVE2_TEXT_SIZE);
    }  
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
void Gui_PageRunModeExit(uint8_t language)
{	
	// �˳�
    LCD_SetPosition(icon_16_move5_exit_icon_coord[0][0], icon_16_move5_exit_icon_coord[0][1], 
                    ICON_MOVE4_ICON_WIDE, ICON_MOVE4_ICON_HIGH);
    LCD_DriverWriteDataFromExtFlash(icon_16_move5_exit_icon_addr[0], ICON_MOVE4_ICON_SIZE);
    LCD_SetPosition(icon_16_move5_exit_text_coord[0][0], icon_16_move5_exit_text_coord[0][1], 
                    ICON_MOVE4_TEXT_WIDE, ICON_MOVE4_TEXT_HIGH);
    LCD_DriverWriteDataFromExtFlash(icon_16_move5_exit_text_addr[language % 2], ICON_MOVE4_TEXT_SIZE);    
}

/****************************************************************
** Function name:            Gui_OxygenCalibration
** Descriptions:             У׼������ʾ
** input parameters:         progress:����
** output parameters:        ��
** Returned value:           ��
** Created by:               WJC
** Created Date:             2018-01-08
****************************************************************/
void Gui_OxygenCalibration(uint8_t index, uint8_t progress, uint8_t language)
{
    uint8_t i = 0;
    uint8_t start_column = 0;
//    uint8_t start_page = 0;
	uint8_t dis_progress = 0;
//	uint8_t progress_tmp = 0;

    if((index & 0x01) == 0)
    {
        // Ѫ��ͼ��
        LCD_SetPosition(icon_16_spO2_icon1_coord[0][0], icon_16_spO2_icon1_coord[0][1], 
                        ICON_SP02_ICON1_WIDE, ICON_SP02_ICON1_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_spO2_icon1_addr[index / 2 % 2], ICON_SP02_ICON1_SIZE);
    }
    
    //sp02��ʾ��
    LCD_SetPosition(icon_16_spO2_unit1_coord[0][0], icon_16_spO2_unit1_coord[0][1], 
					ICON_SP02_UNIT_WIDE, ICON_SP02_UNIT_HIGH);
	LCD_DriverWriteDataFromExtFlash(icon_16_spO2_unit1_addr[0], ICON_SP02_UNIT_SIZE);    
    
    if(progress >= 100)
    {
        dis_progress = 10;
    }
    else
    {
        dis_progress = index % 11;
    }
    if(dis_progress == 0)
	{
		//�������ı���
		LCD_SetPosition(icon_16_spO2_icon4_coord[0][0], icon_16_spO2_icon4_coord[0][1], 
						ICON_SP02_ICON4_WIDE, ICON_SP02_ICON4_HIGH);
		LCD_DriverWriteDataFromExtFlash(icon_16_spO2_icon4_addr[0], ICON_SP02_ICON4_SIZE);
	}
    
    start_column = icon_16_spO2_icon3_coord[0][0];
    for(i = 0; i <= dis_progress; i++)
    {
        if(i > 0)
        {
            LCD_SetPosition(start_column, icon_16_spO2_icon3_coord[0][1], 
							ICON_SP02_ICON3_WIDE, ICON_SP02_ICON3_HIGH);
			LCD_DriverWriteDataFromExtFlash(icon_16_spO2_icon3_addr[0], ICON_SP02_ICON3_SIZE);
            start_column += ICON_SP02_ICON3_WIDE + 3;
        }
    } 

//	progress_tmp = progress;
//	if(progress_tmp > 100)
//	{
//		progress_tmp = 100;
//	}
//    
//	start_page = icon_16_spO2_data1_coord[0][1];
//    if(progress_tmp >= 100)
//    {
//        start_column = icon_16_spO2_data1_coord[0][0];
//       
//        Gui_PageClearBackground(0, start_page, start_column, ICON_SP02_NUM_HIGH);
//        
//        // ����ֵ
//        LCD_SetPosition(start_column, start_page, ICON_SP02_NUM_WIDE, ICON_SP02_NUM_HIGH);
//        LCD_DriverWriteDataFromExtFlash(icon_16_spO2_data_addr[(progress_tmp % 1000) / 100], ICON_SP02_NUM_SIZE);
//        
//        start_column += ICON_SP02_NUM_WIDE;
//        Gui_PageClearBackground(start_column, start_page, 1, ICON_FUNCTION_DATA_HIGH);
//        start_column += 1;        
//        // ����ֵ
//        LCD_SetPosition(start_column, start_page, ICON_SP02_NUM_WIDE, ICON_SP02_NUM_HIGH);
//        LCD_DriverWriteDataFromExtFlash(icon_16_spO2_data_addr[(progress_tmp % 100) / 10], ICON_SP02_NUM_SIZE);
//        
//        start_column += ICON_SP02_NUM_WIDE;
//        Gui_PageClearBackground(start_column, start_page, 1, ICON_FUNCTION_DATA_HIGH);
//        start_column += 1;
//        // ����ֵ
//        LCD_SetPosition(start_column, start_page, ICON_SP02_NUM_WIDE, ICON_SP02_NUM_HIGH);
//        LCD_DriverWriteDataFromExtFlash(icon_16_spO2_data_addr[progress_tmp % 10], ICON_SP02_NUM_SIZE);
//        
//        start_column += ICON_SP02_NUM_WIDE;
//        Gui_PageClearBackground(start_column, start_page, icon_16_spO2_icon5_coord[0][0] - start_column, ICON_FUNCTION_DATA_HIGH);
//        // %����
//        LCD_SetPosition(icon_16_spO2_icon5_coord[0][0], icon_16_spO2_icon5_coord[0][1], 
//					    ICON_SP02_ICON5_WIDE, ICON_SP02_ICON5_HIGH);
//        LCD_DriverWriteDataFromExtFlash(icon_16_spO2_icon5_addr[0], ICON_SP02_ICON5_SIZE);
//        
//        start_column = icon_16_spO2_icon5_coord[0][0] + ICON_SP02_ICON5_WIDE;
//        Gui_PageClearBackground(start_column, start_page, 80 - start_column, ICON_FUNCTION_DATA_HIGH);
//    }
//    else if(progress_tmp >= 10)
//    {
//        start_column = 18;
//       
//        Gui_PageClearBackground(0, start_page, start_column, ICON_SP02_NUM_HIGH);
//                
//        // ����ֵ
//        LCD_SetPosition(start_column, start_page, ICON_SP02_NUM_WIDE, ICON_SP02_NUM_HIGH);
//        LCD_DriverWriteDataFromExtFlash(icon_16_spO2_data_addr[(progress_tmp % 100) / 10], ICON_SP02_NUM_SIZE);
//        
//        start_column += ICON_SP02_NUM_WIDE;
//        Gui_PageClearBackground(start_column, start_page, 1, ICON_FUNCTION_DATA_HIGH);
//        start_column += 1;
//        // ����ֵ
//        LCD_SetPosition(start_column, start_page, ICON_SP02_NUM_WIDE, ICON_SP02_NUM_HIGH);
//        LCD_DriverWriteDataFromExtFlash(icon_16_spO2_data_addr[progress_tmp % 10], ICON_SP02_NUM_SIZE);
//        
//        start_column += ICON_SP02_NUM_WIDE;
//        Gui_PageClearBackground(start_column, start_page, icon_16_spO2_icon5_coord[1][0] - start_column, ICON_FUNCTION_DATA_HIGH);
//        // %����
//        LCD_SetPosition(icon_16_spO2_icon5_coord[1][0], icon_16_spO2_icon5_coord[1][1], 
//					    ICON_SP02_ICON5_WIDE, ICON_SP02_ICON5_HIGH);
//        LCD_DriverWriteDataFromExtFlash(icon_16_spO2_icon5_addr[0], ICON_SP02_ICON5_SIZE);
//        
//        start_column = icon_16_spO2_icon5_coord[1][0] + ICON_SP02_ICON5_WIDE;
//        Gui_PageClearBackground(start_column, start_page, 80 - start_column, ICON_FUNCTION_DATA_HIGH);
//    }
//    else
//    {
//        start_column = 26;
//       
//        Gui_PageClearBackground(0, start_page, start_column, ICON_SP02_NUM_HIGH);
//                
//        // ����ֵ
//        LCD_SetPosition(start_column, start_page, ICON_SP02_NUM_WIDE, ICON_SP02_NUM_HIGH);
//        LCD_DriverWriteDataFromExtFlash(icon_16_spO2_data_addr[progress_tmp % 10], ICON_SP02_NUM_SIZE);
//        
//        start_column += ICON_SP02_NUM_WIDE;
//        Gui_PageClearBackground(start_column, start_page, icon_16_spO2_icon5_coord[2][0] - start_column, ICON_FUNCTION_DATA_HIGH);
//        // %����
//        LCD_SetPosition(icon_16_spO2_icon5_coord[2][0], icon_16_spO2_icon5_coord[2][1], 
//					    ICON_SP02_ICON5_WIDE, ICON_SP02_ICON5_HIGH);
//        LCD_DriverWriteDataFromExtFlash(icon_16_spO2_icon5_addr[0], ICON_SP02_ICON5_SIZE);
//        
//        start_column = icon_16_spO2_icon5_coord[2][0] + ICON_SP02_ICON5_WIDE;
//        Gui_PageClearBackground(start_column, start_page, 80 - start_column, ICON_FUNCTION_DATA_HIGH);
//    }
}

/****************************************************************
** Function name:            Gui_OxygenPage
** Descriptions:             Ѫ������
** input parameters:         index:         ͼ������
                             count:         Ѫ��ֵ
** output parameters:        ��
** Returned value:           ��
** Created by:               ����
** Created Date:             2017-07-10
****************************************************************/
void Gui_OxygenPage(uint8_t index, uint8_t ox_count, uint8_t prg_cnt, uint8_t ox_error)
{
    static uint8_t last_init_flag = 0;
    uint8_t start_column = 0;
    uint8_t start_page = 0;
	uint8_t count_tmp = 0;
    uint8_t dis_progress = 0;
	
    // Ѫ��ͼ��
	LCD_SetPosition(icon_16_spO2_icon1_coord[0][0], icon_16_spO2_icon1_coord[0][1], 
					ICON_SP02_ICON1_WIDE, ICON_SP02_ICON1_HIGH);
	LCD_DriverWriteDataFromExtFlash(icon_16_spO2_icon1_addr[index%2], ICON_SP02_ICON1_SIZE);
    
    //sp02��ʾ��
    LCD_SetPosition(icon_16_spO2_unit1_coord[0][0], icon_16_spO2_unit1_coord[0][1], 
					ICON_SP02_UNIT_WIDE, ICON_SP02_UNIT_HIGH);
	LCD_DriverWriteDataFromExtFlash(icon_16_spO2_unit1_addr[0], ICON_SP02_UNIT_SIZE);
	
	start_page = icon_16_spO2_data2_coord[0][1];
	count_tmp = ox_count;
    if(prg_cnt < 100 && ox_error != 1)
    {           
        if(prg_cnt >= 100)
        {
            dis_progress = 10;
        }
        else
        {
            dis_progress = index % 11;
        }
        if(index % 11 == 0)
        {
            //�������ı���
            LCD_SetPosition(icon_16_spO2_icon4_coord[0][0], icon_16_spO2_icon4_coord[0][1], 
                            ICON_SP02_ICON4_WIDE, ICON_SP02_ICON4_HIGH);
            LCD_DriverWriteDataFromExtFlash(icon_16_spO2_icon4_addr[0], ICON_SP02_ICON4_SIZE); 
        }
        else
        {
            start_column = icon_16_spO2_icon3_coord[0][0];
            for(uint8_t i = 0; i <= dis_progress; i++)
            {
                if(i > 0)
                {
                    LCD_SetPosition(start_column, icon_16_spO2_icon3_coord[0][1], 
                                    ICON_SP02_ICON3_WIDE, ICON_SP02_ICON3_HIGH);
                    LCD_DriverWriteDataFromExtFlash(icon_16_spO2_icon3_addr[0], ICON_SP02_ICON3_SIZE);
                    start_column += ICON_SP02_ICON3_WIDE + 3;
                }
            }  
        }
        last_init_flag = 0;
        return ;
    }
    
    if(ox_error != 1)
    {
        if(count_tmp > 99)
        {
            count_tmp = 99;
        }
        
        if(count_tmp >= 10)
        {
            start_column = icon_16_spO2_data2_coord[0][0];
            
            Gui_PageClearBackground(0, start_page, start_column, ICON_FUNCTION_DATA_HIGH);
            // Ѫ��ֵ
            LCD_SetPosition(start_column, start_page, ICON_FUNCTION_DATA_WIDE, ICON_FUNCTION_DATA_HIGH);
            LCD_DriverWriteDataFromExtFlash(icon_16_function_digital_addr[(count_tmp % 100) / 10], ICON_FUNCTION_DATA_SIZE);
            
            start_column += ICON_FUNCTION_DATA_WIDE;
            Gui_PageClearBackground(start_column, start_page, 2, ICON_FUNCTION_DATA_HIGH);
            start_column += 2;
            // Ѫ��ֵ
            LCD_SetPosition(start_column, start_page, ICON_FUNCTION_DATA_WIDE, ICON_FUNCTION_DATA_HIGH);
            LCD_DriverWriteDataFromExtFlash(icon_16_function_digital_addr[count_tmp % 10], ICON_FUNCTION_DATA_SIZE);
            
            start_column += ICON_FUNCTION_DATA_WIDE;
            Gui_PageClearBackground(start_column, start_page, icon_16_spO2_icon2_coord[0][0] - start_column, ICON_FUNCTION_DATA_HIGH);
            // %����
            LCD_SetPosition(icon_16_spO2_icon2_coord[0][0], icon_16_spO2_icon2_coord[0][1], 
                            ICON_SP02_ICON2_WIDE, ICON_SP02_ICON2_HIGH);
            LCD_DriverWriteDataFromExtFlash(icon_16_spO2_icon2_addr[0], ICON_SP02_ICON2_SIZE);
            
            start_column = icon_16_spO2_icon2_coord[0][0] + ICON_SP02_ICON2_WIDE;
            Gui_PageClearBackground(start_column, start_page, 80 - start_column, ICON_FUNCTION_DATA_HIGH);
            
            last_init_flag = 0;
        }
        else 
        {
            if(index == 0 || last_init_flag != 1)
            {
                last_init_flag = 1;
                // "- - %"
                uint8_t dis_buf[20*5*2] = {0};
                memcpy(dis_buf, icon_spo2_init_image, 20*5*2);
                
                Gui_PageClearBackground(0, start_page, 80, ICON_FUNCTION_DATA_HIGH);
                
                LCD_SetPosition(18, 133, 20, 5);
                LCD_DriverWriteDataBuf(dis_buf, 20*5*2);
                LCD_SetPosition(42, 133, 20, 5);
                LCD_DriverWriteDataBuf(dis_buf, 20*5*2);
            }
        }
    }
    else
    {
        // " X X"
        if(index == 0 || last_init_flag != 2)
        {
            last_init_flag = 2;
            Gui_PageClearBackground(0, start_page, 80, ICON_FUNCTION_DATA_HIGH);
            
            LCD_SetPosition(icon_16_spO2_error_coord[0][0], icon_16_spO2_error_coord[0][1], 
                            ICON_SP02ERROR_ICON_WIDE, ICON_SP02ERROR_ICON_HIGH);
            LCD_DriverWriteDataFromExtFlash(icon_16_spO2_error_addr[0], ICON_SP02ERROR_ICON_SIZE);
                   
            LCD_SetPosition(icon_16_spO2_error_coord[1][0], icon_16_spO2_error_coord[1][1], 
                            ICON_SP02ERROR_ICON_WIDE, ICON_SP02ERROR_ICON_HIGH);
            LCD_DriverWriteDataFromExtFlash(icon_16_spO2_error_addr[0], ICON_SP02ERROR_ICON_SIZE);    
        }
    }
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
void Gui_LowOxygenAlertPage(uint8_t index)
{
    /* ��ʾ��̬ͼ�� */
    LCD_SetPosition(icon_16_sp02low_icon_coord[0][0], icon_16_sp02low_icon_coord[0][1],
                    ICON_SP02LOW_ICON1_WIDE, ICON_SP02LOW_ICON1_HIGH);
    LCD_DriverWriteDataFromExtFlash(icon_16_sp02low_icon_addr[index % 2], ICON_SP02LOW_ICON1_SIZE);
}


/****************************************************************
** Function name:           Gui_PageCartoon
** Descriptions:            ��������
** input parameters:        index����̬ͼ����
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ
** Created Date:            2018-06-05
*****************************************************************/
void Gui_PageCartoon(uint8_t index)
{
//    LCD_SetPosition(icon_16_poweron_dynamic_coord[0][0], icon_16_poweron_dynamic_coord[0][1], 
//                   ICON_POWERON_ICON_WIDE, ICON_POWERON_ICON_HIGH);
//    LCD_DriverWriteDataFromExtFlash(icon_16_poweron_dynamic_addr[0], ICON_POWERON_ICON_SIZE); 
}

/****************************************************************
** Function name:           Gui_UpdateBp
** Descriptions:            ��ʾѪѹ��������
** input parameters:        bp_sp: ��ѹ bp_dp����ѹ
** output parameters:       
** Returned value:          
** Created by:              YX              
** Created Date:            2017-08-11
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
static void Gui_UpdateBp(uint8_t bp_sp, uint8_t bp_dp)
{
    uint8_t start_column = 0;
    uint8_t start_page = 0;
    
    //�����λ�͸�ѹ֮��ı���
    start_page = icon_16_bp_unit_coord[0][1] + ICON_BP_UNIT_HIGH;
    Gui_PageClearBackground(0, start_page, 
                           LCD_MAX_COLUMN, icon_16_bp_data_coord[0][1] - start_page);
    
    start_page = icon_16_bp_data_coord[0][1]; 
    if(bp_sp >= 100)
    {
        start_column = icon_16_bp_data_coord[0][0];  
        
        Gui_PageClearBackground(0, start_page, start_column, ICON_BP_DATA_HIGH);        
        LCD_SetPosition(start_column, start_page, ICON_BP_DATA_WIDE, ICON_BP_DATA_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_bp_data_addr[(bp_sp / 100) % 10], ICON_BP_DATA_SIZE);                          
        Gui_PageClearBackground(start_column + ICON_BP_DATA_WIDE, start_page, 2, ICON_BP_DATA_HIGH);
        start_column += ICON_BP_DATA_WIDE + 2;        
    }
    else
    {
        start_column = icon_16_bp_data_coord[2][0];
        Gui_PageClearBackground(0, start_page, start_column, ICON_BP_DATA_HIGH);
    }
    
    LCD_SetPosition(start_column, start_page, ICON_BP_DATA_WIDE, ICON_BP_DATA_HIGH);
    LCD_DriverWriteDataFromExtFlash(icon_16_bp_data_addr[(bp_sp / 10) % 10], ICON_BP_DATA_SIZE);          
             
    Gui_PageClearBackground(start_column + ICON_BP_DATA_WIDE, start_page, 2, ICON_BP_DATA_HIGH);
    start_column += ICON_BP_DATA_WIDE + 2;
    LCD_SetPosition(start_column, start_page, ICON_BP_DATA_WIDE, ICON_BP_DATA_HIGH);
    LCD_DriverWriteDataFromExtFlash(icon_16_bp_data_addr[bp_sp % 10], ICON_BP_DATA_SIZE);          
    start_column += ICON_BP_DATA_WIDE;
    
    Gui_PageClearBackground(start_column, start_page, LCD_MAX_COLUMN - start_column, ICON_BP_DATA_HIGH);
 
    
    //�����ѹ�͵�ѹ֮��ı���
    start_page += ICON_BP_DATA_HIGH;
    Gui_PageClearBackground(0, start_page, 
                           LCD_MAX_COLUMN, icon_16_bp_data_coord[1][1] - start_page);
    
    start_page = icon_16_bp_data_coord[1][1]; 
    /* ��ѹ */
    if(bp_dp >= 100)
    {
        start_column = icon_16_bp_data_coord[1][0];  
        
        Gui_PageClearBackground(0, start_page, start_column, ICON_BP_DATA_HIGH);
        
        LCD_SetPosition(start_column, start_page, ICON_BP_DATA_WIDE, ICON_BP_DATA_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_bp_data_addr[(bp_dp / 100) % 10], ICON_BP_DATA_SIZE);          
                 
        Gui_PageClearBackground(start_column + ICON_BP_DATA_WIDE, start_page, 2, ICON_BP_DATA_HIGH);
        start_column += ICON_BP_DATA_WIDE + 2;        
    }
    else
    {
        start_column = icon_16_bp_data_coord[3][0];
        Gui_PageClearBackground(0, start_page, start_column, ICON_BP_DATA_HIGH);
    }    
    LCD_SetPosition(start_column, start_page, ICON_BP_DATA_WIDE, ICON_BP_DATA_HIGH);
    LCD_DriverWriteDataFromExtFlash(icon_16_bp_data_addr[(bp_dp / 10) % 10], ICON_BP_DATA_SIZE);          
    Gui_PageClearBackground(start_column + ICON_BP_DATA_WIDE, start_page, 2, ICON_BP_DATA_HIGH);
    start_column += ICON_BP_DATA_WIDE + 2;
    LCD_SetPosition(start_column, start_page, ICON_BP_DATA_WIDE, ICON_BP_DATA_HIGH);
    LCD_DriverWriteDataFromExtFlash(icon_16_bp_data_addr[bp_dp % 10], ICON_BP_DATA_SIZE);          
    start_column += ICON_BP_DATA_WIDE;
    
    Gui_PageClearBackground(start_column, start_page, LCD_MAX_COLUMN - start_column, ICON_BP_DATA_HIGH); 
    //��ѹ֮�µı���
    start_page += ICON_BP_DATA_HIGH;
    Gui_PageClearBackground(0, start_page, 
                           LCD_MAX_COLUMN, LCD_MAX_ROW - start_page);
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
void Gui_PageBP(uint8_t index, uint8_t bp_sp, uint8_t bp_dp, uint8_t test_mode, uint8_t test_state)
{
    if(index == 0 )
    {
        /* ����Ѫѹͼ�꾲̬ */
        LCD_SetPosition(icon_16_bp_contour_coord[0][0], icon_16_bp_contour_coord[0][1], 
                        ICON_BP_ICON1_WIDE, ICON_BP_ICON1_HIGH);
        if(test_mode == 1)
        {   //P
            LCD_DriverWriteDataFromExtFlash(icon_16_bp_contour_addr[1], ICON_BP_ICON1_SIZE);
        }
        else
        {
            LCD_DriverWriteDataFromExtFlash(icon_16_bp_contour_addr[0], ICON_BP_ICON1_SIZE);
        }
        /* mmhg */
        LCD_SetPosition(icon_16_bp_unit_coord[0][0], icon_16_bp_unit_coord[0][1], 
                        ICON_BP_UNIT_WIDE, ICON_BP_UNIT_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_bp_unit_addr[0], ICON_BP_UNIT_SIZE);

    }
    if(test_state == 0)
    {
        /* ����Ѫѹͼ�궯̬ */
        LCD_SetPosition(icon_16_bp_round_coord[0][0], icon_16_bp_round_coord[0][1], 
                        ICON_BP_ICON2_WIDE, ICON_BP_ICON2_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_bp_round_addr[index % 8], ICON_BP_ICON2_SIZE);     
        
        for(uint8_t i = 0; i < 5; i++)
        {
            LCD_SetPosition(icon_16_bp_bar_coord[i][0], icon_16_bp_bar_coord[i][1], 
                            ICON_BP_ICON3_WIDE, ICON_BP_ICON3_HIGH);
            LCD_DriverWriteDataFromExtFlash(icon_16_bp_bar_addr[0], ICON_BP_ICON3_SIZE);
        }        
    }
    else if(test_state == 1)
    {	 
        index = 5;		
        Gui_UpdateBp(bp_sp, bp_dp);
    }
    else if(test_state >= 2)
    {
        index = 5;
        
        Gui_PageClearBackground(0, icon_16_bp_xicon_coord[0][1], 
                               icon_16_bp_xicon_coord[0][0], ICON_BP_ICON4_HIGH);
        Gui_PageClearBackground(0, icon_16_bp_xicon_coord[3][1], 
                               icon_16_bp_xicon_coord[3][0], ICON_BP_ICON4_HIGH);
        /* xxxx */
        for(uint8_t i = 0; i < 5; i++)
        {
            LCD_SetPosition(icon_16_bp_xicon_coord[i][0], icon_16_bp_xicon_coord[i][1], 
                            ICON_BP_ICON4_WIDE, ICON_BP_ICON4_HIGH);
            LCD_DriverWriteDataFromExtFlash(icon_16_bp_xicon_addr[0], ICON_BP_ICON4_SIZE);
            if(i < 2 || i == 3)
            {
                Gui_PageClearBackground(icon_16_bp_xicon_coord[i][0] + ICON_BP_ICON4_WIDE, icon_16_bp_xicon_coord[i][1], 
                                       icon_16_bp_xicon_coord[i + 1][0] - icon_16_bp_xicon_coord[i][0], ICON_BP_ICON4_HIGH);
            } 
            else if( i == 2 || i == 4)
            {
                Gui_PageClearBackground(icon_16_bp_xicon_coord[i][0] + ICON_BP_ICON4_WIDE, icon_16_bp_xicon_coord[i][1], 
                                       LCD_MAX_COLUMN - (icon_16_bp_xicon_coord[i][0] + ICON_BP_ICON4_WIDE), 
                                       ICON_BP_ICON4_HIGH);
            }
        }  
    }
    /* ����Ѫѹͼ�궯̬ */
    LCD_SetPosition(icon_16_bp_round_coord[0][0], icon_16_bp_round_coord[0][1], 
                    ICON_BP_ICON2_WIDE, ICON_BP_ICON2_HIGH);
    LCD_DriverWriteDataFromExtFlash(icon_16_bp_round_addr[index % 8], ICON_BP_ICON2_SIZE);	
}


