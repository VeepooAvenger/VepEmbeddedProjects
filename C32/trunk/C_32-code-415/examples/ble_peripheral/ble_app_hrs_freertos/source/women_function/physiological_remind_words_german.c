/***************************Copyright(c)***********************************
**                       Veepoo Tech
**                  COPYRIGHT (C) 2016-2019
**-----------File Info-----------------------------------------------------
** File        name:     physiological_remind_words_german.c
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
const char menstrual_words_ge_a[] = "Heute ist der ";
const char menstrual_words_ge_b[] = " Tag der Menstruation";
const char menstrual_words_ge_c[] = "Bitte sitzen Sie nicht Langezeit.";
const char menstrual_words_ge_d[] = "Bitte vergessen Sie nicht, die Damenbinde zu wechseln.";
const char menstrual_words_ge_e[] = "Bitte wechseln Sie die Damenbinde vor dem Arbeitschluss.";
const char menstrual_words_ge_e1[] = "Bitte wechseln Sie die Damenbinde und beachten Sie die Qualitat des Schlafes.";

const char menstrual_words_ge_f[] = "Heute ist die Zeit der Sicherheit,Es gibt noch";
const char menstrual_words_ge_f1[] = " Tage vor der Menstruation.";

const char menstrual_words_ge_g[] = "Heute ist die ";
const char menstrual_words_ge_h[] = " Tage der Zeitraum der Follikelsprung. Seien Sie vorsichtig.";
const char menstrual_words_ge_i[] = "Heute ist die Follikelsprung. Seien Sie besonders vorsichtig.";

const char menstrual_words_ge_j[] = "Nachdem ";
const char menstrual_words_ge_j1[] = " Tage wird Menstruation eintreten. Bitte vorbereiten Sie.";

const char menstrual_words_ge_k[] = "Wir schaetzen das, dass Heute die Mensttuation eintreten wird. Bitte aktualisieren Sie die Daten.";
const char menstrual_words_ge_l[] = "Dieses Mal wird die Menstruation ";
const char menstrual_words_ge_m[] = "Tage verspaeten. Bitte aktualisieren Sie die Daten. ";
const char menstrual_words_ge_n[] = "Informationsfehler,bitte aktualisieren.";
//备孕期提示语
const char pregnant_words_ge_a[] = "Heute ist der Zeitraum der Follikelsprung.";
const char pregnant_words_ge_b[] = "Heute ist der Tag der Follikelsprung.";
const char pregnant_words_ge_c[] = "Die Wahrscheinlichkeit der Schwangerschaft ist groesser.";
//怀孕期提示语
const char gestation_words_ge_a[] = "Sie sind in den ";
const char gestation_words_ge_b[] = " Wochen und ";
const char gestation_words_ge_b1[] = " Tagen schwanger";
const char gestation_words_ge_c[] = ", bitte untersuchen Sie regelmaessig.";
const char gestation_words_ge_d[] = ", bitte untersuchen Sie regelmaessig.";
const char gestation_words_ge_e[] = "Es gibt ";
const char gestation_words_ge_e1[] = " Tagen vor der Baby Geburt";
const char gestation_words_ge_f[] = "Haben Sie sich gut vorbereitet, wenn das Baby heute geboren wird.";
//辣妈期提示语
const char hot_mom_words_ge_a[] = "Das Baby hat ";
const char hot_mom_words_ge_a1[] = " Tage geboren";
const char hot_mom_words_ge_b[] = ".Beachten Sie die Hygiene in der Genesung";
const char hot_mom_words_ge_c[] = "Das Baby ist 30 Tage und,";
const char hot_mom_words_ge_d[] = "Das Baby ist 100 Tage,";
const char hot_mom_words_ge_e[] = "Das Baby ist ein Jahr alt,";
const char hot_mom_words_ge_f[] = " Heute ist die Denkwuedigkeit.";
const char hot_mom_words_ge_g[] = "Bitte aktualisieren Sie Ihre personlichen Daten.";


/*********************************************************************
* local functions
*/

void Phys_GermanRemind(uint8_t type, uint16_t days, uint8_t hours, uint8_t minutes)
{
    if(type == PHYS_REMIND_MENSTRUAL_PERIOD)
    {//经期
        if(hours == 7 && minutes == 30)
        {//今天经期第x天，记得换卫生巾
            //"今天经期第"
            memcpy(phys_remind_info_s.remind_info_buf, menstrual_words_ge_a, strlen(menstrual_words_ge_a));
            phys_remind_info_s.data_size = strlen(menstrual_words_ge_a);
            //days 
            Util_Itoa(days, (char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size]);
            phys_remind_info_s.data_size = strlen((const char *)phys_remind_info_s.remind_info_buf);
            //天
            memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], menstrual_words_ge_b, strlen(menstrual_words_ge_b));            
            phys_remind_info_s.data_size += strlen(menstrual_words_ge_b);
            //,记得更换卫生巾
            memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], menstrual_words_ge_d, strlen(menstrual_words_ge_d));            
            phys_remind_info_s.data_size += strlen(menstrual_words_ge_d);
        }
        else if((hours == 11 || hours == 14) && minutes == 00)
        {//请勿久坐,记得更换卫生巾
            //请勿久坐
            memcpy(phys_remind_info_s.remind_info_buf, menstrual_words_ge_c, strlen(menstrual_words_ge_c));
            phys_remind_info_s.data_size = strlen(menstrual_words_ge_c);
            //，记得更换卫生巾
            memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], menstrual_words_ge_d, strlen(menstrual_words_ge_d));            
            phys_remind_info_s.data_size += strlen(menstrual_words_ge_d);
        }
        else if(hours == 17 && minutes == 30)
        {//下班前记得更换卫生巾 
            memcpy(phys_remind_info_s.remind_info_buf, menstrual_words_ge_e, strlen(menstrual_words_ge_e));
            phys_remind_info_s.data_size = strlen(menstrual_words_ge_e);
        }
        else if(hours == 21 && minutes == 30)
        {//睡前更换卫生巾,注意睡眠质量
            memcpy(phys_remind_info_s.remind_info_buf, menstrual_words_ge_e1, strlen(menstrual_words_ge_e1));
            phys_remind_info_s.data_size = strlen(menstrual_words_ge_e1);
        }
    }
    else if(type == PHYS_REMIND_SAFETY_PERIOD)
    {//安全期
        if(hours == 9 && minutes == 0)
        {//今天安全期，距下次经期X天
            //今天安全期，距下次经期Heute ist die Zeit der Sicherheit,Es gibt noch
            memcpy(phys_remind_info_s.remind_info_buf, menstrual_words_ge_f, strlen(menstrual_words_ge_f));
            phys_remind_info_s.data_size = strlen(menstrual_words_ge_f);
            //days 天
            Util_Itoa(days, (char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size]);
            phys_remind_info_s.data_size = strlen((const char *)phys_remind_info_s.remind_info_buf);
            //天 Tage vor der Menstruation.
            memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], menstrual_words_ge_f1, strlen(menstrual_words_ge_f1));            
            phys_remind_info_s.data_size += strlen(menstrual_words_ge_f1);
        }
    }
    else if(type == PHYS_REMIND_SAFETY_PERIOD1)
    {//安全期
        if(hours == 9 && minutes == 0)
        {//X天后进入经期(<=3天)，请做好准备
            //Nachdem 
            memcpy((char *)&phys_remind_info_s.remind_info_buf[0], menstrual_words_ge_j, strlen(menstrual_words_ge_j));            
            phys_remind_info_s.data_size += strlen(menstrual_words_ge_j);  
            //days 
            Util_Itoa(days, (char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size]);
            phys_remind_info_s.data_size = strlen((const char *)phys_remind_info_s.remind_info_buf);
            //天后进入经期，请做好准备Tage wird Menstruation eintreten. Bitte vorbereiten Sie.
            memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], menstrual_words_ge_j1, strlen(menstrual_words_ge_j1));            
            phys_remind_info_s.data_size += strlen(menstrual_words_ge_j1);   
        }
    }
    else if(type == PHYS_REMIND_OVIPOSIT_PERIOD)
    {//排卵期
        if(hours == 9 && minutes == 0)
        {//今天是排卵期第X天，小心哦
            //今天是排卵期第
            memcpy(phys_remind_info_s.remind_info_buf, menstrual_words_ge_g, strlen(menstrual_words_ge_g));
            phys_remind_info_s.data_size = strlen(menstrual_words_ge_g);
            //days 
            Util_Itoa(days, (char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size]);
            phys_remind_info_s.data_size = strlen((const char *)phys_remind_info_s.remind_info_buf);
            //，小心哦Tage der Zeitraum der Follikelsprung. Seien Sie vorsichtig.
            memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], menstrual_words_ge_h, strlen(menstrual_words_ge_h));            
            phys_remind_info_s.data_size += strlen(menstrual_words_ge_h);
        }
    }
    else if(type == PHYS_REMIND_OVIPOSIT_DAY)
    {//排卵日
        if(hours == 9 && minutes == 0)
        {//今天是排卵日，要格外小心哦
            memcpy(phys_remind_info_s.remind_info_buf, menstrual_words_ge_i, strlen(menstrual_words_ge_i));
            phys_remind_info_s.data_size = strlen(menstrual_words_ge_i);
        }
    }
    else if(type == PHYS_REMIND_OVIPOSIT_PERIOD1)
    {//备孕期排卵期
        if(hours == 9 && minutes == 0)
        {//今天排卵期，怀孕几率较大
            //今天排卵期，Heute ist der Zeitraum der Follikelsprung
            memcpy(phys_remind_info_s.remind_info_buf, pregnant_words_ge_a, strlen(pregnant_words_ge_a));
            phys_remind_info_s.data_size = strlen(pregnant_words_ge_a);
            //怀孕几率较大Die Wahrscheinlichkeit der Schwangerschaft ist groesser.
            memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], pregnant_words_ge_c, strlen(pregnant_words_ge_c));            
            phys_remind_info_s.data_size += strlen(pregnant_words_ge_c);
        }
    }
    else if(type == PHYS_REMIND_OVIPOSIT_DAY1)
    {//备孕期排卵日
        if(hours == 9 && minutes == 0)
        {//今天排卵日，怀孕几率较大
            //今天排卵日，Heute ist der Tag der Follikelsprung
            memcpy(phys_remind_info_s.remind_info_buf, pregnant_words_ge_b, strlen(pregnant_words_ge_b));
            phys_remind_info_s.data_size = strlen(pregnant_words_ge_b);
            //怀孕几率较大Die Wahrscheinlichkeit der Schwangerschaft ist groesser.
            memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], pregnant_words_ge_c, strlen(pregnant_words_ge_c));            
            phys_remind_info_s.data_size += strlen(pregnant_words_ge_c);
        }
    }
    else if(type == PHYS_REMIND_FORECAST_MENSTRUAL_PERIOD)
    {//预测经期
        if(hours == 7 && minutes == 30 && days == 1)
        {//预测今天进入经期，请更新信息
            memcpy(phys_remind_info_s.remind_info_buf, menstrual_words_ge_k, strlen(menstrual_words_ge_k));
            phys_remind_info_s.data_size = strlen(menstrual_words_ge_k);
        } 
        else if(hours == 9 && minutes == 0 && days > 1)
        {//本次月经延迟X天，请更新信息
            //本次月经延迟 Dieses Mal wird die Menstruation 
            memcpy(phys_remind_info_s.remind_info_buf, menstrual_words_ge_l, strlen(menstrual_words_ge_l));
            phys_remind_info_s.data_size = strlen(menstrual_words_ge_l);
            //days 
            Util_Itoa(days, (char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size]);
            phys_remind_info_s.data_size = strlen((const char *)phys_remind_info_s.remind_info_buf);
            //天，请更新信息Tage verspaeten. Bitte aktualisieren Sie die Daten. 
            memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], menstrual_words_ge_m, strlen(menstrual_words_ge_m));            
            phys_remind_info_s.data_size += strlen(menstrual_words_ge_m);
        } 
    }
    else if(type == PHYS_REMIND_PREGNANCY_ONE)
    {//怀孕第一阶段（1~3个月）
        if(hours == 9 && minutes == 0)
        {//您已怀孕X周X天，请定期做检查
            //您已怀孕Sie sind in den
            memcpy(phys_remind_info_s.remind_info_buf, gestation_words_ge_a, strlen(gestation_words_ge_a));
            phys_remind_info_s.data_size = strlen(gestation_words_ge_a);
            //n周
            if(days / 7 != 0)
            {
                //days/7  X Wochen und 
                Util_Itoa(days / 7, (char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size]);
                phys_remind_info_s.data_size = strlen((const char *)phys_remind_info_s.remind_info_buf);
                memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], gestation_words_ge_b, strlen(gestation_words_ge_b));            
                phys_remind_info_s.data_size += strlen(gestation_words_ge_b);
            }
            //n天
            if(days % 7 != 0)
            {
                //days%7 X Tagen schwanger
                Util_Itoa(days % 7, (char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size]);
                phys_remind_info_s.data_size = strlen((const char *)phys_remind_info_s.remind_info_buf);
                memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], gestation_words_ge_b1, strlen(gestation_words_ge_b1));            
                phys_remind_info_s.data_size += strlen(gestation_words_ge_b1);
            }
            //,请定期做检查bitte untersuchen Sie regelmaessig.
            memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], gestation_words_ge_c, strlen(gestation_words_ge_c));            
            phys_remind_info_s.data_size += strlen(gestation_words_ge_c);
        } 
    }
    else if(type == PHYS_REMIND_PREGNANCY_TWO)
    {//怀孕第二阶段（3~6个月）
        if(hours == 9 && minutes == 0)
        {//09：00您已怀孕X周X天，请保持好心情
            //您已怀孕Sie sind in den
            memcpy(phys_remind_info_s.remind_info_buf, gestation_words_ge_a, strlen(gestation_words_ge_a));
            phys_remind_info_s.data_size = strlen(gestation_words_ge_a);
            //n周
            if(days / 7 != 0)
            {
                //days/7  X Wochen und 
                Util_Itoa(days / 7, (char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size]);
                phys_remind_info_s.data_size = strlen((const char *)phys_remind_info_s.remind_info_buf);
                memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], gestation_words_ge_b, strlen(gestation_words_ge_b));            
                phys_remind_info_s.data_size += strlen(gestation_words_ge_b);
            }
            //n天
            if(days % 7 != 0)
            {
                //days%7 X Tagen schwanger
                Util_Itoa(days % 7, (char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size]);
                phys_remind_info_s.data_size = strlen((const char *)phys_remind_info_s.remind_info_buf);
                memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], gestation_words_ge_b1, strlen(gestation_words_ge_b1));            
                phys_remind_info_s.data_size += strlen(gestation_words_ge_b1);
            }
            //,请定期做检查bitte untersuchen Sie regelmaessig.
            memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], gestation_words_ge_d, strlen(gestation_words_ge_d));            
            phys_remind_info_s.data_size += strlen(gestation_words_ge_d);
        }
    }
    else if(type == PHYS_REMIND_PREGNANCY_THU)
    {//怀孕第三阶段（6个月~出生前）
        if(hours == 9 && minutes == 0)
        {//09：00距宝宝诞生X天，请保持好心情
            //距宝宝诞生 Es gibt
            memcpy(phys_remind_info_s.remind_info_buf, gestation_words_ge_e, strlen(gestation_words_ge_e));
            phys_remind_info_s.data_size = strlen(gestation_words_ge_e);
            //n天 n Tagen vor der Baby Geburt
            Util_Itoa(days, (char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size]);
            phys_remind_info_s.data_size = strlen((const char *)phys_remind_info_s.remind_info_buf);
            memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], gestation_words_ge_e1, strlen(gestation_words_ge_e1));            
            phys_remind_info_s.data_size += strlen(gestation_words_ge_e1);
            
            //,请定期做检查bitte untersuchen Sie regelmaessig.
            memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], gestation_words_ge_d, strlen(gestation_words_ge_d));            
            phys_remind_info_s.data_size += strlen(gestation_words_ge_d);
        }
    }
    else if(type == PHYS_REMIND_PREGNANCY_BIRTH)
    {//怀孕第四阶段（宝宝出生）
        if(hours == 9 && minutes == 0)
        {//预祝宝宝今天诞生您准备好了吗
            memcpy(phys_remind_info_s.remind_info_buf, gestation_words_ge_f, strlen(gestation_words_ge_f));
            phys_remind_info_s.data_size = strlen(gestation_words_ge_f);
        }
    }
    else if(type == PHYS_REMIND_PREGNANCY_ERROR)
    {//怀孕信息过期，更新信息
        if(hours == 9 && minutes == 0)
        {//09：00请更新您的个人信息(如果经期信息有效,该信息不提醒)
            //Bitte aktualisieren Sie Ihre pers?nlichen Daten.
            memcpy(phys_remind_info_s.remind_info_buf, hot_mom_words_ge_g, strlen(hot_mom_words_ge_g));
            phys_remind_info_s.data_size = strlen(hot_mom_words_ge_g);
        }
    }
    else if(type == PHYS_REMIND_BABY_BORN)
    {//宝宝出生后
        if(hours == 9 && minutes == 30)
        {
            if(days <= 10)
            {//宝宝出生第X天，恢复期注意卫生
                //宝宝出生第Das Baby hat 
                memcpy(phys_remind_info_s.remind_info_buf, hot_mom_words_ge_a, strlen(hot_mom_words_ge_a));
                phys_remind_info_s.data_size = strlen(hot_mom_words_ge_a);
                //days 
                Util_Itoa(days, (char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size]);
                phys_remind_info_s.data_size = strlen((const char *)phys_remind_info_s.remind_info_buf);
                //天  Tage geboren
                memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], hot_mom_words_ge_a1, strlen(hot_mom_words_ge_a1));            
                phys_remind_info_s.data_size += strlen(hot_mom_words_ge_a1);
                //，请恢复期注意卫生.Beachten Sie die Hygiene in der Genesung
                memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], hot_mom_words_ge_b, strlen(hot_mom_words_ge_b));            
                phys_remind_info_s.data_size += strlen(hot_mom_words_ge_b);
            }
            else if(days == 30)
            {//宝宝满月(30天)了，值得纪念的一天              
                memcpy(phys_remind_info_s.remind_info_buf, hot_mom_words_ge_c, strlen(hot_mom_words_ge_c));
                phys_remind_info_s.data_size = strlen(hot_mom_words_ge_c); 
                memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], hot_mom_words_ge_f, strlen(hot_mom_words_ge_f));            
                phys_remind_info_s.data_size += strlen(hot_mom_words_ge_f); 
            }
            else if(days == 100)
            {//宝宝一百天，值得纪念的一天
                memcpy(phys_remind_info_s.remind_info_buf, hot_mom_words_ge_d, strlen(hot_mom_words_ge_d));
                phys_remind_info_s.data_size = strlen(hot_mom_words_ge_d);
                memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], hot_mom_words_ge_f, strlen(hot_mom_words_ge_f));            
                phys_remind_info_s.data_size += strlen(hot_mom_words_ge_f); 
            }
            else if(days == 365)
            {//宝宝一周岁，值得纪念的一天
                memcpy(phys_remind_info_s.remind_info_buf, hot_mom_words_ge_e, strlen(hot_mom_words_ge_e));
                phys_remind_info_s.data_size = strlen(hot_mom_words_ge_e); 
                memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], hot_mom_words_ge_f, strlen(hot_mom_words_ge_f));            
                phys_remind_info_s.data_size += strlen(hot_mom_words_ge_f);
            }
            else if(days < 365)
            {//宝宝出生第x天
                //宝宝出生第Das Baby hat 
                memcpy(phys_remind_info_s.remind_info_buf, hot_mom_words_ge_a, strlen(hot_mom_words_ge_a));
                phys_remind_info_s.data_size = strlen(hot_mom_words_ge_a);
                //days 
                Util_Itoa(days, (char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size]);
                phys_remind_info_s.data_size = strlen((const char *)phys_remind_info_s.remind_info_buf);
                //天 Tage geboren
                memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], hot_mom_words_ge_a1, strlen(hot_mom_words_ge_a1));            
                phys_remind_info_s.data_size += strlen(hot_mom_words_ge_a1);
            }
            else
            {//请更新您的个人信息(如果经期信息有效,该信息不提醒)
                //Bitte aktualisieren Sie Ihre pers?nlichen Daten.
                memcpy(phys_remind_info_s.remind_info_buf, hot_mom_words_ge_g, strlen(hot_mom_words_ge_g));
                phys_remind_info_s.data_size = strlen(hot_mom_words_ge_g);
            }
        }
    }
    else
    {//非法
        if(hours == 9 && minutes == 0)
        {//09：00信息错误，请更新
            memcpy(phys_remind_info_s.remind_info_buf, menstrual_words_ge_n, strlen(menstrual_words_ge_n));
            phys_remind_info_s.data_size = strlen(menstrual_words_ge_n);  
        }
    }        
}





