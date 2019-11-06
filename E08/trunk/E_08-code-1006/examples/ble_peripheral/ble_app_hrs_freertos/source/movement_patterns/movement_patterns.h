/***************************Copyright(c)***********************************
**                       Veepoo Tech
**                  COPYRIGHT (C) 2016-2019
**-----------File Info-----------------------------------------------------
** File        name:     movement_patterns.h
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

#ifndef _MOVEMENT_PATTERNS_H
#define _MOVEMENT_PATTERNS_H
#include "flash_data_type.h"

#define MOVEMENT_RUN_MAX_TIME       18000       //运动模式的最大时间5小时
#define MOVEMENT_HEAD_MAX_PACK_NUM  3           //头部信息发送的最大包数
#define MOVEMENT_HEAD_CRC_LEN       (sizeof(movement_patterns_head_t) - 10)
    
#define MOVEMENT_RECORD_MAX_PACK_NUM    301            //记录信息保存的最大包数
    
#define MOVEMENT_MIN_VALID_TIME         60         //运动最小有效保存时间（s）

#define MOVEMENT_PUSE_TIMEOUT_MIN_TIME      3       //3分钟
#define MOVEMENT_PUSE_TIMEOUT_MAX_TIME      120     //120分钟
#define MOVEMENT_PUSE_TIMES_MAX_CNT         200     //最大暂停次数


#define USE_MULTI_RUNMODE_EXIT_CONDITION 1   //是否使用多运动退出方案

enum
{
    MOVEMENT_PATTERNS_DATA_BLOCK_0 = 0,
    MOVEMENT_PATTERNS_DATA_BLOCK_1, 
    MOVEMENT_PATTERNS_DATA_BLOCK_2,
    MOVEMENT_PATTERNS_DATA_BLOCK_MAX            //DAILY_DATA_BLOCK_MAX == DAILY_DATA_BLOCK_TOTAL
};

#define MOVEMENT_PATTERNS_MAX_BLOCK     MOVEMENT_PATTERNS_DATA_BLOCK_MAX     

typedef enum
{
    MOVEMENT_STATE_IDEL = 0,        //空闲
    MOVEMENT_STATE_RUN,             //运动中
    MOVEMENT_STATE_PAUSE,           //暂停中
    MOVEMENT_STATE_FINISH,          //超时完成
}MOVEMENT_STATE_TYPE_E;


typedef enum
{
	APP_MSM_MODE_TYPE_NULL                 ,                    // 未主动进入运动模式
	APP_MSM_MODE_TYPE_RUN_OUTDOOR          ,                    // 户外跑步
	APP_MSM_MODE_TYPE_WALK_OUTDOOR         ,                    // 户外步行
	APP_MSM_MODE_TYPE_RUN_INDOOR           ,                    // 室内跑步
	APP_MSM_MODE_TYPE_WALK_INDOOR          ,                    // 室内步行
	APP_MSM_MODE_TYPE_HIKING               ,                    // 徒步
	APP_MSM_MODE_TYPE_TREADMILLS_MACHINE   ,                    // 踏步机
	APP_MSM_MODE_TYPE_BIKING_OUTDOOR       ,                    // 户外骑行
	APP_MSM_MODE_TYPE_BIKING_INDOOR        ,                    // 室内骑行
	APP_MSM_MODE_TYPE_ELLIPTICAL_MACHINE   ,                    // 椭圆机
	APP_MSM_MODE_TYPE_ROWING_MACHINE       ,                    // 划船机
	APP_MSM_MODE_TYPE_MAX,
}MOVEMENT_SELECT_MODE_TYPE_E;


#pragma pack(1)
//运动模式单条记录,数据信息
typedef struct
{
    uint16_t year;           //年
    uint8_t  month;          //月
    uint8_t  day;            //日
    uint8_t  hour;           //时
    uint8_t  minute;         //分
    uint8_t  heart;          //心率
    uint16_t movement;       //运动量 
    uint16_t step;           //计步 
    uint16_t calories;       //卡路里（ca）
    uint16_t distance;       //距离（米）
    uint8_t  pause_flag;     //暂停标志
    uint8_t  retain[4];      //保留
}movement_patterns_data_t;
//运动模式头部信息,数据信息
typedef struct
{    
    uint8_t flag;               //块号使用标志 
    uint16_t start_year;        //开始年
    uint8_t start_month;        //开始月
    uint8_t start_day;          //开始日
    uint8_t start_hour;         //开始小时
    uint8_t start_minute;       //开始分钟
    uint8_t start_second;       //开始秒
    
    uint16_t end_year;           //结束年
    uint8_t end_month;          //结束月
    uint8_t end_day;            //结束日
    uint8_t end_hour;           //结束小时
    uint8_t end_minute;         //结束分钟
    uint8_t end_second;         //结束秒
    
    uint32_t all_step;          //总计步
    uint32_t all_distance;      //总距离
    uint32_t all_calories;      //总卡路里
    uint32_t all_movement;      //总运动量
    uint16_t record_cnt;        //总记录条数
    uint8_t  pause_times;       //暂停次数
    uint16_t all_pause_time;    //总暂停时间
    uint16_t crc;               //CRC校验 
	uint8_t multi_runmode_type; //多运动模式类型记录
    uint8_t  retain[7];         //保留,对齐
}movement_patterns_head_t;

typedef struct{ 
    uint8_t movement_block;
    uint16_t movement_num_start;
    uint16_t movement_num_total;
}movement_send_info_t;


//运动模式控制,控制信息
typedef struct
{ 
    uint8_t  block;                 //块号
    uint16_t pack_cnt;              //记录条数  
    
    uint8_t  run_state;             //运行状态
    uint16_t run_time;              //进入运动模式总时间（s）
    uint16_t run_vail_time;         //运动模式有效总时间（s）
    uint16_t current_pause_time;    //当前暂停时间
    uint8_t  pause_flag;            //暂停标志（前一分钟内）
    
    uint16_t run_start_time;        //暂停后运行的有效时间（s）

    uint8_t   heart;                  //实时心率
    uint32_t  distance; 
    uint32_t  calories;
    uint32_t  step;
    uint32_t  movement;
    
    uint8_t   ui_heart;                  //实时心率
    uint32_t  ui_distance; 
    uint32_t  ui_calories;
    uint32_t  ui_step;
    uint32_t  ui_movement;
    
    uint32_t last_step;              //最后的计步
    uint32_t last_distance;          //最后距离
    uint32_t last_calories;          //最后卡路里
    uint32_t last_movement;          //最后运动量  
    
    uint32_t pause_step;             //暂停的计步
    uint32_t pause_distance;         //暂停距离
    uint32_t pause_calories;         //暂停卡路里
    uint32_t pause_movement;         //暂停运动量 
     
}movement_patterns_info_t;

typedef struct
{
    uint32_t move_save_start;
    uint32_t move_save_min;
    uint32_t move_save_exit;
    
    uint32_t move_save_total;
}move_save_opt_t;


#pragma pack()

extern movement_send_info_t     movement_send_s;
extern movement_patterns_head_t movement_head_s;   //运动数据头部信息
extern movement_patterns_data_t movement_data_s;   //当前一分钟记录信息
extern uint8_t movement_heart_buf[80];             //存储一分钟心率
extern uint8_t movement_heart_cnt;                 //记录一分钟心率个数

extern MOVEMENT_SELECT_MODE_TYPE_E runmode_type; //当前运动状态控制信息
extern uint8_t sport_mode_para;

//运动模式初始化(上电、复位)
void Movement_RestInit(void);

//运动模式初始化
void Movement_Init(void);

//运动模式开始
void Movement_Start(void);

//运动模式暂停
void Movement_Pause(void);

//保存头部信息
void Movement_SaveHeadInfo(void);

//运动模式结束
void Movement_Finish(void);

//运动模式退出
void Movement_Exit(void);

//运动模式状态
uint8_t Movement_GetState(void);

//运动模式中;返回0：空闲；1：运动模式中
uint8_t Movement_IsBusy(void);

// 运动监听处理，放在1s事件中
void Movement_Monitor(void);

//根据块号擦除运动数据;返回0：擦除失败；1：擦除成功
uint8_t Movement_EreaFlashByBlock(uint8_t block);

//擦除所有运动模式数据;返回0：擦除失败；1：擦除成功;
uint8_t Movement_EreaFlashAllBlock(void);

//保存一分钟运动数据;返回0：保存失败；1：保存成功
uint8_t Movement_SaveMovementData(uint8_t state);

//保存运动数据头部数据;返回0：保存失败；1：保存成功
uint8_t Movement_SaveMovementHead(void);

//获取运动模式头部信息CRC;crc_num：CRC的个数;crc:crc值，crc[0] = CRC0低8bit；crc[1] = CRC0高8bit
void Movement_GetHeadCRC(uint8_t *crc, uint8_t crc_num);

//设置发送运动模式数据信息 
uint8_t Movement_SetSendDataInfo(uint8_t block);

//发送运动数据;返回0：发送结束；1：发送需要继续
uint8_t Movement_SendDataHandler(void);

//暂停超时时间检查;time:暂停超时时间;返回0：无效值；1：有效值
uint8_t Movement_IsVaildThePuseTimeOut(uint16_t time);

movement_patterns_info_t *Movement_GetInfoData(void);

//获取运动是否到达最大暂停的次数；返回0：否,1：是
uint8_t Movement_IsMaxPauseCnt(void);

//用于恢复出厂设置
void Movement_RestoreFactoryExit(void);

extern movement_patterns_info_t movement_info_s;


void Movement_FlashReadHead(uint8_t block_count, uint8_t *head_read_buf, uint16_t head_read_len);

void Movement_FlashReadData(uint8_t block_count, uint16_t number, uint8_t *data, uint16_t read_len);

//运动模式显示参数初始化
void Movement_DisplayInit(void);


void Movement_AddSaveOptEvent(uint8_t opt_type);
void Movement_SaveOptEventDeal(void);

uint8_t Movement_GetSecondHeartData(void);

#endif  //__GUI_DATA_INTEFACE_H


