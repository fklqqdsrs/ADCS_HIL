/*
 * OEM719_REG.h
 *
 *  Created on: May 2, 2024
 *      Author: Chinn
 */

#ifndef INC_OEM719_REG_H_
#define INC_OEM719_REG_H_

#include "BIT_REG.h"
#include "KMUTNB_ADCS_REG.h"

KMUTNB_ADCS_REG oem719_reg[8];

enum OEM719_REG
{
	IMUratePVASId = 0x00,
	INSPVASId = 0x01,
	RawIMUsId = 0x02,
	bestPosId = 0x03,
	bestXYZId = 0x04,
	hwmonitorId = 0x05,
	Unix = 0x06,
	receiverstat = 0x07,

};

double OEM_BestPos_D[3] = {0,0,0};
double OEM_BestXYZ_D[6] = {0,0,0,0,0,0};
float  OEM_Temp_D = 0;
float  OEM_RawIMU_D[7] = {0,0,0,0,0,0,0};
double OEM_Gyro_D[3] = {0,0,0};
uint32_t OEM_Unix_D = 0;
uint32_t OEM_receiverstat_D = 0;

void oem719_reg_setup()
{
	oem719_reg[bestPosId].init((uint8_t*)&OEM_BestPos_D,sizeof(OEM_BestPos_D),(uint8_t)data_type::_192D);
	oem719_reg[bestXYZId].init((uint8_t*)&OEM_BestXYZ_D,sizeof(OEM_BestXYZ_D),(uint8_t)data_type::_384D);
	oem719_reg[hwmonitorId].init((uint8_t*)&OEM_Temp_D,sizeof(OEM_Temp_D),(uint8_t)data_type::_32F);
	oem719_reg[RawIMUsId].init((uint8_t*)&OEM_RawIMU_D,sizeof(OEM_RawIMU_D),(uint8_t)data_type::_224F);
	oem719_reg[INSPVASId].init((uint8_t*)&OEM_Gyro_D,sizeof(OEM_Gyro_D),(uint8_t)data_type::_192D);
	oem719_reg[Unix].init((uint8_t*)&OEM_Unix_D,sizeof(OEM_Unix_D),(uint8_t)data_type::_32UL);
	oem719_reg[receiverstat].init((uint8_t*)&OEM_receiverstat_D,sizeof(OEM_receiverstat_D),(uint8_t)data_type::_32UL);
}

#endif /* INC_OEM719_REG_H_ */
