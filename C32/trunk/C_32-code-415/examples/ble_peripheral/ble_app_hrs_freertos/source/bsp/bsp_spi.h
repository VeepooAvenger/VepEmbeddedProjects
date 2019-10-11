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
    
#define SPI_MAX_DATA            254     //SPI一次发送数据的最大个数,与平台相关（52832）
    
#define SPI_1_2_USE_DMA         1//0//
    
#if SPI_1_2_USE_DMA
#pragma pack(1)
typedef struct 
{
    uint8_t start_flg;
//    uint8_t end_flg; 
    uint8_t read_flg;
    uint8_t write_flg;
    
    uint8_t read_buf_flg;
    uint8_t write_buf_flg;
    
    uint16_t  now_read_len;
    uint32_t now_read_pos;
    uint32_t flash_addr;
    uint32_t data_len;
    uint16_t buf_len[2];
    uint8_t buf[2][256];
}spi_dma_param_t;
#pragma pack()

extern spi_dma_param_t spi_dma_param_s;

#endif //SPI_1_2_USE_DMA

uint8_t Spi12_Operation(const uint32_t address, const uint32_t data_len);    

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
