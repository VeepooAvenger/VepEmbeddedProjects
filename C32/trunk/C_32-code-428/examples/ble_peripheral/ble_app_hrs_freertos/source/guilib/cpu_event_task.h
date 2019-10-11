/***************************Copyright(c)***********************************
* *                      Veepoo Tech
* *                 COPYRIGHT (C) 2016-2019
**-----------File Info-----------------------------------------------------
* *File        name:     cpu_event_task.h
* *Last modified Date:   2017-08-01
* *Last SYS_VERSION:     0.0.0
* *Descriptions:            
**-------------------------------------------------------------------------
* *Created  by:          WJ
* *Created  date:        2017-08-01
* *SYS_VERSION:          0.0.0
* *Descriptions:         
**-------------------------------------------------------------------------
* *Modified by:				
* *Modified date:  			          
* *SYS_VERSION:                 
* *Descriptions:                        
***************************************************************************/
#ifndef _CPU_EVENT_TASK_H_
#define _CPU_EVENT_TASK_H_

#include "cpu_common.h"

//  �͵������¼�����
void Cpu_EventLowPowerHint(cpu_task_controller_t *p_controller, gui_controller_t *p_gui);

//  ��������¼�����
void Cpu_EventIdelCharge(cpu_task_controller_t *p_controller, gui_controller_t *p_gui);

//  ��������:����¼�����
void Cpu_EventAlarm(cpu_task_controller_t *p_controller, gui_controller_t *p_gui);

// ���������¼�����
void Cpu_EventSedentary(cpu_task_controller_t *p_controller, gui_controller_t *p_gui);

// ��Ϣ�����¼�����
void Cpu_EventCallRemind(cpu_task_controller_t *p_controller, gui_controller_t *p_gui);

// ��Ϣ�����¼�����
void Cpu_EventMessageRemind(cpu_task_controller_t *p_controller, gui_controller_t *p_gui);

// ���ʸ澯�¼�����
void Cpu_EventHeartAlert(cpu_task_controller_t *p_controller, gui_controller_t *p_gui);

// ���վ����¼�����
void Cpu_EventCameraReady(cpu_task_controller_t *p_controller, gui_controller_t *p_gui);

// ����OK�¼�����
void Cpu_EventCameraOK(cpu_task_controller_t *p_controller, gui_controller_t *p_gui);

// �ֿ�����¼�����
void Cpu_EventFontTest(cpu_task_controller_t *p_controller, gui_controller_t *p_gui);

// ����ʱʱ�䵽����������
void Cpu_EvenCountDownTimeOut(cpu_task_controller_t *p_controller, gui_controller_t *p_gui);

//����ʱ��ʼ�����¼�����
void Cpu_EvenCountDownTimeStart(cpu_task_controller_t *p_controller, gui_controller_t *p_gui);

//�˶��������¼�����
void Cpu_EvenStarget(cpu_task_controller_t *p_controller, gui_controller_t *p_gui);

//��ʾMAC��ַ�Ͱ汾��
void Cpu_EvenDisMacVer(cpu_task_controller_t *p_controller, gui_controller_t *p_gui);

//Ů�Թ�������
void Cpu_EvenPhysRemind(cpu_task_controller_t *p_controller, gui_controller_t *p_gui);


void Cpu_EvenCartoon(cpu_task_controller_t *p_controller, gui_controller_t *p_gui);

void Cpu_EvenCartoon1(cpu_task_controller_t *p_controller, gui_controller_t *p_gui);

#endif
