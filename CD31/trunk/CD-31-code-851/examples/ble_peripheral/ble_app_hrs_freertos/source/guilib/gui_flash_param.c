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


/////////////////////////////////////////////////////////////
// 主界面1
/////////////////////////////////////////////////////////////


// 主界面1-蓝牙
const uint8_t icon_16_main1_blue_coord[1][2] = {{3,2}};
const uint32_t icon_16_main1_blue_addr[1][2] = 
{
    {
        ICON_MAIN1_ICON1_BLUETOOTH_NOT_ADDR, ICON_MAIN1_ICON1_BLUETOOTH_ADDR
    }
};

// 主界面1-星期
const uint8_t icon_16_main1_week_coord[1][2] = {{38,2}};
const uint32_t icon_16_main1_week_addr[1][2][7] = 
{
    {
        {
            ICON_MAIN1_WEEK_CN_1_ADDR,ICON_MAIN1_WEEK_CN_2_ADDR,ICON_MAIN1_WEEK_CN_3_ADDR,
            ICON_MAIN1_WEEK_CN_4_ADDR,ICON_MAIN1_WEEK_CN_5_ADDR,ICON_MAIN1_WEEK_CN_6_ADDR,
            ICON_MAIN1_WEEK_CN_7_ADDR
        }
        ,
        {
            ICON_MAIN1_WEEK_EN_1_ADDR,ICON_MAIN1_WEEK_EN_2_ADDR,ICON_MAIN1_WEEK_EN_3_ADDR,
            ICON_MAIN1_WEEK_EN_4_ADDR,ICON_MAIN1_WEEK_EN_5_ADDR,ICON_MAIN1_WEEK_EN_6_ADDR,
            ICON_MAIN1_WEEK_EN_7_ADDR
        }
    }
};

// 主界面1-上下午
const uint8_t icon_16_main1_ampm_coord[1][2] = {{83,3}};
const uint32_t icon_16_main1_ampm_addr[1][5] = 
{
    {
        ICON_MAIN1_ICON2_AM_CN_ADDR,ICON_MAIN1_ICON2_PM_CN_ADDR,
        ICON_MAIN1_ICON2_AM_EN_ADDR,ICON_MAIN1_ICON2_PM_EN_ADDR,
        ICON_MAIN1_ICON2_BG_ADDR
    }
};

// 主界面1-女性
const uint8_t icon_16_main1_menstrual_coord[1][2] = {{182,2}};
const uint32_t icon_16_main1_menstrual_addr[1][5] = 
{
    {
        ICON_MAIN1_ICON3_BG_ADDR, ICON_MAIN1_ICON3_1_ADDR, ICON_MAIN1_ICON3_2_ADDR,
        ICON_MAIN1_ICON3_3_ADDR, ICON_MAIN1_ICON3_4_ADDR
    }
};

// 主界面1-电池框
const uint8_t icon_16_main1_battery_coord[1][2] = {{208,2}};
const uint32_t icon_16_main1_battery_addr[1][6] =
{
    {
        ICON_MAIN1_ICON4_BATTERY_1_ADDR, ICON_MAIN1_ICON4_BATTERY_2_ADDR, ICON_MAIN1_ICON4_BATTERY_3_ADDR,
        ICON_MAIN1_ICON4_BATTERY_4_ADDR, ICON_MAIN1_ICON4_BATTERY_5_ADDR, ICON_MAIN1_ICON4_BATTERY_BG_ADDR
    }
};



// 主界面1
// 主界面1-主图标

// 主界面1-时间
const uint8_t icon_16_main1_time_coord[4][2] = 
{
    {1, 32},
    {1 + 1 * ICON_MAIN1_DATA1_WIDE, 32},
    {1 + 2 * ICON_MAIN1_DATA1_WIDE + ICON_MAIN1_DATA1DOT_WIDE + 2, 32},
    {1 + 3 * ICON_MAIN1_DATA1_WIDE + ICON_MAIN1_DATA1DOT_WIDE + 2, 32}
};
const uint32_t icon_16_main1_time_addr[1][10] = 
{
    {
        ICON_MAIN1_DATA1_0_ADDR,ICON_MAIN1_DATA1_1_ADDR,ICON_MAIN1_DATA1_2_ADDR,
        ICON_MAIN1_DATA1_3_ADDR,ICON_MAIN1_DATA1_4_ADDR,ICON_MAIN1_DATA1_5_ADDR,
        ICON_MAIN1_DATA1_6_ADDR,ICON_MAIN1_DATA1_7_ADDR,ICON_MAIN1_DATA1_8_ADDR,
        ICON_MAIN1_DATA1_9_ADDR
    }
};

// 主界面1-时间冒号
const uint8_t icon_16_main1_timedot_coord[1][2] = {{112,32}};
const uint32_t icon_16_main1_timedot_addr[1][1] = 
{
    {
        ICON_MAIN1_DATA1DOT_ADDR
    }
};

// 主界面1-日期
const uint8_t icon_16_main1_data_coord[4][2] = 
{
    {175, 175},
    {175 + 1 * ICON_MAIN1_DATA2_WIDE, 175},
    {175 + 2 * ICON_MAIN1_DATA2_WIDE + ICON_MAIN1_DATA2DOT_WIDE, 175},
    {175 + 3 * ICON_MAIN1_DATA2_WIDE + ICON_MAIN1_DATA2DOT_WIDE, 175}
};
const uint32_t icon_16_main1_data_addr[1][10] = 
{
    {
        ICON_MAIN1_DATA2_0_ADDR,ICON_MAIN1_DATA2_1_ADDR,ICON_MAIN1_DATA2_2_ADDR,
        ICON_MAIN1_DATA2_3_ADDR,ICON_MAIN1_DATA2_4_ADDR,ICON_MAIN1_DATA2_5_ADDR,
        ICON_MAIN1_DATA2_6_ADDR,ICON_MAIN1_DATA2_7_ADDR,ICON_MAIN1_DATA2_8_ADDR,
        ICON_MAIN1_DATA2_9_ADDR
    }
};

// 主界面1-日期点
const uint8_t icon_16_main1_datadot_coord[1][2] = {{201,175}};
const uint32_t icon_16_main1_datadot_addr[1][1] = 
{
    {
        ICON_MAIN1_DATA2DOT_ADDR
    }
};

// 主界面1-计步图标
const uint8_t icon_16_main1_step_icon_coord[1][2] = {{2,204}};
const uint32_t icon_16_main1_step_icon_addr[1][1] = 
{
    {
        ICON_MAIN1_ICON5_ADDR
    }
};

// 主界面1-计步数字
const uint8_t icon_16_main1_step_num_coord[5][2] = 
{
    {34,208},
    {34 + 1 * ICON_MAIN1_DATA3_WIDE,208},
    {34 + 2 * ICON_MAIN1_DATA3_WIDE,208},
    {34 + 3 * ICON_MAIN1_DATA3_WIDE,208},
    {34 + 4 * ICON_MAIN1_DATA3_WIDE,208}
};
const uint32_t icon_16_main1_step_num_addr[1][10] = 
{
    {
        ICON_MAIN1_DATA3_0_ADDR,ICON_MAIN1_DATA3_1_ADDR,ICON_MAIN1_DATA3_2_ADDR,
        ICON_MAIN1_DATA3_3_ADDR,ICON_MAIN1_DATA3_4_ADDR,ICON_MAIN1_DATA3_5_ADDR,
        ICON_MAIN1_DATA3_6_ADDR,ICON_MAIN1_DATA3_7_ADDR,ICON_MAIN1_DATA3_8_ADDR,
        ICON_MAIN1_DATA3_9_ADDR
    }
};

// 主界面1-睡眠图标
const uint8_t icon_16_main1_sleep_icon_coord[1][2] = {{161,204}};
const uint32_t icon_16_main1_sleep_icon_addr[1][1] = 
{
    {
        ICON_MAIN1_ICON6_ADDR
    }
};

// 主界面1-睡眠数字
const uint8_t icon_16_main1_sleep_num_coord[3][2] = 
{
    {192,208},
    {192 + 1 * ICON_MAIN1_DATA3_WIDE,208},
    {192 + 2 * ICON_MAIN1_DATA3_WIDE + ICON_MAIN1_DATA3DOT_WIDE,208}
};
const uint32_t icon_16_main1_sleep_num_addr[1][10] = 
{
    {
        ICON_MAIN1_DATA3_0_ADDR,ICON_MAIN1_DATA3_1_ADDR,ICON_MAIN1_DATA3_2_ADDR,
        ICON_MAIN1_DATA3_3_ADDR,ICON_MAIN1_DATA3_4_ADDR,ICON_MAIN1_DATA3_5_ADDR,
        ICON_MAIN1_DATA3_6_ADDR,ICON_MAIN1_DATA3_7_ADDR,ICON_MAIN1_DATA3_8_ADDR,
        ICON_MAIN1_DATA3_9_ADDR
    }
};

// 主界面1-睡眠点
const uint8_t icon_16_main1_sleepdot_coord[1][2] = {{220,208}};
const uint32_t icon_16_main1_sleepdot_addr[1][1] = 
{
    {
        ICON_MAIN1_DATA3DOT_ADDR
    }
};


/////////////////////////////////////////////////////////////
// 主界面2
/////////////////////////////////////////////////////////////

#define MAIN3_LEFT_OFFSET      (-3)

// 主界面2-时间
const uint8_t icon_16_main2_time_coord[4][2] = 
{
    {65, 10},
    {65 + 1 * ICON_MAIN2_DATA2_WIDE, 10},
    {65, 128},
    {65 + 1 * ICON_MAIN2_DATA2_WIDE, 128}
};
const uint32_t icon_16_main2_hour_addr[1][10] = 
{
    {
        ICON_MAIN2_DATA2_0_ADDR, ICON_MAIN2_DATA2_1_ADDR, ICON_MAIN2_DATA2_2_ADDR,
        ICON_MAIN2_DATA2_3_ADDR, ICON_MAIN2_DATA2_4_ADDR, ICON_MAIN2_DATA2_5_ADDR,
        ICON_MAIN2_DATA2_6_ADDR, ICON_MAIN2_DATA2_7_ADDR, ICON_MAIN2_DATA2_8_ADDR,
        ICON_MAIN2_DATA2_9_ADDR
    }
};
const uint32_t icon_16_main2_minute_addr[1][10] = 
{
    {
        ICON_MAIN2_DATA3_0_ADDR, ICON_MAIN2_DATA3_1_ADDR, ICON_MAIN2_DATA3_2_ADDR,
        ICON_MAIN2_DATA3_3_ADDR, ICON_MAIN2_DATA3_4_ADDR, ICON_MAIN2_DATA3_5_ADDR,
        ICON_MAIN2_DATA3_6_ADDR, ICON_MAIN2_DATA3_7_ADDR, ICON_MAIN2_DATA3_8_ADDR,
        ICON_MAIN2_DATA3_9_ADDR
    }
};

// 主界面2-日期星期背景
const uint8_t icon_16_main2_databg_coord[2][2] = 
{
    {1, 10 + MAIN3_LEFT_OFFSET},
    {1, 86 + MAIN3_LEFT_OFFSET}
};
const uint32_t icon_16_main2_databg_addr[1][1] = 
{
    {
        ICON_MAIN2_ICON1_ADDR
    }
};

// 主界面2-日期
const uint8_t icon_16_main2_data_coord[2][2] = 
{
    {10, 31 + MAIN3_LEFT_OFFSET},
    {10 + 1 * ICON_MAIN2_DATA1_WIDE, 31 + MAIN3_LEFT_OFFSET}
};
const uint32_t icon_16_main2_data_addr[1][10] = 
{
    {
        ICON_MAIN2_DATA1_0_ADDR, ICON_MAIN2_DATA1_1_ADDR, ICON_MAIN2_DATA1_2_ADDR,
        ICON_MAIN2_DATA1_3_ADDR, ICON_MAIN2_DATA1_4_ADDR, ICON_MAIN2_DATA1_5_ADDR,
        ICON_MAIN2_DATA1_6_ADDR, ICON_MAIN2_DATA1_7_ADDR, ICON_MAIN2_DATA1_8_ADDR,
        ICON_MAIN2_DATA1_9_ADDR
    }
};

// 主界面2-星期
const uint8_t icon_16_main2_week_coord[1][2] = 
{
    {5, 110 + MAIN3_LEFT_OFFSET}
};
const uint32_t icon_16_main2_week_addr[1][2][7] = 
{
    {
        {
            ICON_MAIN2_WEEK_CN_1_ADDR,ICON_MAIN2_WEEK_CN_2_ADDR,ICON_MAIN2_WEEK_CN_3_ADDR,
            ICON_MAIN2_WEEK_CN_4_ADDR,ICON_MAIN2_WEEK_CN_5_ADDR,ICON_MAIN2_WEEK_CN_6_ADDR,
            ICON_MAIN2_WEEK_CN_7_ADDR
        }
        ,
        {
            ICON_MAIN2_WEEK_EN_1_ADDR,ICON_MAIN2_WEEK_EN_2_ADDR,ICON_MAIN2_WEEK_EN_3_ADDR,
            ICON_MAIN2_WEEK_EN_4_ADDR,ICON_MAIN2_WEEK_EN_5_ADDR,ICON_MAIN2_WEEK_EN_6_ADDR,
            ICON_MAIN2_WEEK_EN_7_ADDR
        }
    }
};

// 主界面2-计步背景
const uint8_t icon_16_main2_step_bg_coord[1][2] = 
{
    {1, 162 + MAIN3_LEFT_OFFSET}
};
const uint32_t icon_16_main2_step_bg_addr[1][1] = 
{
    {
        ICON_MAIN2_ICON2_BG_ADDR
    }
};

// 主界面2-计步数字
const uint8_t icon_16_main2_step_ring_coord[20][2] = 
{
    { 33, 171 + MAIN3_LEFT_OFFSET}, { 41, 173 + MAIN3_LEFT_OFFSET}, { 44, 178 + MAIN3_LEFT_OFFSET}, { 47, 183 + MAIN3_LEFT_OFFSET}, { 48, 188 + MAIN3_LEFT_OFFSET},
    { 48, 194 + MAIN3_LEFT_OFFSET}, { 47, 199 + MAIN3_LEFT_OFFSET}, { 44, 204 + MAIN3_LEFT_OFFSET}, { 41, 209 + MAIN3_LEFT_OFFSET}, { 33, 211 + MAIN3_LEFT_OFFSET},
    { 24, 211 + MAIN3_LEFT_OFFSET}, { 16, 209 + MAIN3_LEFT_OFFSET}, { 13, 204 + MAIN3_LEFT_OFFSET}, { 10, 199 + MAIN3_LEFT_OFFSET}, {  9, 194 + MAIN3_LEFT_OFFSET},
    {  9, 188 + MAIN3_LEFT_OFFSET}, { 10, 183 + MAIN3_LEFT_OFFSET}, { 13, 178 + MAIN3_LEFT_OFFSET}, { 16, 173 + MAIN3_LEFT_OFFSET}, { 24, 171 + MAIN3_LEFT_OFFSET}
};
const uint32_t icon_16_main2_step_ring_addr[1][20] = 
{
    {
        ICON_MAIN2_ICON3_01_ADDR, ICON_MAIN2_ICON3_02_ADDR, ICON_MAIN2_ICON3_03_ADDR,
        ICON_MAIN2_ICON3_04_ADDR, ICON_MAIN2_ICON3_05_ADDR, ICON_MAIN2_ICON3_06_ADDR,
        ICON_MAIN2_ICON3_07_ADDR, ICON_MAIN2_ICON3_08_ADDR, ICON_MAIN2_ICON3_09_ADDR,
        ICON_MAIN2_ICON3_10_ADDR, ICON_MAIN2_ICON3_11_ADDR, ICON_MAIN2_ICON3_12_ADDR,
        ICON_MAIN2_ICON3_13_ADDR, ICON_MAIN2_ICON3_14_ADDR, ICON_MAIN2_ICON3_15_ADDR,
        ICON_MAIN2_ICON3_16_ADDR, ICON_MAIN2_ICON3_17_ADDR, ICON_MAIN2_ICON3_18_ADDR,
        ICON_MAIN2_ICON3_19_ADDR, ICON_MAIN2_ICON3_20_ADDR
    }
};


/////////////////////////////////////////////////////////////
// 主界面3
/////////////////////////////////////////////////////////////

// 主界面3-时间
const uint8_t icon_16_main3_time_coord[4][2] = 
{
    {6, 58},
    {6 + 1 * ICON_MAIN3_DATA1_WIDE, 58},
    {6 + 2 * ICON_MAIN3_DATA1_WIDE + ICON_MAIN3_DATA1DOT_WIDE, 58},
    {6 + 3 * ICON_MAIN3_DATA1_WIDE + ICON_MAIN3_DATA1DOT_WIDE, 58}
};
const uint32_t icon_16_main3_time_addr[1][10] = 
{
    {
        ICON_MAIN3_DATA1_0_ADDR, ICON_MAIN3_DATA1_1_ADDR, ICON_MAIN3_DATA1_2_ADDR,
        ICON_MAIN3_DATA1_3_ADDR, ICON_MAIN3_DATA1_4_ADDR, ICON_MAIN3_DATA1_5_ADDR,
        ICON_MAIN3_DATA1_6_ADDR, ICON_MAIN3_DATA1_7_ADDR, ICON_MAIN3_DATA1_8_ADDR,
        ICON_MAIN3_DATA1_9_ADDR
    }
};
const uint32_t icon_16_main3_time3_addr[1][10] = 
{
    {
        ICON_MAIN3_DATA3_0_ADDR, ICON_MAIN3_DATA3_1_ADDR, ICON_MAIN3_DATA3_2_ADDR,
        ICON_MAIN3_DATA3_3_ADDR, ICON_MAIN3_DATA3_4_ADDR, ICON_MAIN3_DATA3_5_ADDR,
        ICON_MAIN3_DATA3_6_ADDR, ICON_MAIN3_DATA3_7_ADDR, ICON_MAIN3_DATA3_8_ADDR,
        ICON_MAIN3_DATA3_9_ADDR
    }
};

// 主界面3-时间冒号
const uint8_t icon_16_main3_timedot_coord[1][2] = 
{
    {6 + 2 * ICON_MAIN3_DATA1_WIDE, 58}
};
const uint32_t icon_16_main3_timedot_addr[1][1] = 
{
    {
        ICON_MAIN3_DATA1DOT_ADDR
    }
};

// 主界面3-日期
const uint8_t icon_16_main3_data_coord[4][2] = 
{ 
    {183, 155},
    {183 + 1 * ICON_MAIN3_DATA2_WIDE, 155},
    {183 + 2 * ICON_MAIN3_DATA2_WIDE + ICON_MAIN3_DATA2DOT_WIDE, 155},
    {183 + 3 * ICON_MAIN3_DATA2_WIDE + ICON_MAIN3_DATA2DOT_WIDE, 155}, 
};
const uint32_t icon_16_main3_data_addr[1][10] = 
{
    {
        ICON_MAIN3_DATA2_0_ADDR, ICON_MAIN3_DATA2_1_ADDR, ICON_MAIN3_DATA2_2_ADDR,
        ICON_MAIN3_DATA2_3_ADDR, ICON_MAIN3_DATA2_4_ADDR, ICON_MAIN3_DATA2_5_ADDR,
        ICON_MAIN3_DATA2_6_ADDR, ICON_MAIN3_DATA2_7_ADDR, ICON_MAIN3_DATA2_8_ADDR,
        ICON_MAIN3_DATA2_9_ADDR
    }
};

// 主界面3-日期点
const uint8_t icon_16_main3_datadot_coord[1][2] = 
{
    {183 + 2 * ICON_MAIN3_DATA2_WIDE, 155}
};
const uint32_t icon_16_main3_datadot_addr[1][1] = 
{
    {
        ICON_MAIN3_DATA2DOT_ADDR
    }
};

// 主界面3-背景图片
const uint8_t icon_16_main3_bg_coord[1][2] = 
{
    {0,166}
};
const uint32_t icon_16_main3_bg_addr[1][1] = 
{
    {
        ICON_MAIN3_ICON_BG_ADDR
    }
};

/////////////////////////////////////////////////////////////
// 主界面4
/////////////////////////////////////////////////////////////

// 主界面4-背景
const uint8_t icon_16_main4_bg_coord[1][2] = 
{
    {0, 0}
};
const uint32_t icon_16_main4_bg_addr[1] = 
{
    ICON_MAIN4_BG_ADDR
};

// 主界面4-小时点
const uint8_t icon_16_main4_hourdot_coord[8][2] = 
{
    {170,  24},{211,  65},{212, 174},{170, 214},
    { 61, 214},{ 20, 174},{ 20,  65},{ 61,  24}
};
const uint32_t icon_16_main4_hourdot_addr[2] = 
{
    ICON_MAIN4_DIAN_1_ADDR, ICON_MAIN4_DIAN_2_ADDR
};

// 主界面4-小时3-图标
const uint8_t icon_16_main4_hour3_coord[1][2] = 
{
    {197,  93}
};
const uint32_t icon_16_main4_hour3_addr[2] = 
{
    ICON_MAIN4_ICON1_1_ADDR, ICON_MAIN4_ICON1_2_ADDR
};

// 主界面4-小时6-图标
const uint8_t icon_16_main4_hour6_coord[1][2] = 
{
    {98,  184}
};
const uint32_t icon_16_main4_hour6_addr[2] = 
{
    ICON_MAIN4_ICON2_1_ADDR, ICON_MAIN4_ICON2_2_ADDR
};

// 主界面4-小时9-图标
const uint8_t icon_16_main4_hour9_coord[1][2] = 
{
    {0,  93}
};
const uint32_t icon_16_main4_hour9_addr[2] = 
{
    ICON_MAIN4_ICON3_1_ADDR, ICON_MAIN4_ICON3_3_ADDR
};

// 主界面4-小时12-图标
const uint8_t icon_16_main4_hour12_coord[1][2] = 
{
    {93,  0}
};
const uint32_t icon_16_main4_hour12_addr[2] = 
{
    ICON_MAIN4_ICON4_1_ADDR, ICON_MAIN4_ICON4_4_ADDR
};

// 主界面4-时间
const uint8_t icon_16_main4_time_coord[4][2] = 
{
    {50, 101},
    {50 + 1 * ICON_MAIN4_DATA2_WIDE, 101},
    {50 + 2 * ICON_MAIN4_DATA2_WIDE + ICON_MAIN4_DATA2DOT_WIDE, 101},
    {50 + 3 * ICON_MAIN4_DATA2_WIDE + ICON_MAIN4_DATA2DOT_WIDE, 101}
};
const uint32_t icon_16_main4_time_addr[1][10] = 
{
    {
        ICON_MAIN4_DATA2_0_ADDR, ICON_MAIN4_DATA2_1_ADDR, ICON_MAIN4_DATA2_2_ADDR,
        ICON_MAIN4_DATA2_3_ADDR, ICON_MAIN4_DATA2_4_ADDR, ICON_MAIN4_DATA2_5_ADDR,
        ICON_MAIN4_DATA2_6_ADDR, ICON_MAIN4_DATA2_7_ADDR, ICON_MAIN4_DATA2_8_ADDR,
        ICON_MAIN4_DATA2_9_ADDR
    }
};

// 主界面4-时间冒号
const uint8_t icon_16_main4_timedot_coord[1][2] = 
{
    {50 + 2 * ICON_MAIN4_DATA2_WIDE, 101}
};
const uint32_t icon_16_main4_timedot_addr[1][1] = 
{
    {
        ICON_MAIN4_DATA2DOT_ADDR
    }
};

// 主界面4-日期
const uint8_t icon_16_main4_data_coord[4][2] = 
{
    {87, 77},
    {87 + 1 * ICON_MAIN4_DATA1_WIDE, 77},
    {87 + 2 * ICON_MAIN4_DATA1_WIDE + ICON_MAIN4_DATA1DOT_WIDE, 77},
    {87 + 3 * ICON_MAIN4_DATA1_WIDE + ICON_MAIN4_DATA1DOT_WIDE, 77}
};
const uint32_t icon_16_main4_data_addr[1][10] = 
{
    {
        ICON_MAIN4_DATA1_0_ADDR, ICON_MAIN4_DATA1_1_ADDR, ICON_MAIN4_DATA1_2_ADDR,
        ICON_MAIN4_DATA1_3_ADDR, ICON_MAIN4_DATA1_4_ADDR, ICON_MAIN4_DATA1_5_ADDR,
        ICON_MAIN4_DATA1_6_ADDR, ICON_MAIN4_DATA1_7_ADDR, ICON_MAIN4_DATA1_8_ADDR,
        ICON_MAIN4_DATA1_9_ADDR
    }
};

// 主界面4-日期点
const uint8_t icon_16_main4_datadot_coord[1][2] = 
{
    {87 + 2 * ICON_MAIN4_DATA1_WIDE, 77}
};
const uint32_t icon_16_main4_datadot_addr[1][1] = 
{
    {
        ICON_MAIN4_DATA1DOT_ADDR
    }
};

// 主界面4-星期
const uint8_t icon_16_main4_week_coord[1][2] = 
{
    {90, 144}
};
const uint32_t icon_16_main4_week_addr[1][2][7] = 
{
    {
        {
            ICON_MAIN4_WEEK_CN_1_ADDR, ICON_MAIN4_WEEK_CN_2_ADDR, ICON_MAIN4_WEEK_CN_3_ADDR,
            ICON_MAIN4_WEEK_CN_4_ADDR, ICON_MAIN4_WEEK_CN_5_ADDR, ICON_MAIN4_WEEK_CN_6_ADDR,
            ICON_MAIN4_WEEK_CN7_ADDR,
        }
        ,
        {
            ICON_MAIN4_WEEK_EN_1_ADDR, ICON_MAIN4_WEEK_EN_2_ADDR, ICON_MAIN4_WEEK_EN_3_ADDR,
            ICON_MAIN4_WEEK_EN_4_ADDR, ICON_MAIN4_WEEK_EN_5_ADDR, ICON_MAIN4_WEEK_EN_6_ADDR,
            ICON_MAIN4_WEEK_EN_7_ADDR,
        }
    }
};


///////////////////////////////////////////////////////////////
//// 主界面5
///////////////////////////////////////////////////////////////

// 主界面5-时间
const uint8_t icon_16_main5_time_coord[4][2] = 
{
    {16, 59},
    {16 + 1 * ICON_MAIN5_DATA2_WIDE, 59},
    {16 + 2 * ICON_MAIN5_DATA2_WIDE + ICON_MAIN5_DATA2DOT_WIDE + 10, 59},
    {16 + 3 * ICON_MAIN5_DATA2_WIDE + ICON_MAIN5_DATA2DOT_WIDE + 10, 59}
};
const uint32_t icon_16_main5_time_addr[1][10] = 
{
    {
        ICON_MAIN5_DATA2_0_ADDR, ICON_MAIN5_DATA2_1_ADDR, ICON_MAIN5_DATA2_2_ADDR,
        ICON_MAIN5_DATA2_3_ADDR, ICON_MAIN5_DATA2_4_ADDR, ICON_MAIN5_DATA2_5_ADDR,
        ICON_MAIN5_DATA2_6_ADDR, ICON_MAIN5_DATA2_7_ADDR, ICON_MAIN5_DATA2_8_ADDR,
        ICON_MAIN5_DATA2_9_ADDR
    }
};

// 主界面5-时间冒号
const uint8_t icon_16_main5_timedot_coord[1][2] = 
{
    {16 + 2 * ICON_MAIN5_DATA2_WIDE + 5, 59}
};
const uint32_t icon_16_main5_timedot_addr[1][1] = 
{
    {
        ICON_MAIN5_DATA2DOT_ADDR
    }
};

// 主界面5-日期
const uint8_t icon_16_main5_data_coord[4][2] = 
{
    {93, 34},
    {93 + 1 * ICON_MAIN5_DATA1_WIDE, 34},
    {93 + 2 * ICON_MAIN5_DATA1_WIDE + ICON_MAIN5_DATA1DOT_WIDE, 34},
    {93 + 3 * ICON_MAIN5_DATA1_WIDE + ICON_MAIN5_DATA1DOT_WIDE, 34}
};
const uint32_t icon_16_main5_data_addr[1][10] = 
{
    {
        ICON_MAIN5_DATA1_0_ADDR, ICON_MAIN5_DATA1_1_ADDR, ICON_MAIN5_DATA1_2_ADDR,
        ICON_MAIN5_DATA1_3_ADDR, ICON_MAIN5_DATA1_4_ADDR, ICON_MAIN5_DATA1_5_ADDR,
        ICON_MAIN5_DATA1_6_ADDR, ICON_MAIN5_DATA1_7_ADDR, ICON_MAIN5_DATA1_8_ADDR,
        ICON_MAIN5_DATA1_9_ADDR
    }
};

// 主界面5-日期点
const uint8_t icon_16_main5_datadot_coord[1][2] = 
{
    {93 + 2 * ICON_MAIN5_DATA1_WIDE, 34}
};
const uint32_t icon_16_main5_datadot_addr[1][1] = 
{
    {
        ICON_MAIN5_DATA1DOT_ADDR
    }
};

// 主界面5-背景0
const uint8_t icon_16_main5_bg0_coord[1][2] = 
{
    {224, 110}
};
const uint32_t icon_16_main5_bg0_addr[1][1] = 
{
    {
        ICON_MAIN5_ICON6_ADDR
    }
};

// 主界面5-背景1
const uint8_t icon_16_main5_bg1_coord[1][2] = 
{
    {0, 120}
};
const uint32_t icon_16_main5_bg1_addr[1][1] = 
{
    {
        ICON_MAIN5_ICON5_ADDR
    }
};


///////////////////////////////////////////////////////////////
//// 功能界面
///////////////////////////////////////////////////////////////


// 计步卡路里距离
// 计步卡路里距离-计步背景图标
const uint32_t icon_16_scd_step_bg_coord[1][2] =
{
    {13, 13}
}; 
const uint32_t icon_16_scd_step_bg_addr[1] = 
{
    ICON_STEP_ICON2_ADDR
};

// 计步卡路里距离-计步图标
const uint32_t icon_16_scd_step_icon_coord[1][2] =
{
    {34, 23}
}; 
const uint32_t icon_16_scd_step_icon_addr[8] = 
{
    ICON_STEP_ICON1_01_ADDR, ICON_STEP_ICON1_02_ADDR, ICON_STEP_ICON1_03_ADDR,
    ICON_STEP_ICON1_04_ADDR, ICON_STEP_ICON1_05_ADDR, ICON_STEP_ICON1_06_ADDR,
    ICON_STEP_ICON1_07_ADDR, ICON_STEP_ICON1_08_ADDR
};

// 计步卡路里距离-计步数值
const uint32_t icon_16_scd_step_num_coord[5][2] =
{
    {91, 27},
    {91 + 1 * ICON_STEP_DATA1_WIDE, 27},
    {91 + 2 * ICON_STEP_DATA1_WIDE, 27},
    {91 + 3 * ICON_STEP_DATA1_WIDE, 27},
    {91 + 4 * ICON_STEP_DATA1_WIDE, 27}
}; 
const uint32_t icon_16_scd_step_num_addr[10] = 
{
    ICON_STEP_DATA1_0_ADDR, ICON_STEP_DATA1_1_ADDR, ICON_STEP_DATA1_2_ADDR,
    ICON_STEP_DATA1_3_ADDR, ICON_STEP_DATA1_4_ADDR, ICON_STEP_DATA1_5_ADDR,
    ICON_STEP_DATA1_6_ADDR, ICON_STEP_DATA1_7_ADDR, ICON_STEP_DATA1_8_ADDR,
    ICON_STEP_DATA1_9_ADDR
};

// 计步卡路里距离-计步单位
const uint32_t icon_16_scd_step_unit_coord[1][2] =
{
    {185, 68}
}; 
const uint32_t icon_16_scd_step_unit_addr[2] = 
{
    ICON_STEP_UNIT1_CN_ADDR, ICON_STEP_UNIT1_EN_ADDR
};

// 计步卡路里距离-计步进度条
const uint32_t icon_16_scd_progress_icon_coord[1][2] =
{
    {14, 88}
}; 
const uint32_t icon_16_scd_progress_icon_addr[26] = 
{
    ICON_STEP_ICON3_01_ADDR, ICON_STEP_ICON3_02_ADDR, ICON_STEP_ICON3_03_ADDR,
    ICON_STEP_ICON3_04_ADDR, ICON_STEP_ICON3_05_ADDR, ICON_STEP_ICON3_06_ADDR,
    ICON_STEP_ICON3_07_ADDR, ICON_STEP_ICON3_08_ADDR, ICON_STEP_ICON3_09_ADDR,
    ICON_STEP_ICON3_10_ADDR, ICON_STEP_ICON3_11_ADDR, ICON_STEP_ICON3_12_ADDR,
    ICON_STEP_ICON3_13_ADDR, ICON_STEP_ICON3_14_ADDR, ICON_STEP_ICON3_15_ADDR,
    ICON_STEP_ICON3_16_ADDR, ICON_STEP_ICON3_17_ADDR, ICON_STEP_ICON3_18_ADDR,
    ICON_STEP_ICON3_19_ADDR, ICON_STEP_ICON3_20_ADDR, ICON_STEP_ICON3_21_ADDR,
    ICON_STEP_ICON3_22_ADDR, ICON_STEP_ICON3_23_ADDR, ICON_STEP_ICON3_24_ADDR,
    ICON_STEP_ICON3_25_ADDR, ICON_STEP_ICON3_26_ADDR
};

// 计步卡路里距离-计步进度标语
const uint32_t icon_16_scd_progress_label_coord[2][2] =
{
    {55, 117},
    {13, 117}  //{17, 117}
}; 
const uint32_t icon_16_scd_progress_label_addr[2] = 
{
    ICON_STEP_ICON4_CN_ADDR, ICON_STEP_ICON7_EN_ADDR
};

// 计步卡路里距离-计步进度数字
const uint32_t icon_16_scd_progress_num_coord[2][3][3][2] =
{
    {
        {
            {137, 118}, 
            {137 + ICON_STEP_DATA2_WIDE, 118}, 
            {137 + 2 * ICON_STEP_DATA2_WIDE, 118}
        }
        ,
        {
            {148, 118}, 
            {148 + ICON_STEP_DATA2_WIDE, 118}, 
            {148 + ICON_STEP_DATA2_WIDE, 118}
        }
        ,
        {
            {159, 118}, 
            {159, 118}, 
            {159, 118}
        }
    }
    ,
    {
        {
            {179, 118}, 
            {179 + ICON_STEP_DATA2_WIDE, 118}, 
            {179 + 2 * ICON_STEP_DATA2_WIDE, 118}
        }
        ,
        {
            {190, 118}, 
            {190 + ICON_STEP_DATA2_WIDE, 118}, 
            {190 + ICON_STEP_DATA2_WIDE, 118}
        }
        ,
        {
            {201, 118}, 
            {201, 118}, 
            {201, 118}
        }
    }
}; 
const uint32_t icon_16_scd_progress_num_addr[10] = 
{
    ICON_STEP_DATA2_0_ADDR, ICON_STEP_DATA2_1_ADDR, ICON_STEP_DATA2_2_ADDR,
    ICON_STEP_DATA2_3_ADDR, ICON_STEP_DATA2_4_ADDR, ICON_STEP_DATA2_5_ADDR,
    ICON_STEP_DATA2_6_ADDR, ICON_STEP_DATA2_7_ADDR, ICON_STEP_DATA2_8_ADDR,
    ICON_STEP_DATA2_9_ADDR
};

// 计步卡路里距离-计步进度百分号
const uint32_t icon_16_scd_progress_percent_coord[2][2] =
{
    {170, 118},
    {212, 118}
}; 
const uint32_t icon_16_scd_progress_percent_addr[1] = 
{
    ICON_STEP_DATA2DOT_ADDR
};

// 计步卡路里距离-卡路里图标
const uint32_t icon_16_scd_calories_icon_coord[1][2] =
{
    {21, 148}
}; 
const uint32_t icon_16_scd_calories_icon_addr[1] = 
{
    ICON_STEP_ICON5_ADDR
};

// 计步卡路里距离-卡路里数值
const uint32_t icon_16_scd_calories_num_coord[4][2] =
{
    {61, 147},
    {61 + 1 * ICON_STEP_DATA3_WIDE, 147},
    {61 + 2 * ICON_STEP_DATA3_WIDE, 147},
    {61 + 3 * ICON_STEP_DATA3_WIDE, 147}
}; 
const uint32_t icon_16_scd_calories_num_addr[10] = 
{
    ICON_STEP_DATA3_0_ADDR, ICON_STEP_DATA3_1_ADDR, ICON_STEP_DATA3_2_ADDR,
    ICON_STEP_DATA3_3_ADDR, ICON_STEP_DATA3_4_ADDR, ICON_STEP_DATA3_5_ADDR,
    ICON_STEP_DATA3_6_ADDR, ICON_STEP_DATA3_7_ADDR, ICON_STEP_DATA3_8_ADDR,
    ICON_STEP_DATA3_9_ADDR
};

// 计步卡路里距离-卡路里单位
const uint32_t icon_16_scd_calories_unit_coord[1][2] =
{
    {170, 153}
}; 
const uint32_t icon_16_scd_calories_unit_addr[2] = 
{
    ICON_STEP_UNIT2_CN_ADDR, ICON_STEP_UNIT2_EN_ADDR
};

// 计步卡路里距离-距离图标
const uint32_t icon_16_scd_distance_icon_coord[1][2] =
{
    {21, 187}
}; 
const uint32_t icon_16_scd_distance_icon_addr[1] = 
{
    ICON_STEP_ICON6_ADDR
};

// 距离
// 计步卡路里距离-距离数值
const uint32_t icon_16_scd_distance_num_coord[3][2] =
{
    {61, 187},
    {61 + 1 * ICON_STEP_DATA4_WIDE, 187},
    {61 + 2 * ICON_STEP_DATA4_WIDE + ICON_STEP_DATA4DOT_WIDE + 2, 187}
}; 
const uint32_t icon_16_scd_distance_num_addr[10] = 
{
    ICON_STEP_DATA4_0_ADDR, ICON_STEP_DATA4_1_ADDR, ICON_STEP_DATA4_2_ADDR,
    ICON_STEP_DATA4_3_ADDR, ICON_STEP_DATA4_4_ADDR, ICON_STEP_DATA4_5_ADDR,
    ICON_STEP_DATA4_6_ADDR, ICON_STEP_DATA4_7_ADDR, ICON_STEP_DATA4_8_ADDR,
    ICON_STEP_DATA4_9_ADDR
};

// 计步卡路里距离-距离点符号
const uint32_t icon_16_scd_distance_dot_coord[1][2] ={{112, 187}}; 
const uint32_t icon_16_scd_distance_dot_addr[1] = 
{
    ICON_STEP_DATA4DOT_ADDR
};

// 计步卡路里距离-距离单位
const uint32_t icon_16_scd_distance_unit_coord[1][2] ={{157, 194}}; 
const uint32_t icon_16_scd_distance_unit_addr[3] = 
{
    ICON_STEP_UNIT3_CN_ADDR, ICON_STEP_UNIT3_EN_ADDR, ICON_STEP_UNIT3_MI_ADDR
};


// 运动模式
// 运动模式-进入界面
const uint8_t icon_16_move_entry_man_coord[1][2] = {{49, 25}};
const uint32_t icon_16_move_entry_man_addr[1] = 
{
    ICON_MOVE_ICON1__ADDR
};

const uint8_t icon_16_move_entry_unit_coord[1][2] = {{83, 203}};
const uint32_t icon_16_move_entry_unit_addr[2] = 
{
    ICON_MOVE_ICON2_1_ADDR, ICON_MOVE_ICON2_2_ADDR
};

// 运动模式
// 运动模式-数据界面
// 运动模式-数据界面-上部标签
const uint8_t icon_16_move_icon_coord[1][2] = {{19, 12}};
const uint32_t icon_16_move_icon_addr[9] = 
{
    ICON_MOVE_ICON3_01_ADDR, ICON_MOVE_ICON3_02_ADDR, ICON_MOVE_ICON3_03_ADDR,
    ICON_MOVE_ICON3_04_ADDR, ICON_MOVE_ICON3_05_ADDR, ICON_MOVE_ICON3_06_ADDR,
    ICON_MOVE_ICON3_07_ADDR, ICON_MOVE_ICON3_08_ADDR, ICON_MOVE_ICON3_09_ADDR
};


// 运动模式-数据界面-时间
const uint8_t icon_16_move_time_coord[4][2] = 
{
    {187, 10},
    {187 + 1 * ICON_MOVE_DATA1_WIDE, 10},
    {187 + 2 * ICON_MOVE_DATA1_WIDE + ICON_MOVE_DATA1DOT_WIDE, 10},
    {187 + 3 * ICON_MOVE_DATA1_WIDE + ICON_MOVE_DATA1DOT_WIDE, 10},
};
const uint32_t icon_16_move_time_addr[10] = 
{
    ICON_MOVE_DATA1_0_ADDR, ICON_MOVE_DATA1_1_ADDR, ICON_MOVE_DATA1_2_ADDR,
    ICON_MOVE_DATA1_3_ADDR, ICON_MOVE_DATA1_4_ADDR, ICON_MOVE_DATA1_5_ADDR,
    ICON_MOVE_DATA1_6_ADDR, ICON_MOVE_DATA1_7_ADDR, ICON_MOVE_DATA1_8_ADDR,
    ICON_MOVE_DATA1_9_ADDR
};

// 运动模式-数据界面-时间冒号
const uint8_t icon_16_move_time_dot_coord[1][2] = 
{
    {187 + 2 * ICON_MOVE_DATA1_WIDE, 10},
};
const uint32_t icon_16_move_time_dot_addr[1] = 
{
    ICON_MOVE_DATA1DOT_ADDR
};

// 运动模式-数据界面-运动时间
const uint8_t icon_16_move_runtime_coord[6][2] = 
{
    {18, 85},
    {18 + 1 * ICON_MOVE_DATA2_WIDE, 85},
    {18 + 2 * ICON_MOVE_DATA2_WIDE + 1 * ICON_MOVE_DATA2DOT_WIDE, 85},
    {18 + 3 * ICON_MOVE_DATA2_WIDE + 1 * ICON_MOVE_DATA2DOT_WIDE, 85},
    {18 + 4 * ICON_MOVE_DATA2_WIDE + 2 * ICON_MOVE_DATA2DOT_WIDE, 85},
    {18 + 5 * ICON_MOVE_DATA2_WIDE + 2 * ICON_MOVE_DATA2DOT_WIDE, 85},
};
const uint32_t icon_16_move_runtime_addr[10] = 
{
    ICON_MOVE_DATA2_0_ADDR, ICON_MOVE_DATA2_1_ADDR, ICON_MOVE_DATA2_2_ADDR,
    ICON_MOVE_DATA2_3_ADDR, ICON_MOVE_DATA2_4_ADDR, ICON_MOVE_DATA2_5_ADDR,
    ICON_MOVE_DATA2_6_ADDR, ICON_MOVE_DATA2_7_ADDR, ICON_MOVE_DATA2_8_ADDR,
    ICON_MOVE_DATA2_9_ADDR
};

// 运动模式-数据界面-运动时间冒号
const uint8_t icon_16_move_runtime_dot_coord[2][2] = 
{
    {18 + 2 * ICON_MOVE_DATA2_WIDE, 85},
    {18 + 4 * ICON_MOVE_DATA2_WIDE + 1 * ICON_MOVE_DATA2DOT_WIDE, 85}
};
const uint32_t icon_16_move_runtime_dot_addr[1] = 
{
    ICON_MOVE_DATA2DOT_1_ADDR
};

// 运动模式-数据界面-卡路里数值
const uint8_t icon_16_move_calories_num_coord[4][2] = 
{
    {18, 125},
    {18 + 1 * ICON_MOVE_DATA3_WIDE, 125},
    {18 + 2 * ICON_MOVE_DATA3_WIDE, 125},
    {18 + 3 * ICON_MOVE_DATA3_WIDE, 125}
};
const uint32_t icon_16_move_calories_num_addr[10] = 
{
    ICON_MOVE_DATA3_0_ADDR, ICON_MOVE_DATA3_1_ADDR, ICON_MOVE_DATA3_2_ADDR,
    ICON_MOVE_DATA3_3_ADDR, ICON_MOVE_DATA3_4_ADDR, ICON_MOVE_DATA3_5_ADDR,
    ICON_MOVE_DATA3_6_ADDR, ICON_MOVE_DATA3_7_ADDR, ICON_MOVE_DATA3_8_ADDR,
    ICON_MOVE_DATA3_9_ADDR
};

// 运动模式-数据界面-卡路里单位
const uint8_t icon_16_move_calories_unit_coord[1][2] = 
{
    {143, 124},
};
const uint32_t icon_16_move_calories_unit_addr[2] = 
{
    ICON_MOVE_UNIT1_CN_ADDR, ICON_MOVE_UNIT1_EN_ADDR
};

// 心率
// 运动模式-数据界面-心率数值
const uint8_t icon_16_move_heart_num_coord[3][2] = 
{
    {18, 159},
    {18 + 1 * ICON_MOVE_DATA4_WIDE, 159},
    {18 + 2 * ICON_MOVE_DATA4_WIDE, 159}
};
const uint32_t icon_16_move_heart_num_addr[10] = 
{
    ICON_MOVE_DATA4_0_ADDR, ICON_MOVE_DATA4_1_ADDR, ICON_MOVE_DATA4_2_ADDR,
    ICON_MOVE_DATA4_3_ADDR, ICON_MOVE_DATA4_4_ADDR, ICON_MOVE_DATA4_5_ADDR,
    ICON_MOVE_DATA4_6_ADDR, ICON_MOVE_DATA4_7_ADDR, ICON_MOVE_DATA4_8_ADDR,
    ICON_MOVE_DATA4_9_ADDR
};

// 运动模式-数据界面-心率单位
const uint8_t icon_16_move_heart_unit_coord[1][2] = 
{
    {95, 157},
};
const uint32_t icon_16_move_heart_unit_addr[2] = 
{
    ICON_MOVE_UNIT2_CN_ADDR, ICON_MOVE_UNIT2_EN_ADDR
};

// 运动模式-数据界面-心率图标
const uint8_t icon_16_move_heart_icon_coord[1][2] = 
{
    {168, 159},
};
const uint32_t icon_16_move_heart_icon_addr[8] = 
{
    ICON_MOVE_ICON4_01_ADDR, ICON_MOVE_ICON4_02_ADDR, ICON_MOVE_ICON4_03_ADDR,
    ICON_MOVE_ICON4_04_ADDR, ICON_MOVE_ICON4_05_ADDR, ICON_MOVE_ICON4_06_ADDR,
    ICON_MOVE_ICON4_07_ADDR, ICON_MOVE_ICON4_08_ADDR
};

// 计步
// 运动模式-数据界面-计步数值
const uint8_t icon_16_move_step_num_coord[5][2] = 
{
    {18, 195},
    {18 + 1 * ICON_MOVE_DATA5_WIDE, 195},
    {18 + 2 * ICON_MOVE_DATA5_WIDE, 195},
    {18 + 3 * ICON_MOVE_DATA5_WIDE, 195},
    {18 + 4 * ICON_MOVE_DATA5_WIDE, 195}
};
const uint32_t icon_16_move_step_num_addr[10] = 
{
    ICON_MOVE_DATA5_0_ADDR, ICON_MOVE_DATA5_1_ADDR, ICON_MOVE_DATA5_2_ADDR,
    ICON_MOVE_DATA5_3_ADDR, ICON_MOVE_DATA5_4_ADDR, ICON_MOVE_DATA5_5_ADDR,
    ICON_MOVE_DATA5_6_ADDR, ICON_MOVE_DATA5_7_ADDR, ICON_MOVE_DATA5_8_ADDR,
    ICON_MOVE_DATA5_9_ADDR
};

// 运动模式-数据界面-计步单位
const uint8_t icon_16_move_step_unit_coord[1][2] = 
{
    {138, 194},
};
const uint32_t icon_16_move_step_unit_addr[2] = 
{
    ICON_MOVE_UNIT3_CN_ADDR, ICON_MOVE_UNIT3_EN_ADDR
};



//// 运动模式-暂停界面
//// 运动模式-暂停界面-上部标签
//const uint8_t icon_16_move_pause_label_coord[1][2] = {{77, 9}};
//const uint32_t icon_16_move_pause_label_addr[1] = 
//{
//    ICON_MOVE3_ICON4_ADDR
//};

// 运动模式-暂停界面-暂停图标
const uint8_t icon_16_move_pause_icon_coord[1][2] = {{45, 45}};
const uint32_t icon_16_move_pause_icon_addr[1] = 
{
    ICON_MOVE3_ICON1_ADDR
};


//// 运动模式-继续界面
//// 运动模式-继续界面-上部标签
//const uint8_t icon_16_move_goon_label_coord[1][2] = {{77, 9}};
//const uint32_t icon_16_move_goon_label_addr[1] = 
//{
//    ICON_MOVE2_ICON1_ADDR
//};

// 运动模式-继续界面-继续图标
const uint8_t icon_16_move_goon_icon_coord[1][2] = {{45, 45}};
const uint32_t icon_16_move_goon_icon_addr[1] = 
{
    ICON_MOVE2_ICON1_ADDR
};


//// 运动模式-退出界面
//// 运动模式-退出界面-上部标签
//const uint8_t icon_16_move_exit_label_coord[1][2] = {{77, 9}};
//const uint32_t icon_16_move_exit_label_addr[1] = 
//{
//    ICON_MOVE4_ICON2_ADDR
//};

// 运动模式-退出界面-继续图标
const uint8_t icon_16_move_exit_icon_coord[1][2] = {{45, 45}};
const uint32_t icon_16_move_exit_icon_addr[1] = 
{
    ICON_MOVE4_ICON1_ADDR
};


// 血压界面

// 血压界面-血压动图
const uint8_t icon_16_bp_dynamic_coord[1][2] = {{65, 27}};
const uint32_t icon_16_bp_dynamic_addr[4] = 
{
    ICON_BP_ICON_1_ADDR, ICON_BP_ICON_2_ADDR, ICON_BP_ICON_3_ADDR,
    ICON_BP_ICON_4_ADDR
};

// 血压界面-血压动图私人模式
const uint8_t icon_16_bp_p_icon_coord[1][2] = {{65, 27}};
const uint32_t icon_16_bp_p_icon_addr[4] = 
{
    ICON_BP_ICON1_1_ADDR, ICON_BP_ICON1_2_ADDR, ICON_BP_ICON1_3_ADDR,
    ICON_BP_ICON1_4_ADDR
};

// 血压界面-血压无数据
const uint8_t icon_16_bp_data_not_coord[1][2] = {{39, 151}};
const uint32_t icon_16_bp_data_not_addr[1] = 
{
    ICON_BP_ICON2_NULL_ADDR
};

// 血压界面-血压出错
const uint8_t icon_16_bp_data_err_coord[1][2] = {{39, 151}};
const uint32_t icon_16_bp_data_err_addr[1] = 
{
    ICON_BP_ICON2_ERROR_ADDR
};

// 血压界面-血压数据
const uint8_t icon_16_bp_data_coord[6][2] = 
{
    {47, 154},
    {47 + 1 * ICON_BP_DATA_WIDE, 154},
    {47 + 2 * ICON_BP_DATA_WIDE, 154},
    {130, 154},
    {130 + 1 * ICON_BP_DATA_WIDE, 154},
    {130 + 2 * ICON_BP_DATA_WIDE, 154},
};
const uint32_t icon_16_bp_data_addr[10] = 
{
    ICON_BP_DATA_0_ADDR, ICON_BP_DATA_1_ADDR, ICON_BP_DATA_2_ADDR,
    ICON_BP_DATA_3_ADDR, ICON_BP_DATA_4_ADDR, ICON_BP_DATA_5_ADDR,
    ICON_BP_DATA_6_ADDR, ICON_BP_DATA_7_ADDR, ICON_BP_DATA_8_ADDR,
    ICON_BP_DATA_9_ADDR
};

// 血压界面-血压斜杆
const uint8_t icon_16_bp_datadot_coord[1][2] = {{113, 151}};
const uint32_t icon_16_bp_datadot_addr[1] = 
{
    ICON_BP_DATADOT_ADDR
};

// 血压界面-血压单位
const uint8_t icon_16_bp_data_unit_coord[1][2] = {{89, 198}};
const uint32_t icon_16_bp_data_unit_addr[1] = 
{
    ICON_BP_UNIT_ADDR
};


// 心率界面
// 心率界面-背景
const uint8_t icon_16_heart_bg_coord[1][2] = 
{
    {9, 20}
};
const uint32_t icon_16_heart_bg_addr[1] = 
{
    ICON_HEART_ICON1_BG_ADDR,
};

// 心率界面-背景打点的点
const uint32_t icon_16_heart_bg_dot_addr[1] = 
{
    ICON_HEART_DATADOT_ADDR
};

// 心率界面-背景数字
const uint8_t icon_16_heart_bg_num_coord[3][2] = 
{
    {210, 34},
    {210, 65},
    {210, 94}
};
const uint32_t icon_16_heart_bg_num_addr[10] = 
{
    ICON_HEART_DATA1_0_ADDR, ICON_HEART_DATA1_1_ADDR, ICON_HEART_DATA1_2_ADDR,
    ICON_HEART_DATA1_3_ADDR, ICON_HEART_DATA1_4_ADDR, ICON_HEART_DATA1_5_ADDR,
    ICON_HEART_DATA1_6_ADDR, ICON_HEART_DATA1_7_ADDR, ICON_HEART_DATA1_8_ADDR,
    ICON_HEART_DATA1_9_ADDR
};

// 心率界面-标签
const uint8_t icon_16_heart_label_coord[1][2] = 
{
    {20, 128}
};
const uint32_t icon_16_heart_label_addr[2] = 
{
    ICON_HEART_ICON2_CN_ADDR, ICON_HEART_ICON2_EN_ADDR
};

// 心率界面-心率图标
const uint8_t icon_16_heart_icon_coord[1][2] = 
{
    {20, 158}
};
const uint32_t icon_16_heart_icon_addr[16] = 
{
    ICON_HEART_ICON4_01_ADDR, ICON_HEART_ICON4_02_ADDR, ICON_HEART_ICON4_03_ADDR,
    ICON_HEART_ICON4_04_ADDR, ICON_HEART_ICON4_05_ADDR, ICON_HEART_ICON4_06_ADDR,
    ICON_HEART_ICON4_07_ADDR, ICON_HEART_ICON4_08_ADDR, ICON_HEART_ICON4_09_ADDR,
    ICON_HEART_ICON4_10_ADDR, ICON_HEART_ICON4_11_ADDR, ICON_HEART_ICON4_12_ADDR,
    ICON_HEART_ICON4_13_ADDR, ICON_HEART_ICON4_14_ADDR, ICON_HEART_ICON4_15_ADDR,
    ICON_HEART_ICON4_16_ADDR
};

// 心率界面-数据
const uint8_t icon_16_heart_data_coord[3][2] = 
{
    {73, 159},
    {73 + 1 * ICON_HEART_DATA2_WIDE, 159},
    {73 + 2 * ICON_HEART_DATA2_WIDE, 159}
};
const uint32_t icon_16_heart_data_addr[10] = 
{
    ICON_HEART_DATA2_0_ADDR, ICON_HEART_DATA2_1_ADDR, ICON_HEART_DATA2_2_ADDR,
    ICON_HEART_DATA2_3_ADDR, ICON_HEART_DATA2_4_ADDR, ICON_HEART_DATA2_5_ADDR,
    ICON_HEART_DATA2_6_ADDR, ICON_HEART_DATA2_7_ADDR, ICON_HEART_DATA2_8_ADDR,
    ICON_HEART_DATA2_9_ADDR
};

// 心率界面-单位
const uint8_t icon_16_heart_unit_coord[1][2] = 
{
    {168, 159}
};
const uint32_t icon_16_heart_unit_addr[2] = 
{
    ICON_HEART_UNIT1_CN_ADDR, ICON_HEART_UNIT1_EN_ADDR
};

// 心率界面-上次检测
const uint8_t icon_16_heart_lastlabel_coord[1][2] = 
{
    {20, 204}
};
const uint32_t icon_16_heart_lastlabel_addr[2] = 
{
    ICON_HEART_ICON3_CN_ADDR, ICON_HEART_ICON3_EN_ADDR
};

// 心率界面-上次检测数据
const uint8_t icon_16_heart_lastdata_coord[3][2] = 
{
    {136, 205},
    {136 + 1 * ICON_HEART_DATA3_WIDE, 205},
    {136 + 2 * ICON_HEART_DATA3_WIDE, 205}
};
const uint32_t icon_16_heart_lastdata_addr[10] = 
{
    ICON_HEART_DATA3_0_ADDR, ICON_HEART_DATA3_1_ADDR, ICON_HEART_DATA3_2_ADDR,
    ICON_HEART_DATA3_3_ADDR, ICON_HEART_DATA3_4_ADDR, ICON_HEART_DATA3_5_ADDR,
    ICON_HEART_DATA3_6_ADDR, ICON_HEART_DATA3_7_ADDR, ICON_HEART_DATA3_8_ADDR,
    ICON_HEART_DATA3_9_ADDR
};

// 心率界面-单位
const uint8_t icon_16_heart_lastunit_coord[1][2] = 
{
    {176, 205}
};
const uint32_t icon_16_heart_lastunit_addr[2] = 
{
    ICON_HEART_UNIT2_CN_ADDR, ICON_HEART_UNIT2_EN_ADDR
};

// 心率异常界面
// 心率异常界面-背景
const uint8_t icon_16_heartalert_bg_coord[1][2] = 
{
    {26, 63}
};
const uint32_t icon_16_heartalert_bg_addr[1] = 
{
    ICON_HEARTALERT_ICON1_ADDR,
};

// 心率异常界面-图标
const uint8_t icon_16_heartalert_icon_coord[1][2] = 
{
    {112, 81}
};
const uint32_t icon_16_heartalert_icon_addr[8] = 
{
    ICON_HEARTALERT_ICON2_01_ADDR, ICON_HEARTALERT_ICON2_02_ADDR, ICON_HEARTALERT_ICON2_03_ADDR,
    ICON_HEARTALERT_ICON2_04_ADDR, ICON_HEARTALERT_ICON2_05_ADDR, ICON_HEARTALERT_ICON2_06_ADDR,
    ICON_HEARTALERT_ICON2_07_ADDR, ICON_HEARTALERT_ICON2_08_ADDR
};


// 睡眠界面 
// 睡眠界面-深睡图标
const uint8_t icon_16_sleep_deep_icon_coord[1][2] = 
{
    {22, 29}
};
const uint32_t icon_16_sleep_deep_icon_addr[1] = 
{
    ICON_SLEEP_ICON1_ADDR
};

// 睡眠界面-深睡背景
const uint8_t icon_16_sleep_deep_bg_coord[1][2] = 
{
    {59, 29}
};
const uint32_t icon_16_sleep_deep_bg_addr[1] = 
{
    ICON_SLEEP_ICON3_ADDR
};

// 睡眠界面-深睡标签
const uint8_t icon_16_sleep_deep_label_coord[1][2] = 
{
    {65, 34}
};
const uint32_t icon_16_sleep_deep_label_addr[2] = 
{
    ICON_SLEEP_ICON2_CN_ADDR, ICON_SLEEP_ICON2_EN_ADDR
};

// 睡眠界面-深睡数字
const uint8_t icon_16_sleep_deep_num_coord[4][2] = 
{
    {63, 64},
    {63 + ICON_SLEEP_DATA1_WIDE, 64},
    {63 + 2 * ICON_SLEEP_DATA1_WIDE + 4, 64},
    {63 + 3 * ICON_SLEEP_DATA1_WIDE + 4, 64},
};
const uint32_t icon_16_sleep_deep_num_addr[10] = 
{
    ICON_SLEEP_DATA1_01_ADDR, ICON_SLEEP_DATA1_02_ADDR, ICON_SLEEP_DATA1_03_ADDR,
    ICON_SLEEP_DATA1_04_ADDR, ICON_SLEEP_DATA1_05_ADDR, ICON_SLEEP_DATA1_06_ADDR,
    ICON_SLEEP_DATA1_07_ADDR, ICON_SLEEP_DATA1_08_ADDR, ICON_SLEEP_DATA1_09_ADDR,
    ICON_SLEEP_DATA1_10_ADDR
};

// 睡眠界面-深睡单位
const uint8_t icon_16_sleep_deep_unit_coord[2][2] = 
{
    {63 + 2 * ICON_SLEEP_DATA1_WIDE + 2, 64},
    {63 + 4 * ICON_SLEEP_DATA1_WIDE + 6, 64}
};
const uint32_t icon_16_sleep_deep_unit_addr[2][2] = 
{
    {
        ICON_SLEEP_UNIT1_ADDR, ICON_SLEEP_UNIT3_ADDR
    }
    ,
    {
        ICON_SLEEP_UNIT2_ADDR, ICON_SLEEP_UNIT4_ADDR
    }
};
const uint32_t icon_16_sleep_deep_unit_wide[2][2] = 
{
    {
        ICON_SLEEP_UNIT1_WIDE, ICON_SLEEP_UNIT3_WIDE
    }
    ,
    {
        ICON_SLEEP_UNIT2_WIDE, ICON_SLEEP_UNIT4_WIDE
    }
};
const uint32_t icon_16_sleep_deep_unit_high[2][2] = 
{
    {
        ICON_SLEEP_UNIT1_HIGH, ICON_SLEEP_UNIT3_HIGH
    }
    ,
    {
        ICON_SLEEP_UNIT2_HIGH, ICON_SLEEP_UNIT4_HIGH
    }
};
const uint32_t icon_16_sleep_deep_unit_size[2][2] = 
{
    {
        ICON_SLEEP_UNIT1_SIZE, ICON_SLEEP_UNIT3_SIZE
    }
    ,
    {
        ICON_SLEEP_UNIT2_SIZE, ICON_SLEEP_UNIT4_SIZE
    }
};

// 睡眠界面-浅睡图标
const uint8_t icon_16_sleep_light_icon_coord[1][2] = 
{
    {22, 97}
};
const uint32_t icon_16_sleep_light_icon_addr[1] = 
{
    ICON_SLEEP_ICON4_ADDR
};

// 睡眠界面-浅睡背景
const uint8_t icon_16_sleep_light_bg_coord[1][2] = 
{
    {59, 96}
};
const uint32_t icon_16_sleep_light_bg_addr[1] = 
{
    ICON_SLEEP_ICON6_ADDR
};

// 睡眠界面-浅睡标签
const uint8_t icon_16_sleep_light_label_coord[1][2] = 
{
    {65, 103}
};
const uint32_t icon_16_sleep_light_label_addr[2] = 
{
    ICON_SLEEP_ICON5_CN_ADDR, ICON_SLEEP_ICON5_EN_ADDR
};

// 睡眠界面-浅睡数字
const uint8_t icon_16_sleep_light_num_coord[4][2] = 
{
    {63, 132},
    {63 + ICON_SLEEP_DATA2_WIDE, 132},
    {63 + 2 * ICON_SLEEP_DATA2_WIDE + 4, 132},
    {63 + 3 * ICON_SLEEP_DATA2_WIDE + 4, 132},
};
const uint32_t icon_16_sleep_light_num_addr[10] = 
{
    ICON_SLEEP_DATA2_01_ADDR, ICON_SLEEP_DATA2_02_ADDR, ICON_SLEEP_DATA2_03_ADDR,
    ICON_SLEEP_DATA2_04_ADDR, ICON_SLEEP_DATA2_05_ADDR, ICON_SLEEP_DATA2_06_ADDR,
    ICON_SLEEP_DATA2_07_ADDR, ICON_SLEEP_DATA2_08_ADDR, ICON_SLEEP_DATA2_09_ADDR,
    ICON_SLEEP_DATA2_10_ADDR
};

// 睡眠界面-浅睡单位
const uint8_t icon_16_sleep_light_unit_coord[2][2] = 
{
    {63 + 2 * ICON_SLEEP_DATA2_WIDE + 2, 132},
    {63 + 4 * ICON_SLEEP_DATA2_WIDE + 6, 132}
};
const uint32_t icon_16_sleep_light_unit_addr[2][2] = 
{
    {
        ICON_SLEEP_UNIT5_ADDR, ICON_SLEEP_UNIT7_ADDR
    }
    ,
    {
        ICON_SLEEP_UNIT6_ADDR, ICON_SLEEP_UNIT8_ADDR
    }
};
const uint32_t icon_16_sleep_light_unit_wide[2][2] = 
{
    {
        ICON_SLEEP_UNIT5_WIDE, ICON_SLEEP_UNIT7_WIDE
    }
    ,
    {
        ICON_SLEEP_UNIT6_WIDE, ICON_SLEEP_UNIT8_WIDE
    }
};
const uint32_t icon_16_sleep_light_unit_high[2][2] = 
{
    {
        ICON_SLEEP_UNIT5_HIGH, ICON_SLEEP_UNIT7_HIGH
    }
    ,
    {
        ICON_SLEEP_UNIT6_HIGH, ICON_SLEEP_UNIT8_HIGH
    }
};
const uint32_t icon_16_sleep_light_unit_size[2][2] = 
{
    {
        ICON_SLEEP_UNIT5_SIZE, ICON_SLEEP_UNIT7_SIZE
    }
    ,
    {
        ICON_SLEEP_UNIT6_SIZE, ICON_SLEEP_UNIT8_SIZE
    }
};

// 睡眠界面-睡眠时长图
const uint8_t icon_16_sleep_time_icon_coord[1][2] = 
{
    {19, 167}
};
const uint32_t icon_16_sleep_time_icon_addr[2] = 
{
    ICON_SLEEP_ICON7_ADDR, ICON_SLEEP_ICON7_EN_ADDR
};

// 睡眠界面-睡数字
const uint8_t icon_16_sleep_num_coord[4][2] = 
{
    {108, 168},
    {108 + ICON_SLEEP_DATA3_WIDE, 168},
    {108 + 2 * ICON_SLEEP_DATA3_WIDE + 4, 168},
    {108 + 3 * ICON_SLEEP_DATA3_WIDE + 4, 168},
};
const uint32_t icon_16_sleep_num_addr[10] = 
{
    ICON_SLEEP_DATA3_01_ADDR, ICON_SLEEP_DATA3_02_ADDR, ICON_SLEEP_DATA3_03_ADDR,
    ICON_SLEEP_DATA3_04_ADDR, ICON_SLEEP_DATA3_05_ADDR, ICON_SLEEP_DATA3_06_ADDR,
    ICON_SLEEP_DATA3_07_ADDR, ICON_SLEEP_DATA3_08_ADDR, ICON_SLEEP_DATA3_09_ADDR,
    ICON_SLEEP_DATA3_10_ADDR
};

// 睡眠界面-睡眠单位
const uint8_t icon_16_sleep_unit_coord[2][2] = 
{
    {108 + 2 * ICON_SLEEP_DATA3_WIDE + 2, 168},
    {108 + 4 * ICON_SLEEP_DATA3_WIDE + 6, 168}
};
const uint32_t icon_16_sleep_unit_addr[2][2] = 
{
    {
        ICON_SLEEP_UNIT9_ADDR, ICON_SLEEP_UNIT11_ADDR
    }
    ,
    {
        ICON_SLEEP_UNIT10_ADDR, ICON_SLEEP_UNIT12_ADDR
    }
};
const uint32_t icon_16_sleep_unit_wide[2][2] = 
{
    {
        ICON_SLEEP_UNIT9_WIDE, ICON_SLEEP_UNIT11_WIDE
    }
    ,
    {
        ICON_SLEEP_UNIT10_WIDE, ICON_SLEEP_UNIT12_WIDE
    }
};
const uint32_t icon_16_sleep_unit_high[2][2] = 
{
    {
        ICON_SLEEP_UNIT9_HIGH, ICON_SLEEP_UNIT11_HIGH
    }
    ,
    {
        ICON_SLEEP_UNIT10_HIGH, ICON_SLEEP_UNIT12_HIGH
    }
};
const uint32_t icon_16_sleep_unit_size[2][2] = 
{
    {
        ICON_SLEEP_UNIT9_SIZE, ICON_SLEEP_UNIT11_SIZE
    }
    ,
    {
        ICON_SLEEP_UNIT10_SIZE, ICON_SLEEP_UNIT12_SIZE
    }
};

// 睡眠界面-质量图标
const uint8_t icon_16_sleep_quality_icon_coord[1][2] = 
{
    {19, 197}
};
const uint32_t icon_16_sleep_quality_icon_addr[2] = 
{
    ICON_SLEEP_ICON8_CN_ADDR, ICON_SLEEP_ICON8_EN_ADDR
};

// 睡眠界面-星星
const uint8_t icon_16_sleep_stars_icon_coord[5][2] = 
{
    {109, 194},
    {109 + 1 * ICON_SLEEP_ICON9_WIDE + 1, 194},
    {109 + 2 * ICON_SLEEP_ICON9_WIDE + 2, 194},
    {109 + 3 * ICON_SLEEP_ICON9_WIDE + 3, 194},
    {109 + 4 * ICON_SLEEP_ICON9_WIDE + 4, 194}
};
const uint32_t icon_16_sleep_stars_icon_addr[2] = 
{
    ICON_SLEEP_ICON9_ADDR, ICON_SLEEP_ICON10_ADDR
};


// 查找手机
// 查找手机-背景图标1
const uint8_t icon_16_findphone_bg1_coord[1][2] = {{49, 25}};
const uint32_t icon_16_findphone_bg1_addr[1] = 
{
    ICON_FINDPHONE_ICON1_ADDR
};

// 查找手机-背景图标2
const uint8_t icon_16_findphone_bg2_coord[1][2] = {{49, 133}};
const uint32_t icon_16_findphone_bg2_addr[1] = 
{
    ICON_FINDPHONE_ICON3_ADDR
};

// 查找手机-一级界面动图图标
const uint8_t icon_16_findphone_static_coord[1][2] = {{49, 71}};
const uint32_t icon_16_findphone_static_addr[4] = 
{
    ICON_FINDPHONE_ICON2_1_ADDR, ICON_FINDPHONE_ICON2_2_ADDR, ICON_FINDPHONE_ICON2_3_ADDR,
    ICON_FINDPHONE_ICON2_4_ADDR
};

// 查找手机-语言单位
const uint8_t icon_16_findphone_unit_coord[1][2] = {{57, 203}};
const uint32_t icon_16_findphone_unit_addr[2] = 
{
    ICON_FINDPHONE_ICON4_2_ADDR, ICON_FINDPHONE_ICON4_1_ADDR
};

// 查找手机-2级界面-中静态图标
const uint8_t icon_16_findphone2_center_coord[1][2] = {{79, 35}};
const uint32_t icon_16_findphone2_center_addr[1] = 
{
    ICON_FINDPHONE2_ICON9_ADDR
};

// 查找手机-2级界面-左动态图标
const uint8_t icon_16_findphone2_left_coord[1][2] = {{40, 86}};
const uint32_t icon_16_findphone2_left_addr[4] = 
{
    ICON_FINDPHONE2_ICON1_ADDR, ICON_FINDPHONE2_ICON2_ADDR, ICON_FINDPHONE2_ICON3_ADDR,
    ICON_FINDPHONE2_ICON4_ADDR
};

// 查找手机-2级界面-右动态图标
const uint8_t icon_16_findphone2_right_coord[1][2] = {{168, 84}};
const uint32_t icon_16_findphone2_right_addr[4] = 
{
    ICON_FINDPHONE2_ICON5_ADDR, ICON_FINDPHONE2_ICON6_ADDR, ICON_FINDPHONE2_ICON7_ADDR,
    ICON_FINDPHONE2_ICON8_ADDR
};


// 秒表
// 秒表-进入界面
const uint8_t icon_16_stopwatch_entry_coord[1][2] = {{49, 55}};
const uint32_t icon_16_stopwatch_entry_addr[1] = 
{
    ICON_STOPWATCH_ICON4_ADDR
};

// 秒表-进入界面
const uint8_t icon_16_stopwatch_entrylabel_coord[1][2] = {{50, 203}};
const uint32_t icon_16_stopwatch_entrylabel_addr[2] = 
{
    ICON_STOPWATCH_ICON5_CN_ADDR, ICON_STOPWATCH_ICON5_EN_ADDR
};

// 秒表-数据界面-秒表图标
const uint8_t icon_16_stopwatch_icon_coord[1][2] = {{82, 28}};
const uint32_t icon_16_stopwatch_icon_addr[11] = 
{
    
	ICON_STOPWATCH_ICON1_01_ADDR, ICON_STOPWATCH_ICON1_02_ADDR, ICON_STOPWATCH_ICON1_03_ADDR,
	ICON_STOPWATCH_ICON1_04_ADDR, ICON_STOPWATCH_ICON1_05_ADDR, ICON_STOPWATCH_ICON1_06_ADDR,
	ICON_STOPWATCH_ICON1_07_ADDR, ICON_STOPWATCH_ICON1_08_ADDR, ICON_STOPWATCH_ICON1_09_ADDR,
	ICON_STOPWATCH_ICON1_10_ADDR, ICON_STOPWATCH_ICON1_11_ADDR
};


// 秒表-数据界面-暂停继续图标
const uint8_t icon_16_stopwatch_pause_coord[1][2] = {{91, 171}};
const uint32_t icon_16_stopwatch_pause_addr[2] = 
{
    ICON_STOPWATCH_ICON3_1_ADDR, ICON_STOPWATCH_ICON3_2_ADDR
};

// 秒表-数据界面-时间-分钟
const uint8_t icon_16_stopwatch_data1_coord[2][2] = 
{
    {25, 120},
    {25 + 1 * ICON_STOPWATCH_DATA_WIDE, 120}
};
const uint32_t icon_16_stopwatch_data1_addr[10] = 
{
    ICON_STOPWATCH_DATA_0_ADDR, ICON_STOPWATCH_DATA_1_ADDR, ICON_STOPWATCH_DATA_2_ADDR,
    ICON_STOPWATCH_DATA_3_ADDR, ICON_STOPWATCH_DATA_4_ADDR, ICON_STOPWATCH_DATA_5_ADDR,
    ICON_STOPWATCH_DATA_6_ADDR, ICON_STOPWATCH_DATA_7_ADDR, ICON_STOPWATCH_DATA_8_ADDR,
    ICON_STOPWATCH_DATA_9_ADDR
};

// 秒表-数据界面-时间-秒
const uint8_t icon_16_stopwatch_data2_coord[2][2] = 
{
    {83 + 0 * ICON_STOPWATCH_DATA_WIDE + ICON_STOPWATCH_DATADOT_WIDE + 2,120},
    {83 + 1 * ICON_STOPWATCH_DATA_WIDE + ICON_STOPWATCH_DATADOT_WIDE + 2,120}
};
const uint32_t icon_16_stopwatch_data2_addr[10] = 
{
    ICON_STOPWATCH_DATA_0_ADDR, ICON_STOPWATCH_DATA_1_ADDR, ICON_STOPWATCH_DATA_2_ADDR,
    ICON_STOPWATCH_DATA_3_ADDR, ICON_STOPWATCH_DATA_4_ADDR, ICON_STOPWATCH_DATA_5_ADDR,
    ICON_STOPWATCH_DATA_6_ADDR, ICON_STOPWATCH_DATA_7_ADDR, ICON_STOPWATCH_DATA_8_ADDR,
    ICON_STOPWATCH_DATA_9_ADDR
};

// 秒表-数据界面-时间-毫秒
const uint8_t icon_16_stopwatch_data3_coord[2][2] = 
{
    {150 + 0 * ICON_STOPWATCH_DATA_WIDE + ICON_STOPWATCH_DATADOT_WIDE + 2,120},
    {150 + 1 * ICON_STOPWATCH_DATA_WIDE + ICON_STOPWATCH_DATADOT_WIDE + 2,120}
};

const uint32_t icon_16_stopwatch_data3_addr[10] = 
{
    ICON_STOPWATCH_DATA_0_ADDR, ICON_STOPWATCH_DATA_1_ADDR, ICON_STOPWATCH_DATA_2_ADDR,
    ICON_STOPWATCH_DATA_3_ADDR, ICON_STOPWATCH_DATA_4_ADDR, ICON_STOPWATCH_DATA_5_ADDR,
    ICON_STOPWATCH_DATA_6_ADDR, ICON_STOPWATCH_DATA_7_ADDR, ICON_STOPWATCH_DATA_8_ADDR,
    ICON_STOPWATCH_DATA_9_ADDR
};

// 秒表-数据界面-时间-冒号
const uint8_t icon_16_stopwatch_datadot_coord[2][2] = 
{
    {83 , 120},
	{150, 120}
};
const uint32_t icon_16_stopwatch_datadot_addr[1] = 
{
    ICON_STOPWATCH_DATADOT_ADDR
};


// 倒计时界面
// 倒计时-静止图标

const uint8_t icon_16_countdown_entry_coord[1][2] = 
{
    {49 , 55}
};
const uint32_t icon_16_countdown_entry_addr[1] = 
{
    ICON_COUNTDOWN_ICON4_ADDR
};

const uint8_t icon_16_countdown_entrylabel_coord[1][2] = 
{
    {46 , 203}
};
const uint32_t icon_16_countdown_entrylabel_addr[2] = 
{
    ICON_COUNTDOWN_ICON5_CN_ADDR, ICON_COUNTDOWN_ICON5_EN_ADDR
};

// 倒计时-动态图标


const uint8_t icon_16_countdown_dynamic_coord[1][2] = 
{
    {82 , 28}
};
const uint32_t icon_16_countdown_dynamic_addr[9] = 
{
    ICON_COUNTDOWN_ICON1_1_ADDR, ICON_COUNTDOWN_ICON1_2_ADDR, ICON_COUNTDOWN_ICON1_3_ADDR,
    ICON_COUNTDOWN_ICON1_4_ADDR, ICON_COUNTDOWN_ICON1_5_ADDR, ICON_COUNTDOWN_ICON1_6_ADDR,
    ICON_COUNTDOWN_ICON1_7_ADDR, ICON_COUNTDOWN_ICON1_8_ADDR, ICON_COUNTDOWN_ICON1_9_ADDR,
};

// 倒计时-时间-小时
const uint8_t icon_16_countdown_data1_coord[2][2] = 
{
    {25, 120},
    {25 + 1 * ICON_COUNTDOWN_DATA_WIDE, 120}
};
const uint32_t icon_16_countdown_data1_addr[10] = 
{
    ICON_COUNTDOWN_DATA_0_ADDR, ICON_COUNTDOWN_DATA_1_ADDR, ICON_COUNTDOWN_DATA_2_ADDR,
    ICON_COUNTDOWN_DATA_3_ADDR, ICON_COUNTDOWN_DATA_4_ADDR, ICON_COUNTDOWN_DATA_5_ADDR,
    ICON_COUNTDOWN_DATA_6_ADDR, ICON_COUNTDOWN_DATA_7_ADDR, ICON_COUNTDOWN_DATA_8_ADDR,
    ICON_COUNTDOWN_DATA_9_ADDR
};

// 倒计时-时间-分钟
const uint8_t icon_16_countdown_data2_coord[2][2] = 
{
    {83 + ICON_COUNTDOWN_DATADOT_WIDE + 2, 120},
    {83 + ICON_COUNTDOWN_DATADOT_WIDE + 2 + ICON_COUNTDOWN_DATA_WIDE, 120}
};
const uint32_t icon_16_countdown_data2_addr[10] = 
{
    ICON_COUNTDOWN_DATA_0_ADDR, ICON_COUNTDOWN_DATA_1_ADDR, ICON_COUNTDOWN_DATA_2_ADDR,
    ICON_COUNTDOWN_DATA_3_ADDR, ICON_COUNTDOWN_DATA_4_ADDR, ICON_COUNTDOWN_DATA_5_ADDR,
    ICON_COUNTDOWN_DATA_6_ADDR, ICON_COUNTDOWN_DATA_7_ADDR, ICON_COUNTDOWN_DATA_8_ADDR,
    ICON_COUNTDOWN_DATA_9_ADDR
};

// 倒计时-时间-秒
const uint8_t icon_16_countdown_data3_coord[2][2] = 
{
    {150 + ICON_COUNTDOWN_DATADOT_WIDE + 2, 120},
    {150 + ICON_COUNTDOWN_DATADOT_WIDE + 2 + ICON_COUNTDOWN_DATA_WIDE, 120}
};
const uint32_t icon_16_countdown_data3_addr[10] = 
{
    ICON_COUNTDOWN_DATA_0_ADDR, ICON_COUNTDOWN_DATA_1_ADDR, ICON_COUNTDOWN_DATA_2_ADDR,
    ICON_COUNTDOWN_DATA_3_ADDR, ICON_COUNTDOWN_DATA_4_ADDR, ICON_COUNTDOWN_DATA_5_ADDR,
    ICON_COUNTDOWN_DATA_6_ADDR, ICON_COUNTDOWN_DATA_7_ADDR, ICON_COUNTDOWN_DATA_8_ADDR,
    ICON_COUNTDOWN_DATA_9_ADDR
};

// 倒计时-时间-冒号
const uint8_t icon_16_countdown_datadot_coord[2][2] = 
{
    { 83, 120},
    {150, 120}
};
const uint32_t icon_16_countdown_datadot_addr[1] = 
{
    ICON_COUNTDOWN_DATADOT_ADDR
};

// 倒计时-开始图标
const uint8_t icon_16_countdown_start_icon_coord[1][2] = 
{
    {91 ,171},
};
const uint32_t icon_16_countdown_start_icon_addr[2] = 
{
    ICON_COUNTDOWN_ICON3_1_ADDR, ICON_COUNTDOWN_ICON3_2_ADDR
};


// 开机界面
// 开机界面-图标
const uint8_t icon_16_poweron_icon_coord[1][2] = {{44, 98}};
const uint32_t icon_16_poweron_icon_addr[15] = 
{
    ICON_POWERON_ICON_01_ADDR, ICON_POWERON_ICON_02_ADDR, ICON_POWERON_ICON_03_ADDR,
    ICON_POWERON_ICON_04_ADDR, ICON_POWERON_ICON_05_ADDR, ICON_POWERON_ICON_06_ADDR,
    ICON_POWERON_ICON_07_ADDR, ICON_POWERON_ICON_08_ADDR, ICON_POWERON_ICON_09_ADDR,
    ICON_POWERON_ICON_10_ADDR, ICON_POWERON_ICON_11_ADDR, ICON_POWERON_ICON_12_ADDR,
    ICON_POWERON_ICON_13_ADDR, ICON_POWERON_ICON_14_ADDR, ICON_POWERON_ICON_15_ADDR,
};


// 关机界面
// 关机界面-无物理按键时
const uint8_t icon_16_poweroff_nokey_icon_coord[1][2] = {{60, 37}};
const uint32_t icon_16_poweroff_nokey_icon_addr[1] = 
{
    5942532//ICON_ZZPOWEROFF_ICON1_ADDR
};

// 关机界面-图标
const uint8_t icon_16_poweroff_icon_coord[1][2] = {{36, 102}};
const uint32_t icon_16_poweroff_icon_addr[15] = 
{
    ICON_POWEROFF_ICON_01_ADDR, ICON_POWEROFF_ICON_02_ADDR, ICON_POWEROFF_ICON_03_ADDR,
    ICON_POWEROFF_ICON_04_ADDR, ICON_POWEROFF_ICON_05_ADDR, ICON_POWEROFF_ICON_06_ADDR,
    ICON_POWEROFF_ICON_07_ADDR, ICON_POWEROFF_ICON_08_ADDR, ICON_POWEROFF_ICON_09_ADDR,
    ICON_POWEROFF_ICON_10_ADDR, ICON_POWEROFF_ICON_11_ADDR, ICON_POWEROFF_ICON_12_ADDR,
    ICON_POWEROFF_ICON_13_ADDR, ICON_POWEROFF_ICON_14_ADDR, ICON_POWEROFF_ICON_15_ADDR,
};


// 充电界面
// 充电界面-背景
const uint8_t icon_16_charge_bg_coord[1][2] = {{11, 11}};
const uint32_t icon_16_charge_bg_addr[1] = 
{
    ICON_CHG_BG1_ADDR
};

// 充电界面-充满
const uint8_t icon_16_charge_full_coord[1][2] = {{91, 62}};
const uint32_t icon_16_charge_full_addr[1] = 
{
    ICON_CHG_BG2_ADDR
};

// 充电界面-图标
const uint8_t icon_16_charge_icon_coord[1][2] = {{91, 62}};
const uint32_t icon_16_charge_icon_addr[4][3] = 
{
    {
        ICON_CHG_ICON1_01_ADDR, ICON_CHG_ICON1_02_ADDR, ICON_CHG_ICON1_03_ADDR,
    }
    ,
    {
        ICON_CHG_ICON2_01_ADDR, ICON_CHG_ICON2_02_ADDR, ICON_CHG_ICON2_03_ADDR,
    }
    ,
    {
        ICON_CHG_ICON3_01_ADDR, ICON_CHG_ICON3_02_ADDR, ICON_CHG_ICON3_03_ADDR,
    }
    ,
    {
        ICON_CHG_ICON4_01_ADDR, ICON_CHG_ICON4_02_ADDR, ICON_CHG_ICON4_03_ADDR,
    }
};


// 闹钟界面
// 闹钟界面-图标
const uint8_t icon_16_alarm_icon_coord[1][2] = {{70, 32}};
const uint32_t icon_16_alarm_icon_addr[4] = 
{
    ICON_CLOCK_ICON_1_ADDR, ICON_CLOCK_ICON_2_ADDR, ICON_CLOCK_ICON_3_ADDR,
    ICON_CLOCK_ICON_4_ADDR
};

// 闹钟界面-时间
const uint8_t icon_16_alarm_time_coord[4][2] = 
{
    {79, 159},
    {79 + 1 * ICON_CLOCK_DATA_WIDE, 159},
    {79 + 2 * ICON_CLOCK_DATA_WIDE + ICON_CLOCK_DATADOT_WIDE, 159},
    {79 + 3 * ICON_CLOCK_DATA_WIDE + ICON_CLOCK_DATADOT_WIDE, 159}
};
const uint32_t icon_16_alarm_time_addr[10] = 
{
    ICON_CLOCK_DATA_0_ADDR, ICON_CLOCK_DATA_1_ADDR, ICON_CLOCK_DATA_2_ADDR,
    ICON_CLOCK_DATA_3_ADDR, ICON_CLOCK_DATA_4_ADDR, ICON_CLOCK_DATA_5_ADDR,
    ICON_CLOCK_DATA_6_ADDR, ICON_CLOCK_DATA_7_ADDR, ICON_CLOCK_DATA_8_ADDR,
    ICON_CLOCK_DATA_9_ADDR
};

// 闹钟界面-时间冒号
const uint8_t icon_16_alarm_timedot_coord[1][2] = 
{
    {117, 159}
};
const uint32_t icon_16_alarm_timedot_addr[1] = 
{
    ICON_CLOCK_DATADOT_ADDR
};

// 闹钟界面-上下午
const uint8_t icon_16_alarm_ampm_coord[1][2] = {{100, 190}};
const uint32_t icon_16_alarm_ampm_addr[5] = 
{
    ICON_CLOCK_UNIT_AM_CN_ADDR, ICON_CLOCK_UNIT_PM_CN_ADDR,
    ICON_CLOCK_UNIT_AM_EN_ADDR, ICON_CLOCK_UNIT_PM_EN_ADDR,
    ICON_CLOCK_UNIT_BG_ADDR
};

// 闹钟界面-情景图标
const uint8_t icon_16_alarm_scene_coord[1][2] = {{53, 68}};
const uint32_t icon_16_alarm_scene_addr[20] = 
{
    ICON_REMINDCLOCK_ICON_07_ADDR, ICON_REMINDCLOCK_ICON_02_ADDR, ICON_REMINDCLOCK_ICON_01_ADDR,
    ICON_REMINDCLOCK_ICON_12_ADDR, ICON_REMINDCLOCK_ICON_03_ADDR, ICON_REMINDCLOCK_ICON_04_ADDR,
    ICON_REMINDCLOCK_ICON_05_ADDR, ICON_REMINDCLOCK_ICON_06_ADDR, ICON_REMINDCLOCK_ICON_08_ADDR,
    ICON_REMINDCLOCK_ICON_09_ADDR, ICON_REMINDCLOCK_ICON_10_ADDR, ICON_REMINDCLOCK_ICON_11_ADDR,
    ICON_REMINDCLOCK_ICON_13_ADDR, ICON_REMINDCLOCK_ICON_14_ADDR, ICON_REMINDCLOCK_ICON_15_ADDR,
    ICON_REMINDCLOCK_ICON_16_ADDR, ICON_REMINDCLOCK_ICON_17_ADDR, ICON_REMINDCLOCK_ICON_18_ADDR,
    ICON_REMINDCLOCK_ICON_19_ADDR, ICON_REMINDCLOCK_ICON_20_ADDR
};


// 久坐界面
// 久坐界面-图标
const uint8_t icon_16_sit_icon_coord[1][2] = {{80, 36}};
const uint32_t icon_16_sit_icon_addr[14] = 
{
    ICON_SIT_ICON1_1_ADDR, ICON_SIT_ICON1_2_ADDR, ICON_SIT_ICON1_3_ADDR,
    ICON_SIT_ICON1_4_ADDR, ICON_SIT_ICON1_5_ADDR, ICON_SIT_ICON1_6_ADDR,
    ICON_SIT_ICON1_7_ADDR, ICON_SIT_ICON1_8_ADDR, ICON_SIT_ICON1_9_ADDR,
    ICON_SIT_ICON1_10_ADDR, ICON_SIT_ICON1_11_ADDR, ICON_SIT_ICON1_12_ADDR,
    ICON_SIT_ICON1_14_ADDR
};

const uint8_t icon_16_sit_unit_coord[1][2] = {{84, 183}};
const uint32_t icon_16_sit_unit_addr[2] = 
{
    ICON_SIT_ICON2_CN_ADDR, ICON_SIT_ICON2_EN_ADDR
};


// 来电界面
// 来电界面-图标
const uint8_t icon_16_call_man_coord[1][2] = 
{
    {74, 10}
};
const uint32_t icon_16_call_man_addr[1] = 
{
    ICON_CALL_ICON1_ADDR
};

// 提示语
const uint8_t icon_16_call_label_coord[1][2] = 
{
	{84, 147}
};
const uint32_t icon_16_call_label_addr[2][2] = 
{
    {
        ICON_CALL_ICON4_1_CN_ADDR, ICON_CALL_ICON4_2_CN_ADDR
    }
    ,
    {
        ICON_CALL_ICON4_1_EN_ADDR, ICON_CALL_ICON4_2_EN_ADDR
    }
};

// 提示图标
const uint8_t icon_16_call_icon_coord[2][2] = 
{
    {57, 164},{134, 164}
};
const uint32_t icon_16_call_icon_addr[2][2] = 
{
    {
        ICON_CALL_ICON2_2_ADDR, ICON_CALL_ICON2_1_ADDR
    }
    ,
    {
        ICON_CALL_ICON3_1_ADDR, ICON_CALL_ICON3_2_ADDR
    }
};

// 提示挂断图标提示语
const uint8_t icon_16_call_rejectlabel_coord[1][2] = 
{
    {44, 216}
};
const uint32_t icon_16_call_rejectlabel_addr[2] = 
{
	ICON_CALL_ICON5_CN_ADDR, ICON_CALL_ICON5_EN_ADDR
};

// 提示静音图标提示语
const uint8_t icon_16_call_mutelabel_coord[1][2] = 
{
    {123, 216}
};
const uint32_t icon_16_call_mutelabel_addr[2] = 
{
	ICON_CALL_ICON6_CN_ADDR, ICON_CALL_ICON6_EN_ADDR
};



// 信息提醒界面
// 信息提醒-背景
const uint8_t icon_16_inform_bg_coord[8][2] = 
{
    { 0, 11}, { 93, 11}, { 137, 11},
    { 225, 35}, { 0, 75}, { 0, 194},
    { 148, 194}, { 0, 217}
};
const uint32_t icon_16_inform_bg_addr[5] = 
{
    ICON_INFORM_BACKGROUND1_ADDR, ICON_INFORM_BACKGROUND2_ADDR, ICON_INFORM_BACKGROUND3_ADDR,
    ICON_INFORM_BACKGROUND4_ADDR, ICON_INFORM_BACKGROUND5_ADDR
};

// 信息提醒-图标
const uint8_t icon_16_inform_icon_coord[1][2] = {{30, 11}};
const uint32_t icon_16_inform_icon_addr[14] = 
{
    ICON_INFORM_ICON1_012_ADDR, ICON_INFORM_ICON1_02_ADDR, ICON_INFORM_ICON1_03_ADDR, ICON_INFORM_ICON1_04_ADDR,
    ICON_INFORM_ICON1_05_ADDR, ICON_INFORM_ICON1_06_ADDR, ICON_INFORM_ICON1_07_ADDR, ICON_INFORM_ICON1_08_ADDR,
    ICON_INFORM_ICON1_09_ADDR, ICON_INFORM_ICON1_010_ADDR, ICON_INFORM_ICON1_011_ADDR, ICON_INFORM_ICON1_01_ADDR,
    ICON_INFORM_ICON1_013_ADDR,ICON_INFORM_ICON1_014_ADDR
};

// 信息提醒-图标2
const uint8_t icon_16_inform_icon2_coord[1][2] = {{137, 35}};
const uint32_t icon_16_inform_icon2_addr[2][14] = 
{
    {
        ICON_INFORM_ICON2_12_CN_ADDR, ICON_INFORM_ICON2_02_CN_ADDR, ICON_INFORM_ICON2_03_ADDR, ICON_INFORM_ICON2_04_ADDR,
        ICON_INFORM_ICON2_05_ADDR, ICON_INFORM_ICON2_06_ADDR, ICON_INFORM_ICON2_07_ADDR, ICON_INFORM_ICON2_08_ADDR,
        ICON_INFORM_ICON2_09_ADDR, ICON_INFORM_ICON2_10_ADDR, ICON_INFORM_ICON2_11_ADDR, ICON_INFORM_ICON2_01_ADDR,
        ICON_INFORM_ICON2_13_CN_ADDR, ICON_INFORM_ICON2_14_CN_ADDR
    }
    ,
    {
        ICON_INFORM_ICON2_12_ADDR, ICON_INFORM_ICON2_02_ADDR, ICON_INFORM_ICON2_03_ADDR, ICON_INFORM_ICON2_04_ADDR,
        ICON_INFORM_ICON2_05_ADDR, ICON_INFORM_ICON2_06_ADDR, ICON_INFORM_ICON2_07_ADDR, ICON_INFORM_ICON2_08_ADDR,
        ICON_INFORM_ICON2_09_ADDR, ICON_INFORM_ICON2_10_ADDR, ICON_INFORM_ICON2_11_ADDR, ICON_INFORM_ICON2_01_ADDR,
        ICON_INFORM_ICON2_13_ADDR, ICON_INFORM_ICON2_14_ADDR
    }
};


// 拍照界面
// 拍照界面-大图标
const uint8_t icon_16_camera_icon1_coord[1][2] = {{64, 38}};
const uint32_t icon_16_camera_icon1_addr[8] = 
{
    ICON_CAM_ICON4_1_ADDR, ICON_CAM_ICON4_2_ADDR, ICON_CAM_ICON4_3_ADDR,
    ICON_CAM_ICON4_4_ADDR, ICON_CAM_ICON4_5_ADDR, ICON_CAM_ICON4_6_ADDR,
    ICON_CAM_ICON4_7_ADDR, ICON_CAM_ICON4_8_ADDR
};


// 拍照界面
// 拍照界面-小图标
const uint8_t icon_16_camera_icon2_coord[1][2] = 
{	
	{92, 168}
};
const uint32_t icon_16_camera_icon2_addr[2] = 
{
    ICON_CAM_ICON5_1_ADDR, ICON_CAM_ICON5_2_ADDR
};


// 目标提醒界面
// 目标提醒界面-上部图标
const uint8_t icon_16_starget_icon2_coord[1][2] = {{40, 67}};
const uint32_t icon_16_starget_icon2_addr[2][5] = 
{
    {
        ICON_STANDARD_ICON3_1_ADDR, ICON_STANDARD_ICON3_2_ADDR, ICON_STANDARD_ICON3_3_ADDR,
        ICON_STANDARD_ICON3_4_ADDR, ICON_STANDARD_ICON3_5_ADDR
    }
    ,
    {
        ICON_STANDARD_ICON4_1_ADDR, ICON_STANDARD_ICON4_2_ADDR, ICON_STANDARD_ICON4_3_ADDR,
        ICON_STANDARD_ICON4_4_ADDR, ICON_STANDARD_ICON4_5_ADDR
    }
};

// 目标提醒界面
// 目标提醒界面-下部图标
const uint8_t icon_16_starget_icon1_coord[1][2] = {{76, 132}};
const uint32_t icon_16_starget_icon1_addr[10] = 
{
    ICON_STANDARD_ICON1_01_ADDR, ICON_STANDARD_ICON1_02_ADDR, ICON_STANDARD_ICON1_03_ADDR,
    ICON_STANDARD_ICON1_04_ADDR, ICON_STANDARD_ICON1_05_ADDR, ICON_STANDARD_ICON1_06_ADDR,
    ICON_STANDARD_ICON1_07_ADDR, ICON_STANDARD_ICON1_08_ADDR, ICON_STANDARD_ICON1_09_ADDR,
    ICON_STANDARD_ICON1_10_ADDR
};


// 手机蓝牙断联界面
// 手机蓝牙断联界面-图标
const uint8_t icon_16_blue_disc_coord[1][2] = {{12, 67}};
const uint32_t icon_16_blue_disc_addr[1] = 
{
    ICON_DISCONNECTED_ICON1_ADDR
};

const uint8_t icon_16_blue_dynamic_coord[1][2] = {{72, 106}};
const uint32_t icon_16_blue_dynamic_addr[6] = 
{
    ICON_DISCONNECTED_ICON2_1_ADDR, ICON_DISCONNECTED_ICON2_2_ADDR, ICON_DISCONNECTED_ICON2_3_ADDR,
    ICON_DISCONNECTED_ICON2_4_ADDR, ICON_DISCONNECTED_ICON2_5_ADDR, ICON_DISCONNECTED_ICON2_6_ADDR
};


// 低电提醒界面
// 低电提醒-背景
const uint8_t icon_16_lowpower_bg_coord[1][2] = {{11, 11}};
const uint32_t icon_16_lowpower_bg_addr[1] = 
{
    ICON_CHG_BG1_ADDR
};

// 低电提醒-图标
const uint8_t icon_16_lowpower_icon_coord[1][2] = {{91, 62}};
const uint32_t icon_16_lowpower_icon_addr[4] = 
{
    ICON_CHG_ICON5_01_ADDR, ICON_CHG_ICON5_02_ADDR, ICON_CHG_ICON5_03_ADDR,
    ICON_CHG_ICON5_04_ADDR
};

// 长按进入提示语-图标
const uint8_t icon_16_long_press_icon_coord[1][2] = {{79, 183}};
const uint32_t icon_16_long_press_icon_addr[2] = 
{
    ICON_FINDPHONE_ICON5_2_ADDR, ICON_FINDPHONE_ICON5_1_ADDR
};


// 信息提醒界面--02
// 信息提醒02-图标
const uint8_t icon_16_inform02_icon_coord[1][2] = {{7, 6}};
const uint32_t icon_16_inform02_icon_addr[14] = 
{
    ICON_ZINFO2_ICON1_01_ADDR, ICON_ZINFO2_ICON1_02_ADDR, ICON_ZINFO2_ICON1_03_ADDR,
	ICON_ZINFO2_ICON1_04_ADDR, ICON_ZINFO2_ICON1_05_ADDR, ICON_ZINFO2_ICON1_06_ADDR,
	ICON_ZINFO2_ICON1_07_ADDR, ICON_ZINFO2_ICON1_08_ADDR, ICON_ZINFO2_ICON1_09_ADDR,
	ICON_ZINFO2_ICON1_10_ADDR, ICON_ZINFO2_ICON1_11_ADDR, ICON_ZINFO2_ICON1_12_ADDR,
	ICON_ZINFO2_ICON1_13_ADDR, ICON_ZINFO2_ICON1_14_ADDR
};

// 信息提醒02-动图1
const uint8_t icon_16_inform02_dynamic_coord[1][2] = {{74, 6}};
const uint32_t icon_16_inform02_dynamic_addr[15] = 
{
    ICON_ZINFO2_ICON2_01_ADDR, ICON_ZINFO2_ICON2_02_ADDR, ICON_ZINFO2_ICON2_03_ADDR,
	ICON_ZINFO2_ICON2_04_ADDR, ICON_ZINFO2_ICON2_05_ADDR, ICON_ZINFO2_ICON2_06_ADDR,
	ICON_ZINFO2_ICON2_07_ADDR, ICON_ZINFO2_ICON2_08_ADDR, ICON_ZINFO2_ICON2_09_ADDR,
	ICON_ZINFO2_ICON2_10_ADDR, ICON_ZINFO2_ICON2_10_ADDR, ICON_ZINFO2_ICON2_10_ADDR,
	ICON_ZINFO2_ICON2_10_ADDR, ICON_ZINFO2_ICON2_10_ADDR, ICON_ZINFO2_ICON2_10_ADDR
};

// 信息提醒02-图标01
const uint8_t icon_16_inform02_icon01_coord[1][2] = {{141, 6}};
const uint32_t icon_16_inform02_icon01_addr[1] = 
{
    ICON_ZINFO2_ICON4_ADDR
};

// 信息提醒02-图标02
const uint8_t icon_16_inform02_icon02_coord[1][2] = {{143, 22}};
const uint32_t icon_16_inform02_icon02_addr[1] = 
{
    ICON_ZINFO2_ICON3_ADDR
};

// 
// 
// 
