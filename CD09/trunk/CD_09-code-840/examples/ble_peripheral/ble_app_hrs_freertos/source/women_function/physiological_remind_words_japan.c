/***************************Copyright(c)***********************************
**                       Veepoo Tech
**                  COPYRIGHT (C) 2016-2019
**-----------File Info-----------------------------------------------------
** File        name:     physiological_remind_words_japan.c
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
const char menstrual_words_jp_a[] = {0xe4,0xbb,0x8a,0xe6,0x97,0xa5,0xe3,0x81,0xaf,0xe7,0x94,0x9f,0xe7,0x90,0x86,0xe3,0x81,0xae,0xe7,0xac,0xac,  0x00};
const char menstrual_words_jp_b[] = {0xe6,0x97,0xa5,0xe7,0x9b,0xae,  0x00};
const char menstrual_words_jp_c[] = {0xe9,0x95,0xb7,0xe6,0x99,0x82,0xe9,0x96,0x93,0xe5,0xba,0xa7,0xe3,0x82,0x8b,0xe3,0x81,0xbe,0xe3,0x81,0xbe,0xe3,0x81,0xab,0xe3,0x81,0xaa,0xe3,0x82,0x89,0xe3,0x81,0xaa,0xe3,0x81,0x84,0xe3,0x82,0x88,0xe3,0x81,0x86,0xe3,0x81,0xab,  0x00};
const char menstrual_words_jp_d[] = {0x2c,0xe3,0x83,0x8a,0xe3,0x83,0x97,0xe3,0x82,0xad,0xe3,0x83,0xb3,0xe3,0x81,0xae,0xe5,0x8f,0x96,0xe6,0x9b,0xbf,0xe3,0x81,0x8c,0xe5,0xbf,0x98,0xe3,0x82,0x8c,0xe3,0x81,0x9a,0xe3,0x81,0xab,  0x00};
const char menstrual_words_jp_e[] = {0xe9,0x80,0x80,0xe5,0x8b,0xa4,0xe5,0x89,0x8d,0xe3,0x81,0xab,0xe3,0x83,0x8a,0xe3,0x83,0x97,0xe3,0x82,0xad,0xe3,0x83,0xb3,0xe3,0x82,0x92,0xe5,0x8f,0x96,0xe6,0x9b,0xbf,0xe3,0x82,0x8b,0xe3,0x81,0x93,0xe3,0x81,0xa8,  0x00};
const char menstrual_words_jp_e1[] = {0xe5,0xaf,0x9d,0xe3,0x82,0x8b,0xe5,0x89,0x8d,0xe3,0x81,0xab,0xe3,0x83,0x8a,0xe3,0x83,0x97,0xe3,0x82,0xad,0xe3,0x83,0xb3,0xe3,0x82,0x92,0xe5,0x8f,0x96,0xe6,0x9b,0xbf,0xe3,0x81,0xa6,0xe3,0x80,0x81,0xe7,0x9d,0xa1,0xe7,0x9c,0xa0,0xe3,0x81,0xae,0xe8,0xb3,0xaa,0xe3,0x81,0x8c,0xe9,0xab,0x98,0xe3,0x81,0xbe,0xe3,0x82,0x8b,  0x00};

const char menstrual_words_jp_f[] = {0xe4,0xbb,0x8a,0xe6,0x97,0xa5,0xe3,0x81,0xaf,0xe5,0xae,0x89,0xe5,0x85,0xa8,0xe6,0x9c,0x9f,0xe3,0x81,0xa7,0xe3,0x80,0x81,0xe6,0xac,0xa1,0xe3,0x81,0xae,0xe7,0x94,0x9f,0xe7,0x90,0x86,0xe3,0x81,0xbe,0xe3,0x81,0xa7,  0x00};
const char menstrual_words_jp_f1[] = {0xe6,0x97,0xa5,0xe3,0x81,0x8c,0xe3,0x81,0x82,0xe3,0x82,0x8b,  0x00};
const char menstrual_words_jp_g[] = {0xe4,0xbb,0x8a,0xe6,0x97,0xa5,0xe3,0x81,0xaf,0xe6,0x8e,0x92,0xe5,0x8d,0xb5,0xe6,0x9c,0x9f,0xe3,0x81,0xae,0xe7,0xac,0xac,  0x00};
const char menstrual_words_jp_h[] = {0xe3,0x80,0x81,0xe6,0xb0,0x97,0xe3,0x82,0x92,0xe3,0x81,0xa4,0xe3,0x81,0x91,0xe3,0x81,0xa6,0xe3,0x81,0xad,  0x00};
const char menstrual_words_jp_i[] = {0xe4,0xbb,0x8a,0xe6,0x97,0xa5,0xe3,0x81,0xaf,0xe6,0x8e,0x92,0xe5,0x8d,0xb5,0xe6,0x97,0xa5,0xe3,0x81,0xa7,0xe3,0x80,0x81,0xe7,0x89,0xb9,0xe5,0x88,0xa5,0xe3,0x81,0xab,0xe6,0xb3,0xa8,0xe6,0x84,0x8f,0xe3,0x81,0x97,0xe3,0x81,0xa6,0xe3,0x81,0xad,  0x00};

const char menstrual_words_jp_j[] = {0xe6,0x97,0xa5,0xe5,0xbe,0x8c,0xe3,0x81,0xab,0xe7,0x94,0x9f,0xe7,0x90,0x86,0xe3,0x81,0x8c,0xe3,0x81,0x82,0xe3,0x82,0x8a,0xe3,0x80,0x81,0xe6,0xba,0x96,0xe5,0x82,0x99,0xe3,0x81,0x97,0xe3,0x81,0xa6,0xe3,0x81,0x8a,0xe3,0x81,0x84,0xe3,0x81,0xa6,0xe3,0x81,0x8f,0xe3,0x81,0xa0,0xe3,0x81,0x95,0xe3,0x81,0x84,  0x00};
const char menstrual_words_jp_k[] = {0xe4,0xbb,0x8a,0xe6,0x97,0xa5,0xe3,0x81,0xaf,0xe7,0x94,0x9f,0xe7,0x90,0x86,0xe3,0x81,0xab,0xe3,0x81,0xaa,0xe3,0x82,0x8b,0xe4,0xba,0x88,0xe6,0xb8,0xac,0xe3,0x81,0xa7,0xe3,0x81,0x99,0xe3,0x81,0x8c,0xe3,0x80,0x81,0xe6,0x83,0x85,0xe5,0xa0,0xb1,0xe3,0x82,0x92,0xe6,0x9b,0xb4,0xe6,0x96,0xb0,0xe3,0x81,0x97,0xe3,0x81,0xa6,0xe3,0x81,0x8f,0xe3,0x81,0xa0,0xe3,0x81,0x95,0xe3,0x81,0x84,  0x00};
const char menstrual_words_jp_l[] = {0xe4,0xbb,0x8a,0xe5,0x9b,0x9e,0xe3,0x81,0xae,0xe7,0x94,0x9f,0xe7,0x90,0x86,0xe3,0x81,0xaf,  0x00};
const char menstrual_words_jp_l1[] = {0xe6,0x97,0xa5,0xe3,0x82,0x92,0xe9,0x81,0x85,0xe3,0x82,0x8c,0xe3,0x81,0xa6,  0x00};
const char menstrual_words_jp_m[] = {0xe3,0x80,0x81,0xe6,0x83,0x85,0xe5,0xa0,0xb1,0xe3,0x82,0x92,0xe6,0x9b,0xb4,0xe6,0x96,0xb0,0xe3,0x81,0x97,0xe3,0x81,0xa6,0xe3,0x81,0x8f,0xe3,0x81,0xa0,0xe3,0x81,0x95,0xe3,0x81,0x84,  0x00};
const char menstrual_words_jp_n[] = {0xe6,0x83,0x85,0xe5,0xa0,0xb1,0xe3,0x81,0x8c,0xe9,0x96,0x93,0xe9,0x81,0x95,0xe3,0x81,0xa3,0xe3,0x81,0xa6,0xe3,0x81,0x84,0xe3,0x82,0x8b,0xe3,0x81,0xaa,0xe3,0x82,0x89,0xe3,0x80,0x81,0xe6,0x9b,0xb4,0xe6,0x96,0xb0,0xe3,0x81,0x97,0xe3,0x81,0xa6,0xe3,0x81,0x8f,0xe3,0x81,0xa0,0xe3,0x81,0x95,0xe3,0x81,0x84,  0x00};
//��������ʾ��
const char pregnant_words_jp_a[] = {0xe4,0xbb,0x8a,0xe6,0x97,0xa5,0xe3,0x81,0xaf,0xe6,0x8e,0x92,0xe5,0x8d,0xb5,0xe6,0x9c,0x9f,0xe3,0x81,0xa7,0xe3,0x80,0x81,  0x00};
const char pregnant_words_jp_b[] = {0xe4,0xbb,0x8a,0xe6,0x97,0xa5,0xe3,0x81,0xaf,0xe6,0x8e,0x92,0xe5,0x8d,0xb5,0xe6,0x97,0xa5,0xe3,0x81,0xa7,0xe3,0x80,0x81,  0x00};
const char pregnant_words_jp_c[] = {0xe5,0xa6,0x8a,0xe5,0xa8,0xa0,0xe3,0x81,0x99,0xe3,0x82,0x8b,0xe7,0xa2,0xba,0xe7,0x8e,0x87,0xe3,0x81,0x8c,0xe9,0xab,0x98,0xe3,0x81,0x84,  0x00};
//��������ʾ��
const char gestation_words_jp_a[] = {0xe5,0xa6,0x8a,0xe5,0xa8,0xa0,0xe3,0x81,0x97,0xe3,0x81,0xa6,  0x00};
const char gestation_words_jp_b[] = {0xe9,0x80,0xb1,  0x00};
const char gestation_words_jp_b1[] = {0xe6,0x97,0xa5,0xe5,0xa6,0x8a,0xe5,0xa8,0xa0,0xe3,0x81,0x97,0xe3,0x81,0xa6,  0x00};
const char gestation_words_jp_b2[] = {0xe6,0x97,0xa5,0xe3,0x82,0x92,0xe7,0xb5,0x8c,0xe3,0x81,0xa6,  0x00};

const char gestation_words_jp_c[] = {0xe3,0x80,0x81,0xe5,0xae,0x9a,0xe6,0x9c,0x9f,0xe7,0x9a,0x84,0xe3,0x81,0xab,0xe6,0xa4,0x9c,0xe6,0x9f,0xbb,0xe3,0x81,0x97,0xe3,0x81,0xa6,0xe3,0x81,0x8f,0xe3,0x81,0xa0,0xe3,0x81,0x95,0xe3,0x81,0x84,  0x00};
const char gestation_words_jp_d[] = {0xe3,0x80,0x81,0xe5,0xae,0x9a,0xe6,0x9c,0x9f,0xe7,0x9a,0x84,0xe3,0x81,0xab,0xe6,0xa4,0x9c,0xe6,0x9f,0xbb,0xe3,0x81,0x97,0xe3,0x81,0xa6,0xe3,0x81,0x8f,0xe3,0x81,0xa0,0xe3,0x81,0x95,0xe3,0x81,0x84,  0x00};
const char gestation_words_jp_e[] = {0xe8,0xb5,0xa4,0xe3,0x81,0xa1,0xe3,0x82,0x83,0xe3,0x82,0x93,0xe3,0x81,0x8c,  0x00};
const char gestation_words_jp_e1[] = {0xe6,0x97,0xa5,0xe8,0xaa,0x95,0xe7,0x94,0x9f,0xe3,0x81,0x97,0xe3,0x81,0xa6,  0x00};
const char gestation_words_jp_f[] = {0xe8,0xb5,0xa4,0xe3,0x81,0xa1,0xe3,0x82,0x83,0xe3,0x82,0x93,0xe3,0x81,0xae,0xe8,0xaa,0x95,0xe7,0x94,0x9f,0xe7,0xa5,0x9d,0xe3,0x81,0x84,0xe3,0x81,0xae,0xe6,0xba,0x96,0xe5,0x82,0x99,0xe3,0x81,0xaf,0xe5,0x87,0xba,0xe6,0x9d,0xa5,0xe3,0x81,0xbe,0xe3,0x81,0x97,0xe3,0x81,0x9f,0xe3,0x81,0x8b,  0x00};
//��������ʾ��
const char hot_mom_words_jp_a[] = {0xe8,0xb5,0xa4,0xe3,0x81,0xa1,0xe3,0x82,0x83,0xe3,0x82,0x93,0xe3,0x81,0xaf,  0x00};
const char hot_mom_words_jp_a1[] = {0xe6,0x97,0xa5,0xe8,0xaa,0x95,0xe7,0x94,0x9f,0xe3,0x81,0x97,0xe3,0x81,0xa6,  0x00};

const char hot_mom_words_jp_b[] = {0xe3,0x80,0x81,0xe5,0x9b,0x9e,0xe5,0xbe,0xa9,0xe6,0x9c,0x9f,0xe3,0x81,0xae,0xe8,0xa1,0x9b,0xe7,0x94,0x9f,0xe3,0x82,0x92,0xe3,0x81,0x94,0xe6,0xb3,0xa8,0xe6,0x84,0x8f,0xe3,0x81,0x8f,0xe3,0x81,0xa0,0xe3,0x81,0x95,0xe3,0x81,0x84,  0x00};
const char hot_mom_words_jp_c[] = {0xe8,0xb5,0xa4,0xe3,0x81,0xa1,0xe3,0x82,0x83,0xe3,0x82,0x93,0xe3,0x81,0xaf,0xe6,0xba,0x80,0xe6,0x9c,0x88,0xe3,0x81,0xab,0xe3,0x81,0xaa,0xe3,0x82,0x8a,  0x00};
const char hot_mom_words_jp_d[] = {0xe8,0xb5,0xa4,0xe3,0x81,0xa1,0xe3,0x82,0x83,0xe3,0x82,0x93,0xe3,0x81,0xaf,0xe7,0x99,0xbe,0xe6,0x97,0xa5,0xe3,0x81,0xab,0xe3,0x81,0xaa,0xe3,0x82,0x8a,0xe3,0x81,0xbe,0xe3,0x81,0x97,0xe3,0x81,0x9f,  0x00};
const char hot_mom_words_jp_e[] = {0xe8,0xb5,0xa4,0xe3,0x81,0xa1,0xe3,0x82,0x83,0xe3,0x82,0x93,0xe3,0x81,0xaf,0xe4,0xb8,0x80,0xe6,0xad,0xb3,0xe3,0x81,0xab,0xe3,0x81,0xaa,0xe3,0x82,0x8a,0xe3,0x81,0xbe,0xe3,0x81,0x97,0xe3,0x81,0x9f,  0x00};
const char hot_mom_words_jp_f[] = {0xe3,0x80,0x81,0xe8,0xa8,0x98,0xe5,0xbf,0xb5,0xe3,0x81,0x99,0xe3,0x81,0xb9,0xe3,0x81,0x8d,0xe4,0xb8,0x80,0xe6,0x97,0xa5,0xe3,0x81,0xa7,0xe3,0x81,0x99,  0x00};
const char hot_mom_words_jp_g[] = {0xe5,0x80,0x8b,0xe4,0xba,0xba,0xe6,0x83,0x85,0xe5,0xa0,0xb1,0xe3,0x82,0x92,0xe6,0x9b,0xb4,0xe6,0x96,0xb0,0xe3,0x81,0x97,0xe3,0x81,0xa6,0xe3,0x81,0x8f,0xe3,0x81,0xa0,0xe3,0x81,0x95,0xe3,0x81,0x84,  0x00};


/*********************************************************************
* local functions
*/

void Phys_JapaneseRemind(uint8_t type, uint16_t days, uint8_t hours, uint8_t minutes)
{
    if(type == PHYS_REMIND_MENSTRUAL_PERIOD)
    {//����
        if(hours == 7 && minutes == 30)
        {//���쾭�ڵ�x�죬�ǵû�������
            //"���쾭�ڵ�"
            memcpy(phys_remind_info_s.remind_info_buf, menstrual_words_jp_a, strlen(menstrual_words_jp_a));
            phys_remind_info_s.data_size = strlen(menstrual_words_jp_a);
            //days 
            Util_Itoa(days, (char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size]);
            phys_remind_info_s.data_size = strlen((const char *)phys_remind_info_s.remind_info_buf);
            //��
            memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], menstrual_words_jp_b, strlen(menstrual_words_jp_b));            
            phys_remind_info_s.data_size += strlen(menstrual_words_jp_b);
            //,�ǵø���������
            memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], menstrual_words_jp_d, strlen(menstrual_words_jp_d));            
            phys_remind_info_s.data_size += strlen(menstrual_words_jp_d);
        }
        else if((hours == 11 || hours == 14) && minutes == 00)
        {//�������,�ǵø���������
            //�������
            memcpy(phys_remind_info_s.remind_info_buf, menstrual_words_jp_c, strlen(menstrual_words_jp_c));
            phys_remind_info_s.data_size = strlen(menstrual_words_jp_c);
            //���ǵø���������
            memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], menstrual_words_jp_d, strlen(menstrual_words_jp_d));            
            phys_remind_info_s.data_size += strlen(menstrual_words_jp_d);
        }
        else if(hours == 17 && minutes == 30)
        {//�°�ǰ�ǵø��������� 
            memcpy(phys_remind_info_s.remind_info_buf, menstrual_words_jp_e, strlen(menstrual_words_jp_e));
            phys_remind_info_s.data_size = strlen(menstrual_words_jp_e);
        }
        else if(hours == 21 && minutes == 30)
        {//˯ǰ����������,ע��˯������
            memcpy(phys_remind_info_s.remind_info_buf, menstrual_words_jp_e1, strlen(menstrual_words_jp_e1));
            phys_remind_info_s.data_size = strlen(menstrual_words_jp_e1);
        }
    }
    else if(type == PHYS_REMIND_SAFETY_PERIOD)
    {//��ȫ��
        if(hours == 9 && minutes == 0)
        {//���찲ȫ�ڣ����´ξ���X��
            //���찲ȫ�ڣ����´ξ���
            memcpy(phys_remind_info_s.remind_info_buf, menstrual_words_jp_f, strlen(menstrual_words_jp_f));
            phys_remind_info_s.data_size = strlen(menstrual_words_jp_f);
            //days ��
            Util_Itoa(days, (char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size]);
            phys_remind_info_s.data_size = strlen((const char *)phys_remind_info_s.remind_info_buf);
            //��
            memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], menstrual_words_jp_f1, strlen(menstrual_words_jp_f1));            
            phys_remind_info_s.data_size += strlen(menstrual_words_jp_f1);
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
            memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], menstrual_words_jp_j, strlen(menstrual_words_jp_j));            
            phys_remind_info_s.data_size += strlen(menstrual_words_jp_j);   
        }
    }
    else if(type == PHYS_REMIND_OVIPOSIT_PERIOD)
    {//������
        if(hours == 9 && minutes == 0)
        {//�����������ڵ�X�죬С��Ŷ
            //�����������ڵ�
            memcpy(phys_remind_info_s.remind_info_buf, menstrual_words_jp_g, strlen(menstrual_words_jp_g));
            phys_remind_info_s.data_size = strlen(menstrual_words_jp_g);
            //days 
            Util_Itoa(days, (char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size]);
            phys_remind_info_s.data_size = strlen((const char *)phys_remind_info_s.remind_info_buf);
            //��
            memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], menstrual_words_jp_b, strlen(menstrual_words_jp_b));            
            phys_remind_info_s.data_size += strlen(menstrual_words_jp_b);
            //��С��Ŷ
            memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], menstrual_words_jp_h, strlen(menstrual_words_jp_h));            
            phys_remind_info_s.data_size += strlen(menstrual_words_jp_h);
        }
    }
    else if(type == PHYS_REMIND_OVIPOSIT_DAY)
    {//������
        if(hours == 9 && minutes == 0)
        {//�����������գ�Ҫ����С��Ŷ
            memcpy(phys_remind_info_s.remind_info_buf, menstrual_words_jp_i, strlen(menstrual_words_jp_i));
            phys_remind_info_s.data_size = strlen(menstrual_words_jp_i);
        }
    }
    else if(type == PHYS_REMIND_OVIPOSIT_PERIOD1)
    {//������������
        if(hours == 9 && minutes == 0)
        {//���������ڣ����м��ʽϴ�
            //���������ڣ�
            memcpy(phys_remind_info_s.remind_info_buf, pregnant_words_jp_a, strlen(pregnant_words_jp_a));
            phys_remind_info_s.data_size = strlen(pregnant_words_jp_a);
            //���м��ʽϴ�
            memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], pregnant_words_jp_c, strlen(pregnant_words_jp_c));            
            phys_remind_info_s.data_size += strlen(pregnant_words_jp_c);
        }
    }
    else if(type == PHYS_REMIND_OVIPOSIT_DAY1)
    {//������������
        if(hours == 9 && minutes == 0)
        {//���������գ����м��ʽϴ�
            //���������գ�
            memcpy(phys_remind_info_s.remind_info_buf, pregnant_words_jp_b, strlen(pregnant_words_jp_b));
            phys_remind_info_s.data_size = strlen(pregnant_words_jp_b);
            //���м��ʽϴ�
            memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], pregnant_words_jp_c, strlen(pregnant_words_jp_c));            
            phys_remind_info_s.data_size += strlen(pregnant_words_jp_c);
        }
    }
    else if(type == PHYS_REMIND_FORECAST_MENSTRUAL_PERIOD)
    {//Ԥ�⾭��
        if(hours == 7 && minutes == 30 && days == 1)
        {//Ԥ�������뾭�ڣ��������Ϣ
            memcpy(phys_remind_info_s.remind_info_buf, menstrual_words_jp_k, strlen(menstrual_words_jp_k));
            phys_remind_info_s.data_size = strlen(menstrual_words_jp_k);
        } 
        else if(hours == 9 && minutes == 0 && days > 1)
        {//�����¾��ӳ�X�죬�������Ϣ
            //�����¾��ӳ�
            memcpy(phys_remind_info_s.remind_info_buf, menstrual_words_jp_l, strlen(menstrual_words_jp_l));
            phys_remind_info_s.data_size = strlen(menstrual_words_jp_l);
            //days 
            Util_Itoa(days, (char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size]);
            phys_remind_info_s.data_size = strlen((const char *)phys_remind_info_s.remind_info_buf);
            //��
            memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], menstrual_words_jp_l1, strlen(menstrual_words_jp_l1));            
            phys_remind_info_s.data_size += strlen(menstrual_words_jp_l1);
            //���������Ϣ
            memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], menstrual_words_jp_m, strlen(menstrual_words_jp_m));            
            phys_remind_info_s.data_size += strlen(menstrual_words_jp_m);
        } 
    }
    else if(type == PHYS_REMIND_PREGNANCY_ONE)
    {//���е�һ�׶Σ�1~3���£�
        if(hours == 9 && minutes == 0)
        {//���ѻ���X��X�죬�붨�������
            //���ѻ���
            if(days < 7)
            {
                memcpy(phys_remind_info_s.remind_info_buf, gestation_words_jp_a, strlen(gestation_words_jp_a));
                phys_remind_info_s.data_size = strlen(gestation_words_jp_a); 
                //days%7 
                Util_Itoa(days % 7, (char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size]);
                phys_remind_info_s.data_size = strlen((const char *)phys_remind_info_s.remind_info_buf);
                memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], gestation_words_jp_b2, strlen(gestation_words_jp_b2));            
                phys_remind_info_s.data_size += strlen(gestation_words_jp_b2);
            }
            else
            {
                //n��
                if(days / 7 != 0)
                {
                    //days/7 
                    Util_Itoa(days / 7, (char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size]);
                    phys_remind_info_s.data_size = strlen((const char *)phys_remind_info_s.remind_info_buf);
                    memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], gestation_words_jp_b, strlen(gestation_words_jp_b));            
                    phys_remind_info_s.data_size += strlen(gestation_words_jp_b);
                }
                //n��
                if(days % 7 != 0)
                {
                    //days%7 
                    Util_Itoa(days % 7, (char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size]);
                    phys_remind_info_s.data_size = strlen((const char *)phys_remind_info_s.remind_info_buf);
                    memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], gestation_words_jp_b1, strlen(gestation_words_jp_b1));            
                    phys_remind_info_s.data_size += strlen(gestation_words_jp_b1);
                }
            }
            //,�붨�������
            memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], gestation_words_jp_c, strlen(gestation_words_jp_c));            
            phys_remind_info_s.data_size += strlen(gestation_words_jp_c);
        } 
    }
    else if(type == PHYS_REMIND_PREGNANCY_TWO)
    {//���еڶ��׶Σ�3~6���£�
        if(hours == 9 && minutes == 0)
        {//09��00���ѻ���X��X�죬�뱣�ֺ�����
            //n��
            if(days / 7 != 0)
            {
                //days/7 
                Util_Itoa(days / 7, (char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size]);
                phys_remind_info_s.data_size = strlen((const char *)phys_remind_info_s.remind_info_buf);
                memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], gestation_words_jp_b, strlen(gestation_words_jp_b));            
                phys_remind_info_s.data_size += strlen(gestation_words_jp_b);
            }
            //n��
            if(days % 7 != 0)
            {
                //days%7 
                Util_Itoa(days % 7, (char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size]);
                phys_remind_info_s.data_size = strlen((const char *)phys_remind_info_s.remind_info_buf);
                memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], gestation_words_jp_b1, strlen(gestation_words_jp_b1));            
                phys_remind_info_s.data_size += strlen(gestation_words_jp_b1);
            }
            //,�붨�������
            memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], gestation_words_jp_d, strlen(gestation_words_jp_d));            
            phys_remind_info_s.data_size += strlen(gestation_words_jp_d);
        }
    }
    else if(type == PHYS_REMIND_PREGNANCY_THU)
    {//���е����׶Σ�6����~����ǰ��
        if(hours == 9 && minutes == 0)
        {//09��00�౦������X�죬�뱣�ֺ�����
            //�౦������
            memcpy(phys_remind_info_s.remind_info_buf, gestation_words_jp_e, strlen(gestation_words_jp_e));
            phys_remind_info_s.data_size = strlen(gestation_words_jp_e);
            //n��
            Util_Itoa(days, (char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size]);
            phys_remind_info_s.data_size = strlen((const char *)phys_remind_info_s.remind_info_buf);
            memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], gestation_words_jp_e1, strlen(gestation_words_jp_e1));            
            phys_remind_info_s.data_size += strlen(gestation_words_jp_e1);
            
            //,�붨�������
            memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], gestation_words_jp_d, strlen(gestation_words_jp_d));            
            phys_remind_info_s.data_size += strlen(gestation_words_jp_d);
        }
    }
    else if(type == PHYS_REMIND_PREGNANCY_BIRTH)
    {//���е��Ľ׶Σ�����������
        if(hours == 9 && minutes == 0)
        {//Ԥף�������쵮����׼��������
            memcpy(phys_remind_info_s.remind_info_buf, gestation_words_jp_f, strlen(gestation_words_jp_f));
            phys_remind_info_s.data_size = strlen(gestation_words_jp_f);
        }
    }
    else if(type == PHYS_REMIND_PREGNANCY_ERROR)
    {//������Ϣ���ڣ�������Ϣ
        if(hours == 9 && minutes == 0)
        {//09��00��������ĸ�����Ϣ(���������Ϣ��Ч,����Ϣ������)
            memcpy(phys_remind_info_s.remind_info_buf, hot_mom_words_jp_g, strlen(hot_mom_words_jp_g));
            phys_remind_info_s.data_size = strlen(hot_mom_words_jp_g);
        }
    }
    else if(type == PHYS_REMIND_BABY_BORN)
    {//����������
        if(hours == 9 && minutes == 30)
        {
            if(days <= 10)
            {//����������X�죬�ָ���ע������
                //����������
                memcpy(phys_remind_info_s.remind_info_buf, hot_mom_words_jp_a, strlen(hot_mom_words_jp_a));
                phys_remind_info_s.data_size = strlen(hot_mom_words_jp_a);
                //days 
                Util_Itoa(days, (char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size]);
                phys_remind_info_s.data_size = strlen((const char *)phys_remind_info_s.remind_info_buf);
                //��
                memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], hot_mom_words_jp_a1, strlen(hot_mom_words_jp_a1));            
                phys_remind_info_s.data_size += strlen(hot_mom_words_jp_a1);
                //����ָ���ע������
                memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], hot_mom_words_jp_b, strlen(hot_mom_words_jp_b));            
                phys_remind_info_s.data_size += strlen(hot_mom_words_jp_b);
            }
            else if(days == 30)
            {//��������(30��)�ˣ�ֵ�ü����һ��              
                memcpy(phys_remind_info_s.remind_info_buf, hot_mom_words_jp_c, strlen(hot_mom_words_jp_c));
                phys_remind_info_s.data_size = strlen(hot_mom_words_jp_c); 
                memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], hot_mom_words_jp_f, strlen(hot_mom_words_jp_f));            
                phys_remind_info_s.data_size += strlen(hot_mom_words_jp_f); 
            }
            else if(days == 100)
            {//����һ���죬ֵ�ü����һ��
                memcpy(phys_remind_info_s.remind_info_buf, hot_mom_words_jp_d, strlen(hot_mom_words_jp_d));
                phys_remind_info_s.data_size = strlen(hot_mom_words_jp_d); 
                memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], hot_mom_words_jp_f, strlen(hot_mom_words_jp_f));            
                phys_remind_info_s.data_size += strlen(hot_mom_words_jp_f);
            }
            else if(days == 365)
            {//����һ���ֵ꣬�ü����һ��
                memcpy(phys_remind_info_s.remind_info_buf, hot_mom_words_jp_e, strlen(hot_mom_words_jp_e));
                phys_remind_info_s.data_size = strlen(hot_mom_words_jp_e); 
                memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], hot_mom_words_jp_f, strlen(hot_mom_words_jp_f));            
                phys_remind_info_s.data_size += strlen(hot_mom_words_jp_f);
            }
            else if(days < 365)
            {//����������x��
                //����������
                memcpy(phys_remind_info_s.remind_info_buf, hot_mom_words_jp_a, strlen(hot_mom_words_jp_a));
                phys_remind_info_s.data_size = strlen(hot_mom_words_jp_a);
                //days 
                Util_Itoa(days, (char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size]);
                phys_remind_info_s.data_size = strlen((const char *)phys_remind_info_s.remind_info_buf);
                //��
                memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], hot_mom_words_jp_a1, strlen(hot_mom_words_jp_a1));            
                phys_remind_info_s.data_size += strlen(hot_mom_words_jp_a1);
            }
            else
            {//��������ĸ�����Ϣ(���������Ϣ��Ч,����Ϣ������)
                memcpy(phys_remind_info_s.remind_info_buf, hot_mom_words_jp_g, strlen(hot_mom_words_jp_g));
                phys_remind_info_s.data_size = strlen(hot_mom_words_jp_g);
            }
        }
    }
    else
    {//�Ƿ�
        if(hours == 9 && minutes == 0)
        {//09��00��Ϣ���������
            memcpy(phys_remind_info_s.remind_info_buf, menstrual_words_jp_n, strlen(menstrual_words_jp_n));
            phys_remind_info_s.data_size = strlen(menstrual_words_jp_n);  
        }
    }        
}





