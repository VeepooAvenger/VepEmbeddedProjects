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

uint8_t test_dis_ui_flg = 0;                                 //����UI�����ı�־ 0��δ����  1���ѿ���
static uint32_t traverse_tmp_buf1 = 0;                       //UI�����Ĳ���1
static uint32_t traverse_tmp_buf2 = 0;                       //UI�����Ĳ���2
static uint32_t traverse_tmp_buf3 = 0;                       //UI�����Ĳ���3
static uint32_t traverse_tmp_buf4 = 0;                       //UI�����Ĳ���4

static uint8_t evnet_interval_cnt = 0xFF;                    //���ļ�������������ж��Ƿ�ˢ��UI
static uint8_t task_interval_cnt = 0xFF;                     //���ļ�������������ж��Ƿ�ˢ��UI�Ͷ�ͼ�л�
static uint8_t event_msg_type = 0;

extern cpu_task_t cpu_s;
extern gui_thread_resource_t  gui_res_s;
extern uint8_t main_background_color;

/****************************************************************
** Function name:           Gui_SetTraverseUiState
** Descriptions:            ����UI����״̬
** input parameters:        state:״̬ 1:����UI����  0���ص�UI����
** output parameters:       ��
** Returned value:          ��
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
** Descriptions:            ��ȡUI����״̬
** input parameters:        state:״̬
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-08-02
*****************************************************************/
uint8_t Gui_GetTraverseUiState(void)
{	
	return test_dis_ui_flg;
}

/****************************************************************
** Function name:           Gui_EventAndTaskTraverse
** Descriptions:            �����л��������
** input parameters:        state:�����л���״̬
** output parameters:       �� 
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-08-02
*****************************************************************/
void Gui_EventAndTaskTraverse(uint8_t state)
{
    if(state == 1)
    {
        if(cpu_s.task > CPU_EVENT_TASK_IDEL && cpu_s.task < CPU_TASK_MOVEMENT_EXIT)
        {
            cpu_s.task++;
            cpu_s.job_control.init = 1;
            cpu_s.oled.oled_drive_light = 1;
            Gui_ThreadWakeUpThread();
        }
        else if(cpu_s.task == CPU_TASK_MOVEMENT_EXIT)
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
        if(cpu_s.event >= CPU_EVENT_TASK_CHARGE && cpu_s.event < CPU_EVENT_TASK_MAC_VER)
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
        else if(cpu_s.event == CPU_EVENT_TASK_MAC_VER)
        {
            Gui_SetTraverseUiState(0);
        }
    }
}


/****************************************************************
** Function name:           Cpu_EventLowPowerHintTest
** Descriptions:            �͵���ʾ
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-08-12
*****************************************************************/ 
void Cpu_EventLowPowerHintTest(cpu_task_controller_t *p_controller, gui_controller_t *p_gui)
{	
	if(p_controller->init == 1)
	{ 
        TASK_ENTRY_INIT(p_controller, 2); 
		//!ע��:�˴�����������Ϊ������ˢ�¶���,������ܻ���ʱ
        evnet_interval_cnt = 0xFF;
		p_gui->page_frame_counter = 0;
        
        //�������ͼ
        Gui_PageClearBackground(p_gui->start_column, p_gui->start_page, p_gui->column_size, p_gui->page_size);
	}
    
    //300ms��ˢ�¼��
    p_gui->updata_ui_interval = 300;
    
	if(evnet_interval_cnt != p_gui->page_frame_counter)
	{	
		Gui_ThreadSetDisPlayArea(10, 50, 70, 110);
        evnet_interval_cnt = p_gui->page_frame_counter;
        Gui_PageLowPower(evnet_interval_cnt);
    }
}

/****************************************************************
** Function name:           Cpu_EventIdelCharge
** Descriptions:            �������¼�����
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-08-12
*****************************************************************/ 
void Cpu_EventIdelChargeTest(cpu_task_controller_t *p_controller, gui_controller_t *p_gui)
{		
	if(p_controller->init == 1)
	{ 
        TASK_ENTRY_INIT(p_controller, 3); 
        
		//!ע��:�˴�����������Ϊ������ˢ�¶���,������ܻ���ʱ
        evnet_interval_cnt = 0xFF;
		p_gui->page_frame_counter = 0;
        
        //�������ͼ
        Gui_PageClearBackground(p_gui->start_column, p_gui->start_page, p_gui->column_size, p_gui->page_size);		
	}
    
    //300ms��ˢ�¼��
    p_gui->updata_ui_interval = 300; 

	if(evnet_interval_cnt != p_gui->page_frame_counter)
	{	
        evnet_interval_cnt = p_gui->page_frame_counter;	
		Gui_ThreadSetDisPlayArea(10, 50, 70, 110);
        Gui_PageBatteryCharg(evnet_interval_cnt, 0); 
    }
}

/****************************************************************
** Function name:           Cpu_EventAlarm
** Descriptions:            ���ӽ����¼�����
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
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
            TASK_ENTRY_INIT(p_controller, 10);
            traverse_tmp_buf1 = 0;
            traverse_tmp_buf2 = 0;
            traverse_tmp_buf3 = 1;
            traverse_tmp_buf4 = 0; 
		
		//!ע��:�˴�����������Ϊ������ˢ�¶���,������ܻ���ʱ
        evnet_interval_cnt = 0xFF;
		p_gui->page_frame_counter = 0;
        	
        //�������ͼ
        Gui_PageClearBackground(p_gui->start_column, p_gui->start_page, p_gui->column_size, p_gui->page_size);
	} 
        //499ms��ˢ�¼��
        p_gui->updata_ui_interval = 399; 
    
	if(evnet_interval_cnt != p_gui->page_frame_counter)
	{
        Gui_ThreadSetDisPlayArea(0, 0, 80, 160);
        evnet_interval_cnt = p_gui->page_frame_counter; 
            if(traverse_tmp_buf2++ < 5)
            {
                alarm_mode = 0;
            }
            else
            {
                alarm_mode = traverse_tmp_buf4++ % 20 + 1;
            }
            if(alarm_mode == 1)
            {
                //�������ͼ
                Gui_PageClearBackground(p_gui->start_column, p_gui->start_page, p_gui->column_size, p_gui->page_size);
            }
            alarm_hour = traverse_tmp_buf3 % 100;
            alarm_minute = traverse_tmp_buf3 % 100;
            traverse_tmp_buf3 += 22;
            display_language = traverse_tmp_buf1++ / 3;
            if(display_language > 1)
            {
                display_language = 1;
            }
            Gui_PageAlarm(traverse_tmp_buf1 % 3, alarm_mode, alarm_hour, alarm_minute, display_language, evnet_interval_cnt % 2);
 
	}
}

/****************************************************************
** Function name:           Cpu_EventSedentary
** Descriptions:            ���������¼�����
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-08-12
*****************************************************************/ 
void Cpu_EventSedentaryTest(cpu_task_controller_t *p_controller, gui_controller_t *p_gui)
{
	if(p_controller->init == 1)
	{ 
            TASK_ENTRY_INIT(p_controller, 1); 

		//!ע��:�˴�����������Ϊ������ˢ�¶���,������ܻ���ʱ
        evnet_interval_cnt = 0xFF;
		p_gui->page_frame_counter = 0;
		
        //�������ͼ
        Gui_PageClearBackground(p_gui->start_column, p_gui->start_page, p_gui->column_size, p_gui->page_size);
	}
	
    //300ms��ˢ�¼��
    p_gui->updata_ui_interval = 300;
    
	if(evnet_interval_cnt != p_gui->page_frame_counter)
	{
        Gui_ThreadSetDisPlayArea(10, 50, 70, 110);
        evnet_interval_cnt = p_gui->page_frame_counter;
		Gui_PageSit(evnet_interval_cnt);
	}
}

/****************************************************************
** Function name:           Cpu_EventMessageRemind
** Descriptions:            �������ѽ����¼�����
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
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
            //�������ͼ
            Gui_PageClearBackground(p_gui->start_column, p_gui->start_page, p_gui->column_size, p_gui->page_size);	 
 
		//!ע��:�˴�����������Ϊ������ˢ�¶���,������ܻ���ʱ
        evnet_interval_cnt = 0xFF;
		p_gui->page_frame_counter = 0;         
	} 
        //250ms��ˢ�¼��
        p_gui->updata_ui_interval = 790;
 
    
	if(evnet_interval_cnt != p_gui->page_frame_counter)
	{ 
        Gui_ThreadSetDisPlayArea(0, 0, 80, 160); 
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
            Gui_PageCallNotificationIcon(last_call_state, evnet_interval_cnt);
 
	}
}

/****************************************************************
** Function name:           Cpu_EventMessageRemind
** Descriptions:            ��Ϣ���ѽ����¼�����
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-08-12
*****************************************************************/ 
void Cpu_EventMessageRemindTest(cpu_task_controller_t *p_controller, gui_controller_t *p_gui)
{
	if(p_controller->init == 1)
	{ 
            TASK_ENTRY_INIT(p_controller, 8);
            traverse_tmp_buf1 = 0; 
        
		//!ע��:�˴�����������Ϊ������ˢ�¶���,������ܻ���ʱ
        evnet_interval_cnt = 0xFF;
		p_gui->page_frame_counter = 0;
        
        //�������ͼ
        Gui_PageClearBackground(p_gui->start_column, p_gui->start_page, p_gui->column_size, p_gui->page_size);	      
	} 
        //400ms��ˢ�¼��
        p_gui->updata_ui_interval = 399; 
    
	if(evnet_interval_cnt != p_gui->page_frame_counter)
	{
        Gui_ThreadSetDisPlayArea(0, 0, 80, 160); 
        evnet_interval_cnt = p_gui->page_frame_counter; 
            event_msg_type = traverse_tmp_buf1++ % 14 + 2;
            Gui_PageMessageNotificationIcon(event_msg_type, evnet_interval_cnt); 
	}
}

/****************************************************************
** Function name:           Cpu_EventHeartAlert
** Descriptions:            ����澯�����¼�����
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-08-12
*****************************************************************/ 
void Cpu_EventHeartAlertTest(cpu_task_controller_t *p_controller, gui_controller_t *p_gui)
{  
	if(p_controller->init == 1)
	{ 
            TASK_ENTRY_INIT(p_controller, 2); 
		//!ע��:�˴�����������Ϊ������ˢ�¶���,������ܻ���ʱ
        evnet_interval_cnt = 0xFF;
		p_gui->page_frame_counter = 0;
		
        //�������ͼ
        Gui_PageClearBackground(p_gui->start_column, p_gui->start_page, p_gui->column_size, p_gui->page_size);       
	}
	
    //200ms��ˢ�¼��
    p_gui->updata_ui_interval = 200;
    
	if(evnet_interval_cnt != p_gui->page_frame_counter)
	{
        Gui_ThreadSetDisPlayArea(10, 50, 70, 110);
        evnet_interval_cnt = p_gui->page_frame_counter;
		Gui_PageHeartAlert(evnet_interval_cnt);
	}
}

/****************************************************************
** Function name:           Cpu_EventFontTest
** Descriptions:            �������Խ����¼�����
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-08-12
*****************************************************************/ 
void Cpu_EventFontTestTest(cpu_task_controller_t *p_controller, gui_controller_t *p_gui)
{
    if(p_controller->init == 1)
	{ 
            TASK_ENTRY_INIT(p_controller, 1); 
		//!ע��:�˴�����������Ϊ������ˢ�¶���,������ܻ���ʱ
        evnet_interval_cnt = 0xFF;
		p_gui->page_frame_counter = 0;
		
        //�������ͼ
        Gui_PageClearBackground(p_gui->start_column, p_gui->start_page, p_gui->column_size, p_gui->page_size);
	}
	
    //300ms��ˢ�¼��
    p_gui->updata_ui_interval = 300;
    
	if(evnet_interval_cnt != p_gui->page_frame_counter)
	{
        Gui_ThreadSetDisPlayArea(0, 0, 80, 160);
        evnet_interval_cnt = p_gui->page_frame_counter;
		Gui_PageFontTest();
	}
}

/****************************************************************
** Function name:           Cpu_EventCameraReady
** Descriptions:            ������������¼�����
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-08-12
*****************************************************************/ 
void Cpu_EventCameraReadyTest(cpu_task_controller_t *p_controller, gui_controller_t *p_gui)
{
	if(p_controller->init == 1)
	{ 
            TASK_ENTRY_INIT(p_controller, 1);
            p_controller->key_press_hold = 0; 
		
		//!ע��:�˴�����������Ϊ������ˢ�¶���,������ܻ���ʱ
		evnet_interval_cnt = 0xFF;
		p_gui->page_frame_counter = 0;
		
        //�������ͼ
        Gui_PageClearBackground(p_gui->start_column, p_gui->start_page, p_gui->column_size, p_gui->page_size);
	}
	
    //300ms��ˢ�¼��
    p_gui->updata_ui_interval = 300;
    
	if(evnet_interval_cnt != p_gui->page_frame_counter)
	{
        Gui_ThreadSetDisPlayArea(10, 50, 70, 110);
        evnet_interval_cnt = p_gui->page_frame_counter;
		Gui_PageCameraReady(evnet_interval_cnt);
	}
}

/****************************************************************
** Function name:           Cpu_EventCameraOK
** Descriptions:            ������ɽ����¼�����
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-08-12
*****************************************************************/ 
void Cpu_EventCameraOKTest(cpu_task_controller_t *p_controller, gui_controller_t *p_gui)
{
	if(p_controller->init == 1)
	{
		TASK_ENTRY_INIT(p_controller, 1);
		 
		//!ע��:�˴�����������Ϊ������ˢ�¶���,������ܻ���ʱ
		evnet_interval_cnt = 0xFF;
		p_gui->page_frame_counter = 0;
		
		//�������ͼ
        Gui_PageClearBackground(p_gui->start_column, p_gui->start_page, p_gui->column_size, p_gui->page_size);
	}
	
    //300ms��ˢ�¼��
    p_gui->updata_ui_interval = 300;   
	if(evnet_interval_cnt != p_gui->page_frame_counter)
	{
        Gui_ThreadSetDisPlayArea(10, 50, 70, 110);
        evnet_interval_cnt = p_gui->page_frame_counter;
        Gui_PageCameraOk(evnet_interval_cnt);
	}
}

/****************************************************************
** Function name:           Cpu_EvenMacVer
** Descriptions:            ��ʾMAC��ַ�Ͱ汾��
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-08-12
*****************************************************************/ 
void Cpu_EvenDisMacVerTest(cpu_task_controller_t *p_controller, gui_controller_t *p_gui)
{
    uint8_t dis_mac_data[20] = {0};
    uint8_t dis_ver_data[10] = {0};
	if(p_controller->init == 1)
	{ 
            TASK_ENTRY_INIT(p_controller, 1);  
		//!ע��:�˴�����������Ϊ������ˢ�¶���,������ܻ���ʱ
		evnet_interval_cnt = 0xFF;
		p_gui->page_frame_counter = 0;
		Gui_DataGetBleMac(dis_mac_data);
        Gui_DataGetVer(dis_ver_data);
		//�������ͼ
        Gui_PageClearBackground(p_gui->start_column, p_gui->start_page, p_gui->column_size, p_gui->page_size);
	}
	
    //300ms��ˢ�¼��
    p_gui->updata_ui_interval = 300;   
	if(evnet_interval_cnt != p_gui->page_frame_counter)
	{
        Gui_ThreadSetDisPlayArea(0, 49, 80, 62);
        evnet_interval_cnt = p_gui->page_frame_counter;
        Gui_PageDisMacVer(evnet_interval_cnt, dis_mac_data, dis_ver_data);
	}
}

/****************************************************************
** Function name:           Cpu_EvenStarget
** Descriptions:            �˶�Ŀ�����ѽ���
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-08-12
*****************************************************************/ 
void Cpu_EvenStargetTest(cpu_task_controller_t *p_controller, gui_controller_t *p_gui)
{
	if(p_controller->init == 1)
	{ 
            TASK_ENTRY_INIT(p_controller, 1);  
		
		//!ע��:�˴�����������Ϊ������ˢ�¶���,������ܻ���ʱ
		evnet_interval_cnt = 0xFF;
		p_gui->page_frame_counter = 0;
        
        //�������ͼ
        Gui_PageClearBackground(p_gui->start_column, p_gui->start_page, p_gui->column_size, p_gui->page_size);
	}
	
    //300ms��ˢ�¼��
    p_gui->updata_ui_interval = 300;
    
	if(evnet_interval_cnt != p_gui->page_frame_counter)
	{
        Gui_ThreadSetDisPlayArea(10, 50, 70, 110);
        evnet_interval_cnt = p_gui->page_frame_counter;
        Gui_PageStarget(evnet_interval_cnt);
    }
}

/****************************************************************
** Function name:           Cpu_TaskMain
** Descriptions:            �����洦��
** input parameters:        p_controller��UI�¼�����״̬��p_gui����ʾ��صĲ���
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-08-12
*****************************************************************/ 
void Cpu_TaskMainTest(cpu_task_controller_t *p_controller, gui_controller_t *p_gui)
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
    uint8_t weather = 0;
    int8_t weather_t[6][2] = {{0, 0}, {1,9},{-10, 8},{13, 25},{-13, 30}, {-80,-60}};
    
	if(p_controller->init == 1)
	{
        /* UI������ */
        TASK_ENTRY_INIT(p_controller, 50);
        p_controller->key_press_hold = 0;
        traverse_tmp_buf1 = 0;
        traverse_tmp_buf2 = 0;
        traverse_tmp_buf3 = 0;
//            traverse_tmp_buf4 = 0;
        display_language = 0;
		
		//!ע��:�˴�����������Ϊ������ˢ�¶���,������ܻ���ʱ
        task_interval_cnt = 0xFF;
		p_gui->page_frame_counter = 0;
		Gui_PageClearBackground(p_gui->start_column, p_gui->start_page, p_gui->column_size, p_gui->page_size);
	}
 
        //480ms��ˢ�¼��
        p_gui->updata_ui_interval = 790;
   
	if(task_interval_cnt != p_gui->page_frame_counter)
	{
        Gui_ThreadSetDisPlayArea(0, 0, 80, 160);
        task_interval_cnt = p_gui->page_frame_counter;
 
            /* UI������ */
            minute = hour = traverse_tmp_buf1 % 60;
            traverse_tmp_buf1 += 11;
//            minute = traverse_tmp_buf2 % 60;
//            traverse_tmp_buf2 += 11;
            week = traverse_tmp_buf2++ % 7 + 1;
            time_format = traverse_tmp_buf2 % 2 + 1;
            ble_con_state = traverse_tmp_buf2 % 2;
            battery_charging = traverse_tmp_buf2 % 2;
            battery_level = traverse_tmp_buf2 % 5;
            if(traverse_tmp_buf2 >= 30)
            {
                display_language = 1;
            }
            month = day = traverse_tmp_buf3;
            traverse_tmp_buf3 += 11;
            if(task_interval_cnt % 4 == 1)
            {
                task_interval_cnt++;
            }

            if(task_interval_cnt == 4)
            {
               main_background_color = main_background_color++ % 7;
               Gui_PageClearBackground(0, 0, 80, 160);
               task_interval_cnt = 0;
               weather = 1;                
            }
            else if(task_interval_cnt < 4)
            {
               weather = 0; 
            }
            else
            {
                weather = (++weather) % 9;
                Gui_PageClearBackground(0, 122, 80, 22);
            }
            
            Gui_PageMainAssisant(task_interval_cnt, traverse_tmp_buf1 , ble_con_state, time_format, battery_charging, battery_level, display_language, 0);
            Gui_PageMainDateHour(month, day, hour, minute, week, display_language, time_format, weather, 
                                 weather_t[task_interval_cnt%7][0], weather_t[task_interval_cnt%7][1]);
	}
}

/****************************************************************
** Function name:           Cpu_TaskStep
** Descriptions:            �Ʋ����洦��
** input parameters:        p_controller��UI�¼�����״̬��p_gui����ʾ��صĲ���
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-08-12
*****************************************************************/ 
void Cpu_TaskStepTest(cpu_task_controller_t *p_controller, gui_controller_t *p_gui)
{	
    uint32_t all_step = 0;
    uint32_t all_step_buf[5] = {0, 12, 345, 6789, 12345};
    uint8_t  sports_mark_buf = 0;
//    uint32_t distance_m = 0;
//    float distance_km = 0.0f;
//    float distance_mile = 0.0f;
    
	if(p_controller->init == 1)
	{ 
            /* UI������ */
            TASK_ENTRY_INIT(p_controller, 4);
            traverse_tmp_buf1 = 0;
            display_language = 0; 
        p_controller->key_press_hold = 0;
        
		//!ע��:�˴�����������Ϊ������ˢ�¶���,������ܻ���ʱ
        task_interval_cnt = 0xFF;
		p_gui->page_frame_counter = 0;
		
		//�������ͼ		
		Gui_PageClearBackground(p_gui->start_column, p_gui->start_page, p_gui->column_size, p_gui->page_size);
	}
		 
        //480ms��ˢ�¼��
        p_gui->updata_ui_interval = 790; 
    
	if(task_interval_cnt != p_gui->page_frame_counter)
	{
        Gui_ThreadSetDisPlayArea(0, 0, 80, 160);
        task_interval_cnt = p_gui->page_frame_counter;  
            /* UI������ */
            all_step = all_step_buf[traverse_tmp_buf1++];
            display_language = traverse_tmp_buf1 % 2;
            sports_mark_buf = traverse_tmp_buf1 * 20;
            if(traverse_tmp_buf1 >= 5)
            {
                traverse_tmp_buf1 = 0;
            } 
	    Gui_PageStep(all_step, sports_mark_buf, task_interval_cnt, display_language);    
	}      
}

/****************************************************************
** Function name:           Cpu_TaskDistance
** Descriptions:            ������洦��
** input parameters:        p_controller��UI�¼�����״̬��p_gui����ʾ��صĲ���
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-08-12
*****************************************************************/ 
void Cpu_TaskDistanceTest(cpu_task_controller_t *p_controller, gui_controller_t *p_gui)
{	
    uint8_t distance_dis_unit = 0;
    float distance_km = 0.0f;
    float distance_mile = 0.0f;
    float all_step_buf[5] = {0, 1.2, 34, 567, 890};
    
	if(p_controller->init == 1)
	{ 
            /* UI������ */
            TASK_ENTRY_INIT(p_controller, 4);
            traverse_tmp_buf1 = 0;
            display_language = 0; 
		p_controller->key_press_hold = 0;
		
		//!ע��:�˴�����������Ϊ������ˢ�¶���,������ܻ���ʱ
        task_interval_cnt = 0xFF;
		p_gui->page_frame_counter = 0;
		
		//�������ͼ		
		Gui_PageClearBackground(p_gui->start_column, p_gui->start_page, p_gui->column_size, p_gui->page_size);
	} 
        //480ms��ˢ�¼��
        p_gui->updata_ui_interval = 790; 
    
	if(task_interval_cnt != p_gui->page_frame_counter)
	{
        Gui_ThreadSetDisPlayArea(0, 0, 80, 160);
        task_interval_cnt = p_gui->page_frame_counter; 
            /* UI������ */
            distance_km = distance_mile = all_step_buf[traverse_tmp_buf1];
            distance_dis_unit = traverse_tmp_buf1++ % 2;
            display_language = (traverse_tmp_buf1 == 1)?0 : 1;
            if(traverse_tmp_buf1 >= 5)
            {
                traverse_tmp_buf1 = 0;
            }
 
        if(distance_dis_unit == 1 || display_language == 0)
        {
            Gui_PageDistance(distance_km, task_interval_cnt, 0, display_language);
        }
        else
        {
            Gui_PageDistance(distance_mile, task_interval_cnt, 1, display_language);
        }
        
	}      
}

/****************************************************************
** Function name:           Cpu_TaskCalories
** Descriptions:            ��·����洦��
** input parameters:        p_controller��UI�¼�����״̬��p_gui����ʾ��صĲ���
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-08-12
*****************************************************************/ 
void Cpu_TaskCaloriesTest(cpu_task_controller_t *p_controller, gui_controller_t *p_gui)
{	   
    uint16_t calorie_tmp_buf[4] = {0, 12, 345, 6789};
	if(p_controller->init == 1)
	{ 
            /* UI������ */
            TASK_ENTRY_INIT(p_controller, 4);
            traverse_tmp_buf1 = 0;
            display_language = 0;
 
		p_controller->key_press_hold = 0;
		
		//!ע��:�˴�����������Ϊ������ˢ�¶���,������ܻ���ʱ
        task_interval_cnt = 0xFF;
		p_gui->page_frame_counter = 0;
		
		//�������ͼ		
		Gui_PageClearBackground(p_gui->start_column, p_gui->start_page, p_gui->column_size, p_gui->page_size);
	}
 
        //480ms��ˢ�¼��
        p_gui->updata_ui_interval = 790;
 
	if(task_interval_cnt != p_gui->page_frame_counter)
	{
        Gui_ThreadSetDisPlayArea(0, 0, 80, 160);
        task_interval_cnt = p_gui->page_frame_counter;
 
            /* UI������ */
            Gui_PageCalories(calorie_tmp_buf[traverse_tmp_buf1], task_interval_cnt, traverse_tmp_buf1 % 2);
            if(++traverse_tmp_buf1 >= 4)
            {
                traverse_tmp_buf1 = 0;
            } 
	}      
}

/****************************************************************
** Function name:           Cpu_TaskRun
** Descriptions:            �˶����洦��
** input parameters:        p_controller��UI�¼�����״̬��p_gui����ʾ��صĲ���
** output parameters:       ��
** Returned value:          ��
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

//    uint32_t tdistance_m = 0;
//    float    run_distance_tmp[5] = {0, 1.2, 34, 567, 890};
    uint32_t run_calories_tmp[5] = {0, 12, 345, 6789, 12345};
    uint32_t run_all_step_tmp[5] = {0, 12, 345, 6789, 12345};
    
	if(p_controller->init == 1)
	{ 
            /* UI������ */
            TASK_ENTRY_INIT(p_controller, 13);
            p_controller->key_press_hold = 0;
            traverse_tmp_buf1 = 0;
            traverse_tmp_buf2 = 0;
            traverse_tmp_buf3 = 0;
            traverse_tmp_buf4 = 0;
            display_language = 0; 

		//!ע��:�˴�����������Ϊ������ˢ�¶���,������ܻ���ʱ
        task_interval_cnt = 0xFF;
		p_gui->page_frame_counter = 0;
        last_state = 0;
		//�������ͼ		
		Gui_PageClearBackground(p_gui->start_column, p_gui->start_page, p_gui->column_size, p_gui->page_size);
	} 
 
        //480ms��ˢ�¼��
        p_gui->updata_ui_interval = 590;
 
	if(task_interval_cnt != p_gui->page_frame_counter)
	{
        Gui_ThreadSetDisPlayArea(0, 0, 80, 160);
        task_interval_cnt = p_gui->page_frame_counter; 
            if(traverse_tmp_buf1++ >= 12)
            {
                movement_mode_busy = 1;
                seconds = traverse_tmp_buf1;
                traverse_tmp_buf1 += 10 + 12*60 + 12*3600;
                heart = traverse_tmp_buf2;
                traverse_tmp_buf2 += 37;
//                tdistance_m = run_distance_tmp[traverse_tmp_buf3];
                calories = run_calories_tmp[traverse_tmp_buf3];
                all_step = run_all_step_tmp[traverse_tmp_buf3];
                if(++traverse_tmp_buf3 >= 5)
                {
                    traverse_tmp_buf3 = 0;
                }
            }
            

        if(movement_mode_busy == 1)
        {            
 
                if(last_state == 0)
                {
                    last_state = 1;
                }
                Gui_PageClearBackground(p_gui->start_column, p_gui->start_page, p_gui->column_size, p_gui->page_size);
 
            Gui_PageRunModeData(seconds, heart, distance_m, calories, all_step, uint_type, display_language);
            last_state = 1;
        }
        else            
        {
            if(last_state == 1)
            {
                last_state = 0;
                Gui_PageClearBackground(p_gui->start_column, p_gui->start_page, p_gui->column_size, p_gui->page_size);
            }
            Gui_PageRunMode(task_interval_cnt, display_language);
        }        
	}    
}

/****************************************************************
** Function name:           Cpu_TaskBP
** Descriptions:            Ѫѹ���洦��
** input parameters:        p_controller��UI�¼�����״̬��p_gui����ʾ��صĲ���
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-08-12
*****************************************************************/ 
void Cpu_TaskBPTest(cpu_task_controller_t *p_controller, gui_controller_t *p_gui)
{
    static uint8_t bp_sp = 0;
    static uint8_t bp_dp = 0;
    static uint8_t test_mode = 0;
    static uint8_t test_state = 0;
 
	if(p_controller->init == 1)
	{ 
            /* UI������ */
            TASK_ENTRY_INIT(p_controller, 5);
            traverse_tmp_buf1 = 0;
            traverse_tmp_buf2 = 0;
            traverse_tmp_buf3 = 0;
 
		p_controller->key_press_hold = 0;

		//!ע��:�˴�����������Ϊ������ˢ�¶���,������ܻ���ʱ
        task_interval_cnt = 0xFF;      
		p_gui->page_frame_counter = 0;

		//�������ͼ		
		Gui_PageClearBackground(p_gui->start_column, p_gui->start_page, p_gui->column_size, p_gui->page_size);
        bp_sp = 0;
        bp_dp = 0;
        test_mode = 0;
        test_state = 0;
	} 
 
        //480ms��ˢ�¼��
        p_gui->updata_ui_interval = 480;
    
	if(task_interval_cnt != p_gui->page_frame_counter)
	{
        Gui_ThreadSetDisPlayArea(0, 0, 80, 160);
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
        Gui_PageBP(task_interval_cnt, bp_sp, bp_dp, test_mode, test_state);
	} 
}

/****************************************************************
** Function name:           Cpu_TaskHeart
** Descriptions:            ���ʽ��洦��
** input parameters:        p_controller��UI�¼�����״̬��p_gui����ʾ��صĲ���
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-08-12
*****************************************************************/ 
void Cpu_TaskHeartTest(cpu_task_controller_t *p_controller, gui_controller_t *p_gui)
{
    static uint8_t heart = 0;
        
	if(p_controller->init == 1)
	{ 
            /* UI������ */
            TASK_ENTRY_INIT(p_controller, 5);
            traverse_tmp_buf1 = 1;
		
		p_controller->key_press_hold = 0;
		heart = 0;

		//!ע��:�˴�����������Ϊ������ˢ�¶���,������ܻ���ʱ
        task_interval_cnt = 0xFF;  
		p_gui->page_frame_counter = 0;

		//�������ͼ		
		Gui_PageClearBackground(p_gui->start_column, p_gui->start_page, p_gui->column_size, p_gui->page_size);
	} 
 
        //480ms��ˢ�¼��
        p_gui->updata_ui_interval = 480; 
    
	if(task_interval_cnt != p_gui->page_frame_counter)
	{
        Gui_ThreadSetDisPlayArea(0, 0, 80, 160);
        task_interval_cnt = p_gui->page_frame_counter; 
 
            heart = traverse_tmp_buf1;
            traverse_tmp_buf1 += 22;
 
		Gui_PageHeart(task_interval_cnt, heart);
	}
}

/****************************************************************
** Function name:           Cpu_TaskSleep
** Descriptions:            ˯�߽��洦��
** input parameters:        p_controller��UI�¼�����״̬��p_gui����ʾ��صĲ���
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-08-12
*****************************************************************/ 
void Cpu_TaskSleepTest(cpu_task_controller_t *p_controller, gui_controller_t *p_gui)
{ 
    
	if(p_controller->init == 1)
	{
            /* UI������ */
            TASK_ENTRY_INIT(p_controller, 5);
            p_controller->key_press_hold = 0;
            traverse_tmp_buf1 = 60;
            traverse_tmp_buf2 = 0;
 
		//!ע��:�˴�����������Ϊ������ˢ�¶���,������ܻ���ʱ
        task_interval_cnt = 0xFF;  
		p_gui->page_frame_counter = 0;
		
		//�������ͼ
		Gui_PageClearBackground(p_gui->start_column, p_gui->start_page, p_gui->column_size, p_gui->page_size);
	}	
 
        //480ms��ˢ�¼��
        p_gui->updata_ui_interval = 480;
    
 	if(task_interval_cnt != p_gui->page_frame_counter)
	{
        Gui_ThreadSetDisPlayArea(0, 0, 80, 160);
        task_interval_cnt = p_gui->page_frame_counter;
 
            Gui_PageSleep(0, traverse_tmp_buf1, traverse_tmp_buf2++ % 2);
            traverse_tmp_buf1 += 60 * 11 + 6 ;
 
	}
}

/****************************************************************
** Function name:           Cpu_TaskFindPhone
** Descriptions:            ���ֻ����洦��
** input parameters:        p_controller��UI�¼�����״̬��p_gui����ʾ��صĲ���
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-08-12
*****************************************************************/ 
void Cpu_TaskFindPhoneTest(cpu_task_controller_t *p_controller,gui_controller_t *p_gui)
{
    if(p_controller->init == 1)
	{
 
            /* UI������ */
            TASK_ENTRY_INIT(p_controller, 2);
            p_controller->key_press_hold = 0;
 
        //!ע��:�˴�����������Ϊ������ˢ�¶���,������ܻ���ʱ
        task_interval_cnt = 0xFF;
		p_gui->page_frame_counter = 0;
		
		//�������ͼ
		Gui_PageClearBackground(p_gui->start_column, p_gui->start_page, p_gui->column_size, p_gui->page_size);
	}
 
        //480ms��ˢ�¼��
        p_gui->updata_ui_interval = 480;
 
	if(task_interval_cnt != p_gui->page_frame_counter)
	{
        Gui_ThreadSetDisPlayArea(10, 50, 70, 110);
        task_interval_cnt = p_gui->page_frame_counter;
 
            Gui_PageFindPhone(1, task_interval_cnt);
 
	} 
}

/****************************************************************
** Function name:           Cpu_TaskPowerOff
** Descriptions:            �ػ����洦��
** input parameters:        p_controller��UI�¼�����״̬��p_gui����ʾ��صĲ���
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-08-12
*****************************************************************/ 
void Cpu_TaskPowerOffTest(cpu_task_controller_t *p_controller, gui_controller_t *p_gui)
{   
	if(p_controller->init == 1)
	{
 
            TASK_ENTRY_INIT(p_controller, 1);
            p_controller->key_press_hold = 0;
 
		//!ע��:�˴�����������Ϊ������ˢ�¶���,������ܻ���ʱ
        task_interval_cnt = 0xFF;  
		p_gui->page_frame_counter = 0;
		
		//�������ͼ
		Gui_PageClearBackground(p_gui->start_column, p_gui->start_page, p_gui->column_size, p_gui->page_size);
	}
		
    //1000ms��ˢ�¼��
    p_gui->updata_ui_interval = 300;
    
	if(task_interval_cnt != p_gui->page_frame_counter)
	{
        Gui_ThreadSetDisPlayArea(10, 50, 70, 110);
        task_interval_cnt = p_gui->page_frame_counter;
		Gui_PagePower();        
	} 
}

/****************************************************************
** Function name:           Cpu_TaskStopWatch
** Descriptions:            �����洦��
** input parameters:        p_controller��UI�¼�����״̬��p_gui����ʾ��صĲ���
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-08-12
*****************************************************************/ 
void Cpu_TaskStopWatchTest(cpu_task_controller_t *p_controller, gui_controller_t *p_gui)
{
    static uint8_t last_state = 0;
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
 	
		//!ע��:�˴�����������Ϊ������ˢ�¶���,������ܻ���ʱ
        task_interval_cnt = 0xFF;  
		p_gui->page_frame_counter = 0;
        
        //�������ͼ
		Gui_PageClearBackground(p_gui->start_column, p_gui->start_page, p_gui->column_size, p_gui->page_size);   
	}
 
        //300ms��ˢ�¼��
        p_gui->updata_ui_interval = 790;
 
	if(task_interval_cnt != p_gui->page_frame_counter)
	{
        Gui_ThreadSetDisPlayArea(0, 0, 80, 160);
        task_interval_cnt = p_gui->page_frame_counter;
 
            last_state = 0;
            traverse_tmp_buf1++;
            if(traverse_tmp_buf1 > 1)
            {
                last_state = 1;
                millisecond = traverse_tmp_buf2++;
                total_second = traverse_tmp_buf3;
                traverse_tmp_buf3 += 22 + 11*60 ;
                if(traverse_tmp_buf4++ == 0)
                {
                    //�������ͼ
                    Gui_PageClearBackground(p_gui->start_column, p_gui->start_page, p_gui->column_size, p_gui->page_size); 
                    task_interval_cnt = 0;
                }
            }
 
		GUI_StopWatchPage(last_state, total_second, millisecond, task_interval_cnt);
	} 
}

/****************************************************************
** Function name:           Gui_TaskCountDown
** Descriptions:            ����ʱ���洦��
** input parameters:        p_controller��UI�¼�����״̬��p_gui����ʾ��صĲ���
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-08-12
*****************************************************************/ 
void Gui_TaskCountDownTest(cpu_task_controller_t *p_controller, gui_controller_t *p_gui)
{
	if(p_controller->init == 1)
	{
            TASK_ENTRY_INIT(p_controller, 2);
            p_controller->key_press_hold = 0;
            traverse_tmp_buf1 = 0;
 
        //!ע��:�˴�����������Ϊ������ˢ�¶���,������ܻ���ʱ
        task_interval_cnt = 0xFF;  
		p_gui->page_frame_counter = 0;
        
		//�������ͼ
		Gui_PageClearBackground(p_gui->start_column, p_gui->start_page, p_gui->column_size, p_gui->page_size);
	}
 
        //480ms��ˢ�¼��
        p_gui->updata_ui_interval = 390;
 
	if(task_interval_cnt != p_gui->page_frame_counter)
	{
        Gui_ThreadSetDisPlayArea(10, 50, 70, 110);
        task_interval_cnt = p_gui->page_frame_counter;
 
            Gui_PageCountDown(1, task_interval_cnt);
 
	} 
}



/****************************************************************
** Function name:           Cpu_RunModePause
** Descriptions:            �˶���ͣ�����¼�����
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-08-12
*****************************************************************/ 
void Cpu_TaskRunModePauseTest(cpu_task_controller_t *p_controller, gui_controller_t *p_gui)
{
	if(p_controller->init == 1)
	{
 
            TASK_ENTRY_INIT(p_controller, 1);
            p_controller->key_press_hold = 0;
            traverse_tmp_buf1 = 0;
 
		//!ע��:�˴�����������Ϊ������ˢ�¶���,������ܻ���ʱ
		task_interval_cnt = 0xFF;
		p_gui->page_frame_counter = 0;
		
		//�������ͼ
        Gui_PageClearBackground(p_gui->start_column, p_gui->start_page, p_gui->column_size, p_gui->page_size);
	}
 
        //480ms��ˢ�¼��
        p_gui->updata_ui_interval = 480;
 
	if(task_interval_cnt != p_gui->page_frame_counter)
	{
        Gui_ThreadSetDisPlayArea(10, 50, 70, 110);
        task_interval_cnt = p_gui->page_frame_counter;
 
        Gui_PageRunModePause(traverse_tmp_buf1++ % 2, display_language);
 
	}
}

/****************************************************************
** Function name:           Cpu_EventRunModeExit
** Descriptions:            �˶��˳������¼�����
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-08-12
*****************************************************************/ 
void Cpu_TaskRunModeExitTest(cpu_task_controller_t *p_controller, gui_controller_t *p_gui)
{
	if(p_controller->init == 1)
	{
 
            TASK_ENTRY_INIT(p_controller, 1);
            p_controller->key_press_hold = 0;
  
		//!ע��:�˴�����������Ϊ������ˢ�¶���,������ܻ���ʱ
		task_interval_cnt = 0xFF;
		p_gui->page_frame_counter = 0;
		//�������ͼ
        Gui_PageClearBackground(p_gui->start_column, p_gui->start_page, p_gui->column_size, p_gui->page_size);
	}
	
    //500ms��ˢ�¼��
    p_gui->updata_ui_interval = 300;
    
	if(task_interval_cnt != p_gui->page_frame_counter)
	{
        Gui_ThreadSetDisPlayArea(10, 50, 70, 110);
        task_interval_cnt = p_gui->page_frame_counter;
        Gui_PageRunModeExit(display_language);
	}
}

/****************************************************************
** Function name:           Cpu_EvenBleDiscRemindTest
** Descriptions:            ��������
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-08-12
*****************************************************************/ 
void Cpu_EvenBleDiscRemindTest(cpu_task_controller_t *p_controller, gui_controller_t *p_gui)
{
	if(p_controller->init == 1)
	{
 
            TASK_ENTRY_INIT(p_controller, 1);
            p_controller->key_press_hold = 0;
  
		//!ע��:�˴�����������Ϊ������ˢ�¶���,������ܻ���ʱ
		task_interval_cnt = 0xFF;
		p_gui->page_frame_counter = 0;
		//�������ͼ
        Gui_PageClearBackground(p_gui->start_column, p_gui->start_page, p_gui->column_size, p_gui->page_size);
	}
	
    //500ms��ˢ�¼��
    p_gui->updata_ui_interval = 300;
    
	if(task_interval_cnt != p_gui->page_frame_counter)
	{
        Gui_ThreadSetDisPlayArea(10, 50, 60, 60);
        task_interval_cnt = p_gui->page_frame_counter;
        Gui_PageBleDiscRemind(0);
	}
}

/****************************************************************
** Function name:           Gui_ThreadTestManage
** Descriptions:            �������¼����洦��
** input parameters:        ��
** output parameters:       �� 
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-08-02
*****************************************************************/ 
void Gui_ThreadTestManage(void)
{
    switch(cpu_s.task)
	{
		case CPU_TASK_MAIN:     
            //������
			Cpu_TaskMainTest(&cpu_s.job_control, &cpu_s.gui);
			break;
		case CPU_TASK_STEP:     
            //�Ʋ�
			Cpu_TaskStepTest(&cpu_s.job_control, &cpu_s.gui);
			break;       
        case CPU_TASK_DISTANCE:       
            //����
			Cpu_TaskDistanceTest(&cpu_s.job_control, &cpu_s.gui);
            break;
        case CPU_TASK_MOVEMENT:
            //�˶�ģʽ
			Cpu_TaskRunTest(&cpu_s.job_control, &cpu_s.gui);
            break;
        case CPU_TASK_CALORIES:   
            //��·��
			Cpu_TaskCaloriesTest(&cpu_s.job_control, &cpu_s.gui);         
            break;
        case CPU_TASK_BP:       
            //Ѫѹ
            Cpu_TaskBPTest(&cpu_s.job_control, &cpu_s.gui);
            break;
		case CPU_TASK_HEART:    
            //����
			Cpu_TaskHeartTest(&cpu_s.job_control, &cpu_s.gui);
			break;
		case CPU_TASK_SLEEP:    
            //˯��
			Cpu_TaskSleepTest(&cpu_s.job_control, &cpu_s.gui);
			break;
		case CPU_TASK_FINDPHONE:     
            //�����ֻ�
			Cpu_TaskFindPhoneTest(&cpu_s.job_control, &cpu_s.gui);
			break;
		case CPU_TASK_STOP_WATCH:    
            //���
			Cpu_TaskStopWatchTest(&cpu_s.job_control, &cpu_s.gui);
			break;
		case CPU_TASK_COUNT_DOWN:    
            //����ʱ
			Gui_TaskCountDownTest(&cpu_s.job_control, &cpu_s.gui);
			break;
		case CPU_TASK_POWEROFF:                  
            //�ػ�
			Cpu_TaskPowerOffTest(&cpu_s.job_control, &cpu_s.gui);
			break;
		
        case CPU_TASK_MOVEMENT_PAUSE:   
            // �˶�ģʽ��ͣ
            Cpu_TaskRunModePauseTest(&cpu_s.job_control, &cpu_s.gui);
            break;  
   
        case CPU_TASK_MOVEMENT_EXIT: 
            // �˶�ģʽ�˳�
            Cpu_TaskRunModeExitTest(&cpu_s.job_control, &cpu_s.gui);
            break;        
        
		default:
//            Motor_DelEvent();
			cpu_s.task = CPU_TASK_IDEL;
			break;
	}
    
    switch(cpu_s.event)
	{
		
		case CPU_EVENT_TASK_CHARGE:
            //���
			Cpu_EventIdelChargeTest(&cpu_s.event_control, &cpu_s.gui);
			break;
        
		case CPU_EVENT_TASK_ALARM:     
            //����
			Cpu_EventAlarmTest(&cpu_s.event_control, &cpu_s.gui);
			break;
		
		case CPU_EVENT_TASK_SIT:        
            //����
			Cpu_EventSedentaryTest(&cpu_s.event_control, &cpu_s.gui);
			break;
        
		case CPU_EVENT_TASK_CALL_REMIND:
            //����
			Cpu_EventCallRemindTest(&cpu_s.event_control, &cpu_s.gui);
            break;
        
		case CPU_EVENT_TASK_MESSAGE_REMIND:    
            //��Ϣ
			Cpu_EventMessageRemindTest(&cpu_s.event_control, &cpu_s.gui);
			break;
		
		case CPU_EVENT_TASK_HEARTALERT: 
            //����
			Cpu_EventHeartAlertTest(&cpu_s.event_control, &cpu_s.gui);
			break;
		
		case CPU_EVENT_TASK_CAMERAREADY:    
            //����
			Cpu_EventCameraReadyTest(&cpu_s.event_control, &cpu_s.gui);
			break;
		
		case CPU_EVENT_TASK_CAMERAOK:       
            //�������
			Cpu_EventCameraOKTest(&cpu_s.event_control, &cpu_s.gui);
			break;
		 
		case CPU_EVENT_TASK_FONTTEST:       
            //����
			Cpu_EventFontTestTest(&cpu_s.event_control, &cpu_s.gui);
			break;
		
		case CPU_EVENT_TASK_LOWPOWER:      
            //�͵�
			Cpu_EventLowPowerHintTest(&cpu_s.event_control, &cpu_s.gui);
			break;
        
        case CPU_EVENT_TASK_STARGET:
            Cpu_EvenStargetTest(&cpu_s.event_control, &cpu_s.gui);
            break;
        
        case CPU_EVENT_TASK_MAC_VER:
            //��ʾmac��ver
            Cpu_EvenDisMacVerTest(&cpu_s.event_control, &cpu_s.gui);
            break;
        case CPU_EVENT_TASK_BLE_DISC_REMIND:
            Cpu_EvenBleDiscRemindTest(&cpu_s.event_control, &cpu_s.gui);
            break;

		default:
			cpu_s.event = CPU_EVENT_TASK_IDEL;
			break;
	}
}


