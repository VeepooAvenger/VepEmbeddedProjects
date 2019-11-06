#ifndef BRACELET_PROTOCOL_H
#define BRACELET_PROTOCOL_H

#include "ble_nus.h"
#include "ble_clk.h"

void Protocol_Bat_Data_Handler(ble_nus_t * p_nus, uint8_t * p_data, uint16_t length);
void Protocol_Clk_Data_Handler(ble_clk_t * p_clk, uint8_t * p_data, uint16_t length);

uint32_t Ble_SendDataViaBat(uint8_t *data_addr, uint8_t data_size);

void Ble_SendBatMulData(uint8_t len,...);

#endif
