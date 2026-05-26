#include "app_modules_config.h"
#if CSP_ENABLE_FILE

/*
 * Project: File V3 for TSCNarit
 * Author: Chaturong Khachaban
 * Description: Flight-grade embedded system module
 * 
 * This file consolidates:
 * - fm_fatfs_backend.c (FatFS backend implementation)
 * - fm_littlefs_backend.c (LittleFS backend implementation)
 */

#include "file_core.h"
#include "ff.h"
#if ENABLE_LITTLEFS
#include <lfs.h>
#endif
#include <string.h>
#include <stdlib.h>

#if ENABLE_LITTLEFS
extern lfs_t *g_fm_lfs;
#endif

/* ============================================================================
 * SECTION 1: FatFS Backend Implementation
 * ========================================================================= */

static fm_err_t fresult_to_fm_err(FRESULT fr) {
	switch (fr) {
	case FR_OK:
		return FM_OK;
	case FR_DISK_ERR:
	case FR_INT_ERR:
	case FR_NOT_READY:
	case FR_INVALID_DRIVE:
	case FR_NOT_ENABLED:
	case FR_NO_FILESYSTEM:
	case FR_TIMEOUT:
	default:
		return FM_ERR_IO;
	case FR_NO_FILE:
	case FR_NO_PATH:
		return FM_ERR_NOENT;
	case FR_INVALID_NAME:
	case FR_INVALID_OBJECT:
		return FM_ERR_INVAL;
	case FR_DENIED:
	case FR_WRITE_PROTECTED:
		return FM_ERR_PERM;
	case FR_EXIST:
		return FM_ERR_EXIST;
	}
}

static FIL* get_fil(fm_file_handle_t *handle) {
	if (handle == NULL || handle->impl == NULL) {
		return NULL;
	}
	return (FIL*) handle->impl;
}

static DIR* get_dir(fm_dir_handle_t *handle) {
	if (handle == NULL || handle->impl == NULL) {
		return NULL;
	}
	return (DIR*) handle->impl;
}

fm_err_t fm_fatfs_open(fm_file_handle_t **out, const char *path, uint8_t mode) {
	fm_file_handle_t *handle;
	FIL *fil;
	BYTE fatfs_mode;

	if (out == NULL || path == NULL) {
		return FM_ERR_INVAL;
	}

	handle = (fm_file_handle_t*) malloc(sizeof(fm_file_handle_t));
	if (handle == NULL) {
		return FM_ERR_BUSY;
	}

	fil = (FIL*) malloc(sizeof(FIL));
	if (fil == NULL) {
		free(handle);
		return FM_ERR_BUSY;
	}

	fatfs_mode = 0U;
	if ((mode & FM_O_READ) != 0U) {
		fatfs_mode |= FA_READ;
	}
	if ((mode & FM_O_WRITE) != 0U) {
		fatfs_mode |= FA_WRITE;
	}
	if ((mode & FM_O_CREATE) != 0U) {
		fatfs_mode |= FA_OPEN_ALWAYS;
	}
	if ((mode & FM_O_APPEND) != 0U) {
		fatfs_mode |= FA_OPEN_APPEND;
	}

	if (f_open(fil, path, fatfs_mode) != FR_OK) {
		free(fil);
		free(handle);
		return FM_ERR_IO;
	}

	handle->vfs = NULL;
	handle->fs_type = FM_FS_TYPE_FATFS;
	handle->impl = fil;

	*out = handle;
	return FM_OK;
}

fm_err_t fm_fatfs_close(fm_file_handle_t **handle) {
	FIL *fil;

	if (handle == NULL || *handle == NULL) {
		return FM_OK;
	}

	fil = get_fil(*handle);
	if (fil != NULL) {
		(void) f_close(fil);
		free(fil);
		(*handle)->impl = NULL;
	}

	free(*handle);
	*handle = NULL;

	return FM_OK;
}

fm_err_t fm_fatfs_read(fm_file_handle_t *handle, void *buff, uint32_t btr, uint32_t *br) {
	FIL *fil;
	UINT bytes_read;
	FRESULT fr;

	if (handle == NULL || buff == NULL) {
		return FM_ERR_INVAL;
	}

	fil = get_fil(handle);
	if (fil == NULL) {
		return FM_ERR_INVAL;
	}

	fr = f_read(fil, buff, btr, &bytes_read);

	if (br != NULL) {
		*br = bytes_read;
	}

	return fresult_to_fm_err(fr);
}

fm_err_t fm_fatfs_write(fm_file_handle_t *handle, const void *buff, uint32_t btw, uint32_t *bw) {
	FIL *fil;
	UINT bytes_written;
	FRESULT fr;

	if (handle == NULL || buff == NULL) {
		return FM_ERR_INVAL;
	}

	fil = get_fil(handle);
	if (fil == NULL) {
		return FM_ERR_INVAL;
	}

	fr = f_write(fil, buff, btw, &bytes_written);

	if (bw != NULL) {
		*bw = bytes_written;
	}

	return fresult_to_fm_err(fr);
}

fm_err_t fm_fatfs_seek(fm_file_handle_t *handle, uint32_t offset) {
	FIL *fil;

	if (handle == NULL) {
		return FM_ERR_INVAL;
	}

	fil = get_fil(handle);
	if (fil == NULL) {
		return FM_ERR_INVAL;
	}

	return fresult_to_fm_err(f_lseek(fil, offset));
}

fm_err_t fm_fatfs_size(fm_file_handle_t *handle, uint32_t *out_size) {
	FIL *fil;

	if (handle == NULL || out_size == NULL) {
		return FM_ERR_INVAL;
	}

	fil = get_fil(handle);
	if (fil == NULL) {
		return FM_ERR_INVAL;
	}

	*out_size = f_size(fil);
	return FM_OK;
}

fm_err_t fm_fatfs_sync(fm_file_handle_t *handle) {
	FIL *fil;

	if (handle == NULL) {
		return FM_ERR_INVAL;
	}

	fil = get_fil(handle);
	if (fil == NULL) {
		return FM_ERR_INVAL;
	}

	return fresult_to_fm_err(f_sync(fil));
}

fm_err_t fm_fatfs_stat(const char *path, fm_file_info_t *info) {
	FILINFO fi;
	FRESULT fr;

	if (path == NULL || info == NULL) {
		return FM_ERR_INVAL;
	}

	fr = f_stat(path, &fi);
	if (fr != FR_OK) {
		return fresult_to_fm_err(fr);
	}

	info->size = fi.fsize;
	info->is_dir = (fi.fattrib & AM_DIR) ? 1U : 0U;
	info->attributes = (uint8_t) fi.fattrib;
	info->date = fi.fdate;
	info->time = fi.ftime;

	(void) strncpy(info->name, fi.fname, FM_MAX_NAME_LEN - 1U);
	info->name[FM_MAX_NAME_LEN - 1U] = '\0';

	return FM_OK;
}

fm_err_t fm_fatfs_mkdir(const char *path) {
	if (path == NULL) {
		return FM_ERR_INVAL;
	}

	return fresult_to_fm_err(f_mkdir(path));
}

fm_err_t fm_fatfs_remove(const char *path) {
	FILINFO fi;
	FRESULT fr;
	DIR dir;
	char full_path[FM_MAX_PATH_LEN];

	if (path == NULL) {
		return FM_ERR_INVAL;
	}

	fr = f_stat(path, &fi);
	if (fr != FR_OK) {
		return fresult_to_fm_err(fr);
	}

	if ((fi.fattrib & AM_DIR) != 0U) {
		fr = f_opendir(&dir, path);
		if (fr != FR_OK) {
			return fresult_to_fm_err(fr);
		}

		while (1) {
			fr = f_readdir(&dir, &fi);
			if (fr != FR_OK || fi.fname[0] == 0) {
				break;
			}

			if ((strcmp(fi.fname, ".") == 0) || (strcmp(fi.fname, "..") == 0)) {
				continue;
			}

			size_t path_len = strlen(path);
			size_t name_len = strlen(fi.fname);

			if ((path_len + 1U + name_len + 1U) > sizeof(full_path))
			{
			    return FM_ERR_NAMETOOLONG;
			}

			(void)snprintf(full_path, sizeof(full_path), "%s/%s", path, fi.fname);

			if ((fi.fattrib & AM_DIR) != 0U) {
				(void) fm_fatfs_remove(full_path);
			} else {
				(void) f_unlink(full_path);
			}
		}

		(void) f_closedir(&dir);
	}

	return fresult_to_fm_err(f_unlink(path));
}

fm_err_t fm_fatfs_rename(const char *oldpath, const char *newpath) {
	if (oldpath == NULL || newpath == NULL) {
		return FM_ERR_INVAL;
	}

	return fresult_to_fm_err(f_rename(oldpath, newpath));
}

fm_err_t fm_fatfs_statfs(const char *path, fm_fs_stats_t *stats) {
	FATFS *pfs;
	DWORD fre_clust;
	DWORD cluster_size;
	FRESULT fr;

	if (path == NULL || stats == NULL) {
		return FM_ERR_INVAL;
	}

	fr = f_getfree(path, &fre_clust, &pfs);
	if (fr != FR_OK) {
		return fresult_to_fm_err(fr);
	}

	cluster_size = pfs->csize * 512U;
	stats->total_bytes = (pfs->n_fatent - 2U) * cluster_size;
	stats->free_bytes = fre_clust * cluster_size;
	stats->used_bytes = stats->total_bytes - stats->free_bytes;

	return FM_OK;
}

fm_err_t fm_fatfs_opendir(fm_dir_handle_t **out, const char *path) {
	fm_dir_handle_t *handle;
	DIR *dir;
	FRESULT fr;

	if (out == NULL || path == NULL) {
		return FM_ERR_INVAL;
	}

	handle = (fm_dir_handle_t*) malloc(sizeof(fm_dir_handle_t));
	if (handle == NULL) {
		return FM_ERR_BUSY;
	}

	dir = (DIR*) malloc(sizeof(DIR));
	if (dir == NULL) {
		free(handle);
		return FM_ERR_BUSY;
	}

	fr = f_opendir(dir, path);
	if (fr != FR_OK) {
		free(dir);
		free(handle);
		return fresult_to_fm_err(fr);
	}

	handle->vfs = NULL;
	handle->fs_type = FM_FS_TYPE_FATFS;
	handle->impl = dir;

	*out = handle;
	return FM_OK;
}

fm_err_t fm_fatfs_closedir(fm_dir_handle_t **handle) {
	DIR *dir;

	if (handle == NULL || *handle == NULL) {
		return FM_OK;
	}

	dir = get_dir(*handle);
	if (dir != NULL) {
		(void) f_closedir(dir);
		free(dir);
		(*handle)->impl = NULL;
	}

	free(*handle);
	*handle = NULL;

	return FM_OK;
}

fm_err_t fm_fatfs_readdir(fm_dir_handle_t *handle, fm_file_info_t *info) {
	DIR *dir;
	FILINFO fi;
	FRESULT fr;

	if (handle == NULL || info == NULL) {
		return FM_ERR_INVAL;
	}

	dir = get_dir(handle);
	if (dir == NULL) {
		return FM_ERR_INVAL;
	}

	fr = f_readdir(dir, &fi);
	if (fr != FR_OK) {
		return fresult_to_fm_err(fr);
	}

	if (fi.fname[0] == 0) {
		info->name[0] = '\0';
		return FM_OK;
	}

	info->size = fi.fsize;
	info->is_dir = (fi.fattrib & AM_DIR) ? 1U : 0U;
	info->attributes = (uint8_t) fi.fattrib;
	info->date = fi.fdate;
	info->time = fi.ftime;

	(void) strncpy(info->name, fi.fname, FM_MAX_NAME_LEN - 1U);
	info->name[FM_MAX_NAME_LEN - 1U] = '\0';

	return FM_OK;
}

const fm_vfs_t fm_fatfs_vfs = { .stat = fm_fatfs_stat, .mkdir = fm_fatfs_mkdir, .remove = fm_fatfs_remove, .rename = fm_fatfs_rename, .open = fm_fatfs_open, .read = fm_fatfs_read, .write = fm_fatfs_write, .seek = fm_fatfs_seek, .sync = fm_fatfs_sync, .close = fm_fatfs_close, .opendir = fm_fatfs_opendir, .readdir = fm_fatfs_readdir, .closedir = fm_fatfs_closedir, .size = fm_fatfs_size, .statfs = fm_fatfs_statfs };

/* ============================================================================
 * SECTION 2: LittleFS Backend Implementation
 * ========================================================================= */

#if ENABLE_LITTLEFS

typedef struct lfs_file_impl_s {
	lfs_file_t fil;
} lfs_file_impl_t;

typedef struct lfs_dir_impl_s {
	lfs_dir_t dir;
} lfs_dir_impl_t;

static fm_err_t lfs_result_to_fm_err(int32_t rc) {
	if (rc == 0) {
		return FM_OK;
	}

	switch (rc) {
	case LFS_ERR_NOENT:
		return FM_ERR_NOENT;
	case LFS_ERR_EXIST:
		return FM_ERR_EXIST;
	case LFS_ERR_NOTDIR:
	case LFS_ERR_ISDIR:
	case LFS_ERR_INVAL:
		return FM_ERR_INVAL;
	case LFS_ERR_NOSPC:
		return FM_ERR_NOSPC;
	case LFS_ERR_CORRUPT:
	case LFS_ERR_IO:
	default:
		return FM_ERR_IO;
	}
}

static int32_t fm_to_lfs_flags(uint8_t fm_mode) {
	int32_t lfs_flags = 0;

	if ((fm_mode & FM_O_READ) != 0U) {
		lfs_flags |= LFS_O_RDONLY;
	}
	if ((fm_mode & FM_O_WRITE) != 0U) {
		lfs_flags |= LFS_O_WRONLY;
	}
	if ((fm_mode & FM_O_READ) != 0U && (fm_mode & FM_O_WRITE) != 0U) {
		lfs_flags |= LFS_O_RDWR;
	}

	if ((fm_mode & FM_O_CREATE) != 0U || (fm_mode & FA_CREATE_ALWAYS) != 0U) {
		lfs_flags |= LFS_O_CREAT | LFS_O_TRUNC;
	}
	if ((fm_mode & FA_OPEN_ALWAYS) != 0U) {
		lfs_flags |= LFS_O_CREAT;
	}

	if ((fm_mode & FM_O_APPEND) != 0U || (fm_mode & FA_OPEN_APPEND) != 0U) {
		lfs_flags |= LFS_O_APPEND;
	}

	return lfs_flags;
}

static fm_err_t lfs_vfs_stat(const char *path, fm_file_info_t *info) {
	struct lfs_info lfs_info;
	int32_t rc;

	if (path == NULL) {
		return FM_ERR_INVAL;
	}

	if (g_fm_lfs == NULL) {
		return FM_ERR_IO;
	}

	rc = lfs_stat(g_fm_lfs, path, &lfs_info);
	if (rc != 0) {
		return lfs_result_to_fm_err(rc);
	}

	if (info != NULL) {
		info->size = lfs_info.size;
		info->attributes = 0U;
		info->date = 0U;
		info->time = 0U;
		info->is_dir = (lfs_info.type == LFS_TYPE_DIR) ? 1U : 0U;

		(void) strncpy(info->name, lfs_info.name, FM_MAX_NAME_LEN - 1U);
		info->name[FM_MAX_NAME_LEN - 1U] = '\0';
	}

	return FM_OK;
}

static fm_err_t lfs_vfs_mkdir(const char *path) {
	int32_t rc;

	if (path == NULL) {
		return FM_ERR_INVAL;
	}

	if (g_fm_lfs == NULL) {
		return FM_ERR_IO;
	}

	rc = lfs_mkdir(g_fm_lfs, path);
	return lfs_result_to_fm_err(rc);
}

static fm_err_t lfs_vfs_remove(const char *path) {
	int32_t rc;

	if (path == NULL) {
		return FM_ERR_INVAL;
	}

	if (g_fm_lfs == NULL) {
		return FM_ERR_IO;
	}

	rc = lfs_remove(g_fm_lfs, path);
	return lfs_result_to_fm_err(rc);
}

static fm_err_t lfs_vfs_rename(const char *oldpath, const char *newpath) {
	int32_t rc;

	if ((oldpath == NULL) || (newpath == NULL)) {
		return FM_ERR_INVAL;
	}

	if (g_fm_lfs == NULL) {
		return FM_ERR_IO;
	}

	rc = lfs_rename(g_fm_lfs, oldpath, newpath);
	return lfs_result_to_fm_err(rc);
}

static fm_err_t lfs_vfs_open(fm_file_handle_t **fp, const char *path, uint8_t mode) {
	int32_t rc;
	int32_t lfs_flags;
	fm_file_handle_t *handle;
	lfs_file_impl_t *impl;

	if ((fp == NULL) || (path == NULL)) {
		return FM_ERR_INVAL;
	}

	if (g_fm_lfs == NULL) {
		return FM_ERR_IO;
	}

	*fp = NULL;

	handle = (fm_file_handle_t*) malloc(sizeof(fm_file_handle_t));
	if (handle == NULL) {
		return FM_ERR_BUSY;
	}

	impl = (lfs_file_impl_t*) malloc(sizeof(lfs_file_impl_t));
	if (impl == NULL) {
		free(handle);
		return FM_ERR_BUSY;
	}

	lfs_flags = fm_to_lfs_flags(mode);
	rc = lfs_file_open(g_fm_lfs, &impl->fil, path, lfs_flags);
	if (rc < 0) {
		free(impl);
		free(handle);
		return lfs_result_to_fm_err(rc);
	}

	handle->vfs = NULL;
	handle->fs_type = FM_FS_TYPE_LITTLEFS;
	handle->impl = impl;

	*fp = handle;
	return FM_OK;
}

static fm_err_t lfs_vfs_read(fm_file_handle_t *fp, void *buff, uint32_t btr, uint32_t *br) {
	lfs_ssize_t bytes_read;
	lfs_file_impl_t *impl;

	if (fp == NULL) {
		return FM_ERR_INVAL;
	}

	if (fp->fs_type != FM_FS_TYPE_LITTLEFS) {
		return FM_ERR_INVAL;
	}

	if (g_fm_lfs == NULL) {
		return FM_ERR_IO;
	}

	impl = (lfs_file_impl_t*) fp->impl;
	if (impl == NULL) {
		return FM_ERR_INVAL;
	}

	bytes_read = lfs_file_read(g_fm_lfs, &impl->fil, buff, (lfs_size_t) btr);

	if (bytes_read >= 0) {
		if (br != NULL) {
			*br = (uint32_t) ((lfs_size_t) bytes_read);
		}
		return FM_OK;
	}

	return FM_ERR_IO;
}

static fm_err_t lfs_vfs_write(fm_file_handle_t *fp, const void *buff, uint32_t btw, uint32_t *bw) {
	lfs_ssize_t bytes_written;
	lfs_file_impl_t *impl;

	if (fp == NULL) {
		return FM_ERR_INVAL;
	}

	if (fp->fs_type != FM_FS_TYPE_LITTLEFS) {
		return FM_ERR_INVAL;
	}

	if (g_fm_lfs == NULL) {
		return FM_ERR_IO;
	}

	impl = (lfs_file_impl_t*) fp->impl;
	if (impl == NULL) {
		return FM_ERR_INVAL;
	}

	bytes_written = lfs_file_write(g_fm_lfs, &impl->fil, buff, (lfs_size_t) btw);

	if (bytes_written >= 0) {
		if (bw != NULL) {
			*bw = (uint32_t) ((lfs_size_t) bytes_written);
		}
		return FM_OK;
	}

	return FM_ERR_IO;
}

static fm_err_t lfs_vfs_seek(fm_file_handle_t *fp, uint32_t offset) {
	lfs_soff_t result;
	lfs_file_impl_t *impl;

	if (fp == NULL) {
		return FM_ERR_INVAL;
	}

	if (fp->fs_type != FM_FS_TYPE_LITTLEFS) {
		return FM_ERR_INVAL;
	}

	if (g_fm_lfs == NULL) {
		return FM_ERR_IO;
	}

	impl = (lfs_file_impl_t*) fp->impl;
	if (impl == NULL) {
		return FM_ERR_INVAL;
	}

	result = lfs_file_seek(g_fm_lfs, &impl->fil, (lfs_soff_t) offset, LFS_SEEK_SET);

	return (result >= 0) ? FM_OK : FM_ERR_IO;
}

static fm_err_t lfs_vfs_size(fm_file_handle_t *fp, uint32_t *out_size) {
	lfs_file_impl_t *impl;
	lfs_ssize_t size;

	if (fp == NULL || out_size == NULL) {
		return FM_ERR_INVAL;
	}

	if (fp->fs_type != FM_FS_TYPE_LITTLEFS) {
		return FM_ERR_INVAL;
	}

	if (g_fm_lfs == NULL) {
		return FM_ERR_IO;
	}

	impl = (lfs_file_impl_t*) fp->impl;
	if (impl == NULL) {
		return FM_ERR_INVAL;
	}

	size = lfs_file_size(g_fm_lfs, &impl->fil);
	if (size < 0) {
		return FM_ERR_IO;
	}

	*out_size = (uint32_t) size;
	return FM_OK;
}

static fm_err_t lfs_vfs_sync(fm_file_handle_t *fp) {
	lfs_file_impl_t *impl;
	int32_t rc;

	if (fp == NULL) {
		return FM_ERR_INVAL;
	}

	if (fp->fs_type != FM_FS_TYPE_LITTLEFS) {
		return FM_ERR_INVAL;
	}

	if (g_fm_lfs == NULL) {
		return FM_ERR_IO;
	}

	impl = (lfs_file_impl_t*) fp->impl;
	if (impl == NULL) {
		return FM_ERR_INVAL;
	}

	rc = lfs_file_sync(g_fm_lfs, &impl->fil);
	return lfs_result_to_fm_err(rc);
}

static fm_err_t lfs_vfs_close(fm_file_handle_t **fp) {
	fm_file_handle_t *handle;
	lfs_file_impl_t *impl;
	int32_t rc;

	if ((fp == NULL) || (*fp == NULL)) {
		return FM_OK;
	}

	handle = *fp;

	if (handle->fs_type != FM_FS_TYPE_LITTLEFS) {
		return FM_ERR_INVAL;
	}

	if (g_fm_lfs == NULL) {
		free(handle);
		*fp = NULL;
		return FM_ERR_IO;
	}

	impl = (lfs_file_impl_t*) handle->impl;
	if (impl != NULL) {
		rc = lfs_file_close(g_fm_lfs, &impl->fil);
		free(impl);
	} else {
		rc = 0;
	}

	free(handle);
	*fp = NULL;

	return lfs_result_to_fm_err(rc);
}

static fm_err_t lfs_vfs_opendir(fm_dir_handle_t **dp, const char *path) {
	int32_t rc;
	fm_dir_handle_t *handle;
	lfs_dir_impl_t *impl;

	if ((dp == NULL) || (path == NULL)) {
		return FM_ERR_INVAL;
	}

	if (g_fm_lfs == NULL) {
		return FM_ERR_IO;
	}

	*dp = NULL;

	handle = (fm_dir_handle_t*) malloc(sizeof(fm_dir_handle_t));
	if (handle == NULL) {
		return FM_ERR_BUSY;
	}

	impl = (lfs_dir_impl_t*) malloc(sizeof(lfs_dir_impl_t));
	if (impl == NULL) {
		free(handle);
		return FM_ERR_BUSY;
	}

	rc = lfs_dir_open(g_fm_lfs, &impl->dir, path);
	if (rc < 0) {
		free(impl);
		free(handle);
		return lfs_result_to_fm_err(rc);
	}

	handle->vfs = NULL;
	handle->fs_type = FM_FS_TYPE_LITTLEFS;
	handle->impl = impl;

	*dp = handle;
	return FM_OK;
}

static fm_err_t lfs_vfs_readdir(fm_dir_handle_t *dp, fm_file_info_t *info) {
	struct lfs_info lfs_info;
	lfs_dir_impl_t *impl;
	int32_t rc;

	if (dp == NULL) {
		return FM_ERR_INVAL;
	}

	if (dp->fs_type != FM_FS_TYPE_LITTLEFS) {
		return FM_ERR_INVAL;
	}

	if (g_fm_lfs == NULL) {
		return FM_ERR_IO;
	}

	impl = (lfs_dir_impl_t*) dp->impl;
	if (impl == NULL) {
		return FM_ERR_INVAL;
	}

	rc = lfs_dir_read(g_fm_lfs, &impl->dir, &lfs_info);

	if (info != NULL) {
		if (rc == 0) {
			info->name[0] = '\0';
		} else if (rc > 0) {
			info->size = lfs_info.size;
			info->attributes = 0U;
			info->date = 0U;
			info->time = 0U;
			info->is_dir = (lfs_info.type == LFS_TYPE_DIR) ? 1U : 0U;

			(void) strncpy(info->name, lfs_info.name, FM_MAX_NAME_LEN - 1U);
			info->name[FM_MAX_NAME_LEN - 1U] = '\0';
		}
	}

	return (rc >= 0) ? FM_OK : FM_ERR_IO;
}

static fm_err_t lfs_vfs_closedir(fm_dir_handle_t **dp) {
	fm_dir_handle_t *handle;
	lfs_dir_impl_t *impl;
	int32_t rc;

	if ((dp == NULL) || (*dp == NULL)) {
		return FM_OK;
	}

	handle = *dp;

	if (handle->fs_type != FM_FS_TYPE_LITTLEFS) {
		return FM_ERR_INVAL;
	}

	if (g_fm_lfs == NULL) {
		free(handle);
		*dp = NULL;
		return FM_ERR_IO;
	}

	impl = (lfs_dir_impl_t*) handle->impl;
	if (impl != NULL) {
		rc = lfs_dir_close(g_fm_lfs, &impl->dir);
		free(impl);
	} else {
		rc = 0;
	}

	free(handle);
	*dp = NULL;

	return lfs_result_to_fm_err(rc);
}

static fm_err_t lfs_vfs_statfs(const char *path, fm_fs_stats_t *stats) {
	(void) path;

	if (stats == NULL) {
		return FM_ERR_INVAL;
	}

	if (g_fm_lfs == NULL) {
		return FM_ERR_IO;
	}

	if (g_fm_lfs->cfg != NULL) {
		uint32_t block_size = g_fm_lfs->cfg->block_size;
		uint32_t total_blocks = g_fm_lfs->cfg->block_count;

		stats->total_bytes = (uint64_t) total_blocks * block_size;
		stats->used_bytes = stats->total_bytes - stats->used_bytes;
		stats->free_bytes = stats->total_bytes - stats->used_bytes;
	} else {
		stats->total_bytes = 0;
		stats->used_bytes = 0;
		stats->free_bytes = 0;
	}

	return FM_OK;
}

const fm_vfs_t fm_littlefs_vfs = { .stat = lfs_vfs_stat, .mkdir = lfs_vfs_mkdir, .remove = lfs_vfs_remove, .rename = lfs_vfs_rename, .open = lfs_vfs_open, .read = lfs_vfs_read, .write = lfs_vfs_write, .seek = lfs_vfs_seek, .sync = lfs_vfs_sync, .close = lfs_vfs_close, .opendir = lfs_vfs_opendir, .readdir = lfs_vfs_readdir, .closedir = lfs_vfs_closedir, .size = lfs_vfs_size, .statfs = lfs_vfs_statfs };

#endif /* ENABLE_LITTLEFS */


#endif /* CSP_ENABLE_FILE */
