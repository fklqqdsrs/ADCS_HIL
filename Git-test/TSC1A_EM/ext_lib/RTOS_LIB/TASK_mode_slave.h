#ifndef INC_TASK_mode_slave_H_
#define INC_TASK_mode_slave_H_
#include "main.h"
#include "TASK_TSC1.h"
#include "REG_TSC1.h"
void fcn_mode_slave_Nothing();
enum class slave_enum_t: uint8_t
{
	Nothing = 0,
};

__weak void TASK_mode_slave(void* argument){
	for(;;){

		osDelay(ram_data.mode_slave_delay_ms);
		switch(ram_data.mode_slave){
			case (uint8_t)slave_enum_t::Nothing :
				fcn_mode_slave_Nothing();
				ram_data.mode_slave_delay_ms = 1000;
				break;
			default:
#ifdef debug_uart
				ram_data.mode_slave = 0;
#endif
				break;
		} //switch end
	}//loop end
 }//function end
__weak void fcn_mode_slave_Nothing(){
	
#ifdef debug_uart
uint8_t sample_text[] = "fcn_mode_slave_Nothing with osPriorityNormal\r\n";
	HAL_UART_Transmit(&debug_uart, sample_text, sizeof(sample_text), 10);
	ram_data.mode_slave++;
#endif

	//add code below
}
#endif