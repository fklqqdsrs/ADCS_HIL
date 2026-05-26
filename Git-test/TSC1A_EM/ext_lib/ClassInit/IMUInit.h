/*
 * IMUInit.h
 *
 *  Created on: Jan 27, 2025
 *      Author: Chinnapop
 */

#ifndef CLASSINIT_IMUINIT_H_
#define CLASSINIT_IMUINIT_H_

#include "main.h"
#include "usart.h"
#include "LIB/IMU.h"

IMU KVH1750(&huart2);

void IMUInit()
{
	IMU_Setup();

}




#endif /* CLASSINIT_IMUINIT_H_ */
