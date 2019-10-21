#ifndef _PULSE_DATA_TYPE_H_
#define _PULSE_DATA_TYPE_H_

#include <stdint.h>

// !注意:这里的宏定义随着包含文件实现项目的区分
#define PULSE_DATA_TYPE     1

// FLASH操作参数结构体
typedef struct
{
	uint8_t  time_data_update:1;    //时间数据FLASH写入
	uint8_t  daily_data_update:1;   //日常数据FLASH写入
	uint8_t  oxygen_data_update:1;  //血氧数据FLASH写入
	uint8_t  sleep_data_update:1;   //睡眠数据FLASH写入
	
	uint8_t  clear_current_day:1;   //清除当天数据
	uint8_t  clear_all_day:1;       //清除所有数据
	uint8_t  jump_block:1;          //FLASH换块
	uint8_t  resv:1;                //预留
}flash_opt_param;

// 上传采集数据枚举类型
typedef enum
{
	UPLOAD_ADC_GSENSOR_EVENT = 1,      //上传Gsensor原始ADC数据事件
	UPLOAD_ADC_GREEN_EVENT = 2,        //上传绿光原始ADC数据事件
	UPLOAD_ADC_OXYGEN_EVENT = 3,       //上传血氧原始ADC数据事件
	UPLOAD_ADC_NULL_EVENT,
}UPLOAD_ADC_EVENT;

// 测量参数结构体
typedef struct
{
	uint16_t  heart_btn:1;    // 按键启动心率算法
	uint16_t  heart_app:1;    // App启动心率算法
	uint16_t  heart_time:1;   // 定时测量启动心率算法
	
	uint16_t  bp_btn:1;       // 按键血压算法
	uint16_t  bp_app:1;       // App血压算法
	uint16_t  bp_time:1;      // 定时测量血压算法
	
	uint16_t  bp_init:1;      // 血压算法初始化
	uint16_t  heart_init:1;   // 心率算法初始化
	uint16_t  wear_init:1;    // 佩戴算法初始化
	
	uint16_t  resv:7;         // 保留对齐7字节
}measure_param_t;

//记步历史数据
typedef struct
{
	uint32_t  power_on_step;    // 手环开机记步值
	
	uint32_t  daily_step;       // 日常历史记步数据
	uint32_t  daily_sport;      // 日常历史运动量数据
	
	uint32_t   oxygen_step;     // 血氧历史记步数据
	uint32_t   oxygen_sport;    // 血氧历史运动量数据
}history_pedometer_param;

typedef struct
{
	uint8_t  index;             // 久坐记步数据总数
	uint8_t  step[5];           // 久坐记步数据
	
	uint32_t history_step;      // 久坐历史记步数据
}sedentary_pedometer_param;

//========================================
//   *****脉搏波缓冲区配置*******
//========================================
#define WEAR_DATA_POOL_SIZE       40
#define WEAR_DATA_CACHE_SIZE      40
#define PULSE_DATA_POOL_SIZE      1000
#define PULSE_DATA_CACHE_SIZE     128

// 脉搏数据缓冲数据结构体
typedef struct
{
	uint16_t  wear_read_pos;
	uint16_t  wear_wait_send_len;
	uint16_t  wear_send_package;
	
	uint16_t  pulse_read_pos;
	uint16_t  pulse_wait_send_len;
	uint16_t  pulse_send_package;
	
	uint16_t  wear_cache_update_cnt;          // 佩戴缓存更新数据量
	uint16_t  wear_pool_update_total_cnt;     // 佩戴池更新总数据量
	uint16_t  pulse_cache_update_cnt;         // 脉冲缓存更新数据量
	uint16_t  pulse_pool_update_total_cnt;    // 脉冲池更新总数据量
	
	uint16_t  wear[WEAR_DATA_POOL_SIZE+WEAR_DATA_CACHE_SIZE];     // 佩戴数据池
	uint16_t  pulse[PULSE_DATA_POOL_SIZE+PULSE_DATA_CACHE_SIZE];  // 脉冲数据池
}pulse_data_pool_t;

#endif
