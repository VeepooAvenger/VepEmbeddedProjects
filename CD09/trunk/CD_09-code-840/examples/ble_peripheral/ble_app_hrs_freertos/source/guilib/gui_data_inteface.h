/***************************Copyright(c)***********************************
**                       Veepoo Tech
**                  COPYRIGHT (C) 2016-2019
**-----------File Info-----------------------------------------------------
** File        name:     gui_data_inteface.h
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

#ifndef __GUI_DATA_INTEFACE_H
#define __GUI_DATA_INTEFACE_H
#include "board.h"
#include "configuration.h"

#define BATTERRY_IS_NORMAL      0       //未充电
#define BATTERRY_IS_CHARGING    1       //正在充电
#define BATTERRY_IS_FULL        2       //充满

#define BATTERRY_NOT_LOW_POWER  0       //不是低电
#define BATTERRY_IS_LOW_POWER   1       //低电 
#define BATTERRY_IS_LOW_TO_SHUT 2       //低电关机

#define STOP_WATCH_IDLE         0       //秒表空闲
#define STOP_WATCH_IS_BUSY      1       //秒表忙碌

#define COUNT_DOWN_IDLE         0       //倒计时空闲
#define COUNT_DOWN_IS_BUSY      1       //倒计时忙碌


#define MOVEMENT_MODE_IDLE         0       //运动模式空闲
#define MOVEMENT_MODE_IS_BUSY      1       //运动模式忙碌


typedef enum
{
    GUI_MSG_TYPE_NULL = 0,
    GUI_MSG_TYPE_CALL = 1,
    GUI_MSG_TYPE_MESSAGE,
    GUI_MSG_TYPE_WECHAT,
    GUI_MSG_TYPE_QQ,
    GUI_MSG_TYPE_FACEBOOK,
    GUI_MSG_TYPE_TWITTER,
    GUI_MSG_TYPE_WHATSAPP,
    GUI_MSG_TYPE_LINKEDIN,
    GUI_MSG_TYPE_SNAPCHAT,
    GUI_MSG_TYPE_SKYPE,
    GUI_MSG_TYPE_INSTAGRAM,
    GUI_MSG_TYPE_LINE,
    GUI_MSG_TYPE_GMAIL,
    GUI_MSG_TYPE_OTHER,
    GUI_MSG_TYPE_PHYS,
}GUI_MSG_TYPE_E;

typedef enum
{
    GUI_CALL_NULL = 0,          //就绪
    GUI_CALL_INCOMING,          //来电
    GUI_CALL_MUTE,              //静音
    GUI_CALL_REJECTSUC          //拒接
}GUI_CALL_STATE_E;

typedef enum
{
    GUI_TIME_FORMAT_STANDARD = 0,       //24小时制
    GUI_TIME_FORMAT_AM,                 //12小时制AM
    GUI_TIME_FORMAT_PM                  //12小时制PM
}GUI_TIME_FORMAT_E;

//typedef enum
//{
//    GUI_MAIN_WHITE = 0,       //白色背景
//    GUI_MAIN_RED,             //红色背景
//    GUI_MAIN_YELLOW,          //黄色背景
//    GUI_MAIN_GREEN            //绿色背景
//}GUI_MAIN_BACK_COLOR_E;


typedef enum
{
    GUI_LANGUAGE_CHINESE = 0,       //中文
    GUI_LANGUAGE_ENAGLISH,          //英文
}GUI_LANGUAGE_E;

//系统处于测试模式 
uint8_t Gui_DataSystemIsTestMode(void);


//系统是否开机,返回0:关机状态, 1:开机状态
uint8_t Gui_DataSystemIsPoweOn(void);
//关机操作
void Gui_DataSystemToPoweOff(void);
//开机操作
void Gui_DataSystemToPoweOn(void);

//获取系统语言;返回系统语言,GUI_LANGUAGE_ENAGLISH:英文
GUI_LANGUAGE_E Gui_DataGetSystemLanguage(void);

//获取系统语言;返回系统语言,GUI_LANGUAGE_ENAGLISH:英文
GUI_LANGUAGE_E Gui_DataGetSystemLanguage(void);

//获取UI界面风格;返回UI界面风格
uint8_t Gui_DataGetUiStyle(void);

//时间制式
uint8_t Gui_DataTimeFormatIs24(void);
//获取时间制式显示，返回0:24小时制, 1:12小时制AM,2:12小时制PM
uint8_t Gui_DataTimeFormatDisplay(void);
//获取系统时间-小时
uint8_t Gui_DataGetSystemTimeHour(void);
//获取系统时间-分钟
uint8_t Gui_DataGetSystemTimeMinute(void);
//获取系统时间-秒
uint8_t Gui_DataGetSystemTimeSecond(void);
//获取系统时间-星期(1~7)
uint8_t Gui_DataGetSystemTimeWeek(void);
//获取系统日期,year:年,month:月,day:日
void Gui_DataGetSystemTimeDate(uint8_t *year, uint8_t *month, uint8_t *day);
//获取时间hour:时,minute:分,second:秒
void Gui_DataGetSystemTime(uint8_t *hour, uint8_t *minute, uint8_t *second);


//获取当前计步总数    
uint32_t Gui_DataGetAllSteps(void);
//输出结果：米(取整)，公里（千米 ，保留小数点后三位），英里（mile，七位有效位数）
void Gui_DataGetDistanceValue(uint32_t *meter, float *kilometer, float *mile);
//当前显示距离单位是不是KM
uint8_t Gui_DataDistanceIsKM(void);
//获取当前卡路里值
float Gui_DataGetCalorie(void);
//获取运动达标进度(0~100)
uint8_t Gui_DataGetSportsToMarkProgress(void);


//获取睡眠时间(分钟)
uint16_t Gui_DataGetSleepTime(void);


//打开血压测量
void Gui_DataOpenBPOpreation(void);
//关闭血压上测量
void Gui_DataCloseBPOpreation(void);
//获取血压测量数据bp_sp:高压,bp_dp:低压,test_mode:测量模式,test_state:测量状态
void Gui_DataGetBPdata(uint8_t *bp_sp, uint8_t *bp_dp, uint8_t *test_mode, uint8_t *test_state);

//打开心率测量
void Gui_DataOpenHeartOpreation(void);
//关闭心率测量
void Gui_DataCloseHeartOpreation(void);
//关闭心率\血压测量
void Gui_DataCloseHeartOrBpOpreation(void);
//获取佩戴状态;返回0：佩戴不通过测试关闭；1：测试中
uint8_t Gui_DataGetWearDectState(void);

//获取当前心率结果
uint8_t Gui_DataGetHeart(void);

//获取电池充电状态和电池电量等级
uint8_t Gui_DataGetBatteryLevelAndState(uint8_t *level);
//获取电池低电状态,BATTERRY_IS_LOW_POWER:低电状态,BATTERRY_NOT_LOW_POWER:非低电状态
uint8_t Gui_DataBatteryIsLowPower(void);


//找手机界面是否显示;返回0:不显示页面,1:显示页面
uint8_t Gui_DataFindPhonePageIsEnable(void);

//手机查找发送查找指令
void Gui_DataFindSendData(void);


//获取蓝牙连接状态
uint8_t Gui_DataGetBleState(void);


//获取秒表秒和100毫秒total_second:总计秒数, millisecond:100毫秒
void Gui_DataGetStopWatchSecond(uint16_t *total_second, uint8_t *millisecond);
//获取秒表状态,返回STOP_WATCH_IDLE:空闲,STOP_WATCH_IS_BUSY忙碌(秒表就绪了)
uint8_t Gui_DataStopWatchIsBusy(void);
//设置秒表为就绪状态
void Gui_DataSetStopWatchToReady(void);
//翻转秒表状态，开始、暂停
void Gui_DataToggleStopWatch(void);
//退出秒表
void Gui_DataExitStopWatch(void);
//秒表界面是否显示
uint8_t Gui_DataStopWatchPageIsEnable(void);

//获取倒计时状态,返回STOP_COUNT_DOWN_IDLE:空闲,STOP_COUNT_DOWN_IS_BUSY忙碌
uint8_t Gui_DataCountDownIsBusy(void);
//倒计时界面是否显示
uint8_t Gui_DataCountDownPageIsEnable(void);
//停止倒计时
void Gui_DataCountDownExit(void);

//开始多次倒计时 
void Gui_DataCountDownStart(void);


//获取执行闹钟数据,alarm_hour:时, alarm_minute:分, alarm_mode:模式
void Gui_DataGetAlarmClockData(uint8_t *alarm_hour, uint8_t *alarm_minute, uint8_t *alarm_mode);


//消息推送处理
GUI_MSG_TYPE_E Gui_DataPhoneNotifyDeal(void);
//获取消息的内容(指针)和长度
uint8_t *Gui_DataGetPhoneNotifyData(uint16_t *content_len, uint8_t *page_index);
//清除消息记录
void Gui_DataClearUnreadMessage(void);
//有未读信息需要处理,返回0:没有要处理的消息, 1:有未读信息
uint8_t Gui_DataHaveUnreadMessage(uint8_t reading_flag);
//正在读取信息,返回0:没有读取消息, 1:正在读取信息
uint8_t Gui_DataIsReadMessage(void);
//当前是来电提醒
uint8_t Gui_DataMessageIsCall(void);
//来电静音操作
void Gui_DataMessageCallToMute(void);
//来电挂断
void Gui_DataMessageCallToHangUp(void);
//来电状态：来电、静音、拒接；返回GUI_MSG_TYPE_NULL:就绪、空闲; 
//GUI_CALL_INCOMING:来电; GUI_CALL_MUTE:静音; GUI_CALL_REJECTSUC:拒接;
uint8_t Gui_DataMessageCallState(void);


//清除密码
void Gui_DataClearSystemPassword(void);


//点击拍照
void Gui_DataClickOnThePhoto(void);

//长按退出拍照拍照
void Gui_DataLongKeyToExitPhoto(void);


//获取运动状态,返回STOP_WATCH_IDLE:空闲,STOP_WATCH_IS_BUSY忙碌(运动就绪了)
uint8_t Gui_DataMovementModeIsBusy(void);
//设置运动为就绪状态
void Gui_DataStartMovementMode(void);
//翻转运动状态，开始、暂停
void Gui_DataToggleMovementMode(void);
//退出运动模式
void Gui_DataExitMovementMode(void);
//获取运动数据
void Gui_DataGetMovementModeUiData(uint16_t *seconds, uint8_t *heart, uint32_t *distance, uint32_t *calories, uint32_t *step);
//获取运动模式运行状态；返回0：暂停,1：运行
uint8_t Gui_DataMovementModeIsRun(void);

//获取运动模式是否显示暂停界面；返回0：否,1：是
uint8_t Gui_DataMovementModeIsNotDisplayPause(void);

//姿态亮屏状态清除
void Gui_DataClearPostureToState(void);

//按键是否按下;返回0，按键释放；1,按下中
uint8_t Gui_DataBspKeyPressedState(void);

//睡眠重新计算
void Gui_DataSleepToRecalculate(void);

//获取蓝牙mac地址
void Gui_DataGetBleMac(uint8_t *mac_data);

//获取设备版本号
void Gui_DataGetVer(uint8_t *ver_data);

//获取加密状态
uint8_t Gui_DataGetEncrypt(void);

//清除来电状态
void Gui_DataClearCallingState(void);

//获取女性消息的内容(指针)和长度
uint8_t *Gui_DataGetPhysRmindData(uint16_t *content_len, uint8_t *page_index);

uint8_t Gui_DataIsReadPhysMessage(void);

uint8_t Gui_DataHaveUnreadPhysMessage(uint8_t reading_flag);

//清除女性已读取信息
void Gui_DataClearPhysMessage(void);

//获取经期处于什么阶段
// 0,非法;1,经期;2,安全期;3,排卵期;4,排卵日 
uint8_t Gui_DataGetPhysMenstrualPeriodType(void);

//是否支持血压功能
uint8_t Gui_DataIsBpFunction(void);

//删除蓝牙断连提醒
void Gui_DataDelBleDiscRemind(void);

#endif  //__GUI_DATA_INTEFACE_H


