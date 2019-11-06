/***************************Copyright(c)***********************************
**                       Veepoo Tech
**                  COPYRIGHT (C) 2016-2019
**-----------File Info-----------------------------------------------------
** File        name:     i2c.c
** Last modified Date:   2017-08-24
** Last SYS_VERSION:     0.0.0
** Descriptions:            
**-------------------------------------------------------------------------
** Created  by:          WJ
** Created  date:        2017-08-24
** SYS_VERSION:          0.0.0
** Descriptions:         
**-------------------------------------------------------------------------
** Modified by:				
** Modified date:  			          
** SYS_VERSION:                 
** Descriptions:                        
***************************************************************************/
#include "twi_sw_master.h"
#include <string.h>

#ifdef APP_IIC0

/****************************************************************
** Function name:           I2c_Init
** Descriptions:            ��ʼ��
** input parameters:        ��
** output parameters:       ��
** Returned value:          1:true,0:false
** Created by:              WJ              
** Created Date:            2016-09-16
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/
bool I2c_Init(void)
{
    return  twi_master_init();
}

/****************************************************************
** Function name:           I2c_Write
** Descriptions:            д����
** input parameters:        address:��ַ��data��д���飻bytes��д����
** output parameters:       ��
** Returned value:          1:true,0:false
** Created by:              WJ              
** Created Date:            2016-09-16
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
bool I2c_Write(uint8_t address, uint8_t *data, uint8_t bytes)
{
	return twi_master_transfer(address<<1, data, bytes, TWI_ISSUE_STOP);
}


/****************************************************************
** Function name:           I2c_Read      
** Descriptions:            ������
** input parameters:        address:��ַ��bytes��������
** output parameters:       data����ȡ����
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2016-09-16
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
bool I2c_Read(uint8_t address, uint8_t *data, uint8_t bytes)
{
	bool transfer_succeeded = true;
	transfer_succeeded &= twi_master_transfer((address<<1)|TWI_READ_BIT, data, bytes, TWI_ISSUE_STOP);
	return(transfer_succeeded);
}
#endif

/*<-------------------------------- iic1 ----------------------------------------->*/
#ifdef APP_IIC1

/****************************************************************
** Function name:           I2c1_Init
** Descriptions:            ��ʼ��
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2016-09-16
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/
bool I2c1_Init(void)
{
    return  twi1_master_init();
}

/****************************************************************
** Function name:           I2c1_Write
** Descriptions:            д����
** input parameters:        address:��ַ��data��д���飻bytes��д����
** output parameters:       ��
** Returned value:          1:true,0:false
** Created by:              WJ              
** Created Date:            2016-09-16
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
bool I2c1_Write(uint8_t address, uint8_t *data, uint8_t bytes)
{
    bool transfer_succeeded = true;
    transfer_succeeded = twi1_master_transfer(address<<1, data, bytes, TWI_ISSUE_STOP);
	return transfer_succeeded;	
}

/****************************************************************
** Function name:           I2c1_Read      
** Descriptions:            ������
** input parameters:        address:��ַ��bytes��������
** output parameters:       data����ȡ����
** Returned value:          1:true,0:false
** Created by:              WJ              
** Created Date:            2016-09-16
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
bool I2c1_Read(uint8_t address, uint8_t *data, uint8_t bytes)
{
	bool transfer_succeeded = true;
	transfer_succeeded &= twi1_master_transfer((address<<1)|TWI_READ_BIT, data, bytes, TWI_ISSUE_STOP);
	return(transfer_succeeded);
}
#endif

