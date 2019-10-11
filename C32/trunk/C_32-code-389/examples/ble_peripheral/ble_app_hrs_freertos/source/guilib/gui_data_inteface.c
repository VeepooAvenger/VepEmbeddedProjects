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

extern phys_remind_info_t phys_remind_info_s;   //女性功能提醒消息

/*********************************************************************
* local functions
*/

/****************************************************************
** Function name:           Gui_DataSystemIsTestMode
** Descriptions:            系统处于测试模式 uint8_t Gui_DataSystemIsTestMode(void)
** input parameters:        
** output parameters:       
** Returned value:          返回0:正常模式, 1:测试模式
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
** Descriptions:            系统是否开机
** input parameters:        
** output parameters:       
** Returned value:          返回0:关机状态, 1:开机状态
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
** Descriptions:            系统开机
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-07-31
*****************************************************************/ 
void Gui_DataSystemToPoweOn(void)
{
    App_AddEvent(APP_BOOT_EVENT);
}

/****************************************************************
** Function name:           Gui_DataSystemToPoweOff
** Descriptions:            系统关机
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-07-31
*****************************************************************/ 
void Gui_DataSystemToPoweOff(void)
{
    App_AddEvent(APP_SHUTDOWN_EVENT);
}

/****************************************************************
** Function name:           Gui_DataGetSystemLanguage
** Descriptions:            获取系统语言
** input parameters:        
** output parameters:        
** Returned value:          返回系统语言
                            GUI_LANGUAGE_CHINESE:中文
                            GUI_LANGUAGE_ENAGLISH:英文
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
** Descriptions:            获取UI界面风格
** input parameters:        
** output parameters:        
** Returned value:          返回UI界面风格
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
** Descriptions:            时间的格式是不是24小时制式
** input parameters:        
** output parameters:       
** Returned value:          返回0:12小时制, 1:24小时制
** Created by:              WJ              
** Created Date:            2017-07-28
*****************************************************************/ 
uint8_t Gui_DataTimeFormatIs24(void)
{
    return (system_switch_s.time_formats != 0X02);
}

/****************************************************************
** Function name:           Gui_DataTimeFormatDisplay
** Descriptions:            获取时间制式显示
** input parameters:        
** output parameters:       
** Returned value:          返回0:24小时制, 1:12小时制AM,2:12小时制PM
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
** Descriptions:            时间的格式是不是24小时制式
** input parameters:        
** output parameters:       
** Returned value:          返回0:12小时制, 1:24小时制
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
** Descriptions:            时间的格式是不是24小时制式
** input parameters:        
** output parameters:       
** Returned value:          返回系统时间分钟
** Created by:              WJ              
** Created Date:            2017-07-28
*****************************************************************/ 
uint8_t Gui_DataGetSystemTimeMinute(void)
{
	return clock->minutes;
}

/****************************************************************
** Function name:           Gui_DataGetSystemTimeSecond
** Descriptions:            时间的格式是不是24小时制式
** input parameters:        
** output parameters:       
** Returned value:          返回系统当前秒数
** Created by:              WJ              
** Created Date:            2017-07-28
*****************************************************************/ 
uint8_t Gui_DataGetSystemTimeSecond(void)
{
	return clock->seconds;
}

/****************************************************************
** Function name:           Gui_DataGetSystemTimeSecond
** Descriptions:            时间的格式是不是24小时制式
** input parameters:        
** output parameters:       
** Returned value:          返回系统当前星期(1~7)
** Created by:              WJ              
** Created Date:            2017-07-28
*****************************************************************/ 
uint8_t Gui_DataGetSystemTimeWeek(void)
{
	return clock->week;
}

/****************************************************************
** Function name:           Gui_DataGetSystemTime
** Descriptions:            获取时间
** input parameters:        
** output parameters:       hour:时,minute:分,second:秒
** Returned value:          无
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
** Descriptions:            获取日期
** input parameters:        
** output parameters:       year:年,month:月,day:日
** Returned value:          无
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
** Descriptions:            获取所有计步数
** input parameters:        
** output parameters:       
** Returned value:          返回当前计步总数
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
** Descriptions:            获取距离米\千米\英里
** input parameters:        
** output parameters:       输出结果：米(取整)，公里（千米 ，保留小数点后三位），英里（mile，七位有效位数）
** Returned value:          无
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
        *meter  = gsii_sport_info_s.posi_distance;//取整
    }
    temp = ((uint32_t)(((gsii_sport_info_s.posi_distance)/1000.0f)*1000))/1000.0f;//保留三位小数点
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
** Descriptions:            时间的格式是不是24小时制式
** input parameters:        
** output parameters:       
** Returned value:          返回0:英里, 1:千米
** Created by:              WJ              
** Created Date:            2017-07-28
*****************************************************************/ 
uint8_t Gui_DataDistanceIsKM(void)
{
    return (system_switch_s.distance_unit != 0x02);
}

/****************************************************************
** Function name:           Gui_DataGetCalorie
** Descriptions:            获取卡路里值
** input parameters:        
** output parameters:       
** Returned value:          返回卡路里
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
** Descriptions:            获取运动达标进度
** input parameters:        
** output parameters:       
** Returned value:          返回进度(0~100)
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
** Descriptions:            获取睡眠时间(分钟)
** input parameters:        
** output parameters:       
** Returned value:          返回睡眠时间(分钟)
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
** Descriptions:            打开血压测量
** input parameters:        
** output parameters:       无
** Returned value:          无
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
** Descriptions:            关闭血压测量
** input parameters:        
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-07-28
*****************************************************************/ 
void Gui_DataCloseBPOpreation(void)
{  
    App_StopMeasuringHandler(TEST_CLOSE_BTN);
}

/****************************************************************
** Function name:           Gui_DataGetBPdata
** Descriptions:            获取血压数据(高低压、测量模式、测试状态)
** input parameters:        
** output parameters:       bp_sp:高压,bp_dp:低压,
                            test_mode:测量模式,test_state:测量状态
** Returned value:          无
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
            Bp_GetTestResult(bp_sp, bp_dp);    //获取血压数据
        }
        else
        {          
            *bp_sp = 0;
            *bp_dp = 0;
        }
    }
    if(test_mode != NULL)
    {
        bp_test_mode = Bp_GetBpMode(); //获取血压测量模式
        *test_mode = bp_test_mode->test_mode;     
    }
}

/****************************************************************
** Function name:           Gui_DataGetHeart
** Descriptions:            获取当前心率结果
** input parameters:        
** output parameters:       无
** Returned value:          返回当前心率结果
** Created by:              WJ              
** Created Date:            2017-07-28
*****************************************************************/ 
uint8_t Gui_DataGetHeart(void)
{  
    return Heart_Test_Value_Get();
}

/****************************************************************
** Function name:           Gui_DataCloseHeartOpreation
** Descriptions:            关闭心率测量
** input parameters:        
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-07-28
*****************************************************************/ 
void Gui_DataCloseHeartOpreation(void)
{    
    App_StopMeasuringHandler(TEST_CLOSE_BTN);
}

/****************************************************************
** Function name:           Gui_DataOpenHeartOpreation
** Descriptions:            打开心率测量
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-07-28
*****************************************************************/ 
void Gui_DataOpenHeartOpreation(void)
{  
    if(system_flag_s.measuring_status == APP_HEARTING_STATUS)
    {
        //如果app测量已将开启则不需要开启,只需要变更模式
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
** Descriptions:            关闭心率\血压测量
** input parameters:        
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-07-28
*****************************************************************/ 
void Gui_DataCloseHeartOrBpOpreation(void)
{    
    App_StopMeasuringHandler(TEST_CLOSE_BTN);
}


/****************************************************************
** Function name:           Gui_DataGetWearDectState
** Descriptions:            获取佩戴状态
** input parameters:        无
** output parameters:       无
** Returned value:          返回0：佩戴不通过测试关闭；1：测试中
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
** Descriptions:            获取电池等级和状态
** input parameters:        
** output parameters:       level:电量等级
** Returned value:          返回电池充电状态,BATTERRY_IS_NORMAL:放电
                            BATTERRY_IS_CHARGING:充电中
                            BATTERRY_IS_FULL:充满
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
    //YX.20180712.使用的状态跟充电检测的同步，不直接使用引脚状态
    if(system_flag_s.power_state == POWER_FULL)
    {
        charge_state = BATTERRY_IS_FULL; //充满
    }
    else if(system_flag_s.power_state == POWER_CHARGE)
    {
        charge_state = BATTERRY_IS_CHARGING; //充电中
    }
    else
    {
        charge_state = BATTERRY_IS_NORMAL;   //放电
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
** Descriptions:            电池低电
** input parameters:        
** output parameters:       无
** Returned value:          BATTERRY_IS_LOW_POWER:低电,
                            BATTERRY_NOT_LOW_POWER:非低电
                            BATTERRY_IS_LOW_TO_SHUT:低电关机
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
** Descriptions:            找手机界面是否显示
** input parameters:        
** output parameters:       无 
** Returned value:          返回0:不显示页面,1:显示页面
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
** Descriptions:            手机查找发送数据
** input parameters:        
** output parameters:       无
** Returned value:          无
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
** Descriptions:            获取蓝牙连接状态
** input parameters:        
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-07-28
*****************************************************************/ 
uint8_t Gui_DataGetBleState(void)
{
     
    return system_flag_s.ble_connect;
}

/****************************************************************
** Function name:           Gui_DataGetStopWatchSecond
** Descriptions:            获取秒表秒和100毫秒
** input parameters:        
** output parameters:       total_second:总计秒数, *millisecond:100毫秒
** Returned value:          无
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
** Descriptions:            获取秒表状态
** input parameters:        
** output parameters:       无 
** Returned value:          返回STOP_WATCH_IDLE:空闲,STOP_WATCH_IS_BUSY忙碌(秒表就绪了)
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
** Descriptions:            翻转秒表状态，开始、暂停
** input parameters:        无
** output parameters:       无 
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-07-28
*****************************************************************/ 
void Gui_DataToggleStopWatch(void)
{
    Watch_Toggle();
}

/****************************************************************
** Function name:           Gui_DataSetStopWatchToReady
** Descriptions:            设置秒表为就绪状态
** input parameters:        无
** output parameters:       无 
** Returned value:          无
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
** Descriptions:            退出秒表
** input parameters:        无
** output parameters:       无 
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-07-31
*****************************************************************/ 
void Gui_DataExitStopWatch(void)
{
    Watch_Init();
}

/****************************************************************
** Function name:           Gui_DataStopWatchPageIsEnable
** Descriptions:            秒表界面是否显示
** input parameters:        
** output parameters:       无 
** Returned value:          返回0:不显示页面,1:显示页面
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
** Descriptions:            获取倒计时状态
** input parameters:        
** output parameters:       无 
** Returned value:          返回STOP_COUNT_DOWN_IDLE:空闲,STOP_COUNT_DOWN_IS_BUSY忙碌
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
** Descriptions:            倒计时界面是否显示
** input parameters:        
** output parameters:       无 
** Returned value:          返回0:不显示页面,1:显示页面
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
** Descriptions:            停止倒计时
** input parameters:        无
** output parameters:       无 
** Returned value:          无
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
** Descriptions:            开始多次倒计时
** input parameters:        无
** output parameters:       无 
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-07-28
*****************************************************************/ 
void Gui_DataCountDownStart(void)
{
    Countdown_SetTimeBySystemStart();;
}

/****************************************************************
** Function name:           Gui_DataGetAlarmClockData
** Descriptions:            获取执行闹钟数据
** input parameters:        
** output parameters:        
** Returned value:          alarm_hour:时, alarm_minute:分, alarm_mode:模式
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
** Descriptions:            消息推送处理
** input parameters:        
** output parameters:        
** Returned value:          返回当前消息的内容
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
** Descriptions:            获取消息的内容(指针)和长度
** input parameters:        无
** output parameters:       content_len：信息长度
** Returned value:          返回当前消息的内容地址
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
** Descriptions:            清除消息记录
** input parameters:        无
** output parameters:       无 
** Returned value:          无
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
** Descriptions:            有未读信息需要处理
** input parameters:        
** output parameters:        
** Returned value:          返回0:没有要处理的消息, 1:有未读信息
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
** Descriptions:            是否在查看信息
** input parameters:        
** output parameters:        
** Returned value:          返回0:没有读取消息, 1:正在读取信息
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
** Descriptions:            当前是来电提醒
** input parameters:        无
** output parameters:       无
** Returned value:          返回0:其他的消息提醒, 1:来电提醒
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
** Descriptions:            来电静音操作
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-07-28
*****************************************************************/ 
void Gui_DataMessageCallToMute(void)
{   
    uint8_t ble_ack[20] = {0xC1,0x03,0x00};    
    
    Hid_AudioVolumeAdd();
    Motor_DelEvent();    //停止震动  ;
    if(Phone_NotifyGetCallingState() == PHONE_CALL_INCOMING
      || Phone_NotifyGetCallingState() == PHONE_CALL_MUTE)
    {                
        Phone_NotifySetCallingState(PHONE_CALL_MUTE);//静音
    }    
    Ble_SendDataViaBat(ble_ack, 20);
}

/****************************************************************
** Function name:           Gui_DataMessageCallToMute
** Descriptions:            来电挂断
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-07-28
*****************************************************************/ 
void Gui_DataMessageCallToHangUp(void)
{   
    Motor_DelEvent();//关闭马达	
    Phone_NotifySetCallingState(PHONE_CALL_READYREJECT);//设备主动挂断标志
    Ancs_OperationCalls(1);     
}

/****************************************************************
** Function name:           Gui_DataMessageCallState
** Descriptions:            来电当前状态
** input parameters:        无
** output parameters:       无
** Returned value:          返回GUI_MSG_TYPE_NULL:就绪、空闲; 
                            GUI_CALL_INCOMING:来电; 
                            GUI_CALL_MUTE:静音;
                            GUI_CALL_REJECTSUC:拒接;
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
** Descriptions:            清除密码
** input parameters:        
** output parameters:        
** Returned value:          返回当前消息的长度
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
** Descriptions:            点击拍照
** input parameters:        
** output parameters:        
** Returned value:          返回当前消息的长度
** Created by:              WJ              
** Created Date:            2017-07-28
*****************************************************************/ 
void Gui_DataClickOnThePhoto(void)
{
    Camera_BtnClick();
}


/****************************************************************
** Function name:           Gui_DataLongKeyToExitPhoto
** Descriptions:            长按退出拍照拍照
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-07-28
*****************************************************************/ 
void Gui_DataLongKeyToExitPhoto(void)
{
    Camera_BraceletExit();
}

/****************************************************************
** Function name:           Gui_DataMovementModeIsBusy
** Descriptions:            获取运动模式状态
** input parameters:        
** output parameters:       无 
** Returned value:          返回STOP_WATCH_IDLE:空闲,STOP_WATCH_IS_BUSY忙碌(运动模式就绪了)
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
** Descriptions:            翻转运动模式状态，开始、暂停
** input parameters:        无
** output parameters:       无 
** Returned value:          无
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
** Descriptions:            进入运动模式
** input parameters:        无
** output parameters:       无 
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-07-31
*****************************************************************/ 
void Gui_DataStartMovementMode(void)
{
    Movement_Start();
}

/****************************************************************
** Function name:           Gui_DataExitMovementMode
** Descriptions:            退出运动模式
** input parameters:        无
** output parameters:       无 
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-07-31
*****************************************************************/ 
void Gui_DataExitMovementMode(void)
{
    Movement_Exit();
}

/****************************************************************
** Function name:           Gui_DataGetMovementModeUiData
** Descriptions:            获取运动数据
** input parameters:        无
** output parameters:       时间、心率、距离、计步、卡路里 
** Returned value:          无
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
** Descriptions:            获取运动模式运行状态
** input parameters:        
** output parameters:       无 
** Returned value:          返回0：暂停,1：运行
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
** Descriptions:            获取运动模式是否显示暂停界面
** input parameters:        
** output parameters:       无 
** Returned value:          返回0：否,1：是
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
** Descriptions:            姿态亮屏状态清除
** input parameters:        无
** output parameters:       无 
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-08-02
*****************************************************************/ 
void Gui_DataClearPostureToState(void)
{
    Acc_GestureOledFlagClean();
}

/****************************************************************
** Function name:           Gui_DataBspKeyPressedState
** Descriptions:            按键是否按下
** input parameters:        无
** output parameters:       无 
** Returned value:          返回0，按键释放；1,按下中
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
** Descriptions:            睡眠重新计算
** input parameters:        无
** output parameters:       无 
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-08-17
*****************************************************************/ 
void Gui_DataSleepToRecalculate(void)
{
    static uint32_t time_minutes = 0;
    
    //需要重新计算
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
** Descriptions:            获取蓝牙mac地址
** input parameters:        无
** output parameters:       mac_data:MAC地址
** Returned value:          无
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
** Descriptions:            获取设备版本号
** input parameters:        无
** output parameters:       ver_data:版本号
** Returned value:          无
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
** Descriptions:            获取加密状态
** input parameters:        无
** output parameters:       无 
** Returned value:          0：未加密  1：已加密
** Created by:              YX              
** Created Date:            2017-08-29
*****************************************************************/ 
uint8_t Gui_DataGetEncrypt(void)
{
    return system_flag_s.ota_encrypt;
}

/****************************************************************
** Function name:           Gui_DataClearCallingState
** Descriptions:            清除来电状态
** input parameters:        无
** output parameters:       无 
** Returned value:          无
** Created by:              YX              
** Created Date:            2017-08-29
*****************************************************************/ 
void Gui_DataClearCallingState(void)
{
    Phone_NotifySetCallingState(PHONE_CALL_IDLE);
}

/****************************************************************
** Function name:           Gui_DataGetPhysRmindData
** Descriptions:            获取女性消息的内容(指针)和长度
** input parameters:        无
** output parameters:       content_len：信息长度
** Returned value:          返回当前消息的内容地址
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
** Descriptions:            是否在查看信息
** input parameters:        
** output parameters:        
** Returned value:          返回0:没有读取消息, 1:正在读取信息
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
** Descriptions:            有未读信息需要处理
** input parameters:        
** output parameters:        
** Returned value:          返回0:没有要处理的消息, 1:有未读信息
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
** Descriptions:            获取经期处于什么阶段
** input parameters:        无
** output parameters:       无
** Returned value:          返回经期状态0,非法;1,经期;2,安全期;3,排卵期;4,排卵日 
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
** Descriptions:            清除女性已读取信息
** input parameters:        无
** output parameters:       无
** Returned value:          无 
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
** Descriptions:            是否支持血压功能
** input parameters:        无
** output parameters:       无
** Returned value:          无 
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
** Descriptions:            删除蓝牙断连提醒
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2018-04-03
*****************************************************************/
void Gui_DataDelBleDiscRemind(void)
{
    Remind_BleDisconnectRemove();
}

