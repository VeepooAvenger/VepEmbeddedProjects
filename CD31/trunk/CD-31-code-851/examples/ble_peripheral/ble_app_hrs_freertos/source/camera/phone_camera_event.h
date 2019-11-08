/***************************Copyright(c)***********************************
**                       Veepoo Tech
**                  COPYRIGHT (C) 2016-2019
**-----------File Info-----------------------------------------------------
** File        name:     phone_camera_event.h
** Last modified Date:   2017-08-21
** Last SYS_VERSION:     0.0.0
** Descriptions:            
**-------------------------------------------------------------------------
** Created  by:          WJ
** Created  date:        2017-08-21
** SYS_VERSION:          0.0.0
** Descriptions:         
**-------------------------------------------------------------------------
** Modified by:				
** Modified date:  			          
** SYS_VERSION:                 
** Descriptions:                        
***************************************************************************/

#ifndef _PHONE_CAMERA_EVENT_H_
#define _PHONE_CAMERA_EVENT_H_

#include <stdint.h>
#include <stdbool.h>

/* 相机事件结构体 */
typedef struct
{
		uint8_t  ready:1;             /* 手环拍照模式就绪 */
		uint8_t  photo_ok:1;          /* 手环拍照已发送 */
        uint8_t  photo_state:1;
		uint8_t  resv:5;              /* 保留位 */
	
		uint8_t  current_time;        /* 当前时间 */
		uint8_t  internal_time;       /* 摇一摇间隔时间 */
		uint8_t  ready_keep_time;     /* 拍照就绪保持时间 */
	  uint8_t  tp_keep_time;        /* 拍照OK保持时间 */
}camera_control_t;


//进入拍照模式，开启ＵＩ显示
void Camera_BraceletEntry(void);

//点击拍照
void Camera_BtnClick(void);

//退出拍照，关闭ＵＩ
void Camera_BraceletExit(void);

//退出拍照发送数据
void Camera_BraceletExitSendData(void);

//拍照处理
void Camera_WaitTask(void);

//获取拍照控制参数
camera_control_t *Camera_GetState(void);

#endif
