/*******************Copyright(c)********************************* 
 ** Veepoo Tech 
	** COPYRIGHT (C) 2016-2019 
	**----------------------FileInfo--------------------------------- 
	** File name: board.h 
	** Last modified Date: 2017-xx-xx 
	** Last SYS_VERSION: 0.0.0 
	** Descriptions: 
	**--------------------------------------------------------------- 
	** Createdby: xxx 
	** Createddate: 2017-xx-xx 
	** SYS_VERSION: 0.0.0 
	** Descriptions: Communication protocol with app 
	**--------------------------------------------------------------- 
	** Modified by: 
	** Modified date: 
	** SYS_VERSION: 
	** Modified Descriptions: ****************************************************************/
#ifndef _BOARD_H_
#define _BOARD_H_

#include "nrf_gpio.h"

//按键
#define KEY_PIN         20
#define KEY2_PIN        0xFF

//加速度
#define G_INIT1_PIN    3     //wake
#define G_INIT2_PIN    4    //data
#define G_CS_PIN       5
#define G_SO_PIN       6
#define G_SI_PIN       7
#define G_SCLK_PIN     8
//LCD
#define LCD_RES_PIN       17
#define LCD_CS_PIN        9
#define LCD_DC_PIN		  18
#define LCD_EN_PIN        19
#define LCD_SCLK_PIN	  14
#define LCD_SI_PIN        13
#define LCD_SO_PIN        0xFF

//Flash
#define FLASH_CS_PIN        10
#define FLASH_SO_PIN        15
#define FLASH_SI_PIN        13
#define FLASH_SCLK_PIN      14

//Motor
#define MOTOR_EN_PIN        11

//字库
#define GT24_SI_PIN         FLASH_SI_PIN
#define GT24_SCLK_PIN       FLASH_SCLK_PIN
#define GT24_SO_PIN         FLASH_SO_PIN
#define GT24_CS_PIN         16

//电池管理
#define CHARGING_PIN        22    //充电中  PPR
#define CHG_PIN             21   //充电完成 
#define VBAT_DET_PIN        2

//LDO
#define LDO_EN_TX_PIN       23
#define LDO_TX_INIT()       do{ nrf_gpio_cfg_output(LDO_EN_TX_PIN); nrf_gpio_pin_clear(LDO_EN_TX_PIN);}while(0)
#define LDO_TX_ON()         do{ nrf_gpio_cfg_output(LDO_EN_TX_PIN); nrf_gpio_pin_set(LDO_EN_TX_PIN);}while(0)
#define LDO_TX_OFF()        do{ nrf_gpio_cfg_output(LDO_EN_TX_PIN); nrf_gpio_pin_clear(LDO_EN_TX_PIN);}while(0)

// AFE4900
#define AFE_RESETZ			29
#define AFE_ADC_RDY			25
#define AFE_PROG_OUT1		31


#define AFE_SEN				27 
#define AFE_SCLK			26
#define AFE_SO				30
#define AFE_SI				28



#define PIN_INIT(n)        do{nrf_gpio_cfg_output(n);nrf_gpio_pin_clear(n);}while(0)
#define PIN_HIGH(n)        do{nrf_gpio_cfg_output(n);nrf_gpio_pin_set(n);}while(0)
#define PIN_LOW(n)        do{nrf_gpio_cfg_output(n);nrf_gpio_pin_clear(n);}while(0)

#define PIN_HIGH_RESISTANCE(n)  NRF_GPIO->PIN_CNF[n]=GPIO_PIN_CNF_INPUT_Disconnect<<GPIO_PIN_CNF_INPUT_Pos;


//未使用，只是为了编译

#define GC1_PIN     0xFF



    // Low frequency clock source to be used by the SoftDevice
#define NRF_CLOCK_LFCLKSRC      {.source        = NRF_CLOCK_LF_SRC_XTAL,            \
                                 .rc_ctiv       = 0,                                \
                                 .rc_temp_ctiv  = 0,                                \
                                 .xtal_accuracy = NRF_CLOCK_LF_XTAL_ACCURACY_20_PPM}

    // Low frequency clock source to be used by the SoftDevice
																																	
//#define NRF_CLOCK_LFCLKSRC      {.source        = NRF_CLOCK_LF_SRC_RC,              \
//                                 .rc_ctiv       = 16,                                \
//                                 .rc_temp_ctiv  = 2,                                \
//                                 .xtal_accuracy = NRF_CLOCK_LF_XTAL_ACCURACY_250_PPM}
#endif
