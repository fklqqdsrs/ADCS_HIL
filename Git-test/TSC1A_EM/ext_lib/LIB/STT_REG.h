/*
 * STT_REG.h
 *
 *  Created on: Mar 7, 2025
 *      Author: Chinnapop
 */

#ifndef LIB_STT_REG_H_
#define LIB_STT_REG_H_

#include "REG_MAP/NEW_BIT_REG.h"
#include "REG_MAP/KMUTNB_ADCS_REG.h"

KMUTNB_ADCS_REG stt_reg[3];

enum STT_REG
{
	stt_vector = 0,
};

typedef struct{

	uint16_t stt_vector = 0;

}stt_data_t;

stt_data_t stt_ram_data;

void STT_Setup()
{
	stt_reg[stt_vector].init((uint8_t*)&stt_ram_data.stt_vector, sizeof(uint16_t) );

}



#endif /* LIB_STT_REG_H_ */
