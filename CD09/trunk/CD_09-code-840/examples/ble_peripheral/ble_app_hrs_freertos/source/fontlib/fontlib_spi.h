/***************************Copyright(c)***********************************
**                       Veepoo Tech
**                  COPYRIGHT (C) 2016-2019
**-----------File Info-----------------------------------------------------
** File        name:     fondlib_spi.h
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

/** Note:  字库SPI使用标准默认状态的设置（Word stock spi using standard set the default state）
*   如果共用SPI接口，注意CS的使用方法，需要考虑资源冲突; SPI 极性;SPI 模式(取样边沿).
*   If common SPI interface, pay attention to the use of the CS, 
*   the need to consider resource conflicts;SPI polarity;SPI mode sampling (edge).
**/


#ifndef _FONDLIB_SPI_H_
#define _FONDLIB_SPI_H_
#include <stdint.h>
#include <stdbool.h>
#include "HFMA2Ylib.h"

//字库初始化
void Fontlib_Init(void);

//字库唤醒
void Fontlib_Wakeup(void);

//字库休眠
void Fontlib_Sleep(void);

//Utf8 to GBK
uint16_t Font_Utf8ToGbk (uint8_t *utf8);  

//Utf8 to Unicode, Utf8 range around 0x80 to 0x7ff, to change Unicode
uint16_t Fontlib_Utf8ToUnicode(uint8_t *utf8);

//The test case 
void Fontlib_Test(void);

//获取点阵数据（ASIIC）
void Fontlib_AsciiLattice(uint8_t* p_lattice, uint32_t type, uint8_t ascii);

//获取点阵数据（中文）
uint8_t Fontlib_ChineseLattice(uint8_t* p_data, uint8_t* p_lattice);

//获取点阵数据（韩语）
void Fontlib_KoreanLattice(uint8_t* p_data, uint8_t* p_lattice);

//获取点阵数据（日文）
void Fontlib_JapaneseLattice(uint8_t* p_data, uint8_t* p_lattice);

//获取点阵数据（西里拉文）
void Fontlib_CyrillicLattice(uint8_t* p_data, uint8_t* p_lattice);

//获取点阵数据（拉丁文）
void Fontlib_LattinLattice(uint8_t* p_data, uint8_t* p_lattice);

#endif
