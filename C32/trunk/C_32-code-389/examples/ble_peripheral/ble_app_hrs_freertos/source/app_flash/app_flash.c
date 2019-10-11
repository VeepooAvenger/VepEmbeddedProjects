/***************************Copyright(c)***********************************
**                       Veepoo Tech
**                  COPYRIGHT (C) 2016-2019
**-----------File Info-----------------------------------------------------
** File        name:     app_flash.c
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
#include "fds.h"
#include "fstorage.h"
#include "app_type.h"
#include "fds.h"
#include "app_flash.h"

/*********************************************************************
* Macro Definition
*/

#define PAGE_SIZE_WORDS     1024                    // 4*1024 Byte, The size of each page
#define FLASH_MAX_ADDR      (500 * 1024)

// system data flash
#define SYSTEM_DATA_EACH_WORDS_SIZE     0x30        // system data word size (4 Byte) 



/*********************************************************************
* Global Variables

*/
const system_record_info_t system_record_s[RECORD_MAX_SIZE] = {
    {0x1111,0x2222},    //RECORD_SYSTEM_DATA, Save the system parameters
    {0x1111,0x3333},     //RECORD_SYSTEM_TIME, Save the system time
    {0x1111,0x4444}     //RECORD_SYSTEM_SWITCH, Save the system switch
};

/*********************************************************************
* local functions
*/
static void App_FlashWriteCallback(fs_evt_t const *const evt, fs_ret_t result);

// Our fstorage configuration.
FS_REGISTER_CFG(fs_config_t app_fs_config) =
{
    .callback  = App_FlashWriteCallback,
    .num_pages = ALL_FLASH_PAGE_SIZE,
    // We register with the highest priority in order to be assigned
    // the pages with the highest memory address (closest to the bootloader).
    .priority  = 0xFE
};

/****************************************************************
** Function name:           
** Descriptions:                 
** input parameters:             
** output parameters:       
** Returned value:
** Created by:              WJ              
** Created Date:            2016-09-12
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
static void App_FlashWriteCallback(fs_evt_t const *const evt, fs_ret_t result)
{
    if (result != FS_SUCCESS)
    {
    }
    if(evt->p_context)
    {
        ((app_flash_callback_t)(evt->p_context))(result);
    }
}

static fs_ret_t App_FlashWriteData(fs_config_t const * const p_config,
                                   uint32_t    const * const p_dest,
                                   uint32_t    const * const p_src,
                                   uint16_t    const         length_words,
                                   void *                    p_context)
{
    return fs_store(p_config, p_dest, p_src, length_words, p_context);
}

static fs_ret_t App_FlashErasePage(fs_config_t const * const p_config,
                                   uint32_t    const * const p_page_addr,
                                   uint16_t    const         num_pages,
                                   void *                    p_context)
{
    
    return fs_erase(p_config, p_page_addr, num_pages, p_context);
}

/****************************** Daily data ******************************/
/****************************************************************
** Function name:           App_FlashGetDailyDataAddressOfPage
** Descriptions:            Daily data storage offset
** input parameters:
** output parameters:
** Returned value:
** Created by:              WJ              
** Created Date:            2016-10-19
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/
static uint32_t const *App_FlashGetDailyDataAddressOfPage(uint8_t block_count)
{
    return app_fs_config.p_start_addr + DAILY_DATA_BLOCK_START_PAGE * PAGE_SIZE_WORDS
           + (block_count * DAILY_DATA_BLOCK_UINT * PAGE_SIZE_WORDS);
}

/****************************************************************
** Function name:           App_FlashGetDailyDataOffset
** Descriptions:            Daily data storage offset
** input parameters:
** output parameters:
** Returned value:
** Created by:              WJ              
** Created Date:            2016-10-19
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/
static uint32_t App_FlashGetDailyDataOffset(uint8_t block_count, uint16_t number )
{
    return  DAILY_DATA_BLOCK_START_PAGE * PAGE_SIZE_WORDS +
            + block_count * DAILY_DATA_BLOCK_SIZE + number * DAILY_DATA_EACH_WORDS_SIZE;
}

/****************************************************************
** Function name:           App_FlashReadDailyData
** Descriptions:            Read the daily data
** input parameters:
** output parameters:
** Returned value:
** Created by:              WJ              
** Created Date:            2016-10-19
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/
uint8_t App_FlashReadDailyData(uint8_t block_count, uint16_t number, uint8_t *data)
{
    uint32_t offset = 0;
    
    if(block_count >= DAILY_DATA_BLOCK_MAX)
    {
        return APP_FLASH_FAILED;
    }
    
    offset = App_FlashGetDailyDataOffset(block_count, number + DAILY_DATA_START_NUMBER);   
    memcpy(data, app_fs_config.p_start_addr + offset, DAILY_DATA_EACH_WORDS_SIZE * 4);
    return APP_FLASH_SUCCESS;
}

/****************************************************************
** Function name:           App_FlashWriteDailyData
** Descriptions:            Daily data storage
** input parameters:
** output parameters:
** Returned value:
** Created by:              WJ              
** Created Date:            2016-10-19
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/
uint8_t App_FlashWriteDailyData(const uint8_t block_count, const uint16_t number, 
                                const uint8_t *data, const uint16_t data_len , app_flash_callback_t callback)
{
    fs_ret_t error_code = FS_SUCCESS;  
    static uint32_t buf[DAILY_DATA_EACH_WORDS_SIZE] = {0x0,};  
    uint32_t offset;
    
    if(block_count >= DAILY_DATA_BLOCK_MAX 
        || data_len > DAILY_DATA_EACH_WORDS_SIZE * 4)
    {
        return APP_FLASH_FAILED;
    }
    
    memcpy(buf, data, data_len);        //Note that the data copy is bytes
    offset = App_FlashGetDailyDataOffset(block_count, number + DAILY_DATA_START_NUMBER);
    error_code = App_FlashWriteData(&app_fs_config, app_fs_config.p_start_addr + offset, 
                                    buf, DAILY_DATA_EACH_WORDS_SIZE, callback);//NULL);  
    if(error_code != FS_SUCCESS)
    {
        return APP_FLASH_FAILED;
    }
    return APP_FLASH_SUCCESS;
}

/****************************************************************
** Function name:           App_FlashEraseDailyDataPage
** Descriptions:            Erase the daily data block
** input parameters:
** output parameters:
** Returned value:
** Created by:              WJ              
** Created Date:            2016-10-19
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/
uint8_t App_FlashEraseDailyDataPage(uint8_t block_count)
{
    fs_ret_t  error_code = FS_SUCCESS;
    
    if(block_count >= DAILY_DATA_BLOCK_MAX)
    {
        return APP_FLASH_FAILED;
    }
    error_code = App_FlashErasePage(&app_fs_config, App_FlashGetDailyDataAddressOfPage(block_count), 
                                    DAILY_DATA_BLOCK_UINT, NULL);  
    
    if(error_code != FS_SUCCESS)
    {
        return APP_FLASH_FAILED;
    }   
    return APP_FLASH_SUCCESS;    
}

/****************************************************************
** Function name:           App_FlashWriteDailyHead
** Descriptions:            
** input parameters:
** output parameters:
** Returned value:
** Created by:              WJ              
** Created Date:            2016-11-25
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/
uint8_t App_FlashWriteDailyHead(const uint8_t block_count, const uint8_t *head_data, const uint16_t head_data_len)
{    
    static uint8_t cnt = 0;
    fs_ret_t  error_code = FS_SUCCESS;  
    static uint32_t head_write_buf[2][DAILY_DATA_EACH_WORDS_SIZE*2] = {0x0,};
    uint32_t offset = 0;
    
    if(block_count >= DAILY_DATA_BLOCK_MAX || head_data == NULL 
      || head_data_len == 0 || head_data_len > DAILY_DATA_EACH_WORDS_SIZE * 2 * 4)
    {
        return APP_FLASH_FAILED;
    }
    if(cnt > 1)
    {
        cnt = 0;
    }
    
    memcpy(head_write_buf[cnt], head_data, head_data_len);

    offset = App_FlashGetDailyDataOffset(block_count, 0);
    
    error_code = App_FlashWriteData(&app_fs_config, app_fs_config.p_start_addr + offset, 
                                    head_write_buf[cnt++], DAILY_DATA_EACH_WORDS_SIZE * 2, NULL);  
    if(error_code != FS_SUCCESS)
    {
        return APP_FLASH_FAILED;
    }
    return APP_FLASH_SUCCESS;   
    
}

/****************************************************************
** Function name:           App_FlashReadDailyHead
** Descriptions:            
** input parameters:
** output parameters:
** Returned value:
** Created by:              WJ              
** Created Date:            2016-11-25
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/
uint8_t App_FlashReadDailyHead(const uint8_t block_count, uint8_t *head_read_buf, const uint16_t head_read_len)
{ 
    uint32_t offset = 0;
    
    if(block_count >= DAILY_DATA_BLOCK_MAX || head_read_buf == NULL 
      || head_read_len == 0 || head_read_len > DAILY_DATA_EACH_WORDS_SIZE * 2 * 4)
    {
        return APP_FLASH_FAILED;
    }

    offset = App_FlashGetDailyDataOffset(block_count, 0);   
    memcpy(head_read_buf, app_fs_config.p_start_addr+offset, head_read_len);  
    
    return APP_FLASH_SUCCESS;      
}

/****************************************************************
** Function name:           App_FlashReadDailySleep
** Descriptions:            Read the daily sleep data
** input parameters:
** output parameters:
** Returned value:
** Created by:              WJ              
** Created Date:            2016-11-25
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/
uint8_t App_FlashReadDailySleep(const uint8_t block_count, const uint16_t number, uint8_t *sleep_read_data)
{
    uint32_t offset = 0;
    
    if(block_count >= DAILY_DATA_BLOCK_MAX)
    {
        return APP_FLASH_FAILED;
    }
    
    offset = App_FlashGetDailyDataOffset(block_count, 0);
    offset += SLEEP_DATA_START_OFFSET / 4;
    offset += DAILY_DATA_SLEEP_WORDS_SIZE * number;
    
    memcpy(sleep_read_data, app_fs_config.p_start_addr + offset, DAILY_DATA_SLEEP_WORDS_SIZE * 4);
    return APP_FLASH_SUCCESS;
}

/****************************************************************
** Function name:           App_FlashWriteDailySleep
** Descriptions:            Daily data storage
** input parameters:
** output parameters:
** Returned value:
** Created by:              WJ              
** Created Date:            2016-10-19
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/
uint8_t App_FlashWriteDailySleep(const uint8_t block_count, const uint16_t number, 
                                 const uint8_t *sleep_data, const uint16_t sleep_data_len )
{
    fs_ret_t  error_code = FS_SUCCESS;  
    static uint32_t sleep_write_buf[DAILY_DATA_SLEEP_WORDS_SIZE] = {0x0,};  
    uint32_t offset = 0;
    
    if(block_count >= DAILY_DATA_BLOCK_MAX 
      || sleep_data_len != DAILY_DATA_SLEEP_WORDS_SIZE * 4)
    {
        return APP_FLASH_FAILED;
    }
    
    memcpy(sleep_write_buf, sleep_data, sleep_data_len);        //Note that the data copy is bytes
    offset = App_FlashGetDailyDataOffset(block_count, 0);
    offset += SLEEP_DATA_START_OFFSET  /4;
    offset += DAILY_DATA_SLEEP_WORDS_SIZE*number;
    error_code = App_FlashWriteData(&app_fs_config, app_fs_config.p_start_addr+offset, sleep_write_buf, DAILY_DATA_SLEEP_WORDS_SIZE, NULL);  
    if(error_code != FS_SUCCESS)
    {
        return APP_FLASH_FAILED;
    }
    return APP_FLASH_SUCCESS;
}

/****************************** Aram clock data ******************************/
/****************************************************************
** Function name:           App_FlashGetAlarmClockDataAddresOfPage
** Descriptions:            System data storage offset
** input parameters:
** output parameters:
** Returned value:
** Created by:              WJ              
** Created Date:            2016-10-19
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/
static uint32_t const *App_FlashGetAlarmClockDataAddresOfPage()
{
    return app_fs_config.p_start_addr + ALARM_DATA_BLOCK_START_PAGE * PAGE_SIZE_WORDS;
}

/****************************************************************
** Function name:           app_flash_get_system_data_offset
** Descriptions:            System data storage offset
** input parameters:
** output parameters:
** Returned value:
** Created by:              WJ              
** Created Date:            2016-10-19
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/
static uint32_t App_FlashGetAlarmClockDataOffset()
{
    return  ALARM_DATA_BLOCK_START_PAGE * PAGE_SIZE_WORDS;
}

/****************************************************************
** Function name:           App_FlashReadAlarmClockData
** Descriptions:            Read the alarm clock data
** input parameters:
** output parameters:
** Returned value:
** Created by:              WJ              
** Created Date:            2016-10-19
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/
uint8_t App_FlashReadAlarmClockData(uint8_t *data, uint16_t data_len)
{
    uint32_t offset = 0;
  
    offset = App_FlashGetAlarmClockDataOffset();   
    memcpy(data, app_fs_config.p_start_addr + offset, data_len);
    return APP_FLASH_SUCCESS;
}

/****************************************************************
** Function name:           App_FlashWriteAlarmClockData
** Descriptions:            
** input parameters:
** output parameters:
** Returned value:
** Created by:              WJ              
** Created Date:            2016-10-19
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/
uint8_t App_FlashWriteAlarmClockData(const uint8_t *data, const uint16_t data_len )
{
    uint16_t len = 0;
    fs_ret_t  error_code = FS_SUCCESS;  
    uint32_t *buf = NULL;
    static uint32_t offset = 0;
      
    buf = (uint32_t *)data;
    len = data_len / 4;
    if(data_len % 4 != 0)
    {
        len++;
    }
       
    offset = App_FlashGetAlarmClockDataOffset();

    error_code = App_FlashWriteData(&app_fs_config, app_fs_config.p_start_addr + offset, buf, len, NULL);  
    if(error_code != FS_SUCCESS)
    {
        return APP_FLASH_FAILED;
    }
    return APP_FLASH_SUCCESS;
}

/****************************************************************
** Function name:           App_FlashEraseAlarmClockData
** Descriptions:            Erase the alarm clock data block
** input parameters:
** output parameters:
** Returned value:
** Created by:              WJ              
** Created Date:            2016-10-19
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/
uint8_t App_FlashEraseAlarmClockData(void)
{
    fs_ret_t  error_code = FS_SUCCESS;  
    
    error_code = App_FlashErasePage(&app_fs_config, App_FlashGetAlarmClockDataAddresOfPage(), 
                                    ALARM_DATA_BLOCK_UINT, NULL);  
    
    if(error_code != FS_SUCCESS)
    {
        return APP_FLASH_FAILED; 
    }   
    return APP_FLASH_SUCCESS;    
}

/****************************** System data record******************************/
/****************************************************************
** Function name:           App_FlashFindSystemDataRecord
** Descriptions:            find system record by file id and rec key
** input parameters:
** output parameters:
** Returned value:
** Created by:              WJ              
** Created Date:            2016-10-20
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/  
static ret_code_t App_FlashFindSystemDataRecord(system_record_info_t system_record_s, fds_record_desc_t *record_desc)
{
    fds_find_token_t ftok = {0x00,};
    
    memset(&ftok, 0x00, sizeof(fds_find_token_t));
    // Loop until all records with the given key and file ID have been found.
    ret_code_t ret = fds_record_find(system_record_s.file_id, system_record_s.rec_key, record_desc, &ftok);   
    if (ret != FDS_SUCCESS)
    {
        return NRF_ERROR_NOT_FOUND;
    }

    return NRF_SUCCESS;
}

/****************************************************************
** Function name:           App_FlashWriteSystemDataRecord
** Descriptions:            write or update system record
** input parameters:
** output parameters:
** Returned value:
** Created by:              WJ              
** Created Date:            2016-10-20
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
uint8_t App_FlashWriteSystemDataRecord(system_record_info_t system_record_s, const uint8_t *data, const uint16_t data_len)
{
    static uint8_t cnt = 0; 
    uint16_t data_words = 0;
    ret_code_t ret = FS_SUCCESS;       
    fds_record_t        record = {0x00,};
    fds_record_desc_t   record_desc = {0x00,};
    fds_record_chunk_t  record_chunk = {0x00,};
    static uint32_t  m_deadbeef[2][SYSTEM_DATA_EACH_WORDS_SIZE] = {0,};        // 120 byte
    
    data_words = data_len/4;
    if(data_len % 4 != 0)   // Four byte alignment
    {
        data_words += 1; 
    }
    if(data_words > SYSTEM_DATA_EACH_WORDS_SIZE)
    {
        data_words = SYSTEM_DATA_EACH_WORDS_SIZE;
    }
    if(cnt > 1)
    {
        cnt = 0;
    }    
    memcpy(m_deadbeef[cnt], data, data_len);
    // Set up data.
    record_chunk.p_data         = m_deadbeef[cnt++];       // 这里必须使用uint32_t 类型的数组
    record_chunk.length_words   = data_words;
    // Set up record.
    record.file_id           = system_record_s.file_id;
    record.key               = system_record_s.rec_key;
    record.data.p_chunks     = &record_chunk;
    record.data.num_chunks   = 1;   
    
    ret = App_FlashFindSystemDataRecord(system_record_s, &record_desc);    
    
    if(ret == NRF_ERROR_NOT_FOUND)
    {
    
        ret = fds_record_write(&record_desc, &record);     
    }
    else
    {
        ret = fds_record_update(&record_desc, &record);  
    }
    if (ret != FDS_SUCCESS)
    {    
        // Handle error.
        
        if(ret == FDS_ERR_NO_SPACE_IN_FLASH)
        {
            ret = fds_gc();
            
//            if(ret == FDS_SUCCESS)
//            {
//                ret = fds_record_update(&record_desc, &record);
//                
//                return ((ret == FDS_SUCCESS) ? APP_FLASH_SUCCESS : APP_FLASH_FAILED);
//            }
        }
        return APP_FLASH_FAILED; 
    }   
    return APP_FLASH_SUCCESS;    
}

/****************************************************************
** Function name:           App_FlashReadSystemDataRecord
** Descriptions:            read system record
** input parameters:
** output parameters:
** Returned value:
** Created by:              WJ              
** Created Date:            2016-10-20
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
uint8_t App_FlashReadSystemDataRecord(system_record_info_t system_record_s, uint8_t *data, const uint16_t data_len)
{
    ret_code_t ret = FS_SUCCESS;   
    uint16_t data_len_temp = 0;
    fds_flash_record_t  flash_record = {0x00,};
    fds_record_desc_t   record_desc = {0x00,};
    
    data_len_temp = data_len;
    memset(data, 0x00, data_len);
    ret = App_FlashFindSystemDataRecord(system_record_s, &record_desc);  
    if (ret == FDS_SUCCESS)
    {    
        if (fds_record_open(&record_desc, &flash_record) != FDS_SUCCESS)    
        {       
            // Handle error. 
            return APP_FLASH_FAILED;            
        }    
        // Access the record through the flash_record structure.  
        if(flash_record.p_header->tl.length_words *4 < data_len_temp)
        {
            data_len_temp = flash_record.p_header->tl.length_words * 4;
        }
        memcpy(data, flash_record.p_data, data_len_temp);
        // Close the record when done.    
        if (fds_record_close(&record_desc) != FDS_SUCCESS)    
        {        
            // Handle error.               
        }
    }  
    else
    {
        return APP_FLASH_FAILED;
    }
    return APP_FLASH_SUCCESS;    
}


/****************************************************************
** Function name:           App_FlashDeleteSystemDataRecord
** Descriptions:            delete system record
** input parameters:
** output parameters:
** Returned value:
** Created by:              WJ              
** Created Date:            2016-10-20
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
uint8_t App_FlashDeleteSystemDataRecord(system_record_info_t system_record_s)
{
    fds_record_desc_t descriptor = {0x00,};
    fds_record_desc_t   record_desc = {0x00,};
    
    if(App_FlashFindSystemDataRecord(system_record_s, &record_desc) != NRF_ERROR_NOT_FOUND)
    {  
        descriptor.record_id = record_desc.record_id;
        ret_code_t ret = fds_record_delete(&descriptor);
        if (ret != FDS_SUCCESS)
        {    
            // Error.
            //return APP_FLASH_FAILED;
        }    
    }
    return APP_FLASH_SUCCESS;
}


/****************************************************************
** Function name:           App_FlashEraseAllPage
** Descriptions:            
** input parameters:
** output parameters:
** Returned value:
** Created by:              WJ              
** Created Date:            2016-12-07
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/
uint8_t App_FlashEraseAllPage(void)
{
    fs_ret_t  error_code = FS_SUCCESS;  
    
    error_code = App_FlashErasePage(&app_fs_config, app_fs_config.p_start_addr,
                                   ALL_FLASH_PAGE_SIZE - 1, NULL);  
    
    if(error_code != FS_SUCCESS)
    {
        return APP_FLASH_FAILED; 
    }   
    return APP_FLASH_SUCCESS;    
}


static uint32_t App_FlashGetDataOffset(uint8_t block_count, uint16_t number )
{
    return  block_count * 256 + number * 4;
}

/****************************************************************
** Function name:           App_FlashReadDataByAddress
** Descriptions:            读取flash数据
** input parameters:        block_count
** output parameters:
** Returned value:
** Created by:              WJ              
** Created Date:            2017-02-22
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/
uint8_t App_FlashReadDataByAddress(uint8_t block_count, uint16_t number, uint8_t *data)
{
    uint32_t offset = 0;   
    
    if(block_count >= ALL_FLASH_PAGE_SIZE * 4 )
    {
        return APP_FLASH_FAILED;
    }
    
    offset = App_FlashGetDataOffset(block_count, number);   
    memcpy(data, app_fs_config.p_start_addr + offset, 16);     
    return APP_FLASH_SUCCESS;
}

/****************************************************************
** Function name:           App_FlashEraseProductInformation
** Descriptions:            擦除生产信息和设备ID
** input parameters:
** output parameters:
** Returned value:
** Created by:              WJ              
** Created Date:            2017-02-22
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/
uint8_t App_FlashEraseProductInformation(void)
{
    fs_ret_t  error_code = FS_SUCCESS;  
    
    error_code = App_FlashErasePage(&app_fs_config, (app_fs_config.p_end_addr - PAGE_SIZE_WORDS), 1, NULL);  
    
    if(error_code != FS_SUCCESS)
    {
        return APP_FLASH_FAILED; 
    }   
    return APP_FLASH_SUCCESS;     
}

/****************************************************************
** Function name:           App_FlashUpdataProductInformation
** Descriptions:            更新生产信息和设备ID
** input parameters:
** output parameters:
** Returned value:
** Created by:              WJ              
** Created Date:            2017-02-22
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/
uint8_t App_FlashUpdataProductInformation(const uint8_t *data, const uint16_t data_len )
{
    fs_ret_t  error_code = FS_SUCCESS;  
    
    error_code = App_FlashWriteData(&app_fs_config, (app_fs_config.p_end_addr - PAGE_SIZE_WORDS), 
                                   (uint32_t *)data, data_len / 4, NULL);  
    if(error_code != FS_SUCCESS)
    {
        return APP_FLASH_FAILED;
    }
    return APP_FLASH_SUCCESS;  
}

/****************************************************************
** Function name:           App_FlashGetProductInformation
** Descriptions:            获取生产信息和设备ID
** input parameters:
** output parameters:
** Returned value:
** Created by:              WJ              
** Created Date:            2017-02-22
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/
uint8_t App_FlashGetProductInformation(uint8_t *data, const uint16_t data_len )
{
    memcpy(data, (app_fs_config.p_end_addr - PAGE_SIZE_WORDS), data_len);
    return APP_FLASH_SUCCESS;
}


/****************************** Movement patterns Data ******************************/
/****************************************************************
** Function name:           App_FlashGetMovementDataAddressOfPage
** Descriptions:            Movement data storage offset
** input parameters:
** output parameters:
** Returned value:
** Created by:              WJ              
** Created Date:            2017-08-03
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/
static uint32_t const *App_FlashGetMovementDataAddressOfPage(uint8_t block_count)
{
    return app_fs_config.p_start_addr + MOVEMENT_PATTERNS_DATA_BLOCK_START_PAGE * PAGE_SIZE_WORDS
           + (block_count * MOVEMENT_PATTERNS_DATA_BLOCK_UINT * PAGE_SIZE_WORDS);
}

/****************************************************************
** Function name:           App_FlashGetMovementDataOffset
** Descriptions:            Movement data storage offset
** input parameters:
** output parameters:
** Returned value:
** Created by:              WJ              
** Created Date:            2017-08-03
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/
static uint32_t App_FlashGetMovementDataOffset(uint8_t block_count, uint16_t number )
{
    return  MOVEMENT_PATTERNS_DATA_BLOCK_START_PAGE * PAGE_SIZE_WORDS +
            + block_count * MOVEMENT_PATTERNS_DATA_BLOCK_SIZE + number * MOVEMENT_PATTERNS_DATA_EACH_WORDS_SIZE;
}

/****************************************************************
** Function name:           App_FlashReadMovementData
** Descriptions:            Read the daily data
** input parameters:
** output parameters:
** Returned value:
** Created by:              WJ              
** Created Date:            2017-08-03
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/
uint8_t App_FlashReadMovementData(uint8_t block_count, uint16_t number, uint8_t *data)
{
    uint32_t offset = 0;
    
    if(block_count >= MOVEMENT_PATTERNS_DATA_BLOCK_MAX)
    {
        return APP_FLASH_FAILED;
    }
    
    offset = App_FlashGetMovementDataOffset(block_count, number + MOVEMENT_PATTERNS_DATA_START_NUMBER);   
    memcpy(data, app_fs_config.p_start_addr + offset, MOVEMENT_PATTERNS_DATA_EACH_WORDS_SIZE * 4);
    return APP_FLASH_SUCCESS;
}

/****************************************************************
** Function name:           App_FlashWriteMovementData
** Descriptions:            Movement data storage
** input parameters:
** output parameters:
** Returned value:
** Created by:              WJ              
** Created Date:            2017-08-03
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/
uint8_t App_FlashWriteMovementData(const uint8_t block_count, const uint16_t number, const uint8_t *data, 
                                   const uint16_t data_len, app_flash_callback_t callback)
{
    fs_ret_t  error_code = FS_SUCCESS;  
    static uint32_t buf[MOVEMENT_PATTERNS_DATA_EACH_WORDS_SIZE] = {0x0,};  
    uint32_t offset = 0;
    
    if(block_count >= MOVEMENT_PATTERNS_DATA_BLOCK_MAX 
      || data_len > MOVEMENT_PATTERNS_DATA_EACH_WORDS_SIZE * 4)
    {
        return APP_FLASH_FAILED;
    }
    
    memcpy(buf, data, data_len);        //Note that the data copy is bytes
    offset = App_FlashGetMovementDataOffset(block_count, number + MOVEMENT_PATTERNS_DATA_START_NUMBER);
    error_code = App_FlashWriteData(&app_fs_config, app_fs_config.p_start_addr + offset, 
                                    buf, MOVEMENT_PATTERNS_DATA_EACH_WORDS_SIZE, callback);//NULL);  
    if(error_code != FS_SUCCESS)
    {
        return APP_FLASH_FAILED;
    }
    return APP_FLASH_SUCCESS;
}

/****************************************************************
** Function name:           App_FlashEraseMovementDataPage
** Descriptions:            Erase the daily data block
** input parameters:
** output parameters:
** Returned value:
** Created by:              WJ              
** Created Date:            2017-08-03
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/
uint8_t App_FlashEraseMovementDataPage(uint8_t block_count)
{
    fs_ret_t  error_code = FS_SUCCESS;  
    
    if(block_count >= MOVEMENT_PATTERNS_DATA_BLOCK_MAX)
    {
        return APP_FLASH_FAILED;
    }
    error_code = App_FlashErasePage(&app_fs_config, App_FlashGetMovementDataAddressOfPage(block_count), 
                                    MOVEMENT_PATTERNS_DATA_BLOCK_UINT, NULL);  
    
    if(error_code != FS_SUCCESS)
    {
        return APP_FLASH_FAILED;
    }   
    return APP_FLASH_SUCCESS;    
}


uint8_t App_FlashEraseMovementDataAllPage(void)
{
    fs_ret_t  error_code = FS_SUCCESS;  
    
    error_code = App_FlashErasePage(&app_fs_config, App_FlashGetMovementDataAddressOfPage(0),
                                   MOVEMENT_PATTERNS_DATA_TOTAL_PAGE, NULL);  
    
    if(error_code != FS_SUCCESS)
    {
        return APP_FLASH_FAILED; 
    }   
    return APP_FLASH_SUCCESS; 
}

/****************************************************************
** Function name:           App_FlashWriteMovementHead
** Descriptions:            
** input parameters:
** output parameters:
** Returned value:
** Created by:              WJ              
** Created Date:            2017-08-03
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/
uint8_t App_FlashWriteMovementHead(const uint8_t block_count, const uint8_t *head_data, const uint16_t head_data_len)
{
    fs_ret_t error_code = FS_SUCCESS;  
    uint32_t offset = 0;
    
    if(block_count >= MOVEMENT_PATTERNS_DATA_BLOCK_MAX || head_data == NULL 
      || head_data_len == 0 
      || head_data_len > MOVEMENT_PATTERNS_DATA_EACH_WORDS_SIZE * MOVEMENT_PATTERNS_DATA_START_NUMBER * 4)
    {
        return APP_FLASH_FAILED;
    }

    offset = App_FlashGetMovementDataOffset(block_count, 0);
    
    error_code = App_FlashWriteData(&app_fs_config, app_fs_config.p_start_addr + offset, 
                                   (uint32_t *)head_data, MOVEMENT_PATTERNS_DATA_EACH_WORDS_SIZE * 2, NULL);  
    if(error_code != FS_SUCCESS)
    {
        return APP_FLASH_FAILED;
    }
    return APP_FLASH_SUCCESS;      
}

/****************************************************************
** Function name:           App_FlashReadMovementHead
** Descriptions:            
** input parameters:
** output parameters:
** Returned value:
** Created by:              WJ              
** Created Date:            2017-08-03
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/
uint8_t App_FlashReadMovementHead(const uint8_t block_count, uint8_t *head_read_buf, const uint16_t head_read_len)
{ 
    uint32_t offset = 0;
    
    
    if(block_count >= MOVEMENT_PATTERNS_DATA_BLOCK_MAX || head_read_buf == NULL 
      || head_read_len == 0 || head_read_len > MOVEMENT_PATTERNS_DATA_EACH_WORDS_SIZE * 4 * 4)
    {
        return APP_FLASH_FAILED;
    }

    offset = App_FlashGetMovementDataOffset(block_count, 0);   
    memcpy(head_read_buf, app_fs_config.p_start_addr + offset, head_read_len);  
    
    return APP_FLASH_SUCCESS;      
}

/****************************************************************
** Function name:           App_FlashCheckAreaUsable
** Descriptions:            
** input parameters:
** output parameters:
** Returned value:          返回0：已被使用，1：没有被使用
** Created by:              WJ              
** Created Date:            2017-08-03
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/
static bool App_FlashCheckAreaBufUsable(uint8_t *input_data, uint16_t input_len)
{  
    uint16_t i = 0;
    
    if(input_data == NULL || input_len == 0)
    {
        return 0;
    }

    for(i = 0; i < input_len; i++)
    {
        if(input_data[i] != 0xFF)
        {
            return 0;
        }
    }    
    return 1;    
}


/****************************************************************
** Function name:           App_FlashCheckAreaIsUsable
** Descriptions:            获取指定flash区域是否被使用过
** input parameters:
** output parameters:
** Returned value:          返回0：flash使用过，1：flash未使用过
** Created by:              WJ              
** Created Date:            2017-08-03
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/
uint8_t App_FlashCheckAreaIsUsable(uint8_t start_page, uint8_t page_size)
{  
    uint8_t i = 0;
    uint8_t j = 0;
    uint8_t ret = 1;
    uint8_t flash_data_buf[256] = {0x00,};
    uint32_t offset = 0;
    
    if(page_size == 0)
    {
        return 0;
    }  
    
    for(i = 0; i < page_size; i++)
    {     
        offset = (start_page + i) * PAGE_SIZE_WORDS;       
        for(j = 0; j < 4; j++)
        {
            offset += j * 256 / 4;
            memcpy(flash_data_buf, app_fs_config.p_start_addr + offset, 256); 
            ret &= App_FlashCheckAreaBufUsable(flash_data_buf, 256);
        }
        if(ret == 0)
        {
            return 0;
        }
    }
    
    return 1;
}

/******************************physiological period data ******************************/
/****************************************************************
** Function name:           App_FlashGetPhysPeriodDataAddresOfPage
** Descriptions:            System data storage offset
** input parameters:
** output parameters:
** Returned value:
** Created by:              WJ              
** Created Date:            2016-10-19
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/
static uint32_t const *App_FlashGetPhysPeriodDataAddresOfPage()
{
    return app_fs_config.p_start_addr + PHYS_PERIOD_DATA_BLOCK_START_PAGE * PAGE_SIZE_WORDS;
}

/****************************************************************
** Function name:           App_FlashGetPhysPeriodDataOffset
** Descriptions:            System data storage offset
** input parameters:
** output parameters:
** Returned value:
** Created by:              WJ              
** Created Date:            2016-10-19
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/
static uint32_t App_FlashGetPhysPeriodDataOffset(const uint16_t number)
{
    return  PHYS_PERIOD_DATA_BLOCK_START_PAGE * PAGE_SIZE_WORDS + number * PHYS_PERIOD_DATA_EACH_WORDS_SIZE;
}

/****************************************************************
** Function name:           App_FlashReadPhysPeriodData
** Descriptions:            Read the alarm clock data
** input parameters:
** output parameters:
** Returned value:
** Created by:              WJ              
** Created Date:            2016-10-19
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/
uint8_t App_FlashReadPhysPeriodData(const uint16_t number, uint8_t *data, uint16_t data_len)
{
    uint32_t offset = 0;
  
    offset = App_FlashGetPhysPeriodDataOffset(0);   
    memcpy(data, app_fs_config.p_start_addr + offset, data_len);
    return APP_FLASH_SUCCESS;
}

/****************************************************************
** Function name:           App_FlashWritePhysPeriodData
** Descriptions:            
** input parameters:
** output parameters:
** Returned value:
** Created by:              WJ              
** Created Date:            2016-10-19
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/
uint8_t App_FlashWritePhysPeriodData(const uint16_t number, const uint8_t *data, const uint16_t data_len )
{
    uint16_t len = 0;
    fs_ret_t  error_code = FS_SUCCESS;  
    uint32_t *buf = NULL;
    uint32_t offset = 0;
      
    buf = (uint32_t *)data;
    len = data_len / 4;
    if(data_len % 4 != 0)
    {
        len++;
    }
       
    offset = App_FlashGetPhysPeriodDataOffset(0);

    error_code = App_FlashWriteData(&app_fs_config, app_fs_config.p_start_addr + offset, buf, len, NULL);  
    if(error_code != FS_SUCCESS)
    {
        return APP_FLASH_FAILED;
    }
    return APP_FLASH_SUCCESS;
}

/****************************************************************
** Function name:           App_FlashErasePhysPeriodData
** Descriptions:            Erase the alarm clock data block
** input parameters:
** output parameters:
** Returned value:
** Created by:              WJ              
** Created Date:            2016-10-19
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/
uint8_t App_FlashErasePhysPeriodData(void)
{
    fs_ret_t  error_code = FS_SUCCESS;  
    
    error_code = App_FlashErasePage(&app_fs_config, App_FlashGetPhysPeriodDataAddresOfPage(), 
                                    PHYS_PERIOD_DATA_BLOCK_UINT, NULL);  
    
    if(error_code != FS_SUCCESS)
    {
        return APP_FLASH_FAILED; 
    }   
    return APP_FLASH_SUCCESS;    
}

/******************************physiological period data end******************************/

