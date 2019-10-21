/***************************Copyright(c)***********************************
**                       Veepoo Tech
**                  COPYRIGHT (C) 2016-2019
**-----------File Info-----------------------------------------------------
** File        name:     afe4900_calibration.h
** Last modified Date:   2018-10-19
** Last SYS_VERSION:     0.0.0
** Descriptions:            
**-------------------------------------------------------------------------
** Created  by:          JSH
** Created  date:        2018-10-19
** SYS_VERSION:          0.0.0
** Descriptions:         
**-------------------------------------------------------------------------
** Modified by:				
** Modified date:  			          
** SYS_VERSION:                 
** Descriptions:                        
***************************************************************************/

#ifndef __AFE4900_CALIBRATION_H
#define __AFE4900_CALIBRATION_H


#include "Blood_OxygenDetection.h"
#include "afe4900_calibration.h"
#include "afe4900_init.h"


void AFE4410_Reg_Write(unsigned char reg_address, unsigned int data);
signed int AFE4410_Reg_Read(unsigned char reg_address);

void Afe4404Param_ToAfe4410(bod_result_info_t * afe4404param_s);

void AFE4410_WriteDacReg(
	uint8_t awdpr_ifs_dac,			// dac ����

	uint8_t awdpr_dac_gre,			// �̹�DACֵ
	uint8_t awdpr_dac_red,			// ���DACֵ
	uint8_t awdpr_dac_ird,			// �����DACֵ
	uint8_t awdpr_dac_amb,			// ������DACֵ

	uint8_t awdpr_dac_posi_gre,		// �̹�DAC����
	uint8_t awdpr_dac_posi_red,		// ���DAC����
	uint8_t awdpr_dac_posi_ird,		// �����DAC����
	uint8_t awdpr_dac_posi_amb		// ������DAC����
);

void Tia_GainChange(uint8_t rf1_mid, uint8_t rf2_mid, uint8_t rf3_mid, uint8_t rf4_mid);

void Elec_Write(uint8_t i_led1, uint8_t i_led2, uint8_t i_led3);

void AFE4410_RfRead(uint32_t * rf_gre, uint32_t * rf_red, uint32_t * rf_ird);

void AFE4410_ElecRead(uint32_t * i_gre, uint32_t * i_red, uint32_t * i_ird);

void AFE4410_DacRead(
	int32_t * adr_dac_gre,			// �̹�dacֵ
	int32_t * adr_dac_red,			// ���dacֵ
	int32_t * adr_dac_ird,			// ����dacֵ

	uint8_t * adr_dac_posi_gre,		// �̹�dac����
	uint8_t * adr_dac_posi_red,		// ���dac����
	uint8_t * adr_dac_posi_ird		// �����dac����
);

void Elec_ModeWrite(uint8_t led_mode, uint8_t value);
void Tia_GainModeWrite(uint8_t led_mode, uint8_t value);
void Dac_ModeWrite(
	uint8_t led_mode,
	uint8_t awdpr_ifs_dac,
	uint8_t awdpr_dac,
	uint8_t awdpr_dac_posi
);

#endif

