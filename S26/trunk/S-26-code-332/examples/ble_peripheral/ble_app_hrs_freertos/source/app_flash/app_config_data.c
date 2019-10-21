/*************************************Copyright(c)***************************************************
**                                     Veepoo Tech
**                  
**                               COPYRIGHT(C)  2016-2019
**--------------------------------------FileInfo----------------------------------------------------
** FileName:                           app_config_data.c
** Last Modify Data:                    2017-08-04
** Last SYS_VERSION:                    XXXXXXXXX
** Descriptions:                     
**---------------------------------------------------------------------------------------------------
** CreatedBy:                              GT
** CreateData:                          2017-06-xx
** SystemVersion:             
** Description:                      flash��������
**---------------------------------------------------------------------------------------------------
** Modify By:                       
** Modify Data:
** System Version:
** Modify Description:    

******************************************************************************************************/
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "app_config_data.h"
#include "flash_common.h"
#include "app_error.h"
#include "ble_thread.h"
#include "api_flash.h"
#include "movement_patterns.h"

/*******************************************************************************************************
** Function name:            App_GetMessageEnable
** Descriptions:             �õ���Ϣʹ�� 
** input parameter:          p_message:����ָ��
** Returned Value:           NRF_SUCCESS
** Created  by:              ����
** Created  Datas:           2017-08-04
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
*********************************************************************************************************/
uint32_t App_GetMessageEnable(app_message_enable_t* p_message)
{
    uint32_t message_data[2] = {0};
    uint8_t* p_message_data = (uint8_t*)message_data;
    
    // ��FLASH�ж�ȡ����
    Flash_CommonRead(message_data,APP_CONFIG_BASE_ADDR,sizeof(message_data));
    
    // У���ȡͷ����־
    if(0 == memcmp(&p_message_data[4],"Mesg",4))
    {
        //�ڴ濽������
        memcpy(p_message,p_message_data,sizeof(app_message_enable_t));
    }
    else
    {
        memset(p_message,0,sizeof(app_message_enable_t));
        p_message->call = 1;
    }
    
    return NRF_SUCCESS;
}

/*******************************************************************************************************
** Function name:            App_UpdateMessageEnable
** Descriptions:             ������Ϣʹ��
** input parameter:          p_message:��Ϣ����ָ��
** Returned Value:           NRF_SUCCESS
** Created  by:              ����
** Created  Datas:           2017-08-04
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
*********************************************************************************************************/
uint32_t App_UpdateMessageEnable(const app_message_enable_t* p_message)
{
    uint32_t message_data[2] = {0};
    uint8_t* p_message_data = (uint8_t*)message_data;
    
    memcpy(&p_message_data[0],p_message,sizeof(app_message_enable_t));
    memcpy(&p_message_data[4],"Mesg",4);
    
    return Flash_CommonStore((uint32_t*)APP_CONFIG_BASE_ADDR,message_data, 2);
}

/*******************************************************************************************************
** Function name:            App_GetSedentaryConfig
** Descriptions:             ȡ�þ�������
** input parameter:          p_config:��������ָ��
** Returned Value:           NRF_SUCCESS
** Created  by:              ����
** Created  Datas:           2017-08-04
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
*********************************************************************************************************/
uint32_t App_GetSedentaryConfig(sedentary_config_t* p_config)
{
    uint32_t sedentary_param[3] = {0};
    uint8_t* p_sedentary_param = (uint8_t*)sedentary_param;
    
    // ��FLASH�ж�ȡ����,ƫ������Ҫ�ο��ĵ�����
    Flash_CommonRead(sedentary_param,APP_CONFIG_BASE_ADDR+0x08,sizeof(sedentary_param));
    
    // У���ȡͷ����־ 
    if(0 == memcmp(&p_sedentary_param[6],"Sit",3))
    {
        memcpy(p_config,p_sedentary_param,sizeof(sedentary_config_t));
    }
    else
    {
        p_config->start_minute = 0x00;
        p_config->start_hour = 0x08;
        p_config->end_minute = 0x00;
        p_config->end_hour = 0x12;
        p_config->internal = 0x3C;
        p_config->enable = 0x00;
    }
    
    return NRF_SUCCESS;
}

/*******************************************************************************************************
** Function name:            App_UpdateSedentaryConfig
** Descriptions:             ���¾�������
** input parameter:          p_config:��������ָ��
** Returned Value:           NRF_SUCCESS
** Created  by:              ����
** Created  Datas:           2017-08-04
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
*********************************************************************************************************/
uint32_t App_UpdateSedentaryConfig(const sedentary_config_t* p_config)
{
    uint32_t sedentary_param[3] = {0};
    uint8_t* p_sedentary_param = (uint8_t*)sedentary_param;
    
    memcpy(p_sedentary_param,p_config,sizeof(sedentary_config_t));
    memcpy(&p_sedentary_param[6],"Sit",3);
    
    return Flash_CommonStore((uint32_t*)(APP_CONFIG_BASE_ADDR+0x08),sedentary_param, 3);
}

/*******************************************************************************************************
** Function name:            App_GetDevConfig
** Descriptions:             ȡ���豸����
** input parameter:          p_dev:�豸����ָ��
** Returned Value:           NRF_SUCCESS
** Created  by:              ����
** Created  Datas:           2017-08-04
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
*********************************************************************************************************/
uint32_t App_GetDevConfig(dev_config_t* p_dev)
{
    uint32_t dev_param[2] = {0};
    uint8_t* p_dev_param = (uint8_t*)dev_param;
    
    // ��FLASH�ж�ȡ����,ƫ������Ҫ�ο��ĵ����� 
    Flash_CommonRead(dev_param,APP_CONFIG_BASE_ADDR+0x14,sizeof(dev_param));
    
    // У���ȡͷ����־ 
    if(0 == memcmp(p_dev_param,"Conf",4))
    {
        memcpy(p_dev,&p_dev_param[4],sizeof(dev_config_t));
    }
    else
    {
        p_dev->wear_detect_enable = 0x01;
        p_dev->am_pm_enable = 0x00;
        p_dev->english_style_enable = 0x00;
        p_dev->heart_detect_enable = 0x01;
        p_dev->blood_detect_enable = 0x00;
        p_dev->horizontal_enable = 0x00;
        p_dev->watchtimer_enable = 0;
        p_dev->low_bo_alert_enable = 1;
		p_dev->find_phone_enable = 0;
    }
    
    return NRF_SUCCESS;
}

/*******************************************************************************************************
** Function name:            App_UpdateDevConfig
** Descriptions:             �����豸����
** input parameter:          p_dev:�豸����ָ��
** Returned Value:           NRF_SUCCESS
** Created  by:              ����
** Created  Datas:           2017-08-04
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
*********************************************************************************************************/
uint32_t App_UpdateDevConfig(const dev_config_t* p_dev)
{
    uint32_t dev_param[2] = {0};
    uint8_t* p_dev_param = (uint8_t*)dev_param;
    
    memcpy(p_dev_param,"Conf",4);
    memcpy(&p_dev_param[4],p_dev,2);
    
    return Flash_CommonStore((uint32_t*)(APP_CONFIG_BASE_ADDR+0x14),dev_param, 2);
}

/*******************************************************************************************************
** Function name:            App_GetBloodMode
** Descriptions:             �õ�Ѫѹģʽ
** input parameter:          p_dev:��������ָ��
** Returned Value:           NRF_SUCCESS
** Created  by:              ����
** Created  Datas:           2017-08-04
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
*********************************************************************************************************/
uint32_t App_GetBloodMode(bp_config_t* p_config)
{
    uint32_t blood_param[3] = {0};
    uint8_t* p_blood_param = (uint8_t*)blood_param;
    
    /* ��FLASH�ж�ȡ����,ƫ������Ҫ�ο��ĵ����� */
    Flash_CommonRead(blood_param,APP_CONFIG_BASE_ADDR+0x1C,sizeof(blood_param));
    
    /* У���ȡͷ����־ */
    if(0 == memcmp(&p_blood_param[6],"Blood",5))
    {
        /* �����ȡУ����ȷ */
        p_config->private_low = p_blood_param[2];
        p_config->private_high = p_blood_param[3];
        p_config->mode = (BLOOD_MODE_E)p_blood_param[4];
    }
    else 
    {
        p_config->private_high = 120;
        p_config->private_low = 80;
        p_config->mode = (BLOOD_MODE_E)0x00;
    }
    
    return NRF_SUCCESS;
}

/*******************************************************************************************************
** Function name:            App_SaveBloodMode
** Descriptions:             �洢Ѫѹģʽ
** input parameter:          p_dev:Ѫѹģʽ
** Returned Value:           NRF_SUCCESS
** Created  by:              ����
** Created  Datas:           2017-08-04
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
*********************************************************************************************************/
uint32_t App_SaveBloodMode(const bp_config_t* p_config)
{
    uint32_t blood_param[3] = {0};
    uint8_t* p_blood_param = (uint8_t*)blood_param;
    
    /* ������ֵ */
    p_blood_param[2] = p_config->private_low;
    p_blood_param[3] = p_config->private_high;
    p_blood_param[4] = p_config->mode;
    memcpy(&p_blood_param[6],"Blood",5);
    
    return Flash_CommonStore((uint32_t*)(APP_CONFIG_BASE_ADDR+0x1C),blood_param, 3);
}

/*******************************************************************************************************
** Function name:            App_GetPersonInfomation
** Descriptions:             �õ�������Ϣ
** input parameter:          p_dev:������Ϣ����ָ��
** Returned Value:           NRF_SUCCESS
** Created  by:              ���� 
** Created  Datas:           2017-08-04
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
*********************************************************************************************************/
uint32_t App_GetPersonInfomation(person_infomation_t* p_infomation)
{
    uint32_t person_param[2] = {0};
    uint8_t* p_person_param = (uint8_t*)person_param;
    
    /* ��FLASH�ж�ȡ����,ƫ������Ҫ�ο��ĵ����� */
    Flash_CommonRead(person_param,APP_CONFIG_BASE_ADDR+0x38,sizeof(person_param));
    
    /* У���ȡͷ����־ */
    if(0 == memcmp(&p_person_param[4],"User",4))
    {
        /* �����ȡУ����ȷ */
        p_infomation->gender = (PERSON_GENDER_E)p_person_param[0];
        p_infomation->age = p_person_param[1];
        p_infomation->weight = p_person_param[2];
        p_infomation->height = p_person_param[3];
    }
    else
    {
        /* �����ȡУ����ȷ */
        p_infomation->gender = (PERSON_GENDER_E)0x01;
        p_infomation->age = 31;
        p_infomation->weight = 60;
        p_infomation->height = 170;
    }
    
    return NRF_SUCCESS;
}

/*******************************************************************************************************
** Function name:            App_UpdatePersonInfomation
** Descriptions:             ���¸�����Ϣ����
** input parameter:          p_dev:������Ϣ����ָ��
** Returned Value:           NRF_SUCCESS
** Created  by:              ����
** Created  Datas:           2017-08-04
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
*********************************************************************************************************/
uint32_t App_UpdatePersonInfomation(const person_infomation_t* p_infomation)
{
    uint32_t person_param[2] = {0};
    uint8_t* p_person_param = (uint8_t*)person_param;
    
    /* ������ֵ */
    p_person_param[0] = p_infomation->gender;
    p_person_param[1] = p_infomation->age;
    p_person_param[2] = p_infomation->weight;
    p_person_param[3] = p_infomation->height;
    
    memcpy(&p_person_param[4],"User",4);
    
    return Flash_CommonStore((uint32_t*)(APP_CONFIG_BASE_ADDR+0x38),person_param, 2);
}

/*******************************************************************************************************
** Function name:            App_GetSportCount
** Descriptions:             ȡ���˶��������
** input parameter:          p_dev:����ָ��
** Returned Value:           NRF_SUCCESS
** Created  by:              ����
** Created  Datas:           2017-08-04
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
*********************************************************************************************************/
uint32_t App_GetSportCount(uint16_t* p_config)
{
    uint32_t sport_param[2] = {0};
    uint8_t* p_sport_param = (uint8_t*)sport_param;
    
    Flash_CommonRead(sport_param,APP_CONFIG_BASE_ADDR+0x28,sizeof(sport_param));
    
    /* У���ȡͷ����־ */
    if(0 == memcmp(&p_sport_param[4],"Step",4))
    {
        /* �����ȡУ����ȷ */
        *p_config = (p_sport_param[1]<<8)|(p_sport_param[0]);
		p_flash_param->Remind_flag = p_sport_param[2];
    }
    else
    {
        *p_config = 8000;
		p_flash_param->Remind_flag = 0;//Ĭ��ֵ
    }
    
    return NRF_SUCCESS;
}

/*******************************************************************************************************
** Function name:            App_UpdateSportCount
** Descriptions:             �����˶��������
** input parameter:          p_dev:����ָ��
** Returned Value:           NRF_SUCCESS
** Created  by:              ����
** Created  Datas:           2017-08-04
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
*********************************************************************************************************/
uint32_t App_UpdateSportCount(const uint16_t count)
{
    uint32_t sport_param[2] = {0};
    uint8_t* p_sport_param = (uint8_t*)sport_param;
    
    /* ������ֵ */
    p_sport_param[0] = count&0xFF;
    p_sport_param[1] = (count>>8)&0xFF;
    p_sport_param[2] = p_flash_param->Remind_flag;
    
    memcpy(&p_sport_param[4],"Step",4);
    
    return Flash_CommonStore((uint32_t*)(APP_CONFIG_BASE_ADDR+0x28),sport_param, 2);
}

/*******************************************************************************************************
** Function name:            App_GetSleepTime
** Descriptions:             ȡ��˯��ʱ��
** input parameter:          p_dev:����ָ��
** Returned Value:           NRF_SUCCESS
** Created  by:              ����
** Created  Datas:           2017-08-04
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
*********************************************************************************************************/
uint32_t App_GetSleepTime(float* p_time)
{
    uint32_t sleep_param[2] = {0};
    uint8_t* p_sleep_param = (uint8_t*)sleep_param;
    
    Flash_CommonRead(sleep_param,APP_CONFIG_BASE_ADDR+0x30,sizeof(sleep_param));
    
    /* У���ȡͷ����־ */
    if(0 == memcmp(&p_sleep_param[4],"Slep",4))
    {
        /* �����ȡУ����ȷ */
        *p_time = p_sleep_param[1]+p_sleep_param[0]/60;
    }
    else
    {
        *p_time = 8.0;
    }
    
    return NRF_SUCCESS;
}

/*******************************************************************************************************
** Function name:            App_SaveSleepTime
** Descriptions:             �洢˯��ʱ��
** input parameter:          time��˯��ʱ��
** Returned Value:           NRF_SUCCESS
** Created  by:              ����
** Created  Datas:           2017-08-04
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
*********************************************************************************************************/
uint32_t App_SaveSleepTime(const float time)
{
    uint32_t sleep_param[2] = {0};
    uint8_t* p_sleep_param = (uint8_t*)sleep_param;
    
    /* ������ֵ */
    p_sleep_param[1] = (uint8_t)time;
    p_sleep_param[0] = (uint16_t)(time*60)%60;
    
    memcpy(&p_sleep_param[4],"Slep",4);
    
    return Flash_CommonStore((uint32_t*)(APP_CONFIG_BASE_ADDR+0x30),sleep_param, 2);
}

/*******************************************************************************************************
** Function name:            App_GetBluetoothName
** Descriptions:             �����豸����
** input parameter:          p_dev:����ָ��
** Returned Value:           NRF_SUCCESS
** Created  by:              ����
** Created  Datas:           2017-08-04
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
*********************************************************************************************************/
uint32_t App_GetBluetoothName(bluetooth_name_t *p_bluetooth_name)
{
    uint32_t name_param[9] = {0};
    uint8_t* p_name_param = (uint8_t*)name_param;
    
    Flash_CommonRead(name_param,APP_CONFIG_BASE_ADDR+0x40,sizeof(name_param));
    
    /* У���ȡͷ����־ */
    if(memcmp(p_name_param,"Blueth",6) == 0)
    {
        p_bluetooth_name->length = p_name_param[6];   
        memcpy(p_bluetooth_name->name,&p_name_param[7],p_bluetooth_name->length); //�������Ƴ���
    }
    else
    {     
        p_bluetooth_name->length = strlen(DEVICE_NAME);
        memcpy(p_bluetooth_name->name,DEVICE_NAME,strlen(DEVICE_NAME));//ʹ��Ĭ�ϵ��豸����  
    }
    
    return NRF_SUCCESS;
}

/*******************************************************************************************************
** Function name:            App_UpdateDeviceName
** Descriptions:             �����豸����
** input parameter:          p_device_info:����ָ��
** Returned Value:           NRF_SUCCESS
** Created  by:              ����
** Created  Datas:           2017-08-04
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
*********************************************************************************************************/
uint32_t App_UpdateBluetoothName(const bluetooth_name_t *p_device_info)
{
    uint32_t name_param[9] = {0};
    uint8_t* p_name_param = (uint8_t*)name_param;
    
    memcpy(p_name_param,"Blueth",6);  
    p_name_param[6] = p_device_info->length;
    memcpy(&p_name_param[7],p_device_info->name,p_device_info->length);

    return Flash_CommonStore((uint32_t*)(APP_CONFIG_BASE_ADDR+0x40),name_param, 9);
}

/*******************************************************************************************************
** Function name:            App_UpdateHeartAlertParam
** Descriptions:             �������ʱ���
** input parameter:          p_dev:���� ָ��
** Returned Value:           NRF_SUCCESS
** Created  by:              ����
** Created  Datas:           2017-08-04
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
*********************************************************************************************************/
uint32_t App_UpdateHeartAlertParam(bool enable, uint8_t high,uint8_t low)
{
    uint32_t alert_param[2] = {0};
    uint8_t* p_alert_param = (uint8_t*)alert_param;

    // ���¸�ѹ����ѹ
    p_alert_param[0] = high;
    p_alert_param[1] = low;
    p_alert_param[2] = enable;
    memcpy(&p_alert_param[4],"Beat",4);

    return Flash_CommonStore((uint32_t*)(APP_CONFIG_BASE_ADDR+0x94),alert_param, 2);
}

/*******************************************************************************************************
** Function name:            App_GetHeartAlertParam
** Descriptions:             ȡ�����ʲ���
** input parameter:          high����  low����ֵ
** Returned Value:           NRF_SUCCESS
** Created  by:              ����
** Created  Datas:           2017-08-04
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
*********************************************************************************************************/
uint32_t App_GetHeartAlertParam(uint8_t* enable, uint8_t* high,uint8_t* low)
{
    uint32_t alert_param[2] = {0};
    uint8_t* p_alert_param = (uint8_t*)alert_param;

    Flash_CommonRead(alert_param,APP_CONFIG_BASE_ADDR+0x94,sizeof(alert_param));

    if(0 == memcmp(&p_alert_param[4],"Beat",4))
    {
        *enable = p_alert_param[2];
        *low = p_alert_param[1];
        *high = p_alert_param[0];
    }
    else
    {
        *enable = 0x00;
        *low =  55;
        *high = 115;
    }
    
    return NRF_SUCCESS;
}

/*******************************************************************************************************
** Function name:            App_UpdateTurnWristParam
** Descriptions:             ����ת��������
** input parameter:          ʱ��
** Returned Value:           NRF_SUCCESS
** Created  by:              ����
** Created  Datas:           2017-08-04
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
*********************************************************************************************************/
uint32_t App_UpdateTurnWristParam(const turnwrist_config_t *p_turnwrist)
{
    uint32_t turn_param[3] = {0};
    uint8_t* p_turn_param = (uint8_t*)turn_param;

    //��ֵ����ʱ��
    p_turn_param[0] = p_turnwrist->s_minute;
    p_turn_param[1] = p_turnwrist->s_hour;
    //��ֵ��ʼʱ��
    p_turn_param[2] = p_turnwrist->e_minute;
    p_turn_param[3] = p_turnwrist->e_hour;
    //��ֵ����
    p_turn_param[4] = p_turnwrist->enable;
    // �������������ò���
    p_turn_param[5] = p_turnwrist->sensitivity%10;
    memcpy(&p_turn_param[8],"Turn",4);

    return Flash_CommonStore((uint32_t*)(APP_CONFIG_BASE_ADDR+0x9C),turn_param, 3);
}

/*******************************************************************************************************
** Function name:            App_GetTurnWristParam
** Descriptions:             ȡ����������
** input parameter:          ����
** Returned Value:           NRF_SUCCESS
** Created  by:              ����
** Created  Datas:           2017-08-04
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
*********************************************************************************************************/
uint32_t App_GetTurnWristParam(turnwrist_config_t *p_turnwrist)
{
    uint32_t turn_param[3] = {0};
    uint8_t* p_turn_param = (uint8_t*)turn_param;

    Flash_CommonRead(turn_param,APP_CONFIG_BASE_ADDR+0x9C,sizeof(turn_param));
    if(0 == memcmp(&p_turn_param[8],"Turn",4))
    {
        p_turnwrist->sensitivity = p_turn_param[5]%10;
        p_turnwrist->enable = p_turn_param[4];
        p_turnwrist->e_hour = p_turn_param[3];
        p_turnwrist->e_minute = p_turn_param[2];
        p_turnwrist->s_hour = p_turn_param[1];
        p_turnwrist->s_minute = p_turn_param[0];
    }
    else
    {
        //YX.20180507.���ֵķ����������ܸ�Ϊ24СʱĬ�ϴ�
#if 0		
        p_turnwrist->enable = 0x01;
        p_turnwrist->sensitivity = 5;
        p_turnwrist->s_hour = 0;
        p_turnwrist->s_minute = 0;
        p_turnwrist->e_hour = 23;
        p_turnwrist->e_minute = 59;
#else		
        p_turnwrist->enable = 0x01;
        p_turnwrist->sensitivity = 5;
        p_turnwrist->s_hour = 8;
        p_turnwrist->s_minute = 0;
        p_turnwrist->e_hour = 22;
        p_turnwrist->e_minute = 0;
#endif		
    }
        
    return NRF_SUCCESS;
}

/*******************************************************************************************************
** Function name:            App_UpdateGuiLanguage
** Descriptions:             �����л�
** input parameter:          ����
** Returned Value:           NRF_SUCCESS
** Created  by:              ����
** Created  Datas:           2017-08-04
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
*********************************************************************************************************/
uint32_t App_UpdateGuiLanguage(uint8_t language)
{
    uint32_t language_param[2] = {0};
    uint8_t* p_language_param = (uint8_t*)&language_param;

    p_language_param[0] = language;
//    p_language_param[1] = 0xFF;
//    p_language_param[2] = 0xFF;
//    p_language_param[3] = 0xFF;
    memcpy(&p_language_param[4],"lang",4);

    return Flash_CommonStore((uint32_t*)(APP_CONFIG_BASE_ADDR+0xA8),language_param, 2);
}

/*******************************************************************************************************
** Function name:            App_GetGuiLanguage
** Descriptions:             ȡ�����Բ���
** input parameter:          ����
** Returned Value:           NRF_SUCCESS
** Created  by:              ����
** Created  Datas:           2017-08-04
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
*********************************************************************************************************/
uint32_t App_GetGuiLanguage(uint8_t* language)
{
    uint32_t language_param[2] = {0};
    uint8_t* p_language_param = (uint8_t*)&language_param;
    
    Flash_CommonRead(language_param,APP_CONFIG_BASE_ADDR+0xA8,sizeof(language_param));
    if(0 == memcmp(&p_language_param[4],"lang",4))
    {
        *language = p_language_param[0];
    }
    else
    {
        //Ĭ��ΪӢ��
        *language = 0x02;
        App_UpdateGuiLanguage(0x02);
    }
    
    return NRF_SUCCESS;
}

/*******************************************************************************************************
** Function name:            App_UpdateConnectPassword
** Descriptions:             ������������
** input parameter:          password������
** Returned Value:           NRF_SUCCESS
** Created  by:              ����
** Created  Datas:           2017-08-04
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
*********************************************************************************************************/
uint32_t App_UpdateConnectPassword(uint16_t password)
{
    uint32_t password_param[3] = {0};
    uint8_t* p_password_param = (uint8_t*)password_param;

    memcpy(p_password_param,"Password",8);
    p_password_param[8] = password&0xFF;
    p_password_param[9] = password>>8;

    return Flash_CommonStore((uint32_t*)(APP_CONFIG_BASE_ADDR+0xBC),password_param, 3);
}

/*******************************************************************************************************
** Function name:            App_GetConnectPassword
** Descriptions:             ȡ������
** input parameter:          password������
** Returned Value:           NRF_SUCCESS
** Created  by:              ����
** Created  Datas:           2017-08-04
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
*********************************************************************************************************/
uint32_t App_GetConnectPassword(uint16_t* password)
{
    uint32_t password_param[3] = {0};
    uint8_t* p_password_param = (uint8_t*)password_param;

    Flash_CommonRead(password_param,APP_CONFIG_BASE_ADDR+0xBC,sizeof(password_param));
    if(0 == memcmp(p_password_param,"Password",8))
    {
        *password = p_password_param[8] | p_password_param[9]<<8;
    }
    else
    {
        *password = 0;
    }
    
    return NRF_SUCCESS;
}

/*******************************************************************************************************
** Function name:            App_UpdateBrightnessParam
** Descriptions:             �������Ȳ���
** input parameter:          p_param������ָ��
** Returned Value:           NRF_SUCCESS
** Created  by:              ����
** Created  Datas:           2017-08-04
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
*********************************************************************************************************/
uint32_t App_UpdateBrightnessParam(brightness_config_t* p_param)
{
    uint32_t brightness_config[3] = {0};
    uint8_t* p_brightness_config = (uint8_t*)brightness_config;

    //��ֵ����ʱ��
    p_brightness_config[0] = p_param->other_bright;
    p_brightness_config[1] = p_param->valid_bright;
    //��ֵ��ʼʱ��
    p_brightness_config[2] = p_param->end_minute;
    p_brightness_config[3] = p_param->end_hour;
    //��ֵ����
    p_brightness_config[4] = p_param->start_minute;
    p_brightness_config[5] = p_param->start_hour;
    
    memcpy(&p_brightness_config[8],"Ligt",4);

    return Flash_CommonStore((uint32_t*)(APP_CONFIG_BASE_ADDR+0xB0),brightness_config, 3);
}

/*******************************************************************************************************
** Function name:            App_GetBrightnessParam
** Descriptions:             ȡ�����Ȳ���
** input parameter:          p_param:����ָ��
** Returned Value:           NRF_SUCCESS
** Created  by:              ����
** Created  Datas:           2017-08-04
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
*********************************************************************************************************/
uint32_t App_GetBrightnessParam(brightness_config_t* p_param)
{
    uint32_t brightness_config[3] = {0};
    uint8_t* p_brightness_config = (uint8_t*)brightness_config;

    Flash_CommonRead(brightness_config,APP_CONFIG_BASE_ADDR+0xB0,sizeof(brightness_config));
    if(0 == memcmp(&p_brightness_config[8],"Ligt",4))
    {
        p_param->other_bright = p_brightness_config[0];
        p_param->valid_bright = p_brightness_config[1];
        p_param->end_minute   = p_brightness_config[2];
        p_param->end_hour     = p_brightness_config[3];
        p_param->start_minute = p_brightness_config[4];
        p_param->start_hour   = p_brightness_config[5];
    }
    else
    {
        p_param->other_bright = 4;
        p_param->valid_bright = 2;
        p_param->end_minute   = 0;
        p_param->end_hour     = 8;
        p_param->start_minute = 0;
        p_param->start_hour   = 22;
    }
    
    return NRF_SUCCESS;
}

/*******************************************************************************************************
** Function name:            App_UpdateBloodOxygenParam
** Descriptions:             ����Ѫ������
** input parameter:          p_param��Ѫ������ָ��
** Returned Value:           NRF_SUCCESS
** Created  by:              ���� 
** Created  Datas:           2017-08-04
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
*********************************************************************************************************/
uint32_t App_UpdateBloodOxygenParam(time_range_config_t* p_param)
{
    uint32_t param_config[3] = {0};
    uint8_t* p_param_config = (uint8_t*)param_config;

    p_param_config[0] = p_param->enable;
    p_param_config[1] = p_param->end_minute;
    p_param_config[2] = p_param->end_hour;
    p_param_config[3] = p_param->start_minute;
    p_param_config[4] = p_param->start_hour;
    
    memcpy(&p_param_config[5],"boxygen",7);

    return Flash_CommonStore((uint32_t*)(APP_CONFIG_BASE_ADDR+0xE4),param_config, 3);
}

/*******************************************************************************************************
** Function name:            App_GetBloodOxygenParam
** Descriptions:             ȡ��Ѫ������
** input parameter:          p_param��Ѫ������ָ��
** Returned Value:           NRF_SUCCESS
** Created  by:              ����
** Created  Datas:           2017-08-04
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
*********************************************************************************************************/
uint32_t App_GetBloodOxygenParam(time_range_config_t* p_param)
{
    uint32_t param_config[3] = {0};
    uint8_t* p_param_config = (uint8_t*)param_config;

    Flash_CommonRead(param_config, APP_CONFIG_BASE_ADDR+0xE4, sizeof(param_config));
    if(0 == memcmp(&p_param_config[5],"boxygen",7))
    {
        p_param->enable = p_param_config[0];
        p_param->end_minute = p_param_config[1];
        p_param->end_hour   = p_param_config[2];
        p_param->start_minute     = p_param_config[3];
        p_param->start_hour = p_param_config[4];
    }
    else
    {
        p_param->enable         = 1;
        p_param->end_minute     = 0;
        p_param->end_hour       = 8;
        p_param->start_minute   = 0;
        p_param->start_hour     = 22;
    }
    
    return NRF_SUCCESS;
}

/*******************************************************************************************************
** Function name:            App_FlashWriteMovementHead
** Descriptions:             д�˶�ģʽͷ����
** input parameter:          
** Returned Value:           NRF_SUCCESS
** Created  by:              YX
** Created  Datas:           2017-11-17
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
*********************************************************************************************************/
uint32_t App_FlashWriteMovementHead(const uint8_t block_count, const uint8_t *head_data, const uint16_t head_data_len)
{
    uint32_t param_config[10] = {0};
    uint8_t* p_param_config = (uint8_t*)param_config;
    
    if(block_count >= MOVEMENT_PATTERNS_DATA_BLOCK_MAX || head_data == NULL || head_data_len == 0 
      || head_data_len > RECORD_MOVEMENT_HEAD_OFFSET * 4)
    {
        return 0x01;
    }
    
    memcpy(p_param_config,head_data,head_data_len);
    
//    memcpy(&p_param_config[head_data_len],"move",4);

    return Flash_CommonStore((uint32_t*)RECORD_MOVEMENT_HEAD_ADDR(block_count,0),param_config, 10);
}

/*******************************************************************************************************
** Function name:            App_FlashReadMovementHead
** Descriptions:             ���˶�ģʽͷ����
** input parameter:          
** Returned Value:           NRF_SUCCESS
** Created  by:              YX
** Created  Datas:           2017-11-17
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
*********************************************************************************************************/
uint32_t App_FlashReadMovementHead(const uint8_t block_count, uint8_t *head_read_buf, const uint16_t head_read_len)
{
    uint32_t param_config[10] = {0};
    uint8_t* p_param_config = (uint8_t*)param_config;
    

    if(block_count >= MOVEMENT_PATTERNS_DATA_BLOCK_MAX || head_read_buf == NULL 
      || head_read_len == 0 
      || head_read_len > RECORD_MOVEMENT_HEAD_OFFSET * 4)
    {
        return 0x01;
    }
    
    Flash_CommonRead(param_config, RECORD_MOVEMENT_HEAD_ADDR(block_count,0), sizeof(param_config));
//    if(0 == memcmp(&p_param_config[head_read_len],"move",4))
//    {
        memcpy(head_read_buf,p_param_config,head_read_len);
//    }
//    else
//    {
//        return 0x01;
//    }
    
    return NRF_SUCCESS;
}

/*******************************************************************************************************
** Function name:            App_FlashWriteMovementData
** Descriptions:             д�˶�ģʽ����
** input parameter:          
** Returned Value:           NRF_SUCCESS
** Created  by:              YX
** Created  Datas:           2017-11-17
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
*********************************************************************************************************/
uint32_t App_FlashWriteMovementData(const uint8_t block_count, const uint16_t number, const uint8_t *data, const uint16_t data_len)
{
    uint32_t param_config[5] = {0};
    uint8_t* p_param_config = (uint8_t*)param_config;
    
    if(block_count >= MOVEMENT_PATTERNS_DATA_BLOCK_MAX || data == NULL 
      || data_len == 0 
      || data_len > 20)
    {
        return 0x01;
    }
    
    memcpy(p_param_config,data,data_len);
    memcpy(&p_param_config[16],"data",4);
    
    return Flash_CommonStore((uint32_t*)RECORD_MOVEMENT_DATA_ADDR(block_count,number),param_config, 5);
}

/*******************************************************************************************************
** Function name:            App_FlashReadMovementHead
** Descriptions:             ���˶�ģʽ����
** input parameter:          
** Returned Value:           NRF_SUCCESS
** Created  by:              YX
** Created  Datas:           2017-11-17
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
*********************************************************************************************************/
uint32_t App_FlashReadMovementData(uint8_t block_count, uint16_t number, uint8_t *data)
{
    uint32_t param_config[5] = {0};
    uint8_t* p_param_config = (uint8_t*)param_config;

    if(block_count >= MOVEMENT_PATTERNS_DATA_BLOCK_MAX || data == NULL)
    {
        return 0x01;
    }
    
    Flash_CommonRead(param_config, RECORD_MOVEMENT_DATA_ADDR(block_count,number), sizeof(param_config));
    if(0 == memcmp(&p_param_config[16],"data",4))
    {
        memcpy(data,p_param_config,20);
    }
    else
    {
        return 0x01;
    }
    
    return NRF_SUCCESS;
}

/*******************************************************************************************************
** Function name:            App_FlashEraseMovementDataPage
** Descriptions:             ����ĳ����˶�ģʽ����
** input parameter:          
** Returned Value:           NRF_SUCCESS
** Created  by:              YX
** Created  Datas:           2017-11-17
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
*********************************************************************************************************/
uint32_t App_FlashEraseMovementDataPage(uint8_t block_count)
{
    if(block_count >= MOVEMENT_PATTERNS_DATA_BLOCK_MAX)
    {
        return 0x01;
    }
    return Flash_CommonErase((uint32_t*)RECORD_MOVEMENT_HEAD_ADDR(block_count,0),RECORD_MOVEMENT_TOTAL_SIZE/4096);
}

/*******************************************************************************************************
** Function name:            App_FlashEraseMovementDataAllPage
** Descriptions:             ����ȫ���˶�ģʽ����
** input parameter:          
** Returned Value:           NRF_SUCCESS
** Created  by:              YX
** Created  Datas:           2017-11-17
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
*********************************************************************************************************/
uint32_t App_FlashEraseMovementDataAllPage(void)
{
    return Flash_CommonErase((uint32_t*)RECORD_MOVEMENT_HEAD_ADDR(0,0),RECORD_MOVEMENT_TOTAL_SIZE*3/4096);
}

/****************************************************************
** Function name:           App_FlashCheckAreaUsable
** Descriptions:            
** input parameters:
** output parameters:
** Returned value:          ����0���ѱ�ʹ�ã�1��û�б�ʹ��
** Created by:              WJ              
** Created Date:            2017-08-03
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/
static bool App_FlashCheckAreaBufUsable(uint8_t *input_data, uint16_t input_len)
{  
    uint16_t i = 0;
    
    if(input_data == NULL || input_len == 0)
    {
        return 0;
    }

    for(i = 0; i < input_len; i++)
    {
        if(input_data[i] != 0xFF)
        {
            return 0;
        }
    }    
    return 1;    
}


/****************************************************************
** Function name:           App_FlashCheckAreaIsUsable
** Descriptions:            ��ȡָ��flash�����Ƿ�ʹ�ù�
** input parameters:
** output parameters:
** Returned value:          ����0��flashʹ�ù���1��flashδʹ�ù�
** Created by:              WJ              
** Created Date:            2017-08-03
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/
uint32_t App_FlashCheckAreaIsUsable(uint8_t start_page, uint8_t page_size)
{  
    uint8_t i = 0;
    uint8_t j = 0;
    uint8_t ret = 1;
    uint32_t flash_data_buf[64] = {0x00,};
    uint32_t offset = 0;
    
    if(page_size == 0)
    {
        return 0;
    }  
    
    for(i = 0; i < page_size; i++)
    {     
        offset = (start_page + i) * 4 * 1024;       
        for(j = 0; j < 4; j++)
        {
            offset += j * 256;// / 4;
            Flash_CommonRead(flash_data_buf,offset,256);
            ret &= App_FlashCheckAreaBufUsable((uint8_t *)flash_data_buf, 256);
        }
        if(ret == 0)
        {
            return 0;
        }
    }
    
    return 1;
}
    
/*******************************************************************************************************
** Function name:            App_UpdateRespirationRateParam
** Descriptions:             ����ƣ�ͶȲ���
** input parameter:          p_param:ƣ�ͶȲ���
** Returned Value:           NRF_SUCCESS
** Created  by:              ����
** Created  Datas:           2017-08-04
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
*********************************************************************************************************/
uint32_t App_UpdateRespirationRateParam(time_range_config_t* p_param)
{
    uint32_t param_config[3] = {0};
    uint8_t* p_param_config = (uint8_t*)param_config;

    memcpy(p_param_config,"Breath",6);
    p_param_config[6] = p_param->enable;
    p_param_config[7] = p_param->start_hour;
    p_param_config[8] = p_param->start_minute;
    p_param_config[9] = p_param->end_hour;
    p_param_config[10] = p_param->end_minute;

    return Flash_CommonStore((uint32_t*)(APP_CONFIG_BASE_ADDR+0xF0),param_config, 3);
}

/*******************************************************************************************************
** Function name:            App_GetRespirationRateParam
** Descriptions:             �õ�ƣ�ͶȲ���
** input parameter:          p_param:ƣ�ͶȲ���ָ��
** Returned Value:           NRF_SUCCESS
** Created  by:              ����
** Created  Datas:           2017-08-04
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
*********************************************************************************************************/
uint32_t App_GetRespirationRateParam(time_range_config_t* p_param)
{
    uint32_t param_config[3] = {0};
    uint8_t* p_param_config = (uint8_t*)param_config;

    Flash_CommonRead(param_config, APP_CONFIG_BASE_ADDR+0xF0, sizeof(param_config));
    if(0 == memcmp(p_param_config,"Breath",6))
    {
        p_param->enable       = p_param_config[6];
        p_param->start_minute = p_param_config[7];
        p_param->start_hour   = p_param_config[8];
        p_param->end_hour     = p_param_config[9];
        p_param->end_minute   = p_param_config[10];
    }
    else
    {
        p_param->enable         = 1;
        p_param->end_minute     = 0;
        p_param->end_hour       = 8;
        p_param->start_minute   = 0;
        p_param->start_hour     = 22;
    }
    
    return NRF_SUCCESS;
}

uint32_t App_GetHourglassParam(hourglass_config_t *p_hourglass_config)
{
    uint32_t param_config[4] = {0};
    uint8_t* p_param_config = (uint8_t*)param_config;

    Flash_CommonRead(param_config, APP_CONFIG_BASE_ADDR+0xD4, sizeof(param_config));
    if(0 == memcmp(p_param_config,"Hourglas",8))
    {
        p_hourglass_config->dev_remain_time = (uint32_t)(p_param_config[8]|(p_param_config[9]<<8)|(p_param_config[10]<<16)|(p_param_config[11]<<24));
        p_hourglass_config->ui_enable =  p_param_config[12]&0x01;
    }
    else
    {
        p_hourglass_config->dev_remain_time = 0;
        p_hourglass_config->ui_enable =  0;
    }
    
    return NRF_SUCCESS;	
}

uint32_t App_UpdateHourglassParam(const hourglass_config_t *p_hourglass_config)
{
    uint32_t param_config[4] = {0};
    uint8_t* p_param_config = (uint8_t*)param_config;

    memcpy(p_param_config,"Hourglas",8);
    
    p_param_config[8]  = (uint8_t)(p_hourglass_config->dev_remain_time&0xFF);
    p_param_config[9]  = (uint8_t)((p_hourglass_config->dev_remain_time>>8)&0xFF);
    p_param_config[10] = (uint8_t)((p_hourglass_config->dev_remain_time>>16)&0xFF);
    p_param_config[11] = (uint8_t)((p_hourglass_config->dev_remain_time>>24)&0xFF);
    p_param_config[12] = p_hourglass_config->ui_enable;
    
    return Flash_CommonStore((uint32_t*)(APP_CONFIG_BASE_ADDR+0xD4),param_config, 4);	
}


uint32_t App_GetUiStyleParam(uint8_t *ui_style)
{
    uint32_t param_config[1] = {0};
    uint8_t* p_param_config = (uint8_t*)param_config;

    Flash_CommonRead(param_config, APP_CONFIG_BASE_ADDR+0xFC, sizeof(param_config));
	
    if(0 == memcmp(p_param_config,"Ui",2))
    {
        *ui_style = p_param_config[2];
    }
    else
    {
        *ui_style = 0;
    }
    
    return NRF_SUCCESS;	
}

uint32_t App_UpdateUiStyleParam(const uint8_t param)
{
    uint32_t param_config[1] = {0};
    uint8_t* p_param_config = (uint8_t*)param_config;

    memcpy(p_param_config,"Ui",2);
    
    p_param_config[2]  = (uint8_t)(param);
    p_param_config[3]  = (uint8_t)(0xFF);

    
    return Flash_CommonStore((uint32_t*)(APP_CONFIG_BASE_ADDR+0xFC),param_config, 1);	
}

uint32_t App_GetMoveMaxPuseTimeParam(uint16_t *movement_max_puse_time)
{
    uint32_t param_config[1] = {0};
    uint8_t* p_param_config = (uint8_t*)param_config;

    Flash_CommonRead(param_config, APP_CONFIG_BASE_ADDR+0x100, sizeof(param_config));
	
    if(0 == memcmp(p_param_config,"Mo",2))
    {
        *movement_max_puse_time = (uint16_t)(p_param_config[2]|p_param_config[3]<<8);
    }
    else
    {
        *movement_max_puse_time = 60;
    }
    
    return NRF_SUCCESS;	
}

uint32_t App_UpdateMoveMaxPuseTimeParam(const uint16_t param)
{
    uint32_t param_config[1] = {0};
    uint8_t* p_param_config = (uint8_t*)param_config;

    memcpy(p_param_config,"Mo",2);
    
    p_param_config[2]  = (uint8_t)(param&0xFF);
    p_param_config[3]  = (uint8_t)((param>>8)&0xFF);

    
    return Flash_CommonStore((uint32_t*)(APP_CONFIG_BASE_ADDR+0x100),param_config, 1);	
}


