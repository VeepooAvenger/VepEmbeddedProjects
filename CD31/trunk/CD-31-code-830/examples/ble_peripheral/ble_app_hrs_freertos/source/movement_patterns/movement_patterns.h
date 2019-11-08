/***************************Copyright(c)***********************************
**                       Veepoo Tech
**                  COPYRIGHT (C) 2016-2019
**-----------File Info-----------------------------------------------------
** File        name:     movement_patterns.h
** Last modified Date:   2017-08-04
** Last SYS_VERSION:     0.0.0
** Descriptions:            
**-------------------------------------------------------------------------
** Created  by:          WJ
** Created  date:        2017-08-04
** SYS_VERSION:          0.0.0
** Descriptions:         
**-------------------------------------------------------------------------
** Modified by:				
** Modified date:  			          
** SYS_VERSION:                 
** Descriptions:                        
***************************************************************************/

#ifndef _MOVEMENT_PATTERNS_H
#define _MOVEMENT_PATTERNS_H
#include "app_flash.h"

#define MOVEMENT_PATTERNS_MAX_BLOCK     MOVEMENT_PATTERNS_DATA_BLOCK_MAX     

typedef enum
{
    MOVEMENT_STATE_IDEL = 0,        //����
    MOVEMENT_STATE_RUN,             //�˶���
    MOVEMENT_STATE_PAUSE,           //��ͣ��
    MOVEMENT_STATE_FINISH,          //��ʱ���
}MOVEMENT_STATE_TYPE_E;


#pragma pack(1)
//�˶�ģʽ������¼
typedef struct
{
    uint16_t year;           //��
    uint8_t  month;          //��
    uint8_t  day;            //��
    uint8_t  hour;           //ʱ
    uint8_t  minute;         //��
    uint8_t  heart;          //����
    uint16_t movement;       //�˶��� 
    uint16_t step;           //�Ʋ� 
    uint16_t calories;       //��·�ca��
    uint16_t distance;       //���루�ף�
    uint8_t  pause_flag;     //��ͣ��־
    uint8_t  retain[4];      //����
}movement_patterns_data_t;
//�˶�ģʽͷ����Ϣ
typedef struct
{    
    uint8_t flag;               //���ʹ�ñ�־ 
    uint16_t start_year;         //��ʼ��
    uint8_t start_month;        //��ʼ��
    uint8_t start_day;          //��ʼ��
    uint8_t start_hour;         //��ʼСʱ
    uint8_t start_minute;       //��ʼ����
    uint8_t start_second;       //��ʼ��
    
    uint16_t end_year;           //������
    uint8_t end_month;          //������
    uint8_t end_day;            //������
    uint8_t end_hour;           //����Сʱ
    uint8_t end_minute;         //��������
    uint8_t end_second;         //������
    
    uint32_t all_step;          //�ܼƲ�
    uint32_t all_distance;      //�ܾ���
    uint32_t all_calories;      //�ܿ�·��
    uint32_t all_movement;      //���˶���
    uint16_t record_cnt;        //�ܼ�¼����
    uint8_t  pause_times;       //��ͣ����
    uint16_t all_pause_time;    //����ͣʱ��
    uint16_t crc;               //CRCУ�� 
    uint8_t  retain[8];         //����,����
}movement_patterns_head_t;

typedef struct{ 
    uint8_t movement_block;
    uint16_t movement_num_start;
    uint16_t movement_num_total;
}movement_send_info_t;


//�˶�ģʽ����
typedef struct
{ 
    uint8_t  block;                 //���
    uint16_t pack_cnt;              //��¼����  
    
    uint8_t  run_state;             //����״̬
    uint16_t run_time;              //�����˶�ģʽ��ʱ�䣨s��
    uint16_t run_vail_time;         //�˶�ģʽ��Ч��ʱ�䣨s��
    uint16_t current_pause_time;    //��ǰ��ͣʱ��
    uint8_t  pause_flag;            //��ͣ��־��ǰһ�����ڣ�
    
    uint16_t run_start_time;        //��ͣ�����е���Чʱ�䣨s��

    uint8_t   heart;                  //ʵʱ����
    uint32_t  distance; 
    uint32_t  calories;
    uint32_t  step;
    uint32_t  movement;
    
    uint8_t   ui_heart;                  //ʵʱ����
    uint32_t  ui_distance; 
    uint32_t  ui_calories;
    uint32_t  ui_step;
    uint32_t  ui_movement;
    
    uint32_t last_step;              //���ļƲ�
    uint32_t last_distance;          //������
    uint32_t last_calories;          //���·��
    uint32_t last_movement;          //����˶���  
    
    uint32_t pause_step;             //��ͣ�ļƲ�
    uint32_t pause_distance;         //��ͣ����
    uint32_t pause_calories;         //��ͣ��·��
    uint32_t pause_movement;         //��ͣ�˶��� 
     
}movement_patterns_info_t;
#pragma pack()


//�˶�ģʽ��ʼ��(�ϵ硢��λ)
void Movement_RestInit(void);

//�˶�ģʽ��ʼ��
void Movement_Init(void);

//�˶�ģʽ��ʼ
void Movement_Start(void);

//�˶�ģʽ��ͣ
void Movement_Pause(void);

//����ͷ����Ϣ
void Movement_SaveHeadInfo(void);

//�˶�ģʽ����
void Movement_Finish(void);

//�˶�ģʽ�˳�
void Movement_Exit(void);

//�˶�ģʽ״̬
uint8_t Movement_GetState(void);

//�˶�ģʽ��;����0�����У�1���˶�ģʽ��
uint8_t Movement_IsBusy(void);

// �˶�������������1s�¼���
void Movement_Monitor(void);

//���ݿ�Ų����˶�����;����0������ʧ�ܣ�1�������ɹ�
uint8_t Movement_EreaFlashByBlock(uint8_t block);

//���������˶�ģʽ����;����0������ʧ�ܣ�1�������ɹ�;
uint8_t Movement_EreaFlashAllBlock(void);

//����һ�����˶�����;����0������ʧ�ܣ�1������ɹ�
uint8_t Movement_SaveMovementData(void);

//�����˶�����ͷ������;����0������ʧ�ܣ�1������ɹ�
uint8_t Movement_SaveMovementHead(void);

//��ȡ�˶�ģʽͷ����ϢCRC;crc_num��CRC�ĸ���;crc:crcֵ��crc[0] = CRC0��8bit��crc[1] = CRC0��8bit
void Movement_GetHeadCRC(uint8_t *crc, uint8_t crc_num);

//���÷����˶�ģʽ������Ϣ 
uint8_t Movement_SetSendDataInfo(uint8_t block);

//�����˶�����;����0�����ͽ�����1��������Ҫ����
uint8_t Movement_SendDataHandler(void);

//��ͣ��ʱʱ����;time:��ͣ��ʱʱ��;����0����Чֵ��1����Чֵ
uint8_t Movement_IsVaildThePuseTimeOut(uint16_t time);

movement_patterns_info_t *Movement_GetInfoData(void);

//��ȡ�˶��Ƿ񵽴������ͣ�Ĵ���������0����,1����
uint8_t Movement_IsMaxPauseCnt(void);

//���ڻָ���������
void Movement_RestoreFactoryExit(void);

#endif  //__GUI_DATA_INTEFACE_H


