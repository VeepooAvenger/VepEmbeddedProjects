#ifndef _PULSE_DATA_TYPE_H_
#define _PULSE_DATA_TYPE_H_

#include <stdint.h>

// !ע��:����ĺ궨�����Ű����ļ�ʵ����Ŀ������
#define PULSE_DATA_TYPE     1

// FLASH���������ṹ��
typedef struct
{
	uint8_t  time_data_update:1;    //ʱ������FLASHд��
	uint8_t  daily_data_update:1;   //�ճ�����FLASHд��
	uint8_t  oxygen_data_update:1;  //Ѫ������FLASHд��
	uint8_t  sleep_data_update:1;   //˯������FLASHд��
	
	uint8_t  clear_current_day:1;   //�����������
	uint8_t  clear_all_day:1;       //�����������
	uint8_t  jump_block:1;          //FLASH����
	uint8_t  resv:1;                //Ԥ��
}flash_opt_param;

// �ϴ��ɼ�����ö������
typedef enum
{
	UPLOAD_ADC_GSENSOR_EVENT = 1,      //�ϴ�GsensorԭʼADC�����¼�
	UPLOAD_ADC_GREEN_EVENT = 2,        //�ϴ��̹�ԭʼADC�����¼�
	UPLOAD_ADC_OXYGEN_EVENT = 3,       //�ϴ�Ѫ��ԭʼADC�����¼�
	UPLOAD_ADC_NULL_EVENT,
}UPLOAD_ADC_EVENT;

// ���������ṹ��
typedef struct
{
	uint16_t  heart_btn:1;    // �������������㷨
	uint16_t  heart_app:1;    // App���������㷨
	uint16_t  heart_time:1;   // ��ʱ�������������㷨
	
	uint16_t  bp_btn:1;       // ����Ѫѹ�㷨
	uint16_t  bp_app:1;       // AppѪѹ�㷨
	uint16_t  bp_time:1;      // ��ʱ����Ѫѹ�㷨
	
	uint16_t  bp_init:1;      // Ѫѹ�㷨��ʼ��
	uint16_t  heart_init:1;   // �����㷨��ʼ��
	uint16_t  wear_init:1;    // ����㷨��ʼ��
	
	uint16_t  resv:7;         // ��������7�ֽ�
}measure_param_t;

//�ǲ���ʷ����
typedef struct
{
	uint32_t  power_on_step;    // �ֻ������ǲ�ֵ
	
	uint32_t  daily_step;       // �ճ���ʷ�ǲ�����
	uint32_t  daily_sport;      // �ճ���ʷ�˶�������
	
	uint32_t   oxygen_step;     // Ѫ����ʷ�ǲ�����
	uint32_t   oxygen_sport;    // Ѫ����ʷ�˶�������
}history_pedometer_param;

typedef struct
{
	uint8_t  index;             // �����ǲ���������
	uint8_t  step[5];           // �����ǲ�����
	
	uint32_t history_step;      // ������ʷ�ǲ�����
}sedentary_pedometer_param;

//========================================
//   *****����������������*******
//========================================
#define WEAR_DATA_POOL_SIZE       40
#define WEAR_DATA_CACHE_SIZE      40
#define PULSE_DATA_POOL_SIZE      1000
#define PULSE_DATA_CACHE_SIZE     128

// �������ݻ������ݽṹ��
typedef struct
{
	uint16_t  wear_read_pos;
	uint16_t  wear_wait_send_len;
	uint16_t  wear_send_package;
	
	uint16_t  pulse_read_pos;
	uint16_t  pulse_wait_send_len;
	uint16_t  pulse_send_package;
	
	uint16_t  wear_cache_update_cnt;          // ����������������
	uint16_t  wear_pool_update_total_cnt;     // ����ظ�����������
	uint16_t  pulse_cache_update_cnt;         // ���建�����������
	uint16_t  pulse_pool_update_total_cnt;    // ����ظ�����������
	
	uint16_t  wear[WEAR_DATA_POOL_SIZE+WEAR_DATA_CACHE_SIZE];     // ������ݳ�
	uint16_t  pulse[PULSE_DATA_POOL_SIZE+PULSE_DATA_CACHE_SIZE];  // �������ݳ�
}pulse_data_pool_t;

#endif
