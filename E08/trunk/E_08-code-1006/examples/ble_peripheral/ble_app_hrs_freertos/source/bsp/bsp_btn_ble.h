/* Copyright (c) 2014 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is property of Nordic Semiconductor ASA.
 * Terms and conditions of usage are described in detail in NORDIC
 * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 *
 */

/**@file
 *
 * @defgroup bsp_btn_ble BSP: BLE Button Module
 * @{
 * @ingroup bsp
 *
 * @brief Module for controlling BLE behavior through button actions.
 *
 * @details The application must propagate BLE events to the BLE Button Module.
 * Based on these events, the BLE Button Module configures the Board Support Package
 * to generate BSP events for certain button actions. These BSP events should then be
 * handled by the application's BSP event handler.
 *
 */

#ifndef BSP_BTN_BLE_H__
#define BSP_BTN_BLE_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif
    
/**@brief BLE Button Module error handler type. */
typedef void (*bsp_btn_ble_error_handler_t) (uint32_t nrf_error);
   
void Buttons_Init(void);

void Buttons_PressCalcTime(void);

uint8_t Buttons_GetPressState(void);

uint16_t Buttons_GetPressTime(void);      
    
void Buttons_Disable(void);

void Buttons_Enable(void);

bool Buttons_IsEnable(void);
    
#ifdef __cplusplus
}
#endif

#endif /* BSP_BTN_BLE_H__ */

/** @} */
