/***************************Copyright(c)***********************************
**                       Veepoo Tech
**                  COPYRIGHT (C) 2016-2019
**-----------File Info-----------------------------------------------------
** File        name:     calc_data_type.h
** Last modified Date:   2018-10-18
** Last SYS_VERSION:     0.0.0
** Descriptions:            
**-------------------------------------------------------------------------
** Created  by:          WJ
** Created  date:        2018-10-19
** SYS_VERSION:          0.0.0
** Descriptions:         
**-------------------------------------------------------------------------
** Modified by:				
** Modified date:  			          
** SYS_VERSION:                 
** Descriptions:                        
***************************************************************************/

#ifndef _CALC_DATA_TYPE_H_
#define _CALC_DATA_TYPE_H_

#include <stdint.h>

// 4900 配置模式
typedef enum
{
    MEASURE_MODE_NULL = 0,          // 测量未开启
    MEASURE_MODE_REST,              // ppg模式-上电模式
    MEASURE_MODE_PPG1,              // ppg模式-心率
    MEASURE_MODE_PPG2,              // ppg模式-血氧
    
}MEASURE_MOD_E;

#endif  //_CALC_DATA_TYPE_H_

