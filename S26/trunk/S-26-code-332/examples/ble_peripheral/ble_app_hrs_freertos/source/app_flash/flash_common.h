/******************************************************************************
 * Copyright (c) 2017 - 2018, Veepoo
 * 
 * All rights reserved.
 * 
** 文件名称: flash_common.h
** 创建时间: 2016-12-05
** 创 建 人: GT
** 描    述: 文件主要提供Flash的基础接口,主要包含读、写、擦除
**
*******************************************************************************/

#ifndef _FLASH_COMMON_H_
#define _FLASH_COMMON_H_

#include <stdint.h>
#include <stdbool.h>
#include "fstorage.h"

#ifdef __cplusplus
extern "C" {
#endif

uint32_t Flash_CommonInit(void);

fs_ret_t Flash_CommonStore(uint32_t const * p_dest, uint32_t const * const p_src, uint32_t len_words);

fs_ret_t Flash_CommonErase(uint32_t const * p_dest, uint32_t num_pages);

fs_ret_t Flash_CommonRead(uint32_t const *p_data, uint32_t addr, uint32_t bytes);

fs_ret_t Flash_CommonUpdate(uint32_t const * p_dest, uint32_t const * const p_src, uint32_t len_words);

#ifdef __cplusplus
}
#endif

#endif 

