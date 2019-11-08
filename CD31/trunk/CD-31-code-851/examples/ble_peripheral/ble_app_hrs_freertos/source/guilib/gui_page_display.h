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

#ifndef _GUI_LIB_H_
#define _GUI_LIB_H_

#include <stdint.h>
#include <stdbool.h>


// 屏幕亮灭亮
#define LED_ON_OFF_ON_DEF			1

//#define TEST_DBUG_UI_ENABLED    (0x00)

#define TEST_DBUG_UI_MAX_NUM    (22)

//调试UI界面用
void Gui_DisDbugUi(void);
    
//LCD初始化
void Gui_LcdDriverInit(void);

//LCD打开屏幕
void Gui_LcdDriverOn(void);

//LCD关闭屏幕
void Gui_LcdDriverOff(void);

//关闭背光
void Gui_LcdLedOff(void);

//打开背光
void Gui_LcdLedOn(void);

//LCD唤醒，能加快LCD本身数据处理
void Gui_LcdWakeUp(void);

//区域清屏
void Gui_PageClearBackground(uint8_t s_column, uint8_t s_page, uint8_t column_size, uint8_t page_size);

//更新某块区域的颜色（单一）
void Gui_PageRefreshBackground(uint8_t s_column, uint8_t s_page, uint8_t column_size, uint8_t page_size, uint16_t color);

//显示XXX
void Gui_PageDisXxxIcon(void);

//显示主界面
void Gui_PageMain(uint16_t index, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second, uint8_t week, 
                  uint16_t am_pm, uint32_t step_cnt, uint8_t heart,
                  uint8_t ble_status, uint8_t charge, uint8_t battery_level, uint8_t menstrual,
                  uint16_t calories_cnt, uint32_t step_target, uint16_t sleep_time, uint8_t language);

//计步距离卡路里集合界面
void Gui_PageStepDistanceCalories(uint16_t index, uint32_t step_cnt, uint8_t step_prg,
                                  float distance_cnt,uint8_t uint_type,
                                  uint16_t calories_cnt, uint8_t language);

// 运动模式
void Gui_PageRunMode(uint16_t index, uint8_t language);

// 运动模式数据展示界面
void Gui_PageRunModeData(uint16_t index, uint32_t seconds, uint8_t heart, float distance, float calories, 
                         uint32_t step_cnt, uint8_t uint_type, bool lock, uint8_t hour, uint8_t minute, uint8_t animation_flag, uint8_t language);

// 运动模式-解锁界面
void Gui_PageRunModeUnlock(uint16_t index, uint8_t language);
//
void Gui_PageRunModePause(uint16_t index, uint8_t pause_state, uint8_t language);

void Gui_PageRunModeOperate(uint16_t index, uint8_t pause_state, uint8_t operate_state, uint8_t hour, uint8_t minute, uint8_t language);

//
void Gui_PageRunModeExit(uint16_t index, uint8_t language);

//显示血压界面
void Gui_PageBP(uint16_t index, uint8_t bp_sp, uint8_t bp_dp, uint8_t test_mode, uint8_t test_state, uint8_t progress);

//显示心率界面
void Gui_PageHeart(uint16_t index, uint8_t heart, uint8_t last_heart, uint8_t * heart_array, uint8_t max_heart, uint8_t min_heart, uint8_t language);

//睡眠界面
void Gui_PageSleep(uint16_t index, uint16_t time, uint16_t deep_time, uint16_t light_time, uint8_t quality, uint8_t language);

//找手机界面
void Gui_PageFindPhone(bool dynamic_find, uint16_t index, uint8_t language);

//秒表界面
void GUI_StopWatchPage(uint16_t index, uint8_t entry_flag,  uint8_t reset_flag, uint8_t run_flag, uint16_t total_second, uint8_t one_hundred_ms, 
						uint8_t system_hour, uint8_t system_minute, uint8_t language);

//倒计时界面
void Gui_PageCountDown(uint16_t index, bool dynamic, uint8_t run_state, uint32_t total_second, uint8_t language);

//相机准备界面
void Gui_PageCameraReady(uint16_t index);

//相机OK界面
void Gui_PageCameraOk(uint16_t index);

//显示Mac和版本号
void Gui_PageDisMacVer(uint16_t index, uint8_t *mac_data, uint8_t *ver_data);

//开机界面
void Gui_PagePowerOn(uint16_t index);

void Gui_PageTurnOff(uint16_t index, uint8_t *ble_name, uint8_t *mac_data, uint8_t *ver_data,uint8_t language);
//关机界面
void Gui_PagePowerOff(uint16_t index);

//闹钟界面
void Gui_PageAlarm(uint16_t index, uint8_t am_pm_format, uint8_t scene, uint8_t hour, uint8_t minute, uint8_t language);

//久坐界面
void Gui_PageSit(uint16_t index, uint8_t language);

//低电提醒界面
void Gui_PageLowPower(uint16_t index);

//电池充电界面
void Gui_PageBatteryCharg(uint16_t index, uint8_t state, uint8_t bat_grade);

//运动目标提醒界面
void Gui_PageStarget(uint16_t index, uint32_t step_target_cnt, uint8_t language);

//心率报警界面
void Gui_PageHeartAlert(uint16_t index);

//显示“维亿魄科技”，测试用
void Gui_PageFontTest(uint16_t index);

//显示来电界面
void Gui_PageCallNotificationIcon(uint16_t index, uint8_t call_state, uint8_t language);

//显示来电消息推送界面
void Gui_PageCallNotificationContent(uint32_t index, uint8_t *display_buf, uint16_t buf_len);

//显示消息推送界面
void Gui_PageMessageNotificationIcon(uint16_t index, uint8_t msg_type, uint8_t page_index, uint8_t hour, uint8_t minute, uint8_t language);

//显示消息推送界面
void Gui_PageMessageNotificationContent(uint16_t index, uint8_t *display_buf, uint16_t buf_len, uint8_t page_index);

//蓝牙断联界面
void Gui_PageBleDiscRemind(uint16_t index);

//显示消息推送界面02
void Gui_PageMessageNotificationIcon02(uint16_t index, uint8_t msg_type, uint8_t page_index, uint8_t hour, uint8_t minute, uint8_t language);

//显示消息02
void Gui_PageMessageNotificationContent02(uint16_t index, uint8_t *display_buf, uint16_t buf_len, uint8_t page_index);

#endif
