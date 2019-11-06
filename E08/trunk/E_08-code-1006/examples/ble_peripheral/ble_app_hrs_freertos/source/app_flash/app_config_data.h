/********************************Copyright(c)***********************************
**                          Veepoo Tech
**                        COPYRIGHT (C) 2016
** 文件名称: app_config_data.h
** 创建时间: 2016-11-25
** 创 建 人: GT
** 描    述: 文件主要提供APP设置的信息保存到FLASH的接口
**
**------------------------------------------------------------------------------
*******************************************************************************/

#ifndef _APP_CONFIG_DATA_H_
#define _APP_CONFIG_DATA_H_

#include <stdint.h>
#include <stdbool.h>
#include "flash_data_type.h"

#define APP_FLASH_SUCCESS       0x00
#define APP_FLASH_FAILED        0x01

//typedef void(*app_flash_callback_t)(uint8_t result);

uint32_t App_GetMessageEnable(app_message_enable_t* p_message);

// 保存推送消息开关配置
uint32_t App_UpdateMessageEnable(const app_message_enable_t* p_message);

uint32_t App_GetSedentaryConfig(sedentary_config_t* p_config);

// 更新久坐信息配置
uint32_t App_UpdateSedentaryConfig(const sedentary_config_t* p_config);

uint32_t App_GetDevConfig(dev_config_t* p_dev);

uint32_t App_UpdateDevConfig(const dev_config_t* p_dev);

uint32_t App_GetBloodMode(bp_config_t* p_config);

uint32_t App_SaveBloodMode(const bp_config_t* p_config);

uint32_t App_GetPersonInfomation(person_infomation_t* p_infomation);

// 更新APP设定个人信息配置
uint32_t App_UpdatePersonInfomation(const person_infomation_t* p_infomation);

uint32_t App_GetSportCount(uint16_t* p_config);

uint32_t App_UpdateSportCount(const uint16_t count);

uint32_t App_GetSleepTime(float* p_time);

uint32_t App_SaveSleepTime(const float time);

uint32_t App_SaveSleepTime(const float time);

uint32_t App_GetBluetoothName(bluetooth_name_t *p_bluetooth_name);

uint32_t App_UpdateBluetoothName(const bluetooth_name_t *p_device_info);

uint32_t App_UpdateHeartAlertParam(bool enable, uint8_t high, uint8_t low);

uint32_t App_GetHeartAlertParam(uint8_t* eanble, uint8_t* high, uint8_t* low);

uint32_t App_UpdateTurnWristParam(const turnwrist_config_t *p_turnwrist);

uint32_t App_GetTurnWristParam(turnwrist_config_t *p_turnwrist);

uint32_t App_UpdateGuiLanguage(uint8_t language);

uint32_t App_GetGuiLanguage(uint8_t* language);

uint32_t App_UpdateConnectPassword(uint16_t password);

uint32_t App_GetConnectPassword(uint16_t* password);

uint32_t App_UpdateBrightnessParam(brightness_config_t* p_param);

uint32_t App_GetBrightnessParam(brightness_config_t* p_param);

uint32_t App_UpdateBloodOxygenParam(time_range_config_t* p_param);

uint32_t App_GetBloodOxygenParam(time_range_config_t* p_param);

uint32_t App_UpdateRespirationRateParam(time_range_config_t* p_param);

// 获取呼吸率参数
uint32_t App_GetRespirationRateParam(time_range_config_t* p_param);

// 获取倒计时漏斗FLASH数据
uint32_t App_GetHourglassParam(hourglass_config_t *p_hourglass_config);

// 更新倒计时漏斗参数信息
uint32_t App_UpdateHourglassParam(const hourglass_config_t *p_hourglass_config);

uint32_t App_GetUiStyleParam(uint8_t *ui_style);

uint32_t App_UpdateUiStyleParam(const uint8_t param);

uint32_t App_GetMoveMaxPuseTimeParam(uint16_t *movement_max_puse_time);

uint32_t App_UpdateMoveMaxPuseTimeParam(const uint16_t param);

uint32_t App_FlashCheckAreaIsUsable(uint8_t start_page, uint8_t page_size);

//Erase the alarm clock data block
uint8_t App_FlashErasePhysPeriodData(void);

//Read the alarm clock data
uint8_t App_FlashReadPhysPeriodData(const uint16_t number, uint8_t *data, uint16_t data_len);

//Write the alarm clock data
uint8_t App_FlashWritePhysPeriodData(const uint16_t number, const uint8_t *data, const uint16_t data_len);

uint32_t App_GetUiDisplayTimeParam(uint8_t *ui_display_time);
uint32_t App_UpdateUiDisplayTimeParam(uint8_t ui_display_time);

#endif
