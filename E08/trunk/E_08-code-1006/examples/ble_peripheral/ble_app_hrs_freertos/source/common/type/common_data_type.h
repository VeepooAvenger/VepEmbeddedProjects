#ifndef _COMMON_DATA_TYPE_H_
#define _COMMON_DATA_TYPE_H_

#include <stdint.h>

#pragma pack (1)
#pragma anon_unions 

// 通知消息类型
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

// 电话推送内容结构体
typedef struct
{
    union call_type
    {
        struct call_des
        {
            uint8_t    hang_up_call:1;    // 挂断电话(手机端)
            uint8_t    reject_call:1;     // 拒接来电电话
            uint8_t    reject_suc:1;      // 拒接成功
            uint8_t    mute_call:1;       // 电话静音

            uint8_t    resv:4;            // 对齐预留
        };
        
        uint8_t id;
	};
    
    uint8_t    len;               // 内容长度
    uint8_t    content[128];      // 内容
}call_notify_content_t;

//消息内容结构体
typedef struct
{
    MSG_NOTIFY_TYPE_E  type;    // 通知消息类型
    uint8_t  len;               // 通知消息长度
    uint8_t  dis_len;           // 显示通知消息长度
    uint8_t  content[230];      // 通知内容 
}message_notify_content_t;


#pragma pack ()

//Gensor采集数据结构体
typedef struct
{
    uint8_t data_w_pos;             // 数据缓存更新的位置
    
    uint8_t adc_send_pos;           // 蓝牙发送数据的位置
    uint8_t adc_send_len;           // 蓝牙待发送数据的长度
    
    uint8_t gesture_read_pos;       // 姿态读取数据位置
    uint8_t gesture_data_len;       // 姿态待处理数据长度
    uint8_t gesture_send_pos;       // 姿态等待发送数据位置
    uint8_t gesture_wait_send_len;  // 姿态等待发送数据长度
    
    uint8_t pedometer_read_pos;     // 记步读取数据位置
    uint8_t pedometer_data_len;     // 记步待处理数据长度
    
    int16_t x_gesture_data[50];    // x轴姿态数据
    int16_t y_gesture_data[50];    // y轴姿态数据
    int16_t z_gesture_data[50];    // z轴姿态数据
    int16_t v_gesture_data[50];    // v姿态数据
    
    int16_t x_axis_data[100];    //x轴原始采集数据缓存
    int16_t y_axis_data[100];    //y轴原始采集数据缓存
    int16_t z_axis_data[100];    //z轴原始采集数据缓存
}acc_data_param_t;



#pragma pack (1)
#pragma anon_unions 

// 工厂测试参数结构体类型
typedef struct
{
    uint8_t  sign_on:1;          // 开机标志
    uint8_t  ble_connect:1;      // 蓝牙连接状态
    uint8_t  factory_mode:1;     // 工厂模式
    uint8_t  decryption_ok:1;    // 加密是否通过
    uint8_t  ota_encrypt:1;      // 加密是否通过

    uint8_t  font_test_on:1;     // 字库测试显示	
    uint8_t  code_passed:1;      // 密码通过
    uint8_t  during_findphone:1; // 是否正在查找手机
	
    uint8_t  bond_flag:3;          // 绑定标志
	uint8_t  low_power:1;        //低电标志
	uint8_t  resv:4;
}device_trace_param_t;

// 实时上传控制结构体类型
typedef struct
{
    uint8_t  wechat:1;     // 微信运动上传数据
    uint8_t  pedometer:1;  // 上传记步数据
    uint8_t  resv:6;
}app_upload_control_t;

// 系统时间格式结构体
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

// 时间格式
typedef struct
{
    uint8_t  hour;
    uint8_t  minute;
    uint8_t  second;	
}time_format_t;

#pragma pack ()

#endif

