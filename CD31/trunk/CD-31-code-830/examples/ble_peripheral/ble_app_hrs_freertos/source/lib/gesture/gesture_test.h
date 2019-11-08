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
Modified:Version 030101_15 20170509.
1. modified the sleep gesture as raise hand
2. modified the gesture 11 param
Modified:Version 030101_16 20170509.
1. modified the sleep  raise hand thr part z less 100per G
2. modified leep  raise hand  turn condition
Modified:Version 030101_17 20170509.
1. modified the sleep  raise hand thr part X 50PER to 60
2. modified leep  raise hand  end std v 15 to 13
Modified:Version 030102_00 20170509.
official version
Modified:Version 030103_00 20170515.
1. Modifide the v signal double ckick function
Modified:Version 030105_00 20170519.
1. Modidied turn wrist function
Modified:Version 030106_00 20170608.
1. Modified double click
2. modified turn wrist
Modified:Version 030106_02 20170620.
1. Add condition to click confirm.
Modified:Version 03010700 20170710.
1. modified the turn wrist more easy
Modified:Version 03010702 20170713.
1. modified the turn wrist limit of xy
Modified:Version 03010703 20171018.  03010708 20171019.HYR 03010800 20171019.HYR
1.delete double click function and gesture 7
2.turn wrist adjust.
3.Change the close screen condition.
4.Official version
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

//Using data as 8G 12bit .20171018

/*******************************************************************************
** Function name:           Gesture_Detection16bit
** Descriptions:            Main entrance of the detecting function
** input parameters:        data_x_temp data_y_temp data_z_temp data_v_temp gd_sensitivity_level
** output parameters:       none
** Returned value:          none
** Created by:              HYR
** Created Date:            2017-04-18
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

