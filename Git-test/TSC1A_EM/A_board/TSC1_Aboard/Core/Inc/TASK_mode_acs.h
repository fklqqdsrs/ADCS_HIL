#ifndef INC_TASK_mode_acs_H_
#define INC_TASK_mode_acs_H_
#include "main.h"
#include "TASK_ABOARD.h"
#include "REG_ABOARD.h"
void fcn_mode_acs_Nothing();
void fcn_mode_acs_Detumble();
void fcn_mode_acs_NadirPoint();
void fcn_mode_acs_Ground();
void fcn_mode_acs_Sun();
enum class acs_enum_t: uint8_t
{
	Nothing = 0,
	Detumble = 1,
	NadirPoint = 2,
	Ground = 3,
	Sun = 4,
};

__weak void TASK_mode_acs(void* argument){
	for(;;){

		osEventFlagsWait(TASK_mode_acs_evt, TASK_mode_acs_signal_id,osFlagsWaitAny ,osWaitForever);

		//osDelay(ram_data.mode_acs_delay_ms);
		switch(ram_data.mode_acs){
			case (uint8_t)acs_enum_t::Nothing :
				fcn_mode_acs_Nothing();
				ram_data.mode_acs_delay_ms = 1;
				break;
			case (uint8_t)acs_enum_t::Detumble :
				fcn_mode_acs_Detumble();
				ram_data.mode_acs_delay_ms = 1;
				break;
			case (uint8_t)acs_enum_t::NadirPoint :
				fcn_mode_acs_NadirPoint();
				ram_data.mode_acs_delay_ms = 1;
				break;
			case (uint8_t)acs_enum_t::Ground :
				fcn_mode_acs_Ground();
				ram_data.mode_acs_delay_ms = 1;
				break;
			case (uint8_t)acs_enum_t::Sun :
				fcn_mode_acs_Sun();
				ram_data.mode_acs_delay_ms = 1;
				break;
			default:
#ifdef debug_uart
				ram_data.mode_acs = 0;
#endif
				break;
		} //switch end
	}//loop end
 }//function end
__weak void fcn_mode_acs_Nothing(){
	
#ifdef debug_uart
uint8_t sample_text[] = "fcn_mode_acs_Nothing with osPriorityAboveNormal\r\n";
	HAL_UART_Transmit(&debug_uart, sample_text, sizeof(sample_text), 10);
	ram_data.mode_acs++;
#endif

	//add code below
}

__weak void fcn_mode_acs_Detumble(){
	
#ifdef debug_uart
uint8_t sample_text[] = "fcn_mode_acs_Detumble with osPriorityAboveNormal\r\n";
	HAL_UART_Transmit(&debug_uart, sample_text, sizeof(sample_text), 10);
	ram_data.mode_acs++;
#endif

	//add code below
}

__weak void fcn_mode_acs_NadirPoint(){
	
#ifdef debug_uart
uint8_t sample_text[] = "fcn_mode_acs_NadirPoint with osPriorityAboveNormal\r\n";
	HAL_UART_Transmit(&debug_uart, sample_text, sizeof(sample_text), 10);
	ram_data.mode_acs++;
#endif

	//add code below
}

__weak void fcn_mode_acs_Ground(){
	
#ifdef debug_uart
uint8_t sample_text[] = "fcn_mode_acs_Ground with osPriorityAboveNormal\r\n";
	HAL_UART_Transmit(&debug_uart, sample_text, sizeof(sample_text), 10);
	ram_data.mode_acs++;
#endif

	//add code below
}

__weak void fcn_mode_acs_Sun(){
	
#ifdef debug_uart
uint8_t sample_text[] = "fcn_mode_acs_Sun with osPriorityAboveNormal\r\n";
	HAL_UART_Transmit(&debug_uart, sample_text, sizeof(sample_text), 10);
	ram_data.mode_acs++;
#endif

	//add code below
}
#endif