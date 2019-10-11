/***************************Copyright(c)***********************************
**                       Veepoo Tech
**                  COPYRIGHT (C) 2016-2019
**-----------File Info-----------------------------------------------------
** File        name:     sleep.h
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
#ifndef APP_SLEEP_H__
#define APP_SLEEP_H__

#include "Vph_SleepAlgorithm.h"

#ifdef __CPLUSPLUS
extern "C" {
#endif 
 
//初始化    
void Sleep_Init(void);
    
//保存睡眠数据    
void Sleep_SaveData(void);
 
//低电退出睡眠
void Sleep_LowBateeryProcess(void);    

// 睡眠计算
void Sleep_NormalProcess(void); 

//获取睡眠时间    
uint16_t Sleep_GetTime(void);   

//睡眠遍历计算    
void Sleep_Repetition(void);
    
#ifdef __CPLUSPLUS
}
#endif
    
#endif  //APP_SLEEP_H__
