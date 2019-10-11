/***************************Copyright(c)***********************************
**                       Veepoo Tech
**                  COPYRIGHT (C) 2016-2019
**-----------File Info-----------------------------------------------------
** File        name:     app_handler.c
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
#include <string.h>
#include <stdlib.h>
#include "app_type.h"
#include "bsp_button.h"
#include "daily_info.h"
#include "sleep.h"
#include "ble_wechat_service.h"
#include "blood_pressure.h"
#include "heart.h"
#include "vph_wear_dect.h"
#include "battery.h"
#include "gui_page_display.h"
#include "daily_info.h"
#include "outsit.h"
#include "adc.h"
#include "bsp_led.h"
#include "vph_wear_dect.h"
#include "daily_test.h"
#include "phone_camera_event.h"
#include "phone_notify_roll_event.h"
#include "hal_acc_calculate.h"
#include "nowear_detection.h"
#include "alarm_event.h"
#include "app_adc_send.h"
#include "posture_get.h"
#include "app_hid.h"
#include "movement_patterns.h"
#include "gui_update.h"
#include "gui_thread.h"
#include "physiological_period.h"
#include "configuration.h"
#include "ble_disconnect_remind.h"
/*********************************************************************
* Macro Definition
*/
#define NEASURING_HEART_MAX_TIME    25//0      //���ʼ������ʱ��,//WJ.20171010.���������㷨����Ҫ������ʱ����ܳ�����
#define NEASURING_BP_MAX_TIME       35      //Ѫѹ�������ʱ��
//#define NEASURING_MAX_HEART_NUM     20  

#define NEASURING_MAX_TIME_TIMEOUT  40      //�Զ��������ʱʱ��

#define STORAGE_DELAY_MAX_TIME      20      //��ʱ20��洢����Ҫ��Գ�磬���ţ�����洢����ʱ

/*********************************************************************
* Global Variables
*/
extern uint8_t ble_name[8];                     //��������
extern system_flag_t system_flag_s;             //ϵͳ��־
extern system_switch_t system_switch_s;         //����
extern system_info_t system_info_s;             //ϵͳ��Ϣ

static uint8_t flash_data_type = 0;             //flash��������
static flash_daliy_data_t flash_daliy_data = {0,0,0,0};     //�ճ�����flash��Ϣ
static flash_daliy_sleep_data_t flash_daliy_sleep_data;     //˯������flash��Ϣ

static uint8_t adc_type = 0;                                //adc ����

static uint8_t measuring_time_cnt = 0;                      //�Զ�����ʱ��
static uint8_t storage_delay_flag = 0;                      //�ճ����ݴ洢��־
//static uint8_t measuring_bp_cnt = 0;
static uint8_t daily_heart_get  =   0;                      //�Ƿ��ȡ����

static system_clock_time_t shutdown_clock_time = {0x00,};   //��¼�ػ�ʱ��

//static uint8_t measuring_heart_value[NEASURING_MAX_HEART_NUM] = {0x00,};
//static uint8_t measuring_heart_value_cnt = 0;

static flash_data_t flash_data_read = {0x00,};              //��ȡflash����Ϣ

/*********************************************************************
* local functions
*/
static void App_AccDataSend(void);
static void App_CkeckRoutineEventCompleted(void);
void App_CheckSystemInfoSave(void);

/*******************************************************************************
** Function name:           App_SetFlashDataRead
** Descriptions:            ����Ҫ���͵�Flash������Ϣ
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2016-12-12
**------------------------------------------------------------------------------
** Modified by:
** Modified date:
**------------------------------------------------------------------------------
*******************************************************************************/
void App_SetFlashDataRead(void)
{
    //WJ.20170803.����˶�ģʽ���ճ�������ʼ��ַ���
    //ÿ��1K
    flash_data_read.start_block  = DAILY_DATA_BLOCK_START_PAGE * 4;//0;
    flash_data_read.total_block  = 44;    //40k
    flash_data_read.c_num = 0;
}

/*******************************************************************************
** Function name:           App_SetFlashDaliySleepDataInfo
** Descriptions:            ����Ҫ���͵�˯��������Ϣ
** input parameters:        block�����ݿ飬day����������ݣ�all_cnt���ܴ�����all_pkg���ܰ���c_pkg����ǰ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2016-12-12
**------------------------------------------------------------------------------
** Modified by:
** Modified date:
**------------------------------------------------------------------------------
*******************************************************************************/
void App_SetFlashDaliySleepDataInfo(uint8_t block, uint8_t day, uint8_t all_cnt, uint8_t all_pkg, uint8_t c_pkg)
{
    flash_daliy_sleep_data.block = block;
    flash_daliy_sleep_data.day = day;
    flash_daliy_sleep_data.all_cnt = all_cnt;
    flash_daliy_sleep_data.all_pkg = all_pkg;
    flash_daliy_sleep_data.c_pkg = c_pkg;
}

/*******************************************************************************
** Function name:           App_SetFlashDaliyDataInfo
** Descriptions:            ����Ҫ���͵�Flash������Ϣ
** input parameters:        block�����ݿ飬start_num����ʼ����all_pkg���ܰ���c_pkg����ǰ����days_ago�����������
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2016-12-12
**------------------------------------------------------------------------------
** Modified by:
** Modified date:
**------------------------------------------------------------------------------
*******************************************************************************/
void App_SetFlashDaliyDataInfo(uint8_t block, uint16_t start_num,uint16_t all_pkg, uint16_t c_pkg, uint8_t days_ago)
{
    flash_daliy_data.block = block;
    flash_daliy_data.start_num = start_num;
    flash_daliy_data.all_pkg = all_pkg;
    flash_daliy_data.c_pkg = c_pkg;
    flash_daliy_data.days_ago = days_ago;
}

/*******************************************************************************
** Function name:           App_SetSendFlashDataType
** Descriptions:            ����Ҫ���͵�Flash��������
** input parameters:        type����������
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2016-10-12
**------------------------------------------------------------------------------
** Modified by:
** Modified date:
**------------------------------------------------------------------------------
*******************************************************************************/
void App_SetSendFlashDataType(uint8_t type)
{
    flash_data_type = type;
}

/*******************************************************************************
** Function name:           App_SetAdcDataType
** Descriptions:            ����Ҫ���͵�adc��������
** input parameters:        type��adc����
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2016-10-12
**------------------------------------------------------------------------------
** Modified by:
** Modified date:
**------------------------------------------------------------------------------
*******************************************************************************/
void App_SetAdcDataType(uint8_t type)
{
    adc_type = type;
}

/*******************************************************************************
** Function name:           App_SendAdcDataHandler
** Descriptions:            ����adc����
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2016-10-12
**------------------------------------------------------------------------------
** Modified by:
** Modified date:
**------------------------------------------------------------------------------
*******************************************************************************/
void App_SendAdcDataHandler(void)
{
    if(system_flag_s.ble_connect == 0x00)    //WJ.20170306.app�����޲�������
    {
        return;
    }        
    if(adc_type == ADC_TYPE_BP || adc_type == ADC_TYPE_HEART || adc_type == ADC_TYPE_FATIGUE)
    {
        //App_Ble_Send_Continue();
        if(system_flag_s.measuring_status != APP_HEARTING_STATUS
           && system_flag_s.measuring_status != APP_BLOOD_PRESSURE_STATUS
           && system_flag_s.app_heart_status != 1)
        {
            adc_type = ADC_TYPE_NULL;
            return;
        }
        if(Adc_BleSendDataBuf())
        {
            Timer_BleSendAdcDataStart();
        }
        else
        {
            adc_type = ADC_TYPE_NULL;
        }
    }    
    else if(adc_type == ADC_TYPE_WEAR)
    {
        if(Wear_BleSendADC())
        {
            Timer_BleSendAdcDataStart();
        }        
        else
        {
            adc_type = ADC_TYPE_NULL;
        }        
    }
}

/*******************************************************************************
** Function name:           App_UpdateWechatStep
** Descriptions:            ΢���˶����ݸ���
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2016-10-12
**------------------------------------------------------------------------------
** Modified by:
** Modified date:
**------------------------------------------------------------------------------
*******************************************************************************/
static void App_UpdateWechatStep(void)
{
    static uint8_t i = 0;
    i++;
    if(i >= 60)     //1���Ӹ���һ��΢���˶�����
    {
        i = 0;
        ble_wechat_send_data(All_StepGet());
    }
}

/*******************************************************************************
** Function name:           App_ProductTest
** Descriptions:            
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2016-10-12
**------------------------------------------------------------------------------
** Modified by:
** Modified date:
**------------------------------------------------------------------------------
*******************************************************************************/
static void App_ProductTest(void)
{
    static uint8_t sign_test_cnt = 0;
    if(system_flag_s.sign_test)
    {
        sign_test_cnt++;
        if(sign_test_cnt >= 60)
        {
//            NVIC_SystemReset();     //reset
        }
    }
    else
    {
       sign_test_cnt = 0; 
    }
}

/*******************************************************************************
** Function name:           App_SportComplete
** Descriptions:            �˶�Ŀ����
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              YX     
** Created Date:            2017-09-11
**------------------------------------------------------------------------------
** Modified by:
** Modified date:
**------------------------------------------------------------------------------
*******************************************************************************/
 void App_SportComplete(void)
{
    static uint32_t moving_target_last = 0xFFFFFFFF;
    uint32_t all_step = 0;
    
    if(moving_target_last != system_info_s.user_info.user_moving_target)
    {
        if(moving_target_last != 0xFFFFFFFF)
        {
            system_flag_s.sport_complete = 0;
        }
        moving_target_last = system_info_s.user_info.user_moving_target;
    }
    all_step = All_StepGet();
    if(all_step >= system_info_s.user_info.user_moving_target)
    {
        if(system_flag_s.sport_complete == 0)
        {
            if(Gui_ThreadCanAcceptEvent(CPU_EVENT_TASK_STARGET) == 1)
            {
                system_flag_s.sport_complete = 1;
                Gui_ThreadEventTaskWakeUp(CPU_EVENT_TASK_STARGET);
            }
        }
    }
}

/*******************************************************************************
** Function name:           App_PeriodicHandler
** Descriptions:            1s cycle events
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2016-10-12
**------------------------------------------------------------------------------
** Modified by:
** Modified date:
**------------------------------------------------------------------------------
*******************************************************************************/
void App_PeriodicHandler(void)
{     
    //�˶�����⣬������˶���괦��
    App_SportComplete();
       
#if APP_HALACC_ENABLED    
if(system_flag_s.ota_encrypt)    
{    
    App_AccDataSend();      //send acc data to app per second    
    App_UpdateWechatStep();
}
#endif    
    Heart_UICloseCheck();
    App_CkeckRoutineEventCompleted();
       
    App_ProductTest();
    
    //�ж��Ƿ����˯�߼���
    if(system_flag_s.daily_save_state)
    {
        system_flag_s.daily_save_state = 0;      //Wj.20170227.���ñ�־λ
        App_AddEvent(APP_CALCULATE_SLEEP_EVENT);
    }   

    //�͵�ػ�
    if(Battery_IsLowToShut())
    {
        App_AddEvent(APP_SHUTDOWN_EVENT);
    }   

    App_CheckSystemInfoSave(); //��ʱ��������
    Alarm_SetToFlashCheck();
    
    Posture_Statistical();     //WJ.20170622.�����̬�µ�ͳ�Ʒ�ʽ
    
    //����
    if(system_flag_s.camera_mode == 1)
    {
        Camera_WaitTask();
    }
    
    {
        //�˶�ģʽ
        Movement_Monitor();
        //����ʱ  
        Countdown_ToRunSecond(); 
        
        //����UI�Ķ�ʱ������
        Gui_UpdateTimeoutHandler();
    }
    
    //1����ʱ��
    if(system_flag_s.one_minute == 1)
    {
        system_flag_s.one_minute = 0;            
        //����    
        App_AlarmDectedHandler();   
        //����                 
        Outsit_Monitor();             
        //�����Լ�
        App_CheckAllSetting();  
        //Ů�Թ���
        Phys_Monitor();
    }
    Phys_DelaySave();
    
    //WJ.20180403.������������,����״̬�£����ѿ��ؿ���    
    if(system_flag_s.sign_off != 1 
      && system_flag_s.ble_connect != 1
      && system_switch_s.ble_disconnect_remind == 1)
    {
        Remind_BleDisconnectMonitor(1, 1, 0);
    }
}

/*******************************************************************************
** Function name:           App_SetSystemInfoSave
** Descriptions:            ����ϵͳ��Ϣ��������״̬
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              SQH              
** Created Date:            2016-10-19
**------------------------------------------------------------------------------
** Modified by:
** Modified date:
**------------------------------------------------------------------------------
*******************************************************************************/
void App_CheckSystemInfoSave(void)
{
    if(system_flag_s.system_info_save)
    {
        system_flag_s.system_info_save_cnt++;
        if(system_flag_s.system_info_save_cnt >= 4)
        {
            Storage_SaveSystemInfo();
            system_flag_s.system_info_save = 0;           
            system_flag_s.system_info_save_cnt = 0;
        }
    }    
}

/*******************************************************************************
** Function name:           App_SetSystemInfoSave
** Descriptions:            ����ϵͳ��Ϣ��������״̬
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              SQH              
** Created Date:            2016-10-19
**------------------------------------------------------------------------------
** Modified by:
** Modified date:
**------------------------------------------------------------------------------
*******************************************************************************/
void App_SetSystemInfoSave(void)
{
    system_flag_s.system_info_save = 1;           
    system_flag_s.system_info_save_cnt = 0;    
}

/*******************************************************************************
** Function name:           App_AccDataSend
** Descriptions:            send heart data per second
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              SQH              
** Created Date:            2016-10-19
**------------------------------------------------------------------------------
** Modified by:
** Modified date:
**------------------------------------------------------------------------------
*******************************************************************************/
#if APP_HALACC_ENABLED  
extern uint8_t acc_cnt;
#endif
static void App_AccDataSend(void)
{
    uint8_t ble_ack[20] = {0,};
    uint16_t tData[3] = {0,};
    uint32_t all_step = 0;
  
    if(system_flag_s.step_gauge_report)//check whether send data per second
    {     
        ble_ack[0]=0xA8;           
      
        all_step = All_StepGet(); 
        ble_ack[1] = (uint8_t)((all_step >> 24) & 0xff);
        ble_ack[2] = (uint8_t)((all_step >> 16) & 0xff);
        ble_ack[3] = (uint8_t)((all_step >> 8) & 0xff);
        ble_ack[4] = (uint8_t)((all_step & 0xff));
#if APP_HALACC_ENABLED          
        ble_ack[5] = acc_cnt;
#endif        
        if(!system_flag_s.decryption_ok) //���ܲ�ͨ��      2015-01-26
        {
            ble_ack[2] = 0x0C; 
        }
      
        /* the program below is used to test G-sensor data */
#if APP_HALACC_ENABLED          
        Acc_GetData(tData);
#endif        
        ble_ack[6] = tData[0] & 0xFF;
        ble_ack[7] = tData[1] & 0xFF;
        ble_ack[8] = tData[2] & 0xFF;

        if(system_flag_s.sign_test)      //����״̬�£��ϴ� G-Sensor֮x,y,zֵ
        {   
#if APP_HALACC_ENABLED              
            Acc_GetData(tData);
#endif            
            ble_ack[6] = tData[0] & 0xFF;
            ble_ack[7] = tData[1] & 0xFF;
            ble_ack[8] = tData[2] & 0xFF;						
        } 
        ble_ack[11] = (tData[0] >>8) & 0xFF;
        ble_ack[12] = (tData[1] >>8) & 0xFF;
        ble_ack[13] = (tData[2] >>8) & 0xFF;       
        
        Ble_SendDataViaBat(ble_ack, 20);//�ϴ��Ʋ����
    }
  
}

/*******************************************************************************
** Function name:           App_AdcSamplingCompletedHandler
** Descriptions:            The ADC sampling completed, Blood pressure��Heart rate and Blood oxygen calculation
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2016-10-12
**------------------------------------------------------------------------------
** Modified by:
** Modified date:
**------------------------------------------------------------------------------
*******************************************************************************/
void App_AdcSamplingCompletedHandler(void)
{
    if((!Bp_IsWorkBusy() && !Heart_IsBusy() //&& !Fatigue_IsBuys() 
      && system_flag_s.measuring_status != BATT_DECTED_STATUS)
      || system_flag_s.ota_encrypt == 0)
    {       
        return;
    }
    
    /*------------------------------ Ѫѹ��� --------------------------------*/
    if(system_flag_s.measuring_status == TIME_BLOOD_PRESSURE_STATUS 
      || system_flag_s.measuring_status == BUTTON_BLOOD_PRESSURE_STATUS 
      || system_flag_s.measuring_status == APP_BLOOD_PRESSURE_STATUS)   
    {
        Bp_CalculateProcess();
        
        if(system_flag_s.measuring_status == APP_BLOOD_PRESSURE_STATUS )
        {
            Bp_BleSendTestResultValue();
            if(system_flag_s.adc_type == 0)
            {
                App_SetAdcDataType(ADC_TYPE_BP);
                App_AddEvent(APP_SEND_ADC_DATA_EVENT);
            }
        }
    }//bp detect end
    /*------------------------------ ���ʼ�� --------------------------------*/
    else if(system_flag_s.measuring_status == TIME_HEARTING_STATUS 
           || system_flag_s.measuring_status == BUTTON_HEARTING_STATUS 
           || system_flag_s.measuring_status == APP_HEARTING_STATUS)
    {
        Heart_Calculate_Process();
        
        if(system_flag_s.measuring_status == APP_HEARTING_STATUS || system_flag_s.app_heart_status == 1)
        {
            //�������ʽ��
            Heart_BleSend_Value();
            
            if(system_flag_s.adc_type == 0)
            {
                App_SetAdcDataType(ADC_TYPE_HEART);
                App_AddEvent(APP_SEND_ADC_DATA_EVENT);
            }
        } 
    }
  
    /*------------------------------ ��ؼ�� --------------------------------*/
    if(system_flag_s.measuring_status == BATT_DECTED_STATUS 
      || system_flag_s.battery_dect == 1)
    {
        if(system_flag_s.measuring_status == BATT_DECTED_STATUS)
        {
            Battery_TestingStop();       
            system_flag_s.measuring_status = EVENT_NULL;
        }
        Battery_TestingCalculate();
        system_flag_s.battery_dect = 0;
    }  
}

/*******************************************************************************
** Function name:           App_CkeckRoutineEventCompleted
** Descriptions:            �������Ӵ������¼�������
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2016-12-07
**------------------------------------------------------------------------------
** Modified by:
** Modified date:
**------------------------------------------------------------------------------
*******************************************************************************/
static void App_CkeckRoutineEventCompleted(void)
{
    uint8_t wear_no_pass = 0;
    uint8_t dp = 0;
    uint8_t sp = 0;
    uint8_t flash_daliy_heart = 0;                       //����
    
    if(system_flag_s.routine_check == 0)
    {
        if( storage_delay_flag == 1 )
        {
            if(++measuring_time_cnt >= STORAGE_DELAY_MAX_TIME)
            {
                storage_delay_flag = 0;
                measuring_time_cnt = 0;
                if(daily_heart_get == 1)
                {
                    daily_heart_get = 0;
                    flash_daliy_heart = Heart_GetAveragePulseRate();
                    Daily_SetDailyData(0, flash_daliy_heart, 0, 0);
                }
                App_AddEvent(APP_STORAGE_DAILY_DATA_EVENT);
            }
        }
        return;
    }

    if((system_info_s.wear_detect == 0x01 && system_switch_s.led_gear == 1) && Wear_DectGetState() == 0 && Wear_GetWearFlag() != 0)
    {
        wear_no_pass = 1;
    }
    
    measuring_time_cnt++;     
    if(wear_no_pass == 1 || (system_flag_s.measuring_status > TIME_MAX_MEASURING_STATUS 
      || system_flag_s.measuring_status < TIME_START_MEASURING_STATUS) || measuring_time_cnt >= NEASURING_MAX_TIME_TIMEOUT) 
//      && system_flag_s.measuring_status != TIME_WEAR_STATUS))   //WJ.20170322 ���ͨ�����������ݵ�����
    {   //�����ͨ�� || ����������     
        system_flag_s.routine_check = 0;
        if(system_info_s.wear_detect == 0x01 && system_switch_s.led_gear == 0x01)
        {
            Daily_SetDailyData(0, Wear_GetWearFlag(), 0, 0);
        }
        else
        {
            Daily_SetDailyData(0, 0, 0, 0);
        }
        storage_delay_flag = 1;
        
        if(measuring_time_cnt >= NEASURING_MAX_TIME_TIMEOUT)
        {
            App_StopMeasuringHandler(TEST_CLOSE_TIME);
            measuring_time_cnt = 0;
        }
        
        return;
    }
    else if(system_flag_s.measuring_status == TIME_HEARTING_STATUS)
    {
        if(measuring_time_cnt >= NEASURING_HEART_MAX_TIME 
          || Heart_GetValidPulseRateNum() >= HEART_VALID_DATA_NUM)
        {
            flash_daliy_heart = Heart_GetAveragePulseRate();
            Heart_Close();
            system_flag_s.heart_led_status = false;
            system_flag_s.measuring_status = EVENT_NULL;
            measuring_time_cnt = 0;
            Daily_SetDailyData(0, flash_daliy_heart, 0, 0);
            system_flag_s.routine_check = 0;
            storage_delay_flag = 0;
            App_AddEvent(APP_STORAGE_DAILY_DATA_EVENT);    
            //WJ.20170228.���ʹ�������
            if(system_info_s.heart_alarm_enable.heart_enable 
              && flash_daliy_heart >=system_info_s.heart_alarm_enable.heart_hight 
              && Daily_GetIntervalSteps10m() < 40 && !system_flag_s.sleep_enter)       
            {                 
                Gui_ThreadEventTaskWakeUp(CPU_EVENT_TASK_HEARTALERT); 
            }             
        }
    }
    else if(system_flag_s.measuring_status == TIME_BLOOD_PRESSURE_STATUS)
    {
        if(measuring_time_cnt >= NEASURING_BP_MAX_TIME)
        {
            Bp_Close();
            system_flag_s.heart_led_status = false;
            system_flag_s.measuring_status = EVENT_NULL;
            measuring_time_cnt = 0;
            system_flag_s.routine_check = 0;
            storage_delay_flag = 0;           
            Daily_SetDailyData(0, 0, 0, 0);
            App_AddEvent(APP_STORAGE_DAILY_DATA_EVENT);
        }
        else
        {       
            if(Bp_GetFlagState() > 0)
            {
                Bp_GetTestResult(&sp, &dp);
                Daily_SetDailyData(0, 0, dp, sp);
                Bp_Close();
                system_flag_s.heart_led_status = false;
                system_flag_s.measuring_status = EVENT_NULL;  
                measuring_time_cnt = 0;  
                system_flag_s.routine_check = 0; 
                storage_delay_flag = 0;                
                App_AddEvent(APP_STORAGE_DAILY_DATA_EVENT);                
            }
        }               
    } 
}

/*******************************************************************************
** Function name:           App_RoutineEventDelete
** Descriptions:            ɾ��������¼�����������
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2016-12-07
**------------------------------------------------------------------------------
** Modified by:
** Modified date:
**------------------------------------------------------------------------------
*******************************************************************************/
void App_RoutineEventDelete(void)
{           
    App_StopMeasuringHandler(TEST_CLOSE_TIME);  
    system_flag_s.routine_check = 0;   
    measuring_time_cnt = 0;
    storage_delay_flag = 0;  
//    measuring_bp_cnt = 0;
    
    App_DeleteEvent(APP_ROUTINE_EVENT);   
    App_DeleteEvent(APP_STORAGE_DAILY_DATA_EVENT);      
}

/*******************************************************************************
** Function name:           App_RoutineHandler
** Descriptions:            5 minutes timer events
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2016-10-12
**------------------------------------------------------------------------------
** Modified by:
** Modified date:
**------------------------------------------------------------------------------
*******************************************************************************/
void App_RoutineHandler(void)
{    
    uint8_t storage_flag = 0;
    
    //��ȡ��ǰ����
    Daily_GetDailyData();

#if DAILY_TEST_SLEEP    
#if DAILY_TEST_SLEEP_WRITE_DATA   
    if(clock->day == 13 && clock->hour > 9)
    {
            return;
    }    
    App_AddEvent(APP_STORAGE_DAILY_DATA_EVENT); 
#else
    Daily_TestSleepAddPkg();    
    App_AddEvent(APP_STORAGE_DAILY_DATA_EVENT);  
    return;        
#endif    
    
#else    
    //������� ,�����ֻ�ȡ��
    if(system_flag_s.power_state != POWER_NORMAL 
      || (Get_NoWearStation() >= 2 && Get_NoWearStation() <= 4) 
      || (Get_NoWearStation() == 6))   
    {
        //WJ.2017.02.23-11��ʾ��磬�ֻ��뿪���
        if(system_flag_s.power_state != POWER_NORMAL)
        {           
            Daily_SetDailyData(0, 6, 0, 0);
        }
        else
        {
            Daily_SetDailyData(0, 0, 0, 0);
        }
        storage_delay_flag = 1;
        measuring_time_cnt = 0;
        return;
    }
      
    // ÿʮ����һ��Ѫѹ���
    if(clock->minutes % 10 == 0 )
    {
        if(system_flag_s.bp_flag == 0x01)
        {           
            if(system_flag_s.measuring_status < TIME_START_MEASURING_STATUS && system_switch_s.blood_pressure == 0x01)
            {
                system_flag_s.measuring_status = TIME_BLOOD_PRESSURE_STATUS; 
                system_flag_s.heart_led_status = true;
                Bp_Open();  
                storage_flag = 1;
                measuring_time_cnt = 0;    
                system_flag_s.routine_check = 1;            
            }
        }
    }
    else if(clock->minutes % 10 == 5)//10�����¼��������Զ����
    {
        if(system_flag_s.measuring_status < TIME_START_MEASURING_STATUS && system_switch_s.heart == 0x01)
        {
            system_flag_s.measuring_status = TIME_HEARTING_STATUS; 
            system_flag_s.heart_led_status = true;
            Heart_Open();   
            storage_flag = 1;
            measuring_time_cnt = 0;
            system_flag_s.routine_check = 1;
        }
        else if(system_switch_s.heart == 0x01)
        {
            //WJ.20170808.���APP���߰�����ʱ���������û�����ʵ�����
            Heart_ClearAverValue();     
            daily_heart_get = 1;
        }
    }
    
    if(!storage_flag)
    {
        Daily_SetDailyData(0, 0, 0, 0);                   
        measuring_time_cnt = 0;
        storage_delay_flag = 1;
    }
#endif    
}

/*******************************************************************************
** Function name:           App_StorageDailyDataHandler
** Descriptions:            Data storage daily data events
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2016-10-12
**------------------------------------------------------------------------------
** Modified by:
** Modified date:
**------------------------------------------------------------------------------
*******************************************************************************/
void App_StorageDailyDataHandler(void)
{
    // �洢�ճ����ݣ�5����һ��
    Daily_SaveDailyData();    
}

/*******************************************************************************
** Function name:           App_StorageSystemTimeHandler
** Descriptions:            Data storage daily data events
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2016-10-12
**------------------------------------------------------------------------------
** Modified by:
** Modified date:
**------------------------------------------------------------------------------
*******************************************************************************/
void App_StorageSystemTimeHandler(void)
{
    uint8_t storage_time_data[8] = {0x00,};
    uint8_t time_tmp = 0;
    
    memcpy(storage_time_data, clock, sizeof(system_clock_time_t));
    time_tmp = storage_time_data[0];
    storage_time_data[0] = storage_time_data[1];
    storage_time_data[1] = time_tmp;
    
    // ʱ��洢��һ����һ��
    App_FlashWriteSystemDataRecord(system_record_s[RECORD_SYSTEM_TIME], storage_time_data, 8);  
    
}

/*******************************************************************************
** Function name:           App_BleConnetedHandler
** Descriptions:            Ble Conneted
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2016-10-12
**------------------------------------------------------------------------------
** Modified by:
** Modified date:
**------------------------------------------------------------------------------
*******************************************************************************/
void App_BleConnetedHandler(void)
{
    // �������ӱ�־
    system_flag_s.ble_connect = 0x01;
    disconnet_keep_flg = 0;
    Phone_NotifySetCallingState(PHONE_CALL_IDLE);
    //WJ.20180403.����������������
    Remind_BleDisconnectRemove();
}

/*******************************************************************************
** Function name:           App_BleDisConnetedHandler
** Descriptions:            Bluetooth connection is broken 
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2016-10-12
**------------------------------------------------------------------------------
** Modified by:
** Modified date:
**------------------------------------------------------------------------------
*******************************************************************************/ 
void App_BleDisConnetedHandler(void)
{
    // �������ӱ�־
    system_flag_s.ble_connect = 0x00;
    system_flag_s.code_passed = false;
    // ֹͣ������������¼�
    
    // ֹͣAPP����������¼�
    App_StopMeasuringHandler(TEST_CLOSE_APP);  
    system_flag_s.bp_calibration = 0;
    system_flag_s.app_heart_status = 0;
    system_flag_s.update_uimode = 0;
    Gui_UpdateTimeoutStop();
    
    //WJ.20171010.��������Ͽ��豸�����ڲ���״̬
    if(system_flag_s.sign_test == 1)
    {
        //�ر����
        Motor_PowerOFF();
        //�رղ�����ʾ
        if(Gui_GetThreadJob() == CPU_EVENT_TASK_FONTTEST)
        {
            Gui_ThreadEventTaskWakeUp(CPU_EVENT_TASK_ALL_EXIT);
        }
        if(system_flag_s.measuring_status < TIME_START_MEASURING_STATUS
           && system_flag_s.heart_led_status == true)
        {
            Led_CloseAll();
            system_flag_s.heart_led_status = false;
        }
    }
    system_flag_s.sign_test = 0x00;  
    system_flag_s.step_gauge_report = 0x00;
    //��������          
    {            
        Ancs_SetBlePhone(0x00);           
        Phone_NotifySetCallingState(PHONE_CALL_IDLE);
    }  
    //�˳�����
    system_flag_s.camera_mode = 0;
    
    if(Gui_GetThreadJob() == CPU_EVENT_TASK_CAMERAREADY ||
       Gui_GetThreadJob() == CPU_EVENT_TASK_CAMERAOK ||
       Gui_GetThreadJob() == CPU_EVENT_TASK_CALL_REMIND)
    {       
        Gui_ThreadEventTaskWakeUp(CPU_EVENT_TASK_ALL_EXIT);
    }  
    
     //WJ.20180403.������������,����״̬�£����ѿ��ؿ���    
    if(system_flag_s.sign_off != 1 
      && system_flag_s.ble_connect != 1
      && system_switch_s.ble_disconnect_remind == 1)
    {
        Remind_BleDisconnectDisPlay(1, 1, 0);
    } 
}

/*******************************************************************************
** Function name:           App_ShutdownHandler
** Descriptions:            The shutdown event
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2016-10-12
**------------------------------------------------------------------------------
** Modified by:
** Modified date:
**------------------------------------------------------------------------------
*******************************************************************************/
void App_ShutdownHandler(void)
{
    // �ػ���־λ��
    system_flag_s.sign_off = 0x01;
    
    system_flag_s.adc_type = 0;      //ֻ��Ѫѹ��
    // ʱ�Ӷ�ʱ����Ķ�ʱ���ر�
    
    // �㲥�رգ����ӶϿ�
    Ble_CloseBle();
    // ADC�����رգ�ģ���Դ�ر�
    {    
    App_RoutineEventDelete();          
    App_StopMeasuringHandler(TEST_CLOSE_ALL);
    system_flag_s.bp_calibration = 0;
    }
    // ���ٶ�����
#if APP_HALACC_ENABLED      
    if(system_flag_s.power_state == POWER_NORMAL)    //WJ.20170616.��ֹ�����ʾ�ӳ�
    {        
        Acc_Stop();
    }
#endif    
    // �Ƿ���˯��״̬�����˯��
    Sleep_LowBateeryProcess();

#if APP_MOTOR_ENABLED  
//    Motor_AddEvent(1, 0, 1, 0);
#endif
    
    Message_Clear();
    
    Phys_ClearRemindInfo();
    
    // �ر�OLED��ʾ 
//    if(!Battery_IsCharging())
//    {
        Gui_ThreadPowerOff();   
//    }
    //�������
    Watch_Init();   
    //����ʱ
    Countdown_Init();   
    //�˳��˶�ģʽ
    Movement_Exit();
    
    //��¼�ػ�ʱ��
    memcpy(&shutdown_clock_time, clock, sizeof(system_clock_time_t));    
     
    App_DeleteEvent(APP_ALL_EVENT);  
}

/*******************************************************************************
** Function name:           App_ComparisonSwitchMachineTime
** Descriptions:            ���ػ�ʱ��Աȣ��Լ����촦��
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2016-10-12
**------------------------------------------------------------------------------
** Modified by:
** Modified date:
**------------------------------------------------------------------------------
*******************************************************************************/
static void App_ComparisonSwitchMachineTime(void)
{
    if(shutdown_clock_time.year != clock->year
        || shutdown_clock_time.month != clock->month
        || shutdown_clock_time.day != clock->day)
    {
        //App_AddEvent(APP_NEXT_DAY_EVENT);
        App_AcrossTheDayHandler();
    }  
}

/*******************************************************************************
** Function name:           App_BootHandler
** Descriptions:            Powered event
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2016-10-12
**------------------------------------------------------------------------------
** Modified by:
** Modified date:
**------------------------------------------------------------------------------
*******************************************************************************/
void App_BootHandler(void)
{      
    uint8_t power_state = 0;
    
    //�͵粻������,��δ���
    if(Battery_IsLowToShut() && !Battery_IsCharging()) return;
    
    if(Ble_IsConnState())
    {
        NVIC_SystemReset();     //reset
        return;
    }

    App_DeleteEvent(APP_ALL_EVENT);        
           
    Movement_Init();
    
    //�Աȹػ�ǰʱ���Ƿ��п��첢����Ӧ����
    App_ComparisonSwitchMachineTime();
    //���ٶȿ���
#if APP_HALACC_ENABLED      
    Acc_Start();
#endif    
    
    Outsit_Init();
    
    Config_SystemParamCheck();
    Config_SystemSwitchCheck();
    Alarm_InitCheck();
    Alarm_ValidCheck();
    Posture_StatisticalInit();
    Phys_UpdataParam();
    //������־
    power_state = system_flag_s.power_state;
    App_SystemFlagInit();
    system_flag_s.motor_type = 1;
    system_flag_s.power_state = power_state;
    //WJ.20180403.����������������
    Remind_BleDisconnectRemove(); 

    Battery_TestingStart();        //�ϵ��ص������  

    storage_delay_flag = 0;         //ɾ���ճ����ݴ洢��־

    // ��GUI������MAIN page    
    Gui_ThreadPowerOn();
    
    //�㲥����
    Ble_OpenBle();   
}

/*******************************************************************************
** Function name:           App_SendFlashDataHandler
** Descriptions:            Send Flash data, by Flash address and length of data to be sent
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2016-10-12
**------------------------------------------------------------------------------
** Modified by:
** Modified date:
**------------------------------------------------------------------------------
*******************************************************************************/
void App_SendFlashDataHandler(void)
{
    uint8_t flash_data[20] = {0x00,};
    uint8_t flash_data_sned[20] = {0x00,};
    uint8_t sned_flag = 0;
    uint8_t ret = 0;    
    uint8_t send_sleep_cnt = 0;
    uint8_t send_sleep_index = 0;
    vph_sleep_quality_info_t vph_sleep_quality_info = {0x00,};
    
    
    // ����Flash���ݣ��ճ����ݣ�˯�����ݣ�����Flash���ݣ����ݷ�����Ҫ����
    // flash_data_t �ṹ��
    if(flash_data_type == FLASH_DATA_TYPE_DAILY)
    {       
        ret = App_FlashReadDailyData(flash_daliy_data.block, 
                                    flash_daliy_data.start_num + flash_daliy_data.c_pkg, flash_data);
        if(ret == APP_FLASH_SUCCESS)
        {           
            flash_daliy_data.c_pkg++;
            flash_data_sned[0] = 0xD1;
            flash_data_sned[1] = (flash_daliy_data.c_pkg >> 0) &0xFF; 
            flash_data_sned[2] = (flash_daliy_data.c_pkg >> 8) &0xFF;
            flash_data_sned[3] = (flash_daliy_data.all_pkg >> 0) &0xFF;
            flash_data_sned[4] = (flash_daliy_data.all_pkg >> 8) &0xFF;
            flash_data_sned[5] = ((flash_daliy_data.days_ago << 5) & 0xE0) + (flash_data[2] & 0x1F);
            flash_data_sned[6] = flash_data[16];
            flash_data_sned[7] = flash_data[17];
            flash_data_sned[8] = flash_data[14];
            flash_data_sned[9] = flash_data[15];
            memcpy(&flash_data_sned[10], &flash_data[4], 9);
            flash_data_sned[19] = flash_data[3];
            sned_flag = 1;
        }
        if(flash_daliy_data.c_pkg < flash_daliy_data.all_pkg && system_flag_s.ble_connect == 0x01)
        {
            //�������¶�ʱ��
            Timer_BleSendDataStart();
        }
    }
    else if(flash_data_type == FLASH_DATA_TYPE_SLEEP)
    {
        send_sleep_cnt = flash_daliy_sleep_data.c_pkg/6;
        ret = App_FlashReadDailySleep(flash_daliy_sleep_data.block, send_sleep_cnt, (void *)&vph_sleep_quality_info);
        if(ret == APP_FLASH_SUCCESS)
        {           
            flash_daliy_sleep_data.c_pkg++;
            flash_data_sned[0] = 0xE0;
            flash_data_sned[1] = flash_daliy_sleep_data.all_pkg - flash_daliy_sleep_data.c_pkg;       
            flash_data_sned[2] = flash_daliy_sleep_data.all_cnt;
            flash_data_sned[3] = flash_daliy_sleep_data.day;    //ǰ���������

            send_sleep_index = (flash_daliy_sleep_data.c_pkg - 1) % 6;
            memcpy(&flash_data_sned[4], (((uint8_t *)&vph_sleep_quality_info) + 16 * send_sleep_index), 16);
            sned_flag = 1;
        }
        if(flash_daliy_sleep_data.c_pkg != flash_daliy_sleep_data.all_pkg && system_flag_s.ble_connect == 0x01)
        {
            //�������¶�ʱ��
            Timer_BleSendDataStart();
        }              
    }
    else if(flash_data_type == FLASH_DATA_TYPE_OTHER)
    {
        
        ret = App_FlashReadDataByAddress(flash_data_read.start_block, flash_data_read.c_num, &flash_data_sned[1]);
        if(ret == APP_FLASH_SUCCESS)
        {  
            flash_data_sned[0] = 0xF0;
            flash_data_sned[17] = flash_data_read.c_num;
            flash_data_sned[18] = flash_data_read.start_block - DAILY_DATA_BLOCK_START_PAGE * 4;  
            flash_data_sned[19] = flash_data_read.total_block;            
            if((++flash_data_read.c_num) >= 64)
            {
                flash_data_read.c_num = 0;
                ++flash_data_read.start_block;
            }          
            sned_flag = 1;
        }              
        if((flash_data_read.start_block - DAILY_DATA_BLOCK_START_PAGE * 4) < flash_data_read.total_block && system_flag_s.ble_connect == 0x01)
        {   //�������¶�ʱ��
            Timer_BleSendDataStart();
        }
    }
	else if(flash_data_type == FLASH_DATA_TYPE_ALARM)
    {
        if(Alarm_SendDataHandler())
        {   //�������¶�ʱ��
            Timer_BleSendDataStart();
        }
        sned_flag = 0;
    }
    else if(flash_data_type == FLASH_DATA_TYPE_MOVEMENT)
    {
        if(Movement_SendDataHandler())
        {
            //�������¶�ʱ��
            Timer_BleSendDataStart();
        }
        sned_flag = 0;
    }
    else if(flash_data_type == FLASH_DATA_TYPE_UI)
    {
        if(GUI_SendUiDataHandler())
        {
            //�������¶�ʱ��
            Timer_BleSendDataStart();
        }
        sned_flag = 0;
    }
    
    if(sned_flag && system_flag_s.ble_connect == 0x01)
    {
        Ble_SendDataViaBat(flash_data_sned, 20); 
    }
    
}

/*******************************************************************************
** Function name:           App_AcrossTheDayHandler
** Descriptions:            across the day event
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2016-10-12
**------------------------------------------------------------------------------
** Modified by:
** Modified date:
**------------------------------------------------------------------------------
*******************************************************************************/
void App_AcrossTheDayHandler(void)
{
    //ʱ����촦��
    
    //�����쿪��������¼�
    if( Sclock_IsZero() )
    {
        App_RoutineHandler();
    }
    
    // �洢����
    Daily_NextDay();
      
    system_flag_s.sport_complete = 0x00;
//    }    
}

/*******************************************************************************
** Function name:           App_KeyHandler
** Descriptions:            Button or a touch screen event, UI switch, switch machine, 
                            password, clear, etc
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2016-10-12
**------------------------------------------------------------------------------
** Modified by:
** Modified date:
**------------------------------------------------------------------------------
*******************************************************************************/
void App_KeyHandler(void)
{

}

/*******************************************************************************
** Function name:           App_CalculateSleepHandler
** Descriptions:            Sleep to calculate
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2016-10-12
**------------------------------------------------------------------------------
** Modified by:
** Modified date:
**------------------------------------------------------------------------------
*******************************************************************************/
void App_CalculateSleepHandler(void)
{
    Sleep_NormalProcess();   
}

/*******************************************************************************
** Function name:           App_SpeechHandler
** Descriptions:            ��
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2016-11-18
**------------------------------------------------------------------------------
** Modified by:
** Modified date:
**------------------------------------------------------------------------------
*******************************************************************************/
void App_SpeechHandler(void)
{
    // �����¼�֪ͨapp�˲�������
    // �������ݣ�Ѫѹ�����ʡ�Ѫ��  
}

/*******************************************************************************
** Function name:           App_WearCalculateHandler
** Descriptions:            �������
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-01-10
**------------------------------------------------------------------------------
** Modified by:
** Modified date:
**------------------------------------------------------------------------------
*******************************************************************************/
void App_WearCalculateHandler(void)
{
    static uint8_t last_wear_flag = 0;
    uint8_t wear_flag = 0;
    
    if(system_flag_s.sign_test == 0x01) 
    {
        return;
    }
    
    if(system_info_s.wear_detect != 0x01 || system_switch_s.led_gear != 0x01)
    {
        return;
    }   

    wear_flag = Wear_DectCalculate();
    if(system_flag_s.measuring_status == APP_BLOOD_PRESSURE_STATUS || system_flag_s.measuring_status == APP_HEARTING_STATUS)
    {
        if(wear_flag == 1 || last_wear_flag == 0 || system_flag_s.adc_type == 3)
        {
            Bp_BleSendTestResultValue();
            if(system_flag_s.adc_type != 0x01)
            {
                App_SetAdcDataType(ADC_TYPE_WEAR);
                App_AddEvent(APP_SEND_ADC_DATA_EVENT);
            }
            last_wear_flag = 0;
            if(wear_flag == 0)
            {
                last_wear_flag = 1;
            }
        }
        else
        {
            last_wear_flag = 1;
        }
    }     
}

/*******************************************************************************
** Function name:           App_BatteryDectedHandler
** Descriptions:            ������ؼ��
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-02-16
**------------------------------------------------------------------------------
** Modified by:
** Modified date:
**------------------------------------------------------------------------------
*******************************************************************************/
void App_BatteryDectedHandler(void)
{   
    system_flag_s.battery_dect = 1;
    
    if(system_flag_s.measuring_status == EVENT_NULL)
    {        
        Battery_TestingStart();
    }
    else
    {
        Saadc_CleanBatteryAdcDataLen();
    }
}

/*******************************************************************************
** Function name:           App_AlarmDectedHandler
** Descriptions:            ���Ӽ��
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-02-16
**------------------------------------------------------------------------------
** Modified by:
** Modified date:
**------------------------------------------------------------------------------
*******************************************************************************/
void App_AlarmDectedHandler(void)
{   
    Alarm_Monitor();
}

/*******************************************************************************
** Function name:           App_StopMeasuringHandler
** Descriptions:            ���ݲ������͹رղ���
** input parameters:        close_type���ر�����
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-02-16
**------------------------------------------------------------------------------
** Modified by:
** Modified date:
**------------------------------------------------------------------------------
*******************************************************************************/
void App_StopMeasuringHandler(uint8_t close_type)
{   
    uint8_t test_measuring_flag = 0;
    
    if(close_type == TEST_CLOSE_ALL)
    {
        switch(system_flag_s.measuring_status)
        { 
            case APP_BLOOD_PRESSURE_STATUS:     //app����Ѫѹ 
            {            
                uint8_t ack[20]={0x90, 0x00};                   
                ack[4] = 0x01;
                Ble_SendDataViaBat(ack, 20);//�ϴ��ֻ���ǰ����״̬    
                break;
            }
            case APP_HEARTING_STATUS:           //app��������  
            {                   
                uint8_t ack1[20]={0xD0, 0x00};  
                ack1[5] = 0x01;        
                Ble_SendDataViaBat(ack1, sizeof(ack1));//�ϴ��ֻ���ǰ����״̬
                break;
            }    
        }        
        
        Battery_TestingStop();
        Heart_Close();    
        Bp_Close();        
        test_measuring_flag = 1;        
    }
    else if(close_type == TEST_CLOSE_APP )
    {
        switch(system_flag_s.measuring_status)
        { 
        case APP_BLOOD_PRESSURE_STATUS:     //app����Ѫѹ      
            Bp_Close();        
            test_measuring_flag = 1;  
            break;
        case APP_HEARTING_STATUS:           //app��������  
            Heart_Close();           
            test_measuring_flag = 1;        
            break;
        }  
    }
    else if(close_type == TEST_CLOSE_TIME)
    {
        switch(system_flag_s.measuring_status)
        { 
        case TIME_BLOOD_PRESSURE_STATUS:     //app����Ѫѹ      
            Bp_Close();        
            test_measuring_flag = 1;  
            break;
        case TIME_HEARTING_STATUS:           //app��������  
            Heart_Close();           
            test_measuring_flag = 1;        
            break;
        }         
    }
    else
    {       
        switch(system_flag_s.measuring_status)
        { 
        case BUTTON_BLOOD_PRESSURE_STATUS:  //��������Ѫѹ    
            Bp_Close();         
            test_measuring_flag = 1;          
            break;
        case BUTTON_HEARTING_STATUS:        //������������   
            Heart_Close();      
            test_measuring_flag = 1; 
            break;
        }        
    }
    if(test_measuring_flag == 1)
    {
        Wear_Close();
        system_flag_s.measuring_status = EVENT_NULL;
        system_flag_s.heart_led_status = false;
        if(system_flag_s.bp_calibration != 0)
        {
           Bp_BleSendTestInvalid(); 
           Bp_BleSendTestInvalid(); 
           system_flag_s.bp_calibration = 0;
        }
    }
}

/*******************************************************************************
** Function name:           App_StopAppMeasuringHandler
** Descriptions:            ������������
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              YX              
** Created Date:            2017-07-20
**------------------------------------------------------------------------------
** Modified by:
** Modified date:
**------------------------------------------------------------------------------
*******************************************************************************/
void App_StopAppMeasuringHandler(void)
{   
    uint8_t test_measuring_flag = 0;
    
    switch(system_flag_s.measuring_status)
    { 
        case APP_BLOOD_PRESSURE_STATUS:     //app����Ѫѹ 
        {            
            uint8_t ack[20]={0x90, 0x00};                   
            ack[4] = 0x01;
            Ble_SendDataViaBat(ack, 20);//�ϴ��ֻ���ǰ����״̬  
            Bp_Close();  
            test_measuring_flag = 1;    
            break;
        }
        case APP_HEARTING_STATUS:           //app��������  
        {                   
            uint8_t ack1[20]={0xD0, 0x00};  
            ack1[5] = 0x01;        
            Ble_SendDataViaBat(ack1, sizeof(ack1));//�ϴ��ֻ���ǰ����״̬
            Heart_Close();    
            test_measuring_flag = 1;   
            break;   
        }
    }         

    if(test_measuring_flag == 1)
    {
        Wear_Close();
        system_flag_s.measuring_status = EVENT_NULL;
        system_flag_s.heart_led_status = false;
        if(system_flag_s.bp_calibration != 0)
        {
           Bp_BleSendTestInvalid(); 
           Bp_BleSendTestInvalid(); 
           system_flag_s.bp_calibration = 0;
        }
    }
}

/*******************************************************************************
** Function name:           App_SetBleNameHandler
** Descriptions:            ������������
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              YX              
** Created Date:            2017-07-20
**------------------------------------------------------------------------------
** Modified by:
** Modified date:
**------------------------------------------------------------------------------
*******************************************************************************/
void App_SetBleNameHandler(void)
{   
    static uint32_t         err_code = 0;
    ble_gap_conn_sec_mode_t sec_mode ={0x00,};
    
    if(ble_name[0] != 0x00)
    {
        Ble_CloseBle();//�Ͽ�����
        sd_ble_gap_adv_stop();//�رչ㲥
        Product_SetBleAdvName(ble_name);
        BLE_GAP_CONN_SEC_MODE_SET_OPEN(&sec_mode);
        //���������㲥����
        sd_ble_gap_device_name_set(&sec_mode, ble_name, strlen((char *)ble_name));
        APP_ERROR_CHECK(err_code);
//        nrf_delay_ms(20);
        advertising_init();//���³�ʼ���㲥
        Ble_OpenBle();        //���������㲥
    }
}

/*******************************************************************************
** Function name:           App_SetBpFunctionHandler
** Descriptions:            ����Ѫѹ����ģ�鿪���͹ر�
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-11-11
**------------------------------------------------------------------------------
** Modified by:
** Modified date:
**------------------------------------------------------------------------------
*******************************************************************************/
void App_SetBpFunctionHandler(void)
{       
   Product_SetBpFunction(system_flag_s.bp_flag);
}

/*******************************************************************************
** Function name:           App_TestHandler
** Descriptions:            test,  
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2016-11-18
**------------------------------------------------------------------------------
** Modified by:
** Modified date:
**------------------------------------------------------------------------------
*******************************************************************************/
void App_TestHandler(void)
{
    //����һЩ���ܲ���
    
}
