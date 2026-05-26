/**
 ******************************************************************************
 * @file    hw_i2c.h
 * @brief   I2C Hardware Driver Configuration
 * @details I2C peripheral configuration with conditional compilation support.
 *          Only compiled if ENABLE_HW_I2C is enabled in app_modules_config.h.
 ******************************************************************************
 * @author  Pongpot Chaiboonuang
 * @email   pongpot.c@gmail.com
 ******************************************************************************
 */

#ifndef _HW_I2C_H
#define _HW_I2C_H

/* ---------- (1) INCLUDE MASTER CONFIGURATION FILE --------------- */
#include "app_modules_config.h"

#if defined __cplusplus
extern "C" {
#endif

/* ======================================================================== */
/*                        CONDITIONAL COMPILATION                           */
/* ======================================================================== */

/* Only compile this entire file if I2C is enabled */
#if ENABLE_HW_I2C

/* ---------- (2) SYSTEM INCLUDES -------------------------------------- */
#include "hw_config.h"

/* ---------- (3) PLATFORM-SPECIFIC CONFIGURATIONS -------------------- */

#if defined(STM32L496xx)
#define TSC_CSP_I2C1_NAME    "I2C"
#define TSC_CSP_I2C1          hi2c1
#define TSC_CSP_I2C1_TX_DMA   hdma_i2c1_tx
#define TSC_CSP_I2C1_RX_DMA   hdma_i2c1_rx
#endif

/* ---------- (4) EXTERN VARIABLES ------------------------------------ */
#if defined(STM32L496xx)
extern I2C_HandleTypeDef hi2c1;
extern DMA_HandleTypeDef hdma_i2c1_rx;
extern DMA_HandleTypeDef hdma_i2c1_tx;
#endif

/* ======================================================================== */
/*                        PUBLIC API                                     */
/* ======================================================================== */

/**
 * @brief Initialize I2C peripheral
 * @return HAL status
 */
HAL_StatusTypeDef hw_i2c_init(void);

/**
 * @brief Send data via I2C
 * @param addr I2C device address
 * @param data Pointer to data buffer
 * @param len Length of data
 * @param timeout Timeout in milliseconds
 * @return HAL status
 */
HAL_StatusTypeDef hw_i2c_transmit(uint16_t addr, uint8_t *data, uint16_t len, uint32_t timeout);

/**
 * @brief Receive data via I2C
 * @param addr I2C device address
 * @param data Pointer to data buffer
 * @param len Length of data to receive
 * @param timeout Timeout in milliseconds
 * @return HAL status
 */
HAL_StatusTypeDef hw_i2c_receive(uint16_t addr, uint8_t *data, uint16_t len, uint32_t timeout);

/**
 * @brief Write data to I2C register
 * @param addr I2C device address
 * @param reg Register address
 * @param data Data to write
 * @param timeout Timeout in milliseconds
 * @return HAL status
 */
HAL_StatusTypeDef hw_i2c_write_reg(uint16_t addr, uint16_t reg, uint8_t data, uint32_t timeout);

/**
 * @brief Read data from I2C register
 * @param addr I2C device address
 * @param reg Register address
 * @param data Pointer to store read data
 * @param timeout Timeout in milliseconds
 * @return HAL status
 */
HAL_StatusTypeDef hw_i2c_read_reg(uint16_t addr, uint16_t reg, uint8_t *data, uint32_t timeout);

/* End of conditional compilation block for ENABLE_HW_I2C */
#else /* !ENABLE_HW_I2C */

/* ======================================================================== */
/*                        STUB FUNCTIONS FOR DISABLED MODULE            */
/* ======================================================================== */

/**
 * @brief Stub function for when I2C is disabled
 * @return HAL_ERROR to indicate module is not available
 */
static inline HAL_StatusTypeDef hw_i2c_init(void)
{
    return HAL_ERROR;
}

/**
 * @brief Stub function for when I2C is disabled
 * @return HAL_ERROR to indicate module is not available
 */
static inline HAL_StatusTypeDef hw_i2c_transmit(uint16_t addr, uint8_t *data, uint16_t len, uint32_t timeout)
{
    (void)addr;
    (void)data;
    (void)len;
    (void)timeout;
    return HAL_ERROR;
}

/**
 * @brief Stub function for when I2C is disabled
 * @return HAL_ERROR to indicate module is not available
 */
static inline HAL_StatusTypeDef hw_i2c_receive(uint16_t addr, uint8_t *data, uint16_t len, uint32_t timeout)
{
    (void)addr;
    (void)data;
    (void)len;
    (void)timeout;
    return HAL_ERROR;
}

/**
 * @brief Stub function for when I2C is disabled
 * @return HAL_ERROR to indicate module is not available
 */
static inline HAL_StatusTypeDef hw_i2c_write_reg(uint16_t addr, uint16_t reg, uint8_t data, uint32_t timeout)
{
    (void)addr;
    (void)reg;
    (void)data;
    (void)timeout;
    return HAL_ERROR;
}

/**
 * @brief Stub function for when I2C is disabled
 * @return HAL_ERROR to indicate module is not available
 */
static inline HAL_StatusTypeDef hw_i2c_read_reg(uint16_t addr, uint16_t reg, uint8_t *data, uint32_t timeout)
{
    (void)addr;
    (void)reg;
    (void)data;
    (void)timeout;
    return HAL_ERROR;
}

#endif /* ENABLE_HW_I2C */

#if defined __cplusplus
}
#endif

#endif /* _HW_I2C_H */
