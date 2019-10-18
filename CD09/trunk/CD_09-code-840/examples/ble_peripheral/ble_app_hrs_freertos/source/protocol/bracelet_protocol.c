/***************************Copyright(c)***********************************
**                       Veepoo Tech
**                  COPYRIGHT (C) 2016-2019
**-----------File Info-----------------------------------------------------
** File        name:     bracelet_protocol.c
** Last modified Date:   2017-08-24
** Last SYS_VERSION:     0.0.0
** Descriptions:            
**-------------------------------------------------------------------------
** Created  by:          WJ
** Created  date:        2017-08-24
** SYS_VERSION:          0.0.0
** Descriptions:         
**-------------------------------------------------------------------------
** Modified by:                
** Modified date:                        
** SYS_VERSION:                 
** Descriptions:                        
***************************************************************************/
#include "app_type.h"
#include "user_bootloader.h"
#include "blood_pressure.h"
#include "heart.h"
#include "outsit.h"
#include "phone_camera_event.h"
#include "battery.h"
#include "phone_notify_event.h"
#include "gui_page_display.h"
#include "vph_wear_dect.h"
#include "bsp_led.h"
#include "alarm_event.h"
#include "hal_acc_calculate.h"
#include "gui_update.h"
#include "gui_thread.h"
#include "movement_patterns.h"
#include "gui_traverse.h"
#include "physiological_period.h"
#include "gui_flash_addr.h"
/*********************************************************************
* Macro Definition
*/
#define UI_DEBUG        0

/*********************************************************************
* Global Variables
*/
extern system_flag_t system_flag_s;
extern system_switch_t system_switch_s;
extern system_info_t system_info_s;

//static uint8_t ble_ack[20] = {0x00, };       //WJ.2016.11.18
//static uint8_t ble_ack1[20] = {0x00, };       //WJ.2016.11.18
/*********************************************************************
* local functions
*/
static void Protocol_TimeChange(uint8_t *time_data);
static void Protocol_UploadAncsConfig(void);
static void Protocol_SetAncsConfig(uint8_t *p_data);
static void Protocol_UploadFunConfig(void);
static uint8_t Protocol_RestoreFactorySet(void);
/****************************************************************
** Function name:           Protocol_MeasuringStatus
** Descriptions:            ����״̬�ظ�
** input parameters:        measuring_status:��ǰ����״̬
** output parameters:       ��
** Returned value:          ��
** Returned value:
** Created by:              WJ             
** Created Date:            2017-03-17
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
static uint8_t Protocol_MeasuringStatus(uint8_t measuring_status)  
{
    uint8_t ret_state = 0;
    
    switch(measuring_status)
    {
        case BUTTON_BLOOD_PRESSURE_STATUS:
            ret_state = 0x01;
            break;
        case BUTTON_HEARTING_STATUS:
            ret_state = 0x02;
            break;
//        case BUTTON_BLOOD_OXYGEN_STATUS:
//            ret_state = 0x04;
//            break;
//        case BUTTON_FATIGUE_STATUS:
//            ret_state = 0x05;
//            break;
        default:
            ret_state = 0xFF;
            break;
    }   
    return ret_state;
}

/****************************************************************
** Function name:           Protocol_BloodOxygen
** Descriptions:            Ѫ���򿪹ر�����
** input parameters:        p_nus�����ݽ���ͨ����p_data:���յ�����
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ             
** Created Date:            2016-12-02
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
static void Protocol_BloodOxygen(ble_nus_t *p_nus, uint8_t *p_data)
{  
    uint8_t ble_ack[20] = {0x00, };       //WJ.2016.11.18
    
    ble_ack[0] = p_data[0];     //0x80

    ble_ack[1] = 0x00;//��֧��    
    
    Ble_SendDataViaBat(ble_ack, 20);      
}

/****************************************************************
** Function name:           Protocol_Fatigue
** Descriptions:            ƣ�Ͷȴ򿪹ر�����
** input parameters:        p_nus�����ݽ���ͨ����p_data:���յ�����
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ             
** Created Date:            2016-12-07
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
static void Protocol_Fatigue(ble_nus_t *p_nus, uint8_t *p_data)
{
    uint8_t ble_ack[20] = {0x00, };
    
    ble_ack[0] = p_data[0];     //0x81
    
    Ble_SendDataViaBat(ble_ack, 20);          
}

/****************************************************************
** Function name:           BloodPressOnOFF
** Descriptions:            Ѫѹ�򿪹ر�����
** input parameters:        p_nus�����ݽ���ͨ����p_data:���յ�����
** output parameters:       ��
** Returned value:          ��
** Created by:              SQH              
** Created Date:            2016-10-18
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
static void Protocol_BloodPressOnOFF(ble_nus_t *p_nus, uint8_t *p_data)
{
    uint8_t ble_ack[20] = {0x00, };
    
    ble_ack[0] = p_data[0];     //0x90
    if(system_flag_s.bp_flag != 0x01)
    {
        Ble_SendDataViaBat(ble_ack, 20);
        return;
    }
    
    if(p_data[1] == 0x01)
    {
        if(system_flag_s.measuring_status < TIME_MAX_MEASURING_STATUS)
        {
            //��Ѫѹ����
            if(Bp_SetBpMode(p_data[2]))
            {
                App_StopMeasuringHandler(TEST_CLOSE_ALL);           
                system_flag_s.measuring_status = APP_BLOOD_PRESSURE_STATUS; 
                system_flag_s.heart_led_status = true;
                system_flag_s.bp_calibration = 0;
                Bp_Open();           
                Bp_SetBpMode(p_data[2]);            
                ble_ack[4] = 0x00;    
                ble_ack[5] = 0x01;      //��ʾ�н���              
                ble_ack[6] = 0x01;      //У׼״̬�ɹ�������˽��ģʽ������Ч
            }
//            else   // У׼ģʽУ�鲻ͨ��
//            {              
//                ble_ack[6] = 0x02;      //У׼״̬ʧ��
//            }             
        }
        else
        {
            ble_ack[4] = Protocol_MeasuringStatus(system_flag_s.measuring_status);
        }
    }
    else if(p_data[1] == 0x00)
    {
        if(system_flag_s.measuring_status == APP_BLOOD_PRESSURE_STATUS)
        {
            //�ر�appѪѹ����
            Bp_Close();           
            system_flag_s.heart_led_status = false;
            system_flag_s.measuring_status = EVENT_NULL;
        }
        ble_ack[1] = 0x01;
    }
    
    Ble_SendDataViaBat(ble_ack, 20);
}

/****************************************************************
** Function name:           BloodPressureSet
** Descriptions:            Ѫѹģʽ����
** input parameters:        p_nus�����ݽ���ͨ����p_data:���յ�����
** output parameters:       ��
** Returned value:          ��
** Created by:              SQH              
** Created Date:            2016-10-18
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
static void Protocol_BloodPressureSet(ble_nus_t *p_nus, uint8_t *p_data)
{
    uint8_t ble_ack[20] = {0x00, };
    
    ble_ack[0] = p_data[0];  // 0x91
    ble_ack[1] = 0x01;      
    ble_ack[5] = p_data[1];//�ظ�ģʽ    
    ble_ack[7] = p_data[4];//
#if FUNCTION_SUPPORT_ACCURATE_BP
    if(p_data[1] == 0x00 && p_data[4] == 0x00)//�رո��Ի�����
    {
        system_info_s.bp_test_mode.test_mode = 0x00;
        App_SetSystemInfoSave(); 
    }
    else if(p_data[1] == 0x01 && p_data[4] == 0x00 )//�򿪸��Ի�����
    {
        system_info_s.bp_test_mode.test_mode = 0x01;      
        App_SetSystemInfoSave();       
    }
    else if(p_data[1] == 0x02) //��ȡѪѹģʽ
    {   
    }
    else if(p_data[1] == 0x03) //  �ر�ѪѹУ׼
    {
        if(system_flag_s.bp_calibration)
        {
            Blood_Pressure_Calibration_Close();
            system_flag_s.bp_calibration = 0;
            system_flag_s.measuring_status = EVENT_NULL; 
            system_flag_s.heart_led_status = false;   
        }
    }  
    
    ble_ack[2] = system_info_s.bp_test_mode.set_sp;
    ble_ack[3] = system_info_s.bp_test_mode.set_dp;
    ble_ack[4] = system_info_s.bp_test_mode.test_mode; 
    
    ble_ack[8] = system_info_s.bp_test_mode.calibration.calibration_sp;
    ble_ack[9] = system_info_s.bp_test_mode.calibration.calibration_dp;
    ble_ack[10] = system_info_s.bp_test_mode.calibration.calibration_heart;     
    //��ѪѹУ׼
    if(p_data[4] == 0x01 && (p_data[1] == 0x00 || p_data[1] == 0x01))
    {
        if(system_flag_s.measuring_status < TIME_MAX_MEASURING_STATUS)
        {
            //��Ѫѹ����
            App_StopMeasuringHandler(TEST_CLOSE_ALL);           
            system_flag_s.measuring_status = APP_BLOOD_PRESSURE_STATUS; 
            system_flag_s.heart_led_status = true;                     
            system_flag_s.bp_calibration = 1;
            Blood_Pressure_Calibration_Open(p_data[2], p_data[3], p_data[1]);  
            ble_ack[1] = 0x06;              
        }
        else
        {
             ble_ack[1] = 0x09;
        }        
        
        ble_ack[2] = p_data[2];
        ble_ack[3] = p_data[3];         
    }

#else    
    if(p_data[1] == 0x00)//�رո��Ի�����
    {
        system_info_s.bp_test_mode.test_mode = 0x00;
        system_info_s.bp_test_mode.set_sp =  p_data[2];
        system_info_s.bp_test_mode.set_dp =  p_data[3];
        App_SetSystemInfoSave();
        ble_ack[1] = 0x01;
    }
    else if(p_data[1] == 0x01)//�򿪸��Ի�����
    {
        system_info_s.bp_test_mode.test_mode = 0x01;
        system_info_s.bp_test_mode.set_sp =  p_data[2];
        system_info_s.bp_test_mode.set_dp =  p_data[3];       
        App_SetSystemInfoSave();
        ble_ack[1] = 0x01;
    }
    else if(p_data[1] == 0x02) //��ȡѪѹģʽ
    {
        ble_ack[1] = 0x01;
    }
    else
    {
        ble_ack[1] = 0x00;
    }
   
    ble_ack[2] = system_info_s.bp_test_mode.set_sp;
    ble_ack[3] = system_info_s.bp_test_mode.set_dp;
    ble_ack[4] = system_info_s.bp_test_mode.test_mode;
#endif     
    Ble_SendDataViaBat(ble_ack, 20);
}

/****************************************************************
** Function name:           Protocol_OledSet
** Descriptions:            ��Ļ��������
** input parameters:        p_nus�����ݽ���ͨ����p_data:���յ�����
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ             
** Created Date:            2017-02-16
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
static void Protocol_OledSet(ble_nus_t *p_nus, uint8_t *p_data)
{
    uint8_t ble_ack[20] = {0x00, };
    
    ble_ack[0] = p_data[0];  // 0xB1    
    ble_ack[1] = 1;     
    ble_ack[2] = p_data[1];  
    if(p_data[1] == 0x01)
    {
        if(!memcmp(&p_data[2], &p_data[4], 2)
           || p_data[4] > 23  || p_data[3] > 59 || p_data[5] > 59)
        {
            ble_ack[1] = 0;     //���ݲ�����
        }
        else
        { 
            system_info_s.brightness_leve.leve = p_data[7];
            system_info_s.brightness_leve.setleve = p_data[6];
            system_info_s.brightness_leve.s_hour = p_data[2];
            system_info_s.brightness_leve.s_minute = p_data[3];
            system_info_s.brightness_leve.e_hour = p_data[4];
            system_info_s.brightness_leve.e_minute = p_data[5];     
            App_SetSystemInfoSave();    
//            GUI_SetBrightness(1);
        }            
    }
    
    ble_ack[8] = system_info_s.brightness_leve.leve;
    ble_ack[7] = system_info_s.brightness_leve.setleve;
    ble_ack[3] = system_info_s.brightness_leve.s_hour;
    ble_ack[4] = system_info_s.brightness_leve.s_minute;
    ble_ack[5] = system_info_s.brightness_leve.e_hour;
    ble_ack[6] = system_info_s.brightness_leve.e_minute; 
    
    Ble_SendDataViaBat(ble_ack, 20);   
}

/****************************************************************
** Function name:           Protocol_CountDown
** Descriptions:            ����ʱ����
** input parameters:        p_nus�����ݽ���ͨ����p_data:���յ�����
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ             
** Created Date:            2017-02-16
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
static void Protocol_CountDown(ble_nus_t *p_nus, uint8_t *p_data)
{
    uint8_t ret = 0x00;
    uint32_t total_second = 0;
  
    if(p_data[1] == 0x00 || p_data[1] == 0x01) // 
    {
        total_second = p_data[3] + (p_data[4] << 8)+ (p_data[5] << 16);
        ret = Countdown_SetTime(total_second, p_data[1], p_data[2], p_data[6]);     
    }
	else if(p_data[1] == 0x02)
	{	
        ret = 1;
	}      
    
    Countdown_SendDataToApp(ret, p_data[1]);
}

/****************************************************************
** Function name:           Protocol_FindPhone
** Descriptions:            ���ֻ�
** input parameters:        p_nus�����ݽ���ͨ����p_data:���յ�����
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ             
** Created Date:            2017-02-16
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
static void Protocol_FindPhone(ble_nus_t *p_nus, uint8_t *p_data)
{
    //0xB5
//    CPU_EnhanceIdelMode();
}

/****************************************************************
** Function name:           Protocol_Camera
** Descriptions:            ����
** input parameters:        p_nus�����ݽ���ͨ����p_data:���յ�����
** output parameters:       ��
** Returned value:          ��
** Returned value:
** Created by:              WJ             
** Created Date:            2017-02-16
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
static void Protocol_Camera(ble_nus_t *p_nus, uint8_t *p_data)
{
    uint8_t ble_ack[20] = {0xB6, 0x00, };
   //0xB6
    /* ����Э��, ������ر��� */
    if(system_flag_s.measuring_status < BUTTON_BLOOD_PRESSURE_STATUS 
      && !Watch_IsBusy() && !Battery_IsChargingFull() && !Battery_IsCharging()) // �ֶ�����ʱ����Ӧ���չ��� sqh.20170310
    {
        if(p_data[1] == 0x00)
        {
            Camera_BraceletExit();    //WJ.20170714.��������¼�����ִ��ʱ�˳�����ֱ�ӹ���������
        }
        else if(p_data[1] == 0x01)
        {   
            if(Gui_ThreadCanAcceptEvent(CPU_EVENT_TASK_CAMERAREADY))
            {     
                Motor_DelEvent();                
                Camera_BraceletEntry();   
                Gui_ThreadEventTaskWakeUp(CPU_EVENT_TASK_CAMERAREADY);
            } 
            else if(system_flag_s.camera_mode == 0)
            {
                ble_ack[2] = p_data[1];
                Ble_SendDataViaBat(ble_ack, 20); 
            }
            else 
            {                      
                Camera_BraceletEntry();   
            }
        }
    }
	else
	{	
        if(p_data[1] == 0x00)   //�˳�
        {           
            if(system_flag_s.camera_mode == 1)
            {
                Camera_BraceletExit();
            }
        }
        else
        {
            ble_ack[2] = p_data[1]; 
        }
        
        Ble_SendDataViaBat(ble_ack, 20); 
	}
}

/****************************************************************
** Function name:           Protocol_Android_NotifySet
** Descriptions:            ��׿���紦��
** input parameters:        p_nus�����ݽ���ͨ����p_data:���յ�����
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ             
** Created Date:            2017-02-16
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
static void Protocol_Android_NotifySet(ble_nus_t *p_nus, uint8_t *p_data)
{
    uint8_t ble_ack[20] = {0x00, };
    
    ble_ack[0] = p_data[0];
    
    ble_ack[1] = p_data[1];
    ble_ack[2] = 0x01;

    if(p_data[1] == 0x01)       //�ֻ���0x01
    {
        if(p_data[3] == 0x00)    // Android �ֻ�������
        { 
//             Motor_AddEvent(1, 0, 1, 0);//����һ�� ÿ����1�� ���2��
        } 
    }
    else if(p_data[1] == 0x00 || p_data[1] == 0x02)//�ֻ��ر�0x00
    {
        Motor_DelEvent();//�ر����	
        if((Phone_NotifyGetCallingState() == PHONE_CALL_READYREJECT 
          || Phone_NotifyGetCallingState() == PHONE_CALL_REJECTSUC)
          && Gui_GetThreadJob() == CPU_EVENT_TASK_CALL_REMIND)//�豸�����Ҷ�
        {
            Phone_NotifySetCallingState(PHONE_CALL_REJECTSUC);//�Ҷ�               
            Gui_ThreadEventTaskWakeUp(CPU_EVENT_TASK_CALL_REMIND); 
        }
        else
        {
            Phone_NotifySetCallingState(PHONE_CALL_IDLE);
            if(Gui_GetThreadJob() == CPU_EVENT_TASK_CALL_REMIND)
            {
                Gui_ThreadEventTaskWakeUp(CPU_EVENT_TASK_ALL_EXIT); 
            }
        }
    }
    
    Ble_SendDataViaBat(ble_ack, 20);   
}

/****************************************************************
** Function name:           Protocol_Android_NotifyEvent
** Descriptions:            ��׿��Ϣ��������
** input parameters:        p_nus�����ݽ���ͨ����p_data:���յ�����
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ             
** Created Date:            2017-02-16
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
static void Protocol_Android_NotifyEvent(ble_nus_t *p_nus, uint8_t *p_data)
{
    uint8_t valid_len = 0;
    uint16_t i = 0;

    /* �����еĵ�һ������ʱ, ���ݱ����ʼ�� */
    if(p_data[4] == 0x01)
    {
        Phone_NotifyInit();
        phone_notify_s.msg.package_index = 1;

        /* ��������Ͷ������� */
        if((p_data[1] == 0x00) || (p_data[1] == 0x01))
        {
            if(0x00 == p_data[1])
            {
                phone_notify_s.hint.call = 0x01;
            }
            else
            {
                if(Gui_GetThreadJob() == CPU_EVENT_TASK_CALL_REMIND)
                {
                    return;     //����״̬��������            
                }
                phone_notify_s.hint.message = 0x01;
            }
            
            phone_notify_s.msg.title_len = p_data[2];
            memcpy(phone_notify_s.msg.title, &p_data[6], phone_notify_s.msg.title_len);
            //��׿���˶������ݹ���
        }
        else
        {
            if(Gui_GetThreadJob() == CPU_EVENT_TASK_CALL_REMIND)
            {
                return;     //����״̬��������            
            }
            /* �����Ϣ���� */
            if(p_data[1] == 0x02)
            {
                phone_notify_s.hint.wechat = 0x01;
            }
            else if(p_data[1] == 0x03)
            {
                phone_notify_s.hint.qq = 0x01;
            }
            else if(p_data[1] == 0x05)
            {
                phone_notify_s.hint.facebook = 0x01;
            }
            else if(p_data[1] == 0x06)
            {
                phone_notify_s.hint.twitter = 0x01;
            }
            else if(p_data[1] == 0x08)
            {
                phone_notify_s.hint.linkedin = 0x01;
            }
            else if(p_data[1] == 0x09)
            {
                phone_notify_s.hint.whatsapp= 0x01;
            }
            else if(p_data[1] == 0x0A)
            {
                phone_notify_s.hint.line = 0x01;
            }
            else if(p_data[1] == 0x0B)
            {
                phone_notify_s.hint.instagram = 0x01;
            }
            else if(p_data[1] == 0x0C)
            {
                phone_notify_s.hint.snapchat = 0x01;
            }
            else if(p_data[1] == 0x0D)
            {
                phone_notify_s.hint.skype = 0x01;
            }
            else if(p_data[1] == 0x0E)
            {
                phone_notify_s.hint.gmail = 0x01;
            }
            else //if(p_data[1] == 0x11)
            {
                phone_notify_s.hint.other = 0x01;
            }
//            else
//            {
//                return;
//            }
                        
            if(p_data[5] == 0x00 || p_data[5] == 0x01)
            {
                memcpy(phone_notify_s.msg.title, &p_data[6], p_data[2]);            
                phone_notify_s.msg.title_len = p_data[2];               
            }
            else
            {
                memcpy(phone_notify_s.msg.content, &p_data[6], p_data[2]);            
                phone_notify_s.msg.content_len = p_data[2];    
            }            
        }
    }
    if(p_data[4] != 0x01)
    { 
        if(phone_notify_s.msg.package_index != p_data[4])
        {/* ����APP�ظ����Ͱ����� Add:2016-10-07 by ���� */
            /* ���µ�ǰ���ı�� */
            phone_notify_s.msg.package_index = p_data[4];
            if(p_data[5] == 0x00 || p_data[5] == 0x01)
            {//��ȡ����               
                if(phone_notify_s.msg.title_len+p_data[2] <= TLTLE_DATA_MAX_LEN)
                {
                    memcpy(&phone_notify_s.msg.title[phone_notify_s.msg.title_len], &p_data[6], p_data[2]);            
                    phone_notify_s.msg.title_len += p_data[2];
                }
                else
                {
                    memcpy(&phone_notify_s.msg.title[phone_notify_s.msg.title_len], &p_data[6], 
                           TLTLE_DATA_MAX_LEN - phone_notify_s.msg.title_len);            
                    phone_notify_s.msg.title_len = TLTLE_DATA_MAX_LEN;
                }
            }
            else //if(0x02 == p_data[5])
            {/* ��ȡ���� */   
               /* �������� */
                if((10 == phone_notify_s.msg.title_len) && (0xBF == p_data[6]) && (0xBD == p_data[7]))
                {
                    memcpy(&phone_notify_s.msg.content[phone_notify_s.msg.content_len], &p_data[9], p_data[2]);
                    phone_notify_s.msg.content_len += p_data[2]-3;
                }
                else
                {
                    if(phone_notify_s.msg.content_len+p_data[2] <= CONTENT_DATA_MAX_LEN)
                    {
                        memcpy(&phone_notify_s.msg.content[phone_notify_s.msg.content_len], &p_data[6], p_data[2]);
                        phone_notify_s.msg.content_len += p_data[2];
                    } 
                    else
                    {
                        memcpy(&phone_notify_s.msg.content[phone_notify_s.msg.content_len], &p_data[6], 
                               CONTENT_DATA_MAX_LEN - phone_notify_s.msg.content_len);
                        phone_notify_s.msg.content_len = CONTENT_DATA_MAX_LEN;
                    }
                }
            }
        }
    }
		
    /* ��Ϣ���ͽ��� */
    if(p_data[3] == p_data[4])
    {            
        valid_len = phone_notify_s.msg.title_len;
        for(i = 0; i < valid_len; i++)
        {
            if(phone_notify_s.msg.title[i] == 0x3A)
            {
                phone_notify_s.msg.title_len = i;
//                memcpy(phone_notify_s.msg.title, &p_data[6], phone_notify_s.msg.title_len);
                break;
            }
        }
        if(valid_len > phone_notify_s.msg.title_len + 1)
        {
            if((phone_notify_s.msg.content_len + valid_len-phone_notify_s.msg.title_len - 1) < CONTENT_DATA_MAX_LEN)
            {
                for(i = phone_notify_s.msg.content_len; i > 0; i--)
                {
                    phone_notify_s.msg.content[valid_len - phone_notify_s.msg.title_len + i - 2] = phone_notify_s.msg.content[i - 1];                                       
                }                
            }
            else
            {
                for(i = CONTENT_DATA_MAX_LEN-(valid_len-phone_notify_s.msg.title_len - 1); i >0 ; i--)
                {
                    phone_notify_s.msg.content[valid_len-phone_notify_s.msg.title_len + i - 2] = phone_notify_s.msg.content[i - 1];
                }               
            }
            memcpy(phone_notify_s.msg.content, &phone_notify_s.msg.title[phone_notify_s.msg.title_len + 1], 
                   valid_len-phone_notify_s.msg.title_len - 1);
            phone_notify_s.msg.content_len += valid_len-phone_notify_s.msg.title_len - 1;
        }
        valid_len = phone_notify_s.msg.title_len; 
        /* ��Ч�������� */
        if((phone_notify_s.msg.title[valid_len - 4] == 0xEF) && (phone_notify_s.msg.title[valid_len-3] == 0xBF) 
          && (phone_notify_s.msg.title[valid_len - 2] == 0xBD) && (phone_notify_s.msg.title[valid_len-1] == 0xEF))
        {
            phone_notify_s.msg.title_len = valid_len - 4;
//            memcpy(phone_notify_s.msg.title, &p_data[6], phone_notify_s.msg.title_len);
        }        
        phone_notify_s.control.recv_done = 0x01; 
        //�޸���ʾ��ȫ������
//        if(phone_notify_s.msg.content_len > 0 && phone_notify_s.msg.content_len < CONTENT_DATA_MAX_LEN)
//        {
//            phone_notify_s.msg.content_len++;
//        }
        
        if(phone_notify_s.hint.call == 0x01)
        {
            //����UI�¼�����UI������ʾ
            if(Gui_ThreadCanAcceptEvent(CPU_EVENT_TASK_CALL_REMIND) == 0
               || Phone_NotifyIsCanRemind() == 0)  //�����жϣ���ֹ���� 
            {
                Phone_NotifyInit();
            }
            else
            {
                //WJ.20180129.���Ӳ��ܼ�����Ϣ����ʱ�������
                if(Gui_ThreadCanAcceptEvent(CPU_EVENT_TASK_CALL_REMIND) == 0)
                {
                    Phone_NotifyInit();
                    return;
                }
                Gui_ThreadEventTaskWakeUp(CPU_EVENT_TASK_CALL_REMIND);
            }
        }
        else
        {         
            //����UI�¼�����UI������ʾ
            if(Gui_ThreadCanAcceptEvent(CPU_EVENT_TASK_MESSAGE_REMIND) == 0 
               || Phone_NotifyIsCanRemind() == 0)  //�����жϣ���ֹ���� 
            {
                Phone_NotifyInit();
            }
            else
            {
                //WJ.20180129.���Ӳ��ܼ�����Ϣ����ʱ�������
                if(Gui_ThreadCanAcceptEvent(CPU_EVENT_TASK_MESSAGE_REMIND) == 0)
                {
                    Phone_NotifyInit();
                    return;
                }
                Gui_ThreadEventTaskWakeUp(CPU_EVENT_TASK_MESSAGE_REMIND);
            }
        }
    }
}

#if UI_DEBUG 

/****************************************************************
** Function name:           Protocol_UpdateUiMode
** Descriptions:            ����UI����ģʽ
** input parameters:        p_nus�����ݽ���ͨ����p_data:���յ�����
** output parameters:       ��
** Returned value:          ��
** Created by:              YX             
** Created Date:            2017-08-08
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
static void Protocol_UpdateUiMode(ble_nus_t *p_nus, uint8_t *p_data)
{
    uint8_t ble_ack[20] = {0xC4,};
    
    ble_ack[1] = Gui_UpdateCheck(p_data);
    
    Ble_SendDataViaBat(ble_ack, 20);
}

/****************************************************************
** Function name:           Protocol_ReceiveUpdateUiData
** Descriptions:            ���ܸ��µ�UI����
** input parameters:        p_nus�����ݽ���ͨ����p_data:���յ�����
** output parameters:       ��
** Returned value:          ��
** Created by:              YX             
** Created Date:            2017-08-08
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
static void Protocol_ReceiveUpdateUiData(ble_nus_t *p_nus, uint8_t *p_data)
{
    uint8_t ble_ack[20] = {0xC4,0x02};
    
    if(Gui_UpdateFlash(p_data))
    {
        Ble_SendDataViaBat(ble_ack, 20);
    }
    
}

/****************************************************************
** Function name:           Protocol_EraseUiData
** Descriptions:            ����UI����ģʽ
** input parameters:        p_nus�����ݽ���ͨ����p_data:���յ�����
** output parameters:       ��
** Returned value:          ��
** Returned value:
** Created by:              YX             
** Created Date:            2017-08-08
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
static void Protocol_EraseUiData(ble_nus_t *p_nus, uint8_t *p_data)
{
    uint8_t ble_ack[20] = {0xC5,};
    
    ble_ack[1] = p_data[1];
    ble_ack[2] = Gui_EraseUiData(p_data);
    
    Ble_SendDataViaBat(ble_ack, 20);
}

/****************************************************************
** Function name:           Protocol_ReadUiData
** Descriptions:            �����ȡUI����ģʽ
** input parameters:        p_nus�����ݽ���ͨ����p_data:���յ�����
** output parameters:       ��
** Returned value:          ��
** Created by:              YX             
** Created Date:            2017-08-08
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
static void Protocol_ReadUiData(ble_nus_t *p_nus, uint8_t *p_data)
{
    uint8_t ble_ack[20] = {0xC6,};
    
    ble_ack[1] = Gui_ReadUiData(p_data);
    Ble_SendDataViaBat(ble_ack, 20);
}

#endif

/****************************************************************
** Function name:           Protocol_SetUiStyle
** Descriptions:            ����UI���
** input parameters:        p_nus�����ݽ���ͨ����p_data:���յ�����
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ             
** Created Date:            2017-08-15
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
static void Protocol_SetUiStyle(ble_nus_t *p_nus, uint8_t *p_data)
{
    uint8_t ble_ack[20] = {0xC7, 0x00, };
    
    ble_ack[1] = p_data[1];
    ble_ack[2] = 0x00;
    if(p_data[1] == 0x01)
    {
        if(p_data[2] < FUNCTION_SUPPORT_UI_STYLE)
        {
            if(system_info_s.ui_style != p_data[2])
            {
                system_info_s.ui_style = p_data[2];            
                App_SetSystemInfoSave();
            }
            ble_ack[2] = 0x01;
            Gui_ThreadUpdateMian();
        }        
    }
    else if(p_data[1] == 0x02)
    {
        ble_ack[2] = 0x01;
    }
    
    ble_ack[3] = system_info_s.ui_style;
    
    Ble_SendDataViaBat(ble_ack, 20);
}

/****************************************************************
** Function name:           Protocol_UploadSwitchState
** Descriptions:            �ϴο���������Ϣ
** input parameters:        p_nus�����ݽ���ͨ����p_data:���յ�����
** output parameters:       ��
** Returned value:          ��
** Returned value:
** Created by:              WJ             
** Created Date:            2016-11-19
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
static void Protocol_UploadSwitchState(void)
{
    uint8_t ble_ack[20] = {0x00, };
    
    ble_ack[0] = 0xB8;
    ble_ack[1] = 0x02; 
    memcpy(&ble_ack[2], &system_switch_s, sizeof(system_switch_t));
    
    if(system_flag_s.bp_flag != 0x01)
    {
       ble_ack[5] = 0; 
    }
    
    Ble_SendDataViaBat(ble_ack, 20);     
}

/****************************************************************
** Function name:           Protocol_SwitchState
** Descriptions:            ��������
** input parameters:        p_nus�����ݽ���ͨ����p_data:���յ�����
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ             
** Created Date:            2016-11-19
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/  
static void Protocol_SwitchState(ble_nus_t *p_nus, uint8_t *p_data)
{
    uint8_t ble_ack[20] = {0x00, };
    
    ble_ack[0] = p_data[0];     //0xB8
    ble_ack[1] = p_data[1];
    if(p_data[1] == 0x01)   //set
    {
        memcpy(&system_switch_s, &p_data[2], sizeof(system_switch_t));
        Config_SystemSwitchCheck();
        //�������ݵ�flash
        Storage_SaveSwitchInfo();
        if(system_switch_s.find_phone == 2)
        {
            if(Gui_ThreadIsFindPhone())
            {
                Gui_ThreadTaskToIdle();
            }
        }
    }
    else if(p_data[1] == 0x02)  //get
    {
//        memcpy(&ble_ack[2], &system_switch_s, sizeof(system_switch_t));
    }    
    memcpy(&ble_ack[2], &system_switch_s, sizeof(system_switch_t));
    
    if(system_flag_s.bp_flag != 0x01)
    {
       ble_ack[5] = 0; 
    }
    
    Ble_SendDataViaBat(ble_ack, 20); 
}

/****************************************************************
** Function name:           Protocol_AlarmConfig
** Descriptions:            ����������
** input parameters:        p_nus�����ݽ���ͨ����p_data:���յ�����
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ             
** Created Date:            2016-11-19
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/  
static void Protocol_AlarmConfig(ble_nus_t *p_nus, uint8_t *p_data)
{
    uint8_t ble_ack[20] = {0x00, };
    uint16_t crc16 = 0;
    uint16_t p_crc16 = 0;
    uint8_t total_num = 0;
 
    ble_ack[0] = p_data[0];
    ble_ack[4] = p_data[1];
    ble_ack[1] = 1;    

    if(p_data[1] == 0)  //ɾ��
    {
        Alarm_ClearByID(p_data[2]);
        memcpy(&ble_ack[5], &p_data[2], 10);
    }
    else if(p_data[1] == 1) //����
    {
        ble_ack[1] = Alarm_Set(&p_data[2], 10);
        memcpy(&ble_ack[5], &p_data[2], 10);
    }
    else if(p_data[1] == 2) //��ȡ
    {
        p_crc16 = p_data[18] + (p_data[19] << 8);
        crc16 = Alarm_GetCRC();
        total_num = Alarm_GetValidConfigNum(); 
        if(p_crc16 != crc16 && total_num != 0)
        {
            //���뷢���¼�    
            Alarm_SetSendData(0, total_num, crc16);
            App_SetSendFlashDataType(FLASH_DATA_TYPE_ALARM);
            App_AddEvent(APP_SEND_FLASH_DATA_EVENT);
            return;
        }        
    }    
    ble_ack[2] = 0;
    ble_ack[3] = Alarm_GetValidConfigNum();
    
    crc16 = Alarm_GetCRC();  
    ble_ack[18] = crc16 & 0xFF;
    ble_ack[19] = (crc16>>8) & 0xFF;  
    
    Ble_SendDataViaBat(ble_ack, 20);  
}

/****************************************************************
** Function name:           Protocol_MatchingAndBinding
** Descriptions:            IOS Matching the binding
** input parameters:        p_nus�����ݽ���ͨ����p_data:���յ�����
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ             
** Created Date:            2016-11-19
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/  
static void Protocol_MatchingAndBinding(ble_nus_t *p_nus, uint8_t *p_data)
{
    uint8_t ble_ack[20] = {0x00, };
    
    ble_ack[0] = p_data[0];
    if(p_data[1] == 0x01)   //IOS Matching the binding
    {
#if APP_ANCS_ENABLED         
        Ancs_Start();
#endif        
        ble_ack[1] = 0x01;       
    }
    else if(p_data[1] == 0x00)  //Cancel the matching binding
    {
        ble_ack[1] = 0x01; 
    }
    ble_ack[2] = 0x01;
    Ble_SendDataViaBat(ble_ack, 20);    
}

/****************************************************************
** Function name:           Protocol_AndroidNotify
** Descriptions:            ��
** input parameters:        p_nus�����ݽ���ͨ����p_data:���յ�����
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ             
** Created Date:            2016-11-19
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
static void Protocol_AndroidNotify(ble_nus_t *p_nus, uint8_t *p_data)
{
    //android info deal
}

/****************************************************************
** Function name:           Protocol_UploadFunConfig
** Descriptions:            �ϴ��豸����
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              SQH              
** Created Date:            2016-10-18
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
static void Protocol_UploadFunConfig(void)
{
    uint8_t ble_ack[20] = {0x00, };
    uint8_t ble_ack1[20] = {0x00, };
    
    ble_ack[0] = 0xA7;
    
    if(system_flag_s.bp_flag == 0x01)
    {
        ble_ack[1] = 0x01; //Ѫѹ
    }
    else
    {
        ble_ack[1] = 0x00; //Ѫѹ
    }
    
    if(system_flag_s.sit_flag == 0x01)
    {
        ble_ack[3] = 0x01; //����
    }
    else
    {
        ble_ack[3] = 0x00; //����
    }
    
    ble_ack[2] = 0x00; //����
//    ble_ack[3] = 0x01; //����
    ble_ack[4] = 0x01; //���ʱ���
    ble_ack[5] = 0x01; //΢���˶�
    ble_ack[6] = 0x01; //����
    ble_ack[7] = 0x00; //ƣ�Ͷ�
    ble_ack[8] = 0x00; //Ѫ��
    ble_ack[9] = 0x14; //��Ϣ���Ѱ���20
    ble_ack[10] = 0x01; //���ʱ���
    ble_ack[11] = 0x01; //������������
    ble_ack[12] = 0x03; //Ů��
    ble_ack[13] = 0x00; //��Ļ��������
    ble_ack[14] = 0x00; //0xC3ָ�����
    ble_ack[15] = 0x01; //�ֻ����ȱ�־λ
    ble_ack[16] = 0x00; //�߶�ѪѹУ׼֧��
    ble_ack[17] = 0x03; //������
    ble_ack[18] = 0x00; //���ʹ���  
    ble_ack[19] = 0x01; //����  
    Ble_SendDataViaBat(ble_ack, 20);      
    
    ble_ack1[0] = 0xA7;
    ble_ack1[1] = 0x01; //����ʱ
    ble_ack1[2] = 0x05; //�����ճ�����
    ble_ack1[3] = 0x05; //��Ϣ���������ע
    ble_ack1[4] = 0x00; //֧�ְ�׿HID
    ble_ack1[5] = 0x03; //�˶�ģʽ
    ble_ack1[6] = FUNCTION_SUPPORT_UI_STYLE; //֧��8�ַ��
    
    ble_ack1[11] = 1; //A5ָ������
	
	ble_ack1[14] = 0x01; //������ݱ�־λ
    
    ble_ack1[19] = 0x02; //���� 
    Ble_SendDataViaBat(ble_ack1, 20);     
}

/****************************************************************
** Function name:           PasswordPair
** Descriptions:            ������Լ��ֻ�֧�ֹ��ܻظ�
** input parameters:        p_nus�����ݽ���ͨ����p_data:���յ�����
** output parameters:       ��
** Returned value:          ��
** Created by:              SQH              
** Created Date:            2016-10-18
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
static void Protocol_PasswordPair(ble_nus_t *p_nus, uint8_t *p_data)
{   
    uint8_t pass_flag = 0;
    uint8_t ble_ack[20] = {0x00, };
    
    ble_ack[0] = 0xA1;    
    if(p_data[3] ==0x00)
    {
        /*���������׼���*/
        if(system_info_s.password_num != 256 *p_data[2] + p_data[1])//���벻��ȷ
        {
            system_flag_s.code_passed = false;
        }
        else
        {
            system_flag_s.code_passed = true;           
            pass_flag = 1;
            if(0x00 == p_data[12])  // ANCS 
            {
#if APP_ANCS_ENABLED
                Ancs_Start();
#endif                
            }
            //��׼ʱ��

            if(Sclock_TimeCheck(&p_data[4], sizeof(system_clock_time_t) - 1 ))
            {
                Protocol_TimeChange(&p_data[4]);
                Sclock_SynchronizationTime(&p_data[4], sizeof(system_clock_time_t) - 1);
            }                  
        }
        ble_ack[1] = p_data[1];
        ble_ack[2] = p_data[2];
        ble_ack[3] = (system_flag_s.code_passed == true)?0x06:0x00;
    }
    else if(p_data[3] == 0x01)  //set code
    {
        if(system_flag_s.code_passed == true)
        {
            system_info_s.password_num = p_data[1] + (p_data[2] << 8);
            //save code to flash
            App_SetSystemInfoSave();
            ble_ack[3] = 0x03; //set code ok
        }
        else
        {
            ble_ack[3] = 0x02; //set code failed
        }
    }
    else if(p_data[3] == 0x02)  //read code
    {
        if(system_flag_s.code_passed == true)
        {
            ble_ack[1] = system_info_s.password_num & 0xFF; 
            ble_ack[2] = (system_info_s.password_num >> 8) & 0xFF; 
            ble_ack[3] = 0x05; //read code ok
        }
        else
        {
            ble_ack[3] = 0x04; //read code failed
        }        
    }
    
    ble_ack[4] = (VEEPOO_DEVICE_ID>>8) & 0xFF;
    ble_ack[5] = VEEPOO_DEVICE_ID & 0xFF;
    ble_ack[6] = VERSION_M & 0xFF;
    ble_ack[7] = VERSION_H & 0xFF;
    ble_ack[8] = VERSION_L & 0xFF;   
    ble_ack[9] = VERSION_T & 0xFF;   
    ble_ack[10] = 0;
    ble_ack[11] = 0x01;   
    Ble_GetBleAddr(&ble_ack[12]);                        
    if(system_info_s.wear_detect == 1 || system_info_s.wear_detect == 2)
    {
        ble_ack[19] = system_info_s.wear_detect;
    }
    else
    {
        ble_ack[19] = 0;
    }
    
    if(pass_flag)
    {
        //����
        Protocol_UploadFunConfig();
        /* ��������IOS���ƿ���ָ�� */
        Protocol_UploadAncsConfig();            
        //����״̬
        Protocol_UploadSwitchState();
    }    
    Ble_SendDataViaBat( ble_ack, sizeof(ble_ack));	//WJ.2016.11.18  
}

/****************************************************************
** Function name:           Protocol_ReadInfoByOAD
** input parameters:        p_nus�����ݽ���ͨ����p_data:���յ�����
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ             
** Created Date:            2016-11-19
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
static void Protocol_ReadInfoByOAD(ble_nus_t *p_nus, uint8_t *p_data)
{
    //�˶�ģʽ�²�������
    if(!Movement_IsBusy())
    {
        Dfu_JumpToBootloader();  
        nrf_delay_ms(1000);
    }
}

/****************************************************************
** Function name:           SynchronizationUserInfo
** Descriptions:            ͬ���û���Ϣ
** input parameters:        p_nus�����ݽ���ͨ����p_data:���յ�����
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ             
** Created Date:            2016-11-19
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
static void Protocol_SynchronizationUserInfo(ble_nus_t *p_nus, uint8_t *p_data)
{
    uint8_t ble_ack[20] = {0x00, };
    user_info_t user_info;

    ble_ack[0] = p_data[0]; //0xA3
    
    memcpy(&user_info, &p_data[1], sizeof(user_info_t)); 
    user_info.user_moving_target = p_data[6] + (p_data[5] << 8); 

    //ͬ���û���Ϣ��������أ��Ա����䣬�˶�Ŀ��
    if(memcmp(&user_info, &(system_info_s.user_info), sizeof(user_info_t)))
    {
        memcpy(&(system_info_s.user_info), &user_info, sizeof(user_info_t));  
        App_SetSystemInfoSave();
        Ped_SetInputPersonalInformation(system_info_s.user_info.user_height, system_info_s.user_info.user_weight, 
                                       system_info_s.user_info.user_gender, system_info_s.user_info.user_age);
        
        
        Phys_UpdataInfo();
    }
     
    ble_ack[1] = 0x01;
    Ble_SendDataViaBat(ble_ack, sizeof(ble_ack));     
}

/****************************************************************
** Function name:           Protocol_TimeChange
** Descriptions:            ͬ��ʱ��
** input parameters:        time_data��ʱ������
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ             
** Created Date:            2016-12-12
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
static void Protocol_TimeChange(uint8_t *time_data)
{
    uint8_t month = 0;
    uint8_t day = 0;
    uint32_t c_time = 0;
    uint32_t t_time = 0;
    
    if(!time_data)
    {
        return;
    }
    
    month = clock->month;
    day = clock->day; 
    c_time = clock->hour*60 + clock->minutes;
    t_time = time_data[4]*60 + time_data[5];
    
    Sclock_SynchronizationTime(time_data, 7);
 
    if(month != time_data[2] || day != time_data[3])
    {
//        App_StorageSystemTimeHandler();
        if(Movement_IsBusy())
        {
            Movement_Exit();
        }
        App_AcrossTheDayHandler();
        App_RoutineEventDelete();
        App_DeleteEvent(APP_STORAGE_TIME_EVENT);
        
        Phys_UpdataInfo();
    }
    else
    {
        //WJ.20170905.�޸�Ϊ2Сʱ��ԭ��Ϊ1Сʱ
        if(t_time + 120 < c_time)
        {   //��ǰ����ʱ�����1��Сʱ           
            if(Movement_IsBusy())
            {
                Movement_Exit();
            }
            
            App_StorageSystemTimeHandler();
            App_RoutineEventDelete();
            Daily_ClearDailyCurrentBlcak();
        }
    }
//    App_DeleteEvent(APP_STORAGE_TIME_EVENT);
    
    Alarm_ValidCheck();
}

/****************************************************************
** Function name:           Protocol_DeviceInfo
** Descriptions:            ��ȡ�豸��
** input parameters:        p_nus�����ݽ���ͨ����p_data:���յ�����
** output parameters:       ��
** Returned value:          ��
** Created by:              SQH              
** Created Date:            2016-10-19
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
static void Protocol_DeviceInfo(ble_nus_t *p_nus, uint8_t *p_data)
{
    uint8_t ble_ack[20] = {0x00, };
   
    ble_ack[0] = p_data[0]; //0xA4
    
    ble_ack[1] = 0;//(VEEPOO_DEVICE_ID>>8) & 0xFF;
    ble_ack[2] = 0;//VEEPOO_DEVICE_ID & 0xFF;
    
    Ble_SendDataViaBat(ble_ack, 20);  
}

/****************************************************************
** Function name:           SetDeviceTime
** Descriptions:            �趨�豸ʱ��
** input parameters:        p_nus�����ݽ���ͨ����p_data:���յ�����
** output parameters:       ��
** Returned value:          ��
** Created by:              SQH              
** Created Date:            2016-10-19
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
static void Protocol_SetDeviceTime(ble_nus_t *p_nus, uint8_t *p_data)
{
    uint8_t ble_ack[20] = {0x00, };
    uint8_t time_data[7] = {0, };
   
    ble_ack[0] = p_data[0];

    if(Sclock_TimeCheck(p_data + 1, 7) == 0x01)
    {                                                                                                         
        if(system_flag_s.decryption_ok) //����ͨ��   
        {
            for(uint8_t i = 1; i <= 7; i++)
            {                         
                time_data[i-1] = p_data[i];                      
            }   
            if(p_data[8] == 0x01)
            {   // 12Сʱ��
                system_switch_s.time_formats = 0x02;
            }
            else if(p_data[8] == 0x02)
            {   // 24Сʱ��
                system_switch_s.time_formats = 0x01;
            }
        }
        else
        {
            time_data[0]=0x04;
            time_data[1]=0x57;                                                       
            time_data[2]=0x0B;
            time_data[3]=0x0B;                                                       
            time_data[4]=0x0B;
            time_data[5]=0x0B;
        }                            
        Protocol_TimeChange(time_data);
//        Sclock_SynchronizationTime(time_data, 7);

        system_flag_s.first_time_check_flag = TRUE;//�ֻ���ʱ���Ѿ���APPУ׼ͬ�� 
        ble_ack[1] = 0x01;//�ɹ�
    }
    else
    {
        ble_ack[1] = 0;//���ɹ�
    }
    Ble_SendDataViaBat(ble_ack, 20);    
}


/****************************************************************
** Function name:           Protocol_ReadDeviceTime
** Descriptions:            ��ȡ�豸ʱ��
** input parameters:        p_nus�����ݽ���ͨ����p_data:���յ�����
** output parameters:       ��
** Returned value:          ��
** Created by:              SQH              
** Created Date:            2016-10-19
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
static void Protocol_ReadDeviceTime(ble_nus_t *p_nus, uint8_t *p_data)
{
    uint8_t ble_ack[20] = {0x00, };
    
    ble_ack[0] = 0xA6;
    
    Sclock_ReadTimeToBuf(&ble_ack[1], 7);
    
    if(system_switch_s.time_formats == 0x02)
    {   // 12Сʱ��
        ble_ack[8] = 0x01;
    }
    else 
    {   // 24Сʱ��
        ble_ack[8] = 0x02;
    } 
    
    Ble_SendDataViaBat(ble_ack, 20);		//�ϴ�ʱ�� 
}

/****************************************************************
** Function name:           GestureSwitch
** Descriptions:            ̧����������
** input parameters:        p_nus�����ݽ���ͨ����p_data:���յ�����
** output parameters:       ��
** Returned value:          ��
** Created by:              SQH              
** Created Date:            2016-10-19
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
static void Protocol_GestureSwitch(ble_nus_t *p_nus, uint8_t *p_data)
{
    uint8_t ble_ack[20] = {0x00, };
    
    ble_ack[0] = p_data[0]; //0xAA
    if(p_data[1] == 0x00 || p_data[1] == 0x01)   
    {
        // set Raised my hand against the bright screen to closed
        if(!memcmp(&p_data[2], &p_data[4], 2)        
            || p_data[4] > 23  || p_data[3] > 59 || p_data[5] > 59)
        {
            ble_ack[1] = 0;     //���ݲ�����
        }
        else
        {                 
            system_info_s.turnwrist_config.enable = p_data[1]; //ʹ��
            
            system_info_s.turnwrist_config.s_hour = p_data[2];
            system_info_s.turnwrist_config.s_minute = p_data[3]; 
            system_info_s.turnwrist_config.e_hour = p_data[4];
            system_info_s.turnwrist_config.e_minute = p_data[5]; 
            if(p_data[6] > 0 && p_data[6] < 10)
            {
                system_info_s.gesture_level = p_data[6];
            }
            ble_ack[1] = 1;
            App_SetSystemInfoSave();
        }
    }
    else if(p_data[1] == 0x02)  // read state of Raised my hand against the bright screen
    {
        ble_ack[1] = 1;
    }	
    
    ble_ack[2] = p_data[1]; 
    ble_ack[3] = system_info_s.turnwrist_config.s_hour;
    ble_ack[4] = system_info_s.turnwrist_config.s_minute;
    ble_ack[5] = system_info_s.turnwrist_config.e_hour;
    ble_ack[6] = system_info_s.turnwrist_config.e_minute;
    ble_ack[7] = (system_info_s.turnwrist_config.enable==1)?1:2;
    ble_ack[8] = system_info_s.gesture_level;
    ble_ack[9] = 5;
    Ble_SendDataViaBat(ble_ack, 20);    
}

/****************************************************************
** Function name:           ANCSNotiryControl
** Descriptions:            ANCS����֪ͨ
** input parameters:        p_nus�����ݽ���ͨ����p_data:���յ�����
** output parameters:       ��
** Returned value:          ��
** Created by:              SQH              
** Created Date:            2016-10-19
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
static void Protocol_AncsNotiryControl(ble_nus_t *p_nus, uint8_t *p_data)
{
    uint8_t ble_ack[20] = {0x00, };
    
    if(p_data[1] == 0x01)
    {	
        /* ������Ӧ�ı��� */
        Protocol_SetAncsConfig(p_data);
        return;
    }
    else if(p_data[1] == 0x02)
    {
        /* ��ȡ���� */
        Protocol_UploadAncsConfig();
        return;
    }
    else
    {
        p_data[1] = 0x00;
    }

    Ble_SendDataViaBat(ble_ack, 20);		//�ϴ�app
}

/****************************************************************
** Function name:           Protocol_SetAncsConfig
** Descriptions:            ������Ϣ���Ϳ���
** input parameters:        p_nus�����ݽ���ͨ����p_data:���յ�����
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2016-12-30
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
static void Protocol_SetAncsConfig(uint8_t *p_data)
{
    uint8_t ble_ack[20]={0xAD, 0x00, 0x00, 0x01, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    message_enable_t  *message_enable_s = NULL;

    memcpy(ble_ack, p_data, 20);
    message_enable_s = &(system_info_s.message_enable);

    /* ���濪�ر��� */
    if(ble_ack[2] == 0x02)
    {
        message_enable_s->call_enable = 0x00;
    }
    else if(ble_ack[2] == 0x01)
    {
        message_enable_s->call_enable = 0x01;
    }
    
    if(ble_ack[3] == 0x02)
    {
        message_enable_s->sms_enable = 0x00;
    }
    else if(ble_ack[3] == 0x01)
    {
        message_enable_s->sms_enable = 0x01;
    }
    
    if(ble_ack[4] == 0x02)
    {
        message_enable_s->wechat_enable = 0x00;
    }
    else if(ble_ack[4] == 0x01)
    {
        message_enable_s->wechat_enable = 0x01;
    }
    
    if(ble_ack[5] == 0x02)
    {
        message_enable_s->qq_enable = 0x00;
    }
    else if(ble_ack[5] == 0x01)           
    {
        message_enable_s->qq_enable = 0x01;
    }
    
    if(ble_ack[7] == 0x02 )
    {
        message_enable_s->facebook_enable = 0x00;
    }
    else if(ble_ack[7] == 0x01)
    {
        message_enable_s->facebook_enable = 0x01;
    }
    
    if(ble_ack[8] == 0x02)
    {
        message_enable_s->twitter_enable = 0x00;
    }
    else if(ble_ack[8] == 0x01)
    {
        message_enable_s->twitter_enable = 0x01;
    }
    
    if(ble_ack[10] == 0x02)
    {
        message_enable_s->linkedin_enable = 0x00;
    }
    else if(ble_ack[10] == 0x01)
    {
        message_enable_s->linkedin_enable = 0x01;
    }   
    
    if(ble_ack[11] == 0x02)
    {
        message_enable_s->whatsapp_enable = 0x00;
    }
    else if(ble_ack[11] == 0x01)
    {
        message_enable_s->whatsapp_enable = 0x01;
    }  
    
    if(ble_ack[12] == 0x02)           
    {
        message_enable_s->line_enable = 0x00;
    }
    else if(ble_ack[12] == 0x01)
    {
       message_enable_s->line_enable = 0x01;
    }
    
    if(ble_ack[13] == 0x02)
    {
        message_enable_s->instagram_enable = 0x00;
    }
    else if(ble_ack[13] == 0x01)
    {
        message_enable_s->instagram_enable = 0x01;
    }
    
    if(ble_ack[14] == 0x02)
    {
        message_enable_s->snapchat_enable = 0x00;
    }
    else if(ble_ack[14] == 0x01)
    {
        message_enable_s->snapchat_enable = 0x01;
    }
    
    if(ble_ack[15] == 0x02)
    {
        message_enable_s->skype_enable = 0x00;
    }
    else if(ble_ack[15] == 0x01)
    {
        message_enable_s->skype_enable = 0x01;
    }
    
    if(ble_ack[16] == 0x02)
    {
        message_enable_s->gmail_enable = 0x00;
    }
    else if(ble_ack[16] == 0x01)
    {
        message_enable_s->gmail_enable = 0x01;
    }
    
    if(ble_ack[19] == 0x02)
    {
        message_enable_s->other_enable = 0x00;
    }
    else if(ble_ack[19] == 0x01)
    {
        message_enable_s->other_enable = 0x01;    
    }
    message_enable_s->resv = 0;
    message_enable_s->retain = 0;
    App_SetSystemInfoSave();
    
    Ble_SendDataViaBat( ble_ack, 20);		//�ϴ�app
}


/****************************************************************
** Function name:           Protocol_FindPhoneSet
** Descriptions:            �ֻ����ҹ��ܿ���
** input parameters:        p_nus�����ݽ���ͨ����p_data:���յ�����
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2016-12-30
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
static void Protocol_FindPhoneSet(ble_nus_t *p_nus, uint8_t *p_data)
{
    uint8_t ble_ack[20] = {0x00, };
    
    ble_ack[0] = p_data[0];
//    if(p_data[1] == 0)
//    {
//        system_info_s.lost_phone = 0x02;
//        App_SetSystemInfoSave();
//    }
//    else if(p_data[1] == 1)
//    {
//        system_info_s.lost_phone = 0x01;
//        App_SetSystemInfoSave();
//    }
//    ble_ack[1] = 0x01;
//    ble_ack[2] = p_data[1];
//    ble_ack[3] = system_info_s.lost_phone;    //Wj.20170427.����״̬
    
    Ble_SendDataViaBat( ble_ack, 20);		//�ϴ�app    
}

/****************************************************************
** Function name:           Protocol_UploadAncsConfig
** Descriptions:            �ϱ���Ϣ����״̬
** input parameters:        p_nus�����ݽ���ͨ����p_data:���յ�����
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2016-12-30
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
static void Protocol_UploadAncsConfig(void)
{
    uint8_t ble_ack[20]={0xAD, 0x02, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    message_enable_t  *message_enable_s = NULL;

    message_enable_s = &(system_info_s.message_enable);
                    
    /* ��ȡ�绰������Ϣ */
    if(message_enable_s->call_enable == 0x01)
    {
        ble_ack[2] = 0x01;
    }
    else 
    {
        ble_ack[2] = 0x02;
    }
    if(message_enable_s->sms_enable == 0x01)
    {
    
        /* ��ȡ����������Ϣ */
        ble_ack[3] = 0x01;
    }
    else 
    {
        ble_ack[3] = 0x02;
    }    
    if(message_enable_s->wechat_enable == 0x01)
    {
        /* ��ȡ΢��������Ϣ */
        ble_ack[4] = 0x01;
    }
    else 
    {
        ble_ack[4] = 0x02;
    }
    if(message_enable_s->qq_enable == 0x01)
    {    
        /* ��ȡqq������Ϣ */
        ble_ack[5] = 0x01;
    }
    else 
    {
        ble_ack[5] = 0x02;
    }
    if(message_enable_s->facebook_enable == 0x01)
    {
        /* ��ȡFacebook������Ϣ */
        ble_ack[7] = 0x01;
    }
    else 
    {
        ble_ack[7] = 0x02;
    }
    if(message_enable_s->twitter_enable == 0x01)
    {   
        /* ��ȡTwitter������Ϣ */
        ble_ack[8] = 0x01;
    }
    else
    {
        ble_ack[8] = 0x02;
    }
    if(message_enable_s->linkedin_enable == 0x01)
    {
        /* ��ȡlinkedin������Ϣ */
        ble_ack[10] = 0x01;
    }
    else
    {   
        ble_ack[10] = 0x02;
    }
    if(message_enable_s->whatsapp_enable == 0x01)
    {
        /* ��ȡWhatsApp������Ϣ */
        ble_ack[11] = 0x01;
    }
    else
    {
        ble_ack[11] = 0x02;
    }
    if(message_enable_s->line_enable == 0x01)
    {    
        /* ��ȡline������Ϣ */
        ble_ack[12] = 0x01;
    }
    else
    {
        ble_ack[12] = 0x02;
    }    
    if(message_enable_s->instagram_enable == 0x01)
    {
        /* ��ȡinstagram������Ϣ */
        ble_ack[13] = 0x01;
    }
    else
    {
        ble_ack[13] = 0x02;
    }    
    if(message_enable_s->snapchat_enable == 0x01)
    {
        /* ��ȡSnapchat������Ϣ */
        ble_ack[14] = 0x01;
    }
    else
    {
         ble_ack[14] = 0x02;
    }
    if(message_enable_s->skype_enable == 0x01)
    {    
        /* ��ȡskype������Ϣ */
        ble_ack[15] = 0x01;
    }
    else
    {
        ble_ack[15] = 0x02;
    }    
    if(message_enable_s->gmail_enable == 0x01)
    {
        /* ��ȡgmail������Ϣ */
        ble_ack[16] = 0x01;
    }
    else
    {
         ble_ack[16] = 0x02;
    }
    if(message_enable_s->other_enable == 0x01)
    {
        /* ��ȡ����������Ϣ */
        ble_ack[19] = 0x01;
    }
    else
    {
        ble_ack[19] = 0x02;
    }
    
    Ble_SendDataViaBat(ble_ack, 20);		//�ϴ�app
}

/****************************************************************
** Function name:           Protocol_AlarmClockSet
** Descriptions:            �������ã��ɣ�
** input parameters:        p_nus�����ݽ���ͨ����p_data:���յ�����
** output parameters:       ��
** Returned value:          ��
** Created by:              SQH              
** Created Date:            2016-10-19
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
static void Protocol_AlarmClockSet(ble_nus_t *p_nus, uint8_t *p_data)
{
    uint8_t ble_ack[20] = {0x00, }; 
    ble_ack[0] = p_data[0]; //0xAB      
    
    if(0x10 == p_data[10])      //������Ӧ��
    {
#if APP_MOTOR_ENABLED        
        Motor_AddEvent(6, 1, 1, 1);//�������� ÿ����1�� ���1��               
#endif
        
    }
    else
    {
        ble_ack[1] = 0;                   //���ò��ɹ�
    }
        
    Ble_SendDataViaBat(ble_ack, 20);
}

/****************************************************************
** Function name:           Protocol_SetHeartAlarmRage
** Descriptions:            �������ʸ澯��ֵ
** input parameters:        p_nus�����ݽ���ͨ����p_data:���յ�����
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ             
** Created Date:            2016-11-19
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
static void Protocol_SetHeartAlarmRage(ble_nus_t *p_nus, uint8_t *p_data)
{
    uint8_t ble_ack[20] = {0x00, };
    
    ble_ack[0] = p_data[0]; //0xAC
    if(p_data[3] == 0x00)
    {
        system_info_s.heart_alarm_enable.heart_enable = 0;
        //��������
        App_SetSystemInfoSave();
    }
    else if(p_data[3] == 0x01)
    {
        system_info_s.heart_alarm_enable.heart_enable = 1;
        system_info_s.heart_alarm_enable.heart_hight = p_data[1];
        system_info_s.heart_alarm_enable.heart_low = p_data[2];
        //��������
        App_SetSystemInfoSave();        
    }
    else if(p_data[3] == 0x02)
    {
        
    }   
    
    ble_ack[1] = system_info_s.heart_alarm_enable.heart_hight;
    ble_ack[2] = system_info_s.heart_alarm_enable.heart_low;
    ble_ack[3] = 0x01;
    ble_ack[4] = p_data[3];
    ble_ack[5] = system_info_s.heart_alarm_enable.heart_enable; 
    
    Ble_SendDataViaBat(ble_ack, 20);    
}

/****************************************************************
** Function name:           Protocol_ReadDeviceFunction
** Descriptions:            ��ȡ�豸������Ϣ
** input parameters:        p_nus�����ݽ���ͨ����p_data:���յ�����
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ             
** Created Date:            2016-11-19
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
static void Protocol_ReadDeviceFunction(ble_nus_t *p_nus, uint8_t *p_data)
{      
    Protocol_UploadFunConfig(); 
}

/****************************************************************
** Function name:           Protocol_ReadWalkCnt
** Descriptions:            Э���ȡ�ǲ����ݣ�ȫ��ģ�
** input parameters:        p_nus�����ݽ���ͨ����p_data:���յ�����
** output parameters:       ��
** Returned value:          ��
** Created by:              SQH              
** Created Date:            2016-10-19
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
static void Protocol_ReadWalkCnt(ble_nus_t *p_nus, uint8_t *p_data)
{
    uint8_t ble_ack[20] = {0x00, };
    uint32_t all_step = 0;
    
    ble_ack[0] = p_data[0];    //0xA8
    if(p_data[1] == 0x00) //����
    {                                                      
        all_step = All_StepGet(); 
        ble_ack[1] = (all_step >> 24) & 0xFF;
        ble_ack[2] = (all_step >> 16) & 0xFF;
        ble_ack[3] = (all_step >>  8) & 0xFF;
        ble_ack[4] =  all_step        & 0xFF;                                 
    }
    else if(p_data[1] == 0x01 || p_data[1] == 0x02) //ǰ���졡/��ǰ����
    {
        all_step = Daily_GetAllWalkCntByDays(p_data[1]);
        ble_ack[1] = (all_step >> 24) & 0xFF;
        ble_ack[2] = (all_step >> 16) & 0xFF;
        ble_ack[3] = (all_step >>  8) & 0xFF;
        ble_ack[4] =  all_step        & 0xFF; 
    }
    else if(p_data[1] == 0x03)
    {
        system_flag_s.step_gauge_report = 1; //ÿ���붼Ҫ�ϴ�����
    }
    else if(p_data[1] == 0x04)
    {
        system_flag_s.step_gauge_report = 0; //ÿ���붼Ҫ�ϴ��ر�  
    }
    
    if(!system_flag_s.decryption_ok) //���ܲ�ͨ��    2015-01-26
    {
        ble_ack[2] = 0x0C; 
    }
    ble_ack[5] = p_data[1];        
    Ble_SendDataViaBat(ble_ack, 20);//�ϴ��Ʋ����	                    
}

/****************************************************************
** Function name:           Protocol_ReadSportData
** Descriptions:            ��ȡ�Ʋ�����
** input parameters:        p_nus�����ݽ���ͨ����p_data:���յ�����
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ             
** Created Date:            2016-11-19
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
static void Protocol_ReadSportData(ble_nus_t *p_nus, uint8_t *p_data)
{
    uint8_t ble_ack[20] = {0x00, };
    uint32_t all_step = 0;
    
    ble_ack[0] = p_data[0];    //0xA9
                                                    
    all_step = All_StepGet(); 
    ble_ack[1] = ble_ack[5] = (all_step >> 24) & 0xFF;
    ble_ack[2] = ble_ack[6] = (all_step >> 16) & 0xFF;
    ble_ack[3] = ble_ack[7] = (all_step >>  8) & 0xFF;
    ble_ack[4] = ble_ack[8] = all_step        & 0xFF;                                 

    ble_ack[9]  = 0;    //�˶�����
    ble_ack[10] = 0;    //�˶�����
    
    ble_ack[5] = p_data[1];        
    Ble_SendDataViaBat(ble_ack, 20);//�ϴ��Ʋ����	                    
}

/****************************************************************
** Function name:           Protocol_SetBluetoothDisconnect
** Descriptions:            Disconnect the bluetooth device the initiative
** input parameters:        p_nus�����ݽ���ͨ����p_data:���յ�����
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ             
** Created Date:            2016-11-19
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/  
static void Protocol_SetBluetoothDisconnect(ble_nus_t *p_nus, uint8_t *p_data)
{
    //Disconnect the bluetooth device the initiative
    Ble_DisconnectBle();
}

/****************************************************************
** Function name:           Protocol_ReadCurHeart
** Descriptions:            read current heart
** input parameters:        p_nus�����ݽ���ͨ����p_data:���յ�����
** output parameters:       ��
** Returned value:          ��
** Created by:              SQH              
** Created Date:            2016-10-19
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
static void Protocol_ReadCurHeart(uint8_t *p_data)
{
    uint8_t ble_ack[20] = {0x00, };
    
    ble_ack[0] = p_data[0];
    
    if(p_data[1] == 0x01)
    {
//        if(!system_flag_s.power_state)//�������� ����� ���͵�
        {
            if(system_flag_s.measuring_status < TIME_MAX_MEASURING_STATUS)
            {                
                App_StopMeasuringHandler(TEST_CLOSE_ALL); 
                system_flag_s.heart_led_status = true;
                system_flag_s.measuring_status = APP_HEARTING_STATUS;                
                Heart_Open();
            }
            else
            {
                ble_ack[5] = Protocol_MeasuringStatus(system_flag_s.measuring_status);
                if(ble_ack[5] == 2)
                {
                    system_flag_s.app_heart_status = 1;
                }
            }
        }
    }
    else if(p_data[1] == 0x00)
    {
        if(system_flag_s.measuring_status == APP_HEARTING_STATUS)
        {
            Heart_Close();
            system_flag_s.heart_led_status = false;
            system_flag_s.measuring_status = EVENT_NULL;
            system_flag_s.app_heart_status = 0;
        }
    }   
    
    Ble_SendDataViaBat(ble_ack, 20);
}

/****************************************************************
** Function name:           Protocol_SendDailyData
** Descriptions:            
** input parameters:        p_nus�����ݽ���ͨ����p_data:���յ�����
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ            
** Created Date:            2016-12-12
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
static void Protocol_SendDailyData(ble_nus_t *p_nus, uint8_t *p_data)
{
    uint8_t ble_ack[20] = {0x00, };
    uint8_t block = 0;          // flash addr
    uint8_t month = 0;
    uint8_t day = 0;
    uint16_t start_num = 0;      // data len
    uint16_t all_pkg = 0;
    uint8_t daily_falg = 0;
    daily_pkg_info_t *daily_pkg_info;
    daily_block_info_t daily_block_info;
    
    daily_pkg_info = Daily_GetPkgInfo();
    ble_ack[0] = p_data[0]; //0xD1
    
    
    if(p_data[3] < 5)
    {
        if(p_data[3] == 0)
        {
            block = daily_pkg_info->block;
            all_pkg = daily_pkg_info->pkg_cnt;
        }
        else
        {          
            block = Daily_GetPreviousBlock( p_data[3]);
            App_FlashReadDailyHead(block, (void *)&daily_block_info, sizeof(daily_block_info_t));           
            Sclock_GetPreviousDayDate (p_data[3], &month, &day);
            if(daily_block_info.block_flag == 0x00 
                && daily_block_info.month == month && daily_block_info.day == day)
            {
                all_pkg = daily_block_info.pack_num;
            }
            else
            {
                daily_falg = 1;
            }
        }
 
        if(daily_falg == 0)
        {
            start_num = p_data[1] + (p_data[2] << 8);
            if(start_num > all_pkg  || start_num == 0)
            {
                start_num = 1;      //WJ.20170420.�޸��ճ����ݴ��䲻��ȷ��ֻ��һ����
            }
            
            if(all_pkg > DAILY_PKG_MAX_NUM)
            {
                all_pkg = DAILY_PKG_MAX_NUM;
            }
            
            if(start_num != all_pkg && all_pkg <= DAILY_PKG_MAX_NUM && all_pkg >= 1)
            {
                start_num -= 1;//WJ.20170420.�޸��ճ����ݴ��䲻��ȷ��ֻ��һ����
                //all_pkg = all_pkg - start_num;//WJ.20170420.�޸��ճ����ݴ��䲻��ȷ��ֻ��һ����
                //App_SetFlashDaliyDataInfo(block, start_num, end_num, all_pkg, 0, p_data[3]);
                
                App_SetFlashDaliyDataInfo(block, 0, all_pkg, start_num, p_data[3]);
                App_SetSendFlashDataType(FLASH_DATA_TYPE_DAILY);
                App_AddEvent(APP_SEND_FLASH_DATA_EVENT);
            }
            else
            {
                daily_falg = 1;
            }
        }        
    }
    else
    {
        daily_falg = 1;
    }

    if(daily_falg == 1)
    {
        ble_ack[1] = 0x00;
        ble_ack[2] = 0x00;    
        ble_ack[3] = 0x00;
        ble_ack[4] = 0x00;     
        ble_ack[5] = p_data[3] << 5 ;    
        Ble_SendDataViaBat(ble_ack, 20); 
    }
}

/****************************************************************
** Function name:           Protocol_GetMovementPatternsDataCRC
** Descriptions:            ��ȡ�˶�ģʽ����CRC
** input parameters:        p_nus�����ݽ���ͨ����p_data:���յ�����
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ             
** Created Date:            2017-08-08
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
static void Protocol_GetMovementPatternsDataCRC(ble_nus_t *p_nus, uint8_t *p_data)   
{
    uint8_t ble_ack[20] = {0x00, };

    ble_ack[0] = p_data[0]; 
    ble_ack[1] = p_data[1];     
    ble_ack[2] = 0x01; 
    if(p_data[1] == 0x01)
    {
        Movement_GetHeadCRC(&ble_ack[3], 3);
    }
        
    Ble_SendDataViaBat(ble_ack, 20); 
}

/****************************************************************
** Function name:           Protocol_GetMovementPatternsData
** Descriptions:            ��ȡ�˶�ģʽ�洢����
** input parameters:        p_nus�����ݽ���ͨ����p_data:���յ�����
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ             
** Created Date:            2017-08-08
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
static void Protocol_GetMovementPatternsData(ble_nus_t *p_nus, uint8_t *p_data)
{
    uint8_t ble_ack[20] = {0x00, };
    uint8_t ret = 0;

    if(p_data[1] < MOVEMENT_PATTERNS_MAX_BLOCK)
    {
        ret = Movement_SetSendDataInfo(p_data[1]);        
    }
    
    if(ret == 1)
    {
        App_SetSendFlashDataType(FLASH_DATA_TYPE_MOVEMENT);
        App_AddEvent(APP_SEND_FLASH_DATA_EVENT); 
    }
    else
    {       
        ble_ack[0] = p_data[0]; 
        ble_ack[5] = p_data[1];    
        Ble_SendDataViaBat(ble_ack, 20);     
    }
}

/****************************************************************
** Function name:           Protocol_OpreateMovementPatternsData
** Descriptions:            �˶�ģʽ�����������͹ر�
** input parameters:        p_nus�����ݽ���ͨ����p_data:���յ�����
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ             
** Created Date:            2017-08-08
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
static void Protocol_OpreateMovementPatternsData(ble_nus_t *p_nus, uint8_t *p_data)
{
    uint8_t ble_ack[20] = {0x00, };
    
    ble_ack[0] = p_data[0]; 
    ble_ack[1] = p_data[1];      
    ble_ack[2] = 0x01; 
    if(p_data[1] == 0)
    {     
        /* �ر�*/        
        if(Movement_IsBusy())
        {
            Movement_Exit();
            if(Gui_ThreadPageActive())
            {
                Gui_ThreadTaskWakeUp(CPU_TASK_IDEL);
            }
            
            ble_ack[2] = 0x01; 
            ble_ack[3] = 0x00; 
        }
        else
        {
            ble_ack[2] = 0x01; 
            ble_ack[3] = 0x00; 
        }
    }
    else if(p_data[1] == 1)
    {
        /* ����*/
        if(Watch_IsBusy() || Battery_IsChargingFull() || Battery_IsCharging())
        {
            ble_ack[2] = 0x00; 
            ble_ack[3] = 0x02; 
        }
        else if(Movement_IsBusy())       
        {
            ble_ack[2] = 0x00; 
            ble_ack[3] = 0x01; 
        }
        else
        {
            Movement_Start();
            Gui_ThreadTaskWakeUp(CPU_TASK_MOVEMENT);
            ble_ack[2] = 0x01; 
            ble_ack[3] = 0x00; 
        }
    }
    else if(p_data[1] == 2)    
    {
        //��ȡ�˶�ģʽ����״̬
        ble_ack[2] = 0x01; 
        if(Movement_IsBusy())       
        {
            ble_ack[3] = 0x01; 
        }
        else
        {             
            ble_ack[3] = 0x00; 
        }
    }
    else
    {
        ble_ack[2] = 0x00; 
    }
 
    Ble_SendDataViaBat(ble_ack, 20);    
}

/****************************************************************
** Function name:           Protocol_SetMovementPatternsPuseTimeOut
** Descriptions:            �����˶�ģʽ����ͣ��ʱʱ��
** input parameters:        p_nus�����ݽ���ͨ����p_data:���յ�����
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ             
** Created Date:            2017-08-08
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
static void Protocol_SetMovementPatternsPuseTimeOut(ble_nus_t *p_nus, uint8_t *p_data)
{
    uint8_t ble_ack[20] = {0x00, };
    uint16_t timeout_time = 0;
    
    ble_ack[0] = p_data[0];   
    ble_ack[1] = p_data[1];    
    ble_ack[2] = 0x01;       
    if(p_data[1] == 0x01)
    {       
        timeout_time = p_data[2];
        if(Movement_IsVaildThePuseTimeOut(timeout_time))
        {
            system_info_s.movement_max_puse_time = timeout_time;//p_data[2];           
            App_SetSystemInfoSave();
        }
        else
        {
            ble_ack[2] = 0x00;  
        }
    }
    else if(p_data[1] != 0x02)
    {
        ble_ack[2] = 0x00;  
    }
    
    ble_ack[3] = system_info_s.movement_max_puse_time; 
    Ble_SendDataViaBat(ble_ack, 20);    
}

/****************************************************************
** Function name:           ReadDrinkingData
** Descriptions:            Get drinking data
** input parameters:        p_nus�����ݽ���ͨ����p_data:���յ�����
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ             
** Created Date:            2016-11-19
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
static void Protocol_ReadDrinkingData(ble_nus_t *p_nus, uint8_t *p_data)
{
    //��ȡ��������
}

/****************************************************************
** Function name:           Protocol_GetAllMotionData
** Descriptions:            ��ȡ�Ʋ�����·���������
** input parameters:        p_nus�����ݽ���ͨ����p_data:���յ�����
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ             
** Created Date:            2017-08-15
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
static void Protocol_GetAllMotionData(ble_nus_t *p_nus, uint8_t *p_data)
{
    uint8_t ble_ack[20] = {0x00, };
    uint8_t block = 0;          // flash addr
    uint8_t month = 0;
    uint8_t day = 0;
    daily_block_info_t daily_block_info = {0x00, };
    sport_information_info_t gsii_sport_info_s = {0x00,};    
    
    ble_ack[0] = p_data[0];   
    ble_ack[1] = p_data[1];    

   if(p_data[1] < 5)
   {             
        if(p_data[1] == 0)
        {
            Get_SportInformationInfo(&gsii_sport_info_s);
            memcpy(&ble_ack[2], &gsii_sport_info_s.posi_steps, 4);   
            memcpy(&ble_ack[6], &gsii_sport_info_s.posi_distance, 4); 
            gsii_sport_info_s.posi_cal = gsii_sport_info_s.posi_cal/100;
            memcpy(&ble_ack[10], &gsii_sport_info_s.posi_cal, 4);             
        }
        else
        {          
            block = Daily_GetPreviousBlock(p_data[1]);
            App_FlashReadDailyHead(block, (void *)&daily_block_info, sizeof(daily_block_info_t));           
            Sclock_GetPreviousDayDate (p_data[1], &month, &day);
            if(daily_block_info.block_flag == 0x00 
              && daily_block_info.month == month && daily_block_info.day == day)
            {
                memcpy(&ble_ack[2], &daily_block_info.all_step, 4);   
                memcpy(&ble_ack[6], &daily_block_info.all_distance, 4); 
                daily_block_info.all_calories = daily_block_info.all_calories/100;
                memcpy(&ble_ack[10], &daily_block_info.all_calories, 4); 
            }
        }       
    }
  
    Ble_SendDataViaBat(ble_ack, 20);     
}

/****************************************************************
** Function name:           Protocol_DailySleepCount
** Descriptions:            ����˯�ߴ���
** input parameters:        sleep_cnt��˯�ߴ���ԭʼֵ����λ��ʾ
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ             
** Created Date:            2016-12-12
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
static uint8_t Protocol_DailySleepCount(uint16_t sleep_cnt)
{
    uint8_t i = 0;
    uint8_t sleep_count = 0;
    
    for(i = 15; i > 0; i--)
    {
        if((sleep_cnt >> i) == 0x00)
        {
            sleep_count++;
        }
        else
        {
            break;
        }
    }
    
    if(sleep_count > 10)
    {
        sleep_count = 10;
    }
    
    return sleep_count;
}

/****************************************************************
** Function name:           Protocol_SendDailySleepData
** Descriptions:            Get sleep data
** input parameters:        p_nus�����ݽ���ͨ����p_data:���յ�����
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ             
** Created Date:            2016-12-12
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
static void Protocol_SendDailySleepData(ble_nus_t *p_nus, uint8_t *p_data)
{
    uint8_t ble_ack[20] = {0x00, };
    uint8_t block = 0;          // flash addr
    uint16_t sleep_cnt = 0;
    uint8_t daily_sleep_falg = 0;
    uint8_t month = 0;
    uint8_t day = 0;
    daily_pkg_info_t *daily_pkg_info;
    daily_block_info_t daily_block_info;
    
    
    daily_pkg_info = Daily_GetPkgInfo();
    ble_ack[0] = p_data[0]; //0xE0
    
    if(p_data[1] < 5)
    {
        if(p_data[1] == 0)
        {
            block = daily_pkg_info->block;
        }
        else
        {          
        
            block = Daily_GetPreviousBlock( p_data[1]);
        }
 
        App_FlashReadDailyHead(block, (void *)&daily_block_info, sizeof(daily_block_info_t)); 
        Sclock_GetPreviousDayDate (p_data[3], &month, &day);
       
        if((daily_block_info.block_flag != 0x00 && daily_block_info.block_flag != 0xAA))    //WJ.20170322
//            || (daily_block_info.month != month || daily_block_info.day != day))
        {
            daily_sleep_falg = 0x01;
        }
        else
        {
            sleep_cnt = Protocol_DailySleepCount(daily_block_info.sleep_cnt);
            
            if(sleep_cnt > 0)
            {                          
                App_SetFlashDaliySleepDataInfo(block, p_data[1], sleep_cnt, sleep_cnt*6, 0);
                App_SetSendFlashDataType(FLASH_DATA_TYPE_SLEEP);
                App_AddEvent(APP_SEND_FLASH_DATA_EVENT);
            }
            else
            {
                daily_sleep_falg = 0x01;
            }
        }             
    }

    if(daily_sleep_falg == 0x01)
    {
        ble_ack[1] = 0x00;
        ble_ack[2] = 0x00;        
        ble_ack[3] = p_data[1];      
        Ble_SendDataViaBat(ble_ack, 20); 
    }        
}

/****************************************************************
** Function name:           Protocol_SetSedentaryTime
** Descriptions:            Get drinking data
** input parameters:        p_nus�����ݽ���ͨ����p_data:���յ�����
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ             
** Created Date:            2016-11-19
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
static void Protocol_SetSedentaryTime(ble_nus_t *p_nus, uint8_t *p_data)
{
    uint8_t ble_ack[20] = {0x00, };
    outsit_t outsit_temp = {0x00, };    
    
    ble_ack[0] = p_data[0]; //0xE1 
    if(system_flag_s.sit_flag != 1)
    {
        Ble_SendDataViaBat(ble_ack, 20); 
        return;
    }
    
    ble_ack[1] = 0x01; 
    
    if(p_data[6] == 0x00 || p_data[6] == 0x01)   //close sedentary
    {    
        outsit_temp.on_off_flag = p_data[6]; ;
        outsit_temp.start_time[0] = p_data[1];
        outsit_temp.start_time[1] = p_data[2];
        outsit_temp.end_time[0] = p_data[3];
        outsit_temp.end_time[1] = p_data[4];
        outsit_temp.threshold_value = p_data[5];
        if(Outsit_ConfigCheck(&outsit_temp))
        {
            //������Ϣд��flash
            memcpy(&system_info_s.outsit_cfg, &outsit_temp, sizeof(outsit_temp));
            App_SetSystemInfoSave();
            Outsit_Init();
        }
        else
        {
            ble_ack[1] = 0x00; 
        }
    }

    ble_ack[2] = system_info_s.outsit_cfg.start_time[0];
    ble_ack[3] = system_info_s.outsit_cfg.start_time[1];
    ble_ack[4] = system_info_s.outsit_cfg.end_time[0];
    ble_ack[5] = system_info_s.outsit_cfg.end_time[1];
    ble_ack[6] = system_info_s.outsit_cfg.threshold_value;  
 
    //��ǰ����״̬
    ble_ack[7] = system_info_s.outsit_cfg.on_off_flag; 
    
    ble_ack[8] = p_data[6];
    Ble_SendDataViaBat(ble_ack, 20);    
}

/****************************************************************
** Function name:           Protocol_SetWearTestingSwitch
** Descriptions:            Wear testing switch
** input parameters:        p_nus�����ݽ���ͨ����p_data:���յ�����
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ             
** Created Date:            2016-11-19
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
static void Protocol_SetWearTestingSwitch(ble_nus_t *p_nus, uint8_t *p_data)
{
    uint8_t ble_ack[20] = {0x00, };
    
    ble_ack[0] = p_data[0]; //0xE2
    ble_ack[1] = 0x01;
    if(p_data[1] == 0x00)   //close
    {
        Wear_SetWearSwitchState();
        Wear_Close();   //WJ.20170301.��������Ҫ�ر�PWM
        system_info_s.wear_detect = 0x02;    
        App_SetSystemInfoSave();
        Wear_ClearWearFlag();     //WJ.20170714.�޸���������־��λ��     
    }
    else if(p_data[1] == 0x01)  //open
    {    
        Wear_SetWearSwitchState(); 
        Wear_Close();   //WJ.20170301.��������Ҫ�ر�PWM
        system_info_s.wear_detect = 0x01;
        App_SetSystemInfoSave();
        Wear_ClearWearFlag();     //WJ.20170714.�޸���������־��λ��
    }  
    ble_ack[2] = p_data[1];    
    //�������״̬
    ble_ack[3] = system_info_s.wear_detect;  
    Ble_SendDataViaBat(ble_ack, 20);       
}

/****************************************************************
** Function name:           Protocol_LaguageBroadcasts
** Descriptions:            ��������
** input parameters:        p_nus�����ݽ���ͨ����p_data:���յ�����
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ             
** Created Date:            2016-11-19
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
static void Protocol_LaguageBroadcasts(ble_nus_t *p_nus, uint8_t *p_data)
{

}

/****************************************************************
** Function name:           Protocol_SetRestOperation
** Descriptions:            ��ȡ��λ�Ĵ�����ַ
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ             
** Created Date:            2016-11-19
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
//static void Protocol_SetRestOperation(ble_nus_t *p_nus, uint8_t *p_data)
//{
//    uint8_t ble_ack[20] = {0x00, };   
//    
//    ble_ack[0] = p_data[0];
//    *((uint32_t *)&ble_ack[1]) = NRF_POWER->RESETREAS;
//        
//    Ble_SendDataViaBat(ble_ack, 20); 
//}

///****************************************************************
//** Function name:           RestoreFactory
//** Descriptions:            Clear data and restore factory Settings
//** input parameters:        ��
//** output parameters:       ��
//** Returned value:          ��
//** Created by:              WJ             
//** Created Date:            2016-11-19
//**----------------------------------------------------------------
//** Modified by:
//** Modified date:
//**----------------------------------------------------------------
//*****************************************************************/ 
//static uint8_t Protocol_RestoreFactorySet(void)
//{   
//    uint8_t ret = 1;
//    uint8_t language_temp = 0;

//    Movement_RestoreFactoryExit();
//    if(App_FlashEraseAllPage() != APP_FLASH_SUCCESS)
//    {
//        ret = 0x00;
//        //����ʧ�ܸ�λ
//        NVIC_SystemReset();    
//    }
//    else
//    {   
//        language_temp = system_info_s.language;
//        Config_SystemSwitchInit();    
//        Config_SystemInfoInit(); 
//        
//        //WJ.20171107.������ݣ����Բ���
//        system_info_s.language = language_temp;
//        
//        Daily_Init();
//        Daily_SeHeadtUsing();    
//        
//        All_StepCln();
//        All_StepSet(0);
//        Work_MeasrueCln();       
//        Ped_SetSportsInfo(0, 0, 0);
//        
//        Outsit_Init();
//        Alarm_InitAll(); 
//        
//        App_RoutineEventDelete();
//        App_AddEvent(APP_SHUTDOWN_EVENT);

//        system_flag_s.motor_type = 1;
//        Motor_AddEvent(1, 0, 1, 0);
//        
//        Storage_SaveSwitchInfo();
//        Storage_SaveSystemInfo();
////      pm_peers_delete();
//    }  
//    return ret;
//}

/****************************************************************
** Function name:           Protocol_DelAllData
** Descriptions:            Clear All data
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              YX             
** Created Date:            2018-10-23
*****************************************************************/ 
static uint8_t Protocol_DelAllData(void)
{   
    uint8_t ret = 1;
    uint8_t language_temp = 0;

    Movement_RestoreFactoryExit();
//    if(App_FlashEraseAllPage() != APP_FLASH_SUCCESS)
    App_FlashEraseCheck();
    if(App_FlashEraseAllData() != APP_FLASH_SUCCESS)
    {
        ret = 0x00;
        //����ʧ�ܸ�λ
        NVIC_SystemReset();    
    }
    else
    {   
        
        Daily_Init();
        Daily_SeHeadtUsing();    
        
        All_StepCln();
        All_StepSet(0);
        Work_MeasrueCln();       
        Ped_SetSportsInfo(0, 0, 0);
        
        App_RoutineEventDelete();
//        App_AddEvent(APP_SHUTDOWN_EVENT);

//        system_flag_s.motor_type = 1;
//        Motor_AddEvent(1, 0, 1, 0);
        
//        Storage_SaveSwitchInfo();
//        Storage_SaveSystemInfo();
//      pm_peers_delete();
    }  
    return ret;
}

/****************************************************************
** Function name:           Protocol_DelAllParam
** Descriptions:            Clear All Param
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              YX             
** Created Date:            2018-10-23
*****************************************************************/ 
static void Protocol_DelAllParam(void)
{   
    uint8_t language_temp = 0;
 
    language_temp = system_info_s.language;
    Config_SystemSwitchInit();    
    Config_SystemInfoInit(); 
    
    //WJ.20171107.������ݣ����Բ���
    system_info_s.language = language_temp;
    
    //�������
    Watch_Init();   
    //����ʱ
    Countdown_Init();
    //��������
    Outsit_Init();
    
    App_FlashEraseCheck();
    Alarm_ClearAll();
    App_FlashEraseCheck();
    Phys_ClearAll();
    
    //YX.20181123.����������輰ʱ�Բ������м��
    Config_SystemParamCheck();
    Config_SystemSwitchCheck();
    
    App_FlashEraseCheck();
    Storage_SaveSwitchInfo();
    App_FlashEraseCheck();
    Storage_SaveSystemInfo();

}

/****************************************************************
** Function name:           RestoreFactory
** Descriptions:            Clear data and restore factory Settings
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ             
** Created Date:            2016-11-19
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
static uint8_t Protocol_RestoreFactorySet(void)
{   
    uint8_t ret = 1;
    ret = Protocol_DelAllData();
    if(ret == 1)
    {
        Protocol_DelAllParam();
        system_flag_s.motor_type = 1;
        Motor_AddEvent(1, 0, 1, 0);
        App_AddEvent(APP_SHUTDOWN_EVENT);
    }
    return ret;
}

static void Protocol_RestoreFactory(ble_nus_t *p_nus, uint8_t *p_data)
{
    uint8_t ble_ack[20] = {0x00, };
    
    ble_ack[0] = p_data[0]; //0xF2
    ble_ack[1] = 1;
    ble_ack[2] = p_data[1];
    Ble_SendDataViaBat(ble_ack, 20);
    switch(p_data[1])
    {
        case 0x01://������ݣ��������,�ػ�
            Protocol_RestoreFactorySet();
            break;
        case 0x02://�������
            Protocol_DelAllData();
            break;
        case 0x03://�������
            Protocol_DelAllParam();
            break;
        case 0x04://�ػ�
            Motor_AddEvent(1, 0, 1, 0);
            App_AddEvent(APP_SHUTDOWN_EVENT);
            break;
        default:
            break;
    }
//    //ɾ���ֻ����ݣ��ָ���������
//    ble_ack[1] = Protocol_RestoreFactorySet();
}

/****************************************************************
** Function name:           SetRawDataType
** Descriptions:            Report the raw data type
** input parameters:        p_nus�����ݽ���ͨ����p_data:���յ�����
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ             
** Created Date:            2016-11-19
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
static void Protocol_SetRawDataType(ble_nus_t *p_nus, uint8_t *p_data)
{
    uint8_t ble_ack[20] = {0x00, };
    
    ble_ack[0] = p_data[0]; //0xF3
    if(p_data[1] == 0x00)   //The heart rate of the original data
    {
        system_flag_s.adc_type = 0;
    }
    else if(p_data[1] == 0x01)  //Step gauge the raw data
    {
        system_flag_s.adc_type = 1;
    }
    else if(p_data[1] == 0x02)  //read
    {
        
    } 
    else if(p_data[1] == 0x03)  //Upload the wear test raw data
    {
         system_flag_s.adc_type = 3;
    } 
    //��ǰ״̬
    ble_ack[1] = system_flag_s.adc_type;
    ble_ack[2] = p_data[1];
    Ble_SendDataViaBat(ble_ack, 20);  
}

/****************************************************************
** Function name:           Protocol_SwitchLanguages
** Descriptions:            ϵͳ��������
** input parameters:        p_nus�����ݽ���ͨ����p_data:���յ�����
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ             
** Created Date:            2016-11-19
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
static void Protocol_SwitchLanguages(ble_nus_t *p_nus, uint8_t *p_data)
{
    uint8_t ble_ack[20] = {0x00, };
    uint8_t system_language = 0;
    
    ble_ack[0] = p_data[0];   //0xF4    
    ble_ack[1] = 0x01;
    ble_ack[2] = 0x01;   
    ble_ack[3] = p_data[4];
    ble_ack[4] = p_data[1];
    
    if(p_data[1] == 0x00)
    {
        if(p_data[4] == 0x00)
        {
            system_language = 1;    
        }
        else if(p_data[4] == 0x01)
        {
            system_language = 2;    
        }
        else if(p_data[4] == 0x02)
        {           
            ble_ack[2] = system_info_s.language - 1;   
            ble_ack[4] = system_info_s.language;   
        }
        else
        {
            ble_ack[1] = 0x00; 
        }
    }
    else
    { 
        if( p_data[2] == 0x00 && p_data[1] <= 0x0B )
        {
            system_language = p_data[1];   
        }
        else if(p_data[2] != 0x00 && p_data[2] <= 0x0C )
        {
            system_language = p_data[2]; 
        }
        else
        {
            ble_ack[1] = 0x00;
        }
    }   
    
    if(system_language != 0 && system_info_s.language != system_language)
    {
        system_info_s.language = system_language;
        App_SetSystemInfoSave();
        //����������
        Gui_ThreadUpdateMian();
    }
    
    Ble_SendDataViaBat(ble_ack, 20);    
}

/****************************************************************
** Function name:           ReadBattery
** Descriptions:            read battery voltage and state
** input parameters:        p_nus�����ݽ���ͨ����p_data:���յ�����
** output parameters:       ��
** Returned value:          ��
** Created by:              SQH              
** Created Date:            2016-10-20
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
static void Protocol_ReadBattery(ble_nus_t *p_nus)
{
    uint8_t ble_ack[20] = {0x00, };
    
    ble_ack[0] = 0xA0;
    ble_ack[1] = system_flag_s.power_state;              //battery state
    ble_ack[2] = (Battery_GetAdcValue()>>4) & 0xFF;    //battery voltage
    ble_ack[3] = system_flag_s.sleep_enter;              //sleep state
    ble_ack[4] = Battery_GetGrade();                //battery level
    Ble_SendDataViaBat(ble_ack, 20);
}

/****************************************************************
** Function name:           Read_product_information
** Descriptions:            Equipment factory information
** input parameters:        p_nus�����ݽ���ͨ����p_data:���յ�����
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ             
** Created Date:            2016-11-19
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
static void Protocol_ReadProductInformation(ble_nus_t *p_nus, uint8_t *p_data)
{
    uint8_t ble_ack[20] = {0x00, };
    uint8_t ble_ack1[20] = {0x00, };
    uint32_t boot_vesion = 0;
    
    ble_ack[0] = p_data[0];     //0xFC
    ble_ack1[0] = p_data[0];
    if(p_data[1] == 0)  //get product information
    {
        ble_ack[1] = (uint8_t)(NRF_UICR->CUSTOMER[3]>>24);    //
        ble_ack[2] = (uint8_t)(NRF_UICR->CUSTOMER[3]>>16);    //
        ble_ack[3] = (uint8_t)(NRF_UICR->CUSTOMER[2]>>24);    //
        ble_ack[4] = (uint8_t)(NRF_UICR->CUSTOMER[2]>>16); 
        ble_ack[5] = (uint8_t)(NRF_UICR->CUSTOMER[2]>>8);
        ble_ack[6] = (uint8_t)(NRF_UICR->CUSTOMER[2]>>0);
        ble_ack[7] = (uint8_t)(NRF_UICR->CUSTOMER[5]>>0);      //Ӳ���汾
        ble_ack[8] = (uint8_t)(NRF_UICR->CUSTOMER[3]>>8);      //
        ble_ack[9] = (uint8_t)(NRF_UICR->CUSTOMER[3]>>0);      //
        ble_ack[10] = (uint8_t)(NRF_UICR->CUSTOMER[4]>>24);     //
        ble_ack[11] = (uint8_t)(NRF_UICR->CUSTOMER[4]>>16);
        ble_ack[12] = (uint8_t)(NRF_UICR->CUSTOMER[4]>>8); 
        ble_ack[13] = (uint8_t)(NRF_UICR->CUSTOMER[5]>>24);     //
        ble_ack[14] = (uint8_t)(NRF_UICR->CUSTOMER[5]>>16);
        ble_ack[15] = (uint8_t)(NRF_UICR->CUSTOMER[5]>>8);
        ble_ack[16] = (uint8_t)(NRF_UICR->CUSTOMER[4]>>24);      //
        ble_ack[17] = (uint8_t)(NRF_UICR->CUSTOMER[4]>>0);		
        if(ble_ack[17]==0) ble_ack[16]=0;
        ble_ack[18] = 0x01; 
        ble_ack[19] = 0x01;
        ble_ack1[1] = (uint8_t)(NRF_UICR->CUSTOMER[6]>>24);        //
        ble_ack1[2] = (uint8_t)(NRF_UICR->CUSTOMER[6]>>16);
        ble_ack1[3] = (uint8_t)(NRF_UICR->CUSTOMER[6]>>8);
        ble_ack1[4] = system_flag_s.ota_encrypt;
        
        //bootloader
        boot_vesion = Product_GetBootVesion();
        ble_ack1[5] = (uint8_t)(boot_vesion>>24);        //
        ble_ack1[6] = (uint8_t)(boot_vesion>>16);
        ble_ack1[7] = (uint8_t)(boot_vesion>>8);
        ble_ack1[8] = (uint8_t)(boot_vesion>>0);
        
        ble_ack1[18] = 0x02;
        ble_ack1[19] = 0x01; 
    }
    Ble_SendDataViaBat(ble_ack, 20);
    Ble_SendDataViaBat(ble_ack1, 20);
}

/****************************************************************
** Function name:           Protocol_ResetGsensor
** Descriptions:            ���ü��ٶ�
** input parameters:        p_nus�����ݽ���ͨ����p_data:���յ�����
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ             
** Created Date:            2016-11-19
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/
static void Protocol_ResetGsensor(ble_nus_t *p_nus, uint8_t *p_data)
{
    uint8_t ble_ack[20] = {0x00,};
    
    ble_ack[0] = p_data[0];     //0xFB
    ble_ack[1] = p_data[1];     //
    if(p_data[1] == 0x01)
    {     
        Acc_RestConfig();
        ble_ack[2] = 1;
    }
    else
    {
        ble_ack[2] = 0;
    }
    
    Ble_SendDataViaBat(ble_ack, 20); 
}


/****************************************************************
** Function name:           ReadFlashData
** Descriptions:            Read Flash data to app
** input parameters:        p_nus�����ݽ���ͨ����p_data:���յ�����
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ             
** Created Date:            2016-11-19
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/
static void Protocol_ReadFlashData(ble_nus_t *p_nus, uint8_t *p_data)
{
    //��ȡָ����ַ��flash����
    App_SetFlashDataRead();
    App_SetSendFlashDataType(FLASH_DATA_TYPE_OTHER);
    App_AddEvent(APP_SEND_FLASH_DATA_EVENT);
}

/****************************************************************
** Function name:           Protocol_TestFunctional
** Descriptions:            ��������ָ��
** input parameters:        p_nus�����ݽ���ͨ����p_data:���յ�����
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ             
** Created Date:            2016-11-19
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
static void Protocol_TestFunctional(ble_nus_t *p_nus, uint8_t *p_data)
{
    static uint8_t lcd_open_flag = 0;
    uint8_t ble_ack[20] = {0x00, };
    //���ܲ���
    
    if(system_flag_s.ble_connect != 1)
    {
        system_flag_s.sign_test = 0x00;
        system_flag_s.step_gauge_report = 0x00;
        return ;
    }
    
    ble_ack[0] = p_data[0];
    ble_ack[1] = p_data[1];
    
    ble_ack[2] = 0x01; 
    if(p_data[1] == 0x00)
    {           
        system_flag_s.sign_test = 0x00;
        system_flag_s.step_gauge_report = 0x00; 
    }
    else
    {
        system_flag_s.sign_test = 0x01;
    }

    if(p_data[1]&0x08) 
    {
        Motor_PowerOn();
    }
    else 
    {
        Motor_PowerOFF();
    }          

    if(p_data[1]&0x10)
    {
        lcd_open_flag = 1;
        Gui_ThreadEventTaskWakeUp(CPU_EVENT_TASK_FONTTEST); 
    }
    else if( ( (p_data[1] | 0xEF ) & 0x10) == 0x00)
    {
        if(lcd_open_flag == 1)
        {
            Gui_ThreadEventTaskWakeUp(CPU_EVENT_TASK_ALL_EXIT); 
        }
        lcd_open_flag = 0;
    }                     

    if(p_data[1]&0x20)
    {                   
        system_flag_s.step_gauge_report = 1; //ÿ���붼Ҫ�ϴ�����
    } 
    else 
    {
        system_flag_s.step_gauge_report = 0;
    }

    if(p_data[1]&0x40)      //���ʲ���                                    
    {   
        system_flag_s.sign_test = 0x01;
        App_StopMeasuringHandler(TEST_CLOSE_ALL);
        system_flag_s.measuring_status = APP_HEARTING_STATUS;
        system_flag_s.heart_led_status = true;
        Heart_Open();
    }
    else if(p_data[1]&0x04)
    {
        App_StopMeasuringHandler(TEST_CLOSE_ALL);
        system_flag_s.heart_led_status = true;
        Led_OpenByLevel(2);
    }
    else
    {
        App_StopMeasuringHandler(TEST_CLOSE_ALL);
        system_flag_s.heart_led_status = false;
        Led_CloseAll();
    }

    if(p_data[1]&0x80) //ȫ��                                        
    {    
        Motor_PowerOFF();
        system_flag_s.sign_test = 0x00;
        system_flag_s.step_gauge_report = 0x00; 
        App_StopMeasuringHandler(TEST_CLOSE_ALL);
        Gui_ThreadEventTaskWakeUp(CPU_EVENT_TASK_ALL_EXIT);
        Led_CloseAll();
        system_flag_s.heart_led_status = false;   
        lcd_open_flag = 0;
        
        Ble_DisconnectBle();  
        Protocol_RestoreFactorySet();         
//        Gui_ThreadEventTaskWakeUp(CPU_EVENT_TASK_ALL_EXIT);
    }    
    
    Ble_SendDataViaBat(ble_ack, 20);   
}

/****************************************************************
** Function name:           Protocol_SetBleName
** Descriptions:            ������������
** input parameters:        p_nus�����ݽ���ͨ����p_data:���յ�����
** output parameters:       ��
** Returned value:          ��
** Created by:              YX             
** Created Date:            2017-07-20
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
static void Protocol_SetBleName(ble_nus_t *p_nus, uint8_t *p_data)
{
    uint8_t ble_ack[20] = {0x00,};
    uint8_t i = 0;
    
    ble_ack[0] = p_data[0];     //0x84
    ble_ack[1] = p_data[1];     //
    if(p_data[1] == 0x01)
    {//����
        for(i = 2; i <= 9; i++)
        {
            ble_ack[i+1] = p_data[i];
        }
        if(ble_ack[3] != 0x00)
        {
            ble_ack[2] = 1;
            memset(ble_name, 0, sizeof(ble_name));
            memcpy(ble_name, (uint8_t *)&ble_ack[3], 8);
            App_AddEvent(APP_DEV_SETBLENAME_EVENT);
        }
        else
        {//ʧ��
            ble_ack[2] = 0;
        }
    }
    else
    {
        ble_ack[2] = 0;
    }
    
    Ble_SendDataViaBat(ble_ack, 20); 
}

#if UI_DEBUG 

/****************************************************************
** Function name:			Protocol_SetUiInfo
** Descriptions:            ����UI����
** input parameters:        p_nus�����ݽ���ͨ����p_data:���յ�����
** output parameters:       ��
** Returned value:          ��
** Created by:				YX             
** Created Date:           	2017-07-20
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
static void Protocol_SetUiInfo(ble_nus_t *p_nus, uint8_t *p_data)
{
    uint8_t ble_ack[20] = {0x00,};
    
    ble_ack[0] = p_data[0];     //0xCF
    ble_ack[1] = p_data[1];     //
    if(p_data[1] == 0x01)
    {
        /* ���� */
        ble_ack[2] = 1;
    }
    else if(p_data[1] == 0x00)
    {
        /* ���� */
        ble_ack[2] = 1;
    }
    
    Ble_SendDataViaBat(ble_ack, 20); 
}
#endif

/****************************************************************
** Function name:           Protocol_DisplayUiFunction
** Descriptions:            UIչʾ����
** input parameters:        p_nus�����ݽ���ͨ����p_data:���յ�����
** output parameters:       ��
** Returned value:          ��
** Created by:              YX             
** Created Date:            2017-07-20
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
static void Protocol_DisplayUiFunction(ble_nus_t *p_nus, uint8_t *p_data)
{
    //FD 95 26 51 EF
    uint8_t ble_ack[20] = {0x00,};
//    uint8_t i = 0;
//    
//    ble_ack[0] = p_data[0];     //0xFD
    if(p_data[1] == 0x95 && p_data[2] == 0x26 && p_data[3] == 0x51 && p_data[4] == 0xEF)
    {
        /* ������ȷ������UIչʾ���� */
        Gui_SetTraverseUiState(1);
    }
    else
    {
        if(p_data[1] == 0x55 && p_data[2] == 0x01 
           && p_data[3] == 0x73 && p_data[4] == 0x40)
        {
            ble_ack[5] = Gui_UiCrcCheck(DATA_CRC_BUF, ALL_DATA_SIZE, (void *)&ble_ack[6]);
            memcpy(&ble_ack[1], &p_data[1], 4);
            Ble_SendDataViaBat(ble_ack, 20); 
        }
    }
    
//    Ble_SendDataViaBat(ble_ack, 20); 
}

/****************************************************************
** Function name:           Protocol_DisplayUiFunction
** Descriptions:            Ů�Թ��ܲ�������
** input parameters:        p_nus�����ݽ���ͨ����p_data:���յ�����
** output parameters:       ��
** Returned value:          ��
** Created by:              YX             
** Created Date:            2017-10-26
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
static void Protocol_SetPhysParam(ble_nus_t *p_nus, uint8_t *p_data)
{
    uint8_t ble_ack[20] = {0x00,};
    
    Phys_ParsingProtocol(p_data, ble_ack);
    
    Ble_SendDataViaBat(ble_ack, 20);     
}

/****************************************************************
** Function name:           Protocol_BpFunctionSet
** Descriptions:            �����ر�Ѫѹ����ģ��
** input parameters:        p_nus�����ݽ���ͨ����p_data:���յ�����
** output parameters:       ��
** Returned value:          ��
** Created by:              YX             
** Created Date:            2017-10-26
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
static void Protocol_BpFunctionSet(ble_nus_t *p_nus, uint8_t *p_data)
{
    uint8_t ble_ack[20] = {0x00,};
    
    ble_ack[0] = 0x92;
    ble_ack[1] = p_data[1];
    
    if(p_data[2] == 0x55 && p_data[3] == 0x01 
      && p_data[4] == 0x73 && p_data[5] == 0x40)
    {
        ble_ack[2] = 0x55; 
        ble_ack[3] = 0x01; 
        ble_ack[4] = 0x73;
        ble_ack[5] = 0x40;
    
        if(ble_ack[1] == 0x01 || ble_ack[1] == 0x02)
        {
            if(system_flag_s.bp_flag != ble_ack[1])
            {
                system_flag_s.bp_flag = ble_ack[1];
                App_AddEvent(APP_DEV_SET_BP_FUNCTION_EVENT);
            }
        }
        ble_ack[6] = system_flag_s.bp_flag;               
    }
    
    Ble_SendDataViaBat(ble_ack, 20);     
}


/****************************************************************
** Function name:           Protocol_SitFunctionSet
** Descriptions:            �����رվ�������ģ��
** input parameters:        p_nus�����ݽ���ͨ����p_data:���յ�����
** output parameters:       ��
** Returned value:          ��
** Created by:              YX             
** Created Date:            2017-10-26
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
static void Protocol_SitFunctionSet(ble_nus_t *p_nus, uint8_t *p_data)
{
    uint8_t ble_ack[20] = {0x00,};
    uint8_t result = 0;
    
    ble_ack[0] = 0x94;
    ble_ack[1] = p_data[1];
    ble_ack[2] = p_data[2];
    
    if(p_data[3] == 0x55 && p_data[4] == 0x01 
      && p_data[5] == 0x73 && p_data[6] == 0x40)
    {
        ble_ack[3] = 0x55; 
        ble_ack[4] = 0x01; 
        ble_ack[5] = 0x73;
        ble_ack[6] = 0x40;
    
        if(ble_ack[2] == 0x01 || ble_ack[2] == 0x02)
        {
            if(ble_ack[1] == 0x01)//����
            {
                if(system_flag_s.sit_flag != ble_ack[2])
                {
                    system_flag_s.sit_flag = ble_ack[2];
                    App_AddEvent(APP_DEV_SET_SIT_FUNCTION_EVENT);
                    Outsit_Init();
                    result = ble_ack[2];
                }   
            }
        }
        else if(ble_ack[2] == 0x03)
        {
            result = system_flag_s.sit_flag;
        }

        ble_ack[7] = result;               
    }
    
    Ble_SendDataViaBat(ble_ack, 20);     
}

/****************************************************************
** Function name:           Protocol_BatDataHandler
** Descriptions:            �������ݴ�����
** input parameters:        p_nus�����ݽ���ͨ����p_data:���յ����ݣ�length�����ݳ���
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2016-09-12
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Protocol_BatDataHandler(ble_nus_t *p_nus, uint8_t *p_data1, uint16_t length)
{
    uint8_t p_data[20] = {0x00, };
    uint8_t ble_ack[20] = {0xA1, 0x00}; 
    
    memcpy(p_data, p_data1, length);
    
#if UI_DEBUG     
    if(system_flag_s.update_uimode == 1)
    {
        /* ���ڽ���UI���µ����� */
        Protocol_ReceiveUpdateUiData(p_nus, p_data);
        return;
    }    
#endif
	
	if(p_data[0] == 0xA1)
    {
        Protocol_PasswordPair(p_nus, p_data);
        return;
    }
    
    if(system_flag_s.code_passed != true)
    {
        //���벻ͨ��,�ظ����벻ͨ��       
        ble_ack[0] = 0xA1;          
        ble_ack[1] = 0x00;    
        ble_ack[2] = 0x00;  
        ble_ack[3] = 0x00;    
        ble_ack[4] = (VEEPOO_DEVICE_ID>>8) & 0xFF;
        ble_ack[5] = VEEPOO_DEVICE_ID & 0xFF;
        ble_ack[6] = VERSION_M & 0xFF;
        ble_ack[7] = VERSION_H & 0xFF;
        ble_ack[8] = VERSION_L & 0xFF;   
        ble_ack[9] = VERSION_T & 0xFF;   
        ble_ack[10] = 0;
        ble_ack[11] = 0x01; 
        Ble_SendDataViaBat(ble_ack, 20);   
        return;
    }
    //APP�����뾭�ֻ���׼ͨ��  ����  ������صĲ���
    switch(p_data[0])
    {
        case 0x80:
            Protocol_BloodOxygen(p_nus, p_data);
            break;
        case 0x81:
            Protocol_Fatigue(p_nus, p_data);
            break;
        case 0x84:
            //������������
            Protocol_SetBleName(p_nus, p_data);
            break;
        case 0x85:
            Protocol_SetPhysParam(p_nus, p_data);
            break;
        case 0x90:  
            //����Ѫѹ���
            Protocol_BloodPressOnOFF(p_nus, p_data);
            break;
        case 0x91:  
            //�趨Ѫѹģʽ
            Protocol_BloodPressureSet(p_nus, p_data);
            break;        
        case 0x92:  
            //�����ر�Ѫѹ����ģ��
            Protocol_BpFunctionSet(p_nus, p_data);
            break;   
         case 0x94:  
            //�����رվ�������ģ��
            Protocol_SitFunctionSet(p_nus, p_data);
            break;  
        
        case 0xA0:  
            //��ȡ���״̬, Read the battery status
            Protocol_ReadBattery(p_nus);
            break;                        
        case 0xA1:  
            //��׼����, Password Setting
            Protocol_PasswordPair(p_nus, p_data);
            break;              
        case 0xA2: 
            //Before the firmware update for equipment information
            Protocol_ReadInfoByOAD(p_nus, p_data);
            break;
        case 0xA3:  
            //Synchronization of personal information
            Protocol_SynchronizationUserInfo(p_nus, p_data);
            break;
        case 0xA4: 
            Protocol_DeviceInfo(p_nus, p_data);
            break;
        case 0xA5: 
            //set device time
            Protocol_SetDeviceTime(p_nus, p_data);
            break;
        case 0xA6:  
            //read device time
            Protocol_ReadDeviceTime(p_nus, p_data);
            break;
        case 0xA7:  
            //Read the function of the equipment support
            Protocol_ReadDeviceFunction(p_nus, p_data);
            break;
        case 0xA8:  
            //Read the meter data
            Protocol_ReadWalkCnt(p_nus, p_data);
            break;
        case 0xA9:  
            //Read movement data
            Protocol_ReadSportData(p_nus, p_data);
            break;
        case 0xAA:  
            //bright screen when raising hands
            Protocol_GestureSwitch(p_nus, p_data);
            break;
        case 0xAB:  
            //The alarm clock Settings
            Protocol_AlarmClockSet(p_nus, p_data);
            break;
        case 0xAC:  
            //Set up the heart rate alarm range
            Protocol_SetHeartAlarmRage(p_nus, p_data); 
            break;
        case 0xAD: 
            //ANCS notify control
            Protocol_AncsNotiryControl(p_nus, p_data);
            break;
        case 0xAE: 
            //The lost
            Protocol_FindPhoneSet(p_nus, p_data);
            break;
        case 0xAF:  
            //Bluetooth disconnect
            Protocol_SetBluetoothDisconnect(p_nus, p_data);           
            break;

        case 0xB1:  
            //��Ļ��������    
            Protocol_OledSet(p_nus, p_data);         
            break;
        case 0xB2:  
            //����ʱ
            Protocol_CountDown(p_nus, p_data);  
            break;
        case 0xB5: 
            //�ֻ������ֻ�����
            Protocol_FindPhone(p_nus, p_data);            
            break;
        case 0xB6:  
            //���๦��
            Protocol_Camera(p_nus, p_data);            
            break;
        case 0xB8: 
            //���ؿ���
            Protocol_SwitchState(p_nus, p_data);
            break;           
        case 0xB9:
            Protocol_AlarmConfig(p_nus, p_data);
            break;
        case 0xBC:  
            // IOSϵͳANCS����, 
            Protocol_MatchingAndBinding(p_nus, p_data);
            break;  
        
        case 0xC1:
            Protocol_Android_NotifySet(p_nus, p_data);
            break;              
        case 0xC2:
            Protocol_Android_NotifyEvent(p_nus, p_data);
            break;

#if UI_DEBUG        
        case 0xC4: 
            /* Set the UI update mode */
            Protocol_UpdateUiMode(p_nus, p_data);
            break;
        case 0xC5: 
            /* erase the UI data */
            Protocol_EraseUiData(p_nus, p_data);
            break;
        case 0xC6:  
            /* read the UI data */
            Protocol_ReadUiData(p_nus, p_data);
            break;
#endif        
         case 0xC7: 
             /* set the UI style */
            Protocol_SetUiStyle(p_nus, p_data);
            break; 
#if UI_DEBUG
        case 0xCF:
            //UI��������
            Protocol_SetUiInfo(p_nus, p_data);
            break; 
#endif         
        case 0xCB:  
            // Android 
            Protocol_AndroidNotify(p_nus, p_data);
            break;                  
        
        case 0xD0:  
            //read current heart
            Protocol_ReadCurHeart(p_data);
            break;
        case 0xD1:  
            //read all day heart, means send 5 minute data that stored in flash
            Protocol_SendDailyData(p_nus, p_data);
            break;
        case 0xD3:
            //��ȡ�˶�ģʽ����CRC
            Protocol_GetMovementPatternsDataCRC(p_nus, p_data);
            break;
        case 0xD4:
            //��ȡָ������˶�����
            Protocol_GetMovementPatternsData(p_nus, p_data);
            break;
        case 0xD5:
            //APP�����˶�ģʽ
            Protocol_OpreateMovementPatternsData(p_nus, p_data);
            break;
        case 0xD6:
            //�����˶�ģʽ��ͣ��ʱʱ��
            Protocol_SetMovementPatternsPuseTimeOut(p_nus, p_data);
            break;
        case 0xD7:  
            //Get drinking data
            Protocol_ReadDrinkingData(p_nus, p_data);
            break;
        
        case 0xD8:  
            Protocol_GetAllMotionData(p_nus, p_data);
            break;
            
        case 0xE0:  
            //Send sleep data
            Protocol_SendDailySleepData(p_nus, p_data);
            break;
        case 0xE1:  
            //Set the sedentary remind time period
            Protocol_SetSedentaryTime(p_nus, p_data);
            break;
        case 0xE2:  
            //Set wear detection switch
            Protocol_SetWearTestingSwitch(p_nus, p_data);
            break;          
        case 0xE5:  
            //���Բ����ظ�
            Protocol_LaguageBroadcasts(p_nus, p_data);
            break;
        
        case 0xF0:  
            //read flash data
            Protocol_ReadFlashData(p_nus, p_data);
            break;
        case 0xF1:  
            //����ָ��1
            Protocol_TestFunctional(p_nus, p_data);
            break;
        case 0xF2:   
            //Clear data and restore factory Settings    
            Protocol_RestoreFactory(p_nus, p_data);
            break;
        case 0xF3:   
            //Report the raw data type   
            Protocol_SetRawDataType(p_nus, p_data);
            break;
        case 0xF4:   
            //��Ӣ���л�  
            Protocol_SwitchLanguages(p_nus, p_data);
            break;
         case 0xFB:	
            //���ٶ�����
            Protocol_ResetGsensor(p_nus, p_data);                
            break;	            
        case 0xFC:	
            //Product information
            Protocol_ReadProductInformation(p_nus, p_data);                
            break;		
        
        case 0xFD:  
            //UIչʾ����
            Protocol_DisplayUiFunction(p_nus, p_data);
            break;            
        case 0xFF:  
            if(p_data[1] == 0x55 && p_data[2] == 0x01 
              && p_data[3] == 0x73 && p_data[4] == 0x40 && p_data[5] == 0x00)
            {
                nrf_delay_ms(500);
                NVIC_SystemReset();     //reset
            }
            break;
        default:                   
            break;
    }
    
}

/****************************************************************
** Function name:           Protocol_ClkDataHandler
** Descriptions:            �������ݴ�����
** input parameters:        p_nus�����ݽ���ͨ����p_data:���յ�����
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2016-09-12
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/
void Protocol_ClkDataHandler(ble_clk_t *p_clk, uint8_t *p_data, uint16_t length)
{	 
}

/****************************************************************
** Function name:           Ble_SendDataViaBat
** Descriptions:            ͨ��Bat����������
** input parameters:        p_nus�����ݽ���ͨ����p_data:���յ�����
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2016-09-12
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/
void Ble_SendDataViaBat(uint8_t *data_addr, uint8_t data_size)
{
    if(system_flag_s.ble_connect != 0x01)
    {
        return;
    }
    ble_nus_string_send(Ble_GetBatSrvAddr(), data_addr, data_size); 
}

/****************************************************************
** Function name:           Protocol_ClkDataHandler
** Descriptions:            ͨ��Clk����������
** input parameters:        p_nus�����ݽ���ͨ����p_data:���յ�����
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2016-09-12
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/
void Ble_SendDataViaClk(uint8_t *data_addr, uint8_t data_size)
{
    if( system_flag_s.ble_connect != 0x01) 
    {
        return;
    }
    ble_clk_sending(Ble_GetClkSrvAddr(), data_addr, data_size); 
}

