/***************************Copyright(c)***********************************
**                       Veepoo Tech
**                  COPYRIGHT (C) 2016-2019
**-----------File Info-----------------------------------------------------
** File        name:     gui_thread.h
** Last modified Date:   2017-07-31
** Last SYS_VERSION:     0.0.0
** Descriptions:            
**-------------------------------------------------------------------------
** Created  by:          WJ
** Created  date:        2017-07-31
** SYS_VERSION:          0.0.0
** Descriptions:         
**-------------------------------------------------------------------------
** Modified by:				
** Modified date:  			          
** SYS_VERSION:                 
** Descriptions:                        
***************************************************************************/
#ifndef _GUI_THREAD_H_
#define _GUI_THREAD_H_

#include <stdint.h>
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "cpu_common.h"



#pragma pack(1)
typedef struct
{
	SemaphoreHandle_t  m_repaint_sem;             //GUI�ػ��ź���(��ֵ)
	SemaphoreHandle_t  m_Task_count;              //ͨ��������������ź���
	TaskHandle_t       m_thread_handle;           //�߳̾��
}gui_thread_resource_t;

//��������ṹ��
typedef struct
{
	oled_driver_t          oled;         //OLED���Ʋ���
	gui_controller_t       gui;          //��̬ҳ�����
	
	CPU_EVENT_TYPE         event;        //�¼�
	CPU_TASK_TYPE          task;         //����
	
	cpu_task_controller_t  event_control;    //�˶��������
	cpu_task_controller_t  job_control;      //�������
}cpu_task_t;


typedef struct
{
    uint8_t button_id;
    uint8_t button_event;
}cpu_button_t;

#pragma pack()

extern uint8_t display_language;

extern uint8_t disconnet_keep_flg;
extern uint8_t disconnet_motor_flg;

//UI�̳߳�ʼ��
void Gui_ThreadInit(void);

// ����ʱ���ٹ���
void Gui_ThreadWatchTimerTrace(void);

bool Gui_ThreadPageActive(void);

bool Gui_ThreadTaskPageActive(void);

//�Ƿ����˶�����
bool Gui_ThreadIsTaskRun(void);

// GUI�߳���Ӱ����¼�
void Gui_ThreadAddKeyEvent(BUTTON_EVENT_TYPE event);

//GUI�߳�����ˢ��������  
void Gui_ThreadRepaintTaskWakeUp(void);

//������
void Gui_ThreadTaskWakeUp(CPU_TASK_TYPE task);

//�ⲿ�¼�����
void Gui_ThreadEventTaskWakeUp(CPU_EVENT_TYPE event);

// UI�߳����ȵ���������
void Gui_ThreadBrightAdjustTaskWakeUp(void);

//��̬����������0:�����¼����ʧ��, 1:�����¼���ӳɹ�
uint8_t Gui_ThreadPostureToBrightScreen(void);

//��̬����������0:�����¼����ʧ��, 1:�����¼���ӳɹ�
uint8_t Gui_ThreadPostureToCloseScreen(void);

//���õ�ǰ��ʾ���������´���ʾ����
void Gui_ThreadSetDisPlayArea(uint8_t start_column, uint8_t start_page, uint8_t column_size, uint8_t page_size);

//�ػ�UI����
void Gui_ThreadPowerOff(void);
//����UI����
void Gui_ThreadPowerOn(void);

//����ⲿ�¼�
void Gui_ThreadEventTaskClear(void);

//�����ǰ�¼��¼�
void Gui_ThreadTaskClear(void);

//�¼����ȼ������жϵ�ǰ�¼��Ƿ���Ӧ
uint8_t Gui_ThreadCanAcceptEvent(uint8_t event);

//����������
void Gui_ThreadUpdateStopwatch(void);

//����������
void Gui_ThreadUpdateMian(void);

//�Ƿ��ڲ����ֻ�����
uint8_t Gui_ThreadIsFindPhone(void);

//�������������
void Gui_ThreadTaskToIdle(void);

//��ȡ��ǰ����ID
uint8_t Gui_GetThreadTask(void);

//��ȡ��ǰ�¼�ID
uint8_t Gui_GetThreadJob(void);

void Gui_CheckEvent(void);

void Gui_SetChargingState(uint8_t state);
    
#endif
