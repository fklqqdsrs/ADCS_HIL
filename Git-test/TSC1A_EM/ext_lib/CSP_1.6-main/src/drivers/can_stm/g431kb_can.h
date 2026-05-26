/*
 * g431kb_can.h
 *
 *  Created on: Apr 5, 2024
 *      Author: TSC-PC
 */

#ifndef SRC_DRIVERS_CAN_STM_G431KB_CAN_H_
#define SRC_DRIVERS_CAN_STM_G431KB_CAN_H_

#include <include/csp/interfaces/csp_if_can.h>



#if CSP_HAVE_CAN
#if CSP_SELECT_BOARD == BOARD_stm32g4xx_hal
typedef enum {
    CSP_CAN_MASKED,
    CSP_CAN_PROMISC
} can_mode_e;


/**
 * The function `delay` takes an input in milliseconds and loops for a calculated number of iterations
 * to create a delay.
 * 
 * @param milliseconds The `milliseconds` parameter in the `delay` function represents the duration for
 * which the function will pause or delay the program execution. It specifies the number of
 * milliseconds for which the delay should occur.
 */
void delay(uint32_t milliseconds);

/**
 * The function `csp_can_tx_frame` sends a CAN frame using FDCAN with error handling and retry
 * mechanism.
 * 
 * @param driver_data The `driver_data` parameter is a void pointer that points to the driver data
 * structure, which is then cast to a pointer of type `struct mcan_s`. This structure contains
 * information about the CAN driver, such as the lock semaphore used for synchronization.
 * @param Identifier The `Identifier` parameter in the `csp_can_tx_frame` function is the message
 * identifier for the CAN frame that you want to transmit. It is a unique value that identifies the
 * message being sent on the CAN bus.
 * @param data The `data` parameter in the `csp_can_tx_frame` function is a pointer to an array of
 * uint8_t type, which represents the data to be transmitted in the CAN frame.
 * @param dlc The `dlc` parameter in the `csp_can_tx_frame` function stands for "Data Length Code" and
 * represents the length of the data being transmitted in bytes. It indicates the number of bytes in
 * the `data` array that should be sent as part of the CAN frame.
 * 
 * @return The function `csp_can_tx_frame` returns either `CSP_ERR_NONE` if the message was
 * successfully added to the transmit FIFO queue within the specified number of attempts, or
 * `CSP_ERR_BUSY` if the message could not be added after the maximum number of attempts.
 */
int csp_can_tx_frame(void *driver_data, uint32_t Identifier,const uint8_t *data , uint8_t dlc);

/**
 * The function `csp_driver_can_init` initializes a CAN interface with specified parameters and returns
 * the interface pointer.
 * 
 * @param addr The `addr` parameter in the `csp_driver_can_init` function represents the address of the
 * CAN device on the network. It is used to set the ID of the CAN interface for communication.
 * @param netmask The `netmask` parameter in the `csp_driver_can_init` function is used to specify the
 * network mask for the CAN interface. The network mask is used in conjunction with the address to
 * determine which messages should be received by the CAN interface. It helps in filtering out unwanted
 * messages based on their
 * @param id The `id` parameter in the `csp_driver_can_init` function is used to specify the identifier
 * of the CAN interface being initialized. It is an integer value that helps identify and differentiate
 * between multiple CAN interfaces if they exist in the system.
 * @param mode The `mode` parameter in the `csp_driver_can_init` function specifies the mode of
 * operation for the CAN interface. It can have one of the following values:
 * @param return_iface The `return_iface` parameter in the `csp_driver_can_init` function is a pointer
 * to a pointer to a `csp_iface_t` structure. This parameter is used to return the interface created
 * within the function to the caller. By setting `*return_iface` to the address of the
 * 
 * @return The function `csp_driver_can_init` is returning an error code of type `CSP_ERR_NONE` if the
 * initialization process is successful. If there is an error during initialization, it will return
 * `CSP_ERR_INVAL`.
 */
int csp_driver_can_init( int id, can_mode_e mode,csp_iface_t **return_iface);

#endif /* SRC_DRIVERS_CAN_G431KB_CAN_H_ */
#endif
#endif
