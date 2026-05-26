/*
 * GNSS.c
 *
 *  Created on: 03.10.2020
 *      Author: SimpleMethod
 *
 *Copyright 2020 SimpleMethod
 *
 *Permission is hereby granted, free of charge, to any person obtaining a copy of
 *this software and associated documentation files (the "Software"), to deal in
 *the Software without restriction, including without limitation the rights to
 *use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
 *of the Software, and to permit persons to whom the Software is furnished to do
 *so, subject to the following conditions:
 *
 *The above copyright notice and this permission notice shall be included in all
 *copies or substantial portions of the Software.
 *
 *THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *THE SOFTWARE.
 ******************************************************************************
 */

#include "app_modules_config.h"

#if ENABLE_TIME_GNSS

#include "GNSS.h"
#include "cmsis_os.h"
#include "kservice.h"

GNSS_StateHandle GNSS_Handle;

union u_Short uShort;
union i_Short iShort;
union u_Long uLong;
union i_Long iLong;

/*!
 * Structure initialization.
 * @param GNSS Pointer to main GNSS structure.
 * @param huart Pointer to uart handle.
 */
void GNSS_Init(GNSS_StateHandle *GNSS, UART_HandleTypeDef *huart) {
	GNSS->huart = huart;
	GNSS->year = 0;
	GNSS->month = 0;
	GNSS->day = 0;
	GNSS->hour = 0;
	GNSS->min = 0;
	GNSS->sec = 0;
	GNSS->fixType = 0;
	GNSS->lon = 0;
	GNSS->lat = 0;
	GNSS->height = 0;
	GNSS->hMSL = 0;
	GNSS->hAcc = 0;
	GNSS->vAcc = 0;
	GNSS->gSpeed = 0;
	GNSS->headMot = 0;
}

/*!
 * Searching for a header in data buffer and matching class and message ID to buffer data.
 * @param GNSS Pointer to main GNSS structure.
 */
int GNSS_ParseBuffer(GNSS_StateHandle *GNSS) {

	if (GNSS->uartWorkingBuffer[0] == 0xB5 && GNSS->uartWorkingBuffer[1] == 0x62) {
		if(GNSS_ChecksumRx(GNSS, NULL, NULL) == 0){
			if (GNSS->uartWorkingBuffer[2] == 0x27 && GNSS->uartWorkingBuffer[3] == 0x03) { //Look at: 32.19.1.1 u-blox 8 Receiver description
				GNSS_ParseUniqID(GNSS);
				return 0;
			} else if (GNSS->uartWorkingBuffer[2] == 0x01 && GNSS->uartWorkingBuffer[3] == 0x21) { //Look at: 32.17.14.1 u-blox 8 Receiver description
				GNSS_ParseNavigatorData(GNSS);
				return 0;
			} else if (GNSS->uartWorkingBuffer[2] == 0x01 && GNSS->uartWorkingBuffer[3] == 0x07) { //Look at: 32.17.30.1 u-blox 8 Receiver description
				GNSS_ParsePVTData(GNSS);
				return 0;
			} else if (GNSS->uartWorkingBuffer[2] == 0x01 && GNSS->uartWorkingBuffer[3] == 0x02) { //Look at: 32.17.15.1 u-blox 8 Receiver description
				GNSS_ParsePOSLLHData(GNSS);
				return 0;
			}
		}
	}

	return 1;
}

/*!
 * Make request for unique chip ID data.
 * @param GNSS Pointer to main GNSS structure.
 */
void GNSS_GetUniqID(GNSS_StateHandle *GNSS) {
	HAL_UART_Transmit_DMA(GNSS->huart, getDeviceID, sizeof(getDeviceID) / sizeof(uint8_t));
	HAL_UART_Receive_IT(GNSS->huart, GNSS_Handle.uartWorkingBuffer, 17);
}

/*!
 * Make request for UTC time solution data.
 * @param GNSS Pointer to main GNSS structure.
 */
void GNSS_GetNavigatorData(GNSS_StateHandle *GNSS) {
	HAL_UART_Transmit_DMA(GNSS->huart, getNavigatorData, sizeof(getNavigatorData) / sizeof(uint8_t));
	HAL_UART_Receive_IT(GNSS->huart, GNSS_Handle.uartWorkingBuffer, 28);
}

/*!
 * Make request for geodetic position solution data.
 * @param GNSS Pointer to main GNSS structure.
 */
void GNSS_GetPOSLLHData(GNSS_StateHandle *GNSS) {
	HAL_UART_Transmit_DMA(GNSS->huart, getPOSLLHData, sizeof(getPOSLLHData) / sizeof(uint8_t));
	HAL_UART_Receive_IT(GNSS->huart, GNSS_Handle.uartWorkingBuffer, 36);
}

/*!
 * Make request for navigation position velocity time solution data.
 * @param GNSS Pointer to main GNSS structure.
 */
void GNSS_GetPVTData(GNSS_StateHandle *GNSS) {
	tsc_memset(GNSS_Handle.uartWorkingBuffer, 0, sizeof(GNSS_Handle.uartWorkingBuffer));
	HAL_UART_Transmit_DMA(GNSS->huart, getPVTData, sizeof(getPVTData) / sizeof(uint8_t));
	HAL_UART_Receive_IT(GNSS->huart, GNSS_Handle.uartWorkingBuffer, 100);
}

/*!
 * Parse data to unique chip ID standard.
 * Look at: 32.19.1.1 u-blox 8 Receiver description
 * @param GNSS Pointer to main GNSS structure.
 */
void GNSS_ParseUniqID(GNSS_StateHandle *GNSS) {
	for (int var = 0; var < 5; ++var) {
		GNSS->uniqueID[var] = GNSS_Handle.uartWorkingBuffer[10 + var];
	}
}

/*!
 * Changing the GNSS mode.
 * Look at: 32.10.19 u-blox 8 Receiver description
 */
void GNSS_SetMode(GNSS_StateHandle *GNSS, short gnssMode) {
	if (gnssMode == 0) {
		HAL_UART_Transmit_DMA(GNSS->huart, setPortableMode,sizeof(setPortableMode) / sizeof(uint8_t));
	} else if (gnssMode == 1) {
		HAL_UART_Transmit_DMA(GNSS->huart, setStationaryMode,sizeof(setStationaryMode) / sizeof(uint8_t));
	} else if (gnssMode == 2) {
		HAL_UART_Transmit_DMA(GNSS->huart, setPedestrianMode,sizeof(setPedestrianMode) / sizeof(uint8_t));
	} else if (gnssMode == 3) {
		HAL_UART_Transmit_DMA(GNSS->huart, setAutomotiveMode,sizeof(setAutomotiveMode) / sizeof(uint8_t));
	} else if (gnssMode == 4) {
		HAL_UART_Transmit_DMA(GNSS->huart, setAutomotiveMode,sizeof(setAutomotiveMode) / sizeof(uint8_t));
	} else if (gnssMode == 5) {
		HAL_UART_Transmit_DMA(GNSS->huart, setAirbone1GMode,sizeof(setAirbone1GMode) / sizeof(uint8_t));
	} else if (gnssMode == 6) {
		HAL_UART_Transmit_DMA(GNSS->huart, setAirbone2GMode,sizeof(setAirbone2GMode) / sizeof(uint8_t));
	} else if (gnssMode == 7) {
		HAL_UART_Transmit_DMA(GNSS->huart, setAirbone4GMode,sizeof(setAirbone4GMode) / sizeof(uint8_t));
	} else if (gnssMode == 8) {
		HAL_UART_Transmit_DMA(GNSS->huart, setWirstMode,sizeof(setWirstMode) / sizeof(uint8_t));
	} else if (gnssMode == 9) {
		HAL_UART_Transmit_DMA(GNSS->huart, setBikeMode,sizeof(setBikeMode) / sizeof(uint8_t));
	}
}
/*!
 * Parse data to navigation position velocity time solution standard.
 * Look at: 32.17.15.1 u-blox 8 Receiver description.
 * @param GNSS Pointer to main GNSS structure.
 */
void GNSS_ParsePVTData(GNSS_StateHandle *GNSS) {
	uShort.bytes[0] = GNSS_Handle.uartWorkingBuffer[10];
	GNSS->yearBytes[0]=GNSS_Handle.uartWorkingBuffer[10];
	uShort.bytes[1] = GNSS_Handle.uartWorkingBuffer[11];
	GNSS->yearBytes[1]=GNSS_Handle.uartWorkingBuffer[11];
	GNSS->year = uShort.uShort;
	GNSS->month = GNSS_Handle.uartWorkingBuffer[12];
	GNSS->day = GNSS_Handle.uartWorkingBuffer[13];
	GNSS->hour = GNSS_Handle.uartWorkingBuffer[14];
	GNSS->min = GNSS_Handle.uartWorkingBuffer[15];
	GNSS->sec = GNSS_Handle.uartWorkingBuffer[16];
	GNSS->fixType = GNSS_Handle.uartWorkingBuffer[26];

	for (int var = 0; var < 4; ++var) {
		iLong.bytes[var] = GNSS_Handle.uartWorkingBuffer[var + 30];
		GNSS->lonBytes[var]= GNSS_Handle.uartWorkingBuffer[var + 30];
	}
	GNSS->lon = iLong.iLong;
	GNSS->fLon=(float)iLong.iLong/10000000.0;
	for (int var = 0; var < 4; ++var) {
		iLong.bytes[var] = GNSS_Handle.uartWorkingBuffer[var + 34];
		GNSS->latBytes[var]=GNSS_Handle.uartWorkingBuffer[var + 34];
	}
	GNSS->lat = iLong.iLong;
	GNSS->fLat=(float)iLong.iLong/10000000.0;
	for (int var = 0; var < 4; ++var) {
		iLong.bytes[var] = GNSS_Handle.uartWorkingBuffer[var + 38];
	}
	GNSS->height = iLong.iLong;

	for (int var = 0; var < 4; ++var) {
		iLong.bytes[var] = GNSS_Handle.uartWorkingBuffer[var + 42];
		GNSS->hMSLBytes[var] = GNSS_Handle.uartWorkingBuffer[var + 42];
	}
	GNSS->hMSL = iLong.iLong;

	for (int var = 0; var < 4; ++var) {
		uLong.bytes[var] = GNSS_Handle.uartWorkingBuffer[var + 46];
	}
	GNSS->hAcc = uLong.uLong;

	for (int var = 0; var < 4; ++var) {
		uLong.bytes[var] = GNSS_Handle.uartWorkingBuffer[var + 50];
	}
	GNSS->vAcc = uLong.uLong;

	for (int var = 0; var < 4; ++var) {
		iLong.bytes[var] = GNSS_Handle.uartWorkingBuffer[var + 66];
		GNSS->gSpeedBytes[var] = GNSS_Handle.uartWorkingBuffer[var + 66];
	}
	GNSS->gSpeed = iLong.iLong;

	for (int var = 0; var < 4; ++var) {
		iLong.bytes[var] = GNSS_Handle.uartWorkingBuffer[var + 70];
	}
	GNSS->headMot = iLong.iLong * 1e-5; // Scaling factor 1e-5 per UBX-NAV-PVT spec
}

/*!
 * Parse data to UTC time solution standard.
 * Look at: 32.17.30.1 u-blox 8 Receiver description.
 * @param GNSS Pointer to main GNSS structure.
 */
void GNSS_ParseNavigatorData(GNSS_StateHandle *GNSS) {
	uShort.bytes[0] = GNSS_Handle.uartWorkingBuffer[18];
	uShort.bytes[1] = GNSS_Handle.uartWorkingBuffer[19];
	GNSS->year = uShort.uShort;
	GNSS->month = GNSS_Handle.uartWorkingBuffer[20];
	GNSS->day = GNSS_Handle.uartWorkingBuffer[21];
	GNSS->hour = GNSS_Handle.uartWorkingBuffer[22];
	GNSS->min = GNSS_Handle.uartWorkingBuffer[23];
	GNSS->sec = GNSS_Handle.uartWorkingBuffer[24];
}

/*!
 * Parse data to geodetic position solution standard.
 * Look at: 32.17.14.1 u-blox 8 Receiver description.
 * @param GNSS Pointer to main GNSS structure.
 */
void GNSS_ParsePOSLLHData(GNSS_StateHandle *GNSS) {
	for (int var = 0; var < 4; ++var) {
		iLong.bytes[var] = GNSS_Handle.uartWorkingBuffer[var + 10];
	}
	GNSS->lon = iLong.iLong;
	GNSS->fLon=(float)iLong.iLong/10000000.0;

	for (int var = 0; var < 4; ++var) {
		iLong.bytes[var] = GNSS_Handle.uartWorkingBuffer[var + 14];
	}
	GNSS->lat = iLong.iLong;
	GNSS->fLat=(float)iLong.iLong/10000000.0;

	for (int var = 0; var < 4; ++var) {
		iLong.bytes[var] = GNSS_Handle.uartWorkingBuffer[var + 18];
	}
	GNSS->height = iLong.iLong;

	for (int var = 0; var < 4; ++var) {
		iLong.bytes[var] = GNSS_Handle.uartWorkingBuffer[var + 22];
	}
	GNSS->hMSL = iLong.iLong;

	for (int var = 0; var < 4; ++var) {
		uLong.bytes[var] = GNSS_Handle.uartWorkingBuffer[var + 26];
	}
	GNSS->hAcc = uLong.uLong;

	for (int var = 0; var < 4; ++var) {
		uLong.bytes[var] = GNSS_Handle.uartWorkingBuffer[var + 30];
	}
	GNSS->vAcc = uLong.uLong;
}

/*!
 *  Sends the basic configuration: Activation of the UBX standard, change of NMEA version to 4.10 and turn on of the Galileo system.
 * @param GNSS Pointer to main GNSS structure.
 */
void GNSS_LoadConfig(GNSS_StateHandle *GNSS) {
	HAL_UART_Transmit_DMA(GNSS->huart, configUBX, sizeof(configUBX) / sizeof(uint8_t));
	osDelay(250);
	HAL_UART_Transmit_DMA(GNSS->huart, setNMEA410, sizeof(setNMEA410) / sizeof(uint8_t));
	osDelay(250);
	HAL_UART_Transmit_DMA(GNSS->huart, setGNSS, sizeof(setGNSS) / sizeof(uint8_t));
	osDelay(250);
}

/*!
 *  Creates a checksum based on UBX standard.
 * @param class Class value from UBX doc.1111111
 * @param messageID MessageID value from UBX doc.
 * @param dataLength Data length value from UBX doc.
 * @param payload Just payload.
 * @return  Returns checksum.
 */
uint16_t GNSS_ChecksumTx(uint8_t class, uint8_t messageID, uint16_t dataLength, uint8_t *payload) {
    /* UBX Checksum: 8-Bit Fletcher Algorithm (RFC 1145) */
	uint8_t ck_a, ck_b;

	ck_a = class;
	ck_b = ck_a;

	ck_a += messageID;
	ck_b += ck_a;

	ck_a += (dataLength & 0xFF);
	ck_b += ck_a;

	ck_a += ((dataLength >> 8) & 0xFF);
	ck_b += ck_a;

	for(uint16_t dataPtr=0; dataPtr<dataLength; dataPtr++) {
		ck_a += *(payload + dataPtr);
		ck_b += ck_a;
	}

	return (ck_b << 8) | ck_a;
}

/*!
 *  Creates a checksum based on UBX standard.
 * @param GNSS Pointer to main GNSS structure.
 * @param ck_a Pointer to buffer store checksum A.
 * @param ck_b Pointer to buffer store checksum B.
 * @return  Returns checksum status 0 correct or 1 not correct.
 */
int GNSS_ChecksumRx(GNSS_StateHandle *GNSS, uint8_t *ck_a, uint8_t *ck_b) {
	uint8_t rx_ck_a, rx_ck_b;
	size_t rx_ptr;
	uint16_t length;

	length = GNSS->uartWorkingBuffer[5] << 8;
	length |= GNSS->uartWorkingBuffer[4];
	length += 4;

	rx_ck_a = 0;
	rx_ck_b = 0;

	for(rx_ptr=0; rx_ptr<length; rx_ptr++){
		rx_ck_a += GNSS->uartWorkingBuffer[2 + rx_ptr];
		rx_ck_b += rx_ck_a;
	}

	rx_ptr += 2;

	if(ck_a)
		*ck_a = rx_ck_a;

	if(ck_b)
		*ck_b = rx_ck_b;

	if((GNSS->uartWorkingBuffer[rx_ptr] == rx_ck_a) && (GNSS->uartWorkingBuffer[rx_ptr+1] == rx_ck_b))
		return 0;

	return 1;
}
#endif /* ENABLE_TIME_GNSS */

