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
#include "app_type.h"
#include "app_flash.h"
#include "app_storage.h"
#include "daily_test.h"
#include "daily_info.h"

/*********************************************************************
* Macro Definition
*/

/*********************************************************************
* Global Variables
*/

extern system_flag_t system_flag_s;
extern system_switch_t system_switch_s;
extern system_info_t system_info_s;

/*********************************************************************
* local functions
*/

/****************************************************************
** Function name:           Storage_SaveSwitchInfo
** Descriptions:            
** input parameters:
** output parameters:
** Returned value:
** Created by:              WJ              
** Created Date:            2016-12-05
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Storage_SaveSwitchInfo(void)
{     
    // 开关存储
    App_FlashWriteSystemDataRecord(system_record_s[RECORD_SYSTEM_SWITCH], (void *)&system_switch_s, sizeof(system_switch_t));
    
}

/****************************************************************
** Function name:           Storage_SaveSwitchInfo
** Descriptions:            
** input parameters:
** output parameters:
** Returned value:
** Created by:              WJ              
** Created Date:            2016-12-05
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
uint8_t Storage_GetSwitchInfo(uint8_t *sys_switch_data)
{ 
    uint32_t ret = 0;
    
    ret = App_FlashReadSystemDataRecord(system_record_s[RECORD_SYSTEM_SWITCH], sys_switch_data, sizeof(system_switch_t));    
    
    if(ret == APP_FLASH_SUCCESS)
    {
        return APP_STORAGE_SUCCESS;
    }        
    return APP_STORAGE_FAILED;
}

/****************************************************************
** Function name:           Storage_SaveSystemInfo
** Descriptions:            
** input parameters:
** output parameters:
** Returned value:
** Created by:              WJ              
** Created Date:            2016-12-05
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Storage_SaveSystemInfo(void)
{
    // 
    App_FlashWriteSystemDataRecord(system_record_s[RECORD_SYSTEM_DATA], (void *)&system_info_s, sizeof(system_info_t));

}

/****************************************************************
** Function name:           Storage_GeSystemInfot
** Descriptions:            
** input parameters:
** output parameters:
** Returned value:
** Created by:              WJ              
** Created Date:            2016-12-05
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
uint8_t Storage_GeSystemInfot(uint8_t *system_info_data)
{ 
    uint32_t ret = 0;
    
    ret = App_FlashReadSystemDataRecord(system_record_s[RECORD_SYSTEM_DATA], system_info_data, sizeof(system_info_t));    
    
    if(ret == APP_FLASH_SUCCESS)
    {
        return APP_STORAGE_SUCCESS;
    }        
    return APP_STORAGE_FAILED;
}

/****************************************************************
** Function name:           Storage_TimeInfoInit
** Descriptions:            
** input parameters:
** output parameters:
** Returned value:
** Created by:              WJ              
** Created Date:            2016-12-05
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Storage_TimeInfoInit(void)
{   
    uint8_t storage_time_data[8] = {0x00,};
    uint32_t ret = 0;
      
    // 读取时间
    ret = App_FlashReadSystemDataRecord(system_record_s[RECORD_SYSTEM_TIME], storage_time_data, 8);    
    if(ret == APP_FLASH_SUCCESS)
    {
        if(Sclock_TimeCheck(storage_time_data, sizeof(system_clock_time_t) - 1 ))
        {
            Sclock_SynchronizationTime(storage_time_data, sizeof(system_clock_time_t) - 1);
            Sclock_CompensationTime(1); //复位对时间补偿1s
        }     
    }  
}

/****************************************************************
** Function name:           Storage_AllInfoInit
** Descriptions:            
** input parameters:
** output parameters:
** Returned value:
** Created by:              WJ              
** Created Date:            2016-12-05
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Storage_AllInfoInit(void)
{  
#if DAILY_TEST_SLEEP       	
    DailyTest_Init();
#else    
    Storage_TimeInfoInit();      
#endif    
    Storage_GetSwitchInfo((void *)&system_switch_s);
    Storage_GeSystemInfot((void *)&system_info_s);   
    Daily_InitFromFlash();
}


