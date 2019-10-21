/*******************Copyright(c)*********************************
** Veepoo Tech
** COPYRIGHT (C) 2016-2019
**----------------------FileInfo---------------------------------
** File name:                 sleep.c
** Last modified Date:         2017-08-12
** Last SYS_VERSION:           0.0.1
** Descriptions:
**---------------------------------------------------------------
** Createdby:                  WJ
** Createddate:                2017-01-04
** SYS_VERSION:                0.0.1
** Descriptions:               ˯��
**---------------------------------------------------------------
** Modified by:				
** Modified date:			
** SYS_VERSION:				
** Modified Descriptions:	
****************************************************************/

#include "sleep.h"
#include "SystemTime.h"
#include "api_flash.h"
#include "app_type.h"
#include "log.h"
#include "flash_common.h"

/****************************************************************
** Function name:             Sleep_SaveData
** Descriptions:              ˯�ߴ洢����
** input parameters:          ��
** output parameters:         ��
** Returned value:            ��
** Created by:                WJ
** Created Date:              2016-11-29
****************************************************************/ 
static void Sleep_SaveData(const vph_sleep_info_t* p_sleep_info)
{
    uint8_t curr_month = 0;
    uint8_t curr_day = 0;
    uint8_t yester_day = 0;
    flash_find_index_t*  p_find_index = &p_flash_param->flash_index_s;
    vph_sleep_quality_info_t vsis_sleep_quality_info;
    
    memset(&vsis_sleep_quality_info, 0, sizeof(vph_sleep_quality_info_t));
    curr_day = clock->day;
    curr_month = clock->month; 

    vsis_sleep_quality_info.head = 0xA1;        //���ݱ�־
    /*---------------------------- ��������ʱ��  -------------------------*/
    vsis_sleep_quality_info.sleep_out_time[0] = curr_month;//clock->month;        //��
    vsis_sleep_quality_info.sleep_out_time[1] = p_sleep_info->quit_time.date;
    vsis_sleep_quality_info.sleep_out_time[2] = p_sleep_info->quit_time.hour;
    vsis_sleep_quality_info.sleep_out_time[3] = p_sleep_info->quit_time.minute;
    
    //��˯ʱ�䴦��
    if(p_sleep_info->quit_time.date != p_sleep_info->enter_time.date)//ʮ����֮����˯
    {
        //��˯ʱ����ǰһ��
        //WJ.20180104.˯����1�·ݣ���˯���˳�˯�߲���ͬһ���bug
        //if(vsis_sleep_quality_info.sleep_out_time[0] == 1)
        if(p_sleep_info->quit_time.date == 1 && vsis_sleep_quality_info.sleep_out_time[0] == 1)//�����·���һ��
        {
            vsis_sleep_quality_info.sleep_enter_time[0] = 12;//��˯�·�12��(���ѵ�ǰһ�����·�)
        }
        else
        {
            Sclock_GetPreviousDayDate(1, &curr_month, &yester_day);
            vsis_sleep_quality_info.sleep_enter_time[0] = curr_month;//clock->month;   //WJ.20170303.������˯�·ݴ����BUG
        }    //����ʱ�䵱ǰ�·� 
    }
    else
    {
        if(p_sleep_info->quit_time.date != curr_day)
        {
            Sclock_GetPreviousDayDate(1, &curr_month, &yester_day);
//            vsis_sleep_quality_info.sleep_enter_time[0] = curr_month;   //WJ.20170619.�����˳�˯���·ݴ����BUG            
        }
//        else
//        {
        vsis_sleep_quality_info.sleep_enter_time[0] = curr_month;//clock->month;//��˯ʱ�䵱ǰ�·�(����ʱ����·�)
//        }
    }
    
    /*-------------- ������˯ʱ  --------------*/
    vsis_sleep_quality_info.sleep_enter_time[1] = p_sleep_info->enter_time.date;
    vsis_sleep_quality_info.sleep_enter_time[2] = p_sleep_info->enter_time.hour;
    vsis_sleep_quality_info.sleep_enter_time[3] = p_sleep_info->enter_time.minute;
    /*---------------------------- ����˯��ʱ�估����  -------------------------*/
    vsis_sleep_quality_info.sleep_quality[0] = p_sleep_info->deep_time;                //��˯ֵ
    vsis_sleep_quality_info.sleep_quality[1] = p_sleep_info->shallow_time;            //ǳ˯ֵ
    vsis_sleep_quality_info.sleep_quality[2] = p_sleep_info->quanlity_val;            //˯������
    vsis_sleep_quality_info.total_time = p_sleep_info->deep_time + p_sleep_info->shallow_time;        //��˯��ʱ�䳤��
    /*------------------------------- ����˯������ -----------------------------*/
    memcpy(vsis_sleep_quality_info.sleep_shallow, p_sleep_info->deep_shallow, 31);        //����˯�����ߣ� ����Ϊϴ��֧�ֳ���31��byte���ɴ洢20��Сʱ����
    
    /*------------------------------- ������;����ʱ�� -------------------------------*/
    vsis_sleep_quality_info.start_time[0] = 209;
    vsis_sleep_quality_info.end_time[0] = 210;
    memcpy(&vsis_sleep_quality_info.start_time[1], p_sleep_info->start_time, 11);
    memcpy(&vsis_sleep_quality_info.end_time[1], p_sleep_info->end_time, 11);    

    // ע��:����8���Ժ���˯���ڵ����
    if((p_sleep_info->enter_time.hour >= 8)&&(p_sleep_info->enter_time.date == clock->day))
    {
        Api_SaveSleepResultData(p_find_index->block_index,(uint8_t*)&vsis_sleep_quality_info);
    }
    else
    {
        // �������ǰһ��
        Api_SaveSleepResultData((p_find_index->block_index+2)%3,(uint8_t*)&vsis_sleep_quality_info);
    }
}

/****************************************************************
** Function name:             Sleep_LowBateeryProcess
** Descriptions:               
** input parameters:          ��
** output parameters:         ��
** Returned value:            ��
** Created by:                WJ
** Created Date:              2016-11-29
****************************************************************/ 
void Sleep_LowBateeryProcess(void)
{
    vph_sleep_info_t  m_sleep_info = {0};
    const vph_sleep_info_t* p_sleep_info = Vs_GetSleepingInfo();
    uint8_t daily_id = p_flash_param->flash_index_s.block_index%3;
    uint16_t current_package = p_flash_param->flash_index_s.daily.offset[daily_id];
        
//    if(p_sleep_info->sleep_flag  != 0x01) 
//    {
//        Vs_ClearInfoMsg();      //���˯�߻�������
//        return;
//    }
    
    //����˯�߼��㣬������ش���
    Api_SetSleepBlack(daily_id);
    Vph_SleepLowBattery(current_package);
    if(p_sleep_info->sleep_flag  == 3) //�˳�˯��       
    {
        memcpy(&m_sleep_info,p_sleep_info,sizeof(vph_sleep_info_t));
        
        // �����˳�ʱ��
        m_sleep_info.quit_time.date = clock->day;
        m_sleep_info.quit_time.hour = clock->hour;
        m_sleep_info.quit_time.minute = clock->minutes;
        
        Sleep_SaveData(&m_sleep_info);
        Vs_ClearInfoMsg();        
    } 
}

/****************************************************************
** Function name:             Sleep_NormalProcess
** Descriptions:              ��������µ�˯�ߴ��� 
** input parameters:          ��
** output parameters:         ��
** Returned value:            ��
** Created by:                WJ
** Created Date:              2016-11-29
****************************************************************/ 
void Sleep_NormalProcess(void)
{
    const vph_sleep_info_t* p_sleep_info = Vs_GetSleepingInfo();//�õ�˯������
    uint8_t daily_id = p_flash_param->flash_index_s.block_index%3;//�����
    uint16_t current_package = p_flash_param->flash_index_s.daily.offset[daily_id];//���������ƫ����
    
    //����˯�߼��㣬������ش���
    Api_SetSleepBlack(daily_id);
    p_data_manage->daily_attr.sleep_err = Vph_SleepDetectEntery(current_package);//�ճ�����
    if(p_sleep_info->sleep_flag  == 3)//�Ѿ��˳�˯�߽��д洢       
    {
        Sleep_SaveData(Vs_GetSleepingInfo());//
        Vs_ClearInfoMsg();        
    }     
}

/****************************************************************
** Function name:             Sleep_TraversingOperation
** Descriptions:              ˯�߱������� 
** input parameters:          ��
** output parameters:         ��
** Returned value:            ��
** Created by:                GT
** Created Date:              2017-10-11
****************************************************************/ 
void Sleep_TraversingOperation(void)
{
    const vph_sleep_info_t* p_sleep_info = Vs_GetSleepingInfo();//
    uint8_t daily_id = p_flash_param->flash_index_s.block_index%3;
    uint16_t current_package = p_flash_param->flash_index_s.daily.offset[daily_id];
    
    uint32_t daily_data[3] = {0};
    uint8_t* p_daily_data = (uint8_t*)daily_data;
    
    uint8_t traver_sleep_block = 0;
    uint16_t daily_pkg = 0;
    uint16_t daily_max_pkg = 0;
    uint8_t y_month = 0;
    uint8_t y_day = 0; 
    
    traver_sleep_block = (daily_id + 2)%3;  //�õ�����Ŀ��
    daily_max_pkg = p_flash_param->flash_index_s.daily.offset[traver_sleep_block];
    Sclock_GetPreviousDayDate(1, &y_month, &y_day); //��ȡ������·ݺ�����
    
    if(daily_max_pkg != 0)
    {
        //��ȡ����Ŀ��������
        Flash_CommonRead(daily_data, RECORD_DAILY_DATA_ADDR(traver_sleep_block,0), 9);
    }
    
    if(daily_max_pkg == 0 || p_daily_data[0] != y_month || p_daily_data[1] != y_day)
    {
        //���첻�������ݣ�˯�߱����ӽ�������ݿ�ʼ
        daily_pkg = 0;
        traver_sleep_block = daily_id;
        daily_max_pkg = p_flash_param->flash_index_s.daily.offset[traver_sleep_block];
        Api_SetSleepBlack(traver_sleep_block);
    }
    else
    {
        if(daily_max_pkg >= 48)
        {
            //48 = 12 * 4,�������������8�㿪ʼ����
            daily_pkg = daily_max_pkg - 48 + 1;
        }
        else
        {
            daily_pkg = 1;
        }
        Api_SetSleepBlack(traver_sleep_block);
    }
    
    // 1.���˯�߻�������
    Vs_ClearInfoMsg();  
    
    do{
        // 2.����˯�߼��㣬��������(ǰһ��20:00---��ǰʱ��)
        for(; daily_pkg <= daily_max_pkg; daily_pkg++)
        {
            Vph_SleepDetectEntery(daily_pkg);
            if(p_sleep_info->sleep_flag  == 3)        
            {
                Sleep_SaveData(Vs_GetSleepingInfo());
                Vs_ClearInfoMsg();        
            } 
        }
        if(traver_sleep_block != daily_id)
        {
            traver_sleep_block = daily_id;
            Api_SetSleepBlack(traver_sleep_block);
            daily_max_pkg = current_package;
            daily_pkg = 1;
        }
        else
        {
            break;
        }
    }while(1);
    
    Api_SetSleepBlack(daily_id);
}

void Sleep_DebugOverloadData(void)
{
//    //1.������������
//    Api_FlashFatoryRestore(); 
//    
//    //2.д����������
//    Sleep_OverloadVirtualData();

//    //3.����
//    Sleep_TraversingOperation();
};

/****************************************************************
** Function name:             Sleep_Init
** Descriptions:              ˯�߳�ʼ�� 
** input parameters:          ��
** output parameters:         ��
** Returned value:            ��
** Created by:                WJ
** Created Date:              2016-11-29
****************************************************************/ 
void Sleep_Init(void)
{    
    Vph_SleepingInit(Api_SleepCallback);
    Vs_ClearInfoMsg();   
}
