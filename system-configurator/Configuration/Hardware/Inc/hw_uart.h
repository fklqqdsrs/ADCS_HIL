/**
 ******************************************************************************
 * @file    hw_uart.h
 * @brief   UART Hardware Driver Configuration
 * @details UART peripheral configuration with conditional compilation support.
 *          Only compiled if ENABLE_HW_UART is enabled in app_modules_config.h.
 ******************************************************************************
 * @author  Pongpot Chaiboonuang
 * @email   pongpot.c@gmail.com
 ******************************************************************************
 */

#ifndef _HW_UART_H
#define _HW_UART_H

/* ---------- (1) INCLUDE MASTER CONFIGURATION FILE --------------- */
#include "app_modules_config.h"

#if defined __cplusplus
extern "C" {
#endif

/* ======================================================================== */
/*                        CONDITIONAL COMPILATION                           */
/* ======================================================================== */


/* Only compile this entire file if UART is enabled */


/* ---------- (2) SYSTEM INCLUDES -------------------------------------- */
#include "hw_config.h"

/* ---------- (3) PLATFORM-SPECIFIC CONFIGURATIONS -------------------- */

#if defined(STM32L496xx)
#if ENABLE_HW_UART
#define TSC_CSP_UART1_NAME      "USART1"
#define TSC_CSP_UART1           huart4
#define TSC_CSP_UART1_TX_DMA    hdma_uart4_tx
#define TSC_CSP_UART1_RX_DMA    hdma_uart4_rx
#endif

#ifdef TSC_USING_FINSH
#define TSC_CSP_UART_CONSOLE	hlpuart1
#endif

#if ENABLE_TIME_GNSS
#define TSC_CTP_UART_GNSS       huart4
#endif

#endif

/* ---------- (4) EXTERN VARIABLES ------------------------------------ */
#if defined(STM32L496xx)
#ifdef TSC_USING_FINSH
extern UART_HandleTypeDef hlpuart1;
#endif

#if ENABLE_TIME_GNSS
extern UART_HandleTypeDef huart4;
#endif

#if ENABLE_HW_UART
extern UART_HandleTypeDef huart4;
extern DMA_HandleTypeDef hdma_uart4_rx;
extern DMA_HandleTypeDef hdma_uart4_tx;
#endif
#endif

/* ======================================================================== */
/*                        PUBLIC API                                     */
/* ======================================================================== */

#ifdef TSC_USING_FINSH
void tsc_hw_console_output(const char *str);
#endif

#if defined __cplusplus
}
#endif

#endif /* _HW_UART_H */
