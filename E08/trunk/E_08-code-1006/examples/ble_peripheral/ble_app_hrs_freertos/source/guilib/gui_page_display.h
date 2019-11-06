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
#include "common_data_type.h"
// 屏幕亮灭亮
#define LED_ON_OFF_ON_DEF			1//

#define LED_IS_SMALL			    1//

extern const uint8_t disp_delay_time;

#define TEST_CLEAR_SCREEN_PARAM     0//1//测试清除区域是否完全

//#define TEST_DBUG_UI_ENABLED    (0x00)

#define TEST_DBUG_UI_MAX_NUM    (22)

#define GUI_DISP_MESSAGE_MAXLEN     50
typedef struct
{
    uint8_t message_touch_num;                  //上方消息推送选中的选项  0~3
    uint8_t disp_data_num;                      //下面需要显示数据的数量
    uint8_t message_type[3];                    //左侧消息推送类型图标   0:代表没消息，1~17消息类型
    uint8_t p_message_content[3][GUI_DISP_MESSAGE_MAXLEN];           //消息推送内容文字
    uint16_t content_len[3];                    //消息推送内容长度
    system_clock_time_t message_time_s[3];         // 通知的时间
}page_message_param_t;

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
void Gui_LcdLedOn(void);

//LCD唤醒，能加快LCD本身数据处理
void Gui_LcdWakeUp(void);

//区域清屏
void Gui_PageClearBackground(uint8_t s_column, uint8_t s_page, uint8_t column_size, uint8_t page_size);

//更新某块区域的颜色（单一）
void Gui_PageRefreshBackground(uint8_t s_column, uint8_t s_page, uint8_t column_size, uint8_t page_size, uint16_t color);


//显示XXX
void Gui_PageDisXxxIcon(void);

////显示主界面的时钟和日期
//void Gui_PageMainDateHour(uint16_t index, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t week, 
//                          uint8_t language, uint8_t am_pm_format, uint32_t step_cnt);

////显示主界面其他图标
//void Gui_PageMainAssisant(uint16_t index, uint32_t step_cnt, uint8_t ble_status, uint8_t am_pm_format,
//                          uint8_t charge, uint8_t battery_level, uint8_t language, uint8_t menstrual);
void Gui_PageMain(uint16_t index, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, float distance_cnt, uint8_t week, 
                  uint16_t am_pm, uint32_t step_cnt, uint8_t heart,
                  uint8_t ble_status, uint8_t charge, uint8_t battery_level, uint8_t menstrual,
                  uint16_t calories_cnt, uint32_t step_target, uint16_t sleep_time, uint8_t language, uint8_t *cls_flag);

void Gui_PageChooseMain(uint16_t index, uint8_t Choose, uint8_t *cls_flag);

//显示计步界面
void Gui_PageStep(uint32_t step_cnt, uint8_t target_cnt, uint16_t index, uint8_t hour, uint8_t minute, uint8_t language, uint8_t *cls_flag,
                  float distance_cnt, uint8_t uint_type, uint16_t calories_cnt);

//显示里程界面
void Gui_PageDistance(float dis_count, uint16_t index, uint8_t uint_type, uint8_t hour, uint8_t minute, uint8_t language, uint8_t *cls_flag);

//显示卡路里
void Gui_PageCalories(uint16_t calories_cnt, uint16_t index, uint8_t hour, uint8_t minute, uint8_t language, uint8_t *cls_flag);

// 运动模式
void Gui_PageRunMode(uint16_t index, uint8_t hour, uint8_t minute, uint8_t language, uint8_t *cls_flag);

// 多运动选择模式
void Gui_PageRunModeSelect(uint16_t index, uint8_t page, uint8_t num, uint8_t* switch_tmp, uint8_t language);

// 运动模式数据展示界面
void Gui_PageRunModeData( uint16_t index, uint32_t seconds, uint8_t heart, float distance, float calories, 
                         uint32_t step, uint8_t uint_type, uint8_t animation_flag, uint8_t language, uint8_t *cls_flag, 
                         uint8_t runmode_type_show, uint8_t face_num,uint8_t runmode_type, uint8_t hour, uint8_t minute,
                         uint8_t Operation_state, uint8_t run_state, uint32_t Speed_distance);
// 运动模结束界面
void Gui_PageRunModeDataResult(uint16_t index, uint8_t data_valid, uint32_t seconds, uint8_t heart, float distance, float calories, 
                                uint32_t step_cnt, uint8_t uint_type, bool lock, uint8_t hour, uint8_t minute, uint8_t animation_flag, 
                                uint8_t language, uint8_t runmode_type, uint8_t runmode_show_type, uint8_t page, uint32_t Speed_distance);

// 运动模式暂停开始
void Gui_PageRunModePause(uint16_t index, uint8_t pause_state, uint8_t language, uint8_t *cls_flag);

// 运动模式退出
void Gui_PageRunModeExit(uint16_t index, uint8_t language, uint8_t *cls_flag);

//显示血压界面
void Gui_PageBP(uint16_t index, uint8_t bp_sp, uint8_t bp_dp, uint8_t test_mode, uint8_t hour, uint8_t minute, uint8_t test_state, uint8_t *cls_flag, uint8_t language);

//显示心率界面
void Gui_PageHeart(uint16_t index, uint8_t heart, uint8_t last_heart, uint8_t hour, uint8_t minute, uint8_t language, uint8_t *cls_flag);

//睡眠界面
void Gui_PageSleep(uint16_t index,uint16_t time, uint8_t hour, uint8_t minute, uint8_t language, uint8_t *cls_flag,
                    uint16_t deep_time,uint16_t light_time, uint8_t quality);

//找手机界面
void Gui_PageFindPhone(bool dynamic_find, uint16_t index, uint8_t hour, uint8_t minute, uint8_t language, uint8_t *cls_flag);

//蓝牙未连接界面
void Gui_PageBleUnconnected(uint16_t index ,uint8_t language);

//秒表界面
void GUI_StopWatchPage(uint16_t index, uint8_t entry_flag, uint8_t run_flag, uint16_t total_second, uint8_t one_hundred_ms,
						uint8_t hour, uint8_t minute, uint8_t language, uint8_t *cls_flag);

//倒计时界面
void Gui_PageCountDown(uint16_t index, uint8_t dynamic, uint32_t total_second, uint8_t hour, uint8_t minute, uint8_t language, uint8_t *cls_flag, uint8_t run_state);

//倒计时结束界面
void Gui_PageCountDownTimeOut(uint16_t index,  uint8_t language, uint8_t *cls_flag);

//相机准备界面
void Gui_PageCameraReady(uint16_t index, uint8_t camera_state, uint8_t *cls_flag);

//显示Mac和版本号
void Gui_PageDisMacVer(uint16_t index, uint8_t *mac_data, uint8_t *ver_data, uint8_t language, uint8_t *cls_flag);

//关机界面
void Gui_PagePower(uint16_t index, uint8_t *mac_data, uint8_t *ver_data, uint8_t hour, uint8_t minute, uint8_t language, uint8_t *cls_flag,uint8_t *ble_name);

//闹钟界面
void Gui_PageAlarm(uint16_t index, uint8_t am_pm_format, uint8_t scene, uint8_t hour, uint8_t minute, uint8_t language, uint8_t *cls_flag);

//久坐界面
void Gui_PageSit(uint16_t index, uint8_t language, uint8_t *cls_flag);

//低电提醒界面
void Gui_PageLowPower(uint16_t index, uint8_t *cls_flag);

//电池充电界面
void Gui_PageBatteryCharg(uint16_t index, uint8_t state, uint8_t *cls_flag);


//运动目标提醒界面
void Gui_PageStarget(uint16_t index, uint32_t step_target_cnt, uint8_t language, uint8_t *cls_flag);

//心率报警界面
void Gui_PageHeartAlert(uint16_t index, uint8_t *cls_flag);
//低氧报警界面
void Gui_LowOxygenAlertPage(uint16_t index, uint8_t *cls_flag);
//显示“维亿魄科技”，测试用
void Gui_PageFontTest(uint16_t index, uint8_t *cls_flag);

//显示来电界面
void Gui_PageCallNotificationIcon(uint16_t index, uint8_t call_state, uint8_t language, uint8_t *cls_flag);

//显示来电消息推送界面
void Gui_PageCallNotificationContent(uint16_t index, uint8_t *display_buf, uint16_t buf_len, uint8_t *cls_flag);

//显示消息推送界面
void Gui_PageMessageNotificationIcon(uint16_t index, uint8_t msg_type, uint8_t page_index, uint8_t hour, uint8_t minute, uint8_t *cls_flag);

//显示消息推送界面
uint8_t Gui_PageMessageNotificationContent(uint16_t index, uint8_t *display_buf, uint16_t buf_len, uint8_t *cls_flag);

//蓝牙断联界面
void Gui_PageBleDiscRemind(uint16_t index, uint8_t *cls_flag);

//血氧界面
void Gui_PageOxygen(uint16_t index, uint8_t progress, uint8_t oxygen_cnt, uint8_t hour, uint8_t minute, uint8_t language, uint8_t *cls_flag);
    
void Gui_PagePtt(uint16_t index, uint8_t *cls_flag);
void Gui_PagePttItem1(uint16_t index, uint8_t state, uint8_t *cls_flag);

void Gui_PageEcg(uint16_t index, uint8_t hour, uint8_t minute, uint8_t language, uint8_t *cls_flag);
void Gui_PageEcgItem1(uint16_t index, uint8_t state, uint8_t heart, uint8_t language, uint8_t *cls_flag);
void Gui_PageEcgItem2(uint16_t index, uint8_t state, uint8_t language, uint8_t *cls_flag);

//显示消息
void Gui_PageMessage3(uint16_t index, uint8_t language);
//显示消息推送界面02
void Gui_PageMessageNotificationIcon02(uint16_t index, uint8_t msg_type, uint8_t page_index, uint8_t hour, uint8_t minute, uint8_t language);

//显示消息02
uint16_t  Gui_PageMessageNotificationContent02(uint16_t index, uint8_t *display_buf, uint16_t buf_len, uint8_t page_index);


//设置界面
void Gui_Setting(uint16_t index, uint8_t set_type, uint8_t language);

void Gui_PageLightLevelSet(uint16_t index, uint8_t light_level, uint8_t language);

void Gui_PageLightTimeSet(uint16_t index, uint8_t linght_time ,uint8_t language);

void Gui_PageMainStyleSet(uint16_t index, uint8_t language);

//显示更多功能界面
void Gui_PageFunct(uint16_t index, uint8_t language);
//显示更多功能界面选择    
void Gui_PageFunct2(uint16_t index, uint8_t page, uint8_t num, uint8_t language);

void Gui_PageMessageSwitch(uint16_t index, uint8_t page, uint8_t num, uint8_t* switch_tmp, uint8_t language);
    
void Gui_PageSwitch(uint16_t index, uint8_t page, uint8_t num, uint8_t* switch_tmp, uint8_t language);
    
void Gui_PageClearData(uint16_t index, uint8_t flg, uint8_t language);

void Gui_PageDispMessage(uint16_t index, page_message_param_t message_para, uint8_t language);


#endif
