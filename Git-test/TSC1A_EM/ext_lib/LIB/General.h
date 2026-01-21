/*
 * General.h
 *
 *  Created on: Apr 2, 2024
 *      Author: Chinn
 */

#ifndef INC_GENERAL_H_
#define INC_GENERAL_H_

#include "main.h"
#include "string.h"
#include "stdio.h"
#include "stdint.h"



void LoopDelay(uint16_t NumLoop)
{
	for(uint16_t i = 0; i<NumLoop; i++){}
}


void BigEndian2bytesCpy(uint8_t *dat, uint16_t val)
{

    	dat[0] = val >> 8; // Most significant byte
    	dat[1] = val;

}



void BigEndian4bytesCpy(uint8_t *dat, uint32_t val)
{

    dat[0] = val >> 24; // Most significant byte
    dat[1] = val >> 16;
    dat[2] = val >> 8;
    dat[3] = val ;

}

void BigEndian8bytesCpy(uint8_t *dat, uint64_t val)
{

    dat[0] = val >> 56; // Most significant byte
    dat[1] = val >> 48;
    dat[2] = val >> 40;
    dat[3] = val >> 32;
    dat[4] = val >> 24;
    dat[5] = val >> 16;
    dat[6] = val >> 8;
    dat[7] = val;


	    for (int i = 0; i < 8; i++) {
	        dat[i] = val >> (56 - 8 * i);
	    }

}

void BigEndianCpy(uint8_t *dat,uint16_t size,uint8_t val[])
{
	 for (int i = 0; i < size; i++)
	 {
		 dat[i] = val[size-1-i];
	 }

}

void LittleEndianCpy(uint8_t *dat,uint16_t size,uint8_t val[])
{
	 for (int i = 0; i < size; i++)
	 {
		 dat[i] = val[i];
	 }

}


void reverse_bytes(uint8_t *src, uint8_t *dst, uint32_t len) {
    while (len >= 4) { // Process 4 bytes at a time
        dst[0] = src[len - 1];
        dst[1] = src[len - 2];
        dst[2] = src[len - 3];
        dst[3] = src[len - 4];
        dst += 4;
        len -= 4;
    }
    while (len--) { // Process remaining bytes
        *dst++ = *src--;
    }
}


#endif /* INC_GENERAL_H_ */
