/***************************Copyright(c)***********************************
**                       Veepoo Tech
**                  COPYRIGHT (C) 2016-2019
**-----------File Info-----------------------------------------------------
** File        name:     physiological_remind_words_vietnanese.c
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
const char menstrual_words_vi_a[] = {0x48,0xc3,0xb4,0x6d,0x20,0x6e,0x61,0x79,0x20,0x6c,0xc3,0xa0,0x20,0x6e,0x67,0xc3,0xa0,0x79,0x20,  0x00};
const char menstrual_words_vi_a1[] = {0x20,0x63,0xe1,0xbb,0xa7,0x61,0x20,0x6b,0xe1,0xbb,0xb3,0x20,0x6b,0x69,0x6e,0x68,0x2e,  0x00};
const char menstrual_words_vi_b[] = {0x20,0x6e,0x67,0xc3,0xa0,0x79,  0x00};
const char menstrual_words_vi_c[] = {0x58,0x69,0x6e,0x20,0xc4,0x91,0xe1,0xbb,0xab,0x6e,0x67,0x20,0x6e,0x67,0xe1,0xbb,0x93,0x69,0x20,0x6c,0xc3,0xa2,0x75,0x21,  0x00};
const char menstrual_words_vi_d[] = {0x4e,0x68,0xe1,0xbb,0x9b,0x20,0x74,0x68,0x61,0x79,0x20,0x62,0xc4,0x83,0x6e,0x67,0x20,0x76,0xe1,0xbb,0x87,0x20,0x73,0x69,0x6e,0x68,0x21,  0x00};
const char menstrual_words_vi_e[] = {0x4e,0x68,0xe1,0xbb,0x9b,0x20,0x74,0x68,0x61,0x79,0x20,0x62,0xc4,0x83,0x6e,0x67,0x20,0x76,0xe1,0xbb,0x87,0x20,0x73,0x69,0x6e,0x68,0x20,0x74,0x72,0xc6,0xb0,0xe1,0xbb,0x9b,0x63,0x20,0x6b,0x68,0x69,0x20,0x74,0x61,0x6e,0x20,0x63,0x61,0x21,  0x00};
const char menstrual_words_vi_e1[] = {0x54,0x68,0x61,0x79,0x20,0x62,0xc4,0x83,0x6e,0x67,0x20,0x76,0xe1,0xbb,0x87,0x20,0x73,0x69,0x6e,0x68,0x20,0x74,0x72,0xc6,0xb0,0xe1,0xbb,0x9b,0x63,0x20,0x6b,0x68,0x69,0x20,0xc4,0x91,0x69,0x20,0x6e,0x67,0xe1,0xbb,0xa7,0x2c,0x20,0x63,0x68,0xc3,0xba,0x20,0xc3,0xbd,0x20,0xc4,0x91,0xe1,0xba,0xbf,0x6e,0x20,0x63,0x68,0xe1,0xba,0xa5,0x74,0x20,0x6c,0xc6,0xb0,0xe1,0xbb,0xa3,0x6e,0x67,0x20,0x67,0x69,0xe1,0xba,0xa5,0x63,0x20,0x6e,0x67,0xe1,0xbb,0xa7,0x21,  0x00};

const char menstrual_words_vi_f[] = {0x48,0xc3,0xb4,0x6d,0x20,0x6e,0x61,0x79,0x20,0x6c,0xc3,0xa0,0x20,0x6b,0x68,0x6f,0xe1,0xba,0xa3,0x6e,0x67,0x20,0x74,0x68,0xe1,0xbb,0x9d,0x69,0x20,0x67,0x69,0x61,0x6e,0x20,0x61,0x6e,0x20,0x74,0x6f,0xc3,0xa0,0x6e,0x2c,0x20,0xc4,0x91,0xe1,0xba,0xbf,0x6e,0x20,0x6b,0xe1,0xbb,0xb3,0x20,0x6b,0x69,0x6e,0x68,0x20,0x6e,0x67,0x75,0x79,0xe1,0xbb,0x87,0x74,0x20,0x74,0x69,0xe1,0xba,0xbf,0x70,0x20,0x74,0x68,0x65,0x6f,0x20,0x63,0xc3,0xb2,0x6e,0x20,  0x00};
const char menstrual_words_vi_g[] = {0x48,0xc3,0xb4,0x6d,0x20,0x6e,0x61,0x79,0x20,0x6c,0xc3,0xa0,0x20,0x6e,0x67,0xc3,0xa0,0x79,0x20,0x74,0x68,0xe1,0xbb,0xa9,0x20,  0x00};
const char menstrual_words_vi_h[] = {0x20,0x63,0xe1,0xbb,0xa7,0x61,0x20,0x6b,0xe1,0xbb,0xb3,0x20,0x72,0xe1,0xbb,0xa5,0x6e,0x67,0x20,0x74,0x72,0xe1,0xbb,0xa9,0x6e,0x67,0x2c,0x20,0x68,0xc3,0xa3,0x79,0x20,0x63,0xe1,0xba,0xa9,0x6e,0x20,0x74,0x68,0xe1,0xba,0xad,0x6e,0x21,  0x00};
const char menstrual_words_vi_i[] = {0x48,0xc3,0xb4,0x6d,0x20,0x6e,0x61,0x79,0x20,0x6c,0xc3,0xa0,0x20,0x6e,0x67,0xc3,0xa0,0x79,0x20,0x72,0xe1,0xbb,0xa5,0x6e,0x67,0x20,0x74,0x72,0xe1,0xbb,0xa9,0x6e,0x67,0x2c,0x20,0x63,0xc3,0xa0,0x6e,0x67,0x20,0x70,0x68,0xe1,0xba,0xa3,0x69,0x20,0x63,0xe1,0xba,0xa9,0x6e,0x20,0x74,0x68,0xe1,0xba,0xad,0x6e,0x20,0x68,0xc6,0xa1,0x6e,0x21,  0x00};

const char menstrual_words_vi_j1[] = {0x53,0x61,0x75,0x20,  0x00};
const char menstrual_words_vi_j[] = {0x20,0x6e,0x67,0xc3,0xa0,0x79,0x20,0x73,0xe1,0xba,0xbd,0x20,0x62,0xe1,0xba,0xaf,0x74,0x20,0xc4,0x91,0xe1,0xba,0xa7,0x75,0x20,0x6b,0xe1,0xbb,0xb3,0x20,0x6b,0x69,0x6e,0x68,0x2c,0x20,0x76,0x75,0x69,0x20,0x6c,0xc3,0xb2,0x6e,0x67,0x20,0x63,0x68,0x75,0xe1,0xba,0xa9,0x6e,0x20,0x62,0xe1,0xbb,0x8b,0x20,0x73,0xe1,0xba,0xb5,0x6e,0x21,  0x00};
const char menstrual_words_vi_k[] = {0x44,0xe1,0xbb,0xb1,0x20,0x74,0xc3,0xad,0x6e,0x68,0x20,0x68,0xc3,0xb4,0x6d,0x20,0x6e,0x61,0x79,0x20,0x62,0xe1,0xba,0xaf,0x74,0x20,0xc4,0x91,0xe1,0xba,0xa7,0x75,0x20,0x6b,0xe1,0xbb,0xb3,0x20,0x6b,0x69,0x6e,0x68,0x20,0x6e,0x67,0x75,0x79,0xe1,0xbb,0x87,0x74,0x2c,0x20,0x76,0x75,0x69,0x20,0x6c,0xc3,0xb2,0x6e,0x67,0x20,0x63,0xe1,0xba,0xad,0x70,0x20,0x6e,0x68,0xe1,0xba,0xad,0x74,0x20,0x74,0x68,0xc3,0xb4,0x6e,0x67,0x20,0x74,0x69,0x6e,0x21,  0x00};
const char menstrual_words_vi_l[] = {0x4b,0xe1,0xbb,0xb3,0x20,0x6b,0x69,0x6e,0x68,0x20,0x6e,0x67,0x75,0x79,0xe1,0xbb,0x87,0x74,0x20,0x6c,0xe1,0xba,0xa7,0x6e,0x20,0x6e,0xc3,0xa0,0x79,0x20,0xc4,0x91,0xc3,0xa3,0x20,0x63,0x68,0xe1,0xba,0xad,0x6d,0x20,  0x00};
const char menstrual_words_vi_m[] = {0x2c,0x20,0x76,0x75,0x69,0x20,0x6c,0xc3,0xb2,0x6e,0x67,0x20,0x63,0xe1,0xba,0xad,0x70,0x20,0x6e,0x68,0xe1,0xba,0xad,0x74,0x20,0x74,0x68,0xc3,0xb4,0x6e,0x67,0x20,0x74,0x69,0x6e,0x21,  0x00};
const char menstrual_words_vi_n[] = {0x4c,0xe1,0xbb,0x97,0x69,0x20,0x74,0x68,0xc3,0xb4,0x6e,0x67,0x20,0x74,0x69,0x6e,0x2c,0x20,0x76,0x75,0x69,0x20,0x6c,0xc3,0xb2,0x6e,0x67,0x20,0x63,0xe1,0xba,0xad,0x70,0x20,0x6e,0x68,0xe1,0xba,0xad,0x74,0x21,  0x00};
//��������ʾ��
const char pregnant_words_vi_a[] = {0x48,0xc3,0xb4,0x6d,0x20,0x6e,0x61,0x79,0x20,0x74,0x68,0x75,0xe1,0xbb,0x99,0x63,0x20,0x6b,0xe1,0xbb,0xb3,0x20,0x72,0xe1,0xbb,0xa5,0x6e,0x67,0x20,0x74,0x72,0xe1,0xbb,0xa9,0x6e,0x67,  0x00};
const char pregnant_words_vi_b[] = {0x48,0xc3,0xb4,0x6d,0x20,0x6e,0x61,0x79,0x20,0x6c,0xc3,0xa0,0x20,0x6e,0x67,0xc3,0xa0,0x79,0x20,0x72,0xe1,0xbb,0xa5,0x6e,0x67,0x20,0x74,0x72,0xe1,0xbb,0xa9,0x6e,0x67,0x2c,  0x00};
const char pregnant_words_vi_c[] = {0x2c,0x20,0x78,0xc3,0xa1,0x63,0x20,0x73,0x75,0xe1,0xba,0xa5,0x74,0x20,0x74,0x68,0xe1,0xbb,0xa5,0x20,0x74,0x68,0x61,0x69,0x20,0x72,0xe1,0xba,0xa5,0x74,0x20,0x6c,0xe1,0xbb,0x9b,0x6e,0x2e,  0x00};
//��������ʾ��
const char gestation_words_vi_a[] = {0x42,0xe1,0xba,0xa1,0x6e,0x20,0xc4,0x91,0xc3,0xa3,0x20,0x6d,0x61,0x6e,0x67,0x20,0x74,0x68,0x61,0x69,0x20,  0x00};
const char gestation_words_vi_b[] = {0x20,0x74,0x75,0xe1,0xba,0xa7,0x6e,0x20,  0x00};
const char gestation_words_vi_c[] = {0x2c,0x20,0x68,0xc3,0xa3,0x79,0x20,0xc4,0x91,0x69,0x20,0x6b,0x69,0xe1,0xbb,0x83,0x6d,0x20,0x74,0x72,0x61,0x20,0xc4,0x91,0xe1,0xbb,0x8b,0x6e,0x68,0x20,0x6b,0xe1,0xbb,0xb3,  0x00};
const char gestation_words_vi_d[] = {0x2c,0x20,0x68,0xc3,0xa3,0x79,0x20,0xc4,0x91,0x69,0x20,0x6b,0x69,0xe1,0xbb,0x83,0x6d,0x20,0x74,0x72,0x61,0x20,0xc4,0x91,0xe1,0xbb,0x8b,0x6e,0x68,0x20,0x6b,0xe1,0xbb,0xb3,  0x00};
const char gestation_words_vi_e[] = {0x53,0x69,0x6e,0x68,0x20,0x72,0x61,0x20,0x74,0xe1,0xbb,0xab,0x20,0x62,0xc3,0xa9,0x20,  0x00};
const char gestation_words_vi_f[] = {0x54,0x68,0x69,0xc3,0xaa,0x6e,0x20,0x74,0x68,0xe1,0xba,0xa7,0x6e,0x20,0x62,0xc3,0xa9,0x20,0x62,0xe1,0xbb,0x8f,0x6e,0x67,0x20,0x73,0xe1,0xba,0xbd,0x20,0x72,0x61,0x20,0xc4,0x91,0xe1,0xbb,0x9d,0x69,0x20,0x76,0xc3,0xa0,0x6f,0x20,0x6e,0x67,0xc3,0xa0,0x79,0x20,0x68,0xc3,0xb4,0x6d,0x20,0x6e,0x61,0x79,0x2c,0x20,0x62,0xe1,0xba,0xa1,0x6e,0x20,0xc4,0x91,0xc3,0xa3,0x20,0x73,0xe1,0xba,0xb5,0x6e,0x20,0x73,0xc3,0xa0,0x6e,0x67,0x20,0x63,0x68,0xc6,0xb0,0x61,0x3f,  0x00};
//��������ʾ��
const char hot_mom_words_vi_a[] = {0x45,0x6d,0x20,0x62,0xc3,0xa9,0x20,0xc4,0x91,0xc3,0xa3,0x20,0x73,0x69,0x6e,0x68,0x20,0xc4,0x91,0xc6,0xb0,0xe1,0xbb,0xa3,0x63,0x20,  0x00};
const char hot_mom_words_vi_b[] = {0x2c,0x20,0x68,0xc3,0xa3,0x79,0x20,0x67,0x69,0xe1,0xbb,0xaf,0x20,0x67,0xc3,0xac,0x6e,0x20,0x73,0xe1,0xbb,0xa9,0x63,0x20,0x6b,0x68,0xe1,0xbb,0x8f,0x65,0x20,0x73,0x61,0x75,0x20,0x73,0x69,0x6e,0x68,  0x00};
const char hot_mom_words_vi_c[] = {0x45,0x6d,0x20,0x62,0xc3,0xa9,0x20,0xc4,0x91,0xc3,0xa3,0x20,0xc4,0x91,0xe1,0xba,0xa7,0x79,0x20,0x74,0x68,0xc3,0xa1,0x6e,0x67,0x20,  0x00};
const char hot_mom_words_vi_d[] = {0x45,0x6d,0x20,0x62,0xc3,0xa9,0x20,0xc4,0x91,0xc3,0xa3,0x20,0x62,0x61,0x20,0x74,0x68,0xc3,0xa1,0x6e,0x67,0x20,0x6d,0xc6,0xb0,0xe1,0xbb,0x9d,0x69,0x20,0x6e,0x67,0xc3,0xa0,0x79,  0x00};
const char hot_mom_words_vi_e[] = {0x45,0x6d,0x20,0x62,0xc3,0xa9,0x20,0xc4,0x91,0xc3,0xa3,0x20,0x6d,0xe1,0xbb,0x99,0x74,0x20,0x74,0x75,0xe1,0xbb,0x95,0x69,0x20,0x74,0x72,0xc3,0xb2,0x6e,  0x00};
const char hot_mom_words_vi_f[] = {0x2c,0x20,0x6d,0xe1,0xbb,0x99,0x74,0x20,0x6e,0x67,0xc3,0xa0,0x79,0x20,0x74,0x68,0xe1,0xba,0xad,0x74,0x20,0xc4,0x91,0xc3,0xa1,0x6e,0x67,0x20,0x6e,0x68,0xe1,0xbb,0x9b,  0x00};
const char hot_mom_words_vi_g[] = {0x56,0x75,0x69,0x20,0x6c,0xc3,0xb2,0x6e,0x67,0x20,0x63,0xe1,0xba,0xad,0x70,0x20,0x6e,0x68,0xe1,0xba,0xad,0x74,0x20,0x74,0x68,0xc3,0xb4,0x6e,0x67,0x20,0x74,0x69,0x6e,0x20,0x63,0xc3,0xa1,0x20,0x6e,0x68,0xc3,0xa2,0x6e,0x20,0x63,0xe1,0xbb,0xa7,0x61,0x20,0x62,0xe1,0xba,0xa1,0x6e,  0x00};


/*********************************************************************
* local functions
*/

void Phys_VietnaneseRemind(uint8_t type, uint16_t days, uint8_t hours, uint8_t minutes)
{
    if(type == PHYS_REMIND_MENSTRUAL_PERIOD)
    {//����
        if(hours == 7 && minutes == 30)
        {//���쾭�ڵ�x�죬�ǵû�������
            //"���쾭�ڵ�"
            memcpy(phys_remind_info_s.remind_info_buf, menstrual_words_vi_a, strlen(menstrual_words_vi_a));
            phys_remind_info_s.data_size = strlen(menstrual_words_vi_a);
            //days 
            Util_Itoa(days, (char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size]);
            phys_remind_info_s.data_size = strlen((const char *)phys_remind_info_s.remind_info_buf);
            //��
            memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], menstrual_words_vi_a1, strlen(menstrual_words_vi_a1));            
            phys_remind_info_s.data_size += strlen(menstrual_words_vi_a1);
            //,�ǵø���������
            memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], menstrual_words_vi_d, strlen(menstrual_words_vi_d));            
            phys_remind_info_s.data_size += strlen(menstrual_words_vi_d);
        }
        else if((hours == 11 || hours == 14) && minutes == 00)
        {//�������,�ǵø���������
            //�������
            memcpy(phys_remind_info_s.remind_info_buf, menstrual_words_vi_c, strlen(menstrual_words_vi_c));
            phys_remind_info_s.data_size = strlen(menstrual_words_vi_c);
            //���ǵø���������
            memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], menstrual_words_vi_d, strlen(menstrual_words_vi_d));            
            phys_remind_info_s.data_size += strlen(menstrual_words_vi_d);
        }
        else if(hours == 17 && minutes == 30)
        {//�°�ǰ�ǵø��������� 
            memcpy(phys_remind_info_s.remind_info_buf, menstrual_words_vi_e, strlen(menstrual_words_vi_e));
            phys_remind_info_s.data_size = strlen(menstrual_words_vi_e);
        }
        else if(hours == 21 && minutes == 30)
        {//˯ǰ����������,ע��˯������
            memcpy(phys_remind_info_s.remind_info_buf, menstrual_words_vi_e1, strlen(menstrual_words_vi_e1));
            phys_remind_info_s.data_size = strlen(menstrual_words_vi_e1);
        }
    }
    else if(type == PHYS_REMIND_SAFETY_PERIOD)
    {//��ȫ��
        if(hours == 9 && minutes == 0)
        {//���찲ȫ�ڣ����´ξ���X��
            //���찲ȫ�ڣ����´ξ���
            memcpy(phys_remind_info_s.remind_info_buf, menstrual_words_vi_f, strlen(menstrual_words_vi_f));
            phys_remind_info_s.data_size = strlen(menstrual_words_vi_f);
            //days ��
            Util_Itoa(days, (char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size]);
            phys_remind_info_s.data_size = strlen((const char *)phys_remind_info_s.remind_info_buf);
            //��
            memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], menstrual_words_vi_b, strlen(menstrual_words_vi_b));            
            phys_remind_info_s.data_size += strlen(menstrual_words_vi_b);
        }
    }
    else if(type == PHYS_REMIND_SAFETY_PERIOD1)
    {//��ȫ��
        if(hours == 9 && minutes == 0)
        {//X�����뾭��(<=3��)��������׼��menstrual_words_vi_j1
            memcpy((char *)&phys_remind_info_s.remind_info_buf[0], menstrual_words_vi_j1, strlen(menstrual_words_vi_j1));            
            phys_remind_info_s.data_size += strlen(menstrual_words_vi_j1); 
            //days 
            Util_Itoa(days, (char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size]);
            phys_remind_info_s.data_size = strlen((const char *)phys_remind_info_s.remind_info_buf);
            //�����뾭�ڣ�������׼��
            memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], menstrual_words_vi_j, strlen(menstrual_words_vi_j));            
            phys_remind_info_s.data_size += strlen(menstrual_words_vi_j);   
        }
    }
    else if(type == PHYS_REMIND_OVIPOSIT_PERIOD)
    {//������
        if(hours == 9 && minutes == 0)
        {//�����������ڵ�X�죬С��Ŷ
            //�����������ڵ�
            memcpy(phys_remind_info_s.remind_info_buf, menstrual_words_vi_g, strlen(menstrual_words_vi_g));
            phys_remind_info_s.data_size = strlen(menstrual_words_vi_g);
            //days 
            Util_Itoa(days, (char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size]);
            phys_remind_info_s.data_size = strlen((const char *)phys_remind_info_s.remind_info_buf);
            //�죬С��Ŷ
            memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], menstrual_words_vi_h, strlen(menstrual_words_vi_h));            
            phys_remind_info_s.data_size += strlen(menstrual_words_vi_h);
        }
    }
    else if(type == PHYS_REMIND_OVIPOSIT_DAY)
    {//������
        if(hours == 9 && minutes == 0)
        {//�����������գ�Ҫ����С��Ŷ
            memcpy(phys_remind_info_s.remind_info_buf, menstrual_words_vi_i, strlen(menstrual_words_vi_i));
            phys_remind_info_s.data_size = strlen(menstrual_words_vi_i);
        }
    }
    else if(type == PHYS_REMIND_OVIPOSIT_PERIOD1)
    {//������������
        if(hours == 9 && minutes == 0)
        {//���������ڣ����м��ʽϴ�
            //���������ڣ�
            memcpy(phys_remind_info_s.remind_info_buf, pregnant_words_vi_a, strlen(pregnant_words_vi_a));
            phys_remind_info_s.data_size = strlen(pregnant_words_vi_a);
            //���м��ʽϴ�
            memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], pregnant_words_vi_c, strlen(pregnant_words_vi_c));            
            phys_remind_info_s.data_size += strlen(pregnant_words_vi_c);
        }
    }
    else if(type == PHYS_REMIND_OVIPOSIT_DAY1)
    {//������������
        if(hours == 9 && minutes == 0)
        {//���������գ����м��ʽϴ�
            //���������գ�
            memcpy(phys_remind_info_s.remind_info_buf, pregnant_words_vi_b, strlen(pregnant_words_vi_b));
            phys_remind_info_s.data_size = strlen(pregnant_words_vi_b);
            //���м��ʽϴ�
            memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], pregnant_words_vi_c, strlen(pregnant_words_vi_c));            
            phys_remind_info_s.data_size += strlen(pregnant_words_vi_c);
        }
    }
    else if(type == PHYS_REMIND_FORECAST_MENSTRUAL_PERIOD)
    {//Ԥ�⾭��
        if(hours == 7 && minutes == 30 && days == 1)
        {//Ԥ�������뾭�ڣ��������Ϣ
            memcpy(phys_remind_info_s.remind_info_buf, menstrual_words_vi_k, strlen(menstrual_words_vi_k));
            phys_remind_info_s.data_size = strlen(menstrual_words_vi_k);
        } 
        else if(hours == 9 && minutes == 0 && days > 1)
        {//�����¾��ӳ�X�죬�������Ϣ
            //�����¾��ӳ�
            memcpy(phys_remind_info_s.remind_info_buf, menstrual_words_vi_l, strlen(menstrual_words_vi_l));
            phys_remind_info_s.data_size = strlen(menstrual_words_vi_l);
            //days 
            Util_Itoa(days, (char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size]);
            phys_remind_info_s.data_size = strlen((const char *)phys_remind_info_s.remind_info_buf);
            //��
            memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], menstrual_words_vi_b, strlen(menstrual_words_vi_b));            
            phys_remind_info_s.data_size += strlen(menstrual_words_vi_b);
            //���������Ϣ
            memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], menstrual_words_vi_m, strlen(menstrual_words_vi_m));            
            phys_remind_info_s.data_size += strlen(menstrual_words_vi_m);
        } 
    }
    else if(type == PHYS_REMIND_PREGNANCY_ONE)
    {//���е�һ�׶Σ�1~3���£�
        if(hours == 9 && minutes == 0)
        {//���ѻ���X��X�죬�붨�������
            //���ѻ���
            memcpy(phys_remind_info_s.remind_info_buf, gestation_words_vi_a, strlen(gestation_words_vi_a));
            phys_remind_info_s.data_size = strlen(gestation_words_vi_a);
            //n��
            if(days / 7 != 0)
            {
                //days/7 
                Util_Itoa(days / 7, (char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size]);
                phys_remind_info_s.data_size = strlen((const char *)phys_remind_info_s.remind_info_buf);
                memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], gestation_words_vi_b, strlen(gestation_words_vi_b));            
                phys_remind_info_s.data_size += strlen(gestation_words_vi_b);
            }
            //n��
            if(days % 7 != 0)
            {
                //days%7 
                Util_Itoa(days % 7, (char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size]);
                phys_remind_info_s.data_size = strlen((const char *)phys_remind_info_s.remind_info_buf);
                memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], menstrual_words_vi_b, strlen(menstrual_words_vi_b));            
                phys_remind_info_s.data_size += strlen(menstrual_words_vi_b);
            }
            //,�붨�������
            memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], gestation_words_vi_c, strlen(gestation_words_vi_c));            
            phys_remind_info_s.data_size += strlen(gestation_words_vi_c);
        } 
    }
    else if(type == PHYS_REMIND_PREGNANCY_TWO)
    {//���еڶ��׶Σ�3~6���£�
        if(hours == 9 && minutes == 0)
        {//09��00���ѻ���X��X�죬�뱣�ֺ�����
            //���ѻ���
            memcpy(phys_remind_info_s.remind_info_buf, gestation_words_vi_a, strlen(gestation_words_vi_a));
            phys_remind_info_s.data_size = strlen(gestation_words_vi_a);
            //n��
            if(days / 7 != 0)
            {
                //days/7 
                Util_Itoa(days / 7, (char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size]);
                phys_remind_info_s.data_size = strlen((const char *)phys_remind_info_s.remind_info_buf);
                memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], gestation_words_vi_b, strlen(gestation_words_vi_b));            
                phys_remind_info_s.data_size += strlen(gestation_words_vi_b);
            }
            //n��
            if(days % 7 != 0)
            {
                //days%7 
                Util_Itoa(days % 7, (char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size]);
                phys_remind_info_s.data_size = strlen((const char *)phys_remind_info_s.remind_info_buf);
                memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], menstrual_words_vi_b, strlen(menstrual_words_vi_b));            
                phys_remind_info_s.data_size += strlen(menstrual_words_vi_b);
            }
            //,�붨�������
            memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], gestation_words_vi_d, strlen(gestation_words_vi_d));            
            phys_remind_info_s.data_size += strlen(gestation_words_vi_d);
        }
    }
    else if(type == PHYS_REMIND_PREGNANCY_THU)
    {//���е����׶Σ�6����~����ǰ��
        if(hours == 9 && minutes == 0)
        {//09��00�౦������X�죬�뱣�ֺ�����
            //�౦������
            memcpy(phys_remind_info_s.remind_info_buf, gestation_words_vi_e, strlen(gestation_words_vi_e));
            phys_remind_info_s.data_size = strlen(gestation_words_vi_e);
            //n��
            Util_Itoa(days, (char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size]);
            phys_remind_info_s.data_size = strlen((const char *)phys_remind_info_s.remind_info_buf);
            memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], menstrual_words_vi_b, strlen(menstrual_words_vi_b));            
            phys_remind_info_s.data_size += strlen(menstrual_words_vi_b);
            
            //,�붨�������
            memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], gestation_words_vi_d, strlen(gestation_words_vi_d));            
            phys_remind_info_s.data_size += strlen(gestation_words_vi_d);
        }
    }
    else if(type == PHYS_REMIND_PREGNANCY_BIRTH)
    {//���е��Ľ׶Σ�����������
        if(hours == 9 && minutes == 0)
        {//Ԥף�������쵮����׼��������
            memcpy(phys_remind_info_s.remind_info_buf, gestation_words_vi_f, strlen(gestation_words_vi_f));
            phys_remind_info_s.data_size = strlen(gestation_words_vi_f);
        }
    }
    else if(type == PHYS_REMIND_PREGNANCY_ERROR)
    {//������Ϣ���ڣ�������Ϣ
        if(hours == 9 && minutes == 0)
        {//09��00//��������ĸ�����Ϣ(���������Ϣ��Ч,����Ϣ������)
            memcpy(phys_remind_info_s.remind_info_buf, hot_mom_words_vi_g, strlen(hot_mom_words_vi_g));
            phys_remind_info_s.data_size = strlen(hot_mom_words_vi_g); 
        }
    }
    else if(type == PHYS_REMIND_BABY_BORN)
    {//����������
        if(hours == 9 && minutes == 30)
        {
            if(days <= 10)
            {//����������X�죬�ָ���ע������
                //����������
                memcpy(phys_remind_info_s.remind_info_buf, hot_mom_words_vi_a, strlen(hot_mom_words_vi_a));
                phys_remind_info_s.data_size = strlen(hot_mom_words_vi_a);
                //days 
                Util_Itoa(days, (char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size]);
                phys_remind_info_s.data_size = strlen((const char *)phys_remind_info_s.remind_info_buf);
                //��
                memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], menstrual_words_vi_b, strlen(menstrual_words_vi_b));            
                phys_remind_info_s.data_size += strlen(menstrual_words_vi_b);
                //����ָ���ע������
                memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], hot_mom_words_vi_b, strlen(hot_mom_words_vi_b));            
                phys_remind_info_s.data_size += strlen(hot_mom_words_vi_b);
            }
            else if(days == 30)
            {//��������(30��)�ˣ�ֵ�ü����һ��              
                memcpy(phys_remind_info_s.remind_info_buf, hot_mom_words_vi_c, strlen(hot_mom_words_vi_c));
                phys_remind_info_s.data_size = strlen(hot_mom_words_vi_c);  
                memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], hot_mom_words_vi_f, strlen(hot_mom_words_vi_f));            
                phys_remind_info_s.data_size += strlen(hot_mom_words_vi_f);
            }
            else if(days == 100)
            {//����һ���죬ֵ�ü����һ��
                memcpy(phys_remind_info_s.remind_info_buf, hot_mom_words_vi_d, strlen(hot_mom_words_vi_d));
                phys_remind_info_s.data_size = strlen(hot_mom_words_vi_d); 
                memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], hot_mom_words_vi_f, strlen(hot_mom_words_vi_f));            
                phys_remind_info_s.data_size += strlen(hot_mom_words_vi_f);
            }
            else if(days == 365)
            {//����һ���ֵ꣬�ü����һ��
                memcpy(phys_remind_info_s.remind_info_buf, hot_mom_words_vi_e, strlen(hot_mom_words_vi_e));
                phys_remind_info_s.data_size = strlen(hot_mom_words_vi_e); 
                memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], hot_mom_words_vi_f, strlen(hot_mom_words_vi_f));            
                phys_remind_info_s.data_size += strlen(hot_mom_words_vi_f);
            }
            else if(days < 365)
            {//����������x��
                //����������
                memcpy(phys_remind_info_s.remind_info_buf, hot_mom_words_vi_a, strlen(hot_mom_words_vi_a));
                phys_remind_info_s.data_size = strlen(hot_mom_words_vi_a);
                //days 
                Util_Itoa(days, (char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size]);
                phys_remind_info_s.data_size = strlen((const char *)phys_remind_info_s.remind_info_buf);
                //��
                memcpy((char *)&phys_remind_info_s.remind_info_buf[phys_remind_info_s.data_size], menstrual_words_vi_b, strlen(menstrual_words_vi_b));            
                phys_remind_info_s.data_size += strlen(menstrual_words_vi_b);
            }
            else
            {//��������ĸ�����Ϣ(���������Ϣ��Ч,����Ϣ������)
                memcpy(phys_remind_info_s.remind_info_buf, hot_mom_words_vi_g, strlen(hot_mom_words_vi_g));
                phys_remind_info_s.data_size = strlen(hot_mom_words_vi_g);
            }
        }
    }
    else
    {//�Ƿ�
        if(hours == 9 && minutes == 0)
        {//09��00��Ϣ���������
            memcpy(phys_remind_info_s.remind_info_buf, menstrual_words_vi_n, strlen(menstrual_words_vi_n));
            phys_remind_info_s.data_size = strlen(menstrual_words_vi_n);  
        }
    }        
}





