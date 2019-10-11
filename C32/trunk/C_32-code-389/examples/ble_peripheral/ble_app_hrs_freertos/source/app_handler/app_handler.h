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

// ADC数据类型
#define ADC_TYPE_NULL           0x00        //
#define ADC_TYPE_ADS1220        0x01        //ADS1220 ADC data
#define ADC_TYPE_AFE4404        0x02        //AFE4404 ADC data
#define ADC_TYPE_ACC            0x03        //ACC ADC data
#define ADC_TYPE_BP             0x04        //Blood pressure 
#define ADC_TYPE_HEART          0x05        //Heart wave data
#define ADC_TYPE_WEAR           0x06        //Wear dected wave data
#define ADC_TYPE_FATIGUE        0x07        //fatigue data

//发送Flash数据类型
#define FLASH_DATA_TYPE_DAILY       0x00        //Daily data
#define FLASH_DATA_TYPE_SLEEP       0x01        //Sleep data
#define FLASH_DATA_TYPE_OTHER       0x02        //Other data
#define FLASH_DATA_TYPE_ALARM       0x03        //alarm clock data
#define FLASH_DATA_TYPE_MOVEMENT    0x04        //movement patterns data
#define FLASH_DATA_TYPE_UI          0x05        //ui flash data

//关闭测量种类
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

//设置要发送的Flash数据信息
void App_SetFlashDataRead(void);

//设置要发送的Flash数据类型
void App_SetSendFlashDataType(uint8_t type);

//设置要发送的睡眠数据信息
void App_SetFlashDaliySleepDataInfo(uint8_t block, uint8_t day, uint8_t all_cnt, uint8_t all_pkg, uint8_t c_pkg);

//设置要发送的日常Flash数据信息
void App_SetFlashDaliyDataInfo(uint8_t block, uint16_t start_num, 
                              uint16_t all_pkg, uint16_t c_pkg, uint8_t days_ago);

//设置要发送的adc数据类型
void App_SetAdcDataType(uint8_t type);  

//发送adc数据
void App_SendAdcDataHandler(void);

//1秒钟事件处理
void App_PeriodicHandler(void);

//adc完成事件
void App_AdcSamplingCompletedHandler(void);

//五分钟事件
void App_RoutineHandler(void);

//存储日常数据
void App_StorageDailyDataHandler(void);

//存储时间数据
void App_StorageSystemTimeHandler(void);

//蓝牙连接
void App_BleConnetedHandler(void);

//蓝牙断开
void App_BleDisConnetedHandler(void);

//关机
void App_ShutdownHandler(void);

//开机
void App_BootHandler(void);

//发送flash数据
void App_SendFlashDataHandler(void);

//跨天
void App_AcrossTheDayHandler(void);

//按键
void App_KeyHandler(void);

//睡眠计算
void App_CalculateSleepHandler(void);

//语音播报
void App_SpeechHandler(void);

//
void App_DailyDataHandler(void);

//电池电量检测
void App_BatteryDectedHandler(void);

//五分钟事件删除
void App_RoutineEventDelete(void);

//闹钟检测
void App_AlarmDectedHandler(void);

//停止测量
void App_StopMeasuringHandler(uint8_t close_type);

//设置系统信息参数保存状态
void App_SetSystemInfoSave(void);

//佩戴计算
void App_WearCalculateHandler(void);

//测试
void App_TestHandler(void);

//设置蓝牙名称
void App_SetBleNameHandler(void);

//设置血压功能模块（开启和关闭）
void App_SetBpFunctionHandler(void);

#ifdef __cplusplus
}
#endif    

#endif  //APP_HANDLER_H
