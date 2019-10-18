/***************************Copyright(c)***********************************
**                       Veepoo Tech
**                  COPYRIGHT (C) 2016-2019
**-----------File Info-----------------------------------------------------
** File        name:     hal_acc_calculate.h
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

#ifndef _HAL_ACC_CALCULATE_H_
#define _HAL_ACC_CALCULATE_H_

 
#define APP_HALACC_ENABLED     1

#if APP_HALACC_ENABLED

//���ٶ��¼�����
void Acc_StartHandler(void);

//�Ʋ�����̬����
void Acc_Calculate(void);

// ��ʱ������
void Acc_TimerCreate(void);

//��ʱ������
void Acc_TimerStart(void);

//���ٶ����ó�ʼ��
void Acc_Start(void);

//���ٶ�ֹͣ����������
void Acc_Stop(void);

//���ٶȳ�ʼ��
void Acc_Init(void);

//��ȡxyz������
void Acc_GetData(uint16_t *data);

//���ٶȻ�����
uint8_t Acc_Is_Move(void);

//���ٶ��ѳ�ʼ��
uint8_t Acc_Is_Init(void);

//����������־λ���
void Acc_GestureOledFlagClean(void);

//��ȡ����������־λ
uint8_t Acc_GestureOledFlagGet(void);

//���ͼ��ٶ�����
void Acc_SendConfig(void);

//���ü��ٶ�
void Acc_RestConfig(void);

#endif //APP_HALACC_ENABLED

#endif
