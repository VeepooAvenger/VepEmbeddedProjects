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
** Descriptions:               �ֿ��
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
** Descriptions:                ���������
** input parameters:            TAB_addr��        ��ַ
                                Num��             ���ݸ���
                                p_arr��           ����ָ��
** output parameters:           ��
** Returned value:              ��
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
** Descriptions:                ��ȡ����
** input parameters:            ascii��         �ַ�
                                p_lattice��     ����
** output parameters:           ��
** Returned value:              ��
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
** Descriptions:                ��������
** input parameters:            p_data��        �ַ�
                                p_lattice��     ����
** output parameters:           ��
** Returned value:              �Ƿ�ɹ�
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
** Descriptions:                ��������
** input parameters:            p_data��        �ַ�
                                p_lattice��     ����
** output parameters:           ��
** Returned value:              ��
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
** Descriptions:                ��������
** input parameters:            p_data��        �ַ�
                                p_lattice��     ����
** output parameters:           ��
** Returned value:              ��
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
** Descriptions:                ��������ͣ����
** input parameters:            p_data��        �ַ�
                                p_lattice��     ����
** output parameters:           ��
** Returned value:              ��
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
** Descriptions:                �����ֿ�
** input parameters:            p_data��        �ַ�
                                p_lattice��     ����
** output parameters:           ��
** Returned value:              ��
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
        //���Ӳ���
        LATIN_GetData(UTF8_TO_GBK(p_data), p_lattice);
    }
    else
    {
        LATIN_GetData(UTF8_TO_UNICODE(p_data),p_lattice);
    }
}

