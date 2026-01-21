#ifndef INC_TASK_mode_rwa_H_
#define INC_TASK_mode_rwa_H_
#include "main.h"
#include "TASK_TSC1.h"
#include "REG_TSC1.h"
#include "ClassInit/RWInit.h"
void fcn_mode_rwa_Nothing();
void fcn_mode_rwa_on();
void fcn_mode_rwa_off();
void fcn_mode_rwa_init();
void fcn_mode_rwa_direction_checking();
void fcn_mode_rwa_log();
void fcn_mode_rwa_torque();
void fcn_mode_rwa_speed();
enum class rwa_enum_t: uint8_t
{
	Nothing = 0,
	on = 1,
	off = 2,
	init = 3,
	direction_checking = 4,
	log = 5,
	torque = 6,
	speed = 7,
};

__weak void TASK_mode_rwa(void* argument){
	for(;;){

		osDelay(ram_data.mode_rwa_delay_ms);
		switch(ram_data.mode_rwa){
			case (uint8_t)rwa_enum_t::Nothing :
				fcn_mode_rwa_Nothing();
				ram_data.mode_rwa_delay_ms = 10;
				break;
			case (uint8_t)rwa_enum_t::on :
				fcn_mode_rwa_on();
				ram_data.mode_rwa_delay_ms = 10;
				break;
			case (uint8_t)rwa_enum_t::off :
				fcn_mode_rwa_off();
				ram_data.mode_rwa_delay_ms = 10;
				break;
			case (uint8_t)rwa_enum_t::init :
				fcn_mode_rwa_init();
				ram_data.mode_rwa_delay_ms = 10;
				break;
			case (uint8_t)rwa_enum_t::direction_checking :
				fcn_mode_rwa_direction_checking();
				ram_data.mode_rwa_delay_ms = 10;
				break;
			case (uint8_t)rwa_enum_t::log :
				fcn_mode_rwa_log();
				ram_data.mode_rwa_delay_ms = 10;
				break;
			case (uint8_t)rwa_enum_t::torque :
				fcn_mode_rwa_torque();
				ram_data.mode_rwa_delay_ms = 10;
				break;
			case (uint8_t)rwa_enum_t::speed :
				fcn_mode_rwa_speed();
				ram_data.mode_rwa_delay_ms = 10;
				break;
			default:
#ifdef debug_uart
				ram_data.mode_rwa = 0;
#endif
				break;
		} //switch end
	}//loop end
 }//function end
__weak void fcn_mode_rwa_Nothing(){
	
#ifdef debug_uart
uint8_t sample_text[] = "fcn_mode_rwa_Nothing with osPriorityNormal\r\n";
	HAL_UART_Transmit(&debug_uart, sample_text, sizeof(sample_text), 10);
	ram_data.mode_rwa++;
#endif

	//add code below
}

__weak void fcn_mode_rwa_on(){
	
#ifdef debug_uart
uint8_t sample_text[] = "fcn_mode_rwa_on with osPriorityNormal\r\n";
	HAL_UART_Transmit(&debug_uart, sample_text, sizeof(sample_text), 10);
	ram_data.mode_rwa++;
#endif

	//add code below
}

__weak void fcn_mode_rwa_off(){
	
#ifdef debug_uart
uint8_t sample_text[] = "fcn_mode_rwa_off with osPriorityNormal\r\n";
	HAL_UART_Transmit(&debug_uart, sample_text, sizeof(sample_text), 10);
	ram_data.mode_rwa++;
#endif

	//add code below
}

__weak void fcn_mode_rwa_init(){
	
#ifdef debug_uart
uint8_t sample_text[] = "fcn_mode_rwa_init with osPriorityNormal\r\n";
	HAL_UART_Transmit(&debug_uart, sample_text, sizeof(sample_text), 10);
	ram_data.mode_rwa++;
#endif

	//add code below
}

__weak void fcn_mode_rwa_direction_checking(){
	
#ifdef debug_uart
uint8_t sample_text[] = "fcn_mode_rwa_direction_checking with osPriorityNormal\r\n";
	HAL_UART_Transmit(&debug_uart, sample_text, sizeof(sample_text), 10);
	ram_data.mode_rwa++;
#endif

	//add code below
}

__weak void fcn_mode_rwa_log(){
	
#ifdef debug_uart
uint8_t sample_text[] = "fcn_mode_rwa_log with osPriorityNormal\r\n";
	HAL_UART_Transmit(&debug_uart, sample_text, sizeof(sample_text), 10);
	ram_data.mode_rwa++;
#endif

	//add code below
}

__weak void fcn_mode_rwa_torque(){
	
#ifdef debug_uart
uint8_t sample_text[] = "fcn_mode_rwa_torque with osPriorityNormal\r\n";
	HAL_UART_Transmit(&debug_uart, sample_text, sizeof(sample_text), 10);
	ram_data.mode_rwa++;
#endif

	//add code below
}

__weak void fcn_mode_rwa_speed(){
	
#ifdef debug_uart
uint8_t sample_text[] = "fcn_mode_rwa_speed with osPriorityNormal\r\n";
	HAL_UART_Transmit(&debug_uart, sample_text, sizeof(sample_text), 10);
	ram_data.mode_rwa++;
#endif

	//add code below
}
#endif
