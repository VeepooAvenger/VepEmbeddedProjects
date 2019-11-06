/********************************Copyright(c)***********************************
**                           Veepoo Tech
**                         COPYRIGHT (C) 2016
**--------------File Info-------------------------------------------------------
** File name:               pave_infoalgorithm.h
** Last modified Date:       2016-07-27
** Last SYS_VERSION:         01.01.00.01
** Descriptions:
**
**------------------------------------------------------------------------------
** Created by:              FWT
** Created date:            2018-10-22
** SYS_VERSION:                 01.01.00.01
** Descriptions:
**------------------------------------------------------------------------------
** Modified by:             FWT
** Modified date:           2018-10-22
** SYS_VERSION:             0x01010002
** Descriptions:            1.change the struct define
							2.malloc：8000 local：4206 global：656
**------------------------------------------------------------------------------
** Modified by:             FWT
** Modified date:           2018-10-22
** SYS_VERSION:             0x01010003
** Descriptions:            1.change the struct define
2.malloc：8000 local：4206 global：656
**------------------------------------------------------------------------------
** Modified by:             YQW
** Modified date:           2019-02-28
** SYS_VERSION:             0x01010012
** Descriptions:            1.替换血压算法到0x02000101_20190226版本
                            2.将test_mode_t的定义从vph_bp_24bit.h 移动到本文件中
                            3.替换睡眠算法1到0x01010002_20190228版本
**------------------------------------------------------------------------------
** Modified by:             YQW
** Modified date:           2019-03-01
** SYS_VERSION:             0x01010013
** Descriptions:            1.加入PPG呼吸率算法算法(0x01010104_20190301),在综合调度文件(pwave_infoalgorithm.c)中添加了算法调用逻辑
                            2.编译时裁剪了佩戴算法,减少代码量
                            3.修正了一些错误的取地址使用方式(清空数组不需要对数组名取地址)
**------------------------------------------------------------------------------
** Modified by:             YQW
** Modified date:           2019-03-02
** SYS_VERSION:             0x01010014
** Descriptions:            1.修改呼吸率有效输出条件 (0,50] ----> [0,50]
**------------------------------------------------------------------------------
** Modified by:             YQW
** Modified date:           2019-03-05(测试版本,不得用于发布)
** SYS_VERSION:             0x01010015
** Descriptions:            1.修改入口函数Pwave_InfoAlgorithm的输入参数个数,增加了一个用于提供各种传感器寄存器实时值的接口;
                            2.当传感器类型为Si1182时，该接口的类型为si1182_register_parameter_t*类型,且此参数不能传入NULL;
                            3.配套修改了佩戴算法调度算法、Si1182佩戴算法具体实现;
                            4.0x01010015版本仅仅用于供嵌入式软件写配套嵌入式程序，不用于发布.
**------------------------------------------------------------------------------
** Modified by:             YQW
** Modified date:           2019-03-06
** SYS_VERSION:             0x01010016
** Descriptions:            1.更新佩戴算法到0x01030003_20190306版本(佩戴原理重大变化,使用红外光ctr来做佩戴)
                            2.在调度文件Pwave_InfoAlgorithm.c中做了相应适配修改
                            3.由于入口函数Pwave_InfoAlgorithm的接口发生变化,请嵌入式软件开发人员做好相应适配。
**------------------------------------------------------------------------------
** Modified by:             YQW
** Modified date:           2019-03-16
** SYS_VERSION:             0x01010018
** Descriptions:            1.sleep_alg------->0x01010003_20190316 . 
**------------------------------------------------------------------------------
** Modified by:             YQW
** Modified date:           2019-03-21
** SYS_VERSION:             0x01010019
** Descriptions:            1.resp_alg------->0x01010105_20190321
**------------------------------------------------------------------------------
** Modified by:             YQW
** Modified date:           2019-04-03
** SYS_VERSION:             0x01010100
** Descriptions:            1.修改为正式版本号用于首次发布.
**------------------------------------------------------------------------------
** Modified by:             YQW
** Modified date:           2019-04-18
** SYS_VERSION:             0x01010101
** Descriptions:            1.更新内部血压算法到02000103_20190402版本,优化私人模式下的血压映射方式.
**------------------------------------------------------------------------------
** Modified by:             YQW
** Modified date:           2019-04-19
** SYS_VERSION:             0x01010102
** Descriptions:            1.更新内部睡眠算法1到0x01010004_20190419版本,修改抖动识别和一分钟汇总
**------------------------------------------------------------------------------
** Modified by:             YQW
** Modified date:           2019-04-22
** SYS_VERSION:             0x01010200
** Descriptions:            1.更新内部睡眠算法1到0x01010005_20190422版本,抖动识别回档上一版本，保留一分钟汇总的修改(sleep_state)
**------------------------------------------------------------------------------
** Modified by:             YQW
** Modified date:           2019-04-27
** SYS_VERSION:             0x01010202
** Descriptions:            1.更新内部血压算法到0x02000104_20190427版本
                            2.更新内部呼吸率算法到0x01010106_20190427版本
**------------------------------------------------------------------------------
** Modified by:             YQW
** Modified date:           2019-04-30
** SYS_VERSION:             0x01010203
** Descriptions:            1.更新内部佩戴算法算法到0x00000004_20190429版本,增加了绿光+环境光的佩戴检测方式;
                            2.si1182_register_parameter_t 结构体类型定义增加了佩戴算法需要的灯光模式字节,相应中断代码也要处理((light_type == SI1182_WEAR_DETECTION_MODE_GREEN_AMB));
                            3.Pwave_InfoAlgorithm调度函数中,针对Si1182的佩戴检测灯光模式做了适配调整。
                            4.此算法兼容其他E系列(Si1182)红外+环境光的佩戴检测方式,但需要对灯光模式字节进行设置(light_type == SI1182_WEAR_DETECTION_MODE_IRED_AMB)。
                            5.兼容代码需要改动内容见pwave_infoalgorithm.h文件最末尾(0x01010203),后期会以文档形式进行明确.
                            6.响应消除编译时warnning要求,除睡眠算法1以外,内部文件所有编译时warnning已经全部消除。
**------------------------------------------------------------------------------
** Modified by:             YQW
** Modified date:           2019-05-07
** SYS_VERSION:             0x01010204
** Descriptions:            1.兼容TP优化,取消对环境光数据指针的强制检查(对应佩戴算法更新到0x01030005版本)
20190508:使用宏包裹可能重复的有关定义
**------------------------------------------------------------------------------
** Modified by:             YQW
** Modified date:           2019-05-24
** SYS_VERSION:             0x01010205
** Descriptions:            1.更新内部睡眠算法1到(0x01010006_20190524)版本,其内部子算法也相应优化,减少240Bytes全局变量占用;
                            2.修改Si1182l绿光检测模式的CTR门限(30-->28)
                            3.对应文档不必更新,仍然使用<<Si1182_PPG 库使用说明书_V1.0.pdf>>
**------------------------------------------------------------------------------
** Modified by:             YQW
** Modified date:           2019-06-04
** SYS_VERSION:             0x01010206
** Descriptions:            1.更新内部睡眠算法1到(01010200_20190604)版本.
                            2.将某些结构体从本.h文件中删除.
                            3.对应文档不必更新,仍然使用<<Si1182_PPG 库使用说明书_V1.0.pdf>>
**------------------------------------------------------------------------------
** Modified by:             YQW
** Modified date:           2019-06-15
** SYS_VERSION:             0x01010207
** Descriptions:            1.更新内部佩戴算法到(01030006_20190615)版本,兼容类E28项目布局.
                            2.输入参数增加了一个布局参数,使用方法请嵌入式阅读<<Si1182_PPG 库使用说明书_V1.1.pdf>>
*******************************************************************************/
#ifndef __PWAVE_INFOALGORITHM_H__
#define __PWAVE_INFOALGORITHM_H__

#include "stdlib.h"
#include "stdint.h"
#include "pedometer.h"

#pragma anon_unions

#define PPG_SENSOR_TYPE         ("SI1182")
#define SI1182_PPG_ALG_VERSION  (0x01010207)
#define BOD_MAX_WAVE_NUM        (60)             /* finding max pulse num  12 to 18DH_2017_0607 */



/* Si1182 light type macros definitions */
#ifndef SI1182_WEAR_DETECTION_MODE_IRED_AMB
#define SI1182_WEAR_DETECTION_MODE_IRED_AMB   (0x00)                                /* ired  + amb    (default value)   */
#define SI1182_WEAR_DETECTION_MODE_IRED       (0x00)                                /* ired only                        */
#define SI1182_WEAR_DETECTION_MODE_GREEN_AMB  (0x01)                                /* green + amb    (E02)             */
#define SI1182_WEAR_DETECTION_MODE_GREEN      (0x01)                                /* green only                       */
#define SI1182_WEAR_DETECTION_MODE_GREEN_IRED (0x02)                                /* green + ired   (reserve )        */
#define SI1182_WEAR_DETECTION_MODE_MAX        SI1182_WEAR_DETECTION_MODE_GREEN
#endif


/* Si1182 led layout type macros definitions  */
#ifndef SI1182_LAYOUT_TYPE_0
#define SI1182_LAYOUT_TYPE_0            (0)     /* symmetry 4 leds         */
#define SI1182_LAYOUT_TYPE_1            (1)     /* no symmetry 3 leds  ,E28 projict   */
#define SI1182_LAYOUT_TYPE_2            (2)     /* invalid in this version */
#define SI1182_LAYOUT_TYPE_VALID_MIN    SI1182_LAYOUT_TYPE_0
#define SI1182_LAYOUT_TYPE_VALID_MAX    SI1182_LAYOUT_TYPE_1
#endif


#ifndef PWAVE_INFO_T
#define PWAVE_INFO_T
typedef struct pwave_info
{
    uint8_t move_state                                  ;         /* 运动量的等级，0，1，2，3;2bit */
    uint8_t sleep_state                                 ;        
    uint8_t turn_body_flag                              ;     
    uint8_t hr_state                                    ;           
    uint8_t beats_early                                 ;        
    uint8_t wrong_flag                                  ;         
    uint8_t slp_result_1min                             ;
}pwave_info_t;
#endif


#ifndef DATA_DC_CHAR_T
#define DATA_DC_CHAR_T
typedef struct tag_data_dc_char
{
    /* 添加直流属性   */
    uint8_t low_satu_flag                               ;
    uint8_t high_satu_flag                              ;
    uint8_t shake_flag                                  ;
    uint8_t mutation_flag                               ;
    uint8_t drift_flag                                  ;
    float   amp_mean                                    ;
    float   peak_diff_max                               ;
}data_dc_char_t;
#endif

/*
 * blood pressure private mode setting ( should match with vph_bp_24bit.h)
 */
#ifndef TEST_MODE_T
#define TEST_MODE_T
typedef struct tag_test_mode
{
    uint8_t test_mode   ;    /* 0:normal mode, 1:private mode                           */
    uint8_t set_sp      ;    /* Set the sp for testing.Setting valid when test_mode = 1 */
    uint8_t set_dp      ;    /* Set the dp for testing.Setting valid when test_mode = 1 */
}test_mode_t;
#endif 

/*
* si1182 register parameter(si1182 wear Alg need) structure
*/
#ifndef __SI1182_REGISTER_PARAMETER_T__
#define __SI1182_REGISTER_PARAMETER_T__
typedef struct tag_si1182_register_parameter
{
    uint8_t light_type  ;                     /* see "Si1182 light type macros definitions" in  pwave_infoalgorithm.h */
    uint8_t layout_type ;                     /* see "Si1182 led layout type macros definitions" in pwave_infoalgorithm.h*/
    union 
    {
        struct
        {
            uint8_t i_ird           ;       /* 红外光LED电流刻度    */
            uint8_t adc_range_ird   ;       /* 红外光ADC量程1       */
            uint8_t clk_div_ird     ;       /* 红外光ADC量程2       */
        };
        struct
        {
            uint8_t i_green         ;       /* 绿光LED电流刻度    */
            uint8_t adc_range_green	;       /* 绿光ADC量程1       */
            uint8_t clk_div_green   ;       /* 绿光ADC量程2       */
        };
    };
}si1182_register_parameter_t;
#endif


/*
 * PPG Alg result structure definitions
*/
#ifndef PWAVE_ONEMIN_INFO_T
#define PWAVE_ONEMIN_INFO_T
typedef struct pwave_onemin_info
{
    /* 睡眠相关     */
    uint8_t  one_min_result         ;   /* 每分钟睡眠状态（对应日常数据睡眠状态1~5）      */
    uint8_t  one_min_wrong_flag     ;   /* 每分钟睡眠标志（对应日常数据睡眠状态6）        */
    /* 血压相关 */
    uint8_t  bp_flag                ;   /* 血压结果有效标志位                             */
    uint8_t  high_bp_per_sec        ;   /* 每秒钟血压-高压	                              */
    uint8_t  low_bp_per_sec         ;   /* 每秒钟血压-低压	                              */
    uint8_t  high_bp_per_25sec      ;   /* 每25秒钟血压-高压	                          */
    uint8_t  low_bp_per_25sec       ;   /* 每25秒钟血压-低压                              */
    uint8_t  high_bp_per_5min       ;   /* 每五分钟血压-高压（对应日常数据血压-高压）     */
    uint8_t  low_bp_per_5min        ;   /* 每五分钟血压-低压（对应日常数据血压-低压）     */
    /* 脉率相关             */
    uint8_t  hr_ppg_per_sec_result  ;   /* 每秒HR                                         */
    uint8_t  hr_ppg_per_min_result  ;   /* 每分钟HR（对应日常数据脉率1~5）                */
    /* HRV相关        */
    uint8_t  hrv_ppg_per_sec_result ;   /* 每秒HRV                                        */
    int32_t  heart_load_1m          ;   /* 心脏负荷 define in hrv alg result              */
    /* RR间期         */
    uint8_t  rr_per_sec_result      ;   /* 每秒RR                                         */
    uint8_t  rr_per_6sec_result     ;   /* 每6秒RR                                        */
                                        
    uint8_t  wear_flag              ;   /* 佩戴标志                                       */
    /* 呼吸率        */
    uint8_t  resp_rate_1sec         ;   /* 0~50,>50--invalid value,[0,50]--valid value    */
    uint8_t  resp_rate_1min         ;   /* 0~50,>50--invalid value,[0,50]--valid value    */
    uint8_t  resp_rate_5min         ;   /* 0~50,>50--invalid value,[0,50]--valid value    */
    /* 错误码,保留位 */
    uint8_t  ppg_err1               ;
    uint32_t ppg_err2               ;
    uint32_t ppg_err3               ;
    uint32_t ppg_err4               ;
    uint32_t ppg_err5               ;
}pwave_onemin_info_t;
#endif


/*
* function declarations for external call
*/
extern void                 Pwave_InfoInit      ( void                                          );
extern uint16_t             Pwave_InfoAlgorithm ( const int32_t* green_data                  ,
                                                  const int32_t* ird_data                    ,  
                                                  const int32_t* amb_data                    ,  /* NULL in Si1182 type */
                                                  uint16_t       time_point                  ,
                                                  uint32_t       times_cnt                   ,
                                                  move_index_t*  movement_struct_s           ,
                                                  test_mode_t*   vbd_mode_info               ,
                                                  void*          sensor_register_parameter      /* 当传感器类型为Si1182时,此参数类型为 si1182_register_parameter_t* */
                                                );
extern pwave_onemin_info_t* Get_PwaveInfo       ( void                                          );
extern uint32_t             Get_PwaveVersion    ( void                                          );

#endif // !__PWAVE_INFOALGORITHM_H__
