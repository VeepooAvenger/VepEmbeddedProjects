#ifndef _API_FLASH_H_
#define _API_FLASH_H_

#include <stdint.h>
#include <stdarg.h>
#include "SystemTime.h"
#include "flash_data_type.h"
#include "flash_record_data.h"
#include "data_manage.h"

typedef struct
{
    uint8_t                   alarm_enable;             // 闹钟开关
	uint8_t                   ui_style;                 // ui风格参数
    uint8_t                   update_uimode;            // 更新UI数据模式
    uint8_t                   Remind_flag;              // 某天计步是否提醒
    uint16_t                  movement_max_puse_time;   // 运动模式的最大暂停时间
    uint16_t                  password;                 // 密码
    uint16_t                  systemtime_index;         // 系统时间索引
    uint16_t                  sport_target;             // 运动目标
	bluetooth_name_t          bluetooth_name_s;         // 蓝牙名称
    person_infomation_t       person_s;                 // 用户信息
    turnwrist_config_t        turnwrist_s;              // 翻腕配置信息
    heart_alert_config_t      heart_alert_s;            // 心率配置参数
    bp_config_t               bp_s;                     // 血压配置参数
    CONFIG_LANGUAGE_E         language_e;               // 语言配置参数
    dev_config_t              dev_config_s;             // 开关配置参数
    app_message_enable_t      message_enable_s;         // ANCS开关信息
    sedentary_config_t        sedentary_config_s;       // 久坐配置信息
    brightness_config_t       brightness_s;             // 亮度参数
    hourglass_config_t        hourglass_s;              // 倒计时参数
    union
	{    
        alarm_time_config_t   alarm_time_s[20];         // 闹铃参数
        uint8_t               alarm_old_time[9];        // 三组闹钟    
    };
    time_range_config_t       blood_oxygen_config_s;    // 血氧开关信息
    product_infomation_t      product_infomation_s;     // 产品信息参数
    flash_find_index_t        flash_index_s;            // FLASH索引
    uint8_t                   sit_func_state;           // 久坐功能模块状态
    uint8_t                   bp_func_state;            // 血压功能模块状态
    uint8_t                   ui_display_time;          // UI显示时间
}flash_param_t;

extern flash_param_t* p_flash_param;

uint16_t Crc16(uint16_t crc, uint8_t *data, uint16_t len);

//Flash初始化函数
//void Api_FlashParamInit(void);

// 写入日志ASCII
void Api_WriteLogText(uint8_t* p_content, uint8_t len);

// 清除日志文字
void Api_ClearLogText(void);

// 保存日常数据
void Api_SaveDailyAttrData(const daily_data_attr_t* p_attr);

//// 读取日常属性数据
//uint32_t Api_ReadDailyAttrData(uint8_t day_id,uint16_t package_offset,daily_data_attr_t* p_attr);

// 获取记步数
uint32_t Api_GetStepAllCount(uint8_t app_day_id);

//设置睡眠计算的快号
void Api_SetSleepBlack(uint8_t black);

// 获取睡眠回调
uint32_t Api_SleepCallback(uint8_t *data, uint16_t current_pkg, uint16_t pre_offset);

// 跳块
uint32_t Api_FlashDailyJumpBlock(void);

// 读FLASH数据
uint32_t Api_ReadAllExtFlashData(uint8_t block_id,uint8_t offset,uint8_t* p_data);
//uint32_t Api_ReadAllFlashData(uint8_t block_id,uint8_t offset,uint8_t* p_data);

// FLASH恢复出厂设置
uint32_t Api_FlashFatoryRestore(void);

// 清除用户数据
uint32_t Api_FlashClearUserData(void);
// 清除用户配置
uint32_t Api_FlashClearUserConfig(void);



// 保存睡眠结果
uint32_t Api_SaveSleepResultData(uint8_t day_id, uint8_t *p_data, uint16_t data_len);
// 修改睡眠连接标志 
uint32_t Api_AlterSleepContinueFlag(uint8_t day_id, uint8_t wake_cnt);
// 获取某一段的睡眠向下连接标志
uint8_t Api_GetSleepContinueNextFlag(uint8_t day_id, uint8_t wake_cnt);

// 获取睡眠存储信息
void Api_GetSleepStoreInfo(uint8_t day_id, uint8_t *sleep_num, uint16_t *sleep_size, uint32_t *sleep_offset);
// 获取睡眠某天的曲线长度和
void Api_GetSleepCurveLength(uint8_t day_id, uint8_t *sleep_num, uint16_t *sleep_curve_len);

// 获取睡眠块差值
uint8_t  Api_GetSleepMapDayID(uint8_t block_id);

// 获取睡眠总时间
uint16_t Api_GetSleepTotalTime(uint8_t *sleep_quality, 
                               uint16_t *sleep_shallow_time, 
                               uint16_t *sleep_deep_time);

// 获取睡眠数据，16字节一包数据，用于数据发送到app
uint32_t Api_GetSleepPartData(uint8_t day_id, uint32_t offset, uint8_t* p_data, uint16_t data_len);

// Flash中保存系统时间
uint32_t Api_SaveSystemTime(uint16_t year,uint8_t month,uint8_t day,uint8_t hour,uint8_t minute);

// Flash中读取系统时间
uint32_t Api_ReadSystemTimeData(uint8_t* pdata);

// 对时擦除FLASH数据
bool Api_AdjustTimeClearFlash(const system_clock_time_t* p_history_time,const system_clock_time_t* p_current_time);

// 异步更新APP配置开关信息
uint32_t Api_AsynUpdateAppConfig(void);

// FLASH更新心率告警参数
uint32_t Api_UpdateHeartAlertParam(const heart_alert_config_t* p_alert);

// FLASH更新翻腕参数
uint32_t Api_UpdateTurnWristParam(const turnwrist_config_t* p_turnwrist);

// FALSH更新血压配置参数
uint32_t Api_UpdateBloodConfig(const bp_config_t* p_config);

// 更新FLASH ANCS开关使能
uint32_t Api_UpdateMessageEnableConfig(const app_message_enable_t *p_enable);

// 获取血压模式
BLOOD_MODE_E Api_GetBloodMode(void);

// 更新开关
uint32_t Api_UpdateDevConfig(const dev_config_t* p_dev_config_t);

// 更新久坐配置
uint32_t Api_UpdateSedentaryConfig(const sedentary_config_t* p_sedentary_config_t);

// 更新语言
uint32_t Api_UpdateGuiLanguage(CONFIG_LANGUAGE_E language);

// 更新密码
uint32_t Api_UpdateConnectPassword(uint16_t password);

// 更新亮度信息
uint32_t Api_UpdateBrightness(brightness_config_t* p_bright);

// 更新闹钟时间
uint32_t Api_UpdateAlarmTime(uint8_t index,const alarm_time_config_t* p_alarm);

// 更新用户信息
uint32_t Api_UpdateUserInfomation(const person_infomation_t* p_person,uint16_t target);

// 获取闹钟CRC
uint16_t Api_GetAlarmCRC(void);

// 更新倒计时参数
uint32_t Api_UpdateHourglassParam(const hourglass_config_t *p_hourglass_config);

// 更新血氧参数
uint32_t Api_UpdateBloodOxygenParam(const time_range_config_t* p_param);

// FLASH初始化
void Api_FlashInit(void);

// 获取设备ID
uint32_t Api_GetDevSN(void);

// 设备ID号识别
void Api_DevSNCheck(void);

// 更新蓝牙设备名称
//void Api_UpdateBluetoothName(const bluetooth_name_t* p_bluetooth_name);


uint32_t Api_UpdateUiStyle(uint8_t ui_style);

uint8_t Api_GetUiStyle(void);

//ui_display_time:UI显示保存时间
uint32_t Api_UpdateUiDisplayTime(const uint8_t ui_display_time);
//获取UI显示保存时间
uint8_t Api_GetUiDisplayTime(void);

// 存储 ECG手动测量结果数据
void Api_SaveEcgAttrData(const ecg_result_data_t *p_ecg_data);

// 获取ECG手动测量结果数据
void Api_GetEcgAttrData(uint16_t offset, ecg_result_data_t *p_ecg_data);

//获取睡眠时间-新的方式20点为界限
uint16_t Api_GetSleepTotalTimeNew(uint8_t *last_sleep_time, 
                               uint8_t *sleep_quality, 
                               uint16_t *sleep_shallow_time, 
                               uint16_t *sleep_deep_time);

#endif

