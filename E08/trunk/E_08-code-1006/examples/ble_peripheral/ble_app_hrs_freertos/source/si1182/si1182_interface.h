/*******************Copyright(c)*********************************
** Veepoo Tech
** COPYRIGHT (C) 2016-2019
**----------------------FileInfo---------------------------------
** File name:                  si117x_interface.h
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


#ifndef __SI117X_INTERFACE_H
#define __SI117X_INTERFACE_H

/***************************************************************/

#include "stdint.h"
#include "board.h"

/***************************************************************/

#define SI118X_INT				SI1182_INT


#define SI1182_SPI_CS_LOW		PIN_LOW(SI1182_CS);
#define SI1182_SPI_CS_HIGH		PIN_HIGH(SI1182_CS);


void Si1182_TransferInit(void);

uint32_t Si117x_WriteRegister(uint32_t address, uint32_t data);

uint8_t Si117x_ReadRegister(uint32_t address);

void Si117x_BlockRead(uint32_t adress, uint8_t *out_data, uint32_t data_len);

void Si1182_InterruptInit(void);

void Si1182_InterruptUnInit(void);

void Si1182_FifoDataOk(void);

void Si1182_StartOk(void);

void Si1182_StopOk(void);

void Si117x_Delay10ms(void);

#endif

