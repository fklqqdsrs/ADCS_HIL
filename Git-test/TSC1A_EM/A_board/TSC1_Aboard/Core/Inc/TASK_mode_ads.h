#ifndef INC_TASK_mode_ads_H_
#define INC_TASK_mode_ads_H_
#include "main.h"
#include "TASK_ABOARD.h"
#include "REG_ABOARD.h"
void fcn_mode_ads_Nothing();
void fcn_mode_ads_Calculate();
void fcn_mode_ads_Kalman();
void fcn_mode_ads_Precise();
enum class ads_enum_t: uint8_t
{
	Nothing = 0,
	Calculate = 1,
	Kalman = 2,
	Precise = 3,
};

__weak void TASK_mode_ads(void* argument){
	for(;;){

		osEventFlagsWait(TASK_mode_ads_evt, TASK_mode_ads_signal_id,osFlagsWaitAny ,osWaitForever);

		//osDelay(ram_data.mode_ads_delay_ms);
		switch(ram_data.mode_ads){
			case (uint8_t)ads_enum_t::Nothing :
				fcn_mode_ads_Nothing();
				ram_data.mode_ads_delay_ms = 1;
				break;
			case (uint8_t)ads_enum_t::Calculate :
				fcn_mode_ads_Calculate();
				ram_data.mode_ads_delay_ms = 1;
				break;
			case (uint8_t)ads_enum_t::Kalman :
				fcn_mode_ads_Kalman();
				ram_data.mode_ads_delay_ms = 1;
				break;
			case (uint8_t)ads_enum_t::Precise :
				fcn_mode_ads_Precise();
				ram_data.mode_ads_delay_ms = 1;
				break;
			default:
#ifdef debug_uart
				ram_data.mode_ads = 0;
#endif
				break;
		} //switch end
	}//loop end
 }//function end
__weak void fcn_mode_ads_Nothing(){
	
#ifdef debug_uart
uint8_t sample_text[] = "fcn_mode_ads_Nothing with osPriorityHigh\r\n";
	HAL_UART_Transmit(&debug_uart, sample_text, sizeof(sample_text), 10);
	ram_data.mode_ads++;
#endif

	//add code below
}

__weak void fcn_mode_ads_Calculate(){
	
#ifdef debug_uart
uint8_t sample_text[] = "fcn_mode_ads_Calculate with osPriorityHigh\r\n";
	HAL_UART_Transmit(&debug_uart, sample_text, sizeof(sample_text), 10);
	ram_data.mode_ads++;
#endif

	//add code below
}

__weak void fcn_mode_ads_Kalman(){
	
#ifdef debug_uart
uint8_t sample_text[] = "fcn_mode_ads_Kalman with osPriorityHigh\r\n";
	HAL_UART_Transmit(&debug_uart, sample_text, sizeof(sample_text), 10);
	ram_data.mode_ads++;
#endif

	//add code below
}

__weak void fcn_mode_ads_Precise(){
	
#ifdef debug_uart
uint8_t sample_text[] = "fcn_mode_ads_Precise with osPriorityHigh\r\n";
	HAL_UART_Transmit(&debug_uart, sample_text, sizeof(sample_text), 10);
	ram_data.mode_ads++;
#endif

	//add code below
}
#endif