/*******************Copyright(c)*********************************
** Veepoo Tech
** COPYRIGHT (C) 2016-2019
**----------------------FileInfo---------------------------------
** File name:                  si117x_handle.c
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

#include "si1182_handle.h"
#include "si1182_config.h"
#include "si1182_driver.h"
#include "si1182_interface.h"
#include "si1182_functions.h"
#include "si1182_calibration.h"
#include "SystemTime.h"
#include "common_data_type.h"

#include "data_manage.h"
#include "string.h"
#include "pwave_infoalgorithm.h"
#include "Blood_OxygenDetection.h"

/***************************************************************************/

static uint8_t 		s_si117x_active = 0;
static uint8_t 		dle_save = 0;

uint8_t     adc_ecg_signal_state = 0;
uint8_t     adc_ecg_signal_state1 = 0;
uint8_t     adc_ecg_signal_state2 = 0;

/***************************************************************************/

uint32_t int_time_cnt = 0;
uint32_t int_happen_time = 0;


extern si1182_register_parameter_t     si1182_register_parameter_s;      //1182配置参数,ppg算法使用
extern uint8_t init_mode_1182_task;     //ppg开启的测量模式
extern uint8_t blood_oxygen_calib_cnt;
extern uint8_t bod_calib_i_ird;
extern uint8_t bod_calib_adc_range_ird;
extern uint8_t bod_calib_clk_ird;

/**************************************************************************
 * @brief FIFO interrupt
 *************************************************************************/
void Si1182_IntHandler(void)
{
    uint8_t         ppg_task_num = 0;
    uint8_t         irq_status = 0;
	uint32_t        fifo_num = 0;
    int32_t         fifo_address = 0;
        
    // WJ.20190429.1182异常处理
    p_data_manage->ecg_ppg_s.stop_int_cnt = 0;      // 1182 开启下中断不响应计次(s)，用着异常处理
    
    //WJ.20181229.模式切换中，不需要读取数据，并且防止spi访问冲突
    if(p_data_manage->ecg_ppg_s.read_data != 1)
    {
        return;
    }
    
	/* Check for Si117x FIFO IRQ (cannot assume, could be other type) */
    irq_status = Si117xGetIrqStatus();
    
    // 变换位置，-O3编译导致问题，怀疑：1182响应不过来；或者数据处理时间过长
    if(irq_status & LD_IRQ)
    {   //导联检测
        g_si1182_data.ecg_lead_state = Si1182_ECGLeadDetection();
    }
    
    
	if(irq_status & FIFO_IRQ)
	{
        if(p_data_manage->ecg_ppg_s.oxygen_measure_s.ox_rest == 1 
           && (init_mode_1182_task == MODE_GREEN_RED_IRED_AMB || init_mode_1182_task == MODE_RED_IRED_AMB))
        {
            // 参数全部初始化
            DataManage_SI1182PoolInit();	
            p_data_manage->ecg_ppg_s.oxygen_measure_s.ox_init = 1;
            p_data_manage->ecg_ppg_s.oxygen_measure_s.ox_rest = 0;  // 重新
            
            Si1182_InitByMode(init_mode_1182_task);

            return;
        }
        
        // WJ.20190422.屏蔽为了解决夜间自动测量重新校准，清除数据的问题
        /*
        if(p_data_manage->ecg_ppg_s.oxygen_measure_s.ox_calib == 1)
        {
            p_data_manage->ecg_ppg_s.oxygen_measure_s.ox_calib = 0;
            DataManage_SI1182PoolInit();
        }
        */
        
		fifo_address = Si117xUpdateFifoCount();
        
        // adc 处理标记
        adc_ecg_signal_state = 0;	
        adc_ecg_signal_state1 = 0;		
        
		if(fifo_address < 0)
		{
			g_si1182_data.fifo_err = -1;
			
			Si1182_FifoDataOk();
			
            // adc 处理标记
            adc_ecg_signal_state = 1;
            
			return;
		}  
        
        adc_ecg_signal_state1++;

        if(fifo_address > g_si1182_data.fifo_address)
        {
            fifo_num = fifo_address - g_si1182_data.fifo_address;
        }  
        else
        {
            fifo_num = fifo_address + 0x0800 - g_si1182_data.fifo_address;
        }
        
        g_si1182_data.fifo_address = fifo_address;
		
		g_si1182_data.err_code = Si1182_FifoReadAndConvert((const uint8_t *)g_si1182_data.fifo_data, fifo_num);
	
		Si1182_FifoDataOk();
        
        // 获取红外的刻度和量程
        {   // WJ.20190305.
            uint8_t read_param_data1 = 0;
            uint8_t read_param_data2 = 0;

            if(init_mode_1182_task == MODE_GREEN_IRED_AMB)
            {                                                
                read_param_data1 = Si117x_ParamRead(PARAM_PPG2_LED4_CONFIG);
                read_param_data2 = Si117x_ParamRead(PARAM_PPG2_ADCCONFIG);
                
                ppg_task_num = 3;
            }
            else if(init_mode_1182_task == MODE_GREEN_RED_IRED_AMB || init_mode_1182_task == MODE_RED_IRED_AMB)
        	{
				//FWT.
				if(init_mode_1182_task == MODE_GREEN_RED_IRED_AMB)
				{
					ppg_task_num = 4;
				}
				else
				{
					ppg_task_num = 3;
				}
				if(blood_oxygen_calib_cnt == 0)
				{
					// 血氧初始化默認參數
					read_param_data1 = 11;
					read_param_data2 = ((0x03 & 0x03) << 4) | ((0x00 & 0x0f) << 0);
				}
				else
				{
					// 非夜间模式
					if(p_data_manage->ecg_ppg_s.oxygen_measure_s.ox_time == 0)
					{
						read_param_data1 = bod_calib_i_ird;
						read_param_data2 = ((bod_calib_adc_range_ird & 0x03) << 4) 
						                   | ((bod_calib_clk_ird & 0x0f) << 0);
					}
					else
					{
                        // 血氧初始化默認參數
						read_param_data1 = 11;
						read_param_data2 = ((0x03 & 0x03) << 4) | ((0x00 & 0x0f) << 0);
					}
				}
                //read_param_data1 = Si117x_ParamRead( PARAM_PPG3_LED4_CONFIG);
                //read_param_data2 = Si117x_ParamRead(PARAM_PPG3_ADCCONFIG);
                
                //ppg_task_num = 4;
            }

            si1182_register_parameter_s.i_ird = read_param_data1 & 0x3f;
            si1182_register_parameter_s.clk_div_ird = read_param_data2 & 0x0f;
            si1182_register_parameter_s.adc_range_ird = (read_param_data2 >> 4) & 0x03;
            // ired ONLY    //WJ.20190518.使用红外做佩戴
            si1182_register_parameter_s.light_type = SI1182_WEAR_DETECTION_MODE_IRED;
            //XJ 20190712, SI1182_LAYOUT_TYPE_1为1，只适用与E-28
            si1182_register_parameter_s.layout_type = SI1182_LAYOUT_TYPE_0;
        }
        
        // 动态调节参数
		///Si1182_ParamAdjust();
		// 读取设置的参数
		///Si1182_ReadParameterConfig();       
		// 血氧校准     
        if(p_data_manage->ecg_ppg_s.oxygen_measure_s.ox_app == 1 
           || p_data_manage->ecg_ppg_s.oxygen_measure_s.ox_btn == 1 
           || p_data_manage->ecg_ppg_s.oxygen_measure_s.ox_time == 1)
        {
            Si1182_CalibOx(ppg_task_num);
            //Si1182_CalibAdust();
        }
        
        // WJ.20190429.1182异常处理
        p_data_manage->ecg_ppg_s.stop_read_cnt = 0;     // 1182 开启下中断不读取数据时间(s)，用着异常处理
	}
	
}

/**************************************************************************
 * @brief Start Data logging
 *************************************************************************/
uint32_t StartDataLogging(void)
{
//	if (s_si117x_active==0)
	{
		Si117xFlushFIFO();
		Si117xStart();
		s_si117x_active = 1;
		
		dle_save = 0;
        
        g_si1182_data.fifo_address = 0;
        
        return 1;
	}
    
//    return 0;
}


/**************************************************************************
 * @brief Stop Data logging
 *************************************************************************/
uint32_t StopDataLogging(void)
{
//	if (s_si117x_active)
	{
		while (Si117xStop(0x00) !=0)
		{
			;
		}
		Si117x_WriteRegister(REG_HOSTIN0, 0xff);
		Si117x_WriteRegister(REG_COMMAND, 0x0d);
		s_si117x_active = 0;
        
        return 1;
	}
    
//    return 0;
}

/****************************************************************
** Function name:             Si1182_FifoReadAndConvert
** Descriptions:              读取fifo数据，转换fifo数据
** input parameters:          
** output parameters:         
** Returned value:            
** Created by:                JSH
** Created Date:              2018-12-19
****************************************************************/
int32_t Si1182_FifoReadAndConvert(const uint8_t *data, uint32_t len)
{
	int32_t  ret_value = 0; 
	uint32_t fifo_read_over = 0;
	uint32_t tmp_len = 0;
	
	tmp_len = len;
	
	while(tmp_len >= SI1182_FIFO_BUFFER_SIZE)
	{
        adc_ecg_signal_state1++;	
		if(tmp_len == SI1182_FIFO_BUFFER_SIZE)
		{
			fifo_read_over = 1;
		}
		else
		{
			fifo_read_over = 0;
		}
		
		g_si1182_data.fifo_read_len = SI1182_FIFO_BUFFER_SIZE;
		Si1182FifoRead(g_si1182_data.fifo_data, SI1182_FIFO_BUFFER_SIZE);
		
		if(tmp_len == len)
		{
			ret_value = DataManage_UpdateAdcData(g_si1182_data.adc_mode, 1, fifo_read_over);
		}
		else
		{
			ret_value = DataManage_UpdateAdcData(g_si1182_data.adc_mode, 0, fifo_read_over);
		}
		
		tmp_len -= SI1182_FIFO_BUFFER_SIZE;
	}
	
	if(tmp_len)
	{
        adc_ecg_signal_state1++;
        
		fifo_read_over = 1;
		
		g_si1182_data.fifo_read_len = tmp_len;
		Si1182FifoRead(g_si1182_data.fifo_data, tmp_len);
		
		if(tmp_len == len)
		{
			ret_value = DataManage_UpdateAdcData(g_si1182_data.adc_mode, 1, fifo_read_over);
		}
		else
		{
			ret_value = DataManage_UpdateAdcData(g_si1182_data.adc_mode, 0, fifo_read_over);
		}
	}
	
	return ret_value;
}

/****************************************************************
** Function name:             Si1182_HandleFifoDataPpg
** Descriptions:              Si1182处理原始FIFO的PPG数据，
                              处理后，就是真正的ADC数据
** input parameters:          des：    处理后的FIFO数据指针
                              src：    原始FIFO数据指针
                              len：    原始FIFO数据长度
** output parameters:         
** Returned value:            
** Created by:                JSH
** Created Date:              2017-12-22
****************************************************************/
int32_t Si1182_HandleFifoDataPpg(uint8_t *des, uint8_t *src, uint32_t len)
{
	static uint8_t s_next_dle = 0;          //下一src数组包头出现的位置
	
	uint32_t i = 0;
	uint32_t i_for = 0;
	uint32_t j = 0;
	
	
	if(len == 0)
	{
		return 0;
	}
	

	if(len <= 3)
	{
		if(s_next_dle == 3)
		{
			if(src[0] != 0x10)
			{
				return -2;
			}
		}
		
		// 一个样本数中断的adc数据的末尾几个字节//需考虑超读fifo的情况//处理完返回
		if(len <= s_next_dle)
		{
			// len小于下一个数据块的头
			for(i = 0; i < len; i++)
			{
				des[j++] = src[i];
			}
			
			s_next_dle -= len;
				
			return len;
		}
		else
		{
			for(i = 0; i < s_next_dle; i++)
			{
				des[j++] = src[i];
			}
			
			// 在只有一种数据模式的时候（ppg或ecg），出现一个0x10，必跟随一个0x10
			if(src[i] == 0x10)
			{
				if(i + 1 == len)
				{
					s_next_dle = 3;
					
					return j;
				}
				else
				{
					i += 1;
					
					if(src[i] == 0x10)
					{
						for(; i < len; i++)
						{
							des[j++] = src[i];
						}
						
						s_next_dle = 3 - (len - s_next_dle) + 1;
						
						return j;
					}
					else
					{
						return -2;
					}
				}
			}
			else
			{			
				for(;i < len; i++)
				{
					des[j++] = src[i];
				}
				
				s_next_dle = 3 - (len - s_next_dle);
				
				return j;
			}
		}
	}
	else
	{
		// 保证数组不越界
		len -= 3;
	}
	
	//开启测量时dle_save=0，会出一个dle包头
	if(dle_save == 0)
	{
		if((src[0] == 0x10)&&(src[1] == 0x50))
		{
			dle_save = 1;
			s_next_dle = 0;
			i_for = 2;
		}
		else
		{
			// 包头出错
			return -1;
		}
	}
	else
	{
		if(s_next_dle == 3)
		{
			if(src[0] != 0x10)
			{
				return -2;
			}
		}
		
		for(i = 0; i < s_next_dle; ++i)
		{
			des[j++] = src[i];
		}
		
		i_for = s_next_dle;
	}
	
	//循环处理
	for(i = i_for; i < len; ++i)
	{
		if(src[i] == 0x10)
		{
			if(src[i + 1] == 0x10)          
			{
				i += 1;
			}
			else
			{
				return -2;
			}
		}
				
		des[j++] = src[i++];
		des[j++] = src[i++];
		des[j++] = src[i];
	}
	
	//处理最后3个byte的情况
	if(i == len)
	{
		if(src[i] == 0x10)
		{
			if(src[i + 1] == 0x10)          
			{
				i += 1;
				
				des[j++] = src[i++];
		        des[j++] = src[i];
				
				s_next_dle = 1;
			}
			else
			{
				return -2;
			}
		}
		else
		{
			des[j++] = src[i++];
		    des[j++] = src[i++];
		    des[j++] = src[i];
			
			s_next_dle = 0;
		}
	}
	else if(i == (len + 1))
	{
		if(src[i] == 0x10)
		{
			if(src[i + 1] == 0x10)          
			{
				i += 1;
				
				des[j++] = src[i++];

				s_next_dle = 2;
			}
			else
			{
				return -2;
			}
		}
		else
		{
			des[j++] = src[i++];
		    des[j++] = src[i];
			
			s_next_dle = 1;
		}
	}
	else if(i == (len + 2))
	{
		if(src[i] == 0x10)
		{
			s_next_dle = 3;
		}
		else
		{
			des[j++] = src[i];
			
			s_next_dle = 2;
		}
	}
	
	return j;
}

/****************************************************************
** Function name:             Si117x_HandleFifoDataEcg
** Descriptions:              Si117x处理原始FIFO的PPG数据，
                              处理后，就是真正的ADC数据
** input parameters:          des：    处理后的FIFO数据指针
                              src：    原始FIFO数据指针
                              len：    原始FIFO数据长度
** output parameters:         
** Returned value:            
** Created by:                JSH
** Created Date:              2017-12-22
****************************************************************/
int32_t Si1182_HandleFifoDataEcg(uint8_t *des, uint8_t *src,uint32_t len)
{
	static uint8_t s_next_dle = 0;          //下一src数组包头出现的位置
	
	uint32_t i = 0;
	uint32_t i_for = 0;
	uint32_t j = 0;
	
	
	if(len == 0)
	{
		return 0;
	}
	

	if(len <= 3)
	{
		if(s_next_dle == 3)
		{
			if(src[0] != 0x10)
			{
				return -2;
			}
		}
		
		// 一个样本数中断的adc数据的末尾几个字节//需考虑超读fifo的情况//处理完返回
		if(len <= s_next_dle)
		{
			// len小于下一个数据块的头
			for(i = 0; i < len; i++)
			{
				des[j++] = src[i];
			}
			
			s_next_dle -= len;
				
			return len;
		}
		else
		{
			for(i = 0; i < s_next_dle; i++)
			{
				des[j++] = src[i];
			}
			
			// 在只有一种数据模式的时候（ppg或ecg），出现一个0x10，必跟随一个0x10
			if(src[i] == 0x10)
			{
				if(i + 1 == len)
				{
					s_next_dle = 3;
					
					return j;
				}
				else
				{
					i += 1;
					
					if(src[i] == 0x10)
					{
						for(; i < len; i++)
						{
							des[j++] = src[i];
						}
						
						s_next_dle = 3 - (len - s_next_dle) + 1;
						
						return j;
					}
					else
					{
						return -2;
					}
				}
			}
			else
			{			
				for(;i < len; i++)
				{
					des[j++] = src[i];
				}
				
				s_next_dle = 3 - (len - s_next_dle);
				
				return j;
			}
		}
	}
	else
	{
		// 保证数组不越界
		len -= 3;
	}
	
	//开启测量时dle_save=0，会出一个dle包头
	if(dle_save == 0)
	{
		if((src[0] == 0x10)&&(src[1] == 0x45))
		{
			dle_save = 1;
			s_next_dle = 0;
			i_for = 2;
		}
		else
		{
			// 包头出错
			return -1;
		}
	}
	else
	{
		if(s_next_dle == 3)
		{
			if(src[0] != 0x10)
			{
				return -2;
			}
		}
		
		for(i = 0; i < s_next_dle; ++i)
		{
			des[j++] = src[i];
		}
		
		i_for = s_next_dle;
	}
	
	//循环处理
	for(i = i_for; i < len; ++i)
	{
		if(src[i] == 0x10)
		{
			if(src[i + 1] == 0x10)          
			{
				i += 1;
			}
			else
			{
				return -2;
			}
		}
				
		des[j++] = src[i++];
		des[j++] = src[i++];
		des[j++] = src[i];
	}
	
	//处理最后3个byte的情况
	if(i == len)
	{
		if(src[i] == 0x10)
		{
			if(src[i + 1] == 0x10)          
			{
				i += 1;
				
				des[j++] = src[i++];
		        des[j++] = src[i];
				
				s_next_dle = 1;
			}
			else
			{
				return -2;
			}
		}
		else
		{
			des[j++] = src[i++];
		    des[j++] = src[i++];
		    des[j++] = src[i];
			
			s_next_dle = 0;
		}
	}
	else if(i == (len + 1))
	{
		if(src[i] == 0x10)
		{
			if(src[i + 1] == 0x10)          
			{
				i += 1;
				
				des[j++] = src[i++];

				s_next_dle = 2;
			}
			else
			{
				return -2;
			}
		}
		else
		{
			des[j++] = src[i++];
		    des[j++] = src[i];
			
			s_next_dle = 1;
		}
	}
	else if(i == (len + 2))
	{
		if(src[i] == 0x10)
		{
			s_next_dle = 3;
		}
		else
		{
			des[j++] = src[i];
			
			s_next_dle = 2;
		}
	}
	
	return j;
}

/****************************************************************
** Function name:             Si1182_HandleFifoDataEcgAndLd
** Descriptions:              Si117x处理原始FIFO的PPG数据和导联数据，
                              处理后，就是真正的ADC数据
** input parameters:          des：    处理后的FIFO数据指针
                              src：    原始FIFO数据指针
                              len：    原始FIFO数据长度
** output parameters:         
** Returned value:            
** Created by:                JSH
** Created Date:              2017-12-22
****************************************************************/
extern uint8_t adc_ecg_signal_state2;
int32_t Si1182_HandleFifoDataEcgAndLd(uint8_t *des, const uint8_t *src, uint32_t len, uint8_t *ld_des, uint32_t *ld_des_len)
{
	static uint8_t s_last_num = 0;
	static uint8_t s_ld_data_flag = 0;
	
	static uint8_t	data_buffer[SI1182_FIFO_BUFFER_SIZE + SI1182_FIFO_LAST_DATA_SIZE + SI1182_FIFO_DATA_TEST_SIZE];
	uint32_t data_len = 0;
	uint32_t des_num = 0;
	uint32_t ld_num = 0;
	uint32_t i = 0;
	
	uint32_t last_des_num = 0;
	uint32_t last_ld_num = 0;
	uint32_t last_i = 0;
    
    static uint8_t head_is_ok = 0;
	
    
    adc_ecg_signal_state2 = head_is_ok;
    
	// 输入长度为0
	if(len == 0)
	{
        // adc error
        adc_ecg_signal_state = 3;
        
		*ld_des_len = 0;
		return 0;
	}
	
	// 开始测量，有个包头
	if(dle_save == 0)
	{
		dle_save = 1;
		
		s_last_num = 0;
		s_ld_data_flag = 0;
		memset(data_buffer, 0, sizeof(data_buffer));
        
        head_is_ok = 0;
	}
	
	// 拷贝需要解析的数据到缓存
	if(len > SI1182_FIFO_BUFFER_SIZE)
	{
        // adc error
        adc_ecg_signal_state = 4;
		
        return -1;
	}
	memcpy(&data_buffer[s_last_num], src, len);
	data_len = s_last_num + len;
    
    memset(&data_buffer[data_len], 0, SI1182_FIFO_BUFFER_SIZE + SI1182_FIFO_LAST_DATA_SIZE + SI1182_FIFO_DATA_TEST_SIZE - data_len);
	
    // adc 
    adc_ecg_signal_state1++;
    
	// 循环处理
	for(i = 0; i < data_len; )
	{
		last_i = i;
		last_des_num = des_num;
		last_ld_num = ld_num;
		
        if(data_buffer[i] == 0x10)
        {
            if(data_buffer[i + 1] == 0x10 && head_is_ok == 1)          
            {
                i += 1;
                
                if(s_ld_data_flag == 0)
                {
                    des[des_num++] = data_buffer[i++];
                    des[des_num++] = data_buffer[i++];
                    des[des_num++] = data_buffer[i++];
                }
                else
                {
                    ld_des[ld_num++] = data_buffer[i++];
                    ld_des[ld_num++] = data_buffer[i++];
                    ld_des[ld_num++] = data_buffer[i++];
                    ld_des[ld_num++] = data_buffer[i++];
                }
                
            }
            else if(data_buffer[i + 1] == 0x45)
            {
                i += 2;
                
                if(data_buffer[i] == 0x10)
                {
                    i += 1;
                    
                    if(data_buffer[i] != 0x10)
                    {
                        i--;
                        head_is_ok = 0;
                        continue;
                    }
                }

                des[des_num++] = data_buffer[i++];
                des[des_num++] = data_buffer[i++];
                des[des_num++] = data_buffer[i++];
                
                s_ld_data_flag = 0;
                
                head_is_ok = 1;
            }
            else if(data_buffer[i + 1] == 0x4C)
            {
                i += 2;
                
                if(data_buffer[i] == 0x10)
                {
                    i += 1;
                    
                    if(data_buffer[i] != 0x10)
                    {
                        i--;
                        head_is_ok = 0;
                        continue;
                    }
                }

                ld_des[ld_num++] = data_buffer[i++];
                ld_des[ld_num++] = data_buffer[i++];
                ld_des[ld_num++] = data_buffer[i++];
                ld_des[ld_num++] = data_buffer[i++];
                
                s_ld_data_flag = 1;
                
                head_is_ok = 1;
            }
            else if(data_buffer[i + 1] == 0x46)
            {   //出现这需要减少数据处理频率
                i += 2;
                
                //head_is_ok = 0;
            }
            else
            {
                if(i + 1 >= data_len)
                {
                    break;
                }
                else
                {
                    head_is_ok = 0;
                    i++;                        
                    //return -2;
                }
            }
        }
        else if(head_is_ok == 1)
        {		
            if(s_ld_data_flag == 0)
            {
                des[des_num++] = data_buffer[i++];
                des[des_num++] = data_buffer[i++];
                des[des_num++] = data_buffer[i++];
            }
            else
            {
                ld_des[ld_num++] = data_buffer[i++];
                ld_des[ld_num++] = data_buffer[i++];
                ld_des[ld_num++] = data_buffer[i++];
                ld_des[ld_num++] = data_buffer[i++];
            }
        }
        else
        {
            i++;
        }
    }
		
    if(i > data_len)
    {
        // 保存最后未解析的数据
        s_last_num = data_len - last_i;
        if(s_last_num > 0)
        {
            memcpy(data_buffer, &data_buffer[last_i], s_last_num);
        }   
    }
    else if(data_len == i)
    {
        last_i = i;
        last_des_num = des_num;
        last_ld_num = ld_num;
        // 保存最后未解析的数据
        s_last_num = 0;
    }
    else
    {
        // 保存最后未解析的数据
        s_last_num = data_len - last_i;
        if(s_last_num > 0)
        {
            memcpy(data_buffer, &data_buffer[last_i], s_last_num);
        }   
    }
	
	*ld_des_len = last_ld_num;
	return last_des_num;
}

