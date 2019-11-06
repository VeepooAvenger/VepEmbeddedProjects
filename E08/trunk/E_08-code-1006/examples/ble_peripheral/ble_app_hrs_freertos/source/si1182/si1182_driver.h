/*******************Copyright(c)*********************************
** Veepoo Tech
** COPYRIGHT (C) 2016-2019
**----------------------FileInfo---------------------------------
** File name:                  si1182_driver.h
** Last modified Date:         2017-12-22
** Last SYS_VERSION:           0.0.1
** Descriptions:
**---------------------------------------------------------------
** Createdby:                  JSH
** Createddate:                2017-12-22
** SYS_VERSION:                0.0.1
** Descriptions:               si117x的驱动
**---------------------------------------------------------------
** Modified by:				
** Modified date:			
** SYS_VERSION:				
** Modified Descriptions:	
****************************************************************/


#ifndef __SI1182_DRIVER_H
#define __SI1182_DRIVER_H

/***************************************************************/

#include "stdint.h"

#include "si1182_config.h"
#include "si1182_functions.h"

#include "calc_data_type.h"


#define		MODE_NULL						0
#define		MODE_ECG						1
#define		MODE_GREEN_AMB					2
#define		MODE_GREEN_IRED_AMB				3
#define		MODE_GREEN_RED_IRED_AMB			4
#define		MODE_RED_IRED_AMB				5

/***************************************************************/

#pragma	pack(push)
#pragma	pack(4)

typedef struct
{
	uint8_t		fifo_data[SI1182_FIFO_BUFFER_SIZE];
	int32_t     err_code;
    uint16_t    fifo_read_len;
    uint16_t    fifo_address;
	int32_t		fifo_err;
	uint8_t		fifo_read_over;				// fifo读取完成
	uint8_t		adc_mode;					// fifo的adc模式
	uint8_t		read_config;				// 读取寄存器配置
	uint8_t		param_config[SI117X_MAX_ADDRESS + 1];	// 寄存器配置缓存
    
    uint8_t     ecg_lead_state;             // ecg 导联检测状态.WJ.20190121
}si1182_data_t;

#pragma pack(pop)

extern si1182_data_t g_si1182_data;

/***************************************************************/

void Si1182_PowerOnInit(void);

int32_t Si1182_InitByMode(uint8_t init_mode);
	
void Si1182_Start(void);
	
void Si1182_Stop(void);

int32_t Si1182_GetDataPpg(uint8_t *out_data);

int32_t Si1182_GetDataEcg(uint8_t *out_data);

int32_t Si1182_GetDataEcgAndLd(uint8_t *out_data, uint8_t *ld_data, uint32_t *ld_data_len);

int32_t Si117x_UpdateConfig(uint8_t address, uint8_t value);

int32_t Si117x_ReadConfig(uint8_t address);

void Si1182_SetParameterReadFlag(void);

void Si1182_GetParameterConfig(uint8_t *data, uint32_t packet_num);

// 输出ECG导联状态
uint8_t Si1182_IsLeadOn(void);

void Si1182_CheckExceptionHandling(void);

#endif
