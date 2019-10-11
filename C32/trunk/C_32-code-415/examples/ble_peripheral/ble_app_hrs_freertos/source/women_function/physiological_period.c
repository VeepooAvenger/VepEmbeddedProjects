/***************************Copyright(c)***********************************
**                       Veepoo Tech
**                  COPYRIGHT (C) 2016-2019
**-----------File Info-----------------------------------------------------
** File        name:     physiological_period.c
** Last modified Date:   2017-07-28
** Last SYS_VERSION:     0.0.0
** Descriptions:            
**-------------------------------------------------------------------------
** Created  by:          WJ
** Created  date:        2017-10-18
** SYS_VERSION:          0.0.0
** Descriptions:         
**-------------------------------------------------------------------------
** Modified by:				
** Modified date:  			          
** SYS_VERSION:                 
** Descriptions:                        
***************************************************************************/
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "physiological_period.h"
#include "app_type.h"
#include "app_task.h"
#include "app_flash.h"
#include "system_clock.h"
#include "app_config_data.h"
#include "gui_font_lattice.h"
#include "physiological_remind_words.h"
#include "physiological_remind_words_en.h"
#include "physiological_remind_words_french.h"
#include "physiological_remind_words_german.h"
#include "physiological_remind_words_italiarn.h"
#include "physiological_remind_words_japan.h"
#include "physiological_remind_words_korean.h"
#include "physiological_remind_words_portuguese.h"
#include "physiological_remind_words_russian.h"
#include "physiological_remind_words_spanish.h"
#include "physiological_remind_words_traditional.h"
#include "physiological_remind_words_vietnanese.h"

#include "gui_thread.h"
/*********************************************************************
* Macro Definition
*/
#define APP_PHYS_DEBUG   0

/*********************************************************************
* Global Variables
*/  

__ALIGN(4) static phys_period_info_t phys_period_info_s = {0x00,};
next_menstrual_period_t next_menstrual_period_s = {0x00,};

//static uint8_t phys_from_days = 0;       //距离预期还有多少天,或者超出预期
//static uint8_t phys_from_days_flag = 0;  //0，距离预期；1，超出预期

static uint8_t phys_delay_save_flag = 0;
static uint8_t phys_delay_save_time = 0;

phys_remind_info_t phys_remind_info_s = {0x00,};     //女性功能提醒消息

extern system_info_t system_info_s;
extern system_flag_t system_flag_s;            //系统标志
/*********************************************************************
* local functions
*/
static void Phys_DisplayRemindByLanguage(uint8_t type, uint16_t days, uint8_t hours, uint8_t minutes);
void Phys_CalcNextMenstrualPeriod(void);
void Phys_ClearRemindInfo(void);

/****************************************************************
** Function name:           Phys_InitParam
** Descriptions:            上电初始化
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-10-18
*****************************************************************/
void Phys_InitParam(void)
{
//#if APP_PHYS_DEBUG    
//    Sclock_SetTestTime();    
//#endif   
    
    App_FlashReadPhysPeriodData(0, (void *)&phys_period_info_s, sizeof(phys_period_info_s));
  
    Phys_ClearRemindInfo();
    Phys_CalcNextMenstrualPeriod();   
}

/****************************************************************
** Function name:           Phys_ResetInitParam
** Descriptions:            清除数据初始化
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              JSH              
** Created Date:            2018-7-16
*****************************************************************/
void Phys_ResetInitParam(void)
{    
    memset(&phys_period_info_s, 0,sizeof(phys_period_info_s));
  
    Phys_ClearRemindInfo();
    Phys_CalcNextMenstrualPeriod();   
}

/****************************************************************
** Function name:           Phys_GetSettingFromProtocol
** Descriptions:            解析女性功能协议
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-10-18
*****************************************************************/
void Phys_DelaySave(void)
{
    if(phys_delay_save_flag == 0)
    {
        return;
    }
    phys_delay_save_time++; 
    if(phys_delay_save_flag == 1 && phys_delay_save_time >= 2)
    {
        if(App_FlashErasePhysPeriodData() == APP_FLASH_SUCCESS)
        {
            phys_delay_save_flag = 2;
        }
        phys_delay_save_time = 0;
    }
    else if(phys_delay_save_flag == 2 && phys_delay_save_time >= 3)
    {
        if(App_FlashWritePhysPeriodData(0, (void *)&phys_period_info_s, sizeof(phys_period_info_s)) == APP_FLASH_SUCCESS)
        { 
            phys_delay_save_flag = 0;
        }
        phys_delay_save_time = 0;
    }
}

/****************************************************************
** Function name:           Phys_PowerOffSave
** Descriptions:            女性功能关机保存数据
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              JSH              
** Created Date:            2018-7-16
*****************************************************************/
void Phys_PowerOffSave(void)
{
    if(phys_delay_save_flag == 0)
    {
        return;
    }

    if(phys_delay_save_flag == 1 )
    {
        if(App_FlashErasePhysPeriodData() == APP_FLASH_SUCCESS)
        {
//            App_FlashEraseCheck();
            phys_delay_save_flag = 2;
        }
    }

    if(phys_delay_save_flag == 2)
    {
        if(App_FlashWritePhysPeriodData(0, (void *)&phys_period_info_s, sizeof(phys_period_info_s)) == APP_FLASH_SUCCESS)
        { 
            phys_delay_save_flag = 0;
        }
    }
}

/****************************************************************
** Function name:           Phys_GetSettingFromProtocol
** Descriptions:            解析女性功能协议
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-10-18
*****************************************************************/
void Phys_ParsingProtocol(uint8_t *in_data, uint8_t *out_data)
{
    uint8_t change_data_temp = 0;
    phys_period_info_t phys_period_info_temp_s = {0x00,};
    
    out_data[0] = 0x85; 
    //WJ.20171218.解决协议回复错误问题
    out_data[1] = 0x01; 
    out_data[2] = in_data[1];  
    if(in_data[1] <=4)
    {
        phys_period_info_temp_s.phys_period_time = in_data[1];
        phys_period_info_temp_s.phys_period_year = in_data[3] + (in_data[2] << 8);
        phys_period_info_temp_s.phys_period_month = in_data[4];
        phys_period_info_temp_s.phys_period_day = in_data[5];
        phys_period_info_temp_s.phys_period_cycle = in_data[6];
        phys_period_info_temp_s.phys_mens_days = in_data[7];
        phys_period_info_temp_s.baby_birthday_year = in_data[9] + (in_data[8] << 8);
        phys_period_info_temp_s.baby_birthday_month = in_data[10];
        phys_period_info_temp_s.baby_birthday_day = in_data[11];
        phys_period_info_temp_s.baby_birthday_sex = in_data[12];
        phys_period_info_temp_s.phys_mens_actual_days = in_data[13];
        
        if(memcmp(&phys_period_info_temp_s, &phys_period_info_s, sizeof(phys_period_info_s)) != 0)
        {
            memcpy(&phys_period_info_s, &phys_period_info_temp_s, sizeof(phys_period_info_s));
            //保存数据
            if(App_FlashErasePhysPeriodData() == APP_FLASH_SUCCESS)
            {
                App_FlashWritePhysPeriodData(0, (void *)&phys_period_info_s, sizeof(phys_period_info_s));
            }
            if(phys_delay_save_flag != 2)
            {
                phys_delay_save_flag = 1;
            }
            phys_delay_save_time = 0;
            //WJ.20180119.更换位置，防止信息一致的情况下清除未读信息
            Phys_ClearRemindInfo();
            Phys_CalcNextMenstrualPeriod(); 
        }    
              
    }
    else if(in_data[1] == 5)
    {
        memcpy(&out_data[3], &phys_period_info_s, sizeof(phys_period_info_s) - 3);       
        //年份高低位交换
        change_data_temp = out_data[3];
        out_data[3] = out_data[4];
        out_data[4] = change_data_temp;
        
        //年份高低位交换
        change_data_temp = out_data[9];
        out_data[9] = out_data[10];
        out_data[10] = change_data_temp;
    }    
    else 
    {
        out_data[1] = 0x00;    
    }
}

/****************************************************************
** Function name:           Phys_CalcNextMenstrualPeriod
** Descriptions:            根据设置计算生理期
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-10-19
*****************************************************************/
void Phys_CalcNextMenstrualPeriod(void)
{
    uint8_t  ret = 0;
    uint8_t  c_day = 0;
    uint8_t  c_month = 0;
    uint16_t c_year = 0;
    
    uint16_t days = 0; 
    uint16_t year_days = 0;
    uint16_t year_days1 = 0;
    uint32_t total_days = 0;
    
    //情况计算结果
    memset(&next_menstrual_period_s, 0, sizeof(next_menstrual_period_s));
    if(phys_period_info_s.phys_period_time < PHYS_PERIOD_MESTRUAL 
       || phys_period_info_s.phys_period_time > PHYS_PERIOD_MAX)
    {
        return;
    }
    
    Sclock_GetYearMonthDay(&c_year, &c_month, &c_day);
    if(phys_period_info_s.phys_period_time != PHYS_PERIOD_PREGNANCY)
    {
        //计算下一阶段经期
        //判断设置是否合理
        year_days =  Sclock_SeekYearDays(c_year, c_month, c_day);
        year_days1 = Sclock_SeekYearDays(phys_period_info_s.phys_period_year, 
                                         phys_period_info_s.phys_period_month, 
                                         phys_period_info_s.phys_period_day);
        
        if(c_year < phys_period_info_s.phys_period_year
           //WJ.20180105.没有考虑到跨年前后，跨年时 year_days < year_days1
          || (c_year == phys_period_info_s.phys_period_year && year_days < year_days1))
        {
            //设置日期无效
            next_menstrual_period_s.remind_type = PHYS_REMIND_NULL;
            next_menstrual_period_s.menstrual_period_type = PHYS_MENSTRUAL_NULL;
        }
        else
        {
            //求某年到某月某日有多少天
            days = Sclock_SeekYearDays(phys_period_info_s.phys_period_year, 
                                       phys_period_info_s.phys_period_month, 
                                       phys_period_info_s.phys_period_day);
            days += phys_period_info_s.phys_period_cycle;
            
            year_days = Sclock_SeekYearDays(phys_period_info_s.phys_period_year, 12, 31);
            if(days > year_days)
            {
                // 跨年
                next_menstrual_period_s.menstrual_period_year = phys_period_info_s.phys_period_year + 1;
                days = days - year_days;
            }
            else
            {
                // 当年
                next_menstrual_period_s.menstrual_period_year = phys_period_info_s.phys_period_year; 
            }
            //根据年份和天数求具体日期
            Sclock_SeekYearMonthAndDayByDays(next_menstrual_period_s.menstrual_period_year,
                                             days,
                                             &next_menstrual_period_s.menstrual_period_month,
                                             &next_menstrual_period_s.menstrual_period_day);
            //用户是否设置了实际经期（提前结束）
            if(phys_period_info_s.phys_mens_actual_days != 0)
            {
                next_menstrual_period_s.menstrual_period_actual_days = phys_period_info_s.phys_mens_actual_days;
            }                
            else
            {
                next_menstrual_period_s.menstrual_period_actual_days = phys_period_info_s.phys_mens_days;
            }
            //第几天进入排卵日
            next_menstrual_period_s.menstrual_period_oviposit_days = phys_period_info_s.phys_period_cycle - 14;
            //月经              
            ret = Sclock_GetsDaysByTwoDate(phys_period_info_s.phys_period_year,  
                                          phys_period_info_s.phys_period_month, 
                                          phys_period_info_s.phys_period_day,
                                          c_year, c_month, c_day, 
                                          &total_days);
            
            if(ret == 1)
            {
                if(total_days > phys_period_info_s.phys_period_cycle + 3)
                {
                    //信息过期
                    next_menstrual_period_s.remind_type = PHYS_REMIND_NULL;   
                    next_menstrual_period_s.remind_days = total_days + 1;
                    next_menstrual_period_s.menstrual_period_type = PHYS_MENSTRUAL_NULL; 
                }
                else if(total_days >= phys_period_info_s.phys_period_cycle)
                {
                    //预测经期
                    next_menstrual_period_s.remind_type = PHYS_REMIND_FORECAST_MENSTRUAL_PERIOD;   
                    next_menstrual_period_s.remind_days = total_days - phys_period_info_s.phys_period_cycle + 1;
                    
                    next_menstrual_period_s.menstrual_period_type = PHYS_MENSTRUAL_NULL; 
                }
                //WJ.20171107.经期多算了一天，total_days从0开始
                else if(total_days < next_menstrual_period_s.menstrual_period_actual_days)
                {
                    // 经期
                    next_menstrual_period_s.remind_type = PHYS_REMIND_MENSTRUAL_PERIOD; 
                    next_menstrual_period_s.remind_days = total_days + 1; 
                    next_menstrual_period_s.menstrual_period_type = PHYS_MENSTRUAL_MENSTRUAL_PERIOD; 
                }
                else if(total_days == next_menstrual_period_s.menstrual_period_oviposit_days)
                {
                    //排卵日
                    if(phys_period_info_s.phys_period_time == PHYS_PERIOD_FOR_PREGNANCY)
                    {
                        next_menstrual_period_s.remind_type = PHYS_REMIND_OVIPOSIT_DAY1;       
                    }
                    else
                    {
                        next_menstrual_period_s.remind_type = PHYS_REMIND_OVIPOSIT_DAY;   
                    }
                    
                    next_menstrual_period_s.menstrual_period_type = PHYS_MENSTRUAL_OVIPOSIT_DAY; 
                    next_menstrual_period_s.remind_days = total_days + 1;
                }
                else if(total_days >= next_menstrual_period_s.menstrual_period_oviposit_days - 5 
                       && total_days <= next_menstrual_period_s.menstrual_period_oviposit_days + 4 )
                {
                    //排卵期
                    if(phys_period_info_s.phys_period_time == PHYS_PERIOD_FOR_PREGNANCY)
                    {
                        next_menstrual_period_s.remind_type = PHYS_REMIND_OVIPOSIT_PERIOD1;       
                    }
                    else
                    {
                        next_menstrual_period_s.remind_type = PHYS_REMIND_OVIPOSIT_PERIOD;   
                        
                    }                    
                    next_menstrual_period_s.remind_days = total_days - (next_menstrual_period_s.menstrual_period_oviposit_days  - 5) + 1;                    
                    next_menstrual_period_s.menstrual_period_type = PHYS_MENSTRUAL_OVIPOSIT_PERIOD; 
                }
                else 
                {
                    //安全期 PHYS_REMIND_SAFETY_PERIOD1
                    if(phys_period_info_s.phys_period_cycle - total_days <= 3)
                    {
                        next_menstrual_period_s.remind_type = PHYS_REMIND_SAFETY_PERIOD1;   
                    }
                    else
                    {
                        next_menstrual_period_s.remind_type = PHYS_REMIND_SAFETY_PERIOD;     
                    }
                    //WJ.20171107.天数多了一天
                    next_menstrual_period_s.remind_days = phys_period_info_s.phys_period_cycle - total_days;
                    
                    next_menstrual_period_s.menstrual_period_type = PHYS_MENSTRUAL_SAFETY_PERIOD; 
                }
            }
            else
            {
                //信息过期
                next_menstrual_period_s.remind_type = PHYS_REMIND_NULL;   
                next_menstrual_period_s.remind_days = 0;   
                next_menstrual_period_s.menstrual_period_type = PHYS_MENSTRUAL_NULL;                
            }
        }
    }
    else if(phys_period_info_s.phys_period_time == PHYS_PERIOD_PREGNANCY)
    {
        next_menstrual_period_s.menstrual_period_type = PHYS_MENSTRUAL_NULL; 
        //根据预产期计算怀孕日期
        days = Sclock_SeekYearDays(phys_period_info_s.phys_period_year, 
                                   phys_period_info_s.phys_period_month, 
                                   phys_period_info_s.phys_period_day);
        if(days > 280)
        {
            //怀孕日期在当年
            next_menstrual_period_s.menstrual_period_year = phys_period_info_s.phys_period_year;
            days = days - 280;
        }
        else
        {
            //怀孕日期在去年
            next_menstrual_period_s.menstrual_period_year = phys_period_info_s.phys_period_year - 1;
            year_days = Sclock_SeekYearDays(next_menstrual_period_s.menstrual_period_year, 12, 31);
            days = year_days - (280 - days);
        }
        Sclock_SeekYearMonthAndDayByDays(next_menstrual_period_s.menstrual_period_year,
                                         days,
                                         &next_menstrual_period_s.menstrual_period_month,
                                         &next_menstrual_period_s.menstrual_period_day); 
       
        ret = Sclock_GetsDaysByTwoDate(next_menstrual_period_s.menstrual_period_year,  
                                       next_menstrual_period_s.menstrual_period_month, 
                                       next_menstrual_period_s.menstrual_period_day,
                                       c_year, c_month, c_day, 
                                       &total_days);
        if(ret == 1)
        {
            if(total_days <= 84)
            {
                //WJ.20171107.提示错误
                next_menstrual_period_s.remind_type = PHYS_REMIND_PREGNANCY_ONE;                     
            }
            else if(total_days <= 252)
            {
                next_menstrual_period_s.remind_type = PHYS_REMIND_PREGNANCY_TWO;                     
            }
            else if(total_days < 280)
            {
                next_menstrual_period_s.remind_type = PHYS_REMIND_PREGNANCY_THU;                     
            }
            else if(total_days == 280)
            {
                next_menstrual_period_s.remind_type = PHYS_REMIND_PREGNANCY_BIRTH; 
            }
            else 
            {
                next_menstrual_period_s.remind_type = PHYS_REMIND_NULL;
            }
            next_menstrual_period_s.remind_days = total_days;  
        }
    }
    if(phys_period_info_s.phys_period_time == PHYS_PERIOD_HOT_MAMA)
    {
        //WJ.20180123.跨两个年度提醒错误问题
        if(c_year < phys_period_info_s.baby_birthday_year || c_year >= phys_period_info_s.baby_birthday_year + 2)
        {
            // 年份超期或者无效
            next_menstrual_period_s.baby_birthday_days = 0xFFFF;
            //WJ.20180123.增加提醒
            if(next_menstrual_period_s.remind_type == PHYS_REMIND_NULL)
            {
                next_menstrual_period_s.remind_type = PHYS_REMIND_BABY_BORN;
            }
        }
        else 
        {
            year_days =  Sclock_SeekYearDays(c_year, c_month, c_day);
            year_days1 = Sclock_SeekYearDays(phys_period_info_s.baby_birthday_year, 
                                             phys_period_info_s.baby_birthday_month, 
                                             phys_period_info_s.baby_birthday_day);
            
            if(c_year == phys_period_info_s.baby_birthday_year)
            {
                if(year_days < year_days1)
                {
                    //时间无效
                    next_menstrual_period_s.baby_birthday_days = 0xFFFF;
                    if(next_menstrual_period_s.remind_type == PHYS_REMIND_NULL)
                    {
                        next_menstrual_period_s.remind_type = PHYS_REMIND_BABY_BORN;
                    }
                }
                else
                {
                    //宝宝出生日期
                    next_menstrual_period_s.baby_birthday_days = year_days - year_days1 + 1;
                }
            }
            else
            {
                year_days1 = Sclock_SeekYearDays(phys_period_info_s.baby_birthday_year, 12, 31) - year_days1;                
                year_days = year_days + year_days1 + 1;               
                
                if(year_days > 365)
                {
                    next_menstrual_period_s.baby_birthday_days = 0xFFFF;
                    if(next_menstrual_period_s.remind_type == PHYS_REMIND_NULL)
                    {
                        next_menstrual_period_s.remind_type = PHYS_REMIND_BABY_BORN;
                    }
                }
                else
                {
                    next_menstrual_period_s.baby_birthday_days = year_days;
                }
            }
        }
    } 
}

/****************************************************************
** Function name:           Phys_IsDisplayTime
** Descriptions:            是否是显示时间
** input parameters:        无
** output parameters:       无
** Returned value:          0：不是；1：是
** Created by:              WJ              
** Created Date:            2017-10-25
*****************************************************************/
uint8_t Phys_IsDisplayTime(void)
{   
    uint8_t  c_hours = 0;
    uint8_t  c_minutes = 0;
    uint8_t  c_second = 0;   
    
    Sclock_GetTime(&c_hours, &c_minutes, &c_second);
    if((c_hours == 7 && c_minutes == 30)
      || (c_hours == 9 && c_minutes == 0)
      || (c_hours == 9 && c_minutes == 30)
      || (c_hours == 11 && c_minutes == 0)
      || (c_hours == 14 && c_minutes == 0)
      || (c_hours == 17 && c_minutes == 30)
      || (c_hours == 21 && c_minutes == 30))
    {
        return 1;
    }
    return 0;
}

/****************************************************************
** Function name:           Phys_DisplayRemind
** Descriptions:            根据生理周期和时间提示不同的内容
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-10-19
*****************************************************************/
void Phys_DisplayRemind(void)
{  
//    uint8_t  ret = 0;
    uint8_t  c_hours = 0;
    uint8_t  c_minutes = 0;
    uint8_t  c_second = 0;   
    uint8_t  type = 0;
    uint16_t days = 0;
    
    if(phys_period_info_s.phys_period_time < PHYS_PERIOD_MESTRUAL 
       || phys_period_info_s.phys_period_time > PHYS_PERIOD_MAX)
    {
        return;
    }
    
    if(Phys_IsDisplayTime() != 1)
    {
        return;
    }
    
    Sclock_GetTime(&c_hours, &c_minutes, &c_second);
     
    type = next_menstrual_period_s.remind_type;
    days = next_menstrual_period_s.remind_days;
    if(phys_period_info_s.phys_period_time == PHYS_PERIOD_HOT_MAMA)
    {
        if(c_hours == 9 && c_minutes == 30)
        {
            if(type == PHYS_REMIND_BABY_BORN)
            {
                days = next_menstrual_period_s.baby_birthday_days;
            }
            if(next_menstrual_period_s.baby_birthday_days <= 365 
                && next_menstrual_period_s.baby_birthday_days > 1)
            {
                type = PHYS_REMIND_BABY_BORN;
                days = next_menstrual_period_s.baby_birthday_days;
            }
        }
    }
    if(phys_period_info_s.phys_period_time == PHYS_PERIOD_PREGNANCY)
    {
        if(days < 1)
        {
            return;
        }
        if(next_menstrual_period_s.remind_type == PHYS_REMIND_PREGNANCY_THU)
        {
            days = 280 - days;
        }
        
    }    
    if(type == PHYS_REMIND_MENSTRUAL_PERIOD)
    {
        if((c_hours == 11 || c_hours == 14 || c_hours == 17) 
           && days > next_menstrual_period_s.menstrual_period_actual_days/2 )
        {
            //经期长度除以2天数内全部提醒，后边几天2 3 4不用提醒
            return;
        }
    }
    
    memset(&phys_remind_info_s, 0x00, sizeof(phys_remind_info_s));  
    Phys_DisplayRemindByLanguage(type, days, c_hours, c_minutes);
    phys_remind_info_s.hours = c_hours;
    phys_remind_info_s.minutes = c_minutes;
    phys_remind_info_s.type = type;
    phys_remind_info_s.days = days;
    phys_remind_info_s.language = system_info_s.language;
    phys_remind_info_s.update = 1;
    
    if(phys_remind_info_s.data_size != 0 && system_flag_s.sleep_enter == 0)
    {
        Gui_ThreadEventTaskWakeUp(CPU_EVENT_TASK_PHYS_REMIND); 
    }
}

/****************************************************************
** Function name:           Phys_ClearRemindInfo
** Descriptions:            清除提示语
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-10-24
*****************************************************************/
void Phys_ClearRemindInfo(void)
{  
    memset(&phys_remind_info_s, 0x00, sizeof(phys_remind_info_s)); 
}

/****************************************************************
** Function name:           Phys_ReDisplayRemindByLanguage
** Descriptions:            根据系统语言更新提示内容
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-10-25
*****************************************************************/
static void Phys_ReDisplayRemindByLanguage(void)
{  
    if(phys_remind_info_s.is_reading == 1)
    {
        //正在读取中不再更新
        return;
    }
    if(phys_remind_info_s.update == 1 && phys_remind_info_s.language != system_info_s.language)
    {
        phys_remind_info_s.data_size = 0;
        memset(phys_remind_info_s.remind_info_buf, 0, sizeof(phys_remind_info_s.remind_info_buf));
        Phys_DisplayRemindByLanguage(phys_remind_info_s.type, phys_remind_info_s.days, 
                                     phys_remind_info_s.hours, phys_remind_info_s.minutes);
        phys_remind_info_s.language = system_info_s.language;
    }
}
 
/****************************************************************
** Function name:           Phys_DisplayRemindByLanguage
** Descriptions:            根据系统语言提示内容
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-10-24
*****************************************************************/
static void Phys_DisplayRemindByLanguage(uint8_t type, uint16_t days, uint8_t hours, uint8_t minutes )
{  
    uint8_t language = 1;
    
    language = system_info_s.language;
    switch(language)
    {
        case CONFIG_LANGUAGE_CHINESE:            //中文
            Phys_ChinesRemind(type, days, hours, minutes);
            break;
        case CONFIG_LANGUAGE_JAPANESE:           //日语
            Phys_JapaneseRemind(type, days, hours, minutes);
            break;
        case CONFIG_LANGUAGE_KOREAN:             //韩语
            Phys_KoreanRemind(type, days, hours, minutes);
            break;
        case CONFIG_LANGUAGE_GERMAN:             //德语
            Phys_GermanRemind(type, days, hours, minutes);
            break;
        case CONFIG_LANGUAGE_RUSSIAN:            //俄语
            Phys_RussianRemind(type, days, hours, minutes);
            break;
        case CONFIG_LANGUAGE_SPANISH:            //西班牙语
            Phys_SpanishRemind(type, days, hours, minutes);
            break;
        case CONFIG_LANGUAGE_ITALIARN:           //意大利语
            Phys_ItaliarnRemind(type, days, hours, minutes);
            break;
        case CONFIG_LANGUAGE_FRENCH:             //法语
            Phys_FrenchRemind(type, days, hours, minutes);
            break;
        case CONFIG_LANGUAGE_VIETNAMESE:         //越南语
            Phys_VietnaneseRemind(type, days, hours, minutes);
            break;
        case CONFIG_LANGUAGE_PORTUGUESE:         //葡萄牙语
            Phys_PortugueseRemind(type, days, hours, minutes);
            break;
        case CONFIG_LANGUAGE_TRADITIONAL:        //繁体 
            Phys_TraditionalRemind(type, days, hours, minutes);           
            break;
        default:                                //英文
            Phys_EnglishRemind(type, days, hours, minutes);
            break;
    }
    
    phys_remind_info_s.total_page = Font_FullScreenTotalCountByStr(
                                                phys_remind_info_s.remind_info_buf,
                                                phys_remind_info_s.data_size,        
                                                phys_remind_info_s.read_index_buf);
    phys_remind_info_s.read_len = phys_remind_info_s.read_index_buf[1];
    
}

/****************************************************************
** Function name:           Phys_DisplayRemindByNextPage
** Descriptions:            消息翻页
** input parameters:        reading_flag：是否正在读取
** output parameters:       无
** Returned value:          0：没有消息了；1：还有消息
** Created by:              WJ              
** Created Date:            2017-08-24
*****************************************************************/ 
uint8_t Phys_DisplayRemindByNextPage(uint8_t reading_flag)
{   
    uint8_t next_read_page = 0;
    
    if(!reading_flag && phys_remind_info_s.is_reading == 0 
       && phys_remind_info_s.total_page >= 1)
    {
        //读取时，防止语言对不上
        Phys_ReDisplayRemindByLanguage();
        phys_remind_info_s.read_page = 1;       
    }
    else
    {
        if(phys_remind_info_s.total_page <= 1)
        {
            Phys_ClearRemindInfo();
            return 0;
        }
        if(phys_remind_info_s.is_reading == 0)
        {
            phys_remind_info_s.read_page = 1;
        }
        phys_remind_info_s.read_page++;
    }
    next_read_page = phys_remind_info_s.read_page;
    phys_remind_info_s.read_len = phys_remind_info_s.read_index_buf[next_read_page] - phys_remind_info_s.read_index_buf[next_read_page - 1];
    phys_remind_info_s.read_index = phys_remind_info_s.read_index_buf[next_read_page - 1];
    phys_remind_info_s.is_reading = 1;
    return 1;
}

/****************************************************************
** Function name:           Phys_ExistUnreadMessage
** Descriptions:            是否存在未读信息
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-10-26
*****************************************************************/ 
uint8_t Phys_ExistUnreadMessage(void)
{
    if(phys_remind_info_s.is_reading == 1 &&  phys_remind_info_s.read_page >= phys_remind_info_s.total_page )
    {
        Phys_ClearRemindInfo();
    }
    return (phys_remind_info_s.read_page < phys_remind_info_s.total_page);
}

/****************************************************************
** Function name:           Phys_Monitor
** Descriptions:            女性功能监听
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-10-26
*****************************************************************/ 
void Phys_Monitor(void)
{
#if APP_PHYS_DEBUG    
    Phys_Test();
#else    
    if(Phys_IsDisplayTime() != 1)
    {
        return;
    }
    //判断性别
    if(system_info_s.user_info.user_gender == 1)
    {
        Phys_ClearRemindInfo();
        return;
    }
    Phys_CalcNextMenstrualPeriod();
    Phys_DisplayRemind();
#endif
}

/****************************************************************
** Function name:           Phys_GetMenstrualPeriodType
** Descriptions:            获取经期处于什么阶段
** input parameters:        无
** output parameters:       无
** Returned value:          返回阶段
** Created by:              WJ              
** Created Date:            2017-10-31
*****************************************************************/ 
uint8_t Phys_GetMenstrualPeriodType(void)
{
    if(system_info_s.user_info.user_gender == 1)
    {
        return PHYS_MENSTRUAL_NULL;
    }
    return next_menstrual_period_s.menstrual_period_type;
}

/****************************************************************
** Function name:           Phys_UpdataInfo
** Descriptions:            更新信息
** input parameters:        无
** output parameters:       无
** Returned value:          返回阶段
** Created by:              WJ              
** Created Date:            2017-10-31
*****************************************************************/ 
void Phys_UpdataInfo(void)
{ 
    if(system_info_s.user_info.user_gender == 1)
    {
        Phys_ClearRemindInfo();
        return;
    }
    
    Phys_CalcNextMenstrualPeriod(); 
}

/****************************************************************
** Function name:           Phys_UpdataParam
** Descriptions:            上电初始化
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-10-18
*****************************************************************/
void Phys_UpdataParam(void)
{
    Phys_ClearRemindInfo();
    Phys_CalcNextMenstrualPeriod(); 
}

/****************************************************************
** Function name:           Phys_Test
** Descriptions:            女性功能测试
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-10-26
*****************************************************************/ 
#if APP_PHYS_DEBUG
void Phys_Test(void)
{
    static uint8_t  c_hours = 7;
    static uint8_t  c_minutes = 30; 
    static uint8_t  type = PHYS_REMIND_MENSTRUAL_PERIOD;
    static uint16_t days = 0;
    static uint8_t language = CONFIG_LANGUAGE_CHINESE;
    static uint8_t type_cnt = 0;
    
    static uint8_t  time_buf[5][2] = {{7,30},{11,0},{14,00},{17,30},{21,30}};
    static uint16_t born_days[] = {5, 12, 30, 100, 365, 400};
    
//    if(phys_period_info_s.phys_period_time < PHYS_PERIOD_MESTRUAL 
//       || phys_period_info_s.phys_period_time > PHYS_PERIOD_MAX)
//    {
//        return;
//    }
   
    days = 3;
    
    if(type > PHYS_REMIND_BABY_BORN)
    {
        type = 0;
        type_cnt = 0;
        language++;
        if(language > CONFIG_LANGUAGE_TRADITIONAL+1)
        {
            language = 1;
        }
    }
    
    if(type == PHYS_REMIND_MENSTRUAL_PERIOD)
    {
        c_hours = time_buf[type_cnt][0];
        c_minutes = time_buf[type_cnt][1];
    }
    else if(type >= PHYS_REMIND_PREGNANCY_ONE
           && type <= PHYS_REMIND_PREGNANCY_ERROR)
    {
        if(type != PHYS_REMIND_PREGNANCY_ONE)
        {
            days = 123;
        }
        c_hours = 9;
        c_minutes = 0;
    }
    else if(type == PHYS_REMIND_FORECAST_MENSTRUAL_PERIOD && type_cnt == 0)
    {
        c_hours = 7;
        c_minutes = 30;
        days = 1;
    }
    else if(type == PHYS_REMIND_FORECAST_MENSTRUAL_PERIOD )
    {
        c_hours = 9;
        c_minutes = 0;
    }
    else if(type == PHYS_REMIND_BABY_BORN)
    {
        c_hours = 9;
        c_minutes = 30;
        days = born_days[type_cnt];
    }
    else
    {
        c_hours = 9;
        c_minutes = 0;
    }        
    system_info_s.language = language;
    memset(&phys_remind_info_s, 0x00, sizeof(phys_remind_info_s));  
    Phys_DisplayRemindByLanguage(type, days, c_hours, c_minutes);
    phys_remind_info_s.hours = c_hours;
    phys_remind_info_s.minutes = c_minutes;
    phys_remind_info_s.type = type;
    phys_remind_info_s.days = days;
    phys_remind_info_s.language = system_info_s.language;
    phys_remind_info_s.update = 1;
    
    if(type == PHYS_REMIND_MENSTRUAL_PERIOD)
    {
        type_cnt++;
        if(type_cnt >= 5)
        {
            type++;   
            type_cnt = 0;
        }
    }
    else if(type == PHYS_REMIND_FORECAST_MENSTRUAL_PERIOD)
    {
        type_cnt++;
        days = 3;
        if(type_cnt >= 2)
        {
            type++;   
            type_cnt = 0;
        }
    }
    else if(type == PHYS_REMIND_BABY_BORN)
    {
        type_cnt++;
        if(type_cnt >= 6)
        {
            type++;
            type_cnt = 0;
        }
    }
    else
    {
        type++;   
        type_cnt = 0;
    }
    
    Gui_ThreadEventTaskWakeUp(CPU_EVENT_TASK_PHYS_REMIND); 
}
#endif

