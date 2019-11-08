/***************************Copyright(c)***********************************
**                       Veepoo Tech
**                  COPYRIGHT (C) 2016-2019
**-----------File Info-----------------------------------------------------
** File        name:     tp_gesture.h
** Last modified Date:   2018-03-23
** Last SYS_VERSION:     0.0.0
** Descriptions:            
**-------------------------------------------------------------------------
** Created  by:          YX
** Created  date:        2018-03-23
** SYS_VERSION:          0.0.0
** Descriptions:         
**-------------------------------------------------------------------------
** Modified by:				
** Modified date:  			          
** SYS_VERSION:                 
** Descriptions:                        
***************************************************************************/

#ifndef TP_GESTURE_H__
#define TP_GESTURE_H__

#include "stdint.h"
#include <string.h>

#define TP_NULL_STATE        (0)    //��
#define TP_START_STATE       (1)    //��ʼ
#define TP_STOP_STATE        (2)    //����

#define TP_MODE_NULL            ((uint16_t)0x0000)       //��
#define TP_MODE_CLICK           ((uint16_t)0x0001)       //����
#define TP_MODE_SLIDE           ((uint16_t)0x0002)       //����
#define TP_MODE_MARGIN_SLIDE    ((uint16_t)0x0004)       //��Ե����
#define TP_MODE_LONG_PRESS_3S   ((uint16_t)0x0008)       //����3s
#define TP_MODE_LONG_PRESS_6S   ((uint16_t)0x0010)       //����6s
#define TP_MODE_TRANSLATION     ((uint16_t)0x0020)       //ƽ��

/**************************
(0,MAX_Y)..............(MAX_X,MAX_Y)
..........................
..........................
..........................
(0,0)...............(MAX_X,0)
**************************/
#define TP_NULL_MARGIN_STATE    ((uint8_t)0x00)       //�����ڱ�Ե
#define TP_UP_MARGIN_STATE      ((uint8_t)0x01)       //�����ϱ�Ե
#define TP_DOWN_MARGIN_STATE    ((uint8_t)0x02)       //�����±�Ե
#define TP_LEFT_MARGIN_STATE    ((uint8_t)0x04)       //�������Ե
#define TP_RIGHT_MARGIN_STATE   ((uint8_t)0x08)       //�����ұ�Ե

#define TP_NULL_SLIDE_STATE    ((uint8_t)0x00)       //û�л���
#define TP_DOWN_SLIDE_STATE    ((uint8_t)0x01)       //�����»�
#define TP_UP_SLIDE_STATE      ((uint8_t)0x02)       //�����ϻ�
#define TP_LEFT_SLIDE_STATE    ((uint8_t)0x01)       //������
#define TP_RIGHT_SLIDE_STATE   ((uint8_t)0x02)       //�����һ�

#define TP_X_SLIDE_STATE       ((uint8_t)0x01)       //����X����
#define TP_Y_SLIDE_STATE       ((uint8_t)0x02)       //����Y����

//��ǰλ��֧�ֵĲ�����ʽ
#define TP_GEST_NULL                        ((uint32_t)0x00000000)      //��
#define TP_GEST_CLICK                       ((uint32_t)0x00000001)      //����
#define TP_GEST_LONG_PRESS_3S               ((uint32_t)0x00000002)      //3�볤��
#define TP_GEST_LONG_PRESS_6S               ((uint32_t)0x00000004)      //6�볤��
#define TP_GEST_X_SLIDE                     ((uint32_t)0x00000008)      //X���򻬶�
#define TP_GEST_Y_SLIDE                     ((uint32_t)0x00000010)      //Y���򻬶�
#define TP_GEST_LEFT_MARGIN_SLIDE           ((uint32_t)0x00000020)      //���Ե����
#define TP_GEST_RIGHT_MARGIN_SLIDE          ((uint32_t)0x00000040)      //�ұ�Ե����
#define TP_GEST_UP_MARGIN_SLIDE             ((uint32_t)0x00000080)      //�ϱ�Ե����
#define TP_GEST_DOWN_MARGIN_SLIDE           ((uint32_t)0x00000100)      //�±�Ե����
#define TP_GEST_TRANSLATION                 ((uint32_t)0x00000200)      //ƽ��

#define MAX_SUPPORTED_FINGER_NUM 1  //֧�ִ����������

typedef struct
{
	uint16_t x;		//x����
	uint16_t y;		//Y����
}coord_info_t;

typedef struct
{
	coord_info_t point;	//������
	uint16_t wide;		//��
    uint16_t high;		//��
}area_square_info_t;    //��������


typedef struct
{
	coord_info_t point;	//Բ������
	uint16_t radius;	//�뾶
}area_circle_info_t;    //Բ������


typedef struct
{
	uint8_t x_direct;		//X����(��������)
	uint8_t y_direct;		//Y����(���ϡ�����)
	uint8_t xy_direct;		//�ܷ���(X����,Y����)
	uint8_t xy_degree;		//ƫ�ƵĽǶ�,��ʱ����
}move_direct_info_t;

typedef struct
{
	uint16_t x_dist;		//X�ƶ�����
	uint16_t y_dist;		//Y�ƶ�����
	uint16_t xy_dist;		//ֱ���ƶ�����
}move_dist_info_t;

typedef struct
{
    uint8_t tp_state;					//״̬(0:����,1:ʶ��ɹ�,2:�������)
    uint16_t now_mode;					//��ǰģʽ(����������)
    uint16_t total_mode;				//��ģʽ(��Ե������������ƽ�ơ�����������)
    uint8_t margin_state;               //��Ե��λ��
	move_direct_info_t  now_direct;		//��ǰ����
	move_direct_info_t  total_direct;	//�ܷ���
	coord_info_t start_coord;			//��ʼ����
	coord_info_t last_coord;			//��һ������(��һ�ε����һ������)
	coord_info_t now_coord;				//��ǰ����(���һ������)
	move_dist_info_t total_move_dist;	//���ƶ�����
	move_dist_info_t each_move_dist;	//�����ƶ�����
}tp_gesture_info_t;//���ƵĽ������

typedef struct
{
    uint16_t mode;                      //ģʽ
    coord_info_t last_coord;			//��һ������
	coord_info_t now_coord;				//��ǰ����
    move_direct_info_t direct;          //����
    move_dist_info_t move_dist;         //����
}tp_stage_data_t;


typedef struct
{
    uint8_t         margin;     //��Ե����(0���ޣ�1:�ϣ�2���£�4����8����)
    uint8_t         mode;       //ģʽ����(0���ޣ�1��������2��������8������)
    uint8_t         direct;     //��������(0���ޣ�1:�ϣ�2���£�3����4����)
    uint16_t        dist;       //��������
    coord_info_t    start_coord;//��ʼ����
    coord_info_t    stop_coord; //��������
}tp_result_t;


#define TP_ALL_DATA_T   tp_all_data_t
#define TP_COORD_DATA_T coord_info_t

/*������ؽṹ��Ķ���*/
//typedef struct
//{
//    uint16_t    x;			//x����
//    uint16_t    y;			//Y����
//    uint8_t    width;		    //���
//    uint8_t    sub_status;	//״̬
//}TP_COORD_DATA_T;             //ÿ��������Ĳ���

typedef struct
{
    uint16_t           status;	//��״̬
    TP_COORD_DATA_T    coord;	//ÿ�������������
}TP_ALL_DATA_T;	//tp���ص����в���

extern uint32_t simul_data_len;
extern TP_ALL_DATA_T simul_data_t[];
//ģ�����ݳ�ʼ��
void Tp_SimulatorDataProcess(void);
    
//����30ms����һ��(���ֽ���״̬ʱ,���Ͻ���һ�μ���,��ֹ���β�������һ�μ�����)
uint32_t Tp_GestureProcess(TP_ALL_DATA_T * tp_data ,uint32_t len, uint8_t resv);

//��ȡ���ƽ��
tp_gesture_info_t * Tp_GetGesture(void);

//����֧�ֵĲ���,ʹ�÷�ʽ����
//uint32_t Tp_SetGestureParam(uint32_t state);
tp_result_t Tp_SetGestureParam(uint32_t state);

//��ʼ��(�ɼ����ݵ�Ƶ��:data_hz,��Ե�ٷֱ�x��y:margin_x��margin_y,ƽ�����ƺ�������������ʱ��:time)
//uint32_t Tp_GestureInit(uint32_t data_hz, uint8_t margin_x, uint8_t margin_y, uint32_t time);
uint32_t Tp_GestureInit(uint32_t data_hz, uint16_t max_x_coord, uint16_t max_y_coord, uint8_t margin_x, uint8_t margin_y, uint32_t time, uint32_t slide_min_dist);

#endif
