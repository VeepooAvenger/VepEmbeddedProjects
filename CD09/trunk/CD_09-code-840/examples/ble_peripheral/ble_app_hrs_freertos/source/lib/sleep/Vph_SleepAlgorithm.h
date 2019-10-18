/********************************Copyright(c)***********************************
**                           Veepoo Tech
**                         COPYRIGHT (C) 2016
**--------------File Info-------------------------------------------------------
** File name:               Vph_sleeping.h
** Last modified Date:      2016-08-04
** Last SYS_VERSION:        01.01.00.01
** Descriptions:
**
**------------------------------------------------------------------------------
** Created by:              HYR
** Created date:            2016-08-04
** SYS_VERSION:             01.01.00.01
** Descriptions:
1.sleep_flag = 0 no sleeping
sleep_flag = 1 enter sleeping
sleep_flag = 2 sleeping
sleep_flag = 3 quit sleeping

enter_mode = 0  no enter info
enter_mode = 1

quit_mode = 0 no quit info
quit_mode = 1
quit_mode = 250 no wearing quit
quit_mode = 255 exception quit

vsd_wear_flag = 0 wearing
vsd_wear_flag = 1 for now no wearing
vsd_wear_flag = 2 long time no wearing
**
**------------------------------------------------------------------------------
** Modified by:
** Modified date:
** SYS_VERSION:
** Descriptions:  1.算法需在调用前需在main.c中进行初始化
**				  2.算法在每次5分钟睡眠数据写入后执行，读取本次写入数据，并进行算法判断。
**					为了确保数据已经正确写入flash，请在写入数据时刻的后一分钟或两分钟进行算法判断。
**				  3.必须在本次睡眠检测结束，下次睡眠检测开始之前判断是否退出睡眠，
**					并将睡眠信息数据写入flash，否则下一次睡眠判断，睡眠信息缓存会被清除
**				  4.睡眠信息可通过Vs_GetSleepingInfo()函数获取
**
*******************************************************************************/
#ifndef __VPH_SLEEPALGORITHM_H__
#define __VPH_SLEEPALGORITHM_H__

#include "stdlib.h"
#include "stdint.h"
#include "string.h"


typedef struct
{
    uint8_t block_num;
    uint16_t block_package_cnt;
}pkg_info_t;

typedef struct
{
    uint8_t data;
    uint8_t hour;
    uint8_t min;
    uint8_t pulse;
	uint8_t sleep_gesture;
    uint16_t steps;
    uint16_t momentum;
}daily_info_t;

typedef struct
{
    uint8_t date;
    uint8_t hour;
    uint8_t minute;
}vph_sleep_time_t;

typedef struct
{
    uint8_t sleep_flag;
    uint8_t enter_mode;
    uint8_t quit_mode;
    uint8_t cali_flag;
    uint8_t cali_val;
    uint8_t quanlity_val;
    uint8_t deep_time; 
    uint8_t shallow_time;
    uint8_t deep_shallow[36];
    //起夜时间点
    uint8_t start_time[12];
    uint8_t end_time[12];

    vph_sleep_time_t enter_time;
    vph_sleep_time_t quit_time;
}vph_sleep_info_t;

/*根据app协议定义此结构体，用于写入flash，修改时可删除，使用vph_sleep_info_t*/
#pragma pack(1)
typedef struct
{
    uint8_t head;
    uint8_t sleep_enter_time[4];
    uint8_t sleep_out_time[4];
    uint8_t sleep_quality[3];
    uint8_t sleep_shallow[31];
    uint8_t total_time;
    uint8_t start_time[12];
    uint8_t end_time[12];
    uint8_t reserved[28];
}vph_sleep_quality_info_t;
#pragma pack()

//callback function is used to read flash data
typedef uint32_t(*load_flash_callback_t)(uint8_t *data, uint16_t current_pkg, uint16_t pre_offset);


/********************************************************************
** Function name:           Vph_SleepingInit
** Descriptions:
** input parameters:        none
** output parameters:       none
** Returned value:          none
** Created by:              SQH
** Created Date:            2016-09-07
********************************************************************/
void Vph_SleepingInit(load_flash_callback_t load_flash_cb);

/********************************************************************
** Function name:           Vph_SleepDetectEntery
** Descriptions:
** input parameters:        vsde_pkg_num：1 - 288
vsde_cali_val
vsde_wear_flag
vsde_algorithm_mode: 1:纯运动量算法; 0:心率算法
** output parameters:       none
** Returned value:          none
** Created by:              SQH
** Created Date:            2016-09-07
********************************************************************/
uint8_t Vph_SleepDetectEntery(uint16_t vsde_pkg_num);

/********************************************************************
** Function name:           Vs_GetSleepingInfo
** Descriptions:
** input parameters:        none
** output parameters:       none
** Returned value:          none
** Created by:              HYR
** Created Date:            2016-08-07
********************************************************************/
const vph_sleep_info_t* Vs_GetSleepingInfo(void);


/********************************************************************
** Function name:           Vs_EnterInit
** Descriptions:
** input parameters:        none
** output parameters:       none
** Returned value:          none
** Created by:              HYR
** Created Date:            2016-08-07
********************************************************************/
extern uint32_t Vs_GetVersion(void);

/********************************************************************
** Function name:           Vs_ClearInfoMsg
** Descriptions:            clear sleep temp data
** input parameters:        none
** output parameters:       none
** Returned value:          none
** Created by:              HYR
** Created Date:            2016-09-07
********************************************************************/
extern void Vs_ClearInfoMsg(void);

/********************************************************************
** Function name:           Vph_SleepLowBattery
** Descriptions:			low baterry process
** input parameters:        vsde_pkg_num:0-287
** output parameters:       none
** Returned value:          none
** Created by:              SQH
** Created Date:            2016-09-12
**-------------------------------------------------------------------
** Modified by:
** Modified date:
** Descript:
**-------------------------------------------------------------------
********************************************************************/
void Vph_SleepLowBattery(uint16_t vsde_pkg_num);

/********************************************************************
** Function name:           Vs_GetPackageData
** Descriptions:
** input parameters:        none
** output parameters:       none
** Returned value:          none
** Created by:              SQH
** Created Date:            2016-09-07
********************************************************************/
uint8_t Vs_GetPackageData(pkg_info_t vgpd_package_info, uint8_t vgpd_data_len, daily_info_t *vgpd_data_output, uint16_t pre_offset);


uint8_t Vph_SleepingACCPurely(pkg_info_t vsap_cur_pkg);
/*------------------------------- for test -------------------------------*/
void Vs_GetSaveInfo(vph_sleep_info_t vgsi_sleep_info, uint8_t *vgsi_block_num);


/*---------------------- pure acc judge algorithm ---------------------------*/
typedef struct{
    uint16_t step_over_20_flag : 1;
    uint16_t step_over_10_flag : 1;
    uint16_t movement_over_280_flag : 1;
    uint16_t movement_90_280_flag : 1;
    uint16_t movement_150_280_flag : 1;
    uint16_t movement_80_150_flag : 1;
    uint16_t movement_rate_80_flag : 1;
    uint16_t movement_rate_51_flag : 1;
    uint16_t movement_4_over_20_flag : 1;
    uint16_t movement_over_10_flag : 1;
    uint16_t std_deviation_flag : 1;
    uint16_t keep_same_flag : 1;
    uint16_t not_momentum_100_50_flag : 1;
    uint16_t not_momentum_250_flag : 1;
    uint16_t extra_param_flag : 1;          //18:00 judge flag, if set to 1, could fall in sleep
    uint16_t reserve : 1;

}vs_pure_acc_enter_t;


typedef struct{
    uint16_t over_momentum_level1 : 1;
    uint16_t over_momentum_level2 : 1;
    uint16_t over_momentum_level3 : 1;
    uint16_t over_momentum_level4 : 1;
    uint16_t over_momentum_level5 : 1;
    uint16_t over_momentum_level6 : 1;

    uint16_t over_step_level1 : 1;
    uint16_t over_step_level2 : 1;
    uint16_t over_step_level3 : 1;
    uint16_t over_step_level4 : 1;
    uint16_t over_step_level5 : 1;

    uint16_t keep_same_flag : 1;

    uint16_t reserve : 4;

}vs_pure_acc_quit_t;


typedef struct{
    uint16_t step_over_25_flag : 1;          //出现记步大于25步
    uint16_t step_over_10_flag : 1;          //连续两个记步大于10步
    uint16_t momentum_over_280_flag : 1;     //出现运动量大于280
    uint16_t momentum_over_100_flang : 1;    //连续两个运动量大于100
    uint16_t momentum_over_13_flag : 1;      //连续6个运动量大于13
    uint16_t momentum_same_value_flag : 1;   //连续18个运动量相等
    uint16_t fall_asleep_flag : 1;
    uint16_t reserve : 9;
    vph_sleep_time_t not_asleep_time;
    vph_sleep_time_t next_judge_time;
}vs_time_filter_t;


typedef struct{
    uint8_t not_momentum_10_flag : 1;
    uint8_t momentum_13_flag : 1;
    uint8_t not_momentum_200 : 1;
    uint8_t not_step_10_flag : 1;
    uint8_t momentum_20_flag : 1;
    uint8_t res : 3;
}vs_enter_extra_t;


typedef struct{
    uint8_t momentum_10_cnt;
    uint8_t momentum_13_flag;
    uint8_t momentum_100_50_cnt;
    uint8_t step_10_cnt;
    uint16_t last_momentum;
}vs_enter_extra_cnt_t;


#endif
