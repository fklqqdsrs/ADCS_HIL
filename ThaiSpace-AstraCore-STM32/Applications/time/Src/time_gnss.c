#include <app_modules_config.h>

#if ENABLE_TIME_GNSS

//! @file time_gnss.c
//! @brief C Source File Template

// --------------------------------------------------------------------------------------------------- //
// COMPANY NAME : IOT POW.
//
// INTRODUCTION : GNSS time source.
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
#define __THIS_FILE_NAME__		"time_gnss.c"

// ---------- (2) STANDARD LIBRARY INCLUDE ----------------------------------------------------------- //
#include <kservice.h>

// ---------- (3) USER INCLUDE ----------------------------------------------------------------------- //
#include "time_gnss.h"
#include "GNSS.h"
#include "time_manager_core.h"
#include "thread_manager_core.h"

// ---------- (4) EXTERN OBJECT ---------------------------------------------------------------------- //
// N/A

// ---------- (5) INTERFACE FUNCTION ---------------------------------------------------------- //
// N/A

// ---------- (6) DATA ------------------------------------------------------------------------ //
// N/A

// ---------- (7) PRIVATE PROGRAMMING DEFINE -------------------------------------------------------- //
#define TIME_GNSS_DEBUG
#define gnssTime	"gnssTime"

// ---------- (8) PRIVATE TYPEDEF -------------------------------------------------------------------- //
// N/A

// ---------- (9) PRIVATE FUNCTION ------------------------------------------------------------------- //
void time_gnss_task(void *argument);

// ---------- (10) PRIVATE DATA ----------------------------------------------------------------------- //
/* Definitions for rx_hw_console_sem */
osSemaphoreId_t rx_gnss_sem;
const osSemaphoreAttr_t rx_gnss_sem_attributes = {
  .name = "rx_gnss_sem"
};
// ---------- (11) PRIVATE MACRO DEFINITION ---------------------------------------------------------- //
TSC_THREAD_REGISTER(gnssTime, time_gnss_task, 0, NULL, 1024, osPriorityNormal, 0, 0, 0, 0);

// ---------- (12) SOURCE FILE IMPLEMENTATION -------------------------------------------------------- //
void time_gnss_rx_finish(UART_HandleTypeDef *huart) {
	if(huart->Instance == TSC_CTP_UART_GNSS.Instance)
		osSemaphoreRelease(rx_gnss_sem);
}

void time_gnss_rx_error(UART_HandleTypeDef *huart) {
	if(huart->Instance == TSC_CTP_UART_GNSS.Instance)
		HAL_UART_AbortReceive_IT(&TSC_CTP_UART_GNSS);
}

void time_gnss_task(void *argument) {
	uint32_t tick_next = osKernelGetTickCount(); // Initial reference time

	GNSS_Init(&GNSS_Handle, &TSC_CTP_UART_GNSS);
	osDelay(1000);
	GNSS_LoadConfig(&GNSS_Handle);
	osDelay(1000);
	GNSS_SetMode(&GNSS_Handle, Stationary);

	/* Infinite loop */
	while (1) {
		GNSS_GetPVTData(&GNSS_Handle);
		if(!osSemaphoreAcquire(rx_gnss_sem, 2000)) {
			if(!GNSS_ParseBuffer(&GNSS_Handle)) {
#ifdef TIME_GNSS_DEBUG
				printf("Day: %d-%d-%d \r\n", GNSS_Handle.day, GNSS_Handle.month, GNSS_Handle.year);
				printf("Time: %d:%d:%d \r\n", GNSS_Handle.hour, GNSS_Handle.min, GNSS_Handle.sec);
				printf("Status of fix: %d \r\n", GNSS_Handle.fixType);
				printf("Latitude: %f \r\n", GNSS_Handle.fLat);
				printf("Longitude: %f \r\n", (float) GNSS_Handle.lon / 10000000.0);
				printf("Height above ellipsoid: %ld \r\n", GNSS_Handle.height);
				printf("Height above mean sea level: %ld \r\n", GNSS_Handle.hMSL);
				printf("Ground Speed (2-D): %ld \r\n", GNSS_Handle.gSpeed);
				printf("Unique ID: %04X %04X %04X %04X %04X %04X \r\n",
						GNSS_Handle.uniqueID[0], GNSS_Handle.uniqueID[1],
						GNSS_Handle.uniqueID[2], GNSS_Handle.uniqueID[3],
						GNSS_Handle.uniqueID[4], GNSS_Handle.uniqueID[5]);
#endif
				if(GNSS_Handle.fixType == 3) {
					time_manager_set_time(GNSS_Handle.hour, GNSS_Handle.min, GNSS_Handle.sec);
					time_manager_set_date(GNSS_Handle.year, GNSS_Handle.month, GNSS_Handle.day);
					tick_next += 60000;
				}
				else {
					tick_next += 1000;
				}
			}
			else {
				tick_next += 1000;
			}
			osDelayUntil(tick_next);
		}
		else{
			HAL_UART_AbortReceive_IT(&TSC_CTP_UART_GNSS);
		}
	}
}

void time_gnss_init(void) {
	rx_gnss_sem = osSemaphoreNew(1, 0, &rx_gnss_sem_attributes);
	thread_start(gnssTime);
}

// ---------- (13) END OF SOURCE FILE IMPLEMENTATION ------------------------------------------------- //
// SOURCE FORMAT REVISION B

#endif /* ENABLE_TIME_GNSS */
