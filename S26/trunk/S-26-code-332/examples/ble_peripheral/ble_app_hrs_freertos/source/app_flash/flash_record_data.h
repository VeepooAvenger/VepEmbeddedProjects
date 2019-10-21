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

// Ѫ������������ʼ��
void Record_OxygenDataIndexInit(oxygen_data_index_t* p_index);

// �˶���������������ʼ��
void Record_SportHeartDataIndexInit(sport_heart_record_t* p_index);

// ������������
void Record_ClearAllData(void);

// ������������
void Record_ClearOneDayData(uint8_t day_id);

uint32_t Record_AddDailyData(uint8_t day_id, uint16_t offset, const daily_data_attr_t* p_attr);

uint32_t Record_GetDailyData(uint8_t day_id, uint16_t offset, daily_data_attr_t* p_attr);

// ��ȡ˯�߷�������(˯������ֻ��Ҫ��,ʱ,��,�˶���,�ǲ���Ϣ...)
uint32_t Record_GetSleepAnalysisData(uint8_t day_id, uint16_t offset,uint8_t* p_data);

uint32_t Record_AddOxygenData(uint8_t day_id, uint16_t offset, const blood_oxygen_attr_t* p_attr);

uint32_t Record_GetOxygenData(uint8_t day_id, uint16_t offset, blood_oxygen_attr_t* p_attr);

uint32_t Record_ModifyOahsFlag(uint8_t day_id, uint16_t offset, uint8_t flag, uint8_t *ox_result_buf);

// ����HRVɢ��ͼ����ֵ
uint32_t Record_AddScatterHeartData(uint8_t day_id, uint16_t offset, const scatter_heart_t* p_heart);

// ��ȡHRVɢ��ͼ����ֵ
uint32_t Record_GetScatterHeartData(uint8_t day_id, uint16_t offset, scatter_heart_t* p_heart);

uint32_t Record_AddSportHeartData(uint8_t block_id,uint16_t offset,const sport_heart_attr_t* p_attr);

uint32_t Record_GetSportHeartData(uint8_t block_id,uint16_t Offset,sport_heart_attr_t* p_attr);

void Record_GetSystemTimeIndex(uint16_t* p_index);

void Record_GetAlarmTimeParam(uint8_t index,alarm_time_config_t* p_alarm);

void Record_UpdateAlarmTimeParam(uint8_t index,const alarm_time_config_t* p_alarm);

// ��־���ݶ�ȡ
uint32_t Record_AddLogData(uint16_t offset, const log_data_attr_t* p_attr);

uint32_t Record_GetLogData(uint16_t offset, log_data_attr_t* p_attr);

//// ������Ϣ����
//uint32_t Record_AddUnreadMessageData(uint8_t message_index, const unread_message_detail_t *p_message);

//// ��ȡδ����Ϣ
//uint32_t Record_GetUnreadMessageData(uint8_t message_index, unread_message_detail_t *p_message);

//// �����Ѷ���Ϣ����
//uint32_t Record_UpdateReadMessageLen(uint8_t message_index, uint8_t page_index, uint8_t len);

#endif
