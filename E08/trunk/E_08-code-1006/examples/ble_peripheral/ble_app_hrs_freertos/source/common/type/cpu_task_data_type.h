#ifndef _CPU_TASK_DATA_TYPE_H_
#define _CPU_TASK_DATA_TYPE_H_

#include <stdint.h>
#include "flash_data_type.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

typedef enum
{
    COMMUNICATION_THREAD_PRIORITY = 1,
    OXYGEN_ALGORITHM_THREAD_PRIORITY = 2,
    BLE_STACK_THREAD_PRIORITY = 3,
    GUI_THREAD_PRIORITY = 5,
}THREAD_PRIORITY;

typedef enum
{
    BUTTON_EVENT_NULL  = 0,
    BUTTON_EVENT_CLICK,
    BUTTON_EVENT_LONGPRESS,
}BUTTON_EVENT_TYPE;

// ����ö���Ͷ���
typedef enum
{
    CPU_TASK_IDEL = 0,        //����--����
    CPU_TASK_MAIN,            //����--������
    CPU_TASK_CHOOSE_MAIN,     //����--ѡ����������
    CPU_TASK_MAC,			  //����--MAC�Ͱ汾��
    CPU_TASK_MOVEMENT,        //����--�˶�ģʽ
    CPU_TASK_MOVEMENT_SELECT, //����--���˶�ѡ�����
    CPU_TASK_MOVEMENT_DATA,   //����--���ݽ���
    CPU_TASK_MOVEMENT_RESULT, //����--���˶���������
    CPU_TASK_STEP,            //����--�ǲ�����·����뼯��
	//CPU_TASK_DISTAN,          //����--����
	//CPU_TASK_CALOR,           //����--��·��
    CPU_TASK_HEART,           //����--����
    CPU_TASK_SLEEP,           //����--˯��
    CPU_TASK_FINDPHONE,       //����--�����ֻ�
    CPU_TASK_TIMER,           //����--�����
    CPU_TASK_HOURGLASS,       //����--����ʱ����

    CPU_TASK_ECG,             //����--ECGģʽ����    
    CPU_TASK_ECG_ITEM1,       //����--ECGģʽ����-�Ӳ˵�1
    CPU_TASK_ECG_ITEM2,       //����--ECGģʽ����-�Ӳ˵�2
    
    //CPU_TASK_PTT,             //����--PTTģʽ����
    //CPU_TASK_PTT_ITEM1,       //����--PTTģʽ����-�Ӳ˵�1

    CPU_TASK_BP,              //����--Ѫѹ����    
    CPU_TASK_OXYGEN,          //����--Ѫ������
    CPU_TASK_POWEROFF,        //����--�ػ�
    
    CPU_TASK_FUNCT,           //����--����
    CPU_TASK_FUNCT2,          //����--����
    CPU_TASK_SWITCH,          //����--�������ý���
    CPU_TASK_MESSAGE,         //����--��Ϣ����
    CPU_TASK_DISP_MESSAGE,    //����--��ʾ��Ϣ����
    CPU_TASK_DISP_MESSAGE2,    //����--��ʾ��Ϣ����2
    CPU_TASK_LIGHTLEVEL,      //����--���ȵ��ڽ���
    CPU_TASK_LIGHTTIME,       //����--����ʱ������
    CPU_TASK_CLEARDATA,       //����--������ݽ���
    CPU_TASK_MESSAGE_SWITCH,  //����--��Ϣ��������
    
    CPU_TASK_TASK_MAX_ID
    
}CPU_TASK_TYPE;

// �¼�ģʽö���Ͷ���
typedef enum
{
    CPU_EVENT_TASK_IDEL = 0,          // �¼�����--����
    CPU_EVENT_TASK_ALL_EXIT,          // �¼�����--�˳���������
    CPU_EVENT_TASK_TURNWRIST_START,   // �¼�����--��������
    CPU_EVENT_TASK_TURNWRIST_STOP,    // �¼�����--����ֹͣ
    CPU_EVENT_TASK_CHARGE,            // �¼�����--���
    CPU_EVENT_TASK_ALARM,             // �¼�����--����
    CPU_EVENT_TASK_SIT,               // �¼�����--����
    CPU_EVENT_TASK_CALL,              // �¼�����--��Ϣ
    //CPU_EVENT_TASK_PHYS_REMIND,       // �¼�����--Ů�Թ�������
    CPU_EVENT_TASK_MESSAGE,           // �¼�����--��Ϣ
    
    CPU_EVENT_TASK_LOW_SPO2,    // �¼�����--��������
    CPU_EVENT_TASK_HEARTALERT,        // �¼�����--���ʱ���
    
    CPU_EVENT_TASK_CAMERA,            // �¼�����--��������
    CPU_EVENT_TASK_FONTTEST,          // �¼�����--��ʾ��ά���ǡ�
    CPU_EVENT_TASK_LOWPOWER,          // �¼�����--�͵�����
    CPU_EVENT_TASK_SPORT_TARGET,      // �¼�����--�˶�Ŀ��ﵽ
    CPU_EVENT_TASK_BLE_DISC_REMIND,   // �¼�����--������������
    CPU_EVENT_TASK_HOURGLASS_HINT,    // �¼�����--����ʱAPP��������
    CPU_EVENT_TASK_HOURGLASS_TIMEOUT, // �¼�����--����ʱʱ�䵽��
    CPU_EVENT_TASK_POWERON,           // �¼�����--����
    CPU_EVENT_TASK_POWEROFF,          // �¼�����--�ػ�
    
}CPU_EVENT_TYPE;

// ����ܶ�ʱ��
typedef struct
{
    uint8_t   enable:1;         // ���ʹ�ܼ�ʱ
    uint8_t   entry:1;          // ������״̬(���ͼ�����)
    uint8_t   run:1;            // ����״̬(ʱ���������)
    uint8_t   resv:5;           // Ԥ������
    uint32_t  total_msecond;    // ����ʱ
    uint32_t  stop_msecond;     // ���к���ʱ��
}watch_timer_t;

//�˶�ģʽ����
typedef struct
{
    uint8_t   enable:1;         // �˶�ģʽʹ�ܼ�ʱ
    uint8_t   entry:1;          // �����˶�ģʽ
    uint8_t   run:1;            // ����״̬
    uint8_t   face_num:2;       // �ڼ�������
    uint8_t   resv:3;           // Ԥ������
}movement_cont_t;

////��������
//typedef struct
//{
//    uint8_t start_column;       //��ʼ��
//    uint8_t start_page;         //��ʼҳ(��)
//    uint8_t column_size;        //�ܹ��ж�����
//    uint8_t page_size;          //�ܹ��ж���ҳ(��)
//}gui_cls_param_t;

//OLED����������
typedef struct
{
    uint8_t   bright:1;    // OLED�����ȵ���
    uint8_t   light:1;     // OLED������
    uint8_t   status:1;    // OLED����״̬
    uint8_t   repaint:1;   // oledˢ����
    uint8_t   resv:4;      // ���뱣��
}oled_driver_t;

// �����������
typedef struct
{
    uint8_t   second_conter;            // �������	
}motor_driver_t;

//��̬ҳ�������
typedef struct
{
    uint8_t  animation_enable:1;         // ����ʹ��
    uint8_t  repaint_enable:1;           // ҳ��ʹ��
    uint8_t  resv:6;                     // ����������

    uint16_t  page_frame_counter;         // oledҳ��ˢ�¼�����
    uint16_t  animation_frame_index;      // ����֡���
    uint16_t  animation_tick_counter;     // ����tick������(tick��׼ʱ��Ϊ50ms) 
}gui_controller_t;

//CPU���������
typedef struct
{
    uint8_t  active:1;              // ���񼤻�
    uint8_t  init:1;                // ��ʼ��
    uint8_t  key_press_hold:1;      // ����������Ļ����
    uint8_t  resv:5;                // ��������
    
    uint16_t run_time;              // ��������ʱ��
    uint16_t keep_time;             // ���񱣳�ʱ��
}task_controller_t;

// CPU������ƿ�ṹ��
typedef struct
{
    CPU_TASK_TYPE       task_id;            // ��������ID
    uint8_t             child_task_id;      // ����ID
    task_controller_t   task_controller;
    gui_controller_t    gui_controller;	
}cpu_task_tcb_t;

// CPU������ƿ�ṹ��
typedef struct
{
    CPU_EVENT_TYPE      event_id;           // �¼�����ID  
    uint8_t             child_event_id;     // ����ID
    task_controller_t   task_controller;    // ������Ʋ���
    gui_controller_t    gui_controller;	    // GUI���Ʋ���
}cpu_event_tcb_t;

// CPU�豸���ƿ�ṹ��
typedef struct
{
    BUTTON_EVENT_TYPE   key;
    oled_driver_t       oled;
    motor_driver_t      motor;
    uint8_t             cls_flag;        //��Ҫ������    //0;��������  1:��ȫ�� 2:��С��
//	gui_cls_param_t     cls_param;       // ��������	
}cpu_device_tcb_t;

#define GET_TASK_PID(task_tcb,event_tcb) \
    (uint32_t)((((uint8_t)(task_tcb.task_id))<<24)|(((uint8_t)(task_tcb.child_task_id))<<16)|(((uint8_t)(event_tcb.event_id))<<8)|(event_tcb.child_event_id))

/*
 *  ������:SEMAPHORE_CREATE_RET(sem)
 *  һ���ź���ռ��76��Byte
 */
#define SEMAPHORE_CREATE_BINARY_RET(sem) \
{ \
    sem = xSemaphoreCreateBinary(); \
    if (sem == NULL) \
    { \
        APP_ERROR_HANDLER(NRF_ERROR_NO_MEM); \
    } \
}

/*
 *  ������:THREAD_MUTEX_TASK_EXCUTE_BREAK(task_sem,count_sem,task)
 *         task_sem--�����ź���
 *         count_sem--��������ź���
 *         task--������
*   �� ��: �̻߳�������ִ��
 */
#define THREAD_MUTEX_TASK_EXCUTE(task_sem,count_sem,task) \
{ \
    if(pdTRUE == xSemaphoreTake(task_sem,(TickType_t)0)) \
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
*   �� ��: �߳�������
 */
#define THREAD_TASK_WAKE_UP(task_sem,count_sem,thread_handle) \
{ \
    xSemaphoreGive(task_sem); \
    xSemaphoreGive(count_sem); \
    if(eRunning != eTaskGetState(thread_handle)) \
    { \
        vTaskResume(thread_handle); \
    } \
}


#endif
