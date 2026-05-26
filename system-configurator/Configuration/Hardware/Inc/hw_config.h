/**
 ******************************************************************************
 * @file    hw_config.h
 * @brief   Hardware Configuration Master Header
 * @details Central hardware configuration file for multi STM32 series.
 *          Includes conditional compilation support for all hardware modules.
 *          This file should be included first to access all hardware configurations.
 ******************************************************************************
 * @author  Pongpot Chaiboonuang
 * @email   pongpot.c@gmail.com
 ******************************************************************************
 */

#ifndef _HW_CONFIG_H
#define _HW_CONFIG_H

/* ---------- (1) INCLUDE MASTER CONFIGURATION FILE FIRST --------------- */
#include "app_modules_config.h"
#include <ksdef.h>
#if defined __cplusplus
extern "C" {
#endif

/* ======================================================================== */
/*                        PLATFORM-SPECIFIC HAL INCLUDES                    */
/* ======================================================================== */

#if defined(STM32F767xx)
    #include "stm32f7xx_hal.h"
#elif defined(STM32F4xx)
    #include "stm32f4xx_hal.h"
#elif defined(STM32F1xx)
    #include "stm32f1xx_hal.h"
#elif defined(STM32L496xx)
    #include "stm32l496xx.h"
#include "stm32l4xx_hal.h"
#else
    #error "Unsupported STM32 series"
#endif

/* ======================================================================== */
/*                        CONDITIONAL PERIPHERAL INCLUDES                   */
/* ======================================================================== */

/* Include RTC headers only if RTC is enabled in CubeMX and app config */
#if ENABLE_TIME_GNSS || ENABLE_TIME_CTP
    #if defined(STM32L496xx)
        #ifdef HAL_RTC_MODULE_ENABLED
            #include <rtc.h>
        #endif
    #endif
#endif

/* Include SPI headers only if SPI is enabled in CubeMX and app config */
#if ENABLE_LITTLEFS || ENABLE_S25FL128L
    #if defined(STM32L496xx)
        #ifdef HAL_SPI_MODULE_ENABLED
            #include <spi.h>
        #endif
    #endif
#endif

/* Include USART/UART headers only if UART is enabled in CubeMX and app config */
#if ENABLE_HW_UART || ENABLE_TIME_GNSS || ENABLE_SHELL
    #if defined(STM32L496xx)
        #if defined(HAL_UART_MODULE_ENABLED) || defined(HAL_USART_MODULE_ENABLED) || defined(HAL_LPUART_MODULE_ENABLED)
            #include <usart.h>
        #endif
    #endif
#endif

/* ======================================================================== */
/*                        CONDITIONAL HARDWARE MODULE INCLUDES ------------------ */

#if ENABLE_HW_CAN
    #include "hw_can.h"
#endif

#if ENABLE_HW_UART
    #include "hw_uart.h"
#endif

#if ENABLE_HW_I2C
    #include "hw_i2c.h"
#endif

#if ENABLE_HW_GPIO
    #include "hw_gpio.h"
#endif

/* ======================================================================== */
/*                        PLATFORM-SPECIFIC CONFIGURATIONS -------------------- */

#if defined(STM32L496xx)
    #ifdef HAL_RTC_MODULE_ENABLED
        #define TSC_CSP_RTC        hrtc
    #endif

    #ifdef HAL_SPI_MODULE_ENABLED
        #define SPI_FLASH_NUM      1
        #define SPI_FLASH_HAL      hspi1
    #endif
#endif

/* ======================================================================== */
/*                        EXTERN VARIABLES ------------------------------------ */

#if defined(STM32L496xx)
    #ifdef HAL_RTC_MODULE_ENABLED
        extern RTC_HandleTypeDef hrtc;
    #endif

    #ifdef HAL_SPI_MODULE_ENABLED
        extern SPI_HandleTypeDef hspi1;
    #endif
#endif

/* ======================================================================== */
/*                        UTILITY FUNCTIONS ----------------------------------- */

/**
 * @brief Disable interrupts globally
 * @return Previous interrupt state
 */
static inline uint32_t hw_interrupt_disable(void)
{
    uint32_t level = __get_PRIMASK();
    __disable_irq();
    return level;
}

/**
 * @brief Enable interrupts globally
 * @param level Previous interrupt state from hw_interrupt_disable()
 */
static inline void hw_interrupt_enable(uint32_t level)
{
    __set_PRIMASK(level);
}

#if defined __cplusplus
}
#endif

#endif /* _HW_CONFIG_H */
