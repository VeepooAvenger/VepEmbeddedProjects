/***************************Copyright(c)***********************************
**                       Veepoo Tech
**                  COPYRIGHT (C) 2016-2019
**-----------File Info-----------------------------------------------------
** File        name:     battery.h
** Last modified Date:   2017-08-21
** Last SYS_VERSION:     0.0.0
** Descriptions:            
**-------------------------------------------------------------------------
** Created  by:          WJ
** Created  date:        2017-08-21
** SYS_VERSION:          0.0.0
** Descriptions:         
**-------------------------------------------------------------------------
** Modified by:				
** Modified date:  			          
** SYS_VERSION:                 
** Descriptions:                        
***************************************************************************/
#ifndef BATTERY_H__
#define BATTERY_H__

#include "stdint.h"
#include "board.h"

#define APP_BATTERY_ENABLED     1


#define POWER_NORMAL     0x00              //�ŵ�                              
#define POWER_CHARGE     0x01              //���
#define POWER_LOW        0x02              //�͵� 
#define POWER_FULL       0x03              //����

#define BATTERY_LEVEL_LOW       5          //�͵�ȼ�
#define BATTERY_LEVEL_CHARGE    6          //���ȼ�


#define POWER_CHARGING_PIN   CHARGING_PIN       //�����  

#define POWER_CHG_PIN        CHG_PIN            //������ 

//��ȡ����״̬
uint8_t Battery_GetChargingFullState(void);
    
//���ó���״̬
void Battery_SetChargingFullState(uint8_t flg);
    
//��ʼ��ص������
void Battery_TestingStart(void);

//ֹͣ��ص������
void Battery_TestingStop(void);

//��ص�������
void Battery_TestingCalculate(void);

//��ȡ��ص����ȼ�
uint8_t Battery_GetGrade(void);

//��ȡ��ص�������ֵ��ƽ����
uint16_t Battery_GetAdcValue(void);

//��س��״̬�ܽų�ʼ��
void Battery_InitPin(void);

//������ڳ��
uint8_t Battery_IsCharging(void);

//����Ƿ����
uint8_t Battery_IsChargingFull(void);

//����͵�ػ�״̬;����0��û�У�1:�͵���Ҫ�ػ�
uint8_t Battery_IsLowToShut(void);

//����͵�״̬;����0��û�У�1:�͵�
uint8_t Battery_IsLow(void);

//���ڳ������͵�ػ���־ 
void Battery_ClearLowFlag(void);

void Battery_SetFull(void);

#endif // BATTERY_H__

