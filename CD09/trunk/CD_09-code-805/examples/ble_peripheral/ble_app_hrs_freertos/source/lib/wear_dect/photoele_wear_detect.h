/********************************Copyright(c)***********************************
**                          Veepoo Tech
**                        COPYRIGHT (C) 2016
**--------------File Info-------------------------------------------------------
** File name:               photoelectronic_wear_dect.h
** Last modified Date:      2016-09-21
** Last SYS_VERSION:        01.01.01.03
** Created by:              HYR
** Descriptions:
**------------------------------------------------------------------------------
** Modified by:             HJJ
** Modified date:           2017-07-07 
** Descriptions:            01.01.01.04 modified the way of data updating and the type of  deta_diff
**------------------------------------------------------------------------------
** Modified by:             HJJ
** Modified date:           2017-07-10 
** Descriptions:            01.01.01.05 modified the copy data
**------------------------------------------------------------------------------
** Modified by:             HJJ
** Modified date:           2017-07-13 
** Descriptions:            01.01.01.06  modified the time  of data test
**------------------------------------------------------------------------------
** Modified by:             HJJ
** Modified date:           2017-07-20 
** Descriptions:            01.01.02.00 modified the version
**------------------------------------------------------------------------------
** Modified by:             HJJ
** Modified date:           2017-08-25 
** Descriptions:            01.01.03.00 modified the file's name to photoele_wear_detect
**------------------------------------------------------------------------------
*******************************************************************************/

#ifndef  _PHOTOELE_WEAR_DETECT_H_
#define  _PHOTOELE_WEAR_DETECT_H_

#include "stdint.h"
#include "stdlib.h"
#define WEAR_CHECK_LEVEL_MIN      0
#define WEAR_CHECK_LEVEL_ONE      1
#define WEAR_CHECK_LEVEL_TWO      2
#define WEAR_CHECK_LEVEL_THREE    3
#define WEAR_CHECK_LEVEL_FIVE     5
#define WEAR_CHECK_LEVEL_TWELVE   12

#define UNWEAR_CHECK_LEVEL_THREE  3
#define UNWEAR_CHECK_LEVEL_FIVE   5
#define UNWEAR_CHECK_LEVEL_SEVEN  7
#define UNWEAR_CHECK_LEVEL_TWELVE 12
#define UNWEAR_CHECK_LEVEL_TWENTY 20
/********************************************************************
** Function name:           Pwd_GetVerion
** Descriptions:
** input parameters:        none
** output parameters:       none
** Returned value:          PWD_VERSION
** Created by:              HYR
** Created Date:            2016-09-21
**-------------------------------------------------------------------
********************************************************************/
uint32_t Pwd_GetVerion(void);

/********************************************************************
** Function name:           Photoeleticity_WearDect
** Descriptions:            PWM wave detection 
** input parameters:        wear_test_timer
** output parameters:       none
** Returned value:          pwd_wear_result
** Created by:              HYR
** Created Date:            2016-09-18
**-------------------------------------------------------------------
** Modified by:             HYR
** Modified date:           2017-01-06
Add unwear level setting.
1. 12bit sample default 12
2. 10bit sample default 3
**-------------------------------------------------------------------
********************************************************************/
uint8_t Photoeleticity_WearDect(int16_t pwd_set_wear_level, uint16_t pwd_adc_value);

/********************************************************************
** Function name:           Photoeleticity_UnWearDect
** Descriptions:            
** input parameters:        wear_test_timer 
** output parameters:       none
** Returned value:          puwd_wear_station
** Created by:              HYR
** Created Date:            2016-09-19
**-------------------------------------------------------------------
** Modified by:             HYR
** Modified date:           2017-01-06
Add unwear level setting.
1. 12bit sample default 12
2. 10bit sample default 3
**-------------------------------------------------------------------
********************************************************************/
uint8_t Photoeleticity_UnWearDect(int16_t puwd_set_unwear_level, uint16_t pwd_adc_value);

/********************************************************************
** Function name:           Un_WearDetectAffirm
** Descriptions:            Not wearing a confirmation
** input parameters:        puwd_temp_buff 
** output parameters:       none
** Returned value:          puwd_wear_station
** Created by:              HJJ
** Created Date:            2017-05-17
**-------------------------------------------------------------------
** Modified by:           
** Modified date:           
**-------------------------------------------------------------------*/
uint8_t Un_WearDetectAffirm(const int16_t * puwd_temp_buff);

#endif 
