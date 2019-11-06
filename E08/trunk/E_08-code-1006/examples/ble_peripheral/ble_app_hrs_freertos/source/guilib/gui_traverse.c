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
#include "Ble_Thread.h"
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
extern uint8_t runmode_show_type;   //运动模式显示类型，为2页显示还是3页显示
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
//             
        }
        if(test_dis_ui_flg == 0)
        {
			memset(&cpu,0,sizeof(cpu_param_t));
//             
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
        if(cpu.event_tcb.event_id >= CPU_EVENT_TASK_CHARGE && cpu.event_tcb.event_id < CPU_EVENT_TASK_BLE_DISC_REMIND)
        {
            cpu.event_tcb.event_id++;
            cpu.event_tcb.task_controller.init = 1;
            cpu.device_tcb.oled.light = 1;
            Gui_ThreadWakeUpThread();
        }
        else if(cpu.event_tcb.event_id == CPU_EVENT_TASK_BLE_DISC_REMIND)
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
//		//清除背景
//        Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
//                                p_device->cls_param.column_size, p_device->cls_param.page_size);
    }
    
    if((p_event->gui_controller.animation_tick_counter%4) == 0)
    {
		 
//         
		Gui_PageLowPower(p_event->gui_controller.animation_frame_index++, &p_device->cls_flag);
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
//        //清除背景
//        Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
//                                p_device->cls_param.column_size, p_device->cls_param.page_size);
    }
    

    if(p_event->gui_controller.animation_tick_counter%6 == 0)
    {
         
//         
        Gui_PageBatteryCharg(p_event->gui_controller.animation_frame_index++, 0, &p_device->cls_flag);
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
//        //清除背景
//        Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
//                                p_device->cls_param.column_size, p_device->cls_param.page_size);
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
//            //清除背景图
//            Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
//                                    p_device->cls_param.column_size, p_device->cls_param.page_size);
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
		              0, &p_device->cls_flag);
         
//         
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
//        //清除背景
//        Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
//                                p_device->cls_param.column_size, p_device->cls_param.page_size);
    }
    
    if(p_event->gui_controller.animation_tick_counter%6 == 0)
    {    
		Gui_PageSit(p_event->gui_controller.animation_frame_index++, p_event->gui_controller.animation_frame_index% 2, &p_device->cls_flag);
         
//         
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
//        //清除背景
//        Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
//                                p_device->cls_param.column_size, p_device->cls_param.page_size);
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
		Gui_PageCallNotificationIcon(p_event->gui_controller.animation_frame_index, last_call_state, p_event->gui_controller.animation_frame_index%2, &p_device->cls_flag);
        Gui_PageCallNotificationContent(p_event->gui_controller.animation_frame_index++,temp, 15, &p_device->cls_flag);
         
//         
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
//        //清除背景
//        Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
//                                p_device->cls_param.column_size, p_device->cls_param.page_size);
    }
    
    if(p_event->gui_controller.animation_tick_counter%10 == 0)
    {
         
//         
		event_msg_type = traverse_tmp_buf1++ % 13 + 2;
		Gui_PageMessageNotificationIcon(p_event->gui_controller.animation_frame_index, event_msg_type, 0, 23, 59, &p_device->cls_flag);
        Gui_PageMessageNotificationContent(p_event->gui_controller.animation_frame_index++, temp, 15, &p_device->cls_flag);
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
//        //清除背景
//        Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
//                                p_device->cls_param.column_size, p_device->cls_param.page_size);
    }
    
    if(p_event->gui_controller.animation_tick_counter%6 == 0)
    {
		Gui_PageHeartAlert(p_event->gui_controller.animation_frame_index++, &p_device->cls_flag);
         
//         
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
//        //清除背景
//        Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
//                                p_device->cls_param.column_size, p_device->cls_param.page_size);
    }
    
    if(p_event->gui_controller.animation_tick_counter%6 == 0)
    {
		Gui_PageFontTest(p_event->gui_controller.animation_frame_index++, &p_device->cls_flag);
         
//         
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
//        //清除背景
//        Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
//                                p_device->cls_param.column_size, p_device->cls_param.page_size);
    }
    
    if(p_event->gui_controller.animation_tick_counter%18 == 0)
    {
//        if(traverse_tmp_buf1 >= 2)
//        {
//            traverse_tmp_buf2 = 1;
//            traverse_tmp_buf1 = 0;
//        }
		Gui_PageCameraReady(traverse_tmp_buf1, ++traverse_tmp_buf1%2, &p_device->cls_flag);
         
//         
		
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
//        //清除背景
//        Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
//                                p_device->cls_param.column_size, p_device->cls_param.page_size);
    }
    
    if(p_event->gui_controller.animation_tick_counter % 6 == 0 )//300ms 6
    {
		Gui_PageStarget(p_event->gui_controller.animation_frame_index++, target_step_buf[0], p_event->gui_controller.animation_frame_index%2, &p_device->cls_flag);
         
//         
    }	
}


/****************************************************************
** Function name:           Cpu_BleDiscRemindEventTest
** Descriptions:            蓝牙断连事件
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-08-12
*****************************************************************/ 
void Cpu_BleDiscRemindEventTest(cpu_task_tcb_t* p_task, cpu_event_tcb_t* p_event, cpu_device_tcb_t* p_device)
{
    if(p_event->task_controller.init == 1)
    {
        Cpu_EventParamInit(p_event,2,false,true);
//        //清除背景
//        Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
//                                p_device->cls_param.column_size, p_device->cls_param.page_size);
    }
    
    if(p_event->gui_controller.animation_tick_counter % 10 == 0 )//500ms 10
    {
		Gui_PageBleDiscRemind(p_event->gui_controller.animation_frame_index++, &p_device->cls_flag);
         
//         
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
    uint8_t ui_style_type_max = 4;
    
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

//        //清除背景
//        Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
//                                p_device->cls_param.column_size, p_device->cls_param.page_size);
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
        battery_level = traverse_tmp_buf2 % 5;
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
    
        Gui_PageMain(p_task->gui_controller.animation_frame_index, month, day, hour, minute, 0, week,
                         time_format, step_all[p_task->gui_controller.animation_frame_index % 5], 0, ble_con_state, battery_charging,
                         battery_level, 0, 0, 100, 0, display_language, &p_device->cls_flag);
//		Gui_PageMainDateHour(p_task->gui_controller.animation_frame_index, month, day, hour, minute, week, display_language, time_format, p_task->gui_controller.animation_frame_index);
//        Gui_PageMainAssisant(p_task->gui_controller.animation_frame_index, step_all[p_task->gui_controller.animation_frame_index % 5],ble_con_state,
//                             time_format, battery_charging, battery_level, display_language, 
//                             p_task->gui_controller.animation_frame_index);
        p_task->gui_controller.animation_frame_index++;
         
//         
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
//        //清除背景
//        Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
//                                p_device->cls_param.column_size, p_device->cls_param.page_size);
    }

    if(p_task->gui_controller.animation_tick_counter%6 == 0)
    {
		Gui_PageDisMacVer(p_task->gui_controller.animation_frame_index++, dis_mac_data, dis_ver_data, 0, &p_device->cls_flag);
		 
//         
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
//        //清除背景
//        Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
//                                p_device->cls_param.column_size, p_device->cls_param.page_size);        
    }
        
    if(((p_task->gui_controller.animation_tick_counter%15) == 0))
    {
		
		 
//         
        /* UI遍历中 */
        all_step = all_step_buf[traverse_tmp_buf1++];
        display_language = traverse_tmp_buf1 % 2;
        sports_mark_buf = traverse_tmp_buf1 * 20;
        if(traverse_tmp_buf1 >= 5)
        {
            traverse_tmp_buf1 = 0;
        }
		Gui_PageStep(all_step,sports_mark_buf,p_task->gui_controller.animation_frame_index++, 23, 59,display_language, &p_device->cls_flag,0,0,0);
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
//        //清除背景
//        Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
//                                p_device->cls_param.column_size, p_device->cls_param.page_size);        
    }
        
    if(((p_task->gui_controller.animation_tick_counter%15) == 0))
    {
         
//         
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
			Gui_PageDistance(distance_km,p_task->gui_controller.animation_frame_index++, 0, 23, 59, display_language, &p_device->cls_flag);
        }
        else
        {
			Gui_PageDistance(distance_mile,p_task->gui_controller.animation_frame_index++, 1, 23, 59, display_language, &p_device->cls_flag);
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
//        //清除背景
//        Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
//                                p_device->cls_param.column_size, p_device->cls_param.page_size);        
    }
        
    if(((p_task->gui_controller.animation_tick_counter%15) == 0))
    {
         
//         
		Gui_PageCalories(calorie_tmp_buf[traverse_tmp_buf1],p_task->gui_controller.animation_frame_index++, 23, 59,traverse_tmp_buf1 % 2, &p_device->cls_flag);
        if(++traverse_tmp_buf1 >= 4)
        {
            traverse_tmp_buf1 = 0;
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
    uint8_t  heart = 0;
    uint16_t seconds = 0;
    uint32_t all_step = 0;
    uint32_t tdistance_m = 0;
    uint32_t calories = 0;
    float    run_distance_tmp[5] = {0, 1.2, 34, 567, 890};
    uint32_t run_calories_tmp[5] = {0, 12, 345, 6789, 12345};
    uint32_t run_all_step_tmp[5] = {0, 12, 345, 6789, 12345};
    uint8_t hour = 0;
    if(p_task->task_controller.init == 1)
    {
        Cpu_TaskParamInit(p_task,13,false,true);
        traverse_tmp_buf1 = 0;
        traverse_tmp_buf2 = 0;
        traverse_tmp_buf3 = 0;
        traverse_tmp_buf4 = 0;
        display_language = 0; 
//        //清除背景
//        Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
//                                p_device->cls_param.column_size, p_device->cls_param.page_size);
    }
    
    if((p_task->gui_controller.animation_tick_counter%11 == 0))
    {
         
//         
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
            tdistance_m = run_distance_tmp[traverse_tmp_buf3];
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
//                Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
//                                    p_device->cls_param.column_size, p_device->cls_param.page_size);
                p_task->gui_controller.animation_frame_index = 0;
            }
            
            Gui_PageRunModeData(p_task->gui_controller.animation_frame_index++, seconds, heart, tdistance_m, calories, 
                                        all_step, traverse_tmp_buf1%2, 1, display_language, &p_device->cls_flag, 
                                runmode_show_type,p_movement->face_num,1, hour, clock->minutes,0,0,0);
        }
        else if(movement_mode_busy == 2)
        {
            if(last_state == 1)
            {
                last_state = 0;
//                Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
//                                        p_device->cls_param.column_size, p_device->cls_param.page_size);
            }
            Gui_PageRunModePause(p_task->gui_controller.animation_frame_index++, traverse_tmp_buf1/3,display_language, &p_device->cls_flag);
        }
        else if(movement_mode_busy == 3)
        {
            Gui_PageRunModeExit(p_task->gui_controller.animation_frame_index++, display_language, &p_device->cls_flag);
        }
        else
        {
            if(last_state == 1)
            {
                last_state = 0;
//                Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
//                                        p_device->cls_param.column_size, p_device->cls_param.page_size);
            }
            //显示运动模式人物界面
			Gui_PageRunMode(p_task->gui_controller.animation_frame_index++, 23, 59,display_language, &p_device->cls_flag);
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
//        Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
//                                p_device->cls_param.column_size, p_device->cls_param.page_size);
    } 

    // 修改相关变量 
    if(((p_task->gui_controller.animation_tick_counter%9) == 0))
    {
		 
//         
        heart = traverse_tmp_buf1;
        traverse_tmp_buf1 += 21;
        display_language = traverse_tmp_buf1%2;

		Gui_PageHeart(p_task->gui_controller.animation_frame_index++, heart, heart, 23, 59, display_language, &p_device->cls_flag);
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
//        //清除背景
//        Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
//                                p_device->cls_param.column_size, p_device->cls_param.page_size);
    }    

    if(((p_task->gui_controller.animation_tick_counter%9) == 0))
    {
		 
//         
		Gui_PageSleep(p_task->gui_controller.animation_frame_index++,traverse_tmp_buf1, 23, 59,traverse_tmp_buf2++ % 2, &p_device->cls_flag,0,0,0);
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
//        //清除背景
//        Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
//                                p_device->cls_param.column_size, p_device->cls_param.page_size);
    }
        
    if((p_task->gui_controller.animation_tick_counter%6) == 0)
    {
         
//         
        Gui_PageFindPhone(1,p_task->gui_controller.animation_frame_index++, 23, 59, p_task->gui_controller.animation_frame_index %2, &p_device->cls_flag);	
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
    static uint8_t dis_mac_data[6] = {0};
    static uint8_t dis_ver_data[2] = {0};
    
    if(p_task->task_controller.init == 1)
    {
        Gui_DataGetBleMac(dis_mac_data);
        Gui_DataGetVer(dis_ver_data);
        Cpu_TaskParamInit(p_task,1,false,true);//false);
//        //清除背景
//        Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
//                                p_device->cls_param.column_size, p_device->cls_param.page_size);
    }
        
    if((p_task->gui_controller.animation_tick_counter%6 == 0))
    { 
        traverse_tmp_buf1++;
        display_language = traverse_tmp_buf1%2;
		Gui_PagePower(p_task->gui_controller.animation_frame_index++, dis_mac_data, dis_ver_data, 23, 59, display_language, &p_device->cls_flag,(uint8_t *)DEVICE_NAME);
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
//        //清除背景
//        Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
//                                p_device->cls_param.column_size, p_device->cls_param.page_size);	    
    }

    if((p_task->gui_controller.animation_tick_counter%15 == 0))
    {
         
//         
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
//                //清除背景图
//                Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
//                                        p_device->cls_param.column_size, p_device->cls_param.page_size);
                p_task->gui_controller.animation_frame_index = 0;
            }
        }
        GUI_StopWatchPage(p_task->gui_controller.animation_frame_index++, last_state, last_state, total_second, 
                          millisecond, 23, 59, p_task->gui_controller.animation_frame_index % 2, &p_device->cls_flag);
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
//        //清除背景
//        Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
//                                p_device->cls_param.column_size, p_device->cls_param.page_size);
    }
        
    if(p_task->gui_controller.animation_tick_counter%7 == 0)
    {
		 
//         
        
        Gui_PageCountDown(p_task->gui_controller.animation_frame_index++, true, 100, 23, 59, p_task->gui_controller.animation_frame_index%2, &p_device->cls_flag,0);
    }  
}

/****************************************************************
** Function name:            Cpu_EcgTaskTest
** Descriptions:             Ecg面
** input parameters:         
** output parameters:        无
** Returned value:           无
** Created by:               WJ
** Created Date:             2018-12-06
****************************************************************/
void Cpu_EcgTaskTest(cpu_task_tcb_t* p_task, cpu_device_tcb_t* p_device)
{
    if(p_task->task_controller.init == 1)
    {								
        Cpu_TaskParamInit(p_task, 5, false, true);	      
//        //清除背景
//        Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
//                                p_device->cls_param.column_size, p_device->cls_param.page_size);
    } 
    // 修改相关变量 
    if((p_task->gui_controller.animation_tick_counter%3) == 0)
    {
        traverse_tmp_buf1++;
        display_language = traverse_tmp_buf1%2;
		 
//         
        Gui_PageEcg(p_task->gui_controller.animation_frame_index++, 23, 59, display_language, &p_device->cls_flag);
    }   
}


/****************************************************************
** Function name:            Cpu_EcgItem1TaskTest
** Descriptions:             ECG界面-子界面1
** input parameters:         
** output parameters:        无
** Returned value:           无
** Created by:               WJ
** Created Date:             2018-11-09
****************************************************************/
void Cpu_EcgItem1TaskTest(cpu_task_tcb_t* p_task, cpu_device_tcb_t* p_device)
{
    if(p_task->task_controller.init == 1)
    {								
        Cpu_TaskParamInit(p_task, 8, false, true);	      
//        //清除背景
//        Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
//                                p_device->cls_param.column_size, p_device->cls_param.page_size);
    } 
 
    // 修改相关变量 
    if(((p_task->gui_controller.animation_tick_counter%2) == 0))
    {
		 
//         
        traverse_tmp_buf1++;
        display_language = traverse_tmp_buf1%2;
  
        if(p_task->task_controller.run_time < 4)
        {
            Gui_PageEcgItem1(p_task->gui_controller.animation_frame_index++, 0, p_task->gui_controller.animation_frame_index, display_language, &p_device->cls_flag);
        }
        else
        {
            Gui_PageEcgItem1(p_task->gui_controller.animation_frame_index++, 1, p_task->gui_controller.animation_frame_index, display_language, &p_device->cls_flag);
        }
    } 
}
/****************************************************************
** Function name:            Cpu_EcgItem2TaskTest
** Descriptions:             ECG界面-子界面2
** input parameters:         
** output parameters:        无
** Returned value:           无
** Created by:               WJ
** Created Date:             2018-11-09
****************************************************************/
void Cpu_EcgItem2TaskTest(cpu_task_tcb_t* p_task, cpu_device_tcb_t* p_device)
{
    if(p_task->task_controller.init == 1)
    {			
        Cpu_TaskParamInit(p_task, 6, false, true);	      
//        //清除背景
//        Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
//                                p_device->cls_param.column_size, p_device->cls_param.page_size);
    } 

    // 修改相关变量 
    if(((p_task->gui_controller.animation_tick_counter%2) == 0))
    {
		 
//         
        traverse_tmp_buf1++;
        display_language = traverse_tmp_buf1%2;
     
        if(p_task->task_controller.run_time < 3)
        {
            Gui_PageEcgItem2(p_task->gui_controller.animation_frame_index++, 0, display_language, &p_device->cls_flag);
        }
        else
        {
            Gui_PageEcgItem2(p_task->gui_controller.animation_frame_index++, 1, display_language, &p_device->cls_flag);
        }
    }  
}

/****************************************************************
** Function name:            Cpu_PttTaskTest
** Descriptions:             Ptt界面
** input parameters:         
** output parameters:        无
** Returned value:           无
** Created by:               WJ
** Created Date:             2018-11-09
****************************************************************/
void Cpu_PttTaskTest(cpu_task_tcb_t* p_task, cpu_device_tcb_t* p_device)
{
    if(p_task->task_controller.init == 1)
    {								
        Cpu_TaskParamInit(p_task, 5, false, true);	      
//        //清除背景
//        Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
//                                p_device->cls_param.column_size, p_device->cls_param.page_size);
    } 
    // 修改相关变量 
    if(((p_task->gui_controller.animation_tick_counter%3) == 0))
    {
		 
//         
        traverse_tmp_buf1++;
        display_language = traverse_tmp_buf1%2;
     
        Gui_PagePtt(p_task->gui_controller.animation_frame_index++, &p_device->cls_flag);
    } 
}

/****************************************************************
** Function name:            Cpu_PttItemTaskTest
** Descriptions:             Ptt界面-子界面1
** input parameters:         
** output parameters:        无
** Returned value:           无
** Created by:               WJ
** Created Date:             2018-11-09
****************************************************************/
void Cpu_PttItemTaskTest(cpu_task_tcb_t* p_task, cpu_device_tcb_t* p_device)
{
    if(p_task->task_controller.init == 1)
    {								
        Cpu_TaskParamInit(p_task, 5, false, true);	      
//        //清除背景
//        Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
//                                p_device->cls_param.column_size, p_device->cls_param.page_size);
    } 
    // 修改相关变量 
    if(((p_task->gui_controller.animation_tick_counter%3) == 0))
    {
		 
        traverse_tmp_buf1++;
        display_language = traverse_tmp_buf1%2;
     
        Gui_PagePttItem1(p_task->gui_controller.animation_frame_index++, 0, &p_device->cls_flag);
    } 
}

/****************************************************************
** Function name:            Cpu_BpTaskTest
** Descriptions:             血压界面
** input parameters:         
** output parameters:        无
** Returned value:           无
** Created by:               WJ
** Created Date:             2018-11-09
****************************************************************/
void Cpu_BpTaskTest(cpu_task_tcb_t* p_task, cpu_device_tcb_t* p_device)
{
    if(p_task->task_controller.init == 1)
    {		
		Cpu_TaskParamInit(p_task, 8, false, true);	
//        //清除背景
//        Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
//                                p_device->cls_param.column_size, p_device->cls_param.page_size);
    }   

    // 修改相关变量 
    if(((p_task->gui_controller.animation_tick_counter%2) == 0))
    {
		 
//          
        traverse_tmp_buf1++;
        display_language = traverse_tmp_buf1%2;    
        if(p_task->task_controller.run_time < 3)
        {
            Gui_PageBP(p_task->gui_controller.animation_frame_index++, 0, 0, 
                       p_task->gui_controller.animation_frame_index%2, 23, 59, 0, &p_device->cls_flag,display_language);
        }
        else
        {
            Gui_PageBP(p_task->gui_controller.animation_frame_index++, 
                       p_task->gui_controller.animation_frame_index * 27, 
                       p_task->gui_controller.animation_frame_index * 15, 
                       p_task->gui_controller.animation_frame_index%2, 23, 59, 1, &p_device->cls_flag,display_language);
        }
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
        //任务--主界面
        case CPU_TASK_MAIN:
            Cpu_MainTaskTest(&cpu.task_tcb,&cpu.device_tcb);
            break;
        //任务--MAC和版本号
		case CPU_TASK_MAC:
			Cpu_MacAddTest(&cpu.task_tcb,&cpu.device_tcb);
			break;
        case CPU_TASK_MOVEMENT:
			Cpu_TaskRunTest(&cpu.task_tcb,&cpu.movement,&cpu.device_tcb);
            break;
        case CPU_TASK_STEP:
            Cpu_StepTaskTest(&cpu.task_tcb,&cpu.device_tcb);
            break;
//////		case CPU_TASK_DISTAN:
//////            Cpu_DistanTaskTest(&cpu.task_tcb,&cpu.device_tcb);
//////            break;
//////		case CPU_TASK_CALOR:
//////            Cpu_CalorTaskTest(&cpu.task_tcb,&cpu.device_tcb);
//////            break;
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
        case CPU_TASK_ECG:
            Cpu_EcgTaskTest(&cpu.task_tcb,&cpu.device_tcb);
            break;
        case CPU_TASK_ECG_ITEM1:
            Cpu_EcgItem1TaskTest(&cpu.task_tcb,&cpu.device_tcb);
            break;
        case CPU_TASK_ECG_ITEM2:
            Cpu_EcgItem2TaskTest(&cpu.task_tcb,&cpu.device_tcb);
            break;
        //case CPU_TASK_PTT:
        //    Cpu_PttTaskTest(&cpu.task_tcb,&cpu.device_tcb);
        //    break;
        //case CPU_TASK_PTT_ITEM1:
        //    Cpu_PttItemTaskTest(&cpu.task_tcb,&cpu.device_tcb);
        //    break;
        case CPU_TASK_BP:
            Cpu_BpTaskTest(&cpu.task_tcb,&cpu.device_tcb);
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
//        case CPU_EVENT_TASK_PHYS_REMIND:
//            Cpu_PhysEventTest(&cpu.event_tcb,&cpu.device_tcb);
//            break;
        case CPU_EVENT_TASK_MESSAGE:
            Cpu_MessageHintEventTest(&cpu.event_tcb,&cpu.device_tcb);
            break;
        case CPU_EVENT_TASK_HEARTALERT:
            Cpu_HeartAlertHintEventTest(&cpu.event_tcb,&cpu.device_tcb);
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
        case CPU_EVENT_TASK_BLE_DISC_REMIND:
            Cpu_BleDiscRemindEventTest(&cpu.task_tcb,&cpu.event_tcb,&cpu.device_tcb);
            break;
        default:
            cpu.event_tcb.event_id = CPU_EVENT_TASK_IDEL;
            break;
	}
}


