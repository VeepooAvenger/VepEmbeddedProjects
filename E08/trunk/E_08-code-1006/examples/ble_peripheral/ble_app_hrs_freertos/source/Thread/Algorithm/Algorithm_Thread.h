/******************************************************************************
 * Copyright (c) 2017 - 2018, Veepoo
 * 
 * All rights reserved.
 * 
** �ļ�����: algorithm_thread.h
** ����ʱ��: 2017-06-02
** �� �� ��: GT
** ��    ��: �ļ����ϵͳ�㷨������,������������:
**           1.���Ѫ�������㷨;
**           2.���˯��ʱ������㷨;
**           3.�������������㷨;
**           4.��ɼǲ������㷨;
**           5.�����̬�����㷨;
**
*******************************************************************************/
#ifndef _ALGORITHM_THREAD_H_
#define _ALGORITHM_THREAD_H_

#include <stdbool.h>
#include "flash_data_type.h"
#include "cpu_task_data_type.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "data_manage.h"

//-----------------------------------
//        �㷨�߳̽ṹ��
//-----------------------------------
typedef struct
{
    SemaphoreHandle_t  m_battery_sem;               // ��ص��������ź���
    SemaphoreHandle_t  m_hourglass_sem;             // ����ʱ�ź���
    SemaphoreHandle_t  m_sedentary_sem;             // ���������ź���
    SemaphoreHandle_t  m_step_sport_trace_sem;      // �ǲ��˶��������ź���
    SemaphoreHandle_t  m_alarm_sem;                 // ��������ź���
    SemaphoreHandle_t  m_brightness_sem;            // ���ȼ����ź���
    
    SemaphoreHandle_t  m_ecg_measure_sem;           // ECG�㷨�ź���
    SemaphoreHandle_t  m_ppg_sem;                   // PPG�㷨�ź���

    SemaphoreHandle_t  m_wear_sem;                  // ����㷨�ź���
    SemaphoreHandle_t  m_no_wear_sem;               // ��������㷨�ź���
    
    SemaphoreHandle_t  m_pedometer_sem;             // �ǲ��㷨�ź���
    SemaphoreHandle_t  m_gesture_sem;               // ��̬�㷨�ź���
    //SemaphoreHandle_t  m_fatigue_sem;               // ƣ�Ͷ��㷨�ź���
    SemaphoreHandle_t  m_sleep_sem;                 // ˯���㷨�ź���
    SemaphoreHandle_t  m_sleep_traversing_sem;      // ˯���㷨�ź���
    SemaphoreHandle_t  m_task_count;                // �������������ź���
    SemaphoreHandle_t  m_blood_oxygen_measure_sem;  // Ѫ���㷨�ź���
    SemaphoreHandle_t  m_osahs_sem;                 // ������ͣ�㷨�ź���
    SemaphoreHandle_t  m_one_seconds_sem;           // һ��ʱ�䣬�������¼��㷨
    SemaphoreHandle_t  m_exit_sleep_sem;            // ǿ���˳�˯��
    
    
    TaskHandle_t       m_thread_handle;             // �߳̾��
}algorithm_thread_resource_t;

// �㷨�̳߳�ʼ��
void AlgThread_Init(void);

// �㷨�ź�������
void AlgThread_CalcSignal(void);

// �㷨�߳�����˯�߱�������
void AlgThread_SleepTraversing(void);

// GSensor�����ύ
void AlgThread_GsensorEventCommit(void);
	
// �㷨�߳�����������
void AlgThread_PeriodCycleTask(uint8_t hour, uint8_t minute, uint8_t second);

void Algorithm_AlarmValidCheck(void);

uint8_t Alarm_GetValidConfigNum(void);

void Algorithm_OledBrightnessAdjustTask(void);

void AlgThread_Osahs(void);

// ����ǿ���˳�˯���ź���
void AlgThread_ExitSleepSem(void);

#endif
