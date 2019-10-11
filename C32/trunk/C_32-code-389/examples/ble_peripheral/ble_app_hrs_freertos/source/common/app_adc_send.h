/***************************Copyright(c)***********************************
**                       Veepoo Tech
**                  COPYRIGHT (C) 2016-2019
**-----------File Info-----------------------------------------------------
** File        name:     app_adc_send.h
** Last modified Date:   2017-08-24
** Last SYS_VERSION:     0.0.0
** Descriptions:            
**-------------------------------------------------------------------------
** Created  by:          WJ
** Created  date:        2017-08-24
** SYS_VERSION:          0.0.0
** Descriptions:         
**-------------------------------------------------------------------------
** Modified by:				
** Modified date:  			          
** SYS_VERSION:                 
** Descriptions:                        
***************************************************************************/
#ifndef __APP_ADC_SEND_H
#define __APP_ADC_SEND_H

//初始化
void Adc_SendInit(uint16_t adc_num);

//设置发送内容
void Adc_SetSendBuf(int16_t *s_adc, uint16_t len);

//发送数据
uint8_t Adc_BleSendDataBuf(void);


#endif  //__APP_ADC_SEND_H



