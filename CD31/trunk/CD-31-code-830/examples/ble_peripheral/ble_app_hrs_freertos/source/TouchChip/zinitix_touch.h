
#ifndef _ZINITIX_TOUCH_H
#define	_ZINITIX_TOUCH_H

#include <stdint.h>
#include <stdbool.h>
#include "board.h"
#include "nrf_gpio.h"
#include "nrf_drv_gpiote.h"
#include "nrf_delay.h"
#include "twi_master.h"
#include "tp_gesture.h"
#include "app_util_platform.h"

/*可能需要修改的宏定义，根据具体情况的不同*/
    /*分辨率*/   
#define TPD_RES_MAX_X		(239)
#define TPD_RES_MAX_Y		(239)
/*多指个数的定义1~10*/
#define MAX_SUPPORTED_FINGER_NUM	1

/*根据平台是否支持I2c dma mode 定义TPD_HIGH_SPEED_DMA*/

#define TPD_HIGH_SPEED_DMA

#ifdef TPD_HIGH_SPEED_DMA
#define TOUCH_POINT_MODE	0
#else
#define TOUCH_POINT_MODE	2
#endif

/*自动升级功能 ,需要使用时置1*/
/* Upgrade Method*/
#define TOUCH_ONESHOT_UPGRADE		0

/* gesture wakeup open 1*/
#define	USE_WAKEUP_GESTURE	         0

#define CHIP_OFF_DELAY			70	/*ms*/
#define CHIP_ON_DELAY			200	/*ms*/
#define FIRMWARE_ON_DELAY		150	/*ms*/

#define ZINITIX_INIT_RETRY_CNT	       3
#define I2C_SUCCESS		               1
#define I2C_FAIL		               0

/*报点相关结构体的定义*/
struct _ts_zinitix_coord {
  uint16_t    x;
  uint16_t    y;
  uint8_t    width;
  uint8_t    sub_status;
};

struct _ts_zinitix_point_info {
  uint16_t    status;
  #if TOUCH_POINT_MODE
  uint16_t event_flag;
  #else
  uint8_t    finger_cnt;
  uint8_t    time_stamp;
  #endif
  struct _ts_zinitix_coord    coord[MAX_SUPPORTED_FINGER_NUM];
};

struct _ts_zinitix_point_info touch_info;

/*2. Register Map一些寄存器的定义，用户不需修改，直接使用就可以了*/
#define ZINITIX_SWRESET_CMD		0x0000
#define ZINITIX_WAKEUP_CMD		0x0001
#define ZINITIX_IDLE_CMD		0x0004
#define ZINITIX_SLEEP_CMD		0x0005
#define ZINITIX_CLEAR_INT_STATUS_CMD	0x0003
#define ZINITIX_CALIBRATE_CMD		0x0006
#define ZINITIX_SAVE_STATUS_CMD		0x0007
#define ZINITIX_SAVE_CALIBRATION_CMD	0x0008
#define ZINITIX_RECALL_FACTORY_CMD	0x000f
#define ZINITIX_SENSITIVITY	0x0020
#define	ZINITIX_I2C_CHECKSUM_WCNT	0x016a
#define	ZINITIX_I2C_CHECKSUM_RESULT	0x016c
#define ZINITIX_DEBUG_REG		0x0115	//0~7
#define ZINITIX_TOUCH_MODE		0x0010
#define ZINITIX_CHIP_REVISION		0x0011
#define ZINITIX_FIRMWARE_VERSION	0x0012
#define ZINITIX_MINOR_FW_VERSION	0x0121
#define ZINITIX_DATA_VERSION_REG	0x0013
#define ZINITIX_HW_ID				0x0014
#define ZINITIX_SUPPORTED_FINGER_NUM	0x0015
#define ZINITIX_EEPROM_INFO			0x0018
#define ZINITIX_INITIAL_TOUCH_MODE	0x0019
#define ZINITIX_TOTAL_NUMBER_OF_X	0x0060
#define ZINITIX_TOTAL_NUMBER_OF_Y		0x0061
#define ZINITIX_DELAY_RAW_FOR_HOST	0x007f
#define ZINITIX_BUTTON_SUPPORTED_NUM	0x00B0
#define ZINITIX_BUTTON_SENSITIVITY			0x00B2
#define ZINITIX_X_RESOLUTION		0x00C0
#define ZINITIX_Y_RESOLUTION		0x00C1
#define ZINITIX_POINT_STATUS_REG	0x0080
#define ZINITIX_ICON_STATUS_REG		0x00AA
#define ZINITIX_AFE_FREQUENCY		0x0100
#define ZINITIX_DND_N_COUNT			0x0122
#define ZINITIX_DND_U_COUNT			0x0135
#define ZINITIX_RAWDATA_REG		    0x0200
#define ZINITIX_EEPROM_INFO_REG		0x0018
#define ZINITIX_INT_ENABLE_FLAG		0x00f0
#define ZINITIX_PERIODICAL_INTERRUPT_INTERVAL	0x00f1
#define ZINITIX_CHECKSUM_RESULT	0x012c
#define ZINITIX_INIT_FLASH		0x01d0
#define ZINITIX_WRITE_FLASH		0x01d1
#define ZINITIX_READ_FLASH		0x01d2

#define ZINITIX_INTERNAL_FLAG_03  0x011F          // default 0x0001
/*bit15 : gesture slide on/off
bit14 : gesture long press on/off
bit13 : gesture double click on/off
bit12 : gesture click on/off
bit 0 : gesture function enable/disable
è  不唤醒 ：set 0x011F to 0x0000
è  单击唤醒　：set 0x011F to 0x1001
è  双击唤醒　：set 0x011F to 0x2001
è  长按唤醒  ：set 0x011F to 0x4001
è  滑动唤醒　：set 0x011F to 0x8001
*/

#define REG_GESTURE_SLIDE_DISTANCE    0x0131  // default 30
 
#define REG_GESTURE_LONG_PRESS_MIN    0x0140  // default 50
#define REG_GESTURE_LONG_PRESS_MAX    0x0141  // default 100

//YX.20180710.定义唤醒的一些参数
#define GESTURE_SLIDE_DISTANCE    30        //80
#define GESTURE_LONG_PRESS_MIN    30        //3S
#define GESTURE_LONG_PRESS_MAX    80        //8S

#define BIT_DOWN			1
#define BIT_MOVE			2
#define BIT_UP				3
#define BIT_PALM			4
#define BIT_PALM_REJECT		5
#define BIT_WAKEUP			6
#define RESERVED_1			7
#define BIT_WEIGHT_CHANGE	8
#define BIT_PT_NO_CHANGE	9
#define BIT_REJECT			10
#define BIT_PT_EXIST		11
#define RESERVED_2			12
#define BIT_MUST_ZERO		13
#define BIT_DEBUG			14
#define BIT_ICON_EVENT		15

#define SUB_BIT_EXIST		0
#define SUB_BIT_DOWN		1
#define SUB_BIT_MOVE		2
#define SUB_BIT_UP			3
#define SUB_BIT_UPDATE		4
#define SUB_BIT_WAIT		5

 /*Test Mode (Monitoring Raw Data) */
#define SEC_DND_N_COUNT			13//10//
#define SEC_DND_U_COUNT			2//2//
#define SEC_DND_FREQUENCY		80//99// /* 200khz */
#define SEC_PDND_N_COUNT		14
#define SEC_PDND_U_COUNT		6
#define SEC_PDND_FREQUENCY		79

/* preriod raw data interval */

#define RAWDATA_DELAY_FOR_HOST		100

#define zinitix_bit_set(val, n)		((val) &= ~(1<<(n)), (val) |= (1<<(n)))
#define zinitix_bit_clr(val, n)		((val) &= ~(1<<(n)))
#define zinitix_bit_test(val, n)	((val) & (1<<(n)))
#define zinitix_swap_v(a, b, t)	((t) = (a), (a) = (b), (b) = (t))
#define zinitix_swap_16(s) (((((s) & 0xff) << 8) | (((s) >> 8) & 0xff)))

#endif





