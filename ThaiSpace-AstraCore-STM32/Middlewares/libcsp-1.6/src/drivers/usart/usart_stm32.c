/*
 Cubesat Space Protocol - A small network-layer protocol designed for Cubesats
 Copyright (C) 2012 GomSpace ApS (http://www.gomspace.com)
 Copyright (C) 2012 AAUSAT3 Project (http://aausat3.space.aau.dk)

 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include "cmsis_os.h"
#include <csp/arch/csp_malloc.h>
#include <csp/arch/csp_thread.h>
#include <csp/arch/csp_semaphore.h>
#include <csp/arch/csp_queue.h>
#include <csp/csp.h>
#include <csp/drivers/usart.h>
#include <hw_config.h>

/* Only compile this entire file if I2C is enabled */
#if ENABLE_HW_UART

typedef struct {
	char name[CSP_IFLIST_NAME_MAX + 1];
	csp_usart_callback_t rx_callback;
	void *user_data;
	csp_usart_fd_t fd;
	csp_thread_handle_t thread;
} usart_context_t;

typedef struct {
	const char name[CSP_IFLIST_NAME_MAX + 1];
	UART_HandleTypeDef *huart;
	DMA_HandleTypeDef *hdma_usart_rx, *hdma_usart_tx;
	csp_bin_sem_handle_t uart_tx_sem;
	csp_queue_handle_t uart_rx_que;
	uint8_t cbuf[CONFIG_CSP_UART_RX_BUFFER_LENGTH];
	uint8_t tbuf[2][CONFIG_CSP_UART_TX_BUFFER_LENGTH];
	bool tx_first;
	size_t buff_ptr, buff_num;
} usart_device_t;

usart_device_t usart_device[CONFIG_CSP_UART_DEVICE_NUM] = {{.name = TSC_CSP_UART1_NAME}, {.name = "USART2"}};

void csp_kiss_rx_idle(UART_HandleTypeDef *huart, uint16_t Size, BaseType_t *task_woken) {
	if(huart->Instance == TSC_CSP_UART1.Instance) {
		void *d = (void*) 1;
		csp_queue_enqueue_isr(usart_device[0].uart_rx_que, &d, task_woken);
	}
}

void csp_kiss_rx_error(UART_HandleTypeDef *huart) {
	if(huart->Instance == TSC_CSP_UART1.Instance) {
		HAL_UARTEx_ReceiveToIdle_DMA(usart_device[0].huart, usart_device[0].cbuf, CONFIG_CSP_UART_RX_BUFFER_LENGTH);
		__HAL_DMA_ENABLE_IT(usart_device[0].huart->hdmarx, DMA_IT_HT);
	}
}

void csp_kiss_tx_int(UART_HandleTypeDef *huart, BaseType_t *task_woken) {
	if (huart->Instance == TSC_CSP_UART1.Instance) {
		csp_bin_sem_post_isr(&usart_device[0].uart_tx_sem, task_woken);
	}
}

CSP_DEFINE_TASK(usart_rx_thread){
    static size_t old_pos, pos;
	usart_context_t *ctx = param;
	csp_usart_fd_t fd = ctx->fd;
	void* d;

	HAL_UARTEx_ReceiveToIdle_DMA(usart_device[fd].huart, usart_device[fd].cbuf, CONFIG_CSP_UART_RX_BUFFER_LENGTH);
    __HAL_DMA_ENABLE_IT(usart_device[fd].huart->hdmarx, DMA_IT_HT);

	// Receive loop
	while (1) {
		if(csp_queue_dequeue(usart_device[fd].uart_rx_que, &d, CSP_MAX_TIMEOUT) == CSP_QUEUE_OK){
		    /* Calculate current position in buffer and check for new data available */
		    pos = CONFIG_CSP_UART_RX_BUFFER_LENGTH - __HAL_DMA_GET_COUNTER(usart_device[fd].hdma_usart_rx);
		    if (pos != old_pos) {                       /* Check change in received data */
		        if (pos > old_pos) {                    /* Current position is over previous one */
		            ctx->rx_callback(ctx->user_data, &usart_device[fd].cbuf[old_pos], pos - old_pos, NULL);
		        } else {
		            ctx->rx_callback(ctx->user_data, &usart_device[fd].cbuf[old_pos], CONFIG_CSP_UART_RX_BUFFER_LENGTH - old_pos, NULL);
		            if (pos > 0) {
		                ctx->rx_callback(ctx->user_data, &usart_device[fd].cbuf[0], pos, NULL);
		            }
		        }
		        old_pos = pos;                          /* Save current position as old for next transfers */
		    }
		    (void)d;
		}
	}
	return CSP_TASK_RETURN;
}

int csp_usart_write(csp_usart_fd_t fd, const void *data, size_t data_length) {
	bool tx_data = false, tx_end = false;
	size_t buff_length=0;

	if((data_length == 2) && (!usart_device[fd].tx_first)){
		if((*(uint8_t*)data == 0xC0) && (*((uint8_t*)data+1) == 0x00)){
			usart_device[fd].tx_first = true;
		}
	}else if((data_length == 1) && (usart_device[fd].tx_first)){
		if(*(uint8_t*)data == 0xC0){
			usart_device[fd].tx_first = false;
			tx_end = true;
		}
	}

	if((usart_device[fd].buff_ptr + data_length) > CONFIG_CSP_UART_TX_BUFFER_LENGTH) {
		buff_length = usart_device[fd].buff_ptr;
		usart_device[fd].buff_num = !usart_device[fd].buff_num;
		usart_device[fd].buff_ptr = 0;
		tx_data = true;
	}

	for(size_t i=0; i<data_length; i++){
		usart_device[fd].tbuf[usart_device[fd].buff_num][usart_device[fd].buff_ptr++] = *((uint8_t*)data+i);
	}

	if(tx_data){
		if (csp_bin_sem_wait(&usart_device[fd].uart_tx_sem, CSP_MAX_TIMEOUT) == CSP_SEMAPHORE_OK){
			if (HAL_UART_Transmit_DMA(usart_device[fd].huart, usart_device[fd].tbuf[!usart_device[fd].buff_num], buff_length) != HAL_OK){
				return CSP_ERR_TX;
			}
		}
	}

	if(tx_end && usart_device[fd].buff_ptr){
		if (csp_bin_sem_wait(&usart_device[fd].uart_tx_sem, CSP_MAX_TIMEOUT) == CSP_SEMAPHORE_OK){
			if (HAL_UART_Transmit_DMA(usart_device[fd].huart, usart_device[fd].tbuf[usart_device[fd].buff_num], usart_device[fd].buff_ptr) != HAL_OK){
				usart_device[fd].buff_num = !usart_device[fd].buff_num;
				usart_device[fd].buff_ptr = 0;
				return CSP_ERR_TX;
			}else{
				usart_device[fd].buff_num = !usart_device[fd].buff_num;
				usart_device[fd].buff_ptr = 0;
			}
		}
	}

	return data_length; // best matching CSP error code.
}

int csp_usart1_init (const csp_usart_conf_t *conf){

	usart_device[0].huart = &TSC_CSP_UART1;
	usart_device[0].hdma_usart_tx = &TSC_CSP_UART1_TX_DMA;
	usart_device[0].hdma_usart_rx = &TSC_CSP_UART1_RX_DMA;

	csp_bin_sem_create(&usart_device[0].uart_tx_sem);
	csp_bin_sem_post(&usart_device[0].uart_tx_sem);

	return CSP_ERR_NONE;
}

int csp_usart_init(int num, const csp_usart_conf_t *conf) {
	if(num == 0)
		return csp_usart1_init(conf);

	return CSP_ERR_INVAL;
}

int csp_usart_open(const csp_usart_conf_t *conf, csp_usart_callback_t rx_callback, void *user_data,	csp_usart_fd_t * return_fd) {
	int fd;

	for(fd=0; fd<CONFIG_CSP_UART_DEVICE_NUM; fd++){
		if(strcmp(conf->device, usart_device[fd].name) == 0){
			break;
		}
	}

	if(fd >= CONFIG_CSP_UART_DEVICE_NUM) {
		csp_log_error("%s: Error device: [%s] not found!, errno: %s",
				__FUNCTION__, conf->device, strerror(errno));
		return CSP_ERR_INVAL;
	}

	if(csp_usart_init(fd, conf)) {
		csp_log_error("%s: Error initialize device: [%s] driver, errno: %s",
				__FUNCTION__, conf->device, strerror(errno));
		return CSP_ERR_DRIVER;
	}

	usart_context_t *ctx = csp_calloc(1, sizeof(*ctx));
	if (ctx == NULL) {
		csp_log_error("%s: Error allocating context, device: [%s], errno: %s",
				__FUNCTION__, conf->device, strerror(errno));
		return CSP_ERR_NOMEM;
	}

	strcpy(ctx->name, conf->device);
	ctx->rx_callback = rx_callback;
	ctx->user_data = user_data;
	ctx->fd = fd;

	if (rx_callback) {
		usart_device[fd].uart_rx_que = csp_queue_create(10, sizeof(void *));
		if (csp_thread_create(usart_rx_thread, "usart_rx", 512, ctx, osPriorityRealtime, &ctx->thread) != CSP_ERR_NONE) {
			csp_log_error(
					"%s: csp_thread_create() failed to create Rx thread for device: [%s], errno: %s",
					__FUNCTION__, conf->device, strerror(errno));
			csp_free(ctx);
			return CSP_ERR_NOMEM;
		}
	}

	if (return_fd) {
		*return_fd = fd;
	}

	return CSP_ERR_NONE;
}

#endif
