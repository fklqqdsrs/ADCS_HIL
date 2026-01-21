/*
 * g431kb_i2c.c
 *
 *  Created on: Apr 17, 2024
 *      Author: TSC-PC
 */

#include "include/csp/csp_debug.h"

#include <string.h>
#include "cmsis_os.h"

#include <include/csp/csp.h>
#include <include/csp/interfaces/csp_if_i2c.h>
#include <string.h>
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <src/drivers/i2c/g431kb_i2c.h>

#if CSP_HAVE_I2C
#if CSP_SELECT_BOARD == BOARD_stm32g4xx_hal
#include "main.h"

uint8_t status_buffer_slave[HEADER_SIZE];
uint8_t i2c_RxData[BUFFER_SIZE_I2C];
uint8_t i2c_RxCount = 0;
uint8_t i2c_TxCount = 0;
uint8_t *global_data_to_send = NULL;
uint16_t global_data_length = 0;
bool fiptx = false;
typedef struct {
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

i2c_context_t *global_i2c_ctx = NULL;
I2CHeader *I2CHeader_t;
osThreadId i2c_rx_tHandle;
extern I2C_HandleTypeDef hi2c1;

void slave_i2c_rx_callback();
void Master_i2c_rx_task(void const *argument);
int csp_i2c_init(uint32_t i2c_addr, uint32_t netmask, const char *ifname,
		i2c_mode mode, csp_iface_t **return_iface);

void initializeSlaveAddressList(i2c_context_t *ctx) {
	if (ctx == NULL) {
		return;
	}
	ctx->slaveList.count = 0;
}

void addSlaveAddress(i2c_context_t *ctx, uint16_t address) {
	if (ctx == NULL) {
		return;
	}
	if (ctx->slaveList.count < MAX_SLAVE_COUNT) {
		ctx->slaveList.addresses[ctx->slaveList.count++] = address;
	}
}

int isAddressInList(i2c_context_t *ctx, uint16_t address) {
	if (ctx == NULL) {
		return 0;
	}
	for (int i = 0; i < ctx->slaveList.count; ++i) {
		if (ctx->slaveList.addresses[i] == address) {
			return 1;
		}
	}
	return 0;
}

void initializeI2cRxData() {
//	for(uint8_t num_t =0 ; num_t < 255 ; num_t ++){
//		status_buffer_slave[num_t] = 0x55;
//	}
	//memset(i2c_RxData, 0, BUFFER_SIZE_I2C);
	i2c_RxCount = 0;
}
void resetRxCount() {
	i2c_RxCount = 0;
}
bool isBufferEmpty() {
	if (i2c_RxCount < BUFFER_SIZE_I2C)
		return true;
	return false;
}
bool isBufferalmostfull() {
	return (i2c_RxCount == BUFFER_SIZE_I2C - 1);
}
bool isRxBufferFull() {
	return (i2c_RxCount == BUFFER_SIZE_I2C);
}
bool pushToRxBuffer(uint8_t data) {
	if (i2c_RxCount < BUFFER_SIZE_I2C) {
		i2c_RxData[i2c_RxCount++] = data;
		return true;
	}
	return false;
}
bool pullFromRxBuffer(uint8_t *data, uint8_t len) {
	if (i2c_RxCount >= len) {
		memcpy(data, i2c_RxData, len);
		memmove(i2c_RxData, i2c_RxData + len, BUFFER_SIZE_I2C - len);
		i2c_RxCount -= len;
		return true;
	}
	return false;
}
uint8_t popFromRxBuffer() {
	if (i2c_RxCount > 0) {
		uint8_t data = i2c_RxData[0];
		memmove(i2c_RxData, i2c_RxData + 1, BUFFER_SIZE_I2C - 1);
		i2c_RxCount--;
		return data;
	}
	return 0;
}
void clearBuffers() {
	memset(i2c_RxData, 0xff, BUFFER_SIZE_I2C);
	i2c_RxCount = 0;
}

void i2c_struct_prepareHeader(I2CHeader *header, uint8_t status,
		uint8_t dataLength) {
	if (header == NULL)
		return;
	header->status = status;
	header->dataLength = dataLength;
}

void i2c_struct_headerToByteArray(uint8_t *buffer, I2CHeader *header) {
	if (buffer == NULL || header == NULL)
		return;
	buffer[0] = header->status;
	buffer[1] = header->dataLength;
}

uint8_t get_register_i2c(uint32_t address_slave) {
	HAL_StatusTypeDef status = HAL_I2C_Master_Receive(&hi2c1,
			address_slave << 1, status_buffer_slave, HEADER_SIZE, 1000);
	while (HAL_I2C_GetState(&hi2c1) != HAL_I2C_STATE_READY)
		;
	if (status != HAL_OK) {
		return NONE_ERROR;
	}
	i2c_struct_headerToByteArray(status_buffer_slave, I2CHeader_t);
	switch (status_buffer_slave[0]) {
	case 0:
		return NONE_NULL;
	case 1:
		return MASTER_SEND;
	case 2:
		return MASTER_RECEIVER;
	case 3:
		return SLAVE_SEND;
	case 4:
		return SLAVE_RECEIVER;
	case 5:
		return OTHER1;
	case 6:
		return OTHER2;
	default:
		return NONE_ERROR;
	}
}



void HAL_I2C_ListenCpltCallback(I2C_HandleTypeDef *hi2c) {
	HAL_I2C_EnableListen_IT(hi2c);
}

void HAL_I2C_AddrCallback(I2C_HandleTypeDef *hi2c, uint8_t TransferDirection,uint16_t AddrMatchCode) {
	if (TransferDirection == I2C_DIRECTION_TRANSMIT) {
		i2c_RxCount = 0;
		HAL_I2C_Slave_Sequential_Receive_IT(hi2c, i2c_RxData + i2c_RxCount, 1,
		I2C_FIRST_FRAME);
	} else {
		i2c_TxCount = 0;
		if (fiptx == false)
			HAL_I2C_Slave_Seq_Transmit_IT(hi2c,
					status_buffer_slave + i2c_TxCount, 2, I2C_NEXT_FRAME);
		if (status_buffer_slave[0] == SLAVE_SEND && fiptx == true) {
			HAL_I2C_Slave_Seq_Transmit_IT(hi2c,
					global_data_to_send + i2c_TxCount, global_data_length,
					I2C_NEXT_FRAME);
			status_buffer_slave[0] = 0;
			status_buffer_slave[1] = 0;
			fiptx = false;
		}
		if (status_buffer_slave[0] == SLAVE_SEND) {
			fiptx= true;
		}
	}
}

void HAL_I2C_SlaveTxCpltCallback(I2C_HandleTypeDef *hi2c) {

}

void HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef *hi2c) {
	//i2c_frame->len_rx ++;
	i2c_RxCount++;
	if (i2c_RxCount < BUFFER_SIZE_I2C) {
		if (i2c_RxCount == BUFFER_SIZE_I2C - 1) {
			HAL_I2C_Slave_Sequential_Receive_IT(hi2c, i2c_RxData + i2c_RxCount,
					1,
					I2C_LAST_FRAME);
		} else {
			HAL_I2C_Slave_Sequential_Receive_IT(hi2c, i2c_RxData + i2c_RxCount,
					1,
					I2C_NEXT_FRAME);
		}
	}
	if (i2c_RxCount == BUFFER_SIZE_I2C) {
		slave_i2c_rx_callback();
	}
}

void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *hi2c) {
	uint32_t errorcode = HAL_I2C_GetError(hi2c);
	if (errorcode == 4) {
		 if (global_i2c_ctx->mode == I2C_MODE_SLAVE )slave_i2c_rx_callback();
	}
	if (errorcode == 1) {
		HAL_I2C_DeInit(hi2c);
		HAL_I2C_Init(hi2c);
	}
	resetRxCount();
	if (global_i2c_ctx->mode == I2C_MODE_SLAVE ) HAL_I2C_EnableListen_IT(hi2c);
}

void slave_i2c_rx_callback() {
	int xTaskWoken = pdTRUE;
	if (global_i2c_ctx == NULL) {
		return;
	}
	csp_i2c_frame_t *packet = csp_buffer_get_isr(0);
	global_i2c_ctx->iface.frame++;
	//csp_id_setup_rx(packet);
	memcpy(packet->data, &i2c_RxData, i2c_RxCount);
	packet->len = i2c_RxCount;
	csp_i2c_rx(&global_i2c_ctx->iface, packet, &xTaskWoken);
	//csp_buffer_free_isr(packet);
	i2c_RxCount = 0;
	if (xTaskWoken) {
		portYIELD();
	}
}

void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *hi2c) {
	int xTaskWoken = pdTRUE;
	if (global_i2c_ctx == NULL) {
		return;
	}
	uint8_t length_rx = status_buffer_slave[1];
	csp_i2c_frame_t *packet = csp_buffer_get_isr(0);
	global_i2c_ctx->iface.frame++;
	//csp_id_setup_rx(packet);
	memcpy(packet->data, i2c_RxData, length_rx);
	packet->len = length_rx;
	csp_i2c_rx(&global_i2c_ctx->iface, packet, &xTaskWoken);
	//csp_buffer_free_isr(packet);
	if (xTaskWoken) {
		portYIELD();
	}
}

void Master_i2c_rx_task(void const *argument) {
	printf("Master_i2c_rx_task\r\n");
	i2c_context_t *ctx = (i2c_context_t*)argument ;
	while (1) {
		osDelay(50);
		status_slave status = get_register_i2c(ctx->slaveList.addresses[0]);
		if (status == SLAVE_SEND) {
			uint8_t length_rx = status_buffer_slave[1];
			HAL_I2C_Master_Receive_IT(&hi2c1, ctx->slaveList.addresses[0] << 1, i2c_RxData, length_rx);
		}
	}
}

int csp_i2c_tx_master(void *driver_data, csp_i2c_frame_t *frame) {
    if (driver_data == NULL || frame == NULL) {
        return CSP_ERR_INVAL;
    }
    i2c_context_t *ctx = (i2c_context_t*)driver_data;
    uint16_t data_length = frame->len;
    HAL_I2C_Master_Transmit_IT(&hi2c1, ctx->slaveList.addresses[0] << 1, frame->data, data_length);
    csp_buffer_free(frame);
    return CSP_ERR_NONE ;
}

int csp_i2c_tx_slave(void *driver_data, csp_i2c_frame_t *frame) {
	if (driver_data == NULL || frame == NULL) {
		printf("driver_data == NULL || frame == NULL\r\n");
		return CSP_ERR_INVAL;
	}
	status_buffer_slave[0] = SLAVE_SEND;
	status_buffer_slave[1] = frame->len;
	global_data_to_send = frame->data;
	global_data_length = frame->len;
	csp_buffer_free(frame);
	return CSP_ERR_NONE;
}

int csp_i2c_init(uint32_t i2c_addr, uint32_t netmask, const char *ifname,
		i2c_mode mode, csp_iface_t **return_iface) {

	i2c_context_t *ctx = (i2c_context_t*) calloc(1, sizeof(i2c_context_t));
	if (ctx == NULL) {
		return CSP_ERR_NOMEM;
	}
	initializeI2cRxData();
	ctx->mode = mode ;
	ctx->i2c_lock = xSemaphoreCreateMutexStatic(&ctx->i2c_lock_buf);
	strncpy(ctx->name, ifname, CSP_IFLIST_NAME_MAX);
//	ctx->iface. = i2c_addr;
//	ctx->iface.netmask = netmask;
	ctx->iface.name = ctx->name;
	ctx->iface.driver_data = ctx;
	ctx->iface.interface_data = &ctx->ifdata;

	if (mode == I2C_MODE_MASTER) {
		ctx->ifdata.tx_func = csp_i2c_tx_master;
		osThreadDef(i2c_rx_t, Master_i2c_rx_task, osPriorityHigh, 0, 512);
		i2c_rx_tHandle = osThreadCreate(osThread(i2c_rx_t), ctx);
		initializeSlaveAddressList(ctx);
		addSlaveAddress(ctx, 0x20);
	} else {
		ctx->ifdata.tx_func = csp_i2c_tx_slave;
		if (HAL_I2C_EnableListen_IT(&hi2c1) != HAL_OK) {
			printf("HAL_I2C_EnableListen_IT(&hi2c1) != HAL_OK\r\n");
			Error_Handler();
		}
	}
	global_i2c_ctx = ctx;
	int res = csp_i2c_add_interface(&ctx->iface);
	if (res != CSP_ERR_NONE) {
		printf("%s[%s]: csp_i2c_add_interface() failed, error: %d\n",
				__FUNCTION__, ctx->name, res);
	}
	if (return_iface) {
		*return_iface = &ctx->iface;
	}
	return res;
}

#endif
#endif

