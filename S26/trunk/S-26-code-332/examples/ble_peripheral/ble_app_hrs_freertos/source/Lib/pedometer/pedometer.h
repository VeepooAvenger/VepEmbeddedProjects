/****************************************Copyright (c)****************************************************
**                       		 	     Veepoo Tech
**                          	    COPYRIGHT (C) 2015-2019
**--------------File Info---------------------------------------------------------------------------------
** File name:               VPH_pedometer_02010101_20160711.c
** Last modified Date:      2016-07-11
** Last Version:            02.01.01
** Descriptions:            Pedometer for VPH_N project
**--------------------------------------------------------------------------------------------------------
** Modified by:             HYR VERSION 02010801  //02010900
** Modified date:           20170712
						1. ACC_UP_TREND_THRESHOLD change to 11
						2. valid 1 limit number change to 8
**--------------------------------------------------------------------------------------------------------
** Modified by:             HYR VERSION 020200.00
** Modified date:           2017-08-07
** Descriptions:
						1. Modefied  the input personal information funtion
						2. Complete the output distance and cal
**--------------------------------------------------------------------------------------------------------
** Modified by:             HYR VERSION 020201.00
** Modified date:           2017-08-15
** Descriptions:            1. Add sport info setting
**--------------------------------------------------------------------------------------------------------
** Modified by:             HYR VERSION 020202.00
** Modified date:           2017-08-17
** Descriptions:            1.Modified the setting data
                            2.Add pointer check
**--------------------------------------------------------------------------------------------------------
** Modified by:             HYR VERSION 020203.00
** Modified date:           2017-08-25
** Descriptions:            1. Modefied the movement of 60s 30s
**--------------------------------------------------------------------------------------------------------
** Modified by:             HYR VERSION 020203.01 020203.02 020203.03 020203.08
** Modified date:           2017-09-26     20171106    20171115  20171227
** Descriptions:            1. Arrange the code for momentum
2. Modified the variable name
3. Modified for less momentum steps
4. Modified the run distance gain for distance testing
5. Change api of get/clean step and get/clean momentum
**--------------------------------------------------------------------------------------------------------
**--------------------------------------------------------------------------------------------------------
** Modified by:             HYR VERSION 02030000
** Modified date:           2017-12-27
** Descriptions:            1. Delete the horizon step for too much steps
**--------------------------------------------------------------------------------------------------------
** Modified by:             HYR VERSION 02030006 // 02030100
** Modified date:           2018-01-09
** Descriptions:            1. modified for less steps, up cnt from 6 chang to 5,down cnt change from 4 to 5.
2. all step add horizon check
**--------------------------------------------------------------------------------------------------------
** Modified by:             HYR VERSION    02030200
** Modified date:           2018-01-17
** Descriptions:            1. Add move 120s
                            2. up cnt change from 12 to 11
**--------------------------------------------------------------------------------------------------------
** Modified by:             HYR VERSION    02030300
** Modified date:           2018-01-25
** Descriptions:
1.  ACC_VALID_CHECK_LEN change from 9 to 8
**--------------------------------------------------------------------------------------------------------
** Modified by:             HYR VERSION    02030500
** Modified date:           2018-01-25
** Descriptions:
1. Add start count step num parameter setting.Default:10
2. Add using hand setting                     Default:2
**--------------------------------------------------------------------------------------------------------
** Modified by:             HYR VERSION    02030514
** Modified date:           2018-03-08
** Descriptions:
1. ACC_WEAKHAND_THRESHOLD change from 11 to 14, for step control
2. ACC_VALID_CHECK_LEN change from 8 to 25, for steps missing
3. ACC_Z_GESTURE_THRESHOLD change from 700 to 750, add the count step angle
**--------------------------------------------------------------------------------------------------------
** Modified by:             HYR VERSION    02030515
** Modified date:           2018-03-13
** Descriptions:
1. Change the slow walk to 1-5
2. Change the slow run to 11-18
3. Change the fast run to 19-n
**--------------------------------------------------------------------------------------------------------
** Modified by:             HYR VERSION    02030600
** Modified date:           2018-08-15
** Descriptions:
1. Official version of 02030517
**--------------------------------------------------------------------------------------------------------
** Modified by:             HYR VERSION    02030800
** Modified date:           2018-08-16
** Descriptions:
1. Fixing the shake flag bug; ->02030300
**--------------------------------------------------------------------------------------------------------
** Modified by:             YQW VERSION    02030900
** Modified date:           2018-11-22
** Descriptions:
1. try to Fix the shake flag bug; add EUC_DISTANCE_THRESHOLD
**--------------------------------------------------------------------------------------------------------
** Modified by:             YQW VERSION    02030901
** Modified date:           2018-12-04
** Descriptions:
1. just want increase the version number 
**--------------------------------------------------------------------------------------------------------
** Modified by:             YQW VERSION    02030902
** Modified date:           2018-12-06
** Descriptions:
1. just want increase the version number 
**--------------------------------------------------------------------------------------------------------
** Modified by:             YQW VERSION    02030903
** Modified date:           2019-03-17
** Descriptions:
1. 定义运动模式宏,搜索"sport_mode typedef definitions"即可;
2. 适配多运动模式,在Pedometer_Process新增一个代表当前运动模式及运动模式暂停状态输入参数;
3. 新增的参数:sport_mode_para,未开启多运动模式时默认给0x00;在具体多运动模式下,一共8位,bit0~bit5存储某项具体运动模式对应的宏,bit6~bit7给暂停状态,0x00为非暂停,0x01为暂停,0x02和0x03暂未定义.
4. 若无配套多运动模式逻辑,sport_mode_para默认给0x00
**--------------------------------------------------------------------------------------------------------
** Modified by:             YQW VERSION    02030904
** Modified date:           2019-05-07
** Descriptions:
1.此版本号废弃,不得用于发布,非算法工程师的其他工程师见计步算法此版本号请果断忽略.
**--------------------------------------------------------------------------------------------------------
** Modified by:             YQW VERSION    02030905
** Modified date:           2019-07-04
** Descriptions:
1.本版本由02030903版本的源代码修改而来;
2.优化跑步情况时计步漏计情况(增加运动量较大时的步数补偿);
3.优化步数与里程过于不匹配的情况.    
**--------------------------------------------------------------------------------------------------------
** Modified by:             YQW VERSION    02031000
** Modified date:           2019-07-04
** Descriptions:
1.本版本由02030905_20190704版本的源代码修改版本号而来,用于正式发布;
2.首个兼容多运动模式的计步算法正式版本;
3.优化跑步情况时的路程补偿.
**--------------------------------------------------------------------------------------------------------
** Modified by:             YQW VERSION    02031100
** Modified date:           2019-07-16
** Descriptions:
1.本版本由02030905_20190704版本的源代码修改版本号而来,用于正式发布;
2.首个兼容多运动模式的计步算法正式版本;
3.优化跑步情况时的路程补偿.
4.前一版本升级和源代码不一样,在此版本修正.
*********************************************************************************************************/
#ifndef PEDOMETER_H__
#define PEDOMETER_H__

#include <stdbool.h>
#include <stdint.h>
/*
* Memory Usage(0x02031000)
* Global: 211(Bytes)
* Malloc: 0(Bytes)
* Local:  71(Bytes)
* Const:  0(Bytes)
*/



/*
* macros definitions
*/
/* Alg File info */
#define ACC_PEDOMETER_ALG_VERSION   (   0x02031100  )
#define ACC_PEDOMETER_ALG_DATE      (   "20190716"  )
#define ACC_PEDOMETER_ALG_MODIFIER  (   "YQW"       )
/* data in info*/   
#define ACC_SAMPLE_TIME_GAP         (   20          )   /* uint:ms  */
#define ACC_SAMPLE_FREQUENCE        (   50          )   /* uint:Hz  */
/* sampling data type */
#define ACC_SET_2G_12BIT            (   2           )
#define ACC_SET_4G_12BIT            (   4           )
#define ACC_SET_8G_12BIT            (   8           )
#define ACC_SET_2G_16BIT            (   32          )
#define ACC_SET_4G_16BIT            (   64          )
#define ACC_SET_8G_16BIT            (   128         )

/* Add hand chosing.HYR.20180208 */
#define ACC_DOMINANT_HAND_FALG      (   0x01        )
#define ACC_WEAK_HAND_FLAG          (   0x02        )


/* sport_mode typedef definitions 
*  MSM means " multi sports mode "
*   YQW.20190318
*/
#define MSM_MODE_NULL                   (   0x00                    )/* 未主动进入运动模式 */
#define MSM_MODE_RUN_OUTDOOR            (   0x01                    )/* 户外跑步           */
#define MSM_MODE_WALK_OUTDOOR           (   0x02                    )/* 户外步行           */
#define MSM_MODE_BIKING_OUTDOOR         (   0x03                    )/* 户外骑行           */
#define MSM_MODE_BIKING_INDOOR          (   0x04                    )/* 室内骑行           */
#define MSM_MODE_WALK_INDOOR            (   0x05                    )/* 室内步行           */
#define MSM_MODE_RUN_INDOOR             (   0x06                    )/* 室内跑步           */
#define MSM_MODE_ELLIPTICAL_MACHINE     (   0x07                    )/* 椭圆机             */
#define MSM_MODE_ROWING_MACHINE         (   0x08                    )/* 划船机             */
#define MSM_MODE_TREADMILLS_MACHINE     (   0x09                    )/* 踏步机             */
#define MSM_MODE_HIKING                 (   0x0A                    )/* 徒步               */
#define MSM_MODE_VALID_MIN              (   MSM_MODE_RUN_OUTDOOR    )
#define MSM_MODE_VALID_MAX              (   MSM_MODE_HIKING         )

#define FLAG_NOPAUSE_OFF                (   0x00                    ) /*运动模式运行中或未开启运动模式*/
#define FLAG_PAUSE_ON                   (   0x01                    ) /*运动模式暂停中*/



/*
* 计步参数结构体
*/
typedef struct tag_ped_param_set
{
    /*Start count steps num(default 10) */
    uint8_t start_num;
    /*  
        [1]:dominant hand.ACC_DOMINANT_HAND_FALG
        [2]:weak hand ;
        Default weak hand.
        please see macros "Add hand chosing.HYR.20180208"
    */
    uint8_t hand_chose;
}ped_param_set_t;

/*
* 运动量等级
*/
typedef struct tag_move_index
{
    uint8_t     move_5s     ;
    uint8_t     move_15s    ;
    uint8_t     move_30s    ;
    uint8_t     move_60s    ;
    uint8_t     move_120s   ;
    uint32_t    move_a5s    ;
}move_index_t;

/*
* steps at different sport_mode 
*/
typedef struct tag_walk_station_info
{
    uint32_t    walk_slow                   ;   /* move_15s:1~5     */ 
    uint32_t    walk_fast                   ;   /* move_15s:6~10    */
    uint32_t    run_slow                    ;   /* move_15s:11~15   */
    uint32_t    run_fast                    ;   /* move_15s:16~     */
    uint32_t    steps_total                 ;
    #if 0 
    uint32_t  steps_run_outdoor             ;
    uint32_t  steps_walk_outdoor            ;
    uint32_t  steps_bike_outdoor            ;
    uint32_t  steps_bike_indoor             ;
    uint32_t  steps_walk_indoor             ;
    uint32_t  steps_run_indoor              ;
    uint32_t  steps_elliptical_machine      ;
    uint32_t  steps_rowing_machine          ;
    uint32_t  steps_treadmills_machine      ;
    uint32_t  steps_hike                    ;
    #endif     
}walk_station_info_t;

/*
** all_steps
** all_cal
** all_distance
*/
typedef struct tag_sport_info
{
    // posi_steps = steps_total = AllStepGet
    uint32_t posi_steps     ;
    uint32_t posi_cal       ;
    uint32_t posi_distance  ;
}sport_info_t;



/* make interface name unchanged, make everyone happy */
#define     Get_SportInformationInfo    Get_SportInfo
#define     sport_information_info_t    sport_info_t


/*
* functions declarations for external call 
*/
extern uint8_t  Ped_SetSportsInfo               (   uint32_t                    pss_set_steps       , 
                                                    uint32_t                    pss_set_cal         ,
                                                    uint32_t                    pss_set_distance
                                                );                              
extern void     Ped_SetInputPersonalInformation (   uint16_t                    psipi_height        ,
                                                    float                       psipi_weight        ,
                                                    uint8_t                     psipi_gender        ,
                                                    uint16_t                    psipi_age
                                                );
                                                
/*
    sport_mode_para 未开启多运动模式时默认给0x00;
    在具体多运动模式下,一共8位,
        bit0~bit5存储运动模式,
        bit6~bit7给暂停状态,
        0x00为非暂停,0x01为暂停,0x02和0x03暂未定义         
*/
extern void     Pedometer_Process               (   int16_t*                    pp_data_input       ,
                                                    int16_t                     pp_len              ,
                                                    uint8_t                     get_sensor_set      ,  /* see macros "sampling data type" */
                                                    ped_param_set_t             pp_param_s          ,
                                                    uint8_t                     sport_mode_para 
                                                );
extern uint32_t All_StepGet                     (   void                                                );
extern uint32_t Get_StepsInfo                   (   walk_station_info_t*        asg_steps_s             );
extern void     Get_SportInfo                   (   sport_information_info_t*   gsii_sport_info_s       );
extern void     All_StepCln                     (   void                                                );
extern void     All_StepSet                     (   uint32_t                    step_input              );
extern uint32_t Work_MeasureGet                 (   void                                                );
extern void     Work_MeasrueCln                 (   void                                                );
extern void     Move_IdexGet                    (   move_index_t*               mig_move_index_s        );
extern uint32_t Vph_GetPedometerVersion         (   void                                                );
extern uint8_t  Vph_GetShakeFlag                (   void                                                );

#endif
