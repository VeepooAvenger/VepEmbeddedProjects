/*******************Copyright(c)*********************************
**                  Veepoo Tech
**            COPYRIGHT (C) 2016-2019
**----------------------FileInfo---------------------------------
** File name:                  test_mode.h
** Last modified Date:         2018-06-29
** Last SYS_VERSION:           0.0.1
** Descriptions:
**---------------------------------------------------------------
** Createdby:                  WJ
** Createddate:                2018-06-29
** SYS_VERSION:                0.0.1
** Descriptions:               ����ģ��ӿ�
**---------------------------------------------------------------
** Modified by:				
** Modified date:			
** SYS_VERSION:				
** Modified Descriptions:	
****************************************************************/
#include <string.h>
#include "test_mode.h"
#include "data_manage.h"
#include "configuration.h"
#include "bracelet_protocol.h"
#include "ble_thread.h"
#include "api_flash.h"



//extern uint32_t dac_from_4404;
//extern uint32_t gain_from_4404;
//extern uint32_t charge_from_4404;

//void Test_HeartTimesStartRepeat(void);
void Test_OxygenTimesStartRepeat(void);

/*******************************************************************************************************
** Function name:        Test_ClkTimeOutAutoRepeat
** Descriptions:         ����CLK����ʱ�Զ��ط����ݽӿ�
** input parameter:      p_data ��������ָ��
**                       len    ���ͳ���
** Returned Value:       void
** Created  by:          WJ
** Created  Datas:       2018-06-29
*********************************************************************************************************/
static void Test_ClkTimeOutAutoRepeat(uint8_t* p_data, uint8_t len)
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
** Function name:        Test_ModeInit
** Descriptions:         �Զ��������ݲɼ�����
** input parameter:      ��
** Returned Value:       ��
** Created  by:          WJ
** Created  Datas:       2018-06-29
*********************************************************************************************************/
void Test_ModeInit(void)
{
    memset(&p_data_manage->test_ox_info_s, 0x00, sizeof(p_data_manage->test_ox_info_s));
}

/*******************************************************************************************************
** Function name:        Test_ProtocolParsing
** Descriptions:         Э�����
** input parameter:      ��
** Returned Value:       ��
** Created  by:          WJ
** Created  Datas:       2018-07-02
*********************************************************************************************************/
void Test_ProtocolParsing(uint8_t *in_data, uint8_t *out_data)
{
    if(in_data[1] == 0x02)
    {
        Test_ModeInit();
        p_data_manage->test_ox_info_s.flag = 0x01;
    }
    else
    {
        Test_ModeInit();
    }
    
    out_data[0] = 0x01;
    out_data[1] = in_data[1];
}

///*******************************************************************************************************
//** Function name:        Test_HeartStart
//** Descriptions:         �����Զ�������ʼ
//** input parameter:      ��
//** Returned Value:       ��
//** Created  by:          WJ
//** Created  Datas:       2018-06-29
//*********************************************************************************************************/
//void Test_HeartStart(void)
//{
//    if(p_data_manage->test_ox_info_s.flag)
//    {
//        p_data_manage->test_ox_info_s.mode = 2;
//        p_data_manage->test_ox_info_s.times_end = 0;
//        p_data_manage->test_ox_info_s.send_wait_len = 0; 
//        Test_HeartTimesStartRepeat();
//        p_data_manage->upload = UPLOAD_ADC_GREEN_EVENT;
//    }        
//}

///*******************************************************************************************************
//** Function name:        Test_HeartStop
//** Descriptions:         �����Զ���������
//** input parameter:      ��
//** Returned Value:       ��
//** Created  by:          WJ
//** Created  Datas:       2018-06-29
//*********************************************************************************************************/
//void Test_HeartStop(void)
//{
//    if(p_data_manage->test_ox_info_s.flag && p_data_manage->test_ox_info_s.mode == 2)
//    {
//        p_data_manage->test_ox_info_s.times_end = 1;
//        p_data_manage->test_ox_info_s.send_wait_len = p_data_manage->data_pool.send_wait_len; 
//        
//        if(p_data_manage->test_ox_info_s.send_wait_len < 5)
//        {
//            Test_HeartTimesEndRepeat();
//        }
//    }        
//}

///*******************************************************************************************************
//** Function name:        Test_HeartTimesStart
//** Descriptions:         �����Զ�������ʼ��־����
//** input parameter:      ��
//** Returned Value:       ��
//** Created  by:          WJ
//** Created  Datas:       2018-06-29
//*********************************************************************************************************/
//void Test_HeartTimesStartRepeat(void)
//{
//    uint8_t  ble_ack[20] = { 0x86, 0x01,};
////    uint32_t err_code = NRF_SUCCESS;
//	
//    if(p_data_manage->test_ox_info_s.flag && p_data_manage->test_ox_info_s.mode == 2)
//    {
//        Test_ClkTimeOutAutoRepeat(ble_ack, 20);
//        p_data_manage->test_ox_info_s.times_end = 0;
//        p_data_manage->test_ox_info_s.send_wait_len = 0; 
//    }
//}

///*******************************************************************************************************
//** Function name:        Test_HeartTimesEnd
//** Descriptions:         �����Զ�����������־����
//** input parameter:      ��
//** Returned Value:       ��
//** Created  by:          WJ
//** Created  Datas:       2018-06-29
//*********************************************************************************************************/
//void Test_HeartTimesEndRepeat(void)
//{
//    uint8_t  ble_ack[20] = { 0x86, 0x00,};
////    uint32_t err_code = NRF_SUCCESS;
//	
//    if(p_data_manage->test_ox_info_s.flag && p_data_manage->test_ox_info_s.mode == 2)
//    {
//        if(p_data_manage->heart_measure_s.day_time != 1 && p_data_manage->test_ox_info_s.times_end == 0)
//        {
//            Test_HeartStop();
//        }
//        
//        if(p_data_manage->test_ox_info_s.times_end != 1)
//        {
//            return;
//        }
//        if(p_data_manage->test_ox_info_s.send_wait_len >= 5)
//        {
//            p_data_manage->test_ox_info_s.send_wait_len -= 5;
//        }
//        else
//        {
//            p_data_manage->test_ox_info_s.send_wait_len = 0;
//        }
//        if(p_data_manage->test_ox_info_s.send_wait_len != 0)
//        {
//            return;
//        }
//    
//        Test_ClkTimeOutAutoRepeat(ble_ack, 20);
//        
//        p_data_manage->test_ox_info_s.mode = 0;
//    }
//}

///*******************************************************************************************************
//** Function name:        Test_HeartParameterRepeat
//** Descriptions:         ���ʲ�����������
//** input parameter:      ��
//** Returned Value:       ��
//** Created  by:          WJ
//** Created  Datas:       2018-06-29
//*********************************************************************************************************/
//void Test_HeartParameterRepeat(void)
//{
//    uint8_t  ble_ack[20] = { 0x86, 0x02,};
////    uint32_t err_code = NRF_SUCCESS;
//	
//    if(p_data_manage->test_ox_info_s.flag && p_data_manage->test_ox_info_s.mode == 2)
//    {
//        ble_ack[0] = 0x86;
//        ble_ack[1] = 0x02;
//        
//        ble_ack[2] = p_data_manage->heart_measure_s.real_heart;
//        
//        // �����ͨ��
//        if((p_flash_param->dev_config_s.led_gear == 0 && p_data_manage->wear_measure_s.wear_status == 1))
//        {
//            ble_ack[2] = 0x01;
//        }
//        //����ֻ�������������Ѫ��
//        if(p_data_manage->blood_oxygen_measure_s.btn == 1)
//        {
//            ble_ack[3] = 0x04;
//        } 
//        //����
//        ble_ack[7] = (gain_from_4404 & 0x07);
//        //���� DAC
//        ble_ack[8] = (dac_from_4404 & 0x0F);
//        //�̹� DAC
//        ble_ack[9] = (dac_from_4404 & 0x01E0) >> 5;
//        //������ DAC
//        ble_ack[10] = (dac_from_4404 & 0x03C00)>> 10;
//        //��� DAC
//        ble_ack[11] = (dac_from_4404 & 0x078000)>> 15;
//        // �����̹����
//        ble_ack[12] = (charge_from_4404 & 0x03F);
//        //���ͺ�����
//        ble_ack[13] = (charge_from_4404 & 0x0FC0)>>6;
//        //���ͺ������
//        ble_ack[14] = (charge_from_4404 & 0x03F000)>>12;

//        move_index_t cali_move_index_s = {0};
//        Move_IdexGet(&cali_move_index_s);
//        
//        ble_ack[15] = cali_move_index_s.move_5s;
//        ble_ack[16] = cali_move_index_s.move_30s;
//        ble_ack[17] = cali_move_index_s.move_60s;
//        ble_ack[18] = cali_move_index_s.move_120s;
//        
//        Test_ClkTimeOutAutoRepeat(ble_ack, 20);
//  
//        ble_ack[0] = 0x86;
//        ble_ack[1] = 0x03;
//        ble_ack[2] = 0x00;
//        
//        //WJ.20180702.��Ҫ����ʵ�����
//        ble_ack[3] =  p_data_manage->wear_measure_s.wear_status;         //20180705 HJJ ������
//        ble_ack[4] =  p_data_manage->wear_measure_s.large_num;           //20180705 HJJ ����10^5 �ĸ���
//        ble_ack[5] =  p_data_manage->wear_measure_s.little_num;          //20180705 HJJ С��10^5 ����10^4 �ĸ���
//        ble_ack[6] =  p_data_manage->wear_measure_s.main_signal_value_ave&0xFF;        //��λ
//        ble_ack[7] =  (p_data_manage->wear_measure_s.main_signal_value_ave>>8) &0xFF;  //�м�λ
//        ble_ack[8] =  (p_data_manage->wear_measure_s.main_signal_value_ave>>16) &0xFF; //��λ
//		
//        ble_ack[9] =   p_data_manage->wear_measure_s.amb_value_ave & 0xFF;           //��λ
//        ble_ack[10] = (p_data_manage->wear_measure_s.amb_value_ave>>8)  &0xFF;       //�м�λ
//        ble_ack[11] = (p_data_manage->wear_measure_s.amb_value_ave>>16) &0xFF;       //��λ
//		
//        ble_ack[12] = p_data_manage->heart_measure_s.real_heart;         //20180705 HJJ ʵʱ����ֵ 0;
//		ble_ack[13] = 0;//20180705 HJJ ���ʲ����޺����ʽ����0����;
//        ble_ack[14] = 0;//20180705 HJJ ���ʲ����޺����ʽ����0����;
//        ble_ack[15] = 0;//20180705 HJJ ���ʲ����޺����ʽ����0����;
//        ble_ack[16] = 0;//20180705 HJJ ���ʲ����޺����ʽ����0����;
//        ble_ack[17] = 0;//20180705 HJJ ���ʲ����޺����ʽ����0����;
//        ble_ack[18] = 0;//20180705 HJJ ���ʲ����޺����ʽ����0����;
//        ble_ack[19] = 0;//20180705 HJJ ���ʲ����޺����ʽ����0����;
//		
//        Test_ClkTimeOutAutoRepeat(ble_ack, 20);
//    }
//}

/*******************************************************************************************************
** Function name:        Test_OxygenStart
** Descriptions:         Ѫ���Զ�������ʼ
** input parameter:      ��
** Returned Value:       ��
** Created  by:          WJ
** Created  Datas:       2018-06-29
*********************************************************************************************************/
void Test_OxygenStart(void)
{
    if(p_data_manage->test_ox_info_s.flag)
    {
        p_data_manage->test_ox_info_s.mode = 1;
        p_data_manage->test_ox_info_s.times_end = 0;
        p_data_manage->test_ox_info_s.send_wait_len = 0; 
        Test_OxygenTimesStartRepeat();
        p_data_manage->upload = UPLOAD_ADC_OXYGEN_EVENT;
    }        
}

/*******************************************************************************************************
** Function name:        Test_HeartStop
** Descriptions:         �����Զ���������
** input parameter:      ��
** Returned Value:       ��
** Created  by:          WJ
** Created  Datas:       2018-06-29
*********************************************************************************************************/
void Test_OxygenStop(void)
{
    if(p_data_manage->test_ox_info_s.flag && p_data_manage->test_ox_info_s.mode == 1)
    {
        p_data_manage->test_ox_info_s.times_end = 1;
        p_data_manage->test_ox_info_s.send_wait_len = 0;
    }        
}

/*******************************************************************************************************
** Function name:        Test_OxygenTimesStart
** Descriptions:         Ѫ������Ӳ�����ʼ��־����
** input parameter:      ��
** Returned Value:       ��
** Created  by:          WJ
** Created  Datas:       2018-06-29
*********************************************************************************************************/
void Test_OxygenTimesStartRepeat(void)
{
    uint8_t  ble_ack[20] = { 0x87, 0x01,};
//    uint32_t err_code = NRF_SUCCESS;
	
    if(p_data_manage->test_ox_info_s.flag && p_data_manage->test_ox_info_s.mode == 1)
    {
        Test_ClkTimeOutAutoRepeat(ble_ack, 20);
        
        p_data_manage->test_ox_info_s.times_end = 0;
        p_data_manage->test_ox_info_s.send_wait_len = 0; 
    }
}

/*******************************************************************************************************
** Function name:        Test_OxygenTimesEnd
** Descriptions:         Ѫ������Ӳ���������־����
** input parameter:      ��
** Returned Value:       ��
** Created  by:          WJ
** Created  Datas:       2018-06-29
*********************************************************************************************************/
void Test_OxygenTimesEndRepeat(void)
{
    uint8_t  ble_ack[20] = { 0x87, 0x00,};
//    uint32_t err_code = NRF_SUCCESS;
	
    if(p_data_manage->test_ox_info_s.flag && p_data_manage->test_ox_info_s.mode == 1)
    {
        
        if(p_data_manage->test_ox_info_s.times_end != 1)
        {
            return;
        }    
        if(p_data_manage->test_ox_info_s.send_wait_len >= 5)
        {
            p_data_manage->test_ox_info_s.send_wait_len -= 5;
        }
        else
        {
            p_data_manage->test_ox_info_s.send_wait_len = 0;
        }
        if(p_data_manage->test_ox_info_s.send_wait_len != 0)
        {
            return;
        }
        
        Test_OxygenParameterRepeat();
    
        Test_ClkTimeOutAutoRepeat(ble_ack, 20);
        
        p_data_manage->test_ox_info_s.mode = 0;
    }
}

/*******************************************************************************************************
** Function name:        Test_OxygenParameterRepeat
** Descriptions:         Ѫ��������������
** input parameter:      ��
** Returned Value:       ��
** Created  by:          WJ
** Created  Datas:       2018-06-29
*********************************************************************************************************/
void Test_OxygenParameterRepeat(void)
{
    uint8_t  ble_ack[20] = { 0x87, 0x02,};
//    uint32_t err_code = NRF_SUCCESS;
	
    if(p_data_manage->test_ox_info_s.flag && p_data_manage->test_ox_info_s.mode == 1)
    {
        ble_ack[0] = 0x87;
        ble_ack[1] = 0x02;
        ble_ack[2] = 0x00;
         
        //����ֻ�����������������
        if(p_data_manage->heart_measure_s.btn == 1)
        {
            ble_ack[2] = 0x02;
            ble_ack[3] = p_data_manage->heart_measure_s.real_heart;
        }
        // �����ͨ��
        if((p_flash_param->dev_config_s.led_gear == 0 && p_data_manage->wear_measure_s.wear_status==1))
        {
            ble_ack[3] = 0x01;
        }
        // ��ȡѪ�����������У׼��״̬�ж�
        if(p_data_manage->blood_oxygen_measure_s.init == 0 && p_data_manage->blood_oxygen_measure_s.cal_state == 0)
        {
            ble_ack[4] = p_data_manage->blood_oxygen_measure_s.result;
        }
        //��Ѫ��У׼������У׼״̬�ͽ��ȸ� APP
        ble_ack[5] = p_data_manage->blood_oxygen_measure_s.cal_state;
        ble_ack[6] = p_data_manage->blood_oxygen_measure_s.cal_progress;
        
//        //����
//        ble_ack[7] = (gain_from_4404 & 0x07);
//		
//        //���� DAC
//        ble_ack[8] = (dac_from_4404 & 0x0F);
//        //�̹� DAC
//        ble_ack[9] = (dac_from_4404 & 0x01E0) >> 5;
//        //������ DAC
//        ble_ack[10] = (dac_from_4404 & 0x03C00)>> 10;
//        //��� DAC
//        ble_ack[11] = (dac_from_4404 & 0x078000)>> 15;
//		
//        // �����̹����
//        ble_ack[12] = (charge_from_4404 & 0x03F);
//        //���ͺ�����
//        ble_ack[13] = (charge_from_4404 & 0x0FC0)>>6;
//        //���ͺ������
//        ble_ack[14] = (charge_from_4404 & 0x03F000)>>12;

        move_index_t cali_move_index_s = {0};
        Move_IdexGet(&cali_move_index_s);
        
		
		
        ble_ack[15] = cali_move_index_s.move_5s;
        ble_ack[16] = cali_move_index_s.move_30s;
        ble_ack[17] = cali_move_index_s.move_60s;
        ble_ack[18] = cali_move_index_s.move_120s;
        
        Test_ClkTimeOutAutoRepeat(ble_ack, 20);
        
        ble_ack[0] = 0x87;
        ble_ack[1] = 0x03;
        ble_ack[2] = 0x00;
        
//        //WJ.20180702.��Ҫ����ʵ�����
//        ble_ack[3] =  p_data_manage->wear_measure_s.wear_status;         //20180705 HJJ ������
//        ble_ack[4] =  p_data_manage->wear_measure_s.large_num;           //20180705 HJJ ����10^5 �ĸ���
//        ble_ack[5] =  p_data_manage->wear_measure_s.little_num;          //20180705 HJJ С��10^5 ����10^4 �ĸ���
//        ble_ack[6] =  p_data_manage->wear_measure_s.main_signal_value_ave&0xFF;        //��λ
//        ble_ack[7] =  (p_data_manage->wear_measure_s.main_signal_value_ave>>8) &0xFF;  //�м�λ
//        ble_ack[8] =  (p_data_manage->wear_measure_s.main_signal_value_ave>>16) &0xFF; //��λ
//		
//        ble_ack[9] =   p_data_manage->wear_measure_s.amb_value_ave & 0xFF;           //��λ
//        ble_ack[10] = (p_data_manage->wear_measure_s.amb_value_ave>>8)  &0xFF;       //�м�λ
//        ble_ack[11] = (p_data_manage->wear_measure_s.amb_value_ave>>16) &0xFF;       //��λ
//		
        ble_ack[12] = p_data_manage->heart_measure_s.real_heart;         //20180705 HJJ ʵʱ����ֵ;
//        ble_ack[13] = p_data_manage->respiratory_rate_s.resp_second_value;      //20180705 HJJ ʵʱ������ֵ;
        ble_ack[14] = p_data_manage->oxygen_attr.breath_stop_result;    //20180705 HJJ ������ͣ�ȼ�;
        ble_ack[15] = p_data_manage->oxygen_attr.breath_stop_cnt;        //20180705 HJJ ������ͣ����;
        ble_ack[16] = p_data_manage->oxygen_attr.low_spo2_time;          //20180705 HJJ ƽ������ʱ��;
        ble_ack[17] = p_data_manage->oxygen_attr.breath_stop_time;       //20180705 HJJ ������ͣ��ƽ��ʱ��;
        ble_ack[18] = p_data_manage->oxygen_attr.spo2_modif_flag;        //20180705 HJJ ����Ѫ��������־λ
//        ble_ack[19] = p_data_manage->oxygen_attr.spo2_warning_flag;      //20180705 HJJ ����������־;        
        Test_ClkTimeOutAutoRepeat(ble_ack, 20);
    }
}

