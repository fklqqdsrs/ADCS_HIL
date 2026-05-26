/*
 * s25fl128lagmfm010.h
 *
 * Created on: Aug 19, 2025
 * Author: Chaturong
 */

#ifndef S25FL128L_H_
#define S25FL128L_H_

/* ---------- (1) INCLUDE MASTER CONFIGURATION FILE --------------- */
#include "app_modules_config.h"

#if ENABLE_S25FL128L
#include "main.h"
#include <stdint.h>

// S25FL128L specific opcodes
#define S25FL128L_READ_ID            0x9F
#define S25FL128L_READ_DATA          0x03
#define S25FL128L_FAST_READ          0x0B
#define S25FL128L_WRITE_ENABLE       0x06
#define S25FL128L_READ_STATUS_REG1   0x05
#define S25FL128L_WRITE_STATUS_REG1  0x01
#define S25FL128L_PAGE_PROGRAM       0x02
#define S25FL128L_SECTOR_ERASE       0x20
#define S25FL128L_CHIP_ERASE         0xC7
#define S25FL128L_RESET_ENABLE       0x66 // Reset Enable Command
#define S25FL128L_RESET              0x99 // Reset Command

// Status Register Bits
#define S25FL128L_SR_BUSY_MASK       0x01    // Write In Progress (WIP) bit

// Manufacturer and Device IDs
#define S25FL128L_MANUFACTURER_ID    0x01
#define S25FL128L_DEVICE_ID          0x2018

// Memory constants
#define S25FL128L_PAGE_SIZE          256
#define S25FL128L_SECTOR_SIZE        4096

/**
 * @brief  S25FL128L handle structure definition.
 * This structure contains the configuration and identification info for the chip.
 */
typedef struct {
    SPI_HandleTypeDef *spiHandle;
    GPIO_TypeDef *cs_port;
    uint16_t cs_pin;
    GPIO_TypeDef *wp_port;  //!< GPIO port for the Write Protect pin
    uint16_t wp_pin;        //!< GPIO pin for Write Protect
    GPIO_TypeDef *rst_port; //!< GPIO port for the Reset pin
    uint16_t rst_pin;       //!< GPIO pin for Reset
    uint8_t manufacturer_id;
    uint16_t device_id;
} S25FL128L_HandleTypeDef;

/**
 * @brief  Result status enumeration.
 */
typedef enum {
    S25FL128L_Ok,
    S25FL128L_Err,
    S25FL128L_Timeout
} S25FL128L_result_t;

// Function prototypes
S25FL128L_result_t s25fl128l_init(S25FL128L_HandleTypeDef *hflash, SPI_HandleTypeDef *hspi, GPIO_TypeDef *cs_port, uint16_t cs_pin, GPIO_TypeDef *wp_port, uint16_t wp_pin, GPIO_TypeDef *rst_port, uint16_t rst_pin);
S25FL128L_result_t s25fl128l_read_id(S25FL128L_HandleTypeDef *hflash);
S25FL128L_result_t s25fl128l_unlock_all_blocks(S25FL128L_HandleTypeDef *hflash);
S25FL128L_result_t s25fl128l_read(S25FL128L_HandleTypeDef *hflash, uint32_t address, uint8_t *buf, uint32_t len, uint32_t timeout);
S25FL128L_result_t s25fl128l_write_page(S25FL128L_HandleTypeDef *hflash, uint32_t address, uint8_t *buf, uint16_t len, uint32_t timeout);
S25FL128L_result_t s25fl128l_write(S25FL128L_HandleTypeDef *hflash, uint32_t address, uint8_t *buf, uint32_t len, uint32_t timeout);
S25FL128L_result_t s25fl128l_erase_sector(S25FL128L_HandleTypeDef *hflash, uint32_t address, uint32_t timeout);
S25FL128L_result_t s25fl128l_erase(S25FL128L_HandleTypeDef *hflash, uint32_t address, uint32_t len, uint32_t timeout);
S25FL128L_result_t s25fl128l_chip_erase(S25FL128L_HandleTypeDef *hflash, uint32_t timeout);
void s25fl128l_set_wp(S25FL128L_HandleTypeDef *hflash, GPIO_PinState state);
void s25fl128l_hardware_reset(S25FL128L_HandleTypeDef *hflash);
S25FL128L_result_t s25fl128l_software_reset(S25FL128L_HandleTypeDef *hflash);

#endif /* S25FL128L_H_ */
#endif
