/*******************Copyright(c)*********************************
** Veepoo Tech
** COPYRIGHT (C) 2016-2019
**----------------------FileInfo---------------------------------
** File name:                  main.c
** Last modified Date:         2017-07-10
** Last SYS_VERSION:           0.0.1
** Descriptions:
**---------------------------------------------------------------
** Createdby:                  ����
** Createddate:                2017-07-10
** SYS_VERSION:                0.0.1
** Descriptions:               ������
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
#include "GSensor.h"
#include "si1182_driver.h"
#include "bsp_btn_ble.h"
#include "test_state_machine.h"

uint32_t reset_value = 0;               //��λ����
uint32_t reset_time_minutes = 0;        //��λʱ�����

/****************************************************************
** Function name:             FPU_IRQHandler
** Descriptions:              FPU�жϴ�����
** input parameters:          ��
** output parameters:         ��
** Returned value:            ��
** Created by:                ����
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
** Descriptions:              ����������������
** input parameters:          ��
** output parameters:         ��
** Returned value:            int
** Created by:                ����
** Created Date:              2017-07-10
****************************************************************/
int main(void)
{
    ret_code_t err_code;

    err_code = nrf_drv_clock_init();
    APP_ERROR_CHECK(err_code);
    
    // 1.FPU�жϳ�ʼ��
    NVIC_SetPriority(FPU_IRQn, APP_IRQ_PRIORITY_LOW);
    NVIC_EnableIRQ(FPU_IRQn);
    
    // 2. ���Ź���ʼ��
    App_Watch_Dog_Init();
    App_Watch_Dog_Reload();
    
    reset_value = NRF_POWER->RESETREAS;//*((uint32_t *)(0x40000000+ 0x400));
    
    if(reset_value == 0)
    {
        //WJ.20180319.���ڱ�ʾ��λ
        reset_value = 0xFE;
    }

    // 3. GPIO�ܽų�ʼ��
	PIN_LOW(SGM_IN1_PIN);   
    PIN_LOW(SGM_IN2_PIN); 
    PIN_LOW(MOTOR_EN_PIN);
    PIN_LOW(LCD_EN_PIN);
    
    
    PIN_HIGH(LCD_CS_PIN);
    PIN_HIGH(FLASH_CS_PIN);
    PIN_HIGH(G_CS_PIN);
    PIN_HIGH(FLASH_DATA_CS_PIN);
    
    LDO_TX_OFF();
    PIN_HIGH(SI1182_POWER_LDO_EN);
    
    nrf_gpio_cfg_input(CHARGING_PIN,NRF_GPIO_PIN_NOPULL);
    nrf_gpio_cfg_input(CHG_PIN,NRF_GPIO_PIN_NOPULL);
    nrf_gpio_cfg_input(VBAT_DET_PIN,NRF_GPIO_PIN_NOPULL);
    
    Buttons_Init();
    
	//SPI0--LCD SPI1--AFE4900 SPI2--��Flash���ֿ⣩
    Bps_Spi0Init();
	Bps_Spi1Init();
    Bps_Spi2Init();
	
    // ����Э��ջ�̳߳�ʼ��
    BleThread_Init();
    
	nrf_delay_ms(1000);
	
    //Flash��֤
    Extflash_Open();
    Extflash_DataOpen();

	//1182 ��ʼ��
    Si1182_PowerOnInit();
    
    //���ٶ�
    GSensor_Init();  
    //Gsensor_Stop();
    
	//��Ļ��ʼ��
    LCD_DriverInit(); 

    // �㷨�̳߳�ʼ��
    AlgThread_Init();
    
    // ͨ���̳߳�ʼ��
    ComThread_Init();
     
    // GUI�̳߳�ʼ��
    GuiThread_Init();
    
    Battery_ADC_Init(); 

    // �˶�ģʽ��ʼ��
    Movement_RestInit();
    
    Extflash_SetEcgIndexInit();
    
    // ����ϵͳʱ��
    SystemTime_Enable();
    
    reset_time_minutes = clock->minutes;

    // ���Դ���
	//Si1182_InitByMode(1);
  	//Statemachine_AddState(MEAT_OXYGEN_APP);
	
    // Activate deep sleep mode
    SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;

    // Start FreeRTOS scheduler.
    vTaskStartScheduler();

    while (true)
    {
        APP_ERROR_HANDLER(NRF_ERROR_FORBIDDEN);
    }
}


void Main_Init(void)
{
/*
    App_Watch_Dog_Reload();
    
    Buttons_Init();
    
    //���ٶ�
    Gsensor_Init();  
    
    //Flash��֤
    Extflash_Open();
	//��Ļ��ʼ��
    LCD_DriverInit(); 

    // ͨ���̳߳�ʼ��
    ComThread_Init();
     
    // GUI�̳߳�ʼ��
    GuiThread_Init();
    
    // �˶�ģʽ��ʼ��
    Movement_RestInit();
    
    Extflash_SetEcgIndexInit();
   
    // ����ϵͳʱ��
    SystemTime_Enable();
     
//    //Statemachine_AddState(MEAT_PPG_BTN_HEART);

    //��������
    BleThread_AdvertisingStart();    
	*/
}

