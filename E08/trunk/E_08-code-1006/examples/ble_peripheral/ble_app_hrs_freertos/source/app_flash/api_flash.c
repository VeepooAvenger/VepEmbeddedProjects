/*******************Copyright(c)*********************************
** Veepoo Tech
** COPYRIGHT (C) 2016-2019
**----------------------FileInfo---------------------------------
** File name:                  api_flash.c
** Last modified Date:         2017-07-10
** Last SYS_VERSION:           0.0.1
** Descriptions:
**---------------------------------------------------------------
** Createdby:                  郭涛
** Createddate:                2017-07-10
** SYS_VERSION:                0.0.1
** Descriptions:               flash 接口
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

uint8_t sleep_cal_black = 0;        //计算睡眠的快号

//数据校验数据
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
** Descriptions:         crc 校验  
** input parameter:      crc:校验值  data:需要校验值   
** Returned Value:       crc
** Created  by:          郭涛
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
** Descriptions:       flash 参数初始化 
** input parameter:    无   
** Returned Value:     无
** Created  by:        郭涛
** Created  Datas:     2017-08-04
**-----------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
**************************************************************************/
static void Api_FlashParamInit(void)
{
    memset(&flash_param_s, 0, sizeof(flash_param_s));
	   
    //WJ.20181105.蓝牙名称的存储位置修改到生产信息中
    //App_GetBluetoothName(&flash_param_s.bluetooth_name_s);
    Dev_GetProductionInfomation(&flash_param_s.product_infomation_s);
    // WJ.20181105.获取蓝牙名称
    Dev_CheckBleAdvName();   
    // 久坐
    flash_param_s.sit_func_state = Dev_GetSitFunction();     
    // 血压
    flash_param_s.bp_func_state = Dev_GetBpFunction();    
    
    // 系统时间索引查找
    Record_GetSystemTimeIndex(&flash_param_s.systemtime_index);
       
    // 初始化获取密码
    App_GetConnectPassword(&flash_param_s.password);
    // 获取用户参数信息
    App_GetSportCount(&flash_param_s.sport_target);
    App_GetPersonInfomation(&flash_param_s.person_s);
       
    App_GetHeartAlertParam(&flash_param_s.heart_alert_s.enable, 
                           &flash_param_s.heart_alert_s.high, 
                           &flash_param_s.heart_alert_s.low);
    // 获取翻腕开关信息
    App_GetTurnWristParam(&flash_param_s.turnwrist_s);
    App_GetDevConfig(&flash_param_s.dev_config_s);
    App_GetMessageEnable(&flash_param_s.message_enable_s);
    App_GetSedentaryConfig(&flash_param_s.sedentary_config_s);
    App_GetBrightnessParam(&flash_param_s.brightness_s);
    
    // 血压
    App_GetBloodMode(&flash_param_s.bp_s);    
       
       
    //获取语言信息
    App_GetGuiLanguage(&flash_param_s.language_e);
#if (!APP_USED_OLD_ALARM_ENABLE)    
    //初始化闹钟时间
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
    
    // 日常数据FLASH索引查找计算				
    Record_LogDataIndexInit(&flash_param_s.flash_index_s.log_s);
    Record_DailyDataIndexInit(&flash_param_s.flash_index_s.daily);

    Record_BlockIndexInit(&flash_param_s.flash_index_s.block_index); 
    
    //WJ.20180821.增加UI亮屏保持参数
    App_GetUiDisplayTimeParam(&flash_param_s.ui_display_time);
    
    // WJ.20181105.增加ecg存储索引初始化
    Record_EcgDataIndexInit(&flash_param_s.flash_index_s.ecg);      
    
    {   //WJ.20190105.ECG自动测量数据存储索引初始化
        p_data_manage->ext_flash_ecg_opt_s.init = 1;
        Extflash_EcgErrDeal();
    }
}

/************************************************************************
** Function name:        Api_AsynUpdateAppConfig
** Descriptions:         更新flash参数 
** input parameter:      void   
** Returned Value:       NRF_SUCCESS
** Created  by:          郭涛
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
       
    // 更新心率告警参数
    App_UpdateHeartAlertParam(flash_param_s.heart_alert_s.enable, 
                              flash_param_s.heart_alert_s.high, 
                              flash_param_s.heart_alert_s.low);
    // 更新翻腕开关信息
    App_UpdateTurnWristParam(&flash_param_s.turnwrist_s);
    App_UpdateDevConfig(&flash_param_s.dev_config_s);
    App_UpdateMessageEnable(&flash_param_s.message_enable_s);
    App_UpdateSedentaryConfig(&flash_param_s.sedentary_config_s);
    App_UpdateBrightnessParam(&flash_param_s.brightness_s);
    
    // 血压
    App_SaveBloodMode(&flash_param_s.bp_s);

    //更新语言
    App_UpdateGuiLanguage(flash_param_s.language_e);
    // 更新闹钟闹钟时间
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
       
    //WJ.20180821.增加UI亮屏保持参数
    App_UpdateUiDisplayTimeParam(flash_param_s.ui_display_time);
    
    return NRF_SUCCESS;
}

/************************************************************************
** Function name:        Api_FlashInit
** Descriptions:         flash  初始化 
** input parameter:      无  
** Returned Value:       无
** Created  by:          郭涛
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
    
	//YX.20180123.解决对时清除数据后，马上进行升级造成日常数据存储快出错的问题	
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
** Descriptions:         调整时间清除flash
** input parameter:      无  
** Returned Value:       无
** Created  by:          郭涛
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
        // 擦除当天数据
        b_data_clear_success = true;
	    if(Movement_IsBusy())
        {
            Movement_Exit();
        }
		if(p_history_time->day != p_current_time->day)
		{
			//如果夸天往前对时，清除所有数据
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
		//对时夸天
		if(p_history_time->day < p_current_time->day)
		{
			//则跳到下一块去
			flash_param_s.flash_index_s.block_index = (flash_param_s.flash_index_s.block_index+1) % 3;
			Record_ClearOneDayData(flash_param_s.flash_index_s.block_index);
		}
		else
		{
			//清除所有数据，对时到前一天
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
        //YX.20171124.解决清楚数据后夸快区，造成数据错乱的问题
        ////Record_BlockIndexInit(&flash_param_s.flash_index_s.block_index);
    }

    return b_data_clear_success;    
}

/************************************************************************
** Function name:        Api_WriteLogText
** Descriptions:         写flash log 数据
** input parameter:      p_content:日志内容 len：日志长度   
** Returned Value:       无
** Created  by:          郭涛
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
** Descriptions:         清除flash log 数据
** input parameter:      无
** Returned Value:       无
** Created  by:          郭涛
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
** Descriptions:         存储日常数据
** input parameter:      p_attr: 日常数据指针   
** Returned Value:
** Created  by:          郭涛
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
** Descriptions:      读日常数据 
** input parameter:   day_id: 块索引                 package_off:偏移量 
**                    p_attr:日常数据指针
** Returned Value:    NRF_SUCCESS 
** Created  by:       郭涛
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
** Descriptions:              得到步数
** input parameter:           app_day_id: 日索引
** Returned Value:            NRF_SUCCESS
** Created  by:               郭涛
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
** Descriptions:       睡眠回调函数
** input parameter:    data:数据指针
** Returned Value:     NRF_SUCCESS 
** Created  by:        郭涛
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

    //YX.20180103.解决零点时睡眠错误码为3的问题
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
        {   //超时，放弃处理
            return NRF_ERROR_NOT_FOUND;
        }  
    }
    while(Extflash_DailyReadIsComplete() == 0)
    {
        vTaskDelay(1);
        read_time_out++;
        if(read_time_out > 1200)
        {   //超时，放弃处理
            return NRF_ERROR_NOT_FOUND;
        }
    }
        
    {
    //数据处理，填充
    data_buf_index = 0;
    memcpy(&data[data_buf_index], &p_attr->month, 4);
    data_buf_index += 4;
    
    data[data_buf_index++] = (p_attr->step >> 8) & 0xFF;
    data[data_buf_index++] = p_attr->step & 0xFF;
    data[data_buf_index++] = (p_attr->sport >> 8) & 0xFF;
    data[data_buf_index++] = p_attr->sport & 0xFF;
    data[data_buf_index++] = p_attr->wear;                          //佩戴（心率）
    data[data_buf_index++] = p_attr->gesture;                       //姿态
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
** Descriptions:       日常数据跳块
** input parameter:    无
** Returned Value:     0
** Created  by:        郭涛
** Created  Datas:     2017-08-04
**-----------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
**************************************************************************/
uint32_t Api_FlashDailyJumpBlock(void)
{
    uint8_t current_day_id = flash_param_s.flash_index_s.block_index;
       
    // 第一步:擦除下一块数据
    Record_ClearOneDayData((current_day_id + 1)%3);
	
    flash_param_s.flash_index_s.block_index = (flash_param_s.flash_index_s.block_index+1)%3;
	Record_DailyDataIndexInit(&flash_param_s.flash_index_s.daily);
    
    
    // 存储功耗统计存储
    Record_AddPowerInfoData((current_day_id)%3, &p_data_manage->power_statistics_s);    
    memset(&p_data_manage->power_statistics_s, 0, sizeof(power_statistics_t));
    
    return 0;
}

/************************************************************************
** Function name:      Api_ReadAllFlashData
** Descriptions:       读所有flash数据
** input parameter:    block_id:块索引 offset:偏移量  p_data:数据指针
                       data to store in
** Returned Value:     NRF_SUCCESS
** Created  by:        郭涛
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
        {   //超时，放弃处理
            return 1;
        }  
    }
    while(Extflash_AllOptReadIsComplete() == 0)
    {
        vTaskDelay(1);
        read_time_out++;
        if(read_time_out > 1200)
        {   //超时，放弃处理
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
** Descriptions:             工厂数据存储
** input parameter:          无 
** Returned Value:           NRF_SUCCESS
** Created  by:              郭涛
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
    
    // 清除系统配置参数---片内flash
    Flash_CommonErase((uint32_t*)RECORD_FLASH_BLOCK_DATA_ADDR(0, 0), RECORD_FLASH_BLOCK_DATA_SIZE/4096);
    
    // 清除外部flash数据，睡眠12K，日常108K,运动模式24K,ECG手动测量8k, ECG自动测量180K
    start_sector = RECORD_SLEEP_BASE_ADDR / (4096 * 8);
    sector_block = (ECG_EXT_FLASH_END_SIZE - RECORD_SLEEP_BASE_ADDR) / (4096 * 8);
    
    for(uint8_t i = start_sector; i < sector_block + 1; i++)
    {
        //Extflash_DataSectorErase4K(i);        
        // WJ.20190216.时间过长
        App_Watch_Dog_Reload();
        Extflash_DataBlockErase32K(start_sector + i);
    }
    
    Api_FlashParamInit();
    
    //WJ.20180630.解决清除数据后语言发生改变的bug
    flash_param_s.language_e = temp_language_e;
    Api_AsynUpdateAppConfig();
    
    return NRF_SUCCESS;
}

/****************************************************************
** Function name:           Api_FlashClearUserData
** Descriptions:            清除用户数据
** input parameters:        无
** output parameters:       无
** Returned value:          无
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
        // WJ.20190216.时间过长
        App_Watch_Dog_Reload();
        Extflash_DataBlockErase32K(start_sector + i);
        
    }
    
    // 日常数据FLASH索引查找计算				
    Record_LogDataIndexInit(&flash_param_s.flash_index_s.log_s);
    Record_DailyDataIndexInit(&flash_param_s.flash_index_s.daily);
    Record_BlockIndexInit(&flash_param_s.flash_index_s.block_index); 
    
    // WJ.20181105.增加ecg存储索引初始化
    Record_EcgDataIndexInit(&flash_param_s.flash_index_s.ecg);     
        
    {   //WJ.20190105.ECG自动测量数据存储索引初始化
        p_data_manage->ext_flash_ecg_opt_s.init = 1;
        Extflash_EcgErrDeal();
    }
    
    return NRF_SUCCESS;
}

/****************************************************************
** Function name:           Api_FlashClearUserConfig
** Descriptions:            清除用户配置
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2018-11-21
*****************************************************************/ 
uint32_t Api_FlashClearUserConfig(void)
{
    CONFIG_LANGUAGE_E temp_language_e = flash_param_s.language_e;
    
    Flash_CommonErase((uint32_t*)SYSTEM_PARAM_BASE_ADDR, 1);
    Api_FlashParamInit();
    
    //WJ.20180630.解决清除数据后语言发生改变的bug
    flash_param_s.language_e = temp_language_e;
    Api_AsynUpdateAppConfig();
    
    return NRF_SUCCESS;
}
    

/************************************************************************
** Function name:            Api_SaveSleepResultData
** Descriptions:             存储睡眠数据
** input parameter:          day_id:块索引；
                             p_data:数据指针，4字节对齐,数据量太大，不在重新开辟空间；
                             data_len:4的倍数
** output parameter:         无
** Returned Value:           NRF_SUCCESS
** Created  by:              王炬
** Created  Datas:           2018-10-13
**************************************************************************/
uint32_t Api_SaveSleepResultData(uint8_t day_id, uint8_t *p_data, uint16_t data_len)
{
    uint8_t     sleep_wake_cnt = 0;
    uint32_t    sleep_offset_buf[SLEEP_ONE_DAY_MAX_TIMES + 1] = {0};     //每段睡眠的偏移地址
    
    //过滤不符合条件的存储
    if(day_id >= 3
      || p_data == NULL
      || (uint32_t)p_data & 0x03
      || data_len & 0x03)
    {
        return 1;
    }
    // 获取每段睡眠的偏移量
    Api_GetSleepStoreInfo(day_id, &sleep_wake_cnt, NULL, sleep_offset_buf);
    
    // WJ.20190116.增加存储超出判断
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
        {   //超时，放弃处理
            return 1;
        }  
    }
    while(Extflash_SleepWriteIsComplete() == 0)
    {
        vTaskDelay(1);
        read_time_out++;
        if(read_time_out > 1200)
        {   //超时，放弃处理
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
        {   //超时，放弃处理
            return 1;
        }  
    }
    while(Extflash_SleepReadIsComplete() == 0)
    {
        vTaskDelay(1);
        read_time_out++;
        if(read_time_out > 1200)
        {   //超时，放弃处理
            return 1;
        }
    }
    return 0;
}
    

/************************************************************************
** Function name:            Api_AlterSleepContinueFlag
** Descriptions:             修改上一段的睡眠连接标志
** input parameter:          day_id:块索引；
                             wake_cnt:睡眠段数
** output parameter:         无
** Returned Value:           NRF_SUCCESS
** Created  by:              WJ
** Created  Datas:           2019-01-22
**************************************************************************/
uint32_t Api_AlterSleepContinueFlag(uint8_t day_id, uint8_t wake_cnt)
{
    uint8_t     sleep_wake_cnt = 0;
    uint32_t    sleep_offset_buf[SLEEP_ONE_DAY_MAX_TIMES + 1] = {0};     //每段睡眠的偏移地址
    uint32_t    sleep_buf[2] = {0};
    uint8_t     *p_sleep_buf = (uint8_t *)&sleep_buf[0];

    // 获取每段睡眠的偏移量
    Api_GetSleepStoreInfo(day_id, &sleep_wake_cnt, NULL, sleep_offset_buf);
    
    if(sleep_wake_cnt < wake_cnt || sleep_wake_cnt < 2)
    {   //下一段睡眠不存在，不改变
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
        {   //超时，放弃处理
            return 1;
        }  
    }
    while(Extflash_SleepWriteIsComplete() == 0)
    {
        vTaskDelay(1);
        read_time_out++;
        if(read_time_out > 1200)
        {   //超时，放弃处理
            return 1;
        }
    }
    
    return NRF_SUCCESS;
}
 
/************************************************************************
** Function name:            Api_GetSleepContinueNextFlag
** Descriptions:             获取某一段的睡眠向下连接标志
** input parameter:          day_id:块索引；
                             wake_cnt:睡眠段数
** output parameter:         无
** Returned Value:           NRF_SUCCESS
** Created  by:              WJ
** Created  Datas:           2019-01-22
**************************************************************************/
uint8_t Api_GetSleepContinueNextFlag(uint8_t day_id, uint8_t wake_cnt)
{
    uint8_t     sleep_wake_cnt = 0;
    uint32_t    sleep_offset_buf[SLEEP_ONE_DAY_MAX_TIMES + 1] = {0};     //每段睡眠的偏移地址
    uint32_t    sleep_buf[2] = {0};
    uint8_t     *p_sleep_buf = (uint8_t *)&sleep_buf[0];
    
    // 获取每段睡眠的偏移量
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
** Descriptions:            获取睡眠存储信息
** input parameter:         day_id:日 索引
** output parameter:        sleep_num：睡眠次数；
                            sleep_size: 每段睡眠数据的大小，数组长度为5；
                            sleep_offset：每段睡眠的地址，数组长度为5；
** Returned Value:          无
** Created  by:             王炬
** Created  Datas:          2018-10-13
**************************************************************************/
void Api_GetSleepStoreInfo(uint8_t day_id, uint8_t *sleep_num, uint16_t *sleep_size, uint32_t *sleep_offset)
{
    uint8_t     sleep_wake_cnt  = 0;
	uint32_t    temp_offset = 0;
    __ALIGN(4) uint8_t  p_sleep_head[4] = {0};
    
    // !注意:这里需要做异常处理
    if(day_id >= 3)
    {	    
        return ;
    }
    
    sleep_offset[0] = 0;
    // !注意:这里最多读取5端睡眠
    for(uint8_t index = 0; index < SLEEP_ONE_DAY_MAX_TIMES; index++)
    {
        ////Flash_CommonRead((uint32_t *)p_sleep_head, RECORD_SLEEP_DATA_ADDR(day_id, temp_offset), sizeof(p_sleep_head));
	    if(0 != Api_ReadSleepData(RECORD_SLEEP_DATA_ADDR(day_id, temp_offset), p_sleep_head, sizeof(p_sleep_head)))
        {//异常
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
        // 计算下一段睡眠的起始地址
        temp_offset += p_sleep_head[1] + (p_sleep_head[2] << 8);
        if(temp_offset % 4 != 0)
        {   //注：设计时已按照4字节对齐
            temp_offset = (temp_offset/4 + 1) * 4;
        }
        //获取每段睡眠的起始地址
		//JSH.原来次数少了一次
        if(sleep_offset != NULL && sleep_wake_cnt < SLEEP_ONE_DAY_MAX_TIMES)
        {
            sleep_offset[sleep_wake_cnt] = temp_offset;
        }
        //每段睡眠数据的大小
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
** Descriptions:            失眠存储信息
** input parameter:         day_id:日 索引
** output parameter:        sleep_num：睡眠次数；
                            sleep_curve_len: 睡眠曲线总长度；
** Returned Value:          无
** Created  by:             王炬
** Created  Datas:          2018-10-13
**************************************************************************/
void Api_GetSleepCurveLength(uint8_t day_id, uint8_t *sleep_num, uint16_t *sleep_curve_len)
{
    uint8_t     sleep_wake_cnt  = 0;                //睡眠段数
    uint8_t     sleep_curve_index = 0;              //睡眠曲线长度下标
	uint32_t    temp_offset = 0;                    //睡眠段信息偏移长度
	uint32_t    sleep_curve_offset = 0;             //曲线长度偏移长度 
    __ALIGN(4) uint8_t  p_sleep_head[4] = {0};      //头部信息数据
    __ALIGN(4) uint8_t  read_data_buf[8] = {0};     //获取睡眠曲线信息数据
    
    
    // !注意:这里需要做异常处理
    if(day_id >= 3 || sleep_curve_len == NULL)
    {	    
        return ;
    }
    
    sleep_curve_offset = SLEEP_CURVE_MIG_OFFSET;
    sleep_curve_index = sleep_curve_offset % 4 + 1;
    if(sleep_curve_offset & 0x03)   
    {   //不是4字节对齐
        sleep_curve_offset = sleep_curve_offset/4 * 4;
    }        
    
    sleep_curve_len = 0;
    
    // !注意:这里最多读取5端睡眠
    for(uint8_t index = 0; index < SLEEP_ONE_DAY_MAX_TIMES; index++)
    {
        ////Flash_CommonRead((uint32_t *)p_sleep_head, RECORD_SLEEP_DATA_ADDR(day_id, temp_offset), sizeof(p_sleep_head));
	    if(0 != Api_ReadSleepData(RECORD_SLEEP_DATA_ADDR(day_id, temp_offset), p_sleep_head, sizeof(p_sleep_head)))
        {//异常
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
        {//异常
            return ;
        }
        
        *sleep_curve_len += read_data_buf[sleep_curve_index] + (read_data_buf[sleep_curve_index + 1] << 8); 
        
        // 计算下一段睡眠的起始地址
        temp_offset += p_sleep_head[1] + (p_sleep_head[2] << 8);
        if(temp_offset % 4 != 0)
        {   //注：设计时已按照4字节对齐
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
    uint32_t    sleep_offset_buf[SLEEP_ONE_DAY_MAX_TIMES + 1] = {0};     //每段睡眠的偏移地址
    system_clock_time_t  sleep_entry_time = {0};
    
    // 获取前一天睡眠数据
    sleep_entry_time.year = clock->year;
    Api_GetSleepStoreInfo((block_id+2)%3, &sleep_wake_cnt, NULL, sleep_offset_buf);
    
    for(uint8_t index = 0; index < sleep_wake_cnt; index++)
    {
        ////Flash_CommonRead(sleep_data, RECORD_SLEEP_DATA_ADDR((block_id+2)%3, sleep_offset_buf[index] + 12), sizeof(sleep_data));
        if(0 != Api_ReadSleepData(RECORD_SLEEP_DATA_ADDR((block_id+2)%3, sleep_offset_buf[index] + 12), (uint8_t *)sleep_data, sizeof(sleep_data)))
        {//异常
            return 0;
        }
        
        // 更新睡眠日期时间
        sleep_entry_time.month = p_sleep_data[3];
        sleep_entry_time.day = p_sleep_data[4];
        diff_days = Get_DateDiffDays(&sleep_entry_time, clock);
        // 与当前时间进行对比(这里需要判断特殊时间)
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
** Descriptions:            获取睡眠时间，用于显示
** input parameters:        cur_data：当前时间；old_data：前段的时间
** output parameters:       无
** Returned value:          flase,true
** Created by:              WJ             
** Created Date:            2018-10-16
**----------------------------------------------------------------
*****************************************************************/ 
//static bool Sleep_Adjust(uint8_t *cur_data, uint8_t *old_data)
//{
//	bool days = false;
//	
//	int32_t days0 = Sclock_SeekYearDays(clock->year, cur_data[1], cur_data[2]);////计算当前段睡眠入睡时间所在天数
//	
//	int32_t days1 = Sclock_SeekYearDays(clock->year, cur_data[5], cur_data[6]);/////计算当前段睡眠苏醒时间所在天数
//	
//	int32_t days2 = Sclock_SeekYearDays(clock->year, clock->month, clock->day);/////当前时间所在天数
//	
//	int32_t days3 = Sclock_SeekYearDays(clock->year, old_data[5], old_data[6]);//////前一段睡眠苏醒时间所在天数
//	/*判断入睡和苏醒时间与当前时间是否接近*/
//	if(((days2 - days0 ==0 || (days2 - days0 ==1 && cur_data[3]>8) )/////入睡时间与当前时间有夸天现象必须大于8 
//      && days1 - days0 >=0 && days1 - days0 <=1 
//      && days2 - days1 >=0 && days2 - days1 <=1)
//      ||(cur_data[1] == 12 && cur_data[2] == 31
//      && clock->month ==1 && clock->day ==1))/////考虑跨年那天
//	{
//        if(days3 == 0)
//        {
//            days = true;
//        }
//        else if(days3 <= days0)////第二端睡眠的入睡天数>=第一段睡眠的苏醒天数
//        {
//            if(days0 - days3 == 0)
//            {
//                if(cur_data[3] * 60 + cur_data[4] > old_data[7] * 60 + old_data[8])/////判断睡眠重叠
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
** Descriptions:             获取睡眠时间，显示在今天
** input parameter:          得到睡眠时间
** Returned Value:           睡眠总时间(分钟) 
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

//显示今天的睡眠时间，过滤掉其他时间的睡眠
//cur_time:入睡：月日时分+退出：月日时分
//last_time:上一次合法的退出睡眠时间-月日时分
//yesterday_time:昨天的-月日
//system_time:系统时间-月日时分
//per_index:0,当天；1，昨天
static bool Sleep_TimeCmp(uint8_t *cur_time, uint8_t *last_time, uint8_t *yesterday_time,
                          uint8_t *sys_time, uint8_t per_index)
{
	bool cmp_flag = false;
	
    //用于比较存储在昨天的睡眠数据
    if(per_index == 1)
    {
        //比较每段睡眠的时间，入睡时间>昨天20点，或者退出睡眠时间为今天，计入今天显示时
        if(cur_time[0] == yesterday_time[0] && cur_time[1] == yesterday_time[1])
        {   //是存储在昨天的睡眠数据,且入睡时间在昨天
            if((cur_time[2]*60 + cur_time[3]) > 20*60 ) //20点以后入睡
            {
                cmp_flag = true;
            }
            else
            {   //20点以前入睡
                if(cur_time[5] != yesterday_time[1])    //退出睡眠在第二天，<<系统时间当天>>
                {
                    cmp_flag = true;
                }
            }
        }
        else if(cur_time[0] == sys_time[0] && cur_time[1] == sys_time[1])
        {   //是存储在昨天的睡眠数据,且入睡时间在今天
            if((cur_time[2]*60 + cur_time[3]) < 8*60 ) //8点以前入睡
            {
                cmp_flag = true;
            }          
        }
    }
    
    //用于比较存储在今天的睡眠数据
    if(per_index == 0)
    {
        //比较每段睡眠的时间，入睡时间<20点且>=8点，且退出睡眠时间为今天，计入今天显示时间
        if(cur_time[0] == sys_time[0] && cur_time[1] == sys_time[1] 
           && (cur_time[2] >= 8 && (cur_time[2]*60 + cur_time[3]) <= 20*60))
        {
            if((cur_time[6]*60 + cur_time[7]) < 24*60)
            {   //退出睡眠在今天
                cmp_flag = true;
            }
        }
    }
    
    return cmp_flag;
}

/************************************************************************
** Function name:            Api_GetSleepTotalTimeNew
** Descriptions:             获取睡眠时间，显示在今天
** input parameter:          得到睡眠时间
                             last_sleep_time:退出睡眠的最后一组的时间
** Returned Value:           睡眠总时间(分钟) 
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
    uint8_t     max_sleep_quality = 0;      // 睡眠质量
    uint16_t    sleep_total_time = 0;       // 睡眠总时长
    uint16_t    c_sleep_shallow_time = 0;   // 浅睡时长
    uint16_t    c_sleep_deep_time = 0;      // 深睡时长
    uint32_t    sleep_data[5] = {0};
    uint8_t     *p_sleep_data = (uint8_t*)sleep_data;
    uint8_t     sleep_new_time[12] = {0x00, };     
    uint8_t     sleep_old_time[12] = {0x00, }; 
    uint32_t    sleep_offset_buf[SLEEP_ONE_DAY_MAX_TIMES + 1] = {0};     //每段睡眠的偏移地址

    uint8_t     sys_time[4] = {0};
    uint8_t     yesterday_time[4] = {0};
    
    
    //获取昨天的时间
    sys_time[0] = clock->year & 0xFF;    
    sys_time[1] = (clock->year>>8) & 0xFF;
    sys_time[2] = clock->month & 0xFF;
    sys_time[3] = clock->day & 0xFF;
    Util_GetPreviousDayDate(sys_time, 1, yesterday_time);
    
    //获取昨天存储睡眠的存储块
    block_index = (p_flash_param->flash_index_s.block_index + 2)%3;
    //获取昨天存储每段睡眠时间和睡眠时长
    //比较每段睡眠的时间，入睡时间>昨天20点，或者退出睡眠时间为今天，计入今天显示时
    Api_GetSleepStoreInfo(block_index, &sleep_wake_cnt, NULL, sleep_offset_buf);    
    for(uint8_t index = 0; index < sleep_wake_cnt; index++)
    {
        memset(p_sleep_data, 0x00 , sizeof(sleep_data));
        ////Flash_CommonRead(sleep_data, RECORD_SLEEP_DATA_ADDR(block_index, sleep_offset_buf[index] + 12), sizeof(sleep_data));
        if(0 != Api_ReadSleepData(RECORD_SLEEP_DATA_ADDR(block_index, sleep_offset_buf[index] + 12), (uint8_t *)sleep_data, sizeof(sleep_data)))
        {//异常
            return 0;
        }
        
        memcpy(sleep_new_time, (void *)&p_sleep_data[3], 8);
        if(Sleep_TimeCmp(sleep_new_time, sleep_old_time, &yesterday_time[2], &sys_time[2], 1))
        {
            memcpy(sleep_old_time, (void *)&p_sleep_data[7], 4);
            //读取睡眠时长，总时间偏移量40    
            if(Api_GetSleepContinueNextFlag(block_index, index + 1) != 1)
            {
                ////Flash_CommonRead(sleep_data, RECORD_SLEEP_DATA_ADDR(block_index, sleep_offset_buf[index] + 40), sizeof(sleep_data));   
                if(0 != Api_ReadSleepData(RECORD_SLEEP_DATA_ADDR(block_index, sleep_offset_buf[index] + 28), (uint8_t *)sleep_data, sizeof(sleep_data)))
                {//异常
                    return 0;
                }    
                sleep_total_time += p_sleep_data[12] + (p_sleep_data[13] << 8);
                c_sleep_shallow_time += p_sleep_data[8] + (p_sleep_data[9] << 8);   // 浅睡时长
                c_sleep_deep_time += p_sleep_data[6] + (p_sleep_data[7] << 8);      // 深睡时长
                
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
    
    //获取今天的睡眠存储块
    block_index = p_flash_param->flash_index_s.block_index%3;  
    //比较每段睡眠的时间，入睡时间<20点，且退出睡眠时间为今天，计入今天显示时间    
    Api_GetSleepStoreInfo(block_index, &sleep_wake_cnt, NULL, sleep_offset_buf);    
    for(uint8_t index = 0; index < sleep_wake_cnt; index++)
    {
        memset(p_sleep_data, 0x00 , sizeof(sleep_data));
        /////Flash_CommonRead(sleep_data, RECORD_SLEEP_DATA_ADDR(block_index, sleep_offset_buf[index] + 12), sizeof(sleep_data));
        if(0 != Api_ReadSleepData(RECORD_SLEEP_DATA_ADDR(block_index, sleep_offset_buf[index] + 12), (uint8_t *)sleep_data, sizeof(sleep_data)))
        {//异常
            return 0;
        }
            
        memcpy(sleep_new_time, (void *)&p_sleep_data[3], 8);
        
        if(Sleep_TimeCmp(sleep_new_time, sleep_old_time, &yesterday_time[2], &sys_time[2], 0))
        {
            memcpy(sleep_old_time, (void *)&p_sleep_data[7], 4);
            //读取睡眠时长，总时间偏移量40   

            // 判断当前段睡眠是否下链接
            if(Api_GetSleepContinueNextFlag(block_index, index + 1) != 1)
            {            
                ////Flash_CommonRead(sleep_data, RECORD_SLEEP_DATA_ADDR(block_index, sleep_offset_buf[index] + 40), sizeof(sleep_data));            
                if(0 != Api_ReadSleepData(RECORD_SLEEP_DATA_ADDR(block_index, sleep_offset_buf[index] + 28), (uint8_t *)sleep_data, sizeof(sleep_data)))
                {//异常
                    return 0;
                }
                sleep_total_time += p_sleep_data[12] + (p_sleep_data[13] << 8);
                c_sleep_shallow_time += p_sleep_data[8] + (p_sleep_data[9] << 8);   // 浅睡时长
                c_sleep_deep_time += p_sleep_data[6] + (p_sleep_data[7] << 8);      // 深睡时长
                
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
** Descriptions:             根据偏移量获取睡眠数据
** input parameter:          day_id:那天的数据；offset:偏移量;data_len:获取数据长度
** output parameter:         p_data：获取返回数据
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
        {//异常
            return 0;
        }
        
        memcpy(p_data, part_data, data_len);
    }
    return NRF_SUCCESS;
}

/************************************************************************
** Function name:        Api_SaveSystemTime
** Descriptions:         存储系统时间
** input parameter:      日期数据   
** Returned Value:       NRF_SUCCESS
** Created  by:          郭涛
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
    // 1、判断时间模块是否写满 
    if(flash_param_s.systemtime_index >= 512)
    {
        flash_param_s.systemtime_index = 0;
        //擦除该FLASH块
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
** Descriptions:            读系统时间  
** input parameter:         pdata:日期数据记录   
** Returned Value:          NRF_SUCCESS
** Created  by:             郭涛
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
** Descriptions:         更新心率报警参数  
** input parameter:      p_alert：心率参数  
** Returned Value:       NRF_SUCCESS
** Created  by:          郭涛
** Created  Datas:       2017-08-04
**-----------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
**************************************************************************/
uint32_t Api_UpdateHeartAlertParam(const heart_alert_config_t* p_alert)
{
    //更新内存数据
    flash_param_s.heart_alert_s.enable = p_alert->enable;
    flash_param_s.heart_alert_s.high = p_alert->high;
    flash_param_s.heart_alert_s.low = p_alert->low;
	
    BleThread_AsynUpdateAppConfig();
    
    return NRF_SUCCESS;
}

/************************************************************************
** Function name:           Api_UpdateTurnWristParam
** Descriptions:            更新转手腕参数  
** input parameter:         P_turnwrist:数据指针     
** Returned Value:          NRF_SUCCESS
** Created  by:             郭涛
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
** Descriptions:           更新血氧参数   
** input parameter:        P_config:参数指针    
** Returned Value:         NRF_SUCCESS
** Created  by:            郭涛
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
** Descriptions:           更新信息设置  
** input parameter:        p_enable:数据指针     
** Returned Value:         NRF_SUCCESS
** Created  by:            郭涛
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
** Descriptions:           血液模式  
** input parameter:        无
** Returned Value:         模式
** Created  by:            郭涛
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
** Descriptions:           更新设备参数
** input parameter:        p_dev_config_t: 设备参数 
** Returned Value:         NRF_SUCCESS
** Created  by:            郭涛
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
** Descriptions:           跟新久坐设置参数  
** input parameter:        P_sedentary_config_t:久坐数据指针
** Returned Value:         NRF_SUCCESS
** Created  by:            郭涛
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
** input parameter:        语言
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
** Descriptions:           更新密码  
** input parameter:        password:密码
** Returned Value:         NRF_SUCCESS
** Created  by:            郭涛
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
** Descriptions:           index:闹钟索引  p_alarm:闹钟参数指针   
** input parameter:        password:密码
** Returned Value:         NRF_SUCCESS
** Created  by:            郭涛
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
** Descriptions:           index:闹钟索引  p_alarm:闹钟参数指针  
** input parameter:        password:密码
** Returned Value:         闹钟参数
** Created  by:            郭涛
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
** Descriptions:           更新亮度数据  
** input parameter:        p_bright:亮度数据指针
** Returned Value:         NRF_SUCCESS
** Created  by:            郭涛
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
** Descriptions:           更新用户信息 
** input parameter:        p_person:个人信息数据指针
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
        flash_param_s.Remind_flag = 0;//重新设定目标，则清零
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
** Descriptions:           更新血氧参数数据  
** input parameter:        p_param:参数指针
** Returned Value:         NRF_SUCCESS
** Created  by:            郭涛
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
** Descriptions:           得到设备SN  
** input parameter:        无
** Returned Value:         设备id
** Created  by:            郭涛
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
** Descriptions:           设备sn检测  
** input parameter:        无
** Returned Value:         无
** Created  by:            郭涛
** Created  Datas:         2017-08-04
**-----------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
**************************************************************************/
void Api_DevSNCheck(void)
{
    //WJ.20190103.修改位置后，检查前必须读取
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
** Descriptions:           更新样式  
** input parameter:        ui_style:样式
** Returned Value:         NRF_SUCCESS
** Created  by:            郭涛
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
** Descriptions:           获取主界面风格 
** input parameter:        无
** Returned Value:         风格样式
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
** Descriptions:           更新UI显示保存时间
** input parameter:        ui_display_time:UI显示保存时间
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
** Descriptions:           获取UI显示保存时间
** input parameter:        ui_display_time:UI显示保存时间
** Returned Value:         显示时长
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
** Descriptions:         存储 ECG手动测量结果数据
** input parameter:      p_ecg_data: ECG手动测量结果数据   
** Returned Value:       无
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
        {   //擦除第一块
            Record_ClearEcgResultBlock(0, 1);
            p_flash_index->ecg.offset_start_index = RECORD_ECG_ONE_BLOCK_MAX_CNT + 1;
        }
        else if(p_flash_index->ecg.offset_end_index == RECORD_ECG_ONE_BLOCK_MAX_CNT)
        {   //擦除第二块
            Record_ClearEcgResultBlock(1, 1); 
            p_flash_index->ecg.offset_start_index = 0;
        }
    }
}

/************************************************************************
** Function name:        Api_GetEcgAttrData
** Descriptions:         获取ECG手动测量结果数据
** input parameter:      offset: 数据偏移量   
** output parameter:     p_ecg_data: ECG手动测量结果数据   
** Returned Value:       无
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
        {   //超时，放弃处理
            return;
        }  
    }
    while(Extflash_EcgRestReadIsComplete() == 0)
    {
        vTaskDelay(1);
        read_time_out++;
        if(read_time_out > 1200)
        {   //超时，放弃处理
            return;
        }
    }
}

