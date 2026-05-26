#ifndef INC_TASK_mode_mtq_H_
#define INC_TASK_mode_mtq_H_
#include "main.h"
#include "TASK_TSC1.h"
#include "REG_TSC1.h"
#include "ClassInit/MTQInit.h"
void fcn_mode_mtq_Nothing();
void fcn_mode_mtq_on();
void fcn_mode_mtq_off();
enum class mtq_enum_t: uint8_t
{
	on = 0,
	Nothing = 1,
	off = 2,
};

__weak void TASK_mode_mtq(void* argument){
	for(;;){

		osDelay(ram_data.mode_mtq_delay_ms);
		switch(ram_data.mode_mtq){
			case (uint8_t)mtq_enum_t::on :
				fcn_mode_mtq_on();
				ram_data.mode_mtq_delay_ms = 10;
				break;
			case (uint8_t)mtq_enum_t::Nothing :
				fcn_mode_mtq_Nothing();
				ram_data.mode_mtq_delay_ms = 10;
				break;
			case (uint8_t)mtq_enum_t::off :
				fcn_mode_mtq_off();
				ram_data.mode_mtq_delay_ms = 10;
				break;
			default:
#ifdef debug_uart
				ram_data.mode_mtq = 0;
#endif
				break;
		} //switch end
	}//loop end
 }//function end
__weak void fcn_mode_mtq_Nothing(){
	
#ifdef debug_uart
uint8_t sample_text[] = "fcn_mode_mtq_Nothing with osPriorityNormal\r\n";
	HAL_UART_Transmit(&debug_uart, sample_text, sizeof(sample_text), 10);
#endif



	//add code below
}

__weak void fcn_mode_mtq_on(){
	
#ifdef debug_uart
uint8_t sample_text[] = "fcn_mode_mtq_on with osPriorityNormal\r\n";
	HAL_UART_Transmit(&debug_uart, sample_text, sizeof(sample_text), 10);
	//ram_data.mode_mtq++;
#endif
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_SET);
	ram_data.mode_mtq = (uint8_t)mtq_enum_t::Nothing;
	//add code below
}


__weak void fcn_mode_mtq_off(){

#ifdef debug_uart
uint8_t sample_text[] = "fcn_mode_mtq_on with osPriorityNormal\r\n";
	HAL_UART_Transmit(&debug_uart, sample_text, sizeof(sample_text), 10);
	//ram_data.mode_mtq++;
#endif
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET);
	//add code below
}
#endif
