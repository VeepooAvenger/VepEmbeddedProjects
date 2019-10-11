/***************************Copyright(c)***********************************
**                       Veepoo Tech
**                  COPYRIGHT (C) 2016-2019
**-----------File Info-----------------------------------------------------
** File        name:     cpu_common.h
** Last modified Date:   2017-08-01
** Last SYS_VERSION:     0.0.0
** Descriptions:            
**-------------------------------------------------------------------------
** Created  by:          WJ
** Created  date:        2017-08-01
** SYS_VERSION:          0.0.0
** Descriptions:         
**-------------------------------------------------------------------------
** Modified by:				
** Modified date:  			          
** SYS_VERSION:                 
** Descriptions:                        
***************************************************************************/
#ifndef _CPU_COMMON_H_
#define _CPU_COMMON_H_

#include <stdint.h>
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

// 通知消息类型
typedef enum
{
	NOTIFY_MSG_EVENT_CALL = 1,
	NOTIFY_MSG_EVENT_SMS,
	NOTIFY_MSG_EVENT_WECHAT,
	NOTIFY_MSG_EVENT_QQ,
	NOTIFY_MSG_EVENT_SINA,
	NOTIFY_MSG_EVENT_FACEBOOK,
	NOTIFY_MSG_EVENT_TWITTER,
	NOTIFY_MSG_EVENT_FLICKR,
	NOTIFY_MSG_EVENT_LINKEDLN,
	NOTIFY_MSG_EVENT_WHATSAPP,
	NOTIFY_MSG_EVENT_LINE,
	NOTIFY_MSG_EVENT_INSTAGRAM,
	NOTIFY_MSG_EVENT_SNAPCHAT,
	NOTIFY_MSG_EVENT_SKYPE,
	NOTIFY_MSG_EVENT_GMAIL,
	NOTIFY_MSG_EVENT_OTHER,
}NOTIFY_MSG_EVENT;

typedef enum
{
	BUTTON_EVENT_NONE = 1,
	BUTTON_EVENT_CLICK,
	BUTTON_EVENT_LONGPRESS,
    BUTTON_EVENT_LONGPRESS_6S,
}BUTTON_EVENT_TYPE;


// 任务枚举型定义
typedef enum
{
	CPU_TASK_IDEL = 0,  	  //任务--空闲
	CPU_TASK_MAIN,            //任务--时间
    CPU_TASK_MAIN1,            //任务--时间
    CPU_TASK_PHY,
	CPU_TASK_MOVEMENT,        //任务--运动模式
	CPU_TASK_STEP,            //任务--记步 
	CPU_TASK_DISTANCE,        //任务--距离
	CPU_TASK_CALORIES,        //任务--卡路里
//    CPU_TASK_BP,              //任务--血压
	CPU_TASK_HEART,           //任务--心率
	CPU_TASK_SLEEP,           //任务--睡眠
//	CPU_TASK_FINDPHONE,       //任务--查找手机
	CPU_TASK_STOP_WATCH,      //任务--秒表功能
//	CPU_TASK_COUNT_DOWN,      //任务--倒计时功能
	CPU_TASK_POWEROFF,        //任务--关机
    
	CPU_TASK_MOVEMENT_PAUSE,  //任务--运动模式暂停   
	CPU_TASK_MOVEMENT_EXIT,   //任务--运动模式退出
    


    
}CPU_TASK_TYPE;

// 事件模式枚举型定义
typedef enum
{
	CPU_EVENT_TASK_IDEL = 0,          // 事件任务--空闲
	CPU_EVENT_TASK_ALL_EXIT,          // 事件任务--退出所有任务
	CPU_EVENT_TASK_CHARGE,            // 事件任务--充电
	CPU_EVENT_TASK_ALARM,             // 事件任务--闹钟
	CPU_EVENT_TASK_SIT,               // 事件任务--久坐
	CPU_EVENT_TASK_CALL_REMIND,       // 事件任务--消息
	CPU_EVENT_TASK_MESSAGE_REMIND,    // 事件任务--消息
	CPU_EVENT_TASK_HEARTALERT,        // 事件任务--心率报警
	CPU_EVENT_TASK_CAMERAREADY,       // 事件任务--拍照准备就绪
	CPU_EVENT_TASK_CAMERAOK,          // 事件任务--拍照准备
	CPU_EVENT_TASK_FONTTEST,          // 事件任务--显示“维亿魄”
	CPU_EVENT_TASK_LOWPOWER,          // 事件任务--低电提醒
	CPU_EVENT_TASK_COUNT_DOWN_TIMEOUT,// 事件任务--倒计时时间到了
	CPU_EVENT_TASK_COUNT_DOWN_START,  // 事件任务--倒计时开始
	CPU_EVENT_TASK_STARGET,           // 事件任务--运动目标提醒
    CPU_EVENT_TASK_CARTOON,           // 事件任务--开关机动画，充电动画
    CPU_EVENT_TASK_CARTOON1,
    CPU_EVENT_TASK_MAC_VER,           // 事件任务--显示MAC和VER
	CPU_EVENT_TASK_PHYS_REMIND,       // 事件任务--女性功能提醒
}CPU_EVENT_TYPE;

#pragma pack(1)


//OLED屏驱动参数
typedef struct
{
	uint8_t oled_drive_bright:1;   // OLED屏亮度调节
	uint8_t oled_drive_light:1;    // OLED屏点亮
	uint8_t oled_drive_off:1;      // OLED屏熄灭
	uint8_t oled_light_status:1;   // OLED亮屏状态
	
	uint8_t resv:4;                //
}oled_driver_t;

//静态页面控制器
typedef struct
{
	uint8_t animation_enable:1;      //动画使能
	uint8_t repaint_enable:1;        //页面使能
//	uint8_t repaint:1;               //刷新
//	uint8_t animation_max_frame:5;   //动画最大包数
//    
	uint8_t page_frame_counter;       //oled页面刷新计数器
	uint8_t animation_frame_counter;  //动画帧数计数器    

    
    uint8_t clear_background;      //清除背景        
    uint8_t clear_background_must; //清除背景
    uint8_t start_page;            //开始页(行)
    uint8_t page_size;             //总共有多少页(行)
    uint8_t start_column;          //开始列
    uint8_t column_size;           //总共有多少列
        
    uint16_t updata_ui_interval;   //UI 刷新间隔
}gui_controller_t;

//CPU任务控制器
typedef struct
{
	uint8_t active:1;              //任务激活
	uint8_t init:1;                //初始化
	uint8_t key_press_hold:1;      //按键保持屏幕亮屏
	uint8_t resv:5;
	
	uint8_t run_time;              //任务运行时间
	uint8_t keep_time;             //任务保持时间
    
    uint8_t priority;              //任务的优先级
}cpu_task_controller_t;

#pragma pack()

/*
 * 宏名称:JOB_TASK_ENTRY_INIT
 * 描  述: 
 *   init--任务初始化完成后清0
 *   active--表示任务激活
 *   key_press_hold--表示长按键屏幕是否亮屏
 *   LDO_user_off--表示任务启动时,需要关掉LDO灯
 *   keep_time--表示任务保持时间
 *   run_time--表示任务保持时间
 *
 */
#define TASK_ENTRY_INIT(p_controller, hold_time) \
{ \
	p_controller->init = 0; \
	p_controller->active = 1; \
	p_controller->keep_time = hold_time; \
	p_controller->run_time = 0; \
    p_controller->key_press_hold = 0; \
}

/*
 *  宏名称:SEMAPHORE_CREATE_RET(sem)
 */
#define SEMAPHORE_CREATE_BINARY_RET(sem) \
{ \
	sem = xSemaphoreCreateBinary(); \
	if (NULL == sem) \
        APP_ERROR_HANDLER(NRF_ERROR_NO_MEM); \
}

/*
 *  宏名称:THREAD_MUTEX_TASK_EXCUTE_BREAK(task_sem,count_sem,task)
 *         task_sem--任务信号量
 *         count_sem--任务计数信号量
 *         task--任务函数
 *  功 能: 线程互斥任务执行
 */
#define THREAD_MUTEX_TASK_EXCUTE(task_sem,count_sem,task) \
{ \
	if(pdTRUE == xSemaphoreTake(task_sem,(TickType_t)10)) \
	{  \
		xSemaphoreTake(count_sem,(TickType_t)0); \
		task(); \
	} \
}

/*
 *  宏名称:THREAD_TASK_WAKE_UP(task_sem,count_sem,thread_handle)
 *         task_sem--任务信号量
 *         count_sem--任务计数信号量
 *         thread_handle--线程句柄
 *  功 能: 线程任务唤醒
 */
#define THREAD_TASK_WAKE_UP(task_sem,count_sem,thread_handle) \
{ \
/*	if(1 == p_data_manage->sign_off)  return; */\
	                                          \
	xSemaphoreGive(task_sem); \
	xSemaphoreGive(count_sem); \
	if(eRunning != eTaskGetState(thread_handle)) \
	{ \
		vTaskResume(thread_handle); \
	} \
}

#define MEASURE_IDEL_STATUS() \
{  \
	*(uint8_t*)p_measure_opt == 0; \
}

#endif
