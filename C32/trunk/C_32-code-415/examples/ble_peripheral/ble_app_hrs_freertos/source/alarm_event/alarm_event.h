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
//#include "app_config_data.h"

#define ALARM_CLOCK_MAX_NUM      20

#define COUNTDOWN_MAX_NUM       1

typedef enum
{
    STOPWATCH_IDE,          //׼��
    STOPWATCH_READY,        //����
    STOPWATCH_START,        //��ʼ
    STOPWATCH_STOP,         //ֹͣ
}stopwatch_e;

#pragma pack(1)

//��������ṹ��
typedef struct
{
	uint8_t valid;          //����--��Ч/��Ч(1bit,��ʡ�ڴ�ռ�)
	uint8_t hour;           //����--ʱ	
	uint8_t minute;         //����--��    
	uint8_t state;          //����--״̬(�����ر�)
	uint8_t repeat;         //����--�ظ�
    uint8_t scene_mode;     //�龰ģʽ   
    uint16_t year;          //��    
    uint8_t month;          //��
    uint8_t day;            //��
    uint8_t res[2];         //����4�ֽڶ��룡��������
}alarm_clock_config_t;

typedef struct{ 
    uint8_t alarm_index;
    uint8_t alarm_num_start;
    uint8_t alarm_num_total;
    uint16_t alarm_crc16;
}alarm_clock_data_t;

typedef struct{ 
    uint8_t alarm_hour;
    uint8_t alarm_minute;
    uint8_t alarm_mode;
}alarm_clock_do_t;

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

//����

//��ȡ���ִ�е���������
alarm_clock_do_t *Alarm_GetAlarmClockDo(void);

//�����������ݷ��͵�����
void Alarm_SetSendData(uint8_t start_index, uint8_t total_index, uint16_t crc16);

//�����������ݵ�APP��ÿ�η���һ�飬��Ҫ��ʱѭ������
uint8_t Alarm_SendDataHandler(void);

//ͨ��ID������ӣ���Ҫ����
uint8_t Alarm_ClearByID(uint8_t id);

//������������,������Ҫ���棬���ڻָ��������ã�
//������������������û����ݿ������������ʵ�����ʹ��
uint8_t Alarm_InitAll(void);

//������������,��Ҫ����
uint8_t Alarm_ClearAll(void);

//�������ӣ���ʱ��ִ�����Ӵ��������Ӵ���
void Alarm_Monitor(void);

//�������������Լ죬�ж�������Ч����
uint8_t Alarm_Check(alarm_clock_config_t *alarm_clock_temp);

//��ȡ���ӵ���Ч����
uint8_t Alarm_GetValidConfigNum(void);

// ��������,��������Э��(APP����),����Ϊ���ӵ���Ч����,
uint8_t Alarm_Set(uint8_t *data, uint8_t len);

//�������ӵ�CRC
uint16_t Alarm_GetCRC(void);

//������д�뵽flash��Ŀǰ52��������10s�ı�����ʱ,��1���¼���ִ��
uint8_t Alarm_SetToFlashCheck(void);

//��flash��ȡ�������ݣ�ע��������ƽ̨���ײ�������
uint8_t Alarm_getFromFlash(void);

// �Լ쵥�����ӣ���ʱ����Ҫ����״̬
void Alarm_ValidCheck(void);

//�����Լ죬�����Ч���ӣ��ϵ硢���߶�ʱ�Լ�
void Alarm_InitCheck(void);

//���ӳ�ʼ�����Լ졢��ʱ�Լ�
void Alarm_Init(void);

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
//uint8_t Watch_Toggle(void);

//����ʱ
//����ʱ��ʼ��,����ִ�е�����
void Countdown_Init(void );

//����ʱ���ã�����ͨѶЭ��
uint8_t Countdown_SetTime(uint32_t second, uint8_t state, uint8_t id, uint8_t type);

//�Ӷ�ο�ʼ����ʱ,��������
uint8_t Countdown_SetTimeBySystemStart(void);

//��ȡ��ǰ����ִ�еĵ���ʱ����
countdown_config_t *Countdown_Get(void);

//��ȡ�������еĵ���ʱ״̬
uint8_t Countdown_GetState(void);

//������Ӧ���ݵ�APP,����ʱ���е�ʱ����
void Countdown_SendDataToApp(uint8_t ack, uint8_t operate);

//����ʱ���봦��,����һ�붨ʱ���ж���
void Countdown_ToRunSecond(void);

#endif

