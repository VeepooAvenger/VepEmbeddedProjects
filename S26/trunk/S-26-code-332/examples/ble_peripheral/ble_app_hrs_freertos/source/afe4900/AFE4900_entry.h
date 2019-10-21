/***************************Copyright(c)***********************************
**                       Veepoo Tech
**                  COPYRIGHT (C) 2016-2019
**-----------File Info-----------------------------------------------------
** File        name:     afe4900_entry.h
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

#ifndef __AFE4900_ENTRY_H
#define __AFE4900_ENTRY_H

#include "nrf_drv_gpiote.h"
#include "calc_data_type.h"

//#pragma pack(4)

//typedef struct
//{
//    int reg_value00;
//    int reg_value01;
//    int reg_value02;
//    int reg_value03;
//    int reg_value04;
//    int reg_value05;
//    int reg_value06;
//    int reg_value07;
//    int reg_value08;
//    int reg_value09;
//    int reg_value0A;
//    int reg_value0B;
//    int reg_value0C;
//    int reg_value0D;
//    int reg_value0E;
//    int reg_value0F;
//    int reg_value10;
//    int reg_value11;
//    int reg_value12;
//    int reg_value13;
//    int reg_value14;
//    int reg_value1D;
//    int reg_value1E;
//    int reg_value1F;
//    int reg_value20;
//    int reg_value21;
//    int reg_value22;
//    int reg_value23;
//    int reg_value24;
//    int reg_value28;
//    int reg_value29;
//    int reg_value2A;
//    int reg_value2B;
//    int reg_value2C;
//    int reg_value2D;
//    int reg_value2E;
//    int reg_value2F;
//    int reg_value31;
//    int reg_value34;
//    int reg_value35;
//    int reg_value36;
//    int reg_value37;
//    int reg_value39;
//    int reg_value3A;
//    int reg_value3B;
//    int reg_value3C;
//    int reg_value3D;
//    int reg_value3E;
//    int reg_value3F;
//    int reg_value40;
//    int reg_value42;
//    int reg_value43;
//    int reg_value44;
//    int reg_value45;
//    int reg_value46;
//    int reg_value47;
//    int reg_value48;
//    int reg_value49;
//    int reg_value4A;
//    int reg_value4B;
//    int reg_value4E;
//    int reg_value50;
//    int reg_value51;
//    int reg_value52;
//    int reg_value53;
//    int reg_value57;
//    int reg_value58;
//    int reg_value60;
//    int reg_value64;
//    int reg_value65;
//    int reg_value66;
//    int reg_value67;
//    int reg_value68;
//    int reg_value69;
//    int reg_value6A;
//    int reg_value6B;
//    int reg_value6C;
//    int reg_value6D;
//    int reg_value72;
//    int reg_value73;
//    int reg_valueC8;
//}tset_4900_read_t;
//#pragma pack()


// afe4900 PPG 中断处理函数-心率
void Afe4900_Ppg1Isr(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action);

// afe4900 PPG 中断处理函数-血氧
void Afe4900_Ppg2Isr(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action);

void Afe4900_PpgRestIsr(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action);

// afe4900 关闭，进入低功耗
void Afe4900_Close(void);

// 打开4900，根据模式对应配置
void Afe4900_OpenMode(MEASURE_MOD_E mode);

void Afe4900_Check(void);

bool Afe4900_IsOpen(void);

#endif
