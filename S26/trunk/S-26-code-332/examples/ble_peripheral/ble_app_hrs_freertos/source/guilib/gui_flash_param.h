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

#include "configuration.h"

#ifndef _GUI_FLASH_PARAM_H_
#define _GUI_FLASH_PARAM_H_

/* 功能界面数字 */
extern const uint32_t icon_16_function_digital_addr[12];

/* 计步 */
extern const uint32_t icon_16_step_icon_addr[1];
extern const uint32_t icon_16_step_icon_coord[4][2];

extern const uint32_t icon_16_step_prog_addr[20];
extern const uint32_t icon_16_step_prog_coord[20][2];

extern const uint32_t icon_16_step_ring_addr[1];
extern const uint32_t icon_16_step_ring_coord[1][2];

extern const uint32_t icon_16_step_unit_addr[2];
extern const uint32_t icon_16_step_unit_coord[1][2];

extern const uint8_t icon_16_step_data_coord[3][2];

/* 距离 */
extern const uint32_t icon_16_distance_map1_addr[1];
extern const uint8_t icon_16_distance_map1_coord[4][2];

extern const uint32_t icon_16_distance_map2_addr[1];
extern const uint8_t icon_16_distance_map2_coord[4][2];//30//33

extern const uint32_t icon_16_distance_ring_addr[1];
extern const uint8_t icon_16_distance_ring_coord[1][2];
    
extern const uint32_t icon_16_distance_unit_addr[4];
extern const uint8_t icon_16_distance_unit_coord[1][2];

extern const uint32_t icon_16_distance_point_addr[1];
extern const uint8_t icon_16_distance_point_coord[1][2];

extern const uint8_t icon_16_distance_data_coord[2][2];

/* 卡路里 */
extern const uint32_t icon_16_calories_contour_addr[1];
extern const uint8_t icon_16_calories_contour_coord[1][2];
    
extern const uint32_t icon_16_calories_flame_addr[4];
extern const uint8_t icon_16_calories_flame_coord[1][2];

extern const uint32_t icon_16_calories_unit_addr[2];
extern const uint8_t icon_16_calories_unit_coord[1][2];

extern const uint8_t icon_16_calories_data_coord[4][2];

/* 运动模式 */


/* 心率 */
extern const uint32_t icon_16_heart_contour_addr[1];
extern const uint8_t icon_16_heart_contour_coord[1][2];

extern const uint32_t icon_16_heart_line_addr[7];
extern const uint8_t icon_16_heart_line_coord[1][2];

extern const uint32_t icon_16_heart_unit_addr[2];
extern const uint8_t icon_16_heart_unit_coord[1][2];

extern const uint8_t icon_16_heart_data_coord[1][2];


/* 睡眠 */
extern const uint32_t icon_16_sleep_icon_addr[4];
extern const uint8_t icon_16_sleep_icon_coord[1][2];

extern const uint32_t icon_16_sleep_unit_addr[2];
extern const uint8_t icon_16_sleep_unit_coord[1][2];

extern const uint32_t icon_16_sleep_point_addr[1];
extern const uint8_t icon_16_sleep_point_coord[1][2];

extern const uint8_t icon_16_sleep_data_coord[2][2];


/* 找手机 */
extern const uint32_t icon_16_findphone_magnifier_addr[5];
extern const uint8_t icon_16_findphone_magnifier_coord[1][2];

extern const uint32_t icon_16_findphone_phone_addr[1];
extern const uint8_t icon_16_findphone_phone_coord[1][2];

/* 秒表 */
extern const uint32_t icon_16_stopwatch_contour_addr[1];
extern const uint8_t icon_16_stopwatch_contour_coord[6][2];

extern const uint32_t icon_16_stopwatch_pointer_addr[8];
extern const uint8_t icon_16_stopwatch_pointer_coord[1][2];

extern const uint8_t icon_16_stopwatch_data_coord[6][2];
extern const uint32_t icon_16_stopwatch_number1_addr[10]; 
extern const uint32_t icon_16_stopwatch_number2_addr[10];
extern const uint32_t icon_16_stopwatch_number3_addr[10];

/* 倒计时 */
extern const uint32_t icon_16_countdown_icon_addr[6];
extern const uint8_t icon_16_countdown_icon_coord[1][2];


/* 关机 */
extern const uint32_t icon_16_poweroff_icon_addr[1];
extern const uint8_t icon_16_poweroff_icon_coord[1][2];


/* 充电 */
extern const uint32_t icon_16_chg_bar_addr[7];
extern const uint8_t icon_16_chg_bar_coord[1][2];

extern const uint32_t icon_16_chg_contour_addr[1];
extern const uint8_t icon_16_chg_contour_coord[1][2];

/* 闹钟 */
extern const uint32_t icon_16_clock_ampm_addr[4];
extern const uint8_t icon_16_clock_ampm_coord[1][2];

extern const uint32_t icon_16_clock_contour_addr[1];
extern const uint8_t icon_16_clock_contour_coord[1][2];

extern const uint32_t icon_16_clock_pointer_addr[7];
extern const uint8_t icon_16_clock_pointer_coord[1][2];

extern const uint32_t icon_16_clock_number_addr[10];

extern const uint32_t icon_16_remindclock_icon_addr[20];
extern const uint8_t icon_16_remindclock_icon_coord[2][2];

extern const uint8_t icon_16_clock_data_coord[4][2];

extern const uint32_t icon_16_clock_colon_addr[1];
extern const uint8_t icon_16_clock_colon_coord[1][2];

/* 久坐 */
extern const uint32_t icon_16_sit_icon_addr[2];
extern const uint8_t icon_16_sit_icon_coord[1][2];


/* 消息推送 */
extern const uint32_t icon_16_inform_icon_addr[13];
extern const uint8_t icon_16_inform_icon_coord[2][2];

extern const uint8_t icon_16_inform_text_coord[1][2];

extern const uint32_t icon_16_call_arr_addr[3];
extern const uint8_t icon_16_call_arr_coord[1][2];

extern const uint32_t icon_16_call_hangup_addr[1];
extern const uint8_t icon_16_call_hangup_coord[1][2];

extern const uint32_t icon_16_call_mute_addr[1];
extern const uint8_t icon_16_call_mute_coord[1][2];

extern const uint32_t icon_16_call_phone_addr[1];
extern const uint8_t icon_16_call_phone_coord[1][2];

extern const uint8_t icon_16_call_text_coord[1][2];


/* 心率报警 */
extern const uint32_t icon_16_heartalert_contour_addr[1];
extern const uint8_t icon_16_heartalert_contour_coord[1][2];

extern const uint32_t icon_16_heartalert_round_addr[5];
extern const uint8_t icon_16_heartalert_round_coord[1][2];


/* 照相 */
extern const uint32_t icon_16_camera_contour_addr[1];
extern const uint8_t icon_16_camera_contour_coord[1][2];

extern const uint32_t icon_16_camera_round_addr[3];
extern const uint8_t icon_16_camera_round_coord[1][2];


/* 低电 */
extern const uint32_t icon_16_lowpower_contour_addr[1];
extern const uint8_t icon_16_lowpower_contour_coord[1][2];

extern const uint32_t icon_16_lowpower_lightning_addr[1];
extern const uint8_t icon_16_lowpower_lightning_coord[1][2];

/* CSR*/
extern const uint32_t icon_16_pairing_earphone_addr[1];
extern const uint8_t icon_16_pairing_earphone_coord[1][2];

extern const uint32_t icon_16_pairing_dots_addr[16];
extern const uint8_t icon_16_pairing_dots_coord[1][2];

extern const uint32_t icon_16_pairing_state_addr[2];
extern const uint8_t icon_16_pairing_state_coord[1][2];

/* 运动达标*/
extern const uint32_t icon_16_target_icon_addr[2];
extern const uint8_t icon_16_target_icon_coord[2][2];



//主界面-风格一
   //背景
extern const uint32_t icon_main1_bar_addr[2][1];
extern const uint8_t icon_main1_bar_coord[1][2];
  //电池电量\充电    
extern const uint32_t icon_main1_battery_addr[2][2];
extern const uint8_t icon_main1_battery_coord[1][2];   
extern const uint8_t icon_main1_battery_level_coord[5][2];
  //ble    
extern const uint32_t icon_main1_ble_addr[2][2];
extern const uint8_t icon_main1_ble_coord[1][2];
   //日期
extern const uint32_t icon_main1_date_addr[2][10];
extern const uint8_t icon_main1_date_coord[2][2];     
  //csr3.0    
       
  //时间数字   
extern const uint32_t icon_main1_hour_addr[2][18]; 
extern const uint8_t icon_main1_hour_coord[4][2];
  //星期
extern const uint32_t icon_main1_week_addr[2][14];    
extern const uint8_t icon_main1_week_coord[1][2];  
// 女性功能
extern const uint8_t icon_main1_menstrual_coord[1][2];
extern const uint32_t icon_main1_menstrual_addr[2][20];
    
//主界面-风格二
 //am-pm    
extern const uint32_t icon_main2_ampm_addr[3][2];
extern const uint8_t icon_main2_ampm_coord[1][2];  
  //时间数字   
extern const uint32_t icon_main2_hour_addr[3][18]; 
extern const uint8_t icon_main2_hour_coord[4][2];
  //电池电量\充电    
extern const uint32_t icon_main2_battery_addr[2][2];
extern const uint8_t icon_main2_battery_coord[1][2];
extern const uint8_t icon_main2_battery_level_coord[5][2];
  //ble    
extern const uint32_t icon_main2_ble_addr[2][2];
extern const uint8_t icon_main2_ble_coord[1][2];
   //日期
extern const uint32_t icon_main2_date_addr[3][10];
extern const uint8_t icon_main2_date_coord[4][2];  
 //日期斜杠
extern const uint32_t icon_main2_date_line_addr[3][1];
extern const uint8_t icon_main2_date_line_coord[4][2];   
  //csr3.0    
  
 //百分比数字
extern const uint32_t icon_main2_progress_number_addr[2][10];
extern const uint8_t icon_main2_progress_number_coord[3][2];   
 //百分比-百分号
extern const uint32_t icon_main2_percent_sign_addr[2][1];
extern const uint8_t icon_main2_percent_sign_coord[3][2];   
 //进度-底环
extern const uint32_t icon_main2_progress_ring_addr[2][2];
extern const uint8_t icon_main2_progress_ring_coord[1][2];   
 //进度-组合,给出首地址，其他根据大小计算
extern const uint32_t icon_main2_progress_ring0_addr[2][2];
extern const uint8_t icon_main2_progress_ring0_coord[19][2];   
 //星期
extern const uint32_t icon_main2_week_addr[2][14];    
extern const uint8_t icon_main2_week_coord[1][2];
 //计步-鞋子
extern const uint32_t icon_main2_step_shoes_addr[1][1];
extern const uint8_t icon_main2_step_shoes_coord[1][2];   
 //计步-数字
extern const uint32_t icon_main2_step_number_addr[2][10];
extern const uint8_t icon_main2_step_number_coord[6][2];  
// 女性功能
extern const uint8_t icon_main2_menstrual_coord[1][2];
extern const uint32_t icon_main2_menstrual_addr[1][4];

//主界面-风格三
//am-pm    
extern const uint32_t icon_main3_ampm_addr[1][4];
extern const uint8_t icon_main3_ampm_coord[1][2];  
  //时间数字   
extern const uint32_t icon_main3_hour_addr[1][10]; 
extern const uint8_t icon_main3_hour_coord[4][2];
  //时间分号
extern const uint32_t icon_main3_colon_addr[1][1]; 
extern const uint8_t icon_main3_colon_coord[1][2];
  //电池电量\充电    
extern const uint32_t icon_main3_battery_addr[1][2];
extern const uint8_t icon_main3_battery_coord[1][2];
extern const uint8_t icon_main3_battery_level_coord[5][2];
  //ble    
extern const uint32_t icon_main3_ble_addr[1][2];
extern const uint8_t icon_main3_ble_coord[1][2];
   //日期
extern const uint32_t icon_main3_date_addr[1][10];
extern const uint8_t icon_main3_date_coord[4][2];  
 //日期斜杠
extern const uint32_t icon_main3_date_line_addr[3][10] ;
extern const uint8_t icon_main3_date_line_coord[1][2];   
  //csr3.0    
   
 //百分比数字
extern const uint32_t icon_main3_progress_number_addr[1][10];
extern const uint8_t icon_main3_progress_number_coord[3][2];   
 //百分比-百分号
extern const uint32_t icon_main3_percent_sign_addr[1][1];
extern const uint8_t icon_main3_percent_sign_coord[3][2];   
 //进度-底环
extern const uint32_t icon_main3_progress_ring_addr[1][2];
extern const uint8_t icon_main3_progress_ring_coord[1][2];   
 //进度-组合,给出首地址，其他根据大小计算
extern const uint32_t icon_main3_progress_ring0_addr[1][2];
extern const uint8_t icon_main3_progress_ring0_coord[19][2];   
 //星期
extern const uint32_t icon_main3_week_addr[1][14];    
extern const uint8_t icon_main3_week_coord[1][2];

 //计步-数字
extern const uint32_t icon_main3_step_number_addr[2][10];
extern const uint8_t icon_main3_step_number_coord[6][2];

// 女性功能
extern const uint8_t icon_main3_menstrual_coord[1][2];
extern const uint32_t icon_main3_menstrual_addr[1][4];

//主界面-风格四
//am-pm    
extern const uint32_t icon_main4_ampm_addr[1][4];
extern const uint8_t icon_main4_ampm_coord[1][2];  
  //时间数字   
extern const uint32_t icon_main4_hour_addr[2][10]; 
extern const uint8_t icon_main4_hour_coord[4][2];
  //电池电量\充电    
extern const uint32_t icon_main4_battery_addr[1][2];
extern const uint8_t icon_main4_battery_coord[1][2];
extern const uint8_t icon_main4_battery_level_coord[5][2];
  //ble    
extern const uint32_t icon_main4_ble_addr[1][2];
extern const uint8_t icon_main4_ble_coord[1][2];
   //日期
extern const uint32_t icon_main4_date_addr[1][10];
extern const uint8_t icon_main4_date_coord[2][2];    
  //csr3.0    
   
 //百分比数字
extern const uint32_t icon_main4_progress_number_addr[1][10];
extern const uint8_t icon_main4_progress_number_coord[3][2];   
 //百分比-百分号
extern const uint32_t icon_main4_percent_sign_addr[1][1];
extern const uint8_t icon_main4_percent_sign_coord[3][2];   
 //进度-底环
extern const uint32_t icon_main4_progress_ring_addr[1][2];
extern const uint8_t icon_main4_progress_ring_coord[1][2];   
 //进度-组合,给出首地址，其他根据大小计算
extern const uint32_t icon_main4_progress_ring0_addr[1][2];
extern const uint8_t icon_main4_progress_ring0_coord[19][2];   
 //星期
extern const uint32_t icon_main4_week_addr[1][14];    
extern const uint8_t icon_main4_week_coord[1][2];
 //计步-数字
extern const uint32_t icon_main4_step_number_addr[2][10];
extern const uint8_t icon_main4_step_number_coord[6][2];

// 女性功能
extern const uint8_t icon_main4_menstrual_coord[1][2];

//主界面-风格五

  //电池电量\充电    
extern const uint32_t icon_main5_battery_addr[2][2];
extern const uint8_t icon_main5_battery_coord[1][2];
extern const uint8_t icon_main5_battery_level_coord[5][2];
  //ble    
extern const uint32_t icon_main5_ble_addr[2][2];
extern const uint8_t icon_main5_ble_coord[1][2];
   //日期
extern const uint32_t icon_main5_date_addr[2][10];
extern const uint8_t icon_main5_date_coord[4][2];

 //日期斜杠
extern const uint32_t icon_main5_date_line_addr[2][1];   
extern const uint8_t icon_main5_date_line_coord[1][2];  
  //csr3.0    

 //星期
extern const uint32_t icon_main5_week_addr[2][14];    
extern const uint8_t icon_main5_week_coord[1][2];
//表盘背景
extern const uint32_t icon_main5_dial_bg_addr[2][1];
extern const uint8_t icon_main5_dial_bg_coord[1][2];
// 女性功能
extern const uint8_t icon_main5_menstrual_coord[1][2];
extern const uint32_t icon_main5_menstrual_addr[1][4];

// 运动模式
  //1
extern const uint32_t icon_16_move1_run_man_addr[1];
extern const uint8_t icon_16_move1_run_man_coord[1][2];

extern const uint32_t icon_16_move1_run_line_addr[1];
extern const uint8_t icon_16_move1_run_line_coord[3][2];

extern const uint32_t icon_16_move1_run_text_addr[2];
extern const uint8_t icon_16_move1_run_text_coord[1][2];
  //2-数据面板
extern const uint32_t icon_16_move2_calories_icon_addr[1];
extern const uint8_t icon_16_move2_calories_icon_coord[1][2] ;

extern const uint32_t icon_16_move2_calories_unit_addr[2];
extern const uint8_t icon_16_move2_calories_unit_coord[1][2];

extern const uint32_t icon_16_move2_data_addr[10];
extern const uint8_t icon_16_move2_data_coord[4][2];

extern const uint32_t icon_16_move2_data_point_addr[1];
extern const uint8_t icon_16_move2_data_point_coord[1][2];

extern const uint32_t icon_16_move2_distance_icon_addr[1];
extern const uint8_t icon_16_move2_distance_icon_coord[1][2];

extern const uint32_t icon_16_move2_distance_unit_addr[4];
extern const uint8_t icon_16_move2_distance_unit_coord[1][2];

extern const uint32_t icon_16_move2_heart_icon_addr[1];
extern const uint8_t icon_16_move2_heart_icon_coord[1][2];

extern const uint32_t icon_16_move2_heart_nodata_addr[1];
extern const uint8_t icon_16_move2_heart_nodata_coord[1][2];

extern const uint32_t icon_16_move2_heart_unit_addr[2];
extern const uint8_t icon_16_move2_heart_unit_coord[1][2];

extern const uint32_t icon_16_move2_step_icon_addr[1];
extern const uint8_t icon_16_move2_step_icon_coord[1][2];

extern const uint32_t icon_16_move2_line_addr[1];
extern const uint8_t icon_16_move2_line_coord[4][2];

extern const uint32_t icon_16_move2_timing_addr[10];
extern const uint8_t icon_16_move2_timing_coord[6][2];

extern const uint32_t icon_16_move2_timing_colon_addr[1];
extern const uint8_t icon_16_move2_timing_colon_coord[2][2];
  //3-继续
extern const uint32_t icon_16_move3_goon_icon_addr[1];
extern const uint8_t icon_16_move3_goon_icon_coord[1][2];

extern const uint32_t icon_16_move3_goon_text_addr[2];
extern const uint8_t icon_16_move3_goon_text_coord[1][2];
  //4-暂停
extern const uint32_t icon_16_move4_pause_icon_addr[1];
extern const uint8_t icon_16_move4_pause_icon_coord[1][2] ;

extern const uint32_t icon_16_move4_pause_text_addr[2];
extern const uint8_t icon_16_move4_pause_text_coord[1][2] ;
  //5-退出
extern const uint32_t icon_16_move5_exit_icon_addr[1];
extern const uint8_t icon_16_move5_exit_icon_coord[1][2];

extern const uint32_t icon_16_move5_exit_text_addr[2];
extern const uint8_t icon_16_move5_exit_text_coord[1][2];


// 血氧环境监测
extern const uint32_t icon_16_spO2_icon4_addr[1];
extern const uint8_t  icon_16_spO2_icon4_coord[1][2];
// 亮度进度条
extern const uint32_t icon_16_spO2_icon3_addr[1];
extern const uint8_t  icon_16_spO2_icon3_coord[1][2];
// 进度值
extern const uint8_t  icon_16_spO2_data1_coord[1][2];
// %符号
extern const uint32_t icon_16_spO2_icon5_addr[1];
extern const uint8_t  icon_16_spO2_icon5_coord[3][2];


// 血氧
extern const uint32_t icon_16_spO2_icon1_addr[2];
extern const uint8_t  icon_16_spO2_icon1_coord[1][2];
// SPO2
extern const uint32_t icon_16_spO2_unit1_addr[1];
extern const uint8_t  icon_16_spO2_unit1_coord[1][2];
// 数字
extern const uint32_t icon_16_spO2_data_addr[10]; 
extern const uint8_t  icon_16_spO2_data2_coord[1][2];
// %符号
extern const uint32_t icon_16_spO2_icon2_addr[1];
extern const uint8_t  icon_16_spO2_icon2_coord[2][2];
// X符号
extern const uint32_t icon_16_spO2_error_addr[1];
extern const uint8_t  icon_16_spO2_error_coord[2][2];


    
// 低氧报警
extern const uint32_t icon_16_sp02low_icon_addr[2];
extern const uint8_t  icon_16_sp02low_icon_coord[1][2];


// 蓝牙断联提醒
//extern const uint32_t icon_16_ble_remind_icon_addr[1];
//extern const uint8_t  icon_16_ble_remind_icon_coord[1][2];


// 开机动画
extern const uint32_t icon_16_poweron_dynamic_addr[1];
extern const uint8_t icon_16_poweron_dynamic_coord[1][2];

/* 血压 */
extern const uint32_t icon_16_bp_bar_addr[1];
extern const uint8_t icon_16_bp_bar_coord[5][2] ;

extern const uint32_t icon_16_bp_contour_addr[2];
extern const uint8_t icon_16_bp_contour_coord[1][2];

extern const uint32_t icon_16_bp_round_addr[8];
extern const uint8_t icon_16_bp_round_coord[1][2];

extern const uint32_t icon_16_bp_unit_addr[1];
extern const uint8_t icon_16_bp_unit_coord[1][2];

extern const uint32_t icon_16_bp_xicon_addr[1];
extern const uint8_t icon_16_bp_xicon_coord[5][2];

extern const uint32_t icon_16_bp_data_addr[10]; 
extern const uint8_t icon_16_bp_data_coord[4][2];


#endif


