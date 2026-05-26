/**
 * @file config_storage.h
 * @brief Header file for config save/load with littlefs filesystem
 * @note TSC1 Mission Standards - STM32L496 Architecture
 * @author Original: Pongpot Chaiboonuang
 * @author Refactored: chaturong@narit.or.th
 *
 * --------------------------------------------------------------------------------------------------- //
 * COMPANY NAME : IOT POW. / NARIT
 *
 * INTRODUCTION : CSP time application.
 *
 * CONTACT E-MAIL ADDRESS : pongpot.c@gmail.com, chaturong@narit.or.th
 *
 * --------------------------------------------------------------------------------------------------- //
 */

#ifndef _CONFIG_STORAGE_H
#define _CONFIG_STORAGE_H

#ifdef __cplusplus
extern "C" {
#endif

// ---------- (1) SYSTEM INCLUDE --------------------------------------------------------------------- //
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <config_core.h> /* Provides: config_entry_t, config_profile_t, lfs_t, cfg_type_t, etc. */

// ---------- (2) EXTERNAL MODULE INCLUDE ------------------------------------------------------------ //
// N/A

// ---------- (3) PUBLIC PROGRAMMING DEFINE ---------------------------------------------------------- //
// N/A

// ---------- (4) ENUMERATOR DEFINITION -------------------------------------------------------------- //
// N/A

// ---------- (5) STRUCT OR UNION DATA TYPE DEFINITION ----------------------------------------------- //

/**
 * @brief Binary representation of a configuration entry for file storage.
 * @note Packed to ensure consistent binary layout on flash.
 */
#pragma pack(push, 1)
typedef struct {
	char name[CONFIG_NAME_MAX_LEN];       ///< Parameter name
	cfg_type_t type;                      ///< Data type identifier
	cfg_permission_t permission;          ///< Read/Write permissions
	cfg_access_level_t access;            ///< User access level required
	uint8_t is_dynamic :4;               ///< Flag for dynamic entries
	uint8_t range_en :4;                 ///< Flag for range enforcement
	union save_type_u value;              ///< Current value
	union save_type_u default_value;      ///< Factory default value
	struct {
		union save_type_u min, max;
	} range;         ///< Allowed range
	struct {
		union save_type_u min, max;
	} default_range; ///< Default range
	uint32_t size;                        ///< Size of this structure
	uint32_t crc32;                       ///< Checksum of this entry
} config_entry_bin_t;
#pragma pack(pop)

/**
 * @brief File header for configuration profile.
 */
#pragma pack(push, 1)
typedef struct {
	uint32_t magic_number;                    ///< Magic number for check first time backup
	char name[CONFIG_NAME_MAX_LEN];           ///< Profile name
	uint32_t num_entries;                     ///< Total number of entries
	uint32_t crc32;                           ///< Checksum of the profile data
} config_profile_bin_t;
#pragma pack(pop)

// ---------- (6) TYPEDEF DATA TYPE DEFINITION ------------------------------------------------------- //
// N/A

// ---------- (7) PUBLIC MACRO DEFINITION ------------------------------------------------------------ //
// N/A

// ---------- (8) EXTERN FUNCTION -------------------------------------------------------------------- //

#include "app_modules_config.h"

#if ENABLE_LITTLEFS

/**
 * @brief Save string to spi flash file.
 *
 * Writes a string associated with a specific index to the LittleFS filesystem.
 * Safe for STM32L496 low-power context.
 *
 * @param lfs Pointer to littlefs filesystem handle
 * @param profile Pointer to config profile (context)
 * @param index String index number to save
 * @param str Pointer to string buffer to save
 * @return true on success, false otherwise
 */
bool config_save_string_file(lfs_t *lfs, config_profile_t *profile, uint32_t index, const char *str);

/**
 * @brief Load string from spi flash file.
 *
 * Reads a string associated with a specific index from the LittleFS filesystem.
 * Includes bounds checking for max_len.
 *
 * @param lfs Pointer to littlefs filesystem handle
 * @param profile Pointer to config profile
 * @param index String index number to load
 * @param out Pointer to output string buffer
 * @param max_len Maximum length of the output buffer
 * @return true on success, false otherwise
 */
bool config_load_string_file(lfs_t *lfs, config_profile_t *profile, uint32_t index, char *out, size_t max_len);

/**
 * @brief Convert runtime config entry to binary format.
 *
 * serializes the runtime structure into the packed binary structure.
 *
 * @param src Pointer to source runtime entry
 * @param dst Pointer to destination binary entry
 * @return true on success, false otherwise
 */
bool config_entry_to_bin(const config_entry_t *src, config_entry_bin_t *dst);

/**
 * @brief Convert binary config entry to runtime format.
 *
 * Deserializes the packed binary structure into the runtime structure.
 *
 * @param src Pointer to source binary entry
 * @param dst Pointer to destination runtime entry
 * @param dynamic_entry Flag to indicate if entry is dynamic
 * @return true on success, false otherwise
 */
bool config_bin_to_entry(const config_entry_bin_t *src, config_entry_t *dst, uint8_t dynamic_entry);

/**
 * @brief Check current config profile is backup in spi flash file.
 *
 * Verifies the existence and integrity (Magic Number) of the profile file.
 *
 * @param lfs Pointer to littlefs filesystem
 * @param path Path to load binary file
 * @param profile Pointer to config profile
 * @return true on success (valid backup exists), false otherwise
 */
bool config_is_profile_backup(lfs_t *lfs, const char *path, config_profile_t *profile);

/**
 * @brief Save current config profile to spi flash file.
 *
 * Serializes the entire profile and writes it to LFS with CRC protection.
 *
 * @param lfs Pointer to littlefs filesystem
 * @param path Path to store binary file
 * @param profile Pointer to config profile
 * @return true on success, false otherwise
 */
bool config_save_profile_file(lfs_t *lfs, const char *path, config_profile_t *profile);

/**
 * @brief Load config profile from spi flash file.
 *
 * Reads and deserializes the profile from LFS, verifying CRC and Magic Number.
 *
 * @param lfs Pointer to littlefs filesystem
 * @param path Path to load binary file
 * @param profile Pointer to config profile
 * @return true on success and valid CRC, false otherwise
 */
bool config_load_profile_file(lfs_t *lfs, const char *path, config_profile_t *profile);

#else /* ENABLE_LITTLEFS */

/* Stubs when LittleFS is disabled */
static inline bool config_save_string_file(lfs_t *l, config_profile_t *p, uint32_t i, const char *s) { (void)l; (void)p; (void)i; (void)s; return false; }
static inline bool config_load_string_file(lfs_t *l, config_profile_t *p, uint32_t i, char *o, size_t m) { (void)l; (void)p; (void)i; (void)o; (void)m; return false; }
static inline bool config_entry_to_bin(const config_entry_t *s, config_entry_bin_t *d) { (void)s; (void)d; return false; }
static inline bool config_bin_to_entry(const config_entry_bin_t *s, config_entry_t *d, uint8_t dy) { (void)s; (void)d; (void)dy; return false; }
static inline bool config_is_profile_backup(lfs_t *l, const char *pa, config_profile_t *pr) { (void)l; (void)pa; (void)pr; return false; }
static inline bool config_save_profile_file(lfs_t *l, const char *pa, config_profile_t *pr) { (void)l; (void)pa; (void)pr; return false; }
static inline bool config_load_profile_file(lfs_t *l, const char *pa, config_profile_t *pr) { (void)l; (void)pa; (void)pr; return false; }

#endif /* ENABLE_LITTLEFS */

// ---------- (11) END OF CLASS DECLARATION ---------------------------------------------------------- //
#ifdef __cplusplus
}
#endif

#endif
