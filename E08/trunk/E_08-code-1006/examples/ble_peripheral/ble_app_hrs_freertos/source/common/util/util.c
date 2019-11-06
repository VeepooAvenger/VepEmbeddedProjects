/*******************Copyright(c)*********************************
**                  Veepoo Tech
**            COPYRIGHT (C) 2016-2019
**----------------------FileInfo---------------------------------
** File name:                  util.c
** Last modified Date:         2016-09-12
** Last SYS_VERSION:           0.0.1
** Descriptions:
**---------------------------------------------------------------
** Createdby:                  GT
** Createddate:                2016-11-29
** SYS_VERSION:                0.0.1
** Descriptions:               主要提供功能性的函数接口
**---------------------------------------------------------------
** Modified by:				
** Modified date:			
** SYS_VERSION:				
** Modified Descriptions:	
****************************************************************/
#include "util.h"
#include "api_flash.h"
#include "pedometer.h"

// 判断是不是闰年
#define	IS_LEAP_YEAR(year)	(!((year) % 400) || (((year) % 100) && !((year) % 4)))


// 闰年月份天数
const uint8_t leap_year_days[12] = {31,29,31,30,31,30,31,31,30,31,30,31};
// 平年月份天数
const uint8_t common_year_days[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
	
/****************************************************************
** Function name:             Image_RightRotate
** Descriptions:              图像右旋
** input parameters:          无
** output parameters:         无
** Returned value:            无
** Created by:                GT
** Created Date:              2016-11-29
****************************************************************/
void Image_RightRotate(uint8_t* pInput,uint8_t* pOutput,uint8_t width,uint8_t high) 
{
    for(int i = 0; i < width; i++)
    {
        for(int j = 0; j < high; j++)
        {
            if(pInput[i+width*(int)(j/8)]&(0x01<<(j%8)))
            {
                pOutput[high-1-j+high*(int)(i/8)] |= (0x01<<(i%8));
            }
            else
            {
                pOutput[high-1-j+high*(int)(i/8)] &= ~(0x01<<(i%8));
            }
        }
    }
}

/****************************************************************
** Function name:             Image_LeftRotate
** Descriptions:              图像左旋
** input parameters:          无
** output parameters:         无
** Returned value:            无
** Created by:                GT
** Created Date:              2016-11-29
****************************************************************/
void Image_LeftRotate(uint8_t* pInput,uint8_t* pOutput,uint8_t width,uint8_t high) 
{
    //这里需要数学计算得出相应的参数 Add by GT 2017-04-05
    for(int i = 0; i < width; i++)
    {
        for(int j = 0; j < high; j++)
        {
            if(pInput[i+width*(int)(j/8)]&(0x01<<(j%8)))
            {
                pOutput[high*((int)(width-1-i)/8)+j] |= (0x01<<((width-1-i)%8));
            }
            else
            {
                pOutput[high*((int)(width-1-i)/8)+j] &= ~(0x01<<((width-1-i)%8));
            }
        }
    }
}

void distance_value(uint32_t *step_input,uint32_t *meter,float *kilometer,float *mile, uint32_t *calories)
{
    float temp = 0; 
    sport_information_info_t gsii_sport_info_s = {0x00, };
    
    Get_SportInformationInfo(&gsii_sport_info_s); 

    if(meter != NULL)
    {
        *meter  = gsii_sport_info_s.posi_distance;//取整
    }
    temp = ((uint32_t)(((gsii_sport_info_s.posi_distance)/1000.0f)*1000))/1000.0f;//保留三位小数点
    if(kilometer != NULL)
    {
        *kilometer = temp;
    }
    if(mile != NULL)
    {
        *mile = temp * (0.62138f);
    }
    *step_input = gsii_sport_info_s.posi_steps;
    *calories = gsii_sport_info_s.posi_cal;
}
//void distance_value(uint32_t step_input,uint32_t *meter,float *kilometer,float *mile)
//{
//    float step_length = 0.0;
//    
//    //步长保留三位小数  乘以1000，取整，再除以1000，单位米
//    if(p_flash_param->person_s.height < 155)
//    {
//        step_length = ((uint32_t)((p_flash_param->person_s.height * 20.0f /(42.0f * 100.0f))*1000))/1000.0f;
//    }
//    else if(p_flash_param->person_s.height >= 155 && p_flash_param->person_s.height < 174)
//    {
//        step_length = ((uint32_t)((p_flash_param->person_s.height * 13.0f /(28.0f * 100.0f))*1000))/1000.0f;
//    }
//    else if(p_flash_param->person_s.height >= 174 )
//    {
//        step_length = ((uint32_t)((p_flash_param->person_s.height * 19.0f /(42.0f * 100.0f))*1000))/1000.0f;
//    }
//  
//    //距离保留一位小数 乘以10，取整，再除以10，单位公里
//    *meter     = step_length  * step_input;
//    *kilometer = ((uint32_t)(((step_length  * step_input)/1000.0f)*1000))/1000.0f;
//    *mile      = *kilometer * (0.62138f);
//}

uint8_t Average_CharArray(uint8_t* p_array, uint8_t len)
{
    uint32_t sum = 0;
    
    if(0 == len)  
    {
        return 0;
    }
    
    for(uint8_t index = 0; index < len; index++)
    {
        sum += p_array[index];
    }
	
    return (uint8_t)(sum/len);
}

uint16_t Average_ShortArray(uint16_t* p_array, uint16_t len)
{
    uint32_t sum = 0;
    
    if(0 == len)  
    {
        return 0;
    }
    
    for(uint16_t index = 0; index < len; index++)
    {
        sum += p_array[index];
    }
	
    return (uint16_t)(sum/len);
}

uint8_t Heart_FilterAverage(uint8_t* p_array, uint8_t len)
{
    uint32_t sum = 0;
    uint8_t  max_heart = p_array[0];
    uint8_t  min_heart = p_array[0];
    
    if(len < 2)
    {
        return 0;
    }
    
    // 搜索最大值
    for(uint8_t index = 0; index < len; index++)
    {
        if(max_heart < p_array[index])
        {
            max_heart = p_array[index];
        }
    }
    
     // 搜索最大值
    for(uint8_t index = 0; index < len; index++)
    {
        if(min_heart > p_array[index])
        {
            min_heart = p_array[index];
        }
    }
    
    for(uint8_t index = 0; index < len; index++)
    {
        sum += p_array[index];
    }
	
    return (uint8_t)((sum-max_heart-min_heart)/(len-2));
}

uint32_t Array_Sum(const uint8_t *p_array, uint16_t len)
{
    uint32_t sum = 0;
    
    for(uint16_t index = 0; index < len; index++)
    {
        sum += p_array[index];
    }
	
    return sum;
}

uint16_t Array_ShortSum(const uint16_t* p_array, uint16_t len)
{
    uint16_t sum = 0;
    
    for(uint16_t index = 0; index < len; index++)
    {
        sum += p_array[index];
    }
	
    return sum;
}

// 获取时间显示值
uint8_t Hour_StandardTransform(uint8_t enable,uint8_t hour)
{
    uint8_t display_hour = hour;
    
    if(1 == enable)
    {
        if(display_hour >= 12)
        {
            display_hour = display_hour-12;
        }
	 
        if(display_hour == 0)
        {
            display_hour = 12;
        }
    }
    
    return display_hour;
}

// 获取AM/PM标记值
uint8_t Get_AMPM_Format(uint8_t enable, uint8_t hour)
{
    uint8_t display_format = 0;
    if(enable == 1)
    {
        if(hour >= 12)
        {
            display_format = 2;
        }
        else
        {
            display_format = 1;
        }
    }
        
    return display_format;
}

bool IsLeapYear(uint16_t year)    
{
    return (!((year) % 400) || (((year) % 100) && !((year) % 4)));
}

static uint32_t DateToDecimalString(const system_clock_time_t *p_time)
//uint32_t DateToDecimalString(const system_clock_time_t *p_time)
{
    return p_time->year*10000 + p_time->month*100 + p_time->day;
}

uint32_t Get_DateToDays(const system_clock_time_t *p_time)
{
    uint32_t total_days = 0;
	
    if(IsLeapYear(p_time->year))
    {
        total_days = Array_Sum(leap_year_days,p_time->month-1);
    }
    else
    {
        total_days = Array_Sum(common_year_days,p_time->month-1);
    }
    
    total_days += p_time->day;

    return total_days;    
}

uint32_t Get_DateDiffDays(const system_clock_time_t *p_time1, const system_clock_time_t *p_time2)
{
    uint32_t diff_days = 0;
    const system_clock_time_t *p_min_time = p_time1;
    const system_clock_time_t *p_max_time = p_time2;
    system_clock_time_t current_time = {0};

    // 查找最大日期
    if(DateToDecimalString(p_time1) >= DateToDecimalString(p_time2))
    {
        p_min_time = p_time2;
        p_max_time = p_time1;
    }	

    for(uint16_t year = p_min_time->year; year < p_max_time->year; year++)
    {
        current_time.year = year;
        current_time.month = 12;
        current_time.day = 31;
        diff_days = Get_DateToDays(&current_time);
    }	
    
    return diff_days + Get_DateToDays(p_max_time) - Get_DateToDays(p_min_time);
}

uint32_t Get_SystemTimeDiffHour(const system_clock_time_t *p_time1, const system_clock_time_t *p_time2)
{
    uint32_t diff_hours = 0;
   
    diff_hours = Get_DateDiffDays(p_time1,p_time2)*24;
    if(DateTimeToDecimalString(p_time1) >= DateTimeToDecimalString(p_time2))
    {
        diff_hours = diff_hours + p_time1->hour - p_time2->hour;
    }
    else 
    {
        diff_hours = diff_hours + p_time2->hour - p_time1->hour;    
    }
	
    return diff_hours;	
}

uint32_t DateTimeToDecimalString(const system_clock_time_t *p_time)//uint8_t month, uint8_t day, uint8_t hour, uint8_t minute)
{
    return p_time->month*1000000 + p_time->day*10000 + p_time->hour*100 + p_time->minutes;	
}

/****************************************************************
** Function name:           Util_AverageByUint8
** Descriptions:            求Uint8 类型的数据平均值
** input parameters:        无
** output parameters:       无
** Returned value:          返回平均值
** Created by:              WJ              
** Created Date:            2017-08-11
*****************************************************************/ 
uint8_t Util_AverageByUint8(uint8_t *input_data, uint16_t input_len)
{
    uint16_t i = 0;
    uint32_t average_value = 0;
    
    if(input_data == NULL || input_len == 0)
    {
        return 0;
    }

    for(i = 0; i < input_len; i++)
    {
        average_value += input_data[i];
    }
    
    average_value = average_value/input_len;
    
    return average_value;
}

/****************************************************************
** Function name:           Sclock_SeekDays
** Descriptions:            计算某个月有多少天
** input parameters:        is_leap_year：是不是闰年；moth:那个月
** output parameters:       无
** Returned value:          返回这个月有多少天
** Created by:              WJ              
** Created Date:            2017-08-11
*****************************************************************/ 
static uint8_t Sclock_SeekDays(bool is_leap_year, uint8_t month)
{
    uint8_t days = 31;
	
    if((month == 4) || ( month == 6) || (month == 9) || ( month == 11))
    {
        days = 30;
    }
    
    /* 闰年月处理 */
    if(month == 2)
    {
        if(is_leap_year)
        {
            days = 29;
        }
        else 
        {
            days = 28;
        }
    }
    return days;
}

/****************************************************************
** Function name:           Sclock_SeekYearDays
** Descriptions:            求某年到某月某日有多少天
** input parameters:        无
** output parameters:       无
** Returned value:          返回天数
** Created by:              WJ              
** Created Date:            2017-08-11
*****************************************************************/ 
uint32_t Sclock_SeekYearDays(uint16_t year, uint8_t month, uint8_t day)
{
    uint32_t year_day = 0;

    if(month > 12 || day > 31)
    {
        return 0;
    }
    
    for(uint8_t index = 1; index <= month; index++)
    {
        if(index == month)
        {
            year_day += day;
        }
        else
        {
            year_day += Sclock_SeekDays(IsLeapYear(year), index);
        }
    }
    return year_day;
}

/****************************************************************
** Function name:           Util_Itoa
** Descriptions:            数字转字符串
** input parameters:        x:输入字符串；
** output parameters:       无
** Returned value:          返回字符串地址
** Created by:              WJ              
** Created Date:            2016-12-07
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
char* Util_Reverse(char *x)
{
    int len = strlen(x);
    int i = 0;
    int n = len / 2;
    char tem = 0;
    
    for (i = 0; i < n; i++)
    {
        tem = x[i];
        x[i] = x[len - 1 - i];
        x[len - 1 - i] = tem;
    }
    return x;
}

/****************************************************************
** Function name:           Util_Itoa
** Descriptions:            数字转字符串
** input parameters:        n:输入数值；
** output parameters:       s:输出字符串
** Returned value:          无
** Created by:              WJ              
** Created Date:            2016-12-07
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Util_Itoa(int n, char s[])
{
    int i = 0;
//    int j = 0;
    int sign = 0;
    
    if((sign = n) < 0)//记录符号
    {
        n = -n;//使n成为正数
    }
    i = 0;
    do
    {
        s[i++] = n % 10 + '0';//取下一个数字
        n = n / 10;
    }
    while(n > 0);//删除该数字
    
    if(sign < 0)
    {
        s[i++]='-';
    }
    s[i] = '\0';

    Util_Reverse(s);   
}


/****************************************************************
** Function name:           Sclock_SeekYearMonthAndDayByDays
** Descriptions:            根据年和天数求月日
** input parameters:        无
** output parameters:       无
** Returned value:          返回天数
** Created by:              WJ              
** Created Date:            2017-08-11
*****************************************************************/ 
uint8_t Sclock_SeekYearMonthAndDayByDays(uint16_t year, uint16_t days, uint8_t *month, uint8_t *day)
{
    uint32_t year_day = 0;
//    uint8_t temp_month = 0;
//    uint8_t temp_day = 0;
    uint8_t month_days = 0;
    
    if(month == NULL || day == NULL)
    {
        return 0;
    }
    for(uint8_t index = 1; index <= 12; index++)
    {
        month_days = Sclock_SeekDays(IsLeapYear(year), index);
        
        if((year_day + month_days) > days)
        {
            *month = index;
            *day = days - year_day;
            return 1;
        }
        year_day += month_days;
        
    }
    return 0;
}

/****************************************************************
** Function name:           Sclock_GetYearMonthDay
** Descriptions:            获取当前系统日期
** input parameters:        无
** output parameters:       无
** Returned value:          返回天数
** Created by:              WJ              
** Created Date:            2017-08-11
*****************************************************************/ 
void Sclock_GetYearMonthDay(uint16_t *year, uint8_t *month, uint8_t *day)
{
    if(year != NULL)
    {
        *year = clock->year;
    }
    if(month != NULL)
    {
        *month = clock->month;
    }
    if(day != NULL)
    {
        *day = clock->day;
    }
    
}

/****************************************************************
** Function name:           Sclock_GetsDaysByTwoDate
** Descriptions:            获取当前系统日期
** input parameters:        year/month/day:小的年月日
                            year1/month1/day1:大的年月日                        
** output parameters:       all_days：总天水
** Returned value:          返回时间是否有效
** Created by:              WJ              
** Created Date:            2017-10-23
*****************************************************************/ 
uint8_t Sclock_GetsDaysByTwoDate(uint16_t year, uint8_t month, uint8_t day, uint16_t year1, 
                                  uint8_t month1, uint8_t day1, uint32_t *all_days)
{
    uint16_t i = 0;
    uint32_t total_days = 0;
    uint32_t days = 0;
    uint32_t days1 = 0;
    
    if(year > year1)
    {
        //时间不对
        *all_days = 0;
        return 0;
    }
    days = Sclock_SeekYearDays(year, month, day);
    days1 = Sclock_SeekYearDays(year1, month1, day1);
    
    //WJ.20171107.没有比较年份
    if(year == year1 && days > days1)
    {
        //时间不对
        *all_days = 0;
        return 0;
    }
    
    if(year == year1)
    {
        total_days = days1 - days;
    }
    else
    {
        total_days = Sclock_SeekYearDays(year1, month1, day1);
        for(i = year; i < year1; i++)
        {
            total_days += Sclock_SeekYearDays(i, 12, 31);
        }
        total_days = total_days - Sclock_SeekYearDays(year, month, day);
    }
    
    *all_days = total_days;
    return 1;
}

/****************************************************************
** Function name:           Sclock_GetTime
** Descriptions:            获取当前系统时间
** input parameters:        year/month/day:小的年月日
                            year1/month1/day1:大的年月日                        
** output parameters:       all_days：总天水
** Returned value:          返回时间是否有效
** Created by:              WJ              
** Created Date:            2017-10-23
*****************************************************************/ 
void Sclock_GetTime(uint8_t *hours, uint8_t *minutes, uint8_t *second)
{
    if(hours != NULL)
    {
        *hours = clock->hour;
    }
    if(minutes != NULL)
    {
        *minutes = clock->minutes;
    }
    if(second != NULL)
    {
        *second = clock->seconds;
    } 
}


/****************************************************************
** Function name:           Util_GetPreviousDayDate
** Descriptions:            获取前几天的日期
** input parameters:        time1:当前时间,长度为4,年月日，小端
                            day:前几天
** output parameters:       time2:计算时间,长度为4,年月日，小端
** Returned value:          无
** Created by:              WJ              
** Created Date:            2018-12-10
*****************************************************************/ 
void Util_GetPreviousDayDate(uint8_t *time1, uint16_t pre_day, uint8_t *time2)
{
    uint8_t     temp_month = 0;
    uint8_t     temp_day = 0;
    uint16_t    temp_year = 0;
    uint16_t    i = 0;
    
    temp_year = time1[0] + (time1[1]<<8);
    temp_month = time1[2];
    temp_day = time1[3];
    
    if(pre_day > 0xFFFF || time2 == NULL || time1 == NULL)
    {
        return;
    }
    
    for(i = 0; i < pre_day; i++)
    {
        if(temp_day > 1)
        {
            temp_day = temp_day - 1;
        }
        else
        {
            if(temp_month > 1)
            {
                temp_month = temp_month - 1;       //WJ.20170322
            }
            else
            {
                temp_month = 12;
                temp_year--;
            }
            temp_day = Sclock_SeekDays(IS_LEAP_YEAR(temp_year), temp_month); 
        }            
    }
    
    time2[0] = temp_year & 0xFF;
    time2[1] = (temp_year>>8) & 0xFF;
    time2[2] = temp_month & 0xFF;
    time2[3] = temp_day & 0xFF;
}



#define BP_CALIBRATION_SET_DIFFERENCE   24
#define BP_CALIBRATION_RESULT_DIFFERENCE   15
/****************************************************************
** Function name:           Bp_ResultCheck
** Descriptions:            结果自检
** input parameters:          
** output parameters:
** Returned value:
** Created by:              WJ              
** Created Date:            2017-01-05
*****************************************************************/
uint8_t Bp_ResultCheck(uint8_t check_mode, uint8_t check_sp, uint8_t check_dp, uint8_t check_heart)
{
    if(check_mode == 1)
    {
		if(check_sp < 61 || check_sp > 229 || check_dp < 26 || check_dp > 199 ||
            (check_sp <= (check_dp + BP_CALIBRATION_RESULT_DIFFERENCE)) || check_heart < 30 || check_heart > 200)
        {
            return 0;
        } 
    }
    else
    {
        if(check_sp < 81 || check_sp > 209 || check_dp < 46 || check_dp > 179 ||
            (check_sp <= (check_dp + BP_CALIBRATION_RESULT_DIFFERENCE)) || check_heart < 30 || check_heart > 200)
        {
            return 0;
        }              
    }
    return 1;
}

/****************************************************************
** Function name:           Bp_SetCalibrationCheck
** Descriptions:            设置自检
** input parameters:          
** output parameters:
** Returned value:
** Created by:              WJ              
** Created Date:            2017-01-05
*****************************************************************/
uint8_t Bp_SetCalibrationCheck( uint8_t check_sp, uint8_t check_dp)
{
    if(check_sp < 81 || check_sp > 209 || check_dp < 46 || check_dp > 179 ||
        (check_sp <= (check_dp + BP_CALIBRATION_SET_DIFFERENCE)) )
    {
        return 0;
    }              
    
    return 1;
}
