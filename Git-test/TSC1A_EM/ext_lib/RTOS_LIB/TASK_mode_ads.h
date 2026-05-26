#ifndef INC_TASK_mode_ads_H_
#define INC_TASK_mode_ads_H_
#include "main.h"
#include "TASK_TSC1.h"
#include "REG_TSC1.h"
void fcn_mode_ads_Nothing();
enum class ads_enum_t: uint8_t
{
	Nothing = 0,
};

__weak void TASK_mode_ads(void* argument){
	for(;;){

		osDelay(ram_data.mode_ads_delay_ms);
		switch(ram_data.mode_ads){
			case (uint8_t)ads_enum_t::Nothing :
				fcn_mode_ads_Nothing();
				ram_data.mode_ads_delay_ms = 1000;
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
uint8_t sample_text[] = "fcn_mode_ads_Nothing with osPriorityNormal\r\n";
	HAL_UART_Transmit(&debug_uart, sample_text, sizeof(sample_text), 10);
	ram_data.mode_ads++;
#endif

	//add code below
}
#endif