#ifndef INC_TASK_mode_uart_H_
#define INC_TASK_mode_uart_H_
#include "main.h"
#include "TASK_ABOARD.h"
#include "REG_ABOARD.h"
void fcn_mode_uart_Nothing();
void fcn_mode_uart_verbose();
void fcn_mode_uart_debug();
enum class uart_enum_t: uint8_t
{
	Nothing = 0,
	verbose = 1,
	debug = 2,
};

__weak void TASK_mode_uart(void* argument){
	for(;;){

		osDelay(ram_data.mode_uart_delay_ms);
		switch(ram_data.mode_uart){
			case (uint8_t)uart_enum_t::Nothing :
				fcn_mode_uart_Nothing();
				ram_data.mode_uart_delay_ms = 1000;
				break;
			case (uint8_t)uart_enum_t::verbose :
				fcn_mode_uart_verbose();
				ram_data.mode_uart_delay_ms = 1000;
				break;
			case (uint8_t)uart_enum_t::debug :
				fcn_mode_uart_debug();
				ram_data.mode_uart_delay_ms = 1000;
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
	
#ifdef debug_uart
uint8_t sample_text[] = "fcn_mode_uart_Nothing with osPriorityNormal\r\n";
	HAL_UART_Transmit(&debug_uart, sample_text, sizeof(sample_text), 10);
	ram_data.mode_uart++;
#endif

	//add code below
}

__weak void fcn_mode_uart_verbose(){
	
#ifdef debug_uart
uint8_t sample_text[] = "fcn_mode_uart_verbose with osPriorityNormal\r\n";
	HAL_UART_Transmit(&debug_uart, sample_text, sizeof(sample_text), 10);
	ram_data.mode_uart++;
#endif

	//add code below
}

__weak void fcn_mode_uart_debug(){
	
#ifdef debug_uart
uint8_t sample_text[] = "fcn_mode_uart_debug with osPriorityNormal\r\n";
	HAL_UART_Transmit(&debug_uart, sample_text, sizeof(sample_text), 10);
	ram_data.mode_uart++;
#endif

	//add code below
}
#endif