/********************************Copyright(c)***********************************
**                           Veepoo Tech
**                          COPYRIGHT (C) 2016
**--------------File Info-------------------------------------------------------
** File name:               Respiratory_Rate.h
** Last modified Date:      2017-07-13
** Last SYS_VERSION:        01.01.01.01
** Descriptions:            Function for respiratory rate calculate
**
**------------------------------------------------------------------------------
** Modified by:             HJJ
** Modified date:           2018-04-20
** SYS_VERSION:             0x01010600
** Descriptions:            change the version
**------------------------------------------------------------------------------
** Modified by:             HJJ
** Modified date:           2018-05-11
** SYS_VERSION:             0x01010601
** Descriptions:            remove the unnecessary code
**------------------------------------------------------------------------------
** Modified by:             HJJ
** Modified date:           2018-05-15
** SYS_VERSION:             0x01010701
** Descriptions:            add the condition  of  rcr_move_situation
**------------------------------------------------------------------------------
** Modified by:             HJJ
** Modified date:           2018-05-16
** SYS_VERSION:             0x01010702
** Descriptions:            remove the condition of Motioncombined waveform quality
**------------------------------------------------------------------------------
** Modified by:             HJJ
** Modified date:           2018-05-17
** SYS_VERSION:             0x01010703
** Descriptions:            saving the last value when move
**------------------------------------------------------------------------------
** Modified by:             HJJ
** Modified date:           2018-05-18
** SYS_VERSION:             0x01010704
** Descriptions:            remove the useless definite
**------------------------------------------------------------------------------
** Modified by:             HJJ
** Modified date:           2018-05-29
** SYS_VERSION:             0x01010800
** Descriptions:            reduce the flash of this algorithm and changing the version
**------------------------------------------------------------------------------
** Modified by:             HJJ
** Modified date:           2018-08-09
** SYS_VERSION:             0x01010801
** Descriptions:            add the processing of wavelet and fft
**------------------------------------------------------------------------------
** Modified by:             HYR
** Modified date:           2018-08-14
** SYS_VERSION:             0x01010852
** Descriptions:            1. Change the wavelet and fft .2. Using the 6 level of sym
**------------------------------------------------------------------------------
** Modified by:             HJJ
** Modified date:           2018-08-20
** SYS_VERSION:             0x01010853
** Descriptions:            1.add the inint of struct of signal_characteristic_t
                            2.add the conditon of getting the valid character of signal_characteristic_t
**------------------------------------------------------------------------------
** Modified by:             HJJ
** Modified date:           2018-08-23
** SYS_VERSION:             0x01010900
** Descriptions:            change the version 
**------------------------------------------------------------------------------
*******************************************************************************/
#ifndef __RESP_RATE_
#define __RESP_RATE_
#include "stdlib.h"
#include "stdint.h"
#include "pedometer.h"

//#include "iostream"
//using namespace     std;

#define RR_MAX_WAVE_NUM       48               //change 12 to 48 HJJ 20180309  the peak's num in 15s

typedef struct
{
	uint8_t resp_rate_second;                 //Instantaneous respiration rate for the current second
	uint8_t resp_amp_second;                  // Current second amplitude instantaneous respiration rate
	uint8_t resp_ave_second;                 //Average instantaneous respiration rate for the current  second
	uint8_t resp_rate_revise_second;         //the  modified value of current  second of Frequency instantaneous Respiratory rate
	uint8_t resp_amp_revise_second;          //the modified value of current  second of  amplitude instantaneous respiration rate
	uint8_t resp_ave_revise_second;          //Average instantaneous breathing rate corrected for the current  second
}resp_second_t;

typedef struct
{
	uint8_t resp_rate;                       //respiration rate of one minute
	uint8_t resp_current;                    //Instantaneous respiration rate per second
	uint8_t wave_station;                    //wavestation
	uint8_t signal_valid_flag;               //wavestate vlide flag ,useless now
	int8_t all_flag;                        //wavestate flag 
	resp_second_t resp_second_s;             //every second's value 
}resp_info_t;

typedef struct
{
	uint8_t  pesp_rate;
	uint8_t  peak_peak_num;
	uint8_t  cycle_num;
	uint16_t cycle_buff[RR_MAX_WAVE_NUM - 1];    //interval_value
	uint16_t peak_position[RR_MAX_WAVE_NUM];
	uint16_t valley_position[RR_MAX_WAVE_NUM];
	uint16_t rr_amp_value[RR_MAX_WAVE_NUM];
}signal_characteristic_t;



/***********************Variable Using:******************************
** Local variable deep: 3084byte
** Global variable :  49byte
** Malloc: 0byte
**-------------------------------------------------------------------
********************************************************************/

/********************************************************************
** Function name:           RR_DetectInit
** Descriptions:            Initialize the respiratory rate detection
** input parameters:        none
** output parameters:       none
** Returned value:          none
** Created by:              HYR
** Created Date:            2017-07-13
**-------------------------------------------------------------------
********************************************************************/
void RR_DetectInit(void);

/********************************************************************
** Function name:           Respiratory_RateDetect
** Descriptions:            Main entry of respiaratory rate detection
** input parameters:        bod_red_signal
** output parameters:       none
** Returned value:          rrd_erro
** Created by:              HYR
** Created Date:            2017-07-13
**-------------------------------------------------------------------
********************************************************************/
int32_t Respiratory_RateDetect(const int32_t *rr_singal_input, move_index_t r_move_index_s);

/********************************************************************
** Function name:           Get_RRInfo
** Descriptions:            Get the respiratory rate result
** input parameters:        none
** output parameters:       none
** Returned value:          rr_result
** Created by:              HYR
** Created Date:            2017-07-13
**-------------------------------------------------------------------
********************************************************************/
void Get_RRInfo(resp_info_t* grr_result_info);

/********************************************************************
** Function name:           Get_RRTestInfo
** Descriptions:            Get the debug test info
** input parameters:        none
** output parameters:       grti_info_output
** Returned value:          none
** Created by:              HYR
** Created Date:            2017-07-14
**-------------------------------------------------------------------
********************************************************************/
void Get_RRTestInfo(signal_characteristic_t *grti_info_output);

/********************************************************************
** Function name:           Get_RRVersion
** Descriptions:            Get the respiratory rate detection version
** input parameters:        none
** output parameters:       none
** Returned value:          RR_VERSION
** Created by:              HYR
** Created Date:            2017-07-13
**-------------------------------------------------------------------
********************************************************************/
uint32_t Get_RRVersion(void);

#endif

