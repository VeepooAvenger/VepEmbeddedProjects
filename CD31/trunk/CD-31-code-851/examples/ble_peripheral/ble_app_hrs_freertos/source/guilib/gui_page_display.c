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
#include "backlight_driving.h"
#include "cpu_common.h"


// �������Ա���
static uint8_t language_save = 0xff;

// ����֡�����������ڶ�����ͣ����
static uint16_t animation_cnt = 0;
    
extern uint8_t ui_style_type;

extern uint8_t lcd_backlight_level;

//extern uint8_t dbug_dis_ui_num;
//extern uint8_t dbug_dis_ui_flg;

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
	vTaskDelay(30); 
    Backlight_SetLeve(lcd_backlight_level);
	vTaskDelay(30);
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
    //WJ.20180427.��ֹԽ�紦��
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


static void Gui_PageMainAssistant(uint16_t index, uint8_t week, uint8_t language, uint16_t am_pm, 
                                    uint8_t ble_status, uint8_t charge, uint8_t battery_level, 
                                    uint8_t menstrual, uint8_t y_coord)
{
    if(language)
    {
        language = 1;
    }
    
    // ���� ����
    if(index == 0)
    {
        Gui_PageClearBackground(0, 
                                0, 
                                MAX_X_SIZE, 
                                y_coord); 
    }
    
    // ����
    LCD_SetPosition(icon_16_main1_blue_coord[0][0], icon_16_main1_blue_coord[0][1],
                    ICON_MAIN1_ICON1_WIDE, ICON_MAIN1_ICON1_HIGH);
    LCD_DriverWriteDataFromExtFlash(icon_16_main1_blue_addr[0][ble_status % 2], ICON_MAIN1_ICON1_SIZE);

    // ������
    LCD_SetPosition(icon_16_main1_ampm_coord[0][0], icon_16_main1_ampm_coord[0][1], 
                    ICON_MAIN1_ICON2_WIDE, ICON_MAIN1_ICON2_HIGH);
    if(am_pm == GUI_TIME_FORMAT_AM)
    {
        if(language == 0)
        {
            LCD_DriverWriteDataFromExtFlash(icon_16_main1_ampm_addr[0][0], ICON_MAIN1_ICON2_SIZE);
        }
        else
        {
            LCD_DriverWriteDataFromExtFlash(icon_16_main1_ampm_addr[0][2], ICON_MAIN1_ICON2_SIZE);
        }
    }
    else if(am_pm == GUI_TIME_FORMAT_PM)
    {
        if(language == 0)
        {
            LCD_DriverWriteDataFromExtFlash(icon_16_main1_ampm_addr[0][1], ICON_MAIN1_ICON2_SIZE);
        }
        else
        {
            LCD_DriverWriteDataFromExtFlash(icon_16_main1_ampm_addr[0][3], ICON_MAIN1_ICON2_SIZE);
        }
    }
    else
    { 
        //��Ҫ�滻
        LCD_DriverWriteDataFromExtFlash(icon_16_main1_ampm_addr[0][4], ICON_MAIN1_ICON2_SIZE);
    }
    
    // ����
    if(week)
    {
        week -= 1;
        week %= 7;
    }
    LCD_SetPosition(icon_16_main1_week_coord[0][0], icon_16_main1_week_coord[0][1],
                    ICON_MAIN1_WEEK_WIDE, ICON_MAIN1_WEEK_HIGH);
    LCD_DriverWriteDataFromExtFlash(icon_16_main1_week_addr[0][language][week], ICON_MAIN1_WEEK_SIZE);

    //Ů��
    LCD_SetPosition(icon_16_main1_menstrual_coord[0][0], icon_16_main1_menstrual_coord[0][1], 
                    ICON_MAIN1_ICON3_WIDE, ICON_MAIN1_ICON3_HIGH);
    LCD_DriverWriteDataFromExtFlash(icon_16_main1_menstrual_addr[0][menstrual % 5], ICON_MAIN1_ICON3_SIZE);
        
    // ���
    if(charge == BATTERRY_IS_CHARGING)
    {
        // ����� 
        LCD_SetPosition(icon_16_main1_battery_coord[0][0], icon_16_main1_battery_coord[0][1],
                        ICON_MAIN1_ICON4_WIDE, ICON_MAIN1_ICON4_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_main1_battery_addr[0][index % 5], ICON_MAIN1_ICON4_SIZE);

    }
    else
    {
        if(battery_level == 0)
        {
            if((index % 4 == 1)||(index % 4 == 3))
            {
                // ��ؿ�
                LCD_SetPosition(icon_16_main1_battery_coord[0][0], icon_16_main1_battery_coord[0][1],
                                ICON_MAIN1_ICON4_WIDE, ICON_MAIN1_ICON4_HIGH);
                LCD_DriverWriteDataFromExtFlash(icon_16_main1_battery_addr[0][5], ICON_MAIN1_ICON4_SIZE);
            }
            else if((index % 4 == 0)||(index % 4 == 2))
            {
                // ��ؿ�
                LCD_SetPosition(icon_16_main1_battery_coord[0][0], icon_16_main1_battery_coord[0][1],
                                ICON_MAIN1_ICON4_WIDE, ICON_MAIN1_ICON4_HIGH);
                LCD_DriverWriteDataFromExtFlash(icon_16_main1_battery_addr[0][0], ICON_MAIN1_ICON4_SIZE);
            }
        }
        else
        {
            // �������
            battery_level %= 5;
            
            LCD_SetPosition(icon_16_main1_battery_coord[0][0], icon_16_main1_battery_coord[0][1],
                            ICON_MAIN1_ICON4_WIDE, ICON_MAIN1_ICON4_HIGH);
            LCD_DriverWriteDataFromExtFlash(icon_16_main1_battery_addr[0][battery_level], ICON_MAIN1_ICON4_SIZE);
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
void Gui_PageMainStye01(uint16_t index, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second, uint8_t week, 
                        uint16_t am_pm, uint32_t step_cnt, uint8_t heart,
                        uint8_t ble_status, uint8_t charge, uint8_t battery_level, uint8_t menstrual,
                        uint16_t calories_cnt, uint32_t step_target, uint16_t sleep_time, uint8_t language)
{

    Gui_PageMainAssistant( index, week, language, am_pm, ble_status, charge, battery_level, menstrual, icon_16_main1_time_coord[0][1]);
    
    // ʱ���� ����
    if(index == 0)
    {
        // ʱ��������
        Gui_PageClearBackground(0, 
                                icon_16_main1_time_coord[0][1], 
                                icon_16_main1_time_coord[0][0] - 0, 
                                ICON_MAIN1_DATA1_HIGH); 
    }
    
    // ʱ��
    // ����ʱ��--ʱ
    LCD_SetPosition(icon_16_main1_time_coord[0][0], icon_16_main1_time_coord[0][1],
                    ICON_MAIN1_DATA1_WIDE, ICON_MAIN1_DATA1_HIGH);
    LCD_DriverWriteDataFromExtFlash(icon_16_main1_time_addr[0][(hour / 10) % 10], ICON_MAIN1_DATA1_SIZE);
    LCD_SetPosition(icon_16_main1_time_coord[1][0], icon_16_main1_time_coord[1][1],
                    ICON_MAIN1_DATA1_WIDE, ICON_MAIN1_DATA1_HIGH);
    LCD_DriverWriteDataFromExtFlash(icon_16_main1_time_addr[0][hour % 10], ICON_MAIN1_DATA1_SIZE);
    
    // ʱ���� ����
    if(index == 0)
    {
        // ʱ��ð�ż������
        Gui_PageClearBackground(icon_16_main1_time_coord[1][0] + ICON_MAIN1_DATA1_WIDE, 
                                icon_16_main1_time_coord[0][1], 
                                icon_16_main1_timedot_coord[0][0] - (icon_16_main1_time_coord[1][0] + ICON_MAIN1_DATA1_WIDE), 
                                ICON_MAIN1_DATA1_HIGH); 
    }
    
    // ����ʱ��--ð��
    if(index == 0)
    {
        LCD_SetPosition(icon_16_main1_timedot_coord[0][0], icon_16_main1_timedot_coord[0][1],
                        ICON_MAIN1_DATA1DOT_WIDE, ICON_MAIN1_DATA1DOT_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_main1_timedot_addr[0][0], ICON_MAIN1_DATA1DOT_SIZE);
    }
    
    // ʱ���� ����
    if(index == 0)
    {       
        // ʱ��ð�ż������
        Gui_PageClearBackground(icon_16_main1_timedot_coord[0][0] + ICON_MAIN1_DATA1DOT_WIDE, 
                                icon_16_main1_time_coord[0][1], 
                                icon_16_main1_time_coord[2][0] - (icon_16_main1_timedot_coord[0][0] + ICON_MAIN1_DATA1DOT_WIDE), 
                                ICON_MAIN1_DATA1_HIGH); 
    }
    
    // ����ʱ��--�� 
    LCD_SetPosition(icon_16_main1_time_coord[2][0], icon_16_main1_time_coord[2][1],
                    ICON_MAIN1_DATA1_WIDE, ICON_MAIN1_DATA1_HIGH);
    LCD_DriverWriteDataFromExtFlash(icon_16_main1_time_addr[0][(minute / 10) % 10], ICON_MAIN1_DATA1_SIZE);
    LCD_SetPosition(icon_16_main1_time_coord[3][0], icon_16_main1_time_coord[3][1],
                    ICON_MAIN1_DATA1_WIDE, ICON_MAIN1_DATA1_HIGH);
    LCD_DriverWriteDataFromExtFlash(icon_16_main1_time_addr[0][minute % 10], ICON_MAIN1_DATA1_SIZE); 

    // ʱ���� ����
    if(index == 0)
    {       
        // ʱ���Ҳ�����
        Gui_PageClearBackground(icon_16_main1_time_coord[3][0] + ICON_MAIN1_DATA1_WIDE, 
                                icon_16_main1_time_coord[0][1], 
                                MAX_X_SIZE - (icon_16_main1_time_coord[3][0] + ICON_MAIN1_DATA1_WIDE), 
                                ICON_MAIN1_DATA1_HIGH); 
    }
    
    // ���ڱ��� ����
    if(index == 0)
    {
        Gui_PageClearBackground(0, 
                                icon_16_main1_time_coord[0][1] + ICON_MAIN1_DATA1_HIGH, 
                                MAX_X_SIZE, 
                                (icon_16_main1_data_coord[0][1] + ICON_MAIN1_DATA2_HIGH) - 
                                (icon_16_main1_time_coord[0][1] + ICON_MAIN1_DATA1_HIGH)); 
    }
    
    // ����
    // ������ 
    LCD_SetPosition(icon_16_main1_data_coord[0][0], icon_16_main1_data_coord[0][1], 
                    ICON_MAIN1_DATA2_WIDE, ICON_MAIN1_DATA2_HIGH);
    LCD_DriverWriteDataFromExtFlash(icon_16_main1_data_addr[0][month / 10 % 10], ICON_MAIN1_DATA2_SIZE);
    LCD_SetPosition(icon_16_main1_data_coord[1][0], icon_16_main1_data_coord[1][1], 
                    ICON_MAIN1_DATA2_WIDE, ICON_MAIN1_DATA2_HIGH);
    LCD_DriverWriteDataFromExtFlash(icon_16_main1_data_addr[0][month % 10], ICON_MAIN1_DATA2_SIZE);
    
    // ������
    LCD_SetPosition(icon_16_main1_data_coord[2][0], icon_16_main1_data_coord[2][1], 
                    ICON_MAIN1_DATA2_WIDE, ICON_MAIN1_DATA2_HIGH);
    LCD_DriverWriteDataFromExtFlash(icon_16_main1_data_addr[0][day / 10 % 10], ICON_MAIN1_DATA2_SIZE);
    LCD_SetPosition(icon_16_main1_data_coord[3][0], icon_16_main1_data_coord[3][1], 
                    ICON_MAIN1_DATA2_WIDE, ICON_MAIN1_DATA2_HIGH);
    LCD_DriverWriteDataFromExtFlash(icon_16_main1_data_addr[0][day % 10], ICON_MAIN1_DATA2_SIZE);
    
    // ���� ��
    LCD_SetPosition(icon_16_main1_datadot_coord[0][0], icon_16_main1_datadot_coord[0][1], 
                    ICON_MAIN1_DATA2DOT_WIDE, ICON_MAIN1_DATA2DOT_HIGH);
    LCD_DriverWriteDataFromExtFlash(icon_16_main1_datadot_addr[0][0], ICON_MAIN1_DATA2DOT_SIZE);
    
    
    // ���ڵײ�  ����
    if(index == 0)
    {
        Gui_PageClearBackground(0, 
                                icon_16_main1_data_coord[0][1] + ICON_MAIN1_DATA2_HIGH, 
                                MAX_X_SIZE, 
                                MAX_Y_SIZE - (icon_16_main1_data_coord[0][1] + ICON_MAIN1_DATA2_HIGH)); 
    }
    
    // �Ʋ�ͼ��
    if(index == 0)
    {
        LCD_SetPosition(icon_16_main1_step_icon_coord[0][0], icon_16_main1_step_icon_coord[0][1], 
                        ICON_MAIN1_ICON5_WIDE, ICON_MAIN1_ICON5_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_main1_step_icon_addr[0][0], ICON_MAIN1_ICON5_SIZE);
    }
    
    // �Ʋ�����
    if(step_cnt > 99999)
    {
        step_cnt = 99999;
    }

    LCD_SetPosition(icon_16_main1_step_num_coord[0][0], icon_16_main1_step_num_coord[0][1], 
                    ICON_MAIN1_DATA3_WIDE, ICON_MAIN1_DATA3_HIGH);
    if(step_cnt >= 10000)
    {    
        LCD_DriverWriteDataFromExtFlash(icon_16_main1_step_num_addr[0][(step_cnt / 10000) %10], ICON_MAIN1_DATA3_SIZE);        
    }  
    else
    {    
        LCD_DriverWriteDataFromExtFlash(icon_16_main1_step_num_addr[0][0], ICON_MAIN1_DATA3_SIZE);        
    } 
    
    LCD_SetPosition(icon_16_main1_step_num_coord[1][0], icon_16_main1_step_num_coord[1][1], 
                    ICON_MAIN1_DATA3_WIDE, ICON_MAIN1_DATA3_HIGH);
    if(step_cnt >= 1000)
    {    
        LCD_DriverWriteDataFromExtFlash(icon_16_main1_step_num_addr[0][(step_cnt / 1000) %10], ICON_MAIN1_DATA3_SIZE);        
    }  
    else
    {    
        LCD_DriverWriteDataFromExtFlash(icon_16_main1_step_num_addr[0][0], ICON_MAIN1_DATA3_SIZE);        
    }
    
    LCD_SetPosition(icon_16_main1_step_num_coord[2][0], icon_16_main1_step_num_coord[2][1], 
                    ICON_MAIN1_DATA3_WIDE, ICON_MAIN1_DATA3_HIGH);;
    if(step_cnt >= 100)
    {    
        LCD_DriverWriteDataFromExtFlash(icon_16_main1_step_num_addr[0][(step_cnt / 100) %10], ICON_MAIN1_DATA3_SIZE);        
    }  
    else
    {    
        LCD_DriverWriteDataFromExtFlash(icon_16_main1_step_num_addr[0][0], ICON_MAIN1_DATA3_SIZE);        
    }
    LCD_SetPosition(icon_16_main1_step_num_coord[3][0], icon_16_main1_step_num_coord[3][1], 
                    ICON_MAIN1_DATA3_WIDE, ICON_MAIN1_DATA3_HIGH);
    if(step_cnt >= 10)
    {    
        LCD_DriverWriteDataFromExtFlash(icon_16_main1_step_num_addr[0][(step_cnt / 10) %10], ICON_MAIN1_DATA3_SIZE);        
    }  
    else
    {    
        LCD_DriverWriteDataFromExtFlash(icon_16_main1_step_num_addr[0][0], ICON_MAIN1_DATA3_SIZE);        
    }
    
    LCD_SetPosition(icon_16_main1_step_num_coord[4][0], icon_16_main1_step_num_coord[4][1], 
                    ICON_MAIN1_DATA3_WIDE, ICON_MAIN1_DATA3_HIGH);
    if(step_cnt > 0)
    {    
        LCD_DriverWriteDataFromExtFlash(icon_16_main1_step_num_addr[0][step_cnt %10], ICON_MAIN1_DATA3_SIZE);        
    }  
    else
    {    
        LCD_DriverWriteDataFromExtFlash(icon_16_main1_step_num_addr[0][0], ICON_MAIN1_DATA3_SIZE);        
    }
    
    // ˯��ͼ��
    if(index == 0)
    {
        LCD_SetPosition(icon_16_main1_sleep_icon_coord[0][0], icon_16_main1_sleep_icon_coord[0][1], 
                        ICON_MAIN1_ICON6_WIDE, ICON_MAIN1_ICON6_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_main1_sleep_icon_addr[0][0], ICON_MAIN1_ICON6_SIZE);
    }
    
    // ˯��ʱ��
    {
        uint8_t sleep_hours = sleep_time / 60;
        uint8_t sleep_minutes = 10 * (sleep_time % 60 / 60.0f);
        
        LCD_SetPosition(icon_16_main1_sleep_num_coord[0][0], icon_16_main1_sleep_num_coord[0][1], 
                        ICON_MAIN1_DATA3_WIDE, ICON_MAIN1_DATA3_HIGH);
        if(sleep_hours >= 10)
        {    
            LCD_DriverWriteDataFromExtFlash(icon_16_main1_sleep_num_addr[0][(sleep_hours / 10) %10], ICON_MAIN1_DATA3_SIZE);        
        }  
        else
        {    
            LCD_DriverWriteDataFromExtFlash(icon_16_main1_sleep_num_addr[0][0], ICON_MAIN1_DATA3_SIZE);        
        }

        LCD_SetPosition(icon_16_main1_sleep_num_coord[1][0], icon_16_main1_sleep_num_coord[1][1], 
                        ICON_MAIN1_DATA3_WIDE, ICON_MAIN1_DATA3_HIGH);
        if(sleep_hours > 0)
        {    
            LCD_DriverWriteDataFromExtFlash(icon_16_main1_sleep_num_addr[0][sleep_hours %10], ICON_MAIN1_DATA3_SIZE);        
        }  
        else
        {    
            LCD_DriverWriteDataFromExtFlash(icon_16_main1_sleep_num_addr[0][0], ICON_MAIN1_DATA3_SIZE);        
        }

        LCD_SetPosition(icon_16_main1_sleep_num_coord[2][0], icon_16_main1_sleep_num_coord[2][1], 
                        ICON_MAIN1_DATA3_WIDE, ICON_MAIN1_DATA3_HIGH);
        if(sleep_minutes > 0 || sleep_hours > 0)
        {    
            LCD_DriverWriteDataFromExtFlash(icon_16_main1_sleep_num_addr[0][sleep_minutes %10], ICON_MAIN1_DATA3_SIZE);        
        }  
        else
        {    
            LCD_DriverWriteDataFromExtFlash(icon_16_main1_sleep_num_addr[0][0], ICON_MAIN1_DATA3_SIZE);        
        }

        //С����
        LCD_SetPosition(icon_16_main1_sleepdot_coord[0][0], icon_16_main1_sleepdot_coord[0][1], 
                        ICON_MAIN1_DATA3DOT_WIDE, ICON_MAIN1_DATA3DOT_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_main1_sleepdot_addr[0][0], ICON_MAIN1_DATA3DOT_SIZE); 
    }
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
 void Gui_PageMainStye02(uint16_t index, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second, uint8_t week, 
                         uint16_t am_pm, uint32_t step_cnt, uint8_t heart,
                         uint8_t ble_status, uint8_t charge, uint8_t battery_level, uint8_t menstrual,
                         uint16_t calories_cnt, uint32_t step_target, uint16_t sleep_time, uint8_t language)
{
    // �ϲ� �� ����
    if(index == 0)
    {
        Gui_PageClearBackground(0, 
                                0, 
                                MAX_X_SIZE, 
                                icon_16_main2_time_coord[0][1]); 
        
        Gui_PageClearBackground(0, 
                                0, 
                                icon_16_main2_databg_coord[0][0], 
                                MAX_Y_SIZE);
    }
    
    // ���ڱ���
    if(index == 0)
    {
        LCD_SetPosition(icon_16_main2_databg_coord[0][0], icon_16_main2_databg_coord[0][1], 
                        ICON_MAIN2_ICON1_WIDE, ICON_MAIN2_ICON1_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_main2_databg_addr[0][0], ICON_MAIN2_ICON1_SIZE);
    }
    
    // ������
    {
        LCD_SetPosition(icon_16_main2_data_coord[0][0], icon_16_main2_data_coord[0][1], 
                        ICON_MAIN2_DATA1_WIDE, ICON_MAIN2_DATA1_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_main2_data_addr[0][day / 10 % 10], ICON_MAIN2_DATA1_SIZE);
        
        LCD_SetPosition(icon_16_main2_data_coord[1][0], icon_16_main2_data_coord[1][1], 
                        ICON_MAIN2_DATA1_WIDE, ICON_MAIN2_DATA1_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_main2_data_addr[0][day % 10], ICON_MAIN2_DATA1_SIZE);
    } 
    
    // �����Ҳ��²�����
    if(index == 0)
    {
        Gui_PageClearBackground(icon_16_main2_databg_coord[0][0] + ICON_MAIN2_ICON1_WIDE, 
                                icon_16_main2_time_coord[0][1], 
                                icon_16_main2_time_coord[0][0] - (icon_16_main2_databg_coord[0][0] + ICON_MAIN2_ICON1_WIDE), 
                                ICON_MAIN2_ICON1_HIGH);
        
        Gui_PageClearBackground(0, 
                                icon_16_main2_databg_coord[0][1] + ICON_MAIN2_ICON1_HIGH, 
                                icon_16_main2_time_coord[0][0], 
                                icon_16_main2_week_coord[0][1] - (icon_16_main2_databg_coord[0][1] + ICON_MAIN2_ICON1_HIGH));
    }
    
    // ʱ��
    // ����ʱ��--ʱ
    {
        LCD_SetPosition(icon_16_main2_time_coord[0][0], icon_16_main2_time_coord[0][1],
                        ICON_MAIN2_DATA2_WIDE, ICON_MAIN2_DATA2_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_main2_hour_addr[0][(hour / 10) % 10], ICON_MAIN2_DATA2_SIZE);
        LCD_SetPosition(icon_16_main2_time_coord[1][0], icon_16_main2_time_coord[1][1],
                        ICON_MAIN2_DATA2_WIDE, ICON_MAIN2_DATA2_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_main2_hour_addr[0][hour % 10], ICON_MAIN2_DATA2_SIZE);
    }
    
    // Сʱ�Ҳ�����
    if(index == 0)
    {
        Gui_PageClearBackground(icon_16_main2_time_coord[1][0] + ICON_MAIN2_DATA2_WIDE, 
                                icon_16_main2_time_coord[0][1], 
                                MAX_X_SIZE -(icon_16_main2_time_coord[1][0] + ICON_MAIN2_DATA2_WIDE), 
                                ICON_MAIN2_DATA2_HIGH);
    }
    
    // ���ڱ���
    if(index == 0)
    {
        LCD_SetPosition(icon_16_main2_databg_coord[1][0], icon_16_main2_databg_coord[1][1], 
                        ICON_MAIN2_ICON1_WIDE, ICON_MAIN2_ICON1_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_main2_databg_addr[0][0], ICON_MAIN2_ICON1_SIZE);
    }
    
    // ����
    {
        if(week)
        {
            week -= 1;
            week %= 7;
        }
        LCD_SetPosition(icon_16_main2_week_coord[0][0], icon_16_main2_week_coord[0][1],
                        ICON_MAIN2_WEEK_WIDE, ICON_MAIN2_WEEK_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_main2_week_addr[0][language][week], ICON_MAIN2_WEEK_SIZE);
    }
    
    // �����Ҳ�����
    if(index == 0)
    {
        Gui_PageClearBackground(icon_16_main2_databg_coord[1][0] + ICON_MAIN2_ICON1_WIDE, 
                                icon_16_main2_databg_coord[1][1], 
                                icon_16_main2_time_coord[2][0] -(icon_16_main2_databg_coord[1][0] + ICON_MAIN2_ICON1_WIDE), 
                                ICON_MAIN2_ICON1_HIGH);
    }
    
    // �Ʋ����ϲ�����
    if(index == 0)
    {
        Gui_PageClearBackground(0, 
                                icon_16_main2_databg_coord[1][1] + ICON_MAIN2_ICON1_HIGH, 
                                icon_16_main2_time_coord[2][0], 
                                icon_16_main2_step_bg_coord[0][1] - (icon_16_main2_databg_coord[1][1] + ICON_MAIN2_ICON1_HIGH));
    }
    
    // �Ʋ���������ʾ
    if(index == 0)
    {
        LCD_SetPosition(icon_16_main2_step_bg_coord[0][0], icon_16_main2_step_bg_coord[0][1],
                        ICON_MAIN2_ICON2_WIDE, ICON_MAIN2_ICON2_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_main2_step_bg_addr[0][0], ICON_MAIN2_ICON2_SIZE); 
    }
    
    // �Ʋ�������ʾ
    if(index == 0)
    {
        uint32_t ring_num = 0;
        
        ring_num = (step_cnt * 20) / step_target;
        if(ring_num >= 20)
        {
            ring_num = 20;
        }
        
        for(uint32_t i = 0; i < ring_num; i++)
        {
            LCD_SetPosition(icon_16_main2_step_ring_coord[i][0], icon_16_main2_step_ring_coord[i][1],
                            ICON_MAIN2_ICON3_WIDE, ICON_MAIN2_ICON3_HIGH);
            LCD_DriverWriteDataFromExtFlash(icon_16_main2_step_ring_addr[0][i], ICON_MAIN2_ICON3_SIZE);
        }
    }
    
    //�����ϲ�����
    if(index == 0)
    {
        Gui_PageClearBackground(icon_16_main2_time_coord[2][0], 
                                icon_16_main2_time_coord[0][1] + ICON_MAIN2_DATA2_HIGH, 
                                MAX_X_SIZE -(icon_16_main2_time_coord[2][0]), 
                                icon_16_main2_time_coord[2][1] - (icon_16_main2_time_coord[0][1] + ICON_MAIN2_DATA2_HIGH));
    }
    
    // ����ʱ��--�� 
    {
        LCD_SetPosition(icon_16_main2_time_coord[2][0], icon_16_main2_time_coord[2][1],
                        ICON_MAIN2_DATA3_WIDE, ICON_MAIN2_DATA3_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_main2_minute_addr[0][(minute / 10) % 10], ICON_MAIN2_DATA3_SIZE);
        LCD_SetPosition(icon_16_main2_time_coord[3][0], icon_16_main2_time_coord[3][1],
                        ICON_MAIN2_DATA3_WIDE, ICON_MAIN2_DATA3_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_main2_minute_addr[0][minute % 10], ICON_MAIN2_DATA3_SIZE); 
    }
    
    // �����Ҳ�����
    if(index == 0)
    {
        Gui_PageClearBackground(icon_16_main2_time_coord[3][0] + ICON_MAIN2_DATA3_WIDE, 
                                icon_16_main2_time_coord[3][1], 
                                MAX_X_SIZE -(icon_16_main2_time_coord[3][0] + ICON_MAIN2_DATA3_WIDE), 
                                ICON_MAIN2_DATA2_HIGH);
    }

    
    
    // ��Ļ�ײ�����
    if(index == 0)
    {
        Gui_PageClearBackground(0, 
                                icon_16_main2_step_bg_coord[0][1] + ICON_MAIN2_ICON2_HIGH, 
                                icon_16_main2_time_coord[2][0], 
                                MAX_Y_SIZE - (icon_16_main2_step_bg_coord[0][1] + ICON_MAIN2_ICON2_HIGH));
        
        Gui_PageClearBackground(icon_16_main2_time_coord[2][0], 
                                icon_16_main2_time_coord[2][1] + ICON_MAIN2_DATA3_HIGH, 
                                MAX_X_SIZE - icon_16_main2_time_coord[2][0], 
                                MAX_Y_SIZE - (icon_16_main2_time_coord[2][1] + ICON_MAIN2_DATA3_HIGH));
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
void Gui_PageMainStye03(uint16_t index, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second, uint8_t week, 
                        uint16_t am_pm, uint32_t step_cnt, uint8_t heart,
                        uint8_t ble_status, uint8_t charge, uint8_t battery_level, uint8_t menstrual,
                        uint16_t calories_cnt, uint32_t step_target, uint16_t sleep_time, uint8_t language)
{    
    
    Gui_PageMainAssistant( index, week, language, am_pm, ble_status, charge, battery_level, menstrual, icon_16_main3_time_coord[0][1]);
    
    // ʱ���� ����
    if(index == 0)
    {
        // ʱ��������
        Gui_PageClearBackground(0, 
                                icon_16_main3_time_coord[0][1], 
                                icon_16_main3_time_coord[0][0] - 0, 
                                ICON_MAIN3_DATA1_HIGH); 
    }
    
    // ʱ��
    // ����ʱ��--ʱ
    LCD_SetPosition(icon_16_main3_time_coord[0][0], icon_16_main3_time_coord[0][1],
                    ICON_MAIN3_DATA1_WIDE, ICON_MAIN3_DATA1_HIGH);
    LCD_DriverWriteDataFromExtFlash(icon_16_main3_time_addr[0][(hour / 10) % 10], ICON_MAIN3_DATA1_SIZE);
    
//    // ʱ���� ����
//    if(index == 0)
//    {
//        // Сʱ�������
//        Gui_PageClearBackground(icon_16_main3_time_coord[0][0] + ICON_MAIN3_DATA1_WIDE, 
//                                icon_16_main3_time_coord[0][1], 
//                                icon_16_main3_time_coord[1][0] - (icon_16_main3_time_coord[0][0] + ICON_MAIN3_DATA1_WIDE), 
//                                ICON_MAIN3_DATA1_HIGH); 
//    }
    
    LCD_SetPosition(icon_16_main3_time_coord[1][0], icon_16_main3_time_coord[1][1],
                    ICON_MAIN3_DATA1_WIDE, ICON_MAIN3_DATA1_HIGH);
    LCD_DriverWriteDataFromExtFlash(icon_16_main3_time3_addr[0][hour % 10], ICON_MAIN3_DATA1_SIZE);
    
//    // ʱ���� ����
//    if(index == 0)
//    {        
//        // ʱ��ð�ż������
//        Gui_PageClearBackground(icon_16_main3_time_coord[1][0] + ICON_MAIN3_DATA1_WIDE, 
//                                icon_16_main3_time_coord[0][1], 
//                                icon_16_main3_timedot_coord[0][0] - (icon_16_main3_time_coord[1][0] + ICON_MAIN3_DATA1_WIDE), 
//                                ICON_MAIN3_DATA1_HIGH);
//    }
    
    // ����ʱ��--ð��    
    if(index == 0)
    {
        LCD_SetPosition(icon_16_main3_timedot_coord[0][0], icon_16_main3_timedot_coord[0][1],
                        ICON_MAIN3_DATA1DOT_WIDE, ICON_MAIN3_DATA1DOT_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_main3_timedot_addr[0][0], ICON_MAIN3_DATA1DOT_SIZE);
    }
    
//    // ʱ���� ����
//    if(index == 0)
//    {        
//        // ʱ��ð�ż������
//        Gui_PageClearBackground(icon_16_main3_timedot_coord[0][0] + ICON_MAIN3_DATA1DOT_WIDE, 
//                                icon_16_main3_time_coord[0][1], 
//                                icon_16_main3_time_coord[2][0] - (icon_16_main3_timedot_coord[0][0] + ICON_MAIN3_DATA1DOT_WIDE), 
//                                ICON_MAIN3_DATA1_HIGH); 
//    }
    
    // ����ʱ��--�� 
    LCD_SetPosition(icon_16_main3_time_coord[2][0], icon_16_main3_time_coord[2][1],
                    ICON_MAIN3_DATA1_WIDE, ICON_MAIN3_DATA1_HIGH);
    LCD_DriverWriteDataFromExtFlash(icon_16_main3_time_addr[0][(minute / 10) % 10], ICON_MAIN3_DATA1_SIZE);
    
//    // ʱ���� ����
//    if(index == 0)
//    {                                       
//        // ���Ӽ������
//        Gui_PageClearBackground(icon_16_main3_time_coord[2][0] + ICON_MAIN3_DATA1_WIDE, 
//                                icon_16_main3_time_coord[0][1], 
//                                icon_16_main3_time_coord[3][0] - (icon_16_main3_time_coord[2][0] + ICON_MAIN3_DATA1_WIDE), 
//                                ICON_MAIN3_DATA1_HIGH); 
//    }
    
    LCD_SetPosition(icon_16_main3_time_coord[3][0], icon_16_main3_time_coord[3][1],
                    ICON_MAIN3_DATA1_WIDE, ICON_MAIN3_DATA1_HIGH);
    LCD_DriverWriteDataFromExtFlash(icon_16_main3_time3_addr[0][minute % 10], ICON_MAIN3_DATA1_SIZE); 
    

    
    // ʱ���� ����
    if(index == 0)
    {                                                           
        // ʱ���Ҳ�����
        Gui_PageClearBackground(icon_16_main3_time_coord[3][0] + ICON_MAIN3_DATA1_WIDE, 
                                icon_16_main3_time_coord[0][1], 
                                MAX_X_SIZE - (icon_16_main3_time_coord[3][0] + ICON_MAIN3_DATA1_WIDE), 
                                ICON_MAIN3_DATA1_HIGH); 
    }
    
    // ���ڱ����ϲ� ����
    if(index == 0)
    {
        Gui_PageClearBackground(0, 
                                icon_16_main3_time_coord[0][1] + ICON_MAIN3_DATA1_HIGH, 
                                MAX_X_SIZE, 
                                icon_16_main3_bg_coord[0][1] - (icon_16_main3_time_coord[0][1] + ICON_MAIN3_DATA1_HIGH)); 
    }
    
    // ����ͼƬ
    if(index == 0)
    {
        LCD_SetPosition(icon_16_main3_bg_coord[0][0], icon_16_main3_bg_coord[0][1], 
                        ICON_MAIN3_ICON_WIDE, ICON_MAIN3_ICON_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_main3_bg_addr[0][0], ICON_MAIN3_ICON_SIZE);
    }
    
    // ����
    // ������ 
    LCD_SetPosition(icon_16_main3_data_coord[0][0], icon_16_main3_data_coord[0][1], 
                    ICON_MAIN3_DATA2_WIDE, ICON_MAIN3_DATA2_HIGH);
    LCD_DriverWriteDataFromExtFlash(icon_16_main3_data_addr[0][month / 10 % 10], ICON_MAIN3_DATA2_SIZE);
    LCD_SetPosition(icon_16_main3_data_coord[1][0], icon_16_main3_data_coord[1][1], 
                    ICON_MAIN3_DATA2_WIDE, ICON_MAIN3_DATA2_HIGH);
    LCD_DriverWriteDataFromExtFlash(icon_16_main3_data_addr[0][month % 10], ICON_MAIN3_DATA2_SIZE);
    
    // ������
    LCD_SetPosition(icon_16_main3_data_coord[2][0], icon_16_main3_data_coord[2][1], 
                    ICON_MAIN3_DATA2_WIDE, ICON_MAIN3_DATA2_HIGH);
    LCD_DriverWriteDataFromExtFlash(icon_16_main3_data_addr[0][day / 10 % 10], ICON_MAIN3_DATA2_SIZE);
    LCD_SetPosition(icon_16_main3_data_coord[3][0], icon_16_main3_data_coord[3][1], 
                    ICON_MAIN3_DATA2_WIDE, ICON_MAIN3_DATA2_HIGH);
    LCD_DriverWriteDataFromExtFlash(icon_16_main3_data_addr[0][day % 10], ICON_MAIN3_DATA2_SIZE);
    
    // ���� ��
    LCD_SetPosition(icon_16_main3_datadot_coord[0][0], icon_16_main3_datadot_coord[0][1], 
                    ICON_MAIN3_DATA2DOT_WIDE, ICON_MAIN3_DATA2DOT_HIGH);
    LCD_DriverWriteDataFromExtFlash(icon_16_main3_datadot_addr[0][0], ICON_MAIN3_DATA2DOT_SIZE);
    
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
void Gui_PageMainStye04(uint16_t index, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second, uint8_t week, 
                        uint16_t am_pm, uint32_t step_cnt, uint8_t heart,
                        uint8_t ble_status, uint8_t charge, uint8_t battery_level, uint8_t menstrual,
                        uint16_t calories_cnt, uint32_t step_target, uint16_t sleep_time, uint8_t language)
{
	static uint8_t hour_save = 0;
	
	// ������ʾ 
	if(index == 0)
	{
		LCD_SetPosition(icon_16_main4_bg_coord[0][0], icon_16_main4_bg_coord[0][1], 
						ICON_MAIN4_BG_WIDE, ICON_MAIN4_BG_HIGH);
		LCD_DriverWriteDataFromExtFlash(icon_16_main4_bg_addr[0], ICON_MAIN4_BG_SIZE);
	}
	
	// ������ʾ
	{
		// ������ 
		LCD_SetPosition(icon_16_main4_data_coord[0][0], icon_16_main4_data_coord[0][1], 
						ICON_MAIN4_DATA1_WIDE, ICON_MAIN4_DATA1_HIGH);
		LCD_DriverWriteDataFromExtFlash(icon_16_main4_data_addr[0][month / 10 % 10], ICON_MAIN4_DATA1_SIZE);
		LCD_SetPosition(icon_16_main4_data_coord[1][0], icon_16_main4_data_coord[1][1], 
						ICON_MAIN4_DATA1_WIDE, ICON_MAIN4_DATA1_HIGH);
		LCD_DriverWriteDataFromExtFlash(icon_16_main4_data_addr[0][month % 10], ICON_MAIN4_DATA1_SIZE);
		
		// ������
		LCD_SetPosition(icon_16_main4_data_coord[2][0], icon_16_main4_data_coord[2][1], 
						ICON_MAIN4_DATA1_WIDE, ICON_MAIN4_DATA1_HIGH);
		LCD_DriverWriteDataFromExtFlash(icon_16_main4_data_addr[0][day / 10 % 10], ICON_MAIN4_DATA1_SIZE);
		LCD_SetPosition(icon_16_main4_data_coord[3][0], icon_16_main4_data_coord[3][1], 
						ICON_MAIN4_DATA1_WIDE, ICON_MAIN4_DATA1_HIGH);
		LCD_DriverWriteDataFromExtFlash(icon_16_main4_data_addr[0][day % 10], ICON_MAIN4_DATA1_SIZE);
		
		// ���� ��
		LCD_SetPosition(icon_16_main4_datadot_coord[0][0], icon_16_main4_datadot_coord[0][1], 
						ICON_MAIN4_DATA1DOT_WIDE, ICON_MAIN4_DATA1DOT_HIGH);
		LCD_DriverWriteDataFromExtFlash(icon_16_main4_datadot_addr[0][0], ICON_MAIN4_DATA1DOT_SIZE);
	}
	
	// ʱ��
	{
		// ����ʱ��--ʱ
		LCD_SetPosition(icon_16_main4_time_coord[0][0], icon_16_main4_time_coord[0][1],
						ICON_MAIN4_DATA2_WIDE, ICON_MAIN4_DATA2_HIGH);
		LCD_DriverWriteDataFromExtFlash(icon_16_main4_time_addr[0][(hour / 10) % 10], ICON_MAIN4_DATA2_SIZE);
		
		LCD_SetPosition(icon_16_main4_time_coord[1][0], icon_16_main4_time_coord[1][1],
						ICON_MAIN4_DATA2_WIDE, ICON_MAIN4_DATA2_HIGH);
		LCD_DriverWriteDataFromExtFlash(icon_16_main4_time_addr[0][hour % 10], ICON_MAIN4_DATA2_SIZE);
		
		// ����ʱ��--ð��    
		if(index == 0)
		{
			LCD_SetPosition(icon_16_main4_timedot_coord[0][0], icon_16_main4_timedot_coord[0][1],
							ICON_MAIN4_DATA2DOT_WIDE, ICON_MAIN4_DATA2DOT_HIGH);
			LCD_DriverWriteDataFromExtFlash(icon_16_main4_timedot_addr[0][0], ICON_MAIN4_DATA2DOT_SIZE);
		}
	
		// ����ʱ��--�� 
		LCD_SetPosition(icon_16_main4_time_coord[2][0], icon_16_main4_time_coord[2][1],
						ICON_MAIN4_DATA2_WIDE, ICON_MAIN4_DATA2_HIGH);
		LCD_DriverWriteDataFromExtFlash(icon_16_main4_time_addr[0][(minute / 10) % 10], ICON_MAIN4_DATA2_SIZE);
		
		LCD_SetPosition(icon_16_main4_time_coord[3][0], icon_16_main4_time_coord[3][1],
						ICON_MAIN4_DATA2_WIDE, ICON_MAIN4_DATA2_HIGH);
		LCD_DriverWriteDataFromExtFlash(icon_16_main4_time_addr[0][minute % 10], ICON_MAIN4_DATA2_SIZE); 
	}
	
	// ����
	{
		if(week)
		{
			week -= 1;
			week %= 7;
		}
		LCD_SetPosition(icon_16_main4_week_coord[0][0], icon_16_main4_week_coord[0][1],
						ICON_MAIN4_WEEK_WIDE, ICON_MAIN4_WEEK_HIGH);
		LCD_DriverWriteDataFromExtFlash(icon_16_main4_week_addr[0][language][week], ICON_MAIN4_WEEK_SIZE);
	}
	
	if(index == 0)
	{
		hour_save = hour;
		
		if(hour % 3 == 0)
		{
			if(hour % 12 == 0)
			{
				LCD_SetPosition(icon_16_main4_hour12_coord[0][0], icon_16_main4_hour12_coord[0][1],
								ICON_MAIN4_ICON4_WIDE, ICON_MAIN4_ICON4_HIGH);
				LCD_DriverWriteDataFromExtFlash(icon_16_main4_hour12_addr[0], ICON_MAIN4_ICON4_SIZE);
			}
			else if(hour % 12 == 3)
			{
				LCD_SetPosition(icon_16_main4_hour3_coord[0][0], icon_16_main4_hour3_coord[0][1],
								ICON_MAIN4_ICON1_WIDE, ICON_MAIN4_ICON1_HIGH);
				LCD_DriverWriteDataFromExtFlash(icon_16_main4_hour3_addr[0], ICON_MAIN4_ICON1_SIZE);
			}
			else if(hour % 12 == 6)
			{
				LCD_SetPosition(icon_16_main4_hour6_coord[0][0], icon_16_main4_hour6_coord[0][1],
								ICON_MAIN4_ICON2_WIDE, ICON_MAIN4_ICON2_HIGH);
				LCD_DriverWriteDataFromExtFlash(icon_16_main4_hour6_addr[0], ICON_MAIN4_ICON2_SIZE);
			}
			else if(hour % 12 == 9)
			{
				LCD_SetPosition(icon_16_main4_hour9_coord[0][0], icon_16_main4_hour9_coord[0][1],
								ICON_MAIN4_ICON3_WIDE, ICON_MAIN4_ICON3_HIGH);
				LCD_DriverWriteDataFromExtFlash(icon_16_main4_hour9_addr[0], ICON_MAIN4_ICON3_SIZE);
			}
		}
		else
		{
			uint32_t x = 0;
			
			x = (2 * (hour / 3) + (hour % 3) - 1) % 8;
			
			LCD_SetPosition(icon_16_main4_hourdot_coord[x][0], icon_16_main4_hourdot_coord[x][1],
							ICON_MAIN4_DIAN_WIDE, ICON_MAIN4_DIAN_HIGH);
			LCD_DriverWriteDataFromExtFlash(icon_16_main4_hourdot_addr[0], ICON_MAIN4_DIAN_SIZE);
		}
	}
	else if(hour_save != hour)
	{
		hour_save = hour;
		
		if(hour % 3 == 0)
		{
			if(hour % 12 == 0)
			{
				LCD_SetPosition(icon_16_main4_hourdot_coord[7][0], icon_16_main4_hourdot_coord[7][1],
								ICON_MAIN4_DIAN_WIDE, ICON_MAIN4_DIAN_HIGH);
				LCD_DriverWriteDataFromExtFlash(icon_16_main4_hourdot_addr[1], ICON_MAIN4_DIAN_SIZE);
				
				LCD_SetPosition(icon_16_main4_hour12_coord[0][0], icon_16_main4_hour12_coord[0][1],
								ICON_MAIN4_ICON4_WIDE, ICON_MAIN4_ICON4_HIGH);
				LCD_DriverWriteDataFromExtFlash(icon_16_main4_hour12_addr[0], ICON_MAIN4_ICON4_SIZE);
			}
			else if(hour % 12 == 3)
			{
				LCD_SetPosition(icon_16_main4_hourdot_coord[1][0], icon_16_main4_hourdot_coord[1][1],
								ICON_MAIN4_DIAN_WIDE, ICON_MAIN4_DIAN_HIGH);
				LCD_DriverWriteDataFromExtFlash(icon_16_main4_hourdot_addr[1], ICON_MAIN4_DIAN_SIZE);
				
				LCD_SetPosition(icon_16_main4_hour3_coord[0][0], icon_16_main4_hour3_coord[0][1],
								ICON_MAIN4_ICON1_WIDE, ICON_MAIN4_ICON1_HIGH);
				LCD_DriverWriteDataFromExtFlash(icon_16_main4_hour3_addr[0], ICON_MAIN4_ICON1_SIZE);
			}
			else if(hour % 12 == 6)
			{
				LCD_SetPosition(icon_16_main4_hourdot_coord[3][0], icon_16_main4_hourdot_coord[3][1],
								ICON_MAIN4_DIAN_WIDE, ICON_MAIN4_DIAN_HIGH);
				LCD_DriverWriteDataFromExtFlash(icon_16_main4_hourdot_addr[1], ICON_MAIN4_DIAN_SIZE);
				
				LCD_SetPosition(icon_16_main4_hour6_coord[0][0], icon_16_main4_hour6_coord[0][1],
								ICON_MAIN4_ICON2_WIDE, ICON_MAIN4_ICON2_HIGH);
				LCD_DriverWriteDataFromExtFlash(icon_16_main4_hour6_addr[0], ICON_MAIN4_ICON2_SIZE);
			}
			else if(hour % 12 == 9)
			{
				LCD_SetPosition(icon_16_main4_hourdot_coord[5][0], icon_16_main4_hourdot_coord[5][1],
								ICON_MAIN4_DIAN_WIDE, ICON_MAIN4_DIAN_HIGH);
				LCD_DriverWriteDataFromExtFlash(icon_16_main4_hourdot_addr[1], ICON_MAIN4_DIAN_SIZE);
				
				LCD_SetPosition(icon_16_main4_hour9_coord[0][0], icon_16_main4_hour9_coord[0][1],
								ICON_MAIN4_ICON3_WIDE, ICON_MAIN4_ICON3_HIGH);
				LCD_DriverWriteDataFromExtFlash(icon_16_main4_hour9_addr[0], ICON_MAIN4_ICON3_SIZE);
			}
		}
		else
		{
			if(hour % 3 == 2)
			{
				uint32_t x = 0;
				
				x = (2 * (hour / 3) + (hour % 3) - 1) % 8;
				
				LCD_SetPosition(icon_16_main4_hourdot_coord[x - 1][0], icon_16_main4_hourdot_coord[x - 1][1],
								ICON_MAIN4_DIAN_WIDE, ICON_MAIN4_DIAN_HIGH);
				LCD_DriverWriteDataFromExtFlash(icon_16_main4_hourdot_addr[1], ICON_MAIN4_DIAN_SIZE);
				
				LCD_SetPosition(icon_16_main4_hourdot_coord[x][0], icon_16_main4_hourdot_coord[x][1],
								ICON_MAIN4_DIAN_WIDE, ICON_MAIN4_DIAN_HIGH);
				LCD_DriverWriteDataFromExtFlash(icon_16_main4_hourdot_addr[0], ICON_MAIN4_DIAN_SIZE);
			}
			else if(hour % 3 == 1)
			{
				uint32_t x = 0;
			
				x = (2 * (hour / 3) + (hour % 3) - 1) % 8;
				
				LCD_SetPosition(icon_16_main4_hourdot_coord[x][0], icon_16_main4_hourdot_coord[x][1],
								ICON_MAIN4_DIAN_WIDE, ICON_MAIN4_DIAN_HIGH);
				LCD_DriverWriteDataFromExtFlash(icon_16_main4_hourdot_addr[0], ICON_MAIN4_DIAN_SIZE);
				
				if(hour)
				{
					hour -= 1;
				}
				
				if(hour % 12 == 0)
				{
					LCD_SetPosition(icon_16_main4_hour12_coord[0][0], icon_16_main4_hour12_coord[0][1],
									ICON_MAIN4_ICON4_WIDE, ICON_MAIN4_ICON4_HIGH);
					LCD_DriverWriteDataFromExtFlash(icon_16_main4_hour12_addr[1], ICON_MAIN4_ICON4_SIZE);
				}
				else if(hour % 12 == 3)
				{
					LCD_SetPosition(icon_16_main4_hour3_coord[0][0], icon_16_main4_hour3_coord[0][1],
									ICON_MAIN4_ICON1_WIDE, ICON_MAIN4_ICON1_HIGH);
					LCD_DriverWriteDataFromExtFlash(icon_16_main4_hour3_addr[1], ICON_MAIN4_ICON1_SIZE);
				}
				else if(hour % 12 == 6)
				{
					LCD_SetPosition(icon_16_main4_hour6_coord[0][0], icon_16_main4_hour6_coord[0][1],
									ICON_MAIN4_ICON2_WIDE, ICON_MAIN4_ICON2_HIGH);
					LCD_DriverWriteDataFromExtFlash(icon_16_main4_hour6_addr[1], ICON_MAIN4_ICON2_SIZE);
				}
				else if(hour % 12 == 9)
				{
					LCD_SetPosition(icon_16_main4_hour9_coord[0][0], icon_16_main4_hour9_coord[0][1],
									ICON_MAIN4_ICON3_WIDE, ICON_MAIN4_ICON3_HIGH);
					LCD_DriverWriteDataFromExtFlash(icon_16_main4_hour9_addr[1], ICON_MAIN4_ICON3_SIZE);
				}
			}
		}
	}
		
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
void Gui_PageMainStye05(uint16_t index, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second, uint8_t week, 
                        uint16_t am_pm, uint32_t step_cnt, uint8_t heart,
                        uint8_t ble_status, uint8_t charge, uint8_t battery_level, uint8_t menstrual,
                        uint16_t calories_cnt, uint32_t step_target, uint16_t sleep_time, uint8_t language)
{    
    
    Gui_PageMainAssistant( index, week, language, am_pm, ble_status, charge, battery_level, menstrual, icon_16_main5_data_coord[0][1]);
	
	// ����������
	if(index == 0)
    {
        Gui_PageClearBackground(0, 
                                icon_16_main5_data_coord[0][1], 
                                icon_16_main5_data_coord[0][0], 
								ICON_MAIN5_DATA1_HIGH);
	}
	
	// ������ʾ
	{
		// ������ 
		LCD_SetPosition(icon_16_main5_data_coord[0][0], icon_16_main5_data_coord[0][1], 
						ICON_MAIN5_DATA1_WIDE, ICON_MAIN5_DATA1_HIGH);
		LCD_DriverWriteDataFromExtFlash(icon_16_main5_data_addr[0][month / 10 % 10], ICON_MAIN5_DATA1_SIZE);
		LCD_SetPosition(icon_16_main5_data_coord[1][0], icon_16_main5_data_coord[1][1], 
						ICON_MAIN5_DATA1_WIDE, ICON_MAIN5_DATA1_HIGH);
		LCD_DriverWriteDataFromExtFlash(icon_16_main5_data_addr[0][month % 10], ICON_MAIN5_DATA1_SIZE);
		
		// ������
		LCD_SetPosition(icon_16_main5_data_coord[2][0], icon_16_main5_data_coord[2][1], 
						ICON_MAIN5_DATA1_WIDE, ICON_MAIN5_DATA1_HIGH);
		LCD_DriverWriteDataFromExtFlash(icon_16_main5_data_addr[0][day / 10 % 10], ICON_MAIN5_DATA1_SIZE);
		LCD_SetPosition(icon_16_main5_data_coord[3][0], icon_16_main5_data_coord[3][1], 
						ICON_MAIN5_DATA1_WIDE, ICON_MAIN5_DATA1_HIGH);
		LCD_DriverWriteDataFromExtFlash(icon_16_main5_data_addr[0][day % 10], ICON_MAIN5_DATA1_SIZE);
		
		// ���� ��
		LCD_SetPosition(icon_16_main5_datadot_coord[0][0], icon_16_main5_datadot_coord[0][1], 
						ICON_MAIN5_DATA1DOT_WIDE, ICON_MAIN5_DATA1DOT_HIGH);
		LCD_DriverWriteDataFromExtFlash(icon_16_main5_datadot_addr[0][0], ICON_MAIN5_DATA1DOT_SIZE);
	}
	
	// �����Ҳ�����
	if(index == 0)
    {
        Gui_PageClearBackground(icon_16_main5_data_coord[3][0] + ICON_MAIN5_DATA1_WIDE, 
                                icon_16_main5_data_coord[0][1], 
                                MAX_X_SIZE - (icon_16_main5_data_coord[3][0] + ICON_MAIN5_DATA1_WIDE), 
								ICON_MAIN5_DATA1_HIGH);
	}
	
	// ʱ���ϲ���-����
	if(index == 0)
    {
        Gui_PageClearBackground(0, 
                                icon_16_main5_data_coord[0][1] + ICON_MAIN5_DATA1_HIGH, 
                                MAX_X_SIZE, 
								icon_16_main5_time_coord[0][1] - (icon_16_main5_data_coord[0][1] + ICON_MAIN5_DATA1_HIGH));
		
		Gui_PageClearBackground(0, 
                                icon_16_main5_time_coord[0][1], 
                                icon_16_main5_time_coord[0][0], 
								ICON_MAIN5_DATA2_HIGH);
	}
	
	// ʱ��
	{
		// ����ʱ��--ʱ
		LCD_SetPosition(icon_16_main5_time_coord[0][0], icon_16_main5_time_coord[0][1],
						ICON_MAIN5_DATA2_WIDE, ICON_MAIN5_DATA2_HIGH);
		LCD_DriverWriteDataFromExtFlash(icon_16_main5_time_addr[0][(hour / 10) % 10], ICON_MAIN5_DATA2_SIZE);
		
		LCD_SetPosition(icon_16_main5_time_coord[1][0], icon_16_main5_time_coord[1][1],
						ICON_MAIN5_DATA2_WIDE, ICON_MAIN5_DATA2_HIGH);
		LCD_DriverWriteDataFromExtFlash(icon_16_main5_time_addr[0][hour % 10], ICON_MAIN5_DATA2_SIZE);
		
		// Сʱ�Ҳ�-����
		if(index == 0)
		{
			Gui_PageClearBackground(icon_16_main5_time_coord[1][0] + ICON_MAIN5_DATA2_WIDE, 
									icon_16_main5_time_coord[0][1], 
									icon_16_main5_timedot_coord[0][0] - (icon_16_main5_time_coord[1][0] + ICON_MAIN5_DATA2_WIDE), 
									ICON_MAIN5_DATA2_HIGH);
		}
		
		// ����ʱ��--ð��    
		if(index == 0)
		{
			LCD_SetPosition(icon_16_main5_timedot_coord[0][0], icon_16_main5_timedot_coord[0][1],
							ICON_MAIN5_DATA2DOT_WIDE, ICON_MAIN5_DATA2DOT_HIGH);
			LCD_DriverWriteDataFromExtFlash(icon_16_main5_timedot_addr[0][0], ICON_MAIN5_DATA2DOT_SIZE);
		}
		
		// ������-����
		if(index == 0)
		{
			Gui_PageClearBackground(icon_16_main5_timedot_coord[0][0] + ICON_MAIN5_DATA2DOT_WIDE, 
									icon_16_main5_time_coord[2][1], 
									icon_16_main5_time_coord[2][0] - (icon_16_main5_timedot_coord[0][0] + ICON_MAIN5_DATA2DOT_WIDE), 
									ICON_MAIN5_DATA2_HIGH);
		}
	
		// ����ʱ��--�� 
		LCD_SetPosition(icon_16_main5_time_coord[2][0], icon_16_main5_time_coord[2][1],
						ICON_MAIN5_DATA2_WIDE, ICON_MAIN5_DATA2_HIGH);
		LCD_DriverWriteDataFromExtFlash(icon_16_main5_time_addr[0][(minute / 10) % 10], ICON_MAIN5_DATA2_SIZE);
		
		LCD_SetPosition(icon_16_main5_time_coord[3][0], icon_16_main5_time_coord[3][1],
						ICON_MAIN5_DATA2_WIDE, ICON_MAIN5_DATA2_HIGH);
		LCD_DriverWriteDataFromExtFlash(icon_16_main5_time_addr[0][minute % 10], ICON_MAIN5_DATA2_SIZE); 
		
		// �����Ҳ�-����
		if(index == 0)
		{
			Gui_PageClearBackground(icon_16_main5_time_coord[3][0] + ICON_MAIN5_DATA2_WIDE, 
									icon_16_main5_time_coord[2][1], 
									MAX_X_SIZE - (icon_16_main5_time_coord[3][0] + ICON_MAIN5_DATA2_WIDE), 
									ICON_MAIN5_DATA2_HIGH);
		}
	}
	
	// ������ʾ
	if(index == 0)
	{
		LCD_SetPosition(icon_16_main5_bg0_coord[0][0], icon_16_main5_bg0_coord[0][1],
						ICON_MAIN5_ICON6_WIDE, ICON_MAIN5_ICON6_HIGH);
		LCD_DriverWriteDataFromExtFlash(icon_16_main5_bg0_addr[0][0], ICON_MAIN5_ICON6_SIZE); 
		
		LCD_SetPosition(icon_16_main5_bg1_coord[0][0], icon_16_main5_bg1_coord[0][1],
						ICON_MAIN5_ICON5_WIDE, ICON_MAIN5_ICON5_HIGH);
		LCD_DriverWriteDataFromExtFlash(icon_16_main5_bg1_addr[0][0], ICON_MAIN5_ICON5_SIZE);
	}
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
void Gui_PageMain(uint16_t index, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second, uint8_t week, 
                  uint16_t am_pm, uint32_t step_cnt, uint8_t heart,
                  uint8_t ble_status, uint8_t charge, uint8_t battery_level, uint8_t menstrual,
                  uint16_t calories_cnt, uint32_t step_target, uint16_t sleep_time, uint8_t language)
{
// ���Կ�ʼ	
//	uint8_t heart_i[48] ={50,60,70};
	
//	Gui_PageMainStye05(index, index, index, index, index, index, (index + 1) % 8, 
//					  index % 4, index, index,
//					  index % 2, 1, index % 5, index % 5,
//					  index, 20, index * 60, index > 7 ); 
//	Gui_PageStepDistanceCalories(index, index, index * 3,
//                                  index,index > 10,
//                                  index, index > 5);
//	Gui_PageRunModeData(index, index, index, 0, index, index, 
//                                    index, index, index, index, index > 10,index > 5);
//	Gui_PageRunModePause(index,1,index > 2);
//	Gui_PageRunModeExit(index, 0);
//	Gui_PageBP(index, index, index, 1, 0, index);
//	Gui_PageHeart(0, index, index, 
//                   heart_i, 62 + index * 10, 30 + index*10, index > 5);
	
//	Gui_PageSleep(0, index, index, 
//                   index, index, index > 5);
	
//	Gui_PageFindPhone(1, index, index > 5);
	
//	GUI_StopWatchPage(index, 1,index > 4, index % 10 > 5, index, index,index, index, index > 5);
	
//	Gui_PageCountDown(index, 1, index % 10 > 5, index , index > 5);
//	Gui_PageLowPower(index);
//	Gui_PageBatteryCharg(index, 0, 4);
//	Gui_PageAlarm(0, index % 4, index,index, index, index > 5);
//	Gui_PageSit(index, index > 5);
//	Gui_PageCallNotificationIcon(0, (index+1) % 4, index > 8);
//	Gui_PageMessageNotificationIcon(0, GUI_MSG_TYPE_MESSAGE + index %(GUI_MSG_TYPE_PHYS + 1) , 0, index, index , 0);
//	Gui_PageHeartAlert(index);
//	Gui_PageCameraReady(index);
//	Gui_PageCameraOk(index);
//	Gui_PageStarget( index, index, 1);
//	Gui_PageBleDiscRemind(index);
//	Gui_PagePowerOff(index);
//	Gui_PagePowerOn(index);

//	return;
	
// ���Խ���
	
    if(ui_style_type == 0)
    {
        Gui_PageMainStye01(index, month, day, hour, minute, second, week, 
                          am_pm, step_cnt, heart,
                          ble_status, charge, battery_level, menstrual,
                          calories_cnt, step_target, sleep_time, language);  
    }
    else if(ui_style_type == 1)
    {
        Gui_PageMainStye02(index, month, day, hour, minute, second, week, 
                          am_pm, step_cnt, heart,
                          ble_status, charge, battery_level, menstrual,
                          calories_cnt, step_target, sleep_time, language);        
    }
    else if(ui_style_type == 2)
    {
        Gui_PageMainStye03(index, month, day, hour, minute, second, week, 
                          am_pm, step_cnt, heart,
                          ble_status, charge, battery_level, menstrual,
                          calories_cnt, step_target, sleep_time, language);        
    } 
	else if(ui_style_type == 3)
    {
        Gui_PageMainStye04(index, month, day, hour, minute, second, week, 
                          am_pm, step_cnt, heart,
                          ble_status, charge, battery_level, menstrual,
                          calories_cnt, step_target, sleep_time, language);        
    } 
	else if(ui_style_type == 4)
    {
        Gui_PageMainStye05(index, month, day, hour, minute, second, week, 
                          am_pm, step_cnt, heart,
                          ble_status, charge, battery_level, menstrual,
                          calories_cnt, step_target, sleep_time, language);        
    } 
}

/****************************************************************
** Function name:           Gui_PageStepDistanceCalories
** Descriptions:            �Ʋ����뿨·�Ｏ�Ͻ���
** input parameters:        index����̬ͼ��ļ���
**                          step_cnt������
**                          step_prg��Ŀ��
**                          language: ����
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2018-05-24
*****************************************************************/  
void Gui_PageStepDistanceCalories(uint16_t index, uint32_t step_cnt, uint8_t step_prg,
                                  float distance_cnt,uint8_t uint_type,
                                  uint16_t calories_cnt, uint8_t language)
{   
    if(language)
    {
        language = 1;
    }
    
    // �Ʋ�ͼ���ϲ�����
    if(index == 0)
    {
        Gui_PageClearBackground(0, 
                                0, 
                                MAX_X_SIZE, 
                                icon_16_scd_step_bg_coord[0][1]); 
    }
    
    // �Ʋ���ͼ������
    if(index == 0)
    {
        Gui_PageClearBackground(0, 
                                icon_16_scd_step_bg_coord[0][1], 
                                icon_16_scd_step_bg_coord[0][0], 
                                ICON_STEP_ICON2_HIGH);
    }
    
    // �Ʋ���ͼ����
    if(index == 0)
    {
        LCD_SetPosition(icon_16_scd_step_bg_coord[0][0], icon_16_scd_step_bg_coord[0][1], 
                        ICON_STEP_ICON2_WIDE, ICON_STEP_ICON2_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_scd_step_bg_addr[0], ICON_STEP_ICON2_SIZE);
    }
    
    // �Ʋ���ͼ
    {
        LCD_SetPosition(icon_16_scd_step_icon_coord[0][0], icon_16_scd_step_icon_coord[0][1], 
                        ICON_STEP_ICON1_WIDE, ICON_STEP_ICON1_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_scd_step_icon_addr[index % 8], ICON_STEP_ICON1_SIZE);
    }
    
    //WJ.20180810.�ӿ춯̬ͼ��ˢ���ٶ�
    if(index % 6 != 0)
    {
        return;
    }
    
    // �Ʋ���ͼ������
    if(index == 0)
    {      
        Gui_PageClearBackground(icon_16_scd_step_bg_coord[0][0] + ICON_STEP_ICON2_WIDE, 
                                icon_16_scd_step_bg_coord[0][1], 
                                MAX_X_SIZE - (icon_16_scd_step_bg_coord[0][0] + ICON_STEP_ICON2_WIDE), 
                                ICON_STEP_ICON2_HIGH); 
    }
        
    // �Ʋ���ֵ
    {
        if(step_cnt > 99999)
        {
            step_cnt = 99999;
        }

        LCD_SetPosition(icon_16_scd_step_num_coord[0][0], icon_16_scd_step_num_coord[0][1], 
                        ICON_STEP_DATA1_WIDE, ICON_STEP_DATA1_HIGH);
        if(step_cnt >= 10000)
        {    
            LCD_DriverWriteDataFromExtFlash(icon_16_scd_step_num_addr[(step_cnt / 10000) %10], ICON_STEP_DATA1_SIZE);        
        }  
        else
        {    
            LCD_DriverWriteDataFromExtFlash(icon_16_scd_step_num_addr[0], ICON_STEP_DATA1_SIZE);        
        } 
        
        LCD_SetPosition(icon_16_scd_step_num_coord[1][0], icon_16_scd_step_num_coord[1][1], 
                        ICON_STEP_DATA1_WIDE, ICON_STEP_DATA1_HIGH);
        if(step_cnt >= 1000)
        {    
            LCD_DriverWriteDataFromExtFlash(icon_16_scd_step_num_addr[(step_cnt / 1000) %10], ICON_STEP_DATA1_SIZE);        
        }  
        else
        {    
            LCD_DriverWriteDataFromExtFlash(icon_16_scd_step_num_addr[0], ICON_STEP_DATA1_SIZE);        
        }
        
        LCD_SetPosition(icon_16_scd_step_num_coord[2][0], icon_16_scd_step_num_coord[2][1], 
                        ICON_STEP_DATA1_WIDE, ICON_STEP_DATA1_HIGH);;
        if(step_cnt >= 100)
        {    
            LCD_DriverWriteDataFromExtFlash(icon_16_scd_step_num_addr[(step_cnt / 100) %10], ICON_STEP_DATA1_SIZE);        
        }  
        else
        {    
            LCD_DriverWriteDataFromExtFlash(icon_16_scd_step_num_addr[0], ICON_STEP_DATA1_SIZE);        
        }
        LCD_SetPosition(icon_16_scd_step_num_coord[3][0], icon_16_scd_step_num_coord[3][1], 
                        ICON_STEP_DATA1_WIDE, ICON_STEP_DATA1_HIGH);
        if(step_cnt >= 10)
        {    
            LCD_DriverWriteDataFromExtFlash(icon_16_scd_step_num_addr[(step_cnt / 10) %10], ICON_STEP_DATA1_SIZE);        
        }  
        else
        {    
            LCD_DriverWriteDataFromExtFlash(icon_16_scd_step_num_addr[0], ICON_STEP_DATA1_SIZE);        
        }
        
        LCD_SetPosition(icon_16_scd_step_num_coord[4][0], icon_16_scd_step_num_coord[4][1], 
                        ICON_STEP_DATA1_WIDE, ICON_STEP_DATA1_HIGH);
        if(step_cnt > 0)
        {    
            LCD_DriverWriteDataFromExtFlash(icon_16_scd_step_num_addr[step_cnt %10], ICON_STEP_DATA1_SIZE);        
        }  
        else
        {    
            LCD_DriverWriteDataFromExtFlash(icon_16_scd_step_num_addr[0], ICON_STEP_DATA1_SIZE);        
        }
    }
    
    // �Ʋ���λ��������
    if(index == 0)
    {
        Gui_PageClearBackground(0, 
                                icon_16_scd_step_bg_coord[0][1] + ICON_STEP_ICON2_HIGH, 
                                MAX_X_SIZE, 
                                icon_16_scd_progress_icon_coord[0][1] - 
                                (icon_16_scd_step_bg_coord[0][1] + ICON_STEP_ICON2_HIGH)); 
    }
    
    // �Ʋ���λ
    if((index == 0)||(language_save != language))
    {
        LCD_SetPosition(icon_16_scd_step_unit_coord[0][0], icon_16_scd_step_unit_coord[0][1], 
                        ICON_STEP_UNIT1_WIDE, ICON_STEP_UNIT1_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_scd_step_unit_addr[language % 2], ICON_STEP_UNIT1_SIZE);
    }
    
    // ������������
    if(index == 0)
    {
        Gui_PageClearBackground(0, 
                                icon_16_scd_progress_icon_coord[0][1], 
                                icon_16_scd_progress_icon_coord[0][0], 
                                ICON_STEP_ICON3_HIGH); 
    }
    
    // ��������ʾ
    if(index == 0)
    {
		uint8_t progress = 0;
		
		progress = step_prg;
        if(progress)
        {
            progress /= 4;
        }
        
        if(progress >= 25)
        {
            progress = 25;
        }
        
        LCD_SetPosition(icon_16_scd_progress_icon_coord[0][0], icon_16_scd_progress_icon_coord[0][1], 
                        ICON_STEP_ICON3_WIDE, ICON_STEP_ICON3_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_scd_progress_icon_addr[progress], ICON_STEP_ICON3_SIZE);
    }
    
    // ������������
    if(index == 0)
    {
        Gui_PageClearBackground(icon_16_scd_progress_icon_coord[0][0] + ICON_STEP_ICON3_WIDE, 
                                icon_16_scd_progress_icon_coord[0][1], 
                                MAX_X_SIZE - (icon_16_scd_progress_icon_coord[0][0] + ICON_STEP_ICON3_WIDE), 
                                ICON_STEP_ICON3_HIGH); 
    }
    
    // �Ʋ�-�������ȱ��ﱳ������
    if((index == 0)||(language_save != language))
    {
        Gui_PageClearBackground(0, 
                                icon_16_scd_progress_icon_coord[0][1] + ICON_STEP_ICON3_HIGH, 
                                MAX_X_SIZE, 
                                (icon_16_scd_progress_label_coord[0][1] + ICON_STEP_ICON4_HIGH) - 
                                (icon_16_scd_progress_icon_coord[0][1] + ICON_STEP_ICON3_HIGH));
    }
    
    // �Ʋ�-���ȱ�����ʾ
    if((index == 0)||(language_save != language))
    {
		if(language == 0)
		{
			LCD_SetPosition(icon_16_scd_progress_label_coord[language % 2][0], icon_16_scd_progress_label_coord[language % 2][1], 
							ICON_STEP_ICON4_WIDE, ICON_STEP_ICON4_HIGH);
			LCD_DriverWriteDataFromExtFlash(icon_16_scd_progress_label_addr[0], ICON_STEP_ICON4_SIZE);
		}
		else
		{
			LCD_SetPosition(icon_16_scd_progress_label_coord[language % 2][0], icon_16_scd_progress_label_coord[language % 2][1], 
							ICON_STEP_ICON7_WIDE, ICON_STEP_ICON7_HIGH);
			LCD_DriverWriteDataFromExtFlash(icon_16_scd_progress_label_addr[1], ICON_STEP_ICON7_SIZE);
		}
    }
    
    // �ٷֱ���ʾ
    if((index == 0)||(language_save != language))
    {
        if(step_prg >= 100)
        {
            step_prg = 100;
        }
        
        if(step_prg >= 100)
        {
            LCD_SetPosition(icon_16_scd_progress_num_coord[language % 2][0][0][0], icon_16_scd_progress_num_coord[language % 2][0][0][1], 
                            ICON_STEP_DATA2_WIDE, ICON_STEP_DATA2_HIGH);
            LCD_DriverWriteDataFromExtFlash(icon_16_scd_progress_num_addr[(step_prg / 100) % 10], ICON_STEP_DATA2_SIZE);
            
            LCD_SetPosition(icon_16_scd_progress_num_coord[language % 2][0][1][0], icon_16_scd_progress_num_coord[language % 2][0][1][1], 
                            ICON_STEP_DATA2_WIDE, ICON_STEP_DATA2_HIGH);
            LCD_DriverWriteDataFromExtFlash(icon_16_scd_progress_num_addr[(step_prg / 10) % 10], ICON_STEP_DATA2_SIZE);
            
            LCD_SetPosition(icon_16_scd_progress_num_coord[language % 2][0][2][0], icon_16_scd_progress_num_coord[language % 2][0][2][1], 
                            ICON_STEP_DATA2_WIDE, ICON_STEP_DATA2_HIGH);
            LCD_DriverWriteDataFromExtFlash(icon_16_scd_progress_num_addr[step_prg % 10], ICON_STEP_DATA2_SIZE);
        }
        else if(step_prg >= 10)
        {
            LCD_SetPosition(icon_16_scd_progress_num_coord[language % 2][1][0][0], icon_16_scd_progress_num_coord[language % 2][1][0][1], 
                            ICON_STEP_DATA2_WIDE, ICON_STEP_DATA2_HIGH);
            LCD_DriverWriteDataFromExtFlash(icon_16_scd_progress_num_addr[(step_prg / 10) % 10], ICON_STEP_DATA2_SIZE);
            
            LCD_SetPosition(icon_16_scd_progress_num_coord[language % 2][1][1][0], icon_16_scd_progress_num_coord[language % 2][1][1][1], 
                            ICON_STEP_DATA2_WIDE, ICON_STEP_DATA2_HIGH);
            LCD_DriverWriteDataFromExtFlash(icon_16_scd_progress_num_addr[step_prg % 10], ICON_STEP_DATA2_SIZE);
        }
        else
        {
            LCD_SetPosition(icon_16_scd_progress_num_coord[language % 2][2][0][0], icon_16_scd_progress_num_coord[language % 2][2][0][1], 
                            ICON_STEP_DATA2_WIDE, ICON_STEP_DATA2_HIGH);
            LCD_DriverWriteDataFromExtFlash(icon_16_scd_progress_num_addr[step_prg % 10], ICON_STEP_DATA2_SIZE);   
        }
        
        LCD_SetPosition(icon_16_scd_progress_percent_coord[language % 2][0], icon_16_scd_progress_percent_coord[language % 2][1], 
                            ICON_STEP_DATA2DOT_WIDE, ICON_STEP_DATA2DOT_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_scd_progress_percent_addr[0], ICON_STEP_DATA2DOT_SIZE); 
    }
    
    // �Ʋ�-��·�ﱳ������
    if(index == 0)
    {
        Gui_PageClearBackground(0, 
                                icon_16_scd_progress_label_coord[0][1] + ICON_STEP_ICON4_HIGH, 
                                MAX_X_SIZE, 
                                (icon_16_scd_calories_icon_coord[0][1] + ICON_STEP_ICON5_HIGH) - 
                                (icon_16_scd_progress_label_coord[0][1] + ICON_STEP_ICON4_HIGH)); 
    }
    
    // ��·��ͼ��
    if(index == 0)
    {
        LCD_SetPosition(icon_16_scd_calories_icon_coord[0][0], icon_16_scd_calories_icon_coord[0][1], 
                        ICON_STEP_ICON5_WIDE, ICON_STEP_ICON5_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_scd_calories_icon_addr[0], ICON_STEP_ICON5_SIZE);
    }
    
    // ��·����ֵ
    {
        if(calories_cnt > 9999)
        {
            calories_cnt = 9999;
        }

        LCD_SetPosition(icon_16_scd_calories_num_coord[0][0], icon_16_scd_calories_num_coord[1][1], 
                        ICON_STEP_DATA3_WIDE, ICON_STEP_DATA3_HIGH);
        if(calories_cnt >= 1000)
        {    
            LCD_DriverWriteDataFromExtFlash(icon_16_scd_calories_num_addr[(calories_cnt / 1000) %10], ICON_STEP_DATA3_SIZE);        
        }  
        else
        {    
            LCD_DriverWriteDataFromExtFlash(icon_16_scd_calories_num_addr[0], ICON_STEP_DATA3_SIZE);        
        }
        
        LCD_SetPosition(icon_16_scd_calories_num_coord[1][0], icon_16_scd_calories_num_coord[1][1], 
                        ICON_STEP_DATA3_WIDE, ICON_STEP_DATA3_HIGH);
        if(calories_cnt >= 100)
        {    
            LCD_DriverWriteDataFromExtFlash(icon_16_scd_calories_num_addr[(calories_cnt / 100) %10], ICON_STEP_DATA3_SIZE);        
        }  
        else
        {    
            LCD_DriverWriteDataFromExtFlash(icon_16_scd_calories_num_addr[0], ICON_STEP_DATA3_SIZE);        
        }
        
        LCD_SetPosition(icon_16_scd_calories_num_coord[2][0], icon_16_scd_calories_num_coord[2][1], 
                        ICON_STEP_DATA3_WIDE, ICON_STEP_DATA3_HIGH);
        if(calories_cnt >= 10)
        {    
            LCD_DriverWriteDataFromExtFlash(icon_16_scd_calories_num_addr[(calories_cnt / 10) %10], ICON_STEP_DATA3_SIZE);        
        }  
        else
        {    
            LCD_DriverWriteDataFromExtFlash(icon_16_scd_calories_num_addr[0], ICON_STEP_DATA3_SIZE);        
        }
        
        LCD_SetPosition(icon_16_scd_calories_num_coord[3][0], icon_16_scd_calories_num_coord[3][1], 
                        ICON_STEP_DATA3_WIDE, ICON_STEP_DATA3_HIGH);
        if(calories_cnt > 0)
        {    
            LCD_DriverWriteDataFromExtFlash(icon_16_scd_calories_num_addr[calories_cnt %10], ICON_STEP_DATA3_SIZE);        
        }  
        else
        {    
            LCD_DriverWriteDataFromExtFlash(icon_16_scd_calories_num_addr[0], ICON_STEP_DATA3_SIZE);        
        }               
    }
    
    // ��·�ﵥλ
    if((index == 0)||(language_save != language))
    {
        LCD_SetPosition(icon_16_scd_calories_unit_coord[0][0], icon_16_scd_calories_unit_coord[0][1], 
                        ICON_STEP_UNIT2_WIDE, ICON_STEP_UNIT2_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_scd_calories_unit_addr[language % 2], ICON_STEP_UNIT2_SIZE);
    }
    
    // ���뱳������
    if(index == 0)
    {
        Gui_PageClearBackground(0, 
                                icon_16_scd_calories_icon_coord[0][1] + ICON_STEP_ICON5_HIGH, 
                                MAX_X_SIZE, 
                                (icon_16_scd_distance_icon_coord[0][1] + ICON_STEP_ICON6_HIGH) - 
                                (icon_16_scd_calories_icon_coord[0][1] + ICON_STEP_ICON5_HIGH));
    }
    
    // ����ͼ����ʾ
    if(index == 0)
    {
        LCD_SetPosition(icon_16_scd_distance_icon_coord[0][0], icon_16_scd_distance_icon_coord[0][1], 
                        ICON_STEP_ICON6_WIDE, ICON_STEP_ICON6_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_scd_distance_icon_addr[0], ICON_STEP_ICON6_SIZE);
    }
    
    // ������ֵ
    {
        uint32_t dis_count_tmp = 0;
        
        dis_count_tmp = (uint32_t)(distance_cnt* 10);
        if(dis_count_tmp > 999)
        {
            dis_count_tmp = 999;
        }
        
        LCD_SetPosition(icon_16_scd_distance_num_coord[0][0], icon_16_scd_distance_num_coord[0][1], 
                        ICON_STEP_DATA4_WIDE, ICON_STEP_DATA4_HIGH);
        if(dis_count_tmp >= 100)
        {    
            LCD_DriverWriteDataFromExtFlash(icon_16_scd_distance_num_addr[(dis_count_tmp / 100) %10], ICON_STEP_DATA4_SIZE);        
        }  
        else
        {    
            LCD_DriverWriteDataFromExtFlash(icon_16_scd_distance_num_addr[0], ICON_STEP_DATA4_SIZE);        
        }
        
        LCD_SetPosition(icon_16_scd_distance_num_coord[1][0], icon_16_scd_distance_num_coord[1][1], 
                        ICON_STEP_DATA4_WIDE, ICON_STEP_DATA4_HIGH);
        if(dis_count_tmp >= 10)
        {    
            LCD_DriverWriteDataFromExtFlash(icon_16_scd_distance_num_addr[(dis_count_tmp / 10) %10], ICON_STEP_DATA4_SIZE);        
        }  
        else
        {    
            LCD_DriverWriteDataFromExtFlash(icon_16_scd_distance_num_addr[0], ICON_STEP_DATA4_SIZE);        
        }
        
        // ��������-��
        LCD_SetPosition(icon_16_scd_distance_dot_coord[0][0], icon_16_scd_distance_dot_coord[0][1], 
                        ICON_STEP_DATA4DOT_WIDE, ICON_STEP_DATA4DOT_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_scd_distance_dot_addr[0], ICON_STEP_DATA4DOT_SIZE);  
        

        LCD_SetPosition(icon_16_scd_distance_num_coord[2][0], icon_16_scd_distance_num_coord[2][1], 
                        ICON_STEP_DATA4_WIDE, ICON_STEP_DATA4_HIGH);
        if(dis_count_tmp > 0)
        {    
            LCD_DriverWriteDataFromExtFlash(icon_16_scd_distance_num_addr[dis_count_tmp %10], ICON_STEP_DATA4_SIZE);        
        }  
        else
        {    
            LCD_DriverWriteDataFromExtFlash(icon_16_scd_distance_num_addr[0], ICON_STEP_DATA4_SIZE);        
        }                    
    }
    
    // ���뵥λ
    if((index == 0)||(language_save != language))
    {
        if(language == 0)
        {
            LCD_SetPosition(icon_16_scd_distance_unit_coord[0][0], icon_16_scd_distance_unit_coord[0][1], 
                            ICON_STEP_UNIT3_WIDE, ICON_STEP_UNIT3_HIGH);
            LCD_DriverWriteDataFromExtFlash(icon_16_scd_distance_unit_addr[0], ICON_STEP_UNIT3_SIZE);
        }
        else if(uint_type == 1)
        {
            LCD_SetPosition(icon_16_scd_distance_unit_coord[0][0], icon_16_scd_distance_unit_coord[0][1], 
                            ICON_STEP_UNIT3_WIDE, ICON_STEP_UNIT3_HIGH);
            LCD_DriverWriteDataFromExtFlash(icon_16_scd_distance_unit_addr[2], ICON_STEP_UNIT3_SIZE);
        }
        else
        {
            LCD_SetPosition(icon_16_scd_distance_unit_coord[0][0], icon_16_scd_distance_unit_coord[0][1], 
                            ICON_STEP_UNIT3_WIDE, ICON_STEP_UNIT3_HIGH);
            LCD_DriverWriteDataFromExtFlash(icon_16_scd_distance_unit_addr[1], ICON_STEP_UNIT3_SIZE);
        }
    }
    
    // ��Ļ�ײ�����
    if(index == 0)
    {
        Gui_PageClearBackground(0, 
                                icon_16_scd_distance_icon_coord[0][1] + ICON_STEP_ICON6_HIGH, 
                                MAX_X_SIZE, 
                                MAX_Y_SIZE - (icon_16_scd_distance_unit_coord[0][1] + ICON_STEP_UNIT3_HIGH));
    }
    
    language_save = language;
}


/****************************************************************
** Function name:           Gui_PageRunMode
** Descriptions:            ��ʾ�˶�ģʽ
** input parameters:        index����̬ͼ��ļ���
                            language: ���ԣ�0���ġ�1Ӣ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2018-05-24
*****************************************************************/ 
void Gui_PageRunMode(uint16_t index, uint8_t language)
{	
    if(language)
    {
        language = 1;
    }
    
    // ��ͼ�ϲ�����
    if(index == 0)
    {
        Gui_PageClearBackground(0, 
                                0, 
                                MAX_X_SIZE, 
                                icon_16_move_entry_man_coord[0][1]); 
    }
    
    // ��ͼ������
    if(index == 0)
    {
        Gui_PageClearBackground(0, 
                                icon_16_move_entry_man_coord[0][1], 
                                icon_16_move_entry_man_coord[0][0], 
                                ICON_MOVE_ICON1_HIGH); 
    }
    
    // ��ͼ��ʾ
    if(index == 0)
    {
        LCD_SetPosition(icon_16_move_entry_man_coord[0][0], icon_16_move_entry_man_coord[0][1], 
                        ICON_MOVE_ICON1_WIDE, ICON_MOVE_ICON1_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_move_entry_man_addr[0], ICON_MOVE_ICON1_SIZE);
    }
    
    // ��ͼ������
    if(index == 0)
    {       
        Gui_PageClearBackground(icon_16_move_entry_man_coord[0][0] + ICON_MOVE_ICON1_WIDE, 
                                icon_16_move_entry_man_coord[0][1], 
                                MAX_X_SIZE - (icon_16_move_entry_man_coord[0][0] + ICON_MOVE_ICON1_WIDE),
                                ICON_MOVE_ICON1_HIGH);
    }
    
    // ��λ�ϲ�����
    if(index == 0)
    {
        Gui_PageClearBackground(0, 
                                icon_16_move_entry_man_coord[0][1] + ICON_MOVE_ICON1_HIGH, 
                                MAX_X_SIZE,
                                icon_16_move_entry_unit_coord[0][1] - (icon_16_move_entry_man_coord[0][1] + ICON_MOVE_ICON1_HIGH));
    }
	
    // ������ʾ��
    {
        LCD_SetPosition(icon_16_long_press_icon_coord[0][0], icon_16_long_press_icon_coord[0][1], 
                        ICON_FINDPHONE_ICON5_WIDE, ICON_FINDPHONE_ICON5_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_long_press_icon_addr[language % 2], ICON_FINDPHONE_ICON5_SIZE);
    }
    
    // ��λ������
    if(index == 0)
    {
        Gui_PageClearBackground(0, 
                                icon_16_move_entry_unit_coord[0][1], 
                                icon_16_move_entry_unit_coord[0][0],
                                ICON_MOVE_ICON2_HIGH);
    }
    
    // ��λ��ʾ
    {
        LCD_SetPosition(icon_16_move_entry_unit_coord[0][0], icon_16_move_entry_unit_coord[0][1], 
                        ICON_MOVE_ICON2_WIDE, ICON_MOVE_ICON2_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_move_entry_unit_addr[language % 2], ICON_MOVE_ICON2_SIZE);
    }
    
    // ��λ������
    if(index == 0)
    {       
        Gui_PageClearBackground(icon_16_move_entry_unit_coord[0][0] + ICON_MOVE_ICON2_WIDE, 
                                icon_16_move_entry_unit_coord[0][1], 
                                MAX_X_SIZE - (icon_16_move_entry_unit_coord[0][0] + ICON_MOVE_ICON2_WIDE),
                                ICON_MOVE_ICON2_HIGH);
    }
    
    // ��Ļ�ײ�����
    if(index == 0)
    {
        Gui_PageClearBackground(0, 
                                icon_16_move_entry_unit_coord[0][1] + ICON_MOVE_ICON2_HIGH, 
                                MAX_X_SIZE,
                                MAX_Y_SIZE - (icon_16_move_entry_unit_coord[0][1] + ICON_MOVE_ICON2_HIGH));
    }
}

/****************************************************************
** Function name:           Gui_PageRunModeData
** Descriptions:            ��ʾ�˶�ģʽ���ݽ���
** input parameters:        seconds���˶�ʱ�䣻heart�����ʣ�calories����·�
                            distance�����룻step���Ʋ���
                            uint_type��0,km��1��mi
                            language: ���ԣ�0���ġ�1Ӣ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2018-05-24
**----------------------------------------------------------------
*****************************************************************/ 
void Gui_PageRunModeData(uint16_t index, uint32_t seconds, uint8_t heart, float distance, float calories, 
                         uint32_t step_cnt, uint8_t uint_type, bool lock, uint8_t hour, uint8_t minute, uint8_t animation_flag,uint8_t language)
{	
    static uint8_t last_language = 0xFF;
    static uint8_t last_heart = 0;
    static uint32_t last_seconds = 0;
//    static float last_distance = 0;
    static float last_calories = 0;
    static uint32_t last_step_cnt = 0;
    
    static uint16_t heart_index = 0;
    
	if((index == 0)||(animation_flag == 0))
	{
		animation_cnt = 0;
	}
	else
	{
		animation_cnt++;
	}
	
    if(language)
    {
        language = 1;
    }
    
    // ʱ���ϲ�����
    if(index == 0)
    {
        heart_index = 0;
        Gui_PageClearBackground(0, 
                                0, 
                                MAX_X_SIZE, 
                                icon_16_move_time_coord[0][1] + ICON_MOVE_DATA1_HIGH); 
    }
    
    // �˶�ģʽ-��ͼ
    {
        LCD_SetPosition(icon_16_move_icon_coord[0][0], icon_16_move_icon_coord[0][1], 
                        ICON_MOVE_ICON3_WIDE, ICON_MOVE_ICON3_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_move_icon_addr[animation_cnt % 9], ICON_MOVE_ICON3_SIZE);
    }
     // ���ʶ�ͼ
    {
        LCD_SetPosition(icon_16_move_heart_icon_coord[0][0], icon_16_move_heart_icon_coord[0][1], 
                        ICON_MOVE_ICON4_WIDE, ICON_MOVE_ICON4_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_move_heart_icon_addr[animation_cnt % 8], ICON_MOVE_ICON4_SIZE);
    }   
    
    if(index % 5 != 0)
    {
        return;
    }
   
    // ϵͳʱ��
    if(index % 10 == 0)
    {
        // ����ʱ��--ʱ
        LCD_SetPosition(icon_16_move_time_coord[0][0], icon_16_move_time_coord[0][1],
                        ICON_MOVE_DATA1_WIDE, ICON_MOVE_DATA1_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_move_time_addr[(hour / 10) % 10], ICON_MOVE_DATA1_SIZE);
        LCD_SetPosition(icon_16_move_time_coord[1][0], icon_16_move_time_coord[1][1],
                        ICON_MOVE_DATA1_WIDE, ICON_MOVE_DATA1_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_move_time_addr[hour % 10], ICON_MOVE_DATA1_SIZE);
        
        // ����ʱ��--�� 
        LCD_SetPosition(icon_16_move_time_coord[2][0], icon_16_move_time_coord[2][1],
                        ICON_MOVE_DATA1_WIDE, ICON_MOVE_DATA1_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_move_time_addr[(minute / 10) % 10], ICON_MOVE_DATA1_SIZE);
        LCD_SetPosition(icon_16_move_time_coord[3][0], icon_16_move_time_coord[3][1],
                        ICON_MOVE_DATA1_WIDE, ICON_MOVE_DATA1_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_move_time_addr[minute % 10], ICON_MOVE_DATA1_SIZE); 
        
        // ����ʱ��--ð��
        LCD_SetPosition(icon_16_move_time_dot_coord[0][0], icon_16_move_time_dot_coord[0][1],
                        ICON_MOVE_DATA1DOT_WIDE, ICON_MOVE_DATA1DOT_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_move_time_dot_addr[0], ICON_MOVE_DATA1DOT_SIZE);
    }
    
    // ��ͼ��������
    if(index == 0)
    {
        Gui_PageClearBackground(0, 
                                icon_16_move_time_coord[0][1] + ICON_MOVE_DATA1_HIGH, 
                                icon_16_move_icon_coord[0][0], 
                                (icon_16_move_icon_coord[0][1] + ICON_MOVE_ICON3_HIGH) - 
                                (icon_16_move_time_coord[0][1] + ICON_MOVE_DATA1_HIGH)); 
        
        Gui_PageClearBackground(icon_16_move_icon_coord[0][0] + ICON_MOVE_ICON3_WIDE, 
                                icon_16_move_time_coord[0][1] + ICON_MOVE_DATA1_HIGH, 
                                MAX_X_SIZE - (icon_16_move_icon_coord[0][0] + ICON_MOVE_ICON3_WIDE), 
                                (icon_16_move_icon_coord[0][1] + ICON_MOVE_ICON3_HIGH) - 
                                (icon_16_move_time_coord[0][1] + ICON_MOVE_DATA1_HIGH)); 
    }
    
    // �˶�ʱ���ϲ�����
    if(index == 0)
    {
        Gui_PageClearBackground(0, 
                                icon_16_move_icon_coord[0][1] + ICON_MOVE_ICON3_HIGH, 
                                MAX_X_SIZE, 
                                icon_16_move_runtime_coord[0][1] - (icon_16_move_icon_coord[0][1] + ICON_MOVE_ICON3_HIGH)); 
    }
    
    // �˶�ʱ��
    if(last_seconds != seconds || index == 0) 
    {
        uint8_t hour = 0;
        uint8_t minute = 0;
        uint8_t second = 0;
        uint8_t timing_data[6] = {0};
        
        last_seconds = seconds;
        
        hour   = seconds / 60 / 60;
        minute = (seconds / 60 ) % 60;
        second = seconds % 60;
        timing_data[0] = (hour % 100) / 10;
        timing_data[1] = hour % 10;
        timing_data[2] = (minute % 100) / 10;
        timing_data[3] = minute % 10;
        timing_data[4] = (second % 100) / 10;
        timing_data[5] = second % 10;
        
        // �˶�ʱ��
        for(uint32_t i = 0; i < 6; i++)
        {
            // ��ʾ������������ 
            LCD_SetPosition(icon_16_move_runtime_coord[i][0], icon_16_move_runtime_coord[i][1], 
                            ICON_MOVE_DATA2_WIDE, ICON_MOVE_DATA2_HIGH);
            LCD_DriverWriteDataFromExtFlash(icon_16_move_runtime_addr[timing_data[i]], ICON_MOVE_DATA2_SIZE);
        }
        
        // �˶�ʱ��ð��
        for(uint32_t i = 0; i < 2; i++)
        {
            // ��ʾ�����������ֵĵ� 
            LCD_SetPosition(icon_16_move_runtime_dot_coord[i][0], icon_16_move_runtime_dot_coord[i][1], 
                            ICON_MOVE_DATA2DOT_WIDE, ICON_MOVE_DATA2DOT_HIGH);
            LCD_DriverWriteDataFromExtFlash(icon_16_move_runtime_dot_addr[0], ICON_MOVE_DATA2DOT_SIZE);
        }
    }
    
    // �˶�ʱ����������
    if(index == 0)
    {
        Gui_PageClearBackground(0, 
                                icon_16_move_runtime_coord[0][1], 
                                icon_16_move_runtime_coord[0][0], 
                                ICON_MOVE_DATA2_HIGH); 
        
        Gui_PageClearBackground(icon_16_move_runtime_coord[5][0] + ICON_MOVE_DATA2_WIDE, 
                                icon_16_move_runtime_coord[0][1], 
                                MAX_X_SIZE - (icon_16_move_runtime_coord[5][0] + ICON_MOVE_DATA2_WIDE), 
                                ICON_MOVE_DATA2_HIGH); 
    }
    
    // ��·�ﱳ������
    if(index == 0)
    {
        Gui_PageClearBackground(0, 
                                icon_16_move_runtime_coord[0][1] + ICON_MOVE_DATA2_HIGH, 
                                MAX_X_SIZE, 
                                (icon_16_move_calories_unit_coord[0][1] + ICON_MOVE_UNIT1_HIGH) - 
                                (icon_16_move_runtime_coord[0][1] + ICON_MOVE_DATA2_HIGH)); 
    }
    
    // ��·����ֵ
    if(last_calories != calories || index == 0)
    {
        uint32_t calories_tmp = 0;
        
        last_calories = calories;
        
        calories_tmp = (uint32_t)calories;
        if(calories_tmp > 9999)
        {
            calories = 9999;
        }
        
        LCD_SetPosition(icon_16_move_calories_num_coord[0][0], icon_16_move_calories_num_coord[0][1], 
                        ICON_MOVE_DATA3_WIDE, ICON_MOVE_DATA3_HIGH);
        if(calories_tmp >= 1000)
        {    
            LCD_DriverWriteDataFromExtFlash(icon_16_move_calories_num_addr[(calories_tmp / 1000) %10], ICON_MOVE_DATA3_SIZE);        
        }  
        else
        {    
            LCD_DriverWriteDataFromExtFlash(icon_16_move_calories_num_addr[0], ICON_MOVE_DATA3_SIZE);        
        }
        
        LCD_SetPosition(icon_16_move_calories_num_coord[1][0], icon_16_move_calories_num_coord[1][1], 
                        ICON_MOVE_DATA3_WIDE, ICON_MOVE_DATA3_HIGH);
        if(calories_tmp >= 100)
        {    
            LCD_DriverWriteDataFromExtFlash(icon_16_move_calories_num_addr[(calories_tmp / 100) %10], ICON_MOVE_DATA3_SIZE);        
        }  
        else
        {    
            LCD_DriverWriteDataFromExtFlash(icon_16_move_calories_num_addr[0], ICON_MOVE_DATA3_SIZE);        
        }
        
        LCD_SetPosition(icon_16_move_calories_num_coord[2][0], icon_16_move_calories_num_coord[2][1], 
                        ICON_MOVE_DATA3_WIDE, ICON_MOVE_DATA3_HIGH);
        if(calories_tmp >= 10)
        {    
            LCD_DriverWriteDataFromExtFlash(icon_16_move_calories_num_addr[(calories_tmp / 10) %10], ICON_MOVE_DATA3_SIZE);        
        }  
        else
        {    
            LCD_DriverWriteDataFromExtFlash(icon_16_move_calories_num_addr[0], ICON_MOVE_DATA3_SIZE);        
        }
        
        LCD_SetPosition(icon_16_move_calories_num_coord[3][0], icon_16_move_calories_num_coord[3][1], 
                        ICON_MOVE_DATA3_WIDE, ICON_MOVE_DATA3_HIGH);
        if(calories_tmp > 0)
        {    
            LCD_DriverWriteDataFromExtFlash(icon_16_move_calories_num_addr[calories_tmp %10], ICON_MOVE_DATA3_SIZE);        
        }  
        else
        {    
            LCD_DriverWriteDataFromExtFlash(icon_16_move_calories_num_addr[0], ICON_MOVE_DATA3_SIZE);        
        }
    }
    
    // ��·�ﵥλ
    if(last_language != language || index == 0)
    {
        LCD_SetPosition(icon_16_move_calories_unit_coord[0][0], icon_16_move_calories_unit_coord[0][1], 
                        ICON_MOVE_UNIT1_WIDE, ICON_MOVE_UNIT1_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_move_calories_unit_addr[language % 2], ICON_MOVE_UNIT1_SIZE);
    }
    
    // ���ʱ�������
    if(index == 0)
    {
        Gui_PageClearBackground(0, 
                                icon_16_move_calories_unit_coord[0][1] + ICON_MOVE_UNIT1_HIGH, 
                                MAX_X_SIZE, 
                                (icon_16_move_heart_unit_coord[0][1] + ICON_MOVE_UNIT2_HIGH) - 
                                (icon_16_move_calories_unit_coord[0][1] + ICON_MOVE_UNIT1_HIGH));
    }
    
    // ������ֵ
    if(last_heart != heart || index == 0)
    {
        if(heart > 255)
        {
            heart = 255;
        }
        
        LCD_SetPosition(icon_16_move_heart_num_coord[0][0], icon_16_move_heart_num_coord[0][1], 
                        ICON_MOVE_DATA4_WIDE, ICON_MOVE_DATA4_HIGH);;
        if(heart >= 100)
        {    
            LCD_DriverWriteDataFromExtFlash(icon_16_move_heart_num_addr[(heart / 100) %10], ICON_MOVE_DATA4_SIZE);        
        }  
        else
        {    
            LCD_DriverWriteDataFromExtFlash(icon_16_move_heart_num_addr[0], ICON_MOVE_DATA4_SIZE);        
        }
        
        LCD_SetPosition(icon_16_move_heart_num_coord[1][0], icon_16_move_heart_num_coord[1][1], 
                        ICON_MOVE_DATA4_WIDE, ICON_MOVE_DATA4_HIGH);
        if(heart >= 10)
        {    
            LCD_DriverWriteDataFromExtFlash(icon_16_move_heart_num_addr[(heart / 10) %10], ICON_MOVE_DATA4_SIZE);        
        }  
        else
        {    
            LCD_DriverWriteDataFromExtFlash(icon_16_move_heart_num_addr[0], ICON_MOVE_DATA4_SIZE);        
        }
        
        LCD_SetPosition(icon_16_move_heart_num_coord[2][0], icon_16_move_heart_num_coord[2][1], 
                        ICON_MOVE_DATA4_WIDE, ICON_MOVE_DATA4_HIGH);
        if(heart > 0)
        {    
            LCD_DriverWriteDataFromExtFlash(icon_16_move_heart_num_addr[heart %10], ICON_MOVE_DATA4_SIZE);        
        }  
        else
        {    
            LCD_DriverWriteDataFromExtFlash(icon_16_move_heart_num_addr[0], ICON_MOVE_DATA4_SIZE);        
        }
    }
    
    // ���ʵ�λ
    if(last_language != language || index == 0)
    {
        LCD_SetPosition(icon_16_move_heart_unit_coord[0][0], icon_16_move_heart_unit_coord[0][1], 
                        ICON_MOVE_UNIT2_WIDE, ICON_MOVE_UNIT2_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_move_heart_unit_addr[language % 2], ICON_MOVE_UNIT2_SIZE);
    }
    
    // ���ʶ�ͼ
    if(index == 0)
    {
        LCD_SetPosition(icon_16_move_heart_icon_coord[0][0], icon_16_move_heart_icon_coord[0][1], 
                        ICON_MOVE_ICON4_WIDE, ICON_MOVE_ICON4_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_move_heart_icon_addr[heart_index % 8], ICON_MOVE_ICON4_SIZE);
    }
    
    // �Ʋ���������
    if(index == 0)
    {
        Gui_PageClearBackground(0, 
                                icon_16_move_heart_unit_coord[0][1] + ICON_MOVE_UNIT2_HIGH, 
                                MAX_X_SIZE, 
                                (icon_16_move_step_unit_coord[0][1] + ICON_MOVE_UNIT3_HIGH) - 
                                (icon_16_move_heart_unit_coord[0][1] + ICON_MOVE_UNIT2_HIGH));
    } 
    
    // �Ʋ���ֵ
    if(last_step_cnt != step_cnt || index == 0)
    {
        if(step_cnt > 99999)
        {
            step_cnt = 99999;
        }

        LCD_SetPosition(icon_16_move_step_num_coord[0][0], icon_16_move_step_num_coord[0][1], 
                        ICON_MOVE_DATA5_WIDE, ICON_MOVE_DATA5_HIGH);
        if(step_cnt >= 10000)
        {    
            LCD_DriverWriteDataFromExtFlash(icon_16_move_step_num_addr[(step_cnt / 10000) %10], ICON_MOVE_DATA5_SIZE);        
        }  
        else
        {    
            LCD_DriverWriteDataFromExtFlash(icon_16_move_step_num_addr[0], ICON_MOVE_DATA5_SIZE);        
        } 
        
        LCD_SetPosition(icon_16_move_step_num_coord[1][0], icon_16_move_step_num_coord[1][1], 
                        ICON_MOVE_DATA5_WIDE, ICON_MOVE_DATA5_HIGH);
        if(step_cnt >= 1000)
        {    
            LCD_DriverWriteDataFromExtFlash(icon_16_move_step_num_addr[(step_cnt / 1000) %10], ICON_MOVE_DATA5_SIZE);        
        }  
        else
        {    
            LCD_DriverWriteDataFromExtFlash(icon_16_move_step_num_addr[0], ICON_MOVE_DATA5_SIZE);        
        }
        
        LCD_SetPosition(icon_16_move_step_num_coord[2][0], icon_16_move_step_num_coord[2][1], 
                        ICON_MOVE_DATA5_WIDE, ICON_MOVE_DATA5_HIGH);;
        if(step_cnt >= 100)
        {    
            LCD_DriverWriteDataFromExtFlash(icon_16_move_step_num_addr[(step_cnt / 100) %10], ICON_MOVE_DATA5_SIZE);        
        }  
        else
        {    
            LCD_DriverWriteDataFromExtFlash(icon_16_move_step_num_addr[0], ICON_MOVE_DATA5_SIZE);        
        }
        LCD_SetPosition(icon_16_move_step_num_coord[3][0], icon_16_move_step_num_coord[3][1], 
                        ICON_MOVE_DATA5_WIDE, ICON_MOVE_DATA5_HIGH);
        if(step_cnt >= 10)
        {    
            LCD_DriverWriteDataFromExtFlash(icon_16_move_step_num_addr[(step_cnt / 10) %10], ICON_MOVE_DATA5_SIZE);        
        }  
        else
        {    
            LCD_DriverWriteDataFromExtFlash(icon_16_move_step_num_addr[0], ICON_MOVE_DATA5_SIZE);        
        }
        
        LCD_SetPosition(icon_16_move_step_num_coord[4][0], icon_16_move_step_num_coord[4][1], 
                        ICON_MOVE_DATA5_WIDE, ICON_MOVE_DATA5_HIGH);
        if(step_cnt > 0)
        {    
            LCD_DriverWriteDataFromExtFlash(icon_16_move_step_num_addr[step_cnt %10], ICON_MOVE_DATA5_SIZE);        
        }  
        else
        {    
            LCD_DriverWriteDataFromExtFlash(icon_16_move_step_num_addr[0], ICON_MOVE_DATA5_SIZE);        
        }
    }
    
    // �Ʋ���λ
    if(last_language != language || index == 0)
    {
        LCD_SetPosition(icon_16_move_step_unit_coord[0][0], icon_16_move_step_unit_coord[0][1], 
                        ICON_MOVE_UNIT3_WIDE, ICON_MOVE_UNIT3_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_move_step_unit_addr[language % 2], ICON_MOVE_UNIT3_SIZE);
    }
    
    // �ײ�����
    if(index == 0)
    {
        Gui_PageClearBackground(0, 
                                icon_16_move_step_unit_coord[0][1] + ICON_MOVE_UNIT3_HIGH, 
                                MAX_X_SIZE, 
                                MAX_Y_SIZE - (icon_16_move_step_unit_coord[0][1] + ICON_MOVE_UNIT3_HIGH)); 
    }
    
    
    last_language = language;
    heart_index++;
}

/****************************************************************
** Function name:           Gui_PageRunModeOperate
** Descriptions:            �˶�ģʽ��������
** input parameters:        pause_state��1��ͣ��0����
                            language: ���ԣ�0���ġ�1Ӣ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2018-05-24
**----------------------------------------------------------------
*****************************************************************/ 
void Gui_PageRunModeOperate(uint16_t index, uint8_t pause_state, uint8_t operate_state, uint8_t hour, uint8_t minute, uint8_t language)
{
    
}

/****************************************************************
** Function name:           Gui_PageRunModeUnlock
** Descriptions:            �˶�ģʽ��������
** input parameters:        index����̬ͼ����  
**                          language������
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2018-05-24
**----------------------------------------------------------------
*****************************************************************/
void Gui_PageRunModeUnlock(uint16_t index, uint8_t language)
{
    
}

/****************************************************************
** Function name:           Gui_PageRunModePause
** Descriptions:            �˶�ģʽ��ͣ��ʼ����
** input parameters:        pause_state��1��ͣ��0����
                            language: ���ԣ�0���ġ�1Ӣ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2018-05-24
**----------------------------------------------------------------
*****************************************************************/ 
void Gui_PageRunModePause(uint16_t index, uint8_t pause_state, uint8_t language)
{	
    // ͼ���ϲ�����
    if(index == 0)
    {
        Gui_PageClearBackground(0, 
                                0, 
                                MAX_X_SIZE, 
                                icon_16_move_pause_icon_coord[0][1]); 
    }
    
    // ��ǩ��ͼ��
    if(index == 0)
    {
        //��ͣ
        if(pause_state)
        {
//            // ��ǩ
//            LCD_SetPosition(icon_16_move_pause_label_coord[0][0], icon_16_move_pause_label_coord[0][1], 
//                            ICON_MOVE3_ICON4_WIDE, ICON_MOVE3_ICON4_HIGH);
//            LCD_DriverWriteDataFromExtFlash(icon_16_move_pause_label_addr[0], ICON_MOVE3_ICON4_SIZE);
            // ͼ��
            LCD_SetPosition(icon_16_move_pause_icon_coord[0][0], icon_16_move_pause_icon_coord[0][1], 
                            ICON_MOVE4_ICON1_WIDE, ICON_MOVE4_ICON1_HIGH);
            LCD_DriverWriteDataFromExtFlash(icon_16_move_pause_icon_addr[0], ICON_MOVE4_ICON1_SIZE);
        }            
        else
        {
//            // ��ǩ
//            LCD_SetPosition(icon_16_move_goon_label_coord[0][0], icon_16_move_goon_label_coord[0][1], 
//                            ICON_MOVE2_ICON1_WIDE, ICON_MOVE2_ICON1_HIGH);
//            LCD_DriverWriteDataFromExtFlash(icon_16_move_goon_label_addr[0], ICON_MOVE2_ICON1_SIZE);
            // ͼ��
            LCD_SetPosition(icon_16_move_goon_icon_coord[0][0], icon_16_move_goon_icon_coord[0][1], 
                            ICON_MOVE3_ICON1_WIDE, ICON_MOVE3_ICON1_HIGH);
            LCD_DriverWriteDataFromExtFlash(icon_16_move_goon_icon_addr[0], ICON_MOVE3_ICON1_SIZE);
        }
    }
    
    // ͼ����������
    if(index == 0)
    {
        Gui_PageClearBackground(0, 
                                icon_16_move_pause_icon_coord[0][1], 
                                icon_16_move_pause_icon_coord[0][0], 
                                ICON_MOVE3_ICON1_HIGH); 
        
        Gui_PageClearBackground(icon_16_move_pause_icon_coord[0][0] + ICON_MOVE3_ICON1_WIDE, 
                                icon_16_move_pause_icon_coord[0][1], 
                                MAX_X_SIZE - (icon_16_move_pause_icon_coord[0][0] + ICON_MOVE3_ICON1_WIDE), 
                                ICON_MOVE3_ICON1_HIGH); 
    }
    
    // ��Ļ�ײ�����
    if(index == 0)
    {
        Gui_PageClearBackground(0, 
                                icon_16_move_pause_icon_coord[0][1] + ICON_MOVE3_ICON1_HIGH, 
                                MAX_X_SIZE, 
                                MAX_Y_SIZE - (icon_16_move_pause_icon_coord[0][1] + ICON_MOVE3_ICON1_HIGH)); 
    }
}

/****************************************************************
** Function name:           Gui_PageRunModeExit
** Descriptions:            �˶�ģʽ�˳�����
** input parameters:        
                            language: ���ԣ�0���ġ�1Ӣ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2018-05-24
**----------------------------------------------------------------
*****************************************************************/ 
void Gui_PageRunModeExit(uint16_t index, uint8_t language)
{
        // ͼ���ϲ�����
    if(index == 0)
    {
        Gui_PageClearBackground(0, 
                                0, 
                                MAX_X_SIZE, 
                                icon_16_move_exit_icon_coord[0][1]); 
    }
    
    // ��ǩ��ͼ��
    if(index == 0)
    {
//        // ��ǩ
//        LCD_SetPosition(icon_16_move_exit_label_coord[0][0], icon_16_move_exit_label_coord[0][1], 
//                        ICON_MOVE4_ICON2_WIDE, ICON_MOVE4_ICON2_HIGH);
//        LCD_DriverWriteDataFromExtFlash(icon_16_move_exit_label_addr[0], ICON_MOVE4_ICON2_SIZE);
        // ͼ��
        LCD_SetPosition(icon_16_move_exit_icon_coord[0][0], icon_16_move_exit_icon_coord[0][1], 
                        ICON_MOVE4_ICON1_WIDE, ICON_MOVE4_ICON1_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_move_exit_icon_addr[0], ICON_MOVE4_ICON1_SIZE);
    }
    
    // ͼ����������
    if(index == 0)
    {
        Gui_PageClearBackground(0, 
                                icon_16_move_exit_icon_coord[0][1], 
                                icon_16_move_exit_icon_coord[0][0], 
                                ICON_MOVE4_ICON1_HIGH); 
        
        Gui_PageClearBackground(icon_16_move_exit_icon_coord[0][0] + ICON_MOVE4_ICON1_WIDE, 
                                icon_16_move_exit_icon_coord[0][1], 
                                MAX_X_SIZE - (icon_16_move_exit_icon_coord[0][0] + ICON_MOVE4_ICON1_WIDE), 
                                ICON_MOVE4_ICON1_HIGH); 
    }
    
    // ��Ļ�ײ�����
    if(index == 0)
    {
        Gui_PageClearBackground(0, 
                                icon_16_move_exit_icon_coord[0][1] + ICON_MOVE4_ICON1_HIGH, 
                                MAX_X_SIZE, 
                                MAX_Y_SIZE - (icon_16_move_exit_icon_coord[0][1] + ICON_MOVE4_ICON1_HIGH)); 
    }
}

/****************************************************************
** Function name:           Gui_PageBP
** Descriptions:            ��ʾѪѹ����
** input parameters:        index����̬ͼ����  bp_sp: ��ѹ bp_dp����ѹ
**                          test_mode������ģʽ��1˽��ģʽ��0ͨ��ģʽ
**                          test_state�����Ե�״̬��1�����У�2��������3���Գ���
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2018-05-24
**----------------------------------------------------------------
*****************************************************************/ 
void Gui_PageBP(uint16_t index, uint8_t bp_sp, uint8_t bp_dp, uint8_t test_mode, uint8_t test_state, uint8_t progress)
{

    // ����ͼ���ϲ�����
    if(index == 0 )
    {
        Gui_PageClearBackground(0, 
                                0, 
                                MAX_X_SIZE, 
                                icon_16_bp_dynamic_coord[0][1]); 
    }
    
    if(test_state == 0)
    {    
        // ��ͼ������
        if(index == 0)
        {
            Gui_PageClearBackground(0, 
                                    icon_16_bp_dynamic_coord[0][1], 
                                    icon_16_bp_dynamic_coord[0][0], 
                                    ICON_BP_ICON_HIGH); 
        }
        
        // ����Ѫѹͼ�궯̬ 
        LCD_SetPosition(icon_16_bp_dynamic_coord[0][0], icon_16_bp_dynamic_coord[0][1], 
                        ICON_BP_ICON_WIDE, ICON_BP_ICON_HIGH);
        if(test_mode == 1)
        {
            LCD_DriverWriteDataFromExtFlash(icon_16_bp_p_icon_addr[index % 4], ICON_BP_ICON_SIZE);
        }
        else
        {
            LCD_DriverWriteDataFromExtFlash(icon_16_bp_dynamic_addr[index % 4], ICON_BP_ICON_SIZE);
        }
        
        // ��ͼ�Ҳ�����
        if(index == 0)
        {
            Gui_PageClearBackground(icon_16_bp_dynamic_coord[0][0] + ICON_BP_ICON_WIDE, 
                                    icon_16_bp_dynamic_coord[0][1], 
                                    MAX_X_SIZE - (icon_16_bp_dynamic_coord[0][0] + ICON_BP_ICON_WIDE), 
                                    ICON_BP_ICON_HIGH); 
        }
        
        // ---�ϲ�����
        if(index == 0)
        {
            Gui_PageClearBackground(0, 
                                    icon_16_bp_dynamic_coord[0][1] + ICON_BP_ICON_HIGH, 
                                    MAX_X_SIZE, 
                                    icon_16_bp_data_not_coord[0][1] - (icon_16_bp_dynamic_coord[0][1] + ICON_BP_ICON_HIGH));
        }
        
        // ---������
        if(index == 0)
        {
            Gui_PageClearBackground(0, 
                                    icon_16_bp_data_not_coord[0][1], 
                                    icon_16_bp_data_not_coord[0][0], 
                                    ICON_BP_ICON2_HIGH);
        }
            
        if(index == 0)
        {
            // ����--- 
            LCD_SetPosition(icon_16_bp_data_not_coord[0][0], icon_16_bp_data_not_coord[0][1], 
                            ICON_BP_ICON2_WIDE, ICON_BP_ICON2_HIGH);
            LCD_DriverWriteDataFromExtFlash(icon_16_bp_data_not_addr[0], ICON_BP_ICON2_SIZE);
        }
        
        // ---�Ҳ�����
        if(index == 0)
        {
            Gui_PageClearBackground(icon_16_bp_data_not_coord[0][0] + ICON_BP_ICON2_WIDE, 
                                    icon_16_bp_data_not_coord[0][1], 
                                    MAX_X_SIZE - (icon_16_bp_data_not_coord[0][0] + ICON_BP_ICON2_WIDE), 
                                    ICON_BP_ICON2_HIGH);
        }
        
        // ��λ��������
        if(index == 0)
        {
            Gui_PageClearBackground(0, 
                                    icon_16_bp_data_not_coord[0][1] + ICON_BP_ICON2_HIGH, 
                                    MAX_X_SIZE, 
                                    (icon_16_bp_data_unit_coord[0][1] + ICON_BP_UNIT_HIGH) -
                                    (icon_16_bp_data_not_coord[0][1] + ICON_BP_ICON2_HIGH)); 
        }
    }
    else if(test_state == 1)
    {     
		// ��ͼ������
        if(index == 0)
        {
            Gui_PageClearBackground(0, 
                                    icon_16_bp_dynamic_coord[0][1], 
                                    icon_16_bp_dynamic_coord[0][0], 
                                    ICON_BP_ICON_HIGH); 
        }
		
        // ����Ѫѹͼ�궯̬ 
        LCD_SetPosition(icon_16_bp_dynamic_coord[0][0], icon_16_bp_dynamic_coord[0][1], 
                        ICON_BP_ICON_WIDE, ICON_BP_ICON_HIGH);
        if(test_mode == 1)
        {
            LCD_DriverWriteDataFromExtFlash(icon_16_bp_p_icon_addr[3], ICON_BP_ICON_SIZE);
        }
        else
        {
            LCD_DriverWriteDataFromExtFlash(icon_16_bp_dynamic_addr[3], ICON_BP_ICON_SIZE);
        }
		
		// ��ͼ�Ҳ�����
        if(index == 0)
        {
            Gui_PageClearBackground(icon_16_bp_dynamic_coord[0][0] + ICON_BP_ICON_WIDE, 
                                    icon_16_bp_dynamic_coord[0][1], 
                                    MAX_X_SIZE - (icon_16_bp_dynamic_coord[0][0] + ICON_BP_ICON_WIDE), 
                                    ICON_BP_ICON_HIGH); 
        }
		
		// Ѫѹֵ-��������
        if(index == 0)
        {
            Gui_PageClearBackground(0, 
                                    icon_16_bp_dynamic_coord[0][1] + ICON_BP_ICON_HIGH, 
                                    MAX_X_SIZE, 
                                    (icon_16_bp_datadot_coord[0][1] + ICON_BP_DATADOT_HIGH)- 
									(icon_16_bp_dynamic_coord[0][1] + ICON_BP_ICON_HIGH));
        }
               
        // ��ѹ  
        if(bp_sp >= 100)
        {
            LCD_SetPosition(icon_16_bp_data_coord[0][0], icon_16_bp_data_coord[0][1], 
                            ICON_BP_DATA_WIDE, ICON_BP_DATA_HIGH);
            LCD_DriverWriteDataFromExtFlash(icon_16_bp_data_addr[(bp_sp / 100)% 10], ICON_BP_DATA_SIZE);          
        }
        else
        {
            LCD_SetPosition(icon_16_bp_data_coord[0][0], icon_16_bp_data_coord[0][1], 
                            ICON_BP_DATA_WIDE, ICON_BP_DATA_HIGH);
            LCD_DriverWriteDataFromExtFlash(icon_16_bp_data_addr[0], ICON_BP_DATA_SIZE); 
        }
        
        LCD_SetPosition(icon_16_bp_data_coord[1][0], icon_16_bp_data_coord[1][1], 
                            ICON_BP_DATA_WIDE, ICON_BP_DATA_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_bp_data_addr[(bp_sp / 10)% 10], ICON_BP_DATA_SIZE); 

        LCD_SetPosition(icon_16_bp_data_coord[2][0], icon_16_bp_data_coord[2][1], 
                            ICON_BP_DATA_WIDE, ICON_BP_DATA_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_bp_data_addr[bp_sp % 10], ICON_BP_DATA_SIZE);          

        // б��
        LCD_SetPosition(icon_16_bp_datadot_coord[0][0], icon_16_bp_datadot_coord[0][1], 
                        ICON_BP_DATADOT_WIDE, ICON_BP_DATADOT_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_bp_datadot_addr[0], ICON_BP_DATADOT_SIZE);
              
        // ��ѹ  
        if(bp_dp >= 100)
        {
            LCD_SetPosition(icon_16_bp_data_coord[3][0], icon_16_bp_data_coord[3][1], 
                            ICON_BP_DATA_WIDE, ICON_BP_DATA_HIGH);
            LCD_DriverWriteDataFromExtFlash(icon_16_bp_data_addr[(bp_dp % 1000) / 100], ICON_BP_DATA_SIZE); 
        }
        else
        {
            LCD_SetPosition(icon_16_bp_data_coord[3][0], icon_16_bp_data_coord[3][1], 
                            ICON_BP_DATA_WIDE, ICON_BP_DATA_HIGH);
            LCD_DriverWriteDataFromExtFlash(icon_16_bp_data_addr[0], ICON_BP_DATA_SIZE); 
        }
        
        LCD_SetPosition(icon_16_bp_data_coord[4][0], icon_16_bp_data_coord[4][1], 
                            ICON_BP_DATA_WIDE, ICON_BP_DATA_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_bp_data_addr[(bp_dp % 100) / 10], ICON_BP_DATA_SIZE); 

        LCD_SetPosition(icon_16_bp_data_coord[5][0], icon_16_bp_data_coord[5][1], 
                            ICON_BP_DATA_WIDE, ICON_BP_DATA_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_bp_data_addr[bp_dp % 10], ICON_BP_DATA_SIZE);  
		
		// ��λ��������
        if(index == 0)
        {
            Gui_PageClearBackground(0, 
                                    icon_16_bp_datadot_coord[0][1] + ICON_BP_DATADOT_HIGH, 
                                    MAX_X_SIZE, 
                                    (icon_16_bp_data_unit_coord[0][1] + ICON_BP_UNIT_HIGH) -
                                    (icon_16_bp_datadot_coord[0][1] + ICON_BP_DATADOT_HIGH)); 
        }
    }
    else if(test_state >= 2)
    {
		// ��ͼ������
        if(index == 0)
        {
            Gui_PageClearBackground(0, 
                                    icon_16_bp_dynamic_coord[0][1], 
                                    icon_16_bp_dynamic_coord[0][0], 
                                    ICON_BP_ICON_HIGH); 
        }
		
        // ����Ѫѹͼ�궯̬ 
        LCD_SetPosition(icon_16_bp_dynamic_coord[0][0], icon_16_bp_dynamic_coord[0][1], 
                        ICON_BP_ICON1_WIDE, ICON_BP_ICON1_HIGH);
        if(test_mode == 1)
        {
            LCD_DriverWriteDataFromExtFlash(icon_16_bp_p_icon_addr[3], ICON_BP_ICON_SIZE);
        }
        else
        {
            LCD_DriverWriteDataFromExtFlash(icon_16_bp_dynamic_addr[3], ICON_BP_ICON_SIZE);
        }
		
		// ��ͼ�Ҳ�����
        if(index == 0)
        {
            Gui_PageClearBackground(icon_16_bp_dynamic_coord[0][0] + ICON_BP_ICON_WIDE, 
                                    icon_16_bp_dynamic_coord[0][1], 
                                    MAX_X_SIZE - (icon_16_bp_dynamic_coord[0][0] + ICON_BP_ICON_WIDE), 
                                    ICON_BP_ICON_HIGH); 
        }
        
		// ---�ϲ�����
        if(index == 0)
        {
            Gui_PageClearBackground(0, 
                                    icon_16_bp_dynamic_coord[0][1] + ICON_BP_ICON_HIGH, 
                                    MAX_X_SIZE, 
                                    icon_16_bp_data_not_coord[0][1] - (icon_16_bp_dynamic_coord[0][1] + ICON_BP_ICON_HIGH));
        }
        
        // ---������
        if(index == 0)
        {
            Gui_PageClearBackground(0, 
                                    icon_16_bp_data_not_coord[0][1], 
                                    icon_16_bp_data_not_coord[0][0], 
                                    ICON_BP_ICON2_HIGH);
        }
		
        // ����xxx 
        LCD_SetPosition(icon_16_bp_data_err_coord[0][0], icon_16_bp_data_err_coord[0][1], 
                           ICON_BP_ICON2_WIDE, ICON_BP_ICON2_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_bp_data_err_addr[0], ICON_BP_ICON2_SIZE);
		
		// ---�Ҳ�����
        if(index == 0)
        {
            Gui_PageClearBackground(icon_16_bp_data_not_coord[0][0] + ICON_BP_ICON2_WIDE, 
                                    icon_16_bp_data_not_coord[0][1], 
                                    MAX_X_SIZE - (icon_16_bp_data_not_coord[0][0] + ICON_BP_ICON2_WIDE), 
                                    ICON_BP_ICON2_HIGH);
        }
        
        // ��λ��������
        if(index == 0)
        {
            Gui_PageClearBackground(0, 
                                    icon_16_bp_data_not_coord[0][1] + ICON_BP_ICON2_HIGH, 
                                    MAX_X_SIZE, 
                                    (icon_16_bp_data_unit_coord[0][1] + ICON_BP_UNIT_HIGH) -
                                    (icon_16_bp_data_not_coord[0][1] + ICON_BP_ICON2_HIGH)); 
        }
    }
    
    // Ѫѹ��λ
    if(index == 0)
    {
        LCD_SetPosition(icon_16_bp_data_unit_coord[0][0], icon_16_bp_data_unit_coord[0][1], 
                        ICON_BP_UNIT_WIDE, ICON_BP_UNIT_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_bp_data_unit_addr[0], ICON_BP_UNIT_SIZE);
    }
    
    // ��Ļ�ײ�����
    if(index == 0)
    {
        Gui_PageClearBackground(0, 
                                icon_16_bp_data_unit_coord[0][1] + ICON_BP_UNIT_HIGH, 
                                MAX_X_SIZE, 
                                MAX_Y_SIZE - (icon_16_bp_data_unit_coord[0][1] + ICON_BP_UNIT_HIGH)); 
    }
}

/****************************************************************
** Function name:           Gui_PageHeart
** Descriptions:            ��ʾ���ʽ���
** input parameters:        index����̬ͼ����  heart: ����ֵ
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2018-05-24
**----------------------------------------------------------------
*****************************************************************/ 
void Gui_PageHeart(uint16_t index, uint8_t heart, uint8_t last_heart, 
                   uint8_t * heart_array, uint8_t max_heart, uint8_t min_heart, uint8_t language)
{
    if(language)
    {
        language = 1;
    }
    
    // ͼ���ϲ�����
    if(index == 0)
    {
        Gui_PageClearBackground(0, 
                                0, 
                                MAX_X_SIZE,
                                icon_16_heart_bg_coord[0][1]);
    }
    
    // ����������
    if(index == 0)
    {
        Gui_PageClearBackground(0, 
                                icon_16_heart_bg_coord[0][1], 
                                icon_16_heart_bg_coord[0][0],
                                ICON_HEART_ICON1_HIGH);
    }

    // ������ʾ
    if(index == 0)
    {
        LCD_SetPosition(icon_16_heart_bg_coord[0][0], icon_16_heart_bg_coord[0][1], 
                       ICON_HEART_ICON1_WIDE, ICON_HEART_ICON1_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_heart_bg_addr[0], ICON_HEART_ICON1_SIZE);
        
    }
    
    // ���ʴ��
    if(index == 0)
    {
        uint8_t average_heart = 0;
        uint8_t heart_dot_x_base = 0;
        uint8_t heart_dot_y_base = 0;
        uint8_t heart_value = 0;
        float heart_dot_y_factor = 0;
        uint8_t heart_dot_y = 0;
        
        if((min_heart == 0 )||(max_heart == 0))
        {
            min_heart = 30;
            average_heart = 60;
            max_heart = 90;
        }
        else
        {
			average_heart = (max_heart + min_heart) / 2;
			
            if(min_heart % 10)
            {
                min_heart = min_heart - (min_heart % 10);
            }
              
			if(average_heart % 10 != 0)
			{
				if(average_heart % 10 <= 5)
				{
					average_heart = average_heart - (average_heart % 10) + 5;
				}
				else
				{
					average_heart = average_heart - (average_heart % 10) + 10;
				}
			}
            
            if(average_heart - min_heart < 20)
            {
                max_heart = average_heart + 20;
                min_heart = average_heart - 20;
            }
            else
            {
                max_heart = average_heart + (average_heart - min_heart);
            }
        }
        
        heart_dot_x_base = icon_16_heart_bg_coord[0][0] + 4;
        heart_dot_y_base = icon_16_heart_bg_num_coord[2][1] + ICON_HEART_DATA1_HIGH / 2;
        
        heart_dot_y_factor =(float)(icon_16_heart_bg_num_coord[2][1] - icon_16_heart_bg_num_coord[0][1]) / (float)(max_heart - min_heart);
        
        for(uint32_t i = 0; i < 48; i++)
        {
            heart_value = heart_array[i];
            
            if((heart_value >= 30)&&(heart_value <= 200))
            {
                heart_dot_y = heart_dot_y_base - (uint8_t)(heart_dot_y_factor * (float)(heart_value - min_heart));
                
                LCD_SetPosition(heart_dot_x_base + i * 4, heart_dot_y, 
                                ICON_HEART_DATADOT_WIDE, ICON_HEART_DATADOT_HIGH);
                LCD_DriverWriteDataFromExtFlash(icon_16_heart_bg_dot_addr[0], ICON_HEART_DATADOT_SIZE);
            }
        }
        
        // ���ʴ���ע-�ϱ�ע-�������ֵ
        {
            uint8_t start_column = 0;
            
            if(max_heart > 255)
            {
                max_heart = 255;
            }
            
            start_column = icon_16_heart_bg_num_coord[0][0];
            
            if(max_heart >= 100)
            {
                LCD_SetPosition(start_column, icon_16_heart_bg_num_coord[0][1], 
                                ICON_HEART_DATA1_WIDE, ICON_HEART_DATA1_HIGH);;
                LCD_DriverWriteDataFromExtFlash(icon_16_heart_bg_num_addr[(max_heart / 100) %10], ICON_HEART_DATA1_SIZE); 
                    
                start_column += ICON_HEART_DATA1_WIDE;
            }
            
            if(max_heart >= 10)
            {
                LCD_SetPosition(start_column, icon_16_heart_bg_num_coord[0][1], 
                                ICON_HEART_DATA1_WIDE, ICON_HEART_DATA1_HIGH);
                if(max_heart >= 10)
                {    
                    LCD_DriverWriteDataFromExtFlash(icon_16_heart_bg_num_addr[(max_heart / 10) %10], ICON_HEART_DATA1_SIZE);        
                }  
                else
                {    
                    LCD_DriverWriteDataFromExtFlash(icon_16_heart_bg_num_addr[0], ICON_HEART_DATA1_SIZE);        
                }
                
                start_column += ICON_HEART_DATA1_WIDE;
            }
            
            LCD_SetPosition(start_column, icon_16_heart_bg_num_coord[0][1], 
                            ICON_HEART_DATA1_WIDE, ICON_HEART_DATA1_HIGH);
            if(max_heart > 0)
            {    
                LCD_DriverWriteDataFromExtFlash(icon_16_heart_bg_num_addr[max_heart %10], ICON_HEART_DATA1_SIZE);        
            }  
            else
            {    
                LCD_DriverWriteDataFromExtFlash(icon_16_heart_bg_num_addr[0], ICON_HEART_DATA1_SIZE);        
            }
        }
        
        // ���ʴ���ע-�б�ע
        {
            uint8_t start_column = 0;
            
            if(average_heart > 255)
            {
                average_heart = 255;
            }
            
            start_column = icon_16_heart_bg_num_coord[1][0];
            
            if(average_heart >= 100)
            {
                LCD_SetPosition(start_column, icon_16_heart_bg_num_coord[1][1], 
                                ICON_HEART_DATA1_WIDE, ICON_HEART_DATA1_HIGH);;
                LCD_DriverWriteDataFromExtFlash(icon_16_heart_bg_num_addr[(average_heart / 100) %10], ICON_HEART_DATA1_SIZE); 
                    
                start_column += ICON_HEART_DATA1_WIDE;
            }
            
            if(average_heart >= 10)
            {
                LCD_SetPosition(start_column, icon_16_heart_bg_num_coord[1][1], 
                                ICON_HEART_DATA1_WIDE, ICON_HEART_DATA1_HIGH);
                if(average_heart >= 10)
                {    
                    LCD_DriverWriteDataFromExtFlash(icon_16_heart_bg_num_addr[(average_heart / 10) %10], ICON_HEART_DATA1_SIZE);        
                }  
                else
                {    
                    LCD_DriverWriteDataFromExtFlash(icon_16_heart_bg_num_addr[0], ICON_HEART_DATA1_SIZE);        
                }
                
                start_column += ICON_HEART_DATA1_WIDE;
            }
            
            LCD_SetPosition(start_column, icon_16_heart_bg_num_coord[1][1], 
                            ICON_HEART_DATA1_WIDE, ICON_HEART_DATA1_HIGH);
            if(average_heart > 0)
            {    
                LCD_DriverWriteDataFromExtFlash(icon_16_heart_bg_num_addr[average_heart %10], ICON_HEART_DATA1_SIZE);        
            }  
            else
            {    
                LCD_DriverWriteDataFromExtFlash(icon_16_heart_bg_num_addr[0], ICON_HEART_DATA1_SIZE);        
            }
        }
        
        // ���ʴ���ע-�±�ע
        {
            uint8_t start_column = 0;
            
            if(min_heart > 255)
            {
                min_heart = 255;
            }
            
            start_column = icon_16_heart_bg_num_coord[2][0];
            
            if(min_heart >= 100)
            {
                LCD_SetPosition(start_column, icon_16_heart_bg_num_coord[2][1], 
                                ICON_HEART_DATA1_WIDE, ICON_HEART_DATA1_HIGH);;
                LCD_DriverWriteDataFromExtFlash(icon_16_heart_bg_num_addr[(min_heart / 100) %10], ICON_HEART_DATA1_SIZE); 
                    
                start_column += ICON_HEART_DATA1_WIDE;
            }
            
            if(min_heart >= 10)
            {
                LCD_SetPosition(start_column, icon_16_heart_bg_num_coord[2][1], 
                                ICON_HEART_DATA1_WIDE, ICON_HEART_DATA1_HIGH);
                if(min_heart >= 10)
                {    
                    LCD_DriverWriteDataFromExtFlash(icon_16_heart_bg_num_addr[(min_heart / 10) %10], ICON_HEART_DATA1_SIZE);        
                }  
                else
                {    
                    LCD_DriverWriteDataFromExtFlash(icon_16_heart_bg_num_addr[0], ICON_HEART_DATA1_SIZE);        
                }
                
                start_column += ICON_HEART_DATA1_WIDE;
            }
            
            LCD_SetPosition(start_column, icon_16_heart_bg_num_coord[2][1], 
                            ICON_HEART_DATA1_WIDE, ICON_HEART_DATA1_HIGH);
            if(min_heart > 0)
            {    
                LCD_DriverWriteDataFromExtFlash(icon_16_heart_bg_num_addr[min_heart %10], ICON_HEART_DATA1_SIZE);        
            }  
            else
            {    
                LCD_DriverWriteDataFromExtFlash(icon_16_heart_bg_num_addr[0], ICON_HEART_DATA1_SIZE);        
            }  
        }
    }
    
    // ����������
    if(index == 0)
    {
        Gui_PageClearBackground(icon_16_heart_bg_coord[0][0] + ICON_HEART_ICON1_WIDE, 
                                icon_16_heart_bg_coord[0][1], 
                                MAX_X_SIZE - (icon_16_heart_bg_coord[0][0] + ICON_HEART_ICON1_WIDE),
                                ICON_HEART_ICON1_HIGH);
    }
    
    // ���ʱ�ǩ�ϲ�����
    if(index == 0)
    {
        Gui_PageClearBackground(0, 
                                icon_16_heart_bg_coord[0][1] + ICON_HEART_ICON1_HIGH, 
                                MAX_X_SIZE,
                                icon_16_heart_label_coord[0][1] - (icon_16_heart_bg_coord[0][1] + ICON_HEART_ICON1_HIGH));
    }
    
    // ���ʱ�ǩ������
    if(index == 0)
    {
        Gui_PageClearBackground(0, 
                                icon_16_heart_label_coord[0][1], 
                                icon_16_heart_label_coord[0][0],
                                ICON_HEART_ICON2_HIGH);
    }
    
    // ���ʱ�ǩ��ʾ
    {
        LCD_SetPosition(icon_16_heart_label_coord[0][0], icon_16_heart_label_coord[0][1], 
                       ICON_HEART_ICON2_WIDE, ICON_HEART_ICON2_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_heart_label_addr[language % 2], ICON_HEART_ICON2_SIZE);
    }
    
    // ���ʱ�ǩ������
    if(index == 0)
    {
        Gui_PageClearBackground(icon_16_heart_label_coord[0][0] + ICON_HEART_ICON2_WIDE, 
                                icon_16_heart_label_coord[0][1], 
                                MAX_X_SIZE - (icon_16_heart_label_coord[0][0] + ICON_HEART_ICON2_WIDE),
                                ICON_HEART_ICON2_HIGH);
    }
    
    // ��������ֵ��������
    if(index == 0)
    {
        Gui_PageClearBackground(0, 
                                icon_16_heart_label_coord[0][1] + ICON_HEART_ICON2_HIGH, 
                                MAX_X_SIZE,
                                (icon_16_heart_icon_coord[0][1] + ICON_HEART_ICON4_HIGH) -
                                (icon_16_heart_label_coord[0][1] + ICON_HEART_ICON2_HIGH));
    }
    
    // ���ʶ�ͼ��ʾ
    {
        LCD_SetPosition(icon_16_heart_icon_coord[0][0], icon_16_heart_icon_coord[0][1], 
                       ICON_HEART_ICON4_WIDE, ICON_HEART_ICON4_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_heart_icon_addr[index % 16], ICON_HEART_ICON4_SIZE);
    }
    
    // ����ֵ��ʾ
    {
        if(heart > 255)
        {
            heart = 255;
        }
        
        LCD_SetPosition(icon_16_heart_data_coord[0][0], icon_16_heart_data_coord[0][1], 
                        ICON_HEART_DATA2_WIDE, ICON_HEART_DATA2_HIGH);;
        if(heart >= 100)
        {    
            LCD_DriverWriteDataFromExtFlash(icon_16_heart_data_addr[(heart / 100) %10], ICON_HEART_DATA2_SIZE);        
        }  
        else
        {    
            LCD_DriverWriteDataFromExtFlash(icon_16_heart_data_addr[0], ICON_HEART_DATA2_SIZE);        
        }
        
        LCD_SetPosition(icon_16_heart_data_coord[1][0], icon_16_heart_data_coord[1][1], 
                        ICON_HEART_DATA2_WIDE, ICON_HEART_DATA2_HIGH);
        if(heart >= 10)
        {    
            LCD_DriverWriteDataFromExtFlash(icon_16_heart_data_addr[(heart / 10) %10], ICON_HEART_DATA2_SIZE);        
        }  
        else
        {    
            LCD_DriverWriteDataFromExtFlash(icon_16_heart_data_addr[0], ICON_HEART_DATA2_SIZE);        
        }
        
        LCD_SetPosition(icon_16_heart_data_coord[2][0], icon_16_heart_data_coord[2][1], 
                        ICON_HEART_DATA2_WIDE, ICON_HEART_DATA2_HIGH);
        if(heart > 0)
        {    
            LCD_DriverWriteDataFromExtFlash(icon_16_heart_data_addr[heart %10], ICON_HEART_DATA2_SIZE);        
        }  
        else
        {    
            LCD_DriverWriteDataFromExtFlash(icon_16_heart_data_addr[0], ICON_HEART_DATA2_SIZE);        
        }
    }
    
    // ��λ��ʾ
    {    
        LCD_SetPosition(icon_16_heart_unit_coord[0][0], icon_16_heart_unit_coord[0][1], 
                       ICON_HEART_UNIT1_WIDE, ICON_HEART_UNIT1_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_heart_unit_addr[language % 2], ICON_HEART_UNIT1_SIZE);
    }
    
    // �ϴ����ʱ�������
    if(index == 0)
    {
        Gui_PageClearBackground(0, 
                                icon_16_heart_icon_coord[0][1] + ICON_HEART_ICON4_HIGH, 
                                MAX_X_SIZE,
                                MAX_Y_SIZE -
                                (icon_16_heart_icon_coord[0][1] + ICON_HEART_ICON4_HIGH));
    }
    
    // �ϴ����ʱ�ǩ��ʾ
    {
        LCD_SetPosition(icon_16_heart_lastlabel_coord[0][0], icon_16_heart_lastlabel_coord[0][1], 
                       ICON_HEART_ICON3_WIDE, ICON_HEART_ICON3_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_heart_lastlabel_addr[language % 2], ICON_HEART_ICON3_SIZE);
    }
    
    // �ϴ�����ֵ��ʾ
    if(index == 0)
    {
        if(last_heart > 255)
        {
            last_heart = 255;
        }
        
        if(last_heart >= 100)
        {
            LCD_SetPosition(icon_16_heart_lastdata_coord[0][0], icon_16_heart_lastdata_coord[0][1], 
                            ICON_HEART_DATA3_WIDE, ICON_HEART_DATA3_HIGH);;
            if(last_heart >= 100)
            {    
                LCD_DriverWriteDataFromExtFlash(icon_16_heart_lastdata_addr[(last_heart / 100) %10], ICON_HEART_DATA3_SIZE);        
            }  
            else
            {    
                LCD_DriverWriteDataFromExtFlash(icon_16_heart_lastdata_addr[0], ICON_HEART_DATA3_SIZE);        
            }
        }
        
        LCD_SetPosition(icon_16_heart_lastdata_coord[1][0], icon_16_heart_lastdata_coord[1][1], 
                        ICON_HEART_DATA3_WIDE, ICON_HEART_DATA3_HIGH);
        if(last_heart >= 10)
        {    
            LCD_DriverWriteDataFromExtFlash(icon_16_heart_lastdata_addr[(last_heart / 10) %10], ICON_HEART_DATA3_SIZE);        
        }  
        else
        {    
            LCD_DriverWriteDataFromExtFlash(icon_16_heart_lastdata_addr[0], ICON_HEART_DATA3_SIZE);        
        }
        
        LCD_SetPosition(icon_16_heart_lastdata_coord[2][0], icon_16_heart_lastdata_coord[2][1], 
                        ICON_HEART_DATA3_WIDE, ICON_HEART_DATA3_HIGH);
        if(last_heart > 0)
        {    
            LCD_DriverWriteDataFromExtFlash(icon_16_heart_lastdata_addr[last_heart %10], ICON_HEART_DATA3_SIZE);        
        }  
        else
        {    
            LCD_DriverWriteDataFromExtFlash(icon_16_heart_lastdata_addr[0], ICON_HEART_DATA3_SIZE);        
        }
    }
    
    // �ϴ����ʵ�λ��ʾ
    {    
        LCD_SetPosition(icon_16_heart_lastunit_coord[0][0], icon_16_heart_lastunit_coord[0][1], 
                       ICON_HEART_UNIT2_WIDE, ICON_HEART_UNIT2_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_heart_lastunit_addr[language % 2], ICON_HEART_UNIT2_SIZE);
    }
}

/****************************************************************
** Function name:           Gui_PageSleep
** Descriptions:            ˯�߽���
** input parameters:        index����̬ͼ���� 
**                          time��˯�ߵ�ʱ��
**                          language����λ��ʾ������
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2018-05-24
**----------------------------------------------------------------
*****************************************************************/ 
void Gui_PageSleep(uint16_t index, uint16_t time, uint16_t deep_time, 
                   uint16_t light_time, uint8_t quality, uint8_t language)
{
    uint8_t sleep_hours = time / 60;
    uint8_t sleep_minutes = time % 60;
    uint8_t deep_sleep_hours = deep_time / 60;
    uint8_t deep_sleep_minutes =deep_time % 60;
    uint8_t light_sleep_hours = light_time / 60;
    uint8_t light_sleep_minutes = light_time % 60;
    
    if(language)
    {
        language = 1;
    }
    
    // ��˯�ϲ�����
    if(index == 0)
    {
        Gui_PageClearBackground(0, 
                                0, 
                                MAX_X_SIZE,
                                icon_16_sleep_deep_bg_coord[0][1]);
    }
    
/////////////---------------------------------��˯-------------------------------------//////////////////
    
    // ��˯������
    if(index == 0)
    {
        Gui_PageClearBackground(0, 
                                icon_16_sleep_deep_bg_coord[0][1], 
                                icon_16_sleep_deep_bg_coord[0][0],
                                ICON_SLEEP_ICON3_HIGH);
    }
    
    // ��˯ zzz
    if(index == 0)
    {
        LCD_SetPosition(icon_16_sleep_deep_icon_coord[0][0], icon_16_sleep_deep_icon_coord[0][1], 
                        ICON_SLEEP_ICON1_WIDE, ICON_SLEEP_ICON1_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_sleep_deep_icon_addr[0], ICON_SLEEP_ICON1_SIZE); 
    }
    
    // ��˯ʱ��
    if(index == 0)
    {
        LCD_SetPosition(icon_16_sleep_deep_bg_coord[0][0], icon_16_sleep_deep_bg_coord[0][1], 
                        ICON_SLEEP_ICON3_WIDE, ICON_SLEEP_ICON3_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_sleep_deep_bg_addr[0], ICON_SLEEP_ICON3_SIZE);
        
        LCD_SetPosition(icon_16_sleep_deep_label_coord[0][0], icon_16_sleep_deep_label_coord[0][1], 
                        ICON_SLEEP_ICON2_WIDE, ICON_SLEEP_ICON2_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_sleep_deep_label_addr[language % 2], ICON_SLEEP_ICON2_SIZE);
    }
    
    // ��˯ʱ����ʾ
    if(index == 0)
    {
        // Сʱ-��ֵ��ʾ
        LCD_SetPosition(icon_16_sleep_deep_num_coord[0][0], icon_16_sleep_deep_num_coord[0][1], 
                        ICON_SLEEP_DATA1_WIDE, ICON_SLEEP_DATA1_HIGH);
        if(deep_sleep_hours >= 10)
        {    
            LCD_DriverWriteDataFromExtFlash(icon_16_sleep_deep_num_addr[(deep_sleep_hours / 10) %10], ICON_SLEEP_DATA1_SIZE);        
        }  
        else
        {    
            LCD_DriverWriteDataFromExtFlash(icon_16_sleep_deep_num_addr[0], ICON_SLEEP_DATA1_SIZE);        
        }

        LCD_SetPosition(icon_16_sleep_deep_num_coord[1][0], icon_16_sleep_deep_num_coord[1][1], 
                        ICON_SLEEP_DATA1_WIDE, ICON_SLEEP_DATA1_HIGH);
        if(deep_sleep_hours > 0)
        {    
            LCD_DriverWriteDataFromExtFlash(icon_16_sleep_deep_num_addr[deep_sleep_hours %10], ICON_SLEEP_DATA1_SIZE);        
        }  
        else
        {    
            LCD_DriverWriteDataFromExtFlash(icon_16_sleep_deep_num_addr[0], ICON_SLEEP_DATA1_SIZE);        
        }
        
        // Сʱ-��λ��ʾ
        LCD_SetPosition(icon_16_sleep_deep_unit_coord[0][0], icon_16_sleep_deep_unit_coord[0][1], 
                        icon_16_sleep_deep_unit_wide[language % 2][0], icon_16_sleep_deep_unit_high[language % 2][0]);
        LCD_DriverWriteDataFromExtFlash(icon_16_sleep_deep_unit_addr[language % 2][0], icon_16_sleep_deep_unit_size[language % 2][0]);
        
        // ����-��ֵ��ʾ
        LCD_SetPosition(icon_16_sleep_deep_num_coord[2][0] + icon_16_sleep_deep_unit_wide[language % 2][0], icon_16_sleep_deep_num_coord[2][1], 
                        ICON_SLEEP_DATA1_WIDE, ICON_SLEEP_DATA1_HIGH);
        if(deep_sleep_minutes >= 10)
        {    
            LCD_DriverWriteDataFromExtFlash(icon_16_sleep_deep_num_addr[(deep_sleep_minutes / 10) % 10], ICON_SLEEP_DATA1_SIZE);        
        }  
        else
        {    
            LCD_DriverWriteDataFromExtFlash(icon_16_sleep_deep_num_addr[0], ICON_SLEEP_DATA1_SIZE);        
        }

        LCD_SetPosition(icon_16_sleep_deep_num_coord[3][0] + icon_16_sleep_deep_unit_wide[language % 2][0], icon_16_sleep_deep_num_coord[3][1], 
                        ICON_SLEEP_DATA1_WIDE, ICON_SLEEP_DATA1_HIGH);
        if(deep_sleep_minutes > 0)
        {    
            LCD_DriverWriteDataFromExtFlash(icon_16_sleep_deep_num_addr[deep_sleep_minutes % 10], ICON_SLEEP_DATA1_SIZE);        
        }  
        else
        {    
            LCD_DriverWriteDataFromExtFlash(icon_16_sleep_deep_num_addr[0], ICON_SLEEP_DATA1_SIZE);        
        }
        
        // ����-��λ��ʾ
        LCD_SetPosition(icon_16_sleep_deep_unit_coord[1][0] + icon_16_sleep_deep_unit_wide[language % 2][0], icon_16_sleep_deep_unit_coord[1][1], 
                        icon_16_sleep_deep_unit_wide[language % 2][1], icon_16_sleep_deep_unit_high[language % 2][1]);
        LCD_DriverWriteDataFromExtFlash(icon_16_sleep_deep_unit_addr[language % 2][1], icon_16_sleep_deep_unit_size[language % 2][1]);
    }
    
    // ��˯�Ҳ�����
    if(index == 0)
    {
        Gui_PageClearBackground(icon_16_sleep_deep_bg_coord[0][0] + ICON_SLEEP_ICON3_WIDE, 
                                icon_16_sleep_deep_bg_coord[0][1], 
                                MAX_X_SIZE - (icon_16_sleep_deep_bg_coord[0][0] + ICON_SLEEP_ICON3_WIDE),
                                ICON_SLEEP_ICON3_HIGH);
    }
    
    // ��˯-ǳ˯-��ֱ���-����
    if(index == 0)
    {
        Gui_PageClearBackground(0, 
                                icon_16_sleep_deep_bg_coord[0][1] + ICON_SLEEP_ICON3_HIGH, 
                                MAX_X_SIZE,
                                icon_16_sleep_light_bg_coord[0][1] - (icon_16_sleep_deep_bg_coord[0][1] + ICON_SLEEP_ICON3_HIGH));
    }
    
/////////////---------------------------------ǳ˯-------------------------------------//////////////////    
    
    // ǳ˯������
    if(index == 0)
    {
        Gui_PageClearBackground(0, 
                                icon_16_sleep_light_bg_coord[0][1], 
                                icon_16_sleep_light_bg_coord[0][0],
                                ICON_SLEEP_ICON6_HIGH);
    }
    
    // ǳ˯ z
    if(index == 0)
    {
        LCD_SetPosition(icon_16_sleep_light_icon_coord[0][0], icon_16_sleep_light_icon_coord[0][1], 
                        ICON_SLEEP_ICON4_WIDE, ICON_SLEEP_ICON4_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_sleep_light_icon_addr[0], ICON_SLEEP_ICON4_SIZE); 
    }
    
    // ǳ˯ʱ��
    if(index == 0)
    {
        LCD_SetPosition(icon_16_sleep_light_bg_coord[0][0], icon_16_sleep_light_bg_coord[0][1], 
                        ICON_SLEEP_ICON6_WIDE, ICON_SLEEP_ICON6_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_sleep_light_bg_addr[0], ICON_SLEEP_ICON6_SIZE);
        
        LCD_SetPosition(icon_16_sleep_light_label_coord[0][0], icon_16_sleep_light_label_coord[0][1], 
                        ICON_SLEEP_ICON5_WIDE, ICON_SLEEP_ICON5_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_sleep_light_label_addr[language % 2], ICON_SLEEP_ICON5_SIZE);
    }
    
    // ǳ˯ʱ����ʾ
    if(index == 0)
    {
        // Сʱ-��ֵ��ʾ
        LCD_SetPosition(icon_16_sleep_light_num_coord[0][0], icon_16_sleep_light_num_coord[0][1], 
                        ICON_SLEEP_DATA2_WIDE, ICON_SLEEP_DATA2_HIGH);
        if(light_sleep_hours >= 10)
        {    
            LCD_DriverWriteDataFromExtFlash(icon_16_sleep_light_num_addr[(light_sleep_hours / 10) %10], ICON_SLEEP_DATA2_SIZE);        
        }  
        else
        {    
            LCD_DriverWriteDataFromExtFlash(icon_16_sleep_light_num_addr[0], ICON_SLEEP_DATA2_SIZE);        
        }

        LCD_SetPosition(icon_16_sleep_light_num_coord[1][0], icon_16_sleep_light_num_coord[1][1], 
                        ICON_SLEEP_DATA2_WIDE, ICON_SLEEP_DATA2_HIGH);
        if(light_sleep_hours > 0)
        {    
            LCD_DriverWriteDataFromExtFlash(icon_16_sleep_light_num_addr[light_sleep_hours %10], ICON_SLEEP_DATA2_SIZE);        
        }  
        else
        {    
            LCD_DriverWriteDataFromExtFlash(icon_16_sleep_light_num_addr[0], ICON_SLEEP_DATA2_SIZE);        
        }
        
        // Сʱ-��λ��ʾ
        LCD_SetPosition(icon_16_sleep_light_unit_coord[0][0], icon_16_sleep_light_unit_coord[0][1], 
                        icon_16_sleep_light_unit_wide[language % 2][0], icon_16_sleep_light_unit_high[language % 2][0]);
        LCD_DriverWriteDataFromExtFlash(icon_16_sleep_light_unit_addr[language % 2][0], icon_16_sleep_light_unit_size[language % 2][0]);
        
        // ����-��ֵ��ʾ
        LCD_SetPosition(icon_16_sleep_light_num_coord[2][0] + icon_16_sleep_light_unit_wide[language % 2][0], icon_16_sleep_light_num_coord[2][1], 
                        ICON_SLEEP_DATA2_WIDE, ICON_SLEEP_DATA2_HIGH);
        if(light_sleep_minutes >= 10)
        {    
            LCD_DriverWriteDataFromExtFlash(icon_16_sleep_light_num_addr[(light_sleep_minutes / 10) % 10], ICON_SLEEP_DATA2_SIZE);        
        }  
        else
        {    
            LCD_DriverWriteDataFromExtFlash(icon_16_sleep_light_num_addr[0], ICON_SLEEP_DATA2_SIZE);        
        }

        LCD_SetPosition(icon_16_sleep_light_num_coord[3][0] + icon_16_sleep_light_unit_wide[language % 2][0], icon_16_sleep_light_num_coord[3][1], 
                        ICON_SLEEP_DATA2_WIDE, ICON_SLEEP_DATA2_HIGH);
        if(light_sleep_minutes > 0)
        {    
            LCD_DriverWriteDataFromExtFlash(icon_16_sleep_light_num_addr[light_sleep_minutes % 10], ICON_SLEEP_DATA2_SIZE);        
        }  
        else
        {    
            LCD_DriverWriteDataFromExtFlash(icon_16_sleep_light_num_addr[0], ICON_SLEEP_DATA2_SIZE);        
        }
        
        // ����-��λ��ʾ
        LCD_SetPosition(icon_16_sleep_light_unit_coord[1][0] + icon_16_sleep_light_unit_wide[language % 2][0], icon_16_sleep_light_unit_coord[1][1], 
                        icon_16_sleep_light_unit_wide[language % 2][1], icon_16_sleep_light_unit_high[language % 2][1]);
        LCD_DriverWriteDataFromExtFlash(icon_16_sleep_light_unit_addr[language % 2][1], icon_16_sleep_light_unit_size[language % 2][1]);
    }
    
    // ǳ˯�Ҳ�����
    if(index == 0)
    {
        Gui_PageClearBackground(icon_16_sleep_light_bg_coord[0][0] + ICON_SLEEP_ICON6_WIDE, 
                                icon_16_sleep_light_bg_coord[0][1], 
                                MAX_X_SIZE - (icon_16_sleep_light_bg_coord[0][0] + ICON_SLEEP_ICON6_WIDE),
                                ICON_SLEEP_ICON6_HIGH);
    }
    
//////////////////////--------------------˯��--------------------////////////////////////////////
        
    // ˯��ʱ��-��������
    if(index == 0)
    {
        Gui_PageClearBackground(0, 
                                icon_16_sleep_light_bg_coord[0][1] + ICON_SLEEP_ICON6_HIGH, 
                                MAX_X_SIZE,
                                icon_16_sleep_stars_icon_coord[0][1] - (icon_16_sleep_light_bg_coord[0][1] + ICON_SLEEP_ICON6_HIGH));
    }
    
    // ˯��ʱ��
    if(index == 0)
    {
        LCD_SetPosition(icon_16_sleep_time_icon_coord[0][0], icon_16_sleep_time_icon_coord[0][1], 
                        ICON_SLEEP_ICON7_WIDE, ICON_SLEEP_ICON7_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_sleep_time_icon_addr[language % 2], ICON_SLEEP_ICON7_SIZE);
    }
    
    // ˯��ʱ����ʾ
    if(index == 0)
    {
        // Сʱ-��ֵ��ʾ
        LCD_SetPosition(icon_16_sleep_num_coord[0][0], icon_16_sleep_num_coord[0][1], 
                        ICON_SLEEP_DATA3_WIDE, ICON_SLEEP_DATA3_HIGH);
        if(sleep_hours >= 10)
        {    
            LCD_DriverWriteDataFromExtFlash(icon_16_sleep_num_addr[(sleep_hours / 10) %10], ICON_SLEEP_DATA3_SIZE);        
        }  
        else
        {    
            LCD_DriverWriteDataFromExtFlash(icon_16_sleep_num_addr[0], ICON_SLEEP_DATA3_SIZE);        
        }

        LCD_SetPosition(icon_16_sleep_num_coord[1][0], icon_16_sleep_num_coord[1][1], 
                        ICON_SLEEP_DATA3_WIDE, ICON_SLEEP_DATA3_HIGH);
        if(sleep_hours > 0)
        {    
            LCD_DriverWriteDataFromExtFlash(icon_16_sleep_num_addr[sleep_hours %10], ICON_SLEEP_DATA3_SIZE);        
        }  
        else
        {    
            LCD_DriverWriteDataFromExtFlash(icon_16_sleep_num_addr[0], ICON_SLEEP_DATA3_SIZE);        
        }
        
        // Сʱ-��λ��ʾ
        LCD_SetPosition(icon_16_sleep_unit_coord[0][0], icon_16_sleep_unit_coord[0][1], 
                        icon_16_sleep_unit_wide[language % 2][0], icon_16_sleep_unit_high[language % 2][0]);
        LCD_DriverWriteDataFromExtFlash(icon_16_sleep_unit_addr[language % 2][0], icon_16_sleep_unit_size[language % 2][0]);
        
        // ����-��ֵ��ʾ
        LCD_SetPosition(icon_16_sleep_num_coord[2][0] + icon_16_sleep_unit_wide[language % 2][0], icon_16_sleep_num_coord[2][1], 
                        ICON_SLEEP_DATA3_WIDE, ICON_SLEEP_DATA3_HIGH);
        if(sleep_minutes >= 10)
        {    
            LCD_DriverWriteDataFromExtFlash(icon_16_sleep_num_addr[(sleep_minutes / 10) % 10], ICON_SLEEP_DATA3_SIZE);        
        }  
        else
        {    
            LCD_DriverWriteDataFromExtFlash(icon_16_sleep_num_addr[0], ICON_SLEEP_DATA3_SIZE);        
        }

        LCD_SetPosition(icon_16_sleep_num_coord[3][0] + icon_16_sleep_unit_wide[language % 2][0], icon_16_sleep_num_coord[3][1], 
                        ICON_SLEEP_DATA3_WIDE, ICON_SLEEP_DATA3_HIGH);
        if(sleep_minutes > 0)
        {    
            LCD_DriverWriteDataFromExtFlash(icon_16_sleep_num_addr[sleep_minutes % 10], ICON_SLEEP_DATA3_SIZE);        
        }  
        else
        {    
            LCD_DriverWriteDataFromExtFlash(icon_16_sleep_num_addr[0], ICON_SLEEP_DATA3_SIZE);        
        }
        
        // ����-��λ��ʾ
        LCD_SetPosition(icon_16_sleep_unit_coord[1][0] + icon_16_sleep_unit_wide[language % 2][0], icon_16_sleep_unit_coord[1][1], 
                        icon_16_sleep_unit_wide[language % 2][1], icon_16_sleep_unit_high[language % 2][1]);
        LCD_DriverWriteDataFromExtFlash(icon_16_sleep_unit_addr[language % 2][1], icon_16_sleep_unit_size[language % 2][1]);
    }
    
//////////////////////--------------------˯������--------------------////////////////////////////////  

    // ˯��������������
    if(index == 0)
    {
        Gui_PageClearBackground(0, 
                                icon_16_sleep_stars_icon_coord[0][1], 
                                MAX_X_SIZE,
                                MAX_Y_SIZE - icon_16_sleep_stars_icon_coord[0][1]);
    }
    
    // ˯������ͼ��
    if(index == 0)
    {
        LCD_SetPosition(icon_16_sleep_quality_icon_coord[0][0], icon_16_sleep_quality_icon_coord[0][1], 
                        ICON_SLEEP_ICON8_WIDE, ICON_SLEEP_ICON8_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_sleep_quality_icon_addr[language % 2], ICON_SLEEP_ICON8_SIZE);
    }
    
    // ������ʾ
    if(index == 0)
    {
        if(quality >= 5)
        {
            quality = 5;
        }
        
        for(uint32_t i = 0; i < 5; i++)
        {
            if(i < quality)
            {
                LCD_SetPosition(icon_16_sleep_stars_icon_coord[i][0], icon_16_sleep_stars_icon_coord[i][1], 
                                ICON_SLEEP_ICON9_WIDE, ICON_SLEEP_ICON9_HIGH);
                LCD_DriverWriteDataFromExtFlash(icon_16_sleep_stars_icon_addr[0], ICON_SLEEP_ICON9_SIZE);
            }
            else
            {
                LCD_SetPosition(icon_16_sleep_stars_icon_coord[i][0], icon_16_sleep_stars_icon_coord[i][1], 
                                ICON_SLEEP_ICON10_WIDE, ICON_SLEEP_ICON10_HIGH);
                LCD_DriverWriteDataFromExtFlash(icon_16_sleep_stars_icon_addr[1], ICON_SLEEP_ICON10_SIZE);
            }
        }
    }
}

/****************************************************************
** Function name:           Gui_PagePowerOn
** Descriptions:            ��������
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2018-05-25
**----------------------------------------------------------------
*****************************************************************/ 
void Gui_PagePowerOn(uint16_t index)
{
    // �ػ�ͼ���ϲ�����
    if(index == 0)
    {
        Gui_PageClearBackground(0, 
                                0, 
                                MAX_X_SIZE,
                                icon_16_poweron_icon_coord[0][1]);
    }
    
    // �ػ�ͼ��������
    if(index == 0)
    {
        Gui_PageClearBackground(0, 
                                icon_16_poweron_icon_coord[0][1], 
                                icon_16_poweron_icon_coord[0][0],
                                ICON_POWERON_ICON_HIGH);
    }
    
    // �ػ�ͼ��
    {
        LCD_SetPosition(icon_16_poweron_icon_coord[0][0], icon_16_poweron_icon_coord[0][1], 
                        ICON_POWERON_ICON_WIDE, ICON_POWERON_ICON_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_poweron_icon_addr[index % 15], ICON_POWERON_ICON_SIZE);
    }
    
    // �ػ�ͼ��������
    if(index == 0)
    {
        
        Gui_PageClearBackground(icon_16_poweron_icon_coord[0][0] + ICON_POWERON_ICON_WIDE, 
                                icon_16_poweron_icon_coord[0][1], 
                                MAX_X_SIZE - (icon_16_poweron_icon_coord[0][0] + ICON_POWERON_ICON_WIDE),
                                ICON_POWERON_ICON_HIGH);
    }
    
    // ��Ļ�ײ�����
    if(index == 0)
    {
        Gui_PageClearBackground(0, 
                                icon_16_poweron_icon_coord[0][1] + ICON_POWERON_ICON_HIGH, 
                                MAX_X_SIZE,
                                MAX_Y_SIZE - (icon_16_poweron_icon_coord[0][1] + ICON_POWERON_ICON_HIGH));
    }
}

/****************************************************************
** Function name:           Gui_PageTurnOff
** Descriptions:            �ػ�����
** input parameters:        
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2018-05-25
**----------------------------------------------------------------
*****************************************************************/ 
void Gui_PageTurnOff(uint16_t index, uint8_t *ble_name, uint8_t *mac_data, uint8_t *ver_data,uint8_t language)
{
	// �ػ�ͼ����������
	if(index == 0)
	{
		Gui_PageClearBackground(0, 
								0, 
								MAX_X_SIZE,
								icon_16_poweroff_nokey_icon_coord[0][1]);
		
		Gui_PageClearBackground(0, 
								icon_16_poweroff_nokey_icon_coord[0][1], 
								icon_16_poweroff_nokey_icon_coord[0][0],
								ICON_POWEROFF_ICON1_HIGH);
	}


	// �ػ�ͼ�����²�����
	if(index == 0)
	{
		Gui_PageClearBackground(icon_16_poweroff_nokey_icon_coord[0][0] + ICON_POWEROFF_ICON1_WIDE, 
								icon_16_poweroff_nokey_icon_coord[0][1], 
								MAX_X_SIZE -(icon_16_poweroff_nokey_icon_coord[0][0] + ICON_POWEROFF_ICON1_WIDE),
								ICON_POWEROFF_ICON1_HIGH);
		
		Gui_PageClearBackground(0, 
								icon_16_poweroff_nokey_icon_coord[0][1] + ICON_POWEROFF_ICON1_HIGH, 
								MAX_X_SIZE,
								MAX_Y_SIZE - (icon_16_poweroff_nokey_icon_coord[0][1] + ICON_POWEROFF_ICON1_HIGH));
	}

	
	// ͼƬ��ʾ
	if(index == 0)
	{
		LCD_SetPosition(icon_16_poweroff_nokey_icon_coord[0][0], icon_16_poweroff_nokey_icon_coord[0][1], 
						ICON_ZZPOWEROFF_ICON1_WIDE, ICON_ZZPOWEROFF_ICON1_HIGH);
		LCD_DriverWriteDataFromExtFlash(icon_16_poweroff_nokey_icon_addr[0], ICON_ZZPOWEROFF_ICON1_SIZE);       
	}
	

	
	if(index == 0)
	{
		char buf_tmp[40];
		
		uint8_t buf_index = 0;
		uint8_t temp_data = 0;
		
		//�豸����
		{
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
			
			Font_DisplayTitleText((uint8_t *)buf_tmp, buf_index,170, 1);
		}
		
		// MAC��ַ
		{
			buf_index = 0;
			
			buf_tmp[buf_index++] = 'M';
			buf_tmp[buf_index++] = 'A';
			buf_tmp[buf_index++] = 'C';
			buf_tmp[buf_index++] = ':';
			buf_tmp[buf_index++] = ' ';
			temp_data = (mac_data[1] / 16) % 16;
			if(temp_data >= 0x0A && temp_data <= 0x0F)
			{
				buf_tmp[buf_index++] = temp_data + 'A' - 10;
			}
			else if(temp_data <= 0x09)
			{
				buf_tmp[buf_index++] = temp_data +'0';
			}
			temp_data = mac_data[1] % 16;
			if(temp_data >= 0x0A && temp_data <= 0x0F)
			{
				buf_tmp[buf_index++] = temp_data + 'A' - 10;
			}
			else if(temp_data <= 0x09)
			{
				buf_tmp[buf_index++] = temp_data +'0';
			}
			buf_tmp[buf_index++] = ':';
			temp_data = (mac_data[0] / 16) % 16;
			if(temp_data >= 0x0A && temp_data <= 0x0F)
			{
				buf_tmp[buf_index++] = temp_data + 'A' - 10;
			}
			else if(temp_data <= 0x09)
			{
				buf_tmp[buf_index++] = temp_data +'0';
			}
			temp_data = mac_data[0] % 16;
			if(temp_data >= 0x0A && temp_data <= 0x0F)
			{
				buf_tmp[buf_index++] = temp_data + 'A' - 10;
			}
			else if(temp_data <= 0x09)
			{
				buf_tmp[buf_index++] = temp_data +'0';
			}
     
			Font_DisplayTitleText((uint8_t *)buf_tmp, buf_index,190, 1);
		}
		
		// �汾��
		{		
			buf_index = 0;
			
			if(language == 0)
			{
				char name[10] = {0xE7, 0x89, 0x88,0xE6, 0x9C, 0xAC, 0xE5, 0x8F, 0xB7,0}; 
				strcpy(buf_tmp, name);
				buf_index = strlen(buf_tmp);
			}
			else
			{
				char name[10] = "VERSION"; 
				strcpy(buf_tmp, name);
				buf_index = strlen(buf_tmp);
			}
			
			buf_tmp[buf_index++] = ':';
			buf_tmp[buf_index++] = ' ';
			buf_tmp[buf_index++] = ver_data[0] / 16 + '0';
			buf_tmp[buf_index++] = ver_data[0] % 16 + '0';
			buf_tmp[buf_index++] = '.';
			buf_tmp[buf_index++] = ver_data[1] / 16 + '0';
			buf_tmp[buf_index++] = ver_data[1] % 16 + '0';
			
			Font_DisplayTitleText((uint8_t *)buf_tmp, buf_index, 210, 1);
		}
	}
}
/****************************************************************
** Function name:           Gui_PagePowerOff
** Descriptions:            �ػ�����
** input parameters:        
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2018-05-25
**----------------------------------------------------------------
*****************************************************************/ 
void Gui_PagePowerOff(uint16_t index)
{
	// �ػ�ͼ���ϲ�����
	if(index == 0)
	{
		Gui_PageClearBackground(0, 
								0, 
								MAX_X_SIZE,
								icon_16_poweroff_icon_coord[0][1]);
	}
	
	// �ػ�ͼ��������
	if(index == 0)
	{
		Gui_PageClearBackground(0, 
								icon_16_poweroff_icon_coord[0][1], 
								icon_16_poweroff_icon_coord[0][0],
								ICON_POWEROFF_ICON_HIGH);
	}
	
	// �ػ�ͼ��
	{
		LCD_SetPosition(icon_16_poweroff_icon_coord[0][0], icon_16_poweroff_icon_coord[0][1], 
						ICON_POWEROFF_ICON_WIDE, ICON_POWEROFF_ICON_HIGH);
		LCD_DriverWriteDataFromExtFlash(icon_16_poweroff_icon_addr[index % 15], ICON_POWEROFF_ICON_SIZE);
	}
	
	// �ػ�ͼ��������
	if(index == 0)
	{
		
		Gui_PageClearBackground(icon_16_poweroff_icon_coord[0][0] + ICON_POWEROFF_ICON_WIDE, 
								icon_16_poweroff_icon_coord[0][1], 
								MAX_X_SIZE - (icon_16_poweroff_icon_coord[0][0] + ICON_POWEROFF_ICON_WIDE),
								ICON_POWEROFF_ICON_HIGH);
	}
	
	// ��Ļ�ײ�����
	if(index == 0)
	{
		Gui_PageClearBackground(0, 
								icon_16_poweroff_icon_coord[0][1] + ICON_POWEROFF_ICON_HIGH, 
								MAX_X_SIZE,
								MAX_Y_SIZE - (icon_16_poweroff_icon_coord[0][1] + ICON_POWEROFF_ICON_HIGH));
	}
}

/****************************************************************
** Function name:           Gui_PageFindPhone
** Descriptions:            ���ֻ�����
** input parameters:        dynamic_find���Ƿ�����̬ͼ
**                          index����̬ͼ����
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2018-05-25
**----------------------------------------------------------------
*****************************************************************/ 
void Gui_PageFindPhone(bool dynamic_find, uint16_t index, uint8_t language)
{ 
	if(language)
	{
		language = 1;
	}
			
    // ��̬ͼ��
    if(dynamic_find == 0)
    {
        // ��ͼ�ϲ�����
        if(index == 0)
        {
            Gui_PageClearBackground(0, 
                                    0, 
                                    MAX_X_SIZE,
                                    icon_16_findphone_bg1_coord[0][1]);
        }
        
        // ����1������
        if(index == 0)
        {
            Gui_PageClearBackground(0, 
                                    icon_16_findphone_bg1_coord[0][1], 
                                    icon_16_findphone_bg1_coord[0][0],
                                    ICON_FINDPHONE_ICON1_HIGH);
        }
        
        // ����1��ʾ
        if(index == 0)
        {
            LCD_SetPosition(icon_16_findphone_bg1_coord[0][0], icon_16_findphone_bg1_coord[0][1], 
                            ICON_FINDPHONE_ICON1_WIDE, ICON_FINDPHONE_ICON1_HIGH);
            LCD_DriverWriteDataFromExtFlash(icon_16_findphone_bg1_addr[0], ICON_FINDPHONE_ICON1_SIZE);
        }
        
        // ����1������
        if(index == 0)
        {           
            Gui_PageClearBackground(icon_16_findphone_bg1_coord[0][0] + ICON_FINDPHONE_ICON1_WIDE, 
                                    icon_16_findphone_bg1_coord[0][1], 
                                    MAX_X_SIZE - (icon_16_findphone_bg1_coord[0][0] + ICON_FINDPHONE_ICON1_WIDE),
                                    ICON_FINDPHONE_ICON1_HIGH);
        }
        
        // ��ͼ������
        if(index == 0)
        {
            Gui_PageClearBackground(0, 
                                    icon_16_findphone_static_coord[0][1], 
                                    icon_16_findphone_static_coord[0][0],
                                    ICON_FINDPHONE_ICON2_HIGH);
        }
        
        // ��ͼ��ʾ
        if(index == 0)
        {
            LCD_SetPosition(icon_16_findphone_static_coord[0][0], icon_16_findphone_static_coord[0][1], 
                            ICON_FINDPHONE_ICON2_WIDE, ICON_FINDPHONE_ICON2_HIGH);
            LCD_DriverWriteDataFromExtFlash(icon_16_findphone_static_addr[3], ICON_FINDPHONE_ICON2_SIZE);
        }
        
        // ��ͼ������
        if(index == 0)
        {           
            Gui_PageClearBackground(icon_16_findphone_static_coord[0][0] + ICON_FINDPHONE_ICON2_WIDE, 
                                    icon_16_findphone_static_coord[0][1], 
                                    MAX_X_SIZE - (icon_16_findphone_static_coord[0][0] + ICON_FINDPHONE_ICON2_WIDE),
                                    ICON_FINDPHONE_ICON2_HIGH);
        }
        
        // ����2������
        if(index == 0)
        {
            Gui_PageClearBackground(0, 
                                    icon_16_findphone_bg2_coord[0][1], 
                                    icon_16_findphone_bg2_coord[0][0],
                                    ICON_FINDPHONE_ICON3_HIGH);
        }
        
        // ����2��ʾ
        if(index == 0)
        {
            LCD_SetPosition(icon_16_findphone_bg2_coord[0][0], icon_16_findphone_bg2_coord[0][1], 
                            ICON_FINDPHONE_ICON3_WIDE, ICON_FINDPHONE_ICON3_HIGH);
            LCD_DriverWriteDataFromExtFlash(icon_16_findphone_bg2_addr[0], ICON_FINDPHONE_ICON3_SIZE);
        }
        
        // ����2������
        if(index == 0)
        {           
            Gui_PageClearBackground(icon_16_findphone_bg2_coord[0][0] + ICON_FINDPHONE_ICON3_WIDE, 
                                    icon_16_findphone_bg2_coord[0][1], 
                                    MAX_X_SIZE - (icon_16_findphone_bg2_coord[0][0] + ICON_FINDPHONE_ICON3_WIDE),
                                    ICON_FINDPHONE_ICON3_HIGH);
        }
        // ����2�²�����
        if(index == 0)
        {
            Gui_PageClearBackground(0, 
                                    icon_16_findphone_bg2_coord[0][1] + ICON_FINDPHONE_ICON3_HIGH, 
                                    MAX_X_SIZE,
                                    icon_16_findphone_unit_coord[0][1] - (icon_16_findphone_bg2_coord[0][1] + ICON_FINDPHONE_ICON3_HIGH));
        }
		
		// ������ʾ��
		{
			LCD_SetPosition(icon_16_long_press_icon_coord[0][0], icon_16_long_press_icon_coord[0][1], 
							ICON_FINDPHONE_ICON5_WIDE, ICON_FINDPHONE_ICON5_HIGH);
			LCD_DriverWriteDataFromExtFlash(icon_16_long_press_icon_addr[language % 2], ICON_FINDPHONE_ICON5_SIZE);
		}
        
        // ������ʾ������
        if(index == 0)
        {
            Gui_PageClearBackground(0, 
                                    icon_16_findphone_unit_coord[0][1], 
                                    icon_16_findphone_unit_coord[0][0],
                                    ICON_FINDPHONE_ICON4_HIGH);
        }
        
        // ������ʾ"FindPhone"
        {   
            LCD_SetPosition(icon_16_findphone_unit_coord[0][0], icon_16_findphone_unit_coord[0][1], 
                            ICON_FINDPHONE_ICON4_WIDE, ICON_FINDPHONE_ICON4_HIGH);
            LCD_DriverWriteDataFromExtFlash(icon_16_findphone_unit_addr[language % 2], ICON_FINDPHONE_ICON4_SIZE);
        }
        
        // ������ʾ������
        if(index == 0)
        {           
            Gui_PageClearBackground(icon_16_findphone_unit_coord[0][0] + ICON_FINDPHONE_ICON4_WIDE, 
                                    icon_16_findphone_unit_coord[0][1], 
                                    MAX_X_SIZE - (icon_16_findphone_unit_coord[0][0] + ICON_FINDPHONE_ICON4_WIDE),
                                    ICON_FINDPHONE_ICON4_HIGH);
        }        
        // ������ʾ�²�����
        if(index == 0)
        {
            Gui_PageClearBackground(0, 
                                    icon_16_findphone_unit_coord[0][1] + ICON_FINDPHONE_ICON4_HIGH, 
                                    MAX_X_SIZE,
                                    MAX_Y_SIZE - (icon_16_findphone_unit_coord[0][1] + ICON_FINDPHONE_ICON4_WIDE));
        }
    }
    else
    {
        // ��ͼ�ϲ�����
        if(index == 0)
        {
            Gui_PageClearBackground(0, 
                                    0, 
                                    MAX_X_SIZE,
                                    icon_16_findphone2_center_coord[0][1]);
        }
        
        // 2������-�м侲̬ͼ��������
        if(index == 0)
        {
            Gui_PageClearBackground(0, 
                                    icon_16_findphone2_center_coord[0][1], 
                                    icon_16_findphone2_center_coord[0][0],
                                    ICON_FINDPHONE2_ICON9_HIGH);
        }
        
        // 2������-�м侲̬ͼ����ʾ
        if(index == 0)
        {
            LCD_SetPosition(icon_16_findphone2_center_coord[0][0], icon_16_findphone2_center_coord[0][1], 
                            ICON_FINDPHONE2_ICON9_WIDE, ICON_FINDPHONE2_ICON9_HIGH);
            LCD_DriverWriteDataFromExtFlash(icon_16_findphone2_center_addr[0], ICON_FINDPHONE2_ICON9_SIZE);
        }
        
        // 2������-�м侲̬ͼ��������
        if(index == 0)
        {            
            Gui_PageClearBackground(icon_16_findphone2_center_coord[0][0] + ICON_FINDPHONE2_ICON9_WIDE, 
                                    icon_16_findphone2_center_coord[0][1], 
                                    MAX_X_SIZE - (icon_16_findphone2_center_coord[0][0] + ICON_FINDPHONE2_ICON9_WIDE),
                                    ICON_FINDPHONE2_ICON9_HIGH);
        }
        
        // 2������-��ͼ��ʾ
        {
            LCD_SetPosition(icon_16_findphone2_left_coord[0][0], icon_16_findphone2_left_coord[0][1], 
                            ICON_FINDPHONE2_ICON1_WIDE, ICON_FINDPHONE2_ICON1_HIGH);
            LCD_DriverWriteDataFromExtFlash(icon_16_findphone2_left_addr[index % 4], ICON_FINDPHONE2_ICON1_SIZE);
        }
        
        // 2������-�Ҷ�ͼ��ʾ
        {
            LCD_SetPosition(icon_16_findphone2_right_coord[0][0], icon_16_findphone2_right_coord[0][1], 
                            ICON_FINDPHONE2_ICON5_WIDE, ICON_FINDPHONE2_ICON5_HIGH);
            LCD_DriverWriteDataFromExtFlash(icon_16_findphone2_right_addr[index % 4], ICON_FINDPHONE2_ICON5_SIZE);
        }
        
        // 2������-�м侲̬ͼ���²�����
        if(index == 0)
        {
            Gui_PageClearBackground(0, 
                                    icon_16_findphone2_center_coord[0][1] + ICON_FINDPHONE2_ICON9_HIGH, 
                                    MAX_X_SIZE,
                                    MAX_Y_SIZE - (icon_16_findphone2_center_coord[0][1] + ICON_FINDPHONE2_ICON9_HIGH));
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
//uint8_t stop_watch_lcd_buf[ICON_STOPWATCH_DATADOT_WIDE*2+1] = {0x00, };
void GUI_StopWatchPage(uint16_t index, uint8_t entry_flag, uint8_t reset_flag, uint8_t run_flag, uint16_t total_second, uint8_t one_hundred_ms,
						uint8_t system_hour, uint8_t system_minute, uint8_t language)
{
    uint8_t minute = 0;
    uint8_t second = 0;
    uint8_t rand_num = 0;
    uint8_t data_tmp[6] = {0,};
	
	if(language)
	{
		language = 1;
	}
	
	if(entry_flag == 0)
	{
		// ���ͼ���ϲ�����
		if(index == 0)
		{
			Gui_PageClearBackground(0, 
									0, 
									MAX_X_SIZE,
									icon_16_stopwatch_entry_coord[0][1]);
		}
		
		// ���ͼ��������
		if(index == 0)
		{
			Gui_PageClearBackground(0, 
									icon_16_stopwatch_entry_coord[0][1], 
									icon_16_stopwatch_entry_coord[0][0],
									ICON_STOPWATCH_ICON4_HIGH);
		}
		
		// ���ͼ����ʾ
		if(index == 0)
		{
			LCD_SetPosition(icon_16_stopwatch_entry_coord[0][0], icon_16_stopwatch_entry_coord[0][1], 
							ICON_STOPWATCH_ICON4_WIDE, ICON_STOPWATCH_ICON4_HIGH);
			LCD_DriverWriteDataFromExtFlash(icon_16_stopwatch_entry_addr[0], ICON_STOPWATCH_ICON4_SIZE); 
		}
		
		// ���ͼ��������
		if(index == 0)
		{
			Gui_PageClearBackground(icon_16_stopwatch_entry_coord[0][0] + ICON_STOPWATCH_ICON4_WIDE, 
									icon_16_stopwatch_entry_coord[0][1], 
									MAX_X_SIZE - (icon_16_stopwatch_entry_coord[0][0] + ICON_STOPWATCH_ICON4_WIDE),
									ICON_STOPWATCH_ICON4_HIGH);
		}
		
		// ��Ļ�ײ�����
		if(index == 0)
		{
			Gui_PageClearBackground(0, 
									icon_16_stopwatch_entry_coord[0][1] + ICON_STOPWATCH_ICON4_HIGH, 
									MAX_X_SIZE,
									MAX_Y_SIZE - (icon_16_stopwatch_entry_coord[0][1] + ICON_STOPWATCH_ICON4_HIGH));
		}
		
		// ������ʾ��
		{
			LCD_SetPosition(icon_16_long_press_icon_coord[0][0], icon_16_long_press_icon_coord[0][1], 
							ICON_FINDPHONE_ICON5_WIDE, ICON_FINDPHONE_ICON5_HIGH);
			LCD_DriverWriteDataFromExtFlash(icon_16_long_press_icon_addr[language % 2], ICON_FINDPHONE_ICON5_SIZE);
		}
		
		// ��ʾ����ʾ
		{
			LCD_SetPosition(icon_16_stopwatch_entrylabel_coord[0][0], icon_16_stopwatch_entrylabel_coord[0][1], 
							ICON_STOPWATCH_ICON5_WIDE, ICON_STOPWATCH_ICON5_HIGH);
			LCD_DriverWriteDataFromExtFlash(icon_16_stopwatch_entrylabel_addr[language % 2], ICON_STOPWATCH_ICON5_SIZE); 
		}
	}
	else
	{
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
		
		if(index == 0)
		{
			animation_cnt = 0;
		}
		else if(run_flag == 1)
		{
			animation_cnt++;
		}
	
		// ���ͼ���ϲ�����
		if(index == 0)
		{
			Gui_PageClearBackground(0, 
									0, 
									MAX_X_SIZE,
									icon_16_stopwatch_icon_coord[0][1]);
		}
		
		// ϵͳʱ��
		if(index % 10 == 0)
		{
			// ����ʱ��--ʱ
			LCD_SetPosition(icon_16_move_time_coord[0][0], icon_16_move_time_coord[0][1],
							ICON_MOVE_DATA1_WIDE, ICON_MOVE_DATA1_HIGH);
			LCD_DriverWriteDataFromExtFlash(icon_16_move_time_addr[(system_hour / 10) % 10], ICON_MOVE_DATA1_SIZE);
			LCD_SetPosition(icon_16_move_time_coord[1][0], icon_16_move_time_coord[1][1],
							ICON_MOVE_DATA1_WIDE, ICON_MOVE_DATA1_HIGH);
			LCD_DriverWriteDataFromExtFlash(icon_16_move_time_addr[system_hour % 10], ICON_MOVE_DATA1_SIZE);
			
			// ����ʱ��--�� 
			LCD_SetPosition(icon_16_move_time_coord[2][0], icon_16_move_time_coord[2][1],
							ICON_MOVE_DATA1_WIDE, ICON_MOVE_DATA1_HIGH);
			LCD_DriverWriteDataFromExtFlash(icon_16_move_time_addr[(system_minute / 10) % 10], ICON_MOVE_DATA1_SIZE);
			LCD_SetPosition(icon_16_move_time_coord[3][0], icon_16_move_time_coord[3][1],
							ICON_MOVE_DATA1_WIDE, ICON_MOVE_DATA1_HIGH);
			LCD_DriverWriteDataFromExtFlash(icon_16_move_time_addr[system_minute % 10], ICON_MOVE_DATA1_SIZE); 
			
			// ����ʱ��--ð��
			LCD_SetPosition(icon_16_move_time_dot_coord[0][0], icon_16_move_time_dot_coord[0][1],
							ICON_MOVE_DATA1DOT_WIDE, ICON_MOVE_DATA1DOT_HIGH);
			LCD_DriverWriteDataFromExtFlash(icon_16_move_time_dot_addr[0], ICON_MOVE_DATA1DOT_SIZE);
		}
		
		// ͼ����ʾ
		{
			LCD_SetPosition(icon_16_stopwatch_icon_coord[0][0], icon_16_stopwatch_icon_coord[0][1], 
							ICON_STOPWATCH_ICON1_WIDE, ICON_STOPWATCH_ICON1_HIGH);
			LCD_DriverWriteDataFromExtFlash(icon_16_stopwatch_icon_addr[animation_cnt % 11], ICON_STOPWATCH_ICON1_SIZE);    
			
		}
		
		// ���ͼ����������
		if(index == 0)
		{
			Gui_PageClearBackground(0, 
									icon_16_stopwatch_icon_coord[0][1], 
									icon_16_stopwatch_icon_coord[0][0],
									ICON_STOPWATCH_ICON1_HIGH);
			
			Gui_PageClearBackground(icon_16_stopwatch_icon_coord[0][0] + ICON_STOPWATCH_ICON1_WIDE, 
									icon_16_stopwatch_icon_coord[0][1], 
									MAX_X_SIZE - (icon_16_stopwatch_icon_coord[0][0] + ICON_STOPWATCH_ICON1_WIDE),
									ICON_STOPWATCH_ICON1_HIGH);
			
			Gui_PageClearBackground(0, 
									icon_16_stopwatch_icon_coord[0][1] + ICON_STOPWATCH_ICON1_HIGH, 
									MAX_X_SIZE,
									MAX_Y_SIZE - (icon_16_stopwatch_icon_coord[0][1] + ICON_STOPWATCH_ICON1_HIGH));
		}
		
		// ʱ����ʾ
		{
			for(uint32_t i = 0; i < 2; i++)
			{
				LCD_SetPosition(icon_16_stopwatch_data1_coord[i][0], icon_16_stopwatch_data1_coord[i][1], 
								ICON_STOPWATCH_DATA_WIDE, ICON_STOPWATCH_DATA_HIGH);
				LCD_DriverWriteDataFromExtFlash(icon_16_stopwatch_data1_addr[data_tmp[0 + i]], ICON_STOPWATCH_DATA_SIZE);
			}

			for(uint32_t i = 0; i < 2; i++)
			{
				LCD_SetPosition(icon_16_stopwatch_data2_coord[i][0], icon_16_stopwatch_data2_coord[i][1], 
								ICON_STOPWATCH_DATA_WIDE, ICON_STOPWATCH_DATA_HIGH);
				LCD_DriverWriteDataFromExtFlash(icon_16_stopwatch_data2_addr[data_tmp[2 + i]], ICON_STOPWATCH_DATA_SIZE);
			}
			for(uint32_t i = 0; i < 2; i++)
			{
				LCD_SetPosition(icon_16_stopwatch_data3_coord[i][0], icon_16_stopwatch_data3_coord[i][1], 
								ICON_STOPWATCH_DATA_WIDE, ICON_STOPWATCH_DATA_HIGH);
				LCD_DriverWriteDataFromExtFlash(icon_16_stopwatch_data3_addr[data_tmp[4 + i]], ICON_STOPWATCH_DATA_SIZE);
			}
			
			// ʱ��ð��
//			for(uint32_t i = 0; i < 2; i++)
//			{
				LCD_SetPosition(icon_16_stopwatch_datadot_coord[0][0], icon_16_stopwatch_datadot_coord[0][1], 
								ICON_STOPWATCH_DATADOT_WIDE, ICON_STOPWATCH_DATADOT_HIGH);
				LCD_DriverWriteDataFromExtFlash(icon_16_stopwatch_datadot_addr[0], ICON_STOPWATCH_DATADOT_SIZE);
//			}
            
            LCD_SetPosition(icon_16_stopwatch_datadot_coord[1][0], icon_16_stopwatch_datadot_coord[1][1]+20, 
                            ICON_STOPWATCH_DATADOT_WIDE, ICON_STOPWATCH_DATADOT_HIGH-20);
            LCD_DriverWriteDataFromExtFlash(icon_16_stopwatch_datadot_addr[0]+ICON_STOPWATCH_DATADOT_WIDE*2*20, 
                                            ICON_STOPWATCH_DATADOT_WIDE*(ICON_STOPWATCH_DATADOT_HIGH-20)*2);
//            for(uint8_t i = 0; i < ICON_STOPWATCH_DATADOT_HIGH; i++)
//            {
//                uint8_t stop_watch_lcd_buf[ICON_STOPWATCH_DATADOT_WIDE*2+1] = {0x00, };
//                memcpy(stop_watch_lcd_buf, &stop_watch_dot[ICON_STOPWATCH_DATADOT_WIDE*2*i], ICON_STOPWATCH_DATADOT_WIDE*2);
//                LCD_DriverWriteDataBuf(stop_watch_lcd_buf, ICON_STOPWATCH_DATADOT_WIDE*2);
//            }
//				LCD_DriverWriteDataFromExtFlash(icon_16_stopwatch_datadot_addr[0], ICON_STOPWATCH_DATADOT_SIZE);
		}
		
		
		// ��ͣ����ͼ��
		if(run_flag)
		{
			LCD_SetPosition(icon_16_stopwatch_pause_coord[0][0], icon_16_stopwatch_pause_coord[0][1], 
							ICON_STOPWATCH_ICON3_WIDE, ICON_STOPWATCH_ICON3_HIGH);
			LCD_DriverWriteDataFromExtFlash(icon_16_stopwatch_pause_addr[1], ICON_STOPWATCH_ICON3_SIZE);

		}
		else
		{
			LCD_SetPosition(icon_16_stopwatch_pause_coord[0][0], icon_16_stopwatch_pause_coord[0][1], 
							ICON_STOPWATCH_ICON3_WIDE, ICON_STOPWATCH_ICON3_HIGH);
			LCD_DriverWriteDataFromExtFlash(icon_16_stopwatch_pause_addr[0], ICON_STOPWATCH_ICON3_SIZE);
		}    
	}
}

/****************************************************************
** Function name:           Gui_PageCountDown
** Descriptions:            ����ʱ����
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2018-05-25
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Gui_PageCountDown(uint16_t index, bool dynamic, uint8_t run_state, uint32_t total_second, uint8_t language)
{
	if(language)
	{
		language = 1;
	}
	
	if(dynamic == 0)
	{
		// ��ͼ��������
		if(index == 0)
		{
			Gui_PageClearBackground(0, 
									0, 
									MAX_X_SIZE,
									icon_16_countdown_entry_coord[0][1]);
			
			Gui_PageClearBackground(0, 
									icon_16_countdown_entry_coord[0][1], 
									icon_16_countdown_entry_coord[0][0],
									ICON_COUNTDOWN_ICON4_HIGH);
		}
		
		// ��ͼ��ʾ
		if(index == 0)
		{
			LCD_SetPosition(icon_16_countdown_entry_coord[0][0], icon_16_countdown_entry_coord[0][1], 
							ICON_COUNTDOWN_ICON4_WIDE, ICON_COUNTDOWN_ICON4_HIGH);
			LCD_DriverWriteDataFromExtFlash(icon_16_countdown_entry_addr[0], ICON_COUNTDOWN_ICON4_SIZE);
		}
		
		// ��ͼ�Ҳ�����
		if(index == 0)
		{
			Gui_PageClearBackground(icon_16_countdown_entry_coord[0][0] + ICON_COUNTDOWN_ICON4_WIDE, 
									icon_16_countdown_entry_coord[0][1], 
									MAX_X_SIZE - (icon_16_countdown_entry_coord[0][0] + ICON_COUNTDOWN_ICON4_WIDE),
									ICON_COUNTDOWN_ICON4_HIGH);
		}
		
		// ��Ļ�ײ�����
		if(index == 0)
		{
			Gui_PageClearBackground(0, 
									icon_16_countdown_entry_coord[0][1] + ICON_COUNTDOWN_ICON4_HIGH, 
									MAX_X_SIZE,
									MAX_Y_SIZE - (icon_16_countdown_entry_coord[0][1] + ICON_COUNTDOWN_ICON4_HIGH));
		}
		
		// ������ʾ��
		{
			LCD_SetPosition(icon_16_long_press_icon_coord[0][0], icon_16_long_press_icon_coord[0][1], 
							ICON_FINDPHONE_ICON5_WIDE, ICON_FINDPHONE_ICON5_HIGH);
			LCD_DriverWriteDataFromExtFlash(icon_16_long_press_icon_addr[language % 2], ICON_FINDPHONE_ICON5_SIZE);
		}
		
		// ��ʾ��
		{
			LCD_SetPosition(icon_16_countdown_entrylabel_coord[0][0], icon_16_countdown_entrylabel_coord[0][1], 
							ICON_COUNTDOWN_ICON5_WIDE, ICON_COUNTDOWN_ICON5_HIGH);
			LCD_DriverWriteDataFromExtFlash(icon_16_countdown_entrylabel_addr[language % 2], ICON_COUNTDOWN_ICON5_SIZE);
		}
	}
	else
	{
		if(index == 0)
		{
			animation_cnt = 0;
		}
		else if(run_state != 0)
		{
			animation_cnt++;
		}
		
		// ��ͼ�ϲ�����
		if(index == 0)
		{
			Gui_PageClearBackground(0, 
									0, 
									MAX_X_SIZE,
									icon_16_countdown_dynamic_coord[0][1]);
		}
		
		// ��ͼ������
		if(index == 0)
		{
			Gui_PageClearBackground(0, 
									icon_16_countdown_dynamic_coord[0][1], 
									icon_16_countdown_dynamic_coord[0][0],
									ICON_COUNTDOWN_ICON1_HIGH);
		}
		
		// ��ͼ
		{
			LCD_SetPosition(icon_16_countdown_dynamic_coord[0][0], icon_16_countdown_dynamic_coord[0][1], 
								ICON_COUNTDOWN_ICON1_WIDE, ICON_COUNTDOWN_ICON1_HIGH);
			LCD_DriverWriteDataFromExtFlash(icon_16_countdown_dynamic_addr[animation_cnt % 9], ICON_COUNTDOWN_ICON1_SIZE);
		}
		
		// ��ͼ��������
		if(index == 0)
		{		
			Gui_PageClearBackground(icon_16_countdown_dynamic_coord[0][0] + ICON_COUNTDOWN_ICON1_WIDE, 
									icon_16_countdown_dynamic_coord[0][1], 
									MAX_X_SIZE - (icon_16_countdown_dynamic_coord[0][0] + ICON_COUNTDOWN_ICON1_WIDE),
									ICON_COUNTDOWN_ICON1_HIGH);
		}
		
		// ʱ���ϲ�-����
		if(index == 0)
		{
			Gui_PageClearBackground(0, 
									icon_16_countdown_dynamic_coord[0][1] + ICON_COUNTDOWN_ICON1_HIGH, 
									MAX_X_SIZE,
									icon_16_countdown_data1_coord[0][1]- (icon_16_countdown_dynamic_coord[0][1] + ICON_COUNTDOWN_ICON1_HIGH));
		}
		
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
			
			// ����������
			if(index == 0)
			{
				Gui_PageClearBackground(0, 
										icon_16_countdown_data1_coord[0][1], 
										icon_16_countdown_data1_coord[0][0],
										ICON_COUNTDOWN_DATA_HIGH);
			}
			
			// ������ʾ
			for(uint32_t i = 0; i < 2; i++)
			{
				LCD_SetPosition(icon_16_countdown_data1_coord[i][0], icon_16_countdown_data1_coord[i][1], 
								ICON_COUNTDOWN_DATA_WIDE, ICON_COUNTDOWN_DATA_HIGH);
				LCD_DriverWriteDataFromExtFlash(icon_16_countdown_data1_addr[data_tmp[0 + i]], ICON_COUNTDOWN_DATA_SIZE);
			}
			
			// ���Ӽ������
			if(index == 0)
			{
				Gui_PageClearBackground(icon_16_countdown_data1_coord[0][0] + ICON_COUNTDOWN_DATA_WIDE, 
										icon_16_countdown_data1_coord[0][1], 
										icon_16_countdown_data1_coord[1][0] - (icon_16_countdown_data1_coord[0][0] + ICON_COUNTDOWN_DATA_WIDE),
										ICON_COUNTDOWN_DATA_HIGH);
				
				Gui_PageClearBackground(icon_16_countdown_data1_coord[1][0] + ICON_COUNTDOWN_DATA_WIDE, 
										icon_16_countdown_data1_coord[0][1], 
										icon_16_countdown_datadot_coord[0][0] - (icon_16_countdown_data1_coord[1][0] + ICON_COUNTDOWN_DATA_WIDE),
										ICON_COUNTDOWN_DATA_HIGH);
			}
			
			// ����ð��
			if(index == 0)
			{
				LCD_SetPosition(icon_16_countdown_datadot_coord[0][0], icon_16_countdown_data2_coord[0][1], 
								ICON_COUNTDOWN_DATADOT_WIDE, ICON_COUNTDOWN_DATADOT_HIGH);
				LCD_DriverWriteDataFromExtFlash(icon_16_countdown_datadot_addr[0], ICON_COUNTDOWN_DATADOT_SIZE);
			}
			
			// ����ð���Ҳ�����
			if(index == 0)
			{
				Gui_PageClearBackground(icon_16_countdown_datadot_coord[0][0] + ICON_COUNTDOWN_DATADOT_WIDE, 
										icon_16_countdown_data1_coord[0][1], 
										icon_16_countdown_data2_coord[0][0] - (icon_16_countdown_datadot_coord[0][0] + ICON_COUNTDOWN_DATADOT_WIDE),
										ICON_COUNTDOWN_DATA_HIGH);
			}

			// ����ʾ
			for(uint32_t i = 0; i < 2; i++)
			{
				LCD_SetPosition(icon_16_countdown_data2_coord[i][0], icon_16_countdown_data2_coord[i][1], 
								ICON_COUNTDOWN_DATA_WIDE, ICON_COUNTDOWN_DATA_HIGH);
				LCD_DriverWriteDataFromExtFlash(icon_16_countdown_data2_addr[data_tmp[2 + i]], ICON_COUNTDOWN_DATA_SIZE);
			}
			
			// ��������
			if(index == 0)
			{
				Gui_PageClearBackground(icon_16_countdown_data2_coord[0][0] + ICON_COUNTDOWN_DATA_WIDE, 
										icon_16_countdown_data1_coord[0][1], 
										icon_16_countdown_data2_coord[1][0] - (icon_16_countdown_data2_coord[0][0] + ICON_COUNTDOWN_DATA_WIDE),
										ICON_COUNTDOWN_DATA_HIGH);
				
				Gui_PageClearBackground(icon_16_countdown_data2_coord[1][0] + ICON_COUNTDOWN_DATA_WIDE, 
										icon_16_countdown_data1_coord[0][1], 
										icon_16_countdown_datadot_coord[1][0] - (icon_16_countdown_data2_coord[1][0] + ICON_COUNTDOWN_DATA_WIDE),
										ICON_COUNTDOWN_DATA_HIGH);
			}
			
			// ð��
			if(index == 0)
			{
				LCD_SetPosition(icon_16_countdown_datadot_coord[1][0], icon_16_countdown_data2_coord[1][1], 
								ICON_COUNTDOWN_DATADOT_WIDE, ICON_COUNTDOWN_DATADOT_HIGH);
				LCD_DriverWriteDataFromExtFlash(icon_16_countdown_datadot_addr[0], ICON_COUNTDOWN_DATADOT_SIZE);
			}

			// ��ð���Ҳ�����
			if(index == 0)
			{
				Gui_PageClearBackground(icon_16_countdown_datadot_coord[1][0] + ICON_COUNTDOWN_DATADOT_WIDE, 
										icon_16_countdown_data1_coord[0][1], 
										icon_16_countdown_data3_coord[0][0] - (icon_16_countdown_datadot_coord[1][0] + ICON_COUNTDOWN_DATADOT_WIDE),
										ICON_COUNTDOWN_DATA_HIGH);
			}
			
			// ������ʾ
			for(uint32_t i = 0; i < 2; i++)
			{
				LCD_SetPosition(icon_16_countdown_data3_coord[i][0], icon_16_countdown_data3_coord[i][1], 
								ICON_COUNTDOWN_DATA_WIDE, ICON_COUNTDOWN_DATA_HIGH);
				LCD_DriverWriteDataFromExtFlash(icon_16_countdown_data3_addr[data_tmp[4 + i]], ICON_COUNTDOWN_DATA_SIZE);
			}
			
			// ����������
			if(index == 0)
			{
				Gui_PageClearBackground(icon_16_countdown_data3_coord[0][0] + ICON_COUNTDOWN_DATA_WIDE, 
										icon_16_countdown_data1_coord[0][1], 
										icon_16_countdown_data3_coord[1][0] - (icon_16_countdown_data3_coord[0][0] + ICON_COUNTDOWN_DATA_WIDE),
										ICON_COUNTDOWN_DATA_HIGH);
				
				Gui_PageClearBackground(icon_16_countdown_data3_coord[1][0] + ICON_COUNTDOWN_DATA_WIDE, 
										icon_16_countdown_data1_coord[0][1], 
										MAX_X_SIZE - (icon_16_countdown_data3_coord[1][0] + ICON_COUNTDOWN_DATA_WIDE),
										ICON_COUNTDOWN_DATA_HIGH);
			}
		}

		// ��ͣ����-����-����
		if(index == 0)
		{
			Gui_PageClearBackground(0, 
									icon_16_countdown_data1_coord[0][1] + ICON_COUNTDOWN_DATA_HIGH, 
									MAX_X_SIZE,
									icon_16_countdown_start_icon_coord[0][1] - (icon_16_countdown_data1_coord[0][1] + ICON_COUNTDOWN_DATA_HIGH));
			
			Gui_PageClearBackground(0, 
									icon_16_countdown_start_icon_coord[0][1], 
									icon_16_countdown_start_icon_coord[0][0],
									ICON_COUNTDOWN_ICON3_HIGH);
		}
		
		// ��ͣ����-ͼ��
		{
			LCD_SetPosition(icon_16_countdown_start_icon_coord[0][0], icon_16_countdown_start_icon_coord[0][1], 
							ICON_COUNTDOWN_ICON3_WIDE, ICON_COUNTDOWN_ICON3_HIGH);
			LCD_DriverWriteDataFromExtFlash(icon_16_countdown_start_icon_addr[run_state % 2], ICON_COUNTDOWN_ICON3_SIZE);
		}
		
		// ��ͣ����-���²�����
		if(index == 0)
		{
			Gui_PageClearBackground(icon_16_countdown_start_icon_coord[0][0] + ICON_COUNTDOWN_ICON3_WIDE, 
									icon_16_countdown_start_icon_coord[0][1], 
									MAX_X_SIZE - (icon_16_countdown_start_icon_coord[0][0] + ICON_COUNTDOWN_ICON3_WIDE),
									ICON_COUNTDOWN_ICON3_HIGH);
			
			Gui_PageClearBackground(0, 
									icon_16_countdown_start_icon_coord[0][1] + ICON_COUNTDOWN_ICON3_HIGH, 
									MAX_X_SIZE,
									MAX_Y_SIZE - (icon_16_countdown_start_icon_coord[0][1] + ICON_COUNTDOWN_ICON3_HIGH));
		}
	}
}

/****************************************************************
** Function name:           Gui_PageCameraReady
** Descriptions:            ���׼������
** input parameters:        index����̬ͼ���� 
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2018-05-25
**----------------------------------------------------------------
*****************************************************************/ 
void Gui_PageCameraReady(uint16_t index)
{
    // ͼ���ϲ�����
    if(index == 0)
    {
        Gui_PageClearBackground(0, 
                                0, 
                                MAX_X_SIZE,
                                icon_16_camera_icon1_coord[0][1]);
    }
    
    // ͼ��������
    if(index == 0)
    {
        Gui_PageClearBackground(0, 
                                icon_16_camera_icon1_coord[0][1], 
                                icon_16_camera_icon1_coord[0][0],
                                ICON_CAM_ICON4_HIGH);
    }
    
    // ��ͼ��ʾ-��ͼ��
    if(index == 0)
    {
        LCD_SetPosition(icon_16_camera_icon1_coord[0][0], icon_16_camera_icon1_coord[0][1], 
                        ICON_CAM_ICON4_WIDE, ICON_CAM_ICON4_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_camera_icon1_addr[0], ICON_CAM_ICON4_SIZE); 
    }
    
    // ͼ��������
    if(index == 0)
    {
        Gui_PageClearBackground(icon_16_camera_icon1_coord[0][0] + ICON_CAM_ICON4_WIDE, 
                                icon_16_camera_icon1_coord[0][1], 
                                MAX_X_SIZE - (icon_16_camera_icon1_coord[0][0] + ICON_CAM_ICON4_WIDE),
                                ICON_CAM_ICON4_HIGH);
    }
    
    // ��ͼ�²�����
    if(index == 0)
    {
        Gui_PageClearBackground(0, 
                                icon_16_camera_icon1_coord[0][1] + ICON_CAM_ICON4_HIGH, 
                                MAX_X_SIZE,
                                icon_16_camera_icon2_coord[0][1] - (icon_16_camera_icon1_coord[0][1] + ICON_CAM_ICON4_HIGH));
    }
    
    // ��ͼ������
    if(index == 0)
    {
        Gui_PageClearBackground(0, 
                                icon_16_camera_icon2_coord[0][1], 
                                icon_16_camera_icon2_coord[0][0],
                                ICON_CAM_ICON5_HIGH);
    }
    
    // ��ͼ��ʾ-Сͼ��
    if(index == 0)
    {
        LCD_SetPosition(icon_16_camera_icon2_coord[0][0], icon_16_camera_icon2_coord[0][1], 
                        ICON_CAM_ICON5_WIDE, ICON_CAM_ICON5_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_camera_icon2_addr[0], ICON_CAM_ICON5_SIZE); 
    }
    
    // ͼ��������
    if(index == 0)
    {
        Gui_PageClearBackground(icon_16_camera_icon2_coord[0][0] + ICON_CAM_ICON5_WIDE, 
                                icon_16_camera_icon2_coord[0][1], 
                                MAX_X_SIZE - (icon_16_camera_icon2_coord[0][0] + ICON_CAM_ICON5_WIDE),
                                ICON_CAM_ICON5_HIGH);
    }
    
    // ��Ļ�ײ�����
    if(index == 0)
    {
        Gui_PageClearBackground(0, 
                                icon_16_camera_icon2_coord[0][1] + ICON_CAM_ICON5_HIGH, 
                                MAX_X_SIZE,
                                MAX_Y_SIZE - (icon_16_camera_icon2_coord[0][1] + ICON_CAM_ICON5_HIGH));
    }
}

/****************************************************************
** Function name:           Gui_PageCameraOk
** Descriptions:            ���OK����
** input parameters:        index����̬ͼ���� 
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2018-05-25
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Gui_PageCameraOk(uint16_t index)
{
    // ͼ���ϲ�����
    if(index == 0)
    {
        Gui_PageClearBackground(0, 
                                0, 
                                MAX_X_SIZE,
                                icon_16_camera_icon1_coord[0][1]);
    }
    
    // ͼ��������
    if(index == 0)
    {
        Gui_PageClearBackground(0, 
                                icon_16_camera_icon1_coord[0][1], 
                                icon_16_camera_icon1_coord[0][0],
                                ICON_CAM_ICON4_HIGH);
    }
    
    // ��ͼ��ʾ-��ͼ��
    {
        LCD_SetPosition(icon_16_camera_icon1_coord[0][0], icon_16_camera_icon1_coord[0][1], 
                        ICON_CAM_ICON4_WIDE, ICON_CAM_ICON4_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_camera_icon1_addr[index % 8], ICON_CAM_ICON4_SIZE); 
    }
    
    // ͼ��������
    if(index == 0)
    {
        Gui_PageClearBackground(icon_16_camera_icon1_coord[0][0] + ICON_CAM_ICON4_WIDE, 
                                icon_16_camera_icon1_coord[0][1], 
                                MAX_X_SIZE - (icon_16_camera_icon1_coord[0][0] + ICON_CAM_ICON4_WIDE),
                                ICON_CAM_ICON4_HIGH);
    }
    
    // ��ͼ�²�����
    if(index == 0)
    {
        Gui_PageClearBackground(0, 
                                icon_16_camera_icon1_coord[0][1] + ICON_CAM_ICON4_HIGH, 
                                MAX_X_SIZE,
                                icon_16_camera_icon2_coord[0][1] - (icon_16_camera_icon1_coord[0][1] + ICON_CAM_ICON4_HIGH));
    }
    
    // ��ͼ������
    if(index == 0)
    {
        Gui_PageClearBackground(0, 
                                icon_16_camera_icon2_coord[0][1], 
                                icon_16_camera_icon2_coord[0][0],
                                ICON_CAM_ICON5_HIGH);
    }
    
    // ��ͼ��ʾ-Сͼ��
    {
        LCD_SetPosition(icon_16_camera_icon2_coord[0][0], icon_16_camera_icon2_coord[0][1], 
                        ICON_CAM_ICON5_WIDE, ICON_CAM_ICON5_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_camera_icon2_addr[index % 2], ICON_CAM_ICON5_SIZE); 
    }
    
    // ͼ��������
    if(index == 0)
    {
        Gui_PageClearBackground(icon_16_camera_icon2_coord[0][0] + ICON_CAM_ICON5_WIDE, 
                                icon_16_camera_icon2_coord[0][1], 
                                MAX_X_SIZE - (icon_16_camera_icon2_coord[0][0] + ICON_CAM_ICON5_WIDE),
                                ICON_CAM_ICON5_HIGH);
    }
    
    // ��Ļ�ײ�����
    if(index == 0)
    {
        Gui_PageClearBackground(0, 
                                icon_16_camera_icon2_coord[0][1] + ICON_CAM_ICON5_HIGH, 
                                MAX_X_SIZE,
                                MAX_Y_SIZE - (icon_16_camera_icon2_coord[0][1] + ICON_CAM_ICON5_HIGH));
    }
}

/****************************************************************
** Function name:           Gui_PageDisMacVer
** Descriptions:            ��ʾMac�Ͱ汾��
** input parameters:        index����̬ͼ���� 
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2018-05-25
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Gui_PageDisMacVer(uint16_t index, uint8_t *mac_data, uint8_t *ver_data)
{
    uint8_t buf_index = 0;
    uint8_t temp_data = 0;
    uint8_t dis_mac_data_buf[10] = {0};
    uint8_t dis_ver_data_buf[10] = {0};
    
    if(index == 0)
    {
        Gui_PageClearBackground(0, 
                                0, 
                                MAX_X_SIZE,
                                MAX_Y_SIZE);
    }
        
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
        dis_mac_data_buf[buf_index++] = ':';
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
        //Font_DisContentText(dis_mac_data_buf, 7, 86, 88, 1, 2, 0);     
        Font_DisUiTextOneLine2X(0,dis_mac_data_buf, buf_index, 120, 80, 1, 0, 0, 0x0000);
        
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
        //Font_DisContentText(dis_ver_data_buf, 7, 86, 136, 1, 2, 0);  
        Font_DisUiTextOneLine2X(0,dis_ver_data_buf, buf_index, 120, 128, 1, 0, 0, 0x0000);
    }
}

/****************************************************************
** Function name:           Gui_PageAlarm
** Descriptions:            ���ӽ���
** input parameters:        am_pm_format��am��pm״̬     scene�����ӵ����� 
**                          hour�����ӵ�ʱ   minute�����ӵķ�
**                          language�����ӵ�λ������  index����̬ͼ����
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2018-05-25
**----------------------------------------------------------------
*****************************************************************/ 
void Gui_PageAlarm(uint16_t index, uint8_t am_pm_format, uint8_t scene, uint8_t hour, uint8_t minute, uint8_t language)
{
    if(scene == 0)
    {
        // ͼ���ϲ�����
        if(index == 0)
        {
            Gui_PageClearBackground(0, 
                                    0, 
                                    MAX_X_SIZE,
                                    icon_16_alarm_icon_coord[0][1]);
        }
        
        // ��ͼ��ʾ
        {
            LCD_SetPosition(icon_16_alarm_icon_coord[0][0], icon_16_alarm_icon_coord[0][1], 
                            ICON_CLOCK_ICON_WIDE, ICON_CLOCK_ICON_HIGH);
            LCD_DriverWriteDataFromExtFlash(icon_16_alarm_icon_addr[index % 4], ICON_CLOCK_ICON_SIZE); 
        }
        
        // ͼ����������
        if(index == 0)
        {
            Gui_PageClearBackground(0, 
                                    icon_16_alarm_icon_coord[0][1], 
                                    icon_16_alarm_icon_coord[0][0],
                                    ICON_CLOCK_ICON_HIGH);
            
            Gui_PageClearBackground(icon_16_alarm_icon_coord[0][0] + ICON_CLOCK_ICON_WIDE, 
                                    icon_16_alarm_icon_coord[0][1], 
                                    MAX_X_SIZE - (icon_16_alarm_icon_coord[0][0] + ICON_CLOCK_ICON_WIDE),
                                    ICON_CLOCK_ICON_HIGH);
        }
        
        // ʱ�䱳��-����
        if(index == 0)
        {
            Gui_PageClearBackground(0, 
                                    icon_16_alarm_icon_coord[0][1] + ICON_CLOCK_ICON_HIGH, 
                                    MAX_X_SIZE,
                                    (icon_16_alarm_time_coord[0][1] + ICON_CLOCK_DATA_HIGH) -
                                    (icon_16_alarm_icon_coord[0][1] + ICON_CLOCK_ICON_HIGH));
        }
        
        // ʱ����ʾ
        if(index == 0)
        {
            // ����ʱ��--ʱ
            LCD_SetPosition(icon_16_alarm_time_coord[0][0], icon_16_alarm_time_coord[0][1],
                            ICON_CLOCK_DATA_WIDE, ICON_CLOCK_DATA_HIGH);
            LCD_DriverWriteDataFromExtFlash(icon_16_alarm_time_addr[(hour / 10) % 10], ICON_CLOCK_DATA_SIZE);
            LCD_SetPosition(icon_16_alarm_time_coord[1][0], icon_16_alarm_time_coord[1][1],
                            ICON_CLOCK_DATA_WIDE, ICON_CLOCK_DATA_HIGH);
            LCD_DriverWriteDataFromExtFlash(icon_16_alarm_time_addr[hour % 10], ICON_CLOCK_DATA_SIZE);
            
            // ����ʱ��--�� 
            LCD_SetPosition(icon_16_alarm_time_coord[2][0], icon_16_alarm_time_coord[2][1],
                            ICON_CLOCK_DATA_WIDE, ICON_CLOCK_DATA_HIGH);
            LCD_DriverWriteDataFromExtFlash(icon_16_alarm_time_addr[(minute / 10) % 10], ICON_CLOCK_DATA_SIZE);
            LCD_SetPosition(icon_16_alarm_time_coord[3][0], icon_16_alarm_time_coord[3][1],
                            ICON_CLOCK_DATA_WIDE, ICON_CLOCK_DATA_HIGH);
            LCD_DriverWriteDataFromExtFlash(icon_16_alarm_time_addr[minute % 10], ICON_CLOCK_DATA_SIZE); 
            
            // ����ʱ��--ð��
            LCD_SetPosition(icon_16_alarm_timedot_coord[0][0], icon_16_alarm_timedot_coord[0][1],
                            ICON_CLOCK_DATADOT_WIDE, ICON_CLOCK_DATADOT_HIGH);
            LCD_DriverWriteDataFromExtFlash(icon_16_alarm_timedot_addr[0], ICON_CLOCK_DATADOT_SIZE);
        }
        
        // �����米��-����
        if(index == 0)
        {
            Gui_PageClearBackground(0, 
                                    icon_16_alarm_time_coord[0][1] + ICON_CLOCK_DATA_HIGH, 
                                    MAX_X_SIZE,
                                    (icon_16_alarm_ampm_coord[0][1] + ICON_CLOCK_UNIT_HIGH) - 
                                    (icon_16_alarm_time_coord[0][1] + ICON_CLOCK_DATA_HIGH));
        }
        
        // ��ʾ������
        if(index == 0)
        {
            LCD_SetPosition(icon_16_alarm_ampm_coord[0][0], icon_16_alarm_ampm_coord[0][1], 
                            ICON_CLOCK_UNIT_WIDE, ICON_CLOCK_UNIT_HIGH);
            if(am_pm_format == GUI_TIME_FORMAT_AM)
            {
                if(language == 0)
                {
                    LCD_DriverWriteDataFromExtFlash(icon_16_alarm_ampm_addr[0], ICON_CLOCK_UNIT_SIZE);
                }
                else
                {
                    LCD_DriverWriteDataFromExtFlash(icon_16_alarm_ampm_addr[2], ICON_CLOCK_UNIT_SIZE);
                }
            }
            else if(am_pm_format == GUI_TIME_FORMAT_PM)
            {
                if(language == 0)
                {
                    LCD_DriverWriteDataFromExtFlash(icon_16_alarm_ampm_addr[1], ICON_CLOCK_UNIT_SIZE);
                }
                else
                {
                    LCD_DriverWriteDataFromExtFlash(icon_16_alarm_ampm_addr[3], ICON_CLOCK_UNIT_SIZE);
                }
            }
            else
            { 
                //��Ҫ�滻
                LCD_DriverWriteDataFromExtFlash(icon_16_alarm_ampm_addr[4], ICON_CLOCK_UNIT_SIZE);
            }
        }
        // ��Ļ�ײ�����
        if(index == 0)
        {
            Gui_PageClearBackground(0, 
                                    icon_16_alarm_ampm_coord[0][1] + ICON_CLOCK_UNIT_HIGH, 
                                    MAX_X_SIZE,
                                    MAX_Y_SIZE - (icon_16_alarm_ampm_coord[0][1] + ICON_CLOCK_UNIT_HIGH));
        }
    }
    else
    {
        // �龰����ͼ��
        
        scene -= 1;
        scene %= 20;
        
        // ͼ���ϲ�����
        if(index == 0)
        {
            Gui_PageClearBackground(0, 
                                    0, 
                                    MAX_X_SIZE,
                                    icon_16_alarm_scene_coord[0][1]);
        }
        
        // ͼ����ʾ
        if(index == 0)
        {
            LCD_SetPosition(icon_16_alarm_scene_coord[0][0], icon_16_alarm_scene_coord[0][1], 
                            ICON_REMINDCLOCK_ICON_WIDE, ICON_REMINDCLOCK_ICON_HIGH);
            LCD_DriverWriteDataFromExtFlash(icon_16_alarm_scene_addr[scene % 20], ICON_REMINDCLOCK_ICON_SIZE); 
        }
        
        // ͼ����������
        if(index == 0)
        {
            Gui_PageClearBackground(0, 
                                    icon_16_alarm_scene_coord[0][1], 
                                    icon_16_alarm_scene_coord[0][0],
                                    ICON_REMINDCLOCK_ICON_HIGH);
            
            Gui_PageClearBackground(icon_16_alarm_scene_coord[0][0] + ICON_REMINDCLOCK_ICON_WIDE, 
                                    icon_16_alarm_scene_coord[0][1], 
                                    MAX_X_SIZE - (icon_16_alarm_scene_coord[0][0] + ICON_REMINDCLOCK_ICON_WIDE),
                                    ICON_REMINDCLOCK_ICON_HIGH);
        }
        
        // ��Ļ�ײ�����
        if(index == 0)
        {
            Gui_PageClearBackground(0, 
                                    icon_16_alarm_scene_coord[0][1] + ICON_REMINDCLOCK_ICON_HIGH, 
                                    MAX_X_SIZE,
                                    MAX_Y_SIZE - (icon_16_alarm_scene_coord[0][1] + ICON_REMINDCLOCK_ICON_HIGH));
        }
    }
}

/****************************************************************
** Function name:           Gui_PageSit
** Descriptions:            ��������
** input parameters:        index����̬ͼ����
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2018-05-25
**----------------------------------------------------------------
*****************************************************************/ 
void Gui_PageSit(uint16_t index, uint8_t language)
{
    if(language)
    {
        language = 1;
    }
    
    // ͼ���ϲ�����
    if(index == 0)
    {
        Gui_PageClearBackground(0, 
                                0, 
                                MAX_X_SIZE,
                                icon_16_sit_icon_coord[0][1]);
    }
    
    // ͼ��������
    if(index == 0)
    {
        Gui_PageClearBackground(0, 
                                icon_16_sit_icon_coord[0][1], 
                                icon_16_sit_icon_coord[0][0],
                                ICON_SIT_ICON1_HIGH);
    }
    
    // ��ͼ��ʾ
    {
        LCD_SetPosition(icon_16_sit_icon_coord[0][0], icon_16_sit_icon_coord[0][1], 
                        ICON_SIT_ICON1_WIDE, ICON_SIT_ICON1_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_sit_icon_addr[index % 13], ICON_SIT_ICON1_SIZE); 
    }

    // ͼ��������
    if(index == 0)
    {
        Gui_PageClearBackground(icon_16_sit_icon_coord[0][0] + ICON_SIT_ICON1_WIDE, 
                                icon_16_sit_icon_coord[0][1], 
                                MAX_X_SIZE - (icon_16_sit_icon_coord[0][0] + ICON_SIT_ICON1_WIDE),
                                ICON_SIT_ICON1_HIGH);
    }
    
    // ��λ��������
    if(index == 0)
    {
        Gui_PageClearBackground(0, 
                                icon_16_sit_icon_coord[0][1] + ICON_SIT_ICON1_HIGH, 
                                MAX_X_SIZE,
                                (icon_16_sit_unit_coord[0][1] + ICON_SIT_ICON2_HIGH) - 
                                (icon_16_sit_icon_coord[0][1] + ICON_SIT_ICON1_HIGH));
    }
    
    // ��λ��ʾ
    {
        LCD_SetPosition(icon_16_sit_unit_coord[0][0], icon_16_sit_unit_coord[0][1], 
                        ICON_SIT_ICON2_WIDE, ICON_SIT_ICON2_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_sit_unit_addr[language % 2], ICON_SIT_ICON2_SIZE); 
    }
    
    // ��Ļ�ײ�����
    if(index == 0)
    {
        Gui_PageClearBackground(0, 
                                icon_16_sit_unit_coord[0][1] + ICON_SIT_ICON2_HIGH, 
                                MAX_X_SIZE,
                                MAX_Y_SIZE - (icon_16_sit_unit_coord[0][1] + ICON_SIT_ICON2_HIGH));
    }
}

/****************************************************************
** Function name:           Gui_PageLowPower
** Descriptions:            �͵����ѽ���
** input parameters:        index����̬ͼ����
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2018-05-25
**----------------------------------------------------------------
*****************************************************************/ 
void Gui_PageLowPower(uint16_t index)
{   
    // ͼ���ϲ�����
    if(index == 0)
    {
        Gui_PageClearBackground(0, 
                                0, 
                                MAX_X_SIZE,
                                icon_16_lowpower_icon_coord[0][1]);
    }
    
    // ��ͼ �� ����
    if(index == 0)
    {
        Gui_PageClearBackground(0, 
                                icon_16_lowpower_icon_coord[0][1], 
                                icon_16_lowpower_icon_coord[0][0],
                                ICON_CHG_ICON5_HIGH);
    }
 
    LCD_SetPosition(icon_16_lowpower_icon_coord[0][0], icon_16_lowpower_icon_coord[0][1], 
                   ICON_CHG_ICON5_WIDE, ICON_CHG_ICON5_HIGH);
    LCD_DriverWriteDataFromExtFlash(icon_16_lowpower_icon_addr[index % 4], ICON_CHG_ICON5_SIZE);
    
    // ���� ���� ����
    if(index == 0)
    {
        Gui_PageClearBackground(icon_16_lowpower_icon_coord[0][0] + ICON_CHG_ICON5_WIDE, 
                                icon_16_lowpower_icon_coord[0][1], 
                                MAX_X_SIZE - (icon_16_lowpower_icon_coord[0][0] + ICON_CHG_ICON5_WIDE),
                                ICON_CHG_ICON5_HIGH);
        
        Gui_PageClearBackground(0, 
                                icon_16_lowpower_icon_coord[0][1] + ICON_CHG_ICON5_HIGH, 
                                MAX_X_SIZE,
                                MAX_Y_SIZE - (icon_16_lowpower_icon_coord[0][1] + ICON_CHG_ICON5_HIGH));
    }
}

/****************************************************************
** Function name:           Gui_PageBatteryCharg
** Descriptions:            ��س�����
** input parameters:        index����̬ͼ����
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2018-05-25
**----------------------------------------------------------------
*****************************************************************/ 
void Gui_PageBatteryCharg(uint16_t index, uint8_t state, uint8_t bat_grade)
{	
    // ��ͼ��ʾ
    if(state == 0)
    {
        // ͼ���ϲ�����
        if(index == 0)
        {
            Gui_PageClearBackground(0, 
                                    0, 
                                    MAX_X_SIZE,
                                    icon_16_charge_bg_coord[0][1]);
        }
        
        if(index == 0)
        {
            LCD_SetPosition(icon_16_charge_bg_coord[0][0], icon_16_charge_bg_coord[0][1], 
                           ICON_CHG_BG1_WIDE, ICON_CHG_BG1_HIGH);
            LCD_DriverWriteDataFromExtFlash(icon_16_charge_bg_addr[0], ICON_CHG_BG1_SIZE);
        }
        
        // ���� ������ ����
        if(index == 0)
        {
            Gui_PageClearBackground(0, 
                                    icon_16_charge_bg_coord[0][1], 
                                    icon_16_charge_bg_coord[0][0],
                                    ICON_CHG_BG1_HIGH);
            
            Gui_PageClearBackground(icon_16_charge_bg_coord[0][0] + ICON_CHG_BG1_WIDE, 
                                    icon_16_charge_bg_coord[0][1], 
                                    MAX_X_SIZE - (icon_16_charge_bg_coord[0][0] + ICON_CHG_BG1_WIDE),
                                    ICON_CHG_BG1_HIGH);
            
            Gui_PageClearBackground(0, 
                                    icon_16_charge_bg_coord[0][1] + ICON_CHG_BG1_HIGH, 
                                    MAX_X_SIZE,
                                    MAX_Y_SIZE - (icon_16_charge_bg_coord[0][1] + ICON_CHG_BG1_HIGH));
        }
        
         // ��صȼ���ʾ
		{		
			if(bat_grade)
			{
				bat_grade -= 1;
			}
			
			LCD_SetPosition(icon_16_charge_icon_coord[0][0], icon_16_charge_icon_coord[0][1], 
						   ICON_CHG_ICON1_WIDE, ICON_CHG_ICON1_HIGH);
			LCD_DriverWriteDataFromExtFlash(icon_16_charge_icon_addr[bat_grade % 4][index % 3], ICON_CHG_ICON1_SIZE);
		}
	}
    else
    {
        // ����-�ϲ�-����
        if(index == 0)
        {
            Gui_PageClearBackground(0, 
                                    0, 
                                    MAX_X_SIZE,
                                    icon_16_charge_bg_coord[0][1]);
        }
        
        if(index == 0)
        {
            LCD_SetPosition(icon_16_charge_bg_coord[0][0], icon_16_charge_bg_coord[0][1], 
                           ICON_CHG_BG1_WIDE, ICON_CHG_BG1_HIGH);
            LCD_DriverWriteDataFromExtFlash(icon_16_charge_bg_addr[0], ICON_CHG_BG1_SIZE);
        }
        
        LCD_SetPosition(icon_16_charge_full_coord[0][0], icon_16_charge_full_coord[0][1], 
                        ICON_CHG_BG2_WIDE, ICON_CHG_BG2_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_charge_full_addr[0], ICON_CHG_BG2_SIZE);
        
        // ����-������-����
        if(index == 0)
        {
            Gui_PageClearBackground(0, 
                                    icon_16_charge_bg_coord[0][1], 
                                    icon_16_charge_bg_coord[0][0],
                                    ICON_CHG_BG1_HIGH);
            
            Gui_PageClearBackground(icon_16_charge_bg_coord[0][0] + ICON_CHG_BG1_WIDE, 
                                    icon_16_charge_bg_coord[0][1], 
                                    MAX_X_SIZE - (icon_16_charge_bg_coord[0][0] + ICON_CHG_BG1_WIDE),
                                    ICON_CHG_BG1_HIGH);
            
            Gui_PageClearBackground(0, 
                                    icon_16_charge_bg_coord[0][1] + ICON_CHG_BG1_HIGH, 
                                    MAX_X_SIZE,
                                    MAX_Y_SIZE - (icon_16_charge_bg_coord[0][1] + ICON_CHG_BG1_HIGH));
        }
    }
}

/****************************************************************
** Function name:           Gui_PageHeartAlert
** Descriptions:            ���ʱ�������
** input parameters:        index����̬ͼ����
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2018-05-25
**----------------------------------------------------------------
*****************************************************************/
void Gui_PageHeartAlert(uint16_t index)
{
    // ͼ���ϲ�����
    if(index == 0)
    {
        Gui_PageClearBackground(0, 
                                0, 
                                MAX_X_SIZE,
                                icon_16_heartalert_bg_coord[0][1]);
    }
    
    // ͼ��������
    if(index == 0)
    {
        Gui_PageClearBackground(0, 
                                icon_16_heartalert_bg_coord[0][1], 
                                icon_16_heartalert_bg_coord[0][0],
                                ICON_HEARTALERT_ICON1_HIGH);
    }
    
    // ������ʾ
    if(index == 0)
    {
        LCD_SetPosition(icon_16_heartalert_bg_coord[0][0], icon_16_heartalert_bg_coord[0][1], 
                       ICON_HEARTALERT_ICON1_WIDE, ICON_HEARTALERT_ICON1_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_heartalert_bg_addr[0], ICON_HEARTALERT_ICON1_SIZE);
    }
    
    // ��ͼ��ʾ
    {
        LCD_SetPosition(icon_16_heartalert_icon_coord[0][0], icon_16_heartalert_icon_coord[0][1], 
                       ICON_HEARTALERT_ICON2_WIDE, ICON_HEARTALERT_ICON2_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_heartalert_icon_addr[index % 8], ICON_HEARTALERT_ICON2_SIZE);
    }
    
    // ͼ��������
    if(index == 0)
    {       
        Gui_PageClearBackground(icon_16_heartalert_bg_coord[0][0] + ICON_HEARTALERT_ICON1_WIDE, 
                                icon_16_heartalert_bg_coord[0][1], 
                                MAX_X_SIZE - (icon_16_heartalert_bg_coord[0][0] + ICON_HEARTALERT_ICON1_WIDE),
                                ICON_HEARTALERT_ICON1_HIGH);
    }
    
    // ��Ļ�ײ�����
    if(index == 0)
    {
        Gui_PageClearBackground(0, 
                                icon_16_heartalert_bg_coord[0][1] + ICON_HEARTALERT_ICON1_HIGH, 
                                MAX_X_SIZE,
                                MAX_Y_SIZE - (icon_16_heartalert_bg_coord[0][1] + ICON_HEARTALERT_ICON1_HIGH));
    }
}

/****************************************************************
** Function name:           Gui_PageFontTest
** Descriptions:            ��ʾ��ά���ǿƼ�����������
** input parameters:        
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2018-05-25
**----------------------------------------------------------------
*****************************************************************/ 
void Gui_PageFontTest(uint16_t index)
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
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2018-05-25
**----------------------------------------------------------------
*****************************************************************/ 
void Gui_PageCallNotificationIcon(uint16_t index, uint8_t call_state, uint8_t language)
{	
    if(language)
    {
        language = 1;
    }
    
    // �����ϲ� ������
    if(index == 0)
    {
        Gui_PageClearBackground(0, 
                                0, 
                                MAX_X_SIZE,
                                icon_16_call_man_coord[0][1]);
		
		Gui_PageClearBackground(0, 
                                icon_16_call_man_coord[0][1], 
                                icon_16_call_man_coord[0][0],
                                ICON_CALL_ICON1_HIGH);
    } 
    
    // ������ʾ
    if(index == 0)
    {
        LCD_SetPosition(icon_16_call_man_coord[0][0], icon_16_call_man_coord[0][1], 
                        ICON_CALL_ICON1_WIDE, ICON_CALL_ICON1_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_call_man_addr[0], ICON_CALL_ICON1_SIZE); 
    }
	
	// �����ϲ� �Ҳ�����
	if(index == 0)
	{
		Gui_PageClearBackground(icon_16_call_man_coord[0][0] + ICON_CALL_ICON1_WIDE, 
                                icon_16_call_man_coord[0][1], 
                                MAX_X_SIZE - (icon_16_call_man_coord[0][0] + ICON_CALL_ICON1_WIDE),
                                ICON_CALL_ICON1_HIGH);
    } 
    
    // ����״̬��ʾ��-��������
    if(index == 0)
    {
		Gui_PageClearBackground(0, 
                                icon_16_call_man_coord[0][1] + ICON_CALL_ICON1_HIGH, 
                                MAX_X_SIZE,
                                icon_16_call_icon_coord[0][1] - (icon_16_call_man_coord[0][1] + ICON_CALL_ICON1_HIGH));
    }
    
	// ����״̬��ʾ��
	if(call_state == GUI_CALL_MUTE)
    {
        LCD_SetPosition(icon_16_call_label_coord[0][0], icon_16_call_label_coord[0][1], 
                        ICON_CALL_ICON4_WIDE, ICON_CALL_ICON4_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_call_label_addr[language % 2][0], ICON_CALL_ICON4_SIZE);
    }
    else if(call_state == GUI_CALL_REJECTSUC)
    {
        LCD_SetPosition(icon_16_call_label_coord[0][0], icon_16_call_label_coord[0][1], 
                        ICON_CALL_ICON4_WIDE, ICON_CALL_ICON4_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_call_label_addr[language % 2][1], ICON_CALL_ICON4_SIZE);
    }
    
    // ����Ҷ�ͼ��-������
    if(index == 0)
    {
		Gui_PageClearBackground(0, 
                                icon_16_call_icon_coord[0][1], 
                                icon_16_call_icon_coord[0][0],
                                ICON_CALL_ICON2_HIGH);
    }
    
    // ����Ҷ�ͼ��
    {
        LCD_SetPosition(icon_16_call_icon_coord[0][0], icon_16_call_icon_coord[0][1], 
                        ICON_CALL_ICON2_WIDE, ICON_CALL_ICON2_HIGH);
        if((call_state == GUI_CALL_INCOMING)||(call_state == GUI_CALL_MUTE))
        {
            
            LCD_DriverWriteDataFromExtFlash(icon_16_call_icon_addr[0][0], ICON_CALL_ICON2_SIZE);
        }
        else if(call_state == GUI_CALL_REJECTSUC)
        {
            LCD_DriverWriteDataFromExtFlash(icon_16_call_icon_addr[0][1], ICON_CALL_ICON2_SIZE);
        }
    }
	
	// ����Ҷ�ͼ��-������
    if(index == 0)
    {
		Gui_PageClearBackground(icon_16_call_icon_coord[0][0] + ICON_CALL_ICON2_WIDE, 
                                icon_16_call_icon_coord[0][1], 
                                icon_16_call_icon_coord[1][0] - (icon_16_call_icon_coord[0][0] + ICON_CALL_ICON2_WIDE),
                                ICON_CALL_ICON2_HIGH);
    }

    // ���羲��ͼ��
    {
        LCD_SetPosition(icon_16_call_icon_coord[1][0], icon_16_call_icon_coord[1][1], 
                        ICON_CALL_ICON3_WIDE, ICON_CALL_ICON3_HIGH);
        if(call_state == GUI_CALL_INCOMING)
        {
            LCD_DriverWriteDataFromExtFlash(icon_16_call_icon_addr[1][0], ICON_CALL_ICON3_SIZE);
        }
        else if(call_state == GUI_CALL_MUTE)
        {
            LCD_DriverWriteDataFromExtFlash(icon_16_call_icon_addr[1][1], ICON_CALL_ICON3_SIZE);
        }
    }
	
	// ���羲��ͼ��-������
    if(index == 0)
    {
		Gui_PageClearBackground(icon_16_call_icon_coord[1][0] + ICON_CALL_ICON3_WIDE, 
                                icon_16_call_icon_coord[1][1], 
                                MAX_X_SIZE - (icon_16_call_icon_coord[1][0] + ICON_CALL_ICON3_WIDE),
                                ICON_CALL_ICON3_HIGH);
    }
    
    // ��Ļ�ײ�����
    if(index == 0)
    {
		Gui_PageClearBackground(0, 
                                icon_16_call_icon_coord[0][1] + ICON_CALL_ICON3_HIGH, 
                                MAX_X_SIZE,
                                MAX_Y_SIZE - (icon_16_call_icon_coord[0][1] + ICON_CALL_ICON3_HIGH));
    }
	
	// �Ҷ���ʾ��
	{
        LCD_SetPosition(icon_16_call_rejectlabel_coord[0][0], icon_16_call_rejectlabel_coord[0][1], 
                        ICON_CALL_ICON5_WIDE, ICON_CALL_ICON5_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_call_rejectlabel_addr[language % 2], ICON_CALL_ICON5_SIZE);
	}
	
	// ������ʾ��
	{
        LCD_SetPosition(icon_16_call_mutelabel_coord[0][0], icon_16_call_mutelabel_coord[0][1], 
                        ICON_CALL_ICON6_WIDE, ICON_CALL_ICON6_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_call_mutelabel_addr[language % 2], ICON_CALL_ICON6_SIZE);
	}
}

/****************************************************************
** Function name:           Gui_PageCallNotificationContent
** Descriptions:            ��ʾ������Ϣ���ͽ���
** input parameters:        display_buf����Ϣ����ָ��
                            buf_len�����ݳ���
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2018-05-25
**----------------------------------------------------------------
*****************************************************************/ 
void Gui_PageCallNotificationContent(uint32_t index, uint8_t *display_buf, uint16_t buf_len)
{
    Font_DisUiTextOneLine2X(index, display_buf, buf_len, 120, 107, 1, 0, 0, 0X00);
}

/****************************************************************
** Function name:           Gui_PageMessageNotificationIcon
** Descriptions:            ��ʾ��Ϣ���ͽ���
** input parameters:        msg_type����Ϣ������
**                          page_index����̬ͼ����
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2018-05-25
**----------------------------------------------------------------
*****************************************************************/ 
void Gui_PageMessageNotificationIcon(uint16_t index, uint8_t msg_type, uint8_t page_index, uint8_t hour, uint8_t minute, uint8_t language)
{     
    if(language)
    {
        language = 1;
    }
    
    // ��Ļ�ϲ�����
    if(index == 0)
    {
        Gui_PageClearBackground(0, 
                                0, 
                                MAX_X_SIZE,
                                11);
    }
    
    // ʱ�䱳������
    if(index == 0)
    {
        Gui_PageClearBackground(icon_16_inform_bg_coord[2][0] + ICON_INFORM_BACKGROUND3_WIDE, 
                                11, 
                                MAX_X_SIZE - (icon_16_inform_bg_coord[2][0] + ICON_INFORM_BACKGROUND3_WIDE),
                                35 - 11);
    }
    
    // ʱ��
    {
        // ����ʱ��--ʱ
        LCD_SetPosition(icon_16_move_time_coord[0][0], icon_16_move_time_coord[0][1],
                        ICON_MOVE_DATA1_WIDE, ICON_MOVE_DATA1_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_move_time_addr[(hour / 10) % 10], ICON_MOVE_DATA1_SIZE);
        LCD_SetPosition(icon_16_move_time_coord[1][0], icon_16_move_time_coord[1][1],
                        ICON_MOVE_DATA1_WIDE, ICON_MOVE_DATA1_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_move_time_addr[hour % 10], ICON_MOVE_DATA1_SIZE);
        
        // ����ʱ��--�� 
        LCD_SetPosition(icon_16_move_time_coord[2][0], icon_16_move_time_coord[2][1],
                        ICON_MOVE_DATA1_WIDE, ICON_MOVE_DATA1_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_move_time_addr[(minute / 10) % 10], ICON_MOVE_DATA1_SIZE);
        LCD_SetPosition(icon_16_move_time_coord[3][0], icon_16_move_time_coord[3][1],
                        ICON_MOVE_DATA1_WIDE, ICON_MOVE_DATA1_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_move_time_addr[minute % 10], ICON_MOVE_DATA1_SIZE); 
        
        // ����ʱ��--ð��
        LCD_SetPosition(icon_16_move_time_dot_coord[0][0], icon_16_move_time_dot_coord[0][1],
                        ICON_MOVE_DATA1DOT_WIDE, ICON_MOVE_DATA1DOT_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_move_time_dot_addr[0], ICON_MOVE_DATA1DOT_SIZE);
    }
    
    // ����1
    if(index == 0)
    {
        LCD_SetPosition(icon_16_inform_bg_coord[0][0], icon_16_inform_bg_coord[0][1], 
                        ICON_INFORM_BACKGROUND1_WIDE, ICON_INFORM_BACKGROUND1_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_inform_bg_addr[0], ICON_INFORM_BACKGROUND1_SIZE); 
    }
    
    // ��Ϣ����ͼ��
    if(index == 0)
    {
        if(msg_type >= 2)
        {
            LCD_SetPosition(icon_16_inform_icon_coord[0][0], icon_16_inform_icon_coord[0][1], 
                            ICON_INFORM_ICON1_WIDE, ICON_INFORM_ICON1_HIGH);
            LCD_DriverWriteDataFromExtFlash(icon_16_inform_icon_addr[msg_type - 2], ICON_INFORM_ICON1_SIZE); 
        }
    }
    
    // ����2
    if(index == 0)
    {
        LCD_SetPosition(icon_16_inform_bg_coord[1][0], icon_16_inform_bg_coord[1][1], 
                        ICON_INFORM_BACKGROUND2_WIDE, ICON_INFORM_BACKGROUND2_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_inform_bg_addr[1], ICON_INFORM_BACKGROUND2_SIZE); 
    }
    
    // ����3
    if(index == 0)
    {
        LCD_SetPosition(icon_16_inform_bg_coord[2][0], icon_16_inform_bg_coord[2][1], 
                        ICON_INFORM_BACKGROUND3_WIDE, ICON_INFORM_BACKGROUND3_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_inform_bg_addr[2], ICON_INFORM_BACKGROUND3_SIZE); 
    }
    // ��Ϣ����ͼ��2
    if(index == 0)
    {
        if(msg_type >= 2)
        {
            LCD_SetPosition(icon_16_inform_icon2_coord[0][0], icon_16_inform_icon2_coord[0][1], 
                            ICON_INFORM_ICON2_WIDE, ICON_INFORM_ICON2_HIGH);
            LCD_DriverWriteDataFromExtFlash(icon_16_inform_icon2_addr[language % 2][msg_type - 2], ICON_INFORM_ICON2_SIZE); 
        }
    }
    
    // ����4
    if(index == 0)
    {
        LCD_SetPosition(icon_16_inform_bg_coord[3][0], icon_16_inform_bg_coord[3][1], 
                        ICON_INFORM_BACKGROUND4_WIDE, ICON_INFORM_BACKGROUND4_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_inform_bg_addr[3], ICON_INFORM_BACKGROUND4_SIZE); 
    }
    
#if 0	
    // ����5
    if(index == 0)
    {
        LCD_SetPosition(icon_16_inform_bg_coord[4][0], icon_16_inform_bg_coord[4][1], 
                        ICON_INFORM_BACKGROUND5_WIDE, ICON_INFORM_BACKGROUND5_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_inform_bg_addr[4], ICON_INFORM_BACKGROUND5_SIZE); 
    }
	
#endif 	
}
 
/****************************************************************
** Function name:           Gui_PageMessageNotificationContent
** Descriptions:            ��ʾ��Ϣ���ͽ���
** input parameters:        display_buf����Ϣ����ָ��
                            buf_len�����ݳ���
                            page_index:�ڼ�ҳ
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2018-05-25
**----------------------------------------------------------------
*****************************************************************/ 
void Gui_PageMessageNotificationContent(uint16_t index, uint8_t *display_buf, uint16_t buf_len, uint8_t page_index)
{
    Font_DisplayContentTextFirstPage2X(display_buf, buf_len, 90, 3);
}

/****************************************************************
** Function name:           Gui_PageStarget
** Descriptions:            �˶�Ŀ�����ѽ���
** input parameters:        index����̬ͼ����
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2018-05-25
**----------------------------------------------------------------
*****************************************************************/
void Gui_PageStarget(uint16_t index, uint32_t step_target_cnt, uint8_t language)
{
    if(language)
    {
        language = 1;
    }
    
    // �ʹ�ͼ���ϲ�����
    if(index == 0)
    {
        Gui_PageClearBackground(0, 
                                0, 
                                MAX_X_SIZE,
                                icon_16_starget_icon1_coord[0][1]);
    }
    
    // �ʹ�ͼ��������
    if(index == 0)
    {
        Gui_PageClearBackground(0, 
                                icon_16_starget_icon1_coord[0][1], 
                                icon_16_starget_icon1_coord[0][0],
                                ICON_STANDARD_ICON1_HIGH);
    }
    
    // �ʹڶ�ͼ��ʾ
    if(index < 10)
    {
        LCD_SetPosition(icon_16_starget_icon1_coord[0][0], icon_16_starget_icon1_coord[0][1], 
                        ICON_STANDARD_ICON1_WIDE, ICON_STANDARD_ICON1_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_starget_icon1_addr[index % 10], ICON_STANDARD_ICON1_SIZE); 
    }
    else
    {
        LCD_SetPosition(icon_16_starget_icon2_coord[0][0], icon_16_starget_icon2_coord[0][1], 
                        ICON_STANDARD_ICON3_WIDE, ICON_STANDARD_ICON3_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_starget_icon2_addr[language % 2][(index - 10) % 5], ICON_STANDARD_ICON3_SIZE);
    }
    
    // �ʹ�ͼ��������
    if(index == 0)
    {
        Gui_PageClearBackground(icon_16_starget_icon1_coord[0][0] + ICON_STANDARD_ICON1_WIDE, 
                                icon_16_starget_icon1_coord[0][1], 
                                MAX_X_SIZE - (icon_16_starget_icon1_coord[0][0] + ICON_STANDARD_ICON1_WIDE),
                                ICON_STANDARD_ICON1_HIGH);
    }
    
    // ��Ļ�ײ�����
    if(index == 0)
    {
        Gui_PageClearBackground(0, 
                                icon_16_starget_icon1_coord[0][1] + ICON_STANDARD_ICON1_HIGH, 
                                MAX_X_SIZE,
                                MAX_Y_SIZE - (icon_16_starget_icon1_coord[0][1] + ICON_STANDARD_ICON1_HIGH));
    }
}

/****************************************************************
** Function name:           Gui_PageBleDiscRemind
** Descriptions:            ������������
** input parameters:        index����̬ͼ����
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2018-05-25
**----------------------------------------------------------------
*****************************************************************/
void Gui_PageBleDiscRemind(uint16_t index)
{
    // ͼ���ϲ�����
    if(index == 0)
    {
        Gui_PageClearBackground(0, 
                                0, 
                                MAX_X_SIZE,
                                icon_16_blue_disc_coord[0][1]);
    }
    
    // ͼ��������
    if(index == 0)
    {
        Gui_PageClearBackground(0, 
                                icon_16_blue_disc_coord[0][1], 
                                icon_16_blue_disc_coord[0][0],
                                ICON_DISCONNECTED_ICON1_HIGH);
    }
    
    // ��ͼͼ��ʾ
    if(index == 0)
    {
        LCD_SetPosition(icon_16_blue_disc_coord[0][0], icon_16_blue_disc_coord[0][1], 
                        ICON_DISCONNECTED_ICON1_WIDE, ICON_DISCONNECTED_ICON1_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_blue_disc_addr[0], ICON_DISCONNECTED_ICON1_SIZE); 
    }

    // ��ͼ��ʾ
    {
        LCD_SetPosition(icon_16_blue_dynamic_coord[0][0], icon_16_blue_dynamic_coord[0][1], 
                        ICON_DISCONNECTED_ICON2_WIDE, ICON_DISCONNECTED_ICON2_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_blue_dynamic_addr[index % 6], ICON_DISCONNECTED_ICON2_SIZE); 
    }
    
    // ͼ��������
    if(index == 0)
    {
        Gui_PageClearBackground(icon_16_blue_disc_coord[0][0] + ICON_DISCONNECTED_ICON1_WIDE, 
                                icon_16_blue_disc_coord[0][1], 
                                MAX_X_SIZE - (icon_16_blue_disc_coord[0][0] + ICON_DISCONNECTED_ICON1_WIDE),
                                ICON_DISCONNECTED_ICON1_HIGH);
    }
    
    // ��Ļ�ײ�����
    if(index == 0)
    {
        Gui_PageClearBackground(0, 
                                icon_16_blue_disc_coord[0][1] + ICON_DISCONNECTED_ICON1_HIGH, 
                                MAX_X_SIZE,
                                MAX_Y_SIZE - (icon_16_blue_disc_coord[0][1] + ICON_DISCONNECTED_ICON1_HIGH));
    }
}



/****************************************************************
** Function name:           Gui_PageOxygen
** Descriptions:            У׼������ʾ
** input parameters:        index����̬ͼ����
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2018-05-26
****************************************************************/
void Gui_PageOxygen(uint16_t index, uint8_t progress, uint8_t *progress_bk, uint8_t oxygen_cnt, uint8_t language)
{
   
}


/****************************************************************
** Function name:           Gui_PageMessageNotificationIcon02
** Descriptions:            ��ʾ��Ϣ���ͽ���
** input parameters:        msg_type����Ϣ������
**                          page_index����̬ͼ����
** output parameters:       ��
** Returned value:          ��
** Created by:              JSH              
** Created Date:            2018-09-30
**----------------------------------------------------------------
*****************************************************************/ 
void Gui_PageMessageNotificationIcon02(uint16_t index, uint8_t msg_type, uint8_t page_index, uint8_t hour, uint8_t minute, uint8_t language)
{     
    if(language)
    {
        language = 1;
    }
    
    // ��Ļ�ϲ�����
    if(index == 0)
    {
        Gui_PageClearBackground(0, 
                                0, 
                                MAX_X_SIZE,
                                icon_16_inform02_icon_coord[0][1]);
    }
	
	// ��Ϣ����-��-����
    if(index == 0)
    {
        Gui_PageClearBackground(0, 
                                icon_16_inform02_icon_coord[0][1], 
                                icon_16_inform02_icon_coord[0][0],
                                ICON_ZINFO2_ICON1_HIGH);
    }
	
	// ��Ϣ����ͼ��
    if(index == 0)
    {
        if(msg_type >= 2)
        {
            LCD_SetPosition(icon_16_inform02_icon_coord[0][0], icon_16_inform02_icon_coord[0][1], 
                            ICON_ZINFO2_ICON1_WIDE, ICON_ZINFO2_ICON1_HIGH);
            LCD_DriverWriteDataFromExtFlash(icon_16_inform02_icon_addr[msg_type - 2], ICON_ZINFO2_ICON1_SIZE); 
        }
    }
	
	// ��Ϣ����-��ͼ-ˮƽ���-����
    if(index == 0)
    {
        Gui_PageClearBackground(icon_16_inform02_icon_coord[0][0] + ICON_ZINFO2_ICON1_WIDE, 
                                icon_16_inform02_icon_coord[0][1], 
                                icon_16_inform02_dynamic_coord[0][0] - (icon_16_inform02_icon_coord[0][0] + ICON_ZINFO2_ICON1_WIDE),
                                ICON_ZINFO2_ICON1_HIGH);
    }
	
	// ��ͼ
	{
		LCD_SetPosition(icon_16_inform02_dynamic_coord[0][0], icon_16_inform02_dynamic_coord[0][1], 
						ICON_ZINFO2_ICON2_WIDE, ICON_ZINFO2_ICON2_HIGH);
		LCD_DriverWriteDataFromExtFlash(icon_16_inform02_dynamic_addr[index % 14], ICON_ZINFO2_ICON2_SIZE);
	}
	
	// ��ͼ-�ֱ�ͼ-ˮƽ���-����
    if(index == 0)
    {
        Gui_PageClearBackground(icon_16_inform02_dynamic_coord[0][0] + ICON_ZINFO2_ICON2_WIDE, 
                                icon_16_inform02_icon_coord[0][1], 
                                icon_16_inform02_icon01_coord[0][0] - (icon_16_inform02_dynamic_coord[0][0] + ICON_ZINFO2_ICON2_WIDE),
                                ICON_ZINFO2_ICON1_HIGH);
    }
	
	if(index % 14 == 0)
	{
		LCD_SetPosition(icon_16_inform02_icon01_coord[0][0], icon_16_inform02_icon01_coord[0][1], 
						ICON_ZINFO2_ICON4_WIDE, ICON_ZINFO2_ICON4_HIGH);
		LCD_DriverWriteDataFromExtFlash(icon_16_inform02_icon01_addr[0], ICON_ZINFO2_ICON4_SIZE);
	}
	
	if((index != 0)&&(index % 14 == 10))
	{
		LCD_SetPosition(icon_16_inform02_icon02_coord[0][0], icon_16_inform02_icon02_coord[0][1], 
						ICON_ZINFO2_ICON3_WIDE, ICON_ZINFO2_ICON3_HIGH);
		LCD_DriverWriteDataFromExtFlash(icon_16_inform02_icon02_addr[0], ICON_ZINFO2_ICON3_SIZE);
	}
    
    // ʱ�䱳������
    if(index == 0)
    {
        Gui_PageClearBackground(icon_16_inform02_icon01_coord[0][0] + ICON_ZINFO2_ICON4_WIDE, 
                                icon_16_inform02_icon_coord[0][1], 
                                MAX_X_SIZE - (icon_16_inform02_icon01_coord[0][0] + ICON_ZINFO2_ICON4_WIDE),
                                (icon_16_move_time_coord[0][1] + ICON_MOVE_DATA1_HIGH) - 
								icon_16_inform02_icon_coord[0][1]);
    }
    
    // ʱ��
    {
        // ����ʱ��--ʱ
        LCD_SetPosition(icon_16_move_time_coord[0][0], icon_16_move_time_coord[0][1],
                        ICON_MOVE_DATA1_WIDE, ICON_MOVE_DATA1_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_move_time_addr[(hour / 10) % 10], ICON_MOVE_DATA1_SIZE);
        LCD_SetPosition(icon_16_move_time_coord[1][0], icon_16_move_time_coord[1][1],
                        ICON_MOVE_DATA1_WIDE, ICON_MOVE_DATA1_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_move_time_addr[hour % 10], ICON_MOVE_DATA1_SIZE);
        
        // ����ʱ��--�� 
        LCD_SetPosition(icon_16_move_time_coord[2][0], icon_16_move_time_coord[2][1],
                        ICON_MOVE_DATA1_WIDE, ICON_MOVE_DATA1_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_move_time_addr[(minute / 10) % 10], ICON_MOVE_DATA1_SIZE);
        LCD_SetPosition(icon_16_move_time_coord[3][0], icon_16_move_time_coord[3][1],
                        ICON_MOVE_DATA1_WIDE, ICON_MOVE_DATA1_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_move_time_addr[minute % 10], ICON_MOVE_DATA1_SIZE); 
        
        // ����ʱ��--ð��
        LCD_SetPosition(icon_16_move_time_dot_coord[0][0], icon_16_move_time_dot_coord[0][1],
                        ICON_MOVE_DATA1DOT_WIDE, ICON_MOVE_DATA1DOT_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_move_time_dot_addr[0], ICON_MOVE_DATA1DOT_SIZE);
    }
    
    // ʱ���²�-����
    if(index == 0)
    {
        Gui_PageClearBackground(icon_16_inform02_icon01_coord[0][0] + ICON_ZINFO2_ICON4_WIDE, 
                                icon_16_move_time_coord[0][1] + ICON_MOVE_DATA1_HIGH, 
                                MAX_X_SIZE - (icon_16_inform02_icon01_coord[0][0] + ICON_ZINFO2_ICON4_WIDE),
                                (icon_16_inform02_icon01_coord[0][1] + ICON_ZINFO2_ICON4_HIGH) - 
								(icon_16_move_time_coord[0][1] + ICON_MOVE_DATA1_HIGH));
    }
	
	// ��Ϣ����ͼ��-��ͼ-�²�-����
    if(index == 0)
    {
        Gui_PageClearBackground(0, 
                                icon_16_inform02_icon_coord[0][1] + ICON_ZINFO2_ICON1_HIGH, 
                                icon_16_inform02_icon01_coord[0][0],
                                (icon_16_inform02_icon01_coord[0][1] + ICON_ZINFO2_ICON4_HIGH) - 
								(icon_16_inform02_icon_coord[0][1] + ICON_ZINFO2_ICON1_HIGH));
    }
    
//	// ��Ļ�ײ�-����
//    if(index == 0)
//    {
//        Gui_PageClearBackground(0, 
//                                icon_16_inform02_icon01_coord[0][1] + ICON_ZINFO2_ICON4_HIGH, 
//                                MAX_X_SIZE,
//                                MAX_Y_SIZE - (icon_16_inform02_icon01_coord[0][1] + ICON_ZINFO2_ICON4_HIGH));
//    }
}

/****************************************************************
** Function name:           Gui_PageMessageNotificationContent02
** Descriptions:            ��ʾ��Ϣ���ͽ���
** input parameters:        display_buf����Ϣ����ָ��
                            buf_len�����ݳ���
                            page_index:�ڼ�ҳ
** output parameters:       ��
** Returned value:          ��
** Created by:              JSH              
** Created Date:            2018-09-30
**----------------------------------------------------------------
*****************************************************************/ 
void Gui_PageMessageNotificationContent02(uint16_t index, uint8_t *display_buf, uint16_t buf_len, uint8_t page_index)
{
	// ��Ļ�ײ�-����
    if(index == 0)
    {
        Gui_PageClearBackground(0, 
                                icon_16_inform02_icon01_coord[0][1] + ICON_ZINFO2_ICON4_HIGH, 
                                MAX_X_SIZE,
                                MAX_Y_SIZE - (icon_16_inform02_icon01_coord[0][1] + ICON_ZINFO2_ICON4_HIGH));
    }
	
    Font_DisplayContentTextFirstPage2X(display_buf, buf_len, 52, 5);
}

