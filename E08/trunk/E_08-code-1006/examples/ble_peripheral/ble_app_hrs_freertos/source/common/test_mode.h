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


void Test_ProtocolParsing(uint8_t *in_data, uint8_t *out_data);

//心率自动测量每次
void Test_HeartStart(void);
void Test_HeartStop(void);


#endif
