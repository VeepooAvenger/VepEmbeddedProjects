/***************************Copyright(c)***********************************
**                       Veepoo Tech
**                  COPYRIGHT (C) 2016-2019
**-----------File Info-----------------------------------------------------
** File        name:     afe4900_driver.c
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

#include "afe4900_driver.h"
#include "afe4900_init.h"
#include "afe4900_entry.h"


/*************************************************************************
** Function name:        Afe4900_FifoReadConvert
** Descriptions:         afe4900 fifo读取并转换
** input parameter:      无   
** Returned Value:       无
** Created  by:          JSH
** Created  Datas:       2018-10-27
**************************************************************************/
void Afe4900_FifoReadConvert(uint8_t *data, uint32_t sample_cnt)
{
	uint32_t value = 0;
	int32_t convert_v = 0;
	uint8_t *p_data = NULL;
	int32_t *p_convert_d = NULL;
	
	Afe4900_FifoSpiRead(0xff, data, sample_cnt * 3);
	
	p_data = data + sample_cnt * 3;
	p_convert_d = (int32_t *)(data + sample_cnt * 4); 
	
	for(uint32_t i = 0; i < sample_cnt; i++)
	{
		value = *(--p_data);
		value |= *(--p_data) << 8;
		value |= *(--p_data) << 16;
		
		convert_v = Afe4900_DataConvert((uint32_t)value);
		*(--p_convert_d) = convert_v;
	}
	
	return;
}

/*************************************************************************
** Function name:        Afe4900_FifoDateGet
** Descriptions:         afe4900 fifo数据获取
** input parameter:      无   
** Returned Value:       无
** Created  by:          JSH
** Created  Datas:       2018-10-27
**************************************************************************/
void Afe4900_FifoDateGet(int32_t *data_src, uint32_t sample_types, uint32_t sample_index, int32_t *data_get, uint32_t sample_cnt)
{
	uint32_t i = 0;
	
	data_src += sample_index;
	
	for(i = 0; i < sample_cnt; i++)
	{
		*data_get = *data_src;
		data_get++;
		data_src += sample_types;
	}
	
	return;
}

/*************************************************************************
** Function name:        Afe4900_RegisterReadConvert
** Descriptions:         afe4900 读寄存器并转换
** input parameter:      无   
** Returned Value:       无
** Created  by:          JSH
** Created  Datas:       2018-10-18
**************************************************************************/
int32_t Afe4900_RegisterReadConvert(uint8_t reg_address)
{
	int32_t reg_value = 0;
	int32_t ret = 0;
	
	reg_value = Afe4900_RegRead(reg_address);
	
	ret = Afe4900_DataConvert((uint32_t)reg_value);
	
	return ret;
}


/*************************************************************************
** Function name:        Afe4900_StartSampling
** Descriptions:         afe4900 开始采样
** input parameter:      无   
** Returned Value:       无
** Created  by:          JSH
** Created  Datas:       2018-10-17
**************************************************************************/
void Afe4900_StartSampling(void)
{
	Afe4900_InitStartSampling();
}

/*************************************************************************
** Function name:        Afe4900_StopSampling
** Descriptions:         afe4900 停止采样
** input parameter:      无   
** Returned Value:       无
** Created  by:          JSH
** Created  Datas:       2018-10-17
**************************************************************************/
void Afe4900_StopSampling(void)
{
	Afe4900_InitStopSampling();
}

/*************************************************************************
** Function name:        Afe4900_Ppg1Config
** Descriptions:         afe4900 配置PPG模式-心率模式
** input parameter:      无   
** Returned Value:       无
** Created  by:          JSH
** Created  Datas:       2018-10-17
**************************************************************************/
void Afe4900_Ppg1Config(void)
{
	Afe4900_AdcrdyInterruptInit(Afe4900_Ppg1Isr);
    
	Afe4900_Control1Init();
	Afe4900_SenInit();
	Afe4900_ResetzInit();
	Afe4900_EnableHwPdn();
	Afe4900_DisableHwPdn();
	Afe4900_TriggerHwReset();
	Afe4900_SpiSelInit();
	
	Afe4900_RegInitPpg1();
	Afe4900_AdcrdyInterruptEnable();
}

/*************************************************************************
** Function name:        Afe4900_Ppg2Config
** Descriptions:         afe4900 配置PPG模式-血氧模式
** input parameter:      无   
** Returned Value:       无
** Created  by:          JSH
** Created  Datas:       2018-10-17
**************************************************************************/
void Afe4900_Ppg2Config(void)
{
	Afe4900_AdcrdyInterruptInit(Afe4900_Ppg2Isr);
    
	Afe4900_Control1Init();
	Afe4900_SenInit();
	Afe4900_ResetzInit();
	Afe4900_EnableHwPdn();
	Afe4900_DisableHwPdn();
	Afe4900_TriggerHwReset();
	Afe4900_SpiSelInit();
	
	Afe4900_RegInitPpg2();
	Afe4900_AdcrdyInterruptEnable();
}


/*************************************************************************
** Function name:        Afe4900_PpgRestConfig
** Descriptions:         afe4900 配置PPG模式-上电模式
** input parameter:      无   
** Returned Value:       无
** Created  by:          WJ
** Created  Datas:       2019-02-15
**************************************************************************/
void Afe4900_PpgRestConfig(void)
{
	Afe4900_AdcrdyInterruptInit(Afe4900_PpgRestIsr);
    
	Afe4900_Control1Init();
	Afe4900_SenInit();
	Afe4900_ResetzInit();
	Afe4900_EnableHwPdn();
	Afe4900_DisableHwPdn();
	Afe4900_TriggerHwReset();
	Afe4900_SpiSelInit();
	
	Afe4900_RegInitToRest();
	Afe4900_AdcrdyInterruptEnable();
}

/*************************************************************************
** Function name:        Afe4900_PowerOnInit
** Descriptions:         afe4900 上电初始化
** input parameter:      无   
** Returned Value:       无
** Created  by:          JSH
** Created  Datas:       2018-10-17
**************************************************************************/
void Afe4900_PowerOnInit(void)
{
	Afe4900_LdoTxOff();
	AFE4900_HardwarePowerDown();
}
