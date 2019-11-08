/***************************Copyright(c)***********************************
**                       Veepoo Tech
**                  COPYRIGHT (C) 2016-2019
**-----------File Info-----------------------------------------------------
** File        name:     physiological_period.h
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

#ifndef APP_PHYSIOLOGICAL_PERIOD_H_
#define APP_PHYSIOLOGICAL_PERIOD_H_

#include <stdint.h>
      
#define RMIND_INFO_MAX_SIZE   200      //显示消息的最大长度为200

typedef enum
{
    PHYS_PERIOD_MESTRUAL = 1,           //经期
    PHYS_PERIOD_FOR_PREGNANCY,          //备孕期
    PHYS_PERIOD_PREGNANCY,              //孕期
    PHYS_PERIOD_HOT_MAMA,               //辣妈期
    PHYS_PERIOD_MAX
    
}PHYS_PERIOD_TYPE_E;

typedef enum
{
    PHYS_MENSTRUAL_NULL = 0,                   //非法
    PHYS_MENSTRUAL_MENSTRUAL_PERIOD,           //经期
    PHYS_MENSTRUAL_SAFETY_PERIOD,              //安全期
    PHYS_MENSTRUAL_OVIPOSIT_PERIOD,            //排卵期
    PHYS_MENSTRUAL_OVIPOSIT_DAY,               //排卵日   
   
}PHYS_MENSTRUAL_TYPE_E;

typedef enum
{
    PHYS_REMIND_NULL = 0,                   //非法
    PHYS_REMIND_MENSTRUAL_PERIOD,           //经期
    PHYS_REMIND_SAFETY_PERIOD,              //安全期
    PHYS_REMIND_SAFETY_PERIOD1,             //安全期1，距离经期 <=3
    PHYS_REMIND_OVIPOSIT_PERIOD,            //排卵期
    PHYS_REMIND_OVIPOSIT_DAY,               //排卵日
    PHYS_REMIND_OVIPOSIT_PERIOD1,           //备孕期排卵期
    PHYS_REMIND_OVIPOSIT_DAY1,              //备孕期排卵日
    PHYS_REMIND_FORECAST_MENSTRUAL_PERIOD,  //预测经期    
    
    PHYS_REMIND_PREGNANCY_ONE,              //怀孕第一阶段（1~3个月）
    PHYS_REMIND_PREGNANCY_TWO,              //怀孕第二阶段（3~6个月）
    PHYS_REMIND_PREGNANCY_THU,              //怀孕第三阶段（6个月~出生前）
    PHYS_REMIND_PREGNANCY_BIRTH,            //怀孕第四阶段（宝宝出生）
    PHYS_REMIND_PREGNANCY_ERROR,            //怀孕信息过期，更新信息
    PHYS_REMIND_BABY_BORN,                  //宝宝出生后
   
}PHYS_REMIND_TYPE_E;

#pragma pack(1)
//存储
typedef struct
{
	uint8_t  phys_period_time;          //处于什么生理时期,0:没有记录,1:经期,2:备孕期,3:怀孕期,4:辣妈
	uint16_t phys_period_year;          //最近一次来月经:年
	uint8_t  phys_period_month;         //最近一次来月经:月
	uint8_t  phys_period_day;           //最近一次来月经:日
	uint8_t  phys_period_cycle;         //月经周期
	uint8_t  phys_mens_days;            //经期天数
	uint8_t  phys_mens_actual_days;     //实际经期天数
    
	uint16_t baby_birthday_year;        //宝宝出生日期:年
	uint8_t  baby_birthday_month;       //宝宝出生日期:月
	uint8_t  baby_birthday_day;         //宝宝出生日期:日
	uint8_t  baby_birthday_sex;         //宝宝出生性别
    uint8_t  retain[3];                 //对齐保留
} phys_period_info_t;

//计算
typedef struct
{
	uint8_t   menstrual_period_type;         //生理周期类型(安全期/排卵期等)
	uint16_t  menstrual_period_year;         //下一次来月经:年
	uint8_t   menstrual_period_month;        //下一次来月经:月    
	uint8_t   menstrual_period_day;          //下一次来月经:日
	uint8_t   menstrual_period_actual_days;  //月经期持续时间，注："实际经期"为0时,使用"经期天数"
    uint8_t   menstrual_period_oviposit_days;//第几天进入排卵日     
    uint16_t   remind_type;                   //提醒类型
	uint16_t   remind_days;                   //提醒天数
    
    uint16_t  baby_birthday_days;            //宝宝出生天数    
}next_menstrual_period_t;

typedef struct
{
    uint8_t  remind_info_buf[RMIND_INFO_MAX_SIZE];
    uint16_t data_size;   
    uint8_t  update;  
    
    uint8_t  is_reading;
    uint8_t  read_index; 
    uint8_t  read_len; 
    uint8_t  read_page;    
    uint16_t read_index_buf[5];  
    uint8_t  total_page;
    
    uint8_t  language;
    uint8_t  hours;
    uint8_t  minutes;    
    uint8_t  type;
    uint16_t days;      //WJ.20180816.参数溢出，修正宝妈期超过一周岁，更改语言后提示Bug 
}phys_remind_info_t;

#pragma pack()


extern phys_remind_info_t phys_remind_info_s;   //女性功能提醒消息

void Phys_InitParam(void);

// 清除数据初始化
void Phys_ResetInitParam(void);

//女性数据保存
void Phys_DelaySave(void);

//女性数据关机保存
void Phys_PowerOffSave(void);

void Phys_ClearRemindInfo(void);

//解析女性功能协议
void Phys_ParsingProtocol(uint8_t *in_data, uint8_t *out_data);

//获取下一页信息内容
uint8_t Phys_DisplayRemindByNextPage(uint8_t reading_flag);


//存在未读信息
uint8_t Phys_ExistUnreadMessage(void);

//时间处理监听（1分钟一次）
void Phys_Monitor(void);

//获取经期处于什么阶段
uint8_t Phys_GetMenstrualPeriodType(void);

void Phys_UpdataInfo(void);

void Phys_UpdataParam(void);

void Phys_Test(void);

#endif //APP_PHYSIOLOGICAL_PERIOD_H_
