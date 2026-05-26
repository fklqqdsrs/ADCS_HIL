#ifndef INC_TASK_mode_obc_rwa1_H_
#define INC_TASK_mode_obc_rwa1_H_
#include "main.h"
#include "TASK_TSC1.h"
#include "REG_TSC1.h"
#include "ClassInit/RWInit.h"

void fcn_mode_obc_rwa1_Nothing();
void fcn_mode_obc_rwa1_on();
void fcn_mode_obc_rwa1_off();
void fcn_mode_obc_rwa1_torque();
void fcn_mode_obc_rwa1_speed();
void fcn_mode_obc_rwa1_direction_checking();
enum class obc_rwa1_enum_t: uint8_t
{
	Nothing = 0,
	on = 1,
	off = 2,
	torque = 3,
	speed = 4,
	direction_checking = 5,
};

static int32_t last_mrpm = 0;
static bool start_flag = 0;
__weak void TASK_mode_obc_rwa1(void* argument){
	for(;;){

		osDelay(ram_data.mode_obc_rwa1_delay_ms);
		switch(ram_data.mode_obc_rwa1){
			case (uint8_t)obc_rwa1_enum_t::Nothing :
				fcn_mode_obc_rwa1_Nothing();
				ram_data.mode_obc_rwa1_delay_ms = 1;
				break;
			case (uint8_t)obc_rwa1_enum_t::on :
				fcn_mode_obc_rwa1_on();
				ram_data.mode_obc_rwa1_delay_ms = 50;
				break;
			case (uint8_t)obc_rwa1_enum_t::off :
				fcn_mode_obc_rwa1_off();
				ram_data.mode_obc_rwa1_delay_ms = 50;
				break;
			case (uint8_t)obc_rwa1_enum_t::torque :
				fcn_mode_obc_rwa1_torque();
				ram_data.mode_obc_rwa1_delay_ms = 50;
				break;
			case (uint8_t)obc_rwa1_enum_t::speed :
				fcn_mode_obc_rwa1_speed();
				ram_data.mode_obc_rwa1_delay_ms = 50;
				break;
			case (uint8_t)obc_rwa1_enum_t::direction_checking :
				fcn_mode_obc_rwa1_direction_checking();
				ram_data.mode_obc_rwa1_delay_ms = 50;
				break;
			default:
#ifdef debug_uart
				ram_data.mode_obc_rwa1 = 0;
#endif
				break;
		} //switch end
	}//loop end
 }//function end
__weak void fcn_mode_obc_rwa1_Nothing(){
	


	//add code below

		ram_data.mode_obc_rwa1 = (uint8_t)obc_rwa1_enum_t::on ;
}

__weak void fcn_mode_obc_rwa1_on(){
	


	//add code below
//	if(start_flag == 0)
//	{
	  for(int i = 0; i<3; i++)
	  {
		whl1.SendCommand(SET, pid_en,      PARAM_TYPE_UINT8,  1);
		whl1.SendCommand(SET, drv_enable,  PARAM_TYPE_UINT8,  1);
	  }

//	}
//	start_flag = 1;
	ram_data.mode_obc_rwa1 = (uint8_t)obc_rwa1_enum_t::speed;
}

__weak void fcn_mode_obc_rwa1_off(){
	

	whl1.SendCommand(SET, mrpm_target, PARAM_TYPE_INT32,  0);
	LoopDelay(10000);
//	whl1.SendCommand(SET, pid_en,      PARAM_TYPE_UINT8,  0);
	whl1.SendCommand(SET, drv_enable,  PARAM_TYPE_UINT8,  0);
//	whlreg[WHEEL1].WHL_MRPM_D = 0;
	whl1.Reboot(2);
	start_flag = 0;

	//add code below
}

__weak void fcn_mode_obc_rwa1_torque(){
	

	//add code below
}

__weak void fcn_mode_obc_rwa1_speed(){
	


	if (whlreg[WHEEL1].WHL_MRPM_TARGET_D != last_mrpm)
	{
		whl1.SendCommand(SET, mrpm_target, PARAM_TYPE_INT32,whlreg[WHEEL1].WHL_MRPM_TARGET_D );
	}
	whl1.SendCommand(GET,mrpm );
	whl1.SendCommand(GET,current );
	last_mrpm = whlreg[WHEEL1].WHL_MRPM_TARGET_D;
	//add code below
}

__weak void fcn_mode_obc_rwa1_direction_checking(){
	


	//add code below
}
#endif
