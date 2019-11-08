/***************************Copyright(c)***********************************
**                       Veepoo Tech
**                  COPYRIGHT (C) 2016-2019
**-----------File Info-----------------------------------------------------
** File        name:     tp_gesture.h
** Last modified Date:   2018-03-23
** Last SYS_VERSION:     0.0.0
** Descriptions:            
**-------------------------------------------------------------------------
** Created  by:          YX
** Created  date:        2018-03-23
** SYS_VERSION:          0.0.0
** Descriptions:         
**-------------------------------------------------------------------------
** Modified by:				
** Modified date:  			          
** SYS_VERSION:                 
** Descriptions:                        
***************************************************************************/

#ifndef TP_GESTURE_H__
#define TP_GESTURE_H__

#include "stdint.h"
#include <string.h>

#define TP_NULL_STATE        (0)    //空
#define TP_START_STATE       (1)    //起始
#define TP_STOP_STATE        (2)    //结束

#define TP_MODE_NULL            ((uint16_t)0x0000)       //空
#define TP_MODE_CLICK           ((uint16_t)0x0001)       //单击
#define TP_MODE_SLIDE           ((uint16_t)0x0002)       //滑动
#define TP_MODE_MARGIN_SLIDE    ((uint16_t)0x0004)       //边缘滑动
#define TP_MODE_LONG_PRESS_3S   ((uint16_t)0x0008)       //长按3s
#define TP_MODE_LONG_PRESS_6S   ((uint16_t)0x0010)       //长按6s
#define TP_MODE_TRANSLATION     ((uint16_t)0x0020)       //平移

/**************************
(0,MAX_Y)..............(MAX_X,MAX_Y)
..........................
..........................
..........................
(0,0)...............(MAX_X,0)
**************************/
#define TP_NULL_MARGIN_STATE    ((uint8_t)0x00)       //不处于边缘
#define TP_UP_MARGIN_STATE      ((uint8_t)0x01)       //处于上边缘
#define TP_DOWN_MARGIN_STATE    ((uint8_t)0x02)       //处于下边缘
#define TP_LEFT_MARGIN_STATE    ((uint8_t)0x04)       //处于左边缘
#define TP_RIGHT_MARGIN_STATE   ((uint8_t)0x08)       //处于右边缘

#define TP_NULL_SLIDE_STATE    ((uint8_t)0x00)       //没有滑动
#define TP_DOWN_SLIDE_STATE    ((uint8_t)0x01)       //处于下滑
#define TP_UP_SLIDE_STATE      ((uint8_t)0x02)       //处于上滑
#define TP_LEFT_SLIDE_STATE    ((uint8_t)0x01)       //处于左滑
#define TP_RIGHT_SLIDE_STATE   ((uint8_t)0x02)       //处于右滑

#define TP_X_SLIDE_STATE       ((uint8_t)0x01)       //处于X方向
#define TP_Y_SLIDE_STATE       ((uint8_t)0x02)       //处于Y方向

//当前位置支持的操作方式
#define TP_GEST_NULL                        ((uint32_t)0x00000000)      //空
#define TP_GEST_CLICK                       ((uint32_t)0x00000001)      //单击
#define TP_GEST_LONG_PRESS_3S               ((uint32_t)0x00000002)      //3秒长按
#define TP_GEST_LONG_PRESS_6S               ((uint32_t)0x00000004)      //6秒长按
#define TP_GEST_X_SLIDE                     ((uint32_t)0x00000008)      //X方向滑动
#define TP_GEST_Y_SLIDE                     ((uint32_t)0x00000010)      //Y方向滑动
#define TP_GEST_LEFT_MARGIN_SLIDE           ((uint32_t)0x00000020)      //左边缘滑动
#define TP_GEST_RIGHT_MARGIN_SLIDE          ((uint32_t)0x00000040)      //右边缘滑动
#define TP_GEST_UP_MARGIN_SLIDE             ((uint32_t)0x00000080)      //上边缘滑动
#define TP_GEST_DOWN_MARGIN_SLIDE           ((uint32_t)0x00000100)      //下边缘滑动
#define TP_GEST_TRANSLATION                 ((uint32_t)0x00000200)      //平移

#define MAX_SUPPORTED_FINGER_NUM 1  //支持触摸点的数量

typedef struct
{
	uint16_t x;		//x坐标
	uint16_t y;		//Y坐标
}coord_info_t;

typedef struct
{
	coord_info_t point;	//点坐标
	uint16_t wide;		//宽
    uint16_t high;		//高
}area_square_info_t;    //方形区域


typedef struct
{
	coord_info_t point;	//圆点坐标
	uint16_t radius;	//半径
}area_circle_info_t;    //圆形区域


typedef struct
{
	uint8_t x_direct;		//X方向(向左、向右)
	uint8_t y_direct;		//Y方向(向上、向下)
	uint8_t xy_direct;		//总方向(X方向,Y方向)
	uint8_t xy_degree;		//偏移的角度,暂时不用
}move_direct_info_t;

typedef struct
{
	uint16_t x_dist;		//X移动距离
	uint16_t y_dist;		//Y移动距离
	uint16_t xy_dist;		//直线移动距离
}move_dist_info_t;

typedef struct
{
    uint8_t tp_state;					//状态(0:错误,1:识别成功,2:操作完成)
    uint16_t now_mode;					//当前模式(单击、滑动)
    uint16_t total_mode;				//总模式(边缘滑动、滑动、平移、单击、长按)
    uint8_t margin_state;               //边缘的位置
	move_direct_info_t  now_direct;		//当前方向
	move_direct_info_t  total_direct;	//总方向
	coord_info_t start_coord;			//起始坐标
	coord_info_t last_coord;			//上一次坐标(上一次的最后一个坐标)
	coord_info_t now_coord;				//当前坐标(最后一个坐标)
	move_dist_info_t total_move_dist;	//总移动距离
	move_dist_info_t each_move_dist;	//当次移动距离
}tp_gesture_info_t;//手势的结果数据

typedef struct
{
    uint16_t mode;                      //模式
    coord_info_t last_coord;			//上一次坐标
	coord_info_t now_coord;				//当前坐标
    move_direct_info_t direct;          //方向
    move_dist_info_t move_dist;         //距离
}tp_stage_data_t;


typedef struct
{
    uint8_t         margin;     //边缘属性(0：无，1:上，2：下，4：左，8：右)
    uint8_t         mode;       //模式属性(0：无，1：单击，2：长按，8：滑动)
    uint8_t         direct;     //方向属性(0：无，1:上，2：下，3：左，4：右)
    uint16_t        dist;       //距离属性
    coord_info_t    start_coord;//开始坐标
    coord_info_t    stop_coord; //结束坐标
}tp_result_t;


#define TP_ALL_DATA_T   tp_all_data_t
#define TP_COORD_DATA_T coord_info_t

/*报点相关结构体的定义*/
//typedef struct
//{
//    uint16_t    x;			//x坐标
//    uint16_t    y;			//Y坐标
//    uint8_t    width;		    //宽度
//    uint8_t    sub_status;	//状态
//}TP_COORD_DATA_T;             //每个触摸点的参数

typedef struct
{
    uint16_t           status;	//总状态
    TP_COORD_DATA_T    coord;	//每个触摸点的数据
}TP_ALL_DATA_T;	//tp返回的所有参数

extern uint32_t simul_data_len;
extern TP_ALL_DATA_T simul_data_t[];
//模拟数据初始化
void Tp_SimulatorDataProcess(void);
    
//正常30ms计算一次(出现结束状态时,马上进行一次计算,防止两次操作出现一次计算中)
uint32_t Tp_GestureProcess(TP_ALL_DATA_T * tp_data ,uint32_t len, uint8_t resv);

//获取手势结果
tp_gesture_info_t * Tp_GetGesture(void);

//传入支持的操作,使用方式待定
//uint32_t Tp_SetGestureParam(uint32_t state);
tp_result_t Tp_SetGestureParam(uint32_t state);

//初始化(采集数据的频率:data_hz,边缘百分比x、y:margin_x、margin_y,平移手势和其他手势区分时间:time)
//uint32_t Tp_GestureInit(uint32_t data_hz, uint8_t margin_x, uint8_t margin_y, uint32_t time);
uint32_t Tp_GestureInit(uint32_t data_hz, uint16_t max_x_coord, uint16_t max_y_coord, uint8_t margin_x, uint8_t margin_y, uint32_t time, uint32_t slide_min_dist);

#endif
