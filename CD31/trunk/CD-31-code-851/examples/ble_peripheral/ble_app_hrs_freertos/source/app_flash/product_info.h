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

typedef struct
{
    uint8_t master_sn[4];               //设备识别码1
    uint8_t company_name[8];            //公司名称
    uint8_t product_version[8];         //产品型号
    uint8_t hardware_version[4];        //
    uint8_t firmware_version[4];        //
    uint8_t product_date[4];            //
    uint8_t agent_id[8];                //
    uint8_t product_batch[8];           //
    uint8_t product_count[8];           //
    uint8_t slave_sn[4];                //
    uint8_t product_modify[4];          //

    uint8_t bootloader_version[8];      //boot版本
    uint8_t application_version[8];     //app版本

    uint8_t pin_config[4];              //DFU模式下闪灯配置
    uint8_t blue_name[8];               //蓝牙名称
    uint8_t bp_flag;                    //血压支持
    uint8_t retain[3];                  //保留   
}product_attr_t;

typedef union
{
    product_attr_t  attr;
    uint32_t  data[24];
}product_info_t;

/* 设备sn序列号过滤处理 */
void Product_InfoCheck(void);

//获取产品SN
uint32_t Product_GetSnInfo(void);

//获取boot版本
uint32_t Product_GetBootVesion(void);

//获取设备Flash中的蓝牙名称
uint8_t *Product_GetBleAdvName(void);

//设置设备的蓝牙名称
uint8_t Product_SetBleAdvName(uint8_t* Name);

//检查设备的蓝牙名称
void Product_CheckBleAdvName(void);

void Product_SetBpFunction(uint8_t bp_function);
uint8_t Product_GetBpFunctionState(void);
#endif
