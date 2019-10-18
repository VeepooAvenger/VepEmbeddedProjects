/********************************Copyright(c)***********************************
**                          Veepoo Tech
**                        COPYRIGHT (C) 2016
**--------------File Info-------------------------------------------------------
** File name:               Veepoo_NoWearDect.h
** Last modified Date:      2016-09-02
** Last SYS_VERSION:        0.0.0.1
** Descriptions:
**
**------------------------------------------------------------------------------
** Created by:              HYR
** Created date:            2016-09-02
** SYS_VERSION:             0.0.0.1
** Descriptions:
**------------------------------------------------------------------------------
** Modified by:             HJJ
** Modified date:           2017-07-07
** SYS_VERSION:             01.01.08.02
** DES:                     modified the  process of  giving no wear state 4  
**------------------------------------------------------------------------------
** Modified by:             HJJ
** Modified date:           2017-07-12
** SYS_VERSION:             01.01.08.04 
** DES:                     modified the no wear time  of state 3  
**------------------------------------------------------------------------------
** Modified by:             HJJ
** Modified date:           2017-07-14
** SYS_VERSION:             01.01.08.05 
** DES:                     modified the no wear state of 6  
**------------------------------------------------------------------------------
**** Modified by:           HJJ
** Modified date:           2017-07-20
** SYS_VERSION:             01.01.10.00 
** DES:                     modified the vesion  (nrf52)
**------------------------------------------------------------------------------
** Modified by:             HJJ
** Modified date:           2017-08-25 
** Descriptions:            01.01.11.00 
** DES:                     modified the file's name to nowear_detection
**------------------------------------------------------------------------------
** Modified by:             HJJ
** Modified date:           2017-12-29
** Descriptions:            01.01.12.00 
** DES:                     add the gesture of 51,52,71 and 72 for nowear state 3
**------------------------------------------------------------------------------
*******************************************************************************/

#ifndef _NOWEAR_DETECTION_H_
#define _NOWEAR_DETECTION_H_

#include "stdint.h"
#define  WEAR_PASS_STATE              0
#define  ELECTRONIC_WEAR_NOT_PASS     1  
#define  DEFINE_NOT_WEAR              5
#define  ELECTRONIC_HALF_NOT_PASS     2
#define  GESTURE_OF_11_TWO_HOURS      3
#define  SAME_GESTURE_FOR_THREE_HOURS 4

#define   CHARGED_STATE               6     //charge state 
#define   HALL_SWITCH_ON              7     //open of huoer

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
}sport_history_t;

 typedef struct
{
	uint8_t vnwd_movemotion;   //20170705 HJJ sport
	uint8_t vnwd_steps_number; //20170705 HJJ step 
	uint8_t vnwd_nowear_time;  //20170705 HJJ nowear time 
	uint8_t vnwd_nowear_time_two_hours;  //20170712 HJJ two hours for 11£¨51,52£©
	uint8_t vnwd_nowear_time_half_hour;  //20170712 HJJ half an hour 
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
** Descriptions:            nowear_state = CHARGED_STATE;6 means charge £»
                            nowear_state = HALL_SWITCH_ON;7 means earphone go away from  band ;
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
