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
#include <src/drivers/can_stm/g431kb_can.h>

#if CSP_HAVE_CAN
#if CSP_SELECT_BOARD == BOARD_stm32g4xx_hal

/* The `#include CONFIG_STM32_HAL` line is attempting to include a header file named
 `CONFIG_STM32_HAL`. This header file likely contains configuration settings or definitions specific
 to the STM32 HAL (Hardware Abstraction Layer) for the STM32 microcontroller family. */
#include CONFIG_STM32_HAL
extern FDCAN_HandleTypeDef CONFIG_CSP_CAN;

/* This code snippet is defining a static structure `mcan_s` that represents a CAN driver
 configuration. It includes fields such as `mode`, `id`, `mask`, `ifdata`, `lock`, `lock_buf`, and
 `interface`. */
/** Driver configuration */
static struct mcan_s {
	can_mode_e mode;
	uint32_t id;
	uint32_t mask;
	csp_can_interface_data_t ifdata;
	SemaphoreHandle_t lock;
	StaticSemaphore_t lock_buf;
	csp_iface_t interface;
} mcan[1] = { { .mode = 0, .lock = NULL, .interface = { .name =
CSP_IF_CAN_DEFAULT_NAME, .interface_data = &mcan[0].ifdata, .driver_data = &mcan[0], }, } };

/**
 * The function `delay` takes an input in milliseconds and loops for a calculated number of iterations
 * to create a delay.
 * 
 * @param milliseconds The `milliseconds` parameter in the `delay` function represents the duration for
 * which the function will pause or delay the program execution. It specifies the number of
 * milliseconds for which the delay should occur.
 */
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
FDCAN_RxHeaderTypeDef RxHeader;
uint8_t               RxData[8];
int xTaskWoken = pdFALSE;
void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs){

  if((RxFifo0ITs & FDCAN_IT_RX_FIFO0_NEW_MESSAGE) != RESET){
    /* Retreive Rx messages from RX FIFO0 */
    if (HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, &RxHeader, RxData) != HAL_OK) {
    /* Reception Error */
    Error_Handler();
    }

    if (HAL_FDCAN_ActivateNotification(hfdcan, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0) != HAL_OK) {
      /* Notification Error */
      Error_Handler();
    }
    csp_can_rx(&mcan[0].interface, RxHeader.Identifier, RxData, (uint32_t)RxHeader.DataLength >> 16U,&xTaskWoken);
	if (xTaskWoken) {
		portYIELD();
	}
  }
}

#endif

int csp_can_tx_frame(void *driver_data, uint32_t Identifier,
		const uint8_t *data, uint8_t dlc) {
	FDCAN_TxHeaderTypeDef TxHeader;
	struct mcan_s *driver = (struct mcan_s*) driver_data;
	if ((driver == NULL) || (driver->lock == NULL)) {
		return 0;
	}
	/* Task locking */
	if (xSemaphoreTake(driver->lock, 1) != pdTRUE)
		return CSP_ERR_TIMEDOUT;  // Timeout waiting for semaphore
	osDelay(0);
	TxHeader.Identifier = Identifier;
	TxHeader.IdType = FDCAN_EXTENDED_ID;
	TxHeader.TxFrameType = FDCAN_DATA_FRAME;
	TxHeader.DataLength = (uint32_t) dlc << 16U;
	TxHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
	TxHeader.BitRateSwitch = FDCAN_BRS_ON;
	TxHeader.FDFormat = FDCAN_CLASSIC_CAN;

	int attempts = 3;
	while ((attempts > 0)
			&& (HAL_FDCAN_AddMessageToTxFifoQ(&CONFIG_CSP_CAN, &TxHeader, data)
					!= HAL_OK)) {
		osDelay(50);
		attempts--;
	}
	/* Unlock */
	xSemaphoreGive(driver->lock);

	if (attempts > 0) {
		return CSP_ERR_NONE;
	} else {
		return CSP_ERR_BUSY;
	}
}



int csp_driver_can_init(int id, can_mode_e mode, csp_iface_t **return_iface) {
	mcan[id].lock = xSemaphoreCreateMutexStatic(&mcan[id].lock_buf);
	const csp_conf_t *csp_conf = csp_get_conf();
	if (mode == CSP_CAN_MASKED) {
		mcan[id].mask = CFP_MAKE_DST((1 << CFP_HOST_SIZE) - 1);
		mcan[id].id = CFP_MAKE_DST(csp_get_address());
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
	HAL_FDCAN_Start(&CONFIG_CSP_CAN);
	if (HAL_FDCAN_ActivateNotification(&CONFIG_CSP_CAN, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0) != HAL_OK) {
		csp_log_error("HAL_FDCAN_ActivateNotification != HAL_OK\r\n");
		Error_Handler();
	}
#endif

	if (return_iface) {
		*return_iface = &mcan[id].interface;
	}
	return CSP_ERR_NONE;
}

#endif
#endif
