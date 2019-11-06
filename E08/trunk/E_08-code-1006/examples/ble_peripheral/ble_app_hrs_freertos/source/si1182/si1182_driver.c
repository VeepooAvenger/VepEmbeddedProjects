/*******************Copyright(c)*********************************
** Veepoo Tech
** COPYRIGHT (C) 2016-2019
**----------------------FileInfo---------------------------------
** File name:                  si1182_driver.c
** Last modified Date:         2017-12-22
** Last SYS_VERSION:           0.0.1
** Descriptions:
**---------------------------------------------------------------
** Createdby:                  JSH
** Createddate:                2017-12-22
** SYS_VERSION:                0.0.1
** Descriptions:               si117x的接口
**---------------------------------------------------------------
** Modified by:				
** Modified date:			
** SYS_VERSION:				
** Modified Descriptions:	
****************************************************************/
#include <string.h>

#include "si1182_driver.h"
#include "si1182_config.h"
#include "si1182_handle.h"
#include "si1182_functions.h"
#include "si1182_interface.h"
#include "si1182_calibration.h"
#include "data_manage.h"
#include "ble_thread.h"

/***************************************************************************/

si1182_data_t g_si1182_data;

uint8_t init_mode_1182_task = 0;
/***************************************************************************/

void Si1182_CheckExceptionParamInit(void);

/****************************************************************
** Function name:             Si1182_Start
** Descriptions:              Si117x开始测量
** input parameters:                                      
** output parameters:         
** Returned value:            
** Created by:                JSH
** Created Date:              2017-12-22
****************************************************************/
void Si1182_Start(void)
{   
	if(StartDataLogging())
    {
        Si1182_StartOk();
    }
}

/****************************************************************
** Function name:             Si1182_Stop
** Descriptions:              Si117x停止测量
** input parameters:                                      
** output parameters:         
** Returned value:            
** Created by:                JSH
** Created Date:              2017-12-22
****************************************************************/
void Si1182_Stop(void)
{
	if(StopDataLogging())
    {
        Si1182_StopOk();
    }
}

/****************************************************************
** Function name:             Si117x_UpdateConfig
** Descriptions:              Si117x更新配置
** input parameters:                                      
** output parameters:         
** Returned value:            
** Created by:                JSH
** Created Date:              2017-12-22
****************************************************************/
int32_t Si117x_UpdateConfig(uint8_t address, uint8_t value)
{
    int32_t retval = 0;
    
    retval = Si117x_ParamSet(address, value);
     
    return retval;
}

/****************************************************************
** Function name:             Si117x_ReadConfig
** Descriptions:              Si117x读取配置
** input parameters:                                      
** output parameters:         
** Returned value:            
** Created by:                JSH
** Created Date:              2017-12-22
****************************************************************/
int32_t Si117x_ReadConfig(uint8_t address)
{
    int32_t retval = 0;
    
    retval = Si117x_ParamRead(address);
     
    return retval;
}

/****************************************************************
** Function name:             Si1182_SetParameterReadFlag
** Descriptions:              Si1182设置读取参数标志
** input parameters:                                      
** output parameters:         
** Returned value:            
** Created by:                JSH
** Created Date:              2019-01-15
****************************************************************/
void Si1182_SetParameterReadFlag()
{
	g_si1182_data.read_config = 1;
}

/****************************************************************
** Function name:             Si1182_GetParameterConfig
** Descriptions:              Si1182读取配置
** input parameters:                                      
** output parameters:         
** Returned value:            
** Created by:                JSH
** Created Date:              2019-01-15
****************************************************************/
void Si1182_GetParameterConfig(uint8_t *data, uint32_t packet_num)
{
	uint32_t j = 0;
	uint32_t index = 0;
	
	if(packet_num > SI117X_MAX_ADDRESS / 10)
	{
		memset(data, 0xff, 20);
	}
	else
	{
		for(uint32_t i = 0; i < 10; i++)
		{
			index = packet_num * 10 + i;
			
			data[j++] = index;
			
			if(index <= SI117X_MAX_ADDRESS)
			{
				data[j++] = g_si1182_data.param_config[index];
			}
			else
			{
				data[j++] = 0xff;
			}
		}
	}
	
	return;
}

/****************************************************************
** Function name:             Si1182_GetDataPpg
** Descriptions:              Si117x的adc数据获取
** input parameters:          out_data： 获取数据的指针                            
** output parameters:         
** Returned value:            
** Created by:                JSH
** Created Date:              2017-12-22
****************************************************************/
int32_t Si1182_GetDataPpg(uint8_t *out_data)
{
	int32_t ret = 0;

	ret = Si1182_HandleFifoDataPpg(out_data, g_si1182_data.fifo_data , g_si1182_data.fifo_read_len);

	return ret;
}


/****************************************************************
** Function name:             Si1182_GetDataEcg
** Descriptions:              Si1182的adc数据获取
** input parameters:          out_data： 获取数据的指针                            
** output parameters:         
** Returned value:            
** Created by:                JSH
** Created Date:              2017-12-22
****************************************************************/
int32_t Si1182_GetDataEcg(uint8_t *out_data)
{
	int32_t ret = 0;

	ret = Si1182_HandleFifoDataEcg(out_data, g_si1182_data.fifo_data , g_si1182_data.fifo_read_len);

	return ret;
}

/****************************************************************
** Function name:             Si1182_GetDataEcgAndLd
** Descriptions:              Si1182的adc数据获取
** input parameters:          out_data： 获取数据的指针                            
** output parameters:         
** Returned value:            
** Created by:                JSH
** Created Date:              2017-12-22
****************************************************************/
int32_t Si1182_GetDataEcgAndLd(uint8_t *out_data, uint8_t *ld_data, uint32_t *ld_data_len)
{
	int32_t ret = 0;

	ret = Si1182_HandleFifoDataEcgAndLd(out_data, (const uint8_t *)g_si1182_data.fifo_data , g_si1182_data.fifo_read_len, ld_data, ld_data_len);

	return ret;
}

/****************************************************************
** Function name:             Si1182_PowerOnInit
** Descriptions:              Si1182上电初始化
** input parameters:                                      
** output parameters:         
** Returned value:            
** Created by:                JSH
** Created Date:              2018-12-22
****************************************************************/
void Si1182_PowerOnInit(void)
{
	LDO_TX_OFF();
	Si1182_TransferInit();
	Si1182_ConfigPowerOn();
	Si1182_InterruptInit();
}

/****************************************************************
** Function name:             Si1182_InitByMode
** Descriptions:              Si1182按模式初始化
** input parameters:                                      
** output parameters:         
** Returned value:            
** Created by:                JSH
** Created Date:              2018-12-22
****************************************************************/
extern uint8_t adc_ecg_signal_state;
int32_t Si1182_InitByMode(uint8_t init_mode)
{
	int32_t err_value = 0;
	
    //WJ.20190121.释放中断响应
    Si1182_InterruptUnInit();
    
    Si1182_CheckExceptionParamInit();
    
	if((init_mode == MODE_ECG)||(init_mode == MODE_NULL))
	{
		LDO_TX_OFF();
	}
	else
	{
		LDO_TX_ON();
	}
	
	switch(init_mode)
	{
		case MODE_NULL:
			err_value = Si1182_ConfigModeNull();
			break;
        
		case MODE_ECG:
			err_value = Si1182_ConfigModeWithEcg();
			init_mode_1182_task = MODE_ECG;
            adc_ecg_signal_state = 0;
			break;
        
//		case MODE_GREEN_AMB:
//			err_value = Si1182_ConfigModeWithGreenAmb();
//			init_mode_1182_task = MODE_GREEN_AMB;
//			break;
        
		case MODE_GREEN_IRED_AMB:
			err_value = Si1182_ConfigModeWithGreenIredAmb();
			init_mode_1182_task = MODE_GREEN_IRED_AMB;
			break;
        
		case MODE_GREEN_RED_IRED_AMB:
			err_value = Si1182_ConfigModeWithGreenRedIredAmb();
			init_mode_1182_task = MODE_GREEN_RED_IRED_AMB;
			break;
		
		case MODE_RED_IRED_AMB:
			err_value = Si1182_ConfigModeWithRedIredAmb();
			init_mode_1182_task = MODE_RED_IRED_AMB;
			break;
		
		default:
			init_mode_1182_task = 0;
			err_value = -0xff;
	}
	
	if(err_value != 0)
	{
		// 不能有外设的初始化
		Si1182_PowerOnInit();
		g_si1182_data.adc_mode = 0;
		
		return err_value;
	}

	g_si1182_data.adc_mode = init_mode;
	g_si1182_data.ecg_lead_state = 1;
	
	// 血氧校准参数初始化
	if(g_si1182_data.adc_mode == MODE_RED_IRED_AMB 
      || g_si1182_data.adc_mode == MODE_GREEN_RED_IRED_AMB)
	{
		Si1182_BloodOxgenCalibInit();
	}
    
	Si1182_InterruptInit();
	
	Si1182_Start();
	
	return 0;
}


// 输出ECG导联状态
uint8_t Si1182_IsLeadOn(void)
{    
    return g_si1182_data.ecg_lead_state;
}

// WJ.20190429.1182异常处理
void Si1182_CheckExceptionParamInit(void)
{
    p_data_manage->ecg_ppg_s.stop_int_cnt = 0;      // 1182 开启下中断不响应计次(s)，用着异常处理
    p_data_manage->ecg_ppg_s.stop_read_cnt = 0;     // 1182 开启下中断不读取数据时间(s)，用着异常处理	    
}

void Si1182_CheckExceptionHandling(void)
{
    if(p_data_manage->ecg_ppg_s.afe4900_is_open != 1)
    {
        return;
    }
    
    p_data_manage->ecg_ppg_s.stop_int_cnt++;
    p_data_manage->ecg_ppg_s.stop_read_cnt++;
    
    if( p_data_manage->ecg_ppg_s.stop_int_cnt > 20 
       || p_data_manage->ecg_ppg_s.stop_read_cnt > 20)
    {    
        memset(&p_data_manage->ecg_ppg_s, 0, sizeof(ecg_ppg_measure_t));
        BleThread_PeripheralsDevClose(); 
    }
}

