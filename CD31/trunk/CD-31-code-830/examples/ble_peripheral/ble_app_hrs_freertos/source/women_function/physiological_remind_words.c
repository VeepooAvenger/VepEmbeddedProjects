/***************************Copyright(c)***********************************
**                       Veepoo Tech
**                  COPYRIGHT (C) 2016-2019
**-----------File Info-----------------------------------------------------
** File        name:     physiological_remind_words.c
** Last modified Date:   2017-10-24
** Last SYS_VERSION:     0.0.0
** Descriptions:            
**-------------------------------------------------------------------------
** Created  by:          WJ
** Created  date:        2017-10-24
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
#include <stdlib.h>
#include "physiological_period.h"
#include "util.h"

/*********************************************************************
* Macro Definition
*/

/*********************************************************************
* Global Variables
*/  
//月经期提示语
const char menstrual_words_ch_a[] = {0xe4,0xbb,0x8a,0xe5,0xa4,0xa9,0xe7,0xbb,0x8f,0xe6,0x9c,0x9f,0xe7,0xac,0xac,  0x00};
const char menstrual_words_ch_b[] = {0xe5,0xa4,0xa9,  0x00};
const char menstrual_words_ch_c[] = {0xe8,0xaf,0xb7,0xe5,0x8b,0xbf,0xe4,0xb9,0x85,0xe5,0x9d,0x90,  0x00};
const char menstrual_words_ch_d[] = {0x2c,0xe8,0xae,0xb0,0xe5,0xbe,0x97,0xe6,0x9b,0xb4,0xe6,0x8d,0xa2,0xe5,0x8d,0xab,0xe7,0x94,0x9f,0xe5,0xb7,0xbe,  0x00};
const char menstrual_words_ch_e[] = {0xe4,0xb8,0x8b,0xe7,0x8f,0xad,0xe5,0x89,0x8d,0xe8,0xae,0xb0,0xe5,0xbe,0x97,0xe6,0x9b,0xb4,0xe6,0x8d,0xa2,0xe5,0x8d,0xab,0xe7,0x94,0x9f,0xe5,0xb7,0xbe,  0x00};
const char menstrual_words_ch_e1[] = {0xe7,0x9d,0xa1,0xe5,0x89,0x8d,0xe6,0x9b,0xb4,0xe6,0x8d,0xa2,0xe5,0x8d,0xab,0xe7,0x94,0x9f,0xe5,0xb7,0xbe,0x2c,0xe6,0xb3,0xa8,0xe6,0x84,0x8f,0xe7,0x9d,0xa1,0xe7,0x9c,0xa0,0xe8,0xb4,0xa8,0xe9,0x87,0x8f,  0x00};

const char menstrual_words_ch_f[] = {0xe4,0xbb,0x8a,0xe5,0xa4,0xa9,0xe5,0xae,0x89,0xe5,0x85,0xa8,0xe6,0x9c,0x9f,0x2c,0xe8,0xb7,0x9d,0xe4,0xb8,0x8b,0xe6,0xac,0xa1,0xe7,0xbb,0x8f,0xe6,0x9c,0x9f,  0x00};
const char menstrual_words_ch_g[] = {0xe4,0xbb,0x8a,0xe5,0xa4,0xa9,0xe6,0x98,0xaf,0xe6,0x8e,0x92,0xe5,0x8d,0xb5,0xe6,0x9c,0x9f,0xe7,0xac,0xac,  0x00};
const char menstrual_words_ch_h[] = {0x2c,0xe5,0xb0,0x8f,0xe5,0xbf,0x83,0xe5,0x93,0xa6,  0x00};
const char menstrual_words_ch_i[] = {0xe4,0xbb,0x8a,0xe5,0xa4,0xa9,0xe6,0x98,0xaf,0xe6,0x8e,0x92,0xe5,0x8d,0xb5,0xe6,0x97,0xa5,0x2c,0xe8,0xa6,0x81,0xe6,0xa0,0xbc,0xe5,0xa4,0x96,0xe5,0xb0,0x8f,0xe5,0xbf,0x83,0xe5,0x93,0xa6,  0x00};

const char menstrual_words_ch_j[] = {0xe5,0xa4,0xa9,0xe5,0x90,0x8e,0xe8,0xbf,0x9b,0xe5,0x85,0xa5,0xe7,0xbb,0x8f,0xe6,0x9c,0x9f,0x2c,0xe8,0xaf,0xb7,0xe5,0x81,0x9a,0xe5,0xa5,0xbd,0xe5,0x87,0x86,0xe5,0xa4,0x87,  0x00};
const char menstrual_words_ch_k[] = {0xe9,0xa2,0x84,0xe6,0xb5,0x8b,0xe4,0xbb,0x8a,0xe5,0xa4,0xa9,0xe8,0xbf,0x9b,0xe5,0x85,0xa5,0xe7,0xbb,0x8f,0xe6,0x9c,0x9f,0x2c,0xe8,0xaf,0xb7,0xe6,0x9b,0xb4,0xe6,0x96,0xb0,0xe4,0xbf,0xa1,0xe6,0x81,0xaf,  0x00};
const char menstrual_words_ch_l[] = {0xe6,0x9c,0xac,0xe6,0xac,0xa1,0xe6,0x9c,0x88,0xe7,0xbb,0x8f,0xe5,0xbb,0xb6,0xe8,0xbf,0x9f,  0x00};
const char menstrual_words_ch_m[] = {0x2c,0xe8,0xaf,0xb7,0xe6,0x9b,0xb4,0xe6,0x96,0xb0,0xe4,0xbf,0xa1,0xe6,0x81,0xaf,  0x00};
const char menstrual_words_ch_n[] = {0xe4,0xbf,0xa1,0xe6,0x81,0xaf,0xe9,0x94,0x99,0xe8,0xaf,0xaf,0x2c,0xe8,0xaf,0xb7,0xe6,0x9b,0xb4,0xe6,0x96,0xb0,  0x00};
//备孕期提示语
const char pregnant_words_ch_a[] = {0xe4,0xbb,0x8a,0xe5,0xa4,0xa9,0xe6,0x8e,0x92,0xe5,0x8d,0xb5,0xe6,0x9c,0x9f,  0x00};
const char pregnant_words_ch_b[] = {0xe4,0xbb,0x8a,0xe5,0xa4,0xa9,0xe6,0x8e,0x92,0xe5,0x8d,0xb5,0xe6,0x97,0xa5,  0x00};
const char pregnant_words_ch_c[] = {0x2c,0xe6,0x80,0x80,0xe5,0xad,0x95,0xe6,0x9c,0xba,0xe7,0x8e,0x87,0xe8,0xbe,0x83,0xe5,0xa4,0xa7,  0x00};
//怀孕期提示语
const char gestation_words_ch_a[] = {0xe6,0x82,0xa8,0xe5,0xb7,0xb2,0xe6,0x80,0x80,0xe5,0xad,0x95,  0x00};
const char gestation_words_ch_b[] = {0xe5,0x91,0xa8,  0x00};
const char gestation_words_ch_c[] = {0x2c,0xe8,0xaf,0xb7,0xe5,0xae,0x9a,0xe6,0x9c,0x9f,0xe5,0x81,0x9a,0xe6,0xa3,0x80,0xe6,0x9f,0xa5,0x00};
const char gestation_words_ch_d[] = {0x2c,0xe8,0xaf,0xb7,0xe5,0xae,0x9a,0xe6,0x9c,0x9f,0xe5,0x81,0x9a,0xe6,0xa3,0x80,0xe6,0x9f,0xa5,  0x00};
const char gestation_words_ch_e[] = {0xe8,0xb7,0x9d,0xe5,0xae,0x9d,0xe5,0xae,0x9d,0xe8,0xaf,0x9e,0xe7,0x94,0x9f,  0x00};
const char gestation_words_ch_f[] = {0xe9,0xa2,0x84,0xe7,0xa5,0x9d,0xe5,0xae,0x9d,0xe5,0xae,0x9d,0xe4,0xbb,0x8a,0xe5,0xa4,0xa9,0xe8,0xaf,0x9e,0xe7,0x94,0x9f,0xe6,0x82,0xa8,0xe5,0x87,0x86,0xe5,0xa4,0x87,0xe5,0xa5,0xbd,0xe4,0xba,0x86,0xe5,0x90,0x97,  0x00};
//辣妈期提示语
const char hot_mom_words_ch_a[] = {0xe5,0xae,0x9d,0xe5,0xae,0x9d,0xe5,0x87,0xba,0xe7,0x94,0x9f,0xe7,0xac,0xac,  0x00};
const char hot_mom_words_ch_b[] = {0x2c,0xe6,0x81,0xa2,0xe5,0xa4,0x8d,0xe6,0x9c,0x9f,0xe6,0xb3,0xa8,0xe6,0x84,0x8f,0xe5,0x8d,0xab,0xe7,0x94,0x9f,  0x00};
const char hot_mom_words_ch_c[] = {0xe5,0xae,0x9d,0xe5,0xae,0x9d,0xe6,0xbb,0xa1,0xe6,0x9c,0x88,0xe4,0xba,0x86,  0x00};
const char hot_mom_words_ch_d[] = {0xe5,0xae,0x9d,0xe5,0xae,0x9d,0xe4,0xb8,0x80,0xe7,0x99,0xbe,0xe5,0xa4,0xa9,0xe4,0xba,0x86,  0x00};
const char hot_mom_words_ch_e[] = {0xe5,0xae,0x9d,0xe5,0xae,0x9d,0xe4,0xb8,0x80,0xe5,0x91,0xa8,0xe5,0xb2,0x81,0xe4,0xba,0x86,  0x00};
const char hot_mom_words_ch_f[] = {0x2c,0xe5,0x80,0xbc,0xe5,0xbe,0x97,0xe7,0xba,0xaa,0xe5,0xbf,0xb5,0xe7,0x9a,0x84,0xe4,0xb8,0x80,0xe5,0xa4,0xa9,  0x00};
const char hot_mom_words_ch_g[] = {0xe8,0xaf,0xb7,0xe6,0x9b,0xb4,0xe6,0x96,0xb0,0xe6,0x82,0xa8,0xe7,0x9a,0x84,0xe4,0xb8,0xaa,0xe4,0xba,0xba,0xe4,0xbf,0xa1,0xe6,0x81,0xaf,  0x00};

/*********************************************************************
* local functions
*/
void Phys_ChinesRemind(uint8_t type, uint16_t days, uint8_t hours, uint8_t minutes)
{
    if(type == PHYS_REMIND_MENSTRUAL_PERIOD)
    {//经期
        if(hours == 7 && minutes == 30)
        {//今天经期第x天，记得换卫生巾
            //"今天经期第"
            memcpy(phys_remind_info_s.remind_info_buf, menstrual_words_ch_a, strlen(menstrual_words_ch_a));
            phys_remind_info_s.data_size = strlen(menstrual_words_ch_a);
            //days 
            Util_Itoa(days, (char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size]);
            phys_remind_info_s.data_size = strlen((const char *)phys_remind_info_s.remind_info_buf);
            //天
            memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], menstrual_words_ch_b, strlen(menstrual_words_ch_b));            
            phys_remind_info_s.data_size += strlen(menstrual_words_ch_b);
            //,记得更换卫生巾
            memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], menstrual_words_ch_d, strlen(menstrual_words_ch_d));            
            phys_remind_info_s.data_size += strlen(menstrual_words_ch_d);
        }
        else if((hours == 11 || hours == 14) && minutes == 00)
        {//请勿久坐,记得更换卫生巾
            //请勿久坐
            memcpy(phys_remind_info_s.remind_info_buf, menstrual_words_ch_c, strlen(menstrual_words_ch_c));
            phys_remind_info_s.data_size = strlen(menstrual_words_ch_c);
            //，记得更换卫生巾
            memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], menstrual_words_ch_d, strlen(menstrual_words_ch_d));            
            phys_remind_info_s.data_size += strlen(menstrual_words_ch_d);
        }
        else if(hours == 17 && minutes == 30)
        {//下班前记得更换卫生巾 
            memcpy(phys_remind_info_s.remind_info_buf, menstrual_words_ch_e, strlen(menstrual_words_ch_e));
            phys_remind_info_s.data_size = strlen(menstrual_words_ch_e);
        }
        else if(hours == 21 && minutes == 30)
        {//睡前更换卫生巾,注意睡眠质量
            memcpy(phys_remind_info_s.remind_info_buf, menstrual_words_ch_e1, strlen(menstrual_words_ch_e1));
            phys_remind_info_s.data_size = strlen(menstrual_words_ch_e1);
        }
    }
    else if(type == PHYS_REMIND_SAFETY_PERIOD)
    {//安全期
        if(hours == 9 && minutes == 0)
        {//今天安全期，距下次经期X天
            //今天安全期，距下次经期
            memcpy(phys_remind_info_s.remind_info_buf, menstrual_words_ch_f, strlen(menstrual_words_ch_f));
            phys_remind_info_s.data_size = strlen(menstrual_words_ch_f);
            //days 天
            Util_Itoa(days, (char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size]);
            phys_remind_info_s.data_size = strlen((const char *)phys_remind_info_s.remind_info_buf);
            //天
            memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], menstrual_words_ch_b, strlen(menstrual_words_ch_b));            
            phys_remind_info_s.data_size += strlen(menstrual_words_ch_b);
        }
    }
    else if(type == PHYS_REMIND_SAFETY_PERIOD1)
    {//安全期
        if(hours == 9 && minutes == 0)
        {//X天后进入经期(<=3天)，请做好准备
            //days 
            Util_Itoa(days, (char *)phys_remind_info_s.remind_info_buf);
            phys_remind_info_s.data_size = strlen((const char *)phys_remind_info_s.remind_info_buf);
            //天后进入经期，请做好准备
            memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], menstrual_words_ch_j, strlen(menstrual_words_ch_j));            
            phys_remind_info_s.data_size += strlen(menstrual_words_ch_j);   
        }
    }
    else if(type == PHYS_REMIND_OVIPOSIT_PERIOD)
    {//排卵期
        if(hours == 9 && minutes == 0)
        {//今天是排卵期第X天，小心哦
            //今天是排卵期第
            memcpy(phys_remind_info_s.remind_info_buf, menstrual_words_ch_g, strlen(menstrual_words_ch_g));
            phys_remind_info_s.data_size = strlen(menstrual_words_ch_g);
            //days 
            Util_Itoa(days, (char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size]);
            phys_remind_info_s.data_size = strlen((const char *)phys_remind_info_s.remind_info_buf);
            //天
            memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], menstrual_words_ch_b, strlen(menstrual_words_ch_b));            
            phys_remind_info_s.data_size += strlen(menstrual_words_ch_b);
            //，小心哦
            memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], menstrual_words_ch_h, strlen(menstrual_words_ch_h));            
            phys_remind_info_s.data_size += strlen(menstrual_words_ch_h);
        }
    }
    else if(type == PHYS_REMIND_OVIPOSIT_DAY)
    {//排卵日
        if(hours == 9 && minutes == 0)
        {//今天是排卵日，要格外小心哦
            memcpy(phys_remind_info_s.remind_info_buf, menstrual_words_ch_i, strlen(menstrual_words_ch_i));
            phys_remind_info_s.data_size = strlen(menstrual_words_ch_i);
        }
    }
    else if(type == PHYS_REMIND_OVIPOSIT_PERIOD1)
    {//备孕期排卵期
        if(hours == 9 && minutes == 0)
        {//今天排卵期，怀孕几率较大
            //今天排卵期，
            memcpy(phys_remind_info_s.remind_info_buf, pregnant_words_ch_a, strlen(pregnant_words_ch_a));
            phys_remind_info_s.data_size = strlen(pregnant_words_ch_a);
            //怀孕几率较大
            memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], pregnant_words_ch_c, strlen(pregnant_words_ch_c));            
            phys_remind_info_s.data_size += strlen(pregnant_words_ch_c);
        }
    }
    else if(type == PHYS_REMIND_OVIPOSIT_DAY1)
    {//备孕期排卵日
        if(hours == 9 && minutes == 0)
        {//今天排卵日，怀孕几率较大
            //今天排卵日，
            memcpy(phys_remind_info_s.remind_info_buf, pregnant_words_ch_b, strlen(pregnant_words_ch_b));
            phys_remind_info_s.data_size = strlen(pregnant_words_ch_b);
            //怀孕几率较大
            memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], pregnant_words_ch_c, strlen(pregnant_words_ch_c));            
            phys_remind_info_s.data_size += strlen(pregnant_words_ch_c);
        }
    }
    else if(type == PHYS_REMIND_FORECAST_MENSTRUAL_PERIOD)
    {//预测经期
        if(hours == 7 && minutes == 30 && days == 1)
        {//预测今天进入经期，请更新信息
            memcpy(phys_remind_info_s.remind_info_buf, menstrual_words_ch_k, strlen(menstrual_words_ch_k));
            phys_remind_info_s.data_size = strlen(menstrual_words_ch_k);
        } 
        else if(hours == 9 && minutes == 0 && days > 1)
        {//本次月经延迟X天，请更新信息
            //本次月经延迟
            memcpy(phys_remind_info_s.remind_info_buf, menstrual_words_ch_l, strlen(menstrual_words_ch_l));
            phys_remind_info_s.data_size = strlen(menstrual_words_ch_l);
            //days 
            Util_Itoa(days, (char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size]);
            phys_remind_info_s.data_size = strlen((const char *)phys_remind_info_s.remind_info_buf);
            //天
            memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], menstrual_words_ch_b, strlen(menstrual_words_ch_b));            
            phys_remind_info_s.data_size += strlen(menstrual_words_ch_b);
            //，请更新信息
            memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], menstrual_words_ch_m, strlen(menstrual_words_ch_m));            
            phys_remind_info_s.data_size += strlen(menstrual_words_ch_m);
        } 
    }
    else if(type == PHYS_REMIND_PREGNANCY_ONE)
    {//怀孕第一阶段（1~3个月）
        if(hours == 9 && minutes == 0)
        {//您已怀孕X周X天，请定期做检查
            //您已怀孕
            memcpy(phys_remind_info_s.remind_info_buf, gestation_words_ch_a, strlen(gestation_words_ch_a));
            phys_remind_info_s.data_size = strlen(gestation_words_ch_a);
            //n周
            if(days / 7 != 0)
            {
                //days/7 
                Util_Itoa(days / 7, (char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size]);
                phys_remind_info_s.data_size = strlen((const char *)phys_remind_info_s.remind_info_buf);
                memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], gestation_words_ch_b, strlen(gestation_words_ch_b));            
                phys_remind_info_s.data_size += strlen(gestation_words_ch_b);
            }
            //n天
            if(days % 7 != 0)
            {
                //days%7 
                Util_Itoa(days % 7, (char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size]);
                phys_remind_info_s.data_size = strlen((const char *)phys_remind_info_s.remind_info_buf);
                memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], menstrual_words_ch_b, strlen(menstrual_words_ch_b));            
                phys_remind_info_s.data_size += strlen(menstrual_words_ch_b);
            }
            //,请定期做检查
            memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], gestation_words_ch_c, strlen(gestation_words_ch_c));            
            phys_remind_info_s.data_size += strlen(gestation_words_ch_c);
        } 
    }
    else if(type == PHYS_REMIND_PREGNANCY_TWO)
    {//怀孕第二阶段（3~6个月）
        if(hours == 9 && minutes == 0)
        {//09：00您已怀孕X周X天，请保持好心情
            //您已怀孕
            memcpy(phys_remind_info_s.remind_info_buf, gestation_words_ch_a, strlen(gestation_words_ch_a));
            phys_remind_info_s.data_size = strlen(gestation_words_ch_a);
            //n周
            if(days / 7 != 0)
            {
                //days/7 
                Util_Itoa(days / 7, (char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size]);
                phys_remind_info_s.data_size = strlen((const char *)phys_remind_info_s.remind_info_buf);
                memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], gestation_words_ch_b, strlen(gestation_words_ch_b));            
                phys_remind_info_s.data_size += strlen(gestation_words_ch_b);
            }
            //n天
            if(days % 7 != 0)
            {
                //days%7 
                Util_Itoa(days % 7, (char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size]);
                phys_remind_info_s.data_size = strlen((const char *)phys_remind_info_s.remind_info_buf);
                memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], menstrual_words_ch_b, strlen(menstrual_words_ch_b));            
                phys_remind_info_s.data_size += strlen(menstrual_words_ch_b);
            }
            //,请定期做检查
            memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], gestation_words_ch_d, strlen(gestation_words_ch_d));            
            phys_remind_info_s.data_size += strlen(gestation_words_ch_d);
        }
    }
    else if(type == PHYS_REMIND_PREGNANCY_THU)
    {//怀孕第三阶段（6个月~出生前）
        if(hours == 9 && minutes == 0)
        {//09：00距宝宝诞生X天，请保持好心情
            //距宝宝诞生
            memcpy(phys_remind_info_s.remind_info_buf, gestation_words_ch_e, strlen(gestation_words_ch_e));
            phys_remind_info_s.data_size = strlen(gestation_words_ch_e);
            //n天
            Util_Itoa(days, (char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size]);
            phys_remind_info_s.data_size = strlen((const char *)phys_remind_info_s.remind_info_buf);
            memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], menstrual_words_ch_b, strlen(menstrual_words_ch_b));            
            phys_remind_info_s.data_size += strlen(menstrual_words_ch_b);
            
            //,请定期做检查
            memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], gestation_words_ch_d, strlen(gestation_words_ch_d));            
            phys_remind_info_s.data_size += strlen(gestation_words_ch_d);
        }
    }
    else if(type == PHYS_REMIND_PREGNANCY_BIRTH)
    {//怀孕第四阶段（宝宝出生）
        if(hours == 9 && minutes == 0)
        {//预祝宝宝今天诞生您准备好了吗
            memcpy(phys_remind_info_s.remind_info_buf, gestation_words_ch_f, strlen(gestation_words_ch_f));
            phys_remind_info_s.data_size = strlen(gestation_words_ch_f);
        }
    }
    else if(type == PHYS_REMIND_PREGNANCY_ERROR)
    {//怀孕信息过期，更新信息
        if(hours == 9 && minutes == 0)
        {//09：00请更新您的个人信息(如果经期信息有效,该信息不提醒)
            memcpy(phys_remind_info_s.remind_info_buf, hot_mom_words_ch_g, strlen(hot_mom_words_ch_g));
            phys_remind_info_s.data_size = strlen(hot_mom_words_ch_g);
        }
    }
    else if(type == PHYS_REMIND_BABY_BORN)
    {//宝宝出生后
        if(hours == 9 && minutes == 30)
        {
            if(days <= 10)
            {//宝宝出生第X天，恢复期注意卫生
                //宝宝出生第
                memcpy(phys_remind_info_s.remind_info_buf, hot_mom_words_ch_a, strlen(hot_mom_words_ch_a));
                phys_remind_info_s.data_size = strlen(hot_mom_words_ch_a);
                //days 
                Util_Itoa(days, (char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size]);
                phys_remind_info_s.data_size = strlen((const char *)phys_remind_info_s.remind_info_buf);
                //天
                memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], menstrual_words_ch_b, strlen(menstrual_words_ch_b));            
                phys_remind_info_s.data_size += strlen(menstrual_words_ch_b);
                //，请恢复期注意卫生
                memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], hot_mom_words_ch_b, strlen(hot_mom_words_ch_b));            
                phys_remind_info_s.data_size += strlen(hot_mom_words_ch_b);
            }
            else if(days == 30)
            {//宝宝满月(30天)了，值得纪念的一天              
                memcpy(phys_remind_info_s.remind_info_buf, hot_mom_words_ch_c, strlen(hot_mom_words_ch_c));
                phys_remind_info_s.data_size = strlen(hot_mom_words_ch_c); 
                memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], hot_mom_words_ch_f, strlen(hot_mom_words_ch_f));            
                phys_remind_info_s.data_size += strlen(hot_mom_words_ch_f); 
            }
            else if(days == 100)
            {//宝宝一百天，值得纪念的一天
                memcpy(phys_remind_info_s.remind_info_buf, hot_mom_words_ch_d, strlen(hot_mom_words_ch_d));
                phys_remind_info_s.data_size = strlen(hot_mom_words_ch_d); 
                
                memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], hot_mom_words_ch_f, strlen(hot_mom_words_ch_f));            
                phys_remind_info_s.data_size += strlen(hot_mom_words_ch_f);
            }
            else if(days == 365)
            {//宝宝一周岁，值得纪念的一天
                memcpy(phys_remind_info_s.remind_info_buf, hot_mom_words_ch_e, strlen(hot_mom_words_ch_e));
                phys_remind_info_s.data_size = strlen(hot_mom_words_ch_e); 
                memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], hot_mom_words_ch_f, strlen(hot_mom_words_ch_f));            
                phys_remind_info_s.data_size += strlen(hot_mom_words_ch_f);
            }
            else if(days < 365)
            {//宝宝出生第x天
                //宝宝出生第
                memcpy(phys_remind_info_s.remind_info_buf, hot_mom_words_ch_a, strlen(hot_mom_words_ch_a));
                phys_remind_info_s.data_size = strlen(hot_mom_words_ch_a);
                //days 
                Util_Itoa(days, (char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size]);
                phys_remind_info_s.data_size = strlen((const char *)phys_remind_info_s.remind_info_buf);
                //天
                memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], menstrual_words_ch_b, strlen(menstrual_words_ch_b));            
                phys_remind_info_s.data_size += strlen(menstrual_words_ch_b);
            }
            else
            {//请更新您的个人信息(如果经期信息有效,该信息不提醒)
                memcpy(phys_remind_info_s.remind_info_buf, hot_mom_words_ch_g, strlen(hot_mom_words_ch_g));
                phys_remind_info_s.data_size = strlen(hot_mom_words_ch_g);
            }
        }
    }
    else
    {//非法
        if(hours == 9 && minutes == 0)
        {//09：00信息错误，请更新
            memcpy(phys_remind_info_s.remind_info_buf, menstrual_words_ch_n, strlen(menstrual_words_ch_n));
            phys_remind_info_s.data_size = strlen(menstrual_words_ch_n);  
        }
    }     
}





