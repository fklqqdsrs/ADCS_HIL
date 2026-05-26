/*
 * csp_register_client.h
 *
 *  Created on: Jul 23, 2025
 *      Author: Chaturong
 */

#ifndef FUNC_REGISTER_CSP_REGISTER_CLIENT_H_
#define FUNC_REGISTER_CSP_REGISTER_CLIENT_H_

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

// Access (3 bits) used in bit packing
#define R_NONE     0x00
#define R_ENFORCE  0x01
#define R_SUCCESS  0x02
#define R_BUSY     0x03
#define R_TIMEOUT  0x04
#define R_NOT_FOUND 0x05

// --- Client-Side Function Declarations ---

/**
 * Converts a generic value to a packed 64-bit integer and gets its size.
 */
int convert_and_get_size(void *value, uint8_t type_register, uint64_t *packed_value, size_t *value_size);

/**
 * Adds a register on a remote device via CSP.
 * Returns time taken in ms, or a negative CSP error code on failure.
 */
int add_register_csp(unsigned int device_id, unsigned int register_id, unsigned int sub_register, uint8_t type_register, void *value, uint8_t access_mode, uint16_t timeout);

/**
 * Sets a register's value on a remote device via CSP.
 * Returns time taken in ms, or a negative CSP error code on failure.
 */
int set_register_csp(unsigned int device_id, unsigned int register_id, unsigned int sub_register, uint8_t type_register, void *value, uint16_t timeout);

/**
 * Gets a register's value from a remote device via CSP.
 * Returns time taken in ms, or a negative CSP error code on failure.
 */
int get_register_csp(unsigned int device_id, unsigned int register_id, unsigned int sub_register, uint8_t type_register, void *outValue, uint16_t timeout);

/** Remove a register on a remote device. Returns ms >= 0 or negative CSP error. */
int remove_register_csp(unsigned int device_id, unsigned int register_id, unsigned int sub_register, uint16_t timeout);

/** Get used-register count from a remote device. Returns ms >= 0 or negative CSP error. */
int get_register_count_csp(unsigned int device_id, uint32_t *outCount, uint16_t timeout);

/** Get register status (used, available) from a remote device. Returns ms >= 0 or negative CSP error. */
int get_register_status_csp(unsigned int device_id, uint32_t *outUsed, uint32_t *outAvailable, uint16_t timeout);

#ifdef __cplusplus
}
#endif

#endif /* FUNC_REGISTER_CSP_REGISTER_CLIENT_H_ */
