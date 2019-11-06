/******************************************************************************
 * Copyright (c) 2017 - 2018, Veepoo
 * 
 * All rights reserved.
 * 
** �ļ�����: communication_thread.h
** ����ʱ��: 2017-06-06
** �� �� ��: GT
** ��    ��: �ļ��ṩ��APPͨ���̵߳Ļ������ܽӿڼ������Ӧ������,������������:
**           1.�����������ݵķ���;
**           2.���Ѫ�����ݵķ���;
**           3.���˯�����ݵķ���;
**           4.�������ʵʱ���ݵķ���;
**           5.���Ѫ��ʵʱ���ݵķ���;
**           6.���ƣ�Ͷ�ʵʱ���ݵķ���;
**
*******************************************************************************/
#ifndef _COMMUNICATION_THREAD_H_
#define _COMMUNICATION_THREAD_H_

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#pragma pack (1)
#pragma anon_unions //keil��Ĭ���ǲ�֧�������ṹ��ģ���Ҫ����ָ��#pragma anon_unionsָ����
 
// ͨ���߳���Դ����ṹ��
typedef struct
{
    SemaphoreHandle_t  m_upload_adc_sem;          // �ǲ�����ʵʱ�����ź���(��ֵ)
    SemaphoreHandle_t  m_all_flash_sem;           // FLASH���������ݷ����ź���(��ֵ)
    SemaphoreHandle_t  m_gensor_data_sem;         // ��������ʵʱ�����ź���(��ֵ)
    SemaphoreHandle_t  m_pedometer_data_sem;      // �ǲ�����ʵʱ�����ź���(��ֵ)
    SemaphoreHandle_t  m_wechat_data_sem;         // ����΢�������ź���(��ֵ)
    
    SemaphoreHandle_t  m_daily_data_sem;          // ��������������ź���(��ֵ)
    SemaphoreHandle_t  m_sleep_data_sem;          // ��������������ź���(��ֵ)
    
    SemaphoreHandle_t  m_alarm_data_sem;          // �������������ź���(��ֵ)
    SemaphoreHandle_t  m_movement_sem;            // �����˶�ģʽ�����ź���(��ֵ)
    
    SemaphoreHandle_t  m_heart_app;               // ��������ʵʱ�����ź���(��ֵ)
    SemaphoreHandle_t  m_ecg_app;                 // ����ECGʵʱ�����ź���(��ֵ)
    SemaphoreHandle_t  m_bp_app;                  // ����Ѫѹʵʱ�����ź���(��ֵ)
    SemaphoreHandle_t  m_oxygen_app;              // ����Ѫ��ʵʱ�����ź���(��ֵ)
    
    SemaphoreHandle_t  m_hourglass_app;           // ���͵���ʱʵʱ�����ź���(��ֵ)
    
    SemaphoreHandle_t  m_ecg_data_sem;            // ����ecg����洢�����ź���(��ֵ)
    
    SemaphoreHandle_t  m_power_info_sem;          // ���͹���ͳ�ƴ洢�����ź���(��ֵ)
    SemaphoreHandle_t  m_update_set_sem;          // ���������޸�
        
    SemaphoreHandle_t  m_task_count;              // ͨ��������������ź���

    TaskHandle_t       m_thread_handle;           // �߳̾��
}communication_resource_t;


// �ճ������ϴ����Ʋ���
typedef struct
{
    uint16_t  start;      // ��ʼ����
    uint16_t  total;      // �ܰ���
    uint16_t  split;      // �ָ����
    uint8_t   day_id;     // ���
    uint8_t   data_type;  // ��������
}daily_upload_param_t;

// ˯�������ϴ����Ʋ���
typedef struct
{
    uint8_t  total_package_cnt;  //�ܰ���
    uint8_t  sleep_wake_cnt;     //˯�����Ѵ���
    uint8_t  day_id;             //˯������ID
}sleep_upload_param_t;


// ECG �ֶ�������������ϴ����Ʋ���
typedef struct
{
    uint8_t    id_flag;         //�Ƿ��ȡid, id�����������Ϊ0
    uint16_t   year;            //��
    uint8_t    month;           //��
    uint8_t    day;             //��
    uint8_t    hour;            //ʱ
    uint8_t    minute;          //��
    uint8_t    second;          //��
    
    uint8_t    id_type;         //��Ҫ���͵�����
    uint16_t   send_id;         //��Ҫ���͵�����id
}ecg_upload_param_t;


typedef struct
{
    uint8_t                 send_data[80];         // ��������
    sleep_upload_param_t    sleep;    // ˯���ϴ�����
    daily_upload_param_t    daily;    // �ճ������ϴ�����
    daily_upload_param_t    movement; // �˶�ģʽ�ϴ�����
    
    ecg_upload_param_t      ecg;      // ecg �ֶ���������ϴ�����
    
    uint8_t                 power_info_day; //��������Ĺ���ͳ������
    
}com_upload_param_t;

#pragma pack ()

// ͨ���̳߳�ʼ��
void ComThread_Init(void);

//��ȡ����FLASHԭʼ���ݵ�״̬
uint8_t ComThread_GetSendFlashDataState(void);
    
// ����ADC����
void ComThread_SendADCData(void);

// ����FLASH����
void ComThread_SendAllFlashData(void);

// ��������Ӵ洢����
void ComThread_SendDailyData(uint8_t* p_data);

//�����˶�ģʽ����
void ComThread_SendMovementData(uint8_t data_type, uint8_t movement_block);

// ͨ���̻߳��ѷ���˯����������(ȫ��)
void ComThread_SendSleepData(uint8_t* p_data);

// �����������ݵ�APP��(���20��)
void ComThread_SendAlarmData(void);

// ͨ���߳�����������
void ComThread_PeriodCycleTask(uint8_t hour, uint8_t minute, uint8_t second);

// ����ECG ID���� 
void ComThread_SendEcgIdData(uint8_t id_flag, uint8_t *time_data);

// ����ECG ID���� 
void ComThread_SendEcgDataById(uint8_t id_type, uint16_t id_num, uint8_t *time_data);

// ���͹���ͳ������
void ComThread_SendPowerInfoData(uint8_t day_num);

void ComThread_UpdateSetParamSem(void);

#endif
