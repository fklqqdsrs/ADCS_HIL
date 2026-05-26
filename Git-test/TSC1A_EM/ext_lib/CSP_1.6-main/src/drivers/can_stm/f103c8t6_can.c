/*
 * f103c8t6_can.c
 *
 *  Created on: Jul 14, 2024
 *      Author: thaib
 */

#include "include/csp/csp_debug.h"

#include <string.h>
#include "cmsis_os.h"

#include <include/csp/csp.h>
#include <include/csp/csp_endian.h>
#include <include/csp/interfaces/csp_if_can.h>

#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <src/drivers/can_stm/f103c8t6_can.h>

#if CSP_HAVE_CAN
#if CSP_SELECT_BOARD == BOARD_stm32flxx_hal
/** Driver configration */
static struct mcan_s {
	can_mode_e mode;
	uint32_t id;
	uint32_t mask;
	csp_can_interface_data_t ifdata;
	SemaphoreHandle_t lock;
	StaticSemaphore_t lock_buf;
	csp_iface_t interface;
} mcan[1] = { { .mode = 0, .lock = NULL, .interface = { .name =
CSP_IF_CAN_DEFAULT_NAME, .interface_data = &mcan[0].ifdata, .driver_data =
		&mcan[0], }, } };

#include CONFIG_STM32_HAL
extern CAN_HandleTypeDef CONFIG_CSP_CAN;

void delay(uint32_t milliseconds) {
    uint32_t i;
    for (i = 0; i < (milliseconds * 10); ++i) {
    //	asm("NOP");
    }
}

#if (CAN_TASK)
osThreadId socketcan_rx_tHandle;
void socketcan_rx_task(void const *argument) {
	CAN_RxHeaderTypeDef rxHeader;
	static uint8_t rxData[8];
	printf("socketcan_rx_task started\r\n");
	while (1) {
		while (HAL_CAN_GetRxFifoFillLevel(&CONFIG_CSP_CAN, CAN_RX_FIFO0) > 0) {
			if (HAL_CAN_GetRxMessage(&CONFIG_CSP_CAN, CAN_RX_FIFO0, &rxHeader, rxData)
					== HAL_OK) {
				csp_can_rx(&mcan[0].interface, rxHeader.ExtId, rxData,rxHeader.DLC, NULL);
			}
		}
		osDelay(0);
	}
	/* We should never reach this point */
}
#else

CAN_RxHeaderTypeDef   RxHeader;
uint8_t               RxData[8];

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan){
	int xTaskWoken = pdTRUE;
  if (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader, RxData) == HAL_OK)
  {
	  csp_can_rx(&mcan[0].interface, RxHeader.ExtId, RxData,RxHeader.DLC, &xTaskWoken);
  }
	if (xTaskWoken) {
		portYIELD();
	}
}


#endif

int csp_can_tx_frame(void *driver_data, uint32_t Identifier,
		const uint8_t *data, uint8_t dlc) {
	CAN_TxHeaderTypeDef TxHeader;
	struct mcan_s *driver = (struct mcan_s*) driver_data;
	if ((driver == NULL) || (driver->lock == NULL)) {
		return 0;
	}

	if (xSemaphoreTake(driver->lock, 10) != pdTRUE)
		return CSP_ERR_TIMEDOUT;
	delay(100);
	TxHeader.ExtId = Identifier;
	TxHeader.IDE = CAN_ID_EXT;
	TxHeader.RTR = CAN_RTR_DATA;
	TxHeader.DLC = dlc;

	uint8_t attempts = 3;
	while ((attempts > 0)
			&& (HAL_CAN_AddTxMessage(&CONFIG_CSP_CAN, &TxHeader, data, CAN_TX_MAILBOX0)
					!= HAL_OK)) {
		printf("HAL_CAN_AddTxMessage!=HAL_OK\r\n");
		osDelay(50);
		attempts--;
	}

	xSemaphoreGive(driver->lock);
	if (attempts > 0) {
		return CSP_ERR_NONE;
	} else {
		return CSP_ERR_BUSY;
	}
}

int csp_driver_can_init(int id, can_mode_e mode,csp_iface_t **return_iface) {

	/* Create a mutex type semaphore. */
	mcan[id].lock = xSemaphoreCreateMutexStatic(&mcan[id].lock_buf);

	const csp_conf_t *csp_conf = csp_get_conf();

	/* Generate ID */

	mcan[id].id = CFP_MAKE_DST(csp_get_address());
	if (mode == CSP_CAN_MASKED) {
		mcan[id].mask =  CFP_MAKE_DST((1 << CFP_HOST_SIZE) - 1);
	} else if (mode == CSP_CAN_PROMISC) {
		mcan[id].mask = 0;
	}

	mcan[id].ifdata.tx_func = csp_can_tx_frame;
	mcan[id].interface.interface_data = &mcan[id].ifdata;

	int res = csp_can_add_interface(&mcan[id].interface);
	if (res != CSP_ERR_NONE) {
		return CSP_ERR_INVAL;
	}

	HAL_CAN_Start(&CONFIG_CSP_CAN);

#if CAN_TASK
	osThreadDef(socketcan_rx_t, socketcan_rx_task, osPriorityHigh, 0, 512);
	socketcan_rx_tHandle = osThreadCreate(osThread(socketcan_rx_t), NULL);
#else
	  if (HAL_CAN_ActivateNotification(&CONFIG_CSP_CAN, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK)
	  {
		  Error_Handler();
	  }
#endif


	// Filter for own address
	csp_can_config_filter(&mcan[id], 0, mcan[id].id, mcan[id].mask);

	if (return_iface) {
		*return_iface = &mcan[id].interface;
	}
	return CSP_ERR_NONE;
}

void csp_can_config_filter(struct mcan_s *mcan, int filter_bank, uint32_t id,
		uint32_t mask) {
	CAN_FilterTypeDef sFilterConfig;
	sFilterConfig.FilterBank = filter_bank;
	sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
	sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
	sFilterConfig.FilterIdHigh = id >> 16;
	sFilterConfig.FilterIdLow = id & 0xFFFF;
	sFilterConfig.FilterMaskIdHigh = mask >> 16;
	sFilterConfig.FilterMaskIdLow = mask & 0xFFFF;
	sFilterConfig.FilterFIFOAssignment = CAN_RX_FIFO0;
	sFilterConfig.FilterActivation = ENABLE;

	if (HAL_CAN_ConfigFilter(&CONFIG_CSP_CAN, &sFilterConfig) != HAL_OK) {
		// Filter configuration error
		Error_Handler();
	}
}

#endif
#endif
