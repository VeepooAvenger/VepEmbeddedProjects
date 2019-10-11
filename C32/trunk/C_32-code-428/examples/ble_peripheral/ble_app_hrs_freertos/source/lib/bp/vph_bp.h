/****************************************Copyright (c)****************************************************
**                       		 	            Veepoo Tech
**                          		       COPYRIGHT (C) 2016
**--------------File Info---------------------------------------------------------------------------------
** File name:               vph_bp.c
** Last modified Date:      2016-7-13
** Last Version:            01.01.00
** Descriptions:            Vph Blood Pressure Detection.
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             HYR
** Modified date:           2016-08-17.
** Version:                 01.01.00.10
** Descriptions:            1.Add test mode
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             HYR
** Modified date:           2016-08-18.
** Version:                 01.01.00.11
** Descriptions:            Modified adjust mode 2
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             HYR
** Modified date:           2016-08-19.
** Version:                 01.01.00.12
** Descriptions:            1.Add temp output thought pwv and vari in test mode 1,2
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             HYR
** Modified date:           2016-08-23.
** Version:                 01.01.00.13
** Descriptions:            1. Change average pulse get mode ,2. Adjust mode to calculate.
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             HYR
** Modified date:           2016-08-23.
** Version:                 01.01.01.14
** Descriptions:            1. vdd_last_pulse Only Change valid in test mode 1
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             HYR
** Modified date:           2016-08-24.
** Version:                 01.01.01.15
** Descriptions:            1. Change the linmit of SBP after sport 2. Change the way get pulse
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             HYR
** Modified date:           2016-08-24.
** Version:                 01.01.02.00
** Descriptions:            1.Modified firmode pulse test
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             HYR
** Modified date:           2017-02-08.
** Version:                 01.01.02.02
** Descriptions:            1.Cut the testing time
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             HYR
** Modified date:           2017-04-06.
** Version:                 01.01.06.00
** Descriptions:           Check the mode and test result befor output
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             HYR
** Modified date:           2017-04-07.
** Version:                 01.01.07.00
** Descriptions:           Add error check bp_flag = 3
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             HYR
** Modified date:           2018-10-08.
** Version:                 01.01.07.02
** Descriptions:            1. Add Final check for invalid result.
**                          2. Change the pulse output limit.
                            3. chang the valid judge limit
**--------------------------------------------------------------------------------------------------------
** Modified by:             DH
** Modified date:           2019-03-26.
** Version:                 01.01.09.00
** Descriptions:            1. modify some parameter         
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#ifndef PREGNANCY_H__
#define PREGNANCY_H__
#include <stdbool.h>
#include <stdint.h>

/*
* macros definitions
*/
#define VPH_BP_VERSION              (0x01010900)    //
#define VPH_SIGNAL_LEN              (512)           // 512/200=2.56S
#define VPH_SATURATION_SIGNAL       (1000)
#define VPH_SATURATION_TIMES        (6)
#define VPH_MAXTESTING_TIME         (19)            //20160721.HYR.Change from 44 to 36
#define VPH_WAVE_AMP_RANGE          (150)           //  
#define VPH_WAVE_TERM_RANGE         (25)
#define VPH_START_DIC_POINT         (14)

#pragma pack(1)
typedef struct bp_info
{
	uint8_t bp_flag;            // Testing flag 0 no result ;1 get a result; 2.no valid result 3.error code 4. error code sport  
	uint8_t valid_cnt;          // No use
	uint8_t wave_state;         // Wave station 
	uint8_t sp;                 // output of high blood pressure
	uint8_t dp;                 // output of low blood pressure
	uint8_t pulse_rate;         // output of pulse rate
	uint8_t pwv;                // No use
	uint8_t vari;               // No use
	uint8_t test_mode;         //  ouptut the test mode
}bp_info_t;

typedef struct //test_mode
{
	// Set testing mode for bp detection 0 no setting;
    // 1.first time testing after setting Adjust;
    // 2.Not first time testing after setting Adjust; 
	uint8_t test_mode;    
	//Set the sp for testing.Setting valid when test_mode = 1,2 
	uint8_t set_sp;       
	//Set the dp for testing.Setting valid when test_mode = 1,2 
	uint8_t set_dp;       
}test_mode_t;

#pragma pack()

/*
*  functions declarations for external call
*/
extern void BP_DetectionInit(void);
extern uint32_t Vph_BpDetection(const int16_t* vb_signal_input, int16_t vb_move_index, int16_t vb_test_station, test_mode_t vb_mode_info);
extern bp_info_t* BP_GetInfo(void);
extern uint32_t BP_GetVersion(void);

#endif

//#define MATLAB_PLOT_MODE
#ifdef MATLAB_PLOT_MODE
#include "plot_bymatlab.h"
#endif
