#ifndef INC_TASK_mode_obc_rwa2_H_
#define INC_TASK_mode_obc_rwa2_H_
#include "main.h"
#include "TASK_TSC1.h"
#include "REG_TSC1.h"
#include "ClassInit/RWInit.h"
void fcn_mode_obc_rwa2_Nothing();
void fcn_mode_obc_rwa2_on();
void fcn_mode_obc_rwa2_off();
void fcn_mode_obc_rwa2_torque();
void fcn_mode_obc_rwa2_speed();
void fcn_mode_obc_rwa2_direction_checking();
enum class obc_rwa2_enum_t: uint8_t
{
	Nothing = 0,
	on = 1,
	off = 2,
	torque = 3,
	speed = 4,
	direction_checking = 5,
};

static int32_t last_wheel2_mrpm;
__weak void TASK_mode_obc_rwa2(void* argument){
	for(;;){

		osDelay(ram_data.mode_obc_rwa2_delay_ms);
		switch(ram_data.mode_obc_rwa2){
			case (uint8_t)obc_rwa2_enum_t::Nothing :
				fcn_mode_obc_rwa2_Nothing();
				ram_data.mode_obc_rwa2_delay_ms = 1;
				break;
			case (uint8_t)obc_rwa2_enum_t::on :
				fcn_mode_obc_rwa2_on();
				ram_data.mode_obc_rwa2_delay_ms = 50;
				break;
			case (uint8_t)obc_rwa2_enum_t::off :
				fcn_mode_obc_rwa2_off();
				ram_data.mode_obc_rwa2_delay_ms = 50;
				break;
			case (uint8_t)obc_rwa2_enum_t::torque :
				fcn_mode_obc_rwa2_torque();
				ram_data.mode_obc_rwa2_delay_ms = 50;
				break;
			case (uint8_t)obc_rwa2_enum_t::speed :
				fcn_mode_obc_rwa2_speed();
				ram_data.mode_obc_rwa2_delay_ms = 50;
				break;
			case (uint8_t)obc_rwa2_enum_t::direction_checking :
				fcn_mode_obc_rwa2_direction_checking();
				ram_data.mode_obc_rwa2_delay_ms = 50;
				break;
			default:
#ifdef debug_uart
				ram_data.mode_obc_rwa2 = 0;
#endif
				break;
		} //switch end
	}//loop end
 }//function end
__weak void fcn_mode_obc_rwa2_Nothing(){
	
#ifdef debug_uart
uint8_t sample_text[] = "fcn_mode_obc_rwa2_Nothing with osPriorityNormal\r\n";
	HAL_UART_Transmit(&debug_uart, sample_text, sizeof(sample_text), 10);
	ram_data.mode_obc_rwa2++;
#endif

	//add code below
	ram_data.mode_obc_rwa2 = (uint8_t)obc_rwa2_enum_t::on ;

}

__weak void fcn_mode_obc_rwa2_on(){
	
#ifdef debug_uart
uint8_t sample_text[] = "fcn_mode_obc_rwa2_on with osPriorityNormal\r\n";
	HAL_UART_Transmit(&debug_uart, sample_text, sizeof(sample_text), 10);
	ram_data.mode_obc_rwa2++;
#endif

	//add code below
	  for(int i = 0; i<3; i++)
	  {
		whl2.SendCommand(SET, pid_en,      PARAM_TYPE_UINT8,  1);
		whl2.SendCommand(SET, drv_enable,  PARAM_TYPE_UINT8,  1);
	  }
	ram_data.mode_obc_rwa2 = (uint8_t)obc_rwa2_enum_t::speed;
}

__weak void fcn_mode_obc_rwa2_off(){
	
#ifdef debug_uart
uint8_t sample_text[] = "fcn_mode_obc_rwa2_off with osPriorityNormal\r\n";
	HAL_UART_Transmit(&debug_uart, sample_text, sizeof(sample_text), 10);
	ram_data.mode_obc_rwa2++;
#endif

	whl2.SendCommand(SET, mrpm_target, PARAM_TYPE_INT32,  0);
	LoopDelay(10000);
//	whl1.SendCommand(SET, pid_en,      PARAM_TYPE_UINT8,  0);
	whl2.SendCommand(SET, drv_enable,  PARAM_TYPE_UINT8,  0);
	whlreg[WHEEL2].WHL_MRPM_D = 0;
	whl2.Reboot(4);
	start_flag = 0;
}

__weak void fcn_mode_obc_rwa2_torque(){
	
#ifdef debug_uart
uint8_t sample_text[] = "fcn_mode_obc_rwa2_torque with osPriorityNormal\r\n";
	HAL_UART_Transmit(&debug_uart, sample_text, sizeof(sample_text), 10);
	ram_data.mode_obc_rwa2++;
#endif

	//add code below
}

__weak void fcn_mode_obc_rwa2_speed(){
	
#ifdef debug_uart
uint8_t sample_text[] = "fcn_mode_obc_rwa2_speed with osPriorityNormal\r\n";
	HAL_UART_Transmit(&debug_uart, sample_text, sizeof(sample_text), 10);
	ram_data.mode_obc_rwa2++;
#endif

	//add code below
	if (whlreg[WHEEL2].WHL_MRPM_TARGET_D != last_wheel2_mrpm)
	{
		whl2.SendCommand(SET, mrpm_target, PARAM_TYPE_INT32,whlreg[WHEEL2].WHL_MRPM_TARGET_D );
	}

	whl2.SendCommand(GET,mrpm );
	whl2.SendCommand(GET,current );
	last_wheel2_mrpm = whlreg[WHEEL2].WHL_MRPM_TARGET_D;
}

__weak void fcn_mode_obc_rwa2_direction_checking(){
	
#ifdef debug_uart
uint8_t sample_text[] = "fcn_mode_obc_rwa2_direction_checking with osPriorityNormal\r\n";
	HAL_UART_Transmit(&debug_uart, sample_text, sizeof(sample_text), 10);
	ram_data.mode_obc_rwa2++;
#endif

	//add code below

}
#endif
