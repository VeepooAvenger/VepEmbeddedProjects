/*************************************Copyright(c)***************************************************
**                                     Veepoo Tech
**                  
**                               COPYRIGHT(C)  2016-2019
**--------------------------------------FileInfo----------------------------------------------------
** FileName:                    Flash_common.c
** Last Modify Data:            2017-08-04
** Last SYS_VERSION:            XXXXXXXXX
** Descriptions:                contain all flash opration  Api                
**---------------------------------------------------------------------------------------------------
** CreatedBy:                   GT
** CreateData:                  2017-06-xx
** SystemVersion:             
** Description:                 the file's  funtion  realize the flash read , write and erase at the same time 
**                              register the flash system from address 0x5c000  to 0x74000 can be  operation
**---------------------------------------------------------------------------------------------------
** Modify By:                       
** Modify Data:
** System Version:
** Modify Description:    

******************************************************************************************************/
#include <string.h>
#include "flash_common.h"
#include "softdevice_handler.h"
#include "fstorage.h"
#include "flash_data_type.h"

/************************************************************************
** Function name:        fs_evt_handler
** Descriptions:         ��
**                       
** input parameter:      ��
** Returned Value:
** Created  by:          ���� 
** Created  Datas:       2017-08-04
**-----------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
**************************************************************************/
static void fs_evt2_handler(fs_evt_t const * const evt, fs_ret_t result);
FS_REGISTER_CFG(fs_config_t fs_dfu_config) =
{
    .callback       = fs_evt2_handler,           
    .p_start_addr   = (uint32_t*)LOG_BASE_ADDR, 
    .p_end_addr     = (uint32_t*)0x74000, 
    .num_pages      = ((0x74000-LOG_BASE_ADDR)/0x1000), 
    .priority       = 0xFE, 
};

static void fs_evt2_handler(fs_evt_t const * const evt, fs_ret_t result)
{
}
/************************************************************************
** Function name:        Flash_CommonInit
** Descriptions:         flash��ʼ�� 
** input parameter:      ��
** Returned Value:       NRF_SUCCESS
** Created  by:          ����
** Created  Datas:       2017-08-04
**-----------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
**************************************************************************/
uint32_t Flash_CommonInit(void)
{
    uint32_t err_code = NRF_SUCCESS;

    if (fs_fake_init() != FS_SUCCESS)
    {
        return NRF_ERROR_INVALID_STATE;
    }
    // Enable access to the whole range
    err_code = softdevice_sys_evt_handler_set(fs_sys_event_handler);
    if (err_code != NRF_SUCCESS)
    {
        return NRF_ERROR_INVALID_STATE;
    }
    return err_code;
}

/*************************************************************************
** Function name:         Flash_CommonStore
** Descriptions:          flash�洢
** input parameter:       p_dest: Ŀ�ĵ�ַ  P_src :Դ��ַ len_words:����
** Returned Value:        FS_SUCCESS
** Created  by:           ����
** Created  Datas:        2017-08-04
**-----------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
**************************************************************************/
fs_ret_t Flash_CommonStore(uint32_t const * p_dest, uint32_t const * const p_src, uint32_t len_words)
{
    fs_ret_t ret_val = FS_SUCCESS;
    ret_val = fs_store(&fs_dfu_config, p_dest, p_src, len_words, NULL);
    if (ret_val != FS_SUCCESS)
    {
        return ret_val;
    }
    while(!fs_queue_is_empty())
    {
        intern_softdevice_events_execute();
    }

    return ret_val;
}

/************************************************************************
** Function name:       Flash_CommonErase
** Descriptions:        ��������
** input parameter:     p_dest:  Ŀ�ĵ�ַ  num_pages: ҳ��
** Returned Value:      FS_SUCCESS
** Created  by:         ����
** Created  Datas:      2017-08-04
**-----------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
**************************************************************************/
fs_ret_t Flash_CommonErase(uint32_t const * p_dest, uint32_t num_pages)
{
    fs_ret_t ret_val = FS_SUCCESS;
    ret_val = fs_erase(&fs_dfu_config, p_dest, num_pages, NULL);
    if (ret_val != FS_SUCCESS)
    {
        return ret_val;
    }
    
    while(!fs_queue_is_empty())
    {
        intern_softdevice_events_execute();
    }
    
    return ret_val;
}

/************************************************************************
** Function name:       Flash_CommonRead
** Descriptions:        ��flash 
** input parameter:     p_dest: Ŀ�ĵ�ַ   addr: Դ��ַ
**                      bytes:�ֽ���
** Returned Value:      FS_SUCCESS
** Created  by:         ����
** Created  Datas:      2017-08-04
**-----------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
**************************************************************************/
fs_ret_t Flash_CommonRead(uint32_t const *p_data, uint32_t addr, uint32_t bytes)
{
    if (((uint32_t)p_data & 0x03) || (addr & 0x03))
    {
        return FS_ERR_UNALIGNED_ADDR;
    }    
    memcpy((void*)p_data, (void*)addr, bytes);
    return FS_SUCCESS;
}

/************************************************************************
** Function name:             Flash_CommonUpdate
** Descriptions:              ��
** input parameter:           
**                                                 
** Returned Value:
** Created  by:               ����
** Created  Datas:            2017-08-04
**-----------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
**************************************************************************/
fs_ret_t Flash_CommonUpdate(uint32_t const * p_dest, uint32_t const * const p_src, uint32_t len_words)
{
    return FS_SUCCESS;
}

