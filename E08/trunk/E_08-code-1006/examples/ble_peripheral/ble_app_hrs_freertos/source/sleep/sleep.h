/*****************************************************************
Copyright: 2012-2016, Veepoo Tech. Co., Ltd.
File name:         sleep.h
Description: 
Author:             WJ
Version: 
Date:             2017-01-04
History: 
*****************************************************************/
#ifndef APP_SLEEP_H__
#define APP_SLEEP_H__

#include "Acculate_SleepAlgorithm.h"
#include "Acculate_CurrentState.h"

#define  SLEEP_CALC_ENABLED   1


#define  SLEEP_DEBUG          0
#define  SLEEP_DEBUG_1        0
#define  SLEEP_DEBUG_1_1      0
#define  SLEEP_DEBUG_1_2      0
#define  SLEEP_DEBUG_2        0

#define  SLEEP_CALC_DEBUG     (SLEEP_DEBUG|SLEEP_DEBUG_1|SLEEP_DEBUG_2)


#ifdef __CPLUSPLUS
extern "C" {
#endif 

#if SLEEP_CALC_ENABLED    
    
void Sleep_Init(void);
        
// 正常睡眠
void Sleep_NormalProcess(void);

// 低电睡眠
void Sleep_LowBateeryProcess(void);  
    
// 睡眠遍历操作    
void Sleep_TraversingOperation(void);
    
// 睡眠遍历，用于上电复位
void Sleep_TraversingOperationByRest(void);    
    
// 睡眠调试
void Sleep_DebugOverloadData(void);
        
void Sleep_Test(void);    

void Sleep_TestWriteDailyData(void);

void Sleep_TestWriteSleepInfoData(void);
 
#endif 
    
#ifdef __CPLUSPLUS
}
#endif
    
#endif  //APP_SLEEP_H__
