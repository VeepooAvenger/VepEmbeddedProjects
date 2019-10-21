/*******************Copyright(c)*********************************
** Veepoo Tech
** COPYRIGHT (C) 2016-2019
**----------------------FileInfo---------------------------------
** File name:                cpu_event_task.c
** Last modified Date:       2017-07-10
** Last SYS_VERSION:         0.0.1
** Descriptions:
**---------------------------------------------------------------
** Createdby:                ����
** Createddate:              2017-07-10
** SYS_VERSION:              0.0.1
** Descriptions:             cpu�¼�����
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

uint8_t camera_delay_cnt = 0;       //������ʱ����������1���𶯲Ż���Ӧ����
void Algorithm_OledBrightnessAdjustTask(void);

void Cpu_EventParamInit(cpu_event_tcb_t* p_event,uint16_t keep_time,bool press_hold,bool animation_active)
{
    // �����ʼ��
    p_event->task_controller.init = 0; 
    p_event->task_controller.active = 1; 
    p_event->task_controller.keep_time = keep_time; 
    p_event->task_controller.run_time = 0;
    p_event->task_controller.key_press_hold = press_hold; 

    p_event->gui_controller.animation_enable = animation_active;
    // ע��:���������ʼ��Ϊ0xFF,��Ҫ����̬ҳ��ˢ�¿���
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
    //YX.20180427.�޸���ʾ�ⲿ�¼���һ˲�䣬�а�������������ⲿ�¼���������������ص�������
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
     *    =====�������������߼�����====
     *    1.����������,������Ҫ�������;
     *    2.���û�������,����Ҫ��������;
     *
     */
    if(p_data_manage->device_trace_param_s.sign_on == 0)
    {
        //�ػ��²��ܽ��з�����
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
        //�ػ��²��ܽ��з�����
        return ;
    }
    // ���������߼�����
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
** Descriptions:             �͵����¼�
** input parameters:         p_controller��       CPU���������ָ��
                             p_gui��              ��̬ҳ�������ָ��
** output parameters:        ��
** Returned value:           ��
** Created by:               ����
** Created Date:             2017-07-10
****************************************************************/
void Cpu_LowPowerHintEvent(cpu_task_tcb_t* p_task, cpu_event_tcb_t* p_event, cpu_device_tcb_t* p_device)
{    
    if(p_event->task_controller.init == 1)
    {
        Cpu_EventParamInit(p_event,2,false,true);
		//�������
        Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
                                p_device->cls_param.column_size, p_device->cls_param.page_size);
    }
    
    if((p_event->gui_controller.animation_tick_counter%4) == 0)
    {
		//������������
        Gui_ThreadSetDisPlayArea(16, 33, 48, 91);
		Gui_PageLowPower(p_event->gui_controller.animation_frame_index++);
    }
    
    //�͵�����1S���л���������
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
** Descriptions:             ���г���¼�
** input parameters:         p_controller��         CPU���������ָ��
                             p_gui��                ��̬ҳ�������ָ��
** output parameters:        ��
** Returned value:           ��
** Created by:               ����
** Created Date:             2017-07-10
****************************************************************/
void Cpu_IdelChargeEvent(cpu_task_tcb_t* p_task, cpu_event_tcb_t* p_event, cpu_device_tcb_t* p_device)
{            
    if(p_event->task_controller.init == 1)
    {
        // ����ǳ���״̬,����ʾ8S
        if(Battery_Get_Status() == BATTERY_POWER_FULL)
        {
            Cpu_EventParamInit(p_event,8,false,true);  
        }
        else
        {
            Cpu_EventParamInit(p_event,8,false,true); 
        }
        //�������
        Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
                                p_device->cls_param.column_size, p_device->cls_param.page_size);
    }
	
	// ���ʱֹͣ�� Add by GT 2017-11-01
	MotorDelEvent();
    
    // ������״̬����ʱ,����ˢ��
    if(Battery_Get_Status() == BATTERY_POWER_FULL)
    {
        if(p_event->gui_controller.animation_tick_counter%4 == 0)
         {
             //������������
             Gui_ThreadSetDisPlayArea(16, 33, 49, 90); 
			 Gui_PageBatteryCharg(p_event->gui_controller.animation_frame_index++, 1);
         }
    }
    else 
    {
        if((p_event->gui_controller.animation_tick_counter%4) == 0)
        {
             //������������
             Gui_ThreadSetDisPlayArea(16, 33, 49, 90); 

			 Gui_PageBatteryCharg(p_event->gui_controller.animation_frame_index++, 0);
        }
    }

    // �����¼�����
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
** Descriptions:             ���嵽ʱ�¼�
** input parameters:         p_controller��         CPU���������ָ��
                             p_gui��                ��̬ҳ�������ָ��
** output parameters:        ��
** Returned value:           ��
** Created by:               ����
** Created Date:             2017-07-10
****************************************************************/
void Cpu_AlarmHintEvent(cpu_event_tcb_t* p_event, cpu_device_tcb_t* p_device)
{
    uint8_t am_pm_format = 0;
    uint8_t display_hour = 0;
	
    if(p_event->task_controller.init == 1)
    {
        Cpu_EventParamInit(p_event,20,false,true);
        MotorAddEvent(10, 1, 1, 1);//����10�� ÿ����1�� ���1��
        //�������
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
        //������������
        Gui_ThreadSetDisPlayArea(0, 0, 80, 160);
    }
    
    //����û���������,��������Ҫ����
    if(p_device->key != BUTTON_EVENT_NULL)
    {
        Cpu_ClearCurrentEvent(p_event, p_device);	    
    }
}

/****************************************************************
** Function name:            Cpu_SedentaryHintEvent
** Descriptions:             ���������¼�
** input parameters:         p_controller��         CPU���������ָ��
                             p_gui��                ��̬ҳ�������ָ��
** output parameters:        ��
** Returned value:           ��
** Created by:               ����
** Created Date:             2017-07-10
****************************************************************/
void Cpu_SedentaryHintEvent(cpu_event_tcb_t* p_event, cpu_device_tcb_t* p_device)
{
    if(p_event->task_controller.init == 1)
    {
        Cpu_EventParamInit(p_event,8,false,true);
        MotorAddEvent(2,1,3,2); 
        //�������
        Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
                                p_device->cls_param.column_size, p_device->cls_param.page_size);
    }
    
    if(p_event->gui_controller.animation_tick_counter%6 == 0)
    {    
        p_event->gui_controller.page_frame_counter = clock->seconds;
		Gui_PageSit(p_event->gui_controller.animation_frame_index++);
        //������������
        Gui_ThreadSetDisPlayArea(0, 40, 80, 80);
    }
    
    //����û���������,��������Ҫ����
    if(p_device->key != BUTTON_EVENT_NULL)
    {
        Cpu_ClearCurrentEvent(p_event, p_device);		    
    }
}

/****************************************************************
** Function name:            Cpu_CallHintEvent
** Descriptions:             �����¼�
** input parameters:         p_controller��         CPU���������ָ��
                             p_gui��                ��̬ҳ�������ָ��
** output parameters:        ��
** Returned value:           ��
** Created by:               ����
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
        //�������
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
        // �����¼�������ID
        p_event->child_event_id = p_call_notify->id;
        
        if(p_call_notify->reject_call == 1 && p_call_notify->reject_suc == 1)
        {
            // �ܽӵ绰  
            last_call_state = GUI_CALL_REJECTSUC;
        }
        else if(p_call_notify->mute_call == 1)
        {
            // �����绰    
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
        //������Ҫ���ദ��
        if(last_page_index == 0xFF && (last_call_state == GUI_CALL_INCOMING || last_call_state == GUI_CALL_MUTE))
        {
            Gui_PageCallNotificationContent(p_call_notify->content, p_call_notify->len);
        }
        //������������
        Gui_ThreadSetDisPlayArea(0, 0, 80, 160);
    }
        
    //����û���������,��������Ҫ����
    if(p_device->key != BUTTON_EVENT_NULL)
    {	    
        if(p_device->key == BUTTON_EVENT_LONGPRESS && p_call_notify->hang_up_call !=1)
        {
            Ble_SendBatMulData(3,0xC1,0x02,0x00);	
         
            // ���ANCS������,��Ϊ
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
** Descriptions:             ��Ϣ�����¼�
** input parameters:         p_controller��         CPU���������ָ��
                             p_gui��                ��̬ҳ�������ָ��
** output parameters:        ��
** Returned value:           ��
** Created by:               ����
** Created Date:             2017-07-10
****************************************************************/
void Cpu_MessageHintEvent(cpu_task_tcb_t* p_task, cpu_event_tcb_t* p_event, watch_timer_t *p_timer, movement_cont_t *p_movement,cpu_device_tcb_t* p_device)
{
    message_notify_content_t *p_message_notify = &p_data_manage->app_notify_s.message_content_s[p_data_manage->app_notify_s.update_index%2];
    
    if(p_event->task_controller.init == 1)
    {
        // ��ʾ����
        p_data_manage->app_notify_s.display_len = 0;//
        Cpu_EventParamInit(p_event,5,false,true);
        if(p_data_manage->app_notify_s.unread_msg_count != 0xFF)
        {
            //�����ȡδ����Ϣ����������
            //WJ.20190105.�޸���Ϣ���������1s(���ٹ���)
            MotorAddEvent(1,0,1,0);
        }
        else
        {
            //�����ȡδ����Ϣ���ֿ���һֱ��ȡ����
            p_data_manage->app_notify_s.unread_msg_count = 0;
        }
        p_event->child_event_id = (uint16_t)(p_message_notify->type);
        //�������
        Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
                                p_device->cls_param.column_size, p_device->cls_param.page_size);
    }

    //����û���������,��������Ҫ����
    if(p_device->key != BUTTON_EVENT_NULL)
    {	
        if(p_message_notify->len > 0)
        {
            uint8_t copy_content_len = p_message_notify->len - p_data_manage->app_notify_s.display_len;
            
            // �����ʾ�����ݲ��Ҹ�����ز���
            if(p_message_notify->len >= p_data_manage->app_notify_s.display_len)
            {
                memcpy(p_message_notify->content,&p_message_notify->content[p_data_manage->app_notify_s.display_len], copy_content_len);
                memset(&p_message_notify->content[copy_content_len], 0, sizeof(p_message_notify->content)-copy_content_len);
                p_message_notify->len = copy_content_len;
            }
            // ���³�ʼ����ʾ����
            Cpu_EventParamInit(p_event,4,false,true);
            //�������
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
        //������������
        Gui_ThreadSetDisPlayArea(0, 0, 80, 160);
//        p_data_manage->app_notify_s.display_len = Gui_UpdateMessage(p_message_notify->type, p_message_notify->content, p_message_notify->len); 
		
        //YX.20180427.�����ȡ��������Ϣ�������һ�����UI��ʾ���⣬������һ��
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
                //YX.20180508.�����ȡδ����Ϣʱ�����Ե�������˶�ģʽ������
                else if(p_movement->enable == 1)
                {   //�˶�ģʽ�Ƿ�����������
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
        else   //YX.20180427.�����ȡ��������Ϣ�������һ�����UI��ʾ���⣬������һ��
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
** Descriptions:             ��������
** input parameters:         p_controller��         CPU���������ָ��
                             p_gui��                ��̬ҳ�������ָ��
** output parameters:        ��
** Returned value:           ��
** Created by:               ����
** Created Date:             2017-07-10
****************************************************************/
void Cpu_HeartAlertHintEvent(cpu_event_tcb_t* p_event, cpu_device_tcb_t* p_device)
{
    if(p_event->task_controller.init == 1)
    {
        Cpu_EventParamInit(p_event,8,false,true);        
        MotorAddEvent(2, 1, 3, 2); 
        //�������
        Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
                                p_device->cls_param.column_size, p_device->cls_param.page_size);
    }
    
    if(p_event->gui_controller.animation_tick_counter%4 == 0)
    {
//        GUI_HeartAlertPage();
		Gui_PageHeartAlert(p_event->gui_controller.animation_frame_index++);
        //������������
        Gui_ThreadSetDisPlayArea(0, 46, 80, 67);
    }
    
    //����û���������,��������Ҫ����
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
        //�������
        Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
                                p_device->cls_param.column_size, p_device->cls_param.page_size);
    }
    
    if(p_event->gui_controller.animation_tick_counter%6 == 0)
    {
        Gui_LowOxygenAlertPage(p_event->gui_controller.animation_frame_index++);
        //������������
        Gui_ThreadSetDisPlayArea(6, 37, 68, 84);
    }
    
    //����û���������,��������Ҫ����
    if(p_device->key != BUTTON_EVENT_NULL)
    {
        Cpu_ClearCurrentEvent(p_event, p_device);		    
    }	
}

/****************************************************************
** Function name:            Cpu_FontTestEvent
** Descriptions:             ���Ͳ����¼�
** input parameters:         p_controller��         CPU���������ָ��
                             p_gui��                ��̬ҳ�������ָ��
** output parameters:        ��
** Returned value:           ��
** Created by:               ����
** Created Date:             2017-07-10
****************************************************************/
void Cpu_FontTestEvent(cpu_task_tcb_t* p_task, cpu_event_tcb_t* p_event, cpu_device_tcb_t* p_device)
{
    if(p_event->task_controller.init == 1)
    {
        Cpu_EventParamInit(p_event,60,false,true);
        //�������
        Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
                                p_device->cls_param.column_size, p_device->cls_param.page_size);
    }
    
    if(p_event->gui_controller.animation_tick_counter%10 == 0)
    {
		Gui_PageFontTest();
        //������������
        Gui_ThreadSetDisPlayArea(0, 0, 80, 160);
    }
    
    //����û���������,��������Ҫ����
    if(p_device->key != BUTTON_EVENT_NULL)
    {
        p_device->key = BUTTON_EVENT_NULL;
    }
}

/****************************************************************
** Function name:            Cpu_HourglassAppHintEvent
** Descriptions:             APP��ʼ����ʱ
** input parameters:         p_controller��         CPU���������ָ��
                             p_gui��                ��̬ҳ�������ָ��
** output parameters:        ��
** Returned value:           ��
** Created by:               ����
** Created Date:             2017-07-10
****************************************************************/
void Cpu_HourglassAppHintEvent(cpu_event_tcb_t* p_event, cpu_device_tcb_t* p_device)
{
     if(p_event->task_controller.init == 1)
    {
        Cpu_EventParamInit(p_event,6,false,true);
        //�������
        Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
                                p_device->cls_param.column_size, p_device->cls_param.page_size);
    }
        
    if(p_event->gui_controller.animation_tick_counter%4 == 0)
    {
		Gui_PageCountDown(true,p_event->gui_controller.animation_frame_index++);
        //������������
        Gui_ThreadSetDisPlayArea(0, 40, 80, 81);
    }

    // ����ʱ���ܰ�������
    if(p_device->key != BUTTON_EVENT_NULL)
    {
        Cpu_ClearCurrentEvent(p_event, p_device);
    }  
}

/****************************************************************
** Function name:            Cpu_HourglassTimeOutEvent
** Descriptions:             ����ʱ��ʱ����
** input parameters:         p_controller��         CPU���������ָ��
                             p_gui��                ��̬ҳ�������ָ��
** output parameters:        ��
** Returned value:           ��
** Created by:               ����
** Created Date:             2017-07-10
****************************************************************/
void Cpu_HourglassTimeOutEvent(cpu_event_tcb_t* p_event, cpu_device_tcb_t* p_device)
{
    if(p_event->task_controller.init == 1)
    {
        Cpu_EventParamInit(p_event,6,false,true);
        MotorAddEvent(3, 1, 1, 1);
        //�������
        Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
                                p_device->cls_param.column_size, p_device->cls_param.page_size);
    }
    
    if((p_event->gui_controller.animation_tick_counter%4) == 0)
    {
		Gui_PageCountDown(p_event->gui_controller.animation_enable,p_event->gui_controller.animation_frame_index++);
        //������������
        Gui_ThreadSetDisPlayArea(0, 40, 80, 81);
    }
    
    if(p_device->key != BUTTON_EVENT_NULL)
    {	    
        Cpu_ClearCurrentEvent(p_event, p_device);    
    }
}

/****************************************************************
** Function name:            Cpu_BleDiscRemindEvent
** Descriptions:             ���������¼�
** input parameters:         p_controller��         CPU���������ָ��
                             p_gui��                ��̬ҳ�������ָ��
** output parameters:        ��
** Returned value:           ��
** Created by:               ����
** Created Date:             2017-07-10
****************************************************************/
void Cpu_BleDiscRemindEvent(cpu_event_tcb_t* p_event, cpu_device_tcb_t* p_device)
{

}


/****************************************************************
** Function name:            Cpu_CameraReadyEvent
** Descriptions:             ����׼���¼�
** input parameters:         p_controller��         CPU���������ָ��
                             p_gui��                ��̬ҳ�������ָ��
** output parameters:        ��
** Returned value:           ��
** Created by:               ����
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
        //�������
        Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
                                p_device->cls_param.column_size, p_device->cls_param.page_size);
    }
    
    if(p_event->gui_controller.animation_tick_counter%6 == 0)
    {
		Gui_PageCameraReady(p_event->gui_controller.animation_frame_index++);
        //������������
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
            //YX.2017.12.23.����ֻ��˳����յ�ʱ��IOS�ֻ�Ҳ�˳����յ�����
            Ble_SendBatMulData(3,0xB6,0x01,0x00);
            memset(p_event, 0, sizeof(cpu_event_tcb_t));
            EventManage_ClearEventList();
        }
    }
    
    // �̰�����
    if(((camera_response_enable)&&((Vph_GetShakeFlag()>2 && camera_delay_cnt >= 2) || p_data_manage->turnwrist_camera_state == 1))||(p_device->key == BUTTON_EVENT_CLICK))
    {
        // ��APP�����Ƴ���Ϣ 
        Ble_SendBatMulData(3,0xB6,0x01,0x02);
        camera_internal_time = 0;
        camera_response_enable = false;
        p_data_manage->turnwrist_camera_state = 0;
		camera_flg = 1;
        camera_state = 1;
        //YX.20180510.���Ĳ����������¼���1000��
        p_event->task_controller.run_time = 0;
    }
    
    // ����(����)�˳�����ģʽ
    if(p_device->key == BUTTON_EVENT_LONGPRESS)
    {	    
        // �����˳�   
        //YX.2017.12.23.����ֻ��˳����յ�ʱ��IOS�ֻ�Ҳ�˳����յ�����
        Ble_SendBatMulData(3,0xB6,0x01,0x00);
        Cpu_ClearCurrentEvent(p_event, p_device);	    
    }
    
    p_device->key = BUTTON_EVENT_NULL;
}

void Cpu_DevicePowerOnEvent(cpu_task_tcb_t* p_task, cpu_event_tcb_t* p_event, cpu_device_tcb_t* p_device)
{	        
    // ������ʾ������
    p_data_manage->device_trace_param_s.sign_on = 1;
    
    GuiThread_ClearAllParam();
	
    BleThread_GsensorRestart();
    
    // �˴���Ҫ��ֹ������練�������㲥���´��� Modify by GT 2017-10-30
    BleThread_AdvertisingStop();
    BleThread_AdvertisingStart();	
    
    Posture_StatisticalInit();
    Movement_Init();
    
    //Phys_UpdataParam();
    
    //Remind_BleDisconnectRemove();
    
    // ������һ�� ����һ��
    MotorAddEvent(1,0,1,0);	

    // ����
    Gui_PageClearBackground(0, 0, 80, 160);
    // �������������
    Cpu_ScheduleTask(p_task, p_device, CPU_TASK_MAIN);
    Algorithm_OledBrightnessAdjustTask();
    GuiThread_BrightAdjust();
    
}

void Cpu_DevicePowerOffEvent(cpu_task_tcb_t* p_task, cpu_event_tcb_t* p_event, cpu_device_tcb_t* p_device)
{        
    //YX.20180514.��־λ���������ֹ�ػ����������¼�������
    p_data_manage->device_trace_param_s.sign_on = 0;
    
    MotorDelEvent();
    
     // ֹͣ�㲥
    BleThread_AdvertisingStop();

    GSensor_AccStop();
    Afe4900_Close();
	LCD_CloseScreen();
//    GUI_OledDriverOff();
    
    // �����˯��,���˳�˯��
    Sleep_LowBateeryProcess(); 

    //Ů����Ŀ��ʾ�����
    //Phys_ClearRemindInfo();
    
    //�˳��˶�ģʽ
    Movement_Exit();
    
    GuiThread_ClearAllParam();	
    
      //������ʼ��
	if(Battery_Get_Grade() != 0)
	{
		MotorPowerOn();//��һ�� ����һ��
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
        //�������
        Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
                                p_device->cls_param.column_size, p_device->cls_param.page_size);
    }
    
    if(p_event->gui_controller.animation_tick_counter % 5 == 0 )//300ms 6
    {
		Gui_PageStarget(p_event->gui_controller.animation_frame_index++, p_flash_param->sport_target, display_language);
        //������������
        Gui_ThreadSetDisPlayArea(0, 0, 80, 160);
    }
    
    //����û���������,��������Ҫ����
    if(p_device->key != BUTTON_EVENT_NULL)
    {
        Cpu_ClearCurrentEvent(p_event, p_device);	
        //YX.20180514.�˶�������ѵ����ֱ��Ϩ����������ʾ������
//        Cpu_ScheduleTask(p_task, p_device, CPU_TASK_MAIN);
    }	
}

