/***************************Copyright(c)***********************************
**                       Veepoo Tech
**                  COPYRIGHT (C) 2016-2019
**-----------File Info-----------------------------------------------------
** File        name:     ext_flash.c
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
#include "bsp_spi.h"
#include "ext_flash.h"

/* Instruction codes */
#define BLS_CODE_PROGRAM          0x02 /**< Page Program */
#define BLS_CODE_READ             0x03 /**< Read Data */
#define BLS_CODE_READ_STATUS      0x05 /**< Read Status Register */
#define BLS_CODE_WRITE_ENABLE     0x06 /**< Write Enable */
#define BLS_CODE_SECTOR_ERASE     0x20 /**< Sector Erase */
#define BLS_CODE_MDID             0x90 /**< Manufacturer Device ID */

#define BLS_CODE_DP               0xB9 /**< Power down */
#define BLS_CODE_RDP              0xAB /**< Power standby */

/* Erase instructions */
#define BLS_CODE_ERASE_4K         0x20 /**< Sector Erase */ //erase time 45ms--60ms max=400ms
#define BLS_CODE_ERASE_32K        0x52 //32K erase time 120ms  max=1600ms
#define BLS_CODE_ERASE_64K        0xD8 //64K erase time 150ms  max=2000ms
#define BLS_CODE_ERASE_ALL        0xC7 /**< Mass Erase *///64K erase time 20s  max=200s

/* Bitmasks of the status register */
#define BLS_STATUS_SRWD_BM        0x80
#define BLS_STATUS_BP_BM          0x0C
#define BLS_STATUS_WEL_BM         0x02
#define BLS_STATUS_WIP_BM         0x01

#define BLS_STATUS_BIT_BUSY       0x01 /**< Busy bit of the status register */

/* Part specific constants */
#define BLS_PROGRAM_PAGE_SIZE     256
#define BLS_ERASE_SECTOR_SIZE     4096
#define BLS_ERASE_BLOCK_SIZE      16*BLS_ERASE_SECTOR_SIZE

// Private functions
static int Extflash_WaitReady(void);
//static int Extflash_WaitPowerDown(void);



static uint8_t select_flash_type = 0;


void Extflash_SetCsSelect(uint8_t select_flash)
{
    select_flash_type = select_flash;
}
/****************************************************************
** Function name:           Extflash_Select
** Descriptions:            CS使能
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ             
** Created Date:            2017-08-21
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
static void Extflash_Select(void)
{
	EXT_FLASH_CS_LOW;   
}


/****************************************************************
** Function name:           Extflash_Select
** Descriptions:            CS失能
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ             
** Created Date:            2017-08-21
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
static void Extflash_Deselect(void)
{
	EXT_FLASH_CS_HIGH;
}

/****************************************************************
** Function name:           Extflash_SpiWrite
** Descriptions:            写数据接口
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ             
** Created Date:            2017-08-21
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
static uint8_t Extflash_SpiWrite(const uint8_t *data, uint8_t len)
{
    uint8_t ret = 0;
    
    ret = !Bps_Spi2Write(data, len);   
    return ret;    
}

/****************************************************************
** Function name:           Extflash_SpiRead
** Descriptions:            度数据接口
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ             
** Created Date:            2017-08-21
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
static uint8_t Extflash_SpiRead(uint8_t *data, uint8_t len)
{
    uint8_t ret = 0;
    
    ret = !Bps_Spi2Read(data, len);   
    return ret;
}

/****************************************************************
** Function name:           Extflash_PowerDown
** Descriptions:            休眠
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ             
** Created Date:            2017-08-21
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
static uint8_t Extflash_PowerDown(void)
{
	uint8_t cmd = 0;
	uint8_t success = 0;
	
	cmd = BLS_CODE_DP;
	Extflash_Select();
    
	success = Extflash_SpiWrite(&cmd, sizeof(cmd)) == 0;
    
	Extflash_Deselect();
	
	return success;
}

/****************************************************************
** Function name:           Extflash_PowerStandby
** Descriptions:            CS失能
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ             
** Created Date:            2017-08-21
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
static uint8_t Extflash_PowerStandby(void)
{
	uint8_t cmd = 0;
	uint8_t success = 0;
	
	cmd = BLS_CODE_RDP;
	Extflash_Select();
	success = Extflash_SpiWrite(&cmd, sizeof(cmd)) == 0;   
	Extflash_Deselect();
	
	if(success)
    {
		success = Extflash_WaitReady() == 0;
	}
	return success;
}

/****************************************************************
** Function name:           Extflash_VerifyPart
** Descriptions:            校验
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ             
** Created Date:            2017-08-21
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
	static uint8_t rbuf[2]={0};
static uint8_t Extflash_VerifyPart(void)
{
    //verify the manufacturer and device ID   (90 dummy dummy  00)
	const uint8_t wbuf[] = {BLS_CODE_MDID, 0x00, 0x00, 0x00 };  
	//static uint8_t rbuf[2]={0};		
    uint8_t ret = 0;
	
	Extflash_Select();
	
	ret = Extflash_SpiWrite(wbuf, sizeof(wbuf));
    
	if (ret)
	{
		Extflash_Deselect();
		return 0;
	}

	ret = Extflash_SpiRead(rbuf, sizeof(rbuf));
    
	Extflash_Deselect();
	//the 128Mflash  return EXT_FLASH_MAN_ID = NULL?
	if (ret || rbuf[0] != EXT_FLASH_MAN_ID || rbuf[1] != EXT_FLASH_DEV_ID)
	{

		return 0;
	}
	
	return 1;
}

/****************************************************************
** Function name:           Extflash_WaitReady
** Descriptions:            等待flash就绪
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ             
** Created Date:            2017-08-21
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
static int Extflash_WaitReady(void)
{
	const uint8_t wbuf[1] = { BLS_CODE_READ_STATUS };
	uint8_t ret;
	uint32_t cnt = 0;
	/* Throw away all garbage */
	
	for (;;)
	{
		uint8_t buf[2] = {0xFF, 0xFF};
		
		Extflash_Select();
        
		Extflash_SpiWrite(wbuf, sizeof(wbuf));
		ret = Extflash_SpiRead(buf, sizeof(buf));
        
		Extflash_Deselect();
		
		if (ret)
		{
			/* Error */
			return 2;
		}
		if (!(buf[0] & BLS_STATUS_BIT_BUSY))
		{
			/* Now ready */
			break;
		}
		cnt ++;
		if(cnt > 20000)
		{
			return 1;
		}
	}
	
	return 0;
}

/****************************************************************
** Function name:           Extflash_WriteEnable
** Descriptions:            写使能
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ             
** Created Date:            2017-08-21
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
static uint8_t Extflash_WriteEnable(void)
{
	const uint8_t wbuf[] = { BLS_CODE_WRITE_ENABLE };
	uint8_t ret = 0;
    
	Extflash_Select();
    
	ret = Extflash_SpiWrite(wbuf,sizeof(wbuf));
    
	Extflash_Deselect();
	
	if (ret)
	{
		return 3;
	}
	return 0;
}

/****************************************************************
** Function name:           Extflash_Open
** Descriptions:            初始化flash
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ             
** Created Date:            2017-08-21
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
uint8_t Extflash_Open(void)
{
	/* Put the part is standby mode */
	Extflash_PowerStandby();
	
	return Extflash_VerifyPart();
}

/****************************************************************
** Function name:           Extflash_Select
** Descriptions:            CS失能
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ             
** Created Date:            2017-08-21
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Extflash_Close(void)
{
	// Put the part in low power mode
	Extflash_PowerDown();
}

/****************************************************************
** Function name:           Extflash_Read
** Descriptions:            读数据
** input parameters:        offset：起始地址；length：长度；buf：数据
** output parameters:       无
** Returned value:          返回0：读失败；1：读成功
** Created by:              WJ             
** Created Date:            2017-08-21
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
uint8_t Extflash_Read(uint32_t offset, uint32_t length, uint8_t *buf)
{
	uint8_t wbuf[4];
	uint8_t ret = 0;
    uint32_t read_len = 0;
    uint16_t read_index = 0;
    
	/* Wait till previous erase/program operation completes */
	ret = Extflash_WaitReady();
	if (ret)
	{
		return 0;
	}
	
	/* SPI is driven with very low frequency (1MHz < 33MHz fR spec)
	* in this temporary implementation.
	* and hence it is not necessary to use fast read. */
	wbuf[0] = BLS_CODE_READ;
	wbuf[1] = (offset >> 16) & 0xff;
	wbuf[2] = (offset >> 8) & 0xff;
	wbuf[3] = offset & 0xff;
	
	Extflash_Select();
	
	if (Extflash_SpiWrite(wbuf, sizeof(wbuf)))
	{
		/* failure */
		Extflash_Deselect();
		return 0;
	}
    //WJ.20170725.SPI一次最大读写255个数据  
    while(read_index < length)
    {
        read_len = length - read_index;
        if(read_len > 255)
        {   //note: 52832 SPI一次最大发送255个字节
            read_len = 255;
        }       
        ret &= Extflash_SpiRead(&buf[read_index], read_len);       
        read_index += read_len;
    }
    
//	ret = Extflash_SpiRead(buf, length);
	
	Extflash_Deselect();
	
	return ret == 0;
}

/****************************************************************
** Function name:           Extflash_Write
** Descriptions:            写数据
** input parameters:        offset：起始地址；length：长度；buf：数据
** output parameters:       无
** Returned value:          返回0：写失败；1：写成功
** Created by:              WJ             
** Created Date:            2017-08-21
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
uint8_t Extflash_Write(uint32_t offset, uint32_t length,  uint8_t *buf)
{
    uint8_t ret = 0;
	uint8_t wbuf[4] = {0x00,};
    uint32_t ilen = 0; /* interim length per instruction */	
    
	while (length > 0)
	{
		/* Wait till previous erase/program operation completes */
		ret = Extflash_WaitReady();
		if (ret)
		{
			return 0;
		}
		
		ret = Extflash_WriteEnable();
		if (ret)
		{
			return 0;
		}		
		
		ilen = BLS_PROGRAM_PAGE_SIZE - (offset % BLS_PROGRAM_PAGE_SIZE);
		if (length < ilen)
		{
			ilen = length;
		}
		
		wbuf[0] = BLS_CODE_PROGRAM;
		wbuf[1] = (offset >> 16) & 0xff;
		wbuf[2] = (offset >> 8) & 0xff;
		wbuf[3] = offset & 0xff;
		
		offset += ilen;
		length -= ilen;
		
		/* Up to 100ns CS hold time (which is not clear
		* whether it's application only in between reads)
		* is not imposed here since above instructions
		* should be enough to delay
		* as much. */
		Extflash_Select();
		
		if (Extflash_SpiWrite(wbuf, sizeof(wbuf)))
		{
			/* failure */
			Extflash_Deselect();
			return 0;
		}
		
		if (Extflash_SpiWrite(buf,ilen))
		{
			/* failure */
			Extflash_Deselect();
			return 0;
		}
		buf += ilen;
		
        Extflash_Deselect();
	}
	
	return 1;
}

/****************************************************************
** Function name:           Extflash_Erase
** Descriptions:            按照起始地址和大小擦除
** input parameters:        offset：起始地址；length：长度
** output parameters:       无
** Returned value:          返回0：擦除失败；1：擦除成功
** Created by:              WJ             
** Created Date:            2017-08-21
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
uint8_t Extflash_Erase(uint32_t offset, uint32_t length)
{
	/* Note that Block erase might be more efficient when the floor map
	* is well planned for OTA but to simplify for the temporary implementation,
	* sector erase is used blindly. */
	uint8_t wbuf[4] = {0x00,};
	uint32_t i, numsectors = 0;
	
	wbuf[0] = BLS_CODE_SECTOR_ERASE;
	
	{
		uint32_t endoffset = offset + length - 1;
		offset = (offset / BLS_ERASE_SECTOR_SIZE) * BLS_ERASE_SECTOR_SIZE; 
		numsectors = (endoffset - offset + BLS_ERASE_SECTOR_SIZE - 1) /
			BLS_ERASE_SECTOR_SIZE;
	}
	
	for (i = 0; i < numsectors; i++)
	{
		/* Wait till previous erase/program operation completes */
		int ret = Extflash_WaitReady();
		if (ret)
		{
			return 0;
		}
		
		ret = Extflash_WriteEnable();
		if (ret)
		{
			return 0;
		}
		
		wbuf[1] = (offset >> 16) & 0xff;
		wbuf[2] = (offset >> 8) & 0xff;
		wbuf[3] = offset & 0xff;
		
		Extflash_Select();
		
		if (Extflash_SpiWrite(wbuf, sizeof(wbuf)))
		{
			/* failure */
			Extflash_Deselect();
			return 0;
		}
		Extflash_Deselect();
		
		offset += BLS_ERASE_SECTOR_SIZE;
	}	
	return 1;
}

/****************************************************************
** Function name:           Extflash_SectorErase
** Descriptions:            指定区擦除（16K）
** input parameters:        无
** output parameters:       无
** Returned value:          返回0：擦除失败；1：擦除成功
** Created by:              WJ             
** Created Date:            2017-08-21
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
uint8_t Extflash_SectorErase(uint16_t sector)
{ 
	uint32_t addr;
	uint8_t  wbuf[4];
	
	addr  = sector*BLS_ERASE_SECTOR_SIZE;
	
	/* Wait till previous erase/program operation completes */
	int ret = Extflash_WaitReady();  
	if (ret)
	{
		return 0;
	}
	
	ret = Extflash_WriteEnable();
	if (ret)
	{
		return 0;
	}
	
	wbuf[0] = BLS_CODE_ERASE_4K;
	wbuf[1] = (addr >> 16) & 0xff;
	wbuf[2] = (addr >> 8) & 0xff;
	wbuf[3] = addr & 0xff;
	
	Extflash_Select();
	
	if (Extflash_SpiWrite(wbuf, sizeof(wbuf)))
	{
		/* fail*/
		Extflash_Deselect();
		return 0;
	}
	
	Extflash_Deselect();	
	return 1;	
}
 
/*
block addr <128   
*/
/****************************************************************
** Function name:           Extflash_BlockErase
** Descriptions:            指定块擦除（64K）
** input parameters:        block：指定块
** output parameters:       无
** Returned value:          返回0：擦除失败；1：擦除成功
** Created by:              WJ             
** Created Date:            2017-08-21
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
uint8_t Extflash_BlockErase(uint8_t block)
{ 
	uint32_t addr= 0;  
	uint8_t  wbuf[4] = {0x00, };
	
	
	addr=block*BLS_ERASE_BLOCK_SIZE;     
	/* Wait till previous erase/program operation completes */
	int ret = Extflash_WaitReady();  
	if (ret)
	{
		return 0;
	}
	
	ret = Extflash_WriteEnable();
	if (ret)
	{
		return 0;
	}
	
	wbuf[0] = BLS_CODE_ERASE_64K;
	wbuf[1] = (addr >> 16) & 0xff;
	wbuf[2] = (addr >> 8) & 0xff;
	wbuf[3] = addr & 0xff;
	
	Extflash_Select();
	
	if (Extflash_SpiWrite(wbuf, sizeof(wbuf)))
	{
		/* fail */
		Extflash_Deselect();
		return 0;
	}
	 
	Extflash_Deselect();
	
	return 1;		
}


/****************************************************************
** Function name:           Extflash_AllErase
** Descriptions:            擦除芯片
** input parameters:        无
** output parameters:       无
** Returned value:          返回0：擦除失败；1：擦除成功
** Created by:              WJ             
** Created Date:            2017-08-21
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
uint8_t Extflash_AllErase(void)
{ 
	uint8_t  wbuf[1] = {BLS_CODE_ERASE_ALL };
	    
	/* Wait till previous erase/program operation completes */
	int ret = Extflash_WaitReady();  
	if (ret)
	{
		return 0;
	}
	
	ret = Extflash_WriteEnable();
	if (ret)
	{
		return 0;
	}
	
	Extflash_Select();
	
	if (Extflash_SpiWrite(wbuf, sizeof(wbuf)))
	{
		/* fail */
		Extflash_Deselect();
		return 0;
	}
	 
	Extflash_Deselect();
	
	return 1;		
}


