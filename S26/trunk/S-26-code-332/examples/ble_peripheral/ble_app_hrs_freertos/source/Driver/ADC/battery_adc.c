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
** Descriptions:               睡眠
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

battery_param  st_battery ={0x0000,0x0001};         //电池参数

static uint8_t charge_move = 0;	//电池充满，手环移动


/****************************************************************
** Function name:             Battery_FullGiveMove
** Descriptions:              电池充满电，gsensor运动监测
** input parameters:          无
** output parameters:         无
** Returned value:            无
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
** Descriptions:              电池满电，获取gsensor运动状态
** input parameters:          无
** output parameters:         无
** Returned value:            无
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
** input parameters:          无
** output parameters:         无
** Returned value:            无
** Created by:                GT
** Created Date:              2017-01-04
****************************************************************/ 
static void Saadc_Callback(nrf_drv_saadc_evt_t const * p_event)
{
    
}

/****************************************************************
** Function name:             Battery_ADC_Init
** Descriptions:              电池adc初始化
** input parameters:          无
** output parameters:         无
** Returned value:            无
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
** Descriptions:              电池是否是低电关机
** input parameters:          无
** output parameters:         无
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
** Descriptions:              手环上电计算电量
**                            计算公式: ADC = 4096*v/4.8;
** input parameters:          无
** output parameters:         无
** Returned value:            无
** Created by:                GT
** Created Date:              2017-01-04
****************************************************************/ 

void Battery_PowerOnProcess(void)
{
    uint16_t battery_average = 0;
    uint16_t battery_adc[100] = {0};
    
    // !注意:这里需要采集电池ADC值
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
** Descriptions:              电池电量跟踪
**                            计算公式: ADC = 4096*v/4.8;
** input parameters:          无
** output parameters:         无
** Returned value:            无
** Created by:                GT
** Created Date:              2017-01-04
****************************************************************/ 
void Algorithm_BatteryGradeTraceTask()
{    
    //YX.20180625.增加充满电后，状态不能改为充电，必须拔下充电器，才能改变状态（彩屏下使用）
    static uint8_t bat_chg_state = 0;       //0:电池正常使用 1：电池充电中 2：电池已充满
    
    //由于检测的电池电量与我们计算的有0.02V的相差，所以加上0.02V，AD值：17
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
    // 电池状态更新
    if((0 == nrf_gpio_pin_read(CHARGING_PIN)) && (0 == nrf_gpio_pin_read(CHG_PIN)))
    {
        if(power_off_change_flag == 0 && p_data_manage->device_trace_param_s.sign_on == 0 )
        {
            //YX.2017.12.29.修复：关机时充电，无法获取姿态的问题
            BleThread_GsensorRestart();
            power_off_change_flag = 1;
        }
        //YX.20180625.增加充满电后，状态不能改为充电，必须拔下充电器，才能改变状态
        //YX.20180625.修复充电中，摇一摇无法显示充电图标问题
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
        /* 充电*/
        Vnwd_SetWear(6);
    }
    else if(nrf_gpio_pin_read(CHARGING_PIN) == 0x00 && nrf_gpio_pin_read(CHG_PIN) == 0x01)
    {
        
        bat_chg_state = 2;
        if(power_off_change_flag == 0 && p_data_manage->device_trace_param_s.sign_on == 0 )
        {
            //YX.2017.12.11.修复：关机时充电，无法实现摇一摇显示充满电大图标的问题
            BleThread_GsensorRestart();
            power_off_change_flag = 1;
        }
        GuiThread_ChargFullCommit();
        Movement_Exit();
        st_battery.status = BATTERY_POWER_FULL;
        /* 充电*/
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
        
        //YX.2018.4.13.修复关机充电后，拔下充电器，翻腕可以亮屏的问题
        if(power_off_change_flag == 1 && p_data_manage->device_trace_param_s.sign_on == 0 )
        {
            //关闭加速度
            GSensor_AccStop();
        }
        power_off_change_flag = 0;
    }
	
	if(p_data_manage->device_trace_param_s.low_power == 0 && Battery_IsLowPower() == true)
	{
		p_data_manage->device_trace_param_s.low_power = 1;
	}
      
	//低电状态，正在充电时的电量检测
	if((p_data_manage->device_trace_param_s.low_power == 1)&&(st_battery.status !=  BATTERY_POWER_NORMAL))
	{
		Battery_PowerOnProcess();
	}
	
	//低电关机保护
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
** Descriptions:              获取电池等级
** input parameters:          无
** output parameters:         无
** Returned value:            电池等级
** Created by:                GT
** Created Date:              2017-01-04
****************************************************************/ 
uint8_t Battery_Get_Grade(void)
{
    return st_battery.grade;
}

/****************************************************************
** Function name:             Battery_Get_Status
** Descriptions:              获取电池状态
** input parameters:          无
** output parameters:         无
** Returned value:            电池状态
** Created by:                GT
** Created Date:              2017-01-04
****************************************************************/ 
battery_status Battery_Get_Status(void)
{
    return st_battery.status;
}
