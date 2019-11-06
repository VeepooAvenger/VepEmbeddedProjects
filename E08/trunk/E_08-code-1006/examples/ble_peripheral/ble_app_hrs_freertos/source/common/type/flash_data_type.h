#ifndef _FLASH_DATA_TYPE_H_
#define _FLASH_DATA_TYPE_H_

#include <stdint.h>
#include "common_data_type.h"

/*===================================================================
 *                            ����FLASH����ַ           
 *===================================================================
 */

#define PRODUCT_INFOMATION_BASE_ADDR    0x73000     // ��Ʒ��Ϣ����ַ
#define PRODUCT_INFOMATION_TOTAL_SIZE   0x1000      // ��Ʒ��Ϣ��С

//���Ӻ�ϵͳ���ò�������4K
#define ALARM_CONFIG_BASE_ADDR          0x72000     // ����
#define SYSTEM_PARAM_BASE_ADDR          0x72000     // ϵͳ��������ַ
#define APP_CONFIG_BASE_ADDR            0x72400     // APP���û���ַ

//ϵͳʱ��4K
#define SYSTEM_TIME_BASE_ADDR           0x71000     // ϵͳʱ��


//˯������  (�ĳ�Ƭ��FLASH)
#define RECORD_SLEEP_BASE_ADDR          0x00001000//0x6E000     // ˯�����ݻ���ַ
#define RECORD_SLEEP_DATA_SIZE          0x1000      // ÿ�����ݴ�С4KB
#define RECORD_SLEEP_TOTAL_SIZE         0x3000      // ˯���ܼ�12K
// ˯�����ݵ�ַ��
#define RECORD_SLEEP_DATA_ADDR(day_id, offset)       (uint32_t)(RECORD_SLEEP_BASE_ADDR + RECORD_SLEEP_DATA_SIZE*(day_id) + offset)
// ˯������ƫ�Ƶ�ַ
#define SLEEP_BASIC_CHECK_OFFSET        3            // ˯�߻�����Ϣ��ʼƫ����
#define SLEEP_BASIC_INFO_OFFSET         13           // ˯�߻�����Ϣ��ʼƫ����
#define SLEEP_INSOMINA_OFFSET           51           // ˯��ʧ����Ϣ��ʼƫ����
#define SLEEP_CURVE_MIG_OFFSET          97           // ˯��������Ϣ��ʼƫ����
// ˯����Ϣ����
#define SLEEP_BASIC_CHECK_LEN           8            // ˯����ϢУ�鳤��
#define SLEEP_BASIC_INFO_LEN            36           // ˯�߻�����Ϣ����
#define SLEEP_INSOMINA_LEN              44           // ˯��ʧ����Ϣ����

// ˯����������
#define SLEEP_ONE_DAY_MAX_TIMES         10           // ÿ��������
#define SLEEP_PER_DATA_MAX_SIZE         2980         // ÿ��˯����Ϣ����󳤶�


//�ճ�����
#define RECORD_DAILY_BASE_ADDR          0x00004000//0x59000     // ȫ�����ݻ���ַ
#define RECORD_DAILY_DATA_SIZE          0x9000      // �ճ����ݴ�С(36KB)   
#define RECORD_DAILY_ONE_DAY_SIZE       0x9000      // ȫ�����ݵĴ�С(36KB)
#define RECORD_DAILY_PER_DATA_SIZE      0x78        // ÿ�����ݵĴ�С(120B), ȷ����4�ı���
#define RECORD_DAILY_TOTAL_SIZE         0x1B000     // �������ݴ�С108K

#define RECORD_POWER_INFO_SIZE          16          // ���ڹ���ͳ�ƣ��洢��һ������24Byte

// �ճ����ݵ�ַ��
#define RECORD_DAILY_DATA_ADDR(day_id, offset)       (uint32_t)(RECORD_DAILY_BASE_ADDR + RECORD_DAILY_ONE_DAY_SIZE*(day_id) + offset * RECORD_DAILY_PER_DATA_SIZE)
#define RECORD_DAILY_DATA_MAX_CNT       307         // ȫ����Ա���������������
// ����һЩ�Ϸ��Լ��(���ڷ��������������2Сʱ, ���Ի����26Сʱ����: 26x60/5 = 312)
#define RECORD_DAILY_CONDITION_VALID(day_id, offset)  ((day_id < 3)&&(offset < RECORD_DAILY_DATA_MAX_CNT))

//�˶�ģʽ       
#define RECORD_MOVEMENT_BASE_ADDR           0x0001F000//0x53000     // �˶�ģʽ�Ļ���ַ
#define RECORD_MOVEMENT_ONE_TIMES_SIZE      0x2000      // �˶�ģʽ�����ܴ�С(8KB)
#define RECORD_MOVEMENT_TOTAL_SIZE          0x6000      // �˶�ģʽ�����ܴ�С(24KB)
#define RECORD_MOVEMENT_HEAD_OFFSET         0x50        // �˶�ģʽ���ݵ�ƫ����
// �˶�ģʽ���ݵ�ַ��
#define RECORD_MOVEMENT_HEAD_ADDR(block_id, offset)   (uint32_t)(RECORD_MOVEMENT_BASE_ADDR + (RECORD_MOVEMENT_ONE_TIMES_SIZE*block_id) + (offset*0))
#define RECORD_MOVEMENT_DATA_ADDR(block_id, offset)   (uint32_t)(RECORD_MOVEMENT_BASE_ADDR + RECORD_MOVEMENT_HEAD_OFFSET + (RECORD_MOVEMENT_ONE_TIMES_SIZE*block_id) + (offset*20))


// ECG�ֶ������������        
#define RECORD_ECG_BASE_ADDR            0x00025000//0x51000     // ECG�Ļ���ַ
#define RECORD_ECG_ONE_TIMES_SIZE       0x1000      // ECG�����ܴ�С(4KB)
#define RECORD_ECG_TOTAL_SIZE           0x2000      // ECG�����ܴ�С(8KB)
#define RECORD_ECG_PER_DATA_SIZE        0x18        // ECG���ݵ�ƫ���� 24 Byte
#define RECORD_ECG_MAX_CNT              341         // ECG���ݵ�������� 4096*2/ 24
#define RECORD_ECG_ONE_BLOCK_MAX_CNT    170         // ECG�����ڵ�һ���������� 4096/ 24
// ECG�ֶ�����������ݵ�ַ��
#define RECORD_ECG_DATA_ADDR(offset)            (uint32_t)(RECORD_ECG_BASE_ADDR + (offset*RECORD_ECG_PER_DATA_SIZE))
#define RECORD_ECG_DATA_ERASE_ADDR(block_id)            (uint32_t)(RECORD_ECG_BASE_ADDR + (block_id*RECORD_ECG_ONE_TIMES_SIZE))
// ����һЩ�Ϸ��Լ��(4096*2/24 = 341)
#define RECORD_ECG_CONDITION_VALID(offset)      (offset < RECORD_ECG_MAX_CNT)


//��־
#define LOG_BASE_ADDR                   0x70000//0x50000      // ��־����ַ
#define LOG_DATA_SIZE                   0x1000       // ��־���ݴ�С(4KB)
// ��־���ݵ�ַ��
#define LOG_DATA_ADDR(offset)  (uint32_t)(LOG_BASE_ADDR+offset)


// Flash��ȡ���ݵ�ַ��
#define RECORD_FLASH_BLOCK_DATA_ADDR(block_id, offset)  (uint32_t)(SYSTEM_TIME_BASE_ADDR + (block_id)*1024 + offset)
#define RECORD_FLASH_BLOCK_DATA_SIZE                    (PRODUCT_INFOMATION_BASE_ADDR - SYSTEM_TIME_BASE_ADDR)  
//#define RECORD_FLASH_BLOCK_DATA_ADDR(block_id, offset)  (uint32_t)(RECORD_ECG_BASE_ADDR + (block_id)*1024 + offset)
//#define RECORD_FLASH_BLOCK_DATA_SIZE                    (PRODUCT_INFOMATION_BASE_ADDR - RECORD_ECG_BASE_ADDR)  
//#define RECORD_FLASH_BLOCK_USER_DATA_SIZE               (SYSTEM_TIME_BASE_ADDR - RECORD_ECG_BASE_ADDR)   


// ������־�Ϸ��Լ��
#define LOG_CONDITION_VALID(offset)  (offset < 256)




/*===================================================================
 *                            ����FLASH�ṹ��           
 *===================================================================
 */
 
#pragma pack (1)
#pragma anon_unions
 

//�ճ�����
typedef struct
{
    uint8_t    month;                   //��
    uint8_t    day;                     //��
    uint8_t    hour;                    //ʱ
    uint8_t    minute;                  //��
    uint16_t   step;                    //�Ʋ�
    uint16_t   sport;                   //�˶���
    uint16_t   distance;                //����
    uint16_t   calories;                //��·��
    
    uint8_t    gesture;                 //��̬
    uint8_t    wear;                    //���
    
    uint8_t    sleep_state[6];          //˯��״̬
    uint8_t    pulse_frequency[5];      //����
    uint8_t    heart_rate[5];           //����
    uint8_t    breathing_rate[5];       //������
    
    uint8_t    hrv_type;
    uint8_t    hrv_value[50];
    
    uint8_t    bp_h;                    //Ѫѹ-��ѹ
    uint8_t    bp_l;                    //Ѫѹ-��ѹ
    
    uint8_t    oxgen_value[5];          //Ѫ��
    uint8_t    breath_stop_cnt[5];      //������ͣ����������һ����������0
    uint8_t    osahs_degree[5];         //������ͣ���������һ����������0
	uint8_t    low_spo2_time[5];        //����(ȱ��)ʱ�䣬����һ����������0
    
    uint8_t    cardiac_load[5];             //���ฺ��
    uint8_t    spo2_modification_flag[5];   //5����Ѫ��������־λ�����һ����ֵ
    
    uint8_t    resv[2];
}daily_data_attr_t;

// ECG����
typedef struct
{
    uint8_t    mark;                    //��־
    uint16_t   year;                    //��
    uint8_t    month;                   //��
    uint8_t    day;                     //��
    uint8_t    hour;                    //ʱ
    uint8_t    minute;                  //��
    uint8_t    second;                  //��
    uint8_t    ecg_result_data[14];     //ECG�������
    uint8_t    ecg_res[2];              //����
}ecg_result_data_t;

// ��־���ݸ�ʽ
typedef struct
{
    uint8_t   text[64]; // �ı�����
}log_data_attr_t;

// ��Ʒ������Ϣ�����ṹ��
typedef struct
{
    uint8_t master_sn[4];          // �豸ʶ����1
    uint8_t company_name[8];       // ��˾����
    uint8_t product_version[8];    // ��Ʒ�ͺ�
    uint8_t hardware_version[4];
    uint8_t firmware_version[4];
    uint8_t product_date[4];
    uint8_t agent_id[8];
    uint8_t product_batch[8];
    uint8_t product_count[8];
    uint8_t slave_sn[4];
    uint8_t product_modify[4];
    uint8_t bootloader_version[8];
    uint8_t application_version[8];
    uint8_t pin_config[4];
    //WJ.20181011.����
    uint8_t ble_name[8];               //��������
    uint8_t bp_flag;                    //Ѫѹ֧��
    uint8_t sit_flag;                   //����֧��
    uint8_t retain[2];                  //����  
}product_infomation_t;

//GUI֧������ö�ٶ���
typedef enum
{
    CONFIG_LANGUAGE_CHINESE = 1,  //����
    CONFIG_LANGUAGE_ENAGLISH, //Ӣ��
    CONFIG_LANGUAGE_JAPANESE, //����
    CONFIG_LANGUAGE_KOREAN,   //����
    CONFIG_LANGUAGE_GERMAN,   //����
    CONFIG_LANGUAGE_RUSSIAN,  //����
    CONFIG_LANGUAGE_SPANISH,  //��������
    CONFIG_LANGUAGE_ITALIARN, //�������
    CONFIG_LANGUAGE_FRENCH,   //����
    CONFIG_LANGUAGE_VIETNAMESE,   //Խ����
    CONFIG_LANGUAGE_PORTUGUESE,   //��������
    CONFIG_LANGUAGE_TRADITIONAL,  //����
}CONFIG_LANGUAGE_E;

// ��Ϣ���Ϳ���
typedef union 
{
    uint16_t  type;
    struct data
    {
        uint16_t call:1;       // ����
        uint16_t sms:1;        // ����Ϣ
        uint16_t wechat:1;     // ΢�ſ���
        uint16_t qq:1;         // qq����
        uint16_t sina:1;       // ���˿���
        uint16_t facebook:1;   // facebook����
        uint16_t twitter:1;    // twitter����
        uint16_t flickr:1;     // �ݲ�ʹ��
        uint16_t linkedln:1;   // 
        uint16_t whatsapp:1;   // WhatsApp���� 
        uint16_t line:1;
        uint16_t instagram:1;
        uint16_t snapchat:1;
        uint16_t skype:1;
        uint16_t gmail:1;   
        uint16_t other:1;       //
    };
}app_message_enable_t;

// ������Ϣ 
typedef struct
{
    uint8_t start_hour;    //������ʼ--Сʱ
    uint8_t start_minute;  //������ʼ--����
    uint8_t end_hour;      //����ʱ��--Сʱ
    uint8_t end_minute;    //����ʱ��--����

    uint8_t internal;      //�������ʱ��
    uint8_t enable;        //����ʹ��
}sedentary_config_t;

// �豸�������ÿ��� 
typedef struct
{
    uint8_t resv:1;                     //ҹ�䷭��  0--�ر�      1--��
    uint8_t wear_detect_enable:1;       //������  0--�ر�      1--��
    uint8_t am_pm_enable:1;             //12Сʱ��  0--24Сʱ��  1--12Сʱ��
    uint8_t english_style_enable:1;     //��/Ӣ��   0--����      1--Ӣ��
    uint8_t heart_detect_enable:1;      //���ʼ��  0--�ر�      1--��
    uint8_t blood_detect_enable:1;      //Ѫѹ���  0--�ر�      1--��
    uint8_t ecg_detect_enable:1;        //ecg�Զ�����   0--�ر�      1--�� 
    uint8_t ppg_detect_enable:1;        //ecg�Զ�����   0--�ر�      1--��
	
    uint8_t alarm_enable:1;             // WJ.20190514�޸�Ϊ�������ÿ��أ�������  0--�ر�  1--��
    uint8_t watchtimer_enable:1;        // ����� 0--�ر� 1--��
    uint8_t low_bo_alert_enable:1;      // ������������
    uint8_t find_phone_enable:1;        // �����ֻ����� 0--�ر�  1--��
    uint8_t led_gear:1;                 // led�Ƶĵȼ�
    uint8_t ble_disconnect_enable:1;    // ������������ 
    uint8_t precise_sleep:1;            // ��׼˯��
    uint8_t resv2:1;
}dev_config_t;

// �������ƽṹ��
typedef struct
{
    uint8_t length;   // ������Ϣ
    uint8_t name[8];  // ��ʱֻ֧��8���ֽ�
}bluetooth_name_t;

typedef enum
{
    BLOOD_GENERAL_MODE = 0x00,  // ͨ��ģʽ
    BLOOD_PRIVATE_MODE = 0x01,  // ˽��ģʽ
}BLOOD_MODE_E;

// Ѫѹģʽ������Ϣ 
typedef struct
{
    uint8_t private_high;  // ˽��ģʽ--��Ѫѹ
    uint8_t private_low;   // ˽��ģʽ--��Ѫѹ

    BLOOD_MODE_E mode;     // �û��趨ģʽ
}bp_config_t;

// �Ա���Ϣ 
typedef enum
{
    GENDER_MALE = 0x01,  // �Ա�--��
    GENDER_FEMALE = 0x02,  // �Ա�--Ů
}PERSON_GENDER_E;

// ������Ϣ 
typedef struct
{
    uint8_t   height;        // ���
    uint8_t   weight;        // ����
    uint8_t   age;           // ����
    PERSON_GENDER_E gender;  // �Ա�
}person_infomation_t;

// �������ò����ṹ��
typedef struct
{
    uint8_t enable;        // ����ʹ�ܿ���
    uint8_t sensitivity;   // �����ȵȼ�[1, 10]
    uint8_t s_hour;        // ������ʼʱ��--Сʱ
    uint8_t s_minute;      // ������ʼʱ��--����
    uint8_t e_hour;        // �������ʱ��--Сʱ
    uint8_t e_minute;      // �������ʱ��--����
}turnwrist_config_t;

// ���ʱ��������ṹ��
typedef struct
{
    uint8_t enable;    //ʹ�ܿ���
    uint8_t high;      //��
    uint8_t low;       //��
}heart_alert_config_t;

// ���Ȳ����ṹͼ
typedef struct
{
    uint8_t  start_hour;     //��ʼʱ��--Сʱ
    uint8_t  start_minute;   //��ʼʱ��--����
    uint8_t  end_hour;       //����ʱ��--Сʱ
    uint8_t  end_minute;     //����ʱ��--����
    uint8_t  valid_bright;   //��Ч����
    uint8_t  other_bright;   //��������
}brightness_config_t;

//Ѫ�����ز����ṹ��
typedef struct
{
    uint8_t  start_hour;     //��ʼʱ��--Сʱ
    uint8_t  start_minute;   //��ʼʱ��--����
    uint8_t  end_hour;       //����ʱ��--Сʱ
    uint8_t  end_minute;     //����ʱ��--����
    uint8_t  enable;         //��������
}time_range_config_t;


#define   APP_USED_OLD_ALARM_ENABLE     1       //ʹ�þɵ�����

//��������ṹ��
typedef struct
{
    uint8_t  state:1;   //����--״̬(1bit, ��ʡ�ڴ�ռ�)
    uint8_t  valid:1;   //����--��Ч/��Ч(1bit, ��ʡ�ڴ�ռ�)
    uint8_t  resv:6;
    
    uint16_t  scence:5;  //����--����(3bit, ���8�ֳ���)
    uint16_t  hour:5;    //����--ʱ(5bit, ���32)
    uint16_t  minute:6;  //����--��(6bit, ���64)
    
    uint16_t  repeat:7;  //����--�ظ�
    uint16_t  month:4;   //����--��(4bit, ���16)
    uint16_t  day:5;     //����--��(5bit, ���32)
    
    uint16_t  year;       //����--��(16bit)
}alarm_time_config_t;

// ����ʱ�û�������Ϣ
typedef struct
{
    uint8_t   ui_enable:1;          // UI�Ƿ���ʾ
    uint8_t   app_send:1;           // APP����
    uint8_t   btn_trigger:1;        // ������������ʱ����
    uint8_t   app_trigger:1;        // APP��������ʱ����
    uint8_t   resv:4;               // Ԥ������
    
    uint32_t  dev_remain_time;      // �ֻ�����ʱʱ��
    uint32_t  run_remain_time;      // ���е���ʱʱ��
}hourglass_config_t;

// ��־����������Ϣ
typedef struct
{
    uint16_t  offset;         // ƫ��λ��
    uint16_t  total_cnt;      // ������
}log_data_index_t;

// �ճ�����������Ϣ
typedef struct
{    
    uint32_t  step_cnt[3];       //�ܵļǲ�����
    uint32_t  distance_cnt[3];   //�ܵľ�������
    uint32_t  calories_cnt[3];   //�ܵĿ�·������
    uint16_t  offset[3];         //���������ƫ����
    uint8_t   rest_cnt[3];       //��λ��¼��ʱ
}daily_data_index_t;

// ECG����������Ϣ
typedef struct
{
    uint16_t    offset_start_index;     // ECG �������ƫ����
    uint16_t    offset_end_index;       // ECG �������ƫ����   
}ecg_data_index_t;

// ECG �쳣����������Ϣ
typedef struct
{
    uint8_t     ecg_cnt;       // ECG �Զ������쳣����ƫ����   
    uint16_t    offset_ext_index;       // ECG �Զ������쳣����ƫ����   
}ecg_ext_data_index_t;

// FLASH���������ṹ��
typedef struct
{
    uint8_t                 block_index;    // ���ݱ����������
    log_data_index_t        log_s;          // ��־FLASH����
    daily_data_index_t      daily;          // ������ճ�����λ����Ϣ 
    ecg_data_index_t        ecg;            // ecg�ֶ������������λ����Ϣ 
    ecg_ext_data_index_t    ecg_ext;        // ecg�Զ������쳣����λ����Ϣ 
}flash_find_index_t;

// ���ڹ���ͳ��
typedef struct
{
    uint16_t  bright_screen_num;        //��������
    uint16_t  bright_screen_time;       //����ʱ��
    
    uint16_t  motor_vibration_num;      //����𶯴���
    uint16_t  motor_vibration_time;     //�����ʱ��
    
    uint32_t  ble_connection_time;      //��������ʱ��
    
    uint8_t   charging;                 //�Ƿ�����
    uint8_t   rest_cnt;                 //��λ����
    uint8_t   res[2];
}power_statistics_t;

#pragma pack ()

#endif
