/******************************************************************************
 * Copyright (c) 2017 - 2018, Veepoo
 * 
 * All rights reserved.
 * 
** 文件名称: Thread_DataManage.h
** 创建时间: 2017-06-06
** 创 建 人: GT
** 描    述: 文件提供线程之间的共享数据管理的基本接口,接口描述如下:
**
*******************************************************************************/
#ifndef _DATA_MANAGE_H_
#define _DATA_MANAGE_H_

#include <stdint.h>
#include <stdbool.h>
#include "flash_data_type.h"
#include "afe4404_data_type.h"
#include "common_data_type.h"
#include "event_priority.h"
#include "test_mode.h"

// 消息推送属性结构体
typedef struct
{
    uint8_t                   unread_msg_count;      // 未读消息条数
    uint8_t                   update_index;          // 消息更新索引
    uint8_t                   display_len;           // 显示长度
    call_notify_content_t     call_content_s;        // 电话内容 
	message_notify_content_t  message_content_s[2];  // 消息推送内容
}app_notify_attr_t;

// 女性项目消息结构体
typedef struct
{
    uint8_t                   unread_phys_count;                        // 未读女性消息条数
    uint8_t                   display_len;                              // 显示长度
    uint16_t                  data_len;                                 // 消息的长度
	uint8_t                   content[1];             // 女性消息推送内容
}phys_notify_attr_t;

// 血压
typedef struct
{
    uint8_t cache_bp_sp;
    uint8_t cache_bp_dp;    
}bp_cache_t;


// 数据清除结构体
typedef struct
{
    uint8_t     clear_data:1;       // 清除数据
    uint8_t     clear_config:1;     // 清除配置
    uint8_t     clear_res:6;        // 保留      
}app_factory_clear_t;


//数据管理结构体
typedef struct
{
    uint8_t                      oled_bright;                 // OLED屏的亮度
    uint8_t                      alarm_scence;                // 闹铃场景ID
    uint8_t                      turnwrist_camera_state;      // 翻腕拍照状态
    uint32_t                     sedentary_cnt;               // 久坐计时
    system_clock_time_t          history_time;                // 系统历史时间
    event_queue_t                event_list;                  // 事件列表
    trace_message_t              trace_message_s;             // 跟踪运行时间
    device_trace_param_t         device_trace_param_s;        // 工厂信息
    app_upload_control_t         app_upload_control_s;        // 上传控制参数	
    acc_data_param_t             acc;                         // 加速度变量
    UPLOAD_ADC_EVENT             upload;
    app_notify_attr_t            app_notify_s;                // 消息推送成员变量 
    phys_notify_attr_t           phys_notify_s;               // 女性项目消息参数
    log_data_attr_t              log_attr_s;                  // 日志数据
    daily_data_attr_t            daily_attr;                  // 日常数据
    blood_oxygen_attr_t          oxygen_attr;                 // 血氧数据
    uint8_t                      oxygen_resut[5];             // 每分钟血氧平均值       
    history_pedometer_param      pedometer;                   // 记步参数
    fatigue_measure_t            fatigue_measure_s;           // 疲劳度参数
    blood_oxygen_measure_t       blood_oxygen_measure_s;      // 血氧测量
    heart_measure_t              heart_measure_s;             // 心率测量
    bp_measure_t                 bp_measure_s;                // 血压测量          
    hrv_measure_t                hrv_measure_s;               // HRV测量参数
    heart_load_param_t           heart_load_param_s;          // 心脏负荷参数
    scatter_heart_param_t        scatter_heart_param_s;       // 散点图心率参数
    respiratory_rate_measure_t   respiratory_rate_s;          // 呼吸率测量
    breath_stop_measure_t        breath_stop_s;               // 呼吸暂停
    wear_measure_t               wear_measure_s;              // 佩戴参数
    no_wear_measure_t            no_wear_measure_s;           // 脱手检测算法
    afe4404_data_pool            data_pool;                   // AFE4404数据缓存池
    
    bp_cache_t                   bp_cache_s;
    uint8_t                      ox_cache_s;

    app_factory_clear_t          app_factory_clear_s;           // 清除数据
	test_ox_info_t               test_ox_info_s;              //收集自动测量数据   

}data_manage_param;

extern data_manage_param*  p_data_manage;

// 数据管理初始化
void DataManage_Init(void);

// 数据更新GSensor FIFO数据
void DataManage_UpdateFIFOData(int16_t* p_fifo,uint8_t len);

// 处理姿态数据
void DataManage_ProcessGestureData(void);

// 处理记步数据
void DataManage_ProcessPedometerData(int16_t* p_xdata,int16_t* p_ydata,int16_t* p_zdata);

void DataManage_GetRTAccData(int16_t* p_data);

void DataManage_GetGsensorADCData(int16_t*  p_data);

void DataManage_GetGestureData(int16_t* p_data);

void DataManage_AccData_Send_Reset(void);

void DM_Update_DailyHeartData(uint8_t heart);

// 获取五分钟日常数据
daily_data_attr_t* DM_Get_NewestDailyData(void);

/*************************************************
 *           记步相关接口
 *************************************************/

// 获取手环端记步值
uint32_t DataManage_GetCurrentStepCnt(void);

/*************************************************
 *           OLED亮度相关接口
 *************************************************/
 void DM_Update_Brightness(uint8_t brightness);

/*************************************************
 *           AFE4404相关接口
 *************************************************/
 // AFE4404缓存池数据初始化
void DataManage_AFE4404PoolInit(void);

//  获取红光数据数据池
const int32_t* DataManage_GetRedPoolData(void);

//  获取红外数据池
const int32_t* DataManage_GetiRedDataPool(void);

////  获取HRV红外数据池(需要15S数据)
//const uint32_t* DataManage_GetRespiRedDataPool(void);

//  获取绿光数据池(4S)
const int32_t* DataManage_GetTiredGreenPool(void);

//  获取绿光数据池(4S)
const int32_t* DataManage_GetGreenCompletePool(void);

// 获取呼吸率的绿光数据
const int32_t* DataManage_GetRespGreenDataPool(void);

// 获取佩戴绿光数据池
const int32_t* DataManage_GetWearGreenDataPool(void);

//  获取环境光数据池
const int32_t* DataManage_GetAmbientDataPool(void);


void DataManage_UpdateBloodOxygenADCData(int32_t red, int32_t ired, int32_t green, int32_t ambient);

void DataManage_UpdateHeartADCData( int32_t green, int32_t ambient);


void DataManage_CopyBloodOxygenCacheData(void);

void DataManage_CopyHeartCacheData(void);


// 获取血氧ADC数据
void DataManage_GetOxygenADCData(uint8_t *p_green, uint8_t *p_red, uint8_t *p_ired, uint8_t *p_ambient); 

// 获取心率ADC数据
void DataManage_GetHeartADCData(uint8_t *p_green, uint8_t *p_ambient);

void DataManage_ProcessOxygenAttrData(void);

#endif
