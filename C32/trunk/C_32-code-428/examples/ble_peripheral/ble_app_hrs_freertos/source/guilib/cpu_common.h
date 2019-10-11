/***************************Copyright(c)***********************************
**                       Veepoo Tech
**                  COPYRIGHT (C) 2016-2019
**-----------File Info-----------------------------------------------------
** File        name:     cpu_common.h
** Last modified Date:   2017-08-01
** Last SYS_VERSION:     0.0.0
** Descriptions:            
**-------------------------------------------------------------------------
** Created  by:          WJ
** Created  date:        2017-08-01
** SYS_VERSION:          0.0.0
** Descriptions:         
**-------------------------------------------------------------------------
** Modified by:				
** Modified date:  			          
** SYS_VERSION:                 
** Descriptions:                        
***************************************************************************/
#ifndef _CPU_COMMON_H_
#define _CPU_COMMON_H_

#include <stdint.h>
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

// ֪ͨ��Ϣ����
typedef enum
{
	NOTIFY_MSG_EVENT_CALL = 1,
	NOTIFY_MSG_EVENT_SMS,
	NOTIFY_MSG_EVENT_WECHAT,
	NOTIFY_MSG_EVENT_QQ,
	NOTIFY_MSG_EVENT_SINA,
	NOTIFY_MSG_EVENT_FACEBOOK,
	NOTIFY_MSG_EVENT_TWITTER,
	NOTIFY_MSG_EVENT_FLICKR,
	NOTIFY_MSG_EVENT_LINKEDLN,
	NOTIFY_MSG_EVENT_WHATSAPP,
	NOTIFY_MSG_EVENT_LINE,
	NOTIFY_MSG_EVENT_INSTAGRAM,
	NOTIFY_MSG_EVENT_SNAPCHAT,
	NOTIFY_MSG_EVENT_SKYPE,
	NOTIFY_MSG_EVENT_GMAIL,
	NOTIFY_MSG_EVENT_OTHER,
}NOTIFY_MSG_EVENT;

typedef enum
{
	BUTTON_EVENT_NONE = 1,
	BUTTON_EVENT_CLICK,
	BUTTON_EVENT_LONGPRESS,
    BUTTON_EVENT_LONGPRESS_6S,
}BUTTON_EVENT_TYPE;


// ����ö���Ͷ���
typedef enum
{
	CPU_TASK_IDEL = 0,  	  //����--����
	CPU_TASK_MAIN,            //����--ʱ��
    CPU_TASK_MAIN1,            //����--ʱ��
    CPU_TASK_PHY,
	CPU_TASK_MOVEMENT,        //����--�˶�ģʽ
	CPU_TASK_STEP,            //����--�ǲ� 
	CPU_TASK_DISTANCE,        //����--����
	CPU_TASK_CALORIES,        //����--��·��
//    CPU_TASK_BP,              //����--Ѫѹ
	CPU_TASK_HEART,           //����--����
	CPU_TASK_SLEEP,           //����--˯��
//	CPU_TASK_FINDPHONE,       //����--�����ֻ�
	CPU_TASK_STOP_WATCH,      //����--�����
//	CPU_TASK_COUNT_DOWN,      //����--����ʱ����
	CPU_TASK_POWEROFF,        //����--�ػ�
    
	CPU_TASK_MOVEMENT_PAUSE,  //����--�˶�ģʽ��ͣ   
	CPU_TASK_MOVEMENT_EXIT,   //����--�˶�ģʽ�˳�
    


    
}CPU_TASK_TYPE;

// �¼�ģʽö���Ͷ���
typedef enum
{
	CPU_EVENT_TASK_IDEL = 0,          // �¼�����--����
	CPU_EVENT_TASK_ALL_EXIT,          // �¼�����--�˳���������
	CPU_EVENT_TASK_CHARGE,            // �¼�����--���
	CPU_EVENT_TASK_ALARM,             // �¼�����--����
	CPU_EVENT_TASK_SIT,               // �¼�����--����
	CPU_EVENT_TASK_CALL_REMIND,       // �¼�����--��Ϣ
	CPU_EVENT_TASK_MESSAGE_REMIND,    // �¼�����--��Ϣ
	CPU_EVENT_TASK_HEARTALERT,        // �¼�����--���ʱ���
	CPU_EVENT_TASK_CAMERAREADY,       // �¼�����--����׼������
	CPU_EVENT_TASK_CAMERAOK,          // �¼�����--����׼��
	CPU_EVENT_TASK_FONTTEST,          // �¼�����--��ʾ��ά���ǡ�
	CPU_EVENT_TASK_LOWPOWER,          // �¼�����--�͵�����
	CPU_EVENT_TASK_COUNT_DOWN_TIMEOUT,// �¼�����--����ʱʱ�䵽��
	CPU_EVENT_TASK_COUNT_DOWN_START,  // �¼�����--����ʱ��ʼ
	CPU_EVENT_TASK_STARGET,           // �¼�����--�˶�Ŀ������
    CPU_EVENT_TASK_CARTOON,           // �¼�����--���ػ���������綯��
    CPU_EVENT_TASK_CARTOON1,
    CPU_EVENT_TASK_MAC_VER,           // �¼�����--��ʾMAC��VER
	CPU_EVENT_TASK_PHYS_REMIND,       // �¼�����--Ů�Թ�������
}CPU_EVENT_TYPE;

#pragma pack(1)


//OLED����������
typedef struct
{
	uint8_t oled_drive_bright:1;   // OLED�����ȵ���
	uint8_t oled_drive_light:1;    // OLED������
	uint8_t oled_drive_off:1;      // OLED��Ϩ��
	uint8_t oled_light_status:1;   // OLED����״̬
	
	uint8_t resv:4;                //
}oled_driver_t;

//��̬ҳ�������
typedef struct
{
	uint8_t animation_enable:1;      //����ʹ��
	uint8_t repaint_enable:1;        //ҳ��ʹ��
//	uint8_t repaint:1;               //ˢ��
//	uint8_t animation_max_frame:5;   //����������
//    
	uint8_t page_frame_counter;       //oledҳ��ˢ�¼�����
	uint8_t animation_frame_counter;  //����֡��������    

    
    uint8_t clear_background;      //�������        
    uint8_t clear_background_must; //�������
    uint8_t start_page;            //��ʼҳ(��)
    uint8_t page_size;             //�ܹ��ж���ҳ(��)
    uint8_t start_column;          //��ʼ��
    uint8_t column_size;           //�ܹ��ж�����
        
    uint16_t updata_ui_interval;   //UI ˢ�¼��
}gui_controller_t;

//CPU���������
typedef struct
{
	uint8_t active:1;              //���񼤻�
	uint8_t init:1;                //��ʼ��
	uint8_t key_press_hold:1;      //����������Ļ����
	uint8_t resv:5;
	
	uint8_t run_time;              //��������ʱ��
	uint8_t keep_time;             //���񱣳�ʱ��
    
    uint8_t priority;              //��������ȼ�
}cpu_task_controller_t;

#pragma pack()

/*
 * ������:JOB_TASK_ENTRY_INIT
 * ��  ��: 
 *   init--�����ʼ����ɺ���0
 *   active--��ʾ���񼤻�
 *   key_press_hold--��ʾ��������Ļ�Ƿ�����
 *   LDO_user_off--��ʾ��������ʱ,��Ҫ�ص�LDO��
 *   keep_time--��ʾ���񱣳�ʱ��
 *   run_time--��ʾ���񱣳�ʱ��
 *
 */
#define TASK_ENTRY_INIT(p_controller, hold_time) \
{ \
	p_controller->init = 0; \
	p_controller->active = 1; \
	p_controller->keep_time = hold_time; \
	p_controller->run_time = 0; \
    p_controller->key_press_hold = 0; \
}

/*
 *  ������:SEMAPHORE_CREATE_RET(sem)
 */
#define SEMAPHORE_CREATE_BINARY_RET(sem) \
{ \
	sem = xSemaphoreCreateBinary(); \
	if (NULL == sem) \
        APP_ERROR_HANDLER(NRF_ERROR_NO_MEM); \
}

/*
 *  ������:THREAD_MUTEX_TASK_EXCUTE_BREAK(task_sem,count_sem,task)
 *         task_sem--�����ź���
 *         count_sem--��������ź���
 *         task--������
 *  �� ��: �̻߳�������ִ��
 */
#define THREAD_MUTEX_TASK_EXCUTE(task_sem,count_sem,task) \
{ \
	if(pdTRUE == xSemaphoreTake(task_sem,(TickType_t)10)) \
	{  \
		xSemaphoreTake(count_sem,(TickType_t)0); \
		task(); \
	} \
}

/*
 *  ������:THREAD_TASK_WAKE_UP(task_sem,count_sem,thread_handle)
 *         task_sem--�����ź���
 *         count_sem--��������ź���
 *         thread_handle--�߳̾��
 *  �� ��: �߳�������
 */
#define THREAD_TASK_WAKE_UP(task_sem,count_sem,thread_handle) \
{ \
/*	if(1 == p_data_manage->sign_off)  return; */\
	                                          \
	xSemaphoreGive(task_sem); \
	xSemaphoreGive(count_sem); \
	if(eRunning != eTaskGetState(thread_handle)) \
	{ \
		vTaskResume(thread_handle); \
	} \
}

#define MEASURE_IDEL_STATUS() \
{  \
	*(uint8_t*)p_measure_opt == 0; \
}

#endif
