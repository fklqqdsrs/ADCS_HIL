/*
 * FSS_REG.h
 *
 *  Created on: Mar 7, 2025
 *      Author: Chinnapop
 */

#ifndef LIB_FSS_REG_H_
#define LIB_FSS_REG_H_

#include "REG_MAP/NEW_BIT_REG.h"
#include "REG_MAP/KMUTNB_ADCS_REG.h"

KMUTNB_ADCS_REG fss_reg[3];

enum FSS_REG
{
	id = 0,
};

typedef struct{

	uint8_t ID = 0;

}fss_data_t;

fss_data_t fss_ram_data;

void FSS_Setup()
{
	fss_reg[id].init((uint8_t*)&fss_ram_data.ID, sizeof(uint8_t) );

}



#endif /* LIB_FSS_REG_H_ */
