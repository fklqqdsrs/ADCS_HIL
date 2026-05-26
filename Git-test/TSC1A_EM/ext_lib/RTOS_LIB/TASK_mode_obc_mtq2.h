#ifndef INC_TASK_mode_obc_mtq2_H_
#define INC_TASK_mode_obc_mtq2_H_
#include "main.h"
#include "TASK_TSC1.h"
#include "REG_TSC1.h"
#include "TASK_mode_mtq.h"

void fcn_mode_obc_mtq2_Nothing();
void fcn_mode_obc_mtq2_on();
void fcn_mode_obc_mtq2_off();
void fcn_mode_obc_mtq2_run();
enum class obc_mtq2_enum_t: uint8_t
{
	Nothing = 0,
	on = 1,
	off = 2,
	run = 3,
};

__weak void TASK_mode_obc_mtq2(void* argument){
	for(;;){

		osDelay(ram_data.mode_obc_mtq2_delay_ms);
		switch(ram_data.mode_obc_mtq2){
			case (uint8_t)obc_mtq2_enum_t::Nothing :
				fcn_mode_obc_mtq2_Nothing();
				ram_data.mode_obc_mtq2_delay_ms = 1;
				break;
			case (uint8_t)obc_mtq2_enum_t::on :
				fcn_mode_obc_mtq2_on();
				ram_data.mode_obc_mtq2_delay_ms = 10;
				break;
			case (uint8_t)obc_mtq2_enum_t::off :
				fcn_mode_obc_mtq2_off();
				ram_data.mode_obc_mtq2_delay_ms = 10;
				break;
			case (uint8_t)obc_mtq2_enum_t::run :
				fcn_mode_obc_mtq2_run();
				ram_data.mode_obc_mtq2_delay_ms = 50;
				break;
			default:
#ifdef debug_uart
				ram_data.mode_obc_mtq2 = 0;
#endif
				break;
		} //switch end
	}//loop end
 }//function end
__weak void fcn_mode_obc_mtq2_Nothing(){
	
	ram_data.mode_obc_mtq2 = (uint8_t)obc_mtq2_enum_t::run;

	//add code below
}

__weak void fcn_mode_obc_mtq2_on(){
	

}

__weak void fcn_mode_obc_mtq2_off(){
	


	//add code below
}

__weak void fcn_mode_obc_mtq2_run(){

	MTQy.run_raw_pwm(5000,65534);
	__SEV();
	//add code below
}
#endif
