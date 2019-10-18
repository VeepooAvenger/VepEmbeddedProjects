/***************************Copyright(c)***********************************
**                       Veepoo Tech
**                  COPYRIGHT (C) 2016-2019
**-----------File Info-----------------------------------------------------
** File        name:     motor_event.h
** Last modified Date:   2017-07-28
** Last SYS_VERSION:     0.0.0
** Descriptions:            
**-------------------------------------------------------------------------
** Created  by:          WJ
** Created  date:        2017-07-28
** SYS_VERSION:          0.0.0
** Descriptions:         
**-------------------------------------------------------------------------
** Modified by:				
** Modified date:  			          
** SYS_VERSION:                 
** Descriptions:                        
***************************************************************************/
#ifndef MOTOR_EVENT_H
#define MOTOR_EVENT_H

#include "stdint.h"

#define APP_MOTOR_ENABLED   1
 
#pragma pack(1) 
//������ ÿ�������� ÿ��������
typedef struct
{ 
  uint8_t flag;//�����ʼ��־
  uint8_t continuous;//�����񶯻��Ƿ�����
  uint8_t sustain;//�������ʱ��
  uint8_t sustain_cnt;
  uint8_t interval;//���ʱ����
  uint8_t interval_cnt;
  uint8_t group;//���������
  uint8_t update;//�������
}motor_event_t;  
#pragma pack() 

//��ʼ��
void Motor_Init(void);

//����¼�����
void Motor_EventRun(void);

//ɾ������¼�
void Motor_DelEvent(void);

//�������¼�
void Motor_AddEvent(uint8_t group,uint8_t continuous,uint8_t sustain_time,uint8_t interval);

//��￪��
void Motor_PowerOn(void);

//���ر�
void Motor_PowerOFF(void);

//����Ƿ���
uint8_t Motor_IsWork(void);

#endif

