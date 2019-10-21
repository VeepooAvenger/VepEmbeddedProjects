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

//LCD��ʼ��
void Gui_LcdDriverInit(void);

//LCD����Ļ
void Gui_LcdDriverOn(void);

//LCD�ر���Ļ
void Gui_LcdDriverOff(void);

//�رձ���
void Gui_LcdLedOff(void);

//LCD���ѣ��ܼӿ�LCD�������ݴ���
void Gui_LcdWakeUp(void);

//��������
void Gui_PageClearBackground(uint8_t s_column, uint8_t s_page, uint8_t column_size, uint8_t page_size);

//����ĳ���������ɫ����һ��
void Gui_PageRefreshBackground(uint8_t s_column, uint8_t s_page, uint8_t column_size, uint8_t page_size, uint16_t color);

//��ʾXXX
void Gui_PageDisXxxIcon(void);

//��ʾ������ʱ��ı���ɫ
void Gui_PageMainBackGround(void);

//��ʾ�������ʱ�Ӻ�����
void Gui_PageMainDateHour(uint8_t month,uint8_t day,uint8_t hour,uint8_t minute,uint8_t week, uint8_t language, 
                          uint16_t am_pm_format, uint8_t phys_menstrual, uint16_t step_prg);

//��ʾ����������ͼ��
void Gui_PageMainAssisant(uint8_t index, uint8_t ble_status, uint8_t am_pm_format,
                          uint8_t battery_status, uint8_t battery_level, uint8_t language, uint32_t step_all, uint8_t menstrual);

//��ʾ�Ʋ�����
void Gui_PageStep(uint32_t step_cnt, uint16_t target_cnt, uint8_t index, uint8_t language);

//��ʾ��̽���
void Gui_PageDistance(float dis_count, uint8_t index, uint8_t uint_type, uint8_t language);

//��ʾ��·��
void Gui_PageCalories(float calorie, uint8_t index, uint8_t language);

//��ʾ���ʽ���
void Gui_PageHeart(uint8_t index,uint8_t heart, uint8_t language);

//˯�߽���
void Gui_PageSleep(uint8_t index,uint16_t time, uint8_t language);

//���ֻ�����
void Gui_PageFindPhone(bool dynamic_find, uint8_t index);

//������
void GUI_StopWatchPage(bool entry, uint16_t total_second, uint8_t one_hundred_ms, uint8_t index);

//����ʱ����
void Gui_PageCountDown(bool dynamic, uint8_t index);

//���׼������
void Gui_PageCameraReady(uint8_t index);

//���OK����
void Gui_PageCameraOK(uint8_t index);

//��ʾMac�Ͱ汾��
void Gui_PageDisMacVer(uint8_t index, uint8_t *mac_data, uint8_t *ver_data);

//�ػ�����
void Gui_PagePower(void);

//���ӽ���
void Gui_PageAlarm(uint8_t am_pm_format, uint8_t scene, uint8_t hour, uint8_t minute, uint8_t language, uint8_t index);

//��������
void Gui_PageSit(uint8_t index);

//�͵����ѽ���
void Gui_PageLowPower(uint8_t index);

//��س�����
void Gui_PageBatteryCharg(uint8_t index, uint8_t state);

//�˶�Ŀ�����ѽ���
void Gui_PageStarget(uint8_t index, uint32_t target_step, uint8_t language);

//���ʱ�������
void Gui_PageHeartAlert(uint8_t index);

//��ʾ��ά���ǿƼ�����������
void Gui_PageFontTest(void);

//��ʾ�������
void Gui_PageCallNotificationIcon(uint8_t call_state, uint8_t index);

//��ʾ������Ϣ���ͽ���
void Gui_PageCallNotificationContent(uint8_t *display_buf, uint16_t buf_len);

//��ʾ��Ϣ���ͽ���
void Gui_PageMessageNotificationIcon(uint8_t msg_type, uint8_t index);

//��ʾ��Ϣ���ͽ���
uint8_t Gui_PageMessageNotificationContent(uint8_t *display_buf, uint16_t buf_len, uint8_t index);

// �˶�ģʽ
void Gui_PageRunMode(uint8_t index, uint8_t language);

// �˶�ģʽ����չʾ����
void Gui_PageRunModeData(uint32_t seconds, uint8_t heart, float distance, float calories, 
                         uint32_t step, uint8_t uint_type, uint8_t language);

// �˶�ģʽ��ͣ��ʼ
void Gui_PageRunModePause(uint8_t pause_state, uint8_t language);

// �˶�ģʽ�˳�
void Gui_PageRunModeExit(uint8_t language);

//��ʾѪ��У׼����
void Gui_OxygenCalibration(uint8_t index, uint8_t progress, uint8_t language);

//��ʾѪ������
void Gui_OxygenPage(uint8_t index, uint8_t ox_count, uint8_t prg_cnt, uint8_t ox_error);

//������������
void Gui_LowOxygenAlertPage(uint8_t index);


// ��������
void Gui_PageCartoon(uint8_t index);

// Ѫѹ
void Gui_PageBP(uint8_t index, uint8_t bp_sp, uint8_t bp_dp, uint8_t test_mode, uint8_t test_state);



void Gui_ButtonDynamic(uint16_t index/*, uint8_t key_press_state*/);

#endif
