/******************************************************************************
 * Copyright (c) 2017 - 2018, Veepoo
 * 
 * All rights reserved.
 * 
** �ļ�����: Thread_DataManage.h
** ����ʱ��: 2017-06-06
** �� �� ��: GT
** ��    ��: �ļ��ṩ�߳�֮��Ĺ������ݹ���Ļ����ӿ�,�ӿ���������:
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

// ��Ϣ�������Խṹ��
typedef struct
{
    uint8_t                   unread_msg_count;      // δ����Ϣ����
    uint8_t                   update_index;          // ��Ϣ��������
    uint8_t                   display_len;           // ��ʾ����
    call_notify_content_t     call_content_s;        // �绰���� 
	message_notify_content_t  message_content_s[2];  // ��Ϣ��������
}app_notify_attr_t;

// �㷨�������
typedef struct
{
	uint8_t    sleep_state[6];          //˯��״̬
    uint8_t    sleep_effective;         //˯��״̬�Ƿ���Ч
    uint8_t    pulse_frequency[5];      //����
    uint8_t    heart_rate[5];           //����
    uint8_t    breathing_rate[5];       //������
    
    uint8_t    hrv_type;
    uint8_t    hrv_value[50];
    uint8_t    bp_high;                 //��ѹ
    uint8_t    bp_low;                  //��ѹ
  
    uint8_t    cache_cardiac_load[5];       //���ฺ��
    uint8_t    cache_oxgen_value[5];        //Ѫ��
    
    uint8_t    cache_breath_stop_cnt;       //������ͣ����������һ����������0
    uint8_t    cache_osahs_degree;          //������ͣ���������һ����������0
	uint8_t    cache_low_spo2_time;         //����(ȱ��)ʱ�䣬����һ����������0    
    uint8_t    cache_spo2_modification_flag;   //5����Ѫ��������־λ�����һ����ֵ
    
}algorithm_result_cache_t;



// ��������ṹ��
typedef struct
{
    uint8_t     clear_data:1;       // �������
    uint8_t     clear_config:1;     // �������
    uint8_t     clear_res:6;        // ����      
}app_factory_clear_t;

// ��ȡ˯�߽��
typedef struct
{
    uint16_t     sleep_all_time;         // ˯����ʱ��
    uint16_t     sleep_shallow_time;     // ǳ˯
    uint16_t     sleep_deep_time;        // ��˯     
    uint8_t      sleep_quality;          // ˯������
}sleep_result_t;




//���ݹ���ṹ��
typedef struct
{
    uint8_t                      oled_bright;                 // OLED��������
    uint8_t                      alarm_scence;                // ���峡��ID
    uint8_t                      turnwrist_camera_state;      // ��������״̬
    uint32_t                     sedentary_cnt;               // ������ʱ
    system_clock_time_t          history_time;                // ϵͳ��ʷʱ��
    event_queue_t                event_list;                  // �¼��б�
    device_trace_param_t         device_trace_param_s;        // ������Ϣ
    app_upload_control_t         app_upload_control_s;        // �ϴ����Ʋ���	
    acc_data_param_t             acc;                         // ���ٶȱ���
    UPLOAD_ADC_EVENT             upload;
    app_notify_attr_t            app_notify_s;                // ��Ϣ���ͳ�Ա���� 
	algorithm_result_cache_t	 algorithm_result_cache;	  // �㷨�������
    daily_data_attr_t            daily_attr;                  // �ճ�����
    history_pedometer_param      pedometer;                   // �ǲ�����
    ecg_ppg_time_t               ecg_ppg_time_s;              // �㷨����ʱ�䴦��
    ecg_ppg_measure_t            ecg_ppg_s;                   // ecg\ppg����
    
    wear_measure_t               wear_measure_s;              // �������
    no_wear_measure_t            no_wear_measure_s;           // ���ּ���㷨
    si1182_data_pool_t			 data_pool;                   // AFE4900���ݻ����
	raw_data_control_t           raw_data_control;            // ԭʼ���ݿ��Ʋ���
    
    ecg_result_data_t            ecg_result_data_s;           // ECG ����������� 
    
    ext_flash_ecg_opt_t          ext_flash_ecg_opt_s;         // ECG�쳣���ݲ���  
    ext_flash_movement_opt_t     ext_flash_movement_opt_s;    // �˶�ģʽ���ݲ���
    ext_flash_ecg_rest_opt_t     ext_flash_ecg_rest_opt_s;    // ECG������ݲ���
    ext_flash_daily_opt_t        ext_flash_daily_opt_s;       // �ճ����ݲ���
    ext_flash_sleep_opt_t        ext_flash_sleep_opt_s;       // ˯�����ݲ���
    ext_flash_head_opt_t         ext_flash_head_opt_s;        // �ⲿFLASHͷ����Ϣ
    ext_flash_all_opt_t          ext_flash_all_opt_s;         // �ⲿFLASH����
    
    uint8_t                      app_bp_mode;                 // ����app����ģʽ
    app_factory_clear_t          app_factory_clear_s;         // �������
    //test
    log_data_attr_t              log_attr_s;                  // ��־����
    //trace_message_t              trace_message_s;             // ��������ʱ��
    uint8_t                      ppg_adc_mode;                  // ppg�Զ����������ռ�
    power_statistics_t           power_statistics_s;            // ����ͳ��
    sleep_result_t               sleep_result_s;                // ˯�߽��
    uint8_t                      sleep_flag;                    // ˯��״̬
}data_manage_param;

extern data_manage_param    *p_data_manage;

// ���ݹ����ʼ��
void DataManage_Init(void);

// ���ݸ���GSensor FIFO����
void DataManage_UpdateFIFOData(int16_t* p_fifo,uint8_t len);

// ������̬����
void DataManage_ProcessGestureData(void);

// ����ǲ�����
void DataManage_ProcessPedometerData(int16_t* p_xdata,int16_t* p_ydata,int16_t* p_zdata);

void DataManage_GetRTAccData(int16_t* p_data);

void DataManage_GetGsensorADCData(int16_t*  p_data);

void DataManage_GetGestureData(int16_t* p_data);

void DataManage_AccData_Send_Reset(void);

void DM_Update_DailyHeartData(uint8_t heart);

// ��ȡ������ճ�����
daily_data_attr_t* DataManage_NewestDailyData(void);

// ��������������������
void DataManage_InitDailyData(void);

/*************************************************
 *           �ǲ���ؽӿ�
 *************************************************/

// ��ȡ�ֻ��˼ǲ�ֵ
uint32_t DataManage_GetCurrentStepCnt(void);

/*************************************************
 *           OLED������ؽӿ�
 *************************************************/
void DM_Update_Brightness(uint8_t brightness);


////////////////////////////////////
//         SI1182 �����ӿ�        //
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
