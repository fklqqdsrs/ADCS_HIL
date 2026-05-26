#ifndef INC_TASK_mode_stt_H_
#define INC_TASK_mode_stt_H_
#include "main.h"
#include "TASK_TSC1.h"
#include "REG_TSC1.h"
#include "LIB/STT_REG.h"
#include "LIB/STT.h"

void fcn_mode_stt_Nothing();
void fcn_mode_stt_off();
void fcn_mode_stt_standby();
void fcn_mode_stt_acquisition();
void fcn_mode_stt_tracking();
void fcn_mode_stt_angularRate();
void fcn_mode_stt_selfTest();
void fcn_mode_stt_photo();
enum class stt_enum_t: uint8_t
{
	Nothing = 0,
	off = 1,
	standby = 2,
	acquisition = 3,
	tracking = 4,
	angularRate = 5,
	selfTest = 6,
	photo = 7,
};

__weak void TASK_mode_stt(void* argument){
	for(;;){

		osDelay(ram_data.mode_stt_delay_ms);
		switch(ram_data.mode_stt){
			case (uint8_t)stt_enum_t::Nothing :
				fcn_mode_stt_Nothing();
				ram_data.mode_stt_delay_ms = 1000;
				break;
			case (uint8_t)stt_enum_t::off :
				fcn_mode_stt_off();
				ram_data.mode_stt_delay_ms = 1000;
				break;
			case (uint8_t)stt_enum_t::standby :
				fcn_mode_stt_standby();
				ram_data.mode_stt_delay_ms = 1000;
				break;
			case (uint8_t)stt_enum_t::acquisition :
				fcn_mode_stt_acquisition();
				ram_data.mode_stt_delay_ms = 1000;
				break;
			case (uint8_t)stt_enum_t::tracking :
				fcn_mode_stt_tracking();
				ram_data.mode_stt_delay_ms = 1000;
				break;
			case (uint8_t)stt_enum_t::angularRate :
				fcn_mode_stt_angularRate();
				ram_data.mode_stt_delay_ms = 1000;
				break;
			case (uint8_t)stt_enum_t::selfTest :
				fcn_mode_stt_selfTest();
				ram_data.mode_stt_delay_ms = 1000;
				break;
			case (uint8_t)stt_enum_t::photo :
				fcn_mode_stt_photo();
				ram_data.mode_stt_delay_ms = 1000;
				break;
			default:
#ifdef debug_uart
				ram_data.mode_stt = 0;
#endif
				break;
		} //switch end
	}//loop end
 }//function end
__weak void fcn_mode_stt_Nothing(){
	

	//add code below
}

__weak void fcn_mode_stt_off(){
	


	//add code below
}

__weak void fcn_mode_stt_standby(){
	


	//add code below
}

__weak void fcn_mode_stt_acquisition(){
	


	//add code below
}

__weak void fcn_mode_stt_tracking(){
	


	//add code below
}

__weak void fcn_mode_stt_angularRate(){
	


	//add code below
}

__weak void fcn_mode_stt_selfTest(){
	


	//add code below
}

__weak void fcn_mode_stt_photo(){
	


	//add code below
}
#endif
