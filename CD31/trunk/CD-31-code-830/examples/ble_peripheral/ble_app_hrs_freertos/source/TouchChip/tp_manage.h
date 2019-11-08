/*******************Copyright(c)*********************************
** Veepoo Tech
** COPYRIGHT (C) 2016-2019
**----------------------FileInfo---------------------------------
** File name:                  tp_manage.h
** Last modified Date:         2018-06-07
** Last SYS_VERSION:           0.0.1
** Descriptions:
**---------------------------------------------------------------
** Createdby:                  YX
** Createddate:                2018-06-07
** SYS_VERSION:                0.0.1
** Descriptions:               TP
**---------------------------------------------------------------
** Modified by:				
** Modified date:			
** SYS_VERSION:				
** Modified Descriptions:	
****************************************************************/

#ifndef TP_MANAGE_H
#define TP_MANAGE_H

#include "FreeRTOS.h"
#include "timers.h"
#include "app_timer.h"
#include "app_type.h"
#include "tp_gesture.h"
#include "tp_cal_data.h"

#include "cpu_common.h"

#define APP_TIME_TP_INTERVAL    60      //60ms

#define NULL_STATE          ((uint8_t)0x00)       //无状态
#define UP_SLIDE_STATE      ((uint8_t)0x01)       //处于上滑
#define DOWN_SLIDE_STATE    ((uint8_t)0x02)       //处于下滑
#define LEFT_SLIDE_STATE    ((uint8_t)0x03)       //处于左滑
#define RIGHT_SLIDE_STATE   ((uint8_t)0x04)       //处于右滑
#define CLICK_STATE         ((uint8_t)0x05)       //处于单击
#define LONG_PRESS_3S_STATE ((uint8_t)0x06)       //处于长按3s
#define LONG_PRESS_6S_STATE ((uint8_t)0x07)       //处于长按6s

#define PRESS_DOWN_N_STATE   ((uint8_t)0x00)       //未按下状态
#define PRESS_DOWN_Y_STATE    ((uint8_t)0x01)      //按下状态


#define SLEEP_MODE_1  1     //单击唤醒
#define SLEEP_MODE_2  2     //双击唤醒
#define SLEEP_MODE_3  3     //长按唤醒(超过5S，在8S内，抬手唤醒)
#define SLEEP_MODE_4  4     //滑动唤醒
#define SLEEP_MODE_5  5     //长按唤醒(超过5S唤醒，不用抬手)
#define SLEEP_MODE_6  6     //不唤醒(默认使用)

typedef struct
{
    uint8_t             tp_run_status:2;	//运行的状态
    uint8_t             tp_init:1;	        //初始化
    uint8_t             tp_result_status:1;	//结果状态
    uint8_t             tp_data_status:1;	//数据的状态
    uint8_t             tp_timer_status:1;	//定时器的状态
    uint8_t             resv:2;             //预留
    tp_gesture_info_t   tp_result_s;	    //每个触摸点的数据
}tp_param_t;

//获取是否需要清除中断标志的状态
uint8_t Tp_GetI2cIntStatus(void);

//设置是否需要清除中断标志的状态
uint8_t Tp_SetI2cIntStatus(uint8_t status);

//获取TP通信状态
uint8_t Tp_GetI2cStatus(void);

//设置TP通信状态
uint8_t Tp_SetI2cStatus(uint8_t status);

//是否休眠唤醒
uint8_t Tp_IsSleepWakeup(void);

//Tp唤醒
void Tp_WakeUp(void);

//TP休眠
void Tp_Sleep(uint8_t sleep_mode);

//TP关机
void Tp_ShutDown(void);

//TP初始化
void Tp_Init(uint8_t mode);

//获取触摸按键的状态
uint8_t Tp_GetKeyStatus(tp_key_info* tp_key);

//获取TP的按下状态
uint8_t Tp_GetPressDownSatus(void);

//清除TP的相关参数
void Tp_ClearInfo(void);

//方形区域识别
uint8_t Tp_SquareAreaDiscern(point_coord_t point, uint16_t area_point_x, uint16_t area_point_y, uint16_t wide, uint16_t high);

//圆形区域识别
uint8_t Tp_CircleAreaDiscern(point_coord_t point, uint16_t area_point_x, uint16_t area_point_y, uint16_t radius);

#endif  //TP_MANAGE_H

