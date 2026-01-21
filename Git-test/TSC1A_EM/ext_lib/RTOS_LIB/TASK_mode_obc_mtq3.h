#ifndef INC_TASK_mode_obc_mtq3_H_
#define INC_TASK_mode_obc_mtq3_H_
#include "main.h"
#include "TASK_TSC1.h"
#include "REG_TSC1.h"
#include "TASK_mode_mtq.h"

void fcn_mode_obc_mtq3_Nothing();
void fcn_mode_obc_mtq3_on();
void fcn_mode_obc_mtq3_off();
void fcn_mode_obc_mtq3_run();
enum class obc_mtq3_enum_t: uint8_t
{
	Nothing = 0,
	on = 1,
	off = 2,
	run = 3,
};

__weak void TASK_mode_obc_mtq3(void* argument){
	for(;;){

		osDelay(ram_data.mode_obc_mtq3_delay_ms);
		switch(ram_data.mode_obc_mtq3){
			case (uint8_t)obc_mtq3_enum_t::Nothing :
				fcn_mode_obc_mtq3_Nothing();
				ram_data.mode_obc_mtq3_delay_ms = 1;
				break;
			case (uint8_t)obc_mtq3_enum_t::on :
				fcn_mode_obc_mtq3_on();
				ram_data.mode_obc_mtq3_delay_ms = 10;
				break;
			case (uint8_t)obc_mtq3_enum_t::off :
				fcn_mode_obc_mtq3_off();
				ram_data.mode_obc_mtq3_delay_ms = 10;
				break;
			case (uint8_t)obc_mtq3_enum_t::run :
				fcn_mode_obc_mtq3_run();
				ram_data.mode_obc_mtq3_delay_ms = 50;
				break;
			default:
#ifdef debug_uart
				ram_data.mode_obc_mtq3 = 0;
#endif
				break;
		} //switch end
	}//loop end
 }//function end
__weak void fcn_mode_obc_mtq3_Nothing(){
	

	ram_data.mode_obc_mtq3 = (uint8_t)obc_mtq3_enum_t::run;
	//add code below
}

__weak void fcn_mode_obc_mtq3_on(){
	

}

__weak void fcn_mode_obc_mtq3_off(){
	


}

__weak void fcn_mode_obc_mtq3_run(){
	

	MTQz.run_raw_pwm(5000,65534);
	__SEV();
	//add code below
}
#endif
