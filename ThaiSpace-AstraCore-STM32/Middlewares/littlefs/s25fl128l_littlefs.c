/*
 * s25fl128l_littlefs.c
 *
 *  Created on: Aug 19, 2025
 *      Author: Chaturong
 */

#include "s25fl128l_littlefs.h"

#if ENABLE_LITTLEFS && ENABLE_S25FL128L

#include <stdio.h>

const osMutexAttr_t lfsMutex_attributes = {
  .name = "lfsMutex"
};

osMutexId_t lfs_mutex_id;

// LittleFS callback function prototypes
int littlefs_read(const struct lfs_config *c, lfs_block_t block, lfs_off_t off, void *buffer, lfs_size_t size);
int littlefs_prog(const struct lfs_config *c, lfs_block_t block, lfs_off_t off, const void *buffer, lfs_size_t size);
int littlefs_erase(const struct lfs_config *c, lfs_block_t block);
int littlefs_sync(const struct lfs_config *c);
int littlefs_lock(const struct lfs_config *c);
int littlefs_unlock(const struct lfs_config *c);

// Global LittleFS structures
lfs_t lfs_array[SPI_FLASH_NUM];
lfs_file_t file;
S25FL128L_HandleTypeDef s25fl128l_handle;

// LittleFS configuration
struct lfs_config littlefs_config = {
    .read  = littlefs_read,
    .prog  = littlefs_prog,
    .erase = littlefs_erase,
    .sync  = littlefs_sync,
    .lock  = littlefs_lock,
    .unlock = littlefs_unlock,

    .read_size      = 256,
    .prog_size      = 256,
    .block_size     = 4096,
    .block_count    = 4096,
    .cache_size     = 256,
    .lookahead_size = 256,
    .block_cycles   = 500,
};

int s25fl128l_littlefs_init(void) {
    // 1. Initialize the S25FL128L flash driver
    S25FL128L_result_t res = s25fl128l_init(&s25fl128l_handle, &SPI_FLASH_HAL, 
                                            SPI_FLASH_CS_GPIO_PORT,  SPI_FLASH_CS_GPIO_PIN,
                                            SPI_FLASH_WP_GPIO_PORT,  SPI_FLASH_WP_GPIO_PIN,
                                            SPI_FLASH_RST_GPIO_PORT, SPI_FLASH_RST_GPIO_PIN);

    if (res != S25FL128L_Ok) {
        LFS_DBG("S25FL128L Driver Init Failed!");
        return -1;
    }

    // 1.1 Unlock all blocks (Clear Write Protect bits)
    if (s25fl128l_unlock_all_blocks(&s25fl128l_handle) != S25FL128L_Ok) {
        LFS_DBG("S25FL128L Unlock failed! Aborting init.");
        return -1;
    }
    LFS_DBG("S25FL128L Unlocked successfully");

    // 1.2 Check ID (Optional but good for debug)
    if (s25fl128l_read_id(&s25fl128l_handle) == S25FL128L_Ok) {
        LFS_DBG("S25FL128L ID: 0x%02X 0x%04X", s25fl128l_handle.manufacturer_id, s25fl128l_handle.device_id);
    }

    // 2. Create the LittleFS mutex
    if (lfs_mutex_id == NULL) {
        lfs_mutex_id = osMutexNew(&lfsMutex_attributes);
        if (lfs_mutex_id == NULL) {
            LFS_DBG("Mutex creation failed");
            return -1;
        }
    }

    // 3. Mount the LittleFS filesystem
    LFS_DBG("Mounting LittleFS...");
    int err = lfs_mount(&lfs_array[0], &littlefs_config);

    // 4. If mounting fails, format and mount again
    if (err) {
        LFS_DBG("Mount failed (err=%d), formatting flash...", err);
        int format_err = lfs_format(&lfs_array[0], &littlefs_config);
        if (format_err == 0) {
            LFS_DBG("Format successful, retrying mount...");
            err = lfs_mount(&lfs_array[0], &littlefs_config);
        } else {
            LFS_DBG("Format failed (err=%d)", format_err);
        }
    }

    if (err) {
        LFS_DBG("LittleFS mount FAILED: %d", err);
        return -1;
    }

    LFS_DBG("LittleFS Mounted Successfully");
    return 0;
}

lfs_t* s25fl128l_littlefs_get_handle(void) {
    return &lfs_array[0];
}

// --- LittleFS Callback Implementations ---

int littlefs_read(const struct lfs_config *c, lfs_block_t block, lfs_off_t off, void *buffer, lfs_size_t size) {
    uint32_t address = (block * c->block_size) + off;
    if (s25fl128l_read(&s25fl128l_handle, address, (uint8_t*)buffer, size, 1000) != S25FL128L_Ok) {
        return -1;
    }
    return 0;
}

int littlefs_prog(const struct lfs_config *c, lfs_block_t block, lfs_off_t off, const void *buffer, lfs_size_t size) {
    uint32_t address = (block * c->block_size) + off;
    if (s25fl128l_write_page(&s25fl128l_handle, address, (uint8_t*)buffer, size, 1000) != S25FL128L_Ok) {
        return -1;
    }
    return 0;
}

int littlefs_erase(const struct lfs_config *c, lfs_block_t block) {
    uint32_t address = block * c->block_size;
    if (s25fl128l_erase_sector(&s25fl128l_handle, address, 1000) != S25FL128L_Ok) {
        return -1;
    }
    return 0;
}

int littlefs_sync(const struct lfs_config *c) {
    // S25FL128L driver's write functions are synchronous (they wait for BUSY bit to clear)
    return 0;
}

int littlefs_lock(const struct lfs_config *c) {
    if (osMutexAcquire(lfs_mutex_id, osWaitForever) != osOK) {
        return -1;
    }
    return 0;
}

int littlefs_unlock(const struct lfs_config *c) {
    if (osMutexRelease(lfs_mutex_id) != osOK) {
        return -1;
    }
    return 0;
}

#endif /* ENABLE_LITTLEFS && ENABLE_S25FL128L */
