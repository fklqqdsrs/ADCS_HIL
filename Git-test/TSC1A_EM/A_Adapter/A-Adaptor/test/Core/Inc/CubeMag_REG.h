/*
 * CubeMag_REG.h
 *
 *  Created on: Jul 15, 2024
 *      Author: Chinn
 */

#ifndef INC_CUBEMAG_REG_H_
#define INC_CUBEMAG_REG_H_

#include "BIT_REG.h"
#include "KMUTNB_ADCS_REG.h"


KMUTNB_ADCS_REG cubemag_reg[255];

#define SOFT_RESET 55
#define HARD_RESET 66

enum CUBEMAG_REG
{
	CommuStat = 0x87,
	TelecommandAck = 0x8A,
	Reset = 0x01,
	PersistConfig = 0x07,
	PersistConfigDiag = 0x86,
	ErrorLogClear = 0x05,
	ErrorLogEntry = 0x83,
	ErrorLogIndex = 0x82,
	ErrorLogSetting = 0x84,
	BootStat = 0x89,
	CommomErrorCodes = 0x8B,
	Identification = 0x80,
	Version = 0x88,
	SerialNumber = 0x81,
	CurrentUnixTime = 0x85,
	PriMagTemp = 0xC6,
	PriMagMeasurement = 0xC5,
	PriMagMeasurementRaw = 0xBB,
	MagState = 0xBC,
	MagSamplePrimary = 0x43,
	MMCCalibrationParam = 0xB9,
	CubeMagConfig = 0xBA,
	MMCMagConfig = 0xC4,
	PriMagDiag = 0xC7,
	RedundantMagTemp = 0xC2,
	RedundantMagMeasurement = 0xC1,
	RedundantMagMeasurementRaw = 0xBF,
	MagSampleRedundant = 0x41,
	PNICalibrationParam = 0xBD,
	PNIMagConfig = 0xC0,
	DeploymentStat = 0xBE,
	RedundantMagDiag = 0xC3,
	HealthTelemetry = 0xFE,

};

float CUBEMAG_PRIMAGMEASUREMENT_D[3] = {0,0,0};

void cubemag_reg_setup()
{
	cubemag_reg[PriMagMeasurement].init((uint8_t*)CUBEMAG_PRIMAGMEASUREMENT_D,sizeof(CUBEMAG_PRIMAGMEASUREMENT_D),(uint8_t)data_type::_32UL);
}
#endif /* INC_CUBEMAG_REG_H_ */
