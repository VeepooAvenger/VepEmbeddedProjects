/***************************Copyright(c)***********************************
**                       Veepoo Tech
**                  COPYRIGHT (C) 2016-2019
**-----------File Info-----------------------------------------------------
** File        name:     alarm_event.h
** Last modified Date:   2017-08-24
** Last SYS_VERSION:     0.0.0
** Descriptions:            
**-------------------------------------------------------------------------
** Created  by:          WJ
** Created  date:        2017-08-24
** SYS_VERSION:          0.0.0
** Descriptions:         
**-------------------------------------------------------------------------
** Modified by:				
** Modified date:  			          
** SYS_VERSION:                 
** Descriptions:                        
***************************************************************************/

#ifndef _ALARM_CLOCK_EVENT_H_
#define _ALARM_CLOCK_EVENT_H_

#include <stdint.h>
#include <stdbool.h>

#define COUNTDOWN_MAX_NUM       1

typedef enum
{
    STOPWATCH_IDE,          //׼��
    STOPWATCH_READY,        //����
    STOPWATCH_START,        //��ʼ
    STOPWATCH_STOP,         //ֹͣ
}stopwatch_e;

typedef enum
{
    COUNTDOWN_STATE_IDE,          //׼��
    COUNTDOWN_STATE_RUN,        //����
    COUNTDOWN_STATE_PAUSE,        //��ͣ
}COUNTDOWN_STATE_E;

#pragma pack(1)

typedef struct{   
    uint8_t id;                 
    uint8_t state;               //����ʱ״̬    
    uint8_t type;                //����ʱ����
    uint32_t countdown_second;   //��ʱ�� 
}countdown_config_t;             //����ʱ


typedef struct{ 
    uint8_t stopwatch_100millisecond;
    stopwatch_e state;   
    uint8_t init_start;
    uint16_t total_second;
}stopwatch_config_t;             //���

#pragma pack()

//��������ʱ��100���붨ʱ���¼�
void Watch_100msEvent(void);

//����ʼ��,(�ػ�����������)
void Watch_Init(void);

//���Ӿ�����ֹͣ״̬����
void Watch_Start(void);

//ֹͣ���,����ֹͣ��������ʱ��������
void Watch_Stop(void);

//����Ѿ�׼������,����æµ״̬
uint8_t Watch_IsBusy(void);

//��ȡ���ĵ�ǰ״̬
stopwatch_e Watch_GetState(void);

//��ȡ����������,���ڣգ���ʾ
uint16_t Watch_GetTotalSecond(void);

//��ȡ���ǰ��100������
uint8_t Watch_Get100Millisecond(void);

//�������Ϊ����״̬,(3s����,��ʼ����ɣ����������Ϊ����״̬)
void Watch_SetReady(void);

//��ͣ���������,����״̬�¶̰������л�����
void Watch_Toggle(void);

//����Ѿ���ʼ�������ת
uint8_t Watch_IsRun(void);

void Countdown_SendDataToApp(uint8_t ack, uint8_t operate);

#endif

