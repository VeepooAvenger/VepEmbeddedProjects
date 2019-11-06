#ifndef _CPU_TASK_DATA_TYPE_H_
#define _CPU_TASK_DATA_TYPE_H_

#include <stdint.h>
#include "flash_data_type.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

typedef enum
{
    COMMUNICATION_THREAD_PRIORITY = 1,
    OXYGEN_ALGORITHM_THREAD_PRIORITY = 2,
    BLE_STACK_THREAD_PRIORITY = 3,
    GUI_THREAD_PRIORITY = 5,
}THREAD_PRIORITY;

typedef enum
{
    BUTTON_EVENT_NULL  = 0,
    BUTTON_EVENT_CLICK,
    BUTTON_EVENT_LONGPRESS,
}BUTTON_EVENT_TYPE;

// 任务枚举型定义
typedef enum
{
    CPU_TASK_IDEL = 0,        //任务--空闲
    CPU_TASK_MAIN,            //任务--主界面
    CPU_TASK_CHOOSE_MAIN,     //任务--选择主界面风格
    CPU_TASK_MAC,			  //任务--MAC和版本号
    CPU_TASK_MOVEMENT,        //任务--运动模式
    CPU_TASK_MOVEMENT_SELECT, //任务--多运动选择界面
    CPU_TASK_MOVEMENT_DATA,   //任务--数据界面
    CPU_TASK_MOVEMENT_RESULT, //任务--多运动结束界面
    CPU_TASK_STEP,            //任务--记步、卡路里、距离集合
	//CPU_TASK_DISTAN,          //任务--距离
	//CPU_TASK_CALOR,           //任务--卡路里
    CPU_TASK_HEART,           //任务--心率
    CPU_TASK_SLEEP,           //任务--睡眠
    CPU_TASK_FINDPHONE,       //任务--查找手机
    CPU_TASK_TIMER,           //任务--秒表功能
    CPU_TASK_HOURGLASS,       //任务--倒计时功能

    CPU_TASK_ECG,             //任务--ECG模式测量    
    CPU_TASK_ECG_ITEM1,       //任务--ECG模式测量-子菜单1
    CPU_TASK_ECG_ITEM2,       //任务--ECG模式测量-子菜单2
    
    //CPU_TASK_PTT,             //任务--PTT模式测量
    //CPU_TASK_PTT_ITEM1,       //任务--PTT模式测量-子菜单1

    CPU_TASK_BP,              //任务--血压测量    
    CPU_TASK_OXYGEN,          //任务--血氧测量
    CPU_TASK_POWEROFF,        //任务--关机
    
    CPU_TASK_FUNCT,           //任务--更多
    CPU_TASK_FUNCT2,          //任务--更多
    CPU_TASK_SWITCH,          //任务--开关设置界面
    CPU_TASK_MESSAGE,         //任务--信息界面
    CPU_TASK_DISP_MESSAGE,    //任务--显示信息界面
    CPU_TASK_DISP_MESSAGE2,    //任务--显示信息界面2
    CPU_TASK_LIGHTLEVEL,      //任务--亮度调节界面
    CPU_TASK_LIGHTTIME,       //任务--亮屏时长界面
    CPU_TASK_CLEARDATA,       //任务--清除数据界面
    CPU_TASK_MESSAGE_SWITCH,  //任务--消息开关设置
    
    CPU_TASK_TASK_MAX_ID
    
}CPU_TASK_TYPE;

// 事件模式枚举型定义
typedef enum
{
    CPU_EVENT_TASK_IDEL = 0,          // 事件任务--空闲
    CPU_EVENT_TASK_ALL_EXIT,          // 事件任务--退出所有任务
    CPU_EVENT_TASK_TURNWRIST_START,   // 事件任务--翻腕亮屏
    CPU_EVENT_TASK_TURNWRIST_STOP,    // 事件任务--翻腕停止
    CPU_EVENT_TASK_CHARGE,            // 事件任务--充电
    CPU_EVENT_TASK_ALARM,             // 事件任务--闹钟
    CPU_EVENT_TASK_SIT,               // 事件任务--久坐
    CPU_EVENT_TASK_CALL,              // 事件任务--消息
    //CPU_EVENT_TASK_PHYS_REMIND,       // 事件任务--女性功能提醒
    CPU_EVENT_TASK_MESSAGE,           // 事件任务--消息
    
    CPU_EVENT_TASK_LOW_SPO2,    // 事件任务--低氧报警
    CPU_EVENT_TASK_HEARTALERT,        // 事件任务--心率报警
    
    CPU_EVENT_TASK_CAMERA,            // 事件任务--拍照任务
    CPU_EVENT_TASK_FONTTEST,          // 事件任务--显示“维亿魄”
    CPU_EVENT_TASK_LOWPOWER,          // 事件任务--低电提醒
    CPU_EVENT_TASK_SPORT_TARGET,      // 事件任务--运动目标达到
    CPU_EVENT_TASK_BLE_DISC_REMIND,   // 事件任务--蓝牙断连提醒
    CPU_EVENT_TASK_HOURGLASS_HINT,    // 事件任务--倒计时APP设置提醒
    CPU_EVENT_TASK_HOURGLASS_TIMEOUT, // 事件任务--倒计时时间到了
    CPU_EVENT_TASK_POWERON,           // 事件任务--开机
    CPU_EVENT_TASK_POWEROFF,          // 事件任务--关机
    
}CPU_EVENT_TYPE;

// 秒表功能定时器
typedef struct
{
    uint8_t   enable:1;         // 秒表使能计时
    uint8_t   entry:1;          // 秒表进入状态(秒表图标界面)
    uint8_t   run:1;            // 运行状态(时间计数界面)
    uint8_t   resv:5;           // 预留对齐
    uint32_t  total_msecond;    // 秒表计时
    uint32_t  stop_msecond;     // 运行毫秒时间
}watch_timer_t;

//运动模式参数
typedef struct
{
    uint8_t   enable:1;         // 运动模式使能计时
    uint8_t   entry:1;          // 进入运动模式
    uint8_t   run:1;            // 运行状态
    uint8_t   face_num:2;       // 第几个界面
    uint8_t   resv:3;           // 预留对齐
}movement_cont_t;

////清屏参数
//typedef struct
//{
//    uint8_t start_column;       //开始列
//    uint8_t start_page;         //开始页(行)
//    uint8_t column_size;        //总共有多少列
//    uint8_t page_size;          //总共有多少页(行)
//}gui_cls_param_t;

//OLED屏驱动参数
typedef struct
{
    uint8_t   bright:1;    // OLED屏亮度调节
    uint8_t   light:1;     // OLED屏点亮
    uint8_t   status:1;    // OLED亮屏状态
    uint8_t   repaint:1;   // oled刷新屏
    uint8_t   resv:4;      // 对齐保留
}oled_driver_t;

// 马达驱动参数
typedef struct
{
    uint8_t   second_conter;            // 秒计数器	
}motor_driver_t;

//静态页面控制器
typedef struct
{
    uint8_t  animation_enable:1;         // 动画使能
    uint8_t  repaint_enable:1;           // 页面使能
    uint8_t  resv:6;                     // 动画最大包数

    uint16_t  page_frame_counter;         // oled页面刷新计数器
    uint16_t  animation_frame_index;      // 动画帧序号
    uint16_t  animation_tick_counter;     // 动画tick计数器(tick基准时间为50ms) 
}gui_controller_t;

//CPU任务控制器
typedef struct
{
    uint8_t  active:1;              // 任务激活
    uint8_t  init:1;                // 初始化
    uint8_t  key_press_hold:1;      // 按键保持屏幕亮屏
    uint8_t  resv:5;                // 保留对齐
    
    uint16_t run_time;              // 任务运行时间
    uint16_t keep_time;             // 任务保持时间
}task_controller_t;

// CPU任务控制块结构体
typedef struct
{
    CPU_TASK_TYPE       task_id;            // 常规任务ID
    uint8_t             child_task_id;      // 子类ID
    task_controller_t   task_controller;
    gui_controller_t    gui_controller;	
}cpu_task_tcb_t;

// CPU任务控制块结构体
typedef struct
{
    CPU_EVENT_TYPE      event_id;           // 事件任务ID  
    uint8_t             child_event_id;     // 子类ID
    task_controller_t   task_controller;    // 任务控制参数
    gui_controller_t    gui_controller;	    // GUI控制参数
}cpu_event_tcb_t;

// CPU设备控制块结构体
typedef struct
{
    BUTTON_EVENT_TYPE   key;
    oled_driver_t       oled;
    motor_driver_t      motor;
    uint8_t             cls_flag;        //需要清屏的    //0;不用清屏  1:清全屏 2:清小屏
//	gui_cls_param_t     cls_param;       // 清屏参数	
}cpu_device_tcb_t;

#define GET_TASK_PID(task_tcb,event_tcb) \
    (uint32_t)((((uint8_t)(task_tcb.task_id))<<24)|(((uint8_t)(task_tcb.child_task_id))<<16)|(((uint8_t)(event_tcb.event_id))<<8)|(event_tcb.child_event_id))

/*
 *  宏名称:SEMAPHORE_CREATE_RET(sem)
 *  一个信号量占用76个Byte
 */
#define SEMAPHORE_CREATE_BINARY_RET(sem) \
{ \
    sem = xSemaphoreCreateBinary(); \
    if (sem == NULL) \
    { \
        APP_ERROR_HANDLER(NRF_ERROR_NO_MEM); \
    } \
}

/*
 *  宏名称:THREAD_MUTEX_TASK_EXCUTE_BREAK(task_sem,count_sem,task)
 *         task_sem--任务信号量
 *         count_sem--任务计数信号量
 *         task--任务函数
*   功 能: 线程互斥任务执行
 */
#define THREAD_MUTEX_TASK_EXCUTE(task_sem,count_sem,task) \
{ \
    if(pdTRUE == xSemaphoreTake(task_sem,(TickType_t)0)) \
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
*   功 能: 线程任务唤醒
 */
#define THREAD_TASK_WAKE_UP(task_sem,count_sem,thread_handle) \
{ \
    xSemaphoreGive(task_sem); \
    xSemaphoreGive(count_sem); \
    if(eRunning != eTaskGetState(thread_handle)) \
    { \
        vTaskResume(thread_handle); \
    } \
}


#endif
