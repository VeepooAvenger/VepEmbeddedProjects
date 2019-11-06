/*******************Copyright(c)*********************************
** Veepoo Tech
** COPYRIGHT (C) 2016-2019
**----------------------FileInfo---------------------------------
** File name:                  api_flash.c
** Last modified Date:         2017-07-10
** Last SYS_VERSION:           0.0.1
** Descriptions:
**---------------------------------------------------------------
** Createdby:                  ����
** Createddate:                2017-07-10
** SYS_VERSION:                0.0.1
** Descriptions:               flash �ӿ�
**---------------------------------------------------------------
** Modified by:				
** Modified date:			
** SYS_VERSION:				
** Modified Descriptions:	
****************************************************************/

#include <stdint.h>
#include <string.h>
#include "nrf_error.h"
#include "fstorage.h"
#include "app_error.h"
#include "api_flash.h"
#include "app_timer.h"
#include "nrf_soc.h"
#include "Dev_SN_Check.h"
#include "softdevice_handler.h"
#include "app_config_data.h"
#include "flash_common.h"
#include "log.h"
#include "util.h"
#include "ble_thread.h"
#include "movement_patterns.h"
#include "pedometer.h"
#include "backlight_driving.h"
#include "api_ext_flash.h"
#include "ext_flash.h"
#include "util.h"

#include "watch_dog.h"

static flash_param_t  flash_param_s;
flash_param_t* p_flash_param = (flash_param_t*)&flash_param_s;

uint8_t sleep_cal_black = 0;        //����˯�ߵĿ��

//����У������
static const uint16_t crc16_tab[256] = 
{ 
    0x0000, 0xC0C1, 0xC181, 0x0140, 0xC301, 0x03C0, 0x0280, 0xC241, 
    0xC601, 0x06C0, 0x0780, 0xC741, 0x0500, 0xC5C1, 0xC481, 0x0440, 
    0xCC01, 0x0CC0, 0x0D80, 0xCD41, 0x0F00, 0xCFC1, 0xCE81, 0x0E40, 
    0x0A00, 0xCAC1, 0xCB81, 0x0B40, 0xC901, 0x09C0, 0x0880, 0xC841, 
    0xD801, 0x18C0, 0x1980, 0xD941, 0x1B00, 0xDBC1, 0xDA81, 0x1A40, 
    0x1E00, 0xDEC1, 0xDF81, 0x1F40, 0xDD01, 0x1DC0, 0x1C80, 0xDC41, 
    0x1400, 0xD4C1, 0xD581, 0x1540, 0xD701, 0x17C0, 0x1680, 0xD641, 
    0xD201, 0x12C0, 0x1380, 0xD341, 0x1100, 0xD1C1, 0xD081, 0x1040, 
    0xF001, 0x30C0, 0x3180, 0xF141, 0x3300, 0xF3C1, 0xF281, 0x3240, 
    0x3600, 0xF6C1, 0xF781, 0x3740, 0xF501, 0x35C0, 0x3480, 0xF441, 
    0x3C00, 0xFCC1, 0xFD81, 0x3D40, 0xFF01, 0x3FC0, 0x3E80, 0xFE41, 
    0xFA01, 0x3AC0, 0x3B80, 0xFB41, 0x3900, 0xF9C1, 0xF881, 0x3840, 
    0x2800, 0xE8C1, 0xE981, 0x2940, 0xEB01, 0x2BC0, 0x2A80, 0xEA41, 
    0xEE01, 0x2EC0, 0x2F80, 0xEF41, 0x2D00, 0xEDC1, 0xEC81, 0x2C40, 
    0xE401, 0x24C0, 0x2580, 0xE541, 0x2700, 0xE7C1, 0xE681, 0x2640, 
    0x2200, 0xE2C1, 0xE381, 0x2340, 0xE101, 0x21C0, 0x2080, 0xE041, 
    0xA001, 0x60C0, 0x6180, 0xA141, 0x6300, 0xA3C1, 0xA281, 0x6240, 
    0x6600, 0xA6C1, 0xA781, 0x6740, 0xA501, 0x65C0, 0x6480, 0xA441, 
    0x6C00, 0xACC1, 0xAD81, 0x6D40, 0xAF01, 0x6FC0, 0x6E80, 0xAE41, 
    0xAA01, 0x6AC0, 0x6B80, 0xAB41, 0x6900, 0xA9C1, 0xA881, 0x6840, 
    0x7800, 0xB8C1, 0xB981, 0x7940, 0xBB01, 0x7BC0, 0x7A80, 0xBA41, 
    0xBE01, 0x7EC0, 0x7F80, 0xBF41, 0x7D00, 0xBDC1, 0xBC81, 0x7C40, 
    0xB401, 0x74C0, 0x7580, 0xB541, 0x7700, 0xB7C1, 0xB681, 0x7640, 
    0x7200, 0xB2C1, 0xB381, 0x7340, 0xB101, 0x71C0, 0x7080, 0xB041, 
    0x5000, 0x90C1, 0x9181, 0x5140, 0x9301, 0x53C0, 0x5280, 0x9241, 
    0x9601, 0x56C0, 0x5780, 0x9741, 0x5500, 0x95C1, 0x9481, 0x5440, 
    0x9C01, 0x5CC0, 0x5D80, 0x9D41, 0x5F00, 0x9FC1, 0x9E81, 0x5E40, 
    0x5A00, 0x9AC1, 0x9B81, 0x5B40, 0x9901, 0x59C0, 0x5880, 0x9841, 
    0x8801, 0x48C0, 0x4980, 0x8941, 0x4B00, 0x8BC1, 0x8A81, 0x4A40, 
    0x4E00, 0x8EC1, 0x8F81, 0x4F40, 0x8D01, 0x4DC0, 0x4C80, 0x8C41, 
    0x4400, 0x84C1, 0x8581, 0x4540, 0x8701, 0x47C0, 0x4680, 0x8641, 
    0x8201, 0x42C0, 0x4380, 0x8341, 0x4100, 0x81C1, 0x8081, 0x4040 
};

/************************************************************************
** Function name:        Crc16
** Descriptions:         crc У��  
** input parameter:      crc:У��ֵ  data:��ҪУ��ֵ   
** Returned Value:       crc
** Created  by:          ����
** Created  Datas:       2017-08-04
**-----------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
**************************************************************************/
/*static __inline */uint16_t Crc16(uint16_t crc, uint8_t *data, uint16_t len) 
{ 
    uint32_t uIndex ; 
    while(len--) 
    { 
        uIndex = (crc&0xff) ^ ((*data) & 0xff) ; 
        data = data + 1; 
        crc = ((crc>>8) & 0xff) ^ crc16_tab[uIndex]; 
    } 
    return crc ; 
}

/************************************************************************
** Function name:      Api_FlashParamInit
** Descriptions:       flash ������ʼ�� 
** input parameter:    ��   
** Returned Value:     ��
** Created  by:        ����
** Created  Datas:     2017-08-04
**-----------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
**************************************************************************/
static void Api_FlashParamInit(void)
{
    memset(&flash_param_s, 0, sizeof(flash_param_s));
	   
    //WJ.20181105.�������ƵĴ洢λ���޸ĵ�������Ϣ��
    //App_GetBluetoothName(&flash_param_s.bluetooth_name_s);
    Dev_GetProductionInfomation(&flash_param_s.product_infomation_s);
    // WJ.20181105.��ȡ��������
    Dev_CheckBleAdvName();   
    // ����
    flash_param_s.sit_func_state = Dev_GetSitFunction();     
    // Ѫѹ
    flash_param_s.bp_func_state = Dev_GetBpFunction();    
    
    // ϵͳʱ����������
    Record_GetSystemTimeIndex(&flash_param_s.systemtime_index);
       
    // ��ʼ����ȡ����
    App_GetConnectPassword(&flash_param_s.password);
    // ��ȡ�û�������Ϣ
    App_GetSportCount(&flash_param_s.sport_target);
    App_GetPersonInfomation(&flash_param_s.person_s);
       
    App_GetHeartAlertParam(&flash_param_s.heart_alert_s.enable, 
                           &flash_param_s.heart_alert_s.high, 
                           &flash_param_s.heart_alert_s.low);
    // ��ȡ���󿪹���Ϣ
    App_GetTurnWristParam(&flash_param_s.turnwrist_s);
    App_GetDevConfig(&flash_param_s.dev_config_s);
    App_GetMessageEnable(&flash_param_s.message_enable_s);
    App_GetSedentaryConfig(&flash_param_s.sedentary_config_s);
    App_GetBrightnessParam(&flash_param_s.brightness_s);
    
    // Ѫѹ
    App_GetBloodMode(&flash_param_s.bp_s);    
       
       
    //��ȡ������Ϣ
    App_GetGuiLanguage(&flash_param_s.language_e);
#if (!APP_USED_OLD_ALARM_ENABLE)    
    //��ʼ������ʱ��
    for(uint8_t index = 0; index < 20; index++)
    {
        Record_GetAlarmTimeParam(index, &flash_param_s.alarm_time_s[index]);
    }
#else
    Record_GetOldAlarmTimeParam(flash_param_s.alarm_old_time);
#endif //#if (!APP_USED_OLD_ALARM_ENABLE)    
    App_GetHourglassParam(&flash_param_s.hourglass_s);
    App_GetBloodOxygenParam(&flash_param_s.blood_oxygen_config_s);
	
	App_GetUiStyleParam(&flash_param_s.ui_style);
    App_GetMoveMaxPuseTimeParam(&flash_param_s.movement_max_puse_time);
    
    // �ճ�����FLASH�������Ҽ���				
    Record_LogDataIndexInit(&flash_param_s.flash_index_s.log_s);
    Record_DailyDataIndexInit(&flash_param_s.flash_index_s.daily);

    Record_BlockIndexInit(&flash_param_s.flash_index_s.block_index); 
    
    //WJ.20180821.����UI�������ֲ���
    App_GetUiDisplayTimeParam(&flash_param_s.ui_display_time);
    
    // WJ.20181105.����ecg�洢������ʼ��
    Record_EcgDataIndexInit(&flash_param_s.flash_index_s.ecg);      
    
    {   //WJ.20190105.ECG�Զ��������ݴ洢������ʼ��
        p_data_manage->ext_flash_ecg_opt_s.init = 1;
        Extflash_EcgErrDeal();
    }
}

/************************************************************************
** Function name:        Api_AsynUpdateAppConfig
** Descriptions:         ����flash���� 
** input parameter:      void   
** Returned Value:       NRF_SUCCESS
** Created  by:          ����
** Created  Datas:       2017-08-04
**-----------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
**************************************************************************/
uint32_t Api_AsynUpdateAppConfig(void)
{
    Flash_CommonErase((uint32_t*)SYSTEM_PARAM_BASE_ADDR, 1);
       
    //App_UpdateBluetoothName(&flash_param_s.bluetooth_name_s);
    App_UpdateConnectPassword(flash_param_s.password);
    App_UpdateSportCount(flash_param_s.sport_target);
    App_UpdatePersonInfomation(&flash_param_s.person_s);
       
    // �������ʸ澯����
    App_UpdateHeartAlertParam(flash_param_s.heart_alert_s.enable, 
                              flash_param_s.heart_alert_s.high, 
                              flash_param_s.heart_alert_s.low);
    // ���·��󿪹���Ϣ
    App_UpdateTurnWristParam(&flash_param_s.turnwrist_s);
    App_UpdateDevConfig(&flash_param_s.dev_config_s);
    App_UpdateMessageEnable(&flash_param_s.message_enable_s);
    App_UpdateSedentaryConfig(&flash_param_s.sedentary_config_s);
    App_UpdateBrightnessParam(&flash_param_s.brightness_s);
    
    // Ѫѹ
    App_SaveBloodMode(&flash_param_s.bp_s);

    //��������
    App_UpdateGuiLanguage(flash_param_s.language_e);
    // ������������ʱ��
#if (!APP_USED_OLD_ALARM_ENABLE)    
    for(uint8_t index = 0; index < 20; index++)
    {
        Record_UpdateAlarmTimeParam(index, &flash_param_s.alarm_time_s[index]);
    }
#else
     Record_UpdateOldAlarmTimeParam(flash_param_s.alarm_old_time);
#endif //APP_USED_OLD_ALARM_ENABLE    
                
    App_UpdateHourglassParam(&flash_param_s.hourglass_s);
    App_UpdateBloodOxygenParam(&flash_param_s.blood_oxygen_config_s);
	
	App_UpdateUiStyleParam(flash_param_s.ui_style);
    
    App_UpdateMoveMaxPuseTimeParam(flash_param_s.movement_max_puse_time);
       
    //WJ.20180821.����UI�������ֲ���
    App_UpdateUiDisplayTimeParam(flash_param_s.ui_display_time);
    
    return NRF_SUCCESS;
}

/************************************************************************
** Function name:        Api_FlashInit
** Descriptions:         flash  ��ʼ�� 
** input parameter:      ��  
** Returned Value:       ��
** Created  by:          ����
** Created  Datas:       2017-08-04
**-----------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
**************************************************************************/
void Api_FlashInit(void)
{
	uint8_t history_system_time[8] = {0};
	
    Flash_CommonInit();
    
	//YX.20180123.�����ʱ������ݺ����Ͻ�����������ճ����ݴ洢����������	
    if(Api_ReadSystemTimeData(history_system_time) == NRF_SUCCESS)
    {
        SystemTime_Adjust(history_system_time, sizeof(history_system_time));
    }
    else
    {
        SystemTime_Init();
    }
	
    
    Api_DevSNCheck();
    
    Api_FlashParamInit();
    
}

/************************************************************************
** Function name:        Api_AdjustTimeClearFlash
** Descriptions:         ����ʱ�����flash
** input parameter:      ��  
** Returned Value:       ��
** Created  by:          ����
** Created  Datas:       2017-08-04
**-----------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
**************************************************************************/
bool Api_AdjustTimeClearFlash(const system_clock_time_t *p_history_time, const system_clock_time_t *p_current_time)
{
    uint32_t diff_hours = 0;
    bool b_data_clear_success = false;

    diff_hours = Get_SystemTimeDiffHour(p_current_time, p_history_time);
    if((diff_hours >= 2)&&(diff_hours < 24)&&(DateTimeToDecimalString(p_history_time) > DateTimeToDecimalString(p_current_time)))
    {
        // ������������
        b_data_clear_success = true;
	    if(Movement_IsBusy())
        {
            Movement_Exit();
        }
		if(p_history_time->day != p_current_time->day)
		{
			//���������ǰ��ʱ�������������
			for(uint8_t index = 0; index < 3; index++)
			{
				Record_ClearOneDayData(index);
			}
		}
		else
		{
			Record_ClearOneDayData(flash_param_s.flash_index_s.block_index);
		}
    }
    else if(diff_hours >= 24)
    {
        if(Movement_IsBusy())
        {
            Movement_Exit();
        }
        b_data_clear_success = true;
        for(uint8_t index = 0; index < 3; index++)
        {
            Record_ClearOneDayData(index);
        }
    }
	else if(p_history_time->day != p_current_time->day)
	{
		if(Movement_IsBusy())
        {
            Movement_Exit();
        }
		b_data_clear_success = true;
		//��ʱ����
		if(p_history_time->day < p_current_time->day)
		{
			//��������һ��ȥ
			flash_param_s.flash_index_s.block_index = (flash_param_s.flash_index_s.block_index+1) % 3;
			Record_ClearOneDayData(flash_param_s.flash_index_s.block_index);
		}
		else
		{
			//����������ݣ���ʱ��ǰһ��
			for(uint8_t index = 0; index < 3; index++)
			{
				Record_ClearOneDayData(index);
			}
		}
	}

    if(b_data_clear_success)
    {
        Ped_SetSportsInfo(0, 0, 0);
        Record_DailyDataIndexInit(&flash_param_s.flash_index_s.daily);
        //YX.20171124.���������ݺ�������������ݴ��ҵ�����
        ////Record_BlockIndexInit(&flash_param_s.flash_index_s.block_index);
    }

    return b_data_clear_success;    
}

/************************************************************************
** Function name:        Api_WriteLogText
** Descriptions:         дflash log ����
** input parameter:      p_content:��־���� len����־����   
** Returned Value:       ��
** Created  by:          ����
** Created  Datas:       2017-08-04
**-----------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
**************************************************************************/
void Api_WriteLogText(uint8_t* p_content, uint8_t len)
{
    uint32_t  err_code = NRF_SUCCESS;
    uint32_t  log_data[16] = {0};
       
    memcpy(log_data, p_content, 64);
    err_code == Record_AddLogData(flash_param_s.flash_index_s.log_s.offset, (const log_data_attr_t*)log_data);
    if(err_code == NRF_SUCCESS)
    {
        flash_param_s.flash_index_s.log_s.offset = (flash_param_s.flash_index_s.log_s.offset + 1)%256;
        if((flash_param_s.flash_index_s.log_s.offset%32) == 0)//128*32 =4096
        {
            Flash_CommonErase((uint32_t*)LOG_DATA_ADDR(flash_param_s.flash_index_s.log_s.offset*64), 1);
        }
    }
}

/************************************************************************
** Function name:        Api_ClearLogText
** Descriptions:         ���flash log ����
** input parameter:      ��
** Returned Value:       ��
** Created  by:          ����
** Created  Datas:       2017-08-04
**-----------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
**************************************************************************/
void Api_ClearLogText(void)
{
    Flash_CommonErase((uint32_t*)LOG_DATA_ADDR(0), 4);
}

/************************************************************************
** Function name:        Api_SaveDailyAttrData
** Descriptions:         �洢�ճ�����
** input parameter:      p_attr: �ճ�����ָ��   
** Returned Value:
** Created  by:          ����
** Created  Datas:       2017-08-04
**-----------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
**************************************************************************/
void Api_SaveDailyAttrData(const daily_data_attr_t* p_attr)
{
    uint32_t err_code = NRF_SUCCESS;
    flash_find_index_t* p_flash_index = &flash_param_s.flash_index_s;
    uint8_t current_block_index = p_flash_index->block_index%3;

    err_code = Record_AddDailyData(current_block_index, p_flash_index->daily.offset[current_block_index], p_attr);
    if(err_code == NRF_SUCCESS)
    {
        p_flash_index->daily.offset[current_block_index]++;
    }
}

/************************************************************************
** Function name:     Api_ReadDailyAttrData
** Descriptions:      ���ճ����� 
** input parameter:   day_id: ������                 package_off:ƫ���� 
**                    p_attr:�ճ�����ָ��
** Returned Value:    NRF_SUCCESS 
** Created  by:       ����
** Created  Datas:    2017-08-04
**-----------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
**************************************************************************/
//uint32_t Api_ReadDailyAttrData(uint8_t day_id, uint16_t package_offset, daily_data_attr_t* p_attr)
//{
//    Record_GetDailyData(day_id, package_offset, p_attr);
//    
//    return NRF_SUCCESS;
//}

/************************************************************************
** Function name:             Api_GetStepAllCount
** Descriptions:              �õ�����
** input parameter:           app_day_id: ������
** Returned Value:            NRF_SUCCESS
** Created  by:               ����
** Created  Datas:            2017-08-04
**-----------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
**************************************************************************/
uint32_t Api_GetStepAllCount(uint8_t app_day_id)
{
    uint8_t  local_day_id = (3 + flash_param_s.flash_index_s.block_index - app_day_id)%3;
    
    return flash_param_s.flash_index_s.daily.step_cnt[local_day_id];
}

void Api_SetSleepBlack(uint8_t black)
{
    sleep_cal_black = black%3;
}

/************************************************************************
** Function name:                               Api_SleepCallback
** Descriptions:       ˯�߻ص�����
** input parameter:    data:����ָ��
** Returned Value:     NRF_SUCCESS 
** Created  by:        ����
** Created  Datas:     2017-08-04
**************************************************************************/
//month(1 byte) 
//date(1 byte) 
//hour(1 byte) 
//miniute(1 byte) 
//steps(2 byte)
//movement(2 byte)
//pulse rate(1 byte)
//gesture(1 byte)
//sleep state 1(1 byte)
//sleep state 2(1 byte)
//sleep state 3(1 byte)
//sleep state 4(1 byte)
//sleep state 5(1 byte)
//sleep state 6(1 byte)
//NULL(position 17~20)
uint32_t Api_SleepCallback(uint8_t *data, uint16_t current_pkg, uint16_t pre_offset)
{
    uint8_t         data_buf_index = 0;
    uint8_t         day_id = sleep_cal_black % 3;   //flash_param_s.flash_index_s.block_index%3;
    uint16_t        offset = 0;
    uint32_t        err_code = NRF_SUCCESS;
    uint32_t        record_data[RECORD_DAILY_PER_DATA_SIZE / 4 + 1] = {0};
    daily_data_attr_t   *p_attr = (daily_data_attr_t *)record_data;

    //YX.20180103.������ʱ˯�ߴ�����Ϊ3������
    if(current_pkg == 0xFFFF)
    {
        current_pkg = 0;
    }
    
    if(current_pkg > pre_offset)
    {
        offset = (current_pkg - pre_offset - 1) % RECORD_DAILY_DATA_MAX_CNT;
    }
    else
    {
        day_id = (day_id+2)%3;
        if(flash_param_s.flash_index_s.daily.offset[day_id] > (pre_offset - current_pkg))
        {
            offset = (flash_param_s.flash_index_s.daily.offset[day_id] + current_pkg - pre_offset - 1) % RECORD_DAILY_DATA_MAX_CNT;
        }
        else
        {
            return NRF_ERROR_NOT_FOUND;
        }
    }
       
    ////err_code = Record_GetDailyData(day_id, offset, p_attr);
    uint32_t read_time_out = 0;
    while(Extflash_DailyReadSet(day_id, offset, (uint8_t *)p_attr, sizeof(daily_data_attr_t), 0) == 0)
    {
        vTaskDelay(1);
        read_time_out++;
        if(read_time_out > 1200)
        {   //��ʱ����������
            return NRF_ERROR_NOT_FOUND;
        }  
    }
    while(Extflash_DailyReadIsComplete() == 0)
    {
        vTaskDelay(1);
        read_time_out++;
        if(read_time_out > 1200)
        {   //��ʱ����������
            return NRF_ERROR_NOT_FOUND;
        }
    }
        
    {
    //���ݴ������
    data_buf_index = 0;
    memcpy(&data[data_buf_index], &p_attr->month, 4);
    data_buf_index += 4;
    
    data[data_buf_index++] = (p_attr->step >> 8) & 0xFF;
    data[data_buf_index++] = p_attr->step & 0xFF;
    data[data_buf_index++] = (p_attr->sport >> 8) & 0xFF;
    data[data_buf_index++] = p_attr->sport & 0xFF;
    data[data_buf_index++] = p_attr->wear;                          //��������ʣ�
    data[data_buf_index++] = p_attr->gesture;                       //��̬
    memcpy(&data[data_buf_index], &p_attr->sleep_state[0], 6);
    }
    
    if(err_code != NRF_SUCCESS)
    {
        return NRF_ERROR_NOT_FOUND;
    }
    //FLASH_LOG_PRINT("sleep callback Data:%d:%d %d %d %d %d", data[1], data[2], data[3], data[4], data[5], data[6]);
    
    return err_code;
}

/************************************************************************
** Function name:      Api_FlashDailyJumpBlock
** Descriptions:       �ճ���������
** input parameter:    ��
** Returned Value:     0
** Created  by:        ����
** Created  Datas:     2017-08-04
**-----------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
**************************************************************************/
uint32_t Api_FlashDailyJumpBlock(void)
{
    uint8_t current_day_id = flash_param_s.flash_index_s.block_index;
       
    // ��һ��:������һ������
    Record_ClearOneDayData((current_day_id + 1)%3);
	
    flash_param_s.flash_index_s.block_index = (flash_param_s.flash_index_s.block_index+1)%3;
	Record_DailyDataIndexInit(&flash_param_s.flash_index_s.daily);
    
    
    // �洢����ͳ�ƴ洢
    Record_AddPowerInfoData((current_day_id)%3, &p_data_manage->power_statistics_s);    
    memset(&p_data_manage->power_statistics_s, 0, sizeof(power_statistics_t));
    
    return 0;
}

/************************************************************************
** Function name:      Api_ReadAllFlashData
** Descriptions:       ������flash����
** input parameter:    block_id:������ offset:ƫ����  p_data:����ָ��
                       data to store in
** Returned Value:     NRF_SUCCESS
** Created  by:        ����
** Created  Datas:     2017-08-04
**-----------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
**************************************************************************/
uint32_t Api_ReadAllExtFlashData(uint8_t block_id, uint8_t offset, uint8_t* p_data)
{
    uint32_t read_time_out = 0;
    while(Extflash_AllOptReadSet(RECORD_EXT_FLASH_DATA_ADDR(block_id, offset*16), p_data, 16) == 0)
    {
        vTaskDelay(1);
        read_time_out++;
        if(read_time_out > 1200)
        {   //��ʱ����������
            return 1;
        }  
    }
    while(Extflash_AllOptReadIsComplete() == 0)
    {
        vTaskDelay(1);
        read_time_out++;
        if(read_time_out > 1200)
        {   //��ʱ����������
            return 1;
        }
    }
    
//    uint32_t flash_data[4] = {0};
    
//    Flash_CommonRead(flash_data, RECORD_FLASH_BLOCK_DATA_ADDR(block_id, offset*16), 16);
//    memcpy(p_data, flash_data, 16);
    
    return NRF_SUCCESS;
}

//uint32_t Api_ReadAllFlashData(uint8_t block_id, uint8_t offset, uint8_t* p_data)
//{
//    uint32_t flash_data[4] = {0};
//    
//    Flash_CommonRead(flash_data, RECORD_FLASH_BLOCK_DATA_ADDR(block_id, offset*16), 16);
//    memcpy(p_data, flash_data, 16);
//    
//    return NRF_SUCCESS;
//}

/************************************************************************
** Function name:            Api_FlashFatoryRestore
** Descriptions:             �������ݴ洢
** input parameter:          �� 
** Returned Value:           NRF_SUCCESS
** Created  by:              ����
** Created  Datas:           2017-08-04
**-----------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
**************************************************************************/
uint32_t Api_FlashFatoryRestore(void)
{
    CONFIG_LANGUAGE_E   temp_language_e = flash_param_s.language_e;
    static uint16_t            start_sector = 0;
    static uint16_t            sector_block = 0;
    
    // ���ϵͳ���ò���---Ƭ��flash
    Flash_CommonErase((uint32_t*)RECORD_FLASH_BLOCK_DATA_ADDR(0, 0), RECORD_FLASH_BLOCK_DATA_SIZE/4096);
    
    // ����ⲿflash���ݣ�˯��12K���ճ�108K,�˶�ģʽ24K,ECG�ֶ�����8k, ECG�Զ�����180K
    start_sector = RECORD_SLEEP_BASE_ADDR / (4096 * 8);
    sector_block = (ECG_EXT_FLASH_END_SIZE - RECORD_SLEEP_BASE_ADDR) / (4096 * 8);
    
    for(uint8_t i = start_sector; i < sector_block + 1; i++)
    {
        //Extflash_DataSectorErase4K(i);        
        // WJ.20190216.ʱ�����
        App_Watch_Dog_Reload();
        Extflash_DataBlockErase32K(start_sector + i);
    }
    
    Api_FlashParamInit();
    
    //WJ.20180630.���������ݺ����Է����ı��bug
    flash_param_s.language_e = temp_language_e;
    Api_AsynUpdateAppConfig();
    
    return NRF_SUCCESS;
}

/****************************************************************
** Function name:           Api_FlashClearUserData
** Descriptions:            ����û�����
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2018-11-21
*****************************************************************/ 
uint32_t Api_FlashClearUserData(void)
{
    ////Flash_CommonErase((uint32_t*)RECORD_FLASH_BLOCK_DATA_ADDR(0, 0), RECORD_FLASH_BLOCK_USER_DATA_SIZE/4096);
    static uint16_t start_sector = 0;
    static uint16_t sector_block = 0;
    
    start_sector = RECORD_SLEEP_BASE_ADDR / (4096 * 8);
    sector_block = (ECG_EXT_FLASH_END_SIZE-RECORD_SLEEP_BASE_ADDR) / (4096 * 8);
    
    for(uint8_t i = 0; i < sector_block + 1; i++)
    {
        //Extflash_DataSectorErase4K(start_sector + i);
        // WJ.20190216.ʱ�����
        App_Watch_Dog_Reload();
        Extflash_DataBlockErase32K(start_sector + i);
        
    }
    
    // �ճ�����FLASH�������Ҽ���				
    Record_LogDataIndexInit(&flash_param_s.flash_index_s.log_s);
    Record_DailyDataIndexInit(&flash_param_s.flash_index_s.daily);
    Record_BlockIndexInit(&flash_param_s.flash_index_s.block_index); 
    
    // WJ.20181105.����ecg�洢������ʼ��
    Record_EcgDataIndexInit(&flash_param_s.flash_index_s.ecg);     
        
    {   //WJ.20190105.ECG�Զ��������ݴ洢������ʼ��
        p_data_manage->ext_flash_ecg_opt_s.init = 1;
        Extflash_EcgErrDeal();
    }
    
    return NRF_SUCCESS;
}

/****************************************************************
** Function name:           Api_FlashClearUserConfig
** Descriptions:            ����û�����
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2018-11-21
*****************************************************************/ 
uint32_t Api_FlashClearUserConfig(void)
{
    CONFIG_LANGUAGE_E temp_language_e = flash_param_s.language_e;
    
    Flash_CommonErase((uint32_t*)SYSTEM_PARAM_BASE_ADDR, 1);
    Api_FlashParamInit();
    
    //WJ.20180630.���������ݺ����Է����ı��bug
    flash_param_s.language_e = temp_language_e;
    Api_AsynUpdateAppConfig();
    
    return NRF_SUCCESS;
}
    

/************************************************************************
** Function name:            Api_SaveSleepResultData
** Descriptions:             �洢˯������
** input parameter:          day_id:��������
                             p_data:����ָ�룬4�ֽڶ���,������̫�󣬲������¿��ٿռ䣻
                             data_len:4�ı���
** output parameter:         ��
** Returned Value:           NRF_SUCCESS
** Created  by:              ����
** Created  Datas:           2018-10-13
**************************************************************************/
uint32_t Api_SaveSleepResultData(uint8_t day_id, uint8_t *p_data, uint16_t data_len)
{
    uint8_t     sleep_wake_cnt = 0;
    uint32_t    sleep_offset_buf[SLEEP_ONE_DAY_MAX_TIMES + 1] = {0};     //ÿ��˯�ߵ�ƫ�Ƶ�ַ
    
    //���˲����������Ĵ洢
    if(day_id >= 3
      || p_data == NULL
      || (uint32_t)p_data & 0x03
      || data_len & 0x03)
    {
        return 1;
    }
    // ��ȡÿ��˯�ߵ�ƫ����
    Api_GetSleepStoreInfo(day_id, &sleep_wake_cnt, NULL, sleep_offset_buf);
    
    // WJ.20190116.���Ӵ洢�����ж�
    if(sleep_offset_buf[sleep_wake_cnt] + data_len > 4096)
    {   
        return 1;
    } 
    
    /////Flash_CommonStore((uint32_t const *)RECORD_SLEEP_DATA_ADDR(day_id, sleep_offset_buf[sleep_wake_cnt]), (uint32_t *)p_data, data_len/4);
   
   
    uint32_t read_time_out = 0;
    while(Extflash_SleepWriteSet(RECORD_SLEEP_DATA_ADDR(day_id, sleep_offset_buf[sleep_wake_cnt]), p_data, data_len) == 0)
    {
        vTaskDelay(1);
        read_time_out++;
        if(read_time_out > 1200)
        {   //��ʱ����������
            return 1;
        }  
    }
    while(Extflash_SleepWriteIsComplete() == 0)
    {
        vTaskDelay(1);
        read_time_out++;
        if(read_time_out > 1200)
        {   //��ʱ����������
            return 1;
        }
    }
    
    return NRF_SUCCESS;
}
 
static uint8_t Api_ReadSleepData(uint32_t offset, uint8_t *data, uint16_t data_len)
{
    uint32_t read_time_out = 0;
    while(Extflash_SleepReadSet(offset, data, data_len) == 0)
    {
        vTaskDelay(1);
        read_time_out++;
        if(read_time_out > 1200)
        {   //��ʱ����������
            return 1;
        }  
    }
    while(Extflash_SleepReadIsComplete() == 0)
    {
        vTaskDelay(1);
        read_time_out++;
        if(read_time_out > 1200)
        {   //��ʱ����������
            return 1;
        }
    }
    return 0;
}
    

/************************************************************************
** Function name:            Api_AlterSleepContinueFlag
** Descriptions:             �޸���һ�ε�˯�����ӱ�־
** input parameter:          day_id:��������
                             wake_cnt:˯�߶���
** output parameter:         ��
** Returned Value:           NRF_SUCCESS
** Created  by:              WJ
** Created  Datas:           2019-01-22
**************************************************************************/
uint32_t Api_AlterSleepContinueFlag(uint8_t day_id, uint8_t wake_cnt)
{
    uint8_t     sleep_wake_cnt = 0;
    uint32_t    sleep_offset_buf[SLEEP_ONE_DAY_MAX_TIMES + 1] = {0};     //ÿ��˯�ߵ�ƫ�Ƶ�ַ
    uint32_t    sleep_buf[2] = {0};
    uint8_t     *p_sleep_buf = (uint8_t *)&sleep_buf[0];

    // ��ȡÿ��˯�ߵ�ƫ����
    Api_GetSleepStoreInfo(day_id, &sleep_wake_cnt, NULL, sleep_offset_buf);
    
    if(sleep_wake_cnt < wake_cnt || sleep_wake_cnt < 2)
    {   //��һ��˯�߲����ڣ����ı�
        return 1;
    }
    
    ////Flash_CommonRead((uint32_t *)p_sleep_buf, RECORD_SLEEP_DATA_ADDR(day_id, sleep_offset_buf[sleep_wake_cnt - 1] - 8), 8);
    Api_ReadSleepData(RECORD_SLEEP_DATA_ADDR(day_id, sleep_offset_buf[sleep_wake_cnt - 1] - 8), p_sleep_buf, 8);
    
    for(uint8_t i = 0; i < 4; i++)
    {
        if(p_sleep_buf[4 - i] == 0xA6 && p_sleep_buf[4 - i + 1] == 0x02)
        {
            p_sleep_buf[4 - i + 3] = 1;
            break;
        }
    }
    
    ////Flash_CommonStore((uint32_t const *)RECORD_SLEEP_DATA_ADDR(day_id, sleep_offset_buf[sleep_wake_cnt - 1] - 8), (uint32_t *)p_sleep_buf, 8/4);
   
    uint32_t read_time_out = 0;
    while(Extflash_SleepWriteSet(RECORD_SLEEP_DATA_ADDR(day_id, sleep_offset_buf[sleep_wake_cnt - 1] - 8), p_sleep_buf, 8) == 0)
    {
        vTaskDelay(1);
        read_time_out++;
        if(read_time_out > 1200)
        {   //��ʱ����������
            return 1;
        }  
    }
    while(Extflash_SleepWriteIsComplete() == 0)
    {
        vTaskDelay(1);
        read_time_out++;
        if(read_time_out > 1200)
        {   //��ʱ����������
            return 1;
        }
    }
    
    return NRF_SUCCESS;
}
 
/************************************************************************
** Function name:            Api_GetSleepContinueNextFlag
** Descriptions:             ��ȡĳһ�ε�˯���������ӱ�־
** input parameter:          day_id:��������
                             wake_cnt:˯�߶���
** output parameter:         ��
** Returned Value:           NRF_SUCCESS
** Created  by:              WJ
** Created  Datas:           2019-01-22
**************************************************************************/
uint8_t Api_GetSleepContinueNextFlag(uint8_t day_id, uint8_t wake_cnt)
{
    uint8_t     sleep_wake_cnt = 0;
    uint32_t    sleep_offset_buf[SLEEP_ONE_DAY_MAX_TIMES + 1] = {0};     //ÿ��˯�ߵ�ƫ�Ƶ�ַ
    uint32_t    sleep_buf[2] = {0};
    uint8_t     *p_sleep_buf = (uint8_t *)&sleep_buf[0];
    
    // ��ȡÿ��˯�ߵ�ƫ����
    Api_GetSleepStoreInfo(day_id, &sleep_wake_cnt, NULL, sleep_offset_buf);
    
    if(wake_cnt > sleep_wake_cnt)
    {
        return 0;
    }
    
    ////Flash_CommonRead((uint32_t *)p_sleep_buf, RECORD_SLEEP_DATA_ADDR(day_id, sleep_offset_buf[wake_cnt] - 8), 8);    
    Api_ReadSleepData(RECORD_SLEEP_DATA_ADDR(day_id, sleep_offset_buf[wake_cnt] - 8), p_sleep_buf, 8);
    
    for(uint8_t i = 0; i < 4; i++)
    {
        if(p_sleep_buf[4 - i] == 0xA6 && p_sleep_buf[4 - i + 1] == 0x02)
        {
            if(p_sleep_buf[4 - i + 3] == 1)
            {
                return 1;
            }
            break;
        }
    }
    
    return 0;
}

/************************************************************************
** Function name:           Api_GetSleepStoreInfo
** Descriptions:            ��ȡ˯�ߴ洢��Ϣ
** input parameter:         day_id:�� ����
** output parameter:        sleep_num��˯�ߴ�����
                            sleep_size: ÿ��˯�����ݵĴ�С�����鳤��Ϊ5��
                            sleep_offset��ÿ��˯�ߵĵ�ַ�����鳤��Ϊ5��
** Returned Value:          ��
** Created  by:             ����
** Created  Datas:          2018-10-13
**************************************************************************/
void Api_GetSleepStoreInfo(uint8_t day_id, uint8_t *sleep_num, uint16_t *sleep_size, uint32_t *sleep_offset)
{
    uint8_t     sleep_wake_cnt  = 0;
	uint32_t    temp_offset = 0;
    __ALIGN(4) uint8_t  p_sleep_head[4] = {0};
    
    // !ע��:������Ҫ���쳣����
    if(day_id >= 3)
    {	    
        return ;
    }
    
    sleep_offset[0] = 0;
    // !ע��:��������ȡ5��˯��
    for(uint8_t index = 0; index < SLEEP_ONE_DAY_MAX_TIMES; index++)
    {
        ////Flash_CommonRead((uint32_t *)p_sleep_head, RECORD_SLEEP_DATA_ADDR(day_id, temp_offset), sizeof(p_sleep_head));
	    if(0 != Api_ReadSleepData(RECORD_SLEEP_DATA_ADDR(day_id, temp_offset), p_sleep_head, sizeof(p_sleep_head)))
        {//�쳣
            return ;
        }

        if(p_sleep_head[0] == 0xA1)
        {
            sleep_wake_cnt++;
        }
        else
        {
            break;
        }
        // ������һ��˯�ߵ���ʼ��ַ
        temp_offset += p_sleep_head[1] + (p_sleep_head[2] << 8);
        if(temp_offset % 4 != 0)
        {   //ע�����ʱ�Ѱ���4�ֽڶ���
            temp_offset = (temp_offset/4 + 1) * 4;
        }
        //��ȡÿ��˯�ߵ���ʼ��ַ
		//JSH.ԭ����������һ��
        if(sleep_offset != NULL && sleep_wake_cnt < SLEEP_ONE_DAY_MAX_TIMES)
        {
            sleep_offset[sleep_wake_cnt] = temp_offset;
        }
        //ÿ��˯�����ݵĴ�С
        if(sleep_size != NULL)
        {
            sleep_size[sleep_wake_cnt - 1] = p_sleep_head[1] + (p_sleep_head[2] << 8);
        }
    }
    if(sleep_num != NULL)
    {
        *sleep_num = sleep_wake_cnt;
    }
}

/************************************************************************
** Function name:           Api_GetSleepStoreInfo
** Descriptions:            ʧ�ߴ洢��Ϣ
** input parameter:         day_id:�� ����
** output parameter:        sleep_num��˯�ߴ�����
                            sleep_curve_len: ˯�������ܳ��ȣ�
** Returned Value:          ��
** Created  by:             ����
** Created  Datas:          2018-10-13
**************************************************************************/
void Api_GetSleepCurveLength(uint8_t day_id, uint8_t *sleep_num, uint16_t *sleep_curve_len)
{
    uint8_t     sleep_wake_cnt  = 0;                //˯�߶���
    uint8_t     sleep_curve_index = 0;              //˯�����߳����±�
	uint32_t    temp_offset = 0;                    //˯�߶���Ϣƫ�Ƴ���
	uint32_t    sleep_curve_offset = 0;             //���߳���ƫ�Ƴ��� 
    __ALIGN(4) uint8_t  p_sleep_head[4] = {0};      //ͷ����Ϣ����
    __ALIGN(4) uint8_t  read_data_buf[8] = {0};     //��ȡ˯��������Ϣ����
    
    
    // !ע��:������Ҫ���쳣����
    if(day_id >= 3 || sleep_curve_len == NULL)
    {	    
        return ;
    }
    
    sleep_curve_offset = SLEEP_CURVE_MIG_OFFSET;
    sleep_curve_index = sleep_curve_offset % 4 + 1;
    if(sleep_curve_offset & 0x03)   
    {   //����4�ֽڶ���
        sleep_curve_offset = sleep_curve_offset/4 * 4;
    }        
    
    sleep_curve_len = 0;
    
    // !ע��:��������ȡ5��˯��
    for(uint8_t index = 0; index < SLEEP_ONE_DAY_MAX_TIMES; index++)
    {
        ////Flash_CommonRead((uint32_t *)p_sleep_head, RECORD_SLEEP_DATA_ADDR(day_id, temp_offset), sizeof(p_sleep_head));
	    if(0 != Api_ReadSleepData(RECORD_SLEEP_DATA_ADDR(day_id, temp_offset), p_sleep_head, sizeof(p_sleep_head)))
        {//�쳣
            return ;
        }
        
        if(p_sleep_head[0] == 0xA1)
        {
            sleep_wake_cnt++;
        }
        else
        {
            break;
        }
        /////Flash_CommonRead((uint32_t *)read_data_buf, RECORD_SLEEP_DATA_ADDR(day_id, temp_offset + sleep_curve_offset), sizeof(read_data_buf));
        if(0 != Api_ReadSleepData(RECORD_SLEEP_DATA_ADDR(day_id, temp_offset + sleep_curve_offset), read_data_buf, sizeof(read_data_buf)))
        {//�쳣
            return ;
        }
        
        *sleep_curve_len += read_data_buf[sleep_curve_index] + (read_data_buf[sleep_curve_index + 1] << 8); 
        
        // ������һ��˯�ߵ���ʼ��ַ
        temp_offset += p_sleep_head[1] + (p_sleep_head[2] << 8);
        if(temp_offset % 4 != 0)
        {   //ע�����ʱ�Ѱ���4�ֽڶ���
            temp_offset = (temp_offset/4 + 1) * 4;
        }
    }
    if(sleep_num != NULL)
    {
        *sleep_num = sleep_wake_cnt;
    }
}

 
uint8_t  Api_GetSleepMapDayID(uint8_t block_id)
{
    uint8_t     sleep_map_id = 0;
    uint8_t     sleep_wake_cnt = 0;
    uint32_t    sleep_data[3] = {0};
    uint8_t     *p_sleep_data = (uint8_t*)sleep_data;
    uint8_t     diff_days = 0;
    uint32_t    sleep_offset_buf[SLEEP_ONE_DAY_MAX_TIMES + 1] = {0};     //ÿ��˯�ߵ�ƫ�Ƶ�ַ
    system_clock_time_t  sleep_entry_time = {0};
    
    // ��ȡǰһ��˯������
    sleep_entry_time.year = clock->year;
    Api_GetSleepStoreInfo((block_id+2)%3, &sleep_wake_cnt, NULL, sleep_offset_buf);
    
    for(uint8_t index = 0; index < sleep_wake_cnt; index++)
    {
        ////Flash_CommonRead(sleep_data, RECORD_SLEEP_DATA_ADDR((block_id+2)%3, sleep_offset_buf[index] + 12), sizeof(sleep_data));
        if(0 != Api_ReadSleepData(RECORD_SLEEP_DATA_ADDR((block_id+2)%3, sleep_offset_buf[index] + 12), (uint8_t *)sleep_data, sizeof(sleep_data)))
        {//�쳣
            return 0;
        }
        
        // ����˯������ʱ��
        sleep_entry_time.month = p_sleep_data[3];
        sleep_entry_time.day = p_sleep_data[4];
        diff_days = Get_DateDiffDays(&sleep_entry_time, clock);
        // �뵱ǰʱ����жԱ�(������Ҫ�ж�����ʱ��)
        if((diff_days <= 1) || (diff_days >= 360))
        {
            sleep_map_id = (block_id + 2)%3;
            break;
        }
    }

    return sleep_map_id;   
}

/****************************************************************
** Function name:           Sleep_GetTime
** Descriptions:            ��ȡ˯��ʱ�䣬������ʾ
** input parameters:        cur_data����ǰʱ�䣻old_data��ǰ�ε�ʱ��
** output parameters:       ��
** Returned value:          flase,true
** Created by:              WJ             
** Created Date:            2018-10-16
**----------------------------------------------------------------
*****************************************************************/ 
//static bool Sleep_Adjust(uint8_t *cur_data, uint8_t *old_data)
//{
//	bool days = false;
//	
//	int32_t days0 = Sclock_SeekYearDays(clock->year, cur_data[1], cur_data[2]);////���㵱ǰ��˯����˯ʱ����������
//	
//	int32_t days1 = Sclock_SeekYearDays(clock->year, cur_data[5], cur_data[6]);/////���㵱ǰ��˯������ʱ����������
//	
//	int32_t days2 = Sclock_SeekYearDays(clock->year, clock->month, clock->day);/////��ǰʱ����������
//	
//	int32_t days3 = Sclock_SeekYearDays(clock->year, old_data[5], old_data[6]);//////ǰһ��˯������ʱ����������
//	/*�ж���˯������ʱ���뵱ǰʱ���Ƿ�ӽ�*/
//	if(((days2 - days0 ==0 || (days2 - days0 ==1 && cur_data[3]>8) )/////��˯ʱ���뵱ǰʱ���п�������������8 
//      && days1 - days0 >=0 && days1 - days0 <=1 
//      && days2 - days1 >=0 && days2 - days1 <=1)
//      ||(cur_data[1] == 12 && cur_data[2] == 31
//      && clock->month ==1 && clock->day ==1))/////���ǿ�������
//	{
//        if(days3 == 0)
//        {
//            days = true;
//        }
//        else if(days3 <= days0)////�ڶ���˯�ߵ���˯����>=��һ��˯�ߵ���������
//        {
//            if(days0 - days3 == 0)
//            {
//                if(cur_data[3] * 60 + cur_data[4] > old_data[7] * 60 + old_data[8])/////�ж�˯���ص�
//                {
//                    if((old_data[7] < 8 && cur_data[3] <= 8)||(old_data[7] > 8 && cur_data[3] > 8))
//                     days = true;
//                }
//            }
//            else if(days0 - days3 == 1)
//            {
//                if(old_data[3] > 8 && cur_data[3] < 8)
//                {
//                     days = true;
//                }
//            }
//        }
//        else if(cur_data[1] == 1 && cur_data[2] == 1 && old_data[1] ==12 && old_data[2] ==31)
//        {
//            days = true;
//        }
//        if(days2 == days0)
//        {
//            if((clock->hour * 60 + clock->minutes) <= (cur_data[3] * 60 + cur_data[4]))
//            {
//                days = false;
//            }
//        }
//	}
//	return days;
//}

/************************************************************************
** Function name:            Api_GetSleepWakeCnt
** Descriptions:             ��ȡ˯��ʱ�䣬��ʾ�ڽ���
** input parameter:          �õ�˯��ʱ��
** Returned Value:           ˯����ʱ��(����) 
** Created  by:              WJ
** Created  Datas:           2018-10-16
**************************************************************************/
uint16_t Api_GetSleepTotalTimeNew(uint8_t *last_sleep_time, 
                               uint8_t *sleep_quality, 
                               uint16_t *sleep_shallow_time, 
                               uint16_t *sleep_deep_time);
                               
uint16_t Api_GetSleepTotalTime(uint8_t *sleep_quality, 
                               uint16_t *sleep_shallow_time, 
                               uint16_t *sleep_deep_time)
{
    return Api_GetSleepTotalTimeNew(NULL, sleep_quality, sleep_shallow_time, sleep_deep_time);  
}

//��ʾ�����˯��ʱ�䣬���˵�����ʱ���˯��
//cur_time:��˯������ʱ��+�˳�������ʱ��
//last_time:��һ�κϷ����˳�˯��ʱ��-����ʱ��
//yesterday_time:�����-����
//system_time:ϵͳʱ��-����ʱ��
//per_index:0,���죻1������
static bool Sleep_TimeCmp(uint8_t *cur_time, uint8_t *last_time, uint8_t *yesterday_time,
                          uint8_t *sys_time, uint8_t per_index)
{
	bool cmp_flag = false;
	
    //���ڱȽϴ洢�������˯������
    if(per_index == 1)
    {
        //�Ƚ�ÿ��˯�ߵ�ʱ�䣬��˯ʱ��>����20�㣬�����˳�˯��ʱ��Ϊ���죬���������ʾʱ
        if(cur_time[0] == yesterday_time[0] && cur_time[1] == yesterday_time[1])
        {   //�Ǵ洢�������˯������,����˯ʱ��������
            if((cur_time[2]*60 + cur_time[3]) > 20*60 ) //20���Ժ���˯
            {
                cmp_flag = true;
            }
            else
            {   //20����ǰ��˯
                if(cur_time[5] != yesterday_time[1])    //�˳�˯���ڵڶ��죬<<ϵͳʱ�䵱��>>
                {
                    cmp_flag = true;
                }
            }
        }
        else if(cur_time[0] == sys_time[0] && cur_time[1] == sys_time[1])
        {   //�Ǵ洢�������˯������,����˯ʱ���ڽ���
            if((cur_time[2]*60 + cur_time[3]) < 8*60 ) //8����ǰ��˯
            {
                cmp_flag = true;
            }          
        }
    }
    
    //���ڱȽϴ洢�ڽ����˯������
    if(per_index == 0)
    {
        //�Ƚ�ÿ��˯�ߵ�ʱ�䣬��˯ʱ��<20����>=8�㣬���˳�˯��ʱ��Ϊ���죬���������ʾʱ��
        if(cur_time[0] == sys_time[0] && cur_time[1] == sys_time[1] 
           && (cur_time[2] >= 8 && (cur_time[2]*60 + cur_time[3]) <= 20*60))
        {
            if((cur_time[6]*60 + cur_time[7]) < 24*60)
            {   //�˳�˯���ڽ���
                cmp_flag = true;
            }
        }
    }
    
    return cmp_flag;
}

/************************************************************************
** Function name:            Api_GetSleepTotalTimeNew
** Descriptions:             ��ȡ˯��ʱ�䣬��ʾ�ڽ���
** input parameter:          �õ�˯��ʱ��
                             last_sleep_time:�˳�˯�ߵ����һ���ʱ��
** Returned Value:           ˯����ʱ��(����) 
** Created  by:              WJ
** Created  Datas:           2018-10-16
**************************************************************************/
uint16_t Api_GetSleepTotalTimeNew(uint8_t *last_sleep_time, 
                                  uint8_t *sleep_quality,
                                  uint16_t *sleep_shallow_time, 
                                  uint16_t *sleep_deep_time)
{
    uint8_t     sleep_wake_cnt = 0;
    uint8_t     block_index = 0;
    uint8_t     max_sleep_quality = 0;      // ˯������
    uint16_t    sleep_total_time = 0;       // ˯����ʱ��
    uint16_t    c_sleep_shallow_time = 0;   // ǳ˯ʱ��
    uint16_t    c_sleep_deep_time = 0;      // ��˯ʱ��
    uint32_t    sleep_data[5] = {0};
    uint8_t     *p_sleep_data = (uint8_t*)sleep_data;
    uint8_t     sleep_new_time[12] = {0x00, };     
    uint8_t     sleep_old_time[12] = {0x00, }; 
    uint32_t    sleep_offset_buf[SLEEP_ONE_DAY_MAX_TIMES + 1] = {0};     //ÿ��˯�ߵ�ƫ�Ƶ�ַ

    uint8_t     sys_time[4] = {0};
    uint8_t     yesterday_time[4] = {0};
    
    
    //��ȡ�����ʱ��
    sys_time[0] = clock->year & 0xFF;    
    sys_time[1] = (clock->year>>8) & 0xFF;
    sys_time[2] = clock->month & 0xFF;
    sys_time[3] = clock->day & 0xFF;
    Util_GetPreviousDayDate(sys_time, 1, yesterday_time);
    
    //��ȡ����洢˯�ߵĴ洢��
    block_index = (p_flash_param->flash_index_s.block_index + 2)%3;
    //��ȡ����洢ÿ��˯��ʱ���˯��ʱ��
    //�Ƚ�ÿ��˯�ߵ�ʱ�䣬��˯ʱ��>����20�㣬�����˳�˯��ʱ��Ϊ���죬���������ʾʱ
    Api_GetSleepStoreInfo(block_index, &sleep_wake_cnt, NULL, sleep_offset_buf);    
    for(uint8_t index = 0; index < sleep_wake_cnt; index++)
    {
        memset(p_sleep_data, 0x00 , sizeof(sleep_data));
        ////Flash_CommonRead(sleep_data, RECORD_SLEEP_DATA_ADDR(block_index, sleep_offset_buf[index] + 12), sizeof(sleep_data));
        if(0 != Api_ReadSleepData(RECORD_SLEEP_DATA_ADDR(block_index, sleep_offset_buf[index] + 12), (uint8_t *)sleep_data, sizeof(sleep_data)))
        {//�쳣
            return 0;
        }
        
        memcpy(sleep_new_time, (void *)&p_sleep_data[3], 8);
        if(Sleep_TimeCmp(sleep_new_time, sleep_old_time, &yesterday_time[2], &sys_time[2], 1))
        {
            memcpy(sleep_old_time, (void *)&p_sleep_data[7], 4);
            //��ȡ˯��ʱ������ʱ��ƫ����40    
            if(Api_GetSleepContinueNextFlag(block_index, index + 1) != 1)
            {
                ////Flash_CommonRead(sleep_data, RECORD_SLEEP_DATA_ADDR(block_index, sleep_offset_buf[index] + 40), sizeof(sleep_data));   
                if(0 != Api_ReadSleepData(RECORD_SLEEP_DATA_ADDR(block_index, sleep_offset_buf[index] + 28), (uint8_t *)sleep_data, sizeof(sleep_data)))
                {//�쳣
                    return 0;
                }    
                sleep_total_time += p_sleep_data[12] + (p_sleep_data[13] << 8);
                c_sleep_shallow_time += p_sleep_data[8] + (p_sleep_data[9] << 8);   // ǳ˯ʱ��
                c_sleep_deep_time += p_sleep_data[6] + (p_sleep_data[7] << 8);      // ��˯ʱ��
                
                if(p_sleep_data[2] > max_sleep_quality)
                {
                    max_sleep_quality = p_sleep_data[2];
                }
            }
        }
        if(index == 0)
        {
            memcpy(sleep_old_time, (void *)&p_sleep_data[7], 4);  
        }
        if(last_sleep_time !=NULL)
        {
            memcpy(last_sleep_time, &sleep_new_time[4], 4);
        }
    }
    
    //��ȡ�����˯�ߴ洢��
    block_index = p_flash_param->flash_index_s.block_index%3;  
    //�Ƚ�ÿ��˯�ߵ�ʱ�䣬��˯ʱ��<20�㣬���˳�˯��ʱ��Ϊ���죬���������ʾʱ��    
    Api_GetSleepStoreInfo(block_index, &sleep_wake_cnt, NULL, sleep_offset_buf);    
    for(uint8_t index = 0; index < sleep_wake_cnt; index++)
    {
        memset(p_sleep_data, 0x00 , sizeof(sleep_data));
        /////Flash_CommonRead(sleep_data, RECORD_SLEEP_DATA_ADDR(block_index, sleep_offset_buf[index] + 12), sizeof(sleep_data));
        if(0 != Api_ReadSleepData(RECORD_SLEEP_DATA_ADDR(block_index, sleep_offset_buf[index] + 12), (uint8_t *)sleep_data, sizeof(sleep_data)))
        {//�쳣
            return 0;
        }
            
        memcpy(sleep_new_time, (void *)&p_sleep_data[3], 8);
        
        if(Sleep_TimeCmp(sleep_new_time, sleep_old_time, &yesterday_time[2], &sys_time[2], 0))
        {
            memcpy(sleep_old_time, (void *)&p_sleep_data[7], 4);
            //��ȡ˯��ʱ������ʱ��ƫ����40   

            // �жϵ�ǰ��˯���Ƿ�������
            if(Api_GetSleepContinueNextFlag(block_index, index + 1) != 1)
            {            
                ////Flash_CommonRead(sleep_data, RECORD_SLEEP_DATA_ADDR(block_index, sleep_offset_buf[index] + 40), sizeof(sleep_data));            
                if(0 != Api_ReadSleepData(RECORD_SLEEP_DATA_ADDR(block_index, sleep_offset_buf[index] + 28), (uint8_t *)sleep_data, sizeof(sleep_data)))
                {//�쳣
                    return 0;
                }
                sleep_total_time += p_sleep_data[12] + (p_sleep_data[13] << 8);
                c_sleep_shallow_time += p_sleep_data[8] + (p_sleep_data[9] << 8);   // ǳ˯ʱ��
                c_sleep_deep_time += p_sleep_data[6] + (p_sleep_data[7] << 8);      // ��˯ʱ��
                
                if(p_sleep_data[2] > max_sleep_quality)
                {
                    max_sleep_quality = p_sleep_data[2];
                }
            }
        }
        if(index == 0)
        {
            memcpy(sleep_old_time, (void *)&p_sleep_data[7], 4);  
        }
        if(last_sleep_time !=NULL)
        {
            memcpy(last_sleep_time, &sleep_new_time[4], 4);
        }
    }
    
    if(sleep_quality != NULL)
    {
        *sleep_quality = max_sleep_quality;
    }
    
    if(sleep_shallow_time != NULL)
    {
        *sleep_shallow_time = c_sleep_shallow_time;
    }
    
    if(sleep_deep_time != NULL)
    {
        *sleep_deep_time = c_sleep_deep_time;
    }

    return sleep_total_time;    
}

/************************************************************************
** Function name:            Api_GetSleepPartData
** Descriptions:             ����ƫ������ȡ˯������
** input parameter:          day_id:��������ݣ�offset:ƫ����;data_len:��ȡ���ݳ���
** output parameter:         p_data����ȡ��������
** Returned Value:           NRF_SUCCESS
** Created  by:              WJ
** Created  Datas:           2018-10-15
**************************************************************************/
uint32_t Api_GetSleepPartData(uint8_t day_id, uint32_t offset, uint8_t* p_data, uint16_t data_len)
{
    uint32_t part_data[4] = {0};
    
    if(day_id < 3)
    {
        ////Flash_CommonRead(part_data, RECORD_SLEEP_DATA_ADDR(day_id, offset), sizeof(part_data));
        if(0 != Api_ReadSleepData(RECORD_SLEEP_DATA_ADDR(day_id, offset), (uint8_t *)part_data, sizeof(part_data)))
        {//�쳣
            return 0;
        }
        
        memcpy(p_data, part_data, data_len);
    }
    return NRF_SUCCESS;
}

/************************************************************************
** Function name:        Api_SaveSystemTime
** Descriptions:         �洢ϵͳʱ��
** input parameter:      ��������   
** Returned Value:       NRF_SUCCESS
** Created  by:          ����
** Created  Datas:       2017-08-04
**-----------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
**************************************************************************/
uint32_t Api_SaveSystemTime(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute)
{
    uint32_t system_time[2] = {0};
    uint8_t*  p_system_time = (uint8_t*)system_time;
    // 1���ж�ʱ��ģ���Ƿ�д�� 
    if(flash_param_s.systemtime_index >= 512)
    {
        flash_param_s.systemtime_index = 0;
        //������FLASH��
        Flash_CommonErase((uint32_t*)(SYSTEM_TIME_BASE_ADDR), 1);
    }
       
    p_system_time[0] = (year>>8)&0xFF;
    p_system_time[1] = year&0xFF;
    p_system_time[2] = month;
    p_system_time[3] = day;
    p_system_time[4] = hour;
    p_system_time[5] = minute;
    p_system_time[6] = (flash_param_s.systemtime_index>>8)&0xFF;;
    p_system_time[7] = flash_param_s.systemtime_index&0xFF;;
    Flash_CommonStore((uint32_t*)(SYSTEM_TIME_BASE_ADDR+flash_param_s.systemtime_index*8), system_time, 2);
    flash_param_s.systemtime_index++;
    return NRF_SUCCESS;
} 

/************************************************************************
** Function name:           Api_ReadSystemTimeData
** Descriptions:            ��ϵͳʱ��  
** input parameter:         pdata:�������ݼ�¼   
** Returned Value:          NRF_SUCCESS
** Created  by:             ����
** Created  Datas:          2017-08-04
**-----------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
**************************************************************************/
uint32_t Api_ReadSystemTimeData(uint8_t* pdata)
{
    uint32_t system_time[2] = {0};
    for(uint16_t index = 0; index < 512; index++)
    {
        Flash_CommonRead(system_time, SYSTEM_TIME_BASE_ADDR+index*8, sizeof(system_time));
        if((0xFFFFFFFF == system_time[0])&&(0xFFFFFFFF == system_time[1]))
        {
            if(0 == index)
            {
                 return NRF_ERROR_NOT_FOUND;
            }
            else
            {
                Flash_CommonRead(system_time, SYSTEM_TIME_BASE_ADDR+(index-1)*8, sizeof(system_time));
                memcpy(pdata, system_time, sizeof(system_time));
                break;
            }
        }
    }
    
    return NRF_SUCCESS;
}

/************************************************************************
** Function name:        Api_UpdateHeartAlertParam
** Descriptions:         �������ʱ�������  
** input parameter:      p_alert�����ʲ���  
** Returned Value:       NRF_SUCCESS
** Created  by:          ����
** Created  Datas:       2017-08-04
**-----------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
**************************************************************************/
uint32_t Api_UpdateHeartAlertParam(const heart_alert_config_t* p_alert)
{
    //�����ڴ�����
    flash_param_s.heart_alert_s.enable = p_alert->enable;
    flash_param_s.heart_alert_s.high = p_alert->high;
    flash_param_s.heart_alert_s.low = p_alert->low;
	
    BleThread_AsynUpdateAppConfig();
    
    return NRF_SUCCESS;
}

/************************************************************************
** Function name:           Api_UpdateTurnWristParam
** Descriptions:            ����ת�������  
** input parameter:         P_turnwrist:����ָ��     
** Returned Value:          NRF_SUCCESS
** Created  by:             ����
** Created  Datas:          2017-08-04
**-----------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
**************************************************************************/
uint32_t Api_UpdateTurnWristParam(const turnwrist_config_t* p_turnwrist)
{
    memcpy(&flash_param_s.turnwrist_s, p_turnwrist, sizeof(turnwrist_config_t));
    
    BleThread_AsynUpdateAppConfig();
    
    return NRF_SUCCESS;
}

/************************************************************************
** Function name:          Api_UpdateBloodConfig
** Descriptions:           ����Ѫ������   
** input parameter:        P_config:����ָ��    
** Returned Value:         NRF_SUCCESS
** Created  by:            ����
** Created  Datas:         2017-08-04
**-----------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
**************************************************************************/
uint32_t Api_UpdateBloodConfig(const bp_config_t* p_config)
{
    flash_param_s.bp_s.mode = p_config->mode;
    flash_param_s.bp_s.private_high = p_config->private_high;
    flash_param_s.bp_s.private_low = p_config->private_low;
	
    BleThread_AsynUpdateAppConfig();
    
    return NRF_SUCCESS;
}

/************************************************************************
** Function name:          Api_UpdateMessageEnableConfig
** Descriptions:           ������Ϣ����  
** input parameter:        p_enable:����ָ��     
** Returned Value:         NRF_SUCCESS
** Created  by:            ����
** Created  Datas:         2017-08-04
**-----------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
**************************************************************************/
uint32_t Api_UpdateMessageEnableConfig(const app_message_enable_t *p_enable)
{
    memcpy(&flash_param_s.message_enable_s, p_enable, sizeof(app_message_enable_t));
	
    BleThread_AsynUpdateAppConfig();
    
    return NRF_SUCCESS;
}

/************************************************************************
** Function name:          Api_GetBloodMode
** Descriptions:           ѪҺģʽ  
** input parameter:        ��
** Returned Value:         ģʽ
** Created  by:            ����
** Created  Datas:         2017-08-04
**-----------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
**************************************************************************/
BLOOD_MODE_E Api_GetBloodMode(void)
{
    return flash_param_s.bp_s.mode;
}

/************************************************************************
** Function name:          Api_UpdateDevConfig
** Descriptions:           �����豸����
** input parameter:        p_dev_config_t: �豸���� 
** Returned Value:         NRF_SUCCESS
** Created  by:            ����
** Created  Datas:         2017-08-04
**-----------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
**************************************************************************/
uint32_t Api_UpdateDevConfig(const dev_config_t* p_dev_config_t)
{
    memcpy(&flash_param_s.dev_config_s, p_dev_config_t, sizeof(dev_config_t));
	
    BleThread_AsynUpdateAppConfig();
    
    return NRF_SUCCESS;
}

/************************************************************************
** Function name:          Api_UpdateSedentaryConfig
** Descriptions:           ���¾������ò���  
** input parameter:        P_sedentary_config_t:��������ָ��
** Returned Value:         NRF_SUCCESS
** Created  by:            ����
** Created  Datas:         2017-08-04
**-----------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
**************************************************************************/
uint32_t Api_UpdateSedentaryConfig(const sedentary_config_t* p_sedentary_config_t)
{
    memcpy(&flash_param_s.sedentary_config_s, p_sedentary_config_t, sizeof(sedentary_config_t));
	
    BleThread_AsynUpdateAppConfig();
    
    return NRF_SUCCESS;
}

/************************************************************************
** Function name:          Api_UpdateGuiLanguage
** Descriptions:             
** input parameter:        ����
** Returned Value:         NRF_SUCCESS
** Created  by:            GT
** Created  Datas:         2017-08-04
**-----------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
**************************************************************************/
uint32_t Api_UpdateGuiLanguage(CONFIG_LANGUAGE_E language)
{
    flash_param_s.language_e = language;
        
    return App_UpdateGuiLanguage(language);
}
/************************************************************************
** Function name:          Api_UpdateConnectPassword
** Descriptions:           ��������  
** input parameter:        password:����
** Returned Value:         NRF_SUCCESS
** Created  by:            ����
** Created  Datas:         2017-08-04
**-----------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
**************************************************************************/
uint32_t Api_UpdateConnectPassword(uint16_t password)
{
    flash_param_s.password = password;
	
    BleThread_AsynUpdateAppConfig();
    
    return NRF_SUCCESS;
}

/************************************************************************
** Function name:          Api_UpdateAlarmTime
** Descriptions:           index:��������  p_alarm:���Ӳ���ָ��   
** input parameter:        password:����
** Returned Value:         NRF_SUCCESS
** Created  by:            ����
** Created  Datas:         2017-08-04
**-----------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
**************************************************************************/
uint32_t Api_UpdateAlarmTime(uint8_t index, const alarm_time_config_t* p_alarm)
{
    memcpy(&flash_param_s.alarm_time_s[index%20], p_alarm, sizeof(alarm_time_config_t));
	
    BleThread_AsynUpdateAppConfig();
    
    return NRF_SUCCESS;
}

/************************************************************************
** Function name:          Api_GetAlarmCRC
** Descriptions:           index:��������  p_alarm:���Ӳ���ָ��  
** input parameter:        password:����
** Returned Value:         ���Ӳ���
** Created  by:            ����
** Created  Datas:         2017-08-04
**-----------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
**************************************************************************/
uint16_t Api_GetAlarmCRC(void)
{
    uint16_t alarm_crc = 0xFFFF;
    uint8_t  alarm_data[10] = {0};
    const alarm_time_config_t* p_current_alarm = NULL;
    for(uint8_t index = 0; index < 20; index++)
    {
        p_current_alarm = &p_flash_param->alarm_time_s[index];
        if(1 == p_current_alarm->valid)
        {
            alarm_data[0] = index+1;
            alarm_data[1] = p_current_alarm->hour;
            alarm_data[2] = p_current_alarm->minute;
            alarm_data[3] = p_current_alarm->state;
            alarm_data[4] = p_current_alarm->repeat;
            alarm_data[5] = p_current_alarm->scence;
            alarm_data[6] = p_current_alarm->year&0xFF;
            alarm_data[7] = (p_current_alarm->year&0xFF00)>>8;
            alarm_data[8] = p_current_alarm->month;
            alarm_data[9] = p_current_alarm->day;
            alarm_crc = Crc16(alarm_crc, alarm_data, sizeof(alarm_data));
          }
    }
    return alarm_crc;
}

/************************************************************************
** Function name:          Api_UpdateBrightness
** Descriptions:           ������������  
** input parameter:        p_bright:��������ָ��
** Returned Value:         NRF_SUCCESS
** Created  by:            ����
** Created  Datas:         2017-08-04
**-----------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
**************************************************************************/
uint32_t Api_UpdateBrightness(brightness_config_t* p_bright)
{
    memcpy(&flash_param_s.brightness_s, p_bright, sizeof(brightness_config_t));
	
    BleThread_AsynUpdateAppConfig();
    
    return NRF_SUCCESS;
}

/************************************************************************
** Function name:          Api_UpdateUserInfomation
** Descriptions:           �����û���Ϣ 
** input parameter:        p_person:������Ϣ����ָ��
** Returned Value:         NRF_SUCCESS
** Created  by:            GT
** Created  Datas:         2017-08-04
**-----------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
**************************************************************************/
uint32_t Api_UpdateUserInfomation(const person_infomation_t* p_person, uint16_t target)
{
    memcpy(&flash_param_s.person_s, p_person, sizeof(person_infomation_t));
    if(flash_param_s.sport_target != target)
    {
        flash_param_s.Remind_flag = 0;//�����趨Ŀ�꣬������
    }
	
	flash_param_s.sport_target = target;
    ////App_UpdatePersonInfomation(p_person);
    
    BleThread_AsynUpdateAppConfig();
    
    return NRF_SUCCESS;
}

uint32_t Api_UpdateHourglassParam(const hourglass_config_t *p_hourglass_config)
{
    memcpy(&flash_param_s.hourglass_s, p_hourglass_config, sizeof(hourglass_config_t));
 
    BleThread_AsynUpdateAppConfig();
    
    return NRF_SUCCESS;
}

/************************************************************************
** Function name:          Api_UpdateBloodOxygenParam
** Descriptions:           ����Ѫ����������  
** input parameter:        p_param:����ָ��
** Returned Value:         NRF_SUCCESS
** Created  by:            ����
** Created  Datas:         2017-08-04
**-----------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
**************************************************************************/
uint32_t Api_UpdateBloodOxygenParam(const time_range_config_t* p_param)
{
    memcpy(&flash_param_s.blood_oxygen_config_s, p_param, sizeof(time_range_config_t));
	
    BleThread_AsynUpdateAppConfig();
    
    return NRF_SUCCESS;   
}

/************************************************************************
** Function name:          Api_GetDevSN
** Descriptions:           �õ��豸SN  
** input parameter:        ��
** Returned Value:         �豸id
** Created  by:            ����
** Created  Datas:         2017-08-04
**-----------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
**************************************************************************/
uint32_t Api_GetDevSN(void)
{
    return Dev_GetDevID(&flash_param_s.product_infomation_s);
}

/************************************************************************
** Function name:          Api_DevSNCheck
** Descriptions:           �豸sn���  
** input parameter:        ��
** Returned Value:         ��
** Created  by:            ����
** Created  Datas:         2017-08-04
**-----------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
**************************************************************************/
void Api_DevSNCheck(void)
{
    //WJ.20190103.�޸�λ�ú󣬼��ǰ�����ȡ
    Dev_GetProductionInfomation(&flash_param_s.product_infomation_s);
 
    Dev_SNCheck(&flash_param_s.product_infomation_s);	
}

//void Api_UpdateBluetoothName(const bluetooth_name_t* p_bluetooth_name)
//{
//    memcpy(&flash_param_s.bluetooth_name_s, p_bluetooth_name, sizeof(bluetooth_name_t)); 

//    BleThread_AsynUpdateAppConfig();    
//}

/************************************************************************
** Function name:          Api_UpdateUiStyle
** Descriptions:           ������ʽ  
** input parameter:        ui_style:��ʽ
** Returned Value:         NRF_SUCCESS
** Created  by:            ����
** Created  Datas:         2017-08-04
**-----------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
**************************************************************************/
uint32_t Api_UpdateUiStyle(uint8_t ui_style)
{
    flash_param_s.ui_style = ui_style;
	
    BleThread_AsynUpdateAppConfig();
    
    return NRF_SUCCESS;
}

/************************************************************************
** Function name:          Api_GetUiStyle
** Descriptions:           ��ȡ�������� 
** input parameter:        ��
** Returned Value:         �����ʽ
** Created  by:            YX
** Created  Datas:         2017-12-28
**-----------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
**************************************************************************/
uint8_t Api_GetUiStyle(void)
{
    return flash_param_s.ui_style;
}

/*******************************************************************************************************
** Function name:          Api_UpdateUiDisplayTime
** Descriptions:           ����UI��ʾ����ʱ��
** input parameter:        ui_display_time:UI��ʾ����ʱ��
** Returned Value:         NRF_SUCCESS
** Created  by:            WJ
** Created  Datas:         2018-08-21
*********************************************************************************************************/
uint32_t Api_UpdateUiDisplayTime(const uint8_t ui_display_time)
{
    flash_param_s.ui_display_time = ui_display_time;
    
    BleThread_AsynUpdateAppConfig();
    
    return NRF_SUCCESS;
}

/*******************************************************************************************************
** Function name:          Api_GetUiDisplayTime
** Descriptions:           ��ȡUI��ʾ����ʱ��
** input parameter:        ui_display_time:UI��ʾ����ʱ��
** Returned Value:         ��ʾʱ��
** Created  by:            WJ
** Created  Datas:         2018-08-21
*********************************************************************************************************/
uint8_t Api_GetUiDisplayTime(void)
{
    if(flash_param_s.ui_display_time < UI_DISPLAY_TIME_MIN
      || flash_param_s.ui_display_time > UI_DISPLAY_TIME_MAX)
    {
        return UI_DISPLAY_TIME_RECOMMEND;
    }
    return flash_param_s.ui_display_time;
}

/************************************************************************
** Function name:        Api_SaveEcgAttrData
** Descriptions:         �洢 ECG�ֶ������������
** input parameter:      p_ecg_data: ECG�ֶ������������   
** Returned Value:       ��
** Created  by:          WJ
** Created  Datas:       2018-11-01
**************************************************************************/
void Api_SaveEcgAttrData(const ecg_result_data_t *p_ecg_data)
{
    uint8_t             offset_index = 0;
    flash_find_index_t  *p_flash_index = &flash_param_s.flash_index_s;
    uint32_t            err_code = NRF_SUCCESS;  
    
    offset_index = p_flash_index->ecg.offset_end_index;

    err_code = Record_AddEcgResultData(offset_index, p_ecg_data);
    if(err_code == NRF_SUCCESS)
    {
        if(p_flash_index->ecg.offset_end_index < RECORD_ECG_ONE_BLOCK_MAX_CNT && p_flash_index->ecg.offset_start_index == 0)
        {
        }
        else
        {
            p_flash_index->ecg.offset_start_index = (++p_flash_index->ecg.offset_start_index) % RECORD_ECG_MAX_CNT;
        }
        p_flash_index->ecg.offset_end_index = (++p_flash_index->ecg.offset_end_index) % RECORD_ECG_MAX_CNT;
        if(p_flash_index->ecg.offset_end_index == 0) 
        {   //������һ��
            Record_ClearEcgResultBlock(0, 1);
            p_flash_index->ecg.offset_start_index = RECORD_ECG_ONE_BLOCK_MAX_CNT + 1;
        }
        else if(p_flash_index->ecg.offset_end_index == RECORD_ECG_ONE_BLOCK_MAX_CNT)
        {   //�����ڶ���
            Record_ClearEcgResultBlock(1, 1); 
            p_flash_index->ecg.offset_start_index = 0;
        }
    }
}

/************************************************************************
** Function name:        Api_GetEcgAttrData
** Descriptions:         ��ȡECG�ֶ������������
** input parameter:      offset: ����ƫ����   
** output parameter:     p_ecg_data: ECG�ֶ������������   
** Returned Value:       ��
** Created  by:          WJ
** Created  Datas:       2018-11-01
**************************************************************************/
void Api_GetEcgAttrData(uint16_t offset, ecg_result_data_t *p_ecg_data)
{  
    uint32_t read_time_out = 0;
    while(Extflash_EcgRestReadSet(offset, (uint8_t *)p_ecg_data, sizeof(ecg_result_data_t)) == 0)
    {
        vTaskDelay(1);
        read_time_out++;
        if(read_time_out > 1200)
        {   //��ʱ����������
            return;
        }  
    }
    while(Extflash_EcgRestReadIsComplete() == 0)
    {
        vTaskDelay(1);
        read_time_out++;
        if(read_time_out > 1200)
        {   //��ʱ����������
            return;
        }
    }
}

