/****************************************Copyright (c)****************************************************
**                       		 	            Veepoo Tech
**                          		       COPYRIGHT (C) 2016
**--------------File Info---------------------------------------------------------------------------------
** File name:               vph_bp_24bit.h
** Last modified Date:      2018-12-12
** Last Version:            02.00.01.00
** Descriptions:            Blood Pressure Detection
** 1.data type: 4404,green 50Hz,24bit sample rate
** 2.修改自4404,green,200Hz,12bit(?) sample rate
**--------------------------------------------------------------------------------------------------------
** Modified by:             YQW
** Modified date:           2018-12-21
** DES:                     1.add result process 
** SYS_VERSION:             0x02000101
**--------------------------------------------------------------------------------------------------------
** Modified by:             YQW
** Modified date:           2019-02-20
** DES:                     1.add result process (output value when wearing)
** SYS_VERSION:             00x02000100
**--------------------------------------------------------------------------------------------------------
** Modified by:             YQW
** Modified date:           2019-02-26
** DES:                     1.try to process private mode
** SYS_VERSION:             0x02000101
**--------------------------------------------------------------------------------------------------------
** Modified by:             YQW
** Modified date:           2019-03-18
** DES:                     1.fix big scale bug.
** SYS_VERSION:             0x02000102
**--------------------------------------------------------------------------------------------------------
** Modified by:             YQW
** Modified date:           2019-04-02
** DES:                     1.优化了私人模式下的血压映射方式
** SYS_VERSION:             0x02000103
*********************************************************************************************************/
#ifndef __VPH_BP_24BIT__
#define __VPH_BP_24BIT__
#include <stdbool.h>
#include <stdint.h>

#define VPH_BP_USAGE_FOR_STANDALONE  (0x01)     // 独立成库发布时
#define VPH_BP_USAGE_FOR_INTEGRATION (0x02)     // 作为集成算法的一部分一起编译发布时
#define VPH_BP_USAGE_TYPE            VPH_BP_USAGE_FOR_STANDALONE

#pragma pack(1)  // 设置内存对其粒度

/*
*   Memory Usage:(Version 0x02000103)
*       Global: 277(Bytes)
*       Malloc: 2400(Bytes)
*       Local : >=185(Bytes)
*       Const : 96(Bytes)
*/

/*
* user area (can be modified by Embedded software engineer as required )
*/

//-----------------------------------------------------------------
/*
* !!!!!!! Attention !!!!!!!!!!!!!!!!!!!!!
* The following contents are prohibited from modification
* !!!!!!! Attention !!!!!!!!!!!!!!!!!!!!!
*/
//-----------------------------------------------------------------

/*
* macros definitions
*/
#define VPH_BP_VERSION         (0x02000103)
#define BP_DATA_SAMPLE_RATE    (50)            // uints:Hz
#define BP_DATA_CALC_SENCONDS  (6)             // uints:seconds
#define BP_DATA_RENEW_SENCONDS (1)             // uints:seconds
#define BP_DATA_SAMPLING_BITS  (24)            // Sampling digit
#define BP_DATA_CALC_LENGTH    (BP_DATA_SAMPLE_RATE*BP_DATA_CALC_SENCONDS)
#define BP_DATA_RENEW_LENGTH   (BP_DATA_SAMPLE_RATE*BP_DATA_RENEW_SENCONDS)
#define DBP_BASE               (80)
#define SBP_BASE               (120)

typedef struct tag_bp_info
{
    uint8_t bp_flag;    // Testing flag 0 no result ;1 get a result; 2.no valid result 3.error code 4. error code sport  
    uint8_t valid_cnt;  // No use
    uint8_t wave_state; // Wave station 
    uint8_t sp;         // output of high blood pressure
    uint8_t dp;         // output of low blood pressure
    uint8_t pulse_rate; // output of pulse rate
    uint8_t pwv;        // No use
    uint8_t vari;       // No use
    uint8_t test_mode;  // ouptut the test mode
}bp_info_t;

#if VPH_BP_USAGE_TYPE == VPH_BP_USAGE_FOR_STANDALONE
typedef struct tag_test_mode
{
    //Set testing mode for bp detection [0]:normal mode; [1]:private mode 
    uint8_t test_mode;
    //Set the sp for testing.Setting valid when test_mode = 1 
    uint8_t set_sp;
    //Set the dp for testing.Setting valid when test_mode = 1
    uint8_t set_dp;
}test_mode_t;
#endif

//
typedef struct tag_bp_point
{
    uint8_t sbp; // output of high blood pressure 收缩压 
    uint8_t dbp; // output of low blood pressure  舒张压
}bp_point_t;

// blood pressure algorithm result structure definitions
typedef struct bp_result
{
    bp_info_t  bp_info_s;               // detection information
    bp_point_t bp_point_normal_1s;      // normal mode result
    bp_point_t bp_point_normal_25s;
    bp_point_t bp_point_normal_5m;
    bp_point_t bp_point_private_1s;     // private mode result 
    bp_point_t bp_point_private_25s;
    bp_point_t bp_point_private_5m;
}bp_result_t;

/*
*  functions declarations for external call
*/
extern void     BP_DetectionInit(void);
extern int8_t   Bp_DetectionCalc( const int32_t*  vb_signal_input,
                                  int16_t         vb_move_index,
                                  test_mode_t*    vb_mode_info
                                );
extern void     BP_GetResult(bp_result_t* bp_gresult_s);
extern uint32_t BP_GetVersion(void);

#pragma pack()


#endif //!__VPH_BP_24BIT__

