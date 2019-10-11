/***************************Copyright(c)***********************************
**                       Veepoo Tech
**                  COPYRIGHT (C) 2016-2019
**-----------File Info-----------------------------------------------------
** File        name:     outsit.h
** Last modified Date:   2017-08-21
** Last SYS_VERSION:     0.0.0
** Descriptions:            
**-------------------------------------------------------------------------
** Created  by:          WJ
** Created  date:        2017-08-21
** SYS_VERSION:          0.0.0
** Descriptions:         
**-------------------------------------------------------------------------
** Modified by:				
** Modified date:  			          
** SYS_VERSION:                 
** Descriptions:                        
***************************************************************************/
#ifndef BLE_OUTSIT_H__
#define BLE_OUTSIT_H__

#include "stdint.h"

typedef struct
{
    uint8_t start_time[2];
    uint8_t end_time[2];
    uint8_t threshold_value;
    uint8_t on_off_flag;
    uint8_t outsit_cnt;
}outsit_t;

uint8_t Outsit_Step_time( uint8_t  *minute , uint8_t step );

void Outsit_Monitor(void);
void Outsit_Init(void);

uint8_t Outsit_ConfigCheck(outsit_t *outsit_t);

#endif 
