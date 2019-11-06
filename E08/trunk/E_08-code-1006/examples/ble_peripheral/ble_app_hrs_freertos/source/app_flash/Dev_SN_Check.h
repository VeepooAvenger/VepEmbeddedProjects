#ifndef _DEV_SN_CHECK_H_
#define _DEV_SN_CHECK_H_

#include <stdint.h>
#include "fstorage.h"
#include "flash_data_type.h"

/* 设备sn序列号过滤处理 */
void Dev_SNCheck(product_infomation_t *p_product);

uint32_t Dev_GetProductionInfomation(product_infomation_t *p_product);

uint32_t Dev_GetDevID(product_infomation_t *p_product);

void Dev_DFUPinConfig(uint8_t LD0_EN,uint8_t GA1,uint8_t GA2,uint8_t GA3);

uint8_t Dev_SetBleAdvName(uint8_t* ble_name);

void Dev_CheckBleAdvName(void);


void Dev_SetSitFunction(uint8_t sit_function);
uint8_t Dev_GetSitFunction(void);

void Dev_SetBpFunction(uint8_t sit_function);
uint8_t Dev_GetBpFunction(void);

#endif

