#ifndef _UTIL_H_
#define _UTIL_H_

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "common_data_type.h"

// ͼ����ת90��
void Image_RightRotate(uint8_t* pInput,uint8_t* pOutput,uint8_t width,uint8_t high);

// ͼ����ת90��
void Image_LeftRotate(uint8_t* pInput,uint8_t* pOutput,uint8_t width,uint8_t high); 

void distance_value(uint32_t *step_input,uint32_t *meter,float *kilometer,float *mile, uint32_t *calories);

uint8_t Heart_FilterAverage(uint8_t* p_array, uint8_t len);

uint8_t Average_CharArray(uint8_t* p_array, uint8_t len);

uint16_t Average_ShortArray(uint16_t* p_array, uint16_t len);

uint32_t Array_Sum(const uint8_t *p_array, uint16_t len);

// �����������
uint16_t Array_ShortSum(const uint16_t* p_array, uint16_t len);

// ��ȡʱ����ʾֵ
uint8_t Hour_StandardTransform(uint8_t enable,uint8_t hour);

// ��ȡAM/PM���ֵ
uint8_t Get_AMPM_Format(uint8_t enable, uint8_t hour);

bool IsLeapYear(uint16_t year);   

//
//uint32_t DateToDecimalString(const system_clock_time_t *p_time);

// ���������������ڲ�ֵ
uint32_t Get_DateToDays(const system_clock_time_t *p_time);

// ������������ʱ���
uint32_t Get_DateDiffDays(const system_clock_time_t *p_time1, const system_clock_time_t *p_time2);

// ����ʱ���ֵ(Сʱ)
uint32_t Get_SystemTimeDiffHour(const system_clock_time_t *p_time1, const system_clock_time_t *p_time2);

// ʱ��ת��Ϊ�ַ���
uint32_t DateTimeToDecimalString(const system_clock_time_t *p_time);//(uint8_t month, uint8_t day, uint8_t hour, uint8_t minute);

uint8_t Util_AverageByUint8(uint8_t *input_data, uint16_t input_len);

uint32_t Sclock_SeekYearDays(uint16_t year, uint8_t month, uint8_t day);

//��ֵת�ַ���
void Util_Itoa(int n, char s[]);

//�����������������
uint8_t Sclock_SeekYearMonthAndDayByDays(uint16_t year, uint16_t days, uint8_t *month, uint8_t *day);

//��ȡ��ǰϵͳ����
void Sclock_GetYearMonthDay(uint16_t *year, uint8_t *month, uint8_t *day);

//��ȡ��ǰϵͳ���ڣ�year/month/day:С�������գ�year1/month1/day1:��������գ�
//all_days������ˮ������ʱ���Ƿ���Ч
uint8_t Sclock_GetsDaysByTwoDate(uint16_t year, uint8_t month, uint8_t day, uint16_t year1, 
                                  uint8_t month1, uint8_t day1, uint32_t *all_days);

void Sclock_GetTime(uint8_t *hours, uint8_t *minute, uint8_t *second);


void Util_GetPreviousDayDate(uint8_t *time1, uint16_t pre_day, uint8_t *time2);



uint8_t Bp_ResultCheck(uint8_t check_mode, uint8_t check_sp, uint8_t check_dp, uint8_t check_heart);

uint8_t Bp_SetCalibrationCheck( uint8_t check_sp, uint8_t check_dp);

#endif

