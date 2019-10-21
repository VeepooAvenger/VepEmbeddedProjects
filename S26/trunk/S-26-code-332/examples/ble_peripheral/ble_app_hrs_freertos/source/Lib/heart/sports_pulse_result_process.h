/********************************Copyright(c)******************************************************
**                           Veepoo Tech
**                         COPYRIGHT (C) 2015-2019
**--------------File Info--------------------------------------------------------------------------
** File name:             sports_process.h
** Last SYS_VERSION:        01010101
**-------------------------------------------------------------------------------------------------
** Created by:              HYR
** Created date:            20171010
** Descriptions:            Sports part processing
**
**-------------------------------------------------------------------------------------------------
**Modified: 01010200.HYR.20180416.Change the 60s move param to 120s move
**-------------------------------------------------------------------------------------------------
**Modified:01010300.HYR.20180417. 1.Change the comprehensive of sports pulse and measure pulse.
Make sure the result can change with the measure pulse when the signal quality is good.
2. Change the way of getting the sports variance
**-------------------------------------------------------------------------------------------------
**Modified:01010500.HYR.20180423.
1.Change the output of first pulse
**-------------------------------------------------------------------------------------------------
**Modified:01010600.HYR.20180424.
1. Add the pure sports effect when running fast
2. Change the slow run from 11 - 15 to 11 - 18 ,fast run from 16 - 30 to 19 - 30
**-------------------------------------------------------------------------------------------------
**Modified:01010800.HYR.20180509.
1. Delete the round control of pulse result
2. Change the sports mode switch condition to hold down the pulse rising
3. Change the sports variance
**------------------------------------------------------------------------------
**Modified:01010900.HYR.20180510.
1. Change the min sports pulse control
**------------------------------------------------------------------------------
**Modified:01020000.HYR.20180627.
1. Change pulse delay time from 8 15 to 5 8 to make the result output quickly
2. Change the quality control of value pulse output
**------------------------------------------------------------------------------
**************************************************************************************************/
#ifndef __SPORTS_PROCESSING_
#define __SPORTS_PROCESSING_
#include <stdbool.h>
#include <stdint.h>
#include "pedometer.h"
#include "string.h"
#include "stdlib.h"

//Max valid pulse rate range 
#define PRSV_MAX_VALID_PULSE       180  
//Min valid pulse rate range 
#define PRSV_MIN_VALID_PULSE       30  

#define PRSV_QUIET_PULSET_RATE_NUM    4
#define PRSV_QUIET_STD_PULSE_MIN     55
#define PRSV_QUIET_STD_PULSE_MAX     110

#define PRSV_CYCLE_THREHOLD1         10
#define PRSV_CYCLE_THREHOLD2         20
#define PRSV_CYCLE_THREHOLD3         40
#define PRSV_CYCLE_THREHOLD4         60

#define PRSV_VARIANCE_THREHOLD1      50
#define PRSV_VARIANCE_THREHOLD2      150   //120
#define PRSV_VARIANCE_THREHOLD3      400   //300
#define PRSV_VARIANCE_THREHOLD4      600   //300

#define PRSV_WALK_FAST_THREHOLD      6
#define PRSV_RUN_SLOW_THREHOLD       11
// PRSV_RUN_FAST_THREHOLD:16 change to 19 HYR.20180424
#define PRSV_RUN_FAST_THREHOLD       19 

#define PRSV_WALK_SLOW_PULSE_LIMIT   80
#define PRSV_WALK_FAST_PULSE_LIMIT   100
#define PRSV_RUN_SLOW_PULSE_LIMIT    120
#define PRSV_RUN_FAST_PULSE_LIMIT    140

#define PRSV_WALK_SLOW_PULSE_AVER    75
#define PRSV_WALK_FAST_PULSE_AVER    100
#define PRSV_RUN_SLOW_PULSE_AVER     110
#define PRSV_RUN_FAST_PULSE_AVER     130

#define PRSV_SWALK_PULSE_COEF        0.29f       //0.56f     //0.72 //HYR.20170922. 
#define PRSV_SWALK_MOVE_COEF         4.38f        //4.5f  
#define PRSV_SWALK_BASE_COEF         62.59f      //49.59f     //36.99f    //32.99//HYR.20170922.

#define PRSV_FWALK_PULSE_COEF        0.33f       //0.41f   
#define PRSV_FWALK_MOVE_COEF         3.8f       //3.95f  
#define PRSV_FWALK_BASE_COEF         62.65f      //63.65f     //47.65f  //HYR.20170922.

#define PRSV_SRUN_PULSE_COEF         0.33f   
#define PRSV_SRUN_MOVE_COEF          3.35f        //3.7f      //3.45f  
#define PRSV_SRUN_BASE_COEF          68.0f       //73.5f       //53.41f   //HYR.20170922.

#define PRSV_FRUN_PULSE_COEF         0.29f   
#define PRSV_FRUN_MOVE_COEF          2.8f  
#define PRSV_FRUN_BASE_COEF          81.15f      // 87.15f 

#define PRSV_START_TO_OUTPUT_TIMES    8

extern uint8_t prsv_output_pulse_buff[4];
extern float   sp_best_pulse;


/********************************************************************
** Function name:           Pulse_ResultSports
** Descriptions:            Result of pulse rate process in sports
** input parameters:        data_input prp_best_output  prsv_system_variance:0.2f  prp_signal_qual prsv_covariance prp_move_s
** output parameters:       prp_best_output
** Returned value:          none
** Created by:              HYR
** Created Date:            2017-08-10
********************************************************************/
uint8_t Pulse_ResultSports(uint8_t *prp_data_output, uint8_t prp_data_input, float prp_signal_qual, move_index_t prp_move_s,
						   uint16_t prp_sys_time);

/********************************************************************
** Function name:           Pulse_ResultStatic
** Descriptions:            Result of pulse rate process in static
** input parameters:        prst_data_input prst_signal_qual  prst_move_s   prst_sys_time
** output parameters:       prst_data_output
** Returned value:          prst_erro_code
** Created by:              HYR
** Created Date:            2018-04-11
********************************************************************/
uint8_t Pulse_ResultStatic(uint8_t *prst_data_output, uint8_t prst_data_input, float prst_signal_qual, move_index_t prst_move_s, uint16_t prst_sys_time);

/********************************************************************
** Function name:           Get_QuietPulse
** Descriptions:            get the quiet average pulse rate
** input parameters:        none
** output parameters:       none
** Returned value:          gqp_pulse_rate
** Created by:              HYR
** Created Date:            2017-07-05
**-------------------------------------------------------------------
********************************************************************/
uint8_t Get_QuietPulse(void);

#endif
