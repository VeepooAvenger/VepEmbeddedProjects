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
    SemaphoreHandle_t  m_oxygen_data_sem;         // ����Ѫ�������ź���(��ֵ)
    SemaphoreHandle_t  m_scatter_heart_sem;       // ����ɢ��ͼ���������ź���(��ֵ)
    SemaphoreHandle_t  m_alarm_data_sem;          // �������������ź���(��ֵ)
    SemaphoreHandle_t  m_movement_sem;            // �����˶�ģʽ�����ź���(��ֵ)
    SemaphoreHandle_t  m_heart_app;               // ��������ʵʱ�����ź���(��ֵ)
    SemaphoreHandle_t  m_oxygen_app;              // ����Ѫ��ʵʱ�����ź���(��ֵ)
    SemaphoreHandle_t  m_fatigue_app;             // ����ƣ�Ͷ�ʵʱ�����ź���(��ֵ)
    SemaphoreHandle_t  m_resp_app;                // ����ƣ�Ͷ�ʵʱ�����ź���(��ֵ)
    SemaphoreHandle_t  m_hourglass_app;           // ���͵���ʱʵʱ�����ź���(��ֵ)
    SemaphoreHandle_t  m_bp_app;                  // ����Ѫѹʵʱ�����ź���(��ֵ)
        
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
    uint16_t  last_index;   // ��¼�ϴ�����λ�ã�
}daily_upload_param_t;

// ˯�������ϴ����Ʋ���
typedef struct
{
    uint8_t  total_package_cnt;  //�ܰ���
    uint8_t  sleep_wake_cnt;     //˯�����Ѵ���
    uint8_t  day_id;             //˯������ID
}sleep_upload_param_t;

typedef struct
{
    uint8_t  send_data[80];         // ��������
    sleep_upload_param_t  sleep;    // ˯���ϴ�����
    daily_upload_param_t  daily;    // �ճ������ϴ�����
    daily_upload_param_t  oxygen;   // Ѫ�������ϴ�����
    daily_upload_param_t  sheart;   // HRVɢ��ͼ�ϴ�����
    daily_upload_param_t  movement; // �˶�ģʽ�ϴ�����
}com_upload_param_t;

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

// ����HRVɢ��ͼ���ʴ洢����
void ComThread_SendScatterHeartData(uint8_t* p_data);

//�����˶�ģʽ����
void ComThread_SendMovementData(void);

// ͨ���̻߳��ѷ���˯����������(ȫ��)
void ComThread_SendSleepData(uint8_t* p_data);

// ͨ���̻߳��ѷ���Ѫ����������(ȫ��)
void ComThread_SendOxygenData(uint8_t* p_data);

// �����������ݵ�APP��(���20��)
void ComThread_SendAlarmData(void);

// ͨ���߳�����������
void ComThread_PeriodCycleTask(uint8_t hour, uint8_t minute, uint8_t second);

#endif
