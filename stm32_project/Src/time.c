#include "time.h"

void TIME_getdate_time(RTC_DateTypeDef **Date, RTC_TimeTypeDef **Time)
{
	HAL_RTC_GetTime(&hrtc, *Time, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, *Date, RTC_FORMAT_BIN);
}

void TIME_setRTC(RTC_TimeTypeDef sTime, RTC_DateTypeDef sDate)
{
	HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
	HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
}

void TIME_setTime( uint8_t hours, uint8_t minutes, uint8_t seconds, uint8_t week_day[], uint8_t month[], uint8_t day, uint8_t year)
{
	RTC_TimeTypeDef sTime;
	RTC_DateTypeDef sDate;
	
	sTime.Hours = hours;
  sTime.Minutes =  minutes;
  sTime.Seconds =  seconds;

	sDate.Date =  day;
  sDate.Year =  year;
	
	
	
		if (strcmp((const char *)week_day, "Mon") == 0) sDate.WeekDay = RTC_WEEKDAY_MONDAY;
	 else if (strcmp((const char *)week_day, "Tue") == 0) sDate.WeekDay = RTC_WEEKDAY_TUESDAY;	
	 else if (strcmp((const char *)week_day, "Wed") == 0) sDate.WeekDay = RTC_WEEKDAY_WEDNESDAY;			
	 else if (strcmp((const char *)week_day, "Thu") == 0) sDate.WeekDay = RTC_WEEKDAY_THURSDAY;			
	 else if (strcmp((const char *)week_day, "Fri") == 0) sDate.WeekDay = RTC_WEEKDAY_FRIDAY;			
	 else if (strcmp((const char *)week_day, "Sat") == 0) sDate.WeekDay = RTC_WEEKDAY_SATURDAY;			
	 else if (strcmp((const char *)week_day, "Sun") == 0) sDate.WeekDay = RTC_WEEKDAY_SUNDAY;		
	 else Error_Handler();
	 
	 if (strcmp((const char *)month, "Jan") == 0) sDate.Month = RTC_MONTH_JANUARY;
	 else if (strcmp((const char *)month, "Feb") == 0) sDate.Month = RTC_MONTH_FEBRUARY;	
	 else if (strcmp((const char *)month, "Mar") == 0) sDate.Month = RTC_MONTH_MARCH;			
	 else if (strcmp((const char *)month, "Apr") == 0) sDate.Month = RTC_MONTH_APRIL;			
	 else if (strcmp((const char *)month, "May") == 0) sDate.Month = RTC_MONTH_MAY;			
	 else if (strcmp((const char *)month, "Jun") == 0) sDate.Month = RTC_MONTH_JUNE;			
	 else if (strcmp((const char *)month, "Jul") == 0) sDate.Month = RTC_MONTH_JULY;
	 else if (strcmp((const char *)month, "Aug") == 0) sDate.Month = RTC_MONTH_AUGUST;			
	 else if (strcmp((const char *)month, "Sep") == 0) sDate.Month = RTC_MONTH_SEPTEMBER;			
	 else if (strcmp((const char *)month, "Oct") == 0) sDate.Month = RTC_MONTH_OCTOBER;			
	 else if (strcmp((const char *)month, "Nov") == 0) sDate.Month = RTC_MONTH_NOVEMBER;			
	 else if (strcmp((const char *)month, "Dec") == 0) sDate.Month = RTC_MONTH_DECEMBER;	 
	 else Error_Handler();
	 
	 TIME_setRTC(sTime,sDate);
	 
}