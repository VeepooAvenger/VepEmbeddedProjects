/***************************Copyright(c)***********************************
**                       Veepoo Tech
**                  COPYRIGHT (C) 2016-2019
**-----------File Info-----------------------------------------------------
** File        name:     bracelet_protocol.c
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
#ifndef BRACELET_PROTOCOL_H
#define BRACELET_PROTOCOL_H

#include "ble_nus.h"
#include "ble_clk.h"

//Bat����ͨ����������
void Protocol_BatDataHandler(ble_nus_t * p_nus, uint8_t * p_data, uint16_t length);

//Clk����ͨ����������
void Protocol_ClkDataHandler(ble_clk_t * p_clk, uint8_t * p_data, uint16_t length);

//Bat����ͨ����������
void Ble_SendDataViaBat(uint8_t *data_addr, uint8_t data_size);

//Clk����ͨ����������
void Ble_SendDataViaClk(uint8_t *data_addr, uint8_t data_size);

#endif
