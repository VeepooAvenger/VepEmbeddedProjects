/***************************Copyright(c)***********************************
**                       Veepoo Tech
**                  COPYRIGHT (C) 2016-2019
**-----------File Info-----------------------------------------------------
** File        name:     heart.c
** Last modified Date:   2017-07-28
** Last SYS_VERSION:     0.0.0
** Descriptions:            
**-------------------------------------------------------------------------
** Created  by:          WJ
** Created  date:        2017-07-28
** SYS_VERSION:          0.0.0
** Descriptions:         
**-------------------------------------------------------------------------
** Modified by:				
** Modified date:  			          
** SYS_VERSION:                 
** Descriptions:                        
***************************************************************************/
#include <string.h>
#include <stdint.h>
#include "board.h"
#include "adc.h"
#include "heart.h"
#include "pulse_rate_sport_version.h"
#include "pedometer.h"
#include "bsp_led.h"
#include "bracelet_protocol.h"
#include "app_type.h"
#include "app_task.h"
#include "vph_wear_dect.h"
#include "heart.h"
#include "app_adc_send.h"
#include "util.h"

#if APP_ADC_HEART_ENABLED

/*********************************************************************
* Macro Definition
*/
#define HEART_RESULT_SIZE       10
#define HEART_SAMPLE_SIZE_200   200
#define HEART_INIT_LED_GEAR     4

/*********************************************************************
* Global Variables
*/

static int16_t heart_adc_sample[1000] = {0x00,}; 
static uint8_t heart_get_mode = 0x00;
static uint8_t heart_rate_buff[HEART_RESULT_SIZE] = {0x00,};
static uint8_t heart_rate_cnt = 0x00;
static uint8_t heart_rate_aver = 0x00;
static uint8_t heart_ble_value = 0x00;

static heart_work_mode_t heart_work_mode = HEART_WORK_MODE_IDLE;
static uint8_t heart_led_gear = 1;

extern system_info_t system_info_s;
extern system_flag_t system_flag_s;
extern system_switch_t system_switch_s;
/*********************************************************************
* local functions
*/

uint8_t  Heart_ValueProcess(uint8_t *heart_data, uint8_t n); 


void Heart_WorkMode_Set(heart_work_mode_t work_mode)
{   
    heart_work_mode = work_mode;
    if(work_mode == HEART_WORK_MODE_BUSY)
    {
        Adc_SendInit(HEART_SAMPLE_SIZE_200);
        Led_SetGear(heart_led_gear);
    }     
}

heart_work_mode_t Heart_WorkMode_Get(void)
{   
    return heart_work_mode;
}

uint8_t Heart_IsBusy(void)
{
    return (heart_work_mode == HEART_WORK_MODE_BUSY)?1:0;
}


/****************************************************************
** Function name:           Heart_ClearAverValue
** Descriptions:            清除心率平均值和计数器
** input parameters:          
** output parameters:
** Returned value:
** Created by:              WJ              
** Created Date:            2017-08-16
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/
void Heart_ClearAverValue(void)
{
    heart_rate_aver = 0;
    heart_rate_cnt = 0x00;    
}

/****************************************************************
** Function name:           Heart_Open
** Descriptions:            
** input parameters:          
** output parameters:
** Returned value:
** Created by:              WJ              
** Created Date:            2017-01-04
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/
void Heart_Open(void)
{
    heart_get_mode = 0;
    heart_rate_cnt = 0;
    
    PIN_LOW(LDO_EN_PIN); 
    
    if(system_switch_s.led_gear == 0x01)
    {
        heart_led_gear = 1;
    }
    else
    {
        heart_led_gear = 6;
    }    
    Led_SetGear(heart_led_gear);
    
    if(system_info_s.wear_detect == 0x01 && system_flag_s.sign_test == 0x00 
      && system_flag_s.movement_patterns != 1 && system_switch_s.led_gear == 0x01)
    {
        //需要等待佩戴检测
        Heart_WorkMode_Set(HEART_WORK_MODE_WAITE);              
        Saadc_StartSample(HEART_WEAR_MODE);
        Wear_DectInit();
    }
    else
    {
        Heart_WorkMode_Set(HEART_WORK_MODE_BUSY);        
        Saadc_StartSample(HEART_MODE);  
    }     
    
    heart_rate_aver = 0;
    heart_rate_cnt = 0x00;
    heart_ble_value = 0;
          
    Pulse_RateInit();       //WJ.20170327
}

/****************************************************************
** Function name:           Heart_Close
** Descriptions:            
** input parameters:          
** output parameters:
** Returned value:
** Created by:              WJ              
** Created Date:            2017-01-04
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/
void Heart_Close(void)
{
    if(system_info_s.wear_detect == 0x01)
    {
        Wear_Close();
    }    
    
    //关闭led
    Led_SetGear(0); 
    //YX.20180810.解决心率灯关灯失败的问题
    heart_led_gear = 0;
    //关闭PPI
    Saadc_StopSample();
    //关闭LDO，模拟前端
    PIN_HIGH(LDO_EN_PIN);  
    
    Heart_WorkMode_Set(HEART_WORK_MODE_IDLE);
      
    Led_CloseAll();
}

/****************************************************************
** Function name:           Heart_Calculate_Process
** Descriptions:            
** input parameters:          
** output parameters:
** Returned value:
** Created by:              WJ              
** Created Date:            2017-01-04
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/
void Heart_Calculate_Process(void)
{
    uint16_t i = 0;
//	static  uint8_t pga_gain_adj_timer = 0;
    adc_sample_data_t *adc_sample_data = NULL;
    move_index_t mig_move_index;
    pulse_rate_t pulse_rate;
    
    adc_sample_data = Saadc_GetSampleData();    
    
    if(adc_sample_data->mode != HEART_MODE && adc_sample_data->mode != HEART_WEAR_MODE )
    {
        return;
    }
 
    //WJ.20171212.解决第五次填充完整后没有计算的问题
    if(heart_get_mode < 4)
    {
        memcpy(&heart_adc_sample[heart_get_mode * 200], adc_sample_data->data_buff, adc_sample_data->data_len * 2);       
        Adc_SetSendBuf(adc_sample_data->data_buff, HEART_SAMPLE_SIZE_200);        
        heart_get_mode++;  
        return;
    }
    //WJ.20171212.解决第五次填充完整后没有计算的问题
    if(heart_get_mode == 4)
    {
        memcpy(&heart_adc_sample[heart_get_mode * 200], adc_sample_data->data_buff, adc_sample_data->data_len * 2);       
        Adc_SetSendBuf(adc_sample_data->data_buff, HEART_SAMPLE_SIZE_200); 
        heart_get_mode++;
    }
    else
    {       
        for( i = 0; i < 800; i++)
        {
            heart_adc_sample[i] = heart_adc_sample[i + 200];
        }
        
        memcpy(&heart_adc_sample[800], adc_sample_data->data_buff, adc_sample_data->data_len * 2);    
        Adc_SetSendBuf(adc_sample_data->data_buff, HEART_SAMPLE_SIZE_200); 
        
    }   
    Move_IdexGet(&mig_move_index);
    Pulse_RateDetection(heart_adc_sample, mig_move_index);//.move_5s);  
    
    Get_PulseRateResult(&pulse_rate);  
       
    if (pulse_rate.pulse >= 30 && pulse_rate.pulse <= 200)
    {
        heart_ble_value = pulse_rate.pulse;
        
        if(heart_rate_cnt < HEART_VALID_DATA_NUM)
        {
            heart_rate_buff[heart_rate_cnt++] = heart_ble_value;  
            if(heart_rate_cnt >= HEART_VALID_DATA_NUM)
            {
                heart_rate_aver = Heart_ValueProcess(heart_rate_buff, HEART_VALID_DATA_NUM);
            }
            else
            {
                heart_rate_aver = 0;
            }    
        }        
    }  
}

/****************************************************************
** Function name:           Heart_GetAveragePulseRate
** Descriptions:            Get the average pulse rate for storage 
** input parameters:        none
** output parameters:       none
** Returned value:          heart_rate_aver
** Created by:              HYR              
** Created Date:            2017-07-21
**----------------------------------------------------------------
*****************************************************************/
uint8_t Heart_GetAveragePulseRate(void)
{
    return heart_rate_aver;
}

/****************************************************************
** Function name:           Heart_GetValidPulseRateNum
** Descriptions:            Get the valid pulse rate num 
** input parameters:        none
** output parameters:       none
** Returned value:          heart_rate_cnt
** Created by:              HYR              
** Created Date:            2017-07-21
**----------------------------------------------------------------
*****************************************************************/
uint8_t Heart_GetValidPulseRateNum(void)
{
    return heart_rate_cnt;
}

/****************************************************************
** Function name:           Heart_ValueProcess
** Descriptions:            Process the average pulse rate for storage 
** input parameters:        hvp_heart_data hvp_valid_pulse_num
** output parameters:       none
** Returned value:          hvp_heart_aver
** Created by:              WJ              
** Created Date:            2017-01-04
**----------------------------------------------------------------
** Modified by:             HYR 
** Modified date:           20170721. Change the process for limit number of pulse 
**----------------------------------------------------------------
*****************************************************************/
uint8_t  Heart_ValueProcess(uint8_t *hvp_heart_data, uint8_t hvp_valid_pulse_num) 
{
    uint8_t  i = 0;
    uint8_t  hvp_heart_aver = 0x00;  
    uint16_t heart_sum  = 0x00;  
    if (hvp_valid_pulse_num > 2)  //
    {
        Util_CommonSort(hvp_heart_data, hvp_valid_pulse_num);

        for(i = 1; i < hvp_valid_pulse_num - 1; i++)
        {
            heart_sum += hvp_heart_data[i];
        }
      
        hvp_heart_aver = heart_sum/(hvp_valid_pulse_num-2);
    }
 
    return hvp_heart_aver; 
         
}

/****************************************************************
** Function name:           Heart_BleSend_Value
** Descriptions:            发现心率值到APP
** input parameters:          
** output parameters:
** Returned value:
** Created by:              WJ              
** Created Date:            2017-01-05
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/
void Heart_BleSend_Value(void)
{
    uint8_t heart_ble_buf[20] = {0x00, };    
    
    heart_ble_buf[0] = 0xD0;
    heart_ble_buf[1] = heart_ble_value;
    heart_ble_buf[2] = 0x00;
    heart_ble_buf[3] = 0x00;    
   
    Ble_SendDataViaBat(heart_ble_buf, 20);      
}

/****************************************************************
** Function name:           Heart_BleSend_TestInvalid
** Descriptions:            心率测试无效
** input parameters:          
** output parameters:
** Returned value:
** Created by:              WJ              
** Created Date:            2017-01-05
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/
void Heart_BleSend_TestInvalid(void)
{
    uint8_t heart_ble_buf[20] = {0x00, };    
    
    heart_ble_buf[0] = 0xD0;
    heart_ble_buf[1] = 0x01;    //提示请正常佩戴
    heart_ble_buf[2] = 0x00;
    heart_ble_buf[3] = 0x00;    
   
    Ble_SendDataViaBat(heart_ble_buf, 20);      
}

uint8_t Heart_Test_Value_Get(void)
{
    return heart_ble_value;
}


void Heart_UICloseCheck(void)
{    
    if(system_flag_s.app_heart_status == 1 
        && system_flag_s.measuring_status != BUTTON_HEARTING_STATUS 
        && system_flag_s.measuring_status != APP_HEARTING_STATUS )
    {  
        Heart_BleSend_TestInvalid();  
        Heart_BleSend_TestInvalid();              
        system_flag_s.app_heart_status = 0;
    }
}

#endif //APP_ADC_HEART_ENABLED
