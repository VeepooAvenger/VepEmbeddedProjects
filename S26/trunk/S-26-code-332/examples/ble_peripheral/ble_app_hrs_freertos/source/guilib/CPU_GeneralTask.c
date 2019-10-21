/*******************Copyright(c)*********************************
** Veepoo Tech
** COPYRIGHT (C) 2016-2019
**----------------------FileInfo---------------------------------
** File name:                cpu_general_task.c
** Last modified Date:       2017-07-10
** Last SYS_VERSION:         0.0.1
** Descriptions:
**---------------------------------------------------------------
** Createdby:                ����
** Createddate:              2017-07-10
** SYS_VERSION:              0.0.1
** Descriptions:             cpuһ������
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
#include "Blood_OxygenDetection.h"
#include "AFE4900_entry.h"
#include "data_manage.h"
#include "battery_adc.h"
#include "log.h"
#include "ble_thread.h"
#include "util.h"
#include "movement_patterns.h"
#include "gui_page_display.h"
#include "Gui_Thread.h"
#include "gui_data_inteface.h"
static uint32_t all_step = 0;                //�ܲ���
static uint32_t distance_m = 0;              //����  ��λ����
static float   distance_km = 0;              //����  ��λ��ǧ��
static float   distance_mile = 0;            //����  ��λ��Ӣ��
static float   calories_buf = 0;             //��·��

static uint8_t icon_counter = 0;            // ͼ����ʾ����

extern uint8_t ui_style_type;
extern uint8_t heart_init_cause;
extern uint8_t heart_calib_init_cause;
void Cpu_TaskParamInit(cpu_task_tcb_t* p_task,uint16_t keep_time,bool press_hold,bool animation_active)
{
    // �����ʼ��
    p_task->task_controller.init = 0;
    p_task->task_controller.active = 1; 
    p_task->task_controller.keep_time = keep_time; 
    p_task->task_controller.run_time = 0;
    p_task->task_controller.key_press_hold = press_hold; 

    p_task->gui_controller.animation_enable = animation_active;
    // ע��:���������ʼ��Ϊ0xFF,��Ҫ����̬ҳ��ˢ�¿���
    p_task->gui_controller.page_frame_counter = 0xFF;
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
     p_device->oled.light = 1; 
     p_task->task_id = CPU_TASK_TIMER;
     p_task->task_controller.keep_time = 5;
     p_task->task_controller.run_time = 0;
     p_task->task_controller.key_press_hold = 1;//����˲�����bug           
     p_task->task_controller.active = 1; 
     //YX.20180628.�򿪶���ˢ�±�־
     p_task->gui_controller.animation_enable = 1;
}

void Cpu_IdelToRunTask(cpu_task_tcb_t* p_task, cpu_device_tcb_t* p_device)
{
     p_device->oled.light = 1; 
     p_task->task_id = CPU_TASK_MOVEMENT;
     p_task->task_controller.keep_time = 7;
     p_task->task_controller.run_time = 0;
     p_task->task_controller.key_press_hold = 1;         
     p_task->task_controller.active = 1; 
     //YX.20180628.�򿪶���ˢ�±�־
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
** Descriptions:             ������
** input parameters:         p_controller��       CPU���������ָ��
                             p_gui��              ��̬ҳ�������ָ��
                             p_oled��             oled����ָ��
** output parameters:        ��
** Returned value:           ��
** Created by:               GT
** Created Date:             2017-07-10
****************************************************************/
void Cpu_MainTask(cpu_task_tcb_t* p_task, cpu_device_tcb_t* p_device)
{
    uint8_t time_format = 0;
	uint8_t hour = 0;
    static uint8_t last_minute = 0;
    uint16_t step_prg = 0;
    uint8_t phys_menstrual = 0;

    if(p_task->task_controller.init == 1)
    {
        p_data_manage->heart_measure_s.btn = 0;
        p_data_manage->blood_oxygen_measure_s.btn = 0;	
		Cpu_TaskParamInit(p_task, 5, true, true);
        p_task->child_task_id = 1;
        //�������
        Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
                                p_device->cls_param.column_size, p_device->cls_param.page_size);
    }
    
	if((p_task->gui_controller.animation_tick_counter%6 == 0)&&(p_task->gui_controller.repaint_enable == 1))
    {
        
		if(p_task->child_task_id != 1)
		{
			//�������
			Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
									p_device->cls_param.column_size, p_device->cls_param.page_size);
			p_task->child_task_id = 1;
		}
        //������������
        Gui_ThreadSetDisPlayArea(0, 0, 80, 160);
        
		
        hour = Hour_StandardTransform(p_flash_param->dev_config_s.am_pm_enable,clock->hour);
        time_format = Get_AMPM_Format(p_flash_param->dev_config_s.am_pm_enable,clock->hour);
        distance_value(&all_step,&distance_m,&distance_km,&distance_mile,&calories_buf);
        //phys_menstrual = Phys_GetMenstrualPeriodType();
		if(p_task->gui_controller.animation_frame_index == 0 || last_minute != clock->minutes)
		{
            // �޸������,û�е�����---JSH-20180614
            p_task->gui_controller.animation_frame_index = 0;
            
			last_minute = clock->minutes;
            step_prg = Gui_DataGetSportsToMarkProgress();
            Gui_PageMainDateHour(clock->month, clock->day, hour, clock->minutes, clock->week, display_language, 
                                 time_format, phys_menstrual,step_prg);
		}
        Gui_PageMainAssisant(p_task->gui_controller.animation_frame_index, p_data_manage->device_trace_param_s.ble_connect,
                             time_format, Battery_Get_Status(), Battery_Get_Grade(), display_language, all_step, phys_menstrual);
        p_task->gui_controller.animation_frame_index++;
	}
    
    // �����̰�,����ǲ�����
    if(p_task->gui_controller.repaint_enable == 1)
    {
        if((p_device->key == BUTTON_EVENT_CLICK)&&(Battery_Get_Status() == BATTERY_POWER_NORMAL))
        {
            // ע��:�����߳���ʱ,�����л�����
            Cpu_ScheduleTask(p_task, p_device, CPU_TASK_STEP);
        }
		else if((p_device->key == BUTTON_EVENT_LONGPRESS)&&(Battery_Get_Status() == BATTERY_POWER_NORMAL))
		{
			Cpu_ScheduleTask(p_task, p_device, CPU_TASK_MAC);
		}
        p_device->key = BUTTON_EVENT_NULL;
    }
	
}

/****************************************************************
** Function name:           Cpu_MacAdd
** Descriptions:            MAC�Ͱ汾�ŵ���ʾ
** input parameters:        �� 
** output parameters:       ��
** Returned value:          ��
** Created by:              YX              
** Created Date:            2017-12-12
*****************************************************************/ 
void Cpu_MacAdd(cpu_task_tcb_t* p_task, cpu_device_tcb_t* p_device)
{
	static uint8_t dis_mac_data[6] = {0};
    static uint8_t dis_ver_data[2] = {0};

    if(p_task->task_controller.init == 1)
    {
        Cpu_TaskParamInit(p_task, 3,false,true);//false); 
		Gui_DataGetBleMac(dis_mac_data);
        Gui_DataGetVer(dis_ver_data);
        p_task->child_task_id = 1;
        //�������
        Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
                                p_device->cls_param.column_size, p_device->cls_param.page_size);
    }

    if((p_task->gui_controller.animation_tick_counter%10 == 0) && p_task->gui_controller.repaint_enable == 1)
    {
		if(p_task->child_task_id != 1)
		{
			//�������
			Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
									p_device->cls_param.column_size, p_device->cls_param.page_size);
			p_task->child_task_id = 1;
		}
		
		Gui_PageDisMacVer(p_task->gui_controller.animation_frame_index++, dis_mac_data, dis_ver_data);
		//������������
        Gui_ThreadSetDisPlayArea(0, 49, 80, 62);
    }

  if(p_task->gui_controller.repaint_enable == 1)
  {
      if((p_device->key == BUTTON_EVENT_CLICK)&&(Battery_Get_Status() == BATTERY_POWER_NORMAL))
      {
            p_device->key = BUTTON_EVENT_NULL;
            // ע��:�����߳���ʱ,�����л�����
            Cpu_ScheduleTask(p_task, p_device, CPU_TASK_STEP);
      }
      //p_device->key = BUTTON_EVENT_NULL;
  }

}

/****************************************************************
** Function name:            Cpu_TaskRun
** Descriptions:             �˶�ģʽ����
** input parameters:         p_controller��       CPU���������ָ��
                             p_gui��              ��̬ҳ�������ָ��
                             p_oled��             oled����ָ��
** output parameters:        ��
** Returned value:           ��
** Created by:               YX
** Created Date:             2017-11-14
****************************************************************/
void Cpu_TaskRun(cpu_task_tcb_t* p_task, movement_cont_t* p_movement, cpu_device_tcb_t* p_device)
{
    static uint8_t last_ui_style = 0;
    uint8_t time_format = 0;
	uint8_t hour = 0;
    uint16_t step_prg = 0;
    float calories = 0.0f;
    float tdistance_m = 0.0f;
    uint32_t uint_type = 0;
    
    uint8_t phys_menstrual = 0;

    
    if(p_task->task_controller.init == 1)
    {
        if(p_movement->enable == 1)
        {
            p_movement->face_num = 1;
            Cpu_TaskParamInit(p_task,6,false,true);
            p_task->child_task_id = 3;
        }
        else
        {
            p_movement->entry = 1;
            Cpu_TaskParamInit(p_task,4,true,true);
            p_task->child_task_id = 1;
            /*
            p_data_manage->heart_measure_s.btn = 0;
            p_data_manage->blood_oxygen_measure_s.btn = 0;

            BleThread_AFE4404DevClose();
			*/
        }
        //�������
        Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
                                p_device->cls_param.column_size, p_device->cls_param.page_size);
        p_task->gui_controller.animation_frame_index = 0;
    }
    
    if(p_movement->entry == 1)
    {
        if((p_task->gui_controller.animation_tick_counter%4 == 0)&&(p_task->gui_controller.repaint_enable == 1))
        {
			if(p_task->child_task_id != 1)
			{
				//�������
				Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
										p_device->cls_param.column_size, p_device->cls_param.page_size);
				p_task->child_task_id = 1;
                p_task->gui_controller.animation_frame_index = 0;
			}
			//������������
            Gui_ThreadSetDisPlayArea(0, 0, 80, 160);
            //��ʾ�˶�ģʽ�������
			Gui_PageRunMode(p_task->gui_controller.animation_frame_index++,display_language);
            
        }
        ///Gui_ButtonDynamic(p_task->gui_controller.animation_frame_index - 1);
    }
    else
    {
        // �˶�ģʽ״̬
        if(p_movement->enable == 1)
        {
            // �޸��˶�ģʽ��������---JSH-20180614
            if((p_task->gui_controller.animation_tick_counter%2 == 0)&&(p_task->gui_controller.repaint_enable == 1))
            {

                if(((p_task->task_controller.keep_time == 6)&&(p_task->task_controller.run_time >= 5))
                   ||((p_task->task_controller.keep_time == 7)&&(p_task->task_controller.run_time >= 6)))
                {
                    if(p_device->key == BUTTON_EVENT_CLICK)
                    {
                        p_task->task_controller.keep_time = 7;
                        p_task->task_controller.run_time = 0;
                        p_movement->face_num = 1;
                    }
                    else
                    {
                        p_movement->face_num = 0;
                    }
                    
                    p_device->key = BUTTON_EVENT_NULL;	
                    
					if(p_task->child_task_id != 2)
					{
						//�������
						Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
												p_device->cls_param.column_size, p_device->cls_param.page_size);
						p_task->child_task_id = 2;
						p_task->gui_controller.animation_frame_index = 0;
                        last_ui_style = ui_style_type;
					}
                    if(last_ui_style != ui_style_type)
                    {
                        last_ui_style = ui_style_type;
                        //�������
						Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
												p_device->cls_param.column_size, p_device->cls_param.page_size);
                        p_task->gui_controller.animation_frame_index = 0;
                    }
                    hour = Hour_StandardTransform(p_flash_param->dev_config_s.am_pm_enable,clock->hour);
                    time_format = Get_AMPM_Format(p_flash_param->dev_config_s.am_pm_enable,clock->hour);
                    distance_value(&all_step,&distance_m,&distance_km,&distance_mile,&calories_buf);
                    //phys_menstrual = Phys_GetMenstrualPeriodType();
                    if(p_task->gui_controller.animation_frame_index == 0)
                    {
                        step_prg = Gui_DataGetSportsToMarkProgress();
                        Gui_PageMainDateHour(clock->month, clock->day, hour, clock->minutes, clock->week, display_language, 
                                             time_format, phys_menstrual, step_prg);
                        
                        Gui_PageMainAssisant(p_task->gui_controller.animation_frame_index++, p_data_manage->device_trace_param_s.ble_connect,
                                         time_format, Battery_Get_Status(), Battery_Get_Grade(), display_language, all_step, phys_menstrual);
                        
                        icon_counter = 0;
                    }
                    else
                    {
                        icon_counter++;
                        
                        if(icon_counter % 6 == 0)
                        {
                            Gui_PageMainAssisant(p_task->gui_controller.animation_frame_index++, p_data_manage->device_trace_param_s.ble_connect,
                                         time_format, Battery_Get_Status(), Battery_Get_Grade(), display_language, all_step, phys_menstrual);
                        }
                    }
                    
                    //������������
                    Gui_ThreadSetDisPlayArea(0, 0, 80, 160);
                }
                else if((p_task->task_controller.keep_time == 4)&&(p_task->task_controller.run_time >= 3))
                {
                    if(p_device->key == BUTTON_EVENT_CLICK)
                    {
                        if(p_task->task_controller.run_time >= 4)
                        {
                            p_task->task_controller.keep_time = 7;
                            p_task->task_controller.run_time = 0;
                            p_movement->face_num = 1;
                        }
                        else
                        {
                            p_task->task_controller.keep_time = 4;
                            p_task->task_controller.run_time = 0;
                            p_movement->face_num = 2;
                        }
                    }
                    else
                    {
                        p_movement->face_num = 0;
                    }
                    p_device->key = BUTTON_EVENT_NULL;
					if(p_task->child_task_id != 3)
					{
						//�������
						Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
												p_device->cls_param.column_size, p_device->cls_param.page_size);
						p_task->child_task_id = 3;
					}
                    
                    calories = movement_info_s.ui_calories;  
                    tdistance_m = movement_info_s.ui_distance;
					// JSH.20181102.�����£�����ʾӢ��
                    if(p_flash_param->dev_config_s.english_style_enable == 0) // || (display_language == 0))
                    {
                        tdistance_m = tdistance_m / 1000.0f;
                        uint_type = 0;
                    } 
                    else
                    {
                        tdistance_m = ((uint32_t)((tdistance_m / 1000.0f) * 1000)) / 1000.0f;//������λС����
                        tdistance_m = tdistance_m * (0.62138f);
                        uint_type = 1;
                    }
                    calories = calories / 1000.0f;
                    
					Gui_PageRunModeData(movement_info_s.run_vail_time, movement_info_s.ui_heart, tdistance_m,
                                        calories, movement_info_s.ui_step, uint_type,
                                        display_language);
                    //������������
                    Gui_ThreadSetDisPlayArea(0, 0, 80, 160);
                }
                else
                {
                    if(p_movement->face_num == 1)
                    {
						if(p_task->child_task_id != 3)
						{
							//�������
							Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
													p_device->cls_param.column_size, p_device->cls_param.page_size);
							p_task->child_task_id = 3;
						}
						
                        calories = movement_info_s.ui_calories;  
                        tdistance_m = movement_info_s.ui_distance;
						// JSH.20181102.�����£�����ʾӢ��
                        if(p_flash_param->dev_config_s.english_style_enable == 0) //|| (display_language == 0))
                        {
                            tdistance_m = tdistance_m / 1000.0f;
                            uint_type = 0;
                        } 
                        else
                        {
                            tdistance_m = ((uint32_t)((tdistance_m / 1000.0f) * 1000)) / 1000.0f;//������λС����
                            tdistance_m = tdistance_m * (0.62138f);
                            uint_type = 1;
                        }
                        calories = calories / 1000.0f;
						
                        Gui_PageRunModeData(movement_info_s.run_vail_time, movement_info_s.ui_heart, tdistance_m,
                                            calories, movement_info_s.ui_step, uint_type,
                                            display_language);
                        
                        //������������
                        Gui_ThreadSetDisPlayArea(0, 0, 80, 160);
                    }
                    else if(p_movement->face_num == 2)
                    {
                        if(p_movement->run == 1)
                        {
							if(p_task->child_task_id != 4)
							{
								//�������
								Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
														p_device->cls_param.column_size, p_device->cls_param.page_size);
								p_task->child_task_id = 4;
                                p_task->gui_controller.animation_frame_index = 0;
							}
							Gui_PageRunModePause(1,display_language);
                        }
                        else
                        {
							if(p_task->child_task_id != 5)
							{
								//�������
								Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
														p_device->cls_param.column_size, p_device->cls_param.page_size);
								p_task->child_task_id = 5;
                                p_task->gui_controller.animation_frame_index = 0;
							}
							Gui_PageRunModePause(0,display_language);
                        }
                        //������������
                        Gui_ThreadSetDisPlayArea(0, 0, 80, 160);
                    }
                    else if(p_movement->face_num == 3)
                    {
						if(p_task->child_task_id != 6)
						{
							//�������
							Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
													p_device->cls_param.column_size, p_device->cls_param.page_size);
							p_task->child_task_id = 6;
                            p_task->gui_controller.animation_frame_index = 0;
						}
						Gui_PageRunModeExit(display_language);
                        //������������
                        Gui_ThreadSetDisPlayArea(0, 0, 80, 160);
                    }
                }
            }
            
            if(p_movement->face_num == 2 || p_movement->face_num == 3)
            {
                ///Gui_ButtonDynamic(p_task->gui_controller.animation_frame_index - 1);
            }
            
        }
    }

    if((p_task->gui_controller.repaint_enable == 1)&&(p_device->key != BUTTON_EVENT_NULL))
    {
        //
        if((p_device->key == BUTTON_EVENT_LONGPRESS)&&(p_movement->entry == 1))
        {
            // �����˶�ģʽ
            memset(p_movement,0,sizeof(movement_cont_t));
            p_movement->enable = 1;
            p_movement->face_num = 1;
            p_movement->run = 1;
            Movement_Start();
        
            p_task->task_controller.keep_time = 7;
            p_task->task_controller.run_time = 0;
			p_task->task_controller.active = 1;
        }
        else if((p_device->key == BUTTON_EVENT_LONGPRESS)&&(p_movement->enable == 1)&&(p_movement->face_num == 3))
        {
            Movement_Exit();
        }
        else if((p_device->key == BUTTON_EVENT_LONGPRESS)&&(p_movement->enable == 1)&&(p_movement->face_num == 2))
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
            MotorAddEvent(1,0,1,0);//����1�� ��1��
        }
        else if((p_device->key == BUTTON_EVENT_CLICK)&&(p_movement->enable == 1)&&(p_movement->face_num == 1))
        {
            p_task->task_controller.keep_time = 4;
            p_task->task_controller.run_time = 0;
            p_movement->face_num = 2;
            p_task->task_controller.key_press_hold = 1;
        }
        else if((p_device->key == BUTTON_EVENT_CLICK)&&(p_movement->enable == 1)&&(p_movement->face_num == 2))
        {
            p_task->task_controller.keep_time = 4;
            p_task->task_controller.run_time = 0;
            p_movement->face_num = 3;
            p_task->task_controller.key_press_hold = 1;
        }
        else if((p_device->key == BUTTON_EVENT_CLICK)&&(p_movement->enable == 1)&&(p_movement->face_num == 3))
        {
            p_task->task_controller.keep_time = 7;
            p_task->task_controller.run_time = 0;
            p_movement->face_num = 1;
            p_task->task_controller.key_press_hold = 0;
        }
        else if((p_device->key == BUTTON_EVENT_CLICK)&&(p_movement->enable == 1)&&(p_movement->face_num == 0))
        {
            p_task->task_controller.keep_time = 7;
            p_task->task_controller.run_time = 0;
            p_movement->face_num = 1;
            p_task->task_controller.key_press_hold = 0;
        }
        else if(p_movement->enable == 0)
        {
                Cpu_ScheduleTask(p_task, p_device, CPU_TASK_SLEEP);
                memset(p_movement,0,sizeof(movement_cont_t));        
        }
        p_device->key = BUTTON_EVENT_NULL;	 
    }     
}

/****************************************************************
** Function name:            Cpu_StepTask
** Descriptions:             �ǲ�����
** input parameters:         p_controller��       CPU���������ָ��
                             p_gui��              ��̬ҳ�������ָ��
                             p_oled��             oled����ָ��
** output parameters:        ��
** Returned value:           ��
** Created by:               GT
** Created Date:             2017-07-10
****************************************************************/
void Cpu_StepTask(cpu_task_tcb_t* p_task, cpu_device_tcb_t* p_device)
{    
    if(p_task->task_controller.init == 1)
    {
        Cpu_TaskParamInit(p_task,3,false,true);
        p_task->child_task_id = 1;
        //�������
        Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
                                p_device->cls_param.column_size, p_device->cls_param.page_size);        
    }
        
    if(((p_task->gui_controller.animation_tick_counter%4) == 0)&&(p_task->gui_controller.repaint_enable == 1))
    {
        if(p_task->child_task_id != 1)
		{
			//�������
			Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
									p_device->cls_param.column_size, p_device->cls_param.page_size);
			p_task->child_task_id = 1;
		}
		
		//������������
        Gui_ThreadSetDisPlayArea(0, 0, 80, 160);
        distance_value(&all_step,&distance_m,&distance_km,&distance_mile,&calories_buf);

		Gui_PageStep(all_step, Gui_DataGetSportsToMarkProgress(), p_task->gui_controller.animation_frame_index++, display_language);
    } 
    
    if(p_task->gui_controller.repaint_enable == 1)
    {
        if(p_device->key == BUTTON_EVENT_CLICK)
        {
//            Cpu_ScheduleTask(p_task, p_device, CPU_TASK_OXYGEN);
			Cpu_ScheduleTask(p_task, p_device, CPU_TASK_DISTAN);
        }
        p_device->key = BUTTON_EVENT_NULL;
    }
}

/****************************************************************
** Function name:            Cpu_DistanTask
** Descriptions:             �������
** input parameters:         p_controller��       CPU���������ָ��
                             p_gui��              ��̬ҳ�������ָ��
                             p_oled��             oled����ָ��
** output parameters:        ��
** Returned value:           ��
** Created by:               GT
** Created Date:             2017-07-10
****************************************************************/
void Cpu_DistanTask(cpu_task_tcb_t* p_task, cpu_device_tcb_t* p_device)
{    
    if(p_task->task_controller.init == 1)
    {
        Cpu_TaskParamInit(p_task,3,false,true);
        p_task->child_task_id = 1;
        //�������
        Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
                                p_device->cls_param.column_size, p_device->cls_param.page_size);        
    }
        
    if(((p_task->gui_controller.animation_tick_counter%4) == 0)&&(p_task->gui_controller.repaint_enable == 1))
    {
        if(p_task->child_task_id != 1)
		{
			//�������
			Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
									p_device->cls_param.column_size, p_device->cls_param.page_size);
			p_task->child_task_id = 1;
		}
		
        //������������
        Gui_ThreadSetDisPlayArea(0, 0, 80, 160);
        distance_value(&all_step,&distance_m,&distance_km,&distance_mile,&calories_buf);
		// JSH.20181102.�����£�����ʾӢ��
		if(p_flash_param->dev_config_s.english_style_enable == 0)
        {
			Gui_PageDistance(distance_km,p_task->gui_controller.animation_frame_index++,p_flash_param->dev_config_s.english_style_enable,display_language);
        }
        else
        {
			Gui_PageDistance(distance_mile,p_task->gui_controller.animation_frame_index++,p_flash_param->dev_config_s.english_style_enable,display_language);
        }
    } 
    
    if(p_task->gui_controller.repaint_enable == 1)
    {
        if(p_device->key == BUTTON_EVENT_CLICK)
        {
            Cpu_ScheduleTask(p_task, p_device, CPU_TASK_CALOR);
        }
        p_device->key = BUTTON_EVENT_NULL;
    }
}

/****************************************************************
** Function name:            Cpu_CalorTask
** Descriptions:             ��·�����
** input parameters:         p_controller��       CPU���������ָ��
                             p_gui��              ��̬ҳ�������ָ��
                             p_oled��             oled����ָ��
** output parameters:        ��
** Returned value:           ��
** Created by:               GT
** Created Date:             2017-07-10
****************************************************************/
void Cpu_CalorTask(cpu_task_tcb_t* p_task, cpu_device_tcb_t* p_device)
{    
    if(p_task->task_controller.init == 1)
    {
        Cpu_TaskParamInit(p_task,3,false,true);
        p_task->child_task_id = 1;
        //�������
        Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
                                p_device->cls_param.column_size, p_device->cls_param.page_size);        
    }
        
    if(((p_task->gui_controller.animation_tick_counter%6) == 0)&&(p_task->gui_controller.repaint_enable == 1))
    {
        if(p_task->child_task_id != 1)
		{
			//�������
			Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
									p_device->cls_param.column_size, p_device->cls_param.page_size);
			p_task->child_task_id = 1;
		}
		
        //������������
        Gui_ThreadSetDisPlayArea(0, 0, 80, 160);
        distance_value(&all_step,&distance_m,&distance_km,&distance_mile,&calories_buf);

		Gui_PageCalories(calories_buf/1000.0f,p_task->gui_controller.animation_frame_index++,display_language);
        
    } 
    
    if(p_task->gui_controller.repaint_enable == 1)
    {
        if(p_device->key == BUTTON_EVENT_CLICK)
        {
            Cpu_ScheduleTask(p_task, p_device, CPU_TASK_OXYGEN);
//			Cpu_ScheduleTask(p_task, p_device, CPU_TASK_DRINK);
        }
        p_device->key = BUTTON_EVENT_NULL;
    }
}

/****************************************************************
** Function name:            Cpu_OxygenTask
** Descriptions:             Ѫ������
** input parameters:         p_controller��       CPU���������ָ��
                             p_gui��              ��̬ҳ�������ָ��
                             p_oled��             oled����ָ��
** output parameters:        ��
** Returned value:           ��
** Created by:               GT
** Created Date:             2017-07-10
****************************************************************/
void Cpu_OxygenTask(cpu_task_tcb_t* p_task, cpu_device_tcb_t* p_device)
{   
    uint8_t oxygen_value = 0;
    static uint8_t ox_error = 0;
    
    if(p_task->task_controller.init == 1)
    {       
        ox_error = 0;
        // ��ʼ����������
        p_data_manage->hrv_measure_s.time = 0;
        p_data_manage->respiratory_rate_s.time = 0;
        p_data_manage->heart_measure_s.day_time = 0;
        p_data_manage->heart_measure_s.night_time = 0;
        p_data_manage->heart_measure_s.btn = 0;
        p_data_manage->blood_oxygen_measure_s.time = 0;
        p_data_manage->blood_oxygen_measure_s.result = 0;
        p_data_manage->blood_oxygen_measure_s.ox_day_time = 0; //�����ǲ��ǰ��쿪����
        p_data_manage->blood_oxygen_measure_s.ox_calc_cnt = 0; // У׼��ɺ󣬼�����������ڰ����Զ����� 
        p_data_manage->blood_oxygen_measure_s.ox_prompt_sign_flag = 0;

        p_data_manage->blood_oxygen_measure_s.algorithm_flg = 0;
        p_data_manage->blood_oxygen_measure_s.add_sub_flg = 0;
        p_data_manage->blood_oxygen_measure_s.offse_buf = 0;
        p_data_manage->blood_oxygen_measure_s.oxy_buf = 0;
       
        p_data_manage->bp_measure_s.bp_btn = 0;
        p_data_manage->bp_measure_s.bp_day_time = 0;
        p_data_manage->bp_measure_s.bp_night_time = 0;	   
	    
        p_task->child_task_id = 1;
//        progress_bk = 0;
		
		if(Battery_Get_Grade() == 0)
		{
            p_data_manage->wear_measure_s.wear_status = 0;
			p_data_manage->blood_oxygen_measure_s.btn = 0;
			Cpu_TaskParamInit(p_task,5,false,true);
			p_data_manage->blood_oxygen_measure_s.cal_state = 0x01;
			p_data_manage->blood_oxygen_measure_s.cal_progress = 0;
		}
		else
		{
			p_data_manage->blood_oxygen_measure_s.btn = 1;
			
			if(p_data_manage->blood_oxygen_measure_s.app == 0)
			{
				p_data_manage->blood_oxygen_measure_s.init = 1;
				// !ע��:Ѫ���㷨��Ҫ���������㷨,�����㷨��Ҫ��ʼ�� Add by GT 2017-10-17
				p_data_manage->wear_measure_s.wear_status = 0;
				p_data_manage->blood_oxygen_measure_s.cal_state = 0x01;
				p_data_manage->blood_oxygen_measure_s.cal_progress = 8;
				
				BleThread_AFE4404BloodOxygenOpen();
			}
		
			Cpu_TaskParamInit(p_task, 300, false, true);
		}
        //�������
        Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
                                p_device->cls_param.column_size, p_device->cls_param.page_size);
    }
    
    // ����������    
    if((p_task->task_controller.run_time >= 299) || (p_data_manage->wear_measure_s.wear_status == 1))
    {
        BleThread_AFE4404DevClose();
        memset(p_task, 0, sizeof(cpu_task_tcb_t));
    }
        
    // Ѫ������ˢ��(֡/0.3S)
    if(((p_task->gui_controller.animation_tick_counter%6) == 0)&&(p_task->gui_controller.repaint_enable == 1))
    {   
		//������������
        Gui_ThreadSetDisPlayArea(0, 0, 80, 160);
    
        if(p_task->child_task_id !=  1)
        {
            //�������
            Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
                                    p_device->cls_param.column_size, p_device->cls_param.page_size);
            p_task->child_task_id = 1;
            p_task->gui_controller.animation_frame_index = 0;
        }
        
        oxygen_value = p_data_manage->blood_oxygen_measure_s.result;

        uint16_t test_prg = 100;
        if(p_data_manage->blood_oxygen_measure_s.cal_state != 0 && p_data_manage->blood_oxygen_measure_s.btn == 1)
        {
            test_prg = p_data_manage->blood_oxygen_measure_s.cal_progress;
        } 
        if(p_data_manage->blood_oxygen_measure_s.btn == 1)
        {
            // �źŲ�����Ҫ����Ҫ�ص�
            ox_error = p_data_manage->blood_oxygen_measure_s.ox_prompt_sign_flag;
        }
        Gui_OxygenPage(p_task->gui_controller.animation_frame_index++, oxygen_value, test_prg, ox_error);		
    }  
    
    if(ox_error == 1 && p_data_manage->blood_oxygen_measure_s.btn == 1)
    {// �źŲ�����Ҫ����Ҫ�ص�
        p_data_manage->blood_oxygen_measure_s.btn = 0;
        BleThread_AFE4404DevClose();
        
        p_task->task_controller.run_time = 0;
        p_task->task_controller.keep_time = 3;
    }
    
    
    // �������,���뵽���ʲ�������
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
** Function name:            Cpu_HeartTask
** Descriptions:             ��������
** input parameters:         p_controller��       CPU���������ָ��
                             p_gui��              ��̬ҳ�������ָ��
                             p_oled��             oled����ָ��
** output parameters:        ��
** Returned value:           ��
** Created by:               GT
** Created Date:             2017-07-10
****************************************************************/
void Cpu_HeartTask(cpu_task_tcb_t* p_task, cpu_device_tcb_t* p_device)
{
    static uint8_t heart = 0;
    
    if(p_task->task_controller.init == 1)
    {								
		// ��ʼ����������
		p_data_manage->blood_oxygen_measure_s.btn = 0;
		p_data_manage->blood_oxygen_measure_s.time = 0;
		p_data_manage->hrv_measure_s.time = 0;
		p_data_manage->respiratory_rate_s.time = 0;
		p_data_manage->heart_measure_s.day_time = 0;
		p_data_manage->heart_measure_s.night_time = 0;
        p_data_manage->heart_measure_s.real_heart = 0;
		p_task->child_task_id = 1;
		if(Battery_Get_Grade() == 0)
		{
            p_data_manage->wear_measure_s.wear_status = 0;
			p_data_manage->heart_measure_s.btn = 0;
			Cpu_TaskParamInit(p_task,5,false,true);	      
		}
		else
		{
			p_data_manage->heart_measure_s.btn = 1;

			if(p_data_manage->heart_measure_s.app == 0)
			{
                heart_init_cause = 6;
				p_data_manage->heart_measure_s.init = 1;
				p_data_manage->wear_measure_s.wear_status = 0;
                p_data_manage->heart_measure_s.calib_well = 0;
                p_data_manage->heart_measure_s.calib_init = 1;
                heart_calib_init_cause = 4;
				
				BleThread_AFE4404HeartOpen();
			}
			
			Cpu_TaskParamInit(p_task,61,false,true);
		}
        heart = 0;
        //�������
        Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
                                p_device->cls_param.column_size, p_device->cls_param.page_size);
    } 
    
    if((p_task->task_controller.run_time >= 60) || (p_data_manage->wear_measure_s.wear_status == 1))
    {
//        AFE4404_LOG_PRINT("Heart UI Close!\n");
        BleThread_AFE4404DevClose();
        memset(p_task, 0, sizeof(cpu_task_tcb_t));
    }

    // �޸���ر��� 
    if(((p_task->gui_controller.animation_tick_counter%3) == 0)&&(p_task->gui_controller.repaint_enable == 1))
    {
        if(p_task->child_task_id != 1)
		{
			//�������
			Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
									p_device->cls_param.column_size, p_device->cls_param.page_size);
			p_task->child_task_id = 1;
		}
		//������������
        Gui_ThreadSetDisPlayArea(0, 0, 80, 160);
        
        if(p_data_manage->heart_measure_s.real_heart != 0)
        {
            heart = p_data_manage->heart_measure_s.real_heart;
        }
		Gui_PageHeart(p_task->gui_controller.animation_frame_index++, heart, display_language);
        //������������

    } 

    if(p_task->gui_controller.repaint_enable == 1)
    {
        if(p_device->key == BUTTON_EVENT_CLICK)
        {
            if(p_flash_param->bp_func_state == 0x01)
            {
                Cpu_ScheduleTask(p_task, p_device, CPU_TASK_BP);
            }
            else
            {
                if(p_data_manage->heart_measure_s.btn != 0)
                {
                    p_data_manage->bp_measure_s.bp_btn = 0;
                    p_data_manage->heart_measure_s.btn = 0;
                    p_data_manage->blood_oxygen_measure_s.btn = 0;
                    BleThread_AFE4404DevClose();
                }
                
                Cpu_ScheduleTask(p_task, p_device, CPU_TASK_MOVEMENT);  
            }
        }
        p_device->key = BUTTON_EVENT_NULL;
    }    
}

/****************************************************************
** Function name:            Cpu_SleepTask
** Descriptions:             ˯�߽���
** input parameters:         p_controller��       CPU���������ָ��
                             p_gui��              ��̬ҳ�������ָ��
                             p_oled��             oled����ָ��
** output parameters:        ��
** Returned value:           ��
** Created by:               GT
** Created Date:             2017-07-10
****************************************************************/
void Cpu_SleepTask(cpu_task_tcb_t* p_task, cpu_device_tcb_t* p_device)
{
    if(p_task->task_controller.init == 1)
    {
        Cpu_TaskParamInit(p_task,3,true,true);
        p_task->child_task_id = 1;
        //�������
        Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
                                p_device->cls_param.column_size, p_device->cls_param.page_size);
        
        // !ע��:����˯�߽�����Ҫ����˯���㷨,����(8�����û��˯��)
        if(clock->hour >= 8)//&&(Api_GetSleepTotalTime() == 0))
        {
            AlgThread_SleepTraversing();
        }
    }    

    // �ͻ�Ҫ��ˢ������Ϊ֡/0.3s
    if(((p_task->gui_controller.animation_tick_counter%3) == 0)&&(p_task->gui_controller.repaint_enable == 1))
    {
        if(p_task->child_task_id != 1)
		{
			//�������
			Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
									p_device->cls_param.column_size, p_device->cls_param.page_size);
			p_task->child_task_id = 1;
		}
		//������������
        Gui_ThreadSetDisPlayArea(0, 0, 80, 160);
		Gui_PageSleep(p_task->gui_controller.animation_frame_index++,Api_GetSleepTotalTime(),display_language);
    } 

    // �����̰�,�л�����һҳ
    if(p_task->gui_controller.repaint_enable == 1)
    {
        if(p_device->key == BUTTON_EVENT_CLICK)
        {
            // ��Ҫ�жϲ����ֻ������Ƿ��
            if(p_flash_param->dev_config_s.find_phone_enable == 1)
            {
                Cpu_ScheduleTask(p_task, p_device, CPU_TASK_FINDPHONE);
            }
            else if((p_flash_param->hourglass_s.ui_enable == 1)||(p_flash_param->hourglass_s.app_trigger == 1)
                    //YX.20180427.�޸��ֻ����������ʱ���棬APP�ص����ε���ʱ��������������ʱ������޷���ʾ������ʱҲ�޷������رյ��ε���ʱ������
                    ||(p_flash_param->hourglass_s.btn_trigger == 1))
            {
                Cpu_ScheduleTask(p_task, p_device, CPU_TASK_HOURGLASS);
            }
            else if(p_flash_param->dev_config_s.watchtimer_enable == 1)
            {
                Cpu_ScheduleTask(p_task, p_device, CPU_TASK_TIMER);
            }
            else
            {
                Cpu_ScheduleTask(p_task, p_device, CPU_TASK_POWEROFF);
            }
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
** Descriptions:             �����ֻ�����
** input parameters:         p_controller��       CPU���������ָ��
                             p_gui��              ��̬ҳ�������ָ��
                             p_oled��             oled����ָ��
** output parameters:        ��
** Returned value:           ��
** Created by:               GT
** Created Date:             2017-07-10
****************************************************************/
void Cpu_FindPhoneTask(cpu_task_tcb_t* p_task, cpu_device_tcb_t* p_device)
{
    if(p_task->task_controller.init == 1)
    {
        Cpu_TaskParamInit(p_task,3,true,false);
        p_task->child_task_id = 1;        
        //�������
        Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
                                p_device->cls_param.column_size, p_device->cls_param.page_size);
    }
        
    if(p_task->gui_controller.repaint_enable == 1)
    {
        // �ͻ�����: ֡/0.2Sˢ������ 
        if((p_task->gui_controller.animation_tick_counter%4) == 0)
        {
            if(p_task->child_task_id != 1)
			{
				//�������
				Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
										p_device->cls_param.column_size, p_device->cls_param.page_size);
				p_task->child_task_id = 1;
			}
			//������������
			Gui_ThreadSetDisPlayArea(0, 0, 80, 160);
            if(p_task->gui_controller.animation_enable == 0)
            {
                p_task->gui_controller.animation_frame_index = 0;
                p_data_manage->device_trace_param_s.during_findphone = 0;
                
                if(p_task->gui_controller.page_frame_counter == 0xff)
                {
                    p_task->gui_controller.page_frame_counter = clock->seconds;
                    Gui_PageFindPhone(p_task->gui_controller.animation_enable,p_task->gui_controller.animation_frame_index);
                }
            }
            else
            {
                p_task->gui_controller.animation_frame_index++;
                p_data_manage->device_trace_param_s.during_findphone = 1;
                
                Gui_PageFindPhone(p_task->gui_controller.animation_enable,p_task->gui_controller.animation_frame_index);
            }
        }
								
        if(((p_task->gui_controller.animation_tick_counter%98) == 0)&&(p_task->gui_controller.animation_enable == 1))
        {
            Ble_SendBatMulData(2,0xB5,0);
        }
    } 
    
    if((p_task->gui_controller.repaint_enable == 1)&&(p_device->key == BUTTON_EVENT_CLICK))
    {
        if(p_task->gui_controller.animation_enable != 1 /*&& p_device->key_times <= 1*8*/
           || p_task->gui_controller.animation_enable == 1)
        {
            if((p_flash_param->hourglass_s.ui_enable == 1)||(p_flash_param->hourglass_s.app_trigger == 1)
               //YX.20180427.�޸��ֻ����������ʱ���棬APP�ص����ε���ʱ��������������ʱ������޷���ʾ������ʱҲ�޷������رյ��ε���ʱ������
               ||(p_flash_param->hourglass_s.btn_trigger == 1))
            {
                Cpu_ScheduleTask(p_task, p_device, CPU_TASK_HOURGLASS);
            }
            else if(p_flash_param->dev_config_s.watchtimer_enable == 1)
            {
                Cpu_ScheduleTask(p_task, p_device, CPU_TASK_TIMER);
            }
            else
            {
                Cpu_ScheduleTask(p_task, p_device, CPU_TASK_POWEROFF);
            }
        }
        ///p_task->task_controller.run_time = 0;
    }
    
    if((p_task->gui_controller.repaint_enable == 1)&&(p_device->key == BUTTON_EVENT_LONGPRESS))
    {
        p_device->key = BUTTON_EVENT_NULL;
        p_task->task_controller.keep_time = 60;
        p_task->task_controller.run_time = 0;
        p_task->gui_controller.animation_enable = 1;
        p_task->gui_controller.animation_frame_index = 0xFFFF;
        p_task->gui_controller.animation_tick_counter = 0xFFFE;
    }
}

/****************************************************************
** Function name:            Cpu_PowerOffTask
** Descriptions:             �ػ�����
** input parameters:         p_controller��       CPU���������ָ��
                             p_gui��              ��̬ҳ�������ָ��
                             p_oled��             oled����ָ��
** output parameters:        ��
** Returned value:           ��
** Created by:               GT
** Created Date:             2017-07-10
****************************************************************/
void Cpu_PowerOffTask(cpu_task_tcb_t* p_task, cpu_event_tcb_t *p_event, cpu_device_tcb_t* p_device)
{
    if(p_task->task_controller.init == 1)
    {
        Cpu_TaskParamInit(p_task,3,true,true);//false);
        p_task->child_task_id = 1;
        //�������
        Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
                                p_device->cls_param.column_size, p_device->cls_param.page_size);
    }
        
    if((p_task->gui_controller.animation_tick_counter%6 == 0)&&(p_task->gui_controller.repaint_enable == 1))
    {
        if(p_task->child_task_id != 1)
		{
			//�������
			Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
									p_device->cls_param.column_size, p_device->cls_param.page_size);
			p_task->child_task_id = 1;
		}
		//������������
        Gui_ThreadSetDisPlayArea(7, 40, 66, 73);
		Gui_PagePower();//display_language);
    } 

    // ��������������
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
** Descriptions:             ������
** input parameters:         
** output parameters:        ��
** Returned value:           ��
** Created by:               GT
** Created Date:             2017-07-10
****************************************************************/
void Cpu_TimerTask(cpu_task_tcb_t* p_task, watch_timer_t *p_timer, cpu_device_tcb_t* p_device)
{
    static uint8_t last_ui_style = 0;
    uint8_t time_format = 0;
	uint8_t hour = 0;
    uint8_t phys_menstrual = 0;
    uint16_t step_prg = 0;
    uint32_t timer_msecond = 0;
    
    if(p_task->task_controller.init == 1)
    {
        // �����������г�ʼ��
        p_timer->entry = 1;
        Cpu_TaskParamInit(p_task,4,true,true);
        p_task->child_task_id = 1;
        //�������
        Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
                                p_device->cls_param.column_size, p_device->cls_param.page_size);	    
    }
      
    if(p_timer->entry == 1)
    {
        if((p_task->gui_controller.animation_tick_counter%2 == 0)&&(p_task->gui_controller.repaint_enable == 1))
        {
            if(p_task->child_task_id != 1)
			{
				//�������
				Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
										p_device->cls_param.column_size, p_device->cls_param.page_size);
				p_task->child_task_id = 1;
			}
			//������������
			Gui_ThreadSetDisPlayArea(0, 0, 80, 160);
			GUI_StopWatchPage(false,0,0,p_task->gui_controller.animation_frame_index++);
        }
    }
    else
    {
        // �������ʱ״̬
        if(p_timer->run == 1)
        {
            if((p_task->gui_controller.animation_tick_counter%2 == 0)&&(p_task->gui_controller.repaint_enable == 1))
            {
                //������������
                Gui_ThreadSetDisPlayArea(0, 0, 80, 160);
                
                // �������Ƴ�״̬
                if(((p_task->task_controller.keep_time == 4)&&(p_task->task_controller.run_time >= 3))
                   ||(p_task->task_controller.keep_time == 62)&&(p_task->task_controller.run_time >= 61)
                   ||(p_task->task_controller.keep_time == 5)&&(p_task->task_controller.run_time >= 4))
                {            
                    // �����ʱ�а�������,����Ҫ����������
                    if(p_device->key == BUTTON_EVENT_CLICK)
                    {
                        p_task->task_controller.keep_time = 5;
                        p_task->task_controller.run_time = 0;
                    }
                    p_device->key = BUTTON_EVENT_NULL;	
                    
                    if(p_task->child_task_id != 3)
					{
						//�������
						Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
												p_device->cls_param.column_size, p_device->cls_param.page_size);
						p_task->child_task_id = 3;
						p_task->gui_controller.animation_frame_index = 0;
                        last_ui_style = ui_style_type;
					}
                    if(last_ui_style != ui_style_type)
                    {
                        last_ui_style = ui_style_type;
                        //�������
						Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
												p_device->cls_param.column_size, p_device->cls_param.page_size);
                        p_task->gui_controller.animation_frame_index = 0;
                    }
                    hour = Hour_StandardTransform(p_flash_param->dev_config_s.am_pm_enable,clock->hour);
                    time_format = Get_AMPM_Format(p_flash_param->dev_config_s.am_pm_enable,clock->hour);
                    distance_value(&all_step,&distance_m,&distance_km,&distance_mile,&calories_buf);
                    //phys_menstrual = Phys_GetMenstrualPeriodType();
                    if(p_task->gui_controller.animation_frame_index == 0)
                    {
                        step_prg = Gui_DataGetSportsToMarkProgress();
                        Gui_PageMainDateHour(clock->month, clock->day, hour, clock->minutes, clock->week, display_language, 
                                             time_format, phys_menstrual, step_prg);
                        
                        Gui_PageMainAssisant(p_task->gui_controller.animation_frame_index++, p_data_manage->device_trace_param_s.ble_connect,
                                         time_format, Battery_Get_Status(), Battery_Get_Grade(), display_language, all_step, phys_menstrual);
                        
                        icon_counter = 0;
                    }
                    else
                    {
                        icon_counter++;
                        
                        if(icon_counter % 6 == 0)
                        {
                            Gui_PageMainAssisant(p_task->gui_controller.animation_frame_index++, p_data_manage->device_trace_param_s.ble_connect,
                                         time_format, Battery_Get_Status(), Battery_Get_Grade(), display_language, all_step, phys_menstrual);
                        }
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
                    
                    // ������Ҫ�ж����ʱ���Ƿ����99:59:99 Add by GT 2017-09-27
                    if(timer_msecond >= 599999)
                    {
                        timer_msecond = 599999;
                    }
                    
                    if(p_task->child_task_id != 2)
					{
						//�������
						Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
												p_device->cls_param.column_size, p_device->cls_param.page_size);
						p_task->child_task_id = 2;
						p_task->gui_controller.animation_frame_index = 0;
					}
					GUI_StopWatchPage(true,timer_msecond/100,timer_msecond%100,p_task->gui_controller.animation_frame_index++);

                    /////Gui_ButtonDynamic(p_task->gui_controller.animation_frame_index - 1);
                }
            }
        }
    }

    if((p_task->gui_controller.repaint_enable == 1)&&(p_device->key != BUTTON_EVENT_NULL))
    {
        //�����ǰ������������,����Ҫ���������߼�
        if((p_device->key == BUTTON_EVENT_LONGPRESS)&&(p_timer->entry == 1))
        {
            // ���������
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
            //��ת��������
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
                p_task->task_controller.keep_time = 5;
                p_task->task_controller.run_time = 0;
            }
        } 
        else if(p_task->child_task_id != 3 && p_device->key != BUTTON_EVENT_LONGPRESS_6S)
        {
            ///if(p_device->key_times <= 1*8)
            {
                Cpu_ScheduleTask(p_task, p_device, CPU_TASK_POWEROFF);
            }
            ///p_task->task_controller.run_time = 0;
        }
        
        if(p_task->child_task_id != 3)
        {
            p_device->key = BUTTON_EVENT_NULL;	 
        }
    }     
}

/****************************************************************
** Function name:            Cpu_TimerTask
** Descriptions:             ����ʱ����
** input parameters:         p_controller��       CPU���������ָ��
                             p_gui��              ��̬ҳ�������ָ��
                             p_oled��             oled����ָ��
** output parameters:        ��
** Returned value:           ��
** Created by:               GT
** Created Date:             2017-07-10
****************************************************************/
void Cpu_HourglassTask(cpu_task_tcb_t* p_task, cpu_device_tcb_t* p_device)
{
    if(p_task->task_controller.init == 1)
    {
        Cpu_TaskParamInit(p_task,3,true,true);
        p_task->child_task_id = 1;
        //�������
        Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
                                p_device->cls_param.column_size, p_device->cls_param.page_size);
    }
        
    if(p_task->gui_controller.repaint_enable == 1)
    {
        if(p_task->child_task_id != 1)
		{
			//�������
			Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
									p_device->cls_param.column_size, p_device->cls_param.page_size);
			p_task->child_task_id = 1;
		}
		//������������
        Gui_ThreadSetDisPlayArea(0, 40, 80, 81);
        
        // ���������������ʱ����,����Ҫ��ʾ����
        if((p_flash_param->hourglass_s.btn_trigger == 1)||(p_flash_param->hourglass_s.app_trigger == 1))
        {
            if(p_task->gui_controller.animation_tick_counter%4 == 0)
            {
				Gui_PageCountDown(true,p_task->gui_controller.animation_frame_index++);
            }
        }
        else if(p_task->gui_controller.page_frame_counter != clock->seconds)
        {
            p_task->gui_controller.page_frame_counter = clock->seconds;
			Gui_PageCountDown(false,p_task->gui_controller.animation_frame_index++);
        }
        ///Gui_ButtonDynamic(p_task->gui_controller.animation_frame_index - 1);
    }

    // ���°���(�̰�),�л�����һ������
    if((p_task->gui_controller.repaint_enable == 1)&&(p_device->key == BUTTON_EVENT_CLICK))
    {
        ///if(p_device->key_times <= 1*8)
        {
            // �������ش�
            if(p_flash_param->dev_config_s.watchtimer_enable == 1)
            {
                Cpu_ScheduleTask(p_task, p_device, CPU_TASK_TIMER);
            }
            else
            {
                Cpu_ScheduleTask(p_task, p_device, CPU_TASK_POWEROFF);
            }
        }
        ///p_task->task_controller.run_time = 0;
    }  

    // ����ʱ���ܰ�������
    if((p_task->gui_controller.repaint_enable == 1)&&(p_device->key == BUTTON_EVENT_LONGPRESS))
    {
        p_device->key = BUTTON_EVENT_NULL;
        
        // �������ʱ�Ѿ�������,����Ҫֹͣ
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
** Descriptions:            UI����ʹ�ã���ʾĳ�������UI
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              YX              
** Created Date:            2017-12-11
*****************************************************************/
void Cpu_TraverseInTask(cpu_task_tcb_t* p_task, CPU_TASK_TYPE task_type, uint8_t dis_time)
{
    p_task->task_id = task_type;
    Cpu_TaskParamInit(p_task, dis_time, false, true);
}

/****************************************************************
** Function name:            Cpu_BootUpTask
** Descriptions:             ������������
** input parameters:         p_controller��       CPU���������ָ��
                             p_gui��              ��̬ҳ�������ָ��
                             p_oled��             oled����ָ��
** output parameters:        ��
** Returned value:           ��
** Created by:               JSH
** Created Date:             2019-03-12
****************************************************************/
void Cpu_BootUpTask(cpu_task_tcb_t* p_task, cpu_event_tcb_t *p_event, cpu_device_tcb_t* p_device)
{
    if(p_task->task_controller.init == 1)
    {
        Cpu_TaskParamInit(p_task,3,false,true); 
        //�������
        Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
                                p_device->cls_param.column_size, p_device->cls_param.page_size);
    }
        
    if((p_task->gui_controller.animation_tick_counter%4 == 0)&&(p_task->gui_controller.repaint_enable == 1))
    {
		//������������
        Gui_ThreadSetDisPlayArea(0, 0, 80, 160);
		
		if(p_task->gui_controller.animation_frame_index >= 7)
		{
			Cpu_ScheduleTask(p_task, p_device, CPU_TASK_MAIN);
			
			return;
		}
        Gui_PageCartoon(0);
        p_task->gui_controller.animation_frame_index++;
    }     
}

/****************************************************************
** Function name:            Cpu_BpTask
** Descriptions:             ��������
** input parameters:         p_controller��       CPU���������ָ��
                             p_gui��              ��̬ҳ�������ָ��
                             p_oled��             oled����ָ��
** output parameters:        ��
** Returned value:           ��
** Created by:               JSH
** Created Date:             2019-03-12
****************************************************************/
void Cpu_BpTask(cpu_task_tcb_t* p_task, cpu_device_tcb_t* p_device)
{   
    static uint8_t bp_sp = 0;
    static uint8_t bp_dp = 0;
    static uint8_t test_mode = 0;
    static uint8_t test_state = 0;
    
    if(p_task->task_controller.init == 1)
    {								
		// ��ʼ����������
		p_data_manage->blood_oxygen_measure_s.btn = 0;
		p_data_manage->blood_oxygen_measure_s.time = 0;
		p_data_manage->hrv_measure_s.time = 0;
		p_data_manage->respiratory_rate_s.time = 0;
		p_data_manage->heart_measure_s.day_time = 0;
		p_data_manage->heart_measure_s.night_time = 0;
        p_data_manage->heart_measure_s.real_heart = 0;
        p_data_manage->heart_measure_s.btn = 0;
        
		p_data_manage->bp_measure_s.bp_day_time = 0;
		p_data_manage->bp_measure_s.bp_night_time = 0; 
        p_data_manage->bp_measure_s.bp_flag = 0;
        
        // Ѫѹ����ģʽ����
        Algorithm_BpSetModeParam();
        
		p_task->child_task_id = 1;
        
		if(Battery_Get_Grade() == 0)
		{
            p_data_manage->wear_measure_s.wear_status = 0;
			p_data_manage->bp_measure_s.bp_btn = 0;
			Cpu_TaskParamInit(p_task, 5, false, true);	      
		}
		else
		{
			p_data_manage->bp_measure_s.bp_btn = 1;

            p_data_manage->bp_measure_s.bp_init = 1;
            p_data_manage->wear_measure_s.wear_status = 0;
            p_data_manage->heart_measure_s.calib_well = 0;
            p_data_manage->heart_measure_s.calib_init = 1;
            heart_calib_init_cause = 4;
            
            BleThread_AFE4404BpOpen();			
			Cpu_TaskParamInit(p_task, 71, true, true);
        }
        
        bp_sp = 0;
        bp_dp = 0;
        test_state = 0;
        test_mode = p_data_manage->bp_measure_s.bp_test_mode;
        
        //�������
        Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
                                p_device->cls_param.column_size, p_device->cls_param.page_size);
    } 
    
    if((p_task->task_controller.run_time >= 70) || (p_data_manage->wear_measure_s.wear_status == 1))
    {
        if(p_data_manage->bp_measure_s.bp_btn == 1)
        {
            BleThread_AFE4404DevClose();
        }
        memset(p_task, 0, sizeof(cpu_task_tcb_t));
    }

    // �޸���ر��� 
    if(((p_task->gui_controller.animation_tick_counter % 6) == 0)&&(p_task->gui_controller.repaint_enable == 1))
    {
		//������������
        Gui_ThreadSetDisPlayArea(0, 0, 80, 160);
        
        if(p_task->child_task_id != 1)
        {
            p_task->child_task_id = 1;
            p_task->gui_controller.animation_frame_index = 0;
			Gui_PageClearBackground(p_device->cls_param.start_column, p_device->cls_param.start_page, 
									p_device->cls_param.column_size, p_device->cls_param.page_size);
        }
        
        if(p_data_manage->bp_measure_s.bp_init == 0)
        {
            if(p_data_manage->bp_measure_s.bp_flag == 1 && test_state == 0)
            {
                bp_sp = p_data_manage->bp_measure_s.bp_sp_value;
                bp_dp = p_data_manage->bp_measure_s.bp_dp_value;
                test_state = p_data_manage->bp_measure_s.bp_flag;
            }
            else if(p_data_manage->bp_measure_s.bp_flag > 1 && test_state == 0)
            {
                test_state = 2;
            }
        }
        
		//Gui_PageBP(uint8_t index, uint8_t bp_sp, uint8_t bp_dp, uint8_t test_mode, uint8_t test_state);
		Gui_PageBP(p_task->gui_controller.animation_frame_index++, bp_sp, bp_dp, test_mode, test_state);

    } 
	
	if(p_task->gui_controller.repaint_enable == 1)
    {
        if(p_device->key == BUTTON_EVENT_CLICK)
        {
            if(p_data_manage->bp_measure_s.bp_btn != 0)
            {
                p_data_manage->bp_measure_s.bp_btn = 0;
                p_data_manage->heart_measure_s.btn = 0;
                p_data_manage->blood_oxygen_measure_s.btn = 0;
                BleThread_AFE4404DevClose();
            } 
            Cpu_ScheduleTask(p_task, p_device, CPU_TASK_MOVEMENT); 
        }
        p_device->key = BUTTON_EVENT_NULL;
    }
}

