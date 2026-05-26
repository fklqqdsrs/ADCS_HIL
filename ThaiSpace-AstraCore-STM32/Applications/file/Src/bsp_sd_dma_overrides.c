#include "app_modules_config.h"
#if CSP_ENABLE_FILE

/*
 * Project: File V3 for TSCNarit
 * Author: Chaturong Khachaban
 * Description: Flight-grade embedded system module
 */

#include "stm32l4xx_hal.h"
#include "bsp_driver_sd.h"

extern SD_HandleTypeDef hsd1;

#define MSD_OK ((uint8_t)0x00)
#define MSD_ERROR ((uint8_t)0x01)
#define MSD_BUSY ((uint8_t)0x02)

static volatile uint8_t sd_rx_done = 0;
static volatile uint8_t sd_tx_done = 0;

uint8_t BSP_SD_ReadBlocks_DMA(uint32_t *pData, uint32_t ReadAddr, uint32_t NumOfBlocks) {
	uint8_t sd_state = MSD_OK;

	__HAL_DMA_DISABLE(hsd1.hdmarx);
	hsd1.hdmarx->Init.Direction = DMA_PERIPH_TO_MEMORY;
	hsd1.hdmarx->Instance->CCR &= ~DMA_CCR_DIR;

	if (HAL_SD_ReadBlocks_DMA(&hsd1, (uint8_t*) pData, ReadAddr, NumOfBlocks) != HAL_OK) {
		sd_state = MSD_ERROR;
	}

	return sd_state;
}

uint8_t BSP_SD_WriteBlocks_DMA(uint32_t *pData, uint32_t WriteAddr, uint32_t NumOfBlocks) {
	uint8_t sd_state = MSD_OK;

	__HAL_DMA_DISABLE(hsd1.hdmatx);
	hsd1.hdmatx->Init.Direction = DMA_MEMORY_TO_PERIPH;
	hsd1.hdmatx->Instance->CCR |= (uint32_t) DMA_CCR_DIR;

	if (HAL_SD_WriteBlocks_DMA(&hsd1, (uint8_t*) pData, WriteAddr, NumOfBlocks) != HAL_OK) {
		sd_state = MSD_ERROR;
	}

	return sd_state;
}


#endif /* CSP_ENABLE_FILE */
