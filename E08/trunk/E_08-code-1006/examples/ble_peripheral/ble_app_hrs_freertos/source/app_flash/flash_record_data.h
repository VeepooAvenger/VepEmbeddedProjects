#ifndef _FLASH_RECORD_DATA_H_
#define _FLASH_RECORD_DATA_H_

#include <stdint.h>
#include "flash_data_type.h"

// ��־������ʼ��
void Record_LogDataIndexInit(log_data_index_t* p_index);

// ������ʼ��
void Record_BlockIndexInit(uint8_t*  p_index);

// �ճ�����������ʼ��
void Record_DailyDataIndexInit(daily_data_index_t* p_index);

//// ������������
//void Record_ClearAllData(void);

// ������������
void Record_ClearOneDayData(uint8_t day_id);

uint32_t Record_AddDailyData(uint8_t day_id, uint16_t offset, const daily_data_attr_t* p_attr);

uint32_t Record_GetDailyData(uint8_t day_id, uint16_t offset, daily_data_attr_t* p_attr);

void Record_GetSystemTimeIndex(uint16_t* p_index);

#if (!APP_USED_OLD_ALARM_ENABLE)
void Record_GetAlarmTimeParam(uint8_t index,alarm_time_config_t* p_alarm);

void Record_UpdateAlarmTimeParam(uint8_t index,const alarm_time_config_t* p_alarm);
#else

void Record_GetOldAlarmTimeParam(uint8_t *p_old_alarm);

void Record_UpdateOldAlarmTimeParam(const uint8_t *p_old_alarm);

#endif //APP_USED_OLD_ALARM_ENABLE

// ��־���ݶ�ȡ
uint32_t Record_AddLogData(uint16_t offset, const log_data_attr_t* p_attr);

uint32_t Record_GetLogData(uint16_t offset, log_data_attr_t* p_attr);


// �洢ECG �����������
uint32_t Record_AddEcgResultData(uint16_t offset, const ecg_result_data_t *p_ecg_data);

// ��ȡECG �����������
uint32_t Record_GetEcgResultData(uint16_t offset, ecg_result_data_t *p_ecg_data);

// ����ECG �����������
uint32_t Record_ClearEcgResultBlock(uint8_t block_id, uint8_t block_num);

// ECG���ݴ洢������ʼ��
void Record_EcgDataIndexInit(ecg_data_index_t *p_index);


// ����ͳ�ƴ洢
uint32_t Record_AddPowerInfoData(uint8_t day_id, const power_statistics_t *p_power);

uint32_t Record_GetPowerInfoData(uint8_t day_id, power_statistics_t *p_power);


#endif
