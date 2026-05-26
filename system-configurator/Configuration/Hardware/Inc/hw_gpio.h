/**
 ******************************************************************************
 * @file    hw_gpio.h
 * @brief   GPIO Hardware Driver Configuration
 * @details GPIO peripheral configuration with conditional compilation support.
 *          Only compiled if ENABLE_HW_GPIO is enabled in app_modules_config.h.
 ******************************************************************************
 * @author  Pongpot Chaiboonuang
 * @email   pongpot.c@gmail.com
 ******************************************************************************
 */

#ifndef _HW_GPIO_H
#define _HW_GPIO_H

/* ---------- (1) INCLUDE MASTER CONFIGURATION FILE --------------- */
#include "app_modules_config.h"

#if defined __cplusplus
extern "C" {
#endif

/* ======================================================================== */
/*                        CONDITIONAL COMPILATION                           */
/* ======================================================================== */

/* Only compile this entire file if GPIO is enabled */
#if ENABLE_HW_GPIO

/* ---------- (2) SYSTEM INCLUDES -------------------------------------- */
#include "hw_config.h"

/* ---------- (3) PLATFORM-SPECIFIC CONFIGURATIONS -------------------- */

#if defined(STM32L496xx)
/* Time PPS (Pulse Per Second) GPIO Configuration */
#define TSC_TIME_PPS_GPIO_PIN    GPIO_PIN_3
#define TSC_TIME_PPS_GPIO_PORT   GPIOG
#define TSC_TIME_PPS_GPIO_CLK    __HAL_RCC_GPIOG_CLK_ENABLE()
#define TSC_TIME_PPS_GPIO_EXTI   EXTI3_IRQn

/* SPI Flash Chip Select GPIO Configuration */
#define SPI_FLASH_CS_GPIO_PIN    NOR_CS_Pin
#define SPI_FLASH_CS_GPIO_PORT   NOR_CS_GPIO_Port
#endif

/* ---------- (4) PUBLIC API                                         */
/* ======================================================================== */

/**
 * @brief Initialize GPIO peripheral
 * @return HAL status
 */
HAL_StatusTypeDef hw_gpio_init(void);

/**
 * @brief Configure GPIO pin as input
 * @param port GPIO port
 * @param pin GPIO pin
 * @return HAL status
 */
HAL_StatusTypeDef hw_gpio_config_input(GPIO_TypeDef *port, uint16_t pin);

/**
 * @brief Configure GPIO pin as output
 * @param port GPIO port
 * @param pin GPIO pin
 * @param pull Pull-up/pull-down configuration
 * @param speed GPIO speed
 * @return HAL status
 */
HAL_StatusTypeDef hw_gpio_config_output(GPIO_TypeDef *port, uint16_t pin, 
                                     uint32_t pull, uint32_t speed);

/**
 * @brief Set GPIO pin to high
 * @param port GPIO port
 * @param pin GPIO pin
 */
static inline void hw_gpio_set(GPIO_TypeDef *port, uint16_t pin)
{
    HAL_GPIO_WritePin(port, pin, GPIO_PIN_SET);
}

/**
 * @brief Set GPIO pin to low
 * @param port GPIO port
 * @param pin GPIO pin
 */
static inline void hw_gpio_reset(GPIO_TypeDef *port, uint16_t pin)
{
    HAL_GPIO_WritePin(port, pin, GPIO_PIN_RESET);
}

/**
 * @brief Toggle GPIO pin
 * @param port GPIO port
 * @param pin GPIO pin
 */
static inline void hw_gpio_toggle(GPIO_TypeDef *port, uint16_t pin)
{
    HAL_GPIO_TogglePin(port, pin);
}

/**
 * @brief Read GPIO pin state
 * @param port GPIO port
 * @param pin GPIO pin
 * @return GPIO pin state (GPIO_PIN_SET or GPIO_PIN_RESET)
 */
static inline GPIO_PinState hw_gpio_read(GPIO_TypeDef *port, uint16_t pin)
{
    return HAL_GPIO_ReadPin(port, pin);
}

/* End of conditional compilation block for ENABLE_HW_GPIO */
#else /* !ENABLE_HW_GPIO */

/* ======================================================================== */
/*                        STUB FUNCTIONS FOR DISABLED MODULE            */
/* ======================================================================== */

/**
 * @brief Stub function for when GPIO is disabled
 * @return HAL_ERROR to indicate module is not available
 */
static inline HAL_StatusTypeDef hw_gpio_init(void)
{
    return HAL_ERROR;
}

/**
 * @brief Stub function for when GPIO is disabled
 * @return HAL_ERROR to indicate module is not available
 */
static inline HAL_StatusTypeDef hw_gpio_config_input(GPIO_TypeDef *port, uint16_t pin)
{
    (void)port;
    (void)pin;
    return HAL_ERROR;
}

/**
 * @brief Stub function for when GPIO is disabled
 * @return HAL_ERROR to indicate module is not available
 */
static inline HAL_StatusTypeDef hw_gpio_config_output(GPIO_TypeDef *port, uint16_t pin,
                                                   uint32_t pull, uint32_t speed)
{
    (void)port;
    (void)pin;
    (void)pull;
    (void)speed;
    return HAL_ERROR;
}

/**
 * @brief Stub function for when GPIO is disabled
 */
static inline void hw_gpio_set(GPIO_TypeDef *port, uint16_t pin)
{
    (void)port;
    (void)pin;
}

/**
 * @brief Stub function for when GPIO is disabled
 */
static inline void hw_gpio_reset(GPIO_TypeDef *port, uint16_t pin)
{
    (void)port;
    (void)pin;
}

/**
 * @brief Stub function for when GPIO is disabled
 */
static inline void hw_gpio_toggle(GPIO_TypeDef *port, uint16_t pin)
{
    (void)port;
    (void)pin;
}

/**
 * @brief Stub function for when GPIO is disabled
 * @return GPIO_PIN_RESET to indicate module is not available
 */
static inline GPIO_PinState hw_gpio_read(GPIO_TypeDef *port, uint16_t pin)
{
    (void)port;
    (void)pin;
    return GPIO_PIN_RESET;
}

#endif /* ENABLE_HW_GPIO */

#if defined __cplusplus
}
#endif

#endif /* _HW_GPIO_H */
