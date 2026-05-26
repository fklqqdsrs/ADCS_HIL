#include "app_modules_config.h"
#if CSP_ENABLE_LOG

//! @file csp_log_app.c
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
#define __THIS_FILE_NAME__		"csp_log_app.c"

// ---------- (2) STANDARD LIBRARY INCLUDE ----------------------------------------------------------- //
#include <kservice.h>

// ---------- (3) USER INCLUDE ----------------------------------------------------------------------- //
#define LOG_TAG		"csp_log_app"
#define LOG_LVL		DBG_LOG
#include <tsc_log_core.h>
#include <csp_log_protocol.h>

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
 * @brief Set tsc_log filter level by different backend.
 *
 * @param argc Number of arguments
 * @param argv Argument list: node, be_name, level
 */
static void __csp_log_be_level(int argc, char **argv) {
	struct csp_log_message msg;
	int timeout = 1000, node;

	if (argc > 3) {
		node = atoi(argv[1]);
		if ((node < 0) && (node > 255)) goto PRINT_HELP;

		if (strlen(argv[2]) == 0) goto PRINT_HELP;

		int level = atoi(argv[3]);
		if ((level > LOG_FILTER_LVL_ALL) && (level < 0)) {
			tsc_kprintf("Please input correct level (0-%d).\r\n", LOG_FILTER_LVL_ALL);
			goto PRINT_HELP;
		}

		tsc_strncpy(msg.be_level.be_name, argv[2], TSC_NAME_MAX);
		msg.be_level.level = level;

		if (!csp_log_be_level(node, timeout, &msg)) {
			tsc_kprintf("CSP response csp_log_be_level node:%d, name:%s, level:%d successful \r\n", node, msg.be_level.be_name, msg.be_level.level);
		} else {
			tsc_kprintf("CSP response csp_log_be_level error \r\n");
		}
		return;
	}

	PRINT_HELP:

	tsc_kprintf("Usage: csp_log_be_level <node> <name> <level>\r\nTry 'csp_log_filter' to see log filter info.\r\n");
	tsc_kprintf("<node> must be in 0 - 255 range.\r\n");
	tsc_kprintf("<name> backend name.\r\n");
	tsc_kprintf("<level> backend level.\r\n");
}

/**
 * @brief Set tsc_log filter level by different tag.
 *
 * @param argc Number of arguments
 * @param argv Argument list: node, tag_name, level
 */
static void __csp_log_tag_level(int argc, char **argv) {
	struct csp_log_message msg;
	int timeout = 1000, node;

	if (argc > 3) {
		node = atoi(argv[1]);
		if ((node < 0) && (node > 255)) goto PRINT_HELP;

		if (strlen(argv[2]) == 0) goto PRINT_HELP;

		int level = atoi(argv[3]);
		if ((level > LOG_FILTER_LVL_ALL) && (level < 0)) {
			tsc_kprintf("Please input correct level (0-%d).\r\n", LOG_FILTER_LVL_ALL);
			goto PRINT_HELP;
		}

		tsc_strncpy(msg.tag_level.tag_name, argv[2], TSC_LOG_FILTER_TAG_MAX_LEN);
		msg.tag_level.level = level;

		if (!csp_log_tag_level(node, timeout, &msg)) {
			tsc_kprintf("CSP response csp_log_tag_level node:%d, name:%s, level:%d successful \r\n", node, msg.tag_level.tag_name,
					msg.tag_level.level);
		} else {
			tsc_kprintf("CSP response csp_log_tag_level error \r\n");
		}
		return;
	}

	PRINT_HELP:

	tsc_kprintf("Usage: csp_log_tag_level <node> <name> <level>\r\nTry 'csp_log_filter' to see log filter info.\r\n");
	tsc_kprintf("<node> must be in 0 - 255 range.\r\n");
	tsc_kprintf("<name> tag name.\r\n");
	tsc_kprintf("<level> tag level.\r\n");
}

/**
 * @brief Set tsc_log global filter level.
 *
 * @param argc Number of arguments
 * @param argv Argument list: node, level
 */
static void __csp_log_level(int argc, char **argv) {
	struct csp_log_message msg;
	int timeout = 1000, node;

	if (argc > 2) {
		node = atoi(argv[1]);
		if ((node < 0) && (node > 255)) goto PRINT_HELP;

		int level = atoi(argv[2]);
		if ((level > LOG_FILTER_LVL_ALL) && (level < 0)) {
			tsc_kprintf("Please input correct level (0-%d).\r\n", LOG_FILTER_LVL_ALL);
			goto PRINT_HELP;
		}

		msg.level = level;

		if (!csp_log_level(node, timeout, &msg)) {
			tsc_kprintf("CSP response csp_log_level node:%d, level:%d successful \r\n", node, msg.level);
		} else {
			tsc_kprintf("CSP response csp_log_level error \r\n");
		}
		return;
	}

	PRINT_HELP:

	tsc_kprintf("Usage: csp_log_level <node> <level>\r\nTry 'csp_log_filter' to see log filter info.\r\n");
	tsc_kprintf("<node> must be in 0 - 255 range.\r\n");
	tsc_kprintf("<level> backend level.\r\n");
}

/**
 * @brief Set tsc_log global filter tag.
 *
 * @param argc Number of arguments
 * @param argv Argument list: node, tag_name
 */
static void __csp_log_tag(int argc, char **argv) {
	struct csp_log_message msg;
	int timeout = 1000, node;

	if (argc > 1) {
		node = atoi(argv[1]);
		if ((node < 0) && (node > 255)) goto PRINT_HELP;

		tsc_memset(msg.tag, 0, sizeof(msg.keyword));
	} else {
		goto PRINT_HELP;
	}

	if (argc > 2) {
		if (strlen(argv[2]))
		tsc_strncpy(msg.tag, argv[2], TSC_LOG_FILTER_TAG_MAX_LEN);
	}

	if (!csp_log_tag(node, timeout, &msg)) {
		tsc_kprintf("CSP response csp_log_tag node:%d, tag:%s successful \r\n", node, msg.tag);
	} else {
		tsc_kprintf("CSP response csp_log_tag error \r\n");
	}
	return;

	PRINT_HELP:

	tsc_kprintf("Usage: csp_log_tag <node> [tag]\r\nTry 'csp_log_filter' to see log filter info.\r\n");
	tsc_kprintf("<node> must be in 0 - 255 range.\r\n");
	tsc_kprintf("[tag] tag name.\r\n");
}

/**
 * @brief Set tsc_log global filter keyword.
 *
 * @param argc Number of arguments
 * @param argv Argument list: node, keyword
 */
static void __csp_log_keyword(int argc, char **argv) {
	struct csp_log_message msg;
	int timeout = 1000, node;

	if (argc > 1) {
		node = atoi(argv[1]);
		if ((node < 0) && (node > 255)) goto PRINT_HELP;

		tsc_memset(msg.keyword, 0, sizeof(msg.keyword));
	} else {
		goto PRINT_HELP;
	}

	if (argc > 2) {
		if (strlen(argv[2]))
		tsc_strncpy(msg.keyword, argv[2], TSC_LOG_FILTER_KW_MAX_LEN);
	}

	if (!csp_log_keyword(node, timeout, &msg)) {
		tsc_kprintf("CSP response csp_log_keyword node:%d, keyword:%s successful \r\n", node, msg.keyword);
	} else {
		tsc_kprintf("CSP response csp_log_keyword error \r\n");
	}
	return;

	PRINT_HELP:

	tsc_kprintf("Usage: csp_log_keyword <node> <keyword>\r\nTry 'csp_log_filter' to see log filter info.\r\n");
	tsc_kprintf("<node> must be in 0 - 255 range.\r\n");
	tsc_kprintf("<keyword> filter keyword.\r\n");
}

/**
 * @brief Show tsc_log filter settings.
 *
 * @param argc Number of arguments
 * @param argv Argument list: node
 */
static void __csp_log_filter(int argc, char **argv) {

	if (argc > 1) {
		int node = atoi(argv[1]);
		if ((node >= 0) && (node <= 255)) {
			if (argc == 2) {
				tsc_kprintf("CSP log filter node:%d \r\n", node);
				csp_log_filter(node, 1000);
				return;
			}
			if (argc == 3) {
				int timeout = atoi(argv[2]);
				if ((timeout >= 0) && (timeout <= 5000)) {
					tsc_kprintf("CSP log filter node:%d, timeout:%d \r\n", node, timeout);
					csp_log_filter(node, timeout);
					return;
				}
			}
		}

		tsc_kprintf("<node> must be in 0 - 255 range.\r\n");
		tsc_kprintf("[timeout] must be in 0 - 5000 ms range.\r\n");
	} else {
		tsc_kprintf("Please input: csp_log_filter <node> [timeout]\r\n");
	}
}

/**
 * @brief Show tsc_log test printout.
 *
 * @param argc Number of arguments
 * @param argv Argument list: node
 */
static void __csp_log_test(int argc, char **argv) {
	LOG_E("csp_log_error(...), level: %d", LOG_LVL_ERROR);
	LOG_W("csp_log_warn(...), level: %d", LOG_LVL_WARNING);
	LOG_I("csp_log_info((...), level: %d", LOG_LVL_INFO);
	LOG_D("csp_log_debug(...), level: %d", LOG_LVL_DBG);
}

/* -------------------------------------------------------------------------- */
/* Command Registration (CLI binding)                                         */
/* -------------------------------------------------------------------------- */
TSC_CMD_EXPORT_ALIAS(__csp_log_be_level, csp_log_be_level, CSP log set backend level.);
TSC_CMD_EXPORT_ALIAS(__csp_log_tag_level, csp_log_tag_level, CSP log set tag level.);
TSC_CMD_EXPORT_ALIAS(__csp_log_level, csp_log_level, CSP log set level.);
TSC_CMD_EXPORT_ALIAS(__csp_log_tag, csp_log_tag, CSP log set tag filter.);
TSC_CMD_EXPORT_ALIAS(__csp_log_keyword, csp_log_keyword, CSP log set keyword filter.);
TSC_CMD_EXPORT_ALIAS(__csp_log_filter, csp_log_filter, CSP log filter settings.);
TSC_CMD_EXPORT_ALIAS(__csp_log_test, csp_log_test, CSP log test.);

#endif

// ---------- (13) END OF SOURCE FILE IMPLEMENTATION ------------------------------------------------- //
// SOURCE FORMAT REVISION B


#endif /* CSP_ENABLE_LOG */
