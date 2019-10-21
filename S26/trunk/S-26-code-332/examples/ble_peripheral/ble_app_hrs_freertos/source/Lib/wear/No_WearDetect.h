/********************************Copyright(c)***********************************
**                          Veepoo Tech
**                        COPYRIGHT (C) 2016
**--------------File Info-------------------------------------------------------
** File name:               No_WearDect.h
** Last modified Date:      2016-09-02
** Last SYS_VERSION:        0.0.0.1
** Descriptions:
**
**------------------------------------------------------------------------------
** Modified by:             HYR
** Modified date:           2016-11-30
** SYS_VERSION:             01.01.03.03  Add vnwd_momentum_threhold setting 
**------------------------------------------------------------------------------
** Modified by:             HYR
** Modified date:           2017-01-20
** SYS_VERSION:             01.01.03.05  modified the no wear cnt 
**------------------------------------------------------------------------------
** Modified by:             HYR
** Modified date:           2017-01-20
** SYS_VERSION:             01.01.03.06  modified init fuction
**------------------------------------------------------------------------------
** Modified by:             HYR
** Modified date:           2017-03-06 
** SYS_VERSION:             01.01.06.00  Change the wear result clear 
**------------------------------------------------------------------------------
** Modified by:             HJJ
** Modified date:           2017-07-06 
** SYS_VERSION:             01.01.08.01  add the nowear state 4 
**------------------------------------------------------------------------------
** Modified by:             HJJ
** Modified date:           2017-07-11
** SYS_VERSION:             01.01.01.00 modified the name of this function 
**------------------------------------------------------------------------------
** Modified by:             HJJ
** Modified date:           2017-07-28
** SYS_VERSION:             01.01.03.00 
** Descriptions:            renew the condition of wear flag 
**------------------------------------------------------------------------------
** Modified by:             HJJ
** Modified date:           2017-08-11
** SYS_VERSION:             01.01.04.00 
** Descriptions:            renew the name of files
**------------------------------------------------------------------------------
** Modified by:             HJJ
** Modified date:           2017-10-28
** SYS_VERSION:             01.01.04.01 
** Descriptions:            change the momentum condition of 2 
**------------------------------------------------------------------------------
** Modified by:             HJJ
** Modified date:           2017-12-29 
** SYS_VERSION:             01.01.05.00 
** Description :            add the gesture of  51,52,71,72 for nowear state 3
**-------------------------------------------------------------------
** Modified by:             HJJ
** Modified date:           2018-02-07
** SYS_VERSION:             01.01.05.01
** Description :            add the variable which is the  sum of the mean amount of exercise per half hour in three hours
**------------------------------------------------------------------------------
** Modified by:             HJJ
** Modified date:           2018-03-02
** SYS_VERSION:             01.01.06.00
** Description :            giving the fixed value for nowear flag of 3,and change the version 
**------------------------------------------------------------------------------
** Modified by:             HJJ
** Modified date:           2018-04-16
** SYS_VERSION:             01.01.06.01
** Description :            optimize the nowear process
**------------------------------------------------------------------------------
** Modified by:             HJJ
** Modified date:           2018-05-17
** SYS_VERSION:             01.01.07.00
** Description :            change the version 
**------------------------------------------------------------------------------
*******************************************************************************/


#ifndef __NOWEAR_DETECT_
#define __NOWEAR_DETECT_

#include "stdint.h"
#define  WEAR_PASS_STATE              0
#define  ELECTRONIC_WEAR_NOT_PASS     1  
#define  DEFINE_NOT_WEAR              5
#define  ELECTRONIC_HALF_NOT_PASS     2
#define  GESTURE_OF_11_TWO_HOURS      3
#define  SAME_GESTURE_FOR_THREE_HOURS 4

#define   CHARGED_STATE               6     //³äµç×´Ì¬
#define   HALL_SWITCH_ON              7     //»ô¶û¿ª×´Ì¬

typedef struct
{
	uint32_t steps_5;
	uint32_t steps_10;
	uint32_t steps_15;
	uint32_t steps_20;
	uint32_t steps_30;
	
	uint32_t momentum_5;
	uint32_t momentum_10;
	uint32_t momentum_15;
	uint32_t momentum_20;
	uint32_t momentum_30;
	uint32_t hours_three; //20180206 HJJ add the sum of every half hour for the lastest three hours 
}sport_history_t;

 typedef struct
{
	uint8_t vnwd_movemotion;   //20170705 HJJ sport 
	uint8_t vnwd_steps_number; //20170705 HJJ step
	uint8_t vnwd_nowear_time;  //20170705 HJJ three hours keep same gesture
	uint8_t vnwd_nowear_time_two_hours;  //20170712 HJJ two hours keep 51,52,71or72
	uint8_t vnwd_nowear_time_half_hour;  //20170712 HJJ °ë¸öÐ¡Ê±
}nowear_parameters_t;
	
/********************************************************************
** Function name:           Vnwd_SetWear
** Descriptions:
** input parameters:        vsw_set_state  
** output parameters:       none
** Returned value:          none
** Created by:              HYR
** Created Date:            2016-09-06
**-------------------------------------------------------------------
** Modified by:
** Modified date:
**-------------------------------------------------------------------
********************************************************************/
void Vnwd_SetWear(uint8_t vsw_set_state);
/********************************************************************
** Function name:           Set_NoWearParameters
** Descriptions:
** input parameters:        none
** output parameters:       none
** Returned value:          nowear_info
** Created by:              HJJ
** Created Date:            2017-07-05
**-------------------------------------------------------------------
** Modified by:
** Modified date:
**-------------------------------------------------------------------
********************************************************************/

void  Set_NoWearParameters(nowear_parameters_t snp_nowear_info);
/********************************************************************
** Function name:           Amth_JudgeMovement
** Descriptions:
** input parameters:        tData  
** output parameters:       none
** Returned value:          none
** Created by:              HYR
** Created Date:            2016-09-02
**-------------------------------------------------------------------
** Modified by:
** Modified date:
**-------------------------------------------------------------------
********************************************************************/
uint8_t Vnwd_JudgeMovement(int16_t *ajm_acc_tri_data);

/********************************************************************
** Function name:           Vnwd_HistoryMometum
** Descriptions:
** input parameters:        vhm_5min_momentum vhm_5min_steps
** output parameters:       none
** Returned value:          sport_info
** Created by:              HYR
** Created Date:            2016-09-02
**-------------------------------------------------------------------
** Modified by:
** Modified date:
**-------------------------------------------------------------------
********************************************************************/
sport_history_t Vnwd_HistoryMomentum(uint32_t vhm_momentum_temp, uint32_t vhm_steps_temp);

/********************************************************************
** Function name:           Vnwd_NoWearInit
** Descriptions:
** input parameters:        none
** output parameters:       none
** Returned value:          none
** Created by:              HYR
** Created Date:            2016-09-03
**-------------------------------------------------------------------
** Modified by:
** Modified date:
**-------------------------------------------------------------------
********************************************************************/
void Vnwd_NoWearInit(void);

/********************************************************************
** Function name:           Set_NoWearOnevote
** Descriptions:            nowear_state = CHARGED_STATE;6  means charge
                            nowear_state = HALL_SWITCH_ON;7 means huoer open ;
** input parameters:        csr_hall_state or  power_state,
** output parameters:       
** Returned value:          none
** Created by:              HJJ
** Created Date:            2017-07-14
**-------------------------------------------------------------------
** Modified by:
** Modified date:
**-------------------------------------------------------------------
********************************************************************/
void Set_NoWearOnevote(uint8_t nowear_state);

/********************************************************************
** Function name:           Vnwd_NoWearDect
** Descriptions:
** input parameters:        vnwd_sport_info vnwd_pte_wear_flag
** output parameters:       none
** Returned value:          vnwd_wear_result //0:WEAR, 1:NO WEAR FOR NOW, 2/3:LONG TIME NO WEAR.HYR.20160914
** Created by:              HYR
** Created Date:            2016-09-02
**-------------------------------------------------------------------
** Modified by:             HYR
** Modified date:           2016-11-30 Add vnwd_momentum_threhold setting 
**-------------------------------------------------------------------
********************************************************************/
void Vnwd_NoWearDect(sport_history_t vnwd_sport_info,uint8_t vnwd_pte_wear_flag, uint8_t vnw_gesture_station,uint32_t vnwd_momentum_threhold);

/********************************************************************
** Function name:           Get_NoWearStation
** Descriptions:
** input parameters:        none
** output parameters:       none
** Returned value:          vnwd_wear_result
** Created by:              HYR
** Created Date:            2016-09-03
**-------------------------------------------------------------------
** Modified by:
** Modified date:
**-------------------------------------------------------------------
********************************************************************/
uint8_t Get_NoWearStation(void);
#endif 

