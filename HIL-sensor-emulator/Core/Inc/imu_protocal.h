/*
 * imu_protocal.h
 *
 *  Created on: Jan 20, 2026
 *      Author: Chinnapop
 */

#ifndef INC_IMU_PROTOCAL_H_
#define INC_IMU_PROTOCAL_H_

#include "stdint.h"
#include <string.h>

typedef struct {
    uint8_t gyro_x : 1;   // Bit 0
    uint8_t gyro_y : 1;   // Bit 1
    uint8_t gyro_z : 1;   // Bit 2
    uint8_t reserved1 : 1;// Bit 3
    uint8_t accel_x : 1;  // Bit 4
    uint8_t accel_y : 1;  // Bit 5
    uint8_t accel_z : 1;  // Bit 6
    uint8_t reserved2 : 1;// Bit 7
} imuStatus;

typedef struct{
	float Xrot;
	float Yrot;
	float Zrot;
	float Xaccel;
	float Yaccel;
	float Zaccel;
	uint8_t status;
	uint8_t seq;
	int16_t temp;

}message_data_format_t;

typedef struct  {

	uint32_t header;
	message_data_format_t data;
	uint32_t crc;

}message_format_t;

void crc_generator();
uint32_t calculate_crc32(const uint8_t *data, size_t length);
void reverse_bytes(uint8_t *src, uint8_t *dst, uint32_t len);

#endif /* INC_IMU_PROTOCAL_H_ */
