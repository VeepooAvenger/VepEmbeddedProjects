/***************************Copyright(c)***********************************
**                       Veepoo Tech
**                  COPYRIGHT (C) 2016-2019
**-----------File Info-----------------------------------------------------
** File        name:     blood_pressure.c
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
#include "bsp_led.h"
#include "blood_pressure.h"
#include "app_task.h"
#include "app_type.h"
#include "pedometer.h"
#include "vph_wear_dect.h"  
#include "app_adc_send.h"

/*********************************************************************
* Macro Definition
*/
#define BP_SAMPLE_NUM_512       512
#define BP_SAMPLE_NUM_256		256

#define BP_INIT_LED_GEAR        4

#define BP_CALIBRATION_SET_DIFFERENCE   24
#define BP_CALIBRATION_RESULT_DIFFERENCE   15

/*********************************************************************
* Global Variables
*/

//static bp_option_t    bp_option;
static test_mode_t    bp_mode;

static int16_t bp_sample_buf[512] = {0x00,};
static uint8_t bp_state = 0;
static uint8_t bp_led_gear = 0;
//static uint8_t bp_sample_cnt = 0; //用于采样数据更新512个点
static bp_work_mode_t bp_work_mode = BP_WORK_MODE_IDLE;

extern system_info_t system_info_s;
extern system_flag_t system_flag_s;
extern system_switch_t system_switch_s;
/*********************************************************************
* local functions
*/
static uint8_t Bp_ResultCheck(uint8_t check_mode, uint8_t check_sp, 
                                  uint8_t check_dp, uint8_t check_heart);

static uint8_t Bp_SetCalibrationCheck(uint8_t check_sp, uint8_t check_dp);
/****************************************************************
** Function name:           Bp_GetBpMode
** Descriptions:            获取血压模式
** input parameters:
** output parameters:
** Returned value:
** Created by:              WJ              
** Created Date:            2016-12-05
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
test_mode_t *Bp_GetBpMode(void)
{
    return &bp_mode;
};

/****************************************************************
** Function name:           Bp_SetWorkMode
** Descriptions:            设置测量工作状态
** input parameters:
** output parameters:
** Returned value:
** Created by:              WJ              
** Created Date:            2016-12-05
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Bp_SetWorkMode(bp_work_mode_t work_mode)
{   
    bp_work_mode = work_mode;
    if(work_mode == BP_WORK_MODE_BUSY)
    {
        Adc_SendInit(BP_SAMPLE_NUM_256); 
        Led_SetGear(bp_led_gear);         
    }    
}

/****************************************************************
** Function name:           Bp_GetWorkMode
** Descriptions:            获取测量工作状态
** input parameters:
** output parameters:
** Returned value:
** Created by:              WJ              
** Created Date:            2016-12-05
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
bp_work_mode_t Bp_GetWorkMode(void)
{   
    return bp_work_mode;
}

/****************************************************************
** Function name:           Bp_IsWorkBusy
** Descriptions:            测量忙碌中
** input parameters:
** output parameters:
** Returned value:
** Created by:              WJ              
** Created Date:            2016-12-05
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
uint8_t Bp_IsWorkBusy(void)
{
    return (bp_work_mode == BP_WORK_MODE_BUSY)?1:0;
}

/****************************************************************
** Function name:           Bp_SetBpMode
** Descriptions:            设置测量模式
** input parameters:
** output parameters:
** Returned value:
** Created by:              WJ              
** Created Date:            2016-12-05
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
uint8_t Bp_SetBpMode(uint8_t test_mode)
{
    uint8_t ret = 1;
    
    if(test_mode == 1)
    {
        bp_mode.test_mode = 1;
        bp_mode.set_dp = system_info_s.bp_test_mode.set_dp;
        bp_mode.set_sp = system_info_s.bp_test_mode.set_sp;
        
        ret = Bp_SetCalibrationCheck(bp_mode.set_sp, bp_mode.set_dp);
        if(ret != 1)
        {
            ret = 0;
        }     
    }
    
    if(ret == 0 || test_mode == 0)
    {
        bp_mode.test_mode = 0;    
        bp_mode.set_dp = 0;
        bp_mode.set_sp = 0;
	}
	return ret ;
}

/****************************************************************
** Function name:           Bp_Open
** Descriptions:            开启血压测量
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
void Bp_Open(void)
{    
    PIN_LOW(LDO_EN_PIN); 

    if(system_switch_s.led_gear == 0x01)
    {
        bp_led_gear = 1;
    }
    else
    {
        bp_led_gear = 6;
    } 
    
    Led_SetGear(bp_led_gear); 
    Bp_SetBpMode(system_info_s.bp_test_mode.test_mode);
  
       
    if(system_info_s.wear_detect == 0x01 && system_flag_s.sign_test == 0x00
      && system_switch_s.led_gear == 0x01)
    {
        //需要等待佩戴检测
        Bp_SetWorkMode(BP_WORK_MODE_WAITE); 
        Wear_DectInit();                     
        Saadc_StartSample(BP_WEAR_MODE); 
    }
    else
    {
        Bp_SetWorkMode(BP_WORK_MODE_BUSY);        
        Saadc_StartSample(BP_MODE);  
    } 
      
    bp_state = 0;
    BP_DetectionInit();      
}

/****************************************************************
** Function name:           Bp_Close
** Descriptions:            关闭测量
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
void Bp_Close(void)
{     
    if(system_info_s.wear_detect == 0x01)
    {
        Wear_Close();
    }    
    //关闭led
    Led_SetGear(0);  
    bp_led_gear = 0;    
    //关闭PPI
    Saadc_StopSample();
    //关闭LDO，模拟前端
    PIN_HIGH(LDO_EN_PIN);   
    Bp_SetWorkMode(BP_WORK_MODE_IDLE);
  
    Led_CloseAll();
}

/****************************************************************
** Function name:           Bp_CalculateProcess
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
void Bp_CalculateProcess(void)
{
    uint16_t i = 0;
//	static  uint8_t pga_gain_adj_timer = 0;
    adc_sample_data_t *adc_sample_data = NULL;
//	bp_info_t *info;
    move_index_t mig_move_index; 
    
    adc_sample_data = Saadc_GetSampleData();    
    
    if(adc_sample_data->mode != BP_MODE && adc_sample_data->mode != BP_WEAR_MODE)
    {
        return;
    }

    if(bp_state == 0)
    {
        BP_DetectionInit();
        memset(bp_sample_buf, 0x00, 512); //前256个数据清零
        for(i = BP_SAMPLE_NUM_256; i < BP_SAMPLE_NUM_512 ; i++)	//每次更新256，先进先出
        {			  
            bp_sample_buf[i] = adc_sample_data->data_buff[i - BP_SAMPLE_NUM_256] >> 2;	
        }
//        Adc_SetSendBuf(&bp_sample_buf[BP_SAMPLE_NUM_256], BP_SAMPLE_NUM_256);
        Adc_SetSendBuf(adc_sample_data->data_buff, BP_SAMPLE_NUM_256);
        bp_state = 1;       
    }
    else
    {
        for(i = BP_SAMPLE_NUM_256; i < (BP_SAMPLE_NUM_512); i++)	//每次更新256，先进先出
        {
            bp_sample_buf[i - BP_SAMPLE_NUM_256] = bp_sample_buf[i] ;
            bp_sample_buf[i] = adc_sample_data->data_buff[i - BP_SAMPLE_NUM_256] >> 2;
        }
//        Adc_SetSendBuf(&bp_sample_buf[BP_SAMPLE_NUM_256], BP_SAMPLE_NUM_256);
        Adc_SetSendBuf(adc_sample_data->data_buff, BP_SAMPLE_NUM_256);
        bp_state = 2;
              
        Move_IdexGet(&mig_move_index);        
        Vph_BpDetection(bp_sample_buf, mig_move_index.move_5s, bp_state, bp_mode); 
    }         
}

/****************************************************************
** Function name:           Bp_BleSendTestInvalid
** Descriptions:            血压测试无效通知app
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
void Bp_BleSendTestInvalid(void)
{
    uint8_t bp_ble_buf[20] = {0x00, };    
    
    bp_ble_buf[0] = 0x90;
    bp_ble_buf[1] = 30;    
    bp_ble_buf[2] = 20;
    bp_ble_buf[3] = 0x00;    
    
    bp_ble_buf[5] = 0x01;      //表示有进度
    
    Ble_SendDataViaBat(bp_ble_buf, 20);      
}

/****************************************************************
** Function name:           Bp_BleSendTestResultValue
** Descriptions:            发送结果到APP
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
void Bp_BleSendTestResultValue(void)
{
    const uint8_t progress[20]={1,5,10,15,20,26,31,36,41,46,52,57,62,67,72,77,82,88,92,98};    
    uint8_t ble_ack[20] = {0x90,0x00,};
    bp_info_t *bp_info_temp = NULL;
    
    bp_info_temp = BP_GetInfo();
    
    if(bp_info_temp->bp_flag  == 0x01)
    {   
        ble_ack[1] = bp_info_temp->sp; 
        ble_ack[2] = bp_info_temp->dp;        
        ble_ack[3] = 100;  
    } 
    else if(bp_info_temp->bp_flag == 0x02 || bp_info_temp->bp_flag == 0x03|| bp_info_temp->valid_cnt >= 20)
    {         
        /* 新增当心律板灯灭掉时,App还在运行,此时需要终止APP */
        ble_ack[1] = 30;
        ble_ack[2] = 20;
        ble_ack[3] = 100;  
    }  
    else
    {        
        ble_ack[3] = progress[bp_info_temp->valid_cnt%20];  
    } 
    
    ble_ack[5] = 0x01;      //表示有进度
    
    Ble_SendDataViaBat(ble_ack, 20);
}

/****************************************************************
** Function name:           Bp_GetFlagState
** Descriptions:            获取测量状态
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
uint8_t Bp_GetFlagState(void)
{
    uint8_t ret = 0;
    bp_info_t *bp_info_temp;

    bp_info_temp = BP_GetInfo();
    
    if(bp_info_temp->bp_flag == 1)
    {
        ret = Bp_ResultCheck(bp_mode.test_mode, bp_info_temp->sp,bp_info_temp->dp, 60);
        if(ret == 1)
        {            
            return 1;
        }
        else
        {           
            return 2;
        }   
    }
    else if(bp_info_temp->valid_cnt >= 20)
    {
        return 2;
    }
    
    return bp_info_temp->bp_flag;   
}

/****************************************************************
** Function name:           Bp_GetTestResult
** Descriptions:            获取血压测量值
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
void Bp_GetTestResult(uint8_t *sp, uint8_t *dp)
{
    uint8_t ret = 0;
    bp_info_t *bp_info_temp;
    
    bp_info_temp = BP_GetInfo();
   
    if(bp_info_temp->bp_flag == 1)
    {
        ret = Bp_ResultCheck(bp_mode.test_mode, bp_info_temp->sp,bp_info_temp->dp, 60);
        if(ret == 1)
        {            
            *sp = bp_info_temp->sp;
            *dp = bp_info_temp->dp;
        }
        else
        {           
            *sp = 0;
            *dp = 0;
        }   
    }
    else
    {
         *sp = 0;
         *dp = 0;
    }  
}

/****************************************************************
** Function name:           Bp_ResultCheck
** Descriptions:            结果自检
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
static uint8_t Bp_ResultCheck(uint8_t check_mode, uint8_t check_sp, uint8_t check_dp, uint8_t check_heart)
{
    if(check_mode == 1)
    {
		if(check_sp < 61 || check_sp > 229 || check_dp < 26 || check_dp > 199 ||
            (check_sp <= (check_dp + BP_CALIBRATION_RESULT_DIFFERENCE)) || check_heart < 30 || check_heart > 200)
        {
            return 0;
        } 
    }
    else
    {
        if(check_sp < 81 || check_sp > 209 || check_dp < 46 || check_dp > 179 ||
            (check_sp <= (check_dp + BP_CALIBRATION_RESULT_DIFFERENCE)) || check_heart < 30 || check_heart > 200)
        {
            return 0;
        }              
    }
    return 1;
}


/****************************************************************
** Function name:           Bp_SetCalibrationCheck
** Descriptions:            设置自检
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
static uint8_t Bp_SetCalibrationCheck( uint8_t check_sp, uint8_t check_dp)
{
    if(check_sp < 81 || check_sp > 209 || check_dp < 46 || check_dp > 179 ||
        (check_sp <= (check_dp + BP_CALIBRATION_SET_DIFFERENCE)) )
    {
        return 0;
    }              
    
    return 1;
}

/****************************************************************
** Function name:           Bp_GetProgress
** Descriptions:            获取测量进度
** input parameters:          
** output parameters:
** Returned value:
** Created by:              JSH              
** Created Date:            2018-06-17
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/
uint8_t Bp_GetProgress(void)
{
    bp_info_t *bp_info_temp = NULL;
    
    bp_info_temp = BP_GetInfo();
    
    if(bp_info_temp->valid_cnt > 20)
    {
        return 100;
    }
    else
    {
        return bp_info_temp->valid_cnt * 5;
    }
}
