/*******************Copyright(c)*********************************
** Veepoo Tech
** COPYRIGHT (C) 2016-2019
**----------------------FileInfo---------------------------------
** File name:                   
** Last modified Date:         2018-11-14
** Last SYS_VERSION:           0.0.1
** Descriptions:
**---------------------------------------------------------------
** Createdby:                  WJ
** Createddate:                2018-11-14
** SYS_VERSION:                0.0.1
** Descriptions:               ²âÊÔ½Ó¿Ú
**---------------------------------------------------------------
** Modified by:				
** Modified date:			
** SYS_VERSION:				
** Modified Descriptions:	
****************************************************************/


#ifndef _API_TEST_INTERFACE_H_
#define _API_TEST_INTERFACE_H_


void Test_SetEcgResultData(void);
void Test_SetAutomaticEcgResultData(void);

void Test_SendEcgResultId(void);
void Test_SendEcgResultDataById(void);

void Test_SendEcgAutomaticResultId(void);
void Test_SendEcgAutomaticResultDataById(void);

uint8_t *Test_SetSleepData(uint8_t *p_sleep_info);
void Test_SendSleepDataById(void);
    


#endif //_API_TEST_INTERFACE_H_
