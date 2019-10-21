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
        
// ����˯��
void Sleep_NormalProcess(void);

// �͵�˯��
void Sleep_LowBateeryProcess(void);  
    
// ˯�߱�������    
void Sleep_TraversingOperation(void);
    
// ˯�ߵ���
void Sleep_DebugOverloadData(void);
        
#ifdef __CPLUSPLUS
}
#endif
    
#endif  //APP_SLEEP_H__
