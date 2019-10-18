/***************************Copyright(c)***********************************
**                       Veepoo Tech
**                  COPYRIGHT (C) 2016-2019
**-----------File Info-----------------------------------------------------
** File        name:     gui_update.c
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

#include "gui_update.h"
#include "ext_flash.h"
#include "app_type.h"
#include "app_crc16.h"
#include "watch_dog.h"

/*********************************************************************
* Macro Definition
*/
#define GUI_TIMER_OP_QUEUE_SIZE          20//15                        /**< Size of timer operation queues. */
#define GUI_UPDATE_TIMEOUT_INTERVAL      5000                           /**< System clock process interval (ms). */

/*********************************************************************
* Global Variables
*/  
//static TimerHandle_t m_gui_update_timeout_timer;

gui_data_update_t gui_data_update_s = {0x00,};      //更新UI数据相关的参数
static gui_flash_data_t gui_flash_data_s = {0x00,}; //读取UI数据相关的参数

uint8_t gui_update_time_cnt = 0;                    //更新UI定时器
uint8_t gui_update_time_flag = 0;                   //更新UI定时器标志

extern system_flag_t system_flag_s;
/*********************************************************************
* local functions
*/
extern void Ble_CloseBle(void);


/*******************************************************************************
** Function name:           Gui_SetFlashUiDataRead
** Descriptions:            设置要发送的Flash数据信息
** input parameters:        addr:要发送的起始地址  len:要发送数据的长度
** output parameters:
** Returned value:
** Created by:              YX              
** Created Date:            2017-08-08
**------------------------------------------------------------------------------
** Modified by:
** Modified date:
**------------------------------------------------------------------------------
*******************************************************************************/
void Gui_SetFlashUiDataRead(uint32_t addr, uint32_t len)
{
    gui_flash_data_s.start_addr  = addr;
    gui_flash_data_s.total_len  = len; 
    gui_flash_data_s.read_len = 0;
}

/*******************************************************************************
** Function name:           Gui_UpdateInit
** Descriptions:            更新UI数据的相关参数初始化
** input parameters:        
** output parameters:
** Returned value:
** Created by:              YX              
** Created Date:            2017-08-08
**------------------------------------------------------------------------------
** Modified by:
** Modified date:
**------------------------------------------------------------------------------
*******************************************************************************/
void Gui_UpdateInit(void)
{
	memset(&gui_data_update_s, 0x00, sizeof(gui_data_update_t));
	system_flag_s.update_uimode = 0x00;
}

/*******************************************************************************
** Function name:           Gui_UpdateCheck
** Descriptions:            更新UI数据的相关参数初始化
** input parameters:        p_data：接收到的命令数据
** output parameters:       1：设置成功，进入更新UI数据模式 0：参数有误，进入更新UI数据模式失败
** Returned value:
** Created by:              YX              
** Created Date:            2017-08-08
**------------------------------------------------------------------------------
** Modified by:
** Modified date:
**------------------------------------------------------------------------------
*******************************************************************************/
uint8_t Gui_UpdateCheck(uint8_t *p_data)
{
//    uint32_t Erase_SectorNum = 0;
        
    memset(&gui_data_update_s, 0x00, sizeof(gui_data_update_s));
    
    gui_data_update_s.addr = *(uint32_t *)&p_data[1];
    gui_data_update_s.len = *(uint32_t *)&p_data[5];
    if(gui_data_update_s.addr % EXT_FLASH_4K_NUM == 0x00 
	  /*&& (gui_data_update_s.addr + gui_data_update_s.len) <= 0x100000 */
      && gui_data_update_s.len > 0)
    {        
        system_flag_s.update_uimode = 0x01;
        //WJ.20170821.写的时候不做擦除
//        Erase_SectorNum = gui_data_update_s.len / EXT_FLASH_4K_NUM + ((gui_data_update_s.len % EXT_FLASH_4K_NUM > 0x00) ? 1:0);
//        for(uint32_t i = 0; i < Erase_SectorNum; i++)
//        {
//            Extflash_SectorErase(gui_data_update_s.addr / EXT_FLASH_4K_NUM + i);
//            nrf_delay_ms(100);
//        }
		return 1;
    }      
	system_flag_s.update_uimode = 0x00;
	return 0;
}

/*******************************************************************************
** Function name:           Gui_UpdateFlash
** Descriptions:            更新FLASH中UI的数据
** input parameters:        p_data：接收到的数据
** output parameters:       1：接受到所有的数据，此次操作结束 0：还有数据没有接受完
** Returned value:
** Created by:              YX              
** Created Date:            2017-08-08
**------------------------------------------------------------------------------
** Modified by:
** Modified date:
**------------------------------------------------------------------------------
*******************************************************************************/
uint8_t Gui_UpdateFlash(uint8_t *p_data)
{
    Gui_UpdateTimeoutStop();
    for(uint8_t i = 0; i < 20; i++)
    {
        gui_data_update_s.cache_buf[gui_data_update_s.buf_cnt+i] = p_data[i];
    }
    gui_data_update_s.revice_sum += 20;
    gui_data_update_s.buf_cnt += 20;
    if(gui_data_update_s.revice_sum >= gui_data_update_s.len)
    {
        Extflash_Write(gui_data_update_s.addr + gui_data_update_s.now_sub * 200,
                      (gui_data_update_s.len%200 != 0?gui_data_update_s.len%200:200), gui_data_update_s.cache_buf);
        nrf_delay_ms(100);
        system_flag_s.update_uimode = 0x00;
		return 1;
    }
    else
    {
        Gui_UpdateTimeoutStart();
        if(gui_data_update_s.buf_cnt >= 200)
        {
            for(uint8_t j = 0; j < 200; j++)
            {
                gui_data_update_s.store_buf[j] = gui_data_update_s.cache_buf[j];
            }
            gui_data_update_s.buf_cnt = 0;
            Extflash_Write(gui_data_update_s.addr + gui_data_update_s.now_sub * 200, 200,
                           gui_data_update_s.store_buf);
            gui_data_update_s.now_sub++;
        }
    }
	return 0;
}

/*******************************************************************************
** Function name:           Gui_EraseUiData
** Descriptions:            擦除UI数据
** input parameters:        p_data：接收到的命令数据
** output parameters:       1：擦除成功 0：擦除失败
** Returned value:
** Created by:              YX              
** Created Date:            2017-08-08
**------------------------------------------------------------------------------
** Modified by:
** Modified date:
**------------------------------------------------------------------------------
*******************************************************************************/
uint8_t Gui_EraseUiData(uint8_t *p_data)
{
    uint16_t erase_start_num = 0;
     
    erase_start_num = *(uint16_t*)&p_data[2];
    if(p_data[1] == 0x00)
    {
        return Extflash_SectorErase(erase_start_num);
    }
    else if(p_data[1] == 0x01)
    {
        if(erase_start_num == 0xFEFE)
        {
            return Extflash_AllErase();
        }
        else
        {
            return Extflash_BlockErase(erase_start_num);
        }
    }
    
    return 0;       
}

/*******************************************************************************
** Function name:           Gui_ReadUiData
** Descriptions:            读取UI数据的相关参数初始化
** input parameters:        p_data：接收到的命令数据
** output parameters:       1：设置成功，进入读取UI数据模式 0：参数有误，进入读取UI数据模式失败
** Returned value:
** Created by:              YX              
** Created Date:            2017-08-08
**------------------------------------------------------------------------------
** Modified by:
** Modified date:
**------------------------------------------------------------------------------
*******************************************************************************/
uint8_t Gui_ReadUiData(uint8_t * p_data)
{
	uint32_t data_start_addr = 0;
    uint32_t data_total_len = 0;
    
    system_flag_s.read_uimode = 0x01;
    data_start_addr = *(uint32_t *)&p_data[1];
    data_total_len = *(uint32_t *)&p_data[5];
    if(data_start_addr + data_total_len <= 0x100000 && data_total_len > 0)
    {
        Gui_SetFlashUiDataRead(data_start_addr, data_total_len);
        App_SetSendFlashDataType(FLASH_DATA_TYPE_UI);
        App_AddEvent(APP_SEND_FLASH_DATA_EVENT);
        return 1;
    }
	
    return 0;
}

/*******************************************************************************
** Function name:           GUI_SendUiDataHandler
** Descriptions:            发送UI的数据
** input parameters:        
** output parameters:       0：发送完成 1：还有数据要发送
** Returned value:
** Created by:              YX              
** Created Date:            2017-08-08
**------------------------------------------------------------------------------
** Modified by:
** Modified date:
**------------------------------------------------------------------------------
*******************************************************************************/
uint8_t GUI_SendUiDataHandler(void)
{
    uint8_t ble_ack[20] = {0xC6,0x02,};
    
    Extflash_Read(gui_flash_data_s.start_addr+gui_flash_data_s.read_len, 20, ble_ack);
    Ble_SendDataViaBat(ble_ack, 20);
    gui_flash_data_s.read_len += 20;
    
    if(system_flag_s.ble_connect == 0x00)
    {
        return 0;
    }
    
    if(system_flag_s.read_uimode == 0x01 && gui_flash_data_s.read_len < gui_flash_data_s.total_len)
    {
        return 1;
    }
    else if(system_flag_s.read_uimode == 0x01 && gui_flash_data_s.read_len >= gui_flash_data_s.total_len)
    {
        ble_ack[0] = 0xC6;
        ble_ack[1] = 0x02;
        Ble_SendDataViaBat(ble_ack, 20);
    }
    else
    {
        ble_ack[0] = 0xC6;
        ble_ack[1] = 0x00;
        Ble_SendDataViaBat(ble_ack, 20);
    }
    
    return 0;
}


void Gui_UpdateTimeoutStart(void)
{
    gui_update_time_flag = 1;
    gui_update_time_cnt = 0;
}

void Gui_UpdateTimeoutStop(void)
{
    gui_update_time_flag = 0;
    gui_update_time_cnt = 0;
}


void Gui_UpdateTimeoutHandler(void)
{
    if(system_flag_s.update_uimode == 1)
    {
        if(gui_update_time_flag == 1)
        {
            gui_update_time_cnt++;
            if(gui_update_time_cnt > 5)
            {
                //接受UI数据超时
                Ble_CloseBle();
                system_flag_s.update_uimode = 0x00;
                gui_update_time_cnt = 0;
            }
        }
    }
    
}

void Gui_UpdateTimeoutInit(void)
{  
    gui_update_time_cnt = 0;
    gui_update_time_flag = 0;
}

/*******************************************************************************
** Function name:           Gui_UiCrcCheck
** Descriptions:            ui crc 校验 
** input parameters:        p_data：接收到的命令数据
** output parameters:       无
** Returned value:          0：校验失败；1：校验成功
** Created by:              WJ              
** Created Date:            2018-04-08
*******************************************************************************/
uint8_t Gui_UiCrcCheck(const uint16_t crc_value, const uint32_t ui_len, uint16_t *out_crc)
{
    uint16_t ui_crc = 0xFFFF;
	uint32_t ui_start_addr = 0;
    uint32_t ui_read_len = 0;
    uint8_t  ui_data_buf[128] = {0x00,};

    if(ui_len <= 0)
    {
        return 0;
    }
    
    ui_read_len = 128; 
    ui_crc = 0xFFFF; 
    
    while(1)
    {
        Extflash_Read(ui_start_addr, ui_read_len, ui_data_buf);
        ui_crc = App_Crc16ByCrc16(ui_crc, ui_data_buf, ui_read_len);
        ui_start_addr += ui_read_len;
        if(ui_start_addr + ui_read_len < ui_len)
        {
            ui_read_len = 128;
        }
        else if(ui_start_addr < ui_len)
        {
            ui_read_len = ui_len - ui_start_addr;
        }
        else
        {
            break;
        }
        Wdt_Reload();
    }
      
    *out_crc = ui_crc;
    //校验通过
    if(crc_value == ui_crc)
    {
        return 1;
    }
	
    return 0;
}


