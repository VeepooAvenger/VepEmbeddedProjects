/*******************Copyright(c)*********************************
** Veepoo Tech
** COPYRIGHT (C) 2016-2019
**----------------------FileInfo---------------------------------
** File name:                  test_state_machine.h
** Last modified Date:         2018-11-12
** Last SYS_VERSION:           0.0.1
** Descriptions:
**---------------------------------------------------------------
** Createdby:                  WJ
** Createddate:                2018-11-12
** SYS_VERSION:                0.0.1
** Descriptions:               测量状态机
**---------------------------------------------------------------
** Modified by:				
** Modified date:			
** SYS_VERSION:				
** Modified Descriptions:	
****************************************************************/


#ifndef _API_TEST_STATE_MACHINE_H_
#define _API_TEST_STATE_MACHINE_H_


#define MEAT_PPG_APP_HEART      0x0001
#define MEAT_PPG_APP_BP         0x0002
#define MEAT_PPG_BTN_HEART      0x0004
#define MEAT_PPG_BTN_BP         0x0008
#define MEAT_PPG_TIME           0x0010

#define MEAT_ECG_APP_HEART      0x0020
#define MEAT_ECG_BTN_HEART      0x0040
#define MEAT_ECG_TIME           0x0080      //暂时不使用


#define MEAT_OXYGEN_APP          0x0100
#define MEAT_OXYGEN_BTN          0x0200
#define MEAT_OXYGEN_TIME         0x0400      



uint8_t Statemachine_AddState(uint16_t the_state);

uint8_t Statemachine_BusyStateCode(uint16_t the_state);

uint8_t Statemachine_DelState(uint16_t the_state);

uint8_t Statemachine_IsEcg(void);

#endif  //_API_TEST_STATE_MACHINE_H_
