/***************************Copyright(c)***********************************
**                       Veepoo Tech
**                  COPYRIGHT (C) 2016-2019
**-----------File Info-----------------------------------------------------
** File        name:     afe4900_calibration.h
** Last modified Date:   2018-10-19
** Last SYS_VERSION:     0.0.0
** Descriptions:            
**-------------------------------------------------------------------------
** Created  by:          JSH
** Created  date:        2018-10-19
** SYS_VERSION:          0.0.0
** Descriptions:         
**-------------------------------------------------------------------------
** Modified by:				
** Modified date:  			          
** SYS_VERSION:                 
** Descriptions:                        
***************************************************************************/
#include "Blood_OxygenDetection.h"
#include "afe4900_calibration.h"
#include "afe4900_init.h"

#define setbit(x,y)		    (x |= (1 << y))  
#define clrbit(x,y)		    (x &= ~(1 << y)) 

// ================================================================================================
// ================================================================================================
// ================================================================================================

void Elec_Write(uint8_t i_led1, uint8_t i_led2, uint8_t i_led3);

void Tia_GainChange(
	uint8_t rf1_mid,					// led1 rf
	uint8_t rf2_mid,					// led2 rf 
	uint8_t rf3_mid, 					// led3 rf
	uint8_t rf4_mid						// amb1 rf
);

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
);

void AFE4410_RfRead(uint32_t * rf_gre, uint32_t * rf_red, uint32_t * rf_ird);

void AFE4410_ElecRead(uint32_t * i_gre, uint32_t * i_red, uint32_t * i_ird);

void AFE4410_DacRead(
	int32_t * adr_dac_gre,			// 绿光dac值
	int32_t * adr_dac_red,			// 红光dac值
	int32_t * adr_dac_ird,			// 红外dac值

	uint8_t * adr_dac_posi_gre,		// 绿光dac极性
	uint8_t * adr_dac_posi_red,		// 红光dac极性
	uint8_t * adr_dac_posi_ird		// 红外光dac极性
);

// ================================================================================================
// ================================================================================================
// ================================================================================================

void AFE4410_Reg_Write(unsigned char reg_address, unsigned int data)
{
	Afe4900_RegWrite(reg_address, data);
}

signed int AFE4410_Reg_Read(unsigned char reg_address)
{
	return Afe4900_RegRead(reg_address);
}

// ================================================================================================
// ================================================================================================
// ================================================================================================

// 现在该函数仅用于血氧算法校准写参数
void Afe4404Param_ToAfe4410(bod_result_info_t * afe4410param_s)
{
	// 添加局部变量存储结构体校准参数数据
	uint8_t dac_grn = 0;
	uint8_t dac_red = 0;
	uint8_t dac_ird = 0;

	//21H
	uint32_t afe4410_calbuletion_gain_red = 0;
	uint32_t afe4410_calbuletion_gain_ird = 0;
	uint32_t afe4410_calbuletion_gain_gre = 0;
	//22H
	uint32_t afe4410_calbuletion_current_red = 0;
	//22H
	uint32_t afe4410_calbuletion_current_gre = 0;
	//22H
	uint32_t afe4410_calbuletion_current_ird = 0;

    dac_grn = afe4410param_s->dac_grn;
    dac_red = afe4410param_s->dac_red;
    dac_ird = afe4410param_s->dac_ird;

    afe4410_calbuletion_gain_red = afe4410param_s->afe4410_calbuletion_gain_red;
    afe4410_calbuletion_gain_ird = afe4410param_s->afe4410_calbuletion_gain_ird;
    afe4410_calbuletion_gain_gre = afe4410param_s->afe4410_calbuletion_gain_gre;

    afe4410_calbuletion_current_red = afe4410param_s->afe4410_calbuletion_current_red;
    afe4410_calbuletion_current_gre = afe4410param_s->afe4410_calbuletion_current_gre;
    afe4410_calbuletion_current_ird = afe4410param_s->afe4410_calbuletion_current_ird;
    
// 将局部变量的afe4410参数放到结构体中
    afe4410param_s->dac_grn = dac_grn;
    afe4410param_s->dac_red = dac_red;
    afe4410param_s->dac_ird = dac_ird;

    afe4410param_s->afe4410_calbuletion_gain_red = afe4410_calbuletion_gain_red;
    afe4410param_s->afe4410_calbuletion_gain_ird = afe4410_calbuletion_gain_ird;
    afe4410param_s->afe4410_calbuletion_gain_gre = afe4410_calbuletion_gain_gre;

    afe4410param_s->afe4410_calbuletion_current_red = afe4410_calbuletion_current_red;
    afe4410param_s->afe4410_calbuletion_current_gre = afe4410_calbuletion_current_gre;
    afe4410param_s->afe4410_calbuletion_current_ird = afe4410_calbuletion_current_ird;

    
    Elec_ModeWrite(2,(uint8_t)(afe4410param_s->afe4410_calbuletion_current_red));
    Elec_ModeWrite(3,(uint8_t)(afe4410param_s->afe4410_calbuletion_current_ird));

    Tia_GainModeWrite(2,(uint8_t)(afe4410param_s->afe4410_calbuletion_gain_red));
    Tia_GainModeWrite(3,(uint8_t)(afe4410param_s->afe4410_calbuletion_gain_ird));
    Tia_GainModeWrite(4,(uint8_t)(1));
		
#ifdef AMB_DAC_MODE
    // 环境光减影模式写DAC   
    Dac_ModeWrite(2,AMB_DAC_MODE, afe4410param_s->dac_red, 1);
    Dac_ModeWrite(3,AMB_DAC_MODE, afe4410param_s->dac_ird, 1);
    Dac_ModeWrite(4,AMB_DAC_MODE, 0, 1);
		
#else
    // 非环境光减影模式写DAC
    uint8_t set_dac_ifs = 0;
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

    Dac_ModeWrite(2,need_set_dac_ifs, afe4410param_s->dac_red, 1);
    Dac_ModeWrite(3,need_set_dac_ifs, afe4410param_s->dac_ird, 1);
    Dac_ModeWrite(4,need_set_dac_ifs, 0, 1);
		
#endif	
}

// 单路光写电流函数  
// led_mode : 1 led1  2 led2  3 led3  4 led4
void Elec_ModeWrite(uint8_t led_mode, uint8_t value)
{
	uint8_t 
		msb = 0,
		lsb = 0,
		msb_pos = 0,
		lsb_pos = 0;
	uint32_t reg_22_value = 0;

	msb = (value & 0xFC) >> 2;
	lsb = (value & 0x03) >> 0;

	if(led_mode == 1)
	{
		msb_pos = 0;
		lsb_pos = 18;
	}
	else if(led_mode == 2)
	{
		msb_pos = 6;
		lsb_pos = 20;
	}
	else if(led_mode == 3)
	{
		msb_pos = 12;
		lsb_pos = 22;
	}

	AFE4410_Reg_Write(0x00, 0x00000061);            	// read mode

	reg_22_value = AFE4410_Reg_Read(0x22);

	for (uint8_t i = msb_pos; i < msb_pos + 6; i++)
	{
		clrbit(reg_22_value, i);
	}
	for (uint8_t i = lsb_pos; i < lsb_pos + 2; i++)
	{
		clrbit(reg_22_value, i);
	}

	reg_22_value |= (msb & 0x3F) << msb_pos;
	reg_22_value |= (lsb & 0x03) << lsb_pos;

	AFE4410_Reg_Write(0x00, 0x00000062);            	// write mode

	AFE4410_Reg_Write(0x22, (reg_22_value & 0x00FFFFFF));

	AFE4410_Reg_Write(0x00, 0x00000061);            	// read mode
}


// 单路信号写增益
void Tia_GainModeWrite(uint8_t led_mode, uint8_t value)
{
	
	uint8_t
		cf,
		rf_value 		= 0,
		tia_gain_msb 	= 0,
		tia_gain_lsb 	= 0,
		
		need_write_reg_pos = 0,
		write_st_pos = 0,
		write_ed_pos = 0;

	float cf_value 		= 0;

	uint8_t  rf_last_arr[9] = { 5,4,3,2,1,0,6,7,8 };
	uint8_t  cf_last_arr[8] = { 1,0,3,2,5,4,7,6 };

	uint16_t rf_value_sort[9] = { 10,25,50,100,250,500,1000,1500,2000 };
	float    cf_value_sort[8] = { 2.5f,5.0f,7.5f,10.0f,17.5f,20.0f,22.5f,25.0f };

	uint32_t reg_value = 0,need_write_value = 0;

	if(led_mode == 1)
	{
		need_write_reg_pos = 0x21;
		write_st_pos = 0;
		write_ed_pos = 7;
	}
	else if(led_mode == 2)
	{
		need_write_reg_pos = 0x20;
		write_st_pos = 0;
		write_ed_pos = 7;
	}
	else if(led_mode == 3)
	{
		need_write_reg_pos = 0x1F;
		write_st_pos = 0;
		write_ed_pos = 7;
	}
	else if(led_mode == 4)
	{
		need_write_reg_pos = 0x1F;
		write_st_pos = 8;
		write_ed_pos = 15;
	}

	cf_value = 4000.0f / (float)(rf_value_sort[value - 1]);

	for (uint8_t i = 0; i < 8; i++)
	{
		if (cf_value >= cf_value_sort[i])
		{
			cf_value = cf_value_sort[i];
			cf = i + 1;
			break;
		}
	}

	rf_value = rf_last_arr[value - 1];
	cf = cf_last_arr[cf - 1];

	tia_gain_msb = (rf_value & 0x08) >> 3;
	tia_gain_lsb = (rf_value & 0x07) >> 0;

	AFE4410_Reg_Write(0x00, 0x00000061);            	// read mode

	reg_value = AFE4410_Reg_Read(need_write_reg_pos);

	for (uint8_t i = write_st_pos; i < write_ed_pos; i++)
	{
		clrbit(reg_value, i);
	}

	need_write_value |= (tia_gain_lsb & 0x07) << 0;
	need_write_value |= (cf & 0x07) << 3;
	need_write_value |= (tia_gain_msb & 0x01) << 6;

	need_write_value = need_write_value << write_st_pos;

	reg_value |= need_write_value;

	AFE4410_Reg_Write(0x00, 0x00000062);            	// write mode

	AFE4410_Reg_Write(need_write_reg_pos, (reg_value & 0x00FFFFFF));

	AFE4410_Reg_Write(0x00, 0x00000061);            	// read mode

}


// 单路信号写DAC
void Dac_ModeWrite(
	uint8_t led_mode,
	uint8_t awdpr_ifs_dac,
	uint8_t awdpr_dac,
	uint8_t awdpr_dac_posi
)
{
	uint8_t
		msb_st = 0,
		msb_ed = 0,

		mid_st = 0,
		mid_ed = 0,

		lsb_st = 0,
		lsb_ed = 0,

		ext_st = 0,
		ext_ed = 0,

		posi_st = 0,
		posi_ed = 0,
		
		I_OFFDAC_MSB = 0,
		I_OFFDAC_MID = 0,
		I_OFFDAC_LSB = 0,
		I_OFFDAC_LSB_EXT = 0;

	uint32_t
		reg_21_value = 0,
		reg_3A_value = 0,
		reg_3E_value = 0;

	if(led_mode == 1)
	{
		msb_st = 3;
		msb_ed = 4;

		mid_st = 5;
		mid_ed = 9;

		lsb_st = 2;
		lsb_ed = 3;

		ext_st = 9;
		ext_ed = 10;

		posi_st = 9;
		posi_ed = 10;
	}
	else if(led_mode == 2)
	{
		msb_st = 7;
		msb_ed = 8;

		mid_st = 15;
		mid_ed = 19;

		lsb_st = 6;
		lsb_ed = 7;

		ext_st = 11;
		ext_ed = 12;

		posi_st = 19;
		posi_ed = 20;
	}
	else if(led_mode == 3)
	{
		msb_st = 1;
		msb_ed = 2;

		mid_st = 0;
		mid_ed = 4;

		lsb_st = 0;
		lsb_ed = 1;

		ext_st = 8;
		ext_ed = 9;

		posi_st = 4;
		posi_ed = 5;
	}
	else if(led_mode == 4)
	{
		msb_st = 5;
		msb_ed = 6;

		mid_st = 10;
		mid_ed = 14;

		lsb_st = 4;
		lsb_ed = 5;

		ext_st = 9;
		ext_ed = 10;

		posi_st = 14;
		posi_ed = 15;
	}



	I_OFFDAC_MSB = (awdpr_dac & 0X40) >> 6;
	I_OFFDAC_MID = (awdpr_dac & 0X3C) >> 2;
	I_OFFDAC_LSB = (awdpr_dac & 0X02) >> 1;
	I_OFFDAC_LSB_EXT = (awdpr_dac & 0X01) >> 0;

	AFE4410_Reg_Write(0x00, 0x00000061);            	// read mode
	reg_21_value = AFE4410_Reg_Read(0x21);
	reg_3A_value = AFE4410_Reg_Read(0x3A);
	reg_3E_value = AFE4410_Reg_Read(0x3E);

	for (uint8_t i = 12; i < 15; i++)
	{
		clrbit(reg_21_value, i);
	}
	reg_21_value |= ((awdpr_ifs_dac & 0x07) << 12);

	for (uint8_t i = msb_st; i < msb_ed; i++)
	{
		clrbit(reg_3E_value, i);
	}
	for (uint8_t i = mid_st; i < mid_ed; i++)
	{
		clrbit(reg_3A_value, i);
	}
	for (uint8_t i = lsb_st; i < lsb_ed; i++)
	{
		clrbit(reg_3E_value, i);
	}
	for (uint8_t i = ext_st; i < ext_ed; i++)
	{
		clrbit(reg_3E_value, i);
	}
	for (uint8_t i = posi_st; i < posi_ed; i++)
	{
		clrbit(reg_3A_value, i);
	}

	reg_3A_value |= ((awdpr_dac_posi & 0x01) << posi_st);
	reg_3A_value |= ((I_OFFDAC_MID & 0x0F) << mid_st);

	reg_3E_value |= ((I_OFFDAC_LSB_EXT & 0x01) << ext_st);
	reg_3E_value |= ((I_OFFDAC_LSB & 0x01) << lsb_st);
	reg_3E_value |= ((I_OFFDAC_MSB & 0x01) << msb_st);

	AFE4410_Reg_Write(0x00, 0x00000062);            	// write mode

	AFE4410_Reg_Write(0x21, (reg_21_value & 0x00FFFFFF));
	AFE4410_Reg_Write(0x3A, (reg_3A_value & 0x00FFFFFF));
	AFE4410_Reg_Write(0x3E, (reg_3E_value & 0x00FFFFFF));

	AFE4410_Reg_Write(0x00, 0x00000061);            	// read mode
}


// 三路信号同时写电流
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

	AFE4410_Reg_Write(0x00, 0x00000062);            	// write mode

	AFE4410_Reg_Write(0x22, (reg_22_value & 0x00FFFFFF));

	AFE4410_Reg_Write(0x00, 0x00000061);            	// read mode
}


// 4路信号同时写增益
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
		//tia_gain_sep_msb = 0,
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

	cf1_value = 4000.0f / (float)(rf_value_sort[rf1_mid - 1]);
	cf2_value = 4000.0f / (float)(rf_value_sort[rf2_mid - 1]);
	cf3_value = 4000.0f / (float)(rf_value_sort[rf3_mid - 1]);
	cf4_value = 4000.0f / (float)(rf_value_sort[rf4_mid - 1]);
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
	//tia_gain_sep_msb = (rf2 & 0x08) >> 3;
	tia_gain_sep2_msb = (rf3 & 0x08) >> 3;
	tia_gain_sep3_msb = (rf4 & 0x08) >> 3;

	tia_gain_lsb = (rf1 & 0x07) >> 0;
	tia_gain_sep_lsb = (rf2 & 0x07) >> 0;
	tia_gain_sep2_lsb = (rf3 & 0x07) >> 0;
	tia_gain_sep3_lsb = (rf4 & 0x07) >> 0;

	AFE4410_Reg_Write(0x00, 0x00000061);            	// read mode
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

	AFE4410_Reg_Write(0x00, 0x00000062);            	// write mode

	AFE4410_Reg_Write(0x1F, (reg_1f_value & 0x00FFFFFF));
	AFE4410_Reg_Write(0x20, (reg_20_value & 0x00FFFFFF));
	AFE4410_Reg_Write(0x21, (reg_21_value & 0x00FFFFFF));

	AFE4410_Reg_Write(0x00, 0x00000061);            	// read mode
}


// 4路信号同时写DAC
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

	AFE4410_Reg_Write(0x00, 0x00000061);            	// read mode
	reg_21_value = AFE4410_Reg_Read(0x21);
	reg_3A_value = AFE4410_Reg_Read(0x3A);
	reg_3E_value = AFE4410_Reg_Read(0x3E);

	for (uint8_t i = 12; i < 15; i++)
	{
		clrbit(reg_21_value, i);
	}
	reg_21_value |= ((awdpr_ifs_dac & 0x07) << 12);

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

	AFE4410_Reg_Write(0x00, 0x00000062);            	// write mode

	AFE4410_Reg_Write(0x21, (reg_21_value & 0x00FFFFFF));
	AFE4410_Reg_Write(0x3A, (reg_3A_value & 0x00FFFFFF));
	AFE4410_Reg_Write(0x3E, (reg_3E_value & 0x00FFFFFF));

	AFE4410_Reg_Write(0x00, 0x00000061);            	// read mode
}


// 电流读取及解析函数
void AFE4410_ElecRead(uint32_t * i_gre, uint32_t * i_red, uint32_t * i_ird)
{
	uint32_t reg_22_value = 0;

	reg_22_value = AFE4410_Reg_Read(0x22);

	*i_gre = (((reg_22_value & 0x0000003F) >> 0) << 2) | (((reg_22_value & 0x000C0000) >> 18) << 0);
	*i_red = (((reg_22_value & 0x0000FC0) >> 6) << 2) | (((reg_22_value & 0x00300000) >> 20) << 0);
	*i_ird = (((reg_22_value & 0x0003F000) >> 12) << 2) | (((reg_22_value & 0x00C00000) >> 22) << 0);
}


// 增益读取及解析函数
void AFE4410_RfRead(uint32_t * rf_gre, uint32_t * rf_red, uint32_t * rf_ird)
{
	uint32_t reg_1F_value = 0;
	uint32_t reg_20_value = 0;
	uint32_t reg_21_value = 0;

	reg_1F_value = AFE4410_Reg_Read(0x1F);
	reg_20_value = AFE4410_Reg_Read(0x20);
	reg_21_value = AFE4410_Reg_Read(0x21);

	*rf_gre = (((reg_21_value & 0x00000040) >> 6) << 3) | (((reg_21_value & 0x00000007) >> 0) << 0);
	*rf_red = (((reg_20_value & 0x00000040) >> 6) << 3) | (((reg_20_value & 0x00000007) >> 0) << 0);
	*rf_ird = (((reg_1F_value & 0x00000040) >> 6) << 3) | (((reg_1F_value & 0x00000007) >> 0) << 0);
}


// dac读取及解析函数
void AFE4410_DacRead(
	int32_t * adr_dac_gre,			// 绿光dac值
	int32_t * adr_dac_red,			// 红光dac值
	int32_t * adr_dac_ird,			// 红外dac值

	uint8_t * adr_dac_posi_gre,		// 绿光dac极性
	uint8_t * adr_dac_posi_red,		// 红光dac极性
	uint8_t * adr_dac_posi_ird		// 红外光dac极性
)
{
	uint32_t reg_3A_value = 0;
	uint32_t reg_3E_value = 0;

	reg_3A_value = AFE4410_Reg_Read(0x3A);
	reg_3E_value = AFE4410_Reg_Read(0x3E);

	*adr_dac_posi_gre = (reg_3A_value & 0x00000200) >> 9;
	*adr_dac_posi_red = (reg_3A_value & 0x00080000) >> 19;
	*adr_dac_posi_ird = (reg_3A_value & 0x00000010) >> 4;

	*adr_dac_gre = 
		(((reg_3E_value & 0x00000008) >> 3) << 6) 
		| (((reg_3A_value & 0x000001E0) >> 5) << 2) 
		| (((reg_3E_value & 0x00000004) >> 2) << 1)
		| (((reg_3E_value & 0x00000200) >> 9) << 0);

	*adr_dac_red =
		(((reg_3E_value & 0x00000080) >> 7) << 6)
		| (((reg_3A_value & 0x00078000) >> 15) << 2)
		| (((reg_3E_value & 0x00000040) >> 6) << 1)
		| (((reg_3E_value & 0x00000800) >> 11) << 0);

	*adr_dac_ird =
		(((reg_3E_value & 0x00000002) >> 1) << 6)
		| (((reg_3A_value & 0x0000000F) >> 0) << 2)
		| (((reg_3E_value & 0x00000001) >> 0) << 1)
		| (((reg_3E_value & 0x00000100) >> 8) << 0);
}





