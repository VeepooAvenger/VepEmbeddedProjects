/***************************Copyright(c)***********************************
**                       Veepoo Tech
**                  COPYRIGHT (C) 2016-2019
**-----------File Info-----------------------------------------------------
** File        name:     cpu_event_task.c
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
#include <string.h>
#include "cpu_event_task.h"
#include "motor_event.h"
#include "gui_page_display.h"
#include "gui_data_inteface.h"
#include "gui_thread.h"
#include "bsp_button.h"
#include "cpu_general_task.h"


static uint16_t evnet_interval_cnt = 0;   //最后的间隔次数，用于判断是否刷新UI
static uint8_t event_msg_type = 0;

static uint8_t button_pressed_cnt = 0;  //按键次数
static uint8_t entry_cnt = 0;
static CPU_TASK_TYPE task_save = CPU_TASK_IDEL;

extern uint8_t Gui_ThreadIsReadingNotify(uint8_t event);

extern cpu_task_t cpu_s;

extern uint8_t count_down_start_flag;			// 倒计时启动标志

extern uint8_t count_down_timeout_flag;			// 倒计时到时标志

/****************************************************************
** Function name:           Cpu_TaskClearIntervalCnt
** Descriptions:            //重置刷新间隔计数器
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-08-12
*****************************************************************/ 
void Cpu_EventClearIntervalCnt(void)
{
    evnet_interval_cnt = 0xFFFF;
}

/****************************************************************
** Function name:           Cpu_EventLowPowerHint
** Descriptions:            低电显示界面
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-08-12
*****************************************************************/ 
void Cpu_EventLowPowerHint(cpu_task_controller_t *p_controller, gui_controller_t *p_gui)
{	
	if(p_controller->init == 1)
	{
        TASK_ENTRY_INIT(p_controller, 3);
        
		//!注意:此处变量设置是为了立即刷新动画,否则可能会延时
        evnet_interval_cnt = 0xFFFF;
		p_gui->page_frame_counter = 0; 
        button_pressed_cnt = 0;
		
#if	LED_ON_OFF_ON_DEF		
		Gui_LcdLedOff();
#endif			
	}
    
    // 刷新间隔 150ms
    p_gui->updata_ui_interval = 250;
    
	if(evnet_interval_cnt != p_gui->page_frame_counter)
	{	
        evnet_interval_cnt = p_gui->page_frame_counter;
        Gui_PageLowPower(evnet_interval_cnt);
    }
    
    if(Gui_DataSystemIsPoweOn() && evnet_interval_cnt >= 8)
    {
        Gui_ThreadEventTaskClear();
        GuiThread_JumpToMain();
    }
	
#if	LED_ON_OFF_ON_DEF		
	if(p_gui->page_frame_counter == 0)
	{
		Gui_LcdLedOn();
	}
#endif		
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
void Cpu_EventIdelCharge(cpu_task_controller_t *p_controller, gui_controller_t *p_gui)
{		
    uint8_t bat_level = 0;
	uint8_t charge_state = Gui_DataGetBatteryLevelAndState(&bat_level);
    
	if(p_controller->init == 1)
	{
        TASK_ENTRY_INIT(p_controller, 8);
        
		// 注意:此处变量设置是为了立即刷新动画,否则可能会延时
        evnet_interval_cnt = 0xFFFF;
		p_gui->page_frame_counter = 0;	
        button_pressed_cnt = 0;	
		
#if	LED_ON_OFF_ON_DEF		
		Gui_LcdLedOff();
#endif			
	}
    
    // 刷新间隔 150ms
    p_gui->updata_ui_interval = 250;

    // 如果充电状态改变,则立即关掉显示充电状态
    if(charge_state == BATTERRY_IS_NORMAL)
    {
        p_controller->keep_time = 0;
        p_controller->run_time = 1;
    }
    else
    {
        if(!Gui_DataSystemIsPoweOn())
        {
            // 关机下做开机处理    
            if(Gui_DataBatteryIsLowPower() != BATTERRY_IS_LOW_TO_SHUT)
            {     
                Gui_DataSystemToPoweOn();
            }
            return;
        }
    }

	if(evnet_interval_cnt != p_gui->page_frame_counter)
	{	
        evnet_interval_cnt = p_gui->page_frame_counter;	

        // 如果充电状态满格时,则不再刷新
        if(charge_state == BATTERRY_IS_FULL)
        {            
            Gui_PageBatteryCharg(evnet_interval_cnt, 1, bat_level);
        }
        else
        {        
            Gui_PageBatteryCharg(evnet_interval_cnt, 0, bat_level);
        }
    }
    
    if(p_gui->key_info.key != APP_BSP_KEY_NULL_PRESS)
    {
        if(p_gui->key_info.key == APP_BSP_KEY_SHORT_PRESS)
        {
            // 退出外部事件
            Gui_ThreadEventTaskClear();
            // 回到主界面
            GuiThread_JumpToMain();
        }
        
        Gui_ClearKeyInfo();
    }
	
#if	LED_ON_OFF_ON_DEF		
	if(p_gui->page_frame_counter == 0)
	{
		Gui_LcdLedOn();
	}
#endif		
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
void Cpu_EventAlarm(cpu_task_controller_t *p_controller, gui_controller_t *p_gui)
{
	uint8_t time_format = 0;
    uint8_t alarm_hour = 0;
    uint8_t alarm_minute = 0;
    uint8_t alarm_mode = 0;
    
	if(p_controller->init == 1)
	{
        TASK_ENTRY_INIT(p_controller, 20);

        // 提醒10组 每组振动1秒 间隔1秒
        Motor_AddEvent(10, 1, 1, 1);
		
		// 注意:此处变量设置是为了立即刷新动画,否则可能会延时
        evnet_interval_cnt = 0xFFFF;
		p_gui->page_frame_counter = 0;
        
        button_pressed_cnt = 0;
		
		task_save = cpu_s.task;
		
#if	LED_ON_OFF_ON_DEF		
		Gui_LcdLedOff();
#endif			
	}
    
    // 刷新间隔 160ms
    p_gui->updata_ui_interval = 150;
    
	if(evnet_interval_cnt != p_gui->page_frame_counter)
	{
        evnet_interval_cnt = p_gui->page_frame_counter;
        time_format = Gui_DataTimeFormatDisplay(); 
        Gui_DataGetAlarmClockData(&alarm_hour, &alarm_minute, &alarm_mode);
        if(time_format != GUI_TIME_FORMAT_STANDARD)
        {
            alarm_hour = alarm_hour%12;
            if(alarm_hour == 0)
            {
                // YX.20170921.修复闹钟时间显示问题
                alarm_hour = 12;
            }
        }
        Gui_PageAlarm(evnet_interval_cnt, Gui_DataTimeFormatDisplay(), alarm_mode, alarm_hour, alarm_minute, display_language );
	}
    
    if(p_gui->key_info.key != APP_BSP_KEY_NULL_PRESS)
    {
        if(p_gui->key_info.key == APP_BSP_KEY_SHORT_PRESS)
        {
			// 停止震动
			Motor_DelEvent();   
			Gui_ThreadEventTaskClear();
			
			if(task_save != CPU_TASK_IDEL)
			{
				Cpu_TaskSchedule(&cpu_s.job_control, p_gui, task_save);
				task_save = CPU_TASK_IDEL;
			}
        }
        
        Gui_ClearKeyInfo();
    }
	
#if	LED_ON_OFF_ON_DEF		
	if(p_gui->page_frame_counter == 0)
	{
		Gui_LcdLedOn();
	}
#endif		
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
void Cpu_EventSedentary(cpu_task_controller_t *p_controller, gui_controller_t *p_gui)
{
	if(p_controller->init == 1)
	{
        TASK_ENTRY_INIT(p_controller, 8);
        // 提醒两组 每组振动3秒 间隔2秒
        Motor_AddEvent(2, 1, 3, 2); 

		// 注意:此处变量设置是为了立即刷新动画,否则可能会延时
        evnet_interval_cnt = 0xFFFF;
		p_gui->page_frame_counter = 0;
        button_pressed_cnt = 0;
		
		task_save = cpu_s.task;
		
#if	LED_ON_OFF_ON_DEF		
		Gui_LcdLedOff();
#endif
	}
	
    // 刷新间隔 ms
    p_gui->updata_ui_interval = 150;
    
	if(evnet_interval_cnt != p_gui->page_frame_counter)
	{
        evnet_interval_cnt = p_gui->page_frame_counter;
		Gui_PageSit(evnet_interval_cnt, display_language);
	}
    
    if(p_gui->key_info.key != APP_BSP_KEY_NULL_PRESS)
    {
        if(p_gui->key_info.key == APP_BSP_KEY_SHORT_PRESS)
        {
			// 停止震动
			Motor_DelEvent(); 
			// 退出外部事件
			Gui_ThreadEventTaskClear();
			
			if(task_save != CPU_TASK_IDEL)
			{
				Cpu_TaskSchedule(&cpu_s.job_control, p_gui, task_save);
				task_save = CPU_TASK_IDEL;
			}
        }
        
        Gui_ClearKeyInfo();
    }
	
#if	LED_ON_OFF_ON_DEF		
	if(p_gui->page_frame_counter == 0)
	{
		Gui_LcdLedOn();
	}
#endif		
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
void Cpu_EventCallRemind(cpu_task_controller_t *p_controller, gui_controller_t *p_gui)
{
    static uint8_t last_call_state = 0x00; 
    
    uint8_t page_index = 0;
    uint8_t *display_buf = NULL;
    uint16_t buf_len = 0;
    
	if(p_controller->init == 1)
	{
        // 这里需要分类处理
        event_msg_type = Gui_DataPhoneNotifyDeal();      
        
        if(event_msg_type == GUI_MSG_TYPE_CALL)
        {
            if(Gui_DataMessageCallState() == GUI_CALL_REJECTSUC)
            {               
                TASK_ENTRY_INIT(p_controller, 3);		
            }
            else if(Gui_DataMessageCallState() == GUI_CALL_INCOMING || Gui_DataMessageCallState() == GUI_CALL_MUTE)
            {
                TASK_ENTRY_INIT(p_controller, 60); 	  
            }
            else
            {
                TASK_ENTRY_INIT(p_controller, 1);
            }
        }  
        else
        {
             TASK_ENTRY_INIT(p_controller, 1);
        }         
		// 注意:此处变量设置是为了立即刷新动画,否则可能会延时
        evnet_interval_cnt = 0xFFFF;
		p_gui->page_frame_counter = 0;
        last_call_state = 0x00;     
		
		task_save = cpu_s.task;
			
#if	LED_ON_OFF_ON_DEF		
		Gui_LcdLedOff();
#endif
	}
    
    // 刷新间隔 ms
    p_gui->updata_ui_interval = 250;

	if(evnet_interval_cnt != p_gui->page_frame_counter)
	{ 
        evnet_interval_cnt = p_gui->page_frame_counter;       
        
        display_buf = Gui_DataGetPhoneNotifyData(&buf_len, &page_index);

        // 这里需要分类处理
        if(event_msg_type == GUI_MSG_TYPE_CALL)
        {           
            if(last_call_state != Gui_DataMessageCallState() )
            {
                last_call_state = Gui_DataMessageCallState();

                Gui_PageCallNotificationIcon(evnet_interval_cnt, last_call_state, display_language);
                // JSH.20181112.解决陌生来电号码显示不全
				//Gui_PageCallNotificationContent(display_buf, buf_len);
                if(last_call_state == GUI_CALL_REJECTSUC)
                {
                    p_controller->run_time = 0;
                    p_controller->keep_time = 2;
                }
            }
			
			// JSH.20181112.解决陌生来电号码显示不全
			Gui_PageCallNotificationContent(evnet_interval_cnt, display_buf, buf_len);
			
			if((last_call_state == GUI_CALL_REJECTSUC)&&(p_controller->run_time >= 1))
			{
				// 退出外部事件
				Gui_ThreadEventTaskClear();
				
				if(task_save != CPU_TASK_IDEL)
				{
					Cpu_TaskSchedule(&cpu_s.job_control, p_gui, task_save);
					task_save = CPU_TASK_IDEL;
				}
			}
        }
	}
    
    if(p_gui->key_info.key != APP_BSP_KEY_NULL_PRESS)
    {
        if(p_gui->key_info.key == APP_BSP_KEY_SHORT_PRESS)
        {
			// 停止震动
			Motor_DelEvent();
			// 设置来电静音
			Gui_DataMessageCallToMute();
		}
		else if(p_gui->key_info.key == APP_BSP_PRESS_LONG_3S)
		{
			// 停止震动
			Motor_DelEvent();
			// 来电状态,长按挂断来电
			Gui_DataMessageCallToHangUp();
		}
			        
        Gui_ClearKeyInfo();
    }
	
#if	LED_ON_OFF_ON_DEF		
	if(p_gui->page_frame_counter == 0)
	{
		Gui_LcdLedOn();
	}
#endif
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
void Cpu_EventMessageRemind(cpu_task_controller_t *p_controller, gui_controller_t *p_gui)
{      
    static uint8_t last_page_index = 0xFF; 
    
    uint8_t page_index = 0;
    uint8_t *display_buf = NULL;
    uint16_t buf_len = 0;
	uint8_t hour = 0;
	uint8_t minute = 0;
    
	if(p_controller->init == 1)
	{
        // 这里需要分类处理
        event_msg_type = Gui_DataPhoneNotifyDeal();
        
        if(event_msg_type >= GUI_MSG_TYPE_MESSAGE && event_msg_type <= GUI_MSG_TYPE_OTHER)
        {
            TASK_ENTRY_INIT(p_controller, 5);	
        }
        else
        {
            TASK_ENTRY_INIT(p_controller, 0);	
        }
        
		// 注意:此处变量设置是为了立即刷新动画,否则可能会延时
        evnet_interval_cnt = 0xFFFF;
		p_gui->page_frame_counter = 0;
        last_page_index = 0xFF;	    
        button_pressed_cnt = 0;  
		
		task_save = cpu_s.task;
		
#if	LED_ON_OFF_ON_DEF		
		Gui_LcdLedOff();
#endif
	}

    // 刷新间隔 ms
    p_gui->updata_ui_interval = 100;
    
	if(evnet_interval_cnt != p_gui->page_frame_counter)
	{
        evnet_interval_cnt = p_gui->page_frame_counter;       

        display_buf = Gui_DataGetPhoneNotifyData(&buf_len, &page_index);
        
        // 这里需要分类处理
        if(event_msg_type >= GUI_MSG_TYPE_MESSAGE && event_msg_type <= GUI_MSG_TYPE_OTHER)
        {

#if  1 //	JSH.20180930.第二种信息刷屏方式，注意改屏的左右间距，消息行数	
			
			hour = Gui_DataGetSystemTimeHour(); 
			minute = Gui_DataGetSystemTimeMinute();
			Gui_PageMessageNotificationIcon02(evnet_interval_cnt, event_msg_type, 0,hour,minute,display_language);
				
            if(last_page_index != page_index)
            {
                if(last_page_index != 0xFF)
                {
                    evnet_interval_cnt = 0;
                }

                Gui_PageMessageNotificationContent02(evnet_interval_cnt,display_buf, buf_len, page_index);
                last_page_index = page_index;
            }
			
#else
	
			if(last_page_index != page_index)
            {
                if(last_page_index != 0xFF)
                {
                    evnet_interval_cnt = 0;
                }
				hour = Gui_DataGetSystemTimeHour(); 
				minute = Gui_DataGetSystemTimeMinute();
                Gui_PageMessageNotificationIcon(evnet_interval_cnt, event_msg_type, 0,hour,minute,display_language);
                Gui_PageMessageNotificationContent(evnet_interval_cnt,display_buf, buf_len, page_index);
                last_page_index = page_index;
            }

#endif
			
        }
	}
    
    if(p_gui->key_info.key != APP_BSP_KEY_NULL_PRESS)
    {
        if(p_gui->key_info.key == APP_BSP_KEY_SHORT_PRESS)
        {
            if(Gui_DataHaveUnreadMessage(1))
            {
                p_controller->keep_time = 5;
                p_controller->run_time = 0;
            }
            else
            {
                // 退出外部事件
                Gui_ThreadEventTaskClear();
				
				if(task_save != CPU_TASK_IDEL)
				{
					Cpu_TaskSchedule(&cpu_s.job_control, p_gui, task_save);
					task_save = CPU_TASK_IDEL;
				}
            }
            
            // 停止震动 
            Motor_DelEvent();
        }

        Gui_ClearKeyInfo();
    }
	
#if	LED_ON_OFF_ON_DEF		
	if(p_gui->page_frame_counter == 0)
	{
		Gui_LcdLedOn();
	}
#endif		
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
void Cpu_EventHeartAlert(cpu_task_controller_t *p_controller, gui_controller_t *p_gui)
{  
	if(p_controller->init == 1)
	{
        TASK_ENTRY_INIT(p_controller, 8);
        //提醒2组 每组振3秒 间隔2秒
        Motor_AddEvent(2, 1, 3, 2);
        
		// 注意:此处变量设置是为了立即刷新动画,否则可能会延时
        evnet_interval_cnt = 0xFFFF;
		p_gui->page_frame_counter = 0;   
        button_pressed_cnt = 0; 

		task_save = cpu_s.task;
		
#if	LED_ON_OFF_ON_DEF		
		Gui_LcdLedOff();
#endif	
	}
	
    // 刷新间隔 60ms
    p_gui->updata_ui_interval = 100;
    
	if(evnet_interval_cnt != p_gui->page_frame_counter)
	{
        evnet_interval_cnt = p_gui->page_frame_counter;
		Gui_PageHeartAlert(evnet_interval_cnt);
	}
    
    if(p_gui->key_info.key != APP_BSP_KEY_NULL_PRESS)
    {
        if(p_gui->key_info.key == APP_BSP_KEY_SHORT_PRESS)
        {
			// 停止震动
			Motor_DelEvent(); 
			// 退出外部事件
			Gui_ThreadEventTaskClear();
			
			if(task_save != CPU_TASK_IDEL)
			{
				Cpu_TaskSchedule(&cpu_s.job_control, p_gui, task_save);
				task_save = CPU_TASK_IDEL;
			}
        }
        
        Gui_ClearKeyInfo();
    }
	
#if	LED_ON_OFF_ON_DEF		
	if(p_gui->page_frame_counter == 0)
	{
		Gui_LcdLedOn();
	}
#endif		
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
void Cpu_EventFontTest(cpu_task_controller_t *p_controller, gui_controller_t *p_gui)
{
    if(p_controller->init == 1)
	{
        TASK_ENTRY_INIT(p_controller, 60);

		// 注意:此处变量设置是为了立即刷新动画,否则可能会延时
        evnet_interval_cnt = 0xFFFF;
		p_gui->page_frame_counter = 0; 
        button_pressed_cnt = 0;
		
#if	LED_ON_OFF_ON_DEF		
		Gui_LcdLedOff();
#endif			
	}
	
    // 刷新间隔 ms
    p_gui->updata_ui_interval = 300;
    
	if(evnet_interval_cnt != p_gui->page_frame_counter)
	{
        evnet_interval_cnt = p_gui->page_frame_counter;
		Gui_PageFontTest(evnet_interval_cnt);
	}
   
#if	LED_ON_OFF_ON_DEF		
	if(p_gui->page_frame_counter == 0)
	{
		Gui_LcdLedOn();
	}
#endif		
}

/****************************************************************
** Function name:           Cpu_EvenCountDownTimeOut
** Descriptions:            倒计时超时界面事件处理
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-08-12
*****************************************************************/ 
void Cpu_EvenCountDownTimeOut(cpu_task_controller_t *p_controller, gui_controller_t *p_gui)
{
	if(p_controller->init == 1)
	{
		TASK_ENTRY_INIT(p_controller, 6);
		
		// 注意:此处变量设置是为了立即刷新动画,否则可能会延时
		evnet_interval_cnt = 0xFFFF;
		p_gui->page_frame_counter = 0;
        button_pressed_cnt = 0;
		
		Motor_AddEvent(3, 1, 1, 1);
        entry_cnt = 1;
		
		task_save = cpu_s.task;
		
#if	LED_ON_OFF_ON_DEF	
		if(count_down_timeout_flag)
		{
			count_down_timeout_flag = 0;
			Gui_LcdLedOff();
		}
#endif
	}
	
    // 刷新间隔 70ms
    p_gui->updata_ui_interval = 100;
    
	if(evnet_interval_cnt != p_gui->page_frame_counter)
	{
        evnet_interval_cnt = p_gui->page_frame_counter;
        if(entry_cnt == 0)
        {
            Gui_PageCountDown(evnet_interval_cnt, 1, 0, Gui_DataCountDownCurrentSecond(), display_language);        
        }
        else
        {
			Gui_PageCountDown(evnet_interval_cnt, 1, 1, Gui_DataCountDownCurrentSecond(), display_language); 
        }
    }
    
     if(p_gui->key_info.key != APP_BSP_KEY_NULL_PRESS)
    {   
		if(p_gui->key_info.key == APP_BSP_KEY_SHORT_PRESS)
        { 
			// 停止震动
			Motor_DelEvent();
			// 退出外部事件
			Gui_ThreadEventTaskClear();

			if(task_save != CPU_TASK_IDEL)
			{
				Cpu_TaskSchedule(&cpu_s.job_control, p_gui, task_save);
				task_save = CPU_TASK_IDEL;
			}			
        }

        Gui_ClearKeyInfo();
    }
	
#if	LED_ON_OFF_ON_DEF		
	if(p_gui->page_frame_counter == 0)
	{
		Gui_LcdLedOn();
	}
#endif
}

/****************************************************************
** Function name:           Cpu_EvenCountDownTimeStart
** Descriptions:            倒计时开始界面事件处理
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-08-12
*****************************************************************/ 
void Cpu_EvenCountDownTimeStart(cpu_task_controller_t *p_controller, gui_controller_t *p_gui)
{
    
	if(p_controller->init == 1)
	{
		TASK_ENTRY_INIT(p_controller, 5);
		
		// 注意:此处变量设置是为了立即刷新动画,否则可能会延时
		evnet_interval_cnt = 0xFFFF;
		p_gui->page_frame_counter = 0;
        button_pressed_cnt = 0;
        entry_cnt = 1;
		p_controller->key_press_hold = 1;
		
#if	LED_ON_OFF_ON_DEF	
		if(count_down_start_flag)
		{
			count_down_start_flag = 0;
			Gui_LcdLedOff();
		}
#endif
	}
	
    // 刷新间隔 70ms
    p_gui->updata_ui_interval = 100;
    
	if(evnet_interval_cnt != p_gui->page_frame_counter)
	{
        evnet_interval_cnt = p_gui->page_frame_counter;
        if(entry_cnt == 0)
        {
            Gui_PageCountDown(evnet_interval_cnt, 1, 1, Gui_DataCountDownCurrentSecond(), display_language);        
        }
        else
        {
            if(Gui_DataCountDownIsBusy() == 0)
            {
                Gui_PageCountDown(evnet_interval_cnt, 1, 0, Gui_DataCountDownConfigSecond(), display_language); 
            }
			else if(Gui_DataCountDownIsRun())
			{
				Gui_PageCountDown(evnet_interval_cnt, 1, 1, Gui_DataCountDownCurrentSecond(), display_language); 
			}
			else
			{
				Gui_PageCountDown(evnet_interval_cnt, 1, 0, Gui_DataCountDownCurrentSecond(), display_language); 
			}
        }
    }
    
  
    if(p_gui->key_info.key != APP_BSP_KEY_NULL_PRESS)
    {   
		if(p_gui->key_info.key == APP_BSP_KEY_SHORT_PRESS)
        { 
            // 需要判断倒计时当前状态
            if(Gui_DataCountDownIsRun())
            {       
                Gui_DataCountDownPause();                   
                p_controller->keep_time = 5;            
                p_controller->run_time = 0;             
            }
            else            
            {            
                p_controller->keep_time = 5;            
                p_controller->run_time = 0; 
                Gui_DataCountDownStart();
            }
            entry_cnt = 1;
        }
		else if(p_gui->key_info.key == APP_BSP_PRESS_LONG_3S)
		{
			Gui_DataCountDownExit(); 
			
			// 退出外部事件
			Gui_ThreadEventTaskClear();
			
			Cpu_TaskSchedule(&cpu_s.job_control, p_gui, CPU_TASK_COUNT_DOWN);
		}
        // 停止震动
        Motor_DelEvent();
        
        Gui_ClearKeyInfo();
    }
	
#if	LED_ON_OFF_ON_DEF		
	if(p_gui->page_frame_counter == 0)
	{
		Gui_LcdLedOn();
	}
#endif		
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
void Cpu_EventCameraReady(cpu_task_controller_t *p_controller, gui_controller_t *p_gui)
{
	if(p_controller->init == 1)
	{
        TASK_ENTRY_INIT(p_controller, 60);
        p_controller->key_press_hold = 1;  
		
		// 注意:此处变量设置是为了立即刷新动画,否则可能会延时
		evnet_interval_cnt = 0xFFFF;
		p_gui->page_frame_counter = 0;
        button_pressed_cnt = 0;
		
		task_save = cpu_s.task;
		
#if	LED_ON_OFF_ON_DEF		
		Gui_LcdLedOff();
#endif	
	}
	
    // 刷新间隔 150ms
    p_gui->updata_ui_interval = 150;
    
	if(evnet_interval_cnt != p_gui->page_frame_counter)
	{
        evnet_interval_cnt = p_gui->page_frame_counter;
		Gui_PageCameraReady(evnet_interval_cnt);
	}
    
    if(p_gui->key_info.key != APP_BSP_KEY_NULL_PRESS)
    {
        if(p_gui->key_info.key == APP_BSP_PRESS_LONG_3S)
        { 
            // 退出拍照
            Gui_DataLongKeyToExitPhoto();
			
			// 退出外部事件
			Gui_ThreadEventTaskClear();
			
			if(task_save != CPU_TASK_IDEL)
			{
				Cpu_TaskSchedule(&cpu_s.job_control, p_gui, task_save);
				task_save = CPU_TASK_IDEL;
			}
        }
        else if(p_gui->key_info.key == APP_BSP_KEY_SHORT_PRESS )
        {
            Gui_DataClickOnThePhoto();
            Gui_ThreadEventTaskWakeUp(CPU_EVENT_TASK_CAMERAOK);  
        }
        
        Gui_ClearKeyInfo();
    }
	
#if	LED_ON_OFF_ON_DEF		
	if(p_gui->page_frame_counter == 0)
	{
		Gui_LcdLedOn();
	}
#endif		
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
void Cpu_EventCameraOK(cpu_task_controller_t *p_controller, gui_controller_t *p_gui)
{
	if(p_controller->init == 1)
	{
		TASK_ENTRY_INIT(p_controller, 5);
		 
		// 注意:此处变量设置是为了立即刷新动画,否则可能会延时
		evnet_interval_cnt = 0xFFFF;
		p_gui->page_frame_counter = 0;
        button_pressed_cnt = 0;
		
		task_save = cpu_s.task;
		
#if	LED_ON_OFF_ON_DEF		
		Gui_LcdLedOff();
#endif	
	}
	
    // 刷新间隔 150ms
    p_gui->updata_ui_interval = 150;   
	if(evnet_interval_cnt != p_gui->page_frame_counter)
	{
        evnet_interval_cnt = p_gui->page_frame_counter;        
		Gui_PageCameraOk(evnet_interval_cnt);
	}
    
    if(p_gui->key_info.key != APP_BSP_KEY_NULL_PRESS)
    {
        if(p_gui->key_info.key == APP_BSP_PRESS_LONG_3S)
        { 
            // 退出拍照
            Gui_DataLongKeyToExitPhoto();
			// 退出外部事件
			Gui_ThreadEventTaskClear();
			
			if(task_save != CPU_TASK_IDEL)
			{
				Cpu_TaskSchedule(&cpu_s.job_control, p_gui, task_save);
				task_save = CPU_TASK_IDEL;
			}
        }
        else if(p_gui->key_info.key == APP_BSP_KEY_SHORT_PRESS )
        {
            Gui_DataClickOnThePhoto();
            Gui_ThreadEventTaskWakeUp(CPU_EVENT_TASK_CAMERAOK);  
        }
        
        Gui_ClearKeyInfo();
    }
	
#if	LED_ON_OFF_ON_DEF		
	if(p_gui->page_frame_counter == 0)
	{
		Gui_LcdLedOn();
	}
#endif		
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
void Cpu_EvenStarget(cpu_task_controller_t *p_controller, gui_controller_t *p_gui)
{
	if(p_controller->init == 1)
	{ 
        TASK_ENTRY_INIT(p_controller, 6); 
        Motor_AddEvent(3, 1, 1, 1);  
		
		// 注意:此处变量设置是为了立即刷新动画,否则可能会延时
		evnet_interval_cnt = 0xFFFF;
		p_gui->page_frame_counter = 0;
        button_pressed_cnt = 0;
		
		task_save = cpu_s.task;
		
#if	LED_ON_OFF_ON_DEF		
		Gui_LcdLedOff();
#endif	
	}
	
    // 刷新间隔 ms
	if(p_gui->page_frame_counter < 10)
	{
		p_gui->updata_ui_interval = 100;
	}
	else
	{
		p_gui->updata_ui_interval = 250;
	}
    
	if(evnet_interval_cnt != p_gui->page_frame_counter)
	{
        evnet_interval_cnt = p_gui->page_frame_counter;
        Gui_PageStarget(evnet_interval_cnt,evnet_interval_cnt, display_language);
    }
    
    if(p_gui->key_info.key != APP_BSP_KEY_NULL_PRESS)
    {
        if(p_gui->key_info.key == APP_BSP_KEY_SHORT_PRESS)
        {
			// 停止震动
			Motor_DelEvent(); 
			// 退出外部事件
			Gui_ThreadEventTaskClear();
			
			if(task_save != CPU_TASK_IDEL)
			{
				Cpu_TaskSchedule(&cpu_s.job_control, p_gui, task_save);
				task_save = CPU_TASK_IDEL;
			}
        }
        
        Gui_ClearKeyInfo();
    }
	
#if	LED_ON_OFF_ON_DEF		
	if(p_gui->page_frame_counter == 0)
	{
		Gui_LcdLedOn();
	}
#endif
}

/****************************************************************
** Function name:           Cpu_EvenPhysRemind
** Descriptions:            女性功能提醒
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-08-12
*****************************************************************/ 
void Cpu_EvenPhysRemind(cpu_task_controller_t *p_controller, gui_controller_t *p_gui)
{
    static uint8_t last_page_index = 0xFF; 
    
    uint8_t page_index = 0;
    uint8_t *display_buf = NULL;
    uint16_t buf_len = 0;
	uint8_t hour = 0;
	uint8_t minute = 0;
    
	if(p_controller->init == 1)
	{
        // 这里需要分类处理
        TASK_ENTRY_INIT(p_controller,  5);	
        
        if(Gui_DataIsReadPhysMessage() != 1)
        {
            //读取不需要马达震动
            Motor_AddEvent(1, 1, 1, 1);  
        }
        
		// 注意:此处变量设置是为了立即刷新动画,否则可能会延时
        evnet_interval_cnt = 0xFFFF;
		p_gui->page_frame_counter = 0;
        last_page_index = 0xFF;    
        button_pressed_cnt = 0; 
		
		task_save = cpu_s.task;
		
#if	LED_ON_OFF_ON_DEF		
		Gui_LcdLedOff();
#endif
	}

    // 刷新间隔 ms
    p_gui->updata_ui_interval = 100;
    
	if(evnet_interval_cnt != p_gui->page_frame_counter)
	{
        evnet_interval_cnt = p_gui->page_frame_counter;       
 
        display_buf = Gui_DataGetPhysRmindData(&buf_len, &page_index);       
        
        if(buf_len == 0)
        {
            // WJ.20180119.解决女性功能查看时关闭该功能，导致只显示UI的问题
            TASK_ENTRY_INIT(p_controller,  0);
        }
		
#if  1 //	JSH.20181015.第二种信息刷屏方式，注意改屏的左右间距，消息行数	
			
		hour = Gui_DataGetSystemTimeHour(); 
		minute = Gui_DataGetSystemTimeMinute();
		Gui_PageMessageNotificationIcon02(evnet_interval_cnt, GUI_MSG_TYPE_PHYS, 0,hour,minute,display_language);
			
		if(last_page_index != page_index)
		{
			if(last_page_index != 0xFF)
			{
				evnet_interval_cnt = 0;
			}

			Gui_PageMessageNotificationContent02(evnet_interval_cnt,display_buf, buf_len, page_index);
			last_page_index = page_index;
		}
		
#else		
        
        if(last_page_index != page_index)
        {      
            if(last_page_index != 0xFF)
            {
                evnet_interval_cnt = 0;
            }
			hour = Gui_DataGetSystemTimeHour(); 
			minute = Gui_DataGetSystemTimeMinute();
            Gui_PageMessageNotificationIcon(evnet_interval_cnt, GUI_MSG_TYPE_PHYS, 0, hour, minute , display_language);
            Gui_PageMessageNotificationContent(evnet_interval_cnt, display_buf, buf_len, page_index);
            last_page_index = page_index;
        }
		
#endif
		
	}

    if(p_gui->key_info.key != APP_BSP_KEY_NULL_PRESS)
    {
        if(p_gui->key_info.key == APP_BSP_KEY_SHORT_PRESS)
        {
            // 停止震动 
            Motor_DelEvent();
			
            if(Gui_DataHaveUnreadPhysMessage(1))
            {
                p_controller->keep_time = 5;
                p_controller->run_time = 0;
            }
            else
            {
                // 退出外部事件
                Gui_ThreadEventTaskClear();
				
				if(task_save != CPU_TASK_IDEL)
				{
					Cpu_TaskSchedule(&cpu_s.job_control, p_gui, task_save);
					task_save = CPU_TASK_IDEL;
				}
            }
        }

        Gui_ClearKeyInfo();
    }
	
#if	LED_ON_OFF_ON_DEF		
	if(p_gui->page_frame_counter == 0)
	{
		Gui_LcdLedOn();
	}
#endif		
}

/****************************************************************
** Function name:           Cpu_EvenBleDiscRemind
** Descriptions:            蓝牙断连提醒
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2018-04-03
*****************************************************************/ 
void Cpu_EvenBleDiscRemind(cpu_task_controller_t *p_controller, gui_controller_t *p_gui)
{
	if(p_controller->init == 1)
	{ 
        TASK_ENTRY_INIT(p_controller, 4); 
        Motor_AddEvent(1, 0, 1, 0);  
		
		// 注意:此处变量设置是为了立即刷新动画,否则可能会延时
		evnet_interval_cnt = 0xFFFF;
		p_gui->page_frame_counter = 0;
		
		task_save = cpu_s.task;
		
#if	LED_ON_OFF_ON_DEF		
		Gui_LcdLedOff();
#endif	
	}
	
    // 刷新间隔 350ms
    p_gui->updata_ui_interval = 350;
    
	if(evnet_interval_cnt != p_gui->page_frame_counter)
	{
        evnet_interval_cnt = p_gui->page_frame_counter;
        Gui_PageBleDiscRemind(evnet_interval_cnt);
    }
     
    if(p_gui->key_info.key != APP_BSP_KEY_NULL_PRESS)
    {   
        if(p_gui->key_info.key == APP_BSP_KEY_SHORT_PRESS)
        {
			// 停止震动
			Motor_DelEvent(); 
			// 退出外部事件
			Gui_ThreadEventTaskClear();
			
			if(task_save != CPU_TASK_IDEL)
			{
				Cpu_TaskSchedule(&cpu_s.job_control, p_gui, task_save);
				task_save = CPU_TASK_IDEL;
			}
        }
    }
	
#if	LED_ON_OFF_ON_DEF		
	if(p_gui->page_frame_counter == 0)
	{
		Gui_LcdLedOn();
	}
#endif	
}

