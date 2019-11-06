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
** Descriptions:            ����ecg������λ�½���12.5(��ǰʱ50�ǵ���,�ſ�12.5������λ�Ŀ�ѡ�Ŵ���(���о�����)
**------------------------------------------------------------------------------
** Modified by:             WSF
** Created data:            2019-03-11
** SYS_VERSION:             01.01.00.56
** Descriptions:            �޸�hrv����ĳ�ʼ��ֵ(��Ϊ255),����Ƕ��ʽ�����app������
** Memoryռ��:�ο�20190225(YQW)ͳ��
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
extern int32_t ecg_filter_data[250];//�ĵ����ݵ���ʾ����

typedef struct tag_ecg_afe4490_parameter
{
    uint8_t register_current;
    uint8_t register_scale;
}ecg_afe4490_parameter_t;

typedef struct tag_ecg_result
{
    // �������
    uint8_t  heart_rate;        // ÿ��HR
    uint8_t  hr_per_min_result; // ÿ����HR��ֵ����Ӧ�ճ���������1~5��
    // HRV���
    uint8_t  hrv;// ÿ��HRV
    // RR���
    uint8_t  rr;// ÿ��rr
    uint8_t  rr_per_6sec_result;// ÿ6��RR
    uint8_t  ary;// ���ʲ���
    // ���������
    uint8_t  resp_per_second_result;// ÿ�������
    uint8_t  resp_per_min_result;// ÿ���Ӻ����ʾ�ֵ����Ӧ�ճ����ݺ�����1~5��
    // ���������־λ
    uint8_t  lead_off_detec_res;
    // ��Ͻ��ȱ�־λ
    uint8_t  measurement_progress;// ˵�����ý��ȱ�־λ��ÿ��������ý��ȱ�־λ���ֵΪ30�������ʶ�𵽸�ֵΪ30�󼴿���Ͻ���������ʶ������ֵ��
    uint16_t QTC_sec;
    // Afe4900У׼��Ϣ
    uint8_t  adjust_flag;//У׼�ı�־λ��0,3��ʾ����Ҫ�ԼĴ����Ĳ��������޸ģ�1,2��Ҫ�޸�
    ecg_afe4490_parameter_t ecg_afe4490_parameter_new;//�Ĵ����Ĳ�����ֵ
	//test
	uint8_t RR_cnt;

    uint8_t ecg_ld_judge_flag; // 0:���ͨ��  1:�����ͨ��

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
    uint8_t                 ecg_leadoff;//1182ֻ��һ��
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
