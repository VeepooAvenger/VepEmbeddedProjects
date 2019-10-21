/********************************Copyright(c)***********************************
**                           Veepoo Tech
**                         COPYRIGHT (C) 2014-2017
**--------------File Info------------------------------------------------------
** File name:               Hrv_Detection.h
** Created Date:            2017-06-28
** Last SYS_VERSION:        01.01.01.00
** Descriptions:
**
**-----------------------------------------------------------------------------
** Modified by:           HJJ
**  Date:                 2017-07-04
** SYS_VERSION:           01.01.01.01
** Descriptions:          USE GREEN LIGHT ,200Hz
**-----------------------------------------------------------------------------
** Modified by:           HJJ
**  Date:                 2017-07-13
** SYS_VERSION:           01.01.01.02
** Descriptions:          USE GREEN LIGHT ,200Hz ,4s
**-----------------------------------------------------------------------------
** Modified by:           HJJ
**  Date:                 2017-07-25
** SYS_VERSION:           01.01.02.00
** Descriptions:          change the low filter to smooth filter
**-----------------------------------------------------------------------------
** Modified by:           HJJ
**  Date:                 2017-08-08
** SYS_VERSION:           01.01.03.00
** Descriptions:          add the process of calculating the heart load
**-----------------------------------------------------------------------------
** Modified by:           HJJ
**  Date:                 20170816
** SYS_VERSION:           01.01.04.00
** Descriptions:          change the name of files
**-----------------------------------------------------------------------------
** Modified by:           HJJ
**  Date:                 2017-08-17
** SYS_VERSION:           01.01.05.00
** Descriptions:          remove hrv_flag and add some error code
**-----------------------------------------------------------------------------
** Modified by:           HJJ
**  Date:                 2017-08-21
** SYS_VERSION:           01.01.06.00
** Descriptions:          add process of  the datadirft and Segment processing heart load result
**-----------------------------------------------------------------------------
** Modified by:           HJJ
**  Date:                 2017-08-23
** SYS_VERSION:           01.01.07.00
** Descriptions:          change the type of data
**-----------------------------------------------------------------------------
** Modified by:           HJJ
**  Date:                 2017-08-23
** SYS_VERSION:           01.01.08.00
** Descriptions:          remove extra error code
**-----------------------------------------------------------------------------
** Modified by:           HJJ
**  Date:                 2017-08-24
** SYS_VERSION:           01.01.09.00
** Descriptions:          remove the datadirft process
**-----------------------------------------------------------------------------
** Modified by:           HJJ
**  Date:                 2017-08-28
** SYS_VERSION:           01.01.10.00
** Descriptions:          change the heart load's normal value
**-----------------------------------------------------------------------------
** Modified by:           HJJ
**  Date:                 2017-09-05
** SYS_VERSION:           01.02.00.00
** Descriptions:          change the sample rate 200Hz to 50Hz
**-----------------------------------------------------------------------------
** Modified by:           HJJ
**  Date:                 2017-09-11
** SYS_VERSION:           01.02.02.00
** Descriptions:          change way of search the up_rising point
**-----------------------------------------------------------------------------
** Modified by:           HJJ
**  Date:                 2017-09-12
** SYS_VERSION:           01.02.03.00
** Descriptions:          add the init function (6s,green light )
**-----------------------------------------------------------------------------
** Modified by:           HJJ
**  Date:                 2017-10-19
** SYS_VERSION:           01.02.03.01
** Descriptions:          add the processing of  HR's output
**-----------------------------------------------------------------------------
** Modified by:           HJJ
**  Date:                 2017-10-30
** SYS_VERSION:           01.02.04.00
** Descriptions:          add the pulse rate output
**-----------------------------------------------------------------------------
** Modified by:           HJJ
** Modified date:         2017-11-08
** SYS_VERSION:           01.02.05.00
** Descriptions:          add the limit of result
**-----------------------------------------------------------------------------
** Modified by:           HJJ
** Modified date:         2017-11-16
** SYS_VERSION:           01.02.05.01
** Descriptions:          add the waveform quality and calculate the RR value not
                          the heart value in order to raise the Accuracy of hrv
**-----------------------------------------------------------------------------
** Modified by:           HJJ
** Modified date:         2017-11-16
** SYS_VERSION:           01.02.05.02
** Descriptions:          change the struct of the process
**-----------------------------------------------------------------------------
** Modified by:           HJJ
** Modified date:         2017-11-18
** SYS_VERSION:           01.02.05.04
** Descriptions:          change the type of output's data
**-----------------------------------------------------------------------------
** Modified by:           HJJ
** Modified date:         2017-11-20
** SYS_VERSION:           01.02.06.00
** Descriptions:          change the version
**-----------------------------------------------------------------------------
** Modified by:           HJJ
** Modified date:         2017-11-21
** SYS_VERSION:           01.02.07.00
** Descriptions:          remove the init of heart_valid
**-----------------------------------------------------------------------------
** Modified by:           HJJ
** Modified date:         2018-01-05
** SYS_VERSION:           01.02.07.01
** Descriptions:          add the sport value of latest 1.5s
**-----------------------------------------------------------------------------
** Modified by:           HJJ
** Modified date:         2018-01-10
** SYS_VERSION:           01.02.08.00
** Descriptions:          change the version
**-----------------------------------------------------------------------------
*******************************************************************************/
#ifndef HRV_DETECTION_H
#define HRV_DETECTION_H

/*******************************************************************
********************************************************************/

#include "stdbool.h"
#include "stdint.h"

typedef struct
{
	int8_t  heart_flag;       //20171030 HJJ ������Чֵ�����־λ ��heart_flag = 1ʱ������ֵΪ5s�����ʾ�ֵ
	uint8_t heart_valid;     //20171030 HJJ ��Ч������ֵ
	int16_t hrv_value;       //hrv ֵ
	int16_t hr_value;        //HR ��ֵ
	int16_t heart_err_code;  //20171030 HJJ ���ʴ�����  heart_err_code = 230 ���㲻����Ч����;heart_err_code = 231;���ʸ�����������
	int16_t error_code;      //error_code = 211 ��ʾ���ݴ���6s ;error_code =  212;//20170704 ��ʾ�����쳣 �����ʹ��߻��߹���;
	                         // error_code = 214 �����������ã��Ҳ��������أ����ฺ���޷�����
	                         // error_code = 215 ;�������ֵ�쳣�����½��С��0
	int32_t heart_load;     // 20170808 HJJ heart load ���ฺ��
}hrv_info_t;

const hrv_info_t *Get_Hrv_Info(void);

/********************************************************************
** Function name:           Hrv_SituationCalculate
** Descriptions:
** input parameters:        adc_result
** output parameters:       none
** Returned value:          none
** Created by:              HJJ
** Created Date:            2017-06-28
**-------------------------------------------------------------------
** Modified by:             HJJ 
** Modified date:           2018-01-05
** DES:                     add the sport value input
**-------------------------------------------------------------------
********************************************************************/
void  Hrv_SituationCalculate(int32_t *tsj_hrv_time, int8_t tsj_hrv_num,uint8_t hsc_sport_value);
/********************************************************************
** Function name:           Hrv_Detection
** Descriptions:            calculate the hrv and heart load
** input parameters:        ADC_result, heart_get_mode , move_station
** output parameters:       result_of_pulse
** Returned value:          errc
** Created by:              HJJ
** Created Date:            2017-06-28
**-------------------------------------------------------------------
** Modified by:             HJJ
** Modified date:           2007-08-23
** Descriptions:            change the type of data
**-------------------------------------------------------------------
** Modified by:             HJJ
** Modified date:           2018-01-05
** Descriptions:            add the sport value input
**-------------------------------------------------------------------
********************************************************************/
void Hrv_Detection(const int32_t *input_hrv_data ,uint8_t hd_sport_value);

/********************************************************************
** Function name:           Get_HrvVersion
** Descriptions:            Get_HrvVersion
** input parameters:        none
** output parameters:       none
** Returned value:          Version
** Created by:              HJJ
** Created Date:            2017-06-28
**-------------------------------------------------------------------
********************************************************************/
uint32_t Get_HrvVersion(void);

/********************************************************************
** Function name:           Hrv_DectInit
** Descriptions:            init of hrv's parameter
** input parameters:        none
** output parameters:       none
** Returned value:          none
** Created by:              HJJ
** Created Date:            2017-06-28
**-------------------------------------------------------------------
** Modified by:
** Modified date:
** description  :
**-------------------------------------------------------------------
********************************************************************/
void Hrv_DectInit(void);
#endif 


