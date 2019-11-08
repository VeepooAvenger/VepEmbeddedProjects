

#include "I2C.h"
#include "tp_manage.h"
#include "nrf_drv_gpiote.h" 

#include "zinitix_touch.h"
#if TOUCH_ONESHOT_UPGRADE
#include "zinitix_touch_zxt_firmware.h"
#include "watch_dog.h"
#endif

#if 0
#define zinitix_printk(a,arg...) printk("[ zinitix ]" ": " a,##arg)
#else
#define zinitix_printk(arg...) 
#endif

#define ZINITIX_INT_PIN     TP_INT_PIN
//YX.20180514.增加TP测试功能
#define I2C_BUFFER_SIZE 2//64
#define X_NODE_NUM  18//1
#define Y_NODE_NUM  10//1//

//WJ.20180713.数据缓存大小
#define TP_DATA_CACHE_MAX_NUM   100

#define TP_USED_BSP_IIC     0   //是否使用硬件IIC


static uint16_t tp_product_data[X_NODE_NUM*Y_NODE_NUM*2] = {0};
//static uint16_t tp_product_data_min = 0;
//static uint16_t tp_product_data_max = 0;

uint8_t zinitix_ProductTest(void);

//static uint16_t	 ic_int_mask = 0;

static volatile uint16_t tpd_halt = 0;

static uint16_t chip_code = 0;

//uint8_t jstp_sleep_cnt = 0;
//uint8_t jstp_sleep_flg = 0;


TP_ALL_DATA_T data_cache[TP_DATA_CACHE_MAX_NUM] = {0};
uint8_t simul_data_cache_num = 0;

/****************************************************************
** Function name:               Tp_WriteCmd
** Descriptions:                TP写命令
** input parameters:            无
** output parameters:           无
** Returned value:              无
** Created by:                  YX     
** Created Date:                2018-06-07
****************************************************************/
uint8_t Tp_WriteCmd(uint16_t cmd)
{
    uint8_t ret = 0;
    uint8_t data[2] = {0};
    data[0] = cmd&0xff;
    data[1] = cmd>>8;
    
#if TP_USED_BSP_IIC    
    ret = Bsp_IIC0Write(0x20, data, 2);
#else
    ret = I2c1_Write(0x20, data, 2);
#endif    
    return ret;//I2C_SUCCESS;//I2C_FAIL
}

/****************************************************************
** Function name:               Tp_WriteReg
** Descriptions:                TP写寄存器
** input parameters:            无
** output parameters:           无
** Returned value:              无
** Created by:                  YX     
** Created Date:                2018-06-07
****************************************************************/
uint8_t Tp_WriteReg(uint16_t reg_addr, uint16_t buf)
{
    uint8_t ret = 0;
    uint8_t data[4] = {0};
    data[0] = reg_addr&0xff;
    data[1] = reg_addr>>8;
    data[2] = buf&0xff;
    data[3] = buf>>8;
    
#if TP_USED_BSP_IIC    
    ret = Bsp_IIC0Write(0x20, data, 4);
#else
    ret = I2c1_Write(0x20, data, 4);
#endif    
    
    return ret;//I2C_SUCCESS;//I2C_FAIL
}

/****************************************************************
** Function name:               Tp_ReadData
** Descriptions:                TP读取数据
** input parameters:            无
** output parameters:           无
** Returned value:              无
** Created by:                  YX     
** Created Date:                2018-06-07
****************************************************************/
uint8_t Tp_ReadData(uint16_t reg_addr, uint8_t * data, uint8_t len)
{
    uint8_t ret = 0;
    ret = Tp_WriteCmd(reg_addr);
    
#if TP_USED_BSP_IIC    
    ret = Bsp_IIC0Read(0x20, data, len);
#else
    ret = I2c1_Read(0x20, data, len);
#endif    
    
    return ret;//I2C_SUCCESS;//I2C_FAIL
}

/****************************************************************
** Function name:               Tp_WriteData
** Descriptions:                TP写数据
** input parameters:            无
** output parameters:           无
** Returned value:              无
** Created by:                  YX     
** Created Date:                2018-06-12
****************************************************************/
uint8_t Tp_WriteData(uint16_t reg_addr, uint8_t* data, uint8_t len)
{
    uint8_t ret = 0;
    uint8_t *data_tmp;
    
    data_tmp = (uint8_t*)malloc(len + 2);
    memcpy(&data_tmp[2], data, len);
    data_tmp[0] = reg_addr&0xff;
    data_tmp[1] = reg_addr>>8;
    
#if TP_USED_BSP_IIC    
    ret = Bsp_IIC0Write(0x20, data_tmp, len+2);
#else
    ret = I2c1_Write(0x20, data_tmp, len+2);
#endif    
    
    free(data_tmp);
    
    return ret;
}

void zinitix_power_control(bool enable)
{
    //VCC一直拉高，直接接在VDD上
    if(enable)
    {
        //使能
        nrf_delay_ms(1);
        ZINITIX_RST_HIGH();
        nrf_delay_ms(10);
        ZINITIX_RST_LOW();
        nrf_delay_ms(100);
        ZINITIX_RST_HIGH();
        nrf_delay_ms(CHIP_ON_DELAY);
    }
    else
    {
        //失能
        ZINITIX_RST_LOW();
        nrf_delay_ms(CHIP_OFF_DELAY);
    }
}

static bool ts_power_sequence(void)/*软件的上电序列*/
{
	uint8_t ret=0;

	ret=Tp_WriteReg(0xc000, 0x0001);/*往0xc000寄存器地址中写入0x0001值*/
	if(ret!=I2C_SUCCESS)
	{
		zinitix_printk("power sequence error (vendor cmd enable)\n");
		return false;
	}
	nrf_delay_us(10);
	
	ret=Tp_ReadData(0xcc00, (uint8_t *)&chip_code, 2) ;/*读0xcc00地址的值,芯片的ID 0xE628*/
	if(ret!=I2C_SUCCESS)
	{
		zinitix_printk("fail to read chip code\n");
		return false;
	}
	nrf_delay_us(10);
	zinitix_printk("chip code = 0x%x\n", chip_code);/*log打印出来  uint16_t chip_code*/
	
		
	ret=Tp_WriteCmd(0xc004);/*直接写入0xc004 command,不需要再写寄存器的值*/
	if(ret!=I2C_SUCCESS)
	{
		zinitix_printk("power sequence error (intn clear)\n");
		return false;
	}
	nrf_delay_us(10);
	
	ret=Tp_WriteReg(0xc002, 0x0001);
	if(ret!=I2C_SUCCESS)
	{
		zinitix_printk("power sequence error (nvm init)\n");
		return false;
	}
	nrf_delay_ms(2);
	ret =Tp_WriteReg(0xc001, 0x0001);
	if(ret!=I2C_SUCCESS)
	{
		zinitix_printk("power sequence error (program start)\n");
		return false;
	}
	nrf_delay_ms(FIRMWARE_ON_DELAY);/*delay*/
	zinitix_printk("ts_power_sequence PASS.\n");
	
	return true;

}
#if TOUCH_ONESHOT_UPGRADE

static bool ts_check_need_upgrade(uint16_t curRegVersion)
{
	uint16_t	newRegVersion;
	
	newRegVersion = (uint16_t) (m_firmware_data[60] | (m_firmware_data[61]<<8));
	
	zinitix_printk("cur reg data version = 0x%x, new reg data version = 0x%x\n",curRegVersion, newRegVersion);
	
//	if (curRegVersion < newRegVersion)
    if (curRegVersion != newRegVersion)
		return true;
	
	return false;
}

static bool ts_hw_calibration(void)
{
	uint16_t	chip_eeprom_info;
	int time_out = 0;
	int ret=0;
	
	ret=Tp_WriteReg(ZINITIX_TOUCH_MODE, 0x07);
	if(ret!=I2C_SUCCESS)
	{
		zinitix_printk("fail to set cali touch mode\n");
		return false;
	}
	nrf_delay_ms(10);
	Tp_WriteCmd(ZINITIX_CLEAR_INT_STATUS_CMD);
	nrf_delay_ms(10);
	Tp_WriteCmd(ZINITIX_CLEAR_INT_STATUS_CMD);	
	nrf_delay_ms(50);
	Tp_WriteCmd(ZINITIX_CLEAR_INT_STATUS_CMD);	
	nrf_delay_ms(10);
	ret=Tp_WriteCmd(ZINITIX_CALIBRATE_CMD);
	if(ret!=I2C_SUCCESS)
	{
		zinitix_printk("fail to write cali cmd\n");
		return false;
	}
	Tp_WriteCmd(ZINITIX_CLEAR_INT_STATUS_CMD);
	if(ret!=I2C_SUCCESS)
	{
		zinitix_printk("fail to clear int status \n");
		return false;
	}
	nrf_delay_ms(10);
	Tp_WriteCmd(ZINITIX_CLEAR_INT_STATUS_CMD);
	
	/* wait for h/w calibration*/
	do {
		nrf_delay_ms(500);
		Tp_WriteCmd(ZINITIX_CLEAR_INT_STATUS_CMD);			
		ret=Tp_ReadData(ZINITIX_EEPROM_INFO_REG,(uint8_t *)&chip_eeprom_info, 2) ;
		if(ret!=I2C_SUCCESS)
		{
			zinitix_printk("fail to read eeprom info\n");
			return false;
		}
		zinitix_printk("touch eeprom info = 0x%04X\r\n",chip_eeprom_info);

		if (!zinitix_bit_test(chip_eeprom_info, 0))
			break;
		if(time_out++ == 4)
		{
			Tp_WriteCmd(ZINITIX_CALIBRATE_CMD);
			nrf_delay_ms(10);
			Tp_WriteCmd(ZINITIX_CLEAR_INT_STATUS_CMD);						
			zinitix_printk("h/w calibration retry timeout.\n");
		}
		if(time_out++ > 10)
		{
			zinitix_printk("[error] h/w calibration timeout.\n");
			break;						
		}
	}while (1);
	
	Tp_WriteReg(ZINITIX_INITIAL_TOUCH_MODE, TOUCH_POINT_MODE);
	Tp_WriteReg(ZINITIX_TOUCH_MODE, TOUCH_POINT_MODE);
	if(ic_int_mask!=0)
	{
		ret=Tp_WriteReg( ZINITIX_INT_ENABLE_FLAG, ic_int_mask);
		if(ret!=I2C_SUCCESS)
		{
			zinitix_printk("fail to write int flag\n");
			return false;
		}
	}
	Tp_WriteReg(0xc003, 0x0001);
	Tp_WriteReg(0xc104, 0x0001);
	nrf_delay_us(100);
	
	ret=Tp_WriteCmd(ZINITIX_SAVE_CALIBRATION_CMD) ;
	if(ret!=I2C_SUCCESS)
	{
		zinitix_printk("fail to write save cali cmd\n");
		return false;
	}
	nrf_delay_ms(1000);	
	Tp_WriteReg( 0xc003, 0x0000);
	Tp_WriteReg( 0xc104, 0x0000);
	
	return true;
}

static uint8_t ts_upgrade_firmware(const uint8_t *firmware_data, uint32_t size)
{
	uint16_t flash_addr;
    uint32_t size_tmp;
	uint8_t *verify_data;
	int retry_cnt = 0;
	int i;
	int page_sz = 64;
	uint16_t chip_code;
	int ret = 0;
	uint8_t TC_SECTOR_SZ = 8;
	
    if(chip_code == 0xE700)
    {
        page_sz = 128;
        size = 64*1024;
    }
    else if((chip_code == 0xE548) || (chip_code == 0xE628))
    {
        size = 48*1024;
    }
    else if((chip_code == 0xE538) || (chip_code == 0xE532))
    {
        size = 44*1024;
    }
    
    size_tmp = size / 8;
    
//	verify_data = (uint8_t*)malloc(size);
    verify_data = (uint8_t*)malloc(size_tmp);
	if (verify_data == NULL) {
        nrf_delay_us(10);
	zinitix_printk(KERN_ERR "cannot alloc verify buffer\n");
	return false;
	}
	
retry_upgrade:
	
	zinitix_power_control(0);
	zinitix_power_control(1);
	
	ret=Tp_WriteReg(0xc000, 0x0001);
	if(ret!=I2C_SUCCESS)
	{
		zinitix_printk("power sequence error (vendor cmd enable)\n");
		goto fail_upgrade;
	}
	nrf_delay_us(10);
	ret=Tp_ReadData(0xcc00, (uint8_t *)&chip_code, 2);
	if(ret!=I2C_SUCCESS)
	{
		zinitix_printk("fail to read chip code\n");
		goto fail_upgrade;
	}
	zinitix_printk("chip code = 0x%x\n", chip_code);
	
	
	nrf_delay_us(10);	
	ret=Tp_WriteCmd(0xc004);
	if(ret!=I2C_SUCCESS)
	{
		zinitix_printk("power sequence error (intn clear)\n");
		goto fail_upgrade;
	}
	nrf_delay_us(10);
	
	ret=Tp_WriteReg(0xc002, 0x0001) ;
	if(ret!=I2C_SUCCESS)
	{
		zinitix_printk("power sequence error (nvm init)\n");
		goto fail_upgrade;
	}
	nrf_delay_ms(10);
	
	zinitix_printk("init flash\n");
	ret=Tp_WriteReg(0xc003, 0x0001) ;
	if(ret!=I2C_SUCCESS)
	{
		zinitix_printk("fail to write nvm vpp on\n");
		goto fail_upgrade;
	}
	nrf_delay_ms(1);
	
	
	ret=Tp_WriteReg(0xc104, 0x0001) ;
	if(ret!=I2C_SUCCESS)
	{
		zinitix_printk("fail to write nvm wp disable\n");
		goto fail_upgrade;
	}
	
	ret=Tp_WriteCmd(ZINITIX_INIT_FLASH) ;
	if(ret!=I2C_SUCCESS)
	{
		zinitix_printk("fail to init flash\n");
		goto fail_upgrade;
	}
	 nrf_delay_us(10);
    
    //增加擦出FLASH操作
    if(Tp_WriteCmd(0x01DF) != I2C_SUCCESS)
    {
        zinitix_printk("fail to mass erase\n");
		goto fail_upgrade;
    }
    nrf_delay_ms(100);
    //Mass Erase End
    if(Tp_WriteReg(0x01DE, 0x0001) != I2C_SUCCESS)
    {
        zinitix_printk("fail to enter upgrade mode\n");
		goto fail_upgrade;
    }
    nrf_delay_ms(1);
    if(Tp_WriteReg(0x01D3, 0x0008) != I2C_SUCCESS)
    {
        zinitix_printk("fail to init upgrade mode\n");
		goto fail_upgrade;
    }
    nrf_delay_ms(1);
    //擦出FLASH操作END
    
	zinitix_printk("writing firmware data\n");
	for (flash_addr = 0; flash_addr < size; ) 
	{
		//zinitix_printk(KERN_ERR  "Addr:0x%04x\n", flash_addr);
		for (i = 0; i < page_sz/TC_SECTOR_SZ; i++) 
		{
			ret=Tp_WriteData(ZINITIX_WRITE_FLASH,(uint8_t *)&firmware_data[flash_addr],TC_SECTOR_SZ);
			if(ret!=I2C_SUCCESS)
			{
				zinitix_printk(KERN_INFO"error : write zinitix tc firmare\n");
				goto fail_upgrade;
			}
			flash_addr += TC_SECTOR_SZ;
			nrf_delay_us(150);
		}
		nrf_delay_ms(30);	
        //YX.20180613.喂狗防止看门复位
        Wdt_Reload();
	
	}
	nrf_delay_ms(100);
	
	ret=Tp_WriteReg( 0xc003, 0x0001) ;
	if(ret!=I2C_SUCCESS)
	{
		zinitix_printk("fail to write nvm vpp on\n");
		goto fail_upgrade;
	}	
	
	ret=Tp_WriteReg( 0xc104, 0x0001) ;
	if(ret!=I2C_SUCCESS)
	{
		zinitix_printk("fail to write nvm wp disable\n");
		goto fail_upgrade;
	}
	
	ret=Tp_WriteCmd( ZINITIX_INIT_FLASH) ;
	if(ret!=I2C_SUCCESS)
	{
		zinitix_printk("fail to init flash\n");
		goto fail_upgrade;
	}
	
    for(uint8_t i_tmp = 0; i_tmp < 8; i_tmp++)
    {
        zinitix_printk("read firmware data\n");
        for (flash_addr = 0; flash_addr < size_tmp; ) 
        {
            for (i = 0; i < page_sz/TC_SECTOR_SZ; i++) 
            {
                //zinitix_printk("read :addr=%04x, len=%d\n", flash_addr, TC_SECTOR_SZ);
                ret=Tp_ReadData(ZINITIX_READ_FLASH,(uint8_t*)&verify_data[flash_addr], TC_SECTOR_SZ);
                if(ret!=I2C_SUCCESS)
                {
                    zinitix_printk("error : read zinitix tc firmare\n");
                    goto fail_upgrade;
                }
                flash_addr += TC_SECTOR_SZ;
            }
        }
        
        //YX.20180613.喂狗防止看门复位
        Wdt_Reload();
        
        /* verify */
        zinitix_printk("verify firmware data\n");
        if (memcmp((uint8_t *)&firmware_data[0+i_tmp*size_tmp], (uint8_t *)&verify_data[0], size_tmp) == 0) 
        {
            zinitix_printk("upgrade finished\n");
        }
        else
        {
            goto fail_upgrade;
        }
    }
    //正常验证完成，升级成功
    free(verify_data);
    
    zinitix_power_control(0);
    zinitix_power_control(1);
    ts_power_sequence();
    return true;
	
	fail_upgrade:
	
	zinitix_power_control(0);
	if (retry_cnt++ < ZINITIX_INIT_RETRY_CNT) 
	{
		zinitix_printk("upgrade fail : so retry... (%d)\n", retry_cnt);
		goto retry_upgrade;		
	}
	
	if (verify_data != NULL)
	free(verify_data);
	
	zinitix_printk("upgrade fail..\n");
	return false;
	
	
}
#endif


static bool ts_init_touch(void )/*初始化芯片的一些寄存器,寄存器地址的宏定义在后面*/
{
	int i = 0; 
	uint16_t firmware_version = 0;
	uint16_t reg_data_version = 0;
	int ret = 0;
    int retry_cnt = 0;
#if TOUCH_ONESHOT_UPGRADE
	uint16_t chip_check_sum;
	uint8_t checksum_err;
	uint32_t fw_size=0;
//	int retry_cnt = 0;
#endif


retry_init:

#if TOUCH_ONESHOT_UPGRADE
//fw check sum=0x55aa
	fw_size=sizeof(m_firmware_data);
	zinitix_printk("fw_size = %d KB\r\n", fw_size/1024);
	checksum_err = 0;

	for (i = 0; i < ZINITIX_INIT_RETRY_CNT; i++) 
	{
		if (Tp_ReadData(ZINITIX_CHECKSUM_RESULT, (uint8_t *)&chip_check_sum, 2) !=I2C_SUCCESS) {
			nrf_delay_ms(10);
			continue;
		}

		if(chip_check_sum == 0x55aa)
			break;
		else 
		{
//			checksum_err = 1;
			break;
		}
	}

	if ((i == ZINITIX_INIT_RETRY_CNT) || checksum_err) 
	{
		zinitix_printk("Failed to check firmware data\n");
		goto fail_init;
	}
#endif


	for ( i = 0; i < 10; i++) 
	{
		if (Tp_WriteCmd(ZINITIX_SWRESET_CMD) == I2C_SUCCESS)
			break;
		nrf_delay_ms(10);
	}
	ret=Tp_ReadData( ZINITIX_FIRMWARE_VERSION,(uint8_t *)&firmware_version, 2);/*固件主版本号uint16_t firmware_version;*/
	if(ret!=I2C_SUCCESS)
	{
		zinitix_printk("fail to read FIRMWARE_VERSION\n");
		goto fail_init;
	}
	zinitix_printk(" touch FW version = %d\r\n",firmware_version);
	
	ret=Tp_ReadData( ZINITIX_DATA_VERSION_REG,(uint8_t *)&reg_data_version, 2) ;/*固件寄存器版本号uint16_t reg_data_version;*/
	if(ret!=I2C_SUCCESS)
	{
		zinitix_printk("fail to read reg version\n");
		goto fail_init;
	}
	zinitix_printk(" touch register version = %d\r\n",reg_data_version);
	
#if TOUCH_ONESHOT_UPGRADE

	ret=Tp_ReadData( ZINITIX_INT_ENABLE_FLAG,(uint8_t *)&ic_int_mask, 2) ;
	if(ret!=I2C_SUCCESS)
	{
		zinitix_printk("fail to read int reg\n");
		goto fail_init;
	}
	zinitix_printk(" touch int reg = 0x%x\r\n",ic_int_mask);
	if (ic_int_mask == 0)
	{
		zinitix_printk(" touch ic_int_mask ==0 must set default 0x000F \r\n");
		ic_int_mask=0x000F;
	}

	if (ts_check_need_upgrade(reg_data_version) == true)
	{
		zinitix_printk("start upgrade firmware\n");

		if(ts_upgrade_firmware(m_firmware_data,fw_size) == 0)
		goto fail_init;

		if(ts_hw_calibration() == 0)
		goto fail_init;
		/* disable chip interrupt */
		ret=Tp_WriteReg( ZINITIX_INT_ENABLE_FLAG, 0);
		if(ret!=I2C_SUCCESS)
		{
			zinitix_printk("fail to write int flag\n");
			goto fail_init;
		}
		ret=Tp_ReadData( ZINITIX_FIRMWARE_VERSION,(uint8_t *)&firmware_version, 2);/*固件主版本号uint16_t firmware_version;*/
		if(ret!=I2C_SUCCESS)
		{
			zinitix_printk("fail to read FIRMWARE_VERSION\n");
			goto fail_init;
		}
		zinitix_printk(" touch FW version = %d\r\n",firmware_version);

		ret=Tp_ReadData( ZINITIX_DATA_VERSION_REG,(uint8_t *)&reg_data_version, 2) ;/*固件寄存器版本号uint16_t reg_data_version;*/
		if(ret!=I2C_SUCCESS)
		{
			zinitix_printk("fail to read reg version\n");
			goto fail_init;
		}
		zinitix_printk(" touch register version = %d\r\n",reg_data_version);

	}
#endif


	ret=Tp_WriteReg( ZINITIX_INITIAL_TOUCH_MODE, TOUCH_POINT_MODE);
	if(ret!=I2C_SUCCESS)
	{
		zinitix_printk("fail to write inital touch mode\n");
		goto fail_init;
	}
	
	ret=Tp_WriteReg( ZINITIX_TOUCH_MODE, TOUCH_POINT_MODE);
	if(ret!=I2C_SUCCESS)
	{
		zinitix_printk("fail to write touh mode\n");
		goto fail_init;
	}
	
	ret=Tp_WriteReg(ZINITIX_SUPPORTED_FINGER_NUM,(uint16_t)MAX_SUPPORTED_FINGER_NUM);
	if(ret!=I2C_SUCCESS)
	{
		zinitix_printk("fail to write finger num\n");
		goto fail_init;
	}
	
	ret=Tp_WriteReg(ZINITIX_X_RESOLUTION,(uint16_t)(TPD_RES_MAX_X));
	if(ret!=I2C_SUCCESS)
	{
		zinitix_printk("fail to write x resolution\n");
		goto fail_init;
	}
	
	ret=Tp_WriteReg(ZINITIX_Y_RESOLUTION,(uint16_t)(TPD_RES_MAX_Y));
	if(ret!=I2C_SUCCESS)
	{
		zinitix_printk("fail to write Y resolution\n");
		goto fail_init;
	}
    
	ret=Tp_WriteCmd( ZINITIX_CALIBRATE_CMD);
	if(ret!=I2C_SUCCESS)
	{
		zinitix_printk("fail to write cali cmd\n");
		goto fail_init;
	}
#if TOUCH_ONESHOT_UPGRADE

	ret=Tp_WriteReg( ZINITIX_INT_ENABLE_FLAG, ic_int_mask);
		if(ret!=I2C_SUCCESS)
		{
			zinitix_printk("fail to write int flag\n");
			goto fail_init;
		}
#endif
        
    //YX.20180703.设置唤醒模式的一些参数
    ret=Tp_WriteReg(REG_GESTURE_SLIDE_DISTANCE  ,(uint16_t)(GESTURE_SLIDE_DISTANCE));
	if(ret!=I2C_SUCCESS)
	{
		zinitix_printk("fail to write wakeup mode\n");
		goto fail_init;
	}
    
    ret=Tp_WriteReg(REG_GESTURE_LONG_PRESS_MIN  ,(uint16_t)(GESTURE_LONG_PRESS_MIN));
	if(ret!=I2C_SUCCESS)
	{
		zinitix_printk("fail to write wakeup mode\n");
		goto fail_init;
	}
    
    ret=Tp_WriteReg(REG_GESTURE_LONG_PRESS_MAX  ,(uint16_t)(GESTURE_LONG_PRESS_MAX));
	if(ret!=I2C_SUCCESS)
	{
		zinitix_printk("fail to write wakeup mode\n");
		goto fail_init;
	}
    
	for (i = 0; i < 10; i++) 
	{
		Tp_WriteCmd(ZINITIX_CLEAR_INT_STATUS_CMD);
		nrf_delay_us(10);
	}
	zinitix_printk("successfully initialized\r\n");
	return true;

fail_init:
	zinitix_printk("enter failed initialized\r\n");

	if (++retry_cnt <= ZINITIX_INIT_RETRY_CNT) 
	{
		zinitix_power_control(0);
		zinitix_power_control(1);
		ts_power_sequence();
		zinitix_printk("retry to initiallize(retry cnt = %d)\r\n",retry_cnt);
		goto  retry_init;
	} 
	else if(retry_cnt == ZINITIX_INIT_RETRY_CNT+1) 
	{
		zinitix_printk("retry to initiallize(retry cnt = %d)\r\n", retry_cnt);
		
#if TOUCH_ONESHOT_UPGRADE/*force upgrade here*/
		if(ts_upgrade_firmware(m_firmware_data,fw_size) == false)
		{
			zinitix_printk("retry upgrade fail\r\n");
			return false;
		}

		if(ts_hw_calibration() == false)
		{
			zinitix_printk("retry calibration fail\r\n");
			return false;
		}
		
		goto retry_init;

#endif
    
    }

	return false;


}


//void Tp_DataPro(void);

void tpd_touchinfo(void)/*中断报点函数*/
{
	int ret = 0;
#if 0/*if you want support gesture wakeup ,open this*/
#if !USE_WAKEUP_GESTURE	
	 if(tpd_halt) {
	 	
		 zinitix_printk("zinitix suspend halt status...\n");
		 ret=0;
		
		 goto exit;
		 }
#else
	 uint16_t gesture_flag=0;

	if(tpd_halt) //suspend state support gesture
	{
	      
		if (Tp_ReadData(0x126, (u8 *)&gesture_flag, 2) !=I2C_SUCCESS) //0x115
		{
			zinitix_printk(" gesture read reg error!!!\r\n");
			
			ret=0;
			
		}
		else
		{			
			Tp_WriteCmd(ZINITIX_CLEAR_INT_STATUS_CMD);

			zinitix_printk(" gesture wakeup flag:%d\r\n",gesture_flag);			
			if(gesture_flag==2)/*2 double click*/
			{
			/* send message to wakeup*/
				input_report_key(tpd->dev, KEY_F1, 1);
				input_sync(tpd->dev);
				input_report_key(tpd->dev, KEY_F1, 0);
				input_sync(tpd->dev);
                     /*send message end*/


				if (Tp_WriteReg(0x126, 0) != I2C_SUCCESS)
				{
					zinitix_printk(" gesture write reg error!!!\r\n");
					
					ret=0;
				}
		
			}			
		
		}
		
		goto exit;
	}

#endif
#endif


#ifdef TPD_HIGH_SPEED_DMA  
	/*I2C dma mode 可以一次读很多个字节，需要平台支持，Android平台都支持*/
//    read_len = sizeof(struct _ts_zinitix_point_info);
    
    //YX.20180703.增加状态来互斥TP中断的通信和线程中调用功能的通信
    if(Tp_GetI2cStatus())
    {
        //YX.20180801.增加清除中断标志位的标志，在TP不被占用的时候清除中断标志位
        Tp_SetI2cIntStatus(1);
//        /*clear中断，clear后中断会被拉高*/
//	Tp_WriteCmd( ZINITIX_CLEAR_INT_STATUS_CMD);
        return;
    }
    //Tp_SetI2cStatus(1);
    //用来判断是否属于触摸唤醒TP
    Tp_IsSleepWakeup();
    
	ret=Tp_ReadData( ZINITIX_POINT_STATUS_REG, (uint8_t *)&(touch_info), sizeof(struct _ts_zinitix_point_info)); 
    if(ret!=I2C_SUCCESS)
	{
		zinitix_printk("fail to read point reg\n");
		goto exit;
	}
    //WJ.20180713.使用宏定义
    if(simul_data_cache_num >= TP_DATA_CACHE_MAX_NUM)//30)
    {
        goto exit;
    }
    else
    {
        if(touch_info.coord[0].sub_status == 0x03)
        {
            data_cache[simul_data_cache_num].status = TP_START_STATE;
        }
        else if(touch_info.coord[0].sub_status == 0x08)
        {
            data_cache[simul_data_cache_num].status = TP_STOP_STATE;
        }
        else
        {
            data_cache[simul_data_cache_num].status = touch_info.coord[0].sub_status;
        }
        data_cache[simul_data_cache_num].coord.x = touch_info.coord[0].x;
        data_cache[simul_data_cache_num].coord.y = touch_info.coord[0].y;
        if(data_cache[simul_data_cache_num++].status == TP_STOP_STATE)
        {
            Tp_GestureProcess(data_cache,simul_data_cache_num,0);
    //        Tp_DataPro();
        }
    }
#else /*非dma mode 平台一般只允许最多一次读8个字节*/

	/*多点触控第一个点的信息*/
	ret=Tp_ReadData(ZINITIX_POINT_STATUS_REG,(uint8_t *)(&touch_info),8);
	if(ret!=I2C_SUCCESS)
	{
		zinitix_printk("fail to read point reg\n");
		goto exit;
	}
	
	ret=Tp_ReadData(ZINITIX_POINT_STATUS_REG+4,((uint8_t *)&touch_info + 8),2);
	 if(ret!=I2C_SUCCESS)
	{
		zinitix_printk("fail to read point reg\n");
		goto exit;
	}
	/*后续点的信息*/
	for (uint8_t i = 1; i < MAX_SUPPORTED_FINGER_NUM; i++) 
	{
		if (zinitix_bit_test(touch_info.event_flag, i)) 
		{

			ret=Tp_ReadData( ZINITIX_POINT_STATUS_REG + 2 + (i * 4),(uint8_t *)(&touch_info.coord[i]),sizeof(struct _ts_zinitix_coord)); 
			if(ret!=I2C_SUCCESS)
			{
				zinitix_printk("fail to read point reg\n");
				goto exit;
			}

		}
	} 
#endif
	 zinitix_printk("touch_info.bit_status=0x%x\n", touch_info.bit_status);
	if (touch_info.status == 0x0) 
	{
		zinitix_printk("periodical_interrupt\r\n"); 
		goto exit;
	}
	/*down up 事件的判断*/
	for (uint8_t i = 0; i < MAX_SUPPORTED_FINGER_NUM; i++) 
	{        
		if (zinitix_bit_test(touch_info.coord[i].sub_status, SUB_BIT_DOWN)
		|| zinitix_bit_test(touch_info.coord[i].sub_status, SUB_BIT_MOVE)
		|| zinitix_bit_test(touch_info.coord[i].sub_status, SUB_BIT_EXIST))
		{

//			uint16_t x = touch_info.coord[i].x;
//			uint16_t y = touch_info.coord[i].y;               
//			zinitix_printk("finger dwon [%02d] x = %d, y = %d \r\n", i, x, y);
			
//			Send_down_event(i,x,y);/*平台上报down事件函数*/

		}
		else if (zinitix_bit_test(touch_info.coord[i].sub_status, SUB_BIT_UP)) 
		{
			zinitix_printk("finger [%02d] up \r\n", i);
			
//			Send_up_event(i);/*平台上报up事件函数*/
		}
		else
		{
			memset(&touch_info.coord[i], 0x0, sizeof(struct _ts_zinitix_coord));

		}
	}
exit:
	/*clear中断，clear后中断会被拉高*/
	Tp_WriteCmd( ZINITIX_CLEAR_INT_STATUS_CMD);
    
    //Tp_SetI2cStatus(0);
	
}

void zinitix_init(void)
{
    uint32_t err_code = NRF_SUCCESS;
    
    // GPIOTE驱动注册
    if(!nrf_drv_gpiote_is_init())
    {
        err_code = nrf_drv_gpiote_init();    
        APP_ERROR_CHECK(err_code);
    }
    
    nrf_drv_gpiote_in_config_t in_config = GPIOTE_CONFIG_IN_SENSE_HITOLO(false);    //high to low 
    in_config.pull = NRF_GPIO_PIN_NOPULL;            //PULL UP

    nrf_drv_gpiote_in_uninit(ZINITIX_INT_PIN);
    
    //TP
	zinitix_power_control(1);
	ts_power_sequence();
	ts_init_touch();
    
//    zinitix_ProductTest();
//    nrf_gpio_cfg_input(ZINITIX_INT_PIN, NRF_GPIO_PIN_PULLUP);

    
    err_code = nrf_drv_gpiote_in_init(ZINITIX_INT_PIN, &in_config, (nrf_drv_gpiote_evt_handler_t)tpd_touchinfo);
    APP_ERROR_CHECK(err_code);
    
    nrf_drv_gpiote_in_event_enable(ZINITIX_INT_PIN, true);

}

void zinitix_shutdown(void)
{
    zinitix_power_control(0);
    
    nrf_drv_gpiote_in_uninit(ZINITIX_INT_PIN);
    
    //nrf_drv_gpiote_in_event_disable(ZINITIX_INT_PIN);

}

void zinitix_suspend(uint16_t mode_data)
{
	int i;
    int ret = 0;
    
    //YX.20180703.设置唤醒方式
    ret=Tp_WriteReg(ZINITIX_INTERNAL_FLAG_03  ,(uint16_t)(mode_data));
	if(ret!=I2C_SUCCESS)
	{
		zinitix_printk("fail to write wakeup mode\n");
		return ;
	}
    
	nrf_delay_ms(10);
	Tp_WriteCmd(0x000A);
	nrf_delay_ms(10);
	for(i = 0; i <ZINITIX_INIT_RETRY_CNT; i++) {	   
		if(Tp_WriteCmd(ZINITIX_CLEAR_INT_STATUS_CMD)!=I2C_SUCCESS) {
			zinitix_printk("tpd_resume fail to send wakeup_cmd\r\n");
			nrf_delay_ms(10);
	              continue;
		} else
			break;
	}
	for(i = 0; i <ZINITIX_INIT_RETRY_CNT; i++) {	   
		if(Tp_WriteCmd(ZINITIX_SLEEP_CMD)!=I2C_SUCCESS) {
			zinitix_printk("tpd_resume fail to send wakeup_cmd\r\n");
			nrf_delay_ms(10);
	              continue;

		} else
			break;
	}

	Tp_WriteCmd(0x000B);
	nrf_delay_ms(10);

	tpd_halt=1;


	zinitix_printk("TP_suspend\r\n");
}

void zinitix_resume(void)
{
	int i;
	Tp_WriteCmd(0x000A);
	nrf_delay_ms(10);
	for(i = 0; i <ZINITIX_INIT_RETRY_CNT; i++) {	   
		if(Tp_WriteCmd(ZINITIX_WAKEUP_CMD)!=I2C_SUCCESS) {
			zinitix_printk("tpd_resume fail to send wakeup_cmd\r\n");
			nrf_delay_ms(10);
		} else
			break;
	}
	if (i==ZINITIX_INIT_RETRY_CNT) {  
		Tp_Init(2);/*重新初始化*/
		goto reset_exit;
	}
	nrf_delay_ms(1);
	Tp_WriteCmd(ZINITIX_SWRESET_CMD);
	nrf_delay_us(20);
	for(i=0; i<10; i++)
	{
		Tp_WriteCmd(ZINITIX_CLEAR_INT_STATUS_CMD);
		nrf_delay_ms(10);
	}
	Tp_WriteCmd(0x000B);
	nrf_delay_ms(10);

	tpd_halt=0;

	zinitix_printk("TP_resume\r\n");

reset_exit:
	return;
}

void zinitix_ClearINT(void)
{
    /*clear中断，clear后中断会被拉高*/
	Tp_WriteCmd( ZINITIX_CLEAR_INT_STATUS_CMD);
}

//YX.20180514.增加TP测试功能
static uint8_t get_raw_data(uint8_t *buff, uint16_t skip_cnt)
{
    uint32_t sz = 0;
    uint32_t temp_sz = 0;
    
	Tp_WriteCmd(0x000A);
    nrf_delay_ms(10);
    
    for(uint16_t i = 0; i < skip_cnt; i++) 
    {
        Tp_WriteCmd(ZINITIX_CLEAR_INT_STATUS_CMD);
        nrf_delay_ms(1);
    }
    
    Tp_WriteCmd(ZINITIX_CLEAR_INT_STATUS_CMD);
    nrf_delay_ms(10);
	
    sz = X_NODE_NUM*Y_NODE_NUM*2;
	for(uint8_t i = 0; sz > 0; i++){
		temp_sz = I2C_BUFFER_SIZE;

		if(sz < I2C_BUFFER_SIZE)
			temp_sz = sz;

		if (Tp_ReadData(ZINITIX_RAWDATA_REG + i, (uint8_t *)(buff + (i*I2C_BUFFER_SIZE)), temp_sz)!=I2C_SUCCESS) {
			Tp_WriteCmd(0x000B);
            nrf_delay_ms(10);
            return 0;
		}
		sz -= I2C_BUFFER_SIZE;
	}
    
    Tp_WriteCmd(ZINITIX_CLEAR_INT_STATUS_CMD);
	Tp_WriteCmd(0x000B);
	nrf_delay_ms(10);

	return 1;
}

uint8_t zinitix_ProductTest(void)
{
    uint16_t max = 0;
    uint16_t min = 0;
    //写模式11开始
    Tp_WriteCmd(0x000A);
    nrf_delay_ms(10);
    Tp_WriteCmd(ZINITIX_SWRESET_CMD);
    nrf_delay_ms(10);
    for(uint8_t retry = 0; retry < 10; retry++) 
    {
        if (Tp_WriteReg(ZINITIX_TOUCH_MODE, 11) == I2C_SUCCESS)
        {
            break;
        }
        nrf_delay_ms(1);
    }
    Tp_WriteCmd(ZINITIX_SWRESET_CMD);
    nrf_delay_ms(25);
    Tp_WriteCmd(ZINITIX_SWRESET_CMD);
    nrf_delay_ms(25);
        
    Tp_WriteReg(ZINITIX_DND_N_COUNT, SEC_DND_N_COUNT);
    Tp_WriteReg(ZINITIX_AFE_FREQUENCY, SEC_DND_FREQUENCY);
    Tp_WriteReg(ZINITIX_DND_U_COUNT, SEC_DND_U_COUNT);
//    Tp_WriteReg(0x2D8, 2);
//    Tp_WriteReg(0x2DA, 80);
//    Tp_WriteReg(0x2D9, 13);

    Tp_WriteReg(ZINITIX_DELAY_RAW_FOR_HOST, RAWDATA_DELAY_FOR_HOST);

    Tp_WriteReg(ZINITIX_TOUCH_MODE, 11);

    for(uint8_t i = 0; i < 10; i++) 
    {
        nrf_delay_ms(20);
        Tp_WriteCmd(ZINITIX_SWRESET_CMD);
    }

    Tp_WriteCmd(0x000B);
    nrf_delay_ms(10);
    //写模式11结束
    
    get_raw_data((uint8_t *)tp_product_data, 10);
    
    //写模式TOUCH_POINT_MODE开始
    Tp_WriteCmd(0x000A);
    nrf_delay_ms(10);
	
	Tp_WriteReg(ZINITIX_TOUCH_MODE, TOUCH_POINT_MODE);
    for(uint8_t i = 0; i < 10; i++) 
    {
		nrf_delay_ms(20);
        Tp_WriteCmd(ZINITIX_CLEAR_INT_STATUS_CMD);
    }
    Tp_WriteCmd(0x000B);
    nrf_delay_ms(10);
   //写模式TOUCH_POINT_MODE结束
    
    min = 0xFFFF;
	max = 0x0000;

	for(uint8_t i = 0; i < X_NODE_NUM; i++)
	{
		for(uint8_t j = 0; j < Y_NODE_NUM; j++)
		{
			if (tp_product_data[i * Y_NODE_NUM + j] < min && tp_product_data[i * Y_NODE_NUM + j] != 0)
            {
				min = tp_product_data[i * Y_NODE_NUM + j];
            }
			if (tp_product_data[i * Y_NODE_NUM + j] > max)
            {
				max = tp_product_data[i * Y_NODE_NUM + j];
            }
		}
	}
//    tp_product_data_min = min;
//    tp_product_data_max = max;
    
    return 1;
}

