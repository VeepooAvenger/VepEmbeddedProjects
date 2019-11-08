/***************************Copyright(c)***********************************
**                       Veepoo Tech
**                  COPYRIGHT (C) 2016-2019
**-----------File Info-----------------------------------------------------
** File        name:     app_adc_send.c
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
#include <string.h>
#include <stdint.h>

#include "bracelet_protocol.h"

/*********************************************************************
* Macro Definition
*/
#define SAMPLE_NUM_256		256

/*********************************************************************
* Global Variables
*/

static int16_t adc_sample_buf_bak[SAMPLE_NUM_256] = {0x00,};    //adc发送缓存

static uint8_t   adc_buf[20] = {0x00,};                         //发送数据
static uint16_t  adc_index = 0;                                 //发送数组下标
static uint16_t  adc_buf_cnt = 0;                               //剩余个数
static uint16_t  adc_max_cnt = 0;                               //发送长度

/****************************************************************
** Function name:           Adc_SendInit
** Descriptions:            初始化ADC发送
** input parameters:        adc_num
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-06-02
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/
void Adc_SendInit(uint16_t adc_num)
{       
    adc_max_cnt = adc_num;
    adc_index = 0;
    adc_buf_cnt = 0;    
    memset(adc_sample_buf_bak, 0x00, SAMPLE_NUM_256*2);    
    memset(adc_buf, 0x00, 20);  
}

/****************************************************************
** Function name:           Adc_BufSet
** Descriptions:            填充发送缓存buf
** input parameters:        s_adc：数据指针；len：长度
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-06-02
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/
void Adc_SetSendBuf(int16_t *s_adc, uint16_t len)
{
    if(s_adc == NULL || len == 0 || len > SAMPLE_NUM_256)
    {
        return;
    }
    
    if(len > adc_max_cnt)
    {
        len = adc_max_cnt;
    }
        
    memcpy(adc_sample_buf_bak, s_adc, len * 2);    
}

/****************************************************************
** Function name:           Adc_BleSendDataBuf
** Descriptions:            发送ADC到APP
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-06-02
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/
uint8_t Adc_BleSendDataBuf(void)
{
    uint16_t i = 0;
    uint8_t ble_buf[20] = {0x00,};
    uint16_t ble_buf_cnt = 0;
    uint16_t ble_buf_indx = 0;
    
    if((adc_index + 10 > adc_max_cnt) && adc_index <= adc_max_cnt)
    {
        adc_buf_cnt = (adc_max_cnt - adc_index)*2; 
        for(i = 0; i < adc_buf_cnt/2; i++)
        {
            adc_buf[i * 2] = (adc_sample_buf_bak[adc_index + i]) & 0xFF ;
            adc_buf[(i * 2) + 1] = (adc_sample_buf_bak[adc_index + i] >> 8) & 0xFF ;            
        }        
        adc_index = 0;       
        return 0;
    }
    
    if(adc_buf_cnt > 0 && adc_index == 0)
    {
        memcpy(ble_buf, adc_buf, adc_buf_cnt);       
        ble_buf_cnt = adc_buf_cnt;
        adc_buf_cnt = 0;
    }
    
    ble_buf_indx = ble_buf_cnt;
    ble_buf_cnt = 20 - ble_buf_cnt;   
    for(i = 0; i < ble_buf_cnt/2; i++)
    {
        ble_buf[ble_buf_indx + i * 2] = (adc_sample_buf_bak[adc_index + i]) & 0xFF ;
        ble_buf[ble_buf_indx + (i * 2) + 1] = (adc_sample_buf_bak[adc_index + i] >> 8) & 0xFF ;      
    } 
    
    adc_index += i;
    adc_buf_cnt = 0;
    
    Ble_SendDataViaClk(ble_buf, 20);
    
    return 1;
}
