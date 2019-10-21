#ifndef _BATTERY_ADC_H_
#define _BATTERY_ADC_H_

#include <stdint.h>

#define BATTERY_CHECK_COUNT  120

typedef enum
{
    BATTERY_POWER_NORMAL = 0,      //电池正常使用
    BATTERY_POWER_CHARGE = 1,      //电池充电状态
    BATTERY_POWER_FULL = 2,        //电池满电状态
}battery_status;

typedef struct
{
    int16_t  adc;           //当前ADC值
    int16_t  grade;         //当前电池等级
	uint8_t  bat_grade_low_cnt;   //低电计数
    uint8_t  bat_grade0_cnt;
    uint8_t  bat_grade1_cnt;
    uint8_t  bat_grade2_cnt;
    uint8_t  bat_grade3_cnt;
    uint8_t  bat_grade4_cnt;
    
    battery_status  status;
}battery_param;


void Battery_GiveMove(void);

uint8_t Battery_TakeMove(void);
    
void Battery_ADC_Init(void);

void Battery_PowerOnProcess(void);

void Algorithm_BatteryGradeTraceTask(void);

uint8_t Battery_Get_Grade(void);

battery_status Battery_Get_Status(void);

#endif
