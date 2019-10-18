/***************************Copyright(c)***********************************
**                       Veepoo Tech
**                  COPYRIGHT (C) 2016-2019
**-----------File Info-----------------------------------------------------
** File        name:     hal_acc_calculate.h
** Last modified Date:   2017-08-24
** Last SYS_VERSION:     0.0.0
** Descriptions:            
**-------------------------------------------------------------------------
** Created  by:          WJ
** Created  date:        2017-08-24
** SYS_VERSION:          0.0.0
** Descriptions:         
**-------------------------------------------------------------------------
** Modified by:				
** Modified date:  			          
** SYS_VERSION:                 
** Descriptions:                        
***************************************************************************/

#ifndef _HAL_ACC_CALCULATE_H_
#define _HAL_ACC_CALCULATE_H_

 
#define APP_HALACC_ENABLED     1

#if APP_HALACC_ENABLED

//加速度事件处理
void Acc_StartHandler(void);

//计步、姿态计算
void Acc_Calculate(void);

// 定时器创建
void Acc_TimerCreate(void);

//定时器开启
void Acc_TimerStart(void);

//加速度配置初始化
void Acc_Start(void);

//加速度停止工作，休眠
void Acc_Stop(void);

//加速度初始化
void Acc_Init(void);

//获取xyz轴数据
void Acc_GetData(uint16_t *data);

//加速度唤醒中
uint8_t Acc_Is_Move(void);

//加速度已初始化
uint8_t Acc_Is_Init(void);

//翻腕亮屏标志位清除
void Acc_GestureOledFlagClean(void);

//获取翻腕亮屏标志位
uint8_t Acc_GestureOledFlagGet(void);

//发送加速度配置
void Acc_SendConfig(void);

//重置加速度
void Acc_RestConfig(void);

#endif //APP_HALACC_ENABLED

#endif
