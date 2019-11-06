/***************************Copyright(c)***********************************
**                       Veepoo Tech
**                  COPYRIGHT (C) 2016-2019
**-----------File Info-----------------------------------------------------
** File        name:     ext_flash.h
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
#ifndef EXT_FLASH_H
#define EXT_FLASH_H

#include <stdlib.h>
#include <stdbool.h>

#define EXT_FLASH_MAN_ID            0xC8    // 0xc8//0xEF=winbond  0x1c=feon
#define EXT_FLASH_DEV_ID            0x16    //0x17=winbond128M  0x16=feon or winbond64M

#define EXT_FLASH_DATA_MAN_ID       0x0B    // 0xc8//0xEF=winbond  0x1c=feon
#define EXT_FLASH_DATA_DEV_ID       0x14    //0x17=winbond128M  0x16=feon or winbond64M

#define EXT_FLASH_4K_NUM            0x1000

#ifdef __cplusplus
extern "C"
{
#endif    
    
// 开启Flash
uint8_t Extflash_Open(void);

// 关闭Flash,进入深度休眠
void Extflash_Close(void);

// 读数据
uint8_t Extflash_Read(uint32_t offset, uint32_t length, uint8_t *buf);

// 写数据
uint8_t Extflash_Write(uint32_t offset, uint32_t length, uint8_t *buf);

// 按区擦除（4K）
uint8_t Extflash_SectorErase4K(uint16_t sector);

// 按块擦除(32K)
uint8_t Extflash_BlockErase32K(uint8_t block);

// 按块擦除(64K)
uint8_t Extflash_BlockErase64K(uint8_t block);

// 擦除芯片(写保护下不可用)
uint8_t Extflash_AllErase(void);

// 按照地址和长度擦除对应区域
uint8_t Extflash_Erase(uint32_t offset, uint32_t length);


// 开启Flash
uint8_t Extflash_DataOpen(void);

// 关闭Flash,进入深度休眠
void Extflash_DataClose(void);

// 读数据
uint8_t Extflash_DataRead(uint32_t offset, uint32_t length, uint8_t *buf);

// 写数据
uint8_t Extflash_DataWrite(uint32_t offset, uint32_t length, uint8_t *buf);

// 按区擦除（4K）
uint8_t Extflash_DataSectorErase4K(uint16_t sector);
//
uint8_t Extflash_DataBlockErase32K(uint8_t block);
//
uint8_t Extflash_DataBlockErase64K(uint8_t block);

// 擦除芯片(写保护下不可用)
uint8_t Extflash_DataAllErase(void);

// 按照地址和长度擦除对应区域
uint8_t Extflash_DataErase(uint32_t offset, uint32_t length);

//芯片状态
uint8_t Extflash_DataIsBusy(void);

#ifdef __cplusplus
}
#endif

#endif /* EXT_FLASH_H */
