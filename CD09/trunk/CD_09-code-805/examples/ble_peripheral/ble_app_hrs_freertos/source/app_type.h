/***************************Copyright(c)***********************************
**                       Veepoo Tech
**                  COPYRIGHT (C) 2016-2019
**-----------File Info-----------------------------------------------------
** File        name:     app_type.h
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
#ifndef USER_APP_TYPE_H
#define USER_APP_TYPE_H

#include <string.h>
#include "nrf_delay.h"

#include "configuration.h"
#include "board.h"
#include "bsp_spi.h"
#include "I2C.h"
#include "bsp_iic.h"
#include "motor_event.h"
#include "ble_nus.h"
#include "ble_clk.h"
#include "bracelet_protocol.h"

#include "app_task.h"
#include "app_handler.h"
#include "timer_manage.h"
//ancs
#include "app_ancs.h"
//flash
#include "app_flash.h"
#include "app_storage.h"

#include "system_clock.h"

//sport
#include "pedometer.h"
#include "gesture_test.h"



#include "fontlib_spi.h"
#include "daily_info.h"
#include "outsit.h"
#include "app_config_data.h"
#include "product_info.h"


#define FALSE  0
#define TRUE   1

#pragma pack(1)
typedef	struct
{	
    uint8_t     advertising;            //广播
    uint8_t     heart_led_status;       //心率灯状态
    uint8_t     measuring_status;       //测量状态
    uint8_t     app_heart_status;       //APP心率测量状态
    uint8_t	    accworking;             //    
    uint8_t	    ble_connect;            //BLE ON
    uint8_t     power_state;            //电池状态
    uint8_t	    sleep_enter;            //进入睡眠
    uint8_t     first_time_check_flag;  //第一次时间校准标志
    uint8_t     code_passed;            //密码核准通过标志
    uint8_t     sign_off;               //“关机”标志
    uint8_t     sign_check_light;       //正在进行环境光干扰检测标志，解决没有佩戴时防止在环境光下输出随机心率问题。
    uint8_t     sign_test;
    uint8_t     decryption_ok;          //加密通过标志 
    uint8_t     sport_complete;         //运动达标
    uint8_t     step_gauge_report;
    uint8_t     daily_save_state;       //五分钟存储状态，用于计算睡眠
    uint8_t     routine_check;          //自动检测标志
    uint8_t     adc_type;               //上传ADC类型
    uint8_t     ota_encrypt;
    uint8_t     system_info_save;       //信息保存标志   
    uint8_t     system_info_save_cnt;   //信息保存标志    
    uint8_t     bp_calibration;         //血压校准是否开启
    uint8_t     motor_type;             //马达震动类型，主要针对开关机
    uint8_t     bond_flag;              //需要执行绑定
    uint8_t     movement_patterns;      //运动模式
         
    uint8_t     update_uimode;         //更新UI数据模式
    uint8_t     read_uimode;           //读取UI数据模式
    uint8_t     camera_mode;           //拍照模式
    uint8_t     battery_dect;          //电池检测     
    uint8_t     one_minute;            //1分钟
    uint8_t     bp_flag;               //血压支持标记
    uint8_t     sit_flag;               //久坐支持标记
    uint8_t     power_off_clear;        // 关机清除数据标志
}system_flag_t;

#pragma pack()


enum MEASURING_STATUS{
    
    EVENT_NULL = 0,                //空闲    
    BATT_DECTED_STATUS,            //电池电量检测   
    TIME_START_MEASURING_STATUS,   //定时测试开始      
    TIME_BLOOD_PRESSURE_STATUS,    //定时测试血压  
    TIME_HEARTING_STATUS,          //定时测试心率   
    TIME_MAX_MEASURING_STATUS, 
 
    APP_BLOOD_PRESSURE_STATUS,     //app测试血压    
    APP_HEARTING_STATUS,           //app测试心率
    APP_MAX_MEASURING_STATUS,

    BUTTON_BLOOD_PRESSURE_STATUS,  //按键测试血压
    BUTTON_HEARTING_STATUS,        //按键测试心率 
    BUTTON_MAX_MEASURING_STATUS, 
};

enum LANGUAGE_BROADCASTS{
    NULL_BROADCASTS = 0x00,
    SPORTS_BROADCASTS = 0x01,
    HEART_BP_BO_BROADCASTS = 0x02,  
};

extern uint8_t ble_name[8]; //蓝牙名称

//广播初始化
void advertising_init(void);

//获取bat数据通道
ble_nus_t *Ble_GetBatSrvAddr(void);

//获取Clk数据通道
ble_clk_t *Ble_GetClkSrvAddr(void);

//获取蓝牙MAC
void Ble_GetBleAddr(uint8_t *addr);

//断开蓝牙
void Ble_DisconnectBle(void);

//获取连接句柄
uint16_t *Ble_GetConnHandle(void);

//蓝牙是连接的
uint8_t Ble_IsConnState(void);

//开启广播
void Ble_OpenBle(void);

//停止广播
void Ble_CloseBle(void);

#endif //USER_APP_TYPE_H

