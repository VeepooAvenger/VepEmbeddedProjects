/*******************Copyright(c)*********************************
** Veepoo Tech
** COPYRIGHT (C) 2016-2019
**----------------------FileInfo---------------------------------
** File name:                  main.c
** Last modified Date:         2017-07-10
** Last SYS_VERSION:           0.0.1
** Descriptions:
**---------------------------------------------------------------
** Createdby:                  郭涛
** Createddate:                2017-07-10
** SYS_VERSION:                0.0.1
** Descriptions:               主函数
**---------------------------------------------------------------
** Modified by:				
** Modified date:			
** SYS_VERSION:				
** Modified Descriptions:	
****************************************************************/
#include <stdint.h>
#include <string.h>
#include "algorithm_thread.h"
#include "communication_thread.h"
#include "gui_thread.h"
#include "ble_thread.h"
#include "GSensor.h"
#include "board.h"
#include "watch_dog.h"
#include "app_util_platform.h"
#include "app_type.h"
#include "nrf_gpio.h"
#include "nrf_drv_clock.h"
#include "battery_adc.h"
#include "movement_patterns.h"
#include "lcd_driver_tft.h"
#include "ext_flash.h"
#include "afe4900_driver.h"
#include "calc_data_type.h"
#include "afe4900_entry.h"

uint32_t reset_value = 0;               //复位代码
uint32_t reset_time_minutes = 0;        //复位时间分钟

/****************************************************************
** Function name:             FPU_IRQHandler
** Descriptions:              FPU中断处理函数
** input parameters:          无
** output parameters:         无
** Returned value:            无
** Created by:                郭涛
** Created Date:              2017-07-10
****************************************************************/
void FPU_IRQHandler(void)
{
    #define FPU_EXCEPTION_MASK 0x0000009F

    uint32_t *fpscr = (uint32_t *)(FPU->FPCAR+0x40);
    (void)__get_FPSCR();
    *fpscr = *fpscr & ~(FPU_EXCEPTION_MASK);
}

/****************************************************************
** Function name:             main
** Descriptions:              主函数，程序的入口
** input parameters:          无
** output parameters:         无
** Returned value:            int
** Created by:                郭涛
** Created Date:              2017-07-10
****************************************************************/
int main(void)
{
    ret_code_t err_code;
		    
    // 1.FPU中断初始化
    NVIC_SetPriority(FPU_IRQn, APP_IRQ_PRIORITY_LOW);
    NVIC_EnableIRQ(FPU_IRQn);
    
    // 2. 看门狗初始化
    App_Watch_Dog_Init();
    App_Watch_Dog_Reload();
    
    err_code = nrf_drv_clock_init();
    APP_ERROR_CHECK(err_code);
    
    reset_value = NRF_POWER->RESETREAS;//*((uint32_t *)(0x40000000+ 0x400));
    
    if(reset_value == 0)
    {
        //WJ.20180319.用于表示复位
        reset_value = 0xFE;
    }
    
    // 3. GPIO管脚初始化
    PIN_LOW(MOTOR_EN_PIN);
//    PIN_LOW(LCD_EN_PIN);
    nrf_gpio_cfg_input(CHARGING_PIN,NRF_GPIO_PIN_NOPULL);
    nrf_gpio_cfg_input(CHG_PIN,NRF_GPIO_PIN_NOPULL);
    nrf_gpio_cfg_input(VBAT_DET_PIN,NRF_GPIO_PIN_NOPULL);
    
	//SPI0--加速度 SPI1--4410 SPI2--（屏幕\Flash、字库）
    Bps_Spi0Init();
	Bps_Spi1Init();
    Bps_Spi2Init();
    
    //加速度
    GSensor_AccInit();
    //Flash验证
    Extflash_Open();
	//屏幕初始化
    LCD_DriverInit();
    // 字库初始化
    Font_Init(); 
    
    
    //4900 初始化
    Afe4900_PowerOnInit();
    
    Afe4900_OpenMode(MEASURE_MODE_REST);

    // 蓝牙协议栈线程初始化
    BleThread_Init();
    
    // 算法线程初始化
    AlgThread_Init();
    
    // 通信线程初始化
    ComThread_Init();
     
    // GUI线程初始化
    GuiThread_Init();
    
    Battery_ADC_Init(); 

    // 运动模式初始化
    Movement_RestInit();
    
    // 启动系统时间
    SystemTime_Enable();
    
    reset_time_minutes = clock->minutes;
        
    // Activate deep sleep mode
    SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;

    // Start FreeRTOS scheduler.
    vTaskStartScheduler();

    while (true)
    {
        APP_ERROR_HANDLER(NRF_ERROR_FORBIDDEN);
    }
}

