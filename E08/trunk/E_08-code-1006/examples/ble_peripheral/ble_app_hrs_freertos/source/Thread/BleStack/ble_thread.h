/******************************************************************************
 * Copyright (c) 2017 - 2018, Veepoo
 * 
 * All rights reserved.
 * 
 * �ļ�����: BleStack_Thread.h
 * ����ʱ��: 2017-06-15
 * �� �� ��: GT
 * ��������:
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

#define DEVICE_NAME                      "W74"                            /**< Name of device. Will be included in the advertising data. */
#define MANUFACTURER_NAME                "NordicSemiconductor"            /**< Manufacturer. Will be passed to Device Information Service. */
#define APP_ADV_SLOW_INTERVAL            3200                             /**< Slow advertising interval (in units of 0.625 ms). The default value corresponds to 2 seconds. */
#define APP_ADV_SLOW_TIMEOUT             180                              /**< The advertising time-out in units of seconds. */


#define APP_ADV_INTERVAL                 900//300                              /**< The advertising interval (in units of 0.625 ms. This value corresponds to 187.5 ms). */
#define APP_ADV_TIMEOUT_IN_SECONDS       10//180                              /**< The advertising time-out in units of seconds. */
#define CENTRAL_LINK_COUNT               0                                /**< Number of central links used by the application. When changing this number remember to adjust the RAM settings*/
#define PERIPHERAL_LINK_COUNT            1                                /**< Number of peripheral links used by the application. When changing this number remember to adjust the RAM settings*/


#define IS_SRVC_CHANGED_CHARACT_PRESENT  1                                /**< Include the Service Changed characteristic. If not enabled, the server's database cannot be changed for the lifetime of the device. */

// WJ.20181120.Ϊ�˼ӿ�ӻ������ݷ����ٶȣ���С���Ӽ������������ȷ�Ϻ��Ը���������
#define MIN_CONN_INTERVAL                MSEC_TO_UNITS(32, UNIT_1_25_MS)//MSEC_TO_UNITS(15, UNIT_1_25_MS) /**< Minimum acceptable connection interval (0.4 seconds). */
#define MAX_CONN_INTERVAL                MSEC_TO_UNITS(45, UNIT_1_25_MS)//MSEC_TO_UNITS(35, UNIT_1_25_MS) /**< Maximum acceptable connection interval (0.65 second). */
#define SLAVE_LATENCY                    3 //0                               /**< Slave latency. */
#define CONN_SUP_TIMEOUT                 MSEC_TO_UNITS(4000, UNIT_10_MS)  /**< Connection supervisory time-out (4 seconds). */

#define MIN_CONN_INTERVAL_1                MSEC_TO_UNITS(15, UNIT_1_25_MS)//MSEC_TO_UNITS(15, UNIT_1_25_MS) /**< Minimum acceptable connection interval (0.4 seconds). */
#define MAX_CONN_INTERVAL_1                MSEC_TO_UNITS(35, UNIT_1_25_MS)//MSEC_TO_UNITS(35, UNIT_1_25_MS) /**< Maximum acceptable connection interval (0.65 second). */
#define SLAVE_LATENCY_1                    4                               /**< Slave latency. */
#define CONN_SUP_TIMEOUT_1                 MSEC_TO_UNITS(4000, UNIT_10_MS)  /**< Connection supervisory time-out (4 seconds). */




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
	
    SemaphoreHandle_t  m_ppg_open;                      //  ����ģʽ(��)
    SemaphoreHandle_t  m_ecg_open;                      //  ECGģʽ(��)
    SemaphoreHandle_t  m_oxygen_open;                   //  Ѫ��ģʽ(��)
    //SemaphoreHandle_t  m_ppg_oxygen_open;               //  ppg+Ѫ��ģʽ(��)
    //SemaphoreHandle_t  m_ptt_open;                      //  PTTģʽ(��)
    SemaphoreHandle_t  m_analog_peripherals_close;      //  �������(��)
		
    SemaphoreHandle_t  m_dev_connect;               // ���������ź���
    SemaphoreHandle_t  m_dev_disconnect;            // �����Ͽ��ź���
    SemaphoreHandle_t  m_adv_start;                 // �㲥�����ź���
    SemaphoreHandle_t  m_adv_stop;                  // �㲥ֹͣ�ź���
		
    SemaphoreHandle_t  m_app_config;                // App���������ź���	
    SemaphoreHandle_t  m_save_systemtime;           // ����ϵͳʱ��

    
    SemaphoreHandle_t  m_led_abnormal_monitor;      // AFE4900���쳣ʵʱ���

    SemaphoreHandle_t  m_ble_name_changa;           // �޸���������
    SemaphoreHandle_t  m_sit_func_set;              // ��������ģ��
    SemaphoreHandle_t  m_bp_func_set;               // Ѫѹ����ģ��

/*
    SemaphoreHandle_t  m_factory_clear;             // �ָ���������
    SemaphoreHandle_t  m_init_zerotime;             // 0��ʱ�̳�ʼ��    
    SemaphoreHandle_t  m_save_dailydata;            // �������������    
    SemaphoreHandle_t  m_save_ecgdata;              // ����ecg����   
    SemaphoreHandle_t  m_adjust_time_clear;         // ��ʱ�������
    SemaphoreHandle_t  m_ext_flash_opt;             // �ⲿFLASH����

    SemaphoreHandle_t  m_gsensor_restart;           // GSensor���¿�����ʼ���ź���
	SemaphoreHandle_t  m_gsensor_data_read;			// �ź�����ȡ���ٶ�����
	SemaphoreHandle_t  m_gsensor_stop;				// ���ٶ�ֹͣ
*/	
    SemaphoreHandle_t  m_task_count;                // �������������ź���
	
    TaskHandle_t       m_thread_handle;             //�߳̾��
}bthread_res_t;

// ���������̳߳�ʼ��
void BleThread_Init(void);

// PPG��
void BleThread_PpgOpen(void);

// ECG��
void BleThread_EcgOpen(void);

// Ѫ����
void BleThread_OxygenOpen(void);

// PPG+Ѫ����
void BleThread_PpgAndOxgenOpen(void);

// PTT��ʼ����
void BleThread_PttOpen(void);

// AFE4900�ص�����
void BleThread_PeripheralsDevClose(void);

// ���������豸����
void BleThread_ConnectDevice(void);

// �Ͽ��豸����
void BThread_DisconnectDevice(bool app_command_disconnect);

// �㲥��������
void BleThread_AdvertisingStart(void);

// �㲥ֹͣ����
void BleThread_AdvertisingStop(void);


// ����FLASH��������
void BleThread_PeriodCycleTask(uint8_t hour, uint8_t minute, uint8_t second);

// ��ʱ����FLASH����
void BleThread_AdjustTimeClearFlash(const system_clock_time_t* p_history_time);

// ����APP������Ϣ
void BleThread_AsynUpdateAppConfig(void);

// �ָ������������
void BleThread_FactoryClearData(void);

// ��ȡBAT���Ӿ���ṹ��ָ��
ble_nus_t* BleThread_GetBleNus(void);

// ��ȡCLK���Ӿ���ṹ��ָ��
ble_clk_t* BleThread_GetBleClk(void);

//��ʱ��������������㲥
void BleThread_RestartAdv(void);

void BleThread_UpdateBleNameConfig(void);
// ����
void BleThread_UpdateSitFuncConfig(void);

void BleThread_UpdateBpFuncConfig(void);

void BleThread_SaveEcgResultDataSem(void);

void BleThread_ExtFlashOperationSem(void);



// GSensor����
void BleThread_GsensorRestart(void);

// �ź�����ȡ���ٶ�����
void BleThread_GSensorDataRead(void);
// ���ٶ�����
void BleThread_GSensorStart(void);
// ���ٶ�ֹͣ
void BleThread_GSensorStop(void);



void Thread_GSensorRestartTask(void);
void Thread_ReadGSensorData(void);
void Thread_GSensorStop(void);


void Thread_FactoryClearDataTask(void);         
        // ����ʼ��
void Thread_InitZeroTimeTask(void);  
        // �����ճ���������
void Thread_SaveDailyDataTask(void);  
        // ��ʱ��������
void Thread_AdjustTimeClearDataTask(void);        
        // ����Ecg��������
void Thread_SaveEcgDataTask(void);        
        // �ⲿFLASH����
void Thread_ExtFlashOpt(void); 
void Thread_FactoryClearDataTask(void);
void Thread_SaveEcgDataTask(void);
void Thread_ExtFlashOpt(void);
    

#endif


