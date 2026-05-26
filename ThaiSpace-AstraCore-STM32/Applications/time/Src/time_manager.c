//! @file time_manager.c
//! @brief C Source File Template

// --------------------------------------------------------------------------------------------------- //
// COMPANY NAME : IOT POW.
//
// INTRODUCTION : CSP time manager application.
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
#define __THIS_FILE_NAME__		"time_manager.c"

// ---------- (2) STANDARD LIBRARY INCLUDE ----------------------------------------------------------- //
#include "app_modules_config.h"
#include <kservice.h>
#include <time_manager_core.h>

#define LOG_TAG		"time_manager"
#define LOG_LVL		DBG_LOG
#include <tsc_log_core.h>

#if ENABLE_TIME_RTC
#include "stm32_rtc.h"
#endif

#if ENABLE_TIME_PPS && ENABLE_HW_GPIO
#include <time_pps_it.h>
#endif

// ---------- (3) USER INCLUDE ----------------------------------------------------------------------- //
// N/A

// ---------- (4) EXTERN OBJECT ---------------------------------------------------------------------- //
// N/A

// ---------- (5) INTERFACE FUNCTION ----------------------------------------------------------------- //
// N/A

// ---------- (6) DATA ------------------------------------------------------------------------------- //
// N/A

// ---------- (7) PRIVATE PROGRAMMING DEFINE --------------------------------------------------------- //
// N/A

// ---------- (8) PRIVATE TYPEDEF -------------------------------------------------------------------- //
// N/A

// ---------- (9) PRIVATE FUNCTION ------------------------------------------------------------------- //
// N/A

// ---------- (10) PRIVATE DATA ----------------------------------------------------------------------- //
// N/A

// ---------- (11) PRIVATE MACRO DEFINITION ---------------------------------------------------------- //
// N/A

// ---------- (12) SOURCE FILE IMPLEMENTATION -------------------------------------------------------- //
#if ENABLE_TIME_RTC || (ENABLE_TIME_PPS && ENABLE_HW_GPIO) || (ENABLE_TIME_GNSS && ENABLE_HW_UART)

__weak int time_manager_get_rtc_time(time_t *now){
	return TSC_ENOSYS;
}

__weak int time_manager_get_pps_time(time_t *now){
	return TSC_ENOSYS;
}

__weak int time_manager_set_rtc_time(time_t now){
	return TSC_ENOSYS;
}

__weak int time_manager_set_pps_time(time_t now){
	return TSC_ENOSYS;
}

__weak uint8_t time_manager_is_rtc_syn(void){
	return TIME_MANAGER_NOT_SYNC;
}

__weak uint8_t time_manager_is_pps_syn(void){
	return TIME_MANAGER_NOT_SYNC;
}

/*
 * This function initializes rtc_core
 */
int time_manager_init(void)
{
    LOG_I("time_manager_init: Initializing Time Management system...");
#if ENABLE_TIME_RTC
	LOG_D("time_manager_init: Initializing STM32 Internal RTC");
	stm32_rtc_init();
#endif

#if ENABLE_TIME_PPS && ENABLE_HW_GPIO
    LOG_D("time_manager_init: Initializing PPS input (GPIO)");
    time_pps_int();
#endif

    return TSC_EOK;
}

/**
 * Set system date(time not modify, UTC time).
 */
int time_manager_set_date(uint32_t year, uint32_t month, uint32_t day)
{
    time_t now;
    struct tm tm_new;
    int ret = TSC_EOK;

    LOG_I("time_manager_set_date: Request to set date to %04lu-%02lu-%02lu", year, month, day);

    /* get current time */
    now = time(NULL);

    /* converts Unix Timestamp into struct tm. */
    gmtime_r(&now, &tm_new);

    /* update date. */
    tm_new.tm_year = year - 1900;
    tm_new.tm_mon  = month - 1; /* tm_mon: 0~11 */
    tm_new.tm_mday = day;

    /* converts current data time to Unix Timestamp. */
    now = timegm(&tm_new);


#if ENABLE_TIME_RTC
    /* update to RTC device. */
    ret |= time_manager_set_rtc_time(now);
#endif
#if ENABLE_TIME_PPS && ENABLE_HW_GPIO
    /* update to PPS device. */
    ret |= time_manager_set_pps_time(now);
#endif

    if (ret == TSC_EOK) LOG_I("time_manager_set_date: Success");
    else LOG_E("time_manager_set_date: Failed to update hardware clocks (err=%d)", ret);

    return ret;
}

/**
 * Set system time(date not modify, utc timezone).
 */
int time_manager_set_time(uint32_t hour, uint32_t minute, uint32_t second)
{
    time_t now;
    struct tm tm_new;
    int ret = TSC_EOK;

    LOG_I("time_manager_set_time: Request to set time to %02lu:%02lu:%02lu", hour, minute, second);

    /* get current time */
    now = time(NULL);

    if(now > 0) {
		/* converts Unix Timestamp into struct tm. */
		gmtime_r(&now, &tm_new);
    }
    else {
    	/* Fill up minimum date. */
    	tm_new.tm_year = 2000 - 1900;
    	tm_new.tm_mon = 0;
    	tm_new.tm_mday = 1;
    }

    /* update time. */
    tm_new.tm_hour = hour;
    tm_new.tm_min  = minute;
    tm_new.tm_sec  = second;

    /* converts current data time to Unix Timestamp. */
    now = timegm(&tm_new);

#if ENABLE_TIME_RTC
    /* update to RTC device. */
    ret |= time_manager_set_rtc_time(now);
#endif
#if ENABLE_TIME_PPS && ENABLE_HW_GPIO
    /* update to PPS device. */
    ret |= time_manager_set_pps_time(now);
#endif

    if (ret == TSC_EOK) LOG_I("time_manager_set_time: Success");
    else LOG_E("time_manager_set_time: Failed to update hardware clocks (err=%d)", ret);

    return ret;
}

#ifdef TSC_USING_FINSH
#include <finsh.h>
/**
 * get date and time or set (local timezone) [year month day hour min sec]
 */
static void date(int argc, char **argv)
{
    if (argc == 1)
    {
        time_t now;
        int error[2];
        /* output current time */
        now = time(NULL);
        if(now > 0)
        	tsc_kprintf("%.*s", 25, ctime(&now));
        else
        	tsc_kprintf("Both RTC and PPS not synchronize\r\n");

        error[0] = time_manager_get_pps_time(&now);
        if(error[0])
        	tsc_kprintf("PPS Time: Not sync or not enable.\r\n");
        else
        	tsc_kprintf("PPS Time: %ld\r\n", (uint32_t)now);

        error[1] = time_manager_get_rtc_time(&now);
        if(error[1])
        	tsc_kprintf("RTC Time: Not sync or not enable.\r\n");
        else
        	tsc_kprintf("RTC Time: %ld\r\n", (uint32_t)now);

    	tsc_kprintf("Main time source: ");
        if(!error[0])
        	tsc_kprintf("PPS\r\n\r\n");
        else if(!error[1])
        	tsc_kprintf("RTC\r\n\r\n");
        else
        	tsc_kprintf("RTC and PPS not synchronize\r\n\r\n");

    }
    else if (argc >= 7)
    {
        /* set time and date */
        uint16_t year;
        uint8_t month, day, hour, min, sec;

        year = atoi(argv[1]);
        month = atoi(argv[2]);
        day = atoi(argv[3]);
        hour = atoi(argv[4]);
        min = atoi(argv[5]);
        sec = atoi(argv[6]);
        if (year > 2099 || year < 2000)
        {
            tsc_kprintf("year is out of range [2000-2099]\n");
            return;
        }
        if (month == 0 || month > 12)
        {
            tsc_kprintf("month is out of range [1-12]\n");
            return;
        }
        if (day == 0 || day > 31)
        {
            tsc_kprintf("day is out of range [1-31]\n");
            return;
        }
        if (hour > 23)
        {
            tsc_kprintf("hour is out of range [0-23]\n");
            return;
        }
        if (min > 59)
        {
            tsc_kprintf("minute is out of range [0-59]\n");
            return;
        }
        if (sec > 59)
        {
            tsc_kprintf("second is out of range [0-59]\n");
            return;
        }
        time_manager_set_time(hour, min, sec);
        time_manager_set_date(year, month, day);
    }
    else
    {
        tsc_kprintf("please input: date [year month day hour min sec] or date\n");
        tsc_kprintf("e.g: date 2018 01 01 23 59 59 or date\n");
    }
}

TSC_CMD_EXPORT(date, get date and time or set (local timezone) [year month day hour min sec])
#endif /* TSC_USING_FINSH */

#endif /* ENABLE_TIME_RTC || (ENABLE_TIME_PPS && ENABLE_HW_GPIO) || (ENABLE_TIME_GNSS && ENABLE_HW_UART) */

// ---------- (13) END OF SOURCE FILE IMPLEMENTATION ------------------------------------------------- //
// SOURCE FORMAT REVISION B
