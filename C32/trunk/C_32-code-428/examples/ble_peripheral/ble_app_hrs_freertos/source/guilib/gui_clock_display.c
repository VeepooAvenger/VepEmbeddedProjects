/***************************Copyright(c)***********************************
**                       Veepoo Tech
**                  COPYRIGHT (C) 2016-2019
**-----------File Info-----------------------------------------------------
** File        name:     gui_clock_display.c
** Last modified Date:   2017-07-25
** Last SYS_VERSION:     0.0.0
** Descriptions:            
**-------------------------------------------------------------------------
** Created  by:          WJ
** Created  date:        2017-09-12
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

#include "lcd_driver_tft.h"
#include "gui_flash_param.h"
#include "gui_flash_addr.h"
#include "gui_page_display.h"

///*********************************************************************
//* Macro Definition
//*/
//#define COLCK_POINTER_SIZE          ICON_MAIN3_ICON1_WIDE       //图片宽高大小（宽高相等）
//#define COLCK_ORIGIN_SIZE           8                           //中间原点宽高大小（宽高相等）
//#define COLCK_POINTER_BUF_SIZE      (COLCK_POINTER_SIZE * COLCK_POINTER_SIZE * 2) //图片大小

//#define COLCK_ORIGIN_START_INDEX    (COLCK_POINTER_SIZE - COLCK_ORIGIN_SIZE)    


//const uint32_t clock_h_addr[2] = 
//{
//    ICON_MAIN3_ICON1_01_ADDR, ICON_MAIN3_ICON1_01_ADDR,
//};
//const uint32_t clock_m_addr[2] = 
//{
//    ICON_MAIN3_ICON2_01_ADDR, ICON_MAIN3_ICON2_01_ADDR,
//};

//const uint8_t clock_coord[4][2] = { {36, 56}, {36, 76}, {44 - COLCK_POINTER_SIZE, 76}, {44 - COLCK_POINTER_SIZE, 56}};

////黑色、黑色
//const uint16_t clock_bg_color[2] = {0x0000, 0x0000,};

////指针填充颜色
//const uint16_t clock_pointer_color[2] = {0xFFFF, 0xFFFF};

///*********************************************************************
//* Global Variables
//*/
//static uint16_t clock_h_buf[COLCK_POINTER_SIZE][COLCK_POINTER_SIZE] = {0x00,};
//static uint16_t clock_m_buf[COLCK_POINTER_SIZE][COLCK_POINTER_SIZE] = {0x00,};
//static uint16_t clock_origin_h_buf[COLCK_ORIGIN_SIZE][COLCK_ORIGIN_SIZE] = {0x00,};
//static uint16_t clock_origin_m_buf[COLCK_ORIGIN_SIZE][COLCK_ORIGIN_SIZE] = {0x00,};

///*********************************************************************
//* local functions
//*/


///****************************************************************
//** Function name:			Clock_HoursAndMinutesQuadrant
//** Descriptions:            时钟分钟处于同一个区域位置
//** input parameters:        无
//** output parameters:       无 
//** Returned value:          无
//** Created by:				WJ              
//** Created Date:           	2017-09-12
//*****************************************************************/
//uint8_t Clock_HoursAndMinutesQuadrant(uint8_t hours, uint8_t minutes)
//{
//    hours = hours % 12;
//    minutes = minutes % 60;
//       
//    if(hours < 3 && minutes < 15)
//    {
//        return 1;
//    }
//    else if(hours >= 3 && hours < 6 && minutes >= 15 && minutes < 30)
//    {
//        return 2;
//    }
//    else if(hours >= 6 && hours < 9 && minutes >= 30 && minutes < 45)
//    {
//        return 3;
//    }
//    else if(hours >= 9 && hours <= 11 && minutes >= 45 && minutes < 60)
//    {
//        return 4;
//    }
//    else
//    {
//        return 0;
//    }
//}

///****************************************************************
//** Function name:			Clock_HoursQuadrant
//** Descriptions:            时钟位置
//** input parameters:        无
//** output parameters:       无 
//** Returned value:          无
//** Created by:				WJ              
//** Created Date:           	2017-09-12
//*****************************************************************/
//uint8_t Clock_HoursQuadrant(uint8_t hours)
//{
//    hours = hours % 12;
//    
//    if(hours < 3 )
//    {
//        return 1;
//    }
//    else if(hours >= 3 && hours < 6)
//    {
//        return 2;
//    }
//    else if(hours >= 6 && hours < 9)
//    {
//        return 3;
//    }
//    else if(hours >= 9 && hours <= 11)
//    {
//        return 4;
//    }
//    else
//    {
//        return 0;
//    }
//}

///****************************************************************
//** Function name:			Clock_MinutesQuadrant
//** Descriptions:            分钟位置
//** input parameters:        无
//** output parameters:       无 
//** Returned value:          无
//** Created by:				WJ              
//** Created Date:           	2017-09-12
//*****************************************************************/
//uint8_t Clock_MinutesQuadrant(uint8_t minutes)
//{
//    minutes = minutes % 60;
//    
//    if(minutes < 15)
//    {
//        return 1;
//    }
//    else if(minutes >= 15 && minutes < 30)
//    {
//        return 2;
//    }
//    else if(minutes >= 30 && minutes < 45)
//    {
//        return 3;
//    }
//    else if(minutes >= 45 && minutes < 60)
//    {
//        return 4;
//    }
//    else
//    {
//        return 0;
//    }
//}

///****************************************************************
//** Function name:			Clock_DataRotateDegrees
//** Descriptions:            数据旋转
//** input parameters:        无
//** output parameters:       无 
//** Returned value:          无
//** Created by:				WJ              
//** Created Date:           	2017-09-12
//*****************************************************************/
//void Clock_DataRotateDegrees(uint16_t (*data)[COLCK_POINTER_SIZE], uint16_t len)
//{
//    uint16_t t = 0;
//    
//    for( uint16_t i = 0; i < len - 1;i++)
//    {
//        for(uint16_t j = i; j < len - 1 - i; j++)
//        {
//            t = data[i][j];

//            data[i][j] = data[len-1-j][i];
//            data[len - 1 - j][i] = data[len - 1 - i][len - 1 - j];
//            data[len - 1 - i][len - 1 - j] = data[j][len - 1 - i];
//            data[j][len - 1 - i] = t;
//        }
//    }
//}

///****************************************************************
//** Function name:			Clock_DisplayDial
//** Descriptions:            显示表盘指针
//** input parameters:        无
//** output parameters:       无 
//** Returned value:          无
//** Created by:				WJ              
//** Created Date:           	2017-09-12
//*****************************************************************/
//void Clock_DisplayDial(uint8_t hours, uint8_t minutes, uint8_t style)
//{
//    uint8_t clock_h_postion = 0;
//    uint8_t clock_m_postion = 0;
//    uint8_t clock_hm_postion = 0;
//    uint8_t style_is_red = 0;
//    uint32_t h_flash_addr = 0;
//    uint32_t m_flash_addr = 0;
////    static uint8_t  hours = 8;
////    static uint8_t minutes = 50;

////    minutes = (++minutes) % 60;
////    if(minutes == 0)
////    {
////        hours = (++hours) % 24;
////    }

//    style = style % 2;
////    if(style == 1)
////    {
////        style_is_red = 1;
////    }
//    
//    m_flash_addr = clock_m_addr[style] + (minutes % 15) * COLCK_POINTER_BUF_SIZE;    
//    h_flash_addr = clock_h_addr[style] + ((hours % 12 % 3) * 5 +  minutes / 12) * COLCK_POINTER_BUF_SIZE;    
// 
//    //时钟和分钟在同一个象限内   
//    clock_h_postion = Clock_HoursQuadrant(hours);
//    clock_m_postion = Clock_MinutesQuadrant(minutes);
//    for(uint8_t i = 0; i < 4; i++)
//    {
//        if(!(clock_h_postion == i+1 || clock_m_postion == i+1))
//        {
//            Gui_PageClearBackground(clock_coord[i][0], clock_coord[i][1], ICON_MAIN3_ICON1_WIDE, ICON_MAIN3_ICON1_HIGH);  
//        }
//    }
//    
//    clock_hm_postion = Clock_HoursAndMinutesQuadrant(hours, minutes);
//    
//    LCD_DriverReadFromExtFlash(m_flash_addr, (uint8_t *)clock_m_buf, COLCK_POINTER_BUF_SIZE);
//    LCD_DriverReadFromExtFlash(h_flash_addr, (uint8_t *)clock_h_buf, COLCK_POINTER_BUF_SIZE);
//    if(clock_hm_postion != 0)
//    {  
//        for(uint16_t i = 0; i < COLCK_POINTER_SIZE; i++)
//        {
//            for(uint16_t j = 0; j < COLCK_POINTER_SIZE; j++)
//            {               
//                if(clock_m_buf[i][j] == clock_bg_color[style] || clock_h_buf[i][j] == clock_pointer_color[style_is_red])
//                {
//                    clock_m_buf[i][j] = clock_h_buf[i][j];
//                }   
//            }            
//        } 
//        
//        for(uint16_t n = 0; n < clock_hm_postion - 1; n++)
//        {
//            Clock_DataRotateDegrees(clock_m_buf, COLCK_POINTER_SIZE);            
//        }
//        
//        LCD_SetPosition(clock_coord[clock_hm_postion - 1][0], clock_coord[clock_hm_postion - 1][1], COLCK_POINTER_SIZE, COLCK_POINTER_SIZE);
//        LCD_DriverWriteDataBuf((uint8_t *)clock_m_buf, COLCK_POINTER_BUF_SIZE);         
//    }
//    else 
//    {
//        for(uint16_t n = 0; n < clock_h_postion - 1; n++)
//        {
//            Clock_DataRotateDegrees(clock_h_buf, COLCK_POINTER_SIZE);            
//        }
//        
//        LCD_SetPosition(clock_coord[clock_h_postion - 1][0], clock_coord[clock_h_postion - 1][1], COLCK_POINTER_SIZE, COLCK_POINTER_SIZE);
//        LCD_DriverWriteDataBuf((uint8_t *)clock_h_buf, COLCK_POINTER_BUF_SIZE);          
//        
//        
//        for(uint16_t n = 0; n < clock_m_postion - 1; n++)
//        {
//            Clock_DataRotateDegrees(clock_m_buf, COLCK_POINTER_SIZE);            
//        }
//   
//        //特殊处理重叠 8个宽度
//        if((clock_h_postion == 1 && clock_m_postion == 2) || (clock_h_postion == 4 && clock_m_postion == 3))
//        {
//            for(uint16_t i = 0; i < COLCK_ORIGIN_SIZE; i++)
//            {
//                for(uint16_t j = 0; j < COLCK_POINTER_SIZE; j++)
//                {               
//                    if(clock_m_buf[i][j] == clock_bg_color[style] || clock_h_buf[COLCK_ORIGIN_START_INDEX + i][j] == clock_pointer_color[style_is_red])
//                    {
//                        clock_m_buf[i][j] = clock_h_buf[COLCK_ORIGIN_START_INDEX + i][j];
//                    }   
//                }            
//            }             
//        }
//        else if((clock_h_postion == 2 && clock_m_postion == 1) || (clock_h_postion == 3 && clock_m_postion == 4))
//        {
//            for(uint16_t i = 0; i < COLCK_ORIGIN_SIZE; i++)
//            {
//                for(uint16_t j = 0; j < COLCK_POINTER_SIZE; j++)
//                {               
//                    if(clock_m_buf[COLCK_ORIGIN_START_INDEX + i][j] == clock_bg_color[style] || clock_h_buf[i][j] == clock_pointer_color[style_is_red])
//                    {
//                        clock_m_buf[COLCK_ORIGIN_START_INDEX + i][j] = clock_h_buf[i][j];
//                    }   
//                }            
//            }            
//        }
//        else if((clock_h_postion == 3 && clock_m_postion == 2)|| (clock_h_postion == 4 && clock_m_postion == 1))
//        {
//            for(uint16_t i = 0; i < COLCK_POINTER_SIZE; i++)
//            {
//                for(uint16_t j = 0; j < COLCK_ORIGIN_SIZE; j++)
//                {               
//                    if(clock_m_buf[i][j] == clock_bg_color[style] || clock_h_buf[i][COLCK_ORIGIN_START_INDEX + j] == clock_pointer_color[style_is_red])
//                    {
//                        clock_m_buf[i][j] = clock_h_buf[i][COLCK_ORIGIN_START_INDEX + j];
//                    }   
//                }            
//            }
//        }
//        else if((clock_h_postion == 1 && clock_m_postion == 4) ||(clock_h_postion == 2 && clock_m_postion == 3))
//        {
//            for(uint16_t i = 0; i < COLCK_POINTER_SIZE; i++)
//            {
//                for(uint16_t j = 0; j < COLCK_ORIGIN_SIZE; j++)
//                {               
//                    if(clock_m_buf[i][COLCK_ORIGIN_START_INDEX + j] == clock_bg_color[style] || clock_h_buf[i][j] == clock_pointer_color[style_is_red])
//                    {
//                        clock_m_buf[i][COLCK_ORIGIN_START_INDEX + j] = clock_h_buf[i][j];
//                    }   
//                }            
//            }            
//        }
//                
//        LCD_SetPosition(clock_coord[clock_m_postion - 1][0], clock_coord[clock_m_postion - 1][1], COLCK_POINTER_SIZE, COLCK_POINTER_SIZE);
//        LCD_DriverWriteDataBuf((uint8_t *)clock_m_buf, COLCK_POINTER_BUF_SIZE);  

//        if(clock_h_postion == 1)
//        {
//            for(uint8_t i = 0; i < COLCK_ORIGIN_SIZE ; i++)
//            {
//                memcpy(&clock_origin_h_buf[i][0], &clock_h_buf[COLCK_ORIGIN_START_INDEX + i][0], COLCK_ORIGIN_SIZE * 2);
//            }
//        }
//        else if(clock_h_postion == 2)
//        {
//            for(uint8_t i = 0; i < COLCK_ORIGIN_SIZE ; i++)
//            {
//                memcpy(&clock_origin_h_buf[i][0], &clock_h_buf[i][0], COLCK_ORIGIN_SIZE * 2);
//            }
//        }
//        else if(clock_h_postion == 3)
//        {
//            for(uint8_t i = 0; i < COLCK_ORIGIN_SIZE ; i++)
//            {
//                memcpy(&clock_origin_h_buf[i][0], &clock_h_buf[i][COLCK_ORIGIN_START_INDEX], COLCK_ORIGIN_SIZE * 2);
//            }
//        }
//        else if(clock_h_postion == 4)
//        {
//            for(uint8_t i = 0; i < COLCK_ORIGIN_SIZE ; i++)
//            {
//                memcpy(&clock_origin_h_buf[i][0], &clock_h_buf[COLCK_ORIGIN_START_INDEX + i][COLCK_ORIGIN_START_INDEX], COLCK_ORIGIN_SIZE * 2);
//            }
//        }
//        
//        if(clock_m_postion == 1)
//        {
//            for(uint8_t i = 0; i < COLCK_ORIGIN_SIZE ; i++)
//            {
//                memcpy(&clock_origin_m_buf[i][0], &clock_m_buf[COLCK_ORIGIN_START_INDEX + i][0], COLCK_ORIGIN_SIZE * 2);
//            }
//        }
//        else if(clock_m_postion == 2)
//        {
//            for(uint8_t i = 0; i < COLCK_ORIGIN_SIZE ; i++)
//            {
//                memcpy(&clock_origin_m_buf[i][0], &clock_m_buf[i][0], COLCK_ORIGIN_SIZE * 2);
//            }
//        }
//        else if(clock_m_postion == 3)
//        {
//            for(uint8_t i = 0; i < COLCK_ORIGIN_SIZE ; i++)
//            {
//                memcpy(&clock_origin_m_buf[i][0], &clock_m_buf[i][COLCK_ORIGIN_START_INDEX], COLCK_ORIGIN_SIZE * 2);
//            }
//        }
//        else if(clock_m_postion == 4)
//        {
//            for(uint8_t i = 0; i < COLCK_ORIGIN_SIZE ; i++)
//            {
//                memcpy(&clock_origin_m_buf[i][0], &clock_m_buf[COLCK_ORIGIN_START_INDEX + i][COLCK_ORIGIN_START_INDEX], COLCK_ORIGIN_SIZE * 2);
//            }
//        }   
//        for(uint8_t i = 0; i < COLCK_ORIGIN_SIZE ; i++)
//        {
//            for(uint8_t j = 0; j < COLCK_ORIGIN_SIZE ; j++)
//            {
//                if( clock_origin_h_buf[i][j] == clock_pointer_color[style_is_red] || clock_origin_m_buf[i][ j] == clock_bg_color[style])
//                {
//                    clock_origin_m_buf[i][j] = clock_origin_h_buf[i][j];
//                }  
//            }
//        } 
//        LCD_SetPosition(clock_coord[1][0], clock_coord[1][1], COLCK_ORIGIN_SIZE, COLCK_ORIGIN_SIZE);
//        LCD_DriverWriteDataBuf((uint8_t *)clock_origin_m_buf, COLCK_ORIGIN_SIZE * COLCK_ORIGIN_SIZE * 2);          
//    }           
//}

