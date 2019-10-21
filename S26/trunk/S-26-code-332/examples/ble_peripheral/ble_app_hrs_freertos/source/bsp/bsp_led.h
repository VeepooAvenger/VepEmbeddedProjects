/*****************************************************************
Copyright: 2012-2016, Veepoo Tech. Co., Ltd.
File name:         bsp_led.h
Description: 
Author:             WJ
Version: 
Date:             2017-01-04
History: 
*****************************************************************/
#ifndef BSP_LED_H
#define BSP_LED_H

#include "stdint.h"
#include "string.h"

#ifdef __cplusplus
extern "C" {
#endif


void Bsp_LED_Init(void);    
void Bsp_LED_Gear(uint8_t gear);  

void Bsp_LED_Open(uint8_t leve);    
void Bsp_LED_Close(void);    
#ifdef __cplusplus
}
#endif

#endif  //BSP_LED_H
