#include "app_modules_config.h"
#if CSP_ENABLE_CONFIG

//! @file csp_config_app.c
//! @brief C Source File Template

// --------------------------------------------------------------------------------------------------- //
// COMPANY NAME : IOT POW.
//
// INTRODUCTION : CSP config application.
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
#define __THIS_FILE_NAME__		"csp_config_cli_client.c"

// ---------- (2) STANDARD LIBRARY INCLUDE ----------------------------------------------------------- //
#include "app_modules_config.h"
#include <kservice.h>

// ---------- (3) USER INCLUDE ----------------------------------------------------------------------- //
#include <csp_config_protocol.h>

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

// ---------- (10) PRIVATE DATA ----------------------------------------------------------------------- //
// N/A

// ---------- (11) PRIVATE MACRO DEFINITION ---------------------------------------------------------- //
// N/A

// ---------- (12) SOURCE FILE IMPLEMENTATION -------------------------------------------------------- //

/**
 * @brief CSP Load config from flash.
 *
 * @param argc Number of arguments
 * @param argv Argument list: node
 */
static void __csp_config_load(int argc, char **argv) {
	struct csp_ccp_message msg;
	int timeout = 1000, node;

	if (argc == 2) {
		node = atoi(argv[1]);
		if ((node < 0) && (node > 255)) goto PRINT_HELP;

		if (!csp_ccp_load(node, timeout, &msg)) {
			tsc_kprintf("CSP config loaded from flash");
			if (msg.result) tsc_kprintf(" ERROR!, code [%d].\r\n", msg.result);
			else tsc_kprintf(" successful.\r\n");
		} else {
			tsc_kprintf("CSP failed to load config.\r\n");
		}
		return;
	}

	PRINT_HELP:

	tsc_kprintf("Usage: csp_config_load <node> \r\n");
	tsc_kprintf("<node> must be in 0 - 255 range.\r\n");

}

/**
 * @brief CSP Save config from flash.
 *
 * @param argc Number of arguments
 * @param argv Argument list: node
 */
static void __csp_config_save(int argc, char **argv) {
	struct csp_ccp_message msg;
	int timeout = 1000, node;

	if (argc == 2) {
		node = atoi(argv[1]);
		if ((node < 0) && (node > 255)) goto PRINT_HELP;

		if (!csp_ccp_save(node, timeout, &msg)) {
			tsc_kprintf("CSP config saved to flash");
			if (msg.result) tsc_kprintf(" ERROR!, code [%d].\r\n", msg.result);
			else tsc_kprintf(" successful.\r\n");
		} else {
			tsc_kprintf("CSP failed to save config.\r\n");
		}
		return;
	}

	PRINT_HELP:

	tsc_kprintf("Usage: csp_config_save <node> \r\n");
	tsc_kprintf("<node> must be in 0 - 255 range.\r\n");

}

/**
 * @brief CSP Load all config values to default.
 *
 * @param argc Number of arguments
 * @param argv Argument list: node, save
 */
static void __csp_config_default(int argc, char **argv) {
	struct csp_ccp_message msg;
	int timeout = 1000, node;

	if (argc == 3) {
		node = atoi(argv[1]);
		if ((node < 0) && (node > 255)) goto PRINT_HELP;

		msg.load_default = (strcmp(argv[2], "true") == 0 || strcmp(argv[2], "1") == 0);

		if (!csp_ccp_load_default(node, timeout, &msg)) {
			tsc_kprintf("CSP config load default value");
			if (msg.result) tsc_kprintf(" ERROR!, code [%d].\r\n", msg.result);
			else tsc_kprintf(" successful.\r\n");
		} else {
			tsc_kprintf("CSP failed to load default config.\r\n");
		}
		return;
	}

	PRINT_HELP:

	tsc_kprintf("Usage: csp_config_default <node> [save] \r\n");
	tsc_kprintf("<node> must be in 0 - 255 range.\r\n");
	tsc_kprintf("[save] true or 1 to save data after load default value.\r\n");

}

/**
 * @brief CSP Get config.
 *
 * @param argc Number of arguments
 * @param argv Argument list: node, name
 */
#if ENABLE_LITTLEFS
static void __csp_config_get(int argc, char **argv) {
	struct csp_ccp_message msg;
	int timeout = 1000, node;

	if (argc == 3) {
		node = atoi(argv[1]);
		if ((node < 0) && (node > 255)) goto PRINT_HELP;

		tsc_strncpy(msg.get_config.name, argv[2], CONFIG_NAME_MAX_LEN);

		if (!csp_ccp_get_config(node, timeout, &msg)) {
			tsc_kprintf("CSP config name: [%s] get", argv[2]);
			if (msg.result) tsc_kprintf(" ERROR!, code [%d].\r\n", msg.result);
			else {
				tsc_kprintf(" successful.\r\n");
				config_entry_t *e = tsc_malloc(sizeof(config_entry_t));
				tsc_memset(e, 0, sizeof(config_entry_t));
				config_bin_to_entry(&msg.get_config, e, true);
				if (e->type == CFG_TYPE_STRING) {
					csp_ccp_get_string(node, timeout, msg.get_config.name, e->value, NULL);
					csp_ccp_get_default_string(node, timeout, msg.get_config.name, (char*) e->default_value, NULL);
				}
				config_print_entry(e);
				config_free_entry(e);
			}
		} else {
			tsc_kprintf("CSP failed to get config name: [%s].\r\n", argv[2]);
		}
		return;
	}

	PRINT_HELP:

	tsc_kprintf("Usage: __csp_config_get <node> <name> \r\n");
	tsc_kprintf("<node> must be in 0 - 255 range.\r\n");
	tsc_kprintf("<name> config name to get.\r\n");
}
#endif

/**
 * @brief CSP Remove config.
 *
 * @param argc Number of arguments
 * @param argv Argument list: node, name
 */
static void __csp_config_remove(int argc, char **argv) {
	struct csp_ccp_message msg;
	int timeout = 1000, node;

	if (argc == 3) {
		node = atoi(argv[1]);
		if ((node < 0) && (node > 255)) goto PRINT_HELP;

		tsc_strncpy(msg.remove_config.name, argv[2], CONFIG_NAME_MAX_LEN);

		if (!csp_ccp_remove_config(node, timeout, &msg)) {
			tsc_kprintf("CSP config name: [%s] remove", argv[2]);
			if (msg.result) tsc_kprintf(" ERROR!, code [%d].\r\n", msg.result);
			else tsc_kprintf(" successful.\r\n");
		} else {
			tsc_kprintf("CSP failed to remove config name: [%s].\r\n", argv[2]);
		}
		return;
	}

	PRINT_HELP:

	tsc_kprintf("Usage: csp_config_remove <node> <name> \r\n");
	tsc_kprintf("<node> must be in 0 - 255 range.\r\n");
	tsc_kprintf("<name> config name to remove.\r\n");
}

/**
 * @brief CSP Edit config value.
 *
 * @param argc Number of arguments
 * @param argv Argument list: node, name, type, value
 */
static void __csp_config_edit_val(int argc, char **argv) {
	struct csp_ccp_message msg;
	int timeout = 1000, node;

	if (argc == 5) {
		node = atoi(argv[1]);
		if ((node < 0) && (node > 255)) goto PRINT_HELP;

		tsc_strncpy(msg.edit_value.name, argv[2], CONFIG_NAME_MAX_LEN);
		msg.edit_value.type = atoi(argv[3]);

		void *val = tsc_malloc(config_type_size(msg.edit_value.type));
		config_str_set_entry_val(val, msg.edit_value.type, argv[4]);
		msg.edit_value.value = config_set_entry_bin_val(NULL, msg.edit_value.type, val);
		tsc_free(val);

		if (!csp_ccp_edit_value(node, timeout, &msg)) {
			tsc_kprintf("CSP config name: [%s] edit", argv[2]);
			if (msg.result) tsc_kprintf(" ERROR!, code [%d].\r\n", msg.result);
			else tsc_kprintf(" successful.\r\n");
		} else {
			tsc_kprintf("CSP failed to edit config name: [%s].\r\n", argv[2]);
		}
		return;
	}

	PRINT_HELP:

	tsc_kprintf("Usage: csp_config_edit_val <node> <name> <type> <value>\r\n");
	tsc_kprintf("<node> must be in 0 - 255 range.\r\n");
	tsc_kprintf("<name> config name to edit.\r\n");
	tsc_kprintf("<type> config value type.\r\n");
	tsc_kprintf("<value> config new value.\r\n");
}

/**
 * @brief CSP Edit config minimum value.
 *
 * @param argc Number of arguments
 * @param argv Argument list: node, name, type, min
 */
static void __csp_config_edit_min(int argc, char **argv) {
	struct csp_ccp_message msg;
	int timeout = 1000, node;

	if (argc == 5) {
		node = atoi(argv[1]);
		if ((node < 0) && (node > 255)) goto PRINT_HELP;

		tsc_strncpy(msg.edit_min.name, argv[2], CONFIG_NAME_MAX_LEN);
		msg.edit_min.type = atoi(argv[3]);

		void *val = tsc_malloc(config_type_size(msg.edit_min.type));
		config_str_set_entry_val(val, msg.edit_min.type, argv[4]);
		msg.edit_min.range.min = config_set_entry_bin_val(NULL, msg.edit_value.type, val);
		tsc_free(val);

		if (!csp_ccp_edit_min(node, timeout, &msg)) {
			tsc_kprintf("CSP config name: [%s] edit minimum", argv[2]);
			if (msg.result) tsc_kprintf(" ERROR!, code [%d].\r\n", msg.result);
			else tsc_kprintf(" successful.\r\n");
		} else {
			tsc_kprintf("CSP failed to edit config name: [%s].\r\n", argv[2]);
		}
		return;
	}

	PRINT_HELP:

	tsc_kprintf("Usage: csp_config_edit_min <node> <name> <type> <min>\r\n");
	tsc_kprintf("<node> must be in 0 - 255 range.\r\n");
	tsc_kprintf("<name> config name to edit.\r\n");
	tsc_kprintf("<type> config value type.\r\n");
	tsc_kprintf("<min> config new minimum value.\r\n");
}

/**
 * @brief CSP Edit config maximum value.
 *
 * @param argc Number of arguments
 * @param argv Argument list: node, name, type, max
 */
static void __csp_config_edit_max(int argc, char **argv) {
	struct csp_ccp_message msg;
	int timeout = 1000, node;

	if (argc == 5) {
		node = atoi(argv[1]);
		if ((node < 0) && (node > 255)) goto PRINT_HELP;

		tsc_strncpy(msg.edit_max.name, argv[2], CONFIG_NAME_MAX_LEN);
		msg.edit_max.type = atoi(argv[3]);

		void *val = tsc_malloc(config_type_size(msg.edit_max.type));
		config_str_set_entry_val(val, msg.edit_max.type, argv[4]);
		msg.edit_max.range.max = config_set_entry_bin_val(NULL, msg.edit_value.type, val);
		tsc_free(val);

		if (!csp_ccp_edit_max(node, timeout, &msg)) {
			tsc_kprintf("CSP config name: [%s] edit maximum", argv[2]);
			if (msg.result) tsc_kprintf(" ERROR!, code [%d].\r\n", msg.result);
			else tsc_kprintf(" successful.\r\n");
		} else {
			tsc_kprintf("CSP failed to edit config name: [%s].\r\n", argv[2]);
		}
		return;
	}

	PRINT_HELP:

	tsc_kprintf("Usage: csp_config_edit_max <node> <name> <type> <max>\r\n");
	tsc_kprintf("<node> must be in 0 - 255 range.\r\n");
	tsc_kprintf("<name> config name to edit.\r\n");
	tsc_kprintf("<type> config value type.\r\n");
	tsc_kprintf("<max> config new maximum value.\r\n");
}

static void __csp_config_list(int argc, char **argv) {

	if (argc > 1) {
		int node = atoi(argv[1]);
		if ((node >= 0) && (node <= 255)) {
			if (argc == 2) {
				tsc_kprintf("CSP config list node:%d \r\n", node);
				csp_ccp_list(node, 1000);
				return;
			}
			if (argc == 3) {
				int timeout = atoi(argv[2]);
				if ((timeout >= 0) && (timeout <= 5000)) {
					tsc_kprintf("CSP config list node:%d, timeout:%d \r\n", node, timeout);
					csp_ccp_list(node, timeout);
					return;
				}
			}
		}

		tsc_kprintf("<node> must be in 0 - 255 range.\r\n");
		tsc_kprintf("[timeout] must be in 0 - 5000 ms range.\r\n");
	} else {
		tsc_kprintf("Please input: csp_config_list <node> [timeout]\r\n");
	}
}

/**
 * @brief CSP Get config access level.
 *
 * @param argc Number of arguments
 * @param argv Argument list: node
 */
static void __csp_config_get_access(int argc, char **argv) {
	struct csp_ccp_message msg;
	int timeout = 1000, node;

	if (argc == 2) {
		node = atoi(argv[1]);
		if ((node < 0) && (node > 255)) goto PRINT_HELP;

		if (!csp_ccp_get_access(node, timeout, &msg)) {
			tsc_kprintf("CSP get access level");
			if (msg.result) tsc_kprintf(" ERROR!, code [%d].\r\n", msg.result);
			else {
				tsc_kprintf(" successful.\r\n");
				tsc_kprintf("User: %s, Access Level: %d\r\n", msg.get_access.username, msg.get_access.access_level);
			}
		} else {
			tsc_kprintf("CSP failed to get access level.\r\n");
		}
		return;
	}

	PRINT_HELP:

	tsc_kprintf("Usage: csp_config_get_access <node>\r\n");
	tsc_kprintf("<node> must be in 0 - 255 range.\r\n");
}

/**
 * @brief CSP Config login.
 *
 * @param argc Number of arguments
 * @param argv Argument list: node, user, pass
 */
static void __csp_config_login(int argc, char **argv) {
	struct csp_ccp_message msg;
	int timeout = 1000, node;

	if (argc == 4) {
		node = atoi(argv[1]);
		if ((node < 0) && (node > 255)) goto PRINT_HELP;

		tsc_strncpy(msg.login_access.username, argv[2], CONFIG_NAME_MAX_LEN);
		tsc_strncpy(msg.login_access.password, argv[3], CONFIG_NAME_MAX_LEN);

		if (!csp_ccp_login_access(node, timeout, &msg)) {
			tsc_kprintf("CSP config login");
			if (msg.result) tsc_kprintf(" ERROR!, code [%d].\r\n", msg.result);
			else {
				tsc_kprintf(" successful.\r\n");
				tsc_kprintf("Result: [%s]\r\n", msg.login_access.result);
				tsc_kprintf("User: %s, Access Level: %d\r\n", msg.login_access.username, msg.login_access.access_level);
			}
		} else {
			tsc_kprintf("CSP failed to login.\r\n");
		}
		return;
	}

	PRINT_HELP:

	tsc_kprintf("Usage: csp_config_login <node> <user> <pass>\r\n");
	tsc_kprintf("<node> must be in 0 - 255 range.\r\n");
	tsc_kprintf("<user> Config login username.\r\n");
	tsc_kprintf("<pass> Config login password.\r\n");
}

/**
 * @brief CSP Config logout.
 *
 * @param argc Number of arguments
 * @param argv Argument list: node
 */
static void __csp_config_logout(int argc, char **argv) {
	struct csp_ccp_message msg;
	int timeout = 1000, node;

	if (argc == 2) {
		node = atoi(argv[1]);
		if ((node < 0) && (node > 255)) goto PRINT_HELP;

		if (!csp_ccp_logout_access(node, timeout, &msg)) {
			tsc_kprintf("CSP config logout");
			if (msg.result) tsc_kprintf(" ERROR!, code [%d].\r\n", msg.result);
			else {
				tsc_kprintf(" successful.\r\n");
				tsc_kprintf("Result: [%s]\r\n", msg.login_access.result);
				tsc_kprintf("User: %s, Access Level: %d\r\n", msg.login_access.username, msg.login_access.access_level);
			}
		} else {
			tsc_kprintf("CSP failed to logout.\r\n");
		}
		return;
	}

	PRINT_HELP:

	tsc_kprintf("Usage: csp_config_login <node> <user> <pass>\r\n");
	tsc_kprintf("<node> must be in 0 - 255 range.\r\n");
}

/**
 * @brief CSP Get string.
 *
 * @param argc Number of arguments
 * @param argv Argument list: node, name
 */
static void __csp_config_get_string(int argc, char **argv) {
	int timeout = 1000, node, str_total_len;
	char str_buff[CONFIG_STRING_SLOT_SIZE];

	if (argc == 3) {
		node = atoi(argv[1]);
		if ((node < 0) && (node > 255)) goto PRINT_HELP;

		int error = csp_ccp_get_string(node, timeout, (char*) argv[2], str_buff, &str_total_len);

		if (!error) {
			tsc_kprintf("CSP config get string name: [%s] successful.\r\n", argv[2]);
			tsc_kprintf("Length: %d\r\nString: %s\r\n", str_total_len, str_buff);
		} else {
			tsc_kprintf("CSP config failed to get string. Error code: [%i] \r\n", error);
		}
		return;
	}

	PRINT_HELP:

	tsc_kprintf("Usage: csp_config_get_string <node> <name> \r\n");
	tsc_kprintf("<node> must be in 0 - 255 range.\r\n");
	tsc_kprintf("<name> config string name to get.\r\n");
}

/**
 * @brief CSP Edit config string.
 *
 * @param argc Number of arguments
 * @param argv Argument list: node, name, value
 */
static void __csp_config_edit_string(int argc, char **argv) {
	int timeout = 1000, node;

	if (argc == 4) {
		node = atoi(argv[1]);
		if ((node < 0) && (node > 255)) goto PRINT_HELP;

		int str_len = strlen(argv[3]);
		char *new_str = tsc_malloc(CONFIG_STRING_SLOT_SIZE);
		tsc_strncpy(new_str, argv[3], CONFIG_STRING_SLOT_SIZE);
		new_str[str_len++] = 0;

		int error = csp_ccp_edit_string(node, timeout, (char*) argv[2], new_str, str_len);

		tsc_free(new_str);

		if (!error) {
			tsc_kprintf("CSP config edit string name: [%s] successful.\r\n", argv[2]);
			tsc_kprintf("Length: %d\r\nString: %s\r\n", str_len, argv[3]);
		} else {
			tsc_kprintf("CSP config failed to edit string. Error code: [%i] \r\n", error);
		}
		return;
	}

	PRINT_HELP:

	tsc_kprintf("Usage: csp_config_edit_string <node> <name> <value>\r\n");
	tsc_kprintf("<node> must be in 0 - 255 range.\r\n");
	tsc_kprintf("<name> config name to edit.\r\n");
	tsc_kprintf("<value> config new value.\r\n");
}

/**
 * @brief CSP new config number.
 *
 * @param argc Number of arguments
 * @param argv Argument list: node, name, type, value
 */
#if ENABLE_LITTLEFS
static void __csp_config_new_number(int argc, char **argv) {
	struct csp_ccp_message msg;
	int timeout = 1000, node;

	if (argc == 5) {
		node = atoi(argv[1]);
		if ((node < 0) && (node > 255)) goto PRINT_HELP;

		char *name = argv[2];
		cfg_type_t type = atoi(argv[3]);
		void *value_str = argv[4];
		void *vaule_num = tsc_malloc(config_type_size(type));
		config_str_set_entry_val(vaule_num, type, value_str);
		config_entry_t *e_num = config_new_number(name, 0, type, vaule_num);
		tsc_free(vaule_num);

		if (!config_entry_to_bin(e_num, &msg.new_config)) {
			tsc_kprintf("CSP config new error to convert entry: %s\r\n", name);
			return;
		}

		config_free_entry(e_num);

		if (!csp_ccp_new_config(node, timeout, &msg)) {
			tsc_kprintf("CSP create new config name: [%s]", argv[1]);
			if (msg.result) tsc_kprintf(" ERROR!, code [%d].\r\n", msg.result);
			else tsc_kprintf(" successful.\r\n");

		} else {
			tsc_kprintf("CSP failed to create new config name: [%s].\r\n", argv[1]);
		}
		return;
	}

	PRINT_HELP:

	tsc_kprintf("Usage: csp_config_new_number <node> <name> <type> <value>\r\n");
	tsc_kprintf("<node> must be in 0 - 255 range.\r\n");
	tsc_kprintf("<name> config name to create.\r\n");
	tsc_kprintf("<type> config type to create.\r\n");
	tsc_kprintf("<value> config value to create.\r\n");
}

/**
 * @brief CSP new config number with range.
 *
 * @param argc Number of arguments
 * @param argv Argument list: node, name, type, value, min, max
 */
static void __csp_config_new_range(int argc, char **argv) {
	struct csp_ccp_message msg;
	int timeout = 1000, node;

	if (argc == 7) {
		node = atoi(argv[1]);
		if ((node < 0) && (node > 255)) goto PRINT_HELP;

		char *name = argv[2];
		cfg_type_t type = atoi(argv[3]);
		void *value_str = argv[4];
		void *vaule_num = tsc_malloc(config_type_size(type));
		config_str_set_entry_val(vaule_num, type, value_str);
		void *min_str = argv[5];
		void *min_num = tsc_malloc(config_type_size(type));
		config_str_set_entry_val(min_num, type, min_str);
		void *max_str = argv[6];
		void *max_num = tsc_malloc(config_type_size(type));
		config_str_set_entry_val(max_num, type, max_str);
		config_entry_t *e_range = config_new_range(name, 0, type, vaule_num, min_num, max_num);
		tsc_free(vaule_num);
		tsc_free(min_num);
		tsc_free(max_num);

		if (!config_entry_to_bin(e_range, &msg.new_config)) {
			tsc_kprintf("CSP config new error to convert entry: %s\r\n", name);
			return;
		}

		config_free_entry(e_range);

		if (!csp_ccp_new_config(node, timeout, &msg)) {
			tsc_kprintf("CSP create new config name: [%s]", argv[1]);
			if (msg.result) tsc_kprintf(" ERROR!, code [%d].\r\n", msg.result);
			else tsc_kprintf(" successful.\r\n");
		} else {
			tsc_kprintf("CSP failed to create new config name: [%s].\r\n", argv[1]);
		}
		return;
	}

	PRINT_HELP:

	tsc_kprintf("Usage: csp_config_new_range <node> <name> <type> <value> <min> <max>\r\n");
	tsc_kprintf("<node> must be in 0 - 255 range.\r\n");
	tsc_kprintf("<name> config name to create.\r\n");
	tsc_kprintf("<type> config type to create.\r\n");
	tsc_kprintf("<value> config value to create.\r\n");
	tsc_kprintf("<min> config min value to create.\r\n");
	tsc_kprintf("<max> config max value to create.\r\n");
}

/**
 * @brief CSP new config string.
 *
 * @param argc Number of arguments
 * @param argv Argument list: node, name, type, value
 */
static void __csp_config_new_string(int argc, char **argv) {
	struct csp_ccp_message msg;
	int timeout = 1000, node;

	if (argc == 4) {
		node = atoi(argv[1]);
		if ((node < 0) && (node > 255)) goto PRINT_HELP;

		char *name = argv[2];
		void *value = argv[3];
		config_entry_t *e_str = config_new_string(name, 0, value);

		if (!config_entry_to_bin(e_str, &msg.new_config)) {
			tsc_kprintf("CSP config new error to convert entry: %s\r\n", name);
			return;
		}

		config_free_entry(e_str);

		if (!csp_ccp_new_config(node, timeout, &msg)) {
			tsc_kprintf("CSP create new config name: [%s]", argv[1]);
			if (msg.result) tsc_kprintf(" ERROR!, code [%d].\r\n", msg.result);
			else tsc_kprintf(" successful.\r\n");
		} else {
			tsc_kprintf("CSP failed to create new config name: [%s].", argv[1]);
		}

		__csp_config_edit_string(argc, argv);

		return;
	}

	PRINT_HELP:

	tsc_kprintf("Usage: csp_config_new_string <node> <name> <value>\r\n");
	tsc_kprintf("<node> must be in 0 - 255 range.\r\n");
	tsc_kprintf("<name> config name to create.\r\n");
	tsc_kprintf("<value> config string value to create.\r\n");
}
#endif

/* -------------------------------------------------------------------------- */
/* Command Registration (CLI binding)                                         */
/* -------------------------------------------------------------------------- */
TSC_CMD_EXPORT_ALIAS(__csp_config_load, csp_config_load, CSP config load.);
TSC_CMD_EXPORT_ALIAS(__csp_config_save, csp_config_save, CSP config save.);
TSC_CMD_EXPORT_ALIAS(__csp_config_default, csp_config_default, CSP config load default.);

#if ENABLE_LITTLEFS
TSC_CMD_EXPORT_ALIAS(__csp_config_get, csp_config_get, CSP config get.);
#endif
TSC_CMD_EXPORT_ALIAS(__csp_config_remove, csp_config_remove, CSP config remove.);
TSC_CMD_EXPORT_ALIAS(__csp_config_edit_val, csp_config_edit_val, CSP config edit value.);
TSC_CMD_EXPORT_ALIAS(__csp_config_edit_min, csp_config_edit_min, CSP config edit minimum value.);
TSC_CMD_EXPORT_ALIAS(__csp_config_edit_max, csp_config_edit_max, CSP config edit maximum value.);
TSC_CMD_EXPORT_ALIAS(__csp_config_list, csp_config_list, CSP config list.);

TSC_CMD_EXPORT_ALIAS(__csp_config_get_access, csp_config_get_access, CSP config get access level.);
TSC_CMD_EXPORT_ALIAS(__csp_config_login, csp_config_login, CSP config login.);
TSC_CMD_EXPORT_ALIAS(__csp_config_logout, csp_config_logout, CSP config logout.);

TSC_CMD_EXPORT_ALIAS(__csp_config_get_string, csp_config_get_string, CSP config get string.);
TSC_CMD_EXPORT_ALIAS(__csp_config_edit_string, csp_config_edit_string, CSP config edit string.);

#if ENABLE_LITTLEFS
TSC_CMD_EXPORT_ALIAS(__csp_config_new_number, csp_config_new_number, CSP config new number.);
TSC_CMD_EXPORT_ALIAS(__csp_config_new_range, csp_config_new_range, CSP config new range.);
TSC_CMD_EXPORT_ALIAS(__csp_config_new_string, csp_config_new_string, CSP config new string.);
#endif
#endif
// ---------- (13) END OF SOURCE FILE IMPLEMENTATION ------------------------------------------------- //
// SOURCE FORMAT REVISION B


#endif /* CSP_ENABLE_CONFIG */
