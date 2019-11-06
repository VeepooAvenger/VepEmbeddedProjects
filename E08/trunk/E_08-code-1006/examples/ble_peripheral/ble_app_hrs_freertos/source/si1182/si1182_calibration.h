/*******************Copyright(c)*********************************
** Veepoo Tech
** COPYRIGHT (C) 2016-2019
**----------------------FileInfo---------------------------------
** File name:                  si1182_calibration.h
** Last modified Date:         2019-1-22
** Last SYS_VERSION:           0.0.1
** Descriptions:
**---------------------------------------------------------------
** Createdby:                  JSH
** Createddate:                2019-1-22
** SYS_VERSION:                0.0.1
** Descriptions:               si1182У׼
**---------------------------------------------------------------
** Modified by:				
** Modified date:			
** SYS_VERSION:				
** Modified Descriptions:	
****************************************************************/

#ifndef __SI1182_CALIBRATION_H
#define __SI1182_CALIBRATION_H

#include "stdint.h"
#include "Blood_OxygenDetection.h"

typedef struct
{
	uint8_t i_red;					//
	uint8_t i_ird;					//
	uint8_t adc_range_red;			//
	uint8_t adc_range_ird;			//
	uint8_t clk_div_red;			//
	uint8_t clk_div_ird;			//
	uint8_t decim_red;				//
	uint8_t decim_ird;				//
	uint8_t adc_avg_red;			//
	uint8_t adc_avg_ird;			//
	uint8_t ppg_sw_avg_all;			//
	uint8_t ppg_meas_rate_h;
	uint8_t ppg_meas_rate_l;
	uint8_t blood_oxygen_change_flag;	//
	uint8_t blood_oxygen_calib_start;	// 血氧首次校准的标志
}si1182_calib_t;


// Si1182的校准数据的初始化
void Si1182_CalibInit(void);
// Si1182的校准函数-中断中调用
void Si1182_CalibOx(uint8_t task_num);
// Si1182的血氧校准数据的设置
void Si1182_CalibBloodOxygenSetPara(const bod_result_info_t* cp_param);
// Si1182的血氧校准数据的获取
void Si1182_CalibBloodOxygenGetPara(detection_char_info_t* p_param);
// Si1182的血氧校准的初始化
void Si1182_BloodOxgenCalibInit(void);

#endif
