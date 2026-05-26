/*
 * l496zg_can.h
 *
 *  Created on: Apr 17, 2024
 *      Author: TSC-PC
 */

#ifndef SRC_DRIVERS_CAN_STM_L496ZG_CAN_H_
#define SRC_DRIVERS_CAN_STM_L496ZG_CAN_H_
#include <include/csp/interfaces/csp_if_can.h>
#if CSP_HAVE_CAN
#if CSP_SELECT_BOARD == BOARD_stm32l4xx_hal
#ifdef __cplusplus
extern "C" {
#endif
#include CONFIG_STM32_HAL
extern CAN_HandleTypeDef CONFIG_CSP_CAN;
#define INITIAL_VALUE 0

typedef enum {
    CSP_CAN_MASKED,
    CSP_CAN_PROMISC
} can_mode_e;


/**
 * The function `delay` takes an input in milliseconds and loops for a calculated number of iterations
 * to introduce a delay.
 * 
 * @param milliseconds The `milliseconds` parameter in the `delay` function represents the duration for
 * which the function will pause execution. It specifies the number of milliseconds for which the delay
 * should occur.
 */
void delay(uint32_t milliseconds);



/**
 * The function `csp_can_tx_frame` sends a CAN frame using the specified driver data, identifier, data,
 * and data length with error handling for driver and lock conditions.
 * 
 * @param driver_data The `driver_data` parameter is a void pointer that points to the driver data
 * structure. In this function, it is cast to a pointer of type `struct mcan_s` to access the driver
 * data members.
 * @param Identifier The `Identifier` parameter in the `csp_can_tx_frame` function is the unique
 * identifier for the CAN message being transmitted. It is a 32-bit unsigned integer (`uint32_t`) that
 * represents the message ID.
 * @param data The `data` parameter in the `csp_can_tx_frame` function is a pointer to an array of
 * uint8_t type, which represents the data to be transmitted over the CAN bus. The data array contains
 * the actual payload that will be sent in the CAN frame.
 * @param dlc The `dlc` parameter in the `csp_can_tx_frame` function stands for Data Length Code. It
 * indicates the number of bytes of data being transmitted in the CAN frame. The value of `dlc` should
 * be between 0 and 8, representing the valid range of data bytes
 * 
 * @return The function `csp_can_tx_frame` returns either `CSP_ERR_NONE` if the transmission is
 * successful, or `CSP_ERR_BUSY` if the transmission fails after the maximum number of attempts.
 */
int csp_can_tx_frame(void *driver_data, uint32_t Identifier,const uint8_t *data , uint8_t dlc);


/**
 * The function `csp_driver_can_init` initializes a CAN driver with specified parameters and creates a
 * thread for receiving CAN messages based on the operating system version.
 * 
 * @param id The `id` parameter in the `csp_driver_can_init` function represents the identifier of the
 * CAN driver instance that you want to initialize. It is used to access the specific CAN driver
 * instance within the function.
 * @param mode The `mode` parameter in the `csp_driver_can_init` function is of type `can_mode_e`,
 * which is an enumeration type representing different modes for the CAN interface. The function uses
 * this parameter to configure the CAN interface based on the specified mode.
 * @param return_iface The `return_iface` parameter in the `csp_driver_can_init` function is a pointer
 * to a pointer to a `csp_iface_t` structure. This parameter is used to return a pointer to the
 * initialized CAN interface structure back to the caller of the function. By setting `*return_iface
 * 
 * @return The function `csp_driver_can_init` returns an integer value, which is `CSP_ERR_NONE` if the
 * function execution is successful.
 */
int csp_driver_can_init( int id, can_mode_e mode,csp_iface_t **return_iface);
int csp_driver_can_init1(int id, can_mode_e mode, csp_iface_t **return_iface);
void csp_can_config_filter(uint32_t id, uint32_t mask);

#ifdef __cplusplus
}
#endif
#endif
#endif
#endif /* SRC_DRIVERS_CAN_STM_L496ZG_CAN_H_ */
