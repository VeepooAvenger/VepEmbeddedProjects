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
//�¾�����ʾ��
const char menstrual_words_en_a[] = " day, please change sanitary pad regularly";
const char menstrual_words_en_b[] = "Have a rest and change sanitary pad";
const char menstrual_words_en_c[] = "Please change sanitary pad before work off";
const char menstrual_words_en_d[] = "Please change sanitary pad and have a good night";

const char menstrual_words_en_e[] = "Safe period, ";
const char menstrual_words_en_f[] = " days to next period";
const char menstrual_words_en_g[] = " days be in period";

const char menstrual_words_en_h[] = " days of Ovulation period, careful��";
const char menstrual_words_en_i[] = "Today is Ovulation period, careful��";

const char menstrual_words_en_j[] = "Today is predicted to be the first day of Menstruation, please update information";
const char menstrual_words_en_k[] = "Menstruation delayed for ";
const char menstrual_words_en_l[] = "days, update it in calendar";
const char menstrual_words_en_m[] = "Error information, please update";
//��������ʾ��
const char pregnant_words_en_a[] = "In Ovulation period,Greater chances of getting pregnant";
const char pregnant_words_en_b[] = "In Ovulation period,Greater chances of getting pregnant";
//��������ʾ��
const char gestation_words_en_a[] = "Pregnant for ";
const char gestation_words_en_b[] = " weeks ";
const char gestation_words_en_c[] = " days";

const char gestation_words_en_d[] = ", please antenatal examination regularly";
const char gestation_words_en_e[] = ", please antenatal examination regularly";
const char gestation_words_en_f[] = "Are you ready for delivery today?";
//��������ʾ��
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
    {//����
        if(hours == 7 && minutes == 30)
        {//���쾭�ڵ�x�죬�ǵû�������
            //days 
            Util_Itoa(days, (char *)phys_remind_info_s.remind_info_buf);
            phys_remind_info_s.data_size = strlen((const char *)phys_remind_info_s.remind_info_buf);
            //day, please change sanitary pad regularly
            memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], menstrual_words_en_a, strlen(menstrual_words_en_a));            
            phys_remind_info_s.data_size += strlen(menstrual_words_en_a);
        }
        else if((hours == 11 || hours == 14) && minutes == 00)
        {//�������,�ǵø���������
            //Have a rest and change sanitary pad
            memcpy(phys_remind_info_s.remind_info_buf, menstrual_words_en_b, strlen(menstrual_words_en_b));
            phys_remind_info_s.data_size = strlen(menstrual_words_en_b);
        }
        else if(hours == 17 && minutes == 30)
        {//�°�ǰ�ǵø��������� 
            //Please change sanitary pad before work off
            memcpy(phys_remind_info_s.remind_info_buf, menstrual_words_en_c, strlen(menstrual_words_en_c));
            phys_remind_info_s.data_size = strlen(menstrual_words_en_c);
        }
        else if(hours == 21 && minutes == 30)
        {//˯ǰ����������,ע��˯������
            //Please change sanitary pad and have a good night
            memcpy(phys_remind_info_s.remind_info_buf, menstrual_words_en_d, strlen(menstrual_words_en_d));
            phys_remind_info_s.data_size = strlen(menstrual_words_en_d);
        }
    }
    else if(type == PHYS_REMIND_SAFETY_PERIOD)
    {//��ȫ��
        if(hours == 9 && minutes == 0)
        {//���찲ȫ�ڣ����´ξ���X��
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
    {//��ȫ��
        if(hours == 9 && minutes == 0)
        {//X�����뾭��(<=3��)��������׼��
            //days 
            Util_Itoa(days, (char *)phys_remind_info_s.remind_info_buf);
            phys_remind_info_s.data_size = strlen((const char *)phys_remind_info_s.remind_info_buf);
            //days be in period
            memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], menstrual_words_en_g, strlen(menstrual_words_en_g));            
            phys_remind_info_s.data_size += strlen(menstrual_words_en_g);   
        }
    }
    else if(type == PHYS_REMIND_OVIPOSIT_PERIOD)
    {//������
        if(hours == 9 && minutes == 0)
        {//�����������ڵ�X�죬С��Ŷ
            //days 
            Util_Itoa(days, (char *)phys_remind_info_s.remind_info_buf);
            phys_remind_info_s.data_size = strlen((const char *)phys_remind_info_s.remind_info_buf);
            //days of Ovulation period, careful��
            memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], menstrual_words_en_h, strlen(menstrual_words_en_h));            
            phys_remind_info_s.data_size += strlen(menstrual_words_en_h);
        }
    }
    else if(type == PHYS_REMIND_OVIPOSIT_DAY)
    {//������
        if(hours == 9 && minutes == 0)
        {//�����������գ�Ҫ����С��Ŷ
            //Today is Ovulation period, careful��
            memcpy(phys_remind_info_s.remind_info_buf, menstrual_words_en_i, strlen(menstrual_words_en_i));
            phys_remind_info_s.data_size = strlen(menstrual_words_en_i);
        }
    }
    else if(type == PHYS_REMIND_OVIPOSIT_PERIOD1)
    {//������������
        if(hours == 9 && minutes == 0)
        {//���������ڣ����м��ʽϴ�
            //In Ovulation period,Greater chances of getting pregnant
            memcpy(phys_remind_info_s.remind_info_buf, pregnant_words_en_a, strlen(pregnant_words_en_a));
            phys_remind_info_s.data_size = strlen(pregnant_words_en_a);
        }
    }
    else if(type == PHYS_REMIND_OVIPOSIT_DAY1)
    {//������������
        if(hours == 9 && minutes == 0)
        {//���������գ����м��ʽϴ�
            //In Ovulation period,Greater chances of getting pregnant.
            memcpy(phys_remind_info_s.remind_info_buf, pregnant_words_en_b, strlen(pregnant_words_en_b));
            phys_remind_info_s.data_size = strlen(pregnant_words_en_b);
        }
    }
    else if(type == PHYS_REMIND_FORECAST_MENSTRUAL_PERIOD)
    {//Ԥ�⾭��
        if(hours == 7 && minutes == 30 && days == 1)
        {//Ԥ�������뾭�ڣ��������Ϣ
            //Today is predicted to be the first day of Menstruation, please update information
            memcpy(phys_remind_info_s.remind_info_buf, menstrual_words_en_j, strlen(menstrual_words_en_j));
            phys_remind_info_s.data_size = strlen(menstrual_words_en_j);
        } 
        else if(hours == 9 && minutes == 0 && days > 1)
        {//�����¾��ӳ�X�죬�������Ϣ
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
    {//���е�һ�׶Σ�1~3���£�
        if(hours == 9 && minutes == 0)
        {//���ѻ���X��X�죬�붨�������
            //Pregnant for 
            memcpy(phys_remind_info_s.remind_info_buf, gestation_words_en_a, strlen(gestation_words_en_a));
            phys_remind_info_s.data_size = strlen(gestation_words_en_a);
            //n��x weeks
            if(days / 7 != 0)
            {
                //days/7 x Days
                Util_Itoa(days / 7, (char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size]);
                phys_remind_info_s.data_size = strlen((const char *)phys_remind_info_s.remind_info_buf);
                memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], gestation_words_en_b, strlen(gestation_words_en_b));            
                phys_remind_info_s.data_size += strlen(gestation_words_en_b);
            }
            //n��x Days
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
    {//���еڶ��׶Σ�3~9���£�
        if(hours == 9 && minutes == 0)
        {//09��00���ѻ���X��X�죬�붨������飨�뱣�ֺ����飩
            //Pregnant for 
            memcpy(phys_remind_info_s.remind_info_buf, gestation_words_en_a, strlen(gestation_words_en_a));
            phys_remind_info_s.data_size = strlen(gestation_words_en_a);
            //n��x weeks
            if(days / 7 != 0)
            {
                //days/7 x Days
                Util_Itoa(days / 7, (char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size]);
                phys_remind_info_s.data_size = strlen((const char *)phys_remind_info_s.remind_info_buf);
                memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], gestation_words_en_b, strlen(gestation_words_en_b));            
                phys_remind_info_s.data_size += strlen(gestation_words_en_b);
            }
            //n��x Days
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
    {//���е����׶Σ�9����~����ǰ��
        if(hours == 9 && minutes == 0)
        {//09��00���ѻ���X��X�죬�붨�������(09��00�౦������X�죬�뱣�ֺ�����)
            
            //WJ.20180120.Ӣ�����⴦��û�е�������
            days = 280 - days;
            
            //Pregnant for 
            memcpy(phys_remind_info_s.remind_info_buf, gestation_words_en_a, strlen(gestation_words_en_a));
            phys_remind_info_s.data_size = strlen(gestation_words_en_a);
            //n��x weeks
            if(days / 7 != 0)
            {
                //days/7 x Days
                Util_Itoa(days / 7, (char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size]);
                phys_remind_info_s.data_size = strlen((const char *)phys_remind_info_s.remind_info_buf);
                memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], gestation_words_en_b, strlen(gestation_words_en_b));            
                phys_remind_info_s.data_size += strlen(gestation_words_en_b);
            }
            //n��x Days
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
    {//���е��Ľ׶Σ�����������
        if(hours == 9 && minutes == 0)
        {//Ԥף�������쵮����׼��������
            memcpy(phys_remind_info_s.remind_info_buf, gestation_words_en_f, strlen(gestation_words_en_f));
            phys_remind_info_s.data_size = strlen(gestation_words_en_f);
        }
    }
    else if(type == PHYS_REMIND_PREGNANCY_ERROR)
    {//������Ϣ���ڣ�������Ϣ
        if(hours == 9 && minutes == 0)
        {//09��00��������ĸ�����Ϣ(���������Ϣ��Ч,����Ϣ������)
            //please update your personal information
            memcpy(phys_remind_info_s.remind_info_buf, hot_mom_words_en_g, strlen(hot_mom_words_en_g));
            phys_remind_info_s.data_size = strlen(hot_mom_words_en_g);
        }
    }
    else if(type == PHYS_REMIND_BABY_BORN)
    {//����������
        if(hours == 9 && minutes == 30)
        {
            if(days <= 10)
            {//����������X�죬�ָ���ע������
                //This is the 
                memcpy(phys_remind_info_s.remind_info_buf, hot_mom_words_en_a, strlen(hot_mom_words_en_a));
                phys_remind_info_s.data_size = strlen(hot_mom_words_en_a);
                //days 
                Util_Itoa(days, (char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size]);
                phys_remind_info_s.data_size = strlen((const char *)phys_remind_info_s.remind_info_buf);
                // day of baby��s birth , Pay attention to the hygiene in recovery Period 
                memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], hot_mom_words_en_b, strlen(hot_mom_words_en_b));            
                phys_remind_info_s.data_size += strlen(hot_mom_words_en_b);
            }
            else if(days == 30)
            {//��������(30��)�ˣ�ֵ�ü����һ��       
                //Baby is 30 days from birth, A memorable day                
                memcpy(phys_remind_info_s.remind_info_buf, hot_mom_words_en_c, strlen(hot_mom_words_en_c));
                phys_remind_info_s.data_size = strlen(hot_mom_words_en_c);  
            }
            else if(days == 100)
            {//����һ���죬ֵ�ü����һ��
                //Baby is 100 days from birth, A memorable day
                memcpy(phys_remind_info_s.remind_info_buf, hot_mom_words_en_d, strlen(hot_mom_words_en_d));
                phys_remind_info_s.data_size = strlen(hot_mom_words_en_d); 
            }
            else if(days == 365)
            {//����һ���ֵ꣬�ü����һ��
                //Baby is 1 year from birth, A memorable day
                memcpy(phys_remind_info_s.remind_info_buf, hot_mom_words_en_e, strlen(hot_mom_words_en_e));
                phys_remind_info_s.data_size = strlen(hot_mom_words_en_e); 
            }
            else if(days < 365)
            {//����������x��
                //This is the 
                memcpy(phys_remind_info_s.remind_info_buf, hot_mom_words_en_a, strlen(hot_mom_words_en_a));
                phys_remind_info_s.data_size = strlen(hot_mom_words_en_a);
                //days 
                Util_Itoa(days, (char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size]);
                phys_remind_info_s.data_size = strlen((const char *)phys_remind_info_s.remind_info_buf);
                // day of baby��s birth
                memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], hot_mom_words_en_f, strlen(hot_mom_words_en_f));            
                phys_remind_info_s.data_size += strlen(hot_mom_words_en_f);
            }
            else
            {//��������ĸ�����Ϣ(���������Ϣ��Ч,����Ϣ������)
                //please update your personal information
                memcpy(phys_remind_info_s.remind_info_buf, hot_mom_words_en_g, strlen(hot_mom_words_en_g));
                phys_remind_info_s.data_size = strlen(hot_mom_words_en_g);
            }
        }
    }
    else
    {//�Ƿ�
        if(hours == 9 && minutes == 0)
        {//09��00��Ϣ���������
            //Error information, please update
            memcpy(phys_remind_info_s.remind_info_buf, menstrual_words_en_m, strlen(menstrual_words_en_m));
            phys_remind_info_s.data_size = strlen(menstrual_words_en_m);  
        }
    }        
}





