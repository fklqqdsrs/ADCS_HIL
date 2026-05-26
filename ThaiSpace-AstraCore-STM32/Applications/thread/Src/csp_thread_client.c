#include "app_modules_config.h"
#if CSP_ENABLE_THREAD

//! @file csp_thread_app.c
//! @brief C Source File Template

// --------------------------------------------------------------------------------------------------- //
// COMPANY NAME : IOT POW.
//
// INTRODUCTION : CSP thread application.
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
#define __THIS_FILE_NAME__		"csp_thread_app.c"

// ---------- (2) STANDARD LIBRARY INCLUDE ----------------------------------------------------------- //
#include <kservice.h>

// ---------- (3) USER INCLUDE ----------------------------------------------------------------------- //
#include <csp_thread_protocol.h>

#ifdef TSC_USING_FINSH
#include "finsh.h"


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
 * @brief Start a registered thread.
 *
 * @param argc Number of arguments
 * @param argv Argument list: node, name
 */
static void __csp_thread_start(int argc, char **argv) {
	struct csp_thread_message msg;
	int timeout = 1000, node;

	if (argc == 3) {
		node = atoi(argv[1]);
		if ((node < 0) && (node > 255))
			goto PRINT_HELP;

		if (strlen(argv[2]) == 0)
			goto PRINT_HELP;

		tsc_strncpy(msg.start.name, argv[2], CSP_THREAD_MANAGER_NAME_LENGTH);

		if (!csp_thread_start(node, timeout, &msg)) {
			tsc_kprintf("CSP response csp_thread_start node:%d thread:%s %s", node, msg.start.name, msg.start.status);
		} else {
			tsc_kprintf("CSP response csp_thread_start error \r\n");
		}
		return;
	}

	PRINT_HELP:

	tsc_kprintf("Usage: csp_thread_start <node> <name>\r\nTry 'csp_thread_list' to see names.\r\n");
	tsc_kprintf("<node> must be in 0 - 255 range.\r\n");
	tsc_kprintf("<name> thread name to start run.\r\n");
	tsc_kprintf("[timeout] must be in 0 - 5000 ms range.\r\n");

}

/**
 * @brief Stop a registered thread.
 *
 * @param argc Number of arguments
 * @param argv Argument list: node, name
 */
static void __csp_thread_stop(int argc, char **argv) {
	struct csp_thread_message msg;
	int timeout = 1000, node;

	if (argc == 3) {
		node = atoi(argv[1]);
		if ((node < 0) && (node > 255))
			goto PRINT_HELP;

		if (strlen(argv[2]) == 0)
			goto PRINT_HELP;

		tsc_strncpy(msg.stop.name, argv[2], CSP_THREAD_MANAGER_NAME_LENGTH);

		if (!csp_thread_stop(node, timeout, &msg)) {
			tsc_kprintf("CSP response csp_thread_stop node:%d thread:%s %s", node, msg.stop.name, msg.stop.status);
		} else {
			tsc_kprintf("CSP response csp_thread_stop error \r\n");
		}
		return;
	}

	PRINT_HELP:

	tsc_kprintf("Usage: csp_thread_stop <node> <name>\r\nTry 'csp_thread_list' to see names.\r\n");
	tsc_kprintf("<node> must be in 0 - 255 range.\r\n");
	tsc_kprintf("<name> thread name to stop running.\r\n");
}

/**
 * @brief Restart a registered thread.
 *
 * @param argc Number of arguments
 * @param argv Argument list: node, name
 */
static void __csp_thread_restart(int argc, char **argv) {
	struct csp_thread_message msg;
	int timeout = 1000, node;

	if (argc == 3) {
		node = atoi(argv[1]);
		if ((node < 0) && (node > 255))
			goto PRINT_HELP;

		if (strlen(argv[2]) == 0)
			goto PRINT_HELP;

		tsc_strncpy(msg.restart.name, argv[2], CSP_THREAD_MANAGER_NAME_LENGTH);

		if (!csp_thread_restart(node, timeout, &msg)) {
			tsc_kprintf("CSP response csp_thread_restart node:%d thread:%s %s", node, msg.restart.name, msg.restart.status);
		} else {
			tsc_kprintf("CSP response csp_thread_restart error \r\n");
		}
		return;
	}

	PRINT_HELP:

	tsc_kprintf("Usage: csp_thread_restart <node> <name>\r\nTry 'csp_thread_list' to see names.\r\n");
	tsc_kprintf("<node> must be in 0 - 255 range.\r\n");
	tsc_kprintf("<name> thread name to start run.\r\n");
}

/**
 * @brief Restart a registered thread.
 *
 * @param argc Number of arguments
 * @param argv Argument list: node, name
 */
static void __csp_thread_stat(int argc, char **argv) {
	struct csp_thread_message msg;
	int timeout = 1000, node;

	if (argc == 3) {
		node = atoi(argv[1]);
		if ((node < 0) && (node > 255))
			goto PRINT_HELP;

		if (strlen(argv[2]) == 0)
			goto PRINT_HELP;

		tsc_strncpy(msg.status.name, argv[2], CSP_THREAD_MANAGER_NAME_LENGTH);

		if (!csp_thread_status(node, timeout, &msg)) {
			tsc_kprintf("CSP response csp_thread_stat node:%d \r\n", node);
		    tsc_kprintf("[Thread: %s] Status:\r\n", msg.status.name);
		    tsc_kprintf("  State:         %s\n", msg.status.enabled ? "ON" : "OFF");
		    tsc_kprintf("  Retry Count:   %lu / %lu\n", (unsigned long)msg.status.retry_count, (unsigned long)msg.status.max_retry_limit);
		    tsc_kprintf("  Last Kick:     %lu ms\n", (unsigned long)msg.status.last_heartbeat);
		    tsc_kprintf("  Stack Size:    %u\n", (unsigned int)msg.status.stack_size);
		    tsc_kprintf("  Priority:      %u\n", (unsigned int)msg.status.priority);
		    tsc_kprintf("  Watchdog:      %s\n", msg.status.watchdog_enabled ? "Enabled" : "Disabled");
		    tsc_kprintf("  Timeout:       %lu ms\n", (unsigned long)msg.status.watchdog_timeout_ms);
		    tsc_kprintf("  Autorun:       %s\n", msg.status.autorun ? "Yes" : "No");
		} else {
			tsc_kprintf("CSP response csp_thread_stat error \r\n");
		}
		return;
	}

	PRINT_HELP:

	tsc_kprintf("Usage: csp_thread_stat <node> <name>\r\nTry 'csp_thread_list' to see names.\r\n");
	tsc_kprintf("<node> must be in 0 - 255 range.\r\n");
	tsc_kprintf("<name> thread name to show status.\r\n");
	tsc_kprintf("[timeout] must be in 0 - 5000 ms range.\r\n");

}

static void __csp_thread_stat_all(int argc, char **argv) {

	if (argc > 1) {
		int node = atoi(argv[1]);
		if ((node >= 0) && (node <= 255)) {
			if (argc == 2) {
				tsc_kprintf("CSP thread status all node:%d \r\n", node);
				csp_thread_status_all(node, 1000);
				return;
			}
			if (argc == 3) {
				int timeout = atoi(argv[2]);
				if ((timeout >= 0) && (timeout <= 5000)) {
					tsc_kprintf("CSP thread status all node:%d, timeout:%d \r\n", node, timeout);
					csp_thread_status_all(node, timeout);
					return;
				}
			}
		}

		tsc_kprintf("<node> must be in 0 - 255 range.\r\n");
		tsc_kprintf("[timeout] must be in 0 - 5000 ms range.\r\n");
	} else {
		tsc_kprintf("Please input: csp_thread_stat_all <node> [timeout]\r\n");
	}
}

/**
 * @brief Configure watchdog settings for a thread.
 *
 * Usage: csp_thread_config <name> <timeout_ms> <max_retries> <watchdog_enable>
 */
static void __csp_thread_config(int argc, char **argv)
{
	struct csp_thread_message msg;
	int timeout = 1000;

    if (argc != 8)
    {
    	tsc_kprintf("Usage: thread_config <node> <name> <stack> <priority> <timeout_ms> <max_retries> <wd_enable>\r\n");
        return;
    }

    uint8_t node = atoi(argv[1]);

    tsc_strncpy(msg.config.name, argv[2], CSP_THREAD_MANAGER_NAME_LENGTH);
    msg.config.stack_size = atoi(argv[3]);
    msg.config.priority = atoi(argv[4]);
    msg.config.watchdog_timeout_ms = atoi(argv[5]);
    msg.config.max_retry_limit = atoi(argv[6]);
    msg.config.watchdog_enabled = atoi(argv[7]) ? 1 : 0;

	if (!csp_thread_config(node, timeout, &msg)) {
		tsc_kprintf("CSP response csp_thread_config node:%d\r\n", node);
	    tsc_kprintf("Thread '%s' config updated:\r\n", msg.config.name);
	    tsc_kprintf("  Stack Size:       %u\r\n", (unsigned int)msg.config.stack_size);
	    tsc_kprintf("  Priority:         %u\r\n", (unsigned int)msg.config.priority);
	    tsc_kprintf("  Watchdog Timeout: %lu ms\r\n", (unsigned long)msg.config.watchdog_timeout_ms);
	    tsc_kprintf("  Retry Limit:      %u\r\n", msg.config.max_retry_limit);
	    tsc_kprintf("  Watchdog Enable:  %s\r\n", msg.config.watchdog_enabled ? "ON" : "OFF");
	} else {
		tsc_kprintf("CSP response csp_thread_config error \r\n");
	}
	return;
}

static void __csp_thread_list(int argc, char **argv) {

	if (argc > 1) {
		int node = atoi(argv[1]);
		if ((node >= 0) && (node <= 255)) {
			if (argc == 2) {
				tsc_kprintf("CSP thread list node:%d \r\n", node);
				csp_thread_list(node, 1000);
				return;
			}
			if (argc == 3) {
				int timeout = atoi(argv[2]);
				if ((timeout >= 0) && (timeout <= 5000)) {
					tsc_kprintf("CSP thread list node:%d, timeout:%d \r\n", node, timeout);
					csp_thread_list(node, timeout);
					return;
				}
			}
		}

		tsc_kprintf("<node> must be in 0 - 255 range.\r\n");
		tsc_kprintf("[timeout] must be in 0 - 5000 ms range.\r\n");
	} else {
		tsc_kprintf("Please input: csp_thread_list <node> [timeout]\r\n");
	}
}

/* -------------------------------------------------------------------------- */
/* Command Registration (CLI binding)                                         */
/* -------------------------------------------------------------------------- */
TSC_CMD_EXPORT_ALIAS(__csp_thread_start, csp_thread_start, CSP thread start.);
TSC_CMD_EXPORT_ALIAS(__csp_thread_stop, csp_thread_stop, CSP thread stop.);
TSC_CMD_EXPORT_ALIAS(__csp_thread_restart, csp_thread_restart, CSP thread restart.);
TSC_CMD_EXPORT_ALIAS(__csp_thread_stat, csp_thread_stat,  CSP thread one status.);
TSC_CMD_EXPORT_ALIAS(__csp_thread_stat_all, csp_thread_stat_all, CSP thread all status.);
TSC_CMD_EXPORT_ALIAS(__csp_thread_config, csp_thread_config, CSP thread configuration.);
TSC_CMD_EXPORT_ALIAS(__csp_thread_list, csp_thread_list, CSP thread list.);

#endif

// ---------- (13) END OF SOURCE FILE IMPLEMENTATION ------------------------------------------------- //
// SOURCE FORMAT REVISION B


#endif /* CSP_ENABLE_THREAD */
