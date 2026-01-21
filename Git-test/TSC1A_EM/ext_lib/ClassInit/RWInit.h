/*
 * RWInit.h
 *
 *  Created on: Jan 23, 2025
 *      Author: Chinnapop
 */

#ifndef RWINIT_H_
#define RWINIT_H_

#include "main.h"
#include "can.h"
#include "LIB/WHL500.h"

newPortCAN CANWHL1(&hcan1, 0x01, CAN_RX_FIFO1);
newPortCAN CANWHL2(&hcan1, 0x01, CAN_RX_FIFO1);
newPortCAN CANWHL3(&hcan1, 0x01, CAN_RX_FIFO1);
newPortCAN CANWHL4(&hcan1, 0x01, CAN_RX_FIFO1);


WHL500 whl1(&CANWHL1,0x01,WHEEL1);
WHL500 whl2(&CANWHL2,0x01,WHEEL2);
WHL500 whl3(&CANWHL3,0x01,WHEEL3);
WHL500 whl4(&CANWHL4,0x01,WHEEL4);

void RWinit()
{
//	  CANWHL1.set_filter(CAN_RX_FIFO1);
//	  CANWHL1.set_rxtx_head(whl1.getCANRxHeader(), whl1.getCANTxHeader());
//
//	  CANWHL2.set_filter(CAN_RX_FIFO1);
//	  CANWHL2.set_rxtx_head(whl2.getCANRxHeader(), whl2.getCANTxHeader());
//
//	  CANWHL3.set_filter(CAN_RX_FIFO1);
//	  CANWHL3.set_rxtx_head(whl3.getCANRxHeader(), whl3.getCANTxHeader());
//
//	  CANWHL4.set_filter(CAN_RX_FIFO1);
//	  CANWHL4.set_rxtx_head(whl4.getCANRxHeader(), whl4.getCANTxHeader());

	  whl1.SetMessageParam(PRIOR_HIGH,2, PARAMETER_SERVICE, 35, 0x8B);
	  whl2.SetMessageParam(PRIOR_HIGH,4, PARAMETER_SERVICE, 35, 0x8B);
	  whl3.SetMessageParam(PRIOR_HIGH,5, PARAMETER_SERVICE, 35, 0x8B);
	  whl4.SetMessageParam(PRIOR_HIGH,6, PARAMETER_SERVICE, 35, 0x8B);

	  whl1.SetDeviceIDfilter(7);
	  whl2.SetDeviceIDfilter(4);
	  whl3.SetDeviceIDfilter(5);
	  whl4.SetDeviceIDfilter(6);



}



#endif /* RWINIT_H_ */
