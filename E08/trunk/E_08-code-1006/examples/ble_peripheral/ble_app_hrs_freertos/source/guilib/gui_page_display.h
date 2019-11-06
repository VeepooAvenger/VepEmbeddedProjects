/***************************Copyright(c)***********************************
**                       Veepoo Tech
**                  COPYRIGHT (C) 2016-2019
**-----------File Info-----------------------------------------------------
** File        name:     gui_page_display.c
** Last modified Date:   2017-08-01
** Last SYS_VERSION:     0.0.0
** Descriptions:            
**-------------------------------------------------------------------------
** Created  by:          WJ
** Created  date:        2017-08-01
** SYS_VERSION:          0.0.0
** Descriptions:         
**-------------------------------------------------------------------------
** Modified by:				
** Modified date:  			          
** SYS_VERSION:                 
** Descriptions:                        
***************************************************************************/

#ifndef _GUI_LIB_H_
#define _GUI_LIB_H_

#include <stdint.h>
#include <stdbool.h>
#include "common_data_type.h"
// ��Ļ������
#define LED_ON_OFF_ON_DEF			1//

#define LED_IS_SMALL			    1//

extern const uint8_t disp_delay_time;

#define TEST_CLEAR_SCREEN_PARAM     0//1//������������Ƿ���ȫ

//#define TEST_DBUG_UI_ENABLED    (0x00)

#define TEST_DBUG_UI_MAX_NUM    (22)

#define GUI_DISP_MESSAGE_MAXLEN     50
typedef struct
{
    uint8_t message_touch_num;                  //�Ϸ���Ϣ����ѡ�е�ѡ��  0~3
    uint8_t disp_data_num;                      //������Ҫ��ʾ���ݵ�����
    uint8_t message_type[3];                    //�����Ϣ��������ͼ��   0:����û��Ϣ��1~17��Ϣ����
    uint8_t p_message_content[3][GUI_DISP_MESSAGE_MAXLEN];           //��Ϣ������������
    uint16_t content_len[3];                    //��Ϣ�������ݳ���
    system_clock_time_t message_time_s[3];         // ֪ͨ��ʱ��
}page_message_param_t;

//����UI������
void Gui_DisDbugUi(void);
    
//LCD��ʼ��
void Gui_LcdDriverInit(void);

//LCD����Ļ
void Gui_LcdDriverOn(void);

//LCD�ر���Ļ
void Gui_LcdDriverOff(void);

//�رձ���
void Gui_LcdLedOff(void);
void Gui_LcdLedOn(void);

//LCD���ѣ��ܼӿ�LCD�������ݴ���
void Gui_LcdWakeUp(void);

//��������
void Gui_PageClearBackground(uint8_t s_column, uint8_t s_page, uint8_t column_size, uint8_t page_size);

//����ĳ���������ɫ����һ��
void Gui_PageRefreshBackground(uint8_t s_column, uint8_t s_page, uint8_t column_size, uint8_t page_size, uint16_t color);


//��ʾXXX
void Gui_PageDisXxxIcon(void);

////��ʾ�������ʱ�Ӻ�����
//void Gui_PageMainDateHour(uint16_t index, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t week, 
//                          uint8_t language, uint8_t am_pm_format, uint32_t step_cnt);

////��ʾ����������ͼ��
//void Gui_PageMainAssisant(uint16_t index, uint32_t step_cnt, uint8_t ble_status, uint8_t am_pm_format,
//                          uint8_t charge, uint8_t battery_level, uint8_t language, uint8_t menstrual);
void Gui_PageMain(uint16_t index, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, float distance_cnt, uint8_t week, 
                  uint16_t am_pm, uint32_t step_cnt, uint8_t heart,
                  uint8_t ble_status, uint8_t charge, uint8_t battery_level, uint8_t menstrual,
                  uint16_t calories_cnt, uint32_t step_target, uint16_t sleep_time, uint8_t language, uint8_t *cls_flag);

void Gui_PageChooseMain(uint16_t index, uint8_t Choose, uint8_t *cls_flag);

//��ʾ�Ʋ�����
void Gui_PageStep(uint32_t step_cnt, uint8_t target_cnt, uint16_t index, uint8_t hour, uint8_t minute, uint8_t language, uint8_t *cls_flag,
                  float distance_cnt, uint8_t uint_type, uint16_t calories_cnt);

//��ʾ��̽���
void Gui_PageDistance(float dis_count, uint16_t index, uint8_t uint_type, uint8_t hour, uint8_t minute, uint8_t language, uint8_t *cls_flag);

//��ʾ��·��
void Gui_PageCalories(uint16_t calories_cnt, uint16_t index, uint8_t hour, uint8_t minute, uint8_t language, uint8_t *cls_flag);

// �˶�ģʽ
void Gui_PageRunMode(uint16_t index, uint8_t hour, uint8_t minute, uint8_t language, uint8_t *cls_flag);

// ���˶�ѡ��ģʽ
void Gui_PageRunModeSelect(uint16_t index, uint8_t page, uint8_t num, uint8_t* switch_tmp, uint8_t language);

// �˶�ģʽ����չʾ����
void Gui_PageRunModeData( uint16_t index, uint32_t seconds, uint8_t heart, float distance, float calories, 
                         uint32_t step, uint8_t uint_type, uint8_t animation_flag, uint8_t language, uint8_t *cls_flag, 
                         uint8_t runmode_type_show, uint8_t face_num,uint8_t runmode_type, uint8_t hour, uint8_t minute,
                         uint8_t Operation_state, uint8_t run_state, uint32_t Speed_distance);
// �˶�ģ��������
void Gui_PageRunModeDataResult(uint16_t index, uint8_t data_valid, uint32_t seconds, uint8_t heart, float distance, float calories, 
                                uint32_t step_cnt, uint8_t uint_type, bool lock, uint8_t hour, uint8_t minute, uint8_t animation_flag, 
                                uint8_t language, uint8_t runmode_type, uint8_t runmode_show_type, uint8_t page, uint32_t Speed_distance);

// �˶�ģʽ��ͣ��ʼ
void Gui_PageRunModePause(uint16_t index, uint8_t pause_state, uint8_t language, uint8_t *cls_flag);

// �˶�ģʽ�˳�
void Gui_PageRunModeExit(uint16_t index, uint8_t language, uint8_t *cls_flag);

//��ʾѪѹ����
void Gui_PageBP(uint16_t index, uint8_t bp_sp, uint8_t bp_dp, uint8_t test_mode, uint8_t hour, uint8_t minute, uint8_t test_state, uint8_t *cls_flag, uint8_t language);

//��ʾ���ʽ���
void Gui_PageHeart(uint16_t index, uint8_t heart, uint8_t last_heart, uint8_t hour, uint8_t minute, uint8_t language, uint8_t *cls_flag);

//˯�߽���
void Gui_PageSleep(uint16_t index,uint16_t time, uint8_t hour, uint8_t minute, uint8_t language, uint8_t *cls_flag,
                    uint16_t deep_time,uint16_t light_time, uint8_t quality);

//���ֻ�����
void Gui_PageFindPhone(bool dynamic_find, uint16_t index, uint8_t hour, uint8_t minute, uint8_t language, uint8_t *cls_flag);

//����δ���ӽ���
void Gui_PageBleUnconnected(uint16_t index ,uint8_t language);

//������
void GUI_StopWatchPage(uint16_t index, uint8_t entry_flag, uint8_t run_flag, uint16_t total_second, uint8_t one_hundred_ms,
						uint8_t hour, uint8_t minute, uint8_t language, uint8_t *cls_flag);

//����ʱ����
void Gui_PageCountDown(uint16_t index, uint8_t dynamic, uint32_t total_second, uint8_t hour, uint8_t minute, uint8_t language, uint8_t *cls_flag, uint8_t run_state);

//����ʱ��������
void Gui_PageCountDownTimeOut(uint16_t index,  uint8_t language, uint8_t *cls_flag);

//���׼������
void Gui_PageCameraReady(uint16_t index, uint8_t camera_state, uint8_t *cls_flag);

//��ʾMac�Ͱ汾��
void Gui_PageDisMacVer(uint16_t index, uint8_t *mac_data, uint8_t *ver_data, uint8_t language, uint8_t *cls_flag);

//�ػ�����
void Gui_PagePower(uint16_t index, uint8_t *mac_data, uint8_t *ver_data, uint8_t hour, uint8_t minute, uint8_t language, uint8_t *cls_flag,uint8_t *ble_name);

//���ӽ���
void Gui_PageAlarm(uint16_t index, uint8_t am_pm_format, uint8_t scene, uint8_t hour, uint8_t minute, uint8_t language, uint8_t *cls_flag);

//��������
void Gui_PageSit(uint16_t index, uint8_t language, uint8_t *cls_flag);

//�͵����ѽ���
void Gui_PageLowPower(uint16_t index, uint8_t *cls_flag);

//��س�����
void Gui_PageBatteryCharg(uint16_t index, uint8_t state, uint8_t *cls_flag);


//�˶�Ŀ�����ѽ���
void Gui_PageStarget(uint16_t index, uint32_t step_target_cnt, uint8_t language, uint8_t *cls_flag);

//���ʱ�������
void Gui_PageHeartAlert(uint16_t index, uint8_t *cls_flag);
//������������
void Gui_LowOxygenAlertPage(uint16_t index, uint8_t *cls_flag);
//��ʾ��ά���ǿƼ�����������
void Gui_PageFontTest(uint16_t index, uint8_t *cls_flag);

//��ʾ�������
void Gui_PageCallNotificationIcon(uint16_t index, uint8_t call_state, uint8_t language, uint8_t *cls_flag);

//��ʾ������Ϣ���ͽ���
void Gui_PageCallNotificationContent(uint16_t index, uint8_t *display_buf, uint16_t buf_len, uint8_t *cls_flag);

//��ʾ��Ϣ���ͽ���
void Gui_PageMessageNotificationIcon(uint16_t index, uint8_t msg_type, uint8_t page_index, uint8_t hour, uint8_t minute, uint8_t *cls_flag);

//��ʾ��Ϣ���ͽ���
uint8_t Gui_PageMessageNotificationContent(uint16_t index, uint8_t *display_buf, uint16_t buf_len, uint8_t *cls_flag);

//������������
void Gui_PageBleDiscRemind(uint16_t index, uint8_t *cls_flag);

//Ѫ������
void Gui_PageOxygen(uint16_t index, uint8_t progress, uint8_t oxygen_cnt, uint8_t hour, uint8_t minute, uint8_t language, uint8_t *cls_flag);
    
void Gui_PagePtt(uint16_t index, uint8_t *cls_flag);
void Gui_PagePttItem1(uint16_t index, uint8_t state, uint8_t *cls_flag);

void Gui_PageEcg(uint16_t index, uint8_t hour, uint8_t minute, uint8_t language, uint8_t *cls_flag);
void Gui_PageEcgItem1(uint16_t index, uint8_t state, uint8_t heart, uint8_t language, uint8_t *cls_flag);
void Gui_PageEcgItem2(uint16_t index, uint8_t state, uint8_t language, uint8_t *cls_flag);

//��ʾ��Ϣ
void Gui_PageMessage3(uint16_t index, uint8_t language);
//��ʾ��Ϣ���ͽ���02
void Gui_PageMessageNotificationIcon02(uint16_t index, uint8_t msg_type, uint8_t page_index, uint8_t hour, uint8_t minute, uint8_t language);

//��ʾ��Ϣ02
uint16_t  Gui_PageMessageNotificationContent02(uint16_t index, uint8_t *display_buf, uint16_t buf_len, uint8_t page_index);


//���ý���
void Gui_Setting(uint16_t index, uint8_t set_type, uint8_t language);

void Gui_PageLightLevelSet(uint16_t index, uint8_t light_level, uint8_t language);

void Gui_PageLightTimeSet(uint16_t index, uint8_t linght_time ,uint8_t language);

void Gui_PageMainStyleSet(uint16_t index, uint8_t language);

//��ʾ���๦�ܽ���
void Gui_PageFunct(uint16_t index, uint8_t language);
//��ʾ���๦�ܽ���ѡ��    
void Gui_PageFunct2(uint16_t index, uint8_t page, uint8_t num, uint8_t language);

void Gui_PageMessageSwitch(uint16_t index, uint8_t page, uint8_t num, uint8_t* switch_tmp, uint8_t language);
    
void Gui_PageSwitch(uint16_t index, uint8_t page, uint8_t num, uint8_t* switch_tmp, uint8_t language);
    
void Gui_PageClearData(uint16_t index, uint8_t flg, uint8_t language);

void Gui_PageDispMessage(uint16_t index, page_message_param_t message_para, uint8_t language);


#endif
