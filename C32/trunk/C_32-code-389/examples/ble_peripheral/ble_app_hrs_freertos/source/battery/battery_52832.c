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
	
static uint8_t battery_grade = 0;   // ��5����0 = �͵磨0xB8��<3.50V��5%��  ��ѹ�ػ������ͼ����˸��
                                    //        1 = 1�� ��0xC2��<3.68V��15%�� ��ѹ���������ͼ����˸�����˵��Ȳ�����˸��
                                    //        2 = 2�� ��0xC9��<3.86V��50%��
                                    //        3 = 3�� ��0xD0��<3.95V��80%��
                                    //        4 = 4�� ��0xD0��>=3.95V��80%��  ��س�����IO�ڡ�CHARGE_IN�����Ϊ�ж���׼��
                                    //                                       ��������ʾ4��������������ѭ����ʾ1��~4��

static uint16_t battery_value = 0;
static float    battery_v     = 0;
static uint8_t  battery_low_cnt = 0;
static uint8_t battery_grade_low = 0;
static uint8_t battery_chg_full_flg = 1;    //��س���״̬      2:����  1��û����
    
extern system_flag_t system_flag_s;

static void Battery_Check_Level(uint16_t Value);

/****************************************************************
** Function name:           Battery_GetChargingFullState
** Descriptions:            ��ȡ����״̬
** input parameters:        ��
** output parameters:       ��
** Returned value:          2:����  1��û����
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
** Descriptions:            ���ó���״̬
** input parameters:        flg��������־ 2������ 1��û����
** output parameters:       ��
** Returned value:          ��
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
** Descriptions:            ��ʼ��ص������
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
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
** Descriptions:            ֹͣ��ص������
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
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
** Descriptions:            ��ص�������
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
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
** Descriptions:            �����ص����ȼ�
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ             
** Created Date:            2017-01-11
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
// ��5����0 = �͵磨0xB8��<3.50V��5%��  ��ѹ�ػ������ͼ����˸��
//        1 = 1�� ��0xC2��<3.68V��15%�� ��ѹ���������ͼ����˸�����˵��Ȳ�����˸��
//        2 = 2�� ��0xC9��<3.86V��50%��
//        3 = 3�� ��0xD0��<3.95V��80%��
//        4 = 4�� ��0xD0��>=3.95V��80%��  ��س�����IO�ڡ�CHARGE_IN�����Ϊ�ж���׼�� 
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
** Descriptions:            ��ȡ��ص����ȼ�
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
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
** Descriptions:            ��ȡ��ص�������ֵ��ƽ����
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
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
** Descriptions:            ��س��״̬�ܽų�ʼ��
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
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
** Descriptions:            ������ڳ��
** input parameters:        ��
** output parameters:       ��
** Returned value:          ���س��״̬0��û�г�磻1�����ڳ��
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
** Descriptions:            ����Ƿ����
** input parameters:        ��
** output parameters:       ��
** Returned value:          ����0��û�г�����1:����
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
** Descriptions:            ����͵�ػ�״̬
** input parameters:        ��
** output parameters:       ��
** Returned value:          ����0��û�У�1:�͵���Ҫ�ػ�
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
** Descriptions:            ����͵�״̬
** input parameters:        ��
** output parameters:       ��
** Returned value:          ����0��û�У�1:�͵�
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
** Descriptions:            ���ڳ������͵�ػ���־
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
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
