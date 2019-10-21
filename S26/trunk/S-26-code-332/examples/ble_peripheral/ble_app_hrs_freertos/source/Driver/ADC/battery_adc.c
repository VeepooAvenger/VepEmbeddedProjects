/*******************Copyright(c)*********************************
** Veepoo Tech
** COPYRIGHT (C) 2016-2019
**----------------------FileInfo---------------------------------
** File name:                  battery_adc.c
** Last modified Date:         2017-01-04
** Last SYS_VERSION:           0.0.1
** Descriptions:
**---------------------------------------------------------------
** Createdby:                  GT
** Createddate:                2017-01-04
** SYS_VERSION:                0.0.1
** Descriptions:               ˯��
**---------------------------------------------------------------
** Modified by:				
** Modified date:			
** SYS_VERSION:				
** Modified Descriptions:	
****************************************************************/

#include "battery_adc.h"
#include "nrf_drv_saadc.h"
#include "board.h"
#include "cpu_task_data_type.h"
#include "nrf_delay.h"
#include "util.h"
#include "data_manage.h"
#include "Gui_Thread.h"
#include "movement_patterns.h"
#include "No_WearDetect.h"
#include "ble_thread.h"
#include "GSensor.h"

battery_param  st_battery ={0x0000,0x0001};         //��ز���

static uint8_t charge_move = 0;	//��س������ֻ��ƶ�


/****************************************************************
** Function name:             Battery_FullGiveMove
** Descriptions:              ��س����磬gsensor�˶����
** input parameters:          ��
** output parameters:         ��
** Returned value:            ��
** Created by:                JSH
** Created Date:              2017-11-17
****************************************************************/ 
void Battery_GiveMove(void)
{
//	if(st_battery.status  == BATTERY_POWER_FULL)
//	{
		charge_move = 1;
//	}
//	else
//	{
//		charge_move = 0;
//	}
}

/****************************************************************
** Function name:             Battery_TakeMove
** Descriptions:              ������磬��ȡgsensor�˶�״̬
** input parameters:          ��
** output parameters:         ��
** Returned value:            ��
** Created by:                JSH
** Created Date:              2017-11-17
****************************************************************/ 
uint8_t Battery_TakeMove(void)
{
	if(charge_move)
	{
		charge_move = 0;
		
		return true;
	}
	else
	{
		return false;
	}
}

/****************************************************************
** Function name:             Saadc_Callback
** Descriptions:              
** input parameters:          ��
** output parameters:         ��
** Returned value:            ��
** Created by:                GT
** Created Date:              2017-01-04
****************************************************************/ 
static void Saadc_Callback(nrf_drv_saadc_evt_t const * p_event)
{
    
}

/****************************************************************
** Function name:             Battery_ADC_Init
** Descriptions:              ���adc��ʼ��
** input parameters:          ��
** output parameters:         ��
** Returned value:            ��
** Created by:                GT
** Created Date:              2017-01-04
****************************************************************/ 
void Battery_ADC_Init(void)
{
    ret_code_t err_code;
    
    nrf_drv_saadc_config_t m_default_config = {
        .resolution         = NRF_SAADC_RESOLUTION_12BIT,  \
        .oversample         = NRF_SAADC_OVERSAMPLE_DISABLED,  \
        .interrupt_priority = 6,                        \
        .low_power_mode     = 0  
    };
       
    //the parameter NULL means ADC 10 bit resolution
    err_code = nrf_drv_saadc_init(&m_default_config, Saadc_Callback);
    APP_ERROR_CHECK(err_code);
    
    //internal ref: 0.6v; input gain 1/6; single ended mode
    nrf_saadc_channel_config_t batt_detect_channel_config =
    NRF_DRV_SAADC_DEFAULT_CHANNEL_CONFIG_SE(nrf_drv_saadc_gpio_to_ain(VBAT_DET_PIN));    //ADC_DET_PIN  ADC_PIN
    
     //initialization SAADC channel, !!!the number should change when change pin number!!!
    batt_detect_channel_config.gain = NRF_SAADC_GAIN1_2;
    err_code = nrf_drv_saadc_channel_init(0, &batt_detect_channel_config);      
    APP_ERROR_CHECK(err_code); 
    
    Battery_PowerOnProcess();
    if(st_battery.grade == 0)
    {
        st_battery.grade = 1;
        p_data_manage->device_trace_param_s.low_power = 0;
    }
}


/****************************************************************
** Function name:             Battery_IsLowPower
** Descriptions:              ����Ƿ��ǵ͵�ػ�
** input parameters:          ��
** output parameters:         ��
** Returned value:            true false
** Created by:                JSH
** Created Date:              2017-10-14
****************************************************************/ 
bool Battery_IsLowPower(void)
{
	if((st_battery.grade == 0)&&(st_battery.bat_grade_low_cnt >= BATTERY_CHECK_COUNT))
	{
	    return true;
	}
    else
	{
	    return false;
	}
}


/****************************************************************
** Function name:             Battery_PowerOnProcess
** Descriptions:              �ֻ��ϵ�������
**                            ���㹫ʽ: ADC = 4096*v/4.8;
** input parameters:          ��
** output parameters:         ��
** Returned value:            ��
** Created by:                GT
** Created Date:              2017-01-04
****************************************************************/ 

void Battery_PowerOnProcess(void)
{
    uint16_t battery_average = 0;
    uint16_t battery_adc[100] = {0};
    
    // !ע��:������Ҫ�ɼ����ADCֵ
    for(uint8_t index = 0; index < 100; index++)
    {
        nrf_delay_us(2000);
        nrf_drv_saadc_sample_convert(0,(nrf_saadc_value_t *)&battery_adc[index]);
    }
    
    battery_average = Average_ShortArray(battery_adc, 100);
	
	st_battery.bat_grade_low_cnt = 0;
	
	if(battery_average < 2932)  //3.436
	{
//		st_battery.bat_grade_low_cnt = BATTERY_CHECK_COUNT;
		st_battery.grade = 0;
	}
    else if(battery_average < 2991)  //3.506
    {
        st_battery.grade = 1;
    }
    else if(battery_average < 3058)  //3.584
    {
        st_battery.grade = 2;
    }
    else if(battery_average < 3241) //3.799
    {
        st_battery.grade = 3;
    }
    else
    {
        st_battery.grade = 4;
    }
    
    //st_battery.grade = 4;
	
	if(Battery_IsLowPower() == true)
	{
		p_data_manage->device_trace_param_s.low_power = 1;
	}
	else
	{
		p_data_manage->device_trace_param_s.low_power = 0;
	}
}

/****************************************************************
** Function name:             Algorithm_BatteryGradeTraceTask
** Descriptions:              ��ص�������
**                            ���㹫ʽ: ADC = 4096*v/4.8;
** input parameters:          ��
** output parameters:         ��
** Returned value:            ��
** Created by:                GT
** Created Date:              2017-01-04
****************************************************************/ 
void Algorithm_BatteryGradeTraceTask()
{    
    //YX.20180625.���ӳ������״̬���ܸ�Ϊ��磬������³���������ܸı�״̬��������ʹ�ã�
    static uint8_t bat_chg_state = 0;       //0:�������ʹ�� 1����س���� 2������ѳ���
    
    //���ڼ��ĵ�ص��������Ǽ������0.02V�������Լ���0.02V��ADֵ��17
    uint16_t battery_adc_gather = 0;
    static uint8_t power_off_change_flag = 0;
    
    nrf_drv_saadc_sample_convert(0, (nrf_saadc_value_t *)&battery_adc_gather);
    
	if(battery_adc_gather < 2867)   //(3.384->)3.360
	{
		st_battery.bat_grade_low_cnt++;
        st_battery.bat_grade0_cnt = BATTERY_CHECK_COUNT;
        st_battery.bat_grade1_cnt = 0;
        st_battery.bat_grade2_cnt = 0;
        st_battery.bat_grade3_cnt = 0;
        st_battery.bat_grade4_cnt = 0;
	}
    else if(battery_adc_gather < 2932)  //3.436
    {
		st_battery.bat_grade_low_cnt = 0;
        st_battery.bat_grade0_cnt++;
        st_battery.bat_grade1_cnt = 0;
        st_battery.bat_grade2_cnt = 0; 
        st_battery.bat_grade3_cnt = 0;
        st_battery.bat_grade4_cnt = 0;
    }
    else if(battery_adc_gather < 2991)  //3.506
    {
		st_battery.bat_grade_low_cnt = 0;
        st_battery.bat_grade0_cnt = 0;
        st_battery.bat_grade1_cnt++;
        st_battery.bat_grade2_cnt = 0; 
        st_battery.bat_grade3_cnt = 0;
        st_battery.bat_grade4_cnt = 0;
    }
    else if(battery_adc_gather < 3058)  //3.584
    {
		st_battery.bat_grade_low_cnt = 0;
        st_battery.bat_grade0_cnt = 0;
        st_battery.bat_grade1_cnt = 0;
        st_battery.bat_grade2_cnt++;
        st_battery.bat_grade3_cnt = 0;
        st_battery.bat_grade4_cnt = 0;
    }
    else if(battery_adc_gather < 3241)  //3.799
    {
		st_battery.bat_grade_low_cnt = 0;
        st_battery.bat_grade0_cnt = 0;
        st_battery.bat_grade1_cnt = 0;
        st_battery.bat_grade2_cnt = 0;
        st_battery.bat_grade3_cnt++;
        st_battery.bat_grade4_cnt = 0;
    }
    else
    {
		st_battery.bat_grade_low_cnt = 0;
        st_battery.bat_grade0_cnt = 0;
        st_battery.bat_grade1_cnt = 0;
        st_battery.bat_grade2_cnt = 0;
        st_battery.bat_grade3_cnt = 0;
        st_battery.bat_grade4_cnt++;
    }
	
	if(st_battery.bat_grade_low_cnt >= BATTERY_CHECK_COUNT)
	{
	    st_battery.bat_grade_low_cnt = BATTERY_CHECK_COUNT;
		st_battery.grade = 0;
	}

    if(st_battery.bat_grade0_cnt >= BATTERY_CHECK_COUNT)
    {
        st_battery.bat_grade0_cnt = BATTERY_CHECK_COUNT;
        st_battery.grade = 0;
    }
    
    if(st_battery.bat_grade1_cnt >= BATTERY_CHECK_COUNT)
    {
        st_battery.bat_grade1_cnt = BATTERY_CHECK_COUNT;
        st_battery.grade = 1;
    }	    
    
    if(st_battery.bat_grade2_cnt >= BATTERY_CHECK_COUNT)
    {
        st_battery.bat_grade2_cnt = BATTERY_CHECK_COUNT;
        st_battery.grade = 2;
    }

    if(st_battery.bat_grade3_cnt >= BATTERY_CHECK_COUNT)
    {
        st_battery.bat_grade3_cnt = BATTERY_CHECK_COUNT;
        st_battery.grade = 3;
    } 

    if(st_battery.bat_grade4_cnt >= BATTERY_CHECK_COUNT)
    {
        st_battery.bat_grade4_cnt = BATTERY_CHECK_COUNT;
        st_battery.grade = 4;
    }
      
    //st_battery.grade = 4;
    // ���״̬����
    if((0 == nrf_gpio_pin_read(CHARGING_PIN)) && (0 == nrf_gpio_pin_read(CHG_PIN)))
    {
        if(power_off_change_flag == 0 && p_data_manage->device_trace_param_s.sign_on == 0 )
        {
            //YX.2017.12.29.�޸����ػ�ʱ��磬�޷���ȡ��̬������
            BleThread_GsensorRestart();
            power_off_change_flag = 1;
        }
        //YX.20180625.���ӳ������״̬���ܸ�Ϊ��磬������³���������ܸı�״̬
        //YX.20180625.�޸�����У�ҡһҡ�޷���ʾ���ͼ������
        if(bat_chg_state != 2)
        {
            GuiThread_ChargingCommit();
            st_battery.status = BATTERY_POWER_CHARGE;
            bat_chg_state = 1;
        }
		else
		{
			GuiThread_ChargFullCommit();
		}
        Movement_Exit();
        /* ���*/
        Vnwd_SetWear(6);
    }
    else if(nrf_gpio_pin_read(CHARGING_PIN) == 0x00 && nrf_gpio_pin_read(CHG_PIN) == 0x01)
    {
        
        bat_chg_state = 2;
        if(power_off_change_flag == 0 && p_data_manage->device_trace_param_s.sign_on == 0 )
        {
            //YX.2017.12.11.�޸����ػ�ʱ��磬�޷�ʵ��ҡһҡ��ʾ�������ͼ�������
            BleThread_GsensorRestart();
            power_off_change_flag = 1;
        }
        GuiThread_ChargFullCommit();
        Movement_Exit();
        st_battery.status = BATTERY_POWER_FULL;
        /* ���*/
        Vnwd_SetWear(6);
    }
    else
    {
        bat_chg_state = 0;
        st_battery.status = BATTERY_POWER_NORMAL;
        if(Get_NoWearStation() == 6)
        {
            Vnwd_NoWearInit();
        }
        
        //YX.2018.4.13.�޸��ػ����󣬰��³�����������������������
        if(power_off_change_flag == 1 && p_data_manage->device_trace_param_s.sign_on == 0 )
        {
            //�رռ��ٶ�
            GSensor_AccStop();
        }
        power_off_change_flag = 0;
    }
	
	if(p_data_manage->device_trace_param_s.low_power == 0 && Battery_IsLowPower() == true)
	{
		p_data_manage->device_trace_param_s.low_power = 1;
	}
      
	//�͵�״̬�����ڳ��ʱ�ĵ������
	if((p_data_manage->device_trace_param_s.low_power == 1)&&(st_battery.status !=  BATTERY_POWER_NORMAL))
	{
		Battery_PowerOnProcess();
	}
	
	//�͵�ػ�����
	if((p_data_manage->device_trace_param_s.low_power == 1)&&(st_battery.status ==  BATTERY_POWER_NORMAL)
		&&(p_data_manage->device_trace_param_s.sign_on == 1)
		&&(GuiThread_GetEventType() != CPU_EVENT_TASK_LOWPOWER)
	    &&(GuiThread_GetEventType() != CPU_EVENT_TASK_POWEROFF))
	{
	   GuiThread_AddEvent(CPU_EVENT_TASK_LOWPOWER);
	}
}

/****************************************************************
** Function name:             Battery_Get_Grade
** Descriptions:              ��ȡ��صȼ�
** input parameters:          ��
** output parameters:         ��
** Returned value:            ��صȼ�
** Created by:                GT
** Created Date:              2017-01-04
****************************************************************/ 
uint8_t Battery_Get_Grade(void)
{
    return st_battery.grade;
}

/****************************************************************
** Function name:             Battery_Get_Status
** Descriptions:              ��ȡ���״̬
** input parameters:          ��
** output parameters:         ��
** Returned value:            ���״̬
** Created by:                GT
** Created Date:              2017-01-04
****************************************************************/ 
battery_status Battery_Get_Status(void)
{
    return st_battery.status;
}
