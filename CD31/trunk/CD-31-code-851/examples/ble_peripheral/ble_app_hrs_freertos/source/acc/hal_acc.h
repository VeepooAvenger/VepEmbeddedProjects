/**************************************************************************************************
  Filename:       hal_acc.h
  Revised:        $Date: 2013-08-23 11:45:31 -0700 (Fri, 23 Aug 2013) $
  Revision:       $Revision: 35100 $

  Description:    This file contains the declaration to the HAL KXTI9 abstraction layer.


  Copyright 2012 - 2013  Texas Instruments Incorporated. All rights reserved.

  IMPORTANT: Your use of this Software is limited to those specific rights
  granted under the terms of a software license agreement between the user
  who downloaded the software, his/her employer (which must be your employer)
  and Texas Instruments Incorporated (the "License").  You may not use this
  Software unless you agree to abide by the terms of the License. The License
  limits your use, and you acknowledge, that the Software may not be modified,
  copied or distributed unless embedded on a Texas Instruments microcontroller
  or used solely and exclusively in conjunction with a Texas Instruments radio
  frequency transceiver, which is integrated into your product.  Other than for
  the foregoing purpose, you may not use, reproduce, copy, prepare derivative
  works of, modify, distribute, perform, display or sell this Software and/or
  its documentation for any purpose.

  YOU FURTHER ACKNOWLEDGE AND AGREE THAT THE SOFTWARE AND DOCUMENTATION ARE
  PROVIDED “AS IS” WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED,
  INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF MERCHANTABILITY, TITLE,
  NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT SHALL
  TEXAS INSTRUMENTS OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER CONTRACT,
  NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR OTHER
  LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
  INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE
  OR CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT
  OF SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
  (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.

  Should you have any questions regarding your right to use this Software,
  contact Texas Instruments Incorporated at www.TI.com.
**************************************************************************************************/
#ifndef HAL_ACC_H
#define HAL_ACC_H

/* ------------------------------------------------------------------------------------------------
 *                                          Includes
 * ------------------------------------------------------------------------------------------------
 */
#include <stdbool.h>
#include <stdint.h>
#include "board.h"

/* ------------------------------------------------------------------------------------------------
 *                                          Constants
 * ------------------------------------------------------------------------------------------------
 */
#define HAL_ACC_RANGE_8G       1
#define HAL_ACC_RANGE_4G       2
#define HAL_ACC_RANGE_2G       3
  
// Sensor I2C address
#define HAL_KXTI9_I2C_ADDRESS          0x0F

// KXTI9 register addresses
#define ACC_REG_ADDR_XOUT_HPF_L        0x00 // R
#define ACC_REG_ADDR_XOUT_HPF_H        0x01 // R
#define ACC_REG_ADDR_YOUT_HPF_L        0x02 // R
#define ACC_REG_ADDR_YOUT_HPF_H        0x03 // R
#define ACC_REG_ADDR_ZOUT_HPF_L        0x04 // R
#define ACC_REG_ADDR_ZOUT_HPF_H        0x05 // R
#define ACC_REG_ADDR_XOUT_L            0x06 // R
#define ACC_REG_ADDR_XOUT_H            0x07 // R
#define ACC_REG_ADDR_YOUT_L            0x08 // R
#define ACC_REG_ADDR_YOUT_H            0x09 // R
#define ACC_REG_ADDR_ZOUT_L            0x0A // R
#define ACC_REG_ADDR_ZOUT_H            0x0B // R
#define ACC_REG_ADDR_DCST_RESP         0x0C // R
#define ACC_REG_ADDR_WHO_AM_I          0x0F // R
#define ACC_REG_ADDR_TILT_POS_CUR      0x10 // R
#define ACC_REG_ADDR_TILT_POS_PRE      0x11 // R

#define ACC_REG_ADDR_INT_SRC_REG1      0x15 // R
#define ACC_REG_ADDR_INT_SRC_REG2      0x16 // R
#define ACC_REG_ADDR_STATUS_REG        0x18 // R
#define ACC_REG_ADDR_INT_REL           0x1A // R

#define ACC_REG_ADDR_CTRL_REG1         0x1B // R/W
#define ACC_REG_ADDR_CTRL_REG2         0x1D // R/W

#define ACC_REG_ADDR_INT_CTRL_REG1     0x1E // R/W
#define ACC_REG_ADDR_INT_CTRL_REG2     0x1F // R/W
#define ACC_REG_ADDR_INT_CTRL_REG3     0x20 // R/W
#define ACC_REG_ADDR_DATA_CTRL_REG     0x21 // R/W

#define ACC_REG_ADDR_TILT_TIMER        0x28 // R/W
#define ACC_REG_ADDR_WUF_TIMER         0x29 // R/W
#define ACC_REG_ADDR_TDT_TIMER         0x2B // R/W
#define ACC_REG_ADDR_TDT_H_THRESH      0x2C // R/W
#define ACC_REG_ADDR_TDT_L_THRESH      0x2D // R/W
#define ACC_REG_ADDR_TDT_TAP_TIMER     0x2E // R/W
#define ACC_REG_ADDR_TDT_TOTAL_TIMER   0x2F // R/W
#define ACC_REG_ADDR_TDT_LATENCY_TIMER 0x30 // R/W
#define ACC_REG_ADDR_TDT_WINDOW_TIMER  0x31 // R/W

#define ACC_REG_ADDR_BUF_CTRL1         0x32 // R/W
#define ACC_REG_ADDR_BUF_CTRL2         0x33 // R/W
#define ACC_REG_ADDR_BUF_STATUS_REG1   0x34 // R
#define ACC_REG_ADDR_BUF_STATUS_REG2   0x35 // R/W
#define ACC_REG_ADDR_BUF_CLEAR         0x36 // W

#define ACC_REG_ADDR_SELF_TEST         0x3A // R/W

#define ACC_REG_ADDR_WUF_THRESH        0x5A // R/W
#define ACC_REG_ADDR_TILT_ANGLE        0x5C // R/W
#define ACC_REG_ADDR_HYST_SET          0x5F // R/W
#define ACC_REG_ADDR_BUF_READ          0x7F // R/W

#define	ACC_REG_ADDR_THREHOLD				 	 0x6a

// Select register valies
#define REG_VAL_WHO_AM_I               0x09 // (data sheet says 0x04)

// CTRL1 BIT MASKS
#define ACC_REG_CTRL_PC                0x80 // Power control  '1' On    '0' Off
#define ACC_REG_CTRL_RES               0x40 // Resolution     '1' High  '0' Low
#define ACC_REG_CTRL_DRDYE             0x20 // Data Ready     '1' On    '0' Off
#define ACC_REG_CTRL_GSEL_HI           0x10 // Range     '00' +/-2g    '01' +/-4g
#define ACC_REG_CTRL_GSEL_LO           0x08 //           '10' +/-8g    '11' N/A
#define ACC_REG_CTRL_GSEL_TDTE         0x04 // Directional Tap '1' On   '0' Off
#define ACC_REG_CTRL_GSEL_WUFE         0x02 // Wake Up         '1' On   '0' Off
#define ACC_REG_CTRL_GSEL_TPE          0x01 // Tilt Position   '1' On   '0' Off

// Range +- 2G
#define ACC_REG_CTRL_ON_2G             ( ACC_REG_CTRL_PC )
#define ACC_REG_CTRL_OFF_2G            ( 0 )

// Range +- 4G
#define ACC_REG_CTRL_ON_4G             ( ACC_REG_CTRL_PC | ACC_REG_CTRL_GSEL_LO)
#define ACC_REG_CTRL_OFF_4G            ( ACC_REG_CTRL_GSEL_LO )

// Range +- 8G
#define ACC_REG_CTRL_ON_8G             ( ACC_REG_CTRL_PC | ACC_REG_CTRL_GSEL_HI)
#define ACC_REG_CTRL_OFF_8G            ( ACC_REG_CTRL_GSEL_HI)
	
  /* ------------------------------------------------------------------------------------------------
 *                                           Typedefs
 * ------------------------------------------------------------------------------------------------
 */

/* Self test assertion; return FALSE (failed) if condition is not met */
#define ST_ASSERT(cond) st( if (!(cond)) return false; )

#define HAL_ACC_INT    G_INIT1_PIN

//#define ACC_USE_HARDWARE_IIC        //Ê¹ÓÃÓ²¼þIIC

/* ------------------------------------------------------------------------------------------------
 *                                          Functions
 * ------------------------------------------------------------------------------------------------
 */
void HalAcc_Init(void);
void HalAcc_Configuration(void);
void HalAcc_CleanInterrupt(void);
bool HalAcc_Write(uint8_t register_address, uint8_t *pData, uint8_t bytes);
bool HalAcc_Read(uint8_t register_address, uint8_t *pData, uint8_t bytes);
bool HalAcc_ReadData(int16_t *pData);
void HalAcc_Sleep(void);

void HalAcc_GetData(uint8_t *data);

void HalAcc_PinInterruptInit(void);

void HalAcc_PinHanderUnInit(void);

/**************************************************************************************************
*/


#endif

/**************************************************************************************************
*/
