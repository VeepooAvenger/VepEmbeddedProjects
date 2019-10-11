/***************************Copyright(c)***********************************
**                       Veepoo Tech
**                  COPYRIGHT (C) 2016-2019
**-----------File Info-----------------------------------------------------
** File        name:     backlight_driving.h
** Last modified Date:   2018-08-07
** Last SYS_VERSION:     0.0.0
** Descriptions:            
**-------------------------------------------------------------------------
** Created  by:          WJ
** Created  date:        2018-08-07
** SYS_VERSION:          0.0.0
** Descriptions:         
**-------------------------------------------------------------------------
** Modified by:				
** Modified date:  			          
** SYS_VERSION:                 
** Descriptions:                        
***************************************************************************/

#ifndef APP_BACKLIGHT_DRIVING_H_
#define APP_BACKLIGHT_DRIVING_H_

#include "stdint.h"


#define BACKLIGHT_MAX_CHANAGE_LEVEL     12           //定义设备可支持最大调光等级



typedef struct
{
    uint8_t     state;      //是否开启了背光
    uint8_t     level;      //当前等级
}backlight_driving_info_t;

//背光调级
void Backlight_SetLeve(uint8_t set_level);

void Backlight_TestSetLeve(uint8_t set_level);

#endif // APP_BACKLIGHT_DRIVING_H_

