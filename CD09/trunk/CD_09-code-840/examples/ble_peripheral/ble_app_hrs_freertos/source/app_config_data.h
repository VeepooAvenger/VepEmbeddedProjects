/***************************Copyright(c)***********************************
**                       Veepoo Tech
**                  COPYRIGHT (C) 2016-2019
**-----------File Info-----------------------------------------------------
** File        name:     app_config_data.h
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

#ifndef _APP_CONFIG_DATA_H_
#define _APP_CONFIG_DATA_H_

#include <stdint.h>
#include <stdbool.h>
#include "common.h"
#include "alarm_event.h"
#include "outsit.h"
#include "blood_pressure.h"


#define FUNCTION_SUPPORT_DISTANCE           1       //����\����Ӣ��
#define FUNCTION_SUPPORT_TIME_FORMATS       1       //ʱ��
#define FUNCTION_SUPPORT_HEART              1       //����
#define FUNCTION_SUPPORT_BP                 2       //Ѫѹ
#define FUNCTION_SUPPORT_BO                 0       //Ѫ��
#define FUNCTION_SUPPORT_SP_BROADCAST       0       //�˶���������                       
#define FUNCTION_SUPPORT_HBO_BROADCAST      0       //����Ѫ��Ѫѹ����     

#define FUNCTION_SUPPORT_WEAR_DET           1       //������
#define FUNCTION_SUPPORT_FIND_PHONE         2       //�ֻ�����֧��
#define FUNCTION_SUPPORT_ACCURATE_BP        0       //�߶�Ѫѹ��̬У׼
#define FUNCTION_SUPPORT_STOPWATCH          2       //���֧��

#define FUNCTION_SUPPORT_UI_STYLE           7       //֧��UI������� 


#define FUNCTION_LED_GEAR                   1       //����

#define FUNCTION_HRV                        0       //hrv
#define FUNCTION_CALL_ANSWERING             0       //�����Զ�����

#define FUNCTION_BLE_DISCONNECT_REMIND      2       //������������

typedef enum
{
    CONFIG_LANGUAGE_CHINESE = 1,       //����
    CONFIG_LANGUAGE_ENAGLISH,          //Ӣ��
    CONFIG_LANGUAGE_JAPANESE,          //����
    CONFIG_LANGUAGE_KOREAN,            //����
    CONFIG_LANGUAGE_GERMAN,            //����
    CONFIG_LANGUAGE_RUSSIAN,           //����
    CONFIG_LANGUAGE_SPANISH,           //��������
    CONFIG_LANGUAGE_ITALIARN,          //�������
    CONFIG_LANGUAGE_FRENCH,            //����
    CONFIG_LANGUAGE_VIETNAMESE,        //Խ����
    CONFIG_LANGUAGE_PORTUGUESE,        //��������
    CONFIG_LANGUAGE_TRADITIONAL,       //����
}CONFIG_LANGUAGE_E;


#pragma pack(1)
typedef struct
{
    uint8_t user_height;
    uint8_t user_weight;
    uint8_t user_age;
    uint8_t user_gender;
    uint16_t user_moving_target;    
}user_info_t;

/* ��Ϣ���Ϳ��� */
typedef struct
{
    uint8_t call_enable:1;      //����
    uint8_t sms_enable:1;       //����Ϣ
    uint8_t wechat_enable:1;    //΢�ſ���
    uint8_t qq_enable:1;        //qq����
    uint8_t facebook_enable:1;  //facebook����
    uint8_t twitter_enable:1;   //twitter����
    uint8_t whatsapp_enable:1;  //WhatsApp����    
    uint8_t linkedin_enable:1;  //linkedin����
    uint8_t line_enable:1;      //line����
    uint8_t instagram_enable:1; //instagram����
    uint8_t snapchat_enable:1;  //Snapchat����
    uint8_t skype_enable:1;     //skype����
    uint8_t gmail_enable:1;     //Gmail����
    uint8_t other_enable:1;     //����
    uint8_t resv:2;             //����
    uint8_t retain;             //����
}message_enable_t;

/* ���ʸ澯 */
typedef struct
{
    uint8_t heart_enable;    //
    uint8_t heart_low;    //
    uint8_t heart_hight;    //
}heart_alarm_enable_t;

//�������ò����ṹ��
typedef struct
{
    uint8_t enable;    //����ʹ�ܿ���
    uint8_t s_hour;    //������ʼʱ��--Сʱ
    uint8_t s_minute;  //������ʼʱ��--����
    uint8_t e_hour;    //�������ʱ��--Сʱ
    uint8_t e_minute;  //�������ʱ��--����
}turnwrist_config_t;

typedef struct
{
    uint8_t leve;      //���ȵȼ�
    uint8_t setleve;   //��������ʱ������ȵȼ�
    uint8_t s_hour;    //������ʼʱ��--Сʱ
    uint8_t s_minute;  //������ʼʱ��--����
    uint8_t e_hour;    //���Ƚ���ʱ��--Сʱ
    uint8_t e_minute;  //���Ƚ���ʱ��--����
}brightness_config_t;

typedef	struct
{
    uint16_t                password_num;
    user_info_t             user_info;          //6
    test_mode_t             bp_test_mode;       //6
    uint8_t                 gesture_level ;     //ת��ȼ���ɾ��ԭ�������ʶ���ֵ
    message_enable_t        message_enable;     //1 
    uint8_t                 wear_detect;             //��� 1
    outsit_t                outsit_cfg;              //���� 7  	
    heart_alarm_enable_t    heart_alarm_enable;      // 3
    uint8_t                 horizontal_enable;       //��������  1
    turnwrist_config_t      turnwrist_config;        //̧�ַ�������  5
    brightness_config_t     brightness_leve;         //��Ļ����      6
    uint8_t                 lost_phone;              //�ֻ�����      1
    countdown_config_t      countdown_config;        //��ε���ʱ״̬ 7
    uint16_t                movement_max_puse_time;  //�˶�ģʽ�������ͣʱ��
    uint8_t                 language;                //ϵͳ����
    uint8_t                 ui_style;                //UI���
}system_info_t;         //sizeof is 

typedef struct{
    uint8_t     distance_unit;           //����Ӣ��
    uint8_t     time_formats;            //ʱ����ʽ
    uint8_t     heart;                   //����
    uint8_t     blood_pressure;          //Ѫѹ    
    uint8_t     sport_broadcasts;        //�˶���������
    uint8_t     heart_bp_bo_broadcasts;  //����Ѫ��Ѫѹ����  
    uint8_t     find_phone;              //�����ֻ����������н�����ʾ    
    uint8_t     stopwatch;               //���   
    uint8_t     blood_oxygen;            //Ѫ��  
    uint8_t     led_gear;                //led��λ  1,����6����2������1��
    uint8_t     hrv;                     //hrv
    uint8_t     call;                    //�����Զ�����  
    uint8_t     ble_disconnect_remind;    //������������
}system_switch_t;

#pragma pack()

//ϵͳ������Ϣ��ʼ��
void Config_SystemInfoInit(void);

//���ֿ��س�ʼ��
void Config_SystemSwitchInit(void);

//ϵͳ�����Լ�
void Config_SystemParamCheck(void);

//���ֿ����Լ�
void Config_SystemSwitchCheck(void);

#endif
