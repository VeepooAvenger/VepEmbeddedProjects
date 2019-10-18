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
    uint8_t     advertising;            //�㲥
    uint8_t     heart_led_status;       //���ʵ�״̬
    uint8_t     measuring_status;       //����״̬
    uint8_t     app_heart_status;       //APP���ʲ���״̬
    uint8_t	    accworking;             //    
    uint8_t	    ble_connect;            //BLE ON
    uint8_t     power_state;            //���״̬
    uint8_t	    sleep_enter;            //����˯��
    uint8_t     first_time_check_flag;  //��һ��ʱ��У׼��־
    uint8_t     code_passed;            //�����׼ͨ����־
    uint8_t     sign_off;               //���ػ�����־
    uint8_t     sign_check_light;       //���ڽ��л�������ż���־�����û�����ʱ��ֹ�ڻ��������������������⡣
    uint8_t     sign_test;
    uint8_t     decryption_ok;          //����ͨ����־ 
    uint8_t     sport_complete;         //�˶����
    uint8_t     step_gauge_report;
    uint8_t     daily_save_state;       //����Ӵ洢״̬�����ڼ���˯��
    uint8_t     routine_check;          //�Զ�����־
    uint8_t     adc_type;               //�ϴ�ADC����
    uint8_t     ota_encrypt;
    uint8_t     system_info_save;       //��Ϣ�����־   
    uint8_t     system_info_save_cnt;   //��Ϣ�����־    
    uint8_t     bp_calibration;         //ѪѹУ׼�Ƿ���
    uint8_t     motor_type;             //��������ͣ���Ҫ��Կ��ػ�
    uint8_t     bond_flag;              //��Ҫִ�а�
    uint8_t     movement_patterns;      //�˶�ģʽ
         
    uint8_t     update_uimode;         //����UI����ģʽ
    uint8_t     read_uimode;           //��ȡUI����ģʽ
    uint8_t     camera_mode;           //����ģʽ
    uint8_t     battery_dect;          //��ؼ��     
    uint8_t     one_minute;            //1����
    uint8_t     bp_flag;               //Ѫѹ֧�ֱ��
    uint8_t     sit_flag;               //����֧�ֱ��
    uint8_t     power_off_clear;        // �ػ�������ݱ�־
}system_flag_t;

#pragma pack()


enum MEASURING_STATUS{
    
    EVENT_NULL = 0,                //����    
    BATT_DECTED_STATUS,            //��ص������   
    TIME_START_MEASURING_STATUS,   //��ʱ���Կ�ʼ      
    TIME_BLOOD_PRESSURE_STATUS,    //��ʱ����Ѫѹ  
    TIME_HEARTING_STATUS,          //��ʱ��������   
    TIME_MAX_MEASURING_STATUS, 
 
    APP_BLOOD_PRESSURE_STATUS,     //app����Ѫѹ    
    APP_HEARTING_STATUS,           //app��������
    APP_MAX_MEASURING_STATUS,

    BUTTON_BLOOD_PRESSURE_STATUS,  //��������Ѫѹ
    BUTTON_HEARTING_STATUS,        //������������ 
    BUTTON_MAX_MEASURING_STATUS, 
};

enum LANGUAGE_BROADCASTS{
    NULL_BROADCASTS = 0x00,
    SPORTS_BROADCASTS = 0x01,
    HEART_BP_BO_BROADCASTS = 0x02,  
};

extern uint8_t ble_name[8]; //��������

//�㲥��ʼ��
void advertising_init(void);

//��ȡbat����ͨ��
ble_nus_t *Ble_GetBatSrvAddr(void);

//��ȡClk����ͨ��
ble_clk_t *Ble_GetClkSrvAddr(void);

//��ȡ����MAC
void Ble_GetBleAddr(uint8_t *addr);

//�Ͽ�����
void Ble_DisconnectBle(void);

//��ȡ���Ӿ��
uint16_t *Ble_GetConnHandle(void);

//���������ӵ�
uint8_t Ble_IsConnState(void);

//�����㲥
void Ble_OpenBle(void);

//ֹͣ�㲥
void Ble_CloseBle(void);

#endif //USER_APP_TYPE_H

