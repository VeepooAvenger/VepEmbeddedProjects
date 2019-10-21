#ifndef MOTOR_EVENT_H
#define MOTOR_EVENT_H

#include "stdint.h"
 
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
void MotorEventRun(void);//һ��ִ��
void MotorDelEvent(void);//ɾ��
void MotorAddEvent(uint8_t group,uint8_t continuous,uint8_t sustain_time,uint8_t interval);//���ӵ���¼� 
motor_event_t *MotorGetData(void);//��ȡ���ṹ��

void MotorPowerOn(void);
void MotorPowerOFF(void);
void MotorDelEvent(void);

#endif

