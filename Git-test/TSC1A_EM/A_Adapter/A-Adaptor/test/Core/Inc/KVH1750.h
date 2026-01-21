/*
 * KVH1750.h
 *
 *  Created on: Aug 7, 2024
 *      Author: Chinn
 */

#ifndef INC_KVH1750_H_
#define INC_KVH1750_H_


#include "KVH1750_REG.h"
#include "main.h"

#define FORMATA 36
#define FORMATB 40
#define DEFAULT FORMATA

typedef struct
{
	uint8_t GyroX;
	uint8_t GyroY;
	uint8_t GyroZ;
	uint8_t AccelX;
	uint8_t AccelY;
	uint8_t AccelZ;

}IMUSTAT;

class KVH1750{


private:
	uint8_t RxBuffer[40];


public:


	void GetData()
	{


		kvh1750_reg[gyro].set_bytes(RxBuffer+4,12,BigEndian);
		kvh1750_reg[accel].set_bytes(RxBuffer+16,12,BigEndian);
		kvh1750_reg[imutemp].set_bytes(RxBuffer+30,2,BigEndian);
		kvh1750_reg[status].set_bytes(RxBuffer+28,1,BigEndian);



	}
};


#endif /* INC_KVH1750_H_ */
