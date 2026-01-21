#ifndef INC_TASK_mode_gps_H_
#define INC_TASK_mode_gps_H_
#include "main.h"
#include "TASK_TSC1.h"
#include "REG_TSC1.h"
#include "ClassInit/GNSSInit.h"


void fcn_mode_gps_Nothing();
void fcn_mode_gps_on();
void fcn_mode_gps_normal();
void fcn_mode_gps_off();
void fcn_mode_gps_recovery();

enum class gps_enum_t: uint8_t
{
	Nothing = 0,
	on = 1,
	normal = 2,
	off = 3,
	recovery = 4,
};

__weak void TASK_mode_gps(void* argument){
	for(;;){

		osDelay(ram_data.mode_gps_delay_ms);
		switch(ram_data.mode_gps){
			case (uint8_t)gps_enum_t::Nothing :
				fcn_mode_gps_Nothing();
				ram_data.mode_gps_delay_ms = 1;
				break;
			case (uint8_t)gps_enum_t::on :
				fcn_mode_gps_on();
				ram_data.mode_gps_delay_ms = 10;
				break;
			case (uint8_t)gps_enum_t::normal :
				fcn_mode_gps_normal();
				ram_data.mode_gps_delay_ms = 50;
				break;
			case (uint8_t)gps_enum_t::off :
				fcn_mode_gps_off();
				ram_data.mode_gps_delay_ms = 10;
				break;
			case (uint8_t)gps_enum_t::recovery :
				fcn_mode_gps_recovery();
				ram_data.mode_gps_delay_ms = 50;
				break;
			default:
#ifdef debug_uart
				ram_data.mode_gps = 0;
#endif
				break;
		} //switch end
	}//loop end
 }//function end
__weak void fcn_mode_gps_Nothing(){
	


	//add code below
	ram_data.mode_gps = (uint8_t)gps_enum_t::on;
}

__weak void fcn_mode_gps_on(){
	

	//add code below
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_3, GPIO_PIN_SET);
	ram_data.mode_gps = (uint8_t)gps_enum_t::normal;
}

__weak void fcn_mode_gps_normal(){
	

	//GNSS.binary_log(CC0M2, bestXYZ, ONTIME, 0);
	//HAL_CAN_ActivateNotification(&hcan1,CAN_IT_RX_FIFO1_MSG_PENDING);
	//add code below
	__SEV();
}

__weak void fcn_mode_gps_off(){
	


	//add code below
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_3, GPIO_PIN_RESET);

}

__weak void fcn_mode_gps_recovery(){
	


	//add code below
}
#endif
