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
#include "board.h"
#include "bsp_spi.h"
#include "ext_flash.h"


#define EXT_FLASH_CS_LOW        PIN_LOW(FLASH_CS_PIN);  
#define EXT_FLASH_CS_HIGH       PIN_HIGH(FLASH_CS_PIN);  

#define EXT_FLASH_D_CS_LOW        PIN_LOW(FLASH_DATA_CS_PIN);  
#define EXT_FLASH_D_CS_HIGH       PIN_HIGH(FLASH_DATA_CS_PIN);

//#define EXT_FLASH_D_CS_LOW			Bsp_DataFlashCsLow();
//#define EXT_FLASH_D_CS_HIGH			Bsp_DataFlashCsHigh();

/* Instruction codes */
#define BLS_CODE_PROGRAM          0x02 /**< Page Program */
#define BLS_CODE_READ             0x03 /**< Read Data */
#define BLS_CODE_READ_STATUS      0x05 /**< Read Status Register */
#define BLS_CODE_WRITE_ENABLE     0x06 /**< Write Enable */
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


const uint8_t flash_id_buf[][2] = {
    {0x5E, 0x13},       //о����8M
    {0x0B, 0x14},       //о����16M 
    {0xEF, 0x15},       //����32M 
    {0xEF, 0x16},       //����64M 
    {0x0B, 0x13},       //о����8M, �°汾20180612
    {0xC8, 0x14},       //GD-8M
    {0xC8, 0x13},       //GD-16M
    {0xC8, 0x16},       //GD-64M
    {0xC8, 0x17},       //GD-128M
    {0x0B, 0x15},       //о����32M
}; 

const uint32_t flash_size_buf[] = {
    0x100000,       //о����8M
    0x200000,       //о����16M 
    0x400000,       //����32M 
    0x800000,       //����64M 
    0x100000,       //о����8M, �°汾20180612
    0x100000,       //GD-8M
    0x200000,       //GD-16M
    0x800000,       //GD-64M
    0x1000000,       //GD-128M
    0x400000,       //о����32M
};


// Private functions
static int Extflash_WaitReady(void);
//static int Extflash_WaitPowerDown(void);


/****************************************************************
** Function name:           Extflash_Select
** Descriptions:            CSʹ��
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ             
** Created Date:            2017-08-21
*****************************************************************/ 
//static 
void Extflash_Select(void)
{
    EXT_FLASH_CS_LOW;
    PIN_HIGH(LCD_CS_PIN);
}

/****************************************************************
** Function name:           Extflash_Select
** Descriptions:            CSʧ��
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ             
** Created Date:            2017-08-21
*****************************************************************/ 
//static 
    void Extflash_Deselect(void)
{
    EXT_FLASH_CS_HIGH;
}

/****************************************************************
** Function name:           Extflash_SpiWrite
** Descriptions:            д���ݽӿ�
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ             
** Created Date:            2017-08-21
*****************************************************************/ 
static uint8_t Extflash_SpiWrite(const uint8_t *data, uint8_t len)
{
    uint8_t ret = 0;
    
    ret = !Bps_Spi2Write(data, len);   
    return ret;    
}

/****************************************************************
** Function name:           Extflash_SpiRead
** Descriptions:            �����ݽӿ�
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ             
** Created Date:            2017-08-21
*****************************************************************/ 
static uint8_t Extflash_SpiRead(uint8_t *data, uint8_t len)
{
    uint8_t ret = 0;
    
    ret = !Bps_Spi2Read(data, len);   
    return ret;
}

/****************************************************************
** Function name:           Extflash_PowerDown
** Descriptions:            ����
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ             
** Created Date:            2017-08-21
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
** Descriptions:            CSʧ��
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ             
** Created Date:            2017-08-21
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
** Descriptions:            У��
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ             
** Created Date:            2017-08-21
*****************************************************************/ 
uint8_t rbuf[2] = {0};
static uint8_t Extflash_VerifyPart(void)
{
    //verify the manufacturer and device ID   (90 dummy dummy  00)
	const uint8_t wbuf[] = {BLS_CODE_MDID, 0x00, 0x00, 0x00 };  
//	uint8_t rbuf[2]={0};		
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
** Descriptions:            �ȴ�flash����
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ             
** Created Date:            2017-08-21
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
** Descriptions:            дʹ��
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ             
** Created Date:            2017-08-21
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
** Descriptions:            ��ʼ��flash
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ             
** Created Date:            2017-08-21
*****************************************************************/ 
uint8_t Extflash_Open(void)
{
	/* Put the part is standby mode */
	Extflash_PowerStandby();
    
	return Extflash_VerifyPart();
}

/****************************************************************
** Function name:           Extflash_Close
** Descriptions:            CSʧ��
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ             
** Created Date:            2017-08-21
*****************************************************************/ 
void Extflash_Close(void)
{
	// Put the part in low power mode
	Extflash_PowerDown();
}

/****************************************************************
** Function name:           Extflash_Read
** Descriptions:            ������
** input parameters:        offset����ʼ��ַ��length�����ȣ�buf������
** output parameters:       ��
** Returned value:          ����0����ʧ�ܣ�1�����ɹ�
** Created by:              WJ             
** Created Date:            2017-08-21
*****************************************************************/ 
uint8_t Extflash_Read(uint32_t offset, uint32_t length, uint8_t *buf)
{
	uint8_t wbuf[4];
	uint8_t ret = 0;
    uint32_t read_len = 0;
    uint16_t read_index = 0;
    
//	/* Wait till previous erase/program operation completes */
//	ret = Extflash_WaitReady();
//	if (ret)
//	{
//		return 0;
//	}
	
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
    //WJ.20170725.SPIһ������д255������  
    while(read_index < length)
    {
        read_len = length - read_index;
        if(read_len > 255)
        {   //note: 52832 SPIһ�������255���ֽ�
            read_len = 255;
        }       
        ret &= Extflash_SpiRead(&buf[read_index], read_len);       
        read_index += read_len;
    }
    
	Extflash_Deselect();
	
	return ret == 0;
}


/****************************************************************
** Function name:           Extflash_PrepareRead
** Descriptions:            ׼��������
** input parameters:        offset����ʼ��ַ
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ             
** Created Date:            2017-08-21
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
uint8_t Extflash_PrepareRead(uint32_t offset)
{
	uint8_t wbuf[5];
    
	/* Wait till previous erase/program operation completes */
	
	/* SPI is driven with very low frequency (1MHz < 33MHz fR spec)
	* in this temporary implementation.
	* and hence it is not necessary to use fast read. */
	wbuf[0] = BLS_CODE_READ;
	wbuf[1] = (offset >> 16) & 0xff;
	wbuf[2] = (offset >> 8) & 0xff;
	wbuf[3] = offset & 0xff;
	
	Extflash_Select();
	
	if (Extflash_SpiWrite(wbuf, 4))
	{
		/* failure */
		Extflash_Deselect();
		return 0;
	}
    
    return 1;
}

/****************************************************************
** Function name:           Extflash_Write
** Descriptions:            д����
** input parameters:        offset����ʼ��ַ��length�����ȣ�buf������
** output parameters:       ��
** Returned value:          ����0��дʧ�ܣ�1��д�ɹ�
** Created by:              WJ             
** Created Date:            2017-08-21
*****************************************************************/ 
uint8_t Extflash_Write(uint32_t offset, uint32_t length, uint8_t *buf)
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
		
        if(ilen > 255)
        {
            if (Extflash_SpiWrite(buf, 255))
            {
                /* failure */
                Extflash_Deselect();
                return 0;
            }
            ilen -= 255;
            buf += 255;
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
** Descriptions:            ������ʼ��ַ�ʹ�С����
** input parameters:        offset����ʼ��ַ��length������
** output parameters:       ��
** Returned value:          ����0������ʧ�ܣ�1�������ɹ�
** Created by:              WJ             
** Created Date:            2017-08-21
*****************************************************************/ 
uint8_t Extflash_Erase(uint32_t offset, uint32_t length)
{
	/* Note that Block erase might be more efficient when the floor map
	* is well planned for OTA but to simplify for the temporary implementation,
	* sector erase is used blindly. */
	uint8_t wbuf[4] = {0x00,};
	uint32_t i = 0, numsectors = 0;
	
	wbuf[0] = BLS_CODE_ERASE_4K;
	
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
** Function name:           Extflash_SectorErase4K
** Descriptions:            ָ����������4K��
** input parameters:        ��
** output parameters:       ��
** Returned value:          ����0������ʧ�ܣ�1�������ɹ�
** Created by:              WJ             
** Created Date:            2017-08-21
*****************************************************************/ 
uint8_t Extflash_SectorErase4K(uint16_t sector)
{ 
	uint32_t addr = 0;
	uint8_t  wbuf[4] = {0,};
	
	addr = sector * 4096;
	
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

/****************************************************************
** Function name:           Extflash_BlockErase32K
** Descriptions:            ָ���������32K��
** input parameters:        block��ָ����
** output parameters:       ��
** Returned value:          ����0������ʧ�ܣ�1�������ɹ�
** Created by:              WJ             
** Created Date:            2017-08-21
*****************************************************************/ 
uint8_t Extflash_BlockErase32K(uint8_t block)
{ 
	uint32_t addr= 0;  
	uint8_t  wbuf[4] = {0x00, };
	
	
	addr = block * (4096 * 8);     
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
	
	wbuf[0] = BLS_CODE_ERASE_32K;
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
** Function name:           Extflash_BlockErase64K
** Descriptions:            ָ���������64K��
** input parameters:        block��ָ����
** output parameters:       ��
** Returned value:          ����0������ʧ�ܣ�1�������ɹ�
** Created by:              WJ             
** Created Date:            2017-08-21
*****************************************************************/ 
uint8_t Extflash_BlockErase64K(uint8_t block)
{ 
	uint32_t addr= 0;  
	uint8_t  wbuf[4] = {0x00, };
	
	
	addr = block * (4096 * 16);     
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
** Descriptions:            ����оƬ
** input parameters:        ��
** output parameters:       ��
** Returned value:          ����0������ʧ�ܣ�1�������ɹ�
** Created by:              WJ             
** Created Date:            2017-08-21
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


// Private functions
static int Extflash_DataWaitReady(void);
//static int Extflash_WaitPowerDown(void);


/****************************************************************
** Function name:           Extflash_DataSelect
** Descriptions:            CSʹ��
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ             
** Created Date:            2017-08-21
*****************************************************************/ 
static void Extflash_DataSelect(void)
{  
    EXT_FLASH_D_CS_LOW;
}

/****************************************************************
** Function name:           Extflash_DataDeselect
** Descriptions:            CSʧ��
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ             
** Created Date:            2017-08-21
*****************************************************************/ 
static void Extflash_DataDeselect(void)
{
    EXT_FLASH_D_CS_HIGH;
}

/****************************************************************
** Function name:           Extflash_DataSpiWrite
** Descriptions:            д���ݽӿ�
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ             
** Created Date:            2017-08-21
*****************************************************************/ 
static uint8_t Extflash_DataSpiWrite(const uint8_t *data, uint8_t len)
{
    uint8_t ret = 0;
    
    ret = !Bps_Spi2Write(data, len);   
    return ret;    
}

/****************************************************************
** Function name:           Extflash_DataSpiRead
** Descriptions:            �����ݽӿ�
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ             
** Created Date:            2017-08-21
*****************************************************************/ 
static uint8_t Extflash_DataSpiRead(uint8_t *data, uint8_t len)
{
    uint8_t ret = 0;
    
    ret = !Bps_Spi2Read(data, len);   
    return ret;
}

/****************************************************************
** Function name:           Extflash_DataPowerDown
** Descriptions:            ����
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ             
** Created Date:            2017-08-21
*****************************************************************/ 
static uint8_t Extflash_DataPowerDown(void)
{
	uint8_t cmd = 0;
	uint8_t success = 0;
	
	cmd = BLS_CODE_DP;
	Extflash_DataSelect();
    
	success = Extflash_DataSpiWrite(&cmd, sizeof(cmd)) == 0;
    
	Extflash_DataDeselect();
	
	return success;
}

/****************************************************************
** Function name:           Extflash_DataPowerStandby
** Descriptions:            CSʧ��
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ             
** Created Date:            2017-08-21
*****************************************************************/ 
static uint8_t Extflash_DataPowerStandby(void)
{
	uint8_t cmd = 0;
	uint8_t success = 0;
	
	cmd = BLS_CODE_RDP;
	Extflash_DataSelect();
	success = Extflash_DataSpiWrite(&cmd, sizeof(cmd)) == 0;   
	Extflash_DataDeselect();
	
	if(success)
    {
		success = Extflash_DataWaitReady() == 0;
	}
	return success;
}

/****************************************************************
** Function name:           Extflash_DataVerifyPart
** Descriptions:            У��
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ             
** Created Date:            2017-08-21
*****************************************************************/ 
uint8_t rbuf_data[2] = {0};
static uint8_t Extflash_DataVerifyPart(void)
{
    //verify the manufacturer and device ID   (90 dummy dummy  00)
	const uint8_t wbuf[] = {BLS_CODE_MDID, 0x00, 0x00, 0x00 };  
//	uint8_t rbuf[2]={0};		
    uint8_t ret = 0;
	
	Extflash_DataSelect();
	
	ret = Extflash_DataSpiWrite(wbuf, sizeof(wbuf));
    
	if (ret)
	{
		Extflash_DataDeselect();
		return 0;
	}

	ret = Extflash_DataSpiRead(rbuf_data, sizeof(rbuf_data));
    
	Extflash_DataDeselect();

	if (ret || rbuf_data[0] != EXT_FLASH_DATA_MAN_ID || rbuf_data[1] != EXT_FLASH_DATA_DEV_ID)
	{
		return 0;
	}
	
	return 1;
}

/****************************************************************
** Function name:           Extflash_DataWaitReady
** Descriptions:            �ȴ�flash����
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ             
** Created Date:            2017-08-21
*****************************************************************/ 
static int Extflash_DataWaitReady(void)
{
	const uint8_t wbuf[1] = { BLS_CODE_READ_STATUS };
	uint8_t ret;
	uint32_t cnt = 0;
	/* Throw away all garbage */
	
	for (;;)
	{
		uint8_t buf[2] = {0xFF, 0xFF};
		
		Extflash_DataSelect();
        
		Extflash_DataSpiWrite(wbuf, sizeof(wbuf));
		ret = Extflash_DataSpiRead(buf, sizeof(buf));
        
		Extflash_DataDeselect();
		
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
** Function name:           Extflash_DataWriteEnable
** Descriptions:            дʹ��
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ             
** Created Date:            2017-08-21
*****************************************************************/ 
static uint8_t Extflash_DataWriteEnable(void)
{
	const uint8_t wbuf[] = { BLS_CODE_WRITE_ENABLE };
	uint8_t ret = 0;
    
	Extflash_DataSelect();
    
	ret = Extflash_DataSpiWrite(wbuf,sizeof(wbuf));
    
	Extflash_DataDeselect();
	
	if (ret)
	{
		return 3;
	}
	return 0;
}

/****************************************************************
** Function name:           Extflash_DataOpen
** Descriptions:            ��ʼ��flash
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ             
** Created Date:            2017-08-21
*****************************************************************/ 
uint8_t Extflash_DataOpen(void)
{
	/* Put the part is standby mode */
	Extflash_DataPowerStandby();
    
	return Extflash_DataVerifyPart();
}

/****************************************************************
** Function name:           Extflash_DataClose
** Descriptions:            CSʧ��
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ             
** Created Date:            2017-08-21
*****************************************************************/ 
void Extflash_DataClose(void)
{
	// Put the part in low power mode
	Extflash_DataPowerDown();
}

/****************************************************************
** Function name:           Extflash_DataRead
** Descriptions:            ������
** input parameters:        offset����ʼ��ַ��length�����ȣ�buf������
** output parameters:       ��
** Returned value:          ����0����ʧ�ܣ�1�����ɹ�
** Created by:              WJ             
** Created Date:            2017-08-21
*****************************************************************/ 
uint8_t Extflash_DataRead(uint32_t offset, uint32_t length, uint8_t *buf)
{
	uint8_t wbuf[4];
	uint8_t ret = 0;
    uint32_t read_len = 0;
    uint16_t read_index = 0;
    
	/* Wait till previous erase/program operation completes */
	ret = Extflash_DataWaitReady();
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
	
	Extflash_DataSelect();
	
	if (Extflash_DataSpiWrite(wbuf, sizeof(wbuf)))
	{
		/* failure */
		Extflash_DataDeselect();
		return 0;
	}
    //WJ.20170725.SPIһ������д255������  
    while(read_index < length)
    {
        read_len = length - read_index;
        if(read_len > 255)
        {   //note: 52832 SPIһ�������255���ֽ�
            read_len = 255;
        }       
        ret &= Extflash_DataSpiRead(&buf[read_index], read_len);       
        read_index += read_len;
    }
    
	Extflash_DataDeselect();
	
	return ret == 0;
}

/****************************************************************
** Function name:           Extflash_DataWrite
** Descriptions:            д����
** input parameters:        offset����ʼ��ַ��length�����ȣ�buf������
** output parameters:       ��
** Returned value:          ����0��дʧ�ܣ�1��д�ɹ�
** Created by:              WJ             
** Created Date:            2017-08-21
*****************************************************************/ 
uint8_t Extflash_DataWrite(uint32_t offset, uint32_t length, uint8_t *buf)
{
    uint8_t ret = 0;
	uint8_t wbuf[4] = {0x00,};
    uint32_t ilen = 0; /* interim length per instruction */	
    
	while (length > 0)
	{
		/* Wait till previous erase/program operation completes */
		ret = Extflash_DataWaitReady();
		if (ret)
		{
			return 0;
		}
		
		ret = Extflash_DataWriteEnable();
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
		Extflash_DataSelect();
		
		if (Extflash_DataSpiWrite(wbuf, sizeof(wbuf)))
		{
			/* failure */
			Extflash_DataDeselect();
			return 0;
		}
		
        if(ilen > 255)
        {
            if (Extflash_DataSpiWrite(buf, 255))
            {
                /* failure */
                Extflash_DataDeselect();
                return 0;
            }
            ilen -= 255;
            buf += 255;
        }           
        if (Extflash_DataSpiWrite(buf,ilen))
        {
            /* failure */
            Extflash_DataDeselect();
            return 0;
        }
		buf += ilen;
		
        Extflash_DataDeselect();
	}
	
	return 1;
}

/****************************************************************
** Function name:           Extflash_DataErase
** Descriptions:            ������ʼ��ַ�ʹ�С����
** input parameters:        offset����ʼ��ַ��length������
** output parameters:       ��
** Returned value:          ����0������ʧ�ܣ�1�������ɹ�
** Created by:              WJ             
** Created Date:            2017-08-21
*****************************************************************/ 
uint8_t Extflash_DataErase(uint32_t offset, uint32_t length)
{
	/* Note that Block erase might be more efficient when the floor map
	* is well planned for OTA but to simplify for the temporary implementation,
	* sector erase is used blindly. */
	uint8_t wbuf[4] = {0x00,};
	uint32_t i = 0, numsectors = 0;
	
	wbuf[0] = BLS_CODE_ERASE_4K;
	
	{
		uint32_t endoffset = offset + length - 1;
		offset = (offset / BLS_ERASE_SECTOR_SIZE) * BLS_ERASE_SECTOR_SIZE; 
		numsectors = (endoffset - offset + BLS_ERASE_SECTOR_SIZE - 1) /
			BLS_ERASE_SECTOR_SIZE;
	}
	
	for (i = 0; i < numsectors; i++)
	{
		/* Wait till previous erase/program operation completes */
		int ret = Extflash_DataWaitReady();
		if (ret)
		{
			return 0;
		}
		
		ret = Extflash_DataWriteEnable();
		if (ret)
		{
			return 0;
		}
		
		wbuf[1] = (offset >> 16) & 0xff;
		wbuf[2] = (offset >> 8) & 0xff;
		wbuf[3] = offset & 0xff;
		
		Extflash_DataSelect();
		
		if (Extflash_DataSpiWrite(wbuf, sizeof(wbuf)))
		{
			/* failure */
			Extflash_DataDeselect();
			return 0;
		}
		Extflash_DataDeselect();
		
		offset += BLS_ERASE_SECTOR_SIZE;
	}	
	return 1;
}

/****************************************************************
** Function name:           Extflash_DataSectorErase4K
** Descriptions:            ָ����������4K��
** input parameters:        ��
** output parameters:       ��
** Returned value:          ����0������ʧ�ܣ�1�������ɹ�
** Created by:              WJ             
** Created Date:            2017-08-21
*****************************************************************/ 
uint8_t Extflash_DataSectorErase4K(uint16_t sector)
{ 
	uint32_t addr = 0;
	uint8_t  wbuf[4] = {0,};
	
	addr = sector * 4096;
	
	/* Wait till previous erase/program operation completes */
	int ret = Extflash_DataWaitReady();  
	if (ret)
	{
		return 0;
	}
	
	ret = Extflash_DataWriteEnable();
	if (ret)
	{
		return 0;
	}
	
	wbuf[0] = BLS_CODE_ERASE_4K;
	wbuf[1] = (addr >> 16) & 0xff;
	wbuf[2] = (addr >> 8) & 0xff;
	wbuf[3] = addr & 0xff;
	
	Extflash_DataSelect();
	
	if (Extflash_DataSpiWrite(wbuf, sizeof(wbuf)))
	{
		/* fail*/
		Extflash_DataDeselect();
		return 0;
	}
	
	Extflash_DataDeselect();	
	return 1;	
}

/****************************************************************
** Function name:           Extflash_DataBlockErase32K
** Descriptions:            ָ���������32K��
** input parameters:        block��ָ����
** output parameters:       ��
** Returned value:          ����0������ʧ�ܣ�1�������ɹ�
** Created by:              WJ             
** Created Date:            2017-08-21
*****************************************************************/ 
uint8_t Extflash_DataBlockErase32K(uint8_t block)
{ 
	uint32_t addr= 0;  
	uint8_t  wbuf[4] = {0x00, };
	
	
	addr = block * (4096 * 8);     
	/* Wait till previous erase/program operation completes */
	int ret = Extflash_DataWaitReady();  
	if (ret)
	{
		return 0;
	}
	
	ret = Extflash_DataWriteEnable();
	if (ret)
	{
		return 0;
	}
	
	wbuf[0] = BLS_CODE_ERASE_32K;
	wbuf[1] = (addr >> 16) & 0xff;
	wbuf[2] = (addr >> 8) & 0xff;
	wbuf[3] = addr & 0xff;
	
	Extflash_DataSelect();
	
	if (Extflash_DataSpiWrite(wbuf, sizeof(wbuf)))
	{
		/* fail */
		Extflash_DataDeselect();
		return 0;
	}
	 
	Extflash_DataDeselect();
	
	return 1;		
}

/****************************************************************
** Function name:           Extflash_DataBlockErase64K
** Descriptions:            ָ���������64K��
** input parameters:        block��ָ����
** output parameters:       ��
** Returned value:          ����0������ʧ�ܣ�1�������ɹ�
** Created by:              WJ             
** Created Date:            2017-08-21
*****************************************************************/ 
uint8_t Extflash_DataBlockErase64K(uint8_t block)
{ 
	uint32_t addr= 0;  
	uint8_t  wbuf[4] = {0x00, };
	
	
	addr = block * (4096 * 16);     
	/* Wait till previous erase/program operation completes */
	int ret = Extflash_DataWaitReady();  
	if (ret)
	{
		return 0;
	}
	
	ret = Extflash_DataWriteEnable();
	if (ret)
	{
		return 0;
	}
	
	wbuf[0] = BLS_CODE_ERASE_64K;
	wbuf[1] = (addr >> 16) & 0xff;
	wbuf[2] = (addr >> 8) & 0xff;
	wbuf[3] = addr & 0xff;
	
	Extflash_DataSelect();
	
	if (Extflash_DataSpiWrite(wbuf, sizeof(wbuf)))
	{
		/* fail */
		Extflash_DataDeselect();
		return 0;
	}
	 
	Extflash_DataDeselect();
	
	return 1;		
}

/****************************************************************
** Function name:           Extflash_DataAllErase
** Descriptions:            ����оƬ
** input parameters:        ��
** output parameters:       ��
** Returned value:          ����0������ʧ�ܣ�1�������ɹ�
** Created by:              WJ             
** Created Date:            2017-08-21
*****************************************************************/ 
uint8_t Extflash_DataAllErase(void)
{ 
	uint8_t  wbuf[1] = {BLS_CODE_ERASE_ALL };
	    
	/* Wait till previous erase/program operation completes */
	int ret = Extflash_DataWaitReady();  
	if (ret)
	{
		return 0;
	}
	
	ret = Extflash_DataWriteEnable();
	if (ret)
	{
		return 0;
	}
	
	Extflash_DataSelect();
	
	if (Extflash_DataSpiWrite(wbuf, sizeof(wbuf)))
	{
		/* fail */
		Extflash_DataDeselect();
		return 0;
	}
	 
	Extflash_DataDeselect();
	
	return 1;		
}

/****************************************************************
** Function name:			Extflash_DataIsBusy
** Descriptions:            оƬ״̬
** input parameters:        ��
** output parameters:       ��
** Returned value:          ����0:����;1:æµ
** Created by:              WJ             
** Created Date:           	2018-05-07
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
uint8_t Extflash_DataIsBusy(void)
{ 
	return Extflash_WaitReady();		
}
