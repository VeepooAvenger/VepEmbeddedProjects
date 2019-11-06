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
** Descriptions:               测试模块接口
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
#include "nrf_delay.h"


/*******************************************************************************************************
** Function name:        Test_ClkTimeOutAutoRepeat
** Descriptions:         蓝牙CLK服务超时自动重发数据接口
** input parameter:      p_data 发送数据指针
**                       len    发送长度
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
            //vTaskDelay(20);
            nrf_delay_ms(20);
            continue;
        }
        else 
        {
            break;	 
        }
    }
}

/*******************************************************************************************************
** Function name:        Test_ProtocolParsing
** Descriptions:         协议解析
** input parameter:      无
** Returned Value:       无
** Created  by:          WJ
** Created  Datas:       2018-07-02
*********************************************************************************************************/
void Test_ProtocolParsing(uint8_t *in_data, uint8_t *out_data)
{
    if(in_data[1] == 0x02)
    {
        p_data_manage->ppg_adc_mode = 0x01;
    }
    else
    {
        Test_HeartStop();
        p_data_manage->ppg_adc_mode = 0x00;
    }
    
    out_data[0] = 0x01;
    out_data[1] = in_data[1];
}

/*******************************************************************************************************
** Function name:        Test_HeartStart
** Descriptions:         心率自动测量开始
** input parameter:      无
** Returned Value:       无
** Created  by:          WJ
** Created  Datas:       2018-06-29
*********************************************************************************************************/
void Test_HeartStart(void)
{
    uint8_t  ble_ack[20] = { 0x86, 0x01,};
    
    if(p_data_manage->ppg_adc_mode)
    {
        Test_ClkTimeOutAutoRepeat(ble_ack, 20);
        p_data_manage->upload = UPLOAD_ADC_GREEN_EVENT;
    }        
}

/*******************************************************************************************************
** Function name:        Test_HeartStop
** Descriptions:         心率自动测量结束
** input parameter:      无
** Returned Value:       无
** Created  by:          WJ
** Created  Datas:       2018-06-29
*********************************************************************************************************/
void Test_HeartStop(void)
{
    uint8_t  ble_ack[20] = { 0x86, 0x00,};
    
    if(p_data_manage->ppg_adc_mode)
    {
        Test_ClkTimeOutAutoRepeat(ble_ack, 20);
    }        
}


