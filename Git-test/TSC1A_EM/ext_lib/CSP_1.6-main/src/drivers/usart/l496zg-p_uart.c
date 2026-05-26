/*
 * l496zg-p_uart.c
 *
 *  Created on: May 7, 2024
 *      Author: HP
 */

#include <src/drivers/usart/f722ze_uart.h>

#include "cmsis_os.h"
#include <FreeRTOS.h>
#include <semphr.h>
#include <task.h>

#include <include/csp/csp_debug.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/time.h>

#include <include/csp/csp.h>
#include <main.h>

#if CSP_HAVE_USART
#if CSP_SELECT_BOARD == BOARD_stm32l4xx_hal

#define ARRAY_LEN(x)            (sizeof(x) / sizeof((x)[0]))
extern UART_HandleTypeDef CONFIG_CSP_UART;

SemaphoreHandle_t usartMutex;
QueueHandle_t cmdQueue;
uint8_t rx_data;

void ClearRxBuffer();
HAL_StatusTypeDef readDATA(UART_HandleTypeDef *huart, uint16_t timeout);

typedef struct {
	csp_usart_callback_t rx_callback;
	void *user_data;
	csp_usart_fd_t fd;
	pthread_t rx_thread;
} usart_context_t;

usart_context_t *ctx = NULL;

#if OS_VERSION == CMSIS_V1
osThreadId usart_rx_tHandle;
void usart_rx_thread(void const * argument);

#elif OS_VERSION == CMSIS_V2
osThreadId_t usart_rx_tHandle;
const osThreadAttr_t usart_rx_t_attributes = {
  .name = "usartRxThread",
  .priority = (osPriority_t) osPriorityHigh,
  .stack_size = 512
};
void usart_rx_thread(void const * argument);

#elif OS_VERSION == CSP_THREAD
CSP_DEFINE_TASK(usart_rx_thread);
#else
#endif


#if OS_VERSION == CMSIS_V1 ||OS_VERSION == CMSIS_V2
void usart_rx_thread(void const * argument) {
	usart_context_t *ctx = (usart_context_t*) argument;
	uint8_t receivedByte;
	for (;;) {
		if (xQueueReceive(cmdQueue, &receivedByte, 1000)) {
			ctx->rx_callback(ctx->user_data, &receivedByte,sizeof(receivedByte), NULL);
		}
	}
}

#elif  OS_VERSION == CSP_THREAD
CSP_DEFINE_TASK(usart_rx_thread) {
	usart_context_t *ctx = (usart_context_t*) argument;
	//csp_log_info("usart rx thread started\r\n");
	uint8_t receivedByte;
	while(1) {
		if (xQueueReceive(cmdQueue, &receivedByte, 1000)) {
			ctx->rx_callback(ctx->user_data, &receivedByte,sizeof(receivedByte), NULL);
		}
	}
}
#else

#endif

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == USART2) {
        HAL_UART_RxCpltCallback_USART2(huart);
    } else if (huart->Instance == USART3) {
        HAL_UART_RxCpltCallback_USART3(huart);
    }
}


void HAL_UART_RxCpltCallback_USART3(UART_HandleTypeDef *huart) {
    BaseType_t xHigherPriorityTaskWoken = pdTRUE;
    xQueueSendFromISR(cmdQueue, &rx_data, &xHigherPriorityTaskWoken);
    if (xHigherPriorityTaskWoken) {
        taskYIELD();
    }
    HAL_UART_Receive_IT(&CONFIG_CSP_UART, &rx_data, 1);
}

int csp_usart_write(csp_usart_fd_t fd, const void *data, size_t data_length) {
	if (fd >= 0) {
		HAL_StatusTypeDef hal_status = HAL_UART_Transmit(&CONFIG_CSP_UART,(uint8_t*) data, data_length, HAL_MAX_DELAY);
		if (hal_status == HAL_OK) {
			return data_length;
		}
	}
	return CSP_ERR_TX;
}


int csp_usart_open(const csp_usart_conf_t *conf,
		csp_usart_callback_t rx_callback, void *user_data,
		csp_usart_fd_t *return_fd) {
	usart_context_t *ctx = calloc(1, sizeof(*ctx));
	if (ctx == NULL) {
		csp_log_warn("%s: Error allocating context, device: [%s], errno: %s\n",
				__FUNCTION__, conf->device, strerror(errno));
		return CSP_ERR_NOMEM;
	}

	ctx->rx_callback = rx_callback;
	ctx->user_data = user_data;
	ctx->fd = 1;
	HAL_UART_Receive_IT(&CONFIG_CSP_UART, &rx_data, 1);
	cmdQueue = xQueueCreate(256, sizeof(uint8_t));
	if (cmdQueue == NULL) {
		csp_log_warn("Create cmdQueue == NUL\r\n");
		while (1);
	}

	if (rx_callback) {


#if OS_VERSION == CMSIS_V1
		osThreadDef(usartRxThread, usart_rx_thread, osPriorityHigh, 0, 512);
		usart_rx_tHandle = osThreadCreate(osThread(usartRxThread), ctx);
		   if (usart_rx_tHandle == NULL) {
				        printf("Failed to create thread : uart_rx_task : CMSIS_V1\n");
				    }
#elif OS_VERSION == CMSIS_V2
		   usart_rx_tHandle = osThreadNew(usart_rx_thread, ctx, &usart_rx_t_attributes);
		    if (usart_rx_tHandle == NULL) {
		        printf("Failed to create thread : uart_rx_task : CMSIS_V2\n");
		    }
#elif OS_VERSION == CSP_THREAD
		    csp_thread_func_t routine = usart_rx_thread;
		    const char * const thread_name = "usartRxThread";
		    unsigned int stack_size = 512;
		    void * parameters = ctx;
		    unsigned int priority = osPriorityHigh;
		    csp_thread_handle_t return_handle;
	int result = csp_thread_create(routine, thread_name, stack_size, parameters, priority, &return_handle);
	  if (result != CSP_ERR_NONE) {
		  printf("Failed to create thread : uart_rx_task : csp thread\n");
 }
#else
		if (xTaskCreate(usart_rx_thread, "usartRxThread", 512, ctx,
				osPriorityRealtime, &ctx->rx_thread) != pdPASS) {
			csp_log_warn(
					"%s: xTaskCreate() failed to create Rx thread for device: [%s], errno: %s\n",
					__FUNCTION__, conf->device, strerror(errno));
			free(ctx);
			return CSP_ERR_NOMEM;
		}
#endif
	}
	if (return_fd) {
		*return_fd = ctx->fd;
	}
	return CSP_ERR_NONE;
}

#endif
#endif
