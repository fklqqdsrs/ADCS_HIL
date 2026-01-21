/*
 * GNSS_INIT.h
 *
 *  Created on: Jan 23, 2025
 *      Author: Chinnapop
 */

#ifndef GNSS_INIT_H_
#define GNSS_INIT_H_

//#include "NEWIO/newPort.h"
//#include "NEWIO/CAN/newPortCAN.h"
#include "LIB/OEM719.h"
#include "main.h"
#include "can.h"

newPortCAN CANGNSS(&hcan1, 0x11, CAN_RX_FIFO1);
OEM719 GNSS(&CANGNSS,0x11,0x06);


void GNSSInit()
{
//	CANGNSS.set_filter(CAN_RX_FIFO1);

	oem719_reg_setup();

}


#endif /* GNSS_INIT_H_ */
