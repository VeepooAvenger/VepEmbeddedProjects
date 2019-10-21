/****************************************Copyright (c)****************************************************
**                       		 	     Veepoo Tech
**                          	    COPYRIGHT (C) 2015-2019
**--------------File Info---------------------------------------------------------------------------------
** File name:               VPH_pedometer_02010101_20160711.c
** Last modified Date:      2016-07-11
** Last Version:            02.01.01
** Descriptions:            Pedometer for VPH_N project
**--------------------------------------------------------------------------------------------------------
** Modified by:             HYR VERSION 02010801  //02010900
** Modified date:           20170712
1. ACC_UP_TREND_THRESHOLD change to 11
2. valid 1 limit number change to 8
**--------------------------------------------------------------------------------------------------------
** Modified by:             HYR VERSION 020200.00
** Modified date:           2017-08-07
** Descriptions:
1. Modefied  the input personal information funtion
2. Complete the output distance and cal
**--------------------------------------------------------------------------------------------------------
** Modified by:             HYR VERSION 020201.00
** Modified date:           2017-08-15
** Descriptions:            1. Add sport info setting
**--------------------------------------------------------------------------------------------------------
** Modified by:             HYR VERSION 020202.00
** Modified date:           2017-08-17
** Descriptions:            1.Modified the setting data
2.Add pointer check
**--------------------------------------------------------------------------------------------------------
** Modified by:             HYR VERSION 020203.00
** Modified date:           2017-08-25
** Descriptions:            1. Modefied the movement of 60s 30s
**--------------------------------------------------------------------------------------------------------
** Modified by:             HYR VERSION 020203.01 020203.02 020203.03 020203.08
** Modified date:           2017-09-26     20171106    20171115  20171227
** Descriptions:            1. Arrange the code for momentum
2. Modified the variable name
3. Modified for less momentum steps
4. Modified the run distance gain for distance testing
5. Change api of get/clean step and get/clean momentum
**--------------------------------------------------------------------------------------------------------
**--------------------------------------------------------------------------------------------------------
** Modified by:             HYR VERSION 02030000
** Modified date:           2017-12-27
** Descriptions:            1. Delete the horizon step for too much steps
**--------------------------------------------------------------------------------------------------------
** Modified by:             HYR VERSION 02030006 // 02030100
** Modified date:           2018-01-09
** Descriptions:            1. modified for less steps, up cnt from 6 chang to 5,down cnt change from 4 to 5.
2. all step add horizon check
**--------------------------------------------------------------------------------------------------------
** Modified by:             HYR VERSION    02030200
** Modified date:           2018-01-17
** Descriptions:            1. Add move 120s
2. up cnt change from 12 to 11
**--------------------------------------------------------------------------------------------------------
** Modified by:             HYR VERSION    02030300
** Modified date:           2018-01-25
** Descriptions:
1.  ACC_VALID_CHECK_LEN change from 9 to 8
**--------------------------------------------------------------------------------------------------------
** Modified by:             HYR VERSION    02030500
** Modified date:           2018-01-25
** Descriptions:
1. Add start count step num parameter setting.Default:10
2. Add using hand setting                     Default:2
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#ifndef PEDOMETER_H__
#define PEDOMETER_H__

#include <stdbool.h>
#include <stdint.h>

#define     ACC_SET_2G_12BIT         2
#define     ACC_SET_4G_12BIT         4
#define     ACC_SET_8G_12BIT         8

#define     ACC_SET_2G_16BIT         32
#define     ACC_SET_4G_16BIT         64
#define     ACC_SET_8G_16BIT         128


typedef struct
{
	//Start count steps num
	uint8_t start_num;
	//1:dominant hand. 2:weak hand ; Default weak hand. 
	uint8_t hand_chose;
}ped_param_set_t;

typedef struct
{
	uint8_t move_5s;
	uint8_t move_15s;
	uint8_t move_30s;
	uint8_t move_60s;
	uint8_t move_120s;
}move_index_t;

typedef struct
{
	//move_15s:1~5
	int32_t walk_slow;
	//move_15s:6~10
	int32_t walk_fast;
	//move_15s:11~15
	int32_t run_slow;
	//move_15s:16~ 
	int32_t run_fast;
	int32_t steps_total;
}walk_station_info_t;

typedef struct
{
	// posi_steps = steps_total = AllStepGet
	uint32_t posi_steps;
	uint32_t posi_cal;
	uint32_t posi_distance;
}sport_information_info_t;

/*******************************************************************************
** Function name:          Ped_SetSportsInfo
** Descriptions:           Set the sports information of steps/cal/distance
** input parameters:       pss_set_steps pss_set_cal pss_set_distance
** output parameters:      none
** Returned value:         0:set fail;  1:set success
** Created by:             HYR
** Created Date:           2017-08-15
*******************************************************************************/
uint8_t Ped_SetSportsInfo(uint32_t pss_set_steps, uint32_t pss_set_cal, uint32_t pss_set_distance);

/*******************************************************************************
** Function name:           Ped_SetInputPersonalInformation
** Descriptions:            Set the personal information
** input parameters:
psipi_height: XX cm
psipi_weight: XX kg
psipi_gender: 0 femail, 1 mail
psipi_age:    0~200
** output parameters:      none
** Returned value:         none
** Created by:             HJJ
** Created Date:           2017-08-03
*******************************************************************************/
void Ped_SetInputPersonalInformation(uint16_t  psipi_height, float psipi_weight, uint8_t psipi_gender, uint16_t psipi_age);

/*******************************************************************************
** Function name:           Pedometer_Process
** Descriptions:
get_sensor_set:
ACC_SET_2G_12BIT         2
ACC_SET_4G_12BIT         4
ACC_SET_8G_12BIT         8

ACC_SET_2G_16BIT         32
ACC_SET_4G_16BIT         64
ACC_SET_8G_16BIT         128
** input parameters:       pp_data_input[3]: include x y z / pp_len:input number count get_sensor_set pp_param_s
** output parameters:      none
** Returned value:         none
** Created by:             HYR
** Created Date:           20161013
*******************************************************************************/
void Pedometer_Process(int16_t *pp_data_input, int16_t pp_len, uint8_t get_sensor_set, ped_param_set_t pp_param_s);

/*******************************************************************************
** Function name:           Work_MeasureGet
** Descriptions:            Get the momentum of sports
** input parameters:        none
** output parameters:       none
** Returned value:          Momentum of ACC
** Created by:              HYR
** Created Date:            20161013
**------------------------------------------------------------------------------
*******************************************************************************/
uint32_t Work_MeasureGet(void);

/*******************************************************************************
** Function name:           All_StepGet
** Descriptions:            Get all the steps
** input parameters:        none
** output parameters:       none
** Returned value:          steps total
** Created by:              HYR
** Created Date:            20161013
**------------------------------------------------------------------------------
*******************************************************************************/
uint32_t All_StepGet(void);

/*******************************************************************************
** Function name:           Move_IdexGet
** Descriptions:            Get the move index
** input parameters:        none
** output parameters:       mig_move_index
** Returned value:          none
** Created by:              HYR
** Created Date:            20161219
**------------------------------------------------------------------------------
*******************************************************************************/
void Move_IdexGet(move_index_t *mig_move_index);

/*******************************************************************************
** Function name:           Get_SportInformationInfo
** Descriptions:            Get steps, distance and kcal information
** input parameters:        none
** output parameters:       gsii_sport_info_s
** Returned value:          none
** Created by:              HJJ
** Created Date:            2017-08-05
**------------------------------------------------------------------------------
*******************************************************************************/
void Get_SportInformationInfo(sport_information_info_t * gsii_sport_info_s);

/*******************************************************************************
** Function name:           All_StepCln
** Descriptions:            clear the steps information
** input parameters:        none
** output parameters:       none
** Returned value:          none
** Created by:              HYR
** Created Date:            20161013
**------------------------------------------------------------------------------
*******************************************************************************/
void All_StepCln(void);

/*******************************************************************************
** Function name:           Work_MeasrueCln
** Descriptions:            Clear the mementum
** input parameters:        none
** output parameters:       none
** Returned value:          none
** Created by:              HYR
** Created Date:            20161013
**------------------------------------------------------------------------------
*******************************************************************************/
void Work_MeasrueCln(void);

/*******************************************************************************
** Function name:           All_StepSet
** Descriptions:            Set the start steps for Algorithm
** input parameters:        step_input
** output parameters:       none
** Returned value:          none
** Created by:              HYR
** Created Date:            20161013
**------------------------------------------------------------------------------
*******************************************************************************/
void All_StepSet(uint32_t step_input);

/*******************************************************************************
** Function name:           Vph_GetPedometerVersion
** Descriptions:            Version of pedometer algorithm
** input parameters:        none
** output parameters:       none
** Returned value:          Version of pedometer algorithm
** Created by:              HYR
** Created Date:            20161013
**------------------------------------------------------------------------------
*******************************************************************************/
uint32_t Vph_GetPedometerVersion(void);

/*******************************************************************************
** Function name:           Vph_GetShakeFlag
** Descriptions:            Return the one second momentum
** input parameters:        none
** output parameters:       none
** Returned value:          shake flag
** Created by:              HYR
** Created Date:            20161122
*******************************************************************************/
uint8_t Vph_GetShakeFlag(void);

/*******************************************************************************
** Function name:           Get_StepsInfo
** Descriptions:            Get all the steps information
** input parameters:        none
** output parameters:       asg_steps_s
** Returned value:          total step
** Created by:              HYR
** Created Date:            20170630
**------------------------------------------------------------------------------
*******************************************************************************/
uint32_t Get_StepsInfo(walk_station_info_t *asg_steps_s);
#endif
