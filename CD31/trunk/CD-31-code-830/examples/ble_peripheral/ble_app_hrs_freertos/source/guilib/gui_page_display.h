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


// ��Ļ������
#define LED_ON_OFF_ON_DEF			1

//#define TEST_DBUG_UI_ENABLED    (0x00)

#define TEST_DBUG_UI_MAX_NUM    (22)

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

//�򿪱���
void Gui_LcdLedOn(void);

//LCD���ѣ��ܼӿ�LCD�������ݴ���
void Gui_LcdWakeUp(void);

//��������
void Gui_PageClearBackground(uint8_t s_column, uint8_t s_page, uint8_t column_size, uint8_t page_size);

//����ĳ���������ɫ����һ��
void Gui_PageRefreshBackground(uint8_t s_column, uint8_t s_page, uint8_t column_size, uint8_t page_size, uint16_t color);

//��ʾXXX
void Gui_PageDisXxxIcon(void);

//��ʾ������
void Gui_PageMain(uint16_t index, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second, uint8_t week, 
                  uint16_t am_pm, uint32_t step_cnt, uint8_t heart,
                  uint8_t ble_status, uint8_t charge, uint8_t battery_level, uint8_t menstrual,
                  uint16_t calories_cnt, uint32_t step_target, uint16_t sleep_time, uint8_t language);

//�Ʋ����뿨·�Ｏ�Ͻ���
void Gui_PageStepDistanceCalories(uint16_t index, uint32_t step_cnt, uint8_t step_prg,
                                  float distance_cnt,uint8_t uint_type,
                                  uint16_t calories_cnt, uint8_t language);

// �˶�ģʽ
void Gui_PageRunMode(uint16_t index, uint8_t language);

// �˶�ģʽ����չʾ����
void Gui_PageRunModeData(uint16_t index, uint32_t seconds, uint8_t heart, float distance, float calories, 
                         uint32_t step_cnt, uint8_t uint_type, bool lock, uint8_t hour, uint8_t minute, uint8_t animation_flag, uint8_t language);

// �˶�ģʽ-��������
void Gui_PageRunModeUnlock(uint16_t index, uint8_t language);
//
void Gui_PageRunModePause(uint16_t index, uint8_t pause_state, uint8_t language);

void Gui_PageRunModeOperate(uint16_t index, uint8_t pause_state, uint8_t operate_state, uint8_t hour, uint8_t minute, uint8_t language);

//
void Gui_PageRunModeExit(uint16_t index, uint8_t language);

//��ʾѪѹ����
void Gui_PageBP(uint16_t index, uint8_t bp_sp, uint8_t bp_dp, uint8_t test_mode, uint8_t test_state, uint8_t progress);

//��ʾ���ʽ���
void Gui_PageHeart(uint16_t index, uint8_t heart, uint8_t last_heart, uint8_t * heart_array, uint8_t max_heart, uint8_t min_heart, uint8_t language);

//˯�߽���
void Gui_PageSleep(uint16_t index, uint16_t time, uint16_t deep_time, uint16_t light_time, uint8_t quality, uint8_t language);

//���ֻ�����
void Gui_PageFindPhone(bool dynamic_find, uint16_t index, uint8_t language);

//������
void GUI_StopWatchPage(uint16_t index, uint8_t entry_flag,  uint8_t reset_flag, uint8_t run_flag, uint16_t total_second, uint8_t one_hundred_ms, 
						uint8_t system_hour, uint8_t system_minute, uint8_t language);

//����ʱ����
void Gui_PageCountDown(uint16_t index, bool dynamic, uint8_t run_state, uint32_t total_second, uint8_t language);

//���׼������
void Gui_PageCameraReady(uint16_t index);

//���OK����
void Gui_PageCameraOk(uint16_t index);

//��ʾMac�Ͱ汾��
void Gui_PageDisMacVer(uint16_t index, uint8_t *mac_data, uint8_t *ver_data);

//��������
void Gui_PagePowerOn(uint16_t index);

void Gui_PageTurnOff(uint16_t index, uint8_t *ble_name, uint8_t *mac_data, uint8_t *ver_data,uint8_t language);
//�ػ�����
void Gui_PagePowerOff(uint16_t index);

//���ӽ���
void Gui_PageAlarm(uint16_t index, uint8_t am_pm_format, uint8_t scene, uint8_t hour, uint8_t minute, uint8_t language);

//��������
void Gui_PageSit(uint16_t index, uint8_t language);

//�͵����ѽ���
void Gui_PageLowPower(uint16_t index);

//��س�����
void Gui_PageBatteryCharg(uint16_t index, uint8_t state, uint8_t bat_grade);

//�˶�Ŀ�����ѽ���
void Gui_PageStarget(uint16_t index, uint32_t step_target_cnt, uint8_t language);

//���ʱ�������
void Gui_PageHeartAlert(uint16_t index);

//��ʾ��ά���ǿƼ�����������
void Gui_PageFontTest(uint16_t index);

//��ʾ�������
void Gui_PageCallNotificationIcon(uint16_t index, uint8_t call_state, uint8_t language);

//��ʾ������Ϣ���ͽ���
void Gui_PageCallNotificationContent(uint32_t index, uint8_t *display_buf, uint16_t buf_len);

//��ʾ��Ϣ���ͽ���
void Gui_PageMessageNotificationIcon(uint16_t index, uint8_t msg_type, uint8_t page_index, uint8_t hour, uint8_t minute, uint8_t language);

//��ʾ��Ϣ���ͽ���
void Gui_PageMessageNotificationContent(uint16_t index, uint8_t *display_buf, uint16_t buf_len, uint8_t page_index);

//������������
void Gui_PageBleDiscRemind(uint16_t index);

//��ʾ��Ϣ���ͽ���02
void Gui_PageMessageNotificationIcon02(uint16_t index, uint8_t msg_type, uint8_t page_index, uint8_t hour, uint8_t minute, uint8_t language);

//��ʾ��Ϣ02
void Gui_PageMessageNotificationContent02(uint16_t index, uint8_t *display_buf, uint16_t buf_len, uint8_t page_index);

#endif
