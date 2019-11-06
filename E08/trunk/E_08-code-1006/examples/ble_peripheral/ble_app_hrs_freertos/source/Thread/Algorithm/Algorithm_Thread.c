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
** Description:             算法线程主要处理算法相关的任务, 主要算法如下:
**                          1.血氧算法
**                          2.心率算法...                        
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
#include "gesture_test.h"
#include "no_weardetect.h"
#include "flash_data_type.h"
#include "cpu_task_data_type.h"
#include "SystemTime.h"
#include "data_manage.h"
#include "battery_adc.h"
#include "pedometer.h"
#include "sleep.h"
#include "gui_thread.h"
#include "calc_data_type.h"
#include "api_flash.h"
#include "log.h"
#include "util.h"
#include "movement_patterns.h"
#include "watch_dog.h"
#include "posture_get.h"
#include "Communication_Thread.h"
#include "ecg_detect.h"
#include "Pwave_InfoAlgorithm.h"
#include "bsp_btn_ble.h"
#include "GSensor.h"

#include "Blood_OxygenDetection.h"
#include "Spo2_OSAHS.h"

#include "test_state_machine.h"
#include "bracelet_protocol.h"
#include "si1182_driver.h"
#include "si1182_calibration.h"



//debug
#define RESP_TEST_DEBUG     0   //测试呼吸率，通过心率测试输出值


#define ECG_TEST_MODE_AUTOMATIC     0   //自动测量模式
#define ECG_TEST_MODE_MANUAL        1   //手动测量模式

#define HEART_CACHE_LEN             10  //心率告警缓存大小

static algorithm_thread_resource_t  alg_res_s;

static uint8_t      heart_h_cache[HEART_CACHE_LEN] = {0};  //心率告警缓存

static uint8_t      alg_hour = 0;
static uint8_t      alg_minute = 0; 
static uint8_t      alg_second = 0;

static uint8_t      alg_sleep_traversing = 0;       // 睡眠遍历标志


/************************************************************************
** Function name:        Algorithm_WearTask
** Descriptions:         算法线程--佩戴算法任务
** input parameter:      无     
** Returned Value:       无
** Created  by:          郭涛
** Created  Datas:       2017-08-04
**************************************************************************/
static void Algorithm_WearTask(void)
{
    if(p_data_manage->wear_measure_s.init == 1)
    {
        p_data_manage->wear_measure_s.init = 0;    
        p_data_manage->wear_measure_s.wear_status = 0; 
        p_data_manage->wear_measure_s.wear_not_pass_cnt = 0;
        p_data_manage->no_wear_measure_s.uncall_wear_cnt = 0;
    } 

	if(p_data_manage->ecg_ppg_s.ppg_measure_s.p_wear_flag == 1
      && (Movement_GetState() != MOVEMENT_STATE_RUN) 
      && p_data_manage->device_trace_param_s.factory_mode == 0)
    {
        if(++p_data_manage->wear_measure_s.wear_not_pass_cnt > 5) 
        {
            p_data_manage->wear_measure_s.wear_status = 1;
            p_data_manage->no_wear_measure_s.last_wear_status = 1;

            // 如果佩戴不通过,则需要关掉AFE4900并且初始化所有的测量出生状态
            if(p_data_manage->ecg_ppg_s.ecg_ppg_mode == MEASURE_MODE_PPG)
            {
                //自动测量和按键测量状态在这里处理，app测量状态在关闭的时候处理
                p_data_manage->ecg_ppg_s.read_data = 0;
                p_data_manage->ecg_ppg_s.ppg_measure_s.time = 0;
                p_data_manage->ecg_ppg_s.ppg_measure_s.btn = 0;
                p_data_manage->ecg_ppg_s.ecg_ppg_mode = MEASURE_MODE_NULL;
                BleThread_PeripheralsDevClose();
            }
            else if(p_data_manage->ecg_ppg_s.ecg_ppg_mode == MEASURE_MODE_OXYGEN)
            {
                //自动测量和按键测量状态在这里处理，app测量状态在关闭的时候处理
                p_data_manage->ecg_ppg_s.read_data = 0;
                p_data_manage->ecg_ppg_s.oxygen_measure_s.ox_btn = 0;
                p_data_manage->ecg_ppg_s.oxygen_measure_s.ox_time = 0;  
                p_data_manage->ecg_ppg_s.ecg_ppg_mode = MEASURE_MODE_NULL;
                BleThread_PeripheralsDevClose();   
            }
        }
    }
    else
    {
        p_data_manage->wear_measure_s.wear_status = 0;
        p_data_manage->wear_measure_s.wear_not_pass_cnt = 0;  
    }    
}

/************************************************************************
** Function name:        Algorithm_NoWearTask
** Descriptions:         算法线程--脱手检测算法任务
**                       由于脱手检测算法在每五分钟执行一次, 但是佩戴检测算法执行完成需要六秒才能完成, 
**                       需要注意状态。
** input parameter:      无     
** Returned Value:       无
** Created  by:          郭涛
** Created  Date:        2017-08-04
**************************************************************************/
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
    m_sport_history_s = Vnwd_HistoryMomentum(p_data_manage->no_wear_measure_s.amount, p_data_manage->no_wear_measure_s.step);
    
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

/************************************************************************
** Function name:        Algorithm_PpgTask
** Descriptions:         算法线程--PPG算法任务
** input parameter:      无     
** Returned Value:       无
** Created  by:          WJ
** Created  Datas:       2018-10-18
**************************************************************************/
si1182_register_parameter_t     si1182_register_parameter_s = {0};      //1182配置参数

static void Algorithm_PpgTask(void)
{
#if (!SLEEP_CALC_DEBUG)    
    uint8_t             heart_h = 0;
//	static uint8_t      sta_heart_pulse = 0;
	static uint32_t     ppg_algorithm_cnt = 0;
    static test_mode_t  test_mode_s = {0};	 
//	uint16_t            ppg_err = 0;
    move_index_t        move_index_s = {0};
    pwave_onemin_info_t     *pwave_onemin_info = NULL;
    ppg_measure_t           *p_ppg_measure = NULL;    
    respiratory_rate_measure_t      *p_resp_rate = NULL;
    static uint8_t      calc_ppg_seconds = 0;   //用于ppg算法使用，睡眠算法1

#if RESP_TEST_DEBUG    
    static uint8_t resp_flg = 0;
#endif
    
    p_ppg_measure = &p_data_manage->ecg_ppg_s.ppg_measure_s;
    p_resp_rate   = &p_data_manage->ecg_ppg_s.respiratory_rate_measure_s;
    
    // 心率算法初始化
    if(p_ppg_measure->init == 1)
    {
        p_ppg_measure->init = 0;
        //WJ.20181024.大小需要去掉标记头部信息,主要需要指针类型转换
        memset(((uint8_t *)&p_data_manage->ecg_ppg_s.ppg_measure_s) + 1, 0x00, sizeof(ppg_measure_t) - 1);
		ppg_algorithm_cnt = 0;       
        // PPG算法初始化
        Pwave_InfoInit();
        // 设置血压测量模式
        p_ppg_measure->calc_bp_init = 1;
        
        memset(heart_h_cache, 0, HEART_CACHE_LEN);
        
        calc_ppg_seconds = clock->seconds;

#if RESP_TEST_DEBUG         
        resp_flg = 0;
#endif
    }
    
    if(p_ppg_measure->calc_bp_init == 1)
    {   //血压算法初始化
        p_ppg_measure->calc_bp_init = 0;
        if(p_ppg_measure->app_bp == 1)
        {   // app测量
            test_mode_s.test_mode = p_data_manage->app_bp_mode;    
        }
        else
        {
            test_mode_s.test_mode = p_flash_param->bp_s.mode;
        }
        test_mode_s.set_sp = p_flash_param->bp_s.private_high;
        test_mode_s.set_dp = p_flash_param->bp_s.private_low; 
        //初始化接口
        p_ppg_measure->bp_progress = 0;
    }
    
    Move_IdexGet(&move_index_s);
	ppg_algorithm_cnt++;
    if(ppg_algorithm_cnt > 300)
    {
        ppg_algorithm_cnt = 40;
    }
    // WJ.20190117.优化时间跳跃导致计算不出结果的问题
    if((calc_ppg_seconds % 10 == 9) && (clock->seconds % 10 == 1))
    {
        calc_ppg_seconds = (calc_ppg_seconds + 1) % 60;
    }
    else
    {
        calc_ppg_seconds = clock->seconds;
    }
    
    // PPG算法调用
	//ppg_err = 
    Pwave_InfoAlgorithm(DataManage_GetGreenDataPool(), 
                        DataManage_GetIredDataPool(),
                        NULL/*DataManage_GetAmbDataPool()*/, 
                        calc_ppg_seconds, ppg_algorithm_cnt, &move_index_s, 
                        &test_mode_s,
                        &si1182_register_parameter_s);	

	// PPG算法的结果获取
	pwave_onemin_info = Get_PwaveInfo();
	// WJ.20190220.测量手动血氧时，只处理佩戴状态
    {
        p_ppg_measure->p_wear_flag = pwave_onemin_info->wear_flag;              // ppg佩戴状态   
        
        // 是否调用佩戴结果处理 //佩戴算法条件:非黑人模式
        if(p_flash_param->dev_config_s.led_gear == 0 
           && (p_data_manage->ecg_ppg_s.ecg_ppg_mode == MEASURE_MODE_PPG 
               || p_data_manage->ecg_ppg_s.ecg_ppg_mode == MEASURE_MODE_OXYGEN))
        {
            xSemaphoreGive(alg_res_s.m_wear_sem); 
            xSemaphoreGive(alg_res_s.m_task_count); 
        }

        if(p_data_manage->ecg_ppg_s.oxygen_measure_s.ox_app == 1 
           || p_data_manage->ecg_ppg_s.oxygen_measure_s.ox_btn == 1)
        {
            return;
        }
    }
#if RESP_TEST_DEBUG       // 测试呼吸率 
     // 呼吸率
    if(pwave_onemin_info->resp_rate_1min != 255 && resp_flg == 0)//pwave_onemin_info->resp_rate_1min <= 50)
    {
        resp_flg = 1; 
        p_ppg_measure->p_second_heart = pwave_onemin_info->resp_rate_1min;
    }
    else if(resp_flg == 0)   
    {
        p_ppg_measure->p_second_heart = 255;
    }
#else    
    
    if(pwave_onemin_info->hr_ppg_per_sec_result >= 30 && pwave_onemin_info->hr_ppg_per_sec_result <= 180)
    {
        p_ppg_measure->p_second_heart = pwave_onemin_info->hr_ppg_per_sec_result;   // 实时心率     
    }

#endif
    
    if(pwave_onemin_info->hr_ppg_per_min_result >= 30 && pwave_onemin_info->hr_ppg_per_min_result <= 180)
    {
        p_ppg_measure->p_minute_heart = pwave_onemin_info->hr_ppg_per_min_result;   // 一分钟心率均值
    }
    if(pwave_onemin_info->hrv_ppg_per_sec_result <= 210)
    {
        p_ppg_measure->p_second_hrv = pwave_onemin_info->hrv_ppg_per_sec_result;     // 每秒hrv
    }
    if(pwave_onemin_info->rr_per_sec_result >= 30 && pwave_onemin_info->rr_per_sec_result <= 210)
    {
        p_ppg_measure->p_second_rr = pwave_onemin_info->rr_per_sec_result;      // 每秒hrv 
    }
    if(pwave_onemin_info->rr_per_6sec_result >= 30 && pwave_onemin_info->rr_per_6sec_result <= 210)
    {
        p_ppg_measure->p_6sec_rr = pwave_onemin_info->rr_per_6sec_result;        // 每6秒hrv 
    }
    
    p_ppg_measure->p_calc_cnt = ppg_algorithm_cnt;
    
    if(p_ppg_measure->app_bp == 1 || p_ppg_measure->btn_bp == 1)
    {
        if(p_ppg_measure->bp_progress < 200)
        {
            p_ppg_measure->bp_progress++;
        }
    }
    
    // 血压数据
    p_ppg_measure->p_bp_flag = pwave_onemin_info->bp_flag;
    if(pwave_onemin_info->bp_flag == 1)
    {
        //if(Bp_ResultCheck(test_mode_s.test_mode, pwave_onemin_info->high_bp_per_sec, pwave_onemin_info->low_bp_per_sec, 0))
        {
            p_ppg_measure->p_bp_high_sec = pwave_onemin_info->high_bp_per_sec;
            p_ppg_measure->p_bp_low_sec = pwave_onemin_info->low_bp_per_sec;
        }
        //if(Bp_ResultCheck(test_mode_s.test_mode, pwave_onemin_info->high_bp_per_25sec, pwave_onemin_info->low_bp_per_25sec, 0)))
        {
            p_ppg_measure->p_bp_high_25sec = pwave_onemin_info->high_bp_per_25sec;
            p_ppg_measure->p_bp_low_25sec = pwave_onemin_info->low_bp_per_25sec;
        }
        //if(Bp_ResultCheck(test_mode_s.test_mode, pwave_onemin_info->high_bp_per_5min, pwave_onemin_info->low_bp_per_5min, 0))
        {
            p_ppg_measure->p_bp_high_5min = pwave_onemin_info->high_bp_per_5min;
            p_ppg_measure->p_bp_low_5min = pwave_onemin_info->low_bp_per_5min;
        }
    }  
    
	if(p_data_manage->ecg_ppg_time_s.second == 0)
	{
		uint32_t minute_index = 0;
		
        // 对60分钟特殊处理，1~5分钟,6~10分钟划分组  
		if(p_data_manage->ecg_ppg_time_s.minute != 0)
		{
			minute_index = (p_data_manage->ecg_ppg_time_s.minute - 1) % 5;
		}
		else
		{
			minute_index = 4;
		}
		
        p_data_manage->algorithm_result_cache.sleep_state[minute_index] = pwave_onemin_info->one_min_result;
		p_data_manage->algorithm_result_cache.sleep_state[5] |= ((pwave_onemin_info->one_min_wrong_flag & 0x01) << minute_index);
		p_data_manage->algorithm_result_cache.sleep_effective++;
        
        
        if(pwave_onemin_info->hr_ppg_per_min_result >= 30 
          && pwave_onemin_info->hr_ppg_per_min_result <= 180)
        {
            p_data_manage->algorithm_result_cache.pulse_frequency[minute_index] = pwave_onemin_info->hr_ppg_per_min_result;
        }
        
        // 心脏负荷
        if(pwave_onemin_info->heart_load_1m <= 100)
        {
            p_data_manage->algorithm_result_cache.cache_cardiac_load[minute_index] = pwave_onemin_info->heart_load_1m;
        }
        // 呼吸率
        if(pwave_onemin_info->resp_rate_1min <= 50)
        {
            p_data_manage->algorithm_result_cache.breathing_rate[minute_index] = pwave_onemin_info->resp_rate_1min;
        }
	}
	
	if(p_data_manage->ecg_ppg_time_s.second % 6 == 0)
	{
		uint32_t second_index = 0;
		uint32_t minute_index = 0;
		
		if(p_data_manage->ecg_ppg_time_s.second != 0)
		{
			second_index = (p_data_manage->ecg_ppg_time_s.second - 1) / 6;
            minute_index = (p_data_manage->ecg_ppg_time_s.minute) % 5;
		}
		else
		{
			second_index = 9;
            minute_index = (p_data_manage->ecg_ppg_time_s.minute - 1) % 5;
            
            if(p_data_manage->ecg_ppg_time_s.minute != 0)
            {
                minute_index = (p_data_manage->ecg_ppg_time_s.minute - 1) % 5;
            }
            else
            {
                minute_index = 4;
            }
		}
        
        if(pwave_onemin_info->rr_per_6sec_result >= 30 
          && pwave_onemin_info->rr_per_6sec_result <= 210)
        {
            p_data_manage->algorithm_result_cache.hrv_value[minute_index*10 + second_index] = pwave_onemin_info->rr_per_6sec_result;
        }
		p_data_manage->algorithm_result_cache.hrv_type |= ((0 & 0x01) << minute_index);
	}
    
    if(p_flash_param->dev_config_s.blood_detect_enable == 1 /*&& p_flash_param->bp_func_state == 0x01*/)
    {
        //血压数据填充
        if(pwave_onemin_info->bp_flag == 1  
           && (p_ppg_measure->p_bp_high_5min)
           && (p_ppg_measure->p_bp_low_5min)
           && (p_ppg_measure->app_bp == 0))
        {
            p_data_manage->algorithm_result_cache.bp_high = p_ppg_measure->p_bp_high_5min;//pwave_onemin_info->high_bp_per_5min;
            p_data_manage->algorithm_result_cache.bp_low = p_ppg_measure->p_bp_low_5min;//pwave_onemin_info->low_bp_per_5min;    
        }
    
        if(p_flash_param->dev_config_s.ppg_detect_enable != 1
          && (p_ppg_measure->app == 0 && p_ppg_measure->app_bp == 0 && p_ppg_measure->btn ==0
             && p_ppg_measure->btn_bp == 0 && Movement_GetState() != MOVEMENT_STATE_RUN
             && ppg_algorithm_cnt > 30
             && p_data_manage->ecg_ppg_s.oxygen_measure_s.ox_time == 0))
        {
            //关闭血压自动测量
            Statemachine_DelState(MEAT_PPG_TIME);
        }
    }
    else if(p_flash_param->dev_config_s.heart_detect_enable == 1)
    {   // 10分钟或者五分脉率
        if(p_flash_param->dev_config_s.ppg_detect_enable != 1
          && (p_ppg_measure->app == 0 && p_ppg_measure->app_bp == 0 && p_ppg_measure->btn == 0
             && p_ppg_measure->btn_bp == 0 && Movement_GetState() != MOVEMENT_STATE_RUN
             && ppg_algorithm_cnt > 20
             && p_data_manage->ecg_ppg_s.oxygen_measure_s.ox_time == 0))
        {
            //关闭脉率自动测量
            Statemachine_DelState(MEAT_PPG_TIME);
        } 
        
        if(p_flash_param->dev_config_s.ppg_detect_enable != 1 && ppg_algorithm_cnt > 20)
        {   // 保存数据到存储缓存
            if(pwave_onemin_info->hr_ppg_per_min_result >= 30 
              && pwave_onemin_info->hr_ppg_per_min_result <= 180)
            {
                p_data_manage->algorithm_result_cache.pulse_frequency[0] = pwave_onemin_info->hr_ppg_per_min_result;
            }       
        }
    }

    //呼吸率数据处理
    if(p_resp_rate->resp_init == 1)
    {   
        p_resp_rate->resp_init = 0;
        p_resp_rate->resp_cnt = 0;
    }
    if(p_resp_rate->resp_cnt < 300)
    {
        // 填充呼吸率
        p_resp_rate->resp_result_sec[p_resp_rate->resp_cnt++] = pwave_onemin_info->resp_rate_1sec;
    }

    
    // 根据需要重新初始化血压算法
    //if(pwave_onemin_info->bp_flag == 2)
    //{   // 测试无效，需要重新初始化，算法才会计算法
        //血压算法初始化标记
        //p_ppg_measure->calc_bp_init = 1;
    //}
    
    if(p_flash_param->dev_config_s.ppg_detect_enable == 1 
       && p_data_manage->ecg_ppg_time_s.second == 0
       && clock->seconds == 0)
    {    
        if(p_ppg_measure->app == 0 && p_ppg_measure->app_bp == 0 && p_ppg_measure->btn ==0
           && p_ppg_measure->btn_bp == 0 && Movement_GetState() != MOVEMENT_STATE_RUN)
        {   //心率告警

            //memcpy(&heart_h_cache[0], &heart_h_cache[1], HEART_CACHE_LEN - 1);
            //heart_h_cache[HEART_CACHE_LEN - 1] = p_ppg_measure->p_second_heart; 
            
            heart_h = pwave_onemin_info->hr_ppg_per_min_result;//Heart_FilterAverage(heart_h_cache, HEART_CACHE_LEN);
            
            if((heart_h >= p_flash_param->heart_alert_s.high)&&(p_flash_param->heart_alert_s.enable == 1))
            {
    
#if SLEEP_CALC_ENABLE                
                
                if(Array_ShortSum(p_data_manage->pedometer.step_count_10m, 10) < 40
                   && p_data_manage->sleep_flag != 1)
                {
                    GuiThread_AddEvent(CPU_EVENT_TASK_HEARTALERT);
                    memset(heart_h_cache, 0, HEART_CACHE_LEN);
                }
#endif                
            }   
        }
    }
    
    
#endif  //#if (!SLEEP_CALC_DEBUG)    
}

#define ECG_CALC_TIME_TEST 0

#if ECG_CALC_TIME_TEST
uint32_t test_calc_time[10] = {0};
uint32_t test_calc_time1[10] = {0};
uint8_t  test_time_cnt = 0;
#endif

static void Algorithm_EcgTask(void)
{
#if (!SLEEP_CALC_DEBUG)
    
#if ECG_CALC_TIME_TEST
    test_calc_time[test_time_cnt] = (uint32_t)(xTaskGetTickCount());    
#endif
    
    uint8_t         ecg_tes_mode = 0;
    static uint8_t  ecg_calc_cnt = 0;
    static uint8_t  ecg_id = 0;
//	int32_t         ecg_err = 0;
	move_index_t    move_index_s = {0};
	ecg_result_t    ecg_result = {0};
    ecg_diag_result_t   ecg_diag_result_s = {0};
    ecg_measure_t       *p_ecg_measure = NULL;
	ecg_enter_t		    ecg_enter = {0};
    
    static uint8_t      ecg_save_flag = 0;      //按键测量保存数据标志
    
    static uint8_t      ecg_wear_last_state = 0;    //记录上一次的佩戴状态是
    
	p_ecg_measure = &p_data_manage->ecg_ppg_s.ecg_measure_s;
    
	if(p_ecg_measure->init == 1)
    {
        p_ecg_measure->init = 0;
        //WJ.20181024.大小需要去掉标记头部信息,主要需要指针类型转换
        memset(((uint8_t *)&p_data_manage->ecg_ppg_s.ecg_measure_s) + 1, 0x00, sizeof(ecg_measure_t) - 1);

        // ECG算法初始化
        Ecg_DetectInit();
        ecg_calc_cnt = 0;
        ecg_id++;
        ecg_save_flag = 0;
        ecg_wear_last_state = 0;
    }
    
    if(p_ecg_measure->calc_init == 1)
    {
        p_ecg_measure->calc_init = 0;
        ecg_calc_cnt = 0; 
    }

    
//    if(p_data_manage->ecg_ppg_s.ecg_ppg_mode == MEASURE_MODE_PTT
//      && (p_data_manage->ecg_ppg_s.ptt_app == 0 || p_ecg_measure->app == 0))
//    {   //自动测量模式，目前主要是针对PTT模式
//        ecg_tes_mode = ECG_TEST_MODE_AUTOMATIC;
//        
//        ecg_calc_cnt = (++ecg_calc_cnt) % 32;
//        ecg_calc_cnt = ecg_calc_cnt==0?1:ecg_calc_cnt;
//    }
//    else
    {  //按键和app手动测量模式,或者其他
        ecg_tes_mode = ECG_TEST_MODE_MANUAL;
        ecg_calc_cnt = ++ecg_calc_cnt;
    }
    
	Move_IdexGet(&move_index_s);
	
	ecg_enter.diga_mode = ecg_tes_mode;
	ecg_enter.func_times = ecg_calc_cnt;
    //memcpy(&ecg_enter.ecg_leadoff[0], &p_data_manage->data_pool.m_ecg_leadoff[0], 5);
    //导联赋值
    
//    if(Si1182_IsLeadOn())
//    {
//        ecg_enter.ecg_leadoff = 0;
//    }
//    else
//    {
//        ecg_enter.ecg_leadoff = 1;
//    }
    
    ecg_enter.ecg_leadoff = ecg_wear_last_state;
    
    ecg_enter.ecg_afe4490_parameter_s.register_current = 0;
    ecg_enter.ecg_afe4490_parameter_s.register_scale = 0;
    
	// ecg 最后更新1s数据处理
    ////Reduce_EcgLdInterference(DataManage_GetEcgDataPoolLd(), ECG_CACHE_SIZE);
    // ECG算法调用
//	ecg_err = 
    Ecg_DetectEntry(DataManage_GetEcgDataPool(), &move_index_s, ecg_enter);
	
//    if(ecg_err == 0)
//    {
//    }
//    else  
//    {
//    }

	// ECG算法的结果获取
	Ecg_GetResult(&ecg_result);
	
    p_ecg_measure->e_m_id = ecg_id;
    
    if(ecg_result.heart_rate >= 30 && ecg_result.heart_rate <= 250)
    {
        p_ecg_measure->e_second_heart = ecg_result.heart_rate;
    }
    else
    {
        p_ecg_measure->e_second_heart = 0;
    }
    if(ecg_result.hr_per_min_result >= 30 && ecg_result.hr_per_min_result <= 250)
    {
        p_ecg_measure->e_minute_heart = ecg_result.hr_per_min_result;
    }
    
//    p_ecg_measure->e_minute_heart = ecg_err & 0xFF;
    
    if(ecg_result.hrv <= 210)
    {
        p_ecg_measure->e_second_hrv = ecg_result.hrv;
    }
    else
    {
        p_ecg_measure->e_second_hrv = 0xFF;//约定255未默认无效值
    }
    if(ecg_result.rr >= 30 && ecg_result.rr <= 210)
    {
        p_ecg_measure->e_second_rr = ecg_result.rr;
    }
    if(ecg_result.rr_per_6sec_result >= 30 && ecg_result.rr_per_6sec_result <= 210)
    {
        p_ecg_measure->e_6sec_rr = ecg_result.rr_per_6sec_result;
    }
    p_ecg_measure->e_ary = ecg_result.ary;
    if(ecg_result.resp_per_second_result <= 50)
    {
        p_ecg_measure->e_second_br = ecg_result.resp_per_second_result;
    }
    if(ecg_result.resp_per_min_result <= 50)
    {
        p_ecg_measure->e_minute_br = ecg_result.resp_per_min_result;
	}
	
    p_ecg_measure->e_qtc_time = ecg_result.QTC_sec;
    
    //需要算法给出有效进度
    p_ecg_measure->e_calc_cnt = ecg_calc_cnt;

    ////p_ecg_measure->result_progress = ecg_result.measurement_progress;
    
    if(p_ecg_measure->app == 1)
    {
        p_ecg_measure->e_app_calc_cnt++;
    }
    
    //p_ecg_measure->e_wear_flag = ecg_result.lead_off_detec_res & 0xFF;
    p_ecg_measure->e_wear_flag = ecg_result.ecg_ld_judge_flag & 0xFF;
    ecg_wear_last_state = ecg_result.ecg_ld_judge_flag;
    
    if(p_ecg_measure->e_wear_flag == 1)
    {
        if(p_ecg_measure->e_wear_time < 200)
        {
            p_ecg_measure->e_wear_time++;        
        }
        if(p_ecg_measure->e_wear_time >= 6)
        {
//            if(p_data_manage->ecg_ppg_s.ecg_ppg_mode == MEASURE_MODE_PTT 
//              && p_ecg_measure->btn == 1 && p_ecg_measure->app == 0 )
//            {
//                Buttons_Enable();
//            }
        }
    }
    else
    {
        if(p_ecg_measure->e_wear_flag == 1)
        {
            //还需要明确如何处理-科研给出方案
        }
        p_ecg_measure->e_wear_time = 0;
    }
     
	if(p_data_manage->ecg_ppg_time_s.second == 0 && p_data_manage->ecg_ppg_time_s.ecg_updata_cnt++ < 2)
	{
		uint32_t minute_index = 0;
		
		if(p_data_manage->ecg_ppg_time_s.minute != 0)
		{
			minute_index = (p_data_manage->ecg_ppg_time_s.minute - 1) % 5;
		}
		else
		{
			minute_index = 4;
		}
		
        if(ecg_result.hr_per_min_result >= 30
           && ecg_result.hr_per_min_result <= 250)
        {
            p_data_manage->algorithm_result_cache.heart_rate[minute_index] = ecg_result.hr_per_min_result;
        }
        if(ecg_result.resp_per_min_result <= 50)
        {
            p_data_manage->algorithm_result_cache.breathing_rate[minute_index] = ecg_result.resp_per_min_result;
        }
	}
	
	if(p_data_manage->ecg_ppg_time_s.second % 6 == 0 && p_data_manage->ecg_ppg_time_s.ecg_updata_cnt++ < 2)
	{
		uint32_t second_index = 0;
		uint32_t minute_index = 0;
		
		if(p_data_manage->ecg_ppg_time_s.second != 0)
		{
			second_index = (p_data_manage->ecg_ppg_time_s.second - 1) / 6;
            minute_index = (p_data_manage->ecg_ppg_time_s.minute) % 5;
		}
		else
		{
			second_index = 9;
            minute_index = (p_data_manage->ecg_ppg_time_s.minute - 1) % 5;
            
            if(p_data_manage->ecg_ppg_time_s.minute != 0)
            {
                minute_index = (p_data_manage->ecg_ppg_time_s.minute - 1) % 5;
            }
            else
            {
                minute_index = 4;
            }
		}
        
        if(ecg_result.rr_per_6sec_result >= 30
           && ecg_result.rr_per_6sec_result <= 210)
        {
            p_data_manage->algorithm_result_cache.hrv_value[minute_index*10 + second_index] = ecg_result.rr_per_6sec_result;
        }
		p_data_manage->algorithm_result_cache.hrv_type |= ((1 & 0x01) << minute_index);
	}
    // 诊断结果
    Ecg_GetDiagResult(&ecg_diag_result_s);
    
    if(p_ecg_measure->result_progress == 0 && ecg_diag_result_s.ecg_result_save_flag == 2)
    {
        p_ecg_measure->ecg_test_result_s.lead_off_type = ecg_diag_result_s.ecg_result_info_s.lead_off_type;
        memcpy(p_ecg_measure->ecg_test_result_s.diag_param_buf, &ecg_diag_result_s.ecg_result_info_s.diag_param1, 8);
        p_ecg_measure->ecg_test_result_s.heart_rate = ecg_diag_result_s.ecg_result_info_s.heart_rate;
        p_ecg_measure->ecg_test_result_s.resp_rate = ecg_diag_result_s.ecg_result_info_s.resp_rate;
        p_ecg_measure->ecg_test_result_s.hrv = ecg_diag_result_s.ecg_result_info_s.hrv;
        p_ecg_measure->ecg_test_result_s.QT_time = ecg_diag_result_s.ecg_result_info_s.QT_time;
        memcpy(p_ecg_measure->ecg_test_result_s.rese, &ecg_diag_result_s.ecg_result_info_s.rese1, 12);
    }
    /*
    if(ecg_diag_result_s.ecg_result_save_flag == 1 
      && p_data_manage->ecg_ppg_s.ecg_ppg_mode == MEASURE_MODE_PTT
      && (p_ecg_measure->btn == 1 && p_data_manage->ecg_ppg_s.ppg_measure_s.btn == 1))
    {   //自动测量
        Extflash_EcgWriteSet((uint8_t *)p_data_manage->data_pool.ecg_data, 250*6*4, 
                            (uint8_t *)&p_data_manage->ecg_ppg_s.ecg_measure_s.ecg_test_result_s, 25);
    }
    else*/ 
    if(ecg_save_flag == 0
       && ecg_diag_result_s.ecg_result_save_flag == 2 
       /*&& p_data_manage->ecg_ppg_s.ecg_measure_s.result_progress >= 30*/
       && p_data_manage->ecg_ppg_s.ecg_ppg_mode == MEASURE_MODE_ECG
       && p_ecg_measure->btn == 1)
    {   // 手动测量-按键
        // 保存结果 ECG,发送信号量

#if 0   // WJ.20190327.暂时屏蔽数据保存，新需求，出货           
        memcpy(&p_data_manage->ecg_result_data_s.ecg_result_data[0], &p_ecg_measure->ecg_test_result_s, 14);
        BleThread_SaveEcgResultDataSem();
#endif        
        // WJ.20190108.防止数据多次保存
        ecg_save_flag = 1;
    }
    
    // WJ.20190111.出结果逻辑修改    
    if(ecg_diag_result_s.ecg_result_save_flag == 2)
    {
        if(p_ecg_measure->result_progress == 0)
        {
            p_ecg_measure->e_test_result_prg1 = ecg_result.measurement_progress;
        }
        p_ecg_measure->result_progress = 30;
    }
    
    p_ecg_measure->e_test_result_prg = ecg_result.measurement_progress;
    
#if ECG_CALC_TIME_TEST
    test_calc_time1[test_time_cnt++] = (uint32_t)(xTaskGetTickCount()) - test_calc_time[test_time_cnt];  
    test_time_cnt = test_time_cnt%10;
#endif
    
#endif //#if (!SLEEP_CALC_DEBUG)    
}

/************************************************************************
** Function name:        Algorithm_PedometerTask
** Descriptions:         算法线程--记步算法任务
** input parameter:      void     
** Returned Value:       void
** Created  by:          GT
** Created  Datas:       2017-08-04
**************************************************************************/
//uint32_t move_5s = 0;
static void Algorithm_PedometerTask(void)
{
    int16_t         pedometer_data[3] = {0};
    static uint8_t  pedometer_counter = 0; 
	move_index_t    move_index_s = {0};
    
    const ped_param_set_t pp_param_s = {10, 2};     //10步起计，惯用手

    while(p_data_manage->acc.pedometer_data_len > 0)
    {
        DataManage_ProcessPedometerData(&pedometer_data[0], &pedometer_data[1], &pedometer_data[2]);
        Pedometer_Process(&pedometer_data[0], pedometer_counter++, 8, pp_param_s, sport_mode_para);
        
        // 注意:pedometer_counter是算法要求输入累加值的.
        if(pedometer_counter >= 25)
        {
            pedometer_counter = 0; 
        }
    }
    
	Move_IdexGet(&move_index_s);
    // 加速度工作，
    //move_5s = move_index_s.move_5s;
    if(Gsensor_IsMove() == 1 && move_index_s.move_5s >= 1)
    {
        Battery_GiveMove();
    }
}

/************************************************************************
** Function name:        Algorithm_GestureTask
** Descriptions:         算法线程--姿态算法任务
** input parameter:      无    
** Returned Value:       void
** Created  by:          GT
** Created  Datas:       2017-08-04
**************************************************************************/
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
        
        // !注意:此处因为涉及到翻腕灵敏度的参数问题, 所以需要对异常值做处理
        if((p_flash_param->turnwrist_s.sensitivity == 0)||(p_flash_param->turnwrist_s.sensitivity >= 10))
        {
            p_flash_param->turnwrist_s.sensitivity = 5;
        }
        
        Gesture_Detection16bit(p_acc_data->x_gesture_data, p_acc_data->y_gesture_data,   
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

/************************************************************************
** Function name:        Algorithm_SleepTask
** Descriptions:         算法线程--睡眠算法任务
** input parameter:      无     
** Returned Value:       void
** Created  by:          GT
** Created  Datas:       2017-08-04
**************************************************************************/
static void Algorithm_SleepTask(void)
{
#if SLEEP_CALC_ENABLED     
    // 如果正常使用, 则调用睡眠算法
    if(Battery_Get_Status() == BATTERY_POWER_NORMAL)
    {
        if(Battery_Get_Grade() != 0)
        {
            Sleep_NormalProcess();
        }
        else
        {
            // WJ.20181214.如果有睡眠,则退出睡眠
            Sleep_LowBateeryProcess(); 
        }
    }
    else
    {
        // 防止充电过程中有残留睡眠数据
        Vs_ClearInfoMsg();
    }
#endif    
}

/************************************************************************
** Function name:        Algorithm_SleepTraversingTask
** Descriptions:         算法线程--睡眠遍历算法任务
** input parameter:      无     
** Returned Value:       void
** Created  by:          GT
** Created  Datas:       2017-08-04
**************************************************************************/
static void Algorithm_SleepTraversingTask(void)
{
    alg_sleep_traversing = 1;
#if SLEEP_CALC_ENABLED     
    //Sleep_TraversingOperation();
    Sleep_TraversingOperationByRest();    
#endif    
    alg_sleep_traversing = 0;
}

/************************************************************************
** Function name:        Algorithm_SedentaryTask
** Descriptions:         算法线程--久坐算法任务
** input parameter:      void     
** Returned Value:       void
** Created  by:          GT
** Created  Datas:       2017-08-04
**************************************************************************/
static void Algorithm_SedentaryTask(void)
{
    
#if SLEEP_CALC_ENABLE    
    bool bRet = true;
//    static uint16_t continue_cnt = 0;
    static sedentary_config_t sedentary_config_bk = {0};
    uint32_t current_time = clock->hour*100 + clock->minutes;
    uint32_t sedentary_start = p_flash_param->sedentary_config_s.start_hour*100 + p_flash_param->sedentary_config_s.start_minute;
    uint32_t sedentary_end   = p_flash_param->sedentary_config_s.end_hour*100 + p_flash_param->sedentary_config_s.end_minute;
        
    if(p_flash_param->sit_func_state == 0)
    {        
        p_data_manage->sedentary_cnt = 0;
        return;
    }
    
    //WJ.20181127.充电重置久坐，信号量的添加删除了充电的判断标志
    if(Battery_Get_Status() != BATTERY_POWER_NORMAL)
    { 
        p_data_manage->sedentary_cnt = 0;
    }
    
    if(sedentary_config_bk.start_hour != p_flash_param->sedentary_config_s.start_hour
       || sedentary_config_bk.start_minute != p_flash_param->sedentary_config_s.start_minute
       || sedentary_config_bk.end_hour != p_flash_param->sedentary_config_s.end_hour
       || sedentary_config_bk.end_minute != p_flash_param->sedentary_config_s.end_minute
       || sedentary_config_bk.internal != p_flash_param->sedentary_config_s.internal)
    {
        sedentary_config_bk = p_flash_param->sedentary_config_s;
//        continue_cnt = 0;
        //YX.20180418.修复关机久坐计数不清零问题
        p_data_manage->sedentary_cnt = 0;
        bRet = false;
    }
    else
    {
        bRet = (p_flash_param->sedentary_config_s.enable == 1);
        bRet = bRet&&(sedentary_start <= current_time)&&(current_time <= sedentary_end);
        bRet = bRet&&(Array_ShortSum(p_data_manage->pedometer.step_count, 5) < 13);
        // !注意:为避免久坐误提醒, 此处需要添加时间限制 Add by GT 2017-10-23
        bRet = (bRet)&&(clock->hour >= 8)&&(clock->hour < 18);
        // !注意:新增如果在睡眠中, 则不能提醒久坐 Add by GT 2017-10-30
        bRet = (bRet)&&(p_data_manage->sleep_flag != 1);
        //不充电
        bRet = (bRet)&&(Battery_Get_Status() == BATTERY_POWER_NORMAL);
        //佩戴
        bRet = (bRet)&&(Get_NoWearStation() < 2 || Get_NoWearStation() == 5);
    }
    
    if(bRet)
    {
        //YX.20180418.修复关机久坐计数不清零问题
//        continue_cnt++;
        p_data_manage->sedentary_cnt++;
//        if(continue_cnt >= p_flash_param->sedentary_config_s.internal)
        if(p_data_manage->sedentary_cnt >= p_flash_param->sedentary_config_s.internal)
        {
//            continue_cnt = 0;
            p_data_manage->sedentary_cnt = 0;
            GuiThread_AddEvent(CPU_EVENT_TASK_SIT);
        }
    }
    else
    {
//        continue_cnt = 0;
        p_data_manage->sedentary_cnt = 0;
    }
#endif    
}

/************************************************************************
** Function name:        Algorithm_OledBrightnessAdjustTask
** Descriptions:         算法线程--OLED屏亮度实时调节函数
** input parameter:      void     
** Returned Value:       void
** Created  by:          GT
** Created  Datas:       2017-08-04
**************************************************************************/
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

/************************************************************************
** Function name:        Algorithm_AlarmMonitorTask
** Descriptions:         算法线程--闹铃监测任务
** input parameter:      无     
** Returned Value:       void
** Created  by:          GT
** Created  Datas:       2017-08-04
**************************************************************************/
static void Algorithm_AlarmMonitorTask(void)
{
#if (!APP_USED_OLD_ALARM_ENABLE)  
    
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
             //1、如果是单次闹铃, 则需要关闭
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
        
    // 如果有单次闹钟, 则不再遍历重复闹钟
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
    
    if(p_flash_param->dev_config_s.alarm_enable == 1 && alarm_is_arrive)
    {
        GuiThread_AddEvent(CPU_EVENT_TASK_ALARM);
    } 
#else    
   
    if((alg_minute == p_flash_param->alarm_old_time[1] && alg_hour == p_flash_param->alarm_old_time[0] && p_flash_param->alarm_old_time[2] == 0x01)
       ||(alg_minute== p_flash_param->alarm_old_time[4] && alg_hour == p_flash_param->alarm_old_time[3] && p_flash_param->alarm_old_time[5] == 0x01)
       ||(alg_minute== p_flash_param->alarm_old_time[7] && alg_hour == p_flash_param->alarm_old_time[6] && p_flash_param->alarm_old_time[8] == 0x01))
    {
        GuiThread_AddEvent(CPU_EVENT_TASK_ALARM);
    } 
    

#endif//#if (!APP_USED_OLD_ALARM_ENABLE)      
}

/****************************************************************
** Function name:           Alarm_IsOutTargetTime
** Descriptions:            单次闹钟时间是否已过, 用于APP设置闹钟校验，重启自检
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
        
        //1、如果是单次闹铃, 则需要关闭
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
        if(p_flash_param->alarm_time_s[index].valid == 1)
        {
            total_num++;
        }
    }
    
    return total_num;
    //return 0;
}

/************************************************************************
** Function name:        Algorithm_StepSportTraceTask
** Descriptions:         算法线程--记步运动量每分钟计算任务
** input parameter:      无     
** Returned Value:       void
** Created  by:          GT
** Created  Datas:       2017-08-04
**************************************************************************/
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
    
    //WJ.20190108.增加10分钟计步数据
    memcpy(&p_data_manage->pedometer.step_count_10m[0],
           &p_data_manage->pedometer.step_count_10m[1], 18);
    p_data_manage->pedometer.step_count_10m[9] = p_data_manage->pedometer.step_count[p_data_manage->pedometer.index];
    
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
        p_data_manage->pedometer.calories_count[p_data_manage->pedometer.index] = (uint16_t)((gsii_sport_info_s.posi_cal - p_data_manage->pedometer.calories_history_count)/100);
        ////p_data_manage->pedometer.calories_count[p_data_manage->pedometer.index] /= 100;
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
    
    // WJ.20190509.睡眠活动量
    if((p_flash_param->blood_oxygen_config_s.enable == 1)
       && (clock->hour < 7 || (clock->hour == 7 && clock->minutes < 5)))
    {
        p_data_manage->daily_attr.spo2_modification_flag[clock->minutes%5] = p_data_manage->pedometer.last_minute_sport;
    }
}

// 倒计时
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
                // 如果当前正处于倒计时显示界面, 则需要清除掉提醒界面
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

/**************************************************************
** Function name:           Algorithm_FatigueTask
** Descriptions:            算法线程--疲劳度算法
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2018-12-26
*****************************************************************/ 
//static void Algorithm_FatigueTask(void)
//{ 
//}
/***************************************************************
** Function name:           Algorithm_BloodOxygenTask
** Descriptions:            算法线程--血氧算法
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2018-12-26
*****************************************************************/ 

#define SPO2_TEST_ENABLED   0

#if SPO2_TEST_ENABLED
uint32_t write_reg_1182_times = 0;
uint8_t  red_save_adc_range[10] = {0};
uint8_t  ird_save_adc_range[10] = {0};
uint8_t  red_save_clk[10] = {0};
uint8_t  ird_save_clk[10] = {0};

uint8_t  red_i[10] = {0};
uint8_t  ird_i[10] = {0};

uint8_t  read_red_adc_range = 0;
uint8_t  read_ird_adc_range = 0;
uint8_t  read_red_clk = 0;
uint8_t  read_ird_clk = 0;
uint8_t  read_red_i = 0;
uint8_t  read_ird_i = 0;

uint32_t point_l = 0; 

uint32_t spo2_cnt_times = 0;
uint32_t spo2_calc_cnt = 0;

#endif

oxygen_measure_t        *p_oxygen_measure = NULL;  
static void Algorithm_BloodOxygenTask(void)
{    
#if SPO2_TEST_ENABLED    
    //test
    spo2_calc_cnt++;
#endif   
    
    detection_char_info_t   oxygen_detection = {0};
    bod_result_info_t       *p_bod_info = NULL;
    move_index_t            move_index_s = {0};
   
    p_oxygen_measure = &p_data_manage->ecg_ppg_s.oxygen_measure_s;

    p_bod_info = Get_OxygenInfo();
    Move_IdexGet(&move_index_s);
            
    // 血氧算法初始化
    if(p_oxygen_measure->ox_init == 1)
    {       
        p_oxygen_measure->ox_init = 0;
        memset(((uint8_t *)p_oxygen_measure) + 1, 0x00, sizeof(oxygen_measure_t) - 1);
        Blood_OxygenInit();
        
#if SPO2_TEST_ENABLED 		
        write_reg_1182_times = 0;
        spo2_cnt_times = 0;
        memset(red_save_adc_range, 0, 10 * sizeof(uint8_t));
        memset(ird_save_adc_range, 0, 10 * sizeof(uint8_t));
        memset(red_save_clk, 0, 10 * sizeof(uint8_t));
        memset(ird_save_clk, 0, 10 * sizeof(uint8_t));
#endif        
			
    }   
    oxygen_detection.bod_night_flag = p_oxygen_measure->ox_time;
    oxygen_detection.bod_gesture = Posture_GetSecPosture();//71;//每秒的姿态，暂时没改
    oxygen_detection.bod_move_situation = move_index_s.move_5s;//5秒运动量

    //err_code = 
    Blood_OxygenDetection(DataManage_GetRedDataPool(), DataManage_GetIredDataPool(), &oxygen_detection);
    
    // WJ.20190404.夜间模式下不生效.FWT科研需求
    if(p_bod_info->sig_satu_flag == 1 
       && p_data_manage->ecg_ppg_s.oxygen_measure_s.ox_time == 0)
    {
        // 重新开灯
        p_oxygen_measure->ox_init = 1;
        p_oxygen_measure->ox_rest = 1;  // 重新
        return;
    }
    
    
    {	// JSHs and FWT	
		p_oxygen_measure->bod_legal_flag = p_bod_info->bod_legal_flag;
		p_oxygen_measure->ir_prefusion = p_bod_info->ir_prefusion;
		p_oxygen_measure->r_prefusion = p_bod_info->r_prefusion;

#if SPO2_TEST_ENABLED 		
		if(spo2_cnt_times == 7)
		{
			point_l++;
		}
		spo2_cnt_times++;
		// 校准结果
		if(p_bod_info->write_reg_flag == 1 && p_bod_info->calib_1182_over_flag == 0)
		{
			red_save_adc_range[write_reg_1182_times] = p_bod_info->adc_range_red;
			ird_save_adc_range[write_reg_1182_times] = p_bod_info->adc_range_ird;
			red_save_clk[write_reg_1182_times] = p_bod_info->clk_div_red;
			ird_save_clk[write_reg_1182_times] = p_bod_info->clk_div_ird;
			
			red_i[write_reg_1182_times] = p_bod_info->i_red;
			ird_i[write_reg_1182_times] = p_bod_info->i_ird;
			
			write_reg_1182_times++;
		}
#endif        
        
        Si1182_CalibBloodOxygenGetPara(&oxygen_detection);
        
#if SPO2_TEST_ENABLED     
        read_red_adc_range = oxygen_detection.adc_range_red;
        read_ird_adc_range = oxygen_detection.adc_range_ird;
        read_red_clk = oxygen_detection.clk_div_red;
        read_ird_clk = oxygen_detection.clk_div_ird;
        read_red_i = oxygen_detection.i_red;
        read_ird_i = oxygen_detection.i_ird;
        
        if(read_red_adc_range != p_bod_info->adc_range_red
           || read_ird_adc_range != p_bod_info->adc_range_ird
           || read_red_clk != p_bod_info->clk_div_red
           || read_ird_clk != p_bod_info->clk_div_ird
           || read_red_i != p_bod_info->i_red
           || read_ird_i != p_bod_info->i_ird)
        {
            read_red_adc_range = 255;
        }
#endif        
    }
	
    if(p_bod_info->bo_degree > 50 && p_bod_info->bo_degree <= 99)
    {
        //实时血氧值
        p_oxygen_measure->ox_result = p_bod_info->bo_degree;
        if(p_oxygen_measure->ox_cnt < 300)
        {
            p_oxygen_measure->ox_time_result[p_oxygen_measure->ox_cnt++] = p_bod_info->bo_degree;
        }
        //p_oxygen_measure->ox_sum_result += p_bod_info->bo_degree;
        //p_oxygen_measure->ox_sum_cnt++;
    }
   
    /* // 每分钟血氧值由呼吸暂停给出
    // second为6的倍数
    static uint8_t ox_copy_second = 0;
	if(p_oxygen_measure->ox_time == 1 
       && p_data_manage->ecg_ppg_time_s.second == 54 && ox_copy_second != 54)
	{
		uint8_t minute_index = 0;
		
        minute_index = p_data_manage->ecg_ppg_time_s.minute % 5;
        p_data_manage->algorithm_result_cache.cache_oxgen_value[minute_index] = p_oxygen_measure->ox_sum_result/p_oxygen_measure->ox_sum_cnt;
        p_oxygen_measure->ox_sum_result = 0;
        p_oxygen_measure->ox_sum_cnt = 0;
	}
    ox_copy_second = p_data_manage->ecg_ppg_time_s.second;    
    */
    
    
    /* 不使用，测量的时候会调用ppg算法，由ppg任务处理佩戴
//////    // 是否调用佩戴结果处理 //佩戴算法条件:非黑人模式/血氧自动测量不在这里开启佩戴
//////    if((p_data_manage->ecg_ppg_s.oxygen_measure_s.ox_time == 0)
//////       && p_flash_param->dev_config_s.led_gear == 0 
//////       && (p_data_manage->ecg_ppg_s.ecg_ppg_mode == MEASURE_MODE_PPG 
//////           || p_data_manage->ecg_ppg_s.ecg_ppg_mode == MEASURE_MODE_OXYGEN))
//////    {
//////        xSemaphoreGive(alg_res_s.m_wear_sem); 
//////        xSemaphoreGive(alg_res_s.m_task_count); 
//////    }
    */
}

/****************************************************************
** Function name:           Algorithm_OsahsTask
** Descriptions:            算法线程--呼吸暂停算法
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2018-12-26
*****************************************************************/ 
static void Algorithm_OsahsTask(void)
{
    osahs_out_param_t   spo2_osahs_s = {0};
    osahs_entry_param_t osahs_entry_param_s = {0};
 
    Osahs_DetecInit();
    
    osahs_entry_param_s.spo2_data = p_data_manage->ecg_ppg_s.oxygen_measure_s.ox_time_result;
    osahs_entry_param_s.move_arr = p_data_manage->pedometer.sport_count;
    osahs_entry_param_s.spo2_arr_len = p_data_manage->ecg_ppg_s.oxygen_measure_s.ox_cnt % 301;
  
    

//	OSAHS_Detect(p_data_manage->ecg_ppg_s.oxygen_measure_s.ox_time_result,
//                 p_data_manage->ecg_ppg_s.oxygen_measure_s.ox_cnt % 301,
//	             p_data_manage->ecg_ppg_s.respiratory_rate_measure_s.resp_result_sec, 
//                 p_data_manage->ecg_ppg_s.respiratory_rate_measure_s.resp_cnt % 301,
//                 p_data_manage->pedometer.sport_count, 5);
    
    Osahs_DetecEntry(&osahs_entry_param_s, &spo2_osahs_s);
    
//    Get_OSAHSInfo(&spo2_osahs_s);
				
    // 更新呼吸暂停数据、血氧数据结果   
    p_data_manage->algorithm_result_cache.cache_breath_stop_cnt = spo2_osahs_s.osahs_times;//呼吸暂停次数，填充第一个，后面填0
    p_data_manage->algorithm_result_cache.cache_osahs_degree = spo2_osahs_s.osahs_degree;//呼吸暂停结果，填充第一个，后面填0
    p_data_manage->algorithm_result_cache.cache_low_spo2_time = spo2_osahs_s.osahs_cnt;  //低氧(缺氧)时间，填充第一个，后面填0    
    //5分钟血氧修正标志位，填充一样的值 
    //p_data_manage->algorithm_result_cache.cache_spo2_modification_flag = spo2_osahs_s.SPO2_modification_flag;
    p_data_manage->algorithm_result_cache.cache_oxgen_value[0] = spo2_osahs_s. spo2_one_min_aver[0];
    p_data_manage->algorithm_result_cache.cache_oxgen_value[1] = spo2_osahs_s. spo2_one_min_aver[1];
    p_data_manage->algorithm_result_cache.cache_oxgen_value[2] = spo2_osahs_s. spo2_one_min_aver[2];
    p_data_manage->algorithm_result_cache.cache_oxgen_value[3] = spo2_osahs_s. spo2_one_min_aver[3];
    p_data_manage->algorithm_result_cache.cache_oxgen_value[4] = spo2_osahs_s. spo2_one_min_aver[4];
    // 血氧低氧报警功能,报警条件:血氧连续10次以上低于80开始报警
    if((spo2_osahs_s.osahs_warning == 1) && (p_flash_param->blood_oxygen_config_s.enable == 1)
      && p_flash_param->dev_config_s.low_bo_alert_enable == 1)
    {
        GuiThread_AddEvent(CPU_EVENT_TASK_LOW_SPO2);
    } 
    
	
    // 重置缓存数
    p_data_manage->ecg_ppg_s.respiratory_rate_measure_s.resp_cnt = 0;
    p_data_manage->ecg_ppg_s.oxygen_measure_s.ox_cnt = 0;     
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
    
        // 久坐提醒任务
        Algorithm_SedentaryTask();
    }
    // 运动模式
    Movement_Monitor();
    
    //电池电量
    Algorithm_BatteryGradeTraceTask();
}

// 退出睡眠
static void Algorithm_ExitSleepTask(void)
{
    Sleep_LowBateeryProcess();
}

/************************************************************************
** Function name:        AlgorithmThread_Handle
** Descriptions:         算法线程--线程运行体 
** input parameter:      arg--无效参数    
** Returned Value:       void
** Created  by:          GT
** Created  Datas:       2017-08-04
**************************************************************************/

uint8_t  alg_run_cnt = 0;
uint8_t alg_stop_run_cnt = 0;

void Main_Init(void);
static void AlgorithmThread_Handle(void* arg)
{
    UNUSED_PARAMETER(arg);
     
/*	 
   //上电电量检测，确认是否启动正常程序,最大延时5分钟
    for(uint8_t i = 0; i < 5 * 60; i++)   
    {
        if(Battery_IsBootBatteryAdc())
        {
            break;
        }
        vTaskDelay(1000);
        App_Watch_Dog_Reload();
    }
    Main_Init();	 
*/	 
#if SLEEP_CALC_ENABLED     
    Sleep_Test();
    Sleep_TestWriteSleepInfoData();
    Sleep_TestWriteDailyData();
#endif
    
    alg_hour = clock->hour;
    alg_minute = clock->minutes;
    alg_second = clock->seconds;
    
    while(1)
    {	  	
        // 强制退出睡眠
        THREAD_MUTEX_TASK_EXCUTE(alg_res_s.m_exit_sleep_sem, alg_res_s.m_task_count, Algorithm_ExitSleepTask);
        
        // 1秒事件
        THREAD_MUTEX_TASK_EXCUTE(alg_res_s.m_one_seconds_sem, alg_res_s.m_task_count, Algorithm_OneScondsTask);
       
        alg_run_cnt = 1;
        // 姿态处理算法任务(姿态实时性要求比较高, 所以放在较前位置)
        THREAD_MUTEX_TASK_EXCUTE(alg_res_s.m_gesture_sem, alg_res_s.m_task_count, Algorithm_GestureTask);
       
        alg_run_cnt = 2;
        // 电池电量检测算法任务
        //THREAD_MUTEX_TASK_EXCUTE(alg_res_s.m_battery_sem, alg_res_s.m_task_count, Algorithm_BatteryGradeTraceTask);
        
        alg_run_cnt = 3;
        // 倒计时任务
        //THREAD_MUTEX_TASK_EXCUTE(alg_res_s.m_hourglass_sem, alg_res_s.m_task_count, Algorithm_HourglassTask);
       
        alg_run_cnt = 4;
        // 久坐算法监测任务
        //THREAD_MUTEX_TASK_EXCUTE(alg_res_s.m_sedentary_sem, alg_res_s.m_task_count, Algorithm_SedentaryTask);
        
        alg_run_cnt = 5;
        // OLED亮度实时计算监测任务
        //THREAD_MUTEX_TASK_EXCUTE(alg_res_s.m_brightness_sem, alg_res_s.m_task_count, Algorithm_OledBrightnessAdjustTask);
        
        alg_run_cnt = 6;
        // 闹铃实时监测任务
        //THREAD_MUTEX_TASK_EXCUTE(alg_res_s.m_alarm_sem, alg_res_s.m_task_count, Algorithm_AlarmMonitorTask);
        
        alg_run_cnt = 7;
        // 佩戴检测处理算法任务
        THREAD_MUTEX_TASK_EXCUTE(alg_res_s.m_wear_sem, alg_res_s.m_task_count, Algorithm_WearTask);
       
        alg_run_cnt = 8;
        // 脱手检测处理算法任务
        THREAD_MUTEX_TASK_EXCUTE(alg_res_s.m_no_wear_sem, alg_res_s.m_task_count, Algorithm_NoWearTask);
        
        alg_run_cnt = 9;
        // PPG处理算法任务
        THREAD_MUTEX_TASK_EXCUTE(alg_res_s.m_ppg_sem, alg_res_s.m_task_count, Algorithm_PpgTask);
        
        alg_run_cnt = 10;
        // ECG处理算法任务
        THREAD_MUTEX_TASK_EXCUTE(alg_res_s.m_ecg_measure_sem, alg_res_s.m_task_count, Algorithm_EcgTask);
        
        alg_run_cnt = 11;
        // 记步处理算法任务
        THREAD_MUTEX_TASK_EXCUTE(alg_res_s.m_pedometer_sem, alg_res_s.m_task_count, Algorithm_PedometerTask);
    
        alg_run_cnt = 12;
        // 睡眠算法任务
        THREAD_MUTEX_TASK_EXCUTE(alg_res_s.m_sleep_sem, alg_res_s.m_task_count, Algorithm_SleepTask);
        
        alg_run_cnt = 13;
        // 睡眠遍历算法任务
        THREAD_MUTEX_TASK_EXCUTE(alg_res_s.m_sleep_traversing_sem, alg_res_s.m_task_count, Algorithm_SleepTraversingTask);
        
        alg_run_cnt = 14;
        // 记步运动量计算任务
        THREAD_MUTEX_TASK_EXCUTE(alg_res_s.m_step_sport_trace_sem, alg_res_s.m_task_count, Algorithm_StepSportTraceTask);
        
        // 疲劳度处理算法任务
        //THREAD_MUTEX_TASK_EXCUTE(alg_res_s.m_fatigue_sem, alg_res_s.m_task_count, Algorithm_FatigueTask);
 
        alg_run_cnt = 15;
        // 血氧处理算法任务
        THREAD_MUTEX_TASK_EXCUTE(alg_res_s.m_blood_oxygen_measure_sem, alg_res_s.m_task_count, Algorithm_BloodOxygenTask);
          
        alg_run_cnt = 16;       
        // 呼吸暂停综合征算法任务
        THREAD_MUTEX_TASK_EXCUTE(alg_res_s.m_osahs_sem, alg_res_s.m_task_count, Algorithm_OsahsTask);
 
        alg_run_cnt = 17;  
            
        if(!((BleThread_GetBleNus()->conn_handle != BLE_CONN_HANDLE_INVALID)&&(p_data_manage->device_trace_param_s.sign_on == 1)) 
           || ComThread_GetSendFlashDataState() != 0)
        {
            App_Watch_Dog_Reload(); //1秒喂狗1次
        }            
        alg_run_cnt = 18; 
        /* !注意:这里需要判断算法任务的是否处理完成, 只有任务处理完成, 才能挂起线程
         *        如果这里返回pdFALSE时, 说明线程没有调度任务 
         */
        if(pdFALSE == xSemaphoreTake(alg_res_s.m_task_count, (TickType_t)0))
        {
            // 血氧算法数据处理
            alg_run_cnt = 0;
            vTaskSuspend(alg_res_s.m_thread_handle);
        }
        alg_stop_run_cnt = 0;
    }
}

/************************************************************************
** Function name:        AlgThread_Init
** Descriptions:         线程初始化功能, 主要完成以下初始化动作:
**                         1.创建算法所需信号量资源;
**                         2.数据变量初始化;
**                         3.完成线程的创建;
** input parameter:      无     
** Returned Value:       void
** Created  by:          GT
** Created  Datas:       2017-08-04
**************************************************************************/
void AlgThread_Init(void)
{
    // 创建所有的信号量资源
    //SEMAPHORE_CREATE_BINARY_RET(alg_res_s.m_battery_sem);
    //SEMAPHORE_CREATE_BINARY_RET(alg_res_s.m_hourglass_sem);
    //SEMAPHORE_CREATE_BINARY_RET(alg_res_s.m_sedentary_sem);
    SEMAPHORE_CREATE_BINARY_RET(alg_res_s.m_step_sport_trace_sem);
    //SEMAPHORE_CREATE_BINARY_RET(alg_res_s.m_alarm_sem);
    //SEMAPHORE_CREATE_BINARY_RET(alg_res_s.m_brightness_sem);

    SEMAPHORE_CREATE_BINARY_RET(alg_res_s.m_ecg_measure_sem);
    SEMAPHORE_CREATE_BINARY_RET(alg_res_s.m_ppg_sem);

    SEMAPHORE_CREATE_BINARY_RET(alg_res_s.m_no_wear_sem);
    SEMAPHORE_CREATE_BINARY_RET(alg_res_s.m_wear_sem);
    
    SEMAPHORE_CREATE_BINARY_RET(alg_res_s.m_pedometer_sem);
    SEMAPHORE_CREATE_BINARY_RET(alg_res_s.m_gesture_sem);
    //SEMAPHORE_CREATE_BINARY_RET(alg_res_s.m_fatigue_sem);
    SEMAPHORE_CREATE_BINARY_RET(alg_res_s.m_sleep_sem);
    SEMAPHORE_CREATE_BINARY_RET(alg_res_s.m_sleep_traversing_sem);
	
    SEMAPHORE_CREATE_BINARY_RET(alg_res_s.m_blood_oxygen_measure_sem);
    SEMAPHORE_CREATE_BINARY_RET(alg_res_s.m_osahs_sem);
    
    SEMAPHORE_CREATE_BINARY_RET(alg_res_s.m_one_seconds_sem);
    SEMAPHORE_CREATE_BINARY_RET(alg_res_s.m_exit_sleep_sem);
    
    // 创建计数信号量, 初始值为0, 信号量最大值为17
    alg_res_s.m_task_count = xSemaphoreCreateCounting(13, 0);
    if (alg_res_s.m_task_count == NULL)
    {
        APP_ERROR_HANDLER(NRF_ERROR_NO_MEM);
    }
    
#if SLEEP_CALC_ENABLED 				
    //初始化睡眠读取Flash回调函数
    Sleep_Init();
#endif
    
    DataManage_Init();
    
    //闹钟自检单次闹钟状态
    Algorithm_AlarmValidCheck();
    
    // 初始化获取亮度信息
    Algorithm_OledBrightnessAdjustTask();
            
    // 创建数据保存任务线程                              5.5K  5K
    if(xTaskCreate(AlgorithmThread_Handle, "Algorithm", 1408/*1280*//*1792*/, NULL, OXYGEN_ALGORITHM_THREAD_PRIORITY, &alg_res_s.m_thread_handle) != pdPASS)  
    {
        APP_ERROR_HANDLER(NRF_ERROR_NO_MEM);
    }
    
    xSemaphoreGive(alg_res_s.m_sleep_traversing_sem);
    xSemaphoreGive(alg_res_s.m_task_count); 
}

/************************************************************************
** Function name:        AlgThread_CalcSignal
** Descriptions:         算法唤醒任务信号处理
** input parameter:      无     
** Returned Value:       无
** Created  by:          WJ
** Created  Datas:       2018-10-18
**************************************************************************/
void AlgThread_CalcSignal(void)
{  
    // 调用PPG算法
    if(p_data_manage->data_pool.pool_update_second >= 1
       && (p_data_manage->ecg_ppg_s.ecg_ppg_mode == MEASURE_MODE_PPG))
    {
        xSemaphoreGive(alg_res_s.m_ppg_sem); 
        xSemaphoreGive(alg_res_s.m_task_count); 
    }
                                
    // 调用ECG算法
    if(p_data_manage->data_pool.pool_update_second >= 1 
       && (p_data_manage->ecg_ppg_s.ecg_ppg_mode == MEASURE_MODE_ECG))    
    {
        xSemaphoreGive(alg_res_s.m_ecg_measure_sem); 
        xSemaphoreGive(alg_res_s.m_task_count); 
    }
    
    // 调用血氧算法
    if(p_data_manage->data_pool.pool_update_second >= 1 
       && (p_data_manage->ecg_ppg_s.ecg_ppg_mode == MEASURE_MODE_OXYGEN))    
    {
        xSemaphoreGive(alg_res_s.m_blood_oxygen_measure_sem); 
        xSemaphoreGive(alg_res_s.m_task_count); 
                
        //if(p_data_manage->ecg_ppg_s.oxygen_measure_s.ox_time == 1)
        {
            xSemaphoreGive(alg_res_s.m_ppg_sem); 
            xSemaphoreGive(alg_res_s.m_task_count);         
        }
    }
    
    if(xSemaphoreTake(alg_res_s.m_task_count, (TickType_t)0) == pdTRUE)
    {
        // 上面做计数信号量(减一)处理, 这里需要加一还原
        xSemaphoreGive(alg_res_s.m_task_count);
        vTaskResume(alg_res_s.m_thread_handle);
    }   
}

/************************************************************************
** Function name:        AlgThread_SleepTraversing
** Descriptions:         判断是否需要进行睡眠算法遍历
** input parameter:      无     
** Returned Value:       无
** Created  by:          WJ
** Created  Datas:       2018-10-18
**************************************************************************/
void AlgThread_SleepTraversing(void)
{
    static uint32_t time_minutes = 0;
    
    //WJ.20181212.睡眠时间小于3小时重新遍历
    if(Api_GetSleepTotalTime(NULL, NULL, NULL) < 3*60)// != 0)
    {
        time_minutes = 0;
    }
    else
    {
        if(time_minutes == 0 || (clock->hour * 60 + clock->minutes - time_minutes) >= 5)
        {
            time_minutes = clock->hour * 60 + clock->minutes;
            THREAD_TASK_WAKE_UP(alg_res_s.m_sleep_traversing_sem, alg_res_s.m_task_count, alg_res_s.m_thread_handle);
        }
        else if(time_minutes > clock->hour * 60 + clock->minutes)
        {
            time_minutes = 0;
        }
    }
}

/************************************************************************
** Function name:        AlgThread_GsensorEventCommit
** Descriptions:         加速度数据处理，触发信号量
** input parameter:      无     
** Returned Value:       无
** Created  by:          WJ
** Created  Datas:       2018-10-18
**************************************************************************/
void AlgThread_GsensorEventCommit(void)
{
    uint8_t no_wear_flag = 0;
    move_index_t  m_move_index_s = {0};
    
    // 姿态数据长度判断
    if(p_data_manage->acc.gesture_data_len >= 5)
    {
        THREAD_TASK_WAKE_UP(alg_res_s.m_gesture_sem, alg_res_s.m_task_count, alg_res_s.m_thread_handle);
    }
                
    // 记步数据算法调用
    if(p_data_manage->acc.pedometer_data_len >= 25)
    {
        THREAD_TASK_WAKE_UP(alg_res_s.m_pedometer_sem, alg_res_s.m_task_count, alg_res_s.m_thread_handle);
    }   
    
    // 脱手检测处理
    no_wear_flag = Get_NoWearStation();
    Move_IdexGet(&m_move_index_s);
    if((no_wear_flag >= 2)&&(no_wear_flag <= 4)&&(m_move_index_s.move_5s >= 3))
    {
        p_data_manage->no_wear_measure_s.init = 1;
    }
}

/************************************************************************
** Function name:        AlgThread_PeriodCycleTask
** Descriptions:         线程内部任务
** input parameter:      无   
** Returned Value:       时间
** Created  by:          郭涛
** Created  Datas:       2017-08-04
**************************************************************************/
void AlgThread_PeriodCycleTask(uint8_t hour, uint8_t minute, uint8_t second)
{     
    alg_hour = hour;
    alg_minute = minute;
    alg_second = second;
    
    if(alg_stop_run_cnt < 10)
    {
        alg_stop_run_cnt++;
    }
    else
    {   // 算法停止10s ，设备重启
        if(alg_sleep_traversing == 0)
        {
            NVIC_SystemReset();
        }
        alg_stop_run_cnt  = 0;
    }
    
    if((second == 0)&&(p_data_manage->device_trace_param_s.sign_on == 1))
    {
        // 记步运动量跟踪计算任务
        xSemaphoreGive(alg_res_s.m_step_sport_trace_sem); 
        xSemaphoreGive(alg_res_s.m_task_count); 
    
        if(minute%5 == 0)
        {
            // 睡眠算法任务
            xSemaphoreGive(alg_res_s.m_sleep_sem);
            xSemaphoreGive(alg_res_s.m_task_count); 
        }		
    }

    if(Battery_Get_Status() == BATTERY_POWER_NORMAL)
    {        
        // !注意:脱手检测算法在xx:xx:20调用.
        if((minute%5 == 0)&&(second == 20)&&(p_data_manage->device_trace_param_s.sign_on == 1))
        {
            // 脱手检测算法任务
            xSemaphoreGive(alg_res_s.m_no_wear_sem);
            xSemaphoreGive(alg_res_s.m_task_count);		
            if(minute%10 == 5 || hour < 8)
            {
                p_data_manage->no_wear_measure_s.uncall_wear_cnt = 1;
            }
        }
        if(p_data_manage->device_trace_param_s.sign_on == 1)
        {
            Posture_Statistical();     //YX.20171229.添加姿态新的统计方式
        }
    }
    else
    {
        Posture_Statistical();     //YX.20171229.添加姿态新的统计方式
    }
    
    // 1秒事件
    xSemaphoreGive(alg_res_s.m_one_seconds_sem);
    xSemaphoreGive(alg_res_s.m_task_count); 
 
    // 定时唤醒任务线程
    vTaskResume(alg_res_s.m_thread_handle); 
    
    // WJ.20181031.增加spi休眠自检
    Gui_CheckEvent();
    
    if(second % 6 == 0)
    {
        p_data_manage->ecg_ppg_time_s.hour = hour;
        p_data_manage->ecg_ppg_time_s.minute = minute;
        p_data_manage->ecg_ppg_time_s.second = second;
        
        p_data_manage->ecg_ppg_time_s.ppg_updata_cnt = 0;
        p_data_manage->ecg_ppg_time_s.ecg_updata_cnt = 0;
        p_data_manage->ecg_ppg_time_s.ox_updata_cnt = 0; 
    }
    
    // 蓝牙连接时间
    if(p_data_manage->device_trace_param_s.ble_connect == 1)
    {
        p_data_manage->power_statistics_s.ble_connection_time++;
    }
}

/************************************************************************
** Function name:        AlgThread_Osahs
** Descriptions:         呼吸暂停，触发信号量
** input parameter:      无     
** Returned Value:       无
** Created  by:          WJ
** Created  Datas:       2018-12-28
**************************************************************************/
void AlgThread_Osahs(void)
{
    THREAD_TASK_WAKE_UP(alg_res_s.m_osahs_sem, alg_res_s.m_task_count, alg_res_s.m_thread_handle);
}

// 加入强制退出睡眠信号量
void AlgThread_ExitSleepSem(void)
{
    THREAD_TASK_WAKE_UP(alg_res_s.m_exit_sleep_sem, alg_res_s.m_task_count, alg_res_s.m_thread_handle);
}

