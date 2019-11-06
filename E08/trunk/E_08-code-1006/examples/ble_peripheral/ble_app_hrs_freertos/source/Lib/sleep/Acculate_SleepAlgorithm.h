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
//**20181024
//**占用资源：malloc：4320 local：346 global：220

#ifndef __Acculate_SLEEPALGORITHM_H__
#define __Acculate_SLEEPALGORITHM_H__

// 睡眠算法二版本号
#define VS_VERSION        0x02000400


#ifdef _WIN64
//#define MATLAB_PLOT_MODE
#define WAKE_UP  
#endif


#include "stdlib.h"
#include "stdint.h"
#include "string.h"

#pragma pack(1)

/********************************************************************
** Function name:           Get_WriteRawPos
** Descriptions:            本函数于上电后，算法运行前执行，每次上电仅执行一次。当返回值为251时，不可执行算法。请做好B方案
** input parameters:        wirte_pos：该变量为内存写入地址
                            capacity_bytes：该变量为写入容量
** output parameters:       none
** Returned value:          1：该地址及输入长度被鉴定为可使用
                            251：该地址或输入长度被鉴定为不可使用
** Created by:              FWT
** Created Date:            2019-05-06
********************************************************************/
uint8_t Get_WriteRawPos(void* wirte_pos, uint32_t capacity_bytes);

typedef struct
{
	uint8_t block_num;
	uint16_t block_package_cnt;
}pkg_info_t;
//typedef struct
//{
//	uint8_t deep_shallow[1440];
//	uint8_t turn_body[1440];
//	uint8_t move_onemin[1440];
//	uint16_t len;
//}acc_save_data_t;
//41BYTE
// occ. 47
typedef struct
{
	uint8_t month;
	uint8_t data;
	uint8_t hour;
	uint8_t min;
	uint8_t pulse;
	uint8_t sleep_gesture;
	uint8_t sleep_state_temp;
	uint8_t wrong_flag[6];
	uint8_t turn_body_flag[5];
	//uint8_t ware_flag;
	uint8_t permin_sleep_state[5];
	uint8_t sleep_quality[5];
	uint8_t hr_state[5];
	uint8_t beats_early_flag[5];
	uint8_t ori_data[6];
	uint8_t year;//防止闰年的二月
	uint16_t steps;
	uint16_t momentum;
}daily_info_t;

typedef struct
{
	uint8_t month;
	uint8_t date;
	uint8_t hour;
	uint8_t minute;
}acculate_sleep_time_t;

typedef struct
{
	int8_t cnt_res_save;
	int8_t cnt_res1_save;
}save_enter_sleep_t;

// 8 + 9 + 18 + 3 + 42 + 2882 + 6 = 2968
typedef struct
{
	// First Part
	acculate_sleep_time_t enter_time; //入睡时间点
	acculate_sleep_time_t quit_time; //退出时间点
	uint8_t  sleep_flag;//睡眠标志位
	uint8_t  startup_quality; //起夜得分
	uint8_t  deepsleep_quality; //深睡得分
	uint8_t  averstarttodeep_quality; //睡眠效率得分
	uint8_t  firstdeeptime_quality; //入睡效率得分
	uint8_t  all_sleep_len_quality; //睡眠时长得分
	uint8_t  quit_mode; //退出睡眠模式
	uint8_t  quanlity_val; //睡眠质量
	uint8_t  startup_times;//起夜次数
	uint16_t deep_shallow_len_mode; // 深浅睡眠模式
	uint16_t deep_sleep_len; //深睡时长
	uint16_t shallow_sleep_len; //浅睡时长
	uint16_t else_sleep_len; //其他时长
	uint16_t all_sleep_len; //睡眠总时长
	uint16_t first_deep_time; //第一次进入深睡时间
	uint16_t startup_len; //起夜总时长
	uint16_t aver_start_to_deep; //起夜到深睡均值
	uint16_t one_point_how_many_seconds; //一个点代表多少秒
	
	// Second Part
	uint8_t  loss_sleep_flag; //失眠标志
	uint8_t  loss_sleep_degree; //失眠得分
	uint8_t  loss_sleep_times; //失眠次数
	uint16_t loss_sleep_start_time[10]; //失眠开始时间
	uint16_t loss_sleep_end_time[10]; //失眠结束时间
	uint16_t loss_sleep_length; //失眠长度
	
	// Third Part
	uint16_t temp_len;//深浅睡眠数组有效长度
	uint8_t  deep_shallow[2880]; //深浅睡眠数组

	// continue Flag
	uint8_t  as_continue_flag;

	// Forth Part
	uint16_t CRC_value1;//基本睡眠信息校验CRC1
	uint16_t CRC_value2;//失眠信息校验CRC2
	uint16_t CRC_value3;//睡眠曲线信息校验CRC3

}acculate_sleep_quality_t;
//typedef struct
//{
//	uint8_t times;
//	uint8_t enter_date[10];
//	uint8_t enter_hour[10];
//	uint8_t enter_min[10];
//	uint8_t quit_date[10];
//	uint8_t quit_hour[10];
//	uint8_t quit_min[10];
//}acc_sleep_startup_t;

// occ. 80
typedef struct
{
	uint8_t sleep_flag;
	uint8_t enter_mode;
	uint8_t quit_mode;
	uint8_t cali_flag;
	uint8_t cali_val;
	uint8_t quanlity_val;

	uint8_t deep_shallow[36];
	//起夜时间点
	uint8_t start_time[12];
	uint8_t end_time[12];
	uint16_t sleep_show_len;
	uint16_t deep_time;
	uint16_t shallow_time;

	acculate_sleep_time_t enter_time;
	acculate_sleep_time_t quit_time;
}acculate_sleep_info_t;

/*根据app协议定义此结构体，用于写入flash，修改时可删除，使用vph_sleep_info_t*/

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
}acculate_sleep_quality_info_t;
#pragma pack()

//callback function is used to read flash data
//callback function . the max size is 20 bytes.include 
//month(1 byte) 0
//date(1 byte) 1
//hour(1 byte) 2
//miniute(1 byte) 3
//steps(2 byte)  4，5
//movement(2 byte) 6，7
//wear(1 byte)  8
//gesture(1 byte)  9
//sleep state 1(1 byte)  10
//sleep state 2(1 byte)  11
//sleep state 3(1 byte)  12
//sleep state 4(1 byte)  13
//sleep state 5(1 byte)  14
//sleep state 6(1 byte)  15
//year                   16
//NULL(position 17~20)
//choose_flag为0时如下回调，两个小时的数据
//month(1 byte) 
//date(1 byte) 
//hour(1 byte) 
//miniute(1 byte) 
//steps(2 byte)
//movement(2 byte)
//wear(1 byte)
//gesture(1 byte)
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
void Acculate_SleepingInit(load_flash_callback_t load_flash_cb);

/********************************************************************
** Function name:           Vph_SleepDetectEntery
** Descriptions:
** input parameters:        vsde_pkg_num：1 - 288
acc_or_not_flag:			1:精准睡眠; 0:纯运动量睡眠
** output parameters:       none
** Returned value:          none
** Created by:              SQH
** Created Date:            2016-09-07
********************************************************************/
uint8_t Acculate_SleepDetectEntery(uint16_t vsde_pkg_num, 
	uint8_t vsde_pkg_sleep_out, uint8_t acc_or_not_flag);

/********************************************************************
** Function name:           Vs_GetSleepingInfo
** Descriptions:            该函数必须被调用，内部含有赋值代码
** input parameters:        none
** output parameters:       none
** Returned value:          none
** Created by:              HYR
** Created Date:            2016-08-07
********************************************************************/
const acculate_sleep_quality_t* Vs_GetSleepingInfo(void);

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
							acc_or_not_flag:			1:精准睡眠; 0:纯运动量睡眠
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
void Acculate_SleepLowBattery(uint16_t vsde_pkg_num, uint8_t acc_or_not_flag);

/********************************************************************
** Function name:           Acc_QuitByBtn
** Descriptions:			If the user clicks on the sleep interface in the morning, he will exit sleep.
** input parameters:        acc_or_not_flag:			1:精准睡眠; 0:纯运动量睡眠
** output parameters:       none
** Returned value:          none
** Created by:              FWT
** Created Date:            2018-11-24
********************************************************************/
uint8_t Acc_QuitByBtn(uint16_t vsqi_pkg_num, uint8_t acc_or_not_flag);
/********************************************************************
** Function name:           Vs_GetPackageData
** Descriptions:
** input parameters:        none
** output parameters:       none
** Returned value:          none
** Created by:              SQH
** Created Date:            2016-09-07
********************************************************************/
uint8_t Vs_GetPackageData(pkg_info_t vgpd_package_info, daily_info_t *vgpd_data_output, uint16_t pre_offset);



uint8_t Acculate_SleepingACCPurely(pkg_info_t vsap_cur_pkg, uint8_t vsde_pkg_sleep_out);
/*------------------------------- for test -------------------------------*/
void Vs_GetSaveInfo(acculate_sleep_info_t vgsi_sleep_info, uint8_t *vgsi_block_num);

/*---------------------- pure acc judge algorithm ---------------------------*/
typedef struct {
	uint16_t step_over_40_flag : 1;
	uint16_t movement_over_280_flag : 1;
	uint16_t enter_sleep_flag : 1;
	uint16_t two_gether_momentum_over_250_flag : 1;
	uint16_t step_over_20_flag : 1;
	uint16_t step_over_10_flag : 1;
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
	uint16_t movement_sleep_state : 1;
    uint16_t res : 13;
}vs_pure_acc_enter_t;

typedef struct {
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
	uint16_t over_momment_sleep_state : 1;
	uint16_t keep_same_flag : 1;
	uint16_t over_sleep4_flag : 1;

	uint16_t reserve : 2;
}vs_pure_acc_quit_t;

typedef struct {
	uint16_t step_over_25_flag : 1;          //出现记步大于25步
	uint16_t momentum_over_280_flag : 1;     //出现运动量大于280
	uint16_t momentum_over_100_flang : 1;    //连续两个运动量大于100
	uint16_t fall_asleep_flag : 1;
	uint16_t reserve : 12;
	acculate_sleep_time_t not_asleep_time;
	acculate_sleep_time_t next_judge_time;
}vs_time_filter_t;

typedef struct {
	uint8_t not_momentum_10_flag : 1;
	uint8_t momentum_13_flag : 1;
	uint8_t not_momentum_200 : 1;
	uint8_t not_step_10_flag : 1;
	uint8_t momentum_20_flag : 1;
	uint8_t res : 3;
}vs_enter_extra_t;

typedef struct {
	uint8_t momentum_10_cnt;
	uint8_t momentum_13_flag;
	uint8_t momentum_100_50_cnt;
	uint8_t step_10_cnt;
	uint16_t last_momentum;
}vs_enter_extra_cnt_t;

#endif







