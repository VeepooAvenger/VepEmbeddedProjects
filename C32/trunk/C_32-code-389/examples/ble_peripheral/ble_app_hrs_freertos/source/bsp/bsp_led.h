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

//��ʼ����
void Led_Init(void);    

//���õƵȼ�
void Led_SetGear(uint8_t gear);  

//����
void Led_OpenByLevel(uint8_t leve);   

// �ص�
void Led_CloseAll(void);   

    
#ifdef __cplusplus
}
#endif

#endif  //BSP_LED_H
