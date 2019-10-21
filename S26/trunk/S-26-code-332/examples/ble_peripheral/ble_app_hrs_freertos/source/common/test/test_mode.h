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
** Descriptions:               测试模块接口
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
    uint8_t     flag;                   // 是否开启采集自动测量数据
    uint8_t     mode;                   // 1:自动血氧模式、2:心率模式
    uint8_t     times_end;              // 当前阶段结束标志：心率测量结束、血氧测量5分钟结束
    uint16_t    send_wait_len;          // 防止adc发送不完全，或者多发
    uint8_t     result_send;            //
}test_ox_info_t;

#pragma pack()


void Test_ModeInit(void);
void Test_ProtocolParsing(uint8_t *in_data, uint8_t *out_data);

//心率自动测量每次
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
