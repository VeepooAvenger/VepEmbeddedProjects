/***************************Copyright(c)***********************************
**                       Veepoo Tech
**                  COPYRIGHT (C) 2016-2019
**-----------File Info-----------------------------------------------------
** File        name:     product_info.h
** Last modified Date:   2017-07-28
** Last SYS_VERSION:     0.0.0
** Descriptions:            
**-------------------------------------------------------------------------
** Created  by:          WJ
** Created  date:        2017-07-28
** SYS_VERSION:          0.0.0
** Descriptions:         
**-------------------------------------------------------------------------
** Modified by:				
** Modified date:  			          
** SYS_VERSION:                 
** Descriptions:                        
***************************************************************************/
#ifndef _DEV_SN_CHECK_H_
#define _DEV_SN_CHECK_H_

#include <stdint.h>
#include "app_config_data.h"

typedef struct
{
    uint8_t master_sn[4];               //�豸ʶ����1
    uint8_t company_name[8];            //��˾����
    uint8_t product_version[8];         //��Ʒ�ͺ�
    uint8_t hardware_version[4];        //
    uint8_t firmware_version[4];        //
    uint8_t product_date[4];            //
    uint8_t agent_id[8];                //
    uint8_t product_batch[8];           //
    uint8_t product_count[8];           //
    uint8_t slave_sn[4];                //
    uint8_t product_modify[4];          //

    uint8_t bootloader_version[8];      //boot�汾
    uint8_t application_version[8];     //app�汾

    uint8_t pin_config[4];              //DFUģʽ����������
    uint8_t blue_name[8];               //��������
    uint8_t bp_flag;                    //Ѫѹ֧��
    uint8_t sit_flag;                   //����֧��
    uint8_t state;                      //�͹��Ŀ���ģʽ����
	uint8_t notice_time_value;          //��Ϣ��������ʱ��
	uint8_t posture_time_value;         //��������ʱ��
	uint8_t normal_light_time_value;    // �����������ʱ��
	uint8_t motor_delay_time_value;     // ����ʱ����(��Ϣ����)
	uint8_t moter_time_value;           // �����ʱ��(��Ϣ����) 
	//20190725.����˵͹��Ĳ������ṹ����4�ֽڶ���
}product_attr_t;
//ע�⣺����ṹ���СҪ���������ݵĴ�С����һ��������sizeof(product_attr_t) == 4*25
typedef union
{
    product_attr_t  attr;
    uint32_t  data[25];
}product_info_t;

/* �豸sn���кŹ��˴��� */
void Product_InfoCheck(void);

//��ȡ��ƷSN
uint32_t Product_GetSnInfo(void);

//��ȡboot�汾
uint32_t Product_GetBootVesion(void);

//��ȡ�豸Flash�е���������
uint8_t *Product_GetBleAdvName(void);

//�����豸����������
uint8_t Product_SetBleAdvName(uint8_t* Name);

//����豸����������
void Product_CheckBleAdvName(void);

void Product_SetBpFunction(uint8_t bp_function);
uint8_t Product_GetBpFunction(void);

void Product_SetSitFunction(uint8_t sit_function);
uint8_t Product_GetSitFunction(void);

//���õ͹��Ŀ��ƹ���ģ��
void Product_SetLowEnergeFunction(low_energe_protocol_data_t *low_energe_para);

//��ȡ�͹��Ŀ��ƹ���ģ��
uint8_t Product_GetLowEnergeFunction(low_energe_protocol_data_t *low_energe_para);

#endif

