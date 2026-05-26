/*
 * l496zg-p_i2c.h
 *
 *  Created on: May 12, 2024
 *      Author: HP
 */

#ifndef SRC_DRIVERS_I2C_L496ZG_P_I2C_H_
#define SRC_DRIVERS_I2C_L496ZG_P_I2C_H_
#if CSP_HAVE_I2C
#if CSP_SELECT_BOARD == BOARD_stm32l4xx_hal
#ifdef __cplusplus
extern "C"
{
#endif

#include CONFIG_STM32_HAL
#include "cmsis_os.h"
#include <FreeRTOS.h>
#include <semphr.h>
#include <task.h>
#include <include/csp/arch/csp_thread.h>
#include <include/csp/interfaces/csp_if_i2c.h>

#define MAX_SLAVE_COUNT 5
#define BUFFER_SIZE_I2C 256
#define CHUNK_SIZE 32

#define I2C_MTU 256
#define I2C_MASTER 0
#define I2C_SLAVE 1

	typedef enum
	{
		I2C_MODE_MASTER,
		I2C_MODE_SLAVE
	} i2c_mode;

#define HEADER_SIZE 2

	typedef struct
	{
		uint8_t status;
		uint8_t dataLength;
	} I2CHeader;

	typedef enum
	{
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

	typedef struct
	{
		uint8_t slave_count;
		uint32_t addresses[MAX_SLAVE_COUNT];
		uint8_t count;
	} SlaveAddressList;

	typedef struct
	{
		char name[CSP_IFLIST_NAME_MAX + 1];
		i2c_mode mode;
		uint32_t address;
		csp_i2c_interface_data_t ifdata;
		SlaveAddressList slaveList;
		status_slave status_slave_t;
		SemaphoreHandle_t i2c_lock;
		StaticSemaphore_t i2c_lock_buf;
		csp_packet_t packet_t;
		csp_iface_t iface;
	} i2c_context_t;

	extern I2C_HandleTypeDef CONFIG_CSP_I2C;

	int csp_i2c_init(uint32_t i2c_addr, uint32_t netmask, const char *ifname, i2c_mode mode, csp_iface_t **return_iface);
	void slave_i2c_rx_callback();
	void initializeSlaveAddressList(i2c_context_t *ctx);
	void addSlaveAddress(i2c_context_t *ctx, uint16_t address);
	int isAddressInList(i2c_context_t *ctx, uint16_t address);
	void initializeI2cRxData();
	void resetRxCount();
	bool isBufferEmpty();
	bool isBufferalmostfull();
	bool isRxBufferFull();
	bool pushToRxBuffer(uint8_t data);
	bool pullFromRxBuffer(uint8_t *data, uint8_t len);
	uint8_t popFromRxBuffer();
	void clearBuffers();
	void i2c_struct_prepareHeader(I2CHeader *header, uint8_t status,
								  uint8_t dataLength);
	void i2c_struct_headerToByteArray(uint8_t *buffer, I2CHeader *header);
uint8_t get_register_i2c(uint32_t address_slave);
void HAL_I2C_ListenCpltCallback(I2C_HandleTypeDef *hi2c);
void HAL_I2C_AddrCallback(I2C_HandleTypeDef *hi2c, uint8_t TransferDirection,
		uint16_t AddrMatchCode);

		void HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef *hi2c) ;
		void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *hi2c);
		void slave_i2c_rx_callback();
		void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *hi2c) ;
		int csp_i2c_tx_master(void *driver_data, csp_i2c_frame_t *frame);
		int csp_i2c_tx_slave(void *driver_data, csp_i2c_frame_t *frame) ;

#ifdef __cplusplus
}
#endif
#endif

#endif

#endif /* SRC_DRIVERS_I2C_L496ZG_P_I2C_H_ */
