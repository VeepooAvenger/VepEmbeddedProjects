#ifndef TIME_MANAGE_H
#define TIME_MANAGE_H

#include "FreeRTOS.h"
#include "timers.h"
#include "app_timer.h"

#include "app_type.h"

#define APP_TIMER_PRESCALER              0                                /**< Value of the RTC1 PRESCALER register. */
#define OSTIMER_WAIT_FOR_QUEUE           15 

void timers_init(void);

//秒表定时器开启
void Timer_StopWatchStop(void);

//秒表定时器停止
void Timer_StopWatchStart(void);

#endif

