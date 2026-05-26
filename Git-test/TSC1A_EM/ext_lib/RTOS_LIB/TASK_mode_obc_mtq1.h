#ifndef INC_TASK_mode_obc_mtq1_H_
#define INC_TASK_mode_obc_mtq1_H_
#include "main.h"
#include "TASK_TSC1.h"
#include "REG_TSC1.h"
#include "TASK_mode_mtq.h"

void fcn_mode_obc_mtq1_Nothing();
void fcn_mode_obc_mtq1_on();
void fcn_mode_obc_mtq1_off();
void fcn_mode_obc_mtq1_run();

static int16_t last_pwm = 0;

enum class obc_mtq1_enum_t: uint8_t
{
	Nothing = 0,
	on = 1,
	off = 2,
	run = 3,
};

__weak void TASK_mode_obc_mtq1(void* argument){
	for(;;){

		osDelay(ram_data.mode_obc_mtq1_delay_ms);
		switch(ram_data.mode_obc_mtq1){
			case (uint8_t)obc_mtq1_enum_t::Nothing :
				fcn_mode_obc_mtq1_Nothing();
				ram_data.mode_obc_mtq1_delay_ms = 1;
				break;
			case (uint8_t)obc_mtq1_enum_t::on :
				fcn_mode_obc_mtq1_on();
				ram_data.mode_obc_mtq1_delay_ms = 10;
				break;
			case (uint8_t)obc_mtq1_enum_t::off :
				fcn_mode_obc_mtq1_off();
				ram_data.mode_obc_mtq1_delay_ms = 10;
				break;
			case (uint8_t)obc_mtq1_enum_t::run :
				fcn_mode_obc_mtq1_run();
				ram_data.mode_obc_mtq1_delay_ms = 50;
				break;
			default:
#ifdef debug_uart
				ram_data.mode_obc_mtq1 = 0;
#endif
				break;
		} //switch end
	}//loop end
 }//function end
__weak void fcn_mode_obc_mtq1_Nothing(){

	ram_data.mode_obc_mtq1 = (uint8_t)obc_mtq1_enum_t::on;

}

__weak void fcn_mode_obc_mtq1_on(){
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_SET);
	ram_data.mode_obc_mtq1 = (uint8_t)obc_mtq1_enum_t::run;

//	MTQx.run_raw_pwm(15000,65534);

}

__weak void fcn_mode_obc_mtq1_off(){
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET);

}

__weak void fcn_mode_obc_mtq1_run(){
	

	if (MTQ_ram_data[MTQ1].cur_pwm != last_pwm)
	{
		MTQx.run_raw_pwm(MTQ_ram_data[MTQ1].cur_pwm,65534);
	}


	//add code below
//	MTQ_ram_data.cur_pwm[0] = 5000;
//	MTQx.run_raw_pwm(MTQ_ram_data[MTQ1].cur_pwm,65534);
//	MTQx.run_raw_pwm(15000,65534);
    MTQ_ram_data[MTQ1].MTQ_current = (int16_t)MTQx.read_mA();
    last_pwm = MTQ_ram_data[MTQ1].cur_pwm;
	__SEV();
}
#endif
