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

#include "Vph_SleepAlgorithm.h"

#ifdef __CPLUSPLUS
extern "C" {
#endif 
    
void Sleep_Init(void);
        
// Õý³£Ë¯Ãß
void Sleep_NormalProcess(void);

// µÍµçË¯Ãß
void Sleep_LowBateeryProcess(void);  
    
// Ë¯Ãß±éÀú²Ù×÷    
void Sleep_TraversingOperation(void);
    
// Ë¯Ãßµ÷ÊÔ
void Sleep_DebugOverloadData(void);
        
#ifdef __CPLUSPLUS
}
#endif
    
#endif  //APP_SLEEP_H__
