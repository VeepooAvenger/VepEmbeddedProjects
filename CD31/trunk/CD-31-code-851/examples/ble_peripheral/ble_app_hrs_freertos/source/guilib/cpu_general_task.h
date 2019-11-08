/***************************Copyright(c)***********************************
**                       Veepoo Tech
**                  COPYRIGHT (C) 2016-2019
**-----------File Info-----------------------------------------------------
** File        name:     cpu_general_task.h
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
#ifndef _CPU_GENERAL_TASK_H_
#define _CPU_GENERAL_TASK_H_

#include "cpu_common.h"

//ҳ����ת
void Cpu_TaskSchedule(cpu_task_controller_t *p_controller, gui_controller_t *p_gui, CPU_TASK_TYPE task_id);

//����ˢ�¼��������
void Cpu_TaskClearIntervalCnt(void);

//�����洦��
void Cpu_TaskMain(cpu_task_controller_t *p_controller, gui_controller_t *p_gui);

//�Ʋ����洦��
void Cpu_TaskStep(cpu_task_controller_t *p_controller, gui_controller_t *p_gui);

//������洦��
void Cpu_TaskDistance(cpu_task_controller_t *p_controller, gui_controller_t *p_gui);

//��·����洦��
void Cpu_TaskCalories(cpu_task_controller_t *p_controller, gui_controller_t *p_gui);

//�˶����洦��
void Cpu_TaskRun(cpu_task_controller_t *p_controller, gui_controller_t *p_gui);

//�˶�ģʽ��������
void Cpu_TaskRunButton(cpu_task_controller_t *p_controller, gui_controller_t *p_gui);

//�˶�ģʽ���ݽ���
void Cpu_TaskRunData(cpu_task_controller_t *p_controller, gui_controller_t *p_gui);

//Ѫѹ���洦��
void Cpu_TaskBP(cpu_task_controller_t *p_controller, gui_controller_t *p_gui);

//���ʽ��洦��
void Cpu_TaskHeart(cpu_task_controller_t *p_controller, gui_controller_t *p_gui);

//˯�߽��洦��
void Cpu_TaskSleep(cpu_task_controller_t *p_controller, gui_controller_t *p_gui);

//���ֻ����洦��
void Cpu_TaskFindPhone(cpu_task_controller_t *p_controller, gui_controller_t *p_gui);
// ��ͼƬȷ�ϵĹػ�����
void Cpu_TaskTurnOff(cpu_task_controller_t *p_controller, gui_controller_t *p_gui);
//�ػ����洦��
void Cpu_TaskPowerOff(cpu_task_controller_t *p_controller, gui_controller_t *p_gui);
//�������洦��
void Cpu_TaskPowerOn(cpu_task_controller_t *p_controller, gui_controller_t *p_gui);

//�����洦��
void Cpu_TaskStopWatch(cpu_task_controller_t *p_controller, gui_controller_t *p_gui);

//����ʱ���洦��
void Gui_TaskCountDown(cpu_task_controller_t *p_controller, gui_controller_t *p_gui);

// �˶�ģʽ��ͣ
void Cpu_TaskRunModePause(cpu_task_controller_t *p_controller, gui_controller_t *p_gui);

// �˶�ģʽ�˳�
void Cpu_TaskRunModeExit(cpu_task_controller_t *p_controller, gui_controller_t *p_gui);

//MAC and VERSION
void Cpu_TaskDisMacVer(cpu_task_controller_t *p_controller, gui_controller_t *p_gui);


#endif
