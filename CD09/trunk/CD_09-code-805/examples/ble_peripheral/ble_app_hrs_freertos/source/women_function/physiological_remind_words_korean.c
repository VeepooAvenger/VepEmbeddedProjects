/***************************Copyright(c)***********************************
**                       Veepoo Tech
**                  COPYRIGHT (C) 2016-2019
**-----------File Info-----------------------------------------------------
** File        name:     physiological_remind_words_korean.c
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
const char menstrual_words_ko_a[] = {0xec,0x98,0xa4,0xeb,0x8a,0x98,0xec,0x9d,0x80,0x20,0xec,0x83,0x9d,0xeb,0xa6,0xac,0x20,  0x00};
const char menstrual_words_ko_b[] = {0xec,0x9d,0xbc,0xec,0xa7,0xb8,  0x00};
const char menstrual_words_ko_c[] = {0xec,0x98,0xa4,0xeb,0x9e,0x98,0x20,0xec,0x95,0x89,0xec,0x95,0x84,0x20,0xec,0x9e,0x88,0xec,0xa7,0x80,0x20,0xeb,0xa7,0x88,0xec,0x84,0xb8,0xec,0x9a,0x94,  0x00};
const char menstrual_words_ko_d[] = {0xec,0x83,0x9d,0xeb,0xa6,0xac,0xeb,0x8c,0x80,0x20,0xea,0xb5,0x90,0xec,0xb2,0xb4,0x20,0xec,0x9e,0x8a,0xec,0xa7,0x80,0x20,0xeb,0xa7,0x88,0xec,0x84,0xb8,0xec,0x9a,0x94,  0x00};
const char menstrual_words_ko_e[] = {0xed,0x87,0xb4,0xea,0xb7,0xbc,0xec,0xa0,0x84,0x20,0xec,0x83,0x9d,0xeb,0xa6,0xac,0xeb,0x8c,0x80,0x20,0xea,0xb5,0x90,0xec,0xb2,0xb4,0xed,0x95,0x98,0xec,0x84,0xb8,0xec,0x9a,0x94,  0x00};
const char menstrual_words_ko_e1[] = {0xec,0x88,0x98,0xeb,0xa9,0xb4,0xec,0xa0,0x84,0x20,0xec,0x83,0x9d,0xeb,0xa6,0xac,0xeb,0x8c,0x80,0x20,0xea,0xb5,0x90,0xec,0xb2,0xb4,0xed,0x95,0x98,0xec,0x84,0xb8,0xec,0x9a,0x94,0x2c,
                                      0x20,0xec,0x88,0x98,0xeb,0xa9,0xb4,0x20,0xed,0x92,0x88,0xec,0xa7,0x88,0xec,
                                      0x97,0x90,0x20,0xec,0xa3,0xbc,0xec,0x9d,0x98,0xed,0x95,0x98,0xec,0x84,0xb8,0xec,0x9a,0x94,  0x00};

const char menstrual_words_ko_f[] = {0xec,0x98,0xa4,0xeb,0x8a,0x98,0xec,0x9d,0x80,0x20,0xec,0x9e,0x84,0xec,0x8b,0xa0,0x20,0xec,0x95,
                                     0x88,0xec,0xa0,0x84,0xea,0xb8,0xb0,0x2c,0x20,0xeb,0x8b,0xa4,0xec,0x9d,0x8c,0x20,0xec,0x83,0x9d,0xeb,0xa6,0xac,0xea,0xb9,0x8c,0xec,0xa7,0x80,0xeb,0x8a,0x94,0x20,  0x00};
const char menstrual_words_ko_f1[] = {0xec,0x9d,0xbc,  0x00};
const char menstrual_words_ko_g[] = {0xec,0x98,0xa4,0xeb,0x8a,0x98,0xec,0x9d,0x80,0x20,0xeb,0xb0,0xb0,0xeb,0x9e,0x80,0xea,0xb8,0xb0,0x20,  0x00};
const char menstrual_words_ko_h[] = {0xec,0x9d,0xbc,0xec,0xa7,0xb8,0x2c,0x20,0xec,0xa3,0xbc,0xec,0x9d,0x98,0xed,0x95,0x98,0xec,0x84,0xb8,0xec,0x9a,0x94, 0x00};
const char menstrual_words_ko_i[] = {0xec,0x98,0xa4,0xeb,0x8a,0x98,0xec,0x9d,0x80,0x20,0xeb,0xb0,0xb0,0xeb,0x9e,0x80,0xec,0x9d,0xbc,0x2c,0x20,0xed,0x8a,0xb9,0xeb,0xb3,0x84,0xed,0x9e,0x88,0x20,0xec,0xa3,0xbc,0xec,0x9d,0x98,0xed,0x95,0x98,0xec,0x84,0xb8,0xec,0x9a,0x94,  0x00};

const char menstrual_words_ko_j[] = {0xec,0x9d,0xbc,0xed,0x9b,0x84,0x20,0xec,0x83,0x9d,0xeb,0xa6,0xac,0x20,0xec,0x8b,0x9c,0xec,0x9e,0x91,0x2c,0x20,0xec,0xa4,0x80,0xeb,0xb9,0x84,0xed,0x95,0x98,0xec,0x84,0xb8,0xec,0x9a,0x94, 0x00};
const char menstrual_words_ko_k[] = {0xec,0x98,0xa4,0xeb,0x8a,0x98,0x20,0xec,0x83,0x9d,0xeb,0xa6,0xac,0x20,0xec,0x8b,0x9c,0xec,0x9e,0x91,0x20,0xec,0x98,0x88,0xec,0xb8,0xa1,0x2c,0x20,0xec,0xa0,0x95,0xeb,0xb3,0xb4,0xeb,
                                     0xa5,0xbc,0x20,0xec,0x97,0x85,0xeb,0x8d,0xb0,0xec,0x9d,0xb4,0xed,0x8a,0xb8,0xed,0x95,0x98,0xec,0x84,0xb8,0xec,0x9a,0x94,0x20,  0x00};
const char menstrual_words_ko_l[] = {0xec,0x9d,0xb4,0xeb,0xb2,0x88,0x20,0xec,0x83,0x9d,0xeb,0xa6,0xac,0xea,0xb0,0x80,0x20,  0x00};
const char menstrual_words_ko_m[] = {0xec,0x9d,0xbc,0x20,0xec,0xa7,0x80,0xec,0x97,0xb0,0x2c,0x20,0xec,0xa0,0x95,0xeb,0xb3,0xb4,0xeb,0xa5,0xbc,0x20,0xec,0x97,0x85,0xeb,0x8d,0xb0,0xec,0x9d,0xb4,0xed,0x8a,0xb8,0xed,0x95,0x98,0xec,0x84,0xb8,0xec,0x9a,0x94,  0x00};
const char menstrual_words_ko_n[] = {0xec,0xa0,0x95,0xeb,0xb3,0xb4,0x20,0xec,0x97,0x90,0xeb,0x9f,0xac,0x2c,0x20,0xec,0x97,0x85,0xeb,0x8d,0xb0,0xec,0x9d,0xb4,0xed,0x8a,0xb8,0xed,0x95,0x98,0xec,0x84,0xb8,0xec,0x9a,0x94,  0x00};
//��������ʾ��
const char pregnant_words_ko_a[] = {0xec,0x98,0xa4,0xeb,0x8a,0x98,0xec,0x9d,0x80,0x20,0xeb,0xb0,0xb0,0xeb,0x9e,0x80,0xea,0xb8,0xb0,0x2c,0x20,0xec,0x9e,0x84,0xec,0x8b,0xa0,0x20,0xed,0x99,0x95,0xeb,0xa5,0xa0,0xec,0x9d,0xb4,0x20,0xed,0x81,0xbd,0xeb,0x8b,0x88,0xeb,0x8b,0xa4,  0x00};
const char pregnant_words_ko_b[] = {0xec,0x98,0xa4,0xeb,0x8a,0x98,0xec,0x9d,0x80,0x20,0xeb,0xb0,0xb0,0xeb,0x9e,0x80,0xec,0x9d,0xbc,0x2c,0x20,0xec,0x9e,0x84,0xec,0x8b,0xa0,0x20,0xed,0x99,0x95,0xeb,0xa5,0xa0,0xec,0x9d,0xb4,0x20,0xed,0x81,0xbd,0xeb,0x8b,0x88,0xeb,0x8b,0xa4,  0x00};
//��������ʾ��
const char gestation_words_ko_a[] = {0xec,0x9e,0x84,0xec,0x8b,0xa0,0x20,  0x00};
const char gestation_words_ko_a1[] ={0xec,0x9d,0xbc,  0x00};
const char gestation_words_ko_b[] = {0xec,0xa3,0xbc,0x20,  0x00};
const char gestation_words_ko_c[] = {0xec,0xa7,0xb8,0x2c,0x20,0xec,0xa0,0x95,0xea,0xb8,0xb0,0xec,0xa0,0x81,0xec,0x9c,0xbc,0xeb,0xa1,0x9c,0x20,0xea,0xb2,0x80,0xec,0x82,0xac,0xeb,0xa5,0xbc,0x20,0xed,0x95,0x98,0xec,0x84,0xb8,0xec,0x9a,0x94,  0x00};
const char gestation_words_ko_d[] = {0xec,0xa7,0xb8,0x2c,0x20,0xec,0xa0,0x95,0xea,0xb8,0xb0,0xec,0xa0,0x81,0xec,0x9c,0xbc,0xeb,0xa1,0x9c,0x20,0xea,0xb2,0x80,0xec,0x82,0xac,0xeb,0xa5,0xbc,0x20,0xed,0x95,0x98,0xec,0x84,0xb8,0xec,0x9a,0x94,  0x00};
const char gestation_words_ko_e[] = {0xec,0x95,0x84,0xea,0xb8,0xb0,0x20,0xec,0xb6,0x9c,0xec,0x83,0x9d,0xec,0x9d,0xbc,0xec,0x9d,0xb4,0x20,  0x00};
const char gestation_words_ko_e1[] ={0xec,0x9d,0xbc, 0x20, 0xeb,0x82,0xa8, 0xec,0x9d,0x8c, 0x2c,0x20, 0xec,0xa0,0x95,0xea,0xb8,0xb0,0xec,0xa0,0x81,0xec,0x9c,0xbc,0xeb,0xa1,0x9c,0x20,0xea,0xb2,0x80,0xec,
                                     0x82,0xac,0xeb,0xa5,0xbc,0x20,0xed,0x95,0x98,0xec,0x84,0xb8,0xec,0x9a,0x94,  0x00};
const char gestation_words_ko_f[] = {0xec,0xb6,0x95,0xed,0x95,0x98,0xec,0xb6,0x95,0xed,0x95,0x98,0x2c,0x20,0xec,0x98,0xa4,0xeb,0x8a,0x98,0x20,0xec,0xb6,0x9c,0xec,0x83,0x9d,0x20,0xec,0x98,0x88,0xec,0xa0,
                                     0x95,0x2c,0xec,0xa4,0x80,0xeb,0xb9,0x84,0xeb,0x90,0x98,0xec,0x85,0xa8,0xeb,0x82,0x98,0xec,0x9a,0x94,0x20,  0x00};
//��������ʾ��
const char hot_mom_words_ko_a[] = {0xec,0x95,0x84,0xea,0xb8,0xb0,0x20,0xec,0xb6,0x9c,0xec,0x83,0x9d,0x20,  0x00};
const char hot_mom_words_ko_b[] = {0x2c,0x20,0xec,0x82,0xb0,0xed,0x9b,0x84,0x20,0xed,0x9a,0x8c,0xeb,0xb3,0xb5,0xea,0xb8,0xb0,0x20,0xec,0x9c,0x84,0xec,0x83,0x9d,0xec,0x97,0x90,0x20,0xec,0xa3,0xbc,0xec,0x9d,0x98,0xed,0x95,0x98,0xec,0x84,0xb8,0xec,0x9a,0x94,  0x00};
const char hot_mom_words_ko_c[] = {0xec,0x95,0x84,0xea,0xb8,0xb0,0x20,0xec,0xb6,0x9c,0xec,0x83,0x9d,0x20,0xed,0x95,0x9c,0xeb,0x8b,0xac,  0x00};
const char hot_mom_words_ko_d[] = {0xec,0x95,0x84,0xea,0xb8,0xb0,0x20,0xeb,0xb0,0xb1,0xec,0x9d,0xbc,0x20,0xea,0xb8,0xb0,0xeb,0x85,0x90,0xec,0x9d,0xbc,0xec,0x9e,0x85,0xeb,0x8b,0x88,0xeb,0x8b,0xa4,0x2e,  0x00};
const char hot_mom_words_ko_e[] = {0xec,0x95,0x84,0xea,0xb8,0xb0,0x20,0xeb,0x8f,0x8c,0x20,0xea,0xb8,0xb0,0xeb,0x85,0x90,0xec,0x9d,0xbc,0xec,0x9e,0x85,0xeb,0x8b,0x88,0xeb,0x8b,0xa4,0x2e,  0x00};
const char hot_mom_words_ko_f[] = {0x20,0xea,0xb8,0xb0,0xeb,0x85,0x90,0xec,0x9d,0xbc,0xec,0x9e,0x85,0xeb,0x8b,0x88,0xeb,0x8b,0xa4,0x2e,0x20,  0x00};
const char hot_mom_words_ko_g[] = {0xea,0xb0,0x9c,0xec,0x9d,0xb8,0x20,0xec,0xa0,0x95,0xeb,0xb3,0xb4,0xeb,0xa5,0xbc,0x20,0xec,0x97,0x85,0xeb,0x8d,0xb0,0xec,0x9d,0xb4,0xed,0x8a,0xb8,0xed,0x95,0x98,0xec,0x84,0xb8,0xec,0x9a,0x94,  0x00};


/*********************************************************************
* local functions
*/

void Phys_KoreanRemind(uint8_t type, uint16_t days, uint8_t hours, uint8_t minutes)
{
    if(type == PHYS_REMIND_MENSTRUAL_PERIOD)
    {//����
        if(hours == 7 && minutes == 30)
        {//���쾭�ڵ�x�죬�ǵû�������
            //"���쾭�ڵ�"
            memcpy(phys_remind_info_s.remind_info_buf, menstrual_words_ko_a, strlen(menstrual_words_ko_a));
            phys_remind_info_s.data_size = strlen(menstrual_words_ko_a);
            //days 
            Util_Itoa(days, (char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size]);
            phys_remind_info_s.data_size = strlen((const char *)phys_remind_info_s.remind_info_buf);
            //��
            memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], menstrual_words_ko_b, strlen(menstrual_words_ko_b));            
            phys_remind_info_s.data_size += strlen(menstrual_words_ko_b);
            //,�ǵø���������
            memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], menstrual_words_ko_d, strlen(menstrual_words_ko_d));            
            phys_remind_info_s.data_size += strlen(menstrual_words_ko_d);
        }
        else if((hours == 11 || hours == 14) && minutes == 00)
        {//�������,�ǵø���������
            //�������
            memcpy(phys_remind_info_s.remind_info_buf, menstrual_words_ko_c, strlen(menstrual_words_ko_c));
            phys_remind_info_s.data_size = strlen(menstrual_words_ko_c);
            //���ǵø���������
            memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], menstrual_words_ko_d, strlen(menstrual_words_ko_d));            
            phys_remind_info_s.data_size += strlen(menstrual_words_ko_d);
        }
        else if(hours == 17 && minutes == 30)
        {//�°�ǰ�ǵø��������� 
            memcpy(phys_remind_info_s.remind_info_buf, menstrual_words_ko_e, strlen(menstrual_words_ko_e));
            phys_remind_info_s.data_size = strlen(menstrual_words_ko_e);
        }
        else if(hours == 21 && minutes == 30)
        {//˯ǰ����������,ע��˯������
            memcpy(phys_remind_info_s.remind_info_buf, menstrual_words_ko_e1, strlen(menstrual_words_ko_e1));
            phys_remind_info_s.data_size = strlen(menstrual_words_ko_e1);
        }
    }
    else if(type == PHYS_REMIND_SAFETY_PERIOD)
    {//��ȫ��
        if(hours == 9 && minutes == 0)
        {//���찲ȫ�ڣ����´ξ���X��
            //���찲ȫ�ڣ����´ξ���
            memcpy(phys_remind_info_s.remind_info_buf, menstrual_words_ko_f, strlen(menstrual_words_ko_f));
            phys_remind_info_s.data_size = strlen(menstrual_words_ko_f);
            //days ��
            Util_Itoa(days, (char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size]);
            phys_remind_info_s.data_size = strlen((const char *)phys_remind_info_s.remind_info_buf);
            //��
            memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], menstrual_words_ko_f1, strlen(menstrual_words_ko_f1));            
            phys_remind_info_s.data_size += strlen(menstrual_words_ko_f1);
        }
    }
    else if(type == PHYS_REMIND_SAFETY_PERIOD1)
    {//��ȫ��
        if(hours == 9 && minutes == 0)
        {//X�����뾭��(<=3��)��������׼��
            //days 
            Util_Itoa(days, (char *)phys_remind_info_s.remind_info_buf);
            phys_remind_info_s.data_size = strlen((const char *)phys_remind_info_s.remind_info_buf);
            //�����뾭�ڣ�������׼��
            memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], menstrual_words_ko_j, strlen(menstrual_words_ko_j));            
            phys_remind_info_s.data_size += strlen(menstrual_words_ko_j);   
        }
    }
    else if(type == PHYS_REMIND_OVIPOSIT_PERIOD)
    {//������
        if(hours == 9 && minutes == 0)
        {//�����������ڵ�X�죬С��Ŷ
            //�����������ڵ�
            memcpy(phys_remind_info_s.remind_info_buf, menstrual_words_ko_g, strlen(menstrual_words_ko_g));
            phys_remind_info_s.data_size = strlen(menstrual_words_ko_g);
            //days 
            Util_Itoa(days, (char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size]);
            phys_remind_info_s.data_size = strlen((const char *)phys_remind_info_s.remind_info_buf);
            //�죬С��Ŷ
            memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], menstrual_words_ko_h, strlen(menstrual_words_ko_h));            
            phys_remind_info_s.data_size += strlen(menstrual_words_ko_h);
        }
    }
    else if(type == PHYS_REMIND_OVIPOSIT_DAY)
    {//������
        if(hours == 9 && minutes == 0)
        {//�����������գ�Ҫ����С��Ŷ
            memcpy(phys_remind_info_s.remind_info_buf, menstrual_words_ko_i, strlen(menstrual_words_ko_i));
            phys_remind_info_s.data_size = strlen(menstrual_words_ko_i);
        }
    }
    else if(type == PHYS_REMIND_OVIPOSIT_PERIOD1)
    {//������������
        if(hours == 9 && minutes == 0)
        {//���������ڣ����м��ʽϴ�
            //���������ڣ����м��ʽϴ�
            memcpy(phys_remind_info_s.remind_info_buf, pregnant_words_ko_a, strlen(pregnant_words_ko_a));
            phys_remind_info_s.data_size = strlen(pregnant_words_ko_a);
        }
    }
    else if(type == PHYS_REMIND_OVIPOSIT_DAY1)
    {//������������
        if(hours == 9 && minutes == 0)
        {//���������գ����м��ʽϴ�
            //���������գ����м��ʽϴ�
            memcpy(phys_remind_info_s.remind_info_buf, pregnant_words_ko_b, strlen(pregnant_words_ko_b));
            phys_remind_info_s.data_size = strlen(pregnant_words_ko_b);
        }
    }
    else if(type == PHYS_REMIND_FORECAST_MENSTRUAL_PERIOD)
    {//Ԥ�⾭��
        if(hours == 7 && minutes == 30 && days == 1)
        {//Ԥ�������뾭�ڣ��������Ϣ
            memcpy(phys_remind_info_s.remind_info_buf, menstrual_words_ko_k, strlen(menstrual_words_ko_k));
            phys_remind_info_s.data_size = strlen(menstrual_words_ko_k);
        } 
        else if(hours == 9 && minutes == 0 && days > 1)
        {//�����¾��ӳ�X�죬�������Ϣ
            //�����¾��ӳ�
            memcpy(phys_remind_info_s.remind_info_buf, menstrual_words_ko_l, strlen(menstrual_words_ko_l));
            phys_remind_info_s.data_size = strlen(menstrual_words_ko_l);
            //days 
            Util_Itoa(days, (char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size]);
            phys_remind_info_s.data_size = strlen((const char *)phys_remind_info_s.remind_info_buf);
            //�죬�������Ϣ
            memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], menstrual_words_ko_m, strlen(menstrual_words_ko_m));            
            phys_remind_info_s.data_size += strlen(menstrual_words_ko_m);
        } 
    }
    else if(type == PHYS_REMIND_PREGNANCY_ONE)
    {//���е�һ�׶Σ�1~3���£�
        if(hours == 9 && minutes == 0)
        {//���ѻ���X��X�죬�붨�������
            //���ѻ���
            memcpy(phys_remind_info_s.remind_info_buf, gestation_words_ko_a, strlen(gestation_words_ko_a));
            phys_remind_info_s.data_size = strlen(gestation_words_ko_a);
            //n��
            if(days / 7 != 0)
            {
                //days/7 
                Util_Itoa(days / 7, (char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size]);
                phys_remind_info_s.data_size = strlen((const char *)phys_remind_info_s.remind_info_buf);
                memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], gestation_words_ko_b, strlen(gestation_words_ko_b));            
                phys_remind_info_s.data_size += strlen(gestation_words_ko_b);
            }
            //n��
            if(days % 7 != 0)
            {
                //days%7 
                Util_Itoa(days % 7, (char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size]);
                phys_remind_info_s.data_size = strlen((const char *)phys_remind_info_s.remind_info_buf);
                memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], gestation_words_ko_a1, strlen(gestation_words_ko_a1));            
                phys_remind_info_s.data_size += strlen(gestation_words_ko_a1);
            }
            //,�붨�������
            memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], gestation_words_ko_c, strlen(gestation_words_ko_c));            
            phys_remind_info_s.data_size += strlen(gestation_words_ko_c);
        } 
    }
    else if(type == PHYS_REMIND_PREGNANCY_TWO)
    {//���еڶ��׶Σ�3~6���£�
        if(hours == 9 && minutes == 0)
        {//09��00���ѻ���X��X�죬�뱣�ֺ�����
            //���ѻ���
            memcpy(phys_remind_info_s.remind_info_buf, gestation_words_ko_a, strlen(gestation_words_ko_a));
            phys_remind_info_s.data_size = strlen(gestation_words_ko_a);
            //n��
            if(days / 7 != 0)
            {
                //days/7 
                Util_Itoa(days / 7, (char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size]);
                phys_remind_info_s.data_size = strlen((const char *)phys_remind_info_s.remind_info_buf);
                memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], gestation_words_ko_b, strlen(gestation_words_ko_b));            
                phys_remind_info_s.data_size += strlen(gestation_words_ko_b);
            }
            //n��
            if(days % 7 != 0)
            {
                //days%7 
                Util_Itoa(days % 7, (char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size]);
                phys_remind_info_s.data_size = strlen((const char *)phys_remind_info_s.remind_info_buf);
                memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], gestation_words_ko_a1, strlen(gestation_words_ko_a1));            
                phys_remind_info_s.data_size += strlen(gestation_words_ko_a1);
            }
            //,�붨�������
            memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], gestation_words_ko_d, strlen(gestation_words_ko_d));            
            phys_remind_info_s.data_size += strlen(gestation_words_ko_d);
        }
    }
    else if(type == PHYS_REMIND_PREGNANCY_THU)
    {//���е����׶Σ�6����~����ǰ��
        if(hours == 9 && minutes == 0)
        {//09��00�౦������X�죬�뱣�ֺ�����
            //�౦������
            memcpy(phys_remind_info_s.remind_info_buf, gestation_words_ko_e, strlen(gestation_words_ko_e));
            phys_remind_info_s.data_size = strlen(gestation_words_ko_e);
            //n��
            Util_Itoa(days, (char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size]);
            phys_remind_info_s.data_size = strlen((const char *)phys_remind_info_s.remind_info_buf);
            //��,�붨�������
            memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], gestation_words_ko_e1, strlen(gestation_words_ko_e1));            
            phys_remind_info_s.data_size += strlen(gestation_words_ko_e1);
        }
    }
    else if(type == PHYS_REMIND_PREGNANCY_BIRTH)
    {//���е��Ľ׶Σ�����������
        if(hours == 9 && minutes == 0)
        {//Ԥף�������쵮����׼��������
            memcpy(phys_remind_info_s.remind_info_buf, gestation_words_ko_f, strlen(gestation_words_ko_f));
            phys_remind_info_s.data_size = strlen(gestation_words_ko_f);
        }
    }
    else if(type == PHYS_REMIND_PREGNANCY_ERROR)
    {//������Ϣ���ڣ�������Ϣ
        if(hours == 9 && minutes == 0)
        {//09��00��������ĸ�����Ϣ(���������Ϣ��Ч,����Ϣ������)
            memcpy(phys_remind_info_s.remind_info_buf, hot_mom_words_ko_g, strlen(hot_mom_words_ko_g));
            phys_remind_info_s.data_size = strlen(hot_mom_words_ko_g);
        }
    }
    else if(type == PHYS_REMIND_BABY_BORN)
    {//����������
        if(hours == 9 && minutes == 30)
        {
            if(days <= 10)
            {//����������X�죬�ָ���ע������
                //����������
                memcpy(phys_remind_info_s.remind_info_buf, hot_mom_words_ko_a, strlen(hot_mom_words_ko_a));
                phys_remind_info_s.data_size = strlen(hot_mom_words_ko_a);
                //days 
                Util_Itoa(days, (char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size]);
                phys_remind_info_s.data_size = strlen((const char *)phys_remind_info_s.remind_info_buf);
                //��
                memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], menstrual_words_ko_b, strlen(menstrual_words_ko_b));            
                phys_remind_info_s.data_size += strlen(menstrual_words_ko_b);
                //����ָ���ע������
                memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], hot_mom_words_ko_b, strlen(hot_mom_words_ko_b));            
                phys_remind_info_s.data_size += strlen(hot_mom_words_ko_b);
            }
            else if(days == 30)
            {//��������(30��)�ˣ�ֵ�ü����һ��              
                memcpy(phys_remind_info_s.remind_info_buf, hot_mom_words_ko_c, strlen(hot_mom_words_ko_c));
                phys_remind_info_s.data_size = strlen(hot_mom_words_ko_c);  
                memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], hot_mom_words_ko_f, strlen(hot_mom_words_ko_f));            
                phys_remind_info_s.data_size += strlen(hot_mom_words_ko_f);
            }
            else if(days == 100)
            {//����һ���죬ֵ�ü����һ��
                memcpy(phys_remind_info_s.remind_info_buf, hot_mom_words_ko_d, strlen(hot_mom_words_ko_d));
                phys_remind_info_s.data_size = strlen(hot_mom_words_ko_d); 
                memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], hot_mom_words_ko_f, strlen(hot_mom_words_ko_f));            
                phys_remind_info_s.data_size += strlen(hot_mom_words_ko_f);
            }
            else if(days == 365)
            {//����һ���ֵ꣬�ü����һ��
                memcpy(phys_remind_info_s.remind_info_buf, hot_mom_words_ko_e, strlen(hot_mom_words_ko_e));
                phys_remind_info_s.data_size = strlen(hot_mom_words_ko_e); 
                memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], hot_mom_words_ko_f, strlen(hot_mom_words_ko_f));            
                phys_remind_info_s.data_size += strlen(hot_mom_words_ko_f);
            }
            else if(days < 365)
            {//����������x��
                //����������
                memcpy(phys_remind_info_s.remind_info_buf, hot_mom_words_ko_a, strlen(hot_mom_words_ko_a));
                phys_remind_info_s.data_size = strlen(hot_mom_words_ko_a);
                //days 
                Util_Itoa(days, (char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size]);
                phys_remind_info_s.data_size = strlen((const char *)phys_remind_info_s.remind_info_buf);
                //��
                memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], menstrual_words_ko_b, strlen(menstrual_words_ko_b));            
                phys_remind_info_s.data_size += strlen(menstrual_words_ko_b);
            }
            else
            {//��������ĸ�����Ϣ(���������Ϣ��Ч,����Ϣ������)
                memcpy(phys_remind_info_s.remind_info_buf, hot_mom_words_ko_g, strlen(hot_mom_words_ko_g));
                phys_remind_info_s.data_size = strlen(hot_mom_words_ko_g);
            }
        }
    }
    else
    {//�Ƿ�
        if(hours == 9 && minutes == 0)
        {//09��00��Ϣ���������
            memcpy(phys_remind_info_s.remind_info_buf, menstrual_words_ko_n, strlen(menstrual_words_ko_n));
            phys_remind_info_s.data_size = strlen(menstrual_words_ko_n);  
        }
    }        
}





