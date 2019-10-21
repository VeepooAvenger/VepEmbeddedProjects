/*******************Copyright(c)*********************************
** Veepoo Tech
** COPYRIGHT (C) 2016-2019
**----------------------FileInfo---------------------------------
** File name: 			    gui_thread.c
** Last modified Date: 		2017-07-10
** Last SYS_VERSION: 		0.0.1
** Descriptions:
**---------------------------------------------------------------
** Createdby: 			    ����
** Createddate: 			2017-07-10
** SYS_VERSION: 			0.0.1
** Descriptions: 			gui�߳�
**---------------------------------------------------------------
** Modified by:				
** Modified date:			
** SYS_VERSION:				
** Modified Descriptions:	
****************************************************************/

#include <string.h>
#include "gui_thread.h"
#include "cpu_task_data_type.h"
#include "SystemTime.h"
#include "CPU_GeneralTask.h"
#include "CPU_EventTask.h"
#include "algorithm_thread.h"
#include "battery_adc.h"
#include "bsp_btn_ble.h"
#include "app_type.h"
#include "motor_event.h"
#include "AFE4900_entry.h"
#include "ble_thread.h"
#include "log.h"
#include "event_manage.h"
#include "Common_data_type.h"
#include "gui_traverse.h"
#include "lcd_driver_tft.h"
#include "gui_page_display.h"
#include "ble_disconnect_remind.h"

cpu_param_t     cpu;				                         //��������ṹ��
gui_resource_t  st_gui_res;		                     //gui��Դ�ṹ��
static uint8_t  button_event_state = BUTTON_EVENT_NULL;   //����״̬
static uint8_t  button_press_time = 0;                    //��������ʱ��

uint8_t     ui_style_type = 0;							     //��������
uint8_t     display_language = CONFIG_LANGUAGE_ENAGLISH;     //UI��ʾ����,Ĭ��Ӣ��

uint32_t    motor_time_ms = 0;
bool        s_sos_enable = 0;           // �����

extern uint8_t  camera_delay_cnt;
//----��������----
static void GuiThread_Handle(void* arg);
void Key_OperationManage(BUTTON_EVENT_TYPE event, uint8_t key_press_times);

/****************************************************************
** Function name:           UiCreate_Resource
** Descriptions:            ��������
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by: 		        ����
** Created Date: 		2017-08-11
****************************************************************/
static void UiCreate_Resource(void)
{       
    
    SEMAPHORE_CREATE_BINARY_RET(st_gui_res.m_thread_sleep);
    SEMAPHORE_CREATE_BINARY_RET(st_gui_res.m_task_count);
    
    // �����߳�                                        384
    if(pdPASS != xTaskCreate(GuiThread_Handle, "Gui", 512/*384*/, NULL, GUI_THREAD_PRIORITY, &st_gui_res.m_thread_handle))  
    {
        APP_ERROR_HANDLER(NRF_ERROR_NO_MEM);
    }
}

/****************************************************************
** Function name:         GuiThread_Init
** Descriptions: 	        gui�̳߳�ʼ��
** input parameters:      ��
** output parameters:     ��
** Returned value:        ��
** Created by: 	        �� ��
** Created Date: 	        2017-07-10
****************************************************************/
void GuiThread_Init(void)
{
    UiCreate_Resource();
    
//    MotorAddEvent(1, 0, 1*2, 0);
}

/****************************************************************
** Function name: 	GuiThread_PageActive
** Descriptions: 		�ж�cpu��ǰ����
** input parameters:	��
** output parameters: 	��
** Returned value: 	0����������		1����ʾ����
** Created by: 		����
** Created Date: 		2017-07-10
****************************************************************/
bool GuiThread_PageActive(void)
{
    return (eTaskGetState(st_gui_res.m_thread_handle) != eSuspended);
}

/****************************************************************
** Function name: 	GuiThread_BrightAdjust
** Descriptions: 		oled����������
** input parameters:	��
** output parameters: 	��
** Returned value: 	��
** Created by: 		����
** Created Date: 		2017-07-10
****************************************************************/
void GuiThread_BrightAdjust(void)
{
    if(!GuiThread_PageActive())
	{
		if(cpu.watch.run == 1)
		{
			Cpu_IdelToTimerTask(&cpu.task_tcb, &cpu.device_tcb);   
		}
        else if(cpu.movement.enable == 1)
        {
            Cpu_IdelToRunTask(&cpu.task_tcb, &cpu.device_tcb);
        }
		else
		{
			Cpu_ScheduleTask(&cpu.task_tcb, &cpu.device_tcb, CPU_TASK_MAIN);
		}
    }
        
    cpu.device_tcb.oled.bright = 1;
    if(eTaskGetState(st_gui_res.m_thread_handle) != eRunning) 
    {
	    vTaskResume(st_gui_res.m_thread_handle);//�ָ̻߳�
    }        
}

/****************************************************************
** Function name: 	    Task_DispatchExcuteManage
** Descriptions: 		�������ִ�й���
** input parameters:	��
** output parameters: 	��
** Returned value: 	    ��
** Created by: 		    ����
** Created Date: 		2017-07-10
****************************************************************/
static void Task_DispatchExcuteManage(void)
{
    //������ⲿ�¼�
    cpu.task_tcb.gui_controller.repaint_enable = 1;
    if((cpu.event_tcb.event_id != CPU_EVENT_TASK_IDEL)||(p_data_manage->event_list.count > 0))
    {
        cpu.task_tcb.gui_controller.repaint_enable = 0;
		cpu.task_tcb.child_task_id = 0;
		cpu.task_tcb.gui_controller.animation_frame_index = 0;
        cpu.task_tcb.gui_controller.animation_tick_counter = 0;
        return ;
    }
    
    switch(cpu.task_tcb.task_id)
    {
        case CPU_TASK_MAIN:
            Cpu_MainTask(&cpu.task_tcb,&cpu.device_tcb);
            break;
		case CPU_TASK_MAC:
			Cpu_MacAdd(&cpu.task_tcb,&cpu.device_tcb);
			break;
		case CPU_TASK_MOVEMENT:
			Cpu_TaskRun(&cpu.task_tcb,&cpu.movement,&cpu.device_tcb);
            break;
        case CPU_TASK_STEP:
            Cpu_StepTask(&cpu.task_tcb,&cpu.device_tcb);
            break;
		case CPU_TASK_DISTAN:
            Cpu_DistanTask(&cpu.task_tcb,&cpu.device_tcb);
            break;
		case CPU_TASK_CALOR:
            Cpu_CalorTask(&cpu.task_tcb,&cpu.device_tcb);
            break;
        case CPU_TASK_OXYGEN:
            Cpu_OxygenTask(&cpu.task_tcb,&cpu.device_tcb);
            break;
        case CPU_TASK_HEART:
            Cpu_HeartTask(&cpu.task_tcb,&cpu.device_tcb);
            break;
        case CPU_TASK_SLEEP:
            Cpu_SleepTask(&cpu.task_tcb,&cpu.device_tcb);
            break;
        case CPU_TASK_FINDPHONE:
            Cpu_FindPhoneTask(&cpu.task_tcb,&cpu.device_tcb);
            break;
        case CPU_TASK_TIMER:
            Cpu_TimerTask(&cpu.task_tcb,&cpu.watch,&cpu.device_tcb);
            break;
        case CPU_TASK_HOURGLASS:
            Cpu_HourglassTask(&cpu.task_tcb,&cpu.device_tcb);
            break;
        case CPU_TASK_POWEROFF:
            Cpu_PowerOffTask(&cpu.task_tcb,&cpu.event_tcb,&cpu.device_tcb);
            break;
        case CPU_TASK_BP:
            Cpu_BpTask(&cpu.task_tcb, &cpu.device_tcb);
            break;
        default:
            cpu.task_tcb.task_id = CPU_TASK_IDEL;
            break;
    }    
}

/****************************************************************
** Function name: 	Cpu_ExitAllTask
** Descriptions: 		cpu�˳���������
** input parameters:	��
** output parameters: 	��
** Returned value: 	��
** Created by: 		����
** Created Date: 		2017-07-10
****************************************************************/
static void Cpu_ExitAllTask(void)
{
    memset(&cpu.task_tcb, 0, sizeof(cpu_task_tcb_t));
	memset(&cpu.event_tcb, 0, sizeof(cpu_task_tcb_t));
	
    Cpu_ClearCurrentEvent(&cpu.event_tcb, &cpu.device_tcb);
    EventManage_ClearEventList();
}

/****************************************************************
** Function name: 	    Event_DispatchExcuteManage
** Descriptions: 		�¼����ȹ���
** input parameters:	��
** output parameters: 	��
** Returned value: 	    ��
** Created by: 		    ����
** Created Date: 		2017-07-10
****************************************************************/
static void Event_DispatchExcuteManage(void)
{
    switch(cpu.event_tcb.event_id)
    {
        case CPU_EVENT_TASK_ALL_EXIT:
            Cpu_ExitAllTask();
            break;
        case CPU_EVENT_TASK_CHARGE:
            Cpu_IdelChargeEvent(&cpu.task_tcb,&cpu.event_tcb,&cpu.device_tcb);
            break;
        case CPU_EVENT_TASK_ALARM:
            Cpu_AlarmHintEvent(&cpu.event_tcb,&cpu.device_tcb);
            break;
        case CPU_EVENT_TASK_SIT:
            Cpu_SedentaryHintEvent(&cpu.event_tcb,&cpu.device_tcb);
            break;
        case CPU_EVENT_TASK_CALL:
            Cpu_CallHintEvent(&cpu.event_tcb,&cpu.device_tcb);
            break;
        case CPU_EVENT_TASK_MESSAGE:
            Cpu_MessageHintEvent(&cpu.task_tcb,&cpu.event_tcb,&cpu.watch,&cpu.movement,&cpu.device_tcb);
            break;
        case CPU_EVENT_TASK_HEARTALERT:
            Cpu_HeartAlertHintEvent(&cpu.event_tcb,&cpu.device_tcb);
            break;
        case CPU_EVENT_TASK_LOWOXYGENALERT:
            Cpu_LowOxygenAlertHintEvent(&cpu.event_tcb,&cpu.device_tcb);
            break;
        case CPU_EVENT_TASK_CAMERA:
            Cpu_CameraEvent(&cpu.event_tcb,&cpu.device_tcb);
            break;
        case CPU_EVENT_TASK_FONTTEST:
            Cpu_FontTestEvent(&cpu.task_tcb,&cpu.event_tcb,&cpu.device_tcb);
            break;
        case CPU_EVENT_TASK_LOWPOWER:
            Cpu_LowPowerHintEvent(&cpu.task_tcb,&cpu.event_tcb,&cpu.device_tcb);
            break;
        case CPU_EVENT_TASK_HOURGLASS_HINT:
            Cpu_HourglassAppHintEvent(&cpu.event_tcb,&cpu.device_tcb);
            break;
        case CPU_EVENT_TASK_HOURGLASS_TIMEOUT:
            Cpu_HourglassTimeOutEvent(&cpu.event_tcb,&cpu.device_tcb);
            break;
        case CPU_EVENT_TASK_POWERON:
            Cpu_DevicePowerOnEvent(&cpu.task_tcb,&cpu.event_tcb,&cpu.device_tcb);
            break;
        case CPU_EVENT_TASK_POWEROFF:
            Cpu_DevicePowerOffEvent(&cpu.task_tcb,&cpu.event_tcb,&cpu.device_tcb);
            break;
		case CPU_EVENT_TASK_SPORT_TARGET:
			Cpu_DeviceSportTargetEvent(&cpu.task_tcb,&cpu.event_tcb,&cpu.device_tcb);
			break;
        default:
            cpu.event_tcb.event_id = CPU_EVENT_TASK_IDEL;
            break;
    }
}

/****************************************************************
** Function name: 	    Oled_DataDriveManage
** Descriptions: 		oled��������������
** input parameters:	��
** output parameters: 	��
** Returned value: 	    ��
** Created by: 		    ����
** Created Date: 		2017-07-10
****************************************************************/
static void Oled_DataDriveManage(void)
{
    if(cpu.device_tcb.oled.light == 1)
    {
        //���ｫ������
        cpu.device_tcb.oled.light = 0;
        if(cpu.device_tcb.oled.status == 0)
        {
            cpu.device_tcb.oled.status = 1;
			LCD_OpenScreen();
//            GUI_OledDriverLight(p_data_manage->oled_bright);
        }
    }
    
    if(cpu.device_tcb.oled.repaint == 1)
    {
        cpu.device_tcb.oled.repaint = 0;
        
        // ���������ȵ��ڿ���
        if((cpu.device_tcb.oled.bright == 1)&&(cpu.device_tcb.oled.status == 1))
        {
            cpu.device_tcb.oled.bright = 0;
//            Oled_SetBrightnessCmd(p_data_manage->oled_bright);
        }
//        // ������������
//        GUI_OledDriverUpdate();
    }
    
    if((cpu.task_tcb.task_id == CPU_TASK_IDEL)&&(cpu.event_tcb.event_id == CPU_EVENT_TASK_IDEL))
    {
        // ��������������Ҫ��,��ʱ�����̲߳�������
        if(p_data_manage->event_list.count == 0)
        {
            cpu.device_tcb.oled.status = 0;
//            GUI_OledDriverOff();	
			//YX.2018.02.09.�޸���Ļ���ֱ������ƣ���Ļ����ʾ������
			LCD_CloseLed();
			LCD_DriverInit();
			LCD_CloseScreen();
        }
    }
}

/****************************************************************
** Function name: 	    Task_DispatchTimeManage
** Descriptions: 		�����������ʱ�����
** input parameters:	��
** output parameters: 	��
** Returned value: 	    ��
** Created by: 		    ����
** Created Date: 		2017-07-10
****************************************************************/
static void Task_DispatchTimeManage(void) 
{  
    static uint8_t task_second_counter = 0xFF;
    
    if(cpu.task_tcb.gui_controller.animation_enable == 1)
    {
        cpu.task_tcb.gui_controller.animation_tick_counter++;
    }
    
    if((task_second_counter != clock->seconds)) 
    { 
        task_second_counter = clock->seconds;
        
        if(cpu.task_tcb.task_controller.active == 1)
        {
            // �������֧�ֳ�������,����Ҫ���ֽ�������
            if((cpu.task_tcb.task_controller.key_press_hold == 1)&&(Buttons_GetPressState() == 1))
            {
                if(cpu.task_tcb.task_controller.run_time > 0)
                {
                    cpu.task_tcb.task_controller.run_time--;
                }
            }  
            
            // ����ʱ�䵽��,��������������
            if(++cpu.task_tcb.task_controller.run_time > cpu.task_tcb.task_controller.keep_time) 
            {      
				if(Gui_GetTraverseUiState() == 0)
				{
					task_second_counter = 0xFF;        
					memset(&cpu.task_tcb, 0, sizeof(cpu_task_tcb_t));
				}
				else
				{
					Gui_EventAndTaskTraverse(1);
				}
            } 
            else
            {
                // �쳣�߼�����
                if(cpu.task_tcb.task_controller.keep_time > 300)
                {
                    task_second_counter = 0xFF;        
                    memset(&cpu.task_tcb, 0, sizeof(cpu_task_tcb_t));
                }
            }
        }
        else
        {
            task_second_counter = 0xFF;        
            memset(&cpu.task_tcb, 0, sizeof(cpu_task_tcb_t));
        }
    }   
}

/****************************************************************
** Function name: 	    Event_DispatchTimeManage
** Descriptions: 		�¼�����ʱ�����
** input parameters:	��
** output parameters: 	��
** Returned value: 	    ��
** Created by: 		    ����
** Created Date: 		2017-07-10
****************************************************************/
void Event_DispatchTimeManage(void) 
{  
    static uint8_t  event_second_counter = 0xFF;
    
    if(cpu.event_tcb.gui_controller.animation_enable == 1)
    {
        // ������Ӧ��tickʵ����һ����
        cpu.event_tcb.gui_controller.animation_tick_counter++;
    }
    
    if(event_second_counter != clock->seconds)
    {
        event_second_counter = clock->seconds;
        
        if(cpu.event_tcb.task_controller.active == 1)
        {
            // 1.�����߼��жϴ���
            if(++cpu.event_tcb.task_controller.run_time > cpu.event_tcb.task_controller.keep_time) 
            { 
				if(Gui_GetTraverseUiState() == 0)
				{
					event_second_counter = 0xFF;
					EventManage_ClearEvent(cpu.event_tcb.event_id);
					memset(&cpu.event_tcb, 0, sizeof(cpu_event_tcb_t));
				}
				else
				{
					Gui_EventAndTaskTraverse(2);
				}
            } 
            else
            {
                if((cpu.event_tcb.event_id != CPU_EVENT_TASK_CAMERA)&&(cpu.event_tcb.task_controller.keep_time > 300))
                {
                    event_second_counter = 0xFF;
                    EventManage_ClearEvent(cpu.event_tcb.event_id);
                    memset(&cpu.event_tcb, 0, sizeof(cpu_event_tcb_t));
                    
                }
                else if((cpu.event_tcb.event_id == CPU_EVENT_TASK_CAMERA)&&(cpu.event_tcb.task_controller.run_time > 60))
                {
                    //ZJH.20181010.�������һ�����޲����Զ��˳��Ĳ���
                    Ble_SendBatMulData(3,0xB6,0x01,0x00);
                    memset(&cpu.event_tcb, 0, sizeof(cpu_event_tcb_t));
                    EventManage_ClearEventList();
                }
            }
        }
        else
        {
            // !ע��:�������������active�������������0,��ǿ�Ƴ�ʼ��,�����¼���Զ�޷��˳�
            event_second_counter = 0xFF;
            EventManage_ClearEvent(cpu.event_tcb.event_id);
            memset(&cpu.event_tcb, 0, sizeof(cpu_event_tcb_t));
        }
    } 
}

static void Event_PreemptiveScheduleManage(void)
{
    cpu_event_tcb_t important_event_tcb = {CPU_EVENT_TASK_IDEL, 0};
    
    // ���ģʽ������ֻ�ģʽ(����¼��б�)
    if(GuiThread_IsDuringFindPhone() || (cpu.event_tcb.event_id == CPU_EVENT_TASK_CHARGE))//((cpu.task_tcb.task_id == CPU_TASK_FINDPHONE)||(cpu.event_tcb.event_id == CPU_EVENT_TASK_CHARGE))  
    {
        EventManage_ClearEventList();
    }
    else
    {
        if((p_data_manage->event_list.count > 0)&&(EventManage_IsHigerPriorityEvent(cpu.event_tcb.event_id)))
        {
            if(EventManage_PopMaxPriorityEvent(&important_event_tcb))
            {
                if(important_event_tcb.event_id != cpu.event_tcb.event_id)
                {
                    if(cpu.event_tcb.event_id != CPU_EVENT_TASK_IDEL)
                    {
                        // ���ȼ��������и������ȼ��¼�,��ʱ��Ҫ��ռִ��
                        EventManage_PushEventTcb(&cpu.event_tcb);
                        cpu.event_tcb.task_controller.init = 1;
                    }
                    //ZJH.20180611.ɾ��������¼�������ⲿ�¼��滻��ʱ������쳣�����
                    MotorDelEvent();
					// YX.20180206.�������������ⲿ�¼��滻��ʱ����ֻ�������
					Gui_PageClearBackground(cpu.device_tcb.cls_param.start_column, cpu.device_tcb.cls_param.start_page, 
											cpu.device_tcb.cls_param.column_size, cpu.device_tcb.cls_param.page_size);
                    // �������ڵ��¼�ִ�в���
                    memcpy(&cpu.event_tcb, &important_event_tcb, sizeof(cpu_event_tcb_t));
                    //YX.20180427.�޸��ⲿʱ�䱻��Ϻ�������ʾ����ʱ��UI��ʾ��ȫ����û��ʾ������
                    cpu.event_tcb.gui_controller.animation_tick_counter = 0;
                    cpu.event_tcb.gui_controller.animation_frame_index = 0;
                    cpu.event_tcb.gui_controller.page_frame_counter = 0xFFFF;
                    camera_delay_cnt = 0;
                }
                else
                {
                    // ���������ͬ���ȼ��¼�ʱ,��Ҫ
                    cpu.event_tcb.task_controller.init = 1;
                }
                
                cpu.device_tcb.oled.light = 1;
            }
        }
    }    
}

static void Motor_DriveManage(void)
{   
    //if(cpu.device_tcb.motor.second_conter != clock->seconds)
    if(((uint32_t)xTaskGetTickCount() - motor_time_ms) >= 1000)
    {
        motor_time_ms = (uint32_t)xTaskGetTickCount();
        //cpu.device_tcb.motor.second_conter = clock->seconds;
        MotorEventRun(); 
    }        
}

static void Thread_SuspendAndDelayManage(void)
{
    if((cpu.task_tcb.task_id == CPU_TASK_IDEL)&&(cpu.event_tcb.event_id == CPU_EVENT_TASK_IDEL))
    {
        // ��������������Ҫ��,��ʱ�����̲߳�������
        if(p_data_manage->event_list.count == 0)
        {
            if(xSemaphoreTake(st_gui_res.m_task_count,(TickType_t)0) == pdFALSE)
            {
                // ���
                cpu.device_tcb.motor.second_conter = 0xFF;
                // !ע��:�˴������̹߳�����Ҫ��������ٵ�PID�������� Modidfy by GT 2017-10-31
                cpu.pid = 0;
                // �˴���������ʱ���ʡ�Ѫ��������־ Add by GT 2017-11-01
                p_data_manage->heart_measure_s.btn = 0;
                p_data_manage->blood_oxygen_measure_s.btn = 0;
                if(p_data_manage->device_trace_param_s.factory_mode == 0)
                {
                    MotorDelEvent();
                }
                p_data_manage->device_trace_param_s.during_findphone = 0;
                
                Bps_Spi2UnInit();
                vTaskSuspend(st_gui_res.m_thread_handle);
                Bps_Spi2Init();
                
                motor_time_ms = 0;
            }
        }
    }
 
    // ���������л�����,��ʱ��������ʱ����
    if(cpu.pid == GET_TASK_PID(cpu.task_tcb,cpu.event_tcb))
    {
        vTaskDelay(50);
    }
    else
    {
        cpu.pid = GET_TASK_PID(cpu.task_tcb,cpu.event_tcb);
    }
}

/****************************************************************
** Function name: 	GuiThread_Handle
** Descriptions: 		gui�̴߳�����
** input parameters:	���ò���
** output parameters: 	��
** Returned value: 	��
** Created by: 		����
** Created Date: 		2017-07-10
****************************************************************/
static void GuiThread_Handle(void* arg)
{    
    UNUSED_PARAMETER(arg);

    st_gui_res.init_complete = 1;
    
    memset(&cpu, 0, sizeof(cpu_param_t));
    Cpu_ScheduleTask(&cpu.task_tcb, &cpu.device_tcb, CPU_TASK_MAIN);
    
    //GuiThread_AddEvent(CPU_EVENT_TASK_ALARM);
    ///Gui_SetTraverseUiState(1);
    
    while(1)
    {
        //��������
        Key_OperationManage((BUTTON_EVENT_TYPE)button_event_state, button_press_time);
		if(Gui_GetTraverseUiState() == 1)
		{
			Gui_ThreadTestManage();
		}
		else
		{
			//��ȡ���Ժ���������
            if(p_flash_param->language_e >= CONFIG_LANGUAGE_CHINESE)
            {
                display_language = p_flash_param->language_e - 1;
                if(display_language)
                {
                    display_language = 1;
                }
            }
			ui_style_type = Api_GetUiStyle(); 
		}
        
        // �¼���ռʽ���ȹ���
        Event_PreemptiveScheduleManage();
        
		if(Gui_GetTraverseUiState() == 0)
		{
			// �����¼�����
			Event_DispatchExcuteManage();
		}
        
        // �¼�ʱ�����
        Event_DispatchTimeManage();
        
		if(Gui_GetTraverseUiState() == 0)
		{
			// �����������
			Task_DispatchExcuteManage();
		}
        
        // ����ʱ�����
        Task_DispatchTimeManage();

        // �����������
        Motor_DriveManage();
        
        // Oled���������ݸ��¹���
        Oled_DataDriveManage();	
		 
        // �̹߳������ʱ����
        Thread_SuspendAndDelayManage();
    } 
}

/****************************************************************
** Function name: 	GuiThread_AddEvent
** Descriptions: 		����¼�
** input parameters:	event���¼�ģʽö���Ͷ���
** output parameters: 	��
** Returned value: 	��
** Created by: 		����
** Created Date: 		2017-07-10
****************************************************************/
uint8_t GuiThread_AddEvent(CPU_EVENT_TYPE event)
{
    
    // �����ֻ����������¼� //WJ.20190411.�޸�=Ϊ==    
    if(p_data_manage->device_trace_param_s.during_findphone == 1 && event != CPU_EVENT_TASK_POWEROFF)//CPU_EVENT_TASK_LOWPOWER
    {
        if(event != CPU_EVENT_TASK_CHARGE)
        {
            return 0;
        }
    }
    if(p_data_manage->device_trace_param_s.sign_on == 0 
       && (event != CPU_EVENT_TASK_POWERON && event != CPU_EVENT_TASK_LOWPOWER))
    {
        return 0;
    }
    
    if((event != CPU_EVENT_TASK_CHARGE)
       && (event != CPU_EVENT_TASK_POWEROFF) 
       && (event != CPU_EVENT_TASK_POWERON) 
       && Battery_Get_Status() != BATTERY_POWER_NORMAL)// 
    {
        return 0;
    }   
    xSemaphoreGive(st_gui_res.m_task_count); 
    
    if(eTaskGetState(st_gui_res.m_thread_handle) != eRunning)
    {
        //�����߳�
        vTaskResume(st_gui_res.m_thread_handle);  
    }
    
//    if(event == CPU_EVENT_TASK_MESSAGE)
//    {
//        cpu.event_tcb.child_event_id = p_data_manage->app_notify_s.message_content_s[p_data_manage->app_notify_s.update_index%2].type;
//    }
    
    do
    {
        if(event == CPU_EVENT_TASK_TURNWRIST_START)
        {
            Cpu_TurnwristStartEvent(&cpu.task_tcb,&cpu.event_tcb,&cpu.movement,&cpu.watch,&cpu.device_tcb);
            break;
        }
        else if(event == CPU_EVENT_TASK_TURNWRIST_STOP)
        {
            Cpu_TurnwristStopEvent(&cpu.task_tcb,&cpu.event_tcb,&cpu.watch,&cpu.device_tcb);
            break;
        }
        else if((event == CPU_EVENT_TASK_ALL_EXIT)||(event == CPU_EVENT_TASK_FONTTEST)||
	         (event == CPU_EVENT_TASK_POWERON)||(event == CPU_EVENT_TASK_POWEROFF)||
	         (event == CPU_EVENT_TASK_LOWPOWER)||(event == CPU_EVENT_TASK_CHARGE))
        {
            cpu.event_tcb.event_id = event;
            cpu.event_tcb.task_controller.init = 1;
            cpu.device_tcb.oled.light = 1;
            if(event == CPU_EVENT_TASK_CHARGE)
            {
                EventManage_ClearEventList();                
            }
            break;		 
        }	    
	 
        EventManage_CollectNewEvent(event);
    }while(0);
    
    return 1;
}

void GuiThread_ClearEvent(CPU_EVENT_TYPE event)
{
    if(cpu.event_tcb.event_id == event)
    {
        Cpu_ClearCurrentEvent(&cpu.event_tcb, &cpu.device_tcb);
    }
    
    EventManage_ClearEvent(event);
}

bool GuiThread_IsDuringFindPhone(void)
{
    return (cpu.task_tcb.task_id == CPU_TASK_FINDPHONE)&&(p_data_manage->device_trace_param_s.during_findphone == 1);
}

CPU_TASK_TYPE GuiThread_GetTaskType(void)
{
    return cpu.task_tcb.task_id;
}

CPU_EVENT_TYPE GuiThread_GetEventType(void)
{
    return cpu.event_tcb.event_id;	
}

const watch_timer_t* GuiThread_GetWatchTimer(void)
{
    return &cpu.watch;
}

uint8_t GuiThread_TimerIsBusy(void)
{
    return cpu.watch.run;
}

/****************************************************************
** Function name:       GuiThread_AddKeyEvent
** Descriptions:        ����������¼�
** input parameters:    ��
** output parameters: 	��
** Returned value:      ��
** Created by:          �� ��
** Created Date:        2017-07-10
****************************************************************/
void GuiThread_AddKeyEvent(BUTTON_EVENT_TYPE event, uint8_t times)
{
    if(st_gui_res.m_thread_handle == NULL)
    {
        return;
    }
      
    if(event == BUTTON_EVENT_CLICK || event == BUTTON_EVENT_LONGPRESS)
    {
        if(p_data_manage->device_trace_param_s.factory_mode == 0)
        {
            MotorDelEvent();//�ر�����¼�
        }    
    }
    
    button_event_state = event;
    button_press_time = times;
    xSemaphoreGive(st_gui_res.m_task_count);

    if(eTaskGetState(st_gui_res.m_thread_handle) != eRunning) 
    {
        xTaskResumeFromISR(st_gui_res.m_thread_handle);  
    }
}

/****************************************************************
** Function name:       Key_OperationManage
** Descriptions:        ������
** input parameters:    ��
** output parameters: 	��
** Returned value:      ��
** Created by:          YX
** Created Date:        2018-01-09
****************************************************************/
void Key_OperationManage(BUTTON_EVENT_TYPE event, uint8_t key_press_times)
{
    if(event == BUTTON_EVENT_NULL)
    {
        return ;
    }       
    button_event_state = BUTTON_EVENT_NULL;
	Gui_SetTraverseUiState(0);

    
    // ��������������
    if(p_data_manage->device_trace_param_s.sign_on == 1)
    {
        if(Battery_Get_Status() == BATTERY_POWER_NORMAL)
        {
            if((cpu.task_tcb.task_id == CPU_TASK_MAIN || cpu.task_tcb.task_id == CPU_TASK_MAC)&&(cpu.event_tcb.event_id == CPU_EVENT_TASK_IDEL))
            {
                //YX.20180509.Ů����Ŀδ����Ϣ��ȡ
                //if(p_data_manage->phys_notify_s.unread_phys_count > 0 && p_data_manage->phys_notify_s.unread_phys_count != 0xFF)
               // {
               //     p_data_manage->phys_notify_s.unread_phys_count = 0xFF;//--;
               //     GuiThread_AddEvent(CPU_EVENT_TASK_PHYS_REMIND);
               //     return;
               // }
                
                if(p_data_manage->app_notify_s.unread_msg_count > 0 && p_data_manage->app_notify_s.unread_msg_count != 0xFF)
                {
                    p_data_manage->app_notify_s.unread_msg_count = 0xFF;//--;
                    GuiThread_AddEvent(CPU_EVENT_TASK_MESSAGE);
                    return;
                }
            }
        }
        
        if((cpu.task_tcb.task_id == CPU_TASK_IDEL)&&(cpu.event_tcb.event_id == CPU_EVENT_TASK_IDEL))
        {
            if(Battery_Get_Status() == BATTERY_POWER_NORMAL)
            {
                //YX.20180509.Ů����Ŀδ����Ϣ��ȡ
                //if(p_data_manage->phys_notify_s.unread_phys_count > 0 && p_data_manage->phys_notify_s.unread_phys_count != 0xFF)
                //{
                //    p_data_manage->phys_notify_s.unread_phys_count = 0xFF;//--;
                //    GuiThread_AddEvent(CPU_EVENT_TASK_PHYS_REMIND);
                //    return;
                //}
                
                if(p_data_manage->app_notify_s.unread_msg_count > 0 && p_data_manage->app_notify_s.unread_msg_count != 0xFF)
                {
                    p_data_manage->app_notify_s.unread_msg_count = 0xFF;//--;
                    GuiThread_AddEvent(CPU_EVENT_TASK_MESSAGE);
                    return;
                }
            }
                        
            //��ת���˶�ģʽ����
            if(cpu.movement.enable == 1)
            {
                cpu.movement.face_num = 1;
                Cpu_IdelToRunTask(&cpu.task_tcb, &cpu.device_tcb);
                xTaskResumeFromISR(st_gui_res.m_thread_handle);
                return;
            }
            
            //��ת��������
            if(cpu.watch.run == 1)
            {
                Cpu_IdelToTimerTask(&cpu.task_tcb, &cpu.device_tcb);                
                xTaskResumeFromISR(st_gui_res.m_thread_handle);
                return;
            }
            
            
            // ����Ǵ���(����)ʱ,���ж��Ƿ�͵�����
            if((event == BUTTON_EVENT_CLICK)&&(Battery_Get_Grade() == 0)
                //YX.20180427.�޸��͵�ʱ��磬Ϩ���£���������ʾ���͵�ͼ�������
                &&(Battery_Get_Status() == BATTERY_POWER_NORMAL))
            {
                Cpu_ScheduleEvent(&cpu.event_tcb, &cpu.device_tcb, CPU_EVENT_TASK_LOWPOWER);
                xTaskResumeFromISR(st_gui_res.m_thread_handle);
                return;
            }

            Cpu_ScheduleTask(&cpu.task_tcb, &cpu.device_tcb, CPU_TASK_MAIN);
            cpu.device_tcb.key = BUTTON_EVENT_NULL;
            cpu.device_tcb.key_times = 0;
        }
        else
        {
            cpu.device_tcb.key = event;
            cpu.device_tcb.key_times = key_press_times;
        }
    }
    else
    {
        if(event == BUTTON_EVENT_LONGPRESS)
        {
            if(p_data_manage->device_trace_param_s.low_power == 0)
            {
                GuiThread_AddEvent(CPU_EVENT_TASK_POWERON);		 
            }
            else
            {
                if(Battery_Get_Status() != BATTERY_POWER_NORMAL)
                {
                    GuiThread_AddEvent(CPU_EVENT_TASK_POWERON);
                }
                else
                {
                    GuiThread_AddEvent(CPU_EVENT_TASK_LOWPOWER);
                }
            }
        }
    }
//		button_event_state = BUTTON_EVENT_NULL;
//		GuiThread_AddKeyEvent(BUTTON_EVENT_NULL);
}

void GuiThread_ChargingCommit(void)
{
	//�޸� �ֻ���������ȥ��磬app���ղ��˳�		//JSH 20171209
	if(cpu.event_tcb.event_id == CPU_EVENT_TASK_CAMERA)
	{
		Ble_SendBatMulData(3,0xB6,0x01,0x00);
	}
		
    if(cpu.task_tcb.task_id == CPU_TASK_MAIN)
    {
        // 1.������в���
        memset(&cpu.watch, 0, sizeof(watch_timer_t));
        memset(&cpu.event_tcb, 0, sizeof(cpu_event_tcb_t));
        // ����¼�
        EventManage_ClearEventList();
    }
    else if((cpu.event_tcb.event_id != CPU_EVENT_TASK_CHARGE)&&(cpu.event_tcb.event_id != CPU_EVENT_TASK_POWERON))
    {
        //YX.20180625.ʹ���˶����������⣬ʹ���жϻ��Ѹ��Ӻ�����ǰʹ���˶���������Ϊ����ʱ�Լ��ٶ���Ӱ�죩
        //�˶�����ʵʱ����ģ�������ٶ����ߣ��˶�����һֱ�����ϴε��˶������䣬�����һֱ���ѳ��ͼ��
        if(Battery_TakeMove())
        {
            // 1.������в���
            memset(&cpu.watch, 0, sizeof(watch_timer_t));
            memset(&cpu.task_tcb, 0, sizeof(cpu_task_tcb_t));
            memset(&cpu.event_tcb, 0, sizeof(cpu_event_tcb_t));
            // ����¼�
            EventManage_ClearEventList();
            
            // ��ӳ���¼�
            GuiThread_AddEvent(CPU_EVENT_TASK_CHARGE);
        }
    }
}

void GuiThread_ChargFullCommit(void)
{
	//�޸� �ֻ���������ȥ��磬app���ղ��˳�		//JSH 20171209
	if(cpu.event_tcb.event_id == CPU_EVENT_TASK_CAMERA)
	{
		Ble_SendBatMulData(3,0xB6,0x01,0x00);
	}
    //YX.20171128.�޸�����ʱ��������ʾ�������˲�䣨1S���ڣ���ʾ����ͼ��
    if(cpu.task_tcb.task_id == CPU_TASK_MAIN)
    {
        // 1.������в���
        memset(&cpu.watch, 0, sizeof(watch_timer_t));
        memset(&cpu.event_tcb, 0, sizeof(cpu_event_tcb_t));
        // ����¼�
        EventManage_ClearEventList();
    }
    else if((cpu.event_tcb.event_id != CPU_EVENT_TASK_CHARGE)&&(Battery_Get_Status() == BATTERY_POWER_FULL)&&(cpu.event_tcb.event_id != CPU_EVENT_TASK_POWERON))
    {
        //YX.20180625.ʹ���˶����������⣬ʹ���жϻ��Ѹ��Ӻ�����ǰʹ���˶���������Ϊ����ʱ�Լ��ٶ���Ӱ�죩
        //�˶�����ʵʱ����ģ�������ٶ����ߣ��˶�����һֱ�����ϴε��˶������䣬�����һֱ���ѳ��ͼ��
        if(Battery_TakeMove())
        {
            // 1.������в���
            memset(&cpu.watch, 0, sizeof(watch_timer_t));
            memset(&cpu.task_tcb, 0, sizeof(cpu_task_tcb_t));
            memset(&cpu.event_tcb, 0, sizeof(cpu_event_tcb_t));
            // ����¼�
            EventManage_ClearEventList();

            // ��ӳ���¼�
            GuiThread_AddEvent(CPU_EVENT_TASK_CHARGE);
        }	
        
    }
}

void GuiThread_ClearAllParam(void)
{
    // ��������¼��б�
    EventManage_ClearEventList();
    
    // ���CPU�������в���(��������˶�ģʽ)
    memset(&cpu, 0, sizeof(cpu_param_t));
    
    // YX.20180418.����ʱ���
    p_flash_param->hourglass_s.btn_trigger = 0;
    p_flash_param->hourglass_s.app_trigger = 0;
    p_flash_param->hourglass_s.app_send = 0;
    p_flash_param->hourglass_s.run_remain_time = 0;
    
    //YX.20180418.�����Ϣ����
    memset(&p_data_manage->app_notify_s,0,sizeof(app_notify_attr_t));
    
    //YX.20180418.�޸��ػ�������������������
    p_data_manage->sedentary_cnt = 0;
}

void GuiThread_InMainTask(void)
{
    Cpu_ScheduleTask(&cpu.task_tcb, &cpu.device_tcb, CPU_TASK_MAIN);
}

void GuiThread_ClearMovementParm(void)
{
    memset(&cpu.movement,0,sizeof(movement_cont_t));
}

void GuiThread_InMovementTask(void)
{
    xSemaphoreGive(st_gui_res.m_task_count);
    
    if(eTaskGetState(st_gui_res.m_thread_handle) != eRunning) 
    {
        xTaskResumeFromISR(st_gui_res.m_thread_handle);  
    }
    Cpu_ScheduleTask(&cpu.task_tcb, &cpu.device_tcb, CPU_TASK_MOVEMENT);
    memset(&cpu.movement,0,sizeof(movement_cont_t));
    cpu.movement.enable = 1;
    cpu.movement.face_num = 1;
    cpu.movement.run = 1;
}

//void GuiThread_WakeUp(void)
//{
//    xSemaphoreGive(st_gui_res.m_task_count);

//    if(eTaskGetState(st_gui_res.m_thread_handle) != eRunning) 
//    {
//        xTaskResumeFromISR(st_gui_res.m_thread_handle);  
//    }
//}

/****************************************************************
** Function name:           Gui_ThreadSetDisPlayArea
** Descriptions:            ���õ�ǰ��ʾ���������´���ʾ����
** input parameters:        start_column:��ʼ�У�start_page����ʼ�У�
                            column_size�������У�page_size:������
** output parameters:       �� 
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-08-02
*****************************************************************/ 
void Gui_ThreadSetDisPlayArea(uint8_t start_column, uint8_t start_page, uint8_t column_size, uint8_t page_size)
{    
    cpu.device_tcb.cls_param.start_column = start_column;
    cpu.device_tcb.cls_param.start_page = start_page;
    cpu.device_tcb.cls_param.column_size = column_size;
    cpu.device_tcb.cls_param.page_size = page_size;
}

/****************************************************************
** Function name:           Gui_ThreadUpdateMian
** Descriptions:            ����������
** input parameters:        �� 
** output parameters:       �� 
** Returned value:          ��
** Created by:              WJ
** Created Date:            2017-08-12
*****************************************************************/ 
void Gui_ThreadUpdateMian(void)
{
    if(cpu.task_tcb.task_id == CPU_TASK_MAIN)
    {
        cpu.task_tcb.task_controller.init = 1;
    }
}

/****************************************************************
** Function name:           GuiThread_ClearTaskParam
** Descriptions:            ��������һЩ����
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              YX
** Created Date:            2018-04-27
*****************************************************************/
void GuiThread_ClearTaskParam(void)
{	
	cpu.task_tcb.child_task_id = 0;
    cpu.task_tcb.gui_controller.animation_frame_index = 0;
    cpu.task_tcb.gui_controller.animation_tick_counter = 0;
}

/****************************************************************
** Function name:           GuiThread_SpiSleep
** Descriptions:            spi ��������ʱUI�߳�����
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ
** Created Date:            2018-07-11
*****************************************************************/
void GuiThread_SpiSleep(void)
{
    if(st_gui_res.init_complete )
    {
        st_gui_res.sleep_flag = 1;
        xSemaphoreTake(st_gui_res.m_thread_sleep, portMAX_DELAY); 
        st_gui_res.sleep_flag = 0;
        st_gui_res.sleep_time = 0;
    }
}

/****************************************************************
** Function name:           GuiThread_SpiWeakUp
** Descriptions:            spi �����������ʱUI�̻߳���
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ
** Created Date:            2018-07-11
*****************************************************************/
void GuiThread_SpiWeakUp(void)
{
    BaseType_t yield_req = pdFALSE;
    if(st_gui_res.init_complete)
    {
        // The returned value may be safely ignored, if error is returned it only means that
        // the semaphore is already given (raised).
        UNUSED_VARIABLE(xSemaphoreGiveFromISR(st_gui_res.m_thread_sleep, &yield_req));
        portYIELD_FROM_ISR(yield_req);  
    }        
}

/****************************************************************
** Function name:           Gui_CheckEvent
** Descriptions:            SPI�¼��Լ�
** input parameters:        �� 
** output parameters:       �� 
** Returned value:          ��
** Created by:              WJ             
** Created Date:            2018-10-31
*****************************************************************/ 
void Gui_CheckEvent(void)
{
    //WJ.20180906.�����߳����������쳣�Լ촦��
    if(st_gui_res.init_complete)
    {
        if(st_gui_res.sleep_flag == 1)
        {
            st_gui_res.sleep_time++;
            if(st_gui_res.sleep_time >= 2)
            {
                GuiThread_SpiWeakUp();
            }
        }
    } 
}

