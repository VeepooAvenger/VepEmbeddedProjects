/*******************Copyright(c)*********************************
** Veepoo Tech
** COPYRIGHT (C) 2016-2019
**----------------------FileInfo---------------------------------
** File name:                  test_interface.h
** Last modified Date:         2018-11-14
** Last SYS_VERSION:           0.0.1
** Descriptions:
**---------------------------------------------------------------
** Createdby:                  WJ
** Createddate:                2018-11-14
** SYS_VERSION:                0.0.1
** Descriptions:               ���Խӿ�
**---------------------------------------------------------------
** Modified by:				
** Modified date:			
** SYS_VERSION:				
** Modified Descriptions:	
****************************************************************/
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include "test_interface.h"

#include "ext_flash.h"
#include "gui_thread.h"
#include "data_manage.h"
#include "api_flash.h"
#include "flash_data_type.h"
#include "SystemTime.h"
#include "api_ext_flash.h"

#include "ble_thread.h"
#include "communication_thread.h"

#include "Acculate_SleepAlgorithm.h"

/****************************************************************
** Function name:           Test_SetEcgResultData
** Descriptions:            ����ECG�ֶ����������������
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2018-11-14
*****************************************************************/ 
void Test_SetEcgResultData(void)
{
    uint8_t   set_data_buf[14] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13};
   
    p_data_manage->ecg_result_data_s.mark = 0xAB;
    p_data_manage->ecg_result_data_s.year = clock->year;
    p_data_manage->ecg_result_data_s.month = clock->month;
    p_data_manage->ecg_result_data_s.day = clock->day;
    p_data_manage->ecg_result_data_s.hour = clock->hour;
    p_data_manage->ecg_result_data_s.minute = clock->minutes;
    p_data_manage->ecg_result_data_s.second = clock->seconds;
    
    memcpy(p_data_manage->ecg_result_data_s.ecg_result_data, set_data_buf, 14);
    
    p_data_manage->ecg_result_data_s.ecg_res[0] = 0xF1;
    p_data_manage->ecg_result_data_s.ecg_res[1] = 0xF2;
    
    
    BleThread_SaveEcgResultDataSem();
    
}

/****************************************************************
** Function name:           Test_SetAutomaticEcgResultData
** Descriptions:            ����ECG�Զ������쳣�����������
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2018-11-14
*****************************************************************/ 
void Test_SetAutomaticEcgResultData(void)
{
    uint8_t buf[25] = {0};
    
    for(uint8_t i = 0; i < 25; i++)
    {
        buf[i] = i;
    }
    memcpy((uint8_t *)&p_data_manage->ecg_ppg_s.ecg_measure_s.ecg_test_result_s, buf, 25);
    for(uint16_t i = 0; i < 250*6; i++)
    {
        p_data_manage->data_pool.ecg_data[i] = i;
    }
 
    Extflash_EcgWriteSet((uint8_t *)p_data_manage->data_pool.ecg_data, 250*6*4, 
                         (uint8_t *)&p_data_manage->ecg_ppg_s.ecg_measure_s.ecg_test_result_s, 25);
    
    BleThread_ExtFlashOperationSem();
    
}

/****************************************************************
** Function name:           Test_SendEcgResultId
** Descriptions:            ����ecg�������id
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2018-11-15
*****************************************************************/ 
void Test_SendEcgResultId(void)
{
   // uint8_t time_data[7] = {0xE2, 0x07, 0x0B, 0x0F, 0x0A, 0x00, 0x00};
    uint8_t time_data[7] = {0xE2, 0x07, 0x0B, 0x0F, 0x0A, 0x25, 0x1E};
        
    ComThread_SendEcgIdData(0x02, time_data);
}

/****************************************************************
** Function name:           Test_SendEcgResultDataById
** Descriptions:            ����ecg�������,ͨ��id
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2018-11-15
*****************************************************************/ 
void Test_SendEcgResultDataById(void)
{
    ComThread_SendEcgDataById(0x02, 0x0A, NULL);
}

/****************************************************************
** Function name:           Test_SendEcgAutomaticResultId
** Descriptions:            ����ecg�Զ��������id
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2018-11-15
*****************************************************************/ 
void Test_SendEcgAutomaticResultId(void)
{
    uint8_t time_data[7] = {0xE2, 0x07, 0x0B, 0x0F, 0x0A, 0x00, 0x00};
        
    ComThread_SendEcgIdData(0x03, time_data);
}

/****************************************************************
** Function name:           Test_SendEcgAutomaticResultDataById
** Descriptions:            ����ecg�Զ��������,ͨ��id
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2018-11-15
*****************************************************************/ 
void Test_SendEcgAutomaticResultDataById(void)
{
    ComThread_SendEcgDataById(3, 0, NULL);
}

/****************************************************************
** Function name:           Test_SetSleepData
** Descriptions:            ���þ�׼˯�ߵ�����
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2018-11-14
*****************************************************************/ 
uint8_t *Test_SetSleepData(uint8_t *p_sleep_info)
{
    static uint8_t sleep_cnt = 0;
	acculate_sleep_quality_t *p_asq = NULL;
	
	p_asq = (acculate_sleep_quality_t *)p_sleep_info;
	
    sleep_cnt = 3;
    if(sleep_cnt == 0)
    {
        // First Part
        p_asq->enter_time.month = clock->month; //��˯ʱ���
        p_asq->enter_time.date = clock->day;
        p_asq->enter_time.hour = 0;
        p_asq->enter_time.minute = 8;
        
        p_asq->quit_time.month = clock->month; //�˳�ʱ���
        p_asq->quit_time.date = clock->day;
        p_asq->quit_time.hour = 12;
        p_asq->quit_time.minute = 8;
        
        p_asq->sleep_flag = 1;//˯�߱�־λ
        p_asq->startup_quality = 1; //��ҹ�÷�
        p_asq->deepsleep_quality = 1; //��˯�÷�
        p_asq->averstarttodeep_quality = 1; //˯��Ч�ʵ÷�
        p_asq->firstdeeptime_quality = 1; //��˯Ч�ʵ÷�
        p_asq->all_sleep_len_quality = 1; //˯��ʱ���÷�
        p_asq->quit_mode = 1; //�˳�˯��ģʽ
        p_asq->quanlity_val = 1; //˯������
        p_asq->startup_times = 1;//��ҹ����
        p_asq->deep_shallow_len_mode = 1; // ��ǳ˯��ģʽ
        p_asq->deep_sleep_len = 450; //��˯ʱ��
        p_asq->shallow_sleep_len = 270; //ǳ˯ʱ��
        p_asq->else_sleep_len = 1; //����ʱ��
        p_asq->all_sleep_len = 720; //˯����ʱ��
        p_asq->first_deep_time = 1; //��һ�ν�����˯ʱ��
        p_asq->startup_len = 1; //��ҹ��ʱ��
        p_asq->aver_start_to_deep = 1; //��ҹ����˯��ֵ
        p_asq->one_point_how_many_seconds = 1; //һ������������
        
        // Second Part
        p_asq->loss_sleep_flag = 1; //ʧ�߱�־
        p_asq->loss_sleep_degree = 1; //ʧ�ߵ÷�
        p_asq->loss_sleep_times = 1; //ʧ�ߴ���
        p_asq->loss_sleep_length = 1; //ʧ�߳���
        //p_asq->loss_sleep_start_time[10]; //ʧ�߿�ʼʱ��
        for(uint32_t i = 0; i < 10; i++)
        {
            p_asq->loss_sleep_start_time[i] = i + 1;
        }
        //p_asq->loss_sleep_end_time[10]; //ʧ�߽���ʱ��
        for(uint32_t i = 0; i < 10; i++)
        {
            p_asq->loss_sleep_end_time[i] = i + 1;
        }
        
        // Third Part
        p_asq->temp_len = 1440;//��ǳ˯��������Ч����
        //p_asq->deep_shallow[2880]; //��ǳ˯������
        for(uint32_t i = 0; i < 2880; i++)
        {
            p_asq->deep_shallow[i] = i & 0xff;
        }

        // Forth Part
        p_asq->CRC_value1 = 0xAA;//����˯����ϢУ��CRC1
        p_asq->CRC_value2 = 0x55;//ʧ����ϢУ��CRC2
        p_asq->CRC_value3 = 0xAA;//˯��������ϢУ��CRC3
	
    }
    else if(sleep_cnt == 1)
    {
       // First Part
        p_asq->enter_time.month = clock->month; //��˯ʱ���
        p_asq->enter_time.date = clock->day;
        p_asq->enter_time.hour = 13;
        p_asq->enter_time.minute = 8;
        
        p_asq->quit_time.month = clock->month; //�˳�ʱ���
        p_asq->quit_time.date = clock->day;
        p_asq->quit_time.hour = 15;
        p_asq->quit_time.minute = 8;
        
        p_asq->sleep_flag = 1;//˯�߱�־λ
        p_asq->startup_quality = 1; //��ҹ�÷�
        p_asq->deepsleep_quality = 1; //��˯�÷�
        p_asq->averstarttodeep_quality = 1; //˯��Ч�ʵ÷�
        p_asq->firstdeeptime_quality = 1; //��˯Ч�ʵ÷�
        p_asq->all_sleep_len_quality = 1; //˯��ʱ���÷�
        p_asq->quit_mode = 1; //�˳�˯��ģʽ
        p_asq->quanlity_val = 1; //˯������
        p_asq->startup_times = 1;//��ҹ����
        p_asq->deep_shallow_len_mode = 1; // ��ǳ˯��ģʽ
        p_asq->deep_sleep_len = 100; //��˯ʱ��
        p_asq->shallow_sleep_len = 20; //ǳ˯ʱ��
        p_asq->else_sleep_len = 1; //����ʱ��
        p_asq->all_sleep_len = 120; //˯����ʱ��
        p_asq->first_deep_time = 1; //��һ�ν�����˯ʱ��
        p_asq->startup_len = 1; //��ҹ��ʱ��
        p_asq->aver_start_to_deep = 1; //��ҹ����˯��ֵ
        p_asq->one_point_how_many_seconds = 1; //һ������������
        
        // Second Part
        p_asq->loss_sleep_flag = 1; //ʧ�߱�־
        p_asq->loss_sleep_degree = 1; //ʧ�ߵ÷�
        p_asq->loss_sleep_times = 1; //ʧ�ߴ���
        p_asq->loss_sleep_length = 1; //ʧ�߳���
        //p_asq->loss_sleep_start_time[10]; //ʧ�߿�ʼʱ��
        for(uint32_t i = 0; i < 10; i++)
        {
            p_asq->loss_sleep_start_time[i] = i + 1;
        }
        //p_asq->loss_sleep_end_time[10]; //ʧ�߽���ʱ��
        for(uint32_t i = 0; i < 10; i++)
        {
            p_asq->loss_sleep_end_time[i] = i + 1;
        }
        
        // Third Part
        p_asq->temp_len = 240;//��ǳ˯��������Ч����
        //p_asq->deep_shallow[2880]; //��ǳ˯������
        for(uint32_t i = 0; i < 2880; i++)
        {
            p_asq->deep_shallow[i] = i & 0xff;
        }

        // Forth Part
        p_asq->CRC_value1 = 0xFF;//����˯����ϢУ��CRC1
        p_asq->CRC_value2 = 0xCC;//ʧ����ϢУ��CRC2
        p_asq->CRC_value3 = 0xDD;//˯��������ϢУ��CRC3        
    }
    else 
    {
       // First Part
        p_asq->enter_time.month = clock->month; //��˯ʱ���
        p_asq->enter_time.date = clock->day;
        p_asq->enter_time.hour = 16;
        p_asq->enter_time.minute = 8;
        
        p_asq->quit_time.month = clock->month; //�˳�ʱ���
        p_asq->quit_time.date = clock->day;
        p_asq->quit_time.hour = 19;
        p_asq->quit_time.minute = 8;
        
        p_asq->sleep_flag = 1;//˯�߱�־λ
        p_asq->startup_quality = 1; //��ҹ�÷�
        p_asq->deepsleep_quality = 1; //��˯�÷�
        p_asq->averstarttodeep_quality = 1; //˯��Ч�ʵ÷�
        p_asq->firstdeeptime_quality = 1; //��˯Ч�ʵ÷�
        p_asq->all_sleep_len_quality = 1; //˯��ʱ���÷�
        p_asq->quit_mode = 1; //�˳�˯��ģʽ
        p_asq->quanlity_val = 1; //˯������
        p_asq->startup_times = 1;//��ҹ����
        p_asq->deep_shallow_len_mode = 1; // ��ǳ˯��ģʽ
        p_asq->deep_sleep_len = 100; //��˯ʱ��
        p_asq->shallow_sleep_len = 180; //ǳ˯ʱ��
        p_asq->else_sleep_len = 1; //����ʱ��
        p_asq->all_sleep_len = 180; //˯����ʱ��
        p_asq->first_deep_time = 1; //��һ�ν�����˯ʱ��
        p_asq->startup_len = 1; //��ҹ��ʱ��
        p_asq->aver_start_to_deep = 1; //��ҹ����˯��ֵ
        p_asq->one_point_how_many_seconds = 1; //һ������������
        
        // Second Part
        p_asq->loss_sleep_flag = 1; //ʧ�߱�־
        p_asq->loss_sleep_degree = 1; //ʧ�ߵ÷�
        p_asq->loss_sleep_times = 1; //ʧ�ߴ���
        p_asq->loss_sleep_length = 1; //ʧ�߳���
        //p_asq->loss_sleep_start_time[10]; //ʧ�߿�ʼʱ��
        for(uint32_t i = 0; i < 10; i++)
        {
            p_asq->loss_sleep_start_time[i] = i + 1;
        }
        //p_asq->loss_sleep_end_time[10]; //ʧ�߽���ʱ��
        for(uint32_t i = 0; i < 10; i++)
        {
            p_asq->loss_sleep_end_time[i] = i + 1;
        }
        
        // Third Part
        p_asq->temp_len = 180*2;//��ǳ˯��������Ч����
        //p_asq->deep_shallow[2880]; //��ǳ˯������
        for(uint32_t i = 0; i < 2880; i++)
        {
            p_asq->deep_shallow[i] = i & 0xff;
        }

        // Forth Part
        p_asq->CRC_value1 = 0xEE;//����˯����ϢУ��CRC1
        p_asq->CRC_value2 = 0xDD;//ʧ����ϢУ��CRC2
        p_asq->CRC_value3 = 0xCC;//˯��������ϢУ��CRC3        
    }
	return p_sleep_info;
}


/****************************************************************
** Function name:           Test_SendSleepDataById
** Descriptions:            ����˯������
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2018-11-20
*****************************************************************/ 
void Test_SendSleepDataById(void)
{
    uint8_t data_buf[2] = {0xFF, 0x01};
    
    ComThread_SendSleepData(data_buf);
}

