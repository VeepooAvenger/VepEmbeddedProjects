/*******************Copyright(c)*********************************
** Veepoo Tech
** COPYRIGHT (C) 2016-2019
**----------------------FileInfo---------------------------------
** File name:                  fontlib_spi.c
** Last modified Date:         2016-11-14
** Last SYS_VERSION:           0.0.1
** Descriptions:
**---------------------------------------------------------------
** Createdby:                  WJ
** Createddate:                2016-11-14
** SYS_VERSION:                0.0.1
** Descriptions:               字库库
**---------------------------------------------------------------
** Modified by:				
** Modified date:  			          
** SYS_VERSION:                 
** Descriptions:                        
***************************************************************************/
#include <stdio.h>
#include "fontlib_spi.h"
#include "HFMA2Ylib.h"
#include "ext_flash.h"

/****************************************************************
** Function name:               r_dat_bat
** Descriptions:                读多个数据
** input parameters:            TAB_addr：        地址
                                Num：             数据个数
                                p_arr：           数据指针
** output parameters:           无
** Returned value:              无
** Created by:                  WJ    
** Created Date:                2016-11-14
**---------------------------------------------------------------
** Modified by:                  
** Modified date:              
** Modified Descriptions:
**---------------------------------------------------------------
****************************************************************/
void r_dat_bat(uint32_t TAB_addr,uint8_t Num,uint8_t *p_arr)
{
    static uint32_t addr = 0;
    
    addr = TAB_addr; //+ FLASH_FONT_ADDR; 

    Extflash_Read(addr, Num, p_arr);
}

/****************************************************************
** Function name:               Font_AsciiLattice
** Descriptions:                获取字型
** input parameters:            ascii：         字符
                                p_lattice：     字型
** output parameters:           无
** Returned value:              无
** Created by:                  WJ    
** Created Date:                2016-11-14
**---------------------------------------------------------------
** Modified by:                  
** Modified date:              
** Modified Descriptions:
**---------------------------------------------------------------
****************************************************************/
void Font_AsciiLattice(uint8_t ascii,uint8_t* p_lattice)
{
    ASCII_GetData(ascii,ASCII_8X16,p_lattice);
}

/****************************************************************
** Function name:               Font_ChineseLattice
** Descriptions:                中文字型
** input parameters:            p_data：        字符
                                p_lattice：     字型
** output parameters:           无
** Returned value:              是否成功
** Created by:                  WJ    
** Created Date:                2016-11-14
**---------------------------------------------------------------
** Modified by:                  
** Modified date:              
** Modified Descriptions:
**---------------------------------------------------------------
****************************************************************/
bool Font_ChineseLattice(uint8_t* p_data, uint8_t* p_lattice)
{    
    uint32_t gbk_code = 0;
	
    gbk_code=U2G(UTF8_TO_GBK(p_data));
	
    if(gbk_code  == 0x00)
    {
        return false;
    }
    
    hzbmp16(SEL_GB, gbk_code, 0, 16,p_lattice);
    
    return true;
}

/****************************************************************
** Function name:               Font_KoreanLattice
** Descriptions:                韩语字型
** input parameters:            p_data：        字符
                                p_lattice：     字型
** output parameters:           无
** Returned value:              无
** Created by:                  WJ    
** Created Date:                2016-11-14
**---------------------------------------------------------------
** Modified by:                  
** Modified date:              
** Modified Descriptions:
**---------------------------------------------------------------
****************************************************************/
void Font_KoreanLattice(uint8_t* p_data, uint8_t* p_lattice)
{
     uint32_t gbk_code = 0;
     gbk_code=U2K(UTF8_TO_GBK(p_data));
     hzbmp16(SEL_KSC, gbk_code, 0, 16,p_lattice);
}

/****************************************************************
** Function name:               Font_JapaneseLattice
** Descriptions:                日语字型
** input parameters:            p_data：        字符
                                p_lattice：     字型
** output parameters:           无
** Returned value:              无
** Created by:                  WJ    
** Created Date:                2016-11-14
**---------------------------------------------------------------
** Modified by:                  
** Modified date:              
** Modified Descriptions:
**---------------------------------------------------------------
****************************************************************/
void Font_JapaneseLattice(uint8_t* p_data, uint8_t* p_lattice)
{
    uint32_t gbk_code = 0;
        
    gbk_code=U2J(UTF8_TO_GBK(p_data));
    hzbmp16(SEL_JIS, gbk_code, 0, 16,p_lattice);
}

/****************************************************************
** Function name:               Font_CyrillicLattice
** Descriptions:                西里尔字型（俄语）
** input parameters:            p_data：        字符
                                p_lattice：     字型
** output parameters:           无
** Returned value:              无
** Created by:                  WJ    
** Created Date:                2016-11-14
**---------------------------------------------------------------
** Modified by:                  
** Modified date:              
** Modified Descriptions:
**---------------------------------------------------------------
****************************************************************/
void Font_CyrillicLattice(uint8_t* p_data, uint8_t* p_lattice)
{
     CYRILLIC_GetData(UTF8_TO_UNICODE(p_data),p_lattice);
}

/****************************************************************
** Function name:               Font_LattinLattice
** Descriptions:                拉丁字库
** input parameters:            p_data：        字符
                                p_lattice：     字型
** output parameters:           无
** Returned value:              无
** Created by:                  WJ    
** Created Date:                2016-11-14
**---------------------------------------------------------------
** Modified by:                  
** Modified date:              
** Modified Descriptions:
**---------------------------------------------------------------
****************************************************************/
void Font_LattinLattice(uint8_t* p_data, uint8_t* p_lattice)
{
    if((p_data[0] & 0xE0) == 0xE0)
    {
        //增加部分
        LATIN_GetData(UTF8_TO_GBK(p_data), p_lattice);
    }
    else
    {
        LATIN_GetData(UTF8_TO_UNICODE(p_data),p_lattice);
    }
}

