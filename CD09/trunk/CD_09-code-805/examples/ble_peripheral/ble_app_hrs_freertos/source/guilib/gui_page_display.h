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

//LCD唤醒，能加快LCD本身数据处理
void Gui_LcdWakeUp(void);

//区域清屏
void Gui_PageClearBackground(uint8_t s_column, uint8_t s_page, uint8_t column_size, uint8_t page_size);

//更新某块区域的颜色（单一）
void Gui_PageRefreshBackground(uint8_t s_column, uint8_t s_page, uint8_t column_size, uint8_t page_size, uint16_t color);

//显示XXX
void Gui_PageDisXxxIcon(void);

//显示主界面的时钟和日期
void Gui_PageMainDateHour(uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t week,
                          uint8_t language, uint8_t am_pm_format, uint8_t weather, uint8_t temperature_low,
                          uint8_t temperature_high);

//显示主界面其他图标
void Gui_PageMainAssisant(uint8_t index, uint32_t step_cnt, uint8_t ble_status, uint8_t am_pm_format,
                          uint8_t charge, uint8_t battery_level, uint8_t language, uint8_t menstrual);

//显示计步界面
void Gui_PageStep(uint32_t step_cnt, uint8_t target_cnt, uint8_t index, uint8_t language);

//显示里程界面
void Gui_PageDistance(float dis_count, uint8_t index, uint8_t uint_type, uint8_t language);

//显示卡路里
void Gui_PageCalories(float calorie, uint8_t index, uint8_t language);

// 运动模式
void Gui_PageRunMode(uint8_t index, uint8_t language);

// 运动模式数据展示界面
void Gui_PageRunModeData(uint32_t seconds, uint8_t heart, float distance, float calories, 
                         uint32_t step, uint8_t uint_type, uint8_t language);
// 运动模式暂停开始
void Gui_PageRunModePause(uint8_t pause_state, uint8_t language);

// 运动模式退出
void Gui_PageRunModeExit(uint8_t language);

//
//void Gui_PageOxygen(uint8_t index, uint8_t count);

//
//void GUI_PageRRate(uint8_t index, uint8_t count);

//显示血压界面
void Gui_PageBP(uint8_t index, uint8_t bp_sp, uint8_t bp_dp, uint8_t test_mode, uint8_t test_state);

//显示心率界面
void Gui_PageHeart(uint8_t index,uint8_t heart);

//睡眠界面
void Gui_PageSleep(uint8_t index,uint16_t time, uint8_t language);

//找手机界面
void Gui_PageFindPhone(bool dynamic_find, uint8_t index);

//秒表界面
void GUI_StopWatchPage(bool entry, uint16_t total_second, uint8_t one_hundred_ms, uint8_t index);

//倒计时界面
void Gui_PageCountDown(bool dynamic, uint8_t index);

//相机准备界面
void Gui_PageCameraReady(uint8_t index);

//相机OK界面
void Gui_PageCameraOk(uint8_t index);

//显示Mac和版本号
void Gui_PageDisMacVer(uint8_t index, uint8_t *mac_data, uint8_t *ver_data);

//关机界面
void Gui_PagePower(void);

//闹钟界面
void Gui_PageAlarm(uint8_t am_pm_format, uint8_t scene, uint8_t hour, uint8_t minute, uint8_t language, uint8_t index);

//久坐界面
void Gui_PageSit(uint8_t index);

//低电提醒界面
void Gui_PageLowPower(uint8_t index);

//电池充电界面
void Gui_PageBatteryCharg(uint8_t index, uint8_t state);

//
//void Gui_UpdateBatteryProcess(uint8_t battery_full, uint8_t battery_level);

//运动目标提醒界面
void Gui_PageStarget(uint8_t index);

//心率报警界面
void Gui_PageHeartAlert(uint8_t index);

//显示“维亿魄科技”，测试用
void Gui_PageFontTest(void);

//显示来电界面
void Gui_PageCallNotificationIcon(uint8_t call_state, uint8_t index);

//显示来电消息推送界面
void Gui_PageCallNotificationContent(uint8_t *display_buf, uint16_t buf_len);

//显示消息推送界面
void Gui_PageMessageNotificationIcon(uint8_t msg_type, uint8_t index);

//显示消息推送界面
void Gui_PageMessageNotificationContent(uint8_t *display_buf, uint16_t buf_len, uint8_t index);

//蓝牙断联界面
void Gui_PageBleDiscRemind(uint8_t index);

#endif
