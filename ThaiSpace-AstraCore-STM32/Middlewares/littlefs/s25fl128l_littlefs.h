/*
 * s25fl128l_littlefs.h
 *
 *  Created on: Aug 19, 2025
 *      Author: Chaturong
 */

#ifndef S25FL128L_LITTLEFS_H_
#define S25FL128L_LITTLEFS_H_

#include "app_modules_config.h"

#if ENABLE_LITTLEFS && ENABLE_S25FL128L

#include "main.h"
#include "lfs.h" // Include the LittleFS library header
#include "s25fl128l.h" // Include the flash driver header
#include <cmsis_os.h>

#define DEBUGxxx

#ifdef DEBUGxxx
#define LFS_DBG(...) printf(__VA_ARGS__);\
                     printf("\r\n");
#else
#define LFS_DBG(...) ;
#endif

#define SPI_FLASH_CS_GPIO_PORT  NOR_CS_GPIO_Port
#define SPI_FLASH_CS_GPIO_PIN   NOR_CS_Pin
#define SPI_FLASH_WP_GPIO_PORT  GPIOG
#define SPI_FLASH_WP_GPIO_PIN   NOR_WP_Pin
#define SPI_FLASH_RST_GPIO_PORT GPIOG
#define SPI_FLASH_RST_GPIO_PIN  NOR_RST_Pin

/**
 * @brief  External declarations for global variables.
 */
extern lfs_t lfs_array[SPI_FLASH_NUM];
extern lfs_file_t file;
extern S25FL128L_HandleTypeDef s25fl128l_handle;
extern struct lfs_config littlefs_config;
extern osMutexId_t lfs_mutex_id;




/**
 * @brief  Initializes the S25FL128L flash chip and mounts the LittleFS filesystem.
 * @retval 0 on success, -1 on failure.
 */
int s25fl128l_littlefs_init(void);

/**
 * @brief  Gets the LittleFS handle (lfs_t pointer).
 * @retval Pointer to the lfs_t structure.
 */
lfs_t* s25fl128l_littlefs_get_handle(void);

#endif /* ENABLE_LITTLEFS && ENABLE_S25FL128L */

#endif /* S25FL128L_LITTLEFS_H_ */
