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
** Description:     the file's  funtion  realize some mode's init ,read ,write api
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

/*******************************************************************************************************
** Function name:           Get_MaxTimeIndex
** Descriptions:            最大时间索引
** input parameter:         p_data: 数据指针  len :长度
** Returned Value:
** Created  by:             郭涛
** Created  Datas:          2017-08-04
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
*********************************************************************************************************/
static uint8_t Get_MaxTimeIndex(uint32_t* p_data,uint8_t len)
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

/*******************************************************************************************************
** Function name:                 Record_LogDataIndexInit
** Descriptions:                  日志数据索引初始化 
** input parameter:               p_data: 数据指针 len :长度
** Returned Value:
** Created  by:                   郭涛
** Created  Datas:                2017-08-04
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
*********************************************************************************************************/
void Record_LogDataIndexInit(log_data_index_t* p_index)
{
//    static uint16_t offset = 0;
//    uint32_t record_data[16] = {0};
//    
//    for( offset = 0; offset < LOG_DATA_SIZE; offset += 64)
//    {
//        Flash_CommonRead(record_data,LOG_DATA_ADDR(offset),64);
//        // !注意:这里需要判断如果全是FFFF,则跳出
//        if((0xFFFFFFFF == record_data[0])&&(0xFFFFFFFF == record_data[1]))
//        {
//            p_index->offset = offset/64;
//            break;
//        }
//        else
//        {
//            p_index->total_cnt++;
//        }
//    }
}

void Record_BlockIndexInit(uint8_t*  p_index)
{
    uint32_t offset = 0;
    uint32_t record_data[5] = {0};
    uint32_t record_time[3] = {0};
    daily_data_attr_t*  p_attr = (daily_data_attr_t*)record_data;
    
    // 1、计算五分钟数据索引
    for(uint8_t index = 0; index < 3; index++)
    {
        offset = 0;
        for(offset = 0; offset < RECORD_DAILY_DATA_SIZE; offset += 20)
        {
            memset(record_data, 0, sizeof(record_data));
            Record_GetDailyData(index,offset/20,p_attr);
            // 这里判断心率数据是否有效,判断年月日即可
            if((p_attr->month != 0)&&(p_attr->month <= 12)&&(p_attr->day != 0)&&(p_attr->day <= 31))
            {
                // !注意:这里将整数转换为字符数据,如:08-20 10:30,转换后为:08201030 Add by GT 2017-07-28
                record_time[index] = (((p_attr->month%100)*100+p_attr->day%100)*100+p_attr->hour%100)*100+p_attr->minute%100;
            }
            else
            {
                // !注意:这里需要判断如果全是FFFF,则跳出
                if((0xFFFFFFFF == record_data[0])&&(0xFFFFFFFF == record_data[1]))
                {
                    break;
                }
            }
        }
    } 

    *p_index =  Get_MaxTimeIndex(record_time,3);   
}

/*******************************************************************************************************
** Function name:                      Record_DailyDataIndexInit
** Descriptions:                       日常数据初始化
** input parameter:                    p_index: 日常数据
** Returned Value:
** Created  by:                        郭涛
** Created  Datas:                     2017-08-04
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
*********************************************************************************************************/
void Record_DailyDataIndexInit(daily_data_index_t* p_index)
{
    uint8_t  daily_flag = 0;
    uint32_t offset = 0;
    uint32_t record_data[5] = {0};
    daily_data_attr_t*  p_attr = (daily_data_attr_t*)record_data;
    // 1、计算五分钟数据索引
    for(uint8_t index = 0; index < 3; index++)
    {
        // WJ.20190705.解决跨天血氧计数器没有清除的问题
        p_index->ox_value_cnt[index] = 0;
        p_index->step_cnt[index] = 0;
        p_index->distance_cnt[index] = 0;
        p_index->calories_cnt[index] = 0;

        offset = 0;
        daily_flag = 0;
        
        for(offset = 0; offset < RECORD_DAILY_DATA_SIZE; offset += 20)
        {
            memset(record_data, 0, sizeof(record_data));
            Record_GetDailyData(index,offset/20,p_attr);
            // 这里判断心率数据是否有效,判断年月日即可
            if((p_attr->month != 0)&&(p_attr->month <= 12)&&(p_attr->day != 0)&&(p_attr->day <= 31))
            {
                p_index->step_cnt[index] += p_attr->step;
                p_index->distance_cnt[index] += p_attr->distance;
                p_index->calories_cnt[index] += p_attr->calories;
                
                if(p_attr->hour >= 7 && p_attr->hour < 24 && p_attr->ox_value != 0)
                {
                    p_index->ox_value_cnt[index]++;
                }
            }
            
            // !注意:这里需要判断如果全是FFFF,则跳出
            if((record_data[0] == 0xFFFFFFFF)&&(record_data[1] == 0xFFFFFFFF))
            {
                 p_index->offset[index] = offset/20;
                 daily_flag = 1;
                 break;
            }
        }
        if(daily_flag != 1)
        {
            p_index->offset[index] = offset/20 + 1;
        }
    }
}

/*******************************************************************************************************
** Function name:                      Record_OxygenDataIndexInit
** Descriptions:                       读血氧数据初始化
** input parameter:                    p_index: 血氧数据索引
** Returned Value:
** Created  by:                        郭涛
** Created  Datas:                     2017-08-04
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
*********************************************************************************************************/
void Record_OxygenDataIndexInit(oxygen_data_index_t* p_index)
{
    uint8_t  daily_flag = 0;
    uint32_t offset = 0;
    uint32_t record_data[8] = {0};

    // 处理运动心率数据
    for(uint8_t index = 0; index < 3; index++)
    {
        offset = 0;
        daily_flag = 0;
        
        for(offset = 0; offset < RECORD_BLOOD_OXYGEN_SIZE; offset += 32)
        {
            memset(record_data, 0, sizeof(record_data));
            Flash_CommonRead(record_data,RECORD_BLOOD_OXYGEN_DATA_ADDR(index,offset),32);
            if((record_data[0] == 0xFFFFFFFF)&&(record_data[1] == 0xFFFFFFFF))
            {
                p_index->offset[index] = offset/32;
                daily_flag = 1;
                break;
            }
        }
        if(daily_flag != 1)
        {
            p_index->offset[index] = offset/20 + 1;
        }
    }
}

/*******************************************************************************************************
** Function name:                   Record_SportHeartDataIndexInit
** Descriptions:                    读运动数据指针
** input parameter:                 p_index: 运动数据指针
** Returned Value:
** Created  by:                     郭涛
** Created  Datas:                  2017-08-04
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
*********************************************************************************************************/
void Record_SportHeartDataIndexInit(sport_heart_record_t* p_index)
{
//    uint32_t record_data[5] = {0};
//    uint32_t record_time[3] = {0};
//    sport_heart_attr_t* p_attr = (sport_heart_attr_t*)record_data;
//    // 处理运动心率数据
//    for(uint8_t index = 0; index < 3; index++)
//    {
//        for(uint16_t offset = 0; offset < RECORD_SPORT_HEART_OFFSET; offset += 20)
//        {
//            Flash_CommonRead(record_data,RECORD_MOVEMENT_ADDR(index,offset),20);
//            if((0 == p_attr->year)||(0xFFFF == p_attr->year)||(p_attr->month > 12)||(p_attr->day > 31))
//            {
//                p_index->offset[index] = offset/20;
//                break;
//            }
//            else 
//            {
//                // !注意:这里将整数转换为字符数据,如:2017-08-20 10:30,转换后为:1708201030 Add by GT 2017-07-28
//                record_time[index] = ((((p_attr->year%100)*100+p_attr->month%100)*100+p_attr->day%100)*100+p_attr->hour%100)*100+p_attr->minute%100;
//                // !注意:这里需要判断如果全是FFFF,则跳出
//                if((0xFFFFFFFF == record_data[0])&&(0xFFFFFFFF == record_data[1]))
//                break;
//            }
//        }
//    }
//    
//    p_index->current_block_id = Get_MaxTimeIndex(record_time,3);
}

/*******************************************************************************************************
** Function name:                     Record_ClearAllData
** Descriptions:                      擦除所有数据
** input parameter:                   无
** Returned Value:
** Created  by:                       郭涛
** Created  Datas:                    2017-08-04
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
*********************************************************************************************************/

void Record_ClearAllData(void)
{
    Flash_CommonErase((uint32_t*)RECORD_DAILY_BASE_ADDR,24);//18);
}

/*******************************************************************************************************
** Function name:                           Record_ClearOneDayData
** Descriptions:                            擦除天数据
** input parameter:                         日期id
** Returned Value:
** Created  by:                             郭涛
** Created  Datas:                          2017-08-04
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
*********************************************************************************************************/
void Record_ClearOneDayData(uint8_t day_id)
{
    //输入参数检测处理
    day_id = day_id%3;
    Flash_CommonErase((uint32_t*)RECORD_DAILY_DATA_ADDR(day_id,0), RECORD_DAILY_ONE_DAY_SIZE/4096);
}

/*******************************************************************************************************
** Function name:                Record_AddDailyData
** Descriptions:                 增加天数
** input parameter:              day_id:天数索引  offset:地址索引
                                 p_attr:日常数据
** Returned Value:
** Created  by:                  郭涛
** Created  Datas:               2017-08-04
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
*********************************************************************************************************/
uint32_t Record_AddDailyData(uint8_t day_id, uint16_t offset, const daily_data_attr_t* p_attr)
{
    uint32_t daily_data[5] = {0};
    uint8_t* p_daily_data = (uint8_t*)daily_data;

    // 如果条件不合法,则为无效参数 Add by GT 2017-07-28
    if(!RECORD_DAILY_CONDITION_VALID(day_id,offset))
    {
       return NRF_ERROR_INVALID_PARAM;
    }
	
    memcpy(daily_data,p_attr,sizeof(daily_data_attr_t));
    
    // GT 2017-08-22 FLASH存储和CPU大小端模式刚好相反,需要处理
    p_daily_data[4] = (uint8_t)((p_attr->step>>8)&0xFF);
    p_daily_data[5] = (uint8_t)(p_attr->step&0xFF);
    p_daily_data[6] = (uint8_t)((p_attr->sport>>8)&0xFF);
    p_daily_data[7] = (uint8_t)(p_attr->sport&0xFF);
    p_daily_data[14] = (uint8_t)((p_attr->distance>>8)&0xFF);
    p_daily_data[15] = (uint8_t)(p_attr->distance&0xFF);
    p_daily_data[16] = (uint8_t)((p_attr->calories>>8)&0xFF);
    p_daily_data[17] = (uint8_t)(p_attr->calories&0xFF);
	
    Flash_CommonStore((uint32_t*)RECORD_DAILY_DATA_ADDR(day_id,offset*20),daily_data,5);
	
    return NRF_SUCCESS;
}

/*******************************************************************************************************
** Function name:            Record_GetDailyData
** Descriptions:             从flash数据
** input parameter:          day_id:数据天数索引 offset: 偏移量
                             p_attr:日常数据指针
** Returned Value:
** Created  by:              郭涛
** Created  Datas:           2017-08-04
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
*********************************************************************************************************/

uint32_t Record_GetDailyData(uint8_t day_id, uint16_t offset, daily_data_attr_t* p_attr)
{
    uint32_t daily_data[5] = {0};
    uint8_t  *p_daily_data = (uint8_t*)daily_data;

    // 如果条件不合法,则为无效参数 Add by GT 2017-07-28
    if(!RECORD_DAILY_CONDITION_VALID(day_id,offset))
    {
        return NRF_ERROR_INVALID_PARAM;
    }
    
    Flash_CommonRead(daily_data,RECORD_DAILY_DATA_ADDR(day_id,offset*20),20);
    
    memcpy(p_attr,daily_data,sizeof(daily_data_attr_t));
    
    // !!!注意:这里有高低位的区分
    p_attr->step  = (uint16_t)((p_daily_data[4]<<8)|(p_daily_data[5]));
    p_attr->sport = (uint16_t)((p_daily_data[6]<<8)|(p_daily_data[7]));
    p_attr->distance = (uint16_t)((p_daily_data[14]<<8)|(p_daily_data[15]));
    p_attr->calories = (uint16_t)((p_daily_data[16]<<8)|(p_daily_data[17]));
    
    return NRF_SUCCESS;
}

/*******************************************************************************************************
** Function name:                 Record_GetSleepAnalysisData
** Descriptions:                  取得睡眠分析数据 
** input parameter:               day_id:天数索引  offset: 偏移量
                                  p_attr:数据指针
** Returned Value:
** Created  by:                   郭涛
** Created  Datas:                2017-08-04
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
*********************************************************************************************************/
uint32_t Record_GetSleepAnalysisData(uint8_t day_id, uint16_t offset,uint8_t* p_data)
{
    uint32_t daily_data[3] = {0};
    uint8_t* p_daily_data = (uint8_t*)daily_data;

    // 如果条件不合法,则为无效参数 Add by GT 2017-07-28
    if(!RECORD_DAILY_CONDITION_VALID(day_id,offset))
    {
        return NRF_ERROR_INVALID_PARAM;
    }
     
    // !注意:这里睡眠算法不需要月份数据     
    Flash_CommonRead(daily_data, RECORD_DAILY_DATA_ADDR(day_id,offset*20), 9);
    memcpy(p_data,&p_daily_data[1],8);

    return NRF_SUCCESS;
}

/*******************************************************************************************************
** Function name:                      Record_ModifyOahsFlag
** Descriptions:                       修改5分钟血氧修正标志位
** input parameter:                    无
** Returned Value:                     无
** Created  by:                        郭涛
** Created  Datas:                     2017-08-04
*********************************************************************************************************/
//uint32_t test_oxygen_data[1] = {0};
//uint32_t flag_tmp = 0;
uint32_t Record_ModifyOahsFlag(uint8_t day_id, uint16_t offset, uint8_t flag, uint8_t *ox_result_buf)
{   
    uint32_t oxygen_data[1] = {0};
    uint32_t flag_tmp = 0;
    uint8_t  *p_data = (uint8_t *)&oxygen_data[0];
    //uint8_t* p_oxygen_data = (uint8_t*)test_oxygen_data;//oxygen_data;
    uint16_t ox_offset = 0;

    ox_offset = offset;
    
    if(!RECORD_BLOOD_OXYGEN_CONDITION_VALID(day_id,offset))
    {
        return NRF_ERROR_INVALID_PARAM;
    }
    
    //数据进行处理
    flag_tmp = (((flag<<2)|0xF3) << 8)+0xffff00ff;
    
    for(uint8_t i = 0; (i < 4)&&(offset > 1); i++)
    {
        offset--;
        
        Flash_CommonRead(oxygen_data,RECORD_BLOOD_OXYGEN_DATA_ADDR(day_id,offset*32)+12,4);
        oxygen_data[0] = (oxygen_data[0] & flag_tmp);
        
        Flash_CommonStore((uint32_t*)(RECORD_BLOOD_OXYGEN_DATA_ADDR(day_id,offset*32)+12), oxygen_data, 1);
    }
    //WJ.20190619. 解决第一条没有保存的问题。
    for(uint8_t i = 0; (i < 4)&&(ox_offset >= 1); i++)
    {
        ox_offset--;    
        //存储每分钟血氧数据 
        Flash_CommonRead(oxygen_data, RECORD_BLOOD_OXYGEN_DATA_ADDR(day_id,ox_offset*32), 4);
        p_data[2] = ox_result_buf[3 - i];
        Flash_CommonStore((uint32_t*)(RECORD_BLOOD_OXYGEN_DATA_ADDR(day_id,ox_offset*32)), oxygen_data, 1);
    }
    
    return NRF_SUCCESS;
}


/*******************************************************************************************************
** Function name:                      Record_AddOxygenData
** Descriptions:                       存储血氧数据 
** input parameter:                    day_id:flash 块id  offset: 偏移量
                                       p_attr:数据指针
** Returned Value:
** Created  by:                        郭涛
** Created  Datas:                     2017-08-04
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
*********************************************************************************************************/
uint32_t Record_AddOxygenData(uint8_t day_id,uint16_t offset,const blood_oxygen_attr_t* p_attr)
{
    uint32_t oxygen_data[4] = {0};

    if(!RECORD_BLOOD_OXYGEN_CONDITION_VALID(day_id,offset))
    {
        return NRF_ERROR_INVALID_PARAM;
    }
    
    memcpy(oxygen_data,p_attr,sizeof(blood_oxygen_attr_t));
    Flash_CommonStore((uint32_t*)RECORD_BLOOD_OXYGEN_DATA_ADDR(day_id,offset*32),oxygen_data,4);
    
    return NRF_SUCCESS;
}

/*******************************************************************************************************
** Function name:                      Record_GetOxygenData
** Descriptions:                       取得血氧数据
** input parameter:                    day_id:日期id offset:偏移量
                                       p_attr:数据指针
** Returned Value:
** Created  by:                        郭涛
** Created  Datas:                     2017-08-04
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
*********************************************************************************************************/
uint32_t Record_GetOxygenData(uint8_t day_id,uint16_t offset,blood_oxygen_attr_t* p_attr)
{
    uint32_t oxygen_data[4] = {0};

    if(!RECORD_BLOOD_OXYGEN_CONDITION_VALID(day_id,offset))
    {
        return NRF_ERROR_INVALID_PARAM;
    }
    
    Flash_CommonRead(oxygen_data,RECORD_BLOOD_OXYGEN_DATA_ADDR(day_id,offset*32),16);
    memcpy(p_attr,oxygen_data,sizeof(blood_oxygen_attr_t));
    
    return NRF_SUCCESS;
}

uint32_t Record_AddScatterHeartData(uint8_t day_id, uint16_t offset, const scatter_heart_t* p_heart)
{
    uint32_t sheart_data[4] = {0};

    if(!RECORD_BLOOD_OXYGEN_CONDITION_VALID(day_id,offset))
    {
        return NRF_ERROR_INVALID_PARAM;
    }
    
    memcpy(sheart_data, p_heart, sizeof(scatter_heart_t));
    Flash_CommonStore((uint32_t*)RECORD_BLOOD_OXYGEN_DATA_ADDR(day_id,offset*32+16),sheart_data,4);
    
    return NRF_SUCCESS;
}

uint32_t Record_GetScatterHeartData(uint8_t day_id, uint16_t offset, scatter_heart_t* p_heart)
{
    uint32_t sheart_data[4] = {0};

    if(!RECORD_BLOOD_OXYGEN_CONDITION_VALID(day_id,offset))
    {
        return NRF_ERROR_INVALID_PARAM;
    }
    
    Flash_CommonRead(sheart_data, RECORD_BLOOD_OXYGEN_DATA_ADDR(day_id,offset*32+16), 16);
    memcpy(p_heart, sheart_data, sizeof(scatter_heart_t));
    
    return NRF_SUCCESS;
}


/*******************************************************************************************************
** Function name:        Record_GetSystemTimeIndex
** Descriptions:         取得系统时间索引
** input parameter:      p_index:the index to store
                    
** Returned Value:
** Created  by:         郭涛
** Created  Datas:      2017-08-04
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
*********************************************************************************************************/
void Record_GetSystemTimeIndex(uint16_t* p_index)
{
    uint32_t system_time[2] = {0};
				
    *p_index = 0xFFFF;
    for(uint16_t offset = 0; offset < 512; offset++)
    {
        Flash_CommonRead(system_time,SYSTEM_TIME_BASE_ADDR+offset*8,sizeof(system_time));
        if((0xFFFFFFFF == system_time[0])&&(0xFFFFFFFF == system_time[1]))
        {
            *p_index = offset;
            break;
        }
    }
}

/*******************************************************************************************************
** Function name:                  Record_GetAlarmTimeParam
** Descriptions:                   得到闹钟参数
** input parameter:                index:索引   p_alarm:指针数据
                    
** Returned Value:
** Created  by:                    郭涛
** Created  Datas:                 2017-08-04
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
*********************************************************************************************************/
void Record_GetAlarmTimeParam(uint8_t index,alarm_time_config_t* p_alarm)
{
    uint32_t alarm_param[4] = {0};
    uint8_t* p_alarm_param = (uint8_t*)alarm_param;
				
    Flash_CommonRead(alarm_param,ALARM_CONFIG_BASE_ADDR+index*16,sizeof(alarm_param));
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
        memset((uint8_t*)p_alarm,0,sizeof(p_alarm));
    }
}

/*******************************************************************************************************
** Function name:                      Record_UpdateAlarmTimeParam
** Descriptions:                       更新闹钟参数
** input parameter:                    index:索引  p_alarm:数据指针
                    
** Returned Value:
** Created  by:                        郭涛
** Created  Datas:                     2017-08-04
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
*********************************************************************************************************/
void Record_UpdateAlarmTimeParam(uint8_t index,const alarm_time_config_t* p_alarm)
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
				
    Flash_CommonStore((uint32_t*)(ALARM_CONFIG_BASE_ADDR+index*16),alarm_param,4);
}

/*******************************************************************************************************
** Function name:                      Record_AddLogData
** Descriptions:                       增加log数据 
** input parameter:                    offset:数据索引 p_attr:数据指针
** Returned Value:
** Created  by:                        郭涛
** Created  Datas:                     2017-08-04
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
*********************************************************************************************************/
uint32_t Record_AddLogData(uint16_t offset, const log_data_attr_t* p_attr)
{
//    uint32_t log_data[16] = {0};
//       
//    if(!LOG_CONDITION_VALID(offset))
//         return NRF_ERROR_INVALID_PARAM;
//   
//    memcpy(log_data,p_attr,64);
//    Flash_CommonStore((uint32_t*)LOG_DATA_ADDR(offset*64),log_data,16);
       
    return NRF_SUCCESS;
}

/*******************************************************************************************************
** Function name:                      Record_GetLogData
** Descriptions:                       得到日常数据
** input parameter:                    offset:偏移量  p_attr:数据指针
                    
** Returned Value:
** Created  by:                        郭涛
** Created  Datas:                     2017-08-04
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
*********************************************************************************************************/
uint32_t Record_GetLogData(uint16_t offset, log_data_attr_t* p_attr)
{
//    uint32_t log_data[16] = {0};
//    
//    if(!LOG_CONDITION_VALID(offset))
//    {
//        return NRF_ERROR_INVALID_PARAM;
//    }
//    
//    Flash_CommonRead(log_data,LOG_DATA_ADDR(offset*64),sizeof(log_data));
//    memcpy(p_attr,log_data,sizeof(log_data));
    
    return NRF_SUCCESS;
}

//uint32_t Record_AddUnreadMessageData(uint8_t message_index, const unread_message_detail_t *p_message)
//{
//    uint32_t message_data[64] = {0};
//    uint8_t *p_message_data = (uint8_t*)message_data;

//    memcpy(message_data,p_message,sizeof(message_data));
//    
//    // 注意:这里分两部分写入
//    Flash_CommonStore((uint32_t*)RECORD_UNREAD_MESSAGE_DATA_ADDR(message_index,0),message_data,16);
//    // 写入长度和消息内容
//    Flash_CommonStore((uint32_t*)RECORD_UNREAD_MESSAGE_DATA_ADDR(message_index,0),&message_data[5],236);
//    
//    return NRF_SUCCESS;	
//}

//uint32_t Record_GetUnreadMessageData(uint8_t message_index, unread_message_detail_t *p_message)
//{
//    uint32_t message_data[64] = {0};
//    uint8_t *p_message_data = (uint8_t*)message_data;

//    Flash_CommonRead(message_data, RECORD_UNREAD_MESSAGE_DATA_ADDR(message_index,0), 256);
//    memcpy(p_message, message_data, sizeof(message_data));
//    
//    return NRF_SUCCESS;	
//}

//uint32_t Record_UpdateReadMessageLen(uint8_t message_index, uint8_t page_index, uint8_t len)
//{
//    uint32_t param_data = 0;
//    
//    //

//    Flash_CommonStore((uint32_t*)RECORD_UNREAD_MESSAGE_DATA_ADDR(message_index,page_index), &param_data, 1);
//    
//    return NRF_SUCCESS;	
//}
