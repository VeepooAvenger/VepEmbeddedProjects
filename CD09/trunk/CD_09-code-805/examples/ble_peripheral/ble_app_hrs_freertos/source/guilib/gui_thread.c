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
static uint8_t stop_watch_frist_flag = 0;                           //���ڼ�¼���������һ������
uint8_t display_language = GUI_LANGUAGE_ENAGLISH;                   //UI��ʾ����,Ĭ��Ӣ��
uint8_t main_background_color = 0;                                  //������

static uint8_t posture_to_bright_flg = 0;                           //��̬�����ı�־
uint8_t posture_to_bright_flg1 = 0;                           //��̬�����ı�־
uint8_t mesg_to_delay = 0;                            //��Ϣ������ʱ


static uint8_t gui_movement_change_page = 0;                        //�˶�ģʽ����������
static uint8_t add_event_flag = 0;                                  //���ⲿ�¼�����
uint8_t delay_time_cnt = 0;
uint8_t ui_change_flag = 0;                      //�˶�ģʽ����������

static cpu_button_t cpu_button_s = {0,0};

uint8_t thread_handle_run = 0;                  //�߳�����״̬

extern power_statistics_t power_statistics_s;
extern low_energe_protocol_data_t low_energe_protocol_data;
extern uint8_t gui_counter;  //����GUI 125msһ�μ���
//----��������----
static void Gui_ThreadHandle(void* arg);
static void Gui_ThreadLcdDriveDeal(void);
static void Gui_ThreadTaskMonitor(void);
static void Gui_ThreadTaskManage(void);
static void Gui_ThreadEventManage(void);
uint8_t Gui_ThreadIsReadingNotify(uint8_t event);
static void Gui_ThreadDealKeyEvent(void);
static void Gui_ThreadTaskSwitching(void);

/****************************************************************
** Function name:           Gui_CreateResource
** Descriptions:            ����UI�����߳�
** input parameters:        ��
** output parameters:       �� 
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-08-02
*****************************************************************/
static __inline void Gui_CreateResource(void)
{	
    SEMAPHORE_CREATE_BINARY_RET(gui_res_s.m_thread_sleep);
	
	// �����߳�
	if(pdPASS != xTaskCreate(Gui_ThreadHandle, "Gui", 512, NULL, 2, &gui_res_s.m_thread_handle))  
	{
		APP_ERROR_HANDLER(NRF_ERROR_NO_MEM);
	}    
}

/****************************************************************
** Function name:           Gui_ThreadInit
** Descriptions:            UI�����߳�ע���UI���������ʼ��
** input parameters:        ��
** output parameters:       �� 
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-08-02
*****************************************************************/
void Gui_ThreadInit(void)
{
	memset(&cpu_s, 0, sizeof(cpu_task_t));
	
	cpu_task_controller_t *p_task = &cpu_s.job_control;
	//�������������
    
    TASK_ENTRY_INIT(p_task, 3*4);
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
    
    //UI�̳߳�ʼ�����.Ϊ1ʱ���Ƕ���ʽSPI����
    gui_res_s.sleep_flag = 0;
    gui_res_s.init_complete = 1;
}

/****************************************************************
** Function name:           Gui_ThreadWakeUpThread
** Descriptions:            ����UI�߳�
** input parameters:        ��
** output parameters:       �� 
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-08-02
*****************************************************************/
void Gui_ThreadWakeUpThread(void)
{  
    //WJ.20180828.�߳�δ��ʼ��������ʹ����ص���
    if(gui_res_s.init_complete != 1)
    {
        return;
    }
    
//	if(eRunning != eTaskGetState(gui_res_s.m_thread_handle)) 
	{ 
		vTaskResume(gui_res_s.m_thread_handle); 
	}     
    
    delay_time_cnt = 0;
    ui_change_flag = 1;   
}

/****************************************************************
** Function name:           Gui_ThreadPageActive
** Descriptions:            �Ƿ���UI��ʾ�¼���
** input parameters:        ��
** output parameters:       �� 
** Returned value:          1:�����˶����� 0���������˶�����
** Created by:              WJ              
** Created Date:            2017-08-02
*****************************************************************/
bool Gui_ThreadPageActive(void)
{
	return (cpu_s.task != CPU_TASK_IDEL) || (cpu_s.event > CPU_EVENT_TASK_ALL_EXIT);
}

/****************************************************************
** Function name:           Gui_ThreadTaskPageActive
** Descriptions:            �Ƿ���UI��ʾ�¼���
** input parameters:        ��
** output parameters:       �� 
** Returned value:          1:�����˶����� 0���������˶�����
** Created by:              WJ              
** Created Date:            2017-08-02
*****************************************************************/
bool Gui_ThreadTaskPageActive(void)
{
	return (cpu_s.task != CPU_TASK_IDEL);
}

/****************************************************************
** Function name:           Gui_ThreadIsTaskRun
** Descriptions:            �Ƿ����˶�����
** input parameters:        ��
** output parameters:       �� 
** Returned value:          1:�����˶����� 0���������˶�����
** Created by:              WJ              
** Created Date:            2017-08-02
*****************************************************************/
bool Gui_ThreadIsTaskRun(void)
{
	return (cpu_s.task == CPU_TASK_MOVEMENT);
}

/****************************************************************
** Function name:           Gui_ThreadHandle
** Descriptions:            UI�����߳�
** input parameters:        ��
** output parameters:       �� 
** Returned value:          ��
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
        //WJ.20180414.�Ż���������ʽ
        Gui_ThreadDealKeyEvent();
        
        if(delay_time_cnt == 0 || ui_change_flag == 1)
        {
//            //WJ.20180303.
//            if(cpu_s.oled.oled_drive_light == 1 && cpu_s.oled.oled_light_status == 0)
//            {
//                //����LCD,�ӿ�LCD�������ٶȣ�Ŀǰ����32M�����²��ӻ�������
//                Gui_LcdWakeUp();
//            }            
            //YX.20180914.����������ʱ���������⣬����ʾ����
            if(posture_to_bright_flg == 1)
            {
                posture_to_bright_flg = 0;
                Gui_ThreadTaskSwitching();
                posture_to_bright_flg1 = 1;
            }
            
            ui_change_flag = 0; 
            if(Gui_DataGetEncrypt())
            {
                if(Gui_GetTraverseUiState() == 0)
                {
                    display_language = Gui_DataGetSystemLanguage();
                    main_background_color = Gui_DataGetUiStyle();
                    //��Ӧͨ��ģʽ����
                    Gui_ThreadTaskManage();
                    
                    //��Ӧ�ⲿ�¼�
                    Gui_ThreadEventManage();
                }
                else
                {
                    Gui_ThreadTestManage();
                }
            }
            else
            {
                //��ʾXXX
                Gui_PageDisXxxIcon();
            }
            
            //����ʱ�����
            Gui_ThreadTaskMonitor();
                   
            //LCD�����������Ϳ�����
            Gui_ThreadLcdDriveDeal();
        }
		
        
		if(cpu_s.oled.oled_light_status == 1 || gui_thread_hang < 2
          || cpu_s.oled.oled_drive_light == 1) 
        {
            if(mesg_to_delay == 1 
              && low_energe_protocol_data.state == 1
              && cpu_s.gui.page_frame_counter < low_energe_protocol_data.motor_delay_time_value /2
              && cpu_s.gui.page_frame_counter != 0xFF)
            {
                gui_thread_hang = 0;
            }
            else if(cpu_s.oled.oled_light_status == 0)
            {
                gui_thread_hang++;
                continue;
            }
            else
            { 
                gui_thread_hang = 0;
            }	
            //WJ.20180808.��������ˢ�´���
            if(cpu_s.gui.updata_ui_interval == 0)
            {
                delay_time_cnt = 0;
            }
            else
            {   
                delay_time_cnt++;
                if(delay_time_cnt*50 > cpu_s.gui.updata_ui_interval)
                {
                    uint16_t delay_time = cpu_s.gui.updata_ui_interval - (delay_time_cnt - 1)*50;
                    if(delay_time == 0)
                    {
                        vTaskDelay(10);
                    }
                    else
                    {
                        vTaskDelay(delay_time);
                    }
                    delay_time_cnt = 0;
//                    vTaskDelay(cpu_s.gui.updata_ui_interval - (delay_time_cnt - 1)*50);
                }
                else                
                {
                    vTaskDelay(50); 
                }
            }            
        }
		else
        {
            gui_thread_hang = 0;
            thread_handle_run = 0;
            
            posture_to_bright_flg1 = 0;
            
			vTaskSuspend(gui_res_s.m_thread_handle);
            delay_time_cnt = 0;
        }
	}
}

/****************************************************************
** Function name:           Gui_ThreadTaskManage
** Descriptions:            �������¼����洦��
** input parameters:        ��
** output parameters:       �� 
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-08-02
*****************************************************************/ 
static void Gui_ThreadTaskManage(void)
{
    static uint8_t clear_background_flag = 0;
    
	//������ⲿ�¼�
	if(cpu_s.event != CPU_EVENT_TASK_IDEL)
    {
        clear_background_flag = 1;
        return;
    }

    //YX.20181113.�޸��ⲿ�¼����ڲ��¼��ص�������
    if(add_event_flag == 1 && cpu_s.job_control.init != 0x01)
    {
        clear_background_flag = 1;
    }
    add_event_flag = 0;
//    add_event_flag = 0;
    
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
            //������
			Cpu_TaskMain(&cpu_s.job_control, &cpu_s.gui);
			break;
		case CPU_TASK_STEP:     
            //�Ʋ�
			Cpu_TaskStep(&cpu_s.job_control, &cpu_s.gui);
			break;
        case CPU_TASK_DISTANCE:
            //����
			Cpu_TaskDistance(&cpu_s.job_control, &cpu_s.gui);
            break;
        case CPU_TASK_MOVEMENT:
            //�˶�ģʽ
			Cpu_TaskRun(&cpu_s.job_control, &cpu_s.gui);
            break;
        case CPU_TASK_CALORIES:   
            //��·��
			Cpu_TaskCalories(&cpu_s.job_control, &cpu_s.gui);         
            break;
        case CPU_TASK_BP:       
            //Ѫѹ
            Cpu_TaskBP(&cpu_s.job_control, &cpu_s.gui);
            break;
		case CPU_TASK_HEART:    
            //����
			Cpu_TaskHeart(&cpu_s.job_control, &cpu_s.gui);
			break;
		case CPU_TASK_SLEEP:    
            //˯��
			Cpu_TaskSleep(&cpu_s.job_control, &cpu_s.gui);
			break;
		case CPU_TASK_FINDPHONE:     
            //�����ֻ�
			Cpu_TaskFindPhone(&cpu_s.job_control, &cpu_s.gui);
			break;
		case CPU_TASK_STOP_WATCH:    
            //���
			Cpu_TaskStopWatch(&cpu_s.job_control, &cpu_s.gui);
			break;
		case CPU_TASK_COUNT_DOWN:    
            //����ʱ
			Gui_TaskCountDown(&cpu_s.job_control, &cpu_s.gui);
			break;
		case CPU_TASK_POWEROFF:                  
            //�ػ�
			Cpu_TaskPowerOff(&cpu_s.job_control, &cpu_s.gui);
			break;
		
        case CPU_TASK_MOVEMENT_PAUSE:   
            // �˶�ģʽ��ͣ
            Cpu_TaskRunModePause(&cpu_s.job_control, &cpu_s.gui);
            break;  
   
        case CPU_TASK_MOVEMENT_EXIT: 
            // �˶�ģʽ�˳�
            Cpu_TaskRunModeExit(&cpu_s.job_control, &cpu_s.gui);
            break;        
        
		default:
			cpu_s.task = CPU_TASK_IDEL;
			break;
	}
}

/****************************************************************
** Function name:           Gui_ThreadCpuExitAllTask
** Descriptions:            �ⲿ�¼����洦���˳�
** input parameters:        ��
** output parameters:       �� 
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-08-02
*****************************************************************/ 
static void Gui_ThreadCpuExitAllTask(void)
{
	//�����������
	cpu_s.event_control.active = 1;
	cpu_s.event_control.keep_time = 0;
	cpu_s.event_control.run_time = 1;
}

/****************************************************************
** Function name:           Gui_ThreadEventManage
** Descriptions:            �ⲿ�¼����洦��
** input parameters:        ��
** output parameters:       �� 
** Returned value:          ��
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
            //���
			Cpu_EventIdelCharge(&cpu_s.event_control, &cpu_s.gui);
			break;
        
		case CPU_EVENT_TASK_ALARM:
            //����
			Cpu_EventAlarm(&cpu_s.event_control, &cpu_s.gui);
			break;
		
		case CPU_EVENT_TASK_SIT:
            //����
			Cpu_EventSedentary(&cpu_s.event_control, &cpu_s.gui);
			break;
        
		case CPU_EVENT_TASK_CALL_REMIND:
            //����
			Cpu_EventCallRemind(&cpu_s.event_control, &cpu_s.gui);
            break;
        
		case CPU_EVENT_TASK_MESSAGE_REMIND:    
            //��Ϣ
			Cpu_EventMessageRemind(&cpu_s.event_control, &cpu_s.gui);
			break;
		
		case CPU_EVENT_TASK_HEARTALERT: 
            //����
			Cpu_EventHeartAlert(&cpu_s.event_control, &cpu_s.gui);
			break;
		
		case CPU_EVENT_TASK_CAMERAREADY:    
            //����
			Cpu_EventCameraReady(&cpu_s.event_control, &cpu_s.gui);
			break;
		
		case CPU_EVENT_TASK_CAMERAOK:       
            //�������
			Cpu_EventCameraOK(&cpu_s.event_control, &cpu_s.gui);
			break;
		 
		case CPU_EVENT_TASK_FONTTEST:       
            //����
			Cpu_EventFontTest(&cpu_s.event_control, &cpu_s.gui);
			break;
		
		case CPU_EVENT_TASK_LOWPOWER:      
            //�͵�
			Cpu_EventLowPowerHint(&cpu_s.event_control, &cpu_s.gui);
			break;
		
		case CPU_EVENT_TASK_COUNT_DOWN_TIMEOUT:  
            //����ʱ
			Cpu_EvenCountDownTimeOut(&cpu_s.event_control, &cpu_s.gui);
			break;
			
        case CPU_EVENT_TASK_COUNT_DOWN_START:            
			Cpu_EvenCountDownTimeStart(&cpu_s.event_control, &cpu_s.gui);
            break;
        
        case CPU_EVENT_TASK_STARGET:
            Cpu_EvenStarget(&cpu_s.event_control, &cpu_s.gui);
            break;
        
        case CPU_EVENT_TASK_MAC_VER:
            //��ʾmac��ver
            Cpu_EvenDisMacVer(&cpu_s.event_control, &cpu_s.gui);
            break;
        case CPU_EVENT_TASK_PHYS_REMIND:
            //Ů��
            Cpu_EvenPhysRemind(&cpu_s.event_control, &cpu_s.gui);
            break;
        case CPU_EVENT_TASK_BLE_DISC_REMIND:
            Cpu_EvenBleDiscRemind(&cpu_s.event_control, &cpu_s.gui);
            break;
		default:
			cpu_s.event = CPU_EVENT_TASK_IDEL;
			break;
	}
}

/****************************************************************
** Function name:           Gui_ThreadLcdDriveDeal
** Descriptions:            ��Ļ״̬����
** input parameters:        ��
** output parameters:       �� 
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-08-02
*****************************************************************/ 
static void Gui_ThreadLcdDriveDeal(void)
{
    static uint32_t    last_oled_time = 0;
    static uint32_t    res_oled_time = 0;
    
    //��ֹ�м���������¼����º���   
    if(cpu_s.oled.oled_drive_off == 1 && cpu_s.oled.oled_drive_light != 1)
	{
        cpu_s.oled.oled_drive_off = 0;
        if(cpu_s.oled.oled_light_status == 1)
        {
            //���ｫ�����
            cpu_s.oled.oled_drive_off = 0;
            cpu_s.oled.oled_light_status = 0;
            //WJ.20180205.�����ȹرձ��⴦��
            Gui_LcdLedOff();
            Gui_LcdDriverInit();
            Gui_LcdDriverOff();
            if(cpu_s.task != CPU_TASK_IDEL)
            {
                cpu_s.task = CPU_TASK_IDEL;
            }
            
            // ����ʱ��
            uint32_t  c_oled_time = (uint32_t)xTaskGetTickCount();
            if(c_oled_time >= last_oled_time)
            {
                last_oled_time = c_oled_time - last_oled_time + res_oled_time;      
            }
            else
            {   // �����ת����
                last_oled_time = (0xFFFFFFFF - last_oled_time) + c_oled_time + res_oled_time;   
            }             
            res_oled_time = last_oled_time % 1000;
            power_statistics_s.bright_screen_time += last_oled_time / 1000;
            
        }
	}
    
	if(cpu_s.oled.oled_drive_light == 1)
	{
        if(mesg_to_delay == 1 
          && low_energe_protocol_data.state == 1
          && cpu_s.gui.page_frame_counter < low_energe_protocol_data.motor_delay_time_value /2
          && cpu_s.gui.page_frame_counter != 0xFF)
        {
            return;
        }
        //���ｫ������
        cpu_s.oled.oled_drive_light = 0;
        if(cpu_s.oled.oled_light_status == 0)
        {
            cpu_s.oled.oled_light_status = 1;
            Gui_LcdDriverOn();
            
            power_statistics_s.bright_screen_num++;
            last_oled_time = (uint32_t)xTaskGetTickCount();
        }
        
	}
	
	// ���������ȵ��ڿ���
	if((cpu_s.oled.oled_drive_bright == 1) && (cpu_s.oled.oled_light_status == 1))
	{
		cpu_s.oled.oled_drive_bright = 0;
//		OLED_Driver_SetBrightnessCmd(p_data_manage->oled_bright);
	}
}

/****************************************************************
** Function name:           Gui_ThreadJobTaskRTMonitor
** Descriptions:            ������ʱ���������
** input parameters:        ��
** output parameters:       �� 
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-08-02
*****************************************************************/ 
static void Gui_ThreadJobTaskRTMonitor(void) 
{  
	if(cpu_s.job_control.active == 1)
	{ 
		cpu_s.job_control.run_time++;
		
		// �������֧�ֳ�������,����Ҫ���ֽ�������
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
                //�رհ�������������Ѫѹ����
                if(cpu_s.task == CPU_TASK_HEART || cpu_s.task == CPU_TASK_BP)
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
                    //WJ.20180320.�޸�������ת��ʾʱ�䲻�������
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
                    //WJ.20180320.�޸�������ת��ʾʱ�䲻�������
                    ui_change_flag = 1;
                    delay_time_cnt = 0;
                    cpu_s.gui.updata_ui_interval = 50;
                }
                else
                {
                    cpu_s.job_control.key_press_hold = 0;
                    cpu_s.job_control.active = 0;
                    cpu_s.task = CPU_TASK_IDEL;
                    //�����̬����״̬
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
** Descriptions:            �ⲿ����ʱ���������
** input parameters:        ��
** output parameters:       �� 
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-08-02
*****************************************************************/ 
void Gui_ThreadEventTaskRTMonitor(void) 
{  
	if(cpu_s.event_control.active == 1)
	{
		cpu_s.event_control.run_time++; 
        
        // �������֧�ֳ�������,����Ҫ���ֽ�������
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
                cpu_s.event = CPU_EVENT_TASK_IDEL;
                //����Ѷ���Ϣ
                Gui_DataClearUnreadMessage();
                Gui_DataClearCallingState();
                Gui_DataClearPhysMessage();
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
** Descriptions:            ������ʱ�����
** input parameters:        ��
** output parameters:       �� 
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-08-02
*****************************************************************/ 
static void Gui_ThreadTaskMonitor(void)
{		
	static uint8_t task_counter = 0xFF;
	uint8_t current_counter = 0;
#if APP_MOTOR_ENABLED
    current_counter = gui_counter;
#else
    current_counter = clock->seconds ;
#endif
    
	//ע��,��������ÿ������
	if(task_counter != current_counter 
      || (cpu_s.task == CPU_TASK_IDEL && cpu_s.event == CPU_EVENT_TASK_IDEL))
	{
		task_counter = current_counter;
		Gui_ThreadJobTaskRTMonitor();
		Gui_ThreadEventTaskRTMonitor();
        
        if(cpu_s.task == CPU_TASK_IDEL && cpu_s.event == CPU_EVENT_TASK_IDEL)
        {
            cpu_s.oled.oled_drive_off = 1;    //����
        }
	}
	cpu_s.gui.page_frame_counter++;
    //if(cpu_s.gui.page_frame_counter > 240)
    //{
    //    cpu_s.gui.page_frame_counter = 1;
    //}
}

/****************************************************************
** Function name:           Gui_ThreadEventTaskClear
** Descriptions:            ����ⲿ�¼�
** input parameters:        ��
** output parameters:       �� 
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-08-02
*****************************************************************/ 
void Gui_ThreadEventTaskClear(void)
{
	cpu_s.event = CPU_EVENT_TASK_IDEL;
	cpu_s.event_control.active = 1;
    cpu_s.event_control.keep_time = 0;
    cpu_s.event_control.run_time = 1;
    cpu_s.event_control.key_press_hold = 0;
    
    ui_change_flag = 1;
}

/****************************************************************
** Function name:           Gui_ThreadTaskClear
** Descriptions:            �����ǰ�¼��¼�
** input parameters:        ��
** output parameters:       �� 
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-08-02
*****************************************************************/ 
void Gui_ThreadTaskClear(void)
{
    //YX.20181017.���������ʡ�Ѫѹ����ʱ�����ʵ�û�йص�����
    if(cpu_s.task == CPU_TASK_BP || cpu_s.task == CPU_TASK_HEART)
    {
        Gui_DataCloseHeartOrBpOpreation();
    }
    
	cpu_s.task = CPU_TASK_IDEL;
	cpu_s.job_control.active = 1;
    cpu_s.job_control.keep_time = 0;
    cpu_s.job_control.run_time = 1;
    cpu_s.job_control.key_press_hold = 0;
    
    ui_change_flag = 1;
}

/****************************************************************
** Function name:           Gui_ThreadTaskWakeUp
** Descriptions:            ����¼�������ui����
** input parameters:        task:�¼�����
** output parameters:       �� 
** Returned value:          ��
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
    
	Gui_ThreadWakeUpThread();
}

/****************************************************************
** Function name:           Gui_ThreadEventTaskWakeUp
** Descriptions:            ��������¼�������ui����
** input parameters:        event:�ⲿ�¼�����
** output parameters:       �� 
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-08-02
*****************************************************************/ 
void Gui_ThreadEventTaskWakeUp(CPU_EVENT_TYPE event)
{    
    if(Gui_ThreadCanAcceptEvent(event) == 0)
    {
        return;
    }
	//YX.201801017.���������󣬻�����ʾ������������
    if(event == CPU_EVENT_TASK_CALL_REMIND)
    {
        Gui_ThreadTaskClear();
    }
    
    add_event_flag = 1;
    
    cpu_s.gui.animation_enable = 0x00;
	cpu_s.event = event;
	cpu_s.event_control.init = 1;
    
    //ZJH.20181210.���Ϣ��ʱ���ⲿ�¼����ⲿ�¼�������Ӧ��BUG
    cpu_s.event_control.active = 0;
    
	cpu_s.oled.oled_drive_light = 1;
	
    if(Gui_DataSystemIsTestMode() == 0)
    {
        //ֹͣ��
        Motor_DelEvent();
    }
    
    delay_time_cnt = 0;
    ui_change_flag = 1;  
	Gui_ThreadWakeUpThread();  
}

/****************************************************************
** Function name:           Gui_ThreadStopWatchSwitching
** Descriptions:            �����ҳ���л�
** input parameters:        ��
** output parameters:       �� 
** Returned value:          ��
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
        //������UI����
        stop_watch_frist_flag = 0;
        cpu_s.task = CPU_TASK_STOP_WATCH;
        cpu_s.job_control.init = 1;
    }            
    gui_movement_change_page = 1;  
}

/****************************************************************
** Function name:           Gui_ThreadMovementModeSwitching
** Descriptions:            �˶�ģʽ��ҳ���л�
** input parameters:        ��
** output parameters:       �� 
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-08-11
*****************************************************************/ 
static void Gui_ThreadMovementModeSwitching(void)
{
    /* �˶�ģʽ��*/
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
** Descriptions:            ҳ���л�
** input parameters:        ��
** output parameters:       �� 
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-08-02
*****************************************************************/ 
static void Gui_ThreadTaskSwitching(void)
{    
    uint8_t event_flag = 0;
    
    
    posture_to_bright_flg1 = 0;
    mesg_to_delay = 0;
    
    /*�л�����*/
    if(cpu_s.event != CPU_EVENT_TASK_IDEL)
    {    
        //ȷ���ⲿ�¼���ʾ
        if(cpu_s.event_control.run_time >= 1 && cpu_s.event_control.init != 1)
        {
            //�˳��ⲿ�¼�
            Gui_ThreadEventTaskClear();
            if(cpu_s.task != CPU_TASK_IDEL)
            {
                //�豸û�д��ڲ���ģʽ,��Ҫ�ر���
                if(!Gui_DataSystemIsTestMode())
                {
                    Motor_DelEvent();    //ֹͣ��
                }
                return;
            } 
            event_flag = 1;
        }
        else
        {
            //WJ.20180130.��ֹ�߳�ֹͣ�¼�û�д������
			Gui_ThreadWakeUpThread();
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
        //�ж��Ƿ��������״̬��
       Gui_ThreadStopWatchSwitching();           
    }
    else if(Gui_DataMovementModeIsBusy())
    {
        /* �˶�ģʽ��*/
        Gui_ThreadMovementModeSwitching();
    }
    else
    {
        // ����Ǵ���ʱ,���ж��Ƿ�͵�����
        //WJ.2080509.������ⲿʱ��󣬽���������ǰ��ʾ�͵�ͼ�������
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
            //WJ.20180320.��ֹ������û����ʾ��ҳ
            if(cpu_s.task == CPU_TASK_MAIN && cpu_s.job_control.init == 1)
            {  
                Gui_ThreadWakeUpThread();
                return;
            }
            //�رհ�������������Ѫѹ����
            if(cpu_s.task == CPU_TASK_HEART || cpu_s.task == CPU_TASK_BP)
            {
                Gui_DataCloseHeartOrBpOpreation();
            } 
            
            cpu_s.task++;		  
            
            if((cpu_s.task == CPU_TASK_BP) && Gui_DataIsBpFunction() != 1)
            {
                cpu_s.task++;
            }
            if((cpu_s.task == CPU_TASK_FINDPHONE) && Gui_DataFindPhonePageIsEnable() != 1)
            {
                cpu_s.task++;
            }
            if((cpu_s.task == CPU_TASK_STOP_WATCH) && Gui_DataStopWatchPageIsEnable() != 1)
            {
                cpu_s.task++;
            }           
            if((cpu_s.task == CPU_TASK_COUNT_DOWN) && Gui_DataCountDownPageIsEnable() != 1)
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
    
    //�豸û�д��ڲ���ģʽ,��Ҫ�ر���
    if(!Gui_DataSystemIsTestMode())
    {
        Motor_DelEvent();    //ֹͣ��
    }

    cpu_s.oled.oled_drive_light = 1;     
    Gui_ThreadWakeUpThread();      
}

/****************************************************************
** Function name:           Gui_ThreadKeyClickTask
** Descriptions:            �����¼������̰�����
** input parameters:        ��
** output parameters:       �� 
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-08-02
*****************************************************************/ 
static void Gui_ThreadKeyClickTask(void)
{          
   	if(!Gui_DataSystemIsPoweOn())
	{
        /*�ػ��¶̰���Ч,ֱ�ӷ���*/
        return;
	}
    if(Gui_DataGetBatteryLevelAndState(NULL) != BATTERRY_IS_NORMAL)
    {
        /*�����*/
        if(Gui_GetThreadTask() != CPU_TASK_MAIN)
        {
            Gui_ThreadTaskWakeUp(CPU_TASK_MAIN);
        }
        cpu_s.event = CPU_EVENT_TASK_IDEL;
        return;
    }
    
    //�Ƿ�������״̬,�����о�������
    if(cpu_s.event == CPU_EVENT_TASK_CALL_REMIND && Gui_DataMessageIsCall())
    {
        if( Gui_DataMessageCallState() < GUI_CALL_REJECTSUC)
        {
            //�������羲��
            Gui_DataMessageCallToMute();
            Motor_DelEvent();    //ֹͣ��          
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
       
    //�Ƿ�������
    if(cpu_s.event == CPU_EVENT_TASK_CAMERAREADY     // �¼�����--����׼������
      || cpu_s.event == CPU_EVENT_TASK_CAMERAOK)     // �¼�����--�������
    {
        Gui_DataClickOnThePhoto();
        Gui_ThreadEventTaskWakeUp(CPU_EVENT_TASK_CAMERAOK);
        return;
    }       
    
    //�Ƿ���δ����Ϣ
    //WJ.20171227.������פ������Ϣ���ܷ�ҳ������
//    if(cpu_s.task != CPU_TASK_STOP_WATCH)
//    {
            
        //Ů�Թ�������δ��ȡ
        if((cpu_s.event == CPU_EVENT_TASK_PHYS_REMIND || Gui_ThreadCanAcceptEvent(CPU_EVENT_TASK_PHYS_REMIND))
            && Gui_DataHaveUnreadPhysMessage(Gui_ThreadIsReadingNotify(CPU_EVENT_TASK_PHYS_REMIND)))
        {
            //����UI�¼�����UI������ʾ
            Gui_ThreadEventTaskWakeUp(CPU_EVENT_TASK_PHYS_REMIND);            
            //ֹͣ�� 
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
            //����UI�¼�����UI������ʾ
            Gui_ThreadEventTaskWakeUp(CPU_EVENT_TASK_MESSAGE_REMIND);            
            //ֹͣ�� 
            Motor_DelEvent();
            cpu_s.event_control.keep_time = 5;
            cpu_s.event_control.run_time = 0;
            
            return;
        }
        if(cpu_s.event == CPU_EVENT_TASK_MESSAGE_REMIND && cpu_s.event_control.init != 0x01)
        {            
            cpu_s.event = CPU_EVENT_TASK_IDEL;
            
            if(cpu_s.task != CPU_TASK_IDEL)
            {
                return;
            }
            
        }

//    }

    Gui_ThreadTaskSwitching();        
}

/****************************************************************
** Function name:           Gui_ThreadKeyLongPressTask
** Descriptions:            �����¼�����3s��������
** input parameters:        ��
** output parameters:       �� 
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-08-02
*****************************************************************/ 
static void Gui_ThreadKeyLongPressTask(void)
{
	if(!Gui_DataSystemIsPoweOn())
	{
        /*�ػ�������������*/    
        if(Gui_DataBatteryIsLowPower() == BATTERRY_IS_LOW_TO_SHUT)
        {
            //�͵���ʾ�͵�澯           
            Gui_ThreadEventTaskWakeUp(CPU_EVENT_TASK_LOWPOWER);  
        }
        else
        {        
            Gui_DataSystemToPoweOn();
            Motor_AddEvent(1, 0, MOTOR_TIME_06, 0);
        }
        return;
	}
        
    if(cpu_s.event == CPU_EVENT_TASK_CALL_REMIND && Gui_DataMessageIsCall())
    {
        /*����״̬,�����Ҷ�����*/
        Gui_DataMessageCallToHangUp();
        return;
    }
    else if(cpu_s.event == CPU_EVENT_TASK_CAMERAREADY 
           || cpu_s.event == CPU_EVENT_TASK_CAMERAOK)
    {
        /* �˳�����*/
        Gui_DataLongKeyToExitPhoto();
        return;
    }
    
    if(cpu_s.event == CPU_EVENT_TASK_IDEL)
    {
        if(cpu_s.task == CPU_TASK_MOVEMENT_PAUSE)
        {
            /*��ͣ��ʼ�˶�ģʽ*/
            Gui_DataToggleMovementMode(); 
            Gui_ThreadEventTaskClear();  
            cpu_s.job_control.init = 0x01;     
            cpu_s.job_control.keep_time = 5;            
            cpu_s.job_control.run_time = 0;            
            Motor_AddEvent(1,0,MOTOR_TIME_06,0); 
        }
        else if(cpu_s.task == CPU_TASK_MOVEMENT_EXIT)
        {
            /*�˳��˶�ģʽ*/
            Gui_DataExitMovementMode();
            Gui_ThreadEventTaskClear();
            cpu_s.task = CPU_TASK_MAIN;		
            cpu_s.job_control.init = 0x01;
            gui_movement_change_page = 0; 
        }
        else if(cpu_s.task == CPU_TASK_MOVEMENT)
        {
            /*�����˶�ģʽ*/
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
        else if(cpu_s.task == CPU_TASK_FINDPHONE)
        {
            /*�ֻ�����*/
            cpu_s.gui.animation_enable = 0x01;
            cpu_s.job_control.keep_time = 60;
            cpu_s.job_control.run_time = 0;
            
        }
        else if(cpu_s.task == CPU_TASK_COUNT_DOWN)
        {
            /*����ʱ���ܰ�������*/
            
            //��Ҫ�жϵ���ʱ��ǰ״̬
            if(Gui_DataCountDownIsBusy())
            {
                Gui_DataCountDownExit();
                cpu_s.task = CPU_TASK_MAIN;
                cpu_s.job_control.keep_time = 3;            
                cpu_s.job_control.run_time = 0;		
                cpu_s.job_control.init = 0x01;      
            }
            else
            {
                Gui_DataCountDownStart();
                cpu_s.job_control.keep_time = 0;            
                cpu_s.job_control.run_time = 1;
//                cpu_s.gui.page_frame_counter = 1;
                //WJ.20171108.����ڳ�פ���濪������ʱ�������¼����
                Gui_ThreadEventTaskWakeUp(CPU_EVENT_TASK_COUNT_DOWN_START);                 
            }       
        }
        else if(cpu_s.task == CPU_TASK_STOP_WATCH)
        {
            /*���*/
            if(!Gui_DataStopWatchIsBusy())
            {
                /*������*/
                Gui_DataSetStopWatchToReady();   
                cpu_s.job_control.keep_time = 60;            
                cpu_s.job_control.run_time = 0;
                stop_watch_frist_flag = 1;
                cpu_s.gui.clear_background_must = 1;    
            }
            else
            {
                /*�˳����,��ת������*/
                Gui_DataExitStopWatch();
                //��ת������
                cpu_s.task = CPU_TASK_MAIN;		
                cpu_s.job_control.init = 0x01;
                stop_watch_frist_flag = 0;
            }
        }
        else if(cpu_s.task == CPU_TASK_POWEROFF)
        {
            /*�ػ�����*/
            Gui_DataSystemToPoweOff();
            Motor_AddEvent(1, 0, MOTOR_TIME_06, 0);
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
** Descriptions:            �����¼�����6s��������
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ
** Created Date:            2017-08-02
*****************************************************************/ 
static void Gui_ThreadKeyLongLongPressTask(void)
{   
   	if(!Gui_DataSystemIsPoweOn())
	{
        /*�ػ��¶̰���Ч,ֱ�ӷ���*/    
        return;
	}

	if(cpu_s.task == CPU_TASK_SLEEP)
	{
        /*�������*/
        Gui_DataClearSystemPassword();
		Motor_AddEvent(1,0,MOTOR_TIME_06,0);
	}        
}

/****************************************************************
** Function name:           Gui_ThreadAddKeyEvent
** Descriptions:            �����¼�����
** input parameters:        event����������
** output parameters:       �� 
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-08-02
*****************************************************************/ 
void Gui_ThreadAddKeyEvent(BUTTON_EVENT_TYPE event)
{ 
    cpu_button_s.button_event = event;
    
    //�����߳�
    Gui_ThreadWakeUpThread();
}

/****************************************************************
** Function name:           Gui_ThreadDealKeyEvent
** Descriptions:            �����¼���������������
** input parameters:        ��
** output parameters:       �� 
** Returned value:          ��
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
    /*����UI������*/
    Gui_SetTraverseUiState(0);
    
    //WJ.20180612.������Ҫͨ������ɾ���������������¼�	
    //WJ.20180403.ɾ��������������
//    Gui_DataDelBleDiscRemind();
 
    if(event == BUTTON_EVENT_CLICK)
    {
        /*�̰��¼�*/
        Gui_ThreadKeyClickTask();
    }
    else if(event == BUTTON_EVENT_LONGPRESS)
    {
        /*����3s�¼�*/
        Gui_ThreadKeyLongPressTask();
    }
    else if(event == BUTTON_EVENT_LONGPRESS_6S)
    {
        /*����6s�¼�*/
        Gui_ThreadKeyLongLongPressTask();       
    }
}

/****************************************************************
** Function name:           Gui_ThreadPostureToCloseScreen
** Descriptions:            ��̬����
** input parameters:        ��
** output parameters:       �� 
** Returned value:          ����0:�����¼����ʧ��, 1:�����¼���ӳɹ�
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
** Descriptions:            ��̬����
** input parameters:        ��
** output parameters:       �� 
** Returned value:          ����0:�����¼����ʧ��, 1:�����¼���ӳɹ�
** Created by:              WJ              
** Created Date:            2017-08-02
*****************************************************************/ 
uint8_t Gui_ThreadPostureToBrightScreen(void)
{
//    if(cpu_s.task == CPU_TASK_IDEL && cpu_s.event == CPU_EVENT_TASK_IDEL) 
//    {
//        Gui_ThreadTaskSwitching();
//        return 1;
//    }

    if(cpu_s.task == CPU_TASK_IDEL && cpu_s.event == CPU_EVENT_TASK_IDEL) 
    {
        posture_to_bright_flg = 1;
        Gui_ThreadWakeUpThread();
        return 1;
    }
    return 0;
}

/****************************************************************
** Function name:           Gui_ThreadIsReadingNotify
** Descriptions:            �Ƿ��ڲ鿴��Ϣ
** input parameters:        ��
** output parameters:       �� 
** Returned value:          ��
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
    cpu_s.gui.start_column = start_column;
    cpu_s.gui.start_page = start_page;
    cpu_s.gui.column_size = column_size;
    cpu_s.gui.page_size = page_size;
}

/****************************************************************
** Function name:           Gui_ThreadPowerOff
** Descriptions:            �ػ����洦��
** input parameters:        �� 
** output parameters:       �� 
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-08-12
*****************************************************************/ 
void Gui_ThreadPowerOff(void)
{        
	memset(&cpu_s, 0, sizeof(cpu_task_t));	
    cpu_s.event_control.active = 1;
    cpu_s.gui.updata_ui_interval = 200;   //200ms 
    cpu_s.event = CPU_EVENT_TASK_IDEL;
    cpu_s.task = CPU_TASK_IDEL;  
    if(Gui_DataGetBatteryLevelAndState(NULL) == BATTERRY_IS_CHARGING)
    {    
        cpu_s.event_control.init = 1;
    }  
    else
    {      
        cpu_s.oled.oled_light_status = 1;
        cpu_s.oled.oled_drive_off = 1;
    }
    Gui_ThreadSetDisPlayArea(0, 0, 80, 160);
    Gui_ThreadWakeUpThread();
}

/****************************************************************
** Function name:           Gui_ThreadPowerOn
** Descriptions:            �������洦��
** input parameters:        �� 
** output parameters:       �� 
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-08-12
*****************************************************************/ 
void Gui_ThreadPowerOn(void)
{       
    //UI�̳߳�ʼ�����.Ϊ1ʱ���Ƕ���ʽSPI����
    while(gui_res_s.sleep_flag)
    {
        GuiThread_SpiWeakUp();
    }
    gui_res_s.init_complete = 0;
    Gui_LcdDriverInit();
    gui_res_s.init_complete = 1;
    
	memset(&cpu_s, 0, sizeof(cpu_task_t));	
	cpu_task_controller_t *p_task = &cpu_s.job_control;
	//�������������   
    TASK_ENTRY_INIT(p_task, 3*4);
	cpu_s.event   = CPU_EVENT_TASK_IDEL;
	cpu_s.task    = CPU_TASK_MAIN;
	cpu_s.job_control.init = 1;	
    cpu_s.gui.clear_background = 1;
	cpu_s.oled.oled_drive_light = 1;
    cpu_s.gui.updata_ui_interval = 300;   //300ms    
    Gui_ThreadSetDisPlayArea(0, 0, 80, 160);
    Gui_ThreadWakeUpThread();
}

/****************************************************************
** Function name:           Gui_ThreadCanAcceptEvent
** Descriptions:            �¼����ȼ������жϵ�ǰ�¼��Ƿ���Ӧ
** input parameters:        �� 
** output parameters:       �� 
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-08-12
*****************************************************************/ 
uint8_t Gui_ThreadCanAcceptEvent(uint8_t event)
{
    uint8_t ret = 1;
    
    if(Gui_GetTraverseUiState() == 1)
    {
        /* UI�����У����������¼� */
        return 0;
    }
    
    if(event == cpu_s.event)
    {
        if(cpu_s.event == CPU_EVENT_TASK_CHARGE)
        {
//            TASK_ENTRY_INIT(cpu_s.event_control, 8);
            cpu_s.event_control.keep_time = 8;
            cpu_s.event_control.run_time = 0;
            //WJ.20171121.��������ȥ��磬
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
        /* �����ų���� ���������¼� */
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
    
    if(cpu_s.task == CPU_TASK_FINDPHONE && cpu_s.gui.animation_enable == 1)
    {
        return 0;
    }
    
    if(event == CPU_EVENT_TASK_CALL_REMIND)
    {
        //�����¼�������ִ��
        if(cpu_s.task != CPU_TASK_IDEL)
        {
            cpu_s.job_control.keep_time = 0;
            cpu_s.job_control.run_time = 1;
        }
        gui_movement_change_page = 0; 
        //WJ.20171010.����������ʱ���º�����ʱ�䲻������
        stop_watch_frist_flag = 0;
        
        /* �˳�����*/
        Gui_DataLongKeyToExitPhoto();
        
        return 1;
    }
    
    if(cpu_s.event == CPU_EVENT_TASK_CALL_REMIND)
    {
        /* ����������*/
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
            /* �˶�ģʽ������*/              
            ret = 0;
        }
        else if(cpu_s.task == CPU_TASK_HEART 
                && (event == CPU_EVENT_TASK_CAMERAREADY || event == CPU_EVENT_TASK_CAMERAOK))
        {
            /* ���ʲ�����*/
            ret = 0;
        }
        else if(cpu_s.task == CPU_TASK_BP
                && (event == CPU_EVENT_TASK_CAMERAREADY || event == CPU_EVENT_TASK_CAMERAOK))
        {
            /* Ѫѹ������*/
            ret = 0;
        }
        else if(cpu_s.task == CPU_TASK_STOP_WATCH && Gui_DataStopWatchIsBusy())
        {
            /* ���������*/
            ret = 0;
        }
        else if(cpu_s.event == CPU_EVENT_TASK_ALARM 
                && (event == CPU_EVENT_TASK_PHYS_REMIND || event == CPU_EVENT_TASK_MESSAGE_REMIND || event == CPU_EVENT_TASK_SIT))
        {
            /* ����������*/
            ret = 0;
        }
        else if(cpu_s.event == CPU_EVENT_TASK_SIT && event == CPU_EVENT_TASK_STARGET)
        {
            /* ����������*/
            ret = 0;
        }
        else if(cpu_s.event == CPU_EVENT_TASK_PHYS_REMIND)
        {
            /* Ů����Ϣ������*/
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
            /* ��Ϣ������*/
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
            /* ����ʱ��ʼ��*/
            ret = 0;
        }
        else if(cpu_s.event == CPU_EVENT_TASK_COUNT_DOWN_TIMEOUT
                && !(event == CPU_EVENT_TASK_CAMERAREADY || event == CPU_EVENT_TASK_CAMERAOK)
                && event!= CPU_EVENT_TASK_COUNT_DOWN_START)
        {
            /* ����ʱ��ʼ��*/
            ret = 0;
        }    
    }   
    return ret;   
}

/****************************************************************
** Function name:           Gui_ThreadUpdateStopwatch
** Descriptions:            ����������
** input parameters:        �� 
** output parameters:       �� 
** Returned value:          ��
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
** Descriptions:            ����������
** input parameters:        �� 
** output parameters:       �� 
** Returned value:          ��
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
** Descriptions:            �Ƿ��ڲ����ֻ�����
** input parameters:        �� 
** output parameters:       �� 
** Returned value:          1����  0������
** Created by:              YX              
** Created Date:            2017-08-12
*****************************************************************/ 
uint8_t Gui_ThreadIsFindPhone(void)
{
    return (cpu_s.task == CPU_TASK_FINDPHONE);
}

/****************************************************************
** Function name:           Gui_ThreadTaskToIdle
** Descriptions:            �������������
** input parameters:        �� 
** output parameters:       �� 
** Returned value:          1����  0������
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
** Descriptions:            ��ȡ��ǰ����ID
** input parameters:        �� 
** output parameters:       �� 
** Returned value:          ����ID
** Created by:              YX              
** Created Date:            2017-09-06
*****************************************************************/ 
uint8_t Gui_GetThreadTask(void)
{
    return cpu_s.task;
}

/****************************************************************
** Function name:           Gui_GetThreadJob
** Descriptions:            ��ȡ��ǰ�¼�ID
** input parameters:        �� 
** output parameters:       �� 
** Returned value:          �¼�ID
** Created by:              YX              
** Created Date:            2017-09-06
*****************************************************************/ 
uint8_t Gui_GetThreadJob(void)
{
    return cpu_s.event;
}

/****************************************************************
** Function name:           Gui_CheckEvent
** Descriptions:            UI�¼��Լ죬��ֹ�¼���������UI����ʾ
** input parameters:        �� 
** output parameters:       �� 
** Returned value:          ��
** Created by:              WJ             
** Created Date:            2018-01-30
*****************************************************************/ 
void Gui_CheckEvent(void)
{
    static uint8_t check_times_cnt = 0;	
    static uint8_t task_counter = 0xFF;
	
	//ע��,��������ÿ������
	if(task_counter == clock->seconds)
	{
        return;
    }
    
    task_counter = clock->seconds;
    //���ⲿ�¼������Լ�
    if(Gui_GetThreadJob() != CPU_EVENT_TASK_IDEL)
    {
        //WJ.20180205.�����߳�����״̬����
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
    
    //����ˢ�¼�������ֵ��10s
    if(cpu_s.gui.updata_ui_interval > 10000)
    {
        cpu_s.gui.updata_ui_interval = 1000;
    }
    
    //WJ.20180906.�����߳����������쳣�Լ촦��
    if(gui_res_s.init_complete)
    {
        if(gui_res_s.sleep_flag == 1)
        {
            gui_res_s.sleep_time++;
            if(gui_res_s.sleep_time >= 2)
            {
                GuiThread_SpiWeakUp();
            }
        }
    }
    
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
    if(gui_res_s.init_complete )
    {
        gui_res_s.sleep_flag = 1;
        xSemaphoreTake(gui_res_s.m_thread_sleep, portMAX_DELAY); 
        gui_res_s.sleep_flag = 0;
        gui_res_s.sleep_time = 0;
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
    
    if(gui_res_s.init_complete)
    {
        // The returned value may be safely ignored, if error is returned it only means that
        // the semaphore is already given (raised).
        UNUSED_VARIABLE(xSemaphoreGiveFromISR(gui_res_s.m_thread_sleep, &yield_req));
        portYIELD_FROM_ISR(yield_req);  
    }        
}

