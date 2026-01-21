/*
 * KVH1750_REG.h
 *
 *  Created on: Aug 7, 2024
 *      Author: Chinn
 */

#ifndef INC_KVH1750_REG_H_
#define INC_KVH1750_REG_H_


#include "KMUTNB_ADCS_REG.h"

KMUTNB_ADCS_REG kvh1750_reg[4];

enum KVH1750_REG
{
	gyro = 1,
	accel = 2,
	imutemp = 3,
	status = 4,
};

float KVH_GYRO_D[3] = {0,0,0};
float KVH_ACCEL_D[3] = {0,0,0};
int16_t KVH_TEMP_D = 0;
uint8_t KVH_STATUS_D = 0;

void kvh1750_reg_setup()
{
	kvh1750_reg[gyro].init((uint8_t*)&KVH_GYRO_D,sizeof(KVH_GYRO_D),(uint8_t)data_type::_96F);
	kvh1750_reg[accel].init((uint8_t*)&KVH_ACCEL_D,sizeof(KVH_ACCEL_D),(uint8_t)data_type::_96F);
	kvh1750_reg[imutemp].init((uint8_t*)&KVH_TEMP_D,sizeof(KVH_TEMP_D),(uint8_t)data_type::_16L);
	kvh1750_reg[status].init((uint8_t*)&KVH_STATUS_D,sizeof(KVH_STATUS_D),(uint8_t)data_type::_8UL);
}

#endif /* INC_KVH1750_REG_H_ */
