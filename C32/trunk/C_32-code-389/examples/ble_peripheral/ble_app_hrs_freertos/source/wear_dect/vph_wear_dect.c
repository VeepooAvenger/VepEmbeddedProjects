/***************************Copyright(c)***********************************
**                       Veepoo Tech
**                  COPYRIGHT (C) 2016-2019
**-----------File Info-----------------------------------------------------
** File        name:     vph_wear_dect.c
** Last modified Date:   2017-08-24
** Last SYS_VERSION:     0.0.0
** Descriptions:            
**-------------------------------------------------------------------------
** Created  by:          WJ
** Created  date:        2017-08-24
** SYS_VERSION:          0.0.0
** Descriptions:         
**-------------------------------------------------------------------------
** Modified by:				
** Modified date:  			          
** SYS_VERSION:                 
** Descriptions:                        
***************************************************************************/
#include "pwm.h"
#include "vph_wear_dect.h"
#include "photoele_wear_detect.h"
#include "adc.h"
#include "blood_pressure.h"
#include "heart.h"
#include "bsp_led.h"
#include "app_handler.h"
#include "nowear_detection.h"
#include "app_type.h"

#define VPH_WEAR_DECT_MAC_CNT       1000

#define IF_WATCH_OR_BAND    0    //20170825 HJJ 0:手环,   1:手表

static bool wld_pwm_lock_falg = 0;
uint8_t vwd_wear_flag   = 1;     // 0:WEAR 1:Do not WEAR 2/3: Long time no wear
uint16_t wear_detect_cnt = 0;
uint8_t wear_detect_state = 0;
static int16_t wear_sample_buf_bak[SAMPLE_WEAR_DETECT_SIZE] = {0x00,};
static int16_t wear_adc_index = 0;

static uint8_t wear_change_flag = 0;        //佩戴开关状态变化

#if IF_WATCH_OR_BAND 
static int16_t puwd_temp_buff[512] = {0x00,};
static uint16_t un_wear_affirm_cnt = 0;
static uint8_t wear_move_away = 1;//20170622 HJJ
#endif

extern system_flag_t system_flag_s;
extern system_info_t system_info_s;

/********************************************************************
** Function name:           Wear_GetWearFlag
** Descriptions:            获取佩戴标志
** input parameters:        none 
** output parameters:       none
** Returned value:          vwd_wear_flag
** Created by:              HYR
** Created Date:            2016-09-20
**-------------------------------------------------------------------
** Modified by:
** Modified date:
**-------------------------------------------------------------------
********************************************************************/
uint8_t Wear_GetWearFlag(void)
{
	return vwd_wear_flag;
}

/****************************************************************
** Function name:           Wear_ClearWearFlag
** Descriptions:            清除佩戴标志
** input parameters:        无  
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-01-05
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/
void Wear_ClearWearFlag(void)
{
    vwd_wear_flag = 0;
}

/********************************************************************
** Function name:           Wear_DectInit
** Descriptions:            开启佩戴检测
** input parameters:        none 
** output parameters:       none
** Returned value:          none
** Created by:              HYR
** Created Date:            2016-09-19
**-------------------------------------------------------------------
** Modified by:
** Modified date:
**-------------------------------------------------------------------
********************************************************************/
void Wear_DectInit(void)
{
    wld_pwm_lock_falg = 0;
    wear_detect_cnt = 0;
    vwd_wear_flag = 1;
    wear_detect_state = 1;
    Pwm_Start(20, 50);
    
    wear_change_flag = 0;
#if IF_WATCH_OR_BAN    
    wear_move_away = 1;
    un_wear_affirm_cnt = 0;
#endif
}

/****************************************************************
** Function name:           Wear_SendBleTestInvalid
** Descriptions:            佩戴不通过发送测试无效到app
** input parameters:        无  
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-01-05
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/
static void Wear_SendBleTestInvalid(uint8_t type)
{
    if(type == HEART_WEAR_MODE)
    {                         
        Heart_BleSend_TestInvalid();
    }
    else if(type == BP_WEAR_MODE)
    {
        if(system_flag_s.measuring_status == APP_BLOOD_PRESSURE_STATUS)
        {          
            Bp_BleSendTestInvalid();
        }
    }   
}

/****************************************************************
** Function name:           Wear_Close
** Descriptions:            关闭佩戴检测
** input parameters:        无  
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-01-05
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/
void Wear_DectPwmClose(uint8_t mode)
{
    if(mode == HEART_WEAR_MODE)
    {                       
        Heart_Close();
    }
    else if(mode == BP_WEAR_MODE)
    {
        Bp_Close();
    }
    else
    {
        Wear_Close();
    }
    
    Wear_SendBleTestInvalid(mode);
    Wear_SendBleTestInvalid(mode);
    Led_SetGear(0);
    system_flag_s.measuring_status  = EVENT_NULL;
    system_flag_s.heart_led_status = false;
    system_flag_s.bp_calibration = 0;
    wear_detect_cnt = 0;
    wear_detect_state = 0; 
}


/********************************************************************
** Function name:           Wear_DectCalculate
** Descriptions:            佩戴计算
** input parameters:        wear_test_timer  
** output parameters:       none
** Returned value:          none
** Created by:              HYR
** Created Date:            2016-09-18
**-------------------------------------------------------------------
** Modified by:
** Modified date:
**-------------------------------------------------------------------
********************************************************************/
uint8_t Wear_DectCalculate(void)
{
#if IF_WATCH_OR_BAND     
    uint8_t un_wear_affirm_state = 0;
	uint8_t updata_len = 0;
#endif    
	uint16_t i = 0;    
    adc_sample_wear_data_t *adc_sample_wear_data = NULL;
    
    if(system_flag_s.movement_patterns == 1)
    {
        /* 进入运动模式不做佩戴检测*/
        return 0;
    }
    
    //佩戴开关状态发生改变不再佩戴检测，直到下次开启生效
    if(wear_change_flag == 1)
    {
        return 0;
    }  
      
#if (!IF_WATCH_OR_BAND)     
    if((system_flag_s.measuring_status == TIME_HEARTING_STATUS || system_flag_s.measuring_status == TIME_BLOOD_PRESSURE_STATUS)
       && vwd_wear_flag == 0)
    {
        return vwd_wear_flag;
    }
#endif

    adc_sample_wear_data = Saadc_GetSampleWearData();
    
    if((adc_sample_wear_data->mode != WEAR_DETECT_MODE 
        && adc_sample_wear_data->mode != HEART_WEAR_MODE
        && adc_sample_wear_data->mode != BP_WEAR_MODE) || adc_sample_wear_data->data_len == 0)
    {
        return vwd_wear_flag;
    }

    memcpy(wear_sample_buf_bak, adc_sample_wear_data->data_buff, SAMPLE_WEAR_DETECT_SIZE*2);
    wear_adc_index = 0;
    for(i = 0; i < adc_sample_wear_data->data_len; i++)
    {
        wear_detect_cnt++;
        if (vwd_wear_flag  == 1)
        {
            vwd_wear_flag = Photoeleticity_WearDect(30, wear_sample_buf_bak[i]);//12 //30
            
            if (vwd_wear_flag == 0 && wld_pwm_lock_falg == 0)
            {
                Pwm_Stop();
//                Led_SetGear(4);
                if(adc_sample_wear_data->mode == HEART_WEAR_MODE)
                {
                    Heart_WorkMode_Set(HEART_WORK_MODE_BUSY);
                }
                else if(adc_sample_wear_data->mode == BP_WEAR_MODE)
                {
                    Bp_SetWorkMode(BP_WORK_MODE_BUSY);
                }
                else
                {
                    Wear_Close();
                }

                Saadc_CleanAdcDataLen();
                App_DeleteEvent(APP_ADC_SAMPLING_COMPLETED_EVENT);
                wld_pwm_lock_falg = 1;
                wear_detect_cnt = 0;
                wear_detect_state = 0;    
#if IF_WATCH_OR_BAND  
                wear_move_away = 0;    
                un_wear_affirm_cnt = 0;
#endif                
                return vwd_wear_flag;
            }
            if (wear_detect_cnt > VPH_WEAR_DECT_MAC_CNT)
            {
                Pwm_Stop();
                Wear_DectPwmClose(adc_sample_wear_data->mode);
                vwd_wear_flag = 1;
                break;
            }
        }
        else
        {
#if IF_WATCH_OR_BAND 		
            //WJ.20170905.自动测量防止重复进入佩戴检测
            if((system_flag_s.measuring_status == TIME_HEARTING_STATUS 
              || system_flag_s.measuring_status == TIME_BLOOD_PRESSURE_STATUS))
            { 
                continue;
            }
#endif			
            if (wear_detect_cnt > 50)  // Protect from jump 0/1 too fast
            {
                vwd_wear_flag = Photoeleticity_UnWearDect(12, wear_sample_buf_bak[i]);
                if (vwd_wear_flag)
                {
                    if(adc_sample_wear_data->mode == HEART_WEAR_MODE)
                    {                         
                        Heart_WorkMode_Set(HEART_WORK_MODE_WAITE);
                    }
                    else if(adc_sample_wear_data->mode == BP_WEAR_MODE)
                    {
                        Bp_SetWorkMode(BP_WORK_MODE_WAITE);
                    }
                    Wear_DectInit();
                    wear_detect_cnt = 0;
                    break;
                }
            }
        }
    }
    
#if IF_WATCH_OR_BAND   
	if(vwd_wear_flag == 0 && wear_move_away == 0 && wear_detect_cnt > 50)
	{
		if(un_wear_affirm_cnt + adc_sample_wear_data->data_len < 512)
		{
			memcpy(&puwd_temp_buff[un_wear_affirm_cnt], adc_sample_wear_data->data_buff1, adc_sample_wear_data->data_len*2);
			un_wear_affirm_cnt += adc_sample_wear_data->data_len;
		}
		else
		{
			updata_len = un_wear_affirm_cnt + adc_sample_wear_data->data_len - 512;
			for(i = 0; i < (512 - updata_len); i++)
			{
				puwd_temp_buff[i] = puwd_temp_buff[i+updata_len];
			}
			memcpy(&puwd_temp_buff[512 - adc_sample_wear_data->data_len], 
			adc_sample_wear_data->data_buff1, adc_sample_wear_data->data_len*2);                      
			un_wear_affirm_cnt = 512;
            //WJ.20170913.过滤前三秒的数据
			//if(wear_detect_cnt > 700 && wear_detect_cnt < 800)
            if(wear_detect_cnt > (512 + 600) && wear_detect_cnt < (512 + 700))
			{		
                un_wear_affirm_state = Un_WearDetectAffirm(puwd_temp_buff);
            
                if(un_wear_affirm_state == 5)   //佩戴不通过
                {
                    Wear_DectPwmClose(adc_sample_wear_data->mode);
                    vwd_wear_flag = 5;       //1;//20170622HJJ
                }
            }
            else if(wear_detect_cnt > (512 + 700))
            {
                wear_move_away = 1;
            }
        } 
    }
    
#endif    
	return vwd_wear_flag;
}

/****************************************************************
** Function name:           Wear_Close
** Descriptions:            关闭佩戴检测
** input parameters:        无  
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-01-05
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/
void Wear_Close(void)
{
    adc_sample_wear_data_t *adc_sample_wear_data = NULL;
    
    adc_sample_wear_data = Saadc_GetSampleWearData();
    
    if(wear_detect_state == 1)
    {
        Pwm_Stop();
        wear_detect_state = 0;
    }
//    if(system_info_s.wear_detect != 0x01 || system_switch_s.led_gear == 0x01 )
//   {
        vwd_wear_flag = 0;           
//    }

    if(system_flag_s.measuring_status > TIME_START_MEASURING_STATUS)
    {   
        if(adc_sample_wear_data->mode == HEART_WEAR_MODE)
        {
            Heart_WorkMode_Set(HEART_WORK_MODE_BUSY);
        }
        else if(adc_sample_wear_data->mode == BP_WEAR_MODE)
        {
            Bp_SetWorkMode(BP_WORK_MODE_BUSY);
        }
    }
}

/****************************************************************
** Function name:           Wear_BleSendADC
** Descriptions:            发送ADC到APP
** input parameters:        无  
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-01-05
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/
uint8_t Wear_BleSendADC(void)
{
    uint8_t wear_ble_buf[20] = {0x00,};
 
    memcpy(wear_ble_buf, &wear_sample_buf_bak[wear_adc_index], 20); 
    
    wear_adc_index += 10;
      
    Ble_SendDataViaClk(wear_ble_buf, 20);
    
    if(wear_adc_index >= SAMPLE_WEAR_DETECT_SIZE)
    {
        wear_adc_index = 0;
        return 0;
    }
    
    return 1;
}

/****************************************************************
** Function name:           Wear_DectGetState
** Descriptions:            是否在进行佩戴检测
** input parameters:        无  
** output parameters:       无
** Returned value:          0：没有；1：正在进行
** Created by:              WJ              
** Created Date:            2017-01-05
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/
uint8_t Wear_DectGetState(void)
{   
    return wear_detect_state;
}

/****************************************************************
** Function name:           Wear_SetWearSwitchState
** Descriptions:            佩戴检测开关状态变化，不再做佩戴检测
** input parameters:        无  
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-01-05
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/
void Wear_SetWearSwitchState(void)
{   
    wear_change_flag = 1;
}
