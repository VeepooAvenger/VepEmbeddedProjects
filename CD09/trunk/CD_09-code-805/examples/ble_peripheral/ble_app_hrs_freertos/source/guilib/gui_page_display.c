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
#include "gui_clock_display.h"

extern uint8_t main_background_color;

//extern uint8_t dbug_dis_ui_num;
//extern uint8_t dbug_dis_ui_flg;

/****************************************************************
** Function name:           Gui_LcdDriverInit
** Descriptions:            LCD初始化
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              YX              
** Created Date:            2017-08-07
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Gui_LcdDriverInit(void)
{ 	
    LCD_DriverInit();   //驱动初始化
}

/****************************************************************
** Function name:           Gui_LcdDriverOn
** Descriptions:            LCD打开屏幕
** input parameters:        无
** output parameters:       无
** Returned value:          无
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
** Descriptions:            LCD关闭屏幕
** input parameters:        无
** output parameters:       无
** Returned value:          无
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
** Descriptions:            LCD关闭屏幕背光
** input parameters:        无
** output parameters:       无
** Returned value:          无
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
** Descriptions:            LCD唤醒
** input parameters:        无
** output parameters:       无
** Returned value:          无
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
** Descriptions:            区域清屏
** input parameters:        s_column：横向坐标
**                          s_page：纵向坐标
**                          column_size：横向大小
**                          page_size：纵向大小
** output parameters:       无
** Returned value:          无
** Created by:              YX              
** Created Date:            2017-08-07
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Gui_PageClearBackground(uint8_t s_column, uint8_t s_page, uint8_t column_size, uint8_t page_size)
{   
    //WJ.20180427.防止越界处理
    if(s_column >= LCD_MAX_COLUMN ||  s_page >= LCD_MAX_ROW
      || column_size > LCD_MAX_COLUMN || page_size > LCD_MAX_ROW)
    {
        return;
    }
    
    if(column_size * page_size == 0)
    {
        return ;
    }
    //区域清屏
    LCD_OpenScreenByPosition(s_column, s_page, column_size, page_size); 
}

/****************************************************************
** Function name:           LCD_SetColorScreenByPosition
** Descriptions:            更新某块区域的颜色（单一）
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
void Gui_PageRefreshBackground(uint8_t s_column, uint8_t s_page, uint8_t column_size, uint8_t page_size, uint16_t color)
{     
    //WJ.20180427.防止越界处理
    if(s_column >= LCD_MAX_COLUMN ||  s_page >= LCD_MAX_ROW
      || column_size > LCD_MAX_COLUMN || page_size > LCD_MAX_ROW)
    {
        return;
    }
    if(page_size * column_size == 0)
    {
        return;
    } 
    /*区域刷屏为单种颜色*/
    LCD_SetColorScreenByPosition(s_column, s_page, column_size, page_size, color); 
}

/****************************************************************
** Function name:           Gui_DisDbugUi
** Descriptions:            显示UI调试界面
** input parameters:        无
** output parameters:       无
** Returned value:          无
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
** Descriptions:            显示X
** input parameters:        无
** output parameters:       无
** Returned value:          无
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
** Descriptions:            显示XXX
** input parameters:        无
** output parameters:       无
** Returned value:          无
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
** Descriptions:            显示主界面的背景设置
** input parameters:        color_type：背景颜色
** output parameters:       无
** Returned value:          无
** Created by:              WJ             
** Created Date:            2017-08-09
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
//const uint16_t backgroud_color_buf[4][3] = {
//    {   //white
//    0x0000, 0x40FE, 0x7DEF
//    },
//    {   //red
//    0x0000, 0x03C0, 0x00C8//, 0x08C0
//    },
//    {   //yellow
//    0x0000, 0x07FF, 0xA6F6
//    },
//    {   //green
//    0x0000, 0x44D7, 0xE294
//    }
//};

void Gui_PageMainBackGround(void)
{     
    uint8_t background_type = 0;
    
    background_type = main_background_color;
    if(main_background_color < 2)//2
    {
        LCD_SetPosition(icon_16_main_by_coord[0][0], icon_16_main_by_coord[0][1], 
                        ICON_MAIN2_ICON6_WIDE, ICON_MAIN2_ICON6_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_main_by_addr[background_type][0], ICON_MAIN2_ICON6_SIZE);
        
        LCD_SetPosition(icon_16_main_by_coord[1][0], icon_16_main_by_coord[1][1], 
                        ICON_MAIN2_ICON6_WIDE, ICON_MAIN2_ICON6_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_main_by_addr[background_type][1], ICON_MAIN2_ICON6_SIZE);   
    }
    else if(main_background_color < 4)
    {
        /* 表盘处理*/       
        LCD_SetPosition(icon_16_main_clock_bg_coord[0][0], icon_16_main_clock_bg_coord[0][1], 
                        ICON_MAIN3_ICON_WIDE, ICON_MAIN3_ICON_WIDE);
        LCD_DriverWriteDataFromExtFlash(icon_16_main_clock_bg_addr[0][main_background_color - 3], ICON_MAIN3_ICON_WIDE);
    }
}

/****************************************************************
** Function name:           Gui_UpdataWheather
** Descriptions:            天气显示处理
** input parameters:        weather：类型，temperature_low：低温；temperature_high：高温
** output parameters:       无
** Returned value:          无
** Created by:              WJ             
** Created Date:            2017-08-09
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Gui_UpdataWheather(uint8_t weather, uint8_t temperature_low, uint8_t temperature_high)
{
    int8_t t_low = 0;
    int8_t t_high = 0;
    uint8_t start_column = 0;
    uint8_t start_page = 0;
    uint8_t temp_p = 0;
    
    t_low = temperature_low;
    t_high = temperature_high;

    if(t_low == t_high)
    {
        LCD_SetPosition(icon_16_weather_icon_coord[temp_p][0], icon_16_weather_icon_coord[temp_p][1],
                        ICON_MAIN_ICON_WIDE, ICON_MAIN_ICON_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_weather_icon_addr[0][weather], ICON_MAIN_ICON_SIZE); 

        start_column = icon_16_weather_data_icon_coord[temp_p][0];
        start_page = icon_16_weather_data_icon_coord[temp_p][1];

        if(t_low < 0)
        {
            //
            t_low = -t_low;
            Gui_PageRefreshBackground(start_column, start_page + 4, 4, 2, 0xFFFF);
            start_column += 4;
        }

        if(t_low >= 10)
        {
            LCD_SetPosition(start_column, start_page,
                            ICON_MAIN_DATA5_WIDE, ICON_MAIN_DATA5_HIGH);
            LCD_DriverWriteDataFromExtFlash(icon_16_weather_data_icon_addr[0][(t_low /10) % 10], ICON_MAIN_DATA5_SIZE); 
            start_column += ICON_MAIN_DATA5_WIDE;
        }
        LCD_SetPosition(start_column, start_page,
                        ICON_MAIN_DATA5_WIDE, ICON_MAIN_DATA5_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_weather_data_icon_addr[0][t_low % 10], ICON_MAIN_DATA5_SIZE); 
        start_column += ICON_MAIN_DATA5_WIDE;

        start_page = icon_16_weather_degree_icon_coord[0][1];
        LCD_SetPosition(start_column, start_page,
                    ICON_MAIN_DATA7_WIDE, ICON_MAIN_DATA7_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_weather_degree_icon_addr[0][0], ICON_MAIN_DATA7_SIZE); 
    }
    else
    {
        temp_p = 2;
        if(t_low < 0)
        {
            temp_p++;
        }
        if(t_low%10 != 0)
        {
            temp_p++;
        }
        
        if(t_high < 0)
        {
            temp_p++;
        }
        if(t_high % 10 != 0)
        {
            temp_p++;
        }        
        
        LCD_SetPosition(icon_16_weather_icon_coord[temp_p][0], icon_16_weather_icon_coord[0][1],
                        ICON_MAIN_ICON_WIDE, ICON_MAIN_ICON_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_weather_icon_addr[0][weather], ICON_MAIN_ICON_SIZE); 

        start_column = icon_16_weather_data_icon_coord[temp_p][0];
        start_page = icon_16_weather_data_icon_coord[temp_p][1];

        if(t_low < 0)
        {
            //
            t_low = -t_low;
            Gui_PageRefreshBackground(start_column, start_page + 4, 4, 2, 0xFFFF);
            start_column += 4;
        }

        if(t_low >= 10)
        {
            LCD_SetPosition(start_column, start_page,
                            ICON_MAIN_DATA5_WIDE, ICON_MAIN_DATA5_HIGH);
            LCD_DriverWriteDataFromExtFlash(icon_16_weather_data_icon_addr[0][(t_low /10) % 10], ICON_MAIN_DATA5_SIZE); 
            start_column += ICON_MAIN_DATA5_WIDE;
        }
        LCD_SetPosition(start_column, start_page,
                        ICON_MAIN_DATA5_WIDE, ICON_MAIN_DATA5_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_weather_data_icon_addr[0][t_low % 10], ICON_MAIN_DATA5_SIZE); 
        start_column += ICON_MAIN_DATA5_WIDE + 1;

        //"~"
        LCD_SetPosition(start_column, start_page + 4,
                        ICON_MAIN_DATA6_WIDE, ICON_MAIN_DATA6_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_weather_line_icon_addr[0][0], ICON_MAIN_DATA6_SIZE); 
        start_column += ICON_MAIN_DATA6_WIDE + 1;

        if(t_high < 0)
        {
            //
            t_high = -t_high;
            Gui_PageRefreshBackground(start_column, start_page + 4, 4, 2, 0xFFFF);
            start_column += 4;
        }

        if(t_high >= 10)
        {
            LCD_SetPosition(start_column, start_page,
                            ICON_MAIN_DATA5_WIDE, ICON_MAIN_DATA5_HIGH);
            LCD_DriverWriteDataFromExtFlash(icon_16_weather_data_icon_addr[0][(t_high /10) % 10], ICON_MAIN_DATA5_SIZE); 
            start_column += ICON_MAIN_DATA5_WIDE;
        }
        LCD_SetPosition(start_column, start_page,
                        ICON_MAIN_DATA5_WIDE, ICON_MAIN_DATA5_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_weather_data_icon_addr[0][t_high % 10], ICON_MAIN_DATA5_SIZE); 
        start_column += ICON_MAIN_DATA5_WIDE;
        //"。"
        start_page = icon_16_weather_degree_icon_coord[0][1];
        LCD_SetPosition(start_column, start_page,
                    ICON_MAIN_DATA7_WIDE, ICON_MAIN_DATA7_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_weather_degree_icon_addr[0][0], ICON_MAIN_DATA7_SIZE);                
    }   
}

/****************************************************************
** Function name:           Gui_PageMainDateHour
** Descriptions:            显示主界面的时钟和日期
** input parameters:        month：月
**                          day：天
**                          hour：时
**                          minute：分钟
**                          week：星期
**                          language：语言
**                          am_pm_format：AM,PM的状态
** output parameters:       无
** Returned value:          无
** Created by:              YX              
** Created Date:            2017-08-07
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/     
void Gui_PageMainDateHour(uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t week, 
                          uint8_t language, uint8_t am_pm_format, uint8_t weather, uint8_t temperature_low,
                          uint8_t temperature_high)
{
    uint8_t background_type = 0;

    
    if(main_background_color < 2)
    {
        background_type = main_background_color;        
        
        /* 绘制月 */
        LCD_SetPosition(icon_16_main_date_coord[background_type][0][0], icon_16_main_date_coord[background_type][0][1], 
                        ICON_MAIN2_DATA1_WIDE, ICON_MAIN2_DATA1_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_main_date_addr[background_type][month / 10 % 10], ICON_MAIN2_DATA1_SIZE);
        LCD_SetPosition(icon_16_main_date_coord[background_type][1][0], icon_16_main_date_coord[background_type][1][1], 
                        ICON_MAIN2_DATA1_WIDE, ICON_MAIN2_DATA1_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_main_date_addr[background_type][month % 10], ICON_MAIN2_DATA1_SIZE);
        
        /* 绘制点 */
        LCD_SetPosition(icon_16_main_date_point_coord[0][0], icon_16_main_date_point_coord[0][1], 
                        ICON_MAIN1_DATA2_WIDE, ICON_MAIN1_DATA2_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_main_date_point_addr[background_type][0], ICON_MAIN1_DATA2_SIZE);
        
        /* 绘制日 */
        LCD_SetPosition(icon_16_main_date_coord[background_type][2][0], icon_16_main_date_coord[background_type][2][1], 
                        ICON_MAIN2_DATA1_WIDE, ICON_MAIN2_DATA1_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_main_date_addr[background_type][day / 10 % 10], ICON_MAIN2_DATA1_SIZE);
        LCD_SetPosition(icon_16_main_date_coord[background_type][3][0], icon_16_main_date_coord[background_type][3][1], 
                        ICON_MAIN2_DATA1_WIDE, ICON_MAIN2_DATA1_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_main_date_addr[background_type][day % 10], ICON_MAIN2_DATA1_SIZE);
        
        if(weather == 0 || weather > 8)
        {
            /* 绘制时间--时*/
            LCD_SetPosition(icon_16_main_time_coord[0][0][0], icon_16_main_time_coord[0][0][1],
                            ICON_MAIN1_DATA3_WIDE, ICON_MAIN1_DATA3_HIGH);
            LCD_DriverWriteDataFromExtFlash(icon_16_main_hour_addr[background_type][(hour / 10) % 10], ICON_MAIN1_DATA3_SIZE);
            LCD_SetPosition(icon_16_main_time_coord[0][1][0], icon_16_main_time_coord[0][1][1],
                            ICON_MAIN1_DATA3_WIDE, ICON_MAIN1_DATA3_HIGH);
            LCD_DriverWriteDataFromExtFlash(icon_16_main_hour_addr[background_type][hour % 10], ICON_MAIN1_DATA3_SIZE);
            
            /* 绘制时间--分 */
            LCD_SetPosition(icon_16_main_time_coord[0][2][0], icon_16_main_time_coord[0][2][1],
                            ICON_MAIN_DATA3_WIDE, ICON_MAIN_DATA3_HIGH);
            LCD_DriverWriteDataFromExtFlash(icon_16_main_minute_addr[0][(minute / 10) % 10], ICON_MAIN_DATA3_SIZE);
            LCD_SetPosition(icon_16_main_time_coord[0][3][0], icon_16_main_time_coord[0][3][1],
                            ICON_MAIN_DATA3_WIDE, ICON_MAIN_DATA3_HIGH);
            LCD_DriverWriteDataFromExtFlash(icon_16_main_minute_addr[0][minute % 10], ICON_MAIN_DATA3_SIZE);   
        }
        else
        {
            /* 绘制时间--时*/
            LCD_SetPosition(icon_16_main_time_coord[1][0][0], icon_16_main_time_coord[1][0][1],
                            ICON_MAIN1_DATA3_WIDE, ICON_MAIN1_DATA3_HIGH);
            LCD_DriverWriteDataFromExtFlash(icon_16_main_hour_addr[background_type][(hour / 10) % 10], ICON_MAIN1_DATA3_SIZE);
            LCD_SetPosition(icon_16_main_time_coord[1][1][0], icon_16_main_time_coord[1][1][1],
                            ICON_MAIN1_DATA3_WIDE, ICON_MAIN1_DATA3_HIGH);
            LCD_DriverWriteDataFromExtFlash(icon_16_main_hour_addr[background_type][hour % 10], ICON_MAIN1_DATA3_SIZE);
            
            /* 绘制时间--分 */
            LCD_SetPosition(icon_16_main_time_coord[1][2][0], icon_16_main_time_coord[1][2][1],
                            ICON_MAIN_DATA3_WIDE, ICON_MAIN_DATA3_HIGH);
            LCD_DriverWriteDataFromExtFlash(icon_16_main_minute_addr[0][(minute / 10) % 10], ICON_MAIN_DATA3_SIZE);
            LCD_SetPosition(icon_16_main_time_coord[1][3][0], icon_16_main_time_coord[1][3][1],
                            ICON_MAIN_DATA3_WIDE, ICON_MAIN_DATA3_HIGH);
            LCD_DriverWriteDataFromExtFlash(icon_16_main_minute_addr[0][minute % 10], ICON_MAIN_DATA3_SIZE);   
            
            Gui_UpdataWheather(weather, temperature_low, temperature_high);
              
        }
        /* 绘制星期 */
        LCD_SetPosition(icon_16_main_week_coord[0][0], icon_16_main_week_coord[0][1],
                        ICON_MAIN2_DATA4_WIDE, ICON_MAIN2_DATA4_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_main_week_addr[background_type][language][((week >= 1)?(week-1):week) % 7], ICON_MAIN2_DATA4_SIZE);
    }
    else if(main_background_color < 3)
    {

         /* 绘制月 */
        LCD_SetPosition(icon_16_main5_date_data_coord[0][0], icon_16_main5_date_data_coord[0][1], 
                        ICON_MAIN5_DATA4_WIDE, ICON_MAIN5_DATA4_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_main5_date_data_addr[month / 10 % 10], ICON_MAIN5_DATA4_SIZE);
        LCD_SetPosition(icon_16_main5_date_data_coord[1][0], icon_16_main5_date_data_coord[1][1], 
                        ICON_MAIN5_DATA4_WIDE, ICON_MAIN5_DATA4_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_main5_date_data_addr[month % 10], ICON_MAIN5_DATA4_SIZE);
        
        /* 绘制点 */
        LCD_SetPosition(icon_16_main5_datadot_icon_coord[0][0], icon_16_main5_datadot_icon_coord[0][1], 
                        ICON_MAIN5_DATA4DOT_WIDE, ICON_MAIN5_DATA4DOT_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_main5_datadot_icon_addr[0], ICON_MAIN5_DATA4DOT_SIZE);
        
        /* 绘制日 */
        LCD_SetPosition(icon_16_main5_date_data_coord[2][0], icon_16_main5_date_data_coord[2][1], 
                        ICON_MAIN5_DATA4_WIDE, ICON_MAIN5_DATA4_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_main5_date_data_addr[day / 10 % 10], ICON_MAIN5_DATA4_SIZE);
        LCD_SetPosition(icon_16_main5_date_data_coord[3][0], icon_16_main5_date_data_coord[3][1], 
                        ICON_MAIN5_DATA4_WIDE, ICON_MAIN5_DATA4_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_main5_date_data_addr[day % 10], ICON_MAIN5_DATA4_SIZE);
        
        /* 绘制时间--时*/
        LCD_SetPosition(icon_16_main5_hour_data_coord[0][0], icon_16_main5_hour_data_coord[0][1],
                        ICON_MAIN5_DATA1_WIDE, ICON_MAIN5_DATA1_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_main5_hour_data_addr[0][(hour / 10) % 10], ICON_MAIN5_DATA1_SIZE);
        LCD_SetPosition(icon_16_main5_hour_data_coord[1][0], icon_16_main5_hour_data_coord[1][1],
                        ICON_MAIN5_DATA1_WIDE, ICON_MAIN5_DATA1_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_main5_hour_data_addr[0][hour % 10], ICON_MAIN5_DATA1_SIZE);
        
        /* 绘制时间--分 */
        LCD_SetPosition(icon_16_main5_hour_data_coord[2][0], icon_16_main5_hour_data_coord[2][1],
                        ICON_MAIN5_DATA2_WIDE, ICON_MAIN5_DATA2_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_main5_hour_data_addr[1][(minute / 10) % 10], ICON_MAIN5_DATA2_SIZE);
        LCD_SetPosition(icon_16_main5_hour_data_coord[3][0], icon_16_main5_hour_data_coord[3][1],
                        ICON_MAIN5_DATA2_WIDE, ICON_MAIN5_DATA2_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_main5_hour_data_addr[1][minute % 10], ICON_MAIN5_DATA2_SIZE); 
        
        /* 绘制星期 */
        LCD_SetPosition(icon_16_main5_week_data_coord[0][0], icon_16_main5_week_data_coord[0][1],
                        ICON_MAIN5_WEEK_WIDE, ICON_MAIN5_WEEK_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_main5_week_data_addr[language][((week >= 1)?(week-1):week) % 7], ICON_MAIN5_WEEK_SIZE);
    }
    else if(main_background_color < 4)
    {
        /* 绘制红色背景 */
        LCD_SetPosition(icon_16_main6_background_icon_coord[0][0], icon_16_main6_background_icon_coord[0][1], 
                        ICON_MAIN6_ICON5_WIDE, ICON_MAIN6_ICON5_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_main6_background_icon_addr[0], ICON_MAIN6_ICON5_SIZE);
        
         /* 绘制月 */
        LCD_SetPosition(icon_16_main6_date_data_coord[0][0], icon_16_main6_date_data_coord[0][1], 
                        ICON_MAIN6_DATA2_WIDE, ICON_MAIN6_DATA2_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_main6_date_data_addr[month / 10 % 10], ICON_MAIN6_DATA2_SIZE);
        LCD_SetPosition(icon_16_main6_date_data_coord[1][0], icon_16_main6_date_data_coord[1][1], 
                        ICON_MAIN6_DATA2_WIDE, ICON_MAIN6_DATA2_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_main6_date_data_addr[month % 10], ICON_MAIN6_DATA2_SIZE);
        
        /* 绘制点 */
        LCD_SetPosition(icon_16_main6_datadot_icon_coord[0][0], icon_16_main6_datadot_icon_coord[0][1], 
                        ICON_MAIN6_DATA2DOT_WIDE, ICON_MAIN6_DATA2DOT_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_main6_datadot_icon_addr[0], ICON_MAIN6_DATA2DOT_SIZE);
        
        /* 绘制日 */
        LCD_SetPosition(icon_16_main6_date_data_coord[2][0], icon_16_main6_date_data_coord[2][1], 
                        ICON_MAIN6_DATA2_WIDE, ICON_MAIN6_DATA2_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_main6_date_data_addr[day / 10 % 10], ICON_MAIN6_DATA2_SIZE);
        LCD_SetPosition(icon_16_main6_date_data_coord[3][0], icon_16_main6_date_data_coord[3][1], 
                        ICON_MAIN6_DATA2_WIDE, ICON_MAIN6_DATA2_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_main6_date_data_addr[day % 10], ICON_MAIN6_DATA2_SIZE);
        
        /* 绘制时间--时*/
        LCD_SetPosition(icon_16_main6_hour_data_coord[0][0], icon_16_main6_hour_data_coord[0][1],
                        ICON_MAIN6_DATA1_WIDE, ICON_MAIN6_DATA1_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_main6_hour_data_addr[0][(hour / 10) % 10], ICON_MAIN6_DATA1_SIZE);
        LCD_SetPosition(icon_16_main6_hour_data_coord[1][0], icon_16_main6_hour_data_coord[1][1],
                        ICON_MAIN6_DATA1_WIDE, ICON_MAIN6_DATA1_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_main6_hour_data_addr[0][hour % 10], ICON_MAIN6_DATA1_SIZE);
        
        /* 绘制时间--分 */
        LCD_SetPosition(icon_16_main6_hour_data_coord[2][0], icon_16_main6_hour_data_coord[2][1],
                        ICON_MAIN6_DATA1_WIDE, ICON_MAIN6_DATA1_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_main6_hour_data_addr[1][(minute / 10) % 10], ICON_MAIN6_DATA1_SIZE);
        LCD_SetPosition(icon_16_main6_hour_data_coord[3][0], icon_16_main6_hour_data_coord[3][1],
                        ICON_MAIN6_DATA1_WIDE, ICON_MAIN6_DATA1_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_main6_hour_data_addr[1][minute % 10], ICON_MAIN6_DATA1_SIZE); 
        
        /* 绘制星期 */
        LCD_SetPosition(icon_16_main6_week_data_coord[0][0], icon_16_main6_week_data_coord[0][1],
                        ICON_MAIN6_WEEK_WIDE, ICON_MAIN6_WEEK_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_main6_week_data_addr[language][((week >= 1)?(week-1):week) % 7], ICON_MAIN6_WEEK_SIZE);
    }
    else if(main_background_color < 5)
    {
        
        /* 绘制日 */
        LCD_SetPosition(icon_16_main7_date_data_coord[0][0], icon_16_main7_date_data_coord[0][1], 
                        ICON_MAIN4_DATA3_WIDE, ICON_MAIN4_DATA3_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_main7_date_data_addr[day / 10 % 10], ICON_MAIN4_DATA3_SIZE);
        LCD_SetPosition(icon_16_main7_date_data_coord[1][0], icon_16_main7_date_data_coord[1][1], 
                        ICON_MAIN4_DATA3_WIDE, ICON_MAIN4_DATA3_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_main7_date_data_addr[day % 10], ICON_MAIN4_DATA3_SIZE);
        
        /* 绘制时间--时*/
        LCD_SetPosition(icon_16_main7_hour_data_coord[0][0], icon_16_main7_hour_data_coord[0][1],
                        ICON_MAIN7_DATA1_WIDE, ICON_MAIN7_DATA1_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_main7_hour_data_addr[0][(hour / 10) % 10], ICON_MAIN7_DATA1_SIZE);
        LCD_SetPosition(icon_16_main7_hour_data_coord[1][0], icon_16_main7_hour_data_coord[1][1],
                        ICON_MAIN7_DATA1_WIDE, ICON_MAIN7_DATA1_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_main7_hour_data_addr[0][hour % 10], ICON_MAIN7_DATA1_SIZE);
        
        /* 绘制时间--分 */
        LCD_SetPosition(icon_16_main7_hour_data_coord[2][0], icon_16_main7_hour_data_coord[2][1],
                        ICON_MAIN7_DATA1_WIDE, ICON_MAIN7_DATA1_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_main7_hour_data_addr[1][(minute / 10) % 10], ICON_MAIN7_DATA1_SIZE);
        LCD_SetPosition(icon_16_main7_hour_data_coord[3][0], icon_16_main7_hour_data_coord[3][1],
                        ICON_MAIN7_DATA1_WIDE, ICON_MAIN7_DATA1_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_main7_hour_data_addr[1][minute % 10], ICON_MAIN7_DATA1_SIZE); 
        
        /* 绘制星期 */
        LCD_SetPosition(icon_16_main7_week_data_coord[0][0], icon_16_main7_week_data_coord[0][1],
                        ICON_MAIN7_WEEK_WIDE, ICON_MAIN7_WEEK_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_main7_week_data_addr[language][((week >= 1)?(week-1):week) % 7], ICON_MAIN7_WEEK_SIZE);
    }
    else if(main_background_color < 7)
    {
        Clock_DisplayDial(hour, minute, main_background_color - 3);
        /* 绘制月 */
        LCD_SetPosition(icon_16_main3_date_coord[0][0], icon_16_main3_date_coord[0][1], 
                        ICON_MAIN3_DATA_WIDE, ICON_MAIN3_DATA_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_main_date_addr[2][(month / 10) % 10], ICON_MAIN3_DATA_SIZE);
        LCD_SetPosition(icon_16_main3_date_coord[1][0], icon_16_main3_date_coord[1][1], 
                        ICON_MAIN3_DATA_WIDE, ICON_MAIN3_DATA_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_main_date_addr[2][month % 10], ICON_MAIN3_DATA_SIZE);
        
        /* 绘制点 */
        LCD_SetPosition(icon_16_main3_date_point_coord[0][0], icon_16_main3_date_point_coord[0][1], 
                        ICON_MAIN3_ICON3_WIDE, ICON_MAIN3_ICON3_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_main3_date_point_addr[0][0], ICON_MAIN3_ICON3_SIZE);
        
        /* 绘制日 */
        LCD_SetPosition(icon_16_main3_date_coord[2][0], icon_16_main3_date_coord[2][1], 
                        ICON_MAIN3_DATA_WIDE, ICON_MAIN3_DATA_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_main_date_addr[2][(day / 10) % 10], ICON_MAIN3_DATA_SIZE);
        LCD_SetPosition(icon_16_main3_date_coord[3][0], icon_16_main3_date_coord[3][1], 
                        ICON_MAIN3_DATA_WIDE, ICON_MAIN3_DATA_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_main_date_addr[2][day % 10], ICON_MAIN3_DATA_SIZE);
    }
}

/****************************************************************
** Function name:           Gui_PageMainAssisant
** Descriptions:            显示主界面其他图标
** input parameters:        index：动态图标的计数
**                          ble_status：蓝牙的状态
**                          am_pm_format：AM,PM的状态
**                          charge：充电状态
**                          battery_level：电池的等级
** output parameters:       无       
** Returned value:          无
** Created by:              YX              
** Created Date:            2017-08-07
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/
void Gui_PageMainAssisant(uint8_t index, uint32_t step_cnt, uint8_t ble_status, uint8_t am_pm_format,
                          uint8_t charge, uint8_t battery_level, uint8_t language, uint8_t menstrual)
{
    uint8_t background_type = 0;
    
    background_type = main_background_color;
    if(main_background_color >= 5 && main_background_color <= 6)
    {
        if(index == 0)
        {
            LCD_SetPosition(icon_16_main_clock_bg_coord[0][0], icon_16_main_clock_bg_coord[0][1], 
                            ICON_MAIN3_ICON_WIDE, ICON_MAIN3_ICON_HIGH);
            LCD_DriverWriteDataFromExtFlash(icon_16_main_clock_bg_addr[0][main_background_color - 5], ICON_MAIN3_ICON_SIZE);
        }
        return;
    }
    
    if(main_background_color >= 0 && main_background_color <= 1)
    {
    
        if(index == 0)
        {
            Gui_PageMainBackGround();
        }
        /* 绘制蓝牙图标 */
        LCD_SetPosition(icon_16_main_blue_coord[0][0], icon_16_main_blue_coord[0][1],
                        ICON_MAIN1_ICON3_WIDE, ICON_MAIN1_ICON3_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_main_blue_addr[background_type][ble_status % 2], ICON_MAIN1_ICON3_SIZE);
        
        if(index == 0)
        {
            /* 电池图标 */
            LCD_SetPosition(icon_16_main_battery_coord[0][0], icon_16_main_battery_coord[0][1], 
                            ICON_MAIN1_ICON2_WIDE, ICON_MAIN1_ICON2_HIGH);
            LCD_DriverWriteDataFromExtFlash(icon_16_main_battery_addr[background_type][0], ICON_MAIN1_ICON2_SIZE);
        }
        
        /* 充电图标 */
        if(charge == BATTERRY_IS_CHARGING)
        {
            /* 充电中 */
            if(index % 5 == 0)
            {
                LCD_SetPosition(icon_16_main_battery2_coord[0][0], icon_16_main_battery2_coord[0][1],
                                ICON_MAIN2_ICON5_WIDE - 1, ICON_MAIN2_ICON5_HIGH - 2);
                LCD_DriverWriteDataFromExtFlash(icon_16_main_battery2_addr[background_type][0], ICON_MAIN2_ICON5_SIZE);
            }
            else
            {
                Gui_PageRefreshBackground(icon_16_main_battery2_coord[0][0]+4*(index % 5 - 1), icon_16_main_battery2_coord[0][1],
                                         4, ICON_MAIN2_ICON5_HIGH - 2, 0xFFFF);
            }
        }
        else
        {
            if(battery_level == 0)
            {
                if(index % 4 == 1)
                {
                    /* 清除电池图标 */
                    LCD_SetPosition(icon_16_main_battery_coord[0][0], icon_16_main_battery_coord[0][1], 
                                    ICON_MAIN1_ICON2_WIDE, ICON_MAIN1_ICON2_HIGH);
                    LCD_DriverWriteDataFromExtFlash(icon_16_main_battery_addr[2 + background_type][0], ICON_MAIN1_ICON2_SIZE);

                }
                else if(index % 4 == 3)
                {
                    /* 电池图标 */
                    LCD_SetPosition(icon_16_main_battery_coord[0][0], icon_16_main_battery_coord[0][1], 
                                    ICON_MAIN1_ICON2_WIDE, ICON_MAIN1_ICON2_HIGH);
                    LCD_DriverWriteDataFromExtFlash(icon_16_main_battery_addr[background_type][0], ICON_MAIN1_ICON2_SIZE);
                }
            }
            else
            {
                /* 电池电量 */
                if(battery_level % 5 != 0)
                {
                    for(uint8_t i = 0; i < battery_level % 5; i++)
                    {
                        Gui_PageRefreshBackground(icon_16_main_battery2_coord[0][0]+4*i, icon_16_main_battery2_coord[0][1],
                                                  4, ICON_MAIN2_ICON5_HIGH - 2, 0xFFFF);
                    }
                }
                else
                {
                    LCD_SetPosition(icon_16_main_battery2_coord[0][0], icon_16_main_battery2_coord[0][1],
                                    ICON_MAIN2_ICON5_WIDE, ICON_MAIN2_ICON5_HIGH);
                    LCD_DriverWriteDataFromExtFlash(icon_16_main_battery2_addr[background_type][0], ICON_MAIN2_ICON5_SIZE);
                }
            }
        }
        
        /* 绘制AM、PM格式 */
        LCD_SetPosition(icon_16_main_ampm_coord[0][0], icon_16_main_ampm_coord[0][1], 
                        ICON_MAIN1_ICON1_WIDE, ICON_MAIN1_ICON1_HIGH);
        if(am_pm_format == GUI_TIME_FORMAT_AM)
        {
            if(language == 0)
            {
                LCD_DriverWriteDataFromExtFlash(icon_16_main_ampm_addr[background_type][0], ICON_MAIN1_ICON1_SIZE);
            }
            else
            {
                LCD_DriverWriteDataFromExtFlash(icon_16_main_ampm_addr[background_type][2], ICON_MAIN1_ICON1_SIZE);
            }
        }
        else if(am_pm_format == GUI_TIME_FORMAT_PM)
        {
            if(language == 0)
            {
                LCD_DriverWriteDataFromExtFlash(icon_16_main_ampm_addr[background_type][1], ICON_MAIN1_ICON1_SIZE);
            }
            else
            {
                LCD_DriverWriteDataFromExtFlash(icon_16_main_ampm_addr[background_type][3], ICON_MAIN1_ICON1_SIZE);
            }
        }
        else
        { 
            //需要替换
            LCD_DriverWriteDataFromExtFlash(icon_16_main_ampm_addr[2][background_type], ICON_MAIN1_ICON1_SIZE);
        }
        
        //女性图标
         LCD_SetPosition(icon_16_menstrual_icon_coord[0][0], icon_16_menstrual_icon_coord[0][1], 
                        ICON_MAIN1_ICON4_WIDE, ICON_MAIN1_ICON4_HIGH);
         LCD_DriverWriteDataFromExtFlash(icon_16_menstrual_icon_addr[background_type][menstrual % 5], ICON_MAIN1_ICON4_SIZE);
        
        return;
    }
    
    
    if(main_background_color == 2)
    {
        /* 绘制蓝牙图标 */
        LCD_SetPosition(icon_16_main5_ble_icon_coord[0][0], icon_16_main5_ble_icon_coord[0][1],
                        ICON_MAIN5_ICON1_WIDE, ICON_MAIN5_ICON1_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_main5_ble_icon_addr[ble_status % 2], ICON_MAIN5_ICON1_SIZE);
        
        /* 充电图标 */
        if(charge == BATTERRY_IS_CHARGING)
        {
            /* 充电中 */
            LCD_SetPosition(icon_16_main5_battery_coord[0][0], icon_16_main5_battery_coord[0][1],
                            ICON_MAIN5_ICON4_WIDE , ICON_MAIN5_ICON4_HIGH );
            LCD_DriverWriteDataFromExtFlash(icon_16_main5_battery_addr[index % 5], ICON_MAIN5_ICON4_SIZE);
            
        }
        else
        {
            if(battery_level == 0)
            {
                if(index % 4 == 1)
                {
                    /* 清除电池图标 */
                    Gui_PageClearBackground(icon_16_main5_battery_coord[0][0], icon_16_main5_battery_coord[0][1], 
                                   ICON_MAIN5_ICON4_WIDE, ICON_MAIN5_ICON4_HIGH);
                    

                }
                else if(index % 4 == 3)
                {
                    /* 电池图标 */
                    LCD_SetPosition(icon_16_main5_low_power_coord[0][0], icon_16_main5_low_power_coord[0][1],
                            ICON_MAIN5_ICON4_WIDE , ICON_MAIN5_ICON4_HIGH );
                    LCD_DriverWriteDataFromExtFlash(icon_16_main5_low_power_addr[0], ICON_MAIN5_ICON4_SIZE);
                }
            }
            else
            {
                /* 电池电量 */
                if(battery_level % 5 != 0)
                {
                    LCD_SetPosition(icon_16_main5_battery_coord[0][0], icon_16_main5_battery_coord[0][1],
                            ICON_MAIN5_ICON4_WIDE , ICON_MAIN5_ICON4_HIGH );
                    LCD_DriverWriteDataFromExtFlash(icon_16_main5_battery_addr[battery_level], ICON_MAIN5_ICON4_SIZE);
                }
                else
                {
                    LCD_SetPosition(icon_16_main5_battery_coord[0][0], icon_16_main5_battery_coord[0][1],
                                    ICON_MAIN5_ICON4_WIDE, ICON_MAIN5_ICON4_HIGH);
                    LCD_DriverWriteDataFromExtFlash(icon_16_main5_battery_addr[0], ICON_MAIN5_ICON4_SIZE);
                }
            }
        }
        
        /* 绘制AM、PM格式 */
        LCD_SetPosition(icon_16_main5_ampm_icon_coord[0][0], icon_16_main5_ampm_icon_coord[0][1], 
                        ICON_MAIN5_ICON2_WIDE, ICON_MAIN5_ICON2_HIGH);
        if(am_pm_format == GUI_TIME_FORMAT_AM)
        {
            if(language == 0)
            {
                LCD_DriverWriteDataFromExtFlash(icon_16_main5_ampm_icon_addr[0][0], ICON_MAIN5_ICON2_SIZE);
            }
            else
            {
                LCD_DriverWriteDataFromExtFlash(icon_16_main5_ampm_icon_addr[1][0], ICON_MAIN5_ICON2_SIZE);
            }
        }
        else if(am_pm_format == GUI_TIME_FORMAT_PM)
        {
            if(language == 0)
            {
                LCD_DriverWriteDataFromExtFlash(icon_16_main5_ampm_icon_addr[0][1], ICON_MAIN5_ICON2_SIZE);
            }
            else
            {
                LCD_DriverWriteDataFromExtFlash(icon_16_main5_ampm_icon_addr[1][1], ICON_MAIN5_ICON2_SIZE);
            }
        }
        else
        { 
            //需要替换
            Gui_PageClearBackground(icon_16_main5_ampm_icon_coord[0][0], icon_16_main5_ampm_icon_coord[0][1], 
                        ICON_MAIN5_ICON2_WIDE, ICON_MAIN5_ICON2_HIGH);
        }
        
        //女性图标
         LCD_SetPosition(icon_16_main5_menstrual_coord[0][0], icon_16_main5_menstrual_coord[0][1], 
                        ICON_MAIN5_ICON3_WIDE, ICON_MAIN5_ICON3_HIGH);
         LCD_DriverWriteDataFromExtFlash(icon_16_main5_menstrual_addr[menstrual % 5], ICON_MAIN5_ICON3_SIZE);
        
        //计步图标、数字  
        if(index == 0)
        {
            if(step_cnt > 99999)
            {
                step_cnt = 99999;
            }
            
            LCD_SetPosition(icon_16_main5_step_data_coord[0][0], icon_16_main5_step_data_coord[0][1], 
                        ICON_MAIN5_DATA3_WIDE, ICON_MAIN5_DATA3_HIGH);
            LCD_DriverWriteDataFromExtFlash(icon_16_main5_step_data_addr[step_cnt / 10000 % 10], ICON_MAIN5_DATA3_SIZE);
            
            LCD_SetPosition(icon_16_main5_step_data_coord[0][0] + 10, icon_16_main5_step_data_coord[0][1], 
                        ICON_MAIN5_DATA3_WIDE, ICON_MAIN5_DATA3_HIGH);
            LCD_DriverWriteDataFromExtFlash(icon_16_main5_step_data_addr[step_cnt / 1000 % 10], ICON_MAIN5_DATA3_SIZE);
            
            LCD_SetPosition(icon_16_main5_step_data_coord[0][0] + 20, icon_16_main5_step_data_coord[0][1], 
                        ICON_MAIN5_DATA3_WIDE, ICON_MAIN5_DATA3_HIGH);
            LCD_DriverWriteDataFromExtFlash(icon_16_main5_step_data_addr[step_cnt / 100 % 10], ICON_MAIN5_DATA3_SIZE);
            
            LCD_SetPosition(icon_16_main5_step_data_coord[0][0] + 30, icon_16_main5_step_data_coord[0][1], 
                        ICON_MAIN5_DATA3_WIDE, ICON_MAIN5_DATA3_HIGH);
            LCD_DriverWriteDataFromExtFlash(icon_16_main5_step_data_addr[step_cnt / 10 % 10], ICON_MAIN5_DATA3_SIZE);
            
            LCD_SetPosition(icon_16_main5_step_data_coord[0][0] + 40, icon_16_main5_step_data_coord[0][1], 
                        ICON_MAIN5_DATA3_WIDE, ICON_MAIN5_DATA3_HIGH);
            LCD_DriverWriteDataFromExtFlash(icon_16_main5_step_data_addr[step_cnt % 10], ICON_MAIN5_DATA3_SIZE);
            
            
            
            LCD_SetPosition(icon_16_main5_step_icon_coord[0][0], icon_16_main5_step_icon_coord[0][1], 
                        ICON_MAIN5_ICON5_WIDE, ICON_MAIN5_ICON5_HIGH);
            LCD_DriverWriteDataFromExtFlash(icon_16_main5_step_icon_addr[0], ICON_MAIN5_ICON5_SIZE);
        }
        
        
        
        
    }
    else if(main_background_color == 3)
    {
         /* 绘制蓝牙图标 */
        LCD_SetPosition(icon_16_main6_ble_icon_coord[0][0], icon_16_main6_ble_icon_coord[0][1],
                        ICON_MAIN6_ICON1_WIDE, ICON_MAIN6_ICON1_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_main6_ble_icon_addr[ble_status % 2], ICON_MAIN5_ICON1_SIZE);
        
        /* 充电图标 */
        if(charge == BATTERRY_IS_CHARGING)
        {
            /* 充电中 */
            LCD_SetPosition(icon_16_main6_battery_coord[0][0], icon_16_main6_battery_coord[0][1],
                            ICON_MAIN6_ICON4_WIDE , ICON_MAIN6_ICON4_HIGH );
            LCD_DriverWriteDataFromExtFlash(icon_16_main6_battery_addr[index % 5], ICON_MAIN6_ICON4_SIZE);
            
        }
        else
        {
            if(battery_level == 0)
            {
                if(index % 4 == 1)
                {
                    /* 清除电池图标 */
                    Gui_PageClearBackground(icon_16_main6_battery_coord[0][0], icon_16_main6_battery_coord[0][1], 
                                   ICON_MAIN6_ICON4_WIDE, ICON_MAIN6_ICON4_HIGH);
                    

                }
                else if(index % 4 == 3)
                {
                    /* 电池图标 */
                    LCD_SetPosition(icon_16_main6_low_power_coord[0][0], icon_16_main6_low_power_coord[0][1],
                            ICON_MAIN6_ICON4_WIDE , ICON_MAIN6_ICON4_HIGH );
                    LCD_DriverWriteDataFromExtFlash(icon_16_main6_low_power_addr[0], ICON_MAIN6_ICON4_SIZE);
                }
            }
            else
            {
                /* 电池电量 */
                if(battery_level % 5 != 0)
                {
                    LCD_SetPosition(icon_16_main6_battery_coord[0][0], icon_16_main6_battery_coord[0][1],
                            ICON_MAIN6_ICON4_WIDE , ICON_MAIN6_ICON4_HIGH );
                    LCD_DriverWriteDataFromExtFlash(icon_16_main6_battery_addr[battery_level], ICON_MAIN6_ICON4_SIZE);
                }
                else
                {
                    LCD_SetPosition(icon_16_main6_battery_coord[0][0], icon_16_main6_battery_coord[0][1],
                                    ICON_MAIN6_ICON4_WIDE, ICON_MAIN6_ICON4_HIGH);
                    LCD_DriverWriteDataFromExtFlash(icon_16_main6_battery_addr[0], ICON_MAIN6_ICON4_SIZE);
                }
            }
        }
        
        /* 绘制AM、PM格式 */
        LCD_SetPosition(icon_16_main6_ampm_icon_coord[0][0], icon_16_main6_ampm_icon_coord[0][1], 
                        ICON_MAIN6_ICON2_WIDE, ICON_MAIN6_ICON2_HIGH);
        if(am_pm_format == GUI_TIME_FORMAT_AM)
        {
            if(language == 0)
            {
                LCD_DriverWriteDataFromExtFlash(icon_16_main6_ampm_icon_addr[0][0], ICON_MAIN6_ICON2_SIZE);
            }
            else
            {
                LCD_DriverWriteDataFromExtFlash(icon_16_main6_ampm_icon_addr[1][0], ICON_MAIN6_ICON2_SIZE);
            }
        }
        else if(am_pm_format == GUI_TIME_FORMAT_PM)
        {
            if(language == 0)
            {
                LCD_DriverWriteDataFromExtFlash(icon_16_main6_ampm_icon_addr[0][1], ICON_MAIN6_ICON2_SIZE);
            }
            else
            {
                LCD_DriverWriteDataFromExtFlash(icon_16_main6_ampm_icon_addr[1][1], ICON_MAIN6_ICON2_SIZE);
            }
        }
        else
        { 
            //需要替换
            Gui_PageClearBackground(icon_16_main6_ampm_icon_coord[0][0], icon_16_main6_ampm_icon_coord[0][1], 
                        ICON_MAIN6_ICON2_WIDE, ICON_MAIN6_ICON2_HIGH);
        }
        
        //女性图标
         LCD_SetPosition(icon_16_main6_menstrual_coord[0][0], icon_16_main6_menstrual_coord[0][1], 
                        ICON_MAIN6_ICON3_WIDE, ICON_MAIN6_ICON3_HIGH);
         LCD_DriverWriteDataFromExtFlash(icon_16_main6_menstrual_addr[menstrual % 5], ICON_MAIN6_ICON3_SIZE);
    }
    else if(main_background_color == 4)
    {
         /* 绘制蓝牙图标 */
        LCD_SetPosition(icon_16_main7_ble_icon_coord[0][0], icon_16_main7_ble_icon_coord[0][1],
                        ICON_MAIN7_ICON1_WIDE, ICON_MAIN7_ICON1_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_main7_ble_icon_addr[ble_status % 2], ICON_MAIN7_ICON1_SIZE);
        
        /* 充电图标 */
        if(charge == BATTERRY_IS_CHARGING)
        {
            /* 充电中 */
            LCD_SetPosition(icon_16_main7_battery_coord[0][0], icon_16_main7_battery_coord[0][1],
                            ICON_MAIN7_ICON4_WIDE , ICON_MAIN7_ICON4_HIGH );
            LCD_DriverWriteDataFromExtFlash(icon_16_main7_battery_addr[index % 5], ICON_MAIN7_ICON4_SIZE);
            
        }
        else
        {
            if(battery_level == 0)
            {
                if(index % 4 == 1)
                {
                    /* 清除电池图标 */
                    Gui_PageClearBackground(icon_16_main7_battery_coord[0][0], icon_16_main7_battery_coord[0][1], 
                                   ICON_MAIN7_ICON4_WIDE, ICON_MAIN7_ICON4_HIGH);
                    

                }
                else if(index % 4 == 3)
                {
                    /* 电池图标 */
                    LCD_SetPosition(icon_16_main7_low_power_coord[0][0], icon_16_main7_low_power_coord[0][1],
                            ICON_MAIN7_ICON4_WIDE , ICON_MAIN7_ICON4_HIGH );
                    LCD_DriverWriteDataFromExtFlash(icon_16_main7_low_power_addr[0], ICON_MAIN7_ICON4_SIZE);
                }
            }
            else
            {
                /* 电池电量 */
                if(battery_level % 5 != 0)
                {
                    LCD_SetPosition(icon_16_main7_battery_coord[0][0], icon_16_main7_battery_coord[0][1],
                            ICON_MAIN7_ICON4_WIDE , ICON_MAIN7_ICON4_HIGH );
                    LCD_DriverWriteDataFromExtFlash(icon_16_main7_battery_addr[battery_level], ICON_MAIN7_ICON4_SIZE);
                }
                else
                {
                    LCD_SetPosition(icon_16_main7_battery_coord[0][0], icon_16_main7_battery_coord[0][1],
                                    ICON_MAIN7_ICON4_WIDE, ICON_MAIN7_ICON4_HIGH);
                    LCD_DriverWriteDataFromExtFlash(icon_16_main7_battery_addr[0], ICON_MAIN7_ICON4_SIZE);
                }
            }
        }
        
        /* 绘制AM、PM格式 */
        LCD_SetPosition(icon_16_main7_ampm_icon_coord[0][0], icon_16_main7_ampm_icon_coord[0][1], 
                        ICON_MAIN7_ICON2_WIDE, ICON_MAIN7_ICON2_HIGH);
        if(am_pm_format == GUI_TIME_FORMAT_AM)
        {
            if(language == 0)
            {
                LCD_DriverWriteDataFromExtFlash(icon_16_main7_ampm_icon_addr[0][0], ICON_MAIN7_ICON2_SIZE);
            }
            else
            {
                LCD_DriverWriteDataFromExtFlash(icon_16_main7_ampm_icon_addr[1][0], ICON_MAIN7_ICON2_SIZE);
            }
        }
        else if(am_pm_format == GUI_TIME_FORMAT_PM)
        {
            if(language == 0)
            {
                LCD_DriverWriteDataFromExtFlash(icon_16_main7_ampm_icon_addr[0][1], ICON_MAIN7_ICON2_SIZE);
            }
            else
            {
                LCD_DriverWriteDataFromExtFlash(icon_16_main7_ampm_icon_addr[1][1], ICON_MAIN7_ICON2_SIZE);
            }
        }
        else
        { 
            //需要替换
            Gui_PageClearBackground(icon_16_main7_ampm_icon_coord[0][0], icon_16_main7_ampm_icon_coord[0][1], 
                        ICON_MAIN7_ICON2_WIDE, ICON_MAIN7_ICON2_HIGH);
        }
        
        //女性图标
         LCD_SetPosition(icon_16_main7_menstrual_coord[0][0], icon_16_main7_menstrual_coord[0][1], 
                        ICON_MAIN7_ICON3_WIDE, ICON_MAIN7_ICON3_HIGH);
         LCD_DriverWriteDataFromExtFlash(icon_16_main7_menstrual_addr[menstrual % 5], ICON_MAIN7_ICON3_SIZE);
    }
}

/****************************************************************
** Function name:           Gui_PageStep
** Descriptions:            显示计步界面
** input parameters:        index：动态图标的计数
**                          step_cnt：步数
**                          target_cnt：目标
                            language: 语言
** output parameters:       无
** Returned value:          无
** Created by:              YX              
** Created Date:            2017-08-07
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/  
void Gui_PageStep(uint32_t step_cnt, uint8_t target_cnt, uint8_t index, uint8_t language)
{
    uint8_t start_column = 0;
    uint8_t start_page = 0;
    uint32_t step_cnt_temp = 0;
    
    step_cnt_temp = step_cnt;
    if(step_cnt > 999999)
    {
        step_cnt_temp = 999999;
    }

    /* 卡路里动态图标 */
    LCD_SetPosition(icon_16_step_icon_coord[0][0], icon_16_step_icon_coord[0][1], 
                    ICON_STEP_ICON_WIDE, ICON_STEP_ICON_HIGH);
    LCD_DriverWriteDataFromExtFlash(icon_16_step_icon_addr[index % 3], ICON_STEP_ICON_SIZE);
    
    /* 描绘步数 */    
    start_page = icon_16_step_data_coord[0][1];
    start_column = icon_16_step_data_coord[0][0];
    LCD_SetPosition(start_column, start_page, ICON_FUNCTION_DATA_WIDE, ICON_FUNCTION_DATA_HIGH);
    if(step_cnt_temp >= 100000)
    {    
        LCD_DriverWriteDataFromExtFlash(icon_16_step_data_addr[step_cnt_temp / 100000], ICON_FUNCTION_DATA_SIZE);        
    }  
    else
    {    
        LCD_DriverWriteDataFromExtFlash(icon_16_function1_data_addr[0], ICON_FUNCTION_DATA_SIZE);        
    }
    
    start_column += ICON_FUNCTION_DATA_WIDE + 2;
    LCD_SetPosition(start_column, start_page, ICON_FUNCTION_DATA_WIDE, ICON_FUNCTION_DATA_HIGH);
    if(step_cnt_temp >= 10000)
    {    
        LCD_DriverWriteDataFromExtFlash(icon_16_step_data_addr[(step_cnt_temp / 10000) %10], ICON_FUNCTION_DATA_SIZE);        
    }  
    else
    {    
        LCD_DriverWriteDataFromExtFlash(icon_16_function1_data_addr[0], ICON_FUNCTION_DATA_SIZE);        
    } 
    
    start_column += ICON_FUNCTION_DATA_WIDE + 2;
    LCD_SetPosition(start_column, start_page, ICON_FUNCTION_DATA_WIDE, ICON_FUNCTION_DATA_HIGH);
    if(step_cnt_temp >= 1000)
    {    
        LCD_DriverWriteDataFromExtFlash(icon_16_step_data_addr[(step_cnt_temp / 1000) %10], ICON_FUNCTION_DATA_SIZE);        
    }  
    else
    {    
        LCD_DriverWriteDataFromExtFlash(icon_16_function1_data_addr[0], ICON_FUNCTION_DATA_SIZE);        
    }
    //第二行
    start_page = icon_16_step_data_coord[1][1];
    start_column = icon_16_step_data_coord[1][0];
    LCD_SetPosition(start_column, start_page, ICON_FUNCTION_DATA_WIDE, ICON_FUNCTION_DATA_HIGH);
    if(step_cnt_temp >= 100)
    {    
        LCD_DriverWriteDataFromExtFlash(icon_16_step_data_addr[(step_cnt_temp / 100) %10], ICON_FUNCTION_DATA_SIZE);        
    }  
    else
    {    
        LCD_DriverWriteDataFromExtFlash(icon_16_function1_data_addr[0], ICON_FUNCTION_DATA_SIZE);        
    }
    start_column += ICON_FUNCTION_DATA_WIDE + 2;
    LCD_SetPosition(start_column, start_page, ICON_FUNCTION_DATA_WIDE, ICON_FUNCTION_DATA_HIGH);
    if(step_cnt_temp >= 10)
    {    
        LCD_DriverWriteDataFromExtFlash(icon_16_step_data_addr[(step_cnt_temp / 10) %10], ICON_FUNCTION_DATA_SIZE);        
    }  
    else
    {    
        LCD_DriverWriteDataFromExtFlash(icon_16_function1_data_addr[0], ICON_FUNCTION_DATA_SIZE);        
    }
    
    start_column += ICON_FUNCTION_DATA_WIDE + 2;
    LCD_SetPosition(start_column, start_page, ICON_FUNCTION_DATA_WIDE, ICON_FUNCTION_DATA_HIGH);
    if(step_cnt_temp > 0)
    {    
        LCD_DriverWriteDataFromExtFlash(icon_16_step_data_addr[step_cnt_temp %10], ICON_FUNCTION_DATA_SIZE);        
    }  
    else
    {    
        LCD_DriverWriteDataFromExtFlash(icon_16_function1_data_addr[0], ICON_FUNCTION_DATA_SIZE);        
    }
    /* 显示图标 */
    LCD_SetPosition(icon_16_step_unit_coord[0][0], icon_16_step_unit_coord[0][1], 
                    ICON_STEP_UNIT_WIDE, ICON_STEP_UNIT_HIGH);
    LCD_DriverWriteDataFromExtFlash(icon_16_step_unit_addr[language ], ICON_STEP_UNIT_SIZE);
}

/****************************************************************
** Function name:           Gui_PageDistance
** Descriptions:            显示里程界面
** input parameters:        index：动态图标的计数
**                          dis_count：里程数据
                            uint_type: 单位类型，km\mi
                            language: 语言，0中文、1英文
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
    uint8_t start_column = 0;
    uint8_t start_page = 0;
    uint16_t dis_count_tmp = 0;
    
	/* 里程动态图标 */
    LCD_SetPosition(icon_16_distance_static_coord[0][0], icon_16_distance_static_coord[0][1], 
                    ICON_DISTANCE_ICON1_WIDE, ICON_DISTANCE_ICON1_HIGH);
    LCD_DriverWriteDataFromExtFlash(icon_16_distance_static_addr[index % 2], ICON_DISTANCE_ICON1_SIZE);

    /* 显示距离数据 */
    dis_count_tmp = dis_count;
    if(dis_count > 999)
    {
        dis_count_tmp = 999;
    }
    if(dis_count_tmp < 100)
    {
        dis_count_tmp = dis_count * 10;
    }
    
    start_page = icon_16_distance_data_coord[0][1];
    start_column = icon_16_distance_data_coord[0][0];
    LCD_SetPosition(start_column, start_page, ICON_FUNCTION_DATA_WIDE, ICON_FUNCTION_DATA_HIGH);
    if(dis_count_tmp >= 100)
    {    
        LCD_DriverWriteDataFromExtFlash(icon_16_distance_data_addr[(dis_count_tmp / 100) %10], ICON_FUNCTION_DATA_SIZE);        
    }  
    else
    {    
        LCD_DriverWriteDataFromExtFlash(icon_16_function1_data_addr[0], ICON_FUNCTION_DATA_SIZE);        
    }
    start_column += ICON_FUNCTION_DATA_WIDE + 2;
    LCD_SetPosition(start_column, start_page, ICON_FUNCTION_DATA_WIDE, ICON_FUNCTION_DATA_HIGH);
    if(dis_count_tmp >= 10)
    {    
        LCD_DriverWriteDataFromExtFlash(icon_16_distance_data_addr[(dis_count_tmp / 10) %10], ICON_FUNCTION_DATA_SIZE);        
    }  
    else
    {    
        LCD_DriverWriteDataFromExtFlash(icon_16_function1_data_addr[0], ICON_FUNCTION_DATA_SIZE);        
    }
    
    start_column += ICON_FUNCTION_DATA_WIDE + 2;
    LCD_SetPosition(start_column, start_page, ICON_FUNCTION_DATA_WIDE, ICON_FUNCTION_DATA_HIGH);
    if(dis_count_tmp > 0)
    {    
        LCD_DriverWriteDataFromExtFlash(icon_16_distance_data_addr[dis_count_tmp %10], ICON_FUNCTION_DATA_SIZE);        
    }  
    else
    {    
        LCD_DriverWriteDataFromExtFlash(icon_16_function1_data_addr[0], ICON_FUNCTION_DATA_SIZE);        
    }
    //小数点
    if(dis_count < 100)
    {
        LCD_SetPosition(icon_16_distance_point_coord[0][0], icon_16_distance_point_coord[0][1], ICON_DISTANCE_ICON3_WIDE, ICON_DISTANCE_ICON3_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_distance_point_addr[0], ICON_DISTANCE_ICON3_SIZE);        
    }
    else
    {
        Gui_PageClearBackground(icon_16_distance_point_coord[0][0], icon_16_distance_point_coord[0][1], ICON_DISTANCE_ICON3_WIDE, ICON_DISTANCE_ICON3_HIGH);
    }
    
    /* 距离单位 */
    if(language == 0)
    {
        LCD_SetPosition(icon_16_distance_unit_coord[0][0], icon_16_distance_unit_coord[0][1], 
                        ICON_DISTANCE_UNIT_WIDE, ICON_DISTANCE_UNIT_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_distance_unit_addr[0], ICON_DISTANCE_UNIT_SIZE);
    }
    else if(uint_type == 1)
    {
        LCD_SetPosition(icon_16_distance_unit_coord[0][0], icon_16_distance_unit_coord[0][1], 
                        ICON_DISTANCE_UNIT_WIDE, ICON_DISTANCE_UNIT_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_distance_unit_addr[2], ICON_DISTANCE_UNIT_SIZE);
    }
    
    else
    {
        LCD_SetPosition(icon_16_distance_unit_coord[0][0], icon_16_distance_unit_coord[0][1], 
                        ICON_DISTANCE_UNIT_WIDE, ICON_DISTANCE_UNIT_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_distance_unit_addr[1], ICON_DISTANCE_UNIT_SIZE);
    }
}

/****************************************************************
** Function name:           Gui_PageCalories
** Descriptions:            显示卡路里
** input parameters:        index：动态图标的计数
                            calorie：卡路里（大卡）
                            language: 语言，0中文、1英文
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
    uint8_t calorie_column = 0;
    uint8_t calorie_page = 0;
    uint32_t calorie_tmp = 0;

    /* 卡路里动态图标 */
    LCD_SetPosition(icon_16_calories_icon_coord[0][0], icon_16_calories_icon_coord[0][1], 
                    ICON_CALORIES_ICON_WIDE, ICON_CALORIES_ICON_HIGH);
    LCD_DriverWriteDataFromExtFlash(icon_16_calories_icon_addr[index % 3], ICON_CALORIES_ICON_SIZE);


    /* 显示卡路里数据 */
    calorie_tmp = calorie;
    if(calorie_tmp > 999999)
    {
        calorie_tmp = 999999;
    }

    calorie_page = icon_16_calories_data_coord[0][1];
    calorie_column = icon_16_calories_data_coord[0][0];
    LCD_SetPosition(calorie_column, calorie_page, ICON_FUNCTION_DATA_WIDE, ICON_FUNCTION_DATA_HIGH);
    if(calorie_tmp >= 100000)
    {    
        LCD_DriverWriteDataFromExtFlash(icon_16_calories_data_addr[calorie_tmp / 100000], ICON_FUNCTION_DATA_SIZE);        
    }  
    else
    {    
        LCD_DriverWriteDataFromExtFlash(icon_16_function1_data_addr[0], ICON_FUNCTION_DATA_SIZE);        
    }
    
    calorie_column += ICON_FUNCTION_DATA_WIDE + 2;
    LCD_SetPosition(calorie_column, calorie_page, ICON_FUNCTION_DATA_WIDE, ICON_FUNCTION_DATA_HIGH);
    if(calorie_tmp >= 10000)
    {    
        LCD_DriverWriteDataFromExtFlash(icon_16_calories_data_addr[(calorie_tmp / 10000) %10], ICON_FUNCTION_DATA_SIZE);        
    }  
    else
    {    
        LCD_DriverWriteDataFromExtFlash(icon_16_function1_data_addr[0], ICON_FUNCTION_DATA_SIZE);        
    } 
    
    calorie_column += ICON_FUNCTION_DATA_WIDE + 2;
    LCD_SetPosition(calorie_column, calorie_page, ICON_FUNCTION_DATA_WIDE, ICON_FUNCTION_DATA_HIGH);
    if(calorie_tmp >= 1000)
    {    
        LCD_DriverWriteDataFromExtFlash(icon_16_calories_data_addr[(calorie_tmp / 1000) %10], ICON_FUNCTION_DATA_SIZE);        
    }  
    else
    {    
        LCD_DriverWriteDataFromExtFlash(icon_16_function1_data_addr[0], ICON_FUNCTION_DATA_SIZE);        
    }
    //第二行
    calorie_page = icon_16_calories_data_coord[1][1];
    calorie_column = icon_16_calories_data_coord[1][0];
    LCD_SetPosition(calorie_column, calorie_page, ICON_FUNCTION_DATA_WIDE, ICON_FUNCTION_DATA_HIGH);
    if(calorie_tmp >= 100)
    {    
        LCD_DriverWriteDataFromExtFlash(icon_16_calories_data_addr[(calorie_tmp / 100) %10], ICON_FUNCTION_DATA_SIZE);        
    }  
    else
    {    
        LCD_DriverWriteDataFromExtFlash(icon_16_function1_data_addr[0], ICON_FUNCTION_DATA_SIZE);        
    }
    calorie_column += ICON_FUNCTION_DATA_WIDE + 2;
    LCD_SetPosition(calorie_column, calorie_page, ICON_FUNCTION_DATA_WIDE, ICON_FUNCTION_DATA_HIGH);
    if(calorie_tmp >= 10)
    {    
        LCD_DriverWriteDataFromExtFlash(icon_16_calories_data_addr[(calorie_tmp / 10) %10], ICON_FUNCTION_DATA_SIZE);        
    }  
    else
    {    
        LCD_DriverWriteDataFromExtFlash(icon_16_function1_data_addr[0], ICON_FUNCTION_DATA_SIZE);        
    }
    
    calorie_column += ICON_FUNCTION_DATA_WIDE + 2;
    LCD_SetPosition(calorie_column, calorie_page, ICON_FUNCTION_DATA_WIDE, ICON_FUNCTION_DATA_HIGH);
    if(calorie_tmp > 0)
    {    
        LCD_DriverWriteDataFromExtFlash(icon_16_calories_data_addr[calorie_tmp %10], ICON_FUNCTION_DATA_SIZE);        
    }  
    else
    {    
        LCD_DriverWriteDataFromExtFlash(icon_16_function1_data_addr[0], ICON_FUNCTION_DATA_SIZE);        
    }
    /* 距离单位 */
    LCD_SetPosition(icon_16_calories_unit_coord[0][0], icon_16_calories_unit_coord[0][1], 
                    ICON_CALORIES_UNIT_WIDE, ICON_CALORIES_UNIT_HIGH);
    LCD_DriverWriteDataFromExtFlash(icon_16_calories_unit_addr[language], ICON_CALORIES_UNIT_SIZE);
}

/****************************************************************
** Function name:           Gui_PageRunMode
** Descriptions:            显示运动模式
** input parameters:        index：动态图标的计数
                            language: 语言，0中文、1英文
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
    //运动模式主界面    
    //人
    LCD_SetPosition(icon_16_move1_icon_coord[0][0], icon_16_move1_icon_coord[0][1],
                    ICON_MOVE1_ICON_WIDE, ICON_MOVE1_ICON_HIGH);
    LCD_DriverWriteDataFromExtFlash(icon_16_move1_icon_addr[index % 10], ICON_MOVE1_ICON_SIZE);
    
    LCD_SetPosition(icon_16_move1_icon2_coord[0][0], icon_16_move1_icon2_coord[0][1],
                    ICON_MOVE1_ICON2_WIDE, ICON_MOVE1_ICON2_HIGH);
    LCD_DriverWriteDataFromExtFlash(icon_16_move1_icon2_addr[0], ICON_MOVE1_ICON2_SIZE);

}

/****************************************************************
** Function name:           Gui_PageRunStaticIcon
** Descriptions:            显示运动模式的静态图标
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
    for(uint8_t i = 0; i < 3; i++)
    {
        /* 图标 */
        LCD_SetPosition(icon_16_move_icon2_coord[i][0], icon_16_move_icon2_coord[i][1], 
                        ICON_MOVE2_ICON3_WIDE, ICON_MOVE2_ICON3_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_move_icon2_addr[(i >= 1) ? 0 : 1], ICON_MOVE2_ICON3_SIZE);
    } 
    
    for(uint8_t i = 0; i < 2; i++)
    {
        /* 时间的冒号 */
        LCD_SetPosition(icon_16_move2_icon2_colon_coord[i][0], icon_16_move2_icon2_colon_coord[i][1], 
                        ICON_MOVE2_ICON_WIDE, ICON_MOVE2_ICON_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_move_icon_colon_addr[0], ICON_MOVE2_ICON_SIZE);  
    }
         
    for(uint8_t i = 0; i < 3; i++)
    {
        /* 分割线 */
        LCD_SetPosition(icon_16_move2_icon2_line_coord[i][0], icon_16_move2_icon2_line_coord[i][1], 
                        ICON_MOVE2_ICON2_WIDE, ICON_MOVE2_ICON2_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_move2_icon2_line_addr[0], ICON_MOVE2_ICON2_SIZE);
    }
    
    for(uint8_t i = 0; i < 3; i++)
    {
        /* 图标 */
        LCD_SetPosition(icon_16_move2_icon1_coord[i][0], icon_16_move2_icon1_coord[i][1], 
                        ICON_MOVE2_ICON1_WIDE, ICON_MOVE2_ICON1_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_move2_icon1_addr[i], ICON_MOVE2_ICON1_SIZE);
    } 
}
/****************************************************************
** Function name:           Gui_UpdateRunModeTime
** Descriptions:            显示运动模式数据界面的时间
** input parameters:        seconds：运动时间
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
    
    hour   = seconds / 60 / 60;
    minute = (seconds / 60 ) % 60;
    second = seconds % 60;
    timing_data[0] = (hour % 100) / 10;
    timing_data[1] = hour % 10;
    timing_data[2] = (minute % 100) / 10;
    timing_data[3] = minute % 10;
    timing_data[4] = (second % 100) / 10;
    timing_data[5] = second % 10;
    
    for(uint8_t i = 0; i < 6; i++)
    {
        /* 显示计数器的数字 */
        LCD_SetPosition(icon_16_move2_data2_coord[i][0], icon_16_move2_data2_coord[i][1], 
                        ICON_MOVE2_DATA1_WIDE, ICON_MOVE2_DATA1_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_move2_time_data_addr[timing_data[i]], ICON_MOVE2_DATA1_SIZE);
    }
}

/****************************************************************
** Function name:           Gui_UpdateRunModeHeart
** Descriptions:            显示运动模式数据界面的心率
** input parameters:        heart：心率
** output parameters:       
** Returned value:          
** Created by:              WJ              
** Created Date:            2017-08-11
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
static void Gui_UpdateRunModeHeart(uint8_t heart)    
{
    uint8_t start_column = 0;
    uint8_t start_page = 0;
    uint8_t heart_tmp = 0;
    
    heart_tmp = heart;
    start_page = icon_16_move2_data_coord[0][1];
    start_column = icon_16_move2_data_coord[0][0];
    
    LCD_SetPosition(start_column, start_page, 
                    ICON_MOVE2_DATA1_WIDE, ICON_MOVE2_DATA1_HIGH);
    LCD_DriverWriteDataFromExtFlash(icon_16_move2_time_data_addr[0], ICON_MOVE2_DATA1_SIZE);
    start_column += ICON_MOVE2_DATA1_WIDE;
    LCD_SetPosition(start_column, start_page, 
                    ICON_MOVE2_DATA1_WIDE, ICON_MOVE2_DATA1_HIGH);
    LCD_DriverWriteDataFromExtFlash(icon_16_move2_time_data_addr[0], ICON_MOVE2_DATA1_SIZE);
    start_column += ICON_MOVE2_DATA1_WIDE;
    
    LCD_SetPosition(start_column, start_page, ICON_MOVE2_DATA2_WIDE, ICON_MOVE2_DATA2_HIGH);
    if(heart_tmp >= 100)
    {    
        LCD_DriverWriteDataFromExtFlash(icon_16_move2_heart_data_addr[(heart_tmp / 100) %10], ICON_MOVE2_DATA1_SIZE);        
    }  
    else
    {    
        LCD_DriverWriteDataFromExtFlash(icon_16_move2_time_data_addr[0], ICON_MOVE2_DATA1_SIZE);       
    }
    start_column += ICON_MOVE2_DATA2_WIDE;
    LCD_SetPosition(start_column, start_page, ICON_MOVE2_DATA2_WIDE, ICON_MOVE2_DATA2_HIGH);
    if(heart_tmp >= 10)
    {    
        LCD_DriverWriteDataFromExtFlash(icon_16_move2_heart_data_addr[(heart_tmp / 10) %10], ICON_MOVE2_DATA1_SIZE);        
    }  
    else
    {    
        LCD_DriverWriteDataFromExtFlash(icon_16_move2_time_data_addr[0], ICON_MOVE2_DATA1_SIZE);         
    }
    
    start_column += ICON_MOVE2_DATA2_WIDE;
    LCD_SetPosition(start_column, start_page, ICON_MOVE2_DATA2_WIDE, ICON_MOVE2_DATA2_HIGH);
    if(heart > 0)
    {    
        LCD_DriverWriteDataFromExtFlash(icon_16_move2_heart_data_addr[heart_tmp %10], ICON_MOVE2_DATA1_SIZE);        
    }  
    else
    {    
        LCD_DriverWriteDataFromExtFlash(icon_16_move2_time_data_addr[0], ICON_MOVE2_DATA1_SIZE);         
    }         
}

/****************************************************************
** Function name:           Gui_UpdateRunModeDistance
** Descriptions:            显示运动模式数据界面的距离
** input parameters:        distance：距离
                            uint_type：0,km；1：mi
                            language: 语言，0中文、1英文
** output parameters:       
** Returned value:          
** Created by:              WJ              
** Created Date:            2017-08-11
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
/*static void Gui_UpdateRunModeDistance(float distance, uint8_t uint_type, uint8_t language)    
{
    uint8_t  stop_xcood = 0;
    uint8_t  start_column = 0;
    uint8_t  start_page = 0;  
    uint16_t distance_temp = 0;
    
    start_column = 2;
    start_page = icon_16_move_data_coord[1][1];
    if(distance > 999)
    {
        distance_temp = 999;
    }
    else if(distance >= 100)
    {
        distance_temp = distance;
    }
    else
    {
        distance_temp = distance * 10;
    }
    
    start_column = 21;
    if(distance_temp > 99)
    {
        LCD_SetPosition(start_column, start_page, 
                       ICON_FUNCTION1_DATA_WIDE, ICON_FUNCTION1_DATA_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_function1_data_addr[(distance_temp % 1000) / 100], ICON_FUNCTION1_DATA_SIZE);  
        start_column += ICON_FUNCTION1_DATA_WIDE;
        Gui_PageClearBackground(start_column, start_page, 1, ICON_FUNCTION1_DATA_HIGH);
        start_column += 1;
    }
    
    if(distance_temp > 9)
    {
        LCD_SetPosition(start_column, start_page, 
                       ICON_FUNCTION1_DATA_WIDE, ICON_FUNCTION1_DATA_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_function1_data_addr[(distance_temp % 100) / 10], ICON_FUNCTION1_DATA_SIZE);  
        start_column += ICON_FUNCTION1_DATA_WIDE;
        Gui_PageClearBackground(start_column, start_page, 1, ICON_FUNCTION1_DATA_HIGH);
        start_column += 1;
    }  
    if(distance < 1)
    {
        LCD_SetPosition(start_column, start_page, 
                        ICON_FUNCTION1_DATA_WIDE, ICON_FUNCTION1_DATA_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_function1_data_addr[0], ICON_FUNCTION1_DATA_SIZE);
        start_column += ICON_FUNCTION1_DATA_WIDE;
        Gui_PageClearBackground(start_column, start_page, 2, ICON_FUNCTION1_DATA_HIGH);
        start_column += 2;        
    }
    //显示小数点
    if(distance < 100)
    {
        Gui_PageClearBackground(start_column, start_page, 1, ICON_FUNCTION1_DATA_HIGH);
        start_column += 1;
        LCD_SetPosition(start_column, icon_16_move_data_point_coord[0][1], 
                       ICON_MOVE_DATA2_WIDE, ICON_MOVE_DATA2_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_move_data_point_addr[0], ICON_MOVE_DATA2_SIZE); 
        Gui_PageClearBackground(start_column, start_page, ICON_MOVE_DATA2_WIDE, ICON_FUNCTION1_DATA_HIGH - ICON_MOVE_DATA2_HIGH);
        start_column += ICON_MOVE_DATA2_WIDE;
        Gui_PageClearBackground(start_column, start_page, 2, ICON_FUNCTION1_DATA_HIGH);
        start_column += 2;        
    }
    
    LCD_SetPosition(start_column, start_page, 
                    ICON_FUNCTION1_DATA_WIDE, ICON_FUNCTION1_DATA_HIGH);
    LCD_DriverWriteDataFromExtFlash(icon_16_function1_data_addr[distance_temp % 10], ICON_FUNCTION1_DATA_SIZE);
    start_column += ICON_FUNCTION1_DATA_WIDE;
    Gui_PageClearBackground(start_column, start_page, 2, ICON_FUNCTION1_DATA_HIGH);
    start_column += 2;
    
    //单位   
    LCD_SetPosition(start_column, icon_16_move_distance_unit_coord[0][1],
                    ICON_MOVE_DISTANCE2_WIDE, ICON_MOVE_DISTANCE2_HIGH);
    if(language == 0)
    {
        LCD_DriverWriteDataFromExtFlash(icon_16_move_distance_unit_addr[0], ICON_MOVE_DISTANCE2_SIZE);        
    }
    else
    {
        if(uint_type == 0)
        {
            LCD_DriverWriteDataFromExtFlash(icon_16_move_distance_unit_addr[1], ICON_MOVE_DISTANCE2_SIZE);
        }   
        else
        {
            LCD_DriverWriteDataFromExtFlash(icon_16_move_distance_unit_addr[2], ICON_MOVE_DISTANCE2_SIZE);
        }
    }
    Gui_PageClearBackground(start_column, start_page, ICON_MOVE_DISTANCE2_WIDE, 1);
    stop_xcood = start_column + ICON_MOVE_DISTANCE2_WIDE;
    Gui_PageClearBackground(stop_xcood, start_page, LCD_MAX_COLUMN - stop_xcood, ICON_FUNCTION1_DATA_HIGH);
       
}*/

/****************************************************************
** Function name:           Gui_UpdateRunModeCalories
** Descriptions:            显示运动模式数据界面的卡路里
** input parameters:        calories：卡路里
                            language: 语言，0中文、1英文
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
//    uint8_t  stop_xcood = 0;
    uint8_t  start_column = 0;
    uint8_t  start_page = 0;  
    uint32_t calories_temp = 0;
    
    if(calories > 99999)
    {
        calories_temp = 99999;
    }
    else
    {
        calories_temp = calories;
    }
    
    start_page = icon_16_move2_data_coord[1][1];
    start_column = icon_16_move2_data_coord[1][0];
   
    LCD_SetPosition(start_column, start_page, ICON_MOVE2_DATA2_WIDE, ICON_MOVE2_DATA2_HIGH);
    if(calories_temp >= 10000)
    {    
        LCD_DriverWriteDataFromExtFlash(icon_16_move2_calories_data_addr[(calories_temp / 10000) %10], ICON_MOVE2_DATA1_SIZE);        
    }  
    else
    {    
        LCD_DriverWriteDataFromExtFlash(icon_16_move2_time_data_addr[0], ICON_MOVE2_DATA1_SIZE);       
    }
    start_column += ICON_MOVE2_DATA2_WIDE;
    LCD_SetPosition(start_column, start_page, ICON_MOVE2_DATA2_WIDE, ICON_MOVE2_DATA2_HIGH);
    if(calories_temp >= 1000)
    {    
        LCD_DriverWriteDataFromExtFlash(icon_16_move2_calories_data_addr[(calories_temp / 1000) %10], ICON_MOVE2_DATA1_SIZE);        
    }  
    else
    {    
        LCD_DriverWriteDataFromExtFlash(icon_16_move2_time_data_addr[0], ICON_MOVE2_DATA1_SIZE);       
    }
    start_column += ICON_MOVE2_DATA2_WIDE;
    LCD_SetPosition(start_column, start_page, ICON_MOVE2_DATA2_WIDE, ICON_MOVE2_DATA2_HIGH);
    if(calories_temp >= 100)
    {    
        LCD_DriverWriteDataFromExtFlash(icon_16_move2_calories_data_addr[(calories_temp / 100) %10], ICON_MOVE2_DATA1_SIZE);        
    }  
    else
    {    
        LCD_DriverWriteDataFromExtFlash(icon_16_move2_time_data_addr[0], ICON_MOVE2_DATA1_SIZE);       
    }
    start_column += ICON_MOVE2_DATA2_WIDE;
    LCD_SetPosition(start_column, start_page, ICON_MOVE2_DATA2_WIDE, ICON_MOVE2_DATA2_HIGH);
    if(calories_temp >= 10)
    {    
        LCD_DriverWriteDataFromExtFlash(icon_16_move2_calories_data_addr[(calories_temp / 10) %10], ICON_MOVE2_DATA1_SIZE);        
    }  
    else
    {    
        LCD_DriverWriteDataFromExtFlash(icon_16_move2_time_data_addr[0], ICON_MOVE2_DATA1_SIZE);         
    }
    start_column += ICON_MOVE2_DATA2_WIDE;
    LCD_SetPosition(start_column, start_page, ICON_MOVE2_DATA2_WIDE, ICON_MOVE2_DATA2_HIGH);
    if(calories_temp > 0)
    {    
        LCD_DriverWriteDataFromExtFlash(icon_16_move2_calories_data_addr[calories_temp %10], ICON_MOVE2_DATA1_SIZE);        
    }  
    else
    {    
        LCD_DriverWriteDataFromExtFlash(icon_16_move2_time_data_addr[0], ICON_MOVE2_DATA1_SIZE);         
    }    
}

/****************************************************************
** Function name:           Gui_UpdateRunModeStep
** Descriptions:            显示运动模式数据界面的计步
** input parameters:        step：计步数
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
    uint8_t  start_column = 0;
    uint8_t  start_page = 0;  
    uint32_t step_temp = 0;
    
    start_page = icon_16_move2_data_coord[2][1];
    step_temp = step;
    if(step_temp > 99999)
    {
        step_temp = 99999;
    }
   
    start_page = icon_16_move2_data_coord[2][1];
    start_column = icon_16_move2_data_coord[2][0];
    
    LCD_SetPosition(start_column, start_page, ICON_MOVE2_DATA2_WIDE, ICON_MOVE2_DATA2_HIGH);   
    if(step_temp >= 10000)
    {    
        LCD_DriverWriteDataFromExtFlash(icon_16_move2_step_data_addr[(step_temp / 10000) %10], ICON_MOVE2_DATA1_SIZE);        
    }  
    else
    {    
        LCD_DriverWriteDataFromExtFlash(icon_16_move2_time_data_addr[0], ICON_MOVE2_DATA1_SIZE);       
    }
    start_column += ICON_MOVE2_DATA2_WIDE;
    LCD_SetPosition(start_column, start_page, ICON_MOVE2_DATA2_WIDE, ICON_MOVE2_DATA2_HIGH);
    if(step_temp >= 1000)
    {    
        LCD_DriverWriteDataFromExtFlash(icon_16_move2_step_data_addr[(step_temp / 1000) %10], ICON_MOVE2_DATA1_SIZE);        
    }  
    else
    {    
        LCD_DriverWriteDataFromExtFlash(icon_16_move2_time_data_addr[0], ICON_MOVE2_DATA1_SIZE);       
    }
    start_column += ICON_MOVE2_DATA2_WIDE;
    LCD_SetPosition(start_column, start_page, ICON_MOVE2_DATA2_WIDE, ICON_MOVE2_DATA2_HIGH);
    if(step_temp >= 100)
    {    
        LCD_DriverWriteDataFromExtFlash(icon_16_move2_step_data_addr[(step_temp / 100) %10], ICON_MOVE2_DATA1_SIZE);        
    }  
    else
    {    
        LCD_DriverWriteDataFromExtFlash(icon_16_move2_time_data_addr[0], ICON_MOVE2_DATA1_SIZE);       
    }
    start_column += ICON_MOVE2_DATA2_WIDE;
    LCD_SetPosition(start_column, start_page, ICON_MOVE2_DATA2_WIDE, ICON_MOVE2_DATA2_HIGH);
    if(step_temp >= 10)
    {    
        LCD_DriverWriteDataFromExtFlash(icon_16_move2_step_data_addr[(step_temp / 10) %10], ICON_MOVE2_DATA1_SIZE);        
    }  
    else
    {    
        LCD_DriverWriteDataFromExtFlash(icon_16_move2_time_data_addr[0], ICON_MOVE2_DATA1_SIZE);         
    }
    start_column += ICON_MOVE2_DATA2_WIDE;
    LCD_SetPosition(start_column, start_page, ICON_MOVE2_DATA2_WIDE, ICON_MOVE2_DATA2_HIGH);
    if(step_temp > 0)
    {    
        LCD_DriverWriteDataFromExtFlash(icon_16_move2_step_data_addr[step_temp %10], ICON_MOVE2_DATA1_SIZE);        
    }  
    else
    {    
        LCD_DriverWriteDataFromExtFlash(icon_16_move2_time_data_addr[0], ICON_MOVE2_DATA1_SIZE);         
    }
}

/****************************************************************
** Function name:           Gui_PageRunModeData
** Descriptions:            显示运动模式数据界面
** input parameters:        seconds：运动时间；heart：心率；calories：卡路里；
                            distance：距离；step：计步数
                            uint_type：0,km；1：mi
                            language: 语言，0中文、1英文
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

    //静态图标
    Gui_PageRunStaticIcon();
	// 时间
    Gui_UpdateRunModeTime(seconds);
	//心率
    Gui_UpdateRunModeHeart(heart);	    
//    //距离
//    Gui_UpdateRunModeDistance(distance, uint_type, language);   
    //卡路里
    Gui_UpdateRunModeCalories(calories, language);  
    //计步
    Gui_UpdateRunModeStep(step); 
}

/****************************************************************
** Function name:           Gui_PageRunModePause
** Descriptions:            运动模式暂停开始界面
** input parameters:        pause_state：1暂停，0运行
                            language: 语言，0中文、1英文
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
	// 暂停开始 
    if(pause_state)
    {        
        LCD_SetPosition(icon_16_move4_icon_coord[0][0], icon_16_move4_icon_coord[0][1], 
                        ICON_MOVE4_ICON_WIDE, ICON_MOVE4_ICON_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_move4_icon_addr[0], ICON_MOVE4_ICON_SIZE);
    }
    else        
    {
        LCD_SetPosition(icon_16_move3_icon_coord[0][0], icon_16_move3_icon_coord[0][1],  
                        ICON_MOVE3_ICON_WIDE, ICON_MOVE3_ICON_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_move3_icon_addr[0], ICON_MOVE3_ICON_SIZE);
    }  
    
    for(uint8_t i = 0; i < 3; i++)
    {
        /* 图标 */
        LCD_SetPosition(icon_16_move_icon2_coord[i][0], icon_16_move_icon2_coord[i][1], 
                        ICON_MOVE2_ICON3_WIDE, ICON_MOVE2_ICON3_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_move_icon2_addr[(i == 1) ? 1 : 0], ICON_MOVE2_ICON3_SIZE);
    } 
}

/****************************************************************
** Function name:           Gui_PageRunModeExit
** Descriptions:            运动模式退出界面
** input parameters:        language: 语言，0中文、1英文
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
	// 退出
    LCD_SetPosition(icon_16_move5_icon_coord[0][0], icon_16_move5_icon_coord[0][1], 
                    ICON_MOVE5_ICON_WIDE, ICON_MOVE5_ICON_HIGH);
    LCD_DriverWriteDataFromExtFlash(icon_16_move5_icon_addr[0], ICON_MOVE5_ICON_SIZE); 
    
    for(uint8_t i = 0; i < 3; i++)
    {
        /* 图标 */
        LCD_SetPosition(icon_16_move_icon2_coord[i][0], icon_16_move_icon2_coord[i][1], 
                        ICON_MOVE2_ICON3_WIDE, ICON_MOVE2_ICON3_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_move_icon2_addr[(i == 2) ? 1 : 0], ICON_MOVE2_ICON3_SIZE);
    } 
}

/****************************************************************
** Function name:           Gui_UpdateBp
** Descriptions:            显示血压正常数据
** input parameters:        bp_sp: 高压 bp_dp：低压
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
 
    /* 高压 */   
    start_column = 1;
    start_page = icon_16_bp_data_coord[0][1];
    if(bp_sp < 100)
    {
        start_column = 15;
    }
    else
    {
        LCD_SetPosition(start_column, start_page, ICON_BP_DATA1_WIDE, ICON_BP_DATA1_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_bp_data_addr[(bp_sp / 100)% 10], ICON_BP_DATA1_SIZE);          
        start_column += ICON_BP_DATA1_WIDE;
    }
    LCD_SetPosition(start_column, start_page, ICON_BP_DATA1_WIDE, ICON_BP_DATA1_HIGH);
    LCD_DriverWriteDataFromExtFlash(icon_16_bp_data_addr[(bp_sp / 10)% 10], ICON_BP_DATA1_SIZE);          
    start_column += ICON_BP_DATA1_WIDE;

    LCD_SetPosition(start_column, start_page, ICON_BP_DATA1_WIDE, ICON_BP_DATA1_HIGH);
    LCD_DriverWriteDataFromExtFlash(icon_16_bp_data_addr[bp_sp % 10], ICON_BP_DATA1_SIZE);          

    
    /* 斜杠 */
    start_column = 6;
    start_page = icon_16_bp_data_coord[1][1]; 
    if(bp_dp < 100)
    {
        start_column = 15;
    } 
    LCD_SetPosition(start_column, icon_16_bp_icon_slash_coord[0][1], ICON_BP_ICON3_WIDE, ICON_BP_ICON3_HIGH);
    LCD_DriverWriteDataFromExtFlash(icon_16_bp_icon_slash_addr[0], ICON_BP_ICON3_SIZE);
    start_column += ICON_BP_ICON3_WIDE;
  
    /* 低压 */
    if(bp_dp >= 100)
    {
        LCD_SetPosition(start_column, start_page, ICON_BP_DATA2_WIDE, ICON_BP_DATA2_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_bp_low_data_addr[(bp_dp % 1000) / 100], ICON_BP_DATA2_SIZE); 
        start_column += ICON_BP_DATA2_WIDE;
    }
    LCD_SetPosition(start_column, start_page, ICON_BP_DATA2_WIDE, ICON_BP_DATA2_HIGH);
    LCD_DriverWriteDataFromExtFlash(icon_16_bp_low_data_addr[(bp_dp % 100) / 10], ICON_BP_DATA2_SIZE); 
    start_column += ICON_BP_DATA2_WIDE;

    LCD_SetPosition(start_column, start_page, ICON_BP_DATA2_WIDE, ICON_BP_DATA2_HIGH);
    LCD_DriverWriteDataFromExtFlash(icon_16_bp_low_data_addr[bp_dp % 10], ICON_BP_DATA2_SIZE);
    
}

/****************************************************************
** Function name:           Gui_PageBP
** Descriptions:            显示血压界面
** input parameters:        index：动态图计数  bp_sp: 高压 bp_dp：低压
**                          test_mode：测试模式，1私人模式，0通用模式
**                          test_state：测试的状态，1测试中，2测出结果，3测试出错
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
        /* mmhg */
        LCD_SetPosition(icon_16_bp_unit_coord[0][0], icon_16_bp_unit_coord[0][1], 
                        ICON_BP_UNIT_WIDE, ICON_BP_UNIT_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_bp_unit_addr[0], ICON_BP_UNIT_SIZE);
    }
    
    LCD_SetPosition(icon_16_bp_icon_p_coord[0][0], icon_16_bp_icon_p_coord[0][1], 
                    ICON_BP_ICON1_WIDE, ICON_BP_ICON1_HIGH);
    if(test_mode == 1)
    {
        /* p */
        LCD_DriverWriteDataFromExtFlash(icon_16_bp_icon_dynamic_addr[3], ICON_BP_ICON1_SIZE);
    }
    else
    {
        LCD_DriverWriteDataFromExtFlash(icon_16_bp_icon_dynamic_addr[4], ICON_BP_ICON1_SIZE);
    }
    if(test_state == 0)
    {    
        /* 绘制血压图标动态 */
        LCD_SetPosition(icon_16_bp_icon_dynamic_coord[0][0], icon_16_bp_icon_dynamic_coord[0][1], 
                        ICON_BP_ICON1_WIDE, ICON_BP_ICON1_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_bp_icon_dynamic_addr[index % 3], ICON_BP_ICON1_SIZE);
        

        for(uint8_t i = 0; i < 6; i++)
        {
            LCD_SetPosition(icon_16_bp_icon_bar_coord[i][0], icon_16_bp_icon_bar_coord[i][1], 
                            ICON_BP_ICON4_WIDE, ICON_BP_ICON4_HIGH);
            LCD_DriverWriteDataFromExtFlash(icon_16_bp_icon_bar_addr[0], ICON_BP_ICON4_SIZE);
        }
    }
    else if(test_state == 1)
    {      
        /* 绘制血压图标动态 */
        LCD_SetPosition(icon_16_bp_icon_dynamic_coord[0][0], icon_16_bp_icon_dynamic_coord[0][1], 
                        ICON_BP_ICON1_WIDE, ICON_BP_ICON1_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_bp_icon_dynamic_addr[0], ICON_BP_ICON1_SIZE);

        Gui_UpdateBp(bp_sp, bp_dp);
    }
    else if(test_state >= 2)
    {
        /* 绘制血压图标动态 */
        LCD_SetPosition(icon_16_bp_icon_dynamic_coord[0][0], icon_16_bp_icon_dynamic_coord[0][1], 
                        ICON_BP_ICON1_WIDE, ICON_BP_ICON1_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_bp_icon_dynamic_addr[0], ICON_BP_ICON1_SIZE);
        
        for(uint8_t i = 0; i < 6; i++)
        {
            LCD_SetPosition(icon_16_bp_icon_x_coord[i][0], icon_16_bp_icon_x_coord[i][1], 
                            ICON_BP_ICON5_WIDE, ICON_BP_ICON5_HIGH);
            LCD_DriverWriteDataFromExtFlash(icon_16_bp_icon_x_addr[0], ICON_BP_ICON5_SIZE);
        }
    }
}

/****************************************************************
** Function name:           Gui_PageHeart
** Descriptions:            显示心率界面
** input parameters:        index：动态图计数  heart: 心率值
** output parameters:       
** Returned value:          
** Created by:              YX              
** Created Date:            2017-08-11
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Gui_PageHeart(uint8_t index, uint8_t heart)
{
    uint8_t start_column = 0;
    uint8_t start_page = 0;  

    /* bmp */
    LCD_SetPosition(icon_16_heart_unit_coord[0][0], icon_16_heart_unit_coord[0][1], 
                    ICON_HEART_UNIT_WIDE, ICON_HEART_UNIT_HIGH);
    LCD_DriverWriteDataFromExtFlash(icon_16_heart_unit_addr[0], ICON_HEART_UNIT_SIZE);

    /* 绘制心率图标动态 */
    LCD_SetPosition(icon_16_heart_icon2_coord[0][0], icon_16_heart_icon2_coord[0][1], 
                    ICON_HEART_ICON_WIDE, ICON_HEART_ICON_HIGH);
    LCD_DriverWriteDataFromExtFlash(icon_16_heart_icon2_addr[index % 3], ICON_HEART_ICON_SIZE);
   
	/* 显示心率值 */
    start_page = icon_16_heart_data_coord[0][1];
    start_column = icon_16_heart_data_coord[0][0];
    LCD_SetPosition(start_column, start_page, ICON_FUNCTION_DATA_WIDE, ICON_FUNCTION_DATA_HIGH);
    if(heart >= 100)
    {    
        LCD_DriverWriteDataFromExtFlash(icon_16_bp_data_addr[(heart / 100) %10], ICON_FUNCTION_DATA_SIZE);        
    }  
    else
    {    
        LCD_DriverWriteDataFromExtFlash(icon_16_function1_data_addr[0], ICON_FUNCTION_DATA_SIZE);        
    }
    start_column += ICON_FUNCTION_DATA_WIDE + 2;
    LCD_SetPosition(start_column, start_page, ICON_FUNCTION_DATA_WIDE, ICON_FUNCTION_DATA_HIGH);
    if(heart >= 10)
    {    
        LCD_DriverWriteDataFromExtFlash(icon_16_bp_data_addr[(heart / 10) %10], ICON_FUNCTION_DATA_SIZE);        
    }  
    else
    {    
        LCD_DriverWriteDataFromExtFlash(icon_16_function1_data_addr[0], ICON_FUNCTION_DATA_SIZE);        
    }
    
    start_column += ICON_FUNCTION_DATA_WIDE + 2;
    LCD_SetPosition(start_column, start_page, ICON_FUNCTION_DATA_WIDE, ICON_FUNCTION_DATA_HIGH);
    if(heart > 0)
    {    
        LCD_DriverWriteDataFromExtFlash(icon_16_bp_data_addr[heart %10], ICON_FUNCTION_DATA_SIZE);        
    }  
    else
    {    
        LCD_DriverWriteDataFromExtFlash(icon_16_function1_data_addr[0], ICON_FUNCTION_DATA_SIZE);        
    }
}

/****************************************************************
** Function name:           Gui_PageSleep
** Descriptions:            睡眠界面
** input parameters:        index：动态图计数 
**                          time：睡眠的时间
**                          language：单位显示的语言
** output parameters:       无
** Returned value:          无
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
	/* 睡眠动态图 */
    LCD_SetPosition(icon_16_sleep_icon_coord[0][0], icon_16_sleep_icon_coord[0][1], 
                    ICON_SLEEP_ICON_WIDE, ICON_SLEEP_ICON_HIGH);
    LCD_DriverWriteDataFromExtFlash(icon_16_sleep_icon_addr[index % 4], ICON_SLEEP_ICON_SIZE);
    /* 由于中英文设置，单位会改变，所以实时显示*/
    /* unit */
    LCD_SetPosition(icon_16_sleep_unit_coord[0][0], icon_16_sleep_unit_coord[0][1], 
                    ICON_SLEEP_UNIT_WIDE, ICON_SLEEP_UNIT_HIGH);
    LCD_DriverWriteDataFromExtFlash(icon_16_sleep_unit_addr[language % 2], ICON_SLEEP_UNIT_SIZE);

    start_page = icon_16_sleep_data_coord[0][1];
    start_column = icon_16_sleep_data_coord[0][0];
    LCD_SetPosition(start_column, start_page, ICON_FUNCTION_DATA_WIDE, ICON_FUNCTION_DATA_HIGH);
    if(sleep_hours >= 10)
    {    
        LCD_DriverWriteDataFromExtFlash(icon_16_sleep_data_addr[(sleep_hours / 10) %10], ICON_FUNCTION_DATA_SIZE);        
    }  
    else
    {    
        LCD_DriverWriteDataFromExtFlash(icon_16_function1_data_addr[0], ICON_FUNCTION_DATA_SIZE);        
    }
    start_column += ICON_FUNCTION_DATA_WIDE + 2;
    LCD_SetPosition(start_column, start_page, ICON_FUNCTION_DATA_WIDE, ICON_FUNCTION_DATA_HIGH);
    if(sleep_hours > 0)
    {    
        LCD_DriverWriteDataFromExtFlash(icon_16_sleep_data_addr[sleep_hours %10], ICON_FUNCTION_DATA_SIZE);        
    }  
    else
    {    
        LCD_DriverWriteDataFromExtFlash(icon_16_function1_data_addr[0], ICON_FUNCTION_DATA_SIZE);        
    }
    
    start_column += ICON_FUNCTION_DATA_WIDE + 2;
    LCD_SetPosition(start_column, start_page, ICON_FUNCTION_DATA_WIDE, ICON_FUNCTION_DATA_HIGH);
    if(sleep_minutes > 0 || sleep_hours > 0)
    {    
        LCD_DriverWriteDataFromExtFlash(icon_16_sleep_data_addr[sleep_minutes %10], ICON_FUNCTION_DATA_SIZE);        
    }  
    else
    {    
        LCD_DriverWriteDataFromExtFlash(icon_16_function1_data_addr[0], ICON_FUNCTION_DATA_SIZE);        
    }
    //小数点
    LCD_SetPosition(icon_16_sleep_point_coord[0][0], icon_16_sleep_point_coord[0][1], ICON_SLEEP_ICON2_WIDE, ICON_SLEEP_ICON2_HIGH);
    LCD_DriverWriteDataFromExtFlash(icon_16_sleep_point_addr[0], ICON_SLEEP_ICON2_SIZE);            
}

/****************************************************************
** Function name:           Gui_PagePower
** Descriptions:            关机界面
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
    /* 关机图标 */
    LCD_SetPosition(icon_16_poweroff_icon_coord[0][0], icon_16_poweroff_icon_coord[0][1], 
                    ICON_POWEROFF_ICON_WIDE, ICON_POWEROFF_ICON_HIGH);
    LCD_DriverWriteDataFromExtFlash(icon_16_poweroff_icon_addr[0], ICON_POWEROFF_ICON_SIZE);
}

/****************************************************************
** Function name:           Gui_PageFindPhone
** Descriptions:            找手机界面
** input parameters:        dynamic_find：是否开启动态图
**                          index：动态图计数
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
        /* 手机图标 */
        LCD_SetPosition(icon_16_findphone_dynamic_coord[0][0], icon_16_findphone_dynamic_coord[0][1], 
                        ICON_FINDPHONE_ICON_WIDE, ICON_FINDPHONE_ICON_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_findphone_dynamic_addr[0], ICON_FINDPHONE_ICON_SIZE);
    }
    
	if(!dynamic_find)
    {
        /* 关闭动态图 */
		index = 0;
    }
    /* 动态图标 */
    LCD_SetPosition(icon_16_findphone_dynamic_coord[0][0], icon_16_findphone_dynamic_coord[0][1], 
                    ICON_FINDPHONE_ICON_WIDE, ICON_FINDPHONE_ICON_HIGH);
    LCD_DriverWriteDataFromExtFlash(icon_16_findphone_dynamic_addr[index % 3], ICON_FINDPHONE_ICON_SIZE);    
}

/****************************************************************
** Function name:           Gui_PageStopWatchBackGround
** Descriptions:            显示秒表界面的背景设置
** input parameters:        color_type：背景颜色
** output parameters:       无
** Returned value:          无
** Created by:              WJ             
** Created Date:            2017-08-09
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Gui_PageStopWatchBackGround(void)
{     
//    /*下面部分*/
//    Gui_PageRefreshBackground(3, 5, 74, 46, backgroud_color_buf[main_background_color % 2][1]);
//    /*下面部分*/
//    Gui_PageRefreshBackground(3, 57, 74, 46, backgroud_color_buf[main_background_color % 2][1]);
//    /*下面部分*/
//    Gui_PageRefreshBackground(3, 109, 74, 46, backgroud_color_buf[main_background_color % 2][1]);
//    
//    /* 背景色的8个角 */
//    for(uint8_t i = 0; i < 12; i++)
//    {
//        LCD_SetPosition(icon_16_stopwatch_by_coord[i][0], icon_16_stopwatch_by_coord[i][1],
//                        ICON_MAIN1_BACKGROUND_WIDE, ICON_MAIN1_BACKGROUND_HIGH);
//        LCD_DriverWriteDataFromExtFlash(icon_16_main_by_addr[main_background_color % 2][i % 4], ICON_MAIN1_BACKGROUND_SIZE);
//    }
}

/****************************************************************
** Function name:           GUI_StopWatchPage
** Descriptions:            秒表界面
** input parameters:        entry: 是否已经启动秒表
**                          total_second：总秒数
**                          one_hundred_ms：100毫秒数
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
    uint8_t rand_num = 0;
    uint8_t data_tmp[6] = {0,};
    
    if(entry == 0)
    {
        LCD_SetPosition(icon_16_stopwatch_icon_coord[0][0], icon_16_stopwatch_icon_coord[0][1],
                        ICON_STOPWATCH_ICON_WIDE, ICON_STOPWATCH_ICON_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_stopwatch_icon_addr[0], ICON_STOPWATCH_ICON_SIZE);
    }
    else
    {
        second = total_second % 60;
        minute = total_second / 60;
        if(total_second == 0 && one_hundred_ms == 0)
        {
            rand_num = 0;
        }
        else if((total_second == 5999 && one_hundred_ms >= 9) 
               || total_second > 5999)
        {
            rand_num = 9;
        }
        else
        {
            rand_num = one_hundred_ms + 5; 
        }
        data_tmp[0] = minute / 10;
        data_tmp[1] = minute % 10;
        data_tmp[2] = second / 10;
        data_tmp[3] = second % 10;
        data_tmp[4] = one_hundred_ms % 10;
        data_tmp[5] = rand_num % 10;
        
        if(index == 0)
        {
            Gui_PageStopWatchBackGround();
        }
        for(uint8_t i = 0; i < 6; i++)
        {
            LCD_SetPosition(icon_16_stopwatch_data_coord[i][0], icon_16_stopwatch_data_coord[i][1], 
                            ICON_MAIN1_DATA3_WIDE, ICON_MAIN1_DATA3_HIGH);
            LCD_DriverWriteDataFromExtFlash(icon_16_main_hour_addr[0][data_tmp[i]], ICON_MAIN1_DATA3_SIZE);
        }
    }

}

/****************************************************************
** Function name:           Gui_PageCountDown
** Descriptions:            倒计时界面
** input parameters:        dynamic：是否开启动态图
**                          index：动态图计数 
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
        /* 关闭动态图 */
		index = 0;	
    }
    LCD_SetPosition(icon_16_countdown_icon_coord[0][0], icon_16_countdown_icon_coord[0][1], 
                    ICON_COUNTDOWN_ICON_WIDE, ICON_COUNTDOWN_ICON_HIGH);
    LCD_DriverWriteDataFromExtFlash(icon_16_countdown_icon_addr[index % 3], ICON_COUNTDOWN_ICON_SIZE);
    		
}

/****************************************************************
** Function name:           Gui_PageCameraReady
** Descriptions:            相机准备界面
** input parameters:        index：动态图计数 
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
        /* 显示图标 */
        LCD_SetPosition(icon_16_camera_icon_coord[0][0], icon_16_camera_icon_coord[0][1],
                        ICON_CAMERA_ICON_WIDE, ICON_CAMERA_ICON_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_camera_icon_addr[0], ICON_CAMERA_ICON_SIZE);
    }
}

/****************************************************************
** Function name:           Gui_PageCameraOk
** Descriptions:            相机OK界面
** input parameters:        index：动态图计数 
** output parameters:       
** Returned value:          
** Created by:              YX
** Created Date:            2017-08-11
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Gui_PageCameraOk(uint8_t index)
{
    if(index == 0)
    {
        /* 显示图标 */
        LCD_SetPosition(icon_16_camera_icon_coord[0][0], icon_16_camera_icon_coord[0][1],
                        ICON_CAMERA_ICON_WIDE, ICON_CAMERA_ICON_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_camera_icon_addr[1], ICON_CAMERA_ICON_SIZE);
    }
}

/****************************************************************
** Function name:           Gui_PageDisMacVer
** Descriptions:            显示Mac和版本号
** input parameters:        index：动态图计数 
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
        dis_mac_data_buf[2] = (mac_data[1] / 16) % 16;
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
        dis_mac_data_buf[5] = (mac_data[0] / 16) % 16;
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
        Font_DisContentText(dis_mac_data_buf, 7, 7, 49, 1, 2, 0);
//        Font_DisplayContentText(dis_mac_data_buf, 7, 49, 1);
        
        dis_ver_data_buf[0] = 'V';
        dis_ver_data_buf[1] = ':';
        dis_ver_data_buf[2] = ver_data[0] / 16 + '0';
        dis_ver_data_buf[3] = ver_data[0] % 16 + '0';
        dis_ver_data_buf[4] = ':';
        dis_ver_data_buf[5] = ver_data[1] / 16 + '0';
        dis_ver_data_buf[6] = ver_data[1] % 16 + '0';
        Font_DisContentText(dis_ver_data_buf, 7, 7, 95, 1, 2, 0);
//        Font_DisplayContentText(dis_ver_data_buf, 7, 95, 1);
    }
}

/****************************************************************
** Function name:           Gui_PageAlarm
** Descriptions:            闹钟界面
** input parameters:        am_pm_format：am、pm状态     scene：闹钟的类型 
**                          hour：闹钟的时   minute：闹钟的分
**                          language：闹钟单位的语言  index：动态图计数
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
    
    // 这里需要根据闹铃来提醒
	if(scene == 0)
    {
        /* 动图*/
        LCD_SetPosition(icon_16_clock_icon_coord[0][0], icon_16_clock_icon_coord[0][1], 
                        ICON_CLOCK_ICON_WIDE, ICON_CLOCK_ICON_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_clock_icon_addr[index % 2], ICON_CLOCK_ICON_SIZE);
        if(index == 0)
        {
            /* AM,PM,上午,下午 */
            LCD_SetPosition(icon_16_clock_ampm_coord[0][0], icon_16_clock_ampm_coord[0][1],
                            ICON_CLOCK_ICON2_WIDE, ICON_CLOCK_ICON2_HIGH);
            if(am_pm_format == GUI_TIME_FORMAT_STANDARD)
            {
                Gui_PageClearBackground(icon_16_clock_ampm_coord[0][0], icon_16_clock_ampm_coord[0][1], 
                                        ICON_CLOCK_ICON2_WIDE, ICON_CLOCK_ICON2_HIGH);
            }
            else
            {
                if(language == 0)
                {
                    LCD_DriverWriteDataFromExtFlash(icon_16_clock_ampm_addr[am_pm_format - 1], ICON_CLOCK_ICON2_SIZE);
                }
                else
                {
                    LCD_DriverWriteDataFromExtFlash(icon_16_clock_ampm_addr[am_pm_format + 1], ICON_CLOCK_ICON2_SIZE);
                }
            }
            time_data[0] = hour / 10;
            time_data[1] = hour % 10;
            time_data[2] = minute / 10;
            time_data[3] = minute % 10;
            /* 时间 */
            for(uint8_t i = 0; i < 4; i++)
            {
                LCD_SetPosition(icon_16_clock_data_coord[i][0], icon_16_clock_data_coord[i][1],
                                ICON_STEP_DATA_WIDE, ICON_STEP_DATA_HIGH);
                LCD_DriverWriteDataFromExtFlash(icon_16_step_data_addr[time_data[i]], ICON_STEP_DATA_SIZE);
            }
            /* 冒号 */
            LCD_SetPosition(icon_16_clock_colon_coord[0][0], icon_16_clock_colon_coord[0][1], 
                            ICON_CLOCK_ICON3_WIDE, ICON_CLOCK_ICON3_HIGH);
            LCD_DriverWriteDataFromExtFlash(icon_16_clock_colon_addr[0], ICON_CLOCK_ICON3_SIZE);
        }
    }
	else
    {
        /* 显示图标 */
        LCD_SetPosition(icon_16_remindclock_icon_coord[0][0], icon_16_remindclock_icon_coord[0][1], 
                        ICON_REMINDCLOCK_ICON_WIDE, ICON_REMINDCLOCK_ICON_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_remindclock_icon_addr[(scene - 1) % 20], ICON_REMINDCLOCK_ICON_SIZE);
    }		
}

/****************************************************************
** Function name:           Gui_PageSit
** Descriptions:            久坐界面
** input parameters:        index：动态图计数
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
    ///* 清除可能会花屏的部分 */
    Gui_PageClearBackground(icon_16_sit_icon_coord[0][0], icon_16_sit_icon_coord[1][1], 
                            ICON_SIT_ICON_WIDE, icon_16_sit_icon_coord[0][1] - icon_16_sit_icon_coord[1][1]);
    Gui_PageClearBackground(icon_16_sit_icon_coord[0][0], icon_16_sit_icon_coord[1][1] + ICON_SIT_ICON_HIGH, 
                            ICON_SIT_ICON_WIDE, icon_16_sit_icon_coord[0][1] - icon_16_sit_icon_coord[1][1]);
    /* 显示图标 */
    LCD_SetPosition(icon_16_sit_icon_coord[index % 2][0], icon_16_sit_icon_coord[index % 2][1],
                    ICON_SIT_ICON_WIDE, ICON_SIT_ICON_HIGH);
    LCD_DriverWriteDataFromExtFlash(icon_16_sit_icon_addr[0], ICON_SIT_ICON_SIZE);
}

/****************************************************************
** Function name:           Gui_PageLowPower
** Descriptions:            低电提醒界面
** input parameters:        index：动态图计数
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
    /* 显示静态图标 */
    LCD_SetPosition(icon_16_lowpower_static_coord[0][0], icon_16_lowpower_static_coord[0][1],
                    ICON_CHG_ICON1_WIDE, ICON_CHG_ICON1_HIGH);
    LCD_DriverWriteDataFromExtFlash(icon_16_lowpower_static_addr[0], ICON_CHG_ICON1_SIZE);
    
    /* 显示动态图标 */

    LCD_SetPosition(icon_16_lowpower_dynamic_coord[0][0], icon_16_lowpower_dynamic_coord[0][1],
                    ICON_CHG_ICON2_WIDE, ICON_CHG_ICON2_HIGH);
    LCD_DriverWriteDataFromExtFlash(icon_16_lowpower_dynamic_addr[index % 2], ICON_CHG_ICON2_SIZE);

}

/****************************************************************
** Function name:           Gui_PageBatteryCharg
** Descriptions:            电池充电界面
** input parameters:        index：动态图计数
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
    if(index == 0)//(index % 7 == 0)
    {
        /* 显示静态图标 */
        LCD_SetPosition(icon_16_chg_icon_coord[0][0], icon_16_chg_icon_coord[0][1],
                        ICON_CHG_ICON1_WIDE, ICON_CHG_ICON1_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_chg_icon_addr[0], ICON_CHG_ICON1_SIZE);
    }
    if(state == 1)
    {
        LCD_SetPosition(icon_16_chg_bar_coord[0][0], icon_16_chg_bar_coord[0][1],
                        ICON_CHG_ICON2_WIDE, ICON_CHG_ICON2_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_chg_bar_addr[3], ICON_CHG_ICON2_SIZE);
    }
    else
    { 
        LCD_SetPosition(icon_16_chg_bar_coord[0][0], icon_16_chg_bar_coord[0][1],
                        ICON_CHG_ICON2_WIDE, ICON_CHG_ICON2_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_chg_bar_addr[index % 5], ICON_CHG_ICON2_SIZE);
    }
}


void Gui_UpdateBatteryProcess(uint8_t battery_full, uint8_t battery_level)
{    
    
}

/****************************************************************
** Function name:           Gui_PageHeartAlert
** Descriptions:            心率报警界面
** input parameters:        index：动态图计数
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
        /* 显示静态图标 */
        LCD_SetPosition(icon_16_heartalert_static_coord[0][0], icon_16_heartalert_static_coord[0][1],
                        ICON_HEARTALERT_ICON1_WIDE, ICON_HEARTALERT_ICON1_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_heartalert_static_addr[0], ICON_HEARTALERT_ICON1_SIZE);
    }
    
    /* 显示动态图标 */
    LCD_SetPosition(icon_16_heartalert_dynamic_coord[0][0], icon_16_heartalert_dynamic_coord[0][1],
                    ICON_HEARTALERT_ICON2_WIDE, ICON_HEARTALERT_ICON2_HIGH);
    LCD_DriverWriteDataFromExtFlash(icon_16_heartalert_dynamic_addr[index % 2], ICON_HEARTALERT_ICON2_SIZE);
}

/****************************************************************
** Function name:           Gui_PageFontTest
** Descriptions:            显示“维亿魄科技”，测试用
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
    Font_DisplayFontTestText(); //生产测试使用
}


/****************************************************************
** Function name:           Gui_PageCallNotificationIcon
** Descriptions:            显示来电界面
** input parameters:        call_state：来电的状态
**                          index：动态图计数
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
    static uint8_t call_state_bk = 0;   //上次进来的电话状态
    
    if(call_state_bk != call_state)
    {
//        /* 擦除背景 */
//        Gui_PageClearBackground(icon_16_call_arr_coord[0][0], icon_16_call_arr_coord[0][1],
//                                ICON_CALL_ARR_WIDE, ICON_CALL_ARR_HIGH + 1);
//        call_state_bk = call_state;
    }
    
    if(call_state == GUI_CALL_MUTE)
    {
//        LCD_SetPosition(icon_16_call_static_coord[0][0], icon_16_call_static_coord[0][1],
//                        ICON_CALL_ICON2_WIDE, ICON_CALL_ICON2_HIGH);
//        LCD_DriverWriteDataFromExtFlash(icon_16_call_static_addr[0], ICON_CALL_ICON2_SIZE);
        /* 静音 */
        LCD_SetPosition(icon_16_call_mute_coord[0][0], icon_16_call_mute_coord[0][1],
                        ICON_CALL_ICON4_WIDE, ICON_CALL_ICON4_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_call_mute_addr[0], ICON_CALL_ICON4_SIZE);
//        Gui_PageClearBackground(icon_16_call_mute_coord[0][0], icon_16_call_dynamic_coord[0][1],
//                                ICON_CALL_ICON3_WIDE, icon_16_call_mute_coord[0][1] - icon_16_call_dynamic_coord[0][1]);
    }
    else if(call_state == GUI_CALL_REJECTSUC)
    {
        //清除花屏问题
        Gui_PageClearBackground(icon_16_call_mute_coord[0][0], icon_16_call_mute_coord[0][1],
                                ICON_CALL_ICON4_WIDE, 1);
        /* 拒接 */
        LCD_SetPosition(icon_16_call_hangup_coord[0][0], icon_16_call_hangup_coord[0][1],
                        ICON_CALL_ICON1_WIDE, ICON_CALL_ICON1_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_call_hangup_addr[0], ICON_CALL_ICON1_SIZE);
//        Gui_PageClearBackground(icon_16_call_hangup_coord[0][0] + ICON_CALL_ICON1_WIDE, icon_16_call_hangup_coord[0][1],
//                                6, ICON_CALL_ICON1_HIGH);
    }
    else
    {        
        /* 来电动态图标 */
        LCD_SetPosition(icon_16_call_dynamic_coord[0][0], icon_16_call_dynamic_coord[0][1],
                        ICON_CALL_ICON3_WIDE, ICON_CALL_ICON3_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_call_dynamic_addr[index % 3], ICON_CALL_ICON3_SIZE);
    }
    
    if(index == 0 && (call_state != GUI_CALL_REJECTSUC))
    {
        /* 来电静态图标 */
        LCD_SetPosition(icon_16_call_static_coord[0][0], icon_16_call_static_coord[0][1],
                        ICON_CALL_ICON2_WIDE, ICON_CALL_ICON2_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_call_static_addr[0], ICON_CALL_ICON2_SIZE);
    }    
}

/****************************************************************
** Function name:           Gui_PageCallNotificationContent
** Descriptions:            显示来电消息推送界面
** input parameters:        display_buf：消息数组指针
                            buf_len：内容长度
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
    Font_DisplayTitleText(display_buf, buf_len, icon_16_call_text_coord[0][1], FIRST_PAGE_ROWS);
}

/****************************************************************
** Function name:           Gui_PageMessageNotificationIcon
** Descriptions:            显示消息推送界面
** input parameters:        msg_type：消息的类型
**                          page_index：动态图计数
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
    if(msg_type <= GUI_MSG_TYPE_PHYS && msg_type > GUI_MSG_TYPE_CALL)
    {
//        /* 清除可能会花屏的部分 */
//        Gui_PageClearBackground(icon_16_inform_icon_coord[0][0], icon_16_inform_icon_coord[1][1], 
//                                ICON_INFORM_ICON_WIDE, icon_16_inform_icon_coord[0][1] - icon_16_inform_icon_coord[1][1]);
//        Gui_PageClearBackground(icon_16_inform_icon_coord[0][0], icon_16_inform_icon_coord[1][1] + ICON_INFORM_ICON_HIGH, 
//                                ICON_INFORM_ICON_WIDE, icon_16_inform_icon_coord[0][1] - icon_16_inform_icon_coord[1][1]);
        /* 显示图标 */
        LCD_SetPosition(icon_16_inform_icon_coord[0][0], icon_16_inform_icon_coord[0][1],
                        ICON_INFORM_ICON_WIDE, ICON_INFORM_ICON_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_inform_icon_addr[msg_type - 2], ICON_INFORM_ICON_SIZE);
    } 
    else
    {
        return;
    }
}
 
/****************************************************************
** Function name:           Gui_PageMessageNotificationContent
** Descriptions:            显示消息推送界面
** input parameters:        display_buf：消息数组指针
                            buf_len：内容长度
                            page_index:第几页
** output parameters:
** Returned value:
** Created by:              WJ
** Created Date:            2017-08-11
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Gui_PageMessageNotificationContent(uint8_t *display_buf, uint16_t buf_len, uint8_t page_index)
{
    if(page_index <= 1)
    {
        Font_DisplayContentText(display_buf, buf_len, icon_16_call_text_coord[0][1], FIRST_PAGE_ROWS);
    }
    else
    {
        Font_DisplayContentText(display_buf, buf_len, 2, PER_PAGE_MAX_ROWS);
    }

}

/****************************************************************
** Function name:           Gui_PageStarget
** Descriptions:            运动目标提醒界面
** input parameters:        index：动态图计数
** output parameters:       无
** Returned value:          无
** Created by:              YX
** Created Date:            2017-09-11
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/
void Gui_PageStarget(uint8_t index)
{
    /* 图标显示 */
    LCD_SetPosition(icon_16_target_icon_coord[0][0], icon_16_target_icon_coord[0][1],
                    ICON_TARGET_ICON_WIDE, ICON_TARGET_ICON_HIGH);
    LCD_DriverWriteDataFromExtFlash(icon_16_target_icon_addr[index % 2], ICON_TARGET_ICON_SIZE);
}

/****************************************************************
** Function name:           Gui_PageBleDiscRemind
** Descriptions:            蓝牙断连提醒
** input parameters:        index：动态图计数
** output parameters:       无
** Returned value:          无
** Created by:              WJ
** Created Date:            2018-04-03
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/
void Gui_PageBleDiscRemind(uint8_t index)
{
    if(index % 2 == 0)
    {
        LCD_SetPosition(icon_16_ble_remind_icon_coord[0][0], icon_16_ble_remind_icon_coord[0][1],
                        ICON_YDISCONNECTED_ICON_WIDE, ICON_YDISCONNECTED_ICON_HIGH);
        LCD_DriverWriteDataFromExtFlash(icon_16_ble_remind_icon_addr[0], ICON_YDISCONNECTED_ICON_SIZE);
    }
    else
    {
       Gui_PageClearBackground(icon_16_ble_remind_icon_coord[0][0], icon_16_ble_remind_icon_coord[0][1],
                               ICON_YDISCONNECTED_ICON_WIDE, ICON_YDISCONNECTED_ICON_HIGH);
    }
}

