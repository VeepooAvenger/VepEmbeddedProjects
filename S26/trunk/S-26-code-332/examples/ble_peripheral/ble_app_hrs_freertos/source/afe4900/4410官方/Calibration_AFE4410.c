#include "Calibration_AFE4410.h"
#include "afe4900_calibration.h"

/*******************************************************************************/
/*	        Global Variables										       	   */
/*******************************************************************************/
/* Constants Definitions */
const unsigned long ADC_FULL_SCALE = 0x200000;   				// ADC Full scale
const unsigned long ADC_TEN_PERCENT_FS = 209715; 				// 10% of ADC Full scale
const unsigned long ADC_ONE_PERCENT_FS = 20971; 				// 1% of ADC Full scale

//const unsigned int TFREEZE = 2;   								// Time in seconds between two periodic calibrations
const unsigned int TIMEWAIT = 2; 								// Duration (Number of PRFs) to wait after a configuration change

const unsigned int BLOCK_SIZE_BITS = 1; 						// Samples Window size = 2^BLOCK_SIZE_BITS for measuring DC value
const unsigned int BLOCK_SIZE = 2; 							// Number of samples required to calculate the DC value

const unsigned int LED1_reg_shift = 0;							// LED1 current reg - MSB bit position
const unsigned int LED2_reg_shift = 6;							// LED2 current reg - MSB bit position
const unsigned int LED3_reg_shift = 12;							// LED3 current reg - MSB bit position

//AFE4410 change
// New shift values for the LED current LSB bits
const unsigned int LED1_reg_shift_LSB = 18;							// LED1 current reg - LSB bit position
const unsigned int LED2_reg_shift_LSB = 20;							// LED2 current reg - LSB bit position
const unsigned int LED3_reg_shift_LSB = 22;							// LED3 current reg - LSB bit position

//AFE4410 change
// Added the 2 LSB bits to the LED masks
const unsigned long LED1_mask = 0x0C003F;						// LED1 current - mask bits
const unsigned long LED2_mask = 0x300FC0;						// LED2 current - mask bits
const unsigned long LED3_mask = 0xC3F000;						// LED3 current - mask bits

const unsigned int AMB_reg_shift_led1 = 5;						// Shift position for OFFDAC - LED1
const unsigned int AMB_reg_shift_amb1 = 10;						// Shift position for OFFDAC - AMB1
const unsigned int AMB_reg_shift_led2 = 15;						// Shift position for OFFDAC - LED2
const unsigned int AMB_reg_shift_amb2 = 0;						// Shift position for OFFDAC - LED3 / AMB2

//AFE4410 change
// New shift values for MSB and LSB of offset DAC values
const unsigned int AMB_reg_shift_led1_2 = 2;						// Shift position for OFFDAC - LED1
const unsigned int AMB_reg_shift_amb1_2 = 4;						// Shift position for OFFDAC - AMB1
const unsigned int AMB_reg_shift_led2_2 = 6;						// Shift position for OFFDAC - LED2
const unsigned int AMB_reg_shift_amb2_2 = 0;						// Shift position for OFFDAC - LED3 / AMB2

//AFE4410 change
// New Mask for the MSB and LSB of offset DAC values
const unsigned long AMB1_mask = 0x007FE0; 						// Mask for OFFDAC for LED1 and AMB1
const unsigned long AMB1_mask_2 = 0x00003C; 						// Mask for OFFDAC for LED1 and AMB1

const unsigned long AMB2_mask = 0x0FFC00; 						// Mask for OFFDAC for LED2 and AMB1
const unsigned long AMB2_mask_2 = 0x0000F0; 						// Mask for OFFDAC for LED2 and AMB1

const unsigned long AMB3_mask = 0x007C1F; 						// Mask for OFFDAC for LED3 and AMB1
const unsigned long AMB3_mask_2 = 0x000033; 						// Mask for OFFDAC for LED3 and AMB1

const unsigned long AMB23_mask = 0x0FFC1F;						// Mask for OFFDAC for LED2, LED3 and AMB1
const unsigned long AMB23_mask_2 = 0x0000F3;

const unsigned long AMB123_mask = 0x0FFFFF;						// Mask for OFFDAC for LED1, LED2, LED3 and AMB1
const unsigned long AMB123_mask_2 = 0x0000FF;						// Mask for OFFDAC for LED1, LED2, LED3 and AMB1

const unsigned long AMB1_mask_LED = 0x0003E0;					// Mask for OFFDAC for LED1 only (no AMB1)
const unsigned long AMB1_mask_LED_2 = 0x0000C;					// Mask for OFFDAC for LED1 only (no AMB1)

const unsigned long AMB2_mask_LED = 0x0F8000;					// Mask for OFFDAC for LED2 only (no AMB1)
const unsigned long AMB2_mask_LED_2 = 0x000C0;					// Mask for OFFDAC for LED2 only (no AMB1)

const unsigned long AMB3_mask_LED = 0x00001F;					// Mask for OFFDAC for LED3 only (no AMB1)
const unsigned long AMB3_mask_LED_2 = 0x000003;					// Mask for OFFDAC for LED3 only (no AMB1)

const unsigned long AMB23_mask_LED = 0x0F801F;					// Mask for OFFDAC for LED2 and LED3 only (no AMB1)
const unsigned long AMB23_mask_LED_2 = 0x0000C3;					// Mask for OFFDAC for LED2 and LED3 only (no AMB1)

const unsigned long AMB123_mask_LED = 0x0F83FF;					// Mask for OFFDAC for LED1, LED2 and LED3 only (no AMB1)
const unsigned long AMB123_mask_LED_2 = 0x0000CF;					// Mask for OFFDAC for LED1, LED2 and LED3 only (no AMB1)

const unsigned long ILED_CURR_MIN_code = 2; 							// LED min current reqd. for application - 3.2 mA assuming 100mA range (This is default value)
const unsigned long ILED_CURR_MAX_code = 220; 							// LED max current reqd. for application - 88 mA assuming 100mA range (This is default value)

const short unsigned int LOW_THR_PERCENT = 10;       			// Low Threshold Percent
const short unsigned int HIGH_THR_PERCENT = 90;      			// High Threshold percent
const short unsigned int HYS_PERCENT = 3;            			// Hysteresis percent
const short int TARGET_THR_PERCENT = 50;    					// Target Threshold percent

long TARGET_THR; 												// Target threshold for Gain Calibration
long LOW_THR_HYS_LOW; 											// Low threshold for gain calibration in periodic mode
long LOW_THR_HYS_LOW_USER;
long LOW_THR_HYS_LOW_LED2;
long LOW_THR_HYS_HIGH; 											// Low threshold for gain calibration
long HIGH_THR_HYS_LOW; 											// High threshold for gain calibration
long HIGH_THR_HYS_HIGH; 										// High threshold for gain calibration in periodic mode

//AFE4410 change
//Changed the step size to 0.25 uA and min to 0.125 uA
unsigned long ADC_CODE_AMB_DAC_MIN = 21845;			 			// ADC output for 0.125uA pleth current with Rf = 50K
unsigned long ADC_CODE_AMB_DAC_STEP = 43690;		 			// Step size at the ADC output for an increase of 0.25uA pleth current with Rf = 50K ==> 0.5uA * 100K in codes

//extern volatile int g_OneSecondFlag; 							// Flag to indicate the HRM has generated an SNR value (This flag is set in the ISR for ADC ready interrupt after every second)
unsigned int Ambient_DAC_enabled = 0; 							// Indicates whether the AMB_DAC cancellation is used or not

CALIBRATION_STATES AmbientDACcalibration_state = sInit;
CALIBRATION_STATES Gaincalibration_state = sInit;
CALIBRATION_MODES calibration_mode = sInitialize;//sAmbientDAC;
//PERIODIC_MODES PeriodicCalibration_state = sFreeze_periodic;
PERIODIC_MODES PeriodicCalibration_state = sAmbient_periodic;
OFFSETDAC_CALIB_STATES OffsetDAC_code_Est_state = sOffsetDACInit;
CALIB_ENABLE Calibration_en;
FLAGS flag;
SYSTEM_PARAMS system_param_s;
SIGNAL_PARAMS signal;

//int Calibration = 1;  						// Indicates whether calibration is ON
extern int Calibration;  						// Indicates whether calibration is ON
unsigned int Periodic_started = 0; 			// Indicates whether periodic calibration has started or not

//AFE4410 change
//Added one more element to the array to incorporate for Cf for RF = 1.5M
int Cf_array[9] = { 250, 250, 250, 250, 250, 250, 250, 250, 250 };


//AFE4410 change
// Use last location for the new register 0x3E
unsigned long AFE44xx_Current_Register_Settings[5] = { 0, 0, 0, 0, 0 }; // Array that holds the latest AFE register settings
// For any AFE register change - update the appropriate element
// in the array corresponding to the register
// 0 for register 0x22 - LED current
// 1 for register 0x20 - TIA gain 2
// 2 for register 0x21 - TIA gain 1
// 3 for register 0x3A - Offset DAC 1
// 4 for register 0x3E   - Offset DAC 2

unsigned int Enable_sep_gain = 1;
unsigned int DC_can = 0;
int LED_pulse_width = 0;
// Selection of either LED1 or LED2 or LED3 in MODE1 (HRM only mode)
char LED_Sel = 1;
RF_VALUES RFValue_Init_GainCal = s2M; 							// Initial Rf for Gain calibration (if ambient dac is not enabled)
unsigned int periodic_cal_req = 0;

//AFE4410 change
// Updated the Ipleth for the new LED current resolution of 0.2 mA
unsigned long Ipleth = 15000;
unsigned int LED_DC_can = 6;

//AFE4410 change
// Updated the Ipleth values for the new LED current resolution of 0.2 mA
// Use this for 50mA range
//int Ipleth_array[5] = { 5000, 7500, 10000, 12500, 15000 }; // codes for 1uA, 1.5uA ... 3uA expressed as 1000nA/0.2mA -- unit code is 0.2mA
// Use this for 100mA range
int Ipleth_array[5] = { 2500, 3750, 5000, 6250, 7500 }; // codes for 1uA, 1.5uA ... 3uA expressed as 1000nA/0.4mA -- unit code is 0.4mA
long CTR = 0;

void AFE4410_InitTest(void)
{
	Afe4900_RegWrite(0x00, 0x000062);              //CONTROL0
	Afe4900_RegWrite(1, 14);     //AFE_LED2STC
  Afe4900_RegWrite(2, 18);    	//AFE_LED2ENDC
  Afe4900_RegWrite(3, 31);    	//AFE_LED1LEDSTC
  Afe4900_RegWrite(4, 36);   	//AFE_LED1LEDENDC
  Afe4900_RegWrite(5, 23);    	//AFE_ALED2STC
  Afe4900_RegWrite(6, 27);    	//AFE_ALED2ENDC
  Afe4900_RegWrite(7, 32);    	//AFE_LED1STC
  Afe4900_RegWrite(8, 36);   	//AFE_LED1ENDC
  Afe4900_RegWrite(9, 13);     //AFE_LED2LEDSTC
  Afe4900_RegWrite(10, 18);   	//AFE_LED2LEDENDC
  Afe4900_RegWrite(11, 41);  	//AFE_ALED1STC
  Afe4900_RegWrite(12, 45);  	//AFE_ALED1ENDC
  Afe4900_RegWrite(13, 19);   	//AFE_LED2CONVST
  Afe4900_RegWrite(14, 34);  	//AFE_LED2CONVEND
  Afe4900_RegWrite(15, 35);  	//AFE_ALED2CONVST
  Afe4900_RegWrite(16, 50);  	//AFE_ALED2CONVEND
  Afe4900_RegWrite(17, 51);  	//AFE_LED1CONVST
  Afe4900_RegWrite(18, 66);  	//AFE_LED1CONVEND
  Afe4900_RegWrite(19, 67);  	//AFE_ALED1CONVST
  Afe4900_RegWrite(20, 82);  	//AFE_ALED1CONVEND
  Afe4900_RegWrite(21, 3000);  //AFE_ADCRSTSTCT0
  Afe4900_RegWrite(22, 3000);  //AFE_ADCRSTENDCT0
  Afe4900_RegWrite(23, 3000);  //AFE_ADCRSTSTCT1
  Afe4900_RegWrite(24, 3000);  //AFE_ADCRSTENDCT1
  Afe4900_RegWrite(25, 3000);  //AFE_ADCRSTSTCT2
  Afe4900_RegWrite(26, 3000);  //AFE_ADCRSTENDCT2
  Afe4900_RegWrite(27, 3000);  //AFE_ADCRSTSTCT3
  Afe4900_RegWrite(28, 3000);  //AFE_ADCRSTENDCT3

  Afe4900_RegWrite(29, 2559); 	//AFE_PRPCOUNT

  Afe4900_RegWrite(30, 0x000100);      //AFE_CONTROL1 TimerEN = 1; NUMAV = 0

  Afe4900_RegWrite(32, 0x000000);      //AFE_TIA_SEP_GAIN (LED2) ENSEPGAIN = 0; LED2/LED3 gain = 25K
  Afe4900_RegWrite(33, 0x000005);      //AFE_TIA_GAIN (LED1) LED1/LED1AMB gain = 25K


  Afe4900_RegWrite(58, 0x000000);      //AFE_DAC_SETTING_REG
  Afe4900_RegWrite(34, 0x0030C3);      //LED3 ; LED2 ; LED1
  //CONTROL_DYN_TX(0), LEDCurr = 100mA, CONTROL_DYN_BIAS, ENABLE_OSC = 0, CONTROL_DYN_TIA, CONTROL_DYN_ADC
  Afe4900_RegWrite(35, 0x124218);      //AFE_CONTROL2
  Afe4900_RegWrite(75, 0x0F); 			//CONTROL_DYN_VCM / CONTROL_DYN_DLDO / CONTROL_DYN_ALDO / CONTROL_DYN_BG
  Afe4900_RegWrite(80, 0x08); 			//CONTROL_DYN_TX(1)

  Afe4900_RegWrite(49, 0x20); 			//ENABLE_INPUT_SHORT = 1

  Afe4900_RegWrite(50, 0);  			//PDNCYCLESTC
  Afe4900_RegWrite(51, 0); 			//PDNCYCLEENDC

  Afe4900_RegWrite(54, 22);   			//AFE_LED3LEDSTC
  Afe4900_RegWrite(55, 27);   			//AFE_LED3LEDENDC

  Afe4900_RegWrite(57, 0);     		//CLKDIV_PRF

  Afe4900_RegWrite(67, 40);   			//AFE_LED4LEDSTC
  Afe4900_RegWrite(68, 45);   			//AFE_LED4LEDENDC

  Afe4900_RegWrite(82, 92); 			//DATA_RDY_STC
  Afe4900_RegWrite(83, 92); 			//DATA_RDY_ENDC

  Afe4900_RegWrite(100, 0); 			//DYN_TIA_STC
  Afe4900_RegWrite(101, 49); 			//DYN_TIA_ENDC
  Afe4900_RegWrite(102, 6); 			//DYN_ADC_STC
  Afe4900_RegWrite(103, 88); 			//DYN_ADC_ENDC
  Afe4900_RegWrite(104, 6); 			//DYN_CLOCK_STC
  Afe4900_RegWrite(105, 88); 			//DYN_CLOCK_ENDC
  Afe4900_RegWrite(106, 114); 			//DEEP_SLEEP_STC
  Afe4900_RegWrite(107, 2507); 		//DEEP_SLEEP_ENDC
	Afe4900_RegWrite(0x00, 0x000061);             //AFE4900READ MODE AND ULP
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
void initCalibrationRoutine(void)
{
	unsigned int Calibration_enabled = 0;
	unsigned int Ipleth_num;
	
	AFE4410_InitTest();
	//Calibration = 1;
	Calibration_en.AMB = 1; // Check for calibration modes
	Calibration_en.GAIN = 1;
	Calibration_en.PERIODIC = 1;

	// Selection of either LED1 or LED2 or LED3 in MODE1 (HRM only mode)
	LED_Sel = 1;

	Ipleth_num = 4;
	Ipleth = Ipleth_array[Ipleth_num];

	//AFE4410 change
	// Updated the LED_DC_can for the new Offset DAC resolution of 0.25 uA
	LED_DC_can = Ipleth_num * 2 + 4; // When Iplethnum = 0, (1uA) corresponds to reg settings of 4 (reason for why +4)

	LED_pulse_width = 25;  //LED pulse width in us
	//AFE4410 change
	//Added the Cf setting for TIA Gain = 1.5M
	if (LED_pulse_width >= 500)
	{
		Cf_array[0] = 250;
	}
	else if (LED_pulse_width >= 400)
	{
		Cf_array[0] = 200;
		Cf_array[1] = 200;
	}
	else if (LED_pulse_width >= 200)
	{
		Cf_array[0] = 100;
		Cf_array[1] = 100;
		Cf_array[2] = 200;
	}
	else if (LED_pulse_width >= 100)
	{
		Cf_array[0] = 50;
		Cf_array[1] = 50;
		Cf_array[2] = 100;
		Cf_array[3] = 200;
	}
	else if (LED_pulse_width >= 50)
	{
		Cf_array[0] = 25;
		Cf_array[1] = 25;
		Cf_array[2] = 50;
		Cf_array[3] = 100;
		Cf_array[4] = 200;
	}
	else if (LED_pulse_width >= 25)
	{
		Cf_array[0] = 25;
		Cf_array[1] = 25;
		Cf_array[2] = 25;
		Cf_array[3] = 50;
		Cf_array[4] = 100;
		Cf_array[5] = 200;
	}
	else
	{
		Cf_array[0] = 25;
		Cf_array[1] = 25;
		Cf_array[2] = 25;
		Cf_array[3] = 25;
		Cf_array[4] = 25;
		Cf_array[5] = 25;
		Cf_array[6] = 25;
		Cf_array[7] = 25;
		Cf_array[8] = 25;
	}

	calibration_mode = sAmbientDAC; //calibration mode is set to Ambient Cancellation
	AmbientDACcalibration_state = sInit;
	Gaincalibration_state = sInit;
	//PeriodicCalibration_state = sFreeze_periodic;
	PeriodicCalibration_state = sAmbient_periodic;

	Periodic_started = 0;
	flag.GainCalib = 0;							//flags are initialized to zero
	flag.AmbCancel = 0;

	system_param_s.ILED = ILED_CURR_MIN_code;
	//system_param_s.ILED2 = ILED_CURR_MIN_code;
	system_param_s.AMB_DAC_LED = 0;
	system_param_s.AMB_DAC_SIGN_LED = 1;          // sign: 1 - positive and 0 - negative
	system_param_s.AMB_DAC_AMB = 0;
	system_param_s.AMB_DAC_SIGN_AMB = 1;
	signal.DC = 0;
	system_param_s.RF = s50K;            //sets the initial system_param_s and signal parameters
	system_param_s.Cf = 25;

	Calibration_enabled = Calibration_en.AMB + Calibration_en.GAIN + Calibration_en.PERIODIC;

	if (Calibration_enabled != 0)
	{
		AFE44xx_Current_Register_Settings[2] = 0;
		AFE44xx_Current_Register_Settings[1] = 0;
		// Mods to support selection of either LED1 or LED2 or LED3 in MODE1 (HRM only mode) - 03/24/2015
		//AFE4410 change
		// Added the register initialization for the MSB and LSB of offset DAC settings

		if (LED_Sel == 2)
		{
			AFE44xx_Current_Register_Settings[0] = (AFE44xx_Current_Register_Settings[0] & (~LED2_mask));
			AFE44xx_Current_Register_Settings[3] = (AFE44xx_Current_Register_Settings[3] & (~AMB2_mask));
			AFE44xx_Current_Register_Settings[4] = (AFE44xx_Current_Register_Settings[4] & (~AMB2_mask_2));
		}
		else if (LED_Sel == 3)
		{
			AFE44xx_Current_Register_Settings[0] = (AFE44xx_Current_Register_Settings[0] & (~LED3_mask));
			AFE44xx_Current_Register_Settings[3] = (AFE44xx_Current_Register_Settings[3] & (~AMB3_mask));
			AFE44xx_Current_Register_Settings[4] = (AFE44xx_Current_Register_Settings[4] & (~AMB3_mask_2));
		}
		else //Default is LED1
		{
			AFE44xx_Current_Register_Settings[0] = (AFE44xx_Current_Register_Settings[0] & (~LED1_mask));
			AFE44xx_Current_Register_Settings[3] = (AFE44xx_Current_Register_Settings[3] & (~AMB1_mask));
			AFE44xx_Current_Register_Settings[4] = (AFE44xx_Current_Register_Settings[4] & (~AMB1_mask_2));
		}

		//AFE4410_Reg_Write(AFE_CONTROL0, 0x00000000); // write mode
		AFE4410_Reg_Write(AFE_LEDCNTRL, AFE44xx_Current_Register_Settings[0]);    //0x20
		//AFE4410_Reg_Write(AFE_TIAGAIN, AFE44xx_Current_Register_Settings[1]);
		AFE4410_Reg_Write(AFE_TIAAMBGAIN, AFE44xx_Current_Register_Settings[2]);
		AFE4410_Reg_Write(AFE_DAC_SETTING_REG, AFE44xx_Current_Register_Settings[3]);
		AFE4410_Reg_Write(AFE_DAC_SETTING_REG_2, AFE44xx_Current_Register_Settings[4]);
		//AFE4410_Reg_Write(AFE_CONTROL0, 0x00000001); // read mode
	}

	//Enable_sep_gain = (AFE44xx_Current_Register_Settings[1] & 0x8000)>>15;
	LOW_THR_HYS_LOW = ((LOW_THR_PERCENT - HYS_PERCENT) * ADC_FULL_SCALE) / 100;
	LOW_THR_HYS_LOW_USER = LOW_THR_HYS_LOW;
	LOW_THR_HYS_LOW_LED2 = LOW_THR_HYS_LOW;
	LOW_THR_HYS_HIGH = ((LOW_THR_PERCENT + HYS_PERCENT) * ADC_FULL_SCALE) / 100;
	HIGH_THR_HYS_LOW = ((HIGH_THR_PERCENT - HYS_PERCENT) * ADC_FULL_SCALE) / 100;
	HIGH_THR_HYS_HIGH = ((HIGH_THR_PERCENT + HYS_PERCENT) * ADC_FULL_SCALE) / 100;
	TARGET_THR = ((TARGET_THR_PERCENT * ADC_FULL_SCALE) / 100);
	DC_can = 0;
}
/*********************************************************************
* @fn      		correctamb
*
* @brief			takes in ambient value and current RF value and corrects the ambient current if any
*
* @param   		Meas_DC, Rf_current, ambient
*
* @return  		void
*
*********************************************************************/
AMB_FUNCTION_RET correctamb(long Meas_DC, RF_VALUES Rf_current, char ambient) {
	AMB_FUNCTION_RET amb_results;
	static long Meas_DC_Est_s50K = 0;
	static long Meas_DC_s50K = 0;
	static long AMB_DAC_VALUE = 0;
	int Rf_factor[9] = { 5, 2, 1, 2, 5, 10, 20, 30, 40 };
	int sign_DC = 1;
	char Rf_num;

	Rf_num = Rf_current - 1;
	if (Rf_num <= 2)			// Converts the Meas_DC to case when Rf is 50K
		Meas_DC_s50K = Meas_DC * Rf_factor[Rf_num];
	else
		Meas_DC_s50K = Meas_DC / Rf_factor[Rf_num];
	if (Meas_DC_s50K < 0) {
		Meas_DC_s50K = -1 * Meas_DC_s50K;
		sign_DC = -1;
	}
	AMB_DAC_VALUE = (((Meas_DC_s50K << 3) / ADC_CODE_AMB_DAC_STEP) + 0x4) >> 3;	//Estimate the ambient cancellation required
	if (sign_DC == 1) {
		if (system_param_s.AMB_DAC_SIGN_AMB * system_param_s.AMB_DAC_AMB + AMB_DAC_VALUE > 31) //15
			AMB_DAC_VALUE = 31 - system_param_s.AMB_DAC_SIGN_AMB * system_param_s.AMB_DAC_AMB;
	}
	else {
		if (system_param_s.AMB_DAC_SIGN_AMB * system_param_s.AMB_DAC_AMB - AMB_DAC_VALUE < -31)
			AMB_DAC_VALUE = 31 + system_param_s.AMB_DAC_SIGN_AMB * system_param_s.AMB_DAC_AMB;
	}
	//
	//  if(AMB_DAC_VALUE > 7)
	//    AMB_DAC_VALUE = 7;

	Meas_DC_Est_s50K = sign_DC
		* (Meas_DC_s50K - AMB_DAC_VALUE * ADC_CODE_AMB_DAC_STEP);
	if (Rf_num > 2)						// Convert back to the current RF case
		amb_results.Meas_DC_Est = Meas_DC_Est_s50K * Rf_factor[Rf_num];
	else
		amb_results.Meas_DC_Est = Meas_DC_Est_s50K / Rf_factor[Rf_num];

	amb_results.AMB_DAC_VAL_LED =
		(sign_DC == 1) ?
		system_param_s.AMB_DAC_SIGN_LED * system_param_s.AMB_DAC_LED
		+ AMB_DAC_VALUE :
		system_param_s.AMB_DAC_SIGN_LED * system_param_s.AMB_DAC_LED
		- AMB_DAC_VALUE;
	if (ambient == 1) {
		amb_results.AMB_DAC_VAL_AMB =
			(sign_DC == 1) ?
			system_param_s.AMB_DAC_SIGN_AMB * system_param_s.AMB_DAC_AMB
			+ AMB_DAC_VALUE :
			system_param_s.AMB_DAC_SIGN_AMB * system_param_s.AMB_DAC_AMB
			- AMB_DAC_VALUE;
	}
	else {
		amb_results.AMB_DAC_VAL_AMB = system_param_s.AMB_DAC_SIGN_AMB
			* system_param_s.AMB_DAC_AMB;
	}
	return amb_results;
}
/*********************************************************************
* @fn      		setCfValue
*
* @brief		takes in Cf in int type and update the AFE44xx_Current_Register_Settings[2] accordingly
*
* @param   		int
*
* @return  		void
*
*********************************************************************/
void setCfValue(int Cfvalue)
{
	switch (Cfvalue)
	{
	case (250):	// Set Cf = 25 pF
		AFE44xx_Current_Register_Settings[2] = (AFE44xx_Current_Register_Settings[2] & ~(0x00038)) | (0x00030);
		break;

	case (200):	// Set Cf = 20 pF
		AFE44xx_Current_Register_Settings[2] = (AFE44xx_Current_Register_Settings[2] & ~(0x00038)) | (0x00020);
		break;

	case (100):	// Set Cf = 10 pF
		AFE44xx_Current_Register_Settings[2] = (AFE44xx_Current_Register_Settings[2] & ~(0x00038)) | (0x00010);
		break;

	case (50): // Set Cf = 5 pF
		AFE44xx_Current_Register_Settings[2] = (AFE44xx_Current_Register_Settings[2] & ~(0x00038)) | (0x00000);
		break;

	case (25): // Set Cf = 2.5 pF
		AFE44xx_Current_Register_Settings[2] = (AFE44xx_Current_Register_Settings[2] & ~(0x00038)) | (0x00008);
		break;

	default: // Set Cf = 2.5 pF
		AFE44xx_Current_Register_Settings[2] = (AFE44xx_Current_Register_Settings[2] & ~(0x00038)) | (0x00008);
	}
}
/*********************************************************************
* @fn      		setRfValue
*
* @brief		takes in Rf in RF_VALUES type and update the AFE44xx_Current_Register_Settings[1] accordingly
*
* @param   		RF_VALUES
*
* @return  		void
*
*********************************************************************/
//AFE4410 change
// Updated the TIA Gain mask
void setRfValue(RF_VALUES rfvalue)
{
	switch (rfvalue)
	{
	case (s2M):	// Set TIA gain to 2 MOhm.
		AFE44xx_Current_Register_Settings[2] = (AFE44xx_Current_Register_Settings[2] & ~(0x00047)) | (0x00007);
		system_param_s.Cf = Cf_array[0];
		break;

		//AFE4410 change
		//Added a new case s1_5M
	case (s1_5M):	// Set TIA gain to 1.5 MOhm.
		AFE44xx_Current_Register_Settings[2] = (AFE44xx_Current_Register_Settings[2] & ~(0x00047)) | (0x00040);
		system_param_s.Cf = Cf_array[1];
		break;

	case (s1M): // Set TIA gain to 1 MOhm.
		AFE44xx_Current_Register_Settings[2] = (AFE44xx_Current_Register_Settings[2] & ~(0x00047)) | (0x00006);
		system_param_s.Cf = Cf_array[2];
		break;

	case (s500K): // Set TIA gain to 500 kOhm.
		AFE44xx_Current_Register_Settings[2] = (AFE44xx_Current_Register_Settings[2] & ~(0x00047)) | (0x00000);
		system_param_s.Cf = Cf_array[3];
		break;

	case (s250K): // Set TIA gain to 250 kOhm.
		AFE44xx_Current_Register_Settings[2] = (AFE44xx_Current_Register_Settings[2] & ~(0x00047)) | (0x00001);
		system_param_s.Cf = Cf_array[4];
		break;

	case (s100K): // Set TIA gain to 100 kOhm.
		AFE44xx_Current_Register_Settings[2] = (AFE44xx_Current_Register_Settings[2] & ~(0x00047)) | (0x00002);
		system_param_s.Cf = Cf_array[5];
		break;

	case (s50K): // Set TIA gain to 50 kOhm.
		AFE44xx_Current_Register_Settings[2] = (AFE44xx_Current_Register_Settings[2] & ~(0x00047)) | (0x00003);
		system_param_s.Cf = Cf_array[6];
		break;

	case (s25K): // Set TIA gain to 25 kOhm.
		AFE44xx_Current_Register_Settings[2] = (AFE44xx_Current_Register_Settings[2] & ~(0x00047)) | (0x00004);
		system_param_s.Cf = Cf_array[7];
		break;

	case (s10K): // Set TIA gain to 10 kOhm.
		AFE44xx_Current_Register_Settings[2] = (AFE44xx_Current_Register_Settings[2] & ~(0x00047)) | (0x00005);
		system_param_s.Cf = Cf_array[8];
		break;

	default: // Set Default TIA gain to 10 kOhm.
		AFE44xx_Current_Register_Settings[2] = (AFE44xx_Current_Register_Settings[2] & ~(0x00047)) | (0x00005);
		system_param_s.Cf = Cf_array[8];
	}
}

/*********************************************************************
* @fn      		AmbientCancellation
*
* @brief		Looks at the Ambient values and cancels it off using the AMBIENT_DAC
*
* @param   		int,int
*
* @return  		void
*
*********************************************************************/
void AmbientCancellation(long LEDVALUE, long AMBVALUE)
{
	AMB_FUNCTION_RET amb_results;
	static long ILEDCode, ILEDCode2;
	static RF_VALUES RFValue;
	long LEDInterimCode;

	long AMBInterimCode, AMBInterimCode_2;
	static short int waitTimeCount = 0;
	static short int blockSizeCount = 0;
	static long Meas_DC = 0;
	static long Meas_DC_s50K = 0;

	static int AMB_DAC_VALUE_LED = 0;
	static long AMB_DAC_VALUE_LED_interim = 0;
	static long AMB_DAC_VALUE_LED_interim_2 = 0;
	static int AMB_DAC_VALUE_SIGN_LED = 0;

	static int AMB_DAC_VALUE_AMB = 0;
	static long AMB_DAC_VALUE_AMB_interim = 0;
	static long AMB_DAC_VALUE_AMB_interim_2 = 0;
	static int AMB_DAC_VALUE_SIGN_AMB = 0;

	static int sign_AMB_DAC_LED = 1;
	static int sign_AMB_DAC_AMB = 1;

	int Rf_factor[9] = { 5, 2, 1, 2, 5, 10, 20, 30, 40 };
	char Rf_num;

	if (Calibration_en.AMB == 0)
		AmbientDACcalibration_state = sOver; // If ambient cancellation is not enabled by host, go to state sOver

	switch (AmbientDACcalibration_state)
	{
	case (sInit):
		/* 	Sinit initializes the ambient cancellation routine -
		sets Rf to 50K, ILED to ILED min in the initial execution
		and retains the system_param_s.Rf and system_param_s.ILED in the periodic calibration
		*/
		ILEDCode = system_param_s.ILED; // sets the ILED code to ILED min initially and the current ILED in periodic calibration
		// Mods to support selection of either LED1 or LED2 or LED3 in MODE1 (HRM only mode) - 03/24/2015
		// AFE4410 change
		// Changed the mapping of LED code to the register bits

		if (LED_Sel == 2)
		{
			// shifts the ILED code according to the current field corresponding to the LED number
			LEDInterimCode = (((ILEDCode & 0xFC) >> 2) << LED2_reg_shift) + ((ILEDCode & 0x3) << LED2_reg_shift_LSB);
			//updates the ILED register value
			AFE44xx_Current_Register_Settings[0] = (AFE44xx_Current_Register_Settings[0] & ~(LED2_mask)) | LEDInterimCode;
		}
		else if (LED_Sel == 3)
		{
			// shifts the ILED code according to the current field corresponding to the LED number
			LEDInterimCode = (((ILEDCode & 0xFC) >> 2) << LED3_reg_shift) + ((ILEDCode & 0x3) << LED3_reg_shift_LSB);
			//updates the ILED register value
			AFE44xx_Current_Register_Settings[0] = (AFE44xx_Current_Register_Settings[0] & ~(LED3_mask)) | LEDInterimCode;
		}
		else // Default is LED1
		{
			// shifts the ILED code according to the current field corresponding to the LED number
			LEDInterimCode = (((ILEDCode & 0xFC) >> 2) << LED1_reg_shift) + ((ILEDCode & 0x3) << LED1_reg_shift_LSB);
			//updates the ILED register value
			AFE44xx_Current_Register_Settings[0] = (AFE44xx_Current_Register_Settings[0] & ~(LED1_mask)) | LEDInterimCode;
		}

		RFValue = system_param_s.RF; // sets RFValue to 50K in the initial mode and retains the current value in periodic calibration
		setRfValue(RFValue);
		setCfValue(system_param_s.Cf);
		// updates the TIA GAIN register value
		if (Periodic_started == 0) // writes to the register in the initail mode only
		{
			//AFE4410_Reg_Write(AFE_CONTROL0, 0x00000000); // write mode
			AFE4410_Reg_Write(AFE_LEDCNTRL, AFE44xx_Current_Register_Settings[0]); //0x20
			//AFE4410_Reg_Write(AFE_TIAGAIN, AFE44xx_Current_Register_Settings[1]);
			AFE4410_Reg_Write(AFE_TIAAMBGAIN, AFE44xx_Current_Register_Settings[2]);
			//AFE4410_Reg_Write(AFE_CONTROL0, 0x00000001); // read mode

		}
		AmbientDACcalibration_state = sWaitTime1;			// Go to sWaitTime1
		break;

	case (sWaitTime1):
		/* Waits for 2 samples - i.e. more than 20ms - for the analog change to get settled */
		waitTimeCount++;
		if (waitTimeCount > TIMEWAIT) {
			waitTimeCount = 0;
			blockSizeCount = 0;
			Meas_DC = 0;
			AmbientDACcalibration_state = sWaitforData1;  // Go to sWaitforData1
		}
		break;

	case (sWaitforData1):
		/* Calculate the average of AMBVALUE for BLOCK_SIZE number of samples */
		blockSizeCount++;
		Meas_DC += AMBVALUE;
		if (blockSizeCount == BLOCK_SIZE) {
			blockSizeCount = 0;
			Meas_DC = (Meas_DC >> BLOCK_SIZE_BITS);
			AmbientDACcalibration_state = sCalibrate;		// Go to sCalibrate
		}
		break;

	case (sCalibrate):
		/* Calculate the Ambient cancellation required and writes it to the register */
		amb_results = correctamb(Meas_DC, RFValue, 1);
		/* calls the correctamb() function with parameters as AMBVALUE, current Rf, '1' stands for ambient cancellation
		which indicates that the cancellation has to be done in both the LED and AMB phase*/
		AMB_DAC_VALUE_SIGN_LED = amb_results.AMB_DAC_VAL_LED; // Takes in the Ambient cancellation required for the LED phase
		AMB_DAC_VALUE_SIGN_AMB = amb_results.AMB_DAC_VAL_AMB; // Takes in the Ambient cancellation required for the AMB phase

		if (AMB_DAC_VALUE_SIGN_LED >= 0)	// Isolates the sign and the Value
		{
			sign_AMB_DAC_LED = 1;
			AMB_DAC_VALUE_LED = AMB_DAC_VALUE_SIGN_LED;
		}
		else
		{
			sign_AMB_DAC_LED = 0;
			AMB_DAC_VALUE_LED = -1 * AMB_DAC_VALUE_SIGN_LED;
		}

		if (AMB_DAC_VALUE_SIGN_AMB >= 0)
		{
			sign_AMB_DAC_AMB = 1;
			AMB_DAC_VALUE_AMB = AMB_DAC_VALUE_SIGN_AMB;
		}
		else
		{
			sign_AMB_DAC_AMB = 0;
			AMB_DAC_VALUE_AMB = -1 * AMB_DAC_VALUE_SIGN_AMB;
		}

		// AFE4410 change
		// Changed the mapping of Offset DAC code to the register bits
		AMB_DAC_VALUE_LED_interim = ((AMB_DAC_VALUE_LED & 0x1E) >> 1) | (sign_AMB_DAC_LED << 4); //Calculates the register code for the corresponding Ambient cancellation required in LED phase
		AMB_DAC_VALUE_LED_interim_2 = ((AMB_DAC_VALUE_LED & 0x20) >> 4) | (AMB_DAC_VALUE_LED & 0x1);

		AMB_DAC_VALUE_AMB_interim = ((AMB_DAC_VALUE_AMB & 0x1E) >> 1) | (sign_AMB_DAC_AMB << 4); //Calculates the register code for the corresponding Ambient cancellation required in AMB phase
		AMB_DAC_VALUE_AMB_interim_2 = ((AMB_DAC_VALUE_AMB & 0x20) >> 4) | (AMB_DAC_VALUE_AMB & 0x1);


		if (LED_Sel == 2)
		{
			AMBInterimCode = (AMB_DAC_VALUE_LED_interim << AMB_reg_shift_led2) | (AMB_DAC_VALUE_AMB_interim << AMB_reg_shift_led1)
				| (AMB_DAC_VALUE_AMB_interim << AMB_reg_shift_amb2) | (AMB_DAC_VALUE_AMB_interim << AMB_reg_shift_amb1); //Interim value to be written to the register

			AMBInterimCode_2 = (AMB_DAC_VALUE_LED_interim_2 << AMB_reg_shift_led2_2) | (AMB_DAC_VALUE_AMB_interim_2 << AMB_reg_shift_led1_2)
				| (AMB_DAC_VALUE_AMB_interim_2 << AMB_reg_shift_amb2_2) | (AMB_DAC_VALUE_AMB_interim_2 << AMB_reg_shift_amb1_2); //Interim value to be written to the register
		}
		else if (LED_Sel == 3)
		{
			AMBInterimCode = (AMB_DAC_VALUE_AMB_interim << AMB_reg_shift_led2) | (AMB_DAC_VALUE_AMB_interim << AMB_reg_shift_led1)
				| (AMB_DAC_VALUE_LED_interim << AMB_reg_shift_amb2) | (AMB_DAC_VALUE_AMB_interim << AMB_reg_shift_amb1); //Interim value to be written to the register

			AMBInterimCode_2 = (AMB_DAC_VALUE_AMB_interim_2 << AMB_reg_shift_led2_2) | (AMB_DAC_VALUE_AMB_interim_2 << AMB_reg_shift_led1_2)
				| (AMB_DAC_VALUE_LED_interim_2 << AMB_reg_shift_amb2_2) | (AMB_DAC_VALUE_AMB_interim_2 << AMB_reg_shift_amb1_2); //Interim value to be written to the register
		}
		else
		{ // Default is LED1
			AMBInterimCode = (AMB_DAC_VALUE_AMB_interim << AMB_reg_shift_led2) | (AMB_DAC_VALUE_LED_interim << AMB_reg_shift_led1)
				| (AMB_DAC_VALUE_AMB_interim << AMB_reg_shift_amb2) | (AMB_DAC_VALUE_AMB_interim << AMB_reg_shift_amb1); //Interim value to be written to the register

			AMBInterimCode_2 = (AMB_DAC_VALUE_AMB_interim_2 << AMB_reg_shift_led2_2) | (AMB_DAC_VALUE_LED_interim_2 << AMB_reg_shift_led1_2)
				| (AMB_DAC_VALUE_AMB_interim_2 << AMB_reg_shift_amb2_2) | (AMB_DAC_VALUE_AMB_interim_2 << AMB_reg_shift_amb1_2); //Interim value to be written to the register
		}

		// Updates the AMB_DAC register value
		AFE44xx_Current_Register_Settings[3] = (AFE44xx_Current_Register_Settings[3] & ~(AMB123_mask)) | AMBInterimCode;
		AFE44xx_Current_Register_Settings[4] = (AFE44xx_Current_Register_Settings[4] & ~(AMB123_mask_2)) | AMBInterimCode_2;


		//AFE4410_Reg_Write(AFE_CONTROL0, 0x00000000); // write mode
		AFE4410_Reg_Write(AFE_DAC_SETTING_REG, AFE44xx_Current_Register_Settings[3]);
		AFE4410_Reg_Write(AFE_DAC_SETTING_REG_2, AFE44xx_Current_Register_Settings[4]);
		//AFE4410_Reg_Write(AFE_CONTROL0, 0x00000001); // read mode

		if ((AMB_DAC_VALUE_LED == 0) && (AMB_DAC_VALUE_AMB == 0)) // if the Ambient cancellation is non zero, Ambient_DAC_enabled is set to 1
			Ambient_DAC_enabled = 0; // This is to ensure that max Rf used is 250K whenever the Ambient DAC is used
		else
			Ambient_DAC_enabled = 1;
		AmbientDACcalibration_state = sWaitTime2;			// Go to sWaitTime2
		break;

	case (sWaitTime2):
		/* Waits for 2 samples - i.e. more than 20ms - for the analog change to get settled */
		waitTimeCount++;
		if (waitTimeCount > TIMEWAIT)
		{
			waitTimeCount = 0;
			//blockSizeCount = 0;
			Meas_DC = 0;
			AmbientDACcalibration_state = sWaitforData2;// Go to sWaitforData2
		}
		break;

	case (sWaitforData2):
		/* Calculate the average of AMBVALUE for BLOCK_SIZE number of samples */
		blockSizeCount++;
		Meas_DC += AMBVALUE;
		if (blockSizeCount == BLOCK_SIZE)
		{
			blockSizeCount = 0;
			Meas_DC = (Meas_DC >> BLOCK_SIZE_BITS);
			AmbientDACcalibration_state = sCheck;				// Go to sCheck
		}
		break;

	case (sCheck):
		/* Checks if the Ambient cancellation has actually cancelled the external ambient as expected - if not sets a flag */
		if (Meas_DC < 0)
			Meas_DC *= (-1);
		Rf_num = system_param_s.RF - 1;
		if (Rf_num < 2)	// Translates the Meas_DC to Meas_DC_s50K which means the DC value assuming the RF was 50K
			Meas_DC_s50K = Meas_DC / Rf_factor[Rf_num];
		else
			Meas_DC_s50K = Meas_DC * Rf_factor[Rf_num];

		if (Meas_DC_s50K > (ADC_CODE_AMB_DAC_MIN + ADC_CODE_AMB_DAC_STEP)) //Checks if the ambient current is less than 0.75uA
		{
			flag.AmbCancel = 0;
		}
		else
		{
			flag.AmbCancel = 1 << 4;
		}
		//    AFE44xx_Current_Register_Settings[4] = (AFE44xx_Current_Register_Settings[4] & ~(0x10)) | flag.AmbCancel; //updates the HRM_Calib_rate register value

		system_param_s.RF = RFValue;		// Updates the system_param_s and signal parameters
		system_param_s.ILED = ILEDCode;
		system_param_s.ILED2 = ILEDCode2;
		system_param_s.AMB_DAC_LED = AMB_DAC_VALUE_LED;
		system_param_s.AMB_DAC_SIGN_LED = 2 * sign_AMB_DAC_LED - 1;
		system_param_s.AMB_DAC_AMB = AMB_DAC_VALUE_AMB;
		system_param_s.AMB_DAC_SIGN_AMB = 2 * sign_AMB_DAC_AMB - 1;
		signal.DC = Meas_DC;

		AmbientDACcalibration_state = sOver;					// Go to sOver
		break;

	case (sOver):
		/* Does the termination procedure for the Ambient cancellation routine */
		if (Periodic_started == 0)
			calibration_mode = sGain; // In the initial calibration, Go to sGain mode
		else
			calibration_mode = sPeriodic; // In the periodic calibration, stay in sPeriodic mode always

		AmbientDACcalibration_state = sInit; // Go to sInit in the next execution of this procedure (which will be during periodic calibration)
		PeriodicCalibration_state = sGain_periodic;
	}
}
/*********************************************************************
* @fn      		gaincalibrate
*
* @brief			Calculate the CTR and calibrates to get good signal
*
* @param   		Meas_DC, Rf_current, ambient
*
* @return  		void
*
*********************************************************************/

void gaincalibrate(long Meas_DC, unsigned int sign_DC)//, long Meas_DC_LED2, unsigned int sign_DC_LED2)
{
	static long ILEDCode_gc, LEDInterimCode_gc, ILEDCode2_gc;
	static RF_VALUES RFValue;
	static long AMB_DAC_VAL_LED;
	static long AMB_DAC_VAL_AMB;
	static long AMB_DAC_SIGN_LED;
	static long AMB_DAC_SIGN_AMB;
	static long AMB_DAC_VAL_LED_SIGN;
	static long AMB_DAC_VAL;
	// Use this for 50mA range
	//static long Meas_DC_1K_1code_10CTR = 7;      // 1000 * 2 * 0.2mA * 10nA/1mA volts - convert into codes
	// Use this for 100mA range
	static long Meas_DC_1K_1code_10CTR = 14; // 1000 * 2 * 0.4mA * 10nA/1mA volts - convert into codes
	static long Meas_DC_1K_250nAPleth = 874; // 1000 * 0.25uA * 2
	static long Meas_DC_10CTR;
	static int AMB_DAC_VALUE_LED_interim, AMB_DAC_VALUE_LED_interim_2;
	static int sign_AMB_DAC;
	static long AMBInterimCode, AMBInterimCode_2;
	static long Ipleth_achieved;
	int Rfvalues[9] = { 10, 25, 50, 100, 250, 500, 1000, 1500, 2000 };
	int Rf_int;

	RFValue = system_param_s.RF;				//takes in the current system_param_s parameters
	ILEDCode_gc = system_param_s.ILED;
	//ILEDCode2_gc = system_param_s.ILED2;
	AMB_DAC_VAL_LED = system_param_s.AMB_DAC_LED;
	AMB_DAC_SIGN_LED = system_param_s.AMB_DAC_SIGN_LED;
	AMB_DAC_VAL_AMB = system_param_s.AMB_DAC_AMB;
	AMB_DAC_SIGN_AMB = system_param_s.AMB_DAC_SIGN_AMB;

	Rf_int = Rfvalues[(int)RFValue - 1];

	Meas_DC_10CTR = Meas_DC_1K_1code_10CTR * Rf_int * ILEDCode_gc; //Calculates the DC value assuming that the CTR is 10nA/mA ==>

	// Updates the Meas_DC by incorporating the DC cancellation done at the input if any
	Meas_DC = ((2 * sign_DC - 1) * Meas_DC) + ((AMB_DAC_SIGN_LED * AMB_DAC_VAL_LED - AMB_DAC_SIGN_AMB * AMB_DAC_VAL_AMB) * Meas_DC_1K_250nAPleth * Rf_int);

	if (Meas_DC < 0)
	{
		sign_DC = 0;
		Meas_DC = (-1) * Meas_DC;
	}
	else
		sign_DC = 1;

	CTR = (Meas_DC * 10) / Meas_DC_10CTR;		// Calculates the CTR
	ILEDCode_gc = Ipleth << 4; // Estimates the ILED required to achieve a pleth current of 3uA
	ILEDCode_gc = ILEDCode_gc / CTR;
	ILEDCode_gc = ILEDCode_gc + 0x08;
	ILEDCode_gc = ILEDCode_gc >> 4;

	//ILEDCode_gc = ((Ipleth<<4)/CTR + 0x8)>>4;  // Estimates the ILED required to achieve a pleth current of 3uA
	Ipleth_achieved = ILEDCode_gc * CTR;
	AMB_DAC_VAL = ((LED_DC_can * (Ipleth_achieved << 4)) / Ipleth + 0x8) >> 4;		// Calculates the DC cancellation required
	RFValue = s250K;

	// Limits the ILED by max and min values and adjust the DC cancellation appropriately
	if (ILEDCode_gc > ILED_CURR_MAX_code)
	{
		AMB_DAC_VAL = (AMB_DAC_VAL * ILED_CURR_MAX_code) / ILEDCode_gc;
		ILEDCode_gc = ILED_CURR_MAX_code;// Pick the max ILED if the ILED code required was higher
	}
	else
	{
		if (ILEDCode_gc < ILED_CURR_MIN_code)
		{
			AMB_DAC_VAL = (AMB_DAC_VAL * ILED_CURR_MIN_code) / ILEDCode_gc;
			ILEDCode_gc = ILED_CURR_MIN_code;
		}
	}

	if (ILEDCode2_gc > ILED_CURR_MAX_code)
		ILEDCode2_gc = ILED_CURR_MAX_code;// Pick the max ILED if the ILED code required was higher
	else
	{
		if (ILEDCode2_gc < ILED_CURR_MIN_code)
			ILEDCode2_gc = ILED_CURR_MIN_code;
	}

	if ((AMB_DAC_VAL >= 33) && (AMB_DAC_VAL <= 40))
		RFValue = s100K;
	if ((AMB_DAC_VAL >= 41) && (AMB_DAC_VAL <= 48))
		RFValue = s50K;
	if ((AMB_DAC_VAL >= 49) && (AMB_DAC_VAL <= 56))
		RFValue = s25K;
	if (AMB_DAC_VAL >= 57)
		RFValue = s10K;

	if (AMB_DAC_VAL > 32) //AMB DAC range is divided as 8 for LED phase only and 7 for ambient cancellation
		AMB_DAC_VAL = 32;

	if (AMB_DAC_VAL > 0)
		DC_can = 1;
	else
		DC_can = 0;

	// Mods to support selection of either LED1 or LED2 or LED3 in MODE1 (HRM only mode) - 03/24/2015
	// AFE4410 change
	// Changed the mapping of LED code to the register bits
	if (LED_Sel == 2)
	{
		// shifts the ILED code according to the current field corresponding to the LED number
		LEDInterimCode_gc = (((ILEDCode_gc & 0xFC) >> 2) << LED2_reg_shift) + ((ILEDCode_gc & 0x3) << LED2_reg_shift_LSB);
		//updates the ILED register value
		AFE44xx_Current_Register_Settings[0] = (AFE44xx_Current_Register_Settings[0] & ~(LED2_mask)) | LEDInterimCode_gc;
	}
	else if (LED_Sel == 3)
	{
		// shifts the ILED code according to the current field corresponding to the LED number
		LEDInterimCode_gc = (((ILEDCode_gc & 0xFC) >> 2) << LED3_reg_shift) + ((ILEDCode_gc & 0x3) << LED3_reg_shift_LSB);
		//updates the ILED register value
		AFE44xx_Current_Register_Settings[0] = (AFE44xx_Current_Register_Settings[0] & ~(LED3_mask)) | LEDInterimCode_gc;
	}
	else
	{ // Default is LED1
	  // shifts the ILED code according to the current field corresponding to the LED number
		LEDInterimCode_gc = (((ILEDCode_gc & 0xFC) >> 2) << LED1_reg_shift) + ((ILEDCode_gc & 0x3) << LED1_reg_shift_LSB);
		//updates the ILED register value
		AFE44xx_Current_Register_Settings[0] = (AFE44xx_Current_Register_Settings[0] & ~(LED1_mask)) | LEDInterimCode_gc;
	}

	setRfValue(RFValue);
	setCfValue(system_param_s.Cf);
	// Update the TIA GAIN register value
	AMB_DAC_VAL_LED_SIGN = (sign_DC == 1) ? AMB_DAC_SIGN_AMB * AMB_DAC_VAL_AMB + AMB_DAC_VAL : AMB_DAC_SIGN_AMB * AMB_DAC_VAL_AMB - AMB_DAC_VAL;
	if (AMB_DAC_VAL_LED_SIGN >= 0)
	{
		sign_AMB_DAC = 1;
		AMB_DAC_VAL_LED = AMB_DAC_VAL_LED_SIGN;
	}
	else
	{
		sign_AMB_DAC = 0;
		AMB_DAC_VAL_LED = -1 * AMB_DAC_VAL_LED_SIGN;
	}


	AMB_DAC_VALUE_LED_interim = ((AMB_DAC_VAL_LED & 0x1E) >> 1) | (sign_AMB_DAC << 4); //Calculates the register code for the corresponding Ambient cancellation required in LED phase
	AMB_DAC_VALUE_LED_interim_2 = ((AMB_DAC_VAL_LED & 0x20) >> 4) | (sign_AMB_DAC & 0x1);


	// Mods to support selection of either LED1 or LED2 or LED3 in MODE1 (HRM only mode) - 03/24/2015
	// AFE4410 change
	// Changed the mapping of offset DAC code to the register bits
	if (LED_Sel == 2)
	{
		//Interim value to be written to the register
		AMBInterimCode = (AMB_DAC_VALUE_LED_interim << AMB_reg_shift_led2);
		AMBInterimCode_2 = (AMB_DAC_VALUE_LED_interim_2 << AMB_reg_shift_led2_2);
		// Updates the AMB_DAC register value
		AFE44xx_Current_Register_Settings[3] = (AFE44xx_Current_Register_Settings[3] & ~(AMB2_mask_LED)) | AMBInterimCode;
		AFE44xx_Current_Register_Settings[4] = (AFE44xx_Current_Register_Settings[4] & ~(AMB2_mask_LED_2)) | AMBInterimCode_2;
	}
	else if (LED_Sel == 3)
	{
		//Interim value to be written to the register
		AMBInterimCode = (AMB_DAC_VALUE_LED_interim << AMB_reg_shift_amb2);
		AMBInterimCode_2 = (AMB_DAC_VALUE_LED_interim_2 << AMB_reg_shift_amb2_2);
		// Updates the AMB_DAC register value
		AFE44xx_Current_Register_Settings[3] = (AFE44xx_Current_Register_Settings[3] & ~(AMB3_mask_LED)) | AMBInterimCode;
		AFE44xx_Current_Register_Settings[4] = (AFE44xx_Current_Register_Settings[4] & ~(AMB3_mask_LED_2)) | AMBInterimCode_2;
	}
	else
	{ // Default is LED1
	  //Interim value to be written to the register
		AMBInterimCode = (AMB_DAC_VALUE_LED_interim << AMB_reg_shift_led1);
		AMBInterimCode_2 = (AMB_DAC_VALUE_LED_interim_2 << AMB_reg_shift_led1_2);
		// Updates the AMB_DAC register value
		AFE44xx_Current_Register_Settings[3] = (AFE44xx_Current_Register_Settings[3] & ~(AMB1_mask_LED)) | AMBInterimCode;
		AFE44xx_Current_Register_Settings[4] = (AFE44xx_Current_Register_Settings[4] & ~(AMB1_mask_LED_2)) | AMBInterimCode_2;
	}

	//Updates the system_param_s parameters
	system_param_s.RF = RFValue;
	system_param_s.ILED = ILEDCode_gc;
	system_param_s.ILED2 = ILEDCode2_gc;
	system_param_s.AMB_DAC_LED = AMB_DAC_VAL_LED;
	system_param_s.AMB_DAC_SIGN_LED = 2 * sign_AMB_DAC - 1;
}
/*********************************************************************
* @fn      		GainCalibration
*
* @brief		Looks at the LED values and tries to bring it to TARGET_THR by varying TIA gain and LED currents
*
* @param   		long, long
*
* @return  		void
*
*********************************************************************/
void GainCalibration(long LEDMINUSAMBVALUE, long LEDVALUE)
{
	static long LEDInterimCode;
	static short int waitTimeCount = 0;
	static short int blockSizeCount = 0;
	static long Meas_DC = 0;
	static int sign_DC;
	static long Meas_DC_LED2 = 0;
	//static int sign_DC_LED2;
	static RF_VALUES RFValue;
	static long AMB_DAC_VALUE_AMB, sign_AMB_DAC_AMB, AMB_DAC_VALUE_AMB_interim, AMBInterimCode, AMB_DAC_VALUE_AMB_interim_2, AMBInterimCode_2;
	static long AMB_DAC_VALUE_LED_interim_UPDATED, AMBInterimCode_UPDATED, AMB_DAC_VALUE_LED_interim_UPDATED_2, AMBInterimCode_UPDATED_2;
	static long AMB_DAC_VALUE_SIGN_UPDATED, sign_AMB_DAC_UPDATED, AMB_DAC_VALUE_UPDATED;

	if (Calibration_en.GAIN == 0)
		Gaincalibration_state = sOver;
	switch (Gaincalibration_state)
	{
	case (sInit):
		/* Sets Rf to 250K/2M depending on whether the Ambient DAC is enabled or not and ILED to min ILED in the initial calibration. */
		system_param_s.ILED = ILED_CURR_MIN_code; // sets the ILED to minimum ILED always (both in initial calibration and periodic calibration
		//system_param_s.ILED2 = ILED_CURR_MIN_code;
		// Mods to support selection of either LED1 or LED2 or LED3 in MODE1 (HRM only mode) - 03/24/2015
		// AFE4410 change
		// Changed the mapping of LED code to the register bits
		if (LED_Sel == 2)
		{
			// shifts the ILED code according to the current field corresponding to the LED number
			LEDInterimCode = (((ILED_CURR_MIN_code & 0xFC) >> 2) << LED2_reg_shift) + ((ILED_CURR_MIN_code & 0x3) << LED2_reg_shift_LSB);
			//updates the ILED register value
			AFE44xx_Current_Register_Settings[0] = (AFE44xx_Current_Register_Settings[0] & ~(LED2_mask)) | LEDInterimCode;
		}
		else if (LED_Sel == 3)
		{
			// shifts the ILED code according to the current field corresponding to the LED number
			LEDInterimCode = (((ILED_CURR_MIN_code & 0xFC) >> 2) << LED3_reg_shift) + ((ILED_CURR_MIN_code & 0x3) << LED3_reg_shift_LSB);
			//updates the ILED register value
			AFE44xx_Current_Register_Settings[0] = (AFE44xx_Current_Register_Settings[0] & ~(LED3_mask)) | LEDInterimCode;
		}
		else // Default is LED1
		{
			// shifts the ILED code according to the current field corresponding to the LED number
			LEDInterimCode = (((ILED_CURR_MIN_code & 0xFC) >> 2) << LED1_reg_shift) + ((ILED_CURR_MIN_code & 0x3) << LED1_reg_shift_LSB);
			//updates the ILED register value
			AFE44xx_Current_Register_Settings[0] = (AFE44xx_Current_Register_Settings[0] & ~(LED1_mask)) | LEDInterimCode;
		}
		RFValue = system_param_s.RF;

		if (Ambient_DAC_enabled == 1) // If Ambient cancellation is non zero, then use 250K as the max Rf, otherwise 2M
			RFValue_Init_GainCal = s250K;
		else
			RFValue_Init_GainCal = s2M;

		RFValue = RFValue_Init_GainCal;
		setRfValue(RFValue);
		setCfValue(system_param_s.Cf);
		// Updates the TIA GAIN register value
		//AFE4410_Reg_Write(AFE_CONTROL0, 0x00000000); // write mode
		AFE4410_Reg_Write(AFE_LEDCNTRL, AFE44xx_Current_Register_Settings[0]);    //0x20
		//AFE4410_Reg_Write(AFE_TIAGAIN, AFE44xx_Current_Register_Settings[1]);
		AFE4410_Reg_Write(AFE_TIAAMBGAIN, AFE44xx_Current_Register_Settings[2]);
		//AFE4410_Reg_Write(AFE_CONTROL0, 0x00000001); // read mode

		Gaincalibration_state = sWaitTime1;					// Go to sWaitTime1
		break;

	case (sWaitTime1):
		/* Waits for 2 samples - i.e. more than 20ms - for the analog change to get settled */
		waitTimeCount++;
		if (waitTimeCount > TIMEWAIT)
		{
			waitTimeCount = 0;
			Meas_DC = 0;
			//Meas_DC_LED2 = 0;
			Gaincalibration_state = sWaitforData1;		// Go to sWaitforData1
		}
		break;

	case (sWaitforData1):
		/* Calculate the average of LEDVALUE for BLOCK_SIZE number of samples */
		blockSizeCount++;
		Meas_DC += LEDMINUSAMBVALUE;
		//Meas_DC_LED2 += LED2VALUE;
		if (blockSizeCount == BLOCK_SIZE)
		{
			blockSizeCount = 0;
			Meas_DC = (Meas_DC >> BLOCK_SIZE_BITS);
			//Meas_DC_LED2 = (Meas_DC_LED2 >> BLOCK_SIZE_BITS);
			Gaincalibration_state = sGainadjust;			// Go to sGainadjust
		}
		break;

	case (sGainadjust):
		/* Sets the Rf such that LEDVALUE will less than Target threshold - Done only in initial calibration*/
		if (Meas_DC < 0)// Convert the Meas_DC to positive and store the sign information separately
		{
			Meas_DC = -1 * Meas_DC;
			sign_DC = 0;
		}
		else
			sign_DC = 1;

		//    if (Meas_DC_LED2 < 0)// Convert the Meas_DC to positive and store the sign information separately
		//    {
		//      Meas_DC_LED2 = -1 * Meas_DC_LED2;
		//      sign_DC_LED2 = 0;
		//    }
		//	else
		//      sign_DC_LED2 = 1;

		if ((Meas_DC > TARGET_THR)) //|| (Meas_DC_LED2 > TARGET_THR))// The sInit had set the appropriate Rf and minimmum ILED. Now if the Meas_DC is still higher than Target threshold, it enters this condition
		{
			if (RFValue > s10K)	// Keep decrementing the Rf till Meas_DC becomes less than Target threshold
			{
				RFValue--;
				setRfValue(RFValue);
				setCfValue(system_param_s.Cf);
				// Update the TIA GAIN register value
				//AFE4410_Reg_Write(AFE_CONTROL0, 0x00000000); // write mode
				//AFE4410_Reg_Write(AFE_TIAGAIN, AFE44xx_Current_Register_Settings[1]);
				AFE4410_Reg_Write(AFE_TIAAMBGAIN, AFE44xx_Current_Register_Settings[2]);
				//AFE4410_Reg_Write(AFE_CONTROL0, 0x00000001); // read mode
				Gaincalibration_state = sWaitTime1;			// Go to sWaitTime1
			}
			else
				Gaincalibration_state = sCalibrate;
		}
		else
			Gaincalibration_state = sCalibrate;
		system_param_s.RF = RFValue;
		break;

	case (sCalibrate):
		/* calls the gaincalibrate function and update the register settings */
		gaincalibrate(Meas_DC, sign_DC);//, Meas_DC_LED2, sign_DC_LED2);// This function estimates the CTR and ajust gain,ambdac and ledcurrent accordingly

		//AFE4410_Reg_Write(AFE_CONTROL0, 0x00000000); // write mode
		AFE4410_Reg_Write(AFE_DAC_SETTING_REG, AFE44xx_Current_Register_Settings[3]);
		AFE4410_Reg_Write(AFE_DAC_SETTING_REG_2, AFE44xx_Current_Register_Settings[4]);
		AFE4410_Reg_Write(AFE_LEDCNTRL, AFE44xx_Current_Register_Settings[0]);    //0x20
		//AFE4410_Reg_Write(AFE_TIAGAIN, AFE44xx_Current_Register_Settings[1]);
		AFE4410_Reg_Write(AFE_TIAAMBGAIN, AFE44xx_Current_Register_Settings[2]);
		//AFE4410_Reg_Write(AFE_CONTROL0, 0x00000001); // read mode

		Gaincalibration_state = sWaitTime2a;			// Go to sWaitTime2a
		break;

	case (sWaitTime2a):
		waitTimeCount++;
		if (waitTimeCount > TIMEWAIT)
		{
			waitTimeCount = 0;
			Meas_DC = 0;
			//Meas_DC_LED2 = 0;
			Gaincalibration_state = sWaitforData2a;		// Go to sWaitforData2
		}
		break;

	case (sWaitforData2a):
		/* Calculate the average of LEDVALUE for BLOCK_SIZE number of samples */
		blockSizeCount++;
		Meas_DC += LEDMINUSAMBVALUE;
		//Meas_DC_LED2 += LED2VALUE;
		if (blockSizeCount == BLOCK_SIZE)
		{
			blockSizeCount = 0;
			Meas_DC = (Meas_DC >> BLOCK_SIZE_BITS);
			//Meas_DC_LED2 = (Meas_DC_LED2 >> BLOCK_SIZE_BITS);
			Gaincalibration_state = sCheck1;					// Go to sCheck1
		}
		break;

	case (sCheck1):
		if (Meas_DC < 0)
		{
			// AFE4410 change
			// Updated the thresholds
			if (Meas_DC < -1310720) //equivalent to -1.5uA pleth current with Rf = 250K
			{
				AMB_DAC_VALUE_SIGN_UPDATED = system_param_s.AMB_DAC_SIGN_LED * system_param_s.AMB_DAC_LED - 7; // Subtract 1.75uA dac current.
			}
			else if (Meas_DC < -1092266) //equivalent to -1.25uA pleth current with Rf = 250K
			{
				AMB_DAC_VALUE_SIGN_UPDATED = system_param_s.AMB_DAC_SIGN_LED * system_param_s.AMB_DAC_LED - 6; // Subtract 1.5uA dac current.
			}
			else if (Meas_DC < -873813) //equivalent to -1.25uA pleth current with Rf = 250K
			{
				AMB_DAC_VALUE_SIGN_UPDATED = system_param_s.AMB_DAC_SIGN_LED * system_param_s.AMB_DAC_LED - 5; // Subtract 1.25uA dac current.
			}
			else if (Meas_DC < -655360) //equivalent to -0.75uA pleth current with Rf = 250K
			{
				AMB_DAC_VALUE_SIGN_UPDATED = system_param_s.AMB_DAC_SIGN_LED * system_param_s.AMB_DAC_LED - 4; // Subtract 1uA dac current.
			}
			else if (Meas_DC < -436906) //equivalent to -0.75uA pleth current with Rf = 250K
			{
				AMB_DAC_VALUE_SIGN_UPDATED = system_param_s.AMB_DAC_SIGN_LED * system_param_s.AMB_DAC_LED - 3; // Subtract 0.75uA dac current.
			}
			else if (Meas_DC < -218453) //equivalent to -0.25u pleth current with Rf = 250K
			{
				AMB_DAC_VALUE_SIGN_UPDATED = system_param_s.AMB_DAC_SIGN_LED * system_param_s.AMB_DAC_LED - 2; // Subtract 0.5uA dac current.
			}
			else if (Meas_DC < 0) //equivalent to 0u pleth current with Rf = 250K
			{
				AMB_DAC_VALUE_SIGN_UPDATED = system_param_s.AMB_DAC_SIGN_LED * system_param_s.AMB_DAC_LED - 1; // Subtract 0.25uA dac current.
			}
			else
			{
				AMB_DAC_VALUE_SIGN_UPDATED = system_param_s.AMB_DAC_SIGN_LED * system_param_s.AMB_DAC_LED; // Subtract 0uA dac current.
			}

			if (AMB_DAC_VALUE_SIGN_UPDATED >= 0)
			{
				AMB_DAC_VALUE_UPDATED = AMB_DAC_VALUE_SIGN_UPDATED;
				sign_AMB_DAC_UPDATED = 1;
			}
			else
			{
				AMB_DAC_VALUE_UPDATED = (-1) * AMB_DAC_VALUE_SIGN_UPDATED;
				sign_AMB_DAC_UPDATED = 0;
			}

			// AFE4410 change
			// Changed the mapping of offset DAC code to the register bits
			AMB_DAC_VALUE_LED_interim_UPDATED = ((AMB_DAC_VALUE_UPDATED & 0x1E) >> 1) | (sign_AMB_DAC_UPDATED << 4); //Calculates the register code for the corresponding Ambient cancellation required in LED phase
			AMB_DAC_VALUE_LED_interim_UPDATED_2 = ((AMB_DAC_VALUE_UPDATED & 0x20) >> 4) | (sign_AMB_DAC_UPDATED & 0x1);



			if (LED_Sel == 2)
			{
				//Interim value to be written to the register
				AMBInterimCode_UPDATED = (AMB_DAC_VALUE_LED_interim_UPDATED << AMB_reg_shift_led2);
				AMBInterimCode_UPDATED_2 = (AMB_DAC_VALUE_LED_interim_UPDATED_2 << AMB_reg_shift_led2_2);
				// Updates the AMB_DAC register value
				AFE44xx_Current_Register_Settings[3] = (AFE44xx_Current_Register_Settings[3] & ~(AMB2_mask_LED)) | AMBInterimCode_UPDATED;
				AFE44xx_Current_Register_Settings[4] = (AFE44xx_Current_Register_Settings[4] & ~(AMB2_mask_LED_2)) | AMBInterimCode_UPDATED_2;
			}
			else if (LED_Sel == 3)
			{
				//Interim value to be written to the register
				AMBInterimCode_UPDATED = (AMB_DAC_VALUE_LED_interim_UPDATED << AMB_reg_shift_amb2);
				AMBInterimCode_UPDATED_2 = (AMB_DAC_VALUE_LED_interim_UPDATED_2 << AMB_reg_shift_amb2_2);
				// Updates the AMB_DAC register value
				AFE44xx_Current_Register_Settings[3] = (AFE44xx_Current_Register_Settings[3] & ~(AMB3_mask_LED)) | AMBInterimCode_UPDATED;
				AFE44xx_Current_Register_Settings[4] = (AFE44xx_Current_Register_Settings[4] & ~(AMB3_mask_LED_2)) | AMBInterimCode_UPDATED_2;
			}
			else
			{ // Default is LED1
			  //Interim value to be written to the register
				AMBInterimCode_UPDATED = (AMB_DAC_VALUE_LED_interim_UPDATED << AMB_reg_shift_led1);
				AMBInterimCode_UPDATED_2 = (AMB_DAC_VALUE_LED_interim_UPDATED_2 << AMB_reg_shift_led1_2);
				// Updates the AMB_DAC register value
				AFE44xx_Current_Register_Settings[3] = (AFE44xx_Current_Register_Settings[3] & ~(AMB1_mask_LED)) | AMBInterimCode_UPDATED;
				AFE44xx_Current_Register_Settings[4] = (AFE44xx_Current_Register_Settings[4] & ~(AMB1_mask_LED_2)) | AMBInterimCode_UPDATED_2;
			}


			//AFE4410_Reg_Write(AFE_CONTROL0, 0x00000000); // write mode
			AFE4410_Reg_Write(AFE_DAC_SETTING_REG, AFE44xx_Current_Register_Settings[3]);
			AFE4410_Reg_Write(AFE_DAC_SETTING_REG_2, AFE44xx_Current_Register_Settings[4]);
			//AFE4410_Reg_Write(AFE_CONTROL0, 0x00000001); // read mode
			system_param_s.AMB_DAC_LED = AMB_DAC_VALUE_UPDATED;
			system_param_s.AMB_DAC_SIGN_LED = 2 * sign_AMB_DAC_UPDATED - 1;
		}
		Gaincalibration_state = sWaitTime2b;
		break;

	case (sWaitTime2b):
		/* Waits for 2 samples - i.e. more than 20ms - for the analog change to get settled */
		waitTimeCount++;
		if (waitTimeCount > TIMEWAIT)
		{
			waitTimeCount = 0;
			Meas_DC = 0;
			//Meas_DC_LED2 = 0;
			Gaincalibration_state = sWaitforData2b;		// Go to sWaitforData2
		}
		break;

	case (sWaitforData2b):
		/* Calculate the average of LEDVALUE for BLOCK_SIZE number of samples */
		blockSizeCount++;
		Meas_DC += LEDMINUSAMBVALUE;
		//Meas_DC_LED2 += LED2VALUE;
		if (blockSizeCount == BLOCK_SIZE)
		{
			blockSizeCount = 0;
			Meas_DC = (Meas_DC >> BLOCK_SIZE_BITS);
			//Meas_DC_LED2 = (Meas_DC_LED2 >> BLOCK_SIZE_BITS);
			Gaincalibration_state = sCheck2;					// Go to sCheck
		}
		break;

	case (sCheck2):
		if (Meas_DC < 0)
			Meas_DC *= (-1);
		if (Meas_DC > 0)
		{
			RFValue = system_param_s.RF;
			if (Meas_DC <= ADC_ONE_PERCENT_FS)
			{
				// AFE4410 change
				// changed for Rf = 1.5 M
				if (RFValue < s500K) // Keep incrementing the Rf till Meas_DC becomes closer to Target threshold
				{
					RFValue += 2;
					setRfValue(RFValue);
					setCfValue(system_param_s.Cf);
				}
				else if (RFValue < s1M) // Keep incrementing the Rf till Meas_DC becomes closer to Target threshold
				{
					RFValue += 3;
					setRfValue(RFValue);
					setCfValue(system_param_s.Cf);
				}
				else if (RFValue < s1_5M) // Keep incrementing the Rf till Meas_DC becomes closer to Target threshold
				{
					RFValue += 2;
					setRfValue(RFValue);
					setCfValue(system_param_s.Cf);
				}
				else if (RFValue < s2M) // Keep incrementing the Rf till Meas_DC becomes closer to Target threshold
				{
					RFValue += 1;
					setRfValue(RFValue);
					setCfValue(system_param_s.Cf);
				}

			}
			else if (Meas_DC <= (3 * ADC_ONE_PERCENT_FS))
			{
				if (RFValue < s2M) // Keep incrementing the Rf till Meas_DC becomes closer to Target threshold
				{
					RFValue += 1;
					setRfValue(RFValue);
					setCfValue(system_param_s.Cf);
				}
			}
			// Update the TIA GAIN register value
			//AFE4410_Reg_Write(AFE_CONTROL0, 0x00000000); // write mode
			//AFE4410_Reg_Write(AFE_TIAGAIN, AFE44xx_Current_Register_Settings[1]);
			AFE4410_Reg_Write(AFE_TIAAMBGAIN, AFE44xx_Current_Register_Settings[2]);
			//AFE4410_Reg_Write(AFE_CONTROL0, 0x00000001); // read mode
			system_param_s.RF = RFValue;
		}
		Gaincalibration_state = sWaitTime2;
		break;

	case (sWaitTime2):
		/* Waits for 2 samples - i.e. more than 20ms - for the analog change to get settled */
		waitTimeCount++;
		if (waitTimeCount > TIMEWAIT)
		{
			waitTimeCount = 0;
			Meas_DC = 0;
			//Meas_DC_LED2 = 0;
			Gaincalibration_state = sWaitforData2;		// Go to sWaitforData2
		}
		break;

	case (sWaitforData2):
		/* Calculate the average of LEDVALUE for BLOCK_SIZE number of samples */
		blockSizeCount++;
		Meas_DC += LEDMINUSAMBVALUE;
		//Meas_DC_LED2 += LED2VALUE;
		if (blockSizeCount == BLOCK_SIZE)
		{
			blockSizeCount = 0;
			Meas_DC = (Meas_DC >> BLOCK_SIZE_BITS);
			//Meas_DC_LED2 = (Meas_DC_LED2 >> BLOCK_SIZE_BITS);
			Gaincalibration_state = sCheck;						// Go to sCheck
		}
		break;

	case (sCheck):
		/* Checks if the Gain calibration has actually worked as expected - if not sets a flag */
		LOW_THR_HYS_LOW = Meas_DC - 419430;

		if (LOW_THR_HYS_LOW > LOW_THR_HYS_LOW_USER)
			LOW_THR_HYS_LOW = LOW_THR_HYS_LOW_USER;

		if (DC_can == 0)
		{
			if (LOW_THR_HYS_LOW < 104857) 		// < 5%
				LOW_THR_HYS_LOW = 104857;
		}
		else
		{
			if (LOW_THR_HYS_LOW < -1048576) 	// < -50%
				LOW_THR_HYS_LOW = -1048576;
		}

		if (Meas_DC < 0)
			Meas_DC *= (-1);
		//if (Meas_DC_LED2 < 0)
		//	Meas_DC_LED2 *= (-1);
		//if((Meas_DC > TARGET_THR) || (Meas_DC_LED2 > TARGET_THR))
		if (Meas_DC > TARGET_THR)
			flag.GainCalib = 0;
		else
			flag.GainCalib = 1 << 5;
		// AFE44xx_Current_Register_Settings[4] = (AFE44xx_Current_Register_Settings[4] & ~(0x20)) | flag.GainCalib; //Update the HRM_Calib_rate register value

		signal.DC = Meas_DC;
		Gaincalibration_state = sOver;							// Go to sOver
		break;

	case (sOver):
		/* Does the termination procedure for the Gain calibration routine */
		Gaincalibration_state = sWaitforData3;// Go to sWaitforData3 in the next execution of this function
		Calibration = 0;
		if (Calibration_en.PERIODIC == 1)
		{
			calibration_mode = sPeriodic;
			Periodic_started = 1;
		}
		else
			calibration_mode = sFinish;

		//PeriodicCalibration_state = sFreeze_periodic;
		PeriodicCalibration_state = sAmbient_periodic;
		break;

	case (sWaitforData3):
		/* Calculate the average of LEDVALUE for BLOCK_SIZE number of samples - Also checks if the Gain calibration has to be done or not*/
		if (blockSizeCount == 0)
		{
			Meas_DC = 0;
			//Meas_DC_LED2 = 0;
		}
		blockSizeCount++;
		Meas_DC += LEDVALUE;
		Meas_DC_LED2 += LEDMINUSAMBVALUE;
		if (blockSizeCount == BLOCK_SIZE)
		{
			blockSizeCount = 0;
			Meas_DC = (Meas_DC >> BLOCK_SIZE_BITS);
			Meas_DC_LED2 = (Meas_DC_LED2 >> BLOCK_SIZE_BITS);

			if ((Meas_DC > HIGH_THR_HYS_HIGH) || (Meas_DC_LED2 < LOW_THR_HYS_LOW))
				periodic_cal_req = 1;
			else
				periodic_cal_req = 0;

			//if((Meas_DC > HIGH_THR_HYS_HIGH) || (Meas_DC < LOW_THR_HYS_LOW) || (Meas_DC_LED2 > HIGH_THR_HYS_HIGH) || (Meas_DC_LED2 < LOW_THR_HYS_LOW_LED2))
			// If Meas_DC is outside the low or high threshold
			if (periodic_cal_req == 1)
			{
				if (Meas_DC < 0)// Convert the Meas_DC to positive and store the sign information separately
				{
					Meas_DC = -1 * Meas_DC;
					sign_DC = 0;
				}
				else
					sign_DC = 1;
				//if (Meas_DC_LED2 < 0)// Convert the Meas_DC to positive and store the sign information separately
				//{
				//	Meas_DC_LED2 = -1 * Meas_DC_LED2;
				//	sign_DC_LED2 = 0;
				//}
				//else
				//	sign_DC_LED2 = 1;

				if ((Meas_DC > 1887436))// || (Meas_DC_LED2 > 1887436)) // Meas DC > 90%
				{
					system_param_s.ILED = ILED_CURR_MIN_code; // sets the ILED to minimum ILED
					//system_param_s.ILED2 = ILED_CURR_MIN_code;

					// Mods to support selection of either LED1 or LED2 or LED3 in MODE1 (HRM only mode) - 03/24/2015

					// AFE4410 change
					// Changed the mapping of LED code to the register bits

					if (LED_Sel == 2)
					{
						// shifts the ILED code according to the current field corresponding to the LED number
						LEDInterimCode = (((ILED_CURR_MIN_code & 0xFC) >> 2) << LED2_reg_shift) + ((ILED_CURR_MIN_code & 0x3) << LED2_reg_shift_LSB);
						//updates the ILED register value
						AFE44xx_Current_Register_Settings[0] = (AFE44xx_Current_Register_Settings[0] & ~(LED2_mask)) | LEDInterimCode;
					}
					else if (LED_Sel == 3)
					{
						// shifts the ILED code according to the current field corresponding to the LED number
						LEDInterimCode = (((ILED_CURR_MIN_code & 0xFC) >> 2) << LED3_reg_shift) + ((ILED_CURR_MIN_code & 0x3) << LED3_reg_shift_LSB);
						//updates the ILED register value
						AFE44xx_Current_Register_Settings[0] = (AFE44xx_Current_Register_Settings[0] & ~(LED3_mask)) | LEDInterimCode;
					}
					else
					{ // Default is LED1
					  // shifts the ILED code according to the current field corresponding to the LED number
						LEDInterimCode = (((ILED_CURR_MIN_code & 0xFC) >> 2) << LED1_reg_shift) + ((ILED_CURR_MIN_code & 0x3) << LED1_reg_shift_LSB);
						//updates the ILED register value
						AFE44xx_Current_Register_Settings[0] = (AFE44xx_Current_Register_Settings[0] & ~(LED1_mask)) | LEDInterimCode;
					}

					AMB_DAC_VALUE_AMB = system_param_s.AMB_DAC_AMB;
					sign_AMB_DAC_AMB = (1 + system_param_s.AMB_DAC_SIGN_AMB) >> 1;

					// AFE4410 change
					// Changed the mapping of offset DAC code to the register bits

					AMB_DAC_VALUE_AMB_interim = ((AMB_DAC_VALUE_AMB & 0x1E) >> 1) | (sign_AMB_DAC_AMB << 4); //Calculates the register code for the corresponding Ambient cancellation required in LED phase
					AMB_DAC_VALUE_AMB_interim_2 = ((AMB_DAC_VALUE_AMB & 0x20) >> 4) | (sign_AMB_DAC_AMB & 0x1);

					if (LED_Sel == 2)
					{
						//Interim value to be written to the register
						AMBInterimCode = (AMB_DAC_VALUE_AMB_interim << AMB_reg_shift_led2) ^ (AMB_DAC_VALUE_AMB_interim << AMB_reg_shift_amb1);
						AMBInterimCode_2 = (AMB_DAC_VALUE_AMB_interim_2 << AMB_reg_shift_led2_2) ^ (AMB_DAC_VALUE_AMB_interim_2 << AMB_reg_shift_amb1_2);
						// Updates the AMB_DAC register value
						AFE44xx_Current_Register_Settings[3] = (AFE44xx_Current_Register_Settings[3] & ~(AMB2_mask)) | AMBInterimCode;
						AFE44xx_Current_Register_Settings[4] = (AFE44xx_Current_Register_Settings[4] & ~(AMB2_mask_2)) | AMBInterimCode_2;
					}
					else if (LED_Sel == 3)
					{
						//Interim value to be written to the register
						AMBInterimCode = (AMB_DAC_VALUE_AMB_interim << AMB_reg_shift_amb2) ^ (AMB_DAC_VALUE_AMB_interim << AMB_reg_shift_amb1);
						AMBInterimCode_2 = (AMB_DAC_VALUE_AMB_interim_2 << AMB_reg_shift_amb2_2) ^ (AMB_DAC_VALUE_AMB_interim_2 << AMB_reg_shift_amb1_2);

						// Updates the AMB_DAC register value
						AFE44xx_Current_Register_Settings[3] = (AFE44xx_Current_Register_Settings[3] & ~(AMB3_mask)) | AMBInterimCode;
						AFE44xx_Current_Register_Settings[4] = (AFE44xx_Current_Register_Settings[4] & ~(AMB3_mask_2)) | AMBInterimCode_2;
					}
					else  // Default is LED1
					{
						//Interim value to be written to the register
						AMBInterimCode = (AMB_DAC_VALUE_AMB_interim << AMB_reg_shift_led1) ^ (AMB_DAC_VALUE_AMB_interim << AMB_reg_shift_amb1);
						AMBInterimCode_2 = (AMB_DAC_VALUE_AMB_interim_2 << AMB_reg_shift_led1_2) ^ (AMB_DAC_VALUE_AMB_interim_2 << AMB_reg_shift_amb1_2);

						// Updates the AMB_DAC register value
						AFE44xx_Current_Register_Settings[3] = (AFE44xx_Current_Register_Settings[3] & ~(AMB1_mask)) | AMBInterimCode;
						AFE44xx_Current_Register_Settings[4] = (AFE44xx_Current_Register_Settings[4] & ~(AMB1_mask_2)) | AMBInterimCode_2;
					}
					system_param_s.AMB_DAC_LED = system_param_s.AMB_DAC_AMB;
					system_param_s.AMB_DAC_SIGN_LED = system_param_s.AMB_DAC_SIGN_AMB;

					//AFE4410_Reg_Write(AFE_CONTROL0, 0x00000000); // write mode
					AFE4410_Reg_Write(AFE_LEDCNTRL, AFE44xx_Current_Register_Settings[0]);        //0x20
					AFE4410_Reg_Write(AFE_DAC_SETTING_REG, AFE44xx_Current_Register_Settings[3]);
					AFE4410_Reg_Write(AFE_DAC_SETTING_REG_2, AFE44xx_Current_Register_Settings[4]);
					//AFE4410_Reg_Write(AFE_CONTROL0, 0x00000001); // read mode
					//Gaincalibration_state = sWaitTime3;		// Go to sWaitTime3
					Gaincalibration_state = sWaitTime1;		// Go to sWaitTime1
				}
				else
					Gaincalibration_state = sCalibrate;	// Go to sCalibrate
			}
			else
			{
				Gaincalibration_state = sWaitforData3;// if Meas_DC is within the limits - Come to the same state in the next execution of this function
				PeriodicCalibration_state = sFreeze_periodic;// Go to sFreeze_periodic mode
			}
		}
		break;

	case (sWaitTime3):
		/* Waits for 2 samples - i.e. more than 20ms - for the analog change to get settled */
		waitTimeCount++;
		if (waitTimeCount > TIMEWAIT)
		{
			waitTimeCount = 0;
			Meas_DC = 0;
			//Meas_DC_LED2 = 0;
			Gaincalibration_state = sWaitforData4;		// Go to sWaitforData4
		}
		break;

	case (sWaitforData4):
		/* Calculate the average of LEDVALUE for BLOCK_SIZE number of samples */
		blockSizeCount++;
		Meas_DC += LEDVALUE;
		//Meas_DC_LED2 += LED2VALUE;
		if (blockSizeCount == BLOCK_SIZE)
		{
			blockSizeCount = 0;
			Meas_DC = (Meas_DC >> BLOCK_SIZE_BITS);
			//Meas_DC_LED2 = (Meas_DC_LED2 >> BLOCK_SIZE_BITS);
			Gaincalibration_state = sCalibrate;			// Go to sCalibrate
		}
		break;

	}
}
/*********************************************************************
* @fn      		PeriodicCalibration
*
* @brief			Take care of calling the individual calibration routines in periodic calibration mode
*
* @param   		LEDVALUE,AMBVALUE
*
* @return  		void
*
*********************************************************************/
void PeriodicCalibration(long LEDVALUE, long AMBVALUE, long LED2VALUE)
{
	//static short int Tfreeze = 0;
	switch (PeriodicCalibration_state)
	{
		//		case (sFreeze_periodic):
		//		//Waits for TFREEZE seconds
		//		if (g_OneSecondFlag == 1)
		//		{
		//			g_OneSecondFlag = 0;
		//			Tfreeze++;
		//			if (Tfreeze == TFREEZE)
		//			{
		//				PeriodicCalibration_state = sAmbient_periodic;
		//				Tfreeze = 0;
		//			}
		//		}
		//		break;

	case (sAmbient_periodic):
		//calls the AmbientCancellation function
		AmbientCancellation(LEDVALUE, AMBVALUE);
		break;

	case (sGain_periodic):
		//calls the GainCalibration function
		GainCalibration(LEDVALUE - AMBVALUE, LEDVALUE);
		break;
	}
}
/*********************************************************************
* @fn      	CalibrateAFE4410
*
* @brief	wrapper around the individual calibration routines - calls each function based on the calibration mode
*
* @param   	LEDVALUE,AMBVALUE
*
* @return  	void
*
*********************************************************************/
void CalibrateAFE4410(long LEDVALUE, long AMBVALUE, long LED2VALUE)
{
	switch (calibration_mode)
	{
	case (sInitialize):
		//calls the initCalibrationRoutine
		initCalibrationRoutine();
		break;

	case (sAmbientDAC):
		//calls the AmbientCancellation function
		AmbientCancellation(LEDVALUE, AMBVALUE);
		break;

	case (sGain):
		//calls the GainCalibration function
		GainCalibration(LEDVALUE - AMBVALUE, LEDVALUE);
		break;

	case (sPeriodic):
		//calls the PeriodicCalibration function
		PeriodicCalibration(LEDVALUE, AMBVALUE, LED2VALUE);
		break;

	case (sFinish):
		//ends the calibration
		Calibration = 0;
		break;

	default:
		break;
	}
}
/*********************************************************************
* @fn      		OFFSET_DAC_Code_Est
*
* @brief		Estimate step size at the ADC output for an increase of
*                      0.5uA pleth current with Rf = 50K ==> 0.5uA * 100K in codes
*
* @param   		void
*
* @return  		void
*
*********************************************************************/
unsigned char OFFSET_DAC_Code_Est(long AMBVALUE)
{
	static short int waitTimeCount = 0;
	static short int blockSizeCount = 0;
	static long Meas_DC = 0;

	static int AMB_DAC_VALUE_AMB = 0;
	static long AMB_DAC_VALUE_AMB_interim = 0, AMB_DAC_VALUE_AMB_interim_2 = 0;
	static int AMB_DAC_VALUE_SIGN_AMB = 0;
	static long AMB_mask = 0x007C00;   // Amb masked
	static long AMB_mask_2 = 0x000030;   // Amb masked

	switch (OffsetDAC_code_Est_state)
	{
	case (sOffsetDACInit):
		AMB_DAC_VALUE_AMB = 0;
		AMB_DAC_VALUE_SIGN_AMB = 1;


		// AFE4410 change
		// Changed the mapping of offset DAC code to the register bits
		AMB_DAC_VALUE_AMB_interim = ((AMB_DAC_VALUE_AMB & 0x1E) >> 1) | (AMB_DAC_VALUE_SIGN_AMB << 4); //Calculates the register code for the corresponding Ambient cancellation required in AMB phase
		AMB_DAC_VALUE_AMB_interim_2 = ((AMB_DAC_VALUE_AMB & 0x20) >> 4) | (AMB_DAC_VALUE_SIGN_AMB & 0x1);


		AFE44xx_Current_Register_Settings[3] = (AFE44xx_Current_Register_Settings[3] & ~AMB_mask) | (AMB_DAC_VALUE_AMB_interim << 10);
		AFE44xx_Current_Register_Settings[4] = (AFE44xx_Current_Register_Settings[4] & ~AMB_mask_2) | (AMB_DAC_VALUE_AMB_interim_2 << 4);

		AFE4410_Reg_Write(AFE_CONTROL0, 0x00000000);            // write mode
		AFE4410_Reg_Write(AFE_DAC_SETTING_REG, AFE44xx_Current_Register_Settings[3]); //
		AFE4410_Reg_Write(AFE_DAC_SETTING_REG_2, AFE44xx_Current_Register_Settings[4]); //
		//AFE4410_Reg_Write(AFE_EXT_CLK_DIV_REG, 0x400);          //CONTROL3 register - Disconnect PD inputs
		AFE4410_Reg_Write(AFE_TIAAMBGAIN, 0x03);                // TIA gain = 50K
		AFE4410_Reg_Write(AFE_CONTROL0, 0x00000001);            // read mode
		OffsetDAC_code_Est_state = sOffsetDACWaitTime;
		break;

	case (sOffsetDACWaitTime):
		/* Waits for 2 samples - i.e. more than 20ms - for the analog change to get settled */
		waitTimeCount++;
		if (waitTimeCount > TIMEWAIT)
		{
			waitTimeCount = 0;
			Meas_DC = 0;
			blockSizeCount = 0;
			OffsetDAC_code_Est_state = sOffsetDACWaitforData;// Go to sOffsetDACWaitforData
		}
		break;

	case (sOffsetDACWaitforData):
		/* Calculate the average of LEDVALUE for BLOCK_SIZE number of samples */
		blockSizeCount++;
		Meas_DC += AMBVALUE;
		if (blockSizeCount == 8)
		{
			blockSizeCount = 0;
			Meas_DC = (Meas_DC >> 3);
			OffsetDAC_code_Est_state = sOffsetDACCodeEst;                 //sOffsetDACCodeEst
		}
		break;

	case (sOffsetDACCodeEst):
		Meas_DC_OFFSET_DAC_code[AMB_DAC_VALUE_AMB] = Meas_DC;
		if (AMB_DAC_VALUE_AMB != 0)
		{
			Meas_DC_OFFSET_DAC_code_step[AMB_DAC_VALUE_AMB] = Meas_DC_OFFSET_DAC_code[AMB_DAC_VALUE_AMB] - Meas_DC_OFFSET_DAC_code[AMB_DAC_VALUE_AMB - 1];
			ADC_CODE_AMB_DAC_STEP = Meas_DC_OFFSET_DAC_code[AMB_DAC_VALUE_AMB] - Meas_DC_OFFSET_DAC_code[AMB_DAC_VALUE_AMB - 1];
			ADC_CODE_AMB_DAC_MIN = ADC_CODE_AMB_DAC_STEP >> 1;
		}
		AMB_DAC_VALUE_AMB++;
		if (AMB_DAC_VALUE_AMB == 16) //2
		{
			AMB_DAC_VALUE_AMB = 0;
			OffsetDAC_code_Est_state = sOffsetDACOver;                 	// sOver
			AFE4410_Reg_Write(AFE_CONTROL0, 0x00000000);            	// write mode
			AFE4410_Reg_Write(AFE_DAC_SETTING_REG, 0); //
			AFE4410_Reg_Write(AFE_EXT_CLK_DIV_REG, 0);          		// CONTROL3 register
			AFE4410_Reg_Write(AFE_TIAAMBGAIN, 0);                		// TIA gain = 50K
			AFE4410_Reg_Write(AFE_CONTROL0, 0x00000001);            	// read mode
			return 1;
		}
		else
		{

			// AFE4410 change
			// Changed the mapping of offset DAC code to the register bits

			AMB_DAC_VALUE_AMB_interim = ((AMB_DAC_VALUE_AMB & 0x1E) >> 1) | (AMB_DAC_VALUE_SIGN_AMB << 4); //Calculates the register code for the corresponding Ambient cancellation required in AMB phase
			AMB_DAC_VALUE_AMB_interim_2 = ((AMB_DAC_VALUE_AMB & 0x20) >> 4) | (AMB_DAC_VALUE_SIGN_AMB & 0x1);


			AFE44xx_Current_Register_Settings[3] = (AFE44xx_Current_Register_Settings[3] & ~AMB_mask) | (AMB_DAC_VALUE_AMB_interim << 10);
			AFE44xx_Current_Register_Settings[4] = (AFE44xx_Current_Register_Settings[4] & ~AMB_mask_2) | (AMB_DAC_VALUE_AMB_interim_2 << 4);



			AFE4410_Reg_Write(AFE_CONTROL0, 0x00000000);            	// write mode
			AFE4410_Reg_Write(AFE_DAC_SETTING_REG, AFE44xx_Current_Register_Settings[3]); //
			AFE4410_Reg_Write(AFE_DAC_SETTING_REG_2, AFE44xx_Current_Register_Settings[4]); //
			AFE4410_Reg_Write(AFE_CONTROL0, 0x00000001);            	// read mode
			OffsetDAC_code_Est_state = sOffsetDACWaitTime;            	// sWaitTime1
		}
		break;

	case (sOffsetDACOver):
		return 1;
	}
	return 0;
}



