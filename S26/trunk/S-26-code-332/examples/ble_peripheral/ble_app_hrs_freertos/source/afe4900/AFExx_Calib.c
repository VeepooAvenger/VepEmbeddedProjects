#include "AFExx_Calib.h"

#define setbit(x,y)										   (x |= (1 << y))  
#define clrbit(x,y)										   (x &= ~(1 << y)) 

#define LOW_LINE                                           (1200000)
#define HIGH_LINE                                          (1950000)

#define MIN_LINE                                           (0.1f)
#define MAX_LINE                                           (0.9f)

#define WAITING_NUM										   									 (2)
// 微安
#define ADC_1CODE_CONVERT_TO_HOW_MANY_IPELTH_IN_10KRF      (0.0286102294921875)

// 0: sInitialize 1: sAmbientDAC 2: sGain 3: sPeriodic 4: sFinish
uint8_t calibration_mode = 1;
uint8_t fall_in_loop_count = 0;

uint8_t wait_cnt = 0;
uint8_t red_negative_flag = 0;
uint8_t ird_negative_flag = 0;
uint8_t red_da_fall_in_to_loop_flag = 0;
uint8_t ird_da_fall_in_to_loop_flag = 0;

static float   gre_adc_means = 0.0f;
static float   red_adc_means = 0.0f;
static float   ird_adc_means = 0.0f;
static float   amb_adc_means = 0.0f;
// =================================================================//
static uint8_t
i_gre = 0,
i_red = 0,
i_ird = 0;
// =================================================================//
// ensepgain(1: allow double gain(red&ird and gre&amb) 0: allow only one gain(all in))
// ensepgain4(1: allow 4 gain  0: not allow 4 gain)
static uint8_t
ensepgain = 0,
ensepgain4 = 0;

static uint8_t
tia_cf = 0,
tia_cf_sep = 0,
tia_cf_sep2 = 0,
tia_cf_sep3 = 0,

tia_gain = 0,
tia_gain_sep = 0,
tia_gain_sep2 = 0,
tia_gain_sep3 = 0;

static float
tia_gain_sep_last = 0.0f,
tia_gain_sep2_last = 0.0f;
// =================================================================//
static uint8_t
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
static uint8_t ifs_dac = 0;
static uint8_t ifs_dac_per_value[4] = { 0.125f,0.25f,0.5f,1.0f };

static uint32_t ird_i_last = 0;
static uint32_t red_i_last = 0;
// =================================================================//
static int32_t red_da_last = 0;
static int32_t ird_da_last = 0;
// =================================================================//
void AFE4410_Reg_Init(void)
{
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
	Afe4900_RegWrite(0x22, 0xA91451);              //LEDCNTRL1
	Afe4900_RegWrite(0x23, 0x10C218);              //CONTROL2
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
	Afe4900_RegWrite(0x3A, (0x184210 & (~(0x000001 << 20))) | (afe4410_ppg_param_s.set_early_offset_loop << 20));              //OFFDAC
	Afe4900_RegWrite(0x3B, 0x000000);              //THRDETLOW
	Afe4900_RegWrite(0x3C, 0x200000);              //THRDETHIGH
	Afe4900_RegWrite(0x3D, 0x000000);              //THRDET
	Afe4900_RegWrite(0x3E, 0x000000);              //I_OFFDAC
	Afe4900_RegWrite(0x3F, 0x000000);              //AVG_LED2_ALED2VAL
	Afe4900_RegWrite(0x40, 0x000000);              //AVG_LED1_ALED1VAL
//	  Afe4900_RegWrite(0x42, 0x000000);              //FIFO
	Afe4900_RegWrite(0x42, 0x000000 | (4 << 6) | (0x02 << 4) | (0x00 << 0));
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
		| (afe4410_ppg_param_s.set_en_amb_loop << 11)
		| (afe4410_ppg_param_s.set_choose_amb_phase << 9)
		| (afe4410_ppg_param_s.set_freeze_loop << 5)
		| (afe4410_ppg_param_s.set_hyst_loop << 3));
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
	Afe4900_RegWrite(0x00, 0x000061);             //AFE4900READ MODE AND ULP
}

//void AFE4410_Reg_Write(unsigned char reg_address, unsigned long data)
//{
//	unsigned char configData[3];
//	configData[0] = (unsigned char)(data >> 16);
//	configData[1] = (unsigned char)(((data & 0x00FFFF) >> 8));
//	configData[2] = (unsigned char)(((data & 0x0000FF)));
//}

//signed long AFE4410_Reg_Read(unsigned char Reg_address)
//{
//	unsigned char configData[3];
//	signed long retVal;

//	retVal = configData[0];
//	retVal = (retVal << 8) | configData[1];
//	retVal = (retVal << 8) | configData[2];
//	return retVal;
//}

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
	tia_cf = 6;
	tia_cf_sep = 6;
	tia_cf_sep2 = 6;
	tia_cf_sep3 = 6;
	tia_gain = 5;
	tia_gain_sep = 5;
	tia_gain_sep2 = 5;
	tia_gain_sep3 = 5;
	// =============================================================//
	// Ipelth code
	i_gre = 70;
	i_red = 70;
	i_ird = 70;
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
	calibration_mode = 2;
	fall_in_loop_count = 0;
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

	calibration_mode = 3;
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
			calibration_mode = 3;
		}
	}
	if ((red_da_last >= LOW_LINE && red_da_last <= HIGH_LINE && (red_da > HIGH_LINE || red_da < LOW_LINE)) &&
		(ird_da_last >= LOW_LINE && ird_da_last <= HIGH_LINE && (ird_da > HIGH_LINE || ird_da < LOW_LINE)))
	{
		// 使用上次增益 并且退出增益调节
		Tia_GainChange(1, tia_gain_sep_last, tia_gain_sep2_last, 1);
		calibration_mode = 4;
		return;
	}

	bool red_flag = (red_da_last > LOW_LINE && red_da > HIGH_LINE);
	bool ird_flag = (red_da_last > LOW_LINE && red_da > HIGH_LINE);

	if (red_flag && ird_flag)
	{
		if (tia_gain_sep > 1 && tia_gain_sep2 > 1)
		{
			Tia_GainChange(1, tia_gain_sep - 1, tia_gain_sep2 - 1, 1);
		}
		red_da_fall_in_to_loop_flag = 1;
		ird_da_fall_in_to_loop_flag = 1;
	}
	else if (red_flag && !ird_flag)
	{
		if (tia_gain_sep > 1) 
		{
			Tia_GainChange(1, tia_gain_sep - 1, tia_gain_sep2, 1);
		}
		red_da_fall_in_to_loop_flag = 1;
		ird_da_fall_in_to_loop_flag = 0;
	}
	else if (!red_flag && ird_flag)
	{
		if (tia_gain_sep2 > 1)
		{
			Tia_GainChange(1, tia_gain_sep, tia_gain_sep2 - 1, 1);
		}
		red_da_fall_in_to_loop_flag = 0;
		ird_da_fall_in_to_loop_flag = 1;
	}
	else
	{
		Tia_GainChange(1, tia_gain_sep, tia_gain_sep2, 1);
		red_da_fall_in_to_loop_flag = 0;
		ird_da_fall_in_to_loop_flag = 0;
	}

	red_da_last = red_da;
	ird_da_last = ird_da;

	tia_gain_sep_last = tia_gain_sep;
	tia_gain_sep2_last = tia_gain_sep2;

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
		Elec_Write(70, i_red, i_ird);
		calibration_mode = 3;
		return;
	}
	else
	{
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
		calibration_mode = 5;
		return;
	}

	// 如果加一格大  减一格小  可以减一格增益  先不做了。先减一格电流吧
	if (red_da_last > HIGH_LINE && red_sig < LOW_LINE &&
		ird_da_last > HIGH_LINE && ird_sig < LOW_LINE)
	{
		Elec_Write(i_gre, i_red, i_ird);
		calibration_mode = 5;
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
			//			calibration_mode++;
			gre_adc_means /= WAITING_NUM;
			red_adc_means /= WAITING_NUM;
			ird_adc_means /= WAITING_NUM;
			amb_adc_means /= WAITING_NUM;
		}
	}
	switch (calibration_mode)
	{
	case (1):
		//calls the initCalibrationRoutine
		Init_CalibrationRoutine();
		//		calibration_mode++;
		break;

	case (2):
		//calls the AmbientCancellation function
		Ambient_Cancellation(red_da, amb_da);
		Static_ParamToZero();
		//		calibration_mode++;
		break;

	case (3):
		//calls the GainCalibration function
		Gain_Calibration(red_da, ird_da);
		Static_ParamToZero();
		//		calibration_mode++;
		break;

	case (4):
		//calls the PeriodicCalibration function
		Elec_Calibration(red_da, ird_da);
		Static_ParamToZero();
		//		calibration_mode++;
		break;

	case (5):
		//ends the calibration
		Static_ParamToZero();
		calibration_mode = 6;
		//		calibration_mode++;
				//Calibration = 0;
		break;

	default:
		break;
	}
}
