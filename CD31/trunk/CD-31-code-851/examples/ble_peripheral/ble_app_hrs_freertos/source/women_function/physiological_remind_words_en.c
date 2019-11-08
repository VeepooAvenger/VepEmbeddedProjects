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
const char menstrual_words_en_a[] = " day, please change sanitary pad regularly";
const char menstrual_words_en_b[] = "Have a rest and change sanitary pad";
const char menstrual_words_en_c[] = "Please change sanitary pad before work off";
const char menstrual_words_en_d[] = "Please change sanitary pad and have a good night";

const char menstrual_words_en_e[] = "Safe period, ";
const char menstrual_words_en_f[] = " days to next period";
const char menstrual_words_en_g[] = " days be in period";

const char menstrual_words_en_h[] = " days of Ovulation period, careful！";
const char menstrual_words_en_i[] = "Today is Ovulation period, careful！";

const char menstrual_words_en_j[] = "Today is predicted to be the first day of Menstruation, please update information";
const char menstrual_words_en_k[] = "Menstruation delayed for ";
const char menstrual_words_en_l[] = "days, update it in calendar";
const char menstrual_words_en_m[] = "Error information, please update";
//备孕期提示语
const char pregnant_words_en_a[] = "In Ovulation period,Greater chances of getting pregnant";
const char pregnant_words_en_b[] = "In Ovulation period,Greater chances of getting pregnant";
//怀孕期提示语
const char gestation_words_en_a[] = "Pregnant for ";
const char gestation_words_en_b[] = " weeks ";
const char gestation_words_en_c[] = " days";

const char gestation_words_en_d[] = ", please antenatal examination regularly";
const char gestation_words_en_e[] = ", please antenatal examination regularly";
const char gestation_words_en_f[] = "Are you ready for delivery today?";
//辣妈期提示语
const char hot_mom_words_en_a[] = "This is the ";
const char hot_mom_words_en_b[] = " day of baby's birth, Pay attention to the hygiene in recovery Period";
const char hot_mom_words_en_c[] = "Baby is 30 days from birth, A memorable day";
const char hot_mom_words_en_d[] = "Baby is 100 days from birth, A memorable day";
const char hot_mom_words_en_e[] = "Baby is 1 year from birth, A memorable day";
const char hot_mom_words_en_f[] = " day of baby's birth";
const char hot_mom_words_en_g[] = "please update your personal information";


/*********************************************************************
* local functions
*/

void Phys_EnglishRemind(uint8_t type, uint16_t days, uint8_t hours, uint8_t minutes)
{
    if(type == PHYS_REMIND_MENSTRUAL_PERIOD)
    {//经期
        if(hours == 7 && minutes == 30)
        {//今天经期第x天，记得换卫生巾
            //days 
            Util_Itoa(days, (char *)phys_remind_info_s.remind_info_buf);
            phys_remind_info_s.data_size = strlen((const char *)phys_remind_info_s.remind_info_buf);
            //day, please change sanitary pad regularly
            memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], menstrual_words_en_a, strlen(menstrual_words_en_a));            
            phys_remind_info_s.data_size += strlen(menstrual_words_en_a);
        }
        else if((hours == 11 || hours == 14) && minutes == 00)
        {//请勿久坐,记得更换卫生巾
            //Have a rest and change sanitary pad
            memcpy(phys_remind_info_s.remind_info_buf, menstrual_words_en_b, strlen(menstrual_words_en_b));
            phys_remind_info_s.data_size = strlen(menstrual_words_en_b);
        }
        else if(hours == 17 && minutes == 30)
        {//下班前记得更换卫生巾 
            //Please change sanitary pad before work off
            memcpy(phys_remind_info_s.remind_info_buf, menstrual_words_en_c, strlen(menstrual_words_en_c));
            phys_remind_info_s.data_size = strlen(menstrual_words_en_c);
        }
        else if(hours == 21 && minutes == 30)
        {//睡前更换卫生巾,注意睡眠质量
            //Please change sanitary pad and have a good night
            memcpy(phys_remind_info_s.remind_info_buf, menstrual_words_en_d, strlen(menstrual_words_en_d));
            phys_remind_info_s.data_size = strlen(menstrual_words_en_d);
        }
    }
    else if(type == PHYS_REMIND_SAFETY_PERIOD)
    {//安全期
        if(hours == 9 && minutes == 0)
        {//今天安全期，距下次经期X天
            //Safe period, 
            memcpy(phys_remind_info_s.remind_info_buf, menstrual_words_en_e, strlen(menstrual_words_en_e));
            phys_remind_info_s.data_size = strlen(menstrual_words_en_e);
            //days 
            Util_Itoa(days, (char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size]);
            phys_remind_info_s.data_size = strlen((const char *)phys_remind_info_s.remind_info_buf);
            //days to next period
            memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], menstrual_words_en_f, strlen(menstrual_words_en_f));            
            phys_remind_info_s.data_size += strlen(menstrual_words_en_f);
        }
    }
    else if(type == PHYS_REMIND_SAFETY_PERIOD1)
    {//安全期
        if(hours == 9 && minutes == 0)
        {//X天后进入经期(<=3天)，请做好准备
            //days 
            Util_Itoa(days, (char *)phys_remind_info_s.remind_info_buf);
            phys_remind_info_s.data_size = strlen((const char *)phys_remind_info_s.remind_info_buf);
            //days be in period
            memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], menstrual_words_en_g, strlen(menstrual_words_en_g));            
            phys_remind_info_s.data_size += strlen(menstrual_words_en_g);   
        }
    }
    else if(type == PHYS_REMIND_OVIPOSIT_PERIOD)
    {//排卵期
        if(hours == 9 && minutes == 0)
        {//今天是排卵期第X天，小心哦
            //days 
            Util_Itoa(days, (char *)phys_remind_info_s.remind_info_buf);
            phys_remind_info_s.data_size = strlen((const char *)phys_remind_info_s.remind_info_buf);
            //days of Ovulation period, careful！
            memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], menstrual_words_en_h, strlen(menstrual_words_en_h));            
            phys_remind_info_s.data_size += strlen(menstrual_words_en_h);
        }
    }
    else if(type == PHYS_REMIND_OVIPOSIT_DAY)
    {//排卵日
        if(hours == 9 && minutes == 0)
        {//今天是排卵日，要格外小心哦
            //Today is Ovulation period, careful！
            memcpy(phys_remind_info_s.remind_info_buf, menstrual_words_en_i, strlen(menstrual_words_en_i));
            phys_remind_info_s.data_size = strlen(menstrual_words_en_i);
        }
    }
    else if(type == PHYS_REMIND_OVIPOSIT_PERIOD1)
    {//备孕期排卵期
        if(hours == 9 && minutes == 0)
        {//今天排卵期，怀孕几率较大
            //In Ovulation period,Greater chances of getting pregnant
            memcpy(phys_remind_info_s.remind_info_buf, pregnant_words_en_a, strlen(pregnant_words_en_a));
            phys_remind_info_s.data_size = strlen(pregnant_words_en_a);
        }
    }
    else if(type == PHYS_REMIND_OVIPOSIT_DAY1)
    {//备孕期排卵日
        if(hours == 9 && minutes == 0)
        {//今天排卵日，怀孕几率较大
            //In Ovulation period,Greater chances of getting pregnant.
            memcpy(phys_remind_info_s.remind_info_buf, pregnant_words_en_b, strlen(pregnant_words_en_b));
            phys_remind_info_s.data_size = strlen(pregnant_words_en_b);
        }
    }
    else if(type == PHYS_REMIND_FORECAST_MENSTRUAL_PERIOD)
    {//预测经期
        if(hours == 7 && minutes == 30 && days == 1)
        {//预测今天进入经期，请更新信息
            //Today is predicted to be the first day of Menstruation, please update information
            memcpy(phys_remind_info_s.remind_info_buf, menstrual_words_en_j, strlen(menstrual_words_en_j));
            phys_remind_info_s.data_size = strlen(menstrual_words_en_j);
        } 
        else if(hours == 9 && minutes == 0 && days > 1)
        {//本次月经延迟X天，请更新信息
            //Menstruation delayed for 
            memcpy(phys_remind_info_s.remind_info_buf, menstrual_words_en_k, strlen(menstrual_words_en_k));
            phys_remind_info_s.data_size = strlen(menstrual_words_en_k);
            //days 
            Util_Itoa(days, (char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size]);
            phys_remind_info_s.data_size = strlen((const char *)phys_remind_info_s.remind_info_buf);
            //days, update it in calendar
            memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], menstrual_words_en_l, strlen(menstrual_words_en_l));            
            phys_remind_info_s.data_size += strlen(menstrual_words_en_l);
        } 
    }
    else if(type == PHYS_REMIND_PREGNANCY_ONE)
    {//怀孕第一阶段（1~3个月）
        if(hours == 9 && minutes == 0)
        {//您已怀孕X周X天，请定期做检查
            //Pregnant for 
            memcpy(phys_remind_info_s.remind_info_buf, gestation_words_en_a, strlen(gestation_words_en_a));
            phys_remind_info_s.data_size = strlen(gestation_words_en_a);
            //n周x weeks
            if(days / 7 != 0)
            {
                //days/7 x Days
                Util_Itoa(days / 7, (char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size]);
                phys_remind_info_s.data_size = strlen((const char *)phys_remind_info_s.remind_info_buf);
                memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], gestation_words_en_b, strlen(gestation_words_en_b));            
                phys_remind_info_s.data_size += strlen(gestation_words_en_b);
            }
            //n天x Days
            if(days % 7 != 0)
            {
                //days%7 
                Util_Itoa(days % 7, (char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size]);
                phys_remind_info_s.data_size = strlen((const char *)phys_remind_info_s.remind_info_buf);
                memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], gestation_words_en_c, strlen(gestation_words_en_c));            
                phys_remind_info_s.data_size += strlen(gestation_words_en_c);
            }
            //, please antenatal examination regularly 
            memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], gestation_words_en_d, strlen(gestation_words_en_d));            
            phys_remind_info_s.data_size += strlen(gestation_words_en_d);
        } 
    }
    else if(type == PHYS_REMIND_PREGNANCY_TWO)
    {//怀孕第二阶段（3~9个月）
        if(hours == 9 && minutes == 0)
        {//09：00您已怀孕X周X天，请定期做检查（请保持好心情）
            //Pregnant for 
            memcpy(phys_remind_info_s.remind_info_buf, gestation_words_en_a, strlen(gestation_words_en_a));
            phys_remind_info_s.data_size = strlen(gestation_words_en_a);
            //n周x weeks
            if(days / 7 != 0)
            {
                //days/7 x Days
                Util_Itoa(days / 7, (char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size]);
                phys_remind_info_s.data_size = strlen((const char *)phys_remind_info_s.remind_info_buf);
                memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], gestation_words_en_b, strlen(gestation_words_en_b));            
                phys_remind_info_s.data_size += strlen(gestation_words_en_b);
            }
            //n天x Days
            if(days % 7 != 0)
            {
                //days%7 
                Util_Itoa(days % 7, (char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size]);
                phys_remind_info_s.data_size = strlen((const char *)phys_remind_info_s.remind_info_buf);
                memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], gestation_words_en_c, strlen(gestation_words_en_c));            
                phys_remind_info_s.data_size += strlen(gestation_words_en_c);
            }
            //, please antenatal examination regularly 
            memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], gestation_words_en_d, strlen(gestation_words_en_d));            
            phys_remind_info_s.data_size += strlen(gestation_words_en_d);
        }
    }
    else if(type == PHYS_REMIND_PREGNANCY_THU)
    {//怀孕第三阶段（9个月~出生前）
        if(hours == 9 && minutes == 0)
        {//09：00您已怀孕X周X天，请定期做检查(09：00距宝宝诞生X天，请保持好心情)
            
            //WJ.20180120.英文特殊处理，没有倒计天数
            days = 280 - days;
            
            //Pregnant for 
            memcpy(phys_remind_info_s.remind_info_buf, gestation_words_en_a, strlen(gestation_words_en_a));
            phys_remind_info_s.data_size = strlen(gestation_words_en_a);
            //n周x weeks
            if(days / 7 != 0)
            {
                //days/7 x Days
                Util_Itoa(days / 7, (char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size]);
                phys_remind_info_s.data_size = strlen((const char *)phys_remind_info_s.remind_info_buf);
                memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], gestation_words_en_b, strlen(gestation_words_en_b));            
                phys_remind_info_s.data_size += strlen(gestation_words_en_b);
            }
            //n天x Days
            if(days % 7 != 0)
            {
                //days%7 
                Util_Itoa(days % 7, (char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size]);
                phys_remind_info_s.data_size = strlen((const char *)phys_remind_info_s.remind_info_buf);
                memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], gestation_words_en_c, strlen(gestation_words_en_c));            
                phys_remind_info_s.data_size += strlen(gestation_words_en_c);
            }
            //, please antenatal examination regularly 
            memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], gestation_words_en_d, strlen(gestation_words_en_d));            
            phys_remind_info_s.data_size += strlen(gestation_words_en_d);
        }
    }
    else if(type == PHYS_REMIND_PREGNANCY_BIRTH)
    {//怀孕第四阶段（宝宝出生）
        if(hours == 9 && minutes == 0)
        {//预祝宝宝今天诞生您准备好了吗
            memcpy(phys_remind_info_s.remind_info_buf, gestation_words_en_f, strlen(gestation_words_en_f));
            phys_remind_info_s.data_size = strlen(gestation_words_en_f);
        }
    }
    else if(type == PHYS_REMIND_PREGNANCY_ERROR)
    {//怀孕信息过期，更新信息
        if(hours == 9 && minutes == 0)
        {//09：00请更新您的个人信息(如果经期信息有效,该信息不提醒)
            //please update your personal information
            memcpy(phys_remind_info_s.remind_info_buf, hot_mom_words_en_g, strlen(hot_mom_words_en_g));
            phys_remind_info_s.data_size = strlen(hot_mom_words_en_g);
        }
    }
    else if(type == PHYS_REMIND_BABY_BORN)
    {//宝宝出生后
        if(hours == 9 && minutes == 30)
        {
            if(days <= 10)
            {//宝宝出生第X天，恢复期注意卫生
                //This is the 
                memcpy(phys_remind_info_s.remind_info_buf, hot_mom_words_en_a, strlen(hot_mom_words_en_a));
                phys_remind_info_s.data_size = strlen(hot_mom_words_en_a);
                //days 
                Util_Itoa(days, (char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size]);
                phys_remind_info_s.data_size = strlen((const char *)phys_remind_info_s.remind_info_buf);
                // day of baby’s birth , Pay attention to the hygiene in recovery Period 
                memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], hot_mom_words_en_b, strlen(hot_mom_words_en_b));            
                phys_remind_info_s.data_size += strlen(hot_mom_words_en_b);
            }
            else if(days == 30)
            {//宝宝满月(30天)了，值得纪念的一天       
                //Baby is 30 days from birth, A memorable day                
                memcpy(phys_remind_info_s.remind_info_buf, hot_mom_words_en_c, strlen(hot_mom_words_en_c));
                phys_remind_info_s.data_size = strlen(hot_mom_words_en_c);  
            }
            else if(days == 100)
            {//宝宝一百天，值得纪念的一天
                //Baby is 100 days from birth, A memorable day
                memcpy(phys_remind_info_s.remind_info_buf, hot_mom_words_en_d, strlen(hot_mom_words_en_d));
                phys_remind_info_s.data_size = strlen(hot_mom_words_en_d); 
            }
            else if(days == 365)
            {//宝宝一周岁，值得纪念的一天
                //Baby is 1 year from birth, A memorable day
                memcpy(phys_remind_info_s.remind_info_buf, hot_mom_words_en_e, strlen(hot_mom_words_en_e));
                phys_remind_info_s.data_size = strlen(hot_mom_words_en_e); 
            }
            else if(days < 365)
            {//宝宝出生第x天
                //This is the 
                memcpy(phys_remind_info_s.remind_info_buf, hot_mom_words_en_a, strlen(hot_mom_words_en_a));
                phys_remind_info_s.data_size = strlen(hot_mom_words_en_a);
                //days 
                Util_Itoa(days, (char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size]);
                phys_remind_info_s.data_size = strlen((const char *)phys_remind_info_s.remind_info_buf);
                // day of baby’s birth
                memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], hot_mom_words_en_f, strlen(hot_mom_words_en_f));            
                phys_remind_info_s.data_size += strlen(hot_mom_words_en_f);
            }
            else
            {//请更新您的个人信息(如果经期信息有效,该信息不提醒)
                //please update your personal information
                memcpy(phys_remind_info_s.remind_info_buf, hot_mom_words_en_g, strlen(hot_mom_words_en_g));
                phys_remind_info_s.data_size = strlen(hot_mom_words_en_g);
            }
        }
    }
    else
    {//非法
        if(hours == 9 && minutes == 0)
        {//09：00信息错误，请更新
            //Error information, please update
            memcpy(phys_remind_info_s.remind_info_buf, menstrual_words_en_m, strlen(menstrual_words_en_m));
            phys_remind_info_s.data_size = strlen(menstrual_words_en_m);  
        }
    }        
}





