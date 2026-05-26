//! @file stm32_rtc.c
//! @brief C Source File Template

// --------------------------------------------------------------------------------------------------- //
// COMPANY NAME : IOT POW.
//
// INTRODUCTION : STM32 RTC Device driver.
//
// AUTHOR(S) : Pongpot Chaiboonuang
// CONTACT E-MAIL ADDRESS : pongpot.c@gmail.com
//
// REFERENCE DOCUMENT :
// N/A
//
// NOTE :
// N/A
//
// --------------------------------------------------------------------------------------------------- //

// ---------- I M P O R T A N T   N O T E ------------------------------------------------------------ //
//
// --------------------------------------------------------------------------------------------------- //
// N/A

// ---------- (1) LOCAL PREDEFINE -------------------------------------------------------------------- //
#define __THIS_FILE_NAME__		"stm32_rtc.c"

// ---------- (2) STANDARD LIBRARY INCLUDE ----------------------------------------------------------- //
#include <kservice.h>

// ---------- (3) USER INCLUDE ----------------------------------------------------------------------- //
#include "stm32_rtc.h"
#include "sys/time.h"
#include <time_manager_core.h>

// ---------- (4) EXTERN OBJECT ---------------------------------------------------------------------- //
// N/A

// ---------- (5) INTERFACE FUNCTION ----------------------------------------------------------------- //
// N/A

// ---------- (6) DATA ------------------------------------------------------------------------------- //
// N/A

// ---------- (7) PRIVATE PROGRAMMING DEFINE --------------------------------------------------------- //
#define BKUP_REG_DATA 	0xA5A5

// ---------- (8) PRIVATE TYPEDEF -------------------------------------------------------------------- //
// N/A

// ---------- (9) PRIVATE FUNCTION ------------------------------------------------------------------- //
// N/A

// ---------- (10) PRIVATE DATA ----------------------------------------------------------------------- //


// ---------- (11) PRIVATE MACRO DEFINITION ---------------------------------------------------------- //
// N/A

// ---------- (12) SOURCE FILE IMPLEMENTATION -------------------------------------------------------- //
int stm32_rtc_init(void) {
	RTC_TimeTypeDef sTime = { 0 };
	RTC_DateTypeDef sDate = { 0 };

    if (HAL_RTCEx_BKUPRead(&TSC_CSP_RTC, RTC_BKP_DR1) != BKUP_REG_DATA)
    {
        HAL_RTCEx_BKUPWrite(&TSC_CSP_RTC, RTC_BKP_DR1, 0);
        printf("RTC hasn't been configured, please use <date> command to config.\r\n");

		/** Initialize RTC and set the Time and Date
		 */
		sTime.Hours = 0x0;
		sTime.Minutes = 0x0;
		sTime.Seconds = 0x0;
		sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
		sTime.StoreOperation = RTC_STOREOPERATION_RESET;
		if (HAL_RTC_SetTime(&TSC_CSP_RTC, &sTime, RTC_FORMAT_BCD) != HAL_OK) {
			return -TSC_ERROR;
		}
		sDate.WeekDay = RTC_WEEKDAY_MONDAY;
		sDate.Month = RTC_MONTH_JANUARY;
		sDate.Date = 0x1;
		sDate.Year = 0x25;

		if (HAL_RTC_SetDate(&TSC_CSP_RTC, &sDate, RTC_FORMAT_BCD) != HAL_OK) {
			return -TSC_ERROR;
		}
    }

	return TSC_EOK;
}

static int stm32_rtc_get_time(struct timeval *tv)
{
    RTC_TimeTypeDef RTC_TimeStruct = {0};
    RTC_DateTypeDef RTC_DateStruct = {0};
    struct tm tm_new = {0};

    if (HAL_RTCEx_BKUPRead(&TSC_CSP_RTC, RTC_BKP_DR1) != BKUP_REG_DATA)
    	return -TSC_ERROR;

    HAL_RTC_GetTime(&TSC_CSP_RTC, &RTC_TimeStruct, RTC_FORMAT_BIN);
    HAL_RTC_GetDate(&TSC_CSP_RTC, &RTC_DateStruct, RTC_FORMAT_BIN);

    tm_new.tm_sec  = RTC_TimeStruct.Seconds;
    tm_new.tm_min  = RTC_TimeStruct.Minutes;
    tm_new.tm_hour = RTC_TimeStruct.Hours;
    tm_new.tm_mday = RTC_DateStruct.Date;
    tm_new.tm_mon  = RTC_DateStruct.Month - 1;
    tm_new.tm_year = RTC_DateStruct.Year + 100;

    tv->tv_sec = timegm(&tm_new);

    return TSC_EOK;
}

static int stm32_rtc_set_time(time_t time_stamp)
{
    RTC_TimeTypeDef RTC_TimeStruct = {0};
    RTC_DateTypeDef RTC_DateStruct = {0};
    struct tm tm = {0};

    gmtime_r(&time_stamp, &tm);
    if (tm.tm_year < 100)
    {
        return -TSC_ERROR;
    }

    RTC_TimeStruct.Seconds = tm.tm_sec ;
    RTC_TimeStruct.Minutes = tm.tm_min ;
    RTC_TimeStruct.Hours   = tm.tm_hour;
    RTC_DateStruct.Date    = tm.tm_mday;
    RTC_DateStruct.Month   = tm.tm_mon + 1 ;
    RTC_DateStruct.Year    = tm.tm_year - 100;
    RTC_DateStruct.WeekDay = tm.tm_wday + 1;

    if (HAL_RTC_SetTime(&TSC_CSP_RTC, &RTC_TimeStruct, RTC_FORMAT_BIN) != HAL_OK)
    {
        return -TSC_ERROR;
    }
    if (HAL_RTC_SetDate(&TSC_CSP_RTC, &RTC_DateStruct, RTC_FORMAT_BIN) != HAL_OK)
    {
        return -TSC_ERROR;
    }

//    printf("RTC: set rtc time.\r\n");
    HAL_RTCEx_BKUPWrite(&TSC_CSP_RTC, RTC_BKP_DR1, BKUP_REG_DATA);

    return TSC_EOK;
}


static int stm32_rtc_get_secs(time_t *sec) {
	struct timeval tv;

	if (stm32_rtc_get_time(&tv) != TSC_EOK)
		return -TSC_ERROR;

	*(time_t*) sec = tv.tv_sec;
//	printf("RTC: get rtc_time %lu\r\n", (uint32_t)*sec);

	return TSC_EOK;
}

static int stm32_rtc_set_secs(time_t sec) {
	int result = TSC_EOK;

	if (stm32_rtc_set_time(sec)) {
		result = -TSC_ERROR;
	}
//	printf("RTC: set rtc_time %lu\r\n", (uint32_t)*sec);

	return result;
}

int time_manager_set_rtc_time(time_t now) {
	return stm32_rtc_set_secs(now);
}

int time_manager_get_rtc_time(time_t *now) {
	return stm32_rtc_get_secs(&*now);
}

uint8_t time_manager_is_rtc_syn(void) {
	 if (HAL_RTCEx_BKUPRead(&TSC_CSP_RTC, RTC_BKP_DR1) == BKUP_REG_DATA)
		 return TIME_MANAGER_SYNC;
	 else
		 return TIME_MANAGER_NOT_SYNC;
}
// ---------- (13) END OF SOURCE FILE IMPLEMENTATION ------------------------------------------------- //
// SOURCE FORMAT REVISION B
