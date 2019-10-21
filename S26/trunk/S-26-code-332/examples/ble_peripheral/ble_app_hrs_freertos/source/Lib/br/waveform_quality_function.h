/********************************Copyright(c)***********************************
**                          Veepoo Tech
**                        COPYRIGHT (C) 2016-2019
**--------------File Info-------------------------------------------------------
** File name:                Waveform_QualityFunction.cpp
** Last modified Date:       2018-03-30
** Last SYS_VERSION:         01.01.01.01
**------------------------------------------------------------------------------
** Created by:              HJJ
** Created date:            2018-03-30
** Descriptions:            Signal quality calculate
**
**------------------------------------------------------------------------------
** Created by:              HJJ
** Created date:            2018-05-11
** Descriptions:            remove the unnecessary part
**------------------------------------------------------------------------------
*******************************************************************************/

#ifndef _SIGNAL_QUALITY_
#define _SIGNAL_QUALITY_

#include "Respiratory_Rate.h"
#include "math.h"
#include "string.h"
#include  "stdlib.h"
#include "stdint.h"
typedef struct
{
	uint8_t  shake_flag;
	uint8_t  diff_peak_valley;
	uint8_t  signal_diff_flag;
	uint8_t  rising_num;
	float   cycle_value_std;      

	float  cycle_diff_max;
	float  cycle_diff_std;
	float  peak_value_std_rate;
	float  valley_value_std_rate;
	float  diff_mean_amp_rate;
	float  amp_value_std_rate;
	float  peak_value_std;
	float  valley_value_std;
	int32_t amp_value_ave;
	float amp_value_std;
}wqf_signal_char_t;


/*******************************************************************************
** Function name:           Resp_GetWaveformValue
** Descriptions:            get the waveform quality
** input parameters:        rgwv_chaaracter
** output parameters:       none
** Returned value:          waveform value
** Created by:              HJJ
** Created Date:            2018-03-30
**------------------------------------------------------------------------------
*******************************************************************************/
uint8_t Resp_GetWaveformValue(signal_characteristic_t * rgwv_chaaracter);

/*******************************************************************************
** Function name:           Get_WaveformStd
** Descriptions:            get the waveform std
** input parameters:        rgwv_chaaracter
** output parameters:       none
** Returned value:          waveform std infomation
** Created by:              HJJ
** Created Date:            2018-03-30
**------------------------------------------------------------------------------
*******************************************************************************/
void Get_WaveformStd(signal_characteristic_t*gws_character, wqf_signal_char_t* gws_waveform_std);
/*******************************************************************************
** Function name:           Vph_StandardDeviation
** Descriptions:            Get the standard deviation for input signal
** input parameters:        vph_std_input vph_std_n
** output parameters:       none
** Returned value:          vph_std_deviation
** Created by:              HYR
** Created Date:            2016-07-13
**------------------------------------------------------------------------------
** Modified by:            HJJ
** Modified date:          2018-01-30
** Describe:               change the  data's type  from int32_t to float
**------------------------------------------------------------------------------
*******************************************************************************/
static float Vph_StandardDeviation(uint16_t *vph_std_input, uint8_t vph_std_n);

#endif

