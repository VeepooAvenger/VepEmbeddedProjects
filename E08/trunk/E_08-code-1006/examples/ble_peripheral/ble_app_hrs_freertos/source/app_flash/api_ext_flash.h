/*******************Copyright(c)*********************************
** Veepoo Tech
** COPYRIGHT (C) 2016-2019
**----------------------FileInfo---------------------------------
** File name:                  api_ext_flash.h
** Last modified Date:         2018-11-05
** Last SYS_VERSION:           0.0.1
** Descriptions:
**---------------------------------------------------------------
** Createdby:                  WJ
** Createddate:                2018-11-05
** SYS_VERSION:                0.0.1
** Descriptions:               外部flash接口
**---------------------------------------------------------------
** Modified by:				
** Modified date:			
** SYS_VERSION:				
** Modified Descriptions:	
****************************************************************/

//注目前设计的全部接口在UI任务启动后，只能在UI任务中调用！！！！！！

#ifndef _API_EXT_FLASH_H_
#define _API_EXT_FLASH_H_

#include "data_manage.h"
#include "flash_data_type.h"


#define EXT_FLASH_TOTAL_SIZE        0x200000

//片外FLASH
#define EXT_FLASH_HEAD_BASE_ADDR    0x00000000

//片外FLASH---存储ECG自动测量区域分配
#define ECG_EXT_FLASH_BASE_ADDR     0x00027000
#define ECG_EXT_FLASH_TOTAL_SIZE    0x2D000     // 180K
#define ECG_EXT_FLASH_END_SIZE      (ECG_EXT_FLASH_BASE_ADDR + ECG_EXT_FLASH_TOTAL_SIZE)

// 是否启用存储ECG自动测量数据－－－－暂时不能直接使用，需要修改函数处理！！！！！！！！！！！！
#define USED_EXT_FLASH_ECG_AUTO_ENABLED      1          //启用该配置，需要对下列定义进行检查和修改！！！
                                                        //注意ＵＩ的结束位置

#if USED_EXT_FLASH_ECG_AUTO_ENABLED


#define ECG_RECORD_MAX_CNT          0x1E        // 30条
#define ECG_ONE_RECORD_SIZE         0x1800      // 6K = 6144 Byte

#define ECG_ORIGINA_DATA_INDEX          11      //原始数据区域开始
#define ECG_ORIGINA_DATA_ADC_INDEX      14      //原始数据开始
#define ECG_ORIGINA_DATA_ADC_LEN        6000    //原始数据长度

#define ECG_RESULT_INDEX                6014    //测量结果区域开始
#define ECG_RESULT_DATA_INDEX           6016    //测量结果开始
#define ECG_RESULT_DATA_LEN             25      //结果数据长度

#define ECG_EFFECTIVE_DATA_LEN          6048    //有效数据长度

#endif


// 外部Flash读取数据地址宏
#define RECORD_EXT_FLASH_DATA_ADDR(block_id, offset)  (uint32_t)(EXT_FLASH_HEAD_BASE_ADDR + (block_id)*1024 + offset)
//暂时不要读取自动测量存储数据
#define RECORD_EXT_FLASH_DATA_SIZE                    (ECG_EXT_FLASH_BASE_ADDR - EXT_FLASH_HEAD_BASE_ADDR) //(ECG_EXT_FLASH_END_SIZE - EXT_FLASH_HEAD_BASE_ADDR)  


#pragma pack(1)
typedef struct
{
    uint8_t     init:1;         // 上电初始化
    uint8_t     factory_init:1; // 恢复出厂初始化
    uint8_t     res1:6;         // 保留
    
    uint8_t     write:1;        // 写标记
    uint8_t     read:1;         // 读标记
    uint8_t     res:6;          // 保留
    
    uint8_t     *ecg_write_adc;         // 写数组地址
    uint32_t    ecg_write_adc_len;      // 写数据长度    
    uint8_t     *ecg_write_result;      // 写数组地址
    uint32_t    ecg_write_result_len;   // 写数据长度
     
    uint8_t     record_id;          // 读取记录ID
    uint16_t    offset_index;       // 偏移地址
    uint8_t     *read_data;         // 读数组地址
    uint32_t    read_len;           // 读数据长度
    
}ext_flash_ecg_opt_t;


typedef struct
{
    uint8_t     init:1;                 // 上电初始化
    uint8_t     write_head:1;           // 写头数据标记
    uint8_t     write_data:1;           // 写数据标记
    uint8_t     read_head:1;            // 读头数据标记
    uint8_t     read_data:1;            // 读数据标记
    uint8_t     erase_block:1;          // 擦除一块运动模式数据标记
    uint8_t     erase_all:1;            // 擦出所有运动模式数据标记
    uint8_t     res:1;                  // 保留
    
    uint8_t     *p_write_head;          // 写头数据地址
    uint8_t     write_head_block;       // 写头数据的块数
    uint32_t    write_head_len;         // 写头数据长度
    
    uint8_t     *p_write_data;          // 写数据地址
    uint8_t     write_data_block;       // 写数据的块数
    uint8_t     write_data_number;      // 写数据的条数
    uint32_t    write_data_len;         // 写数据长度
     
    uint8_t     *p_read_head;           // 读头数据地址
    uint8_t     read_head_block;        // 读头数据块数
    uint32_t    read_head_len;          // 读头数据长度
    
    uint8_t     *p_read_data;           // 读数据地址
    uint8_t     read_data_block;        // 读数据的块数
    uint8_t     read_data_number;       // 读数据的条数
    uint32_t    read_data_len;          // 读数据的长度
    
    uint8_t     erase_block_cnt;        // 擦除
}ext_flash_movement_opt_t;      //片外FLASH存储：运动模式数据索引

typedef struct
{
    uint8_t     init:1;             // 上电初始化
    uint8_t     write:1;            // 写标记
    uint8_t     read:1;             // 读标记
    uint8_t     erase_block:1;      // 擦出标记
    uint8_t     res:4;              // 保留
    
    uint8_t     *p_write;           // 写地址
    uint16_t    write_block;        // 写块数
    uint32_t    write_len;          // 写长度
    
     
    uint8_t     *p_read;            // 读地址
    uint16_t    read_block;         // 读块数
    uint32_t    read_len;           // 读长度
    
    uint8_t     erase_block_id;     // 擦除的块ID
    uint8_t     erase_block_num;    // 擦除的块数量
}ext_flash_ecg_rest_opt_t;      //片外FLASH存储：ECG结果数据索引

typedef struct
{
    uint8_t     init:1;             // 上电初始化
    uint8_t     write:1;            // 
    uint8_t     read:1;             // 
    uint8_t     erase_block:1;      //
    uint8_t     erase_all:1;        //
    uint8_t     res:3;              // 
    
    uint8_t     *p_write;           // 
    uint8_t     write_day_id;       //
    uint16_t    write_block;        // 
    uint32_t    write_len;          // 
    
     
    uint8_t     *p_read;            // 
    uint8_t     read_day_id;        //
    uint16_t    read_block;         // 
    uint32_t    read_len;           // 
    
    uint8_t     erase_block_id;     //
    
    uint8_t     write_data_type;          // 数据类型，0:默认日常数据;1：最后一包用于存储功耗统计数据； 
    uint8_t     read_data_type;          // 数据类型，0:默认日常数据;1：最后一包用于存储功耗统计数据； 
    
}ext_flash_daily_opt_t;

typedef struct
{
    uint8_t     init:1;             // 上电初始化
    uint8_t     write:1;            // 写标记
    uint8_t     read:1;             // 读标记
    uint8_t     res:5;              // 保留
    
    uint8_t     *p_write;           // 写地址
    uint32_t    write_offset;       // 写偏移
    uint32_t    write_len;          // 写长度
    
     
    uint8_t     *p_read;            // 读地址
    uint32_t    read_offset;        // 读偏移
    uint32_t    read_len;           // 读长度
    
}ext_flash_sleep_opt_t;     //片外FLASH存储：睡眠数据索引

typedef struct
{
    uint8_t     init:1;             // 上电初始化
    uint8_t     write:1;            // 写标记
    uint8_t     read:1;             // 读标记
    uint8_t     erase:1;            // 擦除标记
    uint8_t     res:4;              // 保留
    
    uint8_t     *p_write;           // 写地址
    uint32_t    write_len;          // 写长度
    
    uint8_t     *p_read;            // 读地址
    uint32_t    read_len;           // 读长度
}ext_flash_head_opt_t;      //片外FLASH存储：FLASH头部数据索引

typedef struct
{
    uint8_t     init:1;             // 上电初始化
    uint8_t     write:1;            // 写标记
    uint8_t     read:1;             // 读标记
    uint8_t     erase:1;            // 擦除部分标记
    uint8_t     erase_all:1;        // 擦除所有标记
    uint8_t     res:3;              // 保留
    
    
    uint8_t     *p_write;           // 写地址
    uint32_t    write_offset;       // 写偏移
    uint32_t    write_len;          // 写长度
    
    uint8_t     *p_read;            // 读地址
    uint32_t    read_offset;        // 读偏移
    uint32_t    read_len;           // 读长度
    
    uint32_t     erase_offset;       // 擦除偏移
    uint32_t     erase_size;         // 擦除的数据大小
}ext_flash_all_opt_t;       //片外FLASH存储：所有数据索引
#pragma pack()

//FLASH所有操作初始化函数
void Extflash_AllInfoInit(void);

//ECG处理
void Extflash_EcgErrDeal(void);

//ECG 读取参数处理 
uint8_t Extflash_EcgReadSet(uint8_t record_id, uint16_t offset_index, uint8_t data_len, uint8_t *data_buf);

//ECG 读取完成
uint8_t Extflash_EcgReadIsComplete(void);

// ECG 写参数处理
uint8_t Extflash_EcgWriteSet(uint8_t *adc_data, uint16_t adc_data_len, 
                            uint8_t *ecg_result, uint16_t ecg_result_len);

//ECG 写完成 
uint8_t Extflash_EcgWriteIsComplete(void);

//索引初始化 
uint8_t Extflash_SetEcgIndexInit(void);

//索引恢复出厂
uint8_t Extflash_SetEcgFactoryInit(void);
    
//Flash操作是否忙碌
uint8_t Extflash_EcgIsBusy(void);    
    
//Movement
//运动模式数据处理
void Extflash_MovementDeal(void);

//运动模式写头数据设置参数
uint8_t Extflash_MovementWriteHeadSet(uint8_t block_count, uint8_t *head_data, uint16_t head_data_len);

//运动模式头数据操作完成
uint8_t Extflash_MovementWriteHeadIsComplete(void);

//运动模式写数据设置设置参数
uint8_t Extflash_MovementWriteDataSet(uint8_t block_count, uint16_t number, uint8_t *data, uint16_t data_len);

//运动模式写数据完成
uint8_t Extflash_MovementWriteDataIsComplete(void);

//运动模式读取头数据设置参数
uint8_t Extflash_MovementReadHeadSet(uint8_t block_count, uint8_t *head_read_buf, uint16_t head_read_len);

//运动模式读取头数据完成
uint8_t Extflash_MovementReadHeadIsComplete(void);

//运动模式读取数据设置参数
uint8_t Extflash_MovementReadDataSet(uint8_t block_count, uint16_t number, uint8_t *data, uint16_t data_len);

//运动模式读取数据完成
uint8_t Extflash_MovementReadDataIsComplete(void);

//运动模式擦除一块数据设置
uint8_t Extflash_MovementEraseBlockSet(uint8_t block_count);

//运动模式擦除一块数据完成
uint8_t Extflash_MovementEraseBlockIsComplete(void);

//运动模式擦除所有数据设置
uint8_t Extflash_MovementEraseAllSet(void );

//运动模式擦除所有数据完成
uint8_t Extflash_MovementEraseAllIsComplete(void);

//运动模式操作是否正忙
uint8_t Extflash_MovementIsBusy(void);

//ECG result
//ECG结果处理函数
void Extflash_EcgRestDeal(void);

//设置写ECG结果参数
uint8_t Extflash_EcgRestWriteSet(uint16_t block_count, uint8_t *data, uint16_t data_len);

//写ECG结果完成
uint8_t Extflash_EcgRestWriteIsComplete(void);

//设置读取ECG结果的参数
uint8_t Extflash_EcgRestReadSet(uint16_t block_count, uint8_t *read_buf, uint16_t read_len);

//读取ECG结果完成
uint8_t Extflash_EcgRestReadIsComplete(void);

//擦除ECG结果数据
uint8_t Extflash_EcgRestEraseBlockSet(uint8_t block_id, uint8_t block_num);

//擦除ECG结果数据完成
uint8_t Extflash_EcgRestEraseBlockIsComplete(void);

//ECG结果数据操作函数
uint8_t Extflash_EcgRestIsBusy(void);

// Daily
void Extflash_DailyDeal(void);

uint8_t Extflash_DailyWriteSet(uint8_t day_id, uint16_t block_count, uint8_t *data, uint16_t data_len, uint8_t data_type);

uint8_t Extflash_DailyWriteIsComplete(void);

uint8_t Extflash_DailyReadSet(uint8_t day_id, uint16_t block_count, uint8_t *data, uint16_t data_len, uint8_t data_type);

uint8_t Extflash_DailyReadIsComplete(void);

uint8_t Extflash_DailyEraseBlockSet(uint8_t block_id);

uint8_t Extflash_DailyEraseBlockIsComplete(void);

uint8_t Extflash_DailyEraseAllSet(void);

uint8_t Extflash_DailyEraseAllIsComplete(void);

uint8_t Extflash_DailyIsBusy(void);

//sleep
//睡眠结果处理函数
void Extflash_SleepDeal(void);

//设置写睡眠结果的参数
uint8_t Extflash_SleepWriteSet(uint32_t offset, uint8_t *data, uint16_t data_len);

//写睡眠结果完成
uint8_t Extflash_SleepWriteIsComplete(void);

//设置读取睡眠结果的参数
uint8_t Extflash_SleepReadSet(uint32_t offset, uint8_t *data, uint16_t data_len);

//读取睡眠结果完成
uint8_t Extflash_SleepReadIsComplete(void);

//睡眠结果操作正忙
uint8_t Extflash_SleepIsBusy(void);


//head
//外部FLASH头部信息处理函数
void Extflash_HeadDeal(void);

//设置写外部FLASH头部信息的参数
uint8_t Extflash_HeadWriteSet(uint8_t *data, uint16_t data_len);

//写外部FLASH头部信息完成
uint8_t Extflash_HeadWriteIsComplete(void);

//设置读取外部FLASH头部信息的参数
uint8_t Extflash_HeadReadSet(uint8_t *data, uint16_t data_len);

//读取外部FLASH头部信息完成
uint8_t Extflash_HeadReadIsComplete(void);

//设置擦除外部FLASH头部信息
uint8_t Extflash_HeadEraseSet(void);

//擦除外部FLASH头部信息完成
uint8_t Extflash_HeadEraseIsComplete(void);

//外部FLASH头部信息操作正忙
uint8_t Extflash_HeadIsBusy(void);


//all FLASH
//外部FLASH处理函数
void Extflash_AllOptDeal(void);

//设置外部FLASH写参数
uint8_t Extflash_AllOptWriteSet(uint32_t write_offset, uint8_t *data, uint16_t data_len);

//外部FLASH写完成
uint8_t Extflash_AllOptWriteIsComplete(void);

//设置外部FLASH读参数
uint8_t Extflash_AllOptReadSet(uint32_t read_offset, uint8_t *read_buf, uint16_t read_len);

//外部FLASH读完成
uint8_t Extflash_AllOptReadIsComplete(void);

//设置外部FLASH擦除的参数
uint8_t Extflash_AllOptEraseSizeSet(uint32_t erase_offset, uint32_t erase_size);

//外部FLASH擦除完成
uint8_t Extflash_AllOptEraseBlockIsComplete(void);

//设置外部FLASH擦除所有数据
uint8_t Extflash_AllOptEraseAllSet(void);

//外部FLASH擦除所有数据完成
uint8_t Extflash_AllOptEraseAllIsComplete(void);

//外部FLASH操作正忙
uint8_t Extflash_AllOptIsBusy(void);
#endif
