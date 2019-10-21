/*******************Copyright(c)*********************************
**                  Veepoo Tech
**            COPYRIGHT (C) 2016-2019
**----------------------FileInfo---------------------------------
** File name:                  test_mode.h
** Last modified Date:         2018-06-29
** Last SYS_VERSION:           0.0.1
** Descriptions:
**---------------------------------------------------------------
** Createdby:                  WJ
** Createddate:                2018-06-29
** SYS_VERSION:                0.0.1
** Descriptions:               ����ģ��ӿ�
**---------------------------------------------------------------
** Modified by:				
** Modified date:			
** SYS_VERSION:				
** Modified Descriptions:	
****************************************************************/

#ifndef _TEST_MODE_H_
#define _TEST_MODE_H_

#include <stdint.h>
#include <stdbool.h>

#pragma pack(1)
typedef struct
{
    uint8_t     flag;                   // �Ƿ����ɼ��Զ���������
    uint8_t     mode;                   // 1:�Զ�Ѫ��ģʽ��2:����ģʽ
    uint8_t     times_end;              // ��ǰ�׶ν�����־�����ʲ���������Ѫ������5���ӽ���
    uint16_t    send_wait_len;          // ��ֹadc���Ͳ���ȫ�����߶෢
    uint8_t     result_send;            //
}test_ox_info_t;

#pragma pack()


void Test_ModeInit(void);
void Test_ProtocolParsing(uint8_t *in_data, uint8_t *out_data);

//�����Զ�����ÿ��
void Test_HeartStart(void);
void Test_HeartStop(void);
void Test_HeartTimesEndRepeat(void);
void Test_HeartParameterRepeat(void);
//
void Test_OxygenStart(void);
void Test_OxygenStop(void);
void Test_OxygenTimesEndRepeat(void);
void Test_OxygenParameterRepeat(void);

#endif
