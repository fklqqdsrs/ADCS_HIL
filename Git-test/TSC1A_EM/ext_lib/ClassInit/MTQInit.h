/*
 * MTQInit.h
 *
 *  Created on: Jan 27, 2025
 *      Author: Chinnapop
 */

#ifndef MTQINIT_H_
#define MTQINIT_H_

#include "i2c.h"
#include "LIB/I2C/newPortI2C.h"
#include "LIB/INA219/newINA.h"
#include "LIB/STSPIN/newMTQ.h"
#include "ClassInit/MTQpin.h"
#include "ClassInit/MTQ_header.h"
#include "LIB/MTQ.h"


newPortI2C I2C2BUS(&hi2c2, AS_MASTER);
newINA x_sensor(&I2C2BUS, (0x40<< 1));
newINA y_sensor(&I2C2BUS, (0x41<< 1));
newINA z_sensor(&I2C2BUS, (0x44<< 1));

NEWMTQ MTQx(&mtq_driver_x,&x_sensor,&mtq_profile_x);
NEWMTQ MTQy(&mtq_driver_y,&y_sensor,&mtq_profile_y);
NEWMTQ MTQz(&mtq_driver_z,&z_sensor,&mtq_profile_z);


void MTQinit()
{
	  x_sensor.begin();
	  y_sensor.begin();
	  z_sensor.begin();

	  mtq_driver_x.begin();
	  mtq_driver_y.begin();
	  mtq_driver_z.begin();

	  MTQ_setup();

}


#endif /* MTQINIT_H_ */
