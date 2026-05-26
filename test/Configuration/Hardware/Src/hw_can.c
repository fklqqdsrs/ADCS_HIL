//! @file hw_can.c
//! @brief C Source File Template

// --------------------------------------------------------------------------------------------------- //
// COMPANY NAME : IOT POW.
//
// INTRODUCTION : TSC CAN hardware config.
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
#define __THIS_FILE_NAME__		"hw_can.c"

// ---------- (2) STANDARD LIBRARY INCLUDE ----------------------------------------------------------- //
#include <kservice.h>
#include "hw_can.h"

// ---------- (3) USER INCLUDE ----------------------------------------------------------------------- //
#include "main.h"

/* Only compile this entire file if CAN is enabled */
#if ENABLE_HW_CAN

// ---------- (4) EXTERN OBJECT ---------------------------------------------------------------------- //
extern void csp_can_stm32_tx_callback(CAN_HandleTypeDef *hcan, portBASE_TYPE *pxTaskWoken);
extern void csp_can_stm32_rx_callback(CAN_HandleTypeDef *hcan, portBASE_TYPE *pxTaskWoken);

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
// N/A

// ---------- (11) PRIVATE MACRO DEFINITION ---------------------------------------------------------- //
// N/A

// ---------- (12) SOURCE FILE IMPLEMENTATION -------------------------------------------------------- //
void HAL_CAN_TxMailbox0CompleteCallback(CAN_HandleTypeDef *hcan)
{
	// release the semaphore here
	/* The xHigherPriorityTaskWoken parameter must be initialized to pdFALSE as
	 it will get set to pdTRUE inside the interrupt safe API function if a
	 context switch is required. */
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;

	csp_can_stm32_tx_callback(hcan, &xHigherPriorityTaskWoken);

	/* Pass the xHigherPriorityTaskWoken value into portEND_SWITCHING_ISR(). If
	 xHigherPriorityTaskWoken was set to pdTRUE inside xSemaphoreGiveFromISR()
	 then calling portEND_SWITCHING_ISR() will request a context switch. If
	 xHigherPriorityTaskWoken is still pdFALSE then calling
	 portEND_SWITCHING_ISR() will have no effect */
	portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
}


void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan) {
	// release the semaphore here
	/* The xHigherPriorityTaskWoken parameter must be initialized to pdFALSE as
	 it will get set to pdTRUE inside the interrupt safe API function if a
	 context switch is required. */
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;

	csp_can_stm32_rx_callback(hcan, &xHigherPriorityTaskWoken);

	/* Pass the xHigherPriorityTaskWoken value into portEND_SWITCHING_ISR(). If
	 xHigherPriorityTaskWoken was set to pdTRUE inside xSemaphoreGiveFromISR()
	 then calling portEND_SWITCHING_ISR() will request a context switch. If
	 xHigherPriorityTaskWoken is still pdFALSE then calling
	 portEND_SWITCHING_ISR() will have no effect */
	portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
}

// ---------- (13) END OF SOURCE FILE IMPLEMENTATION ------------------------------------------------- //
// SOURCE FORMAT REVISION B
#endif
