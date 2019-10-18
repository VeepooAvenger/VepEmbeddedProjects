/***************************Copyright(c)***********************************
**                       Veepoo Tech
**                  COPYRIGHT (C) 2016-2019
**-----------File Info-----------------------------------------------------
** File        name:     gui_flash_param.h
** Last modified Date:   2017-08-09 
** Last SYS_VERSION:     0.0.0
** Descriptions:            
**-------------------------------------------------------------------------
** Created  by:          YX
** Created  date:        2017-08-09
** SYS_VERSION:          0.0.0
** Descriptions:         
**-------------------------------------------------------------------------
** Modified by:				
** Modified date:  			          
** SYS_VERSION:                 
** Descriptions:                        
***************************************************************************/

#include <stdint.h>
#include "gui_flash_addr.h"

/* 主界面 */
const uint32_t icon_16_main_clock_bg_addr[1][2] = 
{
    {
        ICON_MAIN3_ICON_ADDR, ICON_MAIN4_ICON_ADDR,
    }
};
const uint8_t icon_16_main_clock_bg_coord[1][2] = {{0, 0}};

const uint8_t icon_16_main3_date_coord[4][2] = {{17, 23},{26, 23},{44, 23},{53, 23}};

const uint32_t icon_16_main3_date_point_addr[1][1] = {ICON_MAIN3_ICON3_ADDR};
const uint8_t icon_16_main3_date_point_coord[1][2] = {{36, 22}};

const uint32_t icon_16_main_by_addr[2][2] = 
{
    {
        ICON_MAIN1_ICON6_BACKGROUND_1_ADDR, ICON_MAIN1_ICON6_BACKGROUND_2_ADDR
    },
    {
        ICON_MAIN2_ICON6_BACKGROUND_1_ADDR, ICON_MAIN2_ICON6_BACKGROUND_2_ADDR
    }
};
const uint8_t icon_16_main_by_coord[2][2] = {{0, 0}, {0, 142}};

const uint32_t icon_16_main_blue_addr[2][2] = 
{
    {
        ICON_MAIN1_ICON3_BLUETOOTH_NOT_ADDR, ICON_MAIN1_ICON3_BLUETOOTH_ADDR
    },
    {
        ICON_MAIN2_ICON3_BLUETOOTH_NOT_ADDR, ICON_MAIN2_ICON3_BLUETOOTH_ADDR
    },
};
const uint8_t icon_16_main_blue_coord[1][2] = {{0, 3}};

//const uint32_t icon_16_main_chg_addr[1][1] = 
//{
//    {
//        ICON_MAIN_ICON4_LIGHTING_ADDR
//    },
//};
//const uint8_t icon_16_main_chg_coord[1][2] = {{50, 2}};

const uint32_t icon_16_main_battery_addr[4][1] = 
{
    {
        ICON_MAIN1_ICON2_BATTERY_ADDR
    },
    {
        ICON_MAIN2_ICON2_BATTERY_ADDR
    },
    {
        ICON_MAIN1_ICON7_BATTERY_BG_ADDR
    },
    {
        ICON_MAIN2_ICON7_BATTERY_BG_ADDR
    }
};
const uint8_t icon_16_main_battery_coord[1][2] = {{59, 4}};

const uint32_t icon_16_main_battery2_addr[2][1] = 
{ 
    {
        ICON_MAIN1_ICON5_BATTERY_ADDR
    },
    {
        ICON_MAIN2_ICON5_BATTERY_ADDR
    },
};
const uint8_t icon_16_main_battery2_coord[1][2] = {{61, 6}};

const uint32_t icon_16_main_ampm_addr[3][4] = 
{
    {
        ICON_MAIN1_ICON1_AM_CN_ADDR, ICON_MAIN1_ICON1_PM_CN_ADDR, ICON_MAIN1_ICON1_AM_EN_ADDR, ICON_MAIN1_ICON1_PM_EN_ADDR
    },
    {
        ICON_MAIN2_ICON1_AM_CN_ADDR, ICON_MAIN2_ICON1_PM_CN_ADDR, ICON_MAIN2_ICON1_AM_EN_ADDR, ICON_MAIN2_ICON1_PM_EN_ADDR
    },
    {
        ICON_MAIN1_ICON1_BG_ADDR, ICON_MAIN2_ICON1_BG_ADDR, 0, 0
    },

};
const uint8_t icon_16_main_ampm_coord[1][2] = {{17, 2}};

const uint32_t icon_16_main_hour_addr[2][10] = 
{
    {
        ICON_MAIN1_DATA3_TIME_0_ADDR, ICON_MAIN1_DATA3_TIME_1_ADDR, ICON_MAIN1_DATA3_TIME_2_ADDR, ICON_MAIN1_DATA3_TIME_3_ADDR, ICON_MAIN1_DATA3_TIME_4_ADDR,
        ICON_MAIN1_DATA3_TIME_5_ADDR, ICON_MAIN1_DATA3_TIME_6_ADDR, ICON_MAIN1_DATA3_TIME_7_ADDR, ICON_MAIN1_DATA3_TIME_8_ADDR, ICON_MAIN1_DATA3_TIME_9_ADDR
    },
    {
        ICON_MAIN2_DATA3_TIME_0_ADDR, ICON_MAIN2_DATA3_TIME_1_ADDR, ICON_MAIN2_DATA3_TIME_2_ADDR, ICON_MAIN2_DATA3_TIME_3_ADDR, ICON_MAIN2_DATA3_TIME_4_ADDR,
        ICON_MAIN2_DATA3_TIME_5_ADDR, ICON_MAIN2_DATA3_TIME_6_ADDR, ICON_MAIN2_DATA3_TIME_7_ADDR, ICON_MAIN2_DATA3_TIME_8_ADDR, ICON_MAIN2_DATA3_TIME_9_ADDR
    }
};

const uint32_t icon_16_main_minute_addr[1][10] = 
{
    {
        ICON_MAIN_DATA3_TIME_0_ADDR, ICON_MAIN_DATA3_TIME_1_ADDR, ICON_MAIN_DATA3_TIME_2_ADDR, ICON_MAIN_DATA3_TIME_3_ADDR, ICON_MAIN_DATA3_TIME_4_ADDR,
        ICON_MAIN_DATA3_TIME_5_ADDR, ICON_MAIN_DATA3_TIME_6_ADDR, ICON_MAIN_DATA3_TIME_7_ADDR, ICON_MAIN_DATA3_TIME_8_ADDR, ICON_MAIN_DATA3_TIME_9_ADDR
    }
};

//const uint32_t icon_16_main3_time_addr[2][10] = 
//{
//{
//    0x00,
//},
//};

const uint8_t icon_16_main_time_coord[2][4][2] = 
{
    {
        {2, 29}, {42, 29}, {2, 85}, {42, 85}
    },
    {
        {2, 21}, {42, 21}, {2, 70}, {42, 70}
    }
};

const uint32_t icon_16_main_date_addr[3][10] = 
{
    {
    ICON_MAIN1_DATA1_DATE_0_ADDR, ICON_MAIN1_DATA1_DATE_1_ADDR, ICON_MAIN1_DATA1_DATE_2_ADDR, ICON_MAIN1_DATA1_DATE_3_ADDR, ICON_MAIN1_DATA1_DATE_4_ADDR,
    ICON_MAIN1_DATA1_DATE_5_ADDR, ICON_MAIN1_DATA1_DATE_6_ADDR, ICON_MAIN1_DATA1_DATE_7_ADDR, ICON_MAIN1_DATA1_DATE_8_ADDR, ICON_MAIN1_DATA1_DATE_9_ADDR
    },
    {
    ICON_MAIN2_DATA1_DATE_0_ADDR, ICON_MAIN2_DATA1_DATE_1_ADDR, ICON_MAIN2_DATA1_DATE_2_ADDR, ICON_MAIN2_DATA1_DATE_3_ADDR, ICON_MAIN2_DATA1_DATE_4_ADDR,
    ICON_MAIN2_DATA1_DATE_5_ADDR, ICON_MAIN2_DATA1_DATE_6_ADDR, ICON_MAIN2_DATA1_DATE_7_ADDR, ICON_MAIN2_DATA1_DATE_8_ADDR, ICON_MAIN2_DATA1_DATE_9_ADDR
    },
    {
    ICON_MAIN3_DATA_DATE_0_ADDR, ICON_MAIN3_DATA_DATE_1_ADDR, ICON_MAIN3_DATA_DATE_2_ADDR, ICON_MAIN3_DATA_DATE_3_ADDR, ICON_MAIN3_DATA_DATE_4_ADDR,
    ICON_MAIN3_DATA_DATE_5_ADDR, ICON_MAIN3_DATA_DATE_6_ADDR, ICON_MAIN3_DATA_DATE_7_ADDR, ICON_MAIN3_DATA_DATE_8_ADDR, ICON_MAIN3_DATA_DATE_9_ADDR
    }
};
const uint8_t icon_16_main_date_coord[2][4][2] = 
{
    {
        {0, 146}, {8, 146}, {20, 146}, {28, 146}
    },
    {
        {0, 146}, {8, 146}, {20, 146}, {28, 146}
    }
};

const uint32_t icon_16_main_date_point_addr[2][1] = 
{
    {
        ICON_MAIN1_DATA2_DATE_DIAN_ADDR
    }, 
    {
        ICON_MAIN2_DATA2_DATE_DIAN_ADDR
    }
};
const uint8_t icon_16_main_date_point_coord[2][2] = {{17, 153},{17, 153}};

const uint32_t icon_16_main_week_addr[2][2][7] = 
{
	{
        ICON_MAIN1_DATA4_WEEK_CN_1_ADDR, ICON_MAIN1_DATA4_WEEK_CN_2_ADDR, ICON_MAIN1_DATA4_WEEK_CN_3_ADDR, ICON_MAIN1_DATA4_WEEK_CN_4_ADDR, ICON_MAIN1_DATA4_WEEK_CN_5_ADDR,
        ICON_MAIN1_DATA4_WEEK_CN_6_ADDR, ICON_MAIN1_DATA4_WEEK_CN_7_ADDR, ICON_MAIN1_DATA4_WEEK_EN_1_ADDR, ICON_MAIN1_DATA4_WEEK_EN_2_ADDR, ICON_MAIN1_DATA4_WEEK_EN_3_ADDR,
        ICON_MAIN1_DATA4_WEEK_EN_4_ADDR, ICON_MAIN1_DATA4_WEEK_EN_5_ADDR, ICON_MAIN1_DATA4_WEEK_EN_6_ADDR, ICON_MAIN1_DATA4_WEEK_EN_7_ADDR
    },
    {
        ICON_MAIN2_DATA4_WEEK_CN_1_ADDR, ICON_MAIN2_DATA4_WEEK_CN_2_ADDR, ICON_MAIN2_DATA4_WEEK_CN_3_ADDR, ICON_MAIN2_DATA4_WEEK_CN_4_ADDR, ICON_MAIN2_DATA4_WEEK_CN_5_ADDR,
        ICON_MAIN2_DATA4_WEEK_CN_6_ADDR, ICON_MAIN2_DATA4_WEEK_CN_7_ADDR, ICON_MAIN2_DATA4_WEEK_EN_1_ADDR, ICON_MAIN2_DATA4_WEEK_EN_2_ADDR, ICON_MAIN2_DATA4_WEEK_EN_3_ADDR,
        ICON_MAIN2_DATA4_WEEK_EN_4_ADDR, ICON_MAIN2_DATA4_WEEK_EN_5_ADDR, ICON_MAIN2_DATA4_WEEK_EN_6_ADDR, ICON_MAIN2_DATA4_WEEK_EN_7_ADDR
    },
};
const uint8_t icon_16_main_week_coord[1][2] = {{48, 145}};

//女性经期图标
const uint32_t icon_16_menstrual_icon_addr[2][5] = 
{
    {
    ICON_MAIN1_ICON4_BG_ADDR, ICON_MAIN1_ICON4_1_ADDR, ICON_MAIN1_ICON4_2_ADDR,ICON_MAIN1_ICON4_3_ADDR,ICON_MAIN1_ICON4_4_ADDR
    },
    {
    ICON_MAIN2_ICON4_BG_ADDR, ICON_MAIN2_ICON4_1_ADDR, ICON_MAIN2_ICON4_2_ADDR,ICON_MAIN2_ICON4_3_ADDR,ICON_MAIN2_ICON4_4_ADDR  
    }
};
const uint8_t icon_16_menstrual_icon_coord[1][2] = {{42 , 4}};//10

//天气
const uint32_t icon_16_weather_icon_addr[1][8] = 
{
    {
    ICON_MAIN_ICON_WEATHER_1_ADDR, ICON_MAIN_ICON_WEATHER_2_ADDR, ICON_MAIN_ICON_WEATHER_3_ADDR, ICON_MAIN_ICON_WEATHER_4_ADDR,
    ICON_MAIN_ICON_WEATHER_5_ADDR, ICON_MAIN_ICON_WEATHER_6_ADDR, ICON_MAIN_ICON_WEATHER_7_ADDR, ICON_MAIN_ICON_WEATHER_8_ADDR
    }
};
const uint8_t icon_16_weather_icon_coord[7][2] = 
{
    {17 , 122}, {17, 122},{12 , 122}, 
    {10, 122},{10 , 122}, {7, 122},{5, 122}
};

//度
const uint32_t icon_16_weather_degree_icon_addr[1][1] = 
{
    {ICON_MAIN_DATA7_DIAN_ADDR}
};
const uint8_t icon_16_weather_degree_icon_coord[1][2] = {{7 , 124}};//10

//~
const uint32_t icon_16_weather_line_icon_addr[1][1] = 
{
    {ICON_MAIN_DATA6_LINE_ADDR}
};
const uint8_t icon_16_weather_line_icon_coord[1][2] = {{7 , 122}};//10

//数字
const uint32_t icon_16_weather_data_icon_addr[1][10] = 
{
    {
ICON_MAIN_DATA5_WEATHER_0_ADDR, ICON_MAIN_DATA5_WEATHER_1_ADDR, ICON_MAIN_DATA5_WEATHER_2_ADDR, ICON_MAIN_DATA5_WEATHER_3_ADDR, ICON_MAIN_DATA5_WEATHER_4_ADDR,
ICON_MAIN_DATA5_WEATHER_5_ADDR, ICON_MAIN_DATA5_WEATHER_6_ADDR, ICON_MAIN_DATA5_WEATHER_7_ADDR, ICON_MAIN_DATA5_WEATHER_8_ADDR, ICON_MAIN_DATA5_WEATHER_9_ADDR
    }
};
const uint8_t icon_16_weather_data_icon_coord[7][2] = 
{
{44,127}, {44,127}, {35,127}, {32,127}, {30,127}, {27,127},{25 , 127}
};




/* 功能界面的数字 */
const uint32_t icon_16_function1_data_addr[1] = 
{
    ICON_FUNCTION_DATA_TIME0_ADDR
};

/* 计步 */
const uint32_t icon_16_step_icon_addr[3] = {ICON_STEP_ICON_1_ADDR, ICON_STEP_ICON_2_ADDR, ICON_STEP_ICON_3_ADDR};
const uint8_t icon_16_step_icon_coord[1][2] = {{14 , 2}};//10

const uint32_t icon_16_step_icon2_addr[1] = {ICON_STEP_ICON2_ADDR};
const uint8_t icon_16_step_icon2_coord[1][2] = {{0 , 102}};//98

const uint32_t icon_16_step_icon3_addr[1] = {ICON_STEP_ICON3_ADDR};
const uint8_t icon_16_step_icon3_coord[5][2] = {{0 , 102}, {16 , 102}, {32 , 102}, {48 , 102}, {64 , 102}};//98

const uint32_t icon_16_step_unit_addr[2] = {ICON_STEP_UNIT_CN_ADDR, ICON_STEP_UNIT_EN_ADDR};
const uint8_t icon_16_step_unit_coord[1][2] = {{24, 143}};//69

const uint8_t icon_16_step_data_coord[2][2] = {{1, 65}, {1,101}};//间隔为1 //80

const uint32_t icon_16_step_data_addr[10] = 
{
    ICON_STEP_DATA_TIME0_ADDR, ICON_STEP_DATA_TIME1_ADDR, ICON_STEP_DATA_TIME2_ADDR, ICON_STEP_DATA_TIME3_ADDR, ICON_STEP_DATA_TIME4_ADDR,
    ICON_STEP_DATA_TIME5_ADDR, ICON_STEP_DATA_TIME6_ADDR, ICON_STEP_DATA_TIME7_ADDR, ICON_STEP_DATA_TIME8_ADDR, ICON_STEP_DATA_TIME9_ADDR,
};


/* 距离 */
const uint32_t icon_16_distance_static_addr[2] = {ICON_DISTANCE_ICON1_1_ADDR, ICON_DISTANCE_ICON1_2_ADDR};
const uint8_t icon_16_distance_static_coord[1][2] = {{14, 2}};//10
    
const uint32_t icon_16_distance_unit_addr[3] = 
{
	ICON_DISTANCE_UNIT_KM_CN_ADDR, ICON_DISTANCE_UNIT_KM_EN_ADDR, ICON_DISTANCE_UNIT_MI_ADDR
};
const uint8_t icon_16_distance_unit_coord[1][2] = {{26, 145}};//69

const uint32_t icon_16_distance_point_addr[1] = {ICON_DISTANCE_ICON3_DIAN_ADDR};
const uint8_t icon_16_distance_point_coord[1][2] = {{52, 108}};//90

const uint8_t icon_16_distance_data_coord[1][2] = {{1, 80}};//间隔3 80
const uint32_t icon_16_distance_data_addr[10] = 
{
    ICON_DISTANCE_DATA_TIME0_ADDR, ICON_DISTANCE_DATA_TIME1_ADDR, ICON_DISTANCE_DATA_TIME2_ADDR, ICON_DISTANCE_DATA_TIME3_ADDR, ICON_DISTANCE_DATA_TIME4_ADDR,
    ICON_DISTANCE_DATA_TIME5_ADDR, ICON_DISTANCE_DATA_TIME6_ADDR, ICON_DISTANCE_DATA_TIME7_ADDR, ICON_DISTANCE_DATA_TIME8_ADDR, ICON_DISTANCE_DATA_TIME9_ADDR,
};

/* 卡路里 */
const uint32_t icon_16_calories_icon_addr[3] = {ICON_CALORIES_ICON_1_ADDR, ICON_CALORIES_ICON_2_ADDR, ICON_CALORIES_ICON_3_ADDR};
const uint8_t icon_16_calories_icon_coord[1][2] = {{14, 2}};//10

const uint32_t icon_16_calories_unit_addr[2] = {ICON_CALORIES_UNIT_CN_ADDR, ICON_CALORIES_UNIT_EN_ADDR};
const uint8_t icon_16_calories_unit_coord[1][2] = {{26, 144}};//69

const uint8_t icon_16_calories_data_coord[2][2] = {{1, 65},{1,101}};//间隔1 80
const uint32_t icon_16_calories_data_addr[10] = 
{
    ICON_CALORIES_DATA_TIME0_ADDR, ICON_CALORIES_DATA_TIME1_ADDR, ICON_CALORIES_DATA_TIME2_ADDR, ICON_CALORIES_DATA_TIME3_ADDR, ICON_CALORIES_DATA_TIME4_ADDR,
    ICON_CALORIES_DATA_TIME5_ADDR, ICON_CALORIES_DATA_TIME6_ADDR, ICON_CALORIES_DATA_TIME7_ADDR, ICON_CALORIES_DATA_TIME8_ADDR, ICON_CALORIES_DATA_TIME9_ADDR,
};


/* 运动模式 */
const uint32_t icon_16_move_icon_colon_addr[1] = {ICON_MOVE2_ICON_COLON_ADDR};

const uint32_t icon_16_move_icon2_addr[2] = {ICON_MOVE2_ICON3_1_ADDR, ICON_MOVE2_ICON3_2_ADDR};
const uint8_t icon_16_move_icon2_coord[3][2] = {{30, 5}, {38, 5}, {46, 5}};

const uint32_t icon_16_move1_icon_addr[10] = 
{
ICON_MOVE1_ICON_1_ADDR, ICON_MOVE1_ICON_2_ADDR, ICON_MOVE1_ICON_3_ADDR,ICON_MOVE1_ICON_4_ADDR, ICON_MOVE1_ICON_5_ADDR,
ICON_MOVE1_ICON_6_ADDR, ICON_MOVE1_ICON_7_ADDR, ICON_MOVE1_ICON_8_ADDR,ICON_MOVE1_ICON_9_ADDR, ICON_MOVE1_ICON_10_ADDR,
};
const uint8_t icon_16_move1_icon_coord[1][2] = {{11, 25}};//11,29

const uint32_t icon_16_move1_icon2_addr[1] = {ICON_MOVE1_ICON2_ADDR};
const uint8_t icon_16_move1_icon2_coord[1][2] = {{36, 142}};//140

const uint32_t icon_16_move2_icon1_addr[3] = {ICON_MOVE2_ICON1_1_ADDR, ICON_MOVE2_ICON1_2_ADDR, ICON_MOVE2_ICON1_3_ADDR};
const uint8_t icon_16_move2_icon1_coord[3][2] = {{1, 54}, {1, 90}, {1, 133},};

const uint32_t icon_16_move2_icon2_line_addr[1] = {ICON_MOVE2_ICON2_LINE_ADDR};
const uint8_t icon_16_move2_icon2_line_coord[3][2] = {{0, 44}, {0, 83}, {0, 122}};

const uint8_t icon_16_move2_data_coord[3][2] = {{21, 58}, {21, 96}, {21, 135}};//间隔

const uint8_t icon_16_move2_data2_coord[6][2] = {{0, 14}, {12, 14}, {28, 14}, {40, 14}, {56, 14}, {68, 14}};//间隔2

const uint8_t icon_16_move2_icon2_colon_coord[2][2] = {{24, 18}, {52, 18}};

const uint32_t icon_16_move3_icon_addr[1] = {ICON_MOVE3_ICON_ADDR};
const uint8_t icon_16_move3_icon_coord[1][2] = {{10, 50}};

const uint32_t icon_16_move4_icon_addr[1] = {ICON_MOVE4_ICON_ADDR};
const uint8_t icon_16_move4_icon_coord[1][2] = {{10, 50}};

const uint32_t icon_16_move5_icon_addr[1] = {ICON_MOVE5_ICON_ADDR};
const uint8_t icon_16_move5_icon_coord[1][2] = {{10, 50}};

const uint32_t icon_16_move2_time_data_addr[10] = 
{
    ICON_MOVE2_DATA1_TIME_0_ADDR, ICON_MOVE2_DATA1_TIME_1_ADDR, ICON_MOVE2_DATA1_TIME_2_ADDR, ICON_MOVE2_DATA1_TIME_3_ADDR,ICON_MOVE2_DATA1_TIME_4_ADDR, 
    ICON_MOVE2_DATA1_TIME_5_ADDR, ICON_MOVE2_DATA1_TIME_6_ADDR, ICON_MOVE2_DATA1_TIME_7_ADDR, ICON_MOVE2_DATA1_TIME_8_ADDR,ICON_MOVE2_DATA1_TIME_9_ADDR,
};

const uint32_t icon_16_move2_heart_data_addr[10] = 
{
    ICON_MOVE2_DATA2_HEART_0_ADDR, ICON_MOVE2_DATA2_HEART_1_ADDR, ICON_MOVE2_DATA2_HEART_2_ADDR, ICON_MOVE2_DATA2_HEART_3_ADDR,ICON_MOVE2_DATA2_HEART_4_ADDR, 
    ICON_MOVE2_DATA2_HEART_5_ADDR, ICON_MOVE2_DATA2_HEART_6_ADDR, ICON_MOVE2_DATA2_HEART_7_ADDR, ICON_MOVE2_DATA2_HEART_8_ADDR,ICON_MOVE2_DATA2_HEART_9_ADDR,
};

const uint32_t icon_16_move2_calories_data_addr[10] = 
{
    ICON_MOVE2_DATA3_CALORIES_0_ADDR, ICON_MOVE2_DATA3_CALORIES_1_ADDR, ICON_MOVE2_DATA3_CALORIES_2_ADDR, ICON_MOVE2_DATA3_CALORIES_3_ADDR,ICON_MOVE2_DATA3_CALORIES_4_ADDR, 
    ICON_MOVE2_DATA3_CALORIES_5_ADDR, ICON_MOVE2_DATA3_CALORIES_6_ADDR, ICON_MOVE2_DATA3_CALORIES_7_ADDR, ICON_MOVE2_DATA3_CALORIES_8_ADDR,ICON_MOVE2_DATA3_CALORIES_9_ADDR,
};

const uint32_t icon_16_move2_step_data_addr[10] = 
{
    ICON_MOVE2_DATA4_STEP_0_ADDR, ICON_MOVE2_DATA4_STEP_1_ADDR, ICON_MOVE2_DATA4_STEP_2_ADDR, ICON_MOVE2_DATA4_STEP_3_ADDR,ICON_MOVE2_DATA4_STEP_4_ADDR, 
    ICON_MOVE2_DATA4_STEP_5_ADDR, ICON_MOVE2_DATA4_STEP_6_ADDR, ICON_MOVE2_DATA4_STEP_7_ADDR, ICON_MOVE2_DATA4_STEP_8_ADDR,ICON_MOVE2_DATA4_STEP_9_ADDR,
};


/* 血压 */
const uint32_t icon_16_bp_icon_static_addr[1] = {0x00,};
const uint8_t icon_16_bp_icon_static_coord[1][2] = {{15, 14}};//10

const uint32_t icon_16_bp_icon_dynamic_addr[5] = {ICON_BP_ICON1_1_ADDR, ICON_BP_ICON1_2_ADDR, ICON_BP_ICON1_3_ADDR, ICON_BP_ICON1_4_ADDR, ICON_BP_ICON1_5_ADDR};
const uint8_t icon_16_bp_icon_dynamic_coord[1][2] = {{14, 2}};//27

const uint32_t icon_16_bp_icon_slash_addr[1] = {ICON_BP_ICON3_ADDR};
const uint8_t icon_16_bp_icon_slash_coord[1][2] = {{6, 106}};//74

const uint32_t icon_16_bp_icon_bar_addr[1] = {ICON_BP_ICON4_ADDR};
const uint8_t icon_16_bp_icon_bar_coord[6][2] = {{10, 83}, {33, 83}, {56, 83}, {10, 115}, {33, 115},{56, 115}};//85

const uint32_t icon_16_bp_icon_x_addr[1] = {ICON_BP_ICON5_ADDR};
const uint8_t icon_16_bp_icon_x_coord[6][2] = {{10, 78}, {34, 78}, {58, 78}, {10, 108}, {34, 108},{58,108}};//82

//const uint32_t icon_16_bp_icon_p_addr[1] = {ICON_BP_ICON6_ADDR};
const uint8_t icon_16_bp_icon_p_coord[1][2] = {{41, 2}};//23

const uint32_t icon_16_bp_unit_addr[1] = {ICON_BP_UNIT_ADDR};
const uint8_t icon_16_bp_unit_coord[1][2] = {{16, 143}};//69

const uint8_t icon_16_bp_data_coord[2][2] = {{1, 64},{19, 107}};//间隔为1  80
const uint32_t icon_16_bp_data_addr[10] = 
{
    ICON_BP_DATA1_0_ADDR, ICON_BP_DATA1_1_ADDR, ICON_BP_DATA1_2_ADDR, ICON_BP_DATA1_3_ADDR, ICON_BP_DATA1_4_ADDR,
    ICON_BP_DATA1_5_ADDR, ICON_BP_DATA1_6_ADDR, ICON_BP_DATA1_7_ADDR, ICON_BP_DATA1_8_ADDR, ICON_BP_DATA1_9_ADDR,
};
const uint32_t icon_16_bp_low_data_addr[10] = 
{
    ICON_BP_DATA2_0_ADDR, ICON_BP_DATA2_1_ADDR, ICON_BP_DATA2_2_ADDR, ICON_BP_DATA2_3_ADDR, ICON_BP_DATA2_4_ADDR,
    ICON_BP_DATA2_5_ADDR, ICON_BP_DATA2_6_ADDR, ICON_BP_DATA2_7_ADDR, ICON_BP_DATA2_8_ADDR, ICON_BP_DATA2_9_ADDR,
};

/* 心率 */
const uint32_t icon_16_heart_icon1_addr[2] = {0x00,};
const uint8_t icon_16_heart_icon1_coord[1][2] = {{15, 14}};//10

const uint32_t icon_16_heart_icon2_addr[3] = 
{
    ICON_HEART_ICON_1_ADDR, ICON_HEART_ICON_2_ADDR, ICON_HEART_ICON_3_ADDR
};
const uint8_t icon_16_heart_icon2_coord[1][2] = {{14, 2}};//29

const uint32_t icon_16_heart_unit_addr[1] = {ICON_HEART_UNIT_ADDR};
const uint8_t icon_16_heart_unit_coord[1][2] = {{24, 143}};//69

const uint8_t icon_16_heart_data_coord[1][2] = {{1, 80}};//间隔为4 80


/* 睡眠 */
const uint32_t icon_16_sleep_icon_addr[4] =
{
	ICON_SLEEP_ICON_1_ADDR, ICON_SLEEP_ICON_2_ADDR, ICON_SLEEP_ICON_3_ADDR, ICON_SLEEP_ICON_4_ADDR
};
const uint8_t icon_16_sleep_icon_coord[1][2] = {{14, 2}};//10

const uint32_t icon_16_sleep_unit_addr[2] = {ICON_SLEEP_UNIT_CN_ADDR, ICON_SLEEP_UNIT_EN_ADDR};
const uint8_t icon_16_sleep_unit_coord[1][2] = {{20, 144}};

const uint32_t icon_16_sleep_point_addr[1] = {ICON_SLEEP_ICON2_DIAN_ADDR};
const uint8_t icon_16_sleep_point_coord[1][2] = {{52, 108}};//89

const uint8_t icon_16_sleep_data_coord[1][2] = {{1, 80}};//间隔为4 80
const uint32_t icon_16_sleep_data_addr[10] = 
{
    ICON_SLEEP_DATA_TIME0_ADDR, ICON_SLEEP_DATA_TIME1_ADDR, ICON_SLEEP_DATA_TIME2_ADDR, ICON_SLEEP_DATA_TIME3_ADDR, ICON_SLEEP_DATA_TIME4_ADDR,
    ICON_SLEEP_DATA_TIME5_ADDR, ICON_SLEEP_DATA_TIME6_ADDR, ICON_SLEEP_DATA_TIME7_ADDR, ICON_SLEEP_DATA_TIME8_ADDR, ICON_SLEEP_DATA_TIME9_ADDR,
};

/* 找手机 */
//const uint32_t icon_16_findphone_static_addr[1] = {0x00,};
//const uint8_t icon_16_findphone_static_coord[1][2] = {{27, 55}};

const uint32_t icon_16_findphone_dynamic_addr[3] = {ICON_FINDPHONE_ICON_1_ADDR, ICON_FINDPHONE_ICON_2_ADDR, ICON_FINDPHONE_ICON_3_ADDR};
const uint8_t icon_16_findphone_dynamic_coord[1][2] = {{10, 50}};

/* 秒表 */
const uint32_t icon_16_stopwatch_icon_addr[1] = {ICON_STOPWATCH_ICON_ADDR};
const uint8_t icon_16_stopwatch_icon_coord[1][2] = {{10, 50}};

const uint32_t icon_16_stopwatch_data_addr[10] = 
{0x00,
};

const uint8_t icon_16_stopwatch_data_coord[6][2] = 
{
	{3, 3}, {42, 3},
	{3, 57}, {42, 57},
	{3, 111}, {42, 111}
};



/* 倒计时 */
const uint32_t icon_16_countdown_icon_addr[3] = 
{
	ICON_COUNTDOWN_ICON_1_ADDR, ICON_COUNTDOWN_ICON_2_ADDR, ICON_COUNTDOWN_ICON_3_ADDR,
};
const uint8_t icon_16_countdown_icon_coord[1][2] = {{10, 50}};


/* 关机 */
const uint32_t icon_16_poweroff_icon_addr[1] = {ICON_POWEROFF_ICON_ADDR};
const uint8_t icon_16_poweroff_icon_coord[1][2] = {{10, 50}};


/* 充电 */
const uint32_t icon_16_chg_bar_addr[5] = 
{
	ICON_CHG_ICON2_1_ADDR, ICON_CHG_ICON2_2_ADDR, ICON_CHG_ICON2_3_ADDR, ICON_CHG_ICON2_4_ADDR, ICON_CHG_ICON2_5_ADDR
};
const uint8_t icon_16_chg_bar_coord[1][2] = {{32, 63},};

const uint32_t icon_16_chg_icon_addr[1] = {ICON_CHG_ICON1_ADDR};
const uint8_t icon_16_chg_icon_coord[1][2] = {{10, 50}};//{{28, 58}};


/* 闹钟 */
const uint32_t icon_16_clock_ampm_addr[4] = 
{
	ICON_CLOCK_ICON2_AM_CN_ADDR, ICON_CLOCK_ICON2_PM_CN_ADDR, ICON_CLOCK_ICON2_AM_EN_ADDR, ICON_CLOCK_ICON2_PM_EN_ADDR
};
const uint8_t icon_16_clock_ampm_coord[1][2] = {{26, 144}};

const uint32_t icon_16_clock_icon_addr[2] = {ICON_CLOCK_ICON_1_ADDR, ICON_CLOCK_ICON_2_ADDR};
const uint8_t icon_16_clock_icon_coord[1][2] = {{14, 2}};//10

const uint32_t icon_16_remindclock_icon_addr[20] = 
{
	ICON_REMINDCLOCK_ICON_1_ADDR, ICON_REMINDCLOCK_ICON_2_ADDR, ICON_REMINDCLOCK_ICON_3_ADDR, ICON_REMINDCLOCK_ICON_4_ADDR, ICON_REMINDCLOCK_ICON_5_ADDR,
	ICON_REMINDCLOCK_ICON_6_ADDR, ICON_REMINDCLOCK_ICON_7_ADDR, ICON_REMINDCLOCK_ICON_8_ADDR, ICON_REMINDCLOCK_ICON_9_ADDR, ICON_REMINDCLOCK_ICON_10_ADDR,
	ICON_REMINDCLOCK_ICON_11_ADDR, ICON_REMINDCLOCK_ICON_12_ADDR, ICON_REMINDCLOCK_ICON_13_ADDR, ICON_REMINDCLOCK_ICON_14_ADDR, ICON_REMINDCLOCK_ICON_15_ADDR,
	ICON_REMINDCLOCK_ICON_16_ADDR, ICON_REMINDCLOCK_ICON_17_ADDR, ICON_REMINDCLOCK_ICON_18_ADDR, ICON_REMINDCLOCK_ICON_19_ADDR, ICON_REMINDCLOCK_ICON_20_ADDR
};
const uint8_t icon_16_remindclock_icon_coord[1][2] = {{10, 50}};//53

const uint8_t icon_16_clock_data_coord[4][2] = {{14, 64}, {41, 64}, {14, 105}, {41, 105}};//80
//{{13, 107}, {26, 107}, {45, 107}, {58, 107}};

const uint32_t icon_16_clock_colon_addr[1] = {ICON_CLOCK_ICON3_COLON_ADDR};
const uint8_t icon_16_clock_colon_coord[1][2] = {{30, 98}};//83

//const uint8_t icon_16_aralm_data_coord[4][2] = {{14, 64},{41, 64},{14,105},{41,105}};//83

/* 久坐 */
const uint32_t icon_16_sit_icon_addr[2] = {ICON_SIT_ICON_1_ADDR, ICON_SIT_ICON_2_ADDR};
const uint8_t icon_16_sit_icon_coord[2][2] = {{10, 50}, {10, 50}};


/* 消息推送 */
const uint32_t icon_16_inform_icon_addr[14] = 
{
	ICON_INFORM_ICON_12_ADDR, ICON_INFORM_ICON_2_ADDR, ICON_INFORM_ICON_3_ADDR, ICON_INFORM_ICON_4_ADDR,
    ICON_INFORM_ICON_5_ADDR, ICON_INFORM_ICON_6_ADDR, ICON_INFORM_ICON_7_ADDR, ICON_INFORM_ICON_8_ADDR,
    ICON_INFORM_ICON_9_ADDR, ICON_INFORM_ICON_10_ADDR, ICON_INFORM_ICON_11_ADDR, ICON_INFORM_ICON_1_ADDR,
    ICON_INFORM_ICON_13_ADDR,ICON_INFORM_ICON_14_ADDR
};
const uint8_t icon_16_inform_icon_coord[1][2] = {{13, 8}};//10

const uint8_t icon_16_inform_text_coord[1][2] = {{3, 70}};

const uint32_t icon_16_call_static_addr[1] = {ICON_CALL_ICON2_ADDR};
const uint8_t icon_16_call_static_coord[1][2] = {{22, 43}};

const uint32_t icon_16_call_dynamic_addr[3] = {ICON_CALL_ICON3_1_ADDR, ICON_CALL_ICON3_2_ADDR, ICON_CALL_ICON3_3_ADDR};
const uint8_t icon_16_call_dynamic_coord[1][2] = {{38, 38}};

const uint32_t icon_16_call_hangup_addr[1] = {ICON_CALL_ICON1_ADDR};
const uint8_t icon_16_call_hangup_coord[1][2] = {{21, 39}};

const uint32_t icon_16_call_mute_addr[1] = {ICON_CALL_ICON4_ADDR};
const uint8_t icon_16_call_mute_coord[1][2] = {{38, 38}};

const uint8_t icon_16_call_text_coord[1][2] = {{0, 88}};


/* 心率报警 */
const uint32_t icon_16_heartalert_static_addr[1] = {ICON_HEARTALERT_ICON1_ADDR};
const uint8_t icon_16_heartalert_static_coord[1][2] = {{10, 50}};

const uint32_t icon_16_heartalert_dynamic_addr[2] = {ICON_HEARTALERT_ICON2_1_ADDR, ICON_HEARTALERT_ICON2_2_ADDR};
const uint8_t icon_16_heartalert_dynamic_coord[1][2] = {{38, 75}};


/* 相机 */
const uint32_t icon_16_camera_icon_addr[2] = {ICON_CAMERA_ICON_1_ADDR, ICON_CAMERA_ICON_2_ADDR};
const uint8_t icon_16_camera_icon_coord[1][2] = {{10, 50}};


/* 低电 */
const uint32_t icon_16_lowpower_static_addr[1] = {ICON_CHG_ICON1_ADDR,};
const uint8_t icon_16_lowpower_static_coord[1][2] = {{10, 50}};

const uint32_t icon_16_lowpower_dynamic_addr[2] = {ICON_CHG_ICON2_6_ADDR,ICON_CHG_ICON2_5_ADDR};
const uint8_t icon_16_lowpower_dynamic_coord[1][2] = {{32, 63}};

/* 运动达标 */
const uint32_t icon_16_target_icon_addr[2] = {ICON_TARGET_ICON_1_ADDR, ICON_TARGET_ICON_2_ADDR};
const uint8_t icon_16_target_icon_coord[1][2] = {{10, 50}};

// 蓝牙断联
const uint32_t icon_16_ble_remind_icon_addr[1] = {ICON_YDISCONNECTED_ICON_ADDR};
const uint8_t  icon_16_ble_remind_icon_coord[1][2] = {{10, 50}};


/*新增主界面5*/

//蓝牙
const uint32_t icon_16_main5_ble_icon_addr[2] = {ICON_MAIN5_ICON1_BLUETOOTH_NOT_ADDR, ICON_MAIN5_ICON1_BLUETOOTH_ADDR};
const uint8_t icon_16_main5_ble_icon_coord[1][2] = {{2,2}};

//AM/PM
const uint32_t icon_16_main5_ampm_icon_addr[2][2] = 
{
    {ICON_MAIN5_ICON2_AM_CN_ADDR, ICON_MAIN5_ICON2_PM_CN_ADDR},
    {ICON_MAIN5_ICON2_AM_EN_ADDR, ICON_MAIN5_ICON2_PM_EN_ADDR},
};
const uint8_t icon_16_main5_ampm_icon_coord[1][2] = {22,2};

//女性
const uint32_t icon_16_main5_menstrual_addr[5] = 
{
    ICON_MAIN5_ICON3_BG_ADDR, ICON_MAIN5_ICON3_1_ADDR, ICON_MAIN5_ICON3_2_ADDR, ICON_MAIN5_ICON3_3_ADDR, ICON_MAIN5_ICON3_4_ADDR
};
const uint8_t icon_16_main5_menstrual_coord[1][2] = {{ 44, 2}};

//电池电量
const uint32_t icon_16_main5_battery_addr[5] = 
{
    ICON_MAIN5_ICON4_BATTERY_1_ADDR, ICON_MAIN5_ICON4_BATTERY_2_ADDR,ICON_MAIN5_ICON4_BATTERY_3_ADDR, ICON_MAIN5_ICON4_BATTERY_4_ADDR, ICON_MAIN5_ICON4_BATTERY_5_ADDR
};
const uint8_t icon_16_main5_battery_coord[1][2] = {{ 59, 2}};

//低电
const uint32_t icon_16_main5_low_power_addr[1] = { ICON_MAIN5_ICON4_BATTERY_6_ADDR};
const uint8_t icon_16_main5_low_power_coord[1][2] = {{ 59, 2}};

//计步图标
const uint32_t icon_16_main5_step_icon_addr[1] = { ICON_MAIN5_ICON5_ADDR};
const uint8_t icon_16_main5_step_icon_coord[1][2] = {{ 5, 124}};

//星期
const uint32_t icon_16_main5_week_data_addr[2][7] =
{
    {
        ICON_MAIN5_WEEK_CN_1_ADDR, ICON_MAIN5_WEEK_CN_2_ADDR, ICON_MAIN5_WEEK_CN_3_ADDR, ICON_MAIN5_WEEK_CN_4_ADDR,
        ICON_MAIN5_WEEK_CN_5_ADDR, ICON_MAIN5_WEEK_CN_6_ADDR, ICON_MAIN5_WEEK_CN_7_ADDR,
    },
    {
        ICON_MAIN5_WEEK_EN_1_ADDR, ICON_MAIN5_WEEK_EN_2_ADDR, ICON_MAIN5_WEEK_EN_3_ADDR, ICON_MAIN5_WEEK_EN_4_ADDR,
        ICON_MAIN5_WEEK_EN_5_ADDR, ICON_MAIN5_WEEK_EN_6_ADDR, ICON_MAIN5_WEEK_EN_7_ADDR,
    },
};
const uint8_t icon_16_main5_week_data_coord[1][2] = {{ 49, 147}};

//时间数字
const uint32_t icon_16_main5_hour_data_addr[2][10] = 
{
    {
        ICON_MAIN5_DATA1_0_ADDR, ICON_MAIN5_DATA1_1_ADDR, ICON_MAIN5_DATA1_2_ADDR, ICON_MAIN5_DATA1_3_ADDR, ICON_MAIN5_DATA1_4_ADDR,
        ICON_MAIN5_DATA1_5_ADDR, ICON_MAIN5_DATA1_6_ADDR, ICON_MAIN5_DATA1_7_ADDR, ICON_MAIN5_DATA1_8_ADDR, ICON_MAIN5_DATA1_9_ADDR,
    },
    {
        ICON_MAIN5_DATA2_0_ADDR, ICON_MAIN5_DATA2_1_ADDR, ICON_MAIN5_DATA2_2_ADDR, ICON_MAIN5_DATA2_3_ADDR, ICON_MAIN5_DATA2_4_ADDR,
        ICON_MAIN5_DATA2_5_ADDR, ICON_MAIN5_DATA2_6_ADDR, ICON_MAIN5_DATA2_7_ADDR, ICON_MAIN5_DATA2_8_ADDR, ICON_MAIN5_DATA2_9_ADDR,
    },
};
const uint8_t icon_16_main5_hour_data_coord[4][2] = 
{
    { 7,18}, {44 ,18},{7, 71},{ 44,71}
};

//日期数字
const uint32_t icon_16_main5_date_data_addr[10] = 
{
    ICON_MAIN5_DATA4_0_ADDR, ICON_MAIN5_DATA4_1_ADDR, ICON_MAIN5_DATA4_2_ADDR, ICON_MAIN5_DATA4_3_ADDR, ICON_MAIN5_DATA4_4_ADDR,
    ICON_MAIN5_DATA4_5_ADDR, ICON_MAIN5_DATA4_6_ADDR, ICON_MAIN5_DATA4_7_ADDR, ICON_MAIN5_DATA4_8_ADDR, ICON_MAIN5_DATA4_9_ADDR,
};
const uint8_t icon_16_main5_date_data_coord[4][2] = 
{
  { 4, 148},{ 12, 148} ,{ 25, 148},{ 33, 148} 
};

//日期
const uint32_t icon_16_main5_datadot_icon_addr[1] = {ICON_MAIN5_DATA4DOT_ADDR};
const uint8_t icon_16_main5_datadot_icon_coord[1][2] = {{20 ,148}};

//计步数字
const uint32_t icon_16_main5_step_data_addr[10] = 
{
    ICON_MAIN5_DATA3_0_ADDR, ICON_MAIN5_DATA3_1_ADDR, ICON_MAIN5_DATA3_2_ADDR, ICON_MAIN5_DATA3_3_ADDR, ICON_MAIN5_DATA3_4_ADDR,
    ICON_MAIN5_DATA3_5_ADDR, ICON_MAIN5_DATA3_6_ADDR, ICON_MAIN5_DATA3_7_ADDR, ICON_MAIN5_DATA3_8_ADDR, ICON_MAIN5_DATA3_9_ADDR,
};
const uint8_t icon_16_main5_step_data_coord[1][2] = 
{
    {25,128}
};


/*新增主界面6*/

//蓝牙
const uint32_t icon_16_main6_ble_icon_addr[2] = {ICON_MAIN6_ICON1_BLUETOOTH_NOT_ADDR, ICON_MAIN6_ICON1_BLUETOOTH_ADDR};
const uint8_t icon_16_main6_ble_icon_coord[1][2] = {{2,2}};

//AM/PM
const uint32_t icon_16_main6_ampm_icon_addr[2][2] = 
{
    {ICON_MAIN6_ICON2_AM_CN_ADDR, ICON_MAIN6_ICON2_PM_CN_ADDR},
    {ICON_MAIN6_ICON2_AM_EN_ADDR, ICON_MAIN6_ICON2_PM_EN_ADDR},
};
const uint8_t icon_16_main6_ampm_icon_coord[1][2] = {22,2};

//女性
const uint32_t icon_16_main6_menstrual_addr[5] = 
{
    ICON_MAIN6_ICON3_BG_ADDR, ICON_MAIN6_ICON3_1_ADDR, ICON_MAIN6_ICON3_2_ADDR, ICON_MAIN6_ICON3_3_ADDR, ICON_MAIN6_ICON3_4_ADDR
};
const uint8_t icon_16_main6_menstrual_coord[1][2] = {{ 44, 2}};

//电池电量
const uint32_t icon_16_main6_battery_addr[5] = 
{
    ICON_MAIN6_ICON4_BATTERY_1_ADDR, ICON_MAIN6_ICON4_BATTERY_2_ADDR, ICON_MAIN6_ICON4_BATTERY_3_ADDR, ICON_MAIN6_ICON4_BATTERY_4_ADDR,ICON_MAIN6_ICON4_BATTERY_5_ADDR
};
const uint8_t icon_16_main6_battery_coord[1][2] = {{ 59, 2}};

//低电
const uint32_t icon_16_main6_low_power_addr[1] = { ICON_MAIN6_ICON4_BATTERY_6_ADDR};
const uint8_t icon_16_main6_low_power_coord[1][2] = {{ 59, 2}};


//星期日期红色背景
const uint32_t icon_16_main6_background_icon_addr[1] = { ICON_MAIN6_ICON5_ADDR};
const uint8_t icon_16_main6_background_icon_coord[1][2] = {{ 6,80}};   

//星期
const uint32_t icon_16_main6_week_data_addr[2][7] =
{
    {
        ICON_MAIN6_WEEK_CN_1_ADDR, ICON_MAIN6_WEEK_CN_2_ADDR, ICON_MAIN6_WEEK_CN_3_ADDR, ICON_MAIN6_WEEK_CN_4_ADDR,
        ICON_MAIN6_WEEK_CN_5_ADDR, ICON_MAIN6_WEEK_CN_6_ADDR, ICON_MAIN6_WEEK_CN_7_ADDR,
    },
    {
        ICON_MAIN6_WEEK_EN_1_ADDR, ICON_MAIN6_WEEK_EN_2_ADDR, ICON_MAIN6_WEEK_EN_3_ADDR, ICON_MAIN6_WEEK_EN_4_ADDR,
        ICON_MAIN6_WEEK_EN_5_ADDR, ICON_MAIN6_WEEK_EN_6_ADDR, ICON_MAIN6_WEEK_EN_7_ADDR,
    },
};
const uint8_t icon_16_main6_week_data_coord[1][2] = {{ 9, 83}};

//时间数字
const uint32_t icon_16_main6_hour_data_addr[2][10] = 
{
    {
        ICON_MAIN6_DATA1_0_ADDR, ICON_MAIN6_DATA1_1_ADDR, ICON_MAIN6_DATA1_2_ADDR, ICON_MAIN6_DATA1_3_ADDR, ICON_MAIN6_DATA1_4_ADDR,
        ICON_MAIN6_DATA1_5_ADDR, ICON_MAIN6_DATA1_6_ADDR, ICON_MAIN6_DATA1_7_ADDR, ICON_MAIN6_DATA1_8_ADDR, ICON_MAIN6_DATA1_9_ADDR,
    },
    {
        ICON_MAIN6_DATA1_0_ADDR, ICON_MAIN6_DATA1_1_ADDR, ICON_MAIN6_DATA1_2_ADDR, ICON_MAIN6_DATA1_3_ADDR, ICON_MAIN6_DATA1_4_ADDR,
        ICON_MAIN6_DATA1_5_ADDR, ICON_MAIN6_DATA1_6_ADDR, ICON_MAIN6_DATA1_7_ADDR, ICON_MAIN6_DATA1_8_ADDR, ICON_MAIN6_DATA1_9_ADDR,
    },
};
const uint8_t icon_16_main6_hour_data_coord[4][2] = 
{
    { 6,19}, {46 ,19},{6, 102},{ 46,102}
};

//日期数字
const uint32_t icon_16_main6_date_data_addr[10] = 
{
    ICON_MAIN6_DATA2_0_ADDR, ICON_MAIN6_DATA2_1_ADDR, ICON_MAIN6_DATA2_2_ADDR, ICON_MAIN6_DATA2_3_ADDR, ICON_MAIN6_DATA2_4_ADDR,
    ICON_MAIN6_DATA2_5_ADDR, ICON_MAIN6_DATA2_6_ADDR, ICON_MAIN6_DATA2_7_ADDR, ICON_MAIN6_DATA2_8_ADDR, ICON_MAIN6_DATA2_9_ADDR,
};
const uint8_t icon_16_main6_date_data_coord[4][2] = 
{
  { 41, 83},{ 48, 83} ,{ 58, 83},{ 65, 83} 
};

//日期标号
const uint32_t icon_16_main6_datadot_icon_addr[1] = {ICON_MAIN6_DATA2DOT_ADDR};
const uint8_t icon_16_main6_datadot_icon_coord[1][2] = {{54 ,83}};





/*新增主界面7*/

//蓝牙
const uint32_t icon_16_main7_ble_icon_addr[2] = {ICON_MAIN7_ICON1_BLUETOOTH_NOT_ADDR, ICON_MAIN7_ICON1_BLUETOOTH_ADDR};
const uint8_t icon_16_main7_ble_icon_coord[1][2] = {{2,2}};

//AM/PM
const uint32_t icon_16_main7_ampm_icon_addr[2][2] = 
{
    {ICON_MAIN7_ICON2_AM_CN_ADDR, ICON_MAIN7_ICON2_PM_CN_ADDR},
    {ICON_MAIN7_ICON2_AM_EN_ADDR, ICON_MAIN7_ICON2_PM_EN_ADDR},
};
const uint8_t icon_16_main7_ampm_icon_coord[1][2] = {22,2};

//女性
const uint32_t icon_16_main7_menstrual_addr[5] = 
{
    ICON_MAIN7_ICON3_BG_ADDR, ICON_MAIN7_ICON3_1_ADDR, ICON_MAIN7_ICON3_2_ADDR, ICON_MAIN7_ICON3_3_ADDR, ICON_MAIN7_ICON3_4_ADDR
};
const uint8_t icon_16_main7_menstrual_coord[1][2] = {{ 44, 2}};

//电池电量
const uint32_t icon_16_main7_battery_addr[5] = 
{
    ICON_MAIN7_ICON4_BATTERY_1_ADDR, ICON_MAIN7_ICON4_BATTERY_2_ADDR, ICON_MAIN7_ICON4_BATTERY_3_ADDR, ICON_MAIN7_ICON4_BATTERY_4_ADDR, ICON_MAIN7_ICON4_BATTERY_5_ADDR
};
const uint8_t icon_16_main7_battery_coord[1][2] = {{ 59, 2}};

//低电
const uint32_t icon_16_main7_low_power_addr[1] = { ICON_MAIN7_ICON4_BATTERY_6_ADDR};
const uint8_t icon_16_main7_low_power_coord[1][2] = {{ 59, 2}};


//星期
const uint32_t icon_16_main7_week_data_addr[2][7] =
{
    {
        ICON_MAIN7_WEEK_CN_1_ADDR, ICON_MAIN7_WEEK_CN_2_ADDR, ICON_MAIN7_WEEK_CN_3_ADDR, ICON_MAIN7_WEEK_CN_4_ADDR,
        ICON_MAIN7_WEEK_CN_5_ADDR, ICON_MAIN7_WEEK_CN_6_ADDR, ICON_MAIN7_WEEK_CN_7_ADDR,
    },
    {
        ICON_MAIN7_WEEK_EN_1_ADDR, ICON_MAIN7_WEEK_EN_2_ADDR, ICON_MAIN7_WEEK_EN_3_ADDR, ICON_MAIN7_WEEK_EN_4_ADDR,
        ICON_MAIN7_WEEK_EN_5_ADDR, ICON_MAIN7_WEEK_EN_6_ADDR, ICON_MAIN7_WEEK_EN_7_ADDR,
    },
};
const uint8_t icon_16_main7_week_data_coord[1][2] = {{ 2, 146}};

//时间数字
const uint32_t icon_16_main7_hour_data_addr[2][10] = 
{
    {
        ICON_MAIN7_DATA1_0_ADDR, ICON_MAIN7_DATA1_1_ADDR, ICON_MAIN7_DATA1_2_ADDR, ICON_MAIN7_DATA1_3_ADDR, ICON_MAIN7_DATA1_4_ADDR,
        ICON_MAIN7_DATA1_5_ADDR, ICON_MAIN7_DATA1_6_ADDR, ICON_MAIN7_DATA1_7_ADDR, ICON_MAIN7_DATA1_8_ADDR, ICON_MAIN7_DATA1_9_ADDR,
    },
    {
        ICON_MAIN7_DATA2_0_ADDR, ICON_MAIN7_DATA2_1_ADDR, ICON_MAIN7_DATA2_2_ADDR, ICON_MAIN7_DATA2_3_ADDR, ICON_MAIN7_DATA2_4_ADDR,
        ICON_MAIN7_DATA2_5_ADDR, ICON_MAIN7_DATA2_6_ADDR, ICON_MAIN7_DATA2_7_ADDR, ICON_MAIN7_DATA2_8_ADDR, ICON_MAIN7_DATA2_9_ADDR,
    },
};
const uint8_t icon_16_main7_hour_data_coord[4][2] = 
{
    { 1,23}, {41 ,23},{1, 84},{ 41,84}
};

//日期数字
const uint32_t icon_16_main7_date_data_addr[10] = 
{
    ICON_MAIN4_DATA3_0_ADDR, ICON_MAIN4_DATA3_1_ADDR, ICON_MAIN4_DATA3_2_ADDR, ICON_MAIN4_DATA3_3_ADDR, ICON_MAIN4_DATA3_4_ADDR,
    ICON_MAIN4_DATA3_5_ADDR, ICON_MAIN4_DATA3_6_ADDR, ICON_MAIN4_DATA3_7_ADDR, ICON_MAIN4_DATA3_8_ADDR, ICON_MAIN4_DATA3_9_ADDR,
};
const uint8_t icon_16_main7_date_data_coord[2][2] = 
{
  { 62, 146},{ 70, 146}  
};






