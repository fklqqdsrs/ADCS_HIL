/**
 * @file config_core.h
 * @brief TSC1 Mission Standard Configuration Manager Header
 * @details Implements static-allocation configuration management with profile support,
 *          access control, and STM32L496 power-optimized storage handling.
 * @author chaturong@narit.or.th
 * @version 1.0.0
 * @date 2023-10-22
 */

#ifndef CONFIG_CORE_H
#define CONFIG_CORE_H

#ifdef __cplusplus
extern "C" {
#endif
// ---------- (1) SYSTEM INCLUDE --------------------------------------------------------------------- //
#include <stdint.h>
#include <stdbool.h>
#include <lfs.h>

// ---------- (2) EXTERNAL MODULE INCLUDE ------------------------------------------------------------ //
// N/A

// ---------- (3) PUBLIC PROGRAMMING DEFINE ---------------------------------------------------------- //
#define CONFIG_MAGIC_NUMBER 		0xA65FC246

#define CONFIG_NAME_MAX_LEN 		32
#define CONFIG_PATH_MAX_LEN 		64

#define CONFIG_MAX_ENTRIES  		64
#define CONFIG_MAX_PROFILES 		2
#define CONFIG_PROFILE_SLOT_SIZE 	64
#define CONFIG_FILE_PREFIX 			"cfg_"
#define CONFIG_STRING_FILE_PREFIX 	"str_"
#define CONFIG_STRING_SLOT_SIZE 	256
#define CONFIG_STRING_SLOT_MAX 		256

#define CONFIG_STATIC_PROFILE_NAME 	"static"

// ---------- (4) ENUMERATOR DEFINITION -------------------------------------------------------------- //
/** Supported config types */
typedef enum {
	CFG_TYPE_BOOL,
	CFG_TYPE_UINT8,
	CFG_TYPE_UINT16,
	CFG_TYPE_UINT32,
	CFG_TYPE_UINT64,
	CFG_TYPE_INT8,
	CFG_TYPE_INT16,
	CFG_TYPE_INT32,
	CFG_TYPE_INT64,
	CFG_TYPE_FLOAT,
	CFG_TYPE_DOUBLE,
	CFG_TYPE_STRING
} cfg_type_t;

/** Config access permissions */
typedef enum {
	CFG_PERM_NONE = 0,  ///< No access
	CFG_PERM_R,         ///< Read-only
	CFG_PERM_RW         ///< Read/Write
} cfg_permission_t;

/** Config user access level */
typedef enum {
	CFG_ACCESS_NONE = 0,		///< Accessible by everyones
	CFG_ACCESS_OPERATOR,    ///< Accessible by operators
	CFG_ACCESS_DEVELOP,     ///< Accessible by developers
	CFG_ACCESS_MENU,    	///< Accessible by menu-level users
} cfg_access_level_t;

// ---------- (5) STRUCT OR UNION DATA TYPE DEFINITION ----------------------------------------------- //
/** User database (username/password/access) */
typedef struct {
	const char *username;
	const char *password;
	cfg_access_level_t level;
} user_credential_t;

/**
 * @brief Single config entry
 */
typedef struct {
	char *name;       					  ///< Entry name (unique key)
	cfg_type_t type;                      ///< Data type
	cfg_permission_t permission;          ///< Permission (R, RW)
	cfg_access_level_t access;            ///< User access level
	void *value;                          ///< Pointer to value
	const void *default_value;            ///< Pointer to default value
	void *min;                            ///< Pointer to min (or NULL)
	const void *default_min;              ///< Pointer to default min value
	void *max;                            ///< Pointer to max (or NULL)
	const void *default_max;              ///< Pointer to default max value
	uint32_t size;                        ///< Size of value (bytes)

	bool range_en;						  ///< true if min max enable
	bool is_dynamic;                      ///< true if malloc() used
} config_entry_t;

/**
 * @brief Profile that contains multiple config entries
 */
typedef struct {
	char profile_name[CONFIG_NAME_MAX_LEN];   ///< Profile name
	uint32_t num_static_entries;              ///< Total number of static entries
	uint32_t num_entries;                     ///< Total number of all entries
	uint32_t magic_number;                    ///< Magic number for check first time backup
	config_entry_t *entries[CONFIG_MAX_ENTRIES]; ///< Entry list
} config_profile_t;

/**
 * @brief Union structure for save config bin entries
 */
union save_type_u {        // Union declaration
	bool b_val;
	uint8_t u8_val;
	uint16_t u16_val;
	uint32_t u32_val;
	uint64_t u64_val;
	int8_t i8_val;
	int16_t i16_val;
	int32_t i32_val;
	int64_t i64_val;
	float f_val;
	double d_val;
	uint32_t str_index_val;
};

// ---------- (6) TYPEDEF DATA TYPE DEFINITION ------------------------------------------------------- //
// N/A

// ---------- (7) PUBLIC MACRO DEFINITION ------------------------------------------------------------ //
#define TSC_CONFIG_SECTION __attribute__((used, section(".config_table")))

/**
 * @brief Macro to declare a static config entry in flash
 */
#define TSC_CONFIG_ENTRY_T(c_name) 			__config_##c_name##_

#define TSC_CONFIG_ADD_NUMBER_RANGE(c_name, c_type, per, acc, val, def_val, v_min, def_min, v_max, def_max, var)	\
	const var __config_##c_name##_def_val __attribute__((section(".rodata.val"))) = def_val;	\
	const var __config_##c_name##_def_min __attribute__((section(".rodata.val"))) = def_min;	\
	const var __config_##c_name##_def_max __attribute__((section(".rodata.val"))) = def_max;	\
	__USED const config_entry_t __config_##c_name##_ TSC_CONFIG_SECTION = { 		\
        .name = #c_name,                                                    		\
        .type = c_type,                                                  			\
		.permission = per,													   		\
        .access = acc,                                               				\
        .value = &val,                                                  			\
        .default_value = (const void *)&__config_##c_name##_def_val,         		\
        .min = &v_min,                                         						\
        .default_min = (const void *)&__config_##c_name##_def_min,         			\
        .max = &v_max,																\
		.default_max = (const void *)&__config_##c_name##_def_max,					\
		.size = sizeof(var),														\
		.range_en = true,															\
		.is_dynamic = false,														\
    };

#define TSC_CONFIG_ADD_NUMBER(c_name, c_type, per, acc, val, def_val, var)			\
	const var __config_##c_name##_def_val __attribute__((section(".rodata.val"))) = def_val;	\
	__USED const config_entry_t __config_##c_name##_ TSC_CONFIG_SECTION = { 		\
        .name = #c_name,                                                    		\
        .type = c_type,                                                  			\
		.permission = per,													   		\
        .access = acc,                                               				\
        .value = &val,                                                  			\
        .default_value = (const void *)&__config_##c_name##_def_val,         		\
        .min = NULL,                                         						\
        .default_min = NULL,                                            			\
        .max = NULL,																\
		.default_max = NULL,														\
		.size = sizeof(var),														\
		.range_en = false,															\
		.is_dynamic = false,														\
    };

#define TSC_CONFIG_ADD_STRING(c_name, per, acc, val, def_val)						\
	const char __config_##c_name##_def_val[] __attribute__((section(".rodata.val"))) = #def_val;\
	__USED const config_entry_t __config_##c_name##_ TSC_CONFIG_SECTION = { 		\
        .name = #c_name,                                                    		\
        .type = CFG_TYPE_STRING,                                               		\
		.permission = per,													   		\
        .access = acc,                                               				\
        .value = val,                                                  				\
        .default_value = __config_##c_name##_def_val,         						\
        .min = NULL,                                         						\
        .default_min = NULL,                                            			\
        .max = NULL,																\
		.default_max = NULL,														\
		.size = sizeof(val),														\
		.range_en = false,															\
		.is_dynamic = false,														\
    };

#define TSC_CONFIG_ADD_BOOL(c_name, per, acc, val, def_val)							\
		TSC_CONFIG_ADD_NUMBER(c_name, CFG_TYPE_BOOL, per, acc, val, def_val, bool)

// ---------- (8) EXTERN FUNCTION -------------------------------------------------------------------- //
// N/A

// ---------- (9) EXTERN VARIABLE -------------------------------------------------------------------- //
// N/A

// ---------- (10) CLASS DECLARATION ----------------------------------------------------------------- //

#include "app_modules_config.h"

#if CSP_ENABLE_CONFIG

bool config_login(char *username, char *password);
void config_logout(void);
const char* config_get_access_user(void);
cfg_access_level_t config_get_access_level(void);

/**
 * @brief Set the active profile to be used
 * @param profile_name Name of the profile
 * @return true if successful
 */
bool config_set_active_profile(const char *profile_name);
config_profile_t* config_get_active_profile(void);

/**
 * @brief Add a new config entry to active profile
 * @param entry Pointer to the config entry
 * @return true if added successfully
 */
bool config_add_entry(config_entry_t *entry);

/**
 * @brief Remove an entry by name
 * @param name Entry name to remove
 * @return true if removed
 */
bool config_remove_entry(const char *name);

/**
 * @brief Find an entry by name
 * @param name Entry name to search
 * @return Pointer to entry, or NULL if not found
 */
config_entry_t* config_find_entry(const char *name);

/**
 * @brief Free dynamically allocated memory in entry
 * @param e Entry to free
 */
bool config_free_entry(config_entry_t *e);

config_entry_t* config_new_number(char *name, cfg_access_level_t access, cfg_type_t type, void *value);
config_entry_t* config_new_range(char *name, cfg_access_level_t access, cfg_type_t type, void *value, void *min, void *max);
config_entry_t* config_new_string(char *name, cfg_access_level_t access, void *value);

/**
 * @brief Save active profile to all 3 SPI flash devices
 * @return true if at least one write succeeds
 */
bool config_save_profile(void);

/**
 * @brief Load profile from SPI flash, auto-fallback on corruption
 * @return true if successfully loaded
 */
bool config_load_profile(void);

/**
 * @brief Load all entries in profile to their default values
 * @param save Auto save after load default
 */
void config_load_profile_default(bool save);

/**
 * @brief Clear profile by free a dynamic entry
 * profile Pointer to profile to clear
 */
void config_clear_profile(config_profile_t *profile);

/**
 * @brief Print one entry for debugging/logging
 * @param e Pointer to entry
 */
void config_print_entry(const config_entry_t *e);

const char* config_type_str(cfg_type_t type);
const size_t config_type_size(cfg_type_t type);
const char* config_perm_str(cfg_permission_t p);
const char* config_access_str(cfg_access_level_t a);
int config_val_str(cfg_type_t type, void *value, char *buff);
void config_set_entry_val(void *e_val, cfg_type_t type, void *val_set);
union save_type_u config_set_entry_bin_val(union save_type_u *val, cfg_type_t type, void *val_set);
void config_str_set_entry_val(void *val, cfg_type_t type, char *value_str);

/**
 * @brief Save current profile to all backup SPI flash devices
 * @return true if all backups succeeded, false otherwise
 */
bool config_manager_backup(void);

/**
 * @brief Restore config from first valid SPI flash backup
 * @return true if recovery succeeded, false otherwise
 */
bool config_manager_restore(void);

/**
 * @brief Initialize config manager
 */
void config_manager_init(void);
void example_mode_based_config_demo(void);

#else /* CSP_ENABLE_CONFIG */

/* Stubs when configuration manager is disabled */
static inline bool config_login(char *u, char *p) { (void)u; (void)p; return false; }
static inline void config_logout(void) {}
static inline const char* config_get_access_user(void) { return "none"; }
static inline cfg_access_level_t config_get_access_level(void) { return CFG_ACCESS_NONE; }
static inline bool config_set_active_profile(const char *n) { (void)n; return false; }
static inline config_profile_t* config_get_active_profile(void) { return NULL; }
static inline bool config_add_entry(config_entry_t *e) { (void)e; return false; }
static inline bool config_remove_entry(const char *n) { (void)n; return false; }
static inline config_entry_t* config_find_entry(const char *n) { (void)n; return NULL; }
static inline bool config_free_entry(config_entry_t *e) { (void)e; return false; }
static inline config_entry_t* config_new_number(char *n, cfg_access_level_t a, cfg_type_t t, void *v) { (void)n; (void)a; (void)t; (void)v; return NULL; }
static inline config_entry_t* config_new_range(char *n, cfg_access_level_t a, cfg_type_t t, void *v, void *m1, void *m2) { (void)n; (void)a; (void)t; (void)v; (void)m1; (void)m2; return NULL; }
static inline config_entry_t* config_new_string(char *n, cfg_access_level_t a, void *v) { (void)n; (void)a; (void)v; return NULL; }
static inline bool config_save_profile(void) { return false; }
static inline bool config_load_profile(void) { return false; }
static inline void config_load_profile_default(bool s) { (void)s; }
static inline void config_clear_profile(config_profile_t *p) { (void)p; }
static inline void config_print_entry(const config_entry_t *e) { (void)e; }
static inline const char* config_type_str(cfg_type_t t) { (void)t; return "none"; }
static inline const size_t config_type_size(cfg_type_t t) { (void)t; return 0; }
static inline const char* config_perm_str(cfg_permission_t p) { (void)p; return "none"; }
static inline const char* config_access_str(cfg_access_level_t a) { (void)a; return "none"; }
static inline int config_val_str(cfg_type_t t, void *v, char *b) { (void)t; (void)v; if(b) b[0]=0; return 0; }
static inline void config_set_entry_val(void *ev, cfg_type_t t, void *vs) { (void)ev; (void)t; (void)vs; }
static inline union save_type_u config_set_entry_bin_val(union save_type_u *v, cfg_type_t t, void *vs) { (void)t; (void)vs; return *v; }
static inline void config_str_set_entry_val(void *v, cfg_type_t t, char *s) { (void)v; (void)t; (void)s; }
static inline bool config_manager_backup(void) { return false; }
static inline bool config_manager_restore(void) { return false; }
static inline void config_manager_init(void) {}
static inline void example_mode_based_config_demo(void) {}

#endif /* CSP_ENABLE_CONFIG */
// ---------- (11) END OF CLASS DECLARATION ---------------------------------------------------------- //
#if defined __cplusplus
}
#endif

#endif
