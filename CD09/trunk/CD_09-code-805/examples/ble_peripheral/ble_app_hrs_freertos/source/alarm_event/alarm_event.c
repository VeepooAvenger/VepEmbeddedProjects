/***************************Copyright(c)***********************************
**                       Veepoo Tech
**                  COPYRIGHT (C) 2016-2019
**-----------File Info-----------------------------------------------------
** File        name:     alarm_event.c
** Last modified Date:   2017-08-24
** Last SYS_VERSION:     0.0.0
** Descriptions:            
**-------------------------------------------------------------------------
** Created  by:          WJ
** Created  date:        2017-08-24
** SYS_VERSION:          0.0.0
** Descriptions:         
**-------------------------------------------------------------------------
** Modified by:				
** Modified date:  			          
** SYS_VERSION:                 
** Descriptions:                        
***************************************************************************/
#include <string.h>
#include "nrf_delay.h"
#include "alarm_event.h"
#include "timer_manage.h"
#include "system_clock.h"
#include "bracelet_protocol.h"
#include "motor_event.h"
#include "gui_page_display.h"
#include "app_crc16.h"
#include "app_config_data.h"
#include "app_flash.h"
#include "app_task.h"
#include "app_handler.h"
#include "gui_thread.h"

//__ALIGN(4) 用于强制分配变量起始地址为4的倍数
__ALIGN(4) static alarm_clock_config_t alarm_clock_config[ALARM_CLOCK_MAX_NUM];    //闹铃结构体参数 20组
static uint8_t alarm_clock_save = 0;                //是否需要保存闹钟数据的标志位
static uint8_t alarm_clock_save_delay = 0;          //延时存储
static alarm_clock_data_t alarm_clock_data;         //发送闹钟数据的详情
static alarm_clock_do_t alarm_clock_do;

static countdown_config_t countdown_config = {0,};      //倒计时

static stopwatch_config_t stopwatch_config = {0x00, };  //秒表

extern system_info_t system_info_s;   //多次倒计时数据从这里获取

static uint8_t alarm_write_flag = 0;        // 闹钟写数据的控制标志

uint8_t count_down_start_flag = 0;			// 倒计时启动标志

uint8_t count_down_timeout_flag = 0;			// 倒计时到时标志

/*******************************************************闹钟处理******************************************/

/****************************************************************
** Function name:           Alarm_GetAlarmClockDo
** Descriptions:            获取最后执行的闹钟数据
** input parameters:        无
** output parameters:       无
** Returned value:          返回闹钟数据结构体     
** Created by:              WJ              
** Created Date:            2017-07-28
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
alarm_clock_do_t *Alarm_GetAlarmClockDo(void)
{
    return &alarm_clock_do;
}

/****************************************************************
** Function name:           Alarm_SetSendData
** Descriptions:            设置闹钟数据发送的详情
** input parameters:        start_index:遍历起始地址(数组下标);total_index:有效闹钟个数;
                            crc16:当前设备闹钟的CRC
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-05-15
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Alarm_SetSendData(uint8_t start_index, uint8_t total_index, uint16_t crc16)
{
    alarm_clock_data.alarm_index = 0;
    alarm_clock_data.alarm_num_start = start_index;
    alarm_clock_data.alarm_num_total = total_index;
    alarm_clock_data.alarm_crc16 = crc16;
}

/****************************************************************
** Function name:           Alarm_SendDataHandler
** Descriptions:            发送闹钟数据到APP，每次发送一组，需要延时循环发送
** input parameters:        无
** output parameters:       无
** Returned value:          返回0:闹钟全部发送完成;1:还有闹钟需要发送
** Created by:              WJ              
** Created Date:            2017-05-15
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
uint8_t Alarm_SendDataHandler(void)
{
    uint8_t alarm_data_sned[20] = {0x00,};   
    uint8_t i = 0;
	uint16_t crc16 = 0;
	alarm_data_sned[0] = 0xB9;
	alarm_data_sned[1] = 0x01;   
    alarm_data_sned[4] = 0x02;
		  
    if(alarm_clock_data.alarm_num_start >= alarm_clock_data.alarm_num_total)
    {   //发送结束包
    	alarm_data_sned[2] = 0;
    	alarm_data_sned[3] = Alarm_GetValidConfigNum();
       
        crc16 = Alarm_GetCRC();
        alarm_data_sned[18] = (crc16     )&0xFF;
        alarm_data_sned[19] = (crc16 >> 8)&0xFF;
        Ble_SendDataViaBat(alarm_data_sned, 20); 	
        return 0;
    }
         
    alarm_data_sned[2] = ++alarm_clock_data.alarm_num_start;
    alarm_data_sned[3] = alarm_clock_data.alarm_num_total;
    
    i = alarm_clock_data.alarm_index;
    for( ; i < ALARM_CLOCK_MAX_NUM; i++ )
    {       
        alarm_clock_data.alarm_index++; //发送下一组数据的起始下标
        if(alarm_clock_config[i].valid)
        {           
            alarm_data_sned[5] = i+1;
            memcpy(&alarm_data_sned[6], &alarm_clock_config[i].hour, 9);
            alarm_data_sned[18] = (alarm_clock_data.alarm_crc16     )&0xFF;
            alarm_data_sned[19] = (alarm_clock_data.alarm_crc16 >> 8)&0xFF;
            Ble_SendDataViaBat(alarm_data_sned, 20); 
            break;      //每次发送一组有效数据就结束轮询
        }        
    }
    
    if(i >= ALARM_CLOCK_MAX_NUM)
    {   //没有有效数据了
        if(alarm_clock_data.alarm_num_start < alarm_clock_data.alarm_num_total)
        {   //异常导致发送的数据包和要发送的少，下次再发一包结束包
            alarm_clock_data.alarm_num_start = alarm_clock_data.alarm_num_total;
            return 1;
        }
        return 0;
    }
    
    return 1;
}

/****************************************************************
** Function name:           Alarm_ClearByID
** Descriptions:            通过ID清除闹钟，需要保存
** input parameters:        无
** output parameters:       无
** Returned value:          返回0:id无效,删除无效;1:闹钟删除成功
** Created by:              WJ              
** Created Date:            2017-05-15
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
uint8_t Alarm_ClearByID(uint8_t id)
{
    if(id > ALARM_CLOCK_MAX_NUM && id < 1) return 0;
    
    memset(&alarm_clock_config[id - 1], 0x00, sizeof(alarm_clock_config_t));
    alarm_clock_save_delay = 0;         //重置延时
    alarm_clock_save = 1;               //设置需要保存闹钟标志位
    
    return 1;
}

/****************************************************************
** Function name:           Alarm_InitAll
** Descriptions:            重置所有闹钟,但不需要保存，用于恢复出厂设置，
                            闹钟数据在清除整个用户数据块里清除，根据实际情况使用
** input parameters:        无
** output parameters:       无
** Returned value:          返回1:闹钟重置成功
** Created by:              WJ              
** Created Date:            2017-05-15
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
uint8_t Alarm_InitAll(void)
{
    memset(alarm_clock_config, 0x00, sizeof(alarm_clock_config));  
    return 1;
}

/****************************************************************
** Function name:           Alarm_ClearAll
** Descriptions:            重置所有闹钟,需要保存
** input parameters:        无
** output parameters:       无
** Returned value:          返回1:闹钟重置成功
** Created by:              WJ              
** Created Date:            2017-05-15
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/
uint8_t Alarm_ClearAll(void)
{
    memset(alarm_clock_config, 0x00, sizeof(alarm_clock_config));
    
    App_FlashEraseAlarmClockData();
    
    return 1;
}

/****************************************************************
** Function name:           Alarm_IsOutTargetTime
** Descriptions:            单次闹钟时间是否已过,用于APP设置闹钟校验，重启自检
** input parameters:        无
** output parameters:       无
** Returned value:          返回0:不是单次闹钟或者闹钟没有失效; 1:单次闹钟失效
** Created by:              WJ              
** Created Date:            2017-05-15
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/
static bool Alarm_IsOutTargetTime(const alarm_clock_config_t *p_alarm)
{  
    if(p_alarm->repeat != 0)
    {
        return 0;
    }
    if(p_alarm->year < clock->year || (p_alarm->year == clock->year &&p_alarm->month < clock->month)
      || (p_alarm->year == clock->year && p_alarm->month == clock->month && p_alarm->day < clock->day )
      || (p_alarm->year == clock->year && p_alarm->month == clock->month && p_alarm->day == clock->day
      && (p_alarm->hour*60+p_alarm->minute < clock->hour * 60 + clock->minutes)))
    {
        return 1;
    }
    
	return 0;
}

/****************************************************************
** Function name:           Alarm_IsTargetTime
** Descriptions:            闹钟到达时间
** input parameters:        无
** output parameters:       无
** Returned value:          返回0:闹钟未到时见; 1:闹钟已到时间
** Created by:              WJ              
** Created Date:            2017-05-15
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/
static bool Alarm_IsTargetTime(const alarm_clock_config_t *p_alarm)
{
	return ((clock->minutes == p_alarm->minute) && (clock->hour == p_alarm->hour));
}

/****************************************************************
** Function name:           Alarm_Monitor
** Descriptions:            监听闹钟，到时间执行闹钟处理，整分钟处理
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-05-15
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/
void Alarm_Monitor(void)
{
    uint8_t index = 0;
	uint8_t repeat_move_bit = 0;
	uint8_t alarm_is_arrive = 0;
    uint8_t scene_mode = 0;
    uint8_t repeat_aram_flag = 0;
	alarm_clock_config_t *p_current_alarm = NULL;
	
	for(index = 0; index < ALARM_CLOCK_MAX_NUM; index++)
	{        
        if(alarm_clock_config[index].valid != 1) continue;
        
        p_current_alarm = &alarm_clock_config[index];
		if(Alarm_IsTargetTime(p_current_alarm))
		{
			//1、如果是单次闹铃,则需要关闭
			if((1 == p_current_alarm->state) && (0x00 == p_current_alarm->repeat) 
              && clock->year == p_current_alarm->year 
              && clock->month == p_current_alarm->month 
              && clock->day == p_current_alarm->day)
			{
				alarm_is_arrive = 1;
				p_current_alarm->state = 0;
                scene_mode = p_current_alarm->scene_mode;
                break;      //找打一个单次闹钟就不再继续查找
			}
            //2、判断是否是重复闹铃
            else if((p_current_alarm->state == 1) && ( p_current_alarm->repeat != 0x00) 
                    && repeat_aram_flag == 0)
            {    
				if(0 == clock->week)
                {
					repeat_move_bit = 7;
                }
				else
                {
					repeat_move_bit = clock->week;
                }
				repeat_move_bit--;
				if((p_current_alarm->repeat >> repeat_move_bit) & 0x01)
				{
					//闹铃触发
					alarm_is_arrive = 1;
                    scene_mode = p_current_alarm->scene_mode;
                    repeat_aram_flag = 1;
				}
			}
		}
	}
    
	if(alarm_is_arrive)
	{      
        alarm_clock_do.alarm_hour = clock->hour;
        alarm_clock_do.alarm_minute = clock->minutes;
        alarm_clock_do.alarm_mode = scene_mode;       
        Gui_ThreadEventTaskWakeUp(CPU_EVENT_TASK_ALARM);
	} 
}

/****************************************************************
** Function name:           Alarm_Check
** Descriptions:            单个闹钟数据自检，判断数据有效区间
** input parameters:        无
** output parameters:       无
** Returned value:          返回0:闹钟无效; 1:闹钟有效
** Created by:              WJ              
** Created Date:            2017-05-15
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/
uint8_t Alarm_Check(alarm_clock_config_t *alarm_clock_temp)
{
    if(alarm_clock_temp->hour > 23 
        || alarm_clock_temp->minute >59)
    {
         return 0;
    }
    if((alarm_clock_temp->repeat == 0 )
         && (alarm_clock_temp->day > 31 || alarm_clock_temp->day == 0 || 
        alarm_clock_temp->month > 12 || alarm_clock_temp->month == 0
        || alarm_clock_temp->state > 1 || alarm_clock_temp->year > 3000 || alarm_clock_temp->year < 2000))
    {        
        return 0;
    }
    
    return 1;
}

//
/****************************************************************
** Function name:           Alarm_GetValidConfigNum
** Descriptions:            获取闹钟的有效个数
** input parameters:        无
** output parameters:       无
** Returned value:          返回:有效闹钟个数
** Created by:              WJ              
** Created Date:            2017-05-15
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/
uint8_t Alarm_GetValidConfigNum(void)
{
    uint8_t index = 0;
	uint8_t total_num = 0;
    
	for(index = 0; index < ALARM_CLOCK_MAX_NUM; index++)
	{    
        if(alarm_clock_config[index].valid)
        {
            total_num++;
        }
    }
    
    return total_num;
}

/****************************************************************
** Function name:           Alarm_Set
** Descriptions:            设置闹钟,数据来自协议(APP设置),数据为闹钟的有效部分,
                            从ID开始,总计10个字节！！！
** input parameters:        无
** output parameters:       无
** Returned value:          返回0:闹钟无效,设置失败; 1:闹钟有效,设置成功
** Created by:              WJ              
** Created Date:            2017-05-15
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/
uint8_t Alarm_Set(uint8_t *data, uint8_t len)
{
    uint8_t id = 0;
	alarm_clock_config_t alarm_clock_temp = {0};	

	//判断索引是否有效
    id = data[0];
	if((1 <= id) && (id <= ALARM_CLOCK_MAX_NUM))
	{
        memcpy((void*)&(alarm_clock_temp.hour), &data[1], 9);
        if(!Alarm_Check(&alarm_clock_temp) || Alarm_IsOutTargetTime(&alarm_clock_temp))
        {
            return 0;
        }

        alarm_clock_temp.valid = 1;
        
        memcpy(&alarm_clock_config[id - 1], &alarm_clock_temp, sizeof(alarm_clock_config_t));  
        
        //保存     
        alarm_clock_save_delay = 0; 
        alarm_clock_save = 1;
	}
	else
	{		
        return 0;
	}
    
    return 1;
}

/****************************************************************
** Function name:           Alarm_GetCRC
** Descriptions:            计算闹钟的CRC
** input parameters:        无
** output parameters:       无
** Returned value:          返回:有效闹钟的CRC(包括已经执行失效的闹钟)
** Created by:              WJ              
** Created Date:            2017-05-15
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/
uint16_t Alarm_GetCRC(void)
{
    uint8_t index = 0;
	uint16_t alarm_crc = 0xFFFF;
	uint8_t  alarm_data[10] = {0};
	
	for(index = 0; index < ALARM_CLOCK_MAX_NUM; index++)
	{
		if(alarm_clock_config[index].valid == 1)
		{
			alarm_data[0] = index+1;
			alarm_data[1] = alarm_clock_config[index].hour;
			alarm_data[2] = alarm_clock_config[index].minute;
			alarm_data[3] = alarm_clock_config[index].state;
			alarm_data[4] = alarm_clock_config[index].repeat;
			alarm_data[5] = alarm_clock_config[index].scene_mode;
			alarm_data[6] = alarm_clock_config[index].year & 0xFF;
			alarm_data[7] = (alarm_clock_config[index].year>>8) & 0xFF;
			alarm_data[8] = alarm_clock_config[index].month;
			alarm_data[9] = alarm_clock_config[index].day;
			alarm_crc = App_Crc16ByCrc16(alarm_crc, alarm_data, sizeof(alarm_data));
		}
	}
    if(alarm_crc == 0xFFFF)
    {
        alarm_crc = 0;
    }
    
	return alarm_crc;
}

/****************************************************************
** Function name:           Alarm_SetToFlashCheck
** Descriptions:            把闹钟写入到flash，目前52端设置了10s的保存延时,在1秒事件里执行
** input parameters:        无
** output parameters:       无
** Returned value:          返回:有效闹钟的CRC(包括已经执行失效的闹钟)
** Created by:              WJ              
** Created Date:            2017-05-15
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------*/
uint8_t Alarm_SetToFlashCheck(void)
{
//    static uint8_t alarm_write_flag = 0;
    uint8_t ret = 0;
    
    if(alarm_write_flag == 1 && alarm_clock_save == 0)
    {
        alarm_write_flag = 0;
        ret = App_FlashWriteAlarmClockData((void *)alarm_clock_config, sizeof(alarm_clock_config));
    }
    
    if(alarm_clock_save != 1)           //是否有数据需要保存
    {
        alarm_clock_save_delay = 0;
        return ret;
    }
    if(alarm_clock_save_delay++ < 5) return ret;       //10s延时
    
    if(alarm_write_flag == 0)
    {
        if(App_FlashEraseAlarmClockData() == APP_FLASH_SUCCESS)
        {
            alarm_write_flag = 1;
            alarm_clock_save = 0;  
//        nrf_delay_ms(10);
//        ret = App_FlashWriteAlarmClockData((void *)alarm_clock_config, sizeof(alarm_clock_config));
        }
    }
    else
    {        
        alarm_clock_save = 0;  
    }    
    
    return ret;
}

/****************************************************************
** Function name:           Alarm_PowerOffSave
** Descriptions:            关机保存闹钟数据
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              JSH              
** Created Date:            2018-07-16
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------*/
void Alarm_PowerOffSave(void)
{
    if(alarm_clock_save == 0)
    {
        if(alarm_write_flag == 0)
        {
            return;
        }
        else if(alarm_write_flag == 1)
        {
            if(App_FlashWriteAlarmClockData((void *)alarm_clock_config, sizeof(alarm_clock_config)) == APP_FLASH_SUCCESS)
            {
                alarm_write_flag = 0;
            }
        }
    }
    else if(alarm_clock_save == 1)
    {
        if(App_FlashEraseAlarmClockData() == APP_FLASH_SUCCESS)
        {
            App_FlashEraseCheck();
            
            if(App_FlashWriteAlarmClockData((void *)alarm_clock_config, sizeof(alarm_clock_config)) == APP_FLASH_SUCCESS)
            {
                alarm_clock_save = 0;
            }
        }
    }

    return ;
}

/****************************************************************
** Function name:           Alarm_getFromFlash
** Descriptions:            从flash获取闹钟数据，注意这里与平台、底层设计相关
** input parameters:        无
** output parameters:       无
** Returned value:          返回:读取状态
** Created by:              WJ              
** Created Date:            2017-05-15
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------*/
uint8_t Alarm_getFromFlash(void)
{
    uint8_t ret = 0;
    
    ret = App_FlashReadAlarmClockData((void *)alarm_clock_config, sizeof(alarm_clock_config));
    
    return ret;
}

//
/****************************************************************
** Function name:           Alarm_InitCheck
** Descriptions:            闹钟自检，清除无效闹钟，上电、或者定时自检
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-05-15
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------*/
void Alarm_InitCheck(void)
{
    uint8_t index = 0;
	
	for(index = 0; index < ALARM_CLOCK_MAX_NUM; index++)
	{    
        if(!Alarm_Check(&alarm_clock_config[index]) || alarm_clock_config[index].valid != 1)
        {
            memset(&alarm_clock_config[index], 0x00, sizeof(alarm_clock_config_t));
        }
    }    
}
 
/****************************************************************
** Function name:           Alarm_ValidCheck
** Descriptions:            自检单次闹钟，过时的需要重置状态
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-05-15
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------*/
void Alarm_ValidCheck(void)
{
    uint8_t index = 0;
	
	for(index = 0; index < ALARM_CLOCK_MAX_NUM; index++)
	{   
        if(alarm_clock_config[index].valid != 1) continue;
        
        //1、如果是单次闹铃,则需要关闭
        if((1 == alarm_clock_config[index].state)&&(0x00 == alarm_clock_config[index].repeat))
        {
            if(Alarm_IsOutTargetTime(&alarm_clock_config[index]))
            {
                alarm_clock_config[index].state = 0;
            }
        }
    }  
}

/****************************************************************
** Function name:           Alarm_Init
** Descriptions:            闹钟初始化，自检、过时自检
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-05-15
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------*/
void Alarm_Init(void)
{
    Alarm_getFromFlash();
    Alarm_InitCheck();
    Alarm_ValidCheck();
}
/***************************************************闹钟处理结束******************************************/

/*****************************************************秒表处理********************************************/

/****************************************************************
** Function name:           Watch_100ms_Start
** Descriptions:            用于秒表计时的100毫秒定时器开启
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-05-15
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------*/
void Watch_100ms_Start(void)
{
    Timer_StopWatchStart();
}

/****************************************************************
** Function name:           Watch_100ms_Stop
** Descriptions:            用于秒表计时的100毫秒定时器关闭
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-05-15
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------*/
void Watch_100ms_Stop(void)
{
    Timer_StopWatchStop();
}
 
/****************************************************************
** Function name:           Watch_100msEvent
** Descriptions:            用于秒表计时的100毫秒定时器事件
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-05-15
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------*/
void Watch_100msEvent(void)
{    
    if(++stopwatch_config.stopwatch_100millisecond >=10)
    {
        stopwatch_config.stopwatch_100millisecond = 0;
        stopwatch_config.total_second++ ;
        
        if(stopwatch_config.total_second > 5999 || 
            (stopwatch_config.total_second == 5999 && stopwatch_config.stopwatch_100millisecond >= 9))
        {
            stopwatch_config.total_second = 5999;
            stopwatch_config.stopwatch_100millisecond = 9; 
            stopwatch_config.state = STOPWATCH_STOP;
            Watch_100ms_Stop();
        }
    }
//    App_AddEvent(APP_NULL_EVENT);
}

/****************************************************************
** Function name:           Watch_Init
** Descriptions:            秒表初始化,(关机、按键开启)
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-05-15
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------*/
void Watch_Init(void)
{
    memset(&stopwatch_config, 0x00, sizeof(stopwatch_config_t));
    stopwatch_config.state = STOPWATCH_IDE;
    Timer_StopWatchStop();
}
 
/****************************************************************
** Function name:           Watch_SetReady
** Descriptions:            设置秒表为就绪状态,(3s长按,初始化完成，并设置秒表为就绪状态)
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-05-15
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------*/
void Watch_SetReady(void)
{
    stopwatch_config.state = STOPWATCH_READY;        //就绪
}
 
/****************************************************************
** Function name:           Watch_Start
** Descriptions:            秒表从就绪、停止状态开启
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-05-15
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------*/
void Watch_Start(void)
{
    if(stopwatch_config.total_second >= 5999 
        && stopwatch_config.stopwatch_100millisecond == 9)
    {           
        return;
    }
    Watch_100ms_Start();
    stopwatch_config.state = STOPWATCH_START;
    stopwatch_config.init_start = 1;
}
 
/****************************************************************
** Function name:           Watch_Stop
** Descriptions:            停止秒表,按键停止或者秒表计时到达上限
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-05-15
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------*/
void Watch_Stop(void)
{
    Timer_StopWatchStop();
    stopwatch_config.state = STOPWATCH_STOP;
    stopwatch_config.init_start = 0;
}
 
/****************************************************************
** Function name:           Watch_GetState
** Descriptions:            获取秒表的当前状态
** input parameters:        无
** output parameters:       无
** Returned value:          无
                            STOPWATCH_IDE,          //准备
                            STOPWATCH_READY,        //就绪
                            STOPWATCH_START,        //开始
                            STOPWATCH_STOP,         //停止
** Created by:              WJ              
** Created Date:            2017-05-15
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------*/
stopwatch_e Watch_GetState(void)
{
    return stopwatch_config.state;
}

/****************************************************************
** Function name:           Watch_IsBusy
** Descriptions:            秒表已经准备就绪,处于忙碌状态
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-05-15
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------*/
uint8_t Watch_IsBusy(void)
{
    return stopwatch_config.state >= STOPWATCH_READY;
}
 
/****************************************************************
** Function name:           Watch_Toggle
** Descriptions:            暂停、继续秒表,亮屏状态下短按按键切换处理
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-05-15
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------*/
void Watch_Toggle(void)
{
    if(stopwatch_config.state == STOPWATCH_STOP 
        || stopwatch_config.state == STOPWATCH_READY)
    {
        Watch_Start();
    }
    else if(stopwatch_config.state == STOPWATCH_START)
    {
        Watch_Stop();
    }
}

/****************************************************************
** Function name:           Watch_GetTotalSecond
** Descriptions:            获取秒表的总秒数,用于ＵＩ显示
** input parameters:        无
** output parameters:       无
** Returned value:          返回:秒表当前的总秒数  0~5999                      
** Created by:              WJ              
** Created Date:            2017-05-15
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------*/
uint16_t Watch_GetTotalSecond(void)
{
    return stopwatch_config.total_second;
}

/****************************************************************
** Function name:           Watch_Get100Millisecond
** Descriptions:            获取秒表当前的100毫秒数
** input parameters:        无
** output parameters:       无
** Returned value:          返回:秒表当前的100毫秒数 0~9                     
** Created by:              WJ              
** Created Date:            2017-05-15
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------*/
uint8_t Watch_Get100Millisecond(void)
{
    return stopwatch_config.stopwatch_100millisecond;
}

/****************************************************************
** Function name:           Watch_IsRun
** Descriptions:            秒表已经开始，秒表运转
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              JSH              
** Created Date:            2018-05-29
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------*/
uint8_t Watch_IsRun(void)
{
    if(stopwatch_config.state == STOPWATCH_START)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/***************************************************秒表处理结束******************************************/

/****************************************************倒计时处理********************************************/

/****************************************************************
** Function name:           Countdown_Init
** Descriptions:            倒计时初始化,用于执行的数据
** input parameters:        无
** output parameters:       无
** Returned value:          无                 
** Created by:              WJ              
** Created Date:            2017-05-15
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------*/
void Countdown_Init(void)
{
    memset(&countdown_config, 0x00, sizeof(countdown_config_t));    
}
 
/****************************************************************
** Function name:           Countdown_SetShowUI
** Descriptions:            倒计时UI响应,项目相关
** input parameters:        无
** output parameters:       无
** Returned value:          无                   
** Created by:              WJ              
** Created Date:            2017-05-15
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------*/
void Countdown_SetShowUI(uint8_t type)
{ 
    if(type == 0)
    {      
		if((Gui_GetThreadTask() == CPU_TASK_COUNT_DOWN)||( Gui_GetThreadJob() == CPU_EVENT_TASK_COUNT_DOWN_START))
		{
			count_down_timeout_flag = 0;
		}
		else
		{
			count_down_timeout_flag = 1;
		}
		
        Gui_ThreadEventTaskWakeUp(CPU_EVENT_TASK_COUNT_DOWN_TIMEOUT);  
    }
    else
    {   //app开启单次倒计时,
		count_down_start_flag = 1;
        Gui_ThreadEventTaskWakeUp(CPU_EVENT_TASK_COUNT_DOWN_START);  
    }     
}
 
/****************************************************************
** Function name:           Countdown_SetTime
** Descriptions:            倒计时设置，来自通讯协议
** input parameters:        second:倒计时总秒数;
                            state:;
                            id:暂时没有使用;
                            type:
** output parameters:       无
** Returned value:          返回0:设置失败;1:设置成功                    
** Created by:              WJ              
** Created Date:            2017-05-15
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------*/
uint8_t Countdown_SetTime(uint32_t second, uint8_t state, uint8_t id, uint8_t type)
{
    if(id >= COUNTDOWN_MAX_NUM || (second == 0 && type != 0 ) || second > 86400) return 0;    
    
    if(countdown_config.state || state == 0)  //倒计时正在运行
    {
        return 0;
    }
    
    if(type == 1 || type == 0)
    {
        system_info_s.countdown_config.countdown_second = second;   
        system_info_s.countdown_config.state = state;
        system_info_s.countdown_config.id = id;
        system_info_s.countdown_config.type = type;
        App_SetSystemInfoSave();
    }
    else
    {    
        countdown_config.countdown_second = second;    
        countdown_config.state = state;
        countdown_config.id = id;
        countdown_config.type = 2;
        //触发倒计时开始
        Countdown_SetShowUI(1);
    }
    
    return  1;
}
 
/****************************************************************
** Function name:           Countdown_SetTimeBySystemStart
** Descriptions:            从多次开始倒计时,按键开启
** input parameters:        无
** output parameters:       无
** Returned value:          返回 1:设置成功                    
** Created by:              WJ              
** Created Date:            2017-05-15
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------*/
uint8_t Countdown_SetTimeBySystemStart(void)
{    
    if(countdown_config.state == COUNTDOWN_STATE_IDE)
    {
        countdown_config.countdown_second = system_info_s.countdown_config.countdown_second; 
        countdown_config.id = system_info_s.countdown_config.id;
        countdown_config.type = system_info_s.countdown_config.type;
    }
    //WJ.20180809.为了增加暂停状态    
    countdown_config.state = COUNTDOWN_STATE_RUN;  //1;
    
    return  1;
}
 
/****************************************************************
** Function name:           Countdown_SetTimeBySystemPause
** Descriptions:            倒计时暂停
** input parameters:        无
** output parameters:       无
** Returned value:          返回 1:设置成功                    
** Created by:              WJ              
** Created Date:            2018-08-09
**----------------------------------------------------------------*/
uint8_t Countdown_SetTimeBySystemPause(void)
{    
    if(countdown_config.state == COUNTDOWN_STATE_RUN)
    {    
        countdown_config.state = COUNTDOWN_STATE_PAUSE;  //1;
    }
    
    return  1;
}

/****************************************************************
** Function name:           Countdown_Get
** Descriptions:            获取当前正在执行的倒计时配置
** input parameters:        无
** output parameters:       无
** Returned value:          返回 1:?                
** Created by:              WJ              
** Created Date:            2017-05-15
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------*/
countdown_config_t *Countdown_Get(void)
{
    return  &countdown_config;
}
 
/****************************************************************
** Function name:           Countdown_GetState
** Descriptions:            获取正在运行的倒计时状态
** input parameters:        无
** output parameters:       无
** Returned value:          返回0:倒计时未启动; 1:倒计时正在运行                
** Created by:              WJ              
** Created Date:            2017-05-15
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------*/
uint8_t Countdown_GetState(void)
{
    return countdown_config.state;
}
 
/****************************************************************
** Function name:           Countdown_SendDataToApp
** Descriptions:            发送响应数据到APP,倒计时进行的时候发送
** input parameters:        无
** output parameters:       无
** Returned value:          无              
** Created by:              WJ              
** Created Date:            2017-05-15
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------*/
void Countdown_SendDataToApp(uint8_t ack, uint8_t operate)
{    
    uint8_t ble_ack[20] = {0x00, };
    
    ble_ack[0] = 0xB2;   
    if(operate <= 0x02)
    {
        ble_ack[1] = operate;
    }
    else if(operate == 0xFF)
    {
         if(countdown_config.state == 0)
        {
            ble_ack[1] = 0x04; 
        }
        else
        {            
            ble_ack[1] = 0x03;
        }
    }
    else
    {  
        return;
    }        
    
    ble_ack[2] = ack;         
    ble_ack[3] = countdown_config.id; 
    
    ble_ack[4] = (system_info_s.countdown_config.countdown_second) & 0xFF;         
    ble_ack[5] = (system_info_s.countdown_config.countdown_second >> 8) & 0xFF;  
    ble_ack[6] = (system_info_s.countdown_config.countdown_second >> 16) & 0xFF;     
    
    ble_ack[7] = (system_info_s.countdown_config.type == 1)?1:0; 

    if(countdown_config.state)
    {
        ble_ack[8] = (countdown_config.countdown_second) & 0xFF;         
        ble_ack[9] = (countdown_config.countdown_second >> 8) & 0xFF;  
        ble_ack[10] = (countdown_config.countdown_second >> 16) & 0xFF;     
        ble_ack[11] = (countdown_config.type < 2)?1:0; 
    }
    Ble_SendDataViaBat(ble_ack, 20);    
}

/****************************************************************
** Function name:           Countdown_ToRunSecond
** Descriptions:            倒计时跑秒处理,放在一秒定时器中断中
** input parameters:        无 
** output parameters:       无
** Returned value:          无               
** Created by:              WJ              
** Created Date:            2017-05-15
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------*/
void Countdown_ToRunSecond(void)
{    
    if(countdown_config.state == COUNTDOWN_STATE_RUN)
    {
        if(countdown_config.countdown_second > 0)
        {
            countdown_config.countdown_second--;
        }
        if(countdown_config.countdown_second == 0)
        {
            Countdown_Init();
            //countdown_config.state = 0;
            //触发倒计时结束
            Countdown_SetShowUI(0);  
            Countdown_SendDataToApp(1, 0xFF);
        }
        else
        {
            Countdown_SendDataToApp(1, 0xFF);
        }
    }  
}

/****************************************************************
** Function name:           Countdown_GetCurrentSecond
** Descriptions:            获取倒计时的剩余时间
** input parameters:        无 
** output parameters:       无
** Returned value:          无               
** Created by:              JSH              
** Created Date:            2018-07-17
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------*/
uint32_t Countdown_GetCurrentSecond(void)
{    
    return countdown_config.countdown_second;
}
