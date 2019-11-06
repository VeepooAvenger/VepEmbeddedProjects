/*************************************Copyright(c)***************************************************
**                                     Veepoo Tech
**                  
**                               COPYRIGHT(C)  2016-2019
**--------------------------------------FileInfo----------------------------------------------------
** FileName:                           Flash_record_data.c
** Last Modify Data:                    2017-08-04
** Last SYS_VERSION:                    XXXXXXXXX
** Descriptions:                     
**---------------------------------------------------------------------------------------------------
** CreatedBy:                              GT
** CreateData:                          2017-06-xx
** SystemVersion:             
** Description:     the file's  funtion  realize some mode's init , read , write api
**---------------------------------------------------------------------------------------------------
** Modify By:                       
** Modify Data:
** System Version:
** Modify Description:    

******************************************************************************************************/
#include <string.h>
#include <stdlib.h>
#include "flash_record_data.h" 
#include "flash_common.h"
#include "nrf_error.h"
#include "log.h"

#include "ext_flash.h"

#include "watch_dog.h"

/************************************************************************
** Function name:           Get_MaxTimeIndex
** Descriptions:            最大时间索引
** input parameter:         p_data: 数据指针  len :长度
** Returned Value:
** Created  by:             郭涛
** Created  Datas:          2017-08-04
**-----------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
**************************************************************************/
static uint8_t Get_MaxTimeIndex(uint32_t* p_data, uint8_t len)
{
    uint8_t index = 0;
    uint32_t Max = 0;
	uint32_t clock_data = 0;
    
    for(uint8_t i = 0; i < len; i++)
    {
        if(Max < p_data[i])
        {
            Max = p_data[i];
            index = i;
        }
    }
	//YX.20180123.解决对时清除数据后，马上进行升级造成日常数据存储快出错的问题
	clock_data = (clock->month%100)*100+clock->day%100;
	Max = Max / 10000;
	if(clock_data != Max)
	{
		index = (index + 1)% 3;
        //WJ.20190103.跨年设备复位导致找存储块错误的问题。目前只有在跨后的低2天存在，即1月2号  
        for(uint8_t i = 0; i < len; i++)
        {
            if(clock_data == (p_data[i] / 10000))
            {
                index = i;
                break;
            }
        }
	}
       
    return index;
}

/************************************************************************
** Function name:                 Record_LogDataIndexInit
** Descriptions:                  日志数据索引初始化 
** input parameter:               p_data: 数据指针 len :长度
** Returned Value:
** Created  by:                   郭涛
** Created  Datas:                2017-08-04
**-----------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
**************************************************************************/
void Record_LogDataIndexInit(log_data_index_t* p_index)
{
    static uint16_t offset = 0;
    uint32_t record_data[16] = {0};
    
    for( offset = 0; offset < LOG_DATA_SIZE; offset += 64)
    {
        Flash_CommonRead(record_data, LOG_DATA_ADDR(offset), 64);
        // !注意:这里需要判断如果全是FFFF, 则跳出
        if((0xFFFFFFFF == record_data[0])&&(0xFFFFFFFF == record_data[1]))
        {
            p_index->offset = offset/64;
            break;
        }
        else
        {
            p_index->total_cnt++;
        }
    }
}

void Record_BlockIndexInit(uint8_t*  p_index)
{
    uint32_t record_data[RECORD_DAILY_PER_DATA_SIZE / 4 + 1] = {0};
    uint32_t record_time[3] = {0};
    daily_data_attr_t*  p_attr = (daily_data_attr_t*)record_data;
    
    // 1、计算五分钟数据索引
    for(uint8_t index = 0; index < 3; index++)
    {
        for(uint16_t offset = 0; offset < RECORD_DAILY_DATA_SIZE; offset += RECORD_DAILY_PER_DATA_SIZE)
        {
            memset(record_data, 0, sizeof(record_data));
            Record_GetDailyData(index, offset / RECORD_DAILY_PER_DATA_SIZE, p_attr);
            // 这里判断心率数据是否有效, 判断年月日即可
            if((p_attr->month != 0)&&(p_attr->month <= 12)&&(p_attr->day != 0)&&(p_attr->day <= 31))
            {
                // !注意:这里将整数转换为字符数据, 如:08-20 10:30, 转换后为:08201030 Add by GT 2017-07-28
                record_time[index] = (((p_attr->month%100)*100+p_attr->day%100)*100+p_attr->hour%100)*100+p_attr->minute%100;
            }
            else
            {
                // !注意:这里需要判断如果全是FFFF, 则跳出
                if((record_data[0] == 0xFFFFFFFF) && (record_data[1] == 0xFFFFFFFF))
                {
                    break;
                }
            }
        }
    } 

    *p_index =  Get_MaxTimeIndex(record_time, 3);   
}

/************************************************************************
** Function name:                      Record_DailyDataIndexInit
** Descriptions:                       日常数据初始化
** input parameter:                    p_index: 日常数据
** Returned Value:
** Created  by:                        郭涛
** Created  Datas:                     2017-08-04
**-----------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
**************************************************************************/
void Record_DailyDataIndexInit(daily_data_index_t* p_index)
{
    uint32_t record_data[RECORD_DAILY_PER_DATA_SIZE / 4 + 1] = {0};
    daily_data_attr_t*  p_attr = (daily_data_attr_t*)record_data;
    // 1、计算五分钟数据索引
    for(uint8_t index = 0; index < 3; index++)
    {
        for(uint32_t offset = 0; offset < RECORD_DAILY_DATA_SIZE; offset += RECORD_DAILY_PER_DATA_SIZE)
        {
            memset(record_data, 0, sizeof(record_data));
            Record_GetDailyData(index, offset / RECORD_DAILY_PER_DATA_SIZE, p_attr);
            // 这里判断心率数据是否有效, 判断年月日即可
            if((p_attr->month != 0)&&(p_attr->month <= 12)&&(p_attr->day != 0)&&(p_attr->day <= 31))
            {
                p_index->step_cnt[index] += p_attr->step;
                p_index->distance_cnt[index] += p_attr->distance;
                p_index->calories_cnt[index] += p_attr->calories;
                
                // 获取复位记录
                if(p_attr->resv[0] != 0)
                {
                    p_index->rest_cnt[index]++;
                }
            }
            
            // !注意:这里需要判断如果全是FFFF, 则跳出
            if((record_data[0] == 0xFFFFFFFF)&&(record_data[1] == 0xFFFFFFFF))
            {
                 p_index->offset[index] = offset / RECORD_DAILY_PER_DATA_SIZE;
                 break;
            }
        }
        App_Watch_Dog_Reload();
    }
}

/************************************************************************
** Function name:                     Record_ClearAllData
** Descriptions:                      擦除所有数据
** input parameter:                   无
** Returned Value:
** Created  by:                       郭涛
** Created  Datas:                    2017-08-04
**-----------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
**************************************************************************/
//void Record_ClearAllData(void)
//{
//    Flash_CommonErase((uint32_t*)RECORD_DAILY_BASE_ADDR, 24);//18); 
//    //睡眠数据一起擦除，上面已包含,睡眠数据和日常数据块是连续的
//    /*
//    Flash_CommonErase((uint32_t*)RECORD_DAILY_BASE_ADDR, RECORD_DAILY_TOTAL_SIZE/4096);
//    Flash_CommonErase((uint32_t*)RECORD_SLEEP_BASE_ADDR, RECORD_SLEEP_TOTAL_SIZE/4096); 
//    */
//}

/************************************************************************
** Function name:                           Record_ClearOneDayData
** Descriptions:                            擦除天数据
** input parameter:                         日期id
** Returned Value:
** Created  by:                             郭涛
** Created  Datas:                          2017-08-04
**-----------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
**************************************************************************/
void Record_ClearOneDayData(uint8_t day_id)
{
    uint16_t start_sector = 0;
    uint16_t block_num = 0;
    
    day_id = day_id % 3;

    //Flash_CommonErase((uint32_t*)RECORD_DAILY_DATA_ADDR(day_id, 0), RECORD_DAILY_ONE_DAY_SIZE/4096);
    start_sector = RECORD_DAILY_DATA_ADDR(day_id, 0)/4096;
    block_num = RECORD_DAILY_ONE_DAY_SIZE/4096;
    for(uint8_t i = 0; i < block_num; i++)
    {
        Extflash_DataSectorErase4K(start_sector + i);
        // WJ.20190216.时间过长
        App_Watch_Dog_Reload();
    }
    
    start_sector = RECORD_SLEEP_DATA_ADDR(day_id, 0)/4096;
    block_num = RECORD_SLEEP_DATA_SIZE/4096;
    for(uint8_t i = 0; i < block_num; i++)
    {
        Extflash_DataSectorErase4K(start_sector + i);
        // WJ.20190216.时间过长
        App_Watch_Dog_Reload();
    }
    
    ////Flash_CommonErase((uint32_t*)RECORD_DAILY_DATA_ADDR(day_id, 0), RECORD_DAILY_ONE_DAY_SIZE/4096);
    
    //睡眠数据一起擦除
    ////Flash_CommonErase((uint32_t*)RECORD_SLEEP_DATA_ADDR(day_id, 0), RECORD_SLEEP_DATA_SIZE/4096);    
}

/************************************************************************
** Function name:                Record_AddDailyData
** Descriptions:                 增加天数
** input parameter:              day_id:天数索引  offset:地址索引
                                 p_attr:日常数据
** Returned Value:
** Created  by:                  郭涛
** Created  Datas:               2017-08-04
**-----------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
**************************************************************************/
uint32_t Record_AddDailyData(uint8_t day_id, uint16_t offset, const daily_data_attr_t* p_attr)
{
    uint32_t daily_data[RECORD_DAILY_PER_DATA_SIZE / 4] = {0};
    uint8_t* p_daily_data = (uint8_t*)daily_data;

    // 如果条件不合法, 则为无效参数 Add by GT 2017-07-28
    if(!RECORD_DAILY_CONDITION_VALID(day_id, offset))
    {
       return NRF_ERROR_INVALID_PARAM;
    }
	
    memcpy(daily_data, p_attr, sizeof(daily_data_attr_t));
    
//    // GT 2017-08-22 FLASH存储和CPU大小端模式刚好相反, 需要处理
//    p_daily_data[4] = (uint8_t)((p_attr->step>>8)&0xFF);
//    p_daily_data[5] = (uint8_t)(p_attr->step&0xFF);
//    p_daily_data[6] = (uint8_t)((p_attr->sport>>8)&0xFF);
//    p_daily_data[7] = (uint8_t)(p_attr->sport&0xFF);
//    p_daily_data[8] = (uint8_t)((p_attr->distance>>8)&0xFF);
//    p_daily_data[9] = (uint8_t)(p_attr->distance&0xFF);
//    p_daily_data[10] = (uint8_t)((p_attr->calories>>8)&0xFF);
//    p_daily_data[11] = (uint8_t)(p_attr->calories&0xFF);
	
    Extflash_DataWrite(RECORD_DAILY_DATA_ADDR(day_id, offset), RECORD_DAILY_PER_DATA_SIZE, p_daily_data);
//    Flash_CommonStore((uint32_t*)RECORD_DAILY_DATA_ADDR(day_id, offset), daily_data, RECORD_DAILY_PER_DATA_SIZE / 4);
	
    return NRF_SUCCESS;
}

/************************************************************************
** Function name:            Record_GetDailyData
** Descriptions:             从flash数据
** input parameter:          day_id:数据天数索引 offset: 偏移量
                             p_attr:日常数据指针
** Returned Value:
** Created  by:              郭涛
** Created  Datas:           2017-08-04
**-----------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
**************************************************************************/
uint32_t Record_GetDailyData(uint8_t day_id, uint16_t offset, daily_data_attr_t* p_attr)
{
    uint32_t daily_data[RECORD_DAILY_PER_DATA_SIZE / 4 + 1] = {0};
    uint8_t  *p_daily_data = (uint8_t*)daily_data;

    // 如果条件不合法, 则为无效参数 Add by GT 2017-07-28
    if(!RECORD_DAILY_CONDITION_VALID(day_id, offset))
    {
        return NRF_ERROR_INVALID_PARAM;
    }
    
    Extflash_DataRead(RECORD_DAILY_DATA_ADDR(day_id, offset), RECORD_DAILY_PER_DATA_SIZE, p_daily_data);
//    Flash_CommonRead(daily_data, RECORD_DAILY_DATA_ADDR(day_id, offset), RECORD_DAILY_PER_DATA_SIZE);
    
    memcpy(p_attr, daily_data, sizeof(daily_data_attr_t));
    
//    // !!!注意:这里有高低位的区分
//    p_attr->step  = (uint16_t)((p_daily_data[4]<<8)|(p_daily_data[5]));
//    p_attr->sport = (uint16_t)((p_daily_data[6]<<8)|(p_daily_data[7]));
//    p_attr->distance = (uint16_t)((p_daily_data[8]<<8)|(p_daily_data[9]));
//    p_attr->calories = (uint16_t)((p_daily_data[10]<<8)|(p_daily_data[11]));
    
    return NRF_SUCCESS;
}

/************************************************************************
** Function name:        Record_GetSystemTimeIndex
** Descriptions:         取得系统时间索引
** input parameter:      p_index:the index to store
                    
** Returned Value:
** Created  by:         郭涛
** Created  Datas:      2017-08-04
**-----------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
**************************************************************************/
void Record_GetSystemTimeIndex(uint16_t* p_index)
{
    uint32_t system_time[2] = {0};
				
    *p_index = 0xFFFF;
    for(uint16_t offset = 0; offset < 512; offset++)
    {
        Flash_CommonRead(system_time, SYSTEM_TIME_BASE_ADDR+offset*8, sizeof(system_time));
        if((0xFFFFFFFF == system_time[0])&&(0xFFFFFFFF == system_time[1]))
        {
            *p_index = offset;
            break;
        }
    }
}

/************************************************************************
** Function name:                  Record_GetAlarmTimeParam
** Descriptions:                   得到闹钟参数
** input parameter:                index:索引   p_alarm:指针数据
                    
** Returned Value:
** Created  by:                    郭涛
** Created  Datas:                 2017-08-04
**-----------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
**************************************************************************/
#if (!APP_USED_OLD_ALARM_ENABLE)
void Record_GetAlarmTimeParam(uint8_t index, alarm_time_config_t* p_alarm)
{
    uint32_t alarm_param[4] = {0};
    uint8_t* p_alarm_param = (uint8_t*)alarm_param;
				
    Flash_CommonRead(alarm_param, ALARM_CONFIG_BASE_ADDR+index*16, sizeof(alarm_param));
    if((0xFF != p_alarm_param[0])&&(0xFF != p_alarm_param[1]))
    {
        p_alarm->hour   = p_alarm_param[0]&0x1F;  
        p_alarm->minute = p_alarm_param[1]&0x3F;
        p_alarm->state  = p_alarm_param[2]&0x01;
        p_alarm->repeat = p_alarm_param[3]&0x7F;
        p_alarm->scence = p_alarm_param[4]&0x1F;
        p_alarm->year   = p_alarm_param[5]|(p_alarm_param[6]<<8);
        p_alarm->month  = p_alarm_param[7]&0x0F;
        p_alarm->day    = p_alarm_param[8]&0x1F;
        p_alarm->valid  = p_alarm_param[15]&0x01;
    }
    else
    {
        memset((uint8_t*)p_alarm, 0, sizeof(p_alarm));
    }
}
#else
void Record_GetOldAlarmTimeParam(uint8_t *p_old_alarm)
{
    uint8_t  old_alarm_flag = 0;
    uint32_t alarm_param[4] = {0};
    uint8_t* p_alarm_param = (uint8_t*)alarm_param;
				
    Flash_CommonRead(alarm_param, ALARM_CONFIG_BASE_ADDR+0*16, sizeof(alarm_param));

    if((p_alarm_param[0] < 24) && (p_alarm_param[1] < 60))
    {        
        p_old_alarm[0] = p_alarm_param[0];//时
        p_old_alarm[1] = p_alarm_param[1];//分
        p_old_alarm[2] = p_alarm_param[2];//振动方式  
        old_alarm_flag = 1;
    }
    if((p_alarm_param[3] < 24) && (p_alarm_param[4] < 60))
    {
        p_old_alarm[3] = p_alarm_param[3];//时
        p_old_alarm[4] = p_alarm_param[4];//分
        p_old_alarm[5] = p_alarm_param[5];//振动方式 
        old_alarm_flag = 1;
    }
    
    if((p_alarm_param[6] < 24) && (p_alarm_param[7] < 60))
    {
        p_old_alarm[6] = p_alarm_param[6];//时
        p_old_alarm[7] = p_alarm_param[7];//分
        p_old_alarm[8] = p_alarm_param[8];//振动方式 
        old_alarm_flag = 1;
    }            
    
    if(old_alarm_flag != 1)
    {
        memset((uint8_t*)p_old_alarm, 0, 9);
    }
}
#endif // APP_USED_OLD_ALARM_ENABLE

/************************************************************************
** Function name:                      Record_UpdateAlarmTimeParam
** Descriptions:                       更新闹钟参数
** input parameter:                    index:索引  p_alarm:数据指针
                    
** Returned Value:
** Created  by:                        郭涛
** Created  Datas:                     2017-08-04
**-----------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
**************************************************************************/
#if (!APP_USED_OLD_ALARM_ENABLE)
void Record_UpdateAlarmTimeParam(uint8_t index, const alarm_time_config_t* p_alarm)
{
    uint32_t alarm_param[4] = {0};
    uint8_t* p_alarm_param = (uint8_t*)alarm_param;
   
    p_alarm_param[0]  = p_alarm->hour;
    p_alarm_param[1]  = p_alarm->minute;
    p_alarm_param[2]  = p_alarm->state;
    p_alarm_param[3]  = p_alarm->repeat;
    p_alarm_param[4]  = p_alarm->scence;
    p_alarm_param[5]  = p_alarm->year&0xFF;
    p_alarm_param[6]  = p_alarm->year>>8;
    p_alarm_param[7]  = p_alarm->month;
    p_alarm_param[8]  = p_alarm->day;
    p_alarm_param[15] = p_alarm->valid;
				
    Flash_CommonStore((uint32_t*)(ALARM_CONFIG_BASE_ADDR+index*16), alarm_param, 4);
}
#else
void Record_UpdateOldAlarmTimeParam(const uint8_t *p_old_alarm)
{
    uint32_t alarm_param[4] = {0};
    uint8_t *p_alarm_param = (uint8_t*)alarm_param;
   
    memcpy(p_alarm_param, p_old_alarm, 9);
				
    Flash_CommonStore((uint32_t*)(ALARM_CONFIG_BASE_ADDR+0*16), alarm_param, 4);
}
#endif //APP_USED_OLD_ALARM_ENABLE

/************************************************************************
** Function name:                      Record_AddLogData
** Descriptions:                       增加log数据 
** input parameter:                    offset:数据索引 p_attr:数据指针
** Returned Value:
** Created  by:                        郭涛
** Created  Datas:                     2017-08-04
**-----------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
**************************************************************************/
uint32_t Record_AddLogData(uint16_t offset, const log_data_attr_t* p_attr)
{
    uint32_t log_data[16] = {0};
       
    if(!LOG_CONDITION_VALID(offset))
         return NRF_ERROR_INVALID_PARAM;
   
    memcpy(log_data, p_attr, 64);
    Flash_CommonStore((uint32_t*)LOG_DATA_ADDR(offset*64), log_data, 16);
       
    return NRF_SUCCESS;
}

/************************************************************************
** Function name:                      Record_GetLogData
** Descriptions:                       得到日常数据
** input parameter:                    offset:偏移量  p_attr:数据指针
                    
** Returned Value:
** Created  by:                        郭涛
** Created  Datas:                     2017-08-04
**-----------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
**************************************************************************/
uint32_t Record_GetLogData(uint16_t offset, log_data_attr_t* p_attr)
{
    uint32_t log_data[16] = {0};
    
    if(!LOG_CONDITION_VALID(offset))
    {
        return NRF_ERROR_INVALID_PARAM;
    }
    
    Flash_CommonRead(log_data, LOG_DATA_ADDR(offset*64), sizeof(log_data));
    memcpy(p_attr, log_data, sizeof(log_data));
    
    return NRF_SUCCESS;
}

/************************************************************************
** Function name:                Record_AddEcgResultData
** Descriptions:                 存储ECG 测量分析结果
** input parameter:              offset：地址索引
                                 p_ecg_data：ecg数据
** Returned Value:                  
** Created  by:                  WJ
** Created  Datas:               2018-10-01
**************************************************************************/
uint32_t Record_AddEcgResultData(uint16_t offset, const ecg_result_data_t *p_ecg_data)
{
//    uint32_t    ecg_result_data[RECORD_ECG_PER_DATA_SIZE / 4 + 1] = {0};
//    uint8_t     *p_ecg_result_data = (uint8_t*)ecg_result_data;

    // 如果条件不合法, 则为无效参数,放弃保存
    if(!RECORD_ECG_CONDITION_VALID(offset))
    {
       return NRF_ERROR_INVALID_PARAM;
    }
//    memcpy(p_ecg_result_data, (void *)p_ecg_data, RECORD_ECG_PER_DATA_SIZE);
//    Flash_CommonStore((uint32_t *)RECORD_ECG_DATA_ADDR(offset), ecg_result_data, RECORD_ECG_PER_DATA_SIZE / 4);
	
    return !Extflash_DataWrite(RECORD_ECG_DATA_ADDR(offset), RECORD_ECG_PER_DATA_SIZE, (uint8_t *)p_ecg_data);
}

/************************************************************************
** Function name:                Record_GetEcgResultData
** Descriptions:                 读取ECG 测量分析结果
** input parameter:              offset：地址索引
                                 p_ecg_data：ecg数据
** Returned Value:                  
** Created  by:                  WJ
** Created  Datas:               2018-10-01
**************************************************************************/
uint32_t Record_GetEcgResultData(uint16_t offset, ecg_result_data_t *p_ecg_data)
{
//    uint32_t    ecg_result_data[RECORD_ECG_PER_DATA_SIZE / 4 + 1] = {0};

    // 如果条件不合法, 则为无效参数,放弃保存
    if(!RECORD_ECG_CONDITION_VALID(offset))
    {
       return NRF_ERROR_INVALID_PARAM;
    }
    
//    Flash_CommonRead(ecg_result_data, RECORD_ECG_DATA_ADDR(offset), RECORD_ECG_PER_DATA_SIZE);	
//    memcpy(p_ecg_data, (void *)ecg_result_data, sizeof(ecg_result_data_t));
    
    return !Extflash_DataRead(RECORD_ECG_DATA_ADDR(offset), RECORD_ECG_PER_DATA_SIZE, (uint8_t *)p_ecg_data);
}

/************************************************************************
** Function name:                Record_ClearEcgResultBlock
** Descriptions:                 擦除ECG 数据块
** input parameter:              block_id：起始块
                                 block_num：擦除块数
** Returned Value:                  
** Created  by:                  WJ
** Created  Datas:               2018-10-01
**************************************************************************/
uint32_t Record_ClearEcgResultBlock(uint8_t block_id, uint8_t block_num)
{
    uint16_t start_sector = 0;
    // 如果条件不合法, 则为无效参数,放弃
    if(block_id + block_num > 2 || block_num == 0)
    {
       return NRF_ERROR_INVALID_PARAM;
    }
    
    start_sector = RECORD_ECG_DATA_ERASE_ADDR(block_id)/4096;
    for(uint8_t i = 0; i < block_num; i++)
    {
        Extflash_DataSectorErase4K(start_sector + i);
        // WJ.20190216.时间过长
        App_Watch_Dog_Reload();
    }
//    Flash_CommonErase((uint32_t*)RECORD_ECG_DATA_ERASE_ADDR(block_id), block_num);
    
    return NRF_SUCCESS;
}


/************************************************************************
** Function name:                      Record_EcgDataIndexInit
** Descriptions:                       ECG数据存储索引初始化
** input parameter:                    无
** output parameter:                   p_index: ECG存储索引
** Returned Value:                     无
** Created  by:                        WJ
** Created  Datas:                     2018-11-05
**************************************************************************/
void Record_EcgDataIndexInit(ecg_data_index_t *p_index)
{
    uint16_t    offset = 0;
    uint32_t    record_data[RECORD_ECG_PER_DATA_SIZE / 4 + 1] = {0};
    ecg_result_data_t   *p_attr = (ecg_result_data_t *)record_data;
 
    memset(record_data, 0, sizeof(record_data));
    Record_GetEcgResultData(0, p_attr);
    if(record_data[0] != 0xFFFFFFFF)
    {
        memset(record_data, 0, sizeof(record_data));
        Record_GetEcgResultData(RECORD_ECG_ONE_BLOCK_MAX_CNT, p_attr);
        if(record_data[0] != 0xFFFFFFFF)
        {
            memset(record_data, 0, sizeof(record_data));
            Record_GetEcgResultData(RECORD_ECG_MAX_CNT - 1, p_attr);
            if(record_data[0] != 0xFFFFFFFF)
            {
                //擦除2块 
                Record_ClearEcgResultBlock(0, 2);
                p_index->offset_start_index = 0;
                p_index->offset_end_index = 0;
                return;
            }
        }
    }
    
    for(offset = 0; offset < RECORD_ECG_MAX_CNT; offset++)
    {
        memset(record_data, 0, sizeof(record_data));
        Record_GetEcgResultData(offset, p_attr);
   
        // !注意:这里需要判断如果全是FFFF, 则跳出
        if((record_data[0] == 0xFFFFFFFF) && (record_data[1] == 0xFFFFFFFF))
        {
            p_index->offset_end_index = offset;
            if(p_index->offset_end_index <= RECORD_ECG_ONE_BLOCK_MAX_CNT)       //170
            {
                memset(record_data, 0, sizeof(record_data));
                Record_GetEcgResultData(RECORD_ECG_ONE_BLOCK_MAX_CNT + 1, p_attr);
                if((record_data[0] == 0xFFFFFFFF) && (record_data[1] == 0xFFFFFFFF))
                {
                    p_index->offset_start_index = 0;
                }
                else
                {
                    p_index->offset_start_index = (RECORD_ECG_ONE_BLOCK_MAX_CNT + 1 + offset) % RECORD_ECG_MAX_CNT;
                }
            }
            else
            {
                p_index->offset_start_index = p_index->offset_end_index - RECORD_ECG_ONE_BLOCK_MAX_CNT;
            }
            break;
        }
    }
    
    if(offset >= RECORD_ECG_MAX_CNT)
    {
        //擦除第一块 
        Record_ClearEcgResultBlock(0, 1);
        p_index->offset_end_index = RECORD_ECG_ONE_BLOCK_MAX_CNT + 1;
        p_index->offset_start_index = 0; 
    }
}


// 功耗统计存储
uint32_t Record_AddPowerInfoData(uint8_t day_id, const power_statistics_t *p_power)
{
    uint32_t    power_data[RECORD_POWER_INFO_SIZE / 4] = {0};
    uint8_t     *p_power_data = (uint8_t *)power_data;

    memcpy(p_power_data, p_power, sizeof(power_statistics_t));

    //Flash_CommonStore((uint32_t*)RECORD_DAILY_DATA_ADDR(day_id, RECORD_DAILY_DATA_MAX_CNT * RECORD_DAILY_PER_DATA_SIZE), 
    //                  poer_data, RECORD_POWER_INFO_SIZE / 4);
    
    Extflash_DataWrite(RECORD_DAILY_DATA_ADDR(day_id, RECORD_DAILY_DATA_MAX_CNT), RECORD_POWER_INFO_SIZE, p_power_data);
    
    
    return NRF_SUCCESS;   
}
/*
uint32_t Record_GetPowerInfoData(uint8_t day_id, power_statistics_t *p_power)
{
    uint32_t    poer_data[RECORD_POWER_INFO_SIZE / 4] = {0};
    uint8_t     *p_poer_data = (uint8_t *)poer_data;
    
    Flash_CommonRead(poer_data, 
                     RECORD_DAILY_DATA_ADDR(day_id, RECORD_DAILY_DATA_MAX_CNT * RECORD_DAILY_PER_DATA_SIZE), 
                     RECORD_POWER_INFO_SIZE);
    
    memcpy(p_power, p_poer_data, sizeof(power_statistics_t));

    return NRF_SUCCESS;    
}
*/

