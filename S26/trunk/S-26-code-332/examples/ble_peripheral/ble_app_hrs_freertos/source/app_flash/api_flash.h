#ifndef _API_FLASH_H_
#define _API_FLASH_H_

#include <stdint.h>
#include <stdarg.h>
#include "SystemTime.h"
#include "flash_data_type.h"
#include "flash_record_data.h"
#include "data_manage.h"

typedef struct
{
	uint8_t                   ui_style;                 // ui������
    uint8_t                   update_uimode;            // ����UI����ģʽ
    uint8_t                   Remind_flag;              //ĳ��Ʋ��Ƿ�����
    uint16_t                  movement_max_puse_time;   //�˶�ģʽ�������ͣʱ��
    uint16_t                  password;                 // ����
    uint16_t                  systemtime_index;         // ϵͳʱ������
    uint16_t                  sport_target;             // �˶�Ŀ��
    ///ui_gender_t               ui_gender_s;              // ��ŮUI�л�����
	bluetooth_name_t          bluetooth_name_s;         // ��������
    person_infomation_t       person_s;                 // �û���Ϣ
    turnwrist_config_t        turnwrist_s;              // ����������Ϣ
    heart_alert_config_t      heart_alert_s;            // �������ò���
    bp_config_t               bp_s;                     // Ѫѹ���ò���
    CONFIG_LANGUAGE_E         language_e;               // �������ò���
    dev_config_t              dev_config_s;             // �������ò���
    app_message_enable_t      message_enable_s;         // ANCS������Ϣ
    sedentary_config_t        sedentary_config_s;       // ����������Ϣ
    brightness_config_t       brightness_s;             // ���Ȳ���
    hourglass_config_t        hourglass_s;              // ����ʱ����
    alarm_time_config_t       alarm_time_s[20];         // �������
    time_range_config_t       blood_oxygen_config_s;    // Ѫ��������Ϣ
    product_infomation_t      product_infomation_s;     // ��Ʒ��Ϣ����
    flash_find_index_t        flash_index_s;            // FLASH����
    uint8_t                   bp_func_state;            // Ѫѹ����ģ��״̬
}flash_param_t;

extern flash_param_t* p_flash_param;

uint16_t Crc16(uint16_t crc, uint8_t *data,uint16_t len);

//Flash��ʼ������
//void Api_FlashParamInit(void);

// д����־ASCII
void Api_WriteLogText(uint8_t* p_content, uint8_t len);

// �����־����
void Api_ClearLogText(void);

// �����ճ�����
void Api_SaveDailyAttrData(const daily_data_attr_t* p_attr);

// ��ȡ�ճ���������
uint32_t Api_ReadDailyAttrData(uint8_t day_id,uint16_t package_offset,daily_data_attr_t* p_attr);

// ����Ѫ����������
void Api_SaveOxygenAttrData(const blood_oxygen_attr_t* p_attr, const scatter_heart_t* p_heart);

// ��ȡѪ����������
uint32_t Api_ReadOxygenAttrData(uint8_t day_id,uint16_t package_offset,blood_oxygen_attr_t* p_attr);

// ��ȡɢ��ͼ��������
uint32_t Api_ReadScatterHeartData(uint8_t day_id,uint16_t package_offset,scatter_heart_t* p_attr);

// �����˶���������
void Api_SaveSportHeartAttrData(const sport_heart_attr_t* p_attr);

// ��ȡ�˶���������
uint32_t Api_ReadSportHeartAttrData(uint8_t day_id, uint16_t package_offset, sport_heart_attr_t* p_attr);

// ��ȡ�ǲ���
uint32_t Api_GetStepAllCount(uint8_t app_day_id);

//����˯�߼���Ŀ��
void Api_SetSleepBlack(uint8_t black);

// ��ȡ˯�߻ص�
uint32_t Api_SleepCallback(uint8_t *data, uint16_t current_pkg, uint16_t pre_offset);

// ����
uint32_t Api_FlashDailyJumpBlock(void);

// ��FLASH����
uint32_t Api_ReadAllFlashData(uint8_t block_id,uint8_t offset,uint8_t* p_data);

// FLASH�ָ���������
uint32_t Api_FlashFatoryRestore(void);

// ����û�����
uint32_t Api_FlashClearUserData(void);
// ����û�����
uint32_t Api_FlashClearUserConfig(void);


// ����˯�߽������
uint32_t Api_SaveSleepResultData(uint8_t day_id, uint8_t* p_data);

// ��ȡ˯�����Ѵ���
uint32_t Api_GetSleepWakeCnt(uint8_t day_id);

// ��ȡ˯�߿��ֵ
uint8_t  Api_GetSleepMapDayID(uint8_t block_id);

// ��ȡ˯����ʱ��
uint16_t Api_GetSleepTotalTime(void);

// ��ȡ˯�߷ֿ�����
uint32_t Api_GetSleepPartData(uint8_t day_id,uint8_t offset,uint8_t* p_data);

//Flash�б���ϵͳʱ��
uint32_t Api_SaveSystemTime(uint16_t year,uint8_t month,uint8_t day,uint8_t hour,uint8_t minute);

//Flash�ж�ȡϵͳʱ��
uint32_t Api_ReadSystemTimeData(uint8_t* pdata);

// ��ʱ����FLASH����
bool Api_AdjustTimeClearFlash(const system_clock_time_t* p_history_time,const system_clock_time_t* p_current_time);

// �첽����APP���ÿ�����Ϣ
uint32_t Api_AsynUpdateAppConfig(void);

//FLASH�������ʸ澯����
uint32_t Api_UpdateHeartAlertParam(const heart_alert_config_t* p_alert);

//FLASH���·������
uint32_t Api_UpdateTurnWristParam(const turnwrist_config_t* p_turnwrist);

//FALSH����Ѫѹ���ò���
uint32_t Api_UpdateBloodConfig(const bp_config_t* p_config);

// ����FLASH ANCS����ʹ��
uint32_t Api_UpdateMessageEnableConfig(const app_message_enable_t *p_enable);

// ��ȡѪѹģʽ
BLOOD_MODE_E Api_GetBloodMode(void);

// ���¿���
uint32_t Api_UpdateDevConfig(const dev_config_t* p_dev_config_t);

// ���¾�������
uint32_t Api_UpdateSedentaryConfig(const sedentary_config_t* p_sedentary_config_t);

// ��������
uint32_t Api_UpdateGuiLanguage(CONFIG_LANGUAGE_E language);

// ��������
uint32_t Api_UpdateConnectPassword(uint16_t password);

// ����������Ϣ
uint32_t Api_UpdateBrightness(brightness_config_t* p_bright);

// ��������ʱ��
uint32_t Api_UpdateAlarmTime(uint8_t index,const alarm_time_config_t* p_alarm);

// �����û���Ϣ
uint32_t Api_UpdateUserInfomation(const person_infomation_t* p_person,uint16_t target);

// ��ȡ����CRC
uint16_t Api_GetAlarmCRC(void);

// ���µ���ʱ����
uint32_t Api_UpdateHourglassParam(const hourglass_config_t *p_hourglass_config);

// ����Ѫ������
uint32_t Api_UpdateBloodOxygenParam(const time_range_config_t* p_param);

// FLASH��ʼ��
void Api_FlashInit(void);

// ��ȡ�豸ID
uint32_t Api_GetDevSN(void);

// �豸ID��ʶ��
void Api_DevSNCheck(void);

// ���������豸����
//void Api_UpdateBluetoothName(const bluetooth_name_t* p_bluetooth_name);


uint32_t Api_UpdateUiStyle(uint8_t ui_style);

uint8_t Api_GetUiStyle(void);

//�޸�5����Ѫ��������־λ
void Api_ModifyOahsFlag(uint8_t flag, uint8_t *ox_result_buf);
#endif

