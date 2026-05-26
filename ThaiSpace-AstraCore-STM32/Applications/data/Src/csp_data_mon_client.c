#include "app_modules_config.h"
#if CSP_ENABLE_DATA_MON

//! @file csp_data_mon_cli.c
//! @brief C Source File Template

// --------------------------------------------------------------------------------------------------- //
// COMPANY NAME : IOT POW.
//
// INTRODUCTION : CSP data monitor application.
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
#define __THIS_FILE_NAME__		"csp_data_mon_cli.c"

// ---------- (2) STANDARD LIBRARY INCLUDE ----------------------------------------------------------- //
#include <kservice.h>
#include <time.h>
#include <csp/csp.h>
#include <config_core.h>
#include <data_monitor_core.h>
#include <csp_data_mon_protocol.h>


// ---------- (3) USER INCLUDE ----------------------------------------------------------------------- //
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
 * @brief Start a registered data monitor.
 *
 * @param argc Number of arguments
 * @param argv Argument list: node, name
 */
static void __csp_data_mon_start(int argc, char **argv) {
	struct csp_data_mon_message msg;
	int timeout = 1000, node;

	if (argc == 3) {
		node = atoi(argv[1]);
		if ((node < 0) && (node > 255)) goto PRINT_HELP;

		if (strlen(argv[2]) == 0) goto PRINT_HELP;

		tsc_strncpy(msg.start.name, argv[2], CSP_DATA_MONITOR_NAME_LENGTH);

		if (!csp_data_mon_start(node, timeout, &msg)) {
			tsc_kprintf("CSP response csp_data_mon_start node:%d name:%s status:%s", node, msg.start.name, msg.start.status);
		} else {
			tsc_kprintf("CSP response csp_data_mon_start Error! node:%d name:%s status:%s", node, msg.start.name, msg.start.status);
		}
		return;
	}

	PRINT_HELP:

	tsc_kprintf("Usage: csp_data_mon_start <node> <name>\r\nTry 'csp_data_mon_list' to see names.\r\n");
	tsc_kprintf("<node> must be in 0 - 255 range.\r\n");
	tsc_kprintf("<name> data monitor name to start run.\r\n");

}

/**
 * @brief Stop a registered thread.
 *
 * @param argc Number of arguments
 * @param argv Argument list: node, name
 */
static void __csp_data_mon_stop(int argc, char **argv) {
	struct csp_data_mon_message msg;
	int timeout = 1000, node;

	if (argc == 3) {
		node = atoi(argv[1]);
		if ((node < 0) && (node > 255)) goto PRINT_HELP;

		if (strlen(argv[2]) == 0) goto PRINT_HELP;

		tsc_strncpy(msg.stop.name, argv[2], CSP_DATA_MONITOR_NAME_LENGTH);

		if (!csp_data_mon_stop(node, timeout, &msg)) {
			tsc_kprintf("CSP response csp_data_mon_start node:%d name:%s status:%s", node, msg.stop.name, msg.stop.status);
		} else {
			tsc_kprintf("CSP response csp_data_mon_start Error! node:%d name:%s status:%s", node, msg.stop.name, msg.stop.status);
		}
		return;
	}

	PRINT_HELP:

	tsc_kprintf("Usage: csp_data_mon_stop <node> <name>\r\nTry 'csp_data_mon_list' to see names.\r\n");
	tsc_kprintf("<node> must be in 0 - 255 range.\r\n");
	tsc_kprintf("<name> data monitor name to stop running.\r\n");
}

static void __csp_data_mon_list(int argc, char **argv) {

	if (argc > 1) {
		int node = atoi(argv[1]);
		if ((node >= 0) && (node <= 255)) {
			if (argc == 2) {
				tsc_kprintf("CSP data monitor list node:%d \r\n", node);
				csp_data_mon_list(node, 1000);
				return;
			}
			if (argc == 3) {
				int timeout = atoi(argv[2]);
				if ((timeout >= 0) && (timeout <= 5000)) {
					tsc_kprintf("CSP data monitor list node:%d, timeout:%d \r\n", node, timeout);
					csp_data_mon_list(node, timeout);
					return;
				}
			}
		}

		tsc_kprintf("<node> must be in 0 - 255 range.\r\n");
		tsc_kprintf("[timeout] must be in 0 - 5000 ms range.\r\n");
	} else {
		tsc_kprintf("Please input: csp_data_mon_list <node> [timeout]\r\n");
	}
}

static void __csp_data_mon_status(int argc, char **argv) {

	if (argc > 2) {
		int node = atoi(argv[1]);
		if ((node >= 0) && (node <= 255)) {
			if (argc == 3) {
				tsc_kprintf("CSP data monitor status node:%d, data monitor name:%s \r\n", node, argv[2]);
				csp_data_mon_status(node, 1000, argv[2]);
				return;
			}
			if (argc == 4) {
				int timeout = atoi(argv[3]);
				if ((timeout >= 0) && (timeout <= 5000)) {
					tsc_kprintf("CSP data monitor status node:%d, data monitor name:%s, timeout:%d \r\n", node, argv[2], timeout);
					csp_data_mon_status(node, timeout, argv[2]);
					return;
				}
			}
		}
	}

	tsc_kprintf("Please input: csp_data_mon_status <node> <name> [timeout]\r\n");
	tsc_kprintf("<node> must be in 0 - 255 range.\r\n");
	tsc_kprintf("<name> input data monitor name, Try 'csp_data_mon_list' to see names..\r\n");
	tsc_kprintf("[timeout] must be in 0 - 5000 ms range.\r\n");

}

static void __csp_data_mon_config_status(int argc, char **argv) {

	if (argc > 3) {
		int node = atoi(argv[1]);
		if ((node >= 0) && (node <= 255)) {
			if (argc == 4) {
				tsc_kprintf("CSP data monitor config status node:%d, monitor name:%s, config name:%s \r\n", node, argv[2], argv[3]);
				csp_data_mon_config_status(node, 1000, argv[2], argv[3]);
				return;
			}
			if (argc == 5) {
				int timeout = atoi(argv[4]);
				if ((timeout >= 0) && (timeout <= 5000)) {
					tsc_kprintf("CSP data monitor config status node:%d, monitor name:%s, config name:%s, timeout:%d \r\n", node, argv[2], argv[3],
							timeout);
					csp_data_mon_config_status(node, timeout, argv[2], argv[3]);
					return;
				}
			}
		}
	}

	tsc_kprintf("Please input: csp_data_mon_config_status <node> <dm_name> <dmc_name> [timeout]\r\n");
	tsc_kprintf("<node> must be in 0 - 255 range.\r\n");
	tsc_kprintf("<dm_name> input data monitor name, Try 'csp_data_mon_list' to see names..\r\n");
	tsc_kprintf("<dmc_name> input config name, Try 'csp_data_mon_status' to see names..\r\n");
	tsc_kprintf("[timeout] must be in 0 - 5000 ms range.\r\n");

}

/**
 * @brief Helper: parse type and value strings into cfg_type_t and raw bytes.
 *
 * Supported types (case-sensitive):
 *   float | f
 *   u32   | uint32
 *   u16   | uint16
 *   u8    | uint8
 *   i32   | int32
 *   i16   | int16
 *   i8    | int8
 *   bool  | b     (0/1 or true/false)
 *
 * @return 0 on success, -1 on unsupported type.
 */
static int __csp_data_mon_parse_type_value(const char *type_str, const char *val_str, cfg_type_t *out_type, uint8_t *raw, uint8_t *out_len) {
	if (!type_str || !val_str || !out_type || !raw || !out_len) return -1;

	/* float */
	if ((strcmp(type_str, "float") == 0) || (strcmp(type_str, "f") == 0)) {
		float v = (float) atof(val_str);
		*out_type = CFG_TYPE_FLOAT;
		tsc_memcpy(raw, &v, sizeof(v));
		*out_len = (uint8_t) sizeof(v);
		return 0;
	}

	/* unsigned 32-bit */
	if ((strcmp(type_str, "u32") == 0) || (strcmp(type_str, "uint32") == 0)) {
		uint32_t v = (uint32_t) strtoul(val_str, NULL, 0);
		*out_type = CFG_TYPE_UINT32;
		tsc_memcpy(raw, &v, sizeof(v));
		*out_len = (uint8_t) sizeof(v);
		return 0;
	}

	/* unsigned 16-bit */
	if ((strcmp(type_str, "u16") == 0) || (strcmp(type_str, "uint16") == 0)) {
		uint16_t v = (uint16_t) strtoul(val_str, NULL, 0);
		*out_type = CFG_TYPE_UINT16;
		tsc_memcpy(raw, &v, sizeof(v));
		*out_len = (uint8_t) sizeof(v);
		return 0;
	}

	/* unsigned 8-bit */
	if ((strcmp(type_str, "u8") == 0) || (strcmp(type_str, "uint8") == 0)) {
		uint8_t v = (uint8_t) strtoul(val_str, NULL, 0);
		*out_type = CFG_TYPE_UINT8;
		tsc_memcpy(raw, &v, sizeof(v));
		*out_len = (uint8_t) sizeof(v);
		return 0;
	}

	/* signed 32-bit */
	if ((strcmp(type_str, "i32") == 0) || (strcmp(type_str, "int32") == 0)) {
		int32_t v = (int32_t) strtol(val_str, NULL, 0);
		*out_type = CFG_TYPE_INT32;
		tsc_memcpy(raw, &v, sizeof(v));
		*out_len = (uint8_t) sizeof(v);
		return 0;
	}

	/* signed 16-bit */
	if ((strcmp(type_str, "i16") == 0) || (strcmp(type_str, "int16") == 0)) {
		int16_t v = (int16_t) strtol(val_str, NULL, 0);
		*out_type = CFG_TYPE_INT16;
		tsc_memcpy(raw, &v, sizeof(v));
		*out_len = (uint8_t) sizeof(v);
		return 0;
	}

	/* signed 8-bit */
	if ((strcmp(type_str, "i8") == 0) || (strcmp(type_str, "int8") == 0)) {
		int8_t v = (int8_t) strtol(val_str, NULL, 0);
		*out_type = CFG_TYPE_INT8;
		tsc_memcpy(raw, &v, sizeof(v));
		*out_len = (uint8_t) sizeof(v);
		return 0;
	}

	/* bool */
	if ((strcmp(type_str, "bool") == 0) || (strcmp(type_str, "b") == 0)) {
		bool v = false;
		if ((strcmp(val_str, "1") == 0) || (strcmp(val_str, "true") == 0) || (strcmp(val_str, "TRUE") == 0)) v = true;
		*out_type = CFG_TYPE_BOOL;
		tsc_memcpy(raw, &v, sizeof(v));
		*out_len = (uint8_t) sizeof(v);
		return 0;
	}

	return -1;
}

/**
 * @brief Set data monitor value (float) from remote node.
 *
 * Usage:
 *   csp_data_mon_set <node> <dm_name> <dmc_name> <type> <value>
 *
 * Example:
 *   csp_data_mon_set 1 dm_test1 t1_00 float 123.45
 *   csp_data_mon_set 1 pcu_main vsolar_in u32 750
 */
static void __csp_data_mon_set(int argc, char **argv) {
	struct csp_data_mon_message msg;
	int timeout = 1000;
	int node;

	if (argc == 6) {
		node = atoi(argv[1]);
		if ((node < 0) || (node > 255)) goto PRINT_HELP;

		if ((strlen(argv[2]) == 0) || (strlen(argv[3]) == 0)) goto PRINT_HELP;

		const char *type_str = argv[4];
		const char *val_str = argv[5];

		cfg_type_t type;
		uint8_t raw[DATA_MONITOR_MAX_DATA_BYTES];
		uint8_t len = 0;

		if (__csp_data_mon_parse_type_value(type_str, val_str, &type, raw, &len) != 0) goto PRINT_HELP;

		tsc_memset(&msg, 0, sizeof(msg));

		tsc_strncpy(msg.set_data.dm_name, argv[2], CSP_DATA_MONITOR_NAME_LENGTH);
		tsc_strncpy(msg.set_data.dmc_name, argv[3], CSP_DATA_MONITOR_NAME_LENGTH);

		msg.set_data.type = type;
		msg.set_data.len = len;
		tsc_memcpy(msg.set_data.raw, raw, len);

		if (!csp_data_mon_set_data((uint8_t) node, timeout, &msg)) {
			tsc_kprintf("CSP response csp_data_mon_set node:%d dm:%s dmc:%s status:%s", node, msg.set_data.dm_name, msg.set_data.dmc_name,
					msg.set_data.status);
		} else {
			tsc_kprintf("CSP response csp_data_mon_set Error! node:%d dm:%s dmc:%s status:%s", node, msg.set_data.dm_name, msg.set_data.dmc_name,
					msg.set_data.status);
		}
		return;
	}

	PRINT_HELP: tsc_kprintf("Usage: csp_data_mon_set <node> <dm_name> <dmc_name> <type> <value>\r\n");
	tsc_kprintf("<node> must be in 0 - 255 range.\r\n");
	tsc_kprintf("<dm_name> data monitor name, Try 'csp_data_mon_list' to see names.\r\n");
	tsc_kprintf("<dmc_name> config name, Try 'csp_data_mon_status' to see names.\r\n");
	tsc_kprintf("<type> one of: float|f, u32|uint32, u16|uint16, u8|uint8, i32|int32, i16|int16, i8|int8, bool|b.\r\n");
	tsc_kprintf("<value> numeric value (for bool use 0/1 or true/false).\r\n");
}

/* -------------------------------------------------------------------------- */
/* Command Registration (CLI binding)                                         */
/* -------------------------------------------------------------------------- */
TSC_CMD_EXPORT_ALIAS(__csp_data_mon_start, csp_data_mon_start, CSP data monitor start.);
TSC_CMD_EXPORT_ALIAS(__csp_data_mon_stop, csp_data_mon_stop, CSP data monitor stop.);
TSC_CMD_EXPORT_ALIAS(__csp_data_mon_list, csp_data_mon_list, CSP data monitor list.);
TSC_CMD_EXPORT_ALIAS(__csp_data_mon_status, csp_data_mon_status, CSP data monitor status.);
TSC_CMD_EXPORT_ALIAS(__csp_data_mon_config_status, csp_data_mon_config_status, CSP data monitor config status.);
TSC_CMD_EXPORT_ALIAS(__csp_data_mon_set, csp_data_mon_set, CSP data monitor set value.);

#endif

// ---------- (13) END OF SOURCE FILE IMPLEMENTATION ------------------------------------------------- //
// SOURCE FORMAT REVISION B


#endif /* CSP_ENABLE_DATA_MON */
