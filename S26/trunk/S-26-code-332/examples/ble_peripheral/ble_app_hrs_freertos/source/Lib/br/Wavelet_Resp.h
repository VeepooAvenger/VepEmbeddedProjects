/********************************Copyright(c)***********************************
**                          Veepoo Tech
**                        COPYRIGHT (C) 2016-2019
**--------------File Info-------------------------------------------------------
** File name:               My_Wavelet.h
** Last modified Date:       2017-08-14
** Last SYS_VERSION:           01.01.02
** Descriptions:
**
**------------------------------------------------------------------------------
** Created by:              HYR
** Created date:            2017-05-23
** SYS_VERSION:
** Descriptions:           wavelet dwt and idwt test
**
**------------------------------------------------------------------------------
** Modified by:              HYR
** Modified date:            01010300.2017-08-14
1. Add the malloc check for data
**------------------------------------------------------------------------------
** Modified by:              HYR
** Modified date:            01010500.2017-08-18
Modified param setting function delete = for end_point
**------------------------------------------------------------------------------
*******************************************************************************/
#ifndef __MY_WAVELET_
#define __MY_WAVELET_
#include "stdint.h"


#define UPDATE_LEN        100

#define PROCESS_LEN_RESP  (750/2)  //process data length setting 

#define MW_MAX_DEC_LEVEL   10

#define WAVELET_SYM8 1 
#ifdef WAVELET_DB3
#define FILTER_LEN 6 
#endif

#ifdef WAVELET_DB10
#define FILTER_LEN 20 
#endif

#ifdef WAVELET_SYM8
#define FILTER_LEN 16 
#endif
//52 byte
typedef struct MyStruct
{
	uint8_t level_num;
	int32_t total_len;
	int32_t dec_level_len[MW_MAX_DEC_LEVEL + 2];
}wavelet_dec_len_t;
//22 byte
typedef struct
{
	uint8_t dec_level_num;
	uint8_t app_flag;

	uint8_t start_level1;
	uint8_t end_level1;
	uint8_t start_level2;
	uint8_t end_level2;

	int16_t start_point1;
	int16_t end_point1;
	int16_t start_point2;
	int16_t end_point2;

	float setting_rate1;
	float setting_rate2;

}wf_setting_t;

typedef struct
{
	int32_t length;
	float low_fil_dec[FILTER_LEN];
	float high_fil_dec[FILTER_LEN];
	float low_fil_rec[FILTER_LEN];
	float high_fil_rec[FILTER_LEN];
}resp_filter_t;
//20180808HJJ
//extern float *wmd_dec_data;   //Dec param 
//extern float *mwd_ca; //single level approximation coefficients
//extern float *mwd_cd; //single level detail coefficients
//
//extern float * mwr_detail_coef; //Rec param
//extern float * mwr_approximation_temp;

extern resp_filter_t filter_param_resp_s;
;

/*******************************************************************************
** Function name:           Set_FilterRespParam
** Descriptions:            Set the low and high pass filter for wavelet
** input parameters:        none
** output parameters:       none
** Returned value:          none
** Created by:              HYR
** Created Date:            2017-05-22
**------------------------------------------------------------------------------
*******************************************************************************/
void Set_FilterRespParam(void);

/*
* @brief 小波变换之分解
* @param sourceData 源数据
* @param dataLen 源数据长度
* @param db 过滤器类型
* @param cA 分解后的近似部分序列-低频部分
* @param cD 分解后的细节部分序列-高频部分
* @return
*         正常则返回分解后序列的数据长度，错误则返回-1
*/
int Wavelet_RespDwt(float *wd_source_data, int wd_data_len, resp_filter_t wd_filter_s, float *cA, float *cD);

/**
* @brief 小波变换之重构 //single level signal reconstruction
* @param cA 分解后的近似部分序列-低频部分
* @param cD 分解后的细节部分序列-高频部分
* @param cALength 输入数据长度
* @param db 过滤器类型
* @param recData 重构后输出的数据
*/
void  Wavelet_RespIdwt(float *wi_ca, float *wi_cd, int wi_ca_len, resp_filter_t wi_filter_s, int wi_rec_len, float *recData);

/*******************************************************************************
** Function name:           My_WaveletRespDec
** Descriptions:            wavelet decomposition
** input parameters:        mwd_signal_input mwd_dec_num myd_filter_param_s myd_dec_len_s
** output parameters:       mwd_dec_coef_info
** Returned value:          none
** Created by:              HYR
** Created Date:            2017-05-22
**------------------------------------------------------------------------------
*******************************************************************************/
uint8_t My_WaveletRespDec(float *mwd_signal_input, uint8_t mwd_dec_num, resp_filter_t myd_filter_param_s, wavelet_dec_len_t myd_dec_len_s, float *mwd_dec_coef_info);

/*******************************************************************************
** Function name:           My_WaveletRespRec
** Descriptions:            Reconstruction the signal by decomposition coeficient
** input parameters:        mwr_dec_coef_input  mwr_len_s mwr_filter_param_s
** output parameters:       mwr_recon_data_output
** Returned value:          none
** Created by:              HYR
** Created Date:            2017-05-31
**------------------------------------------------------------------------------
*******************************************************************************/
uint8_t My_WaveletRespRec(float *mwr_dec_coef_input, wavelet_dec_len_t mwr_len_s, resp_filter_t mwr_filter_param_s, float * mwr_recon_data_output);

/*******************************************************************************
** Function name:           Check_RespWaveletInfoLen
** Descriptions:            Check the space for wavelet decomposition; max decomposition level less than 10
** input parameters:        cwil_data_len cwil_dec_num cwil_filter_len
** output parameters:       weil_dec_len_info
** Returned value:          none
** Created by:              HYR
** Created Date:            2017-05-24
**------------------------------------------------------------------------------
*******************************************************************************/
int32_t Check_RespWaveletInfoLen(int32_t cwil_data_len, uint8_t cwil_dec_num, int32_t cwil_filter_len, wavelet_dec_len_t *weil_dec_len_info);

/*******************************************************************************
** Function name:           Get_DetailCoefResp
** Descriptions:            Get the setting level detail of wavelet coeficient
** input parameters:        gdc_wavelet_dec_coef gdc_dec_len_s gdc_setting_level gdc_dec_num
** output parameters:       gdc_detail_coef
** Returned value:          none
** Created by:              HYR
** Created Date:            2017-05-26
**------------------------------------------------------------------------------
*******************************************************************************/
void Get_DetailCoefResp(float* gdc_wavelet_dec_coef, wavelet_dec_len_t gdc_dec_len_s, uint8_t gdc_wanted_detail_level, uint8_t gdc_dec_num, float *gdc_detail_coef);

/*******************************************************************************
** Function name:           Setting_WaveletFilterParamResp
** Descriptions:            Setting the wavelet filter param make the filter coming truth
** input parameters:        swfp_dec_coef_input swfp_setting_s
** output parameters:       swfp_dec_coef_output
** Returned value:          none
** Created by:              HYR
** Created Date:            2017-05-27
**------------------------------------------------------------------------------
*******************************************************************************/
void Setting_WaveletFilterParamResp(const float *swfp_dec_coef_input, wavelet_dec_len_t swfp_dec_len_s, wf_setting_t swfp_setting_s, float *swfp_dec_coef_output);

#endif

