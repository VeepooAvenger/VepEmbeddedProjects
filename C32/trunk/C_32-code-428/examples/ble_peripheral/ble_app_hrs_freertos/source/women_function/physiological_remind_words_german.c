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
//�¾�����ʾ��
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
//��������ʾ��
const char pregnant_words_ge_a[] = "Heute ist der Zeitraum der Follikelsprung.";
const char pregnant_words_ge_b[] = "Heute ist der Tag der Follikelsprung.";
const char pregnant_words_ge_c[] = "Die Wahrscheinlichkeit der Schwangerschaft ist groesser.";
//��������ʾ��
const char gestation_words_ge_a[] = "Sie sind in den ";
const char gestation_words_ge_b[] = " Wochen und ";
const char gestation_words_ge_b1[] = " Tagen schwanger";
const char gestation_words_ge_c[] = ", bitte untersuchen Sie regelmaessig.";
const char gestation_words_ge_d[] = ", bitte untersuchen Sie regelmaessig.";
const char gestation_words_ge_e[] = "Es gibt ";
const char gestation_words_ge_e1[] = " Tagen vor der Baby Geburt";
const char gestation_words_ge_f[] = "Haben Sie sich gut vorbereitet, wenn das Baby heute geboren wird.";
//��������ʾ��
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
    {//����
        if(hours == 7 && minutes == 30)
        {//���쾭�ڵ�x�죬�ǵû�������
            //"���쾭�ڵ�"
            memcpy(phys_remind_info_s.remind_info_buf, menstrual_words_ge_a, strlen(menstrual_words_ge_a));
            phys_remind_info_s.data_size = strlen(menstrual_words_ge_a);
            //days 
            Util_Itoa(days, (char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size]);
            phys_remind_info_s.data_size = strlen((const char *)phys_remind_info_s.remind_info_buf);
            //��
            memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], menstrual_words_ge_b, strlen(menstrual_words_ge_b));            
            phys_remind_info_s.data_size += strlen(menstrual_words_ge_b);
            //,�ǵø���������
            memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], menstrual_words_ge_d, strlen(menstrual_words_ge_d));            
            phys_remind_info_s.data_size += strlen(menstrual_words_ge_d);
        }
        else if((hours == 11 || hours == 14) && minutes == 00)
        {//�������,�ǵø���������
            //�������
            memcpy(phys_remind_info_s.remind_info_buf, menstrual_words_ge_c, strlen(menstrual_words_ge_c));
            phys_remind_info_s.data_size = strlen(menstrual_words_ge_c);
            //���ǵø���������
            memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], menstrual_words_ge_d, strlen(menstrual_words_ge_d));            
            phys_remind_info_s.data_size += strlen(menstrual_words_ge_d);
        }
        else if(hours == 17 && minutes == 30)
        {//�°�ǰ�ǵø��������� 
            memcpy(phys_remind_info_s.remind_info_buf, menstrual_words_ge_e, strlen(menstrual_words_ge_e));
            phys_remind_info_s.data_size = strlen(menstrual_words_ge_e);
        }
        else if(hours == 21 && minutes == 30)
        {//˯ǰ����������,ע��˯������
            memcpy(phys_remind_info_s.remind_info_buf, menstrual_words_ge_e1, strlen(menstrual_words_ge_e1));
            phys_remind_info_s.data_size = strlen(menstrual_words_ge_e1);
        }
    }
    else if(type == PHYS_REMIND_SAFETY_PERIOD)
    {//��ȫ��
        if(hours == 9 && minutes == 0)
        {//���찲ȫ�ڣ����´ξ���X��
            //���찲ȫ�ڣ����´ξ���Heute ist die Zeit der Sicherheit,Es gibt noch
            memcpy(phys_remind_info_s.remind_info_buf, menstrual_words_ge_f, strlen(menstrual_words_ge_f));
            phys_remind_info_s.data_size = strlen(menstrual_words_ge_f);
            //days ��
            Util_Itoa(days, (char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size]);
            phys_remind_info_s.data_size = strlen((const char *)phys_remind_info_s.remind_info_buf);
            //�� Tage vor der Menstruation.
            memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], menstrual_words_ge_f1, strlen(menstrual_words_ge_f1));            
            phys_remind_info_s.data_size += strlen(menstrual_words_ge_f1);
        }
    }
    else if(type == PHYS_REMIND_SAFETY_PERIOD1)
    {//��ȫ��
        if(hours == 9 && minutes == 0)
        {//X�����뾭��(<=3��)��������׼��
            //Nachdem 
            memcpy((char *)&phys_remind_info_s.remind_info_buf[0], menstrual_words_ge_j, strlen(menstrual_words_ge_j));            
            phys_remind_info_s.data_size += strlen(menstrual_words_ge_j);  
            //days 
            Util_Itoa(days, (char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size]);
            phys_remind_info_s.data_size = strlen((const char *)phys_remind_info_s.remind_info_buf);
            //�����뾭�ڣ�������׼��Tage wird Menstruation eintreten. Bitte vorbereiten Sie.
            memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], menstrual_words_ge_j1, strlen(menstrual_words_ge_j1));            
            phys_remind_info_s.data_size += strlen(menstrual_words_ge_j1);   
        }
    }
    else if(type == PHYS_REMIND_OVIPOSIT_PERIOD)
    {//������
        if(hours == 9 && minutes == 0)
        {//�����������ڵ�X�죬С��Ŷ
            //�����������ڵ�
            memcpy(phys_remind_info_s.remind_info_buf, menstrual_words_ge_g, strlen(menstrual_words_ge_g));
            phys_remind_info_s.data_size = strlen(menstrual_words_ge_g);
            //days 
            Util_Itoa(days, (char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size]);
            phys_remind_info_s.data_size = strlen((const char *)phys_remind_info_s.remind_info_buf);
            //��С��ŶTage der Zeitraum der Follikelsprung. Seien Sie vorsichtig.
            memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], menstrual_words_ge_h, strlen(menstrual_words_ge_h));            
            phys_remind_info_s.data_size += strlen(menstrual_words_ge_h);
        }
    }
    else if(type == PHYS_REMIND_OVIPOSIT_DAY)
    {//������
        if(hours == 9 && minutes == 0)
        {//�����������գ�Ҫ����С��Ŷ
            memcpy(phys_remind_info_s.remind_info_buf, menstrual_words_ge_i, strlen(menstrual_words_ge_i));
            phys_remind_info_s.data_size = strlen(menstrual_words_ge_i);
        }
    }
    else if(type == PHYS_REMIND_OVIPOSIT_PERIOD1)
    {//������������
        if(hours == 9 && minutes == 0)
        {//���������ڣ����м��ʽϴ�
            //���������ڣ�Heute ist der Zeitraum der Follikelsprung
            memcpy(phys_remind_info_s.remind_info_buf, pregnant_words_ge_a, strlen(pregnant_words_ge_a));
            phys_remind_info_s.data_size = strlen(pregnant_words_ge_a);
            //���м��ʽϴ�Die Wahrscheinlichkeit der Schwangerschaft ist groesser.
            memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], pregnant_words_ge_c, strlen(pregnant_words_ge_c));            
            phys_remind_info_s.data_size += strlen(pregnant_words_ge_c);
        }
    }
    else if(type == PHYS_REMIND_OVIPOSIT_DAY1)
    {//������������
        if(hours == 9 && minutes == 0)
        {//���������գ����м��ʽϴ�
            //���������գ�Heute ist der Tag der Follikelsprung
            memcpy(phys_remind_info_s.remind_info_buf, pregnant_words_ge_b, strlen(pregnant_words_ge_b));
            phys_remind_info_s.data_size = strlen(pregnant_words_ge_b);
            //���м��ʽϴ�Die Wahrscheinlichkeit der Schwangerschaft ist groesser.
            memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], pregnant_words_ge_c, strlen(pregnant_words_ge_c));            
            phys_remind_info_s.data_size += strlen(pregnant_words_ge_c);
        }
    }
    else if(type == PHYS_REMIND_FORECAST_MENSTRUAL_PERIOD)
    {//Ԥ�⾭��
        if(hours == 7 && minutes == 30 && days == 1)
        {//Ԥ�������뾭�ڣ��������Ϣ
            memcpy(phys_remind_info_s.remind_info_buf, menstrual_words_ge_k, strlen(menstrual_words_ge_k));
            phys_remind_info_s.data_size = strlen(menstrual_words_ge_k);
        } 
        else if(hours == 9 && minutes == 0 && days > 1)
        {//�����¾��ӳ�X�죬�������Ϣ
            //�����¾��ӳ� Dieses Mal wird die Menstruation 
            memcpy(phys_remind_info_s.remind_info_buf, menstrual_words_ge_l, strlen(menstrual_words_ge_l));
            phys_remind_info_s.data_size = strlen(menstrual_words_ge_l);
            //days 
            Util_Itoa(days, (char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size]);
            phys_remind_info_s.data_size = strlen((const char *)phys_remind_info_s.remind_info_buf);
            //�죬�������ϢTage verspaeten. Bitte aktualisieren Sie die Daten. 
            memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], menstrual_words_ge_m, strlen(menstrual_words_ge_m));            
            phys_remind_info_s.data_size += strlen(menstrual_words_ge_m);
        } 
    }
    else if(type == PHYS_REMIND_PREGNANCY_ONE)
    {//���е�һ�׶Σ�1~3���£�
        if(hours == 9 && minutes == 0)
        {//���ѻ���X��X�죬�붨�������
            //���ѻ���Sie sind in den
            memcpy(phys_remind_info_s.remind_info_buf, gestation_words_ge_a, strlen(gestation_words_ge_a));
            phys_remind_info_s.data_size = strlen(gestation_words_ge_a);
            //n��
            if(days / 7 != 0)
            {
                //days/7  X Wochen und 
                Util_Itoa(days / 7, (char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size]);
                phys_remind_info_s.data_size = strlen((const char *)phys_remind_info_s.remind_info_buf);
                memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], gestation_words_ge_b, strlen(gestation_words_ge_b));            
                phys_remind_info_s.data_size += strlen(gestation_words_ge_b);
            }
            //n��
            if(days % 7 != 0)
            {
                //days%7 X Tagen schwanger
                Util_Itoa(days % 7, (char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size]);
                phys_remind_info_s.data_size = strlen((const char *)phys_remind_info_s.remind_info_buf);
                memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], gestation_words_ge_b1, strlen(gestation_words_ge_b1));            
                phys_remind_info_s.data_size += strlen(gestation_words_ge_b1);
            }
            //,�붨�������bitte untersuchen Sie regelmaessig.
            memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], gestation_words_ge_c, strlen(gestation_words_ge_c));            
            phys_remind_info_s.data_size += strlen(gestation_words_ge_c);
        } 
    }
    else if(type == PHYS_REMIND_PREGNANCY_TWO)
    {//���еڶ��׶Σ�3~6���£�
        if(hours == 9 && minutes == 0)
        {//09��00���ѻ���X��X�죬�뱣�ֺ�����
            //���ѻ���Sie sind in den
            memcpy(phys_remind_info_s.remind_info_buf, gestation_words_ge_a, strlen(gestation_words_ge_a));
            phys_remind_info_s.data_size = strlen(gestation_words_ge_a);
            //n��
            if(days / 7 != 0)
            {
                //days/7  X Wochen und 
                Util_Itoa(days / 7, (char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size]);
                phys_remind_info_s.data_size = strlen((const char *)phys_remind_info_s.remind_info_buf);
                memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], gestation_words_ge_b, strlen(gestation_words_ge_b));            
                phys_remind_info_s.data_size += strlen(gestation_words_ge_b);
            }
            //n��
            if(days % 7 != 0)
            {
                //days%7 X Tagen schwanger
                Util_Itoa(days % 7, (char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size]);
                phys_remind_info_s.data_size = strlen((const char *)phys_remind_info_s.remind_info_buf);
                memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], gestation_words_ge_b1, strlen(gestation_words_ge_b1));            
                phys_remind_info_s.data_size += strlen(gestation_words_ge_b1);
            }
            //,�붨�������bitte untersuchen Sie regelmaessig.
            memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], gestation_words_ge_d, strlen(gestation_words_ge_d));            
            phys_remind_info_s.data_size += strlen(gestation_words_ge_d);
        }
    }
    else if(type == PHYS_REMIND_PREGNANCY_THU)
    {//���е����׶Σ�6����~����ǰ��
        if(hours == 9 && minutes == 0)
        {//09��00�౦������X�죬�뱣�ֺ�����
            //�౦������ Es gibt
            memcpy(phys_remind_info_s.remind_info_buf, gestation_words_ge_e, strlen(gestation_words_ge_e));
            phys_remind_info_s.data_size = strlen(gestation_words_ge_e);
            //n�� n Tagen vor der Baby Geburt
            Util_Itoa(days, (char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size]);
            phys_remind_info_s.data_size = strlen((const char *)phys_remind_info_s.remind_info_buf);
            memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], gestation_words_ge_e1, strlen(gestation_words_ge_e1));            
            phys_remind_info_s.data_size += strlen(gestation_words_ge_e1);
            
            //,�붨�������bitte untersuchen Sie regelmaessig.
            memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], gestation_words_ge_d, strlen(gestation_words_ge_d));            
            phys_remind_info_s.data_size += strlen(gestation_words_ge_d);
        }
    }
    else if(type == PHYS_REMIND_PREGNANCY_BIRTH)
    {//���е��Ľ׶Σ�����������
        if(hours == 9 && minutes == 0)
        {//Ԥף�������쵮����׼��������
            memcpy(phys_remind_info_s.remind_info_buf, gestation_words_ge_f, strlen(gestation_words_ge_f));
            phys_remind_info_s.data_size = strlen(gestation_words_ge_f);
        }
    }
    else if(type == PHYS_REMIND_PREGNANCY_ERROR)
    {//������Ϣ���ڣ�������Ϣ
        if(hours == 9 && minutes == 0)
        {//09��00��������ĸ�����Ϣ(���������Ϣ��Ч,����Ϣ������)
            //Bitte aktualisieren Sie Ihre pers?nlichen Daten.
            memcpy(phys_remind_info_s.remind_info_buf, hot_mom_words_ge_g, strlen(hot_mom_words_ge_g));
            phys_remind_info_s.data_size = strlen(hot_mom_words_ge_g);
        }
    }
    else if(type == PHYS_REMIND_BABY_BORN)
    {//����������
        if(hours == 9 && minutes == 30)
        {
            if(days <= 10)
            {//����������X�죬�ָ���ע������
                //����������Das Baby hat 
                memcpy(phys_remind_info_s.remind_info_buf, hot_mom_words_ge_a, strlen(hot_mom_words_ge_a));
                phys_remind_info_s.data_size = strlen(hot_mom_words_ge_a);
                //days 
                Util_Itoa(days, (char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size]);
                phys_remind_info_s.data_size = strlen((const char *)phys_remind_info_s.remind_info_buf);
                //��  Tage geboren
                memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], hot_mom_words_ge_a1, strlen(hot_mom_words_ge_a1));            
                phys_remind_info_s.data_size += strlen(hot_mom_words_ge_a1);
                //����ָ���ע������.Beachten Sie die Hygiene in der Genesung
                memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], hot_mom_words_ge_b, strlen(hot_mom_words_ge_b));            
                phys_remind_info_s.data_size += strlen(hot_mom_words_ge_b);
            }
            else if(days == 30)
            {//��������(30��)�ˣ�ֵ�ü����һ��              
                memcpy(phys_remind_info_s.remind_info_buf, hot_mom_words_ge_c, strlen(hot_mom_words_ge_c));
                phys_remind_info_s.data_size = strlen(hot_mom_words_ge_c); 
                memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], hot_mom_words_ge_f, strlen(hot_mom_words_ge_f));            
                phys_remind_info_s.data_size += strlen(hot_mom_words_ge_f); 
            }
            else if(days == 100)
            {//����һ���죬ֵ�ü����һ��
                memcpy(phys_remind_info_s.remind_info_buf, hot_mom_words_ge_d, strlen(hot_mom_words_ge_d));
                phys_remind_info_s.data_size = strlen(hot_mom_words_ge_d);
                memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], hot_mom_words_ge_f, strlen(hot_mom_words_ge_f));            
                phys_remind_info_s.data_size += strlen(hot_mom_words_ge_f); 
            }
            else if(days == 365)
            {//����һ���ֵ꣬�ü����һ��
                memcpy(phys_remind_info_s.remind_info_buf, hot_mom_words_ge_e, strlen(hot_mom_words_ge_e));
                phys_remind_info_s.data_size = strlen(hot_mom_words_ge_e); 
                memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], hot_mom_words_ge_f, strlen(hot_mom_words_ge_f));            
                phys_remind_info_s.data_size += strlen(hot_mom_words_ge_f);
            }
            else if(days < 365)
            {//����������x��
                //����������Das Baby hat 
                memcpy(phys_remind_info_s.remind_info_buf, hot_mom_words_ge_a, strlen(hot_mom_words_ge_a));
                phys_remind_info_s.data_size = strlen(hot_mom_words_ge_a);
                //days 
                Util_Itoa(days, (char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size]);
                phys_remind_info_s.data_size = strlen((const char *)phys_remind_info_s.remind_info_buf);
                //�� Tage geboren
                memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], hot_mom_words_ge_a1, strlen(hot_mom_words_ge_a1));            
                phys_remind_info_s.data_size += strlen(hot_mom_words_ge_a1);
            }
            else
            {//��������ĸ�����Ϣ(���������Ϣ��Ч,����Ϣ������)
                //Bitte aktualisieren Sie Ihre pers?nlichen Daten.
                memcpy(phys_remind_info_s.remind_info_buf, hot_mom_words_ge_g, strlen(hot_mom_words_ge_g));
                phys_remind_info_s.data_size = strlen(hot_mom_words_ge_g);
            }
        }
    }
    else
    {//�Ƿ�
        if(hours == 9 && minutes == 0)
        {//09��00��Ϣ���������
            memcpy(phys_remind_info_s.remind_info_buf, menstrual_words_ge_n, strlen(menstrual_words_ge_n));
            phys_remind_info_s.data_size = strlen(menstrual_words_ge_n);  
        }
    }        
}





