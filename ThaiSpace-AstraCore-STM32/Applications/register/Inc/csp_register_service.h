/*
 * csp_register_service.h
 *
 *  Created on: Sep 18, 2024
 *      Author: thaib
 */

#ifndef SRC_REGISTER_CSP_REGISTER_SERVICE_H_
#define SRC_REGISTER_CSP_REGISTER_SERVICE_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <csp/csp.h>
#include <csp/arch/csp_time.h>
#include "bit_management.h"
#include "register_protocol.h"

#ifdef __cplusplus
extern "C" {
#endif

// --- Shared Protocol Definitions are in register_protocol.h ---

/**
 * @brief Union to hold register values of different types.
 * This is used by the server-side handlers to interpret incoming data.
 */
typedef union {
	uint8_t u8;
	int8_t i8;
	uint16_t u16;
	int16_t i16;
	uint32_t u32;
	int32_t i32;
	float f;
	double d;
} RegisterValue;

// --- Server-Side Function Declarations ---

/**
 * A generic helper function to add a register of any supported type.
 * Used by the service handler.
 */
bool generic_add_register(uint8_t device_id, uint8_t register_id, uint16_t sub_register, uint8_t type_register, RegisterValue *value, uint8_t access);

/**
 * A generic helper function to set a register's value of any supported type.
 * Used by the service handler.
 */
bool generic_set_register(uint8_t device_id, uint8_t register_id, uint16_t sub_register, uint8_t type_register, RegisterValue *value);

/**
 * The main service handler that listens on a CSP port, receives packets,
 * and processes register-related actions (ADD, WRITE, READ).
 */
void service_register(csp_conn_t *conn, csp_packet_t *packet);

#ifdef __cplusplus
}
#endif

#endif /* SRC_REGISTER_CSP_REGISTER_SERVICE_H_ */
