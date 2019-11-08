/***************************Copyright(c)***********************************
**                       Veepoo Tech
**                  COPYRIGHT (C) 2016-2019
**-----------File Info-----------------------------------------------------
** File        name:     adc_52832.c
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
#include "nrf_drv_saadc.h"
#include "nrf_drv_ppi.h"
#include "nrf_drv_timer.h"
#include "adc.h"
#include "board.h"

#include "app_task.h"
#include "app_handler.h"
#include "bracelet_protocol.h"

#if APP_ADC_ENABLED

/*********************************************************************
* Macro Definition
*/
#define MAX_SAMPLES_IN_BUFFER   3

#define ADC_PIN         S_ADC_PIN       //脉搏
#define ADC_DET_PIN     T_ADC_PIN       //佩戴
#define ADC_BATT_PIN    VBAT_DET_PIN    //电池
/*********************************************************************
* Global Variables
*/
static const nrf_drv_timer_t m_timer = NRF_DRV_TIMER_INSTANCE(1);
static nrf_saadc_value_t    m_buffer_pool[2][MAX_SAMPLES_IN_BUFFER];
static nrf_ppi_channel_t    m_ppi_channel;

static uint8_t adc_ppi_init_flag  = 0;


static adc_sample_data_t adc_data;
static adc_sample_data_t adc_sample_data;

static uint16_t curr_sample_size = 0;       //当前采样大小
static uint16_t curr_sample_peroid = 0;     //采样间隔

//佩戴
static adc_sample_wear_data_t adc_wear_data;
static adc_sample_wear_data_t adc_sample_wear_data;

//电池
static adc_sample_batt_data_t adc_batt_data;
static adc_sample_batt_data_t adc_sample_batt_data;

static uint8_t filter_flag = 1;
static uint16_t filter_cnt = 0;


//static uint8_t ble_send_buf[40] = {0,};
//static uint8_t ble_send_cnt = 0;

/****************************************************************
** Function name:           ReadCurHeart
** Descriptions:            read current heart
** input parameters:
** output parameters:
** Returned value:
** Created by:              SQH              
** Created Date:            2016-10-19
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
static void Saadc_Callback(nrf_drv_saadc_evt_t const * p_event)
{
    ret_code_t err_code = 0;
    int16_t adc_temp_data = 0;
    
    if(p_event->type == NRF_DRV_SAADC_EVT_DONE)
    {   
        err_code = nrf_drv_saadc_buffer_convert(p_event->data.done.p_buffer, MAX_SAMPLES_IN_BUFFER);
        APP_ERROR_CHECK(err_code);        
        
        switch(adc_data.mode)
        {
            case HEART_MODE:
            case BP_MODE:
            case HEART_WEAR_MODE:
            case BP_WEAR_MODE:    
                if(filter_flag)
                {
                    filter_cnt++;
                    if(filter_cnt > 50)
                    {
                        filter_flag = 0;
                    }
                }
                else
                {     
                    //血压心率                    
                    adc_temp_data = adc_data.data_buff[adc_data.data_len++] = p_event->data.done.p_buffer[0] ;   
                    if(adc_data.data_len >= curr_sample_size)                
                    {
                        memcpy(adc_sample_data.data_buff, adc_data.data_buff, curr_sample_size*2);
                        adc_sample_data.data_len = curr_sample_size;
                        adc_sample_data.mode = adc_data.mode;
                        adc_data.data_len = 0; 
                        App_AddEvent(APP_ADC_SAMPLING_COMPLETED_EVENT);        //send sample complete event
                    }
                    //加入佩戴确认数据
                    adc_wear_data.data_buff1[adc_wear_data.data_len] = adc_temp_data;
                }                
            case BATTERY_MODE:           
                //电池ADC
                if(adc_batt_data.data_len < SAMPLE_BATT_DETECT_SIZE)
                {
                    adc_batt_data.data_buff[adc_batt_data.data_len++] = p_event->data.done.p_buffer[2];
                    if(adc_batt_data.data_len >= SAMPLE_BATT_DETECT_SIZE)   //WJ.20170327.数组越界
                    { 
                        memcpy(adc_sample_batt_data.data_buff, adc_batt_data.data_buff, SAMPLE_BATT_DETECT_SIZE * 2);
                        adc_sample_batt_data.mode = BATTERY_MODE;
                        adc_sample_batt_data.data_len = SAMPLE_BATT_DETECT_SIZE;  
//                        adc_batt_data.data_len = 0;                    
                        if(adc_data.mode == BATTERY_MODE)
                        {
                            App_AddEvent(APP_ADC_SAMPLING_COMPLETED_EVENT);
                            break;
                        }
                    }    
                }                
            case WEAR_DETECT_MODE:
                if(adc_data.mode != HEART_WEAR_MODE && adc_data.mode != BP_WEAR_MODE)
                {
                    break;
                }
                //佩戴ADC
                adc_wear_data.data_buff[adc_wear_data.data_len++] = p_event->data.done.p_buffer[1];
                if(adc_wear_data.data_len >= SAMPLE_WEAR_DETECT_SIZE)
                {            
                    memcpy(adc_sample_wear_data.data_buff, adc_wear_data.data_buff, SAMPLE_WEAR_DETECT_SIZE * 2);
                    memcpy(adc_sample_wear_data.data_buff1, adc_wear_data.data_buff1, SAMPLE_WEAR_DETECT_SIZE * 2);
                    adc_sample_wear_data.mode = adc_wear_data.mode;
                    adc_sample_wear_data.data_len = SAMPLE_WEAR_DETECT_SIZE;
                    adc_wear_data.data_len = 0;  
                    //加入佩戴计算事件 
                    App_AddEvent(APP_WEAR_DECTED_EVENT);        //
                }                   
                break;
            default:
                break;        
        }   
    }
}

/****************************************************************
** Function name:           timer_handler
** Descriptions:            timer handler, not used
** input parameters:
** output parameters:
** Returned value:
** Created by:              SQH              
** Created Date:            2016-11-04
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void timer_handler(nrf_timer_event_t event_type, void * p_context)
{

}

/****************************************************************
** Function name:           Saadc_SamplingParameterSettings
** Descriptions:            设置采样模式参数
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
static uint8_t Saadc_SamplingParameterSettings(ADC_MODE mode)
{
    if(mode == HEART_MODE|| mode == HEART_WEAR_MODE)
    {
        curr_sample_size = SAMPLE_HEART_SIZE;
        curr_sample_peroid = SAMPLE_HEART_PEROID;
        adc_data.mode = mode;
        adc_data.data_len = 0;
        if(mode == HEART_WEAR_MODE)
        {
            adc_wear_data.mode = HEART_WEAR_MODE;
            adc_wear_data.data_len = 0;
        }      
    }
    else if(mode == BP_MODE || mode == BP_WEAR_MODE)
    {
        curr_sample_size = SAMPLE_BP_SIZE;
        curr_sample_peroid = SAMPLE_BP_PEROID;
        adc_data.mode = mode;
        adc_data.data_len = 0;
        if(mode == BP_WEAR_MODE)
        {
            adc_wear_data.mode = BP_WEAR_MODE;            
            adc_wear_data.data_len = 0;
        }
    }
    else if(mode == WEAR_DETECT_MODE)
    { 
        curr_sample_size = SAMPLE_WEAR_DETECT_SIZE;
        curr_sample_peroid = SAMPLE_WEAR_DETECT_PEROID;
          
        adc_data.mode = WEAR_DETECT_MODE;
        adc_data.data_len = 0;    
        
        adc_wear_data.mode = WEAR_DETECT_MODE;
        adc_wear_data.data_len = 0;
        
    }   
    else if(mode == BATTERY_MODE)
    { 
        curr_sample_size = SAMPLE_BATT_DETECT_SIZE;
        curr_sample_peroid = SAMPLE_BATT_DETECT_PEROID; 
        adc_data.mode = BATTERY_MODE;
        adc_data.data_len = 0;   

        adc_batt_data.mode = BATTERY_MODE;        
        adc_batt_data.data_len = 0;
    }
    else
    {
        curr_sample_size = 0;
        curr_sample_peroid = 0;
        return 0;
    }   

    return 1;
}

/****************************************************************
** Function name:           Saadc_Init
** Descriptions:            initialize saadc config
** input parameters:
** output parameters:
** Returned value:
** Created by:              SQH              
** Created Date:            2016-11-04
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Saadc_Init(void)
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
   
	//心率血压采集    
    //internal ref: 0.6v; input gain 1/6; single ended mode
    nrf_saadc_channel_config_t channel_config =
    NRF_DRV_SAADC_DEFAULT_CHANNEL_CONFIG_SE(nrf_drv_saadc_gpio_to_ain(ADC_PIN));    //ADC_DET_PIN  ADC_PIN
//    channel_config.resistor_p = NRF_SAADC_RESISTOR_PULLUP;
    channel_config.reference = NRF_SAADC_REFERENCE_VDD4;
    channel_config.gain = NRF_SAADC_GAIN1_4;
    channel_config.acq_time = NRF_SAADC_ACQTIME_40US;
    //initialization SAADC channel, !!!the number should change when change pin number!!!
    err_code = nrf_drv_saadc_channel_init(0, &channel_config);      
    APP_ERROR_CHECK(err_code);      

	//佩戴检测    
    //internal ref: 0.6v; input gain 1/6; single ended mode
    nrf_saadc_channel_config_t wear_detect_channel_config =
    NRF_DRV_SAADC_DEFAULT_CHANNEL_CONFIG_SE(nrf_drv_saadc_gpio_to_ain(ADC_DET_PIN));    //ADC_DET_PIN  ADC_PIN
    
    wear_detect_channel_config.acq_time = NRF_SAADC_ACQTIME_40US;

     //initialization SAADC channel, !!!the number should change when change pin number!!!
    err_code = nrf_drv_saadc_channel_init(1, &wear_detect_channel_config);      
    APP_ERROR_CHECK(err_code);  
    
	//电池检测
    //internal ref: 0.6v; input gain 1/6; single ended mode
    nrf_saadc_channel_config_t batt_detect_channel_config =
    NRF_DRV_SAADC_DEFAULT_CHANNEL_CONFIG_SE(nrf_drv_saadc_gpio_to_ain(ADC_BATT_PIN));    //ADC_DET_PIN  ADC_PIN
    
    batt_detect_channel_config.acq_time = NRF_SAADC_ACQTIME_40US;
    batt_detect_channel_config.gain = NRF_SAADC_GAIN1_2;
     //initialization SAADC channel, !!!the number should change when change pin number!!!
    err_code = nrf_drv_saadc_channel_init(2, &batt_detect_channel_config);      
    APP_ERROR_CHECK(err_code);  


    err_code = nrf_drv_saadc_buffer_convert(m_buffer_pool[0], MAX_SAMPLES_IN_BUFFER);
    err_code = nrf_drv_saadc_buffer_convert(m_buffer_pool[1], MAX_SAMPLES_IN_BUFFER);
    APP_ERROR_CHECK(err_code);
    
    adc_ppi_init_flag = 1;
    
}

/****************************************************************
** Function name:           Saadc_Sampling_Event_Enable
** Descriptions:            enable sample event
** input parameters:
** output parameters:
** Returned value:
** Created by:              SQH              
** Created Date:            2016-11-04
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/
void Saadc_SamplingEventEnable(void)
{  
    nrf_drv_timer_enable(&m_timer);
    ret_code_t err_code = nrf_drv_ppi_channel_enable(m_ppi_channel);

    APP_ERROR_CHECK(err_code);
}

/****************************************************************
** Function name:           Saadc_Sampling_Event_Init
** Descriptions:            sample event initialize
** input parameters:        sampling peroid, unit: ms
** output parameters:
** Returned value:
** Created by:              SQH              
** Created Date:            2016-11-04
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/
void Saadc_SamplingEventInit(uint32_t sample_peroid)
{
    ret_code_t err_code;

    err_code = nrf_drv_ppi_init();
//    APP_ERROR_CHECK(err_code);

    nrf_drv_timer_config_t timer_cfg = NRF_DRV_TIMER_DEFAULT_CONFIG;
    timer_cfg.bit_width = NRF_TIMER_BIT_WIDTH_32;
    err_code = nrf_drv_timer_init(&m_timer, &timer_cfg, timer_handler);
    APP_ERROR_CHECK(err_code);

    /* setup m_timer for compare event every ms */
    uint32_t ticks = nrf_drv_timer_ms_to_ticks(&m_timer, sample_peroid);
    nrf_drv_timer_extended_compare(&m_timer,
                                   NRF_TIMER_CC_CHANNEL0,
                                   ticks,
                                   NRF_TIMER_SHORT_COMPARE0_CLEAR_MASK,
                                   false);
//    nrf_drv_timer_enable(&m_timer);

    uint32_t timer_compare_event_addr = nrf_drv_timer_compare_event_address_get(&m_timer,
                                                                                NRF_TIMER_CC_CHANNEL0);
    uint32_t saadc_sample_task_addr   = nrf_drv_saadc_sample_task_get();

    /* setup ppi channel so that timer compare event is triggering sample task in SAADC */
    err_code = nrf_drv_ppi_channel_alloc(&m_ppi_channel);
    APP_ERROR_CHECK(err_code);

    err_code = nrf_drv_ppi_channel_assign(m_ppi_channel,
                                          timer_compare_event_addr,
                                          saadc_sample_task_addr);
    APP_ERROR_CHECK(err_code);
}

/****************************************************************
** Function name:           Saadc_Start_Sample
** Descriptions:            start sample adc value
** input parameters:        mode : 1 bp sample; 2 heart sample   
** output parameters:
** Returned value:
** Created by:              SQH              
** Created Date:            2016-11-04
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/
void Saadc_StartSample(ADC_MODE mode)
{  
    static uint8_t init_flag = 0;
    
    if(!init_flag)
    {
        init_flag = 1;        
        Saadc_SamplingEventInit(5);
    }
    
    if(adc_ppi_init_flag)
    {
        Saadc_StopSample(); //释放
    }

    memset(&adc_sample_data, 0x00, sizeof(adc_sample_data_t));
    memset(&adc_sample_wear_data, 0x00, sizeof(adc_sample_wear_data_t));
    memset(&adc_batt_data, 0x00, sizeof(adc_sample_batt_data_t));
    
    if(!Saadc_SamplingParameterSettings(mode) || curr_sample_peroid == 0)
    {
        return;
    }   

    Saadc_CleanAdcDataLen();    
    
    if(adc_ppi_init_flag != 1)
    {    
        Saadc_Init();
    }    
    
    Saadc_SamplingEventEnable();
    
    adc_sample_data.mode = mode;  
}

/****************************************************************
** Function name:           Saadc_Stop_Sample
** Descriptions:            stop sample
** input parameters:          
** output parameters:
** Returned value:
** Created by:              SQH              
** Created Date:            2016-11-04
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/
void Saadc_StopSample(void)
{
    if(adc_ppi_init_flag)
    {
        nrf_drv_ppi_channel_disable(m_ppi_channel);
        nrf_drv_timer_disable(&m_timer);

        nrf_drv_saadc_uninit();        
        NRF_SAADC->INTENCLR = (SAADC_INTENCLR_END_Clear << SAADC_INTENCLR_END_Pos);
        NVIC_ClearPendingIRQ(SAADC_IRQn);         
        adc_ppi_init_flag = 0;
    }
}

/****************************************************************
** Function name:           Saad_GetSampleData
** Descriptions:            获取心率血压采样数据
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
adc_sample_data_t *Saad_GetSampleData(void)
{
    return &adc_sample_data;
}

/****************************************************************
** Function name:           Saadc_GetSampleWearData
** Descriptions:            获取佩戴检测数据
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
adc_sample_wear_data_t *Saadc_GetSampleWearData(void)
{
    return &adc_sample_wear_data;
}

/****************************************************************
** Function name:           Saadc_GetSampleBattData
** Descriptions:            获取电池采样数据
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
adc_sample_batt_data_t *Saadc_GetSampleBattData(void)
{
    return &adc_sample_batt_data;
}

/****************************************************************
** Function name:           Saadc_CleanAdcDataLen
** Descriptions:            清除adc采样数据，重新开始
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
void Saadc_CleanAdcDataLen(void)
{
    adc_data.data_len = 0;    
    filter_flag = 1;
    filter_cnt = 0;
}


/****************************************************************
** Function name:           Saadc_CleanBatteryAdcDataLen
** Descriptions:            清除电池adc采样数据，重新开始
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
void Saadc_CleanBatteryAdcDataLen(void)
{
    adc_batt_data.data_len = 0;
}


/****************************************************************
** Function name:           Saadc_BatteryInit
** Descriptions:            上电电池电量检测adc初始化
** input parameters:        无  
** output parameters:       无
** Returned value:          返回当前一次adc采样值
** Created by:              WJ              
** Created Date:            2018-08-29
*****************************************************************/
void Saadc_BatteryInit(void)
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
  
	//电池检测
    //internal ref: 0.6v; input gain 1/6; single ended mode
    nrf_saadc_channel_config_t batt_detect_channel_config =
    NRF_DRV_SAADC_DEFAULT_CHANNEL_CONFIG_SE(nrf_drv_saadc_gpio_to_ain(ADC_BATT_PIN));    //ADC_DET_PIN  ADC_PIN
    
    //WJ.20180912.增加采样延时，确保采样精准
    batt_detect_channel_config.acq_time = NRF_SAADC_ACQTIME_40US;
    
    batt_detect_channel_config.gain = NRF_SAADC_GAIN1_2;
     //initialization SAADC channel, !!!the number should change when change pin number!!!
    err_code = nrf_drv_saadc_channel_init(2, &batt_detect_channel_config);      
    APP_ERROR_CHECK(err_code);    
}

/****************************************************************
** Function name:           Saadc_BatteryUnInit
** Descriptions:            上电电池电量检测adc释放
** input parameters:        无  
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2018-08-29
*****************************************************************/
void Saadc_BatteryUnInit(void)
{
    nrf_drv_saadc_uninit();        
    NRF_SAADC->INTENCLR = (SAADC_INTENCLR_END_Clear << SAADC_INTENCLR_END_Pos);
    NVIC_ClearPendingIRQ(SAADC_IRQn); 
}

/****************************************************************
** Function name:           Saadc_CleanBatteryAdcDataLen
** Descriptions:            清除电池adc采样数据，重新开始
** input parameters:        无  
** output parameters:       无
** Returned value:          返回当前一次adc采样值
** Created by:              WJ              
** Created Date:            2018-08-29
*****************************************************************/
int16_t Saadc_GetBatteryAdc(void)
{
    int16_t battery_adc = 0;
    
    nrf_drv_saadc_sample_convert(2, (nrf_saadc_value_t *)&battery_adc);
    
    if(battery_adc > 4096 || battery_adc < 0)
    {
        return 0;
    }
    
    return battery_adc;
}





#endif //APP_ADC_ENABLED



