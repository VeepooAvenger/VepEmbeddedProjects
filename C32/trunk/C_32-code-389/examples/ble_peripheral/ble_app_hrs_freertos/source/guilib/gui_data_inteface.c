/***************************Copyright(c)***********************************
**                       Veepoo Tech
**                  COPYRIGHT (C) 2016-2019
**-----------File Info-----------------------------------------------------
** File        name:     gui_data_inteface.c
** Last modified Date:   2017-07-28
** Last SYS_VERSION:     0.0.0
** Descriptions:            
**-------------------------------------------------------------------------
** Created  by:          WJ
** Created  date:        2017-07-28
** SYS_VERSION:          0.0.0
** Descriptions:         
**-------------------------------------------------------------------------
** Modified by:				
** Modified date:  			          
** SYS_VERSION:                 
** Descriptions:                        
***************************************************************************/
#include "stdint.h"
#include "string.h"
#include "gui_data_inteface.h"
#include "app_type.h"
#include "pedometer.h"
#include "sleep.h"
#include "blood_pressure.h"
#include "heart.h"
#include "battery.h"
#include "alarm_event.h"
#include "phone_notify_event.h"
#include "Phone_notify_roll_event.h"
#include "phone_camera_event.h"
#include "hal_acc_calculate.h"
#include "app_hid.h"
#include "movement_patterns.h"
#include "bsp_button.h"
#include "pedometer.h"
#include "physiological_period.h"
#include "ble_disconnect_remind.h"
/*********************************************************************
* Macro Definition
*/

/*********************************************************************
* Global Variables
*/  

extern system_flag_t system_flag_s;
extern system_switch_t system_switch_s;
extern system_info_t system_info_s;  

extern unread_message_t unread_message_s;

extern phys_remind_info_t phys_remind_info_s;   //Ů�Թ���������Ϣ

/*********************************************************************
* local functions
*/

/****************************************************************
** Function name:           Gui_DataSystemIsTestMode
** Descriptions:            ϵͳ���ڲ���ģʽ uint8_t Gui_DataSystemIsTestMode(void)
** input parameters:        
** output parameters:       
** Returned value:          ����0:����ģʽ, 1:����ģʽ
** Created by:              WJ              
** Created Date:            2017-08-01
*****************************************************************/ 
uint8_t Gui_DataSystemIsTestMode(void)
{
    if(system_flag_s.sign_test)
    {
        return 1;
    }
    return 0;
}

/****************************************************************
** Function name:           Gui_DataSystemIsPoweOn
** Descriptions:            ϵͳ�Ƿ񿪻�
** input parameters:        
** output parameters:       
** Returned value:          ����0:�ػ�״̬, 1:����״̬
** Created by:              WJ              
** Created Date:            2017-07-31
*****************************************************************/ 
uint8_t Gui_DataSystemIsPoweOn(void)
{
    if(system_flag_s.sign_off)
    {
        return 0;
    }
    return 1;
}

/****************************************************************
** Function name:           Gui_DataSystemToPoweOn
** Descriptions:            ϵͳ����
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-07-31
*****************************************************************/ 
void Gui_DataSystemToPoweOn(void)
{
    App_AddEvent(APP_BOOT_EVENT);
}

/****************************************************************
** Function name:           Gui_DataSystemToPoweOff
** Descriptions:            ϵͳ�ػ�
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-07-31
*****************************************************************/ 
void Gui_DataSystemToPoweOff(void)
{
    App_AddEvent(APP_SHUTDOWN_EVENT);
}

/****************************************************************
** Function name:           Gui_DataGetSystemLanguage
** Descriptions:            ��ȡϵͳ����
** input parameters:        
** output parameters:        
** Returned value:          ����ϵͳ����
                            GUI_LANGUAGE_CHINESE:����
                            GUI_LANGUAGE_ENAGLISH:Ӣ��
** Created by:              WJ              
** Created Date:            2017-08-10
*****************************************************************/ 
GUI_LANGUAGE_E Gui_DataGetSystemLanguage(void)
{
    if(system_info_s.language == CONFIG_LANGUAGE_CHINESE)
    {
        return GUI_LANGUAGE_CHINESE;
    }
    return GUI_LANGUAGE_ENAGLISH;
}

/****************************************************************
** Function name:           Gui_DataGetUiStyle
** Descriptions:            ��ȡUI������
** input parameters:        
** output parameters:        
** Returned value:          ����UI������
** Created by:              WJ              
** Created Date:            2017-08-10
*****************************************************************/ 
uint8_t Gui_DataGetUiStyle(void)
{
    if(system_info_s.ui_style < FUNCTION_SUPPORT_UI_STYLE)
    {
        return system_info_s.ui_style;
    }
    return 0;
}

/****************************************************************
** Function name:           Gui_DataTimeFormatIs24
** Descriptions:            ʱ��ĸ�ʽ�ǲ���24Сʱ��ʽ
** input parameters:        
** output parameters:       
** Returned value:          ����0:12Сʱ��, 1:24Сʱ��
** Created by:              WJ              
** Created Date:            2017-07-28
*****************************************************************/ 
uint8_t Gui_DataTimeFormatIs24(void)
{
    return (system_switch_s.time_formats != 0X02);
}

/****************************************************************
** Function name:           Gui_DataTimeFormatDisplay
** Descriptions:            ��ȡʱ����ʽ��ʾ
** input parameters:        
** output parameters:       
** Returned value:          ����0:24Сʱ��, 1:12Сʱ��AM,2:12Сʱ��PM
** Created by:              WJ              
** Created Date:            2017-07-28
*****************************************************************/ 
uint8_t Gui_DataTimeFormatDisplay(void)
{
    if(system_switch_s.time_formats == 0x02)
    {
        if(clock->hour < 12)
        {
            return GUI_TIME_FORMAT_AM;
        }
        else
        {
            return GUI_TIME_FORMAT_PM;
        }
    }
    
    return GUI_TIME_FORMAT_STANDARD;
}


/****************************************************************
** Function name:           Gui_DataTimeFormatIs24
** Descriptions:            ʱ��ĸ�ʽ�ǲ���24Сʱ��ʽ
** input parameters:        
** output parameters:       
** Returned value:          ����0:12Сʱ��, 1:24Сʱ��
** Created by:              WJ              
** Created Date:            2017-07-28
*****************************************************************/ 
uint8_t Gui_DataGetSystemTimeHour(void)
{
	uint8_t display_hour;
	
    display_hour = clock->hour;    
	if(Gui_DataTimeFormatIs24() != 1)
	{
		if(display_hour >= 12)
        {
			display_hour = display_hour - 12;
        }
		if(display_hour == 0)
        {
			display_hour = 12;
        }
	}	
	return display_hour;
}

/****************************************************************
** Function name:           Gui_DataGetSystemTimeMinute
** Descriptions:            ʱ��ĸ�ʽ�ǲ���24Сʱ��ʽ
** input parameters:        
** output parameters:       
** Returned value:          ����ϵͳʱ�����
** Created by:              WJ              
** Created Date:            2017-07-28
*****************************************************************/ 
uint8_t Gui_DataGetSystemTimeMinute(void)
{
	return clock->minutes;
}

/****************************************************************
** Function name:           Gui_DataGetSystemTimeSecond
** Descriptions:            ʱ��ĸ�ʽ�ǲ���24Сʱ��ʽ
** input parameters:        
** output parameters:       
** Returned value:          ����ϵͳ��ǰ����
** Created by:              WJ              
** Created Date:            2017-07-28
*****************************************************************/ 
uint8_t Gui_DataGetSystemTimeSecond(void)
{
	return clock->seconds;
}

/****************************************************************
** Function name:           Gui_DataGetSystemTimeSecond
** Descriptions:            ʱ��ĸ�ʽ�ǲ���24Сʱ��ʽ
** input parameters:        
** output parameters:       
** Returned value:          ����ϵͳ��ǰ����(1~7)
** Created by:              WJ              
** Created Date:            2017-07-28
*****************************************************************/ 
uint8_t Gui_DataGetSystemTimeWeek(void)
{
	return clock->week;
}

/****************************************************************
** Function name:           Gui_DataGetSystemTime
** Descriptions:            ��ȡʱ��
** input parameters:        
** output parameters:       hour:ʱ,minute:��,second:��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-07-28
*****************************************************************/ 
void Gui_DataGetSystemTime(uint8_t *hour, uint8_t *minute, uint8_t *second)
{
    if(hour != NULL)
    {
         *hour = clock->hour;
    }
	if(minute != NULL)
    {
        *minute = clock->minutes;
    }
    if(second != NULL)
    {
        *second = clock->seconds;
    }
}

/****************************************************************
** Function name:           Gui_DataGetSystemTimeDate
** Descriptions:            ��ȡ����
** input parameters:        
** output parameters:       year:��,month:��,day:��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-07-28
*****************************************************************/ 
void Gui_DataGetSystemTimeDate(uint8_t *year, uint8_t *month, uint8_t *day)
{
    if(year != NULL)
    {
         *year = clock->year;
    }
	if(month != NULL)
    {
        *month = clock->month;
    }
    if(day != NULL)
    {
        *day = clock->day;
    }
}

/****************************************************************
** Function name:           Gui_DataGetAllSteps
** Descriptions:            ��ȡ���мƲ���
** input parameters:        
** output parameters:       
** Returned value:          ���ص�ǰ�Ʋ�����
** Created by:              WJ              
** Created Date:            2017-07-28
*****************************************************************/ 
uint32_t Gui_DataGetAllSteps(void)
{
    
    sport_information_info_t gsii_sport_info_s = {0x00, };
    
    Get_SportInformationInfo(&gsii_sport_info_s); 
    return gsii_sport_info_s.posi_steps;//All_StepGet();
}

/****************************************************************
** Function name:           Gui_DataGetDistanceValue
** Descriptions:            ��ȡ������\ǧ��\Ӣ��
** input parameters:        
** output parameters:       ����������(ȡ��)�����ǧ�� ������С�������λ����Ӣ�mile����λ��Чλ����
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-07-28
*****************************************************************/ 
void Gui_DataGetDistanceValue(uint32_t *meter, float *kilometer, float *mile)
{
    float temp = 0; 
    sport_information_info_t gsii_sport_info_s = {0x00, };
    
    Get_SportInformationInfo(&gsii_sport_info_s); 

    if(meter != NULL)
    {
        *meter  = gsii_sport_info_s.posi_distance;//ȡ��
    }
    temp = ((uint32_t)(((gsii_sport_info_s.posi_distance)/1000.0f)*1000))/1000.0f;//������λС����
    if(kilometer != NULL)
    {
        *kilometer = temp;
    }
    if(mile != NULL)
    {
        *mile = temp * (0.62138f);
    }
}

/****************************************************************
** Function name:           Gui_DataTimeFormatIs24
** Descriptions:            ʱ��ĸ�ʽ�ǲ���24Сʱ��ʽ
** input parameters:        
** output parameters:       
** Returned value:          ����0:Ӣ��, 1:ǧ��
** Created by:              WJ              
** Created Date:            2017-07-28
*****************************************************************/ 
uint8_t Gui_DataDistanceIsKM(void)
{
    return (system_switch_s.distance_unit != 0x02);
}

/****************************************************************
** Function name:           Gui_DataGetCalorie
** Descriptions:            ��ȡ��·��ֵ
** input parameters:        
** output parameters:       
** Returned value:          ���ؿ�·��
** Created by:              WJ              
** Created Date:            2017-07-28
*****************************************************************/ 
float Gui_DataGetCalorie(void)
{    
    sport_information_info_t gsii_sport_info_s = {0x00, };
    
    Get_SportInformationInfo(&gsii_sport_info_s);
    
    return (gsii_sport_info_s.posi_cal/1000);    
}

/****************************************************************
** Function name:           Gui_DataGetSportsToMarkProgress
** Descriptions:            ��ȡ�˶�������
** input parameters:        
** output parameters:       
** Returned value:          ���ؽ���(0~100)
** Created by:              WJ              
** Created Date:            2017-07-28
*****************************************************************/ 
uint8_t Gui_DataGetSportsToMarkProgress(void)
{
    uint32_t step_input = 0;
    uint16_t step_mark = 0;
      
    step_mark = system_info_s.user_info.user_moving_target;
    step_input = Gui_DataGetAllSteps();
    
    if(step_input >= step_mark)
    {
        return 100;
    }    
    return (step_input*100)/step_mark;    
}

/****************************************************************
** Function name:           Gui_DataGetSportsToMarkProgress
** Descriptions:            ��ȡ˯��ʱ��(����)
** input parameters:        
** output parameters:       
** Returned value:          ����˯��ʱ��(����)
** Created by:              WJ              
** Created Date:            2017-07-28
*****************************************************************/ 
uint16_t Gui_DataGetSleepTime(void)
{  
    uint16_t sleep_time = 0;
    
    sleep_time = Sleep_GetTime();
    if(sleep_time == 0)
    {
        Gui_DataSleepToRecalculate();
    }
    
    return sleep_time;    
}

/****************************************************************
** Function name:           Gui_DataOpenBPOpreation
** Descriptions:            ��Ѫѹ����
** input parameters:        
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-07-28
*****************************************************************/ 
void Gui_DataOpenBPOpreation(void)
{  
    App_StopMeasuringHandler(TEST_CLOSE_ALL);
    Bp_Open();
    system_flag_s.heart_led_status = true;
    system_flag_s.measuring_status = BUTTON_BLOOD_PRESSURE_STATUS;  
}

/****************************************************************
** Function name:           Gui_DataOpenBPOpreation
** Descriptions:            �ر�Ѫѹ����
** input parameters:        
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-07-28
*****************************************************************/ 
void Gui_DataCloseBPOpreation(void)
{  
    App_StopMeasuringHandler(TEST_CLOSE_BTN);
}

/****************************************************************
** Function name:           Gui_DataGetBPdata
** Descriptions:            ��ȡѪѹ����(�ߵ�ѹ������ģʽ������״̬)
** input parameters:        
** output parameters:       bp_sp:��ѹ,bp_dp:��ѹ,
                            test_mode:����ģʽ,test_state:����״̬
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-07-28
*****************************************************************/ 
void Gui_DataGetBPdata(uint8_t *bp_sp, uint8_t *bp_dp, uint8_t *test_mode, uint8_t *test_state)
{  
    test_mode_t *bp_test_mode = NULL;
    
    *test_state = Bp_GetFlagState();
    if(bp_sp != NULL && bp_dp != NULL)
    {
        if(test_state != NULL && *test_state == 1)
        {
            Bp_GetTestResult(bp_sp, bp_dp);    //��ȡѪѹ����
        }
        else
        {          
            *bp_sp = 0;
            *bp_dp = 0;
        }
    }
    if(test_mode != NULL)
    {
        bp_test_mode = Bp_GetBpMode(); //��ȡѪѹ����ģʽ
        *test_mode = bp_test_mode->test_mode;     
    }
}

/****************************************************************
** Function name:           Gui_DataGetHeart
** Descriptions:            ��ȡ��ǰ���ʽ��
** input parameters:        
** output parameters:       ��
** Returned value:          ���ص�ǰ���ʽ��
** Created by:              WJ              
** Created Date:            2017-07-28
*****************************************************************/ 
uint8_t Gui_DataGetHeart(void)
{  
    return Heart_Test_Value_Get();
}

/****************************************************************
** Function name:           Gui_DataCloseHeartOpreation
** Descriptions:            �ر����ʲ���
** input parameters:        
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-07-28
*****************************************************************/ 
void Gui_DataCloseHeartOpreation(void)
{    
    App_StopMeasuringHandler(TEST_CLOSE_BTN);
}

/****************************************************************
** Function name:           Gui_DataOpenHeartOpreation
** Descriptions:            �����ʲ���
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-07-28
*****************************************************************/ 
void Gui_DataOpenHeartOpreation(void)
{  
    if(system_flag_s.measuring_status == APP_HEARTING_STATUS)
    {
        //���app�����ѽ���������Ҫ����,ֻ��Ҫ���ģʽ
        system_flag_s.app_heart_status = 1;  
    }
    else
    {
        App_StopMeasuringHandler(TEST_CLOSE_ALL);
        Heart_Open();
    }
    system_flag_s.heart_led_status = true;
    system_flag_s.measuring_status = BUTTON_HEARTING_STATUS; 
}


/****************************************************************
** Function name:           Gui_DataCloseHeartOrBpOpreation
** Descriptions:            �ر�����\Ѫѹ����
** input parameters:        
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-07-28
*****************************************************************/ 
void Gui_DataCloseHeartOrBpOpreation(void)
{    
    App_StopMeasuringHandler(TEST_CLOSE_BTN);
}


/****************************************************************
** Function name:           Gui_DataGetWearDectState
** Descriptions:            ��ȡ���״̬
** input parameters:        ��
** output parameters:       ��
** Returned value:          ����0�������ͨ�����Թرգ�1��������
** Created by:              WJ              
** Created Date:            2017-08-12
*****************************************************************/ 
uint8_t Gui_DataGetWearDectState(void)
{  
    if(system_flag_s.heart_led_status == false || system_flag_s.measuring_status == EVENT_NULL)
    {
        return 0;
    }
    return 1;
}

/****************************************************************
** Function name:           Gui_DataGetBatteryLevelAndState
** Descriptions:            ��ȡ��صȼ���״̬
** input parameters:        
** output parameters:       level:�����ȼ�
** Returned value:          ���ص�س��״̬,BATTERRY_IS_NORMAL:�ŵ�
                            BATTERRY_IS_CHARGING:�����
                            BATTERRY_IS_FULL:����
** Created by:              WJ              
** Created Date:            2017-07-28
*****************************************************************/ 
uint8_t Gui_DataGetBatteryLevelAndState(uint8_t *level)
{
    uint8_t charge_state = BATTERRY_IS_NORMAL;
    uint8_t battery_level = 0;
    
    if(level != NULL)
    {
        battery_level = Battery_GetGrade();       
        if(battery_level == BATTERY_LEVEL_LOW)
        {
            battery_level = 0;
        }  
        *level = battery_level;      
    }
    //YX.20180712.ʹ�õ�״̬��������ͬ������ֱ��ʹ������״̬
    if(system_flag_s.power_state == POWER_FULL)
    {
        charge_state = BATTERRY_IS_FULL; //����
    }
    else if(system_flag_s.power_state == POWER_CHARGE)
    {
        charge_state = BATTERRY_IS_CHARGING; //�����
    }
    else
    {
        charge_state = BATTERRY_IS_NORMAL;   //�ŵ�
    }
    
    if(charge_state == BATTERRY_IS_CHARGING)
    {
        charge_state = Battery_GetChargingFullState();
        if(charge_state == 0)
        {
            charge_state = BATTERRY_IS_CHARGING;
        }
    }
    return charge_state;
}

/****************************************************************
** Function name:           Gui_DataBatteryIsLowPower
** Descriptions:            ��ص͵�
** input parameters:        
** output parameters:       ��
** Returned value:          BATTERRY_IS_LOW_POWER:�͵�,
                            BATTERRY_NOT_LOW_POWER:�ǵ͵�
                            BATTERRY_IS_LOW_TO_SHUT:�͵�ػ�
** Created by:              WJ              
** Created Date:            2017-07-28
*****************************************************************/ 
uint8_t Gui_DataBatteryIsLowPower(void)
{
    if(Battery_IsLowToShut())
    {
        return BATTERRY_IS_LOW_TO_SHUT;
    }
    else if(Battery_IsLow())
    {
        return BATTERRY_IS_LOW_POWER;
    }
    return BATTERRY_NOT_LOW_POWER;
}

/****************************************************************
** Function name:           Gui_DataFindPhonePageIsEnable
** Descriptions:            ���ֻ������Ƿ���ʾ
** input parameters:        
** output parameters:       �� 
** Returned value:          ����0:����ʾҳ��,1:��ʾҳ��
** Created by:              WJ              
** Created Date:            2017-08-01
*****************************************************************/ 
uint8_t Gui_DataFindPhonePageIsEnable(void)
{    
    if(system_switch_s.find_phone == 0x01)
    {
        return 1;
    }
    return 0;
}

/****************************************************************
** Function name:           Gui_DataFindSendData
** Descriptions:            �ֻ����ҷ�������
** input parameters:        
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-07-28
**----------------------------------------------------------------
*****************************************************************/ 
void Gui_DataFindSendData(void)
{
    uint8_t report[20] = {0xB5,0};
    
    Ble_SendDataViaBat( report, sizeof(report));
}

/****************************************************************
** Function name:           Gui_DataGetBleState
** Descriptions:            ��ȡ��������״̬
** input parameters:        
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-07-28
*****************************************************************/ 
uint8_t Gui_DataGetBleState(void)
{
     
    return system_flag_s.ble_connect;
}

/****************************************************************
** Function name:           Gui_DataGetStopWatchSecond
** Descriptions:            ��ȡ������100����
** input parameters:        
** output parameters:       total_second:�ܼ�����, *millisecond:100����
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-07-28
*****************************************************************/ 
void Gui_DataGetStopWatchSecond(uint16_t *total_second, uint8_t *millisecond)
{
    if(total_second != NULL)
    {
        *total_second = Watch_GetTotalSecond();
    }
    if(millisecond != NULL)
    {
        *millisecond = Watch_Get100Millisecond();
    }  
}

/****************************************************************
** Function name:           Gui_DataStopWatchIsBusy
** Descriptions:            ��ȡ���״̬
** input parameters:        
** output parameters:       �� 
** Returned value:          ����STOP_WATCH_IDLE:����,STOP_WATCH_IS_BUSYæµ(��������)
** Created by:              WJ              
** Created Date:            2017-07-28
*****************************************************************/ 
uint8_t Gui_DataStopWatchIsBusy(void)
{
    if(Watch_IsBusy() == STOPWATCH_IDE)
    {
        return STOP_WATCH_IDLE;
    }
    return STOP_WATCH_IS_BUSY;
}

/****************************************************************
** Function name:           Gui_DataToggleStopWatch
** Descriptions:            ��ת���״̬����ʼ����ͣ
** input parameters:        ��
** output parameters:       �� 
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-07-28
*****************************************************************/ 
void Gui_DataToggleStopWatch(void)
{
    Watch_Toggle();
}

/****************************************************************
** Function name:           Gui_DataSetStopWatchToReady
** Descriptions:            �������Ϊ����״̬
** input parameters:        ��
** output parameters:       �� 
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-07-31
*****************************************************************/ 
void Gui_DataSetStopWatchToReady(void)
{
    Watch_Init();
    Watch_SetReady();
}

/****************************************************************
** Function name:           Gui_DataExitStopWatch
** Descriptions:            �˳����
** input parameters:        ��
** output parameters:       �� 
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-07-31
*****************************************************************/ 
void Gui_DataExitStopWatch(void)
{
    Watch_Init();
}

/****************************************************************
** Function name:           Gui_DataStopWatchPageIsEnable
** Descriptions:            �������Ƿ���ʾ
** input parameters:        
** output parameters:       �� 
** Returned value:          ����0:����ʾҳ��,1:��ʾҳ��
** Created by:              WJ              
** Created Date:            2017-08-01
*****************************************************************/ 
uint8_t Gui_DataStopWatchPageIsEnable(void)
{    
    if(system_switch_s.stopwatch == 0x01)
    {
        return 1;
    }
    return 0;
}

/****************************************************************
** Function name:           Gui_DataCountDownIsBusy
** Descriptions:            ��ȡ����ʱ״̬
** input parameters:        
** output parameters:       �� 
** Returned value:          ����STOP_COUNT_DOWN_IDLE:����,STOP_COUNT_DOWN_IS_BUSYæµ
** Created by:              WJ              
** Created Date:            2017-07-28
*****************************************************************/ 
uint8_t Gui_DataCountDownIsBusy(void)
{
    if(Countdown_GetState())
    {
        return COUNT_DOWN_IS_BUSY;
    }
    return COUNT_DOWN_IDLE;
}

/****************************************************************
** Function name:           Gui_DataCountDownPageIsEnable
** Descriptions:            ����ʱ�����Ƿ���ʾ
** input parameters:        
** output parameters:       �� 
** Returned value:          ����0:����ʾҳ��,1:��ʾҳ��
** Created by:              WJ              
** Created Date:            2017-08-01
*****************************************************************/ 
uint8_t Gui_DataCountDownPageIsEnable(void)
{    
    if(system_info_s.countdown_config.type != 1 && Countdown_GetState() == 0)
    {
        return 0;
    }
    return  1;
}

/****************************************************************
** Function name:           Gui_DataCountDownExit
** Descriptions:            ֹͣ����ʱ
** input parameters:        ��
** output parameters:       �� 
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-07-28
*****************************************************************/ 
void Gui_DataCountDownExit(void)
{
    Countdown_Init();
    Countdown_SendDataToApp(1, 0xFF);
}

/****************************************************************
** Function name:           Gui_DataCountDownStart
** Descriptions:            ��ʼ��ε���ʱ
** input parameters:        ��
** output parameters:       �� 
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-07-28
*****************************************************************/ 
void Gui_DataCountDownStart(void)
{
    Countdown_SetTimeBySystemStart();;
}

/****************************************************************
** Function name:           Gui_DataGetAlarmClockData
** Descriptions:            ��ȡִ����������
** input parameters:        
** output parameters:        
** Returned value:          alarm_hour:ʱ, alarm_minute:��, alarm_mode:ģʽ
** Created by:              WJ              
** Created Date:            2017-07-28
*****************************************************************/ 
void Gui_DataGetAlarmClockData(uint8_t *alarm_hour, uint8_t *alarm_minute, uint8_t *alarm_mode)
{
    alarm_clock_do_t *alarm_clock_do = NULL;
    
    alarm_clock_do = Alarm_GetAlarmClockDo();
    
    if(alarm_hour != NULL)
    {
        *alarm_hour = alarm_clock_do->alarm_hour;
    }
    if(alarm_minute != NULL)
    {
        *alarm_minute = alarm_clock_do->alarm_minute;
    }    
    if(alarm_mode != NULL)
    {
        *alarm_mode = alarm_clock_do->alarm_mode;
    }    
}

/****************************************************************
** Function name:           Gui_DataPhoneNotifyDeal
** Descriptions:            ��Ϣ���ʹ���
** input parameters:        
** output parameters:        
** Returned value:          ���ص�ǰ��Ϣ������
** Created by:              WJ              
** Created Date:            2017-07-28
*****************************************************************/ 
GUI_MSG_TYPE_E Gui_DataPhoneNotifyDeal(void)
{
    Phone_NotifyCore();
    
    return (GUI_MSG_TYPE_E)unread_message_s.msg_type;
}

/****************************************************************
** Function name:           Gui_DataGetPhoneNotifyData
** Descriptions:            ��ȡ��Ϣ������(ָ��)�ͳ���
** input parameters:        ��
** output parameters:       content_len����Ϣ����
** Returned value:          ���ص�ǰ��Ϣ�����ݵ�ַ
** Created by:              WJ              
** Created Date:            2017-07-28
*****************************************************************/ 
uint8_t *Gui_DataGetPhoneNotifyData(uint16_t *content_len, uint8_t *page_index)
{
    if(content_len == NULL || page_index == NULL)
    {
        *content_len = 0;
        return 0;
    } 
    if(unread_message_s.msg_type == MSG_TYPE_CALL)
    {
        *page_index = 0;
        *content_len = unread_message_s.show_msg.title_len;
        return unread_message_s.show_msg.title;
    }
    else
    {
        *page_index = unread_message_s.current_page_index;
        *content_len = unread_message_s.show_msg.content_len;
        return unread_message_s.show_msg.content;
    }
}

/****************************************************************
** Function name:           Gui_DataClearUnreadMessage
** Descriptions:            �����Ϣ��¼
** input parameters:        ��
** output parameters:       �� 
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-07-28
*****************************************************************/ 
void Gui_DataClearUnreadMessage(void)
{    
    if(Message_IsReading())
    {
        Message_Clear();
    }
}

/****************************************************************
** Function name:           Gui_DataHaveUnreadMessage
** Descriptions:            ��δ����Ϣ��Ҫ����
** input parameters:        
** output parameters:        
** Returned value:          ����0:û��Ҫ�������Ϣ, 1:��δ����Ϣ
** Created by:              WJ              
** Created Date:            2017-07-28
*****************************************************************/ 
uint8_t Gui_DataHaveUnreadMessage(uint8_t reading_flag)
{    
    if(Message_ExistUnread() && Message_NextUnreadMessageHint(reading_flag))
    {
        return 1;
    }
    return 0;
}

/****************************************************************
** Function name:           Gui_DataIsReadMessage
** Descriptions:            �Ƿ��ڲ鿴��Ϣ
** input parameters:        
** output parameters:        
** Returned value:          ����0:û�ж�ȡ��Ϣ, 1:���ڶ�ȡ��Ϣ
** Created by:              WJ              
** Created Date:            2017-07-28
*****************************************************************/ 
uint8_t Gui_DataIsReadMessage(void)
{    
    if(Message_IsReading() == 1)
    {
        return 1;
    }
    return 0;
}

/****************************************************************
** Function name:           Gui_DataMessageIsCall
** Descriptions:            ��ǰ����������
** input parameters:        ��
** output parameters:       ��
** Returned value:          ����0:��������Ϣ����, 1:��������
** Created by:              WJ              
** Created Date:            2017-07-28
*****************************************************************/ 
uint8_t Gui_DataMessageIsCall(void)
{    
    if(unread_message_s.msg_type == GUI_MSG_TYPE_CALL)
    {
        return 1;
    }
    return 0;
}

/****************************************************************
** Function name:           Gui_DataMessageCallToMute
** Descriptions:            ���羲������
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-07-28
*****************************************************************/ 
void Gui_DataMessageCallToMute(void)
{   
    uint8_t ble_ack[20] = {0xC1,0x03,0x00};    
    
    Hid_AudioVolumeAdd();
    Motor_DelEvent();    //ֹͣ��  ;
    if(Phone_NotifyGetCallingState() == PHONE_CALL_INCOMING
      || Phone_NotifyGetCallingState() == PHONE_CALL_MUTE)
    {                
        Phone_NotifySetCallingState(PHONE_CALL_MUTE);//����
    }    
    Ble_SendDataViaBat(ble_ack, 20);
}

/****************************************************************
** Function name:           Gui_DataMessageCallToMute
** Descriptions:            ����Ҷ�
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-07-28
*****************************************************************/ 
void Gui_DataMessageCallToHangUp(void)
{   
    Motor_DelEvent();//�ر����	
    Phone_NotifySetCallingState(PHONE_CALL_READYREJECT);//�豸�����Ҷϱ�־
    Ancs_OperationCalls(1);     
}

/****************************************************************
** Function name:           Gui_DataMessageCallState
** Descriptions:            ���統ǰ״̬
** input parameters:        ��
** output parameters:       ��
** Returned value:          ����GUI_MSG_TYPE_NULL:����������; 
                            GUI_CALL_INCOMING:����; 
                            GUI_CALL_MUTE:����;
                            GUI_CALL_REJECTSUC:�ܽ�;
** Created by:              WJ              
** Created Date:            2017-07-28
*****************************************************************/ 
uint8_t Gui_DataMessageCallState(void)
{       
    if( Phone_NotifyGetCallingState() == PHONE_CALL_MUTE)
    {        
        return GUI_CALL_MUTE;   
    }
    else if(Phone_NotifyGetCallingState() == PHONE_CALL_REJECTSUC)
    {
        return GUI_CALL_REJECTSUC;   
    }
    else if(Phone_NotifyGetCallingState() == PHONE_CALL_INCOMING)
    {
        return GUI_CALL_INCOMING;
    }
    
    return GUI_CALL_NULL;
}

/****************************************************************
** Function name:           Gui_DataClearSystemPassword
** Descriptions:            �������
** input parameters:        
** output parameters:        
** Returned value:          ���ص�ǰ��Ϣ�ĳ���
** Created by:              WJ              
** Created Date:            2017-07-28
*****************************************************************/ 
void Gui_DataClearSystemPassword(void)
{
    system_info_s.password_num = 0;
    App_SetSystemInfoSave();
}

/****************************************************************
** Function name:           Gui_DataClickOnThePhoto
** Descriptions:            �������
** input parameters:        
** output parameters:        
** Returned value:          ���ص�ǰ��Ϣ�ĳ���
** Created by:              WJ              
** Created Date:            2017-07-28
*****************************************************************/ 
void Gui_DataClickOnThePhoto(void)
{
    Camera_BtnClick();
}


/****************************************************************
** Function name:           Gui_DataLongKeyToExitPhoto
** Descriptions:            �����˳���������
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-07-28
*****************************************************************/ 
void Gui_DataLongKeyToExitPhoto(void)
{
    Camera_BraceletExit();
}

/****************************************************************
** Function name:           Gui_DataMovementModeIsBusy
** Descriptions:            ��ȡ�˶�ģʽ״̬
** input parameters:        
** output parameters:       �� 
** Returned value:          ����STOP_WATCH_IDLE:����,STOP_WATCH_IS_BUSYæµ(�˶�ģʽ������)
** Created by:              WJ              
** Created Date:            2017-07-28
*****************************************************************/ 
uint8_t Gui_DataMovementModeIsBusy(void)
{
    if(Movement_IsBusy())
    {
        return MOVEMENT_MODE_IS_BUSY;
    }
    return MOVEMENT_MODE_IDLE;
}

/****************************************************************
** Function name:           Gui_DataToggleMovementMode
** Descriptions:            ��ת�˶�ģʽ״̬����ʼ����ͣ
** input parameters:        ��
** output parameters:       �� 
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-07-28
*****************************************************************/ 
void Gui_DataToggleMovementMode(void)
{       
    if(Movement_GetState() == MOVEMENT_STATE_PAUSE)
    {
        Movement_Start();
    }
    else if(Movement_GetState() == MOVEMENT_STATE_RUN)
    {
        Movement_Pause();
    }
}

/****************************************************************
** Function name:           Gui_DataEnteMovementMode
** Descriptions:            �����˶�ģʽ
** input parameters:        ��
** output parameters:       �� 
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-07-31
*****************************************************************/ 
void Gui_DataStartMovementMode(void)
{
    Movement_Start();
}

/****************************************************************
** Function name:           Gui_DataExitMovementMode
** Descriptions:            �˳��˶�ģʽ
** input parameters:        ��
** output parameters:       �� 
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-07-31
*****************************************************************/ 
void Gui_DataExitMovementMode(void)
{
    Movement_Exit();
}

/****************************************************************
** Function name:           Gui_DataGetMovementModeUiData
** Descriptions:            ��ȡ�˶�����
** input parameters:        ��
** output parameters:       ʱ�䡢���ʡ����롢�Ʋ�����·�� 
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-07-31
*****************************************************************/ 
void Gui_DataGetMovementModeUiData(uint16_t *seconds, uint8_t *heart, uint32_t *distance, uint32_t *calories, uint32_t *step)
{
    movement_patterns_info_t *movement_info_s = NULL;     
    
    movement_info_s = Movement_GetInfoData();
    if(seconds != NULL)
    {
        *seconds = movement_info_s->run_vail_time;
    }
    if(heart != NULL)
    {
        *heart = movement_info_s->ui_heart;
    }
    if(heart != NULL)
    {
        *distance = movement_info_s->ui_distance;
    }
    if(heart != NULL)
    {
        *calories = movement_info_s->ui_calories;
    }
    if(step != NULL)
    {
        *step = movement_info_s->ui_step;
    }
    
}

/****************************************************************
** Function name:           Gui_DataMovementModeIsRun
** Descriptions:            ��ȡ�˶�ģʽ����״̬
** input parameters:        
** output parameters:       �� 
** Returned value:          ����0����ͣ,1������
** Created by:              WJ              
** Created Date:            2017-07-28
*****************************************************************/ 
uint8_t Gui_DataMovementModeIsRun(void)
{
    if(Movement_GetState() == MOVEMENT_STATE_RUN)
    {
        return 1;
    }
    return 0;
}

/****************************************************************
** Function name:           Gui_DataMovementModeIsNotDisplayPause
** Descriptions:            ��ȡ�˶�ģʽ�Ƿ���ʾ��ͣ����
** input parameters:        
** output parameters:       �� 
** Returned value:          ����0����,1����
** Created by:              WJ              
** Created Date:            2017-07-28
*****************************************************************/ 
uint8_t Gui_DataMovementModeIsNotDisplayPause(void)
{
    if(Movement_IsMaxPauseCnt() || Movement_GetState() == MOVEMENT_STATE_FINISH)
    {
        return 1;
    }
    return 0;
}

/****************************************************************
** Function name:           Gui_DataClearPostureToState
** Descriptions:            ��̬����״̬���
** input parameters:        ��
** output parameters:       �� 
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-08-02
*****************************************************************/ 
void Gui_DataClearPostureToState(void)
{
    Acc_GestureOledFlagClean();
}

/****************************************************************
** Function name:           Gui_DataBspKeyPressedState
** Descriptions:            �����Ƿ���
** input parameters:        ��
** output parameters:       �� 
** Returned value:          ����0�������ͷţ�1,������
** Created by:              WJ              
** Created Date:            2017-08-02
*****************************************************************/ 
uint8_t Gui_DataBspKeyPressedState(void)
{
    if(Button_GetPressState() == 1)
    {
        return 1;
    }
    return 0;
}

/****************************************************************
** Function name:           Gui_DataSleepToRecalculate
** Descriptions:            ˯�����¼���
** input parameters:        ��
** output parameters:       �� 
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-08-17
*****************************************************************/ 
void Gui_DataSleepToRecalculate(void)
{
    static uint32_t time_minutes = 0;
    
    //��Ҫ���¼���
    if(clock->hour >= 8)// && clock->hour < 16)
    {                
        if(time_minutes == 0 || (clock->hour * 60 + clock->minutes - time_minutes) >= 5)
        {
            time_minutes = clock->hour * 60 + clock->minutes;
            Sleep_Repetition();
        }
    }
    else
    {
        time_minutes = 0;
    }
}

/****************************************************************
** Function name:           Gui_DataGetBleMac
** Descriptions:            ��ȡ����mac��ַ
** input parameters:        ��
** output parameters:       mac_data:MAC��ַ
** Returned value:          ��
** Created by:              YX              
** Created Date:            2017-08-28
*****************************************************************/ 
void Gui_DataGetBleMac(uint8_t *mac_data)
{
    if(mac_data != NULL)
    {
        Ble_GetBleAddr(mac_data);
    }
}

/****************************************************************
** Function name:           Gui_DataGetVer
** Descriptions:            ��ȡ�豸�汾��
** input parameters:        ��
** output parameters:       ver_data:�汾��
** Returned value:          ��
** Created by:              YX              
** Created Date:            2017-08-28
*****************************************************************/ 
void Gui_DataGetVer(uint8_t *ver_data)
{
    if(ver_data != NULL)
    {
        ver_data[0] = VERSION_H;
        ver_data[1] = VERSION_L;
    }
}

/****************************************************************
** Function name:           Gui_DataGetEncrypt
** Descriptions:            ��ȡ����״̬
** input parameters:        ��
** output parameters:       �� 
** Returned value:          0��δ����  1���Ѽ���
** Created by:              YX              
** Created Date:            2017-08-29
*****************************************************************/ 
uint8_t Gui_DataGetEncrypt(void)
{
    return system_flag_s.ota_encrypt;
}

/****************************************************************
** Function name:           Gui_DataClearCallingState
** Descriptions:            �������״̬
** input parameters:        ��
** output parameters:       �� 
** Returned value:          ��
** Created by:              YX              
** Created Date:            2017-08-29
*****************************************************************/ 
void Gui_DataClearCallingState(void)
{
    Phone_NotifySetCallingState(PHONE_CALL_IDLE);
}

/****************************************************************
** Function name:           Gui_DataGetPhysRmindData
** Descriptions:            ��ȡŮ����Ϣ������(ָ��)�ͳ���
** input parameters:        ��
** output parameters:       content_len����Ϣ����
** Returned value:          ���ص�ǰ��Ϣ�����ݵ�ַ
** Created by:              WJ              
** Created Date:            2017-07-28
*****************************************************************/ 
uint8_t *Gui_DataGetPhysRmindData(uint16_t *content_len, uint8_t *page_index)
{
    if(content_len == NULL || page_index == NULL)
    {
        *content_len = 0;
        return 0;
    } 
    *page_index = phys_remind_info_s.read_page;
    *content_len = phys_remind_info_s.read_len;
    return &phys_remind_info_s.remind_info_buf[phys_remind_info_s.read_index];    
}

/****************************************************************
** Function name:           Gui_DataIsReadPhysMessage
** Descriptions:            �Ƿ��ڲ鿴��Ϣ
** input parameters:        
** output parameters:        
** Returned value:          ����0:û�ж�ȡ��Ϣ, 1:���ڶ�ȡ��Ϣ
** Created by:              WJ              
** Created Date:            2017-10-26
*****************************************************************/ 
uint8_t Gui_DataIsReadPhysMessage(void)
{    
    if(phys_remind_info_s.is_reading == 1)
    {
        return 1;
    }
    return 0;
}

/****************************************************************
** Function name:           Gui_DataHaveUnreadMessage
** Descriptions:            ��δ����Ϣ��Ҫ����
** input parameters:        
** output parameters:        
** Returned value:          ����0:û��Ҫ�������Ϣ, 1:��δ����Ϣ
** Created by:              WJ              
** Created Date:            2017-07-28
*****************************************************************/ 
uint8_t Gui_DataHaveUnreadPhysMessage(uint8_t reading_flag)
{    
    if(Phys_ExistUnreadMessage() && Phys_DisplayRemindByNextPage(reading_flag))
    {
        return 1;
    }
    return 0;
}

/****************************************************************
** Function name:           Gui_DataGetPhysMenstrualPeriodType
** Descriptions:            ��ȡ���ڴ���ʲô�׶�
** input parameters:        ��
** output parameters:       ��
** Returned value:          ���ؾ���״̬0,�Ƿ�;1,����;2,��ȫ��;3,������;4,������ 
** Created by:              WJ              
** Created Date:            2017-10-31
*****************************************************************/ 
uint8_t Gui_DataGetPhysMenstrualPeriodType(void)
{    
    uint8_t type = 0;
    
    type = Phys_GetMenstrualPeriodType();
    return type;
}

/****************************************************************
** Function name:           Gui_DataClearPhysMessage
** Descriptions:            ���Ů���Ѷ�ȡ��Ϣ
** input parameters:        ��
** output parameters:       ��
** Returned value:          �� 
** Created by:              WJ              
** Created Date:            2017-11-07
*****************************************************************/ 
void Gui_DataClearPhysMessage(void)
{    
   if(phys_remind_info_s.is_reading == 1)
   {
       Phys_ClearRemindInfo();
   }
}


/****************************************************************
** Function name:           Gui_DataIsBpFunction
** Descriptions:            �Ƿ�֧��Ѫѹ����
** input parameters:        ��
** output parameters:       ��
** Returned value:          �� 
** Created by:              WJ              
** Created Date:            2017-11-11
*****************************************************************/ 
uint8_t Gui_DataIsBpFunction(void)
{    
   if(system_flag_s.bp_flag == 0x01)
   {
       return 1;
   }
   return 0;
}


/****************************************************************
** Function name:           Gui_DataDelBleDiscRemind
** Descriptions:            ɾ��������������
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2018-04-03
*****************************************************************/
void Gui_DataDelBleDiscRemind(void)
{
    Remind_BleDisconnectRemove();
}

