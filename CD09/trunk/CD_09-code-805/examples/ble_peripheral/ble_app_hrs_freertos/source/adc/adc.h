/***************************Copyright(c)***********************************
**                       Veepoo Tech
**                  COPYRIGHT (C) 2016-2019
**-----------File Info-----------------------------------------------------
** File        name:     adc.h
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
#ifndef HEART_ADC_H__
#define HEART_ADC_H__

#include "stdint.h"

#define APP_ADC_ENABLED     1


#define SAMPLE_HEART_SIZE               200
#define SAMPLE_BP_SIZE                  256
#define SAMPLE_WEAR_DETECT_SIZE         40//20
#define SAMPLE_BATT_DETECT_SIZE         50

#define SAMPLE_HEART_PEROID             5//20
#define SAMPLE_BP_PEROID                5
#define SAMPLE_WEAR_DETECT_PEROID       5
#define SAMPLE_BATT_DETECT_PEROID       5


typedef enum{
    NULL_MODE,              //空闲模式
    HEART_MODE,             //心率模式
    HEART_WEAR_MODE,        //心率佩戴模式
    BP_MODE,                //血压模式
    BP_WEAR_MODE,           //佩戴血压模式
    BATTERY_MODE,           //电池模式
    WEAR_DETECT_MODE,       //佩戴模式
}ADC_MODE;

#pragma pack(1)
typedef struct{
    uint8_t  mode;
    uint16_t data_len;
    int16_t data_buff[256 + 2];
}adc_sample_data_t;

typedef struct{
    uint8_t  mode;
    uint16_t data_len;
    int16_t  data_buff[SAMPLE_WEAR_DETECT_SIZE + 2];    
    int16_t  data_buff1[SAMPLE_WEAR_DETECT_SIZE + 2];
}adc_sample_wear_data_t;

typedef struct{
    uint8_t  mode;
    uint16_t data_len;
    int16_t  data_buff[SAMPLE_BATT_DETECT_SIZE + 2];
}adc_sample_batt_data_t;
#pragma pack()

//设置采样模式并开始采样
void Saadc_StartSample(ADC_MODE mode);

//停止采样
void Saadc_StopSample(void);

//获取心率血压采样数据
adc_sample_data_t *Saadc_GetSampleData(void);

//获取佩戴检测数据
adc_sample_wear_data_t *Saadc_GetSampleWearData(void);

//获取电池采样数据
adc_sample_batt_data_t *Saadc_GetSampleBattData(void);

//清除adc采样数据
void Saadc_CleanAdcDataLen(void);

//清除电池adc采样数据，重新开始
void Saadc_CleanBatteryAdcDataLen(void);

//上电电池电量检测adc释放
void Saadc_BatteryUnInit(void);

//上电电池电量检测adc初始化
void Saadc_BatteryInit(void);

//获取单次电池检测adc，用于开机启动
int16_t Saadc_GetBatteryAdc(void);

#endif

