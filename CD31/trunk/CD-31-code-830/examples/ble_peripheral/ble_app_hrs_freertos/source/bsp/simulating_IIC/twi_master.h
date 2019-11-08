 /* Copyright (c) 2009 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is property of Nordic Semiconductor ASA.
 * Terms and conditions of usage are described in detail in NORDIC
 * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 *
 */

#ifndef __TWI_MASTER_H__
#define __TWI_MASTER_H__

/*lint ++flb "Enter library region" */

#include <stdbool.h>
#include <stdint.h>

//#include "hal_i2c.h"
#include "nrf_delay.h"
#include "nrf_gpio.h"
#include "I2C.h"
#include "board.h"

/** @file
* @brief Software controlled TWI Master driver.
*
*
* @defgroup lib_driver_twi_master Software controlled TWI Master driver
* @{
* @ingroup nrf_drivers
* @brief Software controlled TWI Master driver.
*
* Supported features:
* - Repeated start
* - No multi-master
* - Only 7-bit addressing
* - Supports clock stretching (with optional SMBus style slave timeout)
* - Tries to handle slaves stuck in the middle of transfer
*/

/*********************************************************************
* Macro Definition
*/
#define APP_IIC0    1
#define APP_IIC1    1  

#ifdef APP_IIC0
#define IIC0_SCL    G_SCL_PIN
#define IIC0_SDA    G_SDA_PIN
#endif

#ifdef APP_IIC1
#define IIC1_SCL    TP_SCL_PIN
#define IIC1_SDA    TP_SDA_PIN
#endif

#define TWI_READ_BIT                 (0x01)        //!< If this bit is set in the address field, transfer direction is from slave to master.

#define TWI_ISSUE_STOP               ((bool)true)  //!< Parameter for @ref twi_master_transfer
#define TWI_DONT_ISSUE_STOP          ((bool)false) //!< Parameter for @ref twi_master_transfer

#define TWI_DELAY()         while(0);//nrf_delay_us(4) /*!< Time to wait when pin states are changed. For fast-mode the delay can be zero and for standard-mode 4 us delay is sufficient. */

#define TWI1_DELAY()        nrf_delay_us(4) /*!< Time to wait when pin states are changed. For fast-mode the delay can be zero and for standard-mode 4 us delay is sufficient. */

#ifdef APP_IIC0
/*<-------------------------------------------------- IIC0 ------------------------------------------------------->*/
//#define TWI_SCL_HIGH()      do{nrf_gpio_cfg_output(IIC0_SCL);nrf_gpio_pin_set(IIC0_SCL);}while(0)
//#define TWI_SCL_LOW()       do{nrf_gpio_cfg_output(IIC0_SCL);nrf_gpio_pin_clear(IIC0_SCL);}while(0)
//#define TWI_SDA_HIGH()      do{nrf_gpio_cfg_output(IIC0_SDA);nrf_gpio_pin_set(IIC0_SDA);}while(0)
//#define TWI_SDA_LOW()       do{nrf_gpio_cfg_output(IIC0_SDA);nrf_gpio_pin_clear(IIC0_SDA);}while(0)
//#define TWI_SDA_INPUT()     do{nrf_gpio_cfg_input(IIC0_SDA, BUTTON_PULL);}while(0)
//#define TWI_SDA_OUTPUT()    do{nrf_gpio_cfg_output(IIC0_SDA);}while(0)
//#define TWI_SCL_OUTPUT()    do{nrf_gpio_cfg_output(IIC0_SCL);}while(0)

#define TWI_SCL_HIGH()   do { NRF_GPIO->OUTSET = (1UL << IIC0_SCL); } while (0)   /*!< Pulls SCL line high */
#define TWI_SCL_LOW()    do { NRF_GPIO->OUTCLR = (1UL << IIC0_SCL); } while (0)   /*!< Pulls SCL line low  */
#define TWI_SDA_HIGH()   do { NRF_GPIO->OUTSET = (1UL << IIC0_SDA);  } while (0)   /*!< Pulls SDA line high */
#define TWI_SDA_LOW()    do { NRF_GPIO->OUTCLR = (1UL << IIC0_SDA);  } while (0)   /*!< Pulls SDA line low  */
#define TWI_SDA_INPUT()  do { NRF_GPIO->DIRCLR = (1UL << IIC0_SDA);  } while (0)   /*!< Configures SDA pin as input  */
#define TWI_SDA_OUTPUT() do { NRF_GPIO->DIRSET = (1UL << IIC0_SDA);  } while (0)   /*!< Configures SDA pin as output */
#define TWI_SCL_OUTPUT() do { NRF_GPIO->DIRSET = (1UL << IIC0_SCL); } while (0)   /*!< Configures SCL pin as output */

//#define	TWI_DIS_CLK()		do { nrf_gpio_range_cfg_input(IIC0_SCL, IIC0_SCL, NRF_GPIO_PIN_NOPULL);}	while(0)
//#define	TWI_DIS_SDA()		do { nrf_gpio_range_cfg_input(IIC0_SDA, IIC0_SDA, NRF_GPIO_PIN_NOPULL);}	while(0)
//#define	TWI_DISABLE()		do { TWI_DIS_CLK(); TWI_DIS_SDA();}while(0)

#define TWI_SDA_READ()      ((NRF_GPIO->IN >> IIC0_SDA) & 0x1UL)                     /*!< Reads current state of SDA */
#define TWI_SCL_READ()      ((NRF_GPIO->IN >> IIC0_SCL) & 0x1UL)                    /*!< Reads current state of SCL */
#endif

#ifdef APP_IIC1
/*<-------------------------------------------------- IIC1 ------------------------------------------------------->*/
//#define TWI1_SCL_HIGH()      do{nrf_gpio_cfg_output(IIC1_SCL);nrf_gpio_pin_set(IIC1_SCL);}while(0)
//#define TWI1_SCL_LOW()       do{nrf_gpio_cfg_output(IIC1_SCL);nrf_gpio_pin_clear(IIC1_SCL);}while(0)
//#define TWI1_SDA_HIGH()      do{nrf_gpio_cfg_output(IIC1_SDA);nrf_gpio_pin_set(IIC1_SDA);}while(0)
//#define TWI1_SDA_LOW()       do{nrf_gpio_cfg_output(IIC1_SDA);nrf_gpio_pin_clear(IIC1_SDA);}while(0)
//#define TWI1_SDA_INPUT()     do{nrf_gpio_cfg_input(IIC1_SDA, BUTTON_PULL);}while(0)
//#define TWI1_SDA_OUTPUT()    do{nrf_gpio_cfg_output(IIC1_SDA);}while(0)
//#define TWI1_SCL_OUTPUT()    do{nrf_gpio_cfg_output(IIC1_SCL);}while(0)

#define TWI1_SCL_HIGH()   do { NRF_GPIO->OUTSET = (1UL << IIC1_SCL); } while (0)   /*!< Pulls SCL line high */
#define TWI1_SCL_LOW()    do { NRF_GPIO->OUTCLR = (1UL << IIC1_SCL); } while (0)   /*!< Pulls SCL line low  */
#define TWI1_SDA_HIGH()   do { NRF_GPIO->OUTSET = (1UL << IIC1_SDA);  } while (0)   /*!< Pulls SDA line high */
#define TWI1_SDA_LOW()    do { NRF_GPIO->OUTCLR = (1UL << IIC1_SDA);  } while (0)   /*!< Pulls SDA line low  */
#define TWI1_SDA_INPUT()  do { NRF_GPIO->DIRCLR = (1UL << IIC1_SDA);  } while (0)   /*!< Configures SDA pin as input  */
#define TWI1_SDA_OUTPUT() do { NRF_GPIO->DIRSET = (1UL << IIC1_SDA);  } while (0)   /*!< Configures SDA pin as output */
#define TWI1_SCL_OUTPUT() do { NRF_GPIO->DIRSET = (1UL << IIC1_SCL); } while (0)   /*!< Configures SCL pin as output */

#define    TWI1_DIS_CLK()        do { nrf_gpio_range_cfg_input(IIC1_SCL, IIC1_SCL, NRF_GPIO_PIN_NOPULL);}    while(0)
#define    TWI1_DIS_SDA()        do { nrf_gpio_range_cfg_input(IIC1_SDA, IIC1_SDA, NRF_GPIO_PIN_NOPULL);}    while(0)
#define    TWI1_DISABLE()        do { TWI1_DIS_CLK(); TWI1_DIS_SDA();}while(0)

#define TWI1_SDA_READ()      ((NRF_GPIO->IN >> IIC1_SDA) & 0x1UL)                     /*!< Reads current state of SDA */
#define TWI1_SCL_READ()      ((NRF_GPIO->IN >> IIC1_SCL) & 0x1UL)                    /*!< Reads current state of SCL */
#endif

/**
 * @brief Function for initializing TWI bus IO pins and checks if the bus is operational.
 *
 * Both pins are configured as Standard-0, No-drive-1 (open drain).
 *
 * @return
 * @retval true TWI bus is clear for transfers.
 * @retval false TWI bus is stuck.
 */
bool twi_master_init(void);


/**
 * @brief Function for initializing TWI bus IO pins and checks if the bus is operational.
 *
 * Both pins are configured as Standard-0, No-drive-1 (open drain).
 *
 * @return
 * @retval true TWI bus is clear for transfers.
 * @retval false TWI bus is stuck.
 */
bool twi1_master_init(void);


/**
 * @brief Function for transferring data over TWI bus.
 *
 * If TWI master detects even one NACK from the slave or timeout occurs, STOP condition is issued
 * and the function returns false.
 * Bit 0 (@ref TWI_READ_BIT) in the address parameter controls transfer direction;
 * - If 1, master reads data_length number of bytes from the slave
 * - If 0, master writes data_length number of bytes to the slave.
 *
 * @note Make sure at least data_length number of bytes is allocated in data if TWI_READ_BIT is set.
 * @note @ref TWI_ISSUE_STOP
 *
 * @param address Data transfer direction (LSB) / Slave address (7 MSBs).
 * @param data Pointer to data.
 * @param data_length Number of bytes to transfer.
 * @param issue_stop_condition If @ref TWI_ISSUE_STOP, STOP condition is issued before exiting function. If @ref TWI_DONT_ISSUE_STOP, STOP condition is not issued before exiting function. If transfer failed for any reason, STOP condition will be issued in any case.
 * @return
 * @retval true Data transfer succeeded without errors.
 * @retval false Data transfer failed.
 */
bool twi_master_transfer(uint8_t address, uint8_t *data, uint8_t data_length, bool issue_stop_condition);


/**
 * @brief Function for transferring data over TWI bus.
 *
 * If TWI master detects even one NACK from the slave or timeout occurs, STOP condition is issued
 * and the function returns false.
 * Bit 0 (@ref TWI_READ_BIT) in the address parameter controls transfer direction;
 * - If 1, master reads data_length number of bytes from the slave
 * - If 0, master writes data_length number of bytes to the slave.
 *
 * @note Make sure at least data_length number of bytes is allocated in data if TWI_READ_BIT is set.
 * @note @ref TWI_ISSUE_STOP
 *
 * @param address Data transfer direction (LSB) / Slave address (7 MSBs).
 * @param data Pointer to data.
 * @param data_length Number of bytes to transfer.
 * @param issue_stop_condition If @ref TWI_ISSUE_STOP, STOP condition is issued before exiting function. If @ref TWI_DONT_ISSUE_STOP, STOP condition is not issued before exiting function. If transfer failed for any reason, STOP condition will be issued in any case.
 * @return
 * @retval true Data transfer succeeded without errors.
 * @retval false Data transfer failed.
 */
bool twi1_master_transfer(uint8_t address, uint8_t *data, uint8_t data_length, bool issue_stop_condition);

/**
 *@}
 **/

/*lint --flb "Leave library region" */
#endif //TWI_MASTER_H
