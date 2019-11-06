#ifndef _CONFIGURATION_H_
#define _CONFIGURATION_H_

#include <stdint.h>


/*
 *==========================================================
 *                  【软件版本信息】
 *==========================================================
 */
#define VERSION_M                        0x00     
#define VERSION_H                        0x08   
#define VERSION_L                        0x00
#define VERSION_T                        0x04

/*
 *==========================================================
 *                  【设备ID号】 
 *==========================================================
 */
#define VEEPOO_DEVICE_ID_H               0      // 设备号高8位,不使用，直接使用下面的
#define VEEPOO_DEVICE_ID_L               1006   // 设备号低8位

/*
 *==========================================================
 *                  【产品生产信息】
 *==========================================================
 */
#define COMPANY                          0x5650        //VP
#define DEVICE_TYPE                      0x452D3038    //E-08
#define HARDWARE_TYPE                    0x11          //V1.1
#define SOFTWARE_TYPE                    0x0800        //V08.00
#define PRODUCT_DATE                     0x120C13      //2018.12.19
#define AGENCY_SERIAL_NUMBER             0x005650      //
#define PRODUCT_BATCH                    0x1300        //1900
#define PRODUCT_NUMBER                   0x000000      //000000

/*
 *==========================================================
 *                  【外部FLASH头部信息】
 *==========================================================
 */
#define FLASH_HEAD_LEN                  0x000E              //14
#define FLASH_HEAD_STR                  0x00564545504F4F00  //VEEPOO
#define FLASH_PROJ_TYPE                 0x4500              //E系列
#define FLASH_VERSION_H                 0x01                //
#define FLASH_VERSION_L                 0x00                //

/*
 *==========================================================
 *                  【日志模块使能开关】
 *==========================================================
 */
#define TRACE_DEBUG_ENABLE              0    // 跟踪调试信息
 
#define ALGORITHM_LOG_ENABLE            0
#define COMMUNICATION_LOG_ENABLE        0
#define GUI_LOG_ENABLE                  0
#define BLE_LOG_ENABLE                  0
#define IDEL_LOG_ENABLE                 0

#define AFE4900_LOG_ENABLE              0
#define SLEEP_LOG_ENABLE                0
#define FLASH_LOG_ENABLE                0

/*
 *==========================================================
 *           【BAT和CLK服务数据重发参数配置】
 *==========================================================
 */
#define CLK_DATA_TIMEOUT_REPEAT_CNT     20        // CLK服务蓝牙上传超时等待包数
#define BAT_DATA_TIMEOUT_REPEAT_CNT     20        // BAT服务蓝牙上传超市等待包数
#define UPLOAD_RESPONSE_SPLIT_CNT       3         // 快速响应HBand指令分割包数

#endif
