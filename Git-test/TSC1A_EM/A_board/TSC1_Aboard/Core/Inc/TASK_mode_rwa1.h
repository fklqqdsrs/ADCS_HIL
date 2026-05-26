#ifndef INC_TASK_mode_rwa1_H_
#define INC_TASK_mode_rwa1_H_
#include "main.h"
#include "TASK_ABOARD.h"
#include "REG_ABOARD.h"
void fcn_mode_rwa1_Nothing();
enum class rwa1_enum_t: uint8_t
{
	Nothing = 0,
};

__weak void TASK_mode_rwa1(void* argument){
	for(;;){

		osDelay(ram_data.mode_rwa1_delay_ms);
		switch(ram_data.mode_rwa1){
			case (uint8_t)rwa1_enum_t::Nothing :
				fcn_mode_rwa1_Nothing();
				ram_data.mode_rwa1_delay_ms = 1000;
				break;
			default:
#ifdef debug_uart
				ram_data.mode_rwa1 = 0;
#endif
				break;
		} //switch end
	}//loop end
 }//function end
__weak void fcn_mode_rwa1_Nothing(){
	
#ifdef debug_uart
uint8_t sample_text[] = "fcn_mode_rwa1_Nothing with osPriorityNormal\r\n";
	HAL_UART_Transmit(&debug_uart, sample_text, sizeof(sample_text), 10);
	ram_data.mode_rwa1++;
#endif

	//add code below
}
#endif