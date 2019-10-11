/***************************Copyright(c)***********************************
**                       Veepoo Tech
**                  COPYRIGHT (C) 2016-2019
**-----------File Info-----------------------------------------------------
** File        name:     heart.h
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
#ifndef APP_HEART_H__
#define APP_HEART_H__

#include <stdbool.h>
#include <stdint.h>
#include "board.h"

#define APP_ADC_HEART_ENABLED       1

#define MAX_HEART_BEAT              200
#define MIN_HEART_BEAT              30
#define HEART_VALID_DATA_NUM        5

typedef enum
{
    HEART_WORK_MODE_IDLE = 0,
    HEART_WORK_MODE_WAITE,
    HEART_WORK_MODE_BUSY
}heart_work_mode_t;

//清除心率平均值和计数器
void Heart_ClearAverValue(void);

void Heart_WorkMode_Set(heart_work_mode_t work_mode);
heart_work_mode_t Heart_WorkMode_Get(void);
uint8_t Heart_IsBusy(void);

void Heart_Open(void);
void Heart_Close(void);
void Heart_Calculate_Process(void);
void Heart_BleSend_TestInvalid(void);
void Heart_BleSend_Value(void);
uint8_t Heart_BleSend_ADC(void);
uint8_t Heart_Test_Value_Get(void);
void Heart_UICloseCheck(void);

/****************************************************************
** Function name:           Heart_GetAveragePulseRate
** Descriptions:            Get the average pulse rate for storage 
** input parameters:        none
** output parameters:       none
** Returned value:          heart_rate_aver
** Created by:              WJ              
** Created Date:            2017-07-21
**----------------------------------------------------------------
*****************************************************************/
uint8_t Heart_GetAveragePulseRate(void);

/****************************************************************
** Function name:           Heart_GetValidPulseRateNum
** Descriptions:            Get the valid pulse rate num 
** input parameters:        none
** output parameters:       none
** Returned value:          heart_rate_cnt
** Created by:              HYR              
** Created Date:            2017-07-21
**----------------------------------------------------------------
*****************************************************************/
uint8_t Heart_GetValidPulseRateNum(void);

#endif

         




                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
