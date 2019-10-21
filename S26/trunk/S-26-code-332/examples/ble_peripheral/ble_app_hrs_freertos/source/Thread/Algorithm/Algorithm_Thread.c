/*************************************Copyright(c)***************************************************
**                                     Veepoo Tech
**                  
**                               COPYRIGHT(C)  2016-2019
**--------------------------------------FileInfo----------------------------------------------------
** FileName:                Algorithm_thread.c
** Last Modify Data:        2017-08-04
** Last SYS_VERSION:        0.0.0.1
** Descriptions:                     
**---------------------------------------------------------------------------------------------------
** CreatedBy:               GT
** CreateData:              2017-06-10
** SystemVersion:             
** Description:             算法线程主要处理算法相关的任务,主要算法如下:
**                          1.血氧算法
**                          2.心率算法...
**                          
**---------------------------------------------------------------------------------------------------
** Modify By:                       
** Modify Data:
** System Version:
** Modify Description:    
******************************************************************************************************/
#include <stdlib.h>
#include <string.h>
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "algorithm_thread.h"
#include "ble_thread.h"
#include "api_flash.h"
#include "SystemTime.h"
#include "pedometer.h"
#include "signal_wear_detection_afe4410.h"
#include "Blood_OxygenDetection.h"
#include "tired_grade_detection.h"
#include "heart_detection.h"
#include "Respiratory_Rate.h"
#include "Spo2_OSAHS.h"
#include "gesture_test.h"
#include "no_weardetect.h"
#include "hrv_detection.h"
#include "AFE4900_entry.h"
#include "flash_data_type.h"
#include "cpu_task_data_type.h"
#include "SystemTime.h"
#include "data_manage.h"
#include "battery_adc.h"
#include "pedometer.h"
#include "sleep.h"
#include "gui_thread.h"
#include "afe4404_data_type.h"
#include "api_flash.h"
#include "log.h"
#include "util.h"
#include "Respiratory_Rate.h"
#include "movement_patterns.h"
#include "watch_dog.h"
#include "posture_get.h"
#include "Communication_Thread.h"
#include "afe4900_calibration.h"
#include "Blood_OxygenCalib.h"
#include "ble_disconnect_remind.h"
#include "vph_bp_24bit.h"


static algorithm_thread_resource_t  alg_res_s;

static uint8_t      alg_hour = 0;
static uint8_t      alg_minute = 0; 
static uint8_t      alg_second = 0;


static uint8_t      alg_run_cnt = 0;    // 运行状态计数器


uint32_t 	red_param = 0;
uint32_t 	ird_param = 0;
uint16_t 	calc_times_spo2 = 0;

uint16_t 	red_per = 0;
uint16_t 	ird_per = 0;
uint8_t 	calib_err = 0;
detection_char_info_t oxygen_detection = {0};

extern uint8_t  camera_delay_cnt;

/*******************************************************************************************************
** Function name:        Set_BloodOxygenEnterParam
** Descriptions:        
** input parameter:      无     
** Returned Value:       无
** Created  by:          
** Created  Datas:       
*********************************************************************************************************/
static void Set_BloodOxygenEnterParam(void)
{
    oxygen_detection.current_register_green = INIT_ILED_SCALES;
    oxygen_detection.current_register_red = INIT_ILED_SCALES;
    oxygen_detection.current_register_ired = INIT_ILED_SCALES;

    oxygen_detection.rf_register_green = 5;
    oxygen_detection.rf_register_red = 5;
    oxygen_detection.rf_register_ired = 5;

    oxygen_detection.dac_register_green = 0;
    oxygen_detection.dac_register_red = 0;
    oxygen_detection.dac_register_ired = 0;
    oxygen_detection.dac_pos_green = 1;
    oxygen_detection.dac_pos_red = 1;
    oxygen_detection.dac_pos_ired = 1;
	
#ifdef AMB_DAC_MODE
    oxygen_detection.amb_dac_mode = 1;
#else
    oxygen_detection.amb_dac_mode = 0;
#endif
}

/*******************************************************************************************************
** Function name:        Algorithm_BloodOxygenTask
** Descriptions:         算法线程--血氧任务处理函数
** input parameter:      无     
** Returned Value:       无
** Created  by:          郭涛
** Created  Datas:       2017-08-14
*********************************************************************************************************/

////TEST_Blood显示进度条卡住的问题
//uint8_t test_judge_mode[70] = 0;
//uint8_t test_cal_state[70] = 0;
//uint8_t test_judge_mode_cnt = 0;

int32_t data_ird_save[50] = {0};
bod_result_info_t* p_bod_info = NULL;
uint8_t first_time_value = 0;
extern uint8_t spo2_4410_init;
static void Algorithm_BloodOxygenTask(void)
{
    static uint8_t sta_bo_degree = 0;
    static uint8_t judge_mode_bk = 0;
    //p_bod_info = Get_OxygenInfo();
#ifdef CTR_DIFF_CALIB
    if(calc_ctr_line_flag == 1)
    {
#endif

    // 当CTR校准完成才开始
    p_bod_info = Get_OxygenInfo();
    move_index_t move_index_s = {0};
    Move_IdexGet(&move_index_s);
            
    // 血氧算法初始化
    if(p_data_manage->blood_oxygen_measure_s.init == 1)
    {       
        first_time_value = 1;
        calc_times_spo2 = 0;
        judge_mode_bk = 0;
        sta_bo_degree = 0;
        p_data_manage->blood_oxygen_measure_s.init = 0; 
        p_data_manage->blood_oxygen_measure_s.result = 0;
        
        p_data_manage->blood_oxygen_measure_s.min_start_index = 0;
        p_data_manage->blood_oxygen_measure_s.cal_flag = 0;           //校准标志置0,JSH 20171208
        p_data_manage->blood_oxygen_measure_s.cal_success = 0;        //校准成功标志置0,JSH 20171208

        //新血氧校准，进度条参数初始化
        p_data_manage->blood_oxygen_measure_s.cal_state = 1;
        p_data_manage->blood_oxygen_measure_s.cal_progress = 8;//0;
        //p_data_manage->blood_oxygen_measure_s.cnt = 0;
        //memset(&p_data_manage->blood_oxygen_measure_s.time_result,0, sizeof(p_data_manage->blood_oxygen_measure_s.time_result));

        AFE4404_LOG_PRINT("Blood oxygen Start...\n");
        ALGORIGTHM_LOG_PRINT("Blood oxygen Initing...\n");
        
        memset(Get_OxygenInfo(),0,sizeof(bod_result_info_t));
        // 血氧算法初始化
        Blood_OxygenInit();
        // 血氧校准初始化
        Set_BloodOxygenEnterParam();
        Blood_OxygenCalibInitForOne(p_bod_info);
        
        if(p_data_manage->blood_oxygen_measure_s.ox_day_time == 1)
        {   //区分是不是白天开启的   
            p_data_manage->blood_oxygen_measure_s.ox_calc_cnt = 0; // 校准完成后，计算次数，用于白天自动测量 
        }
        
        p_data_manage->blood_oxygen_measure_s.calc_cnt_test = 0;
    }
		
    p_data_manage->blood_oxygen_measure_s.calc_cnt_test++;
    
    oxygen_detection.bod_night_flag = p_data_manage->blood_oxygen_measure_s.time;
    oxygen_detection.bod_gesture = Posture_GetSecPosture();//71;//每秒的姿态，暂时没改
    oxygen_detection.bod_move_situation = move_index_s.move_5s;//5秒运动量

    calc_times_spo2++;
    // 校准入口拿出来
    if(calib_err == 0)
    {
        Blood_OxygenCalib(
                    p_bod_info,
                    DataManage_GetiRedDataPool(),
                    DataManage_GetRedPoolData(),
                    DataManage_GetAmbientDataPool(),
                    &oxygen_detection,
                    NULL);
    }
		
    // 算法入口拿出来
    Blood_OxygenDetection(
			DataManage_GetRedPoolData(), 
			DataManage_GetiRedDataPool(), 
			DataManage_GetAmbientDataPool(),
			&oxygen_detection);
		
    memcpy(data_ird_save, &p_data_manage->data_pool.ired[IRED_POOL_SIZE], 4*50);
 
    red_per = p_bod_info->r_prefusion;
    ird_per = p_bod_info->ir_prefusion;
		
    if(p_bod_info->r_ctr)
    {
        oxygen_detection.r_ctr = p_bod_info->r_ctr;
    }
    if(p_bod_info->ir_ctr)
    {
        oxygen_detection.ir_ctr = p_bod_info->ir_ctr;
    }
		
    p_data_manage->blood_oxygen_measure_s.bo_ac_r_ir = p_bod_info->bo_ac_r_ir;
    p_data_manage->blood_oxygen_measure_s.cnt = calc_times_spo2;
    p_data_manage->blood_oxygen_measure_s.r_prefusion = p_bod_info->r_prefusion;
    p_data_manage->blood_oxygen_measure_s.ir_prefusion = p_bod_info->ir_prefusion;
    
    p_data_manage->blood_oxygen_measure_s.r_ctr = (uint32_t)(10*p_bod_info->r_ctr);
    p_data_manage->blood_oxygen_measure_s.ir_ctr = (uint32_t)(10*p_bod_info->ir_ctr);
    
    p_data_manage->blood_oxygen_measure_s.bod_legal_flag = p_bod_info->bod_legal_flag;
    //校准进度条处理    
    if(p_data_manage->blood_oxygen_measure_s.cal_state == 2)
    {
        p_data_manage->blood_oxygen_measure_s.cal_state = 0;
        p_data_manage->blood_oxygen_measure_s.cal_progress = 0;
    }
    if(p_bod_info->afe4410_calib_judge_mode == 1)
    {
        p_data_manage->blood_oxygen_measure_s.cal_progress = 35;
    }
    else if(p_bod_info->afe4410_calib_judge_mode == 2)
    {
        p_data_manage->blood_oxygen_measure_s.cal_progress = 85;
    }
    else if(p_bod_info->afe4410_calib_judge_mode == 3)
    {
        p_data_manage->blood_oxygen_measure_s.cal_progress = 98;
    }
    else if(p_bod_info->afe4410_calib_judge_mode == 4 
            && judge_mode_bk != p_bod_info->afe4410_calib_judge_mode)
    {
        p_data_manage->blood_oxygen_measure_s.cal_state = 2;
        p_data_manage->blood_oxygen_measure_s.cal_progress = 100;
        //清除计数器，重新采集数据开始运行其他算法
        p_data_manage->data_pool.cache_update_cnt = 0;
    }
    
    judge_mode_bk = p_bod_info->afe4410_calib_judge_mode;  
   
    
    if(p_bod_info->bo_degree >= 40 && p_bod_info->bo_degree <= 99)
    {
        sta_bo_degree = p_bod_info->bo_degree;
    }
    else
    {
        p_bod_info->bo_degree = sta_bo_degree;
    }
    
    if(p_bod_info->afe4410_all_well_down_flag == 1)
    {
        //校准完成
        p_data_manage->blood_oxygen_measure_s.cal_success = 1;
    }
    else
    {
        //校准中
        if(p_bod_info->afe4410_calib_mark_write_reg == 1)
        {
            p_data_manage->blood_oxygen_measure_s.cal_flag = 1;
            p_data_manage->blood_oxygen_measure_s.current_green = p_bod_info->afe4410_calbuletion_current_gre;
            p_data_manage->blood_oxygen_measure_s.current_red = p_bod_info->afe4410_calbuletion_current_red;
            p_data_manage->blood_oxygen_measure_s.current_ird = p_bod_info->afe4410_calbuletion_current_ird;
            
            p_data_manage->blood_oxygen_measure_s.gain_gre = p_bod_info->afe4410_calbuletion_gain_gre;
            p_data_manage->blood_oxygen_measure_s.gain_red = p_bod_info->afe4410_calbuletion_gain_red;
            p_data_manage->blood_oxygen_measure_s.gain_ird = p_bod_info->afe4410_calbuletion_gain_ird;
            
            p_data_manage->blood_oxygen_measure_s.offdac_grn = p_bod_info->dac_grn;
            p_data_manage->blood_oxygen_measure_s.offdac_red = p_bod_info->dac_red;
            p_data_manage->blood_oxygen_measure_s.offdac_ird = p_bod_info->dac_ird;
        }
    }
    
    p_data_manage->blood_oxygen_measure_s.result = p_bod_info->bo_degree; 
    	
    p_data_manage->blood_oxygen_measure_s.ox_prompt_sign_flag = p_bod_info->prompt_sign_flag;
    p_data_manage->blood_oxygen_measure_s.ox_no_real_spo2_value_flag = p_bod_info->no_real_spo2_value_flag;
    
#ifdef CTR_DIFF_CALIB
    }
#endif

    // WJ.20190617.从Afe4900_Ppg2Isr()函数中移到这里
	if(spo2_4410_init == 1)
	{
		spo2_4410_init = 0;
		p_data_manage->blood_oxygen_measure_s.cnt = 0;
        memset(&p_data_manage->blood_oxygen_measure_s.time_result, 0, sizeof(p_data_manage->blood_oxygen_measure_s.time_result));
	}    
    
    if((p_data_manage->blood_oxygen_measure_s.time == 1)
        &&(p_data_manage->blood_oxygen_measure_s.cnt < 300))
    {
        p_data_manage->blood_oxygen_measure_s.time_result[p_data_manage->blood_oxygen_measure_s.cnt++%300] = p_data_manage->blood_oxygen_measure_s.result;
    }	

    if(p_data_manage->blood_oxygen_measure_s.ox_day_time == 1
       && p_data_manage->blood_oxygen_measure_s.cal_success == 1)
    { //校准完成
        p_data_manage->blood_oxygen_measure_s.ox_calc_cnt++;  
        
        if(p_data_manage->blood_oxygen_measure_s.ox_calc_cnt >= 30)
        {   // 计算完成30次后关闭自动测
            AlgThread_OffTimeMeasurement();
            BleThread_AFE4404DevClose();
            // 获取血氧结果
            p_data_manage->ox_cache_s = Ox_FilterAverage(p_data_manage->blood_oxygen_measure_s.time_result,
                                                         p_data_manage->blood_oxygen_measure_s.cnt);
            if(p_data_manage->ox_cache_s == 0)
            {
                p_data_manage->ox_cache_s = OX_SPECIAL_VALUES_RANDOM; // 需要app填充随机值;
            }
        }
    }
    
    //WJ.20190704.算法内部处理出错，导致校准没有完成
    if(p_data_manage->blood_oxygen_measure_s.cal_state != 0)
    {
        if(p_data_manage->blood_oxygen_measure_s.calc_cnt_test >= 10)
        {   //测试无效
            p_data_manage->blood_oxygen_measure_s.ox_prompt_sign_flag = 1;
        }
    }
    else
    {
        p_data_manage->blood_oxygen_measure_s.calc_cnt_test = 0;
    }
	
}

/*******************************************************************************************************
** Function name:        Algorithm_OsahsTask
** Descriptions:         算法线程--呼吸暂停综合征算法任务
** input parameter:      无     
** Returned Value:       无
** Created  by:          郭涛
** Created  Datas:       2017-08-04
*********************************************************************************************************/
static void Algorithm_OsahsTask(void)
{
    osahs_out_param_t   spo2_osahs_s = {0};
    osahs_entry_param_t osahs_entry_param_s = {0};
    
    uint16_t mover_arr_temp[5] = {0};
    
    
    // 输入结构体数组的头地址
    osahs_entry_param_s.spo2_data = p_data_manage->blood_oxygen_measure_s.time_result;
    //
    memcpy(mover_arr_temp, p_data_manage->pedometer.sport_count, 10);
    memcpy(&mover_arr_temp[0], &mover_arr_temp[1], 4* 2);
	osahs_entry_param_s.move_arr = mover_arr_temp;
    // 输入的数组长度
    osahs_entry_param_s.spo2_arr_len = p_data_manage->blood_oxygen_measure_s.cnt%300 + 1;
    
    Osahs_DetecInit();

    Osahs_DetecEntry(&osahs_entry_param_s, &spo2_osahs_s);
    
    // =========================呼吸暂停结果===============================================//
    // 呼吸暂停次数
    p_data_manage->oxygen_attr.breath_stop_cnt = spo2_osahs_s.osahs_times;
    // 呼吸暂停等级
    p_data_manage->oxygen_attr.breath_stop_result = spo2_osahs_s.osahs_degree;
    // 低氧时间
	if(spo2_osahs_s.osahs_cnt > 255)
	{
		spo2_osahs_s.osahs_cnt = 255;
	}
    //低氧时间
    p_data_manage->oxygen_attr.low_spo2_time = spo2_osahs_s.osahs_cnt;
    // 呼吸暂停时间
    p_data_manage->oxygen_attr.breath_stop_time = spo2_osahs_s.osahs_cnt;
    // 每分钟血氧结果
    memcpy(p_data_manage->oxygen_resut, spo2_osahs_s.spo2_one_min_aver, 5);
    
    // 血氧标志位不需要了
    p_data_manage->oxygen_attr.spo2_modif_flag = 0;

    // 呼吸暂停报警
    if(spo2_osahs_s.osahs_warning == 1 
       && p_flash_param->blood_oxygen_config_s.enable == 1
       && p_flash_param->dev_config_s.low_bo_alert_enable == 1)
    {
        GuiThread_AddEvent(CPU_EVENT_TASK_LOWOXYGENALERT);
    }
}

/*******************************************************************************************************
** Function name:        Algorithm_HRVProcessTask
** Descriptions:         算法线程--HRV算法任务
** input parameter:      无    
** Returned Value:       无
** Created  by:          郭涛
** Created  Datas:       2017-08-04
*********************************************************************************************************/
static void Algorithm_HRVProcessTask(void)
{
    const hrv_info_t*  p_hrv_info = Get_Hrv_Info();
	
    if(p_data_manage->hrv_measure_s.init == 1)
    {
        // HRV参数初始化
        p_data_manage->hrv_measure_s.init = 0;
        p_data_manage->hrv_measure_s.err_code = 0;
        p_data_manage->hrv_measure_s.cnt = 0;
        memset(p_data_manage->hrv_measure_s.result,0,sizeof(p_data_manage->hrv_measure_s.result));
		
        // 心脏负荷参数初始化
        p_data_manage->heart_load_param_s.err_code = 0;
        p_data_manage->heart_load_param_s.cnt = 0;
        memset(p_data_manage->heart_load_param_s.result,0,sizeof(p_data_manage->heart_load_param_s.result));
	    
        // 散点图心率参数初始化
        p_data_manage->scatter_heart_param_s.cnt = 0;
        p_data_manage->scatter_heart_param_s.err_code = 0;
        memset(p_data_manage->scatter_heart_param_s.heart, 0, sizeof(p_data_manage->scatter_heart_param_s.heart));
        
        ALGORIGTHM_LOG_PRINT("HRV detect Initing...\n");
        Hrv_DectInit();
    }
    
    //添加每分钟初始化一次数据缓存
    if(p_data_manage->hrv_measure_s.min_init == 1)
    {
        // HRV参数初始化
        p_data_manage->hrv_measure_s.min_init = 0;
        p_data_manage->hrv_measure_s.err_code = 0;
        p_data_manage->hrv_measure_s.cnt = 0;
        memset(p_data_manage->hrv_measure_s.result,0,sizeof(p_data_manage->hrv_measure_s.result));
		
        // 心脏负荷参数初始化
        p_data_manage->heart_load_param_s.err_code = 0;
        p_data_manage->heart_load_param_s.cnt = 0;
        memset(p_data_manage->heart_load_param_s.result,0,sizeof(p_data_manage->heart_load_param_s.result));
	    
        // 散点图心率参数初始化
        p_data_manage->scatter_heart_param_s.cnt = 0;
        p_data_manage->scatter_heart_param_s.err_code = 0;
        memset(p_data_manage->scatter_heart_param_s.heart, 0, sizeof(p_data_manage->scatter_heart_param_s.heart));
    }
    
    Hrv_Detection((int32_t*)DataManage_GetGreenCompletePool(),Vph_GetShakeFlag());
    
    // 心脏负荷参数赋值
    if((p_hrv_info->error_code == 0)&&(p_data_manage->hrv_measure_s.cnt < 60))
    {
        p_data_manage->hrv_measure_s.result[p_data_manage->hrv_measure_s.cnt%60] = p_hrv_info->hrv_value;
        p_data_manage->hrv_measure_s.cnt++;
        
        // 更新心脏负荷值
        p_data_manage->heart_load_param_s.result[p_data_manage->heart_load_param_s.cnt%60] = p_hrv_info->heart_load;
        p_data_manage->heart_load_param_s.cnt++;
    }
    else
    {
        p_data_manage->hrv_measure_s.err_code =p_hrv_info->error_code;
    }
    
    // 散点图心率值赋值
    if((p_hrv_info->heart_flag == 1)&&(p_data_manage->scatter_heart_param_s.cnt < 10))//(p_hrv_info->heart_err_code == 0)&&
    {
        p_data_manage->scatter_heart_param_s.heart[p_data_manage->scatter_heart_param_s.cnt%10] = p_hrv_info->heart_valid;
        p_data_manage->scatter_heart_param_s.cnt++;
    }
    else
    {
        p_data_manage->scatter_heart_param_s.err_code =p_hrv_info->heart_err_code;
    }
				
    ALGORIGTHM_LOG_PRINT("<HRV> hrv:%d, heart_load:%d,err_code:%d\n",p_hrv_info->hrv_value,p_hrv_info->heart_load,p_hrv_info->error_code);
}

/*******************************************************************************************************
** Function name:        Algorithm_RespirationRateTask
** Descriptions:         算法线程--呼吸率算法任务
** input parameter:      无     
** Returned Value:       无
** Created  by:          郭涛
** Created  Datas:       2017-08-04
*********************************************************************************************************/
static void Algorithm_RespirationRateTask(void)
{
    uint32_t err_code = 0;
    move_index_t move_index_s = {0};
//    const resp_info_t * p_resp_info = Get_RRInfo();
    resp_info_t p_resp_info = {0};
    
    // BR检测初始化
    if(p_data_manage->respiratory_rate_s.init == 1)
    {
        ALGORIGTHM_LOG_PRINT("Respiratory rate Initing...\n");
        RR_DetectInit();
        
        p_data_manage->respiratory_rate_s.init = 0;
        p_data_manage->respiratory_rate_s.err_code = 0;
        p_data_manage->respiratory_rate_s.cnt = 0;
        p_data_manage->respiratory_rate_s.result = 0;
        memset(&p_data_manage->respiratory_rate_s.result_sec,0,sizeof(p_data_manage->respiratory_rate_s.result_sec));
        p_data_manage->respiratory_rate_s.wave_station = 0;
        p_data_manage->respiratory_rate_s.all_flag = 0;
        memset(&p_data_manage->respiratory_rate_s.resp_second_s,0,sizeof(p_data_manage->respiratory_rate_s.resp_second_s));        
    }
    
    //添加每分钟初始化一次数据缓存
    if(p_data_manage->respiratory_rate_s.min_init == 1)
    {
        RR_DetectInit();
        p_data_manage->respiratory_rate_s.result = 0;
        p_data_manage->respiratory_rate_s.err_code = 0;
        p_data_manage->respiratory_rate_s.min_init = 0;
//        p_data_manage->respiratory_rate_s.err_code = 0;
    }
    
    Move_IdexGet(&move_index_s);
    
    err_code = Respiratory_RateDetect((const int32_t*)DataManage_GetRespGreenDataPool(),move_index_s);   
    
    Get_RRInfo(&p_resp_info);
    if(err_code == 0)
    {
        //YX.20180411.夜间测量才需要5分钟的瞬时呼吸率
        if(p_data_manage->respiratory_rate_s.time == 1)
        {
            p_data_manage->respiratory_rate_s.result_sec[p_data_manage->respiratory_rate_s.cnt%300] = p_resp_info.resp_current;
            p_data_manage->respiratory_rate_s.cnt++;
        }
       p_data_manage->respiratory_rate_s.result = p_resp_info.resp_rate;
       p_data_manage->respiratory_rate_s.resp_second_s = p_resp_info.resp_second_s;
       p_data_manage->respiratory_rate_s.wave_station = p_resp_info.wave_station;
       p_data_manage->respiratory_rate_s.all_flag = p_resp_info.all_flag;
       
    }
    else
    {
        p_data_manage->respiratory_rate_s.err_code = err_code;
    }
				
    ALGORIGTHM_LOG_PRINT("<Respiratory> result:%d,err_code: %d\n",Get_RRInfo(),err_code); 
}

/*******************************************************************************************************
** Function name:        Algorithm_WearTask
** Descriptions:         算法线程--佩戴算法任务
** input parameter:      无     
** Returned Value:       无
** Created  by:          郭涛
** Created  Datas:       2017-08-04
*********************************************************************************************************/
extern uint8_t dac_from_4410    ;        
extern uint8_t gain_from_4410   ;
extern uint8_t charge_from_4410 ;

///wear_detection_info_t  p_wear_info = {0,99.0f,1};
static void Algorithm_WearTask(void)
{
    uint8_t swd_dac_value  = 0;
    uint8_t swd_gain_value  = 0;
    uint8_t green_elec_value  = 0;
    wear_detection_info_t  p_wear_info = {0};
    
    swd_gain_value   = gain_from_4410;
    swd_dac_value    = dac_from_4410;
    green_elec_value = charge_from_4410;

    if(p_data_manage->wear_measure_s.init == 1)
    {
        Signal_WearInit();
        p_data_manage->wear_measure_s.init = 0;    
        p_data_manage->wear_measure_s.wear_status = 0; 
        p_data_manage->wear_measure_s.wear_not_pass_cnt = 0;
        p_data_manage->no_wear_measure_s.uncall_wear_cnt = 0;
    }  
    // 佩戴算法输入结构体
    afe4410_wear_input_parameter_t afe4410_wear_input_parameter_s;
		
    afe4410_wear_input_parameter_s.wear_detect_mode         =   GREEN_WEAR_DETECT                                     ;
    afe4410_wear_input_parameter_s.green_data_input_buff    =   (const int32_t*)DataManage_GetWearGreenDataPool()     ;
    afe4410_wear_input_parameter_s.green_dac_value          =   swd_dac_value                                         ;
    afe4410_wear_input_parameter_s.green_gain_value         =   swd_gain_value                                        ;
    afe4410_wear_input_parameter_s.green_elec_value         =   green_elec_value                                      ;
    afe4410_wear_input_parameter_s.amb_data_input_buff      =   (const int32_t*)DataManage_GetAmbientDataPool()       ;
    afe4410_wear_input_parameter_s.heart_detect_mode        =   0                                                     ;
    afe4410_wear_input_parameter_s.level                    =   4                                                     ;
    afe4410_wear_input_parameter_s.ifs_dac                  =   IFS_DAC                                               ;
    afe4410_wear_input_parameter_s.amb_dac_mode             =   AMB_DAC_MODE                                          ;
    afe4410_wear_input_parameter_s.ctr_threshold            =   100.0f                                                ;
   
    Signal_WearDetect( &afe4410_wear_input_parameter_s );

	Get_Wear_Detection_Info(&p_wear_info);

	if(p_wear_info.wear_station == 1)
    {
		//WJ.20180629.自动血氧测量数据采集停止
		if(p_data_manage->blood_oxygen_measure_s.time == 1)
		{
			Test_OxygenStop();
		} 
			
        if(++p_data_manage->wear_measure_s.wear_not_pass_cnt > 5) 
        {
            if(p_data_manage->blood_oxygen_measure_s.ox_day_time == 1)
            {   //佩戴不通过填充特殊值
                p_data_manage->ox_cache_s = OX_SPECIAL_VALUES_WEAR; // 需要app填充随机值
            }            
            
            p_data_manage->wear_measure_s.wear_status = 1;
            p_data_manage->no_wear_measure_s.last_wear_status = 1;
            p_data_manage->oxygen_attr.wear = 1;
		
            p_data_manage->heart_measure_s.btn = 0;
            p_data_manage->heart_measure_s.day_time = 0;
            p_data_manage->heart_measure_s.night_time = 0;
            p_data_manage->blood_oxygen_measure_s.btn = 0;
            p_data_manage->blood_oxygen_measure_s.time = 0;
            p_data_manage->hrv_measure_s.time = 0;
            p_data_manage->respiratory_rate_s.time = 0;
            
            p_data_manage->bp_measure_s.bp_btn = 0;            
            p_data_manage->bp_measure_s.bp_day_time = 0;
            p_data_manage->bp_measure_s.bp_night_time = 0;
            
            // 如果佩戴不通过,则需要关掉AFE4404并且初始化所有的测量出生状态
            BleThread_AFE4404DevClose();
        }
    }
    else
    {
        p_data_manage->wear_measure_s.wear_status = 0;
        p_data_manage->wear_measure_s.wear_not_pass_cnt = 0;  
    }
}

/*******************************************************************************************************
** Function name:        Algorithm_NoWearTask
** Descriptions:         算法线程--脱手检测算法任务
**                       由于脱手检测算法在每五分钟执行一次,但是佩戴检测算法执行完成需要六秒才能完成,
**                       需要注意状态。
** input parameter:      无     
** Returned Value:       无
** Created  by:          郭涛
** Created  Date:        2017-08-04
*********************************************************************************************************/
static void Algorithm_NoWearTask(void)
{
    sport_history_t  m_sport_history_s = {0};
    
    if(p_data_manage->no_wear_measure_s.init == 1)
    {
        p_data_manage->no_wear_measure_s.init = 0;        
        Vnwd_NoWearInit();
    }
    
    // 获取运动量信息
    p_data_manage->no_wear_measure_s.amount = Array_ShortSum(p_data_manage->pedometer.sport_count, 5);
    p_data_manage->no_wear_measure_s.step   = Array_ShortSum(p_data_manage->pedometer.step_count, 5);
    m_sport_history_s = Vnwd_HistoryMomentum(p_data_manage->no_wear_measure_s.amount,p_data_manage->no_wear_measure_s.step);
    
    // 脱手检测函数
    if(p_flash_param->dev_config_s.led_gear == 0)
    {
//        Vnwd_NoWearDect(m_sport_history_s, p_data_manage->no_wear_measure_s.last_wear_status, Gesture_GetMode16bit(), 8);
        Vnwd_NoWearDect(m_sport_history_s, p_data_manage->no_wear_measure_s.last_wear_status, p_data_manage->no_wear_measure_s.gesture, 11);
    }
    else
    {
//        Vnwd_NoWearDect(m_sport_history_s, 0, Gesture_GetMode16bit(), 8);
        Vnwd_NoWearDect(m_sport_history_s, 0, p_data_manage->no_wear_measure_s.gesture, 11);
    }
    
    // !注意:这里需要初始化上一次的佩戴状态 Modify by GT 2017-10-31
//    if(p_data_manage->no_wear_measure_s.uncall_wear_cnt++ >= 2)
    if(p_data_manage->no_wear_measure_s.uncall_wear_cnt == 1)
    {
        p_data_manage->no_wear_measure_s.uncall_wear_cnt = 0;
        p_data_manage->no_wear_measure_s.last_wear_status = 0;
    }
    
    // 获取结果
    p_data_manage->no_wear_measure_s.no_wear_result = Get_NoWearStation();
}

/*******************************************************************************************************
** Function name:        Algorithm_HeartTask
** Descriptions:         算法线程--心率算法任务
** input parameter:      无     
** Returned Value:       无
** Created  by:          郭涛
** Created  Datas:       2017-08-04
*********************************************************************************************************/
//bod_4404_adjust_t get_dac_gre;
//uint8_t dac_gre = 0;
static void Algorithm_HeartTask(void)
{
    static uint8_t sta_heart_pulse = 0;
    move_index_t  move_index_s = {0};
    heart_info_t  m_heart_info = {0};
    
//    dac_gre = get_dac_gre.dac_grn & 0x0F;
//    afe4404_ReadParamReg(&gain_write_check,&elec_write_check,&dac_write_check);
    // 心率算法初始化
    if(p_data_manage->heart_measure_s.init == 1)
    {
        sta_heart_pulse = 0;
        p_data_manage->heart_measure_s.init = 0;
        p_data_manage->heart_measure_s.real_heart = 0;
        p_data_manage->heart_measure_s.valid_heart_cnt = 0;
        p_data_manage->heart_measure_s.time_measure_cnt = 0;
        memset(&p_data_manage->heart_measure_s.heart_cache,0,sizeof(p_data_manage->heart_measure_s.heart_cache));
	    
        ALGORIGTHM_LOG_PRINT("Heart Initing...\n");					
        Heart_Init(); 
    }
    
    //添加每分钟初始化一次数据缓存
    if(p_data_manage->heart_measure_s.min_init == 1)
    {
	    // 防止心率连续异常---JSH-20180627
        Heart_Init();
        p_data_manage->heart_measure_s.min_init = 0;
        p_data_manage->heart_measure_s.real_heart = 0;
        p_data_manage->heart_measure_s.valid_heart_cnt = 0;
        p_data_manage->heart_measure_s.time_measure_cnt = 0;
        memset(&p_data_manage->heart_measure_s.heart_cache,0,sizeof(p_data_manage->heart_measure_s.heart_cache));
    }
    
    Move_IdexGet(&move_index_s);
    
    // 这里需要区分测试模式
    if((p_data_manage->heart_measure_s.btn == 1)||(p_data_manage->heart_measure_s.app == 1) ||(p_data_manage->heart_measure_s.day_time == 1)|| (Movement_GetState() == MOVEMENT_STATE_RUN))
    {
        Heart_Detection((const int32_t*)DataManage_GetGreenCompletePool(),PLUSE_SPORTS_MODE , move_index_s);  
    }        
    else
    {
        Heart_Detection((const int32_t*)DataManage_GetGreenCompletePool(),PLUSE_SPORTS_MODE , move_index_s);  
    }
    
    Get_HeartInfo(&m_heart_info);
    if(m_heart_info.heart_pulse == 0)
    {
        p_data_manage->heart_measure_s.real_heart = sta_heart_pulse;
    }
    else
    {
        p_data_manage->heart_measure_s.real_heart = m_heart_info.heart_pulse;
        sta_heart_pulse = m_heart_info.heart_pulse;
    }

    // 如果启动定时心率测量
    if((p_data_manage->heart_measure_s.day_time == 1)||(p_data_manage->heart_measure_s.night_time == 1))
    {
        p_data_manage->heart_measure_s.time_measure_cnt++;
        
        // 计算心率平均值(去掉最大值/最小值)
        if((m_heart_info.heart_pulse >= 30)&&(m_heart_info.heart_pulse <= 180))
        {
            p_data_manage->heart_measure_s.heart_cache[p_data_manage->heart_measure_s.valid_heart_cnt%10]  = m_heart_info.heart_pulse;
            //YX.20180104.解决夜间心率偏低问题
            p_data_manage->heart_measure_s.valid_heart_cnt++;
        
            if(p_data_manage->heart_measure_s.valid_heart_cnt >= 6)
            {
                 // 如果心率过高取【5-10】次心率平均值,则需要提醒用户
                 p_data_manage->heart_measure_s.time_heart = Heart_FilterAverage(p_data_manage->heart_measure_s.heart_cache,p_data_manage->heart_measure_s.valid_heart_cnt);
                 if((p_data_manage->heart_measure_s.time_heart >= p_flash_param->heart_alert_s.high)
                    &&(p_flash_param->heart_alert_s.enable == 1)
                    && !(Movement_GetState() == MOVEMENT_STATE_RUN))
                 {
                     GuiThread_AddEvent(CPU_EVENT_TASK_HEARTALERT);
                 }
            }
        }
        
        if((p_data_manage->heart_measure_s.time_measure_cnt >= 30) || (p_data_manage->heart_measure_s.valid_heart_cnt >= 6))
        {  
            // 添加关闭退出条件
            if(p_data_manage->heart_measure_s.day_time == 1 && !(Movement_GetState() == MOVEMENT_STATE_RUN)
               && p_data_manage->heart_measure_s.btn == 0 && p_data_manage->heart_measure_s.app == 0
               && p_data_manage->blood_oxygen_measure_s.btn == 0 && p_data_manage->blood_oxygen_measure_s.app == 0
			   && p_data_manage->respiratory_rate_s.app == 0    
               && p_data_manage->bp_measure_s.bp_btn == 0 && p_data_manage->bp_measure_s.bp_app == 0               
               && (p_data_manage->bp_measure_s.bp_day_time == 0))
            {
//                AFE4404_LOG_PRINT(" Time Heart Measure,Time Out Close!\n");
                BleThread_AFE4404DevClose();	 
            }

            p_data_manage->heart_measure_s.day_time = 0;
            p_data_manage->heart_measure_s.night_time = 0;        
        }
    }
    //ALGORIGTHM_LOG_PRINT("<Heart> result:%d\n",m_heart_info.heart_pulse); 
}

/*******************************************************************************************************
** Function name:        Algorithm_PedometerTask
** Descriptions:         算法线程--记步算法任务
** input parameter:      void     
** Returned Value:       void
** Created  by:          GT
** Created  Datas:       2017-08-04
*********************************************************************************************************/
static void Algorithm_PedometerTask(void)
{
    int16_t pedometer_data[3] = {0};
    static uint8_t pedometer_counter = 0;
    ped_param_set_t pp_param_s = {10, 2};
    
    while(p_data_manage->acc.pedometer_data_len > 0)
    {
        DataManage_ProcessPedometerData(&pedometer_data[0],&pedometer_data[1],&pedometer_data[2]);
        Pedometer_Process(&pedometer_data[0], pedometer_counter++, 8, pp_param_s, 0);
        
        // 注意:pedometer_counter是算法要求输入累加值的.
        if(pedometer_counter >= 25)
        {
            pedometer_counter = 0; 
        }
    }
}

/*******************************************************************************************************
** Function name:        Algorithm_GestureTask
** Descriptions:         算法线程--姿态算法任务
** input parameter:      无    
** Returned Value:       void
** Created  by:          GT
** Created  Datas:       2017-08-04
*********************************************************************************************************/
static void Algorithm_GestureTask(void)
{
    uint8_t gesture_mode = 0;
    uint16_t turnwrist_min_time = 0x00;
    uint16_t turnwrist_max_time = 0x00;
    uint16_t current_time = 0x00;
    static bool turnwrist_start = false;
    
    acc_data_param_t* p_acc_data = (acc_data_param_t*)&p_data_manage->acc;
    for(uint8_t len = p_acc_data->gesture_data_len; len > 4; len -= 5)
    {
        DataManage_ProcessGestureData();
        
        // !注意:此处因为涉及到翻腕灵敏度的参数问题,所以需要对异常值做处理
        if((p_flash_param->turnwrist_s.sensitivity == 0)||(p_flash_param->turnwrist_s.sensitivity >= 10))
        {
            p_flash_param->turnwrist_s.sensitivity = 5;
        }
        
        Gesture_Detection16bit(p_acc_data->y_gesture_data,p_acc_data->x_gesture_data, 
                               p_acc_data->z_gesture_data, p_acc_data->v_gesture_data, 
                               p_flash_param->turnwrist_s.sensitivity);
        
        gesture_mode = Gesture_GetMode16bit();
        if((gesture_mode == 5 || gesture_mode == 6))
        { 
            if(0x01 == p_flash_param->turnwrist_s.enable)//如果开了，全天都可以翻腕
            {
                current_time = clock->hour*60+clock->minutes;
                turnwrist_min_time = p_flash_param->turnwrist_s.s_hour*60+p_flash_param->turnwrist_s.s_minute;
                turnwrist_max_time = p_flash_param->turnwrist_s.e_hour*60+p_flash_param->turnwrist_s.e_minute;

                if(turnwrist_min_time <= turnwrist_max_time)
                {
                    if((turnwrist_min_time <= current_time)&&(current_time <= turnwrist_max_time))
                    {
                        turnwrist_start = true;
                        GuiThread_AddEvent(CPU_EVENT_TASK_TURNWRIST_START);
                    }
                }
                else
                {
                    if((current_time >= turnwrist_min_time)||(current_time <= turnwrist_max_time))
                    {
                        turnwrist_start = true;
                        GuiThread_AddEvent(CPU_EVENT_TASK_TURNWRIST_START);
                    }
                }
            }
            if(GuiThread_GetEventType() == CPU_EVENT_TASK_CAMERA)
            {
                p_data_manage->turnwrist_camera_state = 1;
            }
        }
        
        //抬手亮屏关闭
        if((Gesture_GetCloseScreenFlag16bit())&&(turnwrist_start == true))
        {
            turnwrist_start = false;
            GuiThread_AddEvent(CPU_EVENT_TASK_TURNWRIST_STOP);
        }
    } 
}

/*******************************************************************************************************
** Function name:        Algorithm_FatigueTask
** Descriptions:         算法线程--疲劳度算法任务
** input parameter:      无     
** Returned Value:       void
** Created  by:          GT
** Created  Datas:       2017-08-04
*********************************************************************************************************/
static void Algorithm_FatigueTask(void)
{
    if(p_data_manage->fatigue_measure_s.init == 1)
    {
        p_data_manage->fatigue_measure_s.init = 0;
        Tired_DectInit();
    }
    
    Tired_GradeDetection(DataManage_GetTiredGreenPool());     
}

/*******************************************************************************************************
** Function name:        Algorithm_SleepTask
** Descriptions:         算法线程--睡眠算法任务
** input parameter:      无     
** Returned Value:       void
** Created  by:          GT
** Created  Datas:       2017-08-04
*********************************************************************************************************/
static void Algorithm_SleepTask(void)
{
    // 如果正常使用,则调用睡眠算法
    if(Battery_Get_Status() == BATTERY_POWER_NORMAL && Battery_Get_Grade() != 0)
    {
        Sleep_NormalProcess();
    }
    else
    {
        // 防止充电过程中有残留睡眠数据
        Sleep_LowBateeryProcess();
    }
}

/*******************************************************************************************************
** Function name:        Algorithm_SleepTraversingTask
** Descriptions:         算法线程--睡眠遍历算法任务
** input parameter:      无     
** Returned Value:       void
** Created  by:          GT
** Created  Datas:       2017-08-04
*********************************************************************************************************/
static void Algorithm_SleepTraversingTask(void)
{
    Sleep_TraversingOperation();
}

/*******************************************************************************************************
** Function name:        Algorithm_SedentaryTask
** Descriptions:         算法线程--久坐算法任务
** input parameter:      void     
** Returned Value:       void
** Created  by:          GT
** Created  Datas:       2017-08-04
*********************************************************************************************************/
static void Algorithm_SedentaryTask(void)
{
    bool bRet = true;
    //static uint16_t continue_cnt = 0;
    static sedentary_config_t sedentary_config_bk = {0};
    const vph_sleep_info_t* p_sleep_info = Vs_GetSleepingInfo();
    uint32_t current_time = clock->hour*100 + clock->minutes;
    uint32_t sedentary_start = p_flash_param->sedentary_config_s.start_hour*100 + p_flash_param->sedentary_config_s.start_minute;
    uint32_t sedentary_end   = p_flash_param->sedentary_config_s.end_hour*100 + p_flash_param->sedentary_config_s.end_minute;
        
    if(sedentary_config_bk.start_hour != p_flash_param->sedentary_config_s.start_hour
       || sedentary_config_bk.start_minute != p_flash_param->sedentary_config_s.start_minute
       || sedentary_config_bk.end_hour != p_flash_param->sedentary_config_s.end_hour
       || sedentary_config_bk.end_minute != p_flash_param->sedentary_config_s.end_minute
       || sedentary_config_bk.internal != p_flash_param->sedentary_config_s.internal)
    {
        sedentary_config_bk = p_flash_param->sedentary_config_s;
        //continue_cnt = 0;
        //YX.20180418.修复关机久坐计数不清零问题
        p_data_manage->sedentary_cnt = 0;
        bRet = false;
    }
    else
    {
        bRet = (p_flash_param->sedentary_config_s.enable == 1);
        bRet = bRet&&(sedentary_start <= current_time)&&(current_time <= sedentary_end);
        bRet = bRet&&(Array_ShortSum(p_data_manage->pedometer.step_count, 5) < 13);
        // !注意:为避免久坐误提醒,此处需要添加时间限制 Add by GT 2017-10-23
        bRet = (bRet)&&(clock->hour >= 8)&&(clock->hour < 18);
        // !注意:新增如果在睡眠中,则不能提醒久坐 Add by GT 2017-10-30
        bRet = (bRet)&&(p_sleep_info->sleep_flag != 1);
        //不充电
        bRet = (bRet)&&(Battery_Get_Status() == BATTERY_POWER_NORMAL);
        //佩戴
        bRet = (bRet)&&(Get_NoWearStation() < 2 || Get_NoWearStation() == 5);
    }
    
    if(bRet)
    {
        //YX.20180418.修复关机久坐计数不清零问题
        //continue_cnt++;
        p_data_manage->sedentary_cnt++;
        //if(continue_cnt >= p_flash_param->sedentary_config_s.internal)
        if(p_data_manage->sedentary_cnt >= p_flash_param->sedentary_config_s.internal)
        {
            //continue_cnt = 0;
            p_data_manage->sedentary_cnt = 0;
            GuiThread_AddEvent(CPU_EVENT_TASK_SIT);
        }
    }
    else
    {
        //continue_cnt = 0;
        p_data_manage->sedentary_cnt = 0;
    }
}

/*******************************************************************************************************
** Function name:        Algorithm_OledBrightnessAdjustTask
** Descriptions:         算法线程--OLED屏亮度实时调节函数
** input parameter:      void     
** Returned Value:       void
** Created  by:          GT
** Created  Datas:       2017-08-04
*********************************************************************************************************/
//static void Algorithm_OledBrightnessAdjustTask(void)
void Algorithm_OledBrightnessAdjustTask(void)
{
    uint16_t min_time = 0x00;
    uint16_t max_time = 0x00;
    uint16_t current_time = 0x00;
    
    current_time = clock->hour*60 + clock->minutes;
    min_time = p_flash_param->brightness_s.start_hour*60 + p_flash_param->brightness_s.start_minute;
    max_time = p_flash_param->brightness_s.end_hour*60 + p_flash_param->brightness_s.end_minute;

    // 这里需要对亮度设定进行判断
    if(min_time <= max_time)
    {
        if((min_time <= current_time)&&(current_time <= max_time))
        {
            DM_Update_Brightness(p_flash_param->brightness_s.valid_bright);
        }
        else
        {
            DM_Update_Brightness(p_flash_param->brightness_s.other_bright);
        }
    }
    else
    {
        //YX.20180111.亮度显示与时间对不上的问题
        if((current_time >= min_time)||(current_time <= max_time))
        {
            DM_Update_Brightness(p_flash_param->brightness_s.valid_bright);
//            DM_Update_Brightness(p_flash_param->brightness_s.other_bright);
        }
        else
        {
//            DM_Update_Brightness(p_flash_param->brightness_s.valid_bright);
            DM_Update_Brightness(p_flash_param->brightness_s.other_bright);
        }
    }
}

/*******************************************************************************************************
** Function name:        Algorithm_AlarmMonitorTask
** Descriptions:         算法线程--闹铃监测任务
** input parameter:      无     
** Returned Value:       void
** Created  by:          GT
** Created  Datas:       2017-08-04
*********************************************************************************************************/
static void Algorithm_AlarmMonitorTask(void)
{
    uint8_t repeat_move_bit = 0;
    bool alarm_is_arrive = false;
//    alarm_time_config_t alarm_time_s = {0};
    const alarm_time_config_t* p_current_alarm = NULL;
    
    // 先遍历单次闹钟
    for(uint8_t index = 0; index < 20; index++)
    {
        p_current_alarm = &p_flash_param->alarm_time_s[index];
        if((clock->minutes == p_current_alarm->minute)&&(clock->hour == p_current_alarm->hour))
        {
             //1、如果是单次闹铃,则需要关闭
             if((1 == p_current_alarm->state)&&(0x00 == p_current_alarm->repeat))
             {
                  //这里需要比较日期
                  if((clock->year == p_current_alarm->year)&&(clock->month == p_current_alarm->month)&&
                     (clock->day == p_current_alarm->day))
                  {
                     alarm_is_arrive = true;
                     p_data_manage->alarm_scence = p_current_alarm->scence;
                     break;
                  }
              }
        }
    }
        
    // 如果有单次闹钟,则不再遍历重复闹钟
    if(!alarm_is_arrive)
    {
        for(uint8_t index = 0; index < 20; index++)
        {
			 p_current_alarm = &p_flash_param->alarm_time_s[index];
			if((clock->minutes == p_current_alarm->minute)&&(clock->hour == p_current_alarm->hour))
			{
				 //2、判断是否是重复闹铃
				 if((p_current_alarm->state == 1)&&(p_current_alarm->repeat != 0x00))
				 {
					 if(clock->week == 0)
					 {
						 repeat_move_bit = 6;
					 }
					 else
					 {
						 repeat_move_bit = clock->week-1;
					 }
					
					 if((p_current_alarm->repeat>>repeat_move_bit)&0x01)
					 {
						 //闹铃触发
						 alarm_is_arrive = true;
						 p_data_manage->alarm_scence = p_current_alarm->scence;
						 break;
					 }
				}
			}
        }
    }
    
    if(alarm_is_arrive)
    {
        GuiThread_AddEvent(CPU_EVENT_TASK_ALARM);
    } 
}

/****************************************************************
** Function name:           Alarm_IsOutTargetTime
** Descriptions:            单次闹钟时间是否已过,用于APP设置闹钟校验，重启自检
** input parameters:        无
** output parameters:       无
** Returned value:          返回0:不是单次闹钟或者闹钟没有失效; 1:单次闹钟失效
** Created by:              WJ              
** Created Date:            2017-05-15
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/
static bool Alarm_IsOutTargetTime(const alarm_time_config_t *p_alarm)
{  
    if(p_alarm->repeat != 0)
    {
        return 0;
    }
    if(p_alarm->year < clock->year || (p_alarm->year == clock->year &&p_alarm->month < clock->month)
      || (p_alarm->year == clock->year && p_alarm->month == clock->month && p_alarm->day < clock->day )
      || (p_alarm->year == clock->year && p_alarm->month == clock->month && p_alarm->day == clock->day
      && (p_alarm->hour*60+p_alarm->minute <= clock->hour * 60 + clock->minutes)))
    {
        return 1;
    }
    
	return 0;
}

/****************************************************************
** Function name:           Algorithm_AlarmValidCheck
** Descriptions:            自检单次闹钟，过时的需要重置状态
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-05-15
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------*/
void Algorithm_AlarmValidCheck(void)
{
    uint8_t index = 0;
	
	for(index = 0; index < 20; index++)
	{   
        if(p_flash_param->alarm_time_s[index].valid != 1) continue;
        
        //1、如果是单次闹铃,则需要关闭
        if((1 == p_flash_param->alarm_time_s[index].state)&&(0x00 == p_flash_param->alarm_time_s[index].repeat))
        {
            if(Alarm_IsOutTargetTime(&p_flash_param->alarm_time_s[index]))
            {
                p_flash_param->alarm_time_s[index].state = false;
            }
        }
    }  
}


/****************************************************************
** Function name:           Alarm_GetValidConfigNum
** Descriptions:            获取闹钟的有效个数
** input parameters:        无
** output parameters:       无
** Returned value:          返回:有效闹钟个数
** Created by:              WJ              
** Created Date:            2017-05-15
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/
uint8_t Alarm_GetValidConfigNum(void)
{
    uint8_t index = 0;
	uint8_t total_num = 0;
    
	for(index = 0; index < 20; index++)
	{    
        if(p_flash_param->alarm_time_s[index].valid)
        {
            total_num++;
        }
    }
    
    return total_num;
//    return 0;
}

/*******************************************************************************************************
** Function name:        Algorithm_StepSportTraceTask
** Descriptions:         算法线程--记步运动量每分钟计算任务
** input parameter:      无     
** Returned Value:       void
** Created  by:          GT
** Created  Datas:       2017-08-04
*********************************************************************************************************/
static void Algorithm_StepSportTraceTask(void)
{
    sport_information_info_t gsii_sport_info_s = {0x00, };
    
    Get_SportInformationInfo(&gsii_sport_info_s);

    p_data_manage->pedometer.index = (p_data_manage->pedometer.index + 1)%5;
    
    // 更新记步数据
    if(gsii_sport_info_s.posi_steps > p_data_manage->pedometer.step_history_count)
    {
        p_data_manage->pedometer.step_count[p_data_manage->pedometer.index] = (uint16_t)(gsii_sport_info_s.posi_steps - p_data_manage->pedometer.step_history_count);
    }
    else
    {
        p_data_manage->pedometer.step_count[p_data_manage->pedometer.index] = 0;
    }
    
    // 更新距离数据
    if(gsii_sport_info_s.posi_distance > p_data_manage->pedometer.distance_history_count)
    {
        p_data_manage->pedometer.distance_count[p_data_manage->pedometer.index] = (uint16_t)(gsii_sport_info_s.posi_distance - p_data_manage->pedometer.distance_history_count);
    }
    else
    {
        p_data_manage->pedometer.distance_count[p_data_manage->pedometer.index] = 0;
    }
    
    // 更新卡路里数据
    if(gsii_sport_info_s.posi_cal > p_data_manage->pedometer.calories_history_count)
    {
        p_data_manage->pedometer.calories_count[p_data_manage->pedometer.index] = (uint16_t)(gsii_sport_info_s.posi_cal - p_data_manage->pedometer.calories_history_count);
        p_data_manage->pedometer.calories_count[p_data_manage->pedometer.index] /= 100;
    }
    else
    {
        p_data_manage->pedometer.calories_count[p_data_manage->pedometer.index] = 0;
    }
    
    // 更新运动量数据
    if(Work_MeasureGet() >= p_data_manage->pedometer.sport_history_count)
    {
        p_data_manage->pedometer.sport_count[p_data_manage->pedometer.index] = (uint16_t)((Work_MeasureGet() - p_data_manage->pedometer.sport_history_count)>>12);
    }
    else
    {
        p_data_manage->pedometer.sport_count[p_data_manage->pedometer.index] = 0;
    }
    
    // 更新记步运动量历史数据
    p_data_manage->pedometer.step_history_count = gsii_sport_info_s.posi_steps;
    p_data_manage->pedometer.distance_history_count = gsii_sport_info_s.posi_distance;
    p_data_manage->pedometer.calories_history_count = (gsii_sport_info_s.posi_cal / 100) * 100;
    p_data_manage->pedometer.sport_history_count = Work_MeasureGet();
    
    // !注意:这里需要注意short类型赋值数据给char类型 Modify by GT 2017-10-28
    if(p_data_manage->pedometer.step_count[p_data_manage->pedometer.index] > 255)
    {
        p_data_manage->pedometer.last_minute_step = 255;
    }
    else
    {
        p_data_manage->pedometer.last_minute_step = p_data_manage->pedometer.step_count[p_data_manage->pedometer.index];
    }
    
    if(p_data_manage->pedometer.sport_count[p_data_manage->pedometer.index] > 255)
    {
       p_data_manage->pedometer.last_minute_sport = 255;
    }
    else
    {
        p_data_manage->pedometer.last_minute_sport = p_data_manage->pedometer.sport_count[p_data_manage->pedometer.index];
    }
}

static void Algorithm_HourglassTask(void)
{
    // 倒计时提醒功能
    if(Battery_Get_Status() == BATTERY_POWER_NORMAL)
    {
        if((p_flash_param->hourglass_s.app_trigger == 1)||(p_flash_param->hourglass_s.btn_trigger  == 1))
        {
            if(p_flash_param->hourglass_s.run_remain_time >= 1)
            {
                p_flash_param->hourglass_s.run_remain_time--;
            }
            else
            {
                // 如果当前正处于倒计时显示界面,则需要清除掉提醒界面
                GuiThread_ClearEvent(CPU_EVENT_TASK_HOURGLASS_HINT);
                
                GuiThread_AddEvent(CPU_EVENT_TASK_HOURGLASS_TIMEOUT);
                p_flash_param->hourglass_s.btn_trigger = 0;
                p_flash_param->hourglass_s.app_trigger = 0;
                p_flash_param->hourglass_s.app_send = 0;
            }
        }
    }
    else
    {
        if((p_flash_param->hourglass_s.app_trigger == 1)||(p_flash_param->hourglass_s.btn_trigger  == 1))
        {
            p_flash_param->hourglass_s.app_trigger = 0;
            p_flash_param->hourglass_s.btn_trigger = 0;
            p_flash_param->hourglass_s.run_remain_time = 0;
        }
    }
}

void Algorithm_Spo2CalibTask(uint8_t mode, uint8_t set_red_cur, uint8_t set_ird_cur)
{
    static bod_result_info_t afe4404param_s;
        
    afe4404param_s = *Get_OxygenInfo();
    //memcpy((void *)&afe4404param_s, (void *)Get_OxygenInfo(), sizeof(bod_result_info_t));
    
    if(mode == 1)
	{
		// 5点1次校准模式
		afe4404param_s.afe4410_calbuletion_current_red = set_red_cur;
		afe4404param_s.afe4410_calbuletion_current_ird = set_ird_cur;
		afe4404param_s.afe4410_calbuletion_current_gre = 0;
		
		afe4404param_s.afe4410_calbuletion_gain_red = 1;
		afe4404param_s.afe4410_calbuletion_gain_ird = 1;
		afe4404param_s.afe4410_calbuletion_gain_gre = 1;
		
		afe4404param_s.dac_red = 0;
		afe4404param_s.dac_ird = 0;
		afe4404param_s.dac_grn = 0;
	}
	
    Afe4404Param_ToAfe4410(&afe4404param_s);
	
	AFE4410_ElecRead(
                    &oxygen_detection.current_register_green,
                    &oxygen_detection.current_register_red,
                    &oxygen_detection.current_register_ired);
	
	AFE4410_RfRead(
                &oxygen_detection.rf_register_green,
                &oxygen_detection.rf_register_red,
                &oxygen_detection.rf_register_ired);
    
	AFE4410_DacRead(
            &oxygen_detection.dac_register_green,
            &oxygen_detection.dac_register_red,
            &oxygen_detection.dac_register_ired,
            &oxygen_detection.dac_pos_green,
            &oxygen_detection.dac_pos_red,
            &oxygen_detection.dac_pos_ired);
}


// 1秒事件处理
static void Algorithm_OneScondsTask(void)
{
    // 倒计时
    Algorithm_HourglassTask();
    if((alg_second == 0)&&(p_data_manage->device_trace_param_s.sign_on == 1))
    {
        // 闹铃任务 	    
        Algorithm_AlarmMonitorTask();
        // Oled亮度调节任务 
        Algorithm_OledBrightnessAdjustTask();
        // 记步运动量跟踪计算任务 
        Algorithm_StepSportTraceTask();
    
        // 久坐提醒任务
        Algorithm_SedentaryTask();
        
        if(alg_minute % 5 == 0)
        {
            // 睡眠算法任务
            xSemaphoreGive(alg_res_s.m_sleep_sem);
            xSemaphoreGive(alg_res_s.m_task_count); 
        }
    }

    if(Battery_Get_Status() == BATTERY_POWER_NORMAL)
    {
        // !注意:脱手检测算法在xx:xx:20调用.
        if((alg_minute%5 == 0 && alg_second == 20)
          && (p_data_manage->device_trace_param_s.sign_on == 1))
        {
            // 脱手检测算法任务
            xSemaphoreGive(alg_res_s.m_no_wear_sem);
            xSemaphoreGive(alg_res_s.m_task_count);		
            if(alg_minute % 10 == 5 || alg_hour < 8)
            {
                p_data_manage->no_wear_measure_s.uncall_wear_cnt = 1;
            }
        }
        if(p_data_manage->device_trace_param_s.sign_on == 1)
        {
            Posture_Statistical();     //YX.20171229.添加姿态新的统计方式
        }
    }
    
    Posture_Statistical();     
        
    // 运动模式
    Movement_Monitor();
    
    //电池电量
    Algorithm_BatteryGradeTraceTask();
    
    if(p_data_manage->device_trace_param_s.sign_on == 1)
    {
        //开机状态，达到计步值，每天提醒一次
        if((DataManage_GetCurrentStepCnt() >= p_flash_param->sport_target)
          &&(p_flash_param->Remind_flag == 0))
        {
            p_flash_param->Remind_flag = 1;
            BleThread_AsynUpdateAppConfig();
           //add event 
           GuiThread_AddEvent(CPU_EVENT_TASK_SPORT_TARGET);   
        }  
        /*
        //YX.20180408.蓝牙断连提醒,开机状态下，提醒开关开启   
        Remind_BleDisconnectMonitor(p_flash_param->dev_config_s.ble_disconnect_enable, 1, 
                                    p_data_manage->device_trace_param_s.ble_connect);
        */
        camera_delay_cnt++;
    }
}

// 血压测量模式设置
void Algorithm_BpSetModeParam(void)
{
    if(p_flash_param->bp_s.mode == 1)
    {
        p_data_manage->bp_measure_s.bp_test_mode = 1;
    }
    else
    {
        p_data_manage->bp_measure_s.bp_test_mode = 0;           
    }
    p_data_manage->bp_measure_s.bp_set_sp = p_flash_param->bp_s.private_high;
    p_data_manage->bp_measure_s.bp_set_dp = p_flash_param->bp_s.private_low;     
}

// 血压测量
static void Algorithm_BpTask(void)
{
    test_mode_t     test_mode_s = {0};    
    move_index_t    move_index_s = {0};
    bp_result_t     bp_gresult_s = {0}; 
	
    if(p_data_manage->bp_measure_s.bp_init == 1)
    { 
        p_data_manage->bp_measure_s.bp_init = 0;
        p_data_manage->bp_measure_s.bp_calc_prg = 0;        // 计算(进度)次数
        p_data_manage->bp_measure_s.bp_flag = 0;            // 血压计算结果状态
        p_data_manage->bp_measure_s.bp_sp_value = 0;
        p_data_manage->bp_measure_s.bp_dp_value = 0;
        
        BP_DetectionInit();
        
        if(p_data_manage->bp_measure_s.bp_app == 0 && p_data_manage->bp_measure_s.bp_btn == 0)
        {
            Algorithm_BpSetModeParam(); 
        }
    }
    if( p_data_manage->bp_measure_s.bp_calc_prg >= 25)
    {
        //close
        if(p_data_manage->bp_measure_s.bp_day_time == 1 && !(Movement_GetState() == MOVEMENT_STATE_RUN)
           && p_data_manage->heart_measure_s.btn == 0 && p_data_manage->heart_measure_s.app == 0
           && p_data_manage->blood_oxygen_measure_s.btn == 0 && p_data_manage->blood_oxygen_measure_s.app == 0
           && p_data_manage->respiratory_rate_s.app == 0     
           && p_data_manage->bp_measure_s.bp_btn == 0 && p_data_manage->bp_measure_s.bp_app == 0          
           && (p_data_manage->heart_measure_s.day_time == 0))
        {
            BleThread_AFE4404DevClose();	 
        }

        p_data_manage->bp_measure_s.bp_day_time = 0;
        p_data_manage->bp_measure_s.bp_night_time = 0;      
        return;
    }
    p_data_manage->bp_measure_s.bp_calc_prg++;
    if(p_data_manage->bp_measure_s.bp_calc_prg <= 5)
    {   //前5次数据更新不计算;出结果后不计算
        return;
    }

    test_mode_s.test_mode = p_data_manage->bp_measure_s.bp_test_mode;
    test_mode_s.set_sp = p_data_manage->bp_measure_s.bp_set_sp;
    test_mode_s.set_dp = p_data_manage->bp_measure_s.bp_set_dp;
    
    Move_IdexGet(&move_index_s);
    Bp_DetectionCalc(DataManage_GetGreenCompletePool(), move_index_s.move_5s, &test_mode_s);

    if(p_data_manage->bp_measure_s.bp_calc_prg < 25)
    {   //血压需要计算20次后才会更新结果
        return;
    }
    
    BP_GetResult(&bp_gresult_s);  

    if(bp_gresult_s.bp_info_s.bp_flag == 1)
    {
        p_data_manage->bp_measure_s.bp_flag = 1;
        
        if(test_mode_s.test_mode == 1)
        {
            p_data_manage->bp_measure_s.bp_sp_value = bp_gresult_s.bp_point_private_25s.sbp;
            p_data_manage->bp_measure_s.bp_dp_value = bp_gresult_s.bp_point_private_25s.dbp; 
        }
        else
        {
            p_data_manage->bp_measure_s.bp_sp_value = bp_gresult_s.bp_point_normal_25s.sbp;
            p_data_manage->bp_measure_s.bp_dp_value = bp_gresult_s.bp_point_normal_25s.dbp;        
        }
        if(p_data_manage->bp_measure_s.bp_day_time || p_data_manage->bp_measure_s.bp_night_time)
        {
            p_data_manage->bp_cache_s.cache_bp_sp = p_data_manage->bp_measure_s.bp_sp_value;
            p_data_manage->bp_cache_s.cache_bp_dp = p_data_manage->bp_measure_s.bp_dp_value;
        } 
    }
    else
    {
        p_data_manage->bp_measure_s.bp_flag = 2;
        p_data_manage->bp_measure_s.bp_sp_value = 0;
        p_data_manage->bp_measure_s.bp_dp_value = 0;
    }
}

/*******************************************************************************************************
** Function name:        AlgorithmThread_Handle
** Descriptions:         算法线程--线程运行体 
** input parameter:      arg--无效参数    
** Returned Value:       void
** Created  by:          GT
** Created  Datas:       2017-08-04
*********************************************************************************************************/
static void AlgorithmThread_Handle(void* arg)
{
    UNUSED_PARAMETER(arg);
     
    while(1)
    {	  	
        alg_run_cnt = 0;
        
        // 1秒事件
        THREAD_MUTEX_TASK_EXCUTE(alg_res_s.m_one_seconds_sem, alg_res_s.m_task_count, Algorithm_OneScondsTask);
              
       // 姿态处理算法任务(姿态实时性要求比较高,所以放在较前位置)
       THREAD_MUTEX_TASK_EXCUTE(alg_res_s.m_gesture_sem,alg_res_s.m_task_count,Algorithm_GestureTask);
       
        // 电池电量检测算法任务
        //THREAD_MUTEX_TASK_EXCUTE(alg_res_s.m_battery_sem,alg_res_s.m_task_count,Algorithm_BatteryGradeTraceTask);
        
        // 倒计时任务
        //THREAD_MUTEX_TASK_EXCUTE(alg_res_s.m_hourglass_sem,alg_res_s.m_task_count,Algorithm_HourglassTask);
       
        // 久坐算法监测任务
        //THREAD_MUTEX_TASK_EXCUTE(alg_res_s.m_sedentary_sem,alg_res_s.m_task_count,Algorithm_SedentaryTask);
        
        // OLED亮度实时计算监测任务
        //THREAD_MUTEX_TASK_EXCUTE(alg_res_s.m_brightness_sem,alg_res_s.m_task_count,Algorithm_OledBrightnessAdjustTask);
        
        // 闹铃实时监测任务
        //THREAD_MUTEX_TASK_EXCUTE(alg_res_s.m_alarm_sem,alg_res_s.m_task_count,Algorithm_AlarmMonitorTask);
        
        // 佩戴检测处理算法任务
        THREAD_MUTEX_TASK_EXCUTE(alg_res_s.m_wear_sem,alg_res_s.m_task_count,Algorithm_WearTask);
       
        // 脱手检测处理算法任务
        THREAD_MUTEX_TASK_EXCUTE(alg_res_s.m_no_wear_sem,alg_res_s.m_task_count,Algorithm_NoWearTask);
        
        // 心率处理算法任务
        THREAD_MUTEX_TASK_EXCUTE(alg_res_s.m_heart_sem,alg_res_s.m_task_count,Algorithm_HeartTask);
        
        // 血氧处理算法任务
        THREAD_MUTEX_TASK_EXCUTE(alg_res_s.m_blood_oxygen_measure_sem,alg_res_s.m_task_count,Algorithm_BloodOxygenTask);

        // 呼吸暂停综合征算法任务
        THREAD_MUTEX_TASK_EXCUTE(alg_res_s.m_osahs_sem,alg_res_s.m_task_count,Algorithm_OsahsTask);

        // HRV处理算法任务
        THREAD_MUTEX_TASK_EXCUTE(alg_res_s.m_hrv_sem,alg_res_s.m_task_count,Algorithm_HRVProcessTask);
        
        // 呼吸率处理算法任务
        THREAD_MUTEX_TASK_EXCUTE(alg_res_s.m_br_sem,alg_res_s.m_task_count,Algorithm_RespirationRateTask);
        
        // 疲劳度处理算法任务
        THREAD_MUTEX_TASK_EXCUTE(alg_res_s.m_fatigue_sem,alg_res_s.m_task_count,Algorithm_FatigueTask);
        
        // 记步处理算法任务
        THREAD_MUTEX_TASK_EXCUTE(alg_res_s.m_pedometer_sem,alg_res_s.m_task_count,Algorithm_PedometerTask);
    
        // 睡眠算法任务
        THREAD_MUTEX_TASK_EXCUTE(alg_res_s.m_sleep_sem,alg_res_s.m_task_count,Algorithm_SleepTask);
        
        // 睡眠遍历算法任务
        THREAD_MUTEX_TASK_EXCUTE(alg_res_s.m_sleep_traversing_sem,alg_res_s.m_task_count,Algorithm_SleepTraversingTask);
        
        // 记步运动量计算任务
        //THREAD_MUTEX_TASK_EXCUTE(alg_res_s.m_step_sport_trace_sem,alg_res_s.m_task_count,Algorithm_StepSportTraceTask);
                    
        // 血压计算任务
        THREAD_MUTEX_TASK_EXCUTE(alg_res_s.m_bp_sem, alg_res_s.m_task_count, Algorithm_BpTask);
          
        if(!((BleThread_GetBleNus()->conn_handle != BLE_CONN_HANDLE_INVALID)&&(p_data_manage->device_trace_param_s.sign_on == 1)) 
           || ComThread_GetSendFlashDataState() != 0)
        {
            App_Watch_Dog_Reload(); //1秒喂狗1次
        }            
        
        /* !注意:这里需要判断算法任务的是否处理完成,只有任务处理完成,才能挂起线程
         *        如果这里返回pdFALSE时,说明线程没有调度任务 
         */
        if(pdFALSE == xSemaphoreTake(alg_res_s.m_task_count,(TickType_t)0))
        {
            // 血氧算法数据处理
            vTaskSuspend(alg_res_s.m_thread_handle);
        }
    }
}

/*******************************************************************************************************
** Function name:        AlgThread_Init
** Descriptions:         线程初始化功能,主要完成以下初始化动作:
**                         1.创建算法所需信号量资源;
**                         2.数据变量初始化;
**                         3.完成线程的创建;
** input parameter:      无     
** Returned Value:       void
** Created  by:          GT
** Created  Datas:       2017-08-04
*********************************************************************************************************/
void AlgThread_Init(void)
{
    // 创建所有的信号量资源
    //SEMAPHORE_CREATE_BINARY_RET(alg_res_s.m_battery_sem);
    //SEMAPHORE_CREATE_BINARY_RET(alg_res_s.m_hourglass_sem);
    //SEMAPHORE_CREATE_BINARY_RET(alg_res_s.m_sedentary_sem);
    SEMAPHORE_CREATE_BINARY_RET(alg_res_s.m_step_sport_trace_sem);
    //SEMAPHORE_CREATE_BINARY_RET(alg_res_s.m_alarm_sem);
    //SEMAPHORE_CREATE_BINARY_RET(alg_res_s.m_brightness_sem);

    SEMAPHORE_CREATE_BINARY_RET(alg_res_s.m_blood_oxygen_measure_sem);
    SEMAPHORE_CREATE_BINARY_RET(alg_res_s.m_osahs_sem);
    SEMAPHORE_CREATE_BINARY_RET(alg_res_s.m_hrv_sem);
    SEMAPHORE_CREATE_BINARY_RET(alg_res_s.m_br_sem);
    SEMAPHORE_CREATE_BINARY_RET(alg_res_s.m_no_wear_sem);
    SEMAPHORE_CREATE_BINARY_RET(alg_res_s.m_wear_sem);
    SEMAPHORE_CREATE_BINARY_RET(alg_res_s.m_heart_sem);
    SEMAPHORE_CREATE_BINARY_RET(alg_res_s.m_pedometer_sem);
    SEMAPHORE_CREATE_BINARY_RET(alg_res_s.m_gesture_sem);
    SEMAPHORE_CREATE_BINARY_RET(alg_res_s.m_fatigue_sem);
    SEMAPHORE_CREATE_BINARY_RET(alg_res_s.m_sleep_sem);
    SEMAPHORE_CREATE_BINARY_RET(alg_res_s.m_sleep_traversing_sem);
    
    SEMAPHORE_CREATE_BINARY_RET(alg_res_s.m_one_seconds_sem);
	
    SEMAPHORE_CREATE_BINARY_RET(alg_res_s.m_bp_sem);
    
    // 创建计数信号量,初始值为0,信号量最大值为16
    alg_res_s.m_task_count = xSemaphoreCreateCounting(13,0);
    if (alg_res_s.m_task_count == NULL)
    {
        APP_ERROR_HANDLER(NRF_ERROR_NO_MEM);
    }
				
    //初始化睡眠读取Flash回调函数
    Sleep_Init();

    DataManage_Init();
    
    //闹钟自检单次闹钟状态
    Algorithm_AlarmValidCheck();
    
    // 初始化获取亮度信息
    //Algorithm_OledBrightnessAdjustTask();
            
    // 创建数据保存任务线程 RAM=>7K
    if(xTaskCreate(AlgorithmThread_Handle, "Algorithm", 1792, NULL, OXYGEN_ALGORITHM_THREAD_PRIORITY, &alg_res_s.m_thread_handle) != pdPASS)  
    {
        APP_ERROR_HANDLER(NRF_ERROR_NO_MEM);
    }
}

/*******************************************************************************************************
** Function name:        AlgThread_AFE4404Task
** Descriptions:         AFE4404数据处理算法唤醒任务函数
** input parameter:      void     
** Returned Value:       void
** Created  by:          GT
** Created  Datas:       2017-08-04
*********************************************************************************************************/
void AlgThread_AFE4404Task(void)
{    
    // 佩戴算法条件:1.佩戴检测开关-开 2.数据采集时间>=2S
    if((p_flash_param->dev_config_s.wear_detect_enable == 1 
        && p_flash_param->dev_config_s.led_gear == 0)
        &&(p_data_manage->data_pool.pool_update_second >= 2))
    {
        if((Movement_GetState() != MOVEMENT_STATE_RUN))
        {
            xSemaphoreGive(alg_res_s.m_wear_sem); 
            xSemaphoreGive(alg_res_s.m_task_count); 
        }
    }
            
    // 调用疲劳度算法 次/4秒
    if((p_data_manage->fatigue_measure_s.app == 1)
        &&(p_data_manage->data_pool.pool_update_second%4 == 0))
    {
        xSemaphoreGive(alg_res_s.m_fatigue_sem); 
        xSemaphoreGive(alg_res_s.m_task_count); 
    }
            
    // 调用HRV算法 次/1秒
    if((p_data_manage->hrv_measure_s.time == 1)
        &&(p_data_manage->data_pool.pool_update_second >= 6))
    {
        if(p_data_manage->blood_oxygen_measure_s.cal_success)
        {
            xSemaphoreGive(alg_res_s.m_hrv_sem); 
            xSemaphoreGive(alg_res_s.m_task_count); 
        }
    }
    
    // 调用心率算法
    if(p_data_manage->data_pool.pool_update_second >= 6)
    {
        if((p_data_manage->heart_measure_s.app == 1)
           || (p_data_manage->heart_measure_s.btn == 1)
           || (Movement_GetState() == MOVEMENT_STATE_RUN) 
           || (p_data_manage->heart_measure_s.day_time == 1)
           || (p_data_manage->heart_measure_s.night_time == 1 
               && p_data_manage->blood_oxygen_measure_s.cal_success))
        { 
            xSemaphoreGive(alg_res_s.m_heart_sem); 
            xSemaphoreGive(alg_res_s.m_task_count); 
        }
    }
    
    // 血压
    if((p_data_manage->data_pool.pool_update_second >= 1)
        && (((p_data_manage->bp_measure_s.bp_app == 1)
        || (p_data_manage->bp_measure_s.bp_btn == 1)
        || (p_data_manage->bp_measure_s.bp_day_time == 1)
        || (p_data_manage->bp_measure_s.bp_night_time == 1 && p_data_manage->blood_oxygen_measure_s.cal_success))))
    { 
        xSemaphoreGive(alg_res_s.m_bp_sem); 
        xSemaphoreGive(alg_res_s.m_task_count); 
    }
    
                                
    // 调用血氧算法
//    if((p_data_manage->data_pool.pool_update_second >= 6)&&
    if((p_data_manage->data_pool.pool_update_second >= 1)&&
       ((p_data_manage->blood_oxygen_measure_s.app == 1)||(p_data_manage->blood_oxygen_measure_s.btn == 1)||(p_data_manage->blood_oxygen_measure_s.time == 1)))    
    {
        xSemaphoreGive(alg_res_s.m_blood_oxygen_measure_sem); 
        xSemaphoreGive(alg_res_s.m_task_count); 
    }
                
    // 调用呼吸率算法
    //YX.20180308.优化呼吸率算法.缓存15秒数据后，1秒钟调用一次
    if((p_data_manage->data_pool.pool_update_second >= 15)&&
       ((p_data_manage->respiratory_rate_s.app == 1)||(p_data_manage->respiratory_rate_s.time == 1 && p_data_manage->blood_oxygen_measure_s.cal_success)))    
    {
        xSemaphoreGive(alg_res_s.m_br_sem); 
        xSemaphoreGive(alg_res_s.m_task_count); 
    }
                
    if(xSemaphoreTake(alg_res_s.m_task_count,(TickType_t)0) == pdTRUE)
    {
        // 上面做计数信号量(减一)处理,这里需要加一还原
        xSemaphoreGive(alg_res_s.m_task_count);
        vTaskResume(alg_res_s.m_thread_handle);
    }   
}

/*******************************************************************************************************
** Function name:        AlgThread_Osahs
** Descriptions:         呼吸暂停算法唤醒任务函数
** input parameter:      无   
** Returned Value:       void
** Created  by:          GT
** Created  Datas:       2017-08-04
*********************************************************************************************************/
void AlgThread_Osahs(void)
{
    THREAD_TASK_WAKE_UP(alg_res_s.m_osahs_sem,alg_res_s.m_task_count,alg_res_s.m_thread_handle);
}

void AlgThread_SleepTraversing(void)
{
    static uint32_t time_minutes = 0;
    
    if(Api_GetSleepTotalTime() != 0)
    {
        time_minutes = 0;
    }
    else
    {
        if(time_minutes == 0 || (clock->hour * 60 + clock->minutes - time_minutes) >= 5)
        {
            time_minutes = clock->hour * 60 + clock->minutes;
            THREAD_TASK_WAKE_UP(alg_res_s.m_sleep_traversing_sem,alg_res_s.m_task_count,alg_res_s.m_thread_handle);
        }
        else if(time_minutes > clock->hour * 60 + clock->minutes)
        {
            time_minutes = 0;
        }
    }
}

void AlgThread_GsensorEventCommit(void)
{
    uint8_t no_wear_flag = 0;
    move_index_t  m_move_index_s = {0};
    
    // 姿态数据长度判断
    if(p_data_manage->acc.gesture_data_len >= 5)
    {
        THREAD_TASK_WAKE_UP(alg_res_s.m_gesture_sem,alg_res_s.m_task_count,alg_res_s.m_thread_handle);
    }
                
    // 记步数据算法调用
    if(p_data_manage->acc.pedometer_data_len >= 25)
    {
        THREAD_TASK_WAKE_UP(alg_res_s.m_pedometer_sem,alg_res_s.m_task_count,alg_res_s.m_thread_handle);
    }   
    
    // 脱手检测处理
    no_wear_flag = Get_NoWearStation();
    Move_IdexGet(&m_move_index_s);
    if((no_wear_flag >= 2)&&(no_wear_flag <= 4)&&(m_move_index_s.move_5s >= 3))
    {
        p_data_manage->no_wear_measure_s.init = 1;
    }
}

/*******************************************************************************************************
** Function name:        AlgThread_PeriodCycleTask
** Descriptions:         线程内部任务
** input parameter:      无   
** Returned Value:       时间
** Created  by:          郭涛
** Created  Datas:       2017-08-04
*********************************************************************************************************/
void AlgThread_PeriodCycleTask(uint8_t hour, uint8_t minute,uint8_t second)
{     
    alg_hour = hour;
    alg_minute = minute;
    alg_second = second;

    //
    alg_run_cnt++;
    if(alg_run_cnt > 5)
    {   // 5秒复位
        NVIC_SystemReset();     //reset
    }
    
    
    // 1秒事件
    xSemaphoreGive(alg_res_s.m_one_seconds_sem);
    xSemaphoreGive(alg_res_s.m_task_count); 
    
    // 定时唤醒任务线程
    vTaskResume(alg_res_s.m_thread_handle); 
}

/*******************************************************************************************************
** Function name:        AlgThread_OffTimeMeasurement
** Descriptions:         关掉自动测量
** input parameter:      无   
** Returned Value:       无
** Created  by:          YX
** Created  Datas:       2018-11-01
*********************************************************************************************************/
void AlgThread_OffTimeMeasurement(void)
{
    p_data_manage->blood_oxygen_measure_s.init = 0;
    p_data_manage->blood_oxygen_measure_s.time = 0;
    p_data_manage->blood_oxygen_measure_s.ox_day_time = 0; //区分是不是白天开启的
    p_data_manage->blood_oxygen_measure_s.ox_calc_cnt = 0; // 校准完成后，计算次数，用于白天自动测量 
    p_data_manage->blood_oxygen_measure_s.ox_prompt_sign_flag = 0;
    p_data_manage->hrv_measure_s.init = 0;
    p_data_manage->hrv_measure_s.time = 0;
    p_data_manage->respiratory_rate_s.init = 0;
    p_data_manage->respiratory_rate_s.time = 0;
    
    p_data_manage->heart_measure_s.init = 0;
    p_data_manage->heart_measure_s.day_time = 0;
    p_data_manage->heart_measure_s.night_time = 0;
    p_data_manage->heart_measure_s.calib_well = 0;
    p_data_manage->heart_measure_s.calib_init = 0;

    p_data_manage->bp_measure_s.bp_init = 0;
    p_data_manage->bp_measure_s.bp_day_time = 0;
    p_data_manage->bp_measure_s.bp_night_time = 0;
    
}

