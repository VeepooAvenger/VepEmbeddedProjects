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
** Description:             �㷨�߳���Ҫ�����㷨��ص�����, ��Ҫ�㷨����:
**                          1.Ѫ���㷨
**                          2.�����㷨...                        
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
#define RESP_TEST_DEBUG     0   //���Ժ����ʣ�ͨ�����ʲ������ֵ


#define ECG_TEST_MODE_AUTOMATIC     0   //�Զ�����ģʽ
#define ECG_TEST_MODE_MANUAL        1   //�ֶ�����ģʽ

#define HEART_CACHE_LEN             10  //���ʸ澯�����С

static algorithm_thread_resource_t  alg_res_s;

static uint8_t      heart_h_cache[HEART_CACHE_LEN] = {0};  //���ʸ澯����

static uint8_t      alg_hour = 0;
static uint8_t      alg_minute = 0; 
static uint8_t      alg_second = 0;

static uint8_t      alg_sleep_traversing = 0;       // ˯�߱�����־


/************************************************************************
** Function name:        Algorithm_WearTask
** Descriptions:         �㷨�߳�--����㷨����
** input parameter:      ��     
** Returned Value:       ��
** Created  by:          ����
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

            // ��������ͨ��,����Ҫ�ص�AFE4900���ҳ�ʼ�����еĲ�������״̬
            if(p_data_manage->ecg_ppg_s.ecg_ppg_mode == MEASURE_MODE_PPG)
            {
                //�Զ������Ͱ�������״̬�����ﴦ��app����״̬�ڹرյ�ʱ����
                p_data_manage->ecg_ppg_s.read_data = 0;
                p_data_manage->ecg_ppg_s.ppg_measure_s.time = 0;
                p_data_manage->ecg_ppg_s.ppg_measure_s.btn = 0;
                p_data_manage->ecg_ppg_s.ecg_ppg_mode = MEASURE_MODE_NULL;
                BleThread_PeripheralsDevClose();
            }
            else if(p_data_manage->ecg_ppg_s.ecg_ppg_mode == MEASURE_MODE_OXYGEN)
            {
                //�Զ������Ͱ�������״̬�����ﴦ��app����״̬�ڹرյ�ʱ����
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
** Descriptions:         �㷨�߳�--���ּ���㷨����
**                       �������ּ���㷨��ÿ�����ִ��һ��, �����������㷨ִ�������Ҫ����������, 
**                       ��Ҫע��״̬��
** input parameter:      ��     
** Returned Value:       ��
** Created  by:          ����
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
    
    // ��ȡ�˶�����Ϣ
    p_data_manage->no_wear_measure_s.amount = Array_ShortSum(p_data_manage->pedometer.sport_count, 5);
    p_data_manage->no_wear_measure_s.step   = Array_ShortSum(p_data_manage->pedometer.step_count, 5);
    m_sport_history_s = Vnwd_HistoryMomentum(p_data_manage->no_wear_measure_s.amount, p_data_manage->no_wear_measure_s.step);
    
    // ���ּ�⺯��
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
    
    // !ע��:������Ҫ��ʼ����һ�ε����״̬ Modify by GT 2017-10-31
//    if(p_data_manage->no_wear_measure_s.uncall_wear_cnt++ >= 2)
    if(p_data_manage->no_wear_measure_s.uncall_wear_cnt == 1)
    {
        p_data_manage->no_wear_measure_s.uncall_wear_cnt = 0;
        p_data_manage->no_wear_measure_s.last_wear_status = 0;
    }
    
    // ��ȡ���
    p_data_manage->no_wear_measure_s.no_wear_result = Get_NoWearStation();
}

/************************************************************************
** Function name:        Algorithm_PpgTask
** Descriptions:         �㷨�߳�--PPG�㷨����
** input parameter:      ��     
** Returned Value:       ��
** Created  by:          WJ
** Created  Datas:       2018-10-18
**************************************************************************/
si1182_register_parameter_t     si1182_register_parameter_s = {0};      //1182���ò���

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
    static uint8_t      calc_ppg_seconds = 0;   //����ppg�㷨ʹ�ã�˯���㷨1

#if RESP_TEST_DEBUG    
    static uint8_t resp_flg = 0;
#endif
    
    p_ppg_measure = &p_data_manage->ecg_ppg_s.ppg_measure_s;
    p_resp_rate   = &p_data_manage->ecg_ppg_s.respiratory_rate_measure_s;
    
    // �����㷨��ʼ��
    if(p_ppg_measure->init == 1)
    {
        p_ppg_measure->init = 0;
        //WJ.20181024.��С��Ҫȥ�����ͷ����Ϣ,��Ҫ��Ҫָ������ת��
        memset(((uint8_t *)&p_data_manage->ecg_ppg_s.ppg_measure_s) + 1, 0x00, sizeof(ppg_measure_t) - 1);
		ppg_algorithm_cnt = 0;       
        // PPG�㷨��ʼ��
        Pwave_InfoInit();
        // ����Ѫѹ����ģʽ
        p_ppg_measure->calc_bp_init = 1;
        
        memset(heart_h_cache, 0, HEART_CACHE_LEN);
        
        calc_ppg_seconds = clock->seconds;

#if RESP_TEST_DEBUG         
        resp_flg = 0;
#endif
    }
    
    if(p_ppg_measure->calc_bp_init == 1)
    {   //Ѫѹ�㷨��ʼ��
        p_ppg_measure->calc_bp_init = 0;
        if(p_ppg_measure->app_bp == 1)
        {   // app����
            test_mode_s.test_mode = p_data_manage->app_bp_mode;    
        }
        else
        {
            test_mode_s.test_mode = p_flash_param->bp_s.mode;
        }
        test_mode_s.set_sp = p_flash_param->bp_s.private_high;
        test_mode_s.set_dp = p_flash_param->bp_s.private_low; 
        //��ʼ���ӿ�
        p_ppg_measure->bp_progress = 0;
    }
    
    Move_IdexGet(&move_index_s);
	ppg_algorithm_cnt++;
    if(ppg_algorithm_cnt > 300)
    {
        ppg_algorithm_cnt = 40;
    }
    // WJ.20190117.�Ż�ʱ����Ծ���¼��㲻�����������
    if((calc_ppg_seconds % 10 == 9) && (clock->seconds % 10 == 1))
    {
        calc_ppg_seconds = (calc_ppg_seconds + 1) % 60;
    }
    else
    {
        calc_ppg_seconds = clock->seconds;
    }
    
    // PPG�㷨����
	//ppg_err = 
    Pwave_InfoAlgorithm(DataManage_GetGreenDataPool(), 
                        DataManage_GetIredDataPool(),
                        NULL/*DataManage_GetAmbDataPool()*/, 
                        calc_ppg_seconds, ppg_algorithm_cnt, &move_index_s, 
                        &test_mode_s,
                        &si1182_register_parameter_s);	

	// PPG�㷨�Ľ����ȡ
	pwave_onemin_info = Get_PwaveInfo();
	// WJ.20190220.�����ֶ�Ѫ��ʱ��ֻ�������״̬
    {
        p_ppg_measure->p_wear_flag = pwave_onemin_info->wear_flag;              // ppg���״̬   
        
        // �Ƿ�������������� //����㷨����:�Ǻ���ģʽ
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
#if RESP_TEST_DEBUG       // ���Ժ����� 
     // ������
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
        p_ppg_measure->p_second_heart = pwave_onemin_info->hr_ppg_per_sec_result;   // ʵʱ����     
    }

#endif
    
    if(pwave_onemin_info->hr_ppg_per_min_result >= 30 && pwave_onemin_info->hr_ppg_per_min_result <= 180)
    {
        p_ppg_measure->p_minute_heart = pwave_onemin_info->hr_ppg_per_min_result;   // һ�������ʾ�ֵ
    }
    if(pwave_onemin_info->hrv_ppg_per_sec_result <= 210)
    {
        p_ppg_measure->p_second_hrv = pwave_onemin_info->hrv_ppg_per_sec_result;     // ÿ��hrv
    }
    if(pwave_onemin_info->rr_per_sec_result >= 30 && pwave_onemin_info->rr_per_sec_result <= 210)
    {
        p_ppg_measure->p_second_rr = pwave_onemin_info->rr_per_sec_result;      // ÿ��hrv 
    }
    if(pwave_onemin_info->rr_per_6sec_result >= 30 && pwave_onemin_info->rr_per_6sec_result <= 210)
    {
        p_ppg_measure->p_6sec_rr = pwave_onemin_info->rr_per_6sec_result;        // ÿ6��hrv 
    }
    
    p_ppg_measure->p_calc_cnt = ppg_algorithm_cnt;
    
    if(p_ppg_measure->app_bp == 1 || p_ppg_measure->btn_bp == 1)
    {
        if(p_ppg_measure->bp_progress < 200)
        {
            p_ppg_measure->bp_progress++;
        }
    }
    
    // Ѫѹ����
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
		
        // ��60�������⴦��1~5����,6~10���ӻ�����  
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
        
        // ���ฺ��
        if(pwave_onemin_info->heart_load_1m <= 100)
        {
            p_data_manage->algorithm_result_cache.cache_cardiac_load[minute_index] = pwave_onemin_info->heart_load_1m;
        }
        // ������
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
        //Ѫѹ�������
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
            //�ر�Ѫѹ�Զ�����
            Statemachine_DelState(MEAT_PPG_TIME);
        }
    }
    else if(p_flash_param->dev_config_s.heart_detect_enable == 1)
    {   // 10���ӻ����������
        if(p_flash_param->dev_config_s.ppg_detect_enable != 1
          && (p_ppg_measure->app == 0 && p_ppg_measure->app_bp == 0 && p_ppg_measure->btn == 0
             && p_ppg_measure->btn_bp == 0 && Movement_GetState() != MOVEMENT_STATE_RUN
             && ppg_algorithm_cnt > 20
             && p_data_manage->ecg_ppg_s.oxygen_measure_s.ox_time == 0))
        {
            //�ر������Զ�����
            Statemachine_DelState(MEAT_PPG_TIME);
        } 
        
        if(p_flash_param->dev_config_s.ppg_detect_enable != 1 && ppg_algorithm_cnt > 20)
        {   // �������ݵ��洢����
            if(pwave_onemin_info->hr_ppg_per_min_result >= 30 
              && pwave_onemin_info->hr_ppg_per_min_result <= 180)
            {
                p_data_manage->algorithm_result_cache.pulse_frequency[0] = pwave_onemin_info->hr_ppg_per_min_result;
            }       
        }
    }

    //���������ݴ���
    if(p_resp_rate->resp_init == 1)
    {   
        p_resp_rate->resp_init = 0;
        p_resp_rate->resp_cnt = 0;
    }
    if(p_resp_rate->resp_cnt < 300)
    {
        // ��������
        p_resp_rate->resp_result_sec[p_resp_rate->resp_cnt++] = pwave_onemin_info->resp_rate_1sec;
    }

    
    // ������Ҫ���³�ʼ��Ѫѹ�㷨
    //if(pwave_onemin_info->bp_flag == 2)
    //{   // ������Ч����Ҫ���³�ʼ�����㷨�Ż���㷨
        //Ѫѹ�㷨��ʼ�����
        //p_ppg_measure->calc_bp_init = 1;
    //}
    
    if(p_flash_param->dev_config_s.ppg_detect_enable == 1 
       && p_data_manage->ecg_ppg_time_s.second == 0
       && clock->seconds == 0)
    {    
        if(p_ppg_measure->app == 0 && p_ppg_measure->app_bp == 0 && p_ppg_measure->btn ==0
           && p_ppg_measure->btn_bp == 0 && Movement_GetState() != MOVEMENT_STATE_RUN)
        {   //���ʸ澯

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
    
    static uint8_t      ecg_save_flag = 0;      //���������������ݱ�־
    
    static uint8_t      ecg_wear_last_state = 0;    //��¼��һ�ε����״̬��
    
	p_ecg_measure = &p_data_manage->ecg_ppg_s.ecg_measure_s;
    
	if(p_ecg_measure->init == 1)
    {
        p_ecg_measure->init = 0;
        //WJ.20181024.��С��Ҫȥ�����ͷ����Ϣ,��Ҫ��Ҫָ������ת��
        memset(((uint8_t *)&p_data_manage->ecg_ppg_s.ecg_measure_s) + 1, 0x00, sizeof(ecg_measure_t) - 1);

        // ECG�㷨��ʼ��
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
//    {   //�Զ�����ģʽ��Ŀǰ��Ҫ�����PTTģʽ
//        ecg_tes_mode = ECG_TEST_MODE_AUTOMATIC;
//        
//        ecg_calc_cnt = (++ecg_calc_cnt) % 32;
//        ecg_calc_cnt = ecg_calc_cnt==0?1:ecg_calc_cnt;
//    }
//    else
    {  //������app�ֶ�����ģʽ,��������
        ecg_tes_mode = ECG_TEST_MODE_MANUAL;
        ecg_calc_cnt = ++ecg_calc_cnt;
    }
    
	Move_IdexGet(&move_index_s);
	
	ecg_enter.diga_mode = ecg_tes_mode;
	ecg_enter.func_times = ecg_calc_cnt;
    //memcpy(&ecg_enter.ecg_leadoff[0], &p_data_manage->data_pool.m_ecg_leadoff[0], 5);
    //������ֵ
    
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
    
	// ecg ������1s���ݴ���
    ////Reduce_EcgLdInterference(DataManage_GetEcgDataPoolLd(), ECG_CACHE_SIZE);
    // ECG�㷨����
//	ecg_err = 
    Ecg_DetectEntry(DataManage_GetEcgDataPool(), &move_index_s, ecg_enter);
	
//    if(ecg_err == 0)
//    {
//    }
//    else  
//    {
//    }

	// ECG�㷨�Ľ����ȡ
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
        p_ecg_measure->e_second_hrv = 0xFF;//Լ��255δĬ����Чֵ
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
    
    //��Ҫ�㷨������Ч����
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
            //����Ҫ��ȷ��δ���-���и�������
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
    // ��Ͻ��
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
    {   //�Զ�����
        Extflash_EcgWriteSet((uint8_t *)p_data_manage->data_pool.ecg_data, 250*6*4, 
                            (uint8_t *)&p_data_manage->ecg_ppg_s.ecg_measure_s.ecg_test_result_s, 25);
    }
    else*/ 
    if(ecg_save_flag == 0
       && ecg_diag_result_s.ecg_result_save_flag == 2 
       /*&& p_data_manage->ecg_ppg_s.ecg_measure_s.result_progress >= 30*/
       && p_data_manage->ecg_ppg_s.ecg_ppg_mode == MEASURE_MODE_ECG
       && p_ecg_measure->btn == 1)
    {   // �ֶ�����-����
        // ������ ECG,�����ź���

#if 0   // WJ.20190327.��ʱ�������ݱ��棬�����󣬳���           
        memcpy(&p_data_manage->ecg_result_data_s.ecg_result_data[0], &p_ecg_measure->ecg_test_result_s, 14);
        BleThread_SaveEcgResultDataSem();
#endif        
        // WJ.20190108.��ֹ���ݶ�α���
        ecg_save_flag = 1;
    }
    
    // WJ.20190111.������߼��޸�    
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
** Descriptions:         �㷨�߳�--�ǲ��㷨����
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
    
    const ped_param_set_t pp_param_s = {10, 2};     //10����ƣ�������

    while(p_data_manage->acc.pedometer_data_len > 0)
    {
        DataManage_ProcessPedometerData(&pedometer_data[0], &pedometer_data[1], &pedometer_data[2]);
        Pedometer_Process(&pedometer_data[0], pedometer_counter++, 8, pp_param_s, sport_mode_para);
        
        // ע��:pedometer_counter���㷨Ҫ�������ۼ�ֵ��.
        if(pedometer_counter >= 25)
        {
            pedometer_counter = 0; 
        }
    }
    
	Move_IdexGet(&move_index_s);
    // ���ٶȹ�����
    //move_5s = move_index_s.move_5s;
    if(Gsensor_IsMove() == 1 && move_index_s.move_5s >= 1)
    {
        Battery_GiveMove();
    }
}

/************************************************************************
** Function name:        Algorithm_GestureTask
** Descriptions:         �㷨�߳�--��̬�㷨����
** input parameter:      ��    
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
        
        // !ע��:�˴���Ϊ�漰�����������ȵĲ�������, ������Ҫ���쳣ֵ������
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
            if(0x01 == p_flash_param->turnwrist_s.enable)//������ˣ�ȫ�춼���Է���
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
        
        //̧�������ر�
        if((Gesture_GetCloseScreenFlag16bit())&&(turnwrist_start == true))
        {
            turnwrist_start = false;
            GuiThread_AddEvent(CPU_EVENT_TASK_TURNWRIST_STOP);
        }
    } 
}

/************************************************************************
** Function name:        Algorithm_SleepTask
** Descriptions:         �㷨�߳�--˯���㷨����
** input parameter:      ��     
** Returned Value:       void
** Created  by:          GT
** Created  Datas:       2017-08-04
**************************************************************************/
static void Algorithm_SleepTask(void)
{
#if SLEEP_CALC_ENABLED     
    // �������ʹ��, �����˯���㷨
    if(Battery_Get_Status() == BATTERY_POWER_NORMAL)
    {
        if(Battery_Get_Grade() != 0)
        {
            Sleep_NormalProcess();
        }
        else
        {
            // WJ.20181214.�����˯��,���˳�˯��
            Sleep_LowBateeryProcess(); 
        }
    }
    else
    {
        // ��ֹ���������в���˯������
        Vs_ClearInfoMsg();
    }
#endif    
}

/************************************************************************
** Function name:        Algorithm_SleepTraversingTask
** Descriptions:         �㷨�߳�--˯�߱����㷨����
** input parameter:      ��     
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
** Descriptions:         �㷨�߳�--�����㷨����
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
    
    //WJ.20181127.������þ������ź��������ɾ���˳����жϱ�־
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
        //YX.20180418.�޸��ػ�������������������
        p_data_manage->sedentary_cnt = 0;
        bRet = false;
    }
    else
    {
        bRet = (p_flash_param->sedentary_config_s.enable == 1);
        bRet = bRet&&(sedentary_start <= current_time)&&(current_time <= sedentary_end);
        bRet = bRet&&(Array_ShortSum(p_data_manage->pedometer.step_count, 5) < 13);
        // !ע��:Ϊ�������������, �˴���Ҫ���ʱ������ Add by GT 2017-10-23
        bRet = (bRet)&&(clock->hour >= 8)&&(clock->hour < 18);
        // !ע��:���������˯����, �������Ѿ��� Add by GT 2017-10-30
        bRet = (bRet)&&(p_data_manage->sleep_flag != 1);
        //�����
        bRet = (bRet)&&(Battery_Get_Status() == BATTERY_POWER_NORMAL);
        //���
        bRet = (bRet)&&(Get_NoWearStation() < 2 || Get_NoWearStation() == 5);
    }
    
    if(bRet)
    {
        //YX.20180418.�޸��ػ�������������������
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
** Descriptions:         �㷨�߳�--OLED������ʵʱ���ں���
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

    // ������Ҫ�������趨�����ж�
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
        //YX.20180111.������ʾ��ʱ��Բ��ϵ�����
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
** Descriptions:         �㷨�߳�--����������
** input parameter:      ��     
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
    
    // �ȱ�����������
    for(uint8_t index = 0; index < 20; index++)
    {
        p_current_alarm = &p_flash_param->alarm_time_s[index];
        if((clock->minutes == p_current_alarm->minute)&&(clock->hour == p_current_alarm->hour))
        {
             //1������ǵ�������, ����Ҫ�ر�
             if((1 == p_current_alarm->state)&&(0x00 == p_current_alarm->repeat))
             {
                  //������Ҫ�Ƚ�����
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
        
    // ����е�������, ���ٱ����ظ�����
    if(!alarm_is_arrive)
    {
        for(uint8_t index = 0; index < 20; index++)
        {
			 p_current_alarm = &p_flash_param->alarm_time_s[index];
			if((clock->minutes == p_current_alarm->minute)&&(clock->hour == p_current_alarm->hour))
			{
				 //2���ж��Ƿ����ظ�����
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
						 //���崥��
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
** Descriptions:            ��������ʱ���Ƿ��ѹ�, ����APP��������У�飬�����Լ�
** input parameters:        ��
** output parameters:       ��
** Returned value:          ����0:���ǵ������ӻ�������û��ʧЧ; 1:��������ʧЧ
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
** Descriptions:            �Լ쵥�����ӣ���ʱ����Ҫ����״̬
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
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
        
        //1������ǵ�������, ����Ҫ�ر�
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
** Descriptions:            ��ȡ���ӵ���Ч����
** input parameters:        ��
** output parameters:       ��
** Returned value:          ����:��Ч���Ӹ���
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
** Descriptions:         �㷨�߳�--�ǲ��˶���ÿ���Ӽ�������
** input parameter:      ��     
** Returned Value:       void
** Created  by:          GT
** Created  Datas:       2017-08-04
**************************************************************************/
static void Algorithm_StepSportTraceTask(void)
{
    sport_information_info_t gsii_sport_info_s = {0x00, };
    
    Get_SportInformationInfo(&gsii_sport_info_s);

    p_data_manage->pedometer.index = (p_data_manage->pedometer.index + 1)%5;
    
    // ���¼ǲ�����
    if(gsii_sport_info_s.posi_steps > p_data_manage->pedometer.step_history_count)
    {
        p_data_manage->pedometer.step_count[p_data_manage->pedometer.index] = (uint16_t)(gsii_sport_info_s.posi_steps - p_data_manage->pedometer.step_history_count);
    }
    else
    {
        p_data_manage->pedometer.step_count[p_data_manage->pedometer.index] = 0;
    }
    
    //WJ.20190108.����10���ӼƲ�����
    memcpy(&p_data_manage->pedometer.step_count_10m[0],
           &p_data_manage->pedometer.step_count_10m[1], 18);
    p_data_manage->pedometer.step_count_10m[9] = p_data_manage->pedometer.step_count[p_data_manage->pedometer.index];
    
    // ���¾�������
    if(gsii_sport_info_s.posi_distance > p_data_manage->pedometer.distance_history_count)
    {
        p_data_manage->pedometer.distance_count[p_data_manage->pedometer.index] = (uint16_t)(gsii_sport_info_s.posi_distance - p_data_manage->pedometer.distance_history_count);
    }
    else
    {
        p_data_manage->pedometer.distance_count[p_data_manage->pedometer.index] = 0;
    }
    
    // ���¿�·������
    if(gsii_sport_info_s.posi_cal > p_data_manage->pedometer.calories_history_count)
    {
        p_data_manage->pedometer.calories_count[p_data_manage->pedometer.index] = (uint16_t)((gsii_sport_info_s.posi_cal - p_data_manage->pedometer.calories_history_count)/100);
        ////p_data_manage->pedometer.calories_count[p_data_manage->pedometer.index] /= 100;
    }
    else
    {
        p_data_manage->pedometer.calories_count[p_data_manage->pedometer.index] = 0;
    }
    
    // �����˶�������
    if(Work_MeasureGet() >= p_data_manage->pedometer.sport_history_count)
    {
        p_data_manage->pedometer.sport_count[p_data_manage->pedometer.index] = (uint16_t)((Work_MeasureGet() - p_data_manage->pedometer.sport_history_count)>>12);
    }
    else
    {
        p_data_manage->pedometer.sport_count[p_data_manage->pedometer.index] = 0;
    }
    
    // ���¼ǲ��˶�����ʷ����
    p_data_manage->pedometer.step_history_count = gsii_sport_info_s.posi_steps;
    p_data_manage->pedometer.distance_history_count = gsii_sport_info_s.posi_distance;
    p_data_manage->pedometer.calories_history_count = (gsii_sport_info_s.posi_cal / 100) * 100;
    p_data_manage->pedometer.sport_history_count = Work_MeasureGet();
    
    // !ע��:������Ҫע��short���͸�ֵ���ݸ�char���� Modify by GT 2017-10-28
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
    
    // WJ.20190509.˯�߻��
    if((p_flash_param->blood_oxygen_config_s.enable == 1)
       && (clock->hour < 7 || (clock->hour == 7 && clock->minutes < 5)))
    {
        p_data_manage->daily_attr.spo2_modification_flag[clock->minutes%5] = p_data_manage->pedometer.last_minute_sport;
    }
}

// ����ʱ
static void Algorithm_HourglassTask(void)
{
    // ����ʱ���ѹ���
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
                // �����ǰ�����ڵ���ʱ��ʾ����, ����Ҫ��������ѽ���
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
** Descriptions:            �㷨�߳�--ƣ�Ͷ��㷨
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2018-12-26
*****************************************************************/ 
//static void Algorithm_FatigueTask(void)
//{ 
//}
/***************************************************************
** Function name:           Algorithm_BloodOxygenTask
** Descriptions:            �㷨�߳�--Ѫ���㷨
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
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
            
    // Ѫ���㷨��ʼ��
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
    oxygen_detection.bod_gesture = Posture_GetSecPosture();//71;//ÿ�����̬����ʱû��
    oxygen_detection.bod_move_situation = move_index_s.move_5s;//5���˶���

    //err_code = 
    Blood_OxygenDetection(DataManage_GetRedDataPool(), DataManage_GetIredDataPool(), &oxygen_detection);
    
    // WJ.20190404.ҹ��ģʽ�²���Ч.FWT��������
    if(p_bod_info->sig_satu_flag == 1 
       && p_data_manage->ecg_ppg_s.oxygen_measure_s.ox_time == 0)
    {
        // ���¿���
        p_oxygen_measure->ox_init = 1;
        p_oxygen_measure->ox_rest = 1;  // ����
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
		// У׼���
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
        //ʵʱѪ��ֵ
        p_oxygen_measure->ox_result = p_bod_info->bo_degree;
        if(p_oxygen_measure->ox_cnt < 300)
        {
            p_oxygen_measure->ox_time_result[p_oxygen_measure->ox_cnt++] = p_bod_info->bo_degree;
        }
        //p_oxygen_measure->ox_sum_result += p_bod_info->bo_degree;
        //p_oxygen_measure->ox_sum_cnt++;
    }
   
    /* // ÿ����Ѫ��ֵ�ɺ�����ͣ����
    // secondΪ6�ı���
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
    
    
    /* ��ʹ�ã�������ʱ������ppg�㷨����ppg���������
//////    // �Ƿ�������������� //����㷨����:�Ǻ���ģʽ/Ѫ���Զ������������￪�����
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
** Descriptions:            �㷨�߳�--������ͣ�㷨
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
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
				
    // ���º�����ͣ���ݡ�Ѫ�����ݽ��   
    p_data_manage->algorithm_result_cache.cache_breath_stop_cnt = spo2_osahs_s.osahs_times;//������ͣ����������һ����������0
    p_data_manage->algorithm_result_cache.cache_osahs_degree = spo2_osahs_s.osahs_degree;//������ͣ���������һ����������0
    p_data_manage->algorithm_result_cache.cache_low_spo2_time = spo2_osahs_s.osahs_cnt;  //����(ȱ��)ʱ�䣬����һ����������0    
    //5����Ѫ��������־λ�����һ����ֵ 
    //p_data_manage->algorithm_result_cache.cache_spo2_modification_flag = spo2_osahs_s.SPO2_modification_flag;
    p_data_manage->algorithm_result_cache.cache_oxgen_value[0] = spo2_osahs_s. spo2_one_min_aver[0];
    p_data_manage->algorithm_result_cache.cache_oxgen_value[1] = spo2_osahs_s. spo2_one_min_aver[1];
    p_data_manage->algorithm_result_cache.cache_oxgen_value[2] = spo2_osahs_s. spo2_one_min_aver[2];
    p_data_manage->algorithm_result_cache.cache_oxgen_value[3] = spo2_osahs_s. spo2_one_min_aver[3];
    p_data_manage->algorithm_result_cache.cache_oxgen_value[4] = spo2_osahs_s. spo2_one_min_aver[4];
    // Ѫ��������������,��������:Ѫ������10�����ϵ���80��ʼ����
    if((spo2_osahs_s.osahs_warning == 1) && (p_flash_param->blood_oxygen_config_s.enable == 1)
      && p_flash_param->dev_config_s.low_bo_alert_enable == 1)
    {
        GuiThread_AddEvent(CPU_EVENT_TASK_LOW_SPO2);
    } 
    
	
    // ���û�����
    p_data_manage->ecg_ppg_s.respiratory_rate_measure_s.resp_cnt = 0;
    p_data_manage->ecg_ppg_s.oxygen_measure_s.ox_cnt = 0;     
}

// 1���¼�����
static void Algorithm_OneScondsTask(void)
{
    // ����ʱ
    Algorithm_HourglassTask();
    if((alg_second == 0)&&(p_data_manage->device_trace_param_s.sign_on == 1))
    {
        // �������� 	    
        Algorithm_AlarmMonitorTask();
        // Oled���ȵ������� 
        Algorithm_OledBrightnessAdjustTask();
        // �ǲ��˶������ټ������� 
    
        // ������������
        Algorithm_SedentaryTask();
    }
    // �˶�ģʽ
    Movement_Monitor();
    
    //��ص���
    Algorithm_BatteryGradeTraceTask();
}

// �˳�˯��
static void Algorithm_ExitSleepTask(void)
{
    Sleep_LowBateeryProcess();
}

/************************************************************************
** Function name:        AlgorithmThread_Handle
** Descriptions:         �㷨�߳�--�߳������� 
** input parameter:      arg--��Ч����    
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
   //�ϵ������⣬ȷ���Ƿ�������������,�����ʱ5����
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
        // ǿ���˳�˯��
        THREAD_MUTEX_TASK_EXCUTE(alg_res_s.m_exit_sleep_sem, alg_res_s.m_task_count, Algorithm_ExitSleepTask);
        
        // 1���¼�
        THREAD_MUTEX_TASK_EXCUTE(alg_res_s.m_one_seconds_sem, alg_res_s.m_task_count, Algorithm_OneScondsTask);
       
        alg_run_cnt = 1;
        // ��̬�����㷨����(��̬ʵʱ��Ҫ��Ƚϸ�, ���Է��ڽ�ǰλ��)
        THREAD_MUTEX_TASK_EXCUTE(alg_res_s.m_gesture_sem, alg_res_s.m_task_count, Algorithm_GestureTask);
       
        alg_run_cnt = 2;
        // ��ص�������㷨����
        //THREAD_MUTEX_TASK_EXCUTE(alg_res_s.m_battery_sem, alg_res_s.m_task_count, Algorithm_BatteryGradeTraceTask);
        
        alg_run_cnt = 3;
        // ����ʱ����
        //THREAD_MUTEX_TASK_EXCUTE(alg_res_s.m_hourglass_sem, alg_res_s.m_task_count, Algorithm_HourglassTask);
       
        alg_run_cnt = 4;
        // �����㷨�������
        //THREAD_MUTEX_TASK_EXCUTE(alg_res_s.m_sedentary_sem, alg_res_s.m_task_count, Algorithm_SedentaryTask);
        
        alg_run_cnt = 5;
        // OLED����ʵʱ����������
        //THREAD_MUTEX_TASK_EXCUTE(alg_res_s.m_brightness_sem, alg_res_s.m_task_count, Algorithm_OledBrightnessAdjustTask);
        
        alg_run_cnt = 6;
        // ����ʵʱ�������
        //THREAD_MUTEX_TASK_EXCUTE(alg_res_s.m_alarm_sem, alg_res_s.m_task_count, Algorithm_AlarmMonitorTask);
        
        alg_run_cnt = 7;
        // �����⴦���㷨����
        THREAD_MUTEX_TASK_EXCUTE(alg_res_s.m_wear_sem, alg_res_s.m_task_count, Algorithm_WearTask);
       
        alg_run_cnt = 8;
        // ���ּ�⴦���㷨����
        THREAD_MUTEX_TASK_EXCUTE(alg_res_s.m_no_wear_sem, alg_res_s.m_task_count, Algorithm_NoWearTask);
        
        alg_run_cnt = 9;
        // PPG�����㷨����
        THREAD_MUTEX_TASK_EXCUTE(alg_res_s.m_ppg_sem, alg_res_s.m_task_count, Algorithm_PpgTask);
        
        alg_run_cnt = 10;
        // ECG�����㷨����
        THREAD_MUTEX_TASK_EXCUTE(alg_res_s.m_ecg_measure_sem, alg_res_s.m_task_count, Algorithm_EcgTask);
        
        alg_run_cnt = 11;
        // �ǲ������㷨����
        THREAD_MUTEX_TASK_EXCUTE(alg_res_s.m_pedometer_sem, alg_res_s.m_task_count, Algorithm_PedometerTask);
    
        alg_run_cnt = 12;
        // ˯���㷨����
        THREAD_MUTEX_TASK_EXCUTE(alg_res_s.m_sleep_sem, alg_res_s.m_task_count, Algorithm_SleepTask);
        
        alg_run_cnt = 13;
        // ˯�߱����㷨����
        THREAD_MUTEX_TASK_EXCUTE(alg_res_s.m_sleep_traversing_sem, alg_res_s.m_task_count, Algorithm_SleepTraversingTask);
        
        alg_run_cnt = 14;
        // �ǲ��˶�����������
        THREAD_MUTEX_TASK_EXCUTE(alg_res_s.m_step_sport_trace_sem, alg_res_s.m_task_count, Algorithm_StepSportTraceTask);
        
        // ƣ�Ͷȴ����㷨����
        //THREAD_MUTEX_TASK_EXCUTE(alg_res_s.m_fatigue_sem, alg_res_s.m_task_count, Algorithm_FatigueTask);
 
        alg_run_cnt = 15;
        // Ѫ�������㷨����
        THREAD_MUTEX_TASK_EXCUTE(alg_res_s.m_blood_oxygen_measure_sem, alg_res_s.m_task_count, Algorithm_BloodOxygenTask);
          
        alg_run_cnt = 16;       
        // ������ͣ�ۺ����㷨����
        THREAD_MUTEX_TASK_EXCUTE(alg_res_s.m_osahs_sem, alg_res_s.m_task_count, Algorithm_OsahsTask);
 
        alg_run_cnt = 17;  
            
        if(!((BleThread_GetBleNus()->conn_handle != BLE_CONN_HANDLE_INVALID)&&(p_data_manage->device_trace_param_s.sign_on == 1)) 
           || ComThread_GetSendFlashDataState() != 0)
        {
            App_Watch_Dog_Reload(); //1��ι��1��
        }            
        alg_run_cnt = 18; 
        /* !ע��:������Ҫ�ж��㷨������Ƿ������, ֻ�����������, ���ܹ����߳�
         *        ������ﷵ��pdFALSEʱ, ˵���߳�û�е������� 
         */
        if(pdFALSE == xSemaphoreTake(alg_res_s.m_task_count, (TickType_t)0))
        {
            // Ѫ���㷨���ݴ���
            alg_run_cnt = 0;
            vTaskSuspend(alg_res_s.m_thread_handle);
        }
        alg_stop_run_cnt = 0;
    }
}

/************************************************************************
** Function name:        AlgThread_Init
** Descriptions:         �̳߳�ʼ������, ��Ҫ������³�ʼ������:
**                         1.�����㷨�����ź�����Դ;
**                         2.���ݱ�����ʼ��;
**                         3.����̵߳Ĵ���;
** input parameter:      ��     
** Returned Value:       void
** Created  by:          GT
** Created  Datas:       2017-08-04
**************************************************************************/
void AlgThread_Init(void)
{
    // �������е��ź�����Դ
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
    
    // ���������ź���, ��ʼֵΪ0, �ź������ֵΪ17
    alg_res_s.m_task_count = xSemaphoreCreateCounting(13, 0);
    if (alg_res_s.m_task_count == NULL)
    {
        APP_ERROR_HANDLER(NRF_ERROR_NO_MEM);
    }
    
#if SLEEP_CALC_ENABLED 				
    //��ʼ��˯�߶�ȡFlash�ص�����
    Sleep_Init();
#endif
    
    DataManage_Init();
    
    //�����Լ쵥������״̬
    Algorithm_AlarmValidCheck();
    
    // ��ʼ����ȡ������Ϣ
    Algorithm_OledBrightnessAdjustTask();
            
    // �������ݱ��������߳�                              5.5K  5K
    if(xTaskCreate(AlgorithmThread_Handle, "Algorithm", 1408/*1280*//*1792*/, NULL, OXYGEN_ALGORITHM_THREAD_PRIORITY, &alg_res_s.m_thread_handle) != pdPASS)  
    {
        APP_ERROR_HANDLER(NRF_ERROR_NO_MEM);
    }
    
    xSemaphoreGive(alg_res_s.m_sleep_traversing_sem);
    xSemaphoreGive(alg_res_s.m_task_count); 
}

/************************************************************************
** Function name:        AlgThread_CalcSignal
** Descriptions:         �㷨���������źŴ���
** input parameter:      ��     
** Returned Value:       ��
** Created  by:          WJ
** Created  Datas:       2018-10-18
**************************************************************************/
void AlgThread_CalcSignal(void)
{  
    // ����PPG�㷨
    if(p_data_manage->data_pool.pool_update_second >= 1
       && (p_data_manage->ecg_ppg_s.ecg_ppg_mode == MEASURE_MODE_PPG))
    {
        xSemaphoreGive(alg_res_s.m_ppg_sem); 
        xSemaphoreGive(alg_res_s.m_task_count); 
    }
                                
    // ����ECG�㷨
    if(p_data_manage->data_pool.pool_update_second >= 1 
       && (p_data_manage->ecg_ppg_s.ecg_ppg_mode == MEASURE_MODE_ECG))    
    {
        xSemaphoreGive(alg_res_s.m_ecg_measure_sem); 
        xSemaphoreGive(alg_res_s.m_task_count); 
    }
    
    // ����Ѫ���㷨
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
        // �����������ź���(��һ)����, ������Ҫ��һ��ԭ
        xSemaphoreGive(alg_res_s.m_task_count);
        vTaskResume(alg_res_s.m_thread_handle);
    }   
}

/************************************************************************
** Function name:        AlgThread_SleepTraversing
** Descriptions:         �ж��Ƿ���Ҫ����˯���㷨����
** input parameter:      ��     
** Returned Value:       ��
** Created  by:          WJ
** Created  Datas:       2018-10-18
**************************************************************************/
void AlgThread_SleepTraversing(void)
{
    static uint32_t time_minutes = 0;
    
    //WJ.20181212.˯��ʱ��С��3Сʱ���±���
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
** Descriptions:         ���ٶ����ݴ��������ź���
** input parameter:      ��     
** Returned Value:       ��
** Created  by:          WJ
** Created  Datas:       2018-10-18
**************************************************************************/
void AlgThread_GsensorEventCommit(void)
{
    uint8_t no_wear_flag = 0;
    move_index_t  m_move_index_s = {0};
    
    // ��̬���ݳ����ж�
    if(p_data_manage->acc.gesture_data_len >= 5)
    {
        THREAD_TASK_WAKE_UP(alg_res_s.m_gesture_sem, alg_res_s.m_task_count, alg_res_s.m_thread_handle);
    }
                
    // �ǲ������㷨����
    if(p_data_manage->acc.pedometer_data_len >= 25)
    {
        THREAD_TASK_WAKE_UP(alg_res_s.m_pedometer_sem, alg_res_s.m_task_count, alg_res_s.m_thread_handle);
    }   
    
    // ���ּ�⴦��
    no_wear_flag = Get_NoWearStation();
    Move_IdexGet(&m_move_index_s);
    if((no_wear_flag >= 2)&&(no_wear_flag <= 4)&&(m_move_index_s.move_5s >= 3))
    {
        p_data_manage->no_wear_measure_s.init = 1;
    }
}

/************************************************************************
** Function name:        AlgThread_PeriodCycleTask
** Descriptions:         �߳��ڲ�����
** input parameter:      ��   
** Returned Value:       ʱ��
** Created  by:          ����
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
    {   // �㷨ֹͣ10s ���豸����
        if(alg_sleep_traversing == 0)
        {
            NVIC_SystemReset();
        }
        alg_stop_run_cnt  = 0;
    }
    
    if((second == 0)&&(p_data_manage->device_trace_param_s.sign_on == 1))
    {
        // �ǲ��˶������ټ�������
        xSemaphoreGive(alg_res_s.m_step_sport_trace_sem); 
        xSemaphoreGive(alg_res_s.m_task_count); 
    
        if(minute%5 == 0)
        {
            // ˯���㷨����
            xSemaphoreGive(alg_res_s.m_sleep_sem);
            xSemaphoreGive(alg_res_s.m_task_count); 
        }		
    }

    if(Battery_Get_Status() == BATTERY_POWER_NORMAL)
    {        
        // !ע��:���ּ���㷨��xx:xx:20����.
        if((minute%5 == 0)&&(second == 20)&&(p_data_manage->device_trace_param_s.sign_on == 1))
        {
            // ���ּ���㷨����
            xSemaphoreGive(alg_res_s.m_no_wear_sem);
            xSemaphoreGive(alg_res_s.m_task_count);		
            if(minute%10 == 5 || hour < 8)
            {
                p_data_manage->no_wear_measure_s.uncall_wear_cnt = 1;
            }
        }
        if(p_data_manage->device_trace_param_s.sign_on == 1)
        {
            Posture_Statistical();     //YX.20171229.�����̬�µ�ͳ�Ʒ�ʽ
        }
    }
    else
    {
        Posture_Statistical();     //YX.20171229.�����̬�µ�ͳ�Ʒ�ʽ
    }
    
    // 1���¼�
    xSemaphoreGive(alg_res_s.m_one_seconds_sem);
    xSemaphoreGive(alg_res_s.m_task_count); 
 
    // ��ʱ���������߳�
    vTaskResume(alg_res_s.m_thread_handle); 
    
    // WJ.20181031.����spi�����Լ�
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
    
    // ��������ʱ��
    if(p_data_manage->device_trace_param_s.ble_connect == 1)
    {
        p_data_manage->power_statistics_s.ble_connection_time++;
    }
}

/************************************************************************
** Function name:        AlgThread_Osahs
** Descriptions:         ������ͣ�������ź���
** input parameter:      ��     
** Returned Value:       ��
** Created  by:          WJ
** Created  Datas:       2018-12-28
**************************************************************************/
void AlgThread_Osahs(void)
{
    THREAD_TASK_WAKE_UP(alg_res_s.m_osahs_sem, alg_res_s.m_task_count, alg_res_s.m_thread_handle);
}

// ����ǿ���˳�˯���ź���
void AlgThread_ExitSleepSem(void)
{
    THREAD_TASK_WAKE_UP(alg_res_s.m_exit_sleep_sem, alg_res_s.m_task_count, alg_res_s.m_thread_handle);
}

