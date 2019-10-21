/***************************Copyright(c)***********************************
**                       Veepoo Tech
**                  COPYRIGHT (C) 2016-2019
**-----------File Info-----------------------------------------------------
** File        name:     gui_traverse.c
** Last modified Date:   2017-09-21
** Last SYS_VERSION:     0.0.0
** Descriptions:            
**-------------------------------------------------------------------------
** Created  by:          YX
** Created  date:        2017-09-21
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
#include "gui_page_display.h"
#include "app_type.h"
#include "gui_traverse.h"
#include "gui_data_inteface.h"
#include "CPU_GeneralTask.h"

uint8_t test_dis_ui_flg = 0;                                 //开启UI遍历的标志 0：未开启  1：已开启
static uint32_t traverse_tmp_buf1 = 0;                       //UI遍历的参数1
static uint32_t traverse_tmp_buf2 = 0;                       //UI遍历的参数2
static uint32_t traverse_tmp_buf3 = 0;                       //UI遍历的参数3
static uint32_t traverse_tmp_buf4 = 0;                       //UI遍历的参数4

//static uint8_t evnet_interval_cnt = 0xFF;                    //最后的间隔次数，用于判断是否刷新UI
//static uint8_t task_interval_cnt = 0xFF;                     //最后的间隔次数，用于判断是否刷新UI和动图切换
static uint8_t event_msg_type = 0;

extern cpu_param_t cpu;				                    //任务参数结构体
extern gui_resource_t   st_gui_res;		                //gui资源结构体
//extern cpu_param_t cpu_s;
//extern gui_resource_t  gui_res_s;
extern uint8_t ui_style_type;

void Cpu_TaskParamInit(cpu_task_tcb_t* p_task,uint16_t keep_time,bool press_hold,bool animation_active);
void Cpu_EventParamInit(cpu_event_tcb_t* p_event,uint16_t keep_time,bool press_hold,bool animation_active);
/****************************************************************
** Function name:           Gui_ThreadWakeUpThread
** Descriptions:            唤醒UI线程
** input parameters:        无
** output parameters:       无 
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-08-02
*****************************************************************/
static void Gui_ThreadWakeUpThread(void)
{    
    xSemaphoreGive(st_gui_res.m_task_count);

    if(eTaskGetState(st_gui_res.m_thread_handle) != eRunning) 
    {
        xTaskResumeFromISR(st_gui_res.m_thread_handle);  
    }      
}

/****************************************************************
** Function name:           Gui_SetTraverseUiState
** Descriptions:            设置UI遍历状态
** input parameters:        state:状态 1:开启UI遍历  0：关掉UI遍历
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-08-02
*****************************************************************/
void Gui_SetTraverseUiState(uint8_t state)
{	
    if(test_dis_ui_flg != state)
    {
        test_dis_ui_flg = state;
        if(test_dis_ui_flg == 1)
        {
            Gui_ThreadWakeUpThread();
			memset(&cpu,0,sizeof(cpu_param_t));
			Cpu_ScheduleTask(&cpu.task_tcb, &cpu.device_tcb, CPU_TASK_MAIN);//CPU_TASK_POWEROFF);//CPU_TASK_MAIN);//
            Gui_ThreadSetDisPlayArea(0, 0, 80, 160);
        }
        if(test_dis_ui_flg == 0)
        {
			memset(&cpu,0,sizeof(cpu_param_t));
            Gui_ThreadSetDisPlayArea(0, 0, 80, 160);
        }
    }
}

/****************************************************************
** Function name:           Gui_GetTraverseUiState
** Descriptions:            获取UI遍历状态
** input parameters:        state:状态
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-08-02
*****************************************************************/
uint8_t Gui_GetTraverseUiState(void)
{	
	return test_dis_ui_flg;
}

/****************************************************************
** Function name:           Gui_EventAndTaskTraverse
** Descriptions:            遍历切换界面参数
** input parameters:        state:进来切换的状态
** output parameters:       无 
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-08-02
*****************************************************************/
void Gui_EventAndTaskTraverse(uint8_t state)
{
    if(state == 1)
    {
        if(cpu.task_tcb.task_id > CPU_TASK_IDEL && cpu.task_tcb.task_id < CPU_TASK_POWEROFF)
        {
            cpu.task_tcb.task_id++;
            cpu.task_tcb.task_controller.init = 1;
            cpu.device_tcb.oled.light = 1;
            Gui_ThreadWakeUpThread();
        }
        else if(cpu.task_tcb.task_id == CPU_TASK_POWEROFF)
        {
            cpu.task_tcb.task_id = CPU_TASK_IDEL;
            cpu.event_tcb.event_id = CPU_EVENT_TASK_CHARGE;
            cpu.event_tcb.task_controller.init = 1;
            cpu.device_tcb.oled.light = 1;
            Gui_ThreadWakeUpThread();
        }
    }
    if(state == 2)
    {
        if(cpu.event_tcb.event_id >= CPU_EVENT_TASK_CHARGE && cpu.event_tcb.event_id < CPU_EVENT_TASK_HOURGLASS_TIMEOUT)
        {
            cpu.event_tcb.event_id++;
            cpu.event_tcb.task_controller.init = 1;
            cpu.device_tcb.oled.light = 1;
            Gui_ThreadWakeUpThread();
        }
        else if(cpu.event_tcb.event_id == CPU_EVENT_TASK_HOURGLASS_TIMEOUT)
        {
            Gui_SetTraverseUiState(0);
        }
    }
}


/****************************************************************
** Function name:           Cpu_LowPowerHintEventTest
** Descriptions:            低电显示
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-08-12
*****************************************************************/ 
void Cpu_LowPowerHintEventTest(cpu_task_tcb_t* p_task, cpu_event_tcb_t* p_event, cpu_device_tcb_t* p_device)
{
	if(p_event->task_controller.init == 1)
    {
        Cpu_EventParamInit(p_event,2,false,true);
		//清除背景
        Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
                                p_device->cls_param.column_size, p_device->cls_param.page_size);
    }
    
    if((p_event->gui_controller.animation_tick_counter%4) == 0)
    {
		//设置清屏参数
        Gui_ThreadSetDisPlayArea(0, 0, 80, 160);
		Gui_PageLowPower(p_event->gui_controller.animation_frame_index++);
    }
}

/****************************************************************
** Function name:           Cpu_IdelChargeEventTest
** Descriptions:            充电界面事件处理
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-08-12
*****************************************************************/ 
void Cpu_IdelChargeEventTest(cpu_task_tcb_t* p_task, cpu_event_tcb_t* p_event, cpu_device_tcb_t* p_device)
{
    if(p_event->task_controller.init == 1)
    {
        Cpu_EventParamInit(p_event,3,false,true); 
        //清除背景
        Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
                                p_device->cls_param.column_size, p_device->cls_param.page_size);
    }
    

    if(p_event->gui_controller.animation_tick_counter%6 == 0)
    {
        //设置清屏参数
        Gui_ThreadSetDisPlayArea(0, 0, 80, 160);
        Gui_PageBatteryCharg(p_event->gui_controller.animation_frame_index++, 0);
    }
}

/****************************************************************
** Function name:           Cpu_AlarmHintEventTest
** Descriptions:            闹钟界面事件处理
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-08-12
*****************************************************************/ 
void Cpu_AlarmHintEventTest(cpu_event_tcb_t* p_event, cpu_device_tcb_t* p_device)
{
    uint8_t alarm_hour = 0;
    uint8_t alarm_minute = 0;
    uint8_t alarm_mode = 0;
//    static uint8_t alarm_time = 0;
	
    if(p_event->task_controller.init == 1)
    {
        traverse_tmp_buf1 = 0;
        traverse_tmp_buf2 = 0;
        traverse_tmp_buf3 = 1;
        traverse_tmp_buf4 = 0;
//        alarm_time = 10;
        Cpu_EventParamInit(p_event,15,false,true);
        //清除背景
        Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
                                p_device->cls_param.column_size, p_device->cls_param.page_size);
    }
    
    if((p_event->gui_controller.animation_tick_counter%10) == 0)
    {
        if(traverse_tmp_buf2++ < 5)
        {
            alarm_mode = 0;
        }
        else
        {
            alarm_mode = traverse_tmp_buf4++ % 20 + 1;
//            alarm_time = 60;
        }
        if(alarm_mode == 1)
        {
            //清除背景图
            Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
                                    p_device->cls_param.column_size, p_device->cls_param.page_size);
        }
        alarm_hour = traverse_tmp_buf3 % 100;
        alarm_minute = traverse_tmp_buf3 % 100;
        traverse_tmp_buf3 += 22;
        display_language = traverse_tmp_buf1++ / 3;
        if(display_language > 1)
        {
            display_language = 1;
        }
		Gui_PageAlarm(traverse_tmp_buf1 % 3, alarm_mode, alarm_hour,alarm_minute,display_language,
		              0);
        //设置清屏参数
        Gui_ThreadSetDisPlayArea(0, 0, 80, 160);
    }
}

/****************************************************************
** Function name:           Cpu_SedentaryHintEventTest
** Descriptions:            久坐界面事件处理
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-08-12
*****************************************************************/ 
void Cpu_SedentaryHintEventTest(cpu_event_tcb_t* p_event, cpu_device_tcb_t* p_device)
{
    if(p_event->task_controller.init == 1)
    {
        Cpu_EventParamInit(p_event,1,false,true);
        //清除背景
        Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
                                p_device->cls_param.column_size, p_device->cls_param.page_size);
    }
    
    if(p_event->gui_controller.animation_tick_counter%6 == 0)
    {    
		Gui_PageSit(p_event->gui_controller.animation_frame_index++);
        //设置清屏参数
        Gui_ThreadSetDisPlayArea(0, 0, 80, 160);
    }
}

/****************************************************************
** Function name:           Cpu_CallHintEventTest
** Descriptions:            来电提醒界面事件处理
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-08-12
*****************************************************************/ 
void Cpu_CallHintEventTest(cpu_event_tcb_t* p_event, cpu_device_tcb_t* p_device)
{
    static uint8_t last_call_state = 0x00;
    uint8_t temp[15] = {0xE7,0xBB,0xB4,0xE4,0xBA,0xBF,0xE9,0xAD,0x84,0xE7,0xA7,0x91,0xE6,0x8A,0x80};
    
    if(p_event->task_controller.init == 1)
    {
        traverse_tmp_buf1 = 0;
        Cpu_EventParamInit(p_event,4,false,true);        
        //清除背景
        Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
                                p_device->cls_param.column_size, p_device->cls_param.page_size);
    }

    if((p_event->gui_controller.animation_tick_counter%10) == 0)
    {      
        last_call_state = 1;
        if(traverse_tmp_buf1 == 3)
        {
            last_call_state = 2;
        }
        if(traverse_tmp_buf1++ >= 4)
        {
            last_call_state = 3;
        }
		Gui_PageCallNotificationIcon(last_call_state,p_event->gui_controller.animation_frame_index++);
        Gui_PageCallNotificationContent(temp, 15);
        //设置清屏参数
        Gui_ThreadSetDisPlayArea(0, 0, 80, 160);
    }
}


/****************************************************************
** Function name:           Cpu_MessageHintEventTest
** Descriptions:            信息提醒界面事件处理
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-08-12
*****************************************************************/ 
void Cpu_MessageHintEventTest(cpu_event_tcb_t* p_event, cpu_device_tcb_t* p_device)
{
    uint8_t temp[15] = {0xE7,0xBB,0xB4,0xE4,0xBA,0xBF,0xE9,0xAD,0x84,0xE7,0xA7,0x91,0xE6,0x8A,0x80};
    if(p_event->task_controller.init == 1)
    {
        Cpu_EventParamInit(p_event,7,false,true);
        traverse_tmp_buf1 = 0;
        //清除背景
        Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
                                p_device->cls_param.column_size, p_device->cls_param.page_size);
    }
    
    if(p_event->gui_controller.animation_tick_counter%10 == 0)
    {
        //设置清屏参数
        Gui_ThreadSetDisPlayArea(0, 0, 80, 160);
		event_msg_type = traverse_tmp_buf1++ % 13 + 2;
		Gui_PageMessageNotificationIcon(event_msg_type,p_event->gui_controller.animation_frame_index++);
        Gui_PageMessageNotificationContent(temp, 15, 0);
    }
}

/****************************************************************
** Function name:           Cpu_HeartAlertHintEventTest
** Descriptions:            心里告警界面事件处理
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-08-12
*****************************************************************/ 
void Cpu_HeartAlertHintEventTest(cpu_event_tcb_t* p_event, cpu_device_tcb_t* p_device)
{
    if(p_event->task_controller.init == 1)
    {
        Cpu_EventParamInit(p_event,2,false,true);        
        //清除背景
        Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
                                p_device->cls_param.column_size, p_device->cls_param.page_size);
    }
    
    if(p_event->gui_controller.animation_tick_counter%6 == 0)
    {
		Gui_PageHeartAlert(p_event->gui_controller.animation_frame_index++);
        //设置清屏参数
        Gui_ThreadSetDisPlayArea(0, 0, 80, 160);
    }
}

/****************************************************************
** Function name:           Cpu_LowOxygenAlertHintEventTest
** Descriptions:            低氧告警界面事件处理
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-08-12
*****************************************************************/ 
void Cpu_LowOxygenAlertHintEventTest(cpu_event_tcb_t* p_event, cpu_device_tcb_t* p_device)
{
    if(p_event->task_controller.init == 1)
    {
        Cpu_EventParamInit(p_event,2,false,true);        
        //清除背景
        Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
                                p_device->cls_param.column_size, p_device->cls_param.page_size);
    }
    
    if(p_event->gui_controller.animation_tick_counter%6 == 0)
    {
        Gui_LowOxygenAlertPage(p_event->gui_controller.animation_frame_index++);
        //设置清屏参数
        Gui_ThreadSetDisPlayArea(0, 0, 80, 160);
    }
}

/****************************************************************
** Function name:           Cpu_FontTestEventTest
** Descriptions:            生产测试界面事件处理
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-08-12
*****************************************************************/ 
void Cpu_FontTestEventTest(cpu_task_tcb_t* p_task, cpu_event_tcb_t* p_event, cpu_device_tcb_t* p_device)
{
    if(p_event->task_controller.init == 1)
    {
        Cpu_EventParamInit(p_event,1,false,true);
        //清除背景
        Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
                                p_device->cls_param.column_size, p_device->cls_param.page_size);
    }
    
    if(p_event->gui_controller.animation_tick_counter%6 == 0)
    {
		Gui_PageFontTest();
        //设置清屏参数
        Gui_ThreadSetDisPlayArea(0, 0, 80, 160);
    }
}

/****************************************************************
** Function name:           Cpu_CameraEventTest
** Descriptions:            拍照准备事件
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-08-12
*****************************************************************/ 
void Cpu_CameraEventTest(cpu_event_tcb_t* p_event, cpu_device_tcb_t* p_device)
{	

    if(p_event->task_controller.init == 1)
    {
        Cpu_EventParamInit(p_event,2,false,true);
        traverse_tmp_buf1 = 0;
//        traverse_tmp_buf2 = 0;
        //清除背景
        Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
                                p_device->cls_param.column_size, p_device->cls_param.page_size);
    }
    
    if(p_event->gui_controller.animation_tick_counter%18 == 0)
    {
//        if(traverse_tmp_buf1 >= 2)
//        {
//            traverse_tmp_buf2 = 1;
//            traverse_tmp_buf1 = 0;
//        }
		Gui_PageCameraReady(++traverse_tmp_buf1%2);
        //设置清屏参数
        Gui_ThreadSetDisPlayArea(0, 0, 80, 160);
		
    }
}

/****************************************************************
** Function name:           Cpu_DeviceSportTargetEventTest
** Descriptions:            运动目标提醒界面
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-08-12
*****************************************************************/ 
void Cpu_DeviceSportTargetEventTest(cpu_task_tcb_t* p_task, cpu_event_tcb_t* p_event, cpu_device_tcb_t* p_device)
{
    uint32_t target_step_buf[5] = {1000, 8694, 87654, 9999999, 0};
    if(p_event->task_controller.init == 1)
    {
        Cpu_EventParamInit(p_event,3,false,true);
        //清除背景
        Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
                                p_device->cls_param.column_size, p_device->cls_param.page_size);
    }
    
    if(p_event->gui_controller.animation_tick_counter % 6 == 0 )//300ms 6
    {
		Gui_PageStarget(p_event->gui_controller.animation_frame_index++, target_step_buf[p_event->gui_controller.animation_frame_index%5], 0);
        //设置清屏参数
        Gui_ThreadSetDisPlayArea(0, 0, 80, 160);
    }	
}

/****************************************************************
** Function name:           Cpu_MainTaskTest
** Descriptions:            主界面处理
** input parameters:        无 
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-08-12
*****************************************************************/ 
void Cpu_MainTaskTest(cpu_task_tcb_t* p_task, cpu_device_tcb_t* p_device)
{
    uint8_t time_format = 0;
    uint8_t month = 0;
    uint8_t day = 0;
	uint8_t hour = 0;
 	uint8_t minute = 0;
    uint8_t week = 0;
    uint8_t battery_level = 0;
    uint8_t battery_charging = 0;
    uint8_t ble_con_state = 0;
    uint8_t ui_style_type_max = 7;
    
    uint32_t step_all[5] = {12, 345, 6789, 90123, 345670};

    if(p_task->task_controller.init == 1)
    {
		Cpu_TaskParamInit(p_task, 5*ui_style_type_max+5, false, true);
        traverse_tmp_buf1 = 0;
        traverse_tmp_buf2 = 0;
        traverse_tmp_buf3 = 0;
        traverse_tmp_buf4 = 0;
        ui_style_type = 0;
        display_language = 0;

        //清除背景
        Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
                                p_device->cls_param.column_size, p_device->cls_param.page_size);
    }
    
	if((p_task->gui_controller.animation_tick_counter%12 == 0))
    {
        /* UI遍历中 */
        minute = traverse_tmp_buf1 % 60;
        hour = traverse_tmp_buf1 % 24;
        traverse_tmp_buf1 += 11;
        week = ++traverse_tmp_buf2 % 7 + 1;
        time_format = traverse_tmp_buf2 % 2 + 1;
        ble_con_state = traverse_tmp_buf2 % 2;
        battery_level = traverse_tmp_buf2 % 8;
        if(traverse_tmp_buf2 >= 7)
        {
            display_language = 1;
        }
        else
        {
            display_language = 0;
        }
        month = day = traverse_tmp_buf3;
        traverse_tmp_buf3 += 11;
        
        if(p_task->gui_controller.animation_frame_index % 5 == 4)
        {
            ui_style_type = (++ui_style_type) % ui_style_type_max;
            p_task->gui_controller.animation_frame_index = 0;
        }

        if(p_task->gui_controller.animation_frame_index == 0)
        {        
            Gui_PageClearBackground(0, 0, 80, 160);
        }
        if(time_format == GUI_TIME_FORMAT_AM || time_format == GUI_TIME_FORMAT_PM )
        {
            hour = hour % 12;
            if(hour == 0)
            {
                hour = 12;
            }
        }

		Gui_PageMainDateHour(month, day, hour, minute, week, display_language, 
                             time_format, p_task->gui_controller.animation_frame_index, p_task->gui_controller.animation_frame_index * 50);
        Gui_PageMainAssisant(p_task->gui_controller.animation_frame_index, ble_con_state, time_format, 
                             battery_charging, battery_level, display_language, 
                             step_all[p_task->gui_controller.animation_frame_index % 5],p_task->gui_controller.animation_frame_index);
        p_task->gui_controller.animation_frame_index++;
        //设置清屏参数
        Gui_ThreadSetDisPlayArea(0, 0, 80, 160);
	}
}

/****************************************************************
** Function name:           Cpu_MacAddTest
** Descriptions:            MAC和版本号的显示
** input parameters:        无 
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-08-12
*****************************************************************/ 
void Cpu_MacAddTest(cpu_task_tcb_t* p_task, cpu_device_tcb_t* p_device)
{
    static uint8_t dis_mac_data[6] = {0};
    static uint8_t dis_ver_data[2] = {0};
    if(p_task->task_controller.init == 1)
    {
        Cpu_TaskParamInit(p_task, 1,false,true);
		Gui_DataGetBleMac(dis_mac_data);
        Gui_DataGetVer(dis_ver_data);
        //清除背景
        Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
                                p_device->cls_param.column_size, p_device->cls_param.page_size);
    }

    if(p_task->gui_controller.animation_tick_counter%6 == 0)
    {
		Gui_PageDisMacVer(p_task->gui_controller.animation_frame_index++, dis_mac_data, dis_ver_data);
		//设置清屏参数
        Gui_ThreadSetDisPlayArea(0, 0, 80, 160);
    }
}
/****************************************************************
** Function name:           Cpu_StepTaskTest
** Descriptions:            计步界面处理
** input parameters:        p_controller：UI事件控制状态；p_gui：显示相关的参数
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-08-12
*****************************************************************/ 
void Cpu_StepTaskTest(cpu_task_tcb_t* p_task, cpu_device_tcb_t* p_device)
{    
    uint32_t all_step = 0;
    uint32_t all_step_buf[5] = {0, 12, 345, 6789, 12345};
    uint8_t  sports_mark_buf = 0;
    
    if(p_task->task_controller.init == 1)
    {
        Cpu_TaskParamInit(p_task,4,false,true);
        traverse_tmp_buf1 = 0;
        display_language = 0;
        //清除背景
        Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
                                p_device->cls_param.column_size, p_device->cls_param.page_size);        
    }
        
    if(((p_task->gui_controller.animation_tick_counter%15) == 0))
    {
		
		//设置清屏参数
        Gui_ThreadSetDisPlayArea(0, 0, 80, 160);
        /* UI遍历中 */
        all_step = all_step_buf[traverse_tmp_buf1++];
        display_language = traverse_tmp_buf1 % 2;
        sports_mark_buf = traverse_tmp_buf1 * 20;
        if(traverse_tmp_buf1 >= 5)
        {
            traverse_tmp_buf1 = 0;
        }
		Gui_PageStep(all_step,sports_mark_buf,p_task->gui_controller.animation_frame_index++,display_language);
    } 
}

/****************************************************************
** Function name:           Cpu_DistanTaskTest
** Descriptions:            距离界面处理
** input parameters:        p_controller：UI事件控制状态；p_gui：显示相关的参数
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-08-12
*****************************************************************/ 
void Cpu_DistanTaskTest(cpu_task_tcb_t* p_task, cpu_device_tcb_t* p_device)
{    
    uint8_t distance_dis_unit = 0;
    float distance_km = 0.0f;
    float distance_mile = 0.0f;
    float all_step_buf[5] = {0, 1.2, 34, 567, 890};
    
    if(p_task->task_controller.init == 1)
    {
        Cpu_TaskParamInit(p_task,4,false,true);
        traverse_tmp_buf1 = 0;
        display_language = 0;
        //清除背景
        Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
                                p_device->cls_param.column_size, p_device->cls_param.page_size);        
    }
        
    if(((p_task->gui_controller.animation_tick_counter%15) == 0))
    {
        //设置清屏参数
        Gui_ThreadSetDisPlayArea(0, 0, 80, 160);
        /* UI遍历中 */
        distance_km = distance_mile = all_step_buf[traverse_tmp_buf1];
        distance_dis_unit = traverse_tmp_buf1++ % 2;
        display_language = (traverse_tmp_buf1 == 1)?0 : 1;
        if(traverse_tmp_buf1 >= 5)
        {
            traverse_tmp_buf1 = 0;
        }
        
        if(distance_dis_unit == 1 || display_language == 0)
        {
			Gui_PageDistance(distance_km,p_task->gui_controller.animation_frame_index++,0,display_language);
        }
        else
        {
			Gui_PageDistance(distance_mile,p_task->gui_controller.animation_frame_index++,1,display_language);
        }
    } 
}

/****************************************************************
** Function name:           Cpu_CalorTaskTest
** Descriptions:            卡路里界面处理
** input parameters:        p_controller：UI事件控制状态；p_gui：显示相关的参数
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-08-12
*****************************************************************/ 
void Cpu_CalorTaskTest(cpu_task_tcb_t* p_task, cpu_device_tcb_t* p_device)
{    
    uint16_t calorie_tmp_buf[4] = {0, 12, 345, 6789};
    
    if(p_task->task_controller.init == 1)
    {
        Cpu_TaskParamInit(p_task,4,false,true);
        traverse_tmp_buf1 = 0;
        display_language = 0;
        //清除背景
        Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
                                p_device->cls_param.column_size, p_device->cls_param.page_size);        
    }
        
    if(((p_task->gui_controller.animation_tick_counter%15) == 0))
    {
        //设置清屏参数
        Gui_ThreadSetDisPlayArea(0, 0, 80, 160);
		Gui_PageCalories(calorie_tmp_buf[traverse_tmp_buf1],p_task->gui_controller.animation_frame_index++,traverse_tmp_buf1 % 2);
        if(++traverse_tmp_buf1 >= 4)
        {
            traverse_tmp_buf1 = 0;
        }
    } 
}

/****************************************************************
** Function name:            Cpu_OxygenTaskTest
** Descriptions:             血氧界面
** input parameters:         p_controller：       CPU任务控制器指针
                             p_gui：              静态页面控制器指针
                             p_oled：             oled驱动指针
** output parameters:        无
** Returned value:           无
** Created by:               GT
** Created Date:             2017-07-10
****************************************************************/
void Cpu_OxygenTaskTest(cpu_task_tcb_t* p_task, cpu_device_tcb_t* p_device)
{   
	static uint8_t last_state = 0;
    uint8_t value_data[11] = {0,11,22,33,44,55,66,77,88,99,100};
    if(p_task->task_controller.init == 1)
    {
        Cpu_TaskParamInit(p_task,11,false,true);
        traverse_tmp_buf1 = 0;
        traverse_tmp_buf2 = 0;
        traverse_tmp_buf3 = 0;
        traverse_tmp_buf4 = 0;
        display_language = 0; 
        //清除背景
        Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
                                p_device->cls_param.column_size, p_device->cls_param.page_size);
    }

    // 血氧动画刷新(帧/0.3S)
    if(((p_task->gui_controller.animation_tick_counter%10) == 0))
    {   
		//设置清屏参数
        Gui_ThreadSetDisPlayArea(0, 0, 80, 160);
		if(traverse_tmp_buf1 < 11)
        {
            traverse_tmp_buf2 = 1;
        }
        else
        {
            traverse_tmp_buf2 = 2;
            if(last_state == 0)
            {
                last_state = 1;
                Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
                                        p_device->cls_param.column_size, p_device->cls_param.page_size);
            }
        }
        
        if(traverse_tmp_buf2 == 1)
        {
            Gui_OxygenCalibration(p_task->gui_controller.animation_frame_index++,
                                  value_data[traverse_tmp_buf1++],traverse_tmp_buf1%2);
        }
        else if(traverse_tmp_buf2 == 2)
        {
            Gui_OxygenPage(p_task->gui_controller.animation_frame_index++,value_data[traverse_tmp_buf1++%12], 100, 0);
        }
    }  
}

/****************************************************************
** Function name:           Cpu_TaskRunTest
** Descriptions:            运动界面处理
** input parameters:        p_controller：UI事件控制状态；p_gui：显示相关的参数
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-08-12
*****************************************************************/ 
void Cpu_TaskRunTest(cpu_task_tcb_t* p_task, movement_cont_t* p_movement, cpu_device_tcb_t* p_device)
{
    uint8_t movement_mode_busy = 0;
    static uint8_t last_state = 0;
    uint8_t  uint_type = 0;
    uint8_t  heart = 0;
    uint16_t seconds = 0;
    float calories = 0.0f;
    uint32_t all_step = 0;
    float    run_distance_tmp[5] = {0, 1.2, 34, 567, 890};
    uint32_t run_calories_tmp[5] = {0, 12, 345, 6789, 12345};
    uint32_t run_all_step_tmp[5] = {0, 12, 345, 6789, 12345};
    uint32_t distance_m = 0;
    
    if(p_task->task_controller.init == 1)
    {
        Cpu_TaskParamInit(p_task,13,false,true);
        traverse_tmp_buf1 = 0;
        traverse_tmp_buf2 = 0;
        traverse_tmp_buf3 = 0;
        traverse_tmp_buf4 = 0;
        display_language = 0; 
        //清除背景
        Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
                                p_device->cls_param.column_size, p_device->cls_param.page_size);
    }
    
    if((p_task->gui_controller.animation_tick_counter%11 == 0))
    {
        //设置清屏参数
        Gui_ThreadSetDisPlayArea(0, 0, 80, 160);
        if(traverse_tmp_buf4 >= 100)
        {
            movement_mode_busy = 2;
            if(traverse_tmp_buf1++ > 4)
            {
                movement_mode_busy = 3;
            }
            display_language = traverse_tmp_buf1%2;
        }
        else if(traverse_tmp_buf1++ >= 4)//12)
        {
            movement_mode_busy = 1;
            seconds = traverse_tmp_buf1;
            traverse_tmp_buf1 += 10 + 12*60 + 12*3600;
            heart = traverse_tmp_buf2;
            traverse_tmp_buf2 += 37;
            display_language = traverse_tmp_buf2 % 2;
            distance_m = run_distance_tmp[traverse_tmp_buf3];
            calories = run_calories_tmp[traverse_tmp_buf3];
            all_step = run_all_step_tmp[traverse_tmp_buf3];
            if(++traverse_tmp_buf3 >= 5)
            {
                traverse_tmp_buf3 = 0;
            }
            if(traverse_tmp_buf4++ >= 9)
            {
                traverse_tmp_buf1 = 0;
                traverse_tmp_buf2 = 0;
                traverse_tmp_buf3 = 0;
                traverse_tmp_buf4 = 100;
            }
        }
        if(movement_mode_busy == 1)
        {
            if(last_state == 0)
            {
                last_state = 1;
            }
            Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
                                    p_device->cls_param.column_size, p_device->cls_param.page_size);
            Gui_PageRunModeData(seconds, heart, distance_m, calories, all_step, uint_type, display_language);
            last_state = 1;
        }
        else if(movement_mode_busy == 2)
        {
            if(last_state == 1)
            {
                last_state = 0;
                Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
                                        p_device->cls_param.column_size, p_device->cls_param.page_size);
            }
            Gui_PageRunModePause(traverse_tmp_buf1/3,display_language);
        }
        else if(movement_mode_busy == 3)
        {
            Gui_PageRunModeExit(display_language);
        }
        else
        {
            if(last_state == 1)
            {
                last_state = 0;
                Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
                                        p_device->cls_param.column_size, p_device->cls_param.page_size);
            }
            //显示运动模式人物界面
			Gui_PageRunMode(p_task->gui_controller.animation_frame_index++,display_language);
        } 
    }
}

/****************************************************************
** Function name:           Cpu_HeartTaskTest
** Descriptions:            心率界面处理
** input parameters:        p_controller：UI事件控制状态；p_gui：显示相关的参数
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-08-12
*****************************************************************/ 
void Cpu_HeartTaskTest(cpu_task_tcb_t* p_task, cpu_device_tcb_t* p_device)
{
    static uint8_t heart = 0;
    
    if(p_task->task_controller.init == 1)
    {		
        Cpu_TaskParamInit(p_task,5,false,true);	        
		traverse_tmp_buf1 = 1;
        heart = 0;
        //清除背景
        Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
                                p_device->cls_param.column_size, p_device->cls_param.page_size);
    } 

    // 修改相关变量 
    if(((p_task->gui_controller.animation_tick_counter%9) == 0))
    {
		//设置清屏参数
        Gui_ThreadSetDisPlayArea(0, 0, 80, 160);
        heart = traverse_tmp_buf1;
        traverse_tmp_buf1 += 21;
        display_language = traverse_tmp_buf1%2;

		Gui_PageHeart(p_task->gui_controller.animation_frame_index++, heart,display_language);
    }
}

/****************************************************************
** Function name:           Cpu_SleepTaskTest
** Descriptions:            睡眠界面处理
** input parameters:        p_controller：UI事件控制状态；p_gui：显示相关的参数
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-08-12
*****************************************************************/ 
void Cpu_SleepTaskTest(cpu_task_tcb_t* p_task, cpu_device_tcb_t* p_device)
{
    if(p_task->task_controller.init == 1)
    {
        Cpu_TaskParamInit(p_task,5,false,true);
        traverse_tmp_buf1 = 60;
        traverse_tmp_buf2 = 0;
        //清除背景
        Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
                                p_device->cls_param.column_size, p_device->cls_param.page_size);
    }    

    if(((p_task->gui_controller.animation_tick_counter%9) == 0))
    {
		//设置清屏参数
        Gui_ThreadSetDisPlayArea(0, 0, 80, 160);
		Gui_PageSleep(p_task->gui_controller.animation_frame_index++,traverse_tmp_buf1,traverse_tmp_buf2++ % 2);
        traverse_tmp_buf1 += 60 * 11 + 6 ;
    }
}

/****************************************************************
** Function name:           Cpu_FindPhoneTaskTest
** Descriptions:            找手机界面处理
** input parameters:        p_controller：UI事件控制状态；p_gui：显示相关的参数
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-08-12
*****************************************************************/ 
void Cpu_FindPhoneTaskTest(cpu_task_tcb_t* p_task, cpu_device_tcb_t* p_device)
{
    if(p_task->task_controller.init == 1)
    {
        Cpu_TaskParamInit(p_task,2,false,true);
        //清除背景
        Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
                                p_device->cls_param.column_size, p_device->cls_param.page_size);
    }
        
    if((p_task->gui_controller.animation_tick_counter%6) == 0)
    {
        //设置清屏参数
        Gui_ThreadSetDisPlayArea(0, 0, 80, 160);
        Gui_PageFindPhone(1,p_task->gui_controller.animation_frame_index++);	
    } 
}

/****************************************************************
** Function name:           Cpu_PowerOffTaskTest
** Descriptions:            关机界面处理
** input parameters:        p_controller：UI事件控制状态；p_gui：显示相关的参数
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-08-12
*****************************************************************/ 
void Cpu_PowerOffTaskTest(cpu_task_tcb_t* p_task, cpu_event_tcb_t *p_event, cpu_device_tcb_t* p_device)
{
    if(p_task->task_controller.init == 1)
    {
        Cpu_TaskParamInit(p_task,1,false,true);//false);
        //清除背景
        Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
                                p_device->cls_param.column_size, p_device->cls_param.page_size);
    }
        
    if((p_task->gui_controller.animation_tick_counter%6 == 0))
    {
		//设置清屏参数
        Gui_ThreadSetDisPlayArea(0, 0, 80, 160);
		Gui_PagePower();
    }      
}

/****************************************************************
** Function name:           Cpu_TimerTaskTest
** Descriptions:            秒表界面处理
** input parameters:        p_controller：UI事件控制状态；p_gui：显示相关的参数
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-08-12
*****************************************************************/ 
void Cpu_TimerTaskTest(cpu_task_tcb_t* p_task, watch_timer_t *p_timer, cpu_device_tcb_t* p_device)
{
    static uint8_t last_state = 0;
    uint8_t millisecond = 0;
    uint16_t total_second = 0;
    
    if(p_task->task_controller.init == 1)
    {
        Cpu_TaskParamInit(p_task,6,false,true);
        traverse_tmp_buf1 = 0;
        traverse_tmp_buf2 = 0;
        traverse_tmp_buf3 = 1;
        traverse_tmp_buf4 = 0;
        //清除背景
        Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
                                p_device->cls_param.column_size, p_device->cls_param.page_size);	    
    }

    if((p_task->gui_controller.animation_tick_counter%15 == 0))
    {
        //设置清屏参数
        Gui_ThreadSetDisPlayArea(0, 0, 80, 160);
        last_state = 0;
        traverse_tmp_buf1++;
        if(traverse_tmp_buf1 > 2)
        {
            last_state = 1;
            millisecond = traverse_tmp_buf2++;
            total_second = traverse_tmp_buf3;
            traverse_tmp_buf3 += 22 + 11*60 ;
            if(traverse_tmp_buf4++ == 0)
            {
                //清除背景图
                Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
                                        p_device->cls_param.column_size, p_device->cls_param.page_size);
                p_task->gui_controller.animation_frame_index = 0;
            }
        }
        GUI_StopWatchPage(last_state, total_second, millisecond, p_task->gui_controller.animation_frame_index++);
    }
}

/****************************************************************
** Function name:           Cpu_HourglassTaskTest
** Descriptions:            倒计时界面处理
** input parameters:        p_controller：UI事件控制状态；p_gui：显示相关的参数
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-08-12
*****************************************************************/ 
void Cpu_HourglassTaskTest(cpu_task_tcb_t* p_task, cpu_device_tcb_t* p_device)
{
    if(p_task->task_controller.init == 1)
    {
        Cpu_TaskParamInit(p_task,4,false,true);
        //清除背景
        Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
                                p_device->cls_param.column_size, p_device->cls_param.page_size);
    }
        
    if(p_task->gui_controller.animation_tick_counter%7 == 0)
    {
		//设置清屏参数
        Gui_ThreadSetDisPlayArea(0, 0, 80, 160);
        
        Gui_PageCountDown(true,p_task->gui_controller.animation_frame_index++);
    }  
}

/****************************************************************
** Function name:           Gui_ThreadTestManage
** Descriptions:            主任务事件界面处理
** input parameters:        无
** output parameters:       无 
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-08-02
*****************************************************************/ 
void Gui_ThreadTestManage(void)
{
    switch(cpu.task_tcb.task_id)
	{
        case CPU_TASK_MAIN:
            Cpu_MainTaskTest(&cpu.task_tcb,&cpu.device_tcb);
            break;
		case CPU_TASK_MAC:
			Cpu_MacAddTest(&cpu.task_tcb,&cpu.device_tcb);
			break;
        case CPU_TASK_MOVEMENT:
			Cpu_TaskRunTest(&cpu.task_tcb,&cpu.movement,&cpu.device_tcb);
            break;
        case CPU_TASK_STEP:
            Cpu_StepTaskTest(&cpu.task_tcb,&cpu.device_tcb);
            break;
		case CPU_TASK_DISTAN:
            Cpu_DistanTaskTest(&cpu.task_tcb,&cpu.device_tcb);
            break;
		case CPU_TASK_CALOR:
            Cpu_CalorTaskTest(&cpu.task_tcb,&cpu.device_tcb);
            break;
//		case CPU_TASK_DRINK:
//            Cpu_DrinkTaskTest(&cpu.task_tcb,&cpu.device_tcb);
//            break;
        case CPU_TASK_OXYGEN:
            Cpu_OxygenTaskTest(&cpu.task_tcb,&cpu.device_tcb);
            break;
        case CPU_TASK_HEART:
            Cpu_HeartTaskTest(&cpu.task_tcb,&cpu.device_tcb);
            break;
        case CPU_TASK_SLEEP:
            Cpu_SleepTaskTest(&cpu.task_tcb,&cpu.device_tcb);
            break;
        case CPU_TASK_FINDPHONE:
            Cpu_FindPhoneTaskTest(&cpu.task_tcb,&cpu.device_tcb);
            break;
        case CPU_TASK_TIMER:
            Cpu_TimerTaskTest(&cpu.task_tcb,&cpu.watch,&cpu.device_tcb);
            break;
        case CPU_TASK_HOURGLASS:
            Cpu_HourglassTaskTest(&cpu.task_tcb,&cpu.device_tcb);
            break;
        case CPU_TASK_POWEROFF:
            Cpu_PowerOffTaskTest(&cpu.task_tcb,&cpu.event_tcb,&cpu.device_tcb);
            break;
        default:
            cpu.task_tcb.task_id = CPU_TASK_IDEL;
            break;
	}
    
    switch(cpu.event_tcb.event_id)
	{
        case CPU_EVENT_TASK_CHARGE:
            Cpu_IdelChargeEventTest(&cpu.task_tcb,&cpu.event_tcb,&cpu.device_tcb);
            break;
        case CPU_EVENT_TASK_ALARM:
            Cpu_AlarmHintEventTest(&cpu.event_tcb,&cpu.device_tcb);
            break;
        case CPU_EVENT_TASK_SIT:
            Cpu_SedentaryHintEventTest(&cpu.event_tcb,&cpu.device_tcb);
            break;
        case CPU_EVENT_TASK_CALL:
            Cpu_CallHintEventTest(&cpu.event_tcb,&cpu.device_tcb);
            break;
        case CPU_EVENT_TASK_MESSAGE:
            Cpu_MessageHintEventTest(&cpu.event_tcb,&cpu.device_tcb);
            break;
        case CPU_EVENT_TASK_HEARTALERT:
            Cpu_HeartAlertHintEventTest(&cpu.event_tcb,&cpu.device_tcb);
            break;
        case CPU_EVENT_TASK_LOWOXYGENALERT:
            Cpu_LowOxygenAlertHintEventTest(&cpu.event_tcb,&cpu.device_tcb);
            break;
        case CPU_EVENT_TASK_CAMERA:
            Cpu_CameraEventTest(&cpu.event_tcb,&cpu.device_tcb);
            break;
        case CPU_EVENT_TASK_FONTTEST:
            Cpu_FontTestEventTest(&cpu.task_tcb,&cpu.event_tcb,&cpu.device_tcb);
            break;
        case CPU_EVENT_TASK_LOWPOWER:
            Cpu_LowPowerHintEventTest(&cpu.task_tcb,&cpu.event_tcb,&cpu.device_tcb);
            break;
        case CPU_EVENT_TASK_HOURGLASS_HINT:
            break;
        case CPU_EVENT_TASK_HOURGLASS_TIMEOUT:
            break;
		case CPU_EVENT_TASK_SPORT_TARGET:
			Cpu_DeviceSportTargetEventTest(&cpu.task_tcb,&cpu.event_tcb,&cpu.device_tcb);
			break;
        default:
            cpu.event_tcb.event_id = CPU_EVENT_TASK_IDEL;
            break;
	}
}


