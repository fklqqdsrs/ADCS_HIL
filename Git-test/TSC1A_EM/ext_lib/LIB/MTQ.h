/*
 * MTQ.h
 *
 *  Created on: Jan 27, 2025
 *      Author: Chinnapop
 */

#ifndef LIB_MTQ_H_
#define LIB_MTQ_H_

#include "REG_MAP/KMUTNB_ADCS_REG.h"

KMUTNB_ADCS_REG  mtq_reg[3][2];

enum MTQ{
	MTQ1 = 0,
	MTQ2 = 1,
	MTQ3 = 2,

};
enum MTQ_REG{
	Current = 0,
	PWM = 1,
};

typedef struct
{
	int16_t MTQ_current = 0;
	int16_t cur_pwm = 0;

}MTQ_data_t;

MTQ_data_t MTQ_ram_data[3];

void MTQ_setup()
{
	for(int i = 0; i <3; i++)
	{
		mtq_reg[i][Current].init((uint8_t*)&MTQ_ram_data[i].MTQ_current,sizeof(int16_t));
		mtq_reg[i][PWM].init((uint8_t*)&MTQ_ram_data[i].cur_pwm,sizeof(int16_t));
	}

}




#endif /* LIB_MTQ_H_ */
