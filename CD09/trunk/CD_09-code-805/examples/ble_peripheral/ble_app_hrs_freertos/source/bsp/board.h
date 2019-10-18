/* Copyright (c) 2014 Nordic Semiconductor. All Rights Reserved.
*
* The information contained herein is property of Nordic Semiconductor ASA.
* Terms and conditions of usage are described in detail in NORDIC
* SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
*
* Licensees are granted free, non-transferable use of the information. NO
* WARRANTY of ANY KIND is provided. This heading must NOT be removed from
* the file.
*
*/

#ifndef BOARD_H
#define BOARD_H

#include "nrf_gpio.h"

#define KEY_PIN        25

//加速度KX023
#define G_INIT1_PIN    0xFF//31
#define G_INIT2_PIN    0xFF//30
#define G_CS_PIN       0xFF//29
#define G_SO_PIN       0xFF//28
#define G_SI_PIN       0xFF//27
#define G_SCLK_PIN     0xFF//26
#define G_TRIG_PIN     0xFF


//加速度
#define G_SCL_PIN			18
#define G_SDA_PIN			17
#define G_INT_PIN			12


#define FLASH_CS_PIN        20
#define FLASH_SO_PIN        21
#define FLASH_SI_PIN        23
#define FLASH_SCLK_PIN      22

//LCD
#define LCD_RES_PIN			4
#define LCD_SCLK_PIN		6
#define LCD_SDIN_PIN        7
#define LCD_CS_PIN          3
#define LCD_DC_PIN			5
#define LCD_EN_PIN			2
#define LCD_SDOUT_PIN       0xFF

//LDO
#define LDO_EN_PIN			8//4

//Motor
#define MOTOR_EN_PIN        26//14

//字库
#define GT24_SI_PIN			0xFF    
#define GT24_SCLK_PIN   	0xFF    
#define GT24_SO_PIN			0xFF
#define GT24_CS_PIN			0xFF

//ADC
#define T_ADC_PIN		    29   //佩戴
#define S_ADC_PIN           30   //血压心率
#define VBAT_DET_PIN        31   //电池检测

//充电管理
#define CHARGING_PIN        28      //充电中  PPR
#define CHG_PIN             27     //充电完成 

//
#define GC1_PIN             11
#define GC2_PIN             10
#define GC3_PIN             9


#define PIN_HIGH(n)	    do{nrf_gpio_cfg_output(n);nrf_gpio_pin_set(n);}while(0)
#define PIN_LOW(n)		do{nrf_gpio_cfg_output(n);nrf_gpio_pin_clear(n);}while(0)

#define PIN_HIGH_RESISTANCE(n)  NRF_GPIO->PIN_CNF[n]=GPIO_PIN_CNF_INPUT_Disconnect<<GPIO_PIN_CNF_INPUT_Pos;


#define PIN_READ_OUTPUT_STATE(pin_number) (NRF_GPIO->OUTSET & (1UL << pin_number))



    // Low frequency clock source to be used by the SoftDevice
#define NRF_CLOCK_LFCLKSRC      {.source        = NRF_CLOCK_LF_SRC_XTAL,            \
                                 .rc_ctiv       = 0,                                \
                                 .rc_temp_ctiv  = 0,                                \
                                 .xtal_accuracy = NRF_CLOCK_LF_XTAL_ACCURACY_20_PPM}

    //Low frequency clock source to be used by the SoftDevice
#define NRF_CLOCK_LFCLKSRC1      {.source        = NRF_CLOCK_LF_SRC_RC,              \
                                 .rc_ctiv       = 16,                                \
                                 .rc_temp_ctiv  = 2,                                \
                                 .xtal_accuracy = NRF_CLOCK_LF_XTAL_ACCURACY_250_PPM}
                                 
#endif	//BOARD_H

