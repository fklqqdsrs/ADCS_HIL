#ifndef INC_TASK_mode_run_H_
#define INC_TASK_mode_run_H_
#include "main.h"
#include "TASK_TSC1.h"
#include "REG_TSC1.h"
#include <include/csp/csp.h>
#include "ClassInit/RWInit.h"

void fcn_mode_run_Nothing();
void fcn_mode_run_ping();
void fcn_mode_run_safe();


enum class run_enum_t: uint8_t
{
	Nothing = 0,
	ping = 1,
	safe = 2,
};

__weak void TASK_mode_run(void* argument){
	for(;;){

		osDelay(ram_data.mode_run_delay_ms);
		switch(ram_data.mode_run){
			case (uint8_t)run_enum_t::Nothing :
				fcn_mode_run_Nothing();
				ram_data.mode_run_delay_ms = 1000;
				break;
			case (uint8_t)run_enum_t::ping :
				fcn_mode_run_ping();
				ram_data.mode_run_delay_ms = 1000;
				break;
			case (uint8_t)run_enum_t::safe :
				fcn_mode_run_safe();
				ram_data.mode_run_delay_ms = 10;
				break;
			default:


#ifdef debug_uart
				ram_data.mode_run = 0;
#endif
				break;
		} //switch end
	}//loop end
 }//function end




__weak void fcn_mode_run_Nothing(){

	ram_data.ls_stat |= (HAL_GPIO_ReadPin(LS_GPS_FAU_GPIO_Port, LS_GPS_FAU_Pin) << 0);
	ram_data.ls_stat |= (HAL_GPIO_ReadPin(LS_AB_FAU_GPIO_Port, LS_ABR_FAU_Pin) << 1);
	ram_data.ls_stat |= (HAL_GPIO_ReadPin(LS_FOGR_FAU_GPIO_Port, LS_FOGR_FAU_Pin) << 2);
	ram_data.ls_stat |= (HAL_GPIO_ReadPin(LS_STT_FAU_GPIO_Port, LS_STT_FAU_Pin) << 3);
	ram_data.ls_stat |= (HAL_GPIO_ReadPin(LS_FSS1_FAU_GPIO_Port, LS_FSS1_FAU_Pin) << 4);
	ram_data.ls_stat |= (HAL_GPIO_ReadPin(LS_FSS2_FAU_GPIO_Port, LS_FSS2_FAU_Pin) << 5);
	ram_data.ls_stat |= (HAL_GPIO_ReadPin(LS_FSS3_FAU_GPIO_Port, LS_FSS3_FAU_Pin) << 6);
	ram_data.ls_stat |= (HAL_GPIO_ReadPin(LS_FSS4_FAU_GPIO_Port, LS_FSS4_FAU_Pin) << 7);


//	status_ping1 = csp_ping(12, 1000, 1, 0);

	//add code below
}


__weak void fcn_mode_run_ping(){


//	ram_data.ping_stat = csp_ping(16, 1000, 1, 0);
//	status_ping1 = csp_ping(12, 1000, 1, 0);

	//add code below
}

__weak void fcn_mode_run_safe(){

	// turn off fog
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_1, GPIO_PIN_RESET);
	//turn off gps
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_3, GPIO_PIN_RESET);
	//turn off fss
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_10, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_11, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_RESET);
	//turn off STT
	HAL_GPIO_WritePin(GPIOG, GPIO_PIN_9, GPIO_PIN_RESET);
	//turn off MTM
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, GPIO_PIN_RESET);
	//turn off MTQ
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET);
	//turn off RWA
//	whl3.SendCommand(SET, mrpm_target, PARAM_TYPE_INT32,  0);
//	LoopDelay(10000);
//	whl3.SendCommand(SET, drv_enable,  PARAM_TYPE_UINT8,  0);
//	whl3.SendCommand(SET,auto_resume,PARAM_TYPE_UINT8,0);
//	whl3.Reboot(5);

	ram_data.mode_run = (uint8_t)run_enum_t::ping ;

//	status_ping1 = csp_ping(12, 1000, 1, 0);

	//add code below
}
#endif
