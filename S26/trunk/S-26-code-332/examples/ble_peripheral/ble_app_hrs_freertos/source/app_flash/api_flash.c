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

/*******************************************************************************************************
** Function name:        Crc16
** Descriptions:         crc У��  
** input parameter:      crc:У��ֵ  data:��ҪУ��ֵ   
** Returned Value:       crc
** Created  by:          ����
** Created  Datas:       2017-08-04
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
*********************************************************************************************************/
/*static __inline */uint16_t Crc16(uint16_t crc, uint8_t *data,uint16_t len) 
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

/*******************************************************************************************************
** Function name:      Api_FlashParamInit
** Descriptions:       flash ������ʼ�� 
** input parameter:    ��   
** Returned Value:     ��
** Created  by:        ����
** Created  Datas:     2017-08-04
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
*********************************************************************************************************/
static void Api_FlashParamInit(void)
{
    memset(&flash_param_s, 0, sizeof(flash_param_s));
	   
    //WJ.20181105.�������ƵĴ洢λ���޸ĵ�������Ϣ��
    //App_GetBluetoothName(&flash_param_s.bluetooth_name_s);
    Dev_GetProductionInfomation(&flash_param_s.product_infomation_s);
    // WJ.20181105.��ȡ��������
    Dev_CheckBleAdvName();       
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
    //��ʼ������ʱ��
    for(uint8_t index = 0; index < 20; index++)
    {
        Record_GetAlarmTimeParam(index, &flash_param_s.alarm_time_s[index]);
    }
    
    App_GetHourglassParam(&flash_param_s.hourglass_s);
    App_GetBloodOxygenParam(&flash_param_s.blood_oxygen_config_s);
	
	App_GetUiStyleParam(&flash_param_s.ui_style);
    App_GetMoveMaxPuseTimeParam(&flash_param_s.movement_max_puse_time);
    
    // �ճ�����FLASH�������Ҽ���				
    //Record_LogDataIndexInit(&flash_param_s.flash_index_s.log_s);
    Record_DailyDataIndexInit(&flash_param_s.flash_index_s.daily);
    Record_OxygenDataIndexInit(&flash_param_s.flash_index_s.oxygen);
    Record_BlockIndexInit(&flash_param_s.flash_index_s.block_index); 
    //Api_ClearLogText();    
}

/*******************************************************************************************************
** Function name:        Api_AsynUpdateAppConfig
** Descriptions:         ����flash���� 
** input parameter:      void   
** Returned Value:       NRF_SUCCESS
** Created  by:          ����
** Created  Datas:       2017-08-04
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
*********************************************************************************************************/
uint32_t Api_AsynUpdateAppConfig(void)
{
    Flash_CommonErase((uint32_t*)SYSTEM_PARAM_BASE_ADDR,1);
       
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
    for(uint8_t index = 0; index < 20; index++)
    {
        Record_UpdateAlarmTimeParam(index,&flash_param_s.alarm_time_s[index]);
    }
                
    App_UpdateHourglassParam(&flash_param_s.hourglass_s);
    App_UpdateBloodOxygenParam(&flash_param_s.blood_oxygen_config_s);
	
	App_UpdateUiStyleParam(flash_param_s.ui_style);
    
    App_UpdateMoveMaxPuseTimeParam(flash_param_s.movement_max_puse_time);
    
       
    return NRF_SUCCESS;
}

/*******************************************************************************************************
** Function name:        Api_FlashInit
** Descriptions:         flash  ��ʼ�� 
** input parameter:      ��  
** Returned Value:       ��
** Created  by:          ����
** Created  Datas:       2017-08-04
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
*********************************************************************************************************/
void Api_FlashInit(void)
{
	uint8_t history_system_time[8] = {0};
	
    Flash_CommonInit();
 
	//YX.20180123.�����ʱ������ݺ����Ͻ�����������ճ����ݴ洢����������	
    if(Api_ReadSystemTimeData(history_system_time) == NRF_SUCCESS)
    {
        SystemTime_Adjust(history_system_time,sizeof(history_system_time));
    }
    else
    {
        SystemTime_Init();
    }
	
    //Ů����Ŀ��ʼ��
    //Phys_InitParam();
    
    Api_FlashParamInit();
}

/*******************************************************************************************************
** Function name:        Api_AdjustTimeClearFlash
** Descriptions:         ����ʱ�����flash
** input parameter:      ��  
** Returned Value:       ��
** Created  by:          ����
** Created  Datas:       2017-08-04
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
*********************************************************************************************************/
bool Api_AdjustTimeClearFlash(const system_clock_time_t *p_history_time,const system_clock_time_t *p_current_time)
{
    uint32_t diff_hours = 0;
    bool b_data_clear_success = false;

    diff_hours = Get_SystemTimeDiffHour(p_current_time,p_history_time);
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
    //YX.20180509.Ů����Ŀʱ��仯��ͬ����������
    if((p_current_time->month != p_history_time->month)||(p_current_time->day != p_history_time->day))
    {
        //Phys_UpdataInfo();
    }

    if(b_data_clear_success)
    {
        Ped_SetSportsInfo(0, 0, 0);
        Record_DailyDataIndexInit(&flash_param_s.flash_index_s.daily);
        Record_OxygenDataIndexInit(&flash_param_s.flash_index_s.oxygen);
        //YX.20171124.���������ݺ�������������ݴ��ҵ�����
//        Record_BlockIndexInit(&flash_param_s.flash_index_s.block_index);
    }

    return b_data_clear_success;    
}

/*******************************************************************************************************
** Function name:        Api_WriteLogText
** Descriptions:         дflash log ����
** input parameter:      p_content:��־���� len����־����   
** Returned Value:       ��
** Created  by:          ����
** Created  Datas:       2017-08-04
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
*********************************************************************************************************/
void Api_WriteLogText(uint8_t* p_content, uint8_t len)
{
//    uint32_t  err_code = NRF_SUCCESS;
//    uint32_t  log_data[16] = {0};
//       
//    memcpy(log_data,p_content,64);
//    err_code == Record_AddLogData(flash_param_s.flash_index_s.log_s.offset,(const log_data_attr_t*)log_data);
//    if(err_code == NRF_SUCCESS)
//    {
//        flash_param_s.flash_index_s.log_s.offset = (flash_param_s.flash_index_s.log_s.offset + 1)%256;
//        if((flash_param_s.flash_index_s.log_s.offset%32) == 0)//128*32 =4096
//        {
//            Flash_CommonErase((uint32_t*)LOG_DATA_ADDR(flash_param_s.flash_index_s.log_s.offset*64),1);
//        }
//    }
}

/*******************************************************************************************************
** Function name:        Api_ClearLogText
** Descriptions:         ���flash log ����
** input parameter:      ��
** Returned Value:       ��
** Created  by:          ����
** Created  Datas:       2017-08-04
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
*********************************************************************************************************/
void Api_ClearLogText(void)
{
    //Flash_CommonErase((uint32_t*)LOG_DATA_ADDR(0),4);
}

/*******************************************************************************************************
** Function name:        Api_SaveDailyAttrData
** Descriptions:         �洢�ճ�����
** input parameter:      p_attr: �ճ�����ָ��   
** Returned Value:
** Created  by:          ����
** Created  Datas:       2017-08-04
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
*********************************************************************************************************/
void Api_SaveDailyAttrData(const daily_data_attr_t* p_attr)
{
    uint32_t err_code = NRF_SUCCESS;
    flash_find_index_t* p_flash_index = &flash_param_s.flash_index_s;
    uint8_t current_block_index = p_flash_index->block_index%3;

    err_code = Record_AddDailyData(current_block_index,p_flash_index->daily.offset[current_block_index],p_attr);
    if(err_code == NRF_SUCCESS)
    {
        p_flash_index->daily.offset[current_block_index]++;
        if(p_attr->hour >= 7 && p_attr->hour < 24 && p_attr->ox_value != 0)
        {
            p_flash_index->daily.ox_value_cnt[current_block_index]++;
        }
    }
}

/*******************************************************************************************************
** Function name:     Api_ReadDailyAttrData
** Descriptions:      ���ճ����� 
** input parameter:   day_id: ������                 package_off:ƫ���� 
**                    p_attr:�ճ�����ָ��
** Returned Value:    NRF_SUCCESS 
** Created  by:       ����
** Created  Datas:    2017-08-04
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
*********************************************************************************************************/
uint32_t Api_ReadDailyAttrData(uint8_t day_id,uint16_t package_offset,daily_data_attr_t* p_attr)
{
    Record_GetDailyData(day_id,package_offset,p_attr);
    
    return NRF_SUCCESS;
}


/*******************************************************************************************************
** Function name:          Api_SaveOxygenAttrData
** Descriptions:           �洢Ѫ������ 
** input parameter:        p_attr:Ѫ������ָ��
** Returned Value:         ��  
** Created  by:            ����
** Created  Datas:         2017-08-04
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
*********************************************************************************************************/
void Api_SaveOxygenAttrData(const blood_oxygen_attr_t* p_attr, const scatter_heart_t* p_heart)
{
    uint32_t err_code = NRF_SUCCESS;
    uint8_t  day_id = flash_param_s.flash_index_s.block_index%3;
       
    err_code = Record_AddOxygenData(day_id,flash_param_s.flash_index_s.oxygen.offset[day_id],p_attr);
       
    err_code = Record_AddScatterHeartData(day_id,flash_param_s.flash_index_s.oxygen.offset[day_id],p_heart);
    
    // ������Ӧ�ı���
    if(NRF_SUCCESS == err_code)
    {
        flash_param_s.flash_index_s.oxygen.offset[day_id]++;
    }
}


/*******************************************************************************************************
** Function name:          Api_ModifyOahsFlag
** Descriptions:           �޸�5����Ѫ��������־λ
** input parameter:        ��
** Returned Value:         ��  
** Created  by:            YX
** Created  Datas:         2018-05-11
*********************************************************************************************************/
void Api_ModifyOahsFlag(uint8_t flag, uint8_t *ox_result_buf)
{
    uint8_t  day_id = flash_param_s.flash_index_s.block_index%3;
    
    Record_ModifyOahsFlag(day_id,flash_param_s.flash_index_s.oxygen.offset[day_id], flag, ox_result_buf);    
}


/*******************************************************************************************************
** Function name:    Api_ReadOxygenAttrData
** Descriptions:     ��Ѫ������
** input parameter:  day_id:������   package_offset:ƫ����
** Returned Value:   NRF_SUCCESS 
** Created  by:      ����
** Created  Datas:   2017-08-04
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
*********************************************************************************************************/
uint32_t Api_ReadOxygenAttrData(uint8_t day_id,uint16_t package_offset,blood_oxygen_attr_t* p_attr)
{
    return Record_GetOxygenData(day_id,package_offset,p_attr);
}

void Api_SaveScatterHeartData(const scatter_heart_t* p_attr)
{
    uint32_t err_code = NRF_SUCCESS;
    uint8_t  day_id = flash_param_s.flash_index_s.block_index%3;
       
    err_code = Record_AddScatterHeartData(day_id,flash_param_s.flash_index_s.oxygen.offset[day_id],p_attr);
       
    // ������Ӧ�ı���
    if(NRF_SUCCESS == err_code)
    {
        flash_param_s.flash_index_s.oxygen.offset[day_id]++;
    }
}

uint32_t Api_ReadScatterHeartData(uint8_t day_id,uint16_t package_offset,scatter_heart_t* p_attr)
{
    return Record_GetScatterHeartData(day_id,package_offset,p_attr);
}

/*******************************************************************************************************
** Function name:             Api_GetStepAllCount
** Descriptions:              �õ�����
** input parameter:           app_day_id: ������
** Returned Value:            NRF_SUCCESS
** Created  by:               ����
** Created  Datas:            2017-08-04
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
*********************************************************************************************************/
uint32_t Api_GetStepAllCount(uint8_t app_day_id)
{
    uint8_t  local_day_id = (3 + flash_param_s.flash_index_s.block_index - app_day_id)%3;
    
    return flash_param_s.flash_index_s.daily.step_cnt[local_day_id];
}

void Api_SetSleepBlack(uint8_t black)
{
    sleep_cal_black = black%3;
}

/*******************************************************************************************************
** Function name:                               Api_SleepCallback
** Descriptions:       ˯�߻ص�����
** input parameter:    data:����ָ��
** Returned Value:     NRF_SUCCESS 
** Created  by:        ����
** Created  Datas:     2017-08-04
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
*********************************************************************************************************/
uint32_t Api_SleepCallback(uint8_t *data, uint16_t current_pkg, uint16_t pre_offset)
{
    uint32_t err_code = NRF_SUCCESS;
    uint8_t day_id = sleep_cal_black%3;//flash_param_s.flash_index_s.block_index%3;
    uint16_t offset = 0;
    

    //YX.20180103.������ʱ˯�ߴ�����Ϊ3������
    if(current_pkg == 0xFFFF) current_pkg = 0;
    
    if(current_pkg > pre_offset)
    {
        offset = (current_pkg - pre_offset - 1)%312;
    }
    else
    {
        day_id = (day_id+2)%3;
        if(flash_param_s.flash_index_s.daily.offset[day_id] > (pre_offset - current_pkg))
        {
            offset = (flash_param_s.flash_index_s.daily.offset[day_id] + current_pkg - pre_offset - 1)%312;
        }
        else
        {
            return NRF_ERROR_NOT_FOUND;
        }
    }
       
    err_code = Record_GetSleepAnalysisData(day_id,offset,data);
    
    if(err_code != NRF_SUCCESS)
    {
        return NRF_ERROR_NOT_FOUND;
    }
    FLASH_LOG_PRINT("sleep callback Data:%d:%d %d %d %d %d",data[1],data[2],data[3],data[4],data[5],data[6]);
    
    return err_code;
}

/*******************************************************************************************************
** Function name:      Api_FlashDailyJumpBlock
** Descriptions:       �ճ���������
** input parameter:    ��
** Returned Value:     0
** Created  by:        ����
** Created  Datas:     2017-08-04
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
*********************************************************************************************************/
uint32_t Api_FlashDailyJumpBlock(void)
{
    uint8_t current_day_id = flash_param_s.flash_index_s.block_index;
       
    // ��һ��:������һ������
    Record_ClearOneDayData((current_day_id + 1)%3);
	
    flash_param_s.flash_index_s.block_index = (flash_param_s.flash_index_s.block_index+1)%3;
	Record_DailyDataIndexInit(&flash_param_s.flash_index_s.daily);
    Record_OxygenDataIndexInit(&flash_param_s.flash_index_s.oxygen);
    
    return 0;
}

/*******************************************************************************************************
** Function name:      Api_ReadAllFlashData
** Descriptions:       ������flash����
** input parameter:    block_id:������ offset:ƫ����  p_data:����ָ��
                       data to store in
** Returned Value:     NRF_SUCCESS
** Created  by:        ����
** Created  Datas:     2017-08-04
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
*********************************************************************************************************/
uint32_t Api_ReadAllFlashData(uint8_t block_id,uint8_t offset,uint8_t* p_data)
{
    uint32_t flash_data[4] = {0};
    
    Flash_CommonRead(flash_data,RECORD_FLASH_BLOCK_DATA_ADDR(block_id,offset*16),16);
    memcpy(p_data,flash_data,16);
    
    return NRF_SUCCESS;
}

/*******************************************************************************************************
** Function name:            Api_FlashFatoryRestore
** Descriptions:             �������ݴ洢
** input parameter:          �� 
** Returned Value:           NRF_SUCCESS
** Created  by:              ����
** Created  Datas:           2017-08-04
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
*********************************************************************************************************/
uint32_t Api_FlashFatoryRestore(void)
{
    CONFIG_LANGUAGE_E language_temp = flash_param_s.language_e;
    uint8_t ret_val = 0;
    
    ret_val = Flash_CommonErase((uint32_t*)RECORD_FLASH_BLOCK_DATA_ADDR(0,0), RECORD_FLASH_BLOCK_DATA_SIZE/4096);
    
    //ZJH.2018.06.27������ݺ�����д�뵱ǰ���Բ�����FLASH
    if(ret_val != FS_SUCCESS)
    {
        NVIC_SystemReset(); 
    }
    else
    {
        Api_FlashParamInit();
        
        p_flash_param->language_e = (CONFIG_LANGUAGE_E)language_temp;
        
        BleThread_AsynUpdateAppConfig();
    }
    
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
    Flash_CommonErase((uint32_t*)RECORD_MOVEMENT_BASE_ADDR, RECORD_FLASH_BLOCK_USER_DATA_SIZE/4096);
    
    //Flash_CommonErase((uint32_t*)RECORD_FLASH_BLOCK_DATA_ADDR(0, 0), RECORD_FLASH_BLOCK_USER_DATA_SIZE/4096);

    // �ճ�����FLASH�������Ҽ���				
    //Record_LogDataIndexInit(&flash_param_s.flash_index_s.log_s);
    Record_DailyDataIndexInit(&flash_param_s.flash_index_s.daily);
    Record_BlockIndexInit(&flash_param_s.flash_index_s.block_index); 
    
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
    


/*******************************************************************************************************
** Function name:            Api_SaveSleepResultData
** Descriptions:             �洢˯������
** input parameter:          day_id:������ p_data:����ָ��
** Returned Value:           NRF_SUCCESS
** Created  by:              ����
** Created  Datas:           2017-08-04
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
*********************************************************************************************************/
uint32_t Api_SaveSleepResultData(uint8_t day_id, uint8_t* p_data)
{
    uint32_t result_data[24] = {0};
    uint8_t  sleep_wake_cnt = 0;
    
    memcpy(result_data,p_data,sizeof(result_data));
    sleep_wake_cnt = Api_GetSleepWakeCnt(day_id);
    Flash_CommonStore((uint32_t const *)RECORD_SLEEP_DATA_ADDR(day_id,sleep_wake_cnt*96),result_data,sizeof(result_data)/4);
    
    return NRF_SUCCESS;
}

/*******************************************************************************************************
** Function name:            Api_GetSleepWakeCnt
** Descriptions:             �õ����Ѵ���
** input parameter:          day_id:�� ����
** Returned Value:           ���Ѵ���
** Created  by:              ����
** Created  Datas:           2017-08-04
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
*********************************************************************************************************/
uint32_t Api_GetSleepWakeCnt(uint8_t day_id)
{
    uint8_t sleep_wake_cnt  = 0;
    uint32_t sleep_head = 0;
    uint8_t* p_sleep_head = (uint8_t*)&sleep_head;
	
    // !ע��:������Ҫ���쳣����
    if(day_id >= 3)
    {	    
        return 0;
    }
    
    // !ע��:��������ȡ6��˯��
    for(uint8_t index = 0; index < 6; index++)
    {
        sleep_head = 0;
        Flash_CommonRead(&sleep_head,RECORD_SLEEP_DATA_ADDR(day_id,index*96),sizeof(sleep_head));
	    
        if(p_sleep_head[0] == 0xA1)
        {
            sleep_wake_cnt++;
        }
        else
        {
            break;
        }
    }
    
    return sleep_wake_cnt;
 }
 
uint8_t  Api_GetSleepMapDayID(uint8_t block_id)
{
    uint8_t   sleep_map_id = 0;
    uint8_t   sleep_wake_cnt = 0;
    uint32_t  sleep_data[3] = {0};
    uint8_t*  p_sleep_data = (uint8_t*)sleep_data;
    system_clock_time_t  sleep_entry_time = {0};
    uint8_t   diff_days = 0;
    
    //YX.2017.12.11.˯�߽����˯��ʱ���Ϊ�����
//    // �жϽ����Ƿ���˯������
//    sleep_map_id = block_id;
//    sleep_wake_cnt = Api_GetSleepWakeCnt(block_id);
//    if(sleep_wake_cnt > 0)
//    {
//        for(uint8_t index = 0; index < sleep_wake_cnt; index++)
//        {
//            Flash_CommonRead(sleep_data,RECORD_SLEEP_DATA_ADDR(block_id,index*96),sizeof(sleep_data));
//            if((p_sleep_data[1] == clock->month)&&(p_sleep_data[2] == clock->day)&&(p_sleep_data[3] >= 8))
//            {
//                sleep_map_id = block_id;
//                break;
//            }
//        }
//    }
//    else
    {
        // ��ȡǰһ��˯������
        sleep_entry_time.year = clock->year;
        sleep_wake_cnt = Api_GetSleepWakeCnt((block_id+2)%3);
        for(uint8_t index = 0; index < sleep_wake_cnt; index++)
        {
            Flash_CommonRead(sleep_data,RECORD_SLEEP_DATA_ADDR((block_id+2)%3,index*96),sizeof(sleep_data));
            // ����˯������ʱ��
            sleep_entry_time.month = p_sleep_data[1];
            sleep_entry_time.day = p_sleep_data[2];
            diff_days = Get_DateDiffDays(&sleep_entry_time,clock);
            // �뵱ǰʱ����жԱ�(������Ҫ�ж�����ʱ��)
            if((diff_days <= 1) || (diff_days >= 360))
            {
                sleep_map_id = (block_id + 2)%3;
                break;
            }
        }
    }
    
    return sleep_map_id;   
}

/****************************************************************
** Function name:           Sleep_GetTime
** Descriptions:            ��ȡ˯��ʱ�䣬������ʾ
** input parameters:
** output parameters:
** Returned value:
** Created by:              WJ             
** Created Date:            2017-01-04
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
static bool Sleep_Adjust(uint8_t *cur_data,uint8_t *old_data)
{
	bool days = false;
	
	int32_t days0 = Sclock_SeekYearDays(clock->year,cur_data[1],cur_data[2]);////���㵱ǰ��˯����˯ʱ����������
	
	int32_t days1 = Sclock_SeekYearDays(clock->year,cur_data[5],cur_data[6]);/////���㵱ǰ��˯������ʱ����������
	
	int32_t days2 = Sclock_SeekYearDays(clock->year,clock->month,clock->day);/////��ǰʱ����������
	
	int32_t days3 = Sclock_SeekYearDays(clock->year,old_data[5],old_data[6]);//////ǰһ��˯������ʱ����������
	/*�ж���˯������ʱ���뵱ǰʱ���Ƿ�ӽ�*/
	if(((days2 - days0 ==0 || (days2 - days0 ==1 && cur_data[3]>8) )/////��˯ʱ���뵱ǰʱ���п�������������8 
      && days1 - days0 >=0 && days1 - days0 <=1 
      && days2 - days1 >=0 && days2 - days1 <=1)
      ||(cur_data[1] == 12 && cur_data[2] == 31
      && clock->month ==1 && clock->day ==1))/////���ǿ�������
	{
        if(days3 == 0)
        {
            days = true;
        }
        else if(days3 <= days0)////�ڶ���˯�ߵ���˯����>=��һ��˯�ߵ���������
        {
            if(days0 - days3 == 0)
            {
                if(cur_data[3] * 60 + cur_data[4] > old_data[7] * 60 + old_data[8])/////�ж�˯���ص�
                {
                    if((old_data[7] < 8 && cur_data[3] <= 8)||(old_data[7] > 8 && cur_data[3] > 8))
                     days = true;
                }
            }
            else if(days0 - days3 == 1)
            {
                if(old_data[3] > 8 && cur_data[3] < 8)
                {
                     days = true;
                }
            }
        }
        else if(cur_data[1] == 1 && cur_data[2] == 1 && old_data[1] ==12 && old_data[2] ==31)
        {
            days = true;
        }
        if(days2 == days0)
        {
            if((clock->hour * 60 + clock->minutes) <= (cur_data[3] * 60 + cur_data[4]))
            {
                days = false;
            }
        }
	}
	return days;
}

/*******************************************************************************************************
** Function name:            Api_GetSleepWakeCnt
** Descriptions:         
** input parameter:          �õ�˯��ʱ��
** Returned Value:           ˯��ʱ�� 
** Created  by:              ����
** Created  Datas:           2017-08-04
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
*********************************************************************************************************/
uint16_t Api_GetSleepTotalTime(void)
{
    uint8_t   sleep_wake_cnt = 0;
    uint8_t   block_index = 0;
    uint16_t  sleep_total_time = 0; 
    uint32_t  sleep_data[3] = {0};
    uint8_t*  p_sleep_data = (uint8_t*)sleep_data;
    uint8_t sleep_new_time[12] = {0x00, };     
    uint8_t sleep_old_time[12] = {0x00, }; 
//    system_clock_time_t  sleep_entry_time = {0}; 
//    system_clock_time_t  critical_time = {0};    
//    
//    // ������ٽ�ʱ�� xxxx-xx-xx 00:00:00
//    critical_time.year  = clock->year;
//    critical_time.month = clock->month;
//    critical_time.day   = clock->day;
//    critical_time.hour  = 0;
    
    //��ȡ��������ݿ��
    block_index = Api_GetSleepMapDayID(p_flash_param->flash_index_s.block_index);
    //��ȡ��������ݿ���Ϣ
    sleep_wake_cnt = Api_GetSleepWakeCnt(block_index);
    if(sleep_wake_cnt > 0)
    {
        for(uint8_t index = 0; index < sleep_wake_cnt; index++)
        {
            memset(p_sleep_data, 0x00 , sizeof(sleep_data));
            Flash_CommonRead(sleep_data,RECORD_SLEEP_DATA_ADDR(block_index,index*96),sizeof(sleep_data));
            //YX.2018.03.02.���Ӷ��˯�ߵĻ����߼�
            memcpy(sleep_new_time, (void *)p_sleep_data, 12);
            if(Sleep_Adjust(sleep_new_time, sleep_old_time))
            {
                sleep_total_time += p_sleep_data[9]+p_sleep_data[10];
            }
            else
            {
                continue;
            }
  
            memcpy(sleep_old_time, (void *)p_sleep_data, 12);
            
//            sleep_entry_time.year  = clock->year;
//            sleep_entry_time.month = p_sleep_data[1];
//            sleep_entry_time.day   = p_sleep_data[2];
//            sleep_entry_time.hour   = p_sleep_data[3];
//            
//            // ������Ҫ��������ʱ�� 12-31��
//            if((sleep_entry_time.month == 12)&&(sleep_entry_time.day == 31)&&(critical_time.month == 1)&&(critical_time.day == 1))
//            {
//                // ������ǰһ��12��31��
//                sleep_entry_time.year  = clock->year - 1;
//            }
//            
////            if((Get_DateDiffDays(&critical_time, &sleep_entry_time) != 0)&&(Get_SystemTimeDiffHour(&critical_time, &sleep_entry_time) > 4))
////            {
////                //��������8��ǰ��˯��˯�����ݣ����������
////                continue;
////            }
//            if((Get_DateDiffDays(&critical_time, &sleep_entry_time) != 0)&&(Get_SystemTimeDiffHour(&critical_time, &sleep_entry_time) > 16))
//            {
//                //��������8��ǰ��˯��˯�����ݣ����������
//                continue;
//            }
//            
//            // ������Ҫ�ж���Чʱ��
//            if((p_sleep_data[1] <= 12)&&(p_sleep_data[2] <= 31)&&(p_sleep_data[3] <= 23))
//            {
//                sleep_total_time += (p_sleep_data[9]+p_sleep_data[10])*5;
//            }
        }
    }
    
    return sleep_total_time * 5;    
}

/*******************************************************************************************************
** Function name:            Api_GetSleepPartData
** Descriptions:         
** input parameter:          �õ�˯������
** Returned Value:           NRF_SUCCESS
** Created  by:              ����
** Created  Datas:           2017-08-04
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
*********************************************************************************************************/
uint32_t Api_GetSleepPartData(uint8_t day_id,uint8_t offset,uint8_t* p_data)
{
    uint32_t part_data[4] = {0};
    if(day_id < 3)
    {
        Flash_CommonRead(part_data,RECORD_SLEEP_DATA_ADDR(day_id,offset*16),sizeof(part_data));
        memcpy(p_data,part_data,sizeof(part_data));
    }
    return NRF_SUCCESS;
}

/*******************************************************************************************************
** Function name:        Api_SaveSystemTime
** Descriptions:         �洢ϵͳʱ��
** input parameter:      ��������   
** Returned Value:       NRF_SUCCESS
** Created  by:          ����
** Created  Datas:       2017-08-04
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
*********************************************************************************************************/
uint32_t Api_SaveSystemTime(uint16_t year,uint8_t month,uint8_t day,uint8_t hour,uint8_t minute)
{
    uint32_t system_time[2] = {0};
    uint8_t*  p_system_time = (uint8_t*)system_time;
    // 1���ж�ʱ��ģ���Ƿ�д�� 
    if(flash_param_s.systemtime_index >= 512)
    {
        flash_param_s.systemtime_index = 0;
        //������FLASH��
        Flash_CommonErase((uint32_t*)(SYSTEM_TIME_BASE_ADDR),1);
    }
       
    p_system_time[0] = (year>>8)&0xFF;
    p_system_time[1] = year&0xFF;
    p_system_time[2] = month;
    p_system_time[3] = day;
    p_system_time[4] = hour;
    p_system_time[5] = minute;
    p_system_time[6] = (flash_param_s.systemtime_index>>8)&0xFF;;
    p_system_time[7] = flash_param_s.systemtime_index&0xFF;;
    Flash_CommonStore((uint32_t*)(SYSTEM_TIME_BASE_ADDR+flash_param_s.systemtime_index*8),system_time,2);
    flash_param_s.systemtime_index++;
    return NRF_SUCCESS;
} 

/*******************************************************************************************************
** Function name:           Api_ReadSystemTimeData
** Descriptions:            ��ϵͳʱ��  
** input parameter:         pdata:�������ݼ�¼   
** Returned Value:          NRF_SUCCESS
** Created  by:             ����
** Created  Datas:          2017-08-04
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
*********************************************************************************************************/
uint32_t Api_ReadSystemTimeData(uint8_t* pdata)
{
    uint32_t system_time[2] = {0};
    for(uint16_t index = 0; index < 512; index++)
    {
        Flash_CommonRead(system_time,SYSTEM_TIME_BASE_ADDR+index*8,sizeof(system_time));
        if((0xFFFFFFFF == system_time[0])&&(0xFFFFFFFF == system_time[1]))
        {
            if(0 == index)
            {
                 return NRF_ERROR_NOT_FOUND;
            }
            else
            {
                Flash_CommonRead(system_time,SYSTEM_TIME_BASE_ADDR+(index-1)*8,sizeof(system_time));
                memcpy(pdata,system_time,sizeof(system_time));
                break;
            }
        }
    }
    
    return NRF_SUCCESS;
}

/*******************************************************************************************************
** Function name:        Api_UpdateHeartAlertParam
** Descriptions:         �������ʱ�������  
** input parameter:      p_alert�����ʲ���  
** Returned Value:       NRF_SUCCESS
** Created  by:          ����
** Created  Datas:       2017-08-04
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
*********************************************************************************************************/
uint32_t Api_UpdateHeartAlertParam(const heart_alert_config_t* p_alert)
{
    //�����ڴ�����
    flash_param_s.heart_alert_s.enable = p_alert->enable;
    flash_param_s.heart_alert_s.high = p_alert->high;
    flash_param_s.heart_alert_s.low = p_alert->low;
	
    BleThread_AsynUpdateAppConfig();
    
    return NRF_SUCCESS;
}

/*******************************************************************************************************
** Function name:           Api_UpdateTurnWristParam
** Descriptions:            ����ת�������  
** input parameter:         P_turnwrist:����ָ��     
** Returned Value:          NRF_SUCCESS
** Created  by:             ����
** Created  Datas:          2017-08-04
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
*********************************************************************************************************/
uint32_t Api_UpdateTurnWristParam(const turnwrist_config_t* p_turnwrist)
{
    memcpy(&flash_param_s.turnwrist_s,p_turnwrist,sizeof(turnwrist_config_t));
    
    BleThread_AsynUpdateAppConfig();
    
    return NRF_SUCCESS;
}

/*******************************************************************************************************
** Function name:          Api_UpdateBloodConfig
** Descriptions:           ����Ѫ������   
** input parameter:        P_config:����ָ��    
** Returned Value:         NRF_SUCCESS
** Created  by:            ����
** Created  Datas:         2017-08-04
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
*********************************************************************************************************/
uint32_t Api_UpdateBloodConfig(const bp_config_t* p_config)
{
    flash_param_s.bp_s.mode = p_config->mode;
    flash_param_s.bp_s.private_high = p_config->private_high;
    flash_param_s.bp_s.private_low = p_config->private_low;
	
    BleThread_AsynUpdateAppConfig();
    
    return NRF_SUCCESS;
}

/*******************************************************************************************************
** Function name:          Api_UpdateMessageEnableConfig
** Descriptions:           ������Ϣ����  
** input parameter:        p_enable:����ָ��     
** Returned Value:         NRF_SUCCESS
** Created  by:            ����
** Created  Datas:         2017-08-04
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
*********************************************************************************************************/
uint32_t Api_UpdateMessageEnableConfig(const app_message_enable_t *p_enable)
{
    memcpy(&flash_param_s.message_enable_s,p_enable,sizeof(app_message_enable_t));
	
    BleThread_AsynUpdateAppConfig();
    
    return NRF_SUCCESS;
}

/*******************************************************************************************************
** Function name:          Api_GetBloodMode
** Descriptions:           ѪҺģʽ  
** input parameter:        ��
** Returned Value:         ģʽ
** Created  by:            ����
** Created  Datas:         2017-08-04
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
*********************************************************************************************************/
BLOOD_MODE_E Api_GetBloodMode(void)
{
    return flash_param_s.bp_s.mode;
}

/*******************************************************************************************************
** Function name:          Api_UpdateDevConfig
** Descriptions:           �����豸����
** input parameter:        p_dev_config_t: �豸���� 
** Returned Value:         NRF_SUCCESS
** Created  by:            ����
** Created  Datas:         2017-08-04
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
*********************************************************************************************************/
uint32_t Api_UpdateDevConfig(const dev_config_t* p_dev_config_t)
{
    memcpy(&flash_param_s.dev_config_s,p_dev_config_t,sizeof(dev_config_t));
	
    BleThread_AsynUpdateAppConfig();
    
    return NRF_SUCCESS;
}

/*******************************************************************************************************
** Function name:          Api_UpdateSedentaryConfig
** Descriptions:           ���¾������ò���  
** input parameter:        P_sedentary_config_t:��������ָ��
** Returned Value:         NRF_SUCCESS
** Created  by:            ����
** Created  Datas:         2017-08-04
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
*********************************************************************************************************/
uint32_t Api_UpdateSedentaryConfig(const sedentary_config_t* p_sedentary_config_t)
{
    memcpy(&flash_param_s.sedentary_config_s,p_sedentary_config_t,sizeof(sedentary_config_t));
	
    BleThread_AsynUpdateAppConfig();
    
    return NRF_SUCCESS;
}

/*******************************************************************************************************
** Function name:          Api_UpdateGuiLanguage
** Descriptions:             
** input parameter:        ����
** Returned Value:         NRF_SUCCESS
** Created  by:            GT
** Created  Datas:         2017-08-04
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
*********************************************************************************************************/
uint32_t Api_UpdateGuiLanguage(CONFIG_LANGUAGE_E language)
{
    flash_param_s.language_e = language;
        
    return App_UpdateGuiLanguage(language);
}
/*******************************************************************************************************
** Function name:          Api_UpdateConnectPassword
** Descriptions:           ��������  
** input parameter:        password:����
** Returned Value:         NRF_SUCCESS
** Created  by:            ����
** Created  Datas:         2017-08-04
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
*********************************************************************************************************/
uint32_t Api_UpdateConnectPassword(uint16_t password)
{
    flash_param_s.password = password;
	
    BleThread_AsynUpdateAppConfig();
    
    return NRF_SUCCESS;
}

/*******************************************************************************************************
** Function name:          Api_UpdateAlarmTime
** Descriptions:           index:��������  p_alarm:���Ӳ���ָ��   
** input parameter:        password:����
** Returned Value:         NRF_SUCCESS
** Created  by:            ����
** Created  Datas:         2017-08-04
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
*********************************************************************************************************/
uint32_t Api_UpdateAlarmTime(uint8_t index,const alarm_time_config_t* p_alarm)
{
    memcpy(&flash_param_s.alarm_time_s[index%20],p_alarm,sizeof(alarm_time_config_t));
	
    BleThread_AsynUpdateAppConfig();
    
    return NRF_SUCCESS;
}

/*******************************************************************************************************
** Function name:          Api_GetAlarmCRC
** Descriptions:           index:��������  p_alarm:���Ӳ���ָ��  
** input parameter:        password:����
** Returned Value:         ���Ӳ���
** Created  by:            ����
** Created  Datas:         2017-08-04
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
*********************************************************************************************************/
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
            alarm_crc = Crc16(alarm_crc,alarm_data,sizeof(alarm_data));
          }
    }
    return alarm_crc;
}

/*******************************************************************************************************
** Function name:          Api_UpdateBrightness
** Descriptions:           ������������  
** input parameter:        p_bright:��������ָ��
** Returned Value:         NRF_SUCCESS
** Created  by:            ����
** Created  Datas:         2017-08-04
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
*********************************************************************************************************/
uint32_t Api_UpdateBrightness(brightness_config_t* p_bright)
{
    memcpy(&flash_param_s.brightness_s,p_bright,sizeof(brightness_config_t));
	
    BleThread_AsynUpdateAppConfig();
    
    return NRF_SUCCESS;
}

/*******************************************************************************************************
** Function name:          Api_UpdateUserInfomation
** Descriptions:           �����û���Ϣ 
** input parameter:        p_person:������Ϣ����ָ��
** Returned Value:         NRF_SUCCESS
** Created  by:            GT
** Created  Datas:         2017-08-04
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
*********************************************************************************************************/
uint32_t Api_UpdateUserInfomation(const person_infomation_t* p_person,uint16_t target)
{
    memcpy(&flash_param_s.person_s,p_person,sizeof(person_infomation_t));
    if(flash_param_s.sport_target != target)
    {
        flash_param_s.Remind_flag = 0;//�����趨Ŀ�꣬������
    }
	
	flash_param_s.sport_target = target;
//    App_UpdatePersonInfomation(p_person);
    
    BleThread_AsynUpdateAppConfig();
    
    return NRF_SUCCESS;
}

uint32_t Api_UpdateHourglassParam(const hourglass_config_t *p_hourglass_config)
{
    memcpy(&flash_param_s.hourglass_s,p_hourglass_config,sizeof(hourglass_config_t));
 
    BleThread_AsynUpdateAppConfig();
    
    return NRF_SUCCESS;
}


/*******************************************************************************************************
** Function name:          Api_UpdateBloodOxygenParam
** Descriptions:           ����Ѫ����������  
** input parameter:        p_param:����ָ��
** Returned Value:         NRF_SUCCESS
** Created  by:            ����
** Created  Datas:         2017-08-04
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
*********************************************************************************************************/
uint32_t Api_UpdateBloodOxygenParam(const time_range_config_t* p_param)
{
    memcpy(&flash_param_s.blood_oxygen_config_s,p_param,sizeof(time_range_config_t));
	
    BleThread_AsynUpdateAppConfig();
    
    return NRF_SUCCESS;   
}

/*******************************************************************************************************
** Function name:          Api_GetDevSN
** Descriptions:           �õ��豸SN  
** input parameter:        ��
** Returned Value:         �豸id
** Created  by:            ����
** Created  Datas:         2017-08-04
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
*********************************************************************************************************/
uint32_t Api_GetDevSN(void)
{
    return Dev_GetDevID(&flash_param_s.product_infomation_s);
}

/*******************************************************************************************************
** Function name:          Api_DevSNCheck
** Descriptions:           �豸sn���  
** input parameter:        ��
** Returned Value:         ��
** Created  by:            ����
** Created  Datas:         2017-08-04
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
*********************************************************************************************************/
void Api_DevSNCheck(void)
{
    Dev_SNCheck(&flash_param_s.product_infomation_s);	
}

void Api_UpdateBluetoothName(const bluetooth_name_t* p_bluetooth_name)
{
    memcpy(&flash_param_s.bluetooth_name_s, p_bluetooth_name, sizeof(bluetooth_name_t)); 

    BleThread_AsynUpdateAppConfig();    
}

//void Api_AddUnreadMessage(const unread_message_detail_t *p_message)
//{
//    // 1.��ӵ�FLASH��
//    flash_param_s.unread_message_index_s.write_pos = (flash_param_s.unread_message_index_s.write_pos+1)%16;

//    Record_AddUnreadMessageData(flash_param_s.unread_message_index_s.write_pos,p_message);	
//}

//bool Api_GetUnreadMessage(unread_message_detail_t *p_message)
//{
//	
//}

//void Api_UpdateUnreadMessage(uint8_t page_index, uint8_t len)
//{
//}


/*******************************************************************************************************
** Function name:          Api_UpdateUiStyle
** Descriptions:           ������ʽ  
** input parameter:        ui_style:��ʽ
** Returned Value:         NRF_SUCCESS
** Created  by:            ����
** Created  Datas:         2017-08-04
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
*********************************************************************************************************/
uint32_t Api_UpdateUiStyle(uint8_t ui_style)
{
    flash_param_s.ui_style = ui_style;
	
    BleThread_AsynUpdateAppConfig();
    
    return NRF_SUCCESS;
}

/*******************************************************************************************************
** Function name:          Api_GetUiStyle
** Descriptions:           ��ȡ�������� 
** input parameter:        ��
** Returned Value:         �����ʽ
** Created  by:            YX
** Created  Datas:         2017-12-28
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
*********************************************************************************************************/
uint8_t Api_GetUiStyle(void)
{
    return flash_param_s.ui_style;
}

