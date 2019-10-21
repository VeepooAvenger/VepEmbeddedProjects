#ifndef _AFE4404_DATA_TYPE_H_
#define _AFE4404_DATA_TYPE_H_

#include <stdint.h>
#include "Respiratory_Rate.h"
#include "Blood_OxygenDetection.h"


// ����Ѫ��������ֵ
#define OX_SPECIAL_VALUES_RANDOM 254    // Ѫ�����ֵ-app��ȡ��������
#define OX_SPECIAL_VALUES_WEAR   253    // Ѫ��δ���-app��ȡ�����
#define OX_SPECIAL_VALUES_INT    252    // Ѫ�����������-app��ȡ�����


#pragma pack (1)
#pragma anon_unions

// �ϴ��ɼ�����ö������
typedef enum
{
    UPLOAD_ADC_NULL_EVENT = 0,         // ���¼�
    UPLOAD_ADC_GSENSOR_EVENT = 1,      // �ϴ�GsensorԭʼADC�����¼�
    UPLOAD_ADC_GREEN_EVENT = 2,        // �ϴ��̹�ԭʼADC�����¼�
    UPLOAD_ADC_OXYGEN_EVENT = 3,       // �ϴ�Ѫ��ԭʼADC�����¼�
    UPLOAD_ADC_FATIGUE_EVENT = 4,      // �ϴ�ƣ�Ͷ�ԭʼADC�����¼�
    UPLOAD_ADC_GESTURE_EVENT = 5,      // �ϴ���̬����
}UPLOAD_ADC_EVENT;

// ʱ���¼
typedef struct
{
    uint32_t  start_time;       // ��ʼʱ��
    uint32_t  end_time;         // ����ʱ��
    uint32_t  start_stack;      // ��ʼջ��С
    uint32_t  end_stack;        // ����ջ��С	
}trace_message_t;

// ƣ�ͶȲ�������
typedef struct
{
    uint8_t   init:1;           // ƣ�Ͷȳ�ʼ��
    uint8_t   app:1;            // App��ƣ�ͶȲ���
    uint8_t   resv:6;           // ���뱣��λ
}fatigue_measure_t;

// �����ʲ����ṹ��
typedef struct
{
    uint8_t  init:1;           // �����ʲ�����ʼ��
    uint8_t  app:1;            // APP����������
    uint8_t  time:1;           // ��ʱ����������
    uint8_t  min_init:1;       // ÿ���ӳ�ʼ��һ�����ݻ���
    uint8_t  resv:4;           // bit���뱣��
    
    uint8_t  mode;              // ģʽ 0������ģʽ  1������ģʽ 2:��������ģʽ���ֶ�ֹͣ
    uint8_t  percent;           // APP�ٷֱ�

    uint8_t  err_code;          // �����ʴ�����
    uint8_t  result;            // �����ʲ��Խ��    
    uint8_t  result_sec[300];   // �����ʲ��Խ�� 
    resp_second_t resp_second_s;// �����ʵ�˲ʱֵ
    uint8_t wave_station;       // ��ǰ���ε�����
    uint8_t all_flag;           // ��������������ȱʧ��־ 
	uint16_t  cnt;               // �����ʲ��Դ���
}respiratory_rate_measure_t;

// ������ͣ�����ṹ��
typedef struct
{
    uint8_t  init:1;           // �㷨��ʼ��
    uint8_t  resv:7;           // ���뱣��
    
    uint8_t  err_code;         // ������
}breath_stop_measure_t;

// Ѫ�����������ṹ��
typedef struct
{
    uint8_t   init:1;           // ��ʼ��
    uint8_t   btn:1;            // ����Ѫ������
    uint8_t   app:1;            // APPѪ������
    uint8_t   time:1;           // ��ʱѪ������
	uint8_t   cal_flag:1;       // Ѫ��У׼������־
	uint8_t   cal_success:1;    // Ѫ��У׼�ɹ���־
    uint8_t   ox_day_time:1;    // ����Ѫ���Զ�����
    uint8_t   resv:1;           // ���뱣��
    
    uint8_t   ox_calc_cnt;      // У׼��ɺ󣬼�����������ڰ����Զ����� 
    
    uint8_t  current_green;		// ����
    uint8_t  current_red;		// ����
    uint8_t  current_ird;		// ����
    
	uint8_t  gain_gre;              // ����
	uint8_t  gain_red;              // ����
	uint8_t  gain_ird;              // ����
    uint8_t  offdac_grn;        //OFFDAC����grn
    uint8_t  offdac_red;        //OFFDAC����red
    uint8_t  offdac_ird;        //OFFDAC����ird
	
	uint16_t  r_prefusion;
	uint16_t  ir_prefusion;
	uint8_t   bod_legal_flag;
	
	uint32_t   r_ctr;//����ת����
	uint32_t   ir_ctr;//�����ת����
    
    uint8_t test_mode;          //����ģʽ��4����ҽ��ģʽ
    uint8_t algorithm_flg;      //�㷨�����ʹ�õı�־��0��ʹ���㷨��� 1����ʹ���㷨���
    uint8_t add_sub_flg;        //��Ѫ��ֵ���������ı�־��0�������� 1����offse_buf 2����offse_buf
    uint8_t offse_buf;          //ƫ��ֵ
    uint8_t oxy_buf;            //ֱ��ʹ��Ѫ��ֵ��algorithm_flgΪ1ʱ��Ч
    
    bod_app_change_value_t medical_param_s;  //��ҽ��ʱʹ�õĲ���
    
    uint8_t  err_code;          // ������
    uint8_t  result;            // Ѫ��ʵʱ�������
    uint16_t cnt;               // Ѫ�����Դ���
    uint16_t min_start_index;   // Ѫ�����ݷ��ӵ���ʼֵ
    uint8_t  time_result[300];  // Ѫ����ʱ�������Խ��
    
    uint8_t cal_state;          // Ѫ��У׼��״̬
    uint8_t cal_progress;       // Ѫ��У׼�Ľ���
    uint32_t bo_ac_r_ir;
    //uint8_t test_mode;          // Ѫ�����Ե�ģʽ
    
    uint8_t calc_cnt_test;      //ͳ���㷨���ô����������쳣����

    uint8_t ox_prompt_sign_flag;  //�źŹ�ע�Ȳ��У���Ҫ�ص�Ϩ��
    uint8_t ox_no_real_spo2_value_flag; // ��δ����ʵѪ��ֵ����ʾ--

}blood_oxygen_measure_t;

typedef struct
{
    uint8_t  enable:1;          // ����Ѫ����֤����
    uint8_t  resv:7;            // ���뱣��
}blood_oxygen_verify_t;

typedef struct
{
    uint8_t   init:1;           // ���ʳ�ʼ��
    uint8_t   app:1;            // APP��������
    uint8_t   btn:1;            // ������������
    uint8_t   day_time:1;       // ���ʶ�ʱ����(����--���̵Ʋ���)
    uint8_t   night_time:1;     // ���ʶ�ʱ����(ҹ��--������ȫ��)
    uint8_t   min_init:1;       // ÿ���ӳ�ʼ��һ�����ݻ���
    uint8_t   calib_init:1;      // У׼��ʼ��
    uint8_t   calib_well:1;     // У׼���
    
    uint8_t  real_heart;        // ʵʱ����
    uint8_t  time_heart;        // ��ʱ�������ʽ��	
    uint8_t  time_measure_cnt;  // ��ʱ��������
    uint8_t  valid_heart_cnt;   // ��Ч���ʲ��Դ���
    uint8_t  heart_cache[10];   // ���ʲ��Խ��(��Ѫ���л�ȡ)
}heart_measure_t;

typedef struct
{
    uint8_t     bp_init:1;          // Ѫѹ��ʼ��
    uint8_t     bp_app:1;           // APP����Ѫѹ
    uint8_t     bp_btn:1;           // ��������Ѫѹ
    uint8_t     bp_day_time:1;      // Ѫѹ��ʱ����(����--���̵Ʋ���)
    uint8_t     bp_night_time:1;    // Ѫѹ��ʱ����(ҹ��--������ȫ��)
    uint8_t     bp_min_init:1;      // ÿ���ӳ�ʼ��һ�����ݻ���
    uint8_t     bp_calib_init:1;    // У׼��ʼ��
    uint8_t     bp_calib_well:1;    // У׼���

    uint8_t     bp_test_mode;       // ����ģʽ
    uint8_t     bp_set_sp;          // ���ø�ѹ
    uint8_t     bp_set_dp;          // ���õ�ѹ

    uint8_t     bp_calc_prg;        // �������:20�κ�����
    uint8_t     bp_flag;            // Ѫѹ������״̬
    uint8_t     bp_sp_value;        // ��ѹ
    uint8_t     bp_dp_value;        // ��ѹ
    
}bp_measure_t;

//HRV���������ṹ��
typedef struct
{
    uint8_t  init:1;       // HRV������ʼ��
    uint8_t  time:1;       // ��ʱHRV����
    uint8_t  min_init:1;   // ÿ���ӳ�ʼ��һ�����ݻ���
    uint8_t  resv:5;       // ��������
    
    uint8_t  err_code;     // HRV������
    uint8_t  cnt;          // HRV���Դ���
    uint8_t  result[60];   // HRV���Խ��
}hrv_measure_t;

// ���ฺ�ɲ����ṹ��
typedef struct
{
    uint8_t  err_code;    // ���ฺ�ɴ�����
    uint8_t  cnt;         // ���ฺ����Ч�������
    uint8_t  result[60];  // ���ฺ�ɽ��
}heart_load_param_t;

// ɢ��ͼ���ʲ����ṹ��
typedef struct
{
    uint8_t  heart[10];   // ����ֵ
    uint8_t  err_code;    // ������
    uint8_t  cnt;         // ��Ч�������
}scatter_heart_param_t;

// ��������ṹ��
typedef struct
{
    uint8_t  init:1;             // �����ʼ��
    uint8_t  resv:7;
    
    uint8_t  wear_not_pass_cnt;  // �����ͨ������
    uint8_t  wear_status;        // ���״̬
}wear_measure_t;

// ���ּ������ṹ��
typedef struct
{
    uint8_t  init:1;             // ���ּ���ʼ��
    uint8_t  resv:7;

    uint8_t   uncall_wear_cnt;   // δ��������㷨����
    uint8_t   last_wear_status;  // �ϴ����״̬
    uint8_t   no_wear_result;    // ��ʱ�䲻������
    uint8_t   gesture;           // 5���ӵ���̬
    uint32_t  amount;            // �˶���(�����)
    uint32_t  step;              // �ǲ�ֵ(�����)
}no_wear_measure_t;

//�ǲ���ʷ����
typedef struct
{
    uint8_t   index;                // ���ݸ�������
    
    uint8_t   last_minute_step;     // ��һ���Ӽǲ�ֵ
    uint8_t   last_minute_sport;    // ��һ�����˶���
    uint32_t  power_on_step;        // �ֻ������ǲ�ֵ
    uint32_t  step_history_count;   // �ǲ���ʷ����
    uint32_t  distance_history_count;   // ������ʷ����
    uint32_t  calories_history_count;   // ��·����ʷ����
    uint32_t  sport_history_count;  // �˶�����ʷ����
    uint16_t  step_count[5];        // �ǲ���(ÿ���Ӽ���һ��)
    uint16_t  sport_count[5];       // �˶���(ÿ���Ӽ���һ��)
    uint16_t  distance_count[5];        // ����(ÿ���Ӽ���һ��)
    uint16_t  calories_count[5];       // ��·��(ÿ���Ӽ���һ��)
}history_pedometer_param;
#pragma pack ()

#define PPG_BP_CALC_TIME        25  //Ѫѹ�������

//========================================
//   ******AFE4404 ��������С����******
//========================================
#define RED_POOL_SIZE          300
//YX.20180628.�޸��̹�ͺ�������ݴ�С
#define IRED_POOL_SIZE         300//750//
#define GREEN_POOL_SIZE        750//300//
//YX.20180628.��������̹����ݵ�������
#define WEAR_GREEN_POOL_SIZE   100
#define AMBIENT_POOL_SIZE      100
#define DATA_CACHE_POOL_SIZE   50

// AFE4404���ݻ���ؽṹ��
typedef struct
{
    uint32_t  pool_update_second;        // POOL�ظ��µ���������
    uint8_t   cache_update_cnt;          // cache��������
    
    uint16_t  send_read_pos;             // ���ʷ��͵Ķ�λ��
    uint16_t  send_wait_len;             // ���ʵȴ����ͳ���
    uint16_t  send_package_index;        // ���ʷ��Ͱ���

    // �ڴ渴�����ݻ���
    union multiplex_data
    {
        struct bo
        {
              int32_t  red[RED_POOL_SIZE + DATA_CACHE_POOL_SIZE];        // Ѫ��-������ݻ�����
              int32_t  ired[IRED_POOL_SIZE + DATA_CACHE_POOL_SIZE];      // Ѫ��-�������ݻ�����    
              int32_t  green[GREEN_POOL_SIZE + DATA_CACHE_POOL_SIZE];    // �̹����ݻ�����
        };
        //int32_t fatigue_data[200+50];                                   // ƣ�Ͷ����ݻ���	 
    };

    int32_t  wear_green[WEAR_GREEN_POOL_SIZE];                           // ������̹����ݻ�����
    int32_t  ambient[AMBIENT_POOL_SIZE + DATA_CACHE_POOL_SIZE];          // ���������ݻ����� 
    
//    // �ڴ渴�����ݻ���
//    union multiplex_data
//    {
//        struct bo
//        {
//              uint32_t  red[RED_POOL_SIZE + DATA_CACHE_POOL_SIZE];        // Ѫ��-������ݻ�����
//              uint32_t  ired[IRED_POOL_SIZE + DATA_CACHE_POOL_SIZE];      // Ѫ��-�������ݻ�����    
//              uint32_t  green[GREEN_POOL_SIZE + DATA_CACHE_POOL_SIZE];    // �̹����ݻ�����
//        };
//        uint32_t fatigue_data[800+200];                                   // ƣ�Ͷ����ݻ���	 
//    };

//    uint32_t  wear_green[WEAR_GREEN_POOL_SIZE];                           // ������̹����ݻ�����
//    uint32_t  ambient[AMBIENT_POOL_SIZE + DATA_CACHE_POOL_SIZE];          // ���������ݻ����� 
}afe4404_data_pool;



#endif
