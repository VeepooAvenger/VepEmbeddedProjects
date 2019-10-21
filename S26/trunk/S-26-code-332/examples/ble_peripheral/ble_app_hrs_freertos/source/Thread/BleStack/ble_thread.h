/******************************************************************************
 * Copyright (c) 2017 - 2018, Veepoo
 * 
 * All rights reserved.
 * 
 * 文件名称: BleStack_Thread.h
 * 创建时间: 2017-06-15
 * 创 建 人: GT
 * 功能描述:
 *
*******************************************************************************/
#ifndef _BLE_STACK_THREAD_H_
#define _BLE_STACK_THREAD_H_

#include <stdint.h>
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "ble_nus.h"
#include "ble_clk.h"
#include "common_data_type.h"

#define DEVICE_NAME                      "FM01"                            /**< Name of device. Will be included in the advertising data. */
#define MANUFACTURER_NAME                "NordicSemiconductor"            /**< Manufacturer. Will be passed to Device Information Service. */
#define APP_ADV_SLOW_INTERVAL            3200                             /**< Slow advertising interval (in units of 0.625 ms). The default value corresponds to 2 seconds. */
#define APP_ADV_SLOW_TIMEOUT             180                              /**< The advertising time-out in units of seconds. */


#define APP_ADV_INTERVAL                 900//300                              /**< The advertising interval (in units of 0.625 ms. This value corresponds to 187.5 ms). */
#define APP_ADV_TIMEOUT_IN_SECONDS       10//180                              /**< The advertising time-out in units of seconds. */
#define CENTRAL_LINK_COUNT               0                                /**< Number of central links used by the application. When changing this number remember to adjust the RAM settings*/
#define PERIPHERAL_LINK_COUNT            1                                /**< Number of peripheral links used by the application. When changing this number remember to adjust the RAM settings*/


#define IS_SRVC_CHANGED_CHARACT_PRESENT  1                                /**< Include the Service Changed characteristic. If not enabled, the server's database cannot be changed for the lifetime of the device. */

// WJ.20181120.为了加快从机的数据发送速度，减小连接间隔，增加连接确认忽略个数！！！
#define MIN_CONN_INTERVAL                   MSEC_TO_UNITS(40, UNIT_1_25_MS)//MSEC_TO_UNITS(15, UNIT_1_25_MS) /**< Minimum acceptable connection interval (0.4 seconds). */
#define MAX_CONN_INTERVAL                   MSEC_TO_UNITS(65, UNIT_1_25_MS)//MSEC_TO_UNITS(35, UNIT_1_25_MS) /**< Maximum acceptable connection interval (0.65 second). */
#define SLAVE_LATENCY                       3 //0                               /**< Slave latency. */
#define CONN_SUP_TIMEOUT                    MSEC_TO_UNITS(4000, UNIT_10_MS)  /**< Connection supervisory time-out (4 seconds). */

#define MIN_CONN_INTERVAL_1                 MSEC_TO_UNITS(15, UNIT_1_25_MS)//MSEC_TO_UNITS(15, UNIT_1_25_MS) /**< Minimum acceptable connection interval (0.4 seconds). */
#define MAX_CONN_INTERVAL_1                 MSEC_TO_UNITS(35, UNIT_1_25_MS)//MSEC_TO_UNITS(35, UNIT_1_25_MS) /**< Maximum acceptable connection interval (0.65 second). */
#define SLAVE_LATENCY_1                     4                               /**< Slave latency. */
#define CONN_SUP_TIMEOUT_1                  MSEC_TO_UNITS(4000, UNIT_10_MS)  /**< Connection supervisory time-out (4 seconds). */


#define FIRST_CONN_PARAMS_UPDATE_DELAY   5000                             /**< Time from initiating event (connect or start of notification) to first time sd_ble_gap_conn_param_update is called (5 seconds). */
#define NEXT_CONN_PARAMS_UPDATE_DELAY    30000                            /**< Time between each call to sd_ble_gap_conn_param_update after the first call (30 seconds). */
#define MAX_CONN_PARAMS_UPDATE_COUNT     3                                /**< Number of attempts before giving up the connection parameter negotiation. */

#define SEC_PARAM_BOND                   1                                /**< Perform bonding. */
#define SEC_PARAM_MITM                   0                                /**< Man In The Middle protection not required. */
#define SEC_PARAM_LESC                   0                                /**< LE Secure Connections not enabled. */
#define SEC_PARAM_KEYPRESS               0                                /**< Keypress notifications not enabled. */
#define SEC_PARAM_IO_CAPABILITIES        BLE_GAP_IO_CAPS_NONE             /**< No I/O capabilities. */
#define SEC_PARAM_OOB                    0                                /**< Out Of Band data not available. */
#define SEC_PARAM_MIN_KEY_SIZE           7                                /**< Minimum encryption key size. */
#define SEC_PARAM_MAX_KEY_SIZE           16                               /**< Maximum encryption key size. */

#define DEAD_BEEF                        0xDEADBEEF                       /**< Value used as error code on stack dump, can be used to identify stack location on stack unwind. */

//#define OSTIMER_WAIT_FOR_QUEUE           2                                /**< Number of ticks to wait for the timer queue to be ready */

#if (NRF_SD_BLE_API_VERSION == 3)
    #define NRF_BLE_MAX_MTU_SIZE            GATT_MTU_SIZE_DEFAULT             /**< MTU size used in the softdevice enabling and to reply to a BLE_GATTS_EVT_EXCHANGE_MTU_REQUEST event. */
#endif

#define APP_FEATURE_NOT_SUPPORTED        BLE_GATT_STATUS_ATTERR_APP_BEGIN + 2        /**< Reply when unsupported features are requested. */

typedef struct
{
    ble_nus_t          m_ble_nus;                 //  
    ble_clk_t          m_ble_clk;
	
    SemaphoreHandle_t  m_afe4404_heart_open;      // AFE4404 心率模式(开)
    SemaphoreHandle_t  m_afe4404_bo_open;         // AFE4404 血氧模式(开)
    SemaphoreHandle_t  m_afe4404_fatigue_open;    // AFE4404 疲劳度模式(开)
    SemaphoreHandle_t  m_afe4404_close;           // AFE4404设备操作(关)
		
    SemaphoreHandle_t  m_dev_connect;             // 蓝牙连接信号量
    SemaphoreHandle_t  m_dev_disconnect;          // 蓝牙断开信号量
    SemaphoreHandle_t  m_adv_start;               // 广播开启信号量
    SemaphoreHandle_t  m_adv_stop;                // 广播停止信号量
    SemaphoreHandle_t  m_gsensor_restart;         // GSensor重新开机初始化信号量
		
    SemaphoreHandle_t  m_app_config;              // App开关配置信号量
    SemaphoreHandle_t  m_factory_clear;           // 恢复出厂设置
    SemaphoreHandle_t  m_init_zerotime;           // 0点时刻初始化	
    SemaphoreHandle_t  m_save_systemtime;         // 保存系统时间
    SemaphoreHandle_t  m_save_dailydata;          // 保存五分钟数据
    SemaphoreHandle_t  m_save_oxygendata;         // 保存血氧数据
    SemaphoreHandle_t  m_adjust_time_clear;       // 对时清除数据
    
    SemaphoreHandle_t  m_led_abnormal_monitor;    // AFE4404灯异常实时监测

    SemaphoreHandle_t  m_ble_name_changa;         // 修改蓝牙名称
    SemaphoreHandle_t  m_bp_func_set;             // 血压功能模块
	
    SemaphoreHandle_t  m_task_count;              // 任务数量计数信号量
	
    TaskHandle_t       m_thread_handle;           //线程句柄
}bthread_res_t;

// 蓝牙任务线程初始化
void BleThread_Init(void);

// 蓝牙任务线程心率灯打开
void BleThread_AFE4404HeartOpen(void);

// 蓝牙任务线程血氧灯打开
void BleThread_AFE4404BloodOxygenOpen(void);

// 疲劳度开始测量任务
void BleThread_AFE4404FatigueOpen(void);

// 血压测量
void BleThread_AFE4404BpOpen(void);

// AFE4404关灯任务
void BleThread_AFE4404DevClose(void);

// 蓝牙连接设备任务
void BleThread_ConnectDevice(void);

// 断开设备任务
void BThread_DisconnectDevice(bool app_command_disconnect);

// 广播启动任务
void BleThread_AdvertisingStart(void);

// 广播停止任务
void BleThread_AdvertisingStop(void);

// GSensor重启
void BleThread_GsensorRestart(void);

// 周期FLASH更新任务
void BleThread_PeriodCycleTask(uint8_t hour, uint8_t minute, uint8_t second);

// 对时擦除FLASH数据
void BleThread_AdjustTimeClearFlash(const system_clock_time_t* p_history_time);

// 更新APP开关信息
void BleThread_AsynUpdateAppConfig(void);

// 恢复出厂清除数据
void BleThread_FactoryClearData(void);

// 获取BAT连接句柄结构体指针
ble_nus_t* BleThread_GetBleNus(void);

// 获取CLK连接句柄结构体指针
ble_clk_t* BleThread_GetBleClk(void);

//延时后继续开启蓝牙广播
void BleThread_RestartAdv(void);

void BleThread_UpdateBleNameConfig(void);

void BleThread_UpdateBpFuncConfig(void);

#endif

