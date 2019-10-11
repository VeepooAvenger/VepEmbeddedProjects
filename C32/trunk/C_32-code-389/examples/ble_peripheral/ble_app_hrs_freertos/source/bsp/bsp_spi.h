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
* �궨��
*/

/*********************************************************************
* ȫ�ֱ�������
*/

/*********************************************************************
* ��������
*/

#if SPI0_ENABLED    

//��ʼ��
void Bps_Spi0Init(void);

//д
uint8_t Bps_Spi0Write(const uint8_t *data, uint16_t length);

//��
uint8_t Bps_Spi0Read(uint8_t *data, uint16_t length); 

//��д
uint8_t Bps_Spi0ReadWrite(const uint8_t *input, uint16_t input_len, uint8_t *output, uint16_t output_len);

//SPIæµ
uint8_t Bps_Spi2SelectIsBusy(void);

//ѡ����ٶ�
void Bps_Spi2SelectGsensor(uint8_t flag);

//ѡ���ֿ�
void Bps_Spi2SelectGt24(uint8_t flag);

//SPIæµ
uint8_t Bps_Spi0SelectIsBusy(void);

//ѡ����ٶ�
void Bps_Spi0SelectGsensor(uint8_t flag);

////ѡ���ֿ�
//void Bps_Spi0SelectGt24(uint8_t flag);

    
#endif
    
#if SPI1_ENABLED    

//��ʼ��
void Bps_Spi1Init(void);

//д
uint8_t Bps_Spi1Write(const uint8_t *data, uint16_t length);

//��
uint8_t Bps_Spi1Read(uint8_t *data, uint16_t length); 

#endif 

#if SPI2_ENABLED    

//��ʼ��
void Bps_Spi2Init(void);

//д
uint8_t Bps_Spi2Write(const uint8_t *data, uint16_t length);

//��
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
