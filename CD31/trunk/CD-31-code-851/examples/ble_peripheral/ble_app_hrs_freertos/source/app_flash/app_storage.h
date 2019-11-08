/***************************Copyright(c)***********************************
**                       Veepoo Tech
**                  COPYRIGHT (C) 2016-2019
**-----------File Info-----------------------------------------------------
** File        name:     app_storage.c
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
#ifndef APP_STORAGE_H
#define APP_STORAGE_H


#ifdef __CPLUSPLUS
extern "C" {
#endif

#define APP_STORAGE_SUCCESS       0x00
#define APP_STORAGE_FAILED        0x01

//����ϵͳ��������
void Storage_SaveSystemInfo(void);

//���濪������
void Storage_SaveSwitchInfo(void);

//��ȡϵͳ��������    
uint8_t Storage_GeSystemInfot(uint8_t *system_info_data);
    
//��ȡ��������
uint8_t Storage_GetSwitchInfo(uint8_t *sys_switch_data);
    
//��ʼ�����д洢��Ϣ  
void Storage_AllInfoInit(void);
    
    
#ifdef __CPLUSPLUS
}
#endif

#endif /* APP_STORAGE_H */
