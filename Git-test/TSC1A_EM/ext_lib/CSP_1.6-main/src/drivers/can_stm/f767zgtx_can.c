/*
 * f767zgtx_can.c
 *
 *  Created on: Jul 26, 2024
 *      Author: thaib
 */


/*
 * l496zg_can.c
 *
 *  Created on: Apr 17, 2024
 *      Author: TSC-PC
 */

#include "include/csp/csp_debug.h"

#include "cmsis_os.h"
#include <FreeRTOS.h>
#include <semphr.h>
#include <task.h>

#include <include/csp/csp.h>
#include <include/csp/csp_endian.h>
#include <include/csp/interfaces/csp_if_can.h>

#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <src/drivers/can_stm/f767zgtx_can.h>
#if CSP_HAVE_CAN
#if CSP_SELECT_BOARD == BOARD_stm32l767_hal

/* The `#include CONFIG_STM32_HAL` line is attempting to include a header file named
`CONFIG_STM32_HAL`. This header file likely contains configuration settings or definitions specific
to the STM32 HAL (Hardware Abstraction Layer) for the STM32 microcontroller family. */
#include CONFIG_STM32_HAL
extern CAN_HandleTypeDef CONFIG_CSP_CAN;
extern CAN_HandleTypeDef hcan2;

/* This code snippet is defining a static structure `mcan_s` that represents a CAN driver
configuration. It includes fields such as `mode`, `id`, `mask`, `ifdata`, `lock`, `lock_buf`, and
`interface`. */
/** Driver configration */
#define CSP_IF_CAN_DEFAULT_NAME1 "CAN_A"
static struct mcan_s {
	can_mode_e mode;
	uint32_t id;
	uint32_t mask;
	csp_can_interface_data_t ifdata;
	SemaphoreHandle_t lock;
	StaticSemaphore_t lock_buf;
	csp_iface_t interface;
} mcan[2] = { { .mode = 0, .lock = NULL, .interface = { .name =
CSP_IF_CAN_DEFAULT_NAME, .interface_data = &mcan[0].ifdata, .driver_data = &mcan[0], }, },

{ .mode = 0, .lock = NULL, .interface = { .name =
		CSP_IF_CAN_DEFAULT_NAME1, .interface_data = &mcan[0].ifdata, .driver_data = &mcan[0], }, } };

void delay(uint32_t milliseconds) {
    uint32_t i;
    for (i = 0; i < (milliseconds * 10); ++i) {
    	asm("NOP");
    }
}

/* The above code is defining a task function `socketcan_rx_task` that handles receiving CAN messages.
The code includes conditional compilation based on the `OS_VERSION` macro to define the task
differently for different operating system versions (CMSIS_V1, CMSIS_V2, or CSP_THREAD). */
#if CAN_TASK
CAN_RxHeaderTypeDef rxHeader;

#if OS_VERSION == CMSIS_V1
osThreadId socketcan_rx_tHandle;
void socketcan_rx_task(void const * argument);

#elif OS_VERSION == CMSIS_V2
osThreadId_t socketcan_rx_tHandle;
const osThreadAttr_t usart_rx_t_attributes = {
  .name = "socketcan_rx_t",
  .priority = (osPriority_t) osPriorityHigh,
  .stack_size = 512
};
void socketcan_rx_task(void const * argument);

#elif OS_VERSION == CSP_THREAD
CSP_DEFINE_TASK(socketcan_rx_task);
#else
#endif


#if OS_VERSION == CMSIS_V1 ||OS_VERSION == CMSIS_V2
void socketcan_rx_task(void const *argument) {
	static uint8_t rxData[8];
	printf("socketcan_rx_task started\r\n");
	while (1) {
		while (HAL_CAN_GetRxFifoFillLevel(&CONFIG_CSP_CAN, CAN_RX_FIFO0) > 0) {
			if (HAL_CAN_GetRxMessage(&CONFIG_CSP_CAN, CAN_RX_FIFO0, &rxHeader, rxData)
					== HAL_OK) {
				csp_can_rx(&mcan[0].interface, rxHeader.ExtId, rxData,rxHeader.DLC, NULL);
			}
		}
		osDelay(1);
	}
}
#elif  OS_VERSION == CSP_THREAD
CSP_DEFINE_TASK(socketcan_rx_task) {
	static uint8_t rxData[8];
		printf("socketcan_rx_task started\r\n");
		while (1) {
			while (HAL_CAN_GetRxFifoFillLevel(&CONFIG_CSP_CAN, CAN_RX_FIFO0) > 0) {
				if (HAL_CAN_GetRxMessage(&CONFIG_CSP_CAN, CAN_RX_FIFO0, &rxHeader, rxData)
						== HAL_OK) {
					csp_can_rx(&mcan[0].interface, rxHeader.ExtId, rxData,rxHeader.DLC, NULL);
				}
			}
			osDelay(1);
		}
}

#else
#endif
#else

CAN_RxHeaderTypeDef RxHeader;
uint8_t RxData[8];
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan) {
	int xTaskWoken = pdTRUE;

	if(hcan->Instance == CAN3){
	if (HAL_CAN_GetRxMessage(&CONFIG_CSP_CAN, CAN_RX_FIFO0, &RxHeader, RxData) != HAL_OK) {
		Error_Handler();
	}
//	csp_hex_dump("DATA:", RxData, RxHeader.DLC);
	csp_can_rx(&mcan[0].interface, RxHeader.ExtId, RxData, RxHeader.DLC, (BaseType_t*) &xTaskWoken);
	if (xTaskWoken) {
		portYIELD();
	}
	//HAL_CAN_ActivateNotification(hcan, CAN_IT_RX_FIFO0_MSG_PENDING);
	}

	if(hcan->Instance == CAN2){
		if (HAL_CAN_GetRxMessage(&hcan2, CAN_RX_FIFO0, &RxHeader, RxData) != HAL_OK) {
			Error_Handler();
		}
	//	csp_hex_dump("DATA:", RxData, RxHeader.DLC);
		csp_can_rx(&mcan[1].interface, RxHeader.ExtId, RxData, RxHeader.DLC, (BaseType_t*) &xTaskWoken);
		if (xTaskWoken) {
			portYIELD();
		}
	//	HAL_CAN_ActivateNotification(hcan, CAN_IT_RX_FIFO0_MSG_PENDING);
	}
}

#endif

int csp_can_tx_frame(void *driver_data, uint32_t Identifier, const uint8_t *data, uint8_t dlc) {
	CAN_TxHeaderTypeDef TxHeader;
	struct mcan_s *driver = (struct mcan_s*) driver_data;
	if ((driver == NULL) || (driver->lock == NULL)) {
		csp_log_error("driver == NULL) or (driver->lock == NULL\r\n");
		return CSP_ERR_DRIVER;
	}
	if (xSemaphoreTake(driver->lock, 10) != pdTRUE)	return CSP_ERR_TIMEDOUT;
	csp_sleep_ms(1);
	TxHeader.ExtId = Identifier;
	TxHeader.IDE = CAN_ID_EXT;
	TxHeader.RTR = CAN_RTR_DATA;
	TxHeader.DLC = dlc;

	uint8_t attempts = 3;
	while ((attempts > 0)&& (HAL_CAN_AddTxMessage(&CONFIG_CSP_CAN, &TxHeader, data,(uint32_t*) CAN_TX_MAILBOX0)!= HAL_OK)) {
		osDelay(200);
		attempts--;
	}
	xSemaphoreGive(driver->lock);
	if (attempts > 0) return CSP_ERR_NONE;
    else return CSP_ERR_BUSY;
}

int csp_can1_tx_frame(void *driver_data, uint32_t Identifier, const uint8_t *data, uint8_t dlc) {
	CAN_TxHeaderTypeDef TxHeader;
	struct mcan_s *driver = (struct mcan_s*) driver_data;
	if ((driver == NULL) || (driver->lock == NULL)) {
		csp_log_error("driver == NULL) or (driver->lock == NULL\r\n");
		return CSP_ERR_DRIVER;
	}
	if (xSemaphoreTake(driver->lock, 10) != pdTRUE) return CSP_ERR_TIMEDOUT;
	delay(100);

	TxHeader.ExtId = Identifier;
	TxHeader.IDE = CAN_ID_EXT;
	TxHeader.RTR = CAN_RTR_DATA;
	TxHeader.DLC = dlc;

	uint8_t attempts = 3;
	while ((attempts > 0) && (HAL_CAN_AddTxMessage(&hcan2, &TxHeader, data, (uint32_t*) CAN_TX_MAILBOX0) != HAL_OK)) {
		osDelay(200);
		attempts--;
	}
	xSemaphoreGive(driver->lock);
	if (attempts > 0) return CSP_ERR_NONE;
	else return CSP_ERR_BUSY;
}




int csp_driver_can_init( int id, can_mode_e mode,csp_iface_t **return_iface) {
	mcan[id].lock = xSemaphoreCreateMutexStatic(&mcan[id].lock_buf);
	const csp_conf_t *csp_conf = csp_get_conf();
	if (mode == CSP_CAN_MASKED) {
		mcan[id].mask = CFP_MAKE_DST((1 << CFP_HOST_SIZE) - 1);
		mcan[id].id =CFP_MAKE_DST(csp_get_address());
	} else if (mode == CSP_CAN_PROMISC) {
		mcan[id].mask = 0;
	}

	mcan[id].ifdata.tx_func = csp_can_tx_frame;
	mcan[id].interface.interface_data = &mcan[id].ifdata;

	int res = csp_can_add_interface(&mcan[id].interface);
	if (res != CSP_ERR_NONE) {
		return CSP_ERR_INVAL;
	}

#if CAN_TASK
#if OS_VERSION == CMSIS_V1
	osThreadDef(socketcan_rx_t, socketcan_rx_task, osPriorityHigh, 0, 512);
		socketcan_rx_tHandle = osThreadCreate(osThread(socketcan_rx_t), NULL);
		   if (socketcan_rx_tHandle == NULL) {
				        printf("Failed to create thread : socketcan_rx_task : CMSIS_V1\n");
				    }
#elif OS_VERSION == CMSIS_V2
		 i2c_rx_tHandle = osThreadNew(socketcan_rx_task, NULL, &can_rx_t_attributes);
		    if (i2c_rx_tHandle == NULL) {
		        printf("Failed to create thread : socketcan_rx_task : CMSIS_V2\n");
		    }
#elif OS_VERSION == CSP_THREAD
		    csp_thread_func_t routine = socketcan_rx_task;
		    const char * const thread_name = "socketcan_rx_t";
		    unsigned int stack_size = 512;
		    void * parameters = NULL;
		    unsigned int priority = osPriorityHigh;
		    csp_thread_handle_t return_handle;
	int result = csp_thread_create(routine, thread_name, stack_size, parameters, priority, &return_handle);
	  if (result != CSP_ERR_NONE) {
		  printf("Failed to create thread : socketcan_rx_task : csp thread\n");
 }
#else
#error "Unknown OS_VERSION"
#endif
#else
	  HAL_CAN_Start(&CONFIG_CSP_CAN);
	if (HAL_CAN_ActivateNotification(&CONFIG_CSP_CAN, CAN_IT_RX_FIFO0_MSG_PENDING)
			!= HAL_OK) {
		csp_log_error("HAL_CAN_ActivateNotification != HAL_OK\r\n");
		Error_Handler();
	}
#endif

	//csp_can_config_filter(mcan[id], mcan[id].id, mcan[id].mask);
	if (return_iface) {
		*return_iface = &mcan[id].interface;
	}
	return CSP_ERR_NONE;
}
int csp_driver_can_init1(int id, can_mode_e mode, csp_iface_t **return_iface) {
	mcan[id].lock = xSemaphoreCreateMutexStatic(&mcan[id].lock_buf);
//	const csp_conf_t *csp_conf = csp_get_conf();
	if (mode == CSP_CAN_MASKED) {
		mcan[id].mask = CFP_MAKE_DST((1 << CFP_HOST_SIZE) - 1);
		mcan[id].id = CFP_MAKE_DST(csp_get_address());
	} else if (mode == CSP_CAN_PROMISC) {
		mcan[id].mask = 0;
	}

	mcan[id].ifdata.tx_func = csp_can1_tx_frame;
	mcan[id].interface.interface_data = &mcan[id].ifdata;

	int res = csp_can_add_interface(&mcan[id].interface);
	if (res != CSP_ERR_NONE) {
		return CSP_ERR_INVAL;
	}

#if CAN_TASK
#if OS_VERSION == CMSIS_V1
	osThreadDef(socketcan_rx_t, socketcan_rx_task, osPriorityHigh, 0, 512);
		socketcan_rx_tHandle = osThreadCreate(osThread(socketcan_rx_t), NULL);
		   if (socketcan_rx_tHandle == NULL) {
				        printf("Failed to create thread : socketcan_rx_task : CMSIS_V1\n");
				    }
#elif OS_VERSION == CMSIS_V2
		 i2c_rx_tHandle = osThreadNew(socketcan_rx_task, NULL, &can_rx_t_attributes);
		    if (i2c_rx_tHandle == NULL) {
		        printf("Failed to create thread : socketcan_rx_task : CMSIS_V2\n");
		    }
#elif OS_VERSION == CSP_THREAD
		    csp_thread_func_t routine = socketcan_rx_task;
		    const char * const thread_name = "socketcan_rx_t";
		    unsigned int stack_size = 512;
		    void * parameters = NULL;
		    unsigned int priority = osPriorityHigh;
		    csp_thread_handle_t return_handle;
	int result = csp_thread_create(routine, thread_name, stack_size, parameters, priority, &return_handle);
	  if (result != CSP_ERR_NONE) {
		  printf("Failed to create thread : socketcan_rx_task : csp thread\n");
 }
#else
#error "Unknown OS_VERSION"
#endif
#else
	HAL_CAN_Start(&hcan2);
	if (HAL_CAN_ActivateNotification(&hcan2, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK) {
		csp_log_error("HAL_CAN_ActivateNotification != HAL_OK\r\n");
		Error_Handler();
	}
#endif

	//csp_can_config_filter(mcan[id].id, mcan[id].mask);
	if (return_iface) {
		*return_iface = &mcan[id].interface;
	}
	return CSP_ERR_NONE;
}
/**


/**
 * The function `csp_can_config_filter` configures a CAN filter for a specified CAN controller with
 * given ID and netmask values.
 * 
 * @param mcan The `mcan` parameter is a pointer to a structure of type `mcan_s`, which likely
 * represents a CAN controller or module in your system.
 * @param id The `id` parameter in the `csp_can_config_filter` function represents the identifier used
 * for filtering CAN messages. It is a 32-bit value that is typically used to match against the ID of
 * incoming CAN messages.
 * @param netmask The `netmask` parameter in the `csp_can_config_filter` function is used to specify
 * the network mask for the CAN filter configuration. It is a 32-bit value that is used to mask the
 * incoming CAN messages based on their identifiers. The filter will only allow messages through if
 * their identifiers
 */
void csp_can_config_filter(struct mcan_s *mcan,  uint32_t id, uint32_t netmask) {
    CAN_FilterTypeDef sFilterConfig;
    sFilterConfig.FilterBank = 0;
    sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
    sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
    sFilterConfig.FilterIdHigh = id >> 16;
    sFilterConfig.FilterIdLow = id & 0xFFFF;
    sFilterConfig.FilterMaskIdHigh = netmask >> 16;
    sFilterConfig.FilterMaskIdLow = netmask & 0xFFFF;
    sFilterConfig.FilterFIFOAssignment = CAN_RX_FIFO0;
    sFilterConfig.FilterActivation = ENABLE;
    if (HAL_CAN_ConfigFilter(&CONFIG_CSP_CAN, &sFilterConfig) != HAL_OK) {
    	printf("fail config filter\r\n");
        Error_Handler();
    }
}

#endif
#endif


