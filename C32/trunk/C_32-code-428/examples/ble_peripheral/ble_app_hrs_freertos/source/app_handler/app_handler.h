/***************************Copyright(c)***********************************
**                       Veepoo Tech
**                  COPYRIGHT (C) 2016-2019
**-----------File Info-----------------------------------------------------
** File        name:     app_handler.h
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

#ifndef APP_HANDLER_H
#define APP_HANDLER_H

#ifdef __cplusplus
extern "C" {
#endif

#define APP_EVENT_SIZE                      32              //event size
#define APP_NULL_EVENT                      0x00000000      //null event and not deal
#define APP_PERIODIC_EVENT                  0x00000001      //500 ms cycle events
#define APP_NEXT_DAY_EVENT                  0x00000002      //Across the day event
#define APP_KEY_EVENT                       0x00000004      //Button or a touch screen event
#define APP_ADC_SAMPLING_COMPLETED_EVENT    0x00000008      //RATE or BP ADC sampling completed   
#define APP_ROUTINE_EVENT                   0x00000010      //5 minutes timer events
#define APP_STORAGE_DAILY_DATA_EVENT        0x00000020      //Data storage daily data events, five minutes data
#define APP_BLE_CONNECTED_EVENT             0x00000040      //Bluetooth is connected    
#define APP_BLE_DISCONNECTED_EVENT          0x00000080      //Bluetooth connection is broken   
#define APP_ALARM_EVENT                     0x00000100      //Warning, including alarm clocks, sedentary, other alarms
#define APP_SHUTDOWN_EVENT                  0x00000200      //The shutdown event
#define APP_BOOT_EVENT                      0x00000400      //Powered event
#define APP_SEND_FLASH_DATA_EVENT           0x00000800      //Send Flash data   
#define APP_SEND_ADC_DATA_EVENT             0x00001000      //Send ADC data   
#define APP_CALCULATE_SLEEP_EVENT           0x00002000      //Sleep to calculate
#define APP_STORAGE_TIME_EVENT              0x00004000      //Data storage system time data events     
#define APP_WEAR_DECTED_EVENT               0x00008000      //Wear test events 
#define APP_BATTERY_DECTED_EVENT            0x00010000      //Battery test events 

#define APP_DEV_SETBLENAME_EVENT            0x00200000      //Set Ble Name events
#define APP_DEV_SETBP_FUNCTION_EVENT        0x00400000      //Set bp function events
#define APP_TEST_EVENT                      0x80000000      //Test using event

#define APP_ALL_EVENT                       0xFFFFFFFF      //all using event

// ADC��������
#define ADC_TYPE_NULL           0x00        //
#define ADC_TYPE_ADS1220        0x01        //ADS1220 ADC data
#define ADC_TYPE_AFE4404        0x02        //AFE4404 ADC data
#define ADC_TYPE_ACC            0x03        //ACC ADC data
#define ADC_TYPE_BP             0x04        //Blood pressure 
#define ADC_TYPE_HEART          0x05        //Heart wave data
#define ADC_TYPE_WEAR           0x06        //Wear dected wave data
#define ADC_TYPE_FATIGUE        0x07        //fatigue data

//����Flash��������
#define FLASH_DATA_TYPE_DAILY       0x00        //Daily data
#define FLASH_DATA_TYPE_SLEEP       0x01        //Sleep data
#define FLASH_DATA_TYPE_OTHER       0x02        //Other data
#define FLASH_DATA_TYPE_ALARM       0x03        //alarm clock data
#define FLASH_DATA_TYPE_MOVEMENT    0x04        //movement patterns data
#define FLASH_DATA_TYPE_UI          0x05        //ui flash data

//�رղ�������
#define TEST_CLOSE_ALL      0
#define TEST_CLOSE_APP      1
#define TEST_CLOSE_BTN      2
#define TEST_CLOSE_TIME     3

#pragma pack(1)
typedef struct {
    uint32_t start_block;         // flash block 1K 
    uint16_t total_block;         // data len    44K   
    uint16_t c_num;               // data        0~63 
}flash_data_t;    


typedef struct {
    uint8_t block;          // flash addr
    uint16_t start_num;      // data len
    uint16_t all_pkg;
    uint16_t c_pkg;
    uint8_t  days_ago;
}flash_daliy_data_t; 

typedef struct {
    uint8_t  block;          // flash addr
    uint8_t day;             // data 
    uint8_t all_cnt;
    uint8_t all_pkg;
    uint8_t c_pkg;
}flash_daliy_sleep_data_t; 
#pragma pack()

//����Ҫ���͵�Flash������Ϣ
void App_SetFlashDataRead(void);

//����Ҫ���͵�Flash��������
void App_SetSendFlashDataType(uint8_t type);

//����Ҫ���͵�˯��������Ϣ
void App_SetFlashDaliySleepDataInfo(uint8_t block, uint8_t day, uint8_t all_cnt, uint8_t all_pkg, uint8_t c_pkg);

//����Ҫ���͵��ճ�Flash������Ϣ
void App_SetFlashDaliyDataInfo(uint8_t block, uint16_t start_num, 
                              uint16_t all_pkg, uint16_t c_pkg, uint8_t days_ago);

//����Ҫ���͵�adc��������
void App_SetAdcDataType(uint8_t type);  

//����adc����
void App_SendAdcDataHandler(void);

//1�����¼�����
void App_PeriodicHandler(void);

//adc����¼�
void App_AdcSamplingCompletedHandler(void);

//������¼�
void App_RoutineHandler(void);

//�洢�ճ�����
void App_StorageDailyDataHandler(void);

//�洢ʱ������
void App_StorageSystemTimeHandler(void);

//��������
void App_BleConnetedHandler(void);

//�����Ͽ�
void App_BleDisConnetedHandler(void);

//�ػ�
void App_ShutdownHandler(void);

//����
void App_BootHandler(void);

//����flash����
void App_SendFlashDataHandler(void);

//����
void App_AcrossTheDayHandler(void);

//����
void App_KeyHandler(void);

//˯�߼���
void App_CalculateSleepHandler(void);

//��������
void App_SpeechHandler(void);

//
void App_DailyDataHandler(void);

//��ص������
void App_BatteryDectedHandler(void);

//������¼�ɾ��
void App_RoutineEventDelete(void);

//���Ӽ��
void App_AlarmDectedHandler(void);

//ֹͣ����
void App_StopMeasuringHandler(uint8_t close_type);

//����ϵͳ��Ϣ��������״̬
void App_SetSystemInfoSave(void);

//�������
void App_WearCalculateHandler(void);

//����
void App_TestHandler(void);

//������������
void App_SetBleNameHandler(void);

//����Ѫѹ����ģ�飨�����͹رգ�
void App_SetBpFunctionHandler(void);

#ifdef __cplusplus
}
#endif    

#endif  //APP_HANDLER_H
