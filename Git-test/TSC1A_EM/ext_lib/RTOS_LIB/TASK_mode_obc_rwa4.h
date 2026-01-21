#ifndef INC_TASK_mode_obc_rwa4_H_
#define INC_TASK_mode_obc_rwa4_H_
#include "main.h"
#include "TASK_TSC1.h"
#include "REG_TSC1.h"
#include "ClassInit/RWInit.h"
void fcn_mode_obc_rwa4_Nothing();
void fcn_mode_obc_rwa4_on();
void fcn_mode_obc_rwa4_off();
void fcn_mode_obc_rwa4_torque();
void fcn_mode_obc_rwa4_speed();
void fcn_mode_obc_rwa4_direction_checking();
enum class obc_rwa4_enum_t: uint8_t
{
	Nothing = 0,
	on = 1,
	off = 2,
	torque = 3,
	speed = 4,
	direction_checking = 5,
};

static int32_t last_wheel4_mrpm;
__weak void TASK_mode_obc_rwa4(void* argument){
	for(;;){

		osDelay(ram_data.mode_obc_rwa4_delay_ms);
		switch(ram_data.mode_obc_rwa4){
			case (uint8_t)obc_rwa4_enum_t::Nothing :
				fcn_mode_obc_rwa4_Nothing();
				ram_data.mode_obc_rwa4_delay_ms = 1;
				break;
			case (uint8_t)obc_rwa4_enum_t::on :
				fcn_mode_obc_rwa4_on();
				ram_data.mode_obc_rwa4_delay_ms = 50;
				break;
			case (uint8_t)obc_rwa4_enum_t::off :
				fcn_mode_obc_rwa4_off();
				ram_data.mode_obc_rwa4_delay_ms = 50;
				break;
			case (uint8_t)obc_rwa4_enum_t::torque :
				fcn_mode_obc_rwa4_torque();
				ram_data.mode_obc_rwa4_delay_ms = 50;
				break;
			case (uint8_t)obc_rwa4_enum_t::speed :
				fcn_mode_obc_rwa4_speed();
				ram_data.mode_obc_rwa4_delay_ms = 50;
				break;
			case (uint8_t)obc_rwa4_enum_t::direction_checking :
				fcn_mode_obc_rwa4_direction_checking();
				ram_data.mode_obc_rwa4_delay_ms = 50;
				break;
			default:
#ifdef debug_uart
				ram_data.mode_obc_rwa4 = 0;
#endif
				break;
		} //switch end
	}//loop end
 }//function end
__weak void fcn_mode_obc_rwa4_Nothing(){
	
#ifdef debug_uart
uint8_t sample_text[] = "fcn_mode_obc_rwa4_Nothing with osPriorityNormal\r\n";
	HAL_UART_Transmit(&debug_uart, sample_text, sizeof(sample_text), 10);
	ram_data.mode_obc_rwa4++;
#endif

	//add code below
	ram_data.mode_obc_rwa4= (uint8_t)obc_rwa4_enum_t::on ;
}

__weak void fcn_mode_obc_rwa4_on(){
	
#ifdef debug_uart
uint8_t sample_text[] = "fcn_mode_obc_rwa4_on with osPriorityNormal\r\n";
	HAL_UART_Transmit(&debug_uart, sample_text, sizeof(sample_text), 10);
	ram_data.mode_obc_rwa4++;
#endif

	//add code below
	whl4.SendCommand(SET, pid_en,      PARAM_TYPE_UINT8,  1);
	whl4.SendCommand(SET, drv_enable,  PARAM_TYPE_UINT8,  1);
	ram_data.mode_obc_rwa4 = (uint8_t)obc_rwa4_enum_t::speed;
}

__weak void fcn_mode_obc_rwa4_off(){
	
#ifdef debug_uart
uint8_t sample_text[] = "fcn_mode_obc_rwa4_off with osPriorityNormal\r\n";
	HAL_UART_Transmit(&debug_uart, sample_text, sizeof(sample_text), 10);
	ram_data.mode_obc_rwa4++;
#endif

	//add code below
}

__weak void fcn_mode_obc_rwa4_torque(){
	
#ifdef debug_uart
uint8_t sample_text[] = "fcn_mode_obc_rwa4_torque with osPriorityNormal\r\n";
	HAL_UART_Transmit(&debug_uart, sample_text, sizeof(sample_text), 10);
	ram_data.mode_obc_rwa4++;
#endif

	//add code below
}

__weak void fcn_mode_obc_rwa4_speed(){
	
#ifdef debug_uart
uint8_t sample_text[] = "fcn_mode_obc_rwa4_speed with osPriorityNormal\r\n";
	HAL_UART_Transmit(&debug_uart, sample_text, sizeof(sample_text), 10);
	ram_data.mode_obc_rwa4++;
#endif

	//add code below
	//whlreg[WHEEL4].WHL_MRPM_SET_D = 000000;
	if(whlreg[WHEEL4].WHL_MRPM_TARGET_D != last_wheel4_mrpm)
	{
		whl4.SendCommand(SET, mrpm_target, PARAM_TYPE_INT32,whlreg[WHEEL4].WHL_MRPM_TARGET_D );
	}
	whl4.SendCommand(GET,mrpm );
	whl4.SendCommand(GET,current );
	last_wheel4_mrpm = whlreg[WHEEL4].WHL_MRPM_TARGET_D;
}

__weak void fcn_mode_obc_rwa4_direction_checking(){
	
#ifdef debug_uart
uint8_t sample_text[] = "fcn_mode_obc_rwa4_direction_checking with osPriorityNormal\r\n";
	HAL_UART_Transmit(&debug_uart, sample_text, sizeof(sample_text), 10);
	ram_data.mode_obc_rwa4++;
#endif

	//add code below
}
#endif
