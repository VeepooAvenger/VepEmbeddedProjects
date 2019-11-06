/*******************Copyright(c)*********************************
**                  Veepoo Tech
**             COPYRIGHT (C) 2016-2019
**----------------------FileInfo---------------------------------
** File name: 				       log.h
** Last modified Date: 		2017-08-11
** Last SYS_VERSION: 		  0.0.1
** Descriptions:
**---------------------------------------------------------------
** Createdby: 				       郭涛
** Createddate: 			      2017-08-11
** SYS_VERSION: 			      0.0.1
** Descriptions: 			     日志打印宏定义文件
**---------------------------------------------------------------
** Modified by:				
** Modified date:			
** SYS_VERSION:				
** Modified Descriptions:	
****************************************************************/
#ifndef _LOG_H_
#define _LOG_H_

#include <string.h>
#include "api_flash.h"
#include "configuration.h"

// 算法日志定义
#if (ALGORITHM_LOG_ENABLE)
    #define ALGORIGTHM_LOG_PRINT(format,...) \
    { \
        memset(p_data_manage->log_attr_s.text,0,64);  \
        p_data_manage->log_attr_s.text[0] = clock->month/10+48; \
        p_data_manage->log_attr_s.text[1] = clock->month%10+48; \
        p_data_manage->log_attr_s.text[2] = 45; \
        p_data_manage->log_attr_s.text[3] = clock->day/10+48; \
        p_data_manage->log_attr_s.text[4] = clock->day%10+48; \
        p_data_manage->log_attr_s.text[5] = 0; \
        p_data_manage->log_attr_s.text[6] = clock->hour/10+48; \
        p_data_manage->log_attr_s.text[7] = clock->hour%10+48; \
        p_data_manage->log_attr_s.text[8] = 58; \
        p_data_manage->log_attr_s.text[9] = clock->minutes/10+48; \
        p_data_manage->log_attr_s.text[10] = clock->minutes%10+48; \
        p_data_manage->log_attr_s.text[11] = 58; \
        p_data_manage->log_attr_s.text[12] = clock->seconds/10+48; \
        p_data_manage->log_attr_s.text[13] = clock->seconds%10+48; \
        snprintf((char*)&p_data_manage->log_attr_s.text[14],50,format,##__VA_ARGS__); \
        Api_WriteLogText((uint8_t*)p_data_manage->log_attr_s.text,64); \
    }
#else
    #define ALGORIGTHM_LOG_PRINT(format,...)
#endif

// 通信日志定义
#if (COMMUNICATION_LOG_ENABLE)
    #define COMMUNICATION_LOG_PRINT(format,...) \
    { \
        memset(p_data_manage->log_attr_s.text,0,64);  \
        p_data_manage->log_attr_s.text[0] = 91; \
        p_data_manage->log_attr_s.text[1] = clock->hour/10+48; \
        p_data_manage->log_attr_s.text[2] = clock->hour%10+48; \
        p_data_manage->log_attr_s.text[3] = 58; \
        p_data_manage->log_attr_s.text[4] = clock->minutes/10+48; \
        p_data_manage->log_attr_s.text[5] = clock->minutes%10+48; \
        p_data_manage->log_attr_s.text[6] = 58; \
        p_data_manage->log_attr_s.text[7] = clock->seconds/10+48; \
        p_data_manage->log_attr_s.text[8] = clock->seconds%10+48; \
        p_data_manage->log_attr_s.text[9] = 76; \
        p_data_manage->log_attr_s.text[10] = 67; \
        p_data_manage->log_attr_s.text[11] = 111; \
        p_data_manage->log_attr_s.text[12] = 109; \
        p_data_manage->log_attr_s.text[13] = 93; \
        p_data_manage->log_attr_s.text[14] = 0; \
        snprintf((char*)&p_data_manage->log_attr_s.text[15],49,format,##__VA_ARGS__); \
        Api_WriteLogText((uint8_t*)p_data_manage->log_attr_s.text,64); \
    }
#else
    #define COMMUNICATION_LOG_PRINT(format,...)
#endif

// GUI日志定义
#if (GUI_LOG_ENABLE)
    #define GUI_LOG_PRINT(format,...) \
    { \
        memset(p_data_manage->log_attr_s.text,0,64);  \
        p_data_manage->log_attr_s.text[0] = 91; \
	 p_data_manage->log_attr_s.text[1] = clock->hour/10+48; \
        p_data_manage->log_attr_s.text[2] = clock->hour%10+48; \
        p_data_manage->log_attr_s.text[3] = 58; \
        p_data_manage->log_attr_s.text[4] = clock->minutes/10+48; \
        p_data_manage->log_attr_s.text[5] = clock->minutes%10+48; \
        p_data_manage->log_attr_s.text[6] = 58; \
        p_data_manage->log_attr_s.text[7] = clock->seconds/10+48; \
        p_data_manage->log_attr_s.text[8] = clock->seconds%10+48; \
        p_data_manage->log_attr_s.text[9] = 76; \
        p_data_manage->log_attr_s.text[10] = 117; \
        p_data_manage->log_attr_s.text[11] = 105; \
        p_data_manage->log_attr_s.text[12] = 93; \
        p_data_manage->log_attr_s.text[13] = 0; \
        snprintf((char*)&p_data_manage->log_attr_s.text[14],50,format,##__VA_ARGS__); \
        Api_WriteLogText((uint8_t*)p_data_manage->log_attr_s.text,64); \
    }
#else
    #define GUI_LOG_PRINT(format,...)
#endif
    
    // GUI日志定义
#if (BLE_LOG_ENABLE)
    #define BLE_LOG_PRINT(format,...) \
    { \
        memset(p_data_manage->log_attr_s.text,0,64);  \
        p_data_manage->log_attr_s.text[0] = clock->hour/10+48; \
        p_data_manage->log_attr_s.text[1] = clock->hour%10+48; \
        p_data_manage->log_attr_s.text[2] = 58; \
        p_data_manage->log_attr_s.text[3] = clock->minutes/10+48; \
        p_data_manage->log_attr_s.text[4] = clock->minutes%10+48; \
        p_data_manage->log_attr_s.text[5] = 58; \
        p_data_manage->log_attr_s.text[6] = clock->seconds/10+48; \
        p_data_manage->log_attr_s.text[7] = clock->seconds%10+48; \
        p_data_manage->log_attr_s.text[8] = 91; \
        p_data_manage->log_attr_s.text[9] = 66; \
        p_data_manage->log_attr_s.text[10] = 76; \
        p_data_manage->log_attr_s.text[11] = 69; \
        p_data_manage->log_attr_s.text[12] = 93; \
        p_data_manage->log_attr_s.text[13] = 0; \
        snprintf((char*)&p_data_manage->log_attr_s.text[14],50,format,##__VA_ARGS__); \
        Api_WriteLogText((uint8_t*)p_data_manage->log_attr_s.text,64); \
    }
#else
    #define BLE_LOG_PRINT(format,...)
#endif
    
    // GUI日志定义
#if (IDEL_LOG_ENABLE)
    #define IDEL_LOG_PRINT(format,...) \
    { \
        memset(p_data_manage->log_attr_s.text,0,64);  \
	 p_data_manage->log_attr_s.text[0] = clock->hour/10+48; \
        p_data_manage->log_attr_s.text[1] = clock->hour%10+48; \
        p_data_manage->log_attr_s.text[2] = 58; \
        p_data_manage->log_attr_s.text[3] = clock->minutes/10+48; \
        p_data_manage->log_attr_s.text[4] = clock->minutes%10+48; \
        p_data_manage->log_attr_s.text[5] = 58; \
        p_data_manage->log_attr_s.text[6] = clock->seconds/10+48; \
        p_data_manage->log_attr_s.text[7] = clock->seconds%10+48; \
        p_data_manage->log_attr_s.text[8] = 91; \
        p_data_manage->log_attr_s.text[9] = 73; \
        p_data_manage->log_attr_s.text[10] = 100; \
        p_data_manage->log_attr_s.text[11] = 101; \
	 p_data_manage->log_attr_s.text[12] = 108; \
        p_data_manage->log_attr_s.text[13] = 93; \
        p_data_manage->log_attr_s.text[14] = 0; \
        snprintf((char*)&p_data_manage->log_attr_s.text[15],49,format,##__VA_ARGS__); \
        Api_WriteLogText((uint8_t*)p_data_manage->log_attr_s.text,64); \
    }
#else
    #define IDEL_LOG_PRINT(format,...)
#endif

//        memset(p_data_manage->log_attr_s.text,0,64);  \
//        p_data_manage->log_attr_s.text[0] = 91; \
//        p_data_manage->log_attr_s.text[1] = 65; \
//        p_data_manage->log_attr_s.text[2] = 70; \
//        p_data_manage->log_attr_s.text[3] = 69; \
//        p_data_manage->log_attr_s.text[4] = 52; \
//        p_data_manage->log_attr_s.text[5] = 52; \
//        p_data_manage->log_attr_s.text[6] = 48; \
//        p_data_manage->log_attr_s.text[7] = 52; \
//        p_data_manage->log_attr_s.text[8] = 93; \
//        p_data_manage->log_attr_s.text[9] = 0; \
    
// AFE4900 日志定义
#if (AFE4900_LOG_ENABLE)
    #define AFE4900_LOG_PRINT(format,...) \
    { \
        memset(p_data_manage->log_attr_s.text,0,64);  \
        p_data_manage->log_attr_s.text[0] = clock->hour/10+48; \
        p_data_manage->log_attr_s.text[1] = clock->hour%10+48; \
        p_data_manage->log_attr_s.text[2] = 58; \
        p_data_manage->log_attr_s.text[3] = clock->minutes/10+48; \
        p_data_manage->log_attr_s.text[4] = clock->minutes%10+48; \
        p_data_manage->log_attr_s.text[5] = 58; \
        p_data_manage->log_attr_s.text[6] = clock->seconds/10+48; \
        p_data_manage->log_attr_s.text[7] = clock->seconds%10+48; \
        p_data_manage->log_attr_s.text[8] = 91; \
        p_data_manage->log_attr_s.text[9] = 65; \
        p_data_manage->log_attr_s.text[10] = 70; \
        p_data_manage->log_attr_s.text[11] = 69; \
        p_data_manage->log_attr_s.text[12] = 93; \
        p_data_manage->log_attr_s.text[13] = 0; \
        snprintf((char*)&p_data_manage->log_attr_s.text[14],50,format,##__VA_ARGS__); \
        Api_WriteLogText((uint8_t*)p_data_manage->log_attr_s.text,64); \
    }
#else
    #define AFE4900_LOG_PRINT(format,...)
#endif

// AFE4900 日志定义
#if (SLEEP_LOG_ENABLE)
    #define SLEEP_LOG_PRINT(format,...) \
    { \
        memset(p_data_manage->log_attr_s.text,0,64);  \
        p_data_manage->log_attr_s.text[0] = 91; \
        p_data_manage->log_attr_s.text[1] = 83; \
        p_data_manage->log_attr_s.text[2] = 108; \
        p_data_manage->log_attr_s.text[3] = 101; \
        p_data_manage->log_attr_s.text[4] = 101; \
        p_data_manage->log_attr_s.text[5] = 112; \
        p_data_manage->log_attr_s.text[6] = 93; \
        p_data_manage->log_attr_s.text[7] = 0; \
        snprintf((char*)&p_data_manage->log_attr_s.text[8],56,format,##__VA_ARGS__); \
        Api_WriteLogText((uint8_t*)p_data_manage->log_attr_s.text,64); \
    }
#else
    #define SLEEP_LOG_PRINT(format,...)
#endif

// AFE4900 日志定义
#if (FLASH_LOG_ENABLE)
    #define FLASH_LOG_PRINT(format,...) \
    { \
        memset(p_data_manage->log_attr_s.text,0,64);  \
        p_data_manage->log_attr_s.text[0] = 91; \
        p_data_manage->log_attr_s.text[1] = 70; \
        p_data_manage->log_attr_s.text[2] = 108; \
        p_data_manage->log_attr_s.text[3] = 97; \
        p_data_manage->log_attr_s.text[4] = 115; \
        p_data_manage->log_attr_s.text[5] = 104; \
        p_data_manage->log_attr_s.text[6] = 93; \
        p_data_manage->log_attr_s.text[7] = 0; \
        snprintf((char*)&p_data_manage->log_attr_s.text[8],56,format,##__VA_ARGS__); \
        Api_WriteLogText((uint8_t*)p_data_manage->log_attr_s.text,64); \
    }
#else
    #define FLASH_LOG_PRINT(format,...)
#endif













#endif

