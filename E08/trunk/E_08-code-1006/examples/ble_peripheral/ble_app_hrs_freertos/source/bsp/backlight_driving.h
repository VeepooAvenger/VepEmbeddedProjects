/***************************Copyright(c)***********************************
**                       Veepoo Tech
**                  COPYRIGHT (C) 2016-2019
**-----------File Info-----------------------------------------------------
** File        name:     backlight_driving.h
** Last modified Date:   2018-08-07
** Last SYS_VERSION:     0.0.0
** Descriptions:            
**-------------------------------------------------------------------------
** Created  by:          WJ
** Created  date:        2018-08-07
** SYS_VERSION:          0.0.0
** Descriptions:         
**-------------------------------------------------------------------------
** Modified by:				
** Modified date:  			          
** SYS_VERSION:                 
** Descriptions:                        
***************************************************************************/

#ifndef APP_BACKLIGHT_DRIVING_H_
#define APP_BACKLIGHT_DRIVING_H_

#include "stdint.h"

#define CONFIG_BACKLIGHT_MAX_LEVEL               BACKLIGHT_MAX_CHANAGE_LEVEL           //�����豸��֧��������ȼ�
#define CONFIG_BACKLIGHT_MIN_LEVEL_RECOMMEND     4           //ҹ�����ȼ�
#define CONFIG_BACKLIGHT_MAX_LEVEL_RECOMMEND     4           //�������ȼ�

#define BACKLIGHT_MAX_CHANAGE_LEVEL     12//16           //�����豸��֧��������ȼ�


//����ʱ��������ֵ
#define UI_DISPLAY_TIME_MAX             30
#define UI_DISPLAY_TIME_MIN             3
#define UI_DISPLAY_TIME_RECOMMEND       5
//��������


typedef struct
{
    uint8_t     state;      //�Ƿ����˱���
    uint8_t     level;      //��ǰ�ȼ�
}backlight_driving_info_t;

//�������
void Backlight_SetLeve(uint8_t set_level);

void Backlight_TestSetLeve(uint8_t set_level);

#endif // APP_BACKLIGHT_DRIVING_H_

