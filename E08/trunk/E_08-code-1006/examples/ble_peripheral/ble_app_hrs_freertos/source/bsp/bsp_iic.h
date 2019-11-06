/*****************************************************************
Copyright: 2012-2016, Veepoo Tech. Co., Ltd.
File name:         bsp_iic.h
Description: 
Author:             WJ
Version: 
Date:             2016-09-26
History: 
*****************************************************************/
#ifndef BSP_IIC_H
#define BSP_IIC_H

#include "stdint.h"
#include "string.h"

#ifdef __cplusplus
extern "C" {
#endif

void Bsp_IIC0_Init(void);
uint8_t Bsp_IIC0_Write(const uint8_t address, const uint8_t *data, const uint16_t length);
uint8_t Bsp_IIC0_Read(const uint8_t address, uint8_t *data, uint16_t length);
void Bsp_IIC1_Init(void);
uint8_t Bsp_IIC1_Write(const uint8_t address, const uint8_t *data, const uint16_t length);
uint8_t Bsp_IIC1_Read(const uint8_t address, uint8_t *data, uint16_t length);
  
#ifdef __cplusplus
}
#endif

#endif  //BSP_IIC_H
