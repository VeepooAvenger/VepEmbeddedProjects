#ifndef __BLOOD_OXYGENCALIB_
#define __BLOOD_OXYGENCALIB_
//#define Adjust_OneSecond
#include "Blood_OxygenDetection.h"

//#define TI4410_BUFFER_ERROR_FLAG

/********************************************************************
** Function name:           Blood_OxygenCalibInit
** Descriptions:            用于每次校准开启初始化
** input parameters:        none
** output parameters:       none
** Returned value:
** Created by:              FWT
** Created Date:            2018-05-04
**-------------------------------------------------------------------
********************************************************************/
void Blood_OxygenCalibInitForOne(volatile bod_result_info_t *calib_out_s);
#ifdef Adjust_OneSecond
/********************************************************************
** Function name:           Blood_OxygenCalib
** Descriptions:            give the best gain and elec
** input parameters:        *boc_info *ired_signl *amb_signal *red_signal
** output parameters:       none
** Returned value:          boc_err
** Created by:              FWT
** Created Date:            2018-05-04
********************************************************************/
uint8_t Blood_OxygenCalibOne(
	volatile bod_result_info_t *boc_info,
	int32_t          					  red_signal,
	int32_t          					  ired_signl,
	int32_t          					  amb_signal,
	detection_char_info_t      *rigister_value);

#else

uint8_t Blood_OxygenCalib(
	volatile bod_result_info_t *boc_info,
	const int32_t *ired_signl,
	const int32_t *red_signal,
	const int32_t *amb_signal,
	detection_char_info_t *rigister_value, 
	uint32_t spo2_cnt);
//void Blood_OxygenCalibInit()；
#endif // DEBUG



#endif