/********************************Copyright(c)***********************************
**                           Veepoo Tech
**                         COPYRIGHT (C) 2018-2019
**--------------File Info-------------------------------------------------------
** File name:                ecg_test3.h
** Last modified Date:       2018-10-11
** Last SYS_VERSION:         01.01.01.01
** Descriptions:
**
**------------------------------------------------------------------------------
** Modified by:              YQW
** Created data:            2019-02-27
** SYS_VERSION:             01.01.00.55
** Descriptions:            由于ecg电流档位下降到12.5(以前时50那档）,放开12.5电流档位的可选放大倍数(所有均可用)
**------------------------------------------------------------------------------
** Modified by:             WSF
** Created data:            2019-03-11
** SYS_VERSION:             01.01.00.56
** Descriptions:            修改hrv结果的初始化值(改为255),方便嵌入式软件或app做过滤
** Memory占用:参考20190225(YQW)统计
*******************************************************************************/
#ifndef __ECG_DETECT__
#define __ECG_DETECT__

#ifdef _WIN64
//#define MATLAB
#endif // DEBUG

#ifdef MATLAB
#include "plot_bymatlab.h"
#endif // DEBUG


#include "stdint.h"
#include "pedometer.h"



/*
* 20190225-01010254(YQW):
* * Global: 1237(Bytes) + 105(Bytes-resp_ecg) = 1342(Bytes)
* * Malloc: 7600(Bytes)
* * Local:  2399(Bytes)
* * Const:  55(Bytes)
*/

#define AFE4900_ECG_ALG_VERSION     (0x02000301)
#define ECG_DATA_LEN                (1500)
#define REAL_DATA_LE                (1400)
#define ECG_SAMPLE_RATE             (250)
#define ECG_MAX_CYCLE               (24)
#define G_SIZE                      (1500)
#define FILTER_TIMES                (7)
#define FILTER_LEN                  (20)
#define LOCAL_LEN                   ((G_SIZE + FILTER_LEN + 10) / 2)
#define ECG_LD_JUDGE_VALUE          (7000000)
extern int32_t ecg_filter_data[250];//心电数据的显示缓存

typedef struct tag_ecg_afe4490_parameter
{
    uint8_t register_current;
    uint8_t register_scale;
}ecg_afe4490_parameter_t;

typedef struct tag_ecg_result
{
    // 心率相关
    uint8_t  heart_rate;        // 每秒HR
    uint8_t  hr_per_min_result; // 每分钟HR均值（对应日常数据心率1~5）
    // HRV相关
    uint8_t  hrv;// 每秒HRV
    // RR相关
    uint8_t  rr;// 每秒rr
    uint8_t  rr_per_6sec_result;// 每6秒RR
    uint8_t  ary;// 心率不齐
    // 呼吸率相关
    uint8_t  resp_per_second_result;// 每秒呼吸率
    uint8_t  resp_per_min_result;// 每分钟呼吸率均值（对应日常数据呼吸率1~5）
    // 导联脱落标志位
    uint8_t  lead_off_detec_res;
    // 诊断进度标志位
    uint8_t  measurement_progress;// 说明：该进度标志位非每秒计数；该进度标志位最大值为30；软件在识别到该值为30后即可诊断结束，不再识别其后的值；
    uint16_t QTC_sec;
    // Afe4900校准信息
    uint8_t  adjust_flag;//校准的标志位，0,3表示不需要对寄存器的参数进行修改，1,2需要修改
    ecg_afe4490_parameter_t ecg_afe4490_parameter_new;//寄存器的参数的值
	//test
	uint8_t RR_cnt;

    uint8_t ecg_ld_judge_flag; // 0:佩戴通过  1:佩戴不通过

    uint32_t reserve4;
}ecg_result_t;

typedef struct tag_ecg_result_info
{
    uint8_t  lead_off_type;
    uint8_t  diag_param1;
    uint8_t  diag_param2;
    uint8_t  diag_param3;
    uint8_t  diag_param4;
    uint8_t  diag_param5;
    uint8_t  diag_param6;
    uint8_t  diag_param7;
    uint8_t  diag_param8;
    uint8_t  heart_rate;
    uint8_t  resp_rate;
    uint8_t  hrv;
    uint16_t QT_time;
    uint32_t rese1;
    uint32_t rese2;
    uint32_t rese3;
}ecg_result_info_t;

typedef struct tag_ecg_diag_result
{
    uint8_t           ecg_result_save_flag;
    ecg_result_info_t ecg_result_info_s;
}ecg_diag_result_t;

typedef struct tag_ecg_enter
{
    uint8_t                 diga_mode;
    uint8_t                 func_times;
    uint8_t                 ecg_leadoff;//1182只有一个
    ecg_afe4490_parameter_t ecg_afe4490_parameter_s;
}ecg_enter_t;



/*
* functions declarations for external call
*/
extern void    Ecg_DetectInit(void);
extern int32_t Ecg_DetectEntry( const int32_t* ede_ecg_signal,
                                move_index_t*  movement_struct_s,
                                ecg_enter_t    ecg_enter_info
                              );
extern void    Ecg_GetResult(ecg_result_t* egr_result_s);
extern void    Ecg_GetDiagResult(ecg_diag_result_t* ecg_diag_result_s);

#endif //                     
