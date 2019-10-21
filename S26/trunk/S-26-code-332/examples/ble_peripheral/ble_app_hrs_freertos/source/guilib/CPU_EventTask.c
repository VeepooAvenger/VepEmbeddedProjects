/*******************Copyright(c)*********************************
** Veepoo Tech
** COPYRIGHT (C) 2016-2019
**----------------------FileInfo---------------------------------
** File name:                cpu_event_task.c
** Last modified Date:       2017-07-10
** Last SYS_VERSION:         0.0.1
** Descriptions:
**---------------------------------------------------------------
** Createdby:                郭涛
** Createddate:              2017-07-10
** SYS_VERSION:              0.0.1
** Descriptions:             cpu事件任务
**---------------------------------------------------------------
** Modified by:				
** Modified date:			
** SYS_VERSION:				
** Modified Descriptions:	
****************************************************************/

#include <string.h>
#include "CPU_EventTask.h"
#include "CPU_GeneralTask.h"
#include "motor_event.h"
#include "SystemTime.h"
#include "ancs_notify_event.h"
#include "battery_adc.h"
#include "data_manage.h"
#include "api_flash.h"
#include "GSensor.h"
#include "util.h"
#include "ble_thread.h"
#include "log.h"
#include "bracelet_protocol.h"
#include "AFE4900_entry.h"
#include "nrf_delay.h"
#include "event_manage.h"
#include "sleep.h"
#include "Gui_Thread.h"
#include "app_ancs.h"
#include "app_hid.h"
#include "pedometer.h"
#include "movement_patterns.h"
#include "posture_get.h"
#include "gui_page_display.h"
#include "lcd_driver_tft.h"
#include "Gui_Thread.h"
#include "gui_data_inteface.h"
#include "ble_disconnect_remind.h"
#include "gui_flash_param.h"

uint8_t camera_delay_cnt = 0;       //拍照延时计数，大于1，震动才会响应拍照
void Algorithm_OledBrightnessAdjustTask(void);

void Cpu_EventParamInit(cpu_event_tcb_t* p_event,uint16_t keep_time,bool press_hold,bool animation_active)
{
    // 任务初始化
    p_event->task_controller.init = 0; 
    p_event->task_controller.active = 1; 
    p_event->task_controller.keep_time = keep_time; 
    p_event->task_controller.run_time = 0;
    p_event->task_controller.key_press_hold = press_hold; 

    p_event->gui_controller.animation_enable = animation_active;
    // 注意:这里变量初始化为0xFF,主要处理静态页面刷新控制
    p_event->gui_controller.page_frame_counter = 0xFF;
    p_event->gui_controller.animation_tick_counter = 0;
    p_event->gui_controller.animation_frame_index = 0;	
}

void Cpu_ScheduleEvent(cpu_event_tcb_t* p_event, cpu_device_tcb_t* p_device, CPU_EVENT_TYPE event_id)
{
    p_device->key = BUTTON_EVENT_NULL;
    p_event->event_id = event_id;
    p_event->task_controller.init = 1;	
    p_device->oled.light = 1;
}

void Cpu_ClearCurrentEvent(cpu_event_tcb_t* p_event, cpu_device_tcb_t* p_device)
{
    //YX.20180427.修复显示外部事件的一瞬间，有按键操作，造成外部事件界面与任务界面重叠的问题
    GuiThread_ClearTaskParam();
    p_device->key = BUTTON_EVENT_NULL;
    if(p_data_manage->device_trace_param_s.factory_mode == 0)
    {
        MotorDelEvent();
    }
    memset(p_event, 0, sizeof(cpu_event_tcb_t));
}

void Cpu_TurnwristStartEvent(cpu_task_tcb_t* p_task, cpu_event_tcb_t* p_event, movement_cont_t *p_movement, watch_timer_t *p_timer, cpu_device_tcb_t* p_device)
{
    /* 
     *    =====翻腕亮屏事务逻辑处理====
     *    1.如果有秒表开启,则翻腕需要亮秒表功能;
     *    2.如果没有秒表开启,则需要亮主界面;
     *
     */
    if(p_data_manage->device_trace_param_s.sign_on == 0)
    {
        //关机下不能进行翻腕功能
        return ;
    }
    if((p_task->task_id == CPU_TASK_IDEL)&&(p_event->event_id == CPU_EVENT_TASK_IDEL))
    {
        if(p_timer->run == 1)
        {
            Cpu_IdelToTimerTask(p_task, p_device);     
        }
        else if(p_movement->enable == 1)
        {
            p_movement->face_num = 1;
            Cpu_IdelToRunTask(p_task, p_device);
        }
        else if(p_data_manage->event_list.count == 0)
        {
			if(Battery_Get_Grade() == 0)
			{
				Cpu_ScheduleEvent(p_event, p_device, CPU_EVENT_TASK_LOWPOWER);
			}
			else
			{
				Cpu_ScheduleTask(p_task, p_device, CPU_TASK_MAIN);
			}
//            Cpu_ScheduleTask(p_task, p_device, CPU_TASK_MAIN);
        }
    }
}

void Cpu_TurnwristStopEvent(cpu_task_tcb_t* p_task, cpu_event_tcb_t* p_event, watch_timer_t *p_timer, cpu_device_tcb_t* p_device)
{
    if(p_data_manage->device_trace_param_s.sign_on == 0)
    {
        //关机下不能进行翻腕功能
        return ;
    }
    // 翻腕灭屏逻辑处理
    if((p_task->task_id == CPU_TASK_MAIN)&&(p_event->event_id == CPU_EVENT_TASK_IDEL))
    {
        if(p_data_manage->event_list.count == 0)
        {
            Cpu_ClearSechduleTask(p_task);
        }
    }  
}

/****************************************************************
** Function name:            Cpu_LowPowerHintEvent
** Descriptions:             低电量事件
** input parameters:         p_controller：       CPU任务控制器指针
                             p_gui：              静态页面控制器指针
** output parameters:        无
** Returned value:           无
** Created by:               郭涛
** Created Date:             2017-07-10
****************************************************************/
void Cpu_LowPowerHintEvent(cpu_task_tcb_t* p_task, cpu_event_tcb_t* p_event, cpu_device_tcb_t* p_device)
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
        Gui_ThreadSetDisPlayArea(16, 33, 48, 91);
		Gui_PageLowPower(p_event->gui_controller.animation_frame_index++);
    }
    
    //低电运行1S后切换到主界面
    if(p_event->task_controller.run_time >= 1 && p_data_manage->device_trace_param_s.sign_on == 1)
    {
        if(p_data_manage->device_trace_param_s.low_power == 0)
        {
            Cpu_ScheduleTask(p_task, p_device, CPU_TASK_MAIN);
        }
        else
        {
            Cpu_ScheduleEvent(p_event, p_device, CPU_EVENT_TASK_POWEROFF);
        }
    }
}

/****************************************************************
** Function name:            Cpu_IdelChargeEvent
** Descriptions:             空闲充电事件
** input parameters:         p_controller：         CPU任务控制器指针
                             p_gui：                静态页面控制器指针
** output parameters:        无
** Returned value:           无
** Created by:               郭涛
** Created Date:             2017-07-10
****************************************************************/
void Cpu_IdelChargeEvent(cpu_task_tcb_t* p_task, cpu_event_tcb_t* p_event, cpu_device_tcb_t* p_device)
{            
    if(p_event->task_controller.init == 1)
    {
        // 如果是充满状态,则显示8S
        if(Battery_Get_Status() == BATTERY_POWER_FULL)
        {
            Cpu_EventParamInit(p_event,8,false,true);  
        }
        else
        {
            Cpu_EventParamInit(p_event,8,false,true); 
        }
        //清除背景
        Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
                                p_device->cls_param.column_size, p_device->cls_param.page_size);
    }
	
	// 充电时停止震动 Add by GT 2017-11-01
	MotorDelEvent();
    
    // 如果充电状态满格时,则不再刷新
    if(Battery_Get_Status() == BATTERY_POWER_FULL)
    {
        if(p_event->gui_controller.animation_tick_counter%4 == 0)
         {
             //设置清屏参数
             Gui_ThreadSetDisPlayArea(16, 33, 49, 90); 
			 Gui_PageBatteryCharg(p_event->gui_controller.animation_frame_index++, 1);
         }
    }
    else 
    {
        if((p_event->gui_controller.animation_tick_counter%4) == 0)
        {
             //设置清屏参数
             Gui_ThreadSetDisPlayArea(16, 33, 49, 90); 

			 Gui_PageBatteryCharg(p_event->gui_controller.animation_frame_index++, 0);
        }
    }

    // 按键事件处理
    if((p_device->key != BUTTON_EVENT_NULL) || (Battery_Get_Status() ==  BATTERY_POWER_NORMAL))
    {
        p_device->key = BUTTON_EVENT_NULL;
		if(p_data_manage->device_trace_param_s.sign_on == 1)
		{
			Cpu_ClearCurrentEvent(p_event, p_device);
			Cpu_ScheduleTask(p_task, p_device, CPU_TASK_MAIN); 
		}
		else if(Battery_Get_Status() ==  BATTERY_POWER_NORMAL)
		{
			Cpu_ClearCurrentEvent(p_event, p_device);
		}
    }
}

/****************************************************************
** Function name:            Cpu_AlarmHintEvent
** Descriptions:             闹铃到时事件
** input parameters:         p_controller：         CPU任务控制器指针
                             p_gui：                静态页面控制器指针
** output parameters:        无
** Returned value:           无
** Created by:               郭涛
** Created Date:             2017-07-10
****************************************************************/
void Cpu_AlarmHintEvent(cpu_event_tcb_t* p_event, cpu_device_tcb_t* p_device)
{
    uint8_t am_pm_format = 0;
    uint8_t display_hour = 0;
	
    if(p_event->task_controller.init == 1)
    {
        Cpu_EventParamInit(p_event,20,false,true);
        MotorAddEvent(10, 1, 1, 1);//提醒10组 每组振动1秒 间隔1秒
        //清除背景
        Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
                                p_device->cls_param.column_size, p_device->cls_param.page_size);
    }
    
    if((p_event->gui_controller.animation_tick_counter%4) == 0)
    {    
        display_hour = Hour_StandardTransform(p_flash_param->dev_config_s.am_pm_enable,clock->hour);
        am_pm_format = Get_AMPM_Format(p_flash_param->dev_config_s.am_pm_enable,clock->hour);
	    
        p_event->child_event_id = p_data_manage->alarm_scence;
		Gui_PageAlarm(am_pm_format, p_data_manage->alarm_scence,display_hour, clock->minutes, display_language,
		              p_event->gui_controller.animation_frame_index++);
        //设置清屏参数
        Gui_ThreadSetDisPlayArea(0, 0, 80, 160);
    }
    
    //如果用户按动按键,则提醒需要消掉
    if(p_device->key != BUTTON_EVENT_NULL)
    {
        Cpu_ClearCurrentEvent(p_event, p_device);	    
    }
}

/****************************************************************
** Function name:            Cpu_SedentaryHintEvent
** Descriptions:             久坐提醒事件
** input parameters:         p_controller：         CPU任务控制器指针
                             p_gui：                静态页面控制器指针
** output parameters:        无
** Returned value:           无
** Created by:               郭涛
** Created Date:             2017-07-10
****************************************************************/
void Cpu_SedentaryHintEvent(cpu_event_tcb_t* p_event, cpu_device_tcb_t* p_device)
{
    if(p_event->task_controller.init == 1)
    {
        Cpu_EventParamInit(p_event,8,false,true);
        MotorAddEvent(2,1,3,2); 
        //清除背景
        Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
                                p_device->cls_param.column_size, p_device->cls_param.page_size);
    }
    
    if(p_event->gui_controller.animation_tick_counter%6 == 0)
    {    
        p_event->gui_controller.page_frame_counter = clock->seconds;
		Gui_PageSit(p_event->gui_controller.animation_frame_index++);
        //设置清屏参数
        Gui_ThreadSetDisPlayArea(0, 40, 80, 80);
    }
    
    //如果用户按动按键,则提醒需要消掉
    if(p_device->key != BUTTON_EVENT_NULL)
    {
        Cpu_ClearCurrentEvent(p_event, p_device);		    
    }
}

/****************************************************************
** Function name:            Cpu_CallHintEvent
** Descriptions:             来电事件
** input parameters:         p_controller：         CPU任务控制器指针
                             p_gui：                静态页面控制器指针
** output parameters:        无
** Returned value:           无
** Created by:               郭涛
** Created Date:             2017-07-10
****************************************************************/
void Cpu_CallHintEvent(cpu_event_tcb_t* p_event, cpu_device_tcb_t* p_device)
{
    static uint8_t reject_call_state = 0;
    static uint8_t last_call_state = 0x00;
    static uint8_t last_page_index = 0xFF;
    call_notify_content_t *p_call_notify = &(p_data_manage->app_notify_s.call_content_s);
    
    if(p_event->task_controller.init == 1)
    {
        last_page_index = 0xFF;  
        reject_call_state = 0;
        Cpu_EventParamInit(p_event,60,true,true);        
        MotorAddEvent(20,1,1,2);
        Ble_SendBatMulData(3,0xB6,0x01,0x00);
        EventManage_ClearEvent(CPU_EVENT_TASK_CAMERA);
        //清除背景
        Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
                                p_device->cls_param.column_size, p_device->cls_param.page_size);
    }
    
    if(reject_call_state == 1 && p_call_notify->reject_suc == 1)
    {
        reject_call_state = 0;
        p_call_notify->mute_call = 0;
        p_event->task_controller.keep_time = 3;
        p_event->task_controller.run_time = 0;
    }
    
    if((p_event->gui_controller.animation_tick_counter%5) == 0)
    {      
        // 重置事件子类型ID
        p_event->child_event_id = p_call_notify->id;
        
        if(p_call_notify->reject_call == 1 && p_call_notify->reject_suc == 1)
        {
            // 拒接电话  
            last_call_state = GUI_CALL_REJECTSUC;
        }
        else if(p_call_notify->mute_call == 1)
        {
            // 静音电话    
            last_call_state = GUI_CALL_MUTE;
        }
        else if(p_call_notify->hang_up_call == 1)
        {
            p_event->task_controller.keep_time = 0;
            p_event->task_controller.run_time = 0; 		  
        }
        else
        {
            last_call_state = GUI_CALL_INCOMING;
        }
		Gui_PageCallNotificationIcon(last_call_state,p_event->gui_controller.animation_frame_index++);
        //这里需要分类处理
        if(last_page_index == 0xFF && (last_call_state == GUI_CALL_INCOMING || last_call_state == GUI_CALL_MUTE))
        {
            Gui_PageCallNotificationContent(p_call_notify->content, p_call_notify->len);
        }
        //设置清屏参数
        Gui_ThreadSetDisPlayArea(0, 0, 80, 160);
    }
        
    //如果用户按动按键,则提醒需要消掉
    if(p_device->key != BUTTON_EVENT_NULL)
    {	    
        if(p_device->key == BUTTON_EVENT_LONGPRESS && p_call_notify->hang_up_call !=1)
        {
            Ble_SendBatMulData(3,0xC1,0x02,0x00);	
         
            // 如果ANCS连接了,则为
            App_AncsOperationCalls();
        
//            p_call_notify->mute_call = 0;
            p_call_notify->hang_up_call = 0;//0           
            p_call_notify->reject_call = 1;	
            reject_call_state = 1;            
        }
     
        if(p_device->key == BUTTON_EVENT_CLICK && p_call_notify->hang_up_call !=1 
           && p_call_notify->reject_call != 1 && p_call_notify->reject_suc != 1)
        {
			Hid_AudioVolumeAdd();
            Ble_SendBatMulData(3,0xC1,0x03,0x00);	
            p_call_notify->mute_call = 1;		            
        }
	    
        p_device->key = BUTTON_EVENT_NULL;	 
    }
}

/****************************************************************
** Function name:            Cpu_MessageHintEvent
** Descriptions:             信息到来事件
** input parameters:         p_controller：         CPU任务控制器指针
                             p_gui：                静态页面控制器指针
** output parameters:        无
** Returned value:           无
** Created by:               郭涛
** Created Date:             2017-07-10
****************************************************************/
void Cpu_MessageHintEvent(cpu_task_tcb_t* p_task, cpu_event_tcb_t* p_event, watch_timer_t *p_timer, movement_cont_t *p_movement,cpu_device_tcb_t* p_device)
{
    message_notify_content_t *p_message_notify = &p_data_manage->app_notify_s.message_content_s[p_data_manage->app_notify_s.update_index%2];
    
    if(p_event->task_controller.init == 1)
    {
        // 显示长度
        p_data_manage->app_notify_s.display_len = 0;//
        Cpu_EventParamInit(p_event,5,false,true);
        if(p_data_manage->app_notify_s.unread_msg_count != 0xFF)
        {
            //解决读取未读消息出现震动问题
            //WJ.20190105.修改消息提醒马达震动1s(减少功耗)
            MotorAddEvent(1,0,1,0);
        }
        else
        {
            //解决读取未读消息出现可以一直读取问题
            p_data_manage->app_notify_s.unread_msg_count = 0;
        }
        p_event->child_event_id = (uint16_t)(p_message_notify->type);
        //清除背景
        Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
                                p_device->cls_param.column_size, p_device->cls_param.page_size);
    }

    //如果用户按动按键,则提醒需要消掉
    if(p_device->key != BUTTON_EVENT_NULL)
    {	
        if(p_message_notify->len > 0)
        {
            uint8_t copy_content_len = p_message_notify->len - p_data_manage->app_notify_s.display_len;
            
            // 清除显示的内容并且更新相关参数
            if(p_message_notify->len >= p_data_manage->app_notify_s.display_len)
            {
                memcpy(p_message_notify->content,&p_message_notify->content[p_data_manage->app_notify_s.display_len], copy_content_len);
                memset(&p_message_notify->content[copy_content_len], 0, sizeof(p_message_notify->content)-copy_content_len);
                p_message_notify->len = copy_content_len;
            }
            // 重新初始化显示参数
            Cpu_EventParamInit(p_event,4,false,true);
            //清除背景
            Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
                                    p_device->cls_param.column_size, p_device->cls_param.page_size);
//            Gui_PageClearBackground(icon_16_message_text_coord[0][0], icon_16_message_text_coord[0][1], 
//                                    80, 160-icon_16_message_text_coord[0][1]);
        }
        p_data_manage->app_notify_s.unread_msg_count = 0;
        p_device->key = BUTTON_EVENT_NULL;	 
    }
    
    if(p_event->gui_controller.animation_tick_counter == 0)
    {
        p_event->child_event_id = (uint16_t)(p_message_notify->type);
        //设置清屏参数
        Gui_ThreadSetDisPlayArea(0, 0, 80, 160);
//        p_data_manage->app_notify_s.display_len = Gui_UpdateMessage(p_message_notify->type, p_message_notify->content, p_message_notify->len); 
		
        //YX.20180427.解决读取完推送消息，点击下一界面后UI显示问题，会闪屏一次
//		Gui_PageMessageNotificationIcon(p_message_notify->type,p_event->gui_controller.animation_frame_index++);
		p_data_manage->app_notify_s.display_len = Gui_PageMessageNotificationContent(p_message_notify->content, p_message_notify->len,p_event->gui_controller.animation_frame_index++);
		
        if((p_data_manage->app_notify_s.display_len == 0) || (p_message_notify->len > sizeof(p_message_notify->content)))
        {
            Cpu_ClearCurrentEvent(p_event, p_device);
            if(p_task->task_id == CPU_TASK_IDEL && p_task->child_task_id == 0)
            {
				if(p_timer->run == 1)
				{
					Cpu_IdelToTimerTask(p_task, p_device);     
				}
                //YX.20180508.解决读取未读消息时，可以点击调出运动模式的问题
                else if(p_movement->enable == 1)
                {   //运动模式是否正在运行中
                    p_movement->face_num = 1;
                    Cpu_IdelToRunTask(p_task, p_device); 
                }
				else if(Battery_Get_Grade() == 0)
				{
					Cpu_ScheduleEvent(p_event, p_device, CPU_EVENT_TASK_LOWPOWER);
				}
				else
				{
					Cpu_ScheduleTask(p_task, p_device, CPU_TASK_MAIN);
				}
            }
        }
        else   //YX.20180427.解决读取完推送消息，点击下一界面后UI显示问题，会闪屏一次
        {
            Gui_PageMessageNotificationIcon(p_message_notify->type,p_event->gui_controller.animation_frame_index++);
        }
    }
    
    if(p_event->gui_controller.animation_tick_counter % 4 == 0 && p_event->gui_controller.animation_tick_counter != 0)
    {
        Gui_PageMessageNotificationIcon(p_message_notify->type,p_event->gui_controller.animation_frame_index++);
    }
}

/****************************************************************
** Function name:            Cpu_HeartAlertHintEvent
** Descriptions:             心跳报警
** input parameters:         p_controller：         CPU任务控制器指针
                             p_gui：                静态页面控制器指针
** output parameters:        无
** Returned value:           无
** Created by:               郭涛
** Created Date:             2017-07-10
****************************************************************/
void Cpu_HeartAlertHintEvent(cpu_event_tcb_t* p_event, cpu_device_tcb_t* p_device)
{
    if(p_event->task_controller.init == 1)
    {
        Cpu_EventParamInit(p_event,8,false,true);        
        MotorAddEvent(2, 1, 3, 2); 
        //清除背景
        Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
                                p_device->cls_param.column_size, p_device->cls_param.page_size);
    }
    
    if(p_event->gui_controller.animation_tick_counter%4 == 0)
    {
//        GUI_HeartAlertPage();
		Gui_PageHeartAlert(p_event->gui_controller.animation_frame_index++);
        //设置清屏参数
        Gui_ThreadSetDisPlayArea(0, 46, 80, 67);
    }
    
    //如果用户按动按键,则提醒需要消掉
    if(p_device->key != BUTTON_EVENT_NULL)
    {
        Cpu_ClearCurrentEvent(p_event, p_device);	    
    }
}

void Cpu_LowOxygenAlertHintEvent(cpu_event_tcb_t* p_event, cpu_device_tcb_t* p_device)
{
    if(p_event->task_controller.init == 1)
    {
        Cpu_EventParamInit(p_event,8,false,true);        
        MotorAddEvent(2, 1, 3, 2);
        //清除背景
        Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
                                p_device->cls_param.column_size, p_device->cls_param.page_size);
    }
    
    if(p_event->gui_controller.animation_tick_counter%6 == 0)
    {
        Gui_LowOxygenAlertPage(p_event->gui_controller.animation_frame_index++);
        //设置清屏参数
        Gui_ThreadSetDisPlayArea(6, 37, 68, 84);
    }
    
    //如果用户按动按键,则提醒需要消掉
    if(p_device->key != BUTTON_EVENT_NULL)
    {
        Cpu_ClearCurrentEvent(p_event, p_device);		    
    }	
}

/****************************************************************
** Function name:            Cpu_FontTestEvent
** Descriptions:             字型测试事件
** input parameters:         p_controller：         CPU任务控制器指针
                             p_gui：                静态页面控制器指针
** output parameters:        无
** Returned value:           无
** Created by:               郭涛
** Created Date:             2017-07-10
****************************************************************/
void Cpu_FontTestEvent(cpu_task_tcb_t* p_task, cpu_event_tcb_t* p_event, cpu_device_tcb_t* p_device)
{
    if(p_event->task_controller.init == 1)
    {
        Cpu_EventParamInit(p_event,60,false,true);
        //清除背景
        Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
                                p_device->cls_param.column_size, p_device->cls_param.page_size);
    }
    
    if(p_event->gui_controller.animation_tick_counter%10 == 0)
    {
		Gui_PageFontTest();
        //设置清屏参数
        Gui_ThreadSetDisPlayArea(0, 0, 80, 160);
    }
    
    //如果用户按动按键,则提醒需要消掉
    if(p_device->key != BUTTON_EVENT_NULL)
    {
        p_device->key = BUTTON_EVENT_NULL;
    }
}

/****************************************************************
** Function name:            Cpu_HourglassAppHintEvent
** Descriptions:             APP开始倒计时
** input parameters:         p_controller：         CPU任务控制器指针
                             p_gui：                静态页面控制器指针
** output parameters:        无
** Returned value:           无
** Created by:               郭涛
** Created Date:             2017-07-10
****************************************************************/
void Cpu_HourglassAppHintEvent(cpu_event_tcb_t* p_event, cpu_device_tcb_t* p_device)
{
     if(p_event->task_controller.init == 1)
    {
        Cpu_EventParamInit(p_event,6,false,true);
        //清除背景
        Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
                                p_device->cls_param.column_size, p_device->cls_param.page_size);
    }
        
    if(p_event->gui_controller.animation_tick_counter%4 == 0)
    {
		Gui_PageCountDown(true,p_event->gui_controller.animation_frame_index++);
        //设置清屏参数
        Gui_ThreadSetDisPlayArea(0, 40, 80, 81);
    }

    // 倒计时功能按键触发
    if(p_device->key != BUTTON_EVENT_NULL)
    {
        Cpu_ClearCurrentEvent(p_event, p_device);
    }  
}

/****************************************************************
** Function name:            Cpu_HourglassTimeOutEvent
** Descriptions:             倒计时到时任务
** input parameters:         p_controller：         CPU任务控制器指针
                             p_gui：                静态页面控制器指针
** output parameters:        无
** Returned value:           无
** Created by:               郭涛
** Created Date:             2017-07-10
****************************************************************/
void Cpu_HourglassTimeOutEvent(cpu_event_tcb_t* p_event, cpu_device_tcb_t* p_device)
{
    if(p_event->task_controller.init == 1)
    {
        Cpu_EventParamInit(p_event,6,false,true);
        MotorAddEvent(3, 1, 1, 1);
        //清除背景
        Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
                                p_device->cls_param.column_size, p_device->cls_param.page_size);
    }
    
    if((p_event->gui_controller.animation_tick_counter%4) == 0)
    {
		Gui_PageCountDown(p_event->gui_controller.animation_enable,p_event->gui_controller.animation_frame_index++);
        //设置清屏参数
        Gui_ThreadSetDisPlayArea(0, 40, 80, 81);
    }
    
    if(p_device->key != BUTTON_EVENT_NULL)
    {	    
        Cpu_ClearCurrentEvent(p_event, p_device);    
    }
}

/****************************************************************
** Function name:            Cpu_BleDiscRemindEvent
** Descriptions:             蓝牙断连事件
** input parameters:         p_controller：         CPU任务控制器指针
                             p_gui：                静态页面控制器指针
** output parameters:        无
** Returned value:           无
** Created by:               郭涛
** Created Date:             2017-07-10
****************************************************************/
void Cpu_BleDiscRemindEvent(cpu_event_tcb_t* p_event, cpu_device_tcb_t* p_device)
{

}


/****************************************************************
** Function name:            Cpu_CameraReadyEvent
** Descriptions:             拍照准备事件
** input parameters:         p_controller：         CPU任务控制器指针
                             p_gui：                静态页面控制器指针
** output parameters:        无
** Returned value:           无
** Created by:               郭涛
** Created Date:             2017-07-10
****************************************************************/
void Cpu_CameraEvent(cpu_event_tcb_t* p_event, cpu_device_tcb_t* p_device)
{	
	static uint8_t  camera_state = 0;
    static uint8_t  camera_flg = 0;
    static uint8_t  camera_internal_time = 0;
    static bool     camera_response_enable = false;
    battery_status  current_status = BATTERY_POWER_NORMAL;
    
    if(p_event->task_controller.init == 1)
    {
        Cpu_EventParamInit(p_event,70,false,true);
        camera_response_enable = false;
		camera_state = 0;
        camera_flg = 0;
        camera_delay_cnt = 0;
        //清除背景
        Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
                                p_device->cls_param.column_size, p_device->cls_param.page_size);
    }
    
    if(p_event->gui_controller.animation_tick_counter%6 == 0)
    {
		Gui_PageCameraReady(p_event->gui_controller.animation_frame_index++);
        //设置清屏参数
        Gui_ThreadSetDisPlayArea(0, 0, 80, 160);
		if(camera_flg == 1 && camera_state > 0)
        {
            camera_state--;
            if(camera_state == 0)
            {
                camera_flg = 0;
            }
        }
        
        if(camera_response_enable == false && camera_internal_time++ >= 2)
        {
            camera_internal_time = 0;
            camera_response_enable = true;
        }
        
        current_status = Battery_Get_Status();
        if((current_status == BATTERY_POWER_CHARGE)||(current_status == BATTERY_POWER_FULL))
        {
            //YX.2017.12.23.解决手环退出拍照的时候，IOS手机也退出拍照的问题
            Ble_SendBatMulData(3,0xB6,0x01,0x00);
            memset(p_event, 0, sizeof(cpu_event_tcb_t));
            EventManage_ClearEventList();
        }
    }
    
    // 短按拍照
    if(((camera_response_enable)&&((Vph_GetShakeFlag()>2 && camera_delay_cnt >= 2) || p_data_manage->turnwrist_camera_state == 1))||(p_device->key == BUTTON_EVENT_CLICK))
    {
        // 向APP发送推出消息 
        Ble_SendBatMulData(3,0xB6,0x01,0x02);
        camera_internal_time = 0;
        camera_response_enable = false;
        p_data_manage->turnwrist_camera_state = 0;
		camera_flg = 1;
        camera_state = 1;
        //YX.20180510.更改操作过后重新计数1000秒
        p_event->task_controller.run_time = 0;
    }
    
    // 按键(长按)退出拍照模式
    if(p_device->key == BUTTON_EVENT_LONGPRESS)
    {	    
        // 长按退出   
        //YX.2017.12.23.解决手环退出拍照的时候，IOS手机也退出拍照的问题
        Ble_SendBatMulData(3,0xB6,0x01,0x00);
        Cpu_ClearCurrentEvent(p_event, p_device);	    
    }
    
    p_device->key = BUTTON_EVENT_NULL;
}

void Cpu_DevicePowerOnEvent(cpu_task_tcb_t* p_task, cpu_event_tcb_t* p_event, cpu_device_tcb_t* p_device)
{	        
    // 开机显示主界面
    p_data_manage->device_trace_param_s.sign_on = 1;
    
    GuiThread_ClearAllParam();
	
    BleThread_GsensorRestart();
    
    // 此处主要防止开机充电反复启动广播导致错误 Modify by GT 2017-10-30
    BleThread_AdvertisingStop();
    BleThread_AdvertisingStart();	
    
    Posture_StatisticalInit();
    Movement_Init();
    
    //Phys_UpdataParam();
    
    //Remind_BleDisconnectRemove();
    
    // 开机振动一次 持续一秒
    MotorAddEvent(1,0,1,0);	

    // 清屏
    Gui_PageClearBackground(0, 0, 80, 160);
    // 配置主界面参数
    Cpu_ScheduleTask(p_task, p_device, CPU_TASK_MAIN);
    Algorithm_OledBrightnessAdjustTask();
    GuiThread_BrightAdjust();
    
}

void Cpu_DevicePowerOffEvent(cpu_task_tcb_t* p_task, cpu_event_tcb_t* p_event, cpu_device_tcb_t* p_device)
{        
    //YX.20180514.标志位先清除，防止关机后还有其他事件在运行
    p_data_manage->device_trace_param_s.sign_on = 0;
    
    MotorDelEvent();
    
     // 停止广播
    BleThread_AdvertisingStop();

    GSensor_AccStop();
    Afe4900_Close();
	LCD_CloseScreen();
//    GUI_OledDriverOff();
    
    // 如果有睡眠,则退出睡眠
    Sleep_LowBateeryProcess(); 

    //女性项目提示语清除
    //Phys_ClearRemindInfo();
    
    //退出运动模式
    Movement_Exit();
    
    GuiThread_ClearAllParam();	
    
      //参数初始化
	if(Battery_Get_Grade() != 0)
	{
		MotorPowerOn();//振动一次 持续一秒
		nrf_delay_us(1000000);
		MotorPowerOFF();
	}
    
}

void Cpu_DeviceSportTargetEvent(cpu_task_tcb_t* p_task, cpu_event_tcb_t* p_event, cpu_device_tcb_t* p_device)
{
    if(p_event->task_controller.init == 1)
    {
        Cpu_EventParamInit(p_event,6,false,true); //10
        MotorAddEvent(3,1,1,1);
        //清除背景
        Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
                                p_device->cls_param.column_size, p_device->cls_param.page_size);
    }
    
    if(p_event->gui_controller.animation_tick_counter % 5 == 0 )//300ms 6
    {
		Gui_PageStarget(p_event->gui_controller.animation_frame_index++, p_flash_param->sport_target, display_language);
        //设置清屏参数
        Gui_ThreadSetDisPlayArea(0, 0, 80, 160);
    }
    
    //如果用户按动按键,则提醒需要消掉
    if(p_device->key != BUTTON_EVENT_NULL)
    {
        Cpu_ClearCurrentEvent(p_event, p_device);	
        //YX.20180514.运动达标提醒点击后直接熄屏，不再显示主界面
//        Cpu_ScheduleTask(p_task, p_device, CPU_TASK_MAIN);
    }	
}

