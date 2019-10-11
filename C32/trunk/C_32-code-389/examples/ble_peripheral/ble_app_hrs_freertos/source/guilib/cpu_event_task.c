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


static uint8_t evnet_interval_cnt = 0;   //���ļ�������������ж��Ƿ�ˢ��UI
static uint8_t event_msg_type = 0;


extern cpu_task_t cpu_s;

/****************************************************************
** Function name:           Cpu_TaskClearIntervalCnt
** Descriptions:            //����ˢ�¼��������
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-08-12
*****************************************************************/ 
void Cpu_EventClearIntervalCnt(void)
{
    evnet_interval_cnt = 0xFF;
}

/****************************************************************
** Function name:           Cpu_EventLowPowerHint
** Descriptions:            �͵���ʾ����
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
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
       
		//!ע��:�˴�����������Ϊ������ˢ�¶���,������ܻ���ʱ
        evnet_interval_cnt = 0xFF;
		p_gui->page_frame_counter = 0;
        
        //�������ͼ
        Gui_PageClearBackground(p_gui->start_column, p_gui->start_page, p_gui->column_size, p_gui->page_size);
	}
    
    //300ms��ˢ�¼��
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
** Descriptions:            �������¼�����
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-08-12
*****************************************************************/ 
void Cpu_EventIdelCharge(cpu_task_controller_t *p_controller, gui_controller_t *p_gui)
{		
	uint8_t charge_state = Gui_DataGetBatteryLevelAndState(NULL);
    
	if(p_controller->init == 1)
	{
        TASK_ENTRY_INIT(p_controller, 8);
        
		//!ע��:�˴�����������Ϊ������ˢ�¶���,������ܻ���ʱ
        evnet_interval_cnt = 0xFF;
		p_gui->page_frame_counter = 0;
        
        
        //�������
        Gui_LcdLedOff();
        //�������ͼ
        Gui_PageClearBackground(p_gui->start_column, p_gui->start_page, p_gui->column_size, p_gui->page_size);		
	}
    
    
    if(p_gui->page_frame_counter == 0)
    {
        Gui_LcdLedOff();
    }   
    
    
    //300ms��ˢ�¼��
    p_gui->updata_ui_interval = 300;

    // ������״̬�ı�,�������ص���ʾ���״̬
    if(charge_state == BATTERRY_IS_NORMAL)
    {
        p_controller->keep_time = 0;
        p_controller->run_time = 1;
    }
//    else
//    {
//        if(!Gui_DataSystemIsPoweOn())
//        {
//            /*�ػ�������������*/    
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
        // ������״̬����ʱ,����ˢ��
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
** Descriptions:            ���ӽ����¼�����
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
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

        //����10�� ÿ����1�� ���1��
        Motor_AddEvent(10, 1, 1 * 20, 2 * 20);
		
		//!ע��:�˴�����������Ϊ������ˢ�¶���,������ܻ���ʱ
        evnet_interval_cnt = 0xFF;
		p_gui->page_frame_counter = 0;
        

         //�������
         Gui_LcdLedOff();
        //�������ͼ
        Gui_PageClearBackground(p_gui->start_column, p_gui->start_page, p_gui->column_size, p_gui->page_size);
	}
    
    
    
    if(p_gui->page_frame_counter == 0)
    {
        Gui_LcdLedOff();
    }   
   
    
    
    //250ms��ˢ�¼��
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
                //YX.20170921.�޸�����ʱ����ʾ����
                alarm_hour = 12;
            }
        }
        Gui_PageAlarm(Gui_DataTimeFormatDisplay(), alarm_mode, alarm_hour, alarm_minute, display_language, evnet_interval_cnt);
	}
    
     Gui_LcdLedOn();
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
void Cpu_EventSedentary(cpu_task_controller_t *p_controller, gui_controller_t *p_gui)
{
	if(p_controller->init == 1)
	{
        TASK_ENTRY_INIT(p_controller, 6);
        //�������� ÿ����3�� ���2��
        Motor_AddEvent(2, 1, 1 * 20, 2 * 20); 

		//!ע��:�˴�����������Ϊ������ˢ�¶���,������ܻ���ʱ
        evnet_interval_cnt = 0xFF;
		p_gui->page_frame_counter = 0;
		
        
        //�������
        Gui_LcdLedOff();
        //�������ͼ
        Gui_PageClearBackground(p_gui->start_column, p_gui->start_page, p_gui->column_size, p_gui->page_size);
	}
	
    
    if(p_gui->page_frame_counter == 0)
    {
        Gui_LcdLedOff();
    }   
    
    
    //300ms��ˢ�¼��
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
** Descriptions:            �������ѽ����¼�����
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
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
        //������Ҫ���ദ��
        event_msg_type = Gui_DataPhoneNotifyDeal();      
        
        if(event_msg_type == GUI_MSG_TYPE_CALL)
        {
            if(Gui_DataMessageCallState() == GUI_CALL_REJECTSUC)
            {               
                TASK_ENTRY_INIT(p_controller, 3);	
                //�������
                Gui_LcdLedOff();
                //�������ͼ
//                  Gui_PageClearBackground(0, 0, 80, 100);
            }
            else if(Gui_DataMessageCallState() == GUI_CALL_INCOMING || Gui_DataMessageCallState() == GUI_CALL_MUTE)
            {
                TASK_ENTRY_INIT(p_controller, 60);
                //�������
                Gui_LcdLedOff();
                //�������ͼ
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
		//!ע��:�˴�����������Ϊ������ˢ�¶���,������ܻ���ʱ
        evnet_interval_cnt = 0xFF;
		p_gui->page_frame_counter = 0;
        last_page_index = 0xFF;          
	}
    
    
    
    if(p_gui->page_frame_counter == 0)
    {
        Gui_LcdLedOff();
    }   
    
    
    
    
    
    //250ms��ˢ�¼��
    p_gui->updata_ui_interval = 200;
    
	if(evnet_interval_cnt != p_gui->page_frame_counter)
	{ 
        Gui_ThreadSetDisPlayArea(0, 0, 240, 240); 
        evnet_interval_cnt = p_gui->page_frame_counter;       
        
        display_buf = Gui_DataGetPhoneNotifyData(&buf_len, &page_index);

        //������Ҫ���ദ��
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
** Descriptions:            ��Ϣ���ѽ����¼�����
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
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
        //������Ҫ���ദ��
        event_msg_type = Gui_DataPhoneNotifyDeal();
        
        if(event_msg_type >= GUI_MSG_TYPE_MESSAGE && event_msg_type <= GUI_MSG_TYPE_OTHER)
        {
            TASK_ENTRY_INIT(p_controller, 6);//5);	
        }
        else
        {
            TASK_ENTRY_INIT(p_controller, 1);	
        }
        
		//!ע��:�˴�����������Ϊ������ˢ�¶���,������ܻ���ʱ
        evnet_interval_cnt = 0xFF;
		p_gui->page_frame_counter = 0;
        last_page_index = 0xFF;
        
        
        //�������
        Gui_LcdLedOff();
        //�������ͼ
        Gui_PageClearBackground(p_gui->start_column, p_gui->start_page, p_gui->column_size, p_gui->page_size);	      
	}

    
    if(p_gui->page_frame_counter == 0)
    {
        Gui_LcdLedOff();
    }   
    
    
    //400ms��ˢ�¼��
    p_gui->updata_ui_interval = 400;
    
	if(evnet_interval_cnt != p_gui->page_frame_counter)
	{
        Gui_ThreadSetDisPlayArea(0, 0, 240, 240); 
        evnet_interval_cnt = p_gui->page_frame_counter;       

        display_buf = Gui_DataGetPhoneNotifyData(&buf_len, &page_index);
        
        //������Ҫ���ദ��
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
** Descriptions:            ����澯�����¼�����
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-08-12
*****************************************************************/ 
void Cpu_EventHeartAlert(cpu_task_controller_t *p_controller, gui_controller_t *p_gui)
{  
//	if(p_controller->init == 1)
//	{
//        TASK_ENTRY_INIT(p_controller, 8);
//        //����2�� ÿ����3�� ���2��
//        Motor_AddEvent(2, 1, 3, 2);
//        
//		//!ע��:�˴�����������Ϊ������ˢ�¶���,������ܻ���ʱ
//        evnet_interval_cnt = 0xFF;
//		p_gui->page_frame_counter = 0;
//		
//        //�������ͼ
//        Gui_PageClearBackground(p_gui->start_column, p_gui->start_page, p_gui->column_size, p_gui->page_size);       
//	}
//	
//    //200ms��ˢ�¼��
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
** Descriptions:            �������Խ����¼�����
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-08-12
*****************************************************************/ 
void Cpu_EventFontTest(cpu_task_controller_t *p_controller, gui_controller_t *p_gui)
{
    if(p_controller->init == 1)
	{
        TASK_ENTRY_INIT(p_controller, 60);

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
        Gui_ThreadSetDisPlayArea(0, 0, 240, 240);
        evnet_interval_cnt = p_gui->page_frame_counter;
		Gui_PageFontTest();
	}
}

/****************************************************************
** Function name:           Cpu_EvenCountDownTimeOut
** Descriptions:            ����ʱ��ʱ�����¼�����
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-08-12
*****************************************************************/ 
void Cpu_EvenCountDownTimeOut(cpu_task_controller_t *p_controller, gui_controller_t *p_gui)
{
//	if(p_controller->init == 1)
//	{
//		TASK_ENTRY_INIT(p_controller, 6);
//		
//		//!ע��:�˴�����������Ϊ������ˢ�¶���,������ܻ���ʱ
//		evnet_interval_cnt = 0xFF;
//		p_gui->page_frame_counter = 0;
//		
//		Motor_AddEvent(3, 1, 1 * 20, 1 * 20);
//		
//        //�������ͼ
//        Gui_PageClearBackground(p_gui->start_column, p_gui->start_page, p_gui->column_size, p_gui->page_size);
//	}
//	
//    //�������
//    Gui_LcdLedOff();
//    if(p_gui->page_frame_counter == 0)
//    {
//        Gui_LcdLedOff();
//    }   
//    Gui_LcdLedOn();
//    
//    //300ms��ˢ�¼��
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
** Descriptions:            ����ʱ��ʼ�����¼�����
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-08-12
*****************************************************************/ 
void Cpu_EvenCountDownTimeStart(cpu_task_controller_t *p_controller, gui_controller_t *p_gui)
{
//	if(p_controller->init == 1)
//	{
//		TASK_ENTRY_INIT(p_controller, 3);
//		
//		//!ע��:�˴�����������Ϊ������ˢ�¶���,������ܻ���ʱ
//		evnet_interval_cnt = 0xFF;
//		p_gui->page_frame_counter = 0;
//		
//        //�������ͼ
//        Gui_PageClearBackground(p_gui->start_column, p_gui->start_page, p_gui->column_size, p_gui->page_size);
//	}
//	
//    //300ms��ˢ�¼��
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
** Descriptions:            ������������¼�����
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-08-12
*****************************************************************/ 
void Cpu_EventCameraReady(cpu_task_controller_t *p_controller, gui_controller_t *p_gui)
{
	if(p_controller->init == 1)
	{
        TASK_ENTRY_INIT(p_controller, 60);
        p_controller->key_press_hold = 1;  
		
		//!ע��:�˴�����������Ϊ������ˢ�¶���,������ܻ���ʱ
		evnet_interval_cnt = 0xFF;
		p_gui->page_frame_counter = 0;
		
        //�������
    Gui_LcdLedOff();
        //�������ͼ
        Gui_PageClearBackground(p_gui->start_column, p_gui->start_page, p_gui->column_size, p_gui->page_size);
	}
	
    //�������
    Gui_LcdLedOff();
    if(p_gui->page_frame_counter == 0)
    {
        Gui_LcdLedOff();
    }   
    
    
    //300ms��ˢ�¼��
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
** Descriptions:            ������ɽ����¼�����
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-08-12
*****************************************************************/ 
void Cpu_EventCameraOK(cpu_task_controller_t *p_controller, gui_controller_t *p_gui)
{
	if(p_controller->init == 1)
	{
		TASK_ENTRY_INIT(p_controller, 6);
		 
		//!ע��:�˴�����������Ϊ������ˢ�¶���,������ܻ���ʱ
		evnet_interval_cnt = 0xFF;
		p_gui->page_frame_counter = 0;
		
        //�������
    Gui_LcdLedOff();
        
		//�������ͼ
        Gui_PageClearBackground(p_gui->start_column, p_gui->start_page, p_gui->column_size, p_gui->page_size);
	}
	
  
    if(p_gui->page_frame_counter == 0)
    {
        Gui_LcdLedOff();
    }   
   
    
    //300ms��ˢ�¼��
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
** Descriptions:            ��ʾMAC��ַ�Ͱ汾��
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
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
		 
		//!ע��:�˴�����������Ϊ������ˢ�¶���,������ܻ���ʱ
		evnet_interval_cnt = 0xFF;
		p_gui->page_frame_counter = 0;
		Gui_DataGetBleMac(dis_mac_data);
        Gui_DataGetVer(dis_ver_data);
        
        
        //�������
    Gui_LcdLedOff();
		//�������ͼ
        Gui_PageClearBackground(p_gui->start_column, p_gui->start_page, p_gui->column_size, p_gui->page_size);
	}
	
    
    
    if(p_gui->page_frame_counter == 0)
    {
        Gui_LcdLedOff();
    }   
  
    
    //300ms��ˢ�¼��
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
** Descriptions:            �˶�Ŀ�����ѽ���
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-08-12
*****************************************************************/ 
void Cpu_EvenStarget(cpu_task_controller_t *p_controller, gui_controller_t *p_gui)
{
	if(p_controller->init == 1)
	{ 
        TASK_ENTRY_INIT(p_controller, 6); 
        Motor_AddEvent(2, 1, 1 * 20, 2 * 20);  
		
		//!ע��:�˴�����������Ϊ������ˢ�¶���,������ܻ���ʱ
		evnet_interval_cnt = 0xFF;
		p_gui->page_frame_counter = 0;
        
        
        //�������
    Gui_LcdLedOff();
        //�������ͼ
        Gui_PageClearBackground(p_gui->start_column, p_gui->start_page, p_gui->column_size, p_gui->page_size);
	}
	
    
    
    if(p_gui->page_frame_counter == 0)
    {
        Gui_LcdLedOff();
    }   
   
    
    //300ms��ˢ�¼��
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
** Descriptions:            Ů�Թ�������
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
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
        //������Ҫ���ദ��
        TASK_ENTRY_INIT(p_controller,  6);	
        
        if(Gui_DataIsReadPhysMessage() != 1)
        {
            //��ȡ����Ҫ�����
            Motor_AddEvent(1, 1, 0.5 * 20, 1 * 20);  
        }
        
		//!ע��:�˴�����������Ϊ������ˢ�¶���,������ܻ���ʱ
        evnet_interval_cnt = 0xFF;
		p_gui->page_frame_counter = 0;
        last_page_index = 0xFF;
        
        
        //�������
    Gui_LcdLedOff();
        //�������ͼ
        Gui_PageClearBackground(p_gui->start_column, p_gui->start_page, p_gui->column_size, p_gui->page_size);	      
	}
    
    
    
    if(p_gui->page_frame_counter == 0)
    {
        Gui_LcdLedOff();
    }   
    

    //400ms��ˢ�¼��
    p_gui->updata_ui_interval = 400;
    
	if(evnet_interval_cnt != p_gui->page_frame_counter)
	{
        Gui_ThreadSetDisPlayArea(0, 0, 240, 240); 
        evnet_interval_cnt = p_gui->page_frame_counter;       
 
        display_buf = Gui_DataGetPhysRmindData(&buf_len, &page_index);       
        
        if(buf_len == 0)
        {
            //WJ.20180119.���Ů�Թ��ܲ鿴ʱ�رոù��ܣ�����ֻ��ʾUI������
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
** Descriptions:            ������������
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
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
        
		//!ע��:�˴�����������Ϊ������ˢ�¶���,������ܻ���ʱ
		evnet_interval_cnt = 0xFF;
		p_gui->page_frame_counter = 0;
        
        //�������
        Gui_LcdLedOff();
        //�������ͼ
        Gui_PageClearBackground(p_gui->start_column, p_gui->start_page, p_gui->column_size, p_gui->page_size);
	}
	
    if(p_gui->page_frame_counter == 0)
    {
        Gui_LcdLedOff();
    }   
    
    
    //300ms��ˢ�¼��
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
** Descriptions:            ��������
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2018-06-05
*****************************************************************/ 
void Cpu_EvenCartoon(cpu_task_controller_t *p_controller, gui_controller_t *p_gui)
{
     if(p_controller->init == 1)
	{
        TASK_ENTRY_INIT(p_controller, 5);
        
		//!ע��:�˴�����������Ϊ������ˢ�¶���,������ܻ���ʱ
        evnet_interval_cnt = 0xFF;
		p_gui->page_frame_counter = 0;
		
        
         //�������
        Gui_LcdLedOff();
        //�������ͼ
        Gui_PageClearBackground(p_gui->start_column, p_gui->start_page, p_gui->column_size, p_gui->page_size);
	}
    
    if(p_gui->page_frame_counter == 0)
    {
        Gui_LcdLedOff();
    }   
    
    //300ms��ˢ�¼��
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
** Descriptions:            �ػ�����
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2018-06-05
*****************************************************************/ 
void Cpu_EvenCartoon1(cpu_task_controller_t *p_controller, gui_controller_t *p_gui)
{
     if(p_controller->init == 1)
	{
        TASK_ENTRY_INIT(p_controller, 11);
        
		//!ע��:�˴�����������Ϊ������ˢ�¶���,������ܻ���ʱ
        evnet_interval_cnt = 0xFF;
		p_gui->page_frame_counter = 0;
		
        
       //�������
    Gui_LcdLedOff();
        //�������ͼ
        Gui_PageClearBackground(p_gui->start_column, p_gui->start_page, p_gui->column_size, p_gui->page_size);
	}
	
    
    if(p_gui->page_frame_counter == 0)
    {
        Gui_LcdLedOff();
    }   
    
    //300ms��ˢ�¼��
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
