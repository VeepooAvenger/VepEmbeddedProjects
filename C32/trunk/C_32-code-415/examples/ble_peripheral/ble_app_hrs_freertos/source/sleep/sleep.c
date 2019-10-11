/***************************Copyright(c)***********************************
**                       Veepoo Tech
**                  COPYRIGHT (C) 2016-2019
**-----------File Info-----------------------------------------------------
** File        name:     sleep.c
** Last modified Date:   2017-07-28
** Last SYS_VERSION:     0.0.0
** Descriptions:            
**-------------------------------------------------------------------------
** Created  by:          WJ
** Created  date:        2017-07-28
** SYS_VERSION:          0.0.0
** Descriptions:         
**-------------------------------------------------------------------------
** Modified by:				
** Modified date:  			          
** SYS_VERSION:                 
** Descriptions:                        
***************************************************************************/
#include "sleep.h"
#include "daily_info.h"
#include "system_clock.h"
#include "app_flash.h"
#include "app_type.h"

/*********************************************************************
* Macro Definition
*/

#define SLEEP_8PM_NUM       48


/*********************************************************************
* Global Variables
*/
//static uint8_t sleep_state[1024] = {0x00,};
//static uint16_t sleep_state_cnt = 0;
extern system_flag_t system_flag_s;
/*********************************************************************
* local functions
*/

/****************************************************************
** Function name:           Sleep_GetNumberOfSleep
** Descriptions:            ��ȡ��ǰ˯�ߴ���
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
static uint16_t Sleep_GetNumberOfSleep(uint16_t data)
{
    uint8_t i = 0;
    uint16_t sleep_cnt = 0;
    
    for(i = 15; i > 0; i--)
    {
        if((data & (1 << i)) == 0)
        {
            sleep_cnt++;
        }
        else
        {
            break;
        }
    }
    
    return sleep_cnt;
}

/****************************************************************
** Function name:           Sleep_SetNumberOfSleep
** Descriptions:            ���õ�ǰ˯�ߴ���
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
static uint16_t Sleep_SetNumberOfSleep(uint16_t sleep_cnt)
{
    uint16_t sleep_data = 0xFFFF;
    
    sleep_data = sleep_data >> sleep_cnt;
    
    return sleep_data;
}

/****************************************************************
** Function name:           Sleep_SaveData
** Descriptions:            ˯�����ݼ�飬��ֹ����˯���غ�
** input parameters:        �����������š�˯�ߴ�����Ҫ�洢��˯������
** output parameters:
** Returned value:          ����ֵ:0 ���ʧ�ܣ��������� 1���ݺ���
** Created by:              WJ             
** Created Date:            2017-01-04
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
static uint8_t SleepDataCheck(const uint8_t vstg_block_num,const uint8_t sleep_cnt,const vph_sleep_quality_info_t vph_sleep_quality_info)
{
    uint32_t time1 = 0;
    uint32_t time2 = 0;  
	vph_sleep_quality_info_t sleep_quality_info;  
    
    if(sleep_cnt == 1) return 1;//�ɹ�
  
    if(sleep_cnt > 1 && sleep_cnt < 10)    
    { 
        App_FlashReadDailySleep(vstg_block_num, sleep_cnt - 2, (void *)&sleep_quality_info);
        
        if(vph_sleep_quality_info.sleep_enter_time[0] == sleep_quality_info.sleep_out_time[0]  //�·�һ��
        && vph_sleep_quality_info.sleep_enter_time[1] == sleep_quality_info.sleep_out_time[1])//����������Ҫ���
        {
            time1 = vph_sleep_quality_info.sleep_enter_time[2]*60 + vph_sleep_quality_info.sleep_enter_time[3];           
            time2 = sleep_quality_info.sleep_out_time[2]*60 + sleep_quality_info.sleep_out_time[3];
            
            if(time1 > time2)
            {
                return 1;//����ʱ�䲻�غ�
            }
        }
        else//����Ⱦͷ��سɹ�
        {
            return 1;
        }
    }
    return 0;
}

/****************************************************************
** Function name:           Sleep_SaveData
** Descriptions:            
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
void Sleep_SaveData(void)
{
    uint8_t curr_month = 0;
    uint8_t curr_day = 0;
    uint8_t curr_sleep_cnt = 0;
    uint8_t curr_sleep_block = 0;
    const vph_sleep_info_t *p_sleep_info = NULL;			//��ʼ��˯����Ϣ�洢����
	vph_sleep_quality_info_t vsis_sleep_quality_info;
    daily_block_info_t daily_block_info;
    daily_pkg_info_t *daily_pkg_info;
    uint8_t yester_day = 0;
    
	memset(&vsis_sleep_quality_info, 0, sizeof(vph_sleep_quality_info_t));    
    p_sleep_info = Vs_GetSleepingInfo();   

    curr_day = clock->day;
    curr_month = clock->month;   
    
	vsis_sleep_quality_info.head = 0xA1;		//���ݱ�־
	/*---------------------------- ��������ʱ��  -------------------------*/  
	vsis_sleep_quality_info.sleep_out_time[0] = curr_month;        //��
	vsis_sleep_quality_info.sleep_out_time[1] = p_sleep_info->quit_time.date;
	vsis_sleep_quality_info.sleep_out_time[2] = p_sleep_info->quit_time.hour;
	vsis_sleep_quality_info.sleep_out_time[3] = p_sleep_info->quit_time.minute;

    //��˯ʱ�䴦��
    if(p_sleep_info->quit_time.date != p_sleep_info->enter_time.date)   //��˯���˳�����ͬһ��
    {
        //��˯ʱ����ǰһ��
        //WJ.20180104.˯����1�·ݣ���˯���˳�˯�߲���ͬһ���bug
        //if(vsis_sleep_quality_info.sleep_out_time[0] == 1)
        if(p_sleep_info->quit_time.date == 1 && vsis_sleep_quality_info.sleep_out_time[0] == 1)
        {   //�˳�˯����1��1��
            vsis_sleep_quality_info.sleep_enter_time[0] = 12;
        }
        else
        {
            Sclock_GetPreviousDayDate(1, &curr_month, &yester_day);
            vsis_sleep_quality_info.sleep_enter_time[0] = curr_month;   //WJ.20170303.������˯�·ݴ����BUG
        }
    }
    else    //��˯���˳�˯����ͬһ��
    {
        if(p_sleep_info->quit_time.date != curr_day)   //��ʾ���������
        {
            Sclock_GetPreviousDayDate(1, &curr_month, &yester_day);
            //vsis_sleep_quality_info.sleep_enter_time[0] = curr_month;   //WJ.20170619.�����˳�˯���·ݴ����BUG            
        }
        //else
        //{
            vsis_sleep_quality_info.sleep_enter_time[0] = curr_month;
        //}
    }
    
	/*-------------- ������˯ʱ  --------------*/
	vsis_sleep_quality_info.sleep_enter_time[1] = p_sleep_info->enter_time.date;
	vsis_sleep_quality_info.sleep_enter_time[2] = p_sleep_info->enter_time.hour;
	vsis_sleep_quality_info.sleep_enter_time[3] = p_sleep_info->enter_time.minute;
	/*---------------------------- ����˯��ʱ�估����  -------------------------*/
	vsis_sleep_quality_info.sleep_quality[0] = p_sleep_info->deep_time;				//��˯ֵ
	vsis_sleep_quality_info.sleep_quality[1] = p_sleep_info->shallow_time;			//ǳ˯ֵ
	vsis_sleep_quality_info.sleep_quality[2] = p_sleep_info->quanlity_val;			//˯������
	vsis_sleep_quality_info.total_time = p_sleep_info->deep_time + p_sleep_info->shallow_time;		//��˯��ʱ�䳤��
	/*------------------------------- ����˯������ -----------------------------*/
	memcpy(vsis_sleep_quality_info.sleep_shallow, p_sleep_info->deep_shallow, 31);		//����˯�����ߣ� ����Ϊϴ��֧�ֳ���31��byte���ɴ洢20��Сʱ����
	/*------------------------------- ������;����ʱ�� -------------------------------*/
    vsis_sleep_quality_info.start_time[0] = 209;
	memcpy(&vsis_sleep_quality_info.start_time[1], p_sleep_info->start_time, 11);
    vsis_sleep_quality_info.end_time[0] = 210;
	memcpy(&vsis_sleep_quality_info.end_time[1], p_sleep_info->end_time, 11);    
    
    //����˯�����ݵ�flash
    daily_pkg_info = Daily_GetPkgInfo();  //��ȡ�����ճ����ݴ洢�����Ϣ
    curr_sleep_block = daily_pkg_info->block;
    if(p_sleep_info->quit_time.date != p_sleep_info->enter_time.date 
        || p_sleep_info->enter_time.hour < 8 
        || p_sleep_info->enter_time.date != clock->day)
    {
        //���浽����
        curr_sleep_block = Daily_GetPreviousBlock(1);        
        Daily_GetBlockHeadInfo(&daily_block_info, curr_sleep_block);
        if(daily_block_info.block_flag != 0x00)
        {
            daily_block_info.block_flag = 0x00;
        }
    }
    else
    { // ���浽����
		// WJ.20181122.�����0��������ݣ����������˯��ʱ�䣬������
        Daily_GetBlockHeadInfo(&daily_block_info, curr_sleep_block);
    }

    curr_sleep_cnt = Sleep_GetNumberOfSleep(daily_block_info.sleep_cnt);
    curr_sleep_cnt++;
    
    if(!SleepDataCheck(curr_sleep_block, curr_sleep_cnt, vsis_sleep_quality_info))
    {
        return;
    }
    
    App_FlashWriteDailySleep(curr_sleep_block, (curr_sleep_cnt - 1), (void *)&vsis_sleep_quality_info, 
                                    sizeof(vph_sleep_quality_info_t) );
    daily_block_info.sleep_cnt = Sleep_SetNumberOfSleep(curr_sleep_cnt);
    //����ͷ����Ϣ
    App_FlashWriteDailyHead(curr_sleep_block, (void *)&daily_block_info, sizeof(daily_block_info_t));      
}

/****************************************************************
** Function name:           Sleep_LowBateeryProcess
** Descriptions:            �͵���ʱ˯���㷨���� 
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
void Sleep_LowBateeryProcess(void)
{
    const vph_sleep_info_t *p_sleep_info = NULL;			//��ʼ��˯����Ϣ�洢����
    daily_pkg_info_t *daily_pkg_info;

    p_sleep_info = Vs_GetSleepingInfo();

    if(p_sleep_info->sleep_flag != 0x01)
    {
        Vs_ClearInfoMsg();      //���˯�߻�������
        return;
    }
    
    daily_pkg_info = Daily_GetPkgInfo();
    
    Vph_SleepLowBattery(daily_pkg_info->pkg_cnt);
    
    p_sleep_info = Vs_GetSleepingInfo();
    
    //����3�����˳�˯�ߣ������ȡ���洢˯�����ݣ�������һ��˯���ж��Զ����˯�߻�������
    if(p_sleep_info->sleep_flag == 3)		
    {
        Sleep_SaveData();		//�˺�����11��Ϊ��׼�������
        Vs_ClearInfoMsg();      //���˯�߻�������
    }
}

/****************************************************************
** Function name:           Sleep_NormalProcess
** Descriptions:            ��������µ�˯�ߴ��� 
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
void Sleep_NormalProcess(void)
{
    uint8_t error_cod = 0;
    daily_pkg_info_t *daily_pkg_info;
	const vph_sleep_info_t *p_sleep_info = NULL;			//��ʼ��˯����Ϣ�洢����
    
    daily_pkg_info = Daily_GetPkgInfo();
        
    //����˯�߼��㣬������ش���
    error_cod = Vph_SleepDetectEntery(daily_pkg_info->pkg_cnt);
    Daily_SetSleepCode(error_cod);
//    Daily_SetSleepCode(Vph_SleepDetectEntery(daily_pkg_info->pkg_cnt));
    p_sleep_info = Vs_GetSleepingInfo();
    
    if(p_sleep_info == NULL)
    {
        return;
    }
    if(p_sleep_info->sleep_flag == 0x01)
    {
        system_flag_s.sleep_enter = 0x01;
    }
    else
    {
        system_flag_s.sleep_enter = 0x00;
    }
    
    if(p_sleep_info->sleep_flag == 3)		
    {
        Sleep_SaveData();
        Vs_ClearInfoMsg();        
    }     
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
uint16_t Sleep_GetTime(void)
{
    uint8_t i =0;
    uint8_t curr_sleep_block = 0;
    uint8_t curr_sleep_cnt = 0;    
    uint16_t sleep_total_minute = 0;
	vph_sleep_quality_info_t sleep_quality_info;   
    daily_block_info_t daily_block_info;

    uint8_t sleep_new_time[12] = {0x00, };     
    uint8_t sleep_old_time[12] = {0x00, };    
    
    //��ȡ��������ݿ��
    curr_sleep_block = Daily_GetPreviousBlock(1); 
    //��ȡ��������ݿ���Ϣ
    Daily_GetBlockHeadInfo(&daily_block_info, curr_sleep_block);
    if(daily_block_info.block_flag == 0x00)
    {
        curr_sleep_cnt = Sleep_GetNumberOfSleep(daily_block_info.sleep_cnt);
        for( i = 0; i < curr_sleep_cnt; i++)
        {
            memset(&sleep_quality_info, 0x00, sizeof(vph_sleep_quality_info_t));                        
            App_FlashReadDailySleep(curr_sleep_block, i, (void *)&sleep_quality_info);

            memcpy(sleep_new_time, (void *)&sleep_quality_info, 12);
            if(Sleep_Adjust(sleep_new_time, sleep_old_time))
            {
                sleep_total_minute += sleep_quality_info.total_time;
            }
            else
            {
                continue;
            }
  
            memcpy(sleep_old_time, (void *)&sleep_quality_info, 12);
        }       
    }
    
    return sleep_total_minute * 5 ;
}

/****************************************************************
** Function name:           Sleep_Init
** Descriptions:            ��ʼ��
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
void Sleep_Init(void)
{    
    Vph_SleepingInit(&Daily_SleepGetDailyDataCallback);
    Vs_ClearInfoMsg();   
}

/****************************************************************
** Function name:           Sleep_Repetition
** Descriptions:            ˯�߱���
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
void Sleep_Repetition(void)
{
    uint8_t error_cod = 0;
    uint8_t curr_sleep_block = 0;
    uint8_t curr_sleep_cnt = 0;    
    uint16_t daily_pkg = 0;
    uint16_t daily_max_pkg = 0;
    daily_block_info_t daily_block_info;

    daily_pkg_info_t *daily_pkg_info_p = NULL;   
    uint8_t curr_daily_block = 0;       //���ڼ�¼��ǰ��ʵ�����ݴ洢���
    uint16_t curr_daily_pkg = 0;        //���ڼ�¼��ǰ��ʵ�����ݴ洢����

    const vph_sleep_info_t *p_sleep_info = NULL;			//��ʼ��˯����Ϣ�洢����
//    daily_pkg_info_t *daily_pkg_info;    
    
    uint8_t y_month = 0;
    uint8_t y_day = 0; 
    
    //��������8��,����16�㲻�ټ���
    if(clock->hour < 8)/* || clock->hour > 16)*/ return; 
        
    //��ȡ��������ݿ��
    curr_sleep_block = Daily_GetPreviousBlock(1); 
    //��ȡ��������ݿ���Ϣ
    Daily_GetBlockHeadInfo(&daily_block_info, curr_sleep_block);

    //��ȡ����˯�߼�¼����
    curr_sleep_cnt = Sleep_GetNumberOfSleep(daily_block_info.sleep_cnt);   
    
    Sclock_GetPreviousDayDate(1, &y_month, &y_day); //��ȡ������·ݺ�����
    
    //��¼��ǰ��š�������Ϣ��������ɺ���Ҫ��ԭ
    daily_pkg_info_p = Daily_GetPkgInfo();
    curr_daily_block = daily_pkg_info_p->block;
    curr_daily_pkg = daily_pkg_info_p->pkg_cnt;

    if(daily_block_info.block_flag != 0x00 || daily_block_info.pack_num == 0
        || (daily_block_info.month != y_month || daily_block_info.day != y_day)) 
    {   //���첻�������ݣ�˯�߱����ӽ�������ݿ�ʼ
        curr_sleep_cnt = 0;
        daily_pkg = 0;
        curr_sleep_block = curr_daily_block;
        daily_max_pkg = curr_daily_pkg;
    }
    else
    {
        if(daily_block_info.pack_num >= SLEEP_8PM_NUM)
        {
            daily_pkg = daily_block_info.pack_num - SLEEP_8PM_NUM + 1;
        }
        else
        {
            daily_pkg = 1;//daily_block_info.pack_num; 
        }       
        daily_max_pkg = daily_block_info.pack_num;    
    }
    if(curr_sleep_cnt != 0) return;     //��˯�߼�¼���ټ���
    

    //��ʼ��˯�߲���
    Vs_ClearInfoMsg(); 
    do{        
        //���õ�ǰ��ʼ�����ţ�����   
        Daily_SetDailyPkgInfo(curr_sleep_block, daily_pkg++);
        
        //����˯�߼��㣬������ش���
        error_cod = Vph_SleepDetectEntery(daily_pkg_info_p->pkg_cnt);
        Daily_SetSleepCode(error_cod);     
        
//        Daily_SetSleepCode(Vph_SleepDetectEntery(daily_pkg_info_p->pkg_cnt));
        p_sleep_info = Vs_GetSleepingInfo();       
        //����3�����˳�˯�ߣ������ȡ���洢˯�����ݣ�������һ��˯���ж��Զ����˯�߻�������
        if(p_sleep_info->sleep_flag == 3)		
        {           
            Sleep_SaveData();		//�˺�����11��Ϊ��׼�������
            Vs_ClearInfoMsg();      //���˯�߻�������
        }        
    
        if(daily_pkg > daily_max_pkg)   //����
        {
            daily_pkg = 1;
            if(curr_sleep_block != curr_daily_block)
            {
                curr_sleep_block = curr_daily_block;
                daily_max_pkg = curr_daily_pkg;
            }
            else
            {
                break;
            }
        }        
    }while(1);
    

    //��ԭ��ǰ��š�������Ϣ
    Daily_SetDailyPkgInfo(curr_daily_block, curr_daily_pkg);
}
