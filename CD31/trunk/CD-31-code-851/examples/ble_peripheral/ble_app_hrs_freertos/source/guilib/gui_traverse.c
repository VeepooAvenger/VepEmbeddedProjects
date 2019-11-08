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

uint8_t test_dis_ui_flg = 0;                                 //开启UI遍历的标志 0：未开启  1：已开启
static uint32_t traverse_tmp_buf1 = 0;                       //UI遍历的参数1
static uint32_t traverse_tmp_buf2 = 0;                       //UI遍历的参数2
static uint32_t traverse_tmp_buf3 = 0;                       //UI遍历的参数3
static uint32_t traverse_tmp_buf4 = 0;                       //UI遍历的参数4

static uint8_t evnet_interval_cnt = 0xFF;                    //最后的间隔次数，用于判断是否刷新UI
static uint8_t task_interval_cnt = 0xFF;                     //最后的间隔次数，用于判断是否刷新UI和动图切换
static uint8_t event_msg_type = 0;

extern cpu_task_t cpu_s;
extern gui_thread_resource_t  gui_res_s;
extern uint8_t ui_style_type;

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
            cpu_s.task = CPU_TASK_MAIN;
            cpu_s.job_control.init = 1;
            cpu_s.oled.oled_drive_light = 1;
            Gui_ThreadWakeUpThread(); 
        }
        if(test_dis_ui_flg == 0)
        {
            cpu_s.task = CPU_TASK_IDEL;
            cpu_s.job_control.init = 0;
            cpu_s.event = CPU_EVENT_TASK_IDEL;
            cpu_s.event_control.init = 0;
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
        if(cpu_s.task > CPU_EVENT_TASK_IDEL && cpu_s.task < CPU_TASK_TASK_MAX_ID - 1)
        {
            cpu_s.task++;
            cpu_s.job_control.init = 1;
            cpu_s.oled.oled_drive_light = 1;
            Gui_ThreadWakeUpThread();
        }
        else if(cpu_s.task == CPU_TASK_TASK_MAX_ID - 1)
        {
            cpu_s.task = CPU_TASK_IDEL;
            cpu_s.event = CPU_EVENT_TASK_CHARGE;
            cpu_s.event_control.init = 1;
            cpu_s.oled.oled_drive_light = 1;
            Gui_ThreadWakeUpThread();
        }
    }
    if(state == 2)
    {
        if(cpu_s.event >= CPU_EVENT_TASK_CHARGE && cpu_s.event < CPU_EVENT_TASK_BLE_DISC_REMIND)
        {
            cpu_s.event++;
            if(cpu_s.event == CPU_EVENT_TASK_COUNT_DOWN_TIMEOUT)
            {
                cpu_s.event += 2;
            }
            cpu_s.event_control.init = 1;
            cpu_s.oled.oled_drive_light = 1;
            Gui_ThreadWakeUpThread();
        }
        else if(cpu_s.event == CPU_EVENT_TASK_BLE_DISC_REMIND)
        {
            Gui_SetTraverseUiState(0);
        }
    }
}


/****************************************************************
** Function name:           Cpu_EventLowPowerHintTest
** Descriptions:            低电显示
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-08-12
*****************************************************************/ 
void Cpu_EventLowPowerHintTest(cpu_task_controller_t *p_controller, gui_controller_t *p_gui)
{	
	if(p_controller->init == 1)
	{ 
        TASK_ENTRY_INIT(p_controller, 2); 
		//!注意:此处变量设置是为了立即刷新动画,否则可能会延时
        evnet_interval_cnt = 0xFF;
		p_gui->page_frame_counter = 0;
	}
    
    //300ms的刷新间隔
    p_gui->updata_ui_interval = 300;
    
	if(evnet_interval_cnt != p_gui->page_frame_counter)
	{	
        evnet_interval_cnt = p_gui->page_frame_counter;
        Gui_PageLowPower(evnet_interval_cnt);
    }
}

/****************************************************************
** Function name:           Cpu_EventIdelCharge
** Descriptions:            充电界面事件处理
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-08-12
*****************************************************************/ 
void Cpu_EventIdelChargeTest(cpu_task_controller_t *p_controller, gui_controller_t *p_gui)
{		
	if(p_controller->init == 1)
	{ 
        TASK_ENTRY_INIT(p_controller, 7); 
        
		//!注意:此处变量设置是为了立即刷新动画,否则可能会延时
        evnet_interval_cnt = 0xFF;
		p_gui->page_frame_counter = 0;
        
        //清除背景图
        		
	}
    
    //300ms的刷新间隔
    p_gui->updata_ui_interval = 150; 

	if(evnet_interval_cnt != p_gui->page_frame_counter)
	{	
        evnet_interval_cnt = p_gui->page_frame_counter;	
        Gui_PageBatteryCharg(evnet_interval_cnt, evnet_interval_cnt / 40, evnet_interval_cnt / 8); 
    }
}

/****************************************************************
** Function name:           Cpu_EventAlarm
** Descriptions:            闹钟界面事件处理
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-08-12
*****************************************************************/ 
void Cpu_EventAlarmTest(cpu_task_controller_t *p_controller, gui_controller_t *p_gui)
{
    uint8_t alarm_hour = 0;
    uint8_t alarm_minute = 0;
    uint8_t alarm_mode = 0;
    
	if(p_controller->init == 1)
	{ 
        TASK_ENTRY_INIT(p_controller, 15);
        traverse_tmp_buf1 = 0;
        traverse_tmp_buf2 = 0;
        traverse_tmp_buf3 = 1;
        traverse_tmp_buf4 = 0; 
		
		//!注意:此处变量设置是为了立即刷新动画,否则可能会延时
        evnet_interval_cnt = 0xFF;
		p_gui->page_frame_counter = 0;
	} 
        //499ms的刷新间隔
        p_gui->updata_ui_interval = 500; 
    
	if(evnet_interval_cnt != p_gui->page_frame_counter)
	{
        evnet_interval_cnt = p_gui->page_frame_counter; 
        if(traverse_tmp_buf2++ < 5)
        {
            alarm_mode = 0;
        }
        else
        {
            evnet_interval_cnt = 0;
            alarm_mode = traverse_tmp_buf4++ % 20 + 1;
        }
        alarm_hour = traverse_tmp_buf3 % 100;
        alarm_minute = traverse_tmp_buf3 % 100;
        traverse_tmp_buf3 += 22;
        display_language = traverse_tmp_buf1++ / 3;

        display_language++;
        display_language %= 2;
        
        Gui_PageAlarm(evnet_interval_cnt, evnet_interval_cnt % 3, alarm_mode, alarm_hour, alarm_minute, display_language);
	}
}

/****************************************************************
** Function name:           Cpu_EventSedentary
** Descriptions:            久坐界面事件处理
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-08-12
*****************************************************************/ 
void Cpu_EventSedentaryTest(cpu_task_controller_t *p_controller, gui_controller_t *p_gui)
{
	if(p_controller->init == 1)
	{ 
            TASK_ENTRY_INIT(p_controller, 1); 

		//!注意:此处变量设置是为了立即刷新动画,否则可能会延时
        evnet_interval_cnt = 0xFF;
		p_gui->page_frame_counter = 0; 
	}
	
    //300ms的刷新间隔
    p_gui->updata_ui_interval = 300;
    
	if(evnet_interval_cnt != p_gui->page_frame_counter)
	{
        evnet_interval_cnt = p_gui->page_frame_counter;
		Gui_PageSit(evnet_interval_cnt, evnet_interval_cnt % 2);
	}
}

/****************************************************************
** Function name:           Cpu_EventMessageRemind
** Descriptions:            来电提醒界面事件处理
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-08-12
*****************************************************************/ 
void Cpu_EventCallRemindTest(cpu_task_controller_t *p_controller, gui_controller_t *p_gui)
{
    static uint8_t last_call_state = 0x00; 
    
	if(p_controller->init == 1)
	{ 
            TASK_ENTRY_INIT(p_controller, 4);
            traverse_tmp_buf1 = 0;
 
		//!注意:此处变量设置是为了立即刷新动画,否则可能会延时
        evnet_interval_cnt = 0xFF;
		p_gui->page_frame_counter = 0;         
	} 
        //250ms的刷新间隔
        p_gui->updata_ui_interval = 790;
 
    
	if(evnet_interval_cnt != p_gui->page_frame_counter)
	{ 
        evnet_interval_cnt = p_gui->page_frame_counter;       
 
        last_call_state = 1;
        if(traverse_tmp_buf1 == 3)
        {
            last_call_state = 2;
        }
        if(traverse_tmp_buf1++ >= 4)
        {
            last_call_state = 3;
        }
        evnet_interval_cnt = 0;
        Gui_PageCallNotificationIcon(evnet_interval_cnt, last_call_state, evnet_interval_cnt%2);
	}
}

/****************************************************************
** Function name:           Cpu_EventMessageRemind
** Descriptions:            信息提醒界面事件处理
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-08-12
*****************************************************************/ 
void Cpu_EventMessageRemindTest(cpu_task_controller_t *p_controller, gui_controller_t *p_gui)
{
	if(p_controller->init == 1)
	{ 
        TASK_ENTRY_INIT(p_controller, 9);
        traverse_tmp_buf1 = 0; 
        
		//!注意:此处变量设置是为了立即刷新动画,否则可能会延时
        evnet_interval_cnt = 0xFF;
		p_gui->page_frame_counter = 0;
        
        //清除背景图
        	      
	} 
        //400ms的刷新间隔
    p_gui->updata_ui_interval = 500; 
    
	if(evnet_interval_cnt != p_gui->page_frame_counter)
	{ 
        evnet_interval_cnt = p_gui->page_frame_counter; 
        event_msg_type = traverse_tmp_buf1++ % 14 + 2;
        evnet_interval_cnt = 0;
        
        Gui_PageMessageNotificationIcon(evnet_interval_cnt, event_msg_type, 0, evnet_interval_cnt% 24, evnet_interval_cnt*3%60, evnet_interval_cnt%2); 
	}
}

/****************************************************************
** Function name:           Cpu_EventHeartAlert
** Descriptions:            心里告警界面事件处理
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-08-12
*****************************************************************/ 
void Cpu_EventHeartAlertTest(cpu_task_controller_t *p_controller, gui_controller_t *p_gui)
{  
	if(p_controller->init == 1)
	{ 
            TASK_ENTRY_INIT(p_controller, 2); 
		//!注意:此处变量设置是为了立即刷新动画,否则可能会延时
        evnet_interval_cnt = 0xFF;
		p_gui->page_frame_counter = 0;         
	}
	
    //200ms的刷新间隔
    p_gui->updata_ui_interval = 200;
    
	if(evnet_interval_cnt != p_gui->page_frame_counter)
	{
        evnet_interval_cnt = p_gui->page_frame_counter;
		Gui_PageHeartAlert(evnet_interval_cnt);
	}
}

/****************************************************************
** Function name:           Cpu_EventFontTest
** Descriptions:            生产测试界面事件处理
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-08-12
*****************************************************************/ 
void Cpu_EventFontTestTest(cpu_task_controller_t *p_controller, gui_controller_t *p_gui)
{
    if(p_controller->init == 1)
	{ 
            TASK_ENTRY_INIT(p_controller, 1); 
		//!注意:此处变量设置是为了立即刷新动画,否则可能会延时
        evnet_interval_cnt = 0xFF;
		p_gui->page_frame_counter = 0;
	}
	
    //300ms的刷新间隔
    p_gui->updata_ui_interval = 300;
    
	if(evnet_interval_cnt != p_gui->page_frame_counter)
	{
        evnet_interval_cnt = p_gui->page_frame_counter;
		Gui_PageFontTest(evnet_interval_cnt);
	}
}

/****************************************************************
** Function name:           Cpu_EventCameraReady
** Descriptions:            照相就绪界面事件处理
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-08-12
*****************************************************************/ 
void Cpu_EventCameraReadyTest(cpu_task_controller_t *p_controller, gui_controller_t *p_gui)
{
	if(p_controller->init == 1)
	{ 
            TASK_ENTRY_INIT(p_controller, 1);
            p_controller->key_press_hold = 0; 
		
		//!注意:此处变量设置是为了立即刷新动画,否则可能会延时
		evnet_interval_cnt = 0xFF;
		p_gui->page_frame_counter = 0;
	}
	
    //300ms的刷新间隔
    p_gui->updata_ui_interval = 300;
    
	if(evnet_interval_cnt != p_gui->page_frame_counter)
	{
        evnet_interval_cnt = p_gui->page_frame_counter;
		Gui_PageCameraReady(evnet_interval_cnt);
	}
}

/****************************************************************
** Function name:           Cpu_EventCameraOK
** Descriptions:            照相完成界面事件处理
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-08-12
*****************************************************************/ 
void Cpu_EventCameraOKTest(cpu_task_controller_t *p_controller, gui_controller_t *p_gui)
{
	if(p_controller->init == 1)
	{
		TASK_ENTRY_INIT(p_controller, 1);
		 
		//!注意:此处变量设置是为了立即刷新动画,否则可能会延时
		evnet_interval_cnt = 0xFF;
		p_gui->page_frame_counter = 0;
	}
	
    //300ms的刷新间隔
    p_gui->updata_ui_interval = 300;   
	if(evnet_interval_cnt != p_gui->page_frame_counter)
	{
        evnet_interval_cnt = p_gui->page_frame_counter;
        Gui_PageCameraOk(evnet_interval_cnt);
	}
}

/****************************************************************
** Function name:           Cpu_EvenMacVer
** Descriptions:            显示MAC地址和版本号
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-08-12
*****************************************************************/ 
void Cpu_EvenDisMacVerTest(cpu_task_controller_t *p_controller, gui_controller_t *p_gui)
{
    uint8_t dis_mac_data[20] = {0};
    uint8_t dis_ver_data[10] = {0};
	if(p_controller->init == 1)
	{ 
            TASK_ENTRY_INIT(p_controller, 2);  
		//!注意:此处变量设置是为了立即刷新动画,否则可能会延时
		evnet_interval_cnt = 0xFF;
		p_gui->page_frame_counter = 0;
		Gui_DataGetBleMac(dis_mac_data);
        Gui_DataGetVer(dis_ver_data);
	}
	
    //300ms的刷新间隔
    p_gui->updata_ui_interval = 300;   
	if(evnet_interval_cnt != p_gui->page_frame_counter)
	{
        evnet_interval_cnt = p_gui->page_frame_counter;
        Gui_PageDisMacVer(evnet_interval_cnt, dis_mac_data, dis_ver_data);
	}
}

/****************************************************************
** Function name:           Cpu_EvenMacVer
** Descriptions:            显示MAC地址和版本号
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-08-12
*****************************************************************/ 
void Cpu_TaskRunButtonTest(cpu_task_controller_t *p_controller, gui_controller_t *p_gui)
{
    uint8_t hour[6] = {01,12,34,56,78,90};
	if(p_controller->init == 1)
	{ 
        TASK_ENTRY_INIT(p_controller, 5);  
		//!注意:此处变量设置是为了立即刷新动画,否则可能会延时
		evnet_interval_cnt = 0xFF;
		p_gui->page_frame_counter = 0;
	}
	
    //300ms的刷新间隔
    p_gui->updata_ui_interval = 500;   
	if(evnet_interval_cnt != p_gui->page_frame_counter)
	{
        evnet_interval_cnt = p_gui->page_frame_counter;
        
        Gui_PageRunModeOperate(evnet_interval_cnt, (evnet_interval_cnt % 4 > 1 ? 1:0), evnet_interval_cnt % 5,
                               hour[evnet_interval_cnt % 6], hour[evnet_interval_cnt % 6], (evnet_interval_cnt % 4 > 1 ? 1:0));
	}
}

/****************************************************************
** Function name:           Cpu_EvenMacVer
** Descriptions:            显示MAC地址和版本号
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-08-12
*****************************************************************/ 
void Cpu_TaskRunDataTest(cpu_task_controller_t *p_controller, gui_controller_t *p_gui)
{
    uint32_t seconds = 0;
    uint8_t hour[6] = {01,12,34,56,78,90};
    uint32_t all_step_buf[5] = {0, 12, 345, 6789, 12345};
    uint8_t  heart_buff[8] = {12,34,56,78,90,198,176,254};
    float distance_buff[5] = {12.3, 45.6,78.9,100.0,0};
    
	if(p_controller->init == 1)
	{ 
        TASK_ENTRY_INIT(p_controller, 5);  
		//!注意:此处变量设置是为了立即刷新动画,否则可能会延时
		evnet_interval_cnt = 0xFF;
		p_gui->page_frame_counter = 0;
	}
	
    //300ms的刷新间隔
    p_gui->updata_ui_interval = 500;   
	if(evnet_interval_cnt != p_gui->page_frame_counter)
	{
        evnet_interval_cnt = p_gui->page_frame_counter;
        seconds = evnet_interval_cnt + evnet_interval_cnt * 60;
        
        Gui_PageRunModeData(evnet_interval_cnt, seconds, heart_buff[evnet_interval_cnt % 8], distance_buff[evnet_interval_cnt % 5], 
                            all_step_buf[evnet_interval_cnt % 5], all_step_buf[evnet_interval_cnt % 5], 
                            (evnet_interval_cnt % 4 > 1?1:0), (evnet_interval_cnt % 4 > 1?1:0), hour[evnet_interval_cnt % 6], 
                            hour[evnet_interval_cnt % 6], 1,evnet_interval_cnt % 2);
	}
}
/****************************************************************
** Function name:           Cpu_EvenStarget
** Descriptions:            运动目标提醒界面
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-08-12
*****************************************************************/ 
void Cpu_EvenStargetTest(cpu_task_controller_t *p_controller, gui_controller_t *p_gui)
{
	if(p_controller->init == 1)
	{ 
        TASK_ENTRY_INIT(p_controller, 5);  
		
		//!注意:此处变量设置是为了立即刷新动画,否则可能会延时
		evnet_interval_cnt = 0xFF;
		p_gui->page_frame_counter = 0;  
	}
	
    //300ms的刷新间隔
    p_gui->updata_ui_interval = 300;
    
	if(evnet_interval_cnt != p_gui->page_frame_counter)
	{
        evnet_interval_cnt = p_gui->page_frame_counter;
        Gui_PageStarget(evnet_interval_cnt,evnet_interval_cnt * 100, evnet_interval_cnt > 15);
    }
}

/****************************************************************
** Function name:           Cpu_TaskMain
** Descriptions:            主界面处理
** input parameters:        p_controller：UI事件控制状态；p_gui：显示相关的参数
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-08-12
*****************************************************************/ 
void Cpu_TaskMainTest(cpu_task_controller_t *p_controller, gui_controller_t *p_gui)
{
	uint8_t time_format = 0;
//    uint8_t month = 0;
//    uint8_t day = 0;
//	uint8_t hour = 0;
// 	uint8_t minute = 0;
    uint8_t week = 0;
    uint8_t battery_level = 0;
    uint8_t battery_charging = 0;
    uint8_t ble_con_state = 0;
    uint32_t all_step_buf[5] = {0, 12, 345, 6789, 12345};
    uint8_t  heart_buff[8] = {12,34,56,78,90,198,176,254};
	uint8_t data[15] = {12,34,56,78,90,0,1,2,3,4,5,6,7,8,9};
    
	if(p_controller->init == 1)
	{
        /* UI遍历中 */
        TASK_ENTRY_INIT(p_controller, 40);
        p_controller->key_press_hold = 0;
        traverse_tmp_buf1 = 0;
        traverse_tmp_buf2 = 0;
        traverse_tmp_buf3 = 0;
        display_language = 0;
		
		//!注意:此处变量设置是为了立即刷新动画,否则可能会延时
        task_interval_cnt = 0xFF;
		p_gui->page_frame_counter = 0;
        
        ui_style_type = 0;
		
	}
 
        //480ms的刷新间隔
        p_gui->updata_ui_interval = 500;
   
	if(task_interval_cnt != p_gui->page_frame_counter)
	{
        task_interval_cnt = p_gui->page_frame_counter;

        /* UI遍历中 */
//        minute = hour = traverse_tmp_buf1 % 60;
        traverse_tmp_buf1 += 11;
        week = traverse_tmp_buf2++ % 7 + 1;
        time_format = traverse_tmp_buf2 % 2 + 1;
        ble_con_state = traverse_tmp_buf2 % 2;
        battery_charging = traverse_tmp_buf2 % 2;
        battery_level = task_interval_cnt % 5;
//        month = day = traverse_tmp_buf3;
        traverse_tmp_buf3 += 11;
        if(task_interval_cnt % 15 == 14)
        {
            task_interval_cnt = 0;
            ui_style_type++;
            ui_style_type %= FUNCTION_SUPPORT_UI_STYLE;
        }
        
        Gui_PageMain(task_interval_cnt, data[task_interval_cnt % 15], data[task_interval_cnt % 15],
					data[task_interval_cnt % 15], data[task_interval_cnt % 15], 0, week, time_format, 
                     all_step_buf[task_interval_cnt % 5], heart_buff[task_interval_cnt % 8],
                     ble_con_state, battery_charging, battery_level, task_interval_cnt % 5,
                     all_step_buf[task_interval_cnt % 5], all_step_buf[task_interval_cnt % 5],
                    task_interval_cnt * 20, (task_interval_cnt % 4 > 2 ? 1:0));
        
        if(week == 7)
        {
            display_language++;
            display_language %= 2;
        }
	}
}

/****************************************************************
** Function name:           Cpu_TaskStep
** Descriptions:            计步界面处理
** input parameters:        p_controller：UI事件控制状态；p_gui：显示相关的参数
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-08-12
*****************************************************************/ 
void Cpu_TaskStepTest(cpu_task_controller_t *p_controller, gui_controller_t *p_gui)
{	
//    uint32_t all_step = 0;
    uint32_t all_step_buf[5] = {0, 12, 345, 6789, 12345};
//    uint8_t  sports_mark_buf = 0;
//    uint32_t distance_m = 0;
//    float distance_km = 0.0f;
//    float distance_mile = 0.0f;
    float distance_buff[5] = {12.3, 45.6,78.9,100.0,0};
    
	if(p_controller->init == 1)
	{ 
        /* UI遍历中 */
        TASK_ENTRY_INIT(p_controller, 6);
        traverse_tmp_buf1 = 0;
        display_language = 0; 
        p_controller->key_press_hold = 0;

        //!注意:此处变量设置是为了立即刷新动画,否则可能会延时
        task_interval_cnt = 0xFF;
        p_gui->page_frame_counter = 0;
		
	}
		 
    //480ms的刷新间隔
    p_gui->updata_ui_interval = 500; 
    
	if(task_interval_cnt != p_gui->page_frame_counter)
	{
        task_interval_cnt = p_gui->page_frame_counter;  

        if(traverse_tmp_buf1 >= 5)
        {
            traverse_tmp_buf1 = 0;
        }  
        
        if(task_interval_cnt % 3 == 2)
        {
            display_language++;
            display_language %= 2;
        }
        
        Gui_PageStepDistanceCalories(task_interval_cnt, all_step_buf[task_interval_cnt %5], task_interval_cnt * 20,
                              distance_buff[task_interval_cnt %5], task_interval_cnt % 2,
                              all_step_buf[task_interval_cnt %5], (task_interval_cnt % 4 > 2 ? 1:0));
	}      
}

/****************************************************************
** Function name:           Cpu_TaskDistance
** Descriptions:            距离界面处理
** input parameters:        p_controller：UI事件控制状态；p_gui：显示相关的参数
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-08-12
*****************************************************************/ 
void Cpu_TaskDistanceTest(cpu_task_controller_t *p_controller, gui_controller_t *p_gui)
{	
//    uint8_t distance_dis_unit = 0;
//    float distance_km = 0.0f;
//    float distance_mile = 0.0f;
//    float all_step_buf[5] = {0, 1.2, 34, 567, 890};
//    
//	if(p_controller->init == 1)
//	{ 
//            /* UI遍历中 */
//            TASK_ENTRY_INIT(p_controller, 4);
//            traverse_tmp_buf1 = 0;
//            display_language = 0; 
//		p_controller->key_press_hold = 0;
//		
//		//!注意:此处变量设置是为了立即刷新动画,否则可能会延时
//        task_interval_cnt = 0xFF;
//		p_gui->page_frame_counter = 0;
//		
//		//清除背景图		
//		
//	} 
//        //480ms的刷新间隔
//        p_gui->updata_ui_interval = 790; 
//    
//	if(task_interval_cnt != p_gui->page_frame_counter)
//	{
//        //Gui_ThreadSetDisPlayArea(0, 0, 80, 160);
//        task_interval_cnt = p_gui->page_frame_counter; 
//            /* UI遍历中 */
//            distance_km = distance_mile = all_step_buf[traverse_tmp_buf1];
//            distance_dis_unit = traverse_tmp_buf1++ % 2;
//            display_language = (traverse_tmp_buf1 == 1)?0 : 1;
//            if(traverse_tmp_buf1 >= 5)
//            {
//                traverse_tmp_buf1 = 0;
//            }
// 
//        if(distance_dis_unit == 1 || display_language == 0)
//        {
//            Gui_PageDistance(distance_km, task_interval_cnt, 0, display_language);
//        }
//        else
//        {
//            Gui_PageDistance(distance_mile, task_interval_cnt, 1, display_language);
//        }
//        
//	}      
}

/****************************************************************
** Function name:           Cpu_TaskCalories
** Descriptions:            卡路里界面处理
** input parameters:        p_controller：UI事件控制状态；p_gui：显示相关的参数
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-08-12
*****************************************************************/ 
void Cpu_TaskCaloriesTest(cpu_task_controller_t *p_controller, gui_controller_t *p_gui)
{	   
//    uint16_t calorie_tmp_buf[4] = {0, 12, 345, 6789};
//	if(p_controller->init == 1)
//	{ 
//            /* UI遍历中 */
//            TASK_ENTRY_INIT(p_controller, 4);
//            traverse_tmp_buf1 = 0;
//            display_language = 0;
// 
//		p_controller->key_press_hold = 0;
//		
//		//!注意:此处变量设置是为了立即刷新动画,否则可能会延时
//        task_interval_cnt = 0xFF;
//		p_gui->page_frame_counter = 0;
//		
//		//清除背景图		
//		
//	}
// 
//        //480ms的刷新间隔
//        p_gui->updata_ui_interval = 790;
// 
//	if(task_interval_cnt != p_gui->page_frame_counter)
//	{
//        //Gui_ThreadSetDisPlayArea(0, 0, 80, 160);
//        task_interval_cnt = p_gui->page_frame_counter;
// 
//            /* UI遍历中 */
//            Gui_PageCalories(calorie_tmp_buf[traverse_tmp_buf1], task_interval_cnt, traverse_tmp_buf1 % 2);
//            if(++traverse_tmp_buf1 >= 4)
//            {
//                traverse_tmp_buf1 = 0;
//            } 
//	}      
}

/****************************************************************
** Function name:           Cpu_TaskRun
** Descriptions:            运动界面处理
** input parameters:        p_controller：UI事件控制状态；p_gui：显示相关的参数
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-08-12
*****************************************************************/ 
void Cpu_TaskRunTest(cpu_task_controller_t *p_controller, gui_controller_t *p_gui)
{
    uint8_t movement_mode_busy = 0;
    static uint8_t last_state = 0;
    uint8_t  uint_type = 0;
    uint8_t  heart = 0;
    uint16_t seconds = 0;
    float calories = 0.0f;
    float distance_m = 0.0f;
    uint32_t all_step = 0;

    float distance_buff[5] = {12.3, 45.6,78.9,100.0,0};
    uint32_t run_calories_tmp[5] = {0, 12, 345, 6789, 12345};
    uint32_t run_all_step_tmp[5] = {0, 12, 345, 6789, 12345};
    
	if(p_controller->init == 1)
	{ 
            /* UI遍历中 */
            TASK_ENTRY_INIT(p_controller, 13);
            p_controller->key_press_hold = 0;
            traverse_tmp_buf1 = 0;
            traverse_tmp_buf2 = 0;
            traverse_tmp_buf3 = 0;
            traverse_tmp_buf4 = 0;
            display_language = 0; 

		//!注意:此处变量设置是为了立即刷新动画,否则可能会延时
        task_interval_cnt = 0xFF;
		p_gui->page_frame_counter = 0;
        last_state = 0;
	} 
 
        //480ms的刷新间隔
        p_gui->updata_ui_interval = 500;
 
	if(task_interval_cnt != p_gui->page_frame_counter)
	{
        task_interval_cnt = p_gui->page_frame_counter; 
        if(traverse_tmp_buf1++ >= 12)
        {
            movement_mode_busy = 1;
            seconds = traverse_tmp_buf1;
            traverse_tmp_buf1 += 10 + 12*60 + 12*3600;
            heart = traverse_tmp_buf2;
            traverse_tmp_buf2 += 37;
            distance_m = distance_buff[traverse_tmp_buf3];
            calories = run_calories_tmp[traverse_tmp_buf3];
            all_step = run_all_step_tmp[traverse_tmp_buf3];
            if(++traverse_tmp_buf3 >= 5)
            {
                traverse_tmp_buf3 = 0;
                uint_type++;
                uint_type %= 2;
            }
        }
        
        uint_type = traverse_tmp_buf2 % 2;
        display_language++;
        display_language %= 2;

        if(movement_mode_busy == 1)
        {            
            if(last_state == 0)
            {
                last_state = 1;
                task_interval_cnt = 0;
            }
            Gui_PageRunModeData(task_interval_cnt, seconds, heart, distance_m, calories, all_step, uint_type,
                                task_interval_cnt %2, traverse_tmp_buf1, traverse_tmp_buf1, 1,display_language);
            last_state = 1;
        }
        else            
        {
            if(last_state == 1)
            {
                last_state = 0;
                
            }
            Gui_PageRunMode(task_interval_cnt, display_language);
        }        
	}    
}

/****************************************************************
** Function name:           Cpu_TaskBP
** Descriptions:            血压界面处理
** input parameters:        p_controller：UI事件控制状态；p_gui：显示相关的参数
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-08-12
*****************************************************************/ 
void Cpu_TaskBPTest(cpu_task_controller_t *p_controller, gui_controller_t *p_gui)
{
    static uint8_t bp_sp = 0;
    static uint8_t bp_dp = 0;
    static uint8_t test_mode = 0;
    static uint8_t test_state = 0;
    static uint8_t last_state = 0xFF;
 
	if(p_controller->init == 1)
	{ 
            /* UI遍历中 */
            TASK_ENTRY_INIT(p_controller, 10);
            traverse_tmp_buf1 = 0;
            traverse_tmp_buf2 = 0;
            traverse_tmp_buf3 = 0;
 
		p_controller->key_press_hold = 0;

		//!注意:此处变量设置是为了立即刷新动画,否则可能会延时
        task_interval_cnt = 0xFF;      
		p_gui->page_frame_counter = 0;

		//清除背景图		
		
        bp_sp = 0;
        bp_dp = 0;
        test_mode = 0;
        test_state = 0;
	} 
 
        //480ms的刷新间隔
        p_gui->updata_ui_interval = 500;
    
	if(task_interval_cnt != p_gui->page_frame_counter)
	{
        traverse_tmp_buf1++;
        if(traverse_tmp_buf1 <= 1)
        {
            test_state = 0;
        }
        else if(traverse_tmp_buf1 <= 2)
        {
            
            test_state = 2;
            traverse_tmp_buf2 = 10;
            traverse_tmp_buf3 = 21;
        }
        else
        {
            test_mode = 1;
            test_state = 1;
            bp_dp = traverse_tmp_buf2;
            traverse_tmp_buf2 += 22;
            bp_sp = traverse_tmp_buf3;
            traverse_tmp_buf3 += 22;
        }
 
        task_interval_cnt = p_gui->page_frame_counter;  
        if(last_state != test_state)
        {
            last_state = test_state;
            task_interval_cnt = 0;
        }
        Gui_PageBP(task_interval_cnt, bp_sp, bp_dp, test_mode, test_state, task_interval_cnt * 10);
	} 
}

/****************************************************************
** Function name:           Cpu_TaskHeart
** Descriptions:            心率界面处理
** input parameters:        p_controller：UI事件控制状态；p_gui：显示相关的参数
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-08-12
*****************************************************************/ 
void Cpu_TaskHeartTest(cpu_task_controller_t *p_controller, gui_controller_t *p_gui)
{
//    static uint8_t heart = 0;
    uint8_t test_heart[48];
    uint8_t  heart_buff[8] = {12,34,56,78,90,198,176,254};
        
	if(p_controller->init == 1)
	{ 
            /* UI遍历中 */
            TASK_ENTRY_INIT(p_controller, 7);
            traverse_tmp_buf1 = 1;
		
		p_controller->key_press_hold = 0;
//		heart = 0;

		//!注意:此处变量设置是为了立即刷新动画,否则可能会延时
        task_interval_cnt = 0xFF;  
		p_gui->page_frame_counter = 0;
	} 
 
        //480ms的刷新间隔
        p_gui->updata_ui_interval = 500; 
    
	if(task_interval_cnt != p_gui->page_frame_counter)
	{
        task_interval_cnt = p_gui->page_frame_counter; 
 
//            heart = traverse_tmp_buf1;
            traverse_tmp_buf1 += 22;
 
        for(uint32_t i = 0; i< 48; i++)
        {
            test_heart[i] = 60 + i * 2;
        }
		Gui_PageHeart(task_interval_cnt, heart_buff[task_interval_cnt % 8], heart_buff[task_interval_cnt % 8],
        test_heart,test_heart[47],test_heart[0], (task_interval_cnt % 4 > 1 ? 0 : 1));
	}
}

/****************************************************************
** Function name:           Cpu_TaskSleep
** Descriptions:            睡眠界面处理
** input parameters:        p_controller：UI事件控制状态；p_gui：显示相关的参数
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-08-12
*****************************************************************/ 
void Cpu_TaskSleepTest(cpu_task_controller_t *p_controller, gui_controller_t *p_gui)
{ 
    
	if(p_controller->init == 1)
	{
        /* UI遍历中 */
        TASK_ENTRY_INIT(p_controller, 8);
        p_controller->key_press_hold = 0;
        traverse_tmp_buf1 = 60;
        traverse_tmp_buf2 = 0;
 
		//!注意:此处变量设置是为了立即刷新动画,否则可能会延时
        task_interval_cnt = 0xFF;  
		p_gui->page_frame_counter = 0;
	}	
 
    //480ms的刷新间隔
    p_gui->updata_ui_interval = 500;
    
 	if(task_interval_cnt != p_gui->page_frame_counter)
	{
        task_interval_cnt = p_gui->page_frame_counter;
        task_interval_cnt = 0;
        
        Gui_PageSleep(task_interval_cnt, traverse_tmp_buf1, traverse_tmp_buf1, traverse_tmp_buf1,task_interval_cnt, traverse_tmp_buf2++ % 2);
        traverse_tmp_buf1 += 60 + traverse_tmp_buf2 * 10 + traverse_tmp_buf2 + 5;
        traverse_tmp_buf2++;
        traverse_tmp_buf2 %= 5;
	}
}

/****************************************************************
** Function name:           Cpu_TaskFindPhone
** Descriptions:            找手机界面处理
** input parameters:        p_controller：UI事件控制状态；p_gui：显示相关的参数
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-08-12
*****************************************************************/ 
void Cpu_TaskFindPhoneTest(cpu_task_controller_t *p_controller,gui_controller_t *p_gui)
{
    uint8_t dynamic_flag = 0;
    
    if(p_controller->init == 1)
	{
        /* UI遍历中 */
        TASK_ENTRY_INIT(p_controller, 6);
        p_controller->key_press_hold = 0;
 
        //!注意:此处变量设置是为了立即刷新动画,否则可能会延时
        task_interval_cnt = 0xFF;
		p_gui->page_frame_counter = 0;
	}
 
        //480ms的刷新间隔
        p_gui->updata_ui_interval = 500;
 
	if(task_interval_cnt != p_gui->page_frame_counter)
	{
        task_interval_cnt = p_gui->page_frame_counter;
        
        if(task_interval_cnt > 5)
        {
            dynamic_flag = 1;
        }
        else
        {
            dynamic_flag = 0;
        }
 
        Gui_PageFindPhone(dynamic_flag, (task_interval_cnt == 6 ? 0:task_interval_cnt), task_interval_cnt % 2);
	} 
}

/****************************************************************
** Function name:           Cpu_TaskPowerOnTest
** Descriptions:            开机界面处理
** input parameters:        p_controller：UI事件控制状态；p_gui：显示相关的参数
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-08-12
*****************************************************************/ 
void Cpu_TaskPowerOnTest(cpu_task_controller_t *p_controller, gui_controller_t *p_gui)
{   
	if(p_controller->init == 1)
	{
 
        TASK_ENTRY_INIT(p_controller, 5);
        p_controller->key_press_hold = 0;
 
		//!注意:此处变量设置是为了立即刷新动画,否则可能会延时
        task_interval_cnt = 0xFF;  
		p_gui->page_frame_counter = 0;
	}
		
    //1000ms的刷新间隔
    p_gui->updata_ui_interval = 300;
    
	if(task_interval_cnt != p_gui->page_frame_counter)
	{
        task_interval_cnt = p_gui->page_frame_counter;
		Gui_PagePowerOn(task_interval_cnt);        
	} 
}

/****************************************************************
** Function name:           Cpu_TaskPowerOff
** Descriptions:            关机界面处理
** input parameters:        p_controller：UI事件控制状态；p_gui：显示相关的参数
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-08-12
*****************************************************************/ 
void Cpu_TaskPowerOffTest(cpu_task_controller_t *p_controller, gui_controller_t *p_gui)
{   
	if(p_controller->init == 1)
	{
 
        TASK_ENTRY_INIT(p_controller, 5);
        p_controller->key_press_hold = 0;
 
		//!注意:此处变量设置是为了立即刷新动画,否则可能会延时
        task_interval_cnt = 0xFF;  
		p_gui->page_frame_counter = 0;
	}
		
    //1000ms的刷新间隔
    p_gui->updata_ui_interval = 300;
    
	if(task_interval_cnt != p_gui->page_frame_counter)
	{
        task_interval_cnt = p_gui->page_frame_counter;
		Gui_PagePowerOff(task_interval_cnt);        
	} 
}

/****************************************************************
** Function name:           Cpu_TaskStopWatch
** Descriptions:            秒表界面处理
** input parameters:        p_controller：UI事件控制状态；p_gui：显示相关的参数
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-08-12
*****************************************************************/ 
void Cpu_TaskStopWatchTest(cpu_task_controller_t *p_controller, gui_controller_t *p_gui)
{
    uint8_t millisecond = 0;
    uint16_t total_second = 0;
	
	if(p_controller->init == 1)
	{
        TASK_ENTRY_INIT(p_controller, 6);
        p_controller->key_press_hold = 0;
        traverse_tmp_buf1 = 0;
        traverse_tmp_buf2 = 0;
        traverse_tmp_buf3 = 1;
        traverse_tmp_buf4 = 0;
 	
		//!注意:此处变量设置是为了立即刷新动画,否则可能会延时
        task_interval_cnt = 0xFF;  
		p_gui->page_frame_counter = 0;   
	}
 
    //300ms的刷新间隔
    p_gui->updata_ui_interval = 500;
 
	if(task_interval_cnt != p_gui->page_frame_counter)
	{
        task_interval_cnt = p_gui->page_frame_counter;
        traverse_tmp_buf1++;
        if(traverse_tmp_buf1 > 1)
        {
            millisecond = traverse_tmp_buf2++;
            total_second = traverse_tmp_buf3;
            traverse_tmp_buf3 += 22 + 11*60 ;
            if(traverse_tmp_buf4++ == 0)
            {
                task_interval_cnt = 0;
            }
        }
 
		GUI_StopWatchPage(task_interval_cnt, 1,(task_interval_cnt % 4 > 1 ? 1 : 0), 
                        (task_interval_cnt % 4 > 1 ? 1 : 0), total_second, millisecond, 
						task_interval_cnt, task_interval_cnt, task_interval_cnt > 4);
	} 
}

/****************************************************************
** Function name:           Gui_TaskCountDown
** Descriptions:            倒计时界面处理
** input parameters:        p_controller：UI事件控制状态；p_gui：显示相关的参数
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-08-12
*****************************************************************/ 
void Gui_TaskCountDownTest(cpu_task_controller_t *p_controller, gui_controller_t *p_gui)
{
    uint32_t test_buff[2] = {12*60*60 + 34*60 + 56, 19*60*60 + 8*60 + 7};
        
	if(p_controller->init == 1)
	{
            TASK_ENTRY_INIT(p_controller, 6);
            p_controller->key_press_hold = 0;
            traverse_tmp_buf1 = 0;
 
        //!注意:此处变量设置是为了立即刷新动画,否则可能会延时
        task_interval_cnt = 0xFF;  
		p_gui->page_frame_counter = 0;
	}
 
    //480ms的刷新间隔
    p_gui->updata_ui_interval = 300;
 
	if(task_interval_cnt != p_gui->page_frame_counter)
	{
        task_interval_cnt = p_gui->page_frame_counter;
 
        Gui_PageCountDown(task_interval_cnt, (task_interval_cnt > 6 ? 1 : 0), 
                        task_interval_cnt % 3, test_buff[task_interval_cnt % 2], task_interval_cnt%2);
	} 
}



/****************************************************************
** Function name:           Cpu_RunModePause
** Descriptions:            运动暂停界面事件处理
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-08-12
*****************************************************************/ 
void Cpu_TaskRunModePauseTest(cpu_task_controller_t *p_controller, gui_controller_t *p_gui)
{
	if(p_controller->init == 1)
	{
 
        TASK_ENTRY_INIT(p_controller, 4);
        p_controller->key_press_hold = 0;
        traverse_tmp_buf1 = 0;
        traverse_tmp_buf2 = 0;
 
		//!注意:此处变量设置是为了立即刷新动画,否则可能会延时
		task_interval_cnt = 0xFF;
		p_gui->page_frame_counter = 0;
        
	}
 
    //480ms的刷新间隔
    p_gui->updata_ui_interval = 500;
 
	if(task_interval_cnt != p_gui->page_frame_counter)
	{
        task_interval_cnt = p_gui->page_frame_counter;
        
        if(traverse_tmp_buf1 % 3 == 2)
        {
            traverse_tmp_buf2++;
        }
        traverse_tmp_buf1++;
        
        display_language++;
        display_language %= 2;
 
        //Gui_PageRunModePause(task_interval_cnt, traverse_tmp_buf2 % 2, display_language);
 
	}
}

/****************************************************************
** Function name:           Cpu_EventRunModeExit
** Descriptions:            运动退出界面事件处理
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-08-12
*****************************************************************/ 
void Cpu_TaskRunModeExitTest(cpu_task_controller_t *p_controller, gui_controller_t *p_gui)
{
	if(p_controller->init == 1)
	{
 
            TASK_ENTRY_INIT(p_controller, 1);
            p_controller->key_press_hold = 0;
  
		//!注意:此处变量设置是为了立即刷新动画,否则可能会延时
		task_interval_cnt = 0xFF;
		p_gui->page_frame_counter = 0;
		//清除背景图
        
	}
	
    //500ms的刷新间隔
    p_gui->updata_ui_interval = 300;
    
	if(task_interval_cnt != p_gui->page_frame_counter)
	{
        task_interval_cnt = p_gui->page_frame_counter;
        
        display_language++;
        display_language %= 2;
        
        //Gui_PageRunModeExit(task_interval_cnt, display_language);
	}
}

/****************************************************************
** Function name:           Cpu_EvenBleDiscRemindTest
** Descriptions:            蓝牙断联
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-08-12
*****************************************************************/ 
void Cpu_EvenBleDiscRemindTest(cpu_task_controller_t *p_controller, gui_controller_t *p_gui)
{
	if(p_controller->init == 1)
	{
        TASK_ENTRY_INIT(p_controller, 5);
        p_controller->key_press_hold = 0;
  
		//!注意:此处变量设置是为了立即刷新动画,否则可能会延时
		task_interval_cnt = 0xFF;
		p_gui->page_frame_counter = 0;
		//清除背景图
        
	}
	
    //500ms的刷新间隔
    p_gui->updata_ui_interval = 300;
    
	if(task_interval_cnt != p_gui->page_frame_counter)
	{
        task_interval_cnt = p_gui->page_frame_counter;
        Gui_PageBleDiscRemind(task_interval_cnt);
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
    switch(cpu_s.task)
	{
		case CPU_TASK_MAIN:     
            //主界面
			Cpu_TaskMainTest(&cpu_s.job_control, &cpu_s.gui);
			break;
		case CPU_TASK_STEP:     
            //计步
			Cpu_TaskStepTest(&cpu_s.job_control, &cpu_s.gui);
			break;       
        case CPU_TASK_MOVEMENT:
            //运动模式
			Cpu_TaskRunTest(&cpu_s.job_control, &cpu_s.gui);
            break;
        case CPU_TASK_BP:       
            //血压
            Cpu_TaskBPTest(&cpu_s.job_control, &cpu_s.gui);
            break;
		case CPU_TASK_HEART:    
            //心率
			Cpu_TaskHeartTest(&cpu_s.job_control, &cpu_s.gui);
			break;
		case CPU_TASK_SLEEP:    
            //睡眠
			Cpu_TaskSleepTest(&cpu_s.job_control, &cpu_s.gui);
			break;
		case CPU_TASK_FINDPHONE:     
            //查找手机
			Cpu_TaskFindPhoneTest(&cpu_s.job_control, &cpu_s.gui);
			break;
		case CPU_TASK_STOP_WATCH:    
            //秒表
			Cpu_TaskStopWatchTest(&cpu_s.job_control, &cpu_s.gui);
			break;
		case CPU_TASK_COUNT_DOWN:    
            //倒计时
			Gui_TaskCountDownTest(&cpu_s.job_control, &cpu_s.gui);
			break;
        case CPU_TASK_POWERON:                  
            //开机
			Cpu_TaskPowerOnTest(&cpu_s.job_control, &cpu_s.gui);
			break;
        
		case CPU_TASK_POWEROFF:                  
            //关机
			Cpu_TaskPowerOffTest(&cpu_s.job_control, &cpu_s.gui);
			break;
		         
        case CPU_TASK_MAC_VER:
            //显示mac和ver
            Cpu_EvenDisMacVerTest(&cpu_s.event_control, &cpu_s.gui);
            break;

//        case CPU_TASK_MOVEMENT_BUTTON:
//            // 运动模式操作界面
//            Cpu_TaskRunButtonTest(&cpu_s.event_control, &cpu_s.gui);
//            break;
        
        case CPU_TASK_MOVEMENT_DATA:
            // 运动模式数据界面
            Cpu_TaskRunDataTest(&cpu_s.event_control, &cpu_s.gui);
            break;
        
		default:
//            Motor_DelEvent();
			cpu_s.task = CPU_TASK_IDEL;
			break;
	}
    
    switch(cpu_s.event)
	{
		
		case CPU_EVENT_TASK_CHARGE:
            //充电
			Cpu_EventIdelChargeTest(&cpu_s.event_control, &cpu_s.gui);
			break;
        
		case CPU_EVENT_TASK_ALARM:     
            //闹钟
			Cpu_EventAlarmTest(&cpu_s.event_control, &cpu_s.gui);
			break;
		
		case CPU_EVENT_TASK_SIT:        
            //久坐
			Cpu_EventSedentaryTest(&cpu_s.event_control, &cpu_s.gui);
			break;
        
		case CPU_EVENT_TASK_CALL_REMIND:
            //来电
			Cpu_EventCallRemindTest(&cpu_s.event_control, &cpu_s.gui);
            break;
        
		case CPU_EVENT_TASK_MESSAGE_REMIND:    
            //信息
			Cpu_EventMessageRemindTest(&cpu_s.event_control, &cpu_s.gui);
			break;
		
		case CPU_EVENT_TASK_HEARTALERT: 
            //心率
			Cpu_EventHeartAlertTest(&cpu_s.event_control, &cpu_s.gui);
			break;
		
		case CPU_EVENT_TASK_CAMERAREADY:    
            //拍照
			Cpu_EventCameraReadyTest(&cpu_s.event_control, &cpu_s.gui);
			break;
		
		case CPU_EVENT_TASK_CAMERAOK:       
            //拍照完成
			Cpu_EventCameraOKTest(&cpu_s.event_control, &cpu_s.gui);
			break;
		 
		case CPU_EVENT_TASK_FONTTEST:       
            //测试
			Cpu_EventFontTestTest(&cpu_s.event_control, &cpu_s.gui);
			break;
		
		case CPU_EVENT_TASK_LOWPOWER:      
            //低电
			Cpu_EventLowPowerHintTest(&cpu_s.event_control, &cpu_s.gui);
			break;
        
        case CPU_EVENT_TASK_STARGET:
            Cpu_EvenStargetTest(&cpu_s.event_control, &cpu_s.gui);
            break;
        
        case CPU_EVENT_TASK_BLE_DISC_REMIND:
            Cpu_EvenBleDiscRemindTest(&cpu_s.event_control, &cpu_s.gui);
            break;

		default:
			cpu_s.event = CPU_EVENT_TASK_IDEL;
			break;
	}
}


