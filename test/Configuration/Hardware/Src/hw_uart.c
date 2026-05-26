//! @file hw_uart.c
//! @brief C Source File Template

// --------------------------------------------------------------------------------------------------- //
// COMPANY NAME : IOT POW.
//
// INTRODUCTION : TSC USART hardware config.
//
// AUTHOR(S) : Pongpot Chaiboonuang refactored by chaturong khachaban
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
#define __THIS_FILE_NAME__		"hw_uart.c"

// ---------- (2) STANDARD LIBRARY INCLUDE ----------------------------------------------------------- //
#include <kservice.h>
#include "hw_uart.h"
#include "cmsis_os.h"

// ---------- (3) USER INCLUDE ----------------------------------------------------------------------- //
#include "time_gnss.h"
#include "main.h"

/* Only compile this entire file if UART is enabled */
#if ENABLE_HW_UART
// ---------- (4) EXTERN OBJECT ---------------------------------------------------------------------- //
/**
   CSP uart tx dma interrupt.
   @param[in] huart uart handler pointer.
   @param[in] task_woken pointer to xHigherPriorityTaskWoken .
 */
extern void csp_kiss_tx_int(UART_HandleTypeDef *huart, BaseType_t *task_woken);
/**
   CSP uart rx dma idle interrupt.
   @param[in] huart uart handler pointer.
   @param[in] uart rx data size.
   @param[in] task_woken pointer to xHigherPriorityTaskWoken .
*/
extern void csp_kiss_rx_idle(UART_HandleTypeDef *huart, uint16_t Size, BaseType_t *task_woken);
/**
   CSP uart rx dma error interrupt.
   @param[in] huart uart handler pointer.
*/
extern void csp_kiss_rx_error(UART_HandleTypeDef *huart);

#endif /* ENABLE_HW_UART */



/* Console functions - only available when TSC_USING_FINSH is enabled */
#ifdef TSC_USING_FINSH
extern osSemaphoreId_t rx_console_sem;

// ---------- (10) PRIVATE DATA ----------------------------------------------------------------------- //
static uint8_t ch[128];
static uint16_t ch_len;

// ---------- (11) PRIVATE MACRO DEFINITION ---------------------------------------------------------- //
#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

// ---------- (12) SOURCE FILE IMPLEMENTATION -------------------------------------------------------- //

/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
  HAL_UART_Transmit(&TSC_CSP_UART_CONSOLE, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
  return ch;
}

void tsc_hw_console_rx_int(UART_HandleTypeDef *huart, uint16_t Size) {
	if(huart->Instance == TSC_CSP_UART_CONSOLE.Instance) {
		ch_len = Size;
		osSemaphoreRelease(rx_console_sem);
	}
}

void tsc_hw_console_rx_error(UART_HandleTypeDef *huart) {
	if(huart->Instance == TSC_CSP_UART_CONSOLE.Instance) {
		HAL_UARTEx_ReceiveToIdle_IT(&TSC_CSP_UART_CONSOLE, (uint8_t*) &ch, sizeof(ch));
	}
}

void tsc_hw_console_output(const char *str)
{
	/* empty console output */
	size_t num = 0, length = strlen(str);

	do {
		if(*(str + num) != '\n')
			HAL_UART_Transmit(&TSC_CSP_UART_CONSOLE, (uint8_t*) (str + num), 1, HAL_MAX_DELAY);
		else
			HAL_UART_Transmit(&TSC_CSP_UART_CONSOLE, (uint8_t*) "\r\n", 2, HAL_MAX_DELAY);
	} while (++num < length);
}

char tsc_hw_console_getchar(void) {
	static bool ch_buff_tx=false;
	static uint16_t ch_ptr=0;

	if(ch_buff_tx){
		if(ch_ptr < ch_len){
			return ch[ch_ptr++];
		}
	}

	HAL_UARTEx_ReceiveToIdle_IT(&TSC_CSP_UART_CONSOLE, (uint8_t*) &ch, sizeof(ch));

	osSemaphoreAcquire(rx_console_sem, osWaitForever);
	ch_buff_tx = false;
	ch_ptr = 0;

	if(ch_len > 1){
		ch_buff_tx = true;
		return ch[ch_ptr++];
	}else{
		return ch[0];
	}
}

#endif /* TSC_USING_FINSH */



void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
	// release the semaphore here
	/* The xHigherPriorityTaskWoken parameter must be initialized to pdFALSE as
	 it will get set to pdTRUE inside the interrupt safe API function if a
	 context switch is required. */
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
#if ENABLE_HW_UART
	csp_kiss_tx_int(huart, &xHigherPriorityTaskWoken);
#endif /* ENABLE_HW_UART */	
	/* Pass the xHigherPriorityTaskWoken value into portEND_SWITCHING_ISR(). If
	 xHigherPriorityTaskWoken was set to pdTRUE inside xSemaphoreGiveFromISR()
	 then calling portEND_SWITCHING_ISR() will request a context switch. If
	 xHigherPriorityTaskWoken is still pdFALSE then calling
	 portEND_SWITCHING_ISR() will have no effect */
	portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	// release the semaphore here
	/* The xHigherPriorityTaskWoken parameter must be initialized to pdFALSE as
	 it will get set to pdTRUE inside the interrupt safe API function if a
	 context switch is required. */
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;

#if ENABLE_TIME_GNSS
	time_gnss_rx_finish(huart);
#endif /* ENABLE_TIME_GNSS */

	/* Pass the xHigherPriorityTaskWoken value into portEND_SWITCHING_ISR(). If
	 xHigherPriorityTaskWoken was set to pdTRUE inside xSemaphoreGiveFromISR()
	 then calling portEND_SWITCHING_ISR() will request a context switch. If
	 xHigherPriorityTaskWoken is still pdFALSE then calling
	 portEND_SWITCHING_ISR() will have no effect */
	portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size) {
#if ENABLE_HW_UART
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	csp_kiss_rx_idle(huart, Size, &xHigherPriorityTaskWoken);
	portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
#endif /* ENABLE_HW_UART */

	#ifdef TSC_USING_FINSH
	tsc_hw_console_rx_int(huart, Size);
#endif /* TSC_USING_FINSH */
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

#if ENABLE_HW_UART
    csp_kiss_rx_error(huart);
#endif

#if ENABLE_TIME_GNSS
    time_gnss_rx_error(huart);
#endif

#ifdef TSC_USING_FINSH
    tsc_hw_console_rx_error(huart);
#endif

    portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
}

// ---------- (13) END OF SOURCE FILE IMPLEMENTATION ------------------------------------------------- //
// SOURCE FORMAT REVISION B
