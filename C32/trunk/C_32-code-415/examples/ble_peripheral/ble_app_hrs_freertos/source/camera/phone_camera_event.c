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
#include <stdint.h>
#include <string.h>

#include "phone_camera_event.h"
#include "pedometer.h"
#include "bracelet_protocol.h"
#include "gui_thread.h"
#include "app_type.h"
#include "battery.h"

/* 变量声明及初始化 */
static camera_control_t  camera_control_s = 
{
    .ready = 0x00,
    .photo_ok = 0x00,
    .photo_state = 0x00,
    .resv = 0x00,

    .current_time = 0x00,
    .internal_time = 0x00,
    .tp_keep_time = 0x00,
};

extern system_flag_t system_flag_s;

uint8_t camera_delay_cnt = 0;       //拍照延时计数，大于1，震动才会响应拍照

/****************************************************************
** Function name:           Camera_BraceletEntry
** Descriptions:            进入拍照模式
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ             
** Created Date:            2017-08-21
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Camera_BraceletEntry(void)
{
    uint8_t camera_ble[20] = {0xB6, 0x01,0x01,};
    
    camera_control_s.photo_ok = 0x00;
    camera_control_s.ready = 0x01;
    camera_control_s.tp_keep_time = 0x00;
    camera_control_s.ready_keep_time = 60;
    camera_control_s.photo_state = 0x01;
    /* 向APP发送推出消息 */    
    Ble_SendDataViaBat(camera_ble, 20);

    //进入拍照模式
    system_flag_s.camera_mode = 1;
    //拍照延时计数
    camera_delay_cnt = 0;
}

/****************************************************************
** Function name:           Camera_BtnClick
** Descriptions:            按键点击拍照
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ             
** Created Date:            2017-08-21
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Camera_BtnClick(void)
{
    uint8_t camera_ble[20] = {0xB6, 0x01,0x02,};
    
    camera_control_s.ready = 0x01;
    camera_control_s.photo_ok = 0x01;
    camera_control_s.tp_keep_time = 0x02;
    camera_control_s.ready_keep_time = 60;

    /* 向APP发送推出消息 */
    Ble_SendDataViaBat(camera_ble, 20);
}

/****************************************************************
** Function name:           Camera_BraceletExit
** Descriptions:            退出拍照发送数据关闭UI
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ             
** Created Date:            2017-08-21
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Camera_BraceletExit(void)
{
    uint8_t camera_ble[20] = {0xB6, 0x01, 0x00,};
    
    if(system_flag_s.camera_mode == 1)
    {
        /* 所有变量初始化 */
        memset(&camera_control_s, 0, sizeof(camera_control_s));

        /* 向APP发送推出消息 */
        Ble_SendDataViaBat(camera_ble, 20);
        camera_control_s.photo_state = 0x00;
    
    }
    //退出拍照模式
    system_flag_s.camera_mode = 0;    
    //WJ.20170922.防止退出拍照时有其他事件正在执行
    if(Gui_GetThreadJob() == CPU_EVENT_TASK_CAMERAREADY 
      || Gui_GetThreadJob() == CPU_EVENT_TASK_CAMERAOK )
    {
        Gui_ThreadEventTaskWakeUp(CPU_EVENT_TASK_ALL_EXIT); 
    }
}

/****************************************************************
** Function name:           Camera_BraceletExitSendData
** Descriptions:            退出拍照发送数据
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ             
** Created Date:            2017-08-21
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Camera_BraceletExitSendData(void)
{
    uint8_t camera_ble[20] = {0xB6, 0x01, 0x00,};
    
    /* 所有变量初始化 */
    memset(&camera_control_s, 0, sizeof(camera_control_s));

    /* 向APP发送推出消息 */
    Ble_SendDataViaBat(camera_ble, 20);
    camera_control_s.photo_state = 0x00;
}

/****************************************************************
** Function name:           Camera_WaitTask
** Descriptions:            照相事件处理
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ             
** Created Date:            2017-08-21
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Camera_WaitTask(void)
{	
    uint8_t camera_ble[20] = {0xB6, 0x01, 0x02,};
    
    //WJ.20170511提高灵敏度
    if((camera_delay_cnt++ >= 1) && (Vph_GetShakeFlag() >= 2) && (camera_control_s.internal_time == 0x00) && (!Motor_IsWork())
       &&(Gui_GetThreadJob() == CPU_EVENT_TASK_CAMERAREADY || Gui_GetThreadJob() == CPU_EVENT_TASK_CAMERAOK)) //马达没有工作
    {       
        /* 推送消息 */
        Ble_SendDataViaBat(camera_ble, 20);
        
        /* 记录当前时间 */
        camera_control_s.current_time = 0xFF;
        camera_control_s.photo_ok = 0x01;
        camera_control_s.ready = 0x01;
        camera_control_s.tp_keep_time = 0x02;
        camera_control_s.ready_keep_time = 60;
        camera_control_s.internal_time = 0x02;
    }
            
    /* 处理时间逻辑 */
    if(camera_control_s.current_time != clock->seconds)
    {
        camera_control_s.current_time = clock->seconds;
    
        if(camera_control_s.tp_keep_time > 0)
        {
            camera_control_s.tp_keep_time--;
        }
        
        if(camera_control_s.ready_keep_time > 0)
        {
            camera_control_s.ready_keep_time--;
        }
        
        if(camera_control_s.internal_time > 0)
        {
            camera_control_s.internal_time--;
        }
        
        /* 如果两个都为0 */
        if(camera_control_s.tp_keep_time == 0)
        {
            camera_control_s.photo_ok = 0;
            camera_control_s.ready = 0x01;
            //camera_control_s.ready_keep_time = 5;
        }
    }	
     
    if(camera_control_s.photo_ok == 0x01)
    { 
        /* 拍照OK GUI显示 */
        camera_control_s.photo_ok = 0x00;
        camera_control_s.ready = 0x01;
        Gui_ThreadEventTaskWakeUp(CPU_EVENT_TASK_CAMERAOK);
    }
    else
    {
        /* 拍照准备GUI显示 */
        Gui_ThreadEventTaskWakeUp(CPU_EVENT_TASK_CAMERAREADY);
    }
    
    if(camera_control_s.ready_keep_time == 0 || system_flag_s.power_state != POWER_NORMAL)
    {
        //60s超时退出拍照模式
        system_flag_s.camera_mode = 0;
        if(Gui_GetThreadJob() == CPU_EVENT_TASK_CAMERAREADY 
          || Gui_GetThreadJob() == CPU_EVENT_TASK_CAMERAOK )
        {
            Gui_ThreadEventTaskWakeUp(CPU_EVENT_TASK_ALL_EXIT); 
	        //退出拍照模式
	        Camera_BraceletExitSendData();
        }
    }
}

/****************************************************************
** Function name:           Camera_GetState
** Descriptions:            获取照相控制参数结构体
** input parameters:        无
** output parameters:       无
** Returned value:          返回结构体指针
** Created by:              WJ             
** Created Date:            2017-08-21
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
camera_control_t *Camera_GetState(void)
{
    return &camera_control_s;
}
