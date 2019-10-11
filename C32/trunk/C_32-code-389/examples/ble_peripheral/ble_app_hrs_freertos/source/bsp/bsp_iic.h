/***************************Copyright(c)***********************************
**                       Veepoo Tech
**                  COPYRIGHT (C) 2016-2019
**-----------File Info-----------------------------------------------------
** File        name:     bsp_iic.h
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
#ifndef BSP_IIC_H
#define BSP_IIC_H

#include "stdint.h"
#include "string.h"

#ifdef __cplusplus
extern "C" {
#endif

//初始化 
void Bsp_IIC0Init(void);

//写数据
uint8_t Bsp_IIC0Write(const uint8_t address, const uint8_t *data, const uint16_t length);

//读数据
uint8_t Bsp_IIC0Read(const uint8_t address, uint8_t *data, uint16_t length);

//初始化
void Bsp_IIC1Init(void);

//写数据
uint8_t Bsp_IIC1Write(const uint8_t address, const uint8_t *data, const uint16_t length);

//读数据
uint8_t Bsp_IIC1Read(const uint8_t address, uint8_t *data, uint16_t length);
  
#ifdef __cplusplus
}
#endif

#endif  //BSP_IIC_H
