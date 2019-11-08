/**************************************************************************************************
Filename:       hal_acc.c
Revised:        $Date: 2013-08-23 11:45:31 -0700 (Fri, 23 Aug 2013) $
Revision:       $Revision: 35100 $

Description:    Driver for the Kionix KXTI9 Accelerometer.

Copyright 2012-2013 Texas Instruments Incorporated. All rights reserved.

IMPORTANT: Your use of this Software is limited to those specific rights
granted under the terms of a software license agreement between the user
who downloaded the software, his/her employer (which must be your employer)
and Texas Instruments Incorporated (the "License").  You may not use this
Software unless you agree to abide by the terms of the License. The License
limits your use, and you acknowledge, that the Software may not be modified,
copied or distributed unless embedded on a Texas Instruments microcontroller
or used solely and exclusively in conjunction with a Texas Instruments radio
frequency transceiver, which is integrated into your product.  Other than for
the foregoing purpose, you may not use, reproduce, copy, prepare derivative
works of, modify, distribute, perform, display or sell this Software and/or
its documentation for any purpose.

YOU FURTHER ACKNOWLEDGE AND AGREE THAT THE SOFTWARE AND DOCUMENTATION ARE
PROVIDED �AS IS� WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED,
INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF MERCHANTABILITY, TITLE,
NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT SHALL
TEXAS INSTRUMENTS OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER CONTRACT,
NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR OTHER
LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE
OR CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT
OF SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.

Should you have any questions regarding your right to use this Software,
contact Texas Instruments Incorporated at www.TI.com.

**************************************************************************************************/

/* ------------------------------------------------------------------------------------------------
*                                          Includes
* ------------------------------------------------------------------------------------------------
*/

#include "nrf_drv_gpiote.h"
#include "app_error.h" 
#include "hal_acc.h"
#include "app_type.h"
#include "hal_acc_calculate.h"
#include "nrf_gpio.h"



#ifdef ACC_USE_HARDWARE_IIC        //ʹ��Ӳ��IIC
    #include "bsp_iic.h"
#else
    #include "I2C.h"
#endif
/*********************************************************************
* �궨��
*/
#define delay_ms(i)    nrf_delay_ms(i)      //ƽ̨�����ʱ

/*********************************************************************
* ȫ�ֱ���
*/
static uint8_t acc_address = 0;     //iic address, it's had shifted

static uint8_t  acc_xh = 0;
static uint8_t  acc_yh = 0;
static uint8_t  acc_zh = 0;

/*********************************************************************
* ���غ�������
*/
void HalAcc_CleanInterrupt(void);
bool HalAcc_Write(uint8_t register_address, uint8_t *pData, uint8_t bytes);
bool HalAcc_Read(uint8_t register_address, uint8_t *pData, uint8_t bytes);
void HalAcc_Address_Init(uint8_t address);
void HalAcc_Configuration(void);
void HalAcc_Sleep(void);
void HalAcc_Init(void);

/*******************************************************************************
** Function name:		    HalAcc_CleanInterrupt
** Descriptions:            �����ж����
** input parameters:
** output parameters:
** Returned value:
** Created by:			    WJ              
** Created Date:           	2016-08-04
**------------------------------------------------------------------------------
** Modified by:
** Modified date:
**------------------------------------------------------------------------------
*******************************************************************************/
void HalAcc_CleanInterrupt(void)
{
	uint8_t tData;
    HalAcc_Read( ACC_REG_ADDR_INT_REL, &tData,1);
//	while(!(HalAcc_Read( ACC_REG_ADDR_INT_REL, &tData,1))); //clean interrupt 
    
}

/****************************************************************
** Function name:			HalAcc_Sleep
** Descriptions:            
** input parameters:
** output parameters:
** Returned value:
** Created by:				WJ              
** Created Date:           	2016-09-26
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void HalAcc_Sleep(void)
{
	uint8_t tData = 0x00;
	tData	=	0x00;
	HalAcc_Write(ACC_REG_ADDR_CTRL_REG1, &tData,1);//0x1b	
}

/****************************************************************
** Function name:			HalAcc_Write
** Descriptions:            
** input parameters:
** output parameters:
** Returned value:
** Created by:				WJ              
** Created Date:           	2016-09-26
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
bool HalAcc_Write(uint8_t register_address, uint8_t *pData, uint8_t bytes)
{
	uint8_t w2_data[2]={0};
	
	w2_data[0] = register_address;
	w2_data[1] = *pData;
	
#ifdef ACC_USE_HARDWARE_IIC        //ʹ��Ӳ��IIC	
	return  Bsp_IIC0Write(acc_address, w2_data, 2);
#else
    return  I2c_Write(acc_address, w2_data, 2);
#endif
}

/****************************************************************
** Function name:			HalAcc_Read
** Descriptions:            
** input parameters:
** output parameters:
** Returned value:
** Created by:				WJ              
** Created Date:           	2016-09-26
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
bool HalAcc_Read(uint8_t register_address, uint8_t *pData, uint8_t bytes)
{
	bool transfer_succeeded ;
	
#ifdef ACC_USE_HARDWARE_IIC        //ʹ��Ӳ��IIC	
    transfer_succeeded = Bsp_IIC0Write(acc_address, &register_address, 1);
    if(!transfer_succeeded) return transfer_succeeded;
	transfer_succeeded &= Bsp_IIC0Read(acc_address, pData, bytes);	
#else
    transfer_succeeded = I2c_Write(acc_address, &register_address, 1);
    if(!transfer_succeeded) return transfer_succeeded;
	transfer_succeeded &= I2c_Read(acc_address, pData, bytes);
#endif	
	return transfer_succeeded;	
}


/****************************************************************
** Function name:			HalAcc_ReadData
** Descriptions:            
** input parameters:
** output parameters:
** Returned value:
** Created by:				WJ              
** Created Date:           	2016-09-26
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
bool HalAcc_ReadData(int16_t *pData)
{
	uint8_t x_h = 0; 
	uint8_t x_l = 0;

	uint8_t y_h = 0;
	uint8_t y_l = 0;

	uint8_t z_h = 0;
	uint8_t z_l = 0;

	bool success;

	// Read the three registers
	success  = HalAcc_Read( ACC_REG_ADDR_XOUT_H, &x_h, sizeof(x_h));
    if(!success) return success;
	success &= HalAcc_Read( ACC_REG_ADDR_XOUT_L, &x_l, sizeof(x_l));
    if(!success) return success;
	success &= HalAcc_Read( ACC_REG_ADDR_YOUT_H, &y_h, sizeof(y_h));
    if(!success) return success;
	success &= HalAcc_Read( ACC_REG_ADDR_YOUT_L, &y_l, sizeof(y_l));
    if(!success) return success;
	success &= HalAcc_Read( ACC_REG_ADDR_ZOUT_H, &z_h, sizeof(z_h));
    if(!success) return success;
	success &= HalAcc_Read( ACC_REG_ADDR_ZOUT_L, &z_l, sizeof(z_l));
    if(!success) return success;

	acc_xh = x_h;
	acc_yh = y_h;
	acc_zh = z_h;
	
	if (success)
	{
		pData[0] = (int16_t)(((int8_t)x_h<<0x08|x_l)>>0x04);
		pData[1] = (int16_t)(((int8_t)y_h<<0x08|y_l)>>0x04);
		pData[2] = (int16_t)(((int8_t)z_h<<0x08|z_l)>>0x04);
	}
	
	return success;
}

/****************************************************************
** Function name:			HalAcc_GetData
** Descriptions:            
** input parameters:
** output parameters:
** Returned value:
** Created by:				WJ              
** Created Date:           	2016-09-26
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void HalAcc_GetData(uint8_t *data)
{
	data[0] = acc_xh;
	data[1] = acc_yh;
	data[2] = acc_zh;
    
}

/****************************************************************
** Function name:			HalAcc_PinInit
** Descriptions:            
** input parameters:
** output parameters:
** Returned value:          WX
** Created by:				2016.11.17            
** Created Date:           	
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void HalAcc_PinInit(void)
{
    nrf_gpio_cfg_input(HAL_ACC_INT, NRF_GPIO_PIN_NOPULL);
}

/****************************************************************
** Function name:           HalAcc_Address_Init
** Descriptions:			init acc iic address
** input parameters:
** output parameters:
** Returned value:
** Created by:              WJ
** Created Date:            2016-05-19
**---------------------------------------------------------------
** Modified by:
** Modified date:
**---------------------------------------------------------------
****************************************************************/
void HalAcc_Address_Init(uint8_t address)
{
//	acc_address = (uint8_t)(address << 1);	//Using simulating IIC, address need to shift 
    acc_address = (uint8_t)(address);       //Using hardware IIC, hardware address does not need to shift
}

/****************************************************************
** Function name:           HalAccDelay
** Descriptions:			��ʱn����
** input parameters:
** output parameters:
** Returned value:
** Created by:              WJ
** Created Date:            2016-05-19
**---------------------------------------------------------------
** Modified by:
** Modified date:
**---------------------------------------------------------------
****************************************************************/
static void HalAccDelay( uint32_t n)
{
	delay_ms(n);
}

/****************************************************************
** Function name:           HalAcc_Configuration
** Descriptions:			acc configuration
** input parameters:
** output parameters:
** Returned value:
** Created by:              WX
** Created Date:            2016-11-18
**---------------------------------------------------------------
** Modified by:
** Modified date:
**---------------------------------------------------------------
****************************************************************/
uint8_t test_data = 0x00;
void HalAcc_Configuration(void)
{
	uint8_t tData = 0x00;
	uint8_t ret = 1;

    HalAccDelay(100);
    
    //test  ACC_REG_ADDR_WHO_AM_I    
    HalAcc_Read( ACC_REG_ADDR_WHO_AM_I, &test_data, sizeof(test_data));

	/*��ʼ�����ٶȼ�Ϊ����ģʽ*/
	tData = 0x00;
	ret &= HalAcc_Write(ACC_REG_ADDR_CTRL_REG1, &tData, 1);     //0x1B ;PC1Ϊ0	
	HalAccDelay(30);

	tData = 0x02;
	//��/д���ƼĴ��������ü��ٶ����
	ret &= HalAcc_Write(ACC_REG_ADDR_DATA_CTRL_REG, &tData, 1); //0x21; 50Hz
    
	//���ÿ��ƼĴ���1  
    //8bit  ��������
	tData = 0x12;//+-8g//0x02;                                              
	ret &= HalAcc_Write(ACC_REG_ADDR_CTRL_REG1, &tData, 1);     //0x1B ; PC1Ϊ1

	//�رտ��ƼĴ���2��ع���
	tData	=	0x06;                                           
	//OWUFA��OWUFB OWUFC��������������ʺ�������,��3λ//����50hz
	ret &= HalAcc_Write(ACC_REG_ADDR_CTRL_REG2, &tData,1);      //0x1D ; 50Hz	

    //����Ĵ���������ͼ�ⷽ���˶����ܵ���һ���ж�,
	//��6λ����ʾ��������0 = disabled, 1 = enabled
	tData	=	0x3F;//0x7F;//0x00;                                    //��������
	ret &= HalAcc_Write(ACC_REG_ADDR_INT_CTRL_REG2, &tData,1);//0x1F

	tData	=	0x05;
	//����Ĵ�������ʱ���˶������ж�����֮ǰ�������
	ret &= HalAcc_Write(ACC_REG_ADDR_WUF_TIMER, &tData,1);//0x29	

    //WJ.20180830.���󴥷���ֵ0x01->0x02
	tData	=	0x02;//0x01;
	//����Ĵ������õ��ż�����(�˶����)�ж�����
	//XTJ2�����������������ֵ����Ϊ��Ӧ��0.5 g���ٶȵı仯��
	ret &= HalAcc_Write(ACC_REG_ADDR_THREHOLD, &tData,1);//0x6A

	tData	=	0x30;
	// ����Ĵ������������жϵĹܽ�(7)��
	// 6λ0:disabled;
	// 5λ0/1: ��ƽ��/����Ч
	// 4λ: �жϳ���ʱ�䣬0: �����1: ����0.03 -0.05ms
	ret &= HalAcc_Write(ACC_REG_ADDR_INT_CTRL_REG1, &tData,1);//0x1e	

	//���ü��ٶȼ�Ϊ����ģʽ
	tData = 0x92;//0x82;
	ret &= HalAcc_Write(ACC_REG_ADDR_CTRL_REG1, &tData, 1);//0x1B ;PC1Ϊ1
	
	HalAccDelay(30);

    //����жϱ�־
	ret &= HalAcc_Read( ACC_REG_ADDR_INT_REL, &tData,1);//0x1A ; clean interrupt
}

/****************************************************************
** Function name:			HalAccSelect
** Descriptions:            
** input parameters:
** output parameters:
** Returned value:
** Created by:				WX              
** Created Date:           	2016-11-18
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void HalAcc_Init(void)
{
    //Set up I2C that is used to communicate with SHT21
    HalAcc_Address_Init(HAL_KXTI9_I2C_ADDRESS);    //��ʼ��IIC��ַ 

    //Init I2C
#ifdef ACC_USE_HARDWARE_IIC                        //ʹ��Ӳ��IIC
    Bsp_IIC0Init();
#else
    I2c_Init();
#endif  
 
    //HalAcc_PinInit();                              //��ʼ������pin��   
    HalAcc_Configuration();	                       //��ʼ��Acc����
    HalAcc_PinInterruptInit();
}

/****************************************************************
** Function name:           HalAcc_PinInterruptHander
** Descriptions:            �ܽ��ж���Ӧ����
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2018-08-14
**----------------------------------------------------------------
*****************************************************************/ 
static void HalAcc_PinInterruptHander(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action)
{
    //�ر�ʹ��
    nrf_drv_gpiote_in_uninit(HAL_ACC_INT);   
    //����ж�
    HalAcc_CleanInterrupt();
    
    //���ùܽ�Ϊ����״̬
    HalAcc_PinInit();
    //������ʱ������ʼ����
    Acc_TimerStart();
}

/****************************************************************
** Function name:           HalAcc_PinInterruptInit
** Descriptions:            Ӳ���ж�����
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2018-08-14
**----------------------------------------------------------------
*****************************************************************/ 
void HalAcc_PinInterruptInit(void)
{
    uint32_t err_code = NRF_SUCCESS;
    
    // GPIOTE����ע��
    if(!nrf_drv_gpiote_is_init())
    {
        err_code = nrf_drv_gpiote_init();    
        APP_ERROR_CHECK(err_code);
    }
    
    nrf_drv_gpiote_in_config_t in_config = GPIOTE_CONFIG_IN_SENSE_LOTOHI(false);    // low to high
    in_config.pull = NRF_GPIO_PIN_NOPULL;            //PULL UP

    nrf_drv_gpiote_in_uninit(HAL_ACC_INT);

    err_code = nrf_drv_gpiote_in_init(HAL_ACC_INT, &in_config, HalAcc_PinInterruptHander);
    APP_ERROR_CHECK(err_code);
    
    nrf_drv_gpiote_in_event_enable(HAL_ACC_INT, true);    
}

/****************************************************************
** Function name:           HalAcc_PinHanderUnInit
** Descriptions:            Ӳ���ж������ͷ�
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2018-08-14
**----------------------------------------------------------------
*****************************************************************/ 
void HalAcc_PinHanderUnInit(void)
{
    nrf_drv_gpiote_in_uninit(HAL_ACC_INT);  
    
}
