/*
 * Project: File V3 for TSCNarit
 * Author: Chaturong Khachaban
 * Description: Flight-grade embedded system module
 * 
 * This file consolidates:
 * - file_core_types.h (Core type definitions)
 * - file_core_vfs.h (VFS interface and API functions)
 */

#ifndef _FILE_CORE_H
#define _FILE_CORE_H

#if defined __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <ff.h>

#include "app_modules_config.h"

/* ============================================================================
 * SECTION 1: Core Type Definitions (from file_core_types.h)
 * ========================================================================= */

#define FM_COPY_BUFFER_SIZE 512U
#define FM_COPY_MAX_DEPTH 16U

#define FM_MAX_PATH_LEN 128U
#define FM_SD_PREFIX "/D/"
#define FM_NOR_PREFIX "/C/"
#define FM_MAX_VFS_ENTRIES 2U
#define FM_MAX_NAME_LEN 256U

#define FM_O_READ 0x01U
#define FM_O_WRITE 0x02U
#define FM_O_RDWR (FM_O_READ | FM_O_WRITE)
#define FM_O_CREATE 0x04U
#define FM_O_APPEND 0x08U

typedef enum {
	FM_OK = 0U, FM_ERR_IO = 1U, FM_ERR_NOENT = 2U, FM_ERR_NOSPC = 3U, FM_ERR_INVAL = 4U, FM_ERR_EXIST = 5U, FM_ERR_PERM = 6U, FM_ERR_LOCK = 7U, FM_ERR_BUSY = 8U, FM_ERR_TOO_MANY_OPEN = 9U, FM_ERR_GENERIC = 10U, FM_ERR_NOT_SUPPORTED = 11U, FM_ERR_XFS_RENAME = 12U, FM_ERR_NAMETOOLONG = 13U, FM_ERR_NOMEM = 14U
} fm_err_t;

typedef struct fm_file_info_s {
	char name[FM_MAX_NAME_LEN];
	uint32_t size;
	uint16_t attributes;
	uint16_t date;
	uint16_t time;
	uint8_t is_dir;
} fm_file_info_t;

typedef struct fm_file_handle_s fm_file_handle_t;
typedef struct fm_dir_handle_s fm_dir_handle_t;

typedef struct fm_fs_stats_s {
	uint64_t total_bytes;
	uint64_t free_bytes;
	uint64_t used_bytes;
} fm_fs_stats_t;

typedef struct fm_vfs_s {
	fm_err_t (*stat)(const char *path, fm_file_info_t *info);
	fm_err_t (*mkdir)(const char *path);
	fm_err_t (*remove)(const char *path);
	fm_err_t (*rename)(const char *oldpath, const char *newpath);
	fm_err_t (*open)(fm_file_handle_t **fp, const char *path, uint8_t mode);
	fm_err_t (*read)(fm_file_handle_t *fp, void *buff, uint32_t btr, uint32_t *br);
	fm_err_t (*write)(fm_file_handle_t *fp, const void *buff, uint32_t btw, uint32_t *bw);
	fm_err_t (*seek)(fm_file_handle_t *fp, uint32_t offset);
	fm_err_t (*sync)(fm_file_handle_t *fp);
	fm_err_t (*close)(fm_file_handle_t **fp);
	fm_err_t (*opendir)(fm_dir_handle_t **dp, const char *path);
	fm_err_t (*readdir)(fm_dir_handle_t *dp, fm_file_info_t *info);
	fm_err_t (*closedir)(fm_dir_handle_t **dp);
	fm_err_t (*size)(fm_file_handle_t *fp, uint32_t *out_size);
	fm_err_t (*statfs)(const char *path, fm_fs_stats_t *stats);
} fm_vfs_t;

typedef enum {
	FM_FS_TYPE_FATFS = 0U, FM_FS_TYPE_LITTLEFS = 1U, FM_FS_TYPE_UNKNOWN = 2U
} fm_fs_type_t;

struct fm_file_handle_s {
	const fm_vfs_t *vfs;
	fm_fs_type_t fs_type;
	void *impl;
};

struct fm_dir_handle_s {
	const fm_vfs_t *vfs;
	fm_fs_type_t fs_type;
	void *impl;
};

typedef fm_err_t (*fm_iterate_cb_t)(const fm_file_info_t *entry, void *user_ctx);

/* ============================================================================
 * SECTION 2: VFS Interface and API Functions (from file_core_vfs.h)
 * ========================================================================= */

typedef struct lfs lfs_t;

extern lfs_t *g_fm_lfs;

fm_err_t fm_register_vfs(const char *prefix, const fm_vfs_t *vfs, fm_fs_type_t fs_type);
fm_err_t fm_file_manager_init(lfs_t *lfs_ptr);
fm_err_t file_manager_init(void);
fm_err_t fm_echo_to_file(const char *path, const char *str);
const fm_vfs_t* fm_get_vfs_with_type(const char *path, fm_fs_type_t *out_fs_type);
const fm_vfs_t* fm_get_vfs(const char *path);
const char* fm_strip_prefix(const char *path);
const fm_vfs_t* fm_get_fatfs_vfs(void);
const fm_vfs_t* fm_get_littlefs_vfs(void);
fm_err_t fm_open(const char *path, uint8_t mode, fm_file_handle_t **fp);
fm_err_t fm_read(fm_file_handle_t *fp, void *buf, uint32_t len, uint32_t *br);
fm_err_t fm_write(fm_file_handle_t *fp, const void *buf, uint32_t len, uint32_t *bw);
fm_err_t fm_seek(fm_file_handle_t *fp, uint32_t offset);
fm_err_t fm_sync(fm_file_handle_t *fp);
fm_err_t fm_close(fm_file_handle_t **fp);
fm_err_t fm_stat(const char *path, fm_file_info_t *info);
fm_err_t fm_mkdir(const char *path);
fm_err_t fm_rm(const char *path);
fm_err_t fm_mv(const char *src, const char *dst);
fm_err_t fm_copy(const char *src, const char *dst);
fm_err_t fm_opendir(const char *path, fm_dir_handle_t **dp);
fm_err_t fm_readdir(fm_dir_handle_t *dp, fm_file_info_t *info);
fm_err_t fm_closedir(fm_dir_handle_t **dp);
bool fm_path_exists(const char *path);
bool fm_path_is_dir(const char *path);

extern const fm_vfs_t fm_fatfs_vfs;
extern const fm_vfs_t fm_littlefs_vfs;

#if defined __cplusplus
}
#endif

#endif
