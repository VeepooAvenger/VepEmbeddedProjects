/***************************Copyright(c)***********************************
**                       Veepoo Tech
**                  COPYRIGHT (C) 2016-2019
**-----------File Info-----------------------------------------------------
** File        name:     movement_patterns.c
** Last modified Date:   2017-08-04
** Last SYS_VERSION:     0.0.0
** Descriptions:            
**-------------------------------------------------------------------------
** Created  by:          WJ
** Created  date:        2017-08-04
** SYS_VERSION:          0.0.0
** Descriptions:         
**-------------------------------------------------------------------------
** Modified by:				
** Modified date:  			          
** SYS_VERSION:                 
** Descriptions:                        
***************************************************************************/
#include <stdint.h>
#include <string.h>
#include "movement_patterns.h"
#include "app_flash.h"
#include "bracelet_protocol.h"
#include "system_clock.h"
#include "app_crc16.h"
#include "pedometer.h"
#include "app_type.h"
#include "app_task.h"
#include "heart.h"
#include "app_handler.h"
#include "util.h"
/*********************************************************************
* Macro Definition
*/
#define MOVEMENT_RUN_MAX_TIME       18000       //�˶�ģʽ�����ʱ��5Сʱ
#define MOVEMENT_HEAD_MAX_PACK_NUM  3           //ͷ����Ϣ���͵�������
#define MOVEMENT_HEAD_CRC_LEN       (sizeof(movement_patterns_head_t) - 10)
    
#define MOVEMENT_RECORD_MAX_PACK_NUM    301            //��¼��Ϣ�����������
    
#define MOVEMENT_MIN_VALID_TIME         60         //�˶���С��Ч����ʱ�䣨s��

#define MOVEMENT_PUSE_TIMEOUT_MIN_TIME      3       //3����
#define MOVEMENT_PUSE_TIMEOUT_MAX_TIME      120     //120����
#define MOVEMENT_PUSE_TIMES_MAX_CNT         200     //�����ͣ����

/*********************************************************************
* Global Variables
*/  
static movement_patterns_info_t movement_info_s = {0x00,};              //�˶���Ϣ
__ALIGN(4) static movement_patterns_head_t movement_head_s = {0x00,};   //�˶�����ͷ����Ϣ
__ALIGN(4) static movement_patterns_data_t movement_data_s = {0x00,};   //��ǰһ���Ӽ�¼��Ϣ
static movement_send_info_t     movement_send_s = {0x00,};              //�˶�ģʽ���ݷ�����Ϣ
static uint16_t *movement_max_puse_time = NULL;                         //�˶�ģʽ��ͣ��ʱʱ��
static uint8_t movement_heart_buf[80] = {0x00,};                        //�洢һ��������
static uint8_t movement_heart_cnt = 0;                                  //��¼һ�������ʸ���

static uint8_t movement_notify_flag = 0;                                //�˶�����ϱ�app

static uint16_t movement_10minutes_step[10] = {0x00, };                  //10���ӼƲ�
static uint32_t movement_10minutes_move[10] = {0x00, };                  //10�����˶���

static uint32_t movement_10minutes_step_all = 0;
static uint32_t movement_10minutes_move_all = 0;

extern system_flag_t system_flag_s;
extern system_info_t system_info_s;
/*********************************************************************
* local functions
*/
static void Movement_SendCompleteNotify(void);
static void Movement_ExitCriteriaInit(void);
static uint8_t Movement_ExitCriteriaJudgment(void);

/****************************************************************
** Function name:           Movement_OpenHeart
** Descriptions:            �˶�ģʽ������
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-08-04
*****************************************************************/ 
static void Movement_OpenHeart(void)
{ 
    App_StopMeasuringHandler(TEST_CLOSE_ALL);
    system_flag_s.movement_patterns = 1;
    system_flag_s.heart_led_status = true;
    system_flag_s.measuring_status = BUTTON_HEARTING_STATUS;                
    Heart_Open();   
}

/****************************************************************
** Function name:           Movement_CloseHeart
** Descriptions:            �˶�ģʽ�ر�����
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-08-04
*****************************************************************/ 
static void Movement_CloseHeart(void)
{
    App_StopMeasuringHandler(TEST_CLOSE_BTN);
    system_flag_s.movement_patterns = 0;    
}

/****************************************************************
** Function name:           Movement_RestInit
** Descriptions:            �˶�ģʽ�豸��λ��ʼ��
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-08-04
*****************************************************************/ 
void Movement_RestInit(void)
{
    uint8_t i = 0;
    uint8_t time_buf[7] = {0x00, };
    uint8_t time_buf1[7] = {0x00, };
    uint8_t temp_data= 0;
    uint8_t temp_block = 0;
    movement_patterns_head_t movement_head_temp_s = {0x00,};
   
    for( i = 0; i < MOVEMENT_PATTERNS_MAX_BLOCK; i++)
    {
        App_FlashReadMovementHead(i, (void *)&movement_head_temp_s, sizeof(movement_patterns_head_t));       
     
        if(movement_head_temp_s.flag != 0x00 || movement_head_temp_s.crc == 0x0000 || movement_head_temp_s.crc == 0xFFFF)
        {
            temp_block = i;
            break;
        } 
        if(i == 0)
        {            
            memcpy(time_buf, &movement_head_temp_s.start_year, 7);
            temp_data = time_buf[0];
            time_buf[0] = time_buf[1];
            time_buf[1] = temp_data;
        }
        else
        {
            memcpy(time_buf1, &movement_head_temp_s.start_year, 7);
            temp_data = time_buf[0];
            time_buf[0] = time_buf[1];
            time_buf[1] = temp_data;   
            
            if(memcmp(time_buf, time_buf1, 7) > 0)
            {                
                temp_block = i;
            }       
        }
    }    
    movement_info_s.block = temp_block;      
    //Movement_Init();
}

/****************************************************************
** Function name:           Movement_Init
** Descriptions:            �˶�ģʽ��ʼ��
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-08-04
*****************************************************************/ 
void Movement_Init(void)
{
    uint8_t block_temp = 0;
    uint8_t flash_block_usable = 0;
    
    block_temp = movement_info_s.block;
    memset(&movement_info_s, 0x00, sizeof(movement_patterns_info_t));
    //WJ.20170913.�޸������С�������ȫ���±�������һ�εĲ��ֽ��
    memset(&movement_head_s, 0x00, sizeof(movement_patterns_head_t));  
    memset(&movement_data_s, 0x00, sizeof(movement_patterns_data_t));     
    movement_info_s.block = block_temp;
    
    movement_10minutes_step_all = 0;
    movement_10minutes_move_all = 0;
    memset(&movement_10minutes_step, 0x00, sizeof(movement_10minutes_step)) ;     //10���ӼƲ�                       
    memset(&movement_10minutes_move, 0x00, sizeof(movement_10minutes_step)) ;     //10�����˶���
  
    system_flag_s.movement_patterns = 0;
    movement_max_puse_time = &system_info_s.movement_max_puse_time;
    
    flash_block_usable = App_FlashCheckAreaIsUsable(MOVEMENT_PATTERNS_DATA_BLOCK_START_PAGE + movement_info_s.block * MOVEMENT_PATTERNS_DATA_BLOCK_UINT, 
                                                    MOVEMENT_PATTERNS_DATA_BLOCK_UINT);   
    if(flash_block_usable != 1)
    {
        App_FlashEraseMovementDataPage(movement_info_s.block);
    }      
}

/****************************************************************
** Function name:           Movement_Start
** Descriptions:            �˶�ģʽ��ʼ
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-08-04
*****************************************************************/ 
void Movement_Start(void)
{       
    sport_information_info_t gsii_sport_info_s = {0x00, };
    movement_patterns_head_t movement_head_temp_s = {0x00,};
    
    if(movement_head_s.pause_times > MOVEMENT_PUSE_TIMES_MAX_CNT)
    {        
        Movement_Finish();
        return ;
    } 
    
    Get_SportInformationInfo(&gsii_sport_info_s);
    if(movement_info_s.run_state == MOVEMENT_STATE_IDEL)
    {
        /* ��һ�ο�ʼ�����˶�ģʽ*/
        Movement_Init();
        movement_info_s.run_time = 0;
        movement_info_s.current_pause_time = 0;
        
//        //������ͣ�ļƲ������롢��·��˶���
//        {                
//            //�˶���
//            movement_info_s.last_movement = Work_MeasureGet();         
//            //�Ʋ�          
//            movement_info_s.last_step = gsii_sport_info_s.posi_steps; 
//            //��·�С����
//            movement_info_s.last_calories = gsii_sport_info_s.posi_cal;  
//            //���루�ף�
//            movement_info_s.last_distance = gsii_sport_info_s.posi_distance;  
//        } 
            

        App_FlashReadMovementHead(movement_info_s.block, (void *)&movement_head_temp_s, sizeof(movement_patterns_head_t));       
     
        if(movement_head_temp_s.flag != 0xFF)
        {
            Movement_EreaFlashByBlock(movement_info_s.block);
        }
        
        //����ͷ����Ϣ             
        movement_head_s.flag = DAILY_HEAD_FLAG_USING;
        movement_head_s.start_year = clock->year;           //��ʼ��
        movement_head_s.start_month = clock->month;         //��ʼ��
        movement_head_s.start_day = clock->day;             //��ʼ��
        movement_head_s.start_hour = clock->hour;           //��ʼСʱ
        movement_head_s.start_minute = clock->minutes;      //��ʼ����
        movement_head_s.start_second = clock->seconds;      //��ʼ��                   
    }
    
    if(movement_info_s.run_time >= MOVEMENT_RUN_MAX_TIME)
    {
        return ;
    }
    
    movement_info_s.run_state = MOVEMENT_STATE_RUN;
    movement_head_s.all_pause_time += movement_info_s.current_pause_time;
    movement_info_s.current_pause_time = 0;
    
    //�������ļƲ������롢��·��˶���
    movement_info_s.last_step = gsii_sport_info_s.posi_steps;
    movement_info_s.last_movement = Work_MeasureGet();  
    movement_info_s.last_distance = gsii_sport_info_s.posi_distance;
    movement_info_s.last_calories = gsii_sport_info_s.posi_cal;      
    //��Ҫ�������ʼ�⣬��ʱ��λ����������
    Movement_OpenHeart();
    
    Movement_ExitCriteriaInit();
}

/****************************************************************
** Function name:           Movement_Pause
** Descriptions:            �˶�ģʽ��ͣ
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-08-04
*****************************************************************/ 
void Movement_Pause(void)
{
    if(movement_info_s.run_state != MOVEMENT_STATE_RUN)
    {
        return;
    }
    sport_information_info_t gsii_sport_info_s = {0x00, };
    
    movement_info_s.current_pause_time = 0;
    movement_info_s.run_state = MOVEMENT_STATE_PAUSE;
    movement_info_s.pause_flag = 1;
    
    movement_head_s.pause_times++; 
    
    Get_SportInformationInfo(&gsii_sport_info_s);
    //������ͣ�ļƲ������롢��·��˶���
    {                
        //�˶���
        movement_info_s.pause_movement += Work_MeasureGet() - movement_info_s.last_movement;         
        //�Ʋ�          
        movement_info_s.pause_step += gsii_sport_info_s.posi_steps - movement_info_s.last_step; 
        //��·�С����
        movement_info_s.pause_calories += gsii_sport_info_s.posi_cal - movement_info_s.last_calories;  
        //���루�ף�
        movement_info_s.pause_distance += gsii_sport_info_s.posi_distance - movement_info_s.last_distance;  
    }    
       
    //��Ҫ�ر����ʼ��   
    Movement_CloseHeart();
    
    //ʵʱ����
    movement_info_s.ui_heart = 0;                  
}

/****************************************************************
** Function name:           Movement_SaveHeadUsingInfo
** Descriptions:            ���õ�ǰ������ʹ��
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-08-04
*****************************************************************/ 
void Movement_SaveHeadUsingInfo(void)
{
    __ALIGN(4) movement_patterns_head_t movement_head_temp_s = {0x00,};

    memset(&movement_head_temp_s, 0xFF, sizeof(movement_patterns_head_t));  
    movement_head_temp_s.flag = DAILY_HEAD_FLAG_USING;   

    App_FlashWriteMovementHead(movement_info_s.block, (uint8_t *)&movement_head_temp_s, sizeof(movement_patterns_head_t));
}

/****************************************************************
** Function name:           Movement_Finish
** Descriptions:            �˶�ģʽ�������ݱ���
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-08-04
*****************************************************************/ 
void Movement_Finish(void)
{
    if(movement_info_s.run_state == MOVEMENT_STATE_IDEL 
      || movement_info_s.run_state == MOVEMENT_STATE_FINISH)
    {
        return;
    }
    //�ر����ʲ����������˶��������
    //��Ҫ�ر����ʼ�� 
    Movement_CloseHeart();
    
    if(movement_info_s.run_vail_time < MOVEMENT_MIN_VALID_TIME)
    {
        //�˶���Чʱ��С��60s������ͷ����Ϣ����¼��Ч
        return;
    }
    
    //�������һ������(1����)
    Movement_SaveMovementData();
    
    //ȷ��������ͣʱ��    
    movement_head_s.all_pause_time += movement_info_s.current_pause_time;
    movement_info_s.current_pause_time = 0;
    
    //����ͷ����Ϣ
    movement_head_s.flag = 0;                         //���ʹ�ñ�־ 
    movement_head_s.end_year = clock->year;           //������
    movement_head_s.end_month = clock->month;         //������
    movement_head_s.end_day = clock->day;             //������
    movement_head_s.end_hour = clock->hour;           //����Сʱ
    movement_head_s.end_minute = clock->minutes;      //��������
    movement_head_s.end_second = clock->seconds;      //������         
    
    movement_head_s.all_calories = movement_info_s.calories;
    movement_head_s.all_distance = movement_info_s.distance;
    movement_head_s.all_movement = movement_info_s.movement;
    movement_head_s.all_step = movement_info_s.step;
    
    movement_head_s.record_cnt = movement_info_s.pack_cnt;
     
    movement_head_s.crc = App_Crc16((uint8_t *)&movement_head_s.flag + 1, MOVEMENT_HEAD_CRC_LEN);   //CRCУ�� 
    memset((uint8_t *)movement_head_s.retain, 0x00, 8);         //����,����
    Movement_SaveMovementHead();
    
    movement_info_s.block++;
    if(movement_info_s.block >= MOVEMENT_PATTERNS_MAX_BLOCK)
    {
        movement_info_s.block = 0;
        movement_info_s.pack_cnt = 0;
    }       
    movement_info_s.run_state = MOVEMENT_STATE_FINISH; 
    
}

/****************************************************************
** Function name:           Movement_Exit
** Descriptions:            �˶�ģʽ�˳�
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-08-04
*****************************************************************/ 
void Movement_Exit(void)
{
    //WJ.20170906.��ֹ��ιر����ʲ���������ϱ��˶����ݸ��� 
    if(movement_info_s.run_state == MOVEMENT_STATE_IDEL)
    {
        return;
    }
    if(movement_info_s.run_state != MOVEMENT_STATE_IDEL 
      && movement_info_s.run_state != MOVEMENT_STATE_FINISH)
    {
        Movement_Finish();
    }
    
    movement_info_s.run_state = MOVEMENT_STATE_IDEL; 
    Movement_CloseHeart();
    
    movement_notify_flag = 1;
}

/****************************************************************
** Function name:           Movement_RestoreFactoryExit
** Descriptions:            ���ڻָ���������
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-08-04
*****************************************************************/ 
void Movement_RestoreFactoryExit(void)
{
    if(movement_info_s.run_state != MOVEMENT_STATE_IDEL)
    {
        memset(&movement_info_s, 0x00, sizeof(movement_patterns_info_t));
        memset(&movement_head_s, 0x00, sizeof(movement_patterns_head_t));  
        memset(&movement_data_s, 0x00, sizeof(movement_patterns_data_t)); 
    }
}

/****************************************************************
** Function name:           Movement_IsBusy
** Descriptions:            �˶�ģʽ��
** input parameters:        ��
** output parameters:       ��
** Returned value:          �����˶�ģʽ״̬
** Created by:              WJ              
** Created Date:            2017-08-04
*****************************************************************/ 
uint8_t Movement_GetState(void)
{
    return movement_info_s.run_state;
}

/****************************************************************
** Function name:           Movement_IsBusy
** Descriptions:            �˶�ģʽ��
** input parameters:        ��
** output parameters:       ��
** Returned value:          ����0�����У�1���˶�ģʽ��
** Created by:              WJ              
** Created Date:            2017-08-04
*****************************************************************/ 
uint8_t Movement_IsBusy(void)
{
    if(movement_info_s.run_state != MOVEMENT_STATE_IDEL)
    {
        return 1;
    }
    return 0;
}


/****************************************************************
** Function name:           Movement_UpdataData
** Descriptions:            �˶�������������1s�¼���
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-08-04
*****************************************************************/
static void Movement_UpdataData(void)
{    
    sport_information_info_t gsii_sport_info_s = {0x00, };
    
    Get_SportInformationInfo(&gsii_sport_info_s);
    if(movement_info_s.run_vail_time < MOVEMENT_RUN_MAX_TIME)
    {
        movement_info_s.run_vail_time++;
    }
    else
    {                
        Movement_Exit();
        return;
    }     
    
    movement_info_s.ui_heart = Heart_Test_Value_Get();                  //ʵʱ����
    movement_info_s.ui_distance = movement_info_s.distance + gsii_sport_info_s.posi_distance 
                                  - movement_info_s.last_distance + movement_info_s.pause_distance; 
    
    movement_info_s.ui_calories = movement_info_s.calories + gsii_sport_info_s.posi_cal 
                                  - movement_info_s.last_calories + movement_info_s.pause_calories;
    
    movement_info_s.ui_step = movement_info_s.step + gsii_sport_info_s.posi_steps 
                              - movement_info_s.last_step + movement_info_s.pause_step;   
    
    if(movement_info_s.ui_heart > 30 && movement_info_s.ui_heart < 200)
    {
        if(movement_heart_cnt >= 60)
        {
            return;
        }
        movement_heart_buf[movement_heart_cnt++] = movement_info_s.ui_heart;
    }
}

/****************************************************************
** Function name:           Movement_Monitor
** Descriptions:            �˶�������������1s�¼���
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-08-04
*****************************************************************/
void Movement_Monitor(void)
{   
    uint8_t ret = 0;
    
    if(movement_info_s.run_state == MOVEMENT_STATE_IDEL 
      || movement_info_s.run_state == MOVEMENT_STATE_FINISH)
    {
        if(movement_notify_flag == 1)
        {
            movement_notify_flag = 0;
            Movement_SendCompleteNotify();
        }
        return;
    }
    else
    {   
        if(movement_info_s.run_state == MOVEMENT_STATE_RUN)
        {
            Movement_UpdataData();
            //WJ.20170830.λ�ô���
            movement_info_s.run_start_time++;
            ret = Movement_ExitCriteriaJudgment();
            if(ret == 11 || ret == 12)
            {              
                Movement_Exit();
                return;       
            }        
        }        
        
        if(movement_info_s.run_time < MOVEMENT_RUN_MAX_TIME && movement_info_s.pack_cnt < MOVEMENT_RECORD_MAX_PACK_NUM)
        {
            movement_info_s.run_time++;
        }
        else
        {                
            Movement_Exit();
            return;
        }        
        if(movement_info_s.run_state == MOVEMENT_STATE_PAUSE)
        {
            movement_info_s.current_pause_time++;
            //movement_max_puse_time ʹ�õķ��ӻ���
            if(movement_info_s.current_pause_time >= (*movement_max_puse_time * 60))
            {              
                Movement_Exit();
                return;
            }
        }
        if(movement_info_s.run_time % 60 == 0 && movement_info_s.run_time != 0)
        {
            //��������
            Movement_SaveMovementData();
        }
    }    
}

/****************************************************************
** Function name:           Movement_EreaFlashByBlock
** Descriptions:            ���ݿ�Ų����˶�����
** input parameters:        ��
** output parameters:       ��
** Returned value:          ����0������ʧ�ܣ�1�������ɹ�
** Created by:              WJ              
** Created Date:            2017-08-04
*****************************************************************/ 
uint8_t Movement_EreaFlashByBlock(uint8_t block)
{
    uint8_t error_code = APP_FLASH_SUCCESS;
    
    if(block >= MOVEMENT_PATTERNS_MAX_BLOCK)
    {
        return 0;
    }
    error_code = App_FlashEraseMovementDataPage(block);
    
    if(error_code == APP_FLASH_FAILED)
    {
        return 0;
    }    
    return 1;
}


/****************************************************************
** Function name:           Movement_EreaFlashAllBlock
** Descriptions:            ���������˶�ģʽ����
** input parameters:        ��
** output parameters:       ��
** Returned value:          ����0������ʧ�ܣ�1�������ɹ�
** Created by:              WJ              
** Created Date:            2017-08-04
*****************************************************************/ 
uint8_t Movement_EreaFlashAllBlock(void)
{
    uint8_t error_code = APP_FLASH_SUCCESS;
    
    error_code = App_FlashEraseMovementDataAllPage();
    if(error_code == APP_FLASH_FAILED)
    {
        return 0;
    }        
    return 1;
}

/****************************************************************
** Function name:           Movement_SaveMovementData
** Descriptions:            ����һ�����˶�����
** input parameters:        ��
** output parameters:       ��
** Returned value:          ����0������ʧ�ܣ�1������ɹ�
** Created by:              WJ              
** Created Date:            2017-08-04
*****************************************************************/ 
uint8_t Movement_SaveMovementData(void)
{    
    uint8_t average_heart = 0;
    sport_information_info_t gsii_sport_info_s = {0x00, };
    
    Get_SportInformationInfo(&gsii_sport_info_s);
    
    average_heart = Util_AverageByUint8(movement_heart_buf, movement_heart_cnt);
    movement_heart_cnt = 0;
    
    memset(&movement_data_s, 0x00, sizeof(movement_patterns_data_t));  
        
    movement_data_s.year = clock->year;
    movement_data_s.month = clock->month;
    movement_data_s.day = clock->day;
    movement_data_s.hour = clock->hour;    
    movement_data_s.minute = clock->minutes;
    
    if(movement_info_s.run_state == MOVEMENT_STATE_RUN)
    {
        //����
        movement_data_s.heart = average_heart;          
        //�˶���
        movement_data_s.movement = (Work_MeasureGet() - movement_info_s.last_movement + movement_info_s.pause_movement) >> 12;   
        movement_info_s.pause_movement = 0;
        movement_info_s.last_movement = Work_MeasureGet();          
        //�Ʋ� 
        movement_data_s.step = gsii_sport_info_s.posi_steps - movement_info_s.last_step + movement_info_s.pause_step;
        movement_info_s.pause_step = 0;
        movement_info_s.last_step = gsii_sport_info_s.posi_steps;
        //��·�С����
        movement_data_s.calories = gsii_sport_info_s.posi_cal - movement_info_s.last_calories + movement_info_s.pause_calories; 
        movement_info_s.pause_calories = 0;        
        movement_info_s.last_calories = gsii_sport_info_s.posi_cal; 
        //���루�ף�
        movement_data_s.distance = gsii_sport_info_s.posi_distance - movement_info_s.last_distance + movement_info_s.pause_distance; 
        movement_info_s.pause_distance = 0;
        movement_info_s.last_distance = gsii_sport_info_s.posi_distance;
               
        //��ͣ��־
        movement_data_s.pause_flag = movement_info_s.pause_flag;            
    }
    else
    {
        //����
        movement_data_s.heart = average_heart;          
        //�˶���
        movement_data_s.movement = (movement_info_s.pause_movement) >> 12;   
        movement_info_s.pause_movement = 0;     
        //�Ʋ� 
        movement_data_s.step = movement_info_s.pause_step;
        movement_info_s.pause_step = 0;
        //��·�С����
        movement_data_s.calories = movement_info_s.pause_calories; 
        movement_info_s.pause_calories = 0;        
        //���루�ף�
        movement_data_s.distance = movement_info_s.pause_distance; 
        movement_info_s.pause_distance = 0;
        
        //��ͣ��־
        movement_data_s.pause_flag = movement_info_s.pause_flag;         
    }
    if(movement_info_s.run_state == MOVEMENT_STATE_RUN)
    {
        movement_info_s.pause_flag = 0;
    }

    movement_info_s.movement += movement_data_s.movement; 
    movement_info_s.step += movement_data_s.step;
    movement_info_s.calories += movement_data_s.calories;
    movement_info_s.distance += movement_data_s.distance; 
    
    App_FlashWriteMovementData(movement_info_s.block, movement_info_s.pack_cnt++, (uint8_t *)&movement_data_s, 
                               sizeof(movement_patterns_data_t), NULL);
    
    return 1;
}

/****************************************************************
** Function name:           Movement_SaveMovementHead
** Descriptions:            �����˶�����ͷ������
** input parameters:        ��
** output parameters:       ��
** Returned value:          ����0������ʧ�ܣ�1������ɹ�
** Created by:              WJ              
** Created Date:            2017-08-04
*****************************************************************/ 
uint8_t Movement_SaveMovementHead(void)
{        
    uint8_t error_code = APP_FLASH_SUCCESS;
    
    error_code = App_FlashWriteMovementHead(movement_info_s.block, (uint8_t *)&movement_head_s, 
                                           sizeof(movement_patterns_head_t));   
    if(error_code == APP_FLASH_FAILED)
    {
        return 0;
    }        
    return 1;
}

/****************************************************************
** Function name:           Movement_GetHeadCRC
** Descriptions:            ��ȡ�˶�ģʽͷ����ϢCRC
** input parameters:        crc_num��CRC�ĸ���
** output parameters:       crc:crcֵ��crc[0] = CRC0��8bit��crc[1] = CRC0��8bit
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-08-04
*****************************************************************/ 
void Movement_GetHeadCRC(uint8_t *crc, uint8_t crc_num)
{
    uint8_t i = 0;
    movement_patterns_head_t movement_head_temp_s = {0x00,};
    
    if(crc == NULL || crc_num == 0)
    {
       return; 
    }
    
    for(i = 0; i < crc_num; i++)
    {
        App_FlashReadMovementHead(i, (void *)&movement_head_temp_s, sizeof(movement_patterns_head_t));
        if(movement_head_temp_s.flag != 0x00 || movement_head_temp_s.crc == 0x00 || movement_head_temp_s.crc == 0xFFFF)
        {
            crc[i * 2] = 0x00;
            crc[i * 2 + 1] = 0x00;
        }
        else
        {
            //��λ��ǰ
            crc[i * 2] = movement_head_temp_s.crc & 0xFF;
            crc[i * 2 + 1] = (movement_head_temp_s.crc >> 8) & 0xFF;
        }    
    }   
}

/****************************************************************
** Function name:           Movement_SetSendDataInfo
** Descriptions:            ���÷����˶�ģʽ������Ϣ
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-08-04
*****************************************************************/ 
uint8_t Movement_SetSendDataInfo(uint8_t block)
{
    movement_patterns_head_t movement_head_temp_s = {0x00,};
    
    if(block >= MOVEMENT_PATTERNS_MAX_BLOCK)
    {
       return 0; 
    }

    App_FlashReadMovementHead(block, (void *)&movement_head_temp_s, sizeof(movement_patterns_head_t));       
 
    if(movement_head_temp_s.flag != 0x00 || movement_head_temp_s.crc == 0x00 || movement_head_temp_s.crc == 0xFFFF)
    {
        return 0;
    }
    
    movement_send_s.movement_block = block;
    movement_send_s.movement_num_start = 0;
    movement_send_s.movement_num_total = movement_head_temp_s.record_cnt + MOVEMENT_HEAD_MAX_PACK_NUM;

    return 1;
}

/****************************************************************
** Function name:           Movement_SendDataHandler
** Descriptions:            �����˶�����
** input parameters:        ��
** output parameters:       ��
** Returned value:          ����0�����ͽ�����1��������Ҫ����
** Created by:              WJ              
** Created Date:            2017-08-04
*****************************************************************/ 
uint8_t Movement_SendDataHandler(void)
{
    uint8_t movement_ble_ack[20] = {0xD4, 0x00, };
    uint8_t *movement_head_p = NULL;
    uint16_t c_pack_num = 0;
    movement_patterns_head_t movement_head_temp_s = {0x00,};
    movement_patterns_data_t movement_data_temp_s = {0x00,};         //��ǰһ���Ӽ�¼��Ϣ         
    
    if(movement_send_s.movement_block >= MOVEMENT_PATTERNS_MAX_BLOCK
      || movement_send_s.movement_num_start >= movement_send_s.movement_num_total)
    {        
        movement_ble_ack[5] = movement_send_s.movement_block;
        Ble_SendDataViaBat(movement_ble_ack, 20);  
        return 0; 
    }

    if(movement_send_s.movement_num_start < MOVEMENT_HEAD_MAX_PACK_NUM)
    {   
        //ͷ����Ϣ
        App_FlashReadMovementHead(movement_send_s.movement_block, (void *)&movement_head_temp_s, sizeof(movement_patterns_head_t));       
     
        if(movement_head_temp_s.flag != 0x00 || movement_head_temp_s.crc == 0x00 || movement_head_temp_s.crc == 0xFFFF)
        {
            return 0;
        }
        movement_send_s.movement_num_start++;
        movement_ble_ack[1] = movement_send_s.movement_num_start & 0xFF;
        movement_ble_ack[2] = (movement_send_s.movement_num_start >> 8) & 0xFF;
        movement_ble_ack[3] = movement_send_s.movement_num_total & 0xFF;
        movement_ble_ack[4] = (movement_send_s.movement_num_total >> 8) & 0xFF;
        movement_ble_ack[5] = movement_send_s.movement_block;
        
        movement_head_p = (uint8_t *)&movement_head_temp_s;
        movement_head_p += (movement_send_s.movement_num_start - 1) * 14;
        memcpy(&movement_ble_ack[6], movement_head_p, 14);    

        Ble_SendDataViaBat(movement_ble_ack, 20);          
    }
    else
    {
        //һ������Ϣ
        c_pack_num = movement_send_s.movement_num_start - MOVEMENT_HEAD_MAX_PACK_NUM;
        App_FlashReadMovementData(movement_send_s.movement_block, c_pack_num, (uint8_t *)&movement_data_temp_s);        
        movement_send_s.movement_num_start++;
        movement_ble_ack[1] = movement_send_s.movement_num_start & 0xFF;
        movement_ble_ack[2] = (movement_send_s.movement_num_start >> 8) & 0xFF;
        movement_ble_ack[3] = movement_send_s.movement_num_total & 0xFF;
        movement_ble_ack[4] = (movement_send_s.movement_num_total >> 8) & 0xFF;
        movement_ble_ack[5] = movement_send_s.movement_block;
        memcpy(&movement_ble_ack[6], &movement_data_temp_s.heart, 14);   

        Ble_SendDataViaBat(movement_ble_ack, 20);          
    }
    
    if(movement_send_s.movement_num_start >= movement_send_s.movement_num_total)
    {     
        return 0; 
    }    
    return 1;
}

/****************************************************************
** Function name:           Movement_IsVaildThePuseTimeOut
** Descriptions:            ��ͣ��ʱʱ����
** input parameters:        time:��ͣ��ʱʱ��
** output parameters:       ��
** Returned value:          ����0����Чֵ��1����Чֵ
** Created by:              WJ              
** Created Date:            2017-08-04
*****************************************************************/ 
uint8_t Movement_IsVaildThePuseTimeOut(uint16_t time)
{
    if(time < MOVEMENT_PUSE_TIMEOUT_MIN_TIME || time > MOVEMENT_PUSE_TIMEOUT_MAX_TIME) 
    {
        return 0;
    }
    return 1;
}

/****************************************************************
** Function name:           Movement_GetInfoData
** Descriptions:            ��ȡ��Ϣ������ַ
** input parameters:        ��
** output parameters:       ��
** Returned value:          ������Ϣ������ַ
** Created by:              WJ              
** Created Date:            2017-08-04
*****************************************************************/ 
movement_patterns_info_t *Movement_GetInfoData(void)
{ 
    return &movement_info_s;
}

/****************************************************************
** Function name:           Movement_IsMaxPauseCnt
** Descriptions:            ��ȡ�˶��Ƿ񵽴������ͣ�Ĵ���
** input parameters:        ��
** output parameters:       �� 
** Returned value:          ����0����,1����
** Created by:              WJ              
** Created Date:            2017-07-28
*****************************************************************/ 
uint8_t Movement_IsMaxPauseCnt(void)
{
    if(movement_head_s.pause_times >= MOVEMENT_PUSE_TIMES_MAX_CNT)
    {
        return 1;
    }
    return 0;
}

/****************************************************************
** Function name:           Movement_IsMaxPauseCnt
** Descriptions:            ��ȡ�˶��Ƿ񵽴������ͣ�Ĵ���
** input parameters:        ��
** output parameters:       �� 
** Returned value:          ����0����,1����
** Created by:              WJ              
** Created Date:            2017-07-28
*****************************************************************/ 
static void Movement_SendCompleteNotify(void)
{
    uint8_t movement_ble_ack[20] = {0xD3, 0x00,};

    movement_ble_ack[0] = 0xD3; 
    movement_ble_ack[1] = 0x02;     //��ʾ�����ϱ�        
    movement_ble_ack[2] = 0x01; 

    Movement_GetHeadCRC(&movement_ble_ack[3], 3);
           
    Ble_SendDataViaBat(movement_ble_ack, 20);    
}

/****************************************************************
** Function name:           Movement_ExitCriteriaInit
** Descriptions:            �˳�������ʼ��
** input parameters:        ��
** output parameters:       �� 
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-07-28
*****************************************************************/ 
static void Movement_ExitCriteriaInit(void)
{    
    sport_information_info_t gsii_sport_info_s = {0x00, };
    
    Get_SportInformationInfo(&gsii_sport_info_s);
    
    movement_info_s.run_start_time = 0;
    movement_10minutes_step_all = gsii_sport_info_s.posi_steps;
    movement_10minutes_move_all = Work_MeasureGet();
    memset(&movement_10minutes_step, 0x00, sizeof(movement_10minutes_step)) ;     //10���ӼƲ�                       
    memset(&movement_10minutes_move, 0x00, sizeof(movement_10minutes_step)) ;     //10�����˶���
}

/****************************************************************
** Function name:           Movement_ExitCriteriaJudgment
** Descriptions:            �ж��Ƿ��˳�
** input parameters:        ��
** output parameters:       �� 
** Returned value:          ����11��12���˳�,��������
** Created by:              WJ              
** Created Date:            2017-07-28
*****************************************************************/ 
static uint8_t Movement_ExitCriteriaJudgment(void)
{
    uint8_t i = 0;
    uint16_t time_cnt = 0;
    uint32_t all_step = 0;
    uint32_t all_move = 0;
    sport_information_info_t gsii_sport_info_s = {0x00, };
    
    Get_SportInformationInfo(&gsii_sport_info_s);   
    
    if(movement_info_s.run_start_time % 60 != 0 || movement_info_s.run_start_time == 0)
    {
        return 0;
    }
    time_cnt = movement_info_s.run_start_time / 60;
           
    for(i = 0; i < 9; i++)
    {
        movement_10minutes_step[i] = movement_10minutes_step[i + 1];
        movement_10minutes_move[i] = movement_10minutes_move[i + 1];
        if(time_cnt < 30 && i < 5)
        {
            continue;
        }
        all_step += movement_10minutes_step[i];
        all_move += movement_10minutes_move[i];
    }
    movement_10minutes_step[i] = gsii_sport_info_s.posi_steps - movement_10minutes_step_all;
    movement_10minutes_step_all = gsii_sport_info_s.posi_steps;
    
    movement_10minutes_move[i] = Work_MeasureGet() - movement_10minutes_move_all;
    movement_10minutes_move_all = Work_MeasureGet();

    all_step += movement_10minutes_step[i];
    all_move += movement_10minutes_move[i];   
    all_move = all_move >> 12;
       
    if((movement_head_s.pause_times == 0 && time_cnt <= 10) || time_cnt < 5)
    {
        return 1;
    }
    else if(movement_head_s.pause_times == 0)
    {
        time_cnt -= 10;
    }
        
    if(time_cnt < 30)
    {
        if(all_move >= 40)
        {
            return 2;
        }
        else if(all_step >= 20)
        {
            return 3;
        }
        else
        {
            return 11;
        }            
    }
    else
    {
        if(all_move >= 100)
        {
            return 4;
        }
        else if(all_step >= 50)
        {
            return 5;
        }
        else
        {
            return 12;
        }                
    }    
}

