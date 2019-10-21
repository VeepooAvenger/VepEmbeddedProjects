/*************************************Copyright(c)***************************************************
**                                     Veepoo Tech
**                  
**                               COPYRIGHT(C)  2016-2019
**--------------------------------------FileInfo----------------------------------------------------
** FileName:                Communication_thread.c
** Last Modify Data:        2017-08-04
** Last SYS_VERSION:        0.0.0.1
** Descriptions:                     
**---------------------------------------------------------------------------------------------------
** CreatedBy:               GT
** CreateData:              2017-06-14
** SystemVersion:             
** Description:             ͨ���߳���Ҫ����ֻ�������APP�������� 
**                          
**---------------------------------------------------------------------------------------------------
** Modify By:                       
** Modify Data:
** System Version:
** Modify Description:    

******************************************************************************************************/
#include <string.h>
#include "communication_thread.h"
#include "ble_thread.h"
#include "cpu_task_data_type.h"
#include "bracelet_protocol.h"
#include "data_manage.h"
#include "api_flash.h"
#include "app_type.h"
#include "AFE4900_entry.h"
#include "tired_grade_detection.h"
#include "Blood_OxygenDetection.h"
#include "GSensor.h"
#include "ble_wechat_service.h"
#include "configuration.h"
#include "log.h"
#include "Respiratory_Rate.h"
#include "Spo2_OSAHS.h"
#include "app_config_data.h"
#include "movement_patterns.h"
#include "watch_dog.h"
//#include "Afe_4404Calib.h"
static communication_resource_t  com_res_s;
static com_upload_param_t        com_upload_s;

static uint8_t send_flash_data_state = 0;

// ��������
static void CommunicationThread_Handle(void* arg);

/*******************************************************************************************************
** Function name:        ComThread_SetSendFlashDataState
** Descriptions:         ���÷���FLASHԭʼ���ݵ�״̬
** input parameter:      ��     
** Returned Value:       ��
** Created  by:          YX
** Created  Datas:       2018-01-06
*********************************************************************************************************/
void ComThread_SetSendFlashDataState(uint8_t state)
{
    send_flash_data_state = state;
}

/*******************************************************************************************************
** Function name:        ComThread_GetSendFlashDataState
** Descriptions:         ��ȡ����FLASHԭʼ���ݵ�״̬
** input parameter:      ��     
** Returned Value:       ��
** Created  by:          YX
** Created  Datas:       2018-01-06
*********************************************************************************************************/
uint8_t ComThread_GetSendFlashDataState(void)
{
    return send_flash_data_state;
}

/*******************************************************************************************************
** Function name:        ComThread_Init
** Descriptions:         ͨ���̳߳�ʼ������
** input parameter:      ��     
** Returned Value:       ��
** Created  by:          ����
** Created  Datas:       2017-08-04
*********************************************************************************************************/
void ComThread_Init(void)
{
    // ����ͨ���̵߳��ź�����Դ���߳�
    SEMAPHORE_CREATE_BINARY_RET(com_res_s.m_upload_adc_sem);
    SEMAPHORE_CREATE_BINARY_RET(com_res_s.m_all_flash_sem);
    SEMAPHORE_CREATE_BINARY_RET(com_res_s.m_pedometer_data_sem);
    SEMAPHORE_CREATE_BINARY_RET(com_res_s.m_wechat_data_sem);
    SEMAPHORE_CREATE_BINARY_RET(com_res_s.m_hourglass_app);
    SEMAPHORE_CREATE_BINARY_RET(com_res_s.m_movement_sem);
    SEMAPHORE_CREATE_BINARY_RET(com_res_s.m_daily_data_sem);
    SEMAPHORE_CREATE_BINARY_RET(com_res_s.m_sleep_data_sem);    
    SEMAPHORE_CREATE_BINARY_RET(com_res_s.m_oxygen_data_sem);
    SEMAPHORE_CREATE_BINARY_RET(com_res_s.m_scatter_heart_sem);
    SEMAPHORE_CREATE_BINARY_RET(com_res_s.m_alarm_data_sem);
    SEMAPHORE_CREATE_BINARY_RET(com_res_s.m_heart_app);
    SEMAPHORE_CREATE_BINARY_RET(com_res_s.m_oxygen_app);
    SEMAPHORE_CREATE_BINARY_RET(com_res_s.m_fatigue_app);
    SEMAPHORE_CREATE_BINARY_RET(com_res_s.m_resp_app);
    SEMAPHORE_CREATE_BINARY_RET(com_res_s.m_bp_app);
	
    // ���������ź���,��ʼֵΪ0,�ź������ֵΪ10
    com_res_s.m_task_count = xSemaphoreCreateCounting(16,0);
    if (NULL == com_res_s.m_task_count)
    {
        APP_ERROR_HANDLER(NRF_ERROR_NO_MEM);
    }
    
    // �����߳�
    if(xTaskCreate(CommunicationThread_Handle, "Communication", 384, NULL, COMMUNICATION_THREAD_PRIORITY, &com_res_s.m_thread_handle) != pdPASS)  
    {
        APP_ERROR_HANDLER(NRF_ERROR_NO_MEM);
    }
}

/*******************************************************************************************************
** Function name:        Ble_ClkTimeOutAutoRepeat
** Descriptions:         ����CLK����ʱ�Զ��ط����ݽӿ�
** input parameter:      p_data ��������ָ��
**                       len    ���ͳ���
** Returned Value:       void
** Created  by:          GT
** Created  Datas:       2017-08-12
*********************************************************************************************************/
static void Ble_ClkTimeOutAutoRepeat(uint8_t* p_data,uint8_t len)
{
    uint32_t err_code = NRF_SUCCESS;
	
    for(uint8_t index = 0; index < CLK_DATA_TIMEOUT_REPEAT_CNT; index++)
    {
        err_code = ble_clk_sending(BleThread_GetBleClk(), p_data, len);
        if(err_code == BLE_ERROR_NO_TX_PACKETS)
        {
            vTaskDelay(20);
            continue;
        }
        else 
        {
            break;	 
        }
    }
}

/*******************************************************************************************************
** Function name:        Ble_BatTimeOutAutoRepeat
** Descriptions:         ����BAT����ʱ�Զ��ط����ݽӿ�
** input parameter:      p_data ��������ָ��
**                       len    ���ͳ���
** Returned Value:       void
** Created  by:          GT
** Created  Datas:       2017-08-12
*********************************************************************************************************/
static void Ble_BatTimeOutAutoRepeat(uint8_t* p_data,uint8_t len)
{
    uint32_t err_code = NRF_SUCCESS;
	
    for(uint8_t index = 0; index < BAT_DATA_TIMEOUT_REPEAT_CNT; index++)
    {
        err_code = ble_nus_string_send(BleThread_GetBleNus(), p_data, len);
        if(err_code == BLE_ERROR_NO_TX_PACKETS)
        {
            vTaskDelay(20);
            continue;
        }
        else 
        {
            break;	 
        }
    }
}

/*******************************************************************************************************
** Function name:        BLE_SendPedometerData
** Descriptions:         �������ͼǲ�ʵʱ����
** input parameter:      ��     
** Returned Value:       void
** Created  by:          GT
** Created  Datas:       2017-08-04
*********************************************************************************************************/
static void BLE_SendPedometerData(void)
{
    uint8_t ble_ack[20] = {0,};
    int16_t tData[3] = {0};
    uint32_t all_step = 0;
  
    ble_ack[0]=0xA8;           
  
    if(Gensor_Immediate_ADC(tData))
    {
        // 
        all_step = DataManage_GetCurrentStepCnt();
        ble_ack[1] = (uint8_t)((all_step >> 24) & 0xff);
        ble_ack[2] = (uint8_t)((all_step >> 16) & 0xff);
        ble_ack[3] = (uint8_t)((all_step >> 8) & 0xff);
        ble_ack[4] = (uint8_t)((all_step & 0xff));
        ble_ack[5] = 0;
        ble_ack[6] = tData[0];
        ble_ack[7] = tData[1];
        ble_ack[8] = tData[2];
        ble_ack[11] = tData[0]>>8;
        ble_ack[12] = tData[1]>>8;
        ble_ack[13] = tData[2]>>8;
        ble_ack[19] = 0xFF;

        Ble_SendDataViaBat(ble_ack, 20);
    }
}

/*******************************************************************************************************
** Function name:        BLE_SendHeartAppData
** Descriptions:         APP�������ʷ���ʵʱ��������
** input parameter:      ��   
** Returned Value:       void
** Created  by:          GT
** Created  Datas:       2017-08-04
*********************************************************************************************************/
uint8_t ct_from_detec = 0;
uint8_t heart_init_cause = 0;
uint8_t heart_calib_init_cause = 0;

extern uint32_t dac_from_4410;
extern uint32_t gain_from_4410;
extern uint32_t charge_from_4410;

static void BLE_SendHeartAppData(void)
{
    memset(com_upload_s.send_data,0,sizeof(com_upload_s.send_data));
//	volatile static bod_4404_adjust_t bod_4404_adjust;
	ct_from_detec = 0 ;
//	bod_4404_adjust.dac_grn = 0;
//    bod_4404_adjust = *Get_BocInfo();
    
//    ct_from_detec = bod_4404_adjust.dac_grn & 0x0F;
    
	com_upload_s.send_data[0] = 0xD0;
	com_upload_s.send_data[2] = ct_from_detec;
	com_upload_s.send_data[3] = ct_from_detec;
    
    com_upload_s.send_data[6] = heart_init_cause;
	com_upload_s.send_data[7] = heart_calib_init_cause;
    //gain
    com_upload_s.send_data[8] = (gain_from_4410);// & 0x07);
    //����DAC
    com_upload_s.send_data[9] = (dac_from_4410);// & 0x0F);
    //�̹�DAC
    com_upload_s.send_data[10] = (dac_from_4410);// & 0x01E0) >> 5;
    //amb dac
    com_upload_s.send_data[11] = (dac_from_4410);// & 0x03C00)>> 10;
    //red dac
    com_upload_s.send_data[12] = (dac_from_4410);// & 0x078000)>> 15;

    if(p_data_manage->heart_measure_s.init == 0)
    {
        com_upload_s.send_data[1] = p_data_manage->heart_measure_s.real_heart;
    }
	
    //����ֻ�������������Ѫ��
    if(p_data_manage->blood_oxygen_measure_s.btn == 1)
    {
         com_upload_s.send_data[5] = 0x04; 
         p_data_manage->heart_measure_s.app = 0;		    
    }
    // �ֻ�����Ѫѹ
    if(p_data_manage->bp_measure_s.bp_btn == 1)
    {
         com_upload_s.send_data[5] = 0x01; 
         p_data_manage->heart_measure_s.app = 0;	
    }
	
    // �����ͨ��
    if((p_flash_param->dev_config_s.led_gear == 0 && p_data_manage->wear_measure_s.wear_status == 1)||(!Afe4900_IsOpen()))
    {
        com_upload_s.send_data[1] = 0x01;
	    p_data_manage->heart_measure_s.app = 0;	
    }
    
    COMMUNICATION_LOG_PRINT("heart = %d\n",p_data_manage->heart_measure_s.real_heart);
    	
    Ble_SendDataViaBat(com_upload_s.send_data,20);
}

/*******************************************************************************************************
** Function name:        BLE_SendOxygenAppData
** Descriptions:         App����Ѫ������ʵʱ��������
** input parameter:      ��     
** Returned Value:       void
** Created  by:          GT
** Created  Datas:       2017-08-04
*********************************************************************************************************/
static void BLE_SendOxygenAppData(void)
{    
    uint32_t data_tmp = 0;
    bod_app_change_value_t app_medical_param_s = {0};
    
    app_medical_param_s = Get_AppChangeLibValue();
    memset(com_upload_s.send_data, 0, sizeof(com_upload_s.send_data));
    
    com_upload_s.send_data[0] = 0x80;
    com_upload_s.send_data[1] = 0x01; 
    //com_upload_s.send_data[3] = 99; 	
    
    if(p_data_manage->blood_oxygen_measure_s.test_mode == 4)
    {
        com_upload_s.send_data[3] = 0x01;
        com_upload_s.send_data[6] = p_data_manage->blood_oxygen_measure_s.test_mode;
        com_upload_s.send_data[4] = app_medical_param_s.ori_oxygen_value;
        com_upload_s.send_data[5] = app_medical_param_s.cur_out_oxygen_value;
        com_upload_s.send_data[7] = app_medical_param_s.ployfit_param_flag;
        com_upload_s.send_data[8] = app_medical_param_s.rechoose_ployfit_param_flag;
        com_upload_s.send_data[9] = app_medical_param_s.smooth_filter_times;
        com_upload_s.send_data[10] = app_medical_param_s.smooth_filter_points;
       
        data_tmp = app_medical_param_s.cur_quality_value * 10;
        com_upload_s.send_data[11] = data_tmp&0xff;
        com_upload_s.send_data[12] = (data_tmp&0xff00)>>8;
        com_upload_s.send_data[13] = (data_tmp&0xff0000)>>16;
        com_upload_s.send_data[14] = (data_tmp&0xff000000)>>24;
        
        data_tmp = app_medical_param_s.ployfit_diff_value * 10;
        com_upload_s.send_data[15] = data_tmp&0xff;
        com_upload_s.send_data[16] = (data_tmp&0xff00)>>8;
        com_upload_s.send_data[17] = (data_tmp&0xff0000)>>16;
        com_upload_s.send_data[18] = (data_tmp&0xff000000)>>24;
        
        //com_upload_s.send_data[19] = test_oxy_buf;
    }
    else
    {    
        //��Ѫ��У׼������У׼״̬�ͽ��ȸ�APP
        com_upload_s.send_data[4] = p_data_manage->blood_oxygen_measure_s.cal_state; 	
        com_upload_s.send_data[5] = p_data_manage->blood_oxygen_measure_s.cal_progress; 	
        
        // ��ȡѪ�����������У׼��״̬�ж�
        if(p_data_manage->blood_oxygen_measure_s.init == 0 && p_data_manage->blood_oxygen_measure_s.cal_state == 0)
        {
            com_upload_s.send_data[3] = p_data_manage->blood_oxygen_measure_s.result;	    
        }
    }
    
    //����ֻ�����������������
    if(p_data_manage->heart_measure_s.btn == 1)
    {
		com_upload_s.send_data[1] = 0x02; 
        com_upload_s.send_data[2] = 0x02; 
        p_data_manage->blood_oxygen_measure_s.app = 0;   	    
    }
    // �ֻ�����Ѫѹ
    if(p_data_manage->bp_measure_s.bp_btn == 1)
    {
		com_upload_s.send_data[1] = 0x02; 
        com_upload_s.send_data[2] = 0x01; 
        p_data_manage->blood_oxygen_measure_s.app = 0; 
    }
	
    // �����ͨ��
    if((p_flash_param->dev_config_s.led_gear == 0 && p_data_manage->wear_measure_s.wear_status == 1)||((!Afe4900_IsOpen())))
    {
        com_upload_s.send_data[3] = 0x01; 
        p_data_manage->blood_oxygen_measure_s.app = 0;   	    
    }
	
    Ble_SendDataViaBat(com_upload_s.send_data,20);
    
    if(p_data_manage->blood_oxygen_measure_s.test_mode == 4)
    {
        com_upload_s.send_data[3] = 0x02;
        com_upload_s.send_data[6] = p_data_manage->blood_oxygen_measure_s.test_mode;
        
        data_tmp = app_medical_param_s.cur_rate * 10;
        com_upload_s.send_data[7] = data_tmp&0xff;
        com_upload_s.send_data[8] = (data_tmp&0xff00)>>8;
        com_upload_s.send_data[9] = (data_tmp&0xff0000)>>16;
        com_upload_s.send_data[10] = (data_tmp&0xff000000)>>24;
        
        data_tmp = app_medical_param_s.calc_times;
        com_upload_s.send_data[11] = data_tmp&0xff;
        com_upload_s.send_data[12] = (data_tmp&0xff00)>>8;
        com_upload_s.send_data[13] = (data_tmp&0xff0000)>>16;
        com_upload_s.send_data[14] = (data_tmp&0xff000000)>>24;
        
        Ble_SendDataViaBat(com_upload_s.send_data,20);
    }
}

/*******************************************************************************************************
** Function name:        BLE_SendFatigueAppData
** Descriptions:         App����ƣ�Ͷȷ���ʵʱ��������
** input parameter:      ��     
** Returned Value:       void
** Created  by:          GT
** Created  Datas:       2017-08-04
*********************************************************************************************************/
static void BLE_SendFatigueAppData(void)
{
    uint8_t         measure_state = 0;
    tired_info_t    p_tired_info = {0};

    Tired_GetResult(&p_tired_info);    
    
    memset(com_upload_s.send_data,0,sizeof(com_upload_s.send_data));
	
    com_upload_s.send_data[0] = 0x81;
    com_upload_s.send_data[1] = 0x01;
    
    // �㷨���г�ʼ�����ٽ��м��
    if(p_data_manage->fatigue_measure_s.init == 0)
    {
        com_upload_s.send_data[4] = p_tired_info.tired_grade;
        com_upload_s.send_data[3] = (p_tired_info.progress*6)%101;

         // ���β�����Ч
        if((p_tired_info.tired_flag != 0)||((!Afe4900_IsOpen())))
        {
            p_data_manage->fatigue_measure_s.app = 0;
            com_upload_s.send_data[3] = 100;
            measure_state = 1;
            //BleThread_AFE4404DevClose();
        }	    
    }
    
    //����ֻ�������������Ѫ��
    if(p_data_manage->blood_oxygen_measure_s.btn == 1)
    {
         com_upload_s.send_data[2] = 0x04; 
         p_data_manage->fatigue_measure_s.app = 0;	    
    }
    
    //����ֻ�����������������
    if(p_data_manage->heart_measure_s.btn == 1)
    {
         com_upload_s.send_data[2] = 0x02; 
         p_data_manage->fatigue_measure_s.app = 0;	    
    }
    // �ֻ�����Ѫѹ
    if(p_data_manage->bp_measure_s.bp_btn == 1)
    {
         com_upload_s.send_data[2] = 0x01; 
         p_data_manage->fatigue_measure_s.app = 0;	
    }
    
    Ble_SendDataViaBat(com_upload_s.send_data,20);
    if(measure_state == 1)
    {
        BleThread_AFE4404DevClose();
    }
}

/*******************************************************************************************************
** Function name:        BLE_SendRespAppData
** Descriptions:         App���������ʷ���ʵʱ��������
** input parameter:      ��     
** Returned Value:       void
** Created  by:          GT
** Created  Datas:       2017-08-04
*********************************************************************************************************/
static void BLE_SendRespAppData(void)
{    
    memset(com_upload_s.send_data,0,sizeof(com_upload_s.send_data));
	
    com_upload_s.send_data[0] = 0x82;
    com_upload_s.send_data[1] = 0x01;
    com_upload_s.send_data[2] = 0;
    com_upload_s.send_data[3] = ((uint8_t)(p_data_manage->respiratory_rate_s.percent*1.67f))%101;

    if(p_data_manage->respiratory_rate_s.init == 0)
    {
        com_upload_s.send_data[4] = p_data_manage->respiratory_rate_s.result;
    }
    
    //����ֻ�������������Ѫ��
    if(p_data_manage->blood_oxygen_measure_s.btn == 1)
    {
         com_upload_s.send_data[2] = 0x04; 
         p_data_manage->respiratory_rate_s.app = 0;    
    }
    
    //����ֻ�����������������
    if(p_data_manage->heart_measure_s.btn == 1)
    {
         com_upload_s.send_data[2] = 0x02; 
         p_data_manage->respiratory_rate_s.app = 0;	    
    }
    // �ֻ�����Ѫѹ
    if(p_data_manage->bp_measure_s.bp_btn == 1)
    {
         com_upload_s.send_data[2] = 0x01; 
         p_data_manage->respiratory_rate_s.app = 0;	 
    }
	
    // �����ͨ��
    if(p_flash_param->dev_config_s.led_gear == 0 && p_data_manage->wear_measure_s.wear_status == 1)
    {
        com_upload_s.send_data[4] = 0x01;
        p_data_manage->respiratory_rate_s.app = 0;
    }
    com_upload_s.send_data[5] = p_data_manage->respiratory_rate_s.mode;
    if(p_data_manage->respiratory_rate_s.mode != 0)
    {//����ģʽ
		com_upload_s.send_data[7] = p_data_manage->respiratory_rate_s.resp_second_s.resp_rate_second;
		com_upload_s.send_data[8] = p_data_manage->respiratory_rate_s.resp_second_s.resp_amp_second;
		com_upload_s.send_data[9] = p_data_manage->respiratory_rate_s.resp_second_s.resp_ave_second;
		com_upload_s.send_data[10] = p_data_manage->respiratory_rate_s.resp_second_s.resp_rate_revise_second;
		com_upload_s.send_data[11] = p_data_manage->respiratory_rate_s.resp_second_s.resp_amp_revise_second;
		com_upload_s.send_data[12] = p_data_manage->respiratory_rate_s.resp_second_s.resp_ave_revise_second;
		com_upload_s.send_data[13] = p_data_manage->respiratory_rate_s.wave_station;
		com_upload_s.send_data[14] = p_data_manage->respiratory_rate_s.all_flag;

        if(p_data_manage->respiratory_rate_s.mode == 2)
        {
            com_upload_s.send_data[3] = 0;
            p_data_manage->respiratory_rate_s.percent = 0;
        }
    }
    
    if(p_data_manage->respiratory_rate_s.percent++ >= 60)
    {
        //����ʱ�䵽��,�ر�AFE4404	
//        BleThread_AFE4404DevClose();
        com_upload_s.send_data[1] = 2;
        p_data_manage->respiratory_rate_s.app = 0;    
    }
    
    Ble_SendDataViaBat(com_upload_s.send_data,20);
    if(com_upload_s.send_data[1] == 2)
    {    
        BleThread_AFE4404DevClose();
    }
}

/************************************************************************
** Function name:        BLE_SendBpAppData
** Descriptions:         APP����Ѫѹ����ʵʱ��������
** input parameter:      ��   
** Returned Value:       ��
** Created  by:          WJ
** Created  Datas:       2018-11-10
**************************************************************************/
static void BLE_SendBpAppData(void)
{    

    uint8_t bp_close_flag = 0;
    
    memset(com_upload_s.send_data, 0, sizeof(com_upload_s.send_data));
    
	com_upload_s.send_data[0] = 0x90;

    if(p_data_manage->bp_measure_s.bp_init == 0)
    {
        if(p_data_manage->bp_measure_s.bp_calc_prg < 25)
        {
            com_upload_s.send_data[3] = p_data_manage->bp_measure_s.bp_calc_prg*4;
        }
        else
        {
            if(p_data_manage->bp_measure_s.bp_flag == 1)
            {
                com_upload_s.send_data[1] = p_data_manage->bp_measure_s.bp_sp_value;
                com_upload_s.send_data[2] = p_data_manage->bp_measure_s.bp_dp_value;
            }
            else
            {
                com_upload_s.send_data[1] = 20;
                com_upload_s.send_data[2] = 30;
            }
            com_upload_s.send_data[3] = 100;
            bp_close_flag = 1;
        }
    }
	    //����ֻ�������������Ѫ��
    if(p_data_manage->blood_oxygen_measure_s.btn == 1)
    {
        //com_upload_s.send_data[1] = 30;
        //com_upload_s.send_data[2] = 20;
        //com_upload_s.send_data[3] = 100;
        com_upload_s.send_data[4] = 4;
        bp_close_flag = 1;  
    }
    
    //����ֻ�����������������
    if(p_data_manage->heart_measure_s.btn == 1)
    {
        com_upload_s.send_data[4] = 2;
        bp_close_flag = 1;   
    }
    
    // �����ͨ��
    if((p_flash_param->dev_config_s.led_gear == 0 && p_data_manage->wear_measure_s.wear_status == 1)
      || (!Afe4900_IsOpen()) )
    {
        com_upload_s.send_data[1] = 30;
        com_upload_s.send_data[2] = 20;
        com_upload_s.send_data[3] = 100;
        bp_close_flag = 1;
    }
    com_upload_s.send_data[5] = 0x01;   //��ʾ�н���
    
    Ble_BatTimeOutAutoRepeat(com_upload_s.send_data, 20);  
    if(bp_close_flag == 1)
    {   //�رղ���
        BleThread_AFE4404DevClose();
	    p_data_manage->bp_measure_s.bp_app = 0;	
    }
	
}


/*******************************************************************************************************
** Function name:        BLE_SendDailyAttrRecordData
** Descriptions:         App�����ֻ�,�ֻ����ϴ����������
** input parameter:      ��     
** Returned Value:       void
** Created  by:          GT
** Created  Datas:       2017-08-04
*********************************************************************************************************/
static void BLE_SendDailyAttrRecordData(void)
{
    daily_data_attr_t st_daily_attr = {0};
    flash_find_index_t*  p_flash_index = &p_flash_param->flash_index_s;
    uint8_t  local_day_id = (3 + p_flash_index->block_index - com_upload_s.daily.day_id)%3;
    uint16_t local_package_cnt = p_flash_index->daily.offset[local_day_id];
    
    com_upload_s.daily.total = local_package_cnt;
    
    if((com_upload_s.daily.start == 0)||(local_package_cnt < com_upload_s.daily.start))
    {
        memset(com_upload_s.send_data,0,sizeof(com_upload_s.send_data));
        
        com_upload_s.send_data[0] = 0xD1;
        com_upload_s.send_data[1] = (uint8_t)(local_package_cnt&0xFF);
        com_upload_s.send_data[2] = (uint8_t)(local_package_cnt>>8);
        com_upload_s.send_data[3] = (uint8_t)(local_package_cnt&0xFF);
        com_upload_s.send_data[4] = (uint8_t)(local_package_cnt>>8);
        com_upload_s.send_data[5] = (com_upload_s.daily.day_id << 5) & 0xE0;
        
        Ble_BatTimeOutAutoRepeat(com_upload_s.send_data,20);
        return;
    }
    
    // ���ݷ������,�رմ�����
    com_upload_s.daily.split = 0;
    for(uint16_t package = com_upload_s.daily.start; package <= com_upload_s.daily.total; package++)
    {
        Api_ReadDailyAttrData(local_day_id,package-1,&st_daily_attr);
                
	    // ͷ����Ϣ   
        com_upload_s.send_data[0] = 0xD1;
        com_upload_s.send_data[1] = package&0xFF;
        com_upload_s.send_data[2] = (package>>8)&0xFF;
        com_upload_s.send_data[3] = com_upload_s.daily.total&0xFF;
        com_upload_s.send_data[4] = (com_upload_s.daily.total>>8)&0xFF;
//        com_upload_s.send_data[5] = com_upload_s.daily.day_id;
        com_upload_s.send_data[5] = ((com_upload_s.daily.day_id << 5) & 0xE0) + (st_daily_attr.hour & 0x1F);
        
        memcpy(&com_upload_s.send_data[6],&st_daily_attr,sizeof(st_daily_attr));
        
        com_upload_s.send_data[6] = (uint8_t)(((st_daily_attr.calories)>>8)&0xFF);//(uint8_t)(((st_daily_attr.calories/100)>>8)&0xFF);
        com_upload_s.send_data[7] = (uint8_t)((st_daily_attr.calories)&0xFF);//(uint8_t)((st_daily_attr.calories/100)&0xFF);
        
        com_upload_s.send_data[8] = (uint8_t)((st_daily_attr.distance>>8)&0xFF);
        com_upload_s.send_data[9] = (uint8_t)(st_daily_attr.distance&0xFF);
        
//        memcpy(&com_upload_s.send_data[6],&st_daily_attr,sizeof(st_daily_attr));
        
        // ��������Э�鶨��ߵ��ֽڲ�ͬ,��Ҫת��
        com_upload_s.send_data[10] = (uint8_t)((st_daily_attr.step>>8)&0xFF);
        com_upload_s.send_data[11] = (uint8_t)(st_daily_attr.step&0xFF);
        com_upload_s.send_data[12] = (uint8_t)((st_daily_attr.sport>>8)&0xFF);
        com_upload_s.send_data[13] = (uint8_t)(st_daily_attr.sport&0xFF);
        
        com_upload_s.send_data[19] = st_daily_attr.minute;
	    
        Ble_BatTimeOutAutoRepeat(com_upload_s.send_data,20);
        
        // !ע��:������Ҫ���ָ�� Add by GT 2017-11-08
        com_upload_s.daily.start++;
        if(++com_upload_s.daily.split >= UPLOAD_RESPONSE_SPLIT_CNT)
        {
            if(com_upload_s.daily.start <= com_upload_s.daily.total)
            {
                // �����ź���,���ֶϵ���������
                xSemaphoreGive(com_res_s.m_daily_data_sem); 
                xSemaphoreGive(com_res_s.m_task_count);
            }
            break;
        }
    }
}

/*******************************************************************************************************
** Function name:        BLE_SendSleepData
** Descriptions:         App�����ֻ�,�ֻ����ϴ�˯������
** input parameter:      ��    
** Returned Value:       void
** Created  by:          GT
** Created  Datas:       2017-08-04
*********************************************************************************************************/
static void BLE_SendSleepData(void)
{
    uint8_t sleep_data_offset = 0;
    uint8_t local_day_id = 0;
    
    // ��ʼ������
    memset(com_upload_s.send_data,0,sizeof(com_upload_s.send_data));
    
    // ����˯��ID��
    local_day_id = (3 + p_flash_param->flash_index_s.block_index - com_upload_s.sleep.day_id)%3;
    
    // ����FLASH����
    com_upload_s.sleep.sleep_wake_cnt = Api_GetSleepWakeCnt(local_day_id);
    com_upload_s.sleep.total_package_cnt = com_upload_s.sleep.sleep_wake_cnt*6;
    
    com_upload_s.send_data[0] = 0xE0;
    com_upload_s.send_data[2] = com_upload_s.sleep.sleep_wake_cnt;
    com_upload_s.send_data[3] = com_upload_s.sleep.day_id;
    
    for(uint8_t package = com_upload_s.sleep.total_package_cnt; package > 0; package--)
    {
        // ��ȡ˯�߷ָ�����
        Api_GetSleepPartData(local_day_id,sleep_data_offset++,&com_upload_s.send_data[4]);
        
        // ������صı���,������Ҫ��������1 Modify by GT 2017-07-22
        com_upload_s.send_data[1] = package - 1;
        
        // ����˯�����ݵ�APP��
        Ble_BatTimeOutAutoRepeat(com_upload_s.send_data,20);
    }
    
    if(0 == com_upload_s.sleep.total_package_cnt)
    {
        // ���û��˯������,ֱ�ӷ��ͽ������ݵ�APP��
        memset(com_upload_s.send_data,0,sizeof(com_upload_s.send_data));
        com_upload_s.send_data[0] = 0xE0;
        com_upload_s.send_data[3] = com_upload_s.sleep.day_id;
        Ble_BatTimeOutAutoRepeat(com_upload_s.send_data,20);
    }
}

/*******************************************************************************************************
** Function name:        BLE_SendOxygenAttrRecordData
** Descriptions:         App�����ֻ�,�ֻ����ϴ�Ѫ���洢����
** input parameter:      ��     
** Returned Value:       void
** Created  by:          GT
** Created  Datas:       2017-08-04
*********************************************************************************************************/
static void BLE_SendOxygenAttrRecordData(void)
{
    static uint16_t  filter_num = 0;        // ������������ݸ���
    blood_oxygen_attr_t  st_oxygen_attr = {0};
    flash_find_index_t*  p_flash_index = &p_flash_param->flash_index_s;
    uint8_t  local_day_id = (3 + p_flash_index->block_index - com_upload_s.oxygen.day_id)%3;
    uint16_t local_package_cnt = p_flash_index->oxygen.offset[local_day_id];    // ȫ��Ѫ�����ݸ���
    uint16_t local_package_cnt1 = local_package_cnt;        // ����������еĸ���
    // �ճ�����
    daily_data_attr_t st_daily_attr = {0};
    uint16_t d_local_package_cnt = p_flash_index->daily.offset[local_day_id];

    // �����ճ������д洢��Ѫ������
    local_package_cnt += p_flash_index->daily.ox_value_cnt[local_day_id];
    
    com_upload_s.oxygen.total = local_package_cnt;
    
    if((com_upload_s.oxygen.start == 0) || (local_package_cnt < com_upload_s.oxygen.start))
    {
        memset(com_upload_s.send_data, 0, sizeof(com_upload_s.send_data));
        
        com_upload_s.send_data[0] = 0xD2;
        com_upload_s.send_data[1] = (uint8_t)(local_package_cnt&0xFF);
        com_upload_s.send_data[2] = (uint8_t)(local_package_cnt>>8);
        com_upload_s.send_data[3] = (uint8_t)(local_package_cnt&0xFF);
        com_upload_s.send_data[4] = (uint8_t)(local_package_cnt>>8);
        com_upload_s.send_data[5] = com_upload_s.oxygen.day_id;
        
        Ble_BatTimeOutAutoRepeat(com_upload_s.send_data,20);
        return;
    }
    
    // ���ݷ������,�رմ�����
    com_upload_s.oxygen.split = 0;
    for(uint16_t package = com_upload_s.oxygen.start; package <= com_upload_s.oxygen.total; package++)
    {
        if(package <= local_package_cnt1)
        {
            Api_ReadOxygenAttrData(local_day_id, package-1, &st_oxygen_attr);
            filter_num = 0;
        }
        else
        {
            if(com_upload_s.oxygen.last_index == 1)
            {
                filter_num = package - local_package_cnt1 - 1;
            }
            
            for(uint16_t i = com_upload_s.oxygen.last_index; i <= d_local_package_cnt; i++)
            {
                Api_ReadDailyAttrData(local_day_id, i - 1, &st_daily_attr);  
                com_upload_s.oxygen.last_index++;
                if(st_daily_attr.hour >= 7 && st_daily_attr.hour < 24 && st_daily_attr.ox_value != 0)
                {
                    if(filter_num == 0)
                    {
                        break;
                    }
                    else
                    {
                        filter_num--;
                    }
                }
            }
            if(com_upload_s.oxygen.last_index > d_local_package_cnt)
            {
                package = com_upload_s.oxygen.total;
                com_upload_s.oxygen.start = package;
            }
        }
                
        com_upload_s.send_data[0] = 0xD2;
        com_upload_s.send_data[1] = package&0xFF;
        com_upload_s.send_data[2] = (package>>8)&0xFF;
        com_upload_s.send_data[3] = com_upload_s.oxygen.total&0xFF;
        com_upload_s.send_data[4] = (com_upload_s.oxygen.total>>8)&0xFF;
        com_upload_s.send_data[5] = com_upload_s.oxygen.day_id;
        
        if(package <= local_package_cnt1)
        {
            memcpy(&com_upload_s.send_data[6],&st_oxygen_attr, 14);
        }
        else
        {
            com_upload_s.send_data[6] = st_daily_attr.hour;
            com_upload_s.send_data[7] = st_daily_attr.minute;
            com_upload_s.send_data[8] = st_daily_attr.ox_value;
            memset(&com_upload_s.send_data[9], 0, 11);
        }
        
        Ble_BatTimeOutAutoRepeat(com_upload_s.send_data,20);
        
        // !ע��:������Ҫ���ָ�� Add by GT 2017-11-08
        com_upload_s.oxygen.start++;
        if(++com_upload_s.oxygen.split >= UPLOAD_RESPONSE_SPLIT_CNT)
        {
            if(com_upload_s.oxygen.start <= com_upload_s.oxygen.total)
            {
                // �����ź���,���ֶϵ���������
                xSemaphoreGive(com_res_s.m_oxygen_data_sem); 
                xSemaphoreGive(com_res_s.m_task_count);
            }
            break;
        }
    }
}

static void BLE_SendScatterHeartData(void)
{
    scatter_heart_t  st_scatter_heart = {0};
    blood_oxygen_attr_t  st_oxygen_attr = {0};
    flash_find_index_t*  p_flash_index = &p_flash_param->flash_index_s;
    uint8_t  local_day_id = (3 + p_flash_index->block_index - com_upload_s.sheart.day_id)%3;
    uint16_t local_package_cnt = p_flash_index->oxygen.offset[local_day_id];
    
    com_upload_s.sheart.total = local_package_cnt;
    
    if((com_upload_s.sheart.start == 0)||(local_package_cnt < com_upload_s.sheart.start))
    {
        memset(com_upload_s.send_data,0,sizeof(com_upload_s.send_data));
        
        com_upload_s.send_data[0] = 0xD9;
        com_upload_s.send_data[1] = (uint8_t)(local_package_cnt&0xFF);
        com_upload_s.send_data[2] = (uint8_t)(local_package_cnt>>8);
        com_upload_s.send_data[3] = (uint8_t)(local_package_cnt&0xFF);
        com_upload_s.send_data[4] = (uint8_t)(local_package_cnt>>8);
        com_upload_s.send_data[5] = com_upload_s.sheart.day_id;
        
        Ble_BatTimeOutAutoRepeat(com_upload_s.send_data,20);
        return;
    }
    
    // ���ݷ������,�رմ�����
    com_upload_s.sheart.split = 0;
    for(uint16_t package = com_upload_s.sheart.start; package <= com_upload_s.sheart.total; package++)
    {
        Api_ReadScatterHeartData(local_day_id, package-1, &st_scatter_heart);
        Api_ReadOxygenAttrData(local_day_id,package-1,&st_oxygen_attr);
                
        com_upload_s.send_data[0] = 0xD9;
        com_upload_s.send_data[1] = package&0xFF;
        com_upload_s.send_data[2] = (package>>8)&0xFF;
        com_upload_s.send_data[3] = com_upload_s.sheart.total&0xFF;
        com_upload_s.send_data[4] = (com_upload_s.sheart.total>>8)&0xFF;
        com_upload_s.send_data[5] = com_upload_s.sheart.day_id;
        
        // �ϴ�ʱ����
        com_upload_s.send_data[6] = st_oxygen_attr.hour;
        com_upload_s.send_data[7] = st_oxygen_attr.minute;
        memcpy(&com_upload_s.send_data[9],&st_scatter_heart, 10);
        com_upload_s.send_data[19] = st_scatter_heart.resv[1];
        // ����HRV
        com_upload_s.send_data[8] = st_oxygen_attr.hrv;
        
        Ble_BatTimeOutAutoRepeat(com_upload_s.send_data,20);
        
        // !ע��:������Ҫ���ָ�� Add by GT 2017-11-08
        com_upload_s.sheart.start++;
        if(++com_upload_s.sheart.split >= UPLOAD_RESPONSE_SPLIT_CNT)
        {
            if(com_upload_s.sheart.start <= com_upload_s.sheart.total)
            {
                // �����ź���,���ֶϵ���������
                xSemaphoreGive(com_res_s.m_scatter_heart_sem); 
                xSemaphoreGive(com_res_s.m_task_count);
            }
            break;
        }
    }
}

/*******************************************************************************************************
** Function name:        BLE_SendMovementData
** Descriptions:         �����˶�ģʽ����
** input parameter:      ��    
** Returned Value:       void
** Created  by:          GT
** Created  Datas:       2017-08-04
*********************************************************************************************************/
static void BLE_SendMovementData(void)
{
    uint8_t *movement_head_p = NULL;
    uint16_t c_pack_num = 0;
    movement_patterns_head_t movement_head_temp_s = {0x00,};
    movement_patterns_data_t movement_data_temp_s = {0x00,};         //��ǰһ���Ӽ�¼��Ϣ         
    
    if(com_upload_s.movement.total > 300)
    {
        com_upload_s.movement.total = 0;
    }
    
    if(com_upload_s.movement.day_id >= MOVEMENT_PATTERNS_MAX_BLOCK
      || com_upload_s.movement.start >= com_upload_s.movement.total)
    {        
        com_upload_s.send_data[0] = 0xD4;
        com_upload_s.send_data[5] = com_upload_s.movement.day_id;
        Ble_BatTimeOutAutoRepeat(com_upload_s.send_data,20);  
        return ; 
    }
    com_upload_s.movement.split = 0;
    while(1)
    {
        if(com_upload_s.movement.start < MOVEMENT_HEAD_MAX_PACK_NUM)
        {   
            //ͷ����Ϣ
            App_FlashReadMovementHead(com_upload_s.movement.day_id, (void *)&movement_head_temp_s, sizeof(movement_patterns_head_t));       
            if(movement_head_temp_s.flag != 0x00 || movement_head_temp_s.crc == 0x00 || movement_head_temp_s.crc == 0xFFFF)
            {
                return ;
            }
            com_upload_s.movement.start++;
            com_upload_s.send_data[0] = 0xD4;
            com_upload_s.send_data[1] = com_upload_s.movement.start & 0xFF;
            com_upload_s.send_data[2] = (com_upload_s.movement.start >> 8) & 0xFF;
            com_upload_s.send_data[3] = com_upload_s.movement.total & 0xFF;
            com_upload_s.send_data[4] = (com_upload_s.movement.total >> 8) & 0xFF;
            com_upload_s.send_data[5] = com_upload_s.movement.day_id;
            
            movement_head_p = (uint8_t *)&movement_head_temp_s;
            movement_head_p += (com_upload_s.movement.start - 1) * 14;
            memcpy(&com_upload_s.send_data[6], movement_head_p, 14);    

            Ble_BatTimeOutAutoRepeat(com_upload_s.send_data,20);         
        }
        else
        {
            //һ������Ϣ
            c_pack_num = com_upload_s.movement.start - MOVEMENT_HEAD_MAX_PACK_NUM;
            App_FlashReadMovementData(com_upload_s.movement.day_id, c_pack_num, (uint8_t *)&movement_data_temp_s);        
            com_upload_s.movement.start++;
            com_upload_s.send_data[1] = com_upload_s.movement.start & 0xFF;
            com_upload_s.send_data[2] = (com_upload_s.movement.start >> 8) & 0xFF;
            com_upload_s.send_data[3] = com_upload_s.movement.total & 0xFF;
            com_upload_s.send_data[4] = (com_upload_s.movement.total >> 8) & 0xFF;
            com_upload_s.send_data[5] = com_upload_s.movement.day_id;
            memcpy(&com_upload_s.send_data[6], &movement_data_temp_s.heart, 14);

            Ble_BatTimeOutAutoRepeat(com_upload_s.send_data,20);           
        }
        if(com_upload_s.movement.start >= com_upload_s.movement.total)
        {
            return ;
        }
        if(++com_upload_s.movement.split >= UPLOAD_RESPONSE_SPLIT_CNT)
        {
            if(com_upload_s.movement.start < com_upload_s.movement.total)
            {
                // �����ź���,���ֶϵ���������
                xSemaphoreGive(com_res_s.m_movement_sem); 
                xSemaphoreGive(com_res_s.m_task_count);
            }
            return ;
        }
    }
}

/*******************************************************************************************************
** Function name:        BLE_SendAlarmData
** Descriptions:         App�����ֻ�,�ֻ����ϴ�20����������
** input parameter:      ��     
** Returned Value:       void
** Created  by:          GT
** Created  Datas:       2017-08-04
*********************************************************************************************************/
static void BLE_SendAlarmData(void)
{
    uint8_t alarm_index = 0;
    uint8_t total_package = 0;
    uint8_t send_package_index = 0;
    uint8_t valid_index[20] = {0};
    uint8_t send_data[20] = {0};
    uint16_t crc = Api_GetAlarmCRC();
    const alarm_time_config_t* p_current_alarm = NULL;
    
    send_data[0] = 0xB9;
    send_data[1] = 0x01;
    send_data[4] = 0x02;

    for(uint8_t index = 0; index < 20; index++)
    {
        p_current_alarm = &p_flash_param->alarm_time_s[index];
        if(1 == p_current_alarm->valid)
        {
            valid_index[total_package] = index;
            total_package++;
        }
    }    
    
    if(total_package > 0)
    {
        for(uint8_t index = 0; index < total_package; index++)
        {
            //��ȡ��Ϣ
            alarm_index = valid_index[index];
            send_data[2] = send_package_index+1;
            send_data[3] = total_package;
            send_data[5] = alarm_index+1;
            send_data[6] = p_flash_param->alarm_time_s[alarm_index].hour;
            send_data[7] = p_flash_param->alarm_time_s[alarm_index].minute;
            send_data[8] = p_flash_param->alarm_time_s[alarm_index].state;
            send_data[9] = p_flash_param->alarm_time_s[alarm_index].repeat;
            send_data[10] = p_flash_param->alarm_time_s[alarm_index].scence;
            send_data[11] = p_flash_param->alarm_time_s[alarm_index].year&0xFF;
            send_data[12] = p_flash_param->alarm_time_s[alarm_index].year>>8;
            send_data[13] = p_flash_param->alarm_time_s[alarm_index].month;
            send_data[14] = p_flash_param->alarm_time_s[alarm_index].day;
            send_data[18] = (uint8_t)(crc&0xFF);
            send_data[19] = (uint8_t)(crc>>8);
            Ble_BatTimeOutAutoRepeat(send_data, 20);
            
            //���Ͱ�����һ
            send_package_index++;
        }
    }
    else
    {
        //���﷢��һ�������
        Ble_BatTimeOutAutoRepeat(send_data, 20);
    }
}

/*******************************************************************************************************
** Function name:        BLE_SendADCData
** Descriptions:         App�����ֻ�,�ֻ����ϴ�ADC����ԭʼ����
** input parameter:      ��     
** Returned Value:       void
** Created  by:          GT
** Created  Datas:       2017-08-04
*********************************************************************************************************/
static void BLE_SendADCData(void)
{    
    if(p_data_manage->upload == UPLOAD_ADC_GSENSOR_EVENT)
    {
		ComThread_SetSendFlashDataState(1);
        while(p_data_manage->acc.adc_send_len >= 3)
        {
            memset(com_upload_s.send_data,0,sizeof(com_upload_s.send_data));
            DataManage_GetGsensorADCData((int16_t*)com_upload_s.send_data);
            Ble_ClkTimeOutAutoRepeat(com_upload_s.send_data, 20);
        }
    }
    else if((p_data_manage->upload == UPLOAD_ADC_OXYGEN_EVENT) 
			&& (p_data_manage->blood_oxygen_measure_s.app == 1 
		|| p_data_manage->respiratory_rate_s.app == 1)
		|| (p_data_manage->test_ox_info_s.flag 
		&& p_data_manage->test_ox_info_s.mode == 1))  
    {
		ComThread_SetSendFlashDataState(1);
        while(p_data_manage->data_pool.send_wait_len >= 5)
        {
            memset(com_upload_s.send_data,0,sizeof(com_upload_s.send_data));
            DataManage_GetOxygenADCData(&com_upload_s.send_data[1],&com_upload_s.send_data[21],&com_upload_s.send_data[41],&com_upload_s.send_data[61]);
            
//            //YX.201808028.ȥ���̹������
            com_upload_s.send_data[0] = 0x82;
            Ble_ClkTimeOutAutoRepeat(com_upload_s.send_data, 20);								            
            com_upload_s.send_data[20] = 0x83;
            Ble_ClkTimeOutAutoRepeat(&com_upload_s.send_data[20], 20); 
            com_upload_s.send_data[40] = 0x84;
            Ble_ClkTimeOutAutoRepeat(&com_upload_s.send_data[40], 20);
            //YX.201808028.ȥ�������������
            com_upload_s.send_data[60] = 0x85;
            Ble_ClkTimeOutAutoRepeat(&com_upload_s.send_data[60], 20);
					
			//WJ.20180629.Ѫ���Զ��������ݲɼ���������һ�εĿ�ʼ
            Test_OxygenTimesEndRepeat();
        }
				
		//WJ.20180629.Ѫ���Զ��������ݲɼ���������һ�εĿ�ʼ
        Test_OxygenTimesEndRepeat();
    }
    else if((p_data_manage->upload == UPLOAD_ADC_GREEN_EVENT) && (p_data_manage->heart_measure_s.app == 1))
    {
		ComThread_SetSendFlashDataState(1);
        while(p_data_manage->data_pool.send_wait_len >= 5)
        {
            DataManage_GetHeartADCData(&com_upload_s.send_data[1], &com_upload_s.send_data[21]);
									
            com_upload_s.send_data[0] = 0x82;
            Ble_ClkTimeOutAutoRepeat(com_upload_s.send_data, 20);
            com_upload_s.send_data[20] = 0x85;
            Ble_ClkTimeOutAutoRepeat(&com_upload_s.send_data[20], 20);
        }
    }
    else if(p_data_manage->upload == UPLOAD_ADC_FATIGUE_EVENT)
    {
       
    } 
}

/*******************************************************************************************************
** Function name:        Ble_SendAllFlashData
** Descriptions:         App�����ֻ�,�ֻ����ϴ�����FLASHԭʼ����
** input parameter:      ��     
** Returned Value:       void
** Created  by:          GT
** Created  Datas:       2017-08-04
*********************************************************************************************************/
static void Ble_SendAllFlashData(void)
{
    uint32_t  flash_data[5] = {0};
    uint8_t*  p_flash_data = (uint8_t*)flash_data;
    
    ComThread_SetSendFlashDataState(1);
    
    // �����ȡFLASH �ճ�����/˯������/Ѫ������
    for(uint8_t block_id = 0; block_id < 112; block_id++)//132//96;
    {
        for(uint8_t offset = 0; offset < 64; offset++)
        {
            memset(flash_data,0,sizeof(flash_data));
            
            Api_ReadAllFlashData(block_id,offset,&p_flash_data[1]);
            
            //������ת��ΪЭ������
            p_flash_data[0] = 0xF0;
            p_flash_data[17] = offset;
            p_flash_data[18] = block_id;
            Ble_BatTimeOutAutoRepeat(p_flash_data,20);
        }
    }
}

/*******************************************************************************************************
** Function name:        Ble_SendWechatData
** Descriptions:         App�����ֻ�,�ֻ���ʵʱ�ϴ�΢�żǲ�����
** input parameter:      ��     
** Returned Value:       void
** Created  by:          GT
** Created  Datas:       2017-08-04
*********************************************************************************************************/
static void Ble_SendWechatData(void)
{
    ble_wechat_send_data(DataManage_GetCurrentStepCnt());
}

/*******************************************************************************************************
** Function name:        Ble_SendHourglassData
** Descriptions:         App�����ֻ�,�ֻ���ʵʱ�ϴ�����ʱ����
** input parameter:      ��     
** Returned Value:       void
** Created  by:          GT
** Created  Datas:       2017-08-04
*********************************************************************************************************/
static void Ble_SendHourglassData(void)
{
    memset(com_upload_s.send_data,0,sizeof(com_upload_s.send_data));
    
    com_upload_s.send_data[0] = 0xB2;
    if((p_flash_param->hourglass_s.app_trigger == 1)||(p_flash_param->hourglass_s.btn_trigger == 1))
    {
        com_upload_s.send_data[1] = 3;
    }
    else
    {
        com_upload_s.send_data[1] = 4;
        p_flash_param->hourglass_s.app_send = 0;
    }
    
    com_upload_s.send_data[2] = 1;
    
    if(p_flash_param->hourglass_s.ui_enable == 1)
    {
        com_upload_s.send_data[7] = 1;
        if(p_flash_param->hourglass_s.btn_trigger == 1)
        {
            com_upload_s.send_data[11] = 1;
        }
    }
    com_upload_s.send_data[4] = p_flash_param->hourglass_s.dev_remain_time&0xFF;
    com_upload_s.send_data[5] = (p_flash_param->hourglass_s.dev_remain_time>>8)&0xFF;
    com_upload_s.send_data[6] = (p_flash_param->hourglass_s.dev_remain_time>>16)&0xFF;
    com_upload_s.send_data[8] = p_flash_param->hourglass_s.run_remain_time&0xFF;
    com_upload_s.send_data[9] = (p_flash_param->hourglass_s.run_remain_time>>8)&0xFF;
    com_upload_s.send_data[10] = (p_flash_param->hourglass_s.run_remain_time>>16)&0xFF;
    
    Ble_SendDataViaBat(com_upload_s.send_data,20);
}

/*******************************************************************************************************
** Function name:        CommunicationThread_Handle
** Descriptions:         ͨ���߳������� 
** input parameter:      arg--��Ч����  
** Returned Value:       ��
** Created  by:          GT
** Created  Datas:       2017-08-04
*********************************************************************************************************/
static void CommunicationThread_Handle(void* arg)
{    
    UNUSED_PARAMETER(arg);
        
    while(1)
    {  	    
        // ע��:���ﴦ����Ҫ���ȼ�,����APP���ղ�������
        THREAD_MUTEX_TASK_EXCUTE(com_res_s.m_upload_adc_sem,com_res_s.m_task_count,BLE_SendADCData);
        
        // �߳�ִ�з���΢����������
        THREAD_MUTEX_TASK_EXCUTE(com_res_s.m_wechat_data_sem,com_res_s.m_task_count,Ble_SendWechatData);
        
        // �߳�ִ�з��͵���ʱ��������
        THREAD_MUTEX_TASK_EXCUTE(com_res_s.m_hourglass_app,com_res_s.m_task_count,Ble_SendHourglassData);

        // ����������������
        THREAD_MUTEX_TASK_EXCUTE(com_res_s.m_alarm_data_sem,com_res_s.m_task_count,BLE_SendAlarmData);
        
        // ��������ʵʱ��������
        THREAD_MUTEX_TASK_EXCUTE(com_res_s.m_heart_app,com_res_s.m_task_count,BLE_SendHeartAppData);
        
        // ����Ѫ��ʵʱ��������
        THREAD_MUTEX_TASK_EXCUTE(com_res_s.m_oxygen_app,com_res_s.m_task_count,BLE_SendOxygenAppData);
        
        // ����ƣ�Ͷ�ʵʱ��������
        THREAD_MUTEX_TASK_EXCUTE(com_res_s.m_fatigue_app,com_res_s.m_task_count,BLE_SendFatigueAppData);
	 
        // ���ͺ���������
        THREAD_MUTEX_TASK_EXCUTE(com_res_s.m_resp_app,com_res_s.m_task_count,BLE_SendRespAppData);
        
        // ���ͼǲ�ʵʱ����
        THREAD_MUTEX_TASK_EXCUTE(com_res_s.m_pedometer_data_sem,com_res_s.m_task_count,BLE_SendPedometerData);
        
        // �߳�ִ�з����ճ���������
        THREAD_MUTEX_TASK_EXCUTE(com_res_s.m_daily_data_sem,com_res_s.m_task_count,BLE_SendDailyAttrRecordData);
        
        // �����˶�ģʽ��������
        THREAD_MUTEX_TASK_EXCUTE(com_res_s.m_movement_sem,com_res_s.m_task_count,BLE_SendMovementData);
        
        // ����FLASH˯��ȫ����������
        THREAD_MUTEX_TASK_EXCUTE(com_res_s.m_sleep_data_sem,com_res_s.m_task_count,BLE_SendSleepData);
        
        // ����Ѫ��ȫ����������
        THREAD_MUTEX_TASK_EXCUTE(com_res_s.m_oxygen_data_sem,com_res_s.m_task_count,BLE_SendOxygenAttrRecordData);
        
        // ����HRVɢ��ͼ��������
        THREAD_MUTEX_TASK_EXCUTE(com_res_s.m_scatter_heart_sem,com_res_s.m_task_count,BLE_SendScatterHeartData);
        
        // ����FLASHԭʼ����
        THREAD_MUTEX_TASK_EXCUTE(com_res_s.m_all_flash_sem,com_res_s.m_task_count,Ble_SendAllFlashData);
    
        // ����Ѫѹʵʱ��������
        THREAD_MUTEX_TASK_EXCUTE(com_res_s.m_bp_app, com_res_s.m_task_count, BLE_SendBpAppData);
        
        ComThread_SetSendFlashDataState(0);
        
        App_Watch_Dog_Reload(); //1��ι��1��
        
        /* !ע�⣺������Ҫ�ж��㷨������Ƿ������,ֻ�����������,���ܹ����߳�
        *         ������ﷵ��pdFALSEʱ,˵���߳�û�е������� 
        */
        if(pdFALSE == xSemaphoreTake(com_res_s.m_task_count,(TickType_t)0))
        {
            vTaskSuspend(com_res_s.m_thread_handle);
        }
        else
        {
            //����ʱ��ֹ�������Ļ�������������
            vTaskDelay(31);
        }
    }
}

/*******************************************************************************************************
** Function name:        ComThread_SendDailyData
** Descriptions:         ����ͨ���̷߳������������������
** input parameter:      p_data--Э������,��ϸ�ο�ͨ��Э�� 
** Returned Value:       void
** Created  by:          GT
** Created  Datas:       2017-08-04
*********************************************************************************************************/
void ComThread_SendDailyData(uint8_t* p_data)
{
    com_upload_s.daily.start = p_data[1] | (p_data[2]<<8);
    com_upload_s.daily.day_id = p_data[3];
    THREAD_TASK_WAKE_UP(com_res_s.m_daily_data_sem,com_res_s.m_task_count,com_res_s.m_thread_handle);
}

void ComThread_SendScatterHeartData(uint8_t* p_data)
{
    com_upload_s.sheart.start = p_data[1] | (p_data[2]<<8);
    com_upload_s.sheart.day_id = p_data[3];
    THREAD_TASK_WAKE_UP(com_res_s.m_scatter_heart_sem,com_res_s.m_task_count,com_res_s.m_thread_handle);
}


void ComThread_SendMovementData(void)
{
    com_upload_s.movement.start = movement_send_s.movement_num_start;
    com_upload_s.movement.total = movement_send_s.movement_num_total;
    com_upload_s.movement.day_id = movement_send_s.movement_block;
    xSemaphoreGive(com_res_s.m_movement_sem);
    xSemaphoreGive(com_res_s.m_task_count);
    if(xSemaphoreTake(com_res_s.m_task_count,(TickType_t)0) == pdTRUE)
    {
        vTaskResume(com_res_s.m_thread_handle);
    }
//    THREAD_TASK_WAKE_UP(com_res_s.m_movement_sem,com_res_s.m_task_count,com_res_s.m_thread_handle);
}

/*******************************************************************************************************
** Function name:        ComThread_SendADCData
** Descriptions:         ����ͨ���̷߳���ADC����������
** input parameter:      ��
** Returned Value:       void
** Created  by:          GT
** Created  Datas:       2017-08-04
*********************************************************************************************************/
void ComThread_SendADCData(void)
{
    if(BleThread_GetBleClk()->conn_handle != BLE_CONN_HANDLE_INVALID)
    {
        THREAD_TASK_WAKE_UP(com_res_s.m_upload_adc_sem,com_res_s.m_task_count,com_res_s.m_thread_handle);
    }
}

/*******************************************************************************************************
** Function name:        ComThread_SendAllFlashData
** Descriptions:         ����ͨ���̷߳��Ͷ�ȡ�û�FLASH����������
** input parameter:      ��
** Returned Value:       void
** Created  by:          GT
** Created  Datas:       2017-08-04
*********************************************************************************************************/
void ComThread_SendAllFlashData(void)
{
    THREAD_TASK_WAKE_UP(com_res_s.m_all_flash_sem,com_res_s.m_task_count,com_res_s.m_thread_handle);
}

/*******************************************************************************************************
** Function name:        ComThread_SendSleepData
** Descriptions:         ����ͨ���̷߳���˯�ߴ洢����������
** input parameter:      p_data--Э�����ݣ���ϸ�ο�ͨ��Э��
** Returned Value:       void
** Created  by:          GT
** Created  Datas:       2017-08-04
*********************************************************************************************************/
void ComThread_SendSleepData(uint8_t* p_data)
{
    com_upload_s.sleep.day_id = p_data[1];
    THREAD_TASK_WAKE_UP(com_res_s.m_sleep_data_sem,com_res_s.m_task_count,com_res_s.m_thread_handle);
}

/*******************************************************************************************************
** Function name:        ComThread_SendOxygenData
** Descriptions:         ����ͨ���̷߳���Ѫ���洢����������
** input parameter:      p_data--Э�����ݣ���ϸ�ο�ͨ��Э��
** Returned Value:       void
** Created  by:          GT
** Created  Datas:       2017-08-04
*********************************************************************************************************/
void ComThread_SendOxygenData(uint8_t* p_data)
{
    com_upload_s.oxygen.start = p_data[1] | (p_data[2]<<8);
    com_upload_s.oxygen.day_id = p_data[3];
    com_upload_s.oxygen.last_index = 1;
    THREAD_TASK_WAKE_UP(com_res_s.m_oxygen_data_sem,com_res_s.m_task_count,com_res_s.m_thread_handle);
}

/*******************************************************************************************************
** Function name:        ComThread_SendAlarmData
** Descriptions:         ����ͨ���̷߳�������洢����������
** input parameter:      ��
** Returned Value:       void
** Created  by:          GT
** Created  Datas:       2017-08-04
*********************************************************************************************************/
void ComThread_SendAlarmData(void)
{
    THREAD_TASK_WAKE_UP(com_res_s.m_alarm_data_sem,com_res_s.m_task_count,com_res_s.m_thread_handle);
}

void ComThread_PeriodCycleTask(uint8_t hour, uint8_t minute, uint8_t second)
{
    if((BleThread_GetBleNus()->conn_handle != BLE_CONN_HANDLE_INVALID)&&(p_data_manage->device_trace_param_s.sign_on == 1))
    {
        // �ϴ�΢���˶�����
        xSemaphoreGive(com_res_s.m_wechat_data_sem);
        xSemaphoreGive(com_res_s.m_task_count);	    

        // �ϴ��ǲ����
        if(p_data_manage->app_upload_control_s.pedometer == 1)
        {
            xSemaphoreGive(com_res_s.m_pedometer_data_sem); 
            xSemaphoreGive(com_res_s.m_task_count);
        }	 

        // �ϴ���������
        if(p_data_manage->heart_measure_s.app == 1)
        {
            xSemaphoreGive(com_res_s.m_heart_app); 
            xSemaphoreGive(com_res_s.m_task_count);
        }	
		
        // �ϴ�Ѫѹ����
        if(p_data_manage->bp_measure_s.bp_app == 1)
        {
            xSemaphoreGive(com_res_s.m_bp_app); 
            xSemaphoreGive(com_res_s.m_task_count);
        }
		
        // �ϴ�Ѫ��APP��������
        if(p_data_manage->blood_oxygen_measure_s.app == 1)
        {
            xSemaphoreGive(com_res_s.m_oxygen_app);
            xSemaphoreGive(com_res_s.m_task_count);
        } 

        if(p_data_manage->respiratory_rate_s.app == 1)
        {
            xSemaphoreGive(com_res_s.m_resp_app);
            xSemaphoreGive(com_res_s.m_task_count);
        } 	 

        // !ע��:������ҪAPP�������ƣ�Ͷ�&&ƣ�Ͷ��㷨�Ѿ����е���,��������APP�޷��ڶ��β��� Modify by GT 2017-07-25
        if(p_data_manage->fatigue_measure_s.app == 1)
        {
            xSemaphoreGive(com_res_s.m_fatigue_app);
            xSemaphoreGive(com_res_s.m_task_count);
        }  

        // �����������ʱ����,��ʱ����ʵʱ����
        if(p_flash_param->hourglass_s.app_send  == 1)
        {
            xSemaphoreGive(com_res_s.m_hourglass_app);
            xSemaphoreGive(com_res_s.m_task_count);
        }    
        
//        // !ע��:������Ҫ�жϼ����ź����Ƿ�������,������������������߳�
//        if(xSemaphoreTake(com_res_s.m_task_count,(TickType_t)0) == pdTRUE)
//        {
//            // �����������ź���(��һ)����,������Ҫ��һ��ԭ
//            xSemaphoreGive(com_res_s.m_task_count);
//            vTaskResume(com_res_s.m_thread_handle); 
//        }
        vTaskResume(com_res_s.m_thread_handle);        
    }
         
}

