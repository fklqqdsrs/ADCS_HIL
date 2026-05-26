/*
 * f103c8t6_can.h
 *
 *  Created on: Jul 14, 2024
 *      Author: thaib
 */

#ifndef SRC_DRIVERS_CAN_STM_F103C8T6_CAN_H_
#define SRC_DRIVERS_CAN_STM_F103C8T6_CAN_H_

#include <include/csp/interfaces/csp_if_can.h>

#if CSP_HAVE_CAN
#if CSP_SELECT_BOARD == BOARD_stm32flxx_hal

#define INITIAL_VALUE 0

typedef enum {
    CSP_CAN_MASKED,
    CSP_CAN_PROMISC
} can_mode_e;

int csp_can_tx_frame(void *driver_data, uint32_t Identifier,const uint8_t *data , uint8_t dlc);
int csp_driver_can_init(int id, can_mode_e mode,	csp_iface_t **return_iface);

#endif
#endif

#endif /* SRC_DRIVERS_CAN_STM_F103C8T6_CAN_H_ */
