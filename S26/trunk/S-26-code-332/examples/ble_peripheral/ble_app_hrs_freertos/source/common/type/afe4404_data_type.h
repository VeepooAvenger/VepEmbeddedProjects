#ifndef _AFE4404_DATA_TYPE_H_
#define _AFE4404_DATA_TYPE_H_

#include <stdint.h>
#include "Respiratory_Rate.h"
#include "Blood_OxygenDetection.h"


// 定义血氧的特殊值
#define OX_SPECIAL_VALUES_RANDOM 254    // 血氧随机值-app获取后随机填充
#define OX_SPECIAL_VALUES_WEAR   253    // 血氧未佩戴-app获取后填充
#define OX_SPECIAL_VALUES_INT    252    // 血氧测量被打断-app获取后填充


#pragma pack (1)
#pragma anon_unions

// 上传采集数据枚举类型
typedef enum
{
    UPLOAD_ADC_NULL_EVENT = 0,         // 空事件
    UPLOAD_ADC_GSENSOR_EVENT = 1,      // 上传Gsensor原始ADC数据事件
    UPLOAD_ADC_GREEN_EVENT = 2,        // 上传绿光原始ADC数据事件
    UPLOAD_ADC_OXYGEN_EVENT = 3,       // 上传血氧原始ADC数据事件
    UPLOAD_ADC_FATIGUE_EVENT = 4,      // 上传疲劳度原始ADC数据事件
    UPLOAD_ADC_GESTURE_EVENT = 5,      // 上传姿态数据
}UPLOAD_ADC_EVENT;

// 时间记录
typedef struct
{
    uint32_t  start_time;       // 起始时间
    uint32_t  end_time;         // 结束时间
    uint32_t  start_stack;      // 起始栈大小
    uint32_t  end_stack;        // 结束栈大小	
}trace_message_t;

// 疲劳度测量参数
typedef struct
{
    uint8_t   init:1;           // 疲劳度初始化
    uint8_t   app:1;            // App对疲劳度测量
    uint8_t   resv:6;           // 对齐保留位
}fatigue_measure_t;

// 呼吸率测量结构体
typedef struct
{
    uint8_t  init:1;           // 呼吸率测量初始化
    uint8_t  app:1;            // APP测量呼吸率
    uint8_t  time:1;           // 定时测量呼吸率
    uint8_t  min_init:1;       // 每分钟初始化一次数据缓存
    uint8_t  resv:4;           // bit对齐保留
    
    uint8_t  mode;              // 模式 0：正常模式  1：测试模式 2:持续测试模式，手动停止
    uint8_t  percent;           // APP百分比

    uint8_t  err_code;          // 呼吸率错误码
    uint8_t  result;            // 呼吸率测试结果    
    uint8_t  result_sec[300];   // 呼吸率测试结果 
    resp_second_t resp_second_s;// 呼吸率的瞬时值
    uint8_t wave_station;       // 当前波形的质量
    uint8_t all_flag;           // 波形质量和数据缺失标志 
	uint16_t  cnt;               // 呼吸率测试次数
}respiratory_rate_measure_t;

// 呼吸暂停参数结构体
typedef struct
{
    uint8_t  init:1;           // 算法初始化
    uint8_t  resv:7;           // 对齐保留
    
    uint8_t  err_code;         // 错误码
}breath_stop_measure_t;

// 血氧测量参数结构体
typedef struct
{
    uint8_t   init:1;           // 初始化
    uint8_t   btn:1;            // 按键血氧测量
    uint8_t   app:1;            // APP血氧测量
    uint8_t   time:1;           // 定时血氧测量
	uint8_t   cal_flag:1;       // 血氧校准参数标志
	uint8_t   cal_success:1;    // 血氧校准成功标志
    uint8_t   ox_day_time:1;    // 白天血氧自动测量
    uint8_t   resv:1;           // 对齐保留
    
    uint8_t   ox_calc_cnt;      // 校准完成后，计算次数，用于白天自动测量 
    
    uint8_t  current_green;		// 电流
    uint8_t  current_red;		// 电流
    uint8_t  current_ird;		// 电流
    
	uint8_t  gain_gre;              // 增益
	uint8_t  gain_red;              // 增益
	uint8_t  gain_ird;              // 增益
    uint8_t  offdac_grn;        //OFFDAC参数grn
    uint8_t  offdac_red;        //OFFDAC参数red
    uint8_t  offdac_ird;        //OFFDAC参数ird
	
	uint16_t  r_prefusion;
	uint16_t  ir_prefusion;
	uint8_t   bod_legal_flag;
	
	uint32_t   r_ctr;//红光的转换率
	uint32_t   ir_ctr;//红外的转换率
    
    uint8_t test_mode;          //测试模式，4：过医疗模式
    uint8_t algorithm_flg;      //算法结果被使用的标志，0：使用算法结果 1：不使用算法结果
    uint8_t add_sub_flg;        //对血氧值进行修正的标志，0：不修正 1：加offse_buf 2：减offse_buf
    uint8_t offse_buf;          //偏移值
    uint8_t oxy_buf;            //直接使用血氧值，algorithm_flg为1时有效
    
    bod_app_change_value_t medical_param_s;  //过医疗时使用的参数
    
    uint8_t  err_code;          // 错误码
    uint8_t  result;            // 血氧实时测量结果
    uint16_t cnt;               // 血氧测试次数
    uint16_t min_start_index;   // 血氧数据分钟的起始值
    uint8_t  time_result[300];  // 血氧定时测量测试结果
    
    uint8_t cal_state;          // 血氧校准的状态
    uint8_t cal_progress;       // 血氧校准的进度
    uint32_t bo_ac_r_ir;
    //uint8_t test_mode;          // 血氧测试的模式
    
    uint8_t calc_cnt_test;      //统计算法调用次数，用于异常处理

    uint8_t ox_prompt_sign_flag;  //信号灌注度不行，需要关灯熄屏
    uint8_t ox_no_real_spo2_value_flag; // 尚未出真实血氧值，显示--

}blood_oxygen_measure_t;

typedef struct
{
    uint8_t  enable:1;          // 启动血氧验证测量
    uint8_t  resv:7;            // 对齐保留
}blood_oxygen_verify_t;

typedef struct
{
    uint8_t   init:1;           // 心率初始化
    uint8_t   app:1;            // APP测量心率
    uint8_t   btn:1;            // 按键测量心率
    uint8_t   day_time:1;       // 心率定时测量(白天--开绿灯测量)
    uint8_t   night_time:1;     // 心率定时测量(夜间--三个灯全开)
    uint8_t   min_init:1;       // 每分钟初始化一次数据缓存
    uint8_t   calib_init:1;      // 校准初始化
    uint8_t   calib_well:1;     // 校准完成
    
    uint8_t  real_heart;        // 实时心率
    uint8_t  time_heart;        // 定时测量心率结果	
    uint8_t  time_measure_cnt;  // 定时测量次数
    uint8_t  valid_heart_cnt;   // 有效心率测试次数
    uint8_t  heart_cache[10];   // 心率测试结果(从血氧中获取)
}heart_measure_t;

typedef struct
{
    uint8_t     bp_init:1;          // 血压初始化
    uint8_t     bp_app:1;           // APP测量血压
    uint8_t     bp_btn:1;           // 按键测量血压
    uint8_t     bp_day_time:1;      // 血压定时测量(白天--开绿灯测量)
    uint8_t     bp_night_time:1;    // 血压定时测量(夜间--三个灯全开)
    uint8_t     bp_min_init:1;      // 每分钟初始化一次数据缓存
    uint8_t     bp_calib_init:1;    // 校准初始化
    uint8_t     bp_calib_well:1;    // 校准完成

    uint8_t     bp_test_mode;       // 测量模式
    uint8_t     bp_set_sp;          // 设置高压
    uint8_t     bp_set_dp;          // 设置低压

    uint8_t     bp_calc_prg;        // 计算次数:20次后出结果
    uint8_t     bp_flag;            // 血压计算结果状态
    uint8_t     bp_sp_value;        // 高压
    uint8_t     bp_dp_value;        // 低压
    
}bp_measure_t;

//HRV测量参数结构体
typedef struct
{
    uint8_t  init:1;       // HRV测量初始化
    uint8_t  time:1;       // 定时HRV测量
    uint8_t  min_init:1;   // 每分钟初始化一次数据缓存
    uint8_t  resv:5;       // 保留对齐
    
    uint8_t  err_code;     // HRV错误码
    uint8_t  cnt;          // HRV测试次数
    uint8_t  result[60];   // HRV测试结果
}hrv_measure_t;

// 心脏负荷参数结构体
typedef struct
{
    uint8_t  err_code;    // 心脏负荷错误码
    uint8_t  cnt;         // 心脏负荷有效结果次数
    uint8_t  result[60];  // 心脏负荷结果
}heart_load_param_t;

// 散点图心率参数结构体
typedef struct
{
    uint8_t  heart[10];   // 心率值
    uint8_t  err_code;    // 错误码
    uint8_t  cnt;         // 有效结果次数
}scatter_heart_param_t;

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
    uint8_t   index;                // 数据更新索引
    
    uint8_t   last_minute_step;     // 上一分钟记步值
    uint8_t   last_minute_sport;    // 上一分钟运动量
    uint32_t  power_on_step;        // 手环开机记步值
    uint32_t  step_history_count;   // 记步历史数据
    uint32_t  distance_history_count;   // 距离历史数据
    uint32_t  calories_history_count;   // 卡路里历史数据
    uint32_t  sport_history_count;  // 运动量历史数据
    uint16_t  step_count[5];        // 记步数(每分钟计算一次)
    uint16_t  sport_count[5];       // 运动量(每分钟计算一次)
    uint16_t  distance_count[5];        // 距离(每分钟计算一次)
    uint16_t  calories_count[5];       // 卡路里(每分钟计算一次)
}history_pedometer_param;
#pragma pack ()

#define PPG_BP_CALC_TIME        25  //血压计算次数

//========================================
//   ******AFE4404 缓冲区大小配置******
//========================================
#define RED_POOL_SIZE          300
//YX.20180628.修改绿光和红外的数据大小
#define IRED_POOL_SIZE         300//750//
#define GREEN_POOL_SIZE        750//300//
//YX.20180628.把佩戴的绿光数据单独出来
#define WEAR_GREEN_POOL_SIZE   100
#define AMBIENT_POOL_SIZE      100
#define DATA_CACHE_POOL_SIZE   50

// AFE4404数据缓存池结构体
typedef struct
{
    uint32_t  pool_update_second;        // POOL池更新的数据秒数
    uint8_t   cache_update_cnt;          // cache更新数量
    
    uint16_t  send_read_pos;             // 心率发送的读位置
    uint16_t  send_wait_len;             // 心率等待发送长度
    uint16_t  send_package_index;        // 心率发送包数

    // 内存复用数据缓存
    union multiplex_data
    {
        struct bo
        {
              int32_t  red[RED_POOL_SIZE + DATA_CACHE_POOL_SIZE];        // 血氧-红光数据缓冲区
              int32_t  ired[IRED_POOL_SIZE + DATA_CACHE_POOL_SIZE];      // 血氧-红外数据缓冲区    
              int32_t  green[GREEN_POOL_SIZE + DATA_CACHE_POOL_SIZE];    // 绿光数据缓冲区
        };
        //int32_t fatigue_data[200+50];                                   // 疲劳度数据缓存	 
    };

    int32_t  wear_green[WEAR_GREEN_POOL_SIZE];                           // 佩戴的绿光数据缓冲区
    int32_t  ambient[AMBIENT_POOL_SIZE + DATA_CACHE_POOL_SIZE];          // 环境光数据缓冲区 
    
//    // 内存复用数据缓存
//    union multiplex_data
//    {
//        struct bo
//        {
//              uint32_t  red[RED_POOL_SIZE + DATA_CACHE_POOL_SIZE];        // 血氧-红光数据缓冲区
//              uint32_t  ired[IRED_POOL_SIZE + DATA_CACHE_POOL_SIZE];      // 血氧-红外数据缓冲区    
//              uint32_t  green[GREEN_POOL_SIZE + DATA_CACHE_POOL_SIZE];    // 绿光数据缓冲区
//        };
//        uint32_t fatigue_data[800+200];                                   // 疲劳度数据缓存	 
//    };

//    uint32_t  wear_green[WEAR_GREEN_POOL_SIZE];                           // 佩戴的绿光数据缓冲区
//    uint32_t  ambient[AMBIENT_POOL_SIZE + DATA_CACHE_POOL_SIZE];          // 环境光数据缓冲区 
}afe4404_data_pool;



#endif
