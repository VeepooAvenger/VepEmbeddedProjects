/***************************Copyright(c)***********************************
**                       Veepoo Tech
**                  COPYRIGHT (C) 2016-2019
**-----------File Info-----------------------------------------------------
** File        name:     bsp_button.h
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
#ifndef BSP_BTN_BLE_H__
#define BSP_BTN_BLE_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define APP_KEY_ENABLED 1           //WJ.2016.11.21

#define APP_BSP_KEY_NULL_PRESS      0x00
#define APP_BSP_KEY_LONG_PRESS      0x01
#define APP_BSP_KEY_SHORT_PRESS     0x02   
#define APP_BSP_KEY_DOUBLE_PRESS    0x03  
    
#define APP_BSP_KEY_ID_1    0x01
#define APP_BSP_KEY_ID_2    0x02
#define APP_BSP_KEY_ID_3    0x03    
    

#define APP_BSP_PRESS_LONG_3S           3
#define APP_BSP_PRESS_LONG_6S           6  
    
#pragma pack(1)
typedef struct{
    uint8_t key_id; 
    uint8_t key_state;
    uint8_t key_press_time;
}bsp_key_info_t;    
#pragma pack()

//按键初始化
void Button_Init(void);

//获取按键信息
bsp_key_info_t *Button_GetButtonInfo(void);

//长按释放处理
void Button_PressTime(void);

//获取按键状态
uint8_t Button_GetPressState(void);


#ifdef __cplusplus
}
#endif

#endif /* BSP_BTN_BLE_H__ */

