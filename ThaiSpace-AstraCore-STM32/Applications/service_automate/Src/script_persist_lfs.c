#include "app_modules_config.h"
#if ENABLE_SCRIPT_MANAGER

#include "app_modules_config.h"

/**
 * @file script_persist_lfs.c
 * @brief LittleFS-backed persistence for script scheduler (atomic).
 */

#include "script_scheduler.h"
#include <string.h>
#include <stdio.h>

#if ENABLE_LITTLEFS && ENABLE_S25FL128L
#include "lfs.h"

/* ===== Config ===== */
#define SCRIPT_DB_PATH "/scripts.db"
extern lfs_t lfs_array[SPI_FLASH_NUM];
extern sched_entry_t g_sched[SCRIPT_MAX_SCHED];
#endif

/* ===== Types ===== */
#define DB_MAGIC 0x53435250 // "SCRP"
typedef struct {
	uint32_t magic;
	uint32_t entry_count;
	uint32_t reserved[2];
	uint32_t checksum;
} db_header_t;

/* ===== Implementation ===== */

int script_persist_save(void) {
#if ENABLE_LITTLEFS && ENABLE_S25FL128L
	lfs_t *lfs = &lfs_array[0];
	lfs_file_t file;
	int err;

	db_header_t header = { .magic = DB_MAGIC, .entry_count = 0, .reserved = { 0, 0 }, .checksum = 0 };

	// 1. Open for write (create if not exists, truncate)
	err = lfs_file_open(lfs, &file, SCRIPT_DB_PATH, LFS_O_WRONLY | LFS_O_CREAT | LFS_O_TRUNC);
	if (err < 0)
		return err;

	// 2. Count active entries
	for (int i = 0; i < (int)SCRIPT_MAX_SCHED; i++) {
		if (g_sched[i].used)
			header.entry_count++;
	}

	// 3. Write header
	lfs_file_write(lfs, &file, &header, sizeof(header));

	// 4. Write data
	for (int i = 0; i < (int)SCRIPT_MAX_SCHED; i++) {
		if (g_sched[i].used) {
			lfs_file_write(lfs, &file, &g_sched[i], sizeof(sched_entry_t));
		}
	}

	lfs_file_close(lfs, &file);
	return 0;
#else
	return -1;
#endif
}

int script_persist_load(void) {
#if ENABLE_LITTLEFS && ENABLE_S25FL128L
	lfs_t *lfs = &lfs_array[0];
	lfs_file_t file;
	db_header_t header;
	int err;

	err = lfs_file_open(lfs, &file, SCRIPT_DB_PATH, LFS_O_RDONLY);
	if (err < 0)
		return err;

	// Read header
	if (lfs_file_read(lfs, &file, &header, sizeof(header)) != sizeof(header)) {
		lfs_file_close(lfs, &file);
		return -1;
	}

	if (header.magic != DB_MAGIC) {
		lfs_file_close(lfs, &file);
		return -2;
	}

	// Clear current table
	for (int i = 0; i < (int)SCRIPT_MAX_SCHED; i++) {
		g_sched[i].used = false;
	}

	// Read entries directly into table
	for (uint32_t i = 0; i < header.entry_count; i++) {
		if (i >= SCRIPT_MAX_SCHED) break;
		if (lfs_file_read(lfs, &file, &g_sched[i], sizeof(sched_entry_t)) != sizeof(sched_entry_t))
			break;
		g_sched[i].used = true;
	}

	lfs_file_close(lfs, &file);
	return 0;
#else
	return -1;
#endif
}

int script_persist_clear(void) {
#if ENABLE_LITTLEFS && ENABLE_S25FL128L
	lfs_t *lfs = &lfs_array[0];
	return lfs_remove(lfs, SCRIPT_DB_PATH);
#else
	return -1;
#endif
}

#else /* !ENABLE_LITTLEFS */

/* Stub implementations when LittleFS/Persistence is disabled */
int script_persist_save(void) { return 0; }
int script_persist_load(void) { return 0; }
int script_persist_clear(void) { return 0; }

#endif /* ENABLE_LITTLEFS */

