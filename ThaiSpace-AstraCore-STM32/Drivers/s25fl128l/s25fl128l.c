/*
 * s25fl128lagmfm010.c
 *
 * Created on: Aug 19, 2025
 * Author: Chaturong
 */

#include "s25fl128l.h"
#if ENABLE_S25FL128L

// --- Helper Functions ---

static S25FL128L_result_t map_hal_status(HAL_StatusTypeDef hal_status) {
    switch (hal_status) {
        case HAL_OK: return S25FL128L_Ok;
        case HAL_TIMEOUT: return S25FL128L_Timeout;
        default: return S25FL128L_Err;
    }
}

static void s25fl128l_select(S25FL128L_HandleTypeDef *hflash) {
    HAL_GPIO_WritePin(hflash->cs_port, hflash->cs_pin, GPIO_PIN_RESET);
}

static void s25fl128l_deselect(S25FL128L_HandleTypeDef *hflash) {
    HAL_GPIO_WritePin(hflash->cs_port, hflash->cs_pin, GPIO_PIN_SET);
}

static S25FL128L_result_t s25fl128l_transmit(S25FL128L_HandleTypeDef *hflash, uint8_t *buf, uint32_t len, uint32_t timeout) {
    return map_hal_status(HAL_SPI_Transmit(hflash->spiHandle, buf, len, timeout));
}

static S25FL128L_result_t s25fl128l_receive(S25FL128L_HandleTypeDef *hflash, uint8_t *buf, uint32_t len, uint32_t timeout) {
    return map_hal_status(HAL_SPI_Receive(hflash->spiHandle, buf, len, timeout));
}

static S25FL128L_result_t s25fl128l_write_enable(S25FL128L_HandleTypeDef *hflash, uint32_t timeout) {
    uint8_t cmd = S25FL128L_WRITE_ENABLE;
    s25fl128l_select(hflash);
    S25FL128L_result_t res = s25fl128l_transmit(hflash, &cmd, 1, timeout);
    s25fl128l_deselect(hflash);
    return res;
}

static S25FL128L_result_t s25fl128l_read_status_register(S25FL128L_HandleTypeDef *hflash, uint8_t *status_reg, uint32_t timeout) {
    uint8_t txData = S25FL128L_READ_STATUS_REG1;
    s25fl128l_select(hflash);
    S25FL128L_result_t res = s25fl128l_transmit(hflash, &txData, 1, timeout);
    if (res == S25FL128L_Ok) {
        res = s25fl128l_receive(hflash, status_reg, 1, timeout);
    }
    s25fl128l_deselect(hflash);
    return res;
}

static S25FL128L_result_t s25fl128l_wait_for_write_completion(S25FL128L_HandleTypeDef *hflash, uint32_t timeout) {
    uint32_t tickstart = HAL_GetTick();
    uint8_t status_reg = 0;
    
    while (1) {
        S25FL128L_result_t res = s25fl128l_read_status_register(hflash, &status_reg, timeout);
        if (res != S25FL128L_Ok) {
            return res;
        }
        
        // Wait until the Write In Progress (WIP) bit in the status register is cleared
        if ((status_reg & S25FL128L_SR_BUSY_MASK) == 0x00) {
            return S25FL128L_Ok;
        }
        
        if ((HAL_GetTick() - tickstart) > timeout) {
            return S25FL128L_Timeout;
        }
    }
}

// --- Public API Functions ---

S25FL128L_result_t s25fl128l_init(S25FL128L_HandleTypeDef *hflash, SPI_HandleTypeDef *hspi, GPIO_TypeDef *cs_port, uint16_t cs_pin, GPIO_TypeDef *wp_port, uint16_t wp_pin, GPIO_TypeDef *rst_port, uint16_t rst_pin) {
    hflash->spiHandle = hspi;
    hflash->cs_port = cs_port;
    hflash->cs_pin = cs_pin;
    hflash->wp_port = wp_port;
    hflash->wp_pin = wp_pin;
    hflash->rst_port = rst_port;
    hflash->rst_pin = rst_pin;

    // Ensure all pins are in the correct default state
    HAL_GPIO_WritePin(hflash->cs_port, hflash->cs_pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(hflash->wp_port, hflash->wp_pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(hflash->rst_port, hflash->rst_pin, GPIO_PIN_SET);

    return S25FL128L_Ok;
}

S25FL128L_result_t s25fl128l_read_id(S25FL128L_HandleTypeDef *hflash) {
    uint8_t txData[4] = {S25FL128L_READ_ID, 0, 0, 0};
    uint8_t rxData[4]; // Corrected size to 4 to match transmission size

    s25fl128l_select(hflash);
    HAL_StatusTypeDef status = HAL_SPI_TransmitReceive(hflash->spiHandle, txData, rxData, 4, 100);
    if (status != HAL_OK) {
        s25fl128l_deselect(hflash);
        return map_hal_status(status);
    }
    s25fl128l_deselect(hflash);

    // After command byte, the next 3 bytes are ID
    hflash->manufacturer_id = rxData[1];
    hflash->device_id = (rxData[2] << 8) | rxData[3];

    if (hflash->manufacturer_id != S25FL128L_MANUFACTURER_ID || (hflash->device_id != 0x2018 && hflash->device_id != 0x2019)) {
        return S25FL128L_Err;
    }

    return S25FL128L_Ok;
}

S25FL128L_result_t s25fl128l_unlock_all_blocks(S25FL128L_HandleTypeDef *hflash) {
    S25FL128L_result_t res = s25fl128l_write_enable(hflash, 100);
    if (res != S25FL128L_Ok) return res;

    uint8_t txData[2];
    txData[0] = S25FL128L_WRITE_STATUS_REG1;
    txData[1] = 0x00; // Clear all BP bits

    s25fl128l_select(hflash);
    res = s25fl128l_transmit(hflash, txData, 2, 100);
    s25fl128l_deselect(hflash);
    
    if (res != S25FL128L_Ok) return res;

    return s25fl128l_wait_for_write_completion(hflash, 1000);
}

S25FL128L_result_t s25fl128l_read(S25FL128L_HandleTypeDef *hflash, uint32_t address, uint8_t *buf, uint32_t len, uint32_t timeout) {
    uint8_t txData[4];
    txData[0] = S25FL128L_READ_DATA;
    txData[1] = (address >> 16) & 0xFF;
    txData[2] = (address >> 8) & 0xFF;
    txData[3] = address & 0xFF;

    S25FL128L_result_t res = s25fl128l_wait_for_write_completion(hflash, timeout);
    if (res != S25FL128L_Ok) return res;

    s25fl128l_select(hflash);
    res = s25fl128l_transmit(hflash, txData, 4, timeout);
    if (res != S25FL128L_Ok) {
        s25fl128l_deselect(hflash);
        return res;
    }
    res = s25fl128l_receive(hflash, buf, len, timeout);
    s25fl128l_deselect(hflash);
    return res;
}

S25FL128L_result_t s25fl128l_write_page(S25FL128L_HandleTypeDef *hflash, uint32_t address, uint8_t *buf, uint16_t len, uint32_t timeout) {
    if (len == 0 || len > 256) {
        return S25FL128L_Err; // Invalid length
    }

    S25FL128L_result_t res = s25fl128l_write_enable(hflash, timeout);
    if (res != S25FL128L_Ok) return res;

    uint8_t txData[4];
    txData[0] = S25FL128L_PAGE_PROGRAM;
    txData[1] = (address >> 16) & 0xFF;
    txData[2] = (address >> 8) & 0xFF;
    txData[3] = address & 0xFF;

    s25fl128l_select(hflash);
    res = s25fl128l_transmit(hflash, txData, 4, timeout);
    if (res != S25FL128L_Ok) {
        s25fl128l_deselect(hflash);
        return res;
    }
    res = s25fl128l_transmit(hflash, buf, len, timeout);
    s25fl128l_deselect(hflash);

    if (res != S25FL128L_Ok) return res;

    return s25fl128l_wait_for_write_completion(hflash, timeout);
}

S25FL128L_result_t s25fl128l_write(S25FL128L_HandleTypeDef *hflash, uint32_t address, uint8_t *buf, uint32_t len, uint32_t timeout) {
    uint32_t bytes_written = 0;
    while (bytes_written < len) {
        uint32_t start_of_page = (address + bytes_written) / S25FL128L_PAGE_SIZE * S25FL128L_PAGE_SIZE;
        uint32_t offset_in_page = (address + bytes_written) % S25FL128L_PAGE_SIZE;
        uint16_t write_len = S25FL128L_PAGE_SIZE - offset_in_page;
        if (write_len > (len - bytes_written)) {
            write_len = len - bytes_written;
        }

        S25FL128L_result_t res = s25fl128l_write_page(hflash, start_of_page + offset_in_page, buf + bytes_written, write_len, timeout);
        if (res != S25FL128L_Ok) {
            return res;
        }
        bytes_written += write_len;
    }
    return S25FL128L_Ok;
}

S25FL128L_result_t s25fl128l_erase_sector(S25FL128L_HandleTypeDef *hflash, uint32_t address, uint32_t timeout) {
    // Ensure the address is aligned to a 4KB sector boundary (0x1000)
    if (address % S25FL128L_SECTOR_SIZE != 0) {
        return S25FL128L_Err;
    }

    S25FL128L_result_t res = s25fl128l_write_enable(hflash, timeout);
    if (res != S25FL128L_Ok) return res;

    uint8_t txData[4];
    txData[0] = S25FL128L_SECTOR_ERASE;
    txData[1] = (address >> 16) & 0xFF;
    txData[2] = (address >> 8) & 0xFF;
    txData[3] = address & 0xFF;

    s25fl128l_select(hflash);
    res = s25fl128l_transmit(hflash, txData, 4, timeout);
    s25fl128l_deselect(hflash);

    if (res != S25FL128L_Ok) return res;

    return s25fl128l_wait_for_write_completion(hflash, timeout);
}

S25FL128L_result_t s25fl128l_erase(S25FL128L_HandleTypeDef *hflash, uint32_t address, uint32_t len, uint32_t timeout) {
    if (len == 0) return S25FL128L_Ok;

    uint32_t start_sector = address / S25FL128L_SECTOR_SIZE;
    uint32_t end_sector = (address + len - 1) / S25FL128L_SECTOR_SIZE;
    for (uint32_t sector = start_sector; sector <= end_sector; sector++) {
        S25FL128L_result_t res = s25fl128l_erase_sector(hflash, sector * S25FL128L_SECTOR_SIZE, timeout);
        if (res != S25FL128L_Ok) {
            return res;
        }
    }
    return S25FL128L_Ok;
}

S25FL128L_result_t s25fl128l_chip_erase(S25FL128L_HandleTypeDef *hflash, uint32_t timeout) {
    S25FL128L_result_t res = s25fl128l_write_enable(hflash, timeout);
    if (res != S25FL128L_Ok) return res;

    uint8_t cmd = S25FL128L_CHIP_ERASE;
    s25fl128l_select(hflash);
    res = s25fl128l_transmit(hflash, &cmd, 1, timeout);
    s25fl128l_deselect(hflash);

    if (res != S25FL128L_Ok) return res;

    return s25fl128l_wait_for_write_completion(hflash, timeout);
}

void s25fl128l_set_wp(S25FL128L_HandleTypeDef *hflash, GPIO_PinState state) {
    HAL_GPIO_WritePin(hflash->wp_port, hflash->wp_pin, state);
}

void s25fl128l_hardware_reset(S25FL128L_HandleTypeDef *hflash) {
    HAL_GPIO_WritePin(hflash->rst_port, hflash->rst_pin, GPIO_PIN_RESET);
    HAL_Delay(1);
    HAL_GPIO_WritePin(hflash->rst_port, hflash->rst_pin, GPIO_PIN_SET);
}

S25FL128L_result_t s25fl128l_software_reset(S25FL128L_HandleTypeDef *hflash) {
    uint8_t txData[2];
    txData[0] = S25FL128L_RESET_ENABLE;
    txData[1] = S25FL128L_RESET;

    s25fl128l_select(hflash);
    S25FL128L_result_t res = s25fl128l_transmit(hflash, txData, 2, 100);
    s25fl128l_deselect(hflash);

    return res;
}

#endif
