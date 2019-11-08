/***************************Copyright(c)***********************************
**                       Veepoo Tech
**                  COPYRIGHT (C) 2016-2019
**-----------File Info-----------------------------------------------------
** File        name:     i2c.h
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

#ifndef __I2C_H__
#define __I2C_H__

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

//初始化
bool I2c_Init(void);

//写数据
bool I2c_Write(uint8_t address, uint8_t *data, uint8_t bytes);

//读数据
bool I2c_Read(uint8_t address, uint8_t *data, uint8_t bytes);


//初始化
bool I2c1_Init(void);

//写数据
bool I2c1_Write(uint8_t address, uint8_t *data, uint8_t bytes);

//读数据
bool I2c1_Read(uint8_t address, uint8_t *data, uint8_t bytes);
#endif
