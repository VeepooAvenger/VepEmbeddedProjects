/***************************Copyright(c)***********************************
**                       Veepoo Tech
**                  COPYRIGHT (C) 2016-2019
**-----------File Info-----------------------------------------------------
** File        name:     user_bootloader.c
** Last modified Date:   2017-08-21
** Last SYS_VERSION:     0.0.0
** Descriptions:            
**-------------------------------------------------------------------------
** Created  by:          WJ
** Created  date:        2017-08-21
** SYS_VERSION:          0.0.0
** Descriptions:         
**-------------------------------------------------------------------------
** Modified by:				
** Modified date:  			          
** SYS_VERSION:                 
** Descriptions:                        
***************************************************************************/
#include "fds.h"
#include "fstorage.h"
#include "nrf_dfu_flash.h"
#include "nrf_dfu_types.h"
#include "string.h"

/*********************************************************************
* Macro Definition
*/
/**
*   #define NRF_DFU_BANK_LAYOUT_DUAL   0x00
*   #define NRF_DFU_BANK_LAYOUT_SINGLE 0x01
*/
//#define   DOUBLE_BACKUP_UPGRADE   //Double backup upgrade

/*********************************************************************
* Global Variables
*/

static nrf_dfu_settings_t s_dfu_settings;
/*********************************************************************
* local functions
*/

// Function prototypes
static void fs_evt_handler(fs_evt_t const * const evt, fs_ret_t result);

FS_REGISTER_CFG(fs_config_t fs_dfu_config) =
{
    .callback       = fs_evt_handler,            // Function for event callbacks.
    .p_start_addr   = (uint32_t*)BOOTLOADER_SETTINGS_ADDRESS,//MBR_SIZE,
    .p_end_addr     = (uint32_t*)BOOTLOADER_SETTINGS_ADDRESS + CODE_PAGE_SIZE
};


static void fs_evt_handler(fs_evt_t const * const evt, fs_ret_t result)
{
    if (evt->p_context)
    {
        //lint -e611
        ((dfu_flash_callback_t)evt->p_context)(evt, result);
    }
}

/************************************************************
****
** Function name:           bootloader_dfu_settings_write_callback
** Descriptions:            write bootloader parameter callback
** input parameters:
** output parameters:
** Returned value:
** Created by:              WJ              
** Created Date:            2016-10-24
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/
static void bootloader_dfu_settings_write_callback(fs_evt_t const * const evt, fs_ret_t result)
{
    if (result == FS_SUCCESS)
    {
        NVIC_SystemReset();     //reset
    }
}

/****************************************************************
** Function name:           crc32_compute
** Descriptions:            
** input parameters:
** output parameters:
** Returned value:
** Created by:              WJ              
** Created Date:            2016-11-17
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/
//static uint32_t crc32_compute(uint8_t const * p_data, uint32_t size, uint32_t const * p_crc)
//{
//    uint32_t crc;

//    crc = (p_crc == NULL) ? 0xFFFFFFFF : ~(*p_crc);
//    for (uint32_t i = 0; i < size; i++)
//    {
//        crc = crc ^ p_data[i];
//        for (uint32_t j = 8; j > 0; j--)
//        {
//            crc = (crc >> 1) ^ (0xEDB88320U & ((crc & 1) ? 0xFFFFFFFF : 0));
//        }
//    }
//    return ~crc;
//}

/****************************************************************
** Function name:           nrf_dfu_settings_calculate_crc
** Descriptions:            
** input parameters:
** output parameters:
** Returned value:
** Created by:              WJ              
** Created Date:            2016-11-17
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/
//static uint32_t nrf_dfu_settings_calculate_crc(void)
//{
//    // the crc is calculated from the s_dfu_settings struct, except the crc itself and the init command
//    return crc32_compute((uint8_t*)&s_dfu_settings + 4, sizeof(nrf_dfu_settings_t) - 4 - sizeof(s_dfu_settings.init_command), NULL);
//}

/****************************************************************
** Function name:           Dfu_JumpToBootloader
** Descriptions:            jump to bootloader from application
** input parameters:
** output parameters:
** Returned value:
** Created by:              WJ              
** Created Date:            2016-10-24
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/
void Dfu_JumpToBootloader(void)
{
    memcpy(&s_dfu_settings, fs_dfu_config.p_end_addr - CODE_PAGE_SIZE, sizeof(s_dfu_settings));   
    if(fs_erase(&fs_dfu_config,fs_dfu_config.p_end_addr - CODE_PAGE_SIZE, 1, NULL) == FS_SUCCESS)
    {
        static nrf_dfu_settings_t temp_dfu_settings;
        memset(&temp_dfu_settings, 0x00, sizeof(nrf_dfu_settings_t));
        s_dfu_settings.enter_buttonless_dfu = 1;
        //在这里单备份和双边份升级操作无效，由boot决定，宏定义无效
#ifdef DOUBLE_BACKUP_UPGRADE
        s_dfu_settings.bank_layout = NRF_DFU_BANK_LAYOUT_DUAL;
#else
        s_dfu_settings.bank_layout = NRF_DFU_BANK_LAYOUT_SINGLE;
#endif
        if(fs_store(&fs_dfu_config, fs_dfu_config.p_end_addr - CODE_PAGE_SIZE, (uint32_t*)&temp_dfu_settings, 
                    sizeof(nrf_dfu_settings_t)/4, bootloader_dfu_settings_write_callback) != FS_SUCCESS)
        {
            
        }
    }
}
