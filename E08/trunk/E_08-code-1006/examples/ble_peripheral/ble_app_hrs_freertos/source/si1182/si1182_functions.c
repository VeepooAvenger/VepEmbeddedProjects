/*******************Copyright(c)*********************************
** Veepoo Tech
** COPYRIGHT (C) 2016-2019
**----------------------FileInfo---------------------------------
** File name:                  si117x_functions.c
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

// LED1\LED2:红光；LED3:绿光；LED4:红外
/***************************************************************/
#include <string.h>

#include "si1182_functions.h"
#include "si1182_interface.h"
#include "si1182_config.h"
#include "si1182_driver.h"


#define  PPG_SET_TEST           0       //用于开启app ppg参数调试接口

#define  ECG_LD_TEST            0       //导联测试，关闭ECG数据采样

#if ECG_LD_TEST
#define  ECG_LD_INTO_FIFO       (0<<6)  //0导联数据加入FIFO
#else
#define  ECG_LD_INTO_FIFO       (1<<6)  //1导联数据不加入FIFO
#endif

#define NULL 0


/***************************************************************/

// ECG 参数调节配置
#define     ECG_GAIN_INIT			1//1		// ECG增益：0~3 对应，2.8\1.4\1.866\0.4 倍
#define     LD_FREQ_INIT			5//3//5		// 导联激励频率：0~8 对应，1\2\4\5\8\10\20\40\50 kHz
#define     LD_CURRENT_INT			4//5		// 导联激励电流：0~7 对应，0.12\0.25\0.49\0.98\1.95\3.91\7.81\15.63 uA
#define     LD_GAIN_INT				0		    // 导联增益：0~1对应，0.4\2.8 倍
#define     LD_SHRESHOLD_INT		40//64//0		// 导联阈值：0~255 无对应的值
#define     LD_IM_INT				1//0		// 导联阈值极性：0~1 对应，联上中断\脱落中断
#define     LD_ENABLE_INT			0//1		// 导联使能：0~1 对应，关闭导联\使能导联
#define     ECG_SW_AVG_INT			1		// SW_AVG：0~7 对应，2的N次方的值
#define		ECG_DECIM_INT				1		// 0~3 对应，32\64\128\256
#define		ECG_MC_INT					2
#define		LD_MC_INT					25*5//25        //变更这里需要变更样本中断数
#define		ECG_MR_H_INT					0
#define		ECG_MR_L_INT					40
#define		ECG_PARAM_1_INT					0
#define		ECG_PARAM_2_INT					0
#define		ECG_PARAM_3_INT					0

// PPG 参数调节配置
#define     PPG_TASK_ENABLE_INIT			0x00		
#define     PPG_MR_H_INIT					0x01
#define     PPG_MR_L_INT					0x90
#define     PPG_MC_INT					0x01
#define     PPG1_LED_INT				0x6f	
#define     PPG2_LED_INT				0x6f	
#define     PPG3_LED_INT				0x5c	
#define     PPG4_LED_INT				0x00	
#define     PPG1_MODE_INT				0x0f	
#define     PPG2_MODE_INT				0x0f
#define     PPG3_MODE_INT				0x0f
#define     PPG4_MODE_INT				0x0f
#define     PPG1_MCONFIG_INT			0x13	
#define     PPG2_MCONFIG_INT			0x13
#define     PPG3_MCONFIG_INT			0x13	
#define     PPG4_MCONFIG_INT			0x13
#define		PPG1_DECIM_INT				0x03
#define		PPG1_ADC_RANGE_INT			0x03
#define		PPG1_CLK_DIV_INT			0x00
#define		PPG2_DECIM_INT				0x03	
#define		PPG2_ADC_RANGE_INT			0x03
#define		PPG2_CLK_DIV_INT			0x00
#define		PPG3_DECIM_INT				0x03	
#define		PPG3_ADC_RANGE_INT			0x03
#define		PPG3_CLK_DIV_INT			0x00
#define		PPG4_DECIM_INT				0x03	
#define		PPG4_ADC_RANGE_INT			0x03
#define		PPG4_CLK_DIV_INT			0x00
#define		PPG_PARAM_1_INT					0x00	// PPG_SW_AVG
#define		PPG_PARAM_2_INT					0
#define		PPG_PARAM_3_INT					0
#define		PPG_PARAM_4_INT					0

// ECG 参数配置调节
static si1182_ecg_param_t si1182_ecg_param = {	
										ECG_GAIN_INIT,
										LD_FREQ_INIT,
										LD_CURRENT_INT,
										LD_GAIN_INT,
										LD_SHRESHOLD_INT,
										LD_IM_INT,
										LD_ENABLE_INT,
										ECG_SW_AVG_INT,
										ECG_DECIM_INT,
										ECG_MC_INT,
										LD_MC_INT,	
										ECG_MR_H_INT,	
										ECG_MR_L_INT,	
										ECG_PARAM_1_INT,
										ECG_PARAM_2_INT,
										ECG_PARAM_3_INT,
										0
									 };

// PPG 参数配置调节
static si1182_ppg_param_t si1182_ppg_param = {
											PPG_TASK_ENABLE_INIT,			
											PPG_MR_H_INIT,			
											PPG_MR_L_INT,					
											PPG_MC_INT,				
											PPG1_LED_INT,					
											PPG2_LED_INT,					
											PPG3_LED_INT,					
											PPG4_LED_INT,					
											PPG1_MODE_INT,					
											PPG2_MODE_INT,				
											PPG3_MODE_INT,				
											PPG4_MODE_INT,				
											PPG1_MCONFIG_INT,				
											PPG2_MCONFIG_INT,			
											PPG3_MCONFIG_INT,				
											PPG4_MCONFIG_INT,			
											PPG1_DECIM_INT,			
											PPG1_ADC_RANGE_INT,			
											PPG1_CLK_DIV_INT,			
											PPG2_DECIM_INT,					
											PPG2_ADC_RANGE_INT,			
											PPG2_CLK_DIV_INT,			
											PPG3_DECIM_INT,				
											PPG3_ADC_RANGE_INT,			
											PPG3_CLK_DIV_INT,			
											PPG4_DECIM_INT,				
											PPG4_ADC_RANGE_INT,			
											PPG4_CLK_DIV_INT,			
											PPG_PARAM_1_INT,				
											PPG_PARAM_2_INT,				
											PPG_PARAM_3_INT,
											PPG_PARAM_4_INT,
											0,
											0
										};


/***************************************************************/
									 
void Si1182_EcgParamInit(void);
										
void Si1182_PpgParamInit(void);

int16_t Si117x_ParamRead(uint8_t address);

int16_t Si117x_ParamSet(uint8_t address, uint8_t value);

int32_t Si117x_Reset(void);

static int32_t SendCmd(uint32_t command);

int32_t Si117xUpdateFifoCount(void);

int32_t Si117xStop(uint32_t value);

int32_t Si117xStart(void);

int32_t Si117xFlushFIFO(void);

uint32_t Si117x_GetIrqStatus(void);

/***************************************************************/

/***************************************************************/
//电脑生成的配置函数的配套代码

#define HANDLE      uint32_t

#define PARAM_MEAS_RATE_H   PARAM_MEASRATE_H         //
#define PARAM_MEAS_RATE_L   PARAM_MEASRATE_L         //

int32_t Si117xReset(HANDLE si117x_handle)
{
    return Si117x_Reset();
}

uint8_t Si117xReadFromRegister(HANDLE si117x_handle, uint8_t address)
{
    return Si117x_ReadRegister(address);
}

int32_t Si117xWriteToRegister(HANDLE si117x_handle, uint8_t address, uint8_t data)
{
    return Si117x_WriteRegister(address, data);
}

int16_t Si117xParamSet(HANDLE handle, uint8_t address, uint8_t value)
{
    return Si117x_ParamSet(address, value);
}

int16_t Si1182_ParamSet(uint32_t handle, uint8_t address, uint8_t value)
{
    return Si117x_ParamSet(address, value);
}

void delay_10ms(void)
{
    Si117x_Delay10ms();
}

/***************************************************************/
/********************************************************************
 * @brief
 *   Reads a Parameter from the Si117x
 * @param[in] si117x_handle 
 *   The programmer's toolkit handle
 * @param[in] address   
 *   The address of the parameter. 
 * @retval <0       
 *   Error
 * @retval 0-255    
 *   Parameter contents
 ********************************************************************/
int16_t Si117x_ParamRead(uint8_t address)
{
  // returns Parameter[address]
  int16_t retval;
  uint8_t cmd = CMD_PARAM_QUERY_BYTE_MASK + (address & 0x3F);

  retval= SendCmd(cmd);
  if( retval != 0 ) 
  {
    return retval;
  }

  retval = Si117x_ReadRegister(REG_RESPONSE1);
  
  return retval;
}
	
/**************************************************************************
 *   Writes a byte to an Si117x Parameter
 * @param[in] si117x_handle 
 *   The programmer's toolkit handle
 * @param[in] address 
 *   The parameter address 
 * @param[in] value   
 *   The byte value to be written to the Si117x parameter
 * @retval 0    
 *   Success
 * @retval <0   
 *   Error
 * @note This function ensures that command completion is checked.
 * If setting parameter is not done properly, no measurements will
 * occur. This is the most common error. It is highly recommended
 * that host code make use of this function.
 ************************************************************************/
int16_t Si117x_ParamSet(uint8_t address, uint8_t value)
{
  int16_t retval;
  uint8_t buffer[2];

  buffer[0]= value;						// parameter value
  buffer[1]= CMD_PARAM_SET_BYTE_MASK + (address & 0x3F);	// param_write command

  retval = Si117x_WriteRegister(REG_HOSTIN0, buffer[0]);
  retval += SendCmd(buffer[1]);
  return retval;
}


/***************************************************************************//**
 * @brief 
 *   Resets the Si117x, clears any interrupts and initializes the HW_KEY 
 *   register.
 * @param[in] si117x_handle 
 *   The programmer's toolkit handle
 * @retval  0       
 *   Success
 * @retval  <0      
 *   Error
 ******************************************************************************/
int32_t Si117x_Reset(void)
{
	int32_t retval = 0;

	//
	// Do not access the Si117x earlier than 25 ms from power-up. 
	// Uncomment the following lines if Si117xReset() is the first
	// instruction encountered, and if your system MCU boots up too 
	// quickly. 
	//
	Si117x_Delay10ms();
	Si117x_Delay10ms();
	Si117x_Delay10ms();

	// Perform the Reset Command
	retval+=Si117x_WriteRegister(REG_COMMAND, CMD_RESET_SW);

	// Delay for 10 ms. This delay is needed to allow the Si117x
	// to perform internal reset sequence. 
	Si117x_Delay10ms();

	return retval;
}

/***************************************************************************//**
 * @brief 
 *   Helper function to send a command to the Si117x
 ******************************************************************************/
int32_t SendCmd(uint32_t command) 
{
  int16_t response_before = 0, response_after = 0;
  int16_t retval;
  uint8_t count = 0;
  const uint8_t max_count = 5;

  count = 0;
  while (count < max_count) {
    retval = Si117x_ReadRegister(REG_RESPONSE0);
    if (retval < 0) {
      return retval;  // i2c error occurred
    } else if ((retval & RSP0_COUNTER_ERR) == RSP0_COUNTER_ERR) {
      // CMD_ERR flag was set. Typically caused by a previous invalid command code or read/write to invalid parameter address
      // return the command error code * -1
      return -1 * (retval & RSP0_COUNTER_MASK);
    } else if ((retval & RSP0_COUNTER_MASK) == response_before && count > 0) {
      // cmd_ctr is stable, okay to execute command
      break;
    }
    response_before = retval & RSP0_COUNTER_MASK;
    count++;
  }

  // Return an error if command counter is not stable.
  // This typically indicates a communication error.
  if (count >= max_count) {
    return -1;
  }

  // Send the command.
  retval = Si117x_WriteRegister(REG_COMMAND, command);
  if (retval != 0) {
    return retval;
  }

  // Wait for the change in the command counter, confirming command execution.
  if (command != CMD_RESET_CMD_CTR && command != CMD_RESET_SW && command != CMD_SET_I2C_ADDR) {
    count = 0;
    while (count < max_count) {
      response_after = Si117x_ReadRegister(REG_RESPONSE0);
      if (response_after < 0) {
        return response_after;  // i2c error occurred
      } else if ((response_after & RSP0_COUNTER_ERR) == RSP0_COUNTER_ERR) {
        // CMD_ERR flag was set. Typically caused by invalid command code or read/write to invalid parameter address
        // return the command error code * -1
        return -1 * (response_after & RSP0_COUNTER_MASK);
      } else if ((response_after & RSP0_COUNTER_MASK) == ((response_before + 1) & RSP0_COUNTER_MASK)) {
        break;
      }
      count++;
    }

    // Return an error if command counter did not increment after max_count retries.
    // This typically indicates a communication error.
    if (count >= max_count) {
      return -1;
    }
  }

  // Command was sent and executed correctly.
  return 0;
}

/***************************************************************************//**
 * @brief 
 *   Sends a STOP command to the Si117x
 * @param[in] si117x_handle
 *   The programmer's toolkit handle
 * @param[in] value   
 *   The byte value to mark FIFO
 * @retval  0       
 *   Success
 * @retval  <0      
 *   Error
 ******************************************************************************/
int32_t Si117xStop(uint32_t value)
{
	return SendCmd(CMD_STOP);
}

/***************************************************************************//**
 * @brief 
 *   Sends a START command to the Si117x
 * @param[in] si117x_handle
 *   The programmer's toolkit handle
 * @retval  0       
 *   Success
 * @retval  <0      
 *   Error
 ******************************************************************************/
int32_t Si117xStart(void)
{
	return SendCmd(CMD_START);
}

/***************************************************************************//**
 * @brief 
 *   Sends a FLUSH_FIFO command to the Si117x
 * @param[in] si117x_handle
 *   The programmer's toolkit handle
 * @retval  0       
 *   Success
 * @retval  <0      
 *   Error
 ******************************************************************************/
int32_t Si117xFlushFIFO(void)
{
	Si117x_WriteRegister(REG_FIFO_FLUSH, 1);
	Si117x_Delay10ms();
	Si117x_WriteRegister(REG_FIFO_FLUSH, 0);

	return SendCmd(CMD_FLUSH_FIFO);
}

/***************************************************************************//**
 * @brief
 *   Sends the UPDATE_FIFO_COUNT command and returns the current FIFO count
 * @param[in] si117x_handle
 *   The programmer's toolkit handle
 * @param[in] rev_id
 *   The contents REV_ID register of the Si117x. 2 == -B2, 3 == -B3
 *   The resulting FIFO count
 * @retval  >= 0    the FIFO count
 * @retval  < 0      Error
 ******************************************************************************/
int32_t Si117xUpdateFifoCount(void)
{
	int32_t retval = 0;
	uint16_t high = 0, low = 0;
	uint32_t check = 0;
	
	retval = SendCmd(CMD_UPDATE_FIFO_COUNT);

	if (retval < 0) 
	{
		return retval;
	}

	high = Si117x_ReadRegister(REG_FIFO_COUNT_H);
	low =  Si117x_ReadRegister(REG_FIFO_COUNT_L);

	retval = (high << 8) + low;
	
	
	//注意：防止出错，超过FIFO的大小
	check = (uint32_t)retval;
	if(check > 2048)
	{
		return -(0xFB << 16);
	}
	
	return retval;
}

uint32_t Si117xGetIrqStatus(void)
{
	static uint32_t irq_status;

	irq_status = Si117x_ReadRegister(REG_IRQ_STATUS);         //si1171b2 地址是0x18

	if(irq_status)
	{
		Si117x_WriteRegister(REG_HOSTIN0, irq_status);
		SendCmd(CMD_CLEAR_IRQ_STATUS);
	}

	return irq_status;
}

void Si1182FifoRead(uint8_t *out_data,const uint32_t fifo_byte)
{
	uint32_t fifo_data_byte = fifo_byte;
	uint32_t bytesToRead = 0;
	uint32_t out_data_address = 0;
	
	while(fifo_data_byte > 0)
	{
		bytesToRead = fifo_data_byte;    

		if(bytesToRead > 255)
		{
			bytesToRead = 255;
		}
		
		Si117x_BlockRead(REG_FIFO_READ, &out_data[out_data_address], bytesToRead);
		
		fifo_data_byte -= bytesToRead;
		out_data_address += bytesToRead;
	}
	
	return;
}


/////////////////////////////////////////////////////////////////////////////////
////
////					 si1182的配置只需关心以下部分							////
////
/////////////////////////////////////////////////////////////////////////////////


/****************************************************************
** Function name:             Si1182_InitPowerOn
** Descriptions:              上电初始化
** input parameters:                                      
** output parameters:         
** Returned value:            
** Created by:                JSH
** Created Date:              2018-12-21
****************************************************************/
static int32_t Si1182_InitPowerOn(HANDLE si117x_handle)
{
    int32_t retval;  

	SI1182_SPI_CS_HIGH	

    retval = Si117xReset(si117x_handle); 
    delay_10ms(); 
    retval += Si117xWriteToRegister(si117x_handle, REG_HW_KEY, 0x00); 

    /* Configure global parameters */
    retval += Si117xParamSet(si117x_handle, PARAM_TASK_ENABLE, 0x00); 

    /* Enable FIFO interrupt */
    retval += Si117xWriteToRegister(si117x_handle, REG_IRQ_ENABLE, 0x00); 

    return retval;
}

/****************************************************************
** Function name:             Si1182_ReadParameterConfig
** Descriptions:              读取si1182的参数配置
** input parameters:                                      
** output parameters:         
** Returned value:            
** Created by:                JSH
** Created Date:              2019-1-15
****************************************************************/
void Si1182_ReadParameterConfig(void)
{
    int32_t ret;
	
	if(g_si1182_data.read_config == 0)
	{
		return;
	}
	else
	{
		g_si1182_data.read_config = 0;
		
		memset(g_si1182_data.param_config, 0x00, sizeof(g_si1182_data.param_config));
		
		// 读取si117x的全部参数
		for(uint32_t i = 0; i <= SI117X_MAX_ADDRESS; i++)
		{
			ret = Si117x_ReadConfig(i);
			
			if(ret < 0)
			{
				g_si1182_data.param_config[i] = 0xff;
			}
			else
			{
				g_si1182_data.param_config[i] = (uint8_t)ret;
			}
		}
	}
}

/****************************************************************
** Function name:             Si1182_InitEcg
** Descriptions:              初始化ecg
** input parameters:                                      
** output parameters:         
** Returned value:            
** Created by:                JSH
** Created Date:              2018-12-21
****************************************************************/
//static int32_t Si1182_InitEcg(HANDLE si117x_handle)
//{
//    int32_t retval;  

//	SI1182_SPI_CS_HIGH	

//    retval = Si117xReset(si117x_handle); 
//    delay_10ms(); 
//    retval += Si117xWriteToRegister(si117x_handle, REG_HW_KEY, 0x00); 
//	
//    /* Configure global parameters */
//    retval += Si117xParamSet(si117x_handle, PARAM_TASK_ENABLE, 0x40); 
//    retval += Si117xParamSet(si117x_handle, PARAM_MEAS_RATE_H, 0x00); 
//    retval += Si117xParamSet(si117x_handle, PARAM_MEAS_RATE_L, 0x50); 
//    retval += Si117xParamSet(si117x_handle, PARAM_ECG_MEASCOUNT, 0x01); 
//    retval += Si117xParamSet(si117x_handle, PARAM_FIFO_INT_LEVEL_H, 0x80); 
//    retval += Si117xParamSet(si117x_handle, PARAM_FIFO_INT_LEVEL_L, 0xfa); 
//    retval += Si117xParamSet(si117x_handle, PARAM_MEAS_CNTL, 0x04); 

//    /* Configure ECG channel parameters */
//	
//	retval += Si117xParamSet (si117x_handle, PARAM_ECG_MEASCONFIG, 0x07);
//	retval += Si117xParamSet (si117x_handle, PARAM_ECG_ADCCONFIG, 0xA0);
//	retval += Si117xParamSet (si117x_handle, PARAM_ECG_FECONFIG, 0x40);

//    /* Enable FIFO interrupt */
//    retval += Si117xWriteToRegister(si117x_handle, REG_IRQ_ENABLE, 0x01); 

//    return retval;
//}

/****************************************************************
** Function name:             Si1182_InitEcg
** Descriptions:              初始化ecg
** input parameters:                                      
** output parameters:         
** Returned value:            
** Created by:                JSH
** Created Date:              2018-12-21
****************************************************************/
static int32_t Si1182_InitEcgAndLd(HANDLE si117x_handle)
{
    int32_t retval = 0;  
    
	SI1182_SPI_CS_HIGH	

    retval = Si117xReset(si117x_handle); 
    delay_10ms(); 
    retval += Si117xWriteToRegister(si117x_handle, REG_HW_KEY, 0x00); 
	
    //WJ.20190124.对电容充电
    Si117xParamSet(si117x_handle, PARAM_TASK_ENABLE, 0x40);
    Si117xParamSet(si117x_handle, PARAM_ECG_ADCCONFIG, 0x08);
    delay_10ms();
    
    
    /* Configure global parameters */
#if ECG_LD_TEST	
    retval += Si117xParamSet(si117x_handle, PARAM_TASK_ENABLE, 0xc0 & 0xBF);//0x7F);   
#else    
    retval += Si117xParamSet(si117x_handle, PARAM_TASK_ENABLE, 0xc0 & 0x7F); 
#endif

    retval += Si117xParamSet(si117x_handle, PARAM_MEAS_RATE_H, 0x00); 
    retval += Si117xParamSet(si117x_handle, PARAM_MEAS_RATE_L, 0x28); 
    retval += Si117xParamSet(si117x_handle, PARAM_ECG_MEASCOUNT, 0x02); 
    retval += Si117xParamSet(si117x_handle, PARAM_FIFO_INT_LEVEL_H, 0x80); 
#if ECG_LD_TEST	    
    retval += Si117xParamSet(si117x_handle, PARAM_FIFO_INT_LEVEL_L, 0x01);//0x05);//0xff); //导联测试使用一个样本数
#else
    retval += Si117xParamSet(si117x_handle, PARAM_FIFO_INT_LEVEL_L, 250); //ECG测试使用250个样本数
#endif

    retval += Si117xParamSet(si117x_handle, PARAM_MEAS_CNTL, 0x04); 
	retval += Si117xParamSet(si117x_handle, PARAM_INPUT_FREQ_SEL, 0x50);
	
    /* Configure ECG channel parameters */
	
	retval += Si117xParamSet(si117x_handle, PARAM_ECG_MEASCONFIG, 0x0f);
	retval += Si117xParamSet(si117x_handle, PARAM_ECG_ADCCONFIG, 0xA0);
	retval += Si117xParamSet(si117x_handle, PARAM_ECG_FECONFIG, 0x40);

	retval += Si117xParamSet(si117x_handle, PARAM_ECG_THRESHOLD, 0x40);
	retval += Si117xParamSet(si117x_handle, PARAM_ECG_LD_MEASCOUNT, LD_MC_INT);//0x19*5);// 125导联采样检测为1s
    
#if ECG_LD_TEST	
	retval += Si117xParamSet(si117x_handle, PARAM_ECG_LDCONFIG, 0x05 ); //导联数据加入FIFO
#else
	retval += Si117xParamSet(si117x_handle, PARAM_ECG_LDCONFIG, 0x05 | ECG_LD_INTO_FIFO); //导联数据不加入FIFO
#endif

    /* Enable FIFO interrupt */
    retval += Si117xWriteToRegister(si117x_handle, REG_IRQ_ENABLE, 0x01);// | (1<<3)); //FIFO中断和LD中断

    return retval;
}

/****************************************************************
** Function name:             Si1182_InitPpg
** Descriptions:              初始化ppg,开绿光、环境光
** input parameters:                                      
** output parameters:         
** Returned value:            
** Created by:                JSH
** Created Date:              2018-12-21
****************************************************************/
// LED1\LED3:红光；LED2:绿光；LED4:红外
static int32_t Si1182_InitGreenAmb(HANDLE si117x_handle)
{
    int32_t retval;            

	SI1182_SPI_CS_HIGH
 
    retval = Si117xReset(si117x_handle); 
    delay_10ms(); 
    retval += Si117xWriteToRegister(si117x_handle, REG_HW_KEY, 0x00); 

    /* Configure global parameters */
    retval += Si117xParamSet(si117x_handle, PARAM_TASK_ENABLE, 0x03); 
    retval += Si117xParamSet(si117x_handle, PARAM_MEAS_RATE_H, 0x01); 
    retval += Si117xParamSet(si117x_handle, PARAM_MEAS_RATE_L, 0x90); 
    retval += Si117xParamSet(si117x_handle, PARAM_PPG_MEASCOUNT, 0x01); 
    retval += Si117xParamSet(si117x_handle, PARAM_FIFO_INT_LEVEL_H, 0x80); 
    retval += Si117xParamSet(si117x_handle, PARAM_FIFO_INT_LEVEL_L, 0x64); 
    retval += Si117xParamSet(si117x_handle, PARAM_MEAS_CNTL, 0x01); 

    /* Configure PPG1 channel parameters */
    retval += Si117xParamSet(si117x_handle, PARAM_PPG1_LED1_CONFIG, 0x4b); 
    retval += Si117xParamSet(si117x_handle, PARAM_PPG1_LED2_CONFIG, 0x00); 
    retval += Si117xParamSet(si117x_handle, PARAM_PPG1_LED3_CONFIG, 0x4b); 
    retval += Si117xParamSet(si117x_handle, PARAM_PPG1_LED4_CONFIG, 0x00); 
    retval += Si117xParamSet(si117x_handle, PARAM_PPG1_MODE, 0x0f); 
    retval += Si117xParamSet(si117x_handle, PARAM_PPG1_MEASCONFIG, 0x05); 
    retval += Si117xParamSet(si117x_handle, PARAM_PPG1_ADCCONFIG, 0x70); 
	
	/* Configure PPG2 channel parameters */
    retval += Si117xParamSet(si117x_handle, PARAM_PPG2_LED1_CONFIG, 0x00); 
    retval += Si117xParamSet(si117x_handle, PARAM_PPG2_LED2_CONFIG, 0x00); 
    retval += Si117xParamSet(si117x_handle, PARAM_PPG2_LED3_CONFIG, 0x00); 
    retval += Si117xParamSet(si117x_handle, PARAM_PPG2_LED4_CONFIG, 0x4b); 
    retval += Si117xParamSet(si117x_handle, PARAM_PPG2_MODE, 0x0f); 
    retval += Si117xParamSet(si117x_handle, PARAM_PPG2_MEASCONFIG, 0x05); 
    retval += Si117xParamSet(si117x_handle, PARAM_PPG2_ADCCONFIG, 0x70);
	
	/* Configure PPG3 channel parameters */
    retval += Si117xParamSet(si117x_handle, PARAM_PPG3_LED1_CONFIG, 0x00); 
    retval += Si117xParamSet(si117x_handle, PARAM_PPG3_LED2_CONFIG, 0x00); 
    retval += Si117xParamSet(si117x_handle, PARAM_PPG3_LED3_CONFIG, 0x00); 
    retval += Si117xParamSet(si117x_handle, PARAM_PPG3_LED4_CONFIG, 0x00); 
    retval += Si117xParamSet(si117x_handle, PARAM_PPG3_MODE, 0x0f); 
    retval += Si117xParamSet(si117x_handle, PARAM_PPG3_MEASCONFIG, 0x05); 
    retval += Si117xParamSet(si117x_handle, PARAM_PPG3_ADCCONFIG, 0x70);
	
	/* Configure PPG4 channel parameters */
    retval += Si117xParamSet(si117x_handle, PARAM_PPG4_LED1_CONFIG, 0x00); 
    retval += Si117xParamSet(si117x_handle, PARAM_PPG4_LED2_CONFIG, 0x00); 
    retval += Si117xParamSet(si117x_handle, PARAM_PPG4_LED3_CONFIG, 0x00); 
    retval += Si117xParamSet(si117x_handle, PARAM_PPG4_LED4_CONFIG, 0x00); 
    retval += Si117xParamSet(si117x_handle, PARAM_PPG4_MODE, 0x0f); 
    retval += Si117xParamSet(si117x_handle, PARAM_PPG4_MEASCONFIG, 0x05); 
    retval += Si117xParamSet(si117x_handle, PARAM_PPG4_ADCCONFIG, 0x70);

    /* Enable FIFO interrupt */
    retval += Si117xWriteToRegister(si117x_handle, REG_IRQ_ENABLE, 0x01); 

    return retval;
}

/****************************************************************
** Function name:             Si1182_InitPpg
** Descriptions:              初始化ppg,开绿光、红外、环境光
** input parameters:                                      
** output parameters:         
** Returned value:            
** Created by:                JSH
** Created Date:              2018-12-21
****************************************************************/
// LED1\LED3:红光；LED2:绿光；LED4:红外
static int32_t Si1182_InitGreenIredAmb(HANDLE si117x_handle)
{
    int32_t retval;            

	SI1182_SPI_CS_HIGH
 
    retval = Si117xReset(si117x_handle); 
    delay_10ms(); 
    retval += Si117xWriteToRegister(si117x_handle, REG_HW_KEY, 0x00); 

    /* Configure global parameters */
    retval += Si117xParamSet(si117x_handle, PARAM_TASK_ENABLE, 0x07); 
    retval += Si117xParamSet(si117x_handle, PARAM_MEAS_RATE_H, 0x01); 
    retval += Si117xParamSet(si117x_handle, PARAM_MEAS_RATE_L, 0x90); 
    retval += Si117xParamSet(si117x_handle, PARAM_PPG_MEASCOUNT, 0x01); 
    retval += Si117xParamSet(si117x_handle, PARAM_FIFO_INT_LEVEL_H, 0x80); 
    retval += Si117xParamSet(si117x_handle, PARAM_FIFO_INT_LEVEL_L, 0x96); 
    retval += Si117xParamSet(si117x_handle, PARAM_MEAS_CNTL, 0x01);   
    
    /* Configure PPG1 channel parameters */
    retval += Si117xParamSet(si117x_handle, PARAM_PPG1_LED1_CONFIG, 0x00); 
    retval += Si117xParamSet(si117x_handle, PARAM_PPG1_LED2_CONFIG, 0x4B); 
    retval += Si117xParamSet(si117x_handle, PARAM_PPG1_LED3_CONFIG, 0x00); 
    retval += Si117xParamSet(si117x_handle, PARAM_PPG1_LED4_CONFIG, 0x00); 
    retval += Si117xParamSet(si117x_handle, PARAM_PPG1_MODE, 0x0f); 
    retval += Si117xParamSet(si117x_handle, PARAM_PPG1_MEASCONFIG, 0x05); 
    // WJ.20190412.按照可以需求修改。FWT
    retval += Si117xParamSet(si117x_handle, PARAM_PPG1_ADCCONFIG, 0xB0);//0x70); 
 
	/* Configure PPG2 channel parameters */
    retval += Si117xParamSet(si117x_handle, PARAM_PPG2_LED1_CONFIG, 0x00); 
    retval += Si117xParamSet(si117x_handle, PARAM_PPG2_LED2_CONFIG, 0x00); 
    retval += Si117xParamSet(si117x_handle, PARAM_PPG2_LED3_CONFIG, 0x00); 
    retval += Si117xParamSet(si117x_handle, PARAM_PPG2_LED4_CONFIG, 0x4B); 
    retval += Si117xParamSet(si117x_handle, PARAM_PPG2_MODE, 0x0f); 
    retval += Si117xParamSet(si117x_handle, PARAM_PPG2_MEASCONFIG, 0x05);
		// FWT.20190416  降低红外功耗 		
    retval += Si117xParamSet(si117x_handle, PARAM_PPG2_ADCCONFIG, 0x30);


	/* Configure PPG3 channel parameters */
    retval += Si117xParamSet(si117x_handle, PARAM_PPG3_LED1_CONFIG, 0x00); 
    retval += Si117xParamSet(si117x_handle, PARAM_PPG3_LED2_CONFIG, 0x00); 
    retval += Si117xParamSet(si117x_handle, PARAM_PPG3_LED3_CONFIG, 0x00); 
    retval += Si117xParamSet(si117x_handle, PARAM_PPG3_LED4_CONFIG, 0x00); 
    retval += Si117xParamSet(si117x_handle, PARAM_PPG3_MODE, 0x0f); 
    retval += Si117xParamSet(si117x_handle, PARAM_PPG3_MEASCONFIG, 0x15); 
    retval += Si117xParamSet(si117x_handle, PARAM_PPG3_ADCCONFIG, 0x70);

   
	/* Configure PPG4 channel parameters */
    retval += Si117xParamSet(si117x_handle, PARAM_PPG4_LED1_CONFIG, 0x00); 
    retval += Si117xParamSet(si117x_handle, PARAM_PPG4_LED2_CONFIG, 0x00); 
    retval += Si117xParamSet(si117x_handle, PARAM_PPG4_LED3_CONFIG, 0x00); 
    retval += Si117xParamSet(si117x_handle, PARAM_PPG4_LED4_CONFIG, 0x00); 
    retval += Si117xParamSet(si117x_handle, PARAM_PPG4_MODE, 0x0f); 
    retval += Si117xParamSet(si117x_handle, PARAM_PPG4_MEASCONFIG, 0x05); 
    retval += Si117xParamSet(si117x_handle, PARAM_PPG4_ADCCONFIG, 0x70);

    /* Enable FIFO interrupt */
    retval += Si117xWriteToRegister(si117x_handle, REG_IRQ_ENABLE, 0x01); 

    return retval;
}

/****************************************************************
** Function name:             Si1182_InitPpg
** Descriptions:              初始化ppg,开绿光、红光、红外、环境光
** input parameters:                                      
** output parameters:         
** Returned value:            
** Created by:                JSH
** Created Date:              2018-12-21
****************************************************************/
// LED1\LED3:红光；LED2:绿光；LED4:红外
static int32_t Si1182_InitGreenRedIredAmb(HANDLE si117x_handle)
{
	// FWT.20190416.夜间功耗太高  优化！
//////    int32_t retval;            

//////	SI1182_SPI_CS_HIGH
////// 
//////    retval = Si117xReset(si117x_handle); 
//////    delay_10ms(); 
//////    retval += Si117xWriteToRegister(si117x_handle, REG_HW_KEY, 0x00); 

//////    /* Configure global parameters */
//////    retval += Si117xParamSet(si117x_handle, PARAM_TASK_ENABLE, 0x0f); 
//////    retval += Si117xParamSet(si117x_handle, PARAM_MEAS_RATE_H, 0x01); 
//////    retval += Si117xParamSet(si117x_handle, PARAM_MEAS_RATE_L, 0x90); 
//////    retval += Si117xParamSet(si117x_handle, PARAM_PPG_MEASCOUNT, 0x01); 
//////    retval += Si117xParamSet(si117x_handle, PARAM_FIFO_INT_LEVEL_H, 0x80); 
//////    retval += Si117xParamSet(si117x_handle, PARAM_FIFO_INT_LEVEL_L, 0xc8); 
//////    retval += Si117xParamSet(si117x_handle, PARAM_MEAS_CNTL, 0x01); 

//////    /* Configure PPG1 channel parameters */
//////    retval += Si117xParamSet(si117x_handle, PARAM_PPG1_LED1_CONFIG, 0x00); 
//////    retval += Si117xParamSet(si117x_handle, PARAM_PPG1_LED2_CONFIG, 0x4b); 
//////    retval += Si117xParamSet(si117x_handle, PARAM_PPG1_LED3_CONFIG, 0x00); 
//////    retval += Si117xParamSet(si117x_handle, PARAM_PPG1_LED4_CONFIG, 0x00); 
//////    retval += Si117xParamSet(si117x_handle, PARAM_PPG1_MODE, 0x0f); 
//////    retval += Si117xParamSet(si117x_handle, PARAM_PPG1_MEASCONFIG, 0x05); 
//////    retval += Si117xParamSet(si117x_handle, PARAM_PPG1_ADCCONFIG, 0x70); 
//////	
//////	/* Configure PPG2 channel parameters */
//////    retval += Si117xParamSet(si117x_handle, PARAM_PPG2_LED1_CONFIG, 0x4b); 
//////    retval += Si117xParamSet(si117x_handle, PARAM_PPG2_LED2_CONFIG, 0x00); 
//////    retval += Si117xParamSet(si117x_handle, PARAM_PPG2_LED3_CONFIG, 0x4b); 
//////    retval += Si117xParamSet(si117x_handle, PARAM_PPG2_LED4_CONFIG, 0x00); 
//////    retval += Si117xParamSet(si117x_handle, PARAM_PPG2_MODE, 0x0f); 
//////    retval += Si117xParamSet(si117x_handle, PARAM_PPG2_MEASCONFIG, 0x05); 
//////    retval += Si117xParamSet(si117x_handle, PARAM_PPG2_ADCCONFIG, 0x70);
//////	
//////	/* Configure PPG3 channel parameters */
//////    retval += Si117xParamSet(si117x_handle, PARAM_PPG3_LED1_CONFIG, 0x00); 
//////    retval += Si117xParamSet(si117x_handle, PARAM_PPG3_LED2_CONFIG, 0x00); 
//////    retval += Si117xParamSet(si117x_handle, PARAM_PPG3_LED3_CONFIG, 0x00); 
//////    retval += Si117xParamSet(si117x_handle, PARAM_PPG3_LED4_CONFIG, 0x4b); 
//////    retval += Si117xParamSet(si117x_handle, PARAM_PPG3_MODE, 0x0f); 
//////    retval += Si117xParamSet(si117x_handle, PARAM_PPG3_MEASCONFIG, 0x05); 
//////    retval += Si117xParamSet(si117x_handle, PARAM_PPG3_ADCCONFIG, 0x70);
//////	
//////	/* Configure PPG4 channel parameters */
//////    retval += Si117xParamSet(si117x_handle, PARAM_PPG4_LED1_CONFIG, 0x00); 
//////    retval += Si117xParamSet(si117x_handle, PARAM_PPG4_LED2_CONFIG, 0x00); 
//////    retval += Si117xParamSet(si117x_handle, PARAM_PPG4_LED3_CONFIG, 0x00); 
//////    retval += Si117xParamSet(si117x_handle, PARAM_PPG4_LED4_CONFIG, 0x00); 
//////    retval += Si117xParamSet(si117x_handle, PARAM_PPG4_MODE, 0x0f); 
//////    retval += Si117xParamSet(si117x_handle, PARAM_PPG4_MEASCONFIG, 0x15); 
//////    retval += Si117xParamSet(si117x_handle, PARAM_PPG4_ADCCONFIG, 0xb0);

//////    /* Enable FIFO interrupt */
//////    retval += Si117xWriteToRegister(si117x_handle, REG_IRQ_ENABLE, 0x01); 

//////    return retval;
    int32_t retval;            

	SI1182_SPI_CS_HIGH
 
    retval = Si117xReset(si117x_handle); 
    delay_10ms(); 
    retval += Si117xWriteToRegister(si117x_handle, REG_HW_KEY, 0x00); 

    /* Configure global parameters */
    retval += Si117xParamSet(si117x_handle, PARAM_TASK_ENABLE, 0x0f); 
    retval += Si117xParamSet(si117x_handle, PARAM_MEAS_RATE_H, 0x01); 
    retval += Si117xParamSet(si117x_handle, PARAM_MEAS_RATE_L, 0x90); 
    retval += Si117xParamSet(si117x_handle, PARAM_PPG_MEASCOUNT, 0x01); 
    retval += Si117xParamSet(si117x_handle, PARAM_FIFO_INT_LEVEL_H, 0x80); 
    retval += Si117xParamSet(si117x_handle, PARAM_FIFO_INT_LEVEL_L, 0xc8); 
    retval += Si117xParamSet(si117x_handle, PARAM_MEAS_CNTL, 0x01); 

    /* Configure PPG1 channel parameters */
    retval += Si117xParamSet(si117x_handle, PARAM_PPG1_LED1_CONFIG, 0x00); 
    retval += Si117xParamSet(si117x_handle, PARAM_PPG1_LED2_CONFIG, 0x4b); 
    retval += Si117xParamSet(si117x_handle, PARAM_PPG1_LED3_CONFIG, 0x00); 
    retval += Si117xParamSet(si117x_handle, PARAM_PPG1_LED4_CONFIG, 0x00); 
    retval += Si117xParamSet(si117x_handle, PARAM_PPG1_MODE, 0x0f); 
    retval += Si117xParamSet(si117x_handle, PARAM_PPG1_MEASCONFIG, 0x05); 
		// 设置DECIM为256，降低开灯时间降低功耗
    retval += Si117xParamSet(si117x_handle, PARAM_PPG1_ADCCONFIG, 0xB0); 
	
	/* Configure PPG2 channel parameters */
	
		// 设置红光1.7mA电流 该电流为已知最小电流，降低功耗 FWT。20190416
    retval += Si117xParamSet(si117x_handle, PARAM_PPG2_LED1_CONFIG, 0x56); 
    retval += Si117xParamSet(si117x_handle, PARAM_PPG2_LED2_CONFIG, 0x00); 
    retval += Si117xParamSet(si117x_handle, PARAM_PPG2_LED3_CONFIG, 0x56); 
    retval += Si117xParamSet(si117x_handle, PARAM_PPG2_LED4_CONFIG, 0x00); 
    retval += Si117xParamSet(si117x_handle, PARAM_PPG2_MODE, 0x0f); 
    retval += Si117xParamSet(si117x_handle, PARAM_PPG2_MEASCONFIG, 0x05);
		// 设置DECIM为64，降低开灯时间降低功耗
    retval += Si117xParamSet(si117x_handle, PARAM_PPG2_ADCCONFIG, 0xB0);
	
	/* Configure PPG3 channel parameters */
    retval += Si117xParamSet(si117x_handle, PARAM_PPG3_LED1_CONFIG, 0x00); 
    retval += Si117xParamSet(si117x_handle, PARAM_PPG3_LED2_CONFIG, 0x00); 
    retval += Si117xParamSet(si117x_handle, PARAM_PPG3_LED3_CONFIG, 0x00); 
    retval += Si117xParamSet(si117x_handle, PARAM_PPG3_LED4_CONFIG, 0x56); 
    retval += Si117xParamSet(si117x_handle, PARAM_PPG3_MODE, 0x0f); 
    retval += Si117xParamSet(si117x_handle, PARAM_PPG3_MEASCONFIG, 0x05); 
		// 设置DECIM为64，降低开灯时间降低功耗
    retval += Si117xParamSet(si117x_handle, PARAM_PPG3_ADCCONFIG, 0xB0);
	
	/* Configure PPG4 channel parameters */
    retval += Si117xParamSet(si117x_handle, PARAM_PPG4_LED1_CONFIG, 0x00); 
    retval += Si117xParamSet(si117x_handle, PARAM_PPG4_LED2_CONFIG, 0x00); 
    retval += Si117xParamSet(si117x_handle, PARAM_PPG4_LED3_CONFIG, 0x00); 
    retval += Si117xParamSet(si117x_handle, PARAM_PPG4_LED4_CONFIG, 0x00); 
    retval += Si117xParamSet(si117x_handle, PARAM_PPG4_MODE, 0x0f); 
    retval += Si117xParamSet(si117x_handle, PARAM_PPG4_MEASCONFIG, 0x15); 
		// 设置DECIM为64，降低开灯时间降低功耗
    retval += Si117xParamSet(si117x_handle, PARAM_PPG4_ADCCONFIG, 0xB0);

    /* Enable FIFO interrupt */
    retval += Si117xWriteToRegister(si117x_handle, REG_IRQ_ENABLE, 0x01); 

    return retval;
}

/****************************************************************
** Function name:             Si1182_InitPpg
** Descriptions:              初始化ppg,开红光、红外、环境光
** input parameters:                                      
** output parameters:         
** Returned value:            
** Created by:                JSH
** Created Date:              2019-2-19
****************************************************************/
// LED1\LED3:红光；LED2:绿光；LED4:红外
static int32_t Si1182_InitRedIredAmb(HANDLE si117x_handle)
{
    int32_t retval;            

	SI1182_SPI_CS_HIGH
 
    retval = Si117xReset(si117x_handle); 
    delay_10ms(); 
    retval += Si117xWriteToRegister(si117x_handle, REG_HW_KEY, 0x00); 

    /* Configure global parameters */
    retval += Si117xParamSet(si117x_handle, PARAM_TASK_ENABLE, 0x07); 
    retval += Si117xParamSet(si117x_handle, PARAM_MEAS_RATE_H, 0x00); 
    retval += Si117xParamSet(si117x_handle, PARAM_MEAS_RATE_L, 0x32); 
    retval += Si117xParamSet(si117x_handle, PARAM_PPG_MEASCOUNT, 0x01); 
    retval += Si117xParamSet(si117x_handle, PARAM_FIFO_INT_LEVEL_H, 0x80); 
    retval += Si117xParamSet(si117x_handle, PARAM_FIFO_INT_LEVEL_L, 0x96); 
    retval += Si117xParamSet(si117x_handle, PARAM_MEAS_CNTL, 0x1D); 

    /* Configure PPG1 channel parameters */
    retval += Si117xParamSet(si117x_handle, PARAM_PPG1_LED1_CONFIG, 0x4b); 
    retval += Si117xParamSet(si117x_handle, PARAM_PPG1_LED2_CONFIG, 0x00); 
    retval += Si117xParamSet(si117x_handle, PARAM_PPG1_LED3_CONFIG, 0x4b); 
    retval += Si117xParamSet(si117x_handle, PARAM_PPG1_LED4_CONFIG, 0x00); 
    retval += Si117xParamSet(si117x_handle, PARAM_PPG1_MODE, 0x0f); 
    retval += Si117xParamSet(si117x_handle, PARAM_PPG1_MEASCONFIG, 0x05); 
    retval += Si117xParamSet(si117x_handle, PARAM_PPG1_ADCCONFIG, 0xB0); 
	
	/* Configure PPG2 channel parameters */
    retval += Si117xParamSet(si117x_handle, PARAM_PPG2_LED1_CONFIG, 0x00); 
    retval += Si117xParamSet(si117x_handle, PARAM_PPG2_LED2_CONFIG, 0x00); 
    retval += Si117xParamSet(si117x_handle, PARAM_PPG2_LED3_CONFIG, 0x00); 
    retval += Si117xParamSet(si117x_handle, PARAM_PPG2_LED4_CONFIG, 0x4b); 
    retval += Si117xParamSet(si117x_handle, PARAM_PPG2_MODE, 0x0f); 
    retval += Si117xParamSet(si117x_handle, PARAM_PPG2_MEASCONFIG, 0x05); 
    retval += Si117xParamSet(si117x_handle, PARAM_PPG2_ADCCONFIG, 0xB0);
	
	/* Configure PPG3 channel parameters */
    retval += Si117xParamSet(si117x_handle, PARAM_PPG3_LED1_CONFIG, 0x00); 
    retval += Si117xParamSet(si117x_handle, PARAM_PPG3_LED2_CONFIG, 0x00); 
    retval += Si117xParamSet(si117x_handle, PARAM_PPG3_LED3_CONFIG, 0x00); 
    retval += Si117xParamSet(si117x_handle, PARAM_PPG3_LED4_CONFIG, 0x00); 
    retval += Si117xParamSet(si117x_handle, PARAM_PPG3_MODE, 0x0f); 
    retval += Si117xParamSet(si117x_handle, PARAM_PPG3_MEASCONFIG, 0x15); 
    retval += Si117xParamSet(si117x_handle, PARAM_PPG3_ADCCONFIG, 0xB0);
	
	/* Configure PPG4 channel parameters */
    retval += Si117xParamSet(si117x_handle, PARAM_PPG4_LED1_CONFIG, 0x00); 
    retval += Si117xParamSet(si117x_handle, PARAM_PPG4_LED2_CONFIG, 0x00); 
    retval += Si117xParamSet(si117x_handle, PARAM_PPG4_LED3_CONFIG, 0x00); 
    retval += Si117xParamSet(si117x_handle, PARAM_PPG4_LED4_CONFIG, 0x00); 
    retval += Si117xParamSet(si117x_handle, PARAM_PPG4_MODE, 0x0f); 
    retval += Si117xParamSet(si117x_handle, PARAM_PPG4_MEASCONFIG, 0x05); 
    retval += Si117xParamSet(si117x_handle, PARAM_PPG4_ADCCONFIG, 0xB0);

    /* Enable FIFO interrupt */
    retval += Si117xWriteToRegister(si117x_handle, REG_IRQ_ENABLE, 0x01); 

    return retval;
}

/****************************************************************
** Function name:             Si1182_ConfigPowerOn
** Descriptions:              上电初始化
** input parameters:                                      
** output parameters:         
** Returned value:            
** Created by:                JSH
** Created Date:              2018-12-21
****************************************************************/
int32_t Si1182_ConfigPowerOn(void)
{
	Si1182_PpgParamInit();
	Si1182_EcgParamInit();
	return Si1182_InitPowerOn(0);
}

/****************************************************************
** Function name:             Si1182_ConfigModeWithEcg
** Descriptions:              初始化ecg
** input parameters:                                      
** output parameters:         
** Returned value:            
** Created by:                JSH
** Created Date:              2018-12-21
****************************************************************/
int32_t Si1182_ConfigModeNull(void)
{
	return Si1182_InitPowerOn(0);
}

/****************************************************************
** Function name:             Si1182_ConfigModeWithEcg
** Descriptions:              初始化ecg
** input parameters:                                      
** output parameters:         
** Returned value:            
** Created by:                JSH
** Created Date:              2018-12-21
****************************************************************/
int32_t Si1182_ConfigModeWithEcg(void)
{
	int32_t ret_val = 0;
	
	ret_val += Si1182_InitEcgAndLd(0);
	
	si1182_ecg_param.ecg_change_flag = 1; 
	Si1182_EcgParamAdjust();
	
	return ret_val;
}

/****************************************************************
** Function name:             Si1182_ConfigModeWithGreenAmb
** Descriptions:              初始化ppg,开绿光、环境光
** input parameters:                                      
** output parameters:         
** Returned value:            
** Created by:                JSH
** Created Date:              2018-12-21
****************************************************************/
int32_t Si1182_ConfigModeWithGreenAmb(void)
{
	int32_t ret_val = 0;
	
	ret_val += Si1182_InitGreenAmb(0);

#if PPG_SET_TEST	
	si1182_ppg_param.ppg_change_flag_1 = 1;
	si1182_ppg_param.ppg_change_flag_2 = 1;
	Si1182_PpgParamAdjust();
#endif
	
	return ret_val;
}

/****************************************************************
** Function name:             Si1182_ConfigModeWithGreenIredAmb
** Descriptions:              初始化ppg,开绿光、红外、环境光
** input parameters:                                      
** output parameters:         
** Returned value:            
** Created by:                JSH
** Created Date:              2018-12-21
****************************************************************/
int32_t Si1182_ConfigModeWithGreenIredAmb(void)
{
	int32_t ret_val = 0;
	
	ret_val += Si1182_InitGreenIredAmb(0);
    
#if PPG_SET_TEST	
	si1182_ppg_param.ppg_change_flag_1 = 1;
	si1182_ppg_param.ppg_change_flag_2 = 1;
	Si1182_PpgParamAdjust();
#endif
	
	return ret_val;
}

/****************************************************************
** Function name:             Si1182_ConfigModeWithGreenRedIredAmb
** Descriptions:              初始化ppg,开绿光、红光、红外、环境光
** input parameters:                                      
** output parameters:         
** Returned value:            
** Created by:                JSH
** Created Date:              2018-12-21
****************************************************************/
int32_t Si1182_ConfigModeWithGreenRedIredAmb(void)
{
	int32_t ret_val = 0;
	
	ret_val += Si1182_InitGreenRedIredAmb(0);

#if PPG_SET_TEST	
	si1182_ppg_param.ppg_change_flag_1 = 1;
	si1182_ppg_param.ppg_change_flag_2 = 1;
	Si1182_PpgParamAdjust();
#endif
	
	return ret_val;
}

/****************************************************************
** Function name:             Si1182_ConfigModeWithRedIredAmb
** Descriptions:              初始化ppg,开红光、红外、环境光
** input parameters:                                      
** output parameters:         
** Returned value:            
** Created by:                JSH
** Created Date:              2019-2-19
****************************************************************/
int32_t Si1182_ConfigModeWithRedIredAmb(void)
{
	int32_t ret_val = 0;
	
	ret_val += Si1182_InitRedIredAmb(0);

#if PPG_SET_TEST	
	si1182_ppg_param.ppg_change_flag_1 = 1;
	si1182_ppg_param.ppg_change_flag_2 = 1;
	Si1182_PpgParamAdjust();
#endif
	
	return ret_val;
}

/****************************************************************
** Function name:             Si1182_EcgParamSet
** Descriptions:              1182 ECG参数设置
** input parameters:                                      
** output parameters:         
** Returned value:            
** Created by:                JSH
** Created Date:              2019-1-17
****************************************************************/
void Si1182_EcgParamSet(uint8_t ecg_gain, uint8_t ld_freq, uint8_t ld_current, uint8_t ld_gain,
					 uint8_t ld_shreshold, uint8_t ld_im, uint8_t ld_enable, uint8_t ecg_sw_avg,
					 uint8_t ecg_decim, uint8_t ecg_mc, uint8_t ld_mc, uint8_t mr_h,
					 uint8_t mr_l, uint8_t param_1, uint8_t param_2, uint8_t param_3)
{
	uint32_t change_flag = 0;
	
	// ECG增益
	if(ecg_gain <= 3)
	{
		if(si1182_ecg_param.ecg_gain != ecg_gain)
		{
			si1182_ecg_param.ecg_gain = ecg_gain;
			change_flag++;
		}
	}
	// 导联激励频率
	if(ld_freq <= 8)
	{
		if(si1182_ecg_param.ld_freq != ld_freq)
		{
			si1182_ecg_param.ld_freq = ld_freq;
			change_flag++;
		}
	}
	// 导联激励电流
	if(ld_current <= 7)
	{
		if(si1182_ecg_param.ld_current != ld_current)
		{
			si1182_ecg_param.ld_current = ld_current;
			change_flag++;
		}
	}
	// 导联增益
	if(ld_gain <= 1)
	{
		if(si1182_ecg_param.ld_gain != ld_gain)
		{
			si1182_ecg_param.ld_gain = ld_gain;
			change_flag++;
		}
	}
	// 导联阈值
	if(ld_shreshold <= 255)
	{
		if(si1182_ecg_param.ld_shreshold != ld_shreshold)
		{
			si1182_ecg_param.ld_shreshold = ld_shreshold;
			change_flag++;
		}
	}
	// 导联阈值极性
	if(ld_im <= 1)
	{
		if(si1182_ecg_param.ld_im != ld_im)
		{
			si1182_ecg_param.ld_im = ld_im;
			change_flag++;
		}
	}
	// 导联使能
	if(ld_enable <= 1)
	{
		if(si1182_ecg_param.ld_enable != ld_enable)
		{
			si1182_ecg_param.ld_enable = ld_enable;
			change_flag++;
		}
	}
	// SW_AVG
	if(ecg_sw_avg <= 7)
	{
		if(si1182_ecg_param.ecg_sw_avg != ecg_sw_avg)
		{
			si1182_ecg_param.ecg_sw_avg = ecg_sw_avg;
			change_flag++;
		}
	}
	// ECG抽样率
	if(ecg_decim <= 3)
	{
		if(si1182_ecg_param.ecg_decim != ecg_decim)
		{
			si1182_ecg_param.ecg_decim = ecg_decim;
			change_flag++;
		}
	}
	// ECG计数器
	if(ecg_mc <= 255)
	{
		if(si1182_ecg_param.ecg_mc != ecg_mc)
		{
			si1182_ecg_param.ecg_mc = ecg_mc;
			change_flag++;
		}
	}
	// 导联计数器
	if(ld_mc <= 255)
	{
		if(si1182_ecg_param.ld_mc != ld_mc)
		{
			si1182_ecg_param.ld_mc = ld_mc;
			change_flag++;
		}
	}
	// 基础测量速率-高
	if(mr_h <= 255)
	{
		if(si1182_ecg_param.mr_h != mr_h)
		{
			si1182_ecg_param.mr_h = mr_h;
			change_flag++;
		}
	}
	// 基础测量速率-低
	if(mr_l <= 255)
	{
		if(si1182_ecg_param.mr_l != mr_l)
		{
			si1182_ecg_param.mr_l = mr_l;
			change_flag++;
		}
	}
	// 保留参数-1
	if(param_1 <= 255)
	{
		if(si1182_ecg_param.param_1 != param_1)
		{
			si1182_ecg_param.param_1 = param_1;
			change_flag++;
		}
	}
	// 保留参数-2
	if(param_2 <= 255)
	{
		if(si1182_ecg_param.param_2 != param_2)
		{
			si1182_ecg_param.param_2 = param_2;
			change_flag++;
		}
	}
	// 保留参数-1
	if(param_3 <= 255)
	{
		if(si1182_ecg_param.param_3 != param_3)
		{
			si1182_ecg_param.param_3 = param_3;
			change_flag++;
		}
	}
	
	// 更新寄存器判断
	if(change_flag > 0)
	{
		si1182_ecg_param.ecg_change_flag = 1;
	}
}

/****************************************************************
** Function name:             Si1182_EcgParamGet
** Descriptions:              1182 ECG参数获取
** input parameters:                                      
** output parameters:         
** Returned value:            
** Created by:                JSH
** Created Date:              2019-1-17
****************************************************************/
void Si1182_EcgParamGet(uint8_t *ecg_gain, uint8_t *ld_freq, uint8_t *ld_current, uint8_t *ld_gain,
					 uint8_t *ld_shreshold, uint8_t *ld_im, uint8_t *ld_enable, uint8_t *ecg_sw_avg,
					 uint8_t *ecg_decim, uint8_t *ecg_mc, uint8_t *ld_mc, uint8_t *mr_h,
					 uint8_t *mr_l, uint8_t *param_1, uint8_t *param_2, uint8_t *param_3)
{
	if(ecg_gain != NULL)
	{
		*ecg_gain = si1182_ecg_param.ecg_gain;
	}
	if(ld_freq != NULL)
	{
		*ld_freq = si1182_ecg_param.ld_freq;
	}
	if(ld_current != NULL)
	{
		*ld_current = si1182_ecg_param.ld_current;
	}
	if(ld_gain != NULL)
	{
		*ld_gain = si1182_ecg_param.ld_gain;
	}
	if(ld_shreshold != NULL)
	{
		*ld_shreshold = si1182_ecg_param.ld_shreshold;
	}
	if(ld_im != NULL)
	{
		*ld_im = si1182_ecg_param.ld_im;
	}
	if(ld_enable != NULL)
	{
		*ld_enable = si1182_ecg_param.ld_enable;
	}
	if(ecg_sw_avg != NULL)
	{
		*ecg_sw_avg = si1182_ecg_param.ecg_sw_avg;
	}
	
	if(ecg_decim != NULL)
	{
		*ecg_decim = si1182_ecg_param.ecg_decim;
	}
	if(ecg_mc != NULL)
	{
		*ecg_mc = si1182_ecg_param.ecg_mc;
	}
	if(ld_mc != NULL)
	{
		*ld_mc = si1182_ecg_param.ld_mc;
	}
	if(mr_h != NULL)
	{
		*mr_h = si1182_ecg_param.mr_h;
	}
	if(mr_l != NULL)
	{
		*mr_l = si1182_ecg_param.mr_l;
	}
	if(param_1 != NULL)
	{
		*param_1 = si1182_ecg_param.param_1;
	}
	if(param_2 != NULL)
	{
		*param_2 = si1182_ecg_param.param_2;
	}
	if(param_3 != NULL)
	{
		*param_3 = si1182_ecg_param.param_3;
	}
}

/****************************************************************
** Function name:           Si1182_EcgParamInit
** Descriptions:            1182 ECG 默认参数
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              JSH             
** Created Date:            2019-1-17
*****************************************************************/ 
void Si1182_EcgParamInit(void)
{
    si1182_ecg_param.ecg_gain = ECG_GAIN_INIT;
	si1182_ecg_param.ld_freq = LD_FREQ_INIT;
	si1182_ecg_param.ld_current = LD_CURRENT_INT;
	si1182_ecg_param.ld_gain = LD_GAIN_INT;
	si1182_ecg_param.ld_shreshold = LD_SHRESHOLD_INT;
	si1182_ecg_param.ld_im = LD_IM_INT;
	si1182_ecg_param.ld_enable = LD_ENABLE_INT;
	si1182_ecg_param.ecg_sw_avg = ECG_SW_AVG_INT;
	si1182_ecg_param.ecg_decim = ECG_DECIM_INT;
	si1182_ecg_param.ecg_mc = ECG_MC_INT;
	si1182_ecg_param.ld_mc = LD_MC_INT;
	si1182_ecg_param.mr_h = ECG_MR_H_INT;
	si1182_ecg_param.mr_l = ECG_MR_L_INT;
	si1182_ecg_param.param_1 = ECG_PARAM_1_INT;
	si1182_ecg_param.param_2 = ECG_PARAM_2_INT;
	si1182_ecg_param.param_3 = ECG_PARAM_3_INT;
	si1182_ecg_param.ecg_change_flag = 1; 
}

/****************************************************************
** Function name:           Si1182_EcgParamAdjust
** Descriptions:            1182 ECG参数调节
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              JSH             
** Created Date:            2019-1-17
*****************************************************************/ 
void Si1182_EcgParamAdjust(void)
{
	int32_t retval = 0;
		
	if(si1182_ecg_param.ecg_change_flag == 0)
	{
		;
	}
	else
	{
		si1182_ecg_param.ecg_change_flag = 0;
#if ECG_LD_TEST		
#else        
		retval += Si117xParamSet(0, PARAM_TASK_ENABLE, 0x40 | (si1182_ecg_param.ld_enable << 7));
#endif        
		retval += Si117xParamSet(0, PARAM_MEAS_RATE_H, 0x00 | si1182_ecg_param.mr_h); 
		retval += Si117xParamSet(0, PARAM_MEAS_RATE_L, 0x00 | si1182_ecg_param.mr_l); 		
		retval += Si117xParamSet(0, PARAM_ECG_MEASCOUNT, 0x00 | si1182_ecg_param.ecg_mc); 
		retval += Si117xParamSet(0, PARAM_INPUT_FREQ_SEL, 0x00 | (si1182_ecg_param.ld_freq << 4));
		
		retval += Si117xParamSet(0, PARAM_ECG_MEASCONFIG, 0x07 | (si1182_ecg_param.ecg_sw_avg << 3));
		retval += Si117xParamSet(0, PARAM_ECG_ADCCONFIG, 0x20 | (si1182_ecg_param.ecg_decim << 6));
		retval += Si117xParamSet(0, PARAM_ECG_FECONFIG, 0x00 | (si1182_ecg_param.ecg_gain << 6));
		retval += Si117xParamSet(0, PARAM_ECG_THRESHOLD, 0x00 | si1182_ecg_param.ld_shreshold);
		retval += Si117xParamSet(0, PARAM_ECG_LD_MEASCOUNT, 0x00 | si1182_ecg_param.ld_mc);
        
#if ECG_LD_TEST	        
		retval += Si117xParamSet(0, PARAM_ECG_LDCONFIG, 0x00 | ECG_LD_INTO_FIFO 
                                                             | (si1182_ecg_param.ld_im << 7)
															 | (si1182_ecg_param.ld_gain << 3)
															 | (si1182_ecg_param.ld_current << 0));
#else
		retval += Si117xParamSet(0, PARAM_ECG_LDCONFIG, 0x00 | ECG_LD_INTO_FIFO 
                                                             | (si1182_ecg_param.ld_im << 7)
															 | (si1182_ecg_param.ld_gain << 3)
															 | (si1182_ecg_param.ld_current << 0));
#endif
		
		if(retval == 0)
		{
			return;
		}
	}
	
	return;
}

/****************************************************************
** Function name:           Si1182_PpgParamGet1
** Descriptions:            1182 参数获取
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              JSH             
** Created Date:            2019-1-17
*****************************************************************/ 
void Si1182_PpgParamGet1(uint8_t *task_enable, uint8_t *mr_h, uint8_t *mr_l, uint8_t *ppg_mc,
						 uint8_t *ppg1_led, uint8_t *ppg2_led, uint8_t *ppg3_led, uint8_t *ppg4_led,
						 uint8_t *ppg1_mode, uint8_t *ppg2_mode, uint8_t *ppg3_mode, uint8_t *ppg4_mode,
						 uint8_t *ppg1_mconfig, uint8_t *ppg2_mconfig, uint8_t *ppg3_mconfig, uint8_t *ppg4_mconfig)
{
	if(task_enable != NULL)
	{
		*task_enable = si1182_ppg_param.task_enable;
	}
	if(mr_h != NULL)
	{
		*mr_h = si1182_ppg_param.mr_h;
	}
	if(mr_l != NULL)
	{
		*mr_l = si1182_ppg_param.mr_l;
	}
	if(ppg_mc != NULL)
	{
		*ppg_mc = si1182_ppg_param.ppg_mc;
	}
	if(ppg1_led != NULL)
	{
		*ppg1_led = si1182_ppg_param.ppg1_led;
	}
	if(ppg2_led != NULL)
	{
		*ppg2_led = si1182_ppg_param.ppg2_led;
	}
	if(ppg3_led != NULL)
	{
		*ppg3_led = si1182_ppg_param.ppg3_led;
	}
	if(ppg4_led != NULL)
	{
		*ppg4_led = si1182_ppg_param.ppg4_led;
	}	
	if(ppg1_mode != NULL)
	{
		*ppg1_mode = si1182_ppg_param.ppg1_mode;
	}
	if(ppg2_mode != NULL)
	{
		*ppg2_mode = si1182_ppg_param.ppg2_mode;
	}
	if(ppg3_mode != NULL)
	{
		*ppg3_mode = si1182_ppg_param.ppg3_mode;
	}
	if(ppg4_mode != NULL)
	{
		*ppg4_mode = si1182_ppg_param.ppg4_mode;
	}
	if(ppg1_mconfig != NULL)
	{
		*ppg1_mconfig = si1182_ppg_param.ppg1_mconfig;
	}
	if(ppg2_mconfig != NULL)
	{
		*ppg2_mconfig = si1182_ppg_param.ppg2_mconfig;
	}
	if(ppg3_mconfig != NULL)
	{
		*ppg3_mconfig = si1182_ppg_param.ppg3_mconfig;
	}
	if(ppg4_mconfig != NULL)
	{
		*ppg4_mconfig = si1182_ppg_param.ppg4_mconfig;
	}	
}

/****************************************************************
** Function name:           Si1182_PpgParamGet2
** Descriptions:            1182 参数获取
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              JSH             
** Created Date:            2019-1-17
*****************************************************************/ 
void Si1182_PpgParamGet2(uint8_t *ppg1_decim, uint8_t *ppg1_adc_range, uint8_t *ppg1_clk_div, uint8_t *ppg2_decim,
						 uint8_t *ppg2_adc_range, uint8_t *ppg2_clk_div, uint8_t *ppg3_decim, uint8_t *ppg3_adc_range,
						 uint8_t *ppg3_clk_div, uint8_t *ppg4_decim, uint8_t *ppg4_adc_range, uint8_t *ppg4_clk_div,
						 uint8_t *param_1, uint8_t *param_2, uint8_t *param_3, uint8_t *param_4)
{
	if(ppg1_decim != NULL)
	{
		*ppg1_decim = si1182_ppg_param.ppg1_decim;
	}
	if(ppg1_adc_range != NULL)
	{
		*ppg1_adc_range = si1182_ppg_param.ppg1_adc_range;
	}
	if(ppg1_clk_div != NULL)
	{
		*ppg1_clk_div = si1182_ppg_param.ppg1_clk_div;
	}
	if(ppg2_decim != NULL)
	{
		*ppg2_decim = si1182_ppg_param.ppg2_decim;
	}
	if(ppg2_adc_range != NULL)
	{
		*ppg2_adc_range = si1182_ppg_param.ppg2_adc_range;
	}
	if(ppg2_clk_div != NULL)
	{
		*ppg2_clk_div = si1182_ppg_param.ppg2_clk_div;
	}
	if(ppg3_decim != NULL)
	{
		*ppg3_decim = si1182_ppg_param.ppg3_decim;
	}
	if(ppg3_adc_range != NULL)
	{
		*ppg3_adc_range = si1182_ppg_param.ppg3_adc_range;
	}	
	if(ppg3_clk_div != NULL)
	{
		*ppg3_clk_div = si1182_ppg_param.ppg3_clk_div;
	}
	if(ppg4_decim != NULL)
	{
		*ppg4_decim = si1182_ppg_param.ppg4_decim;
	}
	if(ppg4_adc_range != NULL)
	{
		*ppg4_adc_range = si1182_ppg_param.ppg4_adc_range;
	}
	if(ppg4_clk_div != NULL)
	{
		*ppg4_clk_div = si1182_ppg_param.ppg4_clk_div;
	}
	if(param_1 != NULL)
	{
		*param_1 = si1182_ppg_param.param_1;
	}
	if(param_2 != NULL)
	{
		*param_2 = si1182_ppg_param.param_2;
	}
	if(param_3 != NULL)
	{
		*param_3 = si1182_ppg_param.param_3;
	}
	if(param_4 != NULL)
	{
		*param_4 = si1182_ppg_param.param_4;
	}
}

/****************************************************************
** Function name:           Si1182_PpgParamSet1
** Descriptions:            1182 参数设置
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              JSH             
** Created Date:            2019-1-17
*****************************************************************/ 
void Si1182_PpgParamSet1(uint8_t task_enable, uint8_t mr_h, uint8_t mr_l, uint8_t ppg_mc,
						 uint8_t ppg1_led, uint8_t ppg2_led, uint8_t ppg3_led, uint8_t ppg4_led,
						 uint8_t ppg1_mode, uint8_t ppg2_mode, uint8_t ppg3_mode, uint8_t ppg4_mode,
						 uint8_t ppg1_mconfig, uint8_t ppg2_mconfig, uint8_t ppg3_mconfig, uint8_t ppg4_mconfig)
{
	uint32_t change_flag = 0;
	
	if(task_enable <= 255)
	{
		if(si1182_ppg_param.task_enable != task_enable)
		{
			si1182_ppg_param.task_enable = task_enable;
			change_flag++;
		}
	}
	if(mr_h <= 255)
	{
		if(si1182_ppg_param.mr_h != mr_h)
		{
			si1182_ppg_param.mr_h = mr_h;
			change_flag++;
		}
	}
	if(mr_l <= 255)
	{
		if(si1182_ppg_param.mr_l != mr_l)
		{
			si1182_ppg_param.mr_l = mr_l;
			change_flag++;
		}
	}
	if(ppg_mc <= 255)
	{
		if(si1182_ppg_param.ppg_mc != ppg_mc)
		{
			si1182_ppg_param.ppg_mc = ppg_mc;
			change_flag++;
		}
	}
	if(ppg1_led <= 255)
	{
		if(si1182_ppg_param.ppg1_led != ppg1_led)
		{
			si1182_ppg_param.ppg1_led = ppg1_led;
			change_flag++;
		}
	}
	if(ppg2_led <= 255)
	{
		if(si1182_ppg_param.ppg2_led != ppg2_led)
		{
			si1182_ppg_param.ppg2_led = ppg2_led;
			change_flag++;
		}
	}
	if(ppg3_led <= 255)
	{
		if(si1182_ppg_param.ppg3_led != ppg3_led)
		{
			si1182_ppg_param.ppg3_led = ppg3_led;
			change_flag++;
		}
	}
	if(ppg4_led <= 255)
	{
		if(si1182_ppg_param.ppg4_led != ppg4_led)
		{
			si1182_ppg_param.ppg4_led = ppg4_led;
			change_flag++;
		}
	}
	if(ppg1_mode <= 255)
	{
		if(si1182_ppg_param.ppg1_mode != ppg1_mode)
		{
			si1182_ppg_param.ppg1_mode = ppg1_mode;
			change_flag++;
		}
	}
	if(ppg2_mode <= 255)
	{
		if(si1182_ppg_param.ppg2_mode != ppg2_mode)
		{
			si1182_ppg_param.ppg2_mode = ppg2_mode;
			change_flag++;
		}
	}
	if(ppg3_mode <= 255)
	{
		if(si1182_ppg_param.ppg3_mode != ppg3_mode)
		{
			si1182_ppg_param.ppg3_mode = ppg3_mode;
			change_flag++;
		}
	}
	if(ppg4_mode <= 255)
	{
		if(si1182_ppg_param.ppg4_mode != ppg4_mode)
		{
			si1182_ppg_param.ppg4_mode = ppg4_mode;
			change_flag++;
		}
	}
	if(ppg1_mconfig <= 255)
	{
		if(si1182_ppg_param.ppg1_mconfig != ppg1_mconfig)
		{
			si1182_ppg_param.ppg1_mconfig = ppg1_mconfig;
			change_flag++;
		}
	}
	if(ppg2_mconfig <= 255)
	{
		if(si1182_ppg_param.ppg2_mconfig != ppg2_mconfig)
		{
			si1182_ppg_param.ppg2_mconfig = ppg2_mconfig;
			change_flag++;
		}
	}
	if(ppg3_mconfig <= 255)
	{
		if(si1182_ppg_param.ppg3_mconfig != ppg3_mconfig)
		{
			si1182_ppg_param.ppg3_mconfig = ppg3_mconfig;
			change_flag++;
		}
	}
	if(ppg4_mconfig <= 255)
	{
		if(si1182_ppg_param.ppg4_mconfig != ppg4_mconfig)
		{
			si1182_ppg_param.ppg4_mconfig = ppg4_mconfig;
			change_flag++;
		}
	}
	
	if(change_flag > 0)
	{
		si1182_ppg_param.ppg_change_flag_1 = 1;
	}
}

/****************************************************************
** Function name:           Si1182_PpgParamSet2
** Descriptions:            1182 参数设置
** input parameters:        无
** output parameters:       无
** Returned value:          无            
** Created by:              JSH             
** Created Date:            2019-1-17
*****************************************************************/ 
void Si1182_PpgParamSet2(uint8_t ppg1_decim, uint8_t ppg1_adc_range, uint8_t ppg1_clk_div, uint8_t ppg2_decim,
						 uint8_t ppg2_adc_range, uint8_t ppg2_clk_div, uint8_t ppg3_decim, uint8_t ppg3_adc_range,
						 uint8_t ppg3_clk_div, uint8_t ppg4_decim, uint8_t ppg4_adc_range, uint8_t ppg4_clk_div,
						 uint8_t param_1, uint8_t param_2, uint8_t param_3, uint8_t param_4)
{
	uint32_t change_flag = 0;
	
	if(ppg1_decim <= 3)
	{
		if(si1182_ppg_param.ppg1_decim != ppg1_decim)
		{
			si1182_ppg_param.ppg1_decim = ppg1_decim;
			change_flag++;
		}
	}
	if(ppg1_adc_range <= 3)
	{
		if(si1182_ppg_param.ppg1_adc_range != ppg1_adc_range)
		{
			si1182_ppg_param.ppg1_adc_range = ppg1_adc_range;
			change_flag++;
		}
	}
	if(ppg1_clk_div <= 12)
	{
		if(si1182_ppg_param.ppg1_clk_div != ppg1_clk_div)
		{
			si1182_ppg_param.ppg1_clk_div = ppg1_clk_div;
			change_flag++;
		}
	}
	if(ppg2_decim <= 3)
	{
		if(si1182_ppg_param.ppg2_decim != ppg2_decim)
		{
			si1182_ppg_param.ppg2_decim = ppg2_decim;
			change_flag++;
		}
	}
	if(ppg2_adc_range <= 3)
	{
		if(si1182_ppg_param.ppg2_adc_range != ppg2_adc_range)
		{
			si1182_ppg_param.ppg2_adc_range = ppg2_adc_range;
			change_flag++;
		}
	}
	if(ppg2_clk_div <= 12)
	{
		if(si1182_ppg_param.ppg2_clk_div != ppg2_clk_div)
		{
			si1182_ppg_param.ppg2_clk_div = ppg2_clk_div;
			change_flag++;
		}
	}
	if(ppg3_decim <= 3)
	{
		if(si1182_ppg_param.ppg3_decim != ppg3_decim)
		{
			si1182_ppg_param.ppg3_decim = ppg3_decim;
			change_flag++;
		}
	}
	if(ppg3_adc_range <= 3)
	{
		if(si1182_ppg_param.ppg3_adc_range != ppg3_adc_range)
		{
			si1182_ppg_param.ppg3_adc_range = ppg3_adc_range;
			change_flag++;
		}
	}
	if(ppg3_clk_div <= 12)
	{
		if(si1182_ppg_param.ppg3_clk_div != ppg3_clk_div)
		{
			si1182_ppg_param.ppg3_clk_div = ppg3_clk_div;
			change_flag++;
		}
	}
	if(ppg4_decim <= 3)
	{
		if(si1182_ppg_param.ppg4_decim != ppg4_decim)
		{
			si1182_ppg_param.ppg4_decim = ppg4_decim;
			change_flag++;
		}
	}
	if(ppg4_adc_range <= 3)
	{
		if(si1182_ppg_param.ppg4_adc_range != ppg4_adc_range)
		{
			si1182_ppg_param.ppg4_adc_range = ppg4_adc_range;
			change_flag++;
		}
	}
	if(ppg4_clk_div <= 12)
	{
		if(si1182_ppg_param.ppg4_clk_div != ppg4_clk_div)
		{
			si1182_ppg_param.ppg4_clk_div = ppg4_clk_div;
			change_flag++;
		}
	}
	if(param_1 <= 7)	// PPG_SW_AVG
	{
		if(si1182_ppg_param.param_1 != param_1)
		{
			si1182_ppg_param.param_1 = param_1;
			change_flag++;
		}
	}
	if(param_2 <= 255)
	{
		if(si1182_ppg_param.param_2 != param_2)
		{
			si1182_ppg_param.param_2 = param_2;
			change_flag++;
		}
	}
	if(param_3 <= 255)
	{
		if(si1182_ppg_param.param_3 != param_3)
		{
			si1182_ppg_param.param_3 = param_3;
			change_flag++;
		}
	}
	if(param_4 <= 255)
	{
		if(si1182_ppg_param.param_4 != param_4)
		{
			si1182_ppg_param.param_4 = param_4;
			change_flag++;
		}
	}
	
	if(change_flag > 0)
	{
		si1182_ppg_param.ppg_change_flag_2 = 1;
	}
}

/****************************************************************
** Function name:           Si1182_PpgParamInit
** Descriptions:            1182 PPG 参数初始化
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              JSH             
** Created Date:            2019-1-17
*****************************************************************/
void Si1182_PpgParamInit(void)
{
	si1182_ppg_param.task_enable = PPG_TASK_ENABLE_INIT;
	si1182_ppg_param.mr_h = PPG_MR_H_INIT;
	si1182_ppg_param.mr_l = PPG_MR_L_INT;
	si1182_ppg_param.ppg_mc = PPG_MC_INT;
	si1182_ppg_param.ppg1_led = PPG1_LED_INT;
	si1182_ppg_param.ppg2_led = PPG2_LED_INT;
	si1182_ppg_param.ppg3_led = PPG3_LED_INT;
	si1182_ppg_param.ppg4_led = PPG4_LED_INT;
	si1182_ppg_param.ppg1_mode = PPG1_MODE_INT;
	si1182_ppg_param.ppg2_mode = PPG2_MODE_INT;
	si1182_ppg_param.ppg3_mode = PPG3_MODE_INT;
	si1182_ppg_param.ppg4_mode = PPG4_MODE_INT;
	si1182_ppg_param.ppg1_mconfig = PPG1_MCONFIG_INT;
	si1182_ppg_param.ppg2_mconfig = PPG2_MCONFIG_INT;
	si1182_ppg_param.ppg3_mconfig = PPG3_MCONFIG_INT;
	si1182_ppg_param.ppg4_mconfig = PPG4_MCONFIG_INT;
	si1182_ppg_param.ppg1_decim = PPG1_DECIM_INT;
	si1182_ppg_param.ppg1_adc_range = PPG1_ADC_RANGE_INT;
	si1182_ppg_param.ppg1_clk_div = PPG1_CLK_DIV_INT;
	si1182_ppg_param.ppg2_decim = PPG2_DECIM_INT;
	si1182_ppg_param.ppg2_adc_range = PPG2_ADC_RANGE_INT;
	si1182_ppg_param.ppg2_clk_div = PPG2_CLK_DIV_INT;
	si1182_ppg_param.ppg3_decim = PPG3_DECIM_INT;
	si1182_ppg_param.ppg3_adc_range = PPG3_ADC_RANGE_INT;
	si1182_ppg_param.ppg3_clk_div = PPG3_CLK_DIV_INT;
	si1182_ppg_param.ppg4_decim = PPG4_DECIM_INT;
	si1182_ppg_param.ppg4_adc_range = PPG4_ADC_RANGE_INT;
	si1182_ppg_param.ppg4_clk_div = PPG4_CLK_DIV_INT;
	si1182_ppg_param.param_1 = PPG_PARAM_1_INT;
	si1182_ppg_param.param_2 = PPG_PARAM_2_INT;
	si1182_ppg_param.param_3 = PPG_PARAM_3_INT;
	si1182_ppg_param.param_4 = PPG_PARAM_4_INT;
	si1182_ppg_param.ppg_change_flag_1 = 1;
	si1182_ppg_param.ppg_change_flag_2 = 1; 
}

/****************************************************************
** Function name:           Si1182_PpgParamAdjust
** Descriptions:            1182 PPG 参数调节
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              JSH             
** Created Date:            2019-1-17
*****************************************************************/
void Si1182_PpgParamAdjust(void)
{
	int32_t retval = 0;
	    
	if(si1182_ppg_param.ppg_change_flag_1 == 0)
	{
		;
	}
	else
	{
		si1182_ppg_param.ppg_change_flag_1 = 0;
		
		//retval += Si117xParamSet(0, PARAM_TASK_ENABLE, 0x00 | si1182_ppg_param.task_enable);
		retval += Si117xParamSet(0, PARAM_MEAS_RATE_H, 0x01 | si1182_ppg_param.mr_h); 
		retval += Si117xParamSet(0, PARAM_MEAS_RATE_L, 0x00 | si1182_ppg_param.mr_l); 
		retval += Si117xParamSet(0, PARAM_PPG_MEASCOUNT, 0x00 | si1182_ppg_param.ppg_mc);
		retval += Si117xParamSet(0, PARAM_MEAS_CNTL, 0x01 | (si1182_ppg_param.param_1 & 0x07) << 3); 
		
		retval += Si117xParamSet(0, PARAM_PPG1_LED1_CONFIG, 0x00 | si1182_ppg_param.ppg1_led);
		retval += Si117xParamSet(0, PARAM_PPG2_LED2_CONFIG, 0x00 | si1182_ppg_param.ppg2_led);
		retval += Si117xParamSet(0, PARAM_PPG3_LED3_CONFIG, 0x00 | si1182_ppg_param.ppg3_led);
		retval += Si117xParamSet(0, PARAM_PPG4_LED4_CONFIG, 0x00 | si1182_ppg_param.ppg4_led);
		
		retval += Si117xParamSet(0, PARAM_PPG1_MODE, 0x00 | si1182_ppg_param.ppg1_mode);
		retval += Si117xParamSet(0, PARAM_PPG2_MODE, 0x00 | si1182_ppg_param.ppg2_mode);
		retval += Si117xParamSet(0, PARAM_PPG3_MODE, 0x00 | si1182_ppg_param.ppg3_mode);
		retval += Si117xParamSet(0, PARAM_PPG4_MODE, 0x00 | si1182_ppg_param.ppg4_mode);	

		retval += Si117xParamSet(0, PARAM_PPG1_MEASCONFIG, 0x00 | si1182_ppg_param.ppg1_mconfig); 
		retval += Si117xParamSet(0, PARAM_PPG2_MEASCONFIG, 0x00 | si1182_ppg_param.ppg2_mconfig); 
		retval += Si117xParamSet(0, PARAM_PPG3_MEASCONFIG, 0x00 | si1182_ppg_param.ppg3_mconfig); 
		retval += Si117xParamSet(0, PARAM_PPG4_MEASCONFIG, 0x00 | si1182_ppg_param.ppg4_mconfig); 
		
		if(retval == 0)
		{
			;
		}
	}
	
	if(si1182_ppg_param.ppg_change_flag_2 == 0)
	{
		;
	}
	else
	{
		si1182_ppg_param.ppg_change_flag_2 = 0;
		
		retval += Si117xParamSet(0, PARAM_PPG1_ADCCONFIG, 0x00 | ((si1182_ppg_param.ppg1_decim & 0x03) << 6)
															   | ((si1182_ppg_param.ppg1_adc_range & 0x03) << 4)
															   | ((si1182_ppg_param.ppg1_clk_div & 0x0f) << 0));
		retval += Si117xParamSet(0, PARAM_PPG2_ADCCONFIG, 0x00 | ((si1182_ppg_param.ppg2_decim & 0x03) << 6)
															   | ((si1182_ppg_param.ppg2_adc_range & 0x03) << 4)
															   | ((si1182_ppg_param.ppg2_clk_div & 0x0f) << 0));
		retval += Si117xParamSet(0, PARAM_PPG3_ADCCONFIG, 0x00 | ((si1182_ppg_param.ppg3_decim & 0x03) << 6)
															   | ((si1182_ppg_param.ppg3_adc_range & 0x03) << 4)
															   | ((si1182_ppg_param.ppg3_clk_div & 0x0f) << 0));
		retval += Si117xParamSet(0, PARAM_PPG4_ADCCONFIG, 0x00 | ((si1182_ppg_param.ppg4_decim & 0x03) << 6)
															   | ((si1182_ppg_param.ppg4_adc_range & 0x03) << 4)
															   | ((si1182_ppg_param.ppg4_clk_div & 0x0f) << 0));													   
		
		if(retval == 0)
		{
			;
		}
	}
	
	return;
}

/****************************************************************
** Function name:           Si1182_ParamAdjust
** Descriptions:            1182 参数调节
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              JSH             
** Created Date:            2019-1-17
*****************************************************************/
void Si1182_ParamAdjust(void)
{	
	switch(g_si1182_data.adc_mode)
	{
		case MODE_NULL:
			
			break;
		case MODE_ECG:
			Si1182_EcgParamAdjust();
			break;
		case MODE_GREEN_AMB:
		case MODE_GREEN_IRED_AMB:
		case MODE_GREEN_RED_IRED_AMB:
		case MODE_RED_IRED_AMB:
#if PPG_SET_TEST            
			Si1182_PpgParamAdjust();
#endif        
			break;
		default :
			break;
	}
}


/****************************************************************
** Function name:           Si1182_ECGLeadDetection
** Descriptions:            1182 导联检测
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ            
** Created Date:            2019-1-21
*****************************************************************/
uint8_t test_error = 0;

uint8_t Si1182_ECGLeadDetection(void) 
{
    int32_t   retval = 0;
    uint16_t  value = 0;

    retval = Si117x_ReadConfig(PARAM_ECG_LDCONFIG);
    test_error = retval;
    if (retval >= 0) 
    {
        value = (uint8_t) retval;
        if (value & 0x80) // lead on detection
        {
            // Send a lead on debug message
            // Lead Off
            // Flip direction
            Si117xParamSet(0, PARAM_ECG_LDCONFIG, (value & 0x7F));
            return 0;
        } 
        else // lead off detection
        {
            // Send a lead off debug message
            // Lead On
            // Flip direction
            Si117xParamSet(0, PARAM_ECG_LDCONFIG, (value | 0x80));
            return 1;
        }
    }
    
    return 1;
}

