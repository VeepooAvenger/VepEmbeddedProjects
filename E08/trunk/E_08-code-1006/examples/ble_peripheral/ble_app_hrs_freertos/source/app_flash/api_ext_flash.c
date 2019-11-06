/*******************Copyright(c)*********************************
** Veepoo Tech
** COPYRIGHT (C) 2016-2019
**----------------------FileInfo---------------------------------
** File name:                  api_ext_flash.c
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
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include "ext_flash.h"
#include "gui_thread.h"
#include "data_manage.h"
#include "api_flash.h"
#include "flash_data_type.h"
#include "SystemTime.h"
#include "api_ext_flash.h"

#include "movement_patterns.h"
#include "ble_thread.h"
#include "configuration.h"
#include "watch_dog.h"

#include "test_interface.h"



static uint8_t Extflash_AllOptEraseAllOpt(void);


#if USED_EXT_FLASH_ECG_AUTO_ENABLED  
static uint8_t ecg_stroge_id = 1;
/****************************************************************
** Function name:           Extflash_AllInfoInit
** Descriptions:            FLASH所有操作初始化函数
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              YX              
** Created Date:            2018-12-26
*****************************************************************/ 
void Extflash_AllInfoInit(void)
{
    p_data_manage->ext_flash_ecg_opt_s.init = 1;
    p_data_manage->ext_flash_movement_opt_s.init = 1;
    p_data_manage->ext_flash_ecg_rest_opt_s.init = 1;
    p_data_manage->ext_flash_daily_opt_s.init = 1;
    p_data_manage->ext_flash_sleep_opt_s.init = 1;
    p_data_manage->ext_flash_head_opt_s.init = 1;
    p_data_manage->ext_flash_all_opt_s.init = 1;
    BleThread_ExtFlashOperationSem();
}

/****************************************************************
** Function name:           Extflash_EaresEcgData
** Descriptions:            擦除ECG数据
** input parameters:        index：存储id，最大：ECG_RECORD_MAX_CNT/2
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2018-11-05
*****************************************************************/ 
static void Extflash_EaresEcgData(uint8_t index)
{
    uint16_t start_sector = 0;
    
    start_sector = ECG_EXT_FLASH_BASE_ADDR/4096;
    
    start_sector += index * 3;
    
    for(uint8_t i = 0; i < 3; i++)
    {
        Extflash_DataSectorErase4K(start_sector + i);
        // WJ.20190216.时间过长
        App_Watch_Dog_Reload();
    }
}

/****************************************************************
** Function name:           Extflash_EaresEcgDataAll
** Descriptions:            擦除ECG全部数据
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2018-11-05
*****************************************************************/ 
static void Extflash_EaresEcgDataAll(void)
{
    for(uint8_t i = 0; i < ECG_RECORD_MAX_CNT/2; i++)
    {
        Extflash_EaresEcgData(i);
    }
    
    ecg_stroge_id = 1;
}

/****************************************************************
** Function name:           Extflash_ReadEcgData
** Descriptions:            读取ECG指定偏移地址和长度的数据，用于app数据发送
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2018-11-05
*****************************************************************/ 
static void Extflash_ReadEcgData(uint8_t record_id, uint16_t offset_index, uint8_t data_len, uint8_t *data_buf)
{
    uint32_t    ecg_ext_addr = 0;
    
    if(record_id >= ECG_RECORD_MAX_CNT)
    {
        return;
    }    
    
    //存储偏移开始地址
    ecg_ext_addr = ECG_EXT_FLASH_BASE_ADDR + record_id * ECG_ONE_RECORD_SIZE;     
    ecg_ext_addr += offset_index;
    Extflash_DataRead(ecg_ext_addr, data_len, data_buf);
}

/****************************************************************
** Function name:           Extflash_StrogeEcgData
** Descriptions:            存储ECG数据
** input parameters:        origina_data：adc;data_len：数据长度；
                            ecg_result：计算结果；result_len：数据长度
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2018-11-05
*****************************************************************/ 
static void Extflash_StrogeEcgData(uint8_t *origina_data, uint16_t data_len, uint8_t *ecg_result, uint16_t result_len)
{
    uint8_t     ecg_ext_index = 0;
    uint32_t    ecg_ext_addr = 0;
    uint32_t    ecg_ext_offset = 0;
    uint8_t     ecg_temp_buf[50] = {0,};
    
    ecg_ext_index = p_flash_param->flash_index_s.ecg_ext.offset_ext_index;
    
    if(ecg_ext_index >= ECG_RECORD_MAX_CNT)
    {
        // 超出，本次不存储,擦除下一块，为下次存储准备
        Extflash_EaresEcgData(0);  
        p_flash_param->flash_index_s.ecg_ext.offset_ext_index = 0;
        return;
    }
    //存储偏移开始地址
    ecg_ext_addr = ECG_EXT_FLASH_BASE_ADDR + ecg_ext_index * ECG_ONE_RECORD_SIZE;    
    //存储原始数据
    ecg_ext_offset = ecg_ext_addr + ECG_ORIGINA_DATA_ADC_INDEX;
    Extflash_DataWrite(ecg_ext_offset, data_len, origina_data);
    //存储结果
    memset(ecg_temp_buf, 0xff, sizeof(ecg_temp_buf)); 
    ecg_temp_buf[0] = 0xA3;                 //数据类型标志
    ecg_temp_buf[1] = ECG_RESULT_DATA_LEN;  //数据长度    
    memcpy(&ecg_temp_buf[2], ecg_result, ECG_RESULT_DATA_LEN);

    ecg_temp_buf[ECG_RESULT_DATA_LEN + 2 + 0] = 0xA4;   //数据类型标志
    ecg_temp_buf[ECG_RESULT_DATA_LEN + 2 + 1] = 5;      //数据长度    
    ecg_temp_buf[ECG_RESULT_DATA_LEN + 2 + 2] = 250 & 0xFF;         //数据采样频率
    ecg_temp_buf[ECG_RESULT_DATA_LEN + 2 + 3] = (250>>8) & 0xFF;    //数据采样频率
    ecg_temp_buf[ECG_RESULT_DATA_LEN + 2 + 4] = 250 & 0xFF;         //数据采样频率
    ecg_temp_buf[ECG_RESULT_DATA_LEN + 2 + 5] = (250>>8) & 0xFF;    //数据采样频率
    ecg_temp_buf[ECG_RESULT_DATA_LEN + 2 + 6] = 8;                  //波形放大倍数
    
    
    ecg_ext_offset = ecg_ext_addr + ECG_RESULT_INDEX;
    Extflash_DataWrite(ecg_ext_offset, ECG_RESULT_DATA_LEN + 2 + 7, ecg_temp_buf);

    //存储头部
    memset(ecg_temp_buf, 0xff, sizeof(ecg_temp_buf)); 
    ecg_temp_buf[0] = ecg_stroge_id;        //数据类型标志
    ecg_temp_buf[1] = 0xA1;                 //数据类型标志
    ecg_temp_buf[2] = 8;                    //数据长度
    ecg_temp_buf[3] = clock->year & 0xFF;       //年，低字节
    ecg_temp_buf[4] = (clock->year>>8) & 0xFF;  //年，高字节
    ecg_temp_buf[5] = clock->month;         //年
    ecg_temp_buf[6] = clock->day;           //年
    ecg_temp_buf[7] = clock->hour;          //年
    ecg_temp_buf[8] = clock->minutes;       //年
    ecg_temp_buf[9] = clock->seconds;       //年
    ecg_temp_buf[10] = 6;                   //数据总秒
    ecg_temp_buf[11] = 0xA2;                //原始数据区域标志
    ecg_temp_buf[12] = ECG_ORIGINA_DATA_ADC_LEN & 0xFF;         //原始数据长度，低字节
    ecg_temp_buf[13] = (ECG_ORIGINA_DATA_ADC_LEN>>8) & 0xFF;    //原始数据长度，高字节
 
    ecg_ext_offset = ecg_ext_addr;
    Extflash_DataWrite(ecg_ext_offset, 14, ecg_temp_buf);   

    //判断是否擦除下一块
    ecg_ext_index = ++p_flash_param->flash_index_s.ecg_ext.offset_ext_index;
    if(ecg_ext_index >= ECG_RECORD_MAX_CNT)
    {
        ecg_ext_index = 0;
        p_flash_param->flash_index_s.ecg_ext.offset_ext_index = 0;
        
        p_flash_param->flash_index_s.ecg_ext.ecg_cnt = ECG_RECORD_MAX_CNT - 2;  
    }
    else
    {
        if(p_flash_param->flash_index_s.ecg_ext.ecg_cnt >= ECG_RECORD_MAX_CNT - 1)
        {
            p_flash_param->flash_index_s.ecg_ext.ecg_cnt = ECG_RECORD_MAX_CNT - 2; 
        }
        else
        {
            p_flash_param->flash_index_s.ecg_ext.ecg_cnt++;
        }
    }
    
    if(ecg_ext_index % 2 == 0)
    {   //需要换块
        Extflash_EaresEcgData(ecg_ext_index / 2);
    }
    
    ecg_stroge_id = (++ecg_stroge_id) % 200;
}

/****************************************************************
** Function name:           Extflash_InitEcgIndex
** Descriptions:            ECG存储索引初始化
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2018-11-05
*****************************************************************/ 
static void Extflash_InitEcgIndex(ecg_ext_data_index_t *p_index)
{
    uint8_t ecg_read_id[ECG_RECORD_MAX_CNT] = {0};
    uint8_t ecg_read_buf[20] = {0};
    uint8_t id_cnt = 0;
            
    ecg_stroge_id = 1;
    p_index->offset_ext_index = 0;
    
    for(uint8_t i = 0; i < ECG_RECORD_MAX_CNT; i++)
    {
        memset(ecg_read_buf, 0, sizeof(ecg_read_buf));
        Extflash_ReadEcgData(i, 0, 20, ecg_read_buf);
        
        ecg_read_id[i] = ecg_read_buf[0];
        if(ecg_read_id[i] != 0xFF)
        {
            id_cnt++;
        }
    }
    
    if(id_cnt == 0)
    {
        ecg_stroge_id = 1;
        p_index->offset_ext_index = 0;
        p_index->ecg_cnt = 0;        
        return;
    }
    
    for(uint8_t i = 0; i < ECG_RECORD_MAX_CNT; i++)
    {
        if(ecg_read_id[i] == 0xFF)
        {
            p_index->offset_ext_index = i;           
            p_index->ecg_cnt = id_cnt;
            
            if(i > 0)
            {
                ecg_stroge_id = (ecg_read_id[i-1] + 1) % 200;
            }
            else
            {
                if(ecg_read_id[ECG_RECORD_MAX_CNT - 1] == 0xFF)
                {
                    ecg_stroge_id = 1;
                    p_index->offset_ext_index = 0;
                    p_index->ecg_cnt = 0;
                }
                else
                {
                    ecg_stroge_id = (ecg_read_id[ECG_RECORD_MAX_CNT - 1] + 1) % 200;
                }                                       
            }
            break;
        }
    }    
}

#endif //USED_EXT_FLASH_ECG_AUTO_ENABLED  

/****************************************************************
** Function name:           Extflash_EcgErrDeal
** Descriptions:            ECG处理
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2018-11-05
*****************************************************************/ 
void Extflash_EcgErrDeal(void)
{
#if USED_EXT_FLASH_ECG_AUTO_ENABLED   
    ext_flash_ecg_opt_t *p_ext_flash_opt = NULL;
    
    p_ext_flash_opt = &p_data_manage->ext_flash_ecg_opt_s;
 
    if(p_ext_flash_opt->init)
    {
        // WJ.20181114.增加ecg外部flash存储初始化索引
        Extflash_InitEcgIndex(&p_flash_param->flash_index_s.ecg_ext);   
        p_ext_flash_opt->init = 0;
        
        memset(&p_data_manage->ext_flash_ecg_opt_s, 0, sizeof(p_data_manage->ext_flash_ecg_opt_s));
        return;
    }
    
    if(p_ext_flash_opt->factory_init)
    {
        Extflash_EaresEcgDataAll();
        
        p_flash_param->flash_index_s.ecg_ext.ecg_cnt = 0;                // ECG 自动测量异常数据偏移量   
        p_flash_param->flash_index_s.ecg_ext.offset_ext_index = 0;       // ECG 自动测量异常数据偏移量  
        
        p_ext_flash_opt->factory_init = 0;
        
        memset(&p_data_manage->ext_flash_ecg_opt_s, 0, sizeof(p_data_manage->ext_flash_ecg_opt_s));
        return;
    }
    
    
    if(p_ext_flash_opt->write)
    {
        // 注意目前长度没有使用，内部固定长度
        Extflash_StrogeEcgData(p_ext_flash_opt->ecg_write_adc, p_ext_flash_opt->ecg_write_adc_len, 
                               p_ext_flash_opt->ecg_write_result, p_ext_flash_opt->ecg_write_result_len);
        p_ext_flash_opt->write = 0;    
    }
    
    if(p_ext_flash_opt->read)
    {
        Extflash_ReadEcgData(p_ext_flash_opt->record_id, p_ext_flash_opt->offset_index, 
                             p_ext_flash_opt->read_len, p_ext_flash_opt->read_data);        
        p_ext_flash_opt->read = 0; 
    }
#else
    memset(&p_data_manage->ext_flash_ecg_opt_s, 0, sizeof(ext_flash_ecg_opt_t));    
#endif    
}

/****************************************************************
** Function name:           Extflash_EcgReadSet
** Descriptions:            ECG 读取参数处理
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2018-11-07
*****************************************************************/ 
uint8_t Extflash_EcgReadSet(uint8_t record_id, uint16_t offset_index, uint8_t data_len, uint8_t *data_buf)
{
#if USED_EXT_FLASH_ECG_AUTO_ENABLED       
    ext_flash_ecg_opt_t *p_ext_flash_opt = NULL;
    
    p_ext_flash_opt = &p_data_manage->ext_flash_ecg_opt_s;

    if(p_ext_flash_opt->read)
    {
        BleThread_ExtFlashOperationSem();
        return 0;
    }
    else
    {
        p_ext_flash_opt->record_id = record_id;
        p_ext_flash_opt->offset_index = offset_index;
        p_ext_flash_opt->read_len = data_len;
        p_ext_flash_opt->read_data = data_buf;
        
        p_ext_flash_opt->read = 1;
        BleThread_ExtFlashOperationSem();
    }
#endif    
    return 1;
}

/****************************************************************
** Function name:           Extflash_EcgReadIsComplete
** Descriptions:            ECG 读取完成
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2018-11-07
*****************************************************************/ 
uint8_t Extflash_EcgReadIsComplete(void)
{
#if USED_EXT_FLASH_ECG_AUTO_ENABLED       
    ext_flash_ecg_opt_t *p_ext_flash_opt = NULL;
    
    p_ext_flash_opt = &p_data_manage->ext_flash_ecg_opt_s;

    if(p_ext_flash_opt->read)
    {
        BleThread_ExtFlashOperationSem();
        return 0;
    }
#endif    
    return 1;
}

/****************************************************************
** Function name:           Extflash_EcgWriteSet
** Descriptions:            ECG 读取参数处理
** input parameters:        adc_data：adc数据；adc_data_len：adc长度；
                            ecg_result：ecg结果；ecg_result_len：长度
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2018-11-07
*****************************************************************/ 
uint8_t Extflash_EcgWriteSet(uint8_t *adc_data, uint16_t adc_data_len, uint8_t *ecg_result, uint16_t ecg_result_len)
{

#if USED_EXT_FLASH_ECG_AUTO_ENABLED   
    ext_flash_ecg_opt_t *p_ext_flash_opt = NULL;
    
    p_ext_flash_opt = &p_data_manage->ext_flash_ecg_opt_s;

    if(p_ext_flash_opt->write)
    {
        return 0;
    }
    else
    { 
        p_ext_flash_opt->ecg_write_adc = adc_data;
        p_ext_flash_opt->ecg_write_adc_len = adc_data_len;
                               
        p_ext_flash_opt->ecg_write_result = ecg_result;
        p_ext_flash_opt->ecg_write_result_len = ecg_result_len;

        p_ext_flash_opt->write = 1;
        BleThread_ExtFlashOperationSem();
    }
#endif
    
    return 1;
}

/****************************************************************
** Function name:           Extflash_EcgWriteIsComplete
** Descriptions:            ECG 写完成
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2018-11-08
*****************************************************************/ 
uint8_t Extflash_EcgWriteIsComplete(void)
{

#if USED_EXT_FLASH_ECG_AUTO_ENABLED       
    ext_flash_ecg_opt_t *p_ext_flash_opt = NULL;
    
    p_ext_flash_opt = &p_data_manage->ext_flash_ecg_opt_s;

    if(p_ext_flash_opt->write)
    {
        return 0;
    }
#endif    
    return 1;
}

/****************************************************************
** Function name:           Extflash_SetEcgIndexInit
** Descriptions:            索引初始化
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ                   
** Created Date:            2018-11-14
*****************************************************************/ 
uint8_t Extflash_SetEcgIndexInit(void)
{
#if USED_EXT_FLASH_ECG_AUTO_ENABLED       
    ext_flash_ecg_opt_t *p_ext_flash_opt = NULL;
    
    p_ext_flash_opt = &p_data_manage->ext_flash_ecg_opt_s;
    
    p_ext_flash_opt->init = 1;
    
    BleThread_ExtFlashOperationSem();
#endif    
    return 1;
}

/****************************************************************
** Function name:           Extflash_SetEcgFactoryInit
** Descriptions:            索引恢复出厂
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2018-11-14
*****************************************************************/ 
uint8_t Extflash_SetEcgFactoryInit(void)
{
#if USED_EXT_FLASH_ECG_AUTO_ENABLED       
    ext_flash_ecg_opt_t *p_ext_flash_opt = NULL;
    
    p_ext_flash_opt = &p_data_manage->ext_flash_ecg_opt_s;
    
    p_ext_flash_opt->factory_init = 1;
    
    BleThread_ExtFlashOperationSem();
#endif    
    return 1;
}

/****************************************************************
** Function name:           Extflash_EcgIsBusy
** Descriptions:            Flash操作是否忙碌，
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2018-11-19
*****************************************************************/ 
uint8_t Extflash_EcgIsBusy(void)
{ 
#if USED_EXT_FLASH_ECG_AUTO_ENABLED       
    ext_flash_ecg_opt_t *p_ext_flash_opt = NULL;
    
    p_ext_flash_opt = &p_data_manage->ext_flash_ecg_opt_s;
    
    if(p_ext_flash_opt->init || p_ext_flash_opt->factory_init 
      || p_ext_flash_opt->write || p_ext_flash_opt->read)
    {
        return 1;
    }    
#endif    
    return 0;
}




/****************************************************************
** Function name:           Extflash_MovementWriteHead
** Descriptions:            运动模式写头数据
** input parameters:        block_count：块数
                            head_data：头数据地址
                            head_data_len：头数据长度
** output parameters:       无
** Returned value:          无
** Created by:              YX              
** Created Date:            2018-12-26
*****************************************************************/ 
static uint8_t Extflash_MovementWriteHead(uint8_t block_count, uint8_t *head_data, uint16_t head_data_len)
{
    if(block_count >= MOVEMENT_PATTERNS_DATA_BLOCK_MAX || head_data == NULL || head_data_len == 0 
      || head_data_len > RECORD_MOVEMENT_HEAD_OFFSET * 4)
    {
        return 0;
    }
    return Extflash_DataWrite(RECORD_MOVEMENT_HEAD_ADDR(block_count,0), head_data_len, head_data);
}

/****************************************************************
** Function name:           Extflash_MovementWriteData
** Descriptions:            运动模式写数据
** input parameters:        block_count：块数
                            number：第几条
                            data：数据地址
                            data_len：数据长度
** output parameters:       无
** Returned value:          无
** Created by:              YX              
** Created Date:            2018-12-26
*****************************************************************/ 
static uint8_t Extflash_MovementWriteData(uint8_t block_count, uint16_t number, uint8_t *data, uint16_t data_len)
{
    if(block_count >= MOVEMENT_PATTERNS_DATA_BLOCK_MAX || data == NULL 
      || data_len == 0 || data_len > 20)
    {
        return 0;
    }
    return Extflash_DataWrite(RECORD_MOVEMENT_DATA_ADDR(block_count,number), data_len, data);
}

/****************************************************************
** Function name:           Extflash_MovementReadHead
** Descriptions:            运动模式读头数据
** input parameters:        block_count：块数
                            head_read_buf：数据地址
                            head_read_len：数据长度
** output parameters:       无
** Returned value:          无
** Created by:              YX              
** Created Date:            2018-12-26
*****************************************************************/ 
static uint8_t Extflash_MovementReadHead(uint8_t block_count, uint8_t *head_read_buf, uint16_t head_read_len)
{
    if(block_count >= MOVEMENT_PATTERNS_DATA_BLOCK_MAX || head_read_buf == NULL 
      || head_read_len == 0 
      || head_read_len > RECORD_MOVEMENT_HEAD_OFFSET * 4)
    {
        return 0;
    }
    
    return Extflash_DataRead(RECORD_MOVEMENT_HEAD_ADDR(block_count,0), head_read_len, head_read_buf);
}

/****************************************************************
** Function name:           Extflash_MovementReadData
** Descriptions:            运动模式读数据
** input parameters:        block_count：块数
                            number：第几条
                            head_read_buf：数据地址
                            head_read_len：数据长度
** output parameters:       无
** Returned value:          无
** Created by:              YX              
** Created Date:            2018-12-26
*****************************************************************/ 
static uint8_t Extflash_MovementReadData(uint8_t block_count, uint16_t number, uint8_t *data, uint16_t data_len)
{
    if(block_count >= MOVEMENT_PATTERNS_DATA_BLOCK_MAX || data == NULL)
    {
        return 0;
    }
    
    return Extflash_DataRead(RECORD_MOVEMENT_DATA_ADDR(block_count,number), data_len, data);
}

/****************************************************************
** Function name:           Extflash_MovementErasePage
** Descriptions:            运动模式擦除一次运动模式的数据
** input parameters:        block_count：块数
** output parameters:       无
** Returned value:          无
** Created by:              YX              
** Created Date:            2018-12-26
*****************************************************************/
static uint8_t Extflash_MovementErasePage(uint8_t block_count)
{
    uint16_t start_sector = 0;
    uint16_t sector_block = 0;
    
    if(block_count >= MOVEMENT_PATTERNS_DATA_BLOCK_MAX)
    {
        return 0;
    }

    start_sector = RECORD_MOVEMENT_HEAD_ADDR(block_count,0)/4096;
    sector_block = RECORD_MOVEMENT_ONE_TIMES_SIZE/4096;
    
    for(uint8_t i = 0; i < sector_block; i++)
    {
        Extflash_DataSectorErase4K(start_sector + i);
        // WJ.20190216.时间过长
        App_Watch_Dog_Reload();
    }
    
    return 1;
}

/****************************************************************
** Function name:           Extflash_MovementEraseAllPage
** Descriptions:            运动模式擦除所有运动模式的数据
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              YX              
** Created Date:            2018-12-26
*****************************************************************/
static uint8_t Extflash_MovementEraseAllPage(void)
{
    uint16_t start_sector = 0;
    uint16_t sector_block = 0;

    start_sector = RECORD_MOVEMENT_HEAD_ADDR(0,0)/4096;
    sector_block = RECORD_MOVEMENT_TOTAL_SIZE/4096;
    
    for(uint8_t i = 0; i < sector_block; i++)
    {
        Extflash_DataSectorErase4K(start_sector + i);
        // WJ.20190216.时间过长
        App_Watch_Dog_Reload();
    }
    
    return 1;
}

/****************************************************************
** Function name:           Extflash_MovementDeal
** Descriptions:            运动模式数据处理
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              YX              
** Created Date:            2018-12-26
*****************************************************************/
void Extflash_MovementDeal(void)
{
    ext_flash_movement_opt_t *p_ext_flash_opt = NULL;
    
    p_ext_flash_opt = &p_data_manage->ext_flash_movement_opt_s;
 
    if(p_ext_flash_opt->init)
    {
        memset(&p_data_manage->ext_flash_movement_opt_s, 0, sizeof(p_data_manage->ext_flash_movement_opt_s));
        return;
    }
    
    if(p_ext_flash_opt->write_head)
    {
        Extflash_MovementWriteHead(p_ext_flash_opt->write_head_block, 
                                   p_ext_flash_opt->p_write_head, p_ext_flash_opt->write_head_len);
        p_ext_flash_opt->write_head = 0;
        return;
    }
    
    if(p_ext_flash_opt->write_data)
    {
        Extflash_MovementWriteData(p_ext_flash_opt->write_data_block, p_ext_flash_opt->write_data_number,
                                   p_ext_flash_opt->p_write_data, p_ext_flash_opt->write_data_len); 
        p_ext_flash_opt->write_data = 0;
        return;
    }
    
    if(p_ext_flash_opt->read_head)
    {
        Extflash_MovementReadHead(p_ext_flash_opt->read_head_block, 
                                   p_ext_flash_opt->p_read_head, p_ext_flash_opt->read_head_len);  
        p_ext_flash_opt->read_head = 0;
        return;
    }
    
    if(p_ext_flash_opt->read_data)
    {
        Extflash_MovementReadData(p_ext_flash_opt->read_data_block, p_ext_flash_opt->read_data_number,
                                   p_ext_flash_opt->p_read_data, p_ext_flash_opt->read_data_len);
        p_ext_flash_opt->read_data = 0;
        return;
    }
    
    if(p_ext_flash_opt->erase_block)
    {
        Extflash_MovementErasePage(p_ext_flash_opt->erase_block_cnt);  
        p_ext_flash_opt->erase_block = 0;
        return;
    }
    if(p_ext_flash_opt->erase_all)
    {
        Extflash_MovementEraseAllPage();  
        p_ext_flash_opt->erase_all = 0;
        return;
    } 
}

/****************************************************************
** Function name:           Extflash_MovementWriteHeadSet
** Descriptions:            运动模式写头数据设置参数
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              YX              
** Created Date:            2018-12-26
*****************************************************************/
uint8_t Extflash_MovementWriteHeadSet(uint8_t block_count, uint8_t *head_data, uint16_t head_data_len)
{   
    ext_flash_movement_opt_t *p_ext_flash_opt = NULL;
    
    p_ext_flash_opt = &p_data_manage->ext_flash_movement_opt_s;

    if(p_ext_flash_opt->write_head)
    {
        //唤醒线程，执行写操作
        BleThread_ExtFlashOperationSem();
        return 0;
    }
    else
    {
        p_ext_flash_opt->p_write_head = head_data;
        p_ext_flash_opt->write_head_block = block_count;
        p_ext_flash_opt->write_head_len = head_data_len;
        
        p_ext_flash_opt->write_head = 1;
        BleThread_ExtFlashOperationSem();
    }
    return 1;
}

/****************************************************************
** Function name:           Extflash_MovementWriteHeadIsComplete
** Descriptions:            运动模式头数据操作完成
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              YX              
** Created Date:            2018-12-26
*****************************************************************/
uint8_t Extflash_MovementWriteHeadIsComplete(void)
{    
    ext_flash_movement_opt_t *p_ext_flash_opt = NULL;
    
    p_ext_flash_opt = &p_data_manage->ext_flash_movement_opt_s;

    if(p_ext_flash_opt->write_head)
    {
        BleThread_ExtFlashOperationSem();
        return 0;
    }
    return 1;
}

/****************************************************************
** Function name:           Extflash_MovementWriteDataSet
** Descriptions:            运动模式写数据设置设置参数
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              YX              
** Created Date:            2018-12-26
*****************************************************************/
uint8_t Extflash_MovementWriteDataSet(uint8_t block_count, uint16_t number, uint8_t *data, uint16_t data_len)
{ 
    ext_flash_movement_opt_t *p_ext_flash_opt = NULL;
    
    p_ext_flash_opt = &p_data_manage->ext_flash_movement_opt_s;

    if(p_ext_flash_opt->write_data)
    {
        //唤醒线程，执行写操作
        BleThread_ExtFlashOperationSem();
        return 0;
    }
    else
    {
        p_ext_flash_opt->p_write_data = data;
        p_ext_flash_opt->write_data_block = block_count;
        p_ext_flash_opt->write_data_number = number;
        p_ext_flash_opt->write_data_len = data_len;
        
        p_ext_flash_opt->write_data = 1;
        BleThread_ExtFlashOperationSem();
    }
    return 1;
}

/****************************************************************
** Function name:           Extflash_MovementWriteDataIsComplete
** Descriptions:            运动模式写数据完成
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              YX              
** Created Date:            2018-12-26
*****************************************************************/
uint8_t Extflash_MovementWriteDataIsComplete(void)
{
    ext_flash_movement_opt_t *p_ext_flash_opt = NULL;
    
    p_ext_flash_opt = &p_data_manage->ext_flash_movement_opt_s;

    if(p_ext_flash_opt->write_data)
    {
        BleThread_ExtFlashOperationSem();
        return 0;
    }
    return 1;
}

/****************************************************************
** Function name:           Extflash_MovementReadHeadSet
** Descriptions:            运动模式读取头数据设置参数
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              YX              
** Created Date:            2018-12-26
*****************************************************************/
uint8_t Extflash_MovementReadHeadSet(uint8_t block_count, uint8_t *head_read_buf, uint16_t head_read_len)
{   
    ext_flash_movement_opt_t *p_ext_flash_opt = NULL;
    
    p_ext_flash_opt = &p_data_manage->ext_flash_movement_opt_s;

    if(p_ext_flash_opt->read_head)
    {
        //唤醒线程，执行写操作
        BleThread_ExtFlashOperationSem();
        return 0;
    }
    else
    {
        p_ext_flash_opt->p_read_head = head_read_buf;
        p_ext_flash_opt->read_head_block = block_count;
        p_ext_flash_opt->read_head_len = head_read_len;
        
        p_ext_flash_opt->read_head = 1;
        BleThread_ExtFlashOperationSem();
    }
    return 1;
}

/****************************************************************
** Function name:           Extflash_MovementReadHeadIsComplete
** Descriptions:            运动模式读取头数据完成
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              YX              
** Created Date:            2018-12-26
*****************************************************************/
uint8_t Extflash_MovementReadHeadIsComplete(void)
{    
    ext_flash_movement_opt_t *p_ext_flash_opt = NULL;
    
    p_ext_flash_opt = &p_data_manage->ext_flash_movement_opt_s;

    if(p_ext_flash_opt->read_head)
    {
        BleThread_ExtFlashOperationSem();
        return 0;
    }
    return 1;
}

/****************************************************************
** Function name:           Extflash_MovementReadDataSet
** Descriptions:            运动模式读取数据设置参数
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              YX              
** Created Date:            2018-12-26
*****************************************************************/
uint8_t Extflash_MovementReadDataSet(uint8_t block_count, uint16_t number, uint8_t *data, uint16_t data_len)
{ 
    ext_flash_movement_opt_t *p_ext_flash_opt = NULL;
    
    p_ext_flash_opt = &p_data_manage->ext_flash_movement_opt_s;

    if(p_ext_flash_opt->read_data)
    {
        //唤醒线程，执行写操作
        BleThread_ExtFlashOperationSem();
        return 0;
    }
    else
    {
        p_ext_flash_opt->p_read_data = data;
        p_ext_flash_opt->read_data_block = block_count;
        p_ext_flash_opt->read_data_number = number;
        p_ext_flash_opt->read_data_len = data_len;
        
        p_ext_flash_opt->read_data = 1;
        BleThread_ExtFlashOperationSem();
    }
    return 1;
}

/****************************************************************
** Function name:           Extflash_MovementReadDataIsComplete
** Descriptions:            运动模式读取数据完成
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              YX              
** Created Date:            2018-12-26
*****************************************************************/
uint8_t Extflash_MovementReadDataIsComplete(void)
{
    ext_flash_movement_opt_t *p_ext_flash_opt = NULL;
    
    p_ext_flash_opt = &p_data_manage->ext_flash_movement_opt_s;

    if(p_ext_flash_opt->read_data)
    {
        BleThread_ExtFlashOperationSem();
        return 0;
    }
    return 1;
}

/****************************************************************
** Function name:           Extflash_MovementEraseBlockSet
** Descriptions:            运动模式擦除一块数据设置
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              YX              
** Created Date:            2018-12-26
*****************************************************************/
uint8_t Extflash_MovementEraseBlockSet(uint8_t block_count)
{ 
    ext_flash_movement_opt_t *p_ext_flash_opt = NULL;
    
    p_ext_flash_opt = &p_data_manage->ext_flash_movement_opt_s;

    if(p_ext_flash_opt->erase_block)
    {
        //唤醒线程，执行写操作
        BleThread_ExtFlashOperationSem();
        return 0;
    }
    else
    {
        p_ext_flash_opt->erase_block_cnt = block_count;
        p_ext_flash_opt->erase_block = 1;
        BleThread_ExtFlashOperationSem();
    }
    return 1;
}

/****************************************************************
** Function name:           Extflash_MovementEraseBlockIsComplete
** Descriptions:            运动模式擦除一块数据完成
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              YX              
** Created Date:            2018-12-26
*****************************************************************/
uint8_t Extflash_MovementEraseBlockIsComplete(void)
{
    ext_flash_movement_opt_t *p_ext_flash_opt = NULL;
    
    p_ext_flash_opt = &p_data_manage->ext_flash_movement_opt_s;

    if(p_ext_flash_opt->erase_block)
    {
        BleThread_ExtFlashOperationSem();
        return 0;
    }
    return 1;
}

/****************************************************************
** Function name:           Extflash_MovementEraseAllSet
** Descriptions:            运动模式擦除所有数据设置
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              YX              
** Created Date:            2018-12-26
*****************************************************************/
uint8_t Extflash_MovementEraseAllSet(void )
{ 
    ext_flash_movement_opt_t *p_ext_flash_opt = NULL;
    
    p_ext_flash_opt = &p_data_manage->ext_flash_movement_opt_s;

    if(p_ext_flash_opt->erase_all)
    {
        //唤醒线程，执行写操作
        BleThread_ExtFlashOperationSem();
        return 0;
    }
    else
    {
        p_ext_flash_opt->erase_all = 1;
        BleThread_ExtFlashOperationSem();
    }
    return 1;
}

/****************************************************************
** Function name:           Extflash_MovementEraseAllIsComplete
** Descriptions:            运动模式擦除所有数据完成
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              YX              
** Created Date:            2018-12-26
*****************************************************************/
uint8_t Extflash_MovementEraseAllIsComplete(void)
{
    ext_flash_movement_opt_t *p_ext_flash_opt = NULL;
    
    p_ext_flash_opt = &p_data_manage->ext_flash_movement_opt_s;

    if(p_ext_flash_opt->erase_all)
    {
        BleThread_ExtFlashOperationSem();
        return 0;
    }
    return 1;
}

/****************************************************************
** Function name:           Extflash_MovementIsBusy
** Descriptions:            运动模式操作是否正忙
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              YX              
** Created Date:            2018-12-26
*****************************************************************/
uint8_t Extflash_MovementIsBusy(void)
{       
    ext_flash_movement_opt_t *p_ext_flash_opt = NULL;
    
    p_ext_flash_opt = &p_data_manage->ext_flash_movement_opt_s;
    
    if(p_ext_flash_opt->init || p_ext_flash_opt->write_head 
      || p_ext_flash_opt->write_data || p_ext_flash_opt->read_head
      || p_ext_flash_opt->read_data || p_ext_flash_opt->erase_block
      || p_ext_flash_opt->erase_all)
    {
        return 1;
    } 
    return 0;
}

/****************************************************************
** Function name:           Extflash_EcgRestWrite
** Descriptions:            写ECG结果数据
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              YX              
** Created Date:            2018-12-26
*****************************************************************/
static uint8_t Extflash_EcgRestWrite(uint16_t block_count, uint8_t *data, uint16_t data_len)
{
    if(!RECORD_ECG_CONDITION_VALID(block_count))
    {
       return 0;
    }
    return Extflash_DataWrite(RECORD_ECG_DATA_ADDR(block_count), data_len, data);
}

/****************************************************************
** Function name:           Extflash_EcgRestRead
** Descriptions:            读取ECG结果数据
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              YX              
** Created Date:            2018-12-26
*****************************************************************/
static uint8_t Extflash_EcgRestRead(uint16_t block_count, uint8_t *data, uint16_t data_len)
{
    if(!RECORD_ECG_CONDITION_VALID(block_count))
    {
       return 0;
    }
    return Extflash_DataRead(RECORD_ECG_DATA_ADDR(block_count), data_len, data);
}

/****************************************************************
** Function name:           Extflash_EcgRestEraseBlock
** Descriptions:            擦除ECG结果数据
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              YX              
** Created Date:            2018-12-26
*****************************************************************/
static uint8_t Extflash_EcgRestEraseBlock(uint8_t block_id, uint8_t block_num)
{
    uint16_t start_sector = 0;
    
    if(block_id + block_num > 2 || block_num == 0)
    {
       return 0;
    }
    
    start_sector = RECORD_ECG_DATA_ERASE_ADDR(block_id)/4096;
    for(uint8_t i = 0; i < block_num; i++)
    {
        Extflash_DataSectorErase4K(start_sector + i);
        // WJ.20190216.时间过长
        App_Watch_Dog_Reload();
    }
    
    return 1;
}

/****************************************************************
** Function name:           Extflash_EcgRestDeal
** Descriptions:            ECG结果处理函数
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              YX              
** Created Date:            2018-12-26
*****************************************************************/
void Extflash_EcgRestDeal(void)
{
    ext_flash_ecg_rest_opt_t *p_ext_flash_opt = NULL;
    
    p_ext_flash_opt = &p_data_manage->ext_flash_ecg_rest_opt_s;
 
    if(p_ext_flash_opt->init)
    {
        memset(&p_data_manage->ext_flash_ecg_rest_opt_s, 0, sizeof(p_data_manage->ext_flash_ecg_rest_opt_s));
        return;
    }
    
    if(p_ext_flash_opt->write)
    {
        Extflash_EcgRestWrite(p_ext_flash_opt->write_block, 
                                   p_ext_flash_opt->p_write, p_ext_flash_opt->write_len);
        p_ext_flash_opt->write = 0;
        return;
    }
    
    
    if(p_ext_flash_opt->read)
    {
        Extflash_EcgRestRead(p_ext_flash_opt->read_block, 
                                   p_ext_flash_opt->p_read, p_ext_flash_opt->read_len);  
        p_ext_flash_opt->read = 0;
        return;
    }
    
    if(p_ext_flash_opt->erase_block)
    {
        Extflash_EcgRestEraseBlock(p_ext_flash_opt->erase_block_id, p_ext_flash_opt->erase_block_num);  
        p_ext_flash_opt->erase_block = 0;
        return;
    }
}

/****************************************************************
** Function name:           Extflash_EcgRestWriteSet
** Descriptions:            设置写ECG结果参数
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              YX              
** Created Date:            2018-12-26
*****************************************************************/
uint8_t Extflash_EcgRestWriteSet(uint16_t block_count, uint8_t *data, uint16_t data_len)
{   
    ext_flash_ecg_rest_opt_t *p_ext_flash_opt = NULL;
    
    p_ext_flash_opt = &p_data_manage->ext_flash_ecg_rest_opt_s;

    if(p_ext_flash_opt->write)
    {
        //唤醒线程，执行写操作
        BleThread_ExtFlashOperationSem();
        return 0;
    }
    else
    {
        p_ext_flash_opt->p_write = data;
        p_ext_flash_opt->write_block = block_count;
        p_ext_flash_opt->write_len = data_len;
        
        p_ext_flash_opt->write = 1;
        BleThread_ExtFlashOperationSem();
    }
    return 1;
}

/****************************************************************
** Function name:           Extflash_EcgRestWriteIsComplete
** Descriptions:            写ECG结果完成
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              YX              
** Created Date:            2018-12-26
*****************************************************************/
uint8_t Extflash_EcgRestWriteIsComplete(void)
{    
    ext_flash_ecg_rest_opt_t *p_ext_flash_opt = NULL;
    
    p_ext_flash_opt = &p_data_manage->ext_flash_ecg_rest_opt_s;

    if(p_ext_flash_opt->write)
    {
        BleThread_ExtFlashOperationSem();
        return 0;
    }
    return 1;
}

/****************************************************************
** Function name:           Extflash_EcgRestReadSet
** Descriptions:            设置读取ECG结果的参数
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              YX              
** Created Date:            2018-12-26
*****************************************************************/
uint8_t Extflash_EcgRestReadSet(uint16_t block_count, uint8_t *read_buf, uint16_t read_len)
{   
    ext_flash_ecg_rest_opt_t *p_ext_flash_opt = NULL;
    
    p_ext_flash_opt = &p_data_manage->ext_flash_ecg_rest_opt_s;

    if(p_ext_flash_opt->read)
    {
        //唤醒线程，执行写操作
        BleThread_ExtFlashOperationSem();
        return 0;
    }
    else
    {
        p_ext_flash_opt->p_read = read_buf;
        p_ext_flash_opt->read_block = block_count;
        p_ext_flash_opt->read_len = read_len;
        
        p_ext_flash_opt->read = 1;
        BleThread_ExtFlashOperationSem();
    }
    return 1;
}

/****************************************************************
** Function name:           Extflash_EcgRestReadIsComplete
** Descriptions:            读取ECG结果完成
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              YX              
** Created Date:            2018-12-26
*****************************************************************/
uint8_t Extflash_EcgRestReadIsComplete(void)
{    
    ext_flash_ecg_rest_opt_t *p_ext_flash_opt = NULL;
    
    p_ext_flash_opt = &p_data_manage->ext_flash_ecg_rest_opt_s;

    if(p_ext_flash_opt->read)
    {
        BleThread_ExtFlashOperationSem();
        return 0;
    }
    return 1;
}

/****************************************************************
** Function name:           Extflash_EcgRestEraseBlockSet
** Descriptions:            擦除ECG结果数据
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              YX              
** Created Date:            2018-12-26
*****************************************************************/
uint8_t Extflash_EcgRestEraseBlockSet(uint8_t block_id, uint8_t block_num)
{ 
    ext_flash_ecg_rest_opt_t *p_ext_flash_opt = NULL;
    
    p_ext_flash_opt = &p_data_manage->ext_flash_ecg_rest_opt_s;

    if(p_ext_flash_opt->erase_block)
    {
        //唤醒线程，执行写操作
        BleThread_ExtFlashOperationSem();
        return 0;
    }
    else
    {
        p_ext_flash_opt->erase_block_id = block_id;
        p_ext_flash_opt->erase_block_num = block_num;
        p_ext_flash_opt->erase_block = 1;
        BleThread_ExtFlashOperationSem();
    }
    return 1;
}


/****************************************************************
** Function name:           Extflash_EcgRestEraseBlockIsComplete
** Descriptions:            擦除ECG结果数据完成
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              YX              
** Created Date:            2018-12-26
*****************************************************************/
uint8_t Extflash_EcgRestEraseBlockIsComplete(void)
{
    ext_flash_ecg_rest_opt_t *p_ext_flash_opt = NULL;
    
    p_ext_flash_opt = &p_data_manage->ext_flash_ecg_rest_opt_s;

    if(p_ext_flash_opt->erase_block)
    {
        BleThread_ExtFlashOperationSem();
        return 0;
    }
    return 1;
}

/****************************************************************
** Function name:           Extflash_EcgRestEraseBlockIsComplete
** Descriptions:            ECG结果数据操作函数
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              YX              
** Created Date:            2018-12-26
*****************************************************************/
uint8_t Extflash_EcgRestIsBusy(void)
{       
    ext_flash_ecg_rest_opt_t *p_ext_flash_opt = NULL;
    
    p_ext_flash_opt = &p_data_manage->ext_flash_ecg_rest_opt_s;
    
    if(p_ext_flash_opt->init || p_ext_flash_opt->write 
      || p_ext_flash_opt->read || p_ext_flash_opt->erase_block)
    {
        return 1;
    } 
    return 0;
}

static uint8_t Extflash_DailyWrite(uint8_t day_id, uint16_t block_count, uint8_t *data, uint16_t data_len, uint8_t data_type)
{
    if(!RECORD_DAILY_CONDITION_VALID(day_id, block_count) && data_type == 0)
    {
       return 0;
    }
    else if(data_type == 1 && !((day_id < 3)&&(block_count == RECORD_DAILY_DATA_MAX_CNT)) || data_type > 1)
    {
       return 0;
    }
    
    return Extflash_DataWrite(RECORD_DAILY_DATA_ADDR(day_id, block_count), data_len, data);
}

static uint8_t Extflash_DailyRead(uint8_t day_id, uint16_t block_count, uint8_t *data, uint16_t data_len, uint8_t data_type)
{
    if(data_type == 0 && !RECORD_DAILY_CONDITION_VALID(day_id, block_count))
    {
       return 0;
    }
    else if(data_type == 1 && !((day_id < 3)&&(block_count == RECORD_DAILY_DATA_MAX_CNT)) || data_type > 1)
    {
       return 0;
    }
    return Extflash_DataRead(RECORD_DAILY_DATA_ADDR(day_id, block_count), data_len, data);
}

static uint8_t Extflash_DailyEraseBlock(uint8_t block_id)
{
    uint16_t start_sector = 0;
    uint16_t block_num = 0;
    
    block_id = block_id % 3;
    start_sector = RECORD_DAILY_DATA_ADDR(block_id, 0)/4096;
    block_num = RECORD_DAILY_ONE_DAY_SIZE/4096;
    for(uint8_t i = 0; i < block_num; i++)
    {
        Extflash_DataSectorErase4K(start_sector + i);     
        // WJ.20190216.时间过长
        App_Watch_Dog_Reload();
    }
    
    return 1;
}

static uint8_t Extflash_DailyEraseAll(void )
{
    uint16_t start_sector = 0;
    uint16_t block_num = 0;
    
    start_sector = RECORD_DAILY_BASE_ADDR/4096;
    block_num = 24;//睡眠数据一起擦除了
    for(uint8_t i = 0; i < block_num; i++)
    {
        Extflash_DataSectorErase4K(start_sector + i);      
        // WJ.20190216.时间过长
        App_Watch_Dog_Reload();
    }
    
    return 1;
}

void Extflash_DailyDeal(void)
{
    ext_flash_daily_opt_t *p_ext_flash_opt = NULL;
    
    p_ext_flash_opt = &p_data_manage->ext_flash_daily_opt_s;
 
    if(p_ext_flash_opt->init)
    {
        memset(&p_data_manage->ext_flash_daily_opt_s, 0, sizeof(p_data_manage->ext_flash_daily_opt_s));
        return;
    }
    
    if(p_ext_flash_opt->write)
    {
        Extflash_DailyWrite(p_ext_flash_opt->write_day_id, p_ext_flash_opt->write_block,
                            p_ext_flash_opt->p_write, p_ext_flash_opt->write_len, p_ext_flash_opt->write_data_type);
        p_ext_flash_opt->write = 0;
        return;
    }
    
    
    if(p_ext_flash_opt->read)
    {
        Extflash_DailyRead(p_ext_flash_opt->read_day_id, p_ext_flash_opt->read_block, 
                                   p_ext_flash_opt->p_read, p_ext_flash_opt->read_len, p_ext_flash_opt->read_data_type);  
        p_ext_flash_opt->read = 0;
        return;
    }
    
    if(p_ext_flash_opt->erase_block)
    {
        Extflash_DailyEraseBlock(p_ext_flash_opt->erase_block_id);  
        p_ext_flash_opt->erase_block = 0;
        return;
    }
    
    if(p_ext_flash_opt->erase_all)
    {
        Extflash_DailyEraseAll();
        p_ext_flash_opt->erase_all = 0;
        return;
    }
}

uint8_t Extflash_DailyWriteSet(uint8_t day_id, uint16_t block_count, uint8_t *data, uint16_t data_len, uint8_t data_type)
{   
    ext_flash_daily_opt_t *p_ext_flash_opt = NULL;
    
    p_ext_flash_opt = &p_data_manage->ext_flash_daily_opt_s;

    if(p_ext_flash_opt->write)
    {
        //唤醒线程，执行写操作
        BleThread_ExtFlashOperationSem();
        return 0;
    }
    else
    {
        p_ext_flash_opt->p_write = data;
        p_ext_flash_opt->write_day_id = day_id;
        p_ext_flash_opt->write_block = block_count;
        p_ext_flash_opt->write_len = data_len;
        
        p_ext_flash_opt->write = 1;
        
        p_ext_flash_opt->write_data_type = data_type;
        
        BleThread_ExtFlashOperationSem();
    }
    return 1;
}

uint8_t Extflash_DailyWriteIsComplete(void)
{    
    ext_flash_daily_opt_t *p_ext_flash_opt = NULL;
    
    p_ext_flash_opt = &p_data_manage->ext_flash_daily_opt_s;

    if(p_ext_flash_opt->write)
    {
        BleThread_ExtFlashOperationSem();
        return 0;
    }
    return 1;
}

uint8_t Extflash_DailyReadSet(uint8_t day_id, uint16_t block_count, uint8_t *data, uint16_t data_len, uint8_t data_type)
{   
    ext_flash_daily_opt_t *p_ext_flash_opt = NULL;
    
    p_ext_flash_opt = &p_data_manage->ext_flash_daily_opt_s;

    if(p_ext_flash_opt->read)
    {
        //唤醒线程，执行写操作
        BleThread_ExtFlashOperationSem();
        return 0;
    }
    else
    {
        p_ext_flash_opt->p_read = data;
        p_ext_flash_opt->read_day_id = day_id;
        p_ext_flash_opt->read_block = block_count;
        p_ext_flash_opt->read_len = data_len;
        
        p_ext_flash_opt->read = 1;
        
        p_ext_flash_opt->read_data_type = data_type;
        
        BleThread_ExtFlashOperationSem();
    }
    return 1;
}

uint8_t Extflash_DailyReadIsComplete(void)
{    
    ext_flash_daily_opt_t *p_ext_flash_opt = NULL;
    
    p_ext_flash_opt = &p_data_manage->ext_flash_daily_opt_s;

    if(p_ext_flash_opt->read)
    {
        BleThread_ExtFlashOperationSem();
        return 0;
    }
    return 1;
}

uint8_t Extflash_DailyEraseBlockSet(uint8_t block_id)
{ 
    ext_flash_daily_opt_t *p_ext_flash_opt = NULL;
    
    p_ext_flash_opt = &p_data_manage->ext_flash_daily_opt_s;

    if(p_ext_flash_opt->erase_block)
    {
        //唤醒线程，执行写操作
        BleThread_ExtFlashOperationSem();
        return 0;
    }
    else
    {
        p_ext_flash_opt->erase_block_id = block_id;
        p_ext_flash_opt->erase_block = 1;
        BleThread_ExtFlashOperationSem();
    }
    return 1;
}


uint8_t Extflash_DailyEraseBlockIsComplete(void)
{
    ext_flash_daily_opt_t *p_ext_flash_opt = NULL;
    
    p_ext_flash_opt = &p_data_manage->ext_flash_daily_opt_s;

    if(p_ext_flash_opt->erase_block)
    {
        BleThread_ExtFlashOperationSem();
        return 0;
    }
    return 1;
}


uint8_t Extflash_DailyEraseAllSet(void)
{ 
    ext_flash_daily_opt_t *p_ext_flash_opt = NULL;
    
    p_ext_flash_opt = &p_data_manage->ext_flash_daily_opt_s;

    if(p_ext_flash_opt->erase_all)
    {
        //唤醒线程，执行写操作
        BleThread_ExtFlashOperationSem();
        return 0;
    }
    else
    {
        p_ext_flash_opt->erase_all = 1;
        BleThread_ExtFlashOperationSem();
    }
    return 1;
}


uint8_t Extflash_DailyEraseAllIsComplete(void)
{
    ext_flash_daily_opt_t *p_ext_flash_opt = NULL;
    
    p_ext_flash_opt = &p_data_manage->ext_flash_daily_opt_s;

    if(p_ext_flash_opt->erase_all)
    {
        BleThread_ExtFlashOperationSem();
        return 0;
    }
    return 1;
}

uint8_t Extflash_DailyIsBusy(void)
{       
    ext_flash_daily_opt_t *p_ext_flash_opt = NULL;
    
    p_ext_flash_opt = &p_data_manage->ext_flash_daily_opt_s;
    
    if(p_ext_flash_opt->init || p_ext_flash_opt->write 
      || p_ext_flash_opt->read || p_ext_flash_opt->erase_block
      || p_ext_flash_opt->erase_all)
    {
        return 1;
    } 
    return 0;
}


//SLEEP
/****************************************************************
** Function name:           Extflash_SleepWrite
** Descriptions:            写睡眠睡觉
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              YX              
** Created Date:            2018-12-26
*****************************************************************/
static uint8_t Extflash_SleepWrite(uint32_t offset, uint8_t *data, uint16_t data_len)
{
    return Extflash_DataWrite(offset, data_len, data);
}

/****************************************************************
** Function name:           Extflash_SleepRead
** Descriptions:            读取睡眠睡觉
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              YX              
** Created Date:            2018-12-26
*****************************************************************/
static uint8_t Extflash_SleepRead(uint32_t offset, uint8_t *data, uint16_t data_len)
{
    return Extflash_DataRead(offset, data_len, data);
}

/****************************************************************
** Function name:           Extflash_SleepRead
** Descriptions:            睡眠结果处理函数
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              YX              
** Created Date:            2018-12-26
*****************************************************************/
void Extflash_SleepDeal(void)
{
    ext_flash_sleep_opt_t *p_ext_flash_opt = NULL;
    
    p_ext_flash_opt = &p_data_manage->ext_flash_sleep_opt_s;
 
    if(p_ext_flash_opt->init)
    {
        memset(&p_data_manage->ext_flash_sleep_opt_s, 0, sizeof(p_data_manage->ext_flash_sleep_opt_s));
        return;
    }
    
    if(p_ext_flash_opt->write)
    {
        Extflash_SleepWrite(p_ext_flash_opt->write_offset,
                            p_ext_flash_opt->p_write, p_ext_flash_opt->write_len);
        p_ext_flash_opt->write = 0;
        return;
    }
    
    
    if(p_ext_flash_opt->read)
    {
        Extflash_SleepRead(p_ext_flash_opt->read_offset,
                                   p_ext_flash_opt->p_read, p_ext_flash_opt->read_len);  
        p_ext_flash_opt->read = 0;
        return;
    }
}

/****************************************************************
** Function name:           Extflash_SleepWriteSet
** Descriptions:            设置写睡眠结果的参数
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              YX              
** Created Date:            2018-12-26
*****************************************************************/
uint8_t Extflash_SleepWriteSet(uint32_t offset, uint8_t *data, uint16_t data_len)
{   
    ext_flash_sleep_opt_t *p_ext_flash_opt = NULL;
    
    p_ext_flash_opt = &p_data_manage->ext_flash_sleep_opt_s;

    if(p_ext_flash_opt->write)
    {
        //唤醒线程，执行写操作
        BleThread_ExtFlashOperationSem();
        return 0;
    }
    else
    {
        p_ext_flash_opt->p_write = data;
        p_ext_flash_opt->write_offset = offset;
        p_ext_flash_opt->write_len = data_len;
        
        p_ext_flash_opt->write = 1;
        BleThread_ExtFlashOperationSem();
    }
    return 1;
}

/****************************************************************
** Function name:           Extflash_SleepWriteIsComplete
** Descriptions:            写睡眠结果完成
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              YX              
** Created Date:            2018-12-26
*****************************************************************/
uint8_t Extflash_SleepWriteIsComplete(void)
{    
    ext_flash_sleep_opt_t *p_ext_flash_opt = NULL;
    
    p_ext_flash_opt = &p_data_manage->ext_flash_sleep_opt_s;

    if(p_ext_flash_opt->write)
    {
        BleThread_ExtFlashOperationSem();
        return 0;
    }
    return 1;
}

/****************************************************************
** Function name:           Extflash_SleepReadSet
** Descriptions:            设置读取睡眠结果的参数
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              YX              
** Created Date:            2018-12-26
*****************************************************************/
uint8_t Extflash_SleepReadSet(uint32_t offset, uint8_t *data, uint16_t data_len)
{   
    ext_flash_sleep_opt_t *p_ext_flash_opt = NULL;
    
    p_ext_flash_opt = &p_data_manage->ext_flash_sleep_opt_s;

    if(p_ext_flash_opt->read)
    {
        //唤醒线程，执行写操作
        BleThread_ExtFlashOperationSem();
        return 0;
    }
    else
    {
        p_ext_flash_opt->p_read = data;
        p_ext_flash_opt->read_offset = offset;
        p_ext_flash_opt->read_len = data_len;
        
        p_ext_flash_opt->read = 1;
        BleThread_ExtFlashOperationSem();
    }
    return 1;
}

/****************************************************************
** Function name:           Extflash_SleepReadIsComplete
** Descriptions:            读取睡眠结果完成
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              YX              
** Created Date:            2018-12-26
*****************************************************************/
uint8_t Extflash_SleepReadIsComplete(void)
{    
    ext_flash_sleep_opt_t *p_ext_flash_opt = NULL;
    
    p_ext_flash_opt = &p_data_manage->ext_flash_sleep_opt_s;

    if(p_ext_flash_opt->read)
    {
        BleThread_ExtFlashOperationSem();
        return 0;
    }
    return 1;
}

/****************************************************************
** Function name:           Extflash_SleepIsBusy
** Descriptions:            睡眠结果操作正忙
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              YX              
** Created Date:            2018-12-26
*****************************************************************/
uint8_t Extflash_SleepIsBusy(void)
{       
    ext_flash_sleep_opt_t *p_ext_flash_opt = NULL;
    
    p_ext_flash_opt = &p_data_manage->ext_flash_sleep_opt_s;
    
    if(p_ext_flash_opt->init || p_ext_flash_opt->write 
      || p_ext_flash_opt->read)
    {
        return 1;
    } 
    return 0;
}

/****************************************************************
** Function name:           Extflash_HeadWrite
** Descriptions:            写外部FLASH头部信息
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              YX              
** Created Date:            2018-12-26
*****************************************************************/
static uint8_t Extflash_HeadWrite(uint8_t *data, uint16_t data_len)
{
    return Extflash_DataWrite(EXT_FLASH_HEAD_BASE_ADDR, data_len, data);
}

/****************************************************************
** Function name:           Extflash_HeadRead
** Descriptions:            读取外部FLASH头部信息
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              YX              
** Created Date:            2018-12-26
*****************************************************************/
static uint8_t Extflash_HeadRead(uint8_t *data, uint16_t data_len)
{
    return Extflash_DataRead(EXT_FLASH_HEAD_BASE_ADDR, data_len, data);
}

/****************************************************************
** Function name:           Extflash_HeadErase
** Descriptions:            擦除外部FLASH头部信息
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              YX              
** Created Date:            2018-12-26
*****************************************************************/
static uint8_t Extflash_HeadErase(void )
{
    uint16_t start_sector = 0;
    
    start_sector = EXT_FLASH_HEAD_BASE_ADDR/4096;
    Extflash_DataSectorErase4K(start_sector);
    
    return 1;
}

/****************************************************************
** Function name:           Extflash_HeadDeal
** Descriptions:            外部FLASH头部信息处理函数
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              YX              
** Created Date:            2018-12-26
*****************************************************************/
void Extflash_HeadDeal(void)
{
    ext_flash_head_opt_t *p_ext_flash_opt = NULL;
    uint8_t head_data[20] = {0};
//    uint8_t head_str[8] = {0};
//    const uint8_t const_head_str[8] = {0x00,0x56,0x45,0x45,0x50,0x4f,0x4f,0x00};
    uint16_t head_len = 0;
    uint16_t proj_type = 0;
    uint8_t version_high = 0;
    uint8_t version_low = 0;
    uint8_t err_code = 1;
    
    p_ext_flash_opt = &p_data_manage->ext_flash_head_opt_s;
 
    if(p_ext_flash_opt->init)
    {
        memset(&p_data_manage->ext_flash_head_opt_s, 0, sizeof(p_data_manage->ext_flash_head_opt_s));
        
        err_code = Extflash_HeadRead(head_data, FLASH_HEAD_LEN);
        if(!err_code)
        {
            return;
        }
//        memcpy(head_str, head_data, 8);
//        if(strcmp(head_str, const_head_str) != 0)
        if(   head_data[0] != 0x00 || head_data[1] != 0x56
           || head_data[2] != 0x45 || head_data[3] != 0x45
           || head_data[4] != 0x50 || head_data[5] != 0x4f
           || head_data[6] != 0x4f || head_data[7] != 0x00)
        {
            //有误
            err_code = 0;
        }
        else
        {
            head_len = *((uint16_t *)&head_data[8]);
            proj_type = *((uint16_t *)&head_data[10]);
            version_high = head_data[12];
            version_low = head_data[13];
            if(   proj_type != FLASH_PROJ_TYPE 
               || head_len != FLASH_HEAD_LEN
               || version_high != FLASH_VERSION_H
               || version_low != FLASH_VERSION_L)
            {
                //有误
                err_code = 0;
            }
        }
        //头部信息由问题
        if(!err_code)
        {
            memset(head_data, 0, sizeof(head_data));
//            memcpy(head_data, const_head_str, 8);
            head_data[0] = 0x00;
            head_data[1] = 0x56;
            head_data[2] = 0x45;
            head_data[3] = 0x45;
            head_data[4] = 0x50;
            head_data[5] = 0x4f;
            head_data[6] = 0x4f;
            head_data[7] = 0x00;
            head_data[8] = FLASH_HEAD_LEN&0x00ff;
            head_data[9] = (FLASH_HEAD_LEN&0xff00)>>8;
            head_data[10] = FLASH_PROJ_TYPE&0x00ff;
            head_data[11] = (FLASH_PROJ_TYPE&0xff00)>>8;
            head_data[12] = FLASH_VERSION_H;
            head_data[13] = FLASH_VERSION_L;
//            Extflash_HeadErase();
            Extflash_AllOptEraseAllOpt();
            while(Extflash_DataIsBusy())
            {
                vTaskDelay(300);
                App_Watch_Dog_Reload(); //1秒喂狗1次
            }
            Extflash_HeadWrite(head_data, FLASH_HEAD_LEN);
        }
        p_ext_flash_opt->init = 0;
        return;
    }
    
    if(p_ext_flash_opt->write)
    {
        Extflash_HeadWrite(p_ext_flash_opt->p_write, p_ext_flash_opt->write_len);
        p_ext_flash_opt->write = 0;
        return;
    }
    
    
    if(p_ext_flash_opt->read)
    {
        Extflash_HeadRead(p_ext_flash_opt->p_read, p_ext_flash_opt->read_len);  
        p_ext_flash_opt->read = 0;
        return;
    }
    if(p_ext_flash_opt->erase)
    {
        Extflash_HeadErase();
        p_ext_flash_opt->erase = 0;
        return;
    }
}

/****************************************************************
** Function name:           Extflash_HeadWriteSet
** Descriptions:            设置写外部FLASH头部信息的参数
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              YX              
** Created Date:            2018-12-26
*****************************************************************/
uint8_t Extflash_HeadWriteSet(uint8_t *data, uint16_t data_len)
{   
    ext_flash_head_opt_t *p_ext_flash_opt = NULL;
    
    p_ext_flash_opt = &p_data_manage->ext_flash_head_opt_s;

    if(p_ext_flash_opt->write)
    {
        //唤醒线程，执行写操作
        BleThread_ExtFlashOperationSem();
        return 0;
    }
    else
    {
        p_ext_flash_opt->p_write = data;
        p_ext_flash_opt->write_len = data_len;
        
        p_ext_flash_opt->write = 1;
        BleThread_ExtFlashOperationSem();
    }
    return 1;
}

/****************************************************************
** Function name:           Extflash_HeadWriteIsComplete
** Descriptions:            写外部FLASH头部信息完成
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              YX              
** Created Date:            2018-12-26
*****************************************************************/
uint8_t Extflash_HeadWriteIsComplete(void)
{    
    ext_flash_head_opt_t *p_ext_flash_opt = NULL;
    
    p_ext_flash_opt = &p_data_manage->ext_flash_head_opt_s;

    if(p_ext_flash_opt->write)
    {
        BleThread_ExtFlashOperationSem();
        return 0;
    }
    return 1;
}

/****************************************************************
** Function name:           Extflash_HeadReadSet
** Descriptions:            设置读取外部FLASH头部信息的参数
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              YX              
** Created Date:            2018-12-26
*****************************************************************/
uint8_t Extflash_HeadReadSet(uint8_t *data, uint16_t data_len)
{   
    ext_flash_head_opt_t *p_ext_flash_opt = NULL;
    
    p_ext_flash_opt = &p_data_manage->ext_flash_head_opt_s;

    if(p_ext_flash_opt->read)
    {
        //唤醒线程，执行写操作
        BleThread_ExtFlashOperationSem();
        return 0;
    }
    else
    {
        p_ext_flash_opt->p_read = data;
        p_ext_flash_opt->read_len = data_len;
        
        p_ext_flash_opt->read = 1;
        BleThread_ExtFlashOperationSem();
    }
    return 1;
}

/****************************************************************
** Function name:           Extflash_HeadReadIsComplete
** Descriptions:            读取外部FLASH头部信息完成
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              YX              
** Created Date:            2018-12-26
*****************************************************************/
uint8_t Extflash_HeadReadIsComplete(void)
{    
    ext_flash_head_opt_t *p_ext_flash_opt = NULL;
    
    p_ext_flash_opt = &p_data_manage->ext_flash_head_opt_s;

    if(p_ext_flash_opt->read)
    {
        BleThread_ExtFlashOperationSem();
        return 0;
    }
    return 1;
}

/****************************************************************
** Function name:           Extflash_HeadEraseSet
** Descriptions:            设置擦除外部FLASH头部信息
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              YX              
** Created Date:            2018-12-26
*****************************************************************/
uint8_t Extflash_HeadEraseSet(void)
{ 
    ext_flash_head_opt_t *p_ext_flash_opt = NULL;
    
    p_ext_flash_opt = &p_data_manage->ext_flash_head_opt_s;

    if(p_ext_flash_opt->erase)
    {
        //唤醒线程，执行写操作
        BleThread_ExtFlashOperationSem();
        return 0;
    }
    else
    {
        p_ext_flash_opt->erase = 1;
        BleThread_ExtFlashOperationSem();
    }
    return 1;
}

/****************************************************************
** Function name:           Extflash_HeadEraseIsComplete
** Descriptions:            擦除外部FLASH头部信息完成
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              YX              
** Created Date:            2018-12-26
*****************************************************************/
uint8_t Extflash_HeadEraseIsComplete(void)
{
    ext_flash_head_opt_t *p_ext_flash_opt = NULL;
    
    p_ext_flash_opt = &p_data_manage->ext_flash_head_opt_s;

    if(p_ext_flash_opt->erase)
    {
        BleThread_ExtFlashOperationSem();
        return 0;
    }
    return 1;
}

/****************************************************************
** Function name:           Extflash_HeadIsBusy
** Descriptions:            外部FLASH头部信息操作正忙
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              YX              
** Created Date:            2018-12-26
*****************************************************************/
uint8_t Extflash_HeadIsBusy(void)
{       
    ext_flash_head_opt_t *p_ext_flash_opt = NULL;
    
    p_ext_flash_opt = &p_data_manage->ext_flash_head_opt_s;
    
    if(p_ext_flash_opt->init || p_ext_flash_opt->write 
      || p_ext_flash_opt->read || p_ext_flash_opt->erase)
    {
        return 1;
    } 
    return 0;
}

/****************************************************************
** Function name:           Extflash_AllOptWrite
** Descriptions:            外部FLASH写数据
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              YX              
** Created Date:            2018-12-26
*****************************************************************/
static uint8_t Extflash_AllOptWrite(uint32_t offset, uint8_t *data, uint16_t data_len)
{
    return Extflash_DataWrite(offset, data_len, data);
}

/****************************************************************
** Function name:           Extflash_AllOptWrite
** Descriptions:            外部FLASH读取数据
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              YX              
** Created Date:            2018-12-26
*****************************************************************/
static uint8_t Extflash_AllOptRead(uint32_t offset, uint8_t *data, uint16_t data_len)
{
    return Extflash_DataRead(offset, data_len, data);
}

/****************************************************************
** Function name:           Extflash_AllOptEraseSize
** Descriptions:            擦除外部FLASH的部分数据
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              YX              
** Created Date:            2018-12-26
*****************************************************************/
static uint8_t Extflash_AllOptEraseSize(uint32_t offset, uint8_t size)
{
    uint16_t start_sector = 0;
    uint16_t block_num = 0;
    //2M Flash
    if(offset%4096 != 0 || size%4096 != 0
       || (offset+size) > 2*1024*1024)
    {
        return 0;
    }
    
    start_sector = offset/4096;
    block_num = size/4096;
    for(uint8_t i = 0; i < block_num; i++)
    {
        Extflash_DataSectorErase4K(start_sector + i);      
        // WJ.20190216.时间过长
        App_Watch_Dog_Reload();
    }
    
    return 1;
}

/****************************************************************
** Function name:           Extflash_AllOptEraseAllOpt
** Descriptions:            擦除外部FLASH的所有数据
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              YX              
** Created Date:            2018-12-26
*****************************************************************/
static uint8_t Extflash_AllOptEraseAllOpt(void)
{
    return Extflash_DataAllErase();
}

/****************************************************************
** Function name:           Extflash_AllOptDeal
** Descriptions:            外部FLASH处理函数
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              YX              
** Created Date:            2018-12-26
*****************************************************************/
void Extflash_AllOptDeal(void)
{
    ext_flash_all_opt_t *p_ext_flash_opt = NULL;
    
    p_ext_flash_opt = &p_data_manage->ext_flash_all_opt_s;
 
    if(p_ext_flash_opt->init)
    {
        memset(&p_data_manage->ext_flash_all_opt_s, 0, sizeof(p_data_manage->ext_flash_all_opt_s));
        return;
    }
    
    if(p_ext_flash_opt->write)
    {
        Extflash_AllOptWrite(p_ext_flash_opt->write_offset, 
                                   p_ext_flash_opt->p_write, p_ext_flash_opt->write_len);
        p_ext_flash_opt->write = 0;
        return;
    }
    
    
    if(p_ext_flash_opt->read)
    {
        Extflash_AllOptRead(p_ext_flash_opt->read_offset, 
                                   p_ext_flash_opt->p_read, p_ext_flash_opt->read_len);  
        p_ext_flash_opt->read = 0;
        return;
    }
    
    if(p_ext_flash_opt->erase)
    {
        Extflash_AllOptEraseSize(p_ext_flash_opt->erase_offset, p_ext_flash_opt->erase_size);  
        p_ext_flash_opt->erase = 0;
        return;
    }
    
    if(p_ext_flash_opt->erase_all)
    {
        Extflash_AllOptEraseAllOpt();  
        p_ext_flash_opt->erase_all = 0;
        return;
    }
}

/****************************************************************
** Function name:           Extflash_AllOptWriteSet
** Descriptions:            设置外部FLASH写参数
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              YX              
** Created Date:            2018-12-26
*****************************************************************/
uint8_t Extflash_AllOptWriteSet(uint32_t write_offset, uint8_t *data, uint16_t data_len)
{   
    ext_flash_all_opt_t *p_ext_flash_opt = NULL;
    
    p_ext_flash_opt = &p_data_manage->ext_flash_all_opt_s;

    if(p_ext_flash_opt->write)
    {
        //唤醒线程，执行写操作
        BleThread_ExtFlashOperationSem();
        return 0;
    }
    else
    {
        p_ext_flash_opt->p_write = data;
        p_ext_flash_opt->write_offset = write_offset;
        p_ext_flash_opt->write_len = data_len;
        
        p_ext_flash_opt->write = 1;
        BleThread_ExtFlashOperationSem();
    }
    return 1;
}

/****************************************************************
** Function name:           Extflash_AllOptWriteSet
** Descriptions:            外部FLASH写完成
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              YX              
** Created Date:            2018-12-26
*****************************************************************/
uint8_t Extflash_AllOptWriteIsComplete(void)
{    
    ext_flash_all_opt_t *p_ext_flash_opt = NULL;
    
    p_ext_flash_opt = &p_data_manage->ext_flash_all_opt_s;

    if(p_ext_flash_opt->write)
    {
        BleThread_ExtFlashOperationSem();
        return 0;
    }
    return 1;
}

/****************************************************************
** Function name:           Extflash_AllOptReadSet
** Descriptions:            设置外部FLASH读参数
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              YX              
** Created Date:            2018-12-26
*****************************************************************/
uint8_t Extflash_AllOptReadSet(uint32_t read_offset, uint8_t *read_buf, uint16_t read_len)
{   
    ext_flash_all_opt_t *p_ext_flash_opt = NULL;
    
    p_ext_flash_opt = &p_data_manage->ext_flash_all_opt_s;

    if(p_ext_flash_opt->read)
    {
        //唤醒线程，执行写操作
        BleThread_ExtFlashOperationSem();
        return 0;
    }
    else
    {
        p_ext_flash_opt->p_read = read_buf;
        p_ext_flash_opt->read_offset = read_offset;
        p_ext_flash_opt->read_len = read_len;
        
        p_ext_flash_opt->read = 1;
        BleThread_ExtFlashOperationSem();
    }
    return 1;
}

/****************************************************************
** Function name:           Extflash_AllOptReadIsComplete
** Descriptions:            外部FLASH读完成
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              YX              
** Created Date:            2018-12-26
*****************************************************************/
uint8_t Extflash_AllOptReadIsComplete(void)
{    
    ext_flash_all_opt_t *p_ext_flash_opt = NULL;
    
    p_ext_flash_opt = &p_data_manage->ext_flash_all_opt_s;

    if(p_ext_flash_opt->read)
    {
        BleThread_ExtFlashOperationSem();
        return 0;
    }
    return 1;
}

/****************************************************************
** Function name:           Extflash_AllOptEraseSizeSet
** Descriptions:            设置外部FLASH擦除的参数
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              YX              
** Created Date:            2018-12-26
*****************************************************************/
uint8_t Extflash_AllOptEraseSizeSet(uint32_t erase_offset, uint32_t erase_size)
{ 
    ext_flash_all_opt_t *p_ext_flash_opt = NULL;
    
    p_ext_flash_opt = &p_data_manage->ext_flash_all_opt_s;

    if(p_ext_flash_opt->erase)
    {
        //唤醒线程，执行写操作
        BleThread_ExtFlashOperationSem();
        return 0;
    }
    else
    {
        p_ext_flash_opt->erase_offset = erase_offset;
        p_ext_flash_opt->erase_size = erase_size;
        p_ext_flash_opt->erase = 1;
        BleThread_ExtFlashOperationSem();
    }
    return 1;
}

/****************************************************************
** Function name:           Extflash_AllOptEraseBlockIsComplete
** Descriptions:            外部FLASH擦除完成
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              YX              
** Created Date:            2018-12-26
*****************************************************************/
uint8_t Extflash_AllOptEraseBlockIsComplete(void)
{
    ext_flash_all_opt_t *p_ext_flash_opt = NULL;
    
    p_ext_flash_opt = &p_data_manage->ext_flash_all_opt_s;

    if(p_ext_flash_opt->erase)
    {
        BleThread_ExtFlashOperationSem();
        return 0;
    }
    return 1;
}

/****************************************************************
** Function name:           Extflash_AllOptEraseAllSet
** Descriptions:            设置外部FLASH擦除所有数据
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              YX              
** Created Date:            2018-12-26
*****************************************************************/
uint8_t Extflash_AllOptEraseAllSet(void)
{ 
    ext_flash_all_opt_t *p_ext_flash_opt = NULL;
    
    p_ext_flash_opt = &p_data_manage->ext_flash_all_opt_s;

    if(p_ext_flash_opt->erase_all)
    {
        //唤醒线程，执行写操作
        BleThread_ExtFlashOperationSem();
        return 0;
    }
    else
    {
        p_ext_flash_opt->erase_all = 1;
        BleThread_ExtFlashOperationSem();
    }
    return 1;
}

/****************************************************************
** Function name:           Extflash_AllOptEraseAllIsComplete
** Descriptions:            外部FLASH擦除所有数据完成
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              YX              
** Created Date:            2018-12-26
*****************************************************************/
uint8_t Extflash_AllOptEraseAllIsComplete(void)
{
    ext_flash_all_opt_t *p_ext_flash_opt = NULL;
    
    p_ext_flash_opt = &p_data_manage->ext_flash_all_opt_s;

    if(p_ext_flash_opt->erase_all)
    {
        BleThread_ExtFlashOperationSem();
        return 0;
    }
    return 1;
}

/****************************************************************
** Function name:           Extflash_AllOptIsBusy
** Descriptions:            外部FLASH操作正忙
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              YX              
** Created Date:            2018-12-26
*****************************************************************/
uint8_t Extflash_AllOptIsBusy(void)
{       
    ext_flash_all_opt_t *p_ext_flash_opt = NULL;
    
    p_ext_flash_opt = &p_data_manage->ext_flash_all_opt_s;
    
    if(p_ext_flash_opt->init || p_ext_flash_opt->write 
      || p_ext_flash_opt->read || p_ext_flash_opt->erase
      || p_ext_flash_opt->erase_all)
    {
        return 1;
    } 
    return 0;
}

