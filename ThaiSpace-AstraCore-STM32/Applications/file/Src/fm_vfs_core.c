#include "app_modules_config.h"
#if CSP_ENABLE_FILE

/*
 * Project: File V3 for TSCNarit
 * Author: Chaturong Khachaban
 * Description: Flight-grade embedded system module
 */

#include <string.h>
#include <stdlib.h>
#include <ff.h>
#include "file_core.h"
#if ENABLE_LITTLEFS
#include "s25fl128l_littlefs.h"
#endif

static fm_err_t fm_copy_file(const char *src, const char *dst);
static fm_err_t fm_copy_dir_recursive(const char *src, const char *dst, uint32_t depth);
static fm_err_t fm_rm_recursive(const char *path, uint32_t depth);

typedef struct vfs_entry_s {
	const char *prefix;
	const fm_vfs_t *vfs;
	fm_fs_type_t fs_type;
} vfs_entry_t;

static vfs_entry_t g_vfs_table[FM_MAX_VFS_ENTRIES];
static uint32_t g_vfs_count = 0U;
static FATFS g_fatfs;

#if ENABLE_LITTLEFS
lfs_t *g_fm_lfs = NULL;
#endif

extern const fm_vfs_t fm_fatfs_vfs;
#if ENABLE_LITTLEFS
extern const fm_vfs_t fm_littlefs_vfs;
#endif

static fm_fs_type_t fm_detect_fs_type(const char *path) {
	if (path == NULL) {
		return FM_FS_TYPE_UNKNOWN;
	}

	if (strncmp(path, FM_SD_PREFIX, strlen(FM_SD_PREFIX)) == 0) {
		return FM_FS_TYPE_FATFS;
	}

#if ENABLE_LITTLEFS
	if (strncmp(path, FM_NOR_PREFIX, strlen(FM_NOR_PREFIX)) == 0) {
		return FM_FS_TYPE_LITTLEFS;
	}
#endif

	return FM_FS_TYPE_UNKNOWN;
}

fm_err_t fm_register_vfs(const char *prefix, const fm_vfs_t *vfs, fm_fs_type_t fs_type) {
	if ((prefix == NULL) || (vfs == NULL)) {
		return FM_ERR_INVAL;
	}

	if (g_vfs_count >= FM_MAX_VFS_ENTRIES) {
		return FM_ERR_TOO_MANY_OPEN;
	}

	g_vfs_table[g_vfs_count].prefix = prefix;
	g_vfs_table[g_vfs_count].vfs = vfs;
	g_vfs_table[g_vfs_count].fs_type = fs_type;
	g_vfs_count++;

	return FM_OK;
}

const fm_vfs_t* fm_get_vfs(const char *path) {
	size_t path_len;
	size_t prefix_len;
	uint32_t i;

	if (path == NULL) {
		return NULL;
	}

	path_len = strlen(path);

	for (i = 0U; i < g_vfs_count; i++) {
		prefix_len = strlen(g_vfs_table[i].prefix);
		if ((path_len >= prefix_len) && (strncmp(path, g_vfs_table[i].prefix, prefix_len) == 0)) {
			return g_vfs_table[i].vfs;
		}
	}

	return NULL;
}

const fm_vfs_t* fm_get_vfs_with_type(const char *path, fm_fs_type_t *out_fs_type) {
	size_t path_len;
	size_t prefix_len;
	uint32_t i;

	if (path == NULL) {
		if (out_fs_type != NULL) {
			*out_fs_type = FM_FS_TYPE_UNKNOWN;
		}
		return NULL;
	}

	path_len = strlen(path);

	for (i = 0U; i < g_vfs_count; i++) {
		prefix_len = strlen(g_vfs_table[i].prefix);
		if ((path_len >= prefix_len) && (strncmp(path, g_vfs_table[i].prefix, prefix_len) == 0)) {
			if (out_fs_type != NULL) {
				*out_fs_type = g_vfs_table[i].fs_type;
			}
			return g_vfs_table[i].vfs;
		}
	}

	if (out_fs_type != NULL) {
		*out_fs_type = FM_FS_TYPE_UNKNOWN;
	}
	return NULL;
}

static fm_err_t fm_strip_prefix_internal(const char *path, char *out_path, size_t out_len) {
	size_t path_len;
	size_t prefix_len;
	uint32_t i;
	const char *sub_path;

	if ((path == NULL) || (out_path == NULL) || (out_len == 0U)) {
		return FM_ERR_INVAL;
	}

	path_len = strlen(path);

	for (i = 0U; i < g_vfs_count; i++) {
		prefix_len = strlen(g_vfs_table[i].prefix);
		if ((path_len >= prefix_len) && (strncmp(path, g_vfs_table[i].prefix, prefix_len) == 0)) {
			sub_path = path + prefix_len;
			if (*sub_path == '\0') {
				(void) strncpy(out_path, "/", out_len - 1U);
				out_path[out_len - 1U] = '\0';
			} else {
				(void) strncpy(out_path, sub_path, out_len - 1U);
				out_path[out_len - 1U] = '\0';
			}
			return FM_OK;
		}
	}

	(void) strncpy(out_path, path, out_len - 1U);
	out_path[out_len - 1U] = '\0';
	return FM_OK;
}

const char* fm_strip_prefix(const char *path) {
	static char local_path[FM_MAX_PATH_LEN];

	if (path == NULL) {
		return NULL;
	}

	if (fm_strip_prefix_internal(path, local_path, sizeof(local_path)) == FM_OK) {
		return local_path;
	}

	return path;
}

const fm_vfs_t* fm_get_fatfs_vfs(void) {
	uint32_t i;

	for (i = 0U; i < g_vfs_count; i++) {
		if (g_vfs_table[i].fs_type == FM_FS_TYPE_FATFS) {
			return g_vfs_table[i].vfs;
		}
	}

	return NULL;
}

const fm_vfs_t* fm_get_littlefs_vfs(void) {
#if ENABLE_LITTLEFS
	uint32_t i;

	for (i = 0U; i < g_vfs_count; i++) {
		if (g_vfs_table[i].fs_type == FM_FS_TYPE_LITTLEFS) {
			return g_vfs_table[i].vfs;
		}
	}
#endif
	return NULL;
}

fm_err_t fm_file_manager_init(lfs_t *lfs_ptr) {
	fm_err_t rc;
	FRESULT fr;

	(void) memset(g_vfs_table, 0, sizeof(g_vfs_table));
	g_vfs_count = 0U;
#if ENABLE_LITTLEFS
	g_fm_lfs = lfs_ptr;
#else
	(void) lfs_ptr;
#endif

	rc = fm_register_vfs(FM_SD_PREFIX, &fm_fatfs_vfs, FM_FS_TYPE_FATFS);
	if (rc != FM_OK) {
		tsc_kprintf("Failed to register FATFS VFS: %d\n", rc);
		return rc;
	}

#if ENABLE_LITTLEFS
	if (g_fm_lfs != NULL) {
		rc = fm_register_vfs(FM_NOR_PREFIX, &fm_littlefs_vfs, FM_FS_TYPE_LITTLEFS);
		if (rc != FM_OK) {
			tsc_kprintf("Failed to register LittleFS VFS: %d\n", rc);
			return rc;
		}
	}
#endif

	fr = f_mount(&g_fatfs, "0:", 1U);
	if (fr != FR_OK) {
		tsc_kprintf("FATFS mount failed: %d\n", fr);
		return FM_ERR_IO;
	}
	return FM_OK;
}

fm_err_t file_manager_init(void) {
#if ENABLE_LITTLEFS
	lfs_t *lfs_ptr = s25fl128l_littlefs_get_handle();
	return fm_file_manager_init(lfs_ptr);
#else
	return fm_file_manager_init(NULL);
#endif
}

fm_err_t fm_echo_to_file(const char *path, const char *str) {
	fm_file_handle_t *fp = NULL;
	fm_err_t rc;
	uint32_t bw;
	size_t len;

	if ((path == NULL) || (str == NULL)) {
		return FM_ERR_INVAL;
	}

	len = strlen(str);
	if (len == 0) {
		return FM_ERR_INVAL;
	}

	rc = fm_open(path, FM_O_WRITE | FM_O_CREATE, &fp);
	if (rc != FM_OK) {
		return rc;
	}

	rc = fm_write(fp, str, (uint32_t) len, &bw);
	if (rc != FM_OK) {
		fm_close(&fp);
		return rc;
	}

	rc = fm_close(&fp);
	return rc;
}

fm_err_t fm_open(const char *path, uint8_t mode, fm_file_handle_t **fp) {
	const fm_vfs_t *vfs;
	char local_path[FM_MAX_PATH_LEN];
	fm_err_t rc;
	fm_file_handle_t *handle;

	if ((fp == NULL) || (path == NULL)) {
		return FM_ERR_INVAL;
	}

	*fp = NULL;

	vfs = fm_get_vfs_with_type(path, NULL);
	if (vfs == NULL) {
		return FM_ERR_NOENT;
	}

	rc = fm_strip_prefix_internal(path, local_path, sizeof(local_path));
	if (rc != FM_OK) {
		return rc;
	}

	rc = vfs->open(fp, local_path, mode);
	if (rc != FM_OK) {
		return rc;
	}

	handle = *fp;
	if (handle != NULL) {
		handle->vfs = vfs;
		handle->fs_type = fm_detect_fs_type(path);
	}

	return FM_OK;
}

fm_err_t fm_read(fm_file_handle_t *fp, void *buf, uint32_t len, uint32_t *br) {
	const fm_vfs_t *vfs;

	if (fp == NULL) {
		return FM_ERR_INVAL;
	}

	if (fp->vfs == NULL) {
		return FM_ERR_INVAL;
	}

	vfs = fp->vfs;
	return vfs->read(fp, buf, len, br);
}

fm_err_t fm_write(fm_file_handle_t *fp, const void *buf, uint32_t len, uint32_t *bw) {
	const fm_vfs_t *vfs;

	if (fp == NULL) {
		return FM_ERR_INVAL;
	}

	if (fp->vfs == NULL) {
		return FM_ERR_INVAL;
	}

	vfs = fp->vfs;
	return vfs->write(fp, buf, len, bw);
}

fm_err_t fm_seek(fm_file_handle_t *fp, uint32_t offset) {
	const fm_vfs_t *vfs;

	if (fp == NULL) {
		return FM_ERR_INVAL;
	}

	if (fp->vfs == NULL) {
		return FM_ERR_INVAL;
	}

	vfs = fp->vfs;
	return vfs->seek(fp, offset);
}

fm_err_t fm_sync(fm_file_handle_t *fp) {
	const fm_vfs_t *vfs;

	if (fp == NULL) {
		return FM_ERR_INVAL;
	}

	if (fp->vfs == NULL) {
		return FM_ERR_INVAL;
	}

	vfs = fp->vfs;
	return vfs->sync(fp);
}

fm_err_t fm_close(fm_file_handle_t **fp) {
	const fm_vfs_t *vfs;

	if ((fp == NULL) || (*fp == NULL)) {
		return FM_OK;
	}

	if ((*fp)->vfs == NULL) {
		free(*fp);
		*fp = NULL;
		return FM_OK;
	}

	vfs = (*fp)->vfs;
	return vfs->close(fp);
}

fm_err_t fm_stat(const char *path, fm_file_info_t *info) {
	const fm_vfs_t *vfs;
	char local_path[FM_MAX_PATH_LEN];
	fm_err_t rc;

	if (path == NULL) {
		return FM_ERR_INVAL;
	}

	vfs = fm_get_vfs_with_type(path, NULL);
	if (vfs == NULL) {
		return FM_ERR_NOENT;
	}

	rc = fm_strip_prefix_internal(path, local_path, sizeof(local_path));
	if (rc != FM_OK) {
		return rc;
	}

	return vfs->stat(local_path, info);
}

fm_err_t fm_mkdir(const char *path) {
	const fm_vfs_t *vfs;
	char local_path[FM_MAX_PATH_LEN];
	fm_err_t rc;

	if (path == NULL) {
		return FM_ERR_INVAL;
	}

	vfs = fm_get_vfs_with_type(path, NULL);
	if (vfs == NULL) {
		return FM_ERR_NOENT;
	}

	rc = fm_strip_prefix_internal(path, local_path, sizeof(local_path));
	if (rc != FM_OK) {
		return rc;
	}

	return vfs->mkdir(local_path);
}

static void fm_strip_trailing_slash(const char *path, char *out, size_t out_len) {
	size_t len;

	if ((path == NULL) || (out == NULL) || (out_len == 0U)) {
		return;
	}

	(void) strncpy(out, path, out_len - 1U);
	out[out_len - 1U] = '\0';

	len = strlen(out);
	while ((len > 1U) && (out[len - 1U] == '/')) {
		out[len - 1U] = '\0';
		len--;
	}
}

static fm_err_t fm_join_path(const char *base, const char *sub, char *out, size_t out_len) {
	size_t base_len;
	size_t sub_len;
	size_t total_len;
	size_t out_pos;

	if ((base == NULL) || (sub == NULL) || (out == NULL) || (out_len == 0U)) {
		return FM_ERR_INVAL;
	}

	base_len = strlen(base);
	sub_len = strlen(sub);

	while ((base_len > 0U) && (base[base_len - 1U] == '/')) {
		base_len--;
	}

	while ((sub_len > 0U) && (sub[0] == '/')) {
		sub++;
		sub_len--;
	}

	total_len = base_len + sub_len + 2U;
	if (total_len > out_len) {
		return FM_ERR_NAMETOOLONG;
	}

	(void) memcpy(out, base, base_len);
	out_pos = base_len;

	if ((base_len > 0U) && (sub_len > 0U)) {
		out[out_pos] = '/';
		out_pos++;
	}

	(void) memcpy(out + out_pos, sub, sub_len);
	out[out_pos + sub_len] = '\0';

	return FM_OK;
}

static const void* fm_memrchr(const void *s, int c, size_t n) {
	const unsigned char *p = (const unsigned char*) s + n;
	while (n--) {
		if (*(--p) == (unsigned char) c) {
			return p;
		}
	}
	return NULL;
}

static const char* fm_get_filename(const char *path) {
	const char *filename;
	size_t len;

	if (path == NULL) {
		return NULL;
	}

	len = strlen(path);

	while ((len > 0U) && (path[len - 1U] == '/')) {
		len--;
	}

	if (len == 0U) {
		return "";
	}

	filename = (const char*) fm_memrchr(path, '/', len);
	if (filename == NULL) {
		return path;
	}

	return (filename + 1U);
}

static fm_err_t fm_rm_recursive(const char *path, uint32_t depth) {
	fm_dir_handle_t *dir = NULL;
	fm_file_info_t info;
	fm_err_t rc;
	char item_path[FM_MAX_PATH_LEN];
	char path_clean[FM_MAX_PATH_LEN];

	if (depth >= FM_COPY_MAX_DEPTH) {
		tsc_kprintf("[Rm] Maximum directory depth exceeded\r\n");
		return FM_ERR_INVAL;
	}

	fm_strip_trailing_slash(path, path_clean, sizeof(path_clean));

	if (!fm_path_is_dir(path_clean)) {
		const fm_vfs_t *file_vfs;
		char file_local_path[FM_MAX_PATH_LEN];

		file_vfs = fm_get_vfs_with_type(path_clean, NULL);
		if (file_vfs == NULL) {
			return FM_ERR_NOENT;
		}

		rc = fm_strip_prefix_internal(path_clean, file_local_path, sizeof(file_local_path));
		if (rc != FM_OK) {
			return rc;
		}

		return file_vfs->remove(file_local_path);
	}

	rc = fm_opendir(path_clean, &dir);
	if (rc != FM_OK) {
		const fm_vfs_t *dir_vfs;
		char dir_local_path[FM_MAX_PATH_LEN];

		dir_vfs = fm_get_vfs_with_type(path_clean, NULL);
		if (dir_vfs == NULL) {
			return FM_ERR_NOENT;
		}

		rc = fm_strip_prefix_internal(path_clean, dir_local_path, sizeof(dir_local_path));
		if (rc != FM_OK) {
			return rc;
		}

		return dir_vfs->remove(dir_local_path);
	}

	while ((rc = fm_readdir(dir, &info)) == FM_OK) {
		if ((info.name[0] == 0) || (info.name[0] == '\0')) {
			rc = FM_OK;
			break;
		}

		if ((strcmp(info.name, ".") == 0) || (strcmp(info.name, "..") == 0)) {
			continue;
		}

		rc = fm_join_path(path_clean, info.name, item_path, sizeof(item_path));
		if (rc != FM_OK) {
			tsc_kprintf("[Rm] Path too long: %s/%s\r\n", path_clean, info.name);
			break;
		}

		if (info.is_dir != 0U) {
			rc = fm_rm_recursive(item_path, depth + 1U);
			if (rc != FM_OK) {
				tsc_kprintf("[Rm] Failed to remove subdirectory: %s (err: %d)\r\n", item_path, rc);
				break;
			}
		} else {
			const fm_vfs_t *item_vfs;
			char item_local_path[FM_MAX_PATH_LEN];

			item_vfs = fm_get_vfs_with_type(item_path, NULL);
			if (item_vfs == NULL) {
				tsc_kprintf("[Rm] Failed to get VFS for: %s\r\n", item_path);
				rc = FM_ERR_NOENT;
				break;
			}

			rc = fm_strip_prefix_internal(item_path, item_local_path, sizeof(item_local_path));
			if (rc != FM_OK) {
				tsc_kprintf("[Rm] Failed to strip prefix: %s\r\n", item_path);
				break;
			}

			rc = item_vfs->remove(item_local_path);
			if (rc != FM_OK) {
				tsc_kprintf("[Rm] Failed to remove file: %s (err: %d)\r\n", item_path, rc);
				break;
			}
		}
	}

	(void) fm_closedir(&dir);

	if (rc == FM_OK) {
		const fm_vfs_t *dir_vfs;
		char dir_local_path[FM_MAX_PATH_LEN];

		dir_vfs = fm_get_vfs_with_type(path_clean, NULL);
		if (dir_vfs == NULL) {
			tsc_kprintf("[Rm] Failed to get VFS for directory: %s\r\n", path_clean);
			return FM_ERR_NOENT;
		}

		rc = fm_strip_prefix_internal(path_clean, dir_local_path, sizeof(dir_local_path));
		if (rc != FM_OK) {
			tsc_kprintf("[Rm] Failed to strip prefix: %s\r\n", path_clean);
			return rc;
		}

		rc = dir_vfs->remove(dir_local_path);
		if (rc != FM_OK) {
			tsc_kprintf("[Rm] Failed to remove directory: %s (err: %d)\r\n", path_clean, rc);
		}
	}

	return rc;
}

fm_err_t fm_rm(const char *path) {
	const fm_vfs_t *vfs;
	char local_path[FM_MAX_PATH_LEN];
	fm_err_t rc;

	if (path == NULL) {
		return FM_ERR_INVAL;
	}

	vfs = fm_get_vfs_with_type(path, NULL);
	if (vfs == NULL) {
		return FM_ERR_NOENT;
	}

	rc = fm_strip_prefix_internal(path, local_path, sizeof(local_path));
	if (rc != FM_OK) {
		return rc;
	}

	if (fm_path_is_dir(path)) {
		return fm_rm_recursive(path, 0U);
	}

	return vfs->remove(local_path);
}

fm_err_t fm_mv(const char *src, const char *dst) {
	const fm_vfs_t *src_vfs;
	const fm_vfs_t *dst_vfs;
	fm_fs_type_t src_fs_type;
	fm_fs_type_t dst_fs_type;
	char src_local[FM_MAX_PATH_LEN];
	char dst_local[FM_MAX_PATH_LEN];
	fm_err_t rc;

	if ((src == NULL) || (dst == NULL)) {
		return FM_ERR_INVAL;
	}

	src_vfs = fm_get_vfs_with_type(src, &src_fs_type);
	if (src_vfs == NULL) {
		return FM_ERR_NOENT;
	}

	dst_vfs = fm_get_vfs_with_type(dst, &dst_fs_type);
	if (dst_vfs == NULL) {
		return FM_ERR_NOENT;
	}

	if (src_fs_type != dst_fs_type) {
		return FM_ERR_XFS_RENAME;
	}

	rc = fm_strip_prefix_internal(src, src_local, sizeof(src_local));
	if (rc != FM_OK) {
		return rc;
	}

	rc = fm_strip_prefix_internal(dst, dst_local, sizeof(dst_local));
	if (rc != FM_OK) {
		return rc;
	}

	return src_vfs->rename(src_local, dst_local);
}

static fm_err_t fm_copy_file(const char *src, const char *dst) {
	fm_file_handle_t *src_fp = NULL;
	fm_file_handle_t *dst_fp = NULL;
	fm_err_t rc;
	uint32_t bytes_read;
	uint32_t bytes_written;
	uint8_t *buffer = NULL;

	buffer = (uint8_t*) malloc(FM_COPY_BUFFER_SIZE);
	if (buffer == NULL) {
		tsc_kprintf("Failed to allocate copy buffer\n");
		return FM_ERR_NOMEM;
	}

	rc = fm_open(src, FM_O_READ, &src_fp);
	if (rc != FM_OK) {
		tsc_kprintf("Failed to open source file: %s, err: %d\n", src, rc);
		free(buffer);
		return rc;
	}

	rc = fm_open(dst, FM_O_WRITE | FM_O_CREATE, &dst_fp);
	if (rc != FM_OK) {
		tsc_kprintf("Failed to create destination file: %s, err: %d\n", dst, rc);
		(void) fm_close(&src_fp);
		free(buffer);
		return rc;
	}

	do {
		rc = fm_read(src_fp, buffer, FM_COPY_BUFFER_SIZE, &bytes_read);
		if (rc != FM_OK) {
			tsc_kprintf("Read error during copy: %d\n", rc);
			break;
		}

		if (bytes_read == 0U) {
			break;
		}

		rc = fm_write(dst_fp, buffer, bytes_read, &bytes_written);
		if (rc != FM_OK) {
			tsc_kprintf("Write error during copy: %d\n", rc);
			break;
		}

		if (bytes_written != bytes_read) {
			tsc_kprintf("Write incomplete: expected %u, wrote %u\n", bytes_read, bytes_written);
			rc = FM_ERR_IO;
			break;
		}
	} while (bytes_read > 0U);

	if (rc == FM_OK) {
		rc = fm_sync(dst_fp);
		if (rc != FM_OK) {
			tsc_kprintf("Sync error: %d\n", rc);
		}
	}

	(void) fm_close(&src_fp);
	(void) fm_close(&dst_fp);
	free(buffer);

	if (rc != FM_OK) {
		(void) fm_rm(dst);
	}

	return rc;
}

static fm_err_t fm_copy_dir_recursive(const char *src, const char *dst, uint32_t depth) {
	fm_dir_handle_t *dir = NULL;
	fm_file_info_t info;
	fm_err_t rc;
	char src_item[FM_MAX_PATH_LEN];
	char dst_item[FM_MAX_PATH_LEN];
	char src_clean[FM_MAX_PATH_LEN];
	char dst_clean[FM_MAX_PATH_LEN];

	if (depth >= FM_COPY_MAX_DEPTH) {
		tsc_kprintf("[Copy] Maximum directory depth exceeded\r\n");
		return FM_ERR_INVAL;
	}

	fm_strip_trailing_slash(src, src_clean, sizeof(src_clean));
	fm_strip_trailing_slash(dst, dst_clean, sizeof(dst_clean));

	if (fm_path_exists(dst_clean)) {
		if (!fm_path_is_dir(dst_clean)) {
			tsc_kprintf("[Copy] Destination exists but is not a directory: %s\r\n", dst_clean);
			return FM_ERR_EXIST;
		}
	} else {
		rc = fm_mkdir(dst_clean);
		if (rc != FM_OK) {
			tsc_kprintf("[Copy] Failed to create directory: %s (err: %d)\r\n", dst_clean, rc);
			return rc;
		}
	}

	rc = fm_opendir(src_clean, &dir);
	if (rc != FM_OK) {
		tsc_kprintf("[Copy] Failed to open directory: %s (err: %d)\r\n", src_clean, rc);
		return rc;
	}

	while ((rc = fm_readdir(dir, &info)) == FM_OK) {
		if ((info.name[0] == 0) || (info.name[0] == '\0')) {
			rc = FM_OK;
			break;
		}

		if ((strcmp(info.name, ".") == 0) || (strcmp(info.name, "..") == 0)) {
			continue;
		}

		rc = fm_join_path(src_clean, info.name, src_item, sizeof(src_item));
		if (rc != FM_OK) {
			tsc_kprintf("[Copy] Path too long: %s/%s\r\n", src_clean, info.name);
			break;
		}

		rc = fm_join_path(dst_clean, info.name, dst_item, sizeof(dst_item));
		if (rc != FM_OK) {
			tsc_kprintf("[Copy] Path too long: %s/%s\r\n", dst_clean, info.name);
			break;
		}

		if (info.is_dir != 0U) {
			rc = fm_copy_dir_recursive(src_item, dst_item, depth + 1U);
			if (rc != FM_OK) {
				tsc_kprintf("[Copy] Failed to copy subdirectory: %s (err: %d)\r\n", src_item, rc);
				break;
			}
		} else {
			rc = fm_copy_file(src_item, dst_item);
			if (rc != FM_OK) {
				tsc_kprintf("[Copy] Failed to copy file: %s (err: %d)\r\n", src_item, rc);
				break;
			}
		}
	}

	(void) fm_closedir(&dir);
	return rc;
}

fm_err_t fm_copy(const char *src, const char *dst) {
	fm_file_info_t src_info;
	fm_fs_type_t src_fs_type;
	fm_fs_type_t dst_fs_type;
	fm_err_t rc;
	char dst_full[FM_MAX_PATH_LEN];
	char src_clean[FM_MAX_PATH_LEN];
	const char *dst_path;
	const char *src_filename;

	if ((src == NULL) || (dst == NULL)) {
		return FM_ERR_INVAL;
	}

	fm_strip_trailing_slash(src, src_clean, sizeof(src_clean));

	rc = fm_stat(src_clean, &src_info);
	if (rc != FM_OK) {
		tsc_kprintf("[Copy] Source not found: %s (err: %d)\r\n", src_clean, rc);
		return FM_ERR_NOENT;
	}

	(void) fm_get_vfs_with_type(src_clean, &src_fs_type);
	(void) fm_get_vfs_with_type(dst, &dst_fs_type);

	if ((src_fs_type != dst_fs_type) && (src_fs_type != FM_FS_TYPE_UNKNOWN) && (dst_fs_type != FM_FS_TYPE_UNKNOWN)) {
		tsc_kprintf("[Copy] Cross-filesystem: %s -> %s\r\n", (src_fs_type == FM_FS_TYPE_FATFS) ? "FATFS" : "LittleFS", (dst_fs_type == FM_FS_TYPE_FATFS) ? "FATFS" : "LittleFS");
	}

	if (fm_path_is_dir(dst)) {
		src_filename = fm_get_filename(src_clean);
		if ((src_filename == NULL) || (src_filename[0] == '\0')) {
			return FM_ERR_INVAL;
		}

		rc = fm_join_path(dst, src_filename, dst_full, sizeof(dst_full));
		if (rc != FM_OK) {
			tsc_kprintf("[Copy] Failed to construct destination path\r\n");
			return rc;
		}
		dst_path = dst_full;
	} else {
		dst_path = dst;
	}

	if (strcmp(src_clean, dst_path) == 0) {
		tsc_kprintf("[Copy] Source and destination are same\r\n");
		return FM_ERR_INVAL;
	}

	if (src_info.is_dir != 0U) {
		rc = fm_copy_dir_recursive(src_clean, dst_path, 0U);
	} else {
		rc = fm_copy_file(src_clean, dst_path);
	}

	return rc;
}

fm_err_t fm_copy_no_overwrite(const char *src, const char *dst) {
	if ((src == NULL) || (dst == NULL)) {
		return FM_ERR_INVAL;
	}

	if (fm_path_exists(dst)) {
		tsc_kprintf("Destination already exists: %s\n", dst);
		return FM_ERR_EXIST;
	}

	return fm_copy(src, dst);
}

fm_err_t fm_move_with_copy(const char *src, const char *dst) {
	fm_err_t rc;
	fm_fs_type_t src_fs_type;
	fm_fs_type_t dst_fs_type;

	if ((src == NULL) || (dst == NULL)) {
		return FM_ERR_INVAL;
	}

	(void) fm_get_vfs_with_type(src, &src_fs_type);
	(void) fm_get_vfs_with_type(dst, &dst_fs_type);

	if (src_fs_type == dst_fs_type) {
		return fm_mv(src, dst);
	}

	tsc_kprintf("[Move] Cross-filesystem: copy then delete\r\n");

	rc = fm_copy(src, dst);
	if (rc != FM_OK) {
		return rc;
	}

	rc = fm_rm(src);
	if (rc != FM_OK) {
		tsc_kprintf("[Move] Warning: copy succeeded but failed to delete source: %d\r\n", rc);
	}

	return FM_OK;
}

fm_err_t fm_get_dir_size(const char *path, uint32_t *file_count, uint32_t *total_size) {
	fm_dir_handle_t *dir = NULL;
	fm_file_info_t info;
	fm_err_t rc;
	char item_path[FM_MAX_PATH_LEN];
	char path_clean[FM_MAX_PATH_LEN];
	uint32_t count = 0U;
	uint32_t size = 0U;

	if (path == NULL) {
		return FM_ERR_INVAL;
	}

	fm_strip_trailing_slash(path, path_clean, sizeof(path_clean));

	rc = fm_opendir(path_clean, &dir);
	if (rc != FM_OK) {
		return rc;
	}

	while ((rc = fm_readdir(dir, &info)) == FM_OK) {
		if ((info.name[0] == 0) || (info.name[0] == '\0')) {
			break;
		}

		if ((strcmp(info.name, ".") == 0) || (strcmp(info.name, "..") == 0)) {
			continue;
		}

		rc = fm_join_path(path_clean, info.name, item_path, sizeof(item_path));
		if (rc != FM_OK) {
			break;
		}

		if (info.is_dir != 0U) {
			uint32_t sub_count = 0U;
			uint32_t sub_size = 0U;

			rc = fm_get_dir_size(item_path, &sub_count, &sub_size);
			if (rc == FM_OK) {
				count += sub_count;
				size += sub_size;
			}
		} else {
			count++;
			size += info.size;
		}
	}

	(void) fm_closedir(&dir);

	if (file_count != NULL) {
		*file_count = count;
	}
	if (total_size != NULL) {
		*total_size = size;
	}

	return FM_OK;
}

fm_err_t fm_opendir(const char *path, fm_dir_handle_t **dp) {
	const fm_vfs_t *vfs;
	char local_path[FM_MAX_PATH_LEN];
	fm_err_t rc;
	fm_dir_handle_t *handle;

	if ((dp == NULL) || (path == NULL)) {
		return FM_ERR_INVAL;
	}

	*dp = NULL;

	vfs = fm_get_vfs_with_type(path, NULL);
	if (vfs == NULL) {
		return FM_ERR_NOENT;
	}

	rc = fm_strip_prefix_internal(path, local_path, sizeof(local_path));
	if (rc != FM_OK) {
		return rc;
	}

	rc = vfs->opendir(dp, local_path);
	if (rc != FM_OK) {
		return rc;
	}

	handle = *dp;
	if (handle != NULL) {
		handle->vfs = vfs;
		handle->fs_type = fm_detect_fs_type(path);
	}

	return FM_OK;
}

fm_err_t fm_readdir(fm_dir_handle_t *dp, fm_file_info_t *info) {
	const fm_vfs_t *vfs;

	if (dp == NULL) {
		return FM_ERR_INVAL;
	}

	if (dp->vfs == NULL) {
		return FM_ERR_INVAL;
	}

	vfs = dp->vfs;
	return vfs->readdir(dp, info);
}

fm_err_t fm_closedir(fm_dir_handle_t **dp) {
	const fm_vfs_t *vfs;

	if ((dp == NULL) || (*dp == NULL)) {
		return FM_OK;
	}

	if ((*dp)->vfs == NULL) {
		free(*dp);
		*dp = NULL;
		return FM_OK;
	}

	vfs = (*dp)->vfs;
	return vfs->closedir(dp);
}

bool fm_path_exists(const char *path) {
	fm_file_info_t info;
	return (fm_stat(path, &info) == FM_OK);
}

bool fm_path_is_dir(const char *path) {
	fm_file_info_t info;
	fm_err_t rc;

	rc = fm_stat(path, &info);
	if (rc != FM_OK) {
		return false;
	}

	return (info.is_dir != 0U);
}


#endif /* CSP_ENABLE_FILE */
