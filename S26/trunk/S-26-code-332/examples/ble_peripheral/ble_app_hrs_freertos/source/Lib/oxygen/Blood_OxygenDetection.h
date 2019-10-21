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
//finding max pulse num  12 to 18DH_2017_0607

#ifdef _WIN64
#define MATLAB_PLOT_MODE
#ifdef MATLAB_PLOT_MODE
#define MATLAB_PLOT_MODE1
//#define MATLAB_PLOT_MODE2
#define MATLAB_PLOT_MODE3
#endif
#define TEST9_USE_MODE
#endif

// CTR 差值校准模式开启  开启该模式会在校准前增加一段时间用于计算最佳CTR
#define CTR_DIFF_CALIB

// 微分法的找点数
#define CALC_DIFF_POINTS      (20)
// 卡尔曼回升速度控制
#define KALMAN_BACKUP_CNT     (1)
#define KALMAN_BACKUP_V       (2)


#define NIGHT_MODE_LIMIT_FLAG
#ifdef NIGHT_MODE_LIMIT_FLAG
//#define NIGHT_MODE_GESTURE_LIMIT
//#define NIGHT_MODE_SIGORIQUALITY_LIMIT
#define NIGHT_MODE_SIGDCCHANGE_LIMIT
#endif

//#define LD_TEST

#define TARGET_RED_ELEC       (0.10f)
#define TARGER_DAC_ADC_VALUE  (28000)
// AMBDAC减影模式的值

#define AMB_DAC_MODE          (3)   //0 3 5 7
// AMBDAC减影模式下的DAC应该选取的值
#define IFS_DAC               (2)


//#define ILED_IFS_50
#define ILED_IFS_100

#ifdef ILED_IFS_100
// 初始化的电流刻度 100mA下选择35刻度  50mA下选择70刻度
#define INIT_ILED_SCALES      (35)
// ILED的1刻度对应多少毫安的电流
#define ILED_IFS_STEPS        (0.392f)
#endif // DEBUG

#ifdef ILED_IFS_50 
#define INIT_ILED_SCALES      (70)
// ILED的1刻度对应多少毫安的电流
#define ILED_IFS_STEPS        (0.195f)
#endif

// DAC的1刻度(最小量程)对应多少毫安的电流
#define DAC_IFS_STEPS         (0.125f)

#define BOD_MAX_WAVE_NUM      (18)
#define CALBRATION_LEN        (28)
#define SAFE_FREE(p)\
                  if(p!=NULL)  \
				  {            \
					  free(p); \
					  p = NULL;\
				  }
#define MIN(a,b)              ((a) < (b) ? (a) : (b))


#ifdef MATLAB_PLOT_MODE
#include "fstream"
#include "iostream"
using namespace std;
extern int8_t Plot_MatlabInit(void);
extern void Close_PlotAlgo(void);
extern void clf(void);
extern void hold_on(void);
extern void title_str(string *str);
extern void title_num(int32_t *num);
extern void subplot211(void);
extern void subplot212(void);
extern void Plot_ByMatlabOne(float* data1, uint32_t data_len, uint8_t plot_mat);
extern void setfigure(void);
extern void Plot_ByMatlabOneAndMatlabSwt(float* data1, float* data_out, uint32_t data_len, uint8_t plot_mat);
extern void clf_figure2(void);
extern void Plot_ByMatlabDoubleAndPoint(int32_t* data1, int32_t* data2,
    uint32_t data_len, int32_t* x1, int32_t* x2, uint32_t point_len);
extern void title_t(float *numf, uint8_t len);
extern void New_Figure2(void);
extern void New_Figure3(void);
extern void New_Figure4(void);
#endif

extern uint8_t          dc_rate_err_flag;

typedef struct
{
    uint8_t  rising_num;
    uint8_t  shake_flag;
    int8_t   period_value_std;
    uint8_t  signal_diff_flag;
    uint8_t  diff_peak_valley;
    //HYR.20180815.Add
    uint8_t  max_diff_num;
    //HYR.20180820.Add
    uint8_t  min_diff_num;
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

    int32_t  term[BOD_MAX_WAVE_NUM - 1];
    int32_t  start_val[BOD_MAX_WAVE_NUM];
    int32_t  peak_val[BOD_MAX_WAVE_NUM];
    int32_t  amp_val[BOD_MAX_WAVE_NUM];
    int32_t  start_point[BOD_MAX_WAVE_NUM];
    int32_t  peak_point[BOD_MAX_WAVE_NUM];

    int32_t  wave_ac[BOD_MAX_WAVE_NUM];
    int32_t  wave_dc;
    uint32_t ac_aver;
    int32_t  diff_max_min_amp;

    float    mean_amp_max_min;
    float    diff_mean_amp_rate;
}bod_wave_char_t;

typedef struct
{
    // 30
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
    // 122
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

    uint8_t signal_quality_display_degree;

    uint8_t signal_station;
    uint8_t signal_quality_flag;

    //uint8_t SPO2_control_flag;
    //uint8_t SPO2_heart_step_track;
    uint8_t afe4410_calib_judge_mode;
    uint8_t afe4410_calib_adjust_times;
    uint8_t shake_flag;
    uint8_t afe4410_transfer_time_flag;
    uint8_t afe4410_calib_mark_write_reg;
    uint8_t afe4410_calib_over;
    uint8_t afe4410_calib_fail_flag;
    uint8_t afe4410_total_time;
    uint8_t afe4410_all_well_down_flag;

    uint8_t recalib_flag;

    uint8_t r_dc_aver;
    uint8_t ir_dc_aver;
    uint8_t bo_nihe_flag;
    uint8_t bad_ori_quality_cnt;
    uint8_t bod_legal_flag;
    // 26
    uint16_t r_ac_aver;
    uint16_t ir_ac_aver;
    uint16_t ir_prefusion;//红外的灌注度
    uint16_t r_prefusion;//红光的灌注度

    uint16_t red_ctr;
    uint16_t ird_ctr;
    uint16_t green_ctr;
    // 14
    //uint16_t spo2_judge;
    float	wave_quality_degree;
    //float   red_quanlity;
    //float   ired_quanlity;
    float   bo_r_ir;
    //test
    float   bo_ac_rate;
    float   bo_dc_rate;
    float   r_ctr;//红光的转换率
    float   ir_ctr;//红外的转换率
    float   r_ir_ctr_rate;
    // 28
    //增益
    uint32_t afe4410_calbuletion_gain_red;
    uint32_t afe4410_calbuletion_gain_ird;
    uint32_t afe4410_calbuletion_gain_gre;
    //电流
    uint32_t afe4410_calbuletion_current_red;
    uint32_t afe4410_calbuletion_current_gre;
    uint32_t afe4410_calbuletion_current_ird;
    // dac
    uint8_t dac_grn;
    uint8_t dac_red;
    uint8_t dac_ird;
    //进度
    uint32_t afe4410_adjust_schedule;
    uint32_t flow_disp;
    float bo_dc_r_ir;
    float bo_ac_r_ir;
    // 13*4
    uint8_t prompt_sign_flag;   // 默认：0
    uint8_t no_real_spo2_value_flag;  //默认：1
    // 2
}bod_result_info_t;

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
    // 对应的增益的值
    uint32_t rf_register_red;
    uint32_t rf_register_ired;
    uint32_t rf_register_green;
    // 对应的电流值
    uint32_t current_register_red;
    uint32_t current_register_ired;
    uint32_t current_register_green;
    // 对应的DAC的值
    int32_t dac_register_red;
    int32_t dac_register_ired;
    int32_t dac_register_green;

    uint8_t dac_pos_red;
    uint8_t dac_pos_ired;
    uint8_t dac_pos_green;

    // 0: allow amb_dac_mode  1: not allow amb_dac
    uint8_t amb_dac_mode;

    int32_t  adjust_dac_scale;
    //test
    float r_ctr;
    float ir_ctr;
}detection_char_info_t;

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

uint8_t Blood_OxygenDetection(const int32_t *bod_red_singal, const int32_t *bod_infrared_signal, const int32_t *bod_amb_signal,
    detection_char_info_t *bod_fator_s);

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


uint8_t Blood_OxygenCalc(const int32_t *bod_red_singal, const int32_t *bod_infrared_signal,
    detection_char_info_t *boc_info_s);

#ifdef CTR_DIFF_CALIB

extern uint8_t cur_set_red;
extern uint8_t cur_set_ird;
extern uint8_t cur_set_cnt;

extern uint8_t calc_ctr_line_flag;

extern uint8_t calib_ctr_flag;

extern int32_t save_red_cur_adc[30];
extern int32_t save_ird_cur_adc[30];

extern int32_t save_red_ctr_right_adc_code;
extern int32_t save_ird_ctr_right_adc_code;
extern int32_t          fir_red_adc_code;
extern int32_t          fir_ird_adc_code;
void Calib_CtrDiff(int32_t red_av, int32_t ird_av, int32_t amb_av);


#endif // CTR_DIFF_CALIB


#endif



