#ifndef INC_TASK_mode_fss_H_
#define INC_TASK_mode_fss_H_
#include "main.h"
#include "TASK_TSC1.h"
#include "REG_TSC1.h"
#include "LIB/FSS_REG.h"
#include "LIB/fss.h"

void fcn_mode_fss_on();
void fcn_mode_fss_off();
void fcn_mode_fss_init();
void fcn_mode_fss_function_test();
void fcn_mode_fss_direction_checking();
void fcn_mode_fss_log();
void fcn_mode_fss_calibrate();

uint8_t sun_ping[] = {0xC0,0x39,0x11,0xA0,0xDB,0xDC,0x0A,0xC0};
uint8_t init[]     = {0xC0,0x39,0x11,0xA1,0x00,0x10,0x00,0x00,0x72,0xC6,0xC0};
uint8_t telemetry[] = {0xC0,0x39,0x11,0xA7,0x09,0xCF,0x16,0xC0};
extern uint8_t fssRxBuffer[64];

enum class fss_enum_t: uint8_t
{
	on = 0,
	off = 1,
	init = 2,
	function_test = 3,
	direction_checking = 4,
	log = 5,
	calibrate = 6,
};

__weak void TASK_mode_fss(void* argument){
	for(;;){

		osDelay(ram_data.mode_fss_delay_ms);
		switch(ram_data.mode_fss){
			case (uint8_t)fss_enum_t::on:
				fcn_mode_fss_on();
				ram_data.mode_fss_delay_ms = 1;
				break;
			case (uint8_t)fss_enum_t::off :
				fcn_mode_fss_off();
				ram_data.mode_fss_delay_ms = 50;
				break;
			case (uint8_t)fss_enum_t::init :
				fcn_mode_fss_init();
				ram_data.mode_fss_delay_ms = 50;
				break;
			case (uint8_t)fss_enum_t::function_test :
				fcn_mode_fss_function_test();
				ram_data.mode_fss_delay_ms = 50;
				break;
			case (uint8_t)fss_enum_t::direction_checking :
				fcn_mode_fss_direction_checking();
				ram_data.mode_fss_delay_ms = 50;
				break;
			case (uint8_t)fss_enum_t::log :
				fcn_mode_fss_log();
				ram_data.mode_fss_delay_ms = 50;
				break;
			case (uint8_t)fss_enum_t::calibrate :
				fcn_mode_fss_calibrate();
				ram_data.mode_fss_delay_ms = 50;
				break;
			default:
#ifdef debug_uart
				ram_data.mode_fss = 0;
#endif
				break;
		} //switch end
	}//loop end
 }//function end
__weak void fcn_mode_fss_on(){
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_10, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_11, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_SET);
	
	ram_data.mode_fss = (uint8_t)fss_enum_t::init;


	//add code below
}

__weak void fcn_mode_fss_off(){
	
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_10, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_11, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_RESET);


	//add code below
}

__weak void fcn_mode_fss_init(){
	
	HAL_UART_Transmit(&huart6, init, sizeof(init), 100);
	ram_data.mode_fss = (uint8_t)fss_enum_t::log;


	//add code below
}

__weak void fcn_mode_fss_function_test(){
	
	HAL_UART_Transmit(&huart6, sun_ping, sizeof(sun_ping), 100);



	//add code below
}

__weak void fcn_mode_fss_direction_checking(){
	


	//add code below
}

__weak void fcn_mode_fss_log(){

//	HAL_UART_Transmit(&huart6, sun_ping, sizeof(sun_ping), 100);
	 HAL_UART_Receive_DMA(&huart6, fssRxBuffer, sizeof(fssRxBuffer));

	HAL_UART_Transmit(&huart6, telemetry, sizeof(telemetry), 100);
//	HAL_UART_Transmit(&huart6, init, sizeof(init), 100);


}

__weak void fcn_mode_fss_calibrate(){
	


	//add code below
}
#endif
