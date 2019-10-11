/***************************Copyright(c)***********************************
**                       Veepoo Tech
**                  COPYRIGHT (C) 2016-2019
**-----------File Info-----------------------------------------------------
** File        name:     product_info.c
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
#include <string.h>
#include "app_error.h"
#include "product_info.h"
#include "app_type.h"
#include "app_flash.h"
#include "user_bootloader.h"
#include "board.h"

/*********************************************************************
* �궨��
*/

/*********************************************************************
* ȫ�ֱ���
*/

static product_info_t  product_info_s;
						
/*********************************************************************
* ���غ�������
*/
static void Product_SetDfuLedPin(void);

/****************************************************************
** Function name:           Product_InfoInit
** Descriptions:            ��Ʒ��Ϣ��ʼ��
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-08-11
*****************************************************************/ 
static void Product_InfoInit(void)
{
    /* ��ʼ���ṹ�� */
    memset(product_info_s.data, 0, sizeof(product_info_s.data));
}

/****************************************************************
** Function name:           Product_WriteInfomationToFlash
** Descriptions:            ��Ʒ��Ϣд��Flash
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-08-11
*****************************************************************/ 
static void Product_WriteInfomationToFlash(void)
{    
    memset(product_info_s.data, 0xFF, sizeof(product_info_s.data));
    
    /* д���Ʒʶ���� */
    product_info_s.attr.master_sn[0] = VEEPOO_DEVICE_ID&0xFF;
    product_info_s.attr.master_sn[1] = (VEEPOO_DEVICE_ID>>8)&0xFF;
    product_info_s.attr.master_sn[2] = 0xFF;
    product_info_s.attr.master_sn[3] = 0xFF;

    /* д�빫˾���Ʊ��� */
    product_info_s.attr.company_name[0] = COMPANY&0xFF;
    product_info_s.attr.company_name[1] = (COMPANY>>8)&0xFF;
    product_info_s.attr.company_name[2] = 0xFF;
    product_info_s.attr.company_name[3] = 0xFF;

    /* д���Ʒ�ͺű��� */
    product_info_s.attr.product_version[0] = DEVICE_TYPE&0xFF;
    product_info_s.attr.product_version[1] = (DEVICE_TYPE>>8)&0xFF;
    product_info_s.attr.product_version[2] = (DEVICE_TYPE>>16)&0xFF;
    product_info_s.attr.product_version[3] = (DEVICE_TYPE>>24)&0xFF;

        /* д��Ӳ���汾��Ϣ */
    product_info_s.attr.hardware_version[0] = HARDWARE_TYPE&0xFF;
    product_info_s.attr.hardware_version[1] = 0xFF;
    product_info_s.attr.hardware_version[2] = 0xFF;
    product_info_s.attr.hardware_version[3] = 0xFF;

    /* д��̼��汾��Ϣ */
    product_info_s.attr.firmware_version[0] = VERSION_T;
    product_info_s.attr.firmware_version[1] = VERSION_L;
    product_info_s.attr.firmware_version[2] = VERSION_H;
    product_info_s.attr.firmware_version[3] = VERSION_M;

    /* д������������Ϣ */
    product_info_s.attr.product_date[0] = PRODUCT_DATE&0xFF;
    product_info_s.attr.product_date[1] = (PRODUCT_DATE>>8)&0xFF;
    product_info_s.attr.product_date[2] = (PRODUCT_DATE>>16)&0xFF;
    product_info_s.attr.product_date[3] = 0xFF;

    /* д���������Ϣ */
    product_info_s.attr.agent_id[0] = AGENCY_SERIAL_NUMBER&0xFF;
    product_info_s.attr.agent_id[1] = (AGENCY_SERIAL_NUMBER>>8)&0xFF;
    product_info_s.attr.agent_id[2] = (AGENCY_SERIAL_NUMBER>>16)&0xFF;
    product_info_s.attr.agent_id[3] = 0xFF;

    /* д������������Ϣ */
    product_info_s.attr.product_batch[0] = PRODUCT_BATCH&0xFF;
    product_info_s.attr.product_batch[1] = (PRODUCT_BATCH>>8)&0xFF;
    product_info_s.attr.product_batch[2] = 0xFF;
    product_info_s.attr.product_batch[3] = 0xFF;

    /* д������������Ϣ */
    product_info_s.attr.product_count[0] = PRODUCT_NUMBER&0xFF;
    product_info_s.attr.product_count[1] = (PRODUCT_NUMBER>>8)&0xFF;
    product_info_s.attr.product_count[2] = (PRODUCT_NUMBER>>16)&0xFF;
    product_info_s.attr.product_count[3] = 0xFF;

    /* д��ʶ����(��) */
    product_info_s.attr.slave_sn[0] = VEEPOO_DEVICE_ID&0xFF;
    product_info_s.attr.slave_sn[1] = (VEEPOO_DEVICE_ID>>8)&0xFF;
    product_info_s.attr.slave_sn[2] = 0xFF;
    product_info_s.attr.slave_sn[3] = 0xFF;

	product_info_s.attr.pin_config[0] = LDO_EN_PIN;
	product_info_s.attr.pin_config[1] = GC1_PIN;
	product_info_s.attr.pin_config[2] = GC2_PIN;
	product_info_s.attr.pin_config[3] = 0;//1;           //�ߵ�ƽʹ��

    /* д������ */
    App_FlashUpdataProductInformation((uint8_t *)product_info_s.data, sizeof(product_info_s.data));
    nrf_delay_ms(100);
}

/****************************************************************
** Function name:           Product_InfoInit
** Descriptions:            ��Ʒ��Ϣ�Լ�
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-08-11
*****************************************************************/ 
void Product_InfoCheck(void)
{
    uint8_t sn_write_flag = 0;
    
    Product_InfoInit();

    /* ��ȡ��Ʒ��Ϣ�� */
    App_FlashGetProductInformation((uint8_t *)product_info_s.data, sizeof(product_info_s.data));

    /* 1���жϲ�Ʒ��Ϣ�Ƿ���Ч */
    if((0xFF == product_info_s.attr.master_sn[1])&&(0xFF == product_info_s.attr.master_sn[0]))
    {
        Product_WriteInfomationToFlash();
        sn_write_flag = 1;
    }

    /* ��ȡ��Ʒ��Ϣ�� */
    App_FlashGetProductInformation((uint8_t *)product_info_s.data, sizeof(product_info_s.data));

    /* �Ƚ��豸�� */
    bool master_pass = (product_info_s.attr.master_sn[0] == (VEEPOO_DEVICE_ID & 0xFF))
                        && (product_info_s.attr.master_sn[1] == ((VEEPOO_DEVICE_ID>>8)&0xFF));
    bool slave_pass = (product_info_s.attr.slave_sn[0] == (VEEPOO_DEVICE_ID&0xFF))
                       && (product_info_s.attr.slave_sn[1] == ((VEEPOO_DEVICE_ID>>8)&0xFF));

    /* ��ƷIDУ�鲻ͨ��,����DFUģʽ */
    if(!(master_pass || slave_pass) && sn_write_flag == 0)
    {
        Dfu_JumpToBootloader(); 
        return;
    }
    if( sn_write_flag != 1)
    {
        Product_SetDfuLedPin();
    }
}

/****************************************************************
** Function name:           Product_GetSnInfo
** Descriptions:            ��ȡ��ƷSN
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-08-11
*****************************************************************/ 
uint32_t Product_GetSnInfo(void)
{
    uint32_t dev_sn = 0;
    /* ��ʼ������ */
    memset(product_info_s.data,0,sizeof(product_info_s.data));

    /* ��ȡ��Ʒ��Ϣ�� */
    App_FlashGetProductInformation((uint8_t *)product_info_s.data, sizeof(product_info_s.data));

    dev_sn = product_info_s.attr.master_sn[0];
    dev_sn |= (product_info_s.attr.master_sn[1]<<8);
    dev_sn |= (product_info_s.attr.master_sn[2]<<16);
    dev_sn |= (product_info_s.attr.master_sn[3]<<24);

    return dev_sn;
}

/****************************************************************
** Function name:           Product_SetDfuLedPin
** Descriptions:            ����DFUģʽ���ƹܽ�
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-08-11
*****************************************************************/ 
static void Product_SetDfuLedPin(void)
{		
	/* 1���ֽ�FLASH����Ϣ������������ */
    App_FlashGetProductInformation((uint8_t *)product_info_s.data, sizeof(product_info_s.data));
    if(product_info_s.attr.pin_config[0] != LDO_EN_PIN  
      || product_info_s.attr.pin_config[1] != GC1_PIN
      || product_info_s.attr.pin_config[2] != GC2_PIN
      || product_info_s.attr.pin_config[3] != 0)
    {
        App_FlashEraseProductInformation();
        
        product_info_s.attr.pin_config[0] = LDO_EN_PIN;
        product_info_s.attr.pin_config[1] = GC1_PIN;
        product_info_s.attr.pin_config[2] = GC2_PIN;
        product_info_s.attr.pin_config[3] = 0;//1;
        nrf_delay_ms(1000);
        App_FlashUpdataProductInformation((uint8_t *)product_info_s.data, sizeof(product_info_s.data));
    }
}

/****************************************************************
** Function name:           Product_GetBootVesion
** Descriptions:            ��ȡboot�汾
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-08-11
*****************************************************************/ 
uint32_t Product_GetBootVesion(void)
{
    uint32_t boot_vesion = 0;

    App_FlashGetProductInformation((uint8_t *)product_info_s.data, sizeof(product_info_s.data));
    
    memcpy(&boot_vesion, &product_info_s.attr.bootloader_version, 4);
    
    if(boot_vesion == 0xFFFFFFFF) boot_vesion = 0;
        
    
    return boot_vesion;
}

/****************************************************************
** Function name:           Product_GetBleAdvName
** Descriptions:            ��ȡ�豸Flash�е���������
** input parameters:    
** output parameters:
** Returned value:
** Created by:              YX             
** Created Date:            2017-07-20
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/
uint8_t *Product_GetBleAdvName(void)
{
    Product_InfoInit();

    /* ��ȡ��Ʒ��Ϣ�� */
    App_FlashGetProductInformation((uint8_t *)product_info_s.data, sizeof(product_info_s.data));
    
    if(product_info_s.attr.blue_name[0] != 0x00)
    {
        return product_info_s.attr.blue_name;
    }
    return (uint8_t *)0;
}

/****************************************************************
** Function name:           Product_SetBleAdvName
** Descriptions:            �����豸����������
** input parameters:    
** output parameters:
** Returned value:
** Created by:              YX             
** Created Date:            2017-07-20
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/
uint8_t Product_SetBleAdvName(uint8_t* Name)
{
    if(Name[0] != 0x00)
    {//������Ч
        Product_InfoInit();

        /* ��ȡ��Ʒ��Ϣ�� */
        App_FlashGetProductInformation((uint8_t *)product_info_s.data, sizeof(product_info_s.data));
        
        memset(product_info_s.attr.blue_name, 0, sizeof(product_info_s.attr.blue_name));
        
        memcpy(product_info_s.attr.blue_name, Name, strlen((char *)Name));
        
        App_FlashEraseProductInformation();
        nrf_delay_ms(100);
        App_FlashUpdataProductInformation((uint8_t *)product_info_s.data, sizeof(product_info_s.data));
        nrf_delay_ms(100);        
    }
    else
    {//������Ч
        return 0;
    }
    return 1;
}

/****************************************************************
** Function name:           Product_CheckBleAdvName
** Descriptions:            ����豸����������
** input parameters:    
** output parameters:
** Returned value:
** Created by:              YX             
** Created Date:            2017-07-20
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/
void Product_CheckBleAdvName(void)
{
    uint8_t i = 0;
    Product_InfoInit();

    /* ��ȡ��Ʒ��Ϣ�� */
    App_FlashGetProductInformation((uint8_t *)product_info_s.data, sizeof(product_info_s.data));
    
    if(product_info_s.attr.blue_name[0] != 0x00)
    {
        for(i = 0; i < 8; i++)
        {
            if(product_info_s.attr.blue_name[i] == 0xFF)
            {
               product_info_s.attr.blue_name[i] = 0x00; 
            }
        }
        memcpy(ble_name, product_info_s.attr.blue_name, strlen((char *)product_info_s.attr.blue_name));
    }
}


/****************************************************************
** Function name:           Product_SetBpFunction
** Descriptions:            ����Ѫѹ����ģ��
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ            
** Created Date:            2017-11-11
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/
void Product_SetBpFunction(uint8_t bp_function)
{
    Product_InfoInit();

    /* ��ȡ��Ʒ��Ϣ�� */
    App_FlashGetProductInformation((uint8_t *)product_info_s.data, sizeof(product_info_s.data));
    
    if(product_info_s.attr.bp_flag != bp_function)
    {
        App_FlashEraseProductInformation();
        nrf_delay_ms(100);
        
        product_info_s.attr.bp_flag = bp_function;
        memset(product_info_s.attr.retain, 0xFF, sizeof(product_info_s.attr.retain));
        
        App_FlashUpdataProductInformation((uint8_t *)product_info_s.data, sizeof(product_info_s.data));
        nrf_delay_ms(100);         
    }    
}

/****************************************************************
** Function name:           Product_SetBpFunction
** Descriptions:            ��ȡѪѹ����ģ��
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ            
** Created Date:            2017-11-11
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/
uint8_t Product_GetBpFunctionState(void)
{
    Product_InfoInit();

    /* ��ȡ��Ʒ��Ϣ�� */
    App_FlashGetProductInformation((uint8_t *)product_info_s.data, sizeof(product_info_s.data));

    return product_info_s.attr.bp_flag;   
}

