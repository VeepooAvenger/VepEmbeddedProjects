/***************************Copyright(c)***********************************
**                       Veepoo Tech
**                  COPYRIGHT (C) 2016-2019
**-----------File Info-----------------------------------------------------
** File        name:     gui_thread.c
** Last modified Date:   2017-07-31
** Last SYS_VERSION:     0.0.0
** Descriptions:            
**-------------------------------------------------------------------------
** Created  by:          WJ
** Created  date:        2017-07-31
** SYS_VERSION:          0.0.0
** Descriptions:         
**-------------------------------------------------------------------------
** Modified by:				
** Modified date:  			          
** SYS_VERSION:                 
** Descriptions:                        
***************************************************************************/

#include <string.h>
#include "gui_thread.h"
#include "cpu_common.h"
#include "system_clock.h"
#include "cpu_general_task.h"
#include "cpu_event_task.h"
#include "gui_page_display.h"
#include "app_type.h"
#include "motor_event.h"
#include "gui_data_inteface.h"
#include "gui_traverse.h"

cpu_task_t cpu_s;
gui_thread_resource_t  gui_res_s;
static uint8_t stop_watch_frist_flag = 0;                           //用于记录秒表就绪后第一次亮屏
uint8_t display_language = GUI_LANGUAGE_ENAGLISH;                   //UI显示语言,默认英文
uint8_t main_background_color = 0;                                  //界面风格

static uint8_t gui_movement_change_page = 0;                        //运动模式下灭屏操作
uint8_t delay_time_cnt = 0;
uint8_t ui_change_flag = 0;                      //运动模式下灭屏操作

static cpu_button_t cpu_button_s = {0,0};

uint8_t thread_handle_run = 0;                  //线程运行状态

static uint8_t charging_state = 0;               //充电图标延后显示

uint8_t Threadflag = 0;


uint8_t disconnet_keep_flg = 0;
uint8_t disconnet_motor_flg = 0;

static uint8_t posture_to_bright_flg = 0;      //姿态亮屏的标志

extern system_flag_t system_flag_s;            //系统标志
//----声明函数----
static void Gui_ThreadHandle(void* arg);
static void Gui_ThreadLcdDriveDeal(void);
static void Gui_ThreadTaskMonitor(void);
static void Gui_ThreadTaskManage(void);
static void Gui_ThreadEventManage(void);
static uint8_t Gui_ThreadIsReadingNotify(uint8_t event);
static void Gui_ThreadDealKeyEvent(void);
static void Gui_ThreadTaskSwitching(void);

/****************************************************************
** Function name:           Gui_CreateResource
** Descriptions:            创建UI处理线程
** input parameters:        无
** output parameters:       无 
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-08-02
*****************************************************************/
static __inline void Gui_CreateResource(void)
{	
	SEMAPHORE_CREATE_BINARY_RET(gui_res_s.m_repaint_sem);
	
	// 创建线程
	if(pdPASS != xTaskCreate(Gui_ThreadHandle, "Gui", 512, NULL, GUI_THREAD_PRIORITY, &gui_res_s.m_thread_handle))  
	{
		APP_ERROR_HANDLER(NRF_ERROR_NO_MEM);
	}    
}

/****************************************************************
** Function name:           Gui_ThreadInit
** Descriptions:            UI处理线程注册和UI处理参数初始化
** input parameters:        无
** output parameters:       无 
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-08-02
*****************************************************************/
void Gui_ThreadInit(void)
{
	memset(&cpu_s, 0, sizeof(cpu_task_t));
	
	cpu_task_controller_t *p_task = &cpu_s.job_control;
	//配置主界面参数
    
    TASK_ENTRY_INIT(p_task, 3);
	cpu_s.event   = CPU_EVENT_TASK_IDEL;
	cpu_s.task    = CPU_TASK_MAIN;
	cpu_s.job_control.init = 1;	
    cpu_s.gui.clear_background = 1;
	cpu_s.oled.oled_drive_light = 1;
    cpu_s.gui.updata_ui_interval = 500;   //500ms
    Gui_ThreadSetDisPlayArea(0, 0, 80, 160);
    delay_time_cnt = 0;
    ui_change_flag = 1;
    
	Gui_CreateResource();
}

/****************************************************************
** Function name:           Gui_ThreadWakeUpThread
** Descriptions:            唤醒UI线程
** input parameters:        无
** output parameters:       无 
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-08-02
*****************************************************************/
static __inline void Gui_ThreadWakeUpThread(void)
{    
    xSemaphoreGive(gui_res_s.m_repaint_sem);
    xSemaphoreGive(gui_res_s.m_Task_count);
    vTaskResume(gui_res_s.m_thread_handle);      
    delay_time_cnt = 0;
    ui_change_flag = 1;   
}

/****************************************************************
** Function name:           Gui_ThreadPageActive
** Descriptions:            是否处于UI显示事件中
** input parameters:        无
** output parameters:       无 
** Returned value:          1:处于运动界面 0：不处于运动界面
** Created by:              WJ              
** Created Date:            2017-08-02
*****************************************************************/
bool Gui_ThreadPageActive(void)
{
	return (cpu_s.task != CPU_TASK_IDEL) || (cpu_s.event > CPU_EVENT_TASK_ALL_EXIT);
}

/****************************************************************
** Function name:           Gui_ThreadTaskPageActive
** Descriptions:            是否处于UI显示事件中
** input parameters:        无
** output parameters:       无 
** Returned value:          1:处于运动界面 0：不处于运动界面
** Created by:              WJ              
** Created Date:            2017-08-02
*****************************************************************/
bool Gui_ThreadTaskPageActive(void)
{
	return (cpu_s.task != CPU_TASK_IDEL);
}

/****************************************************************
** Function name:           Gui_ThreadIsTaskRun
** Descriptions:            是否处于运动界面
** input parameters:        无
** output parameters:       无 
** Returned value:          1:处于运动界面 0：不处于运动界面
** Created by:              WJ              
** Created Date:            2017-08-02
*****************************************************************/
bool Gui_ThreadIsTaskRun(void)
{
	return (cpu_s.task == CPU_TASK_MOVEMENT);
}

/****************************************************************
** Function name:           Gui_ThreadRepaintTaskWakeUp
** Descriptions:            唤醒UI线程
** input parameters:        无
** output parameters:       无 
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-08-02
*****************************************************************/
void Gui_ThreadRepaintTaskWakeUp(void)
{
	if(eRunning != eTaskGetState(gui_res_s.m_thread_handle))
	{
		vTaskResume(gui_res_s.m_thread_handle);
	}
}

/****************************************************************
** Function name:           Gui_ThreadBrightAdjustTaskWakeUp
** Descriptions:            屏幕亮度调节显示
** input parameters:        无
** output parameters:       无 
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-08-02
*****************************************************************/
void Gui_ThreadBrightAdjustTaskWakeUp(void)
{
	if(!Gui_ThreadPageActive() || cpu_s.task == CPU_TASK_MAIN)
	{
		cpu_s.task = CPU_TASK_MAIN;
		cpu_s.job_control.init = 1;   	
        Gui_ThreadWakeUpThread();
	}

	cpu_s.oled.oled_drive_bright = 1;
    cpu_s.oled.oled_drive_light = 1;
}

/****************************************************************
** Function name:           Gui_ThreadHandle
** Descriptions:            UI处理线程
** input parameters:        无
** output parameters:       无 
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-08-02
*****************************************************************/
static void Gui_ThreadHandle(void* arg)
{	
    static uint8_t gui_thread_hang = 0;
    
	UNUSED_PARAMETER(arg);
       
    //Gui_SetTraverseUiState(1);
    //Gui_ThreadEventTaskWakeUp(CPU_EVENT_TASK_BLE_DISC_REMIND);
	while(1)
	{	     
        thread_handle_run = 1;
        //WJ.20180414.优化按键处理方式
        Gui_ThreadDealKeyEvent();
        
        if(posture_to_bright_flg == 1)
        {
            posture_to_bright_flg = 0;
            Gui_ThreadTaskSwitching();
        }
        if(delay_time_cnt == 0 || ui_change_flag == 1)
        {
            //WJ.20180303.
            if(cpu_s.oled.oled_drive_light == 1 && cpu_s.oled.oled_light_status == 0)
            {
                //唤醒LCD,加快LCD自身处理速度，目前发现32M速率下不加会有问题
                Gui_LcdWakeUp();
            }
            
            ui_change_flag = 0; 
            if(Gui_DataGetEncrypt())
            {
                if(Gui_GetTraverseUiState() == 0)
                {
                    display_language = Gui_DataGetSystemLanguage();
                    main_background_color = Gui_DataGetUiStyle();
                    
                    
                    //响应通用模式任务
                    Gui_ThreadTaskManage();
                    
                    //相应外部事件
                    Gui_ThreadEventManage();
                }
                else
                {
                    Gui_ThreadTestManage();
                }
            }
            else
            {
                //显示XXX
                Gui_PageDisXxxIcon();
            }
            
            //任务时间控制
            Gui_ThreadTaskMonitor();
                   
            //LCD操作：关屏和开屏，
            Gui_ThreadLcdDriveDeal();
        }
        
		if(cpu_s.oled.oled_light_status == 1 || gui_thread_hang < 2) 
        {
            if(cpu_s.oled.oled_light_status == 0)
            {
                gui_thread_hang++;
            }
            else
            { 
                gui_thread_hang = 0;
            }			
            vTaskDelay(50);    
            delay_time_cnt++;
            if(delay_time_cnt*50 >= cpu_s.gui.updata_ui_interval)
            {
                delay_time_cnt = 0;
            }          
        }
		else
        {
            gui_thread_hang = 0;
            thread_handle_run = 0;
			vTaskSuspend(gui_res_s.m_thread_handle);
            delay_time_cnt = 0;
        }
	}
}

/****************************************************************
** Function name:           Gui_ThreadTaskManage
** Descriptions:            主任务事件界面处理
** input parameters:        无
** output parameters:       无 
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-08-02
*****************************************************************/ 
static void Gui_ThreadTaskManage(void)
{
    static uint8_t clear_background_flag = 0;
    
	//如果有外部事件
	if(cpu_s.event != CPU_EVENT_TASK_IDEL)
    {
        clear_background_flag = 1;
        return;
    }

    if((clear_background_flag == 1 && cpu_s.job_control.init != 0x01)
      || cpu_s.gui.clear_background_must == 1)
    {       
		Gui_PageClearBackground(cpu_s.gui.start_column, cpu_s.gui.start_page, cpu_s.gui.column_size, cpu_s.gui.page_size);
        clear_background_flag = 0;
        cpu_s.gui.clear_background = 0;
        cpu_s.gui.page_frame_counter = 0;
        cpu_s.gui.clear_background_must = 0;
        cpu_s.gui.page_frame_counter = 0;
        Cpu_TaskClearIntervalCnt();
        
    }
    clear_background_flag = 0;
    
	switch(cpu_s.task)
	{
		case CPU_TASK_MAIN:     
            //主界面
			Cpu_TaskMain(&cpu_s.job_control, &cpu_s.gui);
			break;
		case CPU_TASK_STEP:     
            //计步
			Cpu_TaskStep(&cpu_s.job_control, &cpu_s.gui);
			break;
        case CPU_TASK_DISTANCE:
            //距离
			Cpu_TaskDistance(&cpu_s.job_control, &cpu_s.gui);
            break;
        case CPU_TASK_MOVEMENT:
            //运动模式
			Cpu_TaskRun(&cpu_s.job_control, &cpu_s.gui);
            break;
        case CPU_TASK_CALORIES:   
            //卡路里
			Cpu_TaskCalories(&cpu_s.job_control, &cpu_s.gui);         
            break;
//        case CPU_TASK_BP:       
//            //血压
//            Cpu_TaskBP(&cpu_s.job_control, &cpu_s.gui);
//            break;
		case CPU_TASK_HEART:    
            //心率
			Cpu_TaskHeart(&cpu_s.job_control, &cpu_s.gui);
			break;
		case CPU_TASK_SLEEP:    
            //睡眠
			Cpu_TaskSleep(&cpu_s.job_control, &cpu_s.gui);
			break;
//		case CPU_TASK_FINDPHONE:     
//            //查找手机
//			Cpu_TaskFindPhone(&cpu_s.job_control, &cpu_s.gui);
//			break;
		case CPU_TASK_STOP_WATCH:    
            //秒表
			Cpu_TaskStopWatch(&cpu_s.job_control, &cpu_s.gui);
			break;
//		case CPU_TASK_COUNT_DOWN:    
//            //倒计时
//			Gui_TaskCountDown(&cpu_s.job_control, &cpu_s.gui);
//			break;
		case CPU_TASK_POWEROFF:                  
            //关机
			Cpu_TaskPowerOff(&cpu_s.job_control, &cpu_s.gui);
			break;
              
//        case CPU_TASK_HRV:
//            Cpu_HrvTask(&cpu_s.job_control, &cpu_s.gui);
//            break;
        
//        case CPU_TASK_POWERON:
//            Cpu_PowerOnTask(&cpu_s.job_control, &cpu_s.gui);
//            break;
        
		
        case CPU_TASK_MOVEMENT_PAUSE:   
            // 运动模式暂停
            Cpu_TaskRunModePause(&cpu_s.job_control, &cpu_s.gui);
            break;  
   
        case CPU_TASK_MOVEMENT_EXIT: 
            // 运动模式退出
            Cpu_TaskRunModeExit(&cpu_s.job_control, &cpu_s.gui);
            break; 
        
        case CPU_TASK_PHY: 
            // 女性
            Cpu_PHY(&cpu_s.job_control, &cpu_s.gui);
            break; 
        case CPU_TASK_MAIN1:     
            //
			Cpu_TaskMain1(&cpu_s.job_control, &cpu_s.gui);
			break;
        
		default:
			cpu_s.task = CPU_TASK_IDEL;
			break;
	}
}

/****************************************************************
** Function name:           Gui_ThreadCpuExitAllTask
** Descriptions:            外部事件界面处理退出
** input parameters:        无
** output parameters:       无 
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-08-02
*****************************************************************/ 
static void Gui_ThreadCpuExitAllTask(void)
{
	//控制任务参数
	cpu_s.event_control.active = 1;
	cpu_s.event_control.keep_time = 0;
	cpu_s.event_control.run_time = 1;
}

/****************************************************************
** Function name:           Gui_ThreadEventManage
** Descriptions:            外部事件界面处理
** input parameters:        无
** output parameters:       无 
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-08-02
*****************************************************************/ 
static void Gui_ThreadEventManage(void)
{
	switch(cpu_s.event)
	{
		case CPU_EVENT_TASK_ALL_EXIT:
			Gui_ThreadCpuExitAllTask();
			break;
		
		case CPU_EVENT_TASK_CHARGE:
            //充电
			Cpu_EventIdelCharge(&cpu_s.event_control, &cpu_s.gui);
			break;
        
		case CPU_EVENT_TASK_ALARM:
            //闹钟
			Cpu_EventAlarm(&cpu_s.event_control, &cpu_s.gui);
			break;
		
		case CPU_EVENT_TASK_SIT:
            //久坐
			Cpu_EventSedentary(&cpu_s.event_control, &cpu_s.gui);
			break;
        
		case CPU_EVENT_TASK_CALL_REMIND:
            //来电
			Cpu_EventCallRemind(&cpu_s.event_control, &cpu_s.gui);
            break;
        
		case CPU_EVENT_TASK_MESSAGE_REMIND:    
            //信息
			Cpu_EventMessageRemind(&cpu_s.event_control, &cpu_s.gui);
			break;
		
//		case CPU_EVENT_TASK_HEARTALERT: 
//            //心率
//			Cpu_EventHeartAlert(&cpu_s.event_control, &cpu_s.gui);
//			break;
		
		case CPU_EVENT_TASK_CAMERAREADY:    
            //拍照
			Cpu_EventCameraReady(&cpu_s.event_control, &cpu_s.gui);
			break;
		
		case CPU_EVENT_TASK_CAMERAOK:       
            //拍照完成
			Cpu_EventCameraOK(&cpu_s.event_control, &cpu_s.gui);
			break;
		 
		case CPU_EVENT_TASK_FONTTEST:       
            //测试
			Cpu_EventFontTest(&cpu_s.event_control, &cpu_s.gui);
			break;
		
		case CPU_EVENT_TASK_LOWPOWER:      
            //低电
			Cpu_EventLowPowerHint(&cpu_s.event_control, &cpu_s.gui);
			break;
		
//		case CPU_EVENT_TASK_COUNT_DOWN_TIMEOUT:  
//            //倒计时
//			Cpu_EvenCountDownTimeOut(&cpu_s.event_control, &cpu_s.gui);
//			break;
			
//        case CPU_EVENT_TASK_COUNT_DOWN_START:            
//			Cpu_EvenCountDownTimeStart(&cpu_s.event_control, &cpu_s.gui);
//            break;
        
        case CPU_EVENT_TASK_STARGET:
            Cpu_EvenStarget(&cpu_s.event_control, &cpu_s.gui);
            break;
        
        case CPU_EVENT_TASK_MAC_VER:
            //显示mac和ver
            Cpu_EvenDisMacVer(&cpu_s.event_control, &cpu_s.gui);
            break;
        case CPU_EVENT_TASK_PHYS_REMIND:
            //女性
            Cpu_EvenPhysRemind(&cpu_s.event_control, &cpu_s.gui);
            break;
        case CPU_EVENT_TASK_BLE_DISC_REMIND:
            Cpu_EvenBleDiscRemind(&cpu_s.event_control, &cpu_s.gui);
            break;
            //开机
        case CPU_EVENT_TASK_CARTOON:
            Cpu_EvenCartoon(&cpu_s.event_control, &cpu_s.gui);
            break;
            //关机
        case CPU_EVENT_TASK_CARTOON1:
            Cpu_EvenCartoon1(&cpu_s.event_control, &cpu_s.gui);
            break;
		default:
			cpu_s.event = CPU_EVENT_TASK_IDEL;
			break;
	}
}

/****************************************************************
** Function name:           Gui_ThreadLcdDriveDeal
** Descriptions:            屏幕状态控制
** input parameters:        无
** output parameters:       无 
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-08-02
*****************************************************************/ 
static void Gui_ThreadLcdDriveDeal(void)
{
    //防止中间插入其他事件导致黑屏   
    if(cpu_s.oled.oled_drive_off == 1 && cpu_s.oled.oled_drive_light != 1)
	{
        cpu_s.oled.oled_drive_off = 0;
        if(cpu_s.oled.oled_light_status == 1)
        {
            //这里将屏灭掉
            cpu_s.oled.oled_drive_off = 0;
            cpu_s.oled.oled_light_status = 0;
            //WJ.20180205.增加先关闭背光处理
            Gui_LcdLedOff();
            Gui_LcdDriverInit();
            Gui_LcdDriverOff();
        }
	}
    
	if(cpu_s.oled.oled_drive_light == 1)
	{
		//这里将屏点亮
		cpu_s.oled.oled_drive_light = 0;
		if(cpu_s.oled.oled_light_status == 0)
		{
			cpu_s.oled.oled_light_status = 1;
            Gui_LcdDriverOn();
            
            if(cpu_s.event == CPU_EVENT_TASK_IDEL && cpu_s.task == CPU_TASK_IDEL)
            {
                return;
            }
			Gui_LcdLedOn();
		}
	}
	
	// 这里是亮度调节控制
	if((cpu_s.oled.oled_drive_bright == 1) && (cpu_s.oled.oled_light_status == 1))
	{
		cpu_s.oled.oled_drive_bright = 0;
//		OLED_Driver_SetBrightnessCmd(p_data_manage->oled_bright);
	}
}

/****************************************************************
** Function name:           Gui_ThreadJobTaskRTMonitor
** Descriptions:            主任务时间监听处理
** input parameters:        无
** output parameters:       无 
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-08-02
*****************************************************************/ 
static void Gui_ThreadJobTaskRTMonitor(void) 
{  
	if(cpu_s.job_control.active == 1)
	{ 
		cpu_s.job_control.run_time++;
		
		// 如果界面支持长按功能,则需要保持界面亮屏
		if((cpu_s.job_control.key_press_hold == 1) && (Gui_DataBspKeyPressedState() == 1))
        {
            if(cpu_s.job_control.run_time > cpu_s.job_control.keep_time)
            {
                cpu_s.job_control.run_time--;
            }
        }
		
		if(cpu_s.job_control.run_time > cpu_s.job_control.keep_time) 
		{ 	
            if(Gui_GetTraverseUiState() == 0)
            {
                if(cpu_s.task == CPU_TASK_MAIN && charging_state == 1)
                {
                    //在充电
                    cpu_s.task = CPU_TASK_IDEL;
                    charging_state = 0;
                    if(Gui_DataGetBatteryLevelAndState(NULL) != BATTERRY_IS_NORMAL)
                    {
                        Gui_ThreadEventTaskWakeUp(CPU_EVENT_TASK_CHARGE);
                    }
                }
                
                //关闭按键开启的心率血压测量
                if(cpu_s.task == CPU_TASK_HEART )//|| cpu_s.task == CPU_TASK_BP)
                {
                    Gui_DataCloseHeartOrBpOpreation();
                }      
                
                if(gui_movement_change_page == 1)
                {
                    cpu_s.task = CPU_TASK_MAIN;
                    cpu_s.job_control.keep_time = 0;   
                    cpu_s.job_control.run_time = 0;
                    cpu_s.gui.clear_background_must = 1;
                    gui_movement_change_page = 0;
                    //WJ.20180320.修复界面跳转显示时间不足的问题
                    ui_change_flag = 1;
                    delay_time_cnt = 0;
                    cpu_s.gui.updata_ui_interval = 50;
                }
                else if(gui_movement_change_page == 2)
                {
                    cpu_s.task = CPU_TASK_MOVEMENT;
                    cpu_s.job_control.keep_time = 0;   
                    cpu_s.job_control.run_time = 0;
                    cpu_s.gui.clear_background_must = 1;
                    gui_movement_change_page = 0;
                    //WJ.20180320.修复界面跳转显示时间不足的问题
                    ui_change_flag = 1;
                    delay_time_cnt = 0;
                    cpu_s.gui.updata_ui_interval = 50;
                }
                else
                {
                    cpu_s.job_control.key_press_hold = 0;
                    cpu_s.job_control.active = 0;
                    cpu_s.task = CPU_TASK_IDEL;
                    //清除姿态亮屏状态
                    Gui_DataClearPostureToState();
                }
            }
            else
            {
                Gui_EventAndTaskTraverse(1);
            }
		}
	} 
}

/****************************************************************
** Function name:           Gui_ThreadEventTaskRTMonitor
** Descriptions:            外部任务时间监听处理
** input parameters:        无
** output parameters:       无 
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-08-02
*****************************************************************/ 
void Gui_ThreadEventTaskRTMonitor(void) 
{  
	if(cpu_s.event_control.active == 1)
	{
		cpu_s.event_control.run_time++; 
        
        // 如果界面支持长按功能,则需要保持界面亮屏
        if((cpu_s.event_control.key_press_hold == 1) && (Gui_DataBspKeyPressedState() == 1))
        {
            cpu_s.event_control.run_time--;
        }
        
		if(cpu_s.event_control.run_time > cpu_s.event_control.keep_time 
          || cpu_s.event == CPU_EVENT_TASK_IDEL) 
		{
            if(Gui_GetTraverseUiState() == 0)
            {
                cpu_s.event_control.key_press_hold = 0;
                cpu_s.event_control.active = 0;
                
//                if(cpu_s.event == CPU_EVENT_TASK_CARTOON && charging_state == 1)
//                {
//                    //在充电
//                    cpu_s.event = CPU_EVENT_TASK_IDEL;
//                    charging_state = 0;
//                    Gui_ThreadEventTaskWakeUp(CPU_EVENT_TASK_CHARGE);
//                }
//                else
                {
                    ui_change_flag = 1;
                    cpu_s.event = CPU_EVENT_TASK_IDEL;
                    //清除已读信息
                    Gui_DataClearUnreadMessage();
                    Gui_DataClearCallingState();
                    Gui_DataClearPhysMessage();
                }
            }
            else
            {
                Gui_EventAndTaskTraverse(2);
            }
		} 
	}
}

/****************************************************************
** Function name:           Gui_ThreadTaskMonitor
** Descriptions:            任务处理时间监听
** input parameters:        无
** output parameters:       无 
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-08-02
*****************************************************************/ 
static void Gui_ThreadTaskMonitor(void)
{		
	static uint8_t task_counter = 0xFF;
	
	//注意,这里是是每秒任务
	if(task_counter != clock->seconds)
	{
		task_counter = clock->seconds;
		Gui_ThreadJobTaskRTMonitor();
		Gui_ThreadEventTaskRTMonitor();
        
        if(cpu_s.task == CPU_TASK_IDEL && cpu_s.event == CPU_EVENT_TASK_IDEL)
        {
            cpu_s.oled.oled_drive_off = 1;    //关屏
        }
	}
	cpu_s.gui.page_frame_counter++;
    if(cpu_s.gui.page_frame_counter > 240)
    {
        cpu_s.gui.page_frame_counter = 1;
    }
}

/****************************************************************
** Function name:           Gui_ThreadEventTaskClear
** Descriptions:            清除外部事件
** input parameters:        无
** output parameters:       无 
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-08-02
*****************************************************************/ 
void Gui_ThreadEventTaskClear(void)
{
	cpu_s.event = CPU_EVENT_TASK_IDEL;
	cpu_s.event_control.active = 1;
    cpu_s.event_control.keep_time = 0;
    cpu_s.event_control.run_time = 0;
    cpu_s.event_control.key_press_hold = 0;
}

/****************************************************************
** Function name:           Gui_ThreadTaskClear
** Descriptions:            清除当前事件事件
** input parameters:        无
** output parameters:       无 
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-08-02
*****************************************************************/ 
void Gui_ThreadTaskClear(void)
{
	cpu_s.task = CPU_TASK_IDEL;
	cpu_s.job_control.active = 1;
    cpu_s.job_control.keep_time = 0;
    cpu_s.job_control.run_time = 0;
    cpu_s.job_control.key_press_hold = 0;
}

/****************************************************************
** Function name:           Gui_ThreadTaskWakeUp
** Descriptions:            添加事件，唤醒ui任务
** input parameters:        task:事件类型
** output parameters:       无 
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-08-02
*****************************************************************/ 
void Gui_ThreadTaskWakeUp(CPU_TASK_TYPE task)
{
    cpu_s.gui.animation_enable = 0x00;
	cpu_s.task = task;
	cpu_s.job_control.init = 1;
	cpu_s.oled.oled_drive_light = 1;
	gui_movement_change_page = 0; 
    
    if(task == CPU_TASK_MOVEMENT)
    {       
        gui_movement_change_page = 1; 
    }
    
	THREAD_TASK_WAKE_UP(gui_res_s.m_repaint_sem, gui_res_s.m_Task_count, gui_res_s.m_thread_handle);
}

/****************************************************************
** Function name:           Gui_ThreadEventTaskWakeUp
** Descriptions:            添加提醒事件，唤醒ui任务
** input parameters:        event:外部事件类型
** output parameters:       无 
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-08-02
*****************************************************************/ 
void Gui_ThreadEventTaskWakeUp(CPU_EVENT_TYPE event)
{    
    if(Gui_ThreadCanAcceptEvent(event) == 0)
    {
        return;
    }

    cpu_s.gui.animation_enable = 0x00;
	cpu_s.event = event;
	cpu_s.event_control.init = 1;
	cpu_s.oled.oled_drive_light = 1;
	
    if(Gui_DataSystemIsTestMode() == 0)
    {
        //停止震动
        Motor_DelEvent();
    }
    
    delay_time_cnt = 0;
    ui_change_flag = 1;    
	THREAD_TASK_WAKE_UP(gui_res_s.m_repaint_sem, gui_res_s.m_Task_count, gui_res_s.m_thread_handle);
}

/****************************************************************
** Function name:           Gui_ThreadStopWatchSwitching
** Descriptions:            秒表下页面切换
** input parameters:        无
** output parameters:       无 
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-08-11
*****************************************************************/ 
static void Gui_ThreadStopWatchSwitching(void)
{
   if(cpu_s.task == CPU_TASK_STOP_WATCH)
    {
        Gui_DataToggleStopWatch();
        if(stop_watch_frist_flag == 0)
        {
            cpu_s.job_control.keep_time = 3;
            cpu_s.job_control.run_time = 0;
        }
    }
    else
    {
        //添加秒表UI唤醒
        stop_watch_frist_flag = 0;
        cpu_s.task = CPU_TASK_STOP_WATCH;
        cpu_s.job_control.init = 1;
    }            
    gui_movement_change_page = 1;  
}

/****************************************************************
** Function name:           Gui_ThreadMovementModeSwitching
** Descriptions:            运动模式下页面切换
** input parameters:        无
** output parameters:       无 
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-08-11
*****************************************************************/ 
static void Gui_ThreadMovementModeSwitching(void)
{
    /* 运动模式中*/
    if(cpu_s.task == CPU_TASK_IDEL || cpu_s.task == CPU_TASK_MOVEMENT_EXIT || cpu_s.task == CPU_TASK_MAIN)
    {
        cpu_s.task = CPU_TASK_MOVEMENT;
        cpu_s.job_control.init = 1;              
        gui_movement_change_page = 1;   
    }
    else if(cpu_s.task == CPU_TASK_MOVEMENT && Gui_DataMovementModeIsNotDisplayPause() == 0)
    {
        cpu_s.task = CPU_TASK_MOVEMENT_PAUSE; 
        cpu_s.job_control.init = 1;             
        gui_movement_change_page = 2;
    }
    else
    {
        cpu_s.task = CPU_TASK_MOVEMENT_EXIT;   
        cpu_s.job_control.init = 1;                  
        gui_movement_change_page = 2;
    }    
}

/****************************************************************
** Function name:           Gui_ThreadTaskSwitching
** Descriptions:            页面切换
** input parameters:        无
** output parameters:       无 
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-08-02
*****************************************************************/ 
static void Gui_ThreadTaskSwitching(void)
{    
    uint8_t event_flag = 0;
    
    /*切换界面*/
    if(cpu_s.event != CPU_EVENT_TASK_IDEL)
    {    
        //确保外部事件显示
        if(cpu_s.event_control.run_time >= 1 && cpu_s.event_control.init != 1)
        {
            disconnet_keep_flg = 0;
            //退出外部事件
            Gui_ThreadEventTaskClear();
            if(cpu_s.task != CPU_TASK_IDEL)
            {
                //设备没有处于测试模式,需要关闭震动
                if(!Gui_DataSystemIsTestMode())
                {
                    Motor_DelEvent();    //停止震动
                }
                return;
            } 
            event_flag = 1;
        }
        else
        {
            //WJ.20180130.防止线程停止事件没有处理完成
            THREAD_TASK_WAKE_UP(gui_res_s.m_repaint_sem, gui_res_s.m_Task_count, gui_res_s.m_thread_handle);    
            if(cpu_s.event > CPU_EVENT_TASK_ALL_EXIT)
            {
                return;
            }
            else
            {
                Gui_ThreadEventTaskClear();
            }
        }
    }  
    
    if(Gui_DataStopWatchIsBusy())
    {
        //判断是否处于秒表激活状态下
       Gui_ThreadStopWatchSwitching();           
    }
    else if(Gui_DataMovementModeIsBusy())
    {
        /* 运动模式中*/
        Gui_ThreadMovementModeSwitching();
    }
    else
    {
        // 如果是待机时,则判断是否低电提醒
        //WJ.2080509.清除了外部时间后，进入主界面前显示低电图标的问题
        if(event_flag == 0 && (cpu_s.task == CPU_TASK_IDEL) && (cpu_s.event == CPU_EVENT_TASK_IDEL )
          && Gui_DataBatteryIsLowPower() != BATTERRY_NOT_LOW_POWER)
        {
            Gui_ThreadEventTaskWakeUp(CPU_EVENT_TASK_LOWPOWER);  
            cpu_s.task = CPU_TASK_MAIN;
            cpu_s.job_control.init = 1;
            cpu_s.job_control.keep_time = 6;
            cpu_s.job_control.run_time = 0;
            cpu_s.job_control.active = 1;
        }           
        else
        {                             
            //WJ.20180320.防止主界面没有显示跳页
            if(cpu_s.task == CPU_TASK_MAIN && cpu_s.job_control.init == 1)
            {
                THREAD_TASK_WAKE_UP(gui_res_s.m_repaint_sem, gui_res_s.m_Task_count, gui_res_s.m_thread_handle);    
                return;
            }
            //关闭按键开启的心率血压测量
            if(cpu_s.task == CPU_TASK_HEART )//|| cpu_s.task == CPU_TASK_BP)
            {
                Gui_DataCloseHeartOrBpOpreation();
            } 
            
            cpu_s.task++;		  
            
//            if(Gui_DataIsBpFunction() != 1)//(cpu_s.task == CPU_TASK_BP) && Gui_DataIsBpFunction() != 1)
//            {
//                cpu_s.task++;
//            }
//            if((cpu_s.task == CPU_TASK_FINDPHONE) && Gui_DataFindPhonePageIsEnable() != 1)
//            {
//                cpu_s.task++;
//            }
            if((cpu_s.task == CPU_TASK_STOP_WATCH) && Gui_DataStopWatchPageIsEnable() != 1)
            {
                cpu_s.task++;
            }           
//            if((cpu_s.task == CPU_TASK_COUNT_DOWN) && Gui_DataCountDownPageIsEnable() != 1)
//            {
//                cpu_s.task++;
//            }
            if((cpu_s.task == CPU_TASK_PHY) && (Gui_DataGetPhysMenstrualPeriodType() == 0))
            {
                cpu_s.task++;
            }
                
            
            if(cpu_s.task > CPU_TASK_POWEROFF)
            {
                cpu_s.task = CPU_TASK_MAIN;
            }
            
            cpu_s.gui.animation_enable = 0x00;
            cpu_s.job_control.init = 1;
            cpu_s.job_control.key_press_hold = 0;        
        }        
    }
    
    //设备没有处于测试模式,需要关闭震动
    if(!Gui_DataSystemIsTestMode())
    {
        Motor_DelEvent();    //停止震动
    }  
    cpu_s.oled.oled_drive_light = 1;     
    Gui_ThreadWakeUpThread();      
}

/****************************************************************
** Function name:           Gui_ThreadKeyClickTask
** Descriptions:            按键事件处理，短按处理
** input parameters:        无
** output parameters:       无 
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-08-02
*****************************************************************/ 
static void Gui_ThreadKeyClickTask(void)
{          
   	if(!Gui_DataSystemIsPoweOn())
	{
        /*关机下短按无效,直接返回*/
        return;
	}
    if(Gui_DataGetBatteryLevelAndState(NULL) != BATTERRY_IS_NORMAL)
    {
        /*充电中*/
        if(Gui_GetThreadTask() != CPU_TASK_MAIN)
        {
            Gui_ThreadTaskWakeUp(CPU_TASK_MAIN);
        }
        cpu_s.event = CPU_EVENT_TASK_IDEL;
        return;
    }
    
    //是否处理来电状态,来电有静音处理
    if(cpu_s.event == CPU_EVENT_TASK_CALL_REMIND && Gui_DataMessageIsCall())
    {
        if( Gui_DataMessageCallState() < GUI_CALL_REJECTSUC)
        {
            //设置来电静音
            Gui_DataMessageCallToMute();
            Motor_DelEvent();    //停止震动          
            cpu_s.gui.animation_enable = 0x00;
            return;
        }
        else
        {
            cpu_s.event_control.keep_time = 0;
            cpu_s.event_control.run_time = 0;
            cpu_s.event = CPU_EVENT_TASK_IDEL;
        }
    }
       
    //是否在拍照
    if(cpu_s.event == CPU_EVENT_TASK_CAMERAREADY     // 事件任务--拍照准备就绪
      || cpu_s.event == CPU_EVENT_TASK_CAMERAOK)     // 事件任务--拍照完成
    {
        Gui_DataClickOnThePhoto();
        Gui_ThreadEventTaskWakeUp(CPU_EVENT_TASK_CAMERAOK);
        return;
    }       
    
    
    if(disconnet_keep_flg == 1 && cpu_s.event == CPU_EVENT_TASK_IDEL && cpu_s.task == CPU_TASK_IDEL)
    {
        disconnet_motor_flg = 1;
        Gui_ThreadEventTaskWakeUp(CPU_EVENT_TASK_BLE_DISC_REMIND);
        return;
    }  
            

    //是否有未读信息
    //女性功能提醒未读取
    if((cpu_s.event == CPU_EVENT_TASK_PHYS_REMIND || Gui_ThreadCanAcceptEvent(CPU_EVENT_TASK_PHYS_REMIND))
        && Gui_DataHaveUnreadPhysMessage(Gui_ThreadIsReadingNotify(CPU_EVENT_TASK_PHYS_REMIND)))
    {
        //唤醒UI事件处理UI推送显示
        Gui_ThreadEventTaskWakeUp(CPU_EVENT_TASK_PHYS_REMIND);            
        //停止震动 
        Motor_DelEvent();
        cpu_s.event_control.keep_time = 5;
        cpu_s.event_control.run_time = 0;
        
        return;
    }
    
    if(cpu_s.event == CPU_EVENT_TASK_PHYS_REMIND && cpu_s.event_control.init != 0x01)
    {            
        cpu_s.event = CPU_EVENT_TASK_IDEL;
    }
    if((cpu_s.event == CPU_EVENT_TASK_MESSAGE_REMIND || (Gui_ThreadCanAcceptEvent(CPU_EVENT_TASK_MESSAGE_REMIND)))
        && Gui_DataHaveUnreadMessage(Gui_ThreadIsReadingNotify(CPU_EVENT_TASK_MESSAGE_REMIND)))
    {
        //唤醒UI事件处理UI推送显示
        Gui_ThreadEventTaskWakeUp(CPU_EVENT_TASK_MESSAGE_REMIND);            
        //停止震动 
        Motor_DelEvent();
        cpu_s.event_control.keep_time = 5;
        cpu_s.event_control.run_time = 0;
        
        return;
    }
    if(cpu_s.event == CPU_EVENT_TASK_MESSAGE_REMIND && cpu_s.event_control.init != 0x01)
    {            
        cpu_s.event = CPU_EVENT_TASK_IDEL;
    }

    Gui_ThreadTaskSwitching();        
}

/****************************************************************
** Function name:           Gui_ThreadKeyLongPressTask
** Descriptions:            按键事件处理，3s长按处理
** input parameters:        无
** output parameters:       无 
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-08-02
*****************************************************************/ 
static void Gui_ThreadKeyLongPressTask(void)
{
	if(!Gui_DataSystemIsPoweOn())
	{
        /*关机下做开机处理*/    
        if(Gui_DataBatteryIsLowPower() == BATTERRY_IS_LOW_TO_SHUT)
        {
            //低电显示低电告警           
            Gui_ThreadEventTaskWakeUp(CPU_EVENT_TASK_LOWPOWER);  
        }
//        else
//        {        
//            Gui_DataSystemToPoweOn();
////            Motor_AddEvent(1, 0, 0.5 * 20, 0);
//        }
        return;
	}
        
    if(cpu_s.event == CPU_EVENT_TASK_CALL_REMIND && Gui_DataMessageIsCall())
    {
        /*来电状态,长按挂断来电*/
        Gui_DataMessageCallToHangUp();
        return;
    }
    else if(cpu_s.event == CPU_EVENT_TASK_CAMERAREADY 
           || cpu_s.event == CPU_EVENT_TASK_CAMERAOK)
    {
        /* 退出拍照*/
        Gui_DataLongKeyToExitPhoto();
        return;
    }
    
    if(cpu_s.event == CPU_EVENT_TASK_IDEL)
    {
        if(cpu_s.task == CPU_TASK_MOVEMENT_PAUSE)
        {
            /*暂停开始运动模式*/
            Gui_DataToggleMovementMode(); 
            Gui_ThreadEventTaskClear();  
            cpu_s.job_control.init = 0x01;     
            cpu_s.job_control.keep_time = 5;            
            cpu_s.job_control.run_time = 0;            
            Motor_AddEvent(1,0,0.5 * 20,0); 
        }
        else if(cpu_s.task == CPU_TASK_MOVEMENT_EXIT)
        {
            /*退出运动模式*/
            Gui_DataExitMovementMode();
            Gui_ThreadEventTaskClear();
            cpu_s.task = CPU_TASK_MAIN;		
            cpu_s.job_control.init = 0x01;
            gui_movement_change_page = 0; 
        }
        else if(cpu_s.task == CPU_TASK_MOVEMENT)
        {
            /*进入运动模式*/
            if(Gui_DataMovementModeIsBusy() == MOVEMENT_MODE_IDLE 
              && Gui_DataGetBatteryLevelAndState(NULL) == BATTERRY_IS_NORMAL)
            {
                Gui_DataStartMovementMode();      
                cpu_s.job_control.init = 0x01;  
                cpu_s.job_control.keep_time = 15;            
                cpu_s.job_control.run_time = 0;           
                gui_movement_change_page = 1;     
            }
        }
//        else if(cpu_s.task == CPU_TASK_FINDPHONE)
//        {
//            /*手机查找*/
//            cpu_s.gui.animation_enable = 0x01;
//            cpu_s.job_control.keep_time = 60;
//            cpu_s.job_control.run_time = 0;
//            
//        }
//        else if(cpu_s.task == CPU_TASK_COUNT_DOWN)
//        {
//            /*倒计时功能按键触发*/
//            
//            //需要判断倒计时当前状态
//            if(Gui_DataCountDownIsBusy())
//            {
//                Gui_DataCountDownExit();
//                cpu_s.task = CPU_TASK_MAIN;
//                cpu_s.job_control.keep_time = 3;            
//                cpu_s.job_control.run_time = 0;		
//                cpu_s.job_control.init = 0x01;      
//            }
//            else
//            {
//                Gui_DataCountDownStart();
//                cpu_s.job_control.keep_time = 0;            
//                cpu_s.job_control.run_time = 1;
////                cpu_s.gui.page_frame_counter = 1;
//                //WJ.20171108.解决在常驻界面开启倒计时被其他事件打断
//                Gui_ThreadEventTaskWakeUp(CPU_EVENT_TASK_COUNT_DOWN_START);                 
//            }       
//        }
        else if(cpu_s.task == CPU_TASK_STOP_WATCH)
        {
            /*秒表*/
            if(!Gui_DataStopWatchIsBusy())
            {
                /*秒表就绪*/
                Gui_DataSetStopWatchToReady();   
                cpu_s.job_control.keep_time = 60;            
                cpu_s.job_control.run_time = 0;
                stop_watch_frist_flag = 1;
                cpu_s.gui.clear_background_must = 0;    
            }
            else
            {
                /*退出秒表,跳转主界面*/
                Gui_DataExitStopWatch();
                //跳转主界面
                cpu_s.task = CPU_TASK_MAIN;		
                cpu_s.job_control.init = 0x01;
                stop_watch_frist_flag = 0;
            }
        }
        else if(cpu_s.task == CPU_TASK_POWEROFF)
        {
            /*关机处理*/
            Gui_DataSystemToPoweOff();
            Motor_AddEvent(1, 0, 0.5 * 20, 0);
            return;
        }
        else if(cpu_s.task == CPU_TASK_MAIN)
        {
            Gui_ThreadEventTaskWakeUp(CPU_EVENT_TASK_MAC_VER);
        }
    }    
}

/****************************************************************
** Function name:           Gui_ThreadAddKeyEvent
** Descriptions:            按键事件处理，6s长按处理
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ
** Created Date:            2017-08-02
*****************************************************************/ 
static void Gui_ThreadKeyLongLongPressTask(void)
{  
    //WJ.20180605.显示开关机动画中，屏蔽按键
    if(cpu_s.event == CPU_EVENT_TASK_CARTOON)
    {
        Gui_ThreadWakeUpThread();
        return;
    }    
    
   	if(!Gui_DataSystemIsPoweOn())
	{
        /*关机下短按无效,直接返回*/    
        return;
	}

	if(cpu_s.task == CPU_TASK_SLEEP)
	{
        /*清除密码*/
        Gui_DataClearSystemPassword();
		Motor_AddEvent(1,0,0.5 * 20,0);
	}        
}

/****************************************************************
** Function name:           Gui_ThreadAddKeyEvent
** Descriptions:            按键事件处理
** input parameters:        event：按键类型
** output parameters:       无 
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-08-02
*****************************************************************/ 
void Gui_ThreadAddKeyEvent(BUTTON_EVENT_TYPE event)
{ 
    cpu_button_s.button_event = event;
    if(event == BUTTON_EVENT_LONGPRESS && system_flag_s.sign_off)
    {
        if(Gui_DataBatteryIsLowPower() != BATTERRY_IS_LOW_TO_SHUT)
        {
            App_AddEvent(APP_BOOT_EVENT);
            return;
        }
        
    }
    
    //唤醒线程
    xSemaphoreGive(gui_res_s.m_repaint_sem);
    xSemaphoreGive(gui_res_s.m_Task_count);
    vTaskResume(gui_res_s.m_thread_handle); 
}

/****************************************************************
** Function name:           Gui_ThreadDealKeyEvent
** Descriptions:            按键事件处理，处理按键分类
** input parameters:        无
** output parameters:       无 
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-08-02
*****************************************************************/ 
static void Gui_ThreadDealKeyEvent(void)
{
    uint8_t event = 0;
    
    event = cpu_button_s.button_event;
    cpu_button_s.button_event = 0;
      
    if(event == 0)
    {  
		return;
    }
    /*调试UI遍历用*/
    Gui_SetTraverseUiState(0);
    
    //WJ.20180612.不在需要通过按键删除蓝牙断连提醒事件	
    //WJ.20180403.删除蓝牙断连提醒
//    Gui_DataDelBleDiscRemind();
 
    if(event == BUTTON_EVENT_CLICK)
    {
        /*短按事件*/
        Gui_ThreadKeyClickTask();
    }
    else if(event == BUTTON_EVENT_LONGPRESS)
    {
        /*长按3s事件*/
        Gui_ThreadKeyLongPressTask();
    }
    else if(event == BUTTON_EVENT_LONGPRESS_6S)
    {
        /*长按6s事件*/
        Gui_ThreadKeyLongLongPressTask();       
    }
}

/****************************************************************
** Function name:           Gui_ThreadPostureToCloseScreen
** Descriptions:            姿态灭屏
** input parameters:        无
** output parameters:       无 
** Returned value:          返回0:关屏事件添加失败, 1:关屏事件添加成功
** Created by:              WJ              
** Created Date:            2017-08-02
*****************************************************************/ 
uint8_t Gui_ThreadPostureToCloseScreen(void)
{
    if(cpu_s.task == CPU_TASK_MAIN && cpu_s.event == CPU_EVENT_TASK_IDEL)
    {  
        cpu_s.job_control.run_time = 1;
        cpu_s.job_control.keep_time = 0;
        Gui_ThreadWakeUpThread();
        return 1;
    }
    else if(cpu_s.task == CPU_TASK_IDEL && cpu_s.event == CPU_EVENT_TASK_IDEL)
    {             
        return 1;
    }   
    return 0;
}

/****************************************************************
** Function name:           Gui_ThreadPostureToBrightScreen
** Descriptions:            姿态亮屏
** input parameters:        无
** output parameters:       无 
** Returned value:          返回0:亮屏事件添加失败, 1:亮屏事件添加成功
** Created by:              WJ              
** Created Date:            2017-08-02
*****************************************************************/ 
uint8_t Gui_ThreadPostureToBrightScreen(void)
{
    if(disconnet_keep_flg == 1)
    {
        disconnet_motor_flg = 1;
        Gui_ThreadEventTaskWakeUp(CPU_EVENT_TASK_BLE_DISC_REMIND);
        return 1;
    }
    
    if(cpu_s.task == CPU_TASK_IDEL && cpu_s.event == CPU_EVENT_TASK_IDEL) 
    {
        Threadflag = 1;
        posture_to_bright_flg = 1;
        Gui_ThreadWakeUpThread();
//        Gui_ThreadTaskSwitching();
        return 1;
    }  
    
    
    Threadflag = 0;
    return 0;
}

/****************************************************************
** Function name:           Gui_ThreadIsReadingNotify
** Descriptions:            是否在查看信息
** input parameters:        无
** output parameters:       无 
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-08-02
*****************************************************************/ 
static uint8_t Gui_ThreadIsReadingNotify(uint8_t event)
{
    if(event == CPU_EVENT_TASK_MESSAGE_REMIND && cpu_s.event == CPU_EVENT_TASK_MESSAGE_REMIND && cpu_s.event_control.init != 1)
    {
        return 1;
    }        
    if(event == CPU_EVENT_TASK_PHYS_REMIND && cpu_s.event == CPU_EVENT_TASK_PHYS_REMIND && cpu_s.event_control.init != 1)
    {
        return 1;
    }
    return 0;
}

/****************************************************************
** Function name:           Gui_ThreadSetDisPlayArea
** Descriptions:            设置当前显示区域，用于下次显示清屏
** input parameters:        start_column:开始列；start_page：开始行；
                            column_size：多少列；page_size:多少行
** output parameters:       无 
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-08-02
*****************************************************************/ 
void Gui_ThreadSetDisPlayArea(uint8_t start_column, uint8_t start_page, uint8_t column_size, uint8_t page_size)
{    
    cpu_s.gui.start_column = start_column;
    cpu_s.gui.start_page = start_page;
    cpu_s.gui.column_size = column_size;
    cpu_s.gui.page_size = page_size;
}

/****************************************************************
** Function name:           Gui_ThreadPowerOff
** Descriptions:            关机界面处理
** input parameters:        无 
** output parameters:       无 
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-08-12
*****************************************************************/ 
void Gui_ThreadPowerOff(void)
{        
	memset(&cpu_s, 0, sizeof(cpu_task_t));	
    cpu_s.event_control.active = 1;
    cpu_s.gui.updata_ui_interval = 500;   //200ms 
    cpu_s.event = CPU_EVENT_TASK_IDEL;
    cpu_s.task = CPU_TASK_IDEL;  
//    if(Gui_DataGetBatteryLevelAndState(NULL) == BATTERRY_IS_CHARGING)
//    {    
//        cpu_s.event_control.init = 1;
//    }  
//    else
//    {      
//        cpu_s.oled.oled_light_status = 1;
////        cpu_s.oled.oled_drive_off = 1;
//    }
    Gui_ThreadSetDisPlayArea(0, 0, 240, 240);
    
    //WJ.20180605.增加开机动画
    Gui_ThreadEventTaskWakeUp(CPU_EVENT_TASK_CARTOON1);
    
    Gui_ThreadWakeUpThread();
}

/****************************************************************
** Function name:           Gui_ThreadPowerOn
** Descriptions:            开机界面处理
** input parameters:        无 
** output parameters:       无 
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-08-12
*****************************************************************/ 
void Gui_ThreadPowerOn(void)
{        
    Gui_LcdDriverInit();
	memset(&cpu_s, 0, sizeof(cpu_task_t));	
	cpu_task_controller_t *p_task = &cpu_s.job_control;
	//配置主界面参数   
    TASK_ENTRY_INIT(p_task, 3);
	cpu_s.event   = CPU_EVENT_TASK_IDEL;
	cpu_s.task    = CPU_TASK_MAIN;
	cpu_s.job_control.init = 1;	
    cpu_s.gui.clear_background = 1;
	cpu_s.oled.oled_drive_light = 1;
    cpu_s.gui.updata_ui_interval = 300;   //300ms    
    Gui_ThreadSetDisPlayArea(0, 0, 240, 240);
    delay_time_cnt = 0;
    ui_change_flag = 1;
    
     //WJ.20180605.增加开机动画
    if(Gui_DataGetBatteryLevelAndState(NULL) == BATTERRY_IS_NORMAL)
    {
        Gui_ThreadEventTaskWakeUp(CPU_EVENT_TASK_CARTOON);
    }
    else
    {
        Gui_ThreadWakeUpThread();
    }
}

/****************************************************************
** Function name:           Gui_ThreadCanAcceptEvent
** Descriptions:            事件优先级处理，判断当前事件是否响应
** input parameters:        无 
** output parameters:       无 
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-08-12
*****************************************************************/ 
uint8_t Gui_ThreadCanAcceptEvent(uint8_t event)
{
    uint8_t ret = 1;
    
    if(Gui_GetTraverseUiState() == 1)
    {
        /* UI遍历中，屏蔽所有事件 */
        return 0;
    }

    //WJ.20180605.开关机动画中，不可以加入其他事件
    if(cpu_s.event == CPU_EVENT_TASK_CARTOON)
    {
        return 0;
    }
    if(event == CPU_EVENT_TASK_CARTOON)
    {
        return 1;
    }

    
    if(event == cpu_s.event)
    {
        if(cpu_s.event == CPU_EVENT_TASK_CHARGE)
        {
//            TASK_ENTRY_INIT(cpu_s.event_control, 8);
            cpu_s.event_control.keep_time = 8;
            cpu_s.event_control.run_time = 0;
            //WJ.20171121.解决来电后去充电，
            Gui_DataClearCallingState();            
            return 0;
        }
        if(cpu_s.event == CPU_EVENT_TASK_CAMERAOK)
        {
            cpu_s.event_control.keep_time = 5;
            cpu_s.event_control.run_time = 0;
            return 0;
        }
        
        if(cpu_s.event != CPU_EVENT_TASK_MESSAGE_REMIND && cpu_s.event != CPU_EVENT_TASK_CALL_REMIND && cpu_s.event != CPU_EVENT_TASK_PHYS_REMIND )
        {
            return 0;
        }

    }
    
    if(event == CPU_EVENT_TASK_CHARGE)
    {
        cpu_s.job_control.keep_time = 0;
        gui_movement_change_page = 0;
        Gui_DataClearCallingState();
        return 1;
    }
    
    if(Gui_DataGetBatteryLevelAndState(NULL) != BATTERRY_IS_NORMAL)
    {
        /* 连接着充电器 屏蔽其他事件 */
        return 0;
    }
    
    if(event == CPU_EVENT_TASK_ALL_EXIT && cpu_s.event > CPU_EVENT_TASK_ALL_EXIT)
    {
        return 1;
    }
    
    if(event == CPU_EVENT_TASK_ALL_EXIT && cpu_s.event <= CPU_EVENT_TASK_ALL_EXIT)
    {
        return 0;
    }
    
//    if(cpu_s.task == CPU_TASK_FINDPHONE && cpu_s.gui.animation_enable == 1)
//    {
//        return 0;
//    }
    
    if(event == CPU_EVENT_TASK_CALL_REMIND)
    {
        //来电事件，允许执行
        if(cpu_s.task != CPU_TASK_IDEL)
        {
            cpu_s.job_control.keep_time = 0;
            cpu_s.job_control.run_time = 1;
        }
        gui_movement_change_page = 0; 
        //WJ.20171010.解决秒表来电时导致黑屏长时间不能亮屏
        stop_watch_frist_flag = 0;
        
        /* 退出拍照*/
        Gui_DataLongKeyToExitPhoto();
        
        return 1;
    }
    
    if(cpu_s.event == CPU_EVENT_TASK_CALL_REMIND)
    {
        /* 来电提醒中*/
        return 0;
    }
    
    if(cpu_s.event == CPU_EVENT_TASK_CAMERAREADY || cpu_s.event == CPU_EVENT_TASK_CAMERAOK)
    {
        if(event == CPU_EVENT_TASK_CAMERAREADY || event == CPU_EVENT_TASK_CAMERAOK)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
    
    if(cpu_s.event <= CPU_EVENT_TASK_ALL_EXIT && cpu_s.task == CPU_TASK_IDEL)
    {
        ret = 1;
    }
    else
    {   
        if((cpu_s.task == CPU_TASK_MOVEMENT && Gui_DataMovementModeIsBusy()) 
           || cpu_s.task == CPU_TASK_MOVEMENT_PAUSE || cpu_s.task == CPU_TASK_MOVEMENT_EXIT)
        {            
            /* 运动模式亮屏中*/              
            ret = 0;
        }
        else if(cpu_s.task == CPU_TASK_HEART 
                && (event == CPU_EVENT_TASK_CAMERAREADY || event == CPU_EVENT_TASK_CAMERAOK))
        {
            /* 心率测量中*/
            ret = 0;
        }
        else if((event == CPU_EVENT_TASK_CAMERAREADY || event == CPU_EVENT_TASK_CAMERAOK))//cpu_s.task == CPU_TASK_BP
                //&& (event == CPU_EVENT_TASK_CAMERAREADY || event == CPU_EVENT_TASK_CAMERAOK))
        {
            /* 血压测量中*/
            ret = 0;
        }
        else if(cpu_s.task == CPU_TASK_STOP_WATCH && Gui_DataStopWatchIsBusy())
        {
            /* 秒表运行中*/
            ret = 0;
        }
        else if(cpu_s.event == CPU_EVENT_TASK_ALARM 
                && (event == CPU_EVENT_TASK_PHYS_REMIND || event == CPU_EVENT_TASK_MESSAGE_REMIND || event == CPU_EVENT_TASK_SIT))
        {
            /* 闹钟提醒中*/
            ret = 0;
        }
        else if(cpu_s.event == CPU_EVENT_TASK_SIT && event == CPU_EVENT_TASK_STARGET)
        {
            /* 久坐提醒中*/
            ret = 0;
        }
        else if(cpu_s.event == CPU_EVENT_TASK_PHYS_REMIND)
        {
            /* 女性消息提醒中*/
            if(Gui_DataIsReadPhysMessage())
            {
                if(!(event == CPU_EVENT_TASK_CAMERAREADY || event == CPU_EVENT_TASK_CAMERAOK))
                {
                    ret = 0;
                }
            }
        }
        else if(cpu_s.event == CPU_EVENT_TASK_MESSAGE_REMIND)
        {
            /* 消息提醒中*/
            if(Gui_DataIsReadMessage())
            {
                if(!(event == CPU_EVENT_TASK_CAMERAREADY || event == CPU_EVENT_TASK_CAMERAOK))
                {
                    ret = 0;
                }
            }
        }
        else if(cpu_s.event == CPU_EVENT_TASK_COUNT_DOWN_START
                && !(event == CPU_EVENT_TASK_CAMERAREADY || event == CPU_EVENT_TASK_CAMERAOK)
                && event!= CPU_EVENT_TASK_COUNT_DOWN_TIMEOUT)
        {
            /* 倒计时开始中*/
            ret = 0;
        }
        else if(cpu_s.event == CPU_EVENT_TASK_COUNT_DOWN_TIMEOUT
                && !(event == CPU_EVENT_TASK_CAMERAREADY || event == CPU_EVENT_TASK_CAMERAOK)
                && event!= CPU_EVENT_TASK_COUNT_DOWN_START)
        {
            /* 倒计时开始中*/
            ret = 0;
        }    
    }   
    return ret;   
}

/****************************************************************
** Function name:           Gui_ThreadUpdateStopwatch
** Descriptions:            更新秒表界面
** input parameters:        无 
** output parameters:       无 
** Returned value:          无
** Created by:              WJ
** Created Date:            2017-08-12
*****************************************************************/ 
void Gui_ThreadUpdateStopwatch(void)
{
    if(cpu_s.task == CPU_TASK_STOP_WATCH)
    {
        cpu_s.job_control.init = 1;
        cpu_s.job_control.run_time = 0;
    }
}

/****************************************************************
** Function name:           Gui_ThreadUpdateMian
** Descriptions:            更新主界面
** input parameters:        无 
** output parameters:       无 
** Returned value:          无
** Created by:              WJ
** Created Date:            2017-08-12
*****************************************************************/ 
void Gui_ThreadUpdateMian(void)
{
    if(cpu_s.task == CPU_TASK_MAIN)
    {
        cpu_s.job_control.init = 1;
        cpu_s.job_control.run_time = 0;
    }
}

/****************************************************************
** Function name:           Gui_ThreadIsFindPhone
** Descriptions:            是否在查找手机界面
** input parameters:        无 
** output parameters:       无 
** Returned value:          1：是  0：不是
** Created by:              YX              
** Created Date:            2017-08-12
*****************************************************************/ 
uint8_t Gui_ThreadIsFindPhone(void)
{
    return 0;//(cpu_s.task == CPU_TASK_FINDPHONE);
}

/****************************************************************
** Function name:           Gui_ThreadTaskToIdle
** Descriptions:            任务界面进入空闲
** input parameters:        无 
** output parameters:       无 
** Returned value:          1：是  0：不是
** Created by:              YX              
** Created Date:            2017-08-12
*****************************************************************/ 
void Gui_ThreadTaskToIdle(void)
{
    cpu_s.task = CPU_TASK_IDEL;
    Motor_DelEvent();
}

/****************************************************************
** Function name:           Gui_GetThreadTask
** Descriptions:            获取当前任务ID
** input parameters:        无 
** output parameters:       无 
** Returned value:          任务ID
** Created by:              YX              
** Created Date:            2017-09-06
*****************************************************************/ 
uint8_t Gui_GetThreadTask(void)
{
    return cpu_s.task;
}

/****************************************************************
** Function name:           Gui_GetThreadJob
** Descriptions:            获取当前事件ID
** input parameters:        无 
** output parameters:       无 
** Returned value:          事件ID
** Created by:              YX              
** Created Date:            2017-09-06
*****************************************************************/ 
uint8_t Gui_GetThreadJob(void)
{
    return cpu_s.event;
}

/****************************************************************
** Function name:           Gui_CheckEvent
** Descriptions:            UI事件自检，防止事件残留导致UI不显示
** input parameters:        无 
** output parameters:       无 
** Returned value:          无
** Created by:              WJ             
** Created Date:            2018-01-30
*****************************************************************/ 
void Gui_CheckEvent(void)
{
    static uint8_t check_times_cnt = 0;	
    static uint8_t task_counter = 0xFF;
	
	//注意,这里是是每秒任务
	if(task_counter == clock->seconds)
	{
        return;
    }
    
    task_counter = clock->seconds;
    //对外部事件处理自检
    if(Gui_GetThreadJob() != CPU_EVENT_TASK_IDEL)
    {
        //WJ.20180205.修正线程运行状态条件
        if(thread_handle_run == 0)//eRunning != eTaskGetState(gui_res_s.m_thread_handle))
        {
            check_times_cnt++;
            if(check_times_cnt > 5)
            {
                check_times_cnt = 0;
                Gui_ThreadEventTaskWakeUp(CPU_EVENT_TASK_ALL_EXIT);
            }
        }
        else
        {
            check_times_cnt = 0;
        }
    }
    
    //限制刷新间隔的最大值：10s
    if(cpu_s.gui.updata_ui_interval > 10000)
    {
        cpu_s.gui.updata_ui_interval = 1000;
    }
}
/****************************************************************
** Function name:           Gui_SetChargingState
** Descriptions:            充电图标延后显示
** input parameters:        无 
** output parameters:       无 
** Returned value:          无
** Created by:              WJ             
** Created Date:            2018-06-05
*****************************************************************/ 
void Gui_SetChargingState(uint8_t state)
{
    charging_state = state;
}

uint8_t Gui_GetChargingState(void)
{
    return charging_state;
}
