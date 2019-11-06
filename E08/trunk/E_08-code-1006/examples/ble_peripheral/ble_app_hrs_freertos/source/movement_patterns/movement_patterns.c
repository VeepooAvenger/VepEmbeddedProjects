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
#include "bracelet_protocol.h"
#include "flash_data_type.h"
#include "data_manage.h"
#include "pedometer.h"
#include "SystemTime.h"
#include "crc16.h"
#include "api_flash.h"
#include "app_config_data.h"
#include "nrf_delay.h"
#include "ble_thread.h"
#include "Gui_Thread.h"
#include "util.h"
#include "test_state_machine.h"
#include "api_ext_flash.h"
#include "ext_flash.h"

/*********************************************************************
* Macro Definition
*/

/*********************************************************************
* Global Variables
*/  
movement_patterns_info_t movement_info_s = {0x00,};          //�˶���Ϣ
movement_patterns_head_t movement_head_s = {0x00,};   //�˶�����ͷ����Ϣ
movement_patterns_data_t movement_data_s = {0x00,};   //��ǰһ���Ӽ�¼��Ϣ

static uint16_t *movement_max_puse_time = NULL;                         //�˶�ģʽ��ͣ��ʱʱ��

static uint8_t movement_notify_flag = 0;                                //�˶�����ϱ�app

static uint16_t movement_10minutes_step[10] = {0x00, };                  //10���ӼƲ�
static uint32_t movement_10minutes_move[10] = {0x00, };                  //10�����˶���

static uint32_t movement_10minutes_step_all = 0;
static uint32_t movement_10minutes_move_all = 0;


movement_send_info_t     movement_send_s = {0x00,};          //�˶�ģʽ���ݷ�����Ϣ
uint8_t movement_heart_buf[80] = {0x00,};                        //�洢һ��������
uint8_t movement_heart_cnt = 0;                                  //��¼һ�������ʸ���

move_save_opt_t move_save_opt_s = {0};

MOVEMENT_SELECT_MODE_TYPE_E runmode_type = APP_MSM_MODE_TYPE_NULL;
uint8_t sport_mode_para = 0;


//ƽ������
uint32_t average_heart_totaltime = 0;
uint16_t average_heart_cnt_totaltime = 0;

//�㷨ģʽ��UIģʽ��ƥ�䣬��Ҫӳ��
const uint8_t algorithm_multi_runmode_map[APP_MSM_MODE_TYPE_MAX] = {
	MSM_MODE_NULL,
	MSM_MODE_RUN_OUTDOOR,
	MSM_MODE_WALK_OUTDOOR,
	MSM_MODE_RUN_INDOOR,
	MSM_MODE_WALK_INDOOR,
	MSM_MODE_HIKING,
	MSM_MODE_TREADMILLS_MACHINE,
	MSM_MODE_BIKING_OUTDOOR,
	MSM_MODE_BIKING_INDOOR,
	MSM_MODE_ELLIPTICAL_MACHINE,
	MSM_MODE_ROWING_MACHINE,	
};

/*********************************************************************
* local functions
*/
static void Movement_SendCompleteNotify(void);
static void Movement_ExitCriteriaInit(void);
static uint8_t Movement_ExitCriteriaJudgment(void);
static uint8_t Movement_ExitCriteriaJudgmentType2(void);

void Movement_FlashReadHead(uint8_t block_count, uint8_t *head_read_buf, uint16_t head_read_len)
{
    uint32_t read_time_out = 0;
    while(Extflash_MovementReadHeadSet(block_count, head_read_buf, head_read_len) == 0)
    {
        vTaskDelay(1);
        read_time_out++;
        if(read_time_out > 1200)
        {   //��ʱ����������
            return;
        }  
    }
    while(Extflash_MovementReadHeadIsComplete() == 0)
    {
        vTaskDelay(1);
        read_time_out++;
        if(read_time_out > 1200)
        {   //��ʱ����������
            return;
        }
    }
}

void Movement_FlashReadData(uint8_t block_count, uint16_t number, uint8_t *data, uint16_t read_len)
{
    uint32_t read_time_out = 0;
    while(Extflash_MovementReadDataSet(block_count, number, data, read_len) == 0)
    {
        vTaskDelay(1);
        read_time_out++;
        if(read_time_out > 1200)
        {   //��ʱ����������
            return;
        }  
    }
    while(Extflash_MovementReadDataIsComplete() == 0)
    {
        vTaskDelay(1);
        read_time_out++;
        if(read_time_out > 1200)
        {   //��ʱ����������
            return;
        }
    }
}

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
    Statemachine_AddState(MEAT_PPG_BTN_HEART);  
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
    Statemachine_DelState(MEAT_PPG_BTN_HEART); 
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
        
//        Movement_FlashReadHead(i, (void *)&movement_head_temp_s, sizeof(movement_patterns_head_t));
        Extflash_DataRead(RECORD_MOVEMENT_HEAD_ADDR(i,0), sizeof(movement_patterns_head_t), (uint8_t *)&movement_head_temp_s);
    
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
//    uint8_t flash_block_usable = 0;
    
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
    
    movement_max_puse_time = &p_flash_param->movement_max_puse_time;
    
    Movement_EreaFlashByBlock(movement_info_s.block);
    // ֱ�Ӳ���-���ȴ�����ͨ���ź���
    Extflash_MovementDeal(); 
	
	

	//���ƽ������
	average_heart_cnt_totaltime = 0;
	average_heart_totaltime = 0;  
	
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
    //movement_patterns_head_t movement_head_temp_s = {0x00,};
    
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

        /*
        Movement_FlashReadHead(movement_info_s.block, (void *)&movement_head_temp_s, sizeof(movement_patterns_head_t));       
     
        */
        //if(movement_head_temp_s.flag != 0xFF)
        {
            Movement_EreaFlashByBlock(movement_info_s.block);
        }
        
        //����ͷ����Ϣ             
        movement_head_s.flag = 0xAA;//DAILY_HEAD_FLAG_USING;
        movement_head_s.start_year = clock->year;           //��ʼ��
        movement_head_s.start_month = clock->month;         //��ʼ��
        movement_head_s.start_day = clock->day;             //��ʼ��
        movement_head_s.start_hour = clock->hour;           //��ʼСʱ
        movement_head_s.start_minute = clock->minutes;      //��ʼ����
        movement_head_s.start_second = clock->seconds;      //��ʼ��   
        
        movement_head_s.multi_runmode_type = runmode_type;  //������˶�ģʽ����
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
        runmode_type = APP_MSM_MODE_TYPE_NULL;
        return;
    }
    //�ر����ʲ����������˶��������
    //��Ҫ�ر����ʼ��
    Movement_CloseHeart();
    
    if(movement_info_s.run_vail_time < MOVEMENT_MIN_VALID_TIME)
    {
        //�˶���Чʱ��С��60s������ͷ����Ϣ����¼��Ч
        runmode_type = APP_MSM_MODE_TYPE_NULL;
        return;
    }
    
    //�������һ������(1����)
    Movement_SaveMovementData(0);
//    
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
     
    movement_head_s.crc = Crc16(0xFFFF, (uint8_t *)&movement_head_s.flag + 1, MOVEMENT_HEAD_CRC_LEN);   //CRCУ�� 
	movement_head_s.multi_runmode_type = runmode_type;  //������˶�ģʽ����
    memset((uint8_t *)movement_head_s.retain, 0x00, 7);         //����,����
//    nrf_delay_ms(100);
    Movement_SaveMovementHead();
    // ֱ�Ӳ���-���ȴ�����ͨ���ź���
    Extflash_MovementDeal(); 
    
    movement_info_s.block++;
    if(movement_info_s.block >= MOVEMENT_PATTERNS_MAX_BLOCK)
    {
        movement_info_s.block = 0;
        movement_info_s.pack_cnt = 0;
    }       
    movement_info_s.run_state = MOVEMENT_STATE_FINISH; 
    
    runmode_type = APP_MSM_MODE_TYPE_NULL;
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
//    if(movement_info_s.run_state != MOVEMENT_STATE_PAUSE)
//    {
    if(GuiThread_GetTaskType() == CPU_TASK_MOVEMENT_DATA)//CPU_TASK_MOVEMENT)
    {
        GuiThread_InMainTask();
    }
    GuiThread_ClearMovementParm();
    //WJ.20170906.��ֹ��ιر����ʲ���������ϱ��˶����ݸ��� 
    if(movement_info_s.run_state == MOVEMENT_STATE_IDEL)
    {
        runmode_type = APP_MSM_MODE_TYPE_NULL;
        return;
    }
    if(movement_info_s.run_state != MOVEMENT_STATE_IDEL 
      && movement_info_s.run_state != MOVEMENT_STATE_FINISH)
    {
        Movement_Finish();
    }
    else
    {    
        Movement_CloseHeart();
    }
    movement_info_s.run_state = MOVEMENT_STATE_IDEL; 
    movement_notify_flag = 1;
    
    runmode_type = APP_MSM_MODE_TYPE_NULL;
	
	//���ƽ������
	average_heart_cnt_totaltime = 0;
	average_heart_totaltime = 0;
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
    
    movement_info_s.ui_heart = p_data_manage->ecg_ppg_s.ppg_measure_s.p_second_heart;                  //ʵʱ����
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
	uint8_t state = 0;  //sport_mode_para [bit6:bit7]

	//�����˶�ģʽ������������㷨����
	if((runmode_type != APP_MSM_MODE_TYPE_NULL) && (movement_info_s.run_state != MOVEMENT_STATE_IDEL))
	{
		//sport_mode_para |= (runmode_type&0x3F);
        sport_mode_para = (algorithm_multi_runmode_map[runmode_type]&0x3F);
		if(movement_info_s.run_state == MOVEMENT_STATE_PAUSE)
		{
			state = 1;
		}
		sport_mode_para |= state << 6;
	}
	else
	{
		state = 0;  //��ղ���
		sport_mode_para = 0;
	}
    
	//����״̬��⣬��ɻ��߿���ʱֱ���˳�
    if(movement_info_s.run_state == MOVEMENT_STATE_IDEL 
      || movement_info_s.run_state == MOVEMENT_STATE_FINISH)
    {
        if(movement_notify_flag == 1)
        {
            Movement_SendCompleteNotify();
            movement_notify_flag = 0;
        }
        return;
    }
    else
    {   
		//�����£��ж����趨�˳������Ƿ�������������˳�
        if(movement_info_s.run_state == MOVEMENT_STATE_RUN)
        {
            Movement_UpdataData();
            //WJ.20170830.λ�ô���
			//����״̬������ʱ�����ӣ������趨�����ж�
            movement_info_s.run_start_time++;
			
#if USE_MULTI_RUNMODE_EXIT_CONDITION 			
			if((runmode_type == APP_MSM_MODE_TYPE_BIKING_OUTDOOR) || (runmode_type == APP_MSM_MODE_TYPE_BIKING_INDOOR))
			{
				ret = Movement_ExitCriteriaJudgmentType2();
			}
			else
#endif
			{
				ret = Movement_ExitCriteriaJudgment();
			}
            if(ret == 11 || ret == 12)
            {              
                Movement_Exit();
                return;       
            }        
        }        
        
		//�����»��������жϣ�������ʱ������ʱ����¼�����洢���˳�
        if(movement_info_s.run_time < MOVEMENT_RUN_MAX_TIME && movement_info_s.pack_cnt < MOVEMENT_RECORD_MAX_PACK_NUM)
        {
			//��������ģʽ��ʱ������
            movement_info_s.run_time++;
        }
        else
        {                
            Movement_Exit();
            return;
        }  
		//��ͣ�������жϣ���ͣʱ�������趨ʱ��
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
		//ÿ�������ݱ���
        if(movement_info_s.run_time % 60 == 0 && movement_info_s.run_time != 0)
        {
            //��������
            //Movement_SaveMovementData(1);            
            Movement_AddSaveOptEvent(2);
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
    uint8_t error_code = 0x00;
    
    if(block >= MOVEMENT_PATTERNS_MAX_BLOCK)
    {
        return 0;
    }

    error_code = Extflash_MovementEraseBlockSet(block);

    return error_code;
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
    uint8_t error_code = 0x00;
    
    error_code = Extflash_MovementEraseAllSet();
     
    return error_code;
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
uint8_t Movement_SaveMovementData(uint8_t state)
{    
//    BleThread_SaveMovementData();
    uint8_t average_heart = 0;
    sport_information_info_t gsii_sport_info_s = {0x00, };
    
    if(movement_info_s.pack_cnt >= 299)
    {
        if(state == 1)
        {
            Movement_Exit();
        }
        return 0;
    }
    
    Get_SportInformationInfo(&gsii_sport_info_s);
    
    average_heart = Util_AverageByUint8(movement_heart_buf, movement_heart_cnt);
    
    // WJ.20190724.����һ��ʱ���ڵ�ƽ������
    if(average_heart > 30 && movement_info_s.ui_heart < 200)
    {
        average_heart_totaltime += average_heart;
        average_heart_cnt_totaltime++;
    }
	
    
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
    
    Extflash_MovementWriteDataSet(movement_info_s.block, movement_info_s.pack_cnt++, (uint8_t *)&movement_data_s, 
                               sizeof(movement_patterns_data_t));
    // ֱ�Ӳ���-���ȴ�����ͨ���ź���
    Extflash_MovementDeal(); 
    
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
    return Extflash_MovementWriteHeadSet(movement_info_s.block, (uint8_t *)&movement_head_s, sizeof(movement_patterns_head_t));
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
        Movement_FlashReadHead(i, (void *)&movement_head_temp_s, sizeof(movement_patterns_head_t));
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

    Movement_FlashReadHead(block, (void *)&movement_head_temp_s, sizeof(movement_patterns_head_t));       
 
    if(movement_head_temp_s.flag != 0x00 || movement_head_temp_s.crc == 0x00 || movement_head_temp_s.crc == 0xFFFF)
    {
        return 0;
    }
    
    movement_send_s.movement_block = block;
    movement_send_s.movement_num_start = 0;
    movement_send_s.movement_num_total = movement_head_temp_s.record_cnt + MOVEMENT_HEAD_MAX_PACK_NUM;
    if(movement_send_s.movement_num_total > 300)
    {
        movement_send_s.movement_num_total = 0;
    }

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
** Function name:           Movement_SendCompleteNotify
** Descriptions:            
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

/****************************************************************
** Function name:           Movement_ExitCriteriaJudgmentType2
** Descriptions:            �ж��Ƿ��˳�����2
** input parameters:        ��
** output parameters:       �� 
** Returned value:          ����11��12���˳�,��������
** Created by:              WJ              
** Created Date:            2017-07-28
*****************************************************************/ 
static uint8_t Movement_ExitCriteriaJudgmentType2(void)
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
        if(all_move >= 20)
        {
            return 2;
        }
        else
        {
            return 11;
        }            
    }
    else
    {
        if(all_move >= 40)
        {
            return 4;
        }
        else
        {
            return 12;
        }                
    }    
}

/****************************************************************
** Function name:           Movement_DisplayInit
** Descriptions:            �˶�ģʽ��ʾ������ʼ��
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-08-04
*****************************************************************/ 
void Movement_DisplayInit(void)
{
    uint8_t block_temp = 0;
//    uint8_t flash_block_usable = 0;
    
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
    
    movement_max_puse_time = &p_flash_param->movement_max_puse_time;
         
}

void Movement_AddSaveOptEvent(uint8_t opt_type)
{
    if(opt_type == 1)
    {
        move_save_opt_s.move_save_total++;
        move_save_opt_s.move_save_start = move_save_opt_s.move_save_total;
        Gui_ThreadWakeupMovementOpt();
    }
    else if(opt_type == 2)
    {
        move_save_opt_s.move_save_total++;
        move_save_opt_s.move_save_min = move_save_opt_s.move_save_total;
        Gui_ThreadWakeupMovementOpt();
    }
    else if(opt_type == 3)
    {
        move_save_opt_s.move_save_total++;
        move_save_opt_s.move_save_exit = move_save_opt_s.move_save_total;
        Gui_ThreadWakeupMovementOpt();
    }  
}


void Movement_SaveOptEventDeal(void)
{
    if(move_save_opt_s.move_save_start == 0 
      || move_save_opt_s.move_save_min == 0
      || move_save_opt_s.move_save_exit == 0)
    {
        move_save_opt_s.move_save_total = 0;
        return;
    }
    
    while(move_save_opt_s.move_save_start != 0 
        || move_save_opt_s.move_save_min != 0
        || move_save_opt_s.move_save_exit != 0)
    {
        if(move_save_opt_s.move_save_start > move_save_opt_s.move_save_min 
          && move_save_opt_s.move_save_start > move_save_opt_s.move_save_exit)
        {
            //
            Movement_Start();
            move_save_opt_s.move_save_start = 0;
        }
        if(move_save_opt_s.move_save_min > move_save_opt_s.move_save_start
          && move_save_opt_s.move_save_min > move_save_opt_s.move_save_exit)
        {
            Movement_SaveMovementData(1);
            move_save_opt_s.move_save_min = 0;        
        }
        if(move_save_opt_s.move_save_exit > move_save_opt_s.move_save_start
          && move_save_opt_s.move_save_exit > move_save_opt_s.move_save_min)
        {
            Movement_Exit();
            move_save_opt_s.move_save_exit = 0;        
        }
    }
}

/****************************************************************
** Function name:           Movement_GetMinuteData
** Descriptions:            
** input parameters:        ��
** output parameters:       ��
** Returned value:          
** Created by:                            
** Created Date:            
*****************************************************************/ 
uint8_t Movement_GetSecondHeartData(void)
{ 
    uint8_t average_heart = 0;
    
    average_heart = Util_AverageByUint8(movement_heart_buf, movement_heart_cnt);
    
    if(average_heart > 30 && movement_info_s.ui_heart < 200)
    {
        average_heart_totaltime += average_heart;
        average_heart_cnt_totaltime++;
    }
    
    if(average_heart_cnt_totaltime != 0)
    {
        average_heart = average_heart_totaltime / average_heart_cnt_totaltime;
    }
    
    return (uint8_t)average_heart;
}


