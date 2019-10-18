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
PROVIDED 揂S IS?WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED,
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



#ifdef ACC_USE_HARDWARE_IIC        //使用硬件IIC
    #include "bsp_iic.h"
#else
    #include "I2C.h"
#endif
/*********************************************************************
* 宏定义
*/
#define delay_ms(i)    nrf_delay_ms(i)      //平台相关延时

/*********************************************************************
* 全局变量
*/
static uint8_t acc_address = 0;     //iic address, it's had shifted

static uint8_t  acc_xh = 0;
static uint8_t  acc_yh = 0;
static uint8_t  acc_zh = 0;

static uint8_t acc_gpiote_init = 0;  //hal_acc_int初始化标志，1：已经初始化  0：没有初始化

/*********************************************************************
* 本地函数声明
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
** Descriptions:            采样中断清除
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
	
#ifdef ACC_USE_HARDWARE_IIC        //使用硬件IIC	
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
	
#ifdef ACC_USE_HARDWARE_IIC        //使用硬件IIC	
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
** Descriptions:			延时n毫秒
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




//复位
/****************************************************************
** Function name:           HalAcc_ResetConfiguration
** Descriptions:			acc reset
** input parameters:
** output parameters:
** Returned value:
** Created by:              ZZZ
** Created Date:            2019-07-04
**---------------------------------------------------------------
** Modified by:
** Modified date:
**---------------------------------------------------------------
****************************************************************/
void HalAcc_ResetConfiguration(void)
{
    uint8_t read_data = 0x00;
	uint8_t tData = 0x00;
	static uint8_t ret = 1;

    for(uint8_t i = 0; i < 3; i++)
    {    
        /*I2C地址获取，一般可以忽略*/
        tData = 0x00;
        ret &= HalAcc_Write(ACC_REG_ADDR_BUF_READ, &tData, 1); 	   

        tData = 0x00;
        ret &= HalAcc_Write(ACC_REG_ADDR_CTRL_REG2, &tData, 1); 	  
        
        tData = 0x80;
        ret &= HalAcc_Write(ACC_REG_ADDR_CTRL_REG2, &tData, 1);  //设备重启
        
        HalAccDelay(4); //延时2毫秒
		HalAcc_Address_Init(HAL_KXTI9_I2C_ADDRESS );    //I2C Primary Address
        //  ACC_REG_ADDR_WHO_AM_I    
        HalAcc_Read( ACC_REG_ADDR_WHO_AM_I, &read_data, sizeof(read_data));   
        if(read_data != 0x35 && read_data != 0x09)
        {   
			// 设备软件复位失败 
            HalAcc_Address_Init(0x0D);  //I2C Flipped Address
			ret &= HalAcc_Write(ACC_REG_ADDR_BUF_READ, &tData, 1); 
			continue;
        }
        
        //  ACC_REG_ADDR_DCST_RESP    
        HalAcc_Read( ACC_REG_ADDR_DCST_RESP, &read_data, sizeof(read_data));  
        if(read_data != 0x55 )
        {      
		   HalAcc_Address_Init(0x0D);  //I2C Flipped Address
		   ret &= HalAcc_Write(ACC_REG_ADDR_BUF_READ, &tData, 1); 
		   continue;   
        }
        //确认复位成功，则退出循环
        break; 
    }
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
void HalAcc_Configuration(void)
{
    uint8_t  ChipID_data = 0x00;
	uint8_t tData = 0x00;
	uint8_t ret = 1;

    HalAccDelay(100);
    
    //ACC软件复位处理
    HalAcc_ResetConfiguration();
    
    //确认IIC的设备地址为0x0F
    HalAcc_Address_Init(HAL_KXTI9_I2C_ADDRESS );    //I2C Primary Address
    
    //ACC_REG_ADDR_WHO_AM_I    读取G-sensor的设备ID
    HalAcc_Read( ACC_REG_ADDR_WHO_AM_I, &ChipID_data, sizeof(ChipID_data));
    
    //1057的芯片ID为0x35,1009的芯片ID为0x09
    if(ChipID_data == 0x35)
    {
        /*初始化加速度计为备用模式*/
        tData = 0x00;
        ret &= HalAcc_Write(ACC_REG_ADDR_CTRL_REG1, &tData, 1);     //0x1B ;PC1为0	
        HalAccDelay(30);

        tData = 0x02;
        //读/写控制寄存器的配置加速度输出
        ret &= HalAcc_Write(ACC_REG_ADDR_DATA_CTRL_REG, &tData, 1); //0x21; 50Hz
        
        //设置控制寄存器1  
        //8bit  唤醒启动
        tData = 0x12;//+-8g//0x02;                                              
        ret &= HalAcc_Write(ACC_REG_ADDR_CTRL_REG1, &tData, 1);     //0x1B ; PC1为1

        //关闭控制寄存器2相关功能
        tData	=	0x06;                                           
        //OWUFA、OWUFB OWUFC集的输出数据速率函数醒来,低3位//唤醒50hz
        ret &= HalAcc_Write(ACC_REG_ADDR_CTRL_REG2, &tData,1);      //0x1D ; 50Hz	

        //这个寄存器控制轴和检测方向运动可能导致一个中断,
        //低6位，表示六个方向；0 = disabled, 1 = enabled
        tData	=	0x3F;//0x7F;//0x00;                                    //开启所有
        ret &= HalAcc_Write(ACC_REG_ADDR_INT_CTRL_REG2, &tData,1);//0x1F

        tData	=	0x05;
        //这个寄存器设置时间运动唤醒中断设置之前必须存在
        ret &= HalAcc_Write(ACC_REG_ADDR_WUF_TIMER, &tData,1);//0x29	

        //WJ.20180830.增大触发阀值0x01->0x02
        tData	=	0x02;//0x01;
        //这个寄存器设置的门槛唤醒(运动检测)中断设置
        //XTJ2将船　　工厂用这个值设置为对应于0.5 g加速度的变化。
        ret &= HalAcc_Write(ACC_REG_ADDR_THREHOLD, &tData,1);//0x6A

        tData	=	0x30;
        // 这个寄存器控制物理中断的管脚(7)。
        // 6位0:disabled;
        // 5位0/1: 电平低/高有效
        // 4位: 中断持续时间，0: 读清除1: 持续0.03 -0.05ms
        ret &= HalAcc_Write(ACC_REG_ADDR_INT_CTRL_REG1, &tData,1);//0x1e	

        //设置加速度计为操作模式
        tData = 0x92;//0x82;
        ret &= HalAcc_Write(ACC_REG_ADDR_CTRL_REG1, &tData, 1);//0x1B ;PC1为1
        
        HalAccDelay(30);

        //清除中断标志
        ret &= HalAcc_Read( ACC_REG_ADDR_INT_REL, &tData,1);//0x1A ; clean interrupt
    }
    else if(ChipID_data == 0x09)
    {
        /*初始化加速度计为备用模式*/
        tData = 0x00;
        ret &= HalAcc_Write(ACC_REG_ADDR_CTRL_REG1, &tData, 1);     //0x1B ;PC1为0	
        HalAccDelay(30);

        tData = 0x02;
        //读/写控制寄存器的配置加速度输出
        ret &= HalAcc_Write(ACC_REG_ADDR_DATA_CTRL_REG, &tData, 1); //0x21; 50Hz
        
        //设置控制寄存器1  
        //8bit  唤醒启动
        tData = 0x12;                                              
        ret &= HalAcc_Write(ACC_REG_ADDR_CTRL_REG1, &tData, 1);     //0x1B ; PC1为1

        //关闭控制寄存器2相关功能
        tData	=	0x06;                                           
        //OWUFA、OWUFB OWUFC集的输出数据速率函数醒来,低3位//唤醒50hz
        ret &= HalAcc_Write(ACC_REG_ADDR_CTRL_REG2, &tData,1);      //0x1D ; 50Hz	

        //这个寄存器控制轴和检测方向运动可能导致一个中断,
        //低6位，表示六个方向；0 = disabled, 1 = enabled
        tData	=	0x3F;//0x00;                                    //开启所有
        ret &= HalAcc_Write(ACC_REG_ADDR_INT_CTRL_REG2, &tData,1);  //0x1F

        tData	=	0x05;
        //这个寄存器设置时间运动唤醒中断设置之前必须存在
        ret &= HalAcc_Write(ACC_REG_ADDR_WUF_TIMER, &tData,1);//0x29	

        tData	=	0x01;
        //这个寄存器设置的门槛唤醒(运动检测)中断设置
        //XTJ2将船　　工厂用这个值设置为对应于0.5 g加速度的变化。
        ret &= HalAcc_Write(ACC_REG_ADDR_THREHOLD, &tData,1);//0x6A

        tData	=	0x30;
        // 这个寄存器控制物理中断的管脚(7)。
        // 6位0:disabled;
        // 5位0/1: 电平低/高有效
        // 4位: 中断持续时间，0: 读清除1: 持续0.03 -0.05ms
        ret &= HalAcc_Write(ACC_REG_ADDR_INT_CTRL_REG1, &tData,1);//0x1e	

        //设置加速度计为操作模式
        tData = 0x92;//0x82;
        ret &= HalAcc_Write(ACC_REG_ADDR_CTRL_REG1, &tData, 1);//0x1B ;PC1为1
        
        HalAccDelay(30);
        
        //清除中断标志
        ret &= HalAcc_Read( ACC_REG_ADDR_INT_REL, &tData,1);//0x1A ; clean interrupt
    }
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
    HalAcc_Address_Init(HAL_KXTI9_I2C_ADDRESS);    //初始化IIC地址 

    //Init I2C
#ifdef ACC_USE_HARDWARE_IIC                        //使用硬件IIC
    Bsp_IIC0Init();
#else
    I2c_Init();
#endif  
 
    //HalAcc_PinInit();                              //初始化唤醒pin脚   
    HalAcc_Configuration();	                       //初始化Acc配置
    HalAcc_PinInterruptInit();
}

/****************************************************************
** Function name:           HalAcc_PinInterruptHander
** Descriptions:            管脚中断响应函数
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2018-08-14
**----------------------------------------------------------------
*****************************************************************/ 
static void HalAcc_PinInterruptHander(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action)
{
    if(acc_gpiote_init == 1)
    {
        //关闭使能
        nrf_drv_gpiote_in_uninit(HAL_ACC_INT);
        acc_gpiote_init = 0;        
    }
    //清除中断
    HalAcc_CleanInterrupt();
    
    //配置管脚为输入状态
    HalAcc_PinInit();
    //开启定时器，开始采样
    Acc_TimerStart();
}

/****************************************************************
** Function name:           HalAcc_PinInterruptInit
** Descriptions:            硬件中断配置
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2018-08-14
**----------------------------------------------------------------
*****************************************************************/ 
void HalAcc_PinInterruptInit(void)
{
    uint32_t err_code = NRF_SUCCESS;
    
    // GPIOTE驱动注册
    if(!nrf_drv_gpiote_is_init())
    {
        err_code = nrf_drv_gpiote_init();    
        APP_ERROR_CHECK(err_code);
    }
    
    nrf_drv_gpiote_in_config_t in_config = GPIOTE_CONFIG_IN_SENSE_LOTOHI(false);    // low to high
    in_config.pull = NRF_GPIO_PIN_NOPULL;            //PULL UP

    if(acc_gpiote_init == 1)
    {
        nrf_drv_gpiote_in_uninit(HAL_ACC_INT);
        acc_gpiote_init = 0;
    }

    err_code = nrf_drv_gpiote_in_init(HAL_ACC_INT, &in_config, HalAcc_PinInterruptHander);
    acc_gpiote_init = 1;
    APP_ERROR_CHECK(err_code);
    
    nrf_drv_gpiote_in_event_enable(HAL_ACC_INT, true);    
}

/****************************************************************
** Function name:           HalAcc_PinHanderUnInit
** Descriptions:            硬件中断配置释放
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2018-08-14
**----------------------------------------------------------------
*****************************************************************/ 
void HalAcc_PinHanderUnInit(void)
{
    if(acc_gpiote_init == 1)
    {
        nrf_drv_gpiote_in_uninit(HAL_ACC_INT);  
        acc_gpiote_init = 0;
    }
    
}
