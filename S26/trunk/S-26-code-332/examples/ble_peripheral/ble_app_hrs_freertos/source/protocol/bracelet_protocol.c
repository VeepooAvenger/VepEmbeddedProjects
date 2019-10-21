/*******************Copyright(c)*********************************
** Veepoo Tech
** COPYRIGHT (C) 2016-2019
**----------------------FileInfo---------------------------------
** File name:                  bracelet_protocol.c
** Last modified Date:         2016-09-12
** Last SYS_VERSION:           0.0.1
** Descriptions:
**---------------------------------------------------------------
** Createdby:                  WJ
** Createddate:                2016-09-12
** SYS_VERSION:                0.0.1
** Descriptions:               �ֻ�Э��
**---------------------------------------------------------------
** Modified by:				
** Modified date:			
** SYS_VERSION:				
** Modified Descriptions:	
****************************************************************/

#include <stdarg.h>
#include "app_type.h"
#include "user_bootloader.h"
#include "bsp_led.h"
#include "AFE4900_entry.h"
#include "communication_thread.h"
#include "algorithm_thread.h"
#include "ble_thread.h"
#include "api_flash.h"
#include "data_manage.h"
#include "gui_thread.h"
#include "battery_adc.h"
#include "configuration.h"
#include "log.h"
#include "app_config_data.h"
#include "movement_patterns.h"
#include "configuration.h"
#include "gui_font_lattice.h"
#include "gui_traverse.h"

#include "afe4900_init.h"
#include "ble_conn_params.h"


// ��������
#define MAIN_STYLE_NUM          7

static void Protocol_SetDeviceTime(ble_nus_t * p_nus, uint8_t * p_data, uint16_t length);
static void Protocol_ReadDeviceTime(ble_nus_t *p_nus);
static void Protocol_GestureSwitch(ble_nus_t * p_nus, uint8_t * p_data);
static void Protocol_ReadWalkCnt(ble_nus_t * p_nus, uint8_t * p_data);
static void Protocol_ReadCurHeart(uint8_t * p_data);

static void Protocol_Upload_Ancs_Config(void);
static void Download_Ancs_Config(uint8_t* p_data, uint16_t length);
static void Protocol_Upload_Fun_Config(void);

extern uint8_t heart_init_cause;
extern uint8_t heart_calib_init_cause;
/****************************************************************
** Function name:             Upload_EnglishStyle_Config
** Descriptions:              �ϴ�������ʽ
** input parameters:          ��
** output parameters:         ��
** Returned value:            ��
** Created by:                WJ
** Created Date:              2016-09-12
****************************************************************/
void Upload_EnglishStyle_Config(void)
{
    uint8_t ack_data[20]={0xB8,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
                
    // ��Ӣ�Ʋ�����ȡ 
    if(p_flash_param->dev_config_s.english_style_enable == 0x01)
    {
         ack_data[2] = 0x02;
    }
    else 
    {
         ack_data[2] = 0x01;
    }
				
    // 12Сʱ/24Сʱ��ʽ
    if(p_flash_param->dev_config_s.am_pm_enable == 0x01)
    {
         ack_data[3] = 0x02;
    }
    else 
    {
         ack_data[3] = 0x01;
    }
            
    // ���ʿ��� 
    if(p_flash_param->dev_config_s.heart_detect_enable == 0x01)
    {
         ack_data[4] = 0x01;
    }
    else 
    {
         ack_data[4] = 0x02;
    }

    // Ѫѹ����
    if(p_flash_param->dev_config_s.blood_detect_enable == 0x01)
    {
        ack_data[5] = 0x01;
    }
    else if(p_flash_param->dev_config_s.blood_detect_enable == 0x00)
    {
        ack_data[5] = 0x02;
    }    

    
    // �����ֻ� 
    if(p_flash_param->dev_config_s.find_phone_enable == 0x01)
    {
         ack_data[8] = 0x01;
    }
    else 
    {
         ack_data[8] = 0x02;
    }
				
    if(p_flash_param->dev_config_s.watchtimer_enable == 1)
    {
         ack_data[9] = 1;
    }
    else 
    {
         ack_data[9] = 0x02;
    }
    
    if(p_flash_param->dev_config_s.low_bo_alert_enable == 1)
    {
         ack_data[10] = 1;
    }
    else 
    {
         ack_data[10] = 0x02;
    }
    
    if(p_flash_param->dev_config_s.led_gear == 1)
    {
         ack_data[11] = 0x02;
    }
    else 
    {
         ack_data[11] = 0x01;
    }
    
	/*
    if(p_flash_param->dev_config_s.ble_disconnect_enable == 1)
    {
         ack_data[14] = 1;
    }
    else 
    {
         ack_data[14] = 0x02;
    }
	*/
				
    //WJ.20181227.Ѫѹ��������
    if(p_flash_param->bp_func_state != 0x01)
    {
       ack_data[5] = 0; 
    }			
    Ble_SendDataViaBat(ack_data, 20);
}

/****************************************************************
** Function name:             Protocol_BloodOxygen
** Descriptions:              Э��Ѫ������
** input parameters:          p_nus��              �ṹ��ָ��
                              p_data��             ����ָ��
** output parameters:         ��
** Returned value:            ��
** Created by:                WJ
** Created Date:              2016-09-12
****************************************************************/
static void Protocol_BloodOxygen(ble_nus_t * p_nus, uint8_t * p_data)
{  
    uint8_t ble_ack[20] = {0x00,};
    float test_tmp1 = 0;
    
    ble_ack[0] = p_data[0];  // 0x80        
    ble_ack[1] = p_data[1];
    
//    memcpy(test_data, p_data, 20);
    if(p_data[1] == 1)   // Open
    {
        if(p_data[2] == 0x04)
        {
            p_data_manage->blood_oxygen_measure_s.test_mode = 4;
        }
        else if(p_data[2] == 0x05)
        {
            if(p_data[3] == 0 || p_data[3] == 1)
            {
                p_data_manage->blood_oxygen_measure_s.algorithm_flg = p_data[3];
            }
            else if(p_data[3] == 2 || p_data[3] == 3)
            {
                p_data_manage->blood_oxygen_measure_s.algorithm_flg = p_data[3] - 2;
                p_data_manage->blood_oxygen_measure_s.medical_param_s.ployfit_param_flag = p_data[7];
                p_data_manage->blood_oxygen_measure_s.medical_param_s.rechoose_ployfit_param_flag = p_data[8];
                p_data_manage->blood_oxygen_measure_s.medical_param_s.smooth_filter_times = p_data[9];
                p_data_manage->blood_oxygen_measure_s.medical_param_s.smooth_filter_points = p_data[10];
                test_tmp1 = p_data[11] + ((uint32_t)p_data[12]<<8) + ((uint32_t)p_data[13]<<16) + ((uint32_t)p_data[14]<<24);
                p_data_manage->blood_oxygen_measure_s.medical_param_s.ployfit_diff_value = test_tmp1/10.0f;
                p_data_manage->blood_oxygen_measure_s.medical_param_s.calc_times = (uint32_t)p_data[15] + ((uint32_t)p_data[16]<<8) + 
                                                                                   ((uint32_t)p_data[17]<<16) + ((uint32_t)p_data[18]<<24);
                if(Set_AppChangeLibValue(p_data_manage->blood_oxygen_measure_s.medical_param_s) == 1)
                {
                    //���óɹ�
                }
            }
            p_data_manage->blood_oxygen_measure_s.add_sub_flg = p_data[4];
            p_data_manage->blood_oxygen_measure_s.offse_buf = p_data[5];
            p_data_manage->blood_oxygen_measure_s.oxy_buf = p_data[6];

            return;
        }
        
		//�͵粻֧�ֲ���
		if(Battery_Get_Grade() == 0)
		{
			ble_ack[2] = 0x0A;
			Ble_SendDataViaBat(ble_ack, 20);
			
			return ;
		}
		
        if((p_data_manage->heart_measure_s.btn == 0)
           && (p_data_manage->blood_oxygen_measure_s.btn == 0) 
           && Movement_GetState() != MOVEMENT_STATE_RUN
           && (p_data_manage->bp_measure_s.bp_btn == 0))
        {	
            //��Ѫ������
            p_data_manage->blood_oxygen_measure_s.init = 1;
			p_data_manage->blood_oxygen_measure_s.cal_success = 0;
			p_data_manage->blood_oxygen_measure_s.cal_state = 0;
			p_data_manage->blood_oxygen_measure_s.cal_progress = 0;
            p_data_manage->blood_oxygen_measure_s.ox_day_time = 0; //�����ǲ��ǰ��쿪����
            p_data_manage->blood_oxygen_measure_s.ox_calc_cnt = 0; // У׼��ɺ󣬼�����������ڰ����Զ����� 
            p_data_manage->blood_oxygen_measure_s.ox_prompt_sign_flag = 0;
            BleThread_AFE4404BloodOxygenOpen();
        }
        if(p_data_manage->bp_measure_s.bp_btn == 1)
		{
            ble_ack[2] = 0x01;
            Ble_SendDataViaBat(ble_ack, 20);
		}
        else if(p_data_manage->heart_measure_s.btn == 1 || Movement_GetState() == MOVEMENT_STATE_RUN)
        {
            ble_ack[2] = 0x02;
            Ble_SendDataViaBat(ble_ack, 20);
        }
        else
        {
//			p_data_manage->upload = UPLOAD_ADC_OXYGEN_EVENT;
            p_data_manage->blood_oxygen_measure_s.app = 1;
            if(p_data_manage->blood_oxygen_measure_s.cal_success == 1 && p_data_manage->blood_oxygen_measure_s.cal_state == 0)
            {
                //����Ѫ��У׼���жϣ����У׼��ɣ�Ҫ�ظ�һ��100
                Ble_SendBatMulData(6,0x80,0x01,0x00,0x00,0x02,0x64);
            }
            //YX.20180319.���ʹ���������Բɼ�ԭʼ�������ݴ�������
            if(p_data_manage->upload == UPLOAD_ADC_OXYGEN_EVENT)
            {
                p_data_manage->data_pool.send_wait_len = (p_data_manage->data_pool.cache_update_cnt+0)%51;
                p_data_manage->data_pool.send_read_pos = 100;
            }
        }
    }
    else if(p_data[1] == 2)  //Close
    {        
        p_data_manage->blood_oxygen_measure_s.app = 0;
        if((p_data_manage->heart_measure_s.btn == 0)
          && (p_data_manage->blood_oxygen_measure_s.btn == 0)
          && Movement_GetState() != MOVEMENT_STATE_RUN
          && (p_data_manage->bp_measure_s.bp_btn == 0))
        {
            BleThread_AFE4404DevClose();
        }
    } 
}

/****************************************************************
** Function name:             Protocol_Fatigue
** Descriptions:              ƣ�Ͷȴ򿪹ر�����
** input parameters:          p_nus��              �ṹ��ָ��
                              p_data��             ����ָ��
** output parameters:         ��
** Returned value:            ��
** Created by:                WJ
** Created Date:              2016-12-07
****************************************************************/ 
static void Protocol_Fatigue(ble_nus_t * p_nus, uint8_t * p_data)
{
    uint8_t ble_ack[20] = {0x00,};
    ble_ack[0] = p_data[0];  // 0x81        
    ble_ack[1] = p_data[1];
    if(p_data[1] == 1)   // Open
    {
		//�͵粻֧�ֲ���
		if(Battery_Get_Grade() == 0)
		{
			ble_ack[2] = 0x0A;
			Ble_SendDataViaBat(ble_ack, 20);
			
			return ;
		}
		
        if((p_data_manage->heart_measure_s.btn == 0)
           && (p_data_manage->blood_oxygen_measure_s.btn == 0)
           && Movement_GetState() != MOVEMENT_STATE_RUN
           && (p_data_manage->bp_measure_s.bp_btn == 0))
        {
            p_data_manage->fatigue_measure_s.app = 1;
            p_data_manage->fatigue_measure_s.init = 1;
            BleThread_AFE4404FatigueOpen();
            
            AlgThread_OffTimeMeasurement();
        }   
        if(p_data_manage->bp_measure_s.bp_btn == 1)
		{
            ble_ack[2] = 0x01;
            Ble_SendDataViaBat(ble_ack, 20);
		}
        else if(p_data_manage->heart_measure_s.btn == 1 || Movement_GetState() == MOVEMENT_STATE_RUN)       
        {
            ble_ack[2] = 0x02;
            Ble_SendDataViaBat(ble_ack, 20);
        }
        else if(p_data_manage->blood_oxygen_measure_s.btn == 1)
        {
            ble_ack[2] = 0x04;
            Ble_SendDataViaBat(ble_ack, 20);
        }
    }
    else if(p_data[1] == 2)  //Close
    {
        p_data_manage->fatigue_measure_s.app = 0;
        if((p_data_manage->heart_measure_s.btn == 0)
           && (p_data_manage->blood_oxygen_measure_s.btn == 0)
           && (Movement_GetState() != MOVEMENT_STATE_RUN)
           && (p_data_manage->bp_measure_s.bp_btn == 0))
        {		
            BleThread_AFE4404DevClose();
        }
	    
    }         
}

/****************************************************************
** Function name:             Protocol_Respiration
** Descriptions:              �����ʴ򿪹ر�����
** input parameters:          p_nus��              �ṹ��ָ��
                              p_data��             ����ָ��
** output parameters:         ��
** Returned value:            ��
** Created by:                WJ
** Created Date:              2016-12-07
****************************************************************/ 
static void Protocol_Respiration(ble_nus_t * p_nus, uint8_t * p_data)
{
    uint8_t ble_ack[20] = {0x00,};
    ble_ack[0] = p_data[0];  // 0x82        
    ble_ack[1] = p_data[1];
    if(p_data[1] == 1)   // Open
    {
		//�͵粻֧�ֲ���
        if(Battery_Get_Grade() == 0)
		{
			ble_ack[2] = 0x0A;
			Ble_SendDataViaBat(ble_ack, 20);
			
			return ;
		}
		
        if((p_data_manage->heart_measure_s.btn == 0)&&(p_data_manage->blood_oxygen_measure_s.btn == 0)&&(Movement_GetState() != MOVEMENT_STATE_RUN))
        {	 
            p_data_manage->respiratory_rate_s.app = 1;
            p_data_manage->respiratory_rate_s.init = 1;
            p_data_manage->respiratory_rate_s.percent = 0;
            if(p_data[2] != 0)
            {
                //����ģʽ
                p_data_manage->respiratory_rate_s.mode = p_data[2];
                p_data_manage->upload = UPLOAD_ADC_OXYGEN_EVENT;
            }
            else
            {
                //����ģʽ
                p_data_manage->respiratory_rate_s.mode = 0;
            }
            BleThread_AFE4404BloodOxygenOpen();
        }  
        if(p_data_manage->bp_measure_s.bp_btn == 1)
		{
            ble_ack[2] = 0x01;
            Ble_SendDataViaBat(ble_ack, 20);
		}
        else if(p_data_manage->heart_measure_s.btn == 1 ||(Movement_GetState() == MOVEMENT_STATE_RUN))       
        {
            ble_ack[2] = 0x02;
            Ble_SendDataViaBat(ble_ack, 20);
        }
        else if(p_data_manage->blood_oxygen_measure_s.btn == 1)
        {
            ble_ack[2] = 0x04;
            Ble_SendDataViaBat(ble_ack, 20);
        }        
    }
    else if(p_data[1] == 2)  //Close
    {
        p_data_manage->respiratory_rate_s.app = 0;
        p_data_manage->respiratory_rate_s.mode = 0;
        p_data_manage->upload = UPLOAD_ADC_NULL_EVENT;
        if((p_data_manage->heart_measure_s.btn == 0)
           && (p_data_manage->blood_oxygen_measure_s.btn == 0)
           && (Movement_GetState() != MOVEMENT_STATE_RUN)
           && (p_data_manage->bp_measure_s.bp_btn == 0))
        {
            BleThread_AFE4404DevClose();
        }
    }         
}

/****************************************************************
** Function name:             Protocol_OledSet
** Descriptions:              Э��oled����
** input parameters:          p_nus��              �ṹ��ָ��
                              p_data��             ����ָ��
** output parameters:         ��
** Returned value:            ��
** Created by:                WJ
** Created Date:              2017-02-16
****************************************************************/ 
static void Protocol_OledSet(ble_nus_t * p_nus, uint8_t * p_data)
{
    uint8_t ble_ack[20] = {0x00,};
    brightness_config_t  st_bright = {0};
    
    ble_ack[0] = p_data[0];  // 0xB1    
    ble_ack[1] = 1;     
    ble_ack[2] = p_data[1];
    
    if(p_data[1] == 1)
    {
        st_bright.start_hour   = p_data[2];
        st_bright.start_minute = p_data[3];
        st_bright.end_hour     = p_data[4];
        st_bright.end_minute   = p_data[5];
        st_bright.valid_bright = p_data[6]; 
        st_bright.other_bright = p_data[7];    
        Api_UpdateBrightness(&st_bright);         
    }
    
    ble_ack[3] = p_flash_param->brightness_s.start_hour;
    ble_ack[4] = p_flash_param->brightness_s.start_minute;
    ble_ack[5] = p_flash_param->brightness_s.end_hour;
    ble_ack[6] = p_flash_param->brightness_s.end_minute;
    ble_ack[7] = p_flash_param->brightness_s.valid_bright;
    ble_ack[8] = p_flash_param->brightness_s.other_bright;
     
    //YX.20180111.������ʾ��ʱ��Բ��ϵ�����
//    DM_Update_Brightness(p_flash_param->brightness_s.valid_bright);
    Algorithm_OledBrightnessAdjustTask();
    GuiThread_BrightAdjust();
    
    Ble_SendDataViaBat(ble_ack, 20);   
}

/****************************************************************
** Function name:             Protocol_FindPhone
** Descriptions:              Э������ֻ�
** input parameters:          p_nus��              �ṹ��ָ��
                              p_data��             ����ָ��
** output parameters:         ��
** Returned value:            ��
** Created by:                WJ
** Created Date:              2017-02-16
****************************************************************/ 
static void Protocol_FindPhone(ble_nus_t * p_nus, uint8_t * p_data)
{
    //0xB5
    //CPU_EnhanceIdelMode();
}

/****************************************************************
** Function name:             Protocol_Camera
** Descriptions:              Э������
** input parameters:          p_nus��              �ṹ��ָ��
                              p_data��             ����ָ��
** output parameters:         ��
** Returned value:            ��
** Created by:                WJ
** Created Date:              2017-02-16
****************************************************************/ 
static void Protocol_Camera(ble_nus_t * p_nus, uint8_t * p_data)
{
	CPU_TASK_TYPE task_id_tmp = CPU_TASK_IDEL;
	
	task_id_tmp = GuiThread_GetTaskType();
    // ����ڳ��,�������������ģʽ
    if((Battery_Get_Status() != BATTERY_POWER_NORMAL)|| (GuiThread_IsDuringFindPhone())
	   || (task_id_tmp == CPU_TASK_OXYGEN) || (task_id_tmp == CPU_TASK_HEART) || Movement_IsBusy()
       || (GuiThread_GetEventType() == CPU_EVENT_TASK_CALL)
       || (task_id_tmp == CPU_TASK_BP))
    {
        Ble_SendBatMulData(3,0xB6,0x00,0x01);
        return;
    }
    
    if(p_data[1] == 1)
    {
        GuiThread_AddEvent(CPU_EVENT_TASK_CAMERA);
        Ble_SendBatMulData(3,0xB6,0x01,0x01);
    }
    else
    {
        GuiThread_ClearEvent(CPU_EVENT_TASK_CAMERA);
        Ble_SendBatMulData(3,0xB6,0x01,0x00);
    }
}

/****************************************************************
** Function name:             Protocol_Android_NotifySet
** Descriptions:              Android ֪ͨ����
** input parameters:          p_nus��              �ṹ��ָ��
                              p_data��             ����ָ��
** output parameters:         ��
** Returned value:            ��
** Created by:                WJ
** Created Date:              2017-02-16
****************************************************************/ 
static void Protocol_Android_NotifySet(ble_nus_t * p_nus, uint8_t * p_data)
{
    uint8_t ble_ack[20] = {0x00,};
    
    ble_ack[0] = p_data[0];
    
    ble_ack[1] = p_data[1];
    ble_ack[2] = 0x01;

    if(p_data[1] == 0 || p_data[1] == 2)//�ֻ��ر�0x00
    {
        MotorDelEvent();//�ر����
        p_data_manage->app_notify_s.call_content_s.mute_call = 0;
        // !ע��:������ֻ��˳����ܽ�,����������Ӧ�Ҷ�(ͼ����ʧ) Modify by GT 2017-11-1
        if(p_data_manage->app_notify_s.call_content_s.reject_call == 0)
        {
            p_data_manage->app_notify_s.call_content_s.hang_up_call = 1;
        }
        else
        {
            p_data_manage->app_notify_s.call_content_s.reject_suc = 1;
        }
    }
    
    Ble_SendDataViaBat(ble_ack, 20);   
}

/****************************************************************
** Function name:             Protocol_Android_NotifyEvent
** Descriptions:              Android ֪ͨ�¼�
** input parameters:          p_nus��              �ṹ��ָ��
                              p_data��             ����ָ��
** output parameters:         ��
** Returned value:            ��
** Created by:                WJ
** Created Date:              2017-02-16
****************************************************************/ 
static void Protocol_Android_NotifyEvent(ble_nus_t * p_nus, uint8_t * p_data)
{   
    static uint8_t title_done_flag = 0;
    uint8_t valid_package_len = p_data[2];
    valid_package_len = (valid_package_len > 14) ? 14 : valid_package_len;
    
    // �绰���ݴ���
    if(p_data[1] == 0)
    {
        call_notify_content_t  *p_call_notify = &p_data_manage->app_notify_s.call_content_s;

        // ��һ������
        if(p_data[4] == 0x01)
        {
            memset(p_call_notify, 0, sizeof(call_notify_content_t)); 
        }
        
        // ��Ҫ�жϻ����Ƿ���
        if(p_call_notify->len + valid_package_len >= sizeof(p_call_notify->content))
        {
             valid_package_len = sizeof(p_call_notify->content) - p_call_notify->len;
        }
        
        // ������Ϣ����
        memcpy(&p_call_notify->content[p_call_notify->len%128], &p_data[6], valid_package_len); 
        p_call_notify->len += valid_package_len;

        // �����ǰ�ǵ绰�������һ��,���������硣
        if(p_data[3] == p_data[4])
        {
            GuiThread_AddEvent(CPU_EVENT_TASK_CALL);
        }            
    }
    
    // ��Ϣ�������ݴ��� 
    if((p_data[1] >= 0x01)&&(p_data[1] <= 0x11))
    {
        uint8_t current_update_index = (p_data_manage->app_notify_s.update_index+1)%2;
        message_notify_content_t   *p_message_notify = &p_data_manage->app_notify_s.message_content_s[current_update_index];

        // ��Ϣ���͵�һ�����ݴ���
        if(p_data[4] == 0x01)
        {
            // ��ʼ���ṹ�����
            memset(p_message_notify, 0, sizeof(message_notify_content_t));
            
            // �����������
            if((p_data[16] == 0xEF)&&(p_data[17] == 0xBF)&&(p_data[18] == 0xBD)&&(p_data[19] == 0xEF))
            {
                valid_package_len = 10;
            }
            title_done_flag = 0;
        }
        
        if(p_data[1] == 0x01)
        {
            if(p_data[5] != 0x00 && p_data[5] != 0x01 && title_done_flag == 0)
            {
                title_done_flag = 1;
            }
            
            if(title_done_flag == 1)
            {
                title_done_flag = 2;
                p_message_notify->len = Font_FullScreenTotalActiveLenght(p_message_notify->content, p_message_notify->len);
                p_message_notify->content[p_message_notify->len] = 0x3A;
                p_message_notify->len++;
            }
        }
        
        // ��Ҫ�жϻ����Ƿ���
        if(p_message_notify->len + valid_package_len >= sizeof(p_message_notify->content))
        {
             valid_package_len = sizeof(p_message_notify->content) - p_message_notify->len;
        }
        
        memcpy(&p_message_notify->content[p_message_notify->len%230], &p_data[6], valid_package_len);
        p_message_notify->len += valid_package_len;

        // ��Ϣ���ͽ��� 
        if(p_data[3] == p_data[4])
        {
            /*
            // !ע��:Android��Ҫ��Ϣ����ʱ���ܸ�����ʾ������(���淽ʽ)
            p_message_notify->type = (MSG_NOTIFY_TYPE_E)p_data[1];
            //YX.2017.12.21.�޸���Ϣ����ΪSINA��FLICKRʱ����ʾ��UI���Ͳ��Ե����⣬Ӧ����ʾΪ��������
            if(p_data[1] == MSG_NOTIFY_SINA_ID || p_data[1] == MSG_NOTIFY_FLICKR_ID)
            {
                p_message_notify->type = MSG_NOTIFY_OTHER_ID;
            }
            */
            //YX.2018.04.16.�޸���Ϣ������ʾ��UIͼ�겻�Ե����⣬�����޸���ʾ�ײ��
            if(p_data[1] == 0x01)
            {
                p_message_notify->type = MSG_NOTIFY_SMS_ID;
            }
            else if(p_data[1] == 0x02)
            {
                p_message_notify->type = MSG_NOTIFY_WECHAT_ID;
            }
            else if(p_data[1] == 0x03)
            {
                p_message_notify->type = MSG_NOTIFY_QQ_ID;
            }
            //else if(p_data[1] == 0x04)
            //{
            //    p_message_notify->type = MSG_NOTIFY_SINA_ID;
            //}
            else if(p_data[1] == 0x05)
            {
                p_message_notify->type = MSG_NOTIFY_FACEBOOK_ID;
            }
            else if(p_data[1] == 0x06)
            {
                p_message_notify->type = MSG_NOTIFY_TWITTER_ID;
            }
            //else if(p_data[1] == 0x07)
            //{
            //    p_message_notify->type = MSG_NOTIFY_FLICKR_ID;
            //}
            else if(p_data[1] == 0x08)
            {
                p_message_notify->type = MSG_NOTIFY_LINKEDLN_ID;
            }
            else if(p_data[1] == 0x09)
            {
                p_message_notify->type = MSG_NOTIFY_WHATSAPP_ID;
            }
            else if(p_data[1] == 0x0A)
            {
                p_message_notify->type = MSG_NOTIFY_LINE_ID;
            }
            else if(p_data[1] == 0x0B)
            {
                p_message_notify->type = MSG_NOTIFY_INSTAGRAM_ID;
            }
            else if(p_data[1] == 0x0C)
            {
                p_message_notify->type = MSG_NOTIFY_SNAPCHAT_ID;
            }
            else if(p_data[1] == 0x0D)
            {
                p_message_notify->type = MSG_NOTIFY_SKYPE_ID;
            }
            else if(p_data[1] == 0x0E)
            {
                p_message_notify->type = MSG_NOTIFY_GMAIL_ID;
            }
            else
            {
                p_message_notify->type = MSG_NOTIFY_OTHER_ID;
            }
            p_data_manage->app_notify_s.update_index = (p_data_manage->app_notify_s.update_index+1)%2;
            p_data_manage->app_notify_s.unread_msg_count = 1;
            GuiThread_AddEvent(CPU_EVENT_TASK_MESSAGE);
        }        
    }        
}

///****************************************************************
//** Function name:           Protocol_UpdateUiMode
//** Descriptions:            ����UI����ģʽ
//** input parameters:        p_nus�����ݽ���ͨ����p_data:���յ�����
//** output parameters:       ��
//** Returned value:          ��
//** Created by:              YX             
//** Created Date:            2017-08-08
//**----------------------------------------------------------------
//** Modified by:
//** Modified date:
//**----------------------------------------------------------------
//*****************************************************************/ 
//static void Protocol_UpdateUiMode(ble_nus_t *p_nus, uint8_t *p_data)
//{
//    uint8_t ble_ack[20] = {0xC4,};
//    
//    ble_ack[1] = Gui_UpdateCheck(p_data);
//    
//    Ble_SendDataViaBat(ble_ack, 20);
//}

///****************************************************************
//** Function name:           Protocol_ReceiveUpdateUiData
//** Descriptions:            ���ܸ��µ�UI����
//** input parameters:        p_nus�����ݽ���ͨ����p_data:���յ�����
//** output parameters:       ��
//** Returned value:          ��
//** Created by:              YX             
//** Created Date:            2017-08-08
//**----------------------------------------------------------------
//** Modified by:
//** Modified date:
//**----------------------------------------------------------------
//*****************************************************************/ 
//static void Protocol_ReceiveUpdateUiData(ble_nus_t *p_nus, uint8_t *p_data)
//{
//    uint8_t ble_ack[20] = {0xC4,0x02};
//    
//    if(Gui_UpdateFlash(p_data))
//    {
//        Ble_SendDataViaBat(ble_ack, 20);
//    }
//    
//}

///****************************************************************
//** Function name:           Protocol_EraseUiData
//** Descriptions:            ����UI����ģʽ
//** input parameters:        p_nus�����ݽ���ͨ����p_data:���յ�����
//** output parameters:       ��
//** Returned value:          ��
//** Returned value:
//** Created by:              YX             
//** Created Date:            2017-08-08
//**----------------------------------------------------------------
//** Modified by:
//** Modified date:
//**----------------------------------------------------------------
//*****************************************************************/ 
//static void Protocol_EraseUiData(ble_nus_t *p_nus, uint8_t *p_data)
//{
//    uint8_t ble_ack[20] = {0xC5,};
//    
//    ble_ack[1] = p_data[1];
//    ble_ack[2] = Gui_EraseUiData(p_data);
//    
//    Ble_SendDataViaBat(ble_ack, 20);
//}

///****************************************************************
//** Function name:           Protocol_ReadUiData
//** Descriptions:            �����ȡUI����ģʽ
//** input parameters:        p_nus�����ݽ���ͨ����p_data:���յ�����
//** output parameters:       ��
//** Returned value:          ��
//** Created by:              YX             
//** Created Date:            2017-08-08
//**----------------------------------------------------------------
//** Modified by:
//** Modified date:
//**----------------------------------------------------------------
//*****************************************************************/ 
//static void Protocol_ReadUiData(ble_nus_t *p_nus, uint8_t *p_data)
//{
//    uint8_t ble_ack[20] = {0xC6,};
//    
//    ble_ack[1] = Gui_ReadUiData(p_data);
//    Ble_SendDataViaBat(ble_ack, 20);
//}

/****************************************************************
** Function name:           Protocol_SetUiStyle
** Descriptions:            ����UI���
** input parameters:        p_nus�����ݽ���ͨ����p_data:���յ�����
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ             
** Created Date:            2017-08-15
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
static void Protocol_SetUiStyle(ble_nus_t *p_nus, uint8_t *p_data)
{
    uint8_t ble_ack[20] = {0xC7, 0x00, };
	uint8_t ui_style = 0;
    
    ble_ack[1] = p_data[1];
    ble_ack[2] = 0x00;
	
//	App_GetUiStyleParam(&ui_style);
//	ble_ack[3] = ui_style;
    ui_style = Api_GetUiStyle();
    ble_ack[3] = ui_style;
	
    if(p_data[1] == 0x01)
    {
        if(p_data[2] < MAIN_STYLE_NUM)
        {
            ui_style = p_data[2]; 
			
            Api_UpdateUiStyle(ui_style);
			
            ble_ack[2] = 0x01;
			ble_ack[3] = ui_style;
            Gui_ThreadUpdateMian();
        } 			
    }
    else if(p_data[1] == 0x02)
    {
        ble_ack[2] = 0x01;
    }
	
    Ble_SendDataViaBat(ble_ack, 20);
}

/****************************************************************
** Function name:			Protocol_SetUiInfo
** Descriptions:            ����UI����
** input parameters:        p_nus�����ݽ���ͨ����p_data:���յ�����
** output parameters:       ��
** Returned value:          ��
** Created by:				YX             
** Created Date:           	2017-07-20
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
static void Protocol_SetUiInfo(ble_nus_t *p_nus, uint8_t *p_data)
{
    uint8_t ble_ack[20] = {0x00,};
    
    ble_ack[0] = p_data[0];     //0xCF
    ble_ack[1] = p_data[1];     //
    if(p_data[1] == 0x01)
    {
        /* ���� */
        ble_ack[2] = 1;
    }
    else if(p_data[1] == 0x00)
    {
        /* ���� */
        ble_ack[2] = 1;
    }
    
    Ble_SendDataViaBat(ble_ack, 20); 
}

/****************************************************************
** Function name:             Protocol_SwitchState
** Descriptions:              Э��ı�״̬
** input parameters:          p_nus��              �ṹ��ָ��
                              p_data��             ����ָ��
** output parameters:         ��
** Returned value:            ��
** Created by:                WJ
** Created Date:              2017-02-16
****************************************************************/ 
static void Protocol_SwitchState(ble_nus_t * p_nus, uint8_t * p_data)
{
    uint8_t ble_ack[20] = {0};
    dev_config_t  dev_config_s = {0};
    
    ble_ack[0] = p_data[0];     //0xB8
    ble_ack[1] = p_data[1];
    
    if(p_data[1] == 1)
    {
        memcpy(&dev_config_s,&p_flash_param->dev_config_s,sizeof(dev_config_t));
        
        // ��Ӣ�Ʋ�����ȡ
        if(p_data[2] == 0x01)
        {
            dev_config_s.english_style_enable = 0x00;
        }
        else if(p_data[2] == 0x02)
        {
            dev_config_s.english_style_enable = 0x01;
        }
								
        // 12Сʱ/24Сʱ��ʽ
        if(p_data[3] == 0x01)
        {
            dev_config_s.am_pm_enable = 0x00;
        }
        else if(p_data[3] == 0x02)
        {
            dev_config_s.am_pm_enable = 0x01;
        }
								
        // ���ʿ���
        if(p_data[4] == 0x01)
        {
            dev_config_s.heart_detect_enable = 0x01;
        }
        else if(p_data[4] == 0x02)
        {
            dev_config_s.heart_detect_enable = 0x00;
        }
        
        // Ѫѹ����
        if(p_data[5] == 0x01)
        {
            dev_config_s.blood_detect_enable = 0x01;
        }
        else if(p_data[5] == 0x02)
        {
            dev_config_s.blood_detect_enable = 0x00;
        }
        
        // �����ֻ�
        if(p_data[8] == 0x01)
        {
            dev_config_s.find_phone_enable = 1;
        }
        else if(p_data[8] == 0x02)
        {
            dev_config_s.find_phone_enable = 0;
        }
        
        // �����
        if(p_data[9] == 1)
        {
            dev_config_s.watchtimer_enable = 1;
        }
        else if(p_data[9] == 2)
        {
            dev_config_s.watchtimer_enable = 0;
        }
	 
        if(p_data[10] == 1)
        {
            dev_config_s.low_bo_alert_enable = 1;
        }
        else if(p_data[10] == 2)
        {
            dev_config_s.low_bo_alert_enable = 0;
        }
		
        if(p_data[11] == 1)
        {
            dev_config_s.led_gear = 0;
        }
        else if(p_data[11] == 2)
        {
            dev_config_s.led_gear = 1;
        }
        
		/*
        if(p_data[14] == 1)
        {
            dev_config_s.ble_disconnect_enable = 1;
        }
        else if(p_data[14] == 2)
        {
            dev_config_s.ble_disconnect_enable = 0;
        }
        */
        
        //WJ.20181227.Ѫѹ��������
        if(p_flash_param->bp_func_state != 0x01)
        {
           ble_ack[5] = 0; 
        }
        
        // ���±������ 
        Api_UpdateDevConfig(&dev_config_s);
        memcpy(ble_ack,p_data, 20);
        Ble_SendDataViaBat(ble_ack, 20); 
    }
    else if(p_data[1] == 2)
    {
        Upload_EnglishStyle_Config();
    } 
}

/****************************************************************
** Function name:             Protocol_MatchingAndBinding
** Descriptions:              IOS Matching the binding
** input parameters:          p_nus��              �ṹ��ָ��
                              p_data��             ����ָ��
** output parameters:         ��
** Returned value:            ��
** Created by:                WJ
** Created Date:              2017-02-16
****************************************************************/ 
static void Protocol_MatchingAndBinding(ble_nus_t * p_nus, uint8_t * p_data)
{
    uint8_t ble_ack[20] = {0x00,};
    
    ble_ack[0] = p_data[0];
    if(p_data[1] == 0x01)   //IOS Matching the binding
    {
        App_Ancs_Start();
        ble_ack[1] = 0x01;       
    }
    else if(p_data[1] == 0x00)  //Cancel the matching binding
    {
        ble_ack[1] = 0x01; 
    }
    ble_ack[2] = 0x01;
    Ble_SendDataViaBat(ble_ack, 20);    
}

/****************************************************************
** Function name:             Protocol_AndroidNotify
** Descriptions:              
** input parameters:          ��
** output parameters:         ��
** Returned value:            ��
** Created by:                WJ
** Created Date:              2017-02-16
****************************************************************/  
static void Protocol_AndroidNotify(ble_nus_t * p_nus, uint8_t * p_data)
{
    //android info deal
}

/****************************************************************
** Function name:             Protocol_Upload_Fun_Config
** Descriptions:              �ϴ�����
** input parameters:          ��
** output parameters:         ��
** Returned value:            ��
** Created by:                WJ
** Created Date:              2017-02-16
****************************************************************/ 
static void Protocol_Upload_Fun_Config(void)
{
    uint8_t ble_ack[20] = {0};
    
    // A7ָ���һ������
    ble_ack[0] = 0xA7;
    if(p_flash_param->bp_func_state == 0x01)
    {        
        ble_ack[1] = 0x01;  //Ѫѹ
    }
    else
    {
        ble_ack[1] = 0x00;  //Ѫѹ
    }
    ble_ack[2] = 0x00; //����
    ble_ack[3] = 0x01; //����
    ble_ack[4] = 0x01; //���ʱ���
    ble_ack[5] = 0x01; //΢���˶�
    ble_ack[6] = 0x01; //����
    ble_ack[7] = 0x01; //ƣ�Ͷ�
//    ble_ack[8] = 0x01; //Ѫ��
    ble_ack[8] = 0x02; //Ѫ��������У׼������
    ble_ack[9] = 0x11; //��Ϣ���Ѱ���
    ble_ack[10] = 0x01; //���ʱ���
    ble_ack[11] = 0x01; //������������
    ble_ack[12] = 0x00; //Ů��
    //ble_ack[13] = 0x01; //��Ļ��������
    ble_ack[14] = 0x00; //0xC3ָ�����
    ble_ack[15] = 0x01; //�ֻ����ȱ�־λ
    ble_ack[17] = 0x01; //������Э��  0x01  ٤�� = 0x01
    ble_ack[19] = 0x01; //����  
    Ble_SendDataViaBat(ble_ack, 20);   

    // A7ָ��ڶ�������
    memset(ble_ack,0,sizeof(ble_ack));
    ble_ack[0] = 0xA7;
    ble_ack[1] = 0x01; //����ʱ����
    ble_ack[3] = 0x04; //��Ϣ���ͱ�ע����
    ble_ack[5] = 0x03; //�˶�ģʽ
	ble_ack[6] = MAIN_STYLE_NUM; //֧��3�ַ��//9
    ble_ack[7] = 0x01; //������
    ble_ack[8] = 0x01; //HRV
    ble_ack[9] = 0x00; //����
    
    ////ble_ack[10] = 0x01; //��Э���ϴ���ʽ(�˶�ģʽ)
    
    ble_ack[11] = 0x01; //A5ָ������
    ble_ack[14] = 0x01; //������� 
    ble_ack[19] = 0x02; //����  
    Ble_SendDataViaBat(ble_ack, 20); 
}

/****************************************************************
** Function name:             Protocol_PasswordPair
** Descriptions:              ������Լ��ֻ�֧�ֹ��ܻظ�
** input parameters:          p_nus��              �ṹ��ָ��
                              p_data��             ����ָ��
                              lenght��             ���ݴ�С
** output parameters:         ��
** Returned value:            ��
** Created by:                WJ
** Created Date:              2017-02-16
****************************************************************/ 
static void Protocol_PasswordPair(ble_nus_t * p_nus, uint8_t * p_data, uint16_t length)
{   
    uint8_t ble_ack[20] = {0x00,};
    
    ble_ack[0] = 0xA1; 
	
    if(p_data[3] == 0x00)//���������׼���
    {
        if(p_flash_param->password != 256 * p_data[2] + p_data[1])//���벻��ȷ
        {
            p_data_manage->device_trace_param_s.code_passed = 0;
        }
        else
        {
            p_data_manage->device_trace_param_s.code_passed = 1;          
            if(0x00 == p_data[12])  // ANCS 
            {
                ble_gap_conn_params_t  ble_gap_conn_params_s = {0};
                ble_gap_conn_params_s.min_conn_interval = MIN_CONN_INTERVAL;
                ble_gap_conn_params_s.max_conn_interval = MAX_CONN_INTERVAL;
                ble_gap_conn_params_s.slave_latency     = SLAVE_LATENCY;
                ble_gap_conn_params_s.conn_sup_timeout  = CONN_SUP_TIMEOUT;
                ble_conn_params_change_conn_params(&ble_gap_conn_params_s);
                
                App_Ancs_Start();
            }
            else
            {   // ��׿
                ble_gap_conn_params_t  ble_gap_conn_params_s = {0};
                ble_gap_conn_params_s.min_conn_interval = MIN_CONN_INTERVAL_1;
                ble_gap_conn_params_s.max_conn_interval = MAX_CONN_INTERVAL_1;
                ble_gap_conn_params_s.slave_latency     = SLAVE_LATENCY_1;
                ble_gap_conn_params_s.conn_sup_timeout  = CONN_SUP_TIMEOUT_1;
                ble_conn_params_change_conn_params(&ble_gap_conn_params_s);
            }
            //��׼ʱ��

            if(SystemTime_Check(&p_data[4], sizeof(system_clock_time_t) - 1) == NRF_SUCCESS)
            {
                BleThread_AdjustTimeClearFlash(clock);
                SystemTime_Adjust(&p_data[4], sizeof(system_clock_time_t) - 1);
            }      
        }
        ble_ack[1] = p_data[1];
        ble_ack[2] = p_data[2];
        ble_ack[3] = (p_data_manage->device_trace_param_s.code_passed == 1)?0x06:0x00;
    }
    else if(p_data[3] == 0x01)  //set code
    {
        if(p_data_manage->device_trace_param_s.code_passed == 1)
        {
            Api_UpdateConnectPassword(p_data[1]+(p_data[2] << 8));
            ble_ack[3] = 0x03; //set code ok
        }
        else
        {
            ble_ack[3] = 0x02; //set code failed
        }
    }
    else if(p_data[3] == 0x02)  //read code
    {
        if(p_data_manage->device_trace_param_s.code_passed == 1)
        {
            ble_ack[1] = p_flash_param->password & 0xFF; 
            ble_ack[2] = (p_flash_param->password >> 8) & 0xFF; 
            ble_ack[3] = 0x05; //read code ok
        }
        else
        {
            ble_ack[3] = 0x04; //read code failed
        }        
    }
    
    ble_ack[4] = (uint8_t)((VEEPOO_DEVICE_ID_L>>8) & 0xFF);
    ble_ack[5] = (uint8_t)(VEEPOO_DEVICE_ID_L & 0xFF);
    ble_ack[6] = VERSION_M & 0xFF;
    ble_ack[7] = VERSION_H & 0xFF;
    ble_ack[8] = VERSION_L & 0xFF;   
    ble_ack[9] = VERSION_T & 0xFF;   
    ble_ack[10] = 0;
    ble_ack[11] = 0x01;   
    get_ble_addr(&ble_ack[12]);   
    ble_ack[18] = 0;           
    ble_ack[19] = (p_flash_param->dev_config_s.wear_detect_enable)?1:2;
    
    //����
    Protocol_Upload_Fun_Config();
    // ��������IOS���ƿ���ָ��
    Protocol_Upload_Ancs_Config();            
    //����״̬
    Upload_EnglishStyle_Config();
    
    Ble_SendDataViaBat( ble_ack, sizeof(ble_ack));    //WJ.2016.11.18  
}

/****************************************************************
** Function name:             Protocol_ReadInfoByOAD
** Descriptions:              ��ȡ��Ϣͨ��OAD
** input parameters:          p_nus��              �ṹ��ָ��
                              p_data��             ����ָ��
** output parameters:         ��
** Returned value:            ��
** Created by:                WJ
** Created Date:              2017-02-16
****************************************************************/ 
static void Protocol_ReadInfoByOAD(ble_nus_t * p_nus, uint8_t * p_data)
{
    //�˶�ģʽ�²�������
//    if(!Movement_IsBusy())
//    {
        //�ر����
        MotorDelEvent();
        
        jump_to_bootloader();  
//    }
}

/****************************************************************
** Function name:             Protocol_SynchronizationUserInfo
** Descriptions:              ͬ���û���Ϣ
** input parameters:          p_nus��              �ṹ��ָ��
                              p_data��             ����ָ��
** output parameters:         ��
** Returned value:            ��
** Created by:                WJ
** Created Date:              2017-02-16
****************************************************************/ 
static void Protocol_SynchronizationUserInfo(ble_nus_t * p_nus, uint8_t * p_data)
{
    uint8_t ble_ack[20] = {0xA3,0x01};
    person_infomation_t  person_s = {0};
    
    person_s.height = p_data[1];
    person_s.weight = p_data[2];
    person_s.age    = p_data[3];
    person_s.gender = (PERSON_GENDER_E)p_data[4];
    
    Api_UpdateUserInfomation(&person_s,(p_data[5]<<8)|(p_data[6]));
    
    Ped_SetInputPersonalInformation(person_s.height, person_s.weight, person_s.gender, person_s.age);
    
    //Phys_UpdataInfo();
    
    Ble_SendDataViaBat( ble_ack, sizeof(ble_ack));
}

/****************************************************************
** Function name:             Protocol_DeviceInfo
** Descriptions:              �����豸��Ϣ
** input parameters:          p_nus��              �ṹ��ָ��
                              p_data��             ����ָ��
** output parameters:         ��
** Returned value:            ��
** Created by:                WJ
** Created Date:              2017-02-16
****************************************************************/ 
static void Protocol_DeviceInfo(ble_nus_t * p_nus, uint8_t * p_data)
{
    uint8_t ble_ack[20] = {0x00,};
//    uint32_t sn = Api_GetDevSN();
   
    ble_ack[0] = p_data[0]; //0xA4
//    ble_ack[1] = (uint8_t)((sn>>8) & 0xFF);
//    ble_ack[2] = (uint8_t)(sn & 0xFF);
    
    Ble_SendDataViaBat(ble_ack, 20);  
}

/****************************************************************
** Function name:             Protocol_SetDeviceTime
** Descriptions:              �趨�豸ʱ��
** input parameters:          p_nus��              �ṹ��ָ��
                              p_data��             ����ָ��
                              length��             ���ݴ�С
** output parameters:         ��
** Returned value:            ��
** Created by:                WJ
** Created Date:              2017-02-16
****************************************************************/ 
static void Protocol_SetDeviceTime(ble_nus_t * p_nus, uint8_t * p_data, uint16_t length)
{
    uint8_t ble_ack[20] = {0x00,};
    uint8_t time_data[7] = {0,};
    dev_config_t  dev_config_s = {0};
    
    memcpy(&dev_config_s,&p_flash_param->dev_config_s,sizeof(dev_config_t));
    ble_ack[0] = p_data[0];//0xA5

    if(NRF_SUCCESS == SystemTime_Check(p_data+1,7))
    {                                                                                                         
        if(p_data_manage->device_trace_param_s.decryption_ok) //����ͨ��   
        {
            //2018.03.31.YX.A5����Э���޸�
            //WJ.20190110.��ʽ���÷��˵�����
            memcpy(&time_data[0], &p_data[1], 7);
            if(p_data[8] == 1)
            {
                dev_config_s.am_pm_enable = 0x01;
            }
            else if(p_data[8] == 2)
            {
                dev_config_s.am_pm_enable = 0x00;
            }
            Api_UpdateDevConfig(&dev_config_s);
        }
        else
        {
            time_data[0]=0x04;
            time_data[1]=0x57;                                                       
            time_data[2]=0x0B;
            time_data[3]=0x0B;                                                       
            time_data[4]=0x0B;
            time_data[5]=0x0B;
        }                            

        BleThread_AdjustTimeClearFlash(clock);
        SystemTime_Adjust(time_data,7);

        //sys_Flag.first_time_check_flag = TRUE;//�ֻ���ʱ���Ѿ���APPУ׼ͬ�� 
        ble_ack[1] = 0x01;//�ɹ�
    }
    else
    {
        ble_ack[1] = 0;//���ɹ�
    }
    Ble_SendDataViaBat(ble_ack, 20);    
}


/****************************************************************
** Function name:             Protocol_ReadDeviceTime
** Descriptions:              ��ȡ�豸ʱ��
** input parameters:          p_nus��              �ṹ��ָ��
** output parameters:         ��
** Returned value:            ��
** Created by:                WJ
** Created Date:              2017-02-16
****************************************************************/ 
static void Protocol_ReadDeviceTime(ble_nus_t *p_nus)
{
    uint8_t ble_ack[20] = {0x00,};
    
    ble_ack[0] = 0xA6;
    
    SystemTime_Read(&ble_ack[1],7);
    //2018.03.31.YX.A6����Э���޸�
    if(p_flash_param->dev_config_s.am_pm_enable == 0)
    {
        ble_ack[8] = 1;
    }
    else
    {
        ble_ack[8] = 2;
    }
    
    Ble_SendDataViaBat(ble_ack, 20);        //�ϴ�ʱ�� 
}

/****************************************************************
** Function name:             Protocol_GestureSwitch
** Descriptions:              ̧����������
** input parameters:          p_nus��              �ṹ��ָ��
                              p_data��             ����ָ��
** output parameters:         ��
** Returned value:            ��
** Created by:                WJ
** Created Date:              2017-02-16
****************************************************************/ 
static void Protocol_GestureSwitch(ble_nus_t * p_nus, uint8_t * p_data)
{
    uint8_t ble_ack[20] = {0x00,};
    turnwrist_config_t  st_turnwrist = {0};
    
    ble_ack[0] = p_data[0]; //0xAA
    if(p_data[1] == 0x00 || p_data[1] == 0x01)   // set Raised my hand against the bright screen to closed
    {
        if(!memcmp(&p_data[2],&p_data[4],2)        
            || p_data[4] > 23  || p_data[3] > 59 || p_data[5] > 59)
        {
            ble_ack[1] = 0;     //���ݲ�����
        }
        else
        {                 
            st_turnwrist.enable = p_data[1]; //ʹ��
            
            st_turnwrist.s_hour = p_data[2];
            st_turnwrist.s_minute = p_data[3]; 
            st_turnwrist.e_hour = p_data[4];
            st_turnwrist.e_minute = p_data[5];
            st_turnwrist.sensitivity = p_data[6]%10;             
            ble_ack[1] = 1;
            Api_UpdateTurnWristParam(&st_turnwrist);
        }
    }
    else if(p_data[1] == 0x02)  // read state of Raised my hand against the bright screen
    {
        ble_ack[1] = 1;
    }    
    
    ble_ack[2] = p_data[1]; 
    ble_ack[3] = p_flash_param->turnwrist_s.s_hour;
    ble_ack[4] = p_flash_param->turnwrist_s.s_minute;
    ble_ack[5] = p_flash_param->turnwrist_s.e_hour;
    ble_ack[6] = p_flash_param->turnwrist_s.e_minute;
    ble_ack[7] = (p_flash_param->turnwrist_s.enable==1)?1:2;
    ble_ack[8] = p_flash_param->turnwrist_s.sensitivity;
    ble_ack[9] = 5;
    Ble_SendDataViaBat(ble_ack, 20);    
}

/****************************************************************
** Function name:             Protocol_ANCSNotiryControl
** Descriptions:              ANCS֪ͨ����
** input parameters:          p_nus��              �ṹ��ָ��
                              p_data��             ����ָ��
** output parameters:         ��
** Returned value:            ��
** Created by:                WJ
** Created Date:              2017-02-16
****************************************************************/ 
static void Protocol_ANCSNotiryControl(ble_nus_t * p_nus, uint8_t * p_data)
{
    uint8_t ble_ack[20] = {0x00,};
    
    if(p_data[1] == 0x01)
    {    
        // ������Ӧ�ı��� 
        Download_Ancs_Config(p_data, 20);
        return;
    }
    else if(p_data[1] == 0x02)
    {
        // ��ȡ����
        Protocol_Upload_Ancs_Config();
        return;
    }
    else
    {
        p_data[1] = 0x00;
    }

    Ble_SendDataViaBat(ble_ack, 20);        //�ϴ�app
}

/****************************************************************
** Function name:             Download_Ancs_Config
** Descriptions:              ������������
** input parameters:          p_data��          ���ݵ�ַ
                              length��          ���ݴ�С
** output parameters:         ��
** Returned value:            ��
** Created by:                WJ
** Created Date:              2017-02-16
****************************************************************/ 
static void Download_Ancs_Config(uint8_t* p_data, uint16_t length)
{
    app_message_enable_t  st_message_enable = {0};
    uint8_t ack[20]={0xAD,0x00,0x00,0x01,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

    memcpy(ack,p_data,20);

    if(p_data[2] == 0x02)
    {
        st_message_enable.call = 0x00;
    }
    else if(p_data[2] == 0x01)
    {
        st_message_enable.call = 0x01;
    }
    
    if(p_data[3] == 0x02)
    {
        st_message_enable.sms = 0x00;
    }
    else if(p_data[3] == 0x01)
    {
        st_message_enable.sms = 0x01;
    }
				
    if(p_data[4] == 0x02)
    {
        st_message_enable.wechat = 0x00;
    }
    else if(p_data[4] == 0x01)
    {
        st_message_enable.wechat = 0x01;
    }
				
    if(p_data[5] == 0x02)
    {
        st_message_enable.qq = 0x00;
    }
    else if(p_data[5] == 0x01)
    {
        st_message_enable.qq = 0x01;
    }
					
    if(p_data[6] == 0x02)
    {
        st_message_enable.sina = 0x00;
    }
    else if(p_data[6] == 0x01)
    {
        st_message_enable.sina = 0x01;
    }
    
    if(p_data[7] == 0x02)
    {
        st_message_enable.facebook = 0x00;
    }
    else if(p_data[7] == 0x01)
    {
        st_message_enable.facebook = 0x01;
    }
				
    if(p_data[8] == 0x02)
    {
        st_message_enable.twitter = 0x00;
    }
    else if(p_data[8] ==  0x01)
    {
        st_message_enable.twitter = 0x01;
    }
				
    if(p_data[10] == 0x02)
    {
        st_message_enable.linkedln = 0x00;
    }
    else if(p_data[10] == 0x01)
    {
        st_message_enable.linkedln = 0x01;
    }
					
    if(p_data[11] == 0x02)
    {
        st_message_enable.whatsapp = 0x00;
    }
    else if(p_data[11] == 0x01)
    {
        st_message_enable.whatsapp = 0x01;
    }
				
    if(p_data[12] == 0x02)
    {
        st_message_enable.line = 0x00;
    }
    else if(p_data[12] == 0x01)
    {
        st_message_enable.line = 0x01;
    }
    
    if(p_data[13] == 0x02)
    {
        st_message_enable.instagram = 0x00;
    }
    else if(p_data[13] == 0x01)
    {
        st_message_enable.instagram = 0x01;
    }
				
    if(p_data[14] == 0x02)
    {
        st_message_enable.snapchat = 0x00;
    }
    else if(p_data[14] == 0x01)
    {
        st_message_enable.snapchat = 0x01;
    }
    
    if(p_data[15] == 0x02)
    {
        st_message_enable.skype = 0x00;
    }
    else if(p_data[15] == 0x01)
    {
        st_message_enable.skype = 0x01;
    }
    
    if(p_data[16] == 2)
    {
        st_message_enable.gmail = 0x00;
    }
    else if(p_data[16] == 1)
    {
        st_message_enable.gmail = 0x01;
    }
    
    if(p_data[19] == 2)
    {
        st_message_enable.other = 0x00;
    }
    else if(p_data[19] == 1)
    {
        st_message_enable.other = 0x01;
    }
	
    Api_UpdateMessageEnableConfig(&st_message_enable);
    Ble_SendDataViaBat( ack, 20);
}

/****************************************************************
** Function name:             Protocol_FindPhoneSet
** Descriptions:              �ֻ����ҹ��ܿ���
** input parameters:          p_nus��              �ṹ��ָ��
                              p_data��             ����ָ��
** output parameters:         ��
** Returned value:            ��
** Created by:                WJ
** Created Date:              2017-02-16
****************************************************************/  
static void Protocol_FindPhoneSet(ble_nus_t * p_nus, uint8_t * p_data)
{
//    uint8_t ble_ack[20] = {0x00, };
//    
//    ble_ack[0] = p_data[0];
//    if(p_data[1] == 0)
//    {
//        system_info.find_phone = 0x02;
//        App_SystemInfoSave_Set();
//    }
//    else if(p_data[1] == 1)
//    {
//        system_info.find_phone = 0x01;
//        App_SystemInfoSave_Set();
//    }
//    ble_ack[1] = 0x01;
//    ble_ack[2] = p_data[1];
//    ble_ack[2] = system_info.find_phone;
//    
//    Ble_SendDataViaBat( ble_ack, 20);        //�ϴ�app    
}

/****************************************************************
** Function name:             Protocol_Upload_Ancs_Config
** Descriptions:              �ϱ���Ϣ����״̬
** input parameters:          ��
** output parameters:         ��
** Returned value:            ��
** Created by:                WJ
** Created Date:              2017-02-16
****************************************************************/ 
static void Protocol_Upload_Ancs_Config(void)
{
    uint8_t ble_ack[20]={0xAD,0x02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
        
    // ��ȡ�绰������Ϣ 
    if(p_flash_param->message_enable_s.call == 1)
    {
        ble_ack[2] = 0x01;
    }
    else 
    {
        ble_ack[2] = 0x02;
    }
    // ��ȡ����������Ϣ 
    if(p_flash_param->message_enable_s.sms == 1)
    {
        ble_ack[3] = 0x01;
    }
    else 
    {
        ble_ack[3] = 0x02;
    }
    // ��ȡ΢��������Ϣ 
    if(p_flash_param->message_enable_s.wechat == 1)
    {
        ble_ack[4] = 0x01;
     }
    else
    {		
        ble_ack[4] = 0x02;
    }		
    // ��ȡqq������Ϣ 
    if(p_flash_param->message_enable_s.qq == 1)
    {
        ble_ack[5] = 0x01;
    }
    else 
    {
        ble_ack[5] = 0x02;
    }
    // ��ȡFacebook������Ϣ
    if(p_flash_param->message_enable_s.facebook == 1)
    {
        ble_ack[7] = 0x01;
    }
    else 
    {
        ble_ack[7] = 0x02;
    }
    
    // ��ȡTwitter������Ϣ
    if(p_flash_param->message_enable_s.twitter == 1)
    {
        ble_ack[8] = 0x01;
    }
    else
    {
        ble_ack[8] = 0x02;
    }
    
    if(p_flash_param->message_enable_s.linkedln == 1)
    {
        ble_ack[10] = 0x01;
    }
    else 
    {
        ble_ack[10] = 0x02;
    }
    
    /* ��ȡWhatsApp������Ϣ */
    if(p_flash_param->message_enable_s.whatsapp == 1)
    {
        ble_ack[11] = 0x01;
    }
    else
    {
        ble_ack[11] = 0x02;
    }
    
    if(p_flash_param->message_enable_s.line == 1)
    {
        ble_ack[12] = 0x01;
    }
    else
    {		
        ble_ack[12] = 0x02;
    }		
    if(p_flash_param->message_enable_s.instagram == 1)
    {
        ble_ack[13] = 0x01;
    }
    else
    {		
        ble_ack[13] = 0x02;
    }
    
    if(p_flash_param->message_enable_s.snapchat == 1)
    {
        ble_ack[14] = 0x01;
    }
    else 
    {
        ble_ack[14] = 0x02;
    }
    
    if(p_flash_param->message_enable_s.skype == 1)
    {
        ble_ack[15] = 0x01;
    }
    else
    {		
        ble_ack[15] = 0x02;
    }
    if(p_flash_param->message_enable_s.gmail == 1)
    {
        ble_ack[16] = 0x01;
    }
    else 
    {
        ble_ack[16] = 0x02;
    }
    
    if(p_flash_param->message_enable_s.other == 1)
    {
        ble_ack[19] = 0x01;
    }
    else 
    {
        ble_ack[19] = 0x02;
    }
	
    Ble_SendDataViaBat(ble_ack, 20);        //�ϴ�app
}


/****************************************************************
** Function name:             Protocol_SetHeartAlarmRage
** Descriptions:              
** input parameters:          p_nus��              �ṹ��ָ��
                              p_data��             ����ָ��
** output parameters:         ��
** Returned value:            ��
** Created by:                WJ
** Created Date:              2017-02-16
****************************************************************/
static void Protocol_SetHeartAlarmRage(ble_nus_t * p_nus, uint8_t * p_data)
{
    uint8_t ble_ack[20] = {0x00,};
    heart_alert_config_t  heart_alert_config_s = {0};
    
    ble_ack[0] = p_data[0]; //0xAC
    memcpy(&heart_alert_config_s,&p_flash_param->heart_alert_s,sizeof(heart_alert_config_t));
    
    if(p_data[3] == 0x00)
    {
        heart_alert_config_s.enable = 0;
        Api_UpdateHeartAlertParam(&heart_alert_config_s);
    }
    else if(p_data[3] == 0x01)
    {
        heart_alert_config_s.enable = 1;
        heart_alert_config_s.high = p_data[1];
        heart_alert_config_s.low = p_data[2];
        Api_UpdateHeartAlertParam(&heart_alert_config_s);       
    } 
    
    ble_ack[1] = p_flash_param->heart_alert_s.high;
    ble_ack[2] = p_flash_param->heart_alert_s.low;
    ble_ack[3] = 0x01;
    ble_ack[4] = p_data[3];
    ble_ack[5] = p_flash_param->heart_alert_s.enable; 
    
    Ble_SendDataViaBat(ble_ack, 20);    
}

/****************************************************************
** Function name:             Protocol_ReadDeviceFunction
** Descriptions:              Э���ȡ�豸
** input parameters:          p_nus��              �ṹ��ָ��
                              p_data��             ����ָ��
** output parameters:         ��
** Returned value:            ��
** Created by:                WJ
** Created Date:              2017-02-16
****************************************************************/
static void Protocol_ReadDeviceFunction(ble_nus_t * p_nus, uint8_t * p_data)
{      
    Protocol_Upload_Fun_Config(); 
    Protocol_Upload_Ancs_Config();
}

/****************************************************************
** Function name:             Protocol_ReadWalkCnt
** Descriptions:              Э���ȡ�ǲ�
** input parameters:          p_nus��              �ṹ��ָ��
                              p_data��             ����ָ��
** output parameters:         ��
** Returned value:            ��
** Created by:                WJ
** Created Date:              2017-02-16
****************************************************************/
static void Protocol_ReadWalkCnt(ble_nus_t * p_nus, uint8_t * p_data)
{
    uint8_t ble_ack[20] = {0x00,};
    uint32_t all_step = 0;
    
    ble_ack[0] = p_data[0];    //0xA8
    if(p_data[1] == 0)
    {
        all_step = DataManage_GetCurrentStepCnt();
        ble_ack[1] = (all_step >> 24) & 0xFF;
        ble_ack[2] = (all_step >> 16) & 0xFF;
        ble_ack[3] = (all_step >>  8) & 0xFF;
        ble_ack[4] =  all_step        & 0xFF; 	    
    }
    else if((p_data[1] == 1) || (p_data[1] == 2))
    {                                                      
        all_step = Api_GetStepAllCount(p_data[1]);
        ble_ack[1] = (all_step >> 24) & 0xFF;
        ble_ack[2] = (all_step >> 16) & 0xFF;
        ble_ack[3] = (all_step >>  8) & 0xFF;
        ble_ack[4] =  all_step        & 0xFF;                                 
    }
    else if(p_data[1] == 0x03)
    {
        p_data_manage->app_upload_control_s.pedometer = 1; //ÿ���붼Ҫ�ϴ�����
    }
    else if(p_data[1] == 0x04)
    {
        p_data_manage->app_upload_control_s.pedometer = 1; //ÿ���붼Ҫ�ϴ�����  
    }
    
    ble_ack[5] = p_data[1]; 
    ble_ack[19] = 0x00;
    Ble_SendDataViaBat(ble_ack, 20);//�ϴ��Ʋ����                        
}

/****************************************************************
** Function name:             Protocol_ReadSportData
** Descriptions:              Э���ȡ�˶�����
** input parameters:          p_nus��              �ṹ��ָ��
                              p_data��             ����ָ��
** output parameters:         ��
** Returned value:            ��
** Created by:                WJ
** Created Date:              2017-02-16
****************************************************************/
static void Protocol_ReadSportData(ble_nus_t * p_nus, uint8_t * p_data)
{
    uint8_t ble_ack[20] = {0x00,};
    uint32_t all_step = 0;
    
    ble_ack[0] = p_data[0];    //0xA9
                                                    
    all_step = All_StepGet(); 
    ble_ack[1] = ble_ack[5] = (all_step >> 24) & 0xFF;
    ble_ack[2] = ble_ack[6] = (all_step >> 16) & 0xFF;
    ble_ack[3] = ble_ack[7] = (all_step >>  8) & 0xFF;
    ble_ack[4] = ble_ack[8] = all_step        & 0xFF;                                 

    ble_ack[9]  = 0;    //�˶�����
    ble_ack[10] = 0;    //�˶�����
    
    ble_ack[5] = p_data[1];        
    Ble_SendDataViaBat(ble_ack, 20);//�ϴ��Ʋ����                        
}

/****************************************************************
** Function name:             Protocol_SetBluetoothDisconnect
** Descriptions:              Э��Ͽ�����
** input parameters:          p_nus��              �ṹ��ָ��
                              p_data��             ����ָ��
** output parameters:         ��
** Returned value:            ��
** Created by:                WJ
** Created Date:              2017-02-16
****************************************************************/  
static void Protocol_SetBluetoothDisconnect(ble_nus_t * p_nus, uint8_t * p_data)
{
    //Disconnect the bluetooth device the initiative
    BThread_DisconnectDevice(true);
}

/****************************************************************
** Function name:             Protocol_ReadCurHeart
** Descriptions:              Э���ȡ��ǰ����
** input parameters:          p_nus��              �ṹ��ָ��
                              p_data��             ����ָ��
** output parameters:         ��
** Returned value:            ��
** Created by:                WJ
** Created Date:              2017-02-16
****************************************************************/ 
static void Protocol_ReadCurHeart(uint8_t * p_data)
{ 
    uint8_t ack_data[20] = {0x00,};
    ack_data[0] = p_data[0];    //0xD0
    ack_data[1] = p_data[1];

    if(p_data[1] == 0x01)
    {
		//�͵粻֧�ֲ���
		if(Battery_Get_Grade() == 0)
		{
			ack_data[5] = 0x0A;
			Ble_SendDataViaBat(ack_data, 20);
			
			return ;
		}
		
		//�ֻ����������Ͳ������³�ʼ��      add by JSH  2017-11-02
		if(p_data_manage->blood_oxygen_measure_s.btn == 1)
		{
            ack_data[5] = 0x04;
            Ble_SendDataViaBat(ack_data, 20);
			return ;
		}
        if(p_data_manage->bp_measure_s.bp_btn == 1)
		{
            ack_data[5] = 0x01;
            Ble_SendDataViaBat(ack_data, 20);
			return ;
		}
        else
        {
//			p_data_manage->upload = UPLOAD_ADC_GREEN_EVENT;
            p_data_manage->heart_measure_s.app = 1;
            //YX.20180319.���ʹ���������Բɼ�ԭʼ�������ݴ�������
            if(p_data_manage->upload == UPLOAD_ADC_GREEN_EVENT)
            {
                p_data_manage->data_pool.send_wait_len = (p_data_manage->data_pool.cache_update_cnt+0)%51;
                p_data_manage->data_pool.send_read_pos = 100;
            }
        }
        
        // ������Ҫ�жϵ�ǰ�ֻ��Ƿ��в���
        if((p_data_manage->heart_measure_s.btn == 0)
            /*&& (p_data_manage->blood_oxygen_measure_s.btn == 0)*/
            && (Movement_GetState() != MOVEMENT_STATE_RUN)
            /*&& (p_data_manage->bp_measure_s.bp_btn == 0)*/)
        {
            heart_init_cause = 3;
            p_data_manage->heart_measure_s.init = 1;
            p_data_manage->heart_measure_s.calib_well = 0;
            p_data_manage->heart_measure_s.calib_init = 1;
            heart_calib_init_cause = 2;
            BleThread_AFE4404HeartOpen();
        }
    }
    else if(p_data[1] == 0x00)
    {
        p_data_manage->heart_measure_s.init = 0;
        p_data_manage->heart_measure_s.app = 0;
        p_data_manage->heart_measure_s.calib_well = 0;
        p_data_manage->heart_measure_s.calib_init = 0;
        if((p_data_manage->heart_measure_s.btn == 0)
           && (p_data_manage->blood_oxygen_measure_s.btn == 0)
           && (Movement_GetState() != MOVEMENT_STATE_RUN)
           && (p_data_manage->bp_measure_s.bp_btn == 0))
        {	    
            BleThread_AFE4404DevClose();
        }
    }   
}

/****************************************************************
** Function name:           Protocol_GetMovementPatternsDataCRC
** Descriptions:            ��ȡ�˶�ģʽ����CRC
** input parameters:        p_nus�����ݽ���ͨ����p_data:���յ�����
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ             
** Created Date:            2017-08-08
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
static void Protocol_GetMovementPatternsDataCRC(ble_nus_t *p_nus, uint8_t *p_data)   
{
    uint8_t ble_ack[20] = {0x00, };

    ble_ack[0] = p_data[0]; 
    ble_ack[1] = p_data[1];     
    ble_ack[2] = 0x01; 
    if(p_data[1] == 0x01)
    {
        Movement_GetHeadCRC(&ble_ack[3], 3);
    }
        
    Ble_SendDataViaBat(ble_ack, 20); 
}

/****************************************************************
** Function name:           Protocol_GetMovementPatternsData
** Descriptions:            ��ȡ�˶�ģʽ�洢����
** input parameters:        p_nus�����ݽ���ͨ����p_data:���յ�����
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ             
** Created Date:            2017-08-08
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
static void Protocol_GetMovementPatternsData(ble_nus_t *p_nus, uint8_t *p_data)
{
    uint8_t ble_ack[20] = {0x00, };
    uint8_t ret = 0;

    if(p_data[1] < MOVEMENT_PATTERNS_MAX_BLOCK)
    {
        ret = Movement_SetSendDataInfo(p_data[1]);        
    }
    
    if(ret == 1)
    {
        //�ϴ��˶�ģʽ����
        ComThread_SendMovementData();
    }
    else
    {       
        ble_ack[0] = p_data[0]; 
        ble_ack[5] = p_data[1];    
        Ble_SendDataViaBat(ble_ack, 20);     
    }
}

/****************************************************************
** Function name:           Protocol_OpreateMovementPatternsData
** Descriptions:            �˶�ģʽ�����������͹ر�
** input parameters:        p_nus�����ݽ���ͨ����p_data:���յ�����
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ             
** Created Date:            2017-08-08
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
static void Protocol_OpreateMovementPatternsData(ble_nus_t *p_nus, uint8_t *p_data)
{
    uint8_t ble_ack[20] = {0x00, };
    
    ble_ack[0] = p_data[0]; 
    ble_ack[1] = p_data[1];      
    ble_ack[2] = 0x01; 
    if(p_data[1] == 0)
    {     
        /* �ر�*/        
        if(Movement_IsBusy())
        {
            Movement_Exit();
            //�˳��˶�ģʽUI����
//            if(GuiThread_GetTaskType() == CPU_TASK_MOVEMENT)
//            {
//                GuiThread_InMainTask();
//            }
            ble_ack[2] = 0x01; 
            ble_ack[3] = 0x00; 
        }
        else
        {
            ble_ack[2] = 0x01; 
            ble_ack[3] = 0x00; 
        }
    }
    else if(p_data[1] == 1)
    {
        /* ����*/
        if(GuiThread_TimerIsBusy() || Battery_Get_Status() == BATTERY_POWER_CHARGE || Battery_Get_Status() == BATTERY_POWER_FULL)
        {
            ble_ack[2] = 0x00; 
            ble_ack[3] = 0x02; 
        }
        else if(Movement_IsBusy())       
        {
            ble_ack[2] = 0x00; 
            ble_ack[3] = 0x01; 
        }
        else
        {
            Movement_Start();
            GuiThread_InMovementTask();
            ble_ack[2] = 0x01; 
            ble_ack[3] = 0x00; 
        }
    }
    else if(p_data[1] == 2)    
    {
        //��ȡ�˶�ģʽ����״̬
        ble_ack[2] = 0x01; 
        if(Movement_IsBusy())       
        {
            ble_ack[3] = 0x01; 
        }
        else
        {             
            ble_ack[3] = 0x00; 
        }
    }
    else
    {
        ble_ack[2] = 0x00; 
    }
 
    Ble_SendDataViaBat(ble_ack, 20);
}

/****************************************************************
** Function name:           Protocol_SetMovementPatternsPuseTimeOut
** Descriptions:            �����˶�ģʽ����ͣ��ʱʱ��
** input parameters:        p_nus�����ݽ���ͨ����p_data:���յ�����
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ             
** Created Date:            2017-08-08
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
static void Protocol_SetMovementPatternsPuseTimeOut(ble_nus_t *p_nus, uint8_t *p_data)
{
    uint8_t ble_ack[20] = {0x00, };
    uint16_t timeout_time = 0;
    
    ble_ack[0] = p_data[0];   
    ble_ack[1] = p_data[1];    
    ble_ack[2] = 0x01;       
    if(p_data[1] == 0x01)
    {       
        timeout_time = p_data[2];
        if(Movement_IsVaildThePuseTimeOut(timeout_time))
        {
            p_flash_param->movement_max_puse_time = timeout_time;//p_data[2];           
            BleThread_AsynUpdateAppConfig();
        }
        else
        {
            ble_ack[2] = 0x00;  
        }
    }
    else if(p_data[1] != 0x02)
    {
        ble_ack[2] = 0x00;  
    }
    
    ble_ack[3] = p_flash_param->movement_max_puse_time; 
    Ble_SendDataViaBat(ble_ack, 20);    
}

/****************************************************************
** Function name:           Protocol_GetAllMotionData
** Descriptions:            ��ȡ�Ʋ�����·���������
** input parameters:        p_nus�����ݽ���ͨ����p_data:���յ�����
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ             
** Created Date:            2017-08-15
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
static void Protocol_GetAllMotionData(ble_nus_t *p_nus, uint8_t *p_data)
{
    uint8_t ble_ack[20] = {0x00, };
//    uint8_t block = 0;          // flash addr
//    uint8_t month = 0;
//    uint8_t day = 0;
//    daily_block_info_t daily_block_info = {0x00, };
    sport_information_info_t gsii_sport_info_s = {0x00,};    
    
    ble_ack[0] = p_data[0];   
    ble_ack[1] = p_data[1];    

   if(p_data[1] < 3)
   {             
        if(p_data[1] == 0)
        {
            Get_SportInformationInfo(&gsii_sport_info_s);
            memcpy(&ble_ack[2], &gsii_sport_info_s.posi_steps, 4);   
            memcpy(&ble_ack[6], &gsii_sport_info_s.posi_distance, 4); 
            gsii_sport_info_s.posi_cal = gsii_sport_info_s.posi_cal/100;
            memcpy(&ble_ack[10], &gsii_sport_info_s.posi_cal, 4);
        }
//        else
//        {          
//            block = Daily_GetPreviousBlock(p_data[3]);
//            App_FlashReadDailyHead(block, (void *)&daily_block_info, sizeof(daily_block_info_t));           
//            Sclock_GetPreviousDayDate (p_data[3], &month, &day);
//            if(daily_block_info.block_flag == 0x00 
//              && daily_block_info.month == month && daily_block_info.day == day)
//            {
//                memcpy(&ble_ack[2], &daily_block_info.all_step, 4);   
//                memcpy(&ble_ack[6], &daily_block_info.all_distance, 4); 
//                daily_block_info.all_calories = daily_block_info.all_calories/100;
//                memcpy(&ble_ack[10], &daily_block_info.all_calories, 4); 
//            }
//        }       
    }
  
    Ble_SendDataViaBat(ble_ack, 20);
}

/****************************************************************
** Function name:             Protocol_SetSedentaryTime
** Descriptions:              Э�����þ���ʱ��
** input parameters:          p_nus��              �ṹ��ָ��
                              p_data��             ����ָ��
** output parameters:         ��
** Returned value:            ��
** Created by:                WJ
** Created Date:              2017-02-16
****************************************************************/ 
static void Protocol_SetSedentaryTime(ble_nus_t * p_nus, uint8_t * p_data)
{
    uint8_t ble_ack[20] = {0x00,};
    sedentary_config_t  st_sedentary_config_t = {0};
    
    ble_ack[0] = p_data[0]; //0xE1
    
    if((p_data[6] == 0) || (p_data[6] == 1))
    {
        st_sedentary_config_t.start_hour = p_data[1];
        st_sedentary_config_t.start_minute = p_data[2];
        st_sedentary_config_t.end_hour = p_data[3];
        st_sedentary_config_t.end_minute = p_data[4];
        st_sedentary_config_t.internal = p_data[5];
        st_sedentary_config_t.enable = p_data[6];
        Api_UpdateSedentaryConfig(&st_sedentary_config_t);
    }

    ble_ack[2] = p_flash_param->sedentary_config_s.start_hour;
    ble_ack[3] = p_flash_param->sedentary_config_s.start_minute;
    ble_ack[4] = p_flash_param->sedentary_config_s.end_hour;
    ble_ack[5] = p_flash_param->sedentary_config_s.end_minute;
    ble_ack[6] = p_flash_param->sedentary_config_s.internal;  
 
    //��ǰ����״̬
    ble_ack[7] = p_flash_param->sedentary_config_s.enable; 
    
    ble_ack[1] = 0x01; 
    ble_ack[8] = p_data[6];
    Ble_SendDataViaBat(ble_ack, 20);    
}

/****************************************************************
** Function name:             Protocol_SetWearTestingSwitch
** Descriptions:              
** input parameters:          ��
** output parameters:         ��
** Returned value:            ��
** Created by:                WJ
** Created Date:              2017-02-16
****************************************************************/ 
static void Protocol_SetWearTestingSwitch(ble_nus_t * p_nus, uint8_t * p_data)
{
    uint8_t ble_ack[20] = {0x00,};
    dev_config_t  dev_config_s = {0};
    
    memcpy(&dev_config_s,&p_flash_param->dev_config_s,sizeof(dev_config_t));
    
    ble_ack[0] = 0xE2; //0xE2
    ble_ack[1] = 0x01;
    ble_ack[2] = p_data[1];
    ble_ack[3] = p_flash_param->dev_config_s.wear_detect_enable;
				
    if(Afe4900_IsOpen())
    {
        ble_ack[1] = 0x00;	
    }
    else
    {
        if(p_data[1] == 0)
        {
             dev_config_s.wear_detect_enable = 0;	
        }
        else if(p_data[1] == 1)
        {
             dev_config_s.wear_detect_enable = 1;
        }

        Api_UpdateDevConfig(&dev_config_s);
     }
 
    Ble_SendDataViaBat(ble_ack, 20);       
}

/****************************************************************
** Function name:             Protocol_RestoreFactory
** Descriptions:              ������ݣ��ָ���������
** input parameters:          p_nus��              �ṹ��ָ��
                              p_data��             ����ָ��
** output parameters:         ��
** Returned value:            ��
** Created by:                WJ
** Created Date:              2017-02-16
****************************************************************/ 
static void Protocol_RestoreFactory(ble_nus_t *p_nus, uint8_t *p_data)
{
    uint8_t ble_ack[20] = {0xF2, 0x01};
    
    if(p_data[1] == 0x01
      && (p_data_manage->app_factory_clear_s.clear_data == 0x00 
         && p_data_manage->app_factory_clear_s.clear_config == 0x00))
    {   //���ȫ�����ݲ��ػ�
        p_data_manage->app_factory_clear_s.clear_data = 0x01;
        p_data_manage->app_factory_clear_s.clear_config = 0x01;
        Movement_RestoreFactoryExit();
        BleThread_FactoryClearData();
        GuiThread_AddEvent(CPU_EVENT_TASK_POWEROFF);
    }
    else if(p_data[1] == 0x02
      && (p_data_manage->app_factory_clear_s.clear_data == 0x00 ))
    { //����û�����
        p_data_manage->app_factory_clear_s.clear_data = 0x01;
        Movement_RestoreFactoryExit();
        BleThread_FactoryClearData();        
    }
    else if(p_data[1] == 0x03
      && (p_data_manage->app_factory_clear_s.clear_config == 0x00))
    { //�������
        p_data_manage->app_factory_clear_s.clear_config = 0x01;
        BleThread_FactoryClearData();       
    }
    else if(p_data[1] == 0x04)
    { //�ػ�
        GuiThread_AddEvent(CPU_EVENT_TASK_POWEROFF);
    }
    else
    {
        ble_ack[1] = 0x00;   
    }
    
    
    Ble_SendDataViaBat(ble_ack, 20); 
}

/****************************************************************
** Function name:             Protocol_SetRawDataType
** Descriptions:              ����ԭʼ��������
** input parameters:          p_nus��            �ṹ��ָ��
                              p_data��           ����ָ��
** output parameters:         ��
** Returned value:            ��
** Created by:                WJ
** Created Date:              2017-02-16
****************************************************************/ 
static void Protocol_SetRawDataType(ble_nus_t * p_nus, uint8_t * p_data)
{
    uint8_t ble_ack[20] = {0xF3,};

    if(p_data[1] == 0)
    {
        p_data_manage->upload = UPLOAD_ADC_GREEN_EVENT;
    }
    else if(p_data[1] == 1)
    {
        p_data_manage->upload = UPLOAD_ADC_GSENSOR_EVENT;
    }        
    else if(p_data[1] == 6) 
    {
        p_data_manage->upload = UPLOAD_ADC_OXYGEN_EVENT;
    }        
    
    //��ǰ״̬
    ble_ack[1] = p_data_manage->upload;
    ble_ack[2] = p_data[1];
    Ble_SendDataViaBat(ble_ack, 20);  
}

/****************************************************************
** Function name:             Protocol_SwitchLanguages
** Descriptions:              Э���л�����
** input parameters:          p_nus��             �ṹ��ָ��
                              p_data��            ����ָ��
** output parameters:         ��
** Returned value:            ��
** Created by:                WJ
** Created Date:              2017-02-16
****************************************************************/ 
static void Protocol_SwitchLanguages(ble_nus_t * p_nus, uint8_t * p_data)
{
    uint8_t ble_ack[20] = {0x00,};
    uint8_t system_language = 0;
    uint8_t saved_language;
    
    saved_language = p_flash_param->language_e;
    ble_ack[0] = p_data[0];   //0xF4    
    ble_ack[1] = 0x01;
    ble_ack[2] = 0x01;   
    ble_ack[3] = p_data[4];
    ble_ack[4] = p_data[1];
    
    if(p_data[1] == 0x00)
    {
        if(p_data[4] == 0x00)
        {
			system_language = 1;   
        }
        else if(p_data[4] == 0x01)
        {
			system_language = 2;
        }
        else if(p_data[4] == 0x02)
        {           
            ble_ack[2] = saved_language - 1;
            ble_ack[4] = saved_language;
        }
        else
        {
            ble_ack[1] = 0x00; 
        }
    }
    else
    { 
        if( p_data[2] == 0x00 && p_data[1] <= 0x0B )
        {
            system_language = p_data[1];   
        }
        else if(p_data[2] != 0x00 && p_data[2] <= 0x0C )
        {
            system_language = p_data[2];
			//WJ.20181012.�ظ���������
			ble_ack[4] = p_data[2];			
        }
        else
        {
            ble_ack[1] = 0x00;
        }
    }   
	
    if(system_language != 0 && saved_language != system_language)
    {
        p_flash_param->language_e = (CONFIG_LANGUAGE_E)system_language;
		BleThread_AsynUpdateAppConfig();
//        Api_UpdateLanguageConfig(system_language);
        Gui_ThreadUpdateMian();
    }
    Ble_SendDataViaBat(ble_ack, 20);    
}

/****************************************************************
** Function name:             Protocol_ReadBattery
** Descriptions:              Э���ȡ���״̬
** input parameters:          p_nus��         �ṹ��ָ��
** output parameters:         ��
** Returned value:            ��
** Created by:                WJ
** Created Date:              2017-02-16
****************************************************************/ 
static void Protocol_ReadBattery(ble_nus_t * p_nus)
{
    uint8_t ble_ack[20] = {0x00,};
    
    ble_ack[0] = 0xA0;
    ble_ack[1] = Battery_Get_Status();     //battery state
    ble_ack[2] = 0;                        //battery voltage
    ble_ack[3] = 0;                        //sleep state
    ble_ack[4] = Battery_Get_Grade();      //battery level
    
    Ble_SendDataViaBat(ble_ack, 7);
}

/****************************************************************
** Function name:             Protocol_ReadProductInformation
** Descriptions:              ��ȡ������Ϣ
** input parameters:          p_nus:            �ṹ��ָ��
                              p_data��          
** output parameters:         ��
** Returned value:            ��
** Created by:                WJ
** Created Date:              2017-02-16
****************************************************************/ 
static void Protocol_ReadProductInformation(ble_nus_t * p_nus, uint8_t * p_data)
{
    uint8_t ble_ack[20] = {0x00,};
    uint8_t ble_ack1[20] = {0x00,};
    
    ble_ack[0] = p_data[0];     //0xFC
    ble_ack1[0] = p_data[0];
    if(p_data[1] == 0)  //get product information
    {
        ble_ack[1] = (uint8_t)(NRF_UICR->CUSTOMER[3]>>24);    //
        ble_ack[2] = (uint8_t)(NRF_UICR->CUSTOMER[3]>>16);    //
        ble_ack[3] = (uint8_t)(NRF_UICR->CUSTOMER[2]>>24);    //
        ble_ack[4] = (uint8_t)(NRF_UICR->CUSTOMER[2]>>16); 
        ble_ack[5] = (uint8_t)(NRF_UICR->CUSTOMER[2]>>8);
        ble_ack[6] = (uint8_t)(NRF_UICR->CUSTOMER[2]>>0);
        ble_ack[7] = (uint8_t)(NRF_UICR->CUSTOMER[5]>>0);      //
        ble_ack[8] = (uint8_t)(NRF_UICR->CUSTOMER[3]>>8);      //
        ble_ack[9] = (uint8_t)(NRF_UICR->CUSTOMER[3]>>0);      //
        ble_ack[10] = (uint8_t)(NRF_UICR->CUSTOMER[4]>>24);     //
        ble_ack[11] = (uint8_t)(NRF_UICR->CUSTOMER[4]>>16);
        ble_ack[12] = (uint8_t)(NRF_UICR->CUSTOMER[4]>>8); 
        ble_ack[13] = (uint8_t)(NRF_UICR->CUSTOMER[5]>>24);     //
        ble_ack[14] = (uint8_t)(NRF_UICR->CUSTOMER[5]>>16);
        ble_ack[15] = (uint8_t)(NRF_UICR->CUSTOMER[5]>>8);
        ble_ack[16] = (uint8_t)(NRF_UICR->CUSTOMER[4]>>24);      //
        ble_ack[17] = (uint8_t)(NRF_UICR->CUSTOMER[4]>>0);
        ble_ack[18] = 0x01; 
        ble_ack[19] = 0x01;
        ble_ack1[1] = (uint8_t)(NRF_UICR->CUSTOMER[6]>>24);        //
        ble_ack1[2] = (uint8_t)(NRF_UICR->CUSTOMER[6]>>16);
        ble_ack1[3] = (uint8_t)(NRF_UICR->CUSTOMER[6]>>8);
        ble_ack1[4] = p_data_manage->device_trace_param_s.decryption_ok;
        ble_ack1[18] = 0x02;
        ble_ack1[19] = 0x01; 
    }
    Ble_SendDataViaBat(ble_ack, 20);
    Ble_SendDataViaBat(ble_ack1, 20);
}

/****************************************************************
** Function name:             Protocol_TestFunctional
** Descriptions:              �豸������Ϣ
** input parameters:          p_nus��          �ṹ��ָ��
                              p_data��         ����ָ��
** output parameters:         ��
** Returned value:            ��
** Created by:                WJ
** Created Date:              2017-02-16
****************************************************************/ 
static void Protocol_TestFunctional(ble_nus_t * p_nus, uint8_t * p_data)
{
    uint8_t ble_ack[20] = {0x00,};
    dev_config_t  dev_config_s = {0};
    
    ble_ack[0] = p_data[0];
    ble_ack[1] = p_data[1];
    ble_ack[2] = 0x01; 
    
    memcpy(&dev_config_s, &p_flash_param->dev_config_s,sizeof(dev_config_t));
    
    if(p_data[1]&0x80) //ȫ��                                        
    {     
        p_data_manage->app_factory_clear_s.clear_data = 0x01;
        p_data_manage->app_factory_clear_s.clear_config = 0x01;
        BleThread_FactoryClearData();
        GuiThread_AddEvent(CPU_EVENT_TASK_POWEROFF);    
        Ble_SendDataViaBat(ble_ack, 20); 
        return;
    }    
    
    if(p_data[1] == 0x00)
    {      
        p_data_manage->device_trace_param_s.factory_mode = 0;   
    }
    else
    {
        p_data_manage->device_trace_param_s.factory_mode = 1;
//        p_data_manage->device_trace_param_s.font_test_on = 0;	 
    }
    //YX.20180605.�޸��������Կ�������û�г�ʼ���㷨������
    p_data_manage->blood_oxygen_measure_s.init = 0;
    p_data_manage->heart_measure_s.init = 0;
	p_data_manage->heart_measure_s.calib_init = 0;
    
    if(((p_data[1]&0x04) == 0x04) || ((p_data[1]&0x40) == 0x40))
    {  
        if(p_data[1]&0x04)
        {
            p_data_manage->blood_oxygen_measure_s.init = 1;
			p_data_manage->blood_oxygen_measure_s.cal_success = 0;
			p_data_manage->blood_oxygen_measure_s.cal_state = 0;
			p_data_manage->blood_oxygen_measure_s.cal_progress = 0;
            p_data_manage->blood_oxygen_measure_s.ox_day_time = 0; //�����ǲ��ǰ��쿪����
            p_data_manage->blood_oxygen_measure_s.ox_calc_cnt = 0; // У׼��ɺ󣬼�����������ڰ����Զ����� 
            p_data_manage->blood_oxygen_measure_s.ox_prompt_sign_flag = 0;
            BleThread_AFE4404BloodOxygenOpen();
            p_data_manage->upload = UPLOAD_ADC_NULL_EVENT;
        }
        else
        {
            p_data_manage->heart_measure_s.calib_well = 0;
            heart_calib_init_cause = 3;
            p_data_manage->heart_measure_s.calib_init = 1;
            BleThread_AFE4404HeartOpen();
            heart_init_cause = 4;
            p_data_manage->heart_measure_s.app = 1;
            p_data_manage->heart_measure_s.init = 1;
        } 

        // ����ģʽ�������⿪�عر�
        dev_config_s.wear_detect_enable = 0;
    }
    else
    {
        p_data_manage->blood_oxygen_measure_s.btn = 0;
        
        // �ǹ���ģʽ�������⿪�ش�
        dev_config_s.wear_detect_enable = 1;
		
//        AFE4404_LOG_PRINT("Factory mode close!\n");
        BleThread_AFE4404DevClose();  
    }

    if(p_data[1]&0x08) 
    {
        MotorPowerOn();
    }
    else 
    {
        MotorPowerOFF();
    }          

    if(p_data[1]&0x10)
    {
        GuiThread_AddEvent(CPU_EVENT_TASK_FONTTEST); 
        p_data_manage->device_trace_param_s.font_test_on = 1;	    
    }
    else if((p_data_manage->device_trace_param_s.font_test_on == 1)&&((p_data[1]&0x10) != 0x10))
    {
        GuiThread_ClearEvent(CPU_EVENT_TASK_FONTTEST);
        p_data_manage->device_trace_param_s.font_test_on = 0;
//        GuiThread_AddEvent(CPU_EVENT_TASK_ALL_EXIT); 
    }                     

    if(p_data[1]&0x20)
    {                   
        p_data_manage->app_upload_control_s.pedometer = 1;
        GuiThread_AddEvent(CPU_EVENT_TASK_FONTTEST); 
        p_data_manage->device_trace_param_s.font_test_on = 1;
    } 
    else 
    {
        p_data_manage->app_upload_control_s.pedometer = 0; 
    }

//    if(p_data[1]&0x80) //ȫ��                                        
//    {         
//        BleThread_FactoryClearData();
//        GuiThread_AddEvent(CPU_EVENT_TASK_POWEROFF);    
//    }    
    
    Ble_SendDataViaBat(ble_ack, 20);   
}

/****************************************************************
** Function name:             Protocol_Process_Alarm
** Descriptions:              Э�����Ӵ���
** input parameters:          p_nus��          �ṹ��ָ��
                              p_data��         ����ָ��
** output parameters:         ��
** Returned value:            ��
** Created by:                WJ
** Created Date:              2017-02-16
****************************************************************/ 
static void Protocol_Process_Alarm(ble_nus_t * p_nus, uint8_t * p_data)
{
    uint16_t crc = 0;
    uint16_t p_crc16 = 0;
    uint8_t ack_data[20] = {0};
    alarm_time_config_t alarm_time_s = {0};
    
    if(p_data[1] == 0)
    {
        //�ж������Ƿ���Ч
        if((1 <= p_data[2])&&(p_data[2] <= 20))
        {
            Api_UpdateAlarmTime(p_data[2]-1,&alarm_time_s);
            
            //��ʾ������Ч
            ack_data[1] = 1;
        }
        else
        {
            ack_data[1] = 0;
        }
        
        //��APP����Ӧ������
        ack_data[0] = 0xB9;
        ack_data[2] = 1;
        ack_data[3] = 1;
        memcpy(&ack_data[4],&p_data[1],11);
        ack_data[18] = p_data[18];
        ack_data[19] = p_data[19];
        
        Ble_SendDataViaBat(ack_data, 20); 
    }
    else if(p_data[1] == 1)
    {
        //�ж������Ƿ���Ч
        if((1 <= p_data[2])&&(p_data[2] <= 20))
        {
            alarm_time_s.hour   = p_data[3]%24;
            alarm_time_s.minute = p_data[4]%60;
            alarm_time_s.state  = p_data[5]%2;
            alarm_time_s.repeat = p_data[6]&0x7F;
            alarm_time_s.scence = p_data[7]%21;
            alarm_time_s.year   = p_data[8]|(p_data[9]<<8);
            alarm_time_s.month  = p_data[10]%13;
            alarm_time_s.day    = p_data[11]%32;
            alarm_time_s.valid  = 1;
            Api_UpdateAlarmTime(p_data[2]-1,&alarm_time_s);
            
            //��ʾ������Ч
            ack_data[1] = 1;
        }
        else
        {
            ack_data[1] = 0;
        }
        
        //��APP����Ӧ������
        ack_data[0] = 0xB9;
        ack_data[2] = 1;
        ack_data[3] = 1;
        memcpy(&ack_data[4],&p_data[1],11);
        ack_data[18] = p_data[18];
        ack_data[19] = p_data[19];
        
        Ble_SendDataViaBat(ack_data, 20); 
    }
    else if(p_data[1] == 2)
    {
        Algorithm_AlarmValidCheck();
        crc = Api_GetAlarmCRC();
        p_crc16 = p_data[18] + (p_data[19] << 8);
        if(p_crc16 != crc && Alarm_GetValidConfigNum() != 0)
        {
            ComThread_SendAlarmData();
        }
        else
        {   //WJ.20190121.�޸�crcһ�µ�ʱ��û�лظ�������    
            ack_data[0] = p_data[0];
            ack_data[1] = 1;  
            ack_data[2] = 0;
            ack_data[3] = Alarm_GetValidConfigNum();
            ack_data[4] = p_data[1];
            
            ack_data[18] = p_crc16 & 0xFF;
            ack_data[19] = (p_crc16>>8) & 0xFF;  
            
            Ble_SendDataViaBat(ack_data, 20);     
        }
    }
}

/****************************************************************
** Function name:             Protocol_Process_Hourglass
** Descriptions:              Э�鵹��ʱ����
** input parameters:          p_nus��           �ṹ��ָ��
                              p_data��          ���ݵ�ַ
** output parameters:         ��
** Returned value:            ��
** Created by:                WJ
** Created Date:              2017-02-16
****************************************************************/ 
static void Protocol_Process_Hourglass(ble_nus_t * p_nus, uint8_t * p_data)
{
	battery_status  current_status = BATTERY_POWER_NORMAL;
    hourglass_config_t* p_hourglass = &p_flash_param->hourglass_s;
    uint8_t ble_ack[20] = {0xB2,0x00};
    uint32_t run_remain_time = 0;
	
	current_status = Battery_Get_Status();
	
    ble_ack[2] = 1;
    if(p_data[1] == 0)
    {
        p_hourglass->app_trigger = 0;
    }
    else if(p_data[1] == 1)
    {
//		if(current_status == BATTERY_POWER_NORMAL)
//		{
			ble_ack[2] = 1;
            run_remain_time = (p_data[3])|(p_data[4]<<8)|(p_data[5]<<16);
            if(run_remain_time > 0 && run_remain_time < 86400)
            {
                if(p_data[6] <= 1)
                {
                    p_hourglass->ui_enable = p_data[6]&0x01;
                    p_hourglass->dev_remain_time = (p_data[3])|(p_data[4]<<8)|(p_data[5]<<16);
                    
                    Api_UpdateHourglassParam(&p_flash_param->hourglass_s);
                }
                else if((p_data[6] == 2)||(p_data[6] == 3))
                {
                    if(current_status == BATTERY_POWER_NORMAL)
                    {
                        GuiThread_ClearEvent(CPU_EVENT_TASK_HOURGLASS_TIMEOUT);
                        GuiThread_AddEvent(CPU_EVENT_TASK_HOURGLASS_HINT);
                        p_hourglass->app_trigger = 1;
                        p_hourglass->app_send = 1;
                        p_hourglass->run_remain_time = run_remain_time;
                    }
                    else
                    {
                        ble_ack[2] = 0;
                    }
                }
            }
            else
            {
                ble_ack[2] = 0;
            }
    }
    else if(p_data[1] == 2)
    {
        ble_ack[2] = 1;
    }
    
    // ��ȡ����������Ϣ
    ble_ack[1] = p_data[1];
    ble_ack[4] = p_hourglass->dev_remain_time&0xFF;
    ble_ack[5] = (p_hourglass->dev_remain_time>>8)&0xFF;
    ble_ack[6] = (p_hourglass->dev_remain_time>>16)&0xFF;
    ble_ack[7] = p_hourglass->ui_enable;
    ble_ack[8] = p_hourglass->run_remain_time&0xFF;
    ble_ack[9] = (p_hourglass->run_remain_time>>8)&0xFF;
    ble_ack[10] = (p_hourglass->run_remain_time>>16)&0xFF;
    if(p_hourglass->app_trigger == 1)
    {
        ble_ack[11] = 1;
    }
				
//    Api_UpdateHourglassParam(&p_flash_param->hourglass_s);
				
    Ble_SendDataViaBat(ble_ack, 20);   
}

/****************************************************************
** Function name:             Protocol_TimeRangeProcess
** Descriptions:              Э��ʱ�䷶Χ����
** input parameters:          p_nus��          �ṹ��ָ��
                              p_data��         ���ݵ�ַ
** output parameters:         ��
** Returned value:            ��
** Created by:                WJ
** Created Date:              2017-02-16
****************************************************************/ 
static void Protocol_TimeRangeProcess(ble_nus_t * p_nus, uint8_t * p_data)
{
    uint8_t ack_data[20] = {0xB3,0x01};
    time_range_config_t  time_range_config_s = {0};
                   
    memcpy(&ack_data[2], &p_data[1], 17);
                            
    if(p_data[2] == 0)
    {
        time_range_config_s.start_hour   = p_data[3];
        time_range_config_s.start_minute = p_data[4];
        time_range_config_s.end_hour     = p_data[5];
        time_range_config_s.end_minute   = p_data[6];
        if(p_data[7] == 0)
        {
            time_range_config_s.enable = 0; 
            
            if(p_flash_param->blood_oxygen_config_s.enable == 1
              && p_data_manage->blood_oxygen_measure_s.time == 1
              && clock->hour < 7
              && Afe4900_IsOpen() == 2)
            {   // ��ʱ�رղ���
                p_data_manage->blood_oxygen_measure_s.time = 0;
                BleThread_AFE4404DevClose();
            }
        }
        else if(p_data[7] == 1)
        {
            time_range_config_s.enable = 1; 	 
        }
	    
        // Ѫ��������/����Ϣ
        Api_UpdateBloodOxygenParam(&time_range_config_s);
	    
        memcpy(&ack_data[3], &p_data[2], 6);
    }
    else if(p_data[2] == 1)
    {
        // Ѫ��������ȡ��Ϣ
        memcpy(&time_range_config_s, &p_flash_param->blood_oxygen_config_s, sizeof(time_range_config_t));
                 
        ack_data[3] = p_data[2];	    
        ack_data[4] = time_range_config_s.start_hour;
        ack_data[5] = time_range_config_s.start_minute;
        ack_data[6] = time_range_config_s.end_hour;
        ack_data[7] = time_range_config_s.end_minute;
        ack_data[8] = time_range_config_s.enable ;
    }
 
    Ble_SendDataViaBat(ack_data, 20);
}

extern uint8_t m_ble_name[10];
static void Protocol_ModifyBluetoothName(ble_nus_t *p_nus, uint8_t *p_data)
{
    uint8_t ack_data[20] = {0x84,0x01,0x01};
//    bluetooth_name_t  bluetooth_name_s = {0};
    ble_gap_conn_sec_mode_t sec_mode;
    
    if(p_data[1] == 1)
    {
        /*
        bluetooth_name_s.length = 8;
        memcpy(&bluetooth_name_s.name[0], &p_data[2], 8);
        
        BLE_GAP_CONN_SEC_MODE_SET_OPEN(&sec_mode);
        sd_ble_gap_device_name_set(&sec_mode,(const uint8_t *)bluetooth_name_s.name,bluetooth_name_s.length);
        
        Api_UpdateBluetoothName(&bluetooth_name_s);  
        */
        //WJ.20181022.�޸��������Ʒ�ʽ
        memset(m_ble_name, 0, sizeof(m_ble_name));
        memcpy(m_ble_name, (uint8_t *)&p_data[2], 8);
        BLE_GAP_CONN_SEC_MODE_SET_OPEN(&sec_mode);
        sd_ble_gap_device_name_set(&sec_mode,(const uint8_t *)m_ble_name, strlen((char *)m_ble_name));
        
        BleThread_UpdateBleNameConfig();
    }

    Ble_SendDataViaBat(ack_data, 20);
}

/****************************************************************
** Function name:           Protocol_TestMode
** Descriptions:            Ѫ���ֻ��Զ��������ݲɼ�
** input parameters:        p_nus�����ݽ���ͨ����p_data:���յ�����
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ             
** Created Date:            2018-07-02
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
static void Protocol_TestMode(ble_nus_t *p_nus, uint8_t *p_data)
{
    uint8_t ble_ack[20] = {0x00,};
    ble_ack[0] = p_data[0];
    if(p_data[0] == 0xFE && (p_data[1] == 0x02 || p_data[1] == 0x03))
    {
        Test_ProtocolParsing(p_data, &ble_ack[1]);
    }
    
    Ble_SendDataViaBat(ble_ack, 20); 
}

/****************************************************************
** Function name:           Protocol_DisplayUiFunction
** Descriptions:            Ů�Թ��ܲ�������
** input parameters:        p_nus�����ݽ���ͨ����p_data:���յ�����
** output parameters:       ��
** Returned value:          ��
** Created by:              YX             
** Created Date:            2017-10-26
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
//static void Protocol_SetPhysParam(ble_nus_t *p_nus, uint8_t *p_data)
//{
//    uint8_t ble_ack[20] = {0x00,};
//    
//    Phys_ParsingProtocol(p_data, ble_ack);
//    
//    Ble_SendDataViaBat(ble_ack, 20);     
//}


/****************************************************************
** Function name:           Protocol_DisplayUiFunction
** Descriptions:            UIչʾ����
** input parameters:        p_nus�����ݽ���ͨ����p_data:���յ�����
** output parameters:       ��
** Returned value:          ��
** Created by:              YX             
** Created Date:            2017-07-20
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
static void Protocol_DisplayUiFunction(ble_nus_t *p_nus, uint8_t *p_data)
{
    //FD 95 26 51 EF
//    uint8_t ble_ack[20] = {0x00,};
//    uint8_t i = 0;
//    
//    ble_ack[0] = p_data[0];     //0xFD
    if(p_data[1] == 0x95 && p_data[2] == 0x26 && p_data[3] == 0x51 && p_data[4] == 0xEF)
    {
        /* ������ȷ������UIչʾ���� */
        Gui_SetTraverseUiState(1);
    }
    else
    {
//        ble_ack[2] = 0;
    }
    
//    Ble_SendDataViaBat(ble_ack, 20); 
}

/****************************************************************
** Function name:           Protocol_InvalidCmd
** Descriptions:            ��Чָ���
** input parameters:        p_nus�����ݽ���ͨ����p_data:���յ�����
** output parameters:       ��
** Returned value:          ��
** Created by:              YX             
** Created Date:            2017-07-20
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
static void Protocol_InvalidCmd(ble_nus_t *p_nus, uint8_t *p_data)
{

    uint8_t ble_ack[20] = {0x00,};
    ble_ack[0] = p_data[0];
    
    Ble_SendDataViaBat(ble_ack, 20); 
}



/****************************************************************
** Function name:           Protocol_PeripheralTest
** Descriptions:            �������ʹ��
** input parameters:        p_nus�����ݽ���ͨ����p_data:���յ�����
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ             
** Created Date:            2018-12-21
*****************************************************************/ 
static void Protocol_PeripheralTest(ble_nus_t *p_nus, uint8_t *p_data)
{
	uint8_t ble_ack[20] = {0x00,};
	uint8_t ble_ack1[20] = {0x00,};
    
    ble_ack[0] = ble_ack1[0] = p_data[0];
    ble_ack[1] = ble_ack1[1] = p_data[1];
    ble_ack[2] = ble_ack1[2] = 0;
    

    if(p_data[1] == 0x09 || p_data[1] == 0x0A)
    {   //4410 PPG����   
        if(p_data[1] == 0x09)
        {
			if(p_data[2] == 1)
			{
				Afe4410_RegPpgParamSet(&p_data[3], 1);
			}
            else if(p_data[2] == 2)
			{
				Afe4410_RegPpgParamSet(&p_data[3], 2);
			}
        }
   
		ble_ack[2] = 1;
        ble_ack[3] = 1;
        Afe4410_RegPpgParamGet(&ble_ack[4], 1);        
        Ble_SendDataViaBat(ble_ack, 20);
        
        
		ble_ack1[2] = 1;
        ble_ack1[3] = 2;
        Afe4410_RegPpgParamGet(&ble_ack1[4], 2);        
        Ble_SendDataViaBat(ble_ack1, 20);
    }
}
/****************************************************************
** Function name:           Protocol_BloodPressTest
** Descriptions:            Ѫѹ����
** input parameters:        p_nus�����ݽ���ͨ����p_data:���յ�����
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2018-11-10
*****************************************************************/ 
static void Protocol_BloodPressTest(ble_nus_t *p_nus, uint8_t *p_data)
{
    uint8_t ble_ack[20] = {0x00, };
    
    ble_ack[0] = p_data[0];     //0x90
    if(p_flash_param->bp_func_state != 0x01)
    {
        Ble_SendDataViaBat(ble_ack, 20);
        return;
    }
    
    if(p_data[1] == 0x01)
    {   
        // �ж��Ƿ�æµ
        if(Battery_Get_Grade() == 0 )
        {
            ble_ack[4] = 0xFE;     // �����\�͵��ж�
        }
        else if(Battery_Get_Status() != BATTERY_POWER_NORMAL)
        {   //����ж�
            ble_ack[4] = 0xFD;     // �����\�͵��ж�
        }
        else if(p_data_manage->blood_oxygen_measure_s.btn == 1)
		{
            ble_ack[4] = 0x04;
		}
		// XJ �˶�ģʽ ����Ѫѹ���
        else if(p_data_manage->heart_measure_s.btn == 1 || Movement_GetState() == MOVEMENT_STATE_RUN)
		{
            ble_ack[4] = 0x02;
		}
        else if(p_data_manage->bp_measure_s.bp_btn == 1)
		{
            ble_ack[4] = 0x01;
		}
        else
        {
            //WJ.20190105.���β��η���
            p_data_manage->upload = UPLOAD_ADC_NULL_EVENT;//UPLOAD_ADC_GREEN_EVENT; 
            p_data_manage->bp_measure_s.bp_test_mode = (p_data[2] == 1?1:0);
            p_data_manage->bp_measure_s.bp_set_sp = p_flash_param->bp_s.private_high;
            p_data_manage->bp_measure_s.bp_set_dp = p_flash_param->bp_s.private_low; 
            
            heart_init_cause = 3;
            p_data_manage->bp_measure_s.bp_init = 1;
            p_data_manage->bp_measure_s.bp_app = 1;
            
            p_data_manage->heart_measure_s.calib_well = 0;
            p_data_manage->heart_measure_s.calib_init = 1;
            heart_calib_init_cause = 2;
            
            BleThread_AFE4404BpOpen();      

            ble_ack[4] = 0x00; 
        }
    }
    else if(p_data[1] == 0x00)
    {
        p_data_manage->bp_measure_s.bp_init = 0;
        p_data_manage->bp_measure_s.bp_app = 0;
        
        p_data_manage->heart_measure_s.calib_well = 0;
        p_data_manage->heart_measure_s.calib_init = 0;
        
        if((p_data_manage->heart_measure_s.btn == 0)
           && (p_data_manage->blood_oxygen_measure_s.btn == 0)
           && (Movement_GetState() != MOVEMENT_STATE_RUN)
           && (p_data_manage->bp_measure_s.bp_btn == 0))
        {	    
            BleThread_AFE4404DevClose();
        }
    }
    
    ble_ack[5] = 0x01;      //��ʾ�н���  
    Ble_SendDataViaBat(ble_ack, 20);
}

/****************************************************************
** Function name:           Protocol_SetBloodPressureParameters
** Descriptions:            ����Ѫѹ��������
** input parameters:        p_nus�����ݽ���ͨ����p_data:���յ�����
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2018-11-10
*****************************************************************/ 
static void Protocol_SetBloodPressureParameters(ble_nus_t *p_nus, uint8_t *p_data)
{
    uint8_t         ble_ack[20] = {0x00, };
    bp_config_t     bp_config_s = {0};
    
    ble_ack[0] = p_data[0];  // 0x91
    ble_ack[1] = 0x01;      
    ble_ack[5] = p_data[1];//�ظ�ģʽ    
    ble_ack[7] = p_data[4];//

    if(p_flash_param->bp_func_state != 0x01)
    {
        ble_ack[1] = 0x00;
        Ble_SendDataViaBat(ble_ack, 20);
        return;
    }    
    
    if(p_data[1] == 0x00 || p_data[1] == 0x01)//�رտ������Ի�����
    {
        bp_config_s.mode = (BLOOD_MODE_E)p_data[1];       
        bp_config_s.private_high = p_data[2];
        bp_config_s.private_low = p_data[3];      
        
        Api_UpdateBloodConfig(&bp_config_s);
        
        ble_ack[1] = 0x01;
    }
    else if(p_data[1] == 0x02) //��ȡѪѹģʽ
    {
        ble_ack[1] = 0x01;
    }
    else
    {
        ble_ack[1] = 0x00;
    }
   
    ble_ack[2] = p_flash_param->bp_s.private_high;
    ble_ack[3] = p_flash_param->bp_s.private_low;
    ble_ack[4] = p_flash_param->bp_s.mode;

    
    Ble_SendDataViaBat(ble_ack, 20);
}


/****************************************************************
** Function name:           Protocol_BpFunctionSet
** Descriptions:            �����ر�Ѫѹ����ģ��
** input parameters:        p_nus�����ݽ���ͨ����p_data:���յ�����
** output parameters:       ��
** Returned value:          ��
** Created by:              YX             
** Created Date:            2017-10-26
*****************************************************************/ 
static void Protocol_BpFunctionSet(ble_nus_t *p_nus, uint8_t *p_data)
{
    uint8_t ble_ack[20] = {0x00,};
    
    ble_ack[0] = 0x92;
    ble_ack[1] = p_data[1];
    
    if(p_data[2] == 0x55 && p_data[3] == 0x01 
      && p_data[4] == 0x73 && p_data[5] == 0x40)
    {
        ble_ack[2] = 0x55; 
        ble_ack[3] = 0x01; 
        ble_ack[4] = 0x73;
        ble_ack[5] = 0x40;
    
        if(ble_ack[1] == 0x01 || ble_ack[1] == 0x02)
        {
            p_flash_param->bp_func_state = (ble_ack[1] == 0x01?1:0);
            BleThread_UpdateBpFuncConfig(); 
        }
        if(p_flash_param->bp_func_state == 0x01)
        {
            ble_ack[6] = 0x01;               
        }
        else
        {
            ble_ack[6] = 0x02;  
        }
    }
    
    Ble_SendDataViaBat(ble_ack, 20);     
}

/****************************************************************
** Function name:             Protocol_Bat_Data_Handler
** Descriptions:              Э�����ݴ���
** input parameters:          p_nus��           �ṹ��ָ��
                              p_data��          ���ݵ�ַ
                              length��          ���ݴ�С
** output parameters:         ��
** Returned value:            ��
** Created by:                WJ
** Created Date:              2017-02-16
****************************************************************/ 
void Protocol_Bat_Data_Handler(ble_nus_t * p_nus, uint8_t * p_data, uint16_t length)
{
    uint8_t ble_ack[20] = {0xA1, 0x00};
    
    //YX.20180514.�ֻ��ػ�ʱ��Э���ϲ����κλظ�
    if(p_data_manage->device_trace_param_s.sign_on == 0)
    {
        return;
    }
    
    if(p_data[0] == 0xA1)
    {
        Protocol_PasswordPair(p_nus, p_data, length);
        return;
    }
    
    if(p_data_manage->device_trace_param_s.code_passed != 1)
    {
        //���벻ͨ��,�ظ����벻ͨ��       
        ble_ack[0] = 0xA1;          
        ble_ack[1] = 0x00;    
        ble_ack[2] = 0x00;  
        ble_ack[3] = 0x00;    
        ble_ack[4] = (VEEPOO_DEVICE_ID_L>>8) & 0xFF;
        ble_ack[5] = VEEPOO_DEVICE_ID_L & 0xFF;
        ble_ack[6] = VERSION_M & 0xFF;
        ble_ack[7] = VERSION_H & 0xFF;
        ble_ack[8] = VERSION_L & 0xFF;   
        ble_ack[9] = VERSION_T & 0xFF;   
        ble_ack[10] = 0;
        ble_ack[11] = 0x01; 
        Ble_SendDataViaBat(ble_ack, 20);   
        return;
    }
    //APP�����뾭�ֻ���׼ͨ��  ����  ������صĲ���
    if(1)
    { 
        switch(p_data[0])
        {
            case 0x80://Ѫ������
                Protocol_BloodOxygen(p_nus, p_data);
                break;
            case 0x81://ƣ�ͶȲ���
                Protocol_Fatigue(p_nus, p_data);
                break;
            case 0x82://�����ʲ��� 
                Protocol_Respiration(p_nus, p_data);
                break;
            case 0x84://�������ֵ��޸�
                Protocol_ModifyBluetoothName(p_nus, p_data);
                break;
//            case 0x85://Ů�Թ��ܲ�������
//                Protocol_SetPhysParam(p_nus, p_data);
//                break;
            case 0x90:  //Ѫѹ����
                Protocol_BloodPressTest(p_nus, p_data);
                break;
            case 0x91:  //Ѫѹ��������
                Protocol_SetBloodPressureParameters(p_nus, p_data);
                break;  
            case 0x92:  
                //�����ر�Ѫѹ����ģ��
                Protocol_BpFunctionSet(p_nus, p_data);
            	break;
            case 0xA0:  //��ȡ���״̬,Read the battery status
                Protocol_ReadBattery(p_nus);
                break;                        
            case 0xA1:  //��׼����,Password Setting
                Protocol_PasswordPair(p_nus, p_data, length);
                break;              
            case 0xA2:  //Before the firmware update for equipment information
                Protocol_ReadInfoByOAD(p_nus, p_data);
                break;
            case 0xA3:  //Synchronization of personal information
                Protocol_SynchronizationUserInfo(p_nus, p_data);
                break;
            case 0xA4: //�ϴ��豸��Ϣ 
                Protocol_DeviceInfo(p_nus, p_data);
                break;
            case 0xA5:  //set device time
                Protocol_SetDeviceTime(p_nus, p_data, length);
                break;
            case 0xA6:  //read device time
                Protocol_ReadDeviceTime(p_nus);
                break;
            case 0xA7:  //Read the function of the equipment support
                Protocol_ReadDeviceFunction(p_nus, p_data);
                break;
            case 0xA8:  //Read the meter data
                Protocol_ReadWalkCnt(p_nus, p_data);
                break;
            case 0xA9:  //Read movement data
                Protocol_ReadSportData(p_nus, p_data);
                break;
            case 0xAA:  //bright screen when raising hands
                Protocol_GestureSwitch(p_nus, p_data);
                break;
            case 0xAC:  //Set up the heart rate alarm range
                Protocol_SetHeartAlarmRage(p_nus, p_data); 
                break;
            case 0xAD:  //ANCS notify control
                Protocol_ANCSNotiryControl(p_nus, p_data);
                break;
            case 0xAE:  //The lost
                Protocol_FindPhoneSet(p_nus, p_data);
                break;
            case 0xAF:  //Bluetooth disconnect
                Protocol_SetBluetoothDisconnect(p_nus, p_data);           
                break;
            case 0xB1:  //��Ļ��������    
                Protocol_OledSet(p_nus, p_data);         
                break;
            case 0xB2:  //����ʱ����
                Protocol_Process_Hourglass(p_nus, p_data);
                break;
            case 0xB3:
                Protocol_TimeRangeProcess(p_nus, p_data);
                break;
            case 0xB5:  //�ֻ������ֻ�����
                Protocol_FindPhone(p_nus, p_data);            
                break;
            case 0xB6:  //���๦��
                Protocol_Camera(p_nus, p_data);            
                break;
            case 0xB8:  //���ؿ���
                Protocol_SwitchState(p_nus, p_data);
                break;  
            case 0xB9://��������
                Protocol_Process_Alarm(p_nus, p_data);
                break;            
            case 0xBC:  // IOSϵͳANCS����,
                Protocol_MatchingAndBinding(p_nus, p_data);
                break;  
            case 0xC1://android ��������
                Protocol_Android_NotifySet(p_nus, p_data);
                break;              
            case 0xC2://android ��Ϣ����
                Protocol_Android_NotifyEvent(p_nus, p_data);
                break;
//			case 0xC4: 
//                /* Set the UI update mode */
//                Protocol_UpdateUiMode(p_nus, p_data);
//                break;
//            case 0xC5: 
//                /* erase the UI data */
//                Protocol_EraseUiData(p_nus, p_data);
//                break;
//            case 0xC6:  
//                /* read the UI data */
//                Protocol_ReadUiData(p_nus, p_data);
//                break;
			case 0xC7:  // set the UI style 
				Protocol_SetUiStyle(p_nus, p_data);
				break;
			case 0xCF:
                //UI��������
                Protocol_SetUiInfo(p_nus, p_data);
                break; 
            case 0xCB://��Ч
                Protocol_AndroidNotify(p_nus, p_data);
                break;                  
            case 0xD0://��ȡ���� 
                Protocol_ReadCurHeart(p_data);
                break;
            
            case 0xD1://�����ճ����� 
                ComThread_SendDailyData(p_data);
                break;
            
            case 0xD2://����Ѫ������
                ComThread_SendOxygenData(p_data);
                break;

	        case 0xD3:
	            //��ȡ�˶�ģʽ����CRC
	            Protocol_GetMovementPatternsDataCRC(p_nus, p_data);
	            break;
	        case 0xD4:
	            //��ȡָ������˶�����
	            Protocol_GetMovementPatternsData(p_nus, p_data);
	            break;
	        case 0xD5:
	            //APP�����˶�ģʽ
	            Protocol_OpreateMovementPatternsData(p_nus, p_data);
	            break;
	        case 0xD6:
	            //�����˶�ģʽ��ͣ��ʱʱ��
	            Protocol_SetMovementPatternsPuseTimeOut(p_nus, p_data);
	            break;
			case 0xD8:  
                Protocol_GetAllMotionData(p_nus, p_data);
            break;
            
            case 0xD9:
                ComThread_SendScatterHeartData(p_data);
                break;
                
            case 0xE0:  //Send sleep data//����˯������ 
                ComThread_SendSleepData(p_data);
                break;
            
            case 0xE1:  //Set the sedentary remind time period///���þ���ʱ�� 
                Protocol_SetSedentaryTime(p_nus, p_data);
                break;
            case 0xE2:  //Set wear detection switch//
                Protocol_SetWearTestingSwitch(p_nus, p_data);
                break;          
            case 0xF0:  //read flash data //��flash
                ComThread_SendAllFlashData();
                break;
            case 0xF1:
                Protocol_TestFunctional(p_nus, p_data);
                break;
            case 0xF2:   //Clear data and restore factory Settings    
                Protocol_RestoreFactory(p_nus, p_data);
                break;
            case 0xF3:   //Report the raw data type   
                Protocol_SetRawDataType(p_nus, p_data);
                break;
            case 0xF4:   //��Ӣ���л�  
                Protocol_SwitchLanguages(p_nus, p_data);
                break;          
            case 0xFB:   //���ٶȡ������4410����  
                Protocol_PeripheralTest(p_nus, p_data);
                break;
            case 0xFC:    //Product information
                Protocol_ReadProductInformation(p_nus, p_data);                
                break; 
            case 0xFD:  
                //UIչʾ����
                Protocol_DisplayUiFunction(p_nus, p_data);
                break; 
			case 0xFE:
                Protocol_TestMode(p_nus, p_data);   
                break;
			case 0xFF:
				if(p_data[1] == 0x55 && p_data[2] == 0x01
				   && p_data[3] == 0x73 && p_data[4] == 0x40
				   && p_data[5] == 0x00)
				{
					nrf_delay_ms(500);
					NVIC_SystemReset();     //reset
				}
				break;
            default:     
                Protocol_InvalidCmd(p_nus, p_data);
                break;
        }
    }
}

/****************************************************************
** Function name:             Protocol_Clk_Data_Handler
** Descriptions:              �������ݴ�����
** input parameters:          p_clk��         �ṹ��ָ��
                              p_data��        ���ݵ�ַ
                              length��        ���ݴ�С
** output parameters:         ��
** Returned value:            ��
** Created by:                WJ
** Created Date:              2017-02-16
****************************************************************/ 
void Protocol_Clk_Data_Handler(ble_clk_t * p_clk, uint8_t * p_data, uint16_t length)
{ 
	
}

/****************************************************************
** Function name:             Ble_SendDataViaBat
** Descriptions:              ������������
** input parameters:          data_addr��      ���ݵ�ַ
                              data_size��      ���ݴ�С
** output parameters:         ��
** Returned value:            
** Created by:                WJ
** Created Date:              2017-02-16
****************************************************************/ 
uint32_t Ble_SendDataViaBat(uint8_t *data_addr, uint8_t data_size)
{
    return ble_nus_string_send(BleThread_GetBleNus(), data_addr, data_size); 
}

/****************************************************************
** Function name:             Ble_SendBatMulData
** Descriptions:              ������������
** input parameters:          len:       ���ݴ�С
** output parameters:         ��
** Returned value:            ��
** Created by:                WJ
** Created Date:              2017-02-16
****************************************************************/ 
void Ble_SendBatMulData(uint8_t len,...)
{
    va_list arg_list; //����va_list����
    uint8_t temp[20] = {0};

    va_start(arg_list, len);  //׼�����ʿɱ����
    
    for(uint8_t i = 0; (i < len)&&(i < 20); i++)
    {
        temp[i] = va_arg(arg_list, int);//uint8_t);
    }
	
    va_end(arg_list);   //��ɿɱ�����Ĵ���
    ble_nus_string_send(BleThread_GetBleNus(), temp, 20);
}


