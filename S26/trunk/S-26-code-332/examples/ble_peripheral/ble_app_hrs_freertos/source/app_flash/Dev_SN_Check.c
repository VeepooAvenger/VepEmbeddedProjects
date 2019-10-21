/*************************************Copyright(c)***************************************************
**                                     Veepoo Tech
**                  
**                               COPYRIGHT(C)  2016-2019
**--------------------------------------FileInfo----------------------------------------------------
** FileName:                    Dev_SN_Check.c
** Last Modify Data:            2017-08-04
** Last SYS_VERSION:            XXXXXXXXX
** Descriptions:                contain all flash opration  Api                
**---------------------------------------------------------------------------------------------------
** CreatedBy:                   GT
** CreateData:                  2017-06-xx
** SystemVersion:             
** Description:                 the file's  funtion  realize the flash read ,write and erase at the same time 
**                              register the flash system from address 0x5c000  to 0x74000 can be  operation
**---------------------------------------------------------------------------------------------------
** Modify By:                       
** Modify Data:
** System Version:
** Modify Description:    

******************************************************************************************************/
#include "Dev_SN_Check.h"
#include <string.h>
#include "fstorage.h"
#include "app_error.h"
#include "softdevice_handler.h"
#include "user_bootloader.h"
#include "flash_common.h"
#include "configuration.h"

#define  PRODUCT_DATA_SIZE      24      //24*4 Byte

/************************************************************************
** Function name:                        Write_ProductInfomation
** Descriptions:           Get  device  sn  
** input parameter:        void
** Returned Value:         the  device id
** Created  by:            GT
** Created  Datas:         2017-08-04
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
*********************************************************************************************************/
static void Write_ProductInfomation(product_infomation_t *p_product)
{
    uint32_t err_code;
    uint32_t product_data[PRODUCT_DATA_SIZE] = {0};
	
    // 写入产品识别码 
    p_product->master_sn[0] = VEEPOO_DEVICE_ID_L&0xFF;
    p_product->master_sn[1] = (VEEPOO_DEVICE_ID_L>>8)&0xFF;
    p_product->master_sn[2] = 0xFF;
    p_product->master_sn[3] = 0xFF;

    // 写入公司名称编码 
    p_product->company_name[0] = COMPANY&0xFF;
    p_product->company_name[1] = (COMPANY>>8)&0xFF;
    p_product->company_name[2] = 0xFF;
    p_product->company_name[3] = 0xFF;

    // 写入产品型号编码 
    p_product->product_version[0] = DEVICE_TYPE&0xFF;
    p_product->product_version[1] = (DEVICE_TYPE>>8)&0xFF;
    p_product->product_version[2] = (DEVICE_TYPE>>16)&0xFF;
    p_product->product_version[3] = (DEVICE_TYPE>>24)&0xFF;

    // 写入硬件版本信息
    p_product->hardware_version[0] = HARDWARE_TYPE&0xFF;
    p_product->hardware_version[1] = 0xFF;
    p_product->hardware_version[2] = 0xFF;
    p_product->hardware_version[3] = 0xFF;

    // 写入固件版本信息
    p_product->firmware_version[0] = VERSION_T;
    p_product->firmware_version[1] = VERSION_L;
    p_product->firmware_version[2] = VERSION_H;
    p_product->firmware_version[3] = VERSION_M;

    // 写入生产日期信息 
    p_product->product_date[0] = PRODUCT_DATE&0xFF;
    p_product->product_date[1] = (PRODUCT_DATE>>8)&0xFF;
    p_product->product_date[2] = (PRODUCT_DATE>>16)&0xFF;
    p_product->product_date[3] = 0xFF;

    // 写入代理编号信息
    p_product->agent_id[0] = AGENCY_SERIAL_NUMBER&0xFF;
    p_product->agent_id[1] = (AGENCY_SERIAL_NUMBER>>8)&0xFF;
    p_product->agent_id[2] = (AGENCY_SERIAL_NUMBER>>16)&0xFF;
    p_product->agent_id[3] = 0xFF;

    // 写入生产批次信息 
    p_product->product_batch[0] = PRODUCT_BATCH&0xFF;
    p_product->product_batch[1] = (PRODUCT_BATCH>>8)&0xFF;
    p_product->product_batch[2] = 0xFF;
    p_product->product_batch[3] = 0xFF;

    // 写入生产数量信息 
    p_product->product_count[0] = PRODUCT_NUMBER&0xFF;
    p_product->product_count[1] = (PRODUCT_NUMBER>>8)&0xFF;
    p_product->product_count[2] = (PRODUCT_NUMBER>>16)&0xFF;
    p_product->product_count[3] = 0xFF;

    // 写入识别码(备) 
    p_product->slave_sn[0] = VEEPOO_DEVICE_ID_L&0xFF;
    p_product->slave_sn[1] = (VEEPOO_DEVICE_ID_L>>8)&0xFF;
    p_product->slave_sn[2] = 0xFF;
    p_product->slave_sn[3] = 0xFF;
    
    memcpy(product_data,p_product,sizeof(product_data));

    // 写入主区 
    err_code = Flash_CommonStore((uint32_t *)PRODUCT_INFOMATION_BASE_ADDR,product_data,sizeof(product_data)/4);
    APP_ERROR_CHECK(err_code);
}

/*******************************************************************************************************
** Function name:          Dev_GetProductionInfomation
** Descriptions:           得到生产信息
** input parameter:        无
** Returned Value:         设备id
** Created  by:            郭涛
** Created  Datas:         2017-08-04
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
*********************************************************************************************************/
uint32_t Dev_GetProductionInfomation(product_infomation_t *p_product)
{
    uint32_t err_code = NRF_SUCCESS;
    uint32_t product_data[PRODUCT_DATA_SIZE] = {0};
    
    err_code = Flash_CommonRead(product_data, PRODUCT_INFOMATION_BASE_ADDR, sizeof(product_data));
    
    memcpy(p_product,product_data,sizeof(product_data));
    
    return err_code;
}

/*******************************************************************************************************
** Function name:          Dev_SNCheck
** Descriptions:           设备序列号检测
** input parameter:        p_product：设备信息
** Returned Value:         设备 id
** Created  by:            郭涛
** Created  Datas:         2017-08-04
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
*********************************************************************************************************/
void Dev_SNCheck(product_infomation_t *p_product)
{
    uint32_t product_data[PRODUCT_DATA_SIZE] = {0};

    // 1、判断产品信息是否有效 
    if((0xFF == p_product->master_sn[1])&&(0xFF == p_product->master_sn[0]))
    {
        //Flash_CommonErase((uint32_t*)PRODUCT_INFOMATION_BASE_ADDR, 1);
        Write_ProductInfomation(p_product);
    }
    
    // 写完后,再读取产品信息区 
    Flash_CommonRead(product_data, PRODUCT_INFOMATION_BASE_ADDR, sizeof(product_data));
    memcpy(p_product,product_data,sizeof(product_data));

    /* 比较设备号 */
    bool master_pass = ((p_product->master_sn[0] == (VEEPOO_DEVICE_ID_L&0xFF))&&
                        (p_product->master_sn[1] == ((VEEPOO_DEVICE_ID_L>>8)&0xFF)));
    bool slave_pass = ((p_product->slave_sn[0] == (VEEPOO_DEVICE_ID_L&0xFF))&&
                       (p_product->slave_sn[1] == ((VEEPOO_DEVICE_ID_L>>8)&0xFF)));

    /* 产品ID校验不通过,进入DFU模式 */
    if(!(master_pass || slave_pass))
    {  
        jump_to_bootloader();
    }
}

/*******************************************************************************************************
** Function name:          Dev_GetDevID
** Descriptions:           取得设备id
** input parameter:        p_product:数据指针
** Returned Value:         设备id
** Created  by:            郭涛
** Created  Datas:         2017-08-04
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
*********************************************************************************************************/
uint32_t Dev_GetDevID(product_infomation_t *p_product)
{
    uint32_t dev_sn = 0;

    dev_sn = p_product->master_sn[0];
    dev_sn |= (p_product->master_sn[1]<<8);
    dev_sn |= (p_product->master_sn[2]<<16);
    dev_sn |= (p_product->master_sn[3]<<24);

    return dev_sn;
}

/*******************************************************************************************************
** Function name:                        Dev_DFUPinConfig
** Descriptions:           DFU pin config   
** input parameter:        生产信息
** Returned Value:         无
** Created  by:            郭涛
** Created  Datas:         2017-08-04
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
*********************************************************************************************************/
void Dev_DFUPinConfig(uint8_t LDO_EN,uint8_t GA1,uint8_t GA2,uint8_t GA3)
{
    //uint32_t err_code;

    //// 写完后,再读取产品信息区 
    //err_code = Flash_CommonRead(p_product->data,PRODUCT_INFOMATION_BASE_ADDR,sizeof(p_product->data));
    //APP_ERROR_CHECK(err_code);

    //p_product->pin_config[0] = LDO_EN;
    //p_product->pin_config[1] = GA1;
    //p_product->pin_config[2] = GA2;
    //p_product->pin_config[3] = GA3;

    //err_code = Flash_CommonStore(&m_product_flash,0,0,p_product->data,sizeof(p_product->data));
    //APP_ERROR_CHECK(err_code);
}

/****************************************************************
** Function name:           Dev_GetBleAdvName
** Descriptions:            获取设备Flash中的蓝牙名称
** input parameters:    
** output parameters:
** Returned value:
** Created by:              WJ             
** Created Date:            2018-10-22
**----------------------------------------------------------------
*****************************************************************/
uint8_t Dev_GetBleAdvName(uint8_t *ble_name)
{
//    uint8_t ble_name_len = 0;
//    uint32_t product_data[PRODUCT_DATA_SIZE] = {0};
//    product_infomation_t *p_product = (product_infomation_t *)product_data;

//    if(ble_name == NULL)
//    {
//        return 0;
//    }
//    
//    /* 读取产品信息区 */
//    Flash_CommonRead(product_data, PRODUCT_INFOMATION_BASE_ADDR, sizeof(product_data));
//    
//    if(product_data->ble_name[0] != 0x00 && product_data->ble_name[0] != 0xFF)
//    {
//        return product_info_s.ble_name;
//    }
//    memset(ble_name, 0, 8);
    return 0;
}

/****************************************************************
** Function name:           Dev_SetBleAdvName
** Descriptions:            设置设备的蓝牙名称
** input parameters:    
** output parameters:
** Returned value:
** Created by:              WJ             
** Created Date:            2018-10-22
**----------------------------------------------------------------
*****************************************************************/
uint8_t Dev_SetBleAdvName(uint8_t *ble_name)
{
    uint32_t err_code = NRF_SUCCESS;
    uint32_t product_data[PRODUCT_DATA_SIZE] = {0};
    product_infomation_t *p_product = (product_infomation_t *)product_data;
    
    if(ble_name[0] != 0x00)
    {   //名称有效
        /* 读取产品信息区 */
        Flash_CommonRead(product_data, PRODUCT_INFOMATION_BASE_ADDR, sizeof(product_data));
        
        memset(p_product->ble_name, 0, sizeof(p_product->ble_name));    

        uint8_t len = 0;//strlen((char *)p_product->ble_name);
        for(uint8_t i = 0; i < 8; i++)
        {
            if(ble_name[i] == 0)
            {
                len = i + 1;
            }
        } 
        len = (len > 8) ? 8:len;
        //比较蓝牙名称，如果一致，不在存储操作
        if(memcmp(p_product->ble_name, ble_name, len) == 0)
        {
            return 1;
        }    
        memcpy(p_product->ble_name, ble_name, len);
        
        err_code = Flash_CommonErase((uint32_t*)PRODUCT_INFOMATION_BASE_ADDR, 1);
        APP_ERROR_CHECK(err_code);     
        err_code = Flash_CommonStore((uint32_t *)PRODUCT_INFOMATION_BASE_ADDR, 
                                     product_data, sizeof(product_data)/4);
        APP_ERROR_CHECK(err_code);      
    }
    else
    {//名称无效
        return 0;
    }
    return 1;
}

/****************************************************************
** Function name:           Dev_CheckBleAdvName
** Descriptions:            检查设备的蓝牙名称
** input parameters:    
** output parameters:
** Returned value:
** Created by:              WJ             
** Created Date:            2018-10-22
**----------------------------------------------------------------
*****************************************************************/
extern uint8_t m_ble_name[10];
void Dev_CheckBleAdvName(void)
{
    uint32_t product_data[PRODUCT_DATA_SIZE] = {0};
    product_infomation_t *p_product = (product_infomation_t *)product_data;

    /* 读取产品信息区 */
    Flash_CommonRead(product_data, PRODUCT_INFOMATION_BASE_ADDR, sizeof(product_data));
    
    if(p_product->ble_name[0] != 0x00)
    {
        for(uint8_t i = 0; i < 8; i++)
        {
            if(p_product->ble_name[i] == 0xFF)
            {
               p_product->ble_name[i] = 0x00; 
            }
        }
        uint8_t len = 0;//strlen((char *)p_product->ble_name);
        for(uint8_t i = 0; i < 8; i++)
        {
            if(p_product->ble_name[i] == 0)
            {
                len = i + 1;
            }
        }
        
        memset(m_ble_name, 0, sizeof(m_ble_name));
        memcpy(m_ble_name, p_product->ble_name, (len > 8) ? 8:len);
    }
}

/****************************************************************
** Function name:           Dev_SetSitFunction
** Descriptions:            设置久坐功能模块
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ            
** Created Date:            2018-11-24
*****************************************************************/
void Dev_SetSitFunction(uint8_t sit_function)
{
    uint32_t err_code = NRF_SUCCESS;
    uint32_t product_data[PRODUCT_DATA_SIZE] = {0};
    product_infomation_t *p_product = (product_infomation_t *)product_data;

    /* 读取产品信息区 */
    Flash_CommonRead(product_data, PRODUCT_INFOMATION_BASE_ADDR, sizeof(product_data));
    
    if(p_product->sit_flag != sit_function)
    {
        p_product->sit_flag = (sit_function==0?0:1);
   
        err_code = Flash_CommonErase((uint32_t*)PRODUCT_INFOMATION_BASE_ADDR, 1);
        APP_ERROR_CHECK(err_code);     
        err_code = Flash_CommonStore((uint32_t*)PRODUCT_INFOMATION_BASE_ADDR, 
                                     product_data, sizeof(product_data)/4);
        APP_ERROR_CHECK(err_code);        
    }    
}

/****************************************************************
** Function name:           Dev_GetSitFunction
** Descriptions:            获取久坐功能模块
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ            
** Created Date:            2018-11-24
*****************************************************************/
uint8_t Dev_GetSitFunction(void)
{
//    uint32_t err_code = NRF_SUCCESS;
    uint32_t product_data[PRODUCT_DATA_SIZE] = {0};
    product_infomation_t *p_product = (product_infomation_t *)product_data;

    /* 读取产品信息区 */
    Flash_CommonRead(product_data, PRODUCT_INFOMATION_BASE_ADDR, sizeof(product_data));

    if(p_product->sit_flag == 0)
    {
        return 0;
    }
    
    return 1;   
}


/****************************************************************
** Function name:           Dev_SetBpFunction
** Descriptions:            设置BP功能模块
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ            
** Created Date:            2018-11-24
*****************************************************************/
void Dev_SetBpFunction(uint8_t bp_function)
{
    uint32_t err_code = NRF_SUCCESS;
    uint32_t product_data[PRODUCT_DATA_SIZE] = {0};
    product_infomation_t *p_product = (product_infomation_t *)product_data;

    /* 读取产品信息区 */
    Flash_CommonRead(product_data, PRODUCT_INFOMATION_BASE_ADDR, sizeof(product_data));
    
    if(p_product->bp_flag != bp_function)
    {
        p_product->bp_flag = (bp_function == 0?0:1);
   
        err_code = Flash_CommonErase((uint32_t*)PRODUCT_INFOMATION_BASE_ADDR, 1);
        APP_ERROR_CHECK(err_code);     
        err_code = Flash_CommonStore((uint32_t*)PRODUCT_INFOMATION_BASE_ADDR, 
                                     product_data, sizeof(product_data)/4);
        APP_ERROR_CHECK(err_code);        
    }    
}

/****************************************************************
** Function name:           Dev_GetBpFunction
** Descriptions:            获取BP功能模块
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ            
** Created Date:            2018-11-24
*****************************************************************/
uint8_t Dev_GetBpFunction(void)
{
//    uint32_t err_code = NRF_SUCCESS;
    uint32_t product_data[PRODUCT_DATA_SIZE] = {0};
    product_infomation_t *p_product = (product_infomation_t *)product_data;

    /* 读取产品信息区 */
    Flash_CommonRead(product_data, PRODUCT_INFOMATION_BASE_ADDR, sizeof(product_data));

    if(p_product->bp_flag == 0)
    {
        return 0;
    }
    
    return 1;   
}
