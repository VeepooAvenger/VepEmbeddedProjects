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


static uint8_t evnet_interval_cnt = 0;   //最后的间隔次数，用于判断是否刷新UI
static uint8_t event_msg_type = 0;


extern cpu_task_t cpu_s;

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
    evnet_interval_cnt = 0xFF;
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
        if(Gui_DataBatteryIsLowPower() == BATTERRY_IS_LOW_TO_SHUT)
        {
            TASK_ENTRY_INIT(p_controller, 6);
        }
        else
        {
            TASK_ENTRY_INIT(p_controller, 1);
        }
       
		//!注意:此处变量设置是为了立即刷新动画,否则可能会延时
        evnet_interval_cnt = 0xFF;
		p_gui->page_frame_counter = 0;
        
        //清除背景图
        Gui_PageClearBackground(p_gui->start_column, p_gui->start_page, p_gui->column_size, p_gui->page_size);
	}
    
    //300ms的刷新间隔
    p_gui->updata_ui_interval = 200;
    
	if(evnet_interval_cnt != p_gui->page_frame_counter)
	{	
		Gui_ThreadSetDisPlayArea(0, 0, 240, 240);
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
void Cpu_EventIdelCharge(cpu_task_controller_t *p_controller, gui_controller_t *p_gui)
{		
	uint8_t charge_state = Gui_DataGetBatteryLevelAndState(NULL);
    
	if(p_controller->init == 1)
	{
        TASK_ENTRY_INIT(p_controller, 8);
        
		//!注意:此处变量设置是为了立即刷新动画,否则可能会延时
        evnet_interval_cnt = 0xFF;
		p_gui->page_frame_counter = 0;
        
        
        //清除背景
        Gui_LcdLedOff();
        //清除背景图
        Gui_PageClearBackground(p_gui->start_column, p_gui->start_page, p_gui->column_size, p_gui->page_size);		
	}
    
    
    if(p_gui->page_frame_counter == 0)
    {
        Gui_LcdLedOff();
    }   
    
    
    //300ms的刷新间隔
    p_gui->updata_ui_interval = 300;

    // 如果充电状态改变,则立即关掉显示充电状态
    if(charge_state == BATTERRY_IS_NORMAL)
    {
        p_controller->keep_time = 0;
        p_controller->run_time = 1;
    }
//    else
//    {
//        if(!Gui_DataSystemIsPoweOn())
//        {
//            /*关机下做开机处理*/    
//            if(Gui_DataBatteryIsLowPower() != BATTERRY_IS_LOW_TO_SHUT)
//            {     
//                Gui_DataSystemToPoweOn();
//            }
//            return;
//        }
//    }

	if(evnet_interval_cnt != p_gui->page_frame_counter)
	{	
        evnet_interval_cnt = p_gui->page_frame_counter;	
        Gui_ThreadSetDisPlayArea(0, 0, 240, 240);
        // 如果充电状态满格时,则不再刷新
        if(charge_state == BATTERRY_IS_FULL)
        {            
            Gui_PageBatteryCharg(evnet_interval_cnt, 1);
        }
        else
        {        
            Gui_PageBatteryCharg(evnet_interval_cnt, 0);
        }
    }
    
    
    Gui_LcdLedOn();
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
        TASK_ENTRY_INIT(p_controller, 30);

        //提醒10组 每组振动1秒 间隔1秒
        Motor_AddEvent(10, 1, 1 * 20, 2 * 20);
		
		//!注意:此处变量设置是为了立即刷新动画,否则可能会延时
        evnet_interval_cnt = 0xFF;
		p_gui->page_frame_counter = 0;
        

         //清除背景
         Gui_LcdLedOff();
        //清除背景图
        Gui_PageClearBackground(p_gui->start_column, p_gui->start_page, p_gui->column_size, p_gui->page_size);
	}
    
    
    
    if(p_gui->page_frame_counter == 0)
    {
        Gui_LcdLedOff();
    }   
   
    
    
    //250ms的刷新间隔
    p_gui->updata_ui_interval = 300;
    
	if(evnet_interval_cnt != p_gui->page_frame_counter)
	{
        Gui_ThreadSetDisPlayArea(0, 0, 240, 240);
        evnet_interval_cnt = p_gui->page_frame_counter;
        time_format = Gui_DataTimeFormatDisplay(); 
        Gui_DataGetAlarmClockData(&alarm_hour, &alarm_minute, &alarm_mode);
        if(time_format != GUI_TIME_FORMAT_STANDARD)
        {
            alarm_hour = alarm_hour%12;
            if(alarm_hour == 0)
            {
                //YX.20170921.修复闹钟时间显示问题
                alarm_hour = 12;
            }
        }
        Gui_PageAlarm(Gui_DataTimeFormatDisplay(), alarm_mode, alarm_hour, alarm_minute, display_language, evnet_interval_cnt);
	}
    
     Gui_LcdLedOn();
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
        TASK_ENTRY_INIT(p_controller, 6);
        //提醒两组 每组振动3秒 间隔2秒
        Motor_AddEvent(2, 1, 1 * 20, 2 * 20); 

		//!注意:此处变量设置是为了立即刷新动画,否则可能会延时
        evnet_interval_cnt = 0xFF;
		p_gui->page_frame_counter = 0;
		
        
        //清除背景
        Gui_LcdLedOff();
        //清除背景图
        Gui_PageClearBackground(p_gui->start_column, p_gui->start_page, p_gui->column_size, p_gui->page_size);
	}
	
    
    if(p_gui->page_frame_counter == 0)
    {
        Gui_LcdLedOff();
    }   
    
    
    //300ms的刷新间隔
    p_gui->updata_ui_interval = 300;
    
	if(evnet_interval_cnt != p_gui->page_frame_counter)
	{
        Gui_ThreadSetDisPlayArea(0, 0, 240, 240);
        evnet_interval_cnt = p_gui->page_frame_counter;
		Gui_PageSit(evnet_interval_cnt);
	}
    
    
    Gui_LcdLedOn();
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
    static uint8_t last_page_index = 0xFF; 
    static uint8_t last_call_state = 0x00; 
    
    uint8_t page_index = 0;
    uint8_t *display_buf = NULL;
    uint16_t buf_len = 0;
    
	if(p_controller->init == 1)
	{
        //这里需要分类处理
        event_msg_type = Gui_DataPhoneNotifyDeal();      
        
        if(event_msg_type == GUI_MSG_TYPE_CALL)
        {
            if(Gui_DataMessageCallState() == GUI_CALL_REJECTSUC)
            {               
                TASK_ENTRY_INIT(p_controller, 3);	
                //清除背景
                Gui_LcdLedOff();
                //清除背景图
//                  Gui_PageClearBackground(0, 0, 80, 100);
            }
            else if(Gui_DataMessageCallState() == GUI_CALL_INCOMING || Gui_DataMessageCallState() == GUI_CALL_MUTE)
            {
                TASK_ENTRY_INIT(p_controller, 60);
                //清除背景
                Gui_LcdLedOff();
                //清除背景图
                Gui_PageClearBackground(p_gui->start_column, p_gui->start_page, p_gui->column_size, p_gui->page_size);	  
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
		//!注意:此处变量设置是为了立即刷新动画,否则可能会延时
        evnet_interval_cnt = 0xFF;
		p_gui->page_frame_counter = 0;
        last_page_index = 0xFF;          
	}
    
    
    
    if(p_gui->page_frame_counter == 0)
    {
        Gui_LcdLedOff();
    }   
    
    
    
    
    
    //250ms的刷新间隔
    p_gui->updata_ui_interval = 200;
    
	if(evnet_interval_cnt != p_gui->page_frame_counter)
	{ 
        Gui_ThreadSetDisPlayArea(0, 0, 240, 240); 
        evnet_interval_cnt = p_gui->page_frame_counter;       
        
        display_buf = Gui_DataGetPhoneNotifyData(&buf_len, &page_index);

        //这里需要分类处理
        if(event_msg_type == GUI_MSG_TYPE_CALL)
        {           
            if(Gui_DataMessageCallState() != GUI_CALL_NULL)
            {
                last_call_state = Gui_DataMessageCallState();    
            }
            
            if(last_page_index == 0xFF && (last_call_state == GUI_CALL_INCOMING || last_call_state == GUI_CALL_MUTE))
            {
                Gui_PageCallNotificationContent(display_buf, buf_len);
                last_page_index = 0xFE;
                evnet_interval_cnt = 0;
            }
            Gui_PageCallNotificationIcon(last_call_state, evnet_interval_cnt);
        }
	}
    
    Gui_LcdLedOn();
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
    
	if(p_controller->init == 1)
	{
        //这里需要分类处理
        event_msg_type = Gui_DataPhoneNotifyDeal();
        
        if(event_msg_type >= GUI_MSG_TYPE_MESSAGE && event_msg_type <= GUI_MSG_TYPE_OTHER)
        {
            TASK_ENTRY_INIT(p_controller, 6);//5);	
        }
        else
        {
            TASK_ENTRY_INIT(p_controller, 1);	
        }
        
		//!注意:此处变量设置是为了立即刷新动画,否则可能会延时
        evnet_interval_cnt = 0xFF;
		p_gui->page_frame_counter = 0;
        last_page_index = 0xFF;
        
        
        //清除背景
        Gui_LcdLedOff();
        //清除背景图
        Gui_PageClearBackground(p_gui->start_column, p_gui->start_page, p_gui->column_size, p_gui->page_size);	      
	}

    
    if(p_gui->page_frame_counter == 0)
    {
        Gui_LcdLedOff();
    }   
    
    
    //400ms的刷新间隔
    p_gui->updata_ui_interval = 400;
    
	if(evnet_interval_cnt != p_gui->page_frame_counter)
	{
        Gui_ThreadSetDisPlayArea(0, 0, 240, 240); 
        evnet_interval_cnt = p_gui->page_frame_counter;       

        display_buf = Gui_DataGetPhoneNotifyData(&buf_len, &page_index);
        
        //这里需要分类处理
        if(event_msg_type >= GUI_MSG_TYPE_MESSAGE && event_msg_type <= GUI_MSG_TYPE_OTHER)
        {
            if(last_page_index != page_index)
            {
                if(last_page_index != 0xFF)
                {
                    Gui_PageClearBackground(0, 0, 240, 240);
                }
                Gui_PageMessageNotificationContent(display_buf, buf_len, page_index, event_msg_type);
                last_page_index = page_index;
            }
//            if(page_index <= 1)
//            {
//                Gui_PageMessageNotificationIcon(event_msg_type, evnet_interval_cnt);
//            }
        }
	}
    
    
    Gui_LcdLedOn();
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
//	if(p_controller->init == 1)
//	{
//        TASK_ENTRY_INIT(p_controller, 8);
//        //提醒2组 每组振3秒 间隔2秒
//        Motor_AddEvent(2, 1, 3, 2);
//        
//		//!注意:此处变量设置是为了立即刷新动画,否则可能会延时
//        evnet_interval_cnt = 0xFF;
//		p_gui->page_frame_counter = 0;
//		
//        //清除背景图
//        Gui_PageClearBackground(p_gui->start_column, p_gui->start_page, p_gui->column_size, p_gui->page_size);       
//	}
//	
//    //200ms的刷新间隔
//    p_gui->updata_ui_interval = 300;
//    
//	if(evnet_interval_cnt != p_gui->page_frame_counter)
//	{
//        Gui_ThreadSetDisPlayArea(0, 0, 240, 240);
//        evnet_interval_cnt = p_gui->page_frame_counter;
//		Gui_PageHeartAlert(evnet_interval_cnt);
//	}
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

		//!注意:此处变量设置是为了立即刷新动画,否则可能会延时
        evnet_interval_cnt = 0xFF;
		p_gui->page_frame_counter = 0;
		
        //清除背景图
        Gui_PageClearBackground(p_gui->start_column, p_gui->start_page, p_gui->column_size, p_gui->page_size);
	}
	
    //300ms的刷新间隔
    p_gui->updata_ui_interval = 300;
    
	if(evnet_interval_cnt != p_gui->page_frame_counter)
	{
        Gui_ThreadSetDisPlayArea(0, 0, 240, 240);
        evnet_interval_cnt = p_gui->page_frame_counter;
		Gui_PageFontTest();
	}
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
//	if(p_controller->init == 1)
//	{
//		TASK_ENTRY_INIT(p_controller, 6);
//		
//		//!注意:此处变量设置是为了立即刷新动画,否则可能会延时
//		evnet_interval_cnt = 0xFF;
//		p_gui->page_frame_counter = 0;
//		
//		Motor_AddEvent(3, 1, 1 * 20, 1 * 20);
//		
//        //清除背景图
//        Gui_PageClearBackground(p_gui->start_column, p_gui->start_page, p_gui->column_size, p_gui->page_size);
//	}
//	
//    //清除背景
//    Gui_LcdLedOff();
//    if(p_gui->page_frame_counter == 0)
//    {
//        Gui_LcdLedOff();
//    }   
//    Gui_LcdLedOn();
//    
//    //300ms的刷新间隔
//    p_gui->updata_ui_interval = 300;
//    
//	if(evnet_interval_cnt != p_gui->page_frame_counter)
//	{
//        Gui_ThreadSetDisPlayArea(10, 50, 70, 110);
//        evnet_interval_cnt = p_gui->page_frame_counter;
//        Gui_PageCountDown(1, evnet_interval_cnt);
//    }
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
//	if(p_controller->init == 1)
//	{
//		TASK_ENTRY_INIT(p_controller, 3);
//		
//		//!注意:此处变量设置是为了立即刷新动画,否则可能会延时
//		evnet_interval_cnt = 0xFF;
//		p_gui->page_frame_counter = 0;
//		
//        //清除背景图
//        Gui_PageClearBackground(p_gui->start_column, p_gui->start_page, p_gui->column_size, p_gui->page_size);
//	}
//	
//    //300ms的刷新间隔
//    p_gui->updata_ui_interval = 300;
//    
//	if(evnet_interval_cnt != p_gui->page_frame_counter)
//	{
//        Gui_ThreadSetDisPlayArea(10, 50, 70, 110);
//        evnet_interval_cnt = p_gui->page_frame_counter;
//        Gui_PageCountDown(1, evnet_interval_cnt);
//    }
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
		
		//!注意:此处变量设置是为了立即刷新动画,否则可能会延时
		evnet_interval_cnt = 0xFF;
		p_gui->page_frame_counter = 0;
		
        //清除背景
    Gui_LcdLedOff();
        //清除背景图
        Gui_PageClearBackground(p_gui->start_column, p_gui->start_page, p_gui->column_size, p_gui->page_size);
	}
	
    //清除背景
    Gui_LcdLedOff();
    if(p_gui->page_frame_counter == 0)
    {
        Gui_LcdLedOff();
    }   
    
    
    //300ms的刷新间隔
    p_gui->updata_ui_interval = 500;
    
	if(evnet_interval_cnt != p_gui->page_frame_counter)
	{
        Gui_ThreadSetDisPlayArea(0, 0, 240, 240);
        evnet_interval_cnt = p_gui->page_frame_counter;
		Gui_PageCameraReady(evnet_interval_cnt);
	}
    
    Gui_LcdLedOn();
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
		TASK_ENTRY_INIT(p_controller, 6);
		 
		//!注意:此处变量设置是为了立即刷新动画,否则可能会延时
		evnet_interval_cnt = 0xFF;
		p_gui->page_frame_counter = 0;
		
        //清除背景
    Gui_LcdLedOff();
        
		//清除背景图
        Gui_PageClearBackground(p_gui->start_column, p_gui->start_page, p_gui->column_size, p_gui->page_size);
	}
	
  
    if(p_gui->page_frame_counter == 0)
    {
        Gui_LcdLedOff();
    }   
   
    
    //300ms的刷新间隔
    p_gui->updata_ui_interval = 500;   
	if(evnet_interval_cnt != p_gui->page_frame_counter)
	{
        Gui_ThreadSetDisPlayArea(0, 0, 240, 240);
        evnet_interval_cnt = p_gui->page_frame_counter;        
		Gui_PageCameraOk(evnet_interval_cnt);
	}
    
     Gui_LcdLedOn();
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
void Cpu_EvenDisMacVer(cpu_task_controller_t *p_controller, gui_controller_t *p_gui)
{
    uint8_t dis_mac_data[20] = {0};
    uint8_t dis_ver_data[10] = {0};
	if(p_controller->init == 1)
	{
        TASK_ENTRY_INIT(p_controller, 6);  
		 
		//!注意:此处变量设置是为了立即刷新动画,否则可能会延时
		evnet_interval_cnt = 0xFF;
		p_gui->page_frame_counter = 0;
		Gui_DataGetBleMac(dis_mac_data);
        Gui_DataGetVer(dis_ver_data);
        
        
        //清除背景
    Gui_LcdLedOff();
		//清除背景图
        Gui_PageClearBackground(p_gui->start_column, p_gui->start_page, p_gui->column_size, p_gui->page_size);
	}
	
    
    
    if(p_gui->page_frame_counter == 0)
    {
        Gui_LcdLedOff();
    }   
  
    
    //300ms的刷新间隔
    p_gui->updata_ui_interval = 300;   
	if(evnet_interval_cnt != p_gui->page_frame_counter)
	{
        Gui_ThreadSetDisPlayArea(0, 0, 240, 240);
        evnet_interval_cnt = p_gui->page_frame_counter;
        Gui_PageDisMacVer(evnet_interval_cnt, dis_mac_data, dis_ver_data);
	}
      Gui_LcdLedOn();
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
        Motor_AddEvent(2, 1, 1 * 20, 2 * 20);  
		
		//!注意:此处变量设置是为了立即刷新动画,否则可能会延时
		evnet_interval_cnt = 0xFF;
		p_gui->page_frame_counter = 0;
        
        
        //清除背景
    Gui_LcdLedOff();
        //清除背景图
        Gui_PageClearBackground(p_gui->start_column, p_gui->start_page, p_gui->column_size, p_gui->page_size);
	}
	
    
    
    if(p_gui->page_frame_counter == 0)
    {
        Gui_LcdLedOff();
    }   
   
    
    //300ms的刷新间隔
    p_gui->updata_ui_interval = 300;
    
	if(evnet_interval_cnt != p_gui->page_frame_counter)
	{
        Gui_ThreadSetDisPlayArea(0, 0, 240, 240);
        evnet_interval_cnt = p_gui->page_frame_counter;
        Gui_PageStarget(evnet_interval_cnt);
    }
    
     Gui_LcdLedOn();
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
    
	if(p_controller->init == 1)
	{
        //这里需要分类处理
        TASK_ENTRY_INIT(p_controller,  6);	
        
        if(Gui_DataIsReadPhysMessage() != 1)
        {
            //读取不需要马达震动
            Motor_AddEvent(1, 1, 0.5 * 20, 1 * 20);  
        }
        
		//!注意:此处变量设置是为了立即刷新动画,否则可能会延时
        evnet_interval_cnt = 0xFF;
		p_gui->page_frame_counter = 0;
        last_page_index = 0xFF;
        
        
        //清除背景
    Gui_LcdLedOff();
        //清除背景图
        Gui_PageClearBackground(p_gui->start_column, p_gui->start_page, p_gui->column_size, p_gui->page_size);	      
	}
    
    
    
    if(p_gui->page_frame_counter == 0)
    {
        Gui_LcdLedOff();
    }   
    

    //400ms的刷新间隔
    p_gui->updata_ui_interval = 400;
    
	if(evnet_interval_cnt != p_gui->page_frame_counter)
	{
        Gui_ThreadSetDisPlayArea(0, 0, 240, 240); 
        evnet_interval_cnt = p_gui->page_frame_counter;       
 
        display_buf = Gui_DataGetPhysRmindData(&buf_len, &page_index);       
        
        if(buf_len == 0)
        {
            //WJ.20180119.解决女性功能查看时关闭该功能，导致只显示UI的问题
            TASK_ENTRY_INIT(p_controller,  0);
        }
        
        if(last_page_index != page_index)
        {      
            if(last_page_index != 0xFF)
            {
                Gui_PageClearBackground(0, 0, 240, 240);
            }
            Gui_PageMessageNotificationContent(display_buf, buf_len, page_index,GUI_MSG_TYPE_PHYS);
            last_page_index = page_index;
        }
//        if(page_index <= 1)
//        {
//            Gui_PageMessageNotificationIcon(GUI_MSG_TYPE_PHYS, evnet_interval_cnt);
//        }
	}   
    
    
    Gui_LcdLedOn();
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
        TASK_ENTRY_INIT(p_controller, 6); 
        
        if(disconnet_motor_flg == 0)
        {
            Motor_AddEvent(2, 1, 0.5 * 20, 2 * 20);  
		}
        disconnet_motor_flg = 0;
        
		//!注意:此处变量设置是为了立即刷新动画,否则可能会延时
		evnet_interval_cnt = 0xFF;
		p_gui->page_frame_counter = 0;
        
        //清除背景
        Gui_LcdLedOff();
        //清除背景图
        Gui_PageClearBackground(p_gui->start_column, p_gui->start_page, p_gui->column_size, p_gui->page_size);
	}
	
    if(p_gui->page_frame_counter == 0)
    {
        Gui_LcdLedOff();
    }   
    
    
    //300ms的刷新间隔
    p_gui->updata_ui_interval = 400;
    
	if(evnet_interval_cnt != p_gui->page_frame_counter)
	{
        Gui_ThreadSetDisPlayArea(0, 0, 240, 240);
        evnet_interval_cnt = p_gui->page_frame_counter;
        Gui_PageBleDiscRemind(evnet_interval_cnt);
    }
    
    
    Gui_LcdLedOn();
}

/****************************************************************
** Function name:           Cpu_EvenCartoon
** Descriptions:            开机动画
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2018-06-05
*****************************************************************/ 
void Cpu_EvenCartoon(cpu_task_controller_t *p_controller, gui_controller_t *p_gui)
{
     if(p_controller->init == 1)
	{
        TASK_ENTRY_INIT(p_controller, 5);
        
		//!注意:此处变量设置是为了立即刷新动画,否则可能会延时
        evnet_interval_cnt = 0xFF;
		p_gui->page_frame_counter = 0;
		
        
         //清除背景
        Gui_LcdLedOff();
        //清除背景图
        Gui_PageClearBackground(p_gui->start_column, p_gui->start_page, p_gui->column_size, p_gui->page_size);
	}
    
    if(p_gui->page_frame_counter == 0)
    {
        Gui_LcdLedOff();
    }   
    
    //300ms的刷新间隔
    p_gui->updata_ui_interval = 200;//220;
    
	if(evnet_interval_cnt != p_gui->page_frame_counter)
	{
        Gui_ThreadSetDisPlayArea(0, 0, 240, 240);
        evnet_interval_cnt = p_gui->page_frame_counter;
        if(evnet_interval_cnt >= 15)
        {
            p_controller->run_time = 5;
            p_controller->keep_time = 4;    
            
            cpu_s.task = CPU_TASK_MAIN;	
            cpu_s.job_control.run_time = 0;
            cpu_s.job_control.keep_time = 5;
            cpu_s.job_control.init = 0x01;     
            
        }
        Gui_PageCartoon(evnet_interval_cnt);
	}   
    
    Gui_LcdLedOn();
}
/****************************************************************
** Function name:           Cpu_EvenCartoon
** Descriptions:            关机动画
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2018-06-05
*****************************************************************/ 
void Cpu_EvenCartoon1(cpu_task_controller_t *p_controller, gui_controller_t *p_gui)
{
     if(p_controller->init == 1)
	{
        TASK_ENTRY_INIT(p_controller, 11);
        
		//!注意:此处变量设置是为了立即刷新动画,否则可能会延时
        evnet_interval_cnt = 0xFF;
		p_gui->page_frame_counter = 0;
		
        
       //清除背景
    Gui_LcdLedOff();
        //清除背景图
        Gui_PageClearBackground(p_gui->start_column, p_gui->start_page, p_gui->column_size, p_gui->page_size);
	}
	
    
    if(p_gui->page_frame_counter == 0)
    {
        Gui_LcdLedOff();
    }   
    
    //300ms的刷新间隔
    p_gui->updata_ui_interval = 220;
    
	if(evnet_interval_cnt != p_gui->page_frame_counter)
	{
        Gui_ThreadSetDisPlayArea(0, 0, 240, 240);
        evnet_interval_cnt = p_gui->page_frame_counter;
        if(evnet_interval_cnt >= 13)
        {
            p_controller->run_time = 5;
            p_controller->keep_time = 4;

        }
        Gui_PageCartoon1(evnet_interval_cnt);
	}  

    Gui_LcdLedOn();    
}
