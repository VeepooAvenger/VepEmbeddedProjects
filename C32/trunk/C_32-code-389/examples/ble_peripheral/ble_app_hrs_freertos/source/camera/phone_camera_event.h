/***************************Copyright(c)***********************************
**                       Veepoo Tech
**                  COPYRIGHT (C) 2016-2019
**-----------File Info-----------------------------------------------------
** File        name:     phone_camera_event.h
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

#ifndef _PHONE_CAMERA_EVENT_H_
#define _PHONE_CAMERA_EVENT_H_

#include <stdint.h>
#include <stdbool.h>

/* ����¼��ṹ�� */
typedef struct
{
		uint8_t  ready:1;             /* �ֻ�����ģʽ���� */
		uint8_t  photo_ok:1;          /* �ֻ������ѷ��� */
        uint8_t  photo_state:1;
		uint8_t  resv:5;              /* ����λ */
	
		uint8_t  current_time;        /* ��ǰʱ�� */
		uint8_t  internal_time;       /* ҡһҡ���ʱ�� */
		uint8_t  ready_keep_time;     /* ���վ�������ʱ�� */
	  uint8_t  tp_keep_time;        /* ����OK����ʱ�� */
}camera_control_t;


//��������ģʽ�������գ���ʾ
void Camera_BraceletEntry(void);

//�������
void Camera_BtnClick(void);

//�˳����գ��رգգ�
void Camera_BraceletExit(void);

//�˳����շ�������
void Camera_BraceletExitSendData(void);

//���մ���
void Camera_WaitTask(void);

//��ȡ���տ��Ʋ���
camera_control_t *Camera_GetState(void);

#endif
