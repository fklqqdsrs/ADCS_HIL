//! @file time_pps.c
//! @brief C Source File Template

// --------------------------------------------------------------------------------------------------- //
// COMPANY NAME : IOT POW.
//
// INTRODUCTION : PPS time source.
//
// AUTHOR(S) : Pongpot Chaiboonuang
// CONTACT E-MAIL ADDRESS : pongpot.c@gmail.com
//
// REFERENCE DOCUMENT :
// N/A
//
// NOTE :
// N/A
//
// --------------------------------------------------------------------------------------------------- //

// ---------- I M P O R T A N T   N O T E ------------------------------------------------------------ //
//
// --------------------------------------------------------------------------------------------------- //
// N/A

// ---------- (1) LOCAL PREDEFINE -------------------------------------------------------------------- //
#define __THIS_FILE_NAME__		"time_pps.c"

// ---------- (2) STANDARD LIBRARY INCLUDE ----------------------------------------------------------- //
#include <kservice.h>
#include "app_modules_config.h"

// ---------- (3) USER INCLUDE ----------------------------------------------------------------------- //
#include <time_pps_it.h>
#include <time_manager_core.h>

#if ENABLE_TIME_PPS && ENABLE_HW_GPIO
// ---------- (4) EXTERN OBJECT ---------------------------------------------------------------------- //
// N/A

// ---------- (5) INTERFACE FUNCTION ----------------------------------------------------------------- //
// N/A

// ---------- (6) DATA ------------------------------------------------------------------------------- //
// N/A

// ---------- (7) PRIVATE PROGRAMMING DEFINE --------------------------------------------------------- //
// N/A

// ---------- (8) PRIVATE TYPEDEF -------------------------------------------------------------------- //
// N/A

// ---------- (9) PRIVATE FUNCTION ------------------------------------------------------------------- //
// N/A

// ---------- (10) PRIVATE DATA ----------------------------------------------------------------------- //
__IO time_t ppsTick;
__IO uint32_t ppsUpdateTick;
__IO uint8_t ppsNotSync;

// ---------- (11) PRIVATE MACRO DEFINITION ---------------------------------------------------------- //
// N/A

// ---------- (12) SOURCE FILE IMPLEMENTATION -------------------------------------------------------- //

/**
  * @brief This function handles EXTI line3 interrupt.
  */
void EXTI3_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI3_IRQn 0 */

  /* USER CODE END EXTI3_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(TSC_TIME_PPS_GPIO_PIN);
  /* USER CODE BEGIN EXTI3_IRQn 1 */

  /* USER CODE END EXTI3_IRQn 1 */
}

/**
 * @brief This function is called to increment  a global variable "ppsTick"
 *        used as PPS time source.
 * @note In the default implementation, this variable is incremented each 1s
 *       in PPS ISR.
 * @note This function is declared as __weak to be overwritten in case of other
 *      implementations in user file.
 * @retval None
 */
__weak void time_pps_interrupt(uint16_t GPIO_Pin) {
	if (GPIO_Pin == TSC_TIME_PPS_GPIO_PIN) {
		if((osKernelGetTickCount() - ppsUpdateTick) < 1100){
			ppsTick++;
			ppsUpdateTick = osKernelGetTickCount();
		}
		else {
			ppsNotSync = 1;
		}
	}
}

/**
 * @brief Provides a PPS value in second.
 * @note This function is declared as __weak to be overwritten in case of other
 *       implementations in user file.
 * @retval PPS value
 */
__weak int time_pps_get(time_t *pps) {

	if(((osKernelGetTickCount() - ppsUpdateTick) < 1100) && (!ppsNotSync)) {
		if(pps)
			*pps = ppsTick;

		return TSC_EOK;
	}
	else {
		ppsNotSync = 1;

		if(pps)
			*pps = (time_t)-1;

		return -TSC_ERROR;
	}
}

/**
 * @brief Set a PPS value in second.
 * @note This function is declared as __weak to be overwritten in case of other
 *       implementations in user file.
 * @param time_t pps set current time to PPS.
 * @retval None
 */
__weak int time_pps_set(time_t pps) {
    struct tm tm = {0};

    gmtime_r(&pps, &tm);
    if (tm.tm_year < 100)
    {
        return -TSC_ERROR;
    }

	ppsTick = pps;
	ppsNotSync = 0;
	ppsUpdateTick = osKernelGetTickCount();

	return TSC_EOK;
}

/**
 * @brief Initialize gpio for received PPS signal.
 * @note This function is declared as __weak to be overwritten in case of other
 *       implementations in user file.
 * @retval None
 */
__weak void time_pps_hal_int(void) {
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOG_CLK_ENABLE();

	/*Configure GPIO pin : PG3 */
	GPIO_InitStruct.Pin = TSC_TIME_PPS_GPIO_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	HAL_GPIO_Init(TSC_TIME_PPS_GPIO_PORT, &GPIO_InitStruct);

	/* EXTI interrupt init*/
	HAL_NVIC_SetPriority(TSC_TIME_PPS_GPIO_EXTI, 5, 0);
	HAL_NVIC_EnableIRQ(TSC_TIME_PPS_GPIO_EXTI);
}

/**
 * @brief Initialize time PPS.
 * @note This function is declared as __weak to be overwritten in case of other
 *       implementations in user file.
 * @retval None
 */
__weak void time_pps_int(void) {
	ppsTick=-1;
	ppsUpdateTick = 0;
	ppsNotSync=1;

	time_pps_hal_int();
}

int time_manager_get_pps_time(time_t *now) {
	return time_pps_get(&*now);
}

int time_manager_set_pps_time(time_t now) {
	return time_pps_set(now);
}

uint8_t time_manager_is_pps_syn(void) {
	 if(((osKernelGetTickCount() - ppsUpdateTick) < 1100) && (!ppsNotSync)) {
		 return TIME_MANAGER_SYNC;
	 }
	 else {
		 ppsNotSync = 1;
		 return TIME_MANAGER_NOT_SYNC;
	 }
}

#endif //#if ENABLE_TIME_PPS && ENABLE_HW_GPIO

// ---------- (13) END OF SOURCE FILE IMPLEMENTATION ------------------------------------------------- //
// SOURCE FORMAT REVISION B
