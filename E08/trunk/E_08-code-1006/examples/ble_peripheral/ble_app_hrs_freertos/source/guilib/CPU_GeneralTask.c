/*******************Copyright(c)*********************************
** Veepoo Tech
** COPYRIGHT (C) 2016-2019
**----------------------FileInfo---------------------------------
** File name:                cpu_general_task.c
** Last modified Date:       2017-07-10
** Last SYS_VERSION:         0.0.1
** Descriptions:
**---------------------------------------------------------------
** Createdby:                郭涛
** Createddate:              2017-07-10
** SYS_VERSION:              0.0.1
** Descriptions:             cpu一般任务
**---------------------------------------------------------------
** Modified by:				
** Modified date:			
** SYS_VERSION:				
** Modified Descriptions:	
****************************************************************/

#include <stdint.h>
#include "CPU_GeneralTask.h"
#include "CPU_EventTask.h"
#include "SystemTime.h"
#include "api_flash.h"
#include "app_config_data.h"
#include "time_manage.h"
#include "algorithm_thread.h"
#include "data_manage.h"
#include "battery_adc.h"
#include "log.h"
#include "ble_thread.h"
#include "util.h"
#include "movement_patterns.h"
#include "gui_page_display.h"
#include "Gui_Thread.h"
#include "gui_data_inteface.h"
#include "test_state_machine.h"
#include "util.h"
#include "lcd_driver_tft.h"
#include "communication_thread.h"
#include "backlight_driving.h"
static uint8_t funct2_page = 0;
static uint8_t funct2_num = 0;

static uint32_t all_step = 0;                //总步数
static uint32_t distance_m = 0;              //距离  单位：米
static float   distance_km = 0;              //距离  单位：千米
static float   distance_mile = 0;            //距离  单位：英里
static uint32_t   calories_buf = 0;             //卡路里
uint8_t Operation_state = 0;
static uint16_t task_interval_cnt = 0xFFFF;     //最后的间隔次数，用于判断是否刷新UI和动图切换
uint8_t runmode_show_type = 0; //运动模式显示类型，为2页显示还是3页显示   
extern uint8_t ui_style_type;
uint8_t current_page = 0; 
void Cpu_TaskParamInit(cpu_task_tcb_t* p_task,uint16_t keep_time,bool press_hold,bool animation_active)
{
    // 任务初始化
    p_task->task_controller.init = 0;
    p_task->task_controller.active = 1; 
    p_task->task_controller.keep_time = keep_time; 
    p_task->task_controller.run_time = 0;
    p_task->task_controller.key_press_hold = press_hold; 

    p_task->gui_controller.animation_enable = animation_active;
    // 注意:这里变量初始化为0xFF,主要处理静态页面刷新控制
    p_task->gui_controller.page_frame_counter = 0;//0xFF;
    p_task->gui_controller.animation_tick_counter = 0;
    p_task->gui_controller.animation_frame_index = 0;	
}

void Cpu_ScheduleTask(cpu_task_tcb_t* p_task, cpu_device_tcb_t* p_device, CPU_TASK_TYPE task_id)
{
    p_task->task_id = task_id;
    p_task->task_controller.init = 1;	
    p_device->oled.light = 1;
    
    if(p_device->key != BUTTON_EVENT_NULL)
    {
        p_device->key = BUTTON_EVENT_NULL;
    }
}

void Cpu_IdelToTimerTask(cpu_task_tcb_t* p_task, cpu_device_tcb_t* p_device)
{
    //WJ.20181128.重置显示时间
    gui_display_time = Api_GetUiDisplayTime();

    p_device->oled.light = 1; 
    p_task->task_id = CPU_TASK_TIMER;
    p_task->task_controller.keep_time = gui_display_time + 2;
    p_task->task_controller.run_time = 0;
    p_task->task_controller.key_press_hold = 1;//秒表退不出的bug           
    p_task->task_controller.active = 1; 
    //YX.20180628.打开动画刷新标志
    p_task->gui_controller.animation_enable = 1;
}

void Cpu_IdelToRunTask(cpu_task_tcb_t* p_task, cpu_device_tcb_t* p_device)
{
    //WJ.20181128.重置显示时间
    gui_display_time = Api_GetUiDisplayTime();

    p_device->oled.light = 1; 
    p_task->task_id = CPU_TASK_MOVEMENT_DATA;//CPU_TASK_MOVEMENT;
    p_task->task_controller.keep_time = gui_display_time + 2;
    p_task->task_controller.run_time = 0;
    p_task->task_controller.key_press_hold = 1;         
    p_task->task_controller.active = 1; 
    //YX.20180628.打开动画刷新标志
    p_task->gui_controller.animation_enable = 1;
}

void Cpu_ClearSechduleTask(cpu_task_tcb_t* p_task)
{
    if(p_data_manage->device_trace_param_s.factory_mode == 0)
    {
        MotorDelEvent();
    }
    memset(p_task, 0, sizeof(cpu_task_tcb_t));
}

/****************************************************************
** Function name:            Cpu_MainTask
** Descriptions:             主界面
** input parameters:         p_controller：       CPU任务控制器指针
                             p_gui：              静态页面控制器指针
                             p_oled：             oled驱动指针
** output parameters:        无
** Returned value:           无
** Created by:               GT
** Created Date:             2017-07-10
****************************************************************/
void Cpu_MainTask(cpu_task_tcb_t* p_task, cpu_device_tcb_t* p_device)
{
    uint8_t time_format = 0;
	uint8_t hour = 0;
//    static uint8_t last_minute = 0;
//    uint16_t step_prg = 0;
//    uint8_t phys_menstrual = 0;

    if(p_task->task_controller.init == 1)
    {
//        p_data_manage->ecg_ppg_s.btn = 0;
//        p_data_manage->blood_oxygen_measure_s.btn = 0;	
		Cpu_TaskParamInit(p_task, gui_display_time, true, true);
        p_task->child_task_id = 1;
//        //清除背景
        Gui_PageClearBackground(0, 0, 
                                240, 240);
    }
         
#if LED_ON_OFF_ON_DEF
    if(p_task->gui_controller.animation_tick_counter == 0)
    {
        Gui_LcdLedOff();
    } 
#endif
  
	if((p_task->gui_controller.animation_tick_counter%6 == 0)&&(p_task->gui_controller.repaint_enable == 1))
    {
        
		if(p_task->child_task_id != 1)
		{
//			//清除背景
//			Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
//									p_device->cls_param.column_size, p_device->cls_param.page_size);
            p_task->gui_controller.animation_frame_index = 0;
			p_task->child_task_id = 1;
		}
         
//         
        hour = Hour_StandardTransform(p_flash_param->dev_config_s.am_pm_enable,clock->hour);
        time_format = Get_AMPM_Format(p_flash_param->dev_config_s.am_pm_enable,clock->hour);
        distance_value(&all_step,&distance_m,&distance_km,&distance_mile,&calories_buf);
        
        task_interval_cnt = p_task->gui_controller.animation_frame_index++;

        if(p_flash_param->dev_config_s.english_style_enable == 0)
        {
            Gui_PageMain(task_interval_cnt, clock->month, clock->day, hour, clock->minutes, distance_km, clock->week,
                         time_format, all_step, 0, p_data_manage->device_trace_param_s.ble_connect, Battery_Get_Status(),
                         Battery_Get_Grade(), 0, calories_buf, 0, 0, display_language, &p_device->cls_flag);
        }
        else
        {
            Gui_PageMain(task_interval_cnt, clock->month, clock->day, hour, clock->minutes, distance_mile, clock->week,
                         time_format, all_step, 0, p_data_manage->device_trace_param_s.ble_connect, Battery_Get_Status(),
                         Battery_Get_Grade(), 0, calories_buf, 0, 0, display_language, &p_device->cls_flag);
        }
        
	}
    
#if LED_ON_OFF_ON_DEF
    //YX.20180830.增加延时，去掉显示的闪屏问题
    if(p_task->gui_controller.animation_tick_counter == 0)
    {
         
        Gui_LcdLedOn();
    } 
#endif
    
    // 按键短按,进入记步界面
    if(p_task->gui_controller.repaint_enable == 1)
    {
        if((p_device->key == BUTTON_EVENT_CLICK)&&(Battery_Get_Status() == BATTERY_POWER_NORMAL))
        {
            // 注意:充电或者充满时,不能切换界面
            Cpu_ScheduleTask(p_task, p_device, CPU_TASK_STEP);
        }
		else if((p_device->key == BUTTON_EVENT_LONGPRESS)&&(Battery_Get_Status() == BATTERY_POWER_NORMAL))
		{
			Cpu_ScheduleTask(p_task, p_device, CPU_TASK_CHOOSE_MAIN);//CPU_TASK_MAC);
		}
        p_device->key = BUTTON_EVENT_NULL;
    }
	
}

/****************************************************************
** Function name:           Cpu_ChooseMainTask
** Descriptions:            选择主界面风格
** input parameters:        无 
** output parameters:       无
** Returned value:          无
** Created by:              YX              
** Created Date:            2017-12-12
*****************************************************************/ 
void Cpu_ChooseMainTask(cpu_task_tcb_t* p_task, cpu_device_tcb_t* p_device)
{
	static uint8_t main_style = 0;

    if(p_task->task_controller.init == 1)
    {
        Cpu_TaskParamInit(p_task, gui_display_time,false,true);//false); 
		main_style = ui_style_type;
        p_task->child_task_id = 1;
    }

#if LED_ON_OFF_ON_DEF
    if(p_task->gui_controller.animation_tick_counter == 0)
    {
        Gui_LcdLedOff();
    } 
#endif
 
    if((p_task->gui_controller.animation_tick_counter%6 == 0) && p_task->gui_controller.repaint_enable == 1)
    {
		if(p_task->child_task_id != 1)
		{
			p_task->gui_controller.animation_frame_index = 0;
			p_task->child_task_id = 1;
		}
		
		Gui_PageChooseMain(p_task->gui_controller.animation_frame_index++, main_style, &p_device->cls_flag);     
    }
    
#if LED_ON_OFF_ON_DEF
    //YX.20180830.增加延时，去掉显示的闪屏问题
    if(p_task->gui_controller.animation_tick_counter == 0)
    {
        Gui_LcdLedOn();
    } 
#endif
  
    if(p_task->gui_controller.repaint_enable == 1)
    {
        if(p_device->key == BUTTON_EVENT_CLICK)
        {
            main_style++;
            if(main_style >= 4)
            {
                main_style = 0;
            }
            p_task->task_controller.run_time = 0;
            p_task->gui_controller.animation_frame_index = 0;
        }
        else if((p_device->key == BUTTON_EVENT_LONGPRESS)&&(Battery_Get_Status() == BATTERY_POWER_NORMAL))
        {
            if(main_style != ui_style_type)
            {
                //需要改变主界面风格样式
                Api_UpdateUiStyle(main_style);
                ui_style_type = main_style;
            }
            
            Cpu_ScheduleTask(p_task, p_device, CPU_TASK_MAIN);
        }
             
        p_device->key = BUTTON_EVENT_NULL;
    }

}

/****************************************************************
** Function name:           Cpu_MacAdd
** Descriptions:            MAC和版本号的显示
** input parameters:        无 
** output parameters:       无
** Returned value:          无
** Created by:              YX              
** Created Date:            2017-12-12
*****************************************************************/ 
void Cpu_MacAdd(cpu_task_tcb_t* p_task, cpu_device_tcb_t* p_device)
{
	static uint8_t dis_mac_data[6] = {0};
    static uint8_t dis_ver_data[2] = {0};

    if(p_task->task_controller.init == 1)
    {
        Cpu_TaskParamInit(p_task, gui_display_time,false,true);//false); 
		Gui_DataGetBleMac(dis_mac_data);
        Gui_DataGetVer(dis_ver_data);
        p_task->child_task_id = 1;
    }

#if LED_ON_OFF_ON_DEF
    if(p_task->gui_controller.animation_tick_counter == 0)
    {
        Gui_LcdLedOff();
    } 
#endif
 
    if((p_task->gui_controller.animation_tick_counter%6 == 0) && p_task->gui_controller.repaint_enable == 1)
    {
		if(p_task->child_task_id != 1)
		{
			p_task->child_task_id = 1;
		}
		Gui_PageDisMacVer(p_task->gui_controller.animation_frame_index++, dis_mac_data, dis_ver_data, display_language, &p_device->cls_flag);    
    }
    
#if LED_ON_OFF_ON_DEF
    //YX.20180830.增加延时，去掉显示的闪屏问题
    if(p_task->gui_controller.animation_tick_counter == 0)
    {
        Gui_LcdLedOn();
    } 
#endif
  
    if(p_task->gui_controller.repaint_enable == 1)
    {
        if((p_device->key == BUTTON_EVENT_CLICK)&&(Battery_Get_Status() == BATTERY_POWER_NORMAL))
        {
            p_device->key = BUTTON_EVENT_NULL;
            // 注意:充电或者充满时,不能切换界面
            Cpu_ScheduleTask(p_task, p_device, CPU_TASK_MAIN);
        }
        p_device->key = BUTTON_EVENT_NULL;
    }
}

/****************************************************************
** Function name:            Cpu_TaskRun
** Descriptions:             运动模式界面
** input parameters:         p_controller：       CPU任务控制器指针
                             p_gui：              静态页面控制器指针
                             p_oled：             oled驱动指针
** output parameters:        无
** Returned value:           无
** Created by:               YX
** Created Date:             2017-11-14
****************************************************************/
void Cpu_TaskRun(cpu_task_tcb_t* p_task, movement_cont_t* p_movement, cpu_device_tcb_t* p_device)
{ 
    uint16_t hour = 0;
    if(p_task->task_controller.init == 1)
    {
        Cpu_TaskParamInit(p_task,gui_display_time,false,true);
        p_task->child_task_id = 1; 
    }

#if LED_ON_OFF_ON_DEF
    if(p_task->gui_controller.animation_tick_counter == 0)
    {
        Gui_LcdLedOff();
    } 
#endif

    if((p_task->gui_controller.animation_tick_counter%4 == 0)&&(p_task->gui_controller.repaint_enable == 1))
    {  
        hour = Hour_StandardTransform(p_flash_param->dev_config_s.am_pm_enable,clock->hour);
        //显示运动模式人物界面
        Gui_PageRunMode(p_task->gui_controller.animation_frame_index++, hour, clock->minutes, display_language, &p_device->cls_flag);            
    }
    
#if LED_ON_OFF_ON_DEF
    //YX.20180830.增加延时，去掉显示的闪屏问题
    if(p_task->gui_controller.animation_tick_counter == 0)
    {
        Gui_LcdLedOn();
    } 
#endif
    if((p_task->gui_controller.repaint_enable == 1)&&(p_device->key != BUTTON_EVENT_NULL))
    {
        if((p_device->key == BUTTON_EVENT_LONGPRESS))
        {
            // 进入多运动选择
            Cpu_ScheduleTask(p_task, p_device, CPU_TASK_MOVEMENT_SELECT);
        }
        else if(p_device->key == BUTTON_EVENT_CLICK)
        {
            Cpu_ScheduleTask(p_task, p_device, CPU_TASK_ECG);          
        }
        p_device->key = BUTTON_EVENT_NULL;	 
    }   
    
}
/****************************************************************
** Function name:            Cpu_TaskRunSelect
** Descriptions:             多运动模式选择界面
** input parameters:         p_controller：       CPU任务控制器指针
                             p_gui：              静态页面控制器指针
                             p_oled：             oled驱动指针
** output parameters:        无
** Returned value:           无
** Created by:               YX
** Created Date:             2017-11-14
****************************************************************/
void  Cpu_TaskRunSelect(cpu_task_tcb_t* p_task, cpu_device_tcb_t* p_device, movement_cont_t* p_movement)
{
    static uint8_t switch_state[4][3] = {0};
    uint8_t slid_ret = 0;
    static uint8_t switch_page = 0;
    static uint8_t switch_num = 0;
    uint16_t switch_tmp = 0;
        
    if(p_task->task_controller.init == 1)
    {
        Cpu_TaskParamInit(p_task,gui_display_time,false,true);
        p_task->child_task_id = 1;
        switch_page = 0;
        switch_num = 0;        
    }

#if LED_ON_OFF_ON_DEF
    if(p_task->gui_controller.animation_tick_counter == 0)
    {
        Gui_LcdLedOff();
    } 
#endif

	if(((p_task->gui_controller.animation_tick_counter%3) == 0)&&(p_task->gui_controller.repaint_enable == 1))
	{    
        Gui_PageRunModeSelect(p_task->gui_controller.animation_frame_index++, switch_page, switch_num, switch_state[switch_page%4], display_language);
	}
    
#if LED_ON_OFF_ON_DEF
    //YX.20180830.增加延时，去掉显示的闪屏问题
    if(p_task->gui_controller.animation_tick_counter == 0)
    {
        Gui_LcdLedOn();
    } 
#endif
    if(p_task->gui_controller.repaint_enable == 1)
    {
        if(p_device->key == BUTTON_EVENT_CLICK)
        {
            switch_num++;
            if(switch_num >= 4 || (switch_page == 3 && switch_num >= 2))
            {
                switch_num = 0;
                switch_page++;
                if(switch_page >= 4)
                {
                    switch_page = 0;
                    switch_num = 0;
                }
                task_interval_cnt = 0xFFFF;
                p_task->gui_controller.animation_frame_index = 0;
            }
            p_task->task_controller.run_time = 0;
        }
        else if(p_device->key == BUTTON_EVENT_LONGPRESS)
        {   
            switch_tmp = switch_page*4 + switch_num;
            if(switch_tmp == 1)
            {
               runmode_type = APP_MSM_MODE_TYPE_RUN_OUTDOOR;  // 户外跑步 
            }
            if(switch_tmp == 2)
            {
               runmode_type = APP_MSM_MODE_TYPE_WALK_OUTDOOR;  // 户外步行 
            }
            if(switch_tmp == 3)
            {
               runmode_type = APP_MSM_MODE_TYPE_RUN_INDOOR;    // 室内跑步
            }
            if(switch_tmp == 5)
            {
               runmode_type = APP_MSM_MODE_TYPE_WALK_INDOOR;   // 室内步行
            }
            if(switch_tmp == 6)
            {
               runmode_type = APP_MSM_MODE_TYPE_HIKING;        // 徒步
            }
            
            if(switch_tmp == 7)
            {
               runmode_type = APP_MSM_MODE_TYPE_TREADMILLS_MACHINE;   // 踏步机
            }
            if(switch_tmp == 9)
            {
               runmode_type = APP_MSM_MODE_TYPE_BIKING_OUTDOOR;      // 户外骑行
            }
            if(switch_tmp == 10)
            {
               runmode_type = APP_MSM_MODE_TYPE_BIKING_INDOOR;       // 室内骑行
            }
            if(switch_tmp == 11)
            {
               runmode_type = APP_MSM_MODE_TYPE_ELLIPTICAL_MACHINE;  // 椭圆机 
            }
            if(switch_tmp == 13)
            {
               runmode_type = APP_MSM_MODE_TYPE_ROWING_MACHINE;     // 划船机
            }
            switch(switch_tmp)
            {
                case 0:case 4:case 8:case 12://返回上一个界面
                    Cpu_ScheduleTask(p_task, p_device, CPU_TASK_MOVEMENT);
                    break;
                case 1: case 2:case 3:case 5:case 6:
//                    runmode_show_type = 0;   //2页数据界面
//                    // 进入运动模式
//                    Movement_Start();
//                    memset(p_movement,0,sizeof(movement_cont_t));
//                    p_movement->enable = 1;
//                    p_movement->face_num = 1;
//                    p_movement->run = 1;               
//                    gui_display_time = Api_GetUiDisplayTime();
//                    p_task->task_controller.keep_time = gui_display_time+2;
//                    p_task->task_controller.run_time = 0;
//                    p_task->task_controller.active = 1;

//                    Cpu_ScheduleTask(p_task, p_device, CPU_TASK_MOVEMENT_DATA);
                
                    runmode_show_type = 1;  //3页数据界面
                    // 进入运动模式
                    Movement_Start();
                    memset(p_movement,0,sizeof(movement_cont_t));
                    p_movement->enable = 1;
                    p_movement->face_num = 1;
                    p_movement->run = 1;               
                    gui_display_time = Api_GetUiDisplayTime();
                    p_task->task_controller.keep_time = gui_display_time+2;
                    p_task->task_controller.run_time = 0;
                    p_task->task_controller.active = 1;

                    Cpu_ScheduleTask(p_task, p_device, CPU_TASK_MOVEMENT_DATA);

                    break;
                case 7: case 9:case 10:case 11:case 13:
//                    runmode_show_type = 1;  //3页数据界面
//                    // 进入运动模式
//                    Movement_Start();
//                    memset(p_movement,0,sizeof(movement_cont_t));
//                    p_movement->enable = 1;
//                    p_movement->face_num = 1;
//                    p_movement->run = 1;               
//                    gui_display_time = Api_GetUiDisplayTime();
//                    p_task->task_controller.keep_time = gui_display_time+2;
//                    p_task->task_controller.run_time = 0;
//                    p_task->task_controller.active = 1;

//                    Cpu_ScheduleTask(p_task, p_device, CPU_TASK_MOVEMENT_DATA);
                
                    runmode_show_type = 0;   //2页数据界面
                    // 进入运动模式
                    Movement_Start();
                    memset(p_movement,0,sizeof(movement_cont_t));
                    p_movement->enable = 1;
                    p_movement->face_num = 1;
                    p_movement->run = 1;               
                    gui_display_time = Api_GetUiDisplayTime();
                    p_task->task_controller.keep_time = gui_display_time+2;
                    p_task->task_controller.run_time = 0;
                    p_task->task_controller.active = 1;

                    Cpu_ScheduleTask(p_task, p_device, CPU_TASK_MOVEMENT_DATA);

                    break;
                default:
                    break;
            }
        }
        p_device->key = BUTTON_EVENT_NULL;
    }  
}

/****************************************************************
** Function name:            Cpu_TaskRunData
** Descriptions:             运动数据界面
** input parameters:         p_controller：       CPU任务控制器指针
                             p_gui：              静态页面控制器指针
                             p_oled：             oled驱动指针
** output parameters:        无
** Returned value:           无
** Created by:               YX
** Created Date:             2017-11-14
****************************************************************/
void  Cpu_TaskRunData(cpu_task_tcb_t* p_task, cpu_device_tcb_t* p_device, movement_cont_t* p_movement)
{
    static uint8_t last_ui_style = 0;
    uint8_t time_format = 0;
	uint8_t hour = 0;
    uint32_t calories = 0.0f;
    float part_distance_m = 0.0f;;
    uint32_t Speed_distance = 0;   
    if(p_task->task_controller.init == 1)
    {
        if(p_movement->enable == 1)
        {
            p_movement->face_num = 1;
            Cpu_TaskParamInit(p_task,gui_display_time,false,true);
            p_task->child_task_id = 3;
        }
        else
        {
            p_movement->entry = 1;
            Cpu_TaskParamInit(p_task,gui_display_time,true,true);
            p_task->child_task_id = 1;
        }        
    }

#if LED_ON_OFF_ON_DEF
    if(p_task->gui_controller.animation_tick_counter == 0)
    {
        Gui_LcdLedOff();
    } 
#endif
    if((p_task->gui_controller.repaint_enable == 1))
	//if(((p_task->gui_controller.animation_tick_counter%6) == 0)&&(p_task->gui_controller.repaint_enable == 1))
	{    
        // 运动模式状态
        if(p_movement->enable == 1)
        {
            //if((p_task->gui_controller.animation_tick_counter%2 == 0)&&(p_task->gui_controller.repaint_enable == 1))
            {
                if(p_flash_param->dev_config_s.english_style_enable == 1)
                {
                    part_distance_m = ((uint32_t)((movement_info_s.ui_distance / 1000.0f) * 1000)) / 1000.0f;//保留三位小数点
                    part_distance_m = part_distance_m * (0.62138f);
                } 
                else 
                {
                    part_distance_m = movement_info_s.ui_distance / 1000.0f;
                }
                calories = movement_info_s.ui_calories/1000.0f;
                Speed_distance =  (uint32_t)(movement_info_s.ui_distance / 1000.0f);
                hour = Hour_StandardTransform(p_flash_param->dev_config_s.am_pm_enable,clock->hour);
                distance_value(&all_step,&distance_m,&distance_km,&distance_mile,&calories_buf);
                
                if((p_movement->face_num == 1 || p_movement->face_num == 0 || p_movement->face_num == 2 || p_movement->face_num == 3)
                   && (p_task->task_controller.run_time > gui_display_time) && ((p_task->gui_controller.animation_tick_counter%6) == 0))
                {
					if(p_task->child_task_id != 2)
					{
                        p_task->child_task_id = 2;
						p_task->gui_controller.animation_frame_index = 0;
                        last_ui_style = ui_style_type;
					}
                    if(last_ui_style != ui_style_type)
                    {
                        last_ui_style = ui_style_type;
                        p_task->gui_controller.animation_frame_index = 0;
                    }
                    hour = Hour_StandardTransform(p_flash_param->dev_config_s.am_pm_enable,clock->hour);
                    time_format = Get_AMPM_Format(p_flash_param->dev_config_s.am_pm_enable,clock->hour);
                    distance_value(&all_step,&distance_m,&distance_km,&distance_mile,&calories_buf);
                    
                    task_interval_cnt = p_task->gui_controller.animation_frame_index++;
                    if(p_flash_param->dev_config_s.english_style_enable == 0)
                    {
                        Gui_PageMain(task_interval_cnt, clock->month, clock->day, hour, clock->minutes, distance_km, clock->week,
                                     time_format, all_step, 0, p_data_manage->device_trace_param_s.ble_connect, Battery_Get_Status(),
                                     Battery_Get_Grade(), 0, calories_buf, 0, 0, display_language, &p_device->cls_flag);
                    }
                    else
                    {
                        Gui_PageMain(task_interval_cnt, clock->month, clock->day, hour, clock->minutes, distance_mile, clock->week,
                                     time_format, all_step, 0, p_data_manage->device_trace_param_s.ble_connect, Battery_Get_Status(),
                                     Battery_Get_Grade(), 0, calories_buf, 0, 0, display_language, &p_device->cls_flag);
                    }
                    if(p_device->key == BUTTON_EVENT_CLICK)
                    {
                        gui_display_time = Api_GetUiDisplayTime();
                        p_task->task_controller.keep_time = gui_display_time;
                        p_task->task_controller.run_time = 0;
                        p_movement->face_num = 0;
                    }
                    else
                    {
                        p_movement->face_num = 0;
                    }
                    
                    p_device->key = BUTTON_EVENT_NULL;
          
                }
//                else if(((p_movement->face_num == 2) || (p_movement->face_num == 3))
//                       && (p_task->task_controller.run_time > gui_display_time))
//                {
//                    if(p_device->key == BUTTON_EVENT_CLICK)
//                    {
//                        gui_display_time = Api_GetUiDisplayTime();
//                        if(p_task->task_controller.run_time >= gui_display_time)
//                        {
//                            p_task->task_controller.keep_time = gui_display_time + 2;
//                            p_task->task_controller.run_time = 0;
//                            p_movement->face_num = 1;
//                        }
//                        else
//                        {
//                            p_task->task_controller.keep_time = gui_display_time;
//                            p_task->task_controller.run_time = 0;
//                            p_movement->face_num = 2;
//                        }
//                    }
//                    else
//                    {
//                        p_movement->face_num = 0;
//                    }
//                    p_device->key = BUTTON_EVENT_NULL;
////					if(p_task->child_task_id != 3)
////					{
////						p_task->child_task_id = 3;
////                        p_task->gui_controller.animation_frame_index = 0;
////					}

//                    Gui_PageRunModeData(p_task->gui_controller.animation_frame_index++,movement_info_s.run_vail_time, movement_info_s.ui_heart, part_distance_m, calories, 
//                                        movement_info_s.ui_step, p_flash_param->dev_config_s.english_style_enable, p_movement->run, display_language, &p_device->cls_flag, 
//                                        runmode_show_type,p_movement->face_num,runmode_type, hour, clock->minutes, Operation_state, p_movement->run,  Speed_distance);      
//                }
                else
                {
                    if(p_movement->face_num == 1)
                    {
						if(p_task->child_task_id != 3)
						{
							p_task->child_task_id = 3;
                            p_task->gui_controller.animation_frame_index = 0;
						}
						Gui_PageRunModeData(p_task->gui_controller.animation_frame_index++,movement_info_s.run_vail_time, movement_info_s.ui_heart, part_distance_m, calories, 
                                            movement_info_s.ui_step, p_flash_param->dev_config_s.english_style_enable, p_movement->run, display_language, &p_device->cls_flag, 
                                            runmode_show_type,p_movement->face_num,runmode_type, hour, clock->minutes, Operation_state, p_movement->run,  Speed_distance);             
                    }
                    else if(p_movement->face_num == 2)
                    {
                        if(p_movement->run == 1)
                        {
							if(p_task->child_task_id != 4)
							{
								p_task->child_task_id = 4;
                                p_task->gui_controller.animation_frame_index = 0;
							}
							//Gui_PageRunModePause(p_task->gui_controller.animation_frame_index++, 0, display_language, &p_device->cls_flag);
                        }
                        else
                        {
							if(p_task->child_task_id != 5)
							{
								p_task->child_task_id = 5;
                                p_task->gui_controller.animation_frame_index = 0;
							}
							//Gui_PageRunModePause(p_task->gui_controller.animation_frame_index++, 1,display_language, &p_device->cls_flag);
                        }
						Gui_PageRunModeData(p_task->gui_controller.animation_frame_index++,movement_info_s.run_vail_time, movement_info_s.ui_heart, part_distance_m, calories, 
                                            movement_info_s.ui_step, p_flash_param->dev_config_s.english_style_enable, p_movement->run, display_language, &p_device->cls_flag, 
                                            runmode_show_type,p_movement->face_num,runmode_type, hour, clock->minutes, Operation_state, p_movement->run,  Speed_distance);                         
                    }
                    else if(p_movement->face_num == 3)
                    {
						if(p_task->child_task_id != 6)
						{
							p_task->child_task_id = 6;
                            p_task->gui_controller.animation_frame_index = 0;
						}
						//Gui_PageRunModeExit(p_task->gui_controller.animation_frame_index++, display_language, &p_device->cls_flag);
						Gui_PageRunModeData(p_task->gui_controller.animation_frame_index++,movement_info_s.run_vail_time, movement_info_s.ui_heart, part_distance_m, calories, 
                                            movement_info_s.ui_step, p_flash_param->dev_config_s.english_style_enable, p_movement->run, display_language, &p_device->cls_flag, 
                                            runmode_show_type,p_movement->face_num,runmode_type, hour, clock->minutes, Operation_state, p_movement->run,  Speed_distance);                         
                    }
                }
            }
        }
	}
    
#if LED_ON_OFF_ON_DEF
    //YX.20180830.增加延时，去掉显示的闪屏问题
    if(p_task->gui_controller.animation_tick_counter == 0)
    {
        Gui_LcdLedOn();
    } 
#endif
    if((p_task->gui_controller.repaint_enable == 1)&&(p_device->key != BUTTON_EVENT_NULL))
    {
//        if((p_device->key == BUTTON_EVENT_LONGPRESS)&&(p_movement->enable == 1)&&(p_movement->face_num == 3))
//        {
//            Movement_Exit();
//        }
//        else if((p_device->key == BUTTON_EVENT_LONGPRESS)&&(p_movement->enable == 1)&&(p_movement->face_num == 2))
//        {
//            if(p_movement->run == 1)
//            {
//                Movement_Pause();
//                p_movement->run = 0;
//            }
//            else
//            {
//                Movement_Start();
//                p_movement->run = 1;
//            }
//            
//            p_task->gui_controller.animation_frame_index = 0xFFFE;
//            MotorAddEvent(1,0,1,0);//提醒1组 振动1秒
//        }
        if((p_device->key == BUTTON_EVENT_CLICK)&&(p_movement->enable == 1)&&(p_movement->face_num == 1))
        {
            if(runmode_show_type == 0)  //1页的显示数据
            {
                gui_display_time = Api_GetUiDisplayTime();
                p_task->task_controller.keep_time = gui_display_time+1;
                p_task->task_controller.run_time = 0;
                p_movement->face_num = 2;                 //操作界面 （暂停 返回 结束 ）
                p_task->task_controller.key_press_hold = 1;
                Operation_state = 0;
            }
            else if(runmode_show_type == 1) //2页的显示数据
            {
                gui_display_time = Api_GetUiDisplayTime();
                p_task->task_controller.keep_time = gui_display_time+1;
                p_task->task_controller.run_time = 0;
                p_movement->face_num = 2;                //数据界面2 （计步、里程 配速 ）
//                p_task->task_controller.key_press_hold = 1;
            }

        }
        else if((p_device->key == BUTTON_EVENT_CLICK)&&(p_movement->enable == 1)
                &&(p_movement->face_num == 2) && ( runmode_show_type == 1))
        {
            gui_display_time = Api_GetUiDisplayTime();
            p_task->task_controller.keep_time = gui_display_time+1;
            p_task->task_controller.run_time = 0;
            p_movement->face_num = 3;                //
            p_task->task_controller.key_press_hold = 1;
            
//            gui_display_time = Api_GetUiDisplayTime();
//            p_task->task_controller.keep_time = gui_display_time+1;
//            p_task->task_controller.run_time = 0;
//            p_movement->face_num = 3;
//            p_task->task_controller.key_press_hold = 1;
        }
//        else if((p_device->key == BUTTON_EVENT_CLICK)&&(p_movement->enable == 1)&&(p_movement->face_num == 3))
//        {
//            gui_display_time = Api_GetUiDisplayTime();
//            p_task->task_controller.keep_time = gui_display_time+2;
//            p_task->task_controller.run_time = 0;
//            p_movement->face_num = 1;
//            p_task->task_controller.key_press_hold = 0;
//        }
        else if((p_device->key == BUTTON_EVENT_CLICK)&&(p_movement->enable == 1)&&(p_movement->face_num == 0))
        {
            gui_display_time = Api_GetUiDisplayTime();
            p_task->task_controller.keep_time = gui_display_time+2;
            p_task->task_controller.run_time = 0;
            p_movement->face_num = 1;
            p_task->task_controller.key_press_hold = 0;
        }
        /********************************操作界面（暂停 返回 结束 ****************************************/
        else if(((p_device->key == BUTTON_EVENT_CLICK) &&(Operation_state == 2))  
                && (((runmode_show_type == 0) && (p_movement->face_num = 2)) || ((runmode_show_type == 1) && (p_movement->face_num = 3))))
        {
            Operation_state = 0;
            p_task->task_controller.key_press_hold = 1;
            gui_display_time = Api_GetUiDisplayTime();
            p_task->task_controller.keep_time = gui_display_time+1;
            p_task->task_controller.run_time = 0;
        }
        else if(((p_device->key == BUTTON_EVENT_CLICK) &&(Operation_state == 1))   
                && (((runmode_show_type == 0) && (p_movement->face_num = 2)) || ((runmode_show_type == 1) && (p_movement->face_num = 3))))
        {
            Operation_state = 2;
            p_task->task_controller.key_press_hold = 1;
            gui_display_time = Api_GetUiDisplayTime();
            p_task->task_controller.keep_time = gui_display_time+1;
            p_task->task_controller.run_time = 0;
        }
        else if(((p_device->key == BUTTON_EVENT_CLICK) &&(Operation_state == 0))    
                && (((runmode_show_type == 0) && (p_movement->face_num = 2)) || ((runmode_show_type == 1) && (p_movement->face_num = 3))))
        {
            Operation_state = 1;
            p_task->task_controller.key_press_hold = 1;
            gui_display_time = Api_GetUiDisplayTime();
            p_task->task_controller.keep_time = gui_display_time+1;
            p_task->task_controller.run_time = 0;
        }
        else if(((p_device->key == BUTTON_EVENT_LONGPRESS) && (Operation_state == 0))
                && (((runmode_show_type == 0) && (p_movement->face_num = 2)) || ((runmode_show_type == 1) && (p_movement->face_num = 3))))
        {
            if(p_movement->run == 1)
            {
                Movement_Pause();             
                p_movement->run = 0;
            }
            else
            {
                Movement_Start();
                p_movement->run = 1;
            }
            p_task->gui_controller.animation_frame_index = 0xFFFE;
            MotorAddEvent(1,0,1,0);//提醒1组 振动1秒
            p_task->task_controller.key_press_hold = 0;
            Operation_state = 0;
        }
        else if(((p_device->key == BUTTON_EVENT_LONGPRESS) && (Operation_state == 1))
                && (((runmode_show_type == 0) && (p_movement->face_num = 2)) || ((runmode_show_type == 1) && (p_movement->face_num = 3))))
        {
            gui_display_time = Api_GetUiDisplayTime();
            p_task->task_controller.keep_time = gui_display_time+1;
            p_task->task_controller.run_time = 0;
            p_movement->face_num = 1;
            p_task->task_controller.key_press_hold = 1;
            Operation_state = 0;
        }
        else if(((p_device->key == BUTTON_EVENT_LONGPRESS) && (Operation_state == 2))
                && (((runmode_show_type == 0) && (p_movement->face_num = 2)) || ((runmode_show_type == 1) && (p_movement->face_num = 3))))
        {
//            gui_display_time = Api_GetUiDisplayTime();
//            p_task->task_controller.keep_time = gui_display_time;
//            p_task->task_controller.run_time = 0;
//            p_task->task_controller.active = 1;
//            //重置运行时间
//            p_task->task_controller.run_time = 0;
            Cpu_ScheduleTask(p_task, p_device, CPU_TASK_MOVEMENT_RESULT);
            p_task->task_controller.init = 1;
            Operation_state = 0;
        }
        /*********************************************************************************************************/
        else if(p_movement->enable == 0)
        {
            Cpu_ScheduleTask(p_task, p_device, CPU_TASK_ECG);  
            memset(p_movement,0,sizeof(movement_cont_t));
            Operation_state = 0;            
        }
        p_device->key = BUTTON_EVENT_NULL;	 
    }
}

/****************************************************************
** Function name:            Cpu_TaskRunSelect
** Descriptions:             多运动模式选择界面
** input parameters:         p_controller：       CPU任务控制器指针
                             p_gui：              静态页面控制器指针
                             p_oled：             oled驱动指针
** output parameters:        无
** Returned value:           无
** Created by:               YX
** Created Date:             2017-11-14
****************************************************************/
void  Cpu_TaskRunDataResult(cpu_task_tcb_t* p_task, cpu_device_tcb_t* p_device, movement_cont_t* p_movement)
{
    uint8_t slid_ret = 0; 
    uint8_t uint_type = 0;    
    uint32_t   distance_m = 0;
    float distance_m1 = 0;    
    float calories = 0;
    uint32_t seconds = 0;
    uint32_t Speed_distance = 0;
    static uint8_t dis_time = 0;
    if(p_task->task_controller.init == 1)
    {
        Cpu_TaskParamInit(p_task,gui_display_time,false,true);
        p_task->child_task_id = 1;
        dis_time = 0;        
    }

#if LED_ON_OFF_ON_DEF
    if(p_task->gui_controller.animation_tick_counter == 0)
    {
        Gui_LcdLedOff();
    } 
#endif


    if(p_flash_param->dev_config_s.english_style_enable == 1)    //英里
    {
        distance_m1 = ((uint32_t)((movement_info_s.ui_distance / 1000.0f) * 1000)) / 1000.0f;//保留三位小数点
        distance_m1 = distance_m1 * (0.62138f);
    } 
    else 
    {
        distance_m1 = movement_info_s.ui_distance / 1000.0f;
    }
    uint_type =  p_flash_param->dev_config_s.english_style_enable;
    seconds =  movement_info_s.run_vail_time;
    calories = movement_info_s.ui_calories/1000.0f;
    Speed_distance =  (uint32_t)(movement_info_s.ui_distance / 1000.0f);
    distance_value(&all_step,&distance_m,&distance_km,&distance_mile,&calories_buf);
    //NOTE 
	uint8_t data_valid = (seconds >= MOVEMENT_MIN_VALID_TIME);
	if(((p_task->gui_controller.animation_tick_counter%3) == 0))
	{    
        Gui_PageRunModeDataResult(p_task->gui_controller.animation_frame_index++, data_valid, seconds, movement_info_s.ui_heart, distance_m1, calories, all_step, 
								 uint_type, 0, 0, 0, 0, display_language, runmode_type - 1, runmode_show_type, current_page,
                                 Speed_distance);
        dis_time++;
        if(dis_time >= 20)
        {
            dis_time = 0;
            Movement_Exit();
            Cpu_ScheduleTask(p_task, p_device, CPU_TASK_MAIN);
//            p_task->task_controller.init = 1;
            p_task->child_task_id = 2;
        }
	}
    
#if LED_ON_OFF_ON_DEF
    //YX.20180830.增加延时，去掉显示的闪屏问题
    if(p_task->gui_controller.animation_tick_counter == 0)
    {
        Gui_LcdLedOn();
    } 
#endif
    if(p_task->gui_controller.repaint_enable == 1)
    {
        if(p_device->key == BUTTON_EVENT_CLICK)
        {

        }
        else if(p_device->key == BUTTON_EVENT_LONGPRESS)
        {   
            
        }
        p_device->key = BUTTON_EVENT_NULL;
    }  
}

/****************************************************************
** Function name:            Cpu_StepTask
** Descriptions:             记步界面
** input parameters:         p_controller：       CPU任务控制器指针
                             p_gui：              静态页面控制器指针
                             p_oled：             oled驱动指针
** output parameters:        无
** Returned value:           无
** Created by:               GT
** Created Date:             2017-07-10
****************************************************************/
void Cpu_StepTask(cpu_task_tcb_t* p_task, cpu_device_tcb_t* p_device)
{    
//    uint8_t cls_flag_tmp = p_device->cls_flag;
    uint8_t hour = 0;
    float  distance_km = 0.0f;
    float  distance_mile = 0.0f;
    if(p_task->task_controller.init == 1)
    {
        Cpu_TaskParamInit(p_task,gui_display_time,false,true);
        p_task->child_task_id = 1;      
    }

#if LED_ON_OFF_ON_DEF
    if(p_task->gui_controller.animation_tick_counter == 0)
    {
        Gui_LcdLedOff();
    } 
#endif
 
    if(((p_task->gui_controller.animation_tick_counter%3) == 0)&&(p_task->gui_controller.repaint_enable == 1))
    {
        if(p_task->child_task_id != 1)
		{
			p_task->gui_controller.animation_frame_index = 0;
			p_task->child_task_id = 1;
		}
        distance_value(&all_step,&distance_m,&distance_km,&distance_mile,&calories_buf);
        hour = Hour_StandardTransform(p_flash_param->dev_config_s.am_pm_enable,clock->hour);
        if(p_flash_param->dev_config_s.english_style_enable == 0)
        {
            Gui_PageStep(all_step, Gui_DataGetSportsToMarkProgress(), 
                     p_task->gui_controller.animation_frame_index++, hour,
                     clock->minutes, display_language, &p_device->cls_flag,
                     distance_km, 0, (uint16_t)(calories_buf/1000.0f));
                       
        }
        else
        {
           Gui_PageStep(all_step, Gui_DataGetSportsToMarkProgress(), 
                     p_task->gui_controller.animation_frame_index++, hour,
                     clock->minutes, display_language, &p_device->cls_flag,
                     distance_mile, 1, (uint16_t)(calories_buf/1000.0f)); 
        
        }
 
    } 
    
#if LED_ON_OFF_ON_DEF
    //YX.20180830.增加延时，去掉显示的闪屏问题
    if(p_task->gui_controller.animation_tick_counter == 0)
    {
         
        Gui_LcdLedOn();
    } 
#endif
  
    if(p_task->gui_controller.repaint_enable == 1)
    {
        if(p_device->key == BUTTON_EVENT_CLICK)
        {
			Cpu_ScheduleTask(p_task, p_device, CPU_TASK_SLEEP);
        }
        p_device->key = BUTTON_EVENT_NULL;
    }
}

/****************************************************************
** Function name:            Cpu_DistanTask
** Descriptions:             距离界面
** input parameters:         p_controller：       CPU任务控制器指针
                             p_gui：              静态页面控制器指针
                             p_oled：             oled驱动指针
** output parameters:        无
** Returned value:           无
** Created by:               GT
** Created Date:             2017-07-10
****************************************************************/
void Cpu_DistanTask(cpu_task_tcb_t* p_task, cpu_device_tcb_t* p_device)
{    
    uint8_t hour = 0;
    if(p_task->task_controller.init == 1)
    {
        Cpu_TaskParamInit(p_task,gui_display_time,false,true);
        p_task->child_task_id = 1;       
    }

#if LED_ON_OFF_ON_DEF
    if(p_task->gui_controller.animation_tick_counter == 0)
    {
        Gui_LcdLedOff();
    } 
#endif
 
    if(((p_task->gui_controller.animation_tick_counter%3) == 0)&&(p_task->gui_controller.repaint_enable == 1))
    {
        if(p_task->child_task_id != 1)
		{
//			//清除背景
//			Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
//									p_device->cls_param.column_size, p_device->cls_param.page_size);
			p_task->gui_controller.animation_frame_index = 0;
			p_task->child_task_id = 1;
		}
  
        distance_value(&all_step,&distance_m,&distance_km,&distance_mile,&calories_buf);
        hour = Hour_StandardTransform(p_flash_param->dev_config_s.am_pm_enable,clock->hour);
        if(p_flash_param->dev_config_s.english_style_enable == 0)
        {
			Gui_PageDistance(distance_km,p_task->gui_controller.animation_frame_index++,p_flash_param->dev_config_s.english_style_enable, hour, clock->minutes,display_language, &p_device->cls_flag);
        }
        else
        {
			Gui_PageDistance(distance_mile,p_task->gui_controller.animation_frame_index++,p_flash_param->dev_config_s.english_style_enable, hour, clock->minutes,display_language, &p_device->cls_flag);
        }
    } 
#if LED_ON_OFF_ON_DEF
    //YX.20180830.增加延时，去掉显示的闪屏问题
    if(p_task->gui_controller.animation_tick_counter == 0)
    {
         
        Gui_LcdLedOn();
    } 
#endif
  
    if(p_task->gui_controller.repaint_enable == 1)
    {
        if(p_device->key == BUTTON_EVENT_CLICK)
        {
            Cpu_ScheduleTask(p_task, p_device, CPU_TASK_HEART);
        }
        p_device->key = BUTTON_EVENT_NULL;
    }
}

/****************************************************************
** Function name:            Cpu_CalorTask
** Descriptions:             卡路里界面
** input parameters:         p_controller：       CPU任务控制器指针
                             p_gui：              静态页面控制器指针
                             p_oled：             oled驱动指针
** output parameters:        无
** Returned value:           无
** Created by:               GT
** Created Date:             2017-07-10
****************************************************************/
void Cpu_CalorTask(cpu_task_tcb_t* p_task, cpu_device_tcb_t* p_device)
{    
////////    uint8_t cls_flag_tmp = p_device->cls_flag;
//////    uint8_t hour = 0;
//////    
//////    if(p_task->task_controller.init == 1)
//////    {
//////        Cpu_TaskParamInit(p_task,gui_display_time,false,true);
//////        p_task->child_task_id = 1;
////////        //清除背景
////////        Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
////////                                p_device->cls_param.column_size, p_device->cls_param.page_size);        
//////    }

//////#if LED_ON_OFF_ON_DEF
//////    if(p_task->gui_controller.animation_tick_counter == 0 && cls_flag_tmp != 2)
//////    {
//////        Gui_LcdLedOff();
//////    } 
//////#endif
////// 
//////    if(((p_task->gui_controller.animation_tick_counter%5) == 0)&&(p_task->gui_controller.repaint_enable == 1))
//////    {
//////        if(p_task->child_task_id != 1)
//////		{
////////			//清除背景
////////			Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
////////									p_device->cls_param.column_size, p_device->cls_param.page_size);

//////            p_task->gui_controller.animation_frame_index = 0;
//////			p_task->child_task_id = 1;
//////		}
//////		
//////         
////////         
//////        distance_value(&all_step,&distance_m,&distance_km,&distance_mile,&calories_buf);
//////        hour = Hour_StandardTransform(p_flash_param->dev_config_s.am_pm_enable,clock->hour);
//////		Gui_PageCalories(calories_buf/1000.0f,p_task->gui_controller.animation_frame_index++, hour, clock->minutes,display_language, &p_device->cls_flag); 
//////        
//////    } 
//////        
//////#if LED_ON_OFF_ON_DEF
//////    //YX.20180830.增加延时，去掉显示的闪屏问题
//////    if(p_task->gui_controller.animation_tick_counter == 0 && cls_flag_tmp != 2)
//////    {
//////         
//////        Gui_LcdLedOn();
//////    } 
//////#endif
//////  
//////    if(p_task->gui_controller.repaint_enable == 1)
//////    {
//////        if(p_device->key == BUTTON_EVENT_CLICK)
//////        {
//////            Cpu_ScheduleTask(p_task, p_device, CPU_TASK_DISTAN);
//////        }
//////        p_device->key = BUTTON_EVENT_NULL;
//////    }
}

/****************************************************************
** Function name:            Cpu_HeartTask
** Descriptions:             心跳界面
** input parameters:         p_controller：       CPU任务控制器指针
                             p_gui：              静态页面控制器指针
                             p_oled：             oled驱动指针
** output parameters:        无
** Returned value:           无
** Created by:               GT
** Created Date:             2017-07-10
****************************************************************/
void Cpu_HeartTask(cpu_task_tcb_t* p_task, cpu_device_tcb_t* p_device)
{
//    uint8_t cls_flag_tmp = p_device->cls_flag;
    static uint8_t heart = 0;
    uint8_t hour = 0;
    
    if(p_task->task_controller.init == 1)
    {								
		// 初始化其他变量
		p_task->child_task_id = 1;
		if(Battery_Get_Grade() == 0)
		{
            p_data_manage->wear_measure_s.wear_status = 0;
			p_data_manage->ecg_ppg_s.ppg_measure_s.btn = 0;
			Cpu_TaskParamInit(p_task, gui_display_time, false, true);	      
		}
		else
		{
            Statemachine_AddState(MEAT_PPG_BTN_HEART);
			p_data_manage->ecg_ppg_s.ppg_measure_s.btn = 1;
            //WJ.20181214.用于app回复
            p_data_manage->ecg_ppg_s.ppg_measure_s.bp_progress = 201;  
			
			Cpu_TaskParamInit(p_task, 60, false, true);
		}
        heart = 0;
//        //清除背景
//        Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
//                                p_device->cls_param.column_size, p_device->cls_param.page_size);
    } 

#if LED_ON_OFF_ON_DEF
    if(p_task->gui_controller.animation_tick_counter == 0)
    {
        Gui_LcdLedOff();
    } 
#endif
 
    if((p_task->task_controller.run_time >= 59) || (p_data_manage->wear_measure_s.wear_status == 1))
    {
        Statemachine_DelState(MEAT_PPG_BTN_HEART);
        p_data_manage->ecg_ppg_s.ppg_measure_s.btn = 0;
        memset(p_task, 0, sizeof(cpu_task_tcb_t));
    }

    // 修改相关变量 
    if(((p_task->gui_controller.animation_tick_counter%3) == 0)&&(p_task->gui_controller.repaint_enable == 1))
    {
        if(p_task->child_task_id != 1)
		{
//			//清除背景
//			Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
//									p_device->cls_param.column_size, p_device->cls_param.page_size);

            p_task->gui_controller.animation_frame_index = 0;
			p_task->child_task_id = 1;
		}
		 
//         
        
        if(p_data_manage->ecg_ppg_s.ppg_measure_s.btn == 1
           && p_data_manage->ecg_ppg_s.ppg_measure_s.init == 0
           && p_data_manage->ecg_ppg_s.ppg_measure_s.p_second_heart != 0)
        {
            if(p_data_manage->ecg_ppg_s.ppg_measure_s.p_second_heart >= 30
               && p_data_manage->ecg_ppg_s.ppg_measure_s.p_second_heart <= 180)
            {
                heart = p_data_manage->ecg_ppg_s.ppg_measure_s.p_second_heart;
            }
        }
        hour = Hour_StandardTransform(p_flash_param->dev_config_s.am_pm_enable,clock->hour);
        
		Gui_PageHeart(p_task->gui_controller.animation_frame_index++, heart, heart, hour, clock->minutes, display_language, &p_device->cls_flag);
    } 
    
#if LED_ON_OFF_ON_DEF
    //YX.20180830.增加延时，去掉显示的闪屏问题
    if(p_task->gui_controller.animation_tick_counter == 0)
    {
         
        Gui_LcdLedOn();
    } 
#endif
  
    if(p_task->gui_controller.repaint_enable == 1)
    {
        if(p_device->key == BUTTON_EVENT_CLICK)
        {
            if(p_data_manage->ecg_ppg_s.ppg_measure_s.btn == 1)
            {
                Statemachine_DelState(MEAT_PPG_BTN_HEART);
            }
            p_data_manage->ecg_ppg_s.ppg_measure_s.btn = 0;
            
            if(p_flash_param->bp_func_state == 0x01)
            {
                Cpu_ScheduleTask(p_task, p_device, CPU_TASK_BP);
            }
            else
            {
                Cpu_ScheduleTask(p_task, p_device, CPU_TASK_SLEEP);
            }
        }
        p_device->key = BUTTON_EVENT_NULL;
    }    
}

/****************************************************************
** Function name:            Cpu_SleepTask
** Descriptions:             睡眠界面
** input parameters:         p_controller：       CPU任务控制器指针
                             p_gui：              静态页面控制器指针
                             p_oled：             oled驱动指针
** output parameters:        无
** Returned value:           无
** Created by:               GT
** Created Date:             2017-07-10
****************************************************************/
void Cpu_SleepTask(cpu_task_tcb_t* p_task, cpu_device_tcb_t* p_device)
{
//    uint8_t cls_flag_tmp = p_device->cls_flag;
    uint8_t hour = 0;
    
    static uint8_t quality = 0;
    static uint16_t all_time = 0;
    static uint16_t shallow_sleep_time = 0;
    static uint16_t deep_sleep_time = 0;
    
    if(p_task->task_controller.init == 1)
    {
        Cpu_TaskParamInit(p_task,gui_display_time,true,true);
        p_task->child_task_id = 1;
//        //清除背景
//        Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
//                                p_device->cls_param.column_size, p_device->cls_param.page_size);
        
        // !注意:进入睡眠界面需要遍历睡眠算法,条件(8点后并且没有睡眠)
        if(clock->hour >= 8)//&&(Api_GetSleepTotalTime() == 0))
        {
           // AlgThread_SleepTraversing();
        }
        // 需要获取具体数据     
        all_time = p_data_manage->sleep_result_s.sleep_all_time;
        shallow_sleep_time = p_data_manage->sleep_result_s.sleep_shallow_time;
        deep_sleep_time = p_data_manage->sleep_result_s.sleep_deep_time;
        quality = p_data_manage->sleep_result_s.sleep_quality; 
        if(all_time != 0)
        {   // WJ.20190611.修正科学睡眠睡眠质量的对应关系（科学睡眠等级为0~4，普通睡眠为1~5）
            quality += 1;
        }        
        
    }    

#if LED_ON_OFF_ON_DEF
    if(p_task->gui_controller.animation_tick_counter == 0)
    {
        Gui_LcdLedOff();
    } 
#endif
 
    // 客户要求刷新速率为帧/0.3s
    if(((p_task->gui_controller.animation_tick_counter%3) == 0)&&(p_task->gui_controller.repaint_enable == 1))
    {
        if(p_task->child_task_id != 1)
		{
//			//清除背景
//			Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
//									p_device->cls_param.column_size, p_device->cls_param.page_size);

            p_task->gui_controller.animation_frame_index = 0;
			p_task->child_task_id = 1;
		}
 
        hour = Hour_StandardTransform(p_flash_param->dev_config_s.am_pm_enable,clock->hour);
		Gui_PageSleep(p_task->gui_controller.animation_frame_index++,all_time, hour, clock->minutes,display_language, &p_device->cls_flag,
         deep_sleep_time, shallow_sleep_time,  quality);
        
    } 
    
#if LED_ON_OFF_ON_DEF
    //YX.20180830.增加延时，去掉显示的闪屏问题
    if(p_task->gui_controller.animation_tick_counter == 0)
    {
         
        Gui_LcdLedOn();
    } 
#endif
  
    // 按键短按,切换到下一页
    if(p_task->gui_controller.repaint_enable == 1)
    {
        if(p_device->key == BUTTON_EVENT_CLICK)
        {
            Cpu_ScheduleTask(p_task, p_device, CPU_TASK_HEART);
        }
        else if(p_device->key == BUTTON_EVENT_LONGPRESS)
        {
            Api_UpdateConnectPassword(0);
            MotorAddEvent(1,0,1,0);
        }
            
        p_device->key = BUTTON_EVENT_NULL;
    }   
}

/****************************************************************
** Function name:            Cpu_FindPhoneTask
** Descriptions:             查找手机界面
** input parameters:         p_controller：       CPU任务控制器指针
                             p_gui：              静态页面控制器指针
                             p_oled：             oled驱动指针
** output parameters:        无
** Returned value:           无
** Created by:               GT
** Created Date:             2017-07-10
****************************************************************/
void Cpu_FindPhoneTask(cpu_task_tcb_t* p_task, cpu_device_tcb_t* p_device)
{
//    uint8_t cls_flag_tmp = p_device->cls_flag;
    static uint8_t find_open_flag = 0;
    static uint8_t disp_time = 0;
    uint8_t hour = 0;
    
    if(p_task->task_controller.init == 1)
    {
        Cpu_TaskParamInit(p_task, gui_display_time, true, true);
        p_task->child_task_id = 1;        
        p_data_manage->device_trace_param_s.during_findphone = 1;
//        //清除背景
//        Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
//                                p_device->cls_param.column_size, p_device->cls_param.page_size);
        find_open_flag = 1;
        p_task->task_controller.keep_time = 60;
        p_task->task_controller.run_time = 0;
        p_task->gui_controller.animation_enable = 1;
        p_task->gui_controller.animation_frame_index = 0;
        p_task->gui_controller.animation_tick_counter = 0xFFFE;
    }

#if LED_ON_OFF_ON_DEF
    if(p_task->gui_controller.animation_tick_counter == 0)
    {
        Gui_LcdLedOff();
    } 
#endif
    if(p_task->gui_controller.repaint_enable == 1 && p_data_manage->device_trace_param_s.ble_connect == 1)
    {
        // 客户需求: 帧/0.3S刷新速率 
        if((p_task->gui_controller.animation_tick_counter%3) == 0)
        { 		 
            if(find_open_flag != 1)
            {
                if(p_task->child_task_id != 1)
                {
//                    //清除背景
//                    Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
//                                            p_device->cls_param.column_size, p_device->cls_param.page_size);
                    p_task->child_task_id = 1;
                    p_task->gui_controller.animation_frame_index = 0;
                    p_data_manage->device_trace_param_s.during_findphone = 0;
                }
            }
            else
            {
                if(p_task->child_task_id != 2)
                {
//                    //清除背景
//                    Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
//                                            p_device->cls_param.column_size, p_device->cls_param.page_size);
                    p_task->child_task_id = 2;
                    p_task->gui_controller.animation_frame_index = 0;
                }
                p_data_manage->device_trace_param_s.during_findphone = 1;
            }
            hour = Hour_StandardTransform(p_flash_param->dev_config_s.am_pm_enable,clock->hour);
            Gui_PageFindPhone(find_open_flag, p_task->gui_controller.animation_frame_index++, hour, clock->minutes, display_language, &p_device->cls_flag);  //find_open_flag
        }
								
        if(((p_task->gui_controller.animation_tick_counter%95) == 0)&&(find_open_flag == 1))
        {
            Ble_SendBatMulData(2,0xB5,0);
        }
    } 
    if(p_data_manage->device_trace_param_s.ble_connect == 0)
    {
       Gui_PageBleUnconnected(p_task->gui_controller.animation_frame_index++,display_language);
       if((p_task->gui_controller.animation_tick_counter%4) == 0)
       {
            disp_time++;
       }
       if(disp_time >=10 )
       {
          disp_time = 0;
          Cpu_ScheduleTask(p_task, p_device, CPU_TASK_FUNCT2);           
       }
        
    }        
#if LED_ON_OFF_ON_DEF
    //YX.20180830.增加延时，去掉显示的闪屏问题
    if(p_task->gui_controller.animation_tick_counter == 0)
    {
         
        Gui_LcdLedOn();
    } 
#endif
  
    if((p_task->gui_controller.repaint_enable == 1)&&(p_device->key == BUTTON_EVENT_CLICK))
    {
        Cpu_ScheduleTask(p_task, p_device, CPU_TASK_FUNCT2);           
    }
    
    if((p_task->gui_controller.repaint_enable == 1)
      &&(p_device->key == BUTTON_EVENT_LONGPRESS)
      &&(find_open_flag != 1))
    {
        p_device->key = BUTTON_EVENT_NULL;
        p_task->task_controller.keep_time = 60;
        p_task->task_controller.run_time = 0;
        p_task->gui_controller.animation_enable = 1;
        p_task->gui_controller.animation_frame_index = 0;
        p_task->gui_controller.animation_tick_counter = 0xFFFE;
        find_open_flag = 1;
    }
}

/****************************************************************
** Function name:            Cpu_PowerOffTask
** Descriptions:             关机界面
** input parameters:         p_controller：       CPU任务控制器指针
                             p_gui：              静态页面控制器指针
                             p_oled：             oled驱动指针
** output parameters:        无
** Returned value:           无
** Created by:               GT
** Created Date:             2017-07-10
****************************************************************/
void Cpu_PowerOffTask(cpu_task_tcb_t* p_task, cpu_event_tcb_t *p_event, cpu_device_tcb_t* p_device)
{
//    uint8_t cls_flag_tmp = p_device->cls_flag;
    uint8_t hour = 0;
    static uint8_t dis_mac_data[6] = {0};
    static uint8_t dis_ver_data[2] = {0};
    static uint8_t BleName[10] = {0};
    if(p_task->task_controller.init == 1)
    {
        Cpu_TaskParamInit(p_task,gui_display_time,true,true);//false);
        Gui_DataGetBleMac(dis_mac_data);
        Gui_DataGetVer(dis_ver_data);
        p_task->child_task_id = 1;
//        //清除背景
//        Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
//                                p_device->cls_param.column_size, p_device->cls_param.page_size);
    }

#if LED_ON_OFF_ON_DEF
    if(p_task->gui_controller.animation_tick_counter == 0)
    {
        Gui_LcdLedOff();
    } 
#endif
 
    if((p_task->gui_controller.animation_tick_counter%6 == 0)&&(p_task->gui_controller.repaint_enable == 1))
    {
        if(p_task->child_task_id != 1)
		{
//			//清除背景
//			Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
//									p_device->cls_param.column_size, p_device->cls_param.page_size);

            p_task->gui_controller.animation_frame_index = 0;
			p_task->child_task_id = 1;
		}
		Gui_DataGetBleMac(dis_mac_data);
        Gui_DataGetVer(dis_ver_data);
        Gui_DataGetBleName(BleName);        
        hour = Hour_StandardTransform(p_flash_param->dev_config_s.am_pm_enable,clock->hour);
		Gui_PagePower(p_task->gui_controller.animation_frame_index++, dis_mac_data, dis_ver_data, hour, clock->minutes, display_language, &p_device->cls_flag,BleName);
    } 
    
#if LED_ON_OFF_ON_DEF
    //YX.20180830.增加延时，去掉显示的闪屏问题
    if(p_task->gui_controller.animation_tick_counter == 0)
    {
         
        Gui_LcdLedOn();
    } 
#endif
  
    // 按键单击任务处理
    if(p_task->gui_controller.repaint_enable == 1)
    {
        if(p_device->key == BUTTON_EVENT_CLICK)
        {
            Cpu_ScheduleTask(p_task, p_device, CPU_TASK_MAIN);
        }
        else if(p_device->key == BUTTON_EVENT_LONGPRESS)
        {
            p_data_manage->device_trace_param_s.sign_on = 0;
            Cpu_DevicePowerOffEvent(p_task,p_event,p_device);
        }
        p_device->key = BUTTON_EVENT_NULL;
    }      
}

/****************************************************************
** Function name:            Cpu_TimerTask
** Descriptions:             秒表界面
** input parameters:         
** output parameters:        无
** Returned value:           无
** Created by:               GT
** Created Date:             2017-07-10
****************************************************************/
void Cpu_TimerTask(cpu_task_tcb_t* p_task, watch_timer_t *p_timer, cpu_device_tcb_t* p_device)
{
//    uint8_t cls_flag_tmp = p_device->cls_flag;
    static uint8_t last_ui_style = 0;
    uint8_t time_format = 0;
    uint8_t hour = 0;
    uint32_t timer_msecond = 0;
    
    if(p_task->task_controller.init == 1)
    {
        // 对秒表参数进行初始化
        p_timer->entry = 1;
        Cpu_TaskParamInit(p_task,gui_display_time,true,true);
        p_task->child_task_id = 1;
//        //清除背景
//        Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
//                                p_device->cls_param.column_size, p_device->cls_param.page_size);	    
    }

#if LED_ON_OFF_ON_DEF
    if(p_task->gui_controller.animation_tick_counter == 0)
    {
        Gui_LcdLedOff();
    } 
#endif
 
    if(p_timer->entry == 1)
    {
        if((p_task->gui_controller.animation_tick_counter%2 == 0)&&(p_task->gui_controller.repaint_enable == 1))
        {
            if(p_task->child_task_id != 1)
			{
//				//清除背景
//				Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
//										p_device->cls_param.column_size, p_device->cls_param.page_size);

                p_task->gui_controller.animation_frame_index = 0;
				p_task->child_task_id = 1;
			}
			 
            hour = Hour_StandardTransform(p_flash_param->dev_config_s.am_pm_enable, clock->hour);
			GUI_StopWatchPage(p_task->gui_controller.animation_frame_index++, false, 0, 0, 0, hour, clock->minutes, display_language, &p_device->cls_flag);
        }
    }
    else
    {
        // 秒表界面计时状态
        if(p_timer->run == 1)
        {
            if((p_task->gui_controller.animation_tick_counter%2 == 0)&&(p_task->gui_controller.repaint_enable == 1))
            {
                // 秒表界面推出状态
                if(((p_task->task_controller.keep_time == 4)&&(p_task->task_controller.run_time >= 3))
                   ||(p_task->task_controller.keep_time == 62)&&(p_task->task_controller.run_time >= 61)
                   ||(p_task->task_controller.keep_time == 5)&&(p_task->task_controller.run_time >= 4)
                   || (p_task->task_controller.keep_time == gui_display_time + 2)&&(p_task->task_controller.run_time >= gui_display_time))
                {         
                     
                    // 如果此时有按键按动,则需要返回秒表界面
                    if(p_device->key == BUTTON_EVENT_CLICK)
                    {
                        gui_display_time = Api_GetUiDisplayTime();
                        p_task->task_controller.keep_time = gui_display_time + 2;                        
//                        p_task->task_controller.keep_time = 5;
                        p_task->task_controller.run_time = 0;
                    }
                    p_device->key = BUTTON_EVENT_NULL;	
                    
                    if(p_task->child_task_id != 3)
					{
//						//清除背景
//						Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
//												p_device->cls_param.column_size, p_device->cls_param.page_size);
						p_task->child_task_id = 3;
						p_task->gui_controller.animation_frame_index = 0;
                        last_ui_style = ui_style_type;
					}
                    if(last_ui_style != ui_style_type)
                    {
                        last_ui_style = ui_style_type;
//                        //清除背景
//						Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
//												p_device->cls_param.column_size, p_device->cls_param.page_size);
                        p_task->gui_controller.animation_frame_index = 0;
                    }
                    
                    hour = Hour_StandardTransform(p_flash_param->dev_config_s.am_pm_enable,clock->hour);
                    time_format = Get_AMPM_Format(p_flash_param->dev_config_s.am_pm_enable,clock->hour);
                    distance_value(&all_step,&distance_m,&distance_km,&distance_mile,&calories_buf);
                    
                    task_interval_cnt = p_task->gui_controller.animation_frame_index++;

                    if(p_flash_param->dev_config_s.english_style_enable == 0)
                    {
                        Gui_PageMain(task_interval_cnt, clock->month, clock->day, hour, clock->minutes, distance_km, clock->week,
                                     time_format, all_step, 0, p_data_manage->device_trace_param_s.ble_connect, Battery_Get_Status(),
                                     Battery_Get_Grade(), 0, calories_buf, 0, 0, display_language, &p_device->cls_flag);
                    }
                    else
                    {
                        Gui_PageMain(task_interval_cnt, clock->month, clock->day, hour, clock->minutes, distance_mile, clock->week,
                                     time_format, all_step, 0, p_data_manage->device_trace_param_s.ble_connect, Battery_Get_Status(),
                                     Battery_Get_Grade(), 0, calories_buf, 0, 0, display_language, &p_device->cls_flag);
                    }
                }
                else
                {
                    if(p_timer->enable == 1)
                    {
                        timer_msecond = p_timer->total_msecond + (uint32_t)(xTaskGetTickCount()/10) - p_timer->stop_msecond;
                    }
                    else
                    {
                        timer_msecond = p_timer->total_msecond;
                    }
                    
                    // 这里需要判断秒表时间是否大于99:59:99 Add by GT 2017-09-27
                    if(timer_msecond >= 599999)
                    {
                        timer_msecond = 599999;
                    }
                    
                    if(p_task->child_task_id != 2)
					{
//						//清除背景
//						Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
//												p_device->cls_param.column_size, p_device->cls_param.page_size);
						p_task->child_task_id = 2;
						p_task->gui_controller.animation_frame_index = 0;
					}
                    hour = Hour_StandardTransform(p_flash_param->dev_config_s.am_pm_enable,clock->hour);
					GUI_StopWatchPage(p_task->gui_controller.animation_frame_index++, true, p_timer->enable, timer_msecond/100, timer_msecond%100, 
                                      hour, clock->minutes, display_language, &p_device->cls_flag);
                }
            }
        }
    }
    
#if LED_ON_OFF_ON_DEF
    //YX.20180830.增加延时，去掉显示的闪屏问题
    if(p_task->gui_controller.animation_tick_counter == 0)
    {
         
        Gui_LcdLedOn();
    } 
#endif
  
    if((p_task->gui_controller.repaint_enable == 1)&&(p_device->key != BUTTON_EVENT_NULL))
    {
        //如果当前界面在秒表界面,则需要处理如下逻辑
        if((p_device->key == BUTTON_EVENT_LONGPRESS)&&(p_timer->entry == 1))
        {
            // 启动秒表功能
            memset(p_timer,0,sizeof(watch_timer_t));
            p_timer->total_msecond = 0;
            p_timer->stop_msecond = (uint32_t)(xTaskGetTickCount()/10);
            p_timer->enable = 0;
            p_timer->run = 1;
        
            p_task->task_controller.keep_time = 62;
            p_task->task_controller.run_time = 0;
			p_task->task_controller.active = 1; 
        }
        else if((p_device->key == BUTTON_EVENT_LONGPRESS)&&(p_timer->run  == 1)&&(p_task->child_task_id != 3))
        {		 
            //跳转到主界面
            Cpu_ScheduleTask(p_task, p_device, CPU_TASK_MAIN);
            memset(p_timer,0,sizeof(watch_timer_t));
        }
        else if((p_device->key == BUTTON_EVENT_CLICK)&&(p_timer->run == 1)&&(p_task->child_task_id != 3))
        {
            if(p_timer->enable == 1)
            {           
                p_timer->enable = 0;
                p_timer->total_msecond += (uint32_t)(xTaskGetTickCount()/10) - p_timer->stop_msecond;
            }
            else
            {
                p_timer->enable = 1;
                p_timer->stop_msecond = (uint32_t)(xTaskGetTickCount()/10);
            }
            if(p_task->task_controller.keep_time != 62)
            {
                gui_display_time = Api_GetUiDisplayTime();
                p_task->task_controller.keep_time = gui_display_time + 2;
                p_task->task_controller.run_time = 0;
            }
        } 
        else if(p_task->child_task_id != 3)
        {           
            // 需要判断查找手机开关是否打开
//            if(p_flash_param->dev_config_s.find_phone_enable == 1)
//            {
//                Cpu_ScheduleTask(p_task, p_device, CPU_TASK_FINDPHONE);
//            }
//            else
            {              
                Cpu_ScheduleTask(p_task, p_device, CPU_TASK_FUNCT2);              
            }         
        }
        
        if(p_task->child_task_id != 3)
        {
            p_device->key = BUTTON_EVENT_NULL;	 
        }
    }     
}

/****************************************************************
** Function name:            Cpu_HourglassTask
** Descriptions:             倒计时界面
** input parameters:         
** output parameters:        无
** Returned value:           无
** Created by:               GT
** Created Date:             2017-07-10
****************************************************************/
void Cpu_HourglassTask(cpu_task_tcb_t* p_task, cpu_device_tcb_t* p_device)
{
//    uint8_t cls_flag_tmp = p_device->cls_flag;
    uint8_t hour = 0;
//     uint8_t run_state = 0;
    if(p_task->task_controller.init == 1)
    {
        Cpu_TaskParamInit(p_task,gui_display_time,true,true);
        p_task->child_task_id = 1;
//        //清除背景
//        Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
//                                p_device->cls_param.column_size, p_device->cls_param.page_size);
    }

#if LED_ON_OFF_ON_DEF
    if(p_task->gui_controller.animation_tick_counter == 0 )
    {
        Gui_LcdLedOff();
    } 
#endif
 
    if(p_task->gui_controller.repaint_enable == 1)
    {
        if(p_task->child_task_id != 1)
		{
//			//清除背景
//			Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
//									p_device->cls_param.column_size, p_device->cls_param.page_size);
			p_task->gui_controller.animation_frame_index = 0;
			p_task->child_task_id = 1;
		}
		 
//         
        
        // 如果按键启动倒计时功能,则需要显示动画
        if((p_flash_param->hourglass_s.btn_trigger == 1)||(p_flash_param->hourglass_s.app_trigger == 1))
        {
            if(p_task->gui_controller.animation_tick_counter%2 == 0)
            {
                hour = Hour_StandardTransform(p_flash_param->dev_config_s.am_pm_enable,clock->hour);
				Gui_PageCountDown(p_task->gui_controller.animation_frame_index++, 1, p_flash_param->hourglass_s.run_remain_time, 
                                  hour, clock->minutes, display_language, &p_device->cls_flag,  1);
            }
        }
        else if(p_task->gui_controller.animation_tick_counter%3 == 0)
        {
//            if(p_flash_param->hourglass_s.run_remain_time == 0)
//            {
//                
//                disp_time++;
//                Gui_PageCountDown2(task_interval_cnt,display_language);
//                if(disp_time >= 8)
//                {
//                    Cpu_ScheduleTask(p_task, p_device, CPU_TASK_FUNCT2);
//                }
//            }
//            else
//            {
//                Gui_PageCountDown(task_interval_cnt, 1, 0, p_flash_param->hourglass_s.run_remain_time, display_language);
//            }
            hour = Hour_StandardTransform(p_flash_param->dev_config_s.am_pm_enable,clock->hour);
			Gui_PageCountDown(p_task->gui_controller.animation_frame_index++, 0, 0, hour, 
                                clock->minutes, display_language, &p_device->cls_flag,  0);
        } 
    }
    
#if LED_ON_OFF_ON_DEF
    //YX.20180830.增加延时，去掉显示的闪屏问题
    if(p_task->gui_controller.animation_tick_counter == 0)
    {
         
        Gui_LcdLedOn();
    } 
#endif
  
    // 按下按键(短按),切换到下一个界面
    if((p_task->gui_controller.repaint_enable == 1)&&(p_device->key == BUTTON_EVENT_CLICK))
    {
        // 需要判断查找手机开关是否打开
//        if(p_flash_param->dev_config_s.watchtimer_enable == 1)
//        {
//            Cpu_ScheduleTask(p_task, p_device, CPU_TASK_TIMER);
//        }
//        else if(p_flash_param->dev_config_s.find_phone_enable == 1)
//        {
//            Cpu_ScheduleTask(p_task, p_device, CPU_TASK_FINDPHONE);
//        }
//        else
        {              
            Cpu_ScheduleTask(p_task, p_device, CPU_TASK_FUNCT);              
        }
    }  

    // 倒计时功能按键触发
    if((p_task->gui_controller.repaint_enable == 1)&&(p_device->key == BUTTON_EVENT_LONGPRESS))
    {
        p_device->key = BUTTON_EVENT_NULL;
        
        // 如果倒计时已经启动了,则需要停止
        if((p_flash_param->hourglass_s.btn_trigger == 1) || (p_flash_param->hourglass_s.app_trigger == 1))
        {
            p_flash_param->hourglass_s.btn_trigger = 0;
            p_flash_param->hourglass_s.app_trigger = 0;
            p_flash_param->hourglass_s.run_remain_time = 0;
            p_task->gui_controller.animation_frame_index = 0;
            p_task->task_controller.keep_time = 3;
            p_task->task_controller.run_time = 0;
            Cpu_ClearSechduleTask(p_task);
        }
        else
        {
            p_flash_param->hourglass_s.app_send = 1;
            p_flash_param->hourglass_s.btn_trigger = 1;
            p_flash_param->hourglass_s.run_remain_time = p_flash_param->hourglass_s.dev_remain_time;
            p_task->task_controller.keep_time = 3;
            p_task->task_controller.run_time = 0;
            p_task->gui_controller.animation_frame_index = 0;
        }
    }  
}

/****************************************************************
** Function name:           Cpu_TraverseInTask
** Descriptions:            UI遍历使用，显示某个任务的UI
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              YX              
** Created Date:            2017-12-11
*****************************************************************/
void Cpu_TraverseInTask(cpu_task_tcb_t* p_task, CPU_TASK_TYPE task_type, uint8_t dis_time)
{
    p_task->task_id = task_type;
    Cpu_TaskParamInit(p_task, dis_time, false, true);
}

/****************************************************************
** Function name:            Cpu_BpTask
** Descriptions:             血压界面
** input parameters:         
** output parameters:        无
** Returned value:           无
** Created by:               WJ
** Created Date:             2018-11-09
****************************************************************/
void Cpu_BpTask(cpu_task_tcb_t* p_task, cpu_device_tcb_t* p_device)
{
//    uint8_t cls_flag_tmp = p_device->cls_flag;
    static uint8_t bp_high = 0;
    static uint8_t bp_low = 0;
    static uint8_t test_mode = 0;
    static uint8_t test_state = 0;
    uint8_t hour = 0;
    
    if(p_task->task_controller.init == 1)
    {								
		// 初始化其他变量
		p_task->child_task_id = 1;
		if(Battery_Get_Grade() == 0)
		{
            p_data_manage->wear_measure_s.wear_status = 0;
			p_data_manage->ecg_ppg_s.ppg_measure_s.btn_bp = 0;
			Cpu_TaskParamInit(p_task, gui_display_time, false, true);	      
		}
		else
		{	
            Statemachine_AddState(MEAT_PPG_BTN_BP);
			p_data_manage->ecg_ppg_s.ppg_measure_s.btn_bp = 1;
		
            Cpu_TaskParamInit(p_task, 60, false, true);
		}
        
        bp_high = 0;
        bp_low = 0;
        test_mode = 0;
        test_state = 0;
        
        test_mode = p_flash_param->bp_s.mode;
        
//        //清除背景
//        Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
//                                p_device->cls_param.column_size, p_device->cls_param.page_size);
    }   

#if LED_ON_OFF_ON_DEF
    if(p_task->gui_controller.animation_tick_counter == 0)
    {
        Gui_LcdLedOff();
    } 
#endif
 
    if((p_task->task_controller.run_time >= 59) || (p_data_manage->wear_measure_s.wear_status == 1))
    {   // 超时或者佩戴未通过关闭
        if(p_data_manage->ecg_ppg_s.ppg_measure_s.btn_bp == 1)
        {
            Statemachine_DelState(MEAT_PPG_BTN_BP);
        }
        p_data_manage->ecg_ppg_s.ppg_measure_s.btn_bp = 0;
        memset(p_task, 0, sizeof(cpu_task_tcb_t));
    }

    // 修改相关变量 
    if(((p_task->gui_controller.animation_tick_counter%3) == 0)&&(p_task->gui_controller.repaint_enable == 1))
    {
        if(p_task->child_task_id != 1)
		{
//			//清除背景
//			Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
//									p_device->cls_param.column_size, p_device->cls_param.page_size);
           	p_task->gui_controller.animation_frame_index = 0;
			p_task->child_task_id = 1;
		}
		 
//         
        
        if(test_state == 0 && p_data_manage->ecg_ppg_s.ppg_measure_s.btn_bp == 1)
        {
            if(p_data_manage->ecg_ppg_s.ppg_measure_s.calc_bp_init == 0 && p_data_manage->ecg_ppg_s.ppg_measure_s.bp_progress >= PPG_BP_CALC_TIME)
            {
                //获取血压值，测量模式，测量状态
                test_state = p_data_manage->ecg_ppg_s.ppg_measure_s.p_bp_flag;
                bp_high = p_data_manage->ecg_ppg_s.ppg_measure_s.p_bp_high_sec;//p_bp_high_25sec;
                bp_low = p_data_manage->ecg_ppg_s.ppg_measure_s.p_bp_low_sec;//p_bp_low_25sec;
                
                if(test_state != 1 || bp_high < 30 || bp_low < 30)//Bp_ResultCheck(test_mode, bp_high, bp_low, 0) == 0)
                {
                    test_state = 2;
                    bp_high = 0;
                    bp_low = 0;
                }       

                Statemachine_DelState(MEAT_PPG_BTN_BP);
                p_data_manage->ecg_ppg_s.ppg_measure_s.btn_bp = 0;
            }
        }
        hour = Hour_StandardTransform(p_flash_param->dev_config_s.am_pm_enable,clock->hour);
        Gui_PageBP(p_task->gui_controller.animation_frame_index++, bp_high, bp_low, test_mode, test_state, hour, clock->minutes, &p_device->cls_flag,display_language);
    } 
    
#if LED_ON_OFF_ON_DEF
    //YX.20180830.增加延时，去掉显示的闪屏问题
    if(p_task->gui_controller.animation_tick_counter == 0)
    {
         
        Gui_LcdLedOn();
    } 
#endif
  
    if(p_task->gui_controller.repaint_enable == 1)
    {
        if(p_device->key == BUTTON_EVENT_CLICK)
        {
            if(p_data_manage->ecg_ppg_s.ppg_measure_s.btn_bp == 1)
            {   
                Statemachine_DelState(MEAT_PPG_BTN_BP);
                p_data_manage->ecg_ppg_s.ppg_measure_s.init = 0; 
                //WJ.20181214.用于app回复
                p_data_manage->ecg_ppg_s.ppg_measure_s.bp_progress = 201;                
            }
            p_data_manage->ecg_ppg_s.ppg_measure_s.btn_bp = 0;

            
            Cpu_ScheduleTask(p_task, p_device, CPU_TASK_OXYGEN);//CPU_TASK_MOVEMENT); 
            p_device->key = BUTTON_EVENT_NULL;
        }
    }    
}

/****************************************************************
** Function name:            Cpu_PttTask
** Descriptions:             Ptt界面
** input parameters:         
** output parameters:        无
** Returned value:           无
** Created by:               WJ
** Created Date:             2018-11-09
****************************************************************/
void Cpu_PttTask(cpu_task_tcb_t* p_task, cpu_device_tcb_t* p_device)
{
    if(p_task->task_controller.init == 1)
    {								
		// 初始化其他变量
		p_task->child_task_id = 1;

        Cpu_TaskParamInit(p_task, gui_display_time, false, true);	      
//        //清除背景
//        Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
//                                p_device->cls_param.column_size, p_device->cls_param.page_size);
    } 
    
#if LED_ON_OFF_ON_DEF
    if(p_task->gui_controller.animation_tick_counter == 0)
    {
        Gui_LcdLedOff();
    } 
#endif
 
//    // 修改相关变量 
//    if(((p_task->gui_controller.animation_tick_counter%3) == 0)&&(p_task->gui_controller.repaint_enable == 1))
//    {
//        if(p_task->child_task_id != 1)
//		{
//			//清除背景
//			Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
//									p_device->cls_param.column_size, p_device->cls_param.page_size);
//			p_task->child_task_id = 1;
//            p_task->gui_controller.animation_frame_index = 0;
//		}
//		 
//         
//     
//        Gui_PagePtt(p_task->gui_controller.animation_frame_index++);
//    } 
    
#if LED_ON_OFF_ON_DEF
    //YX.20180830.增加延时，去掉显示的闪屏问题
    if(p_task->gui_controller.animation_tick_counter == 0)
    {
         
        Gui_LcdLedOn();
    } 
#endif
  
//    if(p_task->gui_controller.repaint_enable == 1)
//    {
//        if(p_device->key == BUTTON_EVENT_CLICK)
//        {
//            Cpu_ScheduleTask(p_task, p_device, CPU_TASK_POWEROFF);
//            p_device->key = BUTTON_EVENT_NULL;
//        }
//        else if(p_device->key == BUTTON_EVENT_LONGPRESS)
//        {
//            Cpu_ScheduleTask(p_task, p_device, CPU_TASK_PTT_ITEM1);
//            p_device->key = BUTTON_EVENT_NULL;
//        }
//    }    
}

/****************************************************************
** Function name:            Cpu_PttItemTask
** Descriptions:             Ptt界面-子界面1
** input parameters:         
** output parameters:        无
** Returned value:           无
** Created by:               WJ
** Created Date:             2018-11-09
****************************************************************/
void Cpu_PttItemTask(cpu_task_tcb_t* p_task, cpu_device_tcb_t* p_device)
{
    if(p_task->task_controller.init == 1)
    {								
		// 初始化其他变量
		p_task->child_task_id = 1;
        
//        Statemachine_AddState(MEAT_PTT_BTN);

        Cpu_TaskParamInit(p_task, gui_display_time, true, true);	      
//        //清除背景
//        Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
//                                p_device->cls_param.column_size, p_device->cls_param.page_size);
    } 
    
#if LED_ON_OFF_ON_DEF
    if(p_task->gui_controller.animation_tick_counter == 0)
    {
        Gui_LcdLedOff();
    } 
#endif
 
//    // 修改相关变量 
//    if(((p_task->gui_controller.animation_tick_counter%3) == 0)&&(p_task->gui_controller.repaint_enable == 1))
//    {
//        if(p_task->child_task_id != 1)
//		{
//			//清除背景
//			Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
//									p_device->cls_param.column_size, p_device->cls_param.page_size);
//			p_task->child_task_id = 1;
//            p_task->gui_controller.animation_frame_index = 0;
//		}
//		 
//         
//     
//        Gui_PagePttItem1(p_task->gui_controller.animation_frame_index++, 
//                         p_data_manage->ecg_ppg_s.ecg_measure_s.e_wear_flag);
//    } 
    
#if LED_ON_OFF_ON_DEF
    //YX.20180830.增加延时，去掉显示的闪屏问题
    if(p_task->gui_controller.animation_tick_counter == 0)
    {
         
        Gui_LcdLedOn();
    } 
#endif
  
//    if(p_task->gui_controller.repaint_enable == 1)
//    {
//        if(p_device->key == BUTTON_EVENT_LONGPRESS
//          && (p_data_manage->ecg_ppg_s.ecg_measure_s.init == 0
//              && p_data_manage->ecg_ppg_s.ecg_measure_s.e_wear_flag == 1 
//              && p_data_manage->ecg_ppg_s.ecg_measure_s.e_wear_time >= 5))
//        {
//            Statemachine_DelState(MEAT_PTT_BTN);

//            Cpu_ScheduleTask(p_task, p_device, CPU_TASK_PTT);
//            p_device->key = BUTTON_EVENT_NULL;
//        }
//        p_device->key = BUTTON_EVENT_NULL;
//    }    
}

/****************************************************************
** Function name:            Cpu_EcgTask
** Descriptions:             Ecg面
** input parameters:         
** output parameters:        无
** Returned value:           无
** Created by:               WJ
** Created Date:             2018-11-09
****************************************************************/
void Cpu_EcgTask(cpu_task_tcb_t* p_task, cpu_device_tcb_t* p_device)
{
//    uint8_t cls_flag_tmp = p_device->cls_flag;
    uint8_t hour = 0;
    if(p_task->task_controller.init == 1)
    {								
		// 初始化其他变量
		p_task->child_task_id = 1;

        Cpu_TaskParamInit(p_task, gui_display_time, true, true);	      
//        //清除背景
//        Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
//                                p_device->cls_param.column_size, p_device->cls_param.page_size);
    } 
    
#if LED_ON_OFF_ON_DEF
    if(p_task->gui_controller.animation_tick_counter == 0)
    {
        Gui_LcdLedOff();
    } 
#endif
 
    // 修改相关变量 
    if(((p_task->gui_controller.animation_tick_counter%4) == 0)&&(p_task->gui_controller.repaint_enable == 1))
    {
        if(p_task->child_task_id != 1)
		{
//			//清除背景
//			Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
//									p_device->cls_param.column_size, p_device->cls_param.page_size);
			p_task->child_task_id = 1;
            p_task->gui_controller.animation_frame_index = 0;
		}
		 
//         
        hour = Hour_StandardTransform(p_flash_param->dev_config_s.am_pm_enable,clock->hour);
        Gui_PageEcg(p_task->gui_controller.animation_frame_index++, hour, clock->minutes, display_language, &p_device->cls_flag);
    } 
    
#if LED_ON_OFF_ON_DEF
    //YX.20180830.增加延时，去掉显示的闪屏问题
    if(p_task->gui_controller.animation_tick_counter == 0)
    {
         
        Gui_LcdLedOn();
    } 
#endif
  
    if(p_task->gui_controller.repaint_enable == 1)
    {
        if(p_device->key == BUTTON_EVENT_CLICK)
        {
            // 需要判断查找手机开关是否打开
//            if((p_flash_param->hourglass_s.ui_enable == 1)
//                   ||(p_flash_param->hourglass_s.app_trigger == 1)                
//                   ||(p_flash_param->hourglass_s.btn_trigger == 1))
//            {
//                Cpu_ScheduleTask(p_task, p_device, CPU_TASK_HOURGLASS);
//            }
//            else if(p_flash_param->dev_config_s.watchtimer_enable == 1)
//            {
//                Cpu_ScheduleTask(p_task, p_device, CPU_TASK_TIMER);
//            }
//            else if(p_flash_param->dev_config_s.find_phone_enable == 1)
//            {
//                Cpu_ScheduleTask(p_task, p_device, CPU_TASK_FINDPHONE);
//            }
//            else
            {              
                Cpu_ScheduleTask(p_task, p_device, CPU_TASK_FUNCT);              
            }
            p_device->key = BUTTON_EVENT_NULL;
        }
        else if(p_device->key == BUTTON_EVENT_LONGPRESS)
        {
            if(Battery_Get_Grade() != 0 && Battery_Get_Status() == BATTERY_POWER_NORMAL)
            {
                Cpu_ScheduleTask(p_task, p_device, CPU_TASK_ECG_ITEM1);
            }
            p_device->key = BUTTON_EVENT_NULL;
        }
    }    
}

/****************************************************************
** Function name:            Cpu_EcgItem1Task
** Descriptions:             ECG界面-子界面1
** input parameters:         
** output parameters:        无
** Returned value:           无
** Created by:               WJ
** Created Date:             2018-11-09
****************************************************************/
static uint8_t ecg_result_state = 0;    //ECG测量结果状态
void Cpu_EcgItem1Task(cpu_task_tcb_t* p_task, cpu_device_tcb_t* p_device)
{
    static uint8_t ecg_heart = 0;
    static uint8_t ecg_state = 0;
    static uint8_t ecg_heart_ok = 0;
    
    if(p_task->task_controller.init == 1)
    {								
		// 初始化其他变量
		p_task->child_task_id = 1;
        
        {
            Statemachine_AddState(MEAT_ECG_BTN_HEART);
        }
        
        Cpu_TaskParamInit(p_task, 121, false, true);	      
//        //清除背景
//        Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
//                                p_device->cls_param.column_size, p_device->cls_param.page_size);
        
        ecg_heart = 0;
        ecg_result_state = 0;
        ecg_state = 0;
        ecg_heart_ok = 0;
    } 
    
#if LED_ON_OFF_ON_DEF
    if(p_task->gui_controller.animation_tick_counter == 0)
    {
        Gui_LcdLedOff();
    } 
#endif
 
    // 导联脱落持续5s，时间超时退出
    if(p_task->task_controller.run_time >= 120
      || (p_data_manage->ecg_ppg_s.ecg_measure_s.init == 0
         && (p_data_manage->ecg_ppg_s.ecg_measure_s.e_wear_flag == 1 
         && p_data_manage->ecg_ppg_s.ecg_measure_s.e_wear_time >= 5)
      || (p_task->task_controller.run_time + (32 - p_data_manage->ecg_ppg_s.ecg_measure_s.result_progress)) >= 120))
    {
        Statemachine_DelState(MEAT_ECG_BTN_HEART);
        // WJ.20190409.修复一直提示测试失败的问题
        //ecg_result_state = 0;
        Cpu_ScheduleTask(p_task, p_device, CPU_TASK_ECG_ITEM2);
        
        if(p_data_manage->ecg_ppg_s.ecg_measure_s.e_wear_flag == 1 
           && p_data_manage->ecg_ppg_s.ecg_measure_s.e_wear_time >= 5)
        {
            ecg_result_state = 2;//2;
        }
        
        return;
    }
    
    // 修改相关变量 
    if(((p_task->gui_controller.animation_tick_counter%2) == 0)&&(p_task->gui_controller.repaint_enable == 1))
    {
        if(p_task->child_task_id != 1)
		{
//			//清除背景
//			Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
//									p_device->cls_param.column_size, p_device->cls_param.page_size);
			p_task->child_task_id = 1;
            p_task->gui_controller.animation_frame_index = 0;
		}
		 
        if(p_data_manage->ecg_ppg_s.ecg_measure_s.init == 0
           && p_data_manage->ecg_ppg_s.ecg_measure_s.e_second_heart >= 30 
           && p_data_manage->ecg_ppg_s.ecg_measure_s.e_second_heart <= 250 )
        {
            ecg_heart = p_data_manage->ecg_ppg_s.ecg_measure_s.e_second_heart;
            ecg_heart_ok = 1;
        }
        
//        if(p_data_manage->ecg_ppg_s.ecg_measure_s.e_wear_flag == 1)
//        {
//            ecg_heart = 0;
//        }


#if 0   // WJ.20190327.暂时屏蔽数据保存，新需求，出货           
//        if(p_data_manage->ecg_ppg_s.ecg_measure_s.init == 0
//           && p_data_manage->ecg_ppg_s.ecg_measure_s.result_progress >= 30)
//        {
#else
        if(p_data_manage->ecg_ppg_s.ecg_measure_s.init == 0
           && ecg_heart_ok == 1)
        {
#endif            
            //ecg_state = 1;
            ecg_result_state = 1;
        }
        
        Gui_PageEcgItem1(p_task->gui_controller.animation_frame_index++, ecg_state, ecg_heart, display_language, &p_device->cls_flag);
        
        if(p_data_manage->ecg_ppg_s.ecg_measure_s.e_wear_flag == 1
           && ecg_state == 1)
        {
            Statemachine_DelState(MEAT_ECG_BTN_HEART);
            ecg_result_state = 1;
            Cpu_ScheduleTask(p_task, p_device, CPU_TASK_ECG_ITEM2);   
        }
    } 
    
#if LED_ON_OFF_ON_DEF
    //YX.20180830.增加延时，去掉显示的闪屏问题
    if(p_task->gui_controller.animation_tick_counter == 0)
    {
         
        Gui_LcdLedOn();
    } 
#endif
  
    if(p_task->gui_controller.repaint_enable == 1)
    {
        if(p_device->key == BUTTON_EVENT_LONGPRESS)
        {
            Statemachine_DelState(MEAT_ECG_BTN_HEART);
            Cpu_ScheduleTask(p_task, p_device, CPU_TASK_ECG);            
        }
        //开启ECG，就不支持按键操作，需要通过导联脱落检测退出
        p_device->key = BUTTON_EVENT_NULL;
    }    
}

/****************************************************************
** Function name:            Cpu_EcgItem2Task
** Descriptions:             ECG界面-子界面2
** input parameters:         
** output parameters:        无
** Returned value:           无
** Created by:               WJ
** Created Date:             2018-11-09
****************************************************************/
void Cpu_EcgItem2Task(cpu_task_tcb_t* p_task, cpu_device_tcb_t* p_device)
{
    if(p_task->task_controller.init == 1)
    {								
		// 初始化其他变量
		p_task->child_task_id = 1;

        Cpu_TaskParamInit(p_task, gui_display_time, false, true);	      
//        //清除背景
//        Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
//                                p_device->cls_param.column_size, p_device->cls_param.page_size);
    } 
    
#if LED_ON_OFF_ON_DEF
    if(p_task->gui_controller.animation_tick_counter == 0)
    {
        Gui_LcdLedOff();
    } 
#endif
 
    if((p_task->task_controller.run_time >= 3))
    {   // 超时回到一级菜单
        p_device->key = BUTTON_EVENT_NULL;
        Cpu_ScheduleTask(p_task, p_device, CPU_TASK_ECG);
        return;
    }
    
    // 修改相关变量 
    if(((p_task->gui_controller.animation_tick_counter%2) == 0)&&(p_task->gui_controller.repaint_enable == 1))
    {
        if(p_task->child_task_id != 1)
		{
//			//清除背景
//			Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
//									p_device->cls_param.column_size, p_device->cls_param.page_size);
			p_task->child_task_id = 1;
            p_task->gui_controller.animation_frame_index = 0;
		}
		 
//         
     
        Gui_PageEcgItem2(p_task->gui_controller.animation_frame_index++, ecg_result_state, display_language, &p_device->cls_flag);
    } 
    
#if LED_ON_OFF_ON_DEF
    //YX.20180830.增加延时，去掉显示的闪屏问题
    if(p_task->gui_controller.animation_tick_counter == 0)
    {
         
        Gui_LcdLedOn();
    } 
#endif
  
    if(p_task->gui_controller.repaint_enable == 1)
    {
        //开启ECG，不响应按键
        p_device->key = BUTTON_EVENT_NULL;
    }    
}

/****************************************************************
** Function name:            Cpu_OxygenTask
** Descriptions:             血氧
** input parameters:         
** output parameters:        无
** Returned value:           无
** Created by:               WJ
** Created Date:             2018-12-29
****************************************************************/
void Cpu_OxygenTask(cpu_task_tcb_t* p_task, cpu_device_tcb_t* p_device)
{   
    uint8_t     spo2_value = 0;
    uint8_t     hour = 0;
    
    if(p_task->task_controller.init == 1)
    {       
        // 初始化其他变量
        p_task->child_task_id = 1;
		
        //YX.20180720.增加充电屏蔽
		if(Battery_Get_Grade() == 0|| Battery_Get_Status() != BATTERY_POWER_NORMAL)
		{
			Cpu_TaskParamInit(p_task, gui_display_time, false, true);
		}
		else
		{
            Statemachine_AddState(MEAT_OXYGEN_BTN);
			Cpu_TaskParamInit(p_task, 299, false, true);
		}
    }
    
#if LED_ON_OFF_ON_DEF
    if(p_task->gui_controller.animation_tick_counter == 0)
    {
        Gui_LcdLedOff();
    } 
#endif
    
    // 检测佩戴条件    
    if((p_task->task_controller.run_time >= 299) || (p_data_manage->wear_measure_s.wear_status == 1))
    {
        Statemachine_DelState(MEAT_OXYGEN_BTN);
        p_data_manage->ecg_ppg_s.oxygen_measure_s.ox_btn = 0;
        memset(p_task, 0, sizeof(cpu_task_tcb_t));
        return;
    }
    // 血氧动画刷新(帧/0.3S)
    if(((p_task->gui_controller.animation_tick_counter%3) == 0)&&(p_task->gui_controller.repaint_enable == 1))
    {   
        
        if(p_task->child_task_id != 1)
		{
			//清除背景
            //Gui_PageClearBackground(0, 0, LCD_MAX_COLUMN, LCD_MAX_ROW);
			p_task->child_task_id = 1;
            p_task->gui_controller.animation_frame_index = 0;
		}
		 
        if(p_data_manage->ecg_ppg_s.oxygen_measure_s.ox_init == 0
            && !(Battery_Get_Grade() == 0 || Battery_Get_Status() != BATTERY_POWER_NORMAL))
        {
            spo2_value = p_data_manage->ecg_ppg_s.oxygen_measure_s.ox_result;
        }
		
        hour = Hour_StandardTransform(p_flash_param->dev_config_s.am_pm_enable, clock->hour);    
        Gui_PageOxygen(p_task->gui_controller.animation_frame_index++, 100, 
                       spo2_value,
                       hour, clock->minutes, display_language, &p_device->cls_flag);	
    }  
    
#if LED_ON_OFF_ON_DEF
    //YX.20180830.增加延时，去掉显示的闪屏问题
    if(p_task->gui_controller.animation_tick_counter == 0)
    {
         
        Gui_LcdLedOn();
    } 
#endif
    
    // 按键点击,进入到心率测量界面
    if(p_task->gui_controller.repaint_enable == 1)
    {
        if(p_device->key == BUTTON_EVENT_CLICK)
        {
            Statemachine_DelState(MEAT_OXYGEN_BTN);
            Cpu_ScheduleTask(p_task, p_device, CPU_TASK_MOVEMENT);
        }
        p_device->key = BUTTON_EVENT_NULL;
    }
}

/****************************************************************
** Function name:           Cpu_FunctTask
** Descriptions:            更多功能界面处理
** input parameters:        p_controller：UI事件控制状态；p_gui：显示相关的参数
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-08-12
*****************************************************************/ 
void Cpu_FunctTask(cpu_task_tcb_t* p_task, cpu_device_tcb_t* p_device)
{

    uint8_t hour = 0;
    uint16_t deep_time = 0;
    uint16_t light_time = 0; 
    uint8_t quality = 0;
    if(p_task->task_controller.init == 1)
    {
        Cpu_TaskParamInit(p_task,gui_display_time,true,true);
        p_task->child_task_id = 1;

    }    

#if LED_ON_OFF_ON_DEF
    if(p_task->gui_controller.animation_tick_counter == 0)
    {
        Gui_LcdLedOff();
    } 
#endif
 
    // 客户要求刷新速率为帧/0.3s
    if(((p_task->gui_controller.animation_tick_counter%4) == 0)&&(p_task->gui_controller.repaint_enable == 1))
    {
        if(p_task->child_task_id != 1)
		{
			p_task->child_task_id = 1;
            p_task->gui_controller.animation_frame_index = 0;
		}

        Gui_PageFunct(p_task->gui_controller.animation_frame_index++,display_language);
    } 
    
#if LED_ON_OFF_ON_DEF
    //YX.20180830.增加延时，去掉显示的闪屏问题
    if(p_task->gui_controller.animation_tick_counter == 0)
    {
         
        Gui_LcdLedOn();
    } 
#endif
  
    // 按键短按,切换到下一页
    if(p_task->gui_controller.repaint_enable == 1)
    {
        if(p_device->key == BUTTON_EVENT_CLICK)
        {
            Cpu_ScheduleTask(p_task, p_device, CPU_TASK_POWEROFF);
        }
        else if(p_device->key == BUTTON_EVENT_LONGPRESS)
        {   
            Cpu_ScheduleTask(p_task, p_device, CPU_TASK_FUNCT2);
            funct2_page = 0;
            funct2_num = 0;
        }	
        p_device->key = BUTTON_EVENT_NULL;        
    } 	
}
/****************************************************************
** Function name:           Cpu_Funct2Task
** Descriptions:            更多功能界面处理
** input parameters:        p_controller：UI事件控制状态；p_gui：显示相关的参数
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-08-12
*****************************************************************/ 
void Cpu_Funct2Task(cpu_task_tcb_t* p_task, cpu_device_tcb_t* p_device)
{
    uint8_t slid_ret = 0;
    uint16_t funct_tmp = 0;
	if(p_task->task_controller.init == 1)
	{ 
        Cpu_TaskParamInit(p_task, gui_display_time, true, true);
		
        p_task->child_task_id = 1;
	} 
	
#if	LED_ON_OFF_ON_DEF	
	if(p_task->gui_controller.animation_tick_counter == 0)
	{
		Gui_LcdLedOff();
	}
#endif	

	if(((p_task->gui_controller.animation_tick_counter%3) == 0)&&(p_task->gui_controller.repaint_enable == 1))
	{
        if(p_task->child_task_id != 1)
		{
			p_task->child_task_id = 1;
            p_task->gui_controller.animation_frame_index = 0;
		}
        Gui_PageFunct2(p_task->gui_controller.animation_frame_index++, funct2_page, funct2_num, display_language);
	}
	
#if	LED_ON_OFF_ON_DEF	
	if(p_task->gui_controller.animation_tick_counter == 0)
	{
		Gui_LcdLedOn();
	}
#endif	
    
    if(p_task->gui_controller.repaint_enable == 1)
    {
        if(p_device->key == BUTTON_EVENT_CLICK)
        {
            funct2_num++;
            if(funct2_num >= 4 || (funct2_num >= 3 && funct2_page == 2))
            {
                funct2_num = 0;
                funct2_page++;
                if(funct2_page >= 3)
                {
                    funct2_page = 0;
                    funct2_num = 0;
                }
                task_interval_cnt = 0xFFFF;
                p_task->gui_controller.animation_frame_index = 0;
            }
            p_task->task_controller.run_time = 0;
            p_task->child_task_id = 1;
        }
        else if(p_device->key == BUTTON_EVENT_LONGPRESS)
        {   
            funct_tmp = funct2_page*4 + funct2_num;
            switch(funct_tmp)
            {
                case 0:case 4:case 8://返回上一个界面
                    Cpu_ScheduleTask(p_task, p_device, CPU_TASK_FUNCT);
                    break;
                case 1://进入秒表界面
                    Cpu_ScheduleTask(p_task, p_device, CPU_TASK_TIMER);
                    break;
                case 2://进入倒计时界面
                    Cpu_ScheduleTask(p_task, p_device, CPU_TASK_HOURGLASS);
                    break;
                case 3://进入消息设置界面
                    Cpu_ScheduleTask(p_task, p_device, CPU_TASK_MESSAGE_SWITCH);
                    break;
                case 5://进入找手机
                    Cpu_ScheduleTask(p_task, p_device, CPU_TASK_FINDPHONE);
                    break;
                case 6://进入亮度调节
                    Cpu_ScheduleTask(p_task, p_device, CPU_TASK_LIGHTLEVEL);
                    break;
                case 7://进入锁屏时长
                    Cpu_ScheduleTask(p_task, p_device, CPU_TASK_LIGHTTIME);
                    break;
                case 9://进入开关设置
                    Cpu_ScheduleTask(p_task, p_device, CPU_TASK_SWITCH);
                    break;
                case 10://清除数据
                    Cpu_ScheduleTask(p_task, p_device, CPU_TASK_CLEARDATA);
                    break;
                default:
                    break;
            }
        }
		
        p_device->key = BUTTON_EVENT_NULL;
    }	
}
  /****************************************************************
** Function name:           Cpu_MessageSwitchTask
** Descriptions:            消息开关设置界面处理
** input parameters:        p_controller：UI事件控制状态；p_gui：显示相关的参数
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-08-12
*****************************************************************/ 
void Cpu_MessageSwitchTask(cpu_task_tcb_t* p_task, cpu_device_tcb_t* p_device)
{
    static uint8_t switch_state[5][3] = {0};
    uint8_t slid_ret = 0;
    static uint8_t funct2_page = 0;
    static uint8_t funct2_num = 0;
    uint16_t funct_tmp = 0;
    
	if(p_task->task_controller.init == 1)
	{ 
        Cpu_TaskParamInit(p_task, gui_display_time, true, true);
		
        p_task->child_task_id = 1;
        
        funct2_page = 0;
        funct2_num = 0;
        switch_state[0][0] = p_flash_param->message_enable_s.call;
        switch_state[0][1] = p_flash_param->message_enable_s.sms;
        switch_state[0][2] = p_flash_param->message_enable_s.wechat;
        switch_state[1][0] = p_flash_param->message_enable_s.qq;
        switch_state[1][1] = p_flash_param->message_enable_s.facebook;
        switch_state[1][2] = p_flash_param->message_enable_s.twitter;
        switch_state[2][0] = p_flash_param->message_enable_s.linkedln;
        switch_state[2][1] = p_flash_param->message_enable_s.whatsapp;
        switch_state[2][2] = p_flash_param->message_enable_s.line;
        switch_state[3][0] = p_flash_param->message_enable_s.instagram;
        switch_state[3][1] = p_flash_param->message_enable_s.snapchat;
        switch_state[3][2] = p_flash_param->message_enable_s.skype;
        switch_state[4][0] = p_flash_param->message_enable_s.gmail;
        switch_state[4][1] = p_flash_param->message_enable_s.other;
        switch_state[4][2] = 0;
	} 
	
#if	LED_ON_OFF_ON_DEF	
	if(p_task->gui_controller.animation_tick_counter == 0)
	{
//////        //出现滑动界面后，然后切换成覆盖显示的时候添加，将偏移设置成正常数值
//////        if(p_device->slid_param.slid_flag == 0)
//////        {
//////            p_device->y_offset = 0;//偏移量至0
//////            LCD_SetYoffset(0);
//////            Gui_SetYoffset(0);
//////        }
		Gui_LcdLedOff();
	}
#endif	
    

	if(((p_task->gui_controller.animation_tick_counter%3) == 0)&&(p_task->gui_controller.repaint_enable == 1))
	{ 
        
        Gui_PageMessageSwitch(p_task->gui_controller.animation_frame_index++, funct2_page, funct2_num, switch_state[funct2_page%5], display_language);
	}
	
#if	LED_ON_OFF_ON_DEF	
	if(p_task->gui_controller.animation_tick_counter == 0)
	{
		Gui_LcdLedOn();
	}
#endif	
    
    if(p_task->gui_controller.repaint_enable == 1)
    {
        if(p_device->key == BUTTON_EVENT_CLICK)
        {
            funct2_num++;
            if(funct2_num >= 4 || (funct2_page == 4 && funct2_num >= 3))
            {
                funct2_num = 0;
                funct2_page++;
                if(funct2_page >= 5)
                {
                    funct2_page = 0;
                }
                p_task->gui_controller.animation_frame_index = 0;
            }
            p_task->task_controller.run_time = 0;
            p_task->child_task_id = 1;
        }
        else if(p_device->key == BUTTON_EVENT_LONGPRESS)
        {   
            funct_tmp = funct2_page*4 + funct2_num;
            switch(funct_tmp)
            {
                case 0:case 4:case 8:case 12:case 16://返回上一个界面
                    Cpu_ScheduleTask(p_task, p_device, CPU_TASK_FUNCT2);
                    break;
                case 1://来电
                    if(p_flash_param->message_enable_s.call == 1)
                    {
                        p_flash_param->message_enable_s.call = 2;
                    }
                    else
                    {
                        p_flash_param->message_enable_s.call = 1;
                    }
                    switch_state[0][0] = p_flash_param->message_enable_s.call;
                    upload_switch_s.switch_flg.message_flg = 1;
                    //重置运行时间
                    p_task->task_controller.run_time = 0;
                    break;
                case 2://短信
                    if(p_flash_param->message_enable_s.sms == 1)
                    {
                        p_flash_param->message_enable_s.sms = 2;
                    }
                    else
                    {
                        p_flash_param->message_enable_s.sms = 1;
                    }
                    switch_state[0][1] = p_flash_param->message_enable_s.sms;
                    upload_switch_s.switch_flg.message_flg = 1;
                    //重置运行时间
                    p_task->task_controller.run_time = 0;
                    break;
                case 3://wechat
                    if(p_flash_param->message_enable_s.wechat == 1)
                    {
                        p_flash_param->message_enable_s.wechat = 2;
                    }
                    else
                    {
                        p_flash_param->message_enable_s.wechat = 1;
                    }
                    switch_state[0][2] = p_flash_param->message_enable_s.wechat;
                    upload_switch_s.switch_flg.message_flg = 1;
                    //重置运行时间
                    p_task->task_controller.run_time = 0;
                    break;
                case 5://qq
                    if(p_flash_param->message_enable_s.qq == 1)
                    {
                        p_flash_param->message_enable_s.qq = 2;
                    }
                    else
                    {
                        p_flash_param->message_enable_s.qq = 1;
                    }
                    switch_state[1][0] = p_flash_param->message_enable_s.qq;
                    upload_switch_s.switch_flg.message_flg = 1;
                    //重置运行时间
                    p_task->task_controller.run_time = 0;
                    break;
                case 6://facebook
                    if(p_flash_param->message_enable_s.facebook == 1)
                    {
                        p_flash_param->message_enable_s.facebook = 2;
                    }
                    else
                    {
                        p_flash_param->message_enable_s.facebook = 1;
                    }
                    switch_state[1][1] = p_flash_param->message_enable_s.facebook;
                    upload_switch_s.switch_flg.message_flg = 1;
                    //重置运行时间
                    p_task->task_controller.run_time = 0;
                    break;
                case 7://twitter
                    if(p_flash_param->message_enable_s.twitter == 1)
                    {
                        p_flash_param->message_enable_s.twitter = 2;
                    }
                    else
                    {
                        p_flash_param->message_enable_s.twitter = 1;
                    }
                    switch_state[1][2] = p_flash_param->message_enable_s.twitter;
                    upload_switch_s.switch_flg.message_flg = 1;
                    //重置运行时间
                    p_task->task_controller.run_time = 0;
                    break;
                case 9://linkedln
                    if(p_flash_param->message_enable_s.linkedln == 1)
                    {
                        p_flash_param->message_enable_s.linkedln = 2;
                    }
                    else
                    {
                        p_flash_param->message_enable_s.linkedln = 1;
                    }
                    switch_state[2][0] = p_flash_param->message_enable_s.linkedln;
                    upload_switch_s.switch_flg.message_flg = 1;
                    //重置运行时间
                    p_task->task_controller.run_time = 0;
                    break;
                case 10://whatsapp
                    if(p_flash_param->message_enable_s.whatsapp == 1)
                    {
                        p_flash_param->message_enable_s.whatsapp = 2;
                    }
                    else
                    {
                        p_flash_param->message_enable_s.whatsapp = 1;
                    }
                    switch_state[2][1] = p_flash_param->message_enable_s.whatsapp;
                    upload_switch_s.switch_flg.message_flg = 1;
                    //重置运行时间
                    p_task->task_controller.run_time = 0;
                    break;
                case 11://line
                    if(p_flash_param->message_enable_s.line == 1)
                    {
                        p_flash_param->message_enable_s.line = 2;
                    }
                    else
                    {
                        p_flash_param->message_enable_s.line = 1;
                    }
                    switch_state[2][2] = p_flash_param->message_enable_s.line;
                    upload_switch_s.switch_flg.message_flg = 1;
                    //重置运行时间
                    p_task->task_controller.run_time = 0;
                    break;
                case 13://instagram
                    if(p_flash_param->message_enable_s.instagram == 1)
                    {
                        p_flash_param->message_enable_s.instagram = 2;
                    }
                    else
                    {
                        p_flash_param->message_enable_s.instagram = 1;
                    }
                    switch_state[3][0] = p_flash_param->message_enable_s.instagram;
                    upload_switch_s.switch_flg.message_flg = 1;
                    //重置运行时间
                    p_task->task_controller.run_time = 0;
                    break;
                case 14://snapchat
                    if(p_flash_param->message_enable_s.snapchat == 1)
                    {
                        p_flash_param->message_enable_s.snapchat = 2;
                    }
                    else
                    {
                        p_flash_param->message_enable_s.snapchat = 1;
                    }
                    switch_state[3][1] = p_flash_param->message_enable_s.snapchat;
                    upload_switch_s.switch_flg.message_flg = 1;
                    //重置运行时间
                    p_task->task_controller.run_time = 0;
                    break;
                case 15://skype
                    if(p_flash_param->message_enable_s.skype == 1)
                    {
                        p_flash_param->message_enable_s.skype = 2;
                    }
                    else
                    {
                        p_flash_param->message_enable_s.skype = 1;
                    }
                    switch_state[3][2] = p_flash_param->message_enable_s.skype;
                    upload_switch_s.switch_flg.message_flg = 1;
                    //重置运行时间
                    p_task->task_controller.run_time = 0;
                    break;
                case 17://gmail
                    if(p_flash_param->message_enable_s.gmail == 1)
                    {
                        p_flash_param->message_enable_s.gmail = 2;
                    }
                    else
                    {
                        p_flash_param->message_enable_s.gmail = 1;
                    }
                    switch_state[4][0] = p_flash_param->message_enable_s.gmail;
                    upload_switch_s.switch_flg.message_flg = 1;
                    //重置运行时间
                    p_task->task_controller.run_time = 0;
                    break;
                case 18://other
                    if(p_flash_param->message_enable_s.other == 1)
                    {
                        p_flash_param->message_enable_s.other = 2;
                    }
                    else
                    {
                        p_flash_param->message_enable_s.other = 1;
                    }
                    switch_state[4][1] = p_flash_param->message_enable_s.other;
                    upload_switch_s.switch_flg.message_flg = 1;
                    //重置运行时间
                    p_task->task_controller.run_time = 0;
                    break;
                default:
                    break;
            }        
        }
		switch_state[0][0] = p_flash_param->message_enable_s.call;
        switch_state[0][1] = p_flash_param->message_enable_s.sms;
        switch_state[0][2] = p_flash_param->message_enable_s.wechat;
        switch_state[1][0] = p_flash_param->message_enable_s.qq;
        switch_state[1][1] = p_flash_param->message_enable_s.facebook;
        switch_state[1][2] = p_flash_param->message_enable_s.twitter;
        switch_state[2][0] = p_flash_param->message_enable_s.linkedln;
        switch_state[2][1] = p_flash_param->message_enable_s.whatsapp;
        switch_state[2][2] = p_flash_param->message_enable_s.line;
        switch_state[3][0] = p_flash_param->message_enable_s.instagram;
        switch_state[3][1] = p_flash_param->message_enable_s.snapchat;
        switch_state[3][2] = p_flash_param->message_enable_s.skype;
        switch_state[4][0] = p_flash_param->message_enable_s.gmail;
        switch_state[4][1] = p_flash_param->message_enable_s.other;
        switch_state[4][2] = 0;
        p_device->key = BUTTON_EVENT_NULL;
    }	
}
/****************************************************************
** Function name:           Cpu_SwitchTask
** Descriptions:            开关设置界面处理
** input parameters:        p_controller：UI事件控制状态；p_gui：显示相关的参数
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-08-12
*****************************************************************/ 
void Cpu_SwitchTask(cpu_task_tcb_t* p_task, cpu_device_tcb_t* p_device)
{
    static uint8_t switch_state[4][3] = {0};
    uint8_t slid_ret = 0;
    static uint8_t switch_page = 0;
    static uint8_t switch_num = 0;
    uint16_t switch_tmp = 0;
    
	if(p_task->task_controller.init == 1)
	{ 
        Cpu_TaskParamInit(p_task, gui_display_time, true, true);
		
        p_task->child_task_id = 1;
        
        switch_page = 0;
        switch_num = 0;
        switch_state[0][0] = p_flash_param->dev_config_s.alarm_enable;
        switch_state[0][1] = p_flash_param->dev_config_s.ble_disconnect_enable%2;
        switch_state[0][2] = p_flash_param->sedentary_config_s.enable;
        switch_state[1][0] = p_flash_param->turnwrist_s.enable;
        switch_state[1][1] = p_flash_param->dev_config_s.blood_detect_enable;
        switch_state[1][2] = p_flash_param->dev_config_s.heart_detect_enable;
        switch_state[2][0] = p_flash_param->heart_alert_s.enable;
        switch_state[2][1] = p_flash_param->blood_oxygen_config_s.enable;
        switch_state[2][2] = p_flash_param->dev_config_s.low_bo_alert_enable;
        switch_state[3][0] = 0;
	} 
	
#if	LED_ON_OFF_ON_DEF	
	if(p_task->gui_controller.animation_tick_counter == 0)
	{
		Gui_LcdLedOff();
	}
#endif	

	if(((p_task->gui_controller.animation_tick_counter%3) == 0)&&(p_task->gui_controller.repaint_enable == 1))
	{    
        Gui_PageSwitch(p_task->gui_controller.animation_frame_index++, switch_page, switch_num, switch_state[switch_page%4], display_language);
	}

#if	LED_ON_OFF_ON_DEF	
	if(p_task->gui_controller.animation_tick_counter == 0)
	{
		Gui_LcdLedOn();
	}
#endif	
    
    if(p_task->gui_controller.repaint_enable == 1)
    {
        if(p_device->key == BUTTON_EVENT_CLICK)
        {
            switch_num++;
            if(switch_num >= 4 || (switch_page == 3 && switch_num >= 2))
            {
                switch_num = 0;
                switch_page++;
                if(switch_page >= 3)
                {
                    switch_page = 0;
                    switch_num = 0;
                }
                task_interval_cnt = 0xFFFF;
                p_task->gui_controller.animation_frame_index = 0;
            }
            p_task->task_controller.run_time = 0;
        }
        else if(p_device->key == BUTTON_EVENT_LONGPRESS)
        {   
            switch_tmp = switch_page*4 + switch_num;
            switch(switch_tmp)
            {
                case 0:case 4:case 8://返回上一个界面
                    Cpu_ScheduleTask(p_task, p_device, CPU_TASK_FUNCT2);
                    break;
                case 1://闹钟开关
                    if(p_flash_param->dev_config_s.alarm_enable == 0)
                    {
                        p_flash_param->dev_config_s.alarm_enable = 1;
                    }
                    else
                    {
                        p_flash_param->dev_config_s.alarm_enable = 0;
                    }
                    switch_state[0][0] = p_flash_param->dev_config_s.alarm_enable;
                    upload_switch_s.switch_flg.alarm_flg = 1;
                    upload_switch_s.switch_flg.dev_config_flg = 1;
                    //重置运行时间
                    p_task->task_controller.run_time = 0;
                    break;
                case 2://蓝牙断连开关
                    if(p_flash_param->dev_config_s.ble_disconnect_enable == 1)
                    {
                        p_flash_param->dev_config_s.ble_disconnect_enable = 0;
                    }
                    else
                    {
                        p_flash_param->dev_config_s.ble_disconnect_enable = 1;
                    }
                    switch_state[0][1] = p_flash_param->dev_config_s.ble_disconnect_enable%2;
                    upload_switch_s.switch_flg.dev_config_flg = 1;
                    //重置运行时间
                    p_task->task_controller.run_time = 0;
                    break;
                case 3://久坐提醒
                    if(p_flash_param->sedentary_config_s.enable == 0)
                    {
                        p_flash_param->sedentary_config_s.enable = 1;
                    }   
                    else
                    {
                        p_flash_param->sedentary_config_s.enable = 0;
                    }
                    switch_state[0][2] = p_flash_param->sedentary_config_s.enable;
                    upload_switch_s.switch_flg.dev_config_flg = 1;
                    //重置运行时间
                    p_task->task_controller.run_time = 0;
                    break;
                case 5://转腕亮屏
                    if(p_flash_param->turnwrist_s.enable == 0)
                    {
                        p_flash_param->turnwrist_s.enable = 1;
                    }
                    else
                    {
                        p_flash_param->turnwrist_s.enable = 0;
                    }
                    switch_state[1][0] = p_flash_param->turnwrist_s.enable;
                    upload_switch_s.switch_flg.dev_config_flg = 1;
                    //重置运行时间
                    p_task->task_controller.run_time = 0;
                    break;
                case 6://血压监测
                    if(p_flash_param->dev_config_s.blood_detect_enable == 1)
                    {
                        p_flash_param->dev_config_s.blood_detect_enable = 0;
                    }   
                    else
                    {
                        p_flash_param->dev_config_s.blood_detect_enable = 1;
                    }
                    switch_state[1][1] = p_flash_param->dev_config_s.blood_detect_enable;
                    upload_switch_s.switch_flg.dev_config_flg = 1;
                    //重置运行时间
                    p_task->task_controller.run_time = 0;
                    break;
                case 7://HRV监测
                    if(p_flash_param->dev_config_s.heart_detect_enable ==  1)
                    {
                        p_flash_param->dev_config_s.heart_detect_enable = 0;
                    }
                    else
                    {
                        p_flash_param->dev_config_s.heart_detect_enable = 1;
                    }
                    switch_state[1][2] = p_flash_param->dev_config_s.heart_detect_enable;
                    upload_switch_s.switch_flg.dev_config_flg = 1;
                    //重置运行时间
                    p_task->task_controller.run_time = 0;
                    
                    break;
                case 9://脉率监测
                    if(p_flash_param->heart_alert_s.enable == 1)
                    {
                        p_flash_param->heart_alert_s.enable = 2;
                    }
                    else
                    {
                        p_flash_param->heart_alert_s.enable = 1;
                    }
                    switch_state[2][0] = p_flash_param->heart_alert_s.enable;
                    upload_switch_s.switch_flg.dev_config_flg = 1;
                    //重置运行时间
                    p_task->task_controller.run_time = 0;
                    
                    break;
                case 10://脉率报警
                    if(p_flash_param->blood_oxygen_config_s.enable == 1)
                    {
                        p_flash_param->blood_oxygen_config_s.enable = 0;
                    }
                    else
                    {
                        p_flash_param->blood_oxygen_config_s.enable = 1;
                    }
                    switch_state[2][1] = p_flash_param->blood_oxygen_config_s.enable;
                    upload_switch_s.switch_flg.dev_config_flg = 1;
                    //重置运行时间
                    p_task->task_controller.run_time = 0;
                    
                    break;
                case 11://血氧检测
                    if(p_flash_param->dev_config_s.low_bo_alert_enable == 1)
                    {
                        p_flash_param->dev_config_s.low_bo_alert_enable = 0;
                    }   
                    else
                    {
                        p_flash_param->dev_config_s.low_bo_alert_enable =1;
                    }
                    switch_state[2][2] = p_flash_param->dev_config_s.low_bo_alert_enable;
                    upload_switch_s.switch_flg.dev_config_flg = 1;
                    //重置运行时间
                    p_task->task_controller.run_time = 0;
                    break;
                default:
                    break;
            }
        }
        switch_state[0][0] = p_flash_param->dev_config_s.alarm_enable;
        switch_state[0][1] = p_flash_param->dev_config_s.ble_disconnect_enable%2;
        switch_state[0][2] = p_flash_param->sedentary_config_s.enable;
        switch_state[1][0] = p_flash_param->turnwrist_s.enable;
        switch_state[1][1] = p_flash_param->dev_config_s.blood_detect_enable;
        switch_state[1][2] = p_flash_param->dev_config_s.heart_detect_enable;
        switch_state[2][0] = p_flash_param->heart_alert_s.enable;
        switch_state[2][1] = p_flash_param->blood_oxygen_config_s.enable;
        switch_state[2][2] = p_flash_param->dev_config_s.low_bo_alert_enable;
        switch_state[3][0] = 0;
        
        p_device->key = BUTTON_EVENT_NULL;
    }	
}
/****************************************************************
** Function name:           Cpu_LightLevelSetTask
** Descriptions:            亮度调节界面
** input parameters:        p_controller：UI事件控制状态；p_gui：显示相关的参数
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-08-12
*****************************************************************/ 
void Cpu_LightLevelSetTask(cpu_task_tcb_t* p_task, cpu_device_tcb_t* p_device)
{
    if(p_task->task_controller.init == 1)
	{ 
        Cpu_TaskParamInit(p_task, gui_display_time, true, true);
        p_task->child_task_id = 1; 
	}
	
#if	LED_ON_OFF_ON_DEF	
	if(p_task->gui_controller.animation_tick_counter == 0)
	{
		Gui_LcdLedOff();
	}
#endif	
	
	if(((p_task->gui_controller.animation_tick_counter%6) == 0)&&(p_task->gui_controller.repaint_enable == 1))
	{
        Gui_PageLightLevelSet(p_task->gui_controller.animation_frame_index++,  p_data_manage->oled_bright, display_language); 
	}
	
#if	LED_ON_OFF_ON_DEF		
	if(p_task->gui_controller.animation_tick_counter == 0)
	{
		Gui_LcdLedOn();
	}
#endif		
	
	if(p_task->gui_controller.repaint_enable == 1)
    {   
        if(p_device->key == BUTTON_EVENT_CLICK)
        {
            //亮度等级加1
            if(p_data_manage->oled_bright < BACKLIGHT_MAX_CHANAGE_LEVEL)
            {
                //if(p_data_manage->oled_bright == p_flash_param->brightness_s.valid_bright)
                {
                    //p_flash_param->brightness_s.valid_bright++;
                }
                //if(p_data_manage->oled_bright == p_flash_param->brightness_s.other_bright)
                {
                    //p_flash_param->brightness_s.other_bright++;
                }
                p_data_manage->oled_bright++;
                p_flash_param->brightness_s.other_bright =  p_data_manage->oled_bright;
                p_flash_param->brightness_s.valid_bright =  p_data_manage->oled_bright;
                
            }
            else
            {
                //if(p_data_manage->oled_bright == p_flash_param->brightness_s.valid_bright)
                {
                    p_flash_param->brightness_s.valid_bright = 1;
                }
                //if(p_data_manage->oled_bright == p_flash_param->brightness_s.other_bright)
                {
                    p_flash_param->brightness_s.other_bright = 1;
                }
                p_data_manage->oled_bright = 1;
            }
            p_device->oled.bright = 1;
            upload_switch_s.switch_flg.oled_bright_flg = 1;
            
            DM_Update_Brightness(p_data_manage->oled_bright);
            Gui_ThreadBrightAdjustTaskWakeUp();
//            cpu.device_tcb.oled.light = 1;
            //重置运行时间
            Cpu_TaskParamInit(p_task, gui_display_time, true, true);
        }
		else if(p_device->key == BUTTON_EVENT_LONGPRESS)
        {   
            Cpu_ScheduleTask(p_task, p_device, CPU_TASK_FUNCT2);
        }
        p_device->key = BUTTON_EVENT_NULL;
    }
}
/****************************************************************
** Function name:           Cpu_DispMessage3Task
** Descriptions:            消息界面处理
** input parameters:        p_controller：UI事件控制状态；p_gui：显示相关的参数
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-08-12
*****************************************************************/ 
void Cpu_DispMessage3Task(cpu_task_tcb_t* p_task, cpu_device_tcb_t* p_device)
{
    uint8_t quality = 0;
    if(p_task->task_controller.init == 1)
    {
        Cpu_TaskParamInit(p_task,gui_display_time,true,true);
        p_task->child_task_id = 1;

    }    

#if LED_ON_OFF_ON_DEF
    if(p_task->gui_controller.animation_tick_counter == 0)
    {
        Gui_LcdLedOff();
    } 
#endif
 
    // 客户要求刷新速率为帧/0.3s
    if(((p_task->gui_controller.animation_tick_counter%3) == 0)&&(p_task->gui_controller.repaint_enable == 1))
    {
        if(p_task->child_task_id != 1)
		{
			p_task->child_task_id = 1;
            p_task->gui_controller.animation_frame_index = 0;
		}

        Gui_PageMessage3(p_task->gui_controller.animation_frame_index++,display_language);
    } 
    
#if LED_ON_OFF_ON_DEF
    //YX.20180830.增加延时，去掉显示的闪屏问题
    if(p_task->gui_controller.animation_tick_counter == 0)
    {
         
        Gui_LcdLedOn();
    } 
#endif
  
    // 按键短按,切换到下一页
    if(p_task->gui_controller.repaint_enable == 1)
    {
        if(p_device->key == BUTTON_EVENT_CLICK)
        {
            Cpu_ScheduleTask(p_task, p_device, CPU_TASK_FUNCT);
        }
        else if(p_device->key == BUTTON_EVENT_LONGPRESS)
        {   
            Cpu_ScheduleTask(p_task, p_device, CPU_TASK_DISP_MESSAGE); //消息选择显示界面
        }	
        p_device->key = BUTTON_EVENT_NULL;        
    } 	
}

/****************************************************************
** Function name:           Cpu_DispMessageTask
** Descriptions:            显示信息推送界面
** input parameters:        p_controller：UI事件控制状态；p_gui：显示相关的参数
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-08-12
*****************************************************************/ 
void Cpu_DispMessageTask(cpu_task_tcb_t* p_task, cpu_device_tcb_t* p_device)
{
    uint8_t slid_ret = 0;
    static uint8_t disp_time = 0;
    static page_message_param_t menu_message = {0};
    if(p_task->task_controller.init == 1)
	{ 
        Cpu_TaskParamInit(p_task, gui_display_time, true, true);
        p_task->child_task_id = 1; 

        disp_time = 0;
        memset(&menu_message, 0 ,sizeof(page_message_param_t));
//        Gui_CalcMessageDispInfo(&menu_message);
	}
	
#if	LED_ON_OFF_ON_DEF	
	if(p_task->gui_controller.animation_tick_counter == 0)
	{
		Gui_LcdLedOff();
	}
#endif	
	
    
	if(((p_task->gui_controller.animation_tick_counter%3) == 0)&&(p_task->gui_controller.repaint_enable == 1))
	{
        Gui_PageDispMessage(p_task->gui_controller.animation_frame_index++, menu_message, display_language); 
        if(menu_message.disp_data_num == 0)
        {
            disp_time++;
            if(disp_time >= 8)
            {
                Cpu_ScheduleTask(p_task, p_device, CPU_TASK_MESSAGE);
            }
        }
	}
	
#if	LED_ON_OFF_ON_DEF
	if(p_task->gui_controller.animation_tick_counter == 0)
	{
		Gui_LcdLedOn();
	}
#endif		
	
	if(p_task->gui_controller.repaint_enable == 1)
    {   
        if(p_device->key == BUTTON_EVENT_CLICK)
        {
            menu_message.message_touch_num++;
            if(menu_message.message_touch_num >= menu_message.disp_data_num+2)
            {
                menu_message.message_touch_num = 0;
            }
            p_task->task_controller.run_time = 0;
        }
		else if(p_device->key == BUTTON_EVENT_LONGPRESS)
        {   
            if(menu_message.message_touch_num == 0)
            {
                Cpu_ScheduleTask(p_task, p_device, CPU_TASK_FUNCT2);
            }
            else if(menu_message.message_touch_num == 1)
            {
                //清除所有的信息
//                Gui_ClearMessageDispInfo();
                
                Cpu_TaskParamInit(p_task, gui_display_time, true, true);
                
                disp_time = 0;
//                memset(&menu_message, 0 ,sizeof(page_message_param_t));
                p_task->task_controller.run_time = 0;
//                Cpu_ScheduleTask(p_task, p_device, CPU_TASK_FUNCT2);
            }
            else
            {
//                memset(&message_data_tmp, 0, sizeof(message_notify_attr_t));
//                Gui_GetMessageInfo(&message_data_tmp, menu_message.message_touch_num-2);
                Cpu_ScheduleTask(p_task, p_device, CPU_TASK_DISP_MESSAGE2);
            }
        }
        p_device->key = BUTTON_EVENT_NULL;
    }
}
/****************************************************************
** Function name:           Cpu_DispMessage2Task
** Descriptions:            显示信息推送界面
** input parameters:        p_controller：UI事件控制状态；p_gui：显示相关的参数
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-08-12
*****************************************************************/ 
void Cpu_DispMessage2Task(cpu_task_tcb_t* p_task, cpu_device_tcb_t* p_device)
{
//    uint8_t slid_ret = 0;
//    static uint16_t disp_page_num[20] = {0};    //每一页的数据节点
//    static uint16_t disp_page_cnt = 0;
    uint16_t disp_page_num_tmp = 0;
    static uint16_t disp_len_max = 0;      //往后还可以显示的最大长度的数据
//    uint8_t menu_message[300] = {0};
    
    if(p_task->task_controller.init == 1)
	{ 
        Cpu_TaskParamInit(p_task, gui_display_time, false, true);
        p_task->child_task_id = 1; 
		 
        
//        memset(disp_page_num, 0, 20);
//        disp_page_cnt = 0;
//        disp_len_max = message_data_tmp.len;
//        memcpy(menu_message, &message_data_tmp.content, disp_len_max);
	}
	
#if	LED_ON_OFF_ON_DEF	
	if(p_task->gui_controller.animation_tick_counter == 0)
	{

		Gui_LcdLedOff();
	}
#endif	
	
    if(p_task->gui_controller.repaint_enable == 1)
    {   
        if(p_device->key == BUTTON_EVENT_CLICK)
        {
//            //已经显示消息内容时，往下翻页
//            if((disp_len_max - disp_page_num[disp_page_cnt + 1]) > 0)
//            {
//                p_task->task_controller.run_time = 0;
//                p_task->gui_controller.animation_tick_counter = 0;
//                
//                disp_page_cnt++;
//                disp_len_max = disp_len_max - disp_page_num[disp_page_cnt];
//                memcpy(menu_message, &message_data_tmp.content[disp_page_num[disp_page_cnt]], disp_len_max);
//            }
//            else
//            {
//                Cpu_ScheduleTask(p_task, p_device, CPU_TASK_DISP_MESSAGE);
//            }
        }
		else if(p_device->key == BUTTON_EVENT_LONGPRESS)
        {   
            Cpu_ScheduleTask(p_task, p_device, CPU_TASK_DISP_MESSAGE);
        }
        p_device->key = BUTTON_EVENT_NULL;
    }
    
	if(((p_task->gui_controller.animation_tick_counter%3) == 0)&&(p_task->gui_controller.repaint_enable == 1))
	{

//        disp_page_num_tmp = disp_page_num[disp_page_cnt];
        
        if(task_interval_cnt == 0)//1)
        {
//            Gui_PageMessageNotificationIcon02(p_task->gui_controller.animation_frame_index++, message_data_tmp.type, 0, message_data_tmp.message_time_s.hour, 
//                                              message_data_tmp.message_time_s.minutes, display_language);
//            disp_page_num[disp_page_cnt+1] = disp_page_num_tmp + Gui_PageMessageNotificationContent02(task_interval_cnt, menu_message, disp_len_max, 0);

        }
	}
	
#if	LED_ON_OFF_ON_DEF
	if(p_task->gui_controller.animation_tick_counter == 0)
	{
		Gui_LcdLedOn();
	}
#endif		

}

/****************************************************************
** Function name:           Cpu_LightTimeSetTask
** Descriptions:            亮屏时间调节界面
** input parameters:        p_controller：UI事件控制状态；p_gui：显示相关的参数
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-08-12
*****************************************************************/ 
void Cpu_LightTimeSetTask(cpu_task_tcb_t* p_task, cpu_device_tcb_t* p_device)
{
    uint8_t slid_ret = 0;
    if(p_task->task_controller.init == 1)
	{ 
        Cpu_TaskParamInit(p_task, gui_display_time, true, true);
        p_task->child_task_id = 1; 
	}
	
#if	LED_ON_OFF_ON_DEF	
	if(p_task->gui_controller.animation_tick_counter == 0)
	{
		Gui_LcdLedOff();
	}
#endif	
	
	if(((p_task->gui_controller.animation_tick_counter%3) == 0)&&(p_task->gui_controller.repaint_enable == 1))
	{
        Gui_PageLightTimeSet(p_task->gui_controller.animation_frame_index++,  p_flash_param->ui_display_time, display_language);       
	}
	
#if	LED_ON_OFF_ON_DEF
	if(p_task->gui_controller.animation_tick_counter == 0)
	{
		Gui_LcdLedOn();
	}
#endif		
	
	if(p_task->gui_controller.repaint_enable == 1)
    {   
        if(p_device->key == BUTTON_EVENT_CLICK)
        {   
            if(p_flash_param->ui_display_time < UI_DISPLAY_TIME_MAX)
            {
                p_flash_param->ui_display_time++;
            }
            else
            {
                p_flash_param->ui_display_time = UI_DISPLAY_TIME_MIN;
            }
            upload_switch_s.switch_flg.display_time_flg = 1;
            //重置运行时间
            Cpu_TaskParamInit(p_task, gui_display_time, false, true);
        }
		else if(p_device->key == BUTTON_EVENT_LONGPRESS)
        {   
            Cpu_ScheduleTask(p_task, p_device, CPU_TASK_FUNCT2);
        }
        p_device->key = BUTTON_EVENT_NULL;
    }
}
/****************************************************************
** Function name:           Cpu_ClearDataTask
** Descriptions:            清除数据界面
** input parameters:        p_controller：UI事件控制状态；p_gui：显示相关的参数
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-08-12
*****************************************************************/ 
void Cpu_ClearDataTask(cpu_task_tcb_t* p_task, cpu_device_tcb_t* p_device)
{
    uint8_t slid_ret = 0;
    static uint8_t disp_tmp = 0;
    if(p_task->task_controller.init == 1)
	{ 
        Cpu_TaskParamInit(p_task, gui_display_time, true, true);
        p_task->child_task_id = 1; 

        disp_tmp = 0;
	}
	
#if	LED_ON_OFF_ON_DEF	
	if(p_task->gui_controller.animation_tick_counter == 0)
	{
		Gui_LcdLedOff();
	}
#endif	
    
	if(((p_task->gui_controller.animation_tick_counter%6) == 0)&&(p_task->gui_controller.repaint_enable == 1))
	{
        Gui_PageClearData(p_task->gui_controller.animation_frame_index++,  disp_tmp, display_language); 
	}
	
#if	LED_ON_OFF_ON_DEF
	if(p_task->gui_controller.animation_tick_counter == 0)
	{
		Gui_LcdLedOn();
	}
#endif		
	
	if(p_task->gui_controller.repaint_enable == 1)
    {   
        if(p_device->key == BUTTON_EVENT_CLICK)
        {
            if(disp_tmp == 0)
            {
                disp_tmp = 1;
            }
            else
            {
                disp_tmp = 0;
            }
            p_task->task_controller.run_time = 0;
        }
		else if(p_device->key == BUTTON_EVENT_LONGPRESS)
        {   
            if(disp_tmp == 1)
            {
                //清除数据
                Movement_RestoreFactoryExit();
                p_data_manage->app_factory_clear_s.clear_data = 0x01;
                BleThread_FactoryClearData();
                GuiThread_AddEvent(CPU_EVENT_TASK_POWEROFF);
//                Cpu_ScheduleTask(p_task, p_device, CPU_TASK_FUNCT2);
            }
            else
            {
                Cpu_ScheduleTask(p_task, p_device, CPU_TASK_FUNCT2);
            }
        }
        p_device->key = BUTTON_EVENT_NULL;
    }
}


