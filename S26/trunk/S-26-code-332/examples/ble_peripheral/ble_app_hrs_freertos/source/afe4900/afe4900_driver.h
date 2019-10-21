/***************************Copyright(c)***********************************
**                       Veepoo Tech
**                  COPYRIGHT (C) 2016-2019
**-----------File Info-----------------------------------------------------
** File        name:     afe4900_driver.h
** Last modified Date:   2018-10-18
** Last SYS_VERSION:     0.0.0
** Descriptions:            
**-------------------------------------------------------------------------
** Created  by:          JSH
** Created  date:        2018-10-18
** SYS_VERSION:          0.0.0
** Descriptions:         
**-------------------------------------------------------------------------
** Modified by:				
** Modified date:  			          
** SYS_VERSION:                 
** Descriptions:                        
***************************************************************************/

#ifndef __AFE4900_DRIVER_H
#define __AFE4900_DRIVER_H

#include<stdint.h>


// afe4900 fifo读取并转换
void Afe4900_FifoReadConvert(uint8_t *data, uint32_t sample_cnt);

// afe4900 fifo数据获取
void Afe4900_FifoDateGet(int32_t *data_src, uint32_t sample_types, uint32_t sample_index, int32_t *data_get, uint32_t sample_cnt);

// afe4900 读寄存器并转换
int32_t Afe4900_RegisterReadConvert(uint8_t reg_address);

// afe4900 开始采样
void Afe4900_StartSampling(void);

// afe4900 停止采样
void Afe4900_StopSampling(void);

// afe4900 配置PPG模式-心率
void Afe4900_Ppg1Config(void);

// afe4900 配置PPG模式-血氧
void Afe4900_Ppg2Config(void);

void Afe4900_PpgRestConfig(void);

// afe4900 上电初始化
void Afe4900_PowerOnInit(void);

#endif

