/*
 * CubeMag_REG.h
 *
 *  Created on: Jul 15, 2024
 *      Author: Chinn
 */

#ifndef INC_CUBEMAG_REG_H_
#define INC_CUBEMAG_REG_H_

#include "REG_MAP/NEW_BIT_REG.h"
#include "REG_MAP/KMUTNB_ADCS_REG.h"

#include <cubeObc/cubeObc.h>
#include <cubeObc/cubeObc_tctlmCommsMasterSvc.h>
#include "tctlmCubeMagCommon1.h"
#include "tctlmCubeMagControlProgramCommon1.h"
#include "tctlmCubeMagControlProgramDeploy1.h"
#include "tctlmCommonFramework1.h"

KMUTNB_ADCS_REG cubemag_reg[255];


enum CUBEMAG_REG
{
	Reset = 1,
	setCurrentUnixtime = 2,
	setErrorLogIndex = 3,
	setErrorLogEntry = 4,
	ErrorLogClear = 5,
	setErrorLogSettings = 6,
	PersistConfig = 7,
	setCubeMagConfig = 60,
	setMMCMagnetometerConfig = 61,
	setPNICalibrationParameters = 62,
	Deploy = 63,
	setPNIMagnetometerConfig = 64,
	SampleRedundant = 65,
	ArmDeploy = 66,
	SamplePrimary = 67,
	setMMCCalibrationParameters = 68,
	Identification = 128,
	SerialNumber = 129,
	getErrorLogIndex = 130,
	getErrorLogEntry = 131,
	getErrorLogSettings = 132,
	getCurrentUnixtime = 133,
	PersistConfigDiagnostics = 134,
	CommunicationStatus = 135,
	Version = 136,
	BootStatus = 137,
	TelecommandAcknowledge = 138,
	CommonErrorCodes = 139,
	Identification2 = 140,
	getMMCCalibrationParameters = 185,
	getCubeMagConfig = 186,
	PrimaryMagnetometerMeasurementRaw = 187,
	State = 188,
	getPNICalibrationParameters = 189,
	DeploymentStatus = 190,
	RedundantMagnetometerMeasurementRaw = 191,
	getPNIMagnetometerConfig = 192,
	RedundantMagnetometerMeasurement = 193,
	RedundantMagnetometerTemperature = 194,
	RedundantMagnetometerDiagnostics = 195,
	getMMCMagnetometerConfig = 196,
	PrimaryMagnetometerMeasurement = 197,
	PrimaryMagnetometerTemperature = 198,
	PrimaryMagnetometerDiagnostics = 199,
	MagX = 200,
	MagY = 201,
	MagZ = 202,
	HealthTelemetry = 254,


};

typedef struct{

	TctlmCommonFramework1_ResetVal ResetVal;  //1
	TctlmCommonFramework1_UnixTime setUnix; //2
	TctlmCommonFramework1_ErrorLogIndex setErrorlogIndex; //3
	TctlmCommonFramework1_ErrorLogEntry setErrorLogEntry; // 4

	//5
	TctlmCommonFramework1_ErrorLogSettings setErrorLogSettings; // 6
	//7
	TctlmCubeMagControlProgramCommon1_Config Config; //60
	TctlmCubeMagControlProgramCommon1_MmcConfig  MmcConfig; //61

	TctlmCubeMagControlProgramDeploy1_PniCalibration  setMagDeployPniCalibrate; //62
	TctlmCubeMagControlProgramDeploy1_MagicDeploy    setDeploy; //63
	TctlmCubeMagControlProgramDeploy1_PniConfig  MagDeployPniConfig;//64
	TctlmCubeMagControlProgramDeploy1_MagicDeployArm setDeployArm;// 66

	TctlmCubeMagControlProgramCommon1_MmcCalibration MmcCalibrate; //68





	TctlmCommonFramework1_Identification Ident;//128
	TctlmCommonFramework1_SerialNumber SerialNumber;//129

	TctlmCommonFramework1_ErrorLogIndex getErrorlogIndex; //130
	TctlmCommonFramework1_ErrorLogEntry getErrorLogEntry; // 131

	//5
	TctlmCommonFramework1_ErrorLogSettings getErrorLogSettings; //131
	TctlmCommonFramework1_UnixTime getUnix; //133

	TctlmCommonFramework1_ConfigPersistDiagnostics ConfigPersistDiag;
	TctlmCommonFramework1_CommsStatus CommsStat;
	TctlmCommonFramework1_Version Version; //136
	TctlmCommonFramework1_BootStatus BootStat; //137
	TctlmCommonFramework1_TelecommandAcknowledge TlcAck; //138
	TctlmCommonFramework1_CommonErrorCodes CommomErrCodes; //139
	TctlmCommonFramework1_Identification2 Ident2; //140

	TctlmCubeMagControlProgramCommon1_PrimaryMagMeasurementRaw PriMagMeasureRaw;  //187
	TctlmCubeMagControlProgramCommon1_State State; //188

	TctlmCubeMagControlProgramDeploy1_PniCalibration getMagDeployPniCalibrate;//189
	TctlmCubeMagControlProgramDeploy1_DeploymentStatus DeploymentStat;//190
	TctlmCubeMagControlProgramDeploy1_RedMagMeasurementRaw RedMagMeasureRaw;//191;
	TctlmCubeMagControlProgramDeploy1_PniConfig getMagDeployPniConfig;//192;
	TctlmCubeMagControlProgramDeploy1_RedMagMeasurement  RedMagMeasure;//193;
	float RedMagTemp; //194
	TctlmCubeMagControlProgramDeploy1_RedMagDiagnostics RedMagDiag; //195
	TctlmCubeMagControlProgramDeploy1_HealthTlm HelthTlm; //254

	TctlmCubeMagControlProgramCommon1_PrimaryMagMeasurement PriMagMeasure; //197
	float Temp; //198
	TctlmCubeMagControlProgramCommon1_PrimaryMagDiagnostics PriMagDiag; //199
	float Magx;
	float Magy;
	float Magz;

} cubemag_data_t;

cubemag_data_t cubemag_ram_data;

//float CUBEMAG_PRIMAGMEASUREMENT_D[3] = {0,0,0};

void cubemag_reg_setup()
{
//	cubemag_reg[Reset].init((uint8_t*) ,sizeof() );
	cubemag_reg[Reset].init((uint8_t*)&cubemag_ram_data.ResetVal,sizeof(TctlmCommonFramework1_ResetVal));
	cubemag_reg[getCurrentUnixtime].init((uint8_t*)&cubemag_ram_data.setUnix,sizeof(TctlmCommonFramework1_UnixTime));
	cubemag_reg[getErrorLogIndex].init((uint8_t*)&cubemag_ram_data.setErrorlogIndex,sizeof(TctlmCommonFramework1_ErrorLogIndex));
	cubemag_reg[getErrorLogEntry].init((uint8_t*)&cubemag_ram_data.setErrorLogEntry,sizeof(TctlmCommonFramework1_ErrorLogEntry));
//	cubemag_reg[ErrorLogClear].init((uint8_t*)&cubemag_ram_data.,sizeof(TctlmCommonFramework1_ErrorLogEntry));
	cubemag_reg[getErrorLogSettings].init((uint8_t*)&cubemag_ram_data.setErrorLogSettings,sizeof(TctlmCommonFramework1_ErrorLogSettings));
//	cubemag_reg[PersistConfig].init((uint8_t*)&cubemag_ram_data.,sizeof(TctlmCommonFramework1_ErrorLogSettings));
	cubemag_reg[getCubeMagConfig].init((uint8_t*)&cubemag_ram_data.Config,sizeof(TctlmCubeMagControlProgramCommon1_Config));
	cubemag_reg[getMMCMagnetometerConfig].init((uint8_t*)&cubemag_ram_data.MmcConfig,sizeof(TctlmCubeMagControlProgramCommon1_MmcConfig));
	cubemag_reg[getPNICalibrationParameters].init((uint8_t*)&cubemag_ram_data.setMagDeployPniCalibrate,sizeof(TctlmCubeMagControlProgramDeploy1_PniCalibration));
	cubemag_reg[Deploy].init((uint8_t*)&cubemag_ram_data.setDeploy,sizeof(TctlmCubeMagControlProgramDeploy1_MagicDeploy));
	cubemag_reg[getPNIMagnetometerConfig].init((uint8_t*)&cubemag_ram_data.MagDeployPniConfig,sizeof(TctlmCubeMagControlProgramDeploy1_PniConfig));
//	cubemag_reg[SampleRedundant].init((uint8_t*)&cubemag_ram_data.setDeploy,sizeof(TctlmCubeMagControlProgramDeploy1_MagicDeploy));
	cubemag_reg[getMMCCalibrationParameters].init((uint8_t*)&cubemag_ram_data.MmcCalibrate,sizeof(TctlmCubeMagControlProgramCommon1_MmcCalibration));
	cubemag_reg[Identification].init((uint8_t*)&cubemag_ram_data.Ident,sizeof(TctlmCommonFramework1_Identification));
	cubemag_reg[SerialNumber].init((uint8_t*)&cubemag_ram_data.SerialNumber,sizeof(TctlmCommonFramework1_SerialNumber));
	cubemag_reg[setErrorLogIndex].init((uint8_t*)&cubemag_ram_data.getErrorlogIndex,sizeof(TctlmCommonFramework1_ErrorLogIndex));
	cubemag_reg[setErrorLogEntry].init((uint8_t*)&cubemag_ram_data.getErrorLogEntry,sizeof(TctlmCommonFramework1_ErrorLogEntry));
	cubemag_reg[setErrorLogSettings].init((uint8_t*)&cubemag_ram_data.getErrorLogSettings,sizeof(TctlmCommonFramework1_ErrorLogSettings));
	cubemag_reg[setCurrentUnixtime].init((uint8_t*)&cubemag_ram_data.getUnix,sizeof(TctlmCommonFramework1_UnixTime));
	cubemag_reg[PersistConfigDiagnostics].init((uint8_t*)&cubemag_ram_data.ConfigPersistDiag,sizeof(TctlmCommonFramework1_ConfigPersistDiagnostics));
	cubemag_reg[CommunicationStatus].init((uint8_t*)&cubemag_ram_data.CommsStat,sizeof(TctlmCommonFramework1_CommsStatus));
	cubemag_reg[Version].init((uint8_t*)&cubemag_ram_data.Version,sizeof(TctlmCommonFramework1_Version));
	cubemag_reg[BootStatus].init((uint8_t*)&cubemag_ram_data.BootStat,sizeof(TctlmCommonFramework1_BootStatus));
	cubemag_reg[TelecommandAcknowledge].init((uint8_t*)&cubemag_ram_data.TlcAck,sizeof(TctlmCommonFramework1_TelecommandAcknowledge));
	cubemag_reg[CommonErrorCodes].init((uint8_t*)&cubemag_ram_data.CommomErrCodes,sizeof(TctlmCommonFramework1_CommonErrorCodes));
	cubemag_reg[Identification2].init((uint8_t*)&cubemag_ram_data.Ident2,sizeof(TctlmCommonFramework1_Identification2));
//	cubemag_reg[setMMCCalibrationParameters].init((uint8_t*)&cubemag_ram_data.Version,sizeof(TctlmCommonFramework1_Version));
	cubemag_reg[PrimaryMagnetometerMeasurementRaw].init((uint8_t*)&cubemag_ram_data.PriMagMeasureRaw,sizeof(TctlmCubeMagControlProgramCommon1_PrimaryMagMeasurementRaw));
	cubemag_reg[State].init((uint8_t*)&cubemag_ram_data.State,sizeof(TctlmCubeMagControlProgramCommon1_State));
	cubemag_reg[setPNICalibrationParameters].init((uint8_t*)&cubemag_ram_data.getMagDeployPniCalibrate,sizeof(TctlmCubeMagControlProgramDeploy1_PniCalibration));
	cubemag_reg[DeploymentStatus].init((uint8_t*)&cubemag_ram_data.DeploymentStat,sizeof(TctlmCubeMagControlProgramDeploy1_DeploymentStatus));
	cubemag_reg[RedundantMagnetometerMeasurementRaw].init((uint8_t*)&cubemag_ram_data.RedMagMeasureRaw,sizeof(TctlmCubeMagControlProgramDeploy1_RedMagMeasurementRaw));
	cubemag_reg[setPNIMagnetometerConfig].init((uint8_t*)&cubemag_ram_data.getMagDeployPniConfig,sizeof(TctlmCubeMagControlProgramDeploy1_PniConfig));
	cubemag_reg[RedundantMagnetometerMeasurement].init((uint8_t*)&cubemag_ram_data.RedMagMeasure,sizeof(TctlmCubeMagControlProgramDeploy1_RedMagMeasurement));
	cubemag_reg[RedundantMagnetometerTemperature].init((uint8_t*)&cubemag_ram_data.RedMagTemp,sizeof(float));
	cubemag_reg[RedundantMagnetometerDiagnostics].init((uint8_t*)&cubemag_ram_data.RedMagDiag,sizeof(TctlmCubeMagControlProgramDeploy1_RedMagDiagnostics));
	//cubemag_reg[setMMCMagnetometerConfig].init((uint8_t*)&cubemag_ram_data.,sizeof(float));
	cubemag_reg[PrimaryMagnetometerMeasurement].init((uint8_t*)&cubemag_ram_data.PriMagMeasure,sizeof(TctlmCubeMagControlProgramCommon1_PrimaryMagMeasurement));
	cubemag_reg[PrimaryMagnetometerTemperature].init((uint8_t*)&cubemag_ram_data.Temp,sizeof(float));
	cubemag_reg[PrimaryMagnetometerDiagnostics].init((uint8_t*)&cubemag_ram_data.PriMagDiag,sizeof(TctlmCubeMagControlProgramCommon1_PrimaryMagDiagnostics));
	cubemag_reg[HealthTelemetry].init((uint8_t*)&cubemag_ram_data.HelthTlm,sizeof(TctlmCubeMagControlProgramDeploy1_HealthTlm));
	cubemag_reg[MagX].init((uint8_t*)&cubemag_ram_data.Magx,sizeof(float));
	cubemag_reg[MagY].init((uint8_t*)&cubemag_ram_data.Magy,sizeof(float));
	cubemag_reg[MagZ].init((uint8_t*)&cubemag_ram_data.Magz,sizeof(float));

	//cubemag_reg[PriMagMeasurement].init((uint8_t*)CUBEMAG_PRIMAGMEASUREMENT_D,sizeof(CUBEMAG_PRIMAGMEASUREMENT_D),(uint8_t)data_type::_32UL);
}
#endif /* INC_CUBEMAG_REG_H_ */
