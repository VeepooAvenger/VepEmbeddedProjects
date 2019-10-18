/***************************Copyright(c)***********************************
**                       Veepoo Tech
**                  COPYRIGHT (C) 2016-2019
**-----------File Info-----------------------------------------------------
** File        name:     app_config_data.h
** Last modified Date:   2017-08-24
** Last SYS_VERSION:     0.0.0
** Descriptions:            
**-------------------------------------------------------------------------
** Created  by:          WJ
** Created  date:        2017-08-24
** SYS_VERSION:          0.0.0
** Descriptions:         
**-------------------------------------------------------------------------
** Modified by:				
** Modified date:  			          
** SYS_VERSION:                 
** Descriptions:                        
***************************************************************************/

#ifndef _APP_CONFIG_DATA_H_
#define _APP_CONFIG_DATA_H_

#include <stdint.h>
#include <stdbool.h>
#include "common.h"
#include "alarm_event.h"
#include "outsit.h"
#include "blood_pressure.h"


#define FUNCTION_SUPPORT_DISTANCE           1       //距离\公制英制
#define FUNCTION_SUPPORT_TIME_FORMATS       1       //时间
#define FUNCTION_SUPPORT_HEART              1       //心率
#define FUNCTION_SUPPORT_BP                 2       //血压
#define FUNCTION_SUPPORT_BO                 0       //血氧
#define FUNCTION_SUPPORT_SP_BROADCAST       0       //运动语音播报                       
#define FUNCTION_SUPPORT_HBO_BROADCAST      0       //心率血氧血压播报     

#define FUNCTION_SUPPORT_WEAR_DET           1       //佩戴检测
#define FUNCTION_SUPPORT_FIND_PHONE         2       //手机查找支持
#define FUNCTION_SUPPORT_ACCURATE_BP        0       //高端血压动态校准
#define FUNCTION_SUPPORT_STOPWATCH          2       //秒表支持

#define FUNCTION_SUPPORT_UI_STYLE           7       //支持UI风格种类 


#define FUNCTION_LED_GEAR                   1       //白人

#define FUNCTION_HRV                        0       //hrv
#define FUNCTION_CALL_ANSWERING             0       //来电自动接听

#define FUNCTION_BLE_DISCONNECT_REMIND      2       //蓝牙断连提醒

typedef enum
{
    CONFIG_LANGUAGE_CHINESE = 1,       //中文
    CONFIG_LANGUAGE_ENAGLISH,          //英文
    CONFIG_LANGUAGE_JAPANESE,          //日语
    CONFIG_LANGUAGE_KOREAN,            //韩语
    CONFIG_LANGUAGE_GERMAN,            //德语
    CONFIG_LANGUAGE_RUSSIAN,           //俄语
    CONFIG_LANGUAGE_SPANISH,           //西班牙语
    CONFIG_LANGUAGE_ITALIARN,          //意大利语
    CONFIG_LANGUAGE_FRENCH,            //法语
    CONFIG_LANGUAGE_VIETNAMESE,        //越南语
    CONFIG_LANGUAGE_PORTUGUESE,        //葡萄牙语
    CONFIG_LANGUAGE_TRADITIONAL,       //繁体
}CONFIG_LANGUAGE_E;


#pragma pack(1)
typedef struct
{
    uint8_t user_height;
    uint8_t user_weight;
    uint8_t user_age;
    uint8_t user_gender;
    uint16_t user_moving_target;    
}user_info_t;

/* 消息推送开关 */
typedef struct
{
    uint8_t call_enable:1;      //来电
    uint8_t sms_enable:1;       //短信息
    uint8_t wechat_enable:1;    //微信开关
    uint8_t qq_enable:1;        //qq开关
    uint8_t facebook_enable:1;  //facebook开关
    uint8_t twitter_enable:1;   //twitter开关
    uint8_t whatsapp_enable:1;  //WhatsApp开关    
    uint8_t linkedin_enable:1;  //linkedin开关
    uint8_t line_enable:1;      //line开关
    uint8_t instagram_enable:1; //instagram开关
    uint8_t snapchat_enable:1;  //Snapchat开关
    uint8_t skype_enable:1;     //skype开关
    uint8_t gmail_enable:1;     //Gmail开关
    uint8_t other_enable:1;     //其他
    uint8_t resv:2;             //保留
    uint8_t retain;             //保留
}message_enable_t;

/* 心率告警 */
typedef struct
{
    uint8_t heart_enable;    //
    uint8_t heart_low;    //
    uint8_t heart_hight;    //
}heart_alarm_enable_t;

//翻腕配置参数结构体
typedef struct
{
    uint8_t enable;    //翻腕使能开关
    uint8_t s_hour;    //翻腕起始时间--小时
    uint8_t s_minute;  //翻腕起始时间--分钟
    uint8_t e_hour;    //翻腕结束时间--小时
    uint8_t e_minute;  //翻腕结束时间--分钟
}turnwrist_config_t;

typedef struct
{
    uint8_t leve;      //亮度等级
    uint8_t setleve;   //跟随设置时间的亮度等级
    uint8_t s_hour;    //亮度起始时间--小时
    uint8_t s_minute;  //亮度起始时间--分钟
    uint8_t e_hour;    //亮度结束时间--小时
    uint8_t e_minute;  //亮度结束时间--分钟
}brightness_config_t;

typedef	struct
{
    uint16_t                password_num;
    user_info_t             user_info;          //6
    test_mode_t             bp_test_mode;       //6
    uint8_t                 gesture_level ;     //转腕等级，删除原来的心率定标值
    message_enable_t        message_enable;     //1 
    uint8_t                 wear_detect;             //佩戴 1
    outsit_t                outsit_cfg;              //久坐 7  	
    heart_alarm_enable_t    heart_alarm_enable;      // 3
    uint8_t                 horizontal_enable;       //横屏开关  1
    turnwrist_config_t      turnwrist_config;        //抬手翻腕亮屏  5
    brightness_config_t     brightness_leve;         //屏幕亮度      6
    uint8_t                 lost_phone;              //手机防丢      1
    countdown_config_t      countdown_config;        //多次倒计时状态 7
    uint16_t                movement_max_puse_time;  //运动模式的最大暂停时间
    uint8_t                 language;                //系统语言
    uint8_t                 ui_style;                //UI风格
}system_info_t;         //sizeof is 

typedef struct{
    uint8_t     distance_unit;           //公制英制
    uint8_t     time_formats;            //时间制式
    uint8_t     heart;                   //心率
    uint8_t     blood_pressure;          //血压    
    uint8_t     sport_broadcasts;        //运动过量播报
    uint8_t     heart_bp_bo_broadcasts;  //心率血氧血压播报  
    uint8_t     find_phone;              //查找手机，开启后有界面显示    
    uint8_t     stopwatch;               //秒表   
    uint8_t     blood_oxygen;            //血氧  
    uint8_t     led_gear;                //led档位  1,黑人6档，2；白人1档
    uint8_t     hrv;                     //hrv
    uint8_t     call;                    //来电自动接听  
    uint8_t     ble_disconnect_remind;    //蓝牙断连提醒
}system_switch_t;

#pragma pack()

//系统配置信息初始化
void Config_SystemInfoInit(void);

//部分开关初始化
void Config_SystemSwitchInit(void);

//系统参数自检
void Config_SystemParamCheck(void);

//部分开关自检
void Config_SystemSwitchCheck(void);

#endif
