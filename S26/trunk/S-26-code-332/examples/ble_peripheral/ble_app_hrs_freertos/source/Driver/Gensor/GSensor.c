/*******************Copyright(c)*********************************
** Veepoo Tech
** COPYRIGHT (C) 2016-2019
**----------------------FileInfo---------------------------------
** File name:                  GSensor.c
** Last modified Date:         2017-08-12
** Last SYS_VERSION:           0.0.1
** Descriptions:
**---------------------------------------------------------------
** Createdby:                  WX
** Createddate:                2017-01-04
** SYS_VERSION:                0.0.1
** Descriptions:               ���ٶ�
**---------------------------------------------------------------
** Modified by:				
** Modified date:			
** SYS_VERSION:				
** Modified Descriptions:	
****************************************************************/

#include "app_type.h"
#include "GSensor.h"
#include "board.h"
#include "nrf_delay.h"
#include "bsp_spi.h"
#include "app_error.h" 
#include "nrf_drv_gpiote.h"
#include "data_manage.h"
#include "algorithm_thread.h"
#include "battery_adc.h"
#include "gui_thread.h"
#include "communication_thread.h"
#include "ble_thread.h"


#define GSENSOR_CS_HIGH             GSensor_CS_High()
#define GSENSOR_CS_LOW              GSensor_CS_Low()

#define GSENSOR_INIT_CNTL1_VALUE    0x92    //�������ٵĿ���KX023_REG_CNTL1��ֵ


static uint8_t gsensor_lock = 0;             //ͨѶ����
static uint8_t gsensor_int_lock = 0;         //�ж�����
static uint8_t gsensor_stop_flag = 0;      //�رռ��ٶȱ��

void GSensor_Int1handler(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action);
void GSensor_CS_High(void);
void GSensor_CS_Low(void);

/****************************************************************
** Function name:             GSensor_read_reg
** Descriptions:              ���ٶȶ�ȡ�����Ĵ���
** input parameters:          reg_addr��          �Ĵ�����ַ
** output parameters:         �Ĵ�����ֵ
** Returned value:            ��
** Created by:                WX
** Created Date:              2016-11-29
****************************************************************/ 
static uint8_t GSensor_read_reg(uint8_t reg_addr)
{
    uint8_t reg_value = 0;
    
    reg_addr = reg_addr | 0x80;
    
    GSENSOR_CS_LOW;
    Bps_Spi0Write(&reg_addr,1);
    Bps_Spi0Read(&reg_value, 1);
    GSENSOR_CS_HIGH;

    return reg_value;
}

/****************************************************************
** Function name:             GSensor_read_regs
** Descriptions:              ���ٶȶ��Ĵ���
** input parameters:          addr��           ��ַ
                              value��          ����ָ��
                              len��            ���ݴ�С
** output parameters:         ��
** Returned value:            ��
** Created by:                WX
** Created Date:              2016-11-29
****************************************************************/ 
static void GSensor_read_regs(uint8_t reg_addr,uint8_t* p_data,uint8_t len)
{
    reg_addr = reg_addr | 0x80;
    
    GSENSOR_CS_LOW;
    Bps_Spi0Write(&reg_addr,1);
    Bps_Spi0Read(p_data, len);
    GSENSOR_CS_HIGH;
}

/****************************************************************
** Function name:             GSensor_write_reg
** Descriptions:              ���ٶ�д�Ĵ���
** input parameters:          addr��           ��ַ
                              value��          ��ֵ
** output parameters:         ��
** Returned value:            ��
** Created by:                WX
** Created Date:              2016-11-29
****************************************************************/ 
static void GSensor_write_reg(uint8_t addr, uint8_t value)
{
    static uint8_t temp[2] = {0};
    
    temp[0] = addr;
    temp[1] = value;
    
    GSENSOR_CS_LOW;
    Bps_Spi0Write(temp,2);
    GSENSOR_CS_HIGH;
}

/****************************************************************
** Function name:             GSensor_CS_High
** Descriptions:              ���ٶȲ�ѡ��
** input parameters:          ��
** output parameters:         ��
** Returned value:            ��
** Created by:                WX
** Created Date:              2016-11-29
****************************************************************/ 
void GSensor_CS_High(void)
{
    do   //high
    {
        nrf_gpio_pin_set(GSENSOR_CS);
    }while(0);
}

/****************************************************************
** Function name:             GSensor_CS_Low
** Descriptions:              ���ٶ�ѡ��
** input parameters:          ��
** output parameters:         ��
** Returned value:            ��
** Created by:                WX
** Created Date:              2016-11-29
****************************************************************/ 
void GSensor_CS_Low(void)
{
    do   //high
    {
        nrf_gpio_pin_clear(GSENSOR_CS);
    }while(0);
}

/****************************************************************
** Function name:             GSensor_INTInit
** Descriptions:              ���ٶ��жϳ�ʼ��
** input parameters:          ��
** output parameters:         ��
** Returned value:            ��
** Created by:                WX
** Created Date:              2016-11-29
****************************************************************/ 
void GSensor_INTInit(void)
{
    uint32_t err_code = NRF_SUCCESS;
    
    nrf_gpio_cfg_output(GSENSOR_CS);
    nrf_gpio_pin_set(GSENSOR_CS);//high

    if(!nrf_drv_gpiote_is_init())
    {
        err_code = nrf_drv_gpiote_init();    
        APP_ERROR_CHECK(err_code);
    }
    
    //nrf_drv_gpiote_in_config_t in_config = GPIOTE_CONFIG_IN_SENSE_LOTOHI(true);   
    nrf_drv_gpiote_in_config_t in_config = GPIOTE_CONFIG_IN_SENSE_TOGGLE(true); 
    
    in_config.pull = NRF_GPIO_PIN_PULLDOWN;//NRF_GPIO_PIN_PULLUP;            //PULL UP            

    static uint8_t gsensor_int_init = 0;
    if(gsensor_int_init == 1)
    {
        nrf_drv_gpiote_in_uninit(GSENSOR_INIT1);
    }
    //INT1
    err_code = nrf_drv_gpiote_in_init(GSENSOR_INIT1, &in_config, GSensor_Int1handler);
    APP_ERROR_CHECK(err_code);
    
    gsensor_int_init = 1;
    
    nrf_drv_gpiote_in_event_enable(GSENSOR_INIT1, true);
}

/****************************************************************
** Function name:             GSensor_Int1handler
** Descriptions:              ���ٶ��жϴ���
** input parameters:          pin��         �ܽ�
                              action��      �ܽŵ�ƽ
** output parameters:         ��
** Returned value:            ��
** Created by:                WX
** Created Date:              2016-11-29
****************************************************************/ 
void GSensor_Int1handler(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action)
{
    volatile uint8_t reg_value = 0;
    uint8_t len = 0;
    int16_t acc_axis_data[30] = {0};
    const acc_data_param_t* p_acc_data = &p_data_manage->acc;
    static int16_t sport_hold_cnt = 0;
    int16_t sport_hold_cnt_tmp = 0;
    
    if(!nrf_gpio_pin_read(GSENSOR_INIT1))
    {
        return;
    }
    
    if(gsensor_lock == 1)
    {   //������λ��
        return;
    }
    gsensor_int_lock = 1;
    gsensor_lock = 1;
    
    if(p_data_manage->device_trace_param_s.factory_mode == 1)
    {
        sport_hold_cnt_tmp = 5;
    }
    else
    {
        sport_hold_cnt_tmp = 300;//40;60s/8s
    }
    
    do
    {
        // 1.�ƶ�����жϲ��Ҵ�FIFO�ɼ�
        reg_value = GSensor_read_reg(KX023_REG_INS2);
        if(reg_value&0x02)
        {
			Battery_GiveMove();
            sport_hold_cnt = 0;
            // ���˶��ж���ʱ�ر�
            GSensor_write_reg(KX023_REG_INC4,0x60);
            // �ص��˶��ж�ʹ��
        	if(GSensor_read_reg(KX023_REG_BUF_CNTL2)  != 0xC0)
            {
                GSensor_write_reg(KX023_REG_BUF_CNTL2,0xC0);  
                break;
            } 
        }
        
        // ���FIFO���˵���û���˶��ж�,��ر�FIFO�ɼ�
        if(((reg_value&0x60) != 0)&&(sport_hold_cnt++ >= sport_hold_cnt_tmp))//5))//300))//40;60s/8s
        {
            // �жϻָ�
            GSensor_write_reg(KX023_REG_INC4,0x62);
            GSensor_write_reg(KX023_REG_BUF_CNTL2,0x00); 
            break;            
        }
        
        // 2.��ȡFIFO�ĳ���
        len = GSensor_read_reg(KX023_REG_BUF_STATUS1);
        if(len > 0)
        {
            len = (len > 60) ? 60 : len; 
            GSensor_read_regs(KX023_REG_BUF_READ,(uint8_t*)&acc_axis_data[0], len);
            
            //���FIFO����
            GSensor_write_reg(KX023_REG_BUF_CLEAR,0x00);
                
            // �������ݴ���
            DataManage_UpdateFIFOData(acc_axis_data,(int8_t)(len/6));
                
            AlgThread_GsensorEventCommit();
                                        
            if(((UPLOAD_ADC_GSENSOR_EVENT == p_data_manage->upload)&&(p_acc_data->adc_send_len >= 3)) ||
               ((UPLOAD_ADC_GESTURE_EVENT == p_data_manage->upload)&&(p_acc_data->gesture_wait_send_len >= 3)))
            {
                ComThread_SendADCData();
            }
        }        
    }while(0);
    
    // ���жϱ�־,��һ��INT_REL�Ĵ�������
    GSensor_read_reg(KX023_REG_INT_REL);
    
    gsensor_lock = 0;
    gsensor_int_lock = 0;
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
    
    GSensor_write_reg(0x7F, 0x00);     
    GSensor_write_reg(KX023_REG_CNTL2, 0x00);   
    GSensor_write_reg(KX023_REG_CNTL2, 0x80);         //RAM��������
    
    //��ʱ2ms
    nrf_delay_ms(4);
    
    GSensor_read_regs(KX023_REG_WHO,(uint8_t*)&data[2], 1);
    if(0x15 == data[2])//KX023 0X15; KX022 0X14
    {
        GSensor_read_regs(KX023_REG_COTR,(uint8_t*)&data[2], 1);
        if(0x55 == data[2])
        {
            return 1;
        }
    }
    return 0;
}

/****************************************************************
** Function name:             GSensor_configure
** Descriptions:              ���ٶ�����
** input parameters:          ��
** output parameters:         ��
** Returned value:            ��
** Created by:                WX
** Created Date:              2016-11-29
****************************************************************/ 
uint8_t data[2] = {0};
void GSensor_configure(void)
{        
    uint8_t cnt = 0;

    while(cnt++ < 5)
    {
        //acc�����λ
        if(Gsensor_SoftReset())
        {
            break;
        }
    }
    // !ע��:�ڲ��Թ����з���,���Gensor���ϵ�,������ֵ�ǲ���ָ�Ĭ��ֵ Add by GT 2017-06-20
    // ����,�ڳ�ʼ��ʱӦ�ó�ʼ�����еļĴ���
    
    // �رռ��ٶ��뻽��ʧ��
    GSensor_write_reg(KX023_REG_CNTL1, 0x00);  
    
    //��ȡ�����ļĴ�����ֵΪ0x15��������ٶ�����
    GSensor_read_regs(KX023_REG_WHO,(uint8_t*)&data[0], 1); 
    
    // CNTL2�Ĵ���д��Ĭ��ֵ
    GSensor_write_reg(KX023_REG_CNTL2, 0x00); 
    
    // �������Ƶ�� ���������50hz 
    GSensor_write_reg(KX023_REG_CNTL3, 0x9F);     
    
    // ���Ƶ�� ����� 0x02:50hz 0x03:100hz
    GSensor_write_reg(KX023_REG_ODCNTL, 0x02);//0x00);//0x01);//0x03);//
    
    //����һ���ߵ�ƽֱ����ȡINT_REL����жϱ�־
    GSensor_write_reg(KX023_REG_INC1,0x30);  

    // ʹ���ƶ�����ж�ʹ��
    GSensor_write_reg(KX023_REG_INC2,0x3F);  

    // tap/double�ж�ʹ�ܹر�
    GSensor_write_reg(KX023_REG_INC3,0x3F); 

    // �жϴ�����ʽ����(BUF full/WMI1/WUFI1���ַ�ʽ)
    GSensor_write_reg(KX023_REG_INC4,0x62);

    // ����INT2ʹ��Ĭ��ֵ
    GSensor_write_reg(KX023_REG_INC5,0x10);
    
    // �ر�INT2�жϹܽ����еĴ�����ʽ
    GSensor_write_reg(KX023_REG_INC6,0x00);
    
    //�ֱ���//8bit��Ч����(�߷ֱ���)
    GSensor_write_reg(KX023_REG_CNTL1,0x52);              
    
    // �˶���������//0.1*50 = 5
    GSensor_write_reg(KX023_REG_WUFC,0x19);              
    
    GSensor_write_reg(KX023_REG_ATH,0x01);              

    // ���β���������ֵ����
    GSensor_write_reg(KX023_REG_LP_CNTL,0x00);
    
    // ����buf����
    GSensor_write_reg(KX023_REG_BUF_CNTL1, 0x0A);  

    // �ر�FIFO�ɼ�ʹ��
    GSensor_write_reg(KX023_REG_BUF_CNTL2, 0x00); 
    
    // ���FIFO����
    GSensor_write_reg(KX023_REG_BUF_CLEAR, 0xE0); 

    //����ж�
    GSensor_read_reg(KX023_REG_INT_REL);    
   
   //ʹ�ܼ��ٶ�
    GSensor_write_reg(KX023_REG_CNTL1,GSENSOR_INIT_CNTL1_VALUE);   
}

/****************************************************************
** Function name:             GSensor_AccInit
** Descriptions:              ���ٶȳ�ʼ��
** input parameters:          ��
** output parameters:         ��
** Returned value:            ��
** Created by:                WX
** Created Date:              2016-11-29
****************************************************************/ 
void GSensor_AccInit(void)
{
//    Bps_Spi0Init();

    gsensor_lock = 0;
    gsensor_int_lock = 0;
	
    //���ų�ʼ��
    GSensor_INTInit(); 
	
    nrf_delay_us(20000);
	
    //acc��ʼ��
    GSensor_configure();
    
}

/****************************************************************
** Function name:             GSensor_AccStop
** Descriptions:              
** input parameters:          ��
** output parameters:         ��
** Returned value:            ��
** Created by:                WX
** Created Date:              2016-11-29
****************************************************************/ 
void GSensor_AccStop(void)
{
	if(gsensor_lock == 1)
	{   // ���ùر��ź�
        gsensor_stop_flag = 1;
        BleThread_GsensorRestart();
        return;
    }
    gsensor_lock = 1;
    
    // �رռ��ٶ��뻽��ʧ��
    GSensor_write_reg(KX023_REG_CNTL1, 0x00);

    nrf_drv_gpiote_in_event_disable(GSENSOR_INIT1);	
    
    gsensor_stop_flag = 0;    
    gsensor_lock = 0;
    
}

void Thread_GSensorRestartTask(void)
{ 	
    // WJ.20181102.���³�ʼ�����ٶ�,��ֹ���ٶ��쳣����ͨ�����ػ��ָ�
	if(gsensor_lock == 1)
	{
        //״̬���������ź�-���ݶ�ȡ�У��Ӻ���
        if(gsensor_int_lock == 1)
        {
            BleThread_GsensorRestart();
            return;
        }
	}
    
    // �ٴιػ�
    if(gsensor_stop_flag == 1)
    {
        GSensor_AccStop();
        return;
    }
    
    // �ٴο���
	gsensor_lock = 1;
    nrf_drv_gpiote_in_event_disable(GSENSOR_INIT1);	
    //acc��ʼ��
    GSensor_configure();   
/*    
    //acc��ʼ��
    GSensor_write_reg(KX023_REG_CNTL1,GSENSOR_INIT_CNTL1_VALUE);  
*/   
    //���ų�ʼ��
    nrf_drv_gpiote_in_event_enable(GSENSOR_INIT1, true);
    
	gsensor_lock = 0;
}

/****************************************************************
** Function name:             Gensor_Immediate_ADC
** Descriptions:              ���ٶ�adc
** input parameters:          p_data��          ����ָ��
** output parameters:         ��
** Returned value:            bool
** Created by:                WX
** Created Date:              2016-11-29
****************************************************************/ 
bool Gensor_Immediate_ADC(int16_t* p_data)
{
    bool bRet = false;
    
    if(GSensor_read_reg(KX023_REG_BUF_CNTL2) & 0x80)
    {
        p_data[0] = GSensor_read_reg(KX023_REG_XOUTL) | (GSensor_read_reg(KX023_REG_XOUTH)<<8);
        p_data[1] = GSensor_read_reg(KX023_REG_YOUTL) | (GSensor_read_reg(KX023_REG_YOUTH)<<8);
        p_data[2] = GSensor_read_reg(KX023_REG_ZOUTL) | (GSensor_read_reg(KX023_REG_ZOUTH)<<8);
        bRet = true;
    }
    
    return bRet;
}



