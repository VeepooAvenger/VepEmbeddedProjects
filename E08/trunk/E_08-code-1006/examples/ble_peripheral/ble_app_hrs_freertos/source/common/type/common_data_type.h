#ifndef _COMMON_DATA_TYPE_H_
#define _COMMON_DATA_TYPE_H_

#include <stdint.h>

#pragma pack (1)
#pragma anon_unions 

// ֪ͨ��Ϣ����
typedef enum
{
    MSG_NOTIFY_NULL_ID = 0,
    MSG_NOTIFY_SMS_ID,
    MSG_NOTIFY_WECHAT_ID,
    MSG_NOTIFY_QQ_ID,
//    MSG_NOTIFY_SINA_ID,
    MSG_NOTIFY_FACEBOOK_ID,
    MSG_NOTIFY_TWITTER_ID,
//    MSG_NOTIFY_FLICKR_ID,
    MSG_NOTIFY_WHATSAPP_ID,
    MSG_NOTIFY_LINKEDLN_ID,
    MSG_NOTIFY_SNAPCHAT_ID,
    MSG_NOTIFY_SKYPE_ID,
    MSG_NOTIFY_INSTAGRAM_ID,
    MSG_NOTIFY_LINE_ID,
    MSG_NOTIFY_GMAIL_ID,
    MSG_NOTIFY_OTHER_ID,
    MSG_NOTIFY_MAX_ID,
}MSG_NOTIFY_TYPE_E;

// �绰�������ݽṹ��
typedef struct
{
    union call_type
    {
        struct call_des
        {
            uint8_t    hang_up_call:1;    // �Ҷϵ绰(�ֻ���)
            uint8_t    reject_call:1;     // �ܽ�����绰
            uint8_t    reject_suc:1;      // �ܽӳɹ�
            uint8_t    mute_call:1;       // �绰����

            uint8_t    resv:4;            // ����Ԥ��
        };
        
        uint8_t id;
	};
    
    uint8_t    len;               // ���ݳ���
    uint8_t    content[128];      // ����
}call_notify_content_t;

//��Ϣ���ݽṹ��
typedef struct
{
    MSG_NOTIFY_TYPE_E  type;    // ֪ͨ��Ϣ����
    uint8_t  len;               // ֪ͨ��Ϣ����
    uint8_t  dis_len;           // ��ʾ֪ͨ��Ϣ����
    uint8_t  content[230];      // ֪ͨ���� 
}message_notify_content_t;


#pragma pack ()

//Gensor�ɼ����ݽṹ��
typedef struct
{
    uint8_t data_w_pos;             // ���ݻ�����µ�λ��
    
    uint8_t adc_send_pos;           // �����������ݵ�λ��
    uint8_t adc_send_len;           // �������������ݵĳ���
    
    uint8_t gesture_read_pos;       // ��̬��ȡ����λ��
    uint8_t gesture_data_len;       // ��̬���������ݳ���
    uint8_t gesture_send_pos;       // ��̬�ȴ���������λ��
    uint8_t gesture_wait_send_len;  // ��̬�ȴ��������ݳ���
    
    uint8_t pedometer_read_pos;     // �ǲ���ȡ����λ��
    uint8_t pedometer_data_len;     // �ǲ����������ݳ���
    
    int16_t x_gesture_data[50];    // x����̬����
    int16_t y_gesture_data[50];    // y����̬����
    int16_t z_gesture_data[50];    // z����̬����
    int16_t v_gesture_data[50];    // v��̬����
    
    int16_t x_axis_data[100];    //x��ԭʼ�ɼ����ݻ���
    int16_t y_axis_data[100];    //y��ԭʼ�ɼ����ݻ���
    int16_t z_axis_data[100];    //z��ԭʼ�ɼ����ݻ���
}acc_data_param_t;



#pragma pack (1)
#pragma anon_unions 

// �������Բ����ṹ������
typedef struct
{
    uint8_t  sign_on:1;          // ������־
    uint8_t  ble_connect:1;      // ��������״̬
    uint8_t  factory_mode:1;     // ����ģʽ
    uint8_t  decryption_ok:1;    // �����Ƿ�ͨ��
    uint8_t  ota_encrypt:1;      // �����Ƿ�ͨ��

    uint8_t  font_test_on:1;     // �ֿ������ʾ	
    uint8_t  code_passed:1;      // ����ͨ��
    uint8_t  during_findphone:1; // �Ƿ����ڲ����ֻ�
	
    uint8_t  bond_flag:3;          // �󶨱�־
	uint8_t  low_power:1;        //�͵��־
	uint8_t  resv:4;
}device_trace_param_t;

// ʵʱ�ϴ����ƽṹ������
typedef struct
{
    uint8_t  wechat:1;     // ΢���˶��ϴ�����
    uint8_t  pedometer:1;  // �ϴ��ǲ�����
    uint8_t  resv:6;
}app_upload_control_t;

// ϵͳʱ���ʽ�ṹ��
typedef struct
{
    uint16_t  year;     // 2000+
    uint8_t   month;    // 1-12
    uint8_t   day;      // 0-31
    uint8_t   hour;     // 0-23
    uint8_t   minutes;  // 0-59
    uint8_t   seconds;  // 0-59
    uint8_t   week;
}system_clock_time_t;

// ʱ���ʽ
typedef struct
{
    uint8_t  hour;
    uint8_t  minute;
    uint8_t  second;	
}time_format_t;

#pragma pack ()

#endif

