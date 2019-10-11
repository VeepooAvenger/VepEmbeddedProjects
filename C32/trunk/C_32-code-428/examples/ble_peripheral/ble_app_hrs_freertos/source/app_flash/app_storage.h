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

//保存系统参数配置
void Storage_SaveSystemInfo(void);

//保存开关配置
void Storage_SaveSwitchInfo(void);

//获取系统参数配置    
uint8_t Storage_GeSystemInfot(uint8_t *system_info_data);
    
//获取开关配置
uint8_t Storage_GetSwitchInfo(uint8_t *sys_switch_data);
    
//初始化所有存储信息  
void Storage_AllInfoInit(void);
    
    
#ifdef __CPLUSPLUS
}
#endif

#endif /* APP_STORAGE_H */
