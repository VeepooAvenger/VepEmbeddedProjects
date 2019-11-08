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
#define MOVEMENT_RUN_MAX_TIME       18000       //运动模式的最大时间5小时
#define MOVEMENT_HEAD_MAX_PACK_NUM  3           //头部信息发送的最大包数
#define MOVEMENT_HEAD_CRC_LEN       (sizeof(movement_patterns_head_t) - 10)
    
#define MOVEMENT_RECORD_MAX_PACK_NUM    301            //记录信息保存的最大包数
    
#define MOVEMENT_MIN_VALID_TIME         60         //运动最小有效保存时间（s）

#define MOVEMENT_PUSE_TIMEOUT_MIN_TIME      3       //3分钟
#define MOVEMENT_PUSE_TIMEOUT_MAX_TIME      120     //120分钟
#define MOVEMENT_PUSE_TIMES_MAX_CNT         200     //最大暂停次数

/*********************************************************************
* Global Variables
*/  
static movement_patterns_info_t movement_info_s = {0x00,};              //运动信息
__ALIGN(4) static movement_patterns_head_t movement_head_s = {0x00,};   //运动数据头部信息
__ALIGN(4) static movement_patterns_data_t movement_data_s = {0x00,};   //当前一分钟记录信息
static movement_send_info_t     movement_send_s = {0x00,};              //运动模式数据发送信息
static uint16_t *movement_max_puse_time = NULL;                         //运动模式暂停超时时间
static uint8_t movement_heart_buf[80] = {0x00,};                        //存储一分钟心率
static uint8_t movement_heart_cnt = 0;                                  //记录一分钟心率个数

static uint8_t movement_notify_flag = 0;                                //运动完成上报app

static uint16_t movement_10minutes_step[10] = {0x00, };                  //10分钟计步
static uint32_t movement_10minutes_move[10] = {0x00, };                  //10分钟运动量

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
** Descriptions:            运动模式打开心率
** input parameters:        无
** output parameters:       无
** Returned value:          无
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
** Descriptions:            运动模式关闭心率
** input parameters:        无
** output parameters:       无
** Returned value:          无
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
** Descriptions:            运动模式设备复位初始化
** input parameters:        无
** output parameters:       无
** Returned value:          无
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
** Descriptions:            运动模式初始化
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-08-04
*****************************************************************/ 
void Movement_Init(void)
{
    uint8_t block_temp = 0;
    uint8_t flash_block_usable = 0;
    
    block_temp = movement_info_s.block;
    memset(&movement_info_s, 0x00, sizeof(movement_patterns_info_t));
    //WJ.20170913.修改清除大小，清除不全导致保留了上一次的部分结果
    memset(&movement_head_s, 0x00, sizeof(movement_patterns_head_t));  
    memset(&movement_data_s, 0x00, sizeof(movement_patterns_data_t));     
    movement_info_s.block = block_temp;
    
    movement_10minutes_step_all = 0;
    movement_10minutes_move_all = 0;
    memset(&movement_10minutes_step, 0x00, sizeof(movement_10minutes_step)) ;     //10分钟计步                       
    memset(&movement_10minutes_move, 0x00, sizeof(movement_10minutes_step)) ;     //10分钟运动量
  
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
** Descriptions:            运动模式开始
** input parameters:        无
** output parameters:       无
** Returned value:          无
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
        /* 第一次开始启动运动模式*/
        Movement_Init();
        movement_info_s.run_time = 0;
        movement_info_s.current_pause_time = 0;
        
//        //重置暂停的计步、距离、卡路里、运动量
//        {                
//            //运动量
//            movement_info_s.last_movement = Work_MeasureGet();         
//            //计步          
//            movement_info_s.last_step = gsii_sport_info_s.posi_steps; 
//            //卡路里（小卡）
//            movement_info_s.last_calories = gsii_sport_info_s.posi_cal;  
//            //距离（米）
//            movement_info_s.last_distance = gsii_sport_info_s.posi_distance;  
//        } 
            

        App_FlashReadMovementHead(movement_info_s.block, (void *)&movement_head_temp_s, sizeof(movement_patterns_head_t));       
     
        if(movement_head_temp_s.flag != 0xFF)
        {
            Movement_EreaFlashByBlock(movement_info_s.block);
        }
        
        //保存头部信息             
        movement_head_s.flag = DAILY_HEAD_FLAG_USING;
        movement_head_s.start_year = clock->year;           //开始年
        movement_head_s.start_month = clock->month;         //开始月
        movement_head_s.start_day = clock->day;             //开始日
        movement_head_s.start_hour = clock->hour;           //开始小时
        movement_head_s.start_minute = clock->minutes;      //开始分钟
        movement_head_s.start_second = clock->seconds;      //开始秒                   
    }
    
    if(movement_info_s.run_time >= MOVEMENT_RUN_MAX_TIME)
    {
        return ;
    }
    
    movement_info_s.run_state = MOVEMENT_STATE_RUN;
    movement_head_s.all_pause_time += movement_info_s.current_pause_time;
    movement_info_s.current_pause_time = 0;
    
    //重置最后的计步、距离、卡路里、运动量
    movement_info_s.last_step = gsii_sport_info_s.posi_steps;
    movement_info_s.last_movement = Work_MeasureGet();  
    movement_info_s.last_distance = gsii_sport_info_s.posi_distance;
    movement_info_s.last_calories = gsii_sport_info_s.posi_cal;      
    //需要开启心率检测，暂时定位不做佩戴检测
    Movement_OpenHeart();
    
    Movement_ExitCriteriaInit();
}

/****************************************************************
** Function name:           Movement_Pause
** Descriptions:            运动模式暂停
** input parameters:        无
** output parameters:       无
** Returned value:          无
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
    //重置暂停的计步、距离、卡路里、运动量
    {                
        //运动量
        movement_info_s.pause_movement += Work_MeasureGet() - movement_info_s.last_movement;         
        //计步          
        movement_info_s.pause_step += gsii_sport_info_s.posi_steps - movement_info_s.last_step; 
        //卡路里（小卡）
        movement_info_s.pause_calories += gsii_sport_info_s.posi_cal - movement_info_s.last_calories;  
        //距离（米）
        movement_info_s.pause_distance += gsii_sport_info_s.posi_distance - movement_info_s.last_distance;  
    }    
       
    //需要关闭心率检测   
    Movement_CloseHeart();
    
    //实时心率
    movement_info_s.ui_heart = 0;                  
}

/****************************************************************
** Function name:           Movement_SaveHeadUsingInfo
** Descriptions:            设置当前块正在使用
** input parameters:        无
** output parameters:       无
** Returned value:          无
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
** Descriptions:            运动模式结束数据保存
** input parameters:        无
** output parameters:       无
** Returned value:          无
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
    //关闭心率测量、保存运动结果数据
    //需要关闭心率检测 
    Movement_CloseHeart();
    
    if(movement_info_s.run_vail_time < MOVEMENT_MIN_VALID_TIME)
    {
        //运动有效时间小于60s不保存头部信息，记录无效
        return;
    }
    
    //保存最后一组数据(1分钟)
    Movement_SaveMovementData();
    
    //确保最后的暂停时间    
    movement_head_s.all_pause_time += movement_info_s.current_pause_time;
    movement_info_s.current_pause_time = 0;
    
    //保存头部信息
    movement_head_s.flag = 0;                         //块号使用标志 
    movement_head_s.end_year = clock->year;           //结束年
    movement_head_s.end_month = clock->month;         //结束月
    movement_head_s.end_day = clock->day;             //结束日
    movement_head_s.end_hour = clock->hour;           //结束小时
    movement_head_s.end_minute = clock->minutes;      //结束分钟
    movement_head_s.end_second = clock->seconds;      //结束秒         
    
    movement_head_s.all_calories = movement_info_s.calories;
    movement_head_s.all_distance = movement_info_s.distance;
    movement_head_s.all_movement = movement_info_s.movement;
    movement_head_s.all_step = movement_info_s.step;
    
    movement_head_s.record_cnt = movement_info_s.pack_cnt;
     
    movement_head_s.crc = App_Crc16((uint8_t *)&movement_head_s.flag + 1, MOVEMENT_HEAD_CRC_LEN);   //CRC校验 
    memset((uint8_t *)movement_head_s.retain, 0x00, 8);         //保留,对齐
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
** Descriptions:            运动模式退出
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-08-04
*****************************************************************/ 
void Movement_Exit(void)
{
    //WJ.20170906.防止多次关闭心率测量、多次上报运动数据更新 
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
** Descriptions:            用于恢复出厂设置
** input parameters:        无
** output parameters:       无
** Returned value:          无
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
** Descriptions:            运动模式中
** input parameters:        无
** output parameters:       无
** Returned value:          返回运动模式状态
** Created by:              WJ              
** Created Date:            2017-08-04
*****************************************************************/ 
uint8_t Movement_GetState(void)
{
    return movement_info_s.run_state;
}

/****************************************************************
** Function name:           Movement_IsBusy
** Descriptions:            运动模式中
** input parameters:        无
** output parameters:       无
** Returned value:          返回0：空闲；1：运动模式中
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
** Descriptions:            运动监听处理，放在1s事件中
** input parameters:        无
** output parameters:       无
** Returned value:          无
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
    
    movement_info_s.ui_heart = Heart_Test_Value_Get();                  //实时心率
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
** Descriptions:            运动监听处理，放在1s事件中
** input parameters:        无
** output parameters:       无
** Returned value:          无
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
            //WJ.20170830.位置错误
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
            //movement_max_puse_time 使用的分钟基数
            if(movement_info_s.current_pause_time >= (*movement_max_puse_time * 60))
            {              
                Movement_Exit();
                return;
            }
        }
        if(movement_info_s.run_time % 60 == 0 && movement_info_s.run_time != 0)
        {
            //保存数据
            Movement_SaveMovementData();
        }
    }    
}

/****************************************************************
** Function name:           Movement_EreaFlashByBlock
** Descriptions:            根据块号擦除运动数据
** input parameters:        无
** output parameters:       无
** Returned value:          返回0：擦除失败；1：擦除成功
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
** Descriptions:            擦除所有运动模式数据
** input parameters:        无
** output parameters:       无
** Returned value:          返回0：擦除失败；1：擦除成功
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
** Descriptions:            保存一分钟运动数据
** input parameters:        无
** output parameters:       无
** Returned value:          返回0：保存失败；1：保存成功
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
        //心率
        movement_data_s.heart = average_heart;          
        //运动量
        movement_data_s.movement = (Work_MeasureGet() - movement_info_s.last_movement + movement_info_s.pause_movement) >> 12;   
        movement_info_s.pause_movement = 0;
        movement_info_s.last_movement = Work_MeasureGet();          
        //计步 
        movement_data_s.step = gsii_sport_info_s.posi_steps - movement_info_s.last_step + movement_info_s.pause_step;
        movement_info_s.pause_step = 0;
        movement_info_s.last_step = gsii_sport_info_s.posi_steps;
        //卡路里（小卡）
        movement_data_s.calories = gsii_sport_info_s.posi_cal - movement_info_s.last_calories + movement_info_s.pause_calories; 
        movement_info_s.pause_calories = 0;        
        movement_info_s.last_calories = gsii_sport_info_s.posi_cal; 
        //距离（米）
        movement_data_s.distance = gsii_sport_info_s.posi_distance - movement_info_s.last_distance + movement_info_s.pause_distance; 
        movement_info_s.pause_distance = 0;
        movement_info_s.last_distance = gsii_sport_info_s.posi_distance;
               
        //暂停标志
        movement_data_s.pause_flag = movement_info_s.pause_flag;            
    }
    else
    {
        //心率
        movement_data_s.heart = average_heart;          
        //运动量
        movement_data_s.movement = (movement_info_s.pause_movement) >> 12;   
        movement_info_s.pause_movement = 0;     
        //计步 
        movement_data_s.step = movement_info_s.pause_step;
        movement_info_s.pause_step = 0;
        //卡路里（小卡）
        movement_data_s.calories = movement_info_s.pause_calories; 
        movement_info_s.pause_calories = 0;        
        //距离（米）
        movement_data_s.distance = movement_info_s.pause_distance; 
        movement_info_s.pause_distance = 0;
        
        //暂停标志
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
** Descriptions:            保存运动数据头部数据
** input parameters:        无
** output parameters:       无
** Returned value:          返回0：保存失败；1：保存成功
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
** Descriptions:            获取运动模式头部信息CRC
** input parameters:        crc_num：CRC的个数
** output parameters:       crc:crc值，crc[0] = CRC0低8bit；crc[1] = CRC0高8bit
** Returned value:          无
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
            //低位在前
            crc[i * 2] = movement_head_temp_s.crc & 0xFF;
            crc[i * 2 + 1] = (movement_head_temp_s.crc >> 8) & 0xFF;
        }    
    }   
}

/****************************************************************
** Function name:           Movement_SetSendDataInfo
** Descriptions:            设置发送运动模式数据信息
** input parameters:        无
** output parameters:       无
** Returned value:          无
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
** Descriptions:            发送运动数据
** input parameters:        无
** output parameters:       无
** Returned value:          返回0：发送结束；1：发送需要继续
** Created by:              WJ              
** Created Date:            2017-08-04
*****************************************************************/ 
uint8_t Movement_SendDataHandler(void)
{
    uint8_t movement_ble_ack[20] = {0xD4, 0x00, };
    uint8_t *movement_head_p = NULL;
    uint16_t c_pack_num = 0;
    movement_patterns_head_t movement_head_temp_s = {0x00,};
    movement_patterns_data_t movement_data_temp_s = {0x00,};         //当前一分钟记录信息         
    
    if(movement_send_s.movement_block >= MOVEMENT_PATTERNS_MAX_BLOCK
      || movement_send_s.movement_num_start >= movement_send_s.movement_num_total)
    {        
        movement_ble_ack[5] = movement_send_s.movement_block;
        Ble_SendDataViaBat(movement_ble_ack, 20);  
        return 0; 
    }

    if(movement_send_s.movement_num_start < MOVEMENT_HEAD_MAX_PACK_NUM)
    {   
        //头部信息
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
        //一分钟信息
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
** Descriptions:            暂停超时时间检查
** input parameters:        time:暂停超时时间
** output parameters:       无
** Returned value:          返回0：无效值；1：有效值
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
** Descriptions:            获取信息变量地址
** input parameters:        无
** output parameters:       无
** Returned value:          返回信息变量地址
** Created by:              WJ              
** Created Date:            2017-08-04
*****************************************************************/ 
movement_patterns_info_t *Movement_GetInfoData(void)
{ 
    return &movement_info_s;
}

/****************************************************************
** Function name:           Movement_IsMaxPauseCnt
** Descriptions:            获取运动是否到达最大暂停的次数
** input parameters:        无
** output parameters:       无 
** Returned value:          返回0：否,1：是
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
** Descriptions:            获取运动是否到达最大暂停的次数
** input parameters:        无
** output parameters:       无 
** Returned value:          返回0：否,1：是
** Created by:              WJ              
** Created Date:            2017-07-28
*****************************************************************/ 
static void Movement_SendCompleteNotify(void)
{
    uint8_t movement_ble_ack[20] = {0xD3, 0x00,};

    movement_ble_ack[0] = 0xD3; 
    movement_ble_ack[1] = 0x02;     //表示主动上报        
    movement_ble_ack[2] = 0x01; 

    Movement_GetHeadCRC(&movement_ble_ack[3], 3);
           
    Ble_SendDataViaBat(movement_ble_ack, 20);    
}

/****************************************************************
** Function name:           Movement_ExitCriteriaInit
** Descriptions:            退出条件初始化
** input parameters:        无
** output parameters:       无 
** Returned value:          无
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
    memset(&movement_10minutes_step, 0x00, sizeof(movement_10minutes_step)) ;     //10分钟计步                       
    memset(&movement_10minutes_move, 0x00, sizeof(movement_10minutes_step)) ;     //10分钟运动量
}

/****************************************************************
** Function name:           Movement_ExitCriteriaJudgment
** Descriptions:            判断是否退出
** input parameters:        无
** output parameters:       无 
** Returned value:          返回11、12：退出,其他：否
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

