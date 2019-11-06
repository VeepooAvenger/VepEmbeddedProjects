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
** Modified by:             YQW
** Modified date:           2019-04-29
** SYS_VERSION:             01.01.07.01
** Description :            1.modify the file format
                            2.mask the unused code
                            3.investigate the memory usage
**------------------------------------------------------------------------------
*******************************************************************************/
#ifndef __NOWEAR_DETECT_H__
#define __NOWEAR_DETECT_H__
#include <stdint.h>
#include <string.h>


/*
* Memory Usage(0x01010701)
* Global : 140(Bytes)
* Malloc : 0(Bytes)
* Local  : 22(Bytes)
*/
#define VNWD_NOWEAR_VERSION             ( 0x01010701    )
/* NO_WEAR_ALG_STATE macros */
#define WEAR_PASS_STATE                 ( 0             )                /* 手环佩戴中                        */
#define ELECTRONIC_WEAR_NOT_PASS        ( 1             )                /* 手环脱手检测确认中??              */
#define ELECTRONIC_HALF_NOT_PASS        ( 2             )                /* 手环脱手半小时                    */
#define GESTURE_OF_11_TWO_HOURS         ( 3             )                /* 手环脱手两个小时                  */
#define SAME_GESTURE_FOR_THREE_HOURS    ( 4             )                /* 手环脱手三个小时(改成了四小时)    */
#define DEFINE_NOT_WEAR                 ( 5             )                /* 手环脱手检测                      */
#define CHARGED_STATE                   ( 6             )                /* 充电状态                          */
#define HALL_SWITCH_ON                  ( 7             )                /* 霍尔开状态                        */
#define NO_WEAR_ALG_STATE_MIN           WEAR_PASS_STATE
#define NO_WEAR_ALG_STATE_MAX           HALL_SWITCH_ON

typedef struct tag_sport_history
{
    uint32_t steps_5                    ;
    uint32_t steps_10                   ;
    uint32_t steps_15                   ;
    uint32_t steps_20                   ;
    uint32_t steps_30                   ;
    uint32_t momentum_5                 ;
    uint32_t momentum_10                ;
    uint32_t momentum_15                ;
    uint32_t momentum_20                ;
    uint32_t momentum_30                ;
    uint32_t hours_three                ; /* 20180206 HJJ add the sum of every half hour for the lastest three hours */
}sport_history_t;


typedef struct tag_nowear_parameters
{
    uint8_t vnwd_movemotion             ;   /*  20170705 HJJ sport                          */
    uint8_t vnwd_steps_number           ;   /*  20170705 HJJ step                           */
    uint8_t vnwd_nowear_time            ;   /*  20170705 HJJ three hours keep same gesture  */
    uint8_t vnwd_nowear_time_two_hours  ;   /*  20170712 HJJ two hours keep 51,52,71or72    */
    uint8_t vnwd_nowear_time_half_hour  ;   /*  20170712 HJJ 半个小时                       */
}nowear_parameters_t;



/* make interface-name unchanged , make everyone happy, YQW 2019.01.14  */
#define Vnwd_SetWear         Vnwd_SetNoWearStation
#define Get_NoWearStation    Vnwd_GetNoWearStation
#define Set_NoWearParameters Vnwd_SetNoWearParameters
#define Set_NoWearOnevote    Vnwd_SetNoWearOneVote


/*
* function declarations for external call
*/
extern void             Vnwd_NoWearInit             ( void                                              );
extern void             Vnwd_NoWearDect             ( sport_history_t      vnwd_sport_info_s        ,       
                                                      uint8_t              vnwd_pte_wear_flag       ,
                                                      uint8_t              vnw_gesture_station      ,
                                                      uint32_t             vnwd_momentum_threhold
                                                    );
extern uint8_t          Vnwd_GetNoWearStation       ( void                                              );
extern uint32_t         Vnwd_GetAlgVersion          ( void                                              );
extern void             Vnwd_SetNoWearStation       ( uint8_t              vsw_set_state                );
extern sport_history_t  Vnwd_HistoryMomentum        ( uint32_t             vhm_momentum_temp        ,
                                                      uint32_t             vhm_steps_temp       
                                                    );
extern void             Vnwd_SetNoWearParameters    ( nowear_parameters_t* snp_nowear_info              );
extern void             Vnwd_SetNoWearOneVote       ( uint8_t              nowear_state                 );  
extern uint8_t          Vnwd_JudgeMovement          ( int16_t*             ajm_acc_tri_data             );                              
#endif  // !__NOWEAR_DETECT_H__





