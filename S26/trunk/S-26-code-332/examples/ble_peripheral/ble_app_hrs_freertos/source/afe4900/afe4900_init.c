/***************************Copyright(c)***********************************
**                       Veepoo Tech
**                  COPYRIGHT (C) 2016-2019
**-----------File Info-----------------------------------------------------
** File        name:     afe4900_init.c
** Last modified Date:   2018-10-18
** Last SYS_VERSION:     0.0.0
** Descriptions:            
**-------------------------------------------------------------------------
** Created  by:          JSH
** Created  date:        2018-10-18
** SYS_VERSION:          0.0.0
** Descriptions:         
**-------------------------------------------------------------------------
** Modified by:				
** Modified date:  			          
** SYS_VERSION:                 
** Descriptions:                        
***************************************************************************/
#include "afe4900_init.h"
#include "board.h"
#include "nrf_delay.h"
#include "nrf_gpio.h"
#include "nrf_drv_gpiote.h"
#include "app_error.h"
#include "afe4900_entry.h"
#include "bsp_spi.h"
#include "Blood_OxygenDetection.h"
#include "afe4900_calibration.h"
#include "Afe4410_HrCalib.h"
#include "algorithm_thread.h"

static uint8_t afe4900_gpiote_init = 0;
// AFE4900的PTT模式的标志
static uint8_t afe4900_ptt_flag = 0;
///导联电流阀值:0~3对应,12.5\25\50\100nA
///导联电压阀值:0~7对应，1.15\1.1\1.05\1.0\1.35\1.3\1.25\1.2V
///ECG放大倍数：0~6对应，3\2\4\5\6\9\12倍
#define     ECG_LEAD_OFF_I  2
#define     ECG_LEAD_OFF_V  1
#define     ECG_LEAD_OFF_G  1
// YQW modify 2019年1月4日 14:56:04
afe4900_param_t  afe4900_param = {ECG_LEAD_OFF_I, ECG_LEAD_OFF_V, ECG_LEAD_OFF_G, ECG_LEAD_OFF_G};

afe4410_ppg_param_t afe4410_ppg_param_s = {0, 50, 50,
                    5, 5, 5, 5,
                    0,
                    0, 0, 0, 0,
                    1, 1, 1, 1,
                    1, 3, 3, 1, 1,};

void Afe4410_RegPpgParamSetDo(void);                    
                    
// 心率4410初始化标志位		用于校准								
extern uint8_t 				 hr_init_flag;
// 血氧4410初始化标志位		用于校准
extern uint8_t         spo2_4410_init;
/*******************************************************************************************************
** Function name:        AFE4900_HardwarePowerDown
** Descriptions:         afe4900硬件低功耗
** input parameter:      无   
** Returned Value:       无
** Created  by:          JSH
** Created  Datas:       2018-10-12
*********************************************************************************************************/
void AFE4900_HardwarePowerDown(void)
{
    Afe4900_LdoTxOff();
    
	Afe4900_Control1Init();
	Afe4900_SenInit();
	Afe4900_ResetzInit();
	Afe4900_EnableHwPdn();
}

/*******************************************************************************************************
** Function name:        Afe4900_LdoTxOn
** Descriptions:         afe4900开启LDO
** input parameter:      无   
** Returned Value:       无
** Created  by:          JSH
** Created  Datas:       2018-10-22
*********************************************************************************************************/
void Afe4900_LdoTxOn(void)
{
	LDO_TX_ON();
}

/*******************************************************************************************************
** Function name:        Afe4900_LdoTxOff
** Descriptions:         afe4900关闭LDO
** input parameter:      无   
** Returned Value:       无
** Created  by:          JSH
** Created  Datas:       2018-10-22
*********************************************************************************************************/
void Afe4900_LdoTxOff(void)
{
	LDO_TX_OFF();
}

/*******************************************************************************************************
** Function name:        Afe4900_Control1Init
** Descriptions:         afe4900的Control1初始化
** input parameter:      无   
** Returned Value:       无
** Created  by:          JSH
** Created  Datas:       2018-10-12
*********************************************************************************************************/
void Afe4900_Control1Init(void)
{
	// 硬件已实现初始化
}

/*******************************************************************************************************
** Function name:        Afe4900_SenInit
** Descriptions:         afe4900的SPI片选初始化
** input parameter:      无   
** Returned Value:       无
** Created  by:          JSH
** Created  Datas:       2018-10-12
*********************************************************************************************************/
void Afe4900_SenInit(void)
{
	nrf_gpio_cfg_output(AFE_SEN); 
	nrf_gpio_pin_set(AFE_SEN);
}

/*******************************************************************************************************
** Function name:        Afe4900_ResetzInit
** Descriptions:         afe4900的Resetz初始化
** input parameter:      无   
** Returned Value:       无
** Created  by:          JSH
** Created  Datas:       2018-10-12
*********************************************************************************************************/
void Afe4900_ResetzInit(void)
{
	nrf_gpio_cfg_output(AFE_RESETZ); 
	nrf_gpio_pin_set(AFE_RESETZ);
}

/*******************************************************************************************************
** Function name:        Afe4900_EnableHwPdn
** Descriptions:         
** input parameter:      无   
** Returned Value:       无
** Created  by:          JSH
** Created  Datas:       2018-10-12
*********************************************************************************************************/
void Afe4900_EnableHwPdn(void)
{
	nrf_gpio_cfg_output(AFE_RESETZ); 
	nrf_gpio_pin_clear(AFE_RESETZ);
	nrf_delay_ms(10);
}

/*******************************************************************************************************
** Function name:        Afe4900_DisableHwPdn
** Descriptions:         
** input parameter:      无   
** Returned Value:       无
** Created  by:          JSH
** Created  Datas:       2018-10-12
*********************************************************************************************************/
void Afe4900_DisableHwPdn(void)
{
	nrf_gpio_cfg_output(AFE_RESETZ); 
	nrf_gpio_pin_set(AFE_RESETZ);
	nrf_delay_ms(10);
}

/*******************************************************************************************************
** Function name:        Afe4900_TriggerHwReset
** Descriptions:         
** input parameter:      无   
** Returned Value:       无
** Created  by:          JSH
** Created  Datas:       2018-10-12
*********************************************************************************************************/
void Afe4900_TriggerHwReset(void)
{
	nrf_gpio_cfg_output(AFE_RESETZ); 
	nrf_gpio_pin_clear(AFE_RESETZ);
	nrf_delay_us(30);
	
	nrf_gpio_cfg_output(AFE_RESETZ); 
	nrf_gpio_pin_set(AFE_RESETZ);
	nrf_delay_ms(10);
}

/*******************************************************************************************************
** Function name:        Afe4900_AdcrdyInterruptInit
** Descriptions:         afe4900中断初始化
** input parameter:      无   
** Returned Value:       无
** Created  by:          JSH
** Created  Datas:       2018-10-12
*********************************************************************************************************/
void Afe4900_AdcrdyInterruptInit(nrf_drv_gpiote_evt_handler_t event_handler)
{
	ret_code_t err_code;
    
    // GPIOTE驱动注册
    if(!nrf_drv_gpiote_is_init())
    {
        err_code = nrf_drv_gpiote_init();    
        APP_ERROR_CHECK(err_code);
    }
    
    nrf_drv_gpiote_in_config_t in_config = GPIOTE_CONFIG_IN_SENSE_LOTOHI(true);    //high to low       
    in_config.pull = NRF_GPIO_PIN_NOPULL;            //PULL UP

    if(afe4900_gpiote_init == 1)
    {
        nrf_drv_gpiote_in_uninit(AFE_ADC_RDY);
        afe4900_gpiote_init = 0;
    }
	   
    err_code = nrf_drv_gpiote_in_init(AFE_ADC_RDY, &in_config, event_handler);
    APP_ERROR_CHECK(err_code);
    
    afe4900_gpiote_init = 1;
    
    //nrf_drv_gpiote_in_event_enable(AFE_ADC_RDY, true);
}

/*******************************************************************************************************
** Function name:        Afe4900_AdcrdyInterruptEnable
** Descriptions:         afe4900中断使能
** input parameter:      无   
** Returned Value:       无
** Created  by:          JSH
** Created  Datas:       2018-10-12
*********************************************************************************************************/
void Afe4900_AdcrdyInterruptEnable(void)
{
	nrf_drv_gpiote_in_event_enable(AFE_ADC_RDY, true);
}

/*******************************************************************************************************
** Function name:        Afe4900_AdcrdyInterruptDisable
** Descriptions:         afe4900中断不使能
** input parameter:      无   
** Returned Value:       无
** Created  by:          JSH
** Created  Datas:       2018-10-12
*********************************************************************************************************/
void Afe4900_AdcrdyInterruptDisable(void)
{
    nrf_drv_gpiote_in_event_disable(AFE_ADC_RDY);
}

/*******************************************************************************************************
** Function name:        Afe4900_SpiSelInit
** Descriptions:         afe4900的spi与iic传输接口的选择
** input parameter:      无
** Returned Value:       无
** Created  by:          JSH
** Created  Datas:       2018-10-12
*********************************************************************************************************/
void Afe4900_SpiSelInit(void)
{
	// 已在硬件初始化，选择spi
}

/*******************************************************************************************************
** Function name:        AFE4900_SpiWrite
** Descriptions:         afe4900的spi写
** input parameter:      无   
** Returned Value:       无
** Created  by:          JSH
** Created  Datas:       2018-10-12
*********************************************************************************************************/
void AFE4900_SpiWrite(uint8_t address, uint8_t *data)
{
	nrf_gpio_cfg_output(AFE_SEN); 
	nrf_gpio_pin_clear(AFE_SEN);
	
    Bps_Spi1Write(&address, 1);
    Bps_Spi1Write(data, 3);
	
	nrf_gpio_pin_set(AFE_SEN);
}

/*******************************************************************************************************
** Function name:        AFE4900_SpiRead
** Descriptions:         afe4900的spi读
** input parameter:      无
** Returned Value:       无
** Created  by:          JSH
** Created  Datas:       2018-10-12
*********************************************************************************************************/
void AFE4900_SpiRead(uint8_t address, uint8_t *data)
{
	nrf_gpio_cfg_output(AFE_SEN); 
	nrf_gpio_pin_clear(AFE_SEN);
	
    Bps_Spi1Write(&address, 1);
    Bps_Spi1Read(data, 3);
	
	nrf_gpio_pin_set(AFE_SEN);
}

/*******************************************************************************************************
** Function name:        Afe4900_FifoSpiRead
** Descriptions:         afe4900的fifo spi读
** input parameter:      无
** Returned Value:       无
** Created  by:          JSH
** Created  Datas:       2018-10-12
*********************************************************************************************************/
void Afe4900_FifoSpiRead(uint8_t address, uint8_t *data, uint32_t data_cnt)
{
	nrf_gpio_cfg_output(AFE_SEN); 
	nrf_gpio_pin_clear(AFE_SEN);
	
    Bps_Spi1Write(&address, 1);
	
	while(data_cnt > 255)
	{
        Bps_Spi1Read(data, 255);
		data += 255;
		data_cnt -= 255;
	}
	
	if(data_cnt)
	{
        Bps_Spi1Read(data, data_cnt);
	}
	
	nrf_gpio_pin_set(AFE_SEN);
}

/*******************************************************************************************************
** Function name:        Afe4900_RegWrite
** Descriptions:         afe4900的寄存器写
** input parameter:      无   
** Returned Value:       无
** Created  by:          JSH
** Created  Datas:       2018-10-12
*********************************************************************************************************/
void Afe4900_RegWrite(unsigned char reg_address, unsigned int data)
{
	unsigned char config_data[3] = {0};
	
	config_data[0]=(unsigned char)(data >>16);
	config_data[1]=(unsigned char)(((data & 0x00FFFF) >>8));
	config_data[2]=(unsigned char)(((data & 0x0000FF)));

	AFE4900_SpiWrite(reg_address, config_data);
}

/*******************************************************************************************************
** Function name:        Afe4900_RegRead
** Descriptions:         afe4900的寄存器读
** input parameter:      无   
** Returned Value:       无
** Created  by:          JSH
** Created  Datas:       2018-10-12
*********************************************************************************************************/
signed int Afe4900_RegRead(unsigned char reg_address)
{
	unsigned char config_data[3] = {0};
	unsigned int ret_val = 0;

	AFE4900_SpiRead(reg_address, config_data);
	
	ret_val = config_data[0];
	ret_val = (ret_val << 8) | config_data[1];
	ret_val = (ret_val << 8) | config_data[2];

	return (signed int)ret_val;
}

/****************************************************************
** Function name:             Afe4900_DataConvert
** Descriptions:              afe4900 数据转换
** input parameters:          data： 数据
** output parameters:         无
** Returned value:            无
** Created by:                JSH
** Created Date:              2018-10-18
****************************************************************/ 
int32_t Afe4900_DataConvert(uint32_t data)
{
	// afe4900 datasheet 第28页
    int32_t temp = 0;
    uint8_t symbol = (data>>21)&0x07;
    
    if(0x00 == symbol)
    {
        //如果在测量范围内并且是正向电压
        temp = data&0x1FFFFF;
    }
    else if(0x07 == symbol)
    {
        //如果在测量范围内并且是负向电压
        temp = (int32_t)(data | 0xFF000000);
    }
    else if(0x01 == symbol)
    {
        //超出测量范围内并且是正向电压
        temp = 0x200000;
    }
    else if(0x06 == symbol)
    {
        //超出测量范围内并且是负向电压//有问题
		temp = 0x200000;
		temp = - temp;
    }

    return temp;
}

/*******************************************************************************************************
** Function name:        Afe4900_IsEnablePtt
** Descriptions:         AFE4900是否开启了PTT模式
** input parameter:      无   
** Returned Value:       无
** Created  by:          JSH
** Created  Datas:       2018-10-15
*********************************************************************************************************/
uint32_t Afe4900_IsEnablePtt(void)
{
	if(afe4900_ptt_flag == 1)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

/*******************************************************************************************************
** Function name:        Afe4900_InitStartSampling
** Descriptions:         afe4900的寄存器初始化开始采样
** input parameter:      无   
** Returned Value:       无
** Created  by:          JSH
** Created  Datas:       2018-10-12
*********************************************************************************************************/
void Afe4900_InitStartSampling(void)
{
	Afe4900_RegWrite(0x00, 0x000061);             //CONTROL0
}

/*******************************************************************************************************
** Function name:        Afe4900_InitStopSampling
** Descriptions:         afe4900的寄存器初始化停止采样
** input parameter:      无   
** Returned Value:       无
** Created  by:          JSH
** Created  Datas:       2018-10-12
*********************************************************************************************************/
void Afe4900_InitStopSampling(void)
{
	Afe4900_RegWrite(0x00, 0x000063);             //CONTROL0
}
	
/*******************************************************************************************************
** Function name:        Afe4900_RegInitPpg1
** Descriptions:         afe4900的寄存器初始化PPG模式
** input parameter:      无   
** Returned Value:       无
** Created  by:          JSH
** Created  Datas:       2018-10-12
*********************************************************************************************************/
extern uint8_t hr_init_flag;
void Afe4900_RegInitPpg1(void)
{
    // 此变量为心率校准初始化变量 
	hr_init_flag = 1;
	Afe4900_RegWrite(0x00, 0x000062);              //CONTROL0
    Afe4900_RegWrite(0x01, 0x00002B);              //LED2STC
    Afe4900_RegWrite(0x02, 0x00002F);              //LED2ENDC
    Afe4900_RegWrite(0x03, 0x00003B);              //LED1LEDSTC
    Afe4900_RegWrite(0x04, 0x000043);              //LED1LEDENDC
    Afe4900_RegWrite(0x05, 0x000035);              //ALED2STC
    Afe4900_RegWrite(0x06, 0x000039);              //ALED2ENDC
    Afe4900_RegWrite(0x07, 0x00003F);              //LED1STC
    Afe4900_RegWrite(0x08, 0x000043);              //LED1ENDC
    Afe4900_RegWrite(0x09, 0x000027);              //LED2LEDSTC
    Afe4900_RegWrite(0x0A, 0x00002F);              //LED2LEDENDC
    Afe4900_RegWrite(0x0B, 0x000049);              //ALED1STC
    Afe4900_RegWrite(0x0C, 0x00004D);              //ALED1ENDC
    Afe4900_RegWrite(0x0D, 0x000031);              //LED2CONVST
    Afe4900_RegWrite(0x0E, 0x000041);              //LED2CONVEND
    Afe4900_RegWrite(0x0F, 0x000043);              //ALED2CONVST
    Afe4900_RegWrite(0x10, 0x000053);              //ALED2CONVEND
    Afe4900_RegWrite(0x11, 0x000055);              //LED1CONVST
    Afe4900_RegWrite(0x12, 0x000065);              //LED1CONVEND
    Afe4900_RegWrite(0x13, 0x000067);              //ALED1CONVST
    Afe4900_RegWrite(0x14, 0x000077);              //ALED1CONVEND
    Afe4900_RegWrite(0x1D, 0x0009FF);              //PRPCOUNT
    Afe4900_RegWrite(0x1E, 0x000101);              //CONTROL1
    Afe4900_RegWrite(0x1F, 0x003D3D);              //TIAGAIN_2_3
    Afe4900_RegWrite(0x20, 0x00803D);              //TIAGAIN
    Afe4900_RegWrite(0x21, 0x00003D);              //TIA_AMB_GAIN
    //Afe4900_RegWrite(0x22, 0x040033);              //LEDCNTRL1
    Afe4900_RegWrite(0x22, 0x0C0008);              //LEDCNTRL1
    // FWT.20190418  选择100mA量程
    Afe4900_RegWrite(0x23, 0x12C218);              //CONTROL2
    Afe4900_RegWrite(0x24, 0x000000);              //LEDCNTRL2
    Afe4900_RegWrite(0x28, 0x000000);              //TOGGLE
    Afe4900_RegWrite(0x29, 0x000000);              //CLKDIV1
    Afe4900_RegWrite(0x2A, 0x000000);              //LED2VAL
    Afe4900_RegWrite(0x2B, 0x000000);              //ALED2VAL
    Afe4900_RegWrite(0x2C, 0x000000);              //LED1VAL
    Afe4900_RegWrite(0x2D, 0x000000);              //ALED1VAL
    Afe4900_RegWrite(0x2E, 0x000000);              //LED2-ALED2VAL
    Afe4900_RegWrite(0x2F, 0x000000);              //LED1-ALED1VAL
    Afe4900_RegWrite(0x31, 0x000020);              //CONTROL3
    Afe4900_RegWrite(0x34, 0x000000);              //PROG_INT2_STC
    Afe4900_RegWrite(0x35, 0x000000);              //PROG_INT2_ENDC
    Afe4900_RegWrite(0x36, 0x000031);              //LED3LEDSTC
    Afe4900_RegWrite(0x37, 0x000039);              //LED3LEDENDC
    Afe4900_RegWrite(0x39, 0x000000);              //CLKDIV2
    Afe4900_RegWrite(0x3A, 0x000000);              //OFFDAC
    Afe4900_RegWrite(0x3B, 0x000000);              //THRDETLOW
    Afe4900_RegWrite(0x3C, 0x000000);              //THRDETHIGH
    Afe4900_RegWrite(0x3D, 0x000000);              //THRDET
    Afe4900_RegWrite(0x3E, 0x000000);              //I_OFFDAC
    Afe4900_RegWrite(0x3F, 0x000000);              //AVG_LED2_ALED2VAL
    Afe4900_RegWrite(0x40, 0x000000);              //AVG_LED1_ALED1VAL
    Afe4900_RegWrite(0x42, 0x000000 | (4 << 6)|(0x02 << 4)|(0x02 << 0));              //FIFO
    Afe4900_RegWrite(0x43, 0x000045);              //LED4LEDSTC
    Afe4900_RegWrite(0x44, 0x00004D);              //LED4LEDENDC
    Afe4900_RegWrite(0x45, 0x000000);              //TG_PD1STC
    Afe4900_RegWrite(0x46, 0x000000);              //TG_PD1ENDC
    Afe4900_RegWrite(0x47, 0x000000);              //TG_PD2STC
    Afe4900_RegWrite(0x48, 0x000000);              //TG_PD2ENDC
    Afe4900_RegWrite(0x49, 0x000000);              //TG_PD3STC
    Afe4900_RegWrite(0x4A, 0x000000);              //TG_PD3ENDC
    
    Afe4900_RegWrite(0x4B, 0x000000);              //CONTROL4
    // 20190320 FWT 下面是TI的工程师过来所说的更改生成的代码。主要更改了寄存器 0x31 0x4b 0x6c
    //Afe4900_RegWrite(0x4B, 0x00000F);              //CONTROL4
    
    Afe4900_RegWrite(0x4E, 0x000000);              //DUAL_PD
    Afe4900_RegWrite(0x50, 0x000008);              //CONTROL5
    Afe4900_RegWrite(0x51, 0x000000);              //FIFO_OFFSET
    Afe4900_RegWrite(0x52, 0x00007D);              //DATA_RDY_STC
    Afe4900_RegWrite(0x53, 0x00007D);              //DATA_RDY_ENDC
    Afe4900_RegWrite(0x57, 0x000000);              //PROG_INT1_STC
    Afe4900_RegWrite(0x58, 0x000000);              //PROG_INT1_ENDC
    Afe4900_RegWrite(0x60, 0x000000);              //AMB_CANCELLATION
    Afe4900_RegWrite(0x64, 0x000000);              //DYN_TIA_STC
    Afe4900_RegWrite(0x65, 0x00007A);              //DYN_TIA_ENDC
    Afe4900_RegWrite(0x66, 0x000000);              //DYN_ADC_STC
    Afe4900_RegWrite(0x67, 0x00007A);              //DYN_ADC_ENDC
    Afe4900_RegWrite(0x68, 0x000000);              //DYN_CLOCK_STC
    Afe4900_RegWrite(0x69, 0x00007A);              //DYN_CLOCK_ENDC
    Afe4900_RegWrite(0x6A, 0x000084);              //DEEP_SLEEP_STC
    Afe4900_RegWrite(0x6B, 0x900000);              //DEEP_SLEEP_ENDC
    
    Afe4900_RegWrite(0x6C, 0x000000);              //PD_SHORT
    // 20190320 FWT 下面是TI的工程师过来所说的更改生成的代码。主要更改了寄存器 0x31 0x4b 0x6c
    //Afe4900_RegWrite(0x6C, 0x000800);              //PD_SHORT

    Afe4900_RegWrite(0x6D, 0x000000);              //REG_POINTER
    Afe4900_RegWrite(0x72, 0x000000);              //LED_DRIVER_CONTROL
    Afe4900_RegWrite(0x73, 0x000000);              //THR_DETECT_LOGIC
    Afe4900_RegWrite(0xC8, 0x000000);              //CLOCK_SETTINGS
    Afe4900_RegWrite(0x00, 0x000063);             //AFE4900READ MODE AND ULP

    {   // 科研添加
        // 此处设置DAC量程
        //uint8_t set_dac_ifs = 0;
        uint8_t need_set_dac_ifs = 0;
        if(IFS_DAC == 1)
        {
            need_set_dac_ifs = 0;
        }
        if(IFS_DAC == 2)
        {
            need_set_dac_ifs = 3;
        }
        if(IFS_DAC == 4)
        {
            need_set_dac_ifs = 5;
        }
        if(IFS_DAC == 5)
        {
            need_set_dac_ifs = 7;
        }
        AFE4410_WriteDacReg(
            need_set_dac_ifs,		// dac 量程

            0,	    // 绿光dac值
            0,	    // 红光dac值
            0,	    // 红外dac值
            0,		// 环境光dac值

            1,		// 绿光dac极性
            1,		// 红光dac极性
            1,		// 红外光dac极性
            1		// 环境光dac极性
        );
    }		
}

/*******************************************************************************************************
** Function name:        Afe4900_RegInitPpg2
** Descriptions:         afe4900的寄存器初始化PPG模式
** input parameter:      无   
** Returned Value:       无
** Created  by:          JSH
** Created  Datas:       2018-10-12
*********************************************************************************************************/
void Afe4900_RegInitPpg2(void)
{
	spo2_4410_init = 1;
#ifdef CTR_DIFF_CALIB
    // 初始化CTR校准参数
    cur_set_red = 0;
    cur_set_ird = 0;
    cur_set_cnt = 0;
    calc_ctr_line_flag = 0;
    calib_ctr_flag = 0;
    memset(save_red_cur_adc,0,30 * sizeof(int32_t));
    memset(save_ird_cur_adc,0,30 * sizeof(int32_t));
#endif // CTR_DIFF_CALIB

    // 20190327.FWT 经信号验证，上面这组参数效果更好，比TI给的参数好多了
        // NUMAVE 2
        Afe4900_RegWrite(0x00, 0x000062);              //CONTROL0
        Afe4900_RegWrite(0x01, 0x00002B);              //LED2STC
        Afe4900_RegWrite(0x02, 0x000037);              //LED2ENDC
        Afe4900_RegWrite(0x03, 0x00004B);              //LED1LEDSTC
        Afe4900_RegWrite(0x04, 0x00005B);              //LED1LEDENDC
        Afe4900_RegWrite(0x05, 0x00003D);              //ALED2STC
        Afe4900_RegWrite(0x06, 0x000049);              //ALED2ENDC
        Afe4900_RegWrite(0x07, 0x00004F);              //LED1STC
        Afe4900_RegWrite(0x08, 0x00005B);              //LED1ENDC
        Afe4900_RegWrite(0x09, 0x000027);              //LED2LEDSTC
        Afe4900_RegWrite(0x0A, 0x000037);              //LED2LEDENDC
        Afe4900_RegWrite(0x0B, 0x000061);              //ALED1STC
        Afe4900_RegWrite(0x0C, 0x00006D);              //ALED1ENDC
        Afe4900_RegWrite(0x0D, 0x000039);              //LED2CONVST
        Afe4900_RegWrite(0x0E, 0x000050);              //LED2CONVEND
        Afe4900_RegWrite(0x0F, 0x000052);              //ALED2CONVST
        Afe4900_RegWrite(0x10, 0x000069);              //ALED2CONVEND
        Afe4900_RegWrite(0x11, 0x00006B);              //LED1CONVST
        Afe4900_RegWrite(0x12, 0x000082);              //LED1CONVEND
        Afe4900_RegWrite(0x13, 0x000084);              //ALED1CONVST
        Afe4900_RegWrite(0x14, 0x00009B);              //ALED1CONVEND
        Afe4900_RegWrite(0x1D, 0x0009FF);              //PRPCOUNT
        Afe4900_RegWrite(0x1E, 0x000102);              //CONTROL1
        Afe4900_RegWrite(0x1F, 0x002525);              //TIAGAIN_2_3
        Afe4900_RegWrite(0x20, 0x008025);              //TIAGAIN
#ifdef AMB_DAC_MODE
        Afe4900_RegWrite(0x21, 0x005225);              //TIA_AMB_GAIN
#else
        Afe4900_RegWrite(0x21, 0x000225);              //TIA_AMB_GAIN
#endif
        Afe4900_RegWrite(0x22, 0xF08200);              //LEDCNTRL1
        Afe4900_RegWrite(0x23, 0x12C218);              //CONTROL2
        Afe4900_RegWrite(0x24, 0x000000);              //LEDCNTRL2
        Afe4900_RegWrite(0x28, 0x000000);              //TOGGLE
        Afe4900_RegWrite(0x29, 0x000000);              //CLKDIV1
        Afe4900_RegWrite(0x2A, 0x000000);              //LED2VAL
        Afe4900_RegWrite(0x2B, 0x000000);              //ALED2VAL
        Afe4900_RegWrite(0x2C, 0x000000);              //LED1VAL
        Afe4900_RegWrite(0x2D, 0x000000);              //ALED1VAL
        Afe4900_RegWrite(0x2E, 0x000000);              //LED2-ALED2VAL
        Afe4900_RegWrite(0x2F, 0x000000);              //LED1-ALED1VAL
        Afe4900_RegWrite(0x31, 0x000000);              //CONTROL3
        Afe4900_RegWrite(0x34, 0x000000);              //PROG_INT2_STC
        Afe4900_RegWrite(0x35, 0x000000);              //PROG_INT2_ENDC
        Afe4900_RegWrite(0x36, 0x000039);              //LED3LEDSTC
        Afe4900_RegWrite(0x37, 0x000049);              //LED3LEDENDC
        Afe4900_RegWrite(0x39, 0x000000);              //CLKDIV2
//	  Afe4900_RegWrite(0x3A, 0x184210);              //OFFDAC
        Afe4900_RegWrite(0x3A, (0x184210& (~(0x000001<<20))) | (afe4410_ppg_param_s.set_early_offset_loop<<20));              //OFFDAC
        Afe4900_RegWrite(0x3B, 0x000000);              //THRDETLOW
        Afe4900_RegWrite(0x3C, 0x200000);              //THRDETHIGH
        Afe4900_RegWrite(0x3D, 0x000000);              //THRDET
        Afe4900_RegWrite(0x3E, 0x000000);              //I_OFFDAC
        Afe4900_RegWrite(0x3F, 0x000000);              //AVG_LED2_ALED2VAL
        Afe4900_RegWrite(0x40, 0x000000);              //AVG_LED1_ALED1VAL
//	  Afe4900_RegWrite(0x42, 0x000000);              //FIFO
        Afe4900_RegWrite(0x42, 0x000000 | (4 << 6)|(0x02 << 4)|(0x00 << 0));
        Afe4900_RegWrite(0x43, 0x00005D);              //LED4LEDSTC
        Afe4900_RegWrite(0x44, 0x00006D);              //LED4LEDENDC
        Afe4900_RegWrite(0x45, 0x000000);              //TG_PD1STC
        Afe4900_RegWrite(0x46, 0x000000);              //TG_PD1ENDC
        Afe4900_RegWrite(0x47, 0x000000);              //TG_PD2STC
        Afe4900_RegWrite(0x48, 0x000000);              //TG_PD2ENDC
        Afe4900_RegWrite(0x49, 0x000000);              //TG_PD3STC
        Afe4900_RegWrite(0x4A, 0x000000);              //TG_PD3ENDC
        Afe4900_RegWrite(0x4B, 0x000000);              //CONTROL4
        Afe4900_RegWrite(0x4E, 0x000000);              //DUAL_PD
        Afe4900_RegWrite(0x50, 0x000008);              //CONTROL5
        Afe4900_RegWrite(0x51, 0x000000);              //FIFO_OFFSET
        Afe4900_RegWrite(0x52, 0x0000A1);              //DATA_RDY_STC
        Afe4900_RegWrite(0x53, 0x0000A1);              //DATA_RDY_ENDC
        Afe4900_RegWrite(0x57, 0x000000);              //PROG_INT1_STC
        Afe4900_RegWrite(0x58, 0x000000);              //PROG_INT1_ENDC
#ifdef AMB_DAC_MODE
        Afe4900_RegWrite(0x60, 0x000E38
                         | (afe4410_ppg_param_s.set_en_amb_loop<<11)
                         | (afe4410_ppg_param_s.set_choose_amb_phase<<9)
                         | (afe4410_ppg_param_s.set_freeze_loop<<5)
                         | (afe4410_ppg_param_s.set_hyst_loop<<3));
#else
        // FWT 20190222 CHANGE
        Afe4900_RegWrite(0x60, 0x000000);              //AMB_CANCELLATION
#endif
        Afe4900_RegWrite(0x64, 0x000000);              //DYN_TIA_STC
        Afe4900_RegWrite(0x65, 0x00009E);              //DYN_TIA_ENDC
        Afe4900_RegWrite(0x66, 0x000000);              //DYN_ADC_STC
        Afe4900_RegWrite(0x67, 0x00009E);              //DYN_ADC_ENDC
        Afe4900_RegWrite(0x68, 0x000000);              //DYN_CLOCK_STC
        Afe4900_RegWrite(0x69, 0x00009E);              //DYN_CLOCK_ENDC
        Afe4900_RegWrite(0x6A, 0x0000A8);              //DEEP_SLEEP_STC
        Afe4900_RegWrite(0x6B, 0x900000);              //DEEP_SLEEP_ENDC
        Afe4900_RegWrite(0x6C, 0x000000);              //PD_SHORT
        Afe4900_RegWrite(0x6D, 0x000000);              //REG_POINTER
        Afe4900_RegWrite(0x72, 0x000000);              //LED_DRIVER_CONTROL
        Afe4900_RegWrite(0x73, 0x000000);              //THR_DETECT_LOGIC
        Afe4900_RegWrite(0xC8, 0x000000);              //CLOCK_SETTINGS
        Afe4900_RegWrite(0x00, 0x000063);             //AFE4900READ MODE AND ULP

    // Afe4410_RegPpgParamSetDo();

    // 如果是夜间模式且心率自动测量开关打开 则需要打开绿灯

    // 设置绿光校准初始化参数
    hr_init_flag = 1;
    // 设置三路灯电流
    Elec_ModeWrite(1,(uint8_t)(INIT_ILED_SCALES_HR_CALIB));
    Elec_ModeWrite(2,(uint8_t)(INIT_ILED_SCALES));
    Elec_ModeWrite(3,(uint8_t)(INIT_ILED_SCALES));
    // 设置DAC量程
    //uint8_t set_dac_ifs = 0;
    uint8_t need_set_dac_ifs = 0;
    if(IFS_DAC == 1)
    {
        need_set_dac_ifs = 0;
    }
    if(IFS_DAC == 2)
    {
        need_set_dac_ifs = 3;
    }
    if(IFS_DAC == 4)
    {
        need_set_dac_ifs = 5;
    }
    if(IFS_DAC == 5)
    {
        need_set_dac_ifs = 7;
    }
    AFE4410_WriteDacReg(
        need_set_dac_ifs,		// dac 量程

        0,			// 绿光dac值
        0,			// 红光dac值
        0,			// 红外dac值
        0,		// 环境光dac值

        1,		// 绿光dac极性
        1,		// 红光dac极性
        1,		// 红外光dac极性
        1		// 环境光dac极性
    );

		
    // 读取已经设置的参数  供血氧算法使用
    AFE4410_ElecRead(
        &oxygen_detection.current_register_green,
        &oxygen_detection.current_register_red,
        &oxygen_detection.current_register_ired);

    AFE4410_RfRead(
        &oxygen_detection.rf_register_green,
        &oxygen_detection.rf_register_red,
        &oxygen_detection.rf_register_ired);

    AFE4410_DacRead(
        &oxygen_detection.dac_register_green,
        &oxygen_detection.dac_register_red,
        &oxygen_detection.dac_register_ired,
        &oxygen_detection.dac_pos_green,
        &oxygen_detection.dac_pos_red,
        &oxygen_detection.dac_pos_ired);
}

/*******************************************************************************************************
** Function name:        Afe4900_RegInitToRest
** Descriptions:         设备上电处理，消除DAC误差需求
** input parameter:      无   
** Returned Value:       无
** Created  by:          WJ
** Created  Datas:       2019-02-15
*********************************************************************************************************/
void Afe4900_RegInitToRest(void)
{  
    Afe4900_RegWrite(0x00, 0x000062);              //CONTROL0
    Afe4900_RegWrite(0x01, 0x00002B);              //LED2STC
    Afe4900_RegWrite(0x02, 0x000033);              //LED2ENDC
    Afe4900_RegWrite(0x03, 0x000043);              //LED1LEDSTC
    Afe4900_RegWrite(0x04, 0x00004F);              //LED1LEDENDC
    Afe4900_RegWrite(0x05, 0x000039);              //ALED2STC
    Afe4900_RegWrite(0x06, 0x000041);              //ALED2ENDC
    Afe4900_RegWrite(0x07, 0x000047);              //LED1STC
    Afe4900_RegWrite(0x08, 0x00004F);              //LED1ENDC
    Afe4900_RegWrite(0x09, 0x000027);              //LED2LEDSTC
    Afe4900_RegWrite(0x0A, 0x000033);              //LED2LEDENDC
    Afe4900_RegWrite(0x0B, 0x000055);              //ALED1STC
    Afe4900_RegWrite(0x0C, 0x00005D);              //ALED1ENDC
    Afe4900_RegWrite(0x0D, 0x000035);              //LED2CONVST
    Afe4900_RegWrite(0x0E, 0x000045);              //LED2CONVEND
    Afe4900_RegWrite(0x0F, 0x000047);              //ALED2CONVST
    Afe4900_RegWrite(0x10, 0x000057);              //ALED2CONVEND
    Afe4900_RegWrite(0x11, 0x000059);              //LED1CONVST
    Afe4900_RegWrite(0x12, 0x000069);              //LED1CONVEND
    Afe4900_RegWrite(0x13, 0x00006B);              //ALED1CONVST
    Afe4900_RegWrite(0x14, 0x00007B);              //ALED1CONVEND
    Afe4900_RegWrite(0x1D, 0x0009FF);              //PRPCOUNT
    Afe4900_RegWrite(0x1E, 0x000101);              //CONTROL1
    Afe4900_RegWrite(0x1F, 0x003535);              //TIAGAIN_2_3
    Afe4900_RegWrite(0x20, 0x008035);              //TIAGAIN
    Afe4900_RegWrite(0x21, 0x000035);              //TIA_AMB_GAIN
    Afe4900_RegWrite(0x22, 0x000000);              //LEDCNTRL1
    Afe4900_RegWrite(0x23, 0x12C218);              //CONTROL2
    Afe4900_RegWrite(0x24, 0x000000);              //LEDCNTRL2
    Afe4900_RegWrite(0x28, 0x000000);              //TOGGLE
    Afe4900_RegWrite(0x29, 0x000000);              //CLKDIV1
    Afe4900_RegWrite(0x2A, 0x000000);              //LED2VAL
    Afe4900_RegWrite(0x2B, 0x000000);              //ALED2VAL
    Afe4900_RegWrite(0x2C, 0x000000);              //LED1VAL
    Afe4900_RegWrite(0x2D, 0x000000);              //ALED1VAL
    Afe4900_RegWrite(0x2E, 0x000000);              //LED2-ALED2VAL
    Afe4900_RegWrite(0x2F, 0x000000);              //LED1-ALED1VAL
    Afe4900_RegWrite(0x31, 0x000420);              //CONTROL3
    Afe4900_RegWrite(0x34, 0x000000);              //PROG_INT2_STC
    Afe4900_RegWrite(0x35, 0x000000);              //PROG_INT2_ENDC
    Afe4900_RegWrite(0x36, 0x000035);              //LED3LEDSTC
    Afe4900_RegWrite(0x37, 0x000041);              //LED3LEDENDC
    Afe4900_RegWrite(0x39, 0x000000);              //CLKDIV2
    Afe4900_RegWrite(0x3A, 0x184210);              //OFFDAC
    Afe4900_RegWrite(0x3B, 0x000000);              //THRDETLOW
    Afe4900_RegWrite(0x3C, 0x000000);              //THRDETHIGH
    Afe4900_RegWrite(0x3D, 0x000000);              //THRDET
    Afe4900_RegWrite(0x3E, 0x000F00);              //I_OFFDAC
    Afe4900_RegWrite(0x3F, 0x000000);              //AVG_LED2_ALED2VAL
    Afe4900_RegWrite(0x40, 0x000000);              //AVG_LED1_ALED1VAL    

    Afe4900_RegWrite(0x42, 0x000000 | (4 << 6)|(0x02 << 4)|(0x00 << 0));    //FIFO

    Afe4900_RegWrite(0x43, 0x000051);              //LED4LEDSTC
    Afe4900_RegWrite(0x44, 0x00005D);              //LED4LEDENDC
    Afe4900_RegWrite(0x45, 0x000000);              //TG_PD1STC
    Afe4900_RegWrite(0x46, 0x000000);              //TG_PD1ENDC
    Afe4900_RegWrite(0x47, 0x000000);              //TG_PD2STC
    Afe4900_RegWrite(0x48, 0x000000);              //TG_PD2ENDC
    Afe4900_RegWrite(0x49, 0x000000);              //TG_PD3STC
    Afe4900_RegWrite(0x4A, 0x000000);              //TG_PD3ENDC
    Afe4900_RegWrite(0x4B, 0x000000);              //CONTROL4
    Afe4900_RegWrite(0x4E, 0x000000);              //DUAL_PD
    Afe4900_RegWrite(0x50, 0x000008);              //CONTROL5
    Afe4900_RegWrite(0x51, 0x000000);              //FIFO_OFFSET
    Afe4900_RegWrite(0x52, 0x000081);              //DATA_RDY_STC
    Afe4900_RegWrite(0x53, 0x000081);              //DATA_RDY_ENDC
    Afe4900_RegWrite(0x57, 0x000000);              //PROG_INT1_STC
    Afe4900_RegWrite(0x58, 0x000000);              //PROG_INT1_ENDC
    Afe4900_RegWrite(0x60, 0x000000);              //AMB_CANCELLATION
    Afe4900_RegWrite(0x64, 0x000000);              //DYN_TIA_STC
    Afe4900_RegWrite(0x65, 0x00007E);              //DYN_TIA_ENDC
    Afe4900_RegWrite(0x66, 0x000000);              //DYN_ADC_STC
    Afe4900_RegWrite(0x67, 0x00007E);              //DYN_ADC_ENDC
    Afe4900_RegWrite(0x68, 0x000000);              //DYN_CLOCK_STC
    Afe4900_RegWrite(0x69, 0x00007E);              //DYN_CLOCK_ENDC
    Afe4900_RegWrite(0x6A, 0x000088);              //DEEP_SLEEP_STC
    Afe4900_RegWrite(0x6B, 0x900000);              //DEEP_SLEEP_ENDC
    Afe4900_RegWrite(0x6C, 0x000000);              //PD_SHORT
    Afe4900_RegWrite(0x6D, 0x000000);              //REG_POINTER
    Afe4900_RegWrite(0x72, 0x000000);              //LED_DRIVER_CONTROL
    Afe4900_RegWrite(0x73, 0x000000);              //THR_DETECT_LOGIC
    Afe4900_RegWrite(0xC8, 0x000000);              //CLOCK_SETTINGS
    Afe4900_RegWrite(0x00, 0x000063);              //AFE4900READ MODE AND ULP
}

/*******************************************************************************************************
** Function name:        Afe4410_RegPpgParamSet
** Descriptions:         afe4410 通过app参数设置
** input parameter:      无
** Returned Value:       无
** Created  by:          WJ
** Created  Datas:       2019-01-13
*********************************************************************************************************/
void Afe4410_RegPpgParamSet(uint8_t *data_buf, uint8_t pack_num)
{
    if(data_buf == NULL)
    {
        return;
    }

    //主要协议发送的数据顺序，
    if(pack_num == 1)
    {
        memcpy(&afe4410_ppg_param_s, data_buf, 16);
    }
    else if(pack_num == 2)
    {
        memcpy(&afe4410_ppg_param_s.set_en_amb_loop, data_buf, 5);
    }
}

/*******************************************************************************************************
** Function name:        Afe4410_RegPpgParamGet
** Descriptions:         app获取afe4410参数
** input parameter:      无
** Returned Value:       无
** Created  by:          WJ
** Created  Datas:       2019-01-13
*********************************************************************************************************/
void Afe4410_RegPpgParamGet(uint8_t *data_buf, uint8_t pack_num)
{
    if(data_buf == NULL)
    {
        return;
    }    
    //主要协议发送的数据顺序，
    if(pack_num == 1)
    {
        memcpy(data_buf, &afe4410_ppg_param_s, 16);
    }
    else if(pack_num == 2)
    {
        memcpy(data_buf, &afe4410_ppg_param_s.set_en_amb_loop, 5);
    }
}

void Afe4410_RegPpgParamSetDo(void)
{        
    AFE4410_WriteDacReg(
                    afe4410_ppg_param_s.set_awdpr_ifs_dac,			

                    afe4410_ppg_param_s.set_awdpr_dac_gre,			
                    afe4410_ppg_param_s.set_awdpr_dac_red,			
                    afe4410_ppg_param_s.set_awdpr_dac_ird,			
                    afe4410_ppg_param_s.set_awdpr_dac_amb,			

                    afe4410_ppg_param_s.set_awdpr_dac_posi_gre,		
                    afe4410_ppg_param_s.set_awdpr_dac_posi_red,		
                    afe4410_ppg_param_s.set_awdpr_dac_posi_ird,		
                    afe4410_ppg_param_s.set_awdpr_dac_posi_amb);

    Tia_GainChange(afe4410_ppg_param_s.set_rf1_mid, 
                   afe4410_ppg_param_s.set_rf2_mid, 
                   afe4410_ppg_param_s.set_rf3_mid, 
                   afe4410_ppg_param_s.set_rf4_mid);

    Elec_Write(afe4410_ppg_param_s.set_i_led1, afe4410_ppg_param_s.set_i_led2, afe4410_ppg_param_s.set_i_led3);
}

