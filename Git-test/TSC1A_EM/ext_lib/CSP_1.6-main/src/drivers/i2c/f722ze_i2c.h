/*
 * f722ze_i2c.h
 *
 *  Created on: May 12, 2024
 *      Author: HP
 */

#ifndef SRC_DRIVERS_I2C_F722ZE_I2C_H_
#define SRC_DRIVERS_I2C_F722ZE_I2C_H_

#pragma once
#if CSP_HAVE_I2C
#if CSP_SELECT_BOARD == BOARD_stm32f7xx_hal
#include <include/csp/interfaces/csp_if_i2c.h>

#define MAX_SLAVE_COUNT 5
#define BUFFER_SIZE_I2C 256
#define CHUNK_SIZE 32

#define I2C_MTU     256
#define I2C_MASTER  0
#define I2C_SLAVE   1



//   typedef void (*i2c_callback_t) (csp_i2c_frame_t * frame, void * pxTaskWoken);
//typedef struct csp_i2c_interface_data {
//    csp_packet_t *pbufs;
//} csp_i2c_interface_data_t;

typedef enum {
    I2C_MODE_MASTER,
    I2C_MODE_SLAVE
} i2c_mode;

#define HEADER_SIZE 2

typedef struct {
    uint8_t status;
    uint8_t dataLength;
} I2CHeader;

    typedef enum {
    	NONE_NULL,
    	MASTER_SEND,
    	MASTER_RECEIVER,
    	SLAVE_SEND,
    	SLAVE_RECEIVER,
		OTHER1,
		OTHER2,
		OTHER3,
		OTHER4,
		NONE_ERROR,
    } status_slave;

typedef struct {
	uint8_t slave_count;
    uint32_t addresses[MAX_SLAVE_COUNT];
    uint8_t count;
}SlaveAddressList;

int csp_i2c_init(uint32_t i2c_addr, uint32_t netmask, const char *ifname,i2c_mode mode, csp_iface_t **return_iface) ;
#if CSP_SELECT_BOARD == BOARD_stm32g4xx_hal
#include "stm32g4xx_hal.h"
#endif
#endif

#endif
#endif /* SRC_DRIVERS_I2C_F722ZE_I2C_H_ */
