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
#include "afe4404_data_type.h"
#include "common_data_type.h"
#include "event_priority.h"
#include "test_mode.h"

// ��Ϣ�������Խṹ��
typedef struct
{
    uint8_t                   unread_msg_count;      // δ����Ϣ����
    uint8_t                   update_index;          // ��Ϣ��������
    uint8_t                   display_len;           // ��ʾ����
    call_notify_content_t     call_content_s;        // �绰���� 
	message_notify_content_t  message_content_s[2];  // ��Ϣ��������
}app_notify_attr_t;

// Ů����Ŀ��Ϣ�ṹ��
typedef struct
{
    uint8_t                   unread_phys_count;                        // δ��Ů����Ϣ����
    uint8_t                   display_len;                              // ��ʾ����
    uint16_t                  data_len;                                 // ��Ϣ�ĳ���
	uint8_t                   content[1];             // Ů����Ϣ��������
}phys_notify_attr_t;

// Ѫѹ
typedef struct
{
    uint8_t cache_bp_sp;
    uint8_t cache_bp_dp;    
}bp_cache_t;


// ��������ṹ��
typedef struct
{
    uint8_t     clear_data:1;       // �������
    uint8_t     clear_config:1;     // �������
    uint8_t     clear_res:6;        // ����      
}app_factory_clear_t;


//���ݹ���ṹ��
typedef struct
{
    uint8_t                      oled_bright;                 // OLED��������
    uint8_t                      alarm_scence;                // ���峡��ID
    uint8_t                      turnwrist_camera_state;      // ��������״̬
    uint32_t                     sedentary_cnt;               // ������ʱ
    system_clock_time_t          history_time;                // ϵͳ��ʷʱ��
    event_queue_t                event_list;                  // �¼��б�
    trace_message_t              trace_message_s;             // ��������ʱ��
    device_trace_param_t         device_trace_param_s;        // ������Ϣ
    app_upload_control_t         app_upload_control_s;        // �ϴ����Ʋ���	
    acc_data_param_t             acc;                         // ���ٶȱ���
    UPLOAD_ADC_EVENT             upload;
    app_notify_attr_t            app_notify_s;                // ��Ϣ���ͳ�Ա���� 
    phys_notify_attr_t           phys_notify_s;               // Ů����Ŀ��Ϣ����
    log_data_attr_t              log_attr_s;                  // ��־����
    daily_data_attr_t            daily_attr;                  // �ճ�����
    blood_oxygen_attr_t          oxygen_attr;                 // Ѫ������
    uint8_t                      oxygen_resut[5];             // ÿ����Ѫ��ƽ��ֵ       
    history_pedometer_param      pedometer;                   // �ǲ�����
    fatigue_measure_t            fatigue_measure_s;           // ƣ�ͶȲ���
    blood_oxygen_measure_t       blood_oxygen_measure_s;      // Ѫ������
    heart_measure_t              heart_measure_s;             // ���ʲ���
    bp_measure_t                 bp_measure_s;                // Ѫѹ����          
    hrv_measure_t                hrv_measure_s;               // HRV��������
    heart_load_param_t           heart_load_param_s;          // ���ฺ�ɲ���
    scatter_heart_param_t        scatter_heart_param_s;       // ɢ��ͼ���ʲ���
    respiratory_rate_measure_t   respiratory_rate_s;          // �����ʲ���
    breath_stop_measure_t        breath_stop_s;               // ������ͣ
    wear_measure_t               wear_measure_s;              // �������
    no_wear_measure_t            no_wear_measure_s;           // ���ּ���㷨
    afe4404_data_pool            data_pool;                   // AFE4404���ݻ����
    
    bp_cache_t                   bp_cache_s;
    uint8_t                      ox_cache_s;

    app_factory_clear_t          app_factory_clear_s;           // �������
	test_ox_info_t               test_ox_info_s;              //�ռ��Զ���������   

}data_manage_param;

extern data_manage_param*  p_data_manage;

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
daily_data_attr_t* DM_Get_NewestDailyData(void);

/*************************************************
 *           �ǲ���ؽӿ�
 *************************************************/

// ��ȡ�ֻ��˼ǲ�ֵ
uint32_t DataManage_GetCurrentStepCnt(void);

/*************************************************
 *           OLED������ؽӿ�
 *************************************************/
 void DM_Update_Brightness(uint8_t brightness);

/*************************************************
 *           AFE4404��ؽӿ�
 *************************************************/
 // AFE4404��������ݳ�ʼ��
void DataManage_AFE4404PoolInit(void);

//  ��ȡ����������ݳ�
const int32_t* DataManage_GetRedPoolData(void);

//  ��ȡ�������ݳ�
const int32_t* DataManage_GetiRedDataPool(void);

////  ��ȡHRV�������ݳ�(��Ҫ15S����)
//const uint32_t* DataManage_GetRespiRedDataPool(void);

//  ��ȡ�̹����ݳ�(4S)
const int32_t* DataManage_GetTiredGreenPool(void);

//  ��ȡ�̹����ݳ�(4S)
const int32_t* DataManage_GetGreenCompletePool(void);

// ��ȡ�����ʵ��̹�����
const int32_t* DataManage_GetRespGreenDataPool(void);

// ��ȡ����̹����ݳ�
const int32_t* DataManage_GetWearGreenDataPool(void);

//  ��ȡ���������ݳ�
const int32_t* DataManage_GetAmbientDataPool(void);


void DataManage_UpdateBloodOxygenADCData(int32_t red, int32_t ired, int32_t green, int32_t ambient);

void DataManage_UpdateHeartADCData( int32_t green, int32_t ambient);


void DataManage_CopyBloodOxygenCacheData(void);

void DataManage_CopyHeartCacheData(void);


// ��ȡѪ��ADC����
void DataManage_GetOxygenADCData(uint8_t *p_green, uint8_t *p_red, uint8_t *p_ired, uint8_t *p_ambient); 

// ��ȡ����ADC����
void DataManage_GetHeartADCData(uint8_t *p_green, uint8_t *p_ambient);

void DataManage_ProcessOxygenAttrData(void);

#endif
