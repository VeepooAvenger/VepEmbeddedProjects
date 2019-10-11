/***************************Copyright(c)***********************************
**                       Veepoo Tech
**                  COPYRIGHT (C) 2016-2019
**-----------File Info-----------------------------------------------------
** File        name:     Gsensor.h
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
#ifndef _GSENEOR_H_
#define _GSENEOR_H_

#include "stdint.h"

#define KX023_REG_XHPL                  0x00 
#define KX023_REG_XHPH                  0x01 
#define KX023_REG_YHPL                  0x02 
#define KX023_REG_YHPH                  0x03 
#define KX023_REG_ZHPL                  0x04 
#define KX023_REG_ZHPH                  0x05 
#define KX023_REG_XOUTL                 0x06 
#define KX023_REG_XOUTH                 0x07 
#define KX023_REG_YOUTL                 0x08 
#define KX023_REG_YOUTH                 0x09 
#define KX023_REG_ZOUTL                 0x0A 
#define KX023_REG_ZOUTH                 0x0B 
#define KX023_REG_COTR                  0x0C 
#define KX023_REG_WHO                   0x0F 
#define KX023_REG_TSCP                  0x10 
#define KX023_REG_TSPP                  0x11 
#define KX023_REG_INS1                  0x12 
#define KX023_REG_INS2                  0x13 
#define KX023_REG_INS3                  0x14 
#define KX023_REG_STAT                  0x15
#define KX023_REG_INT_REL               0x17 
#define KX023_REG_CNTL1                 0x18 
#define KX023_REG_CNTL2                 0x19 
#define KX023_REG_CNTL3                 0x1A 
#define KX023_REG_ODCNTL                0x1B 
#define KX023_REG_INC1                  0x1C 
#define KX023_REG_INC2                  0x1D 
#define KX023_REG_INC3                  0x1E 
#define KX023_REG_INC4                  0x1F 
#define KX023_REG_INC5                  0x20
#define KX023_REG_INC6                  0x21 
#define KX023_REG_TILT_TIMER            0x22 
#define KX023_REG_WUFC                  0x23 
#define KX023_REG_TDTRC                 0x24 
#define KX023_REG_TDTC                  0x25 
#define KX023_REG_TTH                   0x26 
#define KX023_REG_TTL                   0x27 
#define KX023_REG_FTD                   0x28 
#define KX023_REG_STD                   0x29 
#define KX023_REG_TLT                   0x2A
#define KX023_REG_TWS                   0x2B
#define KX023_REG_ATH                   0x30 
#define KX023_REG_TILT_ANGLE_LL         0x32 
#define KX023_REG_TILT_ANGLE_HL         0x33
#define KX023_REG_HYST_SET              0x34 
#define KX023_REG_LP_CNTL               0x35 
#define KX023_REG_BUF_CNTL1             0x3A 
#define KX023_REG_BUF_CNTL2             0x3B 
#define KX023_REG_BUF_STATUS1           0x3C 
#define KX023_REG_BUF_STATUS2           0x3D
#define KX023_REG_BUF_CLEAR             0x3E 
#define KX023_REG_BUF_READ              0x3F 
#define KX023_REG_SELF_TEST             0x60

//#define APP_HALACC_ENABLED 0

#define GSENSOR_INIT1   G_INIT1_PIN     //wake
#define GSENSOR_INIT2   G_INIT2_PIN    //data
#define GSENSOR_CS      G_CS_PIN
#define GSENSOR_TRIG    G_TRIG_PIN    //trig

//初始化
void Gsensor_AccInit(void);

//寄存器配置
void Gsensor_Configure(void);

//使能运动唤醒
void Gsensor_EnableMotion(void);

//失能运动唤醒
void Gsensor_DisableMotion(void);

//清除中断
void Gsensor_ClearInterruptFlag(void);

//失能buf
void Gsensor_FifoDisablePara(void);

//使能buf
void Gsensor_FifoEnablePara(void);

//清除buf
void Gsensor_CleanFifo(void);

//读取缓存数据
uint8_t Gsensor_ReadoneSample(int16_t *data, uint16_t len);

//写寄存器
uint8_t Gsensor_WriteReadReg(uint8_t *input, uint8_t input_len, uint8_t *output, uint8_t output_len);

//获取缓存数据个数
uint8_t Gsensor_GetBufferCurrentByte(void);

//获取中断管脚1的状态
uint8_t Gsensor_Init1State(void);

//休眠
void Gsensor_Sleep(void);

//自检
uint8_t Gsensor_Check(void);

//读取寄存器值
void Gsensor_ReadConfig(uint8_t *r_data);
#endif     
