/***************************Copyright(c)***********************************
**                       Veepoo Tech
**                  COPYRIGHT (C) 2016-2019
**-----------File Info-----------------------------------------------------
** File        name:     phone_camera_event.h
** Last modified Date:   2017-08-21
** Last SYS_VERSION:     0.0.0
** Descriptions:            
**-------------------------------------------------------------------------
** Created  by:          WJ
** Created  date:        2017-08-21
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

#include "phone_camera_event.h"
#include "pedometer.h"
#include "bracelet_protocol.h"
#include "gui_thread.h"
#include "app_type.h"
#include "battery.h"

/* ������������ʼ�� */
static camera_control_t  camera_control_s = 
{
    .ready = 0x00,
    .photo_ok = 0x00,
    .photo_state = 0x00,
    .resv = 0x00,

    .current_time = 0x00,
    .internal_time = 0x00,
    .tp_keep_time = 0x00,
};

extern system_flag_t system_flag_s;

uint8_t camera_delay_cnt = 0;       //������ʱ����������1���𶯲Ż���Ӧ����

/****************************************************************
** Function name:           Camera_BraceletEntry
** Descriptions:            ��������ģʽ
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ             
** Created Date:            2017-08-21
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Camera_BraceletEntry(void)
{
    uint8_t camera_ble[20] = {0xB6, 0x01,0x01,};
    
    camera_control_s.photo_ok = 0x00;
    camera_control_s.ready = 0x01;
    camera_control_s.tp_keep_time = 0x00;
    camera_control_s.ready_keep_time = 60;
    camera_control_s.photo_state = 0x01;
    /* ��APP�����Ƴ���Ϣ */    
    Ble_SendDataViaBat(camera_ble, 20);

    //��������ģʽ
    system_flag_s.camera_mode = 1;
    //������ʱ����
    camera_delay_cnt = 0;
}

/****************************************************************
** Function name:           Camera_BtnClick
** Descriptions:            �����������
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ             
** Created Date:            2017-08-21
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Camera_BtnClick(void)
{
    uint8_t camera_ble[20] = {0xB6, 0x01,0x02,};
    
    camera_control_s.ready = 0x01;
    camera_control_s.photo_ok = 0x01;
    camera_control_s.tp_keep_time = 0x02;
    camera_control_s.ready_keep_time = 60;

    /* ��APP�����Ƴ���Ϣ */
    Ble_SendDataViaBat(camera_ble, 20);
}

/****************************************************************
** Function name:           Camera_BraceletExit
** Descriptions:            �˳����շ������ݹر�UI
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ             
** Created Date:            2017-08-21
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Camera_BraceletExit(void)
{
    uint8_t camera_ble[20] = {0xB6, 0x01, 0x00,};
    
    if(system_flag_s.camera_mode == 1)
    {
        /* ���б�����ʼ�� */
        memset(&camera_control_s, 0, sizeof(camera_control_s));

        /* ��APP�����Ƴ���Ϣ */
        Ble_SendDataViaBat(camera_ble, 20);
        camera_control_s.photo_state = 0x00;
    
    }
    //�˳�����ģʽ
    system_flag_s.camera_mode = 0;    
    //WJ.20170922.��ֹ�˳�����ʱ�������¼�����ִ��
    if(Gui_GetThreadJob() == CPU_EVENT_TASK_CAMERAREADY 
      || Gui_GetThreadJob() == CPU_EVENT_TASK_CAMERAOK )
    {
        Gui_ThreadEventTaskWakeUp(CPU_EVENT_TASK_ALL_EXIT); 
    }
}

/****************************************************************
** Function name:           Camera_BraceletExitSendData
** Descriptions:            �˳����շ�������
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ             
** Created Date:            2017-08-21
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Camera_BraceletExitSendData(void)
{
    uint8_t camera_ble[20] = {0xB6, 0x01, 0x00,};
    
    /* ���б�����ʼ�� */
    memset(&camera_control_s, 0, sizeof(camera_control_s));

    /* ��APP�����Ƴ���Ϣ */
    Ble_SendDataViaBat(camera_ble, 20);
    camera_control_s.photo_state = 0x00;
}

/****************************************************************
** Function name:           Camera_WaitTask
** Descriptions:            �����¼�����
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ             
** Created Date:            2017-08-21
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Camera_WaitTask(void)
{	
    uint8_t camera_ble[20] = {0xB6, 0x01, 0x02,};
    
    //WJ.20170511���������
    if((camera_delay_cnt++ >= 1) && (Vph_GetShakeFlag() >= 2) && (camera_control_s.internal_time == 0x00) && (!Motor_IsWork())
       &&(Gui_GetThreadJob() == CPU_EVENT_TASK_CAMERAREADY || Gui_GetThreadJob() == CPU_EVENT_TASK_CAMERAOK)) //���û�й���
    {       
        /* ������Ϣ */
        Ble_SendDataViaBat(camera_ble, 20);
        
        /* ��¼��ǰʱ�� */
        camera_control_s.current_time = 0xFF;
        camera_control_s.photo_ok = 0x01;
        camera_control_s.ready = 0x01;
        camera_control_s.tp_keep_time = 0x02;
        camera_control_s.ready_keep_time = 60;
        camera_control_s.internal_time = 0x02;
    }
            
    /* ����ʱ���߼� */
    if(camera_control_s.current_time != clock->seconds)
    {
        camera_control_s.current_time = clock->seconds;
    
        if(camera_control_s.tp_keep_time > 0)
        {
            camera_control_s.tp_keep_time--;
        }
        
        if(camera_control_s.ready_keep_time > 0)
        {
            camera_control_s.ready_keep_time--;
        }
        
        if(camera_control_s.internal_time > 0)
        {
            camera_control_s.internal_time--;
        }
        
        /* ���������Ϊ0 */
        if(camera_control_s.tp_keep_time == 0)
        {
            camera_control_s.photo_ok = 0;
            camera_control_s.ready = 0x01;
            //camera_control_s.ready_keep_time = 5;
        }
    }	
     
    if(camera_control_s.photo_ok == 0x01)
    { 
        /* ����OK GUI��ʾ */
        camera_control_s.photo_ok = 0x00;
        camera_control_s.ready = 0x01;
        Gui_ThreadEventTaskWakeUp(CPU_EVENT_TASK_CAMERAOK);
    }
    else
    {
        /* ����׼��GUI��ʾ */
        Gui_ThreadEventTaskWakeUp(CPU_EVENT_TASK_CAMERAREADY);
    }
    
    if(camera_control_s.ready_keep_time == 0 || system_flag_s.power_state != POWER_NORMAL)
    {
        //60s��ʱ�˳�����ģʽ
        system_flag_s.camera_mode = 0;
        if(Gui_GetThreadJob() == CPU_EVENT_TASK_CAMERAREADY 
          || Gui_GetThreadJob() == CPU_EVENT_TASK_CAMERAOK )
        {
            Gui_ThreadEventTaskWakeUp(CPU_EVENT_TASK_ALL_EXIT); 
	        //�˳�����ģʽ
	        Camera_BraceletExitSendData();
        }
    }
}

/****************************************************************
** Function name:           Camera_GetState
** Descriptions:            ��ȡ������Ʋ����ṹ��
** input parameters:        ��
** output parameters:       ��
** Returned value:          ���ؽṹ��ָ��
** Created by:              WJ             
** Created Date:            2017-08-21
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
camera_control_t *Camera_GetState(void)
{
    return &camera_control_s;
}
