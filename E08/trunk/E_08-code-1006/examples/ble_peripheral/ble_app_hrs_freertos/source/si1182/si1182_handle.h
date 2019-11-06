/*******************Copyright(c)*********************************
** Veepoo Tech
** COPYRIGHT (C) 2016-2019
**----------------------FileInfo---------------------------------
** File name:                  si117x_handle.h
** Last modified Date:         2017-12-22
** Last SYS_VERSION:           0.0.1
** Descriptions:
**---------------------------------------------------------------
** Createdby:                  JSH
** Createddate:                2017-12-22
** SYS_VERSION:                0.0.1
** Descriptions:               si117xµÄ½Ó¿Ú
**---------------------------------------------------------------
** Modified by:				
** Modified date:			
** SYS_VERSION:				
** Modified Descriptions:	
****************************************************************/


#ifndef __SI117X_HANDLE_H
#define __SI117X_HANDLE_H


#include "stdint.h"


void Si1182_IntHandler(void);

uint32_t StartDataLogging(void);

uint32_t StopDataLogging(void);

int32_t Si1182_FifoReadAndConvert(const uint8_t *buffer, uint32_t len);

int32_t Si1182_HandleFifoDataPpg(uint8_t *des, uint8_t *src,uint32_t len);

int32_t Si1182_HandleFifoDataEcg(uint8_t *des, uint8_t *src,uint32_t len);

int32_t Si1182_HandleFifoDataEcgAndLd(uint8_t *des, const uint8_t *src,uint32_t len, uint8_t *ld_des, uint32_t *ld_des_len);

#endif

