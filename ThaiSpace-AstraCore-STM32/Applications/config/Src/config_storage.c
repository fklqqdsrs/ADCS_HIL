#include "app_modules_config.h"
#if CSP_ENABLE_CONFIG

#include "app_modules_config.h"

#if ENABLE_LITTLEFS

//! @file tsc_csp_app.c
//! @brief C Source File Template
//! @brief Save/load config entries in binary format with CRC using LittleFS.

// --------------------------------------------------------------------------------------------------- //
// COMPANY NAME : IOT POW.
//
// INTRODUCTION : Config manager save and load data from file.
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
#define __THIS_FILE_NAME__		"config_storage.c"

// ---------- (2) STANDARD LIBRARY INCLUDE ----------------------------------------------------------- //
#include <kservice.h>
#include <lfs.h>
#include <crc32.h>

// ---------- (3) USER INCLUDE ----------------------------------------------------------------------- //
#include <config_storage.h>

// ---------- (4) EXTERN OBJECT ---------------------------------------------------------------------- //
// N/A

// ---------- (5) INTERFACE FUNCTION ----------------------------------------------------------------- //
// N/A

// ---------- (6) DATA ------------------------------------------------------------------------------- //
// N/A

// ---------- (7) PRIVATE PROGRAMMING DEFINE --------------------------------------------------------- //
#define	CONFIG_ENTRY_COPY_TO_BIN(vtype, dtype)															\
			if(src->value) dst->value.vtype##_val = *(dtype *)src->value;								\
			if(src->default_value) dst->default_value.vtype##_val = *(dtype *)src->default_value;		\
			if(src->range_en) {																			\
				if(src->min) dst->range.min.vtype##_val = *(dtype *)src->min;							\
				if(src->max) dst->range.max.vtype##_val = *(dtype *)src->max;							\
				if(src->default_min) dst->default_range.min.vtype##_val = *(dtype *)src->default_min;	\
				if(src->default_max) dst->default_range.max.vtype##_val = *(dtype *)src->default_max;	\
			}

#define	CONFIG_BIN_COPY_TO_ENTRY(vtype, dtype)															\
			if(dynamic_entry) {																			\
				dst->value = (dtype *)tsc_malloc(sizeof(dtype));										\
				dst->default_value = (dtype *)tsc_malloc(sizeof(dtype));								\
				*(dtype *)dst->value = src->value.vtype##_val;											\
				*(dtype *)dst->default_value = src->default_value.vtype##_val;							\
				if(src->range_en) {																		\
					dst->min = (dtype *)tsc_malloc(sizeof(dtype));										\
					dst->default_min = (dtype *)tsc_malloc(sizeof(dtype));								\
					dst->max = (dtype *)tsc_malloc(sizeof(dtype));										\
					dst->default_max = (dtype *)tsc_malloc(sizeof(dtype));								\
					*(dtype *)dst->min = src->range.min.vtype##_val;									\
					*(dtype *)dst->default_min = src->default_range.min.vtype##_val;					\
					*(dtype *)dst->max = src->range.max.vtype##_val;									\
					*(dtype *)dst->default_max = src->default_range.max.vtype##_val;					\
				}																						\
			}																							\
			else {																						\
			if(dst->value) *(dtype *)dst->value = (dtype)src->value.vtype##_val;						\
				if(src->range_en) {																		\
					if(dst->min) *(dtype *)dst->min = (dtype)src->range.min.vtype##_val;				\
					if(dst->max) *(dtype *)dst->max = (dtype)src->range.max.vtype##_val; 				\
				}																						\
			}

// ---------- (8) PRIVATE TYPEDEF -------------------------------------------------------------------- //
// N/A

// ---------- (9) PRIVATE FUNCTION ------------------------------------------------------------------- //
// N/A

// ---------- (10) PRIVATE DATA ----------------------------------------------------------------------- //
static uint32_t next_string_index = 0; // Simple auto-increment index
//static uint32_t crc32_table[256];

// ---------- (11) PRIVATE MACRO DEFINITION ---------------------------------------------------------- //
// N/A

// ---------- (12) SOURCE FILE IMPLEMENTATION -------------------------------------------------------- //
//void crc32_init(void) {
//    uint32_t poly = 0xEDB88320;
//    for (uint32_t i = 0; i < 256; i++) {
//        uint32_t crc = i;
//        for (uint32_t j = 0; j < 8; j++)
//            crc = (crc & 1) ? (crc >> 1) ^ poly : (crc >> 1);
//        crc32_table[i] = crc;
//    }
//}
//
//uint32_t crc32_compute(const uint8_t *data, size_t len) {
//    uint32_t crc = 0xFFFFFFFF;
//    for (size_t i = 0; i < len; i++)
//        crc = (crc >> 8) ^ crc32_table[(crc ^ data[i]) & 0xFF];
//    return crc ^ 0xFFFFFFFF;
//}

/**
 * @brief Save string to spi flash file
 * @param profile Pointer to config profile
 * @param lfs Pointer to littlefs filesystem
 * @param index String index number to save
 * @param str Pointer to string buffer to save
 * @return true on success, false otherwise
 */
bool config_save_string_file(lfs_t *lfs, config_profile_t *profile, uint32_t index, const char *str) {
    if (index >= CONFIG_STRING_SLOT_MAX) return false;

    char path[CONFIG_PATH_MAX_LEN];
    snprintf(path, sizeof(path), "%s%s.dat", CONFIG_STRING_FILE_PREFIX, profile->profile_name);

    lfs_file_t file;
    if (lfs_file_open(lfs, &file, path, LFS_O_WRONLY | LFS_O_CREAT) < 0) {
    	tsc_kprintf("%s: Save string [%s] failed!\r\n", __FUNCTION__, str);
        return false;
    }

    lfs_file_seek(lfs, &file, index * CONFIG_STRING_SLOT_SIZE, LFS_SEEK_SET);
    char buf[CONFIG_STRING_SLOT_SIZE] = {0};
    strncpy(buf, str, CONFIG_STRING_SLOT_SIZE - 1);
    lfs_file_write(lfs, &file, buf, CONFIG_STRING_SLOT_SIZE);
    lfs_file_close(lfs, &file);
    return true;
}

/**
 * @brief Load string from spi flash file
 * @param profile Pointer to config profile
 * @param lfs Pointer to littlefs filesystem
 * @param index String index number to load
 * @param str Pointer to string buffer to load
 * @return true on success, false otherwise
 */
bool config_load_string_file(lfs_t *lfs, config_profile_t *profile, uint32_t index, char *out, size_t max_len) {
    if (index >= CONFIG_STRING_SLOT_MAX || max_len < CONFIG_STRING_SLOT_SIZE)
        return false;

    char path[CONFIG_PATH_MAX_LEN];
    snprintf(path, sizeof(path), "%s%s.dat", CONFIG_STRING_FILE_PREFIX, profile->profile_name);

    lfs_file_t file;
    if (lfs_file_open(lfs, &file, path, LFS_O_RDONLY) < 0) {
    	tsc_kprintf("%s: Load string index [%d] failed!\r\n", __FUNCTION__, index);
        return false;
    }

    lfs_file_seek(lfs, &file, index * CONFIG_STRING_SLOT_SIZE, LFS_SEEK_SET);
    lfs_file_read(lfs, &file, out, CONFIG_STRING_SLOT_SIZE);
    out[CONFIG_STRING_SLOT_SIZE - 1] = '\0';
    lfs_file_close(lfs, &file);
    return true;
}

bool config_entry_to_bin(const config_entry_t *src, config_entry_bin_t *dst) {
    memset(dst, 0, sizeof(config_entry_bin_t));
    strncpy(dst->name, src->name, sizeof(dst->name) - 1);
    dst->type = src->type;
    dst->permission = src->permission;
    dst->access = src->access;
    dst->size = src->size;
    dst->range_en = src->range_en;
    dst->is_dynamic = src->is_dynamic;

    switch (src->type) {
    	case CFG_TYPE_BOOL: {
    		dst->value.b_val = *(bool *)src->value;
    		dst->default_value.b_val = *(bool *)src->default_value;
    		break;
    	}
    	case CFG_TYPE_UINT8: {
    		CONFIG_ENTRY_COPY_TO_BIN(u8, uint8_t);
    		break;
    	}
    	case CFG_TYPE_UINT16: {
    		CONFIG_ENTRY_COPY_TO_BIN(u16, uint16_t);
    		break;
    	}
    	case CFG_TYPE_UINT32: {
    		CONFIG_ENTRY_COPY_TO_BIN(u32, uint32_t);
    		break;
    	}
    	case CFG_TYPE_UINT64: {
    		CONFIG_ENTRY_COPY_TO_BIN(u64, uint64_t);
    		break;
    	}
    	case CFG_TYPE_INT8: {
    		CONFIG_ENTRY_COPY_TO_BIN(i8, int8_t);
    		break;
    	}
    	case CFG_TYPE_INT16: {
    		CONFIG_ENTRY_COPY_TO_BIN(i16, int16_t);
    		break;
    	}
    	case CFG_TYPE_INT32: {
    		CONFIG_ENTRY_COPY_TO_BIN(i32, int32_t);
    		break;
    	}
    	case CFG_TYPE_INT64: {
    		CONFIG_ENTRY_COPY_TO_BIN(u16, int64_t);
    		break;
    	}
    	case CFG_TYPE_FLOAT: {
    		CONFIG_ENTRY_COPY_TO_BIN(f, float);
    		break;
    	}
    	case CFG_TYPE_DOUBLE: {
    		CONFIG_ENTRY_COPY_TO_BIN(d, double);
    		break;
    	}
        case CFG_TYPE_STRING: {
			dst->value.str_index_val = next_string_index++;
			dst->default_value.str_index_val = next_string_index++;
            break;
        }
        default:
            break;
    }
    return true;
}

bool config_bin_to_entry(const config_entry_bin_t *src, config_entry_t *dst, uint8_t dynamic_entry) {
    if(dynamic_entry) {
        char *name = tsc_malloc(CONFIG_NAME_MAX_LEN);
        int len = strlen((char *)src->name);
        len = (len < CONFIG_NAME_MAX_LEN - 1) ? len : (CONFIG_NAME_MAX_LEN - 1);
        tsc_strncpy(name, (char *)src->name, len);
        *(name + len) = 0;

		dst->name = name;
		dst->type = src->type;
		dst->permission = src->permission;
		dst->access = src->access;
		dst->is_dynamic = src->is_dynamic;
		dst->range_en = src->range_en;
	    dst->size = src->size;
    }
    else {
    	if(strncmp(dst->name, src->name, CONFIG_NAME_MAX_LEN))
    		return false;
    }

    switch (src->type) {
		case CFG_TYPE_BOOL: {

			 if(dynamic_entry) {
				 bool *value = tsc_malloc(sizeof(bool));
				 bool *default_value = tsc_malloc(sizeof(bool));
				 *value = src->value.b_val;
				 *default_value = src->default_value.b_val;
				 dst->value = value;
				 dst->default_value = default_value;
			 }
			 else {
				 if(dst->value)
					 *(bool *)dst->value = (bool)src->value.b_val;
			 }
			 break;
		}
    	case CFG_TYPE_UINT8: {
    		CONFIG_BIN_COPY_TO_ENTRY(u8, uint8_t);
    		break;
    	}
    	case CFG_TYPE_UINT16: {
    		CONFIG_BIN_COPY_TO_ENTRY(u16, uint16_t);
    		break;
    	}
    	case CFG_TYPE_UINT32: {
    		CONFIG_BIN_COPY_TO_ENTRY(u32, uint32_t);
    		break;
    	}
    	case CFG_TYPE_UINT64: {
    		CONFIG_BIN_COPY_TO_ENTRY(u64, uint64_t);
    		break;
    	}
    	case CFG_TYPE_INT8: {
    		CONFIG_BIN_COPY_TO_ENTRY(i8, int8_t);
    		break;
    	}
    	case CFG_TYPE_INT16: {
    		CONFIG_BIN_COPY_TO_ENTRY(i16, int16_t);
    		break;
    	}
    	case CFG_TYPE_INT32: {
    		CONFIG_BIN_COPY_TO_ENTRY(i32, int32_t);
    		break;
    	}
    	case CFG_TYPE_INT64: {
    		CONFIG_BIN_COPY_TO_ENTRY(u16, int64_t);
    		break;
    	}
    	case CFG_TYPE_FLOAT: {
    		CONFIG_BIN_COPY_TO_ENTRY(f, float);
    		break;
    	}
    	case CFG_TYPE_DOUBLE: {
    		CONFIG_BIN_COPY_TO_ENTRY(d, double);
    		break;
    	}
        case CFG_TYPE_STRING: {
            if(dynamic_entry) {
				dst->value = tsc_malloc(CONFIG_STRING_SLOT_SIZE);
				tsc_memset((char*)dst->value, 0, CONFIG_STRING_SLOT_SIZE);
				dst->default_value = tsc_malloc(CONFIG_STRING_SLOT_SIZE);
				tsc_memset((char*)dst->default_value, 0, CONFIG_STRING_SLOT_SIZE);
            }
            break;
        }
        default:
            break;
    }
    return true;
}

/**
 * @brief Check current config profile is backup in spi flash file
 * @param lfs Pointer to littlefs filesystem
 * @param path Path to load binary file
 * @param profile Pointer to config profile
 * @return true on success, false otherwise
 */
bool config_is_profile_backup(lfs_t *lfs, const char *path, config_profile_t *profile)
{
    lfs_file_t file;
	config_profile_bin_t read_profile;

    if (lfs_file_open(lfs, &file, path, LFS_O_RDONLY) < 0) {
    	tsc_kprintf("%s: Error file not found!, Path: [%s]\r\n", __FUNCTION__, path);
        return false;
    }

    do {
		ssize_t r = lfs_file_read(lfs, &file, &read_profile, sizeof(read_profile));
		if (r <= 0) break;
		if ((size_t)r != sizeof(read_profile)) break;

		uint32_t crc = crc32_compute((uint8_t *)&read_profile, sizeof(read_profile) - sizeof(read_profile.crc32));
		if (crc != read_profile.crc32) {
			tsc_kprintf("%s: Error CRC check! Profile name: [%s]\r\n", __FUNCTION__, profile->profile_name);
			break;
		}
    } while(0);

    lfs_file_close(lfs, &file);
    if(profile->magic_number == read_profile.magic_number) return true;

    tsc_kprintf("%s: Error Profile name: [%s] not backup to flash!\r\n", __FUNCTION__, profile->profile_name);
    return false;
}

/**
 * @brief Save current config profile to spi flash file
 * @param lfs Pointer to littlefs filesystem
 * @param path Path to store binary file
 * @param profile Pointer to config profile
 * @return true on success, false otherwise
 */
bool config_save_profile_file(lfs_t *lfs, const char *path, config_profile_t *profile) {
    lfs_file_t file;
    if (lfs_file_open(lfs, &file, path, LFS_O_WRONLY | LFS_O_CREAT | LFS_O_TRUNC) < 0)
        return false;

    next_string_index = 0;

    config_profile_bin_t write_profile;
    write_profile.magic_number = profile->magic_number;
    tsc_strncpy(write_profile.name, profile->profile_name, CONFIG_NAME_MAX_LEN);
    write_profile.num_entries = profile->num_entries;
    write_profile.crc32 = crc32_compute((uint8_t *)&write_profile, sizeof(write_profile) - sizeof(write_profile.crc32));

    lfs_file_write(lfs, &file, &write_profile, sizeof(write_profile));
    lfs_file_seek(lfs, &file, CONFIG_PROFILE_SLOT_SIZE, LFS_SEEK_SET);

    for (uint32_t i = 0; i < profile->num_entries; i++) {
        config_entry_bin_t bin;
        if (config_entry_to_bin(profile->entries[i], &bin)) {
        	if(bin.type == CFG_TYPE_STRING) {
				config_save_string_file(lfs, profile, bin.value.str_index_val, (char *)profile->entries[i]->value);
				config_save_string_file(lfs, profile, bin.default_value.str_index_val, (char *)profile->entries[i]->default_value);
        	}
        }
        else
        	continue;
        bin.crc32 = crc32_compute((uint8_t *)&bin, sizeof(bin) - 4);
        lfs_file_write(lfs, &file, &bin, sizeof(bin));
    }

    lfs_file_close(lfs, &file);
    return true;
}

/**
 * @brief Load config profile from spi flash file
 * @param lfs Pointer to littlefs filesystem
 * @param path Path to load binary file
 * @param profile Pointer to config profile
 * @return true on success and valid CRC, false otherwise
 */
bool config_load_profile_file(lfs_t *lfs, const char *path, config_profile_t *profile) {
	bool error = true;
    lfs_file_t file;
    uint32_t num_entries=0;

    if (lfs_file_open(lfs, &file, path, LFS_O_RDONLY) < 0)
        return false;

    config_clear_profile(profile);

    lfs_file_seek(lfs, &file, CONFIG_PROFILE_SLOT_SIZE, LFS_SEEK_SET);

    while (1) {
        config_entry_bin_t bin;
        config_entry_t *entry;

        ssize_t r = lfs_file_read(lfs, &file, &bin, sizeof(bin));
        if (r <= 0) break;
        if((size_t)r != sizeof(bin)) {
        	error = false;
        	break;
        }

        uint32_t crc = crc32_compute((uint8_t *)&bin, sizeof(bin) - 4);
        if (crc != bin.crc32) {
        	error = false;
        	break;
        }

        if(bin.is_dynamic) {
        	entry = tsc_malloc(sizeof(config_entry_t));
        	tsc_memset(entry, 0, sizeof(config_entry_t));
        }
        else {
        	entry = profile->entries[num_entries++];
        }

		if (config_bin_to_entry(&bin, entry, bin.is_dynamic)) {
			if(bin.type == CFG_TYPE_STRING) {
				config_load_string_file(lfs, profile, bin.value.str_index_val, entry->value, CONFIG_STRING_SLOT_SIZE);
				if(bin.is_dynamic)
					config_load_string_file(lfs, profile, bin.default_value.str_index_val, (char *)entry->default_value, CONFIG_STRING_SLOT_SIZE);
			}
		}
		else {
			if (bin.is_dynamic) tsc_free(entry);
			error = false;
			break;
		}

        if(bin.is_dynamic) config_add_entry(entry);
        if(num_entries > profile->num_static_entries) {
        	error = false;
        	break;
        }
    }

    lfs_file_close(lfs, &file);
    return error;
}

#endif /* ENABLE_LITTLEFS */

// ---------- (13) END OF SOURCE FILE IMPLEMENTATION ------------------------------------------------- //
// SOURCE FORMAT REVISION B


#endif /* CSP_ENABLE_CONFIG */
