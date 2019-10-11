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

#define GSENSOR_INIT_CNTL1_VALUE        0x92    //�������ٵĿ���KX023_REG_CNTL1��ֵ

//variable
//static uint8_t GSensor_on_flag = 1;//Ĭ�Ͽ���
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
** Descriptions:            ���ų�ʼ��
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
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
** Descriptions:            ����ж�
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WX             
** Created Date:            2016.11.11          
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Gsensor_ClearInterruptFlag(void)
{
    //��ȡINT_REL�Ĵ���
    uint8_t data[2] = {0};
    
    data[0] = KX023_REG_INT_REL;                //����жϱ�־
    Gsensor_WriteReadReg(data, 1, &data[1], 1);
}

/****************************************************************
** Function name:           GSensor_ClearSampleBuffer
** Descriptions:            �������buf
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
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
    data[1] = 0xE0;                                 //���buf �˴�0xE0Ϊ���ֵ
    Gsensor_WriteOneReg(data, 2); 
}

/****************************************************************
** Function name:           Gsensor_GetBufferCurrentByte
** Descriptions:            ��ȡ��ǰbuffer�ж����ֽ�
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
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
    
    //��ȡ��ǰbuffer�������ж��ٸ�byte
    temp = KX023_REG_BUF_STATUS1;
    Gsensor_WriteReadReg(&temp, 1, &count, 1);
    
    return count;
}

/****************************************************************
** Function name:           Gsensor_ReadoneSample
** Descriptions:            ��ȡ��������
** input parameters:        len:��ȡ���ݳ���
** output parameters:       data:������飻
** Returned value:          ��
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
   
    //��ȡbuffer����  ��һ������
    temp = KX023_REG_BUF_READ;
    ret = Gsensor_WriteReadReg(&temp, 1, (uint8_t *)data, len);
    
    return ret;
}

/****************************************************************
** Function name:           Gsensor_Int1handler
** Descriptions:            �ж�1�ص�����
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
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
        //ʹ��FIFO
        Gsensor_FifoEnablePara();
        fifostartflag = 1;
    }
    //���жϱ�־
    Gsensor_ClearInterruptFlag();
}

/****************************************************************
** Function name:           Gsensor_Int2handler
** Descriptions:            �ж�2�ص�����
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WX             
** Created Date:            2016.11.11          
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/
void Gsensor_Int2handler(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action)
{
     //���жϱ�־
    Gsensor_ClearInterruptFlag();
    
    //�������ݼ��㺯��
    Acc_Calculate();    
    if(fifostartflag == 1)
    {
        Gsensor_FifoDisablePara();
        //���FIFO
        Gsensor_CleanFifo();
    }
}

/****************************************************************
** Function name:           Gsensor_WriteOneReg
** Descriptions:            д�Ĵ���
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
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
** Descriptions:            д��
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
** Descriptions:            ��ȡ����
** input parameters:        len:��ȡ����
** output parameters:       wbuf������   
** Returned value:          0����ȡʧ�ܣ�1����ȡ�ɹ�
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
** Descriptions:            �����λ
** input parameters:        ��
** output parameters:       ��
** Returned value:          0����λʧ�ܣ�1��λ�ɹ�
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
    Gsensor_WriteOneReg(data, 2);         //RAM��������
    
    //��ʱ2ms
    nrf_delay_ms(4);
    
    data[0] = KX023_REG_WHO;                      //
    Gsensor_WriteReadReg(data, 1, &data[2], 1);   //��ȡ�������Ĵ���1��ֵ������
    if(0x15 == data[2])//KX023 0X15; KX022 0X14
    {
        data[0] = KX023_REG_COTR;                      //
        Gsensor_WriteReadReg(data, 1, &data[2], 1);   //��ȡ�������Ĵ���1��ֵ������
        if(0x55 == data[2])
        {
            return 1;
        }
    }
    return 0;

}

/****************************************************************
** Function name:           Gsensor_Configure
** Descriptions:            ����
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
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
        //acc�����λ
        if(Gsensor_SoftReset())
        {
            break;
        }
    }
    
    data[0] = KX023_REG_CNTL1;
    data[1] = 0x00;                             //�رռ��ٶ��뻽��ʧ��
    Gsensor_WriteOneReg(data, 2);  
    
    /*Read WHO_AM_I Register*/
    data[0] = KX023_REG_WHO; 
    Gsensor_WriteReadReg(&data[0], 1, &data1, 1);

    //�ֱ���
    data[0] = KX023_REG_CNTL1;                  //���ƼĴ���1  
    data[1] = 0x12;//0x52;//                    //0001 0010:���ٶȷ�Χ +/-8g
    Gsensor_WriteOneReg(data,2);                //8bit��Ч����(�߷ֱ���)
   
    //���Ƶ��
    data[0] = KX023_REG_ODCNTL;                 //���ݿ��ƼĴ���(���������),��ͨ�˲�
    data[1] = 0x02;                             //������������ʺ͹���������  
    Gsensor_WriteOneReg(data,2);                //�����50hz    
    
    //�������Ƶ��
    data[0] = KX023_REG_CNTL3;                  //���ƼĴ���3
    data[1] = 0x9F;//0x9E;//0x02;//0x9F;//      //100hz
    Gsensor_WriteOneReg(data,2);                //���������50hz  
    
    //�˶���������
    data[0] = KX023_REG_WUFC;                   //���Ѷ�ʱ���ж�,WUFC (counts) = Desired Delay Time (sec) x OWUF (Hz)
    data[1] = 0x19;//0x0A;//5;  //0x05;//       // ��ʱ0.1s���������ѷ�ֵ����ʱ��󴥷��ж�
    Gsensor_WriteOneReg(data,2);                //0.1*50 = 5//0.1*100hz=10 counts
 
     //�˶�������
    data[0] = KX023_REG_INC2;                   
    data[1] = 0x7F;                   			
    Gsensor_WriteOneReg(data,2);              
    
    data[0] = KX023_REG_ATH;                    //������ֵn*16
    data[1] = 0x01;//0x03;//8;//0x01;           //0.5*16=8��0.5g����
    Gsensor_WriteOneReg(data,2);              

    data[0] = KX023_REG_INC1;                   //�жϿ��ƼĴ���1
    data[1] = 0x30;//0x38;                      //ʹ�������ж�����1�����������жϼ���Ϊ�ߣ�
    Gsensor_WriteOneReg(data,2);                //����һ���ߵ�ƽֱ����ȡINT_REL����жϱ�־

    data[0] = KX023_REG_INC4;                   //�жϿ��ƼĴ���4
    data[1] = 0x02;                             //�����ж�ʹ��
    Gsensor_WriteOneReg(data,2);                //

    //���β���������ֵ����
    data[0] = KX023_REG_LP_CNTL;                //�͹��Ŀ��ƼĴ���
    data[1] = 0x00;//0x20;                      //������
    Gsensor_WriteOneReg(data,2);
    
    //����buf����
    data[0] = KX023_REG_BUF_CNTL1;
    data[1] = 0x28;                             //������ֵ40(���41)
    Gsensor_WriteOneReg(data, 2);  

    data[0] = KX023_REG_BUF_CNTL2;              //WJ.20170606.�޸Ļ��淽ʽ
    data[1] = 0xC1;//0xC0;//1100 0011;          //ʹ��ȡ��buf��16bit���ݻ���
    Gsensor_WriteOneReg(data, 2);               //    
    
    data[0] = KX023_REG_BUF_CLEAR;
    data[1] = 0xE0;                             //д�����ֵ���buf
    Gsensor_WriteOneReg(data, 2); 

    //����ж�
    data[0] = KX023_REG_INT_REL;                //����жϱ�־
    Gsensor_WriteReadReg(data, 1, &data[1], 1);	
   
   //ʹ�ܼ��ٶ�
    data[0] = KX023_REG_CNTL1;                  //�������ٶ�
    data[1] = GSENSOR_INIT_CNTL1_VALUE;//0x92;  //1001 0010��4��5bitΪ2/4/8g����
    Gsensor_WriteOneReg(data,2);       
}

/****************************************************************
** Function name:           Gsensor_AccInit
** Descriptions:            ACC��ʼ��
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
** Created by:              WX             
** Created Date:            2016.11.30        
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Gsensor_AccInit(void)
{
    //���ų�ʼ��
    GSensor_IntInit();
    //acc��ʼ��
    Gsensor_Configure();
}

/****************************************************************
** Function name:           Gsensor_EnableMotion
** Descriptions:            �˶�ʹ��
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
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
    Gsensor_WriteReadReg(data, 1, &data[2], 1);   //��ȡ�������Ĵ���1��ֵ������
    
    data[0] = KX023_REG_CNTL1;              
    data[1] = 0x00;                                 //ֹͣ���ٶ�
    Gsensor_WriteOneReg(data, 2);
    
    data[0] = KX023_REG_INC4;                       //�жϿ��ƼĴ���4
    data[1] = 0x02;                                 //����Ϊ����ģʽ
    Gsensor_WriteOneReg(data, 2);
    
    data[0] = KX023_REG_CNTL1;  
    data[1] = data[2] | 0x02;                       //���¿������ٶȲ�����ʹ��
    Gsensor_WriteOneReg(data, 2);
}

/****************************************************************
** Function name:           Gsensor_DisableMotion
** Descriptions:            �˶�ʧ��
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
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
    Gsensor_WriteReadReg(data, 1, &data[2], 1);   //��ȡ���ƼĴ���1ֵ������
    
    data[0] = KX023_REG_CNTL1;                      
    data[1] = 0x00;                                 //ֹͣ���ٶ�
    Gsensor_WriteOneReg(data, 2);
    
    data[0] = KX023_REG_INC4;
    data[1] = 0x00;                                 //����ʧ��
    Gsensor_WriteOneReg(data, 2);
    
    data[0] = KX023_REG_CNTL1;
    data[1] = data[2] & 0xFD;                       //���ٶȿ���
    Gsensor_WriteOneReg(data, 2);
}

/****************************************************************
** Function name:           Gsensor_FifoEnablePara
** Descriptions:            FIFO����ʹ������
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
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
    
    data[0] = KX023_REG_CNTL1;                      //��ȡ���ƼĴ���1���ݲ�����
    Gsensor_WriteReadReg(data, 1, &data[2], 1);
    
    data[0] = KX023_REG_CNTL1;
    data[1] = 0x00;                                 //�رռ��ٶ��뻽��ʧ��
    Gsensor_WriteOneReg(data, 2);
  
    //����buf����
    data[0] = KX023_REG_BUF_CNTL1;
    data[1] = 0x28;                             //������ֵ40(���41)
    Gsensor_WriteOneReg(data, 2);  

    data[0] = KX023_REG_BUF_CLEAR;
    data[1] = 0xE0;                             //д�����ֵ���buf
    Gsensor_WriteOneReg(data, 2); 

    data[0] = KX023_REG_BUF_CNTL2;
    data[1] = 0xC0;//1100 0011;                 //ʹ��ȡ��buf��16bit���ݻ���
    Gsensor_WriteOneReg(data, 2);             //FIFO    
      
    data[0] = KX023_REG_CNTL1;
    data[1] = data[2] | 0x80;                       //�������ٶ�
    Gsensor_WriteOneReg(data, 2);
}

/****************************************************************
** Function name:           Gsensor_CleanFifo
** Descriptions:            �������buf
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
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
    data[1] = 0xE0;                                 //���buf
    Gsensor_WriteOneReg(data, 2); 
}

/****************************************************************
** Function name:           Gsensor_FifoDisablePara
** Descriptions:            FIFO����ʧ������
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
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
    
    data[0] = KX023_REG_CNTL1;                      //��ȡ���ƼĴ���1���ݲ�����
    Gsensor_WriteReadReg(data, 1, &data[2], 1);
    
    data[0] = KX023_REG_CNTL1;
    data[1] = 0x00;                                 //�رռ��ٶ��뻽��ʧ��
    Gsensor_WriteOneReg(data, 2);
    
    data[0] = KX023_REG_BUF_CNTL1;
    data[1] = 0x00;                                 //���������ֵ
    Gsensor_WriteOneReg(data, 2);  
    
    data[0] = KX023_REG_BUF_CNTL2;
    data[1] = 0x00;                                 //�ر�ȡ��
    Gsensor_WriteOneReg(data, 2);

    data[0] = KX023_REG_CNTL1;
    data[1] = data[2] | 0x80;//0x8A;                //�������ٶ�  �����ᴥ���ж�
    Gsensor_WriteOneReg(data, 2);
}

/****************************************************************
** Function name:           Gsensor_Init1State
** Descriptions:            ��ȡ�жϹܽ�1��״̬
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
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
** Descriptions:            ����
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
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
    data[1] = 0x00;                                 //�رռ��ٶ��뻽��ʧ��
    Gsensor_WriteOneReg(data, 2);   
}

/****************************************************************
** Function name:           Gsensor_Check
** Descriptions:            �Լ�
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
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
    
    data[0] = KX023_REG_CNTL1;                      //��ȡ���ƼĴ���1���ݲ�����
    Gsensor_WriteReadReg(data, 1, &data[2], 1);    
    
    if( data[2] != GSENSOR_INIT_CNTL1_VALUE)  //�Լ첻ͨ����Ҫ���³�ʼ��
    {
        return 0;
    }
    
    return 1;
}

/****************************************************************
** Function name:           Gsensor_ReadConfig
** Descriptions:            ��ȡ�Ĵ���ֵ�����ڲ���
** input parameters:        ��
** output parameters:       ��
** Returned value:          ��
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

    //�ֱ���
    data[0] = KX023_REG_CNTL1; 
    Gsensor_WriteReadReg(&data[0], 1, &data1, 1);    
    *r_data = data1;
    r_data++;
   
    //���Ƶ��
    data[0] = KX023_REG_ODCNTL; 
    Gsensor_WriteReadReg(&data[0], 1, &data1, 1);    
    *r_data = data1;
    r_data++; 
    
    //�������Ƶ��
    data[0] = KX023_REG_CNTL3; 
    Gsensor_WriteReadReg(&data[0], 1, &data1, 1);    
    *r_data = data1;
    r_data++; 
    
    //�˶���������
    data[0] = KX023_REG_WUFC; 
    Gsensor_WriteReadReg(&data[0], 1, &data1, 1);    
    *r_data = data1;
    r_data++;            
    
    data[0] = KX023_REG_ATH; //������ֵn*16
    Gsensor_WriteReadReg(&data[0], 1, &data1, 1);    
    *r_data = data1;
    r_data++; 
 
    data[0] = KX023_REG_INC1; //�жϿ��ƼĴ���1
    Gsensor_WriteReadReg(&data[0], 1, &data1, 1);    
    *r_data = data1;
    r_data++; 
     
    data[0] = KX023_REG_INC4; //�жϿ��ƼĴ���4
    Gsensor_WriteReadReg(&data[0], 1, &data1, 1);    
    *r_data = data1;
    r_data++;

    //���β���������ֵ����
    data[0] = KX023_REG_LP_CNTL; //�͹��Ŀ��ƼĴ���
    Gsensor_WriteReadReg(&data[0], 1, &data1, 1);    
    *r_data = data1;
    r_data++;      
       
    //����buf����
    data[0] = KX023_REG_BUF_CNTL1;
    Gsensor_WriteReadReg(&data[0], 1, &data1, 1);  
    *r_data = data1;
    r_data++;  
    
    data[0] = KX023_REG_BUF_CNTL2;
    Gsensor_WriteReadReg(&data[0], 1, &data1, 1);  
    *r_data = data1;
    r_data++;  
        
    //�жϹܽŵ�ֵ
    data[0] = KX023_REG_INS2; //�͹��Ŀ��ƼĴ���
    Gsensor_WriteReadReg(&data[0], 1, &data1, 1);    
    *r_data = data1;
    r_data++;       
           
    //�жϹܽŵ�ֵ
    data[0] = KX023_REG_INT_REL; //�͹��Ŀ��ƼĴ���
    Gsensor_WriteReadReg(&data[0], 1, &data1, 1);    
    *r_data = data1;
    r_data++;  
    
}
