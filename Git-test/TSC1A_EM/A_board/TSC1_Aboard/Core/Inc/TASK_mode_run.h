#ifndef INC_TASK_mode_run_H_
#define INC_TASK_mode_run_H_
#include "main.h"
#include "TASK_ABOARD.h"
#include "REG_ABOARD.h"
void fcn_mode_run_Nothing();
void fcn_mode_run_initialize();
void fcn_mode_run_standby();
void fcn_mode_run_safemode();
void fcn_mode_run_simulation();
void fcn_mode_run_execute();
void fcn_mode_run_loop();
void fcn_mode_run_restart();
enum class run_enum_t: uint8_t
{
	Nothing = 0,
	initialize = 1,
	standby = 2,
	safemode = 3,
	simulation = 4,
	execute = 5,
	loop = 6,
	restart = 7,
};

__weak void TASK_mode_run(void* argument){
	for(;;){

		osDelay(ram_data.mode_run_delay_ms);
		switch(ram_data.mode_run){
			case (uint8_t)run_enum_t::Nothing :
				fcn_mode_run_Nothing();
				ram_data.mode_run_delay_ms = 50;
				break;
			case (uint8_t)run_enum_t::initialize :
				fcn_mode_run_initialize();
				ram_data.mode_run_delay_ms = 50;
				break;
			case (uint8_t)run_enum_t::standby :
				fcn_mode_run_standby();
				ram_data.mode_run_delay_ms = 50;
				break;
			case (uint8_t)run_enum_t::safemode :
				fcn_mode_run_safemode();
				ram_data.mode_run_delay_ms = 50;
				break;
			case (uint8_t)run_enum_t::simulation :
				fcn_mode_run_simulation();
				ram_data.mode_run_delay_ms = 50;
				break;
			case (uint8_t)run_enum_t::execute :
				fcn_mode_run_execute();
				ram_data.mode_run_delay_ms = 50;
				break;
			case (uint8_t)run_enum_t::loop :
				fcn_mode_run_loop();
				ram_data.mode_run_delay_ms = 50;
				break;
			case (uint8_t)run_enum_t::restart :
				fcn_mode_run_restart();
				ram_data.mode_run_delay_ms = 50;
				break;
			default:
#ifdef debug_uart
				ram_data.mode_run = 0;
#endif
				break;
		} //switch end
	}//loop end
 }//function end
__weak void fcn_mode_run_Nothing(){
	
#ifdef debug_uart
uint8_t sample_text[] = "fcn_mode_run_Nothing with osPriorityRealtime\r\n";
	HAL_UART_Transmit(&debug_uart, sample_text, sizeof(sample_text), 10);
	ram_data.mode_run++;
#endif

	//add code below
}

__weak void fcn_mode_run_initialize(){
	
#ifdef debug_uart
uint8_t sample_text[] = "fcn_mode_run_initialize with osPriorityRealtime\r\n";
	HAL_UART_Transmit(&debug_uart, sample_text, sizeof(sample_text), 10);
	ram_data.mode_run++;
#endif

	//add code below
}

__weak void fcn_mode_run_standby(){
	
#ifdef debug_uart
uint8_t sample_text[] = "fcn_mode_run_standby with osPriorityRealtime\r\n";
	HAL_UART_Transmit(&debug_uart, sample_text, sizeof(sample_text), 10);
	ram_data.mode_run++;
#endif

	//add code below
}

__weak void fcn_mode_run_safemode(){
	
#ifdef debug_uart
uint8_t sample_text[] = "fcn_mode_run_safemode with osPriorityRealtime\r\n";
	HAL_UART_Transmit(&debug_uart, sample_text, sizeof(sample_text), 10);
	ram_data.mode_run++;
#endif

	//add code below
}

__weak void fcn_mode_run_simulation(){
	
#ifdef debug_uart
uint8_t sample_text[] = "fcn_mode_run_simulation with osPriorityRealtime\r\n";
	HAL_UART_Transmit(&debug_uart, sample_text, sizeof(sample_text), 10);
	ram_data.mode_run++;
#endif

	//add code below
}

__weak void fcn_mode_run_execute(){
	
#ifdef debug_uart
uint8_t sample_text[] = "fcn_mode_run_execute with osPriorityRealtime\r\n";
	HAL_UART_Transmit(&debug_uart, sample_text, sizeof(sample_text), 10);
	ram_data.mode_run++;
#endif

	//add code below
}

__weak void fcn_mode_run_loop(){
	
#ifdef debug_uart
uint8_t sample_text[] = "fcn_mode_run_loop with osPriorityRealtime\r\n";
	HAL_UART_Transmit(&debug_uart, sample_text, sizeof(sample_text), 10);
	ram_data.mode_run++;
#endif

	//add code below
}

__weak void fcn_mode_run_restart(){
	
#ifdef debug_uart
uint8_t sample_text[] = "fcn_mode_run_restart with osPriorityRealtime\r\n";
	HAL_UART_Transmit(&debug_uart, sample_text, sizeof(sample_text), 10);
	ram_data.mode_run++;
#endif

	//add code below
}
#endif