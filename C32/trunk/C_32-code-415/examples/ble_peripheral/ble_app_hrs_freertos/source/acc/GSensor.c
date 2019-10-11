/***************************Copyright(c)***********************************
**                       Veepoo Tech
**                  COPYRIGHT (C) 2016-2019
**-----------File Info-----------------------------------------------------
** File        name:     Gsensor.c
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
#include "app_type.h"
#include "GSensor.h"
#include "board.h"
#include "nrf_delay.h"
#include "bsp_spi.h"
#include "app_error.h" 
#include "nrf_drv_gpiote.h"
#include "hal_acc_calculate.h"

//define
#define GSENSOR_CS_HIGH        Bps_Spi0SelectGsensor(0)
#define GSENSOR_CS_LOW         Bps_Spi0SelectGsensor(1)

#define GSENSOR_INIT_CNTL1_VALUE        0x92    //启动加速的控制KX023_REG_CNTL1的值

//variable
//static uint8_t GSensor_on_flag = 1;//默认开启
static uint8_t fifostartflag = 0;

//Function Declaration
void Gsensor_Int1handler(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action);
void Gsensor_Int2handler(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action);
void GSensor_CS_High(void);
void Gsensor_CsLow(void);
void Gsensor_ClearInterruptFlag(void);
uint8_t Gsensor_WriteOneReg(uint8_t *wbuf, uint16_t len);
void Gsensor_FifoEnablePara(void);
void Gsensor_FifoDisablePara(void);
void GSensor_ClearSampleBuffer(void);

/****************************************************************
** Function name:           GSensor_IntInit
** Descriptions:            引脚初始化
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WX             
** Created Date:            2016.11.11          
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void GSensor_IntInit(void)
{
    GSENSOR_CS_HIGH;//high

    nrf_gpio_cfg_input(GSENSOR_INIT1, NRF_GPIO_PIN_NOPULL);
//    nrf_gpio_cfg_input(GSENSOR_INIT2, NRF_GPIO_PIN_NOPULL);
    PIN_HIGH_RESISTANCE(GSENSOR_INIT2);
}

/****************************************************************
** Function name:           Gsensor_ClearInterruptFlag
** Descriptions:            清除中断
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WX             
** Created Date:            2016.11.11          
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Gsensor_ClearInterruptFlag(void)
{
    //读取INT_REL寄存器
    uint8_t data[2] = {0};
    
    data[0] = KX023_REG_INT_REL;                //清除中断标志
    Gsensor_WriteReadReg(data, 1, &data[1], 1);
}

/****************************************************************
** Function name:           GSensor_ClearSampleBuffer
** Descriptions:            清除缓存buf
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WX             
** Created Date:            2016.11.11          
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void GSensor_ClearSampleBuffer(void)
{
    uint8_t data[2] = {0};
    data[0] = KX023_REG_BUF_CLEAR;
    data[1] = 0xE0;                                 //清除buf 此处0xE0为随机值
    Gsensor_WriteOneReg(data, 2); 
}

/****************************************************************
** Function name:           Gsensor_GetBufferCurrentByte
** Descriptions:            获取当前buffer有多少字节
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WX             
** Created Date:            2016.11.11          
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/
uint8_t Gsensor_GetBufferCurrentByte(void)
{
    uint8_t temp = 0;
    uint8_t count = 0;
    
    //获取当前buffer缓存器有多少个byte
    temp = KX023_REG_BUF_STATUS1;
    Gsensor_WriteReadReg(&temp, 1, &count, 1);
    
    return count;
}

/****************************************************************
** Function name:           Gsensor_ReadoneSample
** Descriptions:            获取采样数据
** input parameters:        len:获取数据长度
** output parameters:       data:输出数组；
** Returned value:          无
** Created by:              WX             
** Created Date:            2016.11.11          
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
uint8_t Gsensor_ReadoneSample(int16_t *data, uint16_t len)
{
    uint8_t temp = 0;
    uint8_t ret = 1;
   
    //读取buffer数据  读一个样本
    temp = KX023_REG_BUF_READ;
    ret = Gsensor_WriteReadReg(&temp, 1, (uint8_t *)data, len);
    
    return ret;
}

/****************************************************************
** Function name:           Gsensor_Int1handler
** Descriptions:            中断1回调函数
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WX             
** Created Date:            2016.11.11          
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Gsensor_Int1handler(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action)
{
    if(fifostartflag == 0)
    {
        //使能FIFO
        Gsensor_FifoEnablePara();
        fifostartflag = 1;
    }
    //清中断标志
    Gsensor_ClearInterruptFlag();
}

/****************************************************************
** Function name:           Gsensor_Int2handler
** Descriptions:            中断2回调函数
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WX             
** Created Date:            2016.11.11          
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/
void Gsensor_Int2handler(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action)
{
     //清中断标志
    Gsensor_ClearInterruptFlag();
    
    //调用数据计算函数
    Acc_Calculate();    
    if(fifostartflag == 1)
    {
        Gsensor_FifoDisablePara();
        //清除FIFO
        Gsensor_CleanFifo();
    }
}

/****************************************************************
** Function name:           Gsensor_WriteOneReg
** Descriptions:            写寄存器
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WX             
** Created Date:            2016.11.11          
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
uint8_t Gsensor_WriteOneReg(uint8_t *wbuf, uint16_t len)
{
    uint8_t ret = 0;
    GSENSOR_CS_LOW;
    ret = Bps_Spi0Write(wbuf,len);
    GSENSOR_CS_HIGH;
    
    return ret;
}

/****************************************************************
** Function name:           Gsensor_ReadOneReg
** Descriptions:            
** input parameters:
** output parameters:
** Returned value:
** Created by:              WX             
** Created Date:            2016.11.11          
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
uint8_t Gsensor_ReadOneReg(uint8_t *wbuf, uint16_t len)
{
    uint8_t ret = 0;
    
    GSENSOR_CS_LOW;
    nrf_delay_us(50);
    ret = Bps_Spi0Read(wbuf, len);
    nrf_delay_us(50);
    GSENSOR_CS_HIGH;
    
    return ret;
}

/****************************************************************
** Function name:           Gsensor_WriteReadReg
** Descriptions:            写读
** input parameters:
** output parameters:
** Returned value:
** Created by:              WX             
** Created Date:            2016.11.11          
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
uint8_t Gsensor_WriteReadReg(uint8_t *input, uint8_t input_len, uint8_t *output, uint8_t output_len)
{
    uint8_t ret = 0;     
    static uint8_t readaddr = 0;
    readaddr = *input | 0x80;
    
    GSENSOR_CS_LOW;
    ret = Bps_Spi0Write(&readaddr, input_len);
    ret &= Bps_Spi0Read(output, output_len);
    GSENSOR_CS_HIGH;

    return ret;
}

/****************************************************************
** Function name:           Gsensor_ReadBufByLen
** Descriptions:            读取数据
** input parameters:        len:读取长度
** output parameters:       wbuf：数据   
** Returned value:          0：读取失败，1：读取成功
** Created by:              WX             
** Created Date:            2016.11.11          
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
uint16_t Gsensor_ReadBufByLen(uint8_t *wbuf, uint16_t len)
{
    uint8_t ret = 0;
    
    GSENSOR_CS_LOW;
    ret = Bps_Spi0Read(wbuf,len);
    GSENSOR_CS_HIGH;
    
    return ret;
}

/****************************************************************
** Function name:           Gsensor_SoftReset
** Descriptions:            软件复位
** input parameters:        无
** output parameters:       无
** Returned value:          0：复位失败；1复位成功
** Created by:              YX             
** Created Date:            2017.06.26
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
static uint8_t Gsensor_SoftReset(void)
{
    uint8_t data[3] = {0};
    
    data[0] = 0x7F;
    data[1] = 0x00;
    Gsensor_WriteOneReg(data, 2);
    
    data[0] = KX023_REG_CNTL2;
    data[1] = 0x00;
    Gsensor_WriteOneReg(data, 2);
    
    data[0] = KX023_REG_CNTL2;
    data[1] = 0x80;
    Gsensor_WriteOneReg(data, 2);         //RAM重新启动
    
    //延时2ms
    nrf_delay_ms(4);
    
    data[0] = KX023_REG_WHO;                      //
    Gsensor_WriteReadReg(data, 1, &data[2], 1);   //读取控制器寄存器1的值并保存
    if(0x15 == data[2])//KX023 0X15; KX022 0X14
    {
        data[0] = KX023_REG_COTR;                      //
        Gsensor_WriteReadReg(data, 1, &data[2], 1);   //读取控制器寄存器1的值并保存
        if(0x55 == data[2])
        {
            return 1;
        }
    }
    return 0;

}

/****************************************************************
** Function name:           Gsensor_Configure
** Descriptions:            配置
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WX             
** Created Date:            2016.11.11          
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Gsensor_Configure(void)
{
    static uint8_t data[2] = {0};
    static uint8_t data1 = 0;
    uint8_t cnt = 0;

    while(cnt++ < 5)
    {
        //acc软件复位
        if(Gsensor_SoftReset())
        {
            break;
        }
    }
    
    data[0] = KX023_REG_CNTL1;
    data[1] = 0x00;                             //关闭加速度与唤醒失能
    Gsensor_WriteOneReg(data, 2);  
    
    /*Read WHO_AM_I Register*/
    data[0] = KX023_REG_WHO; 
    Gsensor_WriteReadReg(&data[0], 1, &data1, 1);

    //分辨率
    data[0] = KX023_REG_CNTL1;                  //控制寄存器1  
    data[1] = 0x12;//0x52;//                    //0001 0010:加速度范围 +/-8g
    Gsensor_WriteOneReg(data,2);                //8bit有效数据(高分辨率)
   
    //输出频率
    data[0] = KX023_REG_ODCNTL;                 //数据控制寄存器(数据输出率),低通滤波
    data[1] = 0x02;                             //配置数据输出率和过滤器设置  
    Gsensor_WriteOneReg(data,2);                //输出率50hz    
    
    //唤醒输出频率
    data[0] = KX023_REG_CNTL3;                  //控制寄存器3
    data[1] = 0x9F;//0x9E;//0x02;//0x9F;//      //100hz
    Gsensor_WriteOneReg(data,2);                //唤醒输出率50hz  
    
    //运动唤醒配置
    data[0] = KX023_REG_WUFC;                   //唤醒定时器中断,WUFC (counts) = Desired Delay Time (sec) x OWUF (Hz)
    data[1] = 0x19;//0x0A;//5;  //0x05;//       // 延时0.1s触发，唤醒阀值持续时间后触发中断
    Gsensor_WriteOneReg(data,2);                //0.1*50 = 5//0.1*100hz=10 counts
 
     //运动唤醒轴
    data[0] = KX023_REG_INC2;                   
    data[1] = 0x7F;                   			
    Gsensor_WriteOneReg(data,2);              
    
    data[0] = KX023_REG_ATH;                    //唤醒阈值n*16
    data[1] = 0x01;//0x03;//8;//0x01;           //0.5*16=8，0.5g唤醒
    Gsensor_WriteOneReg(data,2);              

    data[0] = KX023_REG_INC1;                   //中断控制寄存器1
    data[1] = 0x30;//0x38;                      //使能物理中断引脚1，设置引脚中断极性为高，
    Gsensor_WriteOneReg(data,2);                //产生一个高电平直到读取INT_REL清除中断标志

    data[0] = KX023_REG_INC4;                   //中断控制寄存器4
    data[1] = 0x02;                             //唤醒中断使能
    Gsensor_WriteOneReg(data,2);                //

    //单次采样次数均值配置
    data[0] = KX023_REG_LP_CNTL;                //低功耗控制寄存器
    data[1] = 0x00;//0x20;                      //不启用
    Gsensor_WriteOneReg(data,2);
    
    //缓存buf配置
    data[0] = KX023_REG_BUF_CNTL1;
    data[1] = 0x28;                             //采样阈值40(最大41)
    Gsensor_WriteOneReg(data, 2);  

    data[0] = KX023_REG_BUF_CNTL2;              //WJ.20170606.修改缓存方式
    data[1] = 0xC1;//0xC0;//1100 0011;          //使能取样buf，16bit数据缓存
    Gsensor_WriteOneReg(data, 2);               //    
    
    data[0] = KX023_REG_BUF_CLEAR;
    data[1] = 0xE0;                             //写入随机值清除buf
    Gsensor_WriteOneReg(data, 2); 

    //清除中断
    data[0] = KX023_REG_INT_REL;                //清除中断标志
    Gsensor_WriteReadReg(data, 1, &data[1], 1);	
   
   //使能加速度
    data[0] = KX023_REG_CNTL1;                  //开启加速度
    data[1] = GSENSOR_INIT_CNTL1_VALUE;//0x92;  //1001 0010：4、5bit为2/4/8g控制
    Gsensor_WriteOneReg(data,2);       
}

/****************************************************************
** Function name:           Gsensor_AccInit
** Descriptions:            ACC初始化
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WX             
** Created Date:            2016.11.30        
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Gsensor_AccInit(void)
{
    //引脚初始化
    GSensor_IntInit();
    //acc初始化
    Gsensor_Configure();
}

/****************************************************************
** Function name:           Gsensor_EnableMotion
** Descriptions:            运动使能
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WX             
** Created Date:            2016.11.11          
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Gsensor_EnableMotion(void)
{
    uint8_t data[3] = {0};
//    GSensor_on_flag = 1;
    
    data[0] = KX023_REG_CNTL1;                      //
    Gsensor_WriteReadReg(data, 1, &data[2], 1);   //读取控制器寄存器1的值并保存
    
    data[0] = KX023_REG_CNTL1;              
    data[1] = 0x00;                                 //停止加速度
    Gsensor_WriteOneReg(data, 2);
    
    data[0] = KX023_REG_INC4;                       //中断控制寄存器4
    data[1] = 0x02;                                 //设置为唤醒模式
    Gsensor_WriteOneReg(data, 2);
    
    data[0] = KX023_REG_CNTL1;  
    data[1] = data[2] | 0x02;                       //重新开启加速度并唤醒使能
    Gsensor_WriteOneReg(data, 2);
}

/****************************************************************
** Function name:           Gsensor_DisableMotion
** Descriptions:            运动失能
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WX             
** Created Date:            2016.11.11          
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Gsensor_DisableMotion(void)
{
    uint8_t data[3] = {0};
//    GSensor_on_flag = 0;
    
    data[0] = KX023_REG_CNTL1;
    Gsensor_WriteReadReg(data, 1, &data[2], 1);   //读取控制寄存器1值并保存
    
    data[0] = KX023_REG_CNTL1;                      
    data[1] = 0x00;                                 //停止加速度
    Gsensor_WriteOneReg(data, 2);
    
    data[0] = KX023_REG_INC4;
    data[1] = 0x00;                                 //唤醒失能
    Gsensor_WriteOneReg(data, 2);
    
    data[0] = KX023_REG_CNTL1;
    data[1] = data[2] & 0xFD;                       //加速度开启
    Gsensor_WriteOneReg(data, 2);
}

/****************************************************************
** Function name:           Gsensor_FifoEnablePara
** Descriptions:            FIFO参数使能设置
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WX             
** Created Date:            2016.11.11          
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/
void Gsensor_FifoEnablePara(void)
{
    uint8_t data[3] = {0};
    
    data[0] = KX023_REG_CNTL1;                      //读取控制寄存器1数据并保存
    Gsensor_WriteReadReg(data, 1, &data[2], 1);
    
    data[0] = KX023_REG_CNTL1;
    data[1] = 0x00;                                 //关闭加速度与唤醒失能
    Gsensor_WriteOneReg(data, 2);
  
    //缓存buf配置
    data[0] = KX023_REG_BUF_CNTL1;
    data[1] = 0x28;                             //采样阈值40(最大41)
    Gsensor_WriteOneReg(data, 2);  

    data[0] = KX023_REG_BUF_CLEAR;
    data[1] = 0xE0;                             //写入随机值清除buf
    Gsensor_WriteOneReg(data, 2); 

    data[0] = KX023_REG_BUF_CNTL2;
    data[1] = 0xC0;//1100 0011;                 //使能取样buf，16bit数据缓存
    Gsensor_WriteOneReg(data, 2);             //FIFO    
      
    data[0] = KX023_REG_CNTL1;
    data[1] = data[2] | 0x80;                       //开启加速度
    Gsensor_WriteOneReg(data, 2);
}

/****************************************************************
** Function name:           Gsensor_CleanFifo
** Descriptions:            清除缓存buf
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WX             
** Created Date:            2016.11.11          
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/
void Gsensor_CleanFifo(void)
{
    uint8_t data[2] = {0};
    
    data[0] = KX023_REG_BUF_CLEAR;
    data[1] = 0xE0;                                 //清除buf
    Gsensor_WriteOneReg(data, 2); 
}

/****************************************************************
** Function name:           Gsensor_FifoDisablePara
** Descriptions:            FIFO参数失能设置
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WX             
** Created Date:            2016.11.11          
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/
void Gsensor_FifoDisablePara(void)
{
    uint8_t data[3] = {0};
    
    data[0] = KX023_REG_CNTL1;                      //读取控制寄存器1数据并保存
    Gsensor_WriteReadReg(data, 1, &data[2], 1);
    
    data[0] = KX023_REG_CNTL1;
    data[1] = 0x00;                                 //关闭加速度与唤醒失能
    Gsensor_WriteOneReg(data, 2);
    
    data[0] = KX023_REG_BUF_CNTL1;
    data[1] = 0x00;                                 //清除采样阈值
    Gsensor_WriteOneReg(data, 2);  
    
    data[0] = KX023_REG_BUF_CNTL2;
    data[1] = 0x00;                                 //关闭取样
    Gsensor_WriteOneReg(data, 2);

    data[0] = KX023_REG_CNTL1;
    data[1] = data[2] | 0x80;//0x8A;                //开启加速度  但不会触发中断
    Gsensor_WriteOneReg(data, 2);
}

/****************************************************************
** Function name:           Gsensor_Init1State
** Descriptions:            获取中断管脚1的状态
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WX             
** Created Date:            2016.11.11          
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/
uint8_t Gsensor_Init1State(void)
{
    return nrf_gpio_pin_read(GSENSOR_INIT1);
}

/****************************************************************
** Function name:           Gsensor_Sleep
** Descriptions:            休眠
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WX             
** Created Date:            2016.11.11          
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/
void Gsensor_Sleep(void)
{
    uint8_t data[3] = {0,0,};
    
    data[0] = KX023_REG_CNTL1;
    data[1] = 0x00;                                 //关闭加速度与唤醒失能
    Gsensor_WriteOneReg(data, 2);   
}

/****************************************************************
** Function name:           Gsensor_Check
** Descriptions:            自检
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WX             
** Created Date:            2016.11.11          
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/
uint8_t Gsensor_Check(void)
{        
    uint8_t data[3] = {0,};
    
    data[0] = KX023_REG_CNTL1;                      //读取控制寄存器1数据并保存
    Gsensor_WriteReadReg(data, 1, &data[2], 1);    
    
    if( data[2] != GSENSOR_INIT_CNTL1_VALUE)  //自检不通过需要重新初始化
    {
        return 0;
    }
    
    return 1;
}

/****************************************************************
** Function name:           Gsensor_ReadConfig
** Descriptions:            读取寄存器值，用于测试
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WX             
** Created Date:            2016.11.11          
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/
void Gsensor_ReadConfig(uint8_t *r_data)
{
    uint8_t data[2] = {0};
    uint8_t data1 = 0;
      
    /*Read WHO_AM_I Register*/
    data[0] = KX023_REG_WHO; 
    Gsensor_WriteReadReg(&data[0], 1, &data1, 1);    
    *r_data = data1;
    r_data++;

    //分辨率
    data[0] = KX023_REG_CNTL1; 
    Gsensor_WriteReadReg(&data[0], 1, &data1, 1);    
    *r_data = data1;
    r_data++;
   
    //输出频率
    data[0] = KX023_REG_ODCNTL; 
    Gsensor_WriteReadReg(&data[0], 1, &data1, 1);    
    *r_data = data1;
    r_data++; 
    
    //唤醒输出频率
    data[0] = KX023_REG_CNTL3; 
    Gsensor_WriteReadReg(&data[0], 1, &data1, 1);    
    *r_data = data1;
    r_data++; 
    
    //运动唤醒配置
    data[0] = KX023_REG_WUFC; 
    Gsensor_WriteReadReg(&data[0], 1, &data1, 1);    
    *r_data = data1;
    r_data++;            
    
    data[0] = KX023_REG_ATH; //唤醒阈值n*16
    Gsensor_WriteReadReg(&data[0], 1, &data1, 1);    
    *r_data = data1;
    r_data++; 
 
    data[0] = KX023_REG_INC1; //中断控制寄存器1
    Gsensor_WriteReadReg(&data[0], 1, &data1, 1);    
    *r_data = data1;
    r_data++; 
     
    data[0] = KX023_REG_INC4; //中断控制寄存器4
    Gsensor_WriteReadReg(&data[0], 1, &data1, 1);    
    *r_data = data1;
    r_data++;

    //单次采样次数均值配置
    data[0] = KX023_REG_LP_CNTL; //低功耗控制寄存器
    Gsensor_WriteReadReg(&data[0], 1, &data1, 1);    
    *r_data = data1;
    r_data++;      
       
    //缓存buf配置
    data[0] = KX023_REG_BUF_CNTL1;
    Gsensor_WriteReadReg(&data[0], 1, &data1, 1);  
    *r_data = data1;
    r_data++;  
    
    data[0] = KX023_REG_BUF_CNTL2;
    Gsensor_WriteReadReg(&data[0], 1, &data1, 1);  
    *r_data = data1;
    r_data++;  
        
    //中断管脚的值
    data[0] = KX023_REG_INS2; //低功耗控制寄存器
    Gsensor_WriteReadReg(&data[0], 1, &data1, 1);    
    *r_data = data1;
    r_data++;       
           
    //中断管脚的值
    data[0] = KX023_REG_INT_REL; //低功耗控制寄存器
    Gsensor_WriteReadReg(&data[0], 1, &data1, 1);    
    *r_data = data1;
    r_data++;  
    
}
