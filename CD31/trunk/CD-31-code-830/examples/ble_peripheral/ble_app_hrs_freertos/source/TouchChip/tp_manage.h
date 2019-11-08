/*******************Copyright(c)*********************************
** Veepoo Tech
** COPYRIGHT (C) 2016-2019
**----------------------FileInfo---------------------------------
** File name:                  tp_manage.h
** Last modified Date:         2018-06-07
** Last SYS_VERSION:           0.0.1
** Descriptions:
**---------------------------------------------------------------
** Createdby:                  YX
** Createddate:                2018-06-07
** SYS_VERSION:                0.0.1
** Descriptions:               TP
**---------------------------------------------------------------
** Modified by:				
** Modified date:			
** SYS_VERSION:				
** Modified Descriptions:	
****************************************************************/

#ifndef TP_MANAGE_H
#define TP_MANAGE_H

#include "FreeRTOS.h"
#include "timers.h"
#include "app_timer.h"
#include "app_type.h"
#include "tp_gesture.h"
#include "tp_cal_data.h"

#include "cpu_common.h"

#define APP_TIME_TP_INTERVAL    60      //60ms

#define NULL_STATE          ((uint8_t)0x00)       //��״̬
#define UP_SLIDE_STATE      ((uint8_t)0x01)       //�����ϻ�
#define DOWN_SLIDE_STATE    ((uint8_t)0x02)       //�����»�
#define LEFT_SLIDE_STATE    ((uint8_t)0x03)       //������
#define RIGHT_SLIDE_STATE   ((uint8_t)0x04)       //�����һ�
#define CLICK_STATE         ((uint8_t)0x05)       //���ڵ���
#define LONG_PRESS_3S_STATE ((uint8_t)0x06)       //���ڳ���3s
#define LONG_PRESS_6S_STATE ((uint8_t)0x07)       //���ڳ���6s

#define PRESS_DOWN_N_STATE   ((uint8_t)0x00)       //δ����״̬
#define PRESS_DOWN_Y_STATE    ((uint8_t)0x01)      //����״̬


#define SLEEP_MODE_1  1     //��������
#define SLEEP_MODE_2  2     //˫������
#define SLEEP_MODE_3  3     //��������(����5S����8S�ڣ�̧�ֻ���)
#define SLEEP_MODE_4  4     //��������
#define SLEEP_MODE_5  5     //��������(����5S���ѣ�����̧��)
#define SLEEP_MODE_6  6     //������(Ĭ��ʹ��)

typedef struct
{
    uint8_t             tp_run_status:2;	//���е�״̬
    uint8_t             tp_init:1;	        //��ʼ��
    uint8_t             tp_result_status:1;	//���״̬
    uint8_t             tp_data_status:1;	//���ݵ�״̬
    uint8_t             tp_timer_status:1;	//��ʱ����״̬
    uint8_t             resv:2;             //Ԥ��
    tp_gesture_info_t   tp_result_s;	    //ÿ�������������
}tp_param_t;

//��ȡ�Ƿ���Ҫ����жϱ�־��״̬
uint8_t Tp_GetI2cIntStatus(void);

//�����Ƿ���Ҫ����жϱ�־��״̬
uint8_t Tp_SetI2cIntStatus(uint8_t status);

//��ȡTPͨ��״̬
uint8_t Tp_GetI2cStatus(void);

//����TPͨ��״̬
uint8_t Tp_SetI2cStatus(uint8_t status);

//�Ƿ����߻���
uint8_t Tp_IsSleepWakeup(void);

//Tp����
void Tp_WakeUp(void);

//TP����
void Tp_Sleep(uint8_t sleep_mode);

//TP�ػ�
void Tp_ShutDown(void);

//TP��ʼ��
void Tp_Init(uint8_t mode);

//��ȡ����������״̬
uint8_t Tp_GetKeyStatus(tp_key_info* tp_key);

//��ȡTP�İ���״̬
uint8_t Tp_GetPressDownSatus(void);

//���TP����ز���
void Tp_ClearInfo(void);

//��������ʶ��
uint8_t Tp_SquareAreaDiscern(point_coord_t point, uint16_t area_point_x, uint16_t area_point_y, uint16_t wide, uint16_t high);

//Բ������ʶ��
uint8_t Tp_CircleAreaDiscern(point_coord_t point, uint16_t area_point_x, uint16_t area_point_y, uint16_t radius);

#endif  //TP_MANAGE_H

