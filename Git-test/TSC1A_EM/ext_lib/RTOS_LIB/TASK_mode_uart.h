#ifndef INC_TASK_mode_uart_H_
#define INC_TASK_mode_uart_H_
#include "main.h"
#include "TASK_TSC1.h"
#include "REG_TSC1.h"
#include "RTOS_LIB/TASK_mode_mtm.h"

void sendHeapUsageUART();
void fcn_mode_uart_Nothing();
enum class uart_enum_t: uint8_t
{
	Nothing = 0,
};

__weak void TASK_mode_uart(void* argument){
	for(;;){

		osDelay(ram_data.mode_uart_delay_ms);
		switch(ram_data.mode_uart){
			case (uint8_t)uart_enum_t::Nothing :
				fcn_mode_uart_Nothing();
				ram_data.mode_uart_delay_ms = 50;
				break;
			default:
#ifdef debug_uart
				ram_data.mode_uart = 0;
#endif
				break;
		} //switch end
	}//loop end
 }//function end
__weak void fcn_mode_uart_Nothing(){
	

        size_t freeHeap = xPortGetFreeHeapSize();
        size_t minHeap = xPortGetMinimumEverFreeHeapSize();

        sendHeapUsageUART();

        vTaskDelay(pdMS_TO_TICKS(1000)); // Delay 1 second


	//add code below

}

void sendHeapUsageUART() {
    char buffer[50];  // Buffer to store the formatted string
    size_t freeHeap = xPortGetFreeHeapSize();
    size_t minHeap = xPortGetMinimumEverFreeHeapSize();

    // Format heap usage as ASCII string
    snprintf(buffer, sizeof(buffer), "Free: %u, Min: %u\r\n", (unsigned int)freeHeap, (unsigned int)minHeap);

    // Transmit over UART
    HAL_UART_Transmit(&huart5, (uint8_t*)buffer, strlen(buffer), HAL_MAX_DELAY);
}
#endif
