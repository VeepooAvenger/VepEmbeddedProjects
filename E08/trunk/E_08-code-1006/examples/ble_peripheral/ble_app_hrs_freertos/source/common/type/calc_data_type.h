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

#define ECG_ADC_FREQUENCY_APP_125HZ       0//1     //发给app的波形频率


#define PPG_BP_CALC_TIME            25      //计算血压时间为25s

// 上传采集数据枚举类型
typedef enum
{
    UPLOAD_ADC_NULL_EVENT = 0,          // 空事件
    UPLOAD_ADC_GSENSOR_EVENT = 1,       // 上传Gsensor原始ADC数据事件
    UPLOAD_ADC_GREEN_EVENT = 2,         // 上传PPG原始ADC数据事件
    UPLOAD_ADC_ECG_EVENT = 3,           // 上传ECG原始ADC数据事件
    UPLOAD_ADC_OXYGEN_EVENT = 4,         // 上传血氧原始ADC数据事件
    UPLOAD_ADC_GESTURE_EVENT = 5,       // 上传姿态数据
    UPLOAD_ADC_ECG_ORIGINAL_EVENT = 6,  // 上传ECG原始ADC数据事件(未滤波)
}UPLOAD_ADC_EVENT;

// 4900 配置模式
typedef enum
{
    MEASURE_MODE_NULL = 0,          // 测量未开启
    MEASURE_MODE_PPG,               // 仅ppg模式
    MEASURE_MODE_ECG,               // 仅ecg模式
    MEASURE_MODE_OXYGEN,             // 仅血氧模式
    //MEASURE_MODE_PPG_OXYGEN,         // PPG+血氧模式   
    
}MEASURE_MOD_E;

// 时间记录
typedef struct
{
    uint32_t  start_time;       // 起始时间
    uint32_t  end_time;         // 结束时间
    uint32_t  start_stack;      // 起始栈大小
    uint32_t  end_stack;        // 结束栈大小	
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


// ppg测量参数
typedef struct
{
    uint8_t     init:1;             // ppg初始化
    uint8_t     app:1;              // APP测量心率
    uint8_t     btn:1;              // 按键测量心率
    uint8_t     time:1;             // ppg自动测量
    uint8_t     app_bp:1;           // APP血压测量
    uint8_t     btn_bp:1;           // 按键测量血压
    uint8_t     res:2;              // 保留
      
    uint8_t     calc_bp_init:1;     // 血压算法初始化
    uint8_t     calc_wear_init:1;   // 佩戴算法初始化
    uint8_t     calc_res:6;         // 保留    
    
    uint8_t     bp_progress;        // 血压测量进度，201表示需要关闭app测量
    uint8_t     hreart_state;       // 心率app状态，1表示需要关闭app测量
    
    uint8_t     p_second_heart;     // 实时心率    
    uint8_t     p_minute_heart;     // 一分钟心率均值
    uint8_t     p_second_hrv;       // 每秒hrv
    uint8_t     p_second_rr;        // 每秒rr
    uint8_t     p_6sec_rr;          // 每6秒rr 
    uint8_t     p_wear_flag;        // ppg佩戴状态 
    
    
    uint8_t     p_bp_flag;          // 0测试中，1测试结果有效，2测试结果无效    
    uint8_t     p_bp_high_sec;
    uint8_t     p_bp_low_sec;
    uint8_t     p_bp_high_25sec;
    uint8_t     p_bp_low_25sec;
    uint8_t     p_bp_high_5min;
    uint8_t     p_bp_low_5min;
    
    uint32_t    p_calc_cnt;   // 计算次数
}ppg_measure_t;

// ecg测量参数
typedef struct
{
    uint8_t     init:1;             // ecg初始化
    uint8_t     app:1;              // APP测量心率
    uint8_t     btn:1;              // 按键测量心率
    uint8_t     time:1;             // ecg自动测量
    uint8_t     res:4;              // 保留
    
    uint8_t     calc_init:1;        // 计次初始化
    uint8_t     calc_res:6;         // 保留    
    
    uint8_t     e_second_heart;     // 实时心率    
    uint8_t     e_minute_heart;     // 一分钟心率均值
    uint8_t     e_second_hrv;       // 每秒hrv
    uint8_t     e_second_rr;        // 每秒rr
    uint8_t     e_6sec_rr;          // 每6秒hrv
    uint8_t     e_ary;              // 心率不齐
    uint8_t     e_second_br;        // 每秒呼吸率
    uint8_t     e_minute_br;        // 每分钟呼吸率
    uint8_t     e_wear_flag;        // ecg佩戴状态
    uint8_t     e_wear_time;        // ecg佩戴脱落时间1s
    uint16_t    e_qtc_time;         // QTC Time
    
    uint8_t     e_m_id;             // 重新开始标记，每次加1
    uint8_t     result_progress;    // 诊断进度
    uint32_t    e_calc_cnt;         // 测量次数
    
    uint32_t    e_app_calc_cnt;     // 测量次数
    
    ecg_test_result_t    ecg_test_result_s;   // 测量结果
    
    uint8_t     e_test_result_prg;  // 测量进度
    uint8_t     e_test_result_prg1;  // 测量进度
    
}ecg_measure_t;


// ecg测量参数
typedef struct
{
    uint8_t     ox_init:1;              // 初始化
    uint8_t     ox_app:1;               // APP测量
    uint8_t     ox_btn:1;               // 按键测量
    uint8_t     ox_time:1;              // 自动测量
    uint8_t     ox_rest:1;              // 重置配置    
    uint8_t     ox_calib:1;             // 校准重置 
    uint8_t     ox_res:2;               // 保留

    uint8_t     ox_result;              // 血氧实时测量结果
    uint8_t     ox_time_result[300];    // 血氧定时测量测试结果 
    uint16_t    ox_cnt;                 // 血氧测试次数
    
    uint16_t    ox_sum_result;          // 一分钟血氧累积和
    uint8_t     ox_sum_cnt;             // 一分钟血氧累积个数
		
	// FWT.20190304.	
    uint8_t bod_legal_flag;
    uint16_t ir_prefusion;//红外的灌注度
    uint16_t r_prefusion;//红光的灌注度
}oxygen_measure_t;


// 呼吸率测量结构体
typedef struct
{
    uint8_t     resp_init:1;            // 呼吸率测量初始化
    uint8_t     resp_resv:7;            // bit对齐保留
       
    uint8_t     resp_result_sec[300];   // 呼吸率测试结果 
	uint16_t    resp_cnt;               // 呼吸率测试次数
}respiratory_rate_measure_t;


// ppg\ecg 测量参数
typedef struct
{
    uint8_t                     ecg_ppg_mode;       // ecg\ppg\ppt模式
    uint8_t                     afe4900_is_open;    // 4900开启状态
    uint8_t                     read_data;          // 读取4900数据-切换状态过程中不读取，0，不读取，1读取
    uint8_t                     stop_int_cnt;       // 1182 开启下中断不响应计次(s)，用着异常处理
    uint8_t                     stop_read_cnt;      // 1182 开启下中断不读取数据时间(s)，用着异常处理
    ppg_measure_t               ppg_measure_s;      // ppg 参数
    ecg_measure_t               ecg_measure_s;      // ecg 参数
    oxygen_measure_t            oxygen_measure_s;   // oxygen 参数 
    respiratory_rate_measure_t  respiratory_rate_measure_s;  //呼吸率，只存储结果，不单独测量处理
}ecg_ppg_measure_t;

// 佩戴参数结构体
typedef struct
{
    uint8_t  init:1;             // 佩戴初始化
    uint8_t  resv:7;
    
    uint8_t  wear_not_pass_cnt;  // 佩戴不通过次数
    uint8_t  wear_status;        // 佩戴状态
}wear_measure_t;

// 脱手检测参数结构体
typedef struct
{
    uint8_t  init:1;             // 脱手检测初始化
    uint8_t  resv:7;

    uint8_t   uncall_wear_cnt;   // 未调用佩戴算法次数
    uint8_t   last_wear_status;  // 上次佩戴状态
    uint8_t   no_wear_result;    // 长时间不佩戴结果
    uint8_t   gesture;           // 5分钟的姿态
    uint32_t  amount;            // 运动量(五分钟)
    uint32_t  step;              // 记步值(五分钟)
}no_wear_measure_t;

//记步历史数据
typedef struct
{
    uint8_t   index;                    // 数据更新索引
    
    uint8_t   last_minute_step;         // 上一分钟记步值
    uint8_t   last_minute_sport;        // 上一分钟运动量
    uint32_t  power_on_step;            // 手环开机记步值
    uint32_t  step_history_count;       // 记步历史数据
    uint32_t  distance_history_count;   // 距离历史数据
    uint32_t  calories_history_count;   // 卡路里历史数据
    uint32_t  sport_history_count;      // 运动量历史数据
    uint16_t  step_count[5];            // 记步数(每分钟计算一次)
    uint16_t  sport_count[5];           // 运动量(每分钟计算一次)
    uint16_t  distance_count[5];        // 距离(每分钟计算一次)
    uint16_t  calories_count[5];        // 卡路里(每分钟计算一次)
    
    uint16_t  step_count_10m[10];       // 卡路里(每分钟计算一次)
}history_pedometer_param;


//记步历史数据
typedef struct
{
    uint8_t   hour;         //时
    uint8_t   minute;       //分
    uint8_t   second;       //秒   
    
    uint8_t   ppg_updata_cnt;
    uint8_t   ecg_updata_cnt;
    uint8_t   ox_updata_cnt;
}ecg_ppg_time_t;




#pragma pack ()

////////////////////////////////////
//   SI1182 缓冲区大小配置       //
///////////////////////////////////

// 绿光环境光的数据取余值
#define GREEN_AMB_CACHE_REMAINDER					(3 * 2)
// 绿光红外环境光的数据取余值
#define GREEN_IRED_AMB_CACHE_REMAINDER				(3 * 3)
// 绿光红光红外环境光的数据取余值
#define GREEN_RED_IRED_AMB_CACHE_REMAINDER			(3 * 4)
// 红光红外环境光的数据取余值
#define RED_IRED_AMB_CACHE_REMAINDER				(3 * 3)
// ECG取余值
#define ECG_CACHE_REMAINDER							(3)
// LD取余值
#define LD_CACHE_REMAINDER							(4)

// 绿光
#define GREEN_POOL_SIZE				(50 * 20)
#define GREEN_CACHE_SIZE			(50)
// 红光
#define RED_POOL_SIZE				(50 * 6)
#define RED_CACHE_SIZE				(50)
// 红外
#define IRED_POOL_SIZE				(50 * 6)
#define IRED_CACHE_SIZE				(50)
// 环境光
#define AMB_POOL_SIZE				(50 * 2)
#define AMB_CACHE_SIZE				(50)


// 绿光环境光数据更新缓存
#define GREEN_AMB_CACHE_SIZE			(GREEN_CACHE_SIZE + AMB_CACHE_SIZE)
// 绿光红外环境光数据更新缓存
#define GREEN_IRED_AMB_CACHE_SIZE		(GREEN_CACHE_SIZE + IRED_CACHE_SIZE + AMB_CACHE_SIZE)
// 绿光红光红外环境光数据更新缓存
#define GREEN_RED_IRED_AMB_CACHE_SIZE	(GREEN_CACHE_SIZE + RED_CACHE_SIZE + IRED_CACHE_SIZE + AMB_CACHE_SIZE)
// 红光红外环境光数据更新缓存
#define RED_IRED_AMB_CACHE_SIZE			(RED_CACHE_SIZE + IRED_CACHE_SIZE + AMB_CACHE_SIZE)

// PPG数据缓存
#define	PPG_POOL_SIZE			(GREEN_POOL_SIZE + RED_POOL_SIZE + IRED_POOL_SIZE + AMB_POOL_SIZE)
// PPG中断数据更新缓存
#define PPG_CACHE_SIZE			(50)

// ECG数据缓存
#define ECG_POOL_SIZE			(250*6)
// ECG中断数据更新缓存
#define ECG_CACHE_SIZE			(250)

// 导联数据缓存
#define LD_POOL_SIZE			(5)
#define LD_CACHE_SIZE			(1)

 
// AFE4900 数据缓存池结构体
#pragma pack(push)
#pragma pack(4)


typedef struct
{
	int32_t		green[GREEN_POOL_SIZE + GREEN_CACHE_SIZE];		// 绿光数据缓存
	int32_t		red[RED_POOL_SIZE + RED_CACHE_SIZE];			// 红光数据缓存
	int32_t		ired[IRED_POOL_SIZE + IRED_CACHE_SIZE];		// 红外数据缓存
	//int32_t		amb[AMB_POOL_SIZE + AMB_CACHE_SIZE];			// 环境光数据缓存
}ppg_data_t;

typedef struct
{
	union
	{
		int32_t		ecg_data[ECG_POOL_SIZE + ECG_CACHE_SIZE + 10];		// ECG数据缓存
		ppg_data_t	ppg_data;
	};
	
	uint16_t	ld_data[LD_POOL_SIZE + LD_CACHE_SIZE];				// 导联数据缓存
	uint32_t	ld_update_counter;									// 导联更新计数
	
	uint8_t     m_ecg_leadoff[5];
	
    uint32_t	pool_update_second;				// POOL池更新的数据秒数
    uint8_t		ppg_cache_update_cnt;			// PPG的cache更新计数
	uint8_t		ecg_cache_update_cnt;			// ECG的cache更新计数
    
    uint16_t	ppg_send_read_pos;             // PPG发送的读位置
    uint16_t	ppg_send_wait_len;             // PPG等待发送长度
    uint16_t	ppg_send_package_index;        // PPG发送包数
	
	uint16_t	ecg_send_read_pos;             // ECG发送的读位置
    uint16_t	ecg_send_wait_len;             // ECG等待发送长度
    uint16_t	ecg_send_package_index;        // ECG发送包数
}si1182_data_pool_t;
#pragma pack(pop)

#endif  //_CALC_DATA_TYPE_H_

