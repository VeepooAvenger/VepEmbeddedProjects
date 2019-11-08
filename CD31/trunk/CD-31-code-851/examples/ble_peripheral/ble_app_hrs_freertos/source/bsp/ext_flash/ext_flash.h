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
#include "board.h"

#define EXT_FLASH_MAN_ID            0x5E// 0xc8//0xEF=winbond  0x1c=feon
#define EXT_FLASH_DEV_ID            0x13//0x17=winbond128M  0x16=feon or winbond64M
 
#define EXT_FLASH_4K_NUM            0X1000

#ifdef __cplusplus
extern "C"
{
#endif    

#define EXT_FLASH_CS_LOW        PIN_LOW(FLASH_CS_PIN);  
#define EXT_FLASH_CS_HIGH       PIN_HIGH(FLASH_CS_PIN);  
    
/**
* Initialize storage driver.
*
* @return True when successful.
*/
uint8_t Extflash_Open(void);

/**
* Close the storage driver
*/
void Extflash_Close(void);

/**
* Read storage content
*
* @return True when successful.
*/
uint8_t Extflash_Read(uint32_t offset, uint32_t length, uint8_t *buf);

/**
* Erase storage sectors corresponding to the range.
*
* @return True when successful.
*/
uint8_t Extflash_Erase(uint32_t offset, uint32_t length);

//Ð´Êý¾Ý
uint8_t Extflash_Write(uint32_t offset, uint32_t length,  uint8_t *buf);

//¶ÁÊý¾Ý
uint8_t Extflash_Read(uint32_t offset, uint32_t length, uint8_t *buf);

//°´¿é²Á³ý(16K)
uint8_t Extflash_BlockErase(uint8_t block);

//°´Çø²Á³ý£¨64K£©
uint8_t Extflash_SectorErase(uint16_t sector);

//²Á³ýÐ¾Æ¬
uint8_t Extflash_AllErase(void);

#ifdef __cplusplus
}
#endif

#endif /* EXT_FLASH_H */
