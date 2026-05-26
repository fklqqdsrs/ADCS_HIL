/*
 * CUBEMAG.h
 *
 *  Created on: Aug 14, 2024
 *      Author: Chinn
 */

#ifndef INC_CUBEMAG_H_
#define INC_CUBEMAG_H_


#include "General.h"
#include "NEWIO/CAN/kmCAN.h"
#include "Mag_REG.h"

#define CUBEMAGID 52

class CUBEMAG{

private:
	kmCAN *cubemag;
	uint8_t cubemagid;
	CAN_RxHeaderTypeDef *RxHeaderCubeMag;
	adcs_rwa_data_t *Rx;
	uint8_t RxBuffer[50];
	//newPortCAN *CUBECAN;


public:
	CUBEMAG(kmCAN * mag)
	{
		cubemag = mag;
		RxHeaderCubeMag = cubemag->get_rx_head();

	//	CUBECAN = cubemag->get_CAN_port();
		//cubemagid = cubemag->get_id();
	}

	uint8_t DeviceCheck()
	{
		cubemag->decode_id(cubemag->get_CAN_port()->get_rx_head()->ExtId);
//		Rx = cubemag->get_rx_buffer();
		if(cubemag->get_rx_buffer()->src_addr == CUBEMAGID)
		{
			return 1;

		}

			return 0;

	}
	void RxMessage()
	{
		uint8_t RxMsg[8];
		static uint8_t RxIndex = 0;

		cubemag->get_CAN_port()->readBytes(cubemag->get_CAN_port()->get_rx_head(), RxMsg);
		//CUBECAN->readBytes(RxHeaderCubeMag, RxMsg);

	    if(DeviceCheck() == 1)
	    {
	    	if((RxMsg[7]+1) > 0)
	    	{
	    		memcpy(RxBuffer+RxIndex,RxMsg,8);
	    		RxIndex += 8;

	    		if(RxMsg[7] == 0)
	    		{
	    			PriMag();
	    			RxIndex = 0;
	    		}
	    	}
	    }

	}



	void PriMag()
	{
//		cubemag_reg[PriMagMeasurement].set_bytes(RxBuffer+9, 4, LittleEndian);
//		cubemag_reg[PriMagMeasurement].set_bytes(RxBuffer+4, 3, LittleEndian);
//		cubemag_reg[PriMagMeasurement].set_bytes(RxBuffer+8, 1, LittleEndian);
//		cubemag_reg[PriMagMeasurement].set_bytes(RxBuffer, 4, LittleEndian);
//		LittleEndianCpy((uint8_t *)CUBEMAG_PRIMAGMEASUREMENT_D+8, 4, RxBuffer+9);
//		LittleEndianCpy((uint8_t *)CUBEMAG_PRIMAGMEASUREMENT_D+4, 3, RxBuffer+4);
//		BigEndianCpy((uint8_t *)CUBEMAG_PRIMAGMEASUREMENT_D+7, 1, RxBuffer+8);
		memcpy(CUBEMAG_PRIMAGMEASUREMENT_D,RxBuffer,4);

	}



};


#endif /* INC_CUBEMAG_H_ */
