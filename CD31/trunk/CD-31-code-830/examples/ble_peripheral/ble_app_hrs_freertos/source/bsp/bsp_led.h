/***************************Copyright(c)***********************************
**                       Veepoo Tech
**                  COPYRIGHT (C) 2016-2019
**-----------File Info-----------------------------------------------------
** File        name:     bsp_led.h
** Last modified Date:   2017-07-28
** Last SYS_VERSION:     0.0.0
** Descriptions:            
**-------------------------------------------------------------------------
** Created  by:          WJ
** Created  date:        2017-07-28
** SYS_VERSION:          0.0.0
** Descriptions:         
**-------------------------------------------------------------------------
** Modified by:				
** Modified date:  			          
** SYS_VERSION:                 
** Descriptions:                        
***************************************************************************/
#ifndef BSP_LED_H
#define BSP_LED_H

#include "stdint.h"
#include "string.h"

#ifdef __cplusplus
extern "C" {
#endif

//初始化灯
void Led_Init(void);    

//设置灯等级
void Led_SetGear(uint8_t gear);  

//开灯
void Led_OpenByLevel(uint8_t leve);   

// 关灯
void Led_CloseAll(void);   

    
#ifdef __cplusplus
}
#endif

#endif  //BSP_LED_H
