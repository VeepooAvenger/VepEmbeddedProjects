/*****************************************************************
Copyright: 2012-2016, Veepoo Tech. Co., Ltd.
File name: 		bsp_spi.h
Description: 
Author: 			WJ
Version: 
Date: 			2016-09-14
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

//初始化
void Bps_Spi0Init(void);

//写
uint8_t Bps_Spi0Write(const uint8_t *data, uint16_t length);

//读
uint8_t Bps_Spi0Read(uint8_t *data, uint16_t length); 

//读写
uint8_t Bps_Spi0ReadWrite(const uint8_t *input, uint16_t input_len, uint8_t *output, uint16_t output_len);

//SPI忙碌
uint8_t Bps_Spi2SelectIsBusy(void);

//选择加速度
void Bps_Spi2SelectGsensor(uint8_t flag);

//选择字库
void Bps_Spi2SelectGt24(uint8_t flag);

//SPI忙碌
uint8_t Bps_Spi0SelectIsBusy(void);

//选择加速度
void Bps_Spi0SelectGsensor(uint8_t flag);

////选择字库
//void Bps_Spi0SelectGt24(uint8_t flag);

    
#endif
    
#if SPI1_ENABLED    

//初始化
void Bps_Spi1Init(void);

//写
uint8_t Bps_Spi1Write(const uint8_t *data, uint16_t length);

//读
uint8_t Bps_Spi1Read(uint8_t *data, uint16_t length); 

#endif 

#if SPI2_ENABLED    

//初始化
void Bps_Spi2Init(void);

//写
uint8_t Bps_Spi2Write(const uint8_t *data, uint16_t length);

//读
uint8_t Bps_Spi2Read(uint8_t *data, uint16_t length); 

//
uint8_t Bps_Spi2SelectIsBusy(void);
//
void Bps_Spi2SelectFlash(uint8_t flag);
//
void Bps_Spi2SelectGt24(uint8_t flag);
    
#endif 
    
#ifdef __cplusplus
}
#endif

#endif
