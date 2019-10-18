/***************************Copyright(c)***********************************
**                       Veepoo Tech
**                  COPYRIGHT (C) 2016-2019
**-----------File Info-----------------------------------------------------
** File        name:     app_config_data.c
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
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "app_config_data.h"
#include "app_error.h"
#include "app_type.h"
#include "util.h"

extern system_switch_t system_switch_s;
extern system_info_t system_info_s;
extern low_energe_protocol_data_t low_energe_protocol_data;


/****************************************************************
** Function name:           Config_PasswordNumInit
** Descriptions:            �����ʼ��
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-08-24
*****************************************************************/
void Config_PasswordNumInit(void)
{
   system_info_s.password_num = 0;
}

/****************************************************************
** Function name:           Config_PasswordNumCheck
** Descriptions:            �����Լ�
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-08-24
*****************************************************************/
void Config_PasswordNumCheck(void)
{
   if(system_info_s.password_num > 9999)
   {
       Config_PasswordNumInit();
   }
}

/****************************************************************
** Function name:           Config_UserInfoInit
** Descriptions:            �û���Ϣ��ʼ��
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-08-24
*****************************************************************/
void Config_UserInfoInit(void)
{
    system_info_s.user_info.user_age = 30;
    system_info_s.user_info.user_gender = 0x01;
    system_info_s.user_info.user_height = 175;
    system_info_s.user_info.user_moving_target = 8000;
    system_info_s.user_info.user_weight = 60;         
}

/****************************************************************
** Function name:           Config_UserInfoCheck
** Descriptions:            �û���Ϣ�Լ�
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-08-24
*****************************************************************/
void Config_UserInfoCheck(void)
{
    if((system_info_s.user_info.user_age > 120) || 
//       (system_info_s.user_info.user_age == 0) || 
       (system_info_s.user_info.user_height > 250) ||
       (system_info_s.user_info.user_height == 0) ||
       (system_info_s.user_info.user_weight > 250) ||
       (system_info_s.user_info.user_weight == 0) ||
       (system_info_s.user_info.user_gender > 1))
    {
        Config_UserInfoInit();       
    }   
}

/****************************************************************
** Function name:           Config_BpTestModeInit
** Descriptions:            Ѫѹ����ģʽ��ʼ��
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-08-24
*****************************************************************/
void Config_BpTestModeInit(void)
{
    system_info_s.bp_test_mode.test_mode = 0;
    system_info_s.bp_test_mode.set_sp = 0;//120;
    system_info_s.bp_test_mode.set_dp = 0;//80;    
}

/****************************************************************
** Function name:           Config_BpTestModeCheck
** Descriptions:            Ѫѹ����ģʽ�Լ�
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-08-24
*****************************************************************/
void Config_BpTestModeCheck(void)
{
    if((system_info_s.bp_test_mode.test_mode > 1) ||
       (system_info_s.bp_test_mode.set_sp > 210) ||
       (system_info_s.bp_test_mode.set_dp >= 180) ||
       (system_info_s.bp_test_mode.set_dp < 46) ||
       (system_info_s.bp_test_mode.set_sp < 81) ||
       (system_info_s.bp_test_mode.set_dp+24 > system_info_s.bp_test_mode.set_sp))
    {
        Config_BpTestModeInit();    
    }   
}

/****************************************************************
** Function name:           Config_GestureLevelInit
** Descriptions:            ����ȼ�������ʼ��
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-08-24
*****************************************************************/
void Config_GestureLevelInit(void)
{
     system_info_s.gesture_level = 5;
}
/****************************************************************
** Function name:           Config_HeartCalibrateCheck
** Descriptions:            ����ȼ������Լ�
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-08-24
*****************************************************************/
void Config_GestureLevelCheck(void)
{
    if((system_info_s.gesture_level >= 10) ||system_info_s.gesture_level == 0 )
    {
        Config_GestureLevelInit();
    }   
}

/****************************************************************
** Function name:           Config_MessageEnableInit
** Descriptions:            ��Ϣ���Ϳ��ز�����ʼ��
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-08-24
*****************************************************************/
void Config_MessageEnableInit(void)
{      
    system_info_s.message_enable.call_enable = 0x01;
    system_info_s.message_enable.facebook_enable = 0x00;
    system_info_s.message_enable.qq_enable = 0x00;
    system_info_s.message_enable.sms_enable = 0x00;
    system_info_s.message_enable.twitter_enable = 0x00;
    system_info_s.message_enable.wechat_enable = 0x00;
    system_info_s.message_enable.whatsapp_enable = 0x00;     
    system_info_s.message_enable.resv = 0x00;
    
    system_info_s.message_enable.linkedin_enable = 0x00;
    system_info_s.message_enable.instagram_enable = 0x00;
    system_info_s.message_enable.line_enable = 0x00;
    system_info_s.message_enable.other_enable = 0x00;
    system_info_s.message_enable.skype_enable = 0x00;
    system_info_s.message_enable.snapchat_enable = 0x00;
    system_info_s.message_enable.gmail_enable = 0x00;
    system_info_s.message_enable.resv = 0x00;    
}
/****************************************************************
** Function name:           Config_MessageEnableCheck
** Descriptions:            ��Ϣ���Ϳ��ز����Լ�
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-08-24
*****************************************************************/
void Config_MessageEnableCheck(void)
{
    if(system_info_s.message_enable.resv != 0 || system_info_s.message_enable.resv != 0)
    {        
        Config_MessageEnableInit();            
    }   
}

/****************************************************************
** Function name:           Config_WearDetectInit
** Descriptions:            ������س�ʼ��
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-08-24
*****************************************************************/
void Config_WearDetectInit(void)
{
    system_info_s.wear_detect = 1;
}

/****************************************************************
** Function name:           Config_WearDetectCheck
** Descriptions:            ��������Լ�
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-08-24
*****************************************************************/
void Config_WearDetectCheck(void)
{
    if((system_info_s.wear_detect > 2) ||
       (system_info_s.wear_detect == 0)) //   system_info_s.wear_detect == 0��ʾ��֧��������
    {
        Config_WearDetectInit();
    }
}

/****************************************************************
** Function name:           Config_OutsitCfgInit
** Descriptions:            ������ʼ��
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-08-24
*****************************************************************/
void Config_OutsitCfgInit(void)
{
    system_info_s.outsit_cfg.on_off_flag = 0;
    system_info_s.outsit_cfg.start_time[0] = 14;
    system_info_s.outsit_cfg.start_time[1] = 0;
    system_info_s.outsit_cfg.end_time[0] = 18;
    system_info_s.outsit_cfg.end_time[1] = 0;
    system_info_s.outsit_cfg.threshold_value = 60;            
}

/****************************************************************
** Function name:           Config_OutsitCfgCheck
** Descriptions:            �����Լ�
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-08-24
*****************************************************************/
void Config_OutsitCfgCheck(void)
{
    uint16_t start_time = 0;
    uint16_t end_time = 0;
    
    start_time = system_info_s.outsit_cfg.start_time[0]*60 + system_info_s.outsit_cfg.start_time[1];
    end_time = system_info_s.outsit_cfg.end_time[0]*60 + system_info_s.outsit_cfg.end_time[1];   
    
    if((system_info_s.outsit_cfg.on_off_flag > 1) 
      ||(system_info_s.outsit_cfg.threshold_value < 30) 
      ||(system_info_s.outsit_cfg.threshold_value > 240) 
      || (start_time > 17 * 60 + 30)
      || (start_time < 8 * 60) 
      ||(end_time > 18 * 60) 
      ||(end_time < 8 * 60 + 30) 
      ||(start_time + system_info_s.outsit_cfg.threshold_value > end_time))
    {
        Config_OutsitCfgInit();        
    }
}

/****************************************************************
** Function name:           Config_HeartAlarmEnableInit
** Descriptions:            ���ʸ澯��ʼ��
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-08-24
*****************************************************************/
void Config_HeartAlarmEnableInit(void)
{
    system_info_s.heart_alarm_enable.heart_enable = 0;
    system_info_s.heart_alarm_enable.heart_hight = 115;
    system_info_s.heart_alarm_enable.heart_low = 52;    
}

/****************************************************************
** Function name:           Config_HeartAlarmEnableCheck
** Descriptions:            ���ʸ澯�Լ�
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-08-24
*****************************************************************/
void Config_HeartAlarmEnableCheck(void)
{
    if((system_info_s.heart_alarm_enable.heart_enable > 1) ||
       (system_info_s.heart_alarm_enable.heart_hight > 200) ||
       (system_info_s.heart_alarm_enable.heart_low > 200) ||
       (system_info_s.heart_alarm_enable.heart_hight < 70) ||
       (system_info_s.heart_alarm_enable.heart_low < 30) ||
       (system_info_s.heart_alarm_enable.heart_low > system_info_s.heart_alarm_enable.heart_hight))
    {
        Config_HeartAlarmEnableInit();  
    }
}

/****************************************************************
** Function name:           Config_HorizontalEnableInit
** Descriptions:            ��������ʼ��
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-08-24
*****************************************************************/
void Config_HorizontalEnableInit(void)
{
    system_info_s.horizontal_enable = 0;   
}

/****************************************************************
** Function name:           Config_HorizontalEnableCheck
** Descriptions:            �������Լ�
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-08-24
*****************************************************************/
void Config_HorizontalEnableCheck(void)
{
    if((system_info_s.horizontal_enable > 1))
    {
        Config_HorizontalEnableInit(); 
    }
}

/****************************************************************
** Function name:           Config_TurnwristConfigInit
** Descriptions:            ̧�ַ������� ��ʼ��
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-08-24
*****************************************************************/
void Config_TurnwristConfigInit(void)
{
    system_info_s.turnwrist_config.enable = 1;
    system_info_s.turnwrist_config.s_hour = 8;
    system_info_s.turnwrist_config.s_minute = 0;
    system_info_s.turnwrist_config.e_hour = 22; 
    system_info_s.turnwrist_config.e_minute = 0;            
}

/****************************************************************
** Function name:           Config_TurnwristConfigCheck
** Descriptions:            ̧�ַ������� �Լ�
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-08-24
*****************************************************************/
void Config_TurnwristConfigCheck(void)
{
    uint16_t start_time = 0;
    uint16_t end_time = 0;
    
    start_time = system_info_s.turnwrist_config.s_hour*60 + system_info_s.turnwrist_config.s_minute;
    end_time = system_info_s.turnwrist_config.e_hour*60 + system_info_s.turnwrist_config.e_minute;
    
    if((system_info_s.turnwrist_config.enable > 1) ||
       (start_time == end_time) ||
       (system_info_s.turnwrist_config.s_hour > 23) ||
       (system_info_s.turnwrist_config.e_hour > 23) ||
       (system_info_s.turnwrist_config.s_minute > 59) ||
       (system_info_s.turnwrist_config.e_minute > 59))
    {
        Config_TurnwristConfigInit();    
    }
}

/****************************************************************
** Function name:           Config_BrightnessLeveInit
** Descriptions:            ��Ļ���ȳ�ʼ��
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-08-24
*****************************************************************/
void Config_BrightnessLeveInit(void)
{
    system_info_s.brightness_leve.leve = 2;
    system_info_s.brightness_leve.setleve = 1;
    system_info_s.brightness_leve.s_hour = 22;
    system_info_s.brightness_leve.s_minute = 0;
    system_info_s.brightness_leve.e_hour = 8;
    system_info_s.brightness_leve.e_minute = 0;          
}


/****************************************************************
** Function name:           Config_BrightnessLeveCheck
** Descriptions:            ��Ļ�����Լ�
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-08-24
*****************************************************************/
void Config_BrightnessLeveCheck(void)
{
    uint16_t start_time = 0;
    uint16_t end_time = 0;
    
    start_time = system_info_s.brightness_leve.s_hour*60 + system_info_s.brightness_leve.s_minute;
    end_time = system_info_s.brightness_leve.e_hour*60 + system_info_s.brightness_leve.e_minute;
    
    if((system_info_s.brightness_leve.leve > 8) ||
       (system_info_s.brightness_leve.leve == 0) ||
       (system_info_s.brightness_leve.setleve > 8) ||
       (system_info_s.brightness_leve.setleve == 0) ||
       (start_time == end_time) ||
       (system_info_s.brightness_leve.s_hour > 23) ||
       (system_info_s.brightness_leve.e_hour > 23) ||
       (system_info_s.brightness_leve.s_minute > 59) ||
       (system_info_s.brightness_leve.e_minute > 59))
    {
        Config_BrightnessLeveInit();       
    }
}

/****************************************************************
** Function name:           Config_LostPhoneInit
** Descriptions:            ���ֻ����س�ʼ��
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-08-24
*****************************************************************/
void Config_LostPhoneInit(void)
{
    system_info_s.lost_phone = 2;       
}

/****************************************************************
** Function name:           Config_LostPhoneCheck
** Descriptions:            ���ֻ������Լ�
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-08-24
*****************************************************************/
void Config_LostPhoneCheck(void)
{ 
    if((system_info_s.lost_phone > 2) || 
        (system_info_s.lost_phone == 0))
    {
        Config_LostPhoneInit();       
    }
}

/****************************************************************
** Function name:           Config_CountDownInit
** Descriptions:            ����ʱ���ó�ʼ��
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-08-24
*****************************************************************/
void Config_CountDownInit(void)
{
    memset(&system_info_s.countdown_config, 0x00, sizeof(countdown_config_t));
}

/****************************************************************
** Function name:           Config_CountDownCheck
** Descriptions:            ����ʱ�����Լ�
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-08-24
*****************************************************************/
void Config_CountDownCheck(void)
{ 
    if((system_info_s.countdown_config.countdown_second > 24*60*60) || 
        (system_info_s.countdown_config.type >= 2) ||  
        (system_info_s.countdown_config.id == 0xFF)||  
        (system_info_s.countdown_config.state == 0xFF))   
    {
        Config_CountDownInit();       
    }
}

/****************************************************************
** Function name:           Config_LanguageInit
** Descriptions:            ϵͳ��������
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-08-24
*****************************************************************/
void Config_LanguageInit(void)
{
    system_info_s.language = CONFIG_LANGUAGE_ENAGLISH;
}

/****************************************************************
** Function name:           Config_LanguageCheck
** Descriptions:            ϵͳ��������
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-08-24
*****************************************************************/
void Config_LanguageCheck(void)
{ 
    if(system_info_s.language == 0 || system_info_s.language > CONFIG_LANGUAGE_TRADITIONAL)   
    {
        Config_LanguageInit();       
    }
}

/****************************************************************
** Function name:           Config_MovementPatternsInit
** Descriptions:            �˶�ģʽ����
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-08-24
*****************************************************************/
void Config_MovementPatternsInit(void)
{
    system_info_s.movement_max_puse_time = 60;
}

/****************************************************************
** Function name:           Config_MovementPatternsCheck
** Descriptions:            �˶�ģʽ����
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-08-24
*****************************************************************/
void Config_MovementPatternsCheck(void)
{ 
    if(system_info_s.movement_max_puse_time < 3 || system_info_s.movement_max_puse_time > 120)   
    {
        Config_MovementPatternsInit(); 
    }
}

/****************************************************************
** Function name:           Config_UiStyleInit
** Descriptions:            UI�������
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-08-24
*****************************************************************/
void Config_UiStyleInit(void)
{
    system_info_s.ui_style = 0;
}

/****************************************************************
** Function name:           Config_UiStyleCheck
** Descriptions:            UI�������
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-08-24
*****************************************************************/
void Config_UiStyleCheck(void)
{ 
    if(system_info_s.ui_style >= FUNCTION_SUPPORT_UI_STYLE)   
    {
        Config_UiStyleInit();       
    }
}


/****************************************************************
** Function name:           Config_LowEnergeInit
** Descriptions:            �͹������ò�����ʼ��
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              ZJH              
** Created Date:            2019-07-24
*****************************************************************/
void Config_LowEnergeInit(void)
{
    low_energe_protocol_data.state = FUNCTION_SUPPORT_LOW_ENERGE;     //�͹��Ŀ���ģʽ����
    low_energe_protocol_data.notice_time_value = 8;   //��Ϣ��������ʱ�� 8*0.125 = 1
	low_energe_protocol_data.posture_time_value = 10;   //��������ʱ�� 10*0.125 = 1.25
	low_energe_protocol_data.normal_light_time_value = 12;// ����������ʱ��
	low_energe_protocol_data.motor_delay_time_value = 5;// ����ʱ����(��Ϣ����) 
	low_energe_protocol_data.moter_time_value = 5;// �����ʱ��(��Ϣ����)  
}
    
/****************************************************************
** Function name:           Config_LowEnergeCheck
** Descriptions:            �͹������ò������
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              ZJH              
** Created Date:            2019-07-24
*****************************************************************/
uint8_t  Config_LowEnergeCheck(void)
{
    if( (low_energe_protocol_data.state > 2 ) ||
        (low_energe_protocol_data.notice_time_value < 1) ||
        (low_energe_protocol_data.notice_time_value > 64) ||
        (low_energe_protocol_data.posture_time_value < 1) ||
        (low_energe_protocol_data.posture_time_value > 64) ||
        (low_energe_protocol_data.normal_light_time_value < 1) ||
        (low_energe_protocol_data.normal_light_time_value > 64) ||
        (low_energe_protocol_data.motor_delay_time_value > 10) ||
        (low_energe_protocol_data.moter_time_value < 1) ||
        (low_energe_protocol_data.moter_time_value > 10) )
    {
        return false;
    }
    
    return true;
}



/****************************************************************
** Function name:           Config_SystemInfoInit
** Descriptions:            ϵͳ������Ϣ��ʼ��
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-03-22
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Config_SystemInfoInit(void)
{
    memset(&system_info_s, 0x00, sizeof(system_info_t));
    
    Config_PasswordNumInit();
    Config_UserInfoInit(); 
    Config_BpTestModeInit();
    Config_GestureLevelInit();
    Config_MessageEnableInit();   
    Config_WearDetectInit();
    Config_OutsitCfgInit();  
    Config_HeartAlarmEnableInit(); 
    Config_HorizontalEnableInit(); 
    Config_TurnwristConfigInit();
    Config_BrightnessLeveInit();
    Config_LostPhoneInit();
    Config_CountDownInit();  
    Config_MovementPatternsInit();     
    Config_LanguageInit();  
    Config_UiStyleInit();
}

/****************************************************************
** Function name:           Config_SystemSwitchInit
** Descriptions:            ���ֿ��س�ʼ��
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-03-22
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Config_SystemSwitchInit(void)
{
    memset(&system_switch_s, 0x00, sizeof(system_switch_t));
    
    system_switch_s.blood_oxygen = FUNCTION_SUPPORT_BO;                      //Ѫ��  
    system_switch_s.blood_pressure = FUNCTION_SUPPORT_BP;                    //Ѫѹ
    system_switch_s.distance_unit = FUNCTION_SUPPORT_DISTANCE;               //����Ӣ��
    system_switch_s.heart = FUNCTION_SUPPORT_HEART;                          //����
    system_switch_s.heart_bp_bo_broadcasts = FUNCTION_SUPPORT_HBO_BROADCAST;   //����Ѫ��Ѫѹ���� 
    system_switch_s.sport_broadcasts = FUNCTION_SUPPORT_SP_BROADCAST;         //�˶���������
    system_switch_s.time_formats = FUNCTION_SUPPORT_TIME_FORMATS;             //ʱ����ʽ 
    system_switch_s.hrv = FUNCTION_HRV;
    system_switch_s.call = FUNCTION_CALL_ANSWERING;
    system_switch_s.ble_disconnect_remind = FUNCTION_BLE_DISCONNECT_REMIND;
	system_switch_s.find_phone = FUNCTION_SUPPORT_FIND_PHONE;			// JSH.20181110.���������������������󣬲����ֻ�������ʧ
	system_switch_s.stopwatch = FUNCTION_SUPPORT_STOPWATCH;				// JSH.20181110.�����������������������������ʧ
}

/****************************************************************
** Function name:           Config_SystemParamCheck
** Descriptions:            ϵͳ�����Լ�
** input parameters:        ��
** output parameters:       ��   
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-03-22
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Config_SystemParamCheck(void)
{
    if(Util_CheckBufIsZero((void *)&system_info_s, sizeof(system_info_t)))
    {   
        Config_SystemInfoInit();
    }
    else
    {
        Config_PasswordNumCheck();
        Config_UserInfoCheck(); 
        Config_MessageEnableCheck();
        Config_BpTestModeCheck();
        Config_GestureLevelCheck();
        Config_MessageEnableCheck();   
        Config_WearDetectCheck();
        Config_OutsitCfgCheck();  
        Config_HeartAlarmEnableCheck(); 
        Config_HorizontalEnableCheck(); 
        Config_TurnwristConfigCheck();
        Config_BrightnessLeveCheck(); 
        Config_LostPhoneCheck();
        Config_CountDownCheck(); 
        Config_MovementPatternsCheck();     
        Config_LanguageCheck(); 
        Config_UiStyleCheck(); 
    }
}

/****************************************************************
** Function name:           Config_SystemSwitchCheck
** Descriptions:            ���ֿ����Լ�
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-03-22
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Config_SystemSwitchCheck(void)
{
    if(system_switch_s.distance_unit > 2 || (system_switch_s.distance_unit == 0 && FUNCTION_SUPPORT_DISTANCE != 0))
    {
        system_switch_s.distance_unit = FUNCTION_SUPPORT_DISTANCE;
    }   
    if(system_switch_s.time_formats > 2 || (system_switch_s.time_formats == 0 && FUNCTION_SUPPORT_TIME_FORMATS != 0))
    {
        system_switch_s.time_formats = FUNCTION_SUPPORT_TIME_FORMATS;
    }       
    if(system_switch_s.heart > 2 || (system_switch_s.heart == 0 && FUNCTION_SUPPORT_HEART != 0))
    {
        system_switch_s.heart = FUNCTION_SUPPORT_HEART;
    }        
    if(system_switch_s.blood_pressure > 2 || (system_switch_s.blood_pressure == 0 && FUNCTION_SUPPORT_BP != 0))
    {
        system_switch_s.blood_pressure = FUNCTION_SUPPORT_BP;
    }    
    
	//if(system_switch_s.blood_oxygen > 2 || (system_switch_s.blood_oxygen == 0 && FUNCTION_SUPPORT_BP != 0))
    {
        system_switch_s.blood_oxygen = FUNCTION_SUPPORT_BO;
    }    
    //if(system_switch_s.sport_broadcasts > 2 || (system_switch_s.sport_broadcasts == 0 && FUNCTION_SUPPORT_BP != 0))
    {
        system_switch_s.sport_broadcasts = FUNCTION_SUPPORT_SP_BROADCAST;
    } 
    //if(system_switch_s.heart_bp_bo_broadcasts > 2 || (system_switch_s.heart_bp_bo_broadcasts == 0 && FUNCTION_SUPPORT_HBO_BROADCAST != 0))
    {
        system_switch_s.heart_bp_bo_broadcasts = FUNCTION_SUPPORT_HBO_BROADCAST;
    } 
    if(system_switch_s.find_phone > 2 || (system_switch_s.find_phone == 0 && FUNCTION_SUPPORT_FIND_PHONE != 0))
    {
        system_switch_s.find_phone = FUNCTION_SUPPORT_FIND_PHONE;
    }  
    if(system_switch_s.stopwatch > 2 || (system_switch_s.stopwatch == 0 && FUNCTION_SUPPORT_STOPWATCH != 0))
    {
        system_switch_s.stopwatch = FUNCTION_SUPPORT_STOPWATCH;
    } 
    if(system_switch_s.led_gear > 2 || (system_switch_s.led_gear == 0 && FUNCTION_LED_GEAR != 0))
    {
        system_switch_s.led_gear = FUNCTION_LED_GEAR;
    }
    
    system_switch_s.hrv = 0;
    system_switch_s.call = 0;
    
    if(system_switch_s.ble_disconnect_remind > 2 
      || (system_switch_s.ble_disconnect_remind == 0 && FUNCTION_BLE_DISCONNECT_REMIND != 0))
    {
        system_switch_s.ble_disconnect_remind = FUNCTION_BLE_DISCONNECT_REMIND;
    } 
}
