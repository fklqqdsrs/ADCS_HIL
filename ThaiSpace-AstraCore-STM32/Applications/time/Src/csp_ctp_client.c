//! @file csp_ctp_app.c
//! @brief C Source File Template

// --------------------------------------------------------------------------------------------------- //
// COMPANY NAME : IOT POW.
//
// INTRODUCTION : CSP time application.
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

#include "app_modules_config.h"
#if ENABLE_TIME_CTP

// ---------- (1) LOCAL PREDEFINE -------------------------------------------------------------------- //
#define __THIS_FILE_NAME__		"csp_ctp_app.c"

// ---------- (2) STANDARD LIBRARY INCLUDE ----------------------------------------------------------- //
#include <kservice.h>
#include <csp/arch/csp_clock.h>

// ---------- (3) USER INCLUDE ----------------------------------------------------------------------- //
#include <csp_ctp_protocol.h>
#ifdef TSC_USING_FINSH
#include "finsh.h"
#endif

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

// ---------- (10) PRIVATE DATA ---------------------------------------------------------------------- //
// N/A

// ---------- (11) PRIVATE MACRO DEFINITION ---------------------------------------------------------- //
// N/A

// ---------- (12) SOURCE FILE IMPLEMENTATION -------------------------------------------------------- //

/**
   Get time.

   This function is providing a working implementation for freeRTOS on STM32.
   This function is expected to be equivalent to standard POSIX clock_gettime(CLOCK_REALTIME, ...).

   @param[out] time current time.
*/
void csp_clock_get_time(csp_timestamp_t * gettime) {
	time_t now = time(NULL);

	gettime->tv_nsec = 0;

	if(now > 0)
		gettime->tv_sec = time(NULL);
	else
		gettime->tv_sec = 0;
}

/**
   Set time.

   This function is providing a working implementation for freeRTOS on STM32.
   This function is expected to be equivalent to standard POSIX clock_settime(CLOCK_REALTIME, ...).

   @param[in] time time to set.
   @return #CSP_ERR_NONE on success.
*/
int csp_clock_set_time(const csp_timestamp_t * settime) {
	csp_log_info("csp_clock_set_time(), sec: %ld, nsec: %ld", settime->tv_sec, settime->tv_nsec);

	if(settime->tv_sec == 0)
		return CSP_ERR_INVAL;

	if(stime((const time_t*)&settime->tv_sec))
		return CSP_ERR_INVAL;

	return CSP_ERR_NONE;
}

#ifdef TSC_USING_FINSH
static void __csp_ctp_get_time(int argc, char **argv) {
	struct csp_ctp_message msg;
	int timeout = 1000, node;
	uint32_t sec, nsec;

	if (argc > 1) {
		node = atoi(argv[1]);
		if ((node >= 0) && (node <= 255)) {
			if (argc > 2) {
				int buff = atoi(argv[2]);
				if ((buff >= 0) && (buff <= 5000)) {
					timeout = buff;
				}
			}

			if(!csp_ctp_get_time(node, timeout, &msg)){
				sec = msg.get_time.tv_sec;
				nsec = msg.get_time.tv_nsec;
				tsc_kprintf("CSP response ctp_get_time node:%d sec:%d nsec:%d \r\n", node, sec, nsec);
			}else{
				tsc_kprintf("CSP response ctp_get_time error \r\n");
			}
			return;
		}

		tsc_kprintf("<node> must be in 0 - 255 range.\r\n");
		tsc_kprintf("[timeout] must be in 0 - 5000 ms range.\r\n");
	} else {
		tsc_kprintf("Please input: csp_ctp_get_time <node> [timeout]\r\n");
	}
}

static void __csp_ctp_set_time(int argc, char **argv) {
	struct csp_ctp_message msg;
	int timeout = 1000, node;
	uint32_t sec, nsec;

	if (argc > 3) {
		node = atoi(argv[1]);
		sec = atol(argv[2]);
		nsec = atol(argv[3]);
		if ((node >= 0) && (node <= 255) && (sec >= 0) && (nsec >= 0)) {
			if (argc > 4) {
				int buff = atoi(argv[4]);
				if ((buff >= 0) && (buff <= 5000)) {
					timeout = buff;
				}
			}

			msg.set_time.tv_sec = sec;
			msg.set_time.tv_nsec = nsec;

			tsc_kprintf("CSP ctp_set_time node:%d sec:%d nsec:%d timeout:%d \r\n", node, sec, nsec, timeout);
			if(!csp_ctp_set_time(node, timeout, &msg)){
				sec = msg.set_time.tv_sec;
				nsec = msg.set_time.tv_nsec;

				tsc_kprintf("CSP response ctp_set_time node:%d sec:%d nsec:%d \r\n", node, sec, nsec);
			}else{
				tsc_kprintf("CSP response ctp_set_time error \r\n");
			}
			return;
		}

		tsc_kprintf("<node> must be in 0 - 255 range.\r\n");
		tsc_kprintf("<sec> must more than 0 \r\n");
		tsc_kprintf("<nsec> must more than 0 \r\n");
		tsc_kprintf("[timeout] must be in 0 - 5000 ms range.\r\n");
	} else {
		tsc_kprintf("Please input: csp_ctp_set_time <node> <sec> <nsec> [timeout]\r\n");
	}
}

static void __csp_ctp_status(int argc, char **argv) {
	struct csp_ctp_message msg;
	int timeout = 1000, node;

	if (argc > 1) {
		node = atoi(argv[1]);
		if ((node >= 0) && (node <= 255)) {
			if (argc > 2) {
				int buff = atoi(argv[2]);
				if ((buff >= 0) && (buff <= 5000)) {
					timeout = buff;
				}
			}

			if(!csp_ctp_status(node, timeout, &msg)){
				tsc_kprintf("CSP response ctp_get_status node:%d \r\n", node);
				tsc_kprintf("%s", msg.status.time_str);
				tsc_kprintf("Main Time Source: %s\r\n", msg.status.main_source);
				tsc_kprintf("PPS Time: %ld \r\n", msg.status.pps_time);
				tsc_kprintf("RTC Time: %ld \r\n", msg.status.rtc_time);
			}else{
				tsc_kprintf("CSP response ctp_get_status error \r\n");
			}
			return;
		}

		tsc_kprintf("<node> must be in 0 - 255 range.\r\n");
		tsc_kprintf("[timeout] must be in 0 - 5000 ms range.\r\n");
	} else {
		tsc_kprintf("Please input: csp_ctp_get_status <node> [timeout]\r\n");
	}
}

/* -------------------------------------------------------------------------- */
/* Command Registration (CLI binding)                                         */
/* -------------------------------------------------------------------------- */
TSC_CMD_EXPORT_ALIAS(__csp_ctp_get_time, csp_time_get, CSP time get.);
TSC_CMD_EXPORT_ALIAS(__csp_ctp_set_time, csp_time_set, CSP time set.);
TSC_CMD_EXPORT_ALIAS(__csp_ctp_status, csp_time_status, CSP time status.);

#endif

#endif

// ---------- (13) END OF SOURCE FILE IMPLEMENTATION ------------------------------------------------- //
// SOURCE FORMAT REVISION B
