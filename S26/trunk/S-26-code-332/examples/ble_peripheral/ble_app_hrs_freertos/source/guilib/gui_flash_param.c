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

/* 功能界面的数字 */
const uint32_t icon_16_function_digital_addr[12] = 
{
	ICON_FUNCTION_DATA_0_ADDR, ICON_FUNCTION_DATA_1_ADDR, ICON_FUNCTION_DATA_2_ADDR, ICON_FUNCTION_DATA_3_ADDR, ICON_FUNCTION_DATA_4_ADDR,
	ICON_FUNCTION_DATA_5_ADDR, ICON_FUNCTION_DATA_6_ADDR, ICON_FUNCTION_DATA_7_ADDR, ICON_FUNCTION_DATA_8_ADDR, ICON_FUNCTION_DATA_9_ADDR,
    //特殊符号 
    ICON_FUNCTION_POINT_ADDR, ICON_FUNCTION_SEPARATOR_ADDR
};



/* 计步 */
const uint32_t icon_16_step_icon_addr[1] = {ICON_STEP_SHOES_ADDR};
const uint32_t icon_16_step_icon_coord[4][2] = {{15 , 3}, {15 , 6}, {15 , 3}, {15 , 0}};

const uint32_t icon_16_step_prog_addr[20] = 
{
	ICON_STEP_PROGRESS_01_ADDR, ICON_STEP_PROGRESS_02_ADDR, ICON_STEP_PROGRESS_03_ADDR, ICON_STEP_PROGRESS_04_ADDR, ICON_STEP_PROGRESS_05_ADDR,
	ICON_STEP_PROGRESS_06_ADDR, ICON_STEP_PROGRESS_07_ADDR, ICON_STEP_PROGRESS_08_ADDR, ICON_STEP_PROGRESS_09_ADDR, ICON_STEP_PROGRESS_10_ADDR,
	ICON_STEP_PROGRESS_11_ADDR, ICON_STEP_PROGRESS_12_ADDR, ICON_STEP_PROGRESS_13_ADDR, ICON_STEP_PROGRESS_14_ADDR, ICON_STEP_PROGRESS_15_ADDR,
	ICON_STEP_PROGRESS_16_ADDR, ICON_STEP_PROGRESS_17_ADDR, ICON_STEP_PROGRESS_18_ADDR, ICON_STEP_PROGRESS_19_ADDR, ICON_STEP_PROGRESS_20_ADDR
};
const uint32_t icon_16_step_prog_coord[20][2] =
{
	{66, 39}, {66, 45}, {66, 48}, {66, 51}, {62, 54},
	{56, 57}, {51, 57}, {47, 58}, {43, 58}, {39, 58}, 
    {35, 58}, {31, 58}, {26, 58}, {22, 58}, {14, 57},
	{7, 54},  {2, 51},  {0, 49},  {0, 44},  {0, 39}
};

const uint32_t icon_16_step_ring_addr[1] = {ICON_STEP_RINGS_GRAY_ADDR};
const uint32_t icon_16_step_ring_coord[1][2] = {{0, 39}};

const uint32_t icon_16_step_unit_addr[2] = {ICON_STEP_UNIT_CN_ADDR, ICON_STEP_UNIT_EN_ADDR};
const uint32_t icon_16_step_unit_coord[1][2] = {{22, 68}};

const uint8_t icon_16_step_data_coord[3][2] = {{0, 88},{62, 111},{0,126}};

/* 距离 */
const uint32_t icon_16_distance_map1_addr[1] = {ICON_DISTANCE_MAP1_ADDR};
const uint8_t icon_16_distance_map1_coord[4][2] = {{15, 3}, {15, 6}, {15, 3}, {15, 0}};

const uint32_t icon_16_distance_map2_addr[1] = {ICON_DISTANCE_MAP2_ADDR};
const uint8_t icon_16_distance_map2_coord[4][2] = {{30, 58}, {30, 61}, {30, 58}, {30, 55}};

const uint32_t icon_16_distance_ring_addr[1] = {ICON_DISTANCE_RING_ADDR};
const uint8_t icon_16_distance_ring_coord[1][2] = {{6, 61}};
    
const uint32_t icon_16_distance_unit_addr[4] = 
{
	ICON_DISTANCE_UNIT_KM_CH_ADDR, ICON_DISTANCE_UNIT_KM_EN_ADDR, 
    ICON_DISTANCE_UNIT_MI_ADDR, ICON_DISTANCE_UNIT_MI_CH_ADDR
};
const uint8_t icon_16_distance_unit_coord[1][2] = {{24, 144}};

const uint32_t icon_16_distance_point_addr[1] = {ICON_FUNCTION_POINT_ADDR};
const uint8_t icon_16_distance_point_coord[1][2] = {{0, 131}};

const uint8_t icon_16_distance_data_coord[2][2] = {{2, 105}, {13, 105}};

/* 卡路里 */
const uint32_t icon_16_calories_contour_addr[1] = {ICON_CALORIES_ICON1_PERIPHERY_ADDR};
const uint8_t icon_16_calories_contour_coord[1][2] = {{8, 0}};
    
const uint32_t icon_16_calories_flame_addr[4] = 
{
    ICON_CALORIES_ICON_FLAME_0_ADDR, ICON_CALORIES_ICON_FLAME_1_ADDR, 
    ICON_CALORIES_ICON_FLAME_2_ADDR, ICON_CALORIES_ICON_FLAME_3_ADDR
};
const uint8_t icon_16_calories_flame_coord[1][2] = {{24, 23}};

const uint32_t icon_16_calories_unit_addr[2] = 
{
    ICON_CALORIES_UNIT_KCAL_CN_ADDR, ICON_CALORIES_UNIT_KCAL_EN_ADDR
};
const uint8_t icon_16_calories_unit_coord[1][2] = {{24, 144}};

const uint8_t icon_16_calories_data_coord[4][2] = {{10, 105},{18, 105},{29, 105}, {5, 113}};


/* 运动模式 */

///* 血压 */


/* 心率 */
const uint32_t icon_16_heart_contour_addr[1] = {ICON_HEARTRATE_GRAPHICS_ADDR};
const uint8_t icon_16_heart_contour_coord[1][2] = {{1, 15}};

const uint32_t icon_16_heart_line_addr[7] =
{
	ICON_HEARTRATE_LINE_1_ADDR, ICON_HEARTRATE_LINE_2_ADDR, ICON_HEARTRATE_LINE_3_ADDR, ICON_HEARTRATE_LINE_4_ADDR,
	ICON_HEARTRATE_LINE_5_ADDR, ICON_HEARTRATE_LINE_6_ADDR, ICON_HEARTRATE_LINE_7_ADDR
};
const uint8_t icon_16_heart_line_coord[1][2] = {{1, 31}};

const uint32_t icon_16_heart_unit_addr[2] = {ICON_HEARTRATE_UNIT_CN_ADDR, ICON_HEARTRATE_UNIT_EN_ADDR};
const uint8_t icon_16_heart_unit_coord[1][2] = {{20, 144}};

const uint8_t icon_16_heart_data_coord[1][2] = {{0, 105}};


/* 睡眠 */
const uint32_t icon_16_sleep_icon_addr[4] =
{
	ICON_SLEEP_ICON_1_ADDR, ICON_SLEEP_ICON_2_ADDR, ICON_SLEEP_ICON_3_ADDR, ICON_SLEEP_ICON_4_ADDR
};
const uint8_t icon_16_sleep_icon_coord[1][2] = {{3, 0}};

const uint32_t icon_16_sleep_unit_addr[2] = {ICON_SLEEP_UNIT_HOURS_CN_ADDR, ICON_SLEEP_UNIT_HOURS_EN_ADDR};
const uint8_t icon_16_sleep_unit_coord[1][2] = {{17, 144}};

const uint32_t icon_16_sleep_point_addr[1] = {ICON_FUNCTION_POINT_ADDR};
const uint8_t icon_16_sleep_point_coord[1][2] = {{0, 131}};

const uint8_t icon_16_sleep_data_coord[2][2] = {{2, 105}, {13, 105}};


/* 找手机 */
const uint32_t icon_16_findphone_magnifier_addr[5] = 
{
	ICON_FIND_PHONES1_MAGNIFIER_1_ADDR, ICON_FIND_PHONES1_MAGNIFIER_2_ADDR, ICON_FIND_PHONES1_MAGNIFIER_3_ADDR, ICON_FIND_PHONES1_MAGNIFIER_4_ADDR, ICON_FIND_PHONES1_MAGNIFIER_0_ADDR
};
const uint8_t icon_16_findphone_magnifier_coord[1][2] = {{27, 63}};

const uint32_t icon_16_findphone_phone_addr[1] = {ICON_FIND_PHONES_ADDR};
const uint8_t icon_16_findphone_phone_coord[1][2] = {{13, 38}};

/* 秒表 */
const uint32_t icon_16_stopwatch_contour_addr[1] = {ICON_STOPWATCH_CONTOUR_ADDR};
const uint8_t icon_16_stopwatch_contour_coord[6][2] = {{6, 38}};

const uint32_t icon_16_stopwatch_pointer_addr[8] = 
{ 
	ICON_STOPWATCH_POINTER_1_ADDR, ICON_STOPWATCH_POINTER_2_ADDR, ICON_STOPWATCH_POINTER_3_ADDR,ICON_STOPWATCH_POINTER_4_ADDR, 
    ICON_STOPWATCH_POINTER_5_ADDR, ICON_STOPWATCH_POINTER_6_ADDR, ICON_STOPWATCH_POINTER_7_ADDR,ICON_STOPWATCH_POINTER_8_ADDR,
};
const uint8_t icon_16_stopwatch_pointer_coord[1][2] = {{21, 66}};

const uint8_t icon_16_stopwatch_data_coord[6][2] = 
{
	{5, 2},  {46, 2},
	{5, 56}, {46, 56},
	{5, 111},{46, 111}
};

//秒表数字
const uint32_t icon_16_stopwatch_number1_addr[10] = 
{
	ICON_TIMING_MIN_0_ADDR, ICON_TIMING_MIN_1_ADDR, ICON_TIMING_MIN_2_ADDR, ICON_TIMING_MIN_3_ADDR, ICON_TIMING_MIN_4_ADDR, 
    ICON_TIMING_MIN_5_ADDR, ICON_TIMING_MIN_6_ADDR, ICON_TIMING_MIN_7_ADDR, ICON_TIMING_MIN_8_ADDR, ICON_TIMING_MIN_9_ADDR
};

const uint32_t icon_16_stopwatch_number2_addr[10] = 
{
	ICON_TIMING_SECOND_0_ADDR, ICON_TIMING_SECOND_1_ADDR, ICON_TIMING_SECOND_2_ADDR, ICON_TIMING_SECOND_3_ADDR, ICON_TIMING_SECOND_4_ADDR, 
    ICON_TIMING_SECOND_5_ADDR, ICON_TIMING_SECOND_6_ADDR, ICON_TIMING_SECOND_7_ADDR, ICON_TIMING_SECOND_8_ADDR, ICON_TIMING_SECOND_9_ADDR
};

const uint32_t icon_16_stopwatch_number3_addr[10] = 
{
	ICON_TIMING_MILLISECOND_0_ADDR, ICON_TIMING_MILLISECOND_1_ADDR, ICON_TIMING_MILLISECOND_2_ADDR, ICON_TIMING_MILLISECOND_3_ADDR, ICON_TIMING_MILLISECOND_4_ADDR, 
    ICON_TIMING_MILLISECOND_5_ADDR, ICON_TIMING_MILLISECOND_6_ADDR, ICON_TIMING_MILLISECOND_7_ADDR, ICON_TIMING_MILLISECOND_8_ADDR, ICON_TIMING_MILLISECOND_9_ADDR
};

/* 倒计时 */
const uint32_t icon_16_countdown_icon_addr[6] = 
{
	ICON_COUNTDOWN_ICON_1_ADDR, ICON_COUNTDOWN_ICON_2_ADDR, ICON_COUNTDOWN_ICON_3_ADDR, 
    ICON_COUNTDOWN_ICON_4_ADDR, ICON_COUNTDOWN_ICON_5_ADDR, ICON_COUNTDOWN_ICON_6_ADDR,
};
const uint8_t icon_16_countdown_icon_coord[1][2] = {{0, 40}};


/* 关机 */
const uint32_t icon_16_poweroff_icon_addr[1] = {ICON_OFF_ADDR};
const uint8_t icon_16_poweroff_icon_coord[1][2] = {{7, 40}};


/* 充电 */
const uint32_t icon_16_chg_bar_addr[7] = 
{
	ICON_CHARGE_BAR_1_ADDR, ICON_CHARGE_BAR_2_ADDR, ICON_CHARGE_BAR_3_ADDR,
	ICON_CHARGE_BAR_4_ADDR, ICON_CHARGE_BAR_5_ADDR, ICON_CHARGE_BAR_6_ADDR, ICON_CHARGE_BAR_7_ADDR
};
const uint8_t icon_16_chg_bar_coord[1][2] = {{26, 50}};

const uint32_t icon_16_chg_contour_addr[1] = {ICON_CHARGE_CONTOUR_ADDR};
const uint8_t icon_16_chg_contour_coord[1][2] = {{16, 33}};

/* 闹钟 */
const uint32_t icon_16_clock_ampm_addr[4] = 
{
	ICON_CLOCK_ICON_AM_CN_ADDR, ICON_CLOCK_ICON_PM_CN_ADDR, ICON_CLOCK_ICON_AM_EN_ADDR, ICON_CLOCK_ICON_PM_EN_ADDR
};
const uint8_t icon_16_clock_ampm_coord[1][2] = {{24, 144}};

const uint32_t icon_16_clock_contour_addr[1] = {ICON_CLOCK_CONTOUR_ADDR};
const uint8_t icon_16_clock_contour_coord[1][2] = {{1, 4}};

const uint32_t icon_16_clock_pointer_addr[7] = 
{
	ICON_CLOCK_POINTER_1_ADDR, ICON_CLOCK_POINTER_2_ADDR, ICON_CLOCK_POINTER_3_ADDR,ICON_CLOCK_POINTER_4_ADDR,
    ICON_CLOCK_POINTER_5_ADDR, ICON_CLOCK_POINTER_6_ADDR, ICON_CLOCK_POINTER_7_ADDR, 
};
const uint8_t icon_16_clock_pointer_coord[1][2] = {{20, 24}};

const uint32_t icon_16_clock_number_addr[10] = 
{
	ICON_CLOCK_NUMBER_0_ADDR, ICON_CLOCK_NUMBER_1_ADDR, ICON_CLOCK_NUMBER_2_ADDR, ICON_CLOCK_NUMBER_3_ADDR,ICON_CLOCK_NUMBER_4_ADDR,
    ICON_CLOCK_NUMBER_5_ADDR, ICON_CLOCK_NUMBER_6_ADDR, ICON_CLOCK_NUMBER_7_ADDR, ICON_CLOCK_NUMBER_8_ADDR,ICON_CLOCK_NUMBER_9_ADDR, 
};
const uint8_t icon_16_clock_data_coord[4][2] = {{0, 113}, {18, 113}, {45, 113}, {63, 113}};

const uint32_t icon_16_clock_colon_addr[1] = {ICON_CLOCK_COLON_ADDR};
const uint8_t icon_16_clock_colon_coord[1][2] = {{37, 118}};

 //20个事件提醒
const uint32_t icon_16_remindclock_icon_addr[20] = 
{
	ICON_REMINDCLOCK_ICON_01_ADDR, ICON_REMINDCLOCK_ICON_02_ADDR, ICON_REMINDCLOCK_ICON_03_ADDR, ICON_REMINDCLOCK_ICON_04_ADDR, ICON_REMINDCLOCK_ICON_05_ADDR,
	ICON_REMINDCLOCK_ICON_06_ADDR, ICON_REMINDCLOCK_ICON_07_ADDR, ICON_REMINDCLOCK_ICON_08_ADDR, ICON_REMINDCLOCK_ICON_09_ADDR, ICON_REMINDCLOCK_ICON_10_ADDR,
	ICON_REMINDCLOCK_ICON_11_ADDR, ICON_REMINDCLOCK_ICON_12_ADDR, ICON_REMINDCLOCK_ICON_13_ADDR, ICON_REMINDCLOCK_ICON_14_ADDR, ICON_REMINDCLOCK_ICON_15_ADDR,
	ICON_REMINDCLOCK_ICON_16_ADDR, ICON_REMINDCLOCK_ICON_17_ADDR, ICON_REMINDCLOCK_ICON_18_ADDR, ICON_REMINDCLOCK_ICON_19_ADDR, ICON_REMINDCLOCK_ICON_20_ADDR
};
const uint8_t icon_16_remindclock_icon_coord[2][2] = {{0, 37}, {0, 42}};

/* 久坐 */
const uint32_t icon_16_sit_icon_addr[2] = {ICON_SEDENTARY_ICON_1_ADDR, ICON_SEDENTARY_ICON_2_ADDR};
const uint8_t icon_16_sit_icon_coord[1][2] = {{0, 40}};


/* 消息推送 */
const uint32_t icon_16_inform_icon_addr[13] = 
{
	ICON_INFORM_ICON_01_ADDR, ICON_INFORM_ICON_02_ADDR, ICON_INFORM_ICON_03_ADDR, ICON_INFORM_ICON_04_ADDR,
    ICON_INFORM_ICON_05_ADDR, ICON_INFORM_ICON_06_ADDR, ICON_INFORM_ICON_07_ADDR, ICON_INFORM_ICON_08_ADDR,
    ICON_INFORM_ICON_09_ADDR, ICON_INFORM_ICON_10_ADDR, ICON_INFORM_ICON_11_ADDR, ICON_INFORM_ICON_12_ADDR,
    ICON_INFORM_ICON_13_ADDR, 
};
const uint8_t icon_16_inform_icon_coord[2][2] = {{7, 0}, {7, 4}};

const uint8_t icon_16_inform_text_coord[1][2] = {{0, 70}};

//来电
const uint32_t icon_16_call_phone_addr[1] = {ICON_CALL_PHONE_ADDR};
const uint8_t icon_16_call_phone_coord[1][2] = {{6, 21}};

const uint32_t icon_16_call_arr_addr[3] = {ICON_CALL_ARC_1_ADDR, ICON_CALL_ARC_2_ADDR, ICON_CALL_ARC_3_ADDR};
const uint8_t icon_16_call_arr_coord[1][2] = {{44, 8}};

const uint32_t icon_16_call_hangup_addr[1] = {ICON_CALL_HANGUP_ADDR};
const uint8_t icon_16_call_hangup_coord[1][2] = {{8, 17}};

const uint32_t icon_16_call_mute_addr[1] = {ICON_CALL_MUTE_ADDR};
const uint8_t icon_16_call_mute_coord[1][2] = {{6, 8}};
//文字
const uint8_t icon_16_call_text_coord[1][2] = {{0, 118}};


/* 心率报警 */
const uint32_t icon_16_heartalert_contour_addr[1] = {ICON_ABNORMALITY_HEART_ADDR};
const uint8_t icon_16_heartalert_contour_coord[1][2] = {{0, 46}};

const uint32_t icon_16_heartalert_round_addr[5] = 
{
	ICON_ABNORMALITY_ROUND_1_ADDR, ICON_ABNORMALITY_ROUND_2_ADDR, ICON_ABNORMALITY_ROUND_3_ADDR, ICON_ABNORMALITY_ROUND_4_ADDR, ICON_ABNORMALITY_ROUND_5_ADDR
};
const uint8_t icon_16_heartalert_round_coord[1][2] = {{3, 74}};


/* 相机 */
const uint32_t icon_16_camera_contour_addr[1] = {ICON_CAMERA_CONTOUR_ADDR};
const uint8_t icon_16_camera_contour_coord[1][2] = {{2, 51}};

const uint32_t icon_16_camera_round_addr[3] = 
{
	ICON_CAMERA_ROUND_1_ADDR, ICON_CAMERA_ROUND_2_ADDR, ICON_CAMERA_ROUND_3_ADDR
};
const uint8_t icon_16_camera_round_coord[1][2] = {{28, 72}};

/* 低电 */
const uint32_t icon_16_lowpower_contour_addr[1] = {ICON_LOWELECTRICITY_CONTOUR_ADDR};
const uint8_t icon_16_lowpower_contour_coord[1][2] = {{16, 33}};

const uint32_t icon_16_lowpower_lightning_addr[1] = {ICON_LOWELECTRICITY_LIGHTNING_ADDR};
const uint8_t icon_16_lowpower_lightning_coord[1][2] = {{26, 62}};

/* CSR*/


/* 运动达标*/
const uint32_t icon_16_target_icon_addr[2] = { ICON_GOAL_1_ADDR, ICON_GOAL_2_ADDR};
const uint8_t icon_16_target_icon_coord[2][2] = {{0, 0}, {12, 18}};

//const uint8_t icon_16_target_stars_coord[5][2] = {{0, 0}, {12, 18}};
//const uint8_t icon_16_target_stars_size[5][2] = {{0, 0}, {12, 18}};


//主界面-风格一
   //背景
const uint32_t icon_main1_bar_addr[2][1] = { 
    ICON_MAIN1GREEN_BAR_ADDR, ICON_MAIN1WHITE_BAR_ADDR
};

const uint8_t icon_main1_bar_coord[1][2] = {{0, 145}};
  //电池电量\充电    
const uint32_t icon_main1_battery_addr[2][2] = {   
    {
    ICON_MAIN1GREEN_BATTERY_ADDR, ICON_MAIN1GREEN_CHARGING_ADDR
    },
    {
    ICON_MAIN1WHITE_BATTERY_ADDR, ICON_MAIN1WHITE_CHARGING_ADDR
    }
};
const uint8_t icon_main1_battery_coord[1][2] = {{18, 149}};
const uint8_t icon_main1_battery_level_coord[5][2] = {
    {20, 151}, {23, 151}, {26, 151}, {29, 151},{32, 151}
};
  //ble    
const uint32_t icon_main1_ble_addr[2][2] = {   
    {
    ICON_MAIN1GREEN_BLE_UNLINKED_ADDR, ICON_MAIN1GREEN_BLE_LINK_ADDR
    },
    {
    ICON_MAIN1WHITE_BLE_UNLINKED_ADDR, ICON_MAIN1WHITE_BLE_LINK_ADDR
    },
};
const uint8_t icon_main1_ble_coord[1][2] = {{1, 148}};
   //日期
const uint32_t icon_main1_date_addr[2][10] = {   
    {
    ICON_MAIN1GREEN_DATE_0_ADDR, ICON_MAIN1GREEN_DATE_1_ADDR, ICON_MAIN1GREEN_DATE_2_ADDR,
    ICON_MAIN1GREEN_DATE_3_ADDR, ICON_MAIN1GREEN_DATE_4_ADDR, ICON_MAIN1GREEN_DATE_5_ADDR,
    ICON_MAIN1GREEN_DATE_6_ADDR, ICON_MAIN1GREEN_DATE_7_ADDR, ICON_MAIN1GREEN_DATE_8_ADDR,
    ICON_MAIN1GREEN_DATE_9_ADDR, 
    },
    {
    ICON_MAIN1WHITE_DATE_0_ADDR, ICON_MAIN1WHITE_DATE_1_ADDR, ICON_MAIN1WHITE_DATE_2_ADDR,
    ICON_MAIN1WHITE_DATE_3_ADDR, ICON_MAIN1WHITE_DATE_4_ADDR, ICON_MAIN1WHITE_DATE_5_ADDR,
    ICON_MAIN1WHITE_DATE_6_ADDR, ICON_MAIN1WHITE_DATE_7_ADDR, ICON_MAIN1WHITE_DATE_8_ADDR,
    ICON_MAIN1WHITE_DATE_9_ADDR, 
    }
};
const uint8_t icon_main1_date_coord[2][2] = {{66, 148}, {73,148}};     
  //csr3.0    
     
  //时间数字   
const uint32_t icon_main1_hour_addr[2][18] = { 
    {
        ICON_MAIN1GREEN_TIME_0_ADDR, ICON_MAIN1GREEN_TIME_1_ADDR, ICON_MAIN1GREEN_TIME_2_ADDR,
        ICON_MAIN1GREEN_TIME_3_ADDR, ICON_MAIN1GREEN_TIME_4_ADDR, ICON_MAIN1GREEN_TIME_5_ADDR,
        ICON_MAIN1GREEN_TIME_6_ADDR, ICON_MAIN1GREEN_TIME_7_ADDR, ICON_MAIN1GREEN_TIME_8_ADDR,
        ICON_MAIN1GREEN_TIME_9_ADDR,
        ICON_MAIN1GREEN_TIME_AM_CN_0_ADDR, ICON_MAIN1GREEN_TIME_AM_CN_1_ADDR,
        ICON_MAIN1GREEN_TIME_PM_CN_0_ADDR, ICON_MAIN1GREEN_TIME_PM_CN_1_ADDR,
        ICON_MAIN1GREEN_TIME_AM_EN_0_ADDR, ICON_MAIN1GREEN_TIME_AM_EN_1_ADDR,
        ICON_MAIN1GREEN_TIME_PM_EN_0_ADDR, ICON_MAIN1GREEN_TIME_PM_EN_1_ADDR,
    },
    {
        ICON_MAIN1WHITE_TIME_0_ADDR, ICON_MAIN1WHITE_TIME_1_ADDR, ICON_MAIN1WHITE_TIME_2_ADDR,
        ICON_MAIN1WHITE_TIME_3_ADDR, ICON_MAIN1WHITE_TIME_4_ADDR, ICON_MAIN1WHITE_TIME_5_ADDR,
        ICON_MAIN1WHITE_TIME_6_ADDR, ICON_MAIN1WHITE_TIME_7_ADDR, ICON_MAIN1WHITE_TIME_8_ADDR,
        ICON_MAIN1WHITE_TIME_9_ADDR,
        ICON_MAIN1WHITE_TIME_AM_CN_0_ADDR, ICON_MAIN1WHITE_TIME_AM_CN_1_ADDR,
        ICON_MAIN1WHITE_TIME_PM_CN_0_ADDR, ICON_MAIN1WHITE_TIME_PM_CN_1_ADDR,
        ICON_MAIN1WHITE_TIME_AM_EN_0_ADDR, ICON_MAIN1WHITE_TIME_AM_EN_1_ADDR,
        ICON_MAIN1WHITE_TIME_PM_EN_0_ADDR, ICON_MAIN1WHITE_TIME_PM_EN_1_ADDR,
    }
}; 
const uint8_t icon_main1_hour_coord[4][2] = {{1, 8}, {42, 8}, {1, 77}, {42, 77}};
  //星期
const uint32_t icon_main1_week_addr[2][14] = {
    {
        ICON_MAIN1GREEN_WEEK_CN_1_ADDR, ICON_MAIN1GREEN_WEEK_CN_2_ADDR, ICON_MAIN1GREEN_WEEK_CN_3_ADDR,
        ICON_MAIN1GREEN_WEEK_CN_4_ADDR, ICON_MAIN1GREEN_WEEK_CN_5_ADDR, ICON_MAIN1GREEN_WEEK_CN_6_ADDR,
        ICON_MAIN1GREEN_WEEK_CN_7_ADDR,
        ICON_MAIN1GREEN_WEEK_EN_1_ADDR, ICON_MAIN1GREEN_WEEK_EN_2_ADDR, ICON_MAIN1GREEN_WEEK_EN_3_ADDR,
        ICON_MAIN1GREEN_WEEK_EN_4_ADDR, ICON_MAIN1GREEN_WEEK_EN_5_ADDR, ICON_MAIN1GREEN_WEEK_EN_6_ADDR,
        ICON_MAIN1GREEN_WEEK_EN_7_ADDR,
    },
    {
        ICON_MAIN1WHITE_WEEK_CN_1_ADDR, ICON_MAIN1WHITE_WEEK_CN_2_ADDR, ICON_MAIN1WHITE_WEEK_CN_3_ADDR,
        ICON_MAIN1WHITE_WEEK_CN_4_ADDR, ICON_MAIN1WHITE_WEEK_CN_5_ADDR, ICON_MAIN1WHITE_WEEK_CN_6_ADDR,
        ICON_MAIN1WHITE_WEEK_CN_7_ADDR,
        ICON_MAIN1WHITE_WEEK_EN_1_ADDR, ICON_MAIN1WHITE_WEEK_EN_2_ADDR, ICON_MAIN1WHITE_WEEK_EN_3_ADDR,
        ICON_MAIN1WHITE_WEEK_EN_4_ADDR, ICON_MAIN1WHITE_WEEK_EN_5_ADDR, ICON_MAIN1WHITE_WEEK_EN_6_ADDR,
        ICON_MAIN1WHITE_WEEK_EN_7_ADDR,
    }
};    
const uint8_t icon_main1_week_coord[1][2] = {{42, 147}};

//主界面-风格二
 //am-pm    
const uint32_t icon_main2_ampm_addr[3][2] = {     
    {   //绿色 
    ICON_MAIN2GREEN_AMPM_AM_ADDR, ICON_MAIN2GREEN_AMPM_PM_ADDR
    },
    {  //黄色
    ICON_MAIN2YELLOW_AMPM_AM_ADDR, ICON_MAIN2YELLOW_AMPM_PM_ADDR
    },
    {  //白色
    ICON_MAIN2WHITE_AMPM_AM_ADDR, ICON_MAIN2WHITE_AMPM_PM_ADDR
    },
};
const uint8_t icon_main2_ampm_coord[1][2] = {{15, 35}};  
  //时间数字   
const uint32_t icon_main2_hour_addr[3][18] = { 
    {
        ICON_MAIN2GREEN_TIME_0_ADDR, ICON_MAIN2GREEN_TIME_1_ADDR, ICON_MAIN2GREEN_TIME_2_ADDR,
        ICON_MAIN2GREEN_TIME_3_ADDR, ICON_MAIN2GREEN_TIME_4_ADDR, ICON_MAIN2GREEN_TIME_5_ADDR,
        ICON_MAIN2GREEN_TIME_6_ADDR, ICON_MAIN2GREEN_TIME_7_ADDR, ICON_MAIN2GREEN_TIME_8_ADDR,
        ICON_MAIN2GREEN_TIME_9_ADDR,
        ICON_MAIN2GREEN_TIME_AM_CN_0_ADDR, ICON_MAIN2GREEN_TIME_AM_CN_1_ADDR,
        ICON_MAIN2GREEN_TIME_PM_CN_0_ADDR, ICON_MAIN2GREEN_TIME_PM_CN_1_ADDR,
        ICON_MAIN2GREEN_TIME_AM_EN_0_ADDR, ICON_MAIN2GREEN_TIME_AM_EN_1_ADDR,
        ICON_MAIN2GREEN_TIME_PM_EN_0_ADDR, ICON_MAIN2GREEN_TIME_PM_EN_1_ADDR,
    },
    {
        ICON_MAIN2YELLOW_TIME_0_ADDR, ICON_MAIN2YELLOW_TIME_1_ADDR, ICON_MAIN2YELLOW_TIME_2_ADDR,
        ICON_MAIN2YELLOW_TIME_3_ADDR, ICON_MAIN2YELLOW_TIME_4_ADDR, ICON_MAIN2YELLOW_TIME_5_ADDR,
        ICON_MAIN2YELLOW_TIME_6_ADDR, ICON_MAIN2YELLOW_TIME_7_ADDR, ICON_MAIN2YELLOW_TIME_8_ADDR,
        ICON_MAIN2YELLOW_TIME_9_ADDR,
        ICON_MAIN2YELLOW_TIME_AM_CN_0_ADDR, ICON_MAIN2YELLOW_TIME_AM_CN_1_ADDR,
        ICON_MAIN2YELLOW_TIME_PM_CN_0_ADDR, ICON_MAIN2YELLOW_TIME_PM_CN_1_ADDR,
        ICON_MAIN2YELLOW_TIME_AM_EN_0_ADDR, ICON_MAIN2YELLOW_TIME_AM_EN_1_ADDR,
        ICON_MAIN2YELLOW_TIME_PM_EN_0_ADDR, ICON_MAIN2YELLOW_TIME_PM_EN_1_ADDR,
    },
    {
        ICON_MAIN2WHITE_TIME_0_ADDR, ICON_MAIN2WHITE_TIME_1_ADDR, ICON_MAIN2WHITE_TIME_2_ADDR,
        ICON_MAIN2WHITE_TIME_3_ADDR, ICON_MAIN2WHITE_TIME_4_ADDR, ICON_MAIN2WHITE_TIME_5_ADDR,
        ICON_MAIN2WHITE_TIME_6_ADDR, ICON_MAIN2WHITE_TIME_7_ADDR, ICON_MAIN2WHITE_TIME_8_ADDR,
        ICON_MAIN2WHITE_TIME_9_ADDR,
        ICON_MAIN2WHITE_TIME_AM_CN_0_ADDR, ICON_MAIN2WHITE_TIME_AM_CN_1_ADDR,
        ICON_MAIN2WHITE_TIME_PM_CN_0_ADDR, ICON_MAIN2WHITE_TIME_PM_CN_1_ADDR,
        ICON_MAIN2WHITE_TIME_AM_EN_0_ADDR, ICON_MAIN2WHITE_TIME_AM_EN_1_ADDR,
        ICON_MAIN2WHITE_TIME_PM_EN_0_ADDR, ICON_MAIN2WHITE_TIME_PM_EN_1_ADDR,
    },
}; 
const uint8_t icon_main2_hour_coord[4][2] = {{19, 0}, {51, 0}, {19, 51}, {51, 51}};

  //电池电量\充电    
const uint32_t icon_main2_battery_addr[2][2] = {   
    {
    ICON_MAIN2G_BATTERY_G_ADDR, ICON_MAIN2G_CHARGING_G_ADDR
    },
    {
    ICON_MAIN2Y_BATTERY_Y_ADDR, ICON_MAIN2Y_CHARGING_Y_ADDR
    }
};
const uint8_t icon_main2_battery_coord[1][2] = {{60, 149}};
const uint8_t icon_main2_battery_level_coord[5][2] = {
    {63, 152}, {67, 152}, {70, 152}, {73, 152},{77, 152}
};
  //ble    
const uint32_t icon_main2_ble_addr[2][2] = {   
    {
    ICON_MAIN2G_BLE_UNLINKED_G_ADDR, ICON_MAIN2G_BLE_LINK_G_ADDR
    },
    {
    ICON_MAIN2Y_BLE_UNLINKED_Y_ADDR, ICON_MAIN2Y_BLE_LINK_Y_ADDR
    },
};
const uint8_t icon_main2_ble_coord[1][2] = {{0, 149}};
   //日期
const uint32_t icon_main2_date_addr[3][10] = {   
    {
    ICON_MAIN2G_NUMBER_0_G1_ADDR, ICON_MAIN2G_NUMBER_1_G1_ADDR, ICON_MAIN2G_NUMBER_2_G1_ADDR,
    ICON_MAIN2G_NUMBER_3_G1_ADDR, ICON_MAIN2G_NUMBER_4_G1_ADDR, ICON_MAIN2G_NUMBER_5_G1_ADDR,
    ICON_MAIN2G_NUMBER_6_G1_ADDR, ICON_MAIN2G_NUMBER_7_G1_ADDR, ICON_MAIN2G_NUMBER_8_G1_ADDR,
    ICON_MAIN2G_NUMBER_9_G1_ADDR, 
    },
    {
    ICON_MAIN2Y_NUMBER_0_Y_ADDR, ICON_MAIN2Y_NUMBER_1_Y_ADDR, ICON_MAIN2Y_NUMBER_2_Y_ADDR,
    ICON_MAIN2Y_NUMBER_3_Y_ADDR, ICON_MAIN2Y_NUMBER_4_Y_ADDR, ICON_MAIN2Y_NUMBER_5_Y_ADDR,
    ICON_MAIN2Y_NUMBER_6_Y_ADDR, ICON_MAIN2Y_NUMBER_7_Y_ADDR, ICON_MAIN2Y_NUMBER_8_Y_ADDR,
    ICON_MAIN2Y_NUMBER_9_Y_ADDR, 
    },
    {
    ICON_MAIN2W_NUMBER_0_ADDR, ICON_MAIN2W_NUMBER_1_ADDR, ICON_MAIN2W_NUMBER_2_ADDR,
    ICON_MAIN2W_NUMBER_3_ADDR, ICON_MAIN2W_NUMBER_4_ADDR, ICON_MAIN2W_NUMBER_5_ADDR,
    ICON_MAIN2W_NUMBER_6_ADDR, ICON_MAIN2W_NUMBER_7_ADDR, ICON_MAIN2W_NUMBER_8_ADDR,
    ICON_MAIN2W_NUMBER_9_ADDR, 
    },
};
const uint8_t icon_main2_date_coord[4][2] = {{1, 0}, {1,9}, {1,23}, {1, 32}};  
 //日期斜杠
const uint32_t icon_main2_date_line_addr[3][1] = {   
    {
    ICON_MAIN2G_DATE_LINE_G1_ADDR
    },
    {
    ICON_MAIN2Y_DATE_LINE_Y_ADDR
    },
    { 
    ICON_MAIN2W_DATE_LINE_ADDR
    }
};
const uint8_t icon_main2_date_line_coord[4][2] = {{0, 17}};   
  //csr3.0    
  
 //百分比数字
const uint32_t icon_main2_progress_number_addr[2][10] = {   
    {
    ICON_MAIN2G_PERCENTAGE_NUMBER_0_G_ADDR, ICON_MAIN2G_PERCENTAGE_NUMBER_1_G_ADDR, ICON_MAIN2G_PERCENTAGE_NUMBER_2_G_ADDR,
    ICON_MAIN2G_PERCENTAGE_NUMBER_3_G_ADDR, ICON_MAIN2G_PERCENTAGE_NUMBER_4_G_ADDR, ICON_MAIN2G_PERCENTAGE_NUMBER_5_G_ADDR,
    ICON_MAIN2G_PERCENTAGE_NUMBER_6_G_ADDR, ICON_MAIN2G_PERCENTAGE_NUMBER_7_G_ADDR, ICON_MAIN2G_PERCENTAGE_NUMBER_8_G_ADDR,
    ICON_MAIN2G_PERCENTAGE_NUMBER_9_G_ADDR,
    },
    { 
    ICON_MAIN2Y_PERCENTAGE_NUMBER_0_Y_ADDR, ICON_MAIN2Y_PERCENTAGE_NUMBER_1_Y_ADDR, ICON_MAIN2Y_PERCENTAGE_NUMBER_2_Y_ADDR,
    ICON_MAIN2Y_PERCENTAGE_NUMBER_3_Y_ADDR, ICON_MAIN2Y_PERCENTAGE_NUMBER_4_Y_ADDR, ICON_MAIN2Y_PERCENTAGE_NUMBER_5_Y_ADDR,
    ICON_MAIN2Y_PERCENTAGE_NUMBER_6_Y_ADDR, ICON_MAIN2Y_PERCENTAGE_NUMBER_7_Y_ADDR, ICON_MAIN2Y_PERCENTAGE_NUMBER_8_Y_ADDR,
    ICON_MAIN2Y_PERCENTAGE_NUMBER_9_Y_ADDR, 
    }
};
const uint8_t icon_main2_progress_number_coord[3][2] = {{35, 125},{25, 125},{18, 125}};   
 //百分比-百分号
const uint32_t icon_main2_percent_sign_addr[2][1] = {   
    {
        ICON_MAIN2G_PERCENTAGE0_SIGN_G_ADDR
    },
    { 
        ICON_MAIN2Y_PERCENTAGE0_SIGN_Y_ADDR
    }
};
const uint8_t icon_main2_percent_sign_coord[3][2] = {{49, 133}, {51, 133}, {57, 133},};   

 //进度-底环
const uint32_t icon_main2_progress_ring_addr[2][2] = {   
    {
        ICON_MAIN2G_PROGRESS0_RINGS_0_G_ADDR, ICON_MAIN2G_PROGRESS0_RINGS_20_G_ADDR
    },
    { 
        ICON_MAIN2Y_PROGRESS0_RINGS_0_Y_ADDR, ICON_MAIN2Y_PROGRESS0_RINGS_20_Y_ADDR
    }
};
const uint8_t icon_main2_progress_ring_coord[1][2] = {{0, 108}};   

 //进度-组合,给出首地址，其他根据大小计算
const uint32_t icon_main2_progress_ring0_addr[2][2] = {   
    {
        ICON_MAIN2G_PROGRESS_RINGS_01_G_ADDR,ICON_MAIN2G_PROGRESS_RINGS_21_G_ADDR
    },
    { 
        ICON_MAIN2Y_PROGRESS_RINGS_01_Y_ADDR, ICON_MAIN2Y_PROGRESS_RINGS_21_Y_ADDR,
    }
};
const uint8_t icon_main2_progress_ring0_coord[19][2] = {
    {0, 136},  {0, 132},  {1, 127},  {3, 122},  {7, 118},
    {11, 114}, {15, 111}, {20, 108}, {25, 108}, {31, 108},
    {37, 108}, {42, 108}, {48, 108}, {54, 111}, {57, 114},
    {61, 118}, {65, 124}, {68, 129}, {68, 134}
};   

 //星期
const uint32_t icon_main2_week_addr[2][14] = {
    {
        ICON_MAIN2G_WEEK_CN_1_ADDR, ICON_MAIN2G_WEEK_CN_2_ADDR, ICON_MAIN2G_WEEK_CN_3_ADDR,
        ICON_MAIN2G_WEEK_CN_4_ADDR, ICON_MAIN2G_WEEK_CN_5_ADDR, ICON_MAIN2G_WEEK_CN_6_ADDR,
        ICON_MAIN2G_WEEK_CN_7_ADDR,
        ICON_MAIN2G_WEEK_EN_1_ADDR, ICON_MAIN2G_WEEK_EN_2_ADDR, ICON_MAIN2G_WEEK_EN_3_ADDR,
        ICON_MAIN2G_WEEK_EN_4_ADDR, ICON_MAIN2G_WEEK_EN_5_ADDR, ICON_MAIN2G_WEEK_EN_6_ADDR,
        ICON_MAIN2G_WEEK_EN_7_ADDR,
    },
    {
        ICON_MAIN2Y_WEEK_CN_1_ADDR, ICON_MAIN2Y_WEEK_CN_2_ADDR, ICON_MAIN2Y_WEEK_CN_3_ADDR,
        ICON_MAIN2Y_WEEK_CN_4_ADDR, ICON_MAIN2Y_WEEK_CN_5_ADDR, ICON_MAIN2Y_WEEK_CN_6_ADDR,
        ICON_MAIN2Y_WEEK_CN_7_ADDR,
        ICON_MAIN2Y_WEEK_EN_1_ADDR, ICON_MAIN2Y_WEEK_EN_2_ADDR, ICON_MAIN2Y_WEEK_EN_3_ADDR,
        ICON_MAIN2Y_WEEK_EN_4_ADDR, ICON_MAIN2Y_WEEK_EN_5_ADDR, ICON_MAIN2Y_WEEK_EN_6_ADDR,
        ICON_MAIN2Y_WEEK_EN_7_ADDR,
    }
};    
const uint8_t icon_main2_week_coord[1][2] = {{20, 149}};

 //计步-鞋子
const uint32_t icon_main2_step_shoes_addr[1][1] = {   
    ICON_MAIN2_SHOES_ADDR
};
const uint8_t icon_main2_step_shoes_coord[1][2] = {{1, 47}};   

 //计步-数字
const uint32_t icon_main2_step_number_addr[2][10] = {   
    {
    ICON_MAIN2_STEP_NUMBER_0_G_ADDR, ICON_MAIN2_STEP_NUMBER_1_G_ADDR, ICON_MAIN2_STEP_NUMBER_2_G_ADDR,
    ICON_MAIN2_STEP_NUMBER_3_G_ADDR, ICON_MAIN2_STEP_NUMBER_4_G_ADDR, ICON_MAIN2_STEP_NUMBER_5_G_ADDR,
    ICON_MAIN2_STEP_NUMBER_6_G_ADDR, ICON_MAIN2_STEP_NUMBER_7_G_ADDR, ICON_MAIN2_STEP_NUMBER_8_G_ADDR,
    ICON_MAIN2_STEP_NUMBER_9_G_ADDR,
    }
};
const uint8_t icon_main2_step_number_coord[6][2] = {{1, 65},{1, 74},{1, 83},{1, 92},{1, 101},{1,109}};  

//主界面-风格三
 //am-pm    
const uint32_t icon_main3_ampm_addr[1][4] = {     
    {  
    ICON_MAIN3_AMPM_AM_CN_ADDR, ICON_MAIN3_AMPM_PM_CN_ADDR,
    ICON_MAIN3_AMPM_AM_EN_ADDR, ICON_MAIN3_AMPM_PM_EN_ADDR     
    }
};
const uint8_t icon_main3_ampm_coord[1][2] = {{38, 0}};  
  //时间数字   
const uint32_t icon_main3_hour_addr[1][10] = { 
    {
        ICON_MAIN3_TIME_0_ADDR, ICON_MAIN3_TIME_1_ADDR, ICON_MAIN3_TIME_2_ADDR,
        ICON_MAIN3_TIME_3_ADDR, ICON_MAIN3_TIME_4_ADDR, ICON_MAIN3_TIME_5_ADDR,
        ICON_MAIN3_TIME_6_ADDR, ICON_MAIN3_TIME_7_ADDR, ICON_MAIN3_TIME_8_ADDR,
        ICON_MAIN3_TIME_9_ADDR,
    }
}; 
const uint8_t icon_main3_hour_coord[4][2] = {{0, 26}, {19, 26}, {44, 26}, {63, 26}};
  //时间冒号
const uint32_t icon_main3_colon_addr[1][1] = { 
    {
        ICON_MAIN3_TIME1_COLON_ADDR,
    }
}; 
const uint8_t icon_main3_colon_coord[1][2] = {{37, 32}};
  //电池电量\充电    
const uint32_t icon_main3_battery_addr[1][2] = {   
    {
    ICON_MAIN3_BATTERY_ADDR, ICON_MAIN3_CHARGING_ADDR
    }
};
const uint8_t icon_main3_battery_coord[1][2] = {{64, 0}};
const uint8_t icon_main3_battery_level_coord[8][2] = {
    {66, 3}, {69, 3}, {72, 3}, {75, 3}, {78, 3},
};
  //ble    
const uint32_t icon_main3_ble_addr[1][2] = {   
    {
    ICON_MAIN3_BLE_UNLINKED_ADDR, ICON_MAIN3_BLE_LINK_ADDR
    }
};
const uint8_t icon_main3_ble_coord[1][2] = {{0, 0}};
   //日期
const uint32_t icon_main3_date_addr[1][10] = {   
    {
    ICON_MAIN3_DATE_0_ADDR, ICON_MAIN3_DATE_1_ADDR, ICON_MAIN3_DATE_2_ADDR,
    ICON_MAIN3_DATE_3_ADDR, ICON_MAIN3_DATE_4_ADDR, ICON_MAIN3_DATE_5_ADDR,
    ICON_MAIN3_DATE_6_ADDR, ICON_MAIN3_DATE_7_ADDR, ICON_MAIN3_DATE_8_ADDR,
    ICON_MAIN3_DATE_9_ADDR, 
    },
};
const uint8_t icon_main3_date_coord[4][2] = {{17, 59}, {25, 59}, {39, 59}, {47, 59}};  
 //日期斜杠
const uint32_t icon_main3_date_line_addr[1][1] = {   
    {
    ICON_MAIN3_DATE1_DASH_ADDR
    },
};
const uint8_t icon_main3_date_line_coord[1][2] = {{31, 59}};   
  //csr3.0    

const uint8_t icon_main3_csr_coord[1][2] = {{22, 0}};   
 //百分比数字
const uint32_t icon_main3_progress_number_addr[1][10] = {   
    {
    ICON_MAIN3_PERCENTAGE_NUMBER_0_ADDR, ICON_MAIN3_PERCENTAGE_NUMBER_1_ADDR, ICON_MAIN3_PERCENTAGE_NUMBER_2_ADDR,
    ICON_MAIN3_PERCENTAGE_NUMBER_3_ADDR, ICON_MAIN3_PERCENTAGE_NUMBER_4_ADDR, ICON_MAIN3_PERCENTAGE_NUMBER_5_ADDR,
    ICON_MAIN3_PERCENTAGE_NUMBER_6_ADDR, ICON_MAIN3_PERCENTAGE_NUMBER_7_ADDR, ICON_MAIN3_PERCENTAGE_NUMBER_8_ADDR,
    ICON_MAIN3_PERCENTAGE_NUMBER_9_ADDR,
    }
};
const uint8_t icon_main3_progress_number_coord[3][2] = {{30, 101},{20, 101},{13, 101}};   
 //百分比-百分号
const uint32_t icon_main3_percent_sign_addr[1][1] = {   
    {
        ICON_MAIN3_PERCENTAGE0_SIGN_ADDR
    }
};
const uint8_t icon_main3_percent_sign_coord[3][2] = { {45, 109},{50, 109},{57, 109}, };   

 //进度-底环
const uint32_t icon_main3_progress_ring_addr[1][2] = {   
    {
        ICON_MAIN3_PROGRESS0_0_ADDR, ICON_MAIN3_PROGRESS0_20_ADDR
    }
};
const uint8_t icon_main3_progress_ring_coord[1][2] = {{0, 80}};   

 //进度-组合,给出首地址，其他根据大小计算
const uint32_t icon_main3_progress_ring0_addr[1][2] = {   
    {
        ICON_MAIN3_PROGRESS_01_ADDR, ICON_MAIN3_PROGRESS_21_ADDR
    }
};
const uint8_t icon_main3_progress_ring0_coord[19][2] = {
    {37, 80},  {49, 80},  {56, 86},  {62, 94},  {62, 105},
    {62, 118}, {62, 129}, {56, 138}, {47, 142}, {36, 142},
    {25, 142}, {13, 142}, {5, 137},  {0, 127},  {0, 115},
    {0, 103},  {0, 94},   {6, 85},   {15, 80},  
        
};   

 //星期
const uint32_t icon_main3_week_addr[1][14] = {
    {
        ICON_MAIN3_WEEK_CN_1_ADDR, ICON_MAIN3_WEEK_CN_2_ADDR, ICON_MAIN3_WEEK_CN_3_ADDR,
        ICON_MAIN3_WEEK_CN_4_ADDR, ICON_MAIN3_WEEK_CN_5_ADDR, ICON_MAIN3_WEEK_CN_6_ADDR,
        ICON_MAIN3_WEEK_CN_7_ADDR,
        ICON_MAIN3_WEEK_EN_1_ADDR, ICON_MAIN3_WEEK_EN_2_ADDR, ICON_MAIN3_WEEK_EN_3_ADDR,
        ICON_MAIN3_WEEK_EN_4_ADDR, ICON_MAIN3_WEEK_EN_5_ADDR, ICON_MAIN3_WEEK_EN_6_ADDR,
        ICON_MAIN3_WEEK_EN_7_ADDR,
    }
};    
const uint8_t icon_main3_week_coord[1][2] = {{56, 58}};

 //计步-数字
const uint32_t icon_main3_step_number_addr[2][10] = {   
    {
    ICON_MAIN3_STEP_NUMBER_0_ADDR, ICON_MAIN3_STEP_NUMBER_1_ADDR, ICON_MAIN3_STEP_NUMBER_2_ADDR,
    ICON_MAIN3_STEP_NUMBER_3_ADDR, ICON_MAIN3_STEP_NUMBER_4_ADDR, ICON_MAIN3_STEP_NUMBER_5_ADDR,
    ICON_MAIN3_STEP_NUMBER_6_ADDR, ICON_MAIN3_STEP_NUMBER_7_ADDR, ICON_MAIN3_STEP_NUMBER_8_ADDR,
    ICON_MAIN3_STEP_NUMBER_9_ADDR,
    }
};
const uint8_t icon_main3_step_number_coord[6][2] = {
    {35, 127},{30, 127},{25, 127},{20, 127},{15, 127},{64, 127}
};    

//主界面-风格四
//am-pm    
const uint32_t icon_main4_ampm_addr[1][4] = {     
    {  
    ICON_MAIN4_AMPM_AM_CN_ADDR, ICON_MAIN4_AMPM_PM_CN_ADDR,
    ICON_MAIN4_AMPM_AM_EN_ADDR, ICON_MAIN4_AMPM_PM_EN_ADDR      
    }
};
const uint8_t icon_main4_ampm_coord[1][2] = {{51, 30}};  
  //时间数字   
const uint32_t icon_main4_hour_addr[2][10] = { 
    {
        ICON_MAIN4_TIME_HOURS_0_ADDR, ICON_MAIN4_TIME_HOURS_1_ADDR, ICON_MAIN4_TIME_HOURS_2_ADDR,
        ICON_MAIN4_TIME_HOURS_3_ADDR, ICON_MAIN4_TIME_HOURS_4_ADDR, ICON_MAIN4_TIME_HOURS_5_ADDR,
        ICON_MAIN4_TIME_HOURS_6_ADDR, ICON_MAIN4_TIME_HOURS_7_ADDR, ICON_MAIN4_TIME_HOURS_8_ADDR,
        ICON_MAIN4_TIME_HOURS_9_ADDR,
    },
    {
        ICON_MAIN4_TIME1_MIN_0_ADDR, ICON_MAIN4_TIME1_MIN_1_ADDR, ICON_MAIN4_TIME1_MIN_2_ADDR,
        ICON_MAIN4_TIME1_MIN_3_ADDR, ICON_MAIN4_TIME1_MIN_4_ADDR, ICON_MAIN4_TIME1_MIN_5_ADDR,
        ICON_MAIN4_TIME1_MIN_6_ADDR, ICON_MAIN4_TIME1_MIN_7_ADDR, ICON_MAIN4_TIME1_MIN_8_ADDR,
        ICON_MAIN4_TIME1_MIN_9_ADDR,        
    }
}; 
const uint8_t icon_main4_hour_coord[4][2] = {{0, 30}, {25, 30}, {51, 44}, {66, 44}};
  //电池电量\充电    
const uint32_t icon_main4_battery_addr[1][2] = {   
    {
    ICON_MAIN4_BATTERY_ADDR, ICON_MAIN4_CHARGING_ADDR
    }
};
const uint8_t icon_main4_battery_coord[1][2] = {{64, 0}};
const uint8_t icon_main4_battery_level_coord[5][2] = {
    {66, 3}, {69, 3}, {72, 3}, {75, 3}, {78, 3}
};
  //ble    
const uint32_t icon_main4_ble_addr[1][2] = {   
    {
    ICON_MAIN4_BLE_UNLINKED_ADDR, ICON_MAIN4_BLE_LINK_ADDR
    }
};
const uint8_t icon_main4_ble_coord[1][2] = {{0, 0}};
   //日期
const uint32_t icon_main4_date_addr[1][10] = {   
    {
    ICON_MAIN4_DATE_0_ADDR, ICON_MAIN4_DATE_1_ADDR, ICON_MAIN4_DATE_2_ADDR,
    ICON_MAIN4_DATE_3_ADDR, ICON_MAIN4_DATE_4_ADDR, ICON_MAIN4_DATE_5_ADDR,
    ICON_MAIN4_DATE_6_ADDR, ICON_MAIN4_DATE_7_ADDR, ICON_MAIN4_DATE_8_ADDR,
    ICON_MAIN4_DATE_9_ADDR, 
    },
};
const uint8_t icon_main4_date_coord[2][2] = {{20, 0}, {27, 0}};    
  //csr3.0    

const uint8_t icon_main4_csr_coord[1][2] = {{51, 30}};   
 //百分比数字
const uint32_t icon_main4_progress_number_addr[1][10] = {   
    {
    ICON_MAIN4_PERCENTAGE_NUMBER_0_ADDR, ICON_MAIN4_PERCENTAGE_NUMBER_1_ADDR, ICON_MAIN4_PERCENTAGE_NUMBER_2_ADDR,
    ICON_MAIN4_PERCENTAGE_NUMBER_3_ADDR, ICON_MAIN4_PERCENTAGE_NUMBER_4_ADDR, ICON_MAIN4_PERCENTAGE_NUMBER_5_ADDR,
    ICON_MAIN4_PERCENTAGE_NUMBER_6_ADDR, ICON_MAIN4_PERCENTAGE_NUMBER_7_ADDR, ICON_MAIN4_PERCENTAGE_NUMBER_8_ADDR,
    ICON_MAIN4_PERCENTAGE_NUMBER_9_ADDR,
    }
};
const uint8_t icon_main4_progress_number_coord[3][2] = {{30, 101},{20, 101},{13, 101}};   
 //百分比-百分号
const uint32_t icon_main4_percent_sign_addr[1][1] = {   
    {
        ICON_MAIN4_PERCENTAGE0_SIGN_ADDR
    }
};
const uint8_t icon_main4_percent_sign_coord[3][2] = { {45, 109},{50, 109},{57, 109}, };   

 //进度-底环
const uint32_t icon_main4_progress_ring_addr[1][2] = {   
    {
        ICON_MAIN4_PROGRESS0_0_ADDR, ICON_MAIN4_PROGRESS0_20_ADDR
    }
};
const uint8_t icon_main4_progress_ring_coord[1][2] = {{0, 80}};   

 //进度-组合,给出首地址，其他根据大小计算
const uint32_t icon_main4_progress_ring0_addr[1][2] = {   
    {
        ICON_MAIN4_PROGRESS_01_ADDR, ICON_MAIN4_PROGRESS_21_ADDR
    }
};
const uint8_t icon_main4_progress_ring0_coord[19][2] = {
    {37, 80},  {49, 80},  {56, 86},  {62, 94},  {62, 105},
    {62, 118}, {62, 129}, {56, 138}, {47, 142}, {36, 142},
    {25, 142}, {13, 142}, {5, 137},  {0, 127},  {0, 115},
    {0, 103},  {0, 94},   {6, 85},   {15, 80},  
        
};   

 //星期
const uint32_t icon_main4_week_addr[1][14] = {
    {
        ICON_MAIN4_WEEK_CN_1_ADDR, ICON_MAIN4_WEEK_CN_2_ADDR, ICON_MAIN4_WEEK_CN_3_ADDR,
        ICON_MAIN4_WEEK_CN_4_ADDR, ICON_MAIN4_WEEK_CN_5_ADDR, ICON_MAIN4_WEEK_CN_6_ADDR,
        ICON_MAIN4_WEEK_CN_7_ADDR,
        ICON_MAIN4_WEEK_EN_1_ADDR, ICON_MAIN4_WEEK_EN_2_ADDR, ICON_MAIN4_WEEK_EN_3_ADDR,
        ICON_MAIN4_WEEK_EN_4_ADDR, ICON_MAIN4_WEEK_EN_5_ADDR, ICON_MAIN4_WEEK_EN_6_ADDR,
        ICON_MAIN4_WEEK_EN_7_ADDR,
    }
};    
const uint8_t icon_main4_week_coord[1][2] = {{36, 0}};

 //计步-数字
const uint32_t icon_main4_step_number_addr[2][10] = {   
    {
    ICON_MAIN3_STEP_NUMBER_0_ADDR, ICON_MAIN3_STEP_NUMBER_1_ADDR, ICON_MAIN3_STEP_NUMBER_2_ADDR,
    ICON_MAIN3_STEP_NUMBER_3_ADDR, ICON_MAIN3_STEP_NUMBER_4_ADDR, ICON_MAIN3_STEP_NUMBER_5_ADDR,
    ICON_MAIN3_STEP_NUMBER_6_ADDR, ICON_MAIN3_STEP_NUMBER_7_ADDR, ICON_MAIN3_STEP_NUMBER_8_ADDR,
    ICON_MAIN3_STEP_NUMBER_9_ADDR,
    }
};
const uint8_t icon_main4_step_number_coord[6][2] = {
    {35, 127},{30, 127},{25, 127},{20, 127},{15, 127},{64, 127}
};

// 女性功能
const uint8_t icon_main4_menstrual_coord[1][2] = {{65,73}};

//主界面-风格五

//  //电池电量\充电    
//const uint32_t icon_main5_battery_addr[2][2] = {   
//    {
//    ICON_MAIN5W_BATTERY_ADDR, ICON_MAIN5W_CHARGING_ADDR
//    },
//    {
//    ICON_MAIN5_BATTERY_ADDR, ICON_MAIN5_CHARGING_ADDR
//    }
//};
//const uint8_t icon_main5_battery_coord[1][2] = {{62, 0}};
//const uint8_t icon_main5_battery_level_coord[5][2] = {
//    {64, 3}, {68, 3}, {71, 3}, {74, 3}, {78, 3},
//};
//  //ble    
//const uint32_t icon_main5_ble_addr[2][2] = {  
//    {
//    ICON_MAIN5W_BLE_UNLINKED_ADDR, ICON_MAIN5W_BLE_LINK_ADDR
//    },
//    {
//    ICON_MAIN5_BLE_UNLINKED_ADDR, ICON_MAIN5_BLE_LINK_ADDR
//    }
//};
//const uint8_t icon_main5_ble_coord[1][2] = {{0, 0}};
//   //日期
//const uint32_t icon_main5_date_addr[2][10] = {   
//    {
//    ICON_MAIN5W_DATE_0_ADDR, ICON_MAIN5W_DATE_1_ADDR, ICON_MAIN5W_DATE_2_ADDR,
//    ICON_MAIN5W_DATE_3_ADDR, ICON_MAIN5W_DATE_4_ADDR, ICON_MAIN5W_DATE_5_ADDR,
//    ICON_MAIN5W_DATE_6_ADDR, ICON_MAIN5W_DATE_7_ADDR, ICON_MAIN5W_DATE_8_ADDR,
//    ICON_MAIN5W_DATE_9_ADDR, 
//    }, 
//    {
//    ICON_MAIN5_DATE_COLOR_0_ADDR, ICON_MAIN5_DATE_COLOR_1_ADDR, ICON_MAIN5_DATE_COLOR_2_ADDR,
//    ICON_MAIN5_DATE_COLOR_3_ADDR, ICON_MAIN5_DATE_COLOR_4_ADDR, ICON_MAIN5_DATE_COLOR_5_ADDR,
//    ICON_MAIN5_DATE_COLOR_6_ADDR, ICON_MAIN5_DATE_COLOR_7_ADDR, ICON_MAIN5_DATE_COLOR_8_ADDR,
//    ICON_MAIN5_DATE_COLOR_9_ADDR, 
//    }
//};
//const uint8_t icon_main5_date_coord[4][2] = {{24, 130}, {31, 130}, {43, 130}, {50, 130}};

// //日期斜杠
//const uint32_t icon_main5_date_line_addr[2][1] = {   
//    ICON_MAIN5W_DATE1_SLASH_ADDR, ICON_MAIN5_DATE1_SLASH_ADDR
//};   
//const uint8_t icon_main5_date_line_coord[1][2] = {{37, 129}};  
//  //csr3.0    

//const uint8_t icon_main5_csr_coord[1][2] = {{28, 28}};
// //星期
//const uint32_t icon_main5_week_addr[2][14] = {
//    {
//    ICON_MAIN5W_WEEK_CN_1_ADDR, ICON_MAIN5W_WEEK_CN_2_ADDR, ICON_MAIN5W_WEEK_CN_3_ADDR,
//    ICON_MAIN5W_WEEK_CN_4_ADDR, ICON_MAIN5W_WEEK_CN_5_ADDR, ICON_MAIN5W_WEEK_CN_6_ADDR,
//    ICON_MAIN5W_WEEK_CN_7_ADDR,
//    ICON_MAIN5W_WEEK_EN_1_ADDR, ICON_MAIN5W_WEEK_EN_2_ADDR, ICON_MAIN5W_WEEK_EN_3_ADDR,
//    ICON_MAIN5W_WEEK_EN_4_ADDR, ICON_MAIN5W_WEEK_EN_5_ADDR, ICON_MAIN5W_WEEK_EN_6_ADDR,
//    ICON_MAIN5W_WEEK_EN_7_ADDR,
//    },
//    {
//    ICON_MAIN5_WEEK_COLOR_CN_1_ADDR, ICON_MAIN5_WEEK_COLOR_CN_2_ADDR, ICON_MAIN5_WEEK_COLOR_CN_3_ADDR,
//    ICON_MAIN5_WEEK_COLOR_CN_4_ADDR, ICON_MAIN5_WEEK_COLOR_CN_5_ADDR, ICON_MAIN5_WEEK_COLOR_CN_6_ADDR,
//    ICON_MAIN5_WEEK_COLOR_CN_7_ADDR,
//    ICON_MAIN5_WEEK_COLOR_EN_1_ADDR, ICON_MAIN5_WEEK_COLOR_EN_2_ADDR, ICON_MAIN5_WEEK_COLOR_EN_3_ADDR,
//    ICON_MAIN5_WEEK_COLOR_EN_4_ADDR, ICON_MAIN5_WEEK_COLOR_EN_5_ADDR, ICON_MAIN5_WEEK_COLOR_EN_6_ADDR,
//    ICON_MAIN5_WEEK_COLOR_EN_7_ADDR,
//    }
//};    
//const uint8_t icon_main5_week_coord[1][2] = {{28, 28}};
////表盘背景
//const uint32_t icon_main5_dial_bg_addr[2][1] = {       
//    ICON_MAIN5W_BG_ADDR, ICON_MAIN5_BG_ADDR
//};
//const uint8_t icon_main5_dial_bg_coord[1][2] = {{0, 14}};
//// 女性功能
//const uint8_t icon_main5_menstrual_coord[1][2] = {{34,0}};
//const uint32_t icon_main5_menstrual_addr[1][4] = 
//{
//    ICON_MAIN5_ICON1_1_ADDR, ICON_MAIN5_ICON1_2_ADDR, ICON_MAIN5_ICON1_3_ADDR,
//    ICON_MAIN5_ICON1_4_ADDR
//};


// 运动模式 
  //1
const uint32_t icon_16_move1_run_man_addr[1] = {ICON_MOVE1_ICON1_ADDR};
const uint8_t icon_16_move1_run_man_coord[1][2] = {{28, 19}};

const uint32_t icon_16_move1_run_line_addr[1] = {ICON_MOVE1_ICON2_ADDR};
const uint8_t icon_16_move1_run_line_coord[3][2] = {{1, 43}, {3, 43}, {5, 43}};

const uint32_t icon_16_move1_run_text_addr[2] = {ICON_MOVE1_TEXT_CN_ADDR, ICON_MOVE1_TEXT_EN_ADDR};
const uint8_t icon_16_move1_run_text_coord[1][2] = {{15, 121}};
  //2-数据面板
const uint32_t icon_16_move2_calories_icon_addr[1] = {ICON_MOVE_ICON1_ADDR};
const uint8_t icon_16_move2_calories_icon_coord[1][2] = {{1, 103}};

const uint32_t icon_16_move2_calories_unit_addr[2] = {ICON_MOVE_UNIT2_KCAL_CN_ADDR, ICON_MOVE_UNIT2_KCAL_EN_ADDR};
const uint8_t icon_16_move2_calories_unit_coord[1][2] = {{21, 108}};

const uint32_t icon_16_move2_data_addr[10] = 
{
	ICON_MOVE_DATE1_0_ADDR, ICON_MOVE_DATE1_1_ADDR, ICON_MOVE_DATE1_2_ADDR, ICON_MOVE_DATE1_3_ADDR, ICON_MOVE_DATE1_4_ADDR,
	ICON_MOVE_DATE1_5_ADDR, ICON_MOVE_DATE1_6_ADDR, ICON_MOVE_DATE1_7_ADDR, ICON_MOVE_DATE1_8_ADDR, ICON_MOVE_DATE1_9_ADDR
};
const uint8_t icon_16_move2_data_coord[4][2] = {{21, 46}, {21, 77}, {21, 108}, {21, 139},};

const uint32_t icon_16_move2_data_point_addr[1] = {ICON_MOVE_DATE3_POINT_ADDR};
const uint8_t icon_16_move2_data_point_coord[1][2] = {{0, 86}};

const uint32_t icon_16_move2_distance_icon_addr[1] = {ICON_MOVE_ICON2_ADDR};
const uint8_t icon_16_move2_distance_icon_coord[1][2] = {{1, 73}};

const uint32_t icon_16_move2_distance_unit_addr[4] = {ICON_MOVE_UINT_KM_CN_ADDR, ICON_MOVE_UINT_KM_EN_ADDR, ICON_MOVE_UINT_MI_ADDR, ICON_MOVE_UINT_MI_CN_ADDR};
const uint8_t icon_16_move2_distance_unit_coord[1][2] = {{0, 78}};

const uint32_t icon_16_move2_heart_icon_addr[1] = {ICON_MOVE_ICON3_ADDR};
const uint8_t icon_16_move2_heart_icon_coord[1][2] = {{2, 46}};

const uint32_t icon_16_move2_heart_nodata_addr[1] = {ICON_MOVE_DATA2_NONE_ADDR};
const uint8_t icon_16_move2_heart_nodata_coord[1][2] = {{22, 52}};

const uint32_t icon_16_move2_heart_unit_addr[2] = {ICON_MOVE_UNIT1_BPM_CN_ADDR, ICON_MOVE_UNIT1_BPM_EN_ADDR};
const uint8_t icon_16_move2_heart_unit_coord[1][2] = {{0, 46}};

const uint32_t icon_16_move2_step_icon_addr[1] = {ICON_MOVE_ICON4_ADDR};
const uint8_t icon_16_move2_step_icon_coord[1][2] = {{1, 139}};

const uint32_t icon_16_move2_line_addr[1] = {ICON_MOVE_ICON5_LINE_ADDR};
const uint8_t icon_16_move2_line_coord[4][2] = {{1, 36}, {1, 67}, {1, 98}, {1, 129}};

const uint32_t icon_16_move2_timing_addr[10] = 
{
	ICON_MOVE_TIME_0_ADDR, ICON_MOVE_TIME_1_ADDR, ICON_MOVE_TIME_2_ADDR, ICON_MOVE_TIME_3_ADDR, ICON_MOVE_TIME_4_ADDR,
	ICON_MOVE_TIME_5_ADDR, ICON_MOVE_TIME_6_ADDR, ICON_MOVE_TIME_7_ADDR, ICON_MOVE_TIME_8_ADDR, ICON_MOVE_TIME_9_ADDR
};
const uint8_t icon_16_move2_timing_coord[6][2] = {{0, 13}, {12, 13}, {28, 13}, {40, 13}, {56, 13}, {68, 13},};

const uint32_t icon_16_move2_timing_colon_addr[1] = {ICON_MOVE_TIME1_COLON_ADDR};
const uint8_t icon_16_move2_timing_colon_coord[2][2] = {{24, 16}, {52, 16}};
  //3-继续
const uint32_t icon_16_move3_goon_icon_addr[1] = {ICON_MOVE2_ICON_ADDR};
const uint8_t icon_16_move3_goon_icon_coord[1][2] = {{4, 18}};

const uint32_t icon_16_move3_goon_text_addr[2] = {ICON_MOVE2_TEXT_CN_ADDR, ICON_MOVE2_TEXT_EN_ADDR};
const uint8_t icon_16_move3_goon_text_coord[1][2] = {{2, 121}};
  //4-暂停
const uint32_t icon_16_move4_pause_icon_addr[1] = {ICON_MOVE3_ICON_ADDR};
const uint8_t icon_16_move4_pause_icon_coord[1][2] = {{4, 18}};

const uint32_t icon_16_move4_pause_text_addr[2] = {ICON_MOVE3_TEXT_CN_ADDR, ICON_MOVE3_TEXT_EN_ADDR};
const uint8_t icon_16_move4_pause_text_coord[1][2] = {{1, 121}};
  //5-退出
const uint32_t icon_16_move5_exit_icon_addr[1] = {ICON_MOVE4_ICON_ADDR};
const uint8_t icon_16_move5_exit_icon_coord[1][2] = {{4, 18}};

const uint32_t icon_16_move5_exit_text_addr[2] = {ICON_MOVE4_TEXT_CN_ADDR, ICON_MOVE4_TEXT_EN_ADDR};
const uint8_t icon_16_move5_exit_text_coord[1][2] = {{15, 121}};


// 血氧环境监测//背景进度条
const uint32_t icon_16_spO2_icon4_addr[1] = {ICON_SP02_ICON4_ADDR};
const uint8_t  icon_16_spO2_icon4_coord[1][2] = {1, 126};
// 亮度进度条
const uint32_t icon_16_spO2_icon3_addr[1] = {ICON_SP02_ICON3_ADDR};
const uint8_t  icon_16_spO2_icon3_coord[1][2] = {1, 126};
// 进度值
const uint8_t  icon_16_spO2_data1_coord[1][2] = {10, 139};
// %符号
const uint32_t icon_16_spO2_icon5_addr[1] = {ICON_SP02_ICON5_ADDR};
const uint8_t  icon_16_spO2_icon5_coord[3][2] = {{51, 140},{45, 140},{40, 140}};
// X符号
const uint32_t icon_16_spO2_error_addr[1] = {ICON_SP02ERROR_ICON_2_ADDR};
const uint8_t  icon_16_spO2_error_coord[2][2] = {{17, 123},{17 +ICON_SP02ERROR_ICON_WIDE+2, 123}};


// 血氧
const uint32_t icon_16_spO2_icon1_addr[2] = {ICON_SP02_ICON1_1_ADDR, ICON_SP02_ICON1_2_ADDR};
const uint8_t  icon_16_spO2_icon1_coord[1][2] = {9, 0};
// SPO2
const uint32_t icon_16_spO2_unit1_addr[1] = {ICON_SP02_UNIT_ADDR};
const uint8_t  icon_16_spO2_unit1_coord[1][2] = {17, 84};
// 血氧值
const uint32_t icon_16_spO2_data_addr[10] = {ICON_SP02_NUM_0_ADDR, ICON_SP02_NUM_1_ADDR, ICON_SP02_NUM_2_ADDR,
                                             ICON_SP02_NUM_3_ADDR, ICON_SP02_NUM_4_ADDR, ICON_SP02_NUM_5_ADDR,
                                             ICON_SP02_NUM_6_ADDR, ICON_SP02_NUM_7_ADDR, ICON_SP02_NUM_8_ADDR,
                                             ICON_SP02_NUM_9_ADDR}; 
const uint8_t  icon_16_spO2_data2_coord[1][2] = {8, 117};
// %号
const uint32_t icon_16_spO2_icon2_addr[1] = {ICON_SP02_ICON2_ADDR};
const uint8_t  icon_16_spO2_icon2_coord[2][2] ={{53, 123},{45, 123}};


// 低氧报警
const uint32_t icon_16_sp02low_icon_addr[2] = {ICON_SP02LOW_ICON1_ADDR, ICON_SP02LOW_ICON2_ADDR};
const uint8_t  icon_16_sp02low_icon_coord[1][2] ={6, 37};


/* 血压 */
const uint32_t icon_16_bp_bar_addr[1] = {ICON_BP_ICON3_ADDR};
const uint8_t icon_16_bp_bar_coord[5][2] = {{8, 102}, {30, 102}, {52, 102}, {19, 140}, {41, 140}};

const uint32_t icon_16_bp_contour_addr[2] = {ICON_BP_ICON1_1_ADDR, ICON_BP_ICON1_P_ADDR};
const uint8_t icon_16_bp_contour_coord[1][2] = {{2, 0}};

const uint32_t icon_16_bp_round_addr[8] =
{
	ICON_BP_ICON2_1_ADDR, ICON_BP_ICON2_2_ADDR, ICON_BP_ICON2_3_ADDR, ICON_BP_ICON2_4_ADDR,
	ICON_BP_ICON2_5_ADDR, ICON_BP_ICON2_6_ADDR, ICON_BP_ICON2_7_ADDR, ICON_BP_ICON2_8_ADDR
};
const uint8_t icon_16_bp_round_coord[1][2] = {{4, 27}};

const uint32_t icon_16_bp_unit_addr[1] = {ICON_BP_UNIT_ADDR};
const uint8_t icon_16_bp_unit_coord[1][2] = {{13, 66}};

const uint32_t icon_16_bp_xicon_addr[1] = {ICON_BP_ICON4_ADDR};
const uint8_t icon_16_bp_xicon_coord[5][2] = {{7, 92}, {30, 92}, {53, 92}, {18, 131}, {41, 131}};

const uint32_t icon_16_bp_data_addr[10] = {
    ICON_BP_DATA_0_ADDR, ICON_BP_DATA_1_ADDR, ICON_BP_DATA_2_ADDR,
    ICON_BP_DATA_3_ADDR, ICON_BP_DATA_4_ADDR, ICON_BP_DATA_5_ADDR,
    ICON_BP_DATA_6_ADDR, ICON_BP_DATA_7_ADDR, ICON_BP_DATA_8_ADDR,
    ICON_BP_DATA_9_ADDR}; 
const uint8_t icon_16_bp_data_coord[4][2] = {{6, 88}, {6, 127}, {18, 88}, {18, 127}};
/***********/

