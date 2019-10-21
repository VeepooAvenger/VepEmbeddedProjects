#ifndef TIME_MANAGE_H
#define TIME_MANAGE_H

#include "FreeRTOS.h"
#include "timers.h"
#include "app_timer.h"

#include "app_type.h"

#define APP_TIMER_PRESCALER              0                                /**< Value of the RTC1 PRESCALER register. */
#define OSTIMER_WAIT_FOR_QUEUE           15 

void timers_init(void);

void APP_Time_OledRefreshTimerStart(void);
void APP_Time_OledRefreshTimerStop(void);

void APP_Time_SendData_Start(void);
void APP_Time_SendData_Stop(void);

void APP_Time_SendADCData_Start(void);
void APP_Time_SendADCData_Stop(void);

#endif

