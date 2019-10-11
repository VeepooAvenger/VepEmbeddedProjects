/***************************Copyright(c)***********************************
**                       Veepoo Tech
**                  COPYRIGHT (C) 2016-2019
**-----------File Info-----------------------------------------------------
** File        name:     battery_52832.c
** Last modified Date:   2017-08-21
** Last SYS_VERSION:     0.0.0
** Descriptions:            
**-------------------------------------------------------------------------
** Created  by:          WJ
** Created  date:        2017-08-21
** SYS_VERSION:          0.0.0
** Descriptions:         
**-------------------------------------------------------------------------
** Modified by:				
** Modified date:  			          
** SYS_VERSION:                 
** Descriptions:                        
***************************************************************************/

#include "adc.h"
#include "battery.h"
#include "app_type.h"

#if APP_BATTERY_ENABLED   

#define BATTERY_LOW_CNT_MAX     3
	
static uint8_t battery_grade = 0;   // 分5级：0 = 低电（0xB8，<3.50V，5%）  低压关机（电池图标闪烁）
                                    //        1 = 1格 （0xC2，<3.68V，15%） 低压报警（电池图标闪烁）（此档先不做闪烁）
                                    //        2 = 2格 （0xC9，<3.86V，50%）
                                    //        3 = 3格 （0xD0，<3.95V，80%）
                                    //        4 = 4格 （0xD0，>=3.95V，80%）  电池充满以IO口“CHARGE_IN”变低为判定标准。
                                    //                                       充满后显示4格电量。充电中则循环显示1格~4格。

static uint16_t battery_value = 0;
static float    battery_v     = 0;
static uint8_t  battery_low_cnt = 0;
static uint8_t battery_grade_low = 0;
static uint8_t battery_chg_full_flg = 1;    //电池充满状态      2:充满  1：没充满
    
extern system_flag_t system_flag_s;

static void Battery_Check_Level(uint16_t Value);

/****************************************************************
** Function name:           Battery_GetChargingFullState
** Descriptions:            获取充满状态
** input parameters:        无
** output parameters:       无
** Returned value:          2:充满  1：没充满
** Created by:              WJ            
** Created Date:            2017-01-11
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
uint8_t Battery_GetChargingFullState(void)
{
    return battery_chg_full_flg;
}

/****************************************************************
** Function name:           Battery_SetChargingFullState
** Descriptions:            设置充满状态
** input parameters:        flg：充满标志 2：充满 1：没充满
** output parameters:       无
** Returned value:          无
** Created by:              WJ            
** Created Date:            2017-01-11
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Battery_SetChargingFullState(uint8_t flg)
{
    battery_chg_full_flg = flg;
}

/****************************************************************
** Function name:           Battery_TestingStart
** Descriptions:            开始电池电量检测
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ            
** Created Date:            2017-01-11
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Battery_TestingStart(void)
{
    Saadc_StartSample(BATTERY_MODE);  
    system_flag_s.measuring_status = BATT_DECTED_STATUS;
}

/****************************************************************
** Function name:           Battery_TestingStop
** Descriptions:            停止电池电量检测
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ            
** Created Date:            2017-01-11
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Battery_TestingStop(void)
{
    //WJ.2017.02.09
    Saadc_StopSample();//Saadc_StartSample(BATTERY_MODE);
}

/****************************************************************
** Function name:           Battery_TestingCalculate
** Descriptions:            电池电量计算
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ             
** Created Date:            2017-01-11
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Battery_TestingCalculate(void)
{	
    uint8_t  i = 0;
    uint16_t average = 0;
    uint32_t total_value = 0;   
    adc_sample_batt_data_t *adc_sample_batt_data = NULL;

    adc_sample_batt_data = Saadc_GetSampleBattData();
    
    if(adc_sample_batt_data == NULL || adc_sample_batt_data->mode != BATTERY_MODE 
      || adc_sample_batt_data->data_len != SAMPLE_BATT_DETECT_SIZE)
    {
        return;       
    }
    
    for(i = 0; i < SAMPLE_BATT_DETECT_SIZE; i++)
    {
        total_value += (uint16_t)adc_sample_batt_data->data_buff[i];
    }
    average = total_value/SAMPLE_BATT_DETECT_SIZE;

    battery_value = average;   
    Battery_Check_Level(battery_value);
}

/****************************************************************
** Function name:           Battery_Check_Level
** Descriptions:            计算电池电量等级
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ             
** Created Date:            2017-01-11
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
// 分5级：0 = 低电（0xB8，<3.50V，5%）  低压关机（电池图标闪烁）
//        1 = 1格 （0xC2，<3.68V，15%） 低压报警（电池图标闪烁）（此档先不做闪烁）
//        2 = 2格 （0xC9，<3.86V，50%）
//        3 = 3格 （0xD0，<3.95V，80%）
//        4 = 4格 （0xD0，>=3.95V，80%）  电池充满以IO口“CHARGE_IN”变低为判定标准。 
static void Battery_Check_Level(uint16_t Value)
{    
    static uint8_t last_level_cnt = 0;    
//    static uint8_t last_level_level = 0;
    uint8_t c_battery_grade = 0;
    uint8_t p_battery_v = 0;
    
    p_battery_v = battery_v;
    battery_v = (float)battery_value * (0.6f/4096)*2.0f*4.0f; 

    if(battery_v < 2.50f) return;

//    battery_v = 2.8;
    
    if(battery_v < 3.62f)
    {
        battery_low_cnt++;
        c_battery_grade = BATTERY_LEVEL_LOW;
    }
    else if(battery_v < 3.65f)
    {
        c_battery_grade = 0;
        battery_low_cnt = 0;
    }   
    else if(battery_v < 3.70f)
    {
        c_battery_grade = 1;
        battery_low_cnt = 0;
    }   
    else if(battery_v < 3.80f)
    {
        c_battery_grade = 2;
        battery_low_cnt = 0;
    }
    else if(battery_v < 3.98f)
    {
        c_battery_grade = 3;
        battery_low_cnt = 0;
    }
    else
    {
        if(Battery_IsCharging() && battery_v < 4.01f)
        {
            c_battery_grade = 3;
            battery_low_cnt = 0;
        }
        else
        {
            c_battery_grade = 4;
            battery_low_cnt = 0;
        }
    }
    
    if(p_battery_v == 0)
    {
        battery_grade = c_battery_grade;
        last_level_cnt = 0;
    }
    else
    {
        if(battery_grade != c_battery_grade)
        {
            //last_level_level = c_battery_grade;
//            if(Battery_IsCharging() && c_battery_grade <= 1)
//            {
//                battery_grade = c_battery_grade;
//            }
//            else
            {
                if(++last_level_cnt >= 3)
                {
                    last_level_cnt = 0;
                    battery_grade = c_battery_grade;
                }
            }
        }
        else
        {
            last_level_cnt = 0;
        }
        
        if(battery_grade == 0 || battery_grade == BATTERY_LEVEL_LOW )
        {
            
            battery_grade_low = 1;
        }
        else
        {
            battery_grade_low = 0;
        }
    } 
         
    if(Battery_IsChargingFull())
    {
        battery_grade = 4;
        battery_low_cnt = 0;
        last_level_cnt = 0;       
    } 
}

/****************************************************************
** Function name:           Battery_GetGrade
** Descriptions:            获取电池电量等级
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ             
** Created Date:            2017-01-11
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
uint8_t Battery_GetGrade(void)
{
	return battery_grade;
}

/****************************************************************
** Function name:           Battery_GetAdcValue
** Descriptions:            获取电池电量采样值（平均）
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ             
** Created Date:            2017-01-11
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
uint16_t Battery_GetAdcValue(void)
{
	return battery_value;
}


/****************************************************************
** Function name:           Battery_InitPin
** Descriptions:            电池充电状态管脚初始化
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ             
** Created Date:            2017-01-11
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Battery_InitPin(void)
{  
    nrf_gpio_cfg_input(POWER_CHARGING_PIN,  NRF_GPIO_PIN_NOPULL);
    nrf_gpio_cfg_input(POWER_CHG_PIN,       NRF_GPIO_PIN_NOPULL);
}

/****************************************************************
** Function name:           Battery_IsCharging
** Descriptions:            电池正在充电
** input parameters:        无
** output parameters:       无
** Returned value:          返回充电状态0：没有充电；1：正在充电
** Created by:              WJ             
** Created Date:            2017-01-11
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
uint8_t Battery_IsCharging(void)
{
    if(nrf_gpio_pin_read(POWER_CHARGING_PIN) == 0x00 && nrf_gpio_pin_read(POWER_CHG_PIN) == 0x00)
    {
        return 1;
    }            
    return 0;
}

/****************************************************************
** Function name:           Battery_IsChargingFull
** Descriptions:            电池是否充满
** input parameters:        无
** output parameters:       无
** Returned value:          返回0：没有充满，1:充满
** Created by:              WJ             
** Created Date:            2017-01-11
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
uint8_t Battery_IsChargingFull(void)
{   
    if(nrf_gpio_pin_read(POWER_CHARGING_PIN) == 0x00 && nrf_gpio_pin_read(POWER_CHG_PIN) == 0x01)
    {
        return 1;
    }             
    return 0;
   
}

/****************************************************************
** Function name:           Battery_IsLowToShut
** Descriptions:            进入低电关机状态
** input parameters:        无
** output parameters:       无
** Returned value:          返回0：没有，1:低电需要关机
** Created by:              WJ             
** Created Date:            2017-01-11
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
uint8_t Battery_IsLowToShut(void)
{
    return (battery_grade == BATTERY_LEVEL_LOW) && (battery_low_cnt >= BATTERY_LOW_CNT_MAX);
}


/****************************************************************
** Function name:           Battery_IsLow
** Descriptions:            进入低电状态
** input parameters:        无
** output parameters:       无
** Returned value:          返回0：没有，1:低电
** Created by:              WJ             
** Created Date:            2017-01-11
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
uint8_t Battery_IsLow(void)
{
    return (battery_grade == BATTERY_LEVEL_LOW) || (battery_grade == 0 && battery_grade_low == 1);
}

/****************************************************************
** Function name:           Battery_IsLow
** Descriptions:            用于充电清除低电关机标志
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ             
** Created Date:            2017-01-11
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Battery_ClearLowFlag(void)
{
    battery_low_cnt = 0;
}

void Battery_SetFull(void)
{
    if(Battery_IsChargingFull())
    {
        battery_grade = 4;     
    }     
}

#endif //APP_BATTERY_ENABLED
