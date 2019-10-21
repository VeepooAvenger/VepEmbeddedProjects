/***************************Copyright(c)***********************************
**                       Veepoo Tech
**                  COPYRIGHT (C) 2016-2019
**-----------File Info-----------------------------------------------------
** File        name:     afe4900_init.h
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

#ifndef	__AFE4900_INIT_H
#define __AFE4900_INIT_H

#include <stdint.h>
#include "nrf_drv_gpiote.h"


#pragma pack (1)

typedef struct
{
    uint8_t current;
    uint8_t voltage;
    uint8_t gain;
    uint8_t change_flag;
}afe4900_param_t;

/*
//参数1~参数3—电流：每个刻度为0.392mA；当值为255时，对应100mA.
//参数4~参数7—增益：0~8分别对应：500K、250K、100K、50K、25K、10K、1M、2M、1.5M Ω.
//参数8—DAC量程：0/3/5/7分别对应±15.875uA/±31.75uA/±63.5uA/±127uA.
//参数9~参数12—DAC值：0~127；
//                      当DAC量程为0时，刻度为：0.125；
//                      当DAC量程为3时，刻度为：0.25；
//                      当DAC量程为5时，刻度为：0.5；
//                      当DAC量程为7时，刻度为：1.
***********************************************************
////EN_AMB_LOOP: 0:关闭、1：开启；
////CHOOSE_AMB_PHASE：0~3的对应关系：0-LED2；1-AMB2和LED3；2-LED1；3-AMB1和LED4；
////HYST_LOOP：0~3分别对应47/94/144/188mV；
////FREEZE_LOOP：0:关闭、1：开启；
////EARLY_OFFSET_DAC：0:关闭、1：开启；
*/
typedef struct
{
    uint8_t     set_i_led1;     //绿光电流
    uint8_t     set_i_led2;     //红光电流
    uint8_t     set_i_led3;     //红外电流
    
    uint8_t     set_rf1_mid;    //增益1---绿光
    uint8_t     set_rf2_mid;    //增益2---红光   
    uint8_t     set_rf3_mid;    //增益3---红外光
    uint8_t     set_rf4_mid;    //增益4---环境光
    
	uint8_t     set_awdpr_ifs_dac;			// dac 量程

	uint8_t     set_awdpr_dac_gre;			// 绿光dac值
	uint8_t     set_awdpr_dac_red;			// 红光dac值
	uint8_t     set_awdpr_dac_ird;			// 红外dac值
	uint8_t     set_awdpr_dac_amb;			// 环境光dac值

	uint8_t     set_awdpr_dac_posi_gre;		// 绿光dac极性
	uint8_t     set_awdpr_dac_posi_red;		// 红光dac极性
	uint8_t     set_awdpr_dac_posi_ird;		// 红外dac极性
	uint8_t     set_awdpr_dac_posi_amb;	    // 环境光dac极性    
    
/***************************************************************/    
    
    uint8_t     set_en_amb_loop;            // 0:关闭、1：开启
    uint8_t     set_choose_amb_phase;       // 0~3的对应关系：0-LED2；1-AMB2和LED3；2-LED1；3-AMB1和LED4；
    uint8_t     set_hyst_loop;              // 0~3分别对应47/94/144/188mV；
    uint8_t     set_freeze_loop;            // 0:关闭、1：开启；
    uint8_t     set_early_offset_loop;      // 0:关闭、1：开启；
    
    
}afe4410_ppg_param_t;

#pragma pack ()

void AFE4900_HardwarePowerDown(void);


void Afe4900_Control1Init(void);

void Afe4900_SenInit(void);

void Afe4900_ResetzInit(void);

void Afe4900_EnableHwPdn(void);

void Afe4900_DisableHwPdn(void);

void Afe4900_TriggerHwReset(void);

void Afe4900_AdcrdyInterruptInit(nrf_drv_gpiote_evt_handler_t event_handler);

void Afe4900_AdcrdyInterruptEnable(void);

void Afe4900_AdcrdyInterruptDisable(void);

void Afe4900_SpiSelInit(void);

void Afe4900_RegInit(void);

void Afe4900_PttFlag(void);

uint32_t Afe4900_IsEnablePtt(void);

// afe4900的fifo读
void Afe4900_FifoSpiRead(uint8_t address, uint8_t *data, uint32_t data_cnt);

// afe4900的寄存器写
void Afe4900_RegWrite(unsigned char reg_address, unsigned int data);

// afe4900的寄存器读
int32_t Afe4900_RegRead(unsigned char reg_address);

// afe4900 数据转换
int32_t Afe4900_DataConvert(uint32_t data);

// afe4900的寄存器初始化开始采样
void Afe4900_InitStartSampling(void);

// afe4900的寄存器初始化停止采样
void Afe4900_InitStopSampling(void);

// afe4900的寄存器初始化PPG模式-心率
void Afe4900_RegInitPpg1(void);

// afe4900的寄存器初始化PPG模式-血氧
void Afe4900_RegInitPpg2(void);

void Afe4900_RegInitToRest(void);

// afe4900开启LDO
void Afe4900_LdoTxOn(void);

// afe4900关闭LDO
void Afe4900_LdoTxOff(void);


void Afe4410_RegPpgParamSet(uint8_t *data_buf, uint8_t pack_num);

void Afe4410_RegPpgParamGet(uint8_t *data_buf, uint8_t pack_num);

void Afe4410_RegPpgParamSetDo(void);

#endif

