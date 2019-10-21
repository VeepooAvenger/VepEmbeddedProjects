#include "AFExx_Calib.h"

#define setbit(x,y)										   (x |= (1 << y))  
#define clrbit(x,y)										   (x &= ~(1 << y)) 

#define LOW_LINE                                           (1200000)
#define HIGH_LINE                                          (1700000)

#define MIN_LINE                                           (0.1f)
#define MAX_LINE                                           (0.9f)

#define WAITING_NUM										   (10)
// 微安
#define ADC_1CODE_CONVERT_TO_HOW_MANY_IPELTH_IN_10KRF      (0.0286102294921875)

// 0: sInitialize 1: sAmbientDAC 2: sGain 3: sPeriodic 4: sFinish
uint8_t calibration_mode = 0;

uint8_t wait_cnt = 0;
uint8_t red_negative_flag = 0;
uint8_t ird_negative_flag = 0;
uint8_t red_da_fall_in_to_loop_flag = 0;
uint8_t ird_da_fall_in_to_loop_flag = 0;

float   gre_adc_means = 0.0f;
float   red_adc_means = 0.0f;
float   ird_adc_means = 0.0f;
float   amb_adc_means = 0.0f;
// =================================================================//
uint8_t
i_gre = 0,
i_red = 0,
i_ird = 0;
// =================================================================//
// ensepgain(1: allow double gain(red&ird and gre&amb) 0: allow only one gain(all in))
// ensepgain4(1: allow 4 gain  0: not allow 4 gain)
uint8_t
ensepgain = 0,
ensepgain4 = 0;

uint8_t
tia_cf = 0,
tia_cf_sep = 0,
tia_cf_sep2 = 0,
tia_cf_sep3 = 0,

tia_gain = 0,
tia_gain_sep = 0,
tia_gain_sep2 = 0,
tia_gain_sep3 = 0;

float
tia_gain_sep_last = 0.0f,
tia_gain_sep2_last = 0.0f;
// =================================================================//
uint8_t
dac_gre = 0,
dac_red = 0,
dac_ird = 0,
dac_amb = 0,
// one is nagitive
posi_dac_gre = 0,
posi_dac_red = 0,
posi_dac_ird = 0,
posi_dac_amb = 0;

// ifs_offdac  0:max value(15.875uA)   3:max value(31.75uA)   5:max value(63.5uA)   7:max value(127uA)
uint8_t ifs_dac = 0;
uint8_t ifs_dac_per_value[4] = { 0.125f,0.25f,0.5f,1.0f };

uint32_t ird_i_last = 0;
uint32_t red_i_last = 0;
// =================================================================//
int32_t red_da_last = 0;
int32_t ird_da_last = 0;
// =================================================================//
void AFE4410_Reg_Init(void)
{
	AFE4410_Reg_Write(0, 0x20);	//ENABLE ULP mode
	AFE4410_Reg_Write(1, 14);     //AFE_LED2STC
	AFE4410_Reg_Write(2, 18);    	//AFE_LED2ENDC
	AFE4410_Reg_Write(3, 31);    	//AFE_LED1LEDSTC
	AFE4410_Reg_Write(4, 36);   	//AFE_LED1LEDENDC
	AFE4410_Reg_Write(5, 23);    	//AFE_ALED2STC
	AFE4410_Reg_Write(6, 27);    	//AFE_ALED2ENDC
	AFE4410_Reg_Write(7, 32);    	//AFE_LED1STC
	AFE4410_Reg_Write(8, 36);   	//AFE_LED1ENDC
	AFE4410_Reg_Write(9, 13);     //AFE_LED2LEDSTC
	AFE4410_Reg_Write(10, 18);   	//AFE_LED2LEDENDC
	AFE4410_Reg_Write(11, 41);  	//AFE_ALED1STC
	AFE4410_Reg_Write(12, 45);  	//AFE_ALED1ENDC
	AFE4410_Reg_Write(13, 19);   	//AFE_LED2CONVST
	AFE4410_Reg_Write(14, 34);  	//AFE_LED2CONVEND
	AFE4410_Reg_Write(15, 35);  	//AFE_ALED2CONVST
	AFE4410_Reg_Write(16, 50);  	//AFE_ALED2CONVEND
	AFE4410_Reg_Write(17, 51);  	//AFE_LED1CONVST
	AFE4410_Reg_Write(18, 66);  	//AFE_LED1CONVEND
	AFE4410_Reg_Write(19, 67);  	//AFE_ALED1CONVST
	AFE4410_Reg_Write(20, 82);  	//AFE_ALED1CONVEND
	AFE4410_Reg_Write(21, 3000);  //AFE_ADCRSTSTCT0
	AFE4410_Reg_Write(22, 3000);  //AFE_ADCRSTENDCT0
	AFE4410_Reg_Write(23, 3000);  //AFE_ADCRSTSTCT1
	AFE4410_Reg_Write(24, 3000);  //AFE_ADCRSTENDCT1
	AFE4410_Reg_Write(25, 3000);  //AFE_ADCRSTSTCT2
	AFE4410_Reg_Write(26, 3000);  //AFE_ADCRSTENDCT2
	AFE4410_Reg_Write(27, 3000);  //AFE_ADCRSTSTCT3
	AFE4410_Reg_Write(28, 3000);  //AFE_ADCRSTENDCT3

	AFE4410_Reg_Write(29, 2559); 	//AFE_PRPCOUNT

	AFE4410_Reg_Write(30, 0x000100);      //AFE_CONTROL1 TimerEN = 1; NUMAV = 0

	AFE4410_Reg_Write(32, 0x000000);      //AFE_TIA_SEP_GAIN (LED2) ENSEPGAIN = 0; LED2/LED3 gain = 25K
	AFE4410_Reg_Write(33, 0x000005);      //AFE_TIA_GAIN (LED1) LED1/LED1AMB gain = 25K


	AFE4410_Reg_Write(58, 0x000000);      //AFE_DAC_SETTING_REG
	AFE4410_Reg_Write(34, 0x0030C3);      //LED3 ; LED2 ; LED1
										  //CONTROL_DYN_TX(0), LEDCurr = 100mA, CONTROL_DYN_BIAS, ENABLE_OSC = 0, CONTROL_DYN_TIA, CONTROL_DYN_ADC
	AFE4410_Reg_Write(35, 0x124218);      //AFE_CONTROL2
	AFE4410_Reg_Write(75, 0x0F); 			//CONTROL_DYN_VCM / CONTROL_DYN_DLDO / CONTROL_DYN_ALDO / CONTROL_DYN_BG
	AFE4410_Reg_Write(80, 0x08); 			//CONTROL_DYN_TX(1)

	AFE4410_Reg_Write(49, 0x20); 			//ENABLE_INPUT_SHORT = 1

	AFE4410_Reg_Write(50, 0);  			//PDNCYCLESTC
	AFE4410_Reg_Write(51, 0); 			//PDNCYCLEENDC

	AFE4410_Reg_Write(54, 22);   			//AFE_LED3LEDSTC
	AFE4410_Reg_Write(55, 27);   			//AFE_LED3LEDENDC

	AFE4410_Reg_Write(57, 0);     		//CLKDIV_PRF

	AFE4410_Reg_Write(67, 40);   			//AFE_LED4LEDSTC
	AFE4410_Reg_Write(68, 45);   			//AFE_LED4LEDENDC

	AFE4410_Reg_Write(82, 92); 			//DATA_RDY_STC
	AFE4410_Reg_Write(83, 92); 			//DATA_RDY_ENDC

	AFE4410_Reg_Write(100, 0); 			//DYN_TIA_STC
	AFE4410_Reg_Write(101, 49); 			//DYN_TIA_ENDC
	AFE4410_Reg_Write(102, 6); 			//DYN_ADC_STC
	AFE4410_Reg_Write(103, 88); 			//DYN_ADC_ENDC
	AFE4410_Reg_Write(104, 6); 			//DYN_CLOCK_STC
	AFE4410_Reg_Write(105, 88); 			//DYN_CLOCK_ENDC
	AFE4410_Reg_Write(106, 114); 			//DEEP_SLEEP_STC
	AFE4410_Reg_Write(107, 2507); 		//DEEP_SLEEP_ENDC

#ifdef I2C_SEL
	AFE4410_Reg_Write(0, 0x20);	//ENABLE ULP mode
#else
	AFE4410_Reg_Write(0, 0x21);	//ENABLE ULP mode and SPI_READ mode
#endif
}

void AFE4410_Reg_Write(unsigned char reg_address, unsigned long data)
{
	unsigned char configData[3];
	configData[0] = (unsigned char)(data >> 16);
	configData[1] = (unsigned char)(((data & 0x00FFFF) >> 8));
	configData[2] = (unsigned char)(((data & 0x0000FF)));
}

signed long AFE4410_Reg_Read(unsigned char Reg_address)
{
	unsigned char configData[3];
	signed long retVal;

	retVal = configData[0];
	retVal = (retVal << 8) | configData[1];
	retVal = (retVal << 8) | configData[2];
	return retVal;
}

/*********************************************************************
* @fn      		initCalibrationRoutine
*
* @brief		Initializes Calibration routine variables
*
* @param   		void
*
* @return  		void
*
*********************************************************************/

void Init_CalibrationRoutine(void)
{
	AFE4410_Reg_Init();
	// 初始化 4410 校准部分参数
	// Ipelth TIA Gain Dac EnsepGain EnsepGain4
	// =============================================================//
	ensepgain = 1;
	ensepgain4 = 1;

	// TIA value
	tia_cf = 1;
	tia_cf_sep = 1;
	tia_cf_sep2 = 1;
	tia_cf_sep3 = 1;
	tia_gain = 1;
	tia_gain_sep = 1;
	tia_gain_sep2 = 1;
	tia_gain_sep3 = 1;
	// =============================================================//
	// Ipelth code
	i_gre = 16;
	i_red = 16;
	i_ird = 16;
	// =============================================================//
	// dac code
	dac_gre = 0;
	dac_red = 0;
	dac_ird = 0;
	dac_amb = 0;
	posi_dac_gre = 1;
	posi_dac_red = 1;
	posi_dac_ird = 1;
	posi_dac_amb = 1;
	// =============================================================//
	calibration_mode = 1;
}

void Elec_Write(uint8_t i_led1, uint8_t i_led2, uint8_t i_led3)
{
	uint8_t
		iled1_msb = 0,
		iled2_msb = 0,
		iled3_msb = 0,

		iled1_lsb = 0,
		iled2_lsb = 0,
		iled3_lsb = 0;
	uint32_t reg_22_value = 0;

	iled1_msb = (i_led1 & 0xFC) >> 2;
	iled2_msb = (i_led2 & 0xFC) >> 2;
	iled3_msb = (i_led3 & 0xFC) >> 2;

	iled1_lsb = (i_led1 & 0x03) >> 0;
	iled2_lsb = (i_led2 & 0x03) >> 0;
	iled3_lsb = (i_led3 & 0x03) >> 0;

	reg_22_value = 0;
	reg_22_value |= (iled1_msb & 0x3F) << 0;
	reg_22_value |= (iled2_msb & 0x3F) << 6;
	reg_22_value |= (iled3_msb & 0x3F) << 12;

	reg_22_value |= (iled1_lsb & 0x03) << 18;
	reg_22_value |= (iled2_lsb & 0x03) << 20;
	reg_22_value |= (iled3_lsb & 0x03) << 22;

	AFE4410_Reg_Write(0x0, 0x00000000);            	// write mode

	AFE4410_Reg_Write(0x22, (reg_22_value & 0x0FFF));

	AFE4410_Reg_Write(0x0, 0x00000001);            	// read mode
}

void Tia_GainChange(
	uint8_t rf1_mid,					// led1 rf
	uint8_t rf2_mid,					// led2 rf 
	uint8_t rf3_mid, 					// led3 rf
	uint8_t rf4_mid						// amb1 rf
)
{
	// cf * rf < 4000 form data_sheet
	uint8_t
		cf1 = 0,
		cf2 = 0,
		cf3 = 0,
		cf4 = 0,

		rf1 = 0,
		rf2 = 0,
		rf3 = 0,
		rf4 = 0;
	uint8_t
		cf1_mid = 0,
		cf2_mid = 0,
		cf3_mid = 0,
		cf4_mid = 0;
	uint8_t
		tia_gain_msb = 0,
		tia_gain_sep_msb = 0,
		tia_gain_sep2_msb = 0,
		tia_gain_sep3_msb = 0,

		tia_gain_lsb = 0,
		tia_gain_sep_lsb = 0,
		tia_gain_sep2_lsb = 0,
		tia_gain_sep3_lsb = 0;

	uint8_t  rf_last_arr[9] = { 5,4,3,2,1,0,6,7,8 };
	uint8_t  cf_last_arr[8] = { 1,0,3,2,5,4,7,6 };

	float
		cf1_value = 0.0f,
		cf2_value = 0.0f,
		cf3_value = 0.0f,
		cf4_value = 0.0f;
	uint16_t rf_value_sort[9] = { 10,25,50,100,250,500,1000,1500,2000 };
	float    cf_value_sort[8] = { 2.5f,5.0f,7.5f,10.0f,17.5f,20.0f,22.5f,25.0f };

	uint32_t reg_1f_value = 0;
	uint32_t reg_20_value = 0;
	uint32_t reg_21_value = 0;

	cf1_value = 4000.0f / float(rf_value_sort[rf1_mid - 1]);
	cf2_value = 4000.0f / float(rf_value_sort[rf2_mid - 1]);
	cf3_value = 4000.0f / float(rf_value_sort[rf3_mid - 1]);
	cf4_value = 4000.0f / float(rf_value_sort[rf4_mid - 1]);
	for (uint8_t i = 0; i < 8; i++)
	{
		if (cf1_value >= cf_value_sort[i])
		{
			cf1_value = cf_value_sort[i];
			cf1_mid = i + 1;
			break;
		}
	}
	for (uint8_t i = 0; i < 8; i++)
	{
		if (cf2_value >= cf_value_sort[i])
		{
			cf2_value = cf_value_sort[i];
			cf2_mid = i + 1;
			break;
		}
	}
	for (uint8_t i = 0; i < 8; i++)
	{
		if (cf3_value >= cf_value_sort[i])
		{
			cf3_value = cf_value_sort[i];
			cf3_mid = i + 1;
			break;
		}
	}
	for (uint8_t i = 0; i < 8; i++)
	{
		if (cf4_value >= cf_value_sort[i])
		{
			cf4_value = cf_value_sort[i];
			cf4_mid = i + 1;
			break;
		}
	}
	rf1 = rf_last_arr[rf1_mid - 1];
	rf2 = rf_last_arr[rf2_mid - 1];
	rf3 = rf_last_arr[rf3_mid - 1];
	rf4 = rf_last_arr[rf4_mid - 1];

	cf1 = cf_last_arr[cf1_mid - 1];
	cf2 = cf_last_arr[cf2_mid - 1];
	cf3 = cf_last_arr[cf3_mid - 1];
	cf4 = cf_last_arr[cf4_mid - 1];

	tia_gain_msb = (rf1 & 0x08) >> 3;
	tia_gain_sep_msb = (rf2 & 0x08) >> 3;
	tia_gain_sep2_msb = (rf3 & 0x08) >> 3;
	tia_gain_sep3_msb = (rf4 & 0x08) >> 3;

	tia_gain_lsb = (rf1 & 0x07) >> 0;
	tia_gain_sep_lsb = (rf2 & 0x07) >> 0;
	tia_gain_sep2_lsb = (rf3 & 0x07) >> 0;
	tia_gain_sep3_lsb = (rf4 & 0x07) >> 0;

	AFE4410_Reg_Write(0x0, 0x00000001);            	// read mode
	reg_1f_value = AFE4410_Reg_Read(0x1f);
	reg_20_value = AFE4410_Reg_Read(0x20);
	reg_21_value = AFE4410_Reg_Read(0x21);

	reg_1f_value = 0;
	for (uint8_t i = 0; i < 7; i++)
	{
		clrbit(reg_20_value, i);
	}

	for (uint8_t i = 0; i < 7; i++)
	{
		clrbit(reg_21_value, i);
	}

	reg_1f_value |= (tia_gain_sep2_lsb & 0x07) << 0;
	reg_1f_value |= (cf3 & 0x07) << 3;
	reg_1f_value |= (tia_gain_sep2_msb & 0x01) << 6;

	reg_1f_value |= (tia_gain_sep3_lsb & 0x07) << 8;
	reg_1f_value |= (cf4 & 0x07) << 11;
	reg_1f_value |= (tia_gain_sep3_msb & 0x01) << 14;

	reg_20_value |= (tia_gain_sep_lsb & 0x07) << 0;
	reg_20_value |= (cf2 & 0x07) << 3;
	reg_20_value |= (tia_gain_sep2_msb & 0x01) << 6;

	reg_21_value |= (tia_gain_lsb & 0x07) << 0;
	reg_21_value |= (cf1 & 0x07) << 3;
	reg_21_value |= (tia_gain_msb & 0x01) << 6;

	AFE4410_Reg_Write(0x0, 0x00000000);            	// write mode

	AFE4410_Reg_Write(0x1F, (reg_1f_value & 0x0FFF));
	AFE4410_Reg_Write(0x20, (reg_20_value & 0x0FFF));
	AFE4410_Reg_Write(0x21, (reg_21_value & 0x0FFF));

	AFE4410_Reg_Write(0x0, 0x00000001);            	// read mode
}

void AFE4410_WriteDacReg(
	uint8_t awdpr_ifs_dac,			// dac 量程

	uint8_t awdpr_dac_gre,			// 绿光dac值
	uint8_t awdpr_dac_red,			// 红光dac值
	uint8_t awdpr_dac_ird,			// 红外dac值
	uint8_t awdpr_dac_amb,			// 环境光dac值

	uint8_t awdpr_dac_posi_gre,		// 绿光dac极性
	uint8_t awdpr_dac_posi_red,		// 红光dac极性
	uint8_t awdpr_dac_posi_ird,		// 红外光dac极性
	uint8_t awdpr_dac_posi_amb		// 环境光dac极性
)
{
	uint8_t
		I_OFFDAC_LED1_MSB = 0,
		I_OFFDAC_LED2_MSB = 0,
		I_OFFDAC_LED3_MSB = 0,
		I_OFFDAC_AMB1_MSB = 0,

		I_OFFDAC_LED1_MID = 0,
		I_OFFDAC_LED2_MID = 0,
		I_OFFDAC_LED3_MID = 0,
		I_OFFDAC_AMB1_MID = 0,

		I_OFFDAC_LED1_LSB = 0,
		I_OFFDAC_LED2_LSB = 0,
		I_OFFDAC_LED3_LSB = 0,
		I_OFFDAC_AMB1_LSB = 0,

		I_OFFDAC_LED1_LSB_EXT = 0,
		I_OFFDAC_LED2_LSB_EXT = 0,
		I_OFFDAC_LED3_LSB_EXT = 0,
		I_OFFDAC_AMB1_LSB_EXT = 0;

	uint32_t
		reg_21_value = 0,
		reg_3A_value = 0,
		reg_3E_value = 0;

	I_OFFDAC_LED1_MSB = (awdpr_dac_gre & 0X40) >> 6;
	I_OFFDAC_LED1_MID = (awdpr_dac_gre & 0X3C) >> 2;
	I_OFFDAC_LED1_LSB = (awdpr_dac_gre & 0X02) >> 1;
	I_OFFDAC_LED1_LSB_EXT = (awdpr_dac_gre & 0X01) >> 0;

	I_OFFDAC_LED2_MSB = (awdpr_dac_red & 0X40) >> 6;
	I_OFFDAC_LED2_MID = (awdpr_dac_red & 0X3C) >> 2;
	I_OFFDAC_LED2_LSB = (awdpr_dac_red & 0X02) >> 1;
	I_OFFDAC_LED2_LSB_EXT = (awdpr_dac_red & 0X01) >> 0;

	I_OFFDAC_LED3_MSB = (awdpr_dac_ird & 0X40) >> 6;
	I_OFFDAC_LED3_MID = (awdpr_dac_ird & 0X3C) >> 2;
	I_OFFDAC_LED3_LSB = (awdpr_dac_ird & 0X02) >> 1;
	I_OFFDAC_LED3_LSB_EXT = (awdpr_dac_ird & 0X01) >> 0;

	I_OFFDAC_AMB1_MSB = (awdpr_dac_amb & 0X40) >> 6;
	I_OFFDAC_AMB1_MID = (awdpr_dac_amb & 0X3C) >> 2;
	I_OFFDAC_AMB1_LSB = (awdpr_dac_amb & 0X02) >> 1;
	I_OFFDAC_AMB1_LSB_EXT = (awdpr_dac_amb & 0X01) >> 0;

	AFE4410_Reg_Write(0x0, 0x00000001);            	// read mode
	reg_21_value = AFE4410_Reg_Read(0x21);
	reg_3A_value = AFE4410_Reg_Read(0x3A);
	reg_3E_value = AFE4410_Reg_Read(0x3E);

	for (uint8_t i = 12; i < 15; i++)
	{
		clrbit(reg_21_value, i);
	}
	reg_21_value |= ((ifs_dac & 0x07) << 12);

	for (uint8_t i = 0; i < 20; i++)
	{
		clrbit(reg_3A_value, i);
	}
	reg_3A_value |= ((I_OFFDAC_LED3_MID & 0x0F) << 0);
	reg_3A_value |= ((awdpr_dac_posi_ird & 0x01) << 4);
	reg_3A_value |= ((I_OFFDAC_LED1_MID & 0x0F) << 5);
	reg_3A_value |= ((awdpr_dac_posi_gre & 0x01) << 9);
	reg_3A_value |= ((I_OFFDAC_AMB1_MID & 0x0F) << 10);
	reg_3A_value |= ((awdpr_dac_posi_amb & 0x01) << 14);
	reg_3A_value |= ((I_OFFDAC_LED2_MID & 0x0F) << 15);
	reg_3A_value |= ((awdpr_dac_posi_red & 0x01) << 19);

	for (uint8_t i = 0; i < 12; i++)
	{
		clrbit(reg_3E_value, i);
	}
	reg_3E_value |= ((I_OFFDAC_LED3_LSB & 0x01) << 0);
	reg_3E_value |= ((I_OFFDAC_LED3_MSB & 0x01) << 1);
	reg_3E_value |= ((I_OFFDAC_LED1_LSB & 0x01) << 2);
	reg_3E_value |= ((I_OFFDAC_LED1_MSB & 0x01) << 3);

	reg_3E_value |= ((I_OFFDAC_AMB1_LSB & 0x01) << 4);
	reg_3E_value |= ((I_OFFDAC_AMB1_MSB & 0x01) << 5);
	reg_3E_value |= ((I_OFFDAC_LED2_LSB & 0x01) << 6);
	reg_3E_value |= ((I_OFFDAC_LED2_MSB & 0x01) << 7);

	reg_3E_value |= ((I_OFFDAC_LED3_LSB_EXT & 0x01) << 8);
	reg_3E_value |= ((I_OFFDAC_LED1_LSB_EXT & 0x01) << 9);
	reg_3E_value |= ((I_OFFDAC_AMB1_LSB_EXT & 0x01) << 10);
	reg_3E_value |= ((I_OFFDAC_LED2_LSB_EXT & 0x01) << 11);

	AFE4410_Reg_Write(0x0, 0x00000000);            	// write mode

	AFE4410_Reg_Write(0x21, (reg_21_value & 0x0FFF));
	AFE4410_Reg_Write(0x3A, (reg_3A_value & 0x0FFF));
	AFE4410_Reg_Write(0x3E, (reg_3E_value & 0x0FFF));

	AFE4410_Reg_Write(0x0, 0x00000001);            	// read mode
}

void Static_ParamToZero(void)
{
	gre_adc_means = 0.0f;
	red_adc_means = 0.0f;
	ird_adc_means = 0.0f;
	amb_adc_means = 0.0f;
	wait_cnt = 0;
}

void Ambient_Cancellation(int32_t sig_adc, int32_t amb_adc)
{
	uint8_t need_dac_code = 0;
	float   need_dac_value = 0.0f;
	// 环境消除函数
	// 需要设置适当的DAC来消除环境
	// 需要知道当前当前增益状态下ADC值1刻度的环境光对应的电流
	// 已知环境光对应电流 通过电流来消除环境光

	need_dac_value = amb_adc * ADC_1CODE_CONVERT_TO_HOW_MANY_IPELTH_IN_10KRF;
	if (need_dac_value < 15.875f)
	{
		ifs_dac = 0;
	}
	else if (need_dac_value < 31.75f)
	{
		ifs_dac = 3;
	}
	else if (need_dac_value < 63.5f)
	{
		ifs_dac = 5;
	}
	else
	{
		ifs_dac = 7;
	}
	need_dac_code = abs(need_dac_value / (ifs_dac_per_value[(uint8_t)(ifs_dac / 2)]));
	if (amb_adc < 0)
	{
		posi_dac_gre = 0;
		posi_dac_red = 0;
		posi_dac_ird = 0;
		posi_dac_amb = 0;
	}
	else
	{
		posi_dac_gre = 1;
		posi_dac_red = 1;
		posi_dac_ird = 1;
		posi_dac_amb = 1;
	}
	// 环境光和绿光不更改DAC  兼容以前的血氧
	dac_red = need_dac_code;
	dac_ird = need_dac_code;
	AFE4410_WriteDacReg(
		ifs_dac,

		dac_gre,
		dac_red,
		dac_ird,
		dac_amb,

		posi_dac_gre,
		posi_dac_red,
		posi_dac_ird,
		posi_dac_amb);

	calibration_mode = 2;
}

void Gain_Calibration(int32_t red_da, int32_t ird_da)
{
	if (red_da <= (int32_t)(MIN_LINE * 2097152))
	{
		red_negative_flag = 1;
	}
	else
	{
		red_negative_flag = 0;
	}
	if (ird_da <= (int32_t)(MIN_LINE * 2097152))
	{
		ird_negative_flag = 1;
	}
	else
	{
		ird_negative_flag = 0;
	}

	if (!ird_negative_flag && !red_negative_flag)
	{
		if (red_da < LOW_LINE || ird_da < LOW_LINE ||
			red_da > HIGH_LINE || ird_da > HIGH_LINE)
		{
			calibration_mode = 2;
		}
	}
	if ((red_da_last >= LOW_LINE && red_da_last <= HIGH_LINE && (red_da > HIGH_LINE || red_da < LOW_LINE)) &&
		(ird_da_last >= LOW_LINE && ird_da_last <= HIGH_LINE && (ird_da > HIGH_LINE || ird_da < LOW_LINE)))
	{
		// 使用上次增益 并且退出增益调节
		Tia_GainChange(1, tia_gain_sep_last, tia_gain_sep2_last, 1);
		calibration_mode = 3;
		return;
	}

	if (red_da_last > LOW_LINE && red_da > HIGH_LINE)
	{
		// 红光增益-1（使用上次增益）
		Tia_GainChange(1, tia_gain_sep_last, tia_gain_sep2, 1);
		red_da_fall_in_to_loop_flag = 1;
	}
	else
	{
		red_da_fall_in_to_loop_flag = 0;
	}
	if (ird_da_last > LOW_LINE && ird_da > HIGH_LINE)
	{
		// 红外增益-1（使用上次增益）
		Tia_GainChange(1, tia_gain_sep, tia_gain_sep2_last, 1);
		ird_da_fall_in_to_loop_flag = 1;
	}
	else
	{
		ird_da_fall_in_to_loop_flag = 0;
	}
	red_da_last = red_da;
	ird_da_last = ird_da;

	if (red_negative_flag || ird_negative_flag)
	{
		// 去增加电流
		if (red_negative_flag)
		{
			// 电流加1刻度
			i_red += 1;
		}
		if (ird_negative_flag)
		{
			// 电流加1刻度
			i_ird += 1;
		}
		Elec_Write(16, i_red, i_ird);
		calibration_mode = 2;
		return;
	}
	else
	{
		tia_gain_sep_last = tia_gain_sep;
		tia_gain_sep2_last = tia_gain_sep2;

		if (red_da_fall_in_to_loop_flag == 0)
		{
			if (red_da < LOW_LINE)
			{
				// 红光增益+1
				tia_gain_sep += 1;
			}
			if (red_da > HIGH_LINE)
			{
				// 红光增益-1
				tia_gain_sep -= 1;
			}
		}
		if (ird_da_fall_in_to_loop_flag == 0)
		{
			if (ird_da < LOW_LINE)
			{
				// 红外增益+1
				tia_gain_sep2 += 1;
			}
			if (ird_da > HIGH_LINE)
			{
				// 红外增益-1
				tia_gain_sep2 -= 1;
			}
		}
		Tia_GainChange(1, tia_gain_sep, tia_gain_sep2, 1);
	}
}

void Elec_Calibration(int32_t red_sig, int32_t ird_sig)
{
	if (red_sig > HIGH_LINE && red_da_last <= HIGH_LINE && red_da_last >= LOW_LINE &&
		ird_sig > HIGH_LINE && ird_da_last <= HIGH_LINE && ird_da_last >= LOW_LINE)
	{
		// 使用上一个电流刻度值
		Elec_Write(i_gre, red_i_last, ird_i_last);
		calibration_mode = 4;
		return;
	}

	// 如果加一格大  减一格小  可以减一格增益  先不做了。先减一格电流吧
	if (red_da_last > HIGH_LINE && red_sig < LOW_LINE &&
		ird_da_last > HIGH_LINE && ird_sig < LOW_LINE)
	{
		Elec_Write(i_gre, i_red, i_ird);
		calibration_mode = 4;
		return;
	}

	red_i_last = i_red;
	ird_i_last = i_ird;

	if (red_sig > HIGH_LINE)
	{
		i_red -= 1;
	}
	if (ird_sig > HIGH_LINE)
	{
		i_ird -= 1;
	}

	if (red_sig < LOW_LINE)
	{
		i_red += 1;
	}
	if (ird_sig < LOW_LINE)
	{
		i_ird += 1;
	}

	red_da_last = red_sig;
	ird_da_last = ird_sig;

	Elec_Write(i_gre, i_red, i_ird);
}

void AFExx_Calib(int32_t gre_da, int32_t amb_da, int32_t red_da, int32_t ird_da)
{
	if (calibration_mode)
	{
		wait_cnt++;
		if (wait_cnt < WAITING_NUM)
		{
			// 求的ADC均值
			gre_adc_means += gre_da;
			red_adc_means += red_da;
			ird_adc_means += ird_da;
			amb_adc_means += amb_da;
			return;
		}
		else
		{
			gre_adc_means /= WAITING_NUM;
			red_adc_means /= WAITING_NUM;
			ird_adc_means /= WAITING_NUM;
			amb_adc_means /= WAITING_NUM;
		}
	}
	switch (calibration_mode)
	{
	case (0):
		//calls the initCalibrationRoutine
		Init_CalibrationRoutine();
		break;

	case (1):
		//calls the AmbientCancellation function
		Ambient_Cancellation(red_da, amb_da);
		Static_ParamToZero();
		break;

	case (2):
		//calls the GainCalibration function
		Gain_Calibration(red_da, ird_da);
		Static_ParamToZero();
		break;

	case (3):
		//calls the PeriodicCalibration function
		Elec_Calibration(red_da, ird_da);
		Static_ParamToZero();
		break;

	case (4):
		//ends the calibration
		Static_ParamToZero();
		calibration_mode = 5;
		//Calibration = 0;
		break;

	default:
		break;
	}
}
