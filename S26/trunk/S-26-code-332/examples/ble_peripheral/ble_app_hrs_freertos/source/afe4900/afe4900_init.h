/***************************Copyright(c)***********************************
**                       Veepoo Tech
**                  COPYRIGHT (C) 2016-2019
**-----------File Info-----------------------------------------------------
** File        name:     afe4900_init.h
** Last modified Date:   2018-10-18
** Last SYS_VERSION:     0.0.0
** Descriptions:            
**-------------------------------------------------------------------------
** Created  by:          JSH
** Created  date:        2018-10-18
** SYS_VERSION:          0.0.0
** Descriptions:         
**-------------------------------------------------------------------------
** Modified by:				
** Modified date:  			          
** SYS_VERSION:                 
** Descriptions:                        
***************************************************************************/

#ifndef	__AFE4900_INIT_H
#define __AFE4900_INIT_H

#include <stdint.h>
#include "nrf_drv_gpiote.h"


#pragma pack (1)

typedef struct
{
    uint8_t current;
    uint8_t voltage;
    uint8_t gain;
    uint8_t change_flag;
}afe4900_param_t;

/*
//����1~����3��������ÿ���̶�Ϊ0.392mA����ֵΪ255ʱ����Ӧ100mA.
//����4~����7�����棺0~8�ֱ��Ӧ��500K��250K��100K��50K��25K��10K��1M��2M��1.5M ��.
//����8��DAC���̣�0/3/5/7�ֱ��Ӧ��15.875uA/��31.75uA/��63.5uA/��127uA.
//����9~����12��DACֵ��0~127��
//                      ��DAC����Ϊ0ʱ���̶�Ϊ��0.125��
//                      ��DAC����Ϊ3ʱ���̶�Ϊ��0.25��
//                      ��DAC����Ϊ5ʱ���̶�Ϊ��0.5��
//                      ��DAC����Ϊ7ʱ���̶�Ϊ��1.
***********************************************************
////EN_AMB_LOOP: 0:�رա�1��������
////CHOOSE_AMB_PHASE��0~3�Ķ�Ӧ��ϵ��0-LED2��1-AMB2��LED3��2-LED1��3-AMB1��LED4��
////HYST_LOOP��0~3�ֱ��Ӧ47/94/144/188mV��
////FREEZE_LOOP��0:�رա�1��������
////EARLY_OFFSET_DAC��0:�رա�1��������
*/
typedef struct
{
    uint8_t     set_i_led1;     //�̹����
    uint8_t     set_i_led2;     //������
    uint8_t     set_i_led3;     //�������
    
    uint8_t     set_rf1_mid;    //����1---�̹�
    uint8_t     set_rf2_mid;    //����2---���   
    uint8_t     set_rf3_mid;    //����3---�����
    uint8_t     set_rf4_mid;    //����4---������
    
	uint8_t     set_awdpr_ifs_dac;			// dac ����

	uint8_t     set_awdpr_dac_gre;			// �̹�dacֵ
	uint8_t     set_awdpr_dac_red;			// ���dacֵ
	uint8_t     set_awdpr_dac_ird;			// ����dacֵ
	uint8_t     set_awdpr_dac_amb;			// ������dacֵ

	uint8_t     set_awdpr_dac_posi_gre;		// �̹�dac����
	uint8_t     set_awdpr_dac_posi_red;		// ���dac����
	uint8_t     set_awdpr_dac_posi_ird;		// ����dac����
	uint8_t     set_awdpr_dac_posi_amb;	    // ������dac����    
    
/***************************************************************/    
    
    uint8_t     set_en_amb_loop;            // 0:�رա�1������
    uint8_t     set_choose_amb_phase;       // 0~3�Ķ�Ӧ��ϵ��0-LED2��1-AMB2��LED3��2-LED1��3-AMB1��LED4��
    uint8_t     set_hyst_loop;              // 0~3�ֱ��Ӧ47/94/144/188mV��
    uint8_t     set_freeze_loop;            // 0:�رա�1��������
    uint8_t     set_early_offset_loop;      // 0:�رա�1��������
    
    
}afe4410_ppg_param_t;

#pragma pack ()

void AFE4900_HardwarePowerDown(void);


void Afe4900_Control1Init(void);

void Afe4900_SenInit(void);

void Afe4900_ResetzInit(void);

void Afe4900_EnableHwPdn(void);

void Afe4900_DisableHwPdn(void);

void Afe4900_TriggerHwReset(void);

void Afe4900_AdcrdyInterruptInit(nrf_drv_gpiote_evt_handler_t event_handler);

void Afe4900_AdcrdyInterruptEnable(void);

void Afe4900_AdcrdyInterruptDisable(void);

void Afe4900_SpiSelInit(void);

void Afe4900_RegInit(void);

void Afe4900_PttFlag(void);

uint32_t Afe4900_IsEnablePtt(void);

// afe4900��fifo��
void Afe4900_FifoSpiRead(uint8_t address, uint8_t *data, uint32_t data_cnt);

// afe4900�ļĴ���д
void Afe4900_RegWrite(unsigned char reg_address, unsigned int data);

// afe4900�ļĴ�����
int32_t Afe4900_RegRead(unsigned char reg_address);

// afe4900 ����ת��
int32_t Afe4900_DataConvert(uint32_t data);

// afe4900�ļĴ�����ʼ����ʼ����
void Afe4900_InitStartSampling(void);

// afe4900�ļĴ�����ʼ��ֹͣ����
void Afe4900_InitStopSampling(void);

// afe4900�ļĴ�����ʼ��PPGģʽ-����
void Afe4900_RegInitPpg1(void);

// afe4900�ļĴ�����ʼ��PPGģʽ-Ѫ��
void Afe4900_RegInitPpg2(void);

void Afe4900_RegInitToRest(void);

// afe4900����LDO
void Afe4900_LdoTxOn(void);

// afe4900�ر�LDO
void Afe4900_LdoTxOff(void);


void Afe4410_RegPpgParamSet(uint8_t *data_buf, uint8_t pack_num);

void Afe4410_RegPpgParamGet(uint8_t *data_buf, uint8_t pack_num);

void Afe4410_RegPpgParamSetDo(void);

#endif

