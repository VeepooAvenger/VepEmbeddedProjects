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
#include "calc_data_type.h"
#include "common_data_type.h"
#include "event_priority.h"
#include "api_ext_flash.h"
#include "si1182_config.h"

// 消息推送属性结构体
typedef struct
{
    uint8_t                   unread_msg_count;      // 未读消息条数
    uint8_t                   update_index;          // 消息更新索引
    uint8_t                   display_len;           // 显示长度
    call_notify_content_t     call_content_s;        // 电话内容 
	message_notify_content_t  message_content_s[2];  // 消息推送内容
}app_notify_attr_t;

// 算法结果缓存
typedef struct
{
	uint8_t    sleep_state[6];          //睡眠状态
    uint8_t    sleep_effective;         //睡眠状态是否有效
    uint8_t    pulse_frequency[5];      //脉率
    uint8_t    heart_rate[5];           //心率
    uint8_t    breathing_rate[5];       //呼吸率
    
    uint8_t    hrv_type;
    uint8_t    hrv_value[50];
    uint8_t    bp_high;                 //高压
    uint8_t    bp_low;                  //低压
  
    uint8_t    cache_cardiac_load[5];       //心脏负荷
    uint8_t    cache_oxgen_value[5];        //血氧
    
    uint8_t    cache_breath_stop_cnt;       //呼吸暂停次数，填充第一个，后面填0
    uint8_t    cache_osahs_degree;          //呼吸暂停结果，填充第一个，后面填0
	uint8_t    cache_low_spo2_time;         //低氧(缺氧)时间，填充第一个，后面填0    
    uint8_t    cache_spo2_modification_flag;   //5分钟血氧修正标志位，填充一样的值
    
}algorithm_result_cache_t;



// 数据清除结构体
typedef struct
{
    uint8_t     clear_data:1;       // 清除数据
    uint8_t     clear_config:1;     // 清除配置
    uint8_t     clear_res:6;        // 保留      
}app_factory_clear_t;

// 获取睡眠结果
typedef struct
{
    uint16_t     sleep_all_time;         // 睡眠总时间
    uint16_t     sleep_shallow_time;     // 浅睡
    uint16_t     sleep_deep_time;        // 深睡     
    uint8_t      sleep_quality;          // 睡眠质量
}sleep_result_t;




//数据管理结构体
typedef struct
{
    uint8_t                      oled_bright;                 // OLED屏的亮度
    uint8_t                      alarm_scence;                // 闹铃场景ID
    uint8_t                      turnwrist_camera_state;      // 翻腕拍照状态
    uint32_t                     sedentary_cnt;               // 久坐计时
    system_clock_time_t          history_time;                // 系统历史时间
    event_queue_t                event_list;                  // 事件列表
    device_trace_param_t         device_trace_param_s;        // 工厂信息
    app_upload_control_t         app_upload_control_s;        // 上传控制参数	
    acc_data_param_t             acc;                         // 加速度变量
    UPLOAD_ADC_EVENT             upload;
    app_notify_attr_t            app_notify_s;                // 消息推送成员变量 
	algorithm_result_cache_t	 algorithm_result_cache;	  // 算法结果缓存
    daily_data_attr_t            daily_attr;                  // 日常数据
    history_pedometer_param      pedometer;                   // 记步参数
    ecg_ppg_time_t               ecg_ppg_time_s;              // 算法数据时间处理
    ecg_ppg_measure_t            ecg_ppg_s;                   // ecg\ppg测量
    
    wear_measure_t               wear_measure_s;              // 佩戴参数
    no_wear_measure_t            no_wear_measure_s;           // 脱手检测算法
    si1182_data_pool_t			 data_pool;                   // AFE4900数据缓存池
	raw_data_control_t           raw_data_control;            // 原始数据控制参数
    
    ecg_result_data_t            ecg_result_data_s;           // ECG 测量分析结果 
    
    ext_flash_ecg_opt_t          ext_flash_ecg_opt_s;         // ECG异常数据操作  
    ext_flash_movement_opt_t     ext_flash_movement_opt_s;    // 运动模式数据操作
    ext_flash_ecg_rest_opt_t     ext_flash_ecg_rest_opt_s;    // ECG结果数据操作
    ext_flash_daily_opt_t        ext_flash_daily_opt_s;       // 日常数据操作
    ext_flash_sleep_opt_t        ext_flash_sleep_opt_s;       // 睡眠数据操作
    ext_flash_head_opt_t         ext_flash_head_opt_s;        // 外部FLASH头部信息
    ext_flash_all_opt_t          ext_flash_all_opt_s;         // 外部FLASH操作
    
    uint8_t                      app_bp_mode;                 // 设置app测量模式
    app_factory_clear_t          app_factory_clear_s;         // 清除数据
    //test
    log_data_attr_t              log_attr_s;                  // 日志数据
    //trace_message_t              trace_message_s;             // 跟踪运行时间
    uint8_t                      ppg_adc_mode;                  // ppg自动测量数据收集
    power_statistics_t           power_statistics_s;            // 功耗统计
    sleep_result_t               sleep_result_s;                // 睡眠结果
    uint8_t                      sleep_flag;                    // 睡眠状态
}data_manage_param;

extern data_manage_param    *p_data_manage;

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
daily_data_attr_t* DataManage_NewestDailyData(void);

// 保存完后，重置五分钟数据
void DataManage_InitDailyData(void);

/*************************************************
 *           记步相关接口
 *************************************************/

// 获取手环端记步值
uint32_t DataManage_GetCurrentStepCnt(void);

/*************************************************
 *           OLED亮度相关接口
 *************************************************/
void DM_Update_Brightness(uint8_t brightness);


////////////////////////////////////
//         SI1182 函数接口        //
///////////////////////////////////


void DataManage_SI1182PoolInit(void);

void DataManage_PoolSecondUpdate(void);

int32_t DataManage_UpdateAdcData(uint32_t adc_mode, uint32_t start_flag, uint32_t over_flag);

void DataManage_InitPoolData(void);

void DataManage_GetEcgAdcData(uint8_t *ecg_data, uint8_t adc_type);

void DataManage_GetEcgLdData1(uint8_t *ld_data);

void DataManage_GetEcgLdData2(uint8_t *ld_data);

void DataManage_GetPpgAdcDataGreenAmb(uint8_t *ppg_green, uint8_t *ppg_amb);

void DataManage_GetPpgAdcDataGreenIredAmb(uint8_t *ppg_green, uint8_t *ppg_ired, uint8_t *ppg_amb);

void DataManage_GetPpgAdcDataGreenRedIredAmb(uint8_t *ppg_green, uint8_t *ppg_red, uint8_t *ppg_ired, uint8_t *ppg_amb);

const int32_t* DataManage_GetEcgDataPool(void);
int32_t* DataManage_GetEcgDataPoolLd(void);


const int32_t* DataManage_GetGreenDataPool(void);

const int32_t* DataManage_GetRedDataPool(void);

const int32_t* DataManage_GetIredDataPool(void);

const int32_t* DataManage_GetAmbDataPool(void);

const int32_t* DataManage_GetWearDataGreenPool(void);

const int32_t* DataManage_GetWearDataIredPool(void);

#endif
