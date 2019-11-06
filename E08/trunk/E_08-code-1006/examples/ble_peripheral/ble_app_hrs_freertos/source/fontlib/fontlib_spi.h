/*****************************************************************
Copyright: 2012-2016, Veepoo Tech. Co., Ltd.
File name:         fondlib_spi.h
Description: 
Author:             WJ
Version: 
Date:             2016-11-14
History: 
*****************************************************************/
//ANSI Coding
/** Note:  �ֿ�SPIʹ�ñ�׼Ĭ��״̬�����ã�Word stock spi using standard set the default state��
*   �������SPI�ӿڣ�ע��CS��ʹ�÷�������Ҫ������Դ��ͻ; SPI ����;SPI ģʽ(ȡ������).
*   If common SPI interface, pay attention to the use of the CS, 
*   the need to consider resource conflicts;SPI polarity;SPI mode sampling (edge).
**/


#ifndef _FONDLIB_SPI_H_
#define _FONDLIB_SPI_H_
#include <stdint.h>
#include <stdbool.h>
#include "HFMA2Ylib.h"

#define UTF8_TO_GBK(utf8)       (uint16_t)(((*utf8&0x0F)<<4|(((*(utf8+1))>>2)&0x0F))<<8|(((*(utf8+1))&0x03)<<6|((*(utf8+2))&0x3F))) 
#define UTF8_TO_UNICODE(utf8)   (uint16_t)((*utf8&0x1F)<<6|((*(utf8+1))&0x3F)) 


void Font_AsciiLattice(uint8_t ascii,uint8_t* p_lattice);

bool Font_ChineseLattice(uint8_t* p_data, uint8_t* p_lattice);

void Font_KoreanLattice(uint8_t* p_data, uint8_t* p_lattice);

void Font_JapaneseLattice(uint8_t* p_data, uint8_t* p_lattice);

void Font_CyrillicLattice(uint8_t* p_data, uint8_t* p_lattice);

void Font_LattinLattice(uint8_t* p_data, uint8_t* p_lattice);

#endif
