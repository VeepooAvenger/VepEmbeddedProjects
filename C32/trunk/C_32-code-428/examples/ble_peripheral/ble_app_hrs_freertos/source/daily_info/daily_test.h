/***************************Copyright(c)***********************************
**                       Veepoo Tech
**                  COPYRIGHT (C) 2016-2019
**-----------File Info-----------------------------------------------------
** File        name:     daily_test.h
** Last modified Date:   2017-08-01
** Last SYS_VERSION:     0.0.0
** Descriptions:            
**-------------------------------------------------------------------------
** Created  by:          WJ
** Created  date:        2017-08-01
** SYS_VERSION:          0.0.0
** Descriptions:         
**-------------------------------------------------------------------------
** Modified by:				
** Modified date:  			          
** SYS_VERSION:                 
** Descriptions:                        
***************************************************************************/
#ifndef __DAILY_TEST_H
#define __DAILY_TEST_H


#define DAILY_TEST_SLEEP                0
#define DAILY_TEST_SLEEP_WRITE_DATA     0

#if DAILY_TEST_SLEEP

void DailyTest_Init(void);
uint8_t  DailyTest_setData(uint8_t *data);

#endif

#endif  //end __DAILY_INFO_HSS

