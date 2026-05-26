/*
 * IMU.h
 *
 *  Created on: Aug 26, 2024
 *      Author: Chinn
 */

#ifndef INC_IMU_H_
#define INC_IMU_H_

#include "General.h"
#include "IMU_REG.h"

#define VALID_DATA 1
#define INVALD_DATA 0

uint8_t entryConfigMode[] = "=CONFIG,1";
uint8_t exitConfigMode[] = "=CONFIG,0";

uint8_t dataRate1HZ[] = "DR,1";
uint8_t dataRate5HZ[] = "DR,5";
uint8_t dataRate10HZ[] = "DR,10";
uint8_t dataRate25HZ[] = "DR,25";
uint8_t dataRate50HZ[] = "DR,50";
uint8_t dataRate100HZ[] = "DR,100";


typedef struct {
    uint8_t gyro_x : 1;   // Bit 0
    uint8_t gyro_y : 1;   // Bit 1
    uint8_t gyro_z : 1;   // Bit 2
    uint8_t reserved1 : 1;// Bit 3 (Always 0)
    uint8_t accel_x : 1;  // Bit 4
    uint8_t accel_y : 1;  // Bit 5
    uint8_t accel_z : 1;  // Bit 6
    uint8_t reserved2 : 1;// Bit 7 (Always 0)
} imuStatus;

class IMU{
private:
	uint8_t ImuBuffer[36];
	UART_HandleTypeDef *uart;
	imuStatus imu_stat;


public:
	IMU(UART_HandleTypeDef *Uart)
	{
		uart = Uart;
	}
	void IMUGyro()
	{

		  HAL_UART_Receive_DMA(uart, ImuBuffer, 36);
	//	  oem719_reg[bestPosId].set_bytes(Buffer+36 , 24,LittleEndian);
		  imu_reg[gyro].set_bytes(ImuBuffer, 12, BigEndian);
	//	  reverse_bytes(ImuBuffer+15, (uint8_t*)&imu_ram_data.gyro_dat, 12);
	//	  BigEndianCpy((uint8_t *)imu_ram_data.gyro,12,ImuBuffer+4);
	//	  BigEndianCpy((uint8_t *)Accel,12,buffer+16);
	//	  BigEndianCpy((uint8_t *)&Temp,2,buffer+30);
	//	  Status = buffer[28];
	//	  Yaw =  Gyro[0];
	//	  Roll =  Gyro[1];
	//	  Pitch = Gyro[2];
	}
	uint8_t * returnBuffer()
	{
		return ImuBuffer;

	}

	void decodeimuStatus() {


		imu_stat.gyro_x   = (imu_ram_data.stat >> 0) & 1;
		imu_stat.gyro_y   = (imu_ram_data.stat >> 1) & 1;
		imu_stat.gyro_z   = (imu_ram_data.stat >> 2) & 1;
	    imu_stat.reserved1 = (imu_ram_data.stat >> 3) & 1;
	    imu_stat.accel_x  = (imu_ram_data.stat >> 4) & 1;
	    imu_stat.accel_y  = (imu_ram_data.stat >> 5) & 1;
	    imu_stat.accel_z  = (imu_ram_data.stat >> 6) & 1;
	    imu_stat.reserved2 = (imu_ram_data.stat >> 7) & 1;


	}

};








#endif /* INC_IMU_H_ */
