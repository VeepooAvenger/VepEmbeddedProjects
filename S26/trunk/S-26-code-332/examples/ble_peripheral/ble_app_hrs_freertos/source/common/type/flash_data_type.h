#ifndef _FLASH_DATA_TYPE_H_
#define _FLASH_DATA_TYPE_H_

#include <stdint.h>
#include "common_data_type.h"

/*===================================================================
 *                            定义FLASH基地址           
 *===================================================================
 */

#define PRODUCT_INFOMATION_BASE_ADDR    0x73000     // 产品信息基地址
#define APP_CONFIG_BASE_ADDR            0x72400     // APP配置基地址
#define SYSTEM_PARAM_BASE_ADDR          0x72000     // 系统参数基地址
#define ALARM_CONFIG_BASE_ADDR          0x72000

// 系统时间
#define SYSTEM_TIME_BASE_ADDR           0x71000//0x6D000//


// 日常数据
#define RECORD_DAILY_BASE_ADDR          0x5F000     // 全天数据基地址
#define RECORD_DAILY_DATA_SIZE          0x1C00      // 日常数据大小(7KB)
#define RECORD_DAILY_TOTAL_SIZE         0x1B000     // 
#define RECORD_DAILY_ONE_DAY_SIZE       0x6000      // 全天数据的大小(24KB)

#define RECORD_SLEEP_OFFSET             0x1C00      //睡眠数据的偏移量


// Flash读取数据地址宏
#define RECORD_FLASH_BLOCK_DATA_ADDR(block_id,offset)  (uint32_t)(RECORD_MOVEMENT_BASE_ADDR+(block_id)*1024+offset)
// 日常数据地址宏
#define RECORD_DAILY_DATA_ADDR(day_id,offset)          (uint32_t)(RECORD_DAILY_BASE_ADDR+RECORD_DAILY_ONE_DAY_SIZE*(day_id)+offset)
// 睡眠数据地址宏
#define RECORD_SLEEP_DATA_ADDR(day_id,offset)          (uint32_t)(RECORD_DAILY_BASE_ADDR+RECORD_DAILY_ONE_DAY_SIZE*(day_id)+RECORD_SLEEP_OFFSET+offset)

/**********************************************
 *               血氧数据                     *
 **********************************************/
#define RECORD_BLOOD_OXYGEN_SIZE           0x4000      // 血氧数据总大小(16KB)
#define RECORD_BLOOD_OXYGEN_OFFSET         0x2000      // 血氧偏移量
// 血氧数据地址宏
#define RECORD_BLOOD_OXYGEN_DATA_ADDR(block_id,offset)   (uint32_t)(RECORD_DAILY_BASE_ADDR+RECORD_DAILY_ONE_DAY_SIZE*block_id+RECORD_BLOOD_OXYGEN_OFFSET+offset)

/**********************************************
 *               运动模式                     *
 **********************************************/
#define RECORD_MOVEMENT_BASE_ADDR       0x59000     // 运动模式的基地址
#define RECORD_MOVEMENT_TOTAL_SIZE      0x2000      // 运动模式数据总大小(8KB)

#define RECORD_MOVEMENT_HEAD_OFFSET     0x50        // 运动模式数据的偏移量
// 运动模式数据地址宏
#define RECORD_MOVEMENT_HEAD_ADDR(block_id,offset)   (uint32_t)(RECORD_MOVEMENT_BASE_ADDR + (RECORD_MOVEMENT_TOTAL_SIZE*block_id) + (offset*0))
#define RECORD_MOVEMENT_DATA_ADDR(block_id,offset)   (uint32_t)(RECORD_MOVEMENT_BASE_ADDR + RECORD_MOVEMENT_HEAD_OFFSET + (RECORD_MOVEMENT_TOTAL_SIZE*block_id) + (offset*20))

// 日志
#define LOG_BASE_ADDR                   0x58000//0x6E000     // 日志基地址
#define LOG_DATA_SIZE                   0x1000//0x4000      // 日志数据大小(16KB)

// 日志数据地址宏
#define LOG_DATA_ADDR(offset)  (uint32_t)(LOG_BASE_ADDR+offset)

// 全部数据
#define RECORD_FLASH_BLOCK_DATA_SIZE                    (PRODUCT_INFOMATION_BASE_ADDR - RECORD_MOVEMENT_BASE_ADDR)  
// 用户数据
#define RECORD_FLASH_BLOCK_USER_DATA_SIZE               (SYSTEM_TIME_BASE_ADDR - RECORD_MOVEMENT_BASE_ADDR)   



// 定义日志合法性检测
#define LOG_CONDITION_VALID(offset)  (offset < 256)
// 定义一些合法性检测(由于分五分钟数据冗余2小时,所以会最多26小时数据: 26x60/5 = 312)
#define RECORD_DAILY_CONDITION_VALID(day_id,offset)  ((day_id < 3)&&(offset < 312))
// 定义一些合法性检测(由于血氧最长保存10小时,冗余2小时,所以数据偏移: 12*60 = 712)
#define RECORD_BLOOD_OXYGEN_CONDITION_VALID(day_id,offset)  ((day_id < 3)&&(offset < 480))
// 定义一些合法性检测(由于运动心率单次最长保存5小时,所以数据偏移: 5*60 = 300)
#define RECORD_SPORT_HEART_CONDITION_VALID(day_id,offset)  ((day_id < 3)&&(offset < 300))

/*===================================================================
 *                            定义FLASH结构体           
 *===================================================================
 */
 
#pragma pack (1)
#pragma anon_unions
 
//运动心率结构体
typedef struct
{
    uint16_t  year;       // 时间-年
    uint8_t   month;      // 时间-月
    uint8_t   day;        // 时间-日
    uint8_t   hour;       // 时间-时
    uint8_t   minute;     // 时间-分
    uint8_t   heart;      // 心率值
    uint16_t  sport;      // 运动量
    uint16_t  step;       // 记步值
    uint8_t   calorie;    // 卡路里
    uint16_t  distance;   // 距离
    uint8_t   resv[6];    // 预留对齐20字节
}sport_heart_attr_t;

//血氧结构体
typedef struct
{
    uint8_t hour;               //时
    uint8_t minute;             //分
    uint8_t oxygen;             //血氧结果
    uint8_t breath_stop_cnt;    //呼吸暂停次数
    uint8_t breath_stop_result; //睡眠呼吸暂停综合征的结果
    uint8_t low_spo2_time;      //缺氧时间
    uint8_t breath_stop_time;   //呼吸
    uint8_t heart_load;         //心脏负荷
    uint8_t hrv;                //心率变异性
    uint8_t sport_amount;       //运动量
    uint8_t heart;              //心率
    uint8_t step;               //记步
    uint8_t br;                 //呼吸率
//    uint8_t wear;               // 佩戴状态
    uint8_t wear:2;             //佩戴状态 0~3
    uint8_t spo2_modif_flag:2;  //5 分钟血氧修正标志位  0~3
    uint8_t bit_resv:4;         //预留4位    
    uint8_t resv[2];            //对齐保留
}blood_oxygen_attr_t;

// 散点图心率结构体
typedef struct
{
    uint8_t heart[10];          // 心率值
    uint8_t resv[6];            // 预留对齐
}scatter_heart_t;

//日常数据
typedef struct
{
    uint8_t    month;
    uint8_t    day;
    uint8_t    hour;
    uint8_t    minute;
    uint16_t   step;
    uint16_t   sport;
    uint8_t    heart;
    uint8_t    blood_high;
    uint8_t    blood_low;
    uint8_t    gesture;
    uint8_t    sleep_err;
    uint8_t    gesture_cnt; //睡眠姿态次数;
    uint16_t   distance;
    uint16_t   calories;
    uint8_t    ox_value;
    uint8_t    resv;
}daily_data_attr_t;

// 日志数据格式
typedef struct
{
    uint8_t   text[64]; // 文本内容
}log_data_attr_t;

// 产品生产信息参数结构体
typedef struct
{
    uint8_t master_sn[4];          // 设备识别码1
    uint8_t company_name[8];       // 公司名称
    uint8_t product_version[8];    // 产品型号
    uint8_t hardware_version[4];
    uint8_t firmware_version[4];
    uint8_t product_date[4];
    uint8_t agent_id[8];
    uint8_t product_batch[8];
    uint8_t product_count[8];
    uint8_t slave_sn[4];
    uint8_t product_modify[4];
    uint8_t bootloader_version[8];
    uint8_t application_version[8];
    uint8_t pin_config[4];
    //WJ.20181011.增加
    uint8_t ble_name[8];               //蓝牙名称
    uint8_t bp_flag;                    //血压支持
    uint8_t sit_flag;                   //久坐支持
    uint8_t retain[2];                  //保留  
}product_infomation_t;

//GUI支持语言枚举定义
//typedef enum
//{
//    GUI_LANGUAGE_CHINESE = 0x00,  //中文语言
//    GUI_LANGUAGE_ENGLISH = 0x01,  //英文语言
//}GUI_LANGUAGE_E;
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

// 消息推送开关
typedef union 
{
    uint16_t  type;
    struct data
    {
        uint16_t call:1;       // 来电
        uint16_t sms:1;        // 短信息
        uint16_t wechat:1;     // 微信开关
        uint16_t qq:1;         // qq开关
        uint16_t sina:1;       // 新浪开关
        uint16_t facebook:1;   // facebook开关
        uint16_t twitter:1;    // twitter开关
        uint16_t flickr:1;     // 暂不使用
        uint16_t linkedln:1;   // 
        uint16_t whatsapp:1;   // WhatsApp开关 
        uint16_t line:1;
        uint16_t instagram:1;
        uint16_t snapchat:1;
        uint16_t skype:1;
        uint16_t gmail:1;   
        uint16_t other:1;       //
    };
}app_message_enable_t;

// 久坐信息 
typedef struct
{
    uint8_t start_hour;    //久坐起始--小时
    uint8_t start_minute;  //久坐起始--分钟
    uint8_t end_hour;      //结束时间--小时
    uint8_t end_minute;    //结束时间--分钟

    uint8_t internal;      //久坐间隔时间
    uint8_t enable;        //久坐使能
}sedentary_config_t;

// 设备功能配置开关 
typedef struct
{
    uint8_t resv:1;                    //夜间翻腕  0--关闭      1--打开
    uint8_t wear_detect_enable:1;      //佩戴检测  0--关闭      1--打开
    uint8_t am_pm_enable:1;            //12小时制  0--24小时制  1--12小时制
    uint8_t english_style_enable:1;    //公/英制   0--公制      1--英制
    uint8_t heart_detect_enable:1;     //心率检测  0--关闭      1--打开
    uint8_t blood_detect_enable:1;     //血压检测  0--关闭      1--打开
    uint8_t resv1:2;                   //心率报警  0--关闭      1--打开
	
    uint8_t horizontal_enable:1;       // 横竖屏  0--关闭  1--打开
    uint8_t watchtimer_enable:1;       // 秒表开关 0--关闭 1--打开
    uint8_t low_bo_alert_enable:1;     // 低氧报警开关
    uint8_t find_phone_enable:1;       // 查找手机开关 0--关闭  1--打开
    uint8_t led_gear:1;                //led灯的等级
    uint8_t ble_disconnect_enable:1;   //蓝牙锻炼开关 
    uint8_t resv2:2;
}dev_config_t;

// 蓝牙名称结构体
typedef struct
{
    uint8_t length;   // 长度信息
    uint8_t name[8];  // 暂时只支持8个字节
}bluetooth_name_t;

typedef enum
{
    BLOOD_GENERAL_MODE = 0x00,  // 通用模式
    BLOOD_PRIVATE_MODE = 0x01,  // 私人模式
}BLOOD_MODE_E;

// 血压模式配置信息 
typedef struct
{
    uint8_t private_high;  // 私人模式--高血压
    uint8_t private_low;   // 私人模式--低血压

    BLOOD_MODE_E mode;     // 用户设定模式
}bp_config_t;

// 性别信息 
typedef enum
{
    GENDER_MALE = 0x01,    // 性别--男
    GENDER_FEMALE = 0x02,  // 性别--女
}PERSON_GENDER_E;

// 个人信息 
typedef struct
{
    uint8_t   height;        // 身高
    uint8_t   weight;        // 体重
    uint8_t   age;           // 年龄
    PERSON_GENDER_E gender;  // 性别
}person_infomation_t;

// 翻腕配置参数结构体
typedef struct
{
    uint8_t enable;        // 翻腕使能开关
    uint8_t sensitivity;   // 灵敏度等级[1,10]
    uint8_t s_hour;        // 翻腕起始时间--小时
    uint8_t s_minute;      // 翻腕起始时间--分钟
    uint8_t e_hour;        // 翻腕结束时间--小时
    uint8_t e_minute;      // 翻腕结束时间--分钟
}turnwrist_config_t;

// 心率报警参数结构体
typedef struct
{
    uint8_t enable;    //使能开关
    uint8_t high;      //高
    uint8_t low;       //低
}heart_alert_config_t;

// 亮度参数结构图
typedef struct
{
    uint8_t  start_hour;     //起始时间--小时
    uint8_t  start_minute;   //起始时间--分钟
    uint8_t  end_hour;       //结束时间--小时
    uint8_t  end_minute;     //结束时间--分钟
    uint8_t  valid_bright;   //有效亮度
    uint8_t  other_bright;   //其他亮度
}brightness_config_t;

//血氧开关参数结构体
typedef struct
{
    uint8_t  start_hour;     //起始时间--小时
    uint8_t  start_minute;   //起始时间--分钟
    uint8_t  end_hour;       //结束时间--小时
    uint8_t  end_minute;     //结束时间--分钟
    uint8_t  enable;         //其他亮度
}time_range_config_t;

//闹铃参数结构体
typedef struct
{
    uint8_t  state:1;   //闹钟--状态(1bit,节省内存空间)
    uint8_t  valid:1;   //闹钟--有效/无效(1bit,节省内存空间)
    uint8_t  resv:6;
    
    uint16_t  scence:5;  //闹钟--场景(3bit,最大8种场景)
    uint16_t  hour:5;    //闹钟--时(5bit,最大32)
    uint16_t  minute:6;  //闹钟--分(6bit,最大64)
    
    uint16_t  repeat:7;  //闹钟--重复
    uint16_t  month:4;   //闹钟--月(4bit,最大16)
    uint16_t  day:5;     //闹钟--日(5bit,最大32)
    
    uint16_t  year;       //闹钟--年(16bit)
}alarm_time_config_t;

// 倒计时用户设置信息
typedef struct
{
    uint8_t   ui_enable:1;          // UI是否显示
    uint8_t   app_send:1;           // APP发送
    uint8_t   btn_trigger:1;        // 按键触发倒计时功能
    uint8_t   app_trigger:1;        // APP开启倒计时功能
    uint8_t   resv:4;               // 预留对齐
    
    uint32_t  dev_remain_time;      // 手环倒计时时间
    uint32_t  run_remain_time;      // 运行倒计时时间
}hourglass_config_t;

// 日志数据索引信息
typedef struct
{
    uint16_t  offset;         // 偏移位置
    uint16_t  total_cnt;      // 总数量
}log_data_index_t;

// 日常数据索引信息
typedef struct
{    
    uint32_t  step_cnt[3];       //总的记步数量
    uint32_t  distance_cnt[3];   //总的距离数量
    uint32_t  calories_cnt[3];   //总的卡路里数量
    uint16_t  offset[3];         //五分钟数据偏移量
    uint16_t  ox_value_cnt[3];   //五分钟血氧有效数据个数
}daily_data_index_t;

// 血氧数据索引信息
typedef struct
{
    uint16_t  offset[3];             // 血氧数据偏移量
}oxygen_data_index_t;

// 运动心率FLASH索引结构体类型
typedef struct
{
    uint8_t   current_block_id;     // 当前保存ID
    uint16_t  offset[3];            // 块内偏移量
}sport_heart_record_t;

// FLASH查找索引结构体
typedef struct
{
    uint8_t               block_index;   //  数据保存索引块号
    log_data_index_t      log_s;         //  日志FLASH索引
    oxygen_data_index_t   oxygen;        //  血氧数据记录
    daily_data_index_t    daily;         //  睡眠位置信息 
    sport_heart_record_t  sport_heart;   //  运动心率
}flash_find_index_t;

// 未读消息的索引信息
typedef struct
{
    uint8_t  queue[8];
    uint8_t  write_pos;
    message_notify_content_t  unread_message;
}unread_message_index_t;

#pragma pack ()

#endif
