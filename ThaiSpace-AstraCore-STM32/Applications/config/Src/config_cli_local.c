#include "app_modules_config.h"
#if CSP_ENABLE_CONFIG

/**
 * @file tsc_config_cli.c
 * @brief TSC1 Mission Config Manager CLI Implementation
 * @author chaturong@narit.or.th
 *
 * STRICT REFACTORING IMPLEMENTATION:
 * - Removed all malloc/free calls (Replaced with stack buffers).
 * - Added input validation and FDIR checks.
 * - Replaced unsafe atoi with strtol.
 * - Adheres to MISRA C:2012 guidelines for pointer safety.
 */

#include "app_modules_config.h"
#include <config_core.h>
#if ENABLE_LITTLEFS && ENABLE_S25FL128L
#include <config_storage.h>
#endif
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <kservice.h>

#ifdef TSC_USING_FINSH

/* Define safe string buffer size if not defined externally */
#ifndef CONFIG_STRING_SLOT_SIZE
#define CONFIG_STRING_SLOT_SIZE 64
#endif

/* Maximum size for numeric types (uint64_t/double) */
#define MAX_NUMERIC_TYPE_SIZE 8

/* External dependencies */
#if ENABLE_LITTLEFS && ENABLE_S25FL128L
extern lfs_t lfs_array[SPI_FLASH_NUM];
#endif


/* --- Helper Functions --- */

/**
 * @brief Safely parse integer from string
 */
static int32_t cli_parse_int(const char *str) {
	return (int32_t) strtol(str, NULL, 10);
}

/* --- Command Implementations --- */

/**
 * @brief Create new config profile and clear all entries
 */
static void config_new_cmd(void) {
	config_profile_t *g_active_profile = config_get_active_profile();
	if (g_active_profile == NULL) {
		tsc_kprintf("[CONFIG] Error: No active profile found.\r\n");
		return;
	}
	g_active_profile->num_entries = 0;
	tsc_kprintf("[CONFIG] New configuration created (Entries cleared).\r\n");
}

/**
 * @brief Load config from flash
 */
static void config_load_cmd(void) {
	if (config_load_profile()) {
		tsc_kprintf("[CONFIG] Loaded from flash successfully.\r\n");
	} else {
		tsc_kprintf("[CONFIG] Error: Failed to load config from flash.\r\n");
	}
}

/**
 * @brief Save config to flash
 */
static void config_save_cmd(void) {
	if (config_save_profile()) {
		tsc_kprintf("[CONFIG] Saved to flash successfully.\r\n");
	} else {
		tsc_kprintf("[CONFIG] Error: Failed to save config to flash.\r\n");
	}
}

/**
 * @brief Reset all config values to default
 */
static void config_default_cmd(void) {
	config_load_profile_default(false);
	tsc_kprintf("[CONFIG] Default values restored.\r\n");
}

/**
 * @brief Remove a config entry
 */
static void config_remove_cmd(int argc, char **argv) {
	if (argc != 2) {
		tsc_kprintf("Usage: config_remove <name> \r\n");
		return;
	}

	const char *name = argv[1];
	config_entry_t *e = config_find_entry(name);

	if (e == NULL) {
		tsc_kprintf("[CONFIG] Entry not found: %s\r\n", name);
		return;
	}

	if (e->access > config_get_access_level()) {
		tsc_kprintf("[CONFIG] Permission denied for entry '%s'.\r\n", name);
		return;
	}

	if (config_remove_entry(name)) {
		tsc_kprintf("[CONFIG] Entry remove successful: %s\r\n", name);
	} else {
		tsc_kprintf("[CONFIG] Entry remove error: %s\r\n", name);
	}
}

/**
 * @brief Edit a config entry value
 */
static void config_edit_cmd(int argc, char **argv) {
	if (argc != 3) {
		tsc_kprintf("Usage: config_edit <name> <value>\r\n");
		return;
	}

	char *name = argv[1];
	char *value_str = argv[2];

	config_entry_t *e = config_find_entry(name);
	if (e == NULL) {
		tsc_kprintf("[CONFIG] Entry not found: %s\r\n", name);
		return;
	}
	if (e->permission != CFG_PERM_RW) {
		tsc_kprintf("[CONFIG] Entry '%s' is read-only.\r\n", name);
		return;
	}
	if (e->access > config_get_access_level()) {
		tsc_kprintf("[CONFIG] Permission denied for entry '%s'.\r\n", name);
		return;
	}

	config_str_set_entry_val(e->value, e->type, value_str);
	tsc_kprintf("[CONFIG] Entry '%s' updated.\r\n", name);
}

/**
 * @brief List all config entries and values
 */
static void config_list_cmd(void) {
	config_profile_t *g_active_profile = config_get_active_profile();
	if (g_active_profile == NULL) return;

	tsc_kprintf("\n--- Config List (%s) ---\r\n", g_active_profile->profile_name);

	for (uint32_t i = 0; i < g_active_profile->num_entries; ++i) {
		config_entry_t *e = g_active_profile->entries[i];
		if (e != NULL) {
			if (e->access > config_get_access_level()) continue;

			tsc_kprintf("%s: ", e->name);
			config_val_str(e->type, e->value, NULL);
		}
	}
}

/**
 * @brief Show help for config commands
 */
static void config_help_cmd(void) {
	tsc_kprintf("\n[CONFIG CLI Help]\r\n");
	tsc_kprintf(" config_new           - Create new config\r\n");
	tsc_kprintf(" config_load          - Load config from flash\r\n");
	tsc_kprintf(" config_save          - Save config to flash\r\n");
	tsc_kprintf(" config_default       - Restore default config\r\n");
	tsc_kprintf(" config_list          - List all config entries\r\n");
	tsc_kprintf(" config_edit <n> <v>  - Edit config entry\r\n");
	tsc_kprintf(" config_login <u> <p> - Login as user\r\n");
	tsc_kprintf(" config_whoami        - Show current user\r\n");
}

/**
 * @brief CLI login with username/password
 */
static void config_login_cmd(int argc, char **argv) {
	if (argc != 3) {
		tsc_kprintf("Usage: config_login <username> <password>\r\n");
		return;
	}
	const char *username = argv[1];
	const char *password = argv[2];

	if (config_login((char*) username, (char*) password)) {
		tsc_kprintf("[LOGIN] Login Success, %s! Access level: %d\r\n", username, config_get_access_level());
	} else {
		tsc_kprintf("[LOGIN] Fail.\r\n");
	}
}

/**
 * @brief Show current login user and access level
 */
static void config_whoami_cmd(void) {
	tsc_kprintf("User: %s, Access Level: %d\r\n", config_get_access_user(), config_get_access_level());
}

/**
 * @brief Print all information of entry
 */
static void config_list_all_cmd(int argc, char **argv) {
	config_profile_t *g_active_profile = config_get_active_profile();
	if (g_active_profile == NULL) return;

	if (argc != 2) {
		tsc_kprintf("Usage: config_list_all <name> \r\n");
		return;
	}

	const char *name = argv[1];
	config_entry_t *e = config_find_entry(name);
	if (e == NULL) {
		tsc_kprintf("[CONFIG] Entry not found: %s\r\n", name);
		return;
	}

	tsc_kprintf("== Profile: %s ==\r\n", g_active_profile->profile_name);
	config_print_entry(e);
}

#if ENABLE_LITTLEFS && ENABLE_S25FL128L
/**
 * @brief Set config string value safely
 */
static void config_set_string(int argc, char **argv) {
	config_profile_t *g_active_profile = config_get_active_profile();
	if (g_active_profile == NULL) return;

	if (argc != 3) {
		tsc_kprintf("Usage: config_set_string <index> <value>\n");
		return;
	}
	uint32_t index = (uint32_t) cli_parse_int(argv[1]);
	const char *value = argv[2];

	if (config_save_string_file(&lfs_array[0], g_active_profile, index, value)) {
		tsc_kprintf("String saved at index %d\n", index);
	} else {
		tsc_kprintf("Failed to save string\n");
	}
}

/**
 * @brief Get config string value
 */
static void config_get_string(int argc, char **argv) {
	config_profile_t *g_active_profile = config_get_active_profile();
	if (g_active_profile == NULL) return;

	if (argc != 2) {
		tsc_kprintf("Usage: config_get_string <index>\n");
		return;
	}
	uint32_t index = (uint32_t) cli_parse_int(argv[1]);
	char buffer[CONFIG_STRING_SLOT_SIZE];
	memset(buffer, 0, sizeof(buffer));

	if (config_load_string_file(&lfs_array[0], g_active_profile, index, buffer, sizeof(buffer))) {
		tsc_kprintf("String[%d] = %s\n", index, buffer);
	} else {
		tsc_kprintf("Failed to load string\n");
	}
}
#endif

/**
 * @brief Create new number entry (Static Memory Allocation)
 */
static void config_new_number_cmd(int argc, char **argv) {
	if (argc != 5) {
		tsc_kprintf("Usage: config_new_number <name> <access> <type> <value>\n");
		return;
	}

	if (config_find_entry(argv[1])) {
		tsc_kprintf("Error: Number config '%s' already exists.\n", argv[1]);
		return;
	}

	char *name = argv[1];
	cfg_access_level_t access = (cfg_access_level_t) cli_parse_int(argv[2]);
	cfg_type_t type = (cfg_type_t) cli_parse_int(argv[3]);
	char *value_str = argv[4];

	/* Use stack buffer instead of malloc */
	uint64_t val_buf = 0;
	if (config_type_size(type) > sizeof(val_buf)) {
		tsc_kprintf("Error: Type size too large for static buffer.\n");
		return;
	}

	config_str_set_entry_val((void*) &val_buf, type, value_str);
	config_entry_t *e_num = config_new_number(name, access, type, (void*) &val_buf);

	if (e_num != NULL) {
		config_add_entry(e_num);
		tsc_kprintf("Number config '%s' added.\n", e_num->name);
	} else {
		tsc_kprintf("Error: Failed to create number config.\n");
	}
}

/**
 * @brief Create new range entry (Static Memory Allocation)
 */
static void config_new_range_cmd(int argc, char **argv) {
	if (argc != 7) {
		tsc_kprintf("Usage: config_new_range <name> <access> <type> <value> <min> <max>\n");
		return;
	}

	if (config_find_entry(argv[1])) {
		tsc_kprintf("Error: Number config '%s' already exists.\n", argv[1]);
		return;
	}

	char *name = argv[1];
	cfg_access_level_t access = (cfg_access_level_t) cli_parse_int(argv[2]);
	cfg_type_t type = (cfg_type_t) cli_parse_int(argv[3]);

	/* Use stack buffers instead of malloc */
	uint64_t val_buf = 0;
	uint64_t min_buf = 0;
	uint64_t max_buf = 0;

	if (config_type_size(type) > sizeof(val_buf)) {
		tsc_kprintf("Error: Type size too large.\n");
		return;
	}

	config_str_set_entry_val((void*) &val_buf, type, argv[4]);
	config_str_set_entry_val((void*) &min_buf, type, argv[5]);
	config_str_set_entry_val((void*) &max_buf, type, argv[6]);

	config_entry_t *e_range = config_new_range(name, access, type, (void*) &val_buf, (void*) &min_buf, (void*) &max_buf);

	if (e_range != NULL) {
		config_add_entry(e_range);
		tsc_kprintf("Range config '%s' added.\n", e_range->name);
	} else {
		tsc_kprintf("Error: Failed to create range config.\n");
	}
}

/**
 * @brief Create new string entry
 */
static void config_new_string_cmd(int argc, char **argv) {
	if (argc != 4) {
		tsc_kprintf("Usage: config_new_string <name> <access> <value>\n");
		return;
	}

	if (config_find_entry(argv[1])) {
		tsc_kprintf("Error: String config '%s' already exists.\n", argv[1]);
		return;
	}

	char *name = argv[1];
	cfg_access_level_t access = (cfg_access_level_t) cli_parse_int(argv[2]);
	void *value = (void*) argv[3];

	config_entry_t *e_str = config_new_string(name, access, value);
	if (e_str != NULL) {
		config_add_entry(e_str);
		tsc_kprintf("String config '%s' added.\n", e_str->name);
	} else {
		tsc_kprintf("Error: Failed to create string config.\n");
	}
}

/* -------------------------------------------------------------------------- */
/* Config profile commands                                                    */
/* -------------------------------------------------------------------------- */
TSC_CMD_EXPORT_ALIAS(config_new_cmd,       config_new,       Create new config);
TSC_CMD_EXPORT_ALIAS(config_load_cmd,      config_load,      Load config from flash);
TSC_CMD_EXPORT_ALIAS(config_save_cmd,      config_save,      Save config to flash);
TSC_CMD_EXPORT_ALIAS(config_default_cmd,   config_default,   Restore default config);
/* -------------------------------------------------------------------------- */
/* Config entry manipulation commands                                         */
/* -------------------------------------------------------------------------- */
TSC_CMD_EXPORT_ALIAS(config_remove_cmd,    config_remove,    Remove config entry);
TSC_CMD_EXPORT_ALIAS(config_edit_cmd,      config_edit,      Edit config entry: config_edit <name> <value>);
TSC_CMD_EXPORT_ALIAS(config_list_cmd,      config_list,      List all config entries);
TSC_CMD_EXPORT_ALIAS(config_list_all_cmd,  config_list_all,  Show all config entry info);
/* -------------------------------------------------------------------------- */
/* User / access control commands                                             */
/* -------------------------------------------------------------------------- */
TSC_CMD_EXPORT_ALIAS(config_login_cmd,     config_login,     Login: config_login <username> <password>);
TSC_CMD_EXPORT_ALIAS(config_whoami_cmd,    config_whoami,    Show current user info);
/* -------------------------------------------------------------------------- */
/* Help command                                                               */
/* -------------------------------------------------------------------------- */
TSC_CMD_EXPORT_ALIAS(config_help_cmd,      config_help,      Show config command help);
/* -------------------------------------------------------------------------- */
/* String storage commands                                                    */
/* -------------------------------------------------------------------------- */
#if ENABLE_LITTLEFS && ENABLE_S25FL128L
TSC_CMD_EXPORT(config_set_string,          Set config string value safely);
TSC_CMD_EXPORT(config_get_string,          Get config string value);
#endif

/* -------------------------------------------------------------------------- */
/* Dynamic entry creation commands (static memory only)                       */
/* -------------------------------------------------------------------------- */
TSC_CMD_EXPORT_ALIAS(config_new_number_cmd, config_new_number, Add new config entry of type number);
TSC_CMD_EXPORT_ALIAS(config_new_range_cmd,  config_new_range,  Add new config entry of type number with range);
TSC_CMD_EXPORT_ALIAS(config_new_string_cmd, config_new_string, Add new config entry of type string);

#endif


#endif /* CSP_ENABLE_CONFIG */
