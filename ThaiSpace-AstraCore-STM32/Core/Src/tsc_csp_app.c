/**
 ******************************************************************************
 * @file    tsc_csp_app.c
 * @brief   TSC CSP Application Initialization
 * @details Main application initialization file that conditionally
 *          initializes all CSP-based application modules based on
 *          configuration settings in app_modules_config.h
 *
 *          This file follows the design pattern demonstrated in
 *          hw_can_conditional_example.c, using conditional compilation
 *          to exclude disabled modules from the build, reducing code
 *          size and memory footprint.
 ******************************************************************************
 * @author  Chaturong
 ******************************************************************************
 */

/* ---------- (1) INCLUDE MASTER CONFIGURATION FILE FIRST --------------- */
#include "app_modules_config.h"

/* ---------- (2) STANDARD LIBRARY INCLUDES ----------------------------- */
#include <kservice.h>
#include <shell.h>

/* ---------- (3) PROJECT INCLUDES ------------------------------------- */
#include "tsc_csp_app.h"

/* ======================================================================== */
/*                        CONDITIONAL MODULE INCLUDES                      */
/* ======================================================================== */

#if ENABLE_LIBCSP
#include <csp_route_server.h>
#endif

#if CSP_ENABLE_THREAD
#include <thread_manager_core.h>
#endif

#if ENABLE_TIME_GNSS || ENABLE_TIME_CTP
#include <time_manager_core.h>
#include <time_pps_it.h>
#endif

#if ENABLE_TIME_GNSS
#include "time_gnss.h"
#endif

#if CSP_ENABLE_CONFIG
#include <config_core.h>
#endif

#if CSP_ENABLE_FILE
#include <file_core.h>
#endif

#if ENABLE_LITTLEFS && ENABLE_S25FL128L
#include <s25fl128l_littlefs.h>
#endif

#if ENABLE_LITTLEFS && ENABLE_W25QXX
#include <w25qxx_littlefs.h>
#endif

#if CSP_ENABLE_LOG
#include "tsc_log_core.h"
#endif
/* ======================================================================== */
/*                        CONDITIONAL INITIALIZATION FUNCTIONS             */
/* ======================================================================== */

/**
 * @brief Initialize CSP routing module
 * @note Only compiled if ENABLE_LIBCSP is enabled
 */
#if ENABLE_LIBCSP
static void init_csp_routing(void) {
	csp_task_init();
}
#else
static void init_csp_routing(void)
{
    /* Stub function - CSP routing disabled */
}
#endif /* ENABLE_LIBCSP */

/**
 * @brief Initialize thread manager module
 * @note Only compiled if CSP_ENABLE_THREAD is enabled
 */
#if CSP_ENABLE_THREAD
static void init_thread_manager(void) {
	thread_manager_init();
}
#else
static void init_thread_manager(void)
{
    /* Stub function - thread manager disabled */
}
#endif /* CSP_ENABLE_THREAD */

/**
 * @brief Initialize time management module
 * @note Only compiled if any time feature is enabled
 */
#if ENABLE_TIME_GNSS || ENABLE_TIME_CTP
#include <time.h>
static void init_time_manager(void) {
	static bool is_init = false;
	if (is_init) return;

	time_manager_init();

	/* 1. Parse Build Time (Assuming build machine is in UTC+7) */
	const char *build_date = __DATE__; // "Mmm dd yyyy"
	const char *build_time_str = __TIME__; // "hh:mm:ss"
	const char *months[] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };
	struct tm tm_build = { 0 };
	int month = 0;

	for (int i = 0; i < 12; i++) {
		if (strncmp(build_date, months[i], 3) == 0) {
			month = i;
			break;
		}
	}

	tm_build.tm_mon = month;
	sscanf(build_date + 4, "%d %d", &tm_build.tm_mday, &tm_build.tm_year);
	tm_build.tm_year -= 1900;
	sscanf(build_time_str, "%d:%d:%d", &tm_build.tm_hour, &tm_build.tm_min, &tm_build.tm_sec);
	tm_build.tm_isdst = -1;

	/* Convert build time to time_t (UTC) */
	time_t build_timestamp = timegm(&tm_build);

	/* 2. Get current system time */
	time_t now = time(NULL);

	/* 3. Compare and Update if RTC is behind build time */
	if (now < build_timestamp) {
		LOG_I("RTC time is behind build time. Updating...");
		time_manager_set_date(tm_build.tm_year + 1900, tm_build.tm_mon + 1, tm_build.tm_mday);
		time_manager_set_time(tm_build.tm_hour, tm_build.tm_min, tm_build.tm_sec);
	} else {
		LOG_I("RTC time is up-to-date.");
	}

#if ENABLE_TIME_PPS && ENABLE_HW_GPIO
	time_pps_int();
#endif
	is_init = true;
}
#else
static void init_time_manager(void)
{
    /* Stub function - time management disabled */
}
#endif /* ENABLE_TIME_GNSS || ENABLE_TIME_CTP */

/**
 * @brief Initialize configuration manager
 * @note Only compiled if CSP_ENABLE_CONFIG is enabled
 */
#if CSP_ENABLE_CONFIG
static void init_config_manager(void) {
	config_manager_init();
}
#endif /* CSP_ENABLE_CONFIG */

/**
 * @brief Initialize GNSS time synchronization
 * @note Only compiled if ENABLE_TIME_GNSS is enabled
 */
#if ENABLE_TIME_GNSS
static void init_gnss_time(void) {
#if ENABLE_HW_UART
	time_gnss_init();
#endif
}
#endif /* ENABLE_TIME_GNSS */

/**
 * @brief Initialize Nor flash manager
 * @note Only compiled if ENABLE_LITTLEFS && ENABLE_S25FL128L is enabled
 */
#if ENABLE_LITTLEFS && ENABLE_S25FL128L
static void init_s25fl128l_manager(void) {
	s25fl128l_littlefs_init();
}
#endif /* ENABLE_LITTLEFS && ENABLE_S25FL128L */


/**
 * @brief Show the version of TSC
 */
void tsc_show_version(void) {
	tsc_kprintf("\n \\ | /\n");
	tsc_kprintf("-TSC1-     TSC-1 Satellite Operating System\n");
	tsc_kprintf(" / | \\     %d.%d.%d build %s %s\n", 0, 0, 0, __DATE__, __TIME__);
	tsc_kprintf("2025 National Astronomical Research Institute of Thailand (Public Organisation)\n");
}

/* ======================================================================== */
/*                        MAIN INITIALIZATION                             */
/* ======================================================================== */

/**
 * @brief Initialize logging system with all backends and async support
 */
void tsc_log_system_init(void) {
#if CSP_ENABLE_LOG
	tsc_log_init();
#if TSC_LOG_BACKEND_USING_CONSOLE
	extern int tsc_log_console_backend_init(void);
	tsc_log_console_backend_init();
#endif

#ifdef TSC_LOG_BACKEND_USING_FILE
	extern int tsc_log_file_backend_init(void);
	tsc_log_file_backend_init();
#endif

#ifdef TSC_LOG_USING_ASYNC_OUTPUT
	tsc_log_async_init();
#endif
#endif /* CSP_ENABLE_LOG */
}

/**
 * @brief Main application initialization function
 * @details Initializes all enabled modules based on configuration
 */
void tsc_csp_app_init(void) {

//
//	/* 1. Initialize Nor flash manager */
#if ENABLE_LITTLEFS && ENABLE_S25FL128L
	init_s25fl128l_manager();
#endif

	/* 2. Display version information */
	tsc_show_version();

	/* 5. Initialize shell/CLI */
#if ENABLE_SHELL
	finsh_system_init();
#endif

	/* 6. Initialize configuration manager */
#if CSP_ENABLE_CONFIG
	init_config_manager();
#endif

	/* 7. Initialize CSP routing */
#if ENABLE_LIBCSP
	init_csp_routing();
#endif

	/* 8. Initialize thread manager */
#if CSP_ENABLE_THREAD
	init_thread_manager();
#endif

	/* 2. Initialize Logging (Async thread will start after this) */
#if CSP_ENABLE_LOG
	tsc_log_system_init();
#endif

	/* 3. Initialize time management early */
#if ENABLE_TIME_GNSS || ENABLE_TIME_CTP
	init_time_manager();
#endif

	/* 9. Initialize GNSS time synchronization */
#if ENABLE_TIME_GNSS
	init_gnss_time();
#endif

}

/* ======================================================================== */
/*                        SHELL COMMANDS                                   */
/* ======================================================================== */

#if ENABLE_SHELL
/**
 * @brief Hello world test command
 * @param argc Argument count
 * @param argv Argument values
 * @return 0 on success
 */
int hello(int argc, char *argv[]) {
	if (argc == 1) {
		tsc_kprintf("Hello from shell !\r\n");
	} else if (argc >= 2) {
		tsc_kprintf("Hello from shell with arg\r\n");
		for (int i = 1; i < argc; i++) {
			tsc_kprintf("ARGV[%d]=%s\r\n", i, argv[i]);
		}
	}
	return 0;
}
TSC_CMD_EXPORT(hello, Say hello from shell);

/**
 * @brief Check FreeRTOS heap memory
 * @param argc Argument count
 * @param argv Argument values
 * @return 0 on success
 */
int check_os_heap(int argc, char *argv[]) {
	size_t currentFree = xPortGetFreeHeapSize();
	size_t minEverFree = xPortGetMinimumEverFreeHeapSize();

	printf("Current free heap: %u bytes\r\n", (unsigned int) currentFree);
	printf("Minimum ever free heap: %u bytes\r\n", (unsigned int) minEverFree);

	return 0;
}
TSC_CMD_EXPORT(check_os_heap, FreeRTOS free heap memory.);

/**
 * @brief Check system heap memory
 * @param argc Argument count
 * @param argv Argument values
 * @return 0 on success
 */
int check_sys_heap(int argc, char *argv[]) {
	extern void* _sbrk(ptrdiff_t incr);
	extern char _end;
	extern char _estack;
	extern uint32_t _Min_Stack_Size; /* Symbol defined in the linker script */

	char *heapend = (char*) ((uint32_t) &_estack - (uint32_t) &_Min_Stack_Size);
	char *heap_top = (char*) _sbrk(0);
	char *stack_ptr = (char*) __get_MSP();

	uint32_t total_bytes = heapend - (char*) (uint32_t) &_end;
	uint32_t free_bytes = ((stack_ptr < heapend) ? stack_ptr : heapend) - heap_top;

	printf("Total heap size: %lu bytes\r\n", total_bytes);
	printf("Free heap estimate: %lu bytes\r\n", free_bytes);
	printf("Used heap estimate: %lu bytes\r\n", total_bytes - free_bytes);

	return 0;
}
TSC_CMD_EXPORT(check_sys_heap, System free heap memory.);
#endif /* ENABLE_SHELL */
