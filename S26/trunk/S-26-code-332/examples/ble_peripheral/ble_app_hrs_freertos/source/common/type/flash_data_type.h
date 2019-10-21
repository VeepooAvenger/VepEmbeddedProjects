#ifndef _FLASH_DATA_TYPE_H_
#define _FLASH_DATA_TYPE_H_

#include <stdint.h>
#include "common_data_type.h"

/*===================================================================
 *                            ����FLASH����ַ           
 *===================================================================
 */

#define PRODUCT_INFOMATION_BASE_ADDR    0x73000     // ��Ʒ��Ϣ����ַ
#define APP_CONFIG_BASE_ADDR            0x72400     // APP���û���ַ
#define SYSTEM_PARAM_BASE_ADDR          0x72000     // ϵͳ��������ַ
#define ALARM_CONFIG_BASE_ADDR          0x72000

// ϵͳʱ��
#define SYSTEM_TIME_BASE_ADDR           0x71000//0x6D000//


// �ճ�����
#define RECORD_DAILY_BASE_ADDR          0x5F000     // ȫ�����ݻ���ַ
#define RECORD_DAILY_DATA_SIZE          0x1C00      // �ճ����ݴ�С(7KB)
#define RECORD_DAILY_TOTAL_SIZE         0x1B000     // 
#define RECORD_DAILY_ONE_DAY_SIZE       0x6000      // ȫ�����ݵĴ�С(24KB)

#define RECORD_SLEEP_OFFSET             0x1C00      //˯�����ݵ�ƫ����


// Flash��ȡ���ݵ�ַ��
#define RECORD_FLASH_BLOCK_DATA_ADDR(block_id,offset)  (uint32_t)(RECORD_MOVEMENT_BASE_ADDR+(block_id)*1024+offset)
// �ճ����ݵ�ַ��
#define RECORD_DAILY_DATA_ADDR(day_id,offset)          (uint32_t)(RECORD_DAILY_BASE_ADDR+RECORD_DAILY_ONE_DAY_SIZE*(day_id)+offset)
// ˯�����ݵ�ַ��
#define RECORD_SLEEP_DATA_ADDR(day_id,offset)          (uint32_t)(RECORD_DAILY_BASE_ADDR+RECORD_DAILY_ONE_DAY_SIZE*(day_id)+RECORD_SLEEP_OFFSET+offset)

/**********************************************
 *               Ѫ������                     *
 **********************************************/
#define RECORD_BLOOD_OXYGEN_SIZE           0x4000      // Ѫ�������ܴ�С(16KB)
#define RECORD_BLOOD_OXYGEN_OFFSET         0x2000      // Ѫ��ƫ����
// Ѫ�����ݵ�ַ��
#define RECORD_BLOOD_OXYGEN_DATA_ADDR(block_id,offset)   (uint32_t)(RECORD_DAILY_BASE_ADDR+RECORD_DAILY_ONE_DAY_SIZE*block_id+RECORD_BLOOD_OXYGEN_OFFSET+offset)

/**********************************************
 *               �˶�ģʽ                     *
 **********************************************/
#define RECORD_MOVEMENT_BASE_ADDR       0x59000     // �˶�ģʽ�Ļ���ַ
#define RECORD_MOVEMENT_TOTAL_SIZE      0x2000      // �˶�ģʽ�����ܴ�С(8KB)

#define RECORD_MOVEMENT_HEAD_OFFSET     0x50        // �˶�ģʽ���ݵ�ƫ����
// �˶�ģʽ���ݵ�ַ��
#define RECORD_MOVEMENT_HEAD_ADDR(block_id,offset)   (uint32_t)(RECORD_MOVEMENT_BASE_ADDR + (RECORD_MOVEMENT_TOTAL_SIZE*block_id) + (offset*0))
#define RECORD_MOVEMENT_DATA_ADDR(block_id,offset)   (uint32_t)(RECORD_MOVEMENT_BASE_ADDR + RECORD_MOVEMENT_HEAD_OFFSET + (RECORD_MOVEMENT_TOTAL_SIZE*block_id) + (offset*20))

// ��־
#define LOG_BASE_ADDR                   0x58000//0x6E000     // ��־����ַ
#define LOG_DATA_SIZE                   0x1000//0x4000      // ��־���ݴ�С(16KB)

// ��־���ݵ�ַ��
#define LOG_DATA_ADDR(offset)  (uint32_t)(LOG_BASE_ADDR+offset)

// ȫ������
#define RECORD_FLASH_BLOCK_DATA_SIZE                    (PRODUCT_INFOMATION_BASE_ADDR - RECORD_MOVEMENT_BASE_ADDR)  
// �û�����
#define RECORD_FLASH_BLOCK_USER_DATA_SIZE               (SYSTEM_TIME_BASE_ADDR - RECORD_MOVEMENT_BASE_ADDR)   



// ������־�Ϸ��Լ��
#define LOG_CONDITION_VALID(offset)  (offset < 256)
// ����һЩ�Ϸ��Լ��(���ڷ��������������2Сʱ,���Ի����26Сʱ����: 26x60/5 = 312)
#define RECORD_DAILY_CONDITION_VALID(day_id,offset)  ((day_id < 3)&&(offset < 312))
// ����һЩ�Ϸ��Լ��(����Ѫ�������10Сʱ,����2Сʱ,��������ƫ��: 12*60 = 712)
#define RECORD_BLOOD_OXYGEN_CONDITION_VALID(day_id,offset)  ((day_id < 3)&&(offset < 480))
// ����һЩ�Ϸ��Լ��(�����˶����ʵ��������5Сʱ,��������ƫ��: 5*60 = 300)
#define RECORD_SPORT_HEART_CONDITION_VALID(day_id,offset)  ((day_id < 3)&&(offset < 300))

/*===================================================================
 *                            ����FLASH�ṹ��           
 *===================================================================
 */
 
#pragma pack (1)
#pragma anon_unions
 
//�˶����ʽṹ��
typedef struct
{
    uint16_t  year;       // ʱ��-��
    uint8_t   month;      // ʱ��-��
    uint8_t   day;        // ʱ��-��
    uint8_t   hour;       // ʱ��-ʱ
    uint8_t   minute;     // ʱ��-��
    uint8_t   heart;      // ����ֵ
    uint16_t  sport;      // �˶���
    uint16_t  step;       // �ǲ�ֵ
    uint8_t   calorie;    // ��·��
    uint16_t  distance;   // ����
    uint8_t   resv[6];    // Ԥ������20�ֽ�
}sport_heart_attr_t;

//Ѫ���ṹ��
typedef struct
{
    uint8_t hour;               //ʱ
    uint8_t minute;             //��
    uint8_t oxygen;             //Ѫ�����
    uint8_t breath_stop_cnt;    //������ͣ����
    uint8_t breath_stop_result; //˯�ߺ�����ͣ�ۺ����Ľ��
    uint8_t low_spo2_time;      //ȱ��ʱ��
    uint8_t breath_stop_time;   //����
    uint8_t heart_load;         //���ฺ��
    uint8_t hrv;                //���ʱ�����
    uint8_t sport_amount;       //�˶���
    uint8_t heart;              //����
    uint8_t step;               //�ǲ�
    uint8_t br;                 //������
//    uint8_t wear;               // ���״̬
    uint8_t wear:2;             //���״̬ 0~3
    uint8_t spo2_modif_flag:2;  //5 ����Ѫ��������־λ  0~3
    uint8_t bit_resv:4;         //Ԥ��4λ    
    uint8_t resv[2];            //���뱣��
}blood_oxygen_attr_t;

// ɢ��ͼ���ʽṹ��
typedef struct
{
    uint8_t heart[10];          // ����ֵ
    uint8_t resv[6];            // Ԥ������
}scatter_heart_t;

//�ճ�����
typedef struct
{
    uint8_t    month;
    uint8_t    day;
    uint8_t    hour;
    uint8_t    minute;
    uint16_t   step;
    uint16_t   sport;
    uint8_t    heart;
    uint8_t    blood_high;
    uint8_t    blood_low;
    uint8_t    gesture;
    uint8_t    sleep_err;
    uint8_t    gesture_cnt; //˯����̬����;
    uint16_t   distance;
    uint16_t   calories;
    uint8_t    ox_value;
    uint8_t    resv;
}daily_data_attr_t;

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
//typedef enum
//{
//    GUI_LANGUAGE_CHINESE = 0x00,  //��������
//    GUI_LANGUAGE_ENGLISH = 0x01,  //Ӣ������
//}GUI_LANGUAGE_E;
typedef enum
{
    CONFIG_LANGUAGE_CHINESE = 1,       //����
    CONFIG_LANGUAGE_ENAGLISH,          //Ӣ��
    CONFIG_LANGUAGE_JAPANESE,          //����
    CONFIG_LANGUAGE_KOREAN,            //����
    CONFIG_LANGUAGE_GERMAN,            //����
    CONFIG_LANGUAGE_RUSSIAN,           //����
    CONFIG_LANGUAGE_SPANISH,           //��������
    CONFIG_LANGUAGE_ITALIARN,          //�������
    CONFIG_LANGUAGE_FRENCH,            //����
    CONFIG_LANGUAGE_VIETNAMESE,        //Խ����
    CONFIG_LANGUAGE_PORTUGUESE,        //��������
    CONFIG_LANGUAGE_TRADITIONAL,       //����
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
    uint8_t resv:1;                    //ҹ�䷭��  0--�ر�      1--��
    uint8_t wear_detect_enable:1;      //������  0--�ر�      1--��
    uint8_t am_pm_enable:1;            //12Сʱ��  0--24Сʱ��  1--12Сʱ��
    uint8_t english_style_enable:1;    //��/Ӣ��   0--����      1--Ӣ��
    uint8_t heart_detect_enable:1;     //���ʼ��  0--�ر�      1--��
    uint8_t blood_detect_enable:1;     //Ѫѹ���  0--�ر�      1--��
    uint8_t resv1:2;                   //���ʱ���  0--�ر�      1--��
	
    uint8_t horizontal_enable:1;       // ������  0--�ر�  1--��
    uint8_t watchtimer_enable:1;       // ����� 0--�ر� 1--��
    uint8_t low_bo_alert_enable:1;     // ������������
    uint8_t find_phone_enable:1;       // �����ֻ����� 0--�ر�  1--��
    uint8_t led_gear:1;                //led�Ƶĵȼ�
    uint8_t ble_disconnect_enable:1;   //������������ 
    uint8_t resv2:2;
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
    GENDER_MALE = 0x01,    // �Ա�--��
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
    uint8_t sensitivity;   // �����ȵȼ�[1,10]
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

//��������ṹ��
typedef struct
{
    uint8_t  state:1;   //����--״̬(1bit,��ʡ�ڴ�ռ�)
    uint8_t  valid:1;   //����--��Ч/��Ч(1bit,��ʡ�ڴ�ռ�)
    uint8_t  resv:6;
    
    uint16_t  scence:5;  //����--����(3bit,���8�ֳ���)
    uint16_t  hour:5;    //����--ʱ(5bit,���32)
    uint16_t  minute:6;  //����--��(6bit,���64)
    
    uint16_t  repeat:7;  //����--�ظ�
    uint16_t  month:4;   //����--��(4bit,���16)
    uint16_t  day:5;     //����--��(5bit,���32)
    
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
    uint16_t  ox_value_cnt[3];   //�����Ѫ����Ч���ݸ���
}daily_data_index_t;

// Ѫ������������Ϣ
typedef struct
{
    uint16_t  offset[3];             // Ѫ������ƫ����
}oxygen_data_index_t;

// �˶�����FLASH�����ṹ������
typedef struct
{
    uint8_t   current_block_id;     // ��ǰ����ID
    uint16_t  offset[3];            // ����ƫ����
}sport_heart_record_t;

// FLASH���������ṹ��
typedef struct
{
    uint8_t               block_index;   //  ���ݱ����������
    log_data_index_t      log_s;         //  ��־FLASH����
    oxygen_data_index_t   oxygen;        //  Ѫ�����ݼ�¼
    daily_data_index_t    daily;         //  ˯��λ����Ϣ 
    sport_heart_record_t  sport_heart;   //  �˶�����
}flash_find_index_t;

// δ����Ϣ��������Ϣ
typedef struct
{
    uint8_t  queue[8];
    uint8_t  write_pos;
    message_notify_content_t  unread_message;
}unread_message_index_t;

#pragma pack ()

#endif
