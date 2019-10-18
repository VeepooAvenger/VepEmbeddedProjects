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

#define BATTERRY_IS_NORMAL      0       //δ���
#define BATTERRY_IS_CHARGING    1       //���ڳ��
#define BATTERRY_IS_FULL        2       //����

#define BATTERRY_NOT_LOW_POWER  0       //���ǵ͵�
#define BATTERRY_IS_LOW_POWER   1       //�͵� 
#define BATTERRY_IS_LOW_TO_SHUT 2       //�͵�ػ�

#define STOP_WATCH_IDLE         0       //������
#define STOP_WATCH_IS_BUSY      1       //���æµ

#define COUNT_DOWN_IDLE         0       //����ʱ����
#define COUNT_DOWN_IS_BUSY      1       //����ʱæµ


#define MOVEMENT_MODE_IDLE         0       //�˶�ģʽ����
#define MOVEMENT_MODE_IS_BUSY      1       //�˶�ģʽæµ


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
    GUI_CALL_NULL = 0,          //����
    GUI_CALL_INCOMING,          //����
    GUI_CALL_MUTE,              //����
    GUI_CALL_REJECTSUC          //�ܽ�
}GUI_CALL_STATE_E;

typedef enum
{
    GUI_TIME_FORMAT_STANDARD = 0,       //24Сʱ��
    GUI_TIME_FORMAT_AM,                 //12Сʱ��AM
    GUI_TIME_FORMAT_PM                  //12Сʱ��PM
}GUI_TIME_FORMAT_E;

//typedef enum
//{
//    GUI_MAIN_WHITE = 0,       //��ɫ����
//    GUI_MAIN_RED,             //��ɫ����
//    GUI_MAIN_YELLOW,          //��ɫ����
//    GUI_MAIN_GREEN            //��ɫ����
//}GUI_MAIN_BACK_COLOR_E;


typedef enum
{
    GUI_LANGUAGE_CHINESE = 0,       //����
    GUI_LANGUAGE_ENAGLISH,          //Ӣ��
}GUI_LANGUAGE_E;

//ϵͳ���ڲ���ģʽ 
uint8_t Gui_DataSystemIsTestMode(void);


//ϵͳ�Ƿ񿪻�,����0:�ػ�״̬, 1:����״̬
uint8_t Gui_DataSystemIsPoweOn(void);
//�ػ�����
void Gui_DataSystemToPoweOff(void);
//��������
void Gui_DataSystemToPoweOn(void);

//��ȡϵͳ����;����ϵͳ����,GUI_LANGUAGE_ENAGLISH:Ӣ��
GUI_LANGUAGE_E Gui_DataGetSystemLanguage(void);

//��ȡϵͳ����;����ϵͳ����,GUI_LANGUAGE_ENAGLISH:Ӣ��
GUI_LANGUAGE_E Gui_DataGetSystemLanguage(void);

//��ȡUI������;����UI������
uint8_t Gui_DataGetUiStyle(void);

//ʱ����ʽ
uint8_t Gui_DataTimeFormatIs24(void);
//��ȡʱ����ʽ��ʾ������0:24Сʱ��, 1:12Сʱ��AM,2:12Сʱ��PM
uint8_t Gui_DataTimeFormatDisplay(void);
//��ȡϵͳʱ��-Сʱ
uint8_t Gui_DataGetSystemTimeHour(void);
//��ȡϵͳʱ��-����
uint8_t Gui_DataGetSystemTimeMinute(void);
//��ȡϵͳʱ��-��
uint8_t Gui_DataGetSystemTimeSecond(void);
//��ȡϵͳʱ��-����(1~7)
uint8_t Gui_DataGetSystemTimeWeek(void);
//��ȡϵͳ����,year:��,month:��,day:��
void Gui_DataGetSystemTimeDate(uint8_t *year, uint8_t *month, uint8_t *day);
//��ȡʱ��hour:ʱ,minute:��,second:��
void Gui_DataGetSystemTime(uint8_t *hour, uint8_t *minute, uint8_t *second);


//��ȡ��ǰ�Ʋ�����    
uint32_t Gui_DataGetAllSteps(void);
//����������(ȡ��)�����ǧ�� ������С�������λ����Ӣ�mile����λ��Чλ����
void Gui_DataGetDistanceValue(uint32_t *meter, float *kilometer, float *mile);
//��ǰ��ʾ���뵥λ�ǲ���KM
uint8_t Gui_DataDistanceIsKM(void);
//��ȡ��ǰ��·��ֵ
float Gui_DataGetCalorie(void);
//��ȡ�˶�������(0~100)
uint8_t Gui_DataGetSportsToMarkProgress(void);


//��ȡ˯��ʱ��(����)
uint16_t Gui_DataGetSleepTime(void);


//��Ѫѹ����
void Gui_DataOpenBPOpreation(void);
//�ر�Ѫѹ�ϲ���
void Gui_DataCloseBPOpreation(void);
//��ȡѪѹ��������bp_sp:��ѹ,bp_dp:��ѹ,test_mode:����ģʽ,test_state:����״̬
void Gui_DataGetBPdata(uint8_t *bp_sp, uint8_t *bp_dp, uint8_t *test_mode, uint8_t *test_state);

//�����ʲ���
void Gui_DataOpenHeartOpreation(void);
//�ر����ʲ���
void Gui_DataCloseHeartOpreation(void);
//�ر�����\Ѫѹ����
void Gui_DataCloseHeartOrBpOpreation(void);
//��ȡ���״̬;����0�������ͨ�����Թرգ�1��������
uint8_t Gui_DataGetWearDectState(void);

//��ȡ��ǰ���ʽ��
uint8_t Gui_DataGetHeart(void);

//��ȡ��س��״̬�͵�ص����ȼ�
uint8_t Gui_DataGetBatteryLevelAndState(uint8_t *level);
//��ȡ��ص͵�״̬,BATTERRY_IS_LOW_POWER:�͵�״̬,BATTERRY_NOT_LOW_POWER:�ǵ͵�״̬
uint8_t Gui_DataBatteryIsLowPower(void);


//���ֻ������Ƿ���ʾ;����0:����ʾҳ��,1:��ʾҳ��
uint8_t Gui_DataFindPhonePageIsEnable(void);

//�ֻ����ҷ��Ͳ���ָ��
void Gui_DataFindSendData(void);


//��ȡ��������״̬
uint8_t Gui_DataGetBleState(void);


//��ȡ������100����total_second:�ܼ�����, millisecond:100����
void Gui_DataGetStopWatchSecond(uint16_t *total_second, uint8_t *millisecond);
//��ȡ���״̬,����STOP_WATCH_IDLE:����,STOP_WATCH_IS_BUSYæµ(��������)
uint8_t Gui_DataStopWatchIsBusy(void);
//�������Ϊ����״̬
void Gui_DataSetStopWatchToReady(void);
//��ת���״̬����ʼ����ͣ
void Gui_DataToggleStopWatch(void);
//�˳����
void Gui_DataExitStopWatch(void);
//�������Ƿ���ʾ
uint8_t Gui_DataStopWatchPageIsEnable(void);

//��ȡ����ʱ״̬,����STOP_COUNT_DOWN_IDLE:����,STOP_COUNT_DOWN_IS_BUSYæµ
uint8_t Gui_DataCountDownIsBusy(void);
//����ʱ�����Ƿ���ʾ
uint8_t Gui_DataCountDownPageIsEnable(void);
//ֹͣ����ʱ
void Gui_DataCountDownExit(void);

//��ʼ��ε���ʱ 
void Gui_DataCountDownStart(void);


//��ȡִ����������,alarm_hour:ʱ, alarm_minute:��, alarm_mode:ģʽ
void Gui_DataGetAlarmClockData(uint8_t *alarm_hour, uint8_t *alarm_minute, uint8_t *alarm_mode);


//��Ϣ���ʹ���
GUI_MSG_TYPE_E Gui_DataPhoneNotifyDeal(void);
//��ȡ��Ϣ������(ָ��)�ͳ���
uint8_t *Gui_DataGetPhoneNotifyData(uint16_t *content_len, uint8_t *page_index);
//�����Ϣ��¼
void Gui_DataClearUnreadMessage(void);
//��δ����Ϣ��Ҫ����,����0:û��Ҫ�������Ϣ, 1:��δ����Ϣ
uint8_t Gui_DataHaveUnreadMessage(uint8_t reading_flag);
//���ڶ�ȡ��Ϣ,����0:û�ж�ȡ��Ϣ, 1:���ڶ�ȡ��Ϣ
uint8_t Gui_DataIsReadMessage(void);
//��ǰ����������
uint8_t Gui_DataMessageIsCall(void);
//���羲������
void Gui_DataMessageCallToMute(void);
//����Ҷ�
void Gui_DataMessageCallToHangUp(void);
//����״̬�����硢�������ܽӣ�����GUI_MSG_TYPE_NULL:����������; 
//GUI_CALL_INCOMING:����; GUI_CALL_MUTE:����; GUI_CALL_REJECTSUC:�ܽ�;
uint8_t Gui_DataMessageCallState(void);


//�������
void Gui_DataClearSystemPassword(void);


//�������
void Gui_DataClickOnThePhoto(void);

//�����˳���������
void Gui_DataLongKeyToExitPhoto(void);


//��ȡ�˶�״̬,����STOP_WATCH_IDLE:����,STOP_WATCH_IS_BUSYæµ(�˶�������)
uint8_t Gui_DataMovementModeIsBusy(void);
//�����˶�Ϊ����״̬
void Gui_DataStartMovementMode(void);
//��ת�˶�״̬����ʼ����ͣ
void Gui_DataToggleMovementMode(void);
//�˳��˶�ģʽ
void Gui_DataExitMovementMode(void);
//��ȡ�˶�����
void Gui_DataGetMovementModeUiData(uint16_t *seconds, uint8_t *heart, uint32_t *distance, uint32_t *calories, uint32_t *step);
//��ȡ�˶�ģʽ����״̬������0����ͣ,1������
uint8_t Gui_DataMovementModeIsRun(void);

//��ȡ�˶�ģʽ�Ƿ���ʾ��ͣ���棻����0����,1����
uint8_t Gui_DataMovementModeIsNotDisplayPause(void);

//��̬����״̬���
void Gui_DataClearPostureToState(void);

//�����Ƿ���;����0�������ͷţ�1,������
uint8_t Gui_DataBspKeyPressedState(void);

//˯�����¼���
void Gui_DataSleepToRecalculate(void);

//��ȡ����mac��ַ
void Gui_DataGetBleMac(uint8_t *mac_data);

//��ȡ�豸�汾��
void Gui_DataGetVer(uint8_t *ver_data);

//��ȡ����״̬
uint8_t Gui_DataGetEncrypt(void);

//�������״̬
void Gui_DataClearCallingState(void);

//��ȡŮ����Ϣ������(ָ��)�ͳ���
uint8_t *Gui_DataGetPhysRmindData(uint16_t *content_len, uint8_t *page_index);

uint8_t Gui_DataIsReadPhysMessage(void);

uint8_t Gui_DataHaveUnreadPhysMessage(uint8_t reading_flag);

//���Ů���Ѷ�ȡ��Ϣ
void Gui_DataClearPhysMessage(void);

//��ȡ���ڴ���ʲô�׶�
// 0,�Ƿ�;1,����;2,��ȫ��;3,������;4,������ 
uint8_t Gui_DataGetPhysMenstrualPeriodType(void);

//�Ƿ�֧��Ѫѹ����
uint8_t Gui_DataIsBpFunction(void);

//ɾ��������������
void Gui_DataDelBleDiscRemind(void);

#endif  //__GUI_DATA_INTEFACE_H


