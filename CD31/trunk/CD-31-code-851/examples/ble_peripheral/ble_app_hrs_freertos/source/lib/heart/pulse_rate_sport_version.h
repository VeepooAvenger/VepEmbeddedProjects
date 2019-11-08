/********************************Copyright(c)******************************************************
**                           Veepoo Tech
**                         COPYRIGHT (C) 2015-2017
**--------------File Info--------------------------------------------------------------------------
** File name:             pulse_rate_sport_version.h
** Last modified Date:
** Last SYS_VERSION:
** Descriptions:
**-------------------------------------------------------------------------------------------------
** Created by:              HYR
** Created date:            20170105
** SYS_VERSION:
** Descriptions:            First 200hz version
**
**-------------------------------------------------------------------------------------------------
** Modified by:             HYR
** Modified date:           0x05010600 2017.07.24
1.  Modified the init of pulse output
**-------------------------------------------------------------------------------------------------
** Modified by:             HYR
** Modified date:           //05020103.20170922
1.Update the sports equation
2.Modified the pulse result output
**-------------------------------------------------------------------------------------------------
** Modified by:             HYR
** Modified date:           //05020107 20170925 //0x05020108 0x05020109  0x05020110 0x05020111 0x05020112
1.Update the sports equation
2. MAX move index change to 25
3. Deal the problem of pulse rising up to fast
4. Update the sports equation
5. Change the sports pulse to get trend of signal change
6. Modified the cycle average check
7. Add scaning cycle check
8. Correct the cycle check
9. Modified the prd_pulse_static init data to 73
10.Add the cycle check of last scaning time
**-------------------------------------------------------------------------------------------------
** Modified by:             HYR
** Modified date:           //05020200.20170929
1.official version of 0x05020112
**-------------------------------------------------------------------------------------------------
***************************************************************************************************/
#ifndef HEART_RATE_PROCESS_H
#define HEART_RATE_PROCESS_H

#include <stdbool.h>
#include <stdint.h>
#include "pedometer.h"

#define PRSV_MAX_VALID_PULSE       180  //Max valid pulse rate range 
#define PRSV_MIN_VALID_PULSE       30   //Min valid pulse rate range 


typedef struct
{
	uint8_t pulse;
	uint8_t wave_station;
	uint8_t scaning_times;
	uint8_t reserved2;
	int16_t amp;
	int32_t jump_cnt;
	float reserved1;
}pulse_rate_t;

/********************************************************************
** Function name:           Pulse_RateInit
** Descriptions:            Pulse rate detect init
** input parameters:        none
** output parameters:       none
** Returned value:          none
** Created by:              HYR
** Created Date:            2016-11-08
********************************************************************/
void Pulse_RateInit(void);

/********************************************************************
** Function name:           Pulse_RateDetection
** Descriptions:            Main entry of 200hz sample pulse rate detection
** input parameters:        adc_data_buff, heart_get_mode , move_station
** output parameters:       result_of_pulse
** Returned value:          errc_code
** Created by:              HYR
** Created Date:            2016-11-08
**-------------------------------------------------------------------
** Modified by:
** Modified date:
**-------------------------------------------------------------------
********************************************************************/
uint8_t Pulse_RateDetection(const int16_t *adc_data_buff, move_index_t moving_station);

/********************************************************************
** Function name:           Get_PulseRateResult
** Descriptions:            Get the reulst of pulse rate by pointer
** input parameters:        none
** output parameters:       gprr_pulse_result
** Returned value:          none
** Created by:              HYR
** Created Date:            2016-11-08
********************************************************************/
void Get_PulseRateResult(pulse_rate_t *gprr_pulse_result);

/********************************************************************
** Function name:           Get_PulseVersion
** Descriptions:            Get the algoritm version
** input parameters:        none
** output parameters:       none
** Returned value:          Version
** Created by:              HYR
** Created Date:            2016-11-08
********************************************************************/
uint32_t Get_PulseVersion(void);

#endif 





