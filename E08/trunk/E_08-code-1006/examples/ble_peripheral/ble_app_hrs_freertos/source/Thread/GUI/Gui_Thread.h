/******************************************************************************
 * Copyright (c) 2017 - 2018, Veepoo
 * 
 * All rights reserved.
 * 
** �ļ�����: gui_thread.h
** ����ʱ��: 2017-06-07
** �� �� ��: GT
** ��    ��: �ļ��ṩUI�̵߳Ŀ��ƺͽӿ����
**
*******************************************************************************/
#ifndef _GUI_THREAD_H_
#define _GUI_THREAD_H_

#include <stdint.h>
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "cpu_task_data_type.h"

typedef struct
{    
    uint8_t            init_complete;             //�̳߳�ʼ����־
    uint8_t            sleep_flag;                //�߳��Ƿ��ź�������
    uint8_t            sleep_time;                //�߳���������ʱ��
    SemaphoreHandle_t  m_thread_sleep;            // UI�߳�����
    
    SemaphoreHandle_t  m_gsensor_restart;           // GSensor���¿�����ʼ���ź���
	SemaphoreHandle_t  m_gsensor_data_read;			// �ź�����ȡ���ٶ�����
	SemaphoreHandle_t  m_gsensor_stop;				// ���ٶ�ֹͣ  
    
    SemaphoreHandle_t  m_factory_clear;             // �ָ���������
    SemaphoreHandle_t  m_init_zerotime;             // 0��ʱ�̳�ʼ��    
    SemaphoreHandle_t  m_save_dailydata;            // �������������    
    SemaphoreHandle_t  m_save_ecgdata;              // ����ecg����   
    SemaphoreHandle_t  m_adjust_time_clear;         // ��ʱ�������
    SemaphoreHandle_t  m_ext_flash_opt;             // �ⲿFLASH����
    SemaphoreHandle_t  m_movement_opt;              // �˶�ģʽ����

    
    SemaphoreHandle_t  m_task_count;              // �������������ź���
    TaskHandle_t       m_thread_handle;           // �߳̾��
}gui_resource_t;

//��������ṹ��
typedef struct
{
    uint32_t          pid;             // ������ǰGUI��ʵʱID
    watch_timer_t     watch;           // ���
    movement_cont_t   movement;        // �˶�ģʽ
    cpu_task_tcb_t    task_tcb;
    cpu_event_tcb_t   event_tcb;	
    cpu_device_tcb_t  device_tcb;
}cpu_param_t;



typedef struct
{
    uint8_t oled_bright_flg:1;          //���ȵ���
    uint8_t display_time_flg:1;         //��ʾʱ��
    uint8_t dev_config_flg:1;           //��������  //0xB8
    uint8_t turnwrist_flg:1;            //��������
    uint8_t sedentary_flg:1;            //��������
    uint8_t heart_alert_flg:1;          //���ʱ���
    uint8_t message_flg:1;              //��Ϣ����
    uint8_t oxygen_flg:1;              //Ѫ���Զ�����

    uint8_t alarm_flg:1;               //��������
    //uint8_t wear_flg:1;                 //������
    uint8_t resv:7;
}switch_flg_t;      //�ֻ�����������һЩ״̬��־

typedef union
{
    uint16_t        switch_data;
    switch_flg_t    switch_flg;
}upload_switch_t;       //�ϴ��ֻ��ı��һЩ״̬��APP




extern uint8_t display_language;
extern uint8_t gui_display_time;       //UI������ʾʱ�������ֽ��治������
extern upload_switch_t     upload_switch_s;

// GUI�̳߳�ʼ��
void GuiThread_Init(void);

// GUI�߳�ҳ���Ƿ񼤻�
bool GuiThread_PageActive(void);

// GUI�߳���Ӱ����¼�
void GuiThread_AddKeyEvent(BUTTON_EVENT_TYPE event);

// GUI�߳�����¼�
uint8_t GuiThread_AddEvent(CPU_EVENT_TYPE event);

// ����¼�
void GuiThread_ClearEvent(CPU_EVENT_TYPE event);

// �豸�Ƿ����ڲ����ֻ�
bool GuiThread_IsDuringFindPhone(void);

// GUI�̻߳�ȡ��������
CPU_TASK_TYPE GuiThread_GetTaskType(void);

// GUI�̻߳�ȡ�¼�����
CPU_EVENT_TYPE GuiThread_GetEventType(void);

// GUI�̻߳�ȡ���ṹ��ָ��
const watch_timer_t* GuiThread_GetWatchTimer(void);

uint8_t GuiThread_TimerIsBusy(void);

// UI�߳����ȵ���������
void GuiThread_BrightAdjust(void);

// GUI�̳߳���¼�����
void GuiThread_ChargingCommit(void);

// GUI�̳߳��������ύ
void GuiThread_ChargFullCommit(void);

// GUI�߳�������в���
void GuiThread_ClearAllParam(void);

// ��ȥ������
void GuiThread_InMainTask(void);

void GuiThread_ClearMovementParm(void);

//�����˶�ģʽ
void GuiThread_InMovementTask(void);

//���õ�ǰ��ʾ���������´���ʾ����
void Gui_ThreadSetDisPlayArea(uint8_t start_column, uint8_t start_page, uint8_t column_size, uint8_t page_size);

//����������
void Gui_ThreadUpdateMain(void);

//��������һЩ����
void GuiThread_ClearTaskParam(void);


//spi ��������ʱUI�߳�����
void GuiThread_SpiSleep(void);

//spi �����������ʱUI�̻߳���
void GuiThread_SpiWeakUp(void);

void Gui_CheckEvent(void);

void Gui_ThreadBrightAdjustTaskWakeUp(void);

uint8_t Button_GetPressEvent(void);

// �˶�ģʽ-���ݴ洢�ź���
void Gui_ThreadWakeupMovementOpt(void);
    
#endif
