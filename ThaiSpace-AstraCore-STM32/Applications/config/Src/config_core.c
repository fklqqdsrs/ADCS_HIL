#include <app_modules_config.h>

#if CSP_ENABLE_CONFIG

//! @file config_manager.c
//! @brief C Source File Template

// --------------------------------------------------------------------------------------------------- //
// COMPANY NAME : IOT POW.
//
// INTRODUCTION : Config manager application.
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
// Features:
// - Multiple types: bool, int, float, double, string
// - Permissions and access control
// - Dynamic memory support
// - Factory reset
// - CRC-based integrity check
// - Triple SPI Flash LittleFS support
//
//
// --------------------------------------------------------------------------------------------------- //

// ---------- I M P O R T A N T   N O T E ------------------------------------------------------------ //
//
// --------------------------------------------------------------------------------------------------- //
// N/A

// ---------- (1) LOCAL PREDEFINE -------------------------------------------------------------------- //
#define __THIS_FILE_NAME__		"config_core.c"

// ---------- (2) STANDARD LIBRARY INCLUDE ----------------------------------------------------------- //
#include <string.h>
#include <stdlib.h>
#include <kservice.h>
#include <crc32.h>

// ---------- (3) USER INCLUDE ----------------------------------------------------------------------- //
#include <config_core.h>
#include <config_storage.h>

#define LOG_TAG		"config_manager"
#define LOG_LVL		DBG_LOG
#include <tsc_log_core.h>

// ---------- (4) EXTERN OBJECT ---------------------------------------------------------------------- //
#if ENABLE_LITTLEFS && ENABLE_S25FL128L
extern lfs_t lfs_array[SPI_FLASH_NUM]; // SPI Flash max number
#endif
extern const config_entry_t __start_config_table[];
extern const config_entry_t __stop_config_table[];

// ---------- (5) INTERFACE FUNCTION ----------------------------------------------------------------- //
// N/A

// ---------- (6) DATA ------------------------------------------------------------------------------- //
static config_profile_t g_profiles[CONFIG_MAX_PROFILES];
config_profile_t *g_active_profile = NULL;

static const user_credential_t user_db[] = {
    {"guest",	"",       	CFG_ACCESS_NONE},
    {"oper", 	"oper123",  CFG_ACCESS_OPERATOR},
    {"dev",  	"dev123", 	CFG_ACCESS_DEVELOP},
	{"menu", 	"menu123",	CFG_ACCESS_MENU},
};
static cfg_access_level_t current_user_access_level = CFG_ACCESS_NONE;
static const char *current_user_name = user_db[CFG_ACCESS_NONE].username;

// ---------- (7) PRIVATE PROGRAMMING DEFINE --------------------------------------------------------- //
// N/A

// ---------- (8) PRIVATE TYPEDEF -------------------------------------------------------------------- //
// N/A

// ---------- (9) PRIVATE FUNCTION ------------------------------------------------------------------- //
// N/A

// ---------- (10) PRIVATE DATA ----------------------------------------------------------------------- //
uint32_t static_num, magic_num;
float temp_val, temp_min, temp_max;
char operation_val[256];
bool light_sw;

// ---------- (11) PRIVATE MACRO DEFINITION ---------------------------------------------------------- //
TSC_CONFIG_ADD_NUMBER(magic_num, CFG_TYPE_UINT32, CFG_PERM_RW, CFG_ACCESS_MENU, magic_num, CONFIG_MAGIC_NUMBER, uint32_t);
TSC_CONFIG_ADD_NUMBER(static_num, CFG_TYPE_UINT32, CFG_PERM_RW, CFG_ACCESS_MENU, static_num, 0, uint32_t);
TSC_CONFIG_ADD_NUMBER_RANGE(temperature, CFG_TYPE_FLOAT, CFG_PERM_RW, CFG_ACCESS_MENU, temp_val, 25.0, temp_min, -10.0, temp_max, 40.0, float);
TSC_CONFIG_ADD_STRING(operation, CFG_PERM_RW, CFG_ACCESS_MENU, operation_val, My name is marumashi!);
TSC_CONFIG_ADD_BOOL(light, CFG_PERM_RW, CFG_ACCESS_MENU, light_sw, false);

// ---------- (12) SOURCE FILE IMPLEMENTATION -------------------------------------------------------- //
// ---------- Access Management ----------
bool config_login(char* username, char* password)
{
    LOG_D("config_login: Attempting login for user [%s]", username);
    for (size_t i = 0; i < sizeof(user_db)/sizeof(user_db[0]); ++i) {
        if (strcmp(username, user_db[i].username) == 0 &&
            strcmp(password, user_db[i].password) == 0) {
            current_user_access_level = user_db[i].level;
            current_user_name = user_db[i].username;
            LOG_I("[LOGIN] Welcome, %s! Access level: %d", current_user_name, current_user_access_level);
            return true;
        }
    }

    LOG_E("[LOGIN] Invalid credentials for user [%s]", username);
    return false;
}

void config_logout(void)
{
	LOG_I("[LOGOUT] User %s logged out. Access level reset to NONE.", current_user_name);
	current_user_access_level = CFG_ACCESS_NONE;
	current_user_name = user_db[CFG_ACCESS_NONE].username;
}

const char* config_get_access_user(void)
{
	return current_user_name;
}

cfg_access_level_t config_get_access_level(void)
{
	return current_user_access_level;
}

// ---------- Profile Management ----------
/**
 * @brief Set the active profile to be used
 * @param profile_name Name of the profile
 * @return true if successful
 */
bool config_set_active_profile(const char *profile_name)
{
    LOG_I("config_set_active_profile: Switching to profile [%s]", profile_name);
    for (int i = 0; i < CONFIG_MAX_PROFILES; i++) {
        if (strcmp(g_profiles[i].profile_name, profile_name) == 0) {
            g_active_profile = &g_profiles[i];
            bool ret = config_load_profile();
            if (ret) LOG_I("config_set_active_profile: Profile [%s] activated and loaded", profile_name);
            else LOG_W("config_set_active_profile: Profile [%s] activated but load failed (using defaults)", profile_name);
            return ret;
        }
    }
    LOG_E("config_set_active_profile: Profile [%s] not found", profile_name);
    return false;
}

config_profile_t* config_get_active_profile(void)
{
	if(g_active_profile)
		return g_active_profile;

	return NULL;
}

// ---------- Entry Management ----------
/**
 * @brief Add a new config entry to active profile
 * @param entry Pointer to the config entry
 * @return true if added successfully
 */
bool config_add_entry(config_entry_t *entry)
{
    if (!g_active_profile) {
        LOG_E("config_add_entry: No active profile");
        return false;
    }
    
    if (g_active_profile->num_entries >= CONFIG_MAX_ENTRIES) {
        LOG_E("config_add_entry: Profile [%s] is full", g_active_profile->profile_name);
        return false;
    }

    if (config_find_entry(entry->name)) {
        LOG_W("config_add_entry: Entry [%s] already exists", entry->name);
    	return false;
    }

    LOG_D("config_add_entry: Adding entry [%s] to profile [%s]", entry->name, g_active_profile->profile_name);
    g_active_profile->entries[g_active_profile->num_entries++] = entry;
    return true;
}

/**
 * @brief Remove an entry by name
 * @param name Entry name to remove
 * @return true if removed
 */
bool config_remove_entry(const char *name)
{
    if (!g_active_profile) {
        LOG_E("config_remove_entry: No active profile");
        return false;
    }
    
    for (uint32_t i = 0; i < g_active_profile->num_entries; i++) {
        if (strcmp(g_active_profile->entries[i]->name, name) == 0) {
        	if(g_active_profile->entries[i]->is_dynamic) {
        		LOG_D("config_remove_entry: Removing dynamic entry [%s]", name);
        		config_free_entry(g_active_profile->entries[i]);
				for (uint32_t j = i; j < g_active_profile->num_entries - 1; j++)
					g_active_profile->entries[j] = g_active_profile->entries[j + 1];
				g_active_profile->num_entries--;
	            return true;
        	}
        	else {
        		LOG_E("config_remove_entry: Error entry [%s] is static and cannot be removed", name);
        		return false;
        	}
        }
    }
    LOG_W("config_remove_entry: Entry [%s] not found", name);
    return false;
}

/**
 * @brief Find an entry by name
 * @param name Entry name to search
 * @return Pointer to entry, or NULL if not found
 */
config_entry_t *config_find_entry(const char *name)
{
    if (!g_active_profile) return NULL;
    for (uint32_t i = 0; i < g_active_profile->num_entries; i++) {
        if (strcmp(g_active_profile->entries[i]->name, name) == 0)
            return g_active_profile->entries[i];
    }
    return NULL;
}

/**
 * @brief Free dynamically allocated memory in entry
 * @param e Entry to free
 */
bool config_free_entry(config_entry_t *e)
{
    if (!e) return false;
    if (e->is_dynamic) {
        LOG_D("config_free_entry: Freeing memory for dynamic entry [%s]", e->name ? e->name : "unknown");
        if(e->value) tsc_free(e->value);
        if(e->default_min) tsc_free((void *)e->default_value);
        if (e->min) tsc_free(e->min);
        if (e->default_min) tsc_free((void *)e->default_min);
        if (e->max) tsc_free(e->max);
        if (e->default_max) tsc_free((void *)e->default_max);
        tsc_free(e);
    }

    return true;
}

config_entry_t* config_new_number(char *name, cfg_access_level_t access, cfg_type_t type, void *value)
{
	config_entry_t *e = tsc_malloc(sizeof(config_entry_t));
	if(!e) {
        LOG_E("config_new_number: Failed to allocate memory for entry [%s]", name);
        return NULL;
    }

    memset(e, 0, sizeof(config_entry_t));
    e->name = strdup(name);
    e->type = type;
    e->permission = CFG_PERM_RW;
    e->access = access;
    e->value = tsc_malloc(config_type_size(e->type));
    config_set_entry_val(e->value, e->type, value);
    e->default_value = tsc_malloc(config_type_size(e->type));
    config_set_entry_val((void *)e->default_value, e->type, value);
    e->range_en = false;
    e->is_dynamic = true;
    e->size = config_type_size(e->type);

    LOG_D("config_new_number: Created entry [%s] type %d", name, type);
    return e;
}

config_entry_t* config_new_range(char *name, cfg_access_level_t access, cfg_type_t type, void *value, void *min, void *max)
{
	config_entry_t *e = tsc_malloc(sizeof(config_entry_t));
	if(!e) {
        LOG_E("config_new_range: Failed to allocate memory for entry [%s]", name);
        return NULL;
    }

	memset(e, 0, sizeof(config_entry_t));
	e->name = strdup(name);
	e->type = type;
	e->permission = CFG_PERM_RW;
	e->access = access;
	e->value = tsc_malloc(config_type_size(e->type));
    config_set_entry_val(e->value, e->type, value);
	e->default_value = tsc_malloc(config_type_size(e->type));
    config_set_entry_val((void *)e->default_value, e->type, value);
	e->min = tsc_malloc(config_type_size(e->type));
	config_set_entry_val(e->min, e->type, min);
	e->default_min = tsc_malloc(config_type_size(e->type));
	config_set_entry_val((void *)e->default_min, e->type, min);
	e->max = tsc_malloc(config_type_size(e->type));
	config_set_entry_val(e->max, e->type, max);
	e->default_max = tsc_malloc(config_type_size(e->type));
	config_set_entry_val((void *)e->default_max, e->type, max);
	e->range_en = true;
	e->is_dynamic = true;
	e->size =  config_type_size(e->type);

    LOG_D("config_new_range: Created entry [%s] with range limits", name);
	return e;
}

config_entry_t* config_new_string(char *name, cfg_access_level_t access, void *value)
{
	config_entry_t *e = tsc_malloc(sizeof(config_entry_t));
	if(!e) {
        LOG_E("config_new_string: Failed to allocate memory for entry [%s]", name);
        return NULL;
    }

	memset(e, 0, sizeof(config_entry_t));
	e->name = strdup(name);
	e->type = CFG_TYPE_STRING;
	e->permission = CFG_PERM_RW;
	e->access = access;
	e->value = tsc_malloc(config_type_size(e->type));
	config_set_entry_val(e->value, e->type, value);
	e->default_value = tsc_malloc(config_type_size(e->type));
	config_set_entry_val((void *)e->default_value, e->type, value);
	e->range_en = false;
	e->is_dynamic = true;
	e->size = config_type_size(e->type);

    LOG_D("config_new_string: Created string entry [%s]", name);
	return e;
}

// ---------- Save/Load ----------
/**
 * @brief Save active profile to all 3 SPI flash devices
 * @return true if at least one write succeeds
 */
bool config_save_profile()
{
    if (!g_active_profile) return false;

#if ENABLE_LITTLEFS && ENABLE_S25FL128L
    char path[CONFIG_PATH_MAX_LEN];
    snprintf(path, sizeof(path), "%s%s.dat", CONFIG_FILE_PREFIX, g_active_profile->profile_name);

    LOG_I("config_save_profile: Saving profile [%s] to triple redundant flash", g_active_profile->profile_name);
    bool any_success = false;
    for (int i = 0; i < SPI_FLASH_NUM; i++) {
        lfs_remove(&lfs_array[i], path);
        if(config_save_profile_file(&lfs_array[i], path, g_active_profile)) {
            any_success = true;
            LOG_D("config_save_profile: Save to Flash No.%d successful", i+1);
        } else {
        	LOG_E("config_save_profile: Error saving [%s] to Flash No.%d failed", g_active_profile->profile_name, i+1);
        }
    }

    if (any_success) LOG_I("config_save_profile: Profile [%s] saved successfully", g_active_profile->profile_name);
    else LOG_E("config_save_profile: CRITICAL - Failed to save profile [%s] to ANY flash!", g_active_profile->profile_name);

    return any_success;
#else
    LOG_W("config_save_profile: Flash storage disabled.");
    return true;
#endif
}

/**
 * @brief Load profile from SPI flash, auto-fallback on corruption
 * @return true if successfully loaded
 */
bool config_load_profile()
{
    if (!g_active_profile) return false;

#if ENABLE_LITTLEFS && ENABLE_S25FL128L
    char path[CONFIG_PATH_MAX_LEN];
    snprintf(path, sizeof(path), "%s%s.dat", CONFIG_FILE_PREFIX, g_active_profile->profile_name);

    LOG_I("config_load_profile: Loading profile [%s] from triple redundant flash", g_active_profile->profile_name);
    for (int i = 0; i < SPI_FLASH_NUM; i++) {
    	if(!config_is_profile_backup(&lfs_array[i], path, g_active_profile)) {
    		LOG_D("config_load_profile: Backup not found on Flash No.%d", i+1);
    		continue;
    	}

    	if (config_load_profile_file(&lfs_array[i], path, g_active_profile)) {
    		LOG_I("config_load_profile: Recovered [%s] from Flash No.%d successful", g_active_profile->profile_name, i+1);
    		return true;
    	}
    	else {
    		LOG_E("config_load_profile: Failed to load [%s] from Flash No.%d (possible corruption)", g_active_profile->profile_name, i+1);
    	}
    }

    LOG_W("config_load_profile: No valid profile found on any flash. Loading factory defaults.");
    config_load_profile_default(true);

    return false;
#else
    LOG_I("config_load_profile: Flash storage disabled. Loading factory defaults.");
    config_load_profile_default(false);
    return true;
#endif
}

/**
 * @brief Load all entries in profile to their default values
 * @param save Auto save after load default
 */
void config_load_profile_default(bool save)
{
    if (!g_active_profile) return;

    LOG_I("config_load_profile_default: Resetting profile [%s] to defaults", g_active_profile->profile_name);
    for (uint32_t i = 0; i < g_active_profile->num_entries; i++) {
        config_entry_t *e = g_active_profile->entries[i];
        if(e->type != CFG_TYPE_STRING) {
			tsc_memcpy(e->value, e->default_value, e->size);
			if(e->default_max) tsc_memcpy(e->min, e->default_min, e->size);
			if(e->default_max) tsc_memcpy(e->max, e->default_max, e->size);
        }
        else {
        	tsc_strncpy(e->value, e->default_value, e->size);
        }
    }
    if(save) config_save_profile();
}

/**
 * @brief Clear all entries in the profile
 */
void config_clear_profile(config_profile_t *profile)
{
    if (!profile) return;

    LOG_I("config_clear_profile: Clearing dynamic entries for profile [%s]", profile->profile_name);
    for(uint32_t i = profile->num_static_entries; i < profile->num_entries; i++) {
    	config_free_entry(profile->entries[i]);
    }

    profile->num_entries = profile->num_static_entries;
}

/**
 * @brief Print one entry for debugging/logging
 * @param e Pointer to entry
 */
void config_print_entry(const config_entry_t *e)
{
    if (!e) return;
    tsc_kprintf("Name: %s\r\n", e->name);
    tsc_kprintf("Type: %s(%d)\r\n", config_type_str(e->type), e->type);
    tsc_kprintf("Permission: %s\r\n", config_perm_str(e->permission));
    tsc_kprintf("Access Level: %s\r\n", config_access_str(e->access));
    tsc_kprintf("Size: %lu bytes\r\n", e->size);
    tsc_kprintf("Value: ");
    config_val_str(e->type, e->value, NULL);
    tsc_kprintf("Default Value: ");
    config_val_str(e->type, (void *)e->default_value, NULL);
    tsc_kprintf("Min Value: ");
    config_val_str(e->type, e->min, NULL);
    tsc_kprintf("Default Min Value: ");
    config_val_str(e->type, (void *)e->default_min, NULL);
    tsc_kprintf("Max Value: ");
    config_val_str(e->type, e->max, NULL);
    tsc_kprintf("Default Max Value: ");
    config_val_str(e->type, (void *)e->default_max, NULL);
    tsc_kprintf("Is dynamic: %s\r\n", e->is_dynamic ? "Yes" : "No");
    tsc_kprintf("----------------------\r\n");
}

const char *config_type_str(cfg_type_t type) {
    switch (type) {
        case CFG_TYPE_BOOL: return "bool";
        case CFG_TYPE_UINT8: return "uint8";
        case CFG_TYPE_UINT16: return "uint16";
        case CFG_TYPE_UINT32: return "uint32";
        case CFG_TYPE_UINT64: return "uint64";
        case CFG_TYPE_INT8: return "int8";
        case CFG_TYPE_INT16: return "int16";
        case CFG_TYPE_INT32: return "int32";
        case CFG_TYPE_INT64: return "int64";
        case CFG_TYPE_FLOAT: return "float";
        case CFG_TYPE_DOUBLE: return "double";
        case CFG_TYPE_STRING: return "string";
        default: return "unknown";
    }
}

const size_t config_type_size(cfg_type_t type) {
    switch (type) {
        case CFG_TYPE_BOOL: return sizeof(bool);
        case CFG_TYPE_UINT8: return sizeof(uint8_t);
        case CFG_TYPE_UINT16: return sizeof(uint16_t);
        case CFG_TYPE_UINT32: return sizeof(uint32_t);
        case CFG_TYPE_UINT64: return sizeof(uint64_t);
        case CFG_TYPE_INT8: return sizeof(int8_t);
        case CFG_TYPE_INT16: return sizeof(int16_t);
        case CFG_TYPE_INT32: return sizeof(int32_t);
        case CFG_TYPE_INT64: return sizeof(int64_t);
        case CFG_TYPE_FLOAT: return sizeof(float);
        case CFG_TYPE_DOUBLE: return sizeof(double);
        case CFG_TYPE_STRING: return CONFIG_STRING_SLOT_SIZE;
        default: return 0;
    }
}

const char *config_perm_str(cfg_permission_t p) {
    switch (p) {
        case CFG_PERM_NONE: return "None";
        case CFG_PERM_R:    return "R";
        case CFG_PERM_RW:   return "RW";
        default: return "-";
    }
}

const char *config_access_str(cfg_access_level_t a) {
    switch (a) {
    	case CFG_ACCESS_NONE: return "None";
    	case CFG_ACCESS_OPERATOR: return "Operator";
        case CFG_ACCESS_DEVELOP: return "Develop";
        case CFG_ACCESS_MENU: return "Manufacturer";
        default: return "-";
    }
}

int config_val_str(cfg_type_t type, void *value, char *buff)
{
	char str_buff[CONFIG_STRING_SLOT_SIZE];
	int len;

	if(value == NULL) {
		len = sprintf(str_buff, "NULL");
	}
	else {
		switch (type) {
			case CFG_TYPE_BOOL:
				len = sprintf(str_buff, "%s", *(bool *)value ? "true" : "false"); break;
			case CFG_TYPE_UINT8:
				len = sprintf(str_buff, "%u", *(uint8_t *)value); break;
			case CFG_TYPE_UINT16:
				len = sprintf(str_buff, "%u", *(uint16_t *)value); break;
			case CFG_TYPE_UINT32:
				len = sprintf(str_buff, "%lu", *(uint32_t *)value); break;
			case CFG_TYPE_UINT64:
				len = sprintf(str_buff, "%llu", *(uint64_t *)value); break;
			case CFG_TYPE_INT8:
				len = sprintf(str_buff, "%d", *(int8_t *)value); break;
			case CFG_TYPE_INT16:
				len = sprintf(str_buff, "%d", *(int16_t *)value); break;
			case CFG_TYPE_INT32:
				len = sprintf(str_buff, "%ld", *(int32_t *)value); break;
			case CFG_TYPE_INT64:
				len = sprintf(str_buff, "%lld", *(int64_t *)value); break;
			case CFG_TYPE_FLOAT:
				len = sprintf(str_buff, "%f", *(float *)value); break;
			case CFG_TYPE_DOUBLE:
				len = sprintf(str_buff, "%lf", *(double *)value); break;
			case CFG_TYPE_STRING:
				len = sprintf(str_buff, "%s", (char *)value); break;
			default:
				len = sprintf(str_buff, "NA Type");
		}
	}

    if(buff) {
    	tsc_strcpy(buff, str_buff);
    }
    else {
    	tsc_kprintf("%s\r\n", str_buff);
    }

    return len;
}

void config_set_entry_val(void *e_val, cfg_type_t type, void *val_set) {
	switch (type) {
	case CFG_TYPE_BOOL:
		*(bool*) e_val = *(bool*)val_set;
		break;
	case CFG_TYPE_UINT8:
		*(uint8_t*) e_val = *(uint8_t*)val_set;
		break;
	case CFG_TYPE_UINT16:
		*(uint16_t*) e_val = *(uint16_t*)val_set;
		break;
	case CFG_TYPE_UINT32:
		*(uint32_t*) e_val = *(uint32_t*)val_set;
		break;
	case CFG_TYPE_UINT64:
		*(uint64_t*) e_val = *(uint64_t*)val_set;
		break;
	case CFG_TYPE_INT8:
		*(int8_t*) e_val = *(int8_t*)val_set;
		break;
	case CFG_TYPE_INT16:
		*(int16_t*) e_val = *(int16_t*)val_set;
		break;
	case CFG_TYPE_INT32:
		*(int32_t*) e_val = *(int32_t*)val_set;
		break;
	case CFG_TYPE_INT64:
		*(int64_t*) e_val = *(int64_t*)val_set;
		break;
	case CFG_TYPE_FLOAT:
		*(float*) e_val = *(float*)val_set;
		break;
	case CFG_TYPE_DOUBLE:
		*(double*) e_val = *(double*)val_set;
		break;
	case CFG_TYPE_STRING:
		int str_len = tsc_snprintf((char*) e_val, CONFIG_STRING_SLOT_SIZE, "%s", val_set);
		*(char*)(e_val + (str_len < CONFIG_STRING_SLOT_SIZE) ? str_len : CONFIG_STRING_SLOT_SIZE - 1) = 0;
		break;
	default:
		LOG_E("config_set_entry_val: Unsupported type %d", type);
		return;
	}
}

union save_type_u config_set_entry_bin_val(union save_type_u *val, cfg_type_t type, void *val_set) {
	union save_type_u bin_val;

	switch (type) {
	case CFG_TYPE_BOOL:
		bin_val.b_val = *(bool*)val_set;
		break;
	case CFG_TYPE_UINT8:
		bin_val.u8_val = *(uint8_t*)val_set;
		break;
	case CFG_TYPE_UINT16:
		bin_val.u16_val = *(uint16_t*)val_set;
		break;
	case CFG_TYPE_UINT32:
		bin_val.u32_val = *(uint32_t*)val_set;
		break;
	case CFG_TYPE_UINT64:
		bin_val.u64_val = *(uint64_t*)val_set;
		break;
	case CFG_TYPE_INT8:
		bin_val.i8_val = *(int8_t*)val_set;
		break;
	case CFG_TYPE_INT16:
		bin_val.i16_val = *(int16_t*)val_set;
		break;
	case CFG_TYPE_INT32:
		bin_val.i32_val = *(int32_t*)val_set;
		break;
	case CFG_TYPE_INT64:
		bin_val.i64_val = *(int64_t*)val_set;
		break;
	case CFG_TYPE_FLOAT:
		bin_val.f_val  = *(float*)val_set;
		break;
	case CFG_TYPE_DOUBLE:
		bin_val.d_val = *(double*)val_set;
		break;
	case CFG_TYPE_STRING:
		bin_val.str_index_val = *(uint32_t*)val_set;
		break;
	default:
		LOG_E("config_set_entry_bin_val: Unsupported type %d", type);
		bin_val.u64_val = 0;
		break;
	}
	if(val)
		*val = bin_val;
	return bin_val;
}

void config_str_set_entry_val(void *val, cfg_type_t type, char *value_str) {
	switch (type) {
	case CFG_TYPE_BOOL:
		*(bool*) val = (strcmp(value_str, "true") == 0
				|| strcmp(value_str, "1") == 0);
		break;
	case CFG_TYPE_UINT8:
		*(uint8_t*) val = (uint8_t) strtoul(value_str, NULL, 10);
		break;
	case CFG_TYPE_UINT16:
		*(uint16_t*) val = (uint16_t) strtoul(value_str, NULL, 10);
		break;
	case CFG_TYPE_UINT32:
		*(uint32_t*) val = (uint32_t) strtoul(value_str, NULL, 10);
		break;
	case CFG_TYPE_UINT64:
		*(uint64_t*) val = (uint64_t) strtoul(value_str, NULL, 10);
		break;
	case CFG_TYPE_INT8:
		*(int8_t*) val = (int8_t) strtol(value_str, NULL, 10);
		break;
	case CFG_TYPE_INT16:
		*(int16_t*) val = (int16_t) strtol(value_str, NULL, 10);
		break;
	case CFG_TYPE_INT32:
		*(int32_t*) val = (int32_t) strtol(value_str, NULL, 10);
		break;
	case CFG_TYPE_INT64:
		*(int64_t*) val = (int64_t) strtol(value_str, NULL, 10);
		break;
	case CFG_TYPE_FLOAT:
		*(float*) val = strtof(value_str, NULL);
		break;
	case CFG_TYPE_DOUBLE:
		*(double*) val = strtod(value_str, NULL);
		break;
	case CFG_TYPE_STRING:
		int str_len = tsc_snprintf((char*) val, CONFIG_STRING_SLOT_SIZE, "%s", value_str);
		*(char*)(val + (str_len < CONFIG_STRING_SLOT_SIZE) ? str_len : CONFIG_STRING_SLOT_SIZE - 1) = 0;
		break;
	default:
		LOG_E("config_str_set_entry_val: Unsupported type %d", type);
		return;
	}
}

void config_init_static_profile(config_profile_t *g_profiles)
{
    LOG_I("config_init_static_profile: Initializing static config table...");
	g_profiles->magic_number = CONFIG_MAGIC_NUMBER;
	tsc_sprintf(g_profiles->profile_name, CONFIG_STATIC_PROFILE_NAME);

	const config_entry_t *entry = __start_config_table;
	while (entry < __stop_config_table) {
		g_profiles->entries[g_profiles->num_static_entries++] = (config_entry_t *)entry;

        if(entry->type != CFG_TYPE_STRING) {
			tsc_memcpy(entry->value, entry->default_value, entry->size);
			if(entry->default_max) tsc_memcpy(entry->min, entry->default_min, entry->size);
			if(entry->default_max) tsc_memcpy(entry->max, entry->default_max, entry->size);
        }
        else {
        	tsc_strncpy(entry->value, entry->default_value, entry->size);
        }

		entry++;
	}

	g_profiles->num_entries = g_profiles->num_static_entries;
    LOG_I("config_init_static_profile: Registered %d static entries", g_profiles->num_static_entries);
}

/**
 * @brief Initialize config manager
 */
void config_manager_init(void)
{
    LOG_I("config_manager_init: Starting Configuration Manager...");
	config_init_static_profile(&g_profiles[0]);
	config_set_active_profile(CONFIG_STATIC_PROFILE_NAME);
    LOG_I("config_manager_init: Ready.");
}
// ---------- (13) END OF SOURCE FILE IMPLEMENTATION ------------------------------------------------- //
// SOURCE FORMAT REVISION B

#endif /* CSP_ENABLE_CONFIG */
