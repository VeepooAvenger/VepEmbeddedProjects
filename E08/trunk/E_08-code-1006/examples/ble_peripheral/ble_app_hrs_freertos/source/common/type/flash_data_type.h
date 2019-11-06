#ifndef _FLASH_DATA_TYPE_H_
#define _FLASH_DATA_TYPE_H_

#include <stdint.h>
#include "common_data_type.h"

/*===================================================================
 *                            定义FLASH基地址           
 *===================================================================
 */

#define PRODUCT_INFOMATION_BASE_ADDR    0x73000     // 产品信息基地址
#define PRODUCT_INFOMATION_TOTAL_SIZE   0x1000      // 产品信息大小

//闹钟和系统配置参数共用4K
#define ALARM_CONFIG_BASE_ADDR          0x72000     // 闹钟
#define SYSTEM_PARAM_BASE_ADDR          0x72000     // 系统参数基地址
#define APP_CONFIG_BASE_ADDR            0x72400     // APP配置基地址

//系统时间4K
#define SYSTEM_TIME_BASE_ADDR           0x71000     // 系统时间


//睡眠数据  (改成片外FLASH)
#define RECORD_SLEEP_BASE_ADDR          0x00001000//0x6E000     // 睡眠数据基地址
#define RECORD_SLEEP_DATA_SIZE          0x1000      // 每天数据大小4KB
#define RECORD_SLEEP_TOTAL_SIZE         0x3000      // 睡眠总计12K
// 睡眠数据地址宏
#define RECORD_SLEEP_DATA_ADDR(day_id, offset)       (uint32_t)(RECORD_SLEEP_BASE_ADDR + RECORD_SLEEP_DATA_SIZE*(day_id) + offset)
// 睡眠数据偏移地址
#define SLEEP_BASIC_CHECK_OFFSET        3            // 睡眠基本信息开始偏移量
#define SLEEP_BASIC_INFO_OFFSET         13           // 睡眠基本信息开始偏移量
#define SLEEP_INSOMINA_OFFSET           51           // 睡眠失眠信息开始偏移量
#define SLEEP_CURVE_MIG_OFFSET          97           // 睡眠曲线信息开始偏移量
// 睡眠信息长度
#define SLEEP_BASIC_CHECK_LEN           8            // 睡眠信息校验长度
#define SLEEP_BASIC_INFO_LEN            36           // 睡眠基本信息长度
#define SLEEP_INSOMINA_LEN              44           // 睡眠失眠信息长度

// 睡眠其他定义
#define SLEEP_ONE_DAY_MAX_TIMES         10           // 每天最多五次
#define SLEEP_PER_DATA_MAX_SIZE         2980         // 每段睡眠信息的最大长度


//日常数据
#define RECORD_DAILY_BASE_ADDR          0x00004000//0x59000     // 全天数据基地址
#define RECORD_DAILY_DATA_SIZE          0x9000      // 日常数据大小(36KB)   
#define RECORD_DAILY_ONE_DAY_SIZE       0x9000      // 全天数据的大小(36KB)
#define RECORD_DAILY_PER_DATA_SIZE      0x78        // 每条数据的大小(120B), 确保是4的倍数
#define RECORD_DAILY_TOTAL_SIZE         0x1B000     // 三天数据大小108K

#define RECORD_POWER_INFO_SIZE          16          // 用于功耗统计，存储在一天的最后24Byte

// 日常数据地址宏
#define RECORD_DAILY_DATA_ADDR(day_id, offset)       (uint32_t)(RECORD_DAILY_BASE_ADDR + RECORD_DAILY_ONE_DAY_SIZE*(day_id) + offset * RECORD_DAILY_PER_DATA_SIZE)
#define RECORD_DAILY_DATA_MAX_CNT       307         // 全天可以保存的最大数据条数
// 定义一些合法性检测(由于分五分钟数据冗余2小时, 所以会最多26小时数据: 26x60/5 = 312)
#define RECORD_DAILY_CONDITION_VALID(day_id, offset)  ((day_id < 3)&&(offset < RECORD_DAILY_DATA_MAX_CNT))

//运动模式       
#define RECORD_MOVEMENT_BASE_ADDR           0x0001F000//0x53000     // 运动模式的基地址
#define RECORD_MOVEMENT_ONE_TIMES_SIZE      0x2000      // 运动模式数据总大小(8KB)
#define RECORD_MOVEMENT_TOTAL_SIZE          0x6000      // 运动模式数据总大小(24KB)
#define RECORD_MOVEMENT_HEAD_OFFSET         0x50        // 运动模式数据的偏移量
// 运动模式数据地址宏
#define RECORD_MOVEMENT_HEAD_ADDR(block_id, offset)   (uint32_t)(RECORD_MOVEMENT_BASE_ADDR + (RECORD_MOVEMENT_ONE_TIMES_SIZE*block_id) + (offset*0))
#define RECORD_MOVEMENT_DATA_ADDR(block_id, offset)   (uint32_t)(RECORD_MOVEMENT_BASE_ADDR + RECORD_MOVEMENT_HEAD_OFFSET + (RECORD_MOVEMENT_ONE_TIMES_SIZE*block_id) + (offset*20))


// ECG手动测量结果数据        
#define RECORD_ECG_BASE_ADDR            0x00025000//0x51000     // ECG的基地址
#define RECORD_ECG_ONE_TIMES_SIZE       0x1000      // ECG数据总大小(4KB)
#define RECORD_ECG_TOTAL_SIZE           0x2000      // ECG数据总大小(8KB)
#define RECORD_ECG_PER_DATA_SIZE        0x18        // ECG数据的偏移量 24 Byte
#define RECORD_ECG_MAX_CNT              341         // ECG数据的最大条数 4096*2/ 24
#define RECORD_ECG_ONE_BLOCK_MAX_CNT    170         // ECG数据在第一块的最大条数 4096/ 24
// ECG手动测量结果数据地址宏
#define RECORD_ECG_DATA_ADDR(offset)            (uint32_t)(RECORD_ECG_BASE_ADDR + (offset*RECORD_ECG_PER_DATA_SIZE))
#define RECORD_ECG_DATA_ERASE_ADDR(block_id)            (uint32_t)(RECORD_ECG_BASE_ADDR + (block_id*RECORD_ECG_ONE_TIMES_SIZE))
// 定义一些合法性检测(4096*2/24 = 341)
#define RECORD_ECG_CONDITION_VALID(offset)      (offset < RECORD_ECG_MAX_CNT)


//日志
#define LOG_BASE_ADDR                   0x70000//0x50000      // 日志基地址
#define LOG_DATA_SIZE                   0x1000       // 日志数据大小(4KB)
// 日志数据地址宏
#define LOG_DATA_ADDR(offset)  (uint32_t)(LOG_BASE_ADDR+offset)


// Flash读取数据地址宏
#define RECORD_FLASH_BLOCK_DATA_ADDR(block_id, offset)  (uint32_t)(SYSTEM_TIME_BASE_ADDR + (block_id)*1024 + offset)
#define RECORD_FLASH_BLOCK_DATA_SIZE                    (PRODUCT_INFOMATION_BASE_ADDR - SYSTEM_TIME_BASE_ADDR)  
//#define RECORD_FLASH_BLOCK_DATA_ADDR(block_id, offset)  (uint32_t)(RECORD_ECG_BASE_ADDR + (block_id)*1024 + offset)
//#define RECORD_FLASH_BLOCK_DATA_SIZE                    (PRODUCT_INFOMATION_BASE_ADDR - RECORD_ECG_BASE_ADDR)  
//#define RECORD_FLASH_BLOCK_USER_DATA_SIZE               (SYSTEM_TIME_BASE_ADDR - RECORD_ECG_BASE_ADDR)   


// 定义日志合法性检测
#define LOG_CONDITION_VALID(offset)  (offset < 256)




/*===================================================================
 *                            定义FLASH结构体           
 *===================================================================
 */
 
#pragma pack (1)
#pragma anon_unions
 

//日常数据
typedef struct
{
    uint8_t    month;                   //月
    uint8_t    day;                     //日
    uint8_t    hour;                    //时
    uint8_t    minute;                  //分
    uint16_t   step;                    //计步
    uint16_t   sport;                   //运动量
    uint16_t   distance;                //距离
    uint16_t   calories;                //卡路里
    
    uint8_t    gesture;                 //姿态
    uint8_t    wear;                    //佩戴
    
    uint8_t    sleep_state[6];          //睡眠状态
    uint8_t    pulse_frequency[5];      //脉率
    uint8_t    heart_rate[5];           //心率
    uint8_t    breathing_rate[5];       //呼吸率
    
    uint8_t    hrv_type;
    uint8_t    hrv_value[50];
    
    uint8_t    bp_h;                    //血压-高压
    uint8_t    bp_l;                    //血压-低压
    
    uint8_t    oxgen_value[5];          //血氧
    uint8_t    breath_stop_cnt[5];      //呼吸暂停次数，填充第一个，后面填0
    uint8_t    osahs_degree[5];         //呼吸暂停结果，填充第一个，后面填0
	uint8_t    low_spo2_time[5];        //低氧(缺氧)时间，填充第一个，后面填0
    
    uint8_t    cardiac_load[5];             //心脏负荷
    uint8_t    spo2_modification_flag[5];   //5分钟血氧修正标志位，填充一样的值
    
    uint8_t    resv[2];
}daily_data_attr_t;

// ECG数据
typedef struct
{
    uint8_t    mark;                    //标志
    uint16_t   year;                    //年
    uint8_t    month;                   //月
    uint8_t    day;                     //日
    uint8_t    hour;                    //时
    uint8_t    minute;                  //分
    uint8_t    second;                  //秒
    uint8_t    ecg_result_data[14];     //ECG测量结果
    uint8_t    ecg_res[2];              //保留
}ecg_result_data_t;

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
typedef enum
{
    CONFIG_LANGUAGE_CHINESE = 1,  //中文
    CONFIG_LANGUAGE_ENAGLISH, //英文
    CONFIG_LANGUAGE_JAPANESE, //日语
    CONFIG_LANGUAGE_KOREAN,   //韩语
    CONFIG_LANGUAGE_GERMAN,   //德语
    CONFIG_LANGUAGE_RUSSIAN,  //俄语
    CONFIG_LANGUAGE_SPANISH,  //西班牙语
    CONFIG_LANGUAGE_ITALIARN, //意大利语
    CONFIG_LANGUAGE_FRENCH,   //法语
    CONFIG_LANGUAGE_VIETNAMESE,   //越南语
    CONFIG_LANGUAGE_PORTUGUESE,   //葡萄牙语
    CONFIG_LANGUAGE_TRADITIONAL,  //繁体
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
    uint8_t resv:1;                     //夜间翻腕  0--关闭      1--打开
    uint8_t wear_detect_enable:1;       //佩戴检测  0--关闭      1--打开
    uint8_t am_pm_enable:1;             //12小时制  0--24小时制  1--12小时制
    uint8_t english_style_enable:1;     //公/英制   0--公制      1--英制
    uint8_t heart_detect_enable:1;      //心率检测  0--关闭      1--打开
    uint8_t blood_detect_enable:1;      //血压检测  0--关闭      1--打开
    uint8_t ecg_detect_enable:1;        //ecg自动测量   0--关闭      1--打开 
    uint8_t ppg_detect_enable:1;        //ecg自动测量   0--关闭      1--打开
	
    uint8_t alarm_enable:1;             // WJ.20190514修改为闹钟启用开关；横竖屏  0--关闭  1--打开
    uint8_t watchtimer_enable:1;        // 秒表开关 0--关闭 1--打开
    uint8_t low_bo_alert_enable:1;      // 低氧报警开关
    uint8_t find_phone_enable:1;        // 查找手机开关 0--关闭  1--打开
    uint8_t led_gear:1;                 // led灯的等级
    uint8_t ble_disconnect_enable:1;    // 蓝牙锻炼开关 
    uint8_t precise_sleep:1;            // 精准睡眠
    uint8_t resv2:1;
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
    GENDER_MALE = 0x01,  // 性别--男
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
    uint8_t sensitivity;   // 灵敏度等级[1, 10]
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


#define   APP_USED_OLD_ALARM_ENABLE     1       //使用旧的闹钟

//闹铃参数结构体
typedef struct
{
    uint8_t  state:1;   //闹钟--状态(1bit, 节省内存空间)
    uint8_t  valid:1;   //闹钟--有效/无效(1bit, 节省内存空间)
    uint8_t  resv:6;
    
    uint16_t  scence:5;  //闹钟--场景(3bit, 最大8种场景)
    uint16_t  hour:5;    //闹钟--时(5bit, 最大32)
    uint16_t  minute:6;  //闹钟--分(6bit, 最大64)
    
    uint16_t  repeat:7;  //闹钟--重复
    uint16_t  month:4;   //闹钟--月(4bit, 最大16)
    uint16_t  day:5;     //闹钟--日(5bit, 最大32)
    
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
    uint8_t   rest_cnt[3];       //复位记录此时
}daily_data_index_t;

// ECG数据索引信息
typedef struct
{
    uint16_t    offset_start_index;     // ECG 结果数据偏移量
    uint16_t    offset_end_index;       // ECG 结果数据偏移量   
}ecg_data_index_t;

// ECG 异常数据索引信息
typedef struct
{
    uint8_t     ecg_cnt;       // ECG 自动测量异常数据偏移量   
    uint16_t    offset_ext_index;       // ECG 自动测量异常数据偏移量   
}ecg_ext_data_index_t;

// FLASH查找索引结构体
typedef struct
{
    uint8_t                 block_index;    // 数据保存索引块号
    log_data_index_t        log_s;          // 日志FLASH索引
    daily_data_index_t      daily;          // 五分钟日常数据位置信息 
    ecg_data_index_t        ecg;            // ecg手动测量结果数据位置信息 
    ecg_ext_data_index_t    ecg_ext;        // ecg自动测量异常数据位置信息 
}flash_find_index_t;

// 用于功耗统计
typedef struct
{
    uint16_t  bright_screen_num;        //亮屏次数
    uint16_t  bright_screen_time;       //亮屏时长
    
    uint16_t  motor_vibration_num;      //马达震动次数
    uint16_t  motor_vibration_time;     //马达震动时长
    
    uint32_t  ble_connection_time;      //蓝牙连接时长
    
    uint8_t   charging;                 //是否充过电
    uint8_t   rest_cnt;                 //复位次数
    uint8_t   res[2];
}power_statistics_t;

#pragma pack ()

#endif
