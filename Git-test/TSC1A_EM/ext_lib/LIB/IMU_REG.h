/*
 * IMU_REG.h
 *
 *  Created on: Jan 29, 2025
 *      Author: Chinnapop
 */

#ifndef LIB_IMU_REG_H_
#define LIB_IMU_REG_H_


#include "REG_MAP/NEW_BIT_REG.h"
#include "REG_MAP/KMUTNB_ADCS_REG.h"

KMUTNB_ADCS_REG imu_reg[3];

enum IMU_REG
{
	gyro = 0,
	accel = 1,
	imu_temp = 2,
	imu_stat = 3,
	gyro_x = 4,
	gyro_y = 5,
	gyro_z = 6,

};

typedef struct{
	float gyro_z;
	float gyro_y;
	float gyro_x;

}Gyro;

typedef struct{
	float accel_z;
	float accel_y;
	float accel_x;

}Accel;

typedef struct{

	Gyro gyro_dat = {0,0,0};
	Accel accel_data = {0,0,0};
	int16_t temp = 0;
	uint8_t stat = 0;
	float gyro_x = 0;
	float gyro_y = 0;
	float gyro_z = 0;

}imu_data_t;

imu_data_t imu_ram_data;

void IMU_Setup()
{
	imu_reg[gyro].init((uint8_t*)&imu_ram_data.gyro_dat, sizeof(float)*3 );
	imu_reg[accel].init((uint8_t*)&imu_ram_data.accel_data, sizeof(float)*3 );
	imu_reg[imu_temp].init((uint8_t*)&imu_ram_data.temp, sizeof(int16_t) );
	imu_reg[imu_stat].init((uint8_t*)&imu_ram_data.stat, sizeof(uint8_t) );
	imu_reg[gyro_x].init((uint8_t*)&imu_ram_data.gyro_x, sizeof(float) );
	imu_reg[gyro_y].init((uint8_t*)&imu_ram_data.gyro_y, sizeof(float) );
	imu_reg[gyro_z].init((uint8_t*)&imu_ram_data.gyro_z, sizeof(float) );

}
#endif /* LIB_IMU_REG_H_ */
