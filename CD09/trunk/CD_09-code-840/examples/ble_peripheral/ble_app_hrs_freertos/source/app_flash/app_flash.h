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
#ifndef APP_FLASH_H
#define APP_FLASH_H


#ifdef __CPLUSPLUS
extern "C" {
#endif

#include "fstorage.h"

// ALL_MOTION
#define PHYS_PERIOD_DATA_START_NUMBER         0x00            // Head information
#define PHYS_PERIOD_DATA_BLOCK_START_PAGE     0x00            // Daily data storage start page
#define PHYS_PERIOD_DATA_BLOCK_UINT           0x01            // 1* (1024*4) 4K, A time data
#define PHYS_PERIOD_DATA_BLOCK_TOTAL          0x01            // Total number of days, 3 time
#define PHYS_PERIOD_DATA_BLOCK_SIZE           (PHYS_PERIOD_DATA_BLOCK_UINT * PAGE_SIZE_WORDS)   // 2 page,The size of the block of one time   
#define PHYS_PERIOD_DATA_EACH_WORDS_SIZE      0x04            // 4*4 Byte,Each of the movement data size is 16 Byte
// Total number of times, 3 time
#define PHYS_PERIOD_DATA_TOTAL_PAGE           (PHYS_PERIOD_DATA_BLOCK_UINT * PHYS_PERIOD_DATA_BLOCK_TOTAL)  
#define PHYS_PERIOD_DATA_END_PAGE             (PHYS_PERIOD_DATA_BLOCK_START_PAGE + PHYS_PERIOD_DATA_TOTAL_PAGE)     
    
// movement patterns
#define MOVEMENT_PATTERNS_DATA_START_NUMBER         0x04            // Head information
#define MOVEMENT_PATTERNS_DATA_BLOCK_START_PAGE     PHYS_PERIOD_DATA_END_PAGE           // Daily data storage start page
#define MOVEMENT_PATTERNS_DATA_BLOCK_UINT           0x02            // 2* (1024*4) 8K, A time data
#define MOVEMENT_PATTERNS_DATA_BLOCK_TOTAL          0x03            // Total number of days, 3 time
#define MOVEMENT_PATTERNS_DATA_BLOCK_SIZE           (MOVEMENT_PATTERNS_DATA_BLOCK_UINT * PAGE_SIZE_WORDS)   // 2 page,The size of the block of one time   
#define MOVEMENT_PATTERNS_DATA_EACH_WORDS_SIZE      0x05            // 5*4 Byte,Each of the movement data size is 20 Byte
// Total number of times, 3 time
#define MOVEMENT_PATTERNS_DATA_TOTAL_PAGE           (MOVEMENT_PATTERNS_DATA_BLOCK_UINT * MOVEMENT_PATTERNS_DATA_BLOCK_TOTAL)  
#define MOVEMENT_PATTERNS_DATA_END_PAGE             (MOVEMENT_PATTERNS_DATA_BLOCK_START_PAGE + MOVEMENT_PATTERNS_DATA_TOTAL_PAGE)  


#define DAILY_DATA_START_NUMBER         0x02                                        // 
#define DAILY_DATA_BLOCK_START_PAGE     MOVEMENT_PATTERNS_DATA_END_PAGE             // Daily data storage start page
#define DAILY_DATA_BLOCK_UINT           0x02                                        // 2* (1024*4) 8K, A day's data
#define DAILY_DATA_BLOCK_TOTAL          0x05                                        // Total number of days, 5 days
#define DAILY_DATA_TOTAL_FLASH_PAGE     (DAILY_DATA_BLOCK_UINT * DAILY_DATA_BLOCK_MAX)    // Flash the daily data of the page   
#define DAILY_DATA_MAX_OFFSET           0x1C00                         // 7*1024,
#define SLEEP_DATA_START_OFFSET         DAILY_DATA_MAX_OFFSET          // 3*1024 Byte, The last page of the daily data

#define DAILY_DATA_BLOCK_SIZE           (DAILY_DATA_BLOCK_UINT * PAGE_SIZE_WORDS)   // 2 page,The size of the block of data every day
#define DAILY_DATA_EACH_WORDS_SIZE      0x05            // 5*4 Byte,Each of the daily data size is 20 Byte
#define DAILY_DATA_SLEEP_WORDS_SIZE     0x18            // 96Byte sleep data 
#define DAILY_DATA_END_PAGE             (DAILY_DATA_BLOCK_START_PAGE + DAILY_DATA_TOTAL_FLASH_PAGE)

// alarm clock
#define ALARM_DATA_BLOCK_START_PAGE    DAILY_DATA_END_PAGE  // system data storage start page
#define ALARM_DATA_BLOCK_UINT          0x01                 // 1* (1024*4) 4K, system data 
#define ALARM_DATA_EACH_WORDS_SIZE     0x30                 // other data word size (4 Byte)

//运动模式：24K，日常：40K，闹钟：4K, 生产信息：4K 
#define ALL_FLASH_PAGE_SIZE            (PHYS_PERIOD_DATA_TOTAL_PAGE + MOVEMENT_PATTERNS_DATA_TOTAL_PAGE + ALARM_DATA_BLOCK_UINT + \
                                        DAILY_DATA_TOTAL_FLASH_PAGE + 1)    //A total of user data storage pages

// error code
#define APP_FLASH_SUCCESS       0x00
#define APP_FLASH_FAILED        0x01
   

#define DAILY_HEAD_FLAG_USED    0x00
#define DAILY_HEAD_FLAG_USING   0xAA
       
    
typedef struct
{
    //ID of the file that the record is associated with (must be in the range 0x0000 - 0xBFFF).
    //The value FDS_FILE_ID_INVALID (0xFFFF) is used by the system to identify records that have not been written correctly. 
    uint16_t file_id;   
    //Key that can be used to find the record (must be in the range 0x0001 - 0xBFFF). 
    //The value FDS_RECORD_KEY_DIRTY (0x0000) is reserved by the system to flag records that have been invalidated. 
    uint16_t rec_key;       
}system_record_info_t;    //Used to distinguish record
    
enum
{
    RECORD_SYSTEM_DATA = 0,
    RECORD_SYSTEM_TIME, 
    RECORD_SYSTEM_SWITCH,
    RECORD_MAX_SIZE
};

enum
{
    DAILY_DATA_BLOCK_0 = 0,
    DAILY_DATA_BLOCK_1, 
    DAILY_DATA_BLOCK_2,
    DAILY_DATA_BLOCK_3,
    DAILY_DATA_BLOCK_4,
    DAILY_DATA_BLOCK_MAX            //DAILY_DATA_BLOCK_MAX == DAILY_DATA_BLOCK_TOTAL
};

enum
{
    MOVEMENT_PATTERNS_DATA_BLOCK_0 = 0,
    MOVEMENT_PATTERNS_DATA_BLOCK_1, 
    MOVEMENT_PATTERNS_DATA_BLOCK_2,
    MOVEMENT_PATTERNS_DATA_BLOCK_MAX            //DAILY_DATA_BLOCK_MAX == DAILY_DATA_BLOCK_TOTAL
};


extern const system_record_info_t system_record_s[RECORD_MAX_SIZE];

typedef void(*app_flash_callback_t)(uint8_t result);

uint8_t App_FlashReadDailyData(const uint8_t block_count, const uint16_t number, uint8_t *data);
uint8_t App_FlashWriteDailyData(const uint8_t block_count, const uint16_t number, 
                               const uint8_t *data, const uint16_t data_len, app_flash_callback_t callback);
uint8_t App_FlashEraseDailyDataPage(const uint8_t block_count);

uint8_t App_FlashWriteDailyHead(const uint8_t block_count, const uint8_t *head_data, const uint16_t head_len);
uint8_t App_FlashReadDailyHead(const uint8_t block_count, uint8_t *head_read_buf, const uint16_t head_read_len);

uint8_t App_FlashWriteDailySleep(const uint8_t block_count, const uint16_t number, const uint8_t *sleep_data, 
                                    const uint16_t sleep_data_len );
uint8_t App_FlashReadDailySleep(const uint8_t block_count, const uint16_t number, uint8_t *sleep_read_data);

uint8_t App_FlashReadAlarmClockData(uint8_t *data, const uint16_t data_len);
uint8_t App_FlashWriteAlarmClockData(const uint8_t *data, const uint16_t data_len );
uint8_t App_FlashEraseAlarmClockData(void); 

uint8_t App_FlashWriteSystemDataRecord(system_record_info_t system_record_s, const uint8_t *data, uint16_t data_len);    
uint8_t App_FlashReadSystemDataRecord(system_record_info_t system_record_s, uint8_t *data, const uint16_t data_len);     
uint8_t App_FlashDeleteSystemDataRecord(system_record_info_t system_record_s);  
    
uint8_t App_FlashEraseAllPage(void);

uint8_t App_FlashEraseAllData(void);

uint8_t App_FlashReadDataByAddress(uint8_t block_count, uint16_t number, uint8_t *data);

uint8_t App_FlashEraseProductInformation(void);
uint8_t App_FlashUpdataProductInformation(const uint8_t *data, const uint16_t data_len );
uint8_t App_FlashGetProductInformation(uint8_t *data, const uint16_t data_len );



// Movement data storage offset
static uint32_t const *App_FlashGetMovementDataAddressOfPage(uint8_t block_count);
//Movement data storage offset
static uint32_t App_FlashGetMovementDataOffset(uint8_t block_count, uint16_t number );
//
uint8_t App_FlashReadMovementData(uint8_t block_count, uint16_t number, uint8_t *data);
//Movement data storage
uint8_t App_FlashWriteMovementData(const uint8_t block_count, const uint16_t number, const uint8_t *data, 
                                   const uint16_t data_len, app_flash_callback_t callback);
//擦除运动模式指定数据块数据
uint8_t App_FlashEraseMovementDataPage(uint8_t block_count);
//擦除运动模式的所有数据
uint8_t App_FlashEraseMovementDataAllPage(void);
//写运动模式头部记录
uint8_t App_FlashWriteMovementHead(const uint8_t block_count, const uint8_t *head_data, const uint16_t head_data_len);
//读取运动模式头部信息
uint8_t App_FlashReadMovementHead(const uint8_t block_count, uint8_t *head_read_buf, const uint16_t head_read_len);

//获取指定flash区域是否被使用过;返回0：flash使用过，1：flash未使用过
uint8_t App_FlashCheckAreaIsUsable(uint8_t start_page, uint8_t page_size);

//擦除女性功能数据
uint8_t App_FlashErasePhysPeriodData(void);
//保存女性功能数据
uint8_t App_FlashWritePhysPeriodData(const uint16_t number, const uint8_t *data, const uint16_t data_len );
//读取女性功能数据
uint8_t App_FlashReadPhysPeriodData(const uint16_t number, uint8_t *data, uint16_t data_len);
// 檫除数据后检查执行情况
void App_FlashEraseCheck(void);

#ifdef __CPLUSPLUS
}
#endif

#endif /* APP_FLASH_H */
