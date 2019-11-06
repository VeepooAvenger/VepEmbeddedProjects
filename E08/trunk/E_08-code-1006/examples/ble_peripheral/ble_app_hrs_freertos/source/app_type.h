/*****************************************************************
Copyright: 2012-2016, Veepoo Tech. Co., Ltd.
File name:         app_type.h
Description: 
Author:             WJ
Version: 
Date:             2016-09-14
History: 
*****************************************************************/

#ifndef USER_APP_TYPE_H
#define USER_APP_TYPE_H

#include <string.h>
#include "board.h"
#include "ble_nus.h"
#include "ble_clk.h"
#include "bracelet_protocol.h"
#include "bsp_spi.h"
#include "I2C.h"
#include "bsp_iic.h"
#include "nrf_delay.h"
#include "time_manage.h"
#include "app_ancs.h"
#include "SystemTime.h"
#include "pedometer.h"
#include "gesture_test.h"
#include "motor_event.h"
#include "fontlib_spi.h"

#define FALSE  0
#define TRUE   1

void get_ble_addr(uint8_t *addr);

uint16_t *M_Get_Conn_Handle(void);

#endif //USER_APP_TYPE_H

