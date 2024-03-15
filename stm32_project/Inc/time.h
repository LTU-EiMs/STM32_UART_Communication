#ifndef INC_TIME_H_
#define INC_TIME_H_
#include "rtc.h"
#include "stdlib.h"
#include "string.h"


void TIME_getdate_time(RTC_DateTypeDef **Date, RTC_TimeTypeDef **Time);
void TIME_setRTC(RTC_TimeTypeDef sTime, RTC_DateTypeDef sDate);
void TIME_setTime( uint8_t hours, uint8_t minutes, uint8_t seconds, uint8_t week_day[], uint8_t month[], uint8_t day, uint8_t year);

#endif