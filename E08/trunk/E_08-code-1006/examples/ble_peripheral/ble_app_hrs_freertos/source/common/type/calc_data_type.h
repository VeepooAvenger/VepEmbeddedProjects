/***************************Copyright(c)***********************************
**                       Veepoo Tech
**                  COPYRIGHT (C) 2016-2019
**-----------File Info-----------------------------------------------------
** File        name:     calc_data_type.h
** Last modified Date:   2018-10-18
** Last SYS_VERSION:     0.0.0
** Descriptions:            
**-------------------------------------------------------------------------
** Created  by:          WJ
** Created  date:        2018-10-19
** SYS_VERSION:          0.0.0
** Descriptions:         
**-------------------------------------------------------------------------
** Modified by:				
** Modified date:  			          
** SYS_VERSION:                 
** Descriptions:                        
***************************************************************************/

#ifndef _CALC_DATA_TYPE_H_
#define _CALC_DATA_TYPE_H_

#include <stdint.h>

#pragma pack (1)
#pragma anon_unions

#define ECG_ADC_FREQUENCY_APP_125HZ       0//1     //����app�Ĳ���Ƶ��


#define PPG_BP_CALC_TIME            25      //����Ѫѹʱ��Ϊ25s

// �ϴ��ɼ�����ö������
typedef enum
{
    UPLOAD_ADC_NULL_EVENT = 0,          // ���¼�
    UPLOAD_ADC_GSENSOR_EVENT = 1,       // �ϴ�GsensorԭʼADC�����¼�
    UPLOAD_ADC_GREEN_EVENT = 2,         // �ϴ�PPGԭʼADC�����¼�
    UPLOAD_ADC_ECG_EVENT = 3,           // �ϴ�ECGԭʼADC�����¼�
    UPLOAD_ADC_OXYGEN_EVENT = 4,         // �ϴ�Ѫ��ԭʼADC�����¼�
    UPLOAD_ADC_GESTURE_EVENT = 5,       // �ϴ���̬����
    UPLOAD_ADC_ECG_ORIGINAL_EVENT = 6,  // �ϴ�ECGԭʼADC�����¼�(δ�˲�)
}UPLOAD_ADC_EVENT;

// 4900 ����ģʽ
typedef enum
{
    MEASURE_MODE_NULL = 0,          // ����δ����
    MEASURE_MODE_PPG,               // ��ppgģʽ
    MEASURE_MODE_ECG,               // ��ecgģʽ
    MEASURE_MODE_OXYGEN,             // ��Ѫ��ģʽ
    //MEASURE_MODE_PPG_OXYGEN,         // PPG+Ѫ��ģʽ   
    
}MEASURE_MOD_E;

// ʱ���¼
typedef struct
{
    uint32_t  start_time;       // ��ʼʱ��
    uint32_t  end_time;         // ����ʱ��
    uint32_t  start_stack;      // ��ʼջ��С
    uint32_t  end_stack;        // ����ջ��С	
}trace_message_t;


typedef struct
{
	uint8_t lead_off_type;
	uint8_t diag_param_buf[8];
	uint8_t heart_rate;
	uint8_t resp_rate;
	uint8_t hrv;
	uint16_t QT_time;
	uint32_t rese[3];
}ecg_test_result_t;


// ppg��������
typedef struct
{
    uint8_t     init:1;             // ppg��ʼ��
    uint8_t     app:1;              // APP��������
    uint8_t     btn:1;              // ������������
    uint8_t     time:1;             // ppg�Զ�����
    uint8_t     app_bp:1;           // APPѪѹ����
    uint8_t     btn_bp:1;           // ��������Ѫѹ
    uint8_t     res:2;              // ����
      
    uint8_t     calc_bp_init:1;     // Ѫѹ�㷨��ʼ��
    uint8_t     calc_wear_init:1;   // ����㷨��ʼ��
    uint8_t     calc_res:6;         // ����    
    
    uint8_t     bp_progress;        // Ѫѹ�������ȣ�201��ʾ��Ҫ�ر�app����
    uint8_t     hreart_state;       // ����app״̬��1��ʾ��Ҫ�ر�app����
    
    uint8_t     p_second_heart;     // ʵʱ����    
    uint8_t     p_minute_heart;     // һ�������ʾ�ֵ
    uint8_t     p_second_hrv;       // ÿ��hrv
    uint8_t     p_second_rr;        // ÿ��rr
    uint8_t     p_6sec_rr;          // ÿ6��rr 
    uint8_t     p_wear_flag;        // ppg���״̬ 
    
    
    uint8_t     p_bp_flag;          // 0�����У�1���Խ����Ч��2���Խ����Ч    
    uint8_t     p_bp_high_sec;
    uint8_t     p_bp_low_sec;
    uint8_t     p_bp_high_25sec;
    uint8_t     p_bp_low_25sec;
    uint8_t     p_bp_high_5min;
    uint8_t     p_bp_low_5min;
    
    uint32_t    p_calc_cnt;   // �������
}ppg_measure_t;

// ecg��������
typedef struct
{
    uint8_t     init:1;             // ecg��ʼ��
    uint8_t     app:1;              // APP��������
    uint8_t     btn:1;              // ������������
    uint8_t     time:1;             // ecg�Զ�����
    uint8_t     res:4;              // ����
    
    uint8_t     calc_init:1;        // �ƴγ�ʼ��
    uint8_t     calc_res:6;         // ����    
    
    uint8_t     e_second_heart;     // ʵʱ����    
    uint8_t     e_minute_heart;     // һ�������ʾ�ֵ
    uint8_t     e_second_hrv;       // ÿ��hrv
    uint8_t     e_second_rr;        // ÿ��rr
    uint8_t     e_6sec_rr;          // ÿ6��hrv
    uint8_t     e_ary;              // ���ʲ���
    uint8_t     e_second_br;        // ÿ�������
    uint8_t     e_minute_br;        // ÿ���Ӻ�����
    uint8_t     e_wear_flag;        // ecg���״̬
    uint8_t     e_wear_time;        // ecg�������ʱ��1s
    uint16_t    e_qtc_time;         // QTC Time
    
    uint8_t     e_m_id;             // ���¿�ʼ��ǣ�ÿ�μ�1
    uint8_t     result_progress;    // ��Ͻ���
    uint32_t    e_calc_cnt;         // ��������
    
    uint32_t    e_app_calc_cnt;     // ��������
    
    ecg_test_result_t    ecg_test_result_s;   // �������
    
    uint8_t     e_test_result_prg;  // ��������
    uint8_t     e_test_result_prg1;  // ��������
    
}ecg_measure_t;


// ecg��������
typedef struct
{
    uint8_t     ox_init:1;              // ��ʼ��
    uint8_t     ox_app:1;               // APP����
    uint8_t     ox_btn:1;               // ��������
    uint8_t     ox_time:1;              // �Զ�����
    uint8_t     ox_rest:1;              // ��������    
    uint8_t     ox_calib:1;             // У׼���� 
    uint8_t     ox_res:2;               // ����

    uint8_t     ox_result;              // Ѫ��ʵʱ�������
    uint8_t     ox_time_result[300];    // Ѫ����ʱ�������Խ�� 
    uint16_t    ox_cnt;                 // Ѫ�����Դ���
    
    uint16_t    ox_sum_result;          // һ����Ѫ���ۻ���
    uint8_t     ox_sum_cnt;             // һ����Ѫ���ۻ�����
		
	// FWT.20190304.	
    uint8_t bod_legal_flag;
    uint16_t ir_prefusion;//����Ĺ�ע��
    uint16_t r_prefusion;//���Ĺ�ע��
}oxygen_measure_t;


// �����ʲ����ṹ��
typedef struct
{
    uint8_t     resp_init:1;            // �����ʲ�����ʼ��
    uint8_t     resp_resv:7;            // bit���뱣��
       
    uint8_t     resp_result_sec[300];   // �����ʲ��Խ�� 
	uint16_t    resp_cnt;               // �����ʲ��Դ���
}respiratory_rate_measure_t;


// ppg\ecg ��������
typedef struct
{
    uint8_t                     ecg_ppg_mode;       // ecg\ppg\pptģʽ
    uint8_t                     afe4900_is_open;    // 4900����״̬
    uint8_t                     read_data;          // ��ȡ4900����-�л�״̬�����в���ȡ��0������ȡ��1��ȡ
    uint8_t                     stop_int_cnt;       // 1182 �������жϲ���Ӧ�ƴ�(s)�������쳣����
    uint8_t                     stop_read_cnt;      // 1182 �������жϲ���ȡ����ʱ��(s)�������쳣����
    ppg_measure_t               ppg_measure_s;      // ppg ����
    ecg_measure_t               ecg_measure_s;      // ecg ����
    oxygen_measure_t            oxygen_measure_s;   // oxygen ���� 
    respiratory_rate_measure_t  respiratory_rate_measure_s;  //�����ʣ�ֻ�洢�������������������
}ecg_ppg_measure_t;

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
    uint8_t   index;                    // ���ݸ�������
    
    uint8_t   last_minute_step;         // ��һ���Ӽǲ�ֵ
    uint8_t   last_minute_sport;        // ��һ�����˶���
    uint32_t  power_on_step;            // �ֻ������ǲ�ֵ
    uint32_t  step_history_count;       // �ǲ���ʷ����
    uint32_t  distance_history_count;   // ������ʷ����
    uint32_t  calories_history_count;   // ��·����ʷ����
    uint32_t  sport_history_count;      // �˶�����ʷ����
    uint16_t  step_count[5];            // �ǲ���(ÿ���Ӽ���һ��)
    uint16_t  sport_count[5];           // �˶���(ÿ���Ӽ���һ��)
    uint16_t  distance_count[5];        // ����(ÿ���Ӽ���һ��)
    uint16_t  calories_count[5];        // ��·��(ÿ���Ӽ���һ��)
    
    uint16_t  step_count_10m[10];       // ��·��(ÿ���Ӽ���һ��)
}history_pedometer_param;


//�ǲ���ʷ����
typedef struct
{
    uint8_t   hour;         //ʱ
    uint8_t   minute;       //��
    uint8_t   second;       //��   
    
    uint8_t   ppg_updata_cnt;
    uint8_t   ecg_updata_cnt;
    uint8_t   ox_updata_cnt;
}ecg_ppg_time_t;




#pragma pack ()

////////////////////////////////////
//   SI1182 ��������С����       //
///////////////////////////////////

// �̹⻷���������ȡ��ֵ
#define GREEN_AMB_CACHE_REMAINDER					(3 * 2)
// �̹���⻷���������ȡ��ֵ
#define GREEN_IRED_AMB_CACHE_REMAINDER				(3 * 3)
// �̹�����⻷���������ȡ��ֵ
#define GREEN_RED_IRED_AMB_CACHE_REMAINDER			(3 * 4)
// �����⻷���������ȡ��ֵ
#define RED_IRED_AMB_CACHE_REMAINDER				(3 * 3)
// ECGȡ��ֵ
#define ECG_CACHE_REMAINDER							(3)
// LDȡ��ֵ
#define LD_CACHE_REMAINDER							(4)

// �̹�
#define GREEN_POOL_SIZE				(50 * 20)
#define GREEN_CACHE_SIZE			(50)
// ���
#define RED_POOL_SIZE				(50 * 6)
#define RED_CACHE_SIZE				(50)
// ����
#define IRED_POOL_SIZE				(50 * 6)
#define IRED_CACHE_SIZE				(50)
// ������
#define AMB_POOL_SIZE				(50 * 2)
#define AMB_CACHE_SIZE				(50)


// �̹⻷�������ݸ��»���
#define GREEN_AMB_CACHE_SIZE			(GREEN_CACHE_SIZE + AMB_CACHE_SIZE)
// �̹���⻷�������ݸ��»���
#define GREEN_IRED_AMB_CACHE_SIZE		(GREEN_CACHE_SIZE + IRED_CACHE_SIZE + AMB_CACHE_SIZE)
// �̹�����⻷�������ݸ��»���
#define GREEN_RED_IRED_AMB_CACHE_SIZE	(GREEN_CACHE_SIZE + RED_CACHE_SIZE + IRED_CACHE_SIZE + AMB_CACHE_SIZE)
// �����⻷�������ݸ��»���
#define RED_IRED_AMB_CACHE_SIZE			(RED_CACHE_SIZE + IRED_CACHE_SIZE + AMB_CACHE_SIZE)

// PPG���ݻ���
#define	PPG_POOL_SIZE			(GREEN_POOL_SIZE + RED_POOL_SIZE + IRED_POOL_SIZE + AMB_POOL_SIZE)
// PPG�ж����ݸ��»���
#define PPG_CACHE_SIZE			(50)

// ECG���ݻ���
#define ECG_POOL_SIZE			(250*6)
// ECG�ж����ݸ��»���
#define ECG_CACHE_SIZE			(250)

// �������ݻ���
#define LD_POOL_SIZE			(5)
#define LD_CACHE_SIZE			(1)

 
// AFE4900 ���ݻ���ؽṹ��
#pragma pack(push)
#pragma pack(4)


typedef struct
{
	int32_t		green[GREEN_POOL_SIZE + GREEN_CACHE_SIZE];		// �̹����ݻ���
	int32_t		red[RED_POOL_SIZE + RED_CACHE_SIZE];			// ������ݻ���
	int32_t		ired[IRED_POOL_SIZE + IRED_CACHE_SIZE];		// �������ݻ���
	//int32_t		amb[AMB_POOL_SIZE + AMB_CACHE_SIZE];			// ���������ݻ���
}ppg_data_t;

typedef struct
{
	union
	{
		int32_t		ecg_data[ECG_POOL_SIZE + ECG_CACHE_SIZE + 10];		// ECG���ݻ���
		ppg_data_t	ppg_data;
	};
	
	uint16_t	ld_data[LD_POOL_SIZE + LD_CACHE_SIZE];				// �������ݻ���
	uint32_t	ld_update_counter;									// �������¼���
	
	uint8_t     m_ecg_leadoff[5];
	
    uint32_t	pool_update_second;				// POOL�ظ��µ���������
    uint8_t		ppg_cache_update_cnt;			// PPG��cache���¼���
	uint8_t		ecg_cache_update_cnt;			// ECG��cache���¼���
    
    uint16_t	ppg_send_read_pos;             // PPG���͵Ķ�λ��
    uint16_t	ppg_send_wait_len;             // PPG�ȴ����ͳ���
    uint16_t	ppg_send_package_index;        // PPG���Ͱ���
	
	uint16_t	ecg_send_read_pos;             // ECG���͵Ķ�λ��
    uint16_t	ecg_send_wait_len;             // ECG�ȴ����ͳ���
    uint16_t	ecg_send_package_index;        // ECG���Ͱ���
}si1182_data_pool_t;
#pragma pack(pop)

#endif  //_CALC_DATA_TYPE_H_

