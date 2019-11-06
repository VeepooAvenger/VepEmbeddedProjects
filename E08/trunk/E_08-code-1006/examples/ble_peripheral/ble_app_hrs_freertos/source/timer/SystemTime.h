#ifndef _SYSTEM_TIME_H_
#define _SYSTEM_TIME_H_

#include <stdint.h>
#include <stdbool.h>
#include "common_data_type.h"


extern const system_clock_time_t *clock;

void SystemTime_Update_Second(void);

/* 系统时间初始化 */
void SystemTime_Init(void);

void SystemTime_Read(uint8_t* pData,uint8_t len);

void SystemTime_Adjust(const uint8_t* pData,uint8_t len);

uint8_t SystemTime_Check(uint8_t* p_data, uint8_t len);

uint8_t SystemTime_Check1(uint8_t *p_data, uint8_t len);

void SystemTime_Enable(void);

void Sclock_GetPreviousDayDate(uint16_t p_day, uint8_t *month, uint8_t *day);

//p_time1时间大于p_time2
uint8_t SystemTime_Compare(uint8_t *p_time1, uint8_t *p_time2); 

#endif
