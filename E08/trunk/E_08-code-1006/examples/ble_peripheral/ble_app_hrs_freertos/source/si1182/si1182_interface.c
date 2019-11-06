/*******************Copyright(c)*********************************
** Veepoo Tech
** COPYRIGHT (C) 2016-2019
**----------------------FileInfo---------------------------------
** File name:                  si117x_interface.c
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


#include "si1182_interface.h"
#include "si1182_config.h"
#include "si1182_handle.h"
#include "bsp_spi.h"
#include "nrf_delay.h"
#include "app_error.h" 
#include "nrf_drv_gpiote.h"
#include "board.h"
#include "Communication_Thread.h"
#include "ble_thread.h"
#include "Data_Manage.h"
#include "algorithm_thread.h"

/***************************************************************/


static uint8_t si118x_int_flag = 0;     // 1182 中断关键初始化状态

/***************************************************************/






/****************************************************************
** Function name:             Si117x_SpiInit
** Descriptions:              si117x传输接口初始化
** input parameters:                                   
** output parameters:         
** Returned value:            
** Created by:                JSH
** Created Date:              2017-12-22
****************************************************************/
void Si1182_TransferInit(void)
{
	//Bps_Spi0Init();
}

/****************************************************************
** Function name:             Si117x_WriteRegister
** Descriptions:              si117x写单个寄存器
** input parameters:          adress:                             
** output parameters:         
** Returned value:            寄存器的值
** Created by:                JSH
** Created Date:              2017-12-22
****************************************************************/
uint32_t Si117x_WriteRegister(uint32_t address, uint32_t data)
{
	uint32_t ret = 0;
	uint8_t write_array[2];
	
	address |= 0x00;
	
	write_array[0] = (uint8_t)address;
	write_array[1] = (uint8_t)data;
	
	SI1182_SPI_CS_LOW
    ret = Bps_Spi0Write((const uint8_t *)&write_array, 2);
	SI1182_SPI_CS_HIGH
	
	if(ret)
	{
		ret = 0;
	}
	else
	{
		ret = 1;
	}
	
	return ret;
}

/****************************************************************
** Function name:             Si117x_ReadRegister
** Descriptions:              si117x读取单个寄存器
** input parameters:          adress:                             
** output parameters:         
** Returned value:            寄存器的值
** Created by:                JSH
** Created Date:              2017-12-22
****************************************************************/
uint8_t Si117x_ReadRegister(uint32_t address)
{
	uint8_t data = 0;
	
	address |= 0x80;
	
	SI1182_SPI_CS_LOW
    Bps_Spi0Write((const uint8_t *)&address, 1);
    Bps_Spi0Read(&data, 1);
	SI1182_SPI_CS_HIGH
	
	return data;
}

/****************************************************************
** Function name:             Si117x_BlockRead
** Descriptions:              si117x连续读取
** input parameters:          adress:
                              out_data:
							  data_len:       最大255
** output parameters:         out_data:
** Returned value:            无
** Created by:                JSH
** Created Date:              2017-12-22
****************************************************************/
void Si117x_BlockRead(uint32_t address, uint8_t *out_data, uint32_t data_len)
{
	address |= 0x80;
	
	SI1182_SPI_CS_LOW
    Bps_Spi0Write((const uint8_t *)&address, 1);
    Bps_Spi0Read(out_data, data_len);
	SI1182_SPI_CS_HIGH
	
	return;
}

/****************************************************************
** Function name:             Si1171_IntHandler
** Descriptions:              si117x 中断接口
** input parameters:                                      
** output parameters:         
** Returned value:            
** Created by:                JSH
** Created Date:              2017-12-26
****************************************************************/
void Si118x_IntHandler(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action)
{
	if(nrf_gpio_pin_read(SI118X_INT))
    {
        //引脚为高，非数据中断
        return;
    }    
    
	Si1182_IntHandler();
}

/****************************************************************
** Function name:             Si1182_InterruptInit
** Descriptions:              si117x fifo 中断初始化
** input parameters:                                      
** output parameters:         
** Returned value:            
** Created by:                JSH
** Created Date:              2017-12-22
****************************************************************/
void Si1182_InterruptInit(void)
{
    uint32_t err_code = NRF_SUCCESS;

    if(!nrf_drv_gpiote_is_init())
    {
        err_code = nrf_drv_gpiote_init();    
        APP_ERROR_CHECK(err_code);
    }
    

	if(si118x_int_flag == 1)
	{
		nrf_drv_gpiote_in_uninit(SI118X_INT);
		si118x_int_flag = 0;
	}
	
	//nrf_drv_gpiote_in_config_t in_config = GPIOTE_CONFIG_IN_SENSE_HITOLO(false);//true);   
    nrf_drv_gpiote_in_config_t in_config = GPIOTE_CONFIG_IN_SENSE_TOGGLE(false);    
    in_config.pull = NRF_GPIO_PIN_PULLUP;
    
    err_code = nrf_drv_gpiote_in_init(SI118X_INT, &in_config, Si118x_IntHandler);
    APP_ERROR_CHECK(err_code);
	si118x_int_flag = 1;
    
    nrf_drv_gpiote_in_event_enable(SI118X_INT, true);
}


/****************************************************************
** Function name:             Si1182_InterruptUnInit
** Descriptions:              si117x 释放中断
** input parameters:                                      
** output parameters:         
** Returned value:            
** Created by:                WJ
** Created Date:              2019-01-21
****************************************************************/
void Si1182_InterruptUnInit(void)
{
    uint32_t err_code = NRF_SUCCESS;

    if(!nrf_drv_gpiote_is_init())
    {
        err_code = nrf_drv_gpiote_init();    
        APP_ERROR_CHECK(err_code);
    }
    
	if(si118x_int_flag == 1)
	{
		nrf_drv_gpiote_in_uninit(SI118X_INT);
		si118x_int_flag = 0;
	}
}

/****************************************************************
** Function name:             Si1182_FifoDataOk
** Descriptions:              FIFO数据读取完毕，通知程序来取数据
** input parameters:                                      
** output parameters:         
** Returned value:            
** Created by:                JSH
** Created Date:              2017-12-26
****************************************************************/
void Si1182_FifoDataOk(void)
{
    AlgThread_CalcSignal();
	ComThread_SendADCData();
}

/****************************************************************
** Function name:             Si1182_StartOk
** Descriptions:              si117x开启测量ok
** input parameters:                                      
** output parameters:         
** Returned value:            
** Created by:                JSH
** Created Date:              2018-01-03
****************************************************************/
void Si1182_StartOk(void)
{
    DataManage_InitPoolData();
}

/****************************************************************
** Function name:             Si1182_StopOk
** Descriptions:              si117x停止测量ok
** input parameters:                                      
** output parameters:         
** Returned value:            
** Created by:                JSH
** Created Date:              2018-03-31
****************************************************************/
void Si1182_StopOk(void)
{
//    DataManage_UninitSleepData();
}

/****************************************************************
** Function name:             Si117x_Delay10ms
** Descriptions:              si117x 延时10ms
** input parameters:                                      
** output parameters:         
** Returned value:            
** Created by:                JSH
** Created Date:              2017-12-22
****************************************************************/
void Si117x_Delay10ms(void)
{
	uint32_t i = 0;
	
	//当freertos运行起来之后，可以考虑用系统延时
	for(i = 0; i < 10; ++i)
	{
		nrf_delay_us(1000);
	}
	
	return;
}

