#include "app_modules_config.h"
#if ENABLE_SCRIPT_MANAGER

/**
 * Example platform overrides for time hooks used by the script scheduler.
 *
 * To enable, add this file to your project and define
 *   -DSCRIPT_PLATFORM_ENABLE_RTC_HOOK
 * in your build (Project -> Properties -> C/C++ Build -> Settings -> Symbols),
 * or by adding the macro to your compiler flags.
 *
 * These functions override the weak implementations in script_manager.c.
 * Adjust includes to match your STM32Cube project (e.g., rtc.h/main.h).
 */

#include "script_manager.h"
#define SCRIPT_PLATFORM_ENABLE_RTC_HOOK
#if defined(SCRIPT_PLATFORM_ENABLE_RTC_HOOK)

#include "stm32l4xx_hal.h"
#include "stm32l4xx_hal_rtc.h"

// Replace with your project's RTC handle header if available (e.g., "rtc.h").
extern RTC_HandleTypeDef hrtc;

int script_platform_get_hhmm(int *hh, int *mm) {
    if (!hh || !mm) return -1;
    RTC_TimeTypeDef t = {0};
    RTC_DateTypeDef d = {0};
    if (HAL_RTC_GetTime(&hrtc, &t, RTC_FORMAT_BIN) != HAL_OK) {
        return -1;
    }
    // Per HAL requirement, read date after time read
    if (HAL_RTC_GetDate(&hrtc, &d, RTC_FORMAT_BIN) != HAL_OK) {
        return -1;
    }
    *hh = (int)t.Hours;
    *mm = (int)t.Minutes;
    return 0;
}

uint32_t script_platform_millis(void) {
    return HAL_GetTick();
}

#if 1
int script_platform_get_datetime(int *year, int *month, int *day, int *hh, int *mm, int *ss) {
    if (!year || !month || !day || !hh || !mm || !ss) return -1;
    RTC_TimeTypeDef t = {0};
    RTC_DateTypeDef d = {0};
    if (HAL_RTC_GetTime(&hrtc, &t, RTC_FORMAT_BIN) != HAL_OK) return -1;
    if (HAL_RTC_GetDate(&hrtc, &d, RTC_FORMAT_BIN) != HAL_OK) return -1;
    // HAL returns year offset from 2000 in BIN mode for L4 if configured that way; adjust as needed
    *year  = (int)(2000 + d.Year);
    *month = (int)d.Month;
    *day   = (int)d.Date;
    *hh    = (int)t.Hours;
    *mm    = (int)t.Minutes;
    *ss    = (int)t.Seconds;
    return 0;
}
#endif

#endif /* SCRIPT_PLATFORM_ENABLE_RTC_HOOK */


#endif /* ENABLE_SCRIPT_MANAGER */
