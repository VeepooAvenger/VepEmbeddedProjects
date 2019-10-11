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
 
//��ʼ��    
void Sleep_Init(void);
    
//����˯������    
void Sleep_SaveData(void);
 
//�͵��˳�˯��
void Sleep_LowBateeryProcess(void);    

// ˯�߼���
void Sleep_NormalProcess(void); 

//��ȡ˯��ʱ��    
uint16_t Sleep_GetTime(void);   

//˯�߱�������    
void Sleep_Repetition(void);
    
#ifdef __CPLUSPLUS
}
#endif
    
#endif  //APP_SLEEP_H__
