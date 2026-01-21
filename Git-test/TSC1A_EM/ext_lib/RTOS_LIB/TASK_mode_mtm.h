#ifndef INC_TASK_mode_mtm_H_
#define INC_TASK_mode_mtm_H_
#include "main.h"
#include "can.h"
#include "TASK_TSC1.h"
#include "REG_TSC1.h"
#include "ClassInit/CubemagInit.h"
#include "NEWIO/CAN/newPortCAN.h"
#include "NEWIO/CAN/kmCAN.h"


//#include "ClassInit/RWInit.h"
//#include "LIB/CUBEMAG.h"

void fcn_mode_mtm_Nothing();
void fcn_mode_mtm_on();
void fcn_mode_mtm_off();
void fcn_mode_mtm_init();
void fcn_mode_mtm_config();
void fcn_mode_mtm_telemetry();
void fcn_mode_mtm_telecommand();
void fcn_mode_mtm_calibrate();
void fcn_mode_mtm_deploy();

newPortCAN CANMTM(&hcan1, 0x01, CAN_RX_FIFO1);
kmCAN MTM(&CANMTM, 0x01);

//static HAL_StatusTypeDef can_it_stat_on;
//static HAL_StatusTypeDef can_it_stat_off;

enum class mtm_enum_t: uint8_t
{
	Nothing = 0,
	on = 1,
	off = 2,
	init = 3,
	config = 4,
	telemetry = 5,
	telecommand = 6,
	calibrate = 7,
	deploy = 8,
};

__weak void TASK_mode_mtm(void* argument){
	for(;;){

		osDelay(ram_data.mode_mtm_delay_ms);
		__HAL_CAN_DISABLE_IT(&hcan1, CAN_IT_RX_FIFO1_MSG_PENDING);
		switch(ram_data.mode_mtm){
			case (uint8_t)mtm_enum_t::Nothing :
				fcn_mode_mtm_Nothing();
				ram_data.mode_mtm_delay_ms = 10;
				break;
			case (uint8_t)mtm_enum_t::on :
				fcn_mode_mtm_on();
				ram_data.mode_mtm_delay_ms = 10;
				break;
			case (uint8_t)mtm_enum_t::off :
				fcn_mode_mtm_off();
				ram_data.mode_mtm_delay_ms = 10;
				break;
			case (uint8_t)mtm_enum_t::init :
				fcn_mode_mtm_init();
				ram_data.mode_mtm_delay_ms = 10;
				break;
			case (uint8_t)mtm_enum_t::config :
				fcn_mode_mtm_config();
				ram_data.mode_mtm_delay_ms = 10;
				break;
			case (uint8_t)mtm_enum_t::telemetry :
				fcn_mode_mtm_telemetry();
				ram_data.mode_mtm_delay_ms = 50;
				break;
			case (uint8_t)mtm_enum_t::telecommand :
				fcn_mode_mtm_telecommand();
				ram_data.mode_mtm_delay_ms = 10;
				break;
			case (uint8_t)mtm_enum_t::calibrate :
				fcn_mode_mtm_calibrate();
				ram_data.mode_mtm_delay_ms = 10;
				break;
			case (uint8_t)mtm_enum_t::deploy :
				fcn_mode_mtm_deploy();
				ram_data.mode_mtm_delay_ms = 10;
				break;
			default:
#ifdef debug_uart
				ram_data.mode_mtm = 0;
#endif
				break;
		} //switch end
		__HAL_CAN_ENABLE_IT(&hcan1, CAN_IT_RX_FIFO1_MSG_PENDING);

	}//loop end
 }//function end
__weak void fcn_mode_mtm_Nothing(){
	

	ram_data.mode_mtm = (uint8_t)mtm_enum_t::on;

}

__weak void fcn_mode_mtm_on(){
	


	//add code below
//	HAL_CAN_DeactivateNotification(&hcan1, CAN_IT_TX_MAILBOX_EMPTY | CAN_IT_RX_FIFO1_MSG_PENDING | CAN_IT_BUSOFF);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, GPIO_PIN_SET);
	ram_data.mode_mtm = (uint8_t)mtm_enum_t::telemetry;
}

__weak void fcn_mode_mtm_off(){
	

	//add code below
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, GPIO_PIN_RESET);
	//ram_data.mode_mtm = (uint8_t)mtm_enum_t::Nothing;
}

__weak void fcn_mode_mtm_init(){
	


	//add code below
}

__weak void fcn_mode_mtm_config(){
	

	//add code below
}

__weak void fcn_mode_mtm_telemetry(){
	


	//add code below
//    whl1.SendCommand(SET, pid_en,      PARAM_TYPE_UINT8, 0);
//    whl1.SendCommand(SET, drv_enable,  PARAM_TYPE_UINT8,  0);

	//cubemag_ram_data.PriMagMeasure
//	MTM.request_data(0x34, 0xC5);
	if (tctlmCubeMagControlProgramCommon1_getPrimaryMagMeasurement(&magEndPoint,&cubemag_ram_data.PriMagMeasure )== CUBEOBC_ERROR_OK)
	{
//		 // sendFloatASCII(cubemag_ram_data.PriMagMeasure.y_axis);
	//sendFloatWithMessage("Magnetic Field X= ", cubemag_ram_data.PriMagMeasureRaw.x_axis);
		__SEV();
	}



}

__weak void fcn_mode_mtm_telecommand(){
	


	//add code below
}

__weak void fcn_mode_mtm_calibrate(){
	


	//add code below
}

__weak void fcn_mode_mtm_deploy(){
	


	//add code below
	tctlmCubeMagControlProgramDeploy1_setDeploy(&magEndPoint,cubemag_ram_data.setDeploy);
	tctlmCubeMagControlProgramDeploy1_setDeployArm(&magEndPoint,cubemag_ram_data.setDeployArm);
	__SEV();
	ram_data.mode_mtm = (uint8_t)mtm_enum_t::telemetry;
}
//
ErrorCode cubeObc_canIfc_tx(CONST CanPacket *packet)
{

    uint32_t MailBox;
	cubeTxHeader.DLC = packet->canSize;
	cubeTxHeader.ExtId = packet->canExtId;
	cubeTxHeader.RTR =  CAN_RTR_DATA;
	cubeTxHeader.IDE = CAN_ID_EXT;

	if (HAL_CAN_AddTxMessage(&hcan1,&cubeTxHeader, packet->canData, &MailBox) == HAL_OK)
	{

		return CUBEOBC_ERROR_OK;
	}

	return CUBEOBC_ERROR_TODO;
}

//void cubeObc_canIfc_rxFlush(void)
//{
//
//}

ErrorCode cubeObc_canIfc_rx( CanPacket *packet)
{

//	HAL_CAN_DeactivateNotification(&hcan1, CAN_IT_RX_FIFO1_MSG_PENDING );


	while(! HAL_CAN_GetRxFifoFillLevel(&hcan1,CAN_RX_FIFO1));

	if(HAL_CAN_GetRxMessage(&hcan1, CAN_RX_FIFO1, &cubeRxHeader, packet->canData) != 0)
	{

		return CUBEOBC_ERROR_TODO;
	}


	packet->canExtId = cubeRxHeader.ExtId;
	packet->canSize= cubeRxHeader.DLC;
	packet->idType = CAN_ID_TYPE_EXTENDED;
	return CUBEOBC_ERROR_OK;


//
//	HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO1_MSG_PENDING );


//
//	if(packet->canExtId != 0)
//	{
//		return CUBEOBC_ERROR_OK;
//	}
//	else
//	{
//		return CUBEOBC_ERROR_TODO;
//	}
//	return CUBEOBC_ERROR_OK;
}



#endif
