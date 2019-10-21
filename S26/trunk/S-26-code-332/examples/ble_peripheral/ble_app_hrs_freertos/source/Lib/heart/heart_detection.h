/********************************Copyright(c)***********************************
**                           Veepoo Tech
**                         COPYRIGHT (C) 2016
**--------------File Info-------------------------------------------------------
** File name:               heart_detection.h
** Last modified Date:       2016-07-27
** Last SYS_VERSION:            01.01.00.01
** Descriptions:
**------------------------------------------------------------------------------
** Modified by:             HJJ
** Modified date:           2017-08-03
** SYS_VERSION:             0x01010200
** Descriptions:            modified some parameters to promote tha  accuracy of Heart rate
**------------------------------------------------------------------------------
** Modified by:             HYR
** Modified date:           2017-09-06
** SYS_VERSION:             0x06010900
** Descriptions:            1. Change the sampling rate to 50hz for saving system resource
						2. Change the cycle num check  to get cycle more accurate
						3. Change the quanlity check to make the level more reasonable
						4. Change the kalman filter param to make a stable output
**------------------------------------------------------------------------------
** Modified by:             HYR
** Modified date:           2017-09-20
** SYS_VERSION:             0x06020100
** Descriptions:            1. complete the sports mode
2. add smooth filter for pulse output
**------------------------------------------------------------------------------
** Modified by:             HYR
** Modified date:           2017-10-13
** SYS_VERSION:             0x06020200
** Descriptions:            1.Official verion of 06020103
**------------------------------------------------------------------------------
** Modified by:             HYR
** Modified date:           2017-11-01
** SYS_VERSION:             0x06020201   0x06020300
** Descriptions:            1.Add test mode control
**------------------------------------------------------------------------------
** Modified by:             HYR
** Modified date:           2018-03-07
** SYS_VERSION:             0x06020301 0x06020302 20180312
** Descriptions:            1.Add move_120s condition for high pulse control
2. Fast run pulse rate control change move_120s from 11 to 16.
**------------------------------------------------------------------------------
** Modified by:             HYR
** Modified date:           2018-03-13
** SYS_VERSION:             0x06020303
** Descriptions:            1.Chang the move pulse param.
2. Slow run range from 11-15 change to 11-18,and fast run range from 16-25 change to 19-30
**------------------------------------------------------------------------------
** Modified by:             HYR
** Modified date:           2018-04-25
** SYS_VERSION:             0x06020308
1. Kalman output as 1 when max than 0.5. for the situation of output was min 1 point.
2. Change the sports variance param for pulse follow
3.  pkf_p_covariance change from 0.538 to 0.38. pkf_system_vari change from 0.34 to 0.3
**------------------------------------------------------------------------------
** Modified by:             HYR
** Modified date:           2018-04-27
** SYS_VERSION:             0x06020310
1. Change the scaning check
**------------------------------------------------------------------------------
** Modified by:             HYR
** Modified date:           2018-05-09
** SYS_VERSION:             0x06020312
1. Change the  measure variance for result change too slow.
2. Delete the round control of pulse result
3. Change the sports mode switch condition to hold down the pulse rising
**------------------------------------------------------------------------------
** Modified by:             HYR
** Modified date:           2018-05-10
** SYS_VERSION:             0x06020500
1. Change the min sports pulse control
2. Optimize the pulse charateristic compute
3. Change the signal quality distribute
4. Delete the wave inversion
**------------------------------------------------------------------------------
** Modified by:             HYR
** Modified date:           2018-05-10
** SYS_VERSION:             0x06020600
1. Min cycle demand change to 2
4. Add the wave inversion
**------------------------------------------------------------------------------
** Modified by:             HYR
** Modified date:           2018-05-12
** SYS_VERSION:             0x06020700
1. Change the way of geting cycle,get cycle from peak point
2. Change the cycle check way to get more acurrate cycle
**------------------------------------------------------------------------------
** Modified by:             HYR
** Modified date:           2018-05-14
** SYS_VERSION:             0x06020800
1. Change cycle clear number from 120 to 300 for cycle search missing
2. Filter time change from 5 to 9
**----------------------------------------------------------
** Modified by:             HYR
** Modified date:           2018-05-24
** SYS_VERSION:             0x06020801
1. Add ac value output.
**------------------------------------------------------------------------------
** Modified by:             HYR
** Modified date:           2018-06-27
** SYS_VERSION:             0x06030000
1. Change pulse delay time from 8 15 to 5 8 to make the result output quickly
2. Change the quality control of value pulse output
3. Change the sports mode control to 3
4. Add the high pulse judge in cycle num of signal quanlity check
**------------------------------------------------------------------------------
** Modified by:             HYR
** Modified date:           2018-06-28
** SYS_VERSION:             0x06030100
1. Change the sports pulse kalman control to make the sports pulse output reasonable
2. Change the first sports pulse value way
3. modified the quality control of cycle num
**------------------------------------------------------------------------------
*******************************************************************************/
#ifndef  _HEART_DETECTION_H
#define  _HEART_DETECTION_H
#include "stdlib.h"
#include "stdint.h"
#include "pedometer.h"
#include "sports_pulse_result_process.h"

enum pulse_mode
{
	PULSE_STATTIC_MODE,
	PLUSE_SPORTS_MODE
};

typedef struct
{
	uint8_t heart_pulse;           
	uint8_t wave_station;
	uint8_t cycle_std;
	uint8_t error_code;           //error_code = 1;???????
	//Add 20180524
	int32_t ac_value;
	float   signal_quality;
}heart_info_t;

/********************************************************************
** Function name:           Heart_Init
** Descriptions:            init the variable of heart_info_t
** input parameters:        none
** output parameters:       none
** Returned value:          none
** Created by:              HJJ
** Created Date:            2017-07-27
********************************************************************/
void Heart_Init(void);

/********************************************************************
** Function name:           Heart_Detection
** Descriptions:
** input parameters:        heart_green_singal  heart_ambient_signal
** output parameters:       none
** Returned value:          error code
** Created by:              HYR
** Created Date:            2016-07-27
**-------------------------------------------------------------------
** Modified by:
** Modified date:
**-------------------------------------------------------------------
********************************************************************/
uint8_t Heart_Detection(const int32_t *heart_green_singal, uint8_t hd_test_mode, move_index_t hd_move_s);

/********************************************************************
** Function name:           Get_HeartInfo
** Descriptions:            output the hear information 
** input parameters:        ghi_info_s
** output parameters:       none
** Returned value:          none
** Created by:              HYR
** Created Date:            2017-09-06
**-------------------------------------------------------------------
********************************************************************/
void Get_HeartInfo(heart_info_t * ghi_info_s);

/********************************************************************
** Function name:           Get_HeartVersion
** Descriptions:
** input parameters:        none
** output parameters:       none
** Returned value:          BOD_VERSION
** Created by:              HYR
** Created Date:            2016-07-29
**-------------------------------------------------------------------
** Modified by:
** Modified date:
**-------------------------------------------------------------------
********************************************************************/
uint32_t Get_HeartVersion(void);
#endif
