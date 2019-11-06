/********************************Copyright(c)***********************************
**                           Veepoo Tech
**                         COPYRIGHT (C) 2016
**--------------File Info-------------------------------------------------------
** File name:               Blood_OxygenDetection.h
** Last modified Date:       2016-07-27
** Last SYS_VERSION:            01.01.00.01
** Descriptions:
**
**------------------------------------------------------------------------------
** Created by:              HYR
** Created date:            2016-07-27
** SYS_VERSION:                 01.01.00.01
** Descriptions:
**------------------------------------------------------------------------------
** Modified by:             HYR
** Modified date:           2017-09-09
** SYS_VERSION:             01080100.20170909
** Descriptions:            1. Official verion
**------------------------------------------------------------------------------
** Modified by:             HYR
** Modified date:           2017-09-13
** SYS_VERSION:             01080200
** Descriptions:            1.Arrange the result processing fucntion
2.Do rising again process before 40 seconds
3.Change the system variance from 0.3 to 0.5
4.Modified the init function
**------------------------------------------------------------------------------
** Modified by:             FWT
** Modified date:           2017-09-14
** SYS_VERSION:             0x01080101. 01080210  0x01080102. 0x01080251.20170915HYR
** Descriptions:            1.Add normal drop optimization
2.Arrage the First result process
3.down trend process
**------------------------------------------------------------------------------
** Modified by:             FWT
** Modified date:           2017-09-14
** SYS_VERSION:             0x01080101. 01080210  0x01080102. 0x01080251.20170915HYR  0x01080253 20170926 0x01080255 20170929
** Descriptions:            1.Add normal drop optimization
2.Arrage the First result process
3.down trend process
4. Update the temp fifo
5. Signal quality change
6. ratio control
7.result deal
**------------------------------------------------------------------------------
** Modified by:             HYR
** Modified date:           2017-11-01
** SYS_VERSION:             0x01080600  //01080601
** Descriptions:            1.change the struct define
**------------------------------------------------------------------------------
** Modified by:             HYR
** Modified date:           2018-08-08
** SYS_VERSION:             0x01180009
** Descriptions:
1. Change the night quality check function of different features
**------------------------------------------------------------------------------
** Modified by:             HYR
** Modified date:           2018-08-09
** SYS_VERSION:             0x01180010
** Descriptions:
1. Change the night move factor check
2. Add the matching curve equation rechose in ac factor
**------------------------------------------------------------------------------
** Modified by:             HYR
** Modified date:           2018-08-20
** SYS_VERSION:             0x01180017
** Descriptions:
1. Change the max diff num judge param
2. Add abs for diff
**------------------------------------------------------------------------------
*******************************************************************************/

/********************************error_code**********************************************/
#ifndef __BLOOD_OXYGENDETECTION_
#define __BLOOD_OXYGENDETECTION_
#include "stdlib.h"
#include "stdint.h"


#ifdef _WIN64
//#define MATLAB_PLOT_MODE
#ifdef MATLAB_PLOT_MODE
#define MATLAB_PLOT_MODE1
#define MATLAB_PLOT_MODE2
#define MATLAB_PLOT_MODE3
#define MATLAB_PLOT_MODE4
#include "plot_bymatlab.h"
#endif // DEBUG
#endif

// 卡尔曼回升速度控制
#define KALMAN_BACKUP_CNT     (1)
#define KALMAN_BACKUP_V       (2)

// 微分法的找点数
#define CALC_DIFF_POINTS                    (5)

#define SI1182_DATA_MODE
#define SI1182_DATA_RAW_MODE
#define SAVE_PERFU_COUNT                    (10)
#define FINGER_MODE_DOWN_DIFFICULT          (0.5f)
#define WRIST_MODE_DOWN_DIFFICULT           (0.01f)

//finding max pulse num  12 to 18DH_2017_0607
#define E_SPO2_MAX_WAVE_NUM   18
#define CALBRATION_LEN        28
#define SAFE_FREE(p)          \
				 if(p != NULL)\
					{         \
					  free(p);\
					 p = NULL;\
					}
#ifdef _WIN64
//#define MATLAB_PLOT_MODE
#endif

#ifdef MATLAB_PLOT_MODE

#endif

typedef struct
{
    uint8_t  rising_num;
    uint8_t  shake_flag;
    int8_t   period_value_std;
    uint8_t  signal_diff_flag;
    uint8_t  diff_peak_valley;
    //HYR.20180815.Add
    uint16_t  max_diff_num;
    //HYR.20180820.Add
    uint16_t  min_diff_num;
    // 20170310  HJJ
    int16_t  peak_value_std;
    // 20170310  HJJ
    int16_t  valley_value_std;
    uint16_t cycle_diff_max;
    //int16 to int32  FWT
    int32_t  amp_value_ave;
    //20170401 HJJ ·  int16 to int32  FWT
    int32_t  amp_value_std;

    float    cycle_diff_std;
    float    peak_std_value_rate;
    float    valley_std_value_rate;
    float    diff_mean_amp_rate;
    float    amp_std_value_rate;
}bod_signal_char_t;

typedef struct
{
    uint8_t  rising_num;
    uint8_t  shake_flag;

    int32_t  term[E_SPO2_MAX_WAVE_NUM - 1];
    int32_t  start_val[E_SPO2_MAX_WAVE_NUM];
    int32_t  peak_val[E_SPO2_MAX_WAVE_NUM];
    int32_t  amp_val[E_SPO2_MAX_WAVE_NUM];
    int32_t  start_point[E_SPO2_MAX_WAVE_NUM];
    int32_t  peak_point[E_SPO2_MAX_WAVE_NUM];

    int32_t  wave_ac[E_SPO2_MAX_WAVE_NUM];
    int32_t  wave_dc;
    uint32_t ac_aver;
    int32_t  diff_max_min_amp;

    float    mean_amp_max_min;
    float    diff_mean_amp_rate;
}bod_wave_char_t;

typedef struct
{
    uint8_t ori_oxygen_value;

    uint8_t cur_out_oxygen_value;
    uint8_t ployfit_param_flag;

    uint8_t rechoose_ployfit_param_flag;

    uint8_t smooth_filter_times;
    uint8_t smooth_filter_points;
    uint32_t calc_times;
    float cur_quality_value;
    float ployfit_diff_value;
    float cur_rate;
}bod_app_change_value_t;



typedef struct
{
    //need
    uint8_t bo_degree;
    uint8_t bo_degree1;
    uint8_t bo_degree2;
    uint8_t bo_degree3;
    uint8_t bo_pulse;
    uint8_t wave_station;
    uint8_t judge_flag;
    uint8_t wave_valid_flag;
    //20170310 HJJ signal saturated flag  signal_satu_flag =1,???;signal_satu_flag=2,???;signal_satu_flag= 0,???
    uint8_t dac_grn;
    uint8_t dac_red;
    uint8_t dac_ird;
    uint8_t signal_quality_display_degree;

    uint8_t signal_station;
    uint8_t signal_quality_flag;

    //uint8_t SPO2_control_flag;
    //uint8_t SPO2_heart_step_track;
    uint8_t afe4404_calib_judge_mode;
    uint8_t afe4404_calib_adjust_times;
    uint8_t shake_flag;
    uint8_t afe4404_transfer_time_flag;
    uint8_t afe4404_calib_mark_write_reg;
    uint8_t afe4404_calib_over;
    uint8_t afe4404_calib_fail_flag;
    uint8_t afe4404_total_time;
    uint8_t afe4404_all_well_down_flag;

    uint8_t r_dc_aver;
    uint8_t ir_dc_aver;
    uint8_t bo_nihe_flag;
    uint8_t bad_ori_quality_cnt;

    uint16_t r_ac_aver;
    uint16_t ir_ac_aver;

    uint16_t red_ctr;
    uint16_t ird_ctr;
    uint16_t green_ctr;

    uint8_t bod_legal_flag;
    uint16_t ir_prefusion;//红外的灌注度
    uint16_t r_prefusion;//红光的灌注度

    //uint16_t spo2_judge;
    float	wave_quality_degree;
    //float   red_quanlity;
    //float   ired_quanlity;
    float   bo_r_ir;
    float   bo_dc_r_ir;
    float   bo_ac_r_ir;
    //21H
    uint32_t afe4404_calbuletion_gain;
    //22H
    uint32_t afe4404_calbuletion_current_red;
    //22H
    uint32_t afe4404_calbuletion_current_green;
    //22H
    uint32_t afe4404_calbuletion_current_ird;
    uint32_t afe4404_adjust_schedule;
    uint32_t flow_disp;



    uint8_t prompt_sign_flag;   // 默认：0
    uint8_t no_real_spo2_value_flag;  //默认：1

    float perfusion_degree;

    // ==========================================================
    // 红光电流
    uint16_t i_red;
    //
    uint16_t i_ird;
    //
    uint8_t adc_range_red;
    //
    uint8_t adc_range_ird;
    //
    uint8_t clk_div_red;
    //
    uint8_t clk_div_ird;
    //
    uint8_t decim_red;
    //
    uint8_t decim_ird;
    //
    uint8_t adc_avg_red;
    //
    uint8_t adc_avg_ird;
    //
    uint8_t ppg_sw_avg_all;
    //
    uint8_t calib_1182_over_flag;
    //
    uint8_t calib_1182_times;
    //
    uint16_t calib_meas_rate;
    //
    uint8_t  write_reg_flag;
    //
    uint8_t  sig_satu_flag;
}bod_result_info_t;



typedef struct
{
    //???
    uint8_t bod_move_situation;
    //????
    uint8_t bod_night_flag;
    //??
    uint8_t bod_gesture;
    //?????
    uint8_t bod_simulate_flag;
    // 0x21
    uint32_t rf_register;
    // 0x22
    uint32_t current_register;
    // 0x3A
    uint32_t dac_register;

    uint16_t i_red;
    //
    uint16_t i_ird;
    //
    uint8_t adc_range_red;
    //
    uint8_t adc_range_ird;
    //
    uint8_t clk_div_red;
    //
    uint8_t clk_div_ird;
    //
    uint8_t decim_red;
    //
    uint8_t decim_ird;
    //
    uint8_t adc_avg_red;
    //
    uint8_t adc_avg_ird;
    //
    uint8_t ppg_sw_avg_all;
    //
    uint16_t calib_meas_rate;
    //
    uint8_t red_led_count;
    //
    uint8_t ird_led_count;
    //
    uint8_t task_mode;
}detection_char_info_t;



typedef struct
{
    uint8_t r_signal_satu_flag;
    //20170310 HJJ signal valid or not flag
    uint8_t r_signal_valid_flag;
    uint8_t r_signal_line_flag;
    uint8_t r_signal_diff_flag;
    uint8_t ir_signal_satu_flag;
    //20170310 HJJ signal valid or not flag
    uint8_t ir_signal_valid_flag;
    uint8_t ir_signal_line_flag;
    uint8_t ir_signal_diff_flag;
    uint8_t best_signal_flag;
    float diff_high_quality_flag;
}orignal_signal_info_t;
/********************************************************************
** Function name:           Blood_OxygenInit
** Descriptions:
** input parameters:        none
** output parameters:       none
** Returned value:          none
** Created by:              HYR
** Created Date:            2016-07-27
********************************************************************/
void Blood_OxygenInit(void);

/********************************************************************
** Function name:           Bod_CalibrationLength
** Descriptions:            Get calibration length
** input parameters:        none
** output parameters:       none
** Returned value:          0:free flase; 1: not using malloc;  2: free succeed
** Created by:              HYR
** Created Date:            2017-07-27
********************************************************************/
uint8_t Bod_CalibrationLength(void);

/********************************************************************
** Function name:           Blood_OxygenDetection
** Descriptions:
** input parameters:        bod_red_signal  bod_infrared_signal bod_ambient_signal
** output parameters:       none
** Returned value:          none
** Created by:              HYR
** Created Date:            2016-07-27
********************************************************************/
uint8_t Blood_OxygenDetection(
    const int32_t            *bod_red_singal,
    const int32_t            *bod_infrared_signal,
    detection_char_info_t    *fator_in);
/********************************************************************
** Function name:           Get_AppChangeLibValue
** Descriptions:            return app_change_value
** input parameters:
** output parameters:
** Returned value:          none
** Created by:              FWT
** Created Date:            2018-08-27
********************************************************************/
bod_app_change_value_t Get_AppChangeLibValue(void);
/********************************************************************
** Function name:           Set_AppChangeLibValue
** Descriptions:            return err
** input parameters:		bod_app_change_value_t app_change_value_s
** output parameters:
** Returned value:          none
** Created by:              FWT
** Created Date:            2018-08-27
********************************************************************/
uint8_t Set_AppChangeLibValue(bod_app_change_value_t app_change_value_s);
/********************************************************************
** Function name:           Get_OxygenInfo
** Descriptions:
** input parameters:        none
** output parameters:       none
** Returned value:          bod_info_s
** Created by:              HYR
** Created Date:            2016-07-27
********************************************************************/
bod_result_info_t *Get_OxygenInfo(void);

/********************************************************************
** Function name:           Get_BodVersion
** Descriptions:
** input parameters:        none
** output parameters:       none
** Returned value:          BOD_VERSION
** Created by:              HYR
** Created Date:            2016-07-29
********************************************************************/
uint32_t Get_BodVersion(void);




#endif




