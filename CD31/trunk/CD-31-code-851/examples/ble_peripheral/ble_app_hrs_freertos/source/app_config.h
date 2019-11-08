/*****************************************************************
Copyright: 2012-2016, Veepoo Tech. Co., Ltd.
File name: 		bsp_spi.h
Description: 
Author: 			WJ
Version: 
Date: 			2016-09-14
History: 
*****************************************************************/

#ifndef USE_APP_CONFIG_H
#define USE_APP_CONFIG_H

/**************************SPI********************************************/
// <e> SPI_ENABLED - nrf_drv_spi - SPI/SPIM peripheral driver
//==========================================================
#ifndef SPI_ENABLED
#define SPI_ENABLED 1
#endif
#if  SPI_ENABLED
    #ifndef SPI_DEFAULT_CONFIG_IRQ_PRIORITY
    #define SPI_DEFAULT_CONFIG_IRQ_PRIORITY 6
    #endif

    // <e> SPI0_ENABLED - Enable SPI0 instance
    //==========================================================
    #ifndef SPI0_ENABLED
    #define SPI0_ENABLED 0
    #endif
    #if  SPI0_ENABLED
        // <q> SPI0_USE_EASY_DMA  - Use EasyDMA
        #ifndef SPI0_USE_EASY_DMA
        #define SPI0_USE_EASY_DMA 1
        #endif
    #endif //SPI0_ENABLED

    // <e> SPI1_ENABLED - Enable SPI1 instance
    //==========================================================
    #ifndef SPI1_ENABLED
    #define SPI1_ENABLED 1
    #endif
    #if  SPI1_ENABLED
        // <q> SPI1_USE_EASY_DMA  - Use EasyDMA
        #ifndef SPI1_USE_EASY_DMA
        #define SPI1_USE_EASY_DMA 1
        #endif
    #endif //SPI1_ENABLED
    // <e> SPI2_ENABLED - Enable SPI2 instance
    //==========================================================
    #ifndef SPI2_ENABLED
    #define SPI2_ENABLED 1
    #endif
    #if  SPI2_ENABLED
    // <q> SPI2_USE_EASY_DMA  - Use EasyDMA
     

    #ifndef SPI2_USE_EASY_DMA
    #define SPI2_USE_EASY_DMA 1
    #endif

    #endif //SPI2_ENABLED
    // </e>

#endif //SPI_ENABLED
    

/* ============================= config for SAADC  ============================= */
#ifndef SAADC_ENABLED
#define SAADC_ENABLED 1
#endif

/* - 0 - 8 bit
 * - 1 - 10 bit
 * - 2 - 12 bit
 * - 3 - 14 bit*/
#define SAADC_CONFIG_RESOLUTION 1

/* - 0 - Disabled
 * - 1 - 2x
 * - 2 - 4x
 * - 3 - 8x
 * - 4 - 16x
 * - 5 - 32x
 * - 6 - 64x
 * - 7 - 128x
 * - 8 - 256x*/
#define SAADC_CONFIG_OVERSAMPLE     0

//0 or 1
#define SAADC_CONFIG_LP_MODE    1   //WJ.20170417

/** @brief Interrupt priority
 * Priorities 0,2 (nRF51) and 0,1,4,5 (nRF52) are reserved for SoftDevice
 *  Following options are avaiable:
 * - 0 - 0 (highest)
 * - 1 - 1
 * - 2 - 2
 * - 3 - 3
 * - 4 - 4 (except nRF51 family)
 * - 5 - 5 (except nRF51 family)
 * - 6 - 6 (except nRF51 family)
 * - 7 - 7 (except nRF51 family)*/
#define SAADC_CONFIG_IRQ_PRIORITY   6


//use for ANCS
#ifndef BLE_ANCS_C_ENABLED
#define BLE_ANCS_C_ENABLED 1
#endif

#ifndef BLE_DB_DISCOVERY_ENABLED
#define BLE_DB_DISCOVERY_ENABLED 1
#endif

//use for PPI
#ifndef PPI_ENABLED
#define PPI_ENABLED 1
#endif

// <e> TIMER_ENABLED - nrf_drv_timer - TIMER periperal driver
//==========================================================
#ifndef TIMER_ENABLED
#define TIMER_ENABLED 1
#endif
#if  TIMER_ENABLED
// <o> TIMER_DEFAULT_CONFIG_FREQUENCY  - Timer frequency if in Timer mode
 
// <0=> 16 MHz 
// <1=> 8 MHz 
// <2=> 4 MHz 
// <3=> 2 MHz 
// <4=> 1 MHz 
// <5=> 500 kHz 
// <6=> 250 kHz 
// <7=> 125 kHz 
// <8=> 62.5 kHz 
// <9=> 31.25 kHz 

#ifndef TIMER_DEFAULT_CONFIG_FREQUENCY
#define TIMER_DEFAULT_CONFIG_FREQUENCY 0
#endif

// <o> TIMER_DEFAULT_CONFIG_MODE  - Timer mode or operation
 
// <0=> Timer 
// <1=> Counter 

#ifndef TIMER_DEFAULT_CONFIG_MODE
#define TIMER_DEFAULT_CONFIG_MODE 0
#endif

// <o> TIMER_DEFAULT_CONFIG_BIT_WIDTH  - Timer counter bit width
 
// <0=> 16 bit 
// <1=> 8 bit 
// <2=> 24 bit 
// <3=> 32 bit 

#ifndef TIMER_DEFAULT_CONFIG_BIT_WIDTH
#define TIMER_DEFAULT_CONFIG_BIT_WIDTH 0
#endif

// <o> TIMER_DEFAULT_CONFIG_IRQ_PRIORITY  - Interrupt priority
 

// <i> Priorities 0,2 (nRF51) and 0,1,4,5 (nRF52) are reserved for SoftDevice
// <0=> 0 (highest) 
// <1=> 1 
// <2=> 2 
// <3=> 3 
// <4=> 4 
// <5=> 5 
// <6=> 6 
// <7=> 7 

#ifndef TIMER_DEFAULT_CONFIG_IRQ_PRIORITY
#define TIMER_DEFAULT_CONFIG_IRQ_PRIORITY 6
#endif
//use for TIMER
#ifndef TIMER0_ENABLED
#define TIMER0_ENABLED 1
#endif

// <q> TIMER1_ENABLED  - Enable TIMER1 instance
 

#ifndef TIMER1_ENABLED
#define TIMER1_ENABLED 1
#endif

// <q> TIMER2_ENABLED  - Enable TIMER2 instance
 

#ifndef TIMER2_ENABLED
#define TIMER2_ENABLED 1
#endif

// <q> TIMER3_ENABLED  - Enable TIMER3 instance
 

#ifndef TIMER3_ENABLED
#define TIMER3_ENABLED 0
#endif

// <q> TIMER4_ENABLED  - Enable TIMER4 instance
 

#ifndef TIMER4_ENABLED
#define TIMER4_ENABLED 0
#endif

#endif //TIMER_ENABLED

/*
// <e> CLOCK_ENABLED - nrf_drv_clock - CLOCK peripheral driver
//==========================================================
#ifndef CLOCK_ENABLED
#define CLOCK_ENABLED 1
#endif
#if  CLOCK_ENABLED
// <o> CLOCK_CONFIG_XTAL_FREQ  - HF XTAL Frequency
 
// <0=> Default (64 MHz) 

#ifndef CLOCK_CONFIG_XTAL_FREQ
#define CLOCK_CONFIG_XTAL_FREQ 0
#endif

// <o> CLOCK_CONFIG_LF_SRC  - LF Clock Source
 
// <0=> RC 
// <1=> XTAL 
// <2=> Synth 


#ifndef CLOCK_CONFIG_LF_SRC
#define CLOCK_CONFIG_LF_SRC 0
#endif

// <o> CLOCK_CONFIG_IRQ_PRIORITY  - Interrupt priority
 

// <i> Priorities 0,2 (nRF51) and 0,1,4,5 (nRF52) are reserved for SoftDevice
// <0=> 0 (highest) 
// <1=> 1 
// <2=> 2 
// <3=> 3 
// <4=> 4 
// <5=> 5 
// <6=> 6 
// <7=> 7 

#ifndef CLOCK_CONFIG_IRQ_PRIORITY
#define CLOCK_CONFIG_IRQ_PRIORITY 6
#endif

#endif //CLOCK_ENABLED
// </e>

*/

// <e> RTC_ENABLED - nrf_drv_rtc - RTC peripheral driver
//==========================================================
#ifndef RTC_ENABLED
#define RTC_ENABLED 1
#endif
#if  RTC_ENABLED
// <o> RTC_DEFAULT_CONFIG_FREQUENCY - Frequency  <16-32768> 


#ifndef RTC_DEFAULT_CONFIG_FREQUENCY
#define RTC_DEFAULT_CONFIG_FREQUENCY 32768
#endif

// <q> RTC_DEFAULT_CONFIG_RELIABLE  - Ensures safe compare event triggering
 

#ifndef RTC_DEFAULT_CONFIG_RELIABLE
#define RTC_DEFAULT_CONFIG_RELIABLE 0
#endif

// <o> RTC_DEFAULT_CONFIG_IRQ_PRIORITY  - Interrupt priority
 

// <i> Priorities 0,2 (nRF51) and 0,1,4,5 (nRF52) are reserved for SoftDevice
// <0=> 0 (highest) 
// <1=> 1 
// <2=> 2 
// <3=> 3 
// <4=> 4 
// <5=> 5 
// <6=> 6 
// <7=> 7 

#ifndef RTC_DEFAULT_CONFIG_IRQ_PRIORITY
#define RTC_DEFAULT_CONFIG_IRQ_PRIORITY 6
#endif

// <q> RTC0_ENABLED  - Enable RTC0 instance
 

#ifndef RTC0_ENABLED
#define RTC0_ENABLED 0
#endif

// <q> RTC1_ENABLED  - Enable RTC1 instance
 

#ifndef RTC1_ENABLED
#define RTC1_ENABLED 0
#endif

// <q> RTC2_ENABLED  - Enable RTC2 instance
 

#ifndef RTC2_ENABLED
#define RTC2_ENABLED 1
#endif

// <o> NRF_MAXIMUM_LATENCY_US - Maximum possible time[us] in highest priority interrupt 
#ifndef NRF_MAXIMUM_LATENCY_US
#define NRF_MAXIMUM_LATENCY_US 2000
#endif

#endif //RTC_ENABLED
// </e>



// <e> WDT_ENABLED - nrf_drv_wdt - WDT peripheral driver
//==========================================================
#ifndef WDT_ENABLED
#define WDT_ENABLED 1
#endif
#if  WDT_ENABLED
// <o> WDT_CONFIG_BEHAVIOUR  - WDT behavior in CPU SLEEP or HALT mode
 
// <1=> Run in SLEEP, Pause in HALT 
// <8=> Pause in SLEEP, Run in HALT 
// <9=> Run in SLEEP and HALT 
// <0=> Pause in SLEEP and HALT 

#ifndef WDT_CONFIG_BEHAVIOUR
#define WDT_CONFIG_BEHAVIOUR 1
#endif

// <o> WDT_CONFIG_RELOAD_VALUE - Reload value  <15-4294967295> 


#ifndef WDT_CONFIG_RELOAD_VALUE
#define WDT_CONFIG_RELOAD_VALUE 2000
#endif

// <o> WDT_CONFIG_IRQ_PRIORITY  - Interrupt priority
 

// <i> Priorities 0,2 (nRF51) and 0,1,4,5 (nRF52) are reserved for SoftDevice
// <0=> 0 (highest) 
// <1=> 1 
// <2=> 2 
// <3=> 3 
// <4=> 4 
// <5=> 5 
// <6=> 6 
// <7=> 7 

#ifndef WDT_CONFIG_IRQ_PRIORITY
#define WDT_CONFIG_IRQ_PRIORITY 6
#endif

#endif //WDT_ENABLED
// </e>


// <q> APP_PWM_ENABLED  - app_pwm - PWM functionality
 

#ifndef APP_PWM_ENABLED
#define APP_PWM_ENABLED 1
#endif



#endif //USE_APP_CONFIG_H





