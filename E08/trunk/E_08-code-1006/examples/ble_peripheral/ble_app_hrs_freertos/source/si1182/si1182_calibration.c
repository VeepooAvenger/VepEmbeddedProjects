/*******************Copyright(c)*********************************
** Veepoo Tech
** COPYRIGHT (C) 2016-2019
**----------------------FileInfo---------------------------------
** File name:                  si1182_calibration.c
** Last modified Date:         2019-1-22
** Last SYS_VERSION:           0.0.1
** Descriptions:
**---------------------------------------------------------------
** Createdby:                  JSH
** Createddate:                2019-1-22
** SYS_VERSION:                0.0.1
** Descriptions:               si1182У׼
**---------------------------------------------------------------
** Modified by:				
** Modified date:			
** SYS_VERSION:				
** Modified Descriptions:	
****************************************************************/

#include "si1182_calibration.h"
#include "Blood_OxygenDetection.h"
#include "spo2_1182_calib.h"
#include "si1182_functions.h"
#include "si1182_driver.h"
#include "data_manage.h"


si1182_calib_t si1182_calib;

static uint8_t s_meas_rate_h = 0;
static uint8_t s_meas_rate_l = 0;

// Ѫ��У׼����
uint8_t blood_oxygen_calib_cnt = 0;

uint8_t bod_calib_i_ird = 0;
uint8_t bod_calib_adc_range_ird = 0;
uint8_t bod_calib_clk_ird = 0;


/****************************************************************
** Function name:             Si1182_CalibBloodOxygenGetPara
**  Descriptions:              Si1182��Ѫ��У׼���ݵĻ�ȡ
** input parameters:                                      
** output parameters:         
** Returned value:            
** Created by:                JSH
** Created Date:              2019-03-02
****************************************************************/
void Si1182_CalibBloodOxygenGetPara(detection_char_info_t* p_param)
{
	p_param->i_red = si1182_calib.i_red;
	p_param->i_ird = si1182_calib.i_ird;
	p_param->adc_range_red = si1182_calib.adc_range_red;
	p_param->adc_range_ird = si1182_calib.adc_range_ird;
	p_param->clk_div_red = si1182_calib.clk_div_red;
	p_param->clk_div_ird = si1182_calib.clk_div_ird;
	p_param->decim_red = si1182_calib.decim_red;
	p_param->decim_ird = si1182_calib.decim_ird;
	p_param->adc_avg_red = si1182_calib.adc_avg_red;
	p_param->adc_avg_ird = si1182_calib.adc_avg_ird;
	p_param->ppg_sw_avg_all = si1182_calib.ppg_sw_avg_all;
	p_param->calib_meas_rate = (si1182_calib.ppg_meas_rate_h << 8) + si1182_calib.ppg_meas_rate_l;
	
	p_param->task_mode = 1;
}

/****************************************************************
** Function name:             Si1182_CalibBloodOxygenGetPara
** Descriptions:              Si1182��Ѫ��У׼���ݵ�����
** input parameters:                                      
** output parameters:         
** Returned value:            
** Created by:                JSH
** Created Date:              2019-03-02
****************************************************************/
void Si1182_CalibBloodOxygenSetPara(const bod_result_info_t* cp_param)
{
	si1182_calib.i_red = cp_param->i_red;
	si1182_calib.i_ird = cp_param->i_ird;
	si1182_calib.adc_range_red = cp_param->adc_range_red;
	si1182_calib.adc_range_ird = cp_param->adc_range_ird;
	si1182_calib.clk_div_red = cp_param->clk_div_red;
	si1182_calib.clk_div_ird = cp_param->clk_div_ird;
	si1182_calib.decim_red = cp_param->decim_red;
	si1182_calib.decim_ird = cp_param->decim_ird;
	si1182_calib.adc_avg_red = cp_param->adc_avg_red;
	si1182_calib.adc_avg_ird = cp_param->adc_avg_ird;
	si1182_calib.ppg_sw_avg_all = cp_param->ppg_sw_avg_all;
	si1182_calib.ppg_meas_rate_h = (cp_param->calib_meas_rate >> 8) & 0xff;
	si1182_calib.ppg_meas_rate_l = cp_param->calib_meas_rate & 0xff;
	
	if(cp_param->calib_1182_over_flag == 0)
	{
		si1182_calib.blood_oxygen_change_flag = 1;
	}
}

/****************************************************************
** Function name:             Si1182_CalibInit
** Descriptions:              Si1182��У׼���ݵĳ�ʼ��
** input parameters:                                      
** output parameters:         
** Returned value:            
** Created by:                JSH
** Created Date:              2019-03-02
****************************************************************/
void Si1182_CalibInit(void)
{
	si1182_calib.blood_oxygen_change_flag = 0;
	
	si1182_calib.i_red = 0x0b;
	si1182_calib.i_ird = 0x0b;
	si1182_calib.adc_range_red = 0x03;
	si1182_calib.adc_range_ird = 0x03;
	si1182_calib.clk_div_red = 0x00;
	si1182_calib.clk_div_ird = 0x00;
	si1182_calib.decim_red = 0x01;
	si1182_calib.decim_ird = 0x01;
	si1182_calib.adc_avg_red = 0x02;
	si1182_calib.adc_avg_ird = 0x02;
	si1182_calib.ppg_sw_avg_all = 0;
	si1182_calib.ppg_meas_rate_h = 0x01;
	si1182_calib.ppg_meas_rate_l = 0x90;
	
	s_meas_rate_h = si1182_calib.ppg_meas_rate_h;
	s_meas_rate_l = si1182_calib.ppg_meas_rate_l;
}

/****************************************************************
** Function name:             Si1182_CalibAdust
** Descriptions:              Si1182��У׼д�Ĵ���
** input parameters:                                      
** output parameters:         
** Returned value:            
** Created by:                JSH
** Created Date:              2019-03-02
****************************************************************/
void Si1182_CalibAdust(void)
{
	int32_t retval = 0;
	
	
	if(si1182_calib.blood_oxygen_change_flag == 0)
	{
		return;
	}
	else
	{
		si1182_calib.blood_oxygen_change_flag = 0;
		
		if((s_meas_rate_h != si1182_calib.ppg_meas_rate_h)
			||(s_meas_rate_l != si1182_calib.ppg_meas_rate_l))
		{
			Si1182_ConfigModeWithRedIredAmb();
		}
		else
		{
			Si1182_Stop();
		}

		retval += Si1182_ParamSet(0, PARAM_MEASRATE_H, si1182_calib.ppg_meas_rate_h); 
		retval += Si1182_ParamSet(0, PARAM_MEASRATE_L, si1182_calib.ppg_meas_rate_l);
		retval += Si1182_ParamSet(0, PARAM_MEAS_CNTL, 0x01 | ((0x07 & si1182_calib.ppg_sw_avg_all) << 3)); 
		// ���
		retval += Si1182_ParamSet(0, PARAM_PPG1_LED1_CONFIG, 0x40 | ((0x3f & si1182_calib.i_red) << 0));
		retval += Si1182_ParamSet(0, PARAM_PPG1_LED3_CONFIG, 0x40 | ((0x3f & si1182_calib.i_red) << 0)); 
		retval += Si1182_ParamSet(0, PARAM_PPG1_MEASCONFIG, 0x00 | ((0x07 & si1182_calib.adc_avg_red) << 0)); 
		retval += Si1182_ParamSet(0, PARAM_PPG1_ADCCONFIG, 0x00 | ((0x03 & si1182_calib.decim_red) << 6)
															   | ((0x03 & si1182_calib.adc_range_red) << 4)
															   | ((0x0f & si1182_calib.clk_div_red) << 0));
		// ����
		retval += Si1182_ParamSet(0, PARAM_PPG2_LED4_CONFIG, 0x40 | ((0x3f & si1182_calib.i_ird) << 0)); 
		retval += Si1182_ParamSet(0, PARAM_PPG2_MEASCONFIG, 0x00 | ((0x07 & si1182_calib.adc_avg_ird) << 0)); 
		retval += Si1182_ParamSet(0, PARAM_PPG2_ADCCONFIG, 0x00 | ((0x03 & si1182_calib.decim_ird) << 6)
															   | ((0x03 & si1182_calib.adc_range_ird) << 4)
															   | ((0x0f & si1182_calib.clk_div_ird) << 0));
		

		Si1182_Start();
		
		s_meas_rate_h = si1182_calib.ppg_meas_rate_h;
		s_meas_rate_l = si1182_calib.ppg_meas_rate_l;
		
		if(retval == 0)
		{
			return;
		}
	}
}

/****************************************************************
** Function name:             Si1182_BloodOxgenCalibInit
** Descriptions:              Si1182��Ѫ��У׼�ĳ�ʼ��
** input parameters:                                      
** output parameters:         
** Returned value:            
** Created by:                JSH
** Created Date:              2019-03-02
****************************************************************/
void Si1182_BloodOxgenCalibInit(void)
{
	si1182_calib.blood_oxygen_calib_start = 1;
	blood_oxygen_calib_cnt = 0;
}

/****************************************************************
** Function name:             Si1182_Calib
** Descriptions:              Si1182��У׼����-�ж��е���
** input parameters:                                      
** output parameters:         
** Returned value:            
** Created by:                JSH
** Created Date:              2019-03-02
****************************************************************/
void Si1182_CalibOx(uint8_t task_num)
{
	bod_result_info_t *p_bod_info = NULL;
	detection_char_info_t oxygen_detection = {0};
	uint32_t calib_flag = 0;

    if(si1182_calib.blood_oxygen_calib_start)
    {
        // У׼����
        blood_oxygen_calib_cnt++;
        Si1182_CalibBloodOxygenGetPara(&oxygen_detection);
        p_bod_info = Get_OxygenInfo();
                
        // WJ.20190305.����FWTҪ�����
        oxygen_detection.task_mode = task_num;
                
        // Ѫ��У׼����
        calib_flag = Spo2_1182CalibDetc(DataManage_GetRedDataPool(), DataManage_GetIredDataPool(), 
                                        &oxygen_detection, p_bod_info, blood_oxygen_calib_cnt , 1);
             // FWT.�ṩ�o���ʹ�õą���
        bod_calib_i_ird = p_bod_info->i_ird;
        bod_calib_adc_range_ird = p_bod_info->adc_range_ird;
        bod_calib_clk_ird = p_bod_info->clk_div_ird;
        
        // У׼���
        si1182_calib.blood_oxygen_calib_start = !calib_flag;
        Si1182_CalibBloodOxygenSetPara((const bod_result_info_t *)p_bod_info);
        // 20190409 FWT  Ѫ���й�������ҹ��У׼�鷳���Ȳ���ҹ��У׼д���������ǻ������㷨У׼����
        if(p_data_manage->ecg_ppg_s.oxygen_measure_s.ox_time == 0)
        {
            Si1182_CalibAdust();
        }
	}
}

