/***************************Copyright(c)***********************************
**                       Veepoo Tech
**                  COPYRIGHT (C) 2016-2019
**-----------File Info-----------------------------------------------------
** File        name:     cpu_general_task.c
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
#include <stdint.h>
#include <string.h>
#include "cpu_general_task.h"
#include "gui_page_display.h"
#include "gui_data_inteface.h"
#include "gui_thread.h"
#include "bsp_button.h"
#include "app_config_data.h"
#include "system_clock.h"
#include "app_type.h"

//#define DEVICE_NAME                      "S226"

#define GUI_DISPLAY_TIME_3S         3
#define GUI_DISPLAY_TIME_4S         4
#define GUI_DISPLAY_TIME_5S         5

#define GUI_DISPLAY_TIME_HEART      60
#define GUI_DISPLAY_TIME_BP         70


extern uint8_t ui_style_type;
extern system_info_t system_info_s;

static uint16_t task_interval_cnt = 0xFFFF;     //���ļ�������������ж��Ƿ�ˢ��UI�Ͷ�ͼ�л�

static uint8_t gui_jump_to_main_page = 0;       //��ת�������־

static uint8_t reinit_page_frame_flag = 0;		// ����������³�ʼ����־

extern uint8_t gui_movement_change_page;
extern uint8_t ble_name[8];
extern uint8_t count_down_start_flag;			// ����ʱ������־

extern uint8_t task_return_flag;				// �����¼���ϱ�־

extern void Gui_ThreadMovementModeSwitching(void);
extern void Gui_ThreadStopWatchSwitching(void);

/****************************************************************
** Function name:           Cpu_TaskClearIntervalCnt
** Descriptions:            //����ˢ�¼��������
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-08-12
*****************************************************************/ 
void Cpu_TaskClearIntervalCnt(void)
{
    task_interval_cnt = 0xFFFF;
}

/****************************************************************
** Function name:           Cpu_TaskSchedule
** Descriptions:            ��ת����������
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              YX              
** Created Date:            2018-07-10
*****************************************************************/ 
void Cpu_TaskSchedule(cpu_task_controller_t *p_controller, gui_controller_t *p_gui, CPU_TASK_TYPE task_id)
{
    Gui_SetTaskID(task_id);
    p_controller->init = 1;	
    
    //WJ.20180808.ȷ������ˢ��
    p_gui->updata_ui_interval = 0;
    
    if(p_gui->key_info.key != BUTTON_EVENT_NONE)
    {
        p_gui->key_info.key = BUTTON_EVENT_NONE;
    }
    
    //YX.20180609.������������Ĳ���
    Gui_ClearKeyInfo();
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
void Cpu_TaskMain(cpu_task_controller_t *p_controller, gui_controller_t *p_gui)
{
	uint8_t time_format = 0;
    uint8_t year = 0;
    uint8_t month = 0;
    uint8_t day = 0;
	uint8_t hour = 0;
 	uint8_t minute = 0;
    uint8_t week = 0;
    uint8_t battery_level = 0;
    uint8_t battery_charging = 0;
    uint8_t ble_con_state = 0;
    uint8_t phys_menstrual = 0;
    uint8_t heart = 0;
    
    uint16_t calories_cnt = 0;
    uint16_t sleep_time = 0;
    uint32_t all_step = 0;
	uint32_t step_target = 0;

	if(p_controller->init == 1)
	{ 
        if(gui_jump_to_main_page == 1)
        {
            gui_jump_to_main_page = 0;
            TASK_ENTRY_INIT(p_controller, 1);
        }
        else
        {
            TASK_ENTRY_INIT(p_controller, gui_display_time);
//			TASK_ENTRY_INIT(p_controller, 40);
            p_controller->key_press_hold = 1; 
        }
		p_controller->key_press_hold = 1; 
		
		// ע��:�˴�����������Ϊ������ˢ�¶���,������ܻ���ʱ
        task_interval_cnt = 0xFFFF;
		p_gui->page_frame_counter = 0;	
	} 
	
#if	LED_ON_OFF_ON_DEF	
	if(p_gui->page_frame_counter == 0)
	{
		Gui_LcdLedOff();
	}
#endif	
    
    // ˢ�¼�� ms
    p_gui->updata_ui_interval = 800; 

	if(task_interval_cnt != p_gui->page_frame_counter)
	{
        task_interval_cnt = p_gui->page_frame_counter; 
        hour = Gui_DataGetSystemTimeHour(); 
        minute = Gui_DataGetSystemTimeMinute();
        week = Gui_DataGetSystemTimeWeek();
        time_format = Gui_DataTimeFormatDisplay();
        Gui_DataGetSystemTimeDate(&year, &month, &day);
        ble_con_state = Gui_DataGetBleState();
        battery_charging = Gui_DataGetBatteryLevelAndState(&battery_level);
        phys_menstrual = Gui_DataGetPhysMenstrualPeriodType();
        calories_cnt = Gui_DataGetCalorie();
        all_step = Gui_DataGetAllSteps();
		step_target = Gui_DataGetSportsTarget();
        
        heart = Gui_DataGetDisplayHeart();
        sleep_time = Gui_DataGetSleepTime();
        
        Gui_PageMain(task_interval_cnt, month, day, hour, minute,  0, week, 
                     time_format, all_step,  heart,
                     ble_con_state, battery_charging, battery_level, phys_menstrual,
                     calories_cnt, step_target, sleep_time, display_language);
	}
	
#if	LED_ON_OFF_ON_DEF	
	if(p_gui->page_frame_counter == 0)
	{
		Gui_LcdLedOn();
	}
#endif	
    
    if(p_controller->keep_time == 1 && task_interval_cnt >= 2)
    {
        p_controller->run_time = 2;
        Gui_ClearKeyInfo();
        return;
    }
    
    if(p_gui->key_info.key != APP_BSP_KEY_NULL_PRESS)
    {
		if(Gui_DataGetBatteryLevelAndState(NULL) == BATTERRY_IS_NORMAL)
		{
			if(Gui_DataStopWatchIsBusy())
			{   //�������
				Cpu_TaskSchedule(p_controller, p_gui, CPU_TASK_STOP_WATCH);            
			}
			else if(Gui_DataMovementModeIsBusy())
			{
				/* �˶�ģʽ��*/
				Cpu_TaskSchedule(p_controller, p_gui,  CPU_TASK_MOVEMENT_DATA);
			}
			else if(Gui_DataCountDownIsBusy())
			{
				// ����ʱ
				Cpu_TaskSchedule(p_controller, p_gui,  CPU_TASK_COUNT_DOWN);
			}
			else 
			{
				if(p_gui->key_info.key == APP_BSP_KEY_SHORT_PRESS)
				{   //�Ʋ�����·����뼯�Ͻ���
					Cpu_TaskSchedule(p_controller, p_gui, CPU_TASK_STEP);
				}
				else if(p_gui->key_info.key == APP_BSP_PRESS_LONG_3S)
				{   
					Cpu_TaskSchedule(p_controller, p_gui, CPU_TASK_MAC_VER);
				}
			}
		}
		
        Gui_ClearKeyInfo();
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
void Cpu_TaskStep(cpu_task_controller_t *p_controller, gui_controller_t *p_gui)
{	
    uint8_t     step_prg = 0;
    uint8_t     distance_dis_unit = 0;
    float       distance_km = 0.0f;
    float       distance_mile = 0.0f;
    float       calories_cnt = 0;
    uint32_t    distance_m = 0;
    uint32_t    all_step = 0;
    
	if(p_controller->init == 1)
	{ 
        TASK_ENTRY_INIT(p_controller, gui_display_time); 
        
		// ע��:�˴�����������Ϊ������ˢ�¶���,������ܻ���ʱ
        task_interval_cnt = 0xFFFF;
		p_gui->page_frame_counter = 0;
	}
	
#if	LED_ON_OFF_ON_DEF	
	if(p_gui->page_frame_counter == 0)
	{
		Gui_LcdLedOff();
	}
#endif	
	 
    // ˢ�¼�� ms
    p_gui->updata_ui_interval = 150; 
    
	if(task_interval_cnt != p_gui->page_frame_counter)
	{
        task_interval_cnt = p_gui->page_frame_counter; 
        all_step = Gui_DataGetAllSteps();
              
        Gui_DataGetDistanceValue(&distance_m, &distance_km, &distance_mile);
        distance_dis_unit = Gui_DataDistanceIsKM(); 
        calories_cnt = Gui_DataGetCalorie();
        step_prg = Gui_DataGetSportsToMarkProgress();
        
        if(distance_dis_unit == 1 || display_language == 0)
        {
            Gui_PageStepDistanceCalories(task_interval_cnt, all_step, step_prg,
                                  distance_km, 0,
                                  calories_cnt, display_language);
        }
        else
        {
            Gui_PageStepDistanceCalories(task_interval_cnt, all_step, step_prg,
                                  distance_mile, 1,
                                  (uint16_t)calories_cnt, display_language);
        }
	} 

#if	LED_ON_OFF_ON_DEF		
	if(p_gui->page_frame_counter == 0)
	{
		Gui_LcdLedOn();
	}
#endif	
   
    if(p_gui->key_info.key != APP_BSP_KEY_NULL_PRESS)
    {
        if(p_gui->key_info.key == APP_BSP_KEY_SHORT_PRESS)
        {   //���ʽ���
            Cpu_TaskSchedule(p_controller, p_gui, CPU_TASK_HEART);
			
			task_return_flag = 0;
        }
        
        Gui_ClearKeyInfo();
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
void Cpu_TaskRun(cpu_task_controller_t *p_controller, gui_controller_t *p_gui)
{
	if(p_controller->init == 1)
	{ 
        TASK_ENTRY_INIT(p_controller, gui_display_time);
		p_controller->key_press_hold = 1; 

		// ע��:�˴�����������Ϊ������ˢ�¶���,������ܻ���ʱ
        task_interval_cnt = 0xFFFF;
		p_gui->page_frame_counter = 0;		
	} 
	
#if	LED_ON_OFF_ON_DEF	
	if(p_gui->page_frame_counter == 0)
	{
		Gui_LcdLedOff();
	}
#endif	
 
    // ˢ�¼�� ms
    p_gui->updata_ui_interval = 100; 
    
	if(task_interval_cnt != p_gui->page_frame_counter)
	{
        task_interval_cnt = p_gui->page_frame_counter; 
        Gui_PageRunMode(task_interval_cnt, display_language);      
	}
	
#if	LED_ON_OFF_ON_DEF		
	if(p_gui->page_frame_counter == 0)
	{
		Gui_LcdLedOn();
	}
#endif	

    if(p_gui->key_info.key != APP_BSP_KEY_NULL_PRESS)
    {
        if(p_gui->key_info.key == APP_BSP_KEY_SHORT_PRESS)
        {   //������
            Cpu_TaskSchedule(p_controller, p_gui, CPU_TASK_TURNOFF);
        }
        else if(p_gui->key_info.key == APP_BSP_PRESS_LONG_3S)
        { 
            // �����˶����ݽ���
            Cpu_TaskSchedule(p_controller, p_gui, CPU_TASK_MOVEMENT_DATA);
            Gui_DataStartMovementMode();
        }
        Gui_ClearKeyInfo();
    }

}  


/****************************************************************
** Function name:           Cpu_TaskRunData
** Descriptions:            �˶����ݽ���
** input parameters:        p_controller��UI�¼�����״̬��p_gui����ʾ��صĲ���
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-08-12
*****************************************************************/ 
void Cpu_TaskRunData(cpu_task_controller_t *p_controller, gui_controller_t *p_gui)
{
    uint8_t movement_mode_busy = 0;
    uint8_t  uint_type = 0;
    uint8_t  heart = 0;
    uint16_t seconds = 0;
    float calories = 0.0f;
    //float distance_m = 0.0f;
    uint32_t tcalories = 0;
    uint32_t tdistance_m = 0;
    uint32_t all_step = 0;
    uint8_t hour1 = 0;
    uint8_t minute1 = 0;

	if(p_controller->init == 1)
	{ 
        TASK_ENTRY_INIT(p_controller, gui_display_time);
		// ע��:�˴�����������Ϊ������ˢ�¶���,������ܻ���ʱ
        task_interval_cnt = 0xFFFF;
		p_gui->page_frame_counter = 0;
        
	} 
	
#if	LED_ON_OFF_ON_DEF	
	if(p_gui->page_frame_counter == 0)
	{
		Gui_LcdLedOff();
	}
#endif	
 
    // ˢ�¼�� ms
    p_gui->updata_ui_interval = 50; 
    
	if(task_interval_cnt != p_gui->page_frame_counter)
	{
        task_interval_cnt = p_gui->page_frame_counter; 
        movement_mode_busy = Gui_DataMovementModeIsBusy(); 
//        hour = clock->hour;
//        minute = clock->minutes;
        hour1 = Gui_DataGetSystemTimeHour(); 
        minute1 = Gui_DataGetSystemTimeMinute();
        if(movement_mode_busy == 1)
        { 
            p_gui->updata_ui_interval = 100;
            Gui_DataGetMovementModeUiData(&seconds, &heart, &tdistance_m, &tcalories, &all_step);

            /*if(Gui_DataDistanceIsKM() == 0 && display_language != 0)
            {
                distance_m = ((uint32_t)((tdistance_m / 1000.0f) * 1000)) / 1000.0f;//������λС����
                distance_m = distance_m * (0.62138f);
                uint_type = 1;
            } 
            else 
            {
                distance_m = tdistance_m / 1000.0f;
                uint_type = 0;
            }
            */

            calories = tcalories / 1000.0f; 

			Gui_PageRunModeData(task_interval_cnt, seconds, heart, 0, calories, all_step, 
								uint_type, 0, hour1, minute1, Gui_DataMovementModeIsRun(),display_language);
        }
        else
        {
            p_gui->updata_ui_interval = 150; 
            Gui_PageRunModeData(task_interval_cnt, 0, 0, 0, 
                                0, 0, uint_type, 0, hour1, minute1, Gui_DataMovementModeIsRun(),display_language);
        }            
	}
	
#if	LED_ON_OFF_ON_DEF		
	if(p_gui->page_frame_counter == 0)
	{
		Gui_LcdLedOn();
	}
#endif	

    if(p_gui->key_info.key != APP_BSP_KEY_NULL_PRESS)
    {
        if(p_gui->key_info.key == APP_BSP_KEY_SHORT_PRESS)
        {   
			Cpu_TaskSchedule(p_controller, p_gui, CPU_TASK_MOVEMENT_PAUSE);
        }
				
        Gui_ClearKeyInfo();
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
void Cpu_TaskBP(cpu_task_controller_t *p_controller, gui_controller_t *p_gui)
{
    static uint8_t bp_sp = 0;
    static uint8_t bp_dp = 0;
    static uint8_t test_mode = 0;
    static uint8_t test_state = 0;
 
	if(p_controller->init == 1)
	{ 
        TASK_ENTRY_INIT(p_controller, GUI_DISPLAY_TIME_BP);

		if(task_return_flag == 0)
		{
			task_return_flag = 1;
			
			Gui_DataOpenBPOpreation();   //��Ѫѹ 
			bp_sp = 0;
			bp_dp = 0;
			test_mode = 0;
			test_state = 0;	
		}
		
		if(test_state != 0)
		{
			p_controller->keep_time = 5;
		}

		// ע��:�˴�����������Ϊ������ˢ�¶���,������ܻ���ʱ
        task_interval_cnt = 0xFFFF;      
		p_gui->page_frame_counter = 0;

//        bp_sp = 0;
//        bp_dp = 0;
//        test_mode = 0;
//        test_state = 0;		
	} 
	
#if	LED_ON_OFF_ON_DEF	
	if(p_gui->page_frame_counter == 0)
	{
		Gui_LcdLedOff();
	}
#endif	
 
    // ˢ�¼�� ms
    p_gui->updata_ui_interval = 400;
    
	if(task_interval_cnt != p_gui->page_frame_counter)
	{
        if(!Gui_DataGetWearDectState() && p_controller->init != 1 && test_state == 0)
        {
            // �����жϣ�����
            Gui_ThreadTaskClear();
        }       
        task_interval_cnt = p_gui->page_frame_counter;  
        if((task_interval_cnt != 0 && task_interval_cnt != 0xFF) && test_state != 0)
        { 
        }
        else 
        {
            if(test_state == 0 )
            {
                Gui_DataGetBPdata(&bp_sp, &bp_dp, &test_mode, &test_state);
                if(test_state != 0)
                {
                    // ���Ѫѹ������ɻ��������ʵƲ�����������
                    Gui_DataCloseBPOpreation();	
                           
                    task_interval_cnt = 0;
                }
            }
                      
            Gui_PageBP(task_interval_cnt, bp_sp, bp_dp, test_mode, test_state,Gui_DataGetBPprogress());
        }
	}
	
#if	LED_ON_OFF_ON_DEF		
	if(p_gui->page_frame_counter == 0)
	{
		Gui_LcdLedOn();
	}
#endif		

    if(p_gui->key_info.key != APP_BSP_KEY_NULL_PRESS)
    {
        if(p_gui->key_info.key == APP_BSP_KEY_SHORT_PRESS)
        {   //˯�߽���
            Gui_DataCloseHeartOrBpOpreation();
            Cpu_TaskSchedule(p_controller, p_gui, CPU_TASK_SLEEP);
			
			task_return_flag = 0;
        }

        Gui_ClearKeyInfo();
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
void Cpu_TaskHeart(cpu_task_controller_t *p_controller, gui_controller_t *p_gui)
{
    static uint8_t heart = 0;
    static uint8_t last_heart = 0;
        
	if(p_controller->init == 1)
	{ 
        TASK_ENTRY_INIT(p_controller, GUI_DISPLAY_TIME_HEART);
		
		if(task_return_flag == 0)
		{
			task_return_flag = 1;
			
			Gui_DataOpenHeartOpreation();   //������ 
			
			heart = 0;
		}
        last_heart = Gui_DataGetDisplayHeart();

		// ע��:�˴�����������Ϊ������ˢ�¶���,������ܻ���ʱ
        task_interval_cnt = 0xFFFF;  
		p_gui->page_frame_counter = 0;		
	}  
	
#if	LED_ON_OFF_ON_DEF	
	if(p_gui->page_frame_counter == 0)
	{
		Gui_LcdLedOff();
	}
#endif	
    
    // ˢ�¼�� ms
    p_gui->updata_ui_interval = 150; 
    
	if(task_interval_cnt != p_gui->page_frame_counter)
	{
        task_interval_cnt = p_gui->page_frame_counter;  
        if(!Gui_DataGetWearDectState() && p_controller->init != 1)
        {
            // �����жϣ�����
            Gui_ThreadTaskClear();
        }
        if(Gui_DataGetHeart() != 0)
        {
            heart = Gui_DataGetHeart();
        } 
		Gui_PageHeart(task_interval_cnt, heart, last_heart, Gui_DataGetDailyHeartArray(), Gui_DataGetDailyHeartMax(),
                      Gui_DataGetDailyHeartMin(), display_language);
	}
	
#if	LED_ON_OFF_ON_DEF		
	if(p_gui->page_frame_counter == 0)
	{
		Gui_LcdLedOn();
	}
#endif	
    
    if(p_gui->key_info.key != APP_BSP_KEY_NULL_PRESS)
    {
        if(p_gui->key_info.key == APP_BSP_KEY_SHORT_PRESS)
        {
			//WJ.20180912.�Ƿ�֧��Ѫѹ����
            if(Gui_DataIsBpFunction())
            {
                Cpu_TaskSchedule(p_controller, p_gui, CPU_TASK_BP);
            }
            else
            {
                Cpu_TaskSchedule(p_controller, p_gui, CPU_TASK_SLEEP);
            }
            
            Gui_DataCloseHeartOrBpOpreation();
			
			task_return_flag = 0;
        }
        
        Gui_ClearKeyInfo();
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
void Cpu_TaskSleep(cpu_task_controller_t *p_controller, gui_controller_t *p_gui)
{ 
    static uint8_t quality = 0;
    static uint16_t all_time = 0;
    static uint16_t shallow_sleep_time = 0;
    static uint16_t deep_sleep_time = 0;
    
	if(p_controller->init == 1)
	{ 
        TASK_ENTRY_INIT(p_controller, gui_display_time);
        p_controller->key_press_hold = 1; 
		
		// ע��:�˴�����������Ϊ������ˢ�¶���,������ܻ���ʱ
        task_interval_cnt = 0xFFFF;  
		p_gui->page_frame_counter = 0;
        
        
        Gui_DataGetSleepInfo(&all_time, &shallow_sleep_time, &deep_sleep_time, &quality);	
	} 
	
#if	LED_ON_OFF_ON_DEF	
	if(p_gui->page_frame_counter == 0)
	{
		Gui_LcdLedOff();
	}
#endif	
    
    // ˢ�¼�� ms
    p_gui->updata_ui_interval = 300; 
    
	if(task_interval_cnt != p_gui->page_frame_counter)
	{
        task_interval_cnt = p_gui->page_frame_counter; 
        Gui_PageSleep(task_interval_cnt, all_time, deep_sleep_time, 
                      shallow_sleep_time, quality, display_language); 
	}
	
#if	LED_ON_OFF_ON_DEF		
	if(p_gui->page_frame_counter == 0)
	{
		Gui_LcdLedOn();
	}
#endif		
    
    if(p_gui->key_info.key != APP_BSP_KEY_NULL_PRESS)
    {
        if(p_gui->key_info.key == APP_BSP_KEY_SHORT_PRESS)
        {
            if(Gui_DataCountDownPageIsEnable())
            {   //����ʱ
                Cpu_TaskSchedule(p_controller, p_gui, CPU_TASK_COUNT_DOWN);
            }
            else if(Gui_DataFindPhonePageIsEnable())
            {   //���ֻ�
                Cpu_TaskSchedule(p_controller, p_gui, CPU_TASK_FINDPHONE);
            }
            else if(Gui_DataStopWatchPageIsEnable())
            {   //���
                Cpu_TaskSchedule(p_controller, p_gui, CPU_TASK_STOP_WATCH);
            }
            else
            {   //�˶�ģʽ
                Cpu_TaskSchedule(p_controller, p_gui, CPU_TASK_MOVEMENT);
            }
        }
        //���򳤰�6s
        else if(p_gui->key_info.key == APP_BSP_PRESS_LONG_6S)
        {
            //�������
            Gui_DataClearSystemPassword();
            Motor_AddEvent(1,0,1,0);
            p_controller->key_press_hold = 0; 
        }
        
        Gui_ClearKeyInfo();
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
void Cpu_TaskFindPhone(cpu_task_controller_t *p_controller,gui_controller_t *p_gui)
{
    static uint8_t find_phone_enable = 0;
        
    if(p_controller->init == 1)
	{ 
        TASK_ENTRY_INIT(p_controller, gui_display_time);
		p_controller->key_press_hold = 1; 
        
        //!ע��:�˴�����������Ϊ������ˢ�¶���,������ܻ���ʱ
        task_interval_cnt = 0xFFFF;
		p_gui->page_frame_counter = 0;
        
		if(task_return_flag == 0)
		{
			task_return_flag = 1;
		
			find_phone_enable = 0; 	
			
			reinit_page_frame_flag = 0;
		}
        p_gui->animation_enable = 0x00;
	}
	
	if(reinit_page_frame_flag == 1)
	{
		reinit_page_frame_flag = 0;
		p_gui->page_frame_counter = 0;
	}

#if	LED_ON_OFF_ON_DEF	
	if(p_gui->page_frame_counter == 0)
	{
		Gui_LcdLedOff();
	}
#endif	
    
    // ˢ�¼�� ms
    p_gui->updata_ui_interval = 300; 
    
	if(task_interval_cnt != p_gui->page_frame_counter)
	{
        task_interval_cnt = p_gui->page_frame_counter; 
        Gui_PageFindPhone(find_phone_enable, task_interval_cnt, display_language);
        if(((task_interval_cnt % 17) == 0) && (find_phone_enable == 1))
        {
            Gui_DataFindSendData();
        } 
	} 
	
#if	LED_ON_OFF_ON_DEF		
	if(p_gui->page_frame_counter == 0)
	{
		Gui_LcdLedOn();
	}
#endif	
    
    if(p_gui->key_info.key != APP_BSP_KEY_NULL_PRESS)
    {
        if(find_phone_enable == 0)
        {
            if(p_gui->key_info.key == APP_BSP_KEY_SHORT_PRESS)
            {
                if(Gui_DataStopWatchPageIsEnable())
                {   //���
                    Cpu_TaskSchedule(p_controller, p_gui, CPU_TASK_STOP_WATCH);
					task_return_flag = 0;
                }
                else
                {   //�˶�ģʽ
                    Cpu_TaskSchedule(p_controller, p_gui, CPU_TASK_MOVEMENT);
					task_return_flag = 0;
                }
                p_gui->animation_enable = 0x00;
            }
            else if(p_gui->key_info.key == APP_BSP_PRESS_LONG_3S)
            {
                //ȫ�����
                find_phone_enable = 1;
                p_controller->keep_time = 60;
                p_controller->run_time = 0;
                
                task_interval_cnt = 0xFFFF;
                p_gui->page_frame_counter = 0xFFFF; //��Ҫ����
				
				p_gui->updata_ui_interval = 0;
				reinit_page_frame_flag = 1;
                
                //HYQ:2018/10/12  �޸Ĳ����ֻ����ȼ�
                p_gui->animation_enable = 0x01;
            }
        }
        else
        {
            if(p_gui->key_info.key == APP_BSP_KEY_SHORT_PRESS)
            {
				find_phone_enable = 0;
                p_gui->animation_enable = 0x00;
				//ȫ�����
                Cpu_TaskSchedule(p_controller, p_gui, CPU_TASK_FINDPHONE);
            }
        }
        
        Gui_ClearKeyInfo();
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
void Cpu_TaskTurnOff(cpu_task_controller_t *p_controller, gui_controller_t *p_gui)
{   
	uint8_t dis_mac_data[20] = {0};
    uint8_t dis_ver_data[10] = {0};
	
	if(p_controller->init == 1)
	{ 
        TASK_ENTRY_INIT(p_controller, gui_display_time);
		p_controller->key_press_hold = 1; 
		
		//!ע��:�˴�����������Ϊ������ˢ�¶���,������ܻ���ʱ
        task_interval_cnt = 0xFFFF;  
		p_gui->page_frame_counter = 0;			
	}
	
#if	LED_ON_OFF_ON_DEF	
	if(p_gui->page_frame_counter == 0)
	{
		Gui_LcdLedOff();
	}
#endif	
		
    // ˢ�¼�� ms
    p_gui->updata_ui_interval = 300;
    
	if(task_interval_cnt != p_gui->page_frame_counter)
	{
        task_interval_cnt = p_gui->page_frame_counter;

		Gui_DataGetBleMac(dis_mac_data);
        Gui_DataGetVer(dis_ver_data);
		if(ble_name[0] != 0)
		{
			Gui_PageTurnOff(task_interval_cnt,ble_name,dis_mac_data,dis_ver_data,display_language);    
		}
		else
		{
			Gui_PageTurnOff(task_interval_cnt,(uint8_t *)DEVICE_NAME,dis_mac_data,dis_ver_data,display_language);    
		}
	}

#if	LED_ON_OFF_ON_DEF		
	if(p_gui->page_frame_counter == 0)
	{
		Gui_LcdLedOn();
	}
#endif		
    
    if(p_gui->key_info.key != APP_BSP_KEY_NULL_PRESS)
    {
		if(p_gui->key_info.key == APP_BSP_KEY_SHORT_PRESS)
		{  
			Cpu_TaskSchedule(p_controller, p_gui, CPU_TASK_MAIN);
		}
		else if(p_gui->key_info.key == APP_BSP_PRESS_LONG_3S)
		{   
			Gui_DataSystemToPoweOff();  
			Motor_AddEvent(1, 0, 1, 0);
		}
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
void Cpu_TaskPowerOff(cpu_task_controller_t *p_controller, gui_controller_t *p_gui)
{   
	
	if(p_controller->init == 1)
	{ 
        TASK_ENTRY_INIT(p_controller, GUI_DISPLAY_TIME_5S);
		
		//!ע��:�˴�����������Ϊ������ˢ�¶���,������ܻ���ʱ
        task_interval_cnt = 0xFFFF;  
		p_gui->page_frame_counter = 0;		
	}
	
#if	LED_ON_OFF_ON_DEF	
	if(p_gui->page_frame_counter == 0)
	{
		Gui_LcdLedOff();
	}
#endif	
		
    // ˢ�¼�� ms
    p_gui->updata_ui_interval = 150;
    
	if(task_interval_cnt != p_gui->page_frame_counter)
	{
        task_interval_cnt = p_gui->page_frame_counter;
        if(task_interval_cnt < 14)
        {
            Gui_PagePowerOff(task_interval_cnt);        
        }
        else
        {
            p_controller->run_time = 1;
            p_controller->keep_time = 0;
            p_gui->updata_ui_interval = 50;
			
			Gui_ThreadTaskClear();
        }
	} 
	
#if	LED_ON_OFF_ON_DEF		
	if(p_gui->page_frame_counter == 0)
	{
		Gui_LcdLedOn();
	}
#endif	
    
    if(p_gui->key_info.key != APP_BSP_KEY_NULL_PRESS)
    {
        Gui_ClearKeyInfo();
    }
}

/****************************************************************
** Function name:           Cpu_TaskPowerOn
** Descriptions:            �������洦��
** input parameters:        p_controller��UI�¼�����״̬��p_gui����ʾ��صĲ���
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-08-12
*****************************************************************/ 
void Cpu_TaskPowerOn(cpu_task_controller_t *p_controller, gui_controller_t *p_gui)
{   
	if(p_controller->init == 1)
	{ 
        TASK_ENTRY_INIT(p_controller, GUI_DISPLAY_TIME_5S);
		
		//!ע��:�˴�����������Ϊ������ˢ�¶���,������ܻ���ʱ
        task_interval_cnt = 0xFFFF;  
		p_gui->page_frame_counter = 0;		
	}
	
#if	LED_ON_OFF_ON_DEF	
	if(p_gui->page_frame_counter == 0)
	{
		Gui_LcdLedOff();
	}
#endif	
		
    // ˢ�¼�� ms
    p_gui->updata_ui_interval = 150;
    
	if(task_interval_cnt != p_gui->page_frame_counter)
	{
        task_interval_cnt = p_gui->page_frame_counter;
        
        if(task_interval_cnt < 15)
        {
            Gui_PagePowerOn(task_interval_cnt);
        }
        else if(task_interval_cnt >= 30 || p_controller->run_time >= 5)
        {   
            Cpu_TaskSchedule(p_controller, p_gui, CPU_TASK_MAIN);            
        }
	} 
	
#if	LED_ON_OFF_ON_DEF		
	if(p_gui->page_frame_counter == 0)
	{
		Gui_LcdLedOn();
	}
#endif	
    
    if(p_gui->key_info.key != APP_BSP_KEY_NULL_PRESS)
    {
        Gui_ClearKeyInfo();
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
void Cpu_TaskStopWatch(cpu_task_controller_t *p_controller, gui_controller_t *p_gui)
{

    static uint8_t stop_watch_frist_flag = 0;  //���ڼ�¼���������һ������ 
    uint8_t     millisecond = 0;
    uint16_t    total_second = 0;
	uint8_t hour2 = 0;
 	uint8_t minute2 = 0;	
	if(p_controller->init == 1)
	{ 
        if(Gui_DataStopWatchIsBusy())
        {
            TASK_ENTRY_INIT(p_controller, gui_display_time);
        }
        else
        {
            TASK_ENTRY_INIT(p_controller, gui_display_time + 1);
        }
		p_controller->key_press_hold = 1; 
		
		//!ע��:�˴�����������Ϊ������ˢ�¶���,������ܻ���ʱ
        task_interval_cnt = 0xFFFF;  
		p_gui->page_frame_counter = 0;
        stop_watch_frist_flag = 0;
		p_controller->key_press_hold = 1; 
        
        gui_jump_to_main_page = 0;
		
		reinit_page_frame_flag = 0;
	} 
	
	if(reinit_page_frame_flag == 1)
	{
		reinit_page_frame_flag = 0;
		p_gui->page_frame_counter = 0;
	}
	
#if	LED_ON_OFF_ON_DEF	
	if(p_gui->page_frame_counter == 0)
	{
		Gui_LcdLedOff();
	}
#endif

	// ˢ�¼�� ms
    p_gui->updata_ui_interval = 100; 	
    
	if(task_interval_cnt != p_gui->page_frame_counter)
	{
        task_interval_cnt = p_gui->page_frame_counter; 
        if(Gui_DataStopWatchIsBusy())
        {
            Gui_DataGetStopWatchSecond(&total_second, &millisecond); 
        }

        hour2 = Gui_DataGetSystemTimeHour(); 
        minute2 = Gui_DataGetSystemTimeMinute();
        
		GUI_StopWatchPage(task_interval_cnt,Gui_DataStopWatchIsBusy(), Gui_DataStopWatchIsBusy(),
                          Gui_DataStopWatchIsRun(), total_second, millisecond,hour2, minute2, display_language);
	}
	
#if	LED_ON_OFF_ON_DEF		
	if(p_gui->page_frame_counter == 0)
	{
		Gui_LcdLedOn();
	}
#endif	
	    
    if(p_gui->key_info.key != APP_BSP_KEY_NULL_PRESS)
    {
        if(Gui_DataStopWatchIsBusy())
        {
            if(p_gui->key_info.key == APP_BSP_KEY_SHORT_PRESS)
            {   
				//�����ֹͣ���
                Gui_DataToggleStopWatch(); 
                p_gui->updata_ui_interval = 0;
                task_interval_cnt = 0xFFFF;
                if((stop_watch_frist_flag == 0) &&
                   (p_controller->run_time + 5 > p_controller->keep_time))
                {                        
                    p_controller->keep_time = gui_display_time;
                    p_controller->run_time = 0; 
					stop_watch_frist_flag = 0;
                }
				
            }
            else if(p_gui->key_info.key == APP_BSP_PRESS_LONG_3S)
            {   //���������˳����
                Gui_DataExitStopWatch();
                stop_watch_frist_flag = 0;

				Cpu_TaskSchedule(p_controller, p_gui, CPU_TASK_STOP_WATCH);
            }
        }
        else
        {
            if(p_gui->key_info.key == APP_BSP_KEY_SHORT_PRESS)
            {   //�˶�ģʽ
                Cpu_TaskSchedule(p_controller, p_gui, CPU_TASK_MOVEMENT);
            }
			else if(p_gui->key_info.key == APP_BSP_PRESS_LONG_3S)
			{
				Gui_DataSetStopWatchToReady();
				p_controller->keep_time = 60;            
                p_controller->run_time = 0;
                stop_watch_frist_flag = 1; 
				
                task_interval_cnt = 0xFFFF;
				p_gui->page_frame_counter = 0; 
				
				p_gui->updata_ui_interval = 0;
				reinit_page_frame_flag = 1;
			}
			
        }
        
        Gui_ClearKeyInfo();
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
void Gui_TaskCountDown(cpu_task_controller_t *p_controller, gui_controller_t *p_gui)
{
	if(p_controller->init == 1)
	{ 
        TASK_ENTRY_INIT(p_controller, gui_display_time);
		p_controller->key_press_hold = 1; 
        
        // ע��:�˴�����������Ϊ������ˢ�¶���,������ܻ���ʱ
        task_interval_cnt = 0xFFFF;  
		p_gui->page_frame_counter = 0;
	}
	
#if	LED_ON_OFF_ON_DEF	
	if(p_gui->page_frame_counter == 0)
	{
		Gui_LcdLedOff();
	}
#endif	
	
    // ˢ�¼�� ms
    p_gui->updata_ui_interval = 100; 
    
	if(task_interval_cnt != p_gui->page_frame_counter)
	{
        task_interval_cnt = p_gui->page_frame_counter;
        
        if(Gui_DataCountDownIsBusy() == 0)
        {
            Gui_PageCountDown(task_interval_cnt, 0, 0, Gui_DataCountDownConfigSecond(), display_language); 
        }
        else if(Gui_DataCountDownIsRun())
        {
            Gui_PageCountDown(task_interval_cnt, 1, 1, Gui_DataCountDownCurrentSecond(), display_language); 
        }
		else
		{
			Gui_PageCountDown(task_interval_cnt, 1, 0, Gui_DataCountDownCurrentSecond(), display_language); 
		}
	} 
	
#if	LED_ON_OFF_ON_DEF		
	if(p_gui->page_frame_counter == 0)
	{
		Gui_LcdLedOn();
	}
#endif	
    
    if(p_gui->key_info.key != APP_BSP_KEY_NULL_PRESS)
    {
        if(p_gui->key_info.key == APP_BSP_KEY_SHORT_PRESS)
        {
			if(Gui_DataCountDownIsBusy() == 0)
			{
                if(Gui_DataStopWatchIsBusy())
                {   //�������
                    Cpu_TaskSchedule(p_controller, p_gui, CPU_TASK_STOP_WATCH);            
                }
                else if(Gui_DataMovementModeIsBusy())
                {
                    /* �˶�ģʽ��*/
                    Cpu_TaskSchedule(p_controller, p_gui,  CPU_TASK_MOVEMENT_DATA);
                }
				else if(Gui_DataFindPhonePageIsEnable())
				{   //���ֻ�
					Cpu_TaskSchedule(p_controller, p_gui, CPU_TASK_FINDPHONE);
					task_return_flag = 0;
				}
				else if(Gui_DataStopWatchPageIsEnable())
				{   //���
					Cpu_TaskSchedule(p_controller, p_gui, CPU_TASK_STOP_WATCH);
					task_return_flag = 0;
				}
				else
				{   //�˶�ģʽ
					Cpu_TaskSchedule(p_controller, p_gui, CPU_TASK_MOVEMENT);
					task_return_flag = 0;
				}
			}
			else
			{
				// ��Ҫ�жϵ���ʱ��ǰ״̬
				if(Gui_DataCountDownIsRun())
				{       
					Gui_DataCountDownPause();                   
					p_controller->keep_time = gui_display_time;            
					p_controller->run_time = 0;             
				}
				else            
				{
					if(Gui_DataCountDownIsBusy())
					{               
						p_controller->keep_time = gui_display_time;            
						p_controller->run_time = 0; 
					}
					else
					{
						Gui_ThreadTaskClear();
						// WJ.20171108.����ڳ�פ���濪������ʱ�������¼����
						Gui_ThreadEventTaskWakeUp(CPU_EVENT_TASK_COUNT_DOWN_START); 
					}
					Gui_DataCountDownStart();
				}
			}
        }
        else if(p_gui->key_info.key == APP_BSP_PRESS_LONG_3S)
        {   
			if(Gui_DataCountDownIsBusy())
			{
				Gui_DataCountDownExit();   
				Cpu_TaskSchedule(p_controller, p_gui, CPU_TASK_COUNT_DOWN);
			}
			else
			{
				Gui_ThreadTaskClear();
				count_down_start_flag = 1;
				// WJ.20171108.����ڳ�פ���濪������ʱ�������¼����
				Gui_ThreadEventTaskWakeUp(CPU_EVENT_TASK_COUNT_DOWN_START); 
			}
        }
        
        Gui_ClearKeyInfo();
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
void Cpu_TaskDisMacVer(cpu_task_controller_t *p_controller, gui_controller_t *p_gui)
{
    uint8_t dis_mac_data[20] = {0};
    uint8_t dis_ver_data[10] = {0};
	if(p_controller->init == 1)
	{
        TASK_ENTRY_INIT(p_controller, gui_display_time);  
		 
		// ע��:�˴�����������Ϊ������ˢ�¶���,������ܻ���ʱ
		task_interval_cnt = 0xFFFF;
		p_gui->page_frame_counter = 0;
	}
	
#if	LED_ON_OFF_ON_DEF	
	if(p_gui->page_frame_counter == 0)
	{
		Gui_LcdLedOff();
	}
#endif	
	
    // ˢ�¼�� ms
    p_gui->updata_ui_interval = 300;   
	if(task_interval_cnt != p_gui->page_frame_counter)
	{
        task_interval_cnt = p_gui->page_frame_counter;
		
		Gui_DataGetBleMac(dis_mac_data);
        Gui_DataGetVer(dis_ver_data);
        Gui_PageDisMacVer(task_interval_cnt, dis_mac_data, dis_ver_data);
	}
	
#if	LED_ON_OFF_ON_DEF		
	if(p_gui->page_frame_counter == 0)
	{
		Gui_LcdLedOn();
	}
#endif		
     
    if(p_gui->key_info.key != APP_BSP_KEY_NULL_PRESS)
    {   
        if(p_gui->key_info.key == APP_BSP_KEY_SHORT_PRESS)
        {   //������
            Cpu_TaskSchedule(p_controller, p_gui, CPU_TASK_MAIN); 
        }
        Gui_ClearKeyInfo();
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
void Cpu_TaskRunModePause(cpu_task_controller_t *p_controller, gui_controller_t *p_gui)
{
	if(p_controller->init == 1)
	{ 
        TASK_ENTRY_INIT(p_controller, gui_display_time);
        p_controller->key_press_hold = 1;    
		 
		//!ע��:�˴�����������Ϊ������ˢ�¶���,������ܻ���ʱ
		task_interval_cnt = 0xFFFF;
		p_gui->page_frame_counter = 0;	
		reinit_page_frame_flag = 0;		
	}
	
	if(reinit_page_frame_flag == 1)
	{
		reinit_page_frame_flag = 0;
		p_gui->page_frame_counter = 0;
	}	
	
#if	LED_ON_OFF_ON_DEF
	if(p_gui->page_frame_counter == 0)
	{
		Gui_LcdLedOff();
	}
#endif	
	
	//300ms��ˢ�¼��
    p_gui->updata_ui_interval = 300; 
    
	if(task_interval_cnt != p_gui->page_frame_counter)
	{
        task_interval_cnt = p_gui->page_frame_counter; 
        Gui_PageRunModePause(task_interval_cnt, Gui_DataMovementModeIsRun(), display_language); 
	}
	
#if	LED_ON_OFF_ON_DEF		
	if(p_gui->page_frame_counter == 0)
	{
		Gui_LcdLedOn();
	}
#endif		
	
	if(p_gui->key_info.key != APP_BSP_KEY_NULL_PRESS)
    {   
        if(p_gui->key_info.key == APP_BSP_KEY_SHORT_PRESS)
        {   
            Cpu_TaskSchedule(p_controller, p_gui, CPU_TASK_MOVEMENT_EXIT);
        }
		else if(p_gui->key_info.key == APP_BSP_PRESS_LONG_3S)
        {   
			Gui_DataToggleMovementMode();
			Motor_AddEvent(1, 0, 1, 0);
			p_controller->keep_time = gui_display_time;
			p_controller->run_time = 0;
			
			task_interval_cnt = 0xFFFF;
			p_gui->page_frame_counter = 0;
			
			p_gui->updata_ui_interval = 0;
			reinit_page_frame_flag = 1;
        }
        Gui_ClearKeyInfo();
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
void Cpu_TaskRunModeExit(cpu_task_controller_t *p_controller, gui_controller_t *p_gui)
{
	if(p_controller->init == 1)
	{ 
        TASK_ENTRY_INIT(p_controller, gui_display_time);
        p_controller->key_press_hold = 1; 
		 
		//!ע��:�˴�����������Ϊ������ˢ�¶���,������ܻ���ʱ
		task_interval_cnt = 0xFFFF;
		p_gui->page_frame_counter = 0;	
	}
	
#if	LED_ON_OFF_ON_DEF	
	if(p_gui->page_frame_counter == 0)
	{
		Gui_LcdLedOff();
	}
#endif	
	
    //500ms��ˢ�¼��
    p_gui->updata_ui_interval = 300;
    
	if(task_interval_cnt != p_gui->page_frame_counter)
	{
        task_interval_cnt = p_gui->page_frame_counter;
        Gui_PageRunModeExit(task_interval_cnt,display_language);
	}
	
#if	LED_ON_OFF_ON_DEF		
	if(p_gui->page_frame_counter == 0)
	{
		Gui_LcdLedOn();
	}
#endif		
	
	if(p_gui->key_info.key != APP_BSP_KEY_NULL_PRESS)
    {   
        if(p_gui->key_info.key == APP_BSP_KEY_SHORT_PRESS)
        {   //������
            Cpu_TaskSchedule(p_controller, p_gui, CPU_TASK_MOVEMENT_DATA);
        }
		else if(p_gui->key_info.key == APP_BSP_PRESS_LONG_3S)
        {   
			//�����˳�
		   if(Gui_DataMovementModeIsBusy())
		   {
				Gui_DataExitMovementMode();
		   }
		   // �����˶�����
		   Cpu_TaskSchedule(p_controller, p_gui, CPU_TASK_MOVEMENT);
        }
        Gui_ClearKeyInfo();
    }
}
