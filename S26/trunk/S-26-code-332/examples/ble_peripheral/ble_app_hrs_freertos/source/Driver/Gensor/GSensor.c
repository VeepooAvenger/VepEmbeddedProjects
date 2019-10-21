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
** Descriptions:               加速度
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

#define GSENSOR_INIT_CNTL1_VALUE    0x92    //启动加速的控制KX023_REG_CNTL1的值


static uint8_t gsensor_lock = 0;             //通讯锁定
static uint8_t gsensor_int_lock = 0;         //中断锁定
static uint8_t gsensor_stop_flag = 0;      //关闭加速度标记

void GSensor_Int1handler(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action);
void GSensor_CS_High(void);
void GSensor_CS_Low(void);

/****************************************************************
** Function name:             GSensor_read_reg
** Descriptions:              加速度读取单个寄存器
** input parameters:          reg_addr：          寄存器地址
** output parameters:         寄存器的值
** Returned value:            无
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
** Descriptions:              加速度读寄存器
** input parameters:          addr：           地址
                              value：          数据指针
                              len：            数据大小
** output parameters:         无
** Returned value:            无
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
** Descriptions:              加速度写寄存器
** input parameters:          addr：           地址
                              value：          数值
** output parameters:         无
** Returned value:            无
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
** Descriptions:              加速度不选中
** input parameters:          无
** output parameters:         无
** Returned value:            无
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
** Descriptions:              加速度选中
** input parameters:          无
** output parameters:         无
** Returned value:            无
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
** Descriptions:              加速度中断初始化
** input parameters:          无
** output parameters:         无
** Returned value:            无
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
** Descriptions:              加速度中断处理
** input parameters:          pin：         管脚
                              action：      管脚电平
** output parameters:         无
** Returned value:            无
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
    {   //锁定复位中
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
        // 1.移动侦测中断并且打开FIFO采集
        reg_value = GSensor_read_reg(KX023_REG_INS2);
        if(reg_value&0x02)
        {
			Battery_GiveMove();
            sport_hold_cnt = 0;
            // 将运动中断暂时关闭
            GSensor_write_reg(KX023_REG_INC4,0x60);
            // 关掉运动中断使能
        	if(GSensor_read_reg(KX023_REG_BUF_CNTL2)  != 0xC0)
            {
                GSensor_write_reg(KX023_REG_BUF_CNTL2,0xC0);  
                break;
            } 
        }
        
        // 如果FIFO满了但是没有运动中断,则关闭FIFO采集
        if(((reg_value&0x60) != 0)&&(sport_hold_cnt++ >= sport_hold_cnt_tmp))//5))//300))//40;60s/8s
        {
            // 中断恢复
            GSensor_write_reg(KX023_REG_INC4,0x62);
            GSensor_write_reg(KX023_REG_BUF_CNTL2,0x00); 
            break;            
        }
        
        // 2.读取FIFO的长度
        len = GSensor_read_reg(KX023_REG_BUF_STATUS1);
        if(len > 0)
        {
            len = (len > 60) ? 60 : len; 
            GSensor_read_regs(KX023_REG_BUF_READ,(uint8_t*)&acc_axis_data[0], len);
            
            //清除FIFO数据
            GSensor_write_reg(KX023_REG_BUF_CLEAR,0x00);
                
            // 更新数据处理
            DataManage_UpdateFIFOData(acc_axis_data,(int8_t)(len/6));
                
            AlgThread_GsensorEventCommit();
                                        
            if(((UPLOAD_ADC_GSENSOR_EVENT == p_data_manage->upload)&&(p_acc_data->adc_send_len >= 3)) ||
               ((UPLOAD_ADC_GESTURE_EVENT == p_data_manage->upload)&&(p_acc_data->gesture_wait_send_len >= 3)))
            {
                ComThread_SendADCData();
            }
        }        
    }while(0);
    
    // 清中断标志,读一下INT_REL寄存器就行
    GSensor_read_reg(KX023_REG_INT_REL);
    
    gsensor_lock = 0;
    gsensor_int_lock = 0;
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
    
    GSensor_write_reg(0x7F, 0x00);     
    GSensor_write_reg(KX023_REG_CNTL2, 0x00);   
    GSensor_write_reg(KX023_REG_CNTL2, 0x80);         //RAM重新启动
    
    //延时2ms
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
** Descriptions:              加速度配置
** input parameters:          无
** output parameters:         无
** Returned value:            无
** Created by:                WX
** Created Date:              2016-11-29
****************************************************************/ 
uint8_t data[2] = {0};
void GSensor_configure(void)
{        
    uint8_t cnt = 0;

    while(cnt++ < 5)
    {
        //acc软件复位
        if(Gsensor_SoftReset())
        {
            break;
        }
    }
    // !注意:在测试过程中发现,如果Gensor不断电,传感器值是不会恢复默认值 Add by GT 2017-06-20
    // 所以,在初始化时应该初始化所有的寄存器
    
    // 关闭加速度与唤醒失能
    GSensor_write_reg(KX023_REG_CNTL1, 0x00);  
    
    //读取出来的寄存器的值为0x15，代表加速度正常
    GSensor_read_regs(KX023_REG_WHO,(uint8_t*)&data[0], 1); 
    
    // CNTL2寄存器写入默认值
    GSensor_write_reg(KX023_REG_CNTL2, 0x00); 
    
    // 唤醒输出频率 唤醒输出率50hz 
    GSensor_write_reg(KX023_REG_CNTL3, 0x9F);     
    
    // 输出频率 输出率 0x02:50hz 0x03:100hz
    GSensor_write_reg(KX023_REG_ODCNTL, 0x02);//0x00);//0x01);//0x03);//
    
    //产生一个高电平直到读取INT_REL清除中断标志
    GSensor_write_reg(KX023_REG_INC1,0x30);  

    // 使能移动侦测中断使能
    GSensor_write_reg(KX023_REG_INC2,0x3F);  

    // tap/double中断使能关闭
    GSensor_write_reg(KX023_REG_INC3,0x3F); 

    // 中断触发方式设置(BUF full/WMI1/WUFI1三种方式)
    GSensor_write_reg(KX023_REG_INC4,0x62);

    // 设置INT2使能默认值
    GSensor_write_reg(KX023_REG_INC5,0x10);
    
    // 关闭INT2中断管脚所有的触发方式
    GSensor_write_reg(KX023_REG_INC6,0x00);
    
    //分辨率//8bit有效数据(高分辨率)
    GSensor_write_reg(KX023_REG_CNTL1,0x52);              
    
    // 运动唤醒配置//0.1*50 = 5
    GSensor_write_reg(KX023_REG_WUFC,0x19);              
    
    GSensor_write_reg(KX023_REG_ATH,0x01);              

    // 单次采样次数均值配置
    GSensor_write_reg(KX023_REG_LP_CNTL,0x00);
    
    // 缓存buf配置
    GSensor_write_reg(KX023_REG_BUF_CNTL1, 0x0A);  

    // 关闭FIFO采集使能
    GSensor_write_reg(KX023_REG_BUF_CNTL2, 0x00); 
    
    // 清除FIFO数据
    GSensor_write_reg(KX023_REG_BUF_CLEAR, 0xE0); 

    //清除中断
    GSensor_read_reg(KX023_REG_INT_REL);    
   
   //使能加速度
    GSensor_write_reg(KX023_REG_CNTL1,GSENSOR_INIT_CNTL1_VALUE);   
}

/****************************************************************
** Function name:             GSensor_AccInit
** Descriptions:              加速度初始化
** input parameters:          无
** output parameters:         无
** Returned value:            无
** Created by:                WX
** Created Date:              2016-11-29
****************************************************************/ 
void GSensor_AccInit(void)
{
//    Bps_Spi0Init();

    gsensor_lock = 0;
    gsensor_int_lock = 0;
	
    //引脚初始化
    GSensor_INTInit(); 
	
    nrf_delay_us(20000);
	
    //acc初始化
    GSensor_configure();
    
}

/****************************************************************
** Function name:             GSensor_AccStop
** Descriptions:              
** input parameters:          无
** output parameters:         无
** Returned value:            无
** Created by:                WX
** Created Date:              2016-11-29
****************************************************************/ 
void GSensor_AccStop(void)
{
	if(gsensor_lock == 1)
	{   // 重置关闭信号
        gsensor_stop_flag = 1;
        BleThread_GsensorRestart();
        return;
    }
    gsensor_lock = 1;
    
    // 关闭加速度与唤醒失能
    GSensor_write_reg(KX023_REG_CNTL1, 0x00);

    nrf_drv_gpiote_in_event_disable(GSENSOR_INIT1);	
    
    gsensor_stop_flag = 0;    
    gsensor_lock = 0;
    
}

void Thread_GSensorRestartTask(void)
{ 	
    // WJ.20181102.重新初始化加速度,防止加速度异常后不能通过开关机恢复
	if(gsensor_lock == 1)
	{
        //状态锁定重置信号-数据读取中，延后处理
        if(gsensor_int_lock == 1)
        {
            BleThread_GsensorRestart();
            return;
        }
	}
    
    // 再次关机
    if(gsensor_stop_flag == 1)
    {
        GSensor_AccStop();
        return;
    }
    
    // 再次开机
	gsensor_lock = 1;
    nrf_drv_gpiote_in_event_disable(GSENSOR_INIT1);	
    //acc初始化
    GSensor_configure();   
/*    
    //acc初始化
    GSensor_write_reg(KX023_REG_CNTL1,GSENSOR_INIT_CNTL1_VALUE);  
*/   
    //引脚初始化
    nrf_drv_gpiote_in_event_enable(GSENSOR_INIT1, true);
    
	gsensor_lock = 0;
}

/****************************************************************
** Function name:             Gensor_Immediate_ADC
** Descriptions:              加速度adc
** input parameters:          p_data：          数据指针
** output parameters:         无
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



