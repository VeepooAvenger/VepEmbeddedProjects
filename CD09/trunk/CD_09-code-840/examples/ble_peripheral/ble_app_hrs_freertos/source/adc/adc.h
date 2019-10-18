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
    NULL_MODE,              //����ģʽ
    HEART_MODE,             //����ģʽ
    HEART_WEAR_MODE,        //�������ģʽ
    BP_MODE,                //Ѫѹģʽ
    BP_WEAR_MODE,           //���Ѫѹģʽ
    BATTERY_MODE,           //���ģʽ
    WEAR_DETECT_MODE,       //���ģʽ
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

//���ò���ģʽ����ʼ����
void Saadc_StartSample(ADC_MODE mode);

//ֹͣ����
void Saadc_StopSample(void);

//��ȡ����Ѫѹ��������
adc_sample_data_t *Saadc_GetSampleData(void);

//��ȡ����������
adc_sample_wear_data_t *Saadc_GetSampleWearData(void);

//��ȡ��ز�������
adc_sample_batt_data_t *Saadc_GetSampleBattData(void);

//���adc��������
void Saadc_CleanAdcDataLen(void);

//������adc�������ݣ����¿�ʼ
void Saadc_CleanBatteryAdcDataLen(void);

//�ϵ��ص������adc�ͷ�
void Saadc_BatteryUnInit(void);

//�ϵ��ص������adc��ʼ��
void Saadc_BatteryInit(void);

//��ȡ���ε�ؼ��adc�����ڿ�������
int16_t Saadc_GetBatteryAdc(void);

#endif

