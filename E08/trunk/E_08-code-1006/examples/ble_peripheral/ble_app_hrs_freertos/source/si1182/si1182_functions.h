/*******************Copyright(c)*********************************
** Veepoo Tech
** COPYRIGHT (C) 2016-2019
**----------------------FileInfo---------------------------------
** File name:                  si117x_functions.h
** Last modified Date:         2017-12-22
** Last SYS_VERSION:           0.0.1
** Descriptions:
**---------------------------------------------------------------
** Createdby:                  JSH
** Createddate:                2017-12-22
** SYS_VERSION:                0.0.1
** Descriptions:               si117x的接口
**---------------------------------------------------------------
** Modified by:				
** Modified date:			
** SYS_VERSION:				
** Modified Descriptions:	
****************************************************************/


#ifndef __SI117X_FUNCTIONS_H
#define __SI117X_FUNCTIONS_H


#include "stdint.h"


/***************************************************************/
// 参数设置
typedef struct
{
    uint8_t ecg_gain;
	uint8_t ld_freq;
	uint8_t ld_current;
	uint8_t ld_gain;
	uint8_t ld_shreshold;
	uint8_t ld_im;
	uint8_t ld_enable;
	uint8_t ecg_sw_avg;
	uint8_t ecg_decim;
	uint8_t ecg_mc;
	uint8_t ld_mc;
	uint8_t mr_h;
	uint8_t mr_l;
	uint8_t param_1;
	uint8_t param_2;
	uint8_t param_3;
	uint8_t ecg_change_flag;
}si1182_ecg_param_t;

typedef struct
{
    uint8_t task_enable;
	uint8_t mr_h;
	uint8_t mr_l;
	uint8_t ppg_mc;
	uint8_t ppg1_led;
	uint8_t ppg2_led;
	uint8_t ppg3_led;
	uint8_t ppg4_led;
	uint8_t ppg1_mode;
	uint8_t ppg2_mode;
	uint8_t ppg3_mode;
	uint8_t ppg4_mode;
	uint8_t ppg1_mconfig;
	uint8_t ppg2_mconfig;
	uint8_t ppg3_mconfig;
	uint8_t ppg4_mconfig;
	uint8_t ppg1_decim;
	uint8_t ppg1_adc_range;
	uint8_t ppg1_clk_div;
	uint8_t ppg2_decim;
	uint8_t ppg2_adc_range;
	uint8_t ppg2_clk_div;
	uint8_t ppg3_decim;
	uint8_t ppg3_adc_range;
	uint8_t ppg3_clk_div;
	uint8_t ppg4_decim;
	uint8_t ppg4_adc_range;
	uint8_t ppg4_clk_div;
	uint8_t param_1;			// PPG_SW_AVG
	uint8_t param_2;
	uint8_t param_3;
	uint8_t param_4;
	uint8_t ppg_change_flag_1;
	uint8_t ppg_change_flag_2;
}si1182_ppg_param_t;


void Si1182_ParamAdjust(void);

void Si1182_EcgParamAdjust(void);

void Si1182_EcgParamGet(uint8_t *ecg_gain, uint8_t *ld_freq, uint8_t *ld_current, uint8_t *ld_gain,
						uint8_t *ld_shreshold, uint8_t *ld_im, uint8_t *ld_enable, uint8_t *ecg_sw_avg,
						uint8_t *ecg_decim, uint8_t *ecg_mc, uint8_t *ld_mc, uint8_t *mr_h,
						uint8_t *mr_l, uint8_t *param_1, uint8_t *param_2, uint8_t *param_3);

void Si1182_EcgParamSet(uint8_t ecg_gain, uint8_t ld_freq, uint8_t ld_current, uint8_t ld_gain,
						uint8_t ld_shreshold, uint8_t ld_im, uint8_t ld_enable, uint8_t ecg_sw_avg,
						uint8_t ecg_decim, uint8_t ecg_mc, uint8_t ld_mc, uint8_t mr_h,
						uint8_t mr_l, uint8_t param_1, uint8_t param_2, uint8_t param_3);

void Si1182_PpgParamAdjust(void);

void Si1182_PpgParamGet1(uint8_t *task_enable, uint8_t *mr_h, uint8_t *mr_l, uint8_t *ppg_mc,
						 uint8_t *ppg1_led, uint8_t *ppg2_led, uint8_t *ppg3_led, uint8_t *ppg4_led,
						 uint8_t *ppg1_mode, uint8_t *ppg2_mode, uint8_t *ppg3_mode, uint8_t *ppg4_mode,
						 uint8_t *ppg1_mconfig, uint8_t *ppg2_mconfig, uint8_t *ppg3_mconfig, uint8_t *ppg4_mconfig);

void Si1182_PpgParamGet2(uint8_t *ppg1_decim, uint8_t *ppg1_adc_range, uint8_t *ppg1_clk_div, uint8_t *ppg2_decim,
						 uint8_t *ppg2_adc_range, uint8_t *ppg2_clk_div, uint8_t *ppg3_decim, uint8_t *ppg3_adc_range,
						 uint8_t *ppg3_clk_div, uint8_t *ppg4_decim, uint8_t *ppg4_adc_range, uint8_t *ppg4_clk_div,
						 uint8_t *param_1, uint8_t *param_2, uint8_t *param_3, uint8_t *param_4);

void Si1182_PpgParamSet1(uint8_t task_enable, uint8_t mr_h, uint8_t mr_l, uint8_t ppg_mc,
						 uint8_t ppg1_led, uint8_t ppg2_led, uint8_t ppg3_led, uint8_t ppg4_led,
						 uint8_t ppg1_mode, uint8_t ppg2_mode, uint8_t ppg3_mode, uint8_t ppg4_mode,
						 uint8_t ppg1_mconfig, uint8_t ppg2_mconfig, uint8_t ppg3_mconfig, uint8_t ppg4_mconfig);

void Si1182_PpgParamSet2(uint8_t ppg1_decim, uint8_t ppg1_adc_range, uint8_t ppg1_clk_div, uint8_t ppg2_decim,
						 uint8_t ppg2_adc_range, uint8_t ppg2_clk_div, uint8_t ppg3_decim, uint8_t ppg3_adc_range,
						 uint8_t ppg3_clk_div, uint8_t ppg4_decim, uint8_t ppg4_adc_range, uint8_t ppg4_clk_div,
						 uint8_t param_1, uint8_t param_2, uint8_t param_3, uint8_t param_4);
	
/***************************************************************/

void Si1182_ReadParameterConfig(void);

int16_t Si117x_ParamRead(uint8_t address);

int16_t Si117x_ParamSet(uint8_t address, uint8_t value);

int16_t Si1182_ParamSet(uint32_t handle, uint8_t address, uint8_t value);

int32_t Si117x_Reset(void);

int32_t Si117xUpdateFifoCount(void);

int32_t Si117xStop(uint32_t value);

int32_t Si117xStart(void);

int32_t Si117xFlushFIFO(void);

uint32_t Si117x_GetIrqStatus(void);

uint32_t Si117xGetIrqStatus(void);

void Si1182FifoRead(uint8_t *out_data,const uint32_t fifo_byte);

int32_t Si1182_ConfigPowerOn(void);

int32_t Si1182_ConfigModeNull(void);

int32_t Si1182_ConfigModeWithEcg(void);

int32_t Si1182_ConfigModeWithGreenAmb(void);

int32_t Si1182_ConfigModeWithGreenIredAmb(void);

int32_t Si1182_ConfigModeWithGreenRedIredAmb(void);

int32_t Si1182_ConfigModeWithRedIredAmb(void);

uint8_t Si1182_ECGLeadDetection(void);

/***************************************************************/


#define REG_PART_ID         0x00
#define REG_REV_ID          0x01
#define REG_MFR_ID          0x02
#define REG_HOSTIN3         0x07
#define REG_HOSTIN2         0x08
#define REG_HOSTIN1         0x09
#define REG_HOSTIN0         0x0A
#define REG_COMMAND         0x0B
#define REG_IRQ_ENABLE      0x0F
#define REG_RESPONSE1       0x10
#define REG_RESPONSE0       0x11
#define REG_IRQ_STATUS      0x12
#define REG_HOSTOUT0        0x13
#define REG_HOSTOUT1        0x14
#define REG_HOSTOUT2        0x15
#define REG_FIFO_COUNT_H    0x16
#define REG_FIFO_COUNT_L    0x17
#define REG_IRQ_STATUS_B2   0x18

#define REG_FIFO_FLUSH      0x31
#define REG_HW_KEY          0x3A
#define REG_FIFO_READ       0x3F
//
/// @endcond

/*******************************************************************************
 ************************** Si117x Commands *******************************
 ******************************************************************************/
/// @cond DOXYGEN_SHOULD_SKIP_THIS
#define CMD_RESET_CMD_CTR         0x00
#define CMD_RESET_SW              0x01
#define CMD_SET_I2C_ADDR          0x02
#define CMD_STOP                  0x03
#define CMD_START                 0x04
#define CMD_FLUSH_FIFO            0x06
#define CMD_INSERT_SYNCH_MSG      0x09
#define CMD_RELOAD_I2C_ADDR       0x0B
#define CMD_SET_THREE_WIRE_SPI    0x0C
#define CMD_CLEAR_IRQ_STATUS      0X0D
#define CMD_GET_AUTOENABLE        0x15
#define CMD_SET_INT_OE            0x19
#define CMD_PARAM_QUERY_BYTE_MASK 0x40
#define CMD_PARAM_SET_BYTE_MASK   0x80

//
/// @endcond

/*******************************************************************************
 ************************** Si117x I2C Parameter Offsets ***********************
 ******************************************************************************/
/// @cond DOXYGEN_SHOULD_SKIP_THIS
#define PARAM_TASK_ENABLE          0x00
#define PARAM_MEASRATE_H           0x01
#define PARAM_MEASRATE_L           0x02
#define PARAM_PPG_MEASCOUNT        0x03
#define PARAM_BIOZ_MEASCOUNT       0x04
#define PARAM_ECG_MEASCOUNT        0x05
#define PARAM_FIFO_INT_LEVEL_H     0x06
#define PARAM_FIFO_INT_LEVEL_L     0x07
#define PARAM_MEAS_CNTL            0x08
#define PARAM_BIOZ_DC_VALUE        0x09
#define PARAM_INPUT_FREQ_SEL       0x0A
#define PARAM_PPG1_LED1_CONFIG     0x0B
#define PARAM_PPG1_LED2_CONFIG     0x0C
#define PARAM_PPG1_LED3_CONFIG     0x0D
#define PARAM_PPG1_LED4_CONFIG     0x0E
#define PARAM_PPG1_MODE            0x0F
#define PARAM_PPG1_MEASCONFIG      0x10
#define PARAM_PPG1_ADCCONFIG       0x11
#define PARAM_PPG2_LED1_CONFIG     0x12
#define PARAM_PPG2_LED2_CONFIG     0x13
#define PARAM_PPG2_LED3_CONFIG     0x14
#define PARAM_PPG2_LED4_CONFIG     0x15
#define PARAM_PPG2_MODE            0x16
#define PARAM_PPG2_MEASCONFIG      0x17
#define PARAM_PPG2_ADCCONFIG       0x18
#define PARAM_PPG3_LED1_CONFIG     0x19
#define PARAM_PPG3_LED2_CONFIG     0x1A
#define PARAM_PPG3_LED3_CONFIG     0x1B
#define PARAM_PPG3_LED4_CONFIG     0x1C
#define PARAM_PPG3_MODE            0x1D
#define PARAM_PPG3_MEASCONFIG      0x1E
#define PARAM_PPG3_ADCCONFIG       0x1F
#define PARAM_PPG4_LED1_CONFIG     0x20
#define PARAM_PPG4_LED2_CONFIG     0x21
#define PARAM_PPG4_LED3_CONFIG     0x22
#define PARAM_PPG4_LED4_CONFIG     0x23
#define PARAM_PPG4_MODE            0x24
#define PARAM_PPG4_MEASCONFIG      0x25
#define PARAM_PPG4_ADCCONFIG       0x26
#define PARAM_BIOZ1_CONTROL1       0x27
#define PARAM_BIOZ1_CONTROL2       0x28
#define PARAM_BIOZ1_FE_GAIN1       0x29
#define PARAM_BIOZ1_FE_GAIN2       0x2A
#define PARAM_BIOZ1_FE_MUX         0x2B
#define PARAM_BIOZ2_CONTROL1       0x2C
#define PARAM_BIOZ2_CONTROL2       0x2D
#define PARAM_BIOZ2_FE_GAIN1       0x2E
#define PARAM_BIOZ2_FE_GAIN2       0x2F
#define PARAM_BIOZ2_FE_MUX         0x30
#define PARAM_ECG_MEASCONFIG       0x31
#define PARAM_ECG_ADCCONFIG        0x32
#define PARAM_ECG_FECONFIG         0x33
#define PARAM_PPG4_THRESHOLD       0x34
#define PARAM_ECG_THRESHOLD        0x35
#define PARAM_SYNCH_CONFIG         0x36
#define PARAM_ECG_LD_MEASCOUNT     0x37
#define PARAM_ECG_LDCONFIG		   0x38

/// @endcond

/*******************************************************************************
 *******    Si117x Register and Parameter Bit Definitions  *********************
 ******************************************************************************/
/// @cond DOXYGEN_SHOULD_SKIP_THIS

#define RSP0_CHIPSTAT_MASK  		0xe0
#define RSP0_COUNTER_MASK   		0x0F
#define RSP0_SLEEP          		0x20
#define RSP0_COUNTER_ERR			0x10

#define FIFO_TEST					0x80
#define ACCEL_SYNCH					0x40

// PARAM_MEAS_CNTL
#define FIFO_DISABLE				0x40
#define PPG_SW_AVG					0x8
#define ECG_SZ						0x4
#define BIOZ_SZ						0x2
#define PPG_SZ						0x1


#define A_SYNCH_BY_FIFO				1		// 1=A SYNCH at G FIFO rate. 0=A SYNCH at G sample rate
#define OVERSAMPLING_PROVISION		1		// 1 = structure code to support ECG as well as HRM

//MEASRATE
#define FS_25HZ						800
#define MSCNT_25Hz					1

// REG_IRQ_ENABLE
#define FIFO_INT_EN					0x1
#define PPG1_INT					0x2

// REG_IRQ_STATUS
#define FIFO_IRQ					0x1
#define PPG1_IRQ					0x2
#define WD_IRQ						0x4
#define LD_IRQ             			0x8


#define PPG_OFFSET          		7   //PPG1 to PPG2 offset
#define BIOZ_OFFSET					5	// BIOZ1 to BIOZ2 offset (number of registers in a BIOZ channel setup)

// PARAM_PPG1_LED1_CONFIG
#define LED_EN						0x40
#define LED_CURRENT					0x01
#define LED_CURRENT_STEP  			0x08
#define LED_CURRENT_LEVEL	  		0x01

// PARAM_PPG4_MODE
#define HRM_MODE					  0x01

// PARAM_PPG1_MEASCONFIG
#define ADC_AVG						  0x01
#define SLAVE_EN					  0x08
#define PPG_RAW						  0x10

// PARAM_PPG4_ADCCONFIG
#define CLK_DIV						  0x01
#define ADC_RANGE					  0x10
#define DECIM						  	0x40

// PARAM_ECG_MEASCONFIG
#define ECG_ADC_AVG					0x01
#define SAMPLE_PAIR					0x08

#define ECG_LD_MODE					0x40
#define ECG_IM					  	0x80

// PARAM_ECG_ADCCONFIG
#define ECG_CLKDIV					0x01
#define ECG_SUSPEND					0x08
#define ECG_DECIM						0x40

// PARAM_ECG_LDCONFIG
#define ECG_LD_CURRENT			0x01
#define ECG_LD_MODE					0x40
#define ECG_IM							0x80

// PARAM_BIOZ_CONTROL1
#define NO_OF_SINES					0x01
#define BIOZ_CLKDIV					0x10

// PARAM_BIOZ_CONTROL2
#define CUR_AMP_CNTL				0x01
#define BIOZ_ADC_RANGE			0x20
#define DC_OVERRIDE					0x80

// PARAM_FE_GAIN_ONE
#define R1									0x01
#define R1G									0x08

// PARAM_FE_GAIN_TWO
#define R2									0x01
#define R2G									0x08

// PARAM_FE_MUX
#define IDAC_P							0x01
#define IDAC_N							0x04
#define IA_P								0x10
#define IA_N								0x40

// PARAM_ECG_FECONFIG
#define IA_GAIN							0x40

// PARAM_PPG4_THRESHOLD
#define PPG4_THRESHOLD			0x01

// Commands
#define  FLUSH_FIFO					0x0e
#define  CMD_UPDATE_FIFO_COUNT		0x0e
// PARAM_SYNCH_CONFIG
#define SYNCH_MODE					0x01

// User sample buffer size
#define ECG_BUF_SIZE	1200
#define PPG1_BUF_SIZE	1200
#define PPG2_BUF_SIZE	1200


#endif
