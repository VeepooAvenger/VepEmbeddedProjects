/***************************Copyright(c)***********************************
**                       Veepoo Tech
**                  COPYRIGHT (C) 2016-2019
**-----------File Info-----------------------------------------------------
** File        name:     app_task.c
** Last modified Date:   2017-08-24
** Last SYS_VERSION:     0.0.0
** Descriptions:            
**-------------------------------------------------------------------------
** Created  by:          WJ
** Created  date:        2017-08-24
** SYS_VERSION:          0.0.0
** Descriptions:         
**-------------------------------------------------------------------------
** Modified by:				
** Modified date:  			          
** SYS_VERSION:                 
** Descriptions:                        
***************************************************************************/

#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "semphr.h"
#include "watch_dog.h"
#include "app_type.h"
#include "gui_page_display.h"
#include "sleep.h"
#include "phone_notify_event.h"
#include "bsp_led.h"
#include "battery.h"
#include "bsp_button.h"
#include "outsit.h"
#include "daily_test.h"
#include "blood_pressure.h"
#include "hal_acc_calculate.h"
#include "alarm_event.h"
#include "ext_flash.h"
#include "gui_thread.h"
#include "movement_patterns.h"
#include "vph_solo.h"
#include "nowear_detection.h"
#include "gui_update.h"
#include "physiological_period.h"
#include "I2C.h"
#include "adc.h"
/*********************************************************************
* Macro Definition
*/
#define APP_WATCH_DOG_ENABLED   (0x01)

/*********************************************************************
* Global Variables
*/
static SemaphoreHandle_t app_event_ready;  /**< Semaphore raised if there is a new event to be processed in the BLE thread. */
volatile static uint32_t app_event = 0;

system_flag_t system_flag_s;            //ϵͳ��־
system_switch_t system_switch_s;        //ϵͳ����

system_info_t system_info_s;            //ϵͳ��Ϣ

uint32_t reset_time_minutes = 0;        //��λʱ�����
//static uint8_t sys_event_flag = 0;      //�¼�״̬
/*********************************************************************
* local functions
*/
void App_AddEvent(uint32_t event);
void App_DeleteEvent(uint32_t event);
static void App_EventDeal(void);
void FPU_InterruptInit(void);
static void App_BatteryChargingMonitor(void);
    
/****************************************************************
** Function name:           App_SystemFlagInit
** Descriptions:            ��ʼ��ϵͳ��־
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2016-12-07
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void App_SystemFlagInit(void)
{
    memset(&system_flag_s, 0x00, sizeof(system_flag_t));
    
    system_flag_s.sign_off = 0x00;
    system_flag_s.decryption_ok = 0x01;
    system_flag_s.measuring_status = EVENT_NULL;
    
    if( All_StepGet() >= system_info_s.user_info.user_moving_target)
    {
        system_flag_s.sport_complete = 0x01;
    }
    
    system_flag_s.ota_encrypt = VPH_SoloShow();
    
    system_flag_s.bp_flag = Product_GetBpFunction();
    if(system_flag_s.bp_flag != 0x02)
    {
        system_flag_s.bp_flag = 0x01;
    }
    
    if(FUNCTION_SUPPORT_BP == 0)
    {
        system_flag_s.bp_flag = 0x00;
    }
    
    system_flag_s.sit_flag = Product_GetSitFunction();
    if(system_flag_s.sit_flag != 0x02)
    {
        system_flag_s.sit_flag = 0x01;
    }
      
}

/****************************************************************
** Function name:           App_PowerOnProcessing
** Descriptions:            �ϵ紦��
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2018-08-29
*****************************************************************/ 
#define DELAY_TIME_M    (300)
void App_PowerOnProcessing(void)
{
    Wdt_Init();  
    Wdt_Reload(); //1��ι��1��
    
    //���ʧ��
    PIN_LOW(MOTOR_EN_PIN);
    //���ʵ�ʧ��
    Led_Init();
    //LCDʧ�ܣ�����CS����
    PIN_HIGH(LCD_CS_PIN);
    //LCD����أ����ͱ�������
    PIN_LOW(LCD_EN_PIN);
  
    //��س��ܽ�
    Battery_InitPin();      

	
    Bps_Spi1Init();
    Bps_Spi2Init();
 

    Saadc_BatteryInit();

    Wdt_Reload(); //1��ι��1��
    for(uint16_t i = 0; i < DELAY_TIME_M; i++)
    {
        int16_t battery_adc = 0;
        
        vTaskDelay(1000);
        battery_adc = Saadc_GetBatteryAdc();
        if(battery_adc >= 2986 
          || !(Battery_IsCharging() || Battery_IsChargingFull()))//3.50V//3072)//3.60V
        {
            break;
        }
        Wdt_Reload(); //1��ι��1��
    }
    Saadc_BatteryUnInit();  
}

/****************************************************************
** Function name:           App_Init
** Descriptions:            ��ʼ����������Ӳ����
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2016-12-07
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void App_Init(void)
{
    //Init FPU Interrupt 
    FPU_InterruptInit();
    
#if APP_KEY_ENABLED     
    Button_Init();
#endif //APP_KEY_ENABLED   
    
    // Init a semaphore for the APP thread.
    app_event_ready = xSemaphoreCreateBinary();
    if (NULL == app_event_ready)
    {
        APP_ERROR_HANDLER(NRF_ERROR_NO_MEM);
    }
    
    Product_InfoCheck();
        
        
#if APP_HALACC_ENABLED    
    //nrf_delay_ms(100);
    Acc_Init();
    //nrf_delay_ms(20);
#endif 

    Extflash_Open();
    Gui_LcdDriverInit();
    
    //��ʼ��˯�߶�ȡFlash�ص�����
    Sleep_Init();
    
    // ϵͳ��Ϣ��ʱ�䣬�Ʋ���˯�ߵȣ���Ҫ�Ǵ�Flash��ȡ��ʷ��Ϣ
    Sclock_Init();
    App_SystemFlagInit();
    Storage_AllInfoInit();
    
    Config_SystemParamCheck();
    Config_SystemSwitchCheck();

    Ped_SetInputPersonalInformation(system_info_s.user_info.user_height, system_info_s.user_info.user_weight, 
                                    system_info_s.user_info.user_gender, system_info_s.user_info.user_age);
    Alarm_Init();
    Outsit_Init();
    Movement_RestInit();
    Phys_InitParam();
    
    app_event = 0;	
    
    Battery_TestingStart();        //�ϵ��ص������      
        
    reset_time_minutes = clock->minutes;
    
    Ble_OpenBle();   //�������㲥

    Gui_ThreadInit();
    
	//WJ.20171212.�����ϵ籣��ϵͳ��������ֹĬ�ϲ������ı�
    App_FlashEraseCheck();
	Storage_SaveSystemInfo();
	Storage_SaveSwitchInfo();
	
    Wdt_Reload(); //1��ι��1�� 
//    Motor_AddEvent(1, 0, 1, 0);

	Sclock_Enable();
}

/****************************************************************
** Function name:           App_Thread
** Descriptions:            app����
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2016-09-12
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void App_Thread(void * arg)
{       
    UNUSED_PARAMETER(arg);
    
    //WJ.20180831.�������ڳ����ֻ����ڿ���ʱ�����жϣ���ֹ��ѹ̫�ͳ�������,��Ҫ��ֹ������ʾ��������
    //�ϵ��������
    App_PowerOnProcessing();

    App_Init();

    while(1)
    {
        /* Wait for event from APP */
        while (xSemaphoreTake(app_event_ready, portMAX_DELAY))
        {
#if APP_WATCH_DOG_ENABLED    
            if(!system_flag_s.sign_off)      //WJ.20170607.�������ж�������Ӧ������
            {    
                Wdt_Reload(); //1��ι��1�� 
            }
#endif    
              
//            sys_event_flag = 0;
            // Just wait again in the case when INCLUDE_vTaskSuspend is not enabled
            if(app_event)
            {
                App_EventDeal();
            }   
            App_BatteryChargingMonitor(); 
            
            //UI�ⲿ�¼�ִ���Լ�
            Gui_CheckEvent();
        }
    }
}

/****************************************************************
** Function name:           App_NewEventHandler
** Descriptions:            new event need to call task
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2016-09-26
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void App_NewEventHandler(void)
{
    BaseType_t yield_req = pdFALSE;

    // The returned value may be safely ignored, if error is returned it only means that
    // the semaphore is already given (raised).
    UNUSED_VARIABLE(xSemaphoreGiveFromISR(app_event_ready, &yield_req));
    portYIELD_FROM_ISR(yield_req);   
}

/****************************************************************
** Function name:           app_GetEventValue
** Descriptions:            get app_event value
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2016-09-26
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
uint32_t app_GetEventValue(void)
{
    return app_event;
}

/****************************************************************
** Function name:           App_AddEvent
** Descriptions:            add event to app_event and call task
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2016-09-26
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void App_AddEvent(uint32_t event)
{
    app_event |= event;
//    if(sys_event_flag == 0)
    {    
//        sys_event_flag = 1;
        App_NewEventHandler();  
    }
}

/****************************************************************
** Function name:           App_DeleteEvent
** Descriptions:            delete event from app_event
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2016-09-26
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void App_DeleteEvent(uint32_t event)
{
    app_event &= ~event;
}

/****************************************************************
** Function name:           App_EventDeal
** Descriptions:            deal app_event
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2016-09-26
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
static void App_EventDeal(void)
{
    uint32_t t_event = 0;
    uint8_t  i = 0;
    
    for(i = 0; i < APP_EVENT_SIZE; i++)
    {
        t_event = 1 << i;
        if( !(t_event & app_event) ) continue;
        
        if(system_flag_s.sign_off != 0x01 && system_flag_s.ota_encrypt)
        {
            switch(t_event)
            {
                case APP_PERIODIC_EVENT:        //1s cycle events
                    App_DeleteEvent(t_event);
                    App_PeriodicHandler();
                    break;
                case APP_NEXT_DAY_EVENT:        //Across the day event
                    App_DeleteEvent(t_event);
                    App_AcrossTheDayHandler();
                    break;            
                case APP_KEY_EVENT:             //Button or a touch screen event
                    App_DeleteEvent(t_event);
                    App_KeyHandler();
                    break;

                case APP_ADC_SAMPLING_COMPLETED_EVENT:  //RATE or BP ADC sampling completed
                    App_DeleteEvent(t_event);
                    App_AdcSamplingCompletedHandler();
                    break;
                case APP_ROUTINE_EVENT:         //5 minutes timer events
                    App_DeleteEvent(t_event);
                    App_RoutineHandler();
                    break;
                case APP_STORAGE_DAILY_DATA_EVENT:         //Data storage events, mainly is the daily data
                    App_DeleteEvent(t_event);
                    App_StorageDailyDataHandler();
                    break;
                case APP_BLE_CONNECTED_EVENT:   //Bluetooth is connected 
                    App_DeleteEvent(t_event);
                    App_BleConnetedHandler();
                    break;
                case APP_BLE_DISCONNECTED_EVENT:    //Bluetooth connection is broken
                    App_DeleteEvent(t_event);
                    App_BleDisConnetedHandler();
                    break;       
                case APP_SHUTDOWN_EVENT:        //The shutdown event
                    App_DeleteEvent(t_event);
                    App_ShutdownHandler();
                    break;                  
                case APP_SEND_FLASH_DATA_EVENT: //Send Flash data
                    App_DeleteEvent(t_event);
                    App_SendFlashDataHandler();
                    break;       
                case APP_SEND_ADC_DATA_EVENT:   //Send ADC data 
                    App_DeleteEvent(t_event);  
                    App_SendAdcDataHandler();
                    break;
                case APP_CALCULATE_SLEEP_EVENT: //Sleep to calculate
                    App_DeleteEvent(t_event); 
                    App_CalculateSleepHandler();
                    break;
                case APP_STORAGE_TIME_EVENT:
                    App_DeleteEvent(t_event);
                    App_StorageSystemTimeHandler();
                    break;              
                case APP_WEAR_DECTED_EVENT:                    
                    App_DeleteEvent(t_event);
                    App_WearCalculateHandler();
                    break;
                
                case APP_BATTERY_DECTED_EVENT:                                    
                    App_DeleteEvent(t_event);
                    App_BatteryDectedHandler();
                    break;
                       
                case APP_DEV_SETBLENAME_EVENT:
                    App_DeleteEvent(t_event);
                    App_SetBleNameHandler();
                    break;
                case APP_DEV_SET_BP_FUNCTION_EVENT:
                    App_DeleteEvent(t_event);
                    App_SetBpFunctionHandler();
                    break;
                case APP_DEV_SET_SIT_FUNCTION_EVENT:
                    App_DeleteEvent(t_event);
                    App_SetSitFunctionHandler();
                    break;
                case APP_TEST_EVENT:            //test        
                    App_DeleteEvent(t_event);
                    App_TestHandler();
                    break;
                default:
                    App_DeleteEvent(t_event);
                    break; 
            }
        }
        else
        {
            switch(t_event)
            { 
                case APP_BOOT_EVENT:            
                    App_DeleteEvent(t_event);  //Powered event
                    App_BootHandler();
                    break;
                case APP_BATTERY_DECTED_EVENT:                                    
                    App_DeleteEvent(t_event);
                    App_BatteryDectedHandler();
                    break;
                case APP_ADC_SAMPLING_COMPLETED_EVENT:  //RATE or BP ADC sampling completed
                    App_DeleteEvent(t_event);
                    App_AdcSamplingCompletedHandler();  
                    break;        
                case APP_BLE_CONNECTED_EVENT:   //Bluetooth is connected 
                    App_DeleteEvent(t_event);
                    App_BleConnetedHandler();
                    break;      
                case APP_BLE_DISCONNECTED_EVENT:    //Bluetooth connection is broken
                    App_DeleteEvent(t_event);
                    App_BleDisConnetedHandler();
                    break;
                default:
                    App_DeleteEvent(t_event);
                    break;                 
            }                
        }
    } 
}

/****************************************************************
** Function name:           App_PerformEvent
** Descriptions:            �ж��Ƿ���Ҫ�����¼�����
** input parameters:        ��
** output parameters:       ��
** Returned value:          0���ޣ�1��
** Created by:              WJ              
** Created Date:            2016-09-26
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
uint8_t App_PerformEvent(void)
{
    if(!system_flag_s.sign_off || 
      (system_flag_s.sign_off && (Battery_IsCharging() || Battery_IsChargingFull()
      || system_flag_s.power_state == POWER_CHARGE 
      || system_flag_s.power_state == POWER_FULL)))    
    {
        return 1;
    }
    return 0;
}

/****************************************************************
** Function name:           App_OneSecondTrigger
** Descriptions:            A second trigger, through the system time
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2016-09-26
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void App_OneSecondTrigger(void)
{
#if APP_WATCH_DOG_ENABLED    
    if(system_flag_s.sign_off)
    {  
        //1��ι��1��         
        Wdt_Reload(); 
    }
#endif    
    
#if APP_MOTOR_ENABLED    
    if(Battery_IsLow() && system_flag_s.sign_test == 0x00)    
    {  
        //ֹͣ�� 
        Motor_DelEvent();      
    }  
    //����Ƿ��е�����¼����ж��Ƿ�ʼ��    
    Motor_EventRun();    
#endif  // APP_MOTOR_ENABLED  

    Button_PressTime();
    
    if(App_PerformEvent())
    {                   
        if(clock->seconds == 0)   
        {  
#if !(DAILY_TEST_SLEEP)     
            //û�н���˯�߷���ģʽ   
            if(!system_flag_s.sign_off)
            {
                system_flag_s.one_minute = 1;
                App_AddEvent(APP_STORAGE_TIME_EVENT); 
            }
#endif           
            if(clock->minutes % 5 == 0 && !system_flag_s.sign_off)
            {
                if(clock->hour == 0 && clock->minutes == 0)
                {
                    App_AddEvent(APP_NEXT_DAY_EVENT);   
                }
                else
                {              
                    App_AddEvent(APP_ROUTINE_EVENT);
                }           
            }             
        }
        //���ٶ�����
//        Acc_StartHandler();    
        //1�����¼�����        
        App_AddEvent(APP_PERIODIC_EVENT); 
    }
    //WJ.20171227.�Ż��ػ���ĵ�ص������
    if((clock->seconds == 0) && (clock->minutes % 2 == 0) && !Battery_IsLowToShut())
    {
        App_AddEvent(APP_BATTERY_DECTED_EVENT);            
    }     
}

/*******************************************************************************
** Function name:           FPU_InterruptInit
** Descriptions:            FPU�жϳ�ʼ��
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ             
** Created Date:            2017-02-09
**------------------------------------------------------------------------------
** Modified by:
** Modified date:
**------------------------------------------------------------------------------
*******************************************************************************/
void FPU_InterruptInit(void)
{
    NVIC_SetPriority(FPU_IRQn, APP_IRQ_PRIORITY_LOW);
    NVIC_EnableIRQ(FPU_IRQn);                
}

/*******************************************************************************
** Function name:           FPU_IRQHandler
** Descriptions:            FPU interrupt to Close FPU 
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ             
** Created Date:            2017-02-09
**------------------------------------------------------------------------------
** Modified by:
** Modified date:
**------------------------------------------------------------------------------
*******************************************************************************/
#define FPU_EXCEPTION_MASK 0x0000009F
void FPU_IRQHandler(void)
{
    uint32_t *fpscr = (uint32_t *)(FPU->FPCAR + 0x40);
    (void)__get_FPSCR();
    *fpscr = *fpscr & ~(FPU_EXCEPTION_MASK);
}

/*******************************************************************************
** Function name:           App_CheckAllSetting
** Descriptions:            ���������Լ� 
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ             
** Created Date:            2017-02-09
**------------------------------------------------------------------------------
** Modified by:
** Modified date:
**------------------------------------------------------------------------------
*******************************************************************************/
void App_CheckAllSetting(void)
{
    Config_SystemParamCheck();
    Config_SystemSwitchCheck();
    Alarm_InitCheck();
    Alarm_ValidCheck();        
}

/*******************************************************************************
** Function name:           App_CheckAllSetting
** Descriptions:            ��س����
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ             
** Created Date:            2017-02-09
**------------------------------------------------------------------------------
** Modified by:
** Modified date:
**------------------------------------------------------------------------------
*******************************************************************************/
static void App_BatteryChargingMonitor(void)
{
    static uint8_t bat_chg_state = 0;       //0:�������ʹ�� 1����س���� 2������ѳ���
    static uint8_t last_seconds = 0xFF;
    uint8_t wear_flag = 0;
    static uint8_t battery_full_time = 0;
    
    if(last_seconds == clock->seconds)
    {
        return;
    }
    last_seconds = clock->seconds;
    if(Battery_IsCharging())
    {
        battery_full_time = 0;
        if(bat_chg_state == 0)
        {
            system_flag_s.power_state = POWER_CHARGE;
            Gui_ThreadEventTaskWakeUp(CPU_EVENT_TASK_CHARGE);
            bat_chg_state = 1;
        }
        Movement_Exit();
    }
    else if(Battery_IsChargingFull())
    {
        bat_chg_state = 2;
        Movement_Exit();
    }
    else
    {
        battery_full_time = 0;
        bat_chg_state = 0;
        Battery_SetChargingFullState(0);
        wear_flag = Get_NoWearStation();
        if(wear_flag == 6)
        {
            Vnwd_NoWearInit();
        }
        system_flag_s.power_state = POWER_NORMAL;
        
        //WJ.20180130.������Ӵ��������³���¼�
        if(Gui_GetThreadJob() == CPU_EVENT_TASK_CHARGE)
        {
            Gui_ThreadEventTaskWakeUp(CPU_EVENT_TASK_ALL_EXIT);
        }
    }
    
    if(bat_chg_state == 1)
    {
        //���û����
        Battery_SetChargingFullState(1);
        /* ���*/
        Vnwd_SetWear(6);
        Battery_ClearLowFlag();
        system_flag_s.power_state = POWER_CHARGE;
        //YX.20180712.����豸û�п�����Ҫ����
        if((Acc_Is_Move() && Gui_GetThreadTask() == CPU_TASK_IDEL && Gui_GetThreadJob() == CPU_EVENT_TASK_IDEL)
           || system_flag_s.sign_off == 1)
        {                   
            Gui_ThreadEventTaskWakeUp(CPU_EVENT_TASK_CHARGE);
        }
    }
    else if(bat_chg_state == 2)
    {
        //��س���
        Battery_SetChargingFullState(2);
        Vnwd_SetWear(6);
        Battery_ClearLowFlag();
        system_flag_s.power_state = POWER_FULL;
        //YX.20180712.����豸û�п�����Ҫ����
        if((Acc_Is_Move()&& Gui_GetThreadTask() == CPU_TASK_IDEL && Gui_GetThreadJob() == CPU_EVENT_TASK_IDEL)
           || system_flag_s.sign_off == 1)
        {                   
            Gui_ThreadEventTaskWakeUp(CPU_EVENT_TASK_CHARGE);
        }
        
        if(battery_full_time > 8)
        {
            Battery_SetFull();
        }
        else
        {
            battery_full_time++;
        }
    } 
}

