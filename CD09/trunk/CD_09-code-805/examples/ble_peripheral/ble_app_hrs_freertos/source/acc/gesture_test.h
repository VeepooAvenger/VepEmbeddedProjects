/********************************Copyright(c)***********************************
**                           Veepoo Tech
**                         COPYRIGHT (C) 2015-2018
**--------------File Info--------------------------------------------------------------------------
** File name:               gesture_test.c
** Last modified Date:      2015.10.29
** Last SYS_VERSION:        02.01.06.00
** Descriptions:
gesture setting:
Get_Gesture = 5 Raise hand
Get_Gesture = 6 Turn wrist
Get_Gesture = 11 put down station
gesture_close_screen_flag = 1 / 2  close the screen

Modified:Version 020200_00 20170314.HYR Add double click gesture
Modified:Version 030101_05 20170420.
1. Change the stable check param
2. Change the raise hand param.
***************************************************************************************/
#ifndef __GESTURE_H_
#define __GESTURE_H_

#include "stdint.h"
#include "stdbool.h"

#define MAX_DOUBLE_CLICK_TIME_MS  500
#define MIN_DOUBLE_CLICK_TIME_MS  60

typedef struct
{
	uint8_t gesture_mode;
	uint8_t close_screen_flag;
	uint8_t double_click;
	uint8_t mode_sleep;
}gesture_mode_info_t;

/*******************************************************************************
** Function name:           Gesture_Detection16bit
** Descriptions:            Main entrance of the detecting function
** input parameters:        data_x_temp data_y_temp data_z_temp data_v_temp gd_sensitivity_level
** output parameters:       none
** Returned value:          none
** Created by:              HYR
** Created Date:            2017-04-18
**------------------------------------------------------------------------------
** Modified by:              
** Modified date:            
**------------------------------------------------------------------------------
*******************************************************************************/
void Gesture_Detection16bit(int16_t *data_x_temp, int16_t *data_y_temp, int16_t *data_z_temp, int16_t *data_v_temp, uint8_t gd_sensitivity_level);

/*******************************************************************************
** Function name:           Gesture_GetMode16bit
** Descriptions:            Return the gesture detective result
** input parameters:        none
** output parameters:       none
** Returned value:          gesture_mode
** Created by:              HYR
** Created Date:            2017-04-18
**------------------------------------------------------------------------------
*******************************************************************************/
uint8_t Gesture_GetMode16bit(void);

/*******************************************************************************
** Function name:           Get_GestureVersion16bit
** Descriptions:            Return the gesture detection fucntion versrion
** input parameters:        none
** output parameters:       none
** Returned value:          GESTURE_LIB_VERSION
** Created by:              HYR
** Created Date:            2017-04-18
**------------------------------------------------------------------------------
*******************************************************************************/
uint32_t Get_GestureVersion16bit(void);

/*******************************************************************************
** Function name:           Gesture_GetCloseScreenFlag16bit
** Descriptions:            Fucntion get the close screen flag.Screen should be close when flag = 1/2.
** input parameters:        none
** output parameters:       none
** Returned value:          gesture_close_screen_flag
** Created by:              HYR
** Created Date:            2017-04-18
**------------------------------------------------------------------------------
*******************************************************************************/
uint8_t Gesture_GetCloseScreenFlag16bit(void);

/*******************************************************************************
** Function name:           Gesture_GetDoubleClickFlag16bit
** Descriptions:            Fucntion get the double click flag.Return 1 when double click is done.
** input parameters:        none
** output parameters:       none
** Returned value:          gt_double_click_flag
** Created by:              HYR
** Created Date:            2017-04-18
**------------------------------------------------------------------------------
*******************************************************************************/
uint8_t Gesture_GetDoubleClickFlag16bit(void);

/*******************************************************************************
** Function name:           Gesture_GetGestureForSleep16bit
** Descriptions:            Output an gesture for sleep judge
** input parameters:        none
** output parameters:       none
** Returned value:          gesture_for_sleep
** Created by:              HYR
** Created Date:            2017-04-18
**------------------------------------------------------------------------------
*******************************************************************************/
uint8_t Gesture_GetGestureForSleep16bit(void);

/*******************************************************************************
** Function name:           Gesture_GetGestureForSleep16bit
** Descriptions:            Get all gesture
** input parameters:        none
** output parameters:       ggag_gesture_info
** Returned value:          gesture_for_sleep
** Created by:              HYR
** Created Date:            2017-04-18
**------------------------------------------------------------------------------
*******************************************************************************/
void Gesture_GetAllGesture16bit(gesture_mode_info_t * ggag_gesture_info);

/*-----------------------------------------8bit gesture------------------------------------------------------*/

/*******************************************************************************
** Function name:           Gesture_Detection
** Descriptions:            Main entrance of the detecting function
** input parameters:        none
** output parameters:       none
** Returned value:          none
** Created by:              HYR
** Created Date:            2015-10-29
**------------------------------------------------------------------------------
** Modified by:             HYR
** Modified date:           20160613.
**------------------------------------------------------------------------------
*******************************************************************************/
void Gesture_Detection(int8_t *data_x_temp, int8_t *data_y_temp, int8_t *data_z_temp, uint8_t gd_sensitivity_level);

/*******************************************************************************
** Function name:           Gesture_GetMode
** Descriptions:            Return the gesture detective result
** input parameters:        none
** output parameters:       none
** Returned value:          gesture_mode
** Created by:              HYR
** Created Date:            2015-10-29
**------------------------------------------------------------------------------
*******************************************************************************/
uint8_t Gesture_GetMode(void);

/*******************************************************************************
** Function name:           Get_GestureVersion
** Descriptions:            Return the gesture detection fucntion versrion
** input parameters:        none
** output parameters:       none
** Returned value:          GESTURE_LIB_VERSION
** Created by:              HYR
** Created Date:            2015-10-29
**------------------------------------------------------------------------------
*******************************************************************************/
uint32_t Get_GestureVersion(void);

/*******************************************************************************
** Function name:           Gesture_GetCloseScreenFlag
** Descriptions:            Fucntion get the close screen flag.Screen should be close when flag = 1/2.
** input parameters:        none
** output parameters:       none
** Returned value:          gesture_close_screen_flag
** Created by:              HYR
** Created Date:            2017-02-27
**------------------------------------------------------------------------------
*******************************************************************************/
uint8_t Gesture_GetCloseScreenFlag(void);

/*******************************************************************************
** Function name:           Gesture_GetDoubleClickFlag
** Descriptions:            Fucntion get the double click flag.Return 1 when double click is done.
** input parameters:        none
** output parameters:       none
** Returned value:          gt_double_click_flag
** Created by:              HYR
** Created Date:            2017-03-13
**------------------------------------------------------------------------------
*******************************************************************************/
bool Gesture_GetDoubleClickFlag(void);

/*******************************************************************************
** Function name:           Gesture_GetGestureForSleep
** Descriptions:            Output an gesture for sleep judge
** input parameters:        none
** output parameters:       none
** Returned value:          gesture_for_sleep
** Created by:              HYR
** Created Date:            2017-03-14
**------------------------------------------------------------------------------
*******************************************************************************/
uint8_t Gesture_GetGestureForSleep(void);

#endif





