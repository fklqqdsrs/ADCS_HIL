/**
 ******************************************************************************
 * @file    hw_can.h
 * @brief   CAN Hardware Driver Configuration
 * @details CAN peripheral configuration with conditional compilation support.
 *          Only compiled if ENABLE_HW_CAN is enabled in app_modules_config.h.
 ******************************************************************************
 * @author  Pongpot Chaiboonuang
 * @email   pongpot.c@gmail.com
 ******************************************************************************
 */

#ifndef _HW_CAN_H
#define _HW_CAN_H

/* ---------- (1) INCLUDE MASTER CONFIGURATION FILE --------------- */
#include "app_modules_config.h"

#if defined __cplusplus
extern "C" {
#endif

/* ======================================================================== */
/*                        CONDITIONAL COMPILATION                           */
/* ======================================================================== */

/* Only compile this entire file if CAN is enabled */
#if ENABLE_HW_CAN

/* ---------- (2) SYSTEM INCLUDES -------------------------------------- */
#include "hw_config.h"

/* ---------- (3) PLATFORM-SPECIFIC CONFIGURATIONS -------------------- */

#if defined(STM32F767xx)
#define TSC_CSP_CAN1_NAME        "HCAN1"
#define TSC_CSP_CAN1             hcan1
#define TSC_CSP_CAN1_ACTIVE_FLAG (CAN_IT_TX_MAILBOX_EMPTY | CAN_IT_RX_FIFO0_MSG_PENDING)
#endif

#if defined(STM32L496xx)
#define TSC_CSP_CAN1_NAME        "CAN2"
#define TSC_CSP_CAN1             hcan2
#define TSC_CSP_CAN1_ACTIVE_FLAG (CAN_IT_TX_MAILBOX_EMPTY | CAN_IT_RX_FIFO0_MSG_PENDING)
#endif

/* ---------- (4) EXTERN VARIABLES ------------------------------------ */
extern CAN_HandleTypeDef hcan2;

/* ======================================================================== */
/*                        PUBLIC API                                     */
/* ======================================================================== */

/**
 * @brief Initialize CAN peripheral
 * @return HAL status
 */
HAL_StatusTypeDef hw_can_init(void);

/**
 * @brief Send CAN message
 * @param id CAN message ID
 * @param data Pointer to data buffer
 * @param len Length of data
 * @return HAL status
 */
HAL_StatusTypeDef hw_can_transmit(uint32_t id, uint8_t *data, uint8_t len);

/**
 * @brief Receive CAN message
 * @param id Pointer to store received ID
 * @param data Pointer to store received data
 * @param len Pointer to store received data length
 * @return HAL status
 */
HAL_StatusTypeDef hw_can_receive(uint32_t *id, uint8_t *data, uint8_t *len);

/* End of conditional compilation block for ENABLE_HW_CAN */
#else /* !ENABLE_HW_CAN */

/* ======================================================================== */
/*                        STUB FUNCTIONS FOR DISABLED MODULE            */
/* ======================================================================== */

/**
 * @brief Stub function for when CAN is disabled
 * @return HAL_ERROR to indicate module is not available
 */
static inline HAL_StatusTypeDef hw_can_init(void)
{
    return HAL_ERROR;
}

/**
 * @brief Stub function for when CAN is disabled
 * @return HAL_ERROR to indicate module is not available
 */
static inline HAL_StatusTypeDef hw_can_transmit(uint32_t id, uint8_t *data, uint8_t len)
{
    (void)id;
    (void)data;
    (void)len;
    return HAL_ERROR;
}

/**
 * @brief Stub function for when CAN is disabled
 * @return HAL_ERROR to indicate module is not available
 */
static inline HAL_StatusTypeDef hw_can_receive(uint32_t *id, uint8_t *data, uint8_t *len)
{
    (void)id;
    (void)data;
    (void)len;
    return HAL_ERROR;
}

#endif /* ENABLE_HW_CAN */

#if defined __cplusplus
}
#endif

#endif /* _HW_CAN_H */
