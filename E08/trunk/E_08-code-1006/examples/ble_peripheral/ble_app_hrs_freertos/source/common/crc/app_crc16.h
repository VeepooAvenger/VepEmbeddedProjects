/***************************Copyright(c)***********************************
**                       Veepoo Tech
**                  COPYRIGHT (C) 2016-2019
**-----------File Info-----------------------------------------------------
** File        name:     app_crc16.h
** Last modified Date:   2017-08-21
** Last SYS_VERSION:     0.0.0
** Descriptions:            
**-------------------------------------------------------------------------
** Created  by:          WJ
** Created  date:        2017-08-21
** SYS_VERSION:          0.0.0
** Descriptions:         
**-------------------------------------------------------------------------
** Modified by:				
** Modified date:  			          
** SYS_VERSION:                 
** Descriptions:                        
***************************************************************************/
#ifndef APP_CRC16_H
#define APP_CRC16_H

// crc校验
uint16_t App_Crc16(uint8_t *data, uint16_t len);

//crc校验，并与前一个crc校验值并入计算
uint16_t App_Crc16ByCrc16(uint16_t crc16, uint8_t *data, uint16_t len); 
#endif
