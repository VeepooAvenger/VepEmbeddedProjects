/***************************Copyright(c)***********************************
**                       Veepoo Tech
**                  COPYRIGHT (C) 2016-2019
**-----------File Info-----------------------------------------------------
** File        name:     afe4900_entry.c
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
#include <string.h>

#include "afe4900_driver.h"
#include "afe4900_entry.h"
#include "afe4900_init.h"
#include "nrf_drv_gpiote.h"
#include "communication_thread.h"
#include "data_manage.h"
#include "algorithm_thread.h"
#include "afe4900_calibration.h"
#include "ble_thread.h"
#include "Blood_OxygenDetection.h"
#include "Afe4410_HrCalib.h"
#include "algorithm_thread.h"

#include "board.h"

static int32_t sample_buffer[100] = {0};

// Ѫ��4410��ʼ����־λ
uint8_t         spo2_4410_init = 0;
// ����У׼��ʼ��
uint8_t         hr_init_flag = 0;
// ����У׼������
uint8_t         hr_calib_err = 0;

static volatile uint8_t afe4900_check_time = 0;     // �����ж�4410�ж��Ƿ�����
static uint8_t  afe4900_is_open = 0;                // ��¼4410�Ŀ���״̬
//uint8_t         hr_calib_err = 0;

void Afe4900_WearReadReg(void);

/****************************************************************
** Function name:             ADC_Send_Data
** Descriptions:              ADC��������
** input parameters:          ��
** output parameters:         ��
** Returned value:            ��
** Created by:                GT
** Created Date:              2016-09-12
****************************************************************/ 
static void ADC_Send_Data(void)
{
    if(p_data_manage->upload == UPLOAD_ADC_OXYGEN_EVENT)
    {
         ComThread_SendADCData();
    }
    
    if(p_data_manage->upload == UPLOAD_ADC_GREEN_EVENT)
    {
         ComThread_SendADCData();
    }
}

/*************************************************************************
** Function name:        Afe4900_Ppg1Isr
** Descriptions:         ����
** input parameter:      ��   
** Returned Value:       ��
** Created  by:          JSH
** Created  Datas:       2018-10-17
**************************************************************************/
void Afe4900_Ppg1Isr(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action)
{
    int32_t green_reg = 0;
    int32_t amb_reg = 0;
    int32_t *buffer_p = NULL;  
    
    Afe4900_FifoReadConvert((uint8_t*)sample_buffer, 10);	//5����������,ÿ��2·�ź�

    //���ʹ��
    Afe4900_WearReadReg();
    
    buffer_p = sample_buffer;	
	for(uint32_t i = 0; i < 5; i++)
	{
		green_reg = *(buffer_p + 0);
		amb_reg = *(buffer_p + 1);
        buffer_p += 2;
		
        // 1.���������ݱ��浽������
        DataManage_UpdateHeartADCData(green_reg, amb_reg);
          
        // 2.������Ҫÿ100ms���һ���Ƿ������ݷ���
        if((p_data_manage->data_pool.cache_update_cnt%5) == 0)
        {
            ADC_Send_Data();	 
        }        
        // 3.�ж�1S������Ƿ���
        if(p_data_manage->data_pool.cache_update_cnt >= 50)
        {
			// �������������У׼����Ҫע�������ʹ��Ѫ���ն�ʱ  ������У׼����
			if(Ad_StatuFlag(&(p_data_manage->data_pool.green[450+250]),50))
            {
                hr_init_flag = 1;
                // ����У׼ �������³�ʼ�� ��������
                p_data_manage->heart_measure_s.calib_init = 1;
                p_data_manage->heart_measure_s.calib_well = 0;
                p_data_manage->heart_measure_s.init = 1;
            }
            // ����������
            DataManage_CopyHeartCacheData();
			// У׼���
            if(p_data_manage->heart_measure_s.calib_well == 0)
            {
                p_data_manage->data_pool.pool_update_second = 0;
            }
            else
            {
                // ����AFE4404�����㷨����
                AlgThread_AFE4404Task();
            }
        }
    }

    // ����У׼��ʼ�� 
    if(hr_init_flag == 1)
    {
        //Algorithm_HeartCalibTask();
        Hr_CalibInit();
        hr_calib_err = 0;
        Elec_ModeWrite(1,(uint8_t)(INIT_ILED_SCALES_HR_CALIB));

        Tia_GainModeWrite(1,(uint8_t)(1));
        Tia_GainModeWrite(4,(uint8_t)(1));

#ifdef AMB_DAC_MODE
        // �˴�AMB_DAC_MODEģʽ��ʾ �������Ӱģʽ
        Dac_ModeWrite(1,AMB_DAC_MODE, 0, 1);
        Dac_ModeWrite(4,AMB_DAC_MODE, 0, 1);
#else
        Dac_ModeWrite(1,0, 0, 1);
        Dac_ModeWrite(4,0, 0, 1);
#endif

    }

    // ����У׼����  
    if(hr_init_flag == 0 && hr_calib_err == 0)
    {
        hr_calib_err = Hr_Calib(green_reg,amb_reg);
        // д����  �������̹�
        Elec_ModeWrite(1,(uint8_t)(current_set));
        // д���� �����̹⼰������
        Tia_GainModeWrite(1,(uint8_t)(rf_set));
        Tia_GainModeWrite(4,(uint8_t)(1));

#ifdef AMB_DAC_MODE
        Dac_ModeWrite(1,AMB_DAC_MODE, dac_set, 1);
        Dac_ModeWrite(4,AMB_DAC_MODE, 0, 1);
#else
        Dac_ModeWrite(1,0, dac_set, 1);
        Dac_ModeWrite(4,0, 0, 1);
#endif

    }
    else
    {
        // У׼���
        p_data_manage->heart_measure_s.calib_well = 1;
    }
    // һ��У׼��ʼ��һ�μ���
    hr_init_flag = 0;
}

/*************************************************************************
** Function name:        Afe4900_Ppg2Isr
** Descriptions:         Ѫ��
** input parameter:      ��   
** Returned Value:       ��
** Created  by:          JSH
** Created  Datas:       2018-10-17
**************************************************************************/
void Afe4900_Ppg2Isr(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action)
{
	int32_t green_reg = 0;
	int32_t red_reg = 0;
	int32_t ired_reg = 0;
	int32_t amb_reg = 0;
	int32_t *buffer_p = NULL;
   
    buffer_p = sample_buffer;
    /*// WJ.20190617.�Ƶ�Algorithm_BloodOxygenTask()������
	if(spo2_4410_init == 1)
	{
		spo2_4410_init = 0;
		p_data_manage->blood_oxygen_measure_s.cnt = 0;
        memset(&p_data_manage->blood_oxygen_measure_s.time_result, 0, sizeof(p_data_manage->blood_oxygen_measure_s.time_result));
	}
    */
	Afe4900_FifoReadConvert((uint8_t*)sample_buffer, 20);	//5����������,ÿ��4·�ź�   
    
    if(p_data_manage->blood_oxygen_measure_s.cal_flag == 1)
    {
        p_data_manage->blood_oxygen_measure_s.cal_flag = 0;
#ifdef CTR_DIFF_CALIB
        // ��������˿���CTRУ׼ģʽ������ҪCTRУ׼���֮�����㷨У׼д����
        if(calc_ctr_line_flag == 1)
        {
            // �㷨У׼д����
            Algorithm_Spo2CalibTask(0,0,0);
        }
#else
        // �㷨У׼д����
        Algorithm_Spo2CalibTask(0,0,0);
#endif // CTR_DIFF_CALIB
    }

    //���ʹ��
    Afe4900_WearReadReg();

    for(uint32_t i = 0; i < 5; i++)
    {
        green_reg = *(buffer_p + 2);
        red_reg = *(buffer_p + 0);
        ired_reg = *(buffer_p + 1);
        amb_reg = *(buffer_p + 3);
        buffer_p += 4;
		
		//DataManage_UpdatePpgData(&green_reg, &red_reg, &red_reg, &amb_reg, 1);
        // 1.���������ݱ��浽������
        DataManage_UpdateBloodOxygenADCData(red_reg, ired_reg, green_reg, amb_reg);
            
        // 2.������Ҫÿ100ms���һ���Ƿ������ݷ���
        if((p_data_manage->data_pool.cache_update_cnt%5) == 0)
        {
            ADC_Send_Data();	 
        }
 
        // 3.�ж�1S������Ƿ���
        if(p_data_manage->data_pool.cache_update_cnt >= 50)
        {
            //WJJJ
            //one_sec_cnt++;
            // 4.����������
            DataManage_CopyBloodOxygenCacheData();
	
            // 5.����AFE4404�����㷨����
            AlgThread_AFE4404Task();
        }
    }

#ifdef CTR_DIFF_CALIB
    // CTRУ׼ģʽУ׼����
    Calib_CtrDiff(red_reg,ired_reg,amb_reg);
#endif


    // ����У׼����У׼�����̹���е��ڣ�
    if(hr_init_flag == 1)
    {
        Hr_CalibInit();
        hr_calib_err = 0;
        Elec_ModeWrite(1,(uint8_t)(INIT_ILED_SCALES_HR_CALIB));

        Tia_GainModeWrite(1,(uint8_t)(1));
#ifdef AMB_DAC_MODE
        Dac_ModeWrite(1,AMB_DAC_MODE, 0, 1);
#else
        Dac_ModeWrite(1,0, 0, 1);
#endif
    }
    if(hr_init_flag == 0 && hr_calib_err == 0)
    {
        hr_calib_err = Hr_Calib(green_reg,amb_reg);

        Elec_ModeWrite(1,(uint8_t)(current_set));

        Tia_GainModeWrite(1,(uint8_t)(rf_set));

#ifdef AMB_DAC_MODE
        Dac_ModeWrite(1,AMB_DAC_MODE, dac_set, 1);
#else
        Dac_ModeWrite(1,0, dac_set, 1);
#endif
    }
    hr_init_flag = 0;

				
	AFE4410_ElecRead(
				&oxygen_detection.current_register_green,
				&oxygen_detection.current_register_red,
				&oxygen_detection.current_register_ired);
	
	AFE4410_RfRead(
				&oxygen_detection.rf_register_green,
				&oxygen_detection.rf_register_red,
				&oxygen_detection.rf_register_ired);
    
	AFE4410_DacRead(
				&oxygen_detection.dac_register_green,
				&oxygen_detection.dac_register_red,
				&oxygen_detection.dac_register_ired,
				&oxygen_detection.dac_pos_green,
				&oxygen_detection.dac_pos_red,
				&oxygen_detection.dac_pos_ired);

}

/*************************************************************************
** Function name:        Afe4900_PpgRestIsr
** Descriptions:         �ϵ紦��
** input parameter:      ��   
** Returned Value:       ��
** Created  by:          WJ
** Created  Datas:       2019-02-15
**************************************************************************/
void Afe4900_PpgRestIsr(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action)
{
    static uint8_t red_reg_cnt = 0;
    int32_t red_reg_value = 0;

	int32_t ired_reg = 0;
	int32_t *buffer_p = NULL;

	Afe4900_FifoReadConvert((uint8_t*)sample_buffer, 20);	//5����������,ÿ��4·�ź�   
    
    buffer_p = sample_buffer;	
	for(uint32_t i = 0; i < 5; i++)
	{
		//green_reg = *(buffer_p + 2);
		//red_reg = *(buffer_p + 0);
		ired_reg = *(buffer_p + 1);
		//amb_reg = *(buffer_p + 3);
        buffer_p += 4;
        
        red_reg_value += ired_reg;
	}
    red_reg_cnt += 5;
    
    if(red_reg_cnt >= 10)
    {
        red_reg_value = red_reg_value / red_reg_cnt;
        // ֹͣ����
        Afe4900_Close();
        Afe4900_PowerOnInit();
    }
    oxygen_detection.adjust_dac_scale = red_reg_value;
}

/*************************************************************************
** Function name:        Afe4900_Close
** Descriptions:         afe4900 �رգ�����͹���
** input parameter:      ��   
** Returned Value:       ��
** Created  by:          WJ
** Created  Datas:       2018-10-22
**************************************************************************/
void Afe4900_Close(void)
{
    //�ر��ж���Ӧ
	Afe4900_AdcrdyInterruptDisable();
	
	// Afe4900�رղ���
	Afe4900_StopSampling();
    
    //����4900λ�͹���ģʽ
	AFE4900_HardwarePowerDown();
	
	// Afe4900�ر�LDO
	//Afe4900_LdoTxOff();
    
    //���õĹܽ����óɸ���̬
    
    //WJ.20190111.���û�����ÿ�����־������
    afe4900_is_open = 0;    
}

/*************************************************************************
** Function name:        Afe4900_OpenMode
** Descriptions:         afe4900 ����
** input parameter:      ��   
** Returned Value:       ��
** Created  by:          WJ
** Created  Datas:       2018-10-22
**************************************************************************/
void Afe4900_OpenMode(MEASURE_MOD_E mode)
{    
	afe4900_check_time = 0;
    if(mode == MEASURE_MODE_PPG1)
    {
		Afe4900_LdoTxOn();
        Afe4900_Ppg1Config();
        
        Afe4900_StartSampling();  
        afe4900_is_open = 2;
    }
    else if(mode == MEASURE_MODE_PPG2)
    {		
        Afe4900_LdoTxOn();
        Afe4900_Ppg2Config();
        
        Afe4900_StartSampling();
        afe4900_is_open = 1;
    }
    else if(mode == MEASURE_MODE_REST)
    {		
        Afe4900_LdoTxOn();
        Afe4900_PpgRestConfig();

        Afe4900_StartSampling();
        afe4900_is_open = 1;       
    }
    else
    {
        Afe4900_Close();
        
        afe4900_is_open = 0;
    }
}

/*************************************************************************
** Function name:        Afe4900_Check
** Descriptions:         afe4900 �������
** input parameter:      ��   
** Returned Value:       ��
** Created  by:          WJ
** Created  Datas:       2019-01-10
**************************************************************************/
void Afe4900_Check(void)
{
    if(afe4900_is_open)
    {
        afe4900_check_time++;
        
        if(afe4900_check_time > 10)
        {   //4900���������жϲ���
            BleThread_AFE4404DevClose();
            //memset(&p_data_manage->ecg_ppg_s, 0, sizeof(ecg_ppg_measure_t));
        }    
    }
}

bool Afe4900_IsOpen(void)
{
    return afe4900_is_open;
}

// global 
uint8_t dac_from_4410       = 0x00;        
uint8_t gain_from_4410      = 0x00;
uint8_t charge_from_4410    = 0x00;
// �����ȡ�Ĵ�������
void Afe4900_WearReadReg(void)
{
    // local 
    uint32_t reg3e_value = Afe4900_RegRead(0x3E);
    uint32_t reg3a_value = Afe4900_RegRead(0x3A);
    uint32_t reg21_value = Afe4900_RegRead(0x21);
    uint32_t reg22_value = Afe4900_RegRead(0x22);
    // calc

    dac_from_4410    = (   ( (reg3a_value & 0x01e0 )>> 3)   // MID
                           | ( (reg3e_value & 0x08 )<<3 )  // 
                           | ( (reg3e_value & 0x04)>>1 )
                           | ( (reg3e_value & 0x0200)>>9 )
                       );
    gain_from_4410   = (   ( (reg21_value & 0x40) >> 3 )
                           | ( reg21_value & 0x07 )
                       );
    charge_from_4410 = (   (( reg22_value & 0x3f   ) << 2)
                           | (( reg22_value & 0xc0000) >> 18)
                       );

}
