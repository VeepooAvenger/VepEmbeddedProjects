/*******************Copyright(c)********************************* 
 ** Veepoo Tech 
	** COPYRIGHT (C) 2016-2019 
	**----------------------FileInfo--------------------------------- 
	** File name: protocol.c 
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


//模拟开关
#define SGM_IN1_PIN     0xFF
#define SGM_IN2_PIN     0xFF

//触摸键
#define KEY_PIN			31

//侧键
#define KEY_2_PIN		11


//LCD
#define LCD_RES_PIN       9
#define LCD_CS_PIN        8
#define LCD_DC_PIN		  7                                       
#define LCD_EN_PIN        10
#define LCD_SCLK_PIN	  6
#define LCD_SI_PIN        5
#define LCD_SO_PIN        0xFF

// UI Flash
#define FLASH_CS_PIN        19
#define FLASH_SO_PIN        17
#define FLASH_SI_PIN        15
#define FLASH_SCLK_PIN      16

// 数据存储 Flash
#define FLASH_DATA_CS_PIN        18
#define FLASH_DATA_SO_PIN        FLASH_SO_PIN//13
#define FLASH_DATA_SI_PIN        FLASH_SI_PIN//14
#define FLASH_DATA_SCLK_PIN      FLASH_SCLK_PIN//15

//加速度
#define G_INIT1_PIN		3
#define G_CS_PIN		2
#define G_SCLK_PIN		FLASH_DATA_SCLK_PIN
#define G_SI_PIN		FLASH_DATA_SI_PIN
#define G_SO_PIN		FLASH_DATA_SO_PIN

//Motor
#define MOTOR_EN_PIN        12

//电池管理
#define CHARGING_PIN        14    //充电中  PPR
#define CHG_PIN             13   //充电完成 
#define VBAT_DET_PIN        4

//LDO
#define LDO_EN_TX_PIN       20
#define LDO_TX_INIT()       do{ nrf_gpio_cfg_output(LDO_EN_TX_PIN); nrf_gpio_pin_clear(LDO_EN_TX_PIN);}while(0)
#define LDO_TX_ON()         do{ nrf_gpio_cfg_output(LDO_EN_TX_PIN); nrf_gpio_pin_set(LDO_EN_TX_PIN);}while(0)
#define LDO_TX_OFF()        do{ nrf_gpio_cfg_output(LDO_EN_TX_PIN); nrf_gpio_pin_clear(LDO_EN_TX_PIN);}while(0)

#define SI1182_POWER_LDO_EN       23


#define SI1182_MS			24
#define SI1182_INT			25

#define SI1182_CS			29 
#define SI1182_SCLK			26
#define SI1182_SO			28
#define SI1182_SI			27

#define PIN_INIT(n)        do{nrf_gpio_cfg_output(n);nrf_gpio_pin_clear(n);}while(0)
#define PIN_HIGH(n)        do{nrf_gpio_cfg_output(n);nrf_gpio_pin_set(n);}while(0)
#define PIN_LOW(n)        do{nrf_gpio_cfg_output(n);nrf_gpio_pin_clear(n);}while(0)

#define PIN_HIGH_RESISTANCE(n)  NRF_GPIO->PIN_CNF[n]=GPIO_PIN_CNF_INPUT_Disconnect<<GPIO_PIN_CNF_INPUT_Pos;



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
