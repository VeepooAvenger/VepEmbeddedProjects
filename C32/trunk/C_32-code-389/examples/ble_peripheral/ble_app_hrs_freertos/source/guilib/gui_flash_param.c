/***************************Copyright(c)***********************************
**                       Veepoo Tech
**                  COPYRIGHT (C) 2016-2019
**-----------File Info-----------------------------------------------------
** File        name:     gui_flash_param.h
** Last modified Date:   2017-08-09 
** Last SYS_VERSION:     0.0.0
** Descriptions:            
**-------------------------------------------------------------------------
** Created  by:          YX
** Created  date:        2017-08-09
** SYS_VERSION:          0.0.0
** Descriptions:         
**-------------------------------------------------------------------------
** Modified by:				
** Modified date:  			          
** SYS_VERSION:                 
** Descriptions:                        
***************************************************************************/

#include <stdint.h>
#include "gui_flash_addr.h"



/* 主界面1 */

const uint32_t icon_16_main_minute1_addr[1][10] = //时间
{
    {
        ICON_MAIN_DATA_TIME_0_ADDR, ICON_MAIN_DATA_TIME_1_ADDR, ICON_MAIN_DATA_TIME_2_ADDR, ICON_MAIN_DATA_TIME_3_ADDR, ICON_MAIN_DATA_TIME_4_ADDR,
        ICON_MAIN_DATA_TIME_5_ADDR, ICON_MAIN_DATA_TIME_6_ADDR, ICON_MAIN_DATA_TIME_7_ADDR, ICON_MAIN_DATA_TIME_8_ADDR, ICON_MAIN_DATA_TIME_9_ADDR
    }
};
const uint8_t icon_16_main_time1_coord[4][2] = 
{
   {65, 23}, {87, 23}, {131, 23}, {153, 23}

};



const uint32_t icon_16_main_mh_addr[1] = //：
{
   
        ICON_MAIN_DATA_TIMEDOT_ADDR, 
        
   
};
const uint8_t icon_16_main_mh_coord[1][2] = 
{
   {114, 23}

};




const uint32_t icon_16_main_bj_addr[5] = //背景
{
   
        ICON_MAIN_ICON1_1_ADDR,ICON_MAIN_ICON1_2_ADDR,ICON_MAIN_ICON1_3_ADDR,ICON_MAIN_ICON1_4_ADDR,ICON_MAIN_ICON1_5_ADDR, 
        
   
};
const uint8_t icon_16_main_bj_coord[1][2] = 
{
   {42, 59}

};




/* 主界面2 */

const uint32_t icon_16_main2_minute1_addr[2][10] = //时间、月
{
    {
        ICON_TIME_DATA3_TIME_0_ADDR, ICON_TIME_DATA3_TIME_1_ADDR, ICON_TIME_DATA3_TIME_2_ADDR, ICON_TIME_DATA3_TIME_3_ADDR, ICON_TIME_DATA3_TIME_4_ADDR,
        ICON_TIME_DATA3_TIME_5_ADDR, ICON_TIME_DATA3_TIME_6_ADDR, ICON_TIME_DATA3_TIME_7_ADDR, ICON_TIME_DATA3_TIME_8_ADDR, ICON_TIME_DATA3_TIME_9_ADDR
    },
    {
    
        ICON_TIME_DATA1_DATE_0_ADDR, ICON_TIME_DATA1_DATE_1_ADDR, ICON_TIME_DATA1_DATE_2_ADDR, ICON_TIME_DATA1_DATE_3_ADDR, ICON_TIME_DATA1_DATE_4_ADDR,
        ICON_TIME_DATA1_DATE_5_ADDR, ICON_TIME_DATA1_DATE_6_ADDR, ICON_TIME_DATA1_DATE_7_ADDR, ICON_TIME_DATA1_DATE_8_ADDR, ICON_TIME_DATA1_DATE_9_ADDR
    },
};
const uint8_t icon_16_main2_time1_coord[6][2] = 
{
   {19, 91}, {72, 91}, {125, 91}, {178, 91}, {38, 44}, {62, 44}

};



const uint32_t icon_16_main2_mh_addr[1] = //.
{  
        ICON_TIME_ICON3_BATTERY_ADDR,   
};
const uint8_t icon_16_main2_mh_coord[4][2] = 
{
   {50, 180}, {90, 180}, {130, 180}, {170, 180}, 

};




const uint32_t icon_16_main2_ble_addr[1] = //蓝牙
{
   
        ICON_TIME_ICON1_DISCONNECT_ADDR, 
        
   
};
const uint8_t icon_16_main2_ble_coord[1][2] = 
{
   {104, 14}

};



const uint32_t icon_16_main2_sun_addr[2] = //天气
{
   
        ICON_TIME_ICON2_MOON_ADDR,ICON_TIME_ICON2_SUN_ADDR, 
        
   
};
const uint8_t icon_16_main2_sun_coord[1][2] = 
{
   {172, 44}

};



const uint32_t icon_16_main2_week_addr[2][7] = //星期
{
   {
        ICON_TIME_DATA2_WEEK_CN_1_ADDR,ICON_TIME_DATA2_WEEK_CN_2_ADDR,ICON_TIME_DATA2_WEEK_CN_3_ADDR,ICON_TIME_DATA2_WEEK_CN_4_ADDR,
        ICON_TIME_DATA2_WEEK_CN_5_ADDR,ICON_TIME_DATA2_WEEK_CN_6_ADDR,ICON_TIME_DATA2_WEEK_CN_7_ADDR, 
   },   
   {
        ICON_TIME_DATA2_WEEK_EN_1_ADDR,ICON_TIME_DATA2_WEEK_EN_2_ADDR,ICON_TIME_DATA2_WEEK_EN_3_ADDR,ICON_TIME_DATA2_WEEK_EN_4_ADDR,
        ICON_TIME_DATA2_WEEK_EN_5_ADDR,ICON_TIME_DATA2_WEEK_EN_6_ADDR,ICON_TIME_DATA2_WEEK_EN_7_ADDR, 
   },   
   
};
const uint8_t icon_16_main2_week_coord[1][2] = 
{
   {95, 44}

};




/* 运动界面1 */

const uint32_t icon_16_sport_jm1_addr[10] = //人
{

        ICON_MOVE1_ICON3_1_ADDR, ICON_MOVE1_ICON3_2_ADDR, ICON_MOVE1_ICON3_3_ADDR, ICON_MOVE1_ICON3_4_ADDR, ICON_MOVE1_ICON3_5_ADDR,
        ICON_MOVE1_ICON3_6_ADDR, ICON_MOVE1_ICON3_7_ADDR, ICON_MOVE1_ICON3_8_ADDR, ICON_MOVE1_ICON3_9_ADDR, ICON_MOVE1_ICON3_10_ADDR

};
const uint32_t icon_16_sport1_jm1_addr[4] = //箭头
{

        ICON_MOVE1_ICON2_1_ADDR, ICON_MOVE1_ICON2_2_ADDR, ICON_MOVE1_ICON2_3_ADDR, ICON_MOVE1_ICON2_4_ADDR,
        
};
const uint32_t icon_16_sport3_jm1_addr[1] = //彩虹
{

        ICON_MOVE1_ICON1_ADDR,
         
};
const uint8_t icon_16_sport_jm1_coord[3][2] = 
{
   {0, 0}, {82, 52}, {33, 132},

};


/* 运动界面2 */

const uint32_t icon_16_sport_jm2_addr[1][11] = //彩虹
{
    {
        ICON_MOVE2_ICON1_1_ADDR, ICON_MOVE2_ICON1_2_ADDR, ICON_MOVE2_ICON1_3_ADDR, ICON_MOVE2_ICON1_4_ADDR,
        ICON_MOVE2_ICON1_5_ADDR, ICON_MOVE2_ICON1_6_ADDR, ICON_MOVE2_ICON1_7_ADDR, ICON_MOVE2_ICON1_8_ADDR,
        ICON_MOVE2_ICON1_9_ADDR, ICON_MOVE2_ICON1_10_ADDR, ICON_MOVE2_ICON1_11_ADDR
    }
};
const uint32_t icon_16_sport1_jm2_addr[1] = //心率
{

        ICON_MOVE2_ICON2_ADDR,
        
};
const uint32_t icon_16_sport3_jm2_addr[1] = //卡路里
{
    
        ICON_MOVE2_ICON3_ADDR,
         
};
const uint32_t icon_16_sport4_jm2_addr[1] = //计步
{

        ICON_MOVE2_ICON4_ADDR,
         
};
const uint8_t icon_16_sport_jm2_coord[4][2] = 
{
   {6, 5}, {10, 117}, {115, 117}, {42, 157}

};



const uint32_t icon_16_step_jm2_addr[10] = //数字
{

        ICON_MOVECALORIES_DATA_TIME_0_ADDR, ICON_MOVECALORIES_DATA_TIME_1_ADDR, ICON_MOVECALORIES_DATA_TIME_2_ADDR, ICON_MOVECALORIES_DATA_TIME_3_ADDR,
        ICON_MOVECALORIES_DATA_TIME_4_ADDR, ICON_MOVECALORIES_DATA_TIME_5_ADDR, ICON_MOVECALORIES_DATA_TIME_6_ADDR, ICON_MOVECALORIES_DATA_TIME_7_ADDR,
        ICON_MOVECALORIES_DATA_TIME_8_ADDR, ICON_MOVECALORIES_DATA_TIME_9_ADDR

};
const uint8_t icon_16_step_jm2_coord[6][2] = 
{
   {81, 161}, {100, 161}, {119, 161}, {138, 161}, {157, 161}, {176, 161}

};

const uint8_t icon_16_calories_jm2_coord[4][2] = 
{
   {153, 120}, {172, 120}, {191, 120}, {210, 120}

};

const uint8_t icon_16_time_jm2_coord[4][2] = 
{
   {74, 80}, {93, 80}, {129, 80}, {148, 80}

};

const uint8_t icon_16_heart_jm2_coord[3][2] = 
{
   {48, 120}, {67, 120}, {86, 120}

};




const uint32_t icon_16_mh_jm2_addr[1] = //：
{
    
      ICON_MOVE_DATA_TIME_DOT_ADDR,

};
const uint8_t icon_16_mh_jm2_coord[1][2] = 
{
   {113, 80}

};


const uint32_t icon_16_mh1_jm2_addr[1] = //继续
{
    
      ICON_MOVE3_ICON1_1_ADDR,

};
const uint32_t icon_16_mh2_jm2_addr[1] = //暂停
{
    
      ICON_MOVE3_ICON1_2_ADDR,

};
const uint32_t icon_16_mh3_jm2_addr[1] = //退出
{
    
      ICON_MOVE4_ICON1_ADDR,

};
const uint8_t icon_16_mh3_jm2_coord[1][2] = 
{
   {69, 68}

};


/* 计步界面 */
const uint32_t icon_16_step_jb_addr[10] = //数字
{

        ICON_STEP_DATA_TIME_0_ADDR, ICON_STEP_DATA_TIME_1_ADDR, ICON_STEP_DATA_TIME_2_ADDR, ICON_STEP_DATA_TIME_3_ADDR,
        ICON_STEP_DATA_TIME_4_ADDR, ICON_STEP_DATA_TIME_5_ADDR, ICON_STEP_DATA_TIME_6_ADDR, ICON_STEP_DATA_TIME_7_ADDR,
        ICON_STEP_DATA_TIME_8_ADDR, ICON_STEP_DATA_TIME_9_ADDR

};
const uint8_t icon_16_step_jb_coord[5][2] = 
{
   {117, 123}, {139, 123}, {161, 123}, {183, 123}, {205, 123}

};

const uint32_t icon_16_step_jbr_addr[12] = //人
{

        ICON_STEP_ICON1_1_ADDR, ICON_STEP_ICON1_2_ADDR, ICON_STEP_ICON1_3_ADDR, ICON_STEP_ICON1_4_ADDR,
        ICON_STEP_ICON1_5_ADDR, ICON_STEP_ICON1_6_ADDR, ICON_STEP_ICON1_7_ADDR, ICON_STEP_ICON1_8_ADDR,
        ICON_STEP_ICON1_9_ADDR, ICON_STEP_ICON1_10_ADDR, ICON_STEP_ICON1_11_ADDR, ICON_STEP_ICON1_12_ADDR

};
const uint8_t icon_16_step_jbr_coord[1][2] = 
{
   {51, 29}

};


const uint32_t icon_16_mh3_jb_addr[1] = //STEP
{
    
      ICON_STEP_ICON2_ADDR

};
const uint8_t icon_16_mh3_jb_coord[1][2] = 
{
   {132, 84}

};




/* 卡路里界面 */
const uint32_t icon_16_cal_jb_addr[10] = //数字
{

        ICON_STEP_DATA_TIME_0_ADDR, ICON_STEP_DATA_TIME_1_ADDR, ICON_STEP_DATA_TIME_2_ADDR, ICON_STEP_DATA_TIME_3_ADDR,
        ICON_STEP_DATA_TIME_4_ADDR, ICON_STEP_DATA_TIME_5_ADDR, ICON_STEP_DATA_TIME_6_ADDR, ICON_STEP_DATA_TIME_7_ADDR,
        ICON_STEP_DATA_TIME_8_ADDR, ICON_STEP_DATA_TIME_9_ADDR

};
const uint8_t icon_16_cal_jb_coord[4][2] = 
{
   {128, 123}, {150, 123}, {172, 123}, {194, 123}

};

const uint32_t icon_16_cal_jbr_addr[10] = //人
{

        ICON_CALORIES_ICON1_1_ADDR, ICON_CALORIES_ICON1_2_ADDR, ICON_CALORIES_ICON1_3_ADDR, ICON_CALORIES_ICON1_4_ADDR,
        ICON_CALORIES_ICON1_5_ADDR, ICON_CALORIES_ICON1_6_ADDR, ICON_CALORIES_ICON1_7_ADDR, ICON_CALORIES_ICON1_8_ADDR,
        ICON_CALORIES_ICON1_9_ADDR, ICON_CALORIES_ICON1_10_ADDR

};
const uint8_t icon_16_cal_jbr_coord[1][2] = 
{
   {53, 29}

};


const uint32_t icon_16_mh3_cal_addr[1] = 
{
    
      ICON_CALORIES_ICON2_ADDR,

};
const uint8_t icon_16_mh3_cal_coord[1][2] = 
{
   {132, 84}

};



/* 里程界面 */
const uint32_t icon_16_lc_jb_addr[10] = //数字
{

        ICON_STEP_DATA_TIME_0_ADDR, ICON_STEP_DATA_TIME_1_ADDR, ICON_STEP_DATA_TIME_2_ADDR, ICON_STEP_DATA_TIME_3_ADDR,
        ICON_STEP_DATA_TIME_4_ADDR, ICON_STEP_DATA_TIME_5_ADDR, ICON_STEP_DATA_TIME_6_ADDR, ICON_STEP_DATA_TIME_7_ADDR,
        ICON_STEP_DATA_TIME_8_ADDR, ICON_STEP_DATA_TIME_9_ADDR

};
const uint8_t icon_16_lc_jb_coord[6][2] = 
{
   {51, 164}, {74, 164}, {114, 164}, {100, 164}, {122, 164}, {144, 164}

};

const uint32_t icon_16_lc_jbr_addr[10] = //人
{

        ICON_DISTANCE_ICON1_1_ADDR, ICON_DISTANCE_ICON1_2_ADDR, ICON_DISTANCE_ICON1_3_ADDR, ICON_DISTANCE_ICON1_4_ADDR,
        ICON_DISTANCE_ICON1_5_ADDR, ICON_DISTANCE_ICON1_6_ADDR, ICON_DISTANCE_ICON1_7_ADDR, ICON_DISTANCE_ICON1_8_ADDR,
        ICON_DISTANCE_ICON1_9_ADDR, ICON_DISTANCE_ICON1_10_ADDR,

};
const uint8_t icon_16_lc_jbr_coord[1][2] = 
{
   {49, 29}

};


const uint32_t icon_16_mh3_lc_addr[1] = //图片
{
    
      ICON_DISTANCE_ICON2_ADDR,

};
const uint8_t icon_16_mh3_lc_coord[1][2] = 
{
   {118, 6}

};


const uint32_t icon_16_mh3_dw_addr[2] = //单位
{
    
     ICON_DISTANCE_UNIT_KM_ADDR,ICON_DISTANCE_UNIT_MI_ADDR,

};
const uint8_t icon_16_mh3_dw_coord[1][2] = 
{
   {143, 164}

};


const uint32_t icon_16_mh3_d_addr[1] = //.
{
    
     ICON_DISTANCE_DATA_TIME_DOT_ADDR,

};
const uint8_t icon_16_mh3_d_coord[1][2] = 
{
   {97, 164}

};


/* 心率界面 */
const uint32_t icon_16_lc_bmp_addr[10] = //数字
{

        ICON_HEART_DATA_TIME_0_ADDR, ICON_HEART_DATA_TIME_1_ADDR, ICON_HEART_DATA_TIME_2_ADDR, ICON_HEART_DATA_TIME_3_ADDR,
        ICON_HEART_DATA_TIME_4_ADDR, ICON_HEART_DATA_TIME_5_ADDR, ICON_HEART_DATA_TIME_6_ADDR, ICON_HEART_DATA_TIME_7_ADDR,
        ICON_HEART_DATA_TIME_8_ADDR, ICON_HEART_DATA_TIME_9_ADDR

};
const uint8_t icon_16_lc_bmp_coord[3][2] = 
{
   {87, 164}, {109, 164}, {131, 164}

};


const uint32_t icon_16_mh3_bmp_addr[1] = //图片
{
    
      ICON_HEART_ICON1_ADDR,

};
const uint8_t icon_16_mh3_bmp_coord[1][2] = 
{
   {86, 30}

};

const uint32_t icon_16_mh3_bmp1_addr[12] = //动图
{
    
      ICON_HEART_ICON2_1_ADDR,ICON_HEART_ICON2_2_ADDR,ICON_HEART_ICON2_3_ADDR,ICON_HEART_ICON2_4_ADDR,
      ICON_HEART_ICON2_5_ADDR,ICON_HEART_ICON2_6_ADDR,ICON_HEART_ICON2_7_ADDR,ICON_HEART_ICON2_6_ADDR,
      ICON_HEART_ICON2_5_ADDR,ICON_HEART_ICON2_4_ADDR,ICON_HEART_ICON2_3_ADDR,ICON_HEART_ICON2_2_ADDR,

};
const uint8_t icon_16_mh3_bmp1_coord[1][2] = 
{
   {70, 70}

};


/* 睡眠界面 */
const uint32_t icon_16_lc_sleep_addr[10] = //数字
{

        ICON_SLEEP_DATA_TIME_0_ADDR, ICON_SLEEP_DATA_TIME_1_ADDR, ICON_SLEEP_DATA_TIME_2_ADDR, ICON_SLEEP_DATA_TIME_3_ADDR,
        ICON_SLEEP_DATA_TIME_4_ADDR, ICON_SLEEP_DATA_TIME_5_ADDR, ICON_SLEEP_DATA_TIME_6_ADDR, ICON_SLEEP_DATA_TIME_7_ADDR,
        ICON_SLEEP_DATA_TIME_8_ADDR, ICON_SLEEP_DATA_TIME_9_ADDR

};
const uint8_t icon_16_lc_sleep_coord[3][2] = 
{
   {144, 86}, {166, 86}, {207, 86}

};


const uint32_t icon_16_mh3_sleep_addr[1] = //图片
{
    
      ICON_SLEEP_ICON1_ADDR,

};
const uint8_t icon_16_mh3_sleep_coord[1][2] = 
{
   {25, 69}

};

const uint32_t icon_16_mh3_sleep2_addr[1] = //图片
{
    
      ICON_SLEEP_UNIT_ADDR,

};
const uint8_t icon_16_mh3_sleep2_coord[1][2] = 
{
   {143, 122}

};

const uint32_t icon_16_mh3_sleep1_addr[2] = //动图
{
    
      ICON_SLEEP_ICON2_2_ADDR,ICON_SLEEP_ICON2_1_ADDR,

};
const uint8_t icon_16_mh3_sleep1_coord[1][2] = 
{
   {109, 36}

};

const uint32_t icon_16_mh3_dot_addr[1] = //.
{
    
      ICON_SLEEP_DATA_TIME_DOT_ADDR,

};
const uint8_t icon_16_mh3_dot_coord[1][2] = 
{
   {190, 86}

};



/* 秒表界面 */
const uint32_t icon_16_lc_mb_addr[2][10] = //数字
{
    {
        ICON_STOPWATCH_DATA1_TIME_0_ADDR, ICON_STOPWATCH_DATA1_TIME_1_ADDR, ICON_STOPWATCH_DATA1_TIME_2_ADDR, ICON_STOPWATCH_DATA1_TIME_3_ADDR,
        ICON_STOPWATCH_DATA1_TIME_4_ADDR, ICON_STOPWATCH_DATA1_TIME_5_ADDR, ICON_STOPWATCH_DATA1_TIME_6_ADDR, ICON_STOPWATCH_DATA1_TIME_7_ADDR,
        ICON_STOPWATCH_DATA1_TIME_8_ADDR, ICON_STOPWATCH_DATA1_TIME_9_ADDR
    },
    {
        ICON_STOPWATCH_DATA2_TIME_0_ADDR, ICON_STOPWATCH_DATA2_TIME_1_ADDR, ICON_STOPWATCH_DATA2_TIME_2_ADDR, ICON_STOPWATCH_DATA2_TIME_3_ADDR,
        ICON_STOPWATCH_DATA2_TIME_4_ADDR, ICON_STOPWATCH_DATA2_TIME_5_ADDR, ICON_STOPWATCH_DATA2_TIME_6_ADDR, ICON_STOPWATCH_DATA2_TIME_7_ADDR,
        ICON_STOPWATCH_DATA2_TIME_8_ADDR, ICON_STOPWATCH_DATA2_TIME_9_ADDR
    }
};
const uint8_t icon_16_lc_mb_coord[6][2] = 
{
   {65, 91}, {87, 91}, {131, 91}, {153, 91}, {94, 130}, {121, 130}

};


const uint32_t icon_16_mh3_mb_addr[1] = //冒号
{
    
      ICON_STOPWATCH_DATA1_TIME_DOT_ADDR,

};
const uint8_t icon_16_mh3_mb_coord[1][2] = 
{
   {114, 91}

};

const uint32_t icon_16_mh3_mb1_addr[4] = //暂停等
{
    
      ICON_STOPWATCH_ICON2_1_ADDR,ICON_STOPWATCH_ICON2_2_ADDR,ICON_STOPWATCH_ICON3_1_ADDR,ICON_STOPWATCH_ICON3_2_ADDR,

};
const uint8_t icon_16_mh3_mb1_coord[2][2] = 
{
   {33, 35}, {156, 35}

};

const uint32_t icon_16_mh3_mb2_addr[1] = //图
{
    
      ICON_STOPWATCH_ICON1_ADDR

};
const uint8_t icon_16_mh3_mb2_coord[1][2] = 
{
   {29, 43}

};



/* 关机界面 */

const uint32_t icon_16_mh3_poweroff_addr[1] = //图
{
    
      ICON_POWEROFF_ICON1_ADDR

};
const uint8_t icon_16_mh3_poweroff_coord[1][2] = 
{
   {69, 68}

};


/* 充电界面 */

const uint32_t icon_16_mh3_chg_addr[5] = //图
{
    
      ICON_CHG_ICON1_1_ADDR,ICON_CHG_ICON1_2_ADDR,ICON_CHG_ICON1_3_ADDR,ICON_CHG_ICON1_4_ADDR,ICON_CHG_ICON1_5_ADDR,

};
const uint8_t icon_16_mh3_chg_coord[1][2] = 
{
   {94, 71}

};

const uint32_t icon_16_mh3_chg1_addr[2] = //图
{
    
      ICON_CHG_ICON1_5_ADDR,ICON_CHG_ICON1_6_ADDR,

};
const uint8_t icon_16_mh3_chg1_coord[1][2] = 
{
   {94, 71}

};



/* 蓝牙断链界面 */

const uint32_t icon_16_mh3_ble_addr[2] = //图
{
    
      ICON_DISCONNECTED_ICON1_1_ADDR,ICON_DISCONNECTED_ICON1_2_ADDR,

};
const uint8_t icon_16_mh3_ble_coord[1][2] = 
{
   {84, 46}

};



/* 久坐界面 */

const uint32_t icon_16_mh3_sit_addr[1] = //图
{
    
      ICON_SIT_ICON1_ADDR,

};
const uint8_t icon_16_mh3_sit_coord[1][2] = 
{
   {34, 90}

};

const uint32_t icon_16_mh3_sit1_addr[2] = //图
{
    
      ICON_SIT_ICON2_1_ADDR,ICON_SIT_ICON2_2_ADDR,

};
const uint8_t icon_16_mh3_sit1_coord[1][2] = 
{
   {101, 45}

};



/* 运动达标界面 */

const uint32_t icon_16_mh3_S_addr[3] = //图
{
    
      ICON_STANDARD_ICON1_1_ADDR,ICON_STANDARD_ICON1_2_ADDR,ICON_STANDARD_ICON1_3_ADDR,

};
const uint8_t icon_16_mh3_S_coord[1][2] = 
{
   {49, 36}

};


/* 闹钟界面 */

const uint32_t icon_16_mh3_clock_addr[10] = //图
{
    
      ICON_CLOCK_DATA_TIME_0_ADDR,ICON_CLOCK_DATA_TIME_1_ADDR,ICON_CLOCK_DATA_TIME_2_ADDR,
      ICON_CLOCK_DATA_TIME_3_ADDR,ICON_CLOCK_DATA_TIME_4_ADDR,ICON_CLOCK_DATA_TIME_5_ADDR,
      ICON_CLOCK_DATA_TIME_6_ADDR,ICON_CLOCK_DATA_TIME_7_ADDR,ICON_CLOCK_DATA_TIME_8_ADDR,
      ICON_CLOCK_DATA_TIME_9_ADDR,

};
const uint8_t icon_16_mh3_clock_coord[4][2] = 
{
   {66, 93},{88, 93},{130, 93},{152, 93},

};


const uint32_t icon_16_mh3_clock1_addr[1] = //:
{
    
      ICON_CLOCK_DATA_TIME_DOT_ADDR,

};
const uint8_t icon_16_mh3_clock1_coord[1][2] = 
{
   {114, 93}

};


const uint32_t icon_16_mh3_clock2_addr[2] = //太阳
{
    
      ICON_CLOCK_ICON1_1_ADDR,ICON_CLOCK_ICON1_2_ADDR,

};
const uint8_t icon_16_mh3_clock2_coord[1][2] = 
{
   {100, 133}

};


const uint32_t icon_16_mh3_clock3_addr[1] = //图
{
    
      ICON_CLOCK_ICON2_ADDR,

};
const uint8_t icon_16_mh3_clock3_coord[1][2] = 
{
   {38, 40}

};


//const uint32_t icon_16_mh3_clock4_addr[2] = //图
//{
//    
//      ICON_CLOCK_ICON3_1_ADDR,ICON_CLOCK_ICON3_2_ADDR,

//};
const uint8_t icon_16_mh3_clock4_coord[1][2] = 
{
   {38, 40}

};


const uint32_t icon_16_mh3_clock5_addr[2] = //图左
{
    
      ICON_CLOCK_ICON4_1_ADDR,ICON_CLOCK_ICON4_2_ADDR,

};
const uint8_t icon_16_mh3_clock5_coord[1][2] = 
{
   {38, 40}

};


const uint32_t icon_16_mh3_clock6_addr[2] = //图右
{
    
      ICON_CLOCK_ICON5_1_ADDR,ICON_CLOCK_ICON5_2_ADDR,

};
const uint8_t icon_16_mh3_clock6_coord[1][2] = 
{
   {150, 40}

};


//const uint32_t icon_16_mh3_clock7_addr[1] = //图中
//{
//    
//      ICON_CLOCK_ICON6_1_ADDR,

//};
//const uint8_t icon_16_mh3_clock7_coord[1][2] = 
//{
//   {89, 40}

//};


/* 来电界面 */

const uint32_t icon_16_mh3_call_addr[3] = //图
{
    
      ICON_CALL_ICON2_3_ADDR,ICON_CALL_ICON2_2_ADDR,ICON_CALL_ICON2_4_ADDR,

};

const uint32_t icon_16_mh3_call1_addr[2] = //图
{
    
      ICON_CALL_ICON1_ADDR,ICON_CALL_ICON3_ADDR,

};
const uint8_t icon_16_mh3_call_coord[1][2] = 
{
   {80,13}

};


const uint32_t icon_16_mh3_call2_addr[1] = //图
{
    
      ICON_CALL_ICON4_ADDR,

};
const uint8_t icon_16_mh3_call2_coord[1][2] = 
{
   {26, 105}

};

const uint8_t icon_16_call_text_coord[1][2] = {{24, 110}};//105


/* 拍照界面 */

const uint32_t icon_16_mh3_cam_addr[2] = //图
{
    
      ICON_CAM_ICON_2_ADDR,ICON_CAM_ICON_1_ADDR,

};
const uint8_t icon_16_mh3_cam_coord[1][2] = 
{
   {47, 69}

};


/* 闹钟推送 */

const uint32_t icon_16_remindclock_icon_addr[20] = //图
{
    
      ICON_REMINDCLOCK_ICON_2_ADDR,ICON_REMINDCLOCK_ICON_3_ADDR,ICON_REMINDCLOCK_ICON_4_ADDR,ICON_REMINDCLOCK_ICON_5_ADDR,ICON_REMINDCLOCK_ICON_6_ADDR,
      ICON_REMINDCLOCK_ICON_7_ADDR,ICON_REMINDCLOCK_ICON_8_ADDR,ICON_REMINDCLOCK_ICON_9_ADDR,ICON_REMINDCLOCK_ICON_10_ADDR,ICON_REMINDCLOCK_ICON_11_ADDR,
      ICON_REMINDCLOCK_ICON_12_ADDR,ICON_REMINDCLOCK_ICON_13_ADDR,ICON_REMINDCLOCK_ICON_14_ADDR,ICON_REMINDCLOCK_ICON_15_ADDR,ICON_REMINDCLOCK_ICON_16_ADDR,
      ICON_REMINDCLOCK_ICON_17_ADDR,ICON_REMINDCLOCK_ICON_18_ADDR,ICON_REMINDCLOCK_ICON_19_ADDR,ICON_REMINDCLOCK_ICON_20_ADDR,ICON_REMINDCLOCK_ICON_21_ADDR,

};
const uint8_t icon_16_remindclock_icon_coord[1][2] = 
{
   {63, 62}

};


/* 消息推送 */

const uint32_t icon_16_inform_icon_addr[20] = //图
{
    
      ICON_INFORM_ICON1_1_ADDR,ICON_INFORM_ICON1_2_ADDR,ICON_INFORM_ICON1_3_ADDR,ICON_INFORM_ICON1_4_ADDR,ICON_INFORM_ICON1_5_ADDR,
      ICON_INFORM_ICON1_6_ADDR,ICON_INFORM_ICON1_13_ADDR,ICON_INFORM_ICON1_7_ADDR,ICON_INFORM_ICON1_8_ADDR,ICON_INFORM_ICON1_9_ADDR,
      ICON_INFORM_ICON1_10_ADDR,ICON_INFORM_ICON1_11_ADDR,ICON_INFORM_ICON1_12_ADDR,ICON_INFORM_ICON1_14_ADDR,
      

};
const uint8_t icon_16_inform_icon_coord[1][2] = 
{
   {84, 20}

};


/* 开关机动画 */

const uint32_t icon_16_inform_poweron_addr[2][10] = //图
{
    {
      ICON_POWERON_ICON1_1_ADDR,ICON_POWERON_ICON1_2_ADDR,ICON_POWERON_ICON1_3_ADDR,
      ICON_POWERON_ICON1_4_ADDR,ICON_POWERON_ICON1_5_ADDR,ICON_POWERON_ICON1_6_ADDR,
      ICON_POWERON_ICON1_7_ADDR,ICON_POWERON_ICON1_8_ADDR,ICON_POWERON_ICON1_9_ADDR,
      ICON_POWERON_ICON1_9_ADDR,
    },
    {    
      ICON_POWEROFF_ICON2_1_ADDR,ICON_POWEROFF_ICON2_2_ADDR,ICON_POWEROFF_ICON2_3_ADDR,
      ICON_POWEROFF_ICON2_4_ADDR,ICON_POWEROFF_ICON2_5_ADDR,ICON_POWEROFF_ICON2_6_ADDR,
      ICON_POWEROFF_ICON2_7_ADDR,ICON_POWEROFF_ICON2_8_ADDR,ICON_POWEROFF_ICON2_9_ADDR,
      ICON_POWEROFF_ICON2_10_ADDR,
    },
};
const uint8_t icon_16_inform_poweron_coord[1][2] = 
{
   {42, 54}

};


/* 女性生理期动画 */

const uint32_t icon_16_inform_lady1_addr[10] = //月经期
{
    
      ICON_LADY_ICON1_01_ADDR,ICON_LADY_ICON1_02_ADDR,ICON_LADY_ICON1_03_ADDR,ICON_LADY_ICON1_04_ADDR,ICON_LADY_ICON1_05_ADDR,
      ICON_LADY_ICON1_06_ADDR,ICON_LADY_ICON1_07_ADDR,ICON_LADY_ICON1_08_ADDR,ICON_LADY_ICON1_09_ADDR,ICON_LADY_ICON1_10_ADDR,
};

const uint32_t icon_16_inform_lady11_addr[10] = //月经期
{
    
      ICON_LADY_ICON1_12_ADDR,ICON_LADY_ICON1_13_ADDR,ICON_LADY_ICON1_14_ADDR,ICON_LADY_ICON1_15_ADDR,ICON_LADY_ICON1_16_ADDR,
      ICON_LADY_ICON1_17_ADDR,ICON_LADY_ICON1_18_ADDR,ICON_LADY_ICON1_19_ADDR,ICON_LADY_ICON1_20_ADDR,ICON_LADY_ICON1_21_ADDR,
};

const uint32_t icon_16_inform_lady111_addr[1] = //月经期
{
    
      ICON_LADY_ICON1_11_ADDR,
};

const uint8_t icon_16_inform_lady11_coord[3][2] = 
{
   {63, 63},{107, 63},{132, 63},

};
const uint32_t icon_16_inform_lady2_addr[3] = //安全期
{
    
      ICON_LADY_ICON2_1_ADDR,ICON_LADY_ICON2_2_ADDR,ICON_LADY_ICON2_3_ADDR,
    
};
const uint32_t icon_16_inform_lady3_addr[6] = //排卵期
{
    
      ICON_LADY_ICON3_1_ADDR,ICON_LADY_ICON3_2_ADDR,ICON_LADY_ICON3_3_ADDR,
      ICON_LADY_ICON3_4_ADDR,ICON_LADY_ICON3_5_ADDR,ICON_LADY_ICON3_6_ADDR,
    
};
const uint32_t icon_16_inform_lady4_addr[8] = //排卵日
{
    
      ICON_LADY_ICON4_1_ADDR,ICON_LADY_ICON4_2_ADDR,ICON_LADY_ICON4_3_ADDR,
      ICON_LADY_ICON4_4_ADDR,ICON_LADY_ICON4_5_ADDR,ICON_LADY_ICON4_4_ADDR,ICON_LADY_ICON4_3_ADDR,ICON_LADY_ICON4_2_ADDR,
    
};
const uint8_t icon_16_inform_lady1_coord[5][2] = 
{
   {63, 63},{120, 120},{120, 63},{63, 63},{63, 120},

};