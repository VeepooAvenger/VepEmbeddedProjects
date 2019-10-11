/***************************Copyright(c)***********************************
**                       Veepoo Tech
**                  COPYRIGHT (C) 2016-2019
**-----------File Info-----------------------------------------------------
** File        name:     gui_update.h
** Last modified Date:   2017-08-08
** Last SYS_VERSION:     0.0.0
** Descriptions:            
**-------------------------------------------------------------------------
** Created  by:          YX
** Created  date:        2017-08-08
** SYS_VERSION:          0.0.0
** Descriptions:         
**-------------------------------------------------------------------------
** Modified by:				
** Modified date:  			          
** SYS_VERSION:                 
** Descriptions:                        
***************************************************************************/
#ifndef _GUI_UPDATE_H_
#define _GUI_UPDATE_H_

#include <stdint.h>

typedef struct
{	
    uint8_t     cache_buf[200]; //缓存数据
    uint8_t     store_buf[200]; //存储数据
    uint8_t     buf_cnt;        //数据的长度
    uint32_t    now_sub;        //当前存储的下标组数
    uint32_t    revice_sum;     //已经接收数据的总长度
    uint32_t    addr;           //开始地址
    uint32_t    len;            //数据的总长度
}gui_data_update_t;

typedef struct {
    uint32_t start_addr;         //开始地址 
    uint32_t total_len;          //总长度
    uint32_t read_len;           //已经读取的长度
}gui_flash_data_t; 

/* 更新UI数据的相关参数初始化 */
void Gui_UpdateInit(void);
/* 更新UI数据的相关参数初始化 */
uint8_t Gui_UpdateCheck(uint8_t * p_data);
/* 更新FLASH中UI的数据 */
uint8_t Gui_UpdateFlash(uint8_t * p_data);
/* 擦除UI数据 */
uint8_t Gui_EraseUiData(uint8_t * p_data);
/* 读取UI数据的相关参数初始化 */
uint8_t Gui_ReadUiData(uint8_t * p_data);
/* 接受数据超时定时器初始化 */
void Gui_UpdateTimeoutInit(void);
/* 接受数据超时定时器关闭 */
void Gui_UpdateTimeoutStop(void);
/* 接受数据超时定时器启动 */
void Gui_UpdateTimeoutStart(void);
/* 定时器计数处理 */
void Gui_UpdateTimeoutHandler(void);
/* 发送UI的数据 */
uint8_t GUI_SendUiDataHandler(void);

//crc校验
uint8_t Gui_UiCrcCheck(const uint16_t crc_value, const uint32_t ui_len, uint16_t *out_crc);

#endif

