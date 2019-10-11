/***************************Copyright(c)***********************************
**                       Veepoo Tech
**                  COPYRIGHT (C) 2016-2019
**-----------File Info-----------------------------------------------------
** File        name:     daily_info.h
** Last modified Date:   2017-08-01
** Last SYS_VERSION:     0.0.0
** Descriptions:            
**-------------------------------------------------------------------------
** Created  by:          WJ
** Created  date:        2017-08-01
** SYS_VERSION:          0.0.0
** Descriptions:         
**-------------------------------------------------------------------------
** Modified by:				
** Modified date:  			          
** SYS_VERSION:                 
** Descriptions:                        
***************************************************************************/
#ifndef __DAILY_INFO_H
#define __DAILY_INFO_H

#define USE_DAILY_INFO  1       //是否开启睡眠开关

#if USE_DAILY_INFO
#include <stdint.h>

#define DAILY_PKG_MAX_NUM   (358 - 2)

#pragma pack(1)
typedef struct{
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t steps_h;
    uint8_t steps_l;
    uint8_t movement_h;
    uint8_t movement_l;
    uint8_t heart;
    uint8_t bp_h;
    uint8_t bp_l;
    uint8_t sleep_posture;  //睡眠姿态
    uint8_t sleep_code;     //睡眠计算代码
    uint8_t sleep_posture_cnt;  //睡眠姿态次数;
    uint8_t distance_h;
    uint8_t distance_l;
    uint8_t calories_h;
    uint8_t calories_l;
    uint8_t reserve6;       //复位代码
    uint8_t reserve7;       //复位时间分钟
}daily_data_format_t;       //size = 20   

typedef struct{
    uint8_t block;
    uint16_t pkg_cnt;
}daily_pkg_info_t;

typedef struct{
    uint8_t     block_flag;
    uint8_t     month;
    uint8_t     day;    
    uint8_t     all_step[3];
    uint16_t    pack_num;
    uint16_t    sleep_cnt;
    uint32_t    all_calories;  //存储卡路里
    uint32_t    all_distance;  //存储距离  
    uint8_t     reserve[22];
}daily_block_info_t;

typedef struct{
    uint32_t last_setp_num;
    uint32_t last_sport_momentum;
    uint32_t last_setp_num_5m;
    uint32_t last_setp_num_10m;
    uint32_t last_distance;
    uint32_t last_calories;
}daily_last_info_t;

#pragma pack()

/*********************************************************************
**  Functions declare
*/

//获取前几天存储日常数据的块号
uint8_t Daily_GetPreviousBlock(uint8_t previous_num);

//保存五分钟数据
void Daily_SaveDailyData(void);

//get 5 minute period daily data
void Daily_GetDailyData(void);

//初始化化
void Daily_Init(void);

//设置当天的存储块号和记录条数
void Daily_SetDailyPkgInfo(uint8_t block, uint16_t pkg_cnt);

//换块处理
void Daily_NextDay(void);

//设置日常数据的血氧、心率、血压值
void Daily_SetDailyData(uint8_t oxgen, uint8_t heart, uint8_t bp_dp, uint8_t bp_sp);

//设置当前块号信息为正在使用，保存头部信息
void Daily_SeHeadtUsing(void);

//设置当前块号信息为使用完成，保存头部信息
void Daily_DailyHeadSetUsed(const uint8_t month, const uint8_t day);

//清除指定块号的信息，并作为当天的存储块号
void Daily_ClearDailyByBlcak(uint8_t black);

//清除当天的日常数据存储，用于对时
void Daily_ClearDailyCurrentBlcak(void);

// 获取当前存储日常数据信息
daily_pkg_info_t *Daily_GetPkgInfo(void);

//睡眠算法获取日常数据回调函数接口
uint32_t Daily_SleepGetDailyDataCallback(uint8_t *data, uint16_t current_pkg, uint16_t pre_offset);

//获取指定块号的存储头部信息
uint8_t Daily_GetBlockHeadInfo(daily_block_info_t *daily_block_info, uint8_t block_num);

//获取指定天的总计步数据
uint32_t Daily_GetAllWalkCntByDays(uint8_t ago_days);

//设置最后5分钟的计步
void Daily_SetIntervalSteps5m(uint32_t setp_num );

//获取最后5分钟的记步数据
uint32_t Daily_GetIntervalSteps5m(void);

//获取最后10分钟的记步数据
uint32_t Daily_GetIntervalSteps10m(void);

//用于测试睡眠数据使用，存储包号加1
void Daily_TestSleepAddPkg(void);

//设置睡眠算法返回代码，存储到下一个五分钟记录中
void Daily_SetSleepCode(uint8_t code);

//获取最后设置的睡眠算法返回代码
uint8_t Daily_GetSleepCode(void);

//初始化日常信息
void Daily_InitFromFlash(void);


#endif  //end USE_DAILY_INFO


#endif  //end __DAILY_INFO_HSS

