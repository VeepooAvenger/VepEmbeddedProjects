/***************************Copyright(c)***********************************
**                       Veepoo Tech
**                  COPYRIGHT (C) 2016-2019
**-----------File Info-----------------------------------------------------
** File        name:     app_task.h
** Last modified Date:   2017-08-24
** Last SYS_VERSION:     0.0.0
** Descriptions:            
**-------------------------------------------------------------------------
** Created  by:          WJ
** Created  date:        2017-08-24
** SYS_VERSION:          0.0.0
** Descriptions:         
**-------------------------------------------------------------------------
** Modified by:				
** Modified date:  			          
** SYS_VERSION:                 
** Descriptions:                        
***************************************************************************/
#ifndef APP_TASK_H
#define APP_TASK_H
/*
BLE_STACK_SUPPORT_REQD __STACK_SIZE=3092 __HEAP_SIZE=2048 S132 NRF_SD_BLE_API_VERSION=3 BOARD_CUSTOM NRF52_PAN_12 NRF52_PAN_15 NRF52_PAN_20 NRF52_PAN_30 NRF52_PAN_31 NRF52_PAN_36 NRF52_PAN_51 NRF52_PAN_53 NRF52_PAN_54 NRF52_PAN_55 NRF52_PAN_58 NRF52_PAN_62 NRF52_PAN_63 NRF52_PAN_64 xCONFIG_GPIO_AS_PINRESET SOFTDEVICE_PRESENT NRF52832 NRF52 FREERTOS USE_APP_CONFIG CONFIG_NFCT_PINS_AS_GPIOS DEBUG
*/
#include <stdint.h>

#ifdef __CPLUSPLUS
extern "C" {
#endif

//������
void App_Thread(void * arg);

//����¼�
void App_AddEvent(uint32_t event);

//ɾ���¼�
void App_DeleteEvent(uint32_t event);

//һ�����¼�����
void App_OneSecondTrigger(void);

//��ʼ��ϵͳ��־
void App_SystemFlagInit(void);

//�����¼�����
uint8_t App_PerformEvent(void); 

// �����Լ�
void App_CheckAllSetting(void);  

 
#ifdef __CPLUSPLUS
}
#endif

#endif /* APP_TASK_H */
