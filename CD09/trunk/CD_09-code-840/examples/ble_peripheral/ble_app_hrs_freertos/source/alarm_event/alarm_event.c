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

//__ALIGN(4) ����ǿ�Ʒ��������ʼ��ַΪ4�ı���
__ALIGN(4) static alarm_clock_config_t alarm_clock_config[ALARM_CLOCK_MAX_NUM];    //����ṹ����� 20��
static uint8_t alarm_clock_save = 0;                //�Ƿ���Ҫ�����������ݵı�־λ
static uint8_t alarm_clock_save_delay = 0;          //��ʱ�洢
static alarm_clock_data_t alarm_clock_data;         //�����������ݵ�����
static alarm_clock_do_t alarm_clock_do;

static countdown_config_t countdown_config = {0,};      //����ʱ

static stopwatch_config_t stopwatch_config = {0x00, };  //���

extern system_info_t system_info_s;   //��ε���ʱ���ݴ������ȡ

static uint8_t alarm_write_flag = 0;        // ����д���ݵĿ��Ʊ�־

uint8_t count_down_start_flag = 0;			// ����ʱ������־

uint8_t count_down_timeout_flag = 0;			// ����ʱ��ʱ��־

/*******************************************************���Ӵ���******************************************/

/****************************************************************
** Function name:           Alarm_GetAlarmClockDo
** Descriptions:            ��ȡ���ִ�е���������
** input parameters:        ��
** output parameters:       ��
** Returned value:          �����������ݽṹ��     
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
** Descriptions:            �����������ݷ��͵�����
** input parameters:        start_index:������ʼ��ַ(�����±�);total_index:��Ч���Ӹ���;
                            crc16:��ǰ�豸���ӵ�CRC
** output parameters:       ��
** Returned value:          ��
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
** Descriptions:            �����������ݵ�APP��ÿ�η���һ�飬��Ҫ��ʱѭ������
** input parameters:        ��
** output parameters:       ��
** Returned value:          ����0:����ȫ���������;1:����������Ҫ����
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
    {   //���ͽ�����
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
        alarm_clock_data.alarm_index++; //������һ�����ݵ���ʼ�±�
        if(alarm_clock_config[i].valid)
        {           
            alarm_data_sned[5] = i+1;
            memcpy(&alarm_data_sned[6], &alarm_clock_config[i].hour, 9);
            alarm_data_sned[18] = (alarm_clock_data.alarm_crc16     )&0xFF;
            alarm_data_sned[19] = (alarm_clock_data.alarm_crc16 >> 8)&0xFF;
            Ble_SendDataViaBat(alarm_data_sned, 20); 
            break;      //ÿ�η���һ����Ч���ݾͽ�����ѯ
        }        
    }
    
    if(i >= ALARM_CLOCK_MAX_NUM)
    {   //û����Ч������
        if(alarm_clock_data.alarm_num_start < alarm_clock_data.alarm_num_total)
        {   //�쳣���·��͵����ݰ���Ҫ���͵��٣��´��ٷ�һ��������
            alarm_clock_data.alarm_num_start = alarm_clock_data.alarm_num_total;
            return 1;
        }
        return 0;
    }
    
    return 1;
}

/****************************************************************
** Function name:           Alarm_ClearByID
** Descriptions:            ͨ��ID������ӣ���Ҫ����
** input parameters:        ��
** output parameters:       ��
** Returned value:          ����0:id��Ч,ɾ����Ч;1:����ɾ���ɹ�
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
    alarm_clock_save_delay = 0;         //������ʱ
    alarm_clock_save = 1;               //������Ҫ�������ӱ�־λ
    
    return 1;
}

/****************************************************************
** Function name:           Alarm_InitAll
** Descriptions:            ������������,������Ҫ���棬���ڻָ��������ã�
                            ������������������û����ݿ������������ʵ�����ʹ��
** input parameters:        ��
** output parameters:       ��
** Returned value:          ����1:�������óɹ�
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
** Descriptions:            ������������,��Ҫ����
** input parameters:        ��
** output parameters:       ��
** Returned value:          ����1:�������óɹ�
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
** Descriptions:            ��������ʱ���Ƿ��ѹ�,����APP��������У�飬�����Լ�
** input parameters:        ��
** output parameters:       ��
** Returned value:          ����0:���ǵ������ӻ�������û��ʧЧ; 1:��������ʧЧ
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
** Descriptions:            ���ӵ���ʱ��
** input parameters:        ��
** output parameters:       ��
** Returned value:          ����0:����δ��ʱ��; 1:�����ѵ�ʱ��
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
** Descriptions:            �������ӣ���ʱ��ִ�����Ӵ��������Ӵ���
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
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
			//1������ǵ�������,����Ҫ�ر�
			if((1 == p_current_alarm->state) && (0x00 == p_current_alarm->repeat) 
              && clock->year == p_current_alarm->year 
              && clock->month == p_current_alarm->month 
              && clock->day == p_current_alarm->day)
			{
				alarm_is_arrive = 1;
				p_current_alarm->state = 0;
                scene_mode = p_current_alarm->scene_mode;
                break;      //�Ҵ�һ���������ӾͲ��ټ�������
			}
            //2���ж��Ƿ����ظ�����
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
					//���崥��
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
** Descriptions:            �������������Լ죬�ж�������Ч����
** input parameters:        ��
** output parameters:       ��
** Returned value:          ����0:������Ч; 1:������Ч
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
** Descriptions:            ��ȡ���ӵ���Ч����
** input parameters:        ��
** output parameters:       ��
** Returned value:          ����:��Ч���Ӹ���
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
** Descriptions:            ��������,��������Э��(APP����),����Ϊ���ӵ���Ч����,
                            ��ID��ʼ,�ܼ�10���ֽڣ�����
** input parameters:        ��
** output parameters:       ��
** Returned value:          ����0:������Ч,����ʧ��; 1:������Ч,���óɹ�
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

	//�ж������Ƿ���Ч
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
        
        //����     
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
** Descriptions:            �������ӵ�CRC
** input parameters:        ��
** output parameters:       ��
** Returned value:          ����:��Ч���ӵ�CRC(�����Ѿ�ִ��ʧЧ������)
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
** Descriptions:            ������д�뵽flash��Ŀǰ52��������10s�ı�����ʱ,��1���¼���ִ��
** input parameters:        ��
** output parameters:       ��
** Returned value:          ����:��Ч���ӵ�CRC(�����Ѿ�ִ��ʧЧ������)
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
    
    if(alarm_clock_save != 1)           //�Ƿ���������Ҫ����
    {
        alarm_clock_save_delay = 0;
        return ret;
    }
    if(alarm_clock_save_delay++ < 5) return ret;       //10s��ʱ
    
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
** Descriptions:            �ػ�������������
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
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
** Descriptions:            ��flash��ȡ�������ݣ�ע��������ƽ̨���ײ�������
** input parameters:        ��
** output parameters:       ��
** Returned value:          ����:��ȡ״̬
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
** Descriptions:            �����Լ죬�����Ч���ӣ��ϵ硢���߶�ʱ�Լ�
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
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
** Descriptions:            �Լ쵥�����ӣ���ʱ����Ҫ����״̬
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
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
        
        //1������ǵ�������,����Ҫ�ر�
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
** Descriptions:            ���ӳ�ʼ�����Լ졢��ʱ�Լ�
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
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
/***************************************************���Ӵ������******************************************/

/*****************************************************�����********************************************/

/****************************************************************
** Function name:           Watch_100ms_Start
** Descriptions:            ��������ʱ��100���붨ʱ������
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
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
** Descriptions:            ��������ʱ��100���붨ʱ���ر�
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
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
** Descriptions:            ��������ʱ��100���붨ʱ���¼�
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
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
** Descriptions:            ����ʼ��,(�ػ�����������)
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
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
** Descriptions:            �������Ϊ����״̬,(3s����,��ʼ����ɣ����������Ϊ����״̬)
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-05-15
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------*/
void Watch_SetReady(void)
{
    stopwatch_config.state = STOPWATCH_READY;        //����
}
 
/****************************************************************
** Function name:           Watch_Start
** Descriptions:            ���Ӿ�����ֹͣ״̬����
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
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
** Descriptions:            ֹͣ���,����ֹͣ��������ʱ��������
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
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
** Descriptions:            ��ȡ���ĵ�ǰ״̬
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
                            STOPWATCH_IDE,          //׼��
                            STOPWATCH_READY,        //����
                            STOPWATCH_START,        //��ʼ
                            STOPWATCH_STOP,         //ֹͣ
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
** Descriptions:            ����Ѿ�׼������,����æµ״̬
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
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
** Descriptions:            ��ͣ���������,����״̬�¶̰������л�����
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
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
** Descriptions:            ��ȡ����������,���ڣգ���ʾ
** input parameters:        ��
** output parameters:       ��
** Returned value:          ����:���ǰ��������  0~5999                      
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
** Descriptions:            ��ȡ���ǰ��100������
** input parameters:        ��
** output parameters:       ��
** Returned value:          ����:���ǰ��100������ 0~9                     
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
** Descriptions:            ����Ѿ���ʼ�������ת
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
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

/***************************************************��������******************************************/

/****************************************************����ʱ����********************************************/

/****************************************************************
** Function name:           Countdown_Init
** Descriptions:            ����ʱ��ʼ��,����ִ�е�����
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��                 
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
** Descriptions:            ����ʱUI��Ӧ,��Ŀ���
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��                   
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
    {   //app�������ε���ʱ,
		count_down_start_flag = 1;
        Gui_ThreadEventTaskWakeUp(CPU_EVENT_TASK_COUNT_DOWN_START);  
    }     
}
 
/****************************************************************
** Function name:           Countdown_SetTime
** Descriptions:            ����ʱ���ã�����ͨѶЭ��
** input parameters:        second:����ʱ������;
                            state:;
                            id:��ʱû��ʹ��;
                            type:
** output parameters:       ��
** Returned value:          ����0:����ʧ��;1:���óɹ�                    
** Created by:              WJ              
** Created Date:            2017-05-15
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------*/
uint8_t Countdown_SetTime(uint32_t second, uint8_t state, uint8_t id, uint8_t type)
{
    if(id >= COUNTDOWN_MAX_NUM || (second == 0 && type != 0 ) || second > 86400) return 0;    
    
    if(countdown_config.state || state == 0)  //����ʱ��������
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
        //��������ʱ��ʼ
        Countdown_SetShowUI(1);
    }
    
    return  1;
}
 
/****************************************************************
** Function name:           Countdown_SetTimeBySystemStart
** Descriptions:            �Ӷ�ο�ʼ����ʱ,��������
** input parameters:        ��
** output parameters:       ��
** Returned value:          ���� 1:���óɹ�                    
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
    //WJ.20180809.Ϊ��������ͣ״̬    
    countdown_config.state = COUNTDOWN_STATE_RUN;  //1;
    
    return  1;
}
 
/****************************************************************
** Function name:           Countdown_SetTimeBySystemPause
** Descriptions:            ����ʱ��ͣ
** input parameters:        ��
** output parameters:       ��
** Returned value:          ���� 1:���óɹ�                    
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
** Descriptions:            ��ȡ��ǰ����ִ�еĵ���ʱ����
** input parameters:        ��
** output parameters:       ��
** Returned value:          ���� 1:?                
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
** Descriptions:            ��ȡ�������еĵ���ʱ״̬
** input parameters:        ��
** output parameters:       ��
** Returned value:          ����0:����ʱδ����; 1:����ʱ��������                
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
** Descriptions:            ������Ӧ���ݵ�APP,����ʱ���е�ʱ����
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��              
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
** Descriptions:            ����ʱ���봦��,����һ�붨ʱ���ж���
** input parameters:        �� 
** output parameters:       ��
** Returned value:          ��               
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
            //��������ʱ����
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
** Descriptions:            ��ȡ����ʱ��ʣ��ʱ��
** input parameters:        �� 
** output parameters:       ��
** Returned value:          ��               
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
