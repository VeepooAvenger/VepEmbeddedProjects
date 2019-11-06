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
#include "ble_wechat_service.h"
#include "configuration.h"
#include "log.h"
#include "app_config_data.h"
#include "movement_patterns.h"
#include "watch_dog.h"
#include "app_crc16.h"
#include "GSensor.h"
#include "api_ext_flash.h"
#include "test_state_machine.h"
#include "si1182_driver.h"

#include "gui_thread.h"
#include "backlight_driving.h"


#define ADC_SEND_BLE_BAT        0       //adc����������ͨ��

static communication_resource_t  com_res_s;
static com_upload_param_t        com_upload_s;

static uint8_t send_flash_data_state = 0;

// ��������
static void CommunicationThread_Handle(void* arg);
void ComThread_UploadSwitchDataTask(void);

/************************************************************************
** Function name:        ComThread_SetSendFlashDataState
** Descriptions:         ���÷���FLASHԭʼ���ݵ�״̬
** input parameter:      ��     
** Returned Value:       ��
** Created  by:          YX
** Created  Datas:       2018-01-06
**************************************************************************/
void ComThread_SetSendFlashDataState(uint8_t state)
{
    send_flash_data_state = state;
}

/************************************************************************
** Function name:        ComThread_GetSendFlashDataState
** Descriptions:         ��ȡ����FLASHԭʼ���ݵ�״̬
** input parameter:      ��     
** Returned Value:       ��
** Created  by:          YX
** Created  Datas:       2018-01-06
**************************************************************************/
uint8_t ComThread_GetSendFlashDataState(void)
{
    return send_flash_data_state;
}

/************************************************************************
** Function name:        ComThread_Init
** Descriptions:         ͨ���̳߳�ʼ������
** input parameter:      ��     
** Returned Value:       ��
** Created  by:          ����
** Created  Datas:       2017-08-04
**************************************************************************/
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
    SEMAPHORE_CREATE_BINARY_RET(com_res_s.m_alarm_data_sem);
    
    SEMAPHORE_CREATE_BINARY_RET(com_res_s.m_heart_app);
    SEMAPHORE_CREATE_BINARY_RET(com_res_s.m_ecg_app);
    SEMAPHORE_CREATE_BINARY_RET(com_res_s.m_bp_app);
    SEMAPHORE_CREATE_BINARY_RET(com_res_s.m_oxygen_app);
    
    SEMAPHORE_CREATE_BINARY_RET(com_res_s.m_ecg_data_sem);
	SEMAPHORE_CREATE_BINARY_RET(com_res_s.m_power_info_sem);
    SEMAPHORE_CREATE_BINARY_RET(com_res_s.m_update_set_sem);
    
    // ���������ź���, ��ʼֵΪ0, �ź������ֵΪ12
    com_res_s.m_task_count = xSemaphoreCreateCounting(17, 0);
    if (NULL == com_res_s.m_task_count)
    {
        APP_ERROR_HANDLER(NRF_ERROR_NO_MEM);
    }
    
    // �����߳�
    if(xTaskCreate(CommunicationThread_Handle, "Communication", 256/*384*/, NULL, COMMUNICATION_THREAD_PRIORITY, &com_res_s.m_thread_handle) != pdPASS)  
    {
        APP_ERROR_HANDLER(NRF_ERROR_NO_MEM);
    }
}

/************************************************************************
** Function name:        Ble_ClkTimeOutAutoRepeat
** Descriptions:         ����CLK����ʱ�Զ��ط����ݽӿ�
** input parameter:      p_data ��������ָ��
**                       len    ���ͳ���
** Returned Value:       void
** Created  by:          GT
** Created  Datas:       2017-08-12
**************************************************************************/
static void Ble_ClkTimeOutAutoRepeat(uint8_t* p_data, uint8_t len)
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

/************************************************************************
** Function name:        Ble_BatTimeOutAutoRepeat
** Descriptions:         ����BAT����ʱ�Զ��ط����ݽӿ�
** input parameter:      p_data ��������ָ��
**                       len    ���ͳ���
** Returned Value:       void
** Created  by:          GT
** Created  Datas:       2017-08-12
**************************************************************************/
static void Ble_BatTimeOutAutoRepeat(uint8_t* p_data, uint8_t len)
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

/************************************************************************
** Function name:        BLE_SendPedometerData
** Descriptions:         �������ͼǲ�ʵʱ����
** input parameter:      ��     
** Returned Value:       void
** Created  by:          GT
** Created  Datas:       2017-08-04
**************************************************************************/
static void BLE_SendPedometerData(void)
{
    uint8_t     ble_ack[20] = {0, };
    int16_t     gsenor_data[3] = {0};
    uint32_t    all_step = 0;
  
    ble_ack[0]=0xA8;           
  
    Gsensor_GetData(gsenor_data);
    //if(Gensor_Immediate_ADC(gsenor_data))
    {
        // 
        all_step = DataManage_GetCurrentStepCnt();
        ble_ack[1] = (uint8_t)((all_step >> 24) & 0xff);
        ble_ack[2] = (uint8_t)((all_step >> 16) & 0xff);
        ble_ack[3] = (uint8_t)((all_step >> 8) & 0xff);
        ble_ack[4] = (uint8_t)((all_step & 0xff));
        ble_ack[5] = 0;
        ble_ack[6] = gsenor_data[0];
        ble_ack[7] = gsenor_data[1];
        ble_ack[8] = gsenor_data[2];
        ble_ack[11] = gsenor_data[0]>>8;
        ble_ack[12] = gsenor_data[1]>>8;
        ble_ack[13] = gsenor_data[2]>>8;
        ble_ack[19] = 0xFF;

        Ble_SendDataViaBat(ble_ack, 20);
    }
}

/************************************************************************
** Function name:        BLE_SendHeartAppData
** Descriptions:         APP�������ʷ���ʵʱ��������
** input parameter:      ��   
** Returned Value:       ��
** Created  by:          WJ
** Created  Datas:       2018-10-22
**************************************************************************/
static void BLE_SendHeartAppData(void)
{
    memset(com_upload_s.send_data, 0, sizeof(com_upload_s.send_data));
    
	com_upload_s.send_data[0] = 0xD0;

    if(p_data_manage->ecg_ppg_s.ppg_measure_s.init == 0)
    {
        com_upload_s.send_data[1] = p_data_manage->ecg_ppg_s.ppg_measure_s.p_second_heart;
    }
	
    // ���ֶ������������
    if(p_data_manage->ecg_ppg_s.ecg_ppg_mode != MEASURE_MODE_PPG
      || p_data_manage->ecg_ppg_s.ppg_measure_s.btn_bp == 1
      || p_data_manage->ecg_ppg_s.ppg_measure_s.hreart_state == 1)
    {
        com_upload_s.send_data[1] = 0x01;
	    p_data_manage->ecg_ppg_s.ppg_measure_s.app = 0;	
        p_data_manage->ecg_ppg_s.ppg_measure_s.hreart_state = 0;
    }
    
    // �����ͨ��
    if((p_flash_param->dev_config_s.led_gear == 0 && p_data_manage->wear_measure_s.wear_status == 1)
      || (p_data_manage->ecg_ppg_s.afe4900_is_open != 1) )
    {
        com_upload_s.send_data[1] = 0x01;
	    p_data_manage->ecg_ppg_s.ppg_measure_s.app = 0;	
    }
    
    Ble_BatTimeOutAutoRepeat(com_upload_s.send_data, 20);
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

    if(p_data_manage->ecg_ppg_s.ppg_measure_s.init == 0 
       && p_data_manage->ecg_ppg_s.ppg_measure_s.calc_bp_init == 0)
    {
        if(p_data_manage->ecg_ppg_s.ppg_measure_s.bp_progress < PPG_BP_CALC_TIME)
        {
            com_upload_s.send_data[3] = p_data_manage->ecg_ppg_s.ppg_measure_s.bp_progress*4;
        }
        else
        {
            if(p_data_manage->ecg_ppg_s.ppg_measure_s.p_bp_flag == 1)
            {
                com_upload_s.send_data[1] = p_data_manage->ecg_ppg_s.ppg_measure_s.p_bp_high_25sec;
                com_upload_s.send_data[2] = p_data_manage->ecg_ppg_s.ppg_measure_s.p_bp_low_25sec;
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
	
    // ���ֶ������������
    if(p_data_manage->ecg_ppg_s.ecg_ppg_mode != MEASURE_MODE_PPG
      || p_data_manage->ecg_ppg_s.ppg_measure_s.btn == 1
      || p_data_manage->ecg_ppg_s.ppg_measure_s.btn_bp == 1
      || (p_data_manage->ecg_ppg_s.ppg_measure_s.calc_bp_init == 0 && p_data_manage->ecg_ppg_s.ppg_measure_s.bp_progress > PPG_BP_CALC_TIME)
      || p_data_manage->ecg_ppg_s.ppg_measure_s.bp_progress > 200)
    {
        com_upload_s.send_data[1] = 30;
        com_upload_s.send_data[2] = 20;
        com_upload_s.send_data[3] = 100;
        bp_close_flag = 1;
    }
    
    // �����ͨ��
    if((p_flash_param->dev_config_s.led_gear == 0 && p_data_manage->wear_measure_s.wear_status == 1)
      || (p_data_manage->ecg_ppg_s.afe4900_is_open != 1) )
    {
        com_upload_s.send_data[1] = 30;
        com_upload_s.send_data[2] = 20;
        com_upload_s.send_data[3] = 100;
        bp_close_flag = 1;
    }
    com_upload_s.send_data[5] = 0x01;   //��ʾ�н���
    
    Ble_BatTimeOutAutoRepeat(com_upload_s.send_data, 20);  
    if(bp_close_flag == 1)
    {
        Statemachine_DelState(MEAT_PPG_APP_BP);
	    p_data_manage->ecg_ppg_s.ppg_measure_s.app_bp = 0;	
    }
}

/************************************************************************
** Function name:        BLE_SendEcgAppData
** Descriptions:         APP����Ecg����ʵʱ��������
** input parameter:      ��   
** Returned Value:       ��
** Created  by:          WJ
** Created  Datas:       2018-10-22
**************************************************************************/
static void BLE_SendEcgAppData(void)
{
    static uint8_t      ecg_send_flag = 0;
    ecg_measure_t       *p_ecg_measure = NULL;
    
	p_ecg_measure = &p_data_manage->ecg_ppg_s.ecg_measure_s;
    
    memset(com_upload_s.send_data, 0, sizeof(com_upload_s.send_data));
    
	com_upload_s.send_data[0] = 0x93;
	com_upload_s.send_data[1] = 0x01;
	com_upload_s.send_data[2] = 0x01;
	com_upload_s.send_data[3] = 0x01;
    //�ֻ�״̬
	com_upload_s.send_data[4] = 0x00;

    if(p_ecg_measure->init == 0)
    {
        com_upload_s.send_data[5] = p_ecg_measure->e_second_heart;
        com_upload_s.send_data[6] = p_ecg_measure->e_minute_heart;
        com_upload_s.send_data[7] = p_ecg_measure->e_second_hrv;
        com_upload_s.send_data[8] = p_ecg_measure->e_second_rr;
        com_upload_s.send_data[9] = p_ecg_measure->e_6sec_rr;
        com_upload_s.send_data[10] = p_ecg_measure->e_second_br;
        com_upload_s.send_data[11] = p_ecg_measure->e_minute_br;
        com_upload_s.send_data[12] = p_ecg_measure->e_wear_flag;
        com_upload_s.send_data[13] = p_ecg_measure->e_m_id;
        com_upload_s.send_data[14] = p_ecg_measure->e_qtc_time & 0xFF;
        com_upload_s.send_data[15] = (p_ecg_measure->e_qtc_time>>8) & 0xFF;
        
        if(p_ecg_measure->e_app_calc_cnt >= 90)//60)
        {
            com_upload_s.send_data[19] = 100;
        }
        else
        {
            com_upload_s.send_data[19] = p_ecg_measure->e_app_calc_cnt *1.11f;//1.66f;
            ecg_send_flag = 0;
        }        
    
        //
        if(ecg_send_flag == 0 && p_ecg_measure->result_progress >= 30)
        {
            ecg_send_flag = 1;
            com_upload_s.send_data[20] = 0x93;
            com_upload_s.send_data[21] = 0x01;
            com_upload_s.send_data[22] = 0x01;
            com_upload_s.send_data[23] = 0x02;
      
            memcpy(&com_upload_s.send_data[24], &p_ecg_measure->ecg_test_result_s.lead_off_type, 14);
            com_upload_s.send_data[38] = 0;
            com_upload_s.send_data[39] = p_ecg_measure->e_app_calc_cnt *1.11f;//1.66f;
            if(p_ecg_measure->e_app_calc_cnt >= 90)//60)
            {
                com_upload_s.send_data[39] = 100;
            }
        }            
    }
    else
    {
        com_upload_s.send_data[7] = 255;//p_ecg_measure->e_second_hrv;
    }

    // ���ֶ�����������ϻ��߳�ʱ
    if(p_data_manage->ecg_ppg_s.ecg_ppg_mode != MEASURE_MODE_ECG 
      || p_ecg_measure->btn == 1) // �����ֶ��������app����
    {
        //�ֻ�״̬
        com_upload_s.send_data[4] = 0xFF;
	    p_ecg_measure->app = 0;	
    }
  
    if(ecg_send_flag == 1)
    {   //������Ͻ��
        ecg_send_flag = 2;
        Ble_BatTimeOutAutoRepeat(&com_upload_s.send_data[20], 20);
        //Statemachine_DelState(MEAT_ECG_APP_HEART);
	    //p_ecg_measure->app = 0;	
    }
    Ble_BatTimeOutAutoRepeat(com_upload_s.send_data, 20);

    if(p_ecg_measure->app == 1 && p_ecg_measure->e_app_calc_cnt >= 90)//60)
    {
        Statemachine_DelState(MEAT_ECG_APP_HEART);
	    p_ecg_measure->app = 0;	
        
        com_upload_s.send_data[40] = 0x93;
        com_upload_s.send_data[41] = 0x01;
        com_upload_s.send_data[42] = 0x01;
        if(ecg_send_flag == 2)
        {
            com_upload_s.send_data[43] = 0x04;      //���Խ��� 
        }
        else
        {
            com_upload_s.send_data[43] = 0x03;      //���Գ�ʱ
        }
        Ble_BatTimeOutAutoRepeat(&com_upload_s.send_data[40], 20);       
    }
}


/************************************************************************
** Function name:        BLE_SendEcgAppData
** Descriptions:         APP����Ecg����ʵʱ��������
** input parameter:      ��   
** Returned Value:       ��
** Created  by:          WJ
** Created  Datas:       2018-12-28
**************************************************************************/
static void BLE_SendOxygenAppData(void)
{
    oxygen_measure_t    *p_oxygen_measure = NULL;
    
	p_oxygen_measure = &p_data_manage->ecg_ppg_s.oxygen_measure_s;
    
    memset(com_upload_s.send_data, 0, sizeof(com_upload_s.send_data));
    
    com_upload_s.send_data[0] = 0x80;
    com_upload_s.send_data[1] = 0x01; 
    
    //��Ѫ��У׼������У׼״̬�ͽ��ȸ�APP
    com_upload_s.send_data[4] = 0;      //p_data_manage->blood_oxygen_measure_s.cal_state; 	
    com_upload_s.send_data[5] = 100;    //p_data_manage->blood_oxygen_measure_s.cal_progress; 	
    
    // ��ȡѪ�����
    if(p_oxygen_measure->ox_init == 0)
    {
        com_upload_s.send_data[3] = p_oxygen_measure->ox_result;	    
    }

    // ���ֶ������������
    if(p_data_manage->ecg_ppg_s.ecg_ppg_mode != MEASURE_MODE_OXYGEN)
    {
		com_upload_s.send_data[1] = 0x02; 
        com_upload_s.send_data[2] = 0x02; 
        p_oxygen_measure->ox_app = 0; 
    }
    
    // �����ͨ��
    if((p_flash_param->dev_config_s.led_gear == 0 && p_data_manage->wear_measure_s.wear_status == 1)
      || (p_data_manage->ecg_ppg_s.afe4900_is_open != 1) )
    {
        com_upload_s.send_data[3] = 0x01; 
        p_oxygen_measure->ox_app = 0; 
    }
	
    Ble_SendDataViaBat(com_upload_s.send_data, 20); 
}

/************************************************************************
** Function name:        BLE_SendDailyAttrRecordData
** Descriptions:         App�����ֻ�, �ֻ����ϴ����������
** input parameter:      ��     
** Returned Value:       void
** Created  by:          GT
** Created  Datas:       2017-08-04
**************************************************************************/
static void BLE_SendDailyAttrRecordData(void)
{   
    uint8_t                 send_daily_data_cnt = 0;       
    uint8_t                 send_daily_data_buf[RECORD_DAILY_PER_DATA_SIZE + 20 + 8] = {0};
    daily_data_attr_t       st_daily_attr = {0};
    flash_find_index_t      *p_flash_index = &p_flash_param->flash_index_s;
    uint8_t                 local_day_id = (3 + p_flash_index->block_index - com_upload_s.daily.day_id)%3;
    uint16_t                local_package_cnt = p_flash_index->daily.offset[local_day_id]; 
    uint16_t                crc_value = 0xFFFF;
    
    uint8_t                 send_data_index = 0;
    uint8_t                 send_data_pack = 0;
    
    com_upload_s.daily.total = local_package_cnt;
    
    if((com_upload_s.daily.start == 0)||(local_package_cnt < com_upload_s.daily.start))
    {
        memset(com_upload_s.send_data, 0, sizeof(com_upload_s.send_data));
        
        com_upload_s.send_data[0] = 0xDF;
        com_upload_s.send_data[1] = 0xFF;
        com_upload_s.send_data[2] = 0xFF;
        com_upload_s.send_data[3] = (uint8_t)(local_package_cnt&0xFF);
        com_upload_s.send_data[4] = (uint8_t)(local_package_cnt>>8);
        com_upload_s.send_data[5] = com_upload_s.daily.data_type;
        com_upload_s.send_data[6] = 0x00;   //���� 
        com_upload_s.send_data[7] = com_upload_s.daily.day_id;
        
        Ble_BatTimeOutAutoRepeat(com_upload_s.send_data, 20);
        return;
    }
    
    // ���ݷ������, �رմ�����
    com_upload_s.daily.split = 0;
    for(uint16_t package = com_upload_s.daily.start; package <= com_upload_s.daily.total; package++)
    {
        uint32_t read_time_out = 0;
        while(Extflash_DailyReadSet(local_day_id, package - 1, (uint8_t *)&st_daily_attr, sizeof(st_daily_attr), 0) == 0)
        {
            vTaskDelay(1);
            read_time_out++;
            if(read_time_out > 1200)
            {   //��ʱ����������
                return;
            }  
        }
        while(Extflash_DailyReadIsComplete() == 0)
        {
            vTaskDelay(1);
            read_time_out++;
            if(read_time_out > 1200)
            {   //��ʱ����������
                return;
            }
        }
//        Api_ReadDailyAttrData(local_day_id, package - 1, &st_daily_attr);
        
        //���ݴ���    
    {   
        send_data_index = 0;
        send_data_pack = 0;
        send_daily_data_cnt = 0;
        //����
        send_daily_data_buf[send_daily_data_cnt++] = 0xB1;
        send_daily_data_buf[send_daily_data_cnt++] = 4;
        memcpy(&send_daily_data_buf[send_daily_data_cnt], &st_daily_attr.month, 4);
        send_daily_data_cnt += 4;
        //�Ʋ����˶��������롢��·���̬�����
        send_daily_data_buf[send_daily_data_cnt++] = 0xB2;
        send_daily_data_buf[send_daily_data_cnt++] = 10;
        send_daily_data_buf[send_daily_data_cnt++] = (st_daily_attr.step >> 8) & 0xFF;
        send_daily_data_buf[send_daily_data_cnt++] = st_daily_attr.step & 0xFF;
        send_daily_data_buf[send_daily_data_cnt++] = (st_daily_attr.sport >> 8) & 0xFF;
        send_daily_data_buf[send_daily_data_cnt++] = st_daily_attr.sport & 0xFF;
        send_daily_data_buf[send_daily_data_cnt++] = (st_daily_attr.distance >> 8)&0xFF;
        send_daily_data_buf[send_daily_data_cnt++] = st_daily_attr.distance & 0xFF;
        send_daily_data_buf[send_daily_data_cnt++] = (st_daily_attr.calories >> 8) & 0xFF;
        send_daily_data_buf[send_daily_data_cnt++] = (st_daily_attr.calories) & 0xFF;
        send_daily_data_buf[send_daily_data_cnt++] = st_daily_attr.gesture;
        send_daily_data_buf[send_daily_data_cnt++] = st_daily_attr.wear;
        //˯��״̬
        send_daily_data_buf[send_daily_data_cnt++] = 0xB3;
        send_daily_data_buf[send_daily_data_cnt++] = 6;
        memcpy(&send_daily_data_buf[send_daily_data_cnt], &st_daily_attr.sleep_state[0], 6);
        send_daily_data_cnt += 6;
        //����
        send_daily_data_buf[send_daily_data_cnt++] = 0xB4;
        send_daily_data_buf[send_daily_data_cnt++] = 5;
        memcpy(&send_daily_data_buf[send_daily_data_cnt], &st_daily_attr.pulse_frequency[0], 5);
        send_daily_data_cnt += 5;
        //����
        send_daily_data_buf[send_daily_data_cnt++] = 0xB5;
        send_daily_data_buf[send_daily_data_cnt++] = 5;
        memcpy(&send_daily_data_buf[send_daily_data_cnt], &st_daily_attr.heart_rate[0], 5);
        send_daily_data_cnt += 5;
        //���������
        send_daily_data_buf[send_daily_data_cnt++] = 0xB6;
        send_daily_data_buf[send_daily_data_cnt++] = 5;
        memcpy(&send_daily_data_buf[send_daily_data_cnt], &st_daily_attr.breathing_rate[0], 5);
        send_daily_data_cnt += 5;
        //hrv
        send_daily_data_buf[send_daily_data_cnt++] = 0xB7;
        send_daily_data_buf[send_daily_data_cnt++] = 51;
        send_daily_data_buf[send_daily_data_cnt++] = st_daily_attr.hrv_type;
        memcpy(&send_daily_data_buf[send_daily_data_cnt], &st_daily_attr.hrv_value[0], 50);
        send_daily_data_cnt += 50;
        
        //Ѫѹ
        send_daily_data_buf[send_daily_data_cnt++] = 0xB8;
        send_daily_data_buf[send_daily_data_cnt++] = 2;
        send_daily_data_buf[send_daily_data_cnt++] = st_daily_attr.bp_h;
        send_daily_data_buf[send_daily_data_cnt++] = st_daily_attr.bp_l;
        
        //Ѫ�����
        send_daily_data_buf[send_daily_data_cnt++] = 0xB9;
        send_daily_data_buf[send_daily_data_cnt++] = 30;
        memcpy(&send_daily_data_buf[send_daily_data_cnt], &st_daily_attr.oxgen_value[0], 30);      
        send_daily_data_cnt += 30;    
        // WJ.20190509.Ѫ������ʹ��Ĭ��ֵ��
        send_daily_data_buf[send_daily_data_cnt-5] = 0;
        send_daily_data_buf[send_daily_data_cnt-4] = 0;
        send_daily_data_buf[send_daily_data_cnt-3] = 0;
        send_daily_data_buf[send_daily_data_cnt-2] = 0;
        send_daily_data_buf[send_daily_data_cnt-1] = 0;
        //end
        
        send_daily_data_buf[send_daily_data_cnt++] = 0xBA;
        send_daily_data_buf[send_daily_data_cnt++] = 5;
        // WJ.20190722.�޸��������������
        memcpy(&send_daily_data_buf[send_daily_data_cnt], &st_daily_attr.spo2_modification_flag[0], 5);      
        send_daily_data_cnt += 5;  

        
        crc_value = App_Crc16(send_daily_data_buf, send_daily_data_cnt);
        send_data_pack = send_daily_data_cnt / 16;
        if(send_daily_data_cnt & 0x0F)
        {
            send_data_pack++;
        }
    }    
	    // ͷ����Ϣ   
        com_upload_s.send_data[0] = 0xDF;
        //C_P
        com_upload_s.send_data[1] = package & 0xFF;
        com_upload_s.send_data[2] = (package>>8) & 0xFF;
  
    {   //��һ��ͷ����Ϣ  
        //D_T
        com_upload_s.send_data[3] = 0x00;      
        //T_P
        com_upload_s.send_data[4] = com_upload_s.daily.total & 0xFF;
        com_upload_s.send_data[5] = (com_upload_s.daily.total >> 8) & 0xFF;
        //D_T_S
        com_upload_s.send_data[6] = send_data_pack;
        //Day
        com_upload_s.send_data[7] = com_upload_s.daily.day_id;
        //Read_Type
        com_upload_s.send_data[8] = com_upload_s.daily.data_type;
        //Data_Type:����ʵ��������
        com_upload_s.send_data[9] = 0;//com_upload_s.daily.data_type;
        //Count:���ݳ���
        com_upload_s.send_data[10] = send_daily_data_cnt;
        //CRC:
        com_upload_s.send_data[11] = crc_value & 0xFF;
        com_upload_s.send_data[12] = (crc_value >> 8) & 0xFF;
    }
        Ble_BatTimeOutAutoRepeat(com_upload_s.send_data, 20);
        
        for(uint8_t i = 1; i <= send_data_pack; i++)
        {
            //D_T
            com_upload_s.send_data[3] = i;
            memcpy(&com_upload_s.send_data[4], &send_daily_data_buf[send_data_index], 16);
            send_data_index += 16;
            
            Ble_BatTimeOutAutoRepeat(com_upload_s.send_data, 20);
        }
        com_upload_s.daily.start++;

        // ���ӶϿ����ڴ���
        if(BleThread_GetBleNus()->conn_handle == BLE_CONN_HANDLE_INVALID)
        {
            return;
        }              
        
        //if(++com_upload_s.daily.split >= UPLOAD_RESPONSE_SPLIT_CNT)
        {
            //WJ.20181025.�ܰ�����1��Ϊ�˷��ͽ�����
            if(com_upload_s.daily.start <= com_upload_s.daily.total + 1)
            {
                // �����ź���, ���ֶϵ���������
                xSemaphoreGive(com_res_s.m_daily_data_sem); 
                xSemaphoreGive(com_res_s.m_task_count);
            }
            break;
        }
    }
}

/************************************************************************
** Function name:        BLE_SendSleepData
** Descriptions:         App�����ֻ�, �ֻ����ϴ�˯������
** input parameter:      ��    
** Returned Value:       ��
** Created  by:          WJ
** Created  Datas:       2018-10-15
**************************************************************************/
static void BLE_SendSleepData(void)
{
    uint8_t     send_buf_index = 0;                                 //����buf��ʼ������ݵ��±�
    uint8_t     temp_len = 0;                                       //��¼��ʱ����
    uint8_t     local_day_id = 0;
    uint8_t     sleep_read_index = 0;
    uint16_t    sleep_data_total_size = 0;
    uint16_t    sleep_size_buf[SLEEP_ONE_DAY_MAX_TIMES + 1] = {0};       //ÿ��˯�ߵĴ�С
    uint32_t    sleep_offset_buf[SLEEP_ONE_DAY_MAX_TIMES + 1] = {0};     //ÿ��˯�ߵ�ƫ�Ƶ�ַ
    uint32_t    sleep_data_offset = 0;
    
    // ��ʼ������
    memset(com_upload_s.send_data, 0, sizeof(com_upload_s.send_data));
    
    // ����˯��ID��
    local_day_id = (3 + p_flash_param->flash_index_s.block_index - com_upload_s.sleep.day_id)%3;
    
    // ����FLASH����
    //com_upload_s.sleep.sleep_wake_cnt = Api_GetSleepWakeCnt(local_day_id);
    Api_GetSleepStoreInfo(local_day_id, &com_upload_s.sleep.sleep_wake_cnt, sleep_size_buf, sleep_offset_buf);
    
    for(uint8_t i = 0; i < com_upload_s.sleep.sleep_wake_cnt; i++)
    {	//JSH.δ�ۼ�����
        sleep_data_total_size += sleep_size_buf[i];
    }
    //�ܼƴ��������������������211������
    com_upload_s.sleep.total_package_cnt = sleep_data_total_size / 16;
    if(sleep_data_total_size % 16)
    {
        com_upload_s.sleep.total_package_cnt++;
    }
    
    com_upload_s.send_data[0] = 0xE0;
    com_upload_s.send_data[2] = com_upload_s.sleep.sleep_wake_cnt;
    com_upload_s.send_data[3] = com_upload_s.sleep.day_id;
    
    sleep_read_index = 0;
    temp_len = 0;
    send_buf_index = 4;
    for(uint8_t package = com_upload_s.sleep.total_package_cnt; package > 0;)
    {
        // ��ȡ˯�߷ָ�����
        if(sleep_data_offset < sleep_offset_buf[sleep_read_index + 1] )
        {
            if(sleep_data_offset + (16 - temp_len) <= sleep_offset_buf[sleep_read_index] + sleep_size_buf[sleep_read_index])
            {
                Api_GetSleepPartData(local_day_id, sleep_data_offset, &com_upload_s.send_data[send_buf_index], (16 - temp_len));
                sleep_data_offset += (16 - temp_len);
            }
            else 
            {
                if(sleep_data_offset < sleep_offset_buf[sleep_read_index] + sleep_size_buf[sleep_read_index])
                {
                    temp_len = sleep_offset_buf[sleep_read_index] + sleep_size_buf[sleep_read_index] - sleep_data_offset;
                    Api_GetSleepPartData(local_day_id, sleep_data_offset, &com_upload_s.send_data[send_buf_index], temp_len);
                    send_buf_index += temp_len;
                    
                    //�ȰѲ��㲿�����0
                    memset(&com_upload_s.send_data[send_buf_index], 0, 20 - send_buf_index);
                }
                else
                {
                    temp_len = 0;
                }
                sleep_read_index++;
                if(sleep_read_index < com_upload_s.sleep.sleep_wake_cnt)
                {
                    sleep_data_offset = sleep_offset_buf[sleep_read_index];
                    continue;
                }
            }
        }
        else
        {   //����
            sleep_read_index++;
            continue;
        }
         
        com_upload_s.send_data[1] = package - 1;
        
        // ����˯�����ݵ�APP��
        Ble_BatTimeOutAutoRepeat(com_upload_s.send_data, 20);
        send_buf_index = 4;
        temp_len = 0;
        package--;

        // ���ӶϿ����ڴ���
        if(BleThread_GetBleNus()->conn_handle == BLE_CONN_HANDLE_INVALID)
        {
            return;
        }      
    }

    if(com_upload_s.sleep.total_package_cnt == 0)
    {
        // ���û��˯������, ֱ�ӷ��ͽ������ݵ�APP��
        memset(com_upload_s.send_data, 0, sizeof(com_upload_s.send_data));
        com_upload_s.send_data[0] = 0xE0;
        com_upload_s.send_data[3] = com_upload_s.sleep.day_id;
        Ble_BatTimeOutAutoRepeat(com_upload_s.send_data, 20);
    }
}

/************************************************************************
** Function name:        BLE_SendMovementData
** Descriptions:         �����˶�ģʽ����
** input parameter:      ��    
** Returned Value:       void
** Created  by:          GT
** Created  Datas:       2017-08-04
**************************************************************************/
static void BLE_SendMovementData(void)
{
    uint8_t *movement_head_p = NULL;
    uint16_t c_pack_num = 0;
    movement_patterns_head_t movement_head_temp_s = {0x00, };
    movement_patterns_data_t movement_data_temp_s = {0x00, };         //��ǰһ���Ӽ�¼��Ϣ         
    
    if(com_upload_s.movement.total > 300)
    {
        com_upload_s.movement.total = 0;
    }
    
    if(com_upload_s.movement.day_id >= MOVEMENT_PATTERNS_MAX_BLOCK
      || com_upload_s.movement.start >= com_upload_s.movement.total)
    {        
        com_upload_s.send_data[0] = 0xD4;
        com_upload_s.send_data[5] = com_upload_s.movement.day_id;
        Ble_BatTimeOutAutoRepeat(com_upload_s.send_data, 20);  
        return ; 
    }
    com_upload_s.movement.split = 0;
    while(1)
    {
        if(com_upload_s.movement.start < MOVEMENT_HEAD_MAX_PACK_NUM)
        {   
            //ͷ����Ϣ
            Movement_FlashReadHead(com_upload_s.movement.day_id, (void *)&movement_head_temp_s, sizeof(movement_patterns_head_t));       
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

            Ble_BatTimeOutAutoRepeat(com_upload_s.send_data, 20);         
        }
        else
        {
            //һ������Ϣ
            c_pack_num = com_upload_s.movement.start - MOVEMENT_HEAD_MAX_PACK_NUM;
            Movement_FlashReadData(com_upload_s.movement.day_id, c_pack_num, (uint8_t *)&movement_data_temp_s, 20);        
            com_upload_s.movement.start++;
            com_upload_s.send_data[1] = com_upload_s.movement.start & 0xFF;
            com_upload_s.send_data[2] = (com_upload_s.movement.start >> 8) & 0xFF;
            com_upload_s.send_data[3] = com_upload_s.movement.total & 0xFF;
            com_upload_s.send_data[4] = (com_upload_s.movement.total >> 8) & 0xFF;
            com_upload_s.send_data[5] = com_upload_s.movement.day_id;
            memcpy(&com_upload_s.send_data[6], &movement_data_temp_s.heart, 14);

            Ble_BatTimeOutAutoRepeat(com_upload_s.send_data, 20);           
        }
        if(com_upload_s.movement.start >= com_upload_s.movement.total)
        {
            return ;
        }

        // ���ӶϿ����ڴ���
        if(BleThread_GetBleNus()->conn_handle == BLE_CONN_HANDLE_INVALID)
        {
            return;
        } 
        
        if(++com_upload_s.movement.split >= UPLOAD_RESPONSE_SPLIT_CNT)
        {
            if(com_upload_s.movement.start < com_upload_s.movement.total)
            {
                // �����ź���, ���ֶϵ���������
                xSemaphoreGive(com_res_s.m_movement_sem); 
                xSemaphoreGive(com_res_s.m_task_count);
            }
            return ;
        }
    }
}


void BLE_SendMovementDataByType(void)
{  
    uint8_t ble_ack[20] = {0x00, };
    uint8_t ret = 0;
     
    if(com_upload_s.movement.data_type == 0)
    {   // ��ȡ���ݻظ�
        ret = Movement_SetSendDataInfo(com_upload_s.movement.day_id);        
        if(ret == 1)
        {
            //�ϴ��˶�ģʽ����
            com_upload_s.movement.data_type = 2;
            com_upload_s.movement.start = movement_send_s.movement_num_start;
            com_upload_s.movement.total = movement_send_s.movement_num_total;
            com_upload_s.movement.day_id = movement_send_s.movement_block;
        }
        else
        {       
            ble_ack[0] = 0xD4; 
            ble_ack[5] = com_upload_s.movement.day_id;    
            Ble_SendDataViaBat(ble_ack, 20);     
            return;
        }      
    }
    else if(com_upload_s.movement.data_type == 1)
    {   // ��ȡCRC�ظ�
        ble_ack[0] = 0xD3; 
        ble_ack[1] = 0x01;     
        ble_ack[2] = 0x01; 

        Movement_GetHeadCRC(&ble_ack[3], 3);
    
        Ble_SendDataViaBat(ble_ack, 20);   
        return;        
    }
    
    if(com_upload_s.movement.data_type == 2)
    {
        BLE_SendMovementData();
    }        
}

/************************************************************************
** Function name:        BLE_SendAlarmData
** Descriptions:         App�����ֻ�, �ֻ����ϴ�20����������
** input parameter:      ��     
** Returned Value:       void
** Created  by:          GT
** Created  Datas:       2017-08-04
**************************************************************************/
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
        //WJ.20190121.�޸ģ����ӷ��ʹ�������
        //for(uint8_t index = 0; index < total_package; index++)
        for(uint8_t index = 0; index < 20; index++)
        {
            
            if(p_flash_param->alarm_time_s[index].valid != 1)
            {
                continue;
            }
            
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

            // ���ӶϿ����ڴ���
            if(BleThread_GetBleNus()->conn_handle == BLE_CONN_HANDLE_INVALID)
            {
                return;
            }             
        }
    }
    else
    {
        //���﷢��һ�������
        Ble_BatTimeOutAutoRepeat(send_data, 20);
    }
}

/************************************************************************
** Function name:        BLE_SendADCData
** Descriptions:         App�����ֻ�, �ֻ����ϴ�ADC����ԭʼ����
** input parameter:      ��     
** Returned Value:       void
** Created  by:          GT
** Created  Datas:       2017-08-04
**************************************************************************/
static void BLE_SendADCData(void)
{    
    if(p_data_manage->upload == UPLOAD_ADC_GSENSOR_EVENT)
    {   //acc
		ComThread_SetSendFlashDataState(1);
        while(p_data_manage->acc.adc_send_len >= 3)
        {
            memset(com_upload_s.send_data, 0, sizeof(com_upload_s.send_data));
            DataManage_GetGsensorADCData((int16_t*)com_upload_s.send_data);
            Ble_ClkTimeOutAutoRepeat(com_upload_s.send_data, 20);
        }
    }
    else if(p_data_manage->upload == UPLOAD_ADC_GREEN_EVENT 
            && (p_data_manage->ecg_ppg_s.ppg_measure_s.app == 1 
                || p_data_manage->ecg_ppg_s.ppg_measure_s.app_bp == 1 
                || p_data_manage->ppg_adc_mode == 0x01))
    {   //ppg
		ComThread_SetSendFlashDataState(1);

		while(p_data_manage->data_pool.ppg_send_wait_len >= 5)
		{
			DataManage_GetPpgAdcDataGreenIredAmb(&com_upload_s.send_data[1], &com_upload_s.send_data[21], &com_upload_s.send_data[41]);
			com_upload_s.send_data[0] = 0x82;   //�̹�
			Ble_ClkTimeOutAutoRepeat(&com_upload_s.send_data[0], 20);
			com_upload_s.send_data[20] = 0x84;  //�����
			Ble_ClkTimeOutAutoRepeat(&com_upload_s.send_data[20], 20);	
			//com_upload_s.send_data[40] = 0x85;  //������
			//Ble_ClkTimeOutAutoRepeat(&com_upload_s.send_data[40], 20);				      
        }
    }
	else if(p_data_manage->upload == UPLOAD_ADC_ECG_EVENT 
            && p_data_manage->ecg_ppg_s.ecg_measure_s.app == 1)
	{   //ecg
		ComThread_SetSendFlashDataState(1);
		while(p_data_manage->data_pool.ecg_send_wait_len >= 5)
		{
			DataManage_GetEcgAdcData(&com_upload_s.send_data[1], 1);
			com_upload_s.send_data[0] = 0x88;   //ECG
			Ble_ClkTimeOutAutoRepeat(com_upload_s.send_data, 20); 
		}
#if 0		
		{	// JSH.20190114.test
		// ��������
		DataManage_GetEcgLdData1(&com_upload_s.send_data[1]);
		com_upload_s.send_data[0] = 0x85;   //��������
		Ble_ClkTimeOutAutoRepeat(com_upload_s.send_data, 20); 
		
		DataManage_GetEcgLdData2(&com_upload_s.send_data[1]);
		com_upload_s.send_data[0] = 0x85;   //��������
		Ble_ClkTimeOutAutoRepeat(com_upload_s.send_data, 20);
		}
#endif        
	}
	else if(p_data_manage->upload == UPLOAD_ADC_OXYGEN_EVENT
           && p_data_manage->ecg_ppg_s.oxygen_measure_s.ox_app == 1)
	{   // Ѫ����·
		ComThread_SetSendFlashDataState(1);
		while(p_data_manage->data_pool.ppg_send_wait_len >= 5)
		{
			DataManage_GetPpgAdcDataGreenRedIredAmb(&com_upload_s.send_data[1], &com_upload_s.send_data[21],
													&com_upload_s.send_data[41], &com_upload_s.send_data[61]);
			com_upload_s.send_data[0] = 0x82;   //�̹�
			Ble_ClkTimeOutAutoRepeat(&com_upload_s.send_data[0], 20);
			com_upload_s.send_data[20] = 0x83;  //���
			Ble_ClkTimeOutAutoRepeat(&com_upload_s.send_data[20], 20);
			com_upload_s.send_data[40] = 0x84;  //�����
			Ble_ClkTimeOutAutoRepeat(&com_upload_s.send_data[40], 20);
			//com_upload_s.send_data[60] = 0x85;  //������
			//Ble_ClkTimeOutAutoRepeat(&com_upload_s.send_data[60], 20);			
		}
	}
    else if((p_data_manage->upload == UPLOAD_ADC_ECG_ORIGINAL_EVENT) 
            && (p_data_manage->ecg_ppg_s.ecg_measure_s.app == 1))
    {   //ecg
		ComThread_SetSendFlashDataState(1);

        while(p_data_manage->data_pool.ecg_send_wait_len >= 5)        
        {   //����ԭʼδ�����ź�
            DataManage_GetEcgAdcData(&com_upload_s.send_data[1], 0);     
            									
            com_upload_s.send_data[0] = 0x88;
            Ble_ClkTimeOutAutoRepeat(com_upload_s.send_data, 20);           
        }

		{	// JSH.20190114.test
		// ��������
		DataManage_GetEcgLdData1(&com_upload_s.send_data[1]);
		com_upload_s.send_data[0] = 0x85;   //��������
		Ble_ClkTimeOutAutoRepeat(com_upload_s.send_data, 20); 
		
		DataManage_GetEcgLdData2(&com_upload_s.send_data[1]);
		com_upload_s.send_data[0] = 0x85;   //��������
		Ble_ClkTimeOutAutoRepeat(com_upload_s.send_data, 20);
		}        
    }
}

/************************************************************************
** Function name:        Ble_SendAllFlashData
** Descriptions:         App�����ֻ�, �ֻ����ϴ�����FLASHԭʼ����
** input parameter:      ��     
** Returned Value:       void
** Created  by:          GT
** Created  Datas:       2017-08-04
**************************************************************************/
static void Ble_SendAllFlashData(void)
{
    uint32_t    flash_data[5] = {0};
    uint8_t     *p_flash_data = (uint8_t*)flash_data;
    uint8_t     block_id_tmp = 0;
    
    ComThread_SetSendFlashDataState(1);
    
    // �����ȡFLASH �ճ�����/˯������/Ѫ������
//    for(uint8_t block_id = 0; block_id < RECORD_EXT_FLASH_DATA_SIZE/1024; block_id++)//132//96;
//    {
//        for(uint8_t offset = 0; offset < 64; offset++)
//        {
//            memset(flash_data, 0, sizeof(flash_data));
//            
//            Api_ReadAllFlashData(block_id, offset, &p_flash_data[1]);
//            
//            //������ת��ΪЭ������
//            p_flash_data[0] = 0xF0;
//            p_flash_data[17] = offset;
//            p_flash_data[18] = block_id;
//            Ble_BatTimeOutAutoRepeat(p_flash_data, 20);
//        }
//        
//        block_id_tmp = block_id;
//        // ���ӶϿ����ڴ���
//        if(BleThread_GetBleNus()->conn_handle == BLE_CONN_HANDLE_INVALID)
//        {
//            return;
//        }         
//    }
    
    // �����ȡFLASH ˯������/Ѫ������
    for(uint16_t block_id = 0; block_id < RECORD_EXT_FLASH_DATA_SIZE/1024; block_id++)
    {
        for(uint8_t offset = 0; offset < 64; offset++)
        {
            memset(flash_data, 0, sizeof(flash_data));
            
            Api_ReadAllExtFlashData(block_id, offset, &p_flash_data[1]);
            
            //������ת��ΪЭ������
            p_flash_data[0] = 0xF0;
            p_flash_data[17] = offset;
            p_flash_data[18] = block_id+block_id_tmp;
            Ble_BatTimeOutAutoRepeat(p_flash_data, 20);
        }

        // ���ӶϿ����ڴ���
        if(BleThread_GetBleNus()->conn_handle == BLE_CONN_HANDLE_INVALID)
        {
            return;
        }         
    }
}

/************************************************************************
** Function name:        Ble_SendWechatData
** Descriptions:         App�����ֻ�, �ֻ���ʵʱ�ϴ�΢�żǲ�����
** input parameter:      ��     
** Returned Value:       void
** Created  by:          GT
** Created  Datas:       2017-08-04
**************************************************************************/
//extern uint8_t alg_run_cnt;
//extern uint8_t alg_stop_run_cnt;
//extern uint8_t  reste_flag;
//extern uint32_t reset_value;               //��λ����
//extern uint32_t reset_time_minutes;        //��λʱ�����
static void Ble_SendWechatData(void)
{
//    // test
//    if(alg_stop_run_cnt > 6)
//    {
//        uint8_t test_ble_ack[20] = {0x11, 0x11, 0xFF, 0x00};
//        test_ble_ack[3] = alg_run_cnt;
//        Ble_SendDataViaBat(test_ble_ack, 20);
//        
//        reste_flag = 0;
//        reset_value = alg_run_cnt;
//        reset_time_minutes = 0xFF;
//    }
    
    ble_wechat_send_data(DataManage_GetCurrentStepCnt());
}

/************************************************************************
** Function name:        Ble_SendHourglassData
** Descriptions:         App�����ֻ�, �ֻ���ʵʱ�ϴ�����ʱ����
** input parameter:      ��     
** Returned Value:       void
** Created  by:          GT
** Created  Datas:       2017-08-04
**************************************************************************/
static void Ble_SendHourglassData(void)
{
    memset(com_upload_s.send_data, 0, sizeof(com_upload_s.send_data));
    
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
    
    Ble_SendDataViaBat(com_upload_s.send_data, 20);
}

/****************************************************************
** Function name:           Ble_SemdEcgManualId
** Descriptions:            
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2018-11-07
*****************************************************************/ 
// �����ֶ�����id
void Ble_SemdEcgManualId(void)
{
    uint8_t     ble_send_buf[24] = {0};    
    uint8_t     time_data[7] = {0};    
    uint16_t    ecg_start_index = 0;    
    uint16_t    ecg_inxt_cnt = 0;
    uint16_t    content_len = 0;
    uint8_t     cur_pack_cnt = 0;
    uint8_t     send_data_index = 0;
    ecg_result_data_t    read_ecg_result_s = {0};
    ecg_data_index_t    *p_ecg_data_index = NULL;
    
    p_ecg_data_index = &p_flash_param->flash_index_s.ecg;
    
    ble_send_buf[0] = 0x96;
    ble_send_buf[1] = 0x02;
    cur_pack_cnt = 1;
    ble_send_buf[3] = cur_pack_cnt;
    
    memcpy(time_data, &com_upload_s.ecg.year, 7);

    if(p_ecg_data_index->offset_end_index > p_ecg_data_index->offset_start_index)
    {
        ecg_inxt_cnt = p_ecg_data_index->offset_end_index - p_ecg_data_index->offset_start_index;
    }
    else if(p_ecg_data_index->offset_end_index == p_ecg_data_index->offset_start_index)
    {
        ecg_inxt_cnt = 0;
        ble_send_buf[2] = 0x04;
        Ble_BatTimeOutAutoRepeat(ble_send_buf, 20);
        return;
    }
    else
    {
        ecg_inxt_cnt = RECORD_ECG_MAX_CNT - p_ecg_data_index->offset_start_index + 
                       p_ecg_data_index->offset_end_index;
    }    
         
    if(time_data[0] == 0 && time_data[1] == 0 )
    {
        ecg_start_index = 0;
    }
    else
    {   //�ҵ���һ������������
        for(uint16_t i = 0; i < ecg_inxt_cnt; i++)
        {
            ecg_start_index = (p_ecg_data_index->offset_start_index + i) % RECORD_ECG_MAX_CNT;

            Api_GetEcgAttrData(ecg_start_index, &read_ecg_result_s);
            if(SystemTime_Check1((void *)&read_ecg_result_s.year, 7) == NRF_SUCCESS 
              && SystemTime_Compare(time_data, (void *)&read_ecg_result_s.year) == 1)
            {
                ecg_start_index = i;
                break;
            }
          
            if(i == ecg_inxt_cnt - 1)
            {
                ecg_inxt_cnt = 0;
                ble_send_buf[2] = 0x04;
                Ble_BatTimeOutAutoRepeat(ble_send_buf, 20);
                return;
            }
        }
    }      
    
    content_len = (ecg_inxt_cnt - ecg_start_index) * 2 + 1;
    //send_pack_cnt = content_len / 16 + ((content_len%16 == 0) ? 0:1);
    
    ble_send_buf[2] = 0x01;     //��ʼ��
    send_data_index = 4;
    ble_send_buf[send_data_index++] = 0xA2; 
    ble_send_buf[send_data_index++] = content_len & 0xFF; 
    ble_send_buf[send_data_index++] = (content_len>>8) & 0xFF; 
    ble_send_buf[send_data_index++] = 2; 
    
    // Ĭ��id��������
    for(uint16_t i = ecg_start_index; i < ecg_inxt_cnt; i++)
    {
        ecg_start_index = (p_ecg_data_index->offset_start_index + i) % RECORD_ECG_MAX_CNT;
        ble_send_buf[send_data_index++] = (ecg_start_index) & 0xFF;
        ble_send_buf[send_data_index++] = (ecg_start_index)>>8 & 0xFF;
        if(send_data_index >= 20)
        {
            Ble_BatTimeOutAutoRepeat(ble_send_buf, 20);
            memcpy(&ble_send_buf[4], &ble_send_buf[20], send_data_index - 20);
            send_data_index = 4 + (send_data_index - 20);
            cur_pack_cnt++;
            ble_send_buf[2] = 0x02;         //�м��
            ble_send_buf[3] = cur_pack_cnt;
        }
        
        // ���ӶϿ����ڴ���
        if(BleThread_GetBleNus()->conn_handle == BLE_CONN_HANDLE_INVALID)
        {
            return;
        } 
    }
    
    if(send_data_index != 4)
    {
        memset(&ble_send_buf[send_data_index], 0, 20 - send_data_index);
        Ble_BatTimeOutAutoRepeat(ble_send_buf, 20);
    }
    
    ble_send_buf[2] = 0x03;     //������
    memset(&ble_send_buf[3], 0, 17);
    Ble_BatTimeOutAutoRepeat(ble_send_buf, 20);
}

// �����Զ�����id
void Ble_SemdEcgAutomaticId(void)
{
    uint8_t     ble_send_buf[20] = {0};  

#if USED_EXT_FLASH_ECG_AUTO_ENABLED     
    uint8_t     time_data[7] = {0};    
    uint16_t    ecg_start_index = 0;    
    uint16_t    ecg_temp_index = 0;  
    uint16_t    ecg_inxt_cnt = 0;
    uint16_t    content_len = 0;
    uint8_t     cur_pack_cnt = 0;
    uint8_t     send_data_index = 0;
    uint8_t     read_data_buf[50] = {0};
    ecg_ext_data_index_t    *p_ecg_ext_data_index = NULL;   
    uint16_t    read_time_out = 0;
    
    read_time_out = 0;
    
    p_ecg_ext_data_index = &p_flash_param->flash_index_s.ecg_ext;
    
    ble_send_buf[0] = 0x96;
    ble_send_buf[1] = 0x03;
    cur_pack_cnt = 1;
    ble_send_buf[3] = cur_pack_cnt;
    
    memcpy(time_data, &com_upload_s.ecg.year, 7);
 
    if(p_ecg_ext_data_index->ecg_cnt == 0)   
    {
        ecg_inxt_cnt = 0;
        ble_send_buf[2] = 0x04;
        Ble_BatTimeOutAutoRepeat(ble_send_buf, 20);
        return;
    }  
    else
    {
         ecg_inxt_cnt = p_ecg_ext_data_index->ecg_cnt;
    }    
     
    if(ecg_inxt_cnt >= ECG_RECORD_MAX_CNT - 2)
    {
        ecg_start_index = (p_ecg_ext_data_index->offset_ext_index + 2 - p_ecg_ext_data_index->offset_ext_index%2) % ECG_RECORD_MAX_CNT;
    }
    else
    {
        ecg_start_index = 0;
    }
    
    if(time_data[0] == 0 && time_data[1] == 0 )
    {
    }
    else
    {   //�ҵ���һ������������
        for(uint16_t i = 0; i < ecg_inxt_cnt; i++)
        {
            ecg_temp_index = (ecg_start_index + i) % ECG_RECORD_MAX_CNT;
    
            read_time_out = 0;
            while(Extflash_EcgReadSet(ecg_temp_index, 0, 10, read_data_buf) == 0)
            {
                vTaskDelay(1);
                read_time_out++;
                if(read_time_out > 1200)
                {   //��ʱ����������
                    return;
                }  
            }
            while(Extflash_EcgReadIsComplete() == 0)
            {
                vTaskDelay(1);
                read_time_out++;
                if(read_time_out > 1200)
                {   //��ʱ����������
                    return;
                }
            }
            if(SystemTime_Check1(&read_data_buf[3], 7) == NRF_SUCCESS 
              && SystemTime_Compare(time_data, &read_data_buf[3]) == 1)
            {
                ecg_start_index = ecg_start_index + i;
                ecg_inxt_cnt = ecg_inxt_cnt - i;
                break;
            }
        
            if(i == ecg_inxt_cnt - 1)
            {
                ecg_inxt_cnt = 0;
                ble_send_buf[2] = 0x04;
                Ble_BatTimeOutAutoRepeat(ble_send_buf, 20);
                return;
            }
        }        
    }      
    
    content_len = (ecg_inxt_cnt) * 2 + 1;
    
    ble_send_buf[2] = 0x01;     //��ʼ��
    send_data_index = 4;
    ble_send_buf[send_data_index++] = 0xA3; 
    ble_send_buf[send_data_index++] = content_len & 0xFF; 
    ble_send_buf[send_data_index++] = (content_len>>8) & 0xFF; 
    ble_send_buf[send_data_index++] = 2; 
    
    // Ĭ��id��������
    for(uint16_t i = 0; i < ecg_inxt_cnt; i++)
    {
        uint16_t index = (ecg_start_index + i) % ECG_RECORD_MAX_CNT;
        ble_send_buf[send_data_index++] = (index) & 0xFF;
        ble_send_buf[send_data_index++] = (index)>>8 & 0xFF;
        if(send_data_index >= 20)
        {
            Ble_BatTimeOutAutoRepeat(ble_send_buf, 20);
            memcpy(&ble_send_buf[4], &ble_send_buf[20], send_data_index - 20);
            send_data_index = 4 + (send_data_index - 20);
            cur_pack_cnt++;
            ble_send_buf[2] = 0x02;         //�м��
            ble_send_buf[3] = cur_pack_cnt;
        }
    }
    
    if(send_data_index != 4)
    {
        memset(&ble_send_buf[send_data_index], 0, 20 - send_data_index);
        Ble_BatTimeOutAutoRepeat(ble_send_buf, 20);
    }
    
    ble_send_buf[2] = 0x03;     //������
    memset(&ble_send_buf[3], 0, 17);
    Ble_BatTimeOutAutoRepeat(ble_send_buf, 20);    
#else
    ble_send_buf[0] = 0x96;
    ble_send_buf[1] = 0x03;
    ble_send_buf[2] = 0x04;
    ble_send_buf[3] = 0;
    Ble_BatTimeOutAutoRepeat(ble_send_buf, 20);  
#endif    
}


// �����ֶ���������ͨ��id
void Ble_SendEcgManualDataById(uint16_t id)
{
    uint8_t     ble_send_buf[20] = {0};   
    uint8_t     content_buf[45] = {0};   
    //uint8_t     time_data[7] = {0};    
    uint16_t    ecg_start_index = 0;    
    uint16_t    content_len = 0;
    uint16_t    cur_pack_cnt = 0;
    uint16_t    send_content_index = 0;
    
    ecg_result_data_t    read_ecg_result_s = {0};
    
    //memcpy(time_data, &com_upload_s.ecg.year, 7);
    
    ecg_start_index = id  % RECORD_ECG_MAX_CNT;   
    Api_GetEcgAttrData(ecg_start_index, &read_ecg_result_s);
    
    if(SystemTime_Check1((void *)&read_ecg_result_s.year, 7) == NRF_SUCCESS 
      /*&& SystemTime_Compare(time_data, (void *)&read_ecg_result_s.year) == 1*/)
    {
        content_buf[0] = 0xB1;
        content_buf[1] = 0x08;
        content_len = 2;
        memcpy(&content_buf[2], &read_ecg_result_s.year, 7);
        content_len += 7; 
        content_buf[content_len++] = 30;    //30��
        
        content_buf[content_len++] = 0xB3;
        content_buf[content_len++] = 14;
        memcpy(&content_buf[content_len], &read_ecg_result_s.ecg_result_data, 14);        
        content_len += 14; 
        
        content_buf[content_len++] = 0xB4;
        content_buf[content_len++] = 4;
        content_buf[content_len++] = 250 & 0xFF;
        content_buf[content_len++] = (250>>8) & 0xFF;
        content_buf[content_len++] = 250 & 0xFF;
        content_buf[content_len++] = (250>>8) & 0xFF;
    }

    ble_send_buf[0] = 0x97;
    ble_send_buf[1] = 0x02;
    ble_send_buf[3] = (++cur_pack_cnt) & 0xFF;
    ble_send_buf[4] = (cur_pack_cnt>>8) & 0xFF;
       
    if(content_len == 0)
    {
        ble_send_buf[2] = 0x03;
    }
    else
    {
        ble_send_buf[2] = 0x00;
        
        for(; send_content_index + 15 <= content_len; )
        {
            memcpy(&ble_send_buf[5], &content_buf[send_content_index], 15);
            Ble_BatTimeOutAutoRepeat(ble_send_buf, 20);

            send_content_index += 15;
            
            ble_send_buf[2] = 0x01;
            ble_send_buf[3] = (++cur_pack_cnt) & 0xFF;
            ble_send_buf[4] = (cur_pack_cnt>>8) & 0xFF;      
        }  
         
        if(send_content_index < content_len)
        {
            ble_send_buf[2] = 0x01;
            memcpy(&ble_send_buf[5], &content_buf[send_content_index], content_len - send_content_index);
            memset(&ble_send_buf[5 + (content_len - send_content_index)], 0, 15 - (content_len - send_content_index));
            Ble_BatTimeOutAutoRepeat(ble_send_buf, 20);
        }    
        
        memset(&ble_send_buf[3], 0, 17);
        ble_send_buf[2] = 0x02;    
    }
    
    Ble_BatTimeOutAutoRepeat(ble_send_buf, 20);
}


// �����Զ���������ͨ��id
void Ble_SemdEcgAutomaticDataById(uint16_t id)
{
    uint8_t     ble_send_buf[24] = {0}; 

#if USED_EXT_FLASH_ECG_AUTO_ENABLED     
    uint8_t     time_data[7] = {0};    
    uint16_t    ecg_start_index = 0;    
    uint16_t    cur_pack_cnt = 0;
    uint16_t    read_time_out = 0;
    
    uint32_t    read_addr = 0;
    uint8_t     content_buf[50] = {0};   

    
    ecg_start_index = 0;    
    cur_pack_cnt = 0;
    read_time_out = 0;
    read_addr = 0;    
     
    memcpy(time_data, &clock->year, 7); //��ȡϵͳʱ��

    ecg_start_index = (id) % ECG_RECORD_MAX_CNT;

    read_time_out = 0;
    while(Extflash_EcgReadSet(ecg_start_index, 0, 16, content_buf) == 0)            
    {
        vTaskDelay(1);
        read_time_out++;
        if(read_time_out > 1200)
        {   //��ʱ����������
            return;
        }  
    }
    while(Extflash_EcgReadIsComplete() == 0)
    {
        vTaskDelay(1);
        read_time_out++;
        if(read_time_out > 1200)
        {   //��ʱ����������
            return;
        }
    }
    
    ble_send_buf[0] = 0x97;
    ble_send_buf[1] = 0x03;
    ble_send_buf[3] = (++cur_pack_cnt) & 0xFF;
    ble_send_buf[4] = (cur_pack_cnt>>8) & 0xFF;
    
    if(SystemTime_Check1(&content_buf[3], 7) != NRF_SUCCESS 
      || SystemTime_Compare(&content_buf[3], time_data) != 1)   // ��ϵͳʱ��Ա�
    {
        //���ݲ�ͨ��
        ble_send_buf[2] = 0x03;
        Ble_BatTimeOutAutoRepeat(ble_send_buf, 20);  
        return;
    }
    else
    {
        ble_send_buf[2] = 0x00;
        memcpy(&ble_send_buf[5], &content_buf[1], 15);
        Ble_BatTimeOutAutoRepeat(ble_send_buf, 20);       
    }
    // ѭ����ȡ
    read_addr = 16;
    
    for(; read_addr < ECG_EFFECTIVE_DATA_LEN+1/*6044*/; )    
    {
        memset(&ble_send_buf[5], 0, 15);
        
        read_time_out = 0;
        while(Extflash_EcgReadSet(ecg_start_index, read_addr, 45, content_buf) == 0)
        {
            vTaskDelay(1);
            read_time_out++;
            if(read_time_out > 1200)
            {   //��ʱ����������
                return;
            }
        }
        while(Extflash_EcgReadIsComplete() == 0)
        {
            read_time_out = 0;
            vTaskDelay(1);
            read_time_out++;
            if(read_time_out > 1200)
            {   //��ʱ����������
                return;
            }
        }
        
        for(uint8_t j = 0; j < 3; j++)
        {
            ble_send_buf[2] = 0x01;
            ble_send_buf[3] = (++cur_pack_cnt) & 0xFF;
            ble_send_buf[4] = (cur_pack_cnt>>8) & 0xFF;
            memcpy(&ble_send_buf[5], &content_buf[j*15], 15);
            Ble_BatTimeOutAutoRepeat(ble_send_buf, 20); 
            
            read_addr += 15;
            if(read_addr > ECG_EFFECTIVE_DATA_LEN)
            {
                break;
            }
        }
        
        if(BleThread_GetBleNus()->conn_handle == BLE_CONN_HANDLE_INVALID)
        {
            return;
        }
    }

    ble_send_buf[2] = 0x02;   
    memset(&ble_send_buf[3], 0, 17);
    Ble_BatTimeOutAutoRepeat(ble_send_buf, 20);
    
#else
    ble_send_buf[0] = 0x97;
    ble_send_buf[1] = 0x03;
    ble_send_buf[2] = 0x03;
    ble_send_buf[3] = 0;
    ble_send_buf[4] = 0;
    Ble_BatTimeOutAutoRepeat(ble_send_buf, 20);  
#endif    
}

////uint32_t ecg_flash_time[10] = {0};
////uint32_t ecg_flash_time1[10] = {0};
////uint8_t  ecg_flash_cnt = 0;

static void Ble_SendEcgResultData(void)
{     
//////    ecg_flash_time[ecg_flash_cnt] = (uint32_t)(xTaskGetTickCount());  
    
    
    ComThread_SetSendFlashDataState(1);
    
    // ����ecg ����id  
    if(com_upload_s.ecg.id_flag == 1)
    {   //�Զ����ֶ����ݣ�Ŀǰ��֧��        
         com_upload_s.ecg.id_flag = 0;
    }
    else if(com_upload_s.ecg.id_flag == 2)
    {
         Ble_SemdEcgManualId();
         com_upload_s.ecg.id_flag = 0;
    }
    else if(com_upload_s.ecg.id_flag == 3)
    { 
         Ble_SemdEcgAutomaticId();        
         com_upload_s.ecg.id_flag = 0;
    }
    
    if(com_upload_s.ecg.id_type == 0x02)
    {   // �ֶ�
        Ble_SendEcgManualDataById(com_upload_s.ecg.send_id);
        com_upload_s.ecg.id_type = 0;
    }
    else if(com_upload_s.ecg.id_type == 0x03)
    {   //�Զ�
        Ble_SemdEcgAutomaticDataById(com_upload_s.ecg.send_id);
        com_upload_s.ecg.id_type = 0;
    }
    
//////    ecg_flash_time1[ecg_flash_cnt++] = (uint32_t)(xTaskGetTickCount()) - ecg_flash_time[ecg_flash_cnt];  
//////    ecg_flash_cnt = ecg_flash_cnt%10;
}

static void Ble_SendPowerInfoData(void)
{
    uint8_t             ble_ack[20] = {0x00,};
    power_statistics_t  p_power_temp = {0};
    uint8_t             local_day_id = 0;
    uint8_t             day_id = 0;
    uint8_t             day_n = 1;
    
    ble_ack[0] = 0xEF;
    ble_ack[1] = 0x01;
    ble_ack[2] = 0x01;
     
    if(com_upload_s.power_info_day == 0xFE)
    {
        day_id = 0;
        day_n = 4;
    }
    else
    {
        day_id = com_upload_s.power_info_day;
        day_n = com_upload_s.power_info_day + 1;  
    }
    
    for(; day_id < day_n; day_id++)
    {
        ble_ack[3] = day_id;
        memset(&ble_ack[4], 0, 16);
        if(day_id == 0x00)
        {   //����
            memcpy(&ble_ack[4], &p_data_manage->power_statistics_s, 8);
            ble_ack[12] = p_data_manage->power_statistics_s.ble_connection_time & 0xFF;            
            ble_ack[13] = (p_data_manage->power_statistics_s.ble_connection_time>>8) & 0xFF;            
            ble_ack[14] = (p_data_manage->power_statistics_s.ble_connection_time>>16) & 0xFF;        
            
            ble_ack[15] = p_data_manage->power_statistics_s.charging;        
            ble_ack[16] = p_data_manage->power_statistics_s.rest_cnt;
        }
        else if(day_id <= 0x02)
        {
            local_day_id = (3 + p_flash_param->flash_index_s.block_index - day_id)%3; 
            
            //Record_GetPowerInfoData(local_day_id, &p_power_temp);
            
            uint32_t read_time_out = 0;
            while(Extflash_DailyReadSet(local_day_id, RECORD_DAILY_DATA_MAX_CNT, 
                                       (uint8_t *)&p_power_temp, RECORD_POWER_INFO_SIZE, 1) == 0)
            {
                vTaskDelay(1);
                read_time_out++;
                if(read_time_out > 1200)
                {   //��ʱ����������
                    return;
                }  
            }
            while(Extflash_DailyReadIsComplete() == 0)
            {
                vTaskDelay(1);
                read_time_out++;
                if(read_time_out > 1200)
                {   //��ʱ����������
                    return;
                }
            }
            
            if(p_power_temp.bright_screen_num == 0xFFFF || p_power_temp.bright_screen_time == 0xFFFF)
            {   //���ݲ�����
                ble_ack[2] = 3;
            }
            else
            {
                memcpy(&ble_ack[4], &p_power_temp,  8);
                ble_ack[12] = p_power_temp.ble_connection_time & 0xFF;            
                ble_ack[13] = (p_power_temp.ble_connection_time>>8) & 0xFF;            
                ble_ack[14] = (p_power_temp.ble_connection_time>>16) & 0xFF;        
                
                ble_ack[15] = p_power_temp.charging;        
                ble_ack[16] = p_power_temp.rest_cnt;
            }
        }
        else if(day_id == 3 && com_upload_s.power_info_day == 0xFE)
        {
            ble_ack[3] = 0xFE;
        }
        else
        {   //���ݲ�����
            ble_ack[2] = 3;
        }
        Ble_SendDataViaBat(ble_ack, 20);  
    }    
}

/************************************************************************
** Function name:        CommunicationThread_Handle
** Descriptions:         ͨ���߳������� 
** input parameter:      arg--��Ч����  
** Returned Value:       ��
** Created  by:          GT
** Created  Datas:       2017-08-04
**************************************************************************/
static void CommunicationThread_Handle(void* arg)
{    
    UNUSED_PARAMETER(arg);
        
    while(1)
    {  	    
        // ע��:���ﴦ����Ҫ���ȼ�, ����APP���ղ�������
        THREAD_MUTEX_TASK_EXCUTE(com_res_s.m_upload_adc_sem, com_res_s.m_task_count, BLE_SendADCData);
        
        // �߳�ִ�з���΢����������
        THREAD_MUTEX_TASK_EXCUTE(com_res_s.m_wechat_data_sem, com_res_s.m_task_count, Ble_SendWechatData);
        
        // �߳�ִ�з��͵���ʱ��������
        THREAD_MUTEX_TASK_EXCUTE(com_res_s.m_hourglass_app, com_res_s.m_task_count, Ble_SendHourglassData);

        // ����������������
        THREAD_MUTEX_TASK_EXCUTE(com_res_s.m_alarm_data_sem, com_res_s.m_task_count, BLE_SendAlarmData);
        
        // ��������ʵʱ��������
        THREAD_MUTEX_TASK_EXCUTE(com_res_s.m_heart_app, com_res_s.m_task_count, BLE_SendHeartAppData);
       
        // ����Ѫѹʵʱ��������
        THREAD_MUTEX_TASK_EXCUTE(com_res_s.m_bp_app, com_res_s.m_task_count, BLE_SendBpAppData);
        
        // ����ecgʵʱ��������
        THREAD_MUTEX_TASK_EXCUTE(com_res_s.m_ecg_app, com_res_s.m_task_count, BLE_SendEcgAppData);
        
        // ����Ѫ��ʵʱ��������
        THREAD_MUTEX_TASK_EXCUTE(com_res_s.m_oxygen_app, com_res_s.m_task_count, BLE_SendOxygenAppData);
        
        // ���ͼǲ�ʵʱ����
        THREAD_MUTEX_TASK_EXCUTE(com_res_s.m_pedometer_data_sem, com_res_s.m_task_count, BLE_SendPedometerData);
        
        // �߳�ִ�з����ճ���������
        THREAD_MUTEX_TASK_EXCUTE(com_res_s.m_daily_data_sem, com_res_s.m_task_count, BLE_SendDailyAttrRecordData);
        
        // �����˶�ģʽ��������
        THREAD_MUTEX_TASK_EXCUTE(com_res_s.m_movement_sem, com_res_s.m_task_count, BLE_SendMovementDataByType);
        
        // ����FLASH˯��ȫ����������
        THREAD_MUTEX_TASK_EXCUTE(com_res_s.m_sleep_data_sem, com_res_s.m_task_count, BLE_SendSleepData);
  
        // ����FLASHԭʼ����
        THREAD_MUTEX_TASK_EXCUTE(com_res_s.m_all_flash_sem, com_res_s.m_task_count, Ble_SendAllFlashData);
        
        // ����ECG�������
        THREAD_MUTEX_TASK_EXCUTE(com_res_s.m_ecg_data_sem, com_res_s.m_task_count, Ble_SendEcgResultData);
        
        // ���͹���ͳ�ƽ������
        THREAD_MUTEX_TASK_EXCUTE(com_res_s.m_power_info_sem, com_res_s.m_task_count, Ble_SendPowerInfoData);
        
        THREAD_MUTEX_TASK_EXCUTE(com_res_s.m_update_set_sem, com_res_s.m_task_count, ComThread_UploadSwitchDataTask);

        ComThread_SetSendFlashDataState(0);
        
        App_Watch_Dog_Reload(); //1��ι��1��
        
        /* !ע�⣺������Ҫ�ж��㷨������Ƿ������, ֻ�����������, ���ܹ����߳�
        *         ������ﷵ��pdFALSEʱ, ˵���߳�û�е������� 
        */
        if(pdFALSE == xSemaphoreTake(com_res_s.m_task_count, (TickType_t)0))
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

/************************************************************************
** Function name:        ComThread_SendDailyData
** Descriptions:         ����ͨ���̷߳������������������
** input parameter:      p_data--Э������, ��ϸ�ο�ͨ��Э�� 
** Returned Value:       void
** Created  by:          GT
** Created  Datas:       2017-08-04
**************************************************************************/
void ComThread_SendDailyData(uint8_t* p_data)
{
    com_upload_s.daily.start = p_data[1] | (p_data[2]<<8);
    com_upload_s.daily.day_id = p_data[3];
    THREAD_TASK_WAKE_UP(com_res_s.m_daily_data_sem, com_res_s.m_task_count, com_res_s.m_thread_handle);
}

void ComThread_SendMovementData(uint8_t data_type, uint8_t movement_block)
{
    if(data_type == 0)
    {   //���ݿ�
        com_upload_s.movement.start = 0;
        com_upload_s.movement.total = 0;
        com_upload_s.movement.day_id = movement_block;
        com_upload_s.movement.data_type = 0;
    }
    else if(data_type == 1)
    {   //CRC
        com_upload_s.movement.start = 0;
        com_upload_s.movement.total = 0;
        com_upload_s.movement.day_id = 0;
        com_upload_s.movement.data_type = 1; 
    }
    else
    {
        return;
    }
    xSemaphoreGive(com_res_s.m_movement_sem);
    xSemaphoreGive(com_res_s.m_task_count);
    if(xSemaphoreTake(com_res_s.m_task_count, (TickType_t)0) == pdTRUE)
    {
        vTaskResume(com_res_s.m_thread_handle);
    }
//    THREAD_TASK_WAKE_UP(com_res_s.m_movement_sem, com_res_s.m_task_count, com_res_s.m_thread_handle);
}

/************************************************************************
** Function name:        ComThread_SendADCData
** Descriptions:         ����ͨ���̷߳���ADC����������
** input parameter:      ��
** Returned Value:       void
** Created  by:          GT
** Created  Datas:       2017-08-04
**************************************************************************/
void ComThread_SendADCData(void)
{
    if(BleThread_GetBleClk()->conn_handle != BLE_CONN_HANDLE_INVALID)
    {
        THREAD_TASK_WAKE_UP(com_res_s.m_upload_adc_sem, com_res_s.m_task_count, com_res_s.m_thread_handle);
    }
}

/************************************************************************
** Function name:        ComThread_SendAllFlashData
** Descriptions:         ����ͨ���̷߳��Ͷ�ȡ�û�FLASH����������
** input parameter:      ��
** Returned Value:       void
** Created  by:          GT
** Created  Datas:       2017-08-04
**************************************************************************/
void ComThread_SendAllFlashData(void)
{
    THREAD_TASK_WAKE_UP(com_res_s.m_all_flash_sem, com_res_s.m_task_count, com_res_s.m_thread_handle);
}

/************************************************************************
** Function name:        ComThread_SendSleepData
** Descriptions:         ����ͨ���̷߳���˯�ߴ洢����������
** input parameter:      p_data--Э�����ݣ���ϸ�ο�ͨ��Э��
** Returned Value:       void
** Created  by:          GT
** Created  Datas:       2017-08-04
**************************************************************************/
void ComThread_SendSleepData(uint8_t* p_data)
{
    com_upload_s.sleep.day_id = p_data[1];
    THREAD_TASK_WAKE_UP(com_res_s.m_sleep_data_sem, com_res_s.m_task_count, com_res_s.m_thread_handle);
}

/************************************************************************
** Function name:        ComThread_SendAlarmData
** Descriptions:         ����ͨ���̷߳�������洢����������
** input parameter:      ��
** Returned Value:       void
** Created  by:          GT
** Created  Datas:       2017-08-04
**************************************************************************/
void ComThread_SendAlarmData(void)
{
    THREAD_TASK_WAKE_UP(com_res_s.m_alarm_data_sem, com_res_s.m_task_count, com_res_s.m_thread_handle);
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
        if(p_data_manage->ecg_ppg_s.ppg_measure_s.app == 1)
        {
            xSemaphoreGive(com_res_s.m_heart_app); 
            xSemaphoreGive(com_res_s.m_task_count);
        }	
        // �ϴ�Ѫѹ����
        if(p_data_manage->ecg_ppg_s.ppg_measure_s.app_bp == 1)
        {
            xSemaphoreGive(com_res_s.m_bp_app); 
            xSemaphoreGive(com_res_s.m_task_count);
        }       
        // �ϴ�ECG����
        if(p_data_manage->ecg_ppg_s.ecg_measure_s.app == 1)
        {
            xSemaphoreGive(com_res_s.m_ecg_app); 
            xSemaphoreGive(com_res_s.m_task_count);
        }   
        // �ϴ�Ѫ������
        if(p_data_manage->ecg_ppg_s.oxygen_measure_s.ox_app == 1)
        {
            xSemaphoreGive(com_res_s.m_oxygen_app); 
            xSemaphoreGive(com_res_s.m_task_count);
        }
        
        // �����������ʱ����, ��ʱ����ʵʱ����
        if(p_flash_param->hourglass_s.app_send  == 1)
        {
            xSemaphoreGive(com_res_s.m_hourglass_app);
            xSemaphoreGive(com_res_s.m_task_count);
        }    
        
        vTaskResume(com_res_s.m_thread_handle); 

        xSemaphoreGive(com_res_s.m_update_set_sem); 
        xSemaphoreGive(com_res_s.m_task_count);        
    }    
}

/****************************************************************
** Function name:           ComThread_SendEcgData
** Descriptions:            ����ECG ID����
** input parameters:        id_type��id���ͣ�time_data��ʱ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2018-11-06
*****************************************************************/ 
void ComThread_SendEcgIdData(uint8_t id_flag, uint8_t *time_data)
{
    com_upload_s.ecg.id_type = 0;
    com_upload_s.ecg.id_flag = id_flag;
    memcpy((uint8_t *)&com_upload_s.ecg.year, time_data, 7);
    //memcpy((uint8_t *)&com_upload_s.ecg.month, &time_data[2], 5);
    //com_upload_s.ecg.year = time_data[0] + (time_data[1] << 8);
    
    THREAD_TASK_WAKE_UP(com_res_s.m_ecg_data_sem, com_res_s.m_task_count, com_res_s.m_thread_handle);
}


/****************************************************************
** Function name:           ComThread_SendEcgDataById
** Descriptions:            ����ECG ID����
** input parameters:        id_type��id���ͣ�time_data��ʱ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2018-11-15
*****************************************************************/ 
void ComThread_SendEcgDataById(uint8_t id_type, uint16_t id_num, uint8_t *time_data)
{
    com_upload_s.ecg.id_flag = 0;
    com_upload_s.ecg.id_type = id_type;
    com_upload_s.ecg.send_id = id_num;
    //memcpy((uint8_t *)&com_upload_s.ecg.year, time_data, 7);
    
    THREAD_TASK_WAKE_UP(com_res_s.m_ecg_data_sem, com_res_s.m_task_count, com_res_s.m_thread_handle);
}


/****************************************************************
** Function name:           ComThread_SendPowerInfoData
** Descriptions:            ���͹���ͳ������
** input parameters:        day_num:����
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2019-03-06
*****************************************************************/ 
void ComThread_SendPowerInfoData(uint8_t day_num)
{
    com_upload_s.power_info_day = day_num;
    
    THREAD_TASK_WAKE_UP(com_res_s.m_power_info_sem, com_res_s.m_task_count, com_res_s.m_thread_handle);
}


/*******************************************************************************************************
** Function name:        BLE_UploadSwitchData
** Descriptions:         App�����ֻ�,�ֻ���ʵʱһЩ����״̬
** input parameter:      ��     
** Returned Value:       void
** Created  by:          YX
** Created  Datas:       2018-09-18
*********************************************************************************************************/
void ComThread_UploadSwitchDataTask(void)
{
    uint8_t send_data[20] = {0};

    if(upload_switch_s.switch_data)
    {
        //�洢���ز���
        BleThread_AsynUpdateAppConfig();
    }    
    
    if(upload_switch_s.switch_flg.oled_bright_flg == 1)
    {
        //���ȵ���
        upload_switch_s.switch_flg.oled_bright_flg = 0;
    }
    
    if(upload_switch_s.switch_flg.display_time_flg == 1)
    {
        //��ʾʱ������
        memset(send_data, 0, 20);
        send_data[0] = 0xB4;
        send_data[1] = 1;
        send_data[2] = 2;
        send_data[3] = p_flash_param->ui_display_time;
        send_data[4] = UI_DISPLAY_TIME_RECOMMEND;
        send_data[5] = UI_DISPLAY_TIME_MAX;      
        send_data[6] = UI_DISPLAY_TIME_MIN; 
        Ble_BatTimeOutAutoRepeat(send_data,20);
        upload_switch_s.switch_flg.display_time_flg = 0;
        
    }
    
    if(upload_switch_s.switch_flg.dev_config_flg == 1)
    {
        //��������
        extern void Upload_EnglishStyle_Config(void);
        
        Upload_EnglishStyle_Config();
        upload_switch_s.switch_flg.dev_config_flg = 0; 
    }
    
    if(upload_switch_s.switch_flg.turnwrist_flg == 1)
    {
        //��������
        memset(send_data, 0, 20);
        send_data[0] = 0xAA;
        send_data[1] = 1;
        send_data[2] = 2;

        send_data[3] = p_flash_param->turnwrist_s.s_hour;
        send_data[4] = p_flash_param->turnwrist_s.s_minute;
        send_data[5] = p_flash_param->turnwrist_s.e_hour;
        send_data[6] = p_flash_param->turnwrist_s.e_minute;
        send_data[7] = (p_flash_param->turnwrist_s.enable==1)?1:2;
        send_data[8] = p_flash_param->turnwrist_s.sensitivity;
        send_data[9] = 5;
    
        Ble_BatTimeOutAutoRepeat(send_data,20);
        upload_switch_s.switch_flg.turnwrist_flg = 0;
        
    }
    
    if(upload_switch_s.switch_flg.sedentary_flg == 1)
    {
        //��������
        memset(send_data, 0, 20);
        send_data[0] = 0xE1;
        send_data[1] = 0x01;       
        send_data[2] = p_flash_param->sedentary_config_s.start_hour;
        send_data[3] = p_flash_param->sedentary_config_s.start_minute;
        send_data[4] = p_flash_param->sedentary_config_s.end_hour;
        send_data[5] = p_flash_param->sedentary_config_s.end_minute;
        send_data[6] = p_flash_param->sedentary_config_s.internal;  
        //��ǰ����״̬
        send_data[7] = p_flash_param->sedentary_config_s.enable;         
        send_data[8] = 2;
        Ble_BatTimeOutAutoRepeat(send_data,20);
        upload_switch_s.switch_flg.sedentary_flg = 0;
    }
    
    if(upload_switch_s.switch_flg.heart_alert_flg == 1)
    {
        //���ʱ���
        memset(send_data, 0, 20);
        send_data[0] = 0xAC;

        send_data[1] = p_flash_param->heart_alert_s.high;
        send_data[2] = p_flash_param->heart_alert_s.low;
        send_data[3] = 1;
        send_data[4] = 2;
        send_data[5] = p_flash_param->heart_alert_s.enable;         
        
        Ble_BatTimeOutAutoRepeat(send_data,20);
        upload_switch_s.switch_flg.heart_alert_flg = 0;
        
    }
    if(upload_switch_s.switch_flg.message_flg == 1)
    {  
//        //��Ϣ����
        extern void Protocol_Upload_Ancs_Config(void);
        
        Protocol_Upload_Ancs_Config();
        upload_switch_s.switch_flg.message_flg = 0;
        
    }
    
    if(upload_switch_s.switch_flg.alarm_flg == 1)
    {
        upload_switch_s.switch_flg.alarm_flg = 0;
        
    }
    
    if(upload_switch_s.switch_flg.oxygen_flg == 1)
    {
        // Ѫ��������ȡ��Ϣ
        memset(send_data, 0, 20);
        send_data[0] = 0xB3;
        send_data[1] = 0x01;
        send_data[2] = 0x00;
        send_data[3] = 0x01;	    
        send_data[4] = p_flash_param->blood_oxygen_config_s.start_hour;
        send_data[5] = p_flash_param->blood_oxygen_config_s.start_minute;
        send_data[6] = p_flash_param->blood_oxygen_config_s.end_hour;
        send_data[7] = p_flash_param->blood_oxygen_config_s.end_minute;
        send_data[8] = p_flash_param->blood_oxygen_config_s.enable;
        Ble_BatTimeOutAutoRepeat(send_data,20);
        
    }    
}

void ComThread_UpdateSetParamSem(void)
{
    if(upload_switch_s.switch_data == 0)
    {
       return ;
    }
    THREAD_TASK_WAKE_UP(com_res_s.m_update_set_sem, com_res_s.m_task_count, com_res_s.m_thread_handle);  
}



