#ifndef INC_TASK_mode_obc_rwa3_H_
#define INC_TASK_mode_obc_rwa3_H_
#include "main.h"
#include "TASK_TSC1.h"
#include "REG_TSC1.h"
#include "ClassInit/RWInit.h"
void fcn_mode_obc_rwa3_Nothing();
void fcn_mode_obc_rwa3_on();
void fcn_mode_obc_rwa3_off();
void fcn_mode_obc_rwa3_torque();
void fcn_mode_obc_rwa3_speed();
void fcn_mode_obc_rwa3_direction_checking();
enum class obc_rwa3_enum_t: uint8_t
{
	Nothing = 0,
	on = 1,
	off = 2,
	torque = 3,
	speed = 4,
	direction_checking = 5,
};

static int32_t last_wheel3_mrpm;
__weak void TASK_mode_obc_rwa3(void* argument){
	for(;;){

		osDelay(ram_data.mode_obc_rwa3_delay_ms);
		switch(ram_data.mode_obc_rwa3){
			case (uint8_t)obc_rwa3_enum_t::Nothing :
				fcn_mode_obc_rwa3_Nothing();
				ram_data.mode_obc_rwa3_delay_ms = 1;
				break;
			case (uint8_t)obc_rwa3_enum_t::on :
				fcn_mode_obc_rwa3_on();
				ram_data.mode_obc_rwa3_delay_ms = 50;
				break;
			case (uint8_t)obc_rwa3_enum_t::off :
				fcn_mode_obc_rwa3_off();
				ram_data.mode_obc_rwa3_delay_ms = 50;
				break;
			case (uint8_t)obc_rwa3_enum_t::torque :
				fcn_mode_obc_rwa3_torque();
				ram_data.mode_obc_rwa3_delay_ms = 50;
				break;
			case (uint8_t)obc_rwa3_enum_t::speed :
				fcn_mode_obc_rwa3_speed();
				ram_data.mode_obc_rwa3_delay_ms = 50;
				break;
			case (uint8_t)obc_rwa3_enum_t::direction_checking :
				fcn_mode_obc_rwa3_direction_checking();
				ram_data.mode_obc_rwa3_delay_ms = 50;
				break;
			default:
#ifdef debug_uart
				ram_data.mode_obc_rwa3 = 0;
#endif
				break;
		} //switch end
	}//loop end
 }//function end
__weak void fcn_mode_obc_rwa3_Nothing(){
	


	//add code below
	ram_data.mode_obc_rwa3 = (uint8_t)obc_rwa3_enum_t::on ;
}

__weak void fcn_mode_obc_rwa3_on(){
	
	whl3.SendCommand(SET, auto_resume,      PARAM_TYPE_UINT8,  0);

	//add code below
  for(int i = 0; i<3; i++)
  {
	whl3.SendCommand(SET, pid_en,      PARAM_TYPE_UINT8,  1);
	whl3.SendCommand(SET, drv_enable,  PARAM_TYPE_UINT8,  1);
  }
	ram_data.mode_obc_rwa3 = (uint8_t)obc_rwa3_enum_t::speed;
}

__weak void fcn_mode_obc_rwa3_off(){
	


	//add code below
	whl3.SendCommand(SET, mrpm_target, PARAM_TYPE_INT32,  0);
	LoopDelay(10000);
//	whl3.SendCommand(SET, pid_en,      PARAM_TYPE_UINT8,  0);
	whl3.SendCommand(SET, drv_enable,  PARAM_TYPE_UINT8,  0);
//	whlreg[WHEEL3].WHL_MRPM_D = 0;
	whl3.Reboot(5);
	start_flag = 0;
}

__weak void fcn_mode_obc_rwa3_torque(){
	


	//add code below
}

__weak void fcn_mode_obc_rwa3_speed(){
	


	//add code below
	if(whlreg[WHEEL3].WHL_MRPM_TARGET_D != last_wheel3_mrpm)
	{
		whl3.SendCommand(SET, mrpm_target, PARAM_TYPE_INT32,whlreg[WHEEL3].WHL_MRPM_TARGET_D );
	}
	whl3.SendCommand(GET,mrpm );
	whl3.SendCommand(GET,current );
	last_wheel3_mrpm = whlreg[WHEEL3].WHL_MRPM_TARGET_D;

}

__weak void fcn_mode_obc_rwa3_direction_checking(){
	


	//add code below
}
#endif
