/*****************************************************************
Copyright: 2012-2016, Veepoo Tech. Co., Ltd.
File name:         bsp_spi.h
Description: 
Author:             WJ
Version: 
Date:             2016-09-14
History: 
*****************************************************************/
#ifndef BSP_SPI_H
#define BSP_SPI_H

#include "stdint.h"
#include "string.h"
#include "app_config.h"

#ifdef __cplusplus
extern "C" {
#endif

/*********************************************************************
* 宏定义
*/

/*********************************************************************
* 全局变量声明
*/

/*********************************************************************
* 函数声明
*/

#if SPI0_ENABLED    
void Bps_Spi0Init(void);
uint8_t Bps_Spi0Write(const uint8_t *data, uint16_t length);
uint8_t Bps_Spi0Read(uint8_t *data, uint16_t length); 
uint8_t Bps_Spi0ReadWrite(const uint8_t *input, uint16_t input_len, uint8_t *output, uint16_t output_len);    
#endif
    
#if SPI1_ENABLED    
void Bps_Spi1Init(void);
uint8_t Bps_Spi1Write(const uint8_t *data, uint16_t length);
uint8_t Bps_Spi1Read(uint8_t *data, uint16_t length); 
uint8_t Bps_Spi1ReadWrite(const uint8_t *input, uint16_t input_len, uint8_t *output, uint16_t output_len);    
#endif

#if SPI2_ENABLED    
void Bps_Spi2Init(void);
void Bps_Spi2UnInit(void);

uint8_t Bps_Spi2Write(const uint8_t *data, uint16_t length);
uint8_t Bps_Spi2Read(uint8_t *data, uint16_t length); 
uint8_t Bps_Spi2ReadWrite(const uint8_t *input, uint16_t input_len, uint8_t *output, uint16_t output_len);   
void Bps_Spi2SelectGt24(uint8_t flag);
void Bps_Spi2SelectFlash(uint8_t flag);
void Bps_Spi2SelectLcd(uint8_t flag);
uint8_t Bps_Spi2SelectIsBusy(void);
#endif


uint8_t Bps_SpiSelectIsBusy(void);
void Bps_SpiSelectGsensor(uint8_t flag);
void Bps_SpiSelectGt24(uint8_t flag);
#ifdef __cplusplus
}
#endif

#endif
