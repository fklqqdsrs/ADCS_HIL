#ifndef _CUBE_REG_H
#define _CUBE_REG_H
#include "REG_MAP/BIT_REG.h"
#include "REG_MAP/KMUTNB_ADCS_REG.h"
KMUTNB_ADCS_REG cube_reg[255];
//BIT_REG cube_bit_reg[255];
enum	cube_reg
{
 CAR_Identi=	0x80,
 CAR_BootandRun_Status=	0x81,
 CAR_BootIndex_Status=	0x82,
 CAR_Get_CacheEnabled_state=	0x83,
 CAR_CurrADCSState=	0x84,
 CAR_JPGConverProg=	0x85,
 CAR_Get_SRAM_Scrub_Para=	0x86,
 CAR_CubeACPState=	0x87,
 CAR_GetMTQConfig=	0x88,
 CAR_Get_Wheel_Config=	0x89,
 CAR_Get_RateGyroConfig=	0x8A,
 CAR_Get_CSS_Config=	0x8B,
 CAR_Get_UnixTime=	0x8C,
 CAR_LastLogged_Event=	0x8D,
 CAR_SRAMLatchup_CT=	0x8E,
 CAR_EDAC_Error_CT=	0x8F,
 CAR_Commu_Status=	0x90,
 CAR_Get_UnixTime_SavetoFlash=	0x91,
 CAR_EstAttitudeAngles=	0x92,
 CAR_EstAngRates=	0x93,
 CAR_SatllitePosECI=	0x94,
 CAR_SatelliteVelECI=	0x95,
 CAR_SatellitePosLLH=	0x96,
 CAR_MagneticFieldVector=	0x97,
 CAR_CSSVector=	0x98,
 CAR_FSSVector=	0x99,
 CAR_NadirVector=	0x9A,
 CAR_RateSensorRates=	0x9B,
 CAR_CurrWheelSpeed=	0x9C,
 CAR_CurrMTQCommand=	0x9D,
 CAR_CurrWheelSpeedCommand=	0x9E,
 CAR_IGRFModelMagFieldVector=	0x9F,
 CAR_Modelled_Sun_Vector=	0xA0,
 CAR_EstGyroBias=	0xA1,
 CAR_EstInnovation=	0xA2,
 CAR_QuatErrorVector=	0xA3,
 CAR_QuatCovariance=	0xA4,
 CAR_AngRateCovariance=	0xA5,
 CAR_RawCam2Sensor=	0xA6,
 CAR_RawCam1Sensor=	0xA7,
 CAR_RawCSS1to6=	0xA8,
 CAR_RawCSS7to10=	0xA9,
 CAR_RawMTM=	0xAA,
 CAR_CubeSense1CurrentMea=	0xAB,
 CAR_CubeControlCurrent=	0xAC,
 CAR_WheelCurrents=	0xAD,
 CAR_CubeADCS_Temp=	0xAE,
 CAR_RateSens_Temp=	0xAF,
 CAR_Raw_GPS_Status=	0xB0,
 CAR_Raw_GPS_Time=	0xB1,
 CAR_RawGPSX=	0xB2,
 CAR_RawGPSY=	0xB3,
 CAR_RawGPSZ=	0xB4,
 CAR_ADCS_Exe_time=	0xC4,
 CAR_Get_ADCSPowerCtrl=	0xC5,
 CAR_ADCSMiscCurrents=	0xC6,
 CAR_Get_CmdAttAngle=	0xC7,
 CAR_FineEstAngRates=	0xC9,
 CAR_Get_CubeSensConfig=	0xCB,
 CAR_Get_MTMConfig=	0xCC,
 CAR_Get_SGP4OrbitPara=	0xCF,
 CAR_Get_DetumbCtrl_Para=	0xD0,
 CAR_Get_YWheel_Ctrl_Para=	0xD1,
 CAR_RawRate_Sensor=	0xD8,
 CAR_Get_RW_Ctrl_Para=	0xD9,
 CAR_EstQuaternion=	0xDA,
 CAR_ECEF_Position=	0xDB,
 CAR_ACP_Exe_State=	0xDC,
 CAR_Get_I_Matrix=	0xDE,
 CAR_Get_EstPara=	0xDF,
 CAR_CurrADCSState2=	0xE0,
 CAR_Get_ADCS_Config=	0xE1,
 CAR_Get_Aug_SGP4_Para=	0xE3,
 CAR_GET_ASPG4_TLE=	0xE4,
 CAR_CubeSense2CurrentMea=	0xE8,
 CAR_StatusImgCapSaveOp=	0xE9,
 CAR_SDcardFormat_EraseProgress=	0xEA,
 CAR_Get_SD_Log1_Config=	0xEB,
 CAR_Get_SD_Log2_Config=	0xEC,
 CAR_Get_UART_Log_Config=	0xED,
 CAR_Get_I_PointingRef_Vector=	0xEE,
 CAR_TLC_ACK=	0xF0,
 CAR_FileDLBuff_with_FileContents=	0xF1,
 CAR_DL_Block_Ready=	0xF2,
 CAR_File_Infor=	0xF3,
 CAR_Init_upload_Complete=	0xF4,
 CAR_Upload_Block_Complete=	0xF5,
 CAR_Block_Checksum=	0xF6,
 CAR_Get_HoleMap1=	0xF7,
 CAR_Get_HoleMap2=	0xF8,
 CAR_Get_HoleMap3=	0xF9,
 CAR_Get_HoleMap4=	0xFA,
 CAR_Get_HoleMap5=	0xFB,
 CAR_Get_HoleMap6=	0xFC,
 CAR_Get_HoleMap7=	0xFD,
 CAR_Get_HoleMap8=	0xFE,
 CAR_Reset=	0x1,
 CAR_Set_UnixTime=	0x2,
 CAR_Set_CacheEnabled_state=	0x3,
 CAR_ResetLogPointer=	0x4,
 CAR_AdvanceLogPointer=	0x5,
 CAR_ResetBootReg=	0x6,
 CAR_DeployMTM=	0x7,
 CAR_Set_SRAM_Scrub_Para=	0x8,
 CAR_Set_UnixTime_SavetoFlash=	0x9,
 CAR_ADCSRunMode=	0xA,
 CAR_Set_ADCSPowerCtrl=	0xB,
 CAR_AcpClearErrors=	0xC,
 CAR_AttCtrlMode=	0xD,
 CAR_AttEstMode=	0xE,
 CAR_Set_CmdAttAngle=	0xF,
 CAR_MTQOP=	0x10,
 CAR_WheelSpeed=	0x11,
 CAR_Trigger_ADCSLoop =	0x12,
 CAR_Trigger_ADCSLoopSimSen=	0x13,
 CAR_SetMTQConfig=	0x15,
 CAR_Set_Wheel_Config=	0x16,
 CAR_Set_RateGyroConfig=	0x17,
 CAR_Set_CSS_Config=	0x18,
 CAR_Set_CubeSensConfig=	0x19,
 CAR_Set_MTMConfig=	0x1A,
 CAR_Set_EstPara=	0x1B,
 CAR_Set_Aug_SGP4_Para=	0x1C,
 CAR_Set_ADCS_Config=	0x1E,
 CAR_ASGP4_RunMode=	0x1F,
 CAR_ASGP4_Trigger=	0x20,
 CAR_FormatSDcard=	0x21,
 CAR_Set_I_PointingRef_Vector=	0x22,
 CAR_Set_DetumbCtrl_Para=	0x26,
 CAR_Set_YWheel_Ctrl_Para=	0x27,
 CAR_Set_RW_Ctrl_Para=	0x28,
 CAR_Set_I_Matrix=	0x29,
 CAR_Set_SGP4OrbitPara=	0x2D,
 CAR_Set_Mode_MTMOp=	0x38,
 CAR_ConvertJPG=	0x39,
 CAR_Save_Config=	0x3F,
 CAR_Save_OrbitPara=	0x40,
 CAR_Save_Image =	0x50,
 CAR_SetBootIndex=	0x64,
 CAR_RunSelectedProg=	0x65,
 CAR_ReadProgInfor=	0x66,
 CAR_CopyProg_to_InFlash=	0x67,
 CAR_Set_SD_Log1_Config=	0x68,
 CAR_Set_SD_Log2_Config=	0x69,
 CAR_Set_UART_Log_Config=	0x6A,
 CAR_Erase_File=	0x6C,
 CAR_Load_File_DL_Block=	0x70,
 CAR_AdFile_List_ReadP=	0x71,
 CAR_Init_File_Upload=	0x72,
 CAR_Fille_upload_Packet=	0x73,
 CAR_Final_Upload_Block=	0x74,
 CAR_Reset_Upload_Block=	0x75,
 CAR_ResetFile_List_ReadP=	0x76,
 CAR_Init_Download_Burst=	0x77,
 CAR_Set_HoleMap1=	0x78,
 CAR_Set_HoleMap2=	0x79,
 CAR_Set_HoleMap3=	0x7A,
 CAR_Set_HoleMap4=	0x7B,
 CAR_Set_HoleMap5=	0x7C,
 CAR_Set_HoleMap6=	0x7D,
 CAR_Set_HoleMap7=	0x7E,
 CAR_Set_HoleMap8=	0x7F,
};
uint8_t CAR_Identi_D[8]			 = {0,0,0,0,0,0,0,0};
uint8_t CAR_BootandRun_Status_D[6]			 = {0,0,0,0,0,0};
uint8_t CAR_BootIndex_Status_D[2]			 = {0,0};
uint8_t CAR_Get_CacheEnabled_state_D			 = 0;
uint8_t CAR_CurrADCSState_D[6]			 = {0,0,0,0,0,0};
uint8_t CAR_JPGConverProg_D[3]			 = {0,0,0};
uint16_t CAR_Get_SRAM_Scrub_Para_D			 = 0;
uint8_t CAR_CubeACPState_D			 = 0;
uint8_t CAR_GetMTQConfig_D[3]			 = {0,0,0};
uint8_t CAR_Get_Wheel_Config_D[4]			 = {0,0,0,0};
uint8_t CAR_Get_RateGyroConfig_D[10]			 = {0,0,0,0,0,0,0,0,0,0};
uint8_t CAR_Get_CSS_Config_D[21]			 = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
uint8_t CAR_Get_UnixTime_D[6]			 = {0,0,0,0,0,0};
uint8_t CAR_LastLogged_Event_D[6]			 = {0,0,0,0,0,0};
uint16_t CAR_SRAMLatchup_CT_D[3]			 = {0,0,0};
uint16_t CAR_EDAC_Error_CT_D[3]			 = {0,0,0};
uint8_t CAR_Commu_Status_D[6]			 = {0,0,0,0,0,0};
uint8_t CAR_Get_UnixTime_SavetoFlash_D[2]			 = {0,0};
int16_t CAR_EstAttitudeAngles_D[3]			 = {0,0,0};
int16_t CAR_EstAngRates_D[3]			 = {0,0,0};
int16_t CAR_SatllitePosECI_D[3]			 = {0,0,0};
int16_t CAR_SatelliteVelECI_D[3]			 = {0,0,0};
int16_t CAR_SatellitePosLLH_D[3]			 = {0,0,0};
int16_t CAR_MagneticFieldVector_D[3]			 = {0,0,0};
int16_t CAR_CSSVector_D[3]			 = {0,0,0};
int16_t CAR_FSSVector_D[3]			 = {0,0,0};
int16_t CAR_NadirVector_D[3]			 = {0,0,0};
int16_t CAR_RateSensorRates_D[3]			 = {0,0,0};
int16_t CAR_CurrWheelSpeed_D[3]			 = {0,0,0};
int16_t CAR_CurrMTQCommand_D[3]			 = {0,0,0};
int16_t CAR_CurrWheelSpeedCommand_D[3]			 = {0,0,0};
int16_t CAR_IGRFModelMagFieldVector_D[3]			 = {0,0,0};
int16_t CAR_Modelled_Sun_Vector_D[3]			 = {0,0,0};
int16_t CAR_EstGyroBias_D[3]			 = {0,0,0};
int16_t CAR_EstInnovation_D[3]			 = {0,0,0};
int16_t CAR_QuatErrorVector_D[3]			 = {0,0,0};
int16_t CAR_QuatCovariance_D[3]			 = {0,0,0};
int16_t CAR_AngRateCovariance_D[3]			 = {0,0,0};
uint8_t CAR_RawCam2Sensor_D[6]			 = {0,0,0,0,0,0};
uint8_t CAR_RawCam1Sensor_D[6]			 = {0,0,0,0,0,0};
uint8_t CAR_RawCSS1to6_D[6]			 = {0,0,0,0,0,0};
uint8_t CAR_RawCSS7to10_D[6]			 = {0,0,0,0,0,0};
int16_t CAR_RawMTM_D[3]			 = {0,0,0};
uint16_t CAR_CubeSense1CurrentMea_D[2]			 = {0,0};
int16_t CAR_CubeControlCurrent_D[3]			 = {0,0,0};
int16_t CAR_WheelCurrents_D[3]			 = {0,0,0};
int16_t CAR_CubeADCS_Temp_D[3]			 = {0,0,0};
int16_t CAR_RateSens_Temp_D[3]			 = {0,0,0};
uint8_t CAR_Raw_GPS_Status_D[6]			 = {0,0,0,0,0,0};
uint16_t CAR_Raw_GPS_Time_D[2]			 = {0,0};
uint8_t CAR_RawGPSX_D[6]			 = {0,0,0,0,0,0};
uint8_t CAR_RawGPSY_D[6]			 = {0,0,0,0,0,0};
uint8_t CAR_RawGPSZ_D[6]			 = {0,0,0,0,0,0};
uint16_t CAR_ADCS_Exe_time_D[4]			 = {0,0,0,0};
uint8_t CAR_Get_ADCSPowerCtrl_D[3]			 = {0,0,0};
int16_t CAR_ADCSMiscCurrents_D[3]			 = {0,0,0};
int16_t CAR_Get_CmdAttAngle_D[3]			 = {0,0,0};
int16_t CAR_FineEstAngRates_D[3]			 = {0,0,0};
uint8_t CAR_Get_CubeSensConfig_D[112]			 = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int16_t CAR_Get_MTMConfig_D[15]			 = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
double CAR_Get_SGP4OrbitPara_D[8]			 = {0,0,0,0,0,0,0,0};
uint8_t CAR_Get_DetumbCtrl_Para_D[14]			 = {0,0,0,0,0,0,0,0,0,0,0,0,0,0};
float CAR_Get_YWheel_Ctrl_Para_D[5]			 = {0,0,0,0,0};
int16_t CAR_RawRate_Sensor_D[3]			 = {0,0,0};
uint8_t CAR_Get_RW_Ctrl_Para_D[13]			 = {0,0,0,0,0,0,0,0,0,0,0,0,0};
int16_t CAR_EstQuaternion_D[3]			 = {0,0,0};
int16_t CAR_ECEF_Position_D[3]			 = {0,0,0};
uint8_t CAR_ACP_Exe_State_D[3]			 = {0,0,0};
float CAR_Get_I_Matrix_D[6]			 = {0,0,0,0,0,0};
uint8_t CAR_Get_EstPara_D[31]			 = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
uint8_t CAR_CurrADCSState2_D[6]			 = {0,0,0,0,0,0};
uint8_t CAR_Get_ADCS_Config_D[173]			 = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
uint8_t CAR_Get_Aug_SGP4_Para_D[30]			 = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
uint8_t CAR_GET_ASPG4_TLE_D[33]			 = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
uint16_t CAR_CubeSense2CurrentMea_D[2]			 = {0,0};
uint8_t CAR_StatusImgCapSaveOp_D[2]			 = {0,0};
uint8_t CAR_SDcardFormat_EraseProgress_D[2]			 = {0,0};
uint8_t CAR_Get_SD_Log1_Config_D[13]			 = {0,0,0,0,0,0,0,0,0,0,0,0,0};
uint8_t CAR_Get_SD_Log2_Config_D[13]			 = {0,0,0,0,0,0,0,0,0,0,0,0,0};
uint8_t CAR_Get_UART_Log_Config_D[12]			 = {0,0,0,0,0,0,0,0,0,0,0,0};
int16_t CAR_Get_I_PointingRef_Vector_D[3]			 = {0,0,0};
uint8_t CAR_TLC_ACK_D[4]			 = {0,0,0,0};
uint8_t CAR_FileDLBuff_with_FileContents_D[22]			 = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
uint8_t CAR_DL_Block_Ready_D[5]			 = {0,0,0,0,0};
uint8_t CAR_File_Infor_D[12]			 = {0,0,0,0,0,0,0,0,0,0,0,0};
uint8_t CAR_Init_upload_Complete_D			 = 0;
uint8_t CAR_Upload_Block_Complete_D			 = 0;
uint16_t CAR_Block_Checksum_D			 = 0;
uint8_t CAR_Get_HoleMap1_D[16]			 = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
uint8_t CAR_Get_HoleMap2_D[16]			 = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
uint8_t CAR_Get_HoleMap3_D[16]			 = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
uint8_t CAR_Get_HoleMap4_D[16]			 = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
uint8_t CAR_Get_HoleMap5_D[16]			 = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
uint8_t CAR_Get_HoleMap6_D[16]			 = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
uint8_t CAR_Get_HoleMap7_D[16]			 = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
uint8_t CAR_Get_HoleMap8_D[16]			 = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
uint8_t CAR_Reset_D			 = 0;
uint8_t CAR_Set_UnixTime_D[6]			 = {0,0,0,0,0,0};
uint8_t CAR_Set_CacheEnabled_state_D			 = 0;
uint8_t CAR_ResetLogPointer_D			 = 0;
uint8_t CAR_AdvanceLogPointer_D			 = 0;
uint8_t CAR_ResetBootReg_D			 = 0;
uint8_t CAR_DeployMTM_D			 = 0;
uint16_t CAR_Set_SRAM_Scrub_Para_D			 = 0;
uint8_t CAR_Set_UnixTime_SavetoFlash_D[2]			 = {0,0};
uint8_t CAR_ADCSRunMode_D			 = 0;
uint8_t CAR_Set_ADCSPowerCtrl_D[3]			 = {0,0,0};
uint8_t CAR_AcpClearErrors_D			 = 0;
uint8_t CAR_AttCtrlMode_D[3]			 = {0,0,0};
uint8_t CAR_AttEstMode_D			 = 0;
int16_t CAR_Set_CmdAttAngle_D[3]			 = {0,0,0};
int16_t CAR_MTQOP_D[3]			 = {0,0,0};
int16_t CAR_WheelSpeed_D[3]			 = {0,0,0};
uint8_t CAR_Trigger_ADCSLoop_D			 = 0;
uint8_t CAR_Trigger_ADCSLoopSimSen_D[127]			 = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
uint8_t CAR_SetMTQConfig_D[3]			 = {0,0,0};
uint8_t CAR_Set_Wheel_Config_D[4]			 = {0,0,0,0};
uint8_t CAR_Set_RateGyroConfig_D[10]			 = {0,0,0,0,0,0,0,0,0,0};
uint8_t CAR_Set_CSS_Config_D[21]			 = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
uint8_t CAR_Set_CubeSensConfig_D[112]			 = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int16_t CAR_Set_MTMConfig_D[15]			 = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
uint8_t CAR_Set_EstPara_D[31]			 = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
uint8_t CAR_Set_Aug_SGP4_Para_D[30]			 = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
uint8_t CAR_Set_ADCS_Config_D[173]			 = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
uint8_t CAR_ASGP4_RunMode_D			 = 0;
uint8_t CAR_ASGP4_Trigger_D			 = 0;
uint8_t CAR_FormatSDcard_D			 = 0;
int16_t CAR_Set_I_PointingRef_Vector_D[3]			 = {0,0,0};
uint8_t CAR_Set_DetumbCtrl_Para_D[14]			 = {0,0,0,0,0,0,0,0,0,0,0,0,0,0};
float CAR_Set_YWheel_Ctrl_Para_D[5]			 = {0,0,0,0,0};
uint8_t CAR_Set_RW_Ctrl_Para_D[13]			 = {0,0,0,0,0,0,0,0,0,0,0,0,0};
float CAR_Set_I_Matrix_D[6]			 = {0,0,0,0,0,0};
double CAR_Set_SGP4OrbitPara_D[8]			 = {0,0,0,0,0,0,0,0};
uint8_t CAR_Set_Mode_MTMOp_D			 = 0;
uint8_t CAR_ConvertJPG_D[3]			 = {0,0,0};
uint8_t CAR_Save_Config_D			 = 0;
uint8_t CAR_Save_OrbitPara_D			 = 0;
uint8_t CAR_Save_Image_D[2]			 = {0,0};
uint8_t CAR_SetBootIndex_D			 = 0;
uint8_t CAR_RunSelectedProg_D			 = 0;
uint8_t CAR_ReadProgInfor_D			 = 0;
uint8_t CAR_CopyProg_to_InFlash_D[2]			 = {0,0};
uint8_t CAR_Set_SD_Log1_Config_D[13]			 = {0,0,0,0,0,0,0,0,0,0,0,0,0};
uint8_t CAR_Set_SD_Log2_Config_D[13]			 = {0,0,0,0,0,0,0,0,0,0,0,0,0};
uint8_t CAR_Set_UART_Log_Config_D[12]			 = {0,0,0,0,0,0,0,0,0,0,0,0};
uint8_t CAR_Erase_File_D[3]			 = {0,0,0};
uint8_t CAR_Load_File_DL_Block_D[8]			 = {0,0,0,0,0,0,0,0};
uint8_t CAR_AdFile_List_ReadP_D			 = 0;
uint8_t CAR_Init_File_Upload_D[2]			 = {0,0};
uint8_t CAR_Fille_upload_Packet_D[22]			 = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
uint8_t CAR_Final_Upload_Block_D[7]			 = {0,0,0,0,0,0,0};
uint8_t CAR_Reset_Upload_Block_D			 = 0;
uint8_t CAR_ResetFile_List_ReadP_D			 = 0;
uint8_t CAR_Init_Download_Burst_D[2]			 = {0,0};
uint8_t CAR_Set_HoleMap1_D[16]			 = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
uint8_t CAR_Set_HoleMap2_D[16]			 = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
uint8_t CAR_Set_HoleMap3_D[16]			 = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
uint8_t CAR_Set_HoleMap4_D[16]			 = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
uint8_t CAR_Set_HoleMap5_D[16]			 = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
uint8_t CAR_Set_HoleMap6_D[16]			 = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
uint8_t CAR_Set_HoleMap7_D[16]			 = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
uint8_t CAR_Set_HoleMap8_D[16]			 = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
void cube_reg_setup(){
	cube_reg[CAR_Identi].init((uint8_t*)&CAR_Identi_D,sizeof(CAR_Identi_D),(uint8_t)data_type::_64UL);
	cube_reg[CAR_BootandRun_Status].init((uint8_t*)&CAR_BootandRun_Status_D,sizeof(CAR_BootandRun_Status_D),(uint8_t)data_type::_48UL);
	cube_reg[CAR_BootIndex_Status].init((uint8_t*)&CAR_BootIndex_Status_D,sizeof(CAR_BootIndex_Status_D),(uint8_t)data_type::_16UL);
	cube_reg[CAR_Get_CacheEnabled_state].init((uint8_t*)&CAR_Get_CacheEnabled_state_D,sizeof(CAR_Get_CacheEnabled_state_D),(uint8_t)data_type::_8UL);
	cube_reg[CAR_CurrADCSState].init((uint8_t*)&CAR_CurrADCSState_D,sizeof(CAR_CurrADCSState_D),(uint8_t)data_type::_48UL);
	cube_reg[CAR_JPGConverProg].init((uint8_t*)&CAR_JPGConverProg_D,sizeof(CAR_JPGConverProg_D),(uint8_t)data_type::_24UL);
	cube_reg[CAR_Get_SRAM_Scrub_Para].init((uint8_t*)&CAR_Get_SRAM_Scrub_Para_D,sizeof(CAR_Get_SRAM_Scrub_Para_D),(uint8_t)data_type::_16UL);
	cube_reg[CAR_CubeACPState].init((uint8_t*)&CAR_CubeACPState_D,sizeof(CAR_CubeACPState_D),(uint8_t)data_type::_8UL);
	cube_reg[CAR_GetMTQConfig].init((uint8_t*)&CAR_GetMTQConfig_D,sizeof(CAR_GetMTQConfig_D),(uint8_t)data_type::_24UL);
	cube_reg[CAR_Get_Wheel_Config].init((uint8_t*)&CAR_Get_Wheel_Config_D,sizeof(CAR_Get_Wheel_Config_D),(uint8_t)data_type::_32UL);
	cube_reg[CAR_Get_RateGyroConfig].init((uint8_t*)&CAR_Get_RateGyroConfig_D,sizeof(CAR_Get_RateGyroConfig_D),(uint8_t)data_type::_80UL);
	cube_reg[CAR_Get_CSS_Config].init((uint8_t*)&CAR_Get_CSS_Config_D,sizeof(CAR_Get_CSS_Config_D),(uint8_t)data_type::_168UL);
	cube_reg[CAR_Get_UnixTime].init((uint8_t*)&CAR_Get_UnixTime_D,sizeof(CAR_Get_UnixTime_D),(uint8_t)data_type::_48UL);
	cube_reg[CAR_LastLogged_Event].init((uint8_t*)&CAR_LastLogged_Event_D,sizeof(CAR_LastLogged_Event_D),(uint8_t)data_type::_48UL);
	cube_reg[CAR_SRAMLatchup_CT].init((uint8_t*)&CAR_SRAMLatchup_CT_D,sizeof(CAR_SRAMLatchup_CT_D),(uint8_t)data_type::_48UL);
	cube_reg[CAR_EDAC_Error_CT].init((uint8_t*)&CAR_EDAC_Error_CT_D,sizeof(CAR_EDAC_Error_CT_D),(uint8_t)data_type::_48UL);
	cube_reg[CAR_Commu_Status].init((uint8_t*)&CAR_Commu_Status_D,sizeof(CAR_Commu_Status_D),(uint8_t)data_type::_48UL);
	cube_reg[CAR_Get_UnixTime_SavetoFlash].init((uint8_t*)&CAR_Get_UnixTime_SavetoFlash_D,sizeof(CAR_Get_UnixTime_SavetoFlash_D),(uint8_t)data_type::_16UL);
	cube_reg[CAR_EstAttitudeAngles].init((uint8_t*)&CAR_EstAttitudeAngles_D,sizeof(CAR_EstAttitudeAngles_D),(uint8_t)data_type::_48L);
	cube_reg[CAR_EstAngRates].init((uint8_t*)&CAR_EstAngRates_D,sizeof(CAR_EstAngRates_D),(uint8_t)data_type::_48L);
	cube_reg[CAR_SatllitePosECI].init((uint8_t*)&CAR_SatllitePosECI_D,sizeof(CAR_SatllitePosECI_D),(uint8_t)data_type::_48L);
	cube_reg[CAR_SatelliteVelECI].init((uint8_t*)&CAR_SatelliteVelECI_D,sizeof(CAR_SatelliteVelECI_D),(uint8_t)data_type::_48L);
	cube_reg[CAR_SatellitePosLLH].init((uint8_t*)&CAR_SatellitePosLLH_D,sizeof(CAR_SatellitePosLLH_D),(uint8_t)data_type::_48L);
	cube_reg[CAR_MagneticFieldVector].init((uint8_t*)&CAR_MagneticFieldVector_D,sizeof(CAR_MagneticFieldVector_D),(uint8_t)data_type::_48L);
	cube_reg[CAR_CSSVector].init((uint8_t*)&CAR_CSSVector_D,sizeof(CAR_CSSVector_D),(uint8_t)data_type::_48L);
	cube_reg[CAR_FSSVector].init((uint8_t*)&CAR_FSSVector_D,sizeof(CAR_FSSVector_D),(uint8_t)data_type::_48L);
	cube_reg[CAR_NadirVector].init((uint8_t*)&CAR_NadirVector_D,sizeof(CAR_NadirVector_D),(uint8_t)data_type::_48L);
	cube_reg[CAR_RateSensorRates].init((uint8_t*)&CAR_RateSensorRates_D,sizeof(CAR_RateSensorRates_D),(uint8_t)data_type::_48L);
	cube_reg[CAR_CurrWheelSpeed].init((uint8_t*)&CAR_CurrWheelSpeed_D,sizeof(CAR_CurrWheelSpeed_D),(uint8_t)data_type::_48L);
	cube_reg[CAR_CurrMTQCommand].init((uint8_t*)&CAR_CurrMTQCommand_D,sizeof(CAR_CurrMTQCommand_D),(uint8_t)data_type::_48L);
	cube_reg[CAR_CurrWheelSpeedCommand].init((uint8_t*)&CAR_CurrWheelSpeedCommand_D,sizeof(CAR_CurrWheelSpeedCommand_D),(uint8_t)data_type::_48L);
	cube_reg[CAR_IGRFModelMagFieldVector].init((uint8_t*)&CAR_IGRFModelMagFieldVector_D,sizeof(CAR_IGRFModelMagFieldVector_D),(uint8_t)data_type::_48L);
	cube_reg[CAR_Modelled_Sun_Vector].init((uint8_t*)&CAR_Modelled_Sun_Vector_D,sizeof(CAR_Modelled_Sun_Vector_D),(uint8_t)data_type::_48L);
	cube_reg[CAR_EstGyroBias].init((uint8_t*)&CAR_EstGyroBias_D,sizeof(CAR_EstGyroBias_D),(uint8_t)data_type::_48L);
	cube_reg[CAR_EstInnovation].init((uint8_t*)&CAR_EstInnovation_D,sizeof(CAR_EstInnovation_D),(uint8_t)data_type::_48L);
	cube_reg[CAR_QuatErrorVector].init((uint8_t*)&CAR_QuatErrorVector_D,sizeof(CAR_QuatErrorVector_D),(uint8_t)data_type::_48L);
	cube_reg[CAR_QuatCovariance].init((uint8_t*)&CAR_QuatCovariance_D,sizeof(CAR_QuatCovariance_D),(uint8_t)data_type::_48L);
	cube_reg[CAR_AngRateCovariance].init((uint8_t*)&CAR_AngRateCovariance_D,sizeof(CAR_AngRateCovariance_D),(uint8_t)data_type::_48L);
	cube_reg[CAR_RawCam2Sensor].init((uint8_t*)&CAR_RawCam2Sensor_D,sizeof(CAR_RawCam2Sensor_D),(uint8_t)data_type::_48UL);
	cube_reg[CAR_RawCam1Sensor].init((uint8_t*)&CAR_RawCam1Sensor_D,sizeof(CAR_RawCam1Sensor_D),(uint8_t)data_type::_48UL);
	cube_reg[CAR_RawCSS1to6].init((uint8_t*)&CAR_RawCSS1to6_D,sizeof(CAR_RawCSS1to6_D),(uint8_t)data_type::_48UL);
	cube_reg[CAR_RawCSS7to10].init((uint8_t*)&CAR_RawCSS7to10_D,sizeof(CAR_RawCSS7to10_D),(uint8_t)data_type::_48UL);
	cube_reg[CAR_RawMTM].init((uint8_t*)&CAR_RawMTM_D,sizeof(CAR_RawMTM_D),(uint8_t)data_type::_48L);
	cube_reg[CAR_CubeSense1CurrentMea].init((uint8_t*)&CAR_CubeSense1CurrentMea_D,sizeof(CAR_CubeSense1CurrentMea_D),(uint8_t)data_type::_32UL);
	cube_reg[CAR_CubeControlCurrent].init((uint8_t*)&CAR_CubeControlCurrent_D,sizeof(CAR_CubeControlCurrent_D),(uint8_t)data_type::_48L);
	cube_reg[CAR_WheelCurrents].init((uint8_t*)&CAR_WheelCurrents_D,sizeof(CAR_WheelCurrents_D),(uint8_t)data_type::_48L);
	cube_reg[CAR_CubeADCS_Temp].init((uint8_t*)&CAR_CubeADCS_Temp_D,sizeof(CAR_CubeADCS_Temp_D),(uint8_t)data_type::_48L);
	cube_reg[CAR_RateSens_Temp].init((uint8_t*)&CAR_RateSens_Temp_D,sizeof(CAR_RateSens_Temp_D),(uint8_t)data_type::_48L);
	cube_reg[CAR_Raw_GPS_Status].init((uint8_t*)&CAR_Raw_GPS_Status_D,sizeof(CAR_Raw_GPS_Status_D),(uint8_t)data_type::_48UL);
	cube_reg[CAR_Raw_GPS_Time].init((uint8_t*)&CAR_Raw_GPS_Time_D,sizeof(CAR_Raw_GPS_Time_D),(uint8_t)data_type::_32UL);
	cube_reg[CAR_RawGPSX].init((uint8_t*)&CAR_RawGPSX_D,sizeof(CAR_RawGPSX_D),(uint8_t)data_type::_48UL);
	cube_reg[CAR_RawGPSY].init((uint8_t*)&CAR_RawGPSY_D,sizeof(CAR_RawGPSY_D),(uint8_t)data_type::_48UL);
	cube_reg[CAR_RawGPSZ].init((uint8_t*)&CAR_RawGPSZ_D,sizeof(CAR_RawGPSZ_D),(uint8_t)data_type::_48UL);
	cube_reg[CAR_ADCS_Exe_time].init((uint8_t*)&CAR_ADCS_Exe_time_D,sizeof(CAR_ADCS_Exe_time_D),(uint8_t)data_type::_64UL);
	cube_reg[CAR_Get_ADCSPowerCtrl].init((uint8_t*)&CAR_Get_ADCSPowerCtrl_D,sizeof(CAR_Get_ADCSPowerCtrl_D),(uint8_t)data_type::_24UL);
	cube_reg[CAR_ADCSMiscCurrents].init((uint8_t*)&CAR_ADCSMiscCurrents_D,sizeof(CAR_ADCSMiscCurrents_D),(uint8_t)data_type::_48L);
	cube_reg[CAR_Get_CmdAttAngle].init((uint8_t*)&CAR_Get_CmdAttAngle_D,sizeof(CAR_Get_CmdAttAngle_D),(uint8_t)data_type::_48L);
	cube_reg[CAR_FineEstAngRates].init((uint8_t*)&CAR_FineEstAngRates_D,sizeof(CAR_FineEstAngRates_D),(uint8_t)data_type::_48L);
	cube_reg[CAR_Get_CubeSensConfig].init((uint8_t*)&CAR_Get_CubeSensConfig_D,sizeof(CAR_Get_CubeSensConfig_D),(uint8_t)data_type::_896UL);
	cube_reg[CAR_Get_MTMConfig].init((uint8_t*)&CAR_Get_MTMConfig_D,sizeof(CAR_Get_MTMConfig_D),(uint8_t)data_type::_240L);
	cube_reg[CAR_Get_SGP4OrbitPara].init((uint8_t*)&CAR_Get_SGP4OrbitPara_D,sizeof(CAR_Get_SGP4OrbitPara_D),(uint8_t)data_type::_512D);
	cube_reg[CAR_Get_DetumbCtrl_Para].init((uint8_t*)&CAR_Get_DetumbCtrl_Para_D,sizeof(CAR_Get_DetumbCtrl_Para_D),(uint8_t)data_type::_112UL);
	cube_reg[CAR_Get_YWheel_Ctrl_Para].init((uint8_t*)&CAR_Get_YWheel_Ctrl_Para_D,sizeof(CAR_Get_YWheel_Ctrl_Para_D),(uint8_t)data_type::_160F);
	cube_reg[CAR_RawRate_Sensor].init((uint8_t*)&CAR_RawRate_Sensor_D,sizeof(CAR_RawRate_Sensor_D),(uint8_t)data_type::_48L);
	cube_reg[CAR_Get_RW_Ctrl_Para].init((uint8_t*)&CAR_Get_RW_Ctrl_Para_D,sizeof(CAR_Get_RW_Ctrl_Para_D),(uint8_t)data_type::_104UL);
	cube_reg[CAR_EstQuaternion].init((uint8_t*)&CAR_EstQuaternion_D,sizeof(CAR_EstQuaternion_D),(uint8_t)data_type::_48L);
	cube_reg[CAR_ECEF_Position].init((uint8_t*)&CAR_ECEF_Position_D,sizeof(CAR_ECEF_Position_D),(uint8_t)data_type::_48L);
	cube_reg[CAR_ACP_Exe_State].init((uint8_t*)&CAR_ACP_Exe_State_D,sizeof(CAR_ACP_Exe_State_D),(uint8_t)data_type::_24UL);
	cube_reg[CAR_Get_I_Matrix].init((uint8_t*)&CAR_Get_I_Matrix_D,sizeof(CAR_Get_I_Matrix_D),(uint8_t)data_type::_192F);
	cube_reg[CAR_Get_EstPara].init((uint8_t*)&CAR_Get_EstPara_D,sizeof(CAR_Get_EstPara_D),(uint8_t)data_type::_248UL);
	cube_reg[CAR_CurrADCSState2].init((uint8_t*)&CAR_CurrADCSState2_D,sizeof(CAR_CurrADCSState2_D),(uint8_t)data_type::_48UL);
	cube_reg[CAR_Get_ADCS_Config].init((uint8_t*)&CAR_Get_ADCS_Config_D,sizeof(CAR_Get_ADCS_Config_D),(uint8_t)data_type::_1384UL);
	cube_reg[CAR_Get_Aug_SGP4_Para].init((uint8_t*)&CAR_Get_Aug_SGP4_Para_D,sizeof(CAR_Get_Aug_SGP4_Para_D),(uint8_t)data_type::_240UL);
	cube_reg[CAR_GET_ASPG4_TLE].init((uint8_t*)&CAR_GET_ASPG4_TLE_D,sizeof(CAR_GET_ASPG4_TLE_D),(uint8_t)data_type::_264UL);
	cube_reg[CAR_CubeSense2CurrentMea].init((uint8_t*)&CAR_CubeSense2CurrentMea_D,sizeof(CAR_CubeSense2CurrentMea_D),(uint8_t)data_type::_32UL);
	cube_reg[CAR_StatusImgCapSaveOp].init((uint8_t*)&CAR_StatusImgCapSaveOp_D,sizeof(CAR_StatusImgCapSaveOp_D),(uint8_t)data_type::_16UL);
	cube_reg[CAR_SDcardFormat_EraseProgress].init((uint8_t*)&CAR_SDcardFormat_EraseProgress_D,sizeof(CAR_SDcardFormat_EraseProgress_D),(uint8_t)data_type::_16UL);
	cube_reg[CAR_Get_SD_Log1_Config].init((uint8_t*)&CAR_Get_SD_Log1_Config_D,sizeof(CAR_Get_SD_Log1_Config_D),(uint8_t)data_type::_104UL);
	cube_reg[CAR_Get_SD_Log2_Config].init((uint8_t*)&CAR_Get_SD_Log2_Config_D,sizeof(CAR_Get_SD_Log2_Config_D),(uint8_t)data_type::_104UL);
	cube_reg[CAR_Get_UART_Log_Config].init((uint8_t*)&CAR_Get_UART_Log_Config_D,sizeof(CAR_Get_UART_Log_Config_D),(uint8_t)data_type::_96UL);
	cube_reg[CAR_Get_I_PointingRef_Vector].init((uint8_t*)&CAR_Get_I_PointingRef_Vector_D,sizeof(CAR_Get_I_PointingRef_Vector_D),(uint8_t)data_type::_48L);
	cube_reg[CAR_TLC_ACK].init((uint8_t*)&CAR_TLC_ACK_D,sizeof(CAR_TLC_ACK_D),(uint8_t)data_type::_32UL);
	cube_reg[CAR_FileDLBuff_with_FileContents].init((uint8_t*)&CAR_FileDLBuff_with_FileContents_D,sizeof(CAR_FileDLBuff_with_FileContents_D),(uint8_t)data_type::_176UL);
	cube_reg[CAR_DL_Block_Ready].init((uint8_t*)&CAR_DL_Block_Ready_D,sizeof(CAR_DL_Block_Ready_D),(uint8_t)data_type::_40UL);
	cube_reg[CAR_File_Infor].init((uint8_t*)&CAR_File_Infor_D,sizeof(CAR_File_Infor_D),(uint8_t)data_type::_96UL);
	cube_reg[CAR_Init_upload_Complete].init((uint8_t*)&CAR_Init_upload_Complete_D,sizeof(CAR_Init_upload_Complete_D),(uint8_t)data_type::_8UL);
	cube_reg[CAR_Upload_Block_Complete].init((uint8_t*)&CAR_Upload_Block_Complete_D,sizeof(CAR_Upload_Block_Complete_D),(uint8_t)data_type::_8UL);
	cube_reg[CAR_Block_Checksum].init((uint8_t*)&CAR_Block_Checksum_D,sizeof(CAR_Block_Checksum_D),(uint8_t)data_type::_16UL);
	cube_reg[CAR_Get_HoleMap1].init((uint8_t*)&CAR_Get_HoleMap1_D,sizeof(CAR_Get_HoleMap1_D),(uint8_t)data_type::_128UL);
	cube_reg[CAR_Get_HoleMap2].init((uint8_t*)&CAR_Get_HoleMap2_D,sizeof(CAR_Get_HoleMap2_D),(uint8_t)data_type::_128UL);
	cube_reg[CAR_Get_HoleMap3].init((uint8_t*)&CAR_Get_HoleMap3_D,sizeof(CAR_Get_HoleMap3_D),(uint8_t)data_type::_128UL);
	cube_reg[CAR_Get_HoleMap4].init((uint8_t*)&CAR_Get_HoleMap4_D,sizeof(CAR_Get_HoleMap4_D),(uint8_t)data_type::_128UL);
	cube_reg[CAR_Get_HoleMap5].init((uint8_t*)&CAR_Get_HoleMap5_D,sizeof(CAR_Get_HoleMap5_D),(uint8_t)data_type::_128UL);
	cube_reg[CAR_Get_HoleMap6].init((uint8_t*)&CAR_Get_HoleMap6_D,sizeof(CAR_Get_HoleMap6_D),(uint8_t)data_type::_128UL);
	cube_reg[CAR_Get_HoleMap7].init((uint8_t*)&CAR_Get_HoleMap7_D,sizeof(CAR_Get_HoleMap7_D),(uint8_t)data_type::_128UL);
	cube_reg[CAR_Get_HoleMap8].init((uint8_t*)&CAR_Get_HoleMap8_D,sizeof(CAR_Get_HoleMap8_D),(uint8_t)data_type::_128UL);
	cube_reg[CAR_Reset].init((uint8_t*)&CAR_Reset_D,sizeof(CAR_Reset_D),(uint8_t)data_type::_8UL);
	cube_reg[CAR_Set_UnixTime].init((uint8_t*)&CAR_Set_UnixTime_D,sizeof(CAR_Set_UnixTime_D),(uint8_t)data_type::_48UL);
	cube_reg[CAR_Set_CacheEnabled_state].init((uint8_t*)&CAR_Set_CacheEnabled_state_D,sizeof(CAR_Set_CacheEnabled_state_D),(uint8_t)data_type::_8UL);
	cube_reg[CAR_ResetLogPointer].init((uint8_t*)&CAR_ResetLogPointer_D,sizeof(CAR_ResetLogPointer_D),(uint8_t)data_type::_0UL);
	cube_reg[CAR_AdvanceLogPointer].init((uint8_t*)&CAR_AdvanceLogPointer_D,sizeof(CAR_AdvanceLogPointer_D),(uint8_t)data_type::_0UL);
	cube_reg[CAR_ResetBootReg].init((uint8_t*)&CAR_ResetBootReg_D,sizeof(CAR_ResetBootReg_D),(uint8_t)data_type::_0UL);
	cube_reg[CAR_DeployMTM].init((uint8_t*)&CAR_DeployMTM_D,sizeof(CAR_DeployMTM_D),(uint8_t)data_type::_8UL);
	cube_reg[CAR_Set_SRAM_Scrub_Para].init((uint8_t*)&CAR_Set_SRAM_Scrub_Para_D,sizeof(CAR_Set_SRAM_Scrub_Para_D),(uint8_t)data_type::_16UL);
	cube_reg[CAR_Set_UnixTime_SavetoFlash].init((uint8_t*)&CAR_Set_UnixTime_SavetoFlash_D,sizeof(CAR_Set_UnixTime_SavetoFlash_D),(uint8_t)data_type::_16UL);
	cube_reg[CAR_ADCSRunMode].init((uint8_t*)&CAR_ADCSRunMode_D,sizeof(CAR_ADCSRunMode_D),(uint8_t)data_type::_8UL);
	cube_reg[CAR_Set_ADCSPowerCtrl].init((uint8_t*)&CAR_Set_ADCSPowerCtrl_D,sizeof(CAR_Set_ADCSPowerCtrl_D),(uint8_t)data_type::_24UL);
	cube_reg[CAR_AcpClearErrors].init((uint8_t*)&CAR_AcpClearErrors_D,sizeof(CAR_AcpClearErrors_D),(uint8_t)data_type::_8UL);
	cube_reg[CAR_AttCtrlMode].init((uint8_t*)&CAR_AttCtrlMode_D,sizeof(CAR_AttCtrlMode_D),(uint8_t)data_type::_24UL);
	cube_reg[CAR_AttEstMode].init((uint8_t*)&CAR_AttEstMode_D,sizeof(CAR_AttEstMode_D),(uint8_t)data_type::_8UL);
	cube_reg[CAR_Set_CmdAttAngle].init((uint8_t*)&CAR_Set_CmdAttAngle_D,sizeof(CAR_Set_CmdAttAngle_D),(uint8_t)data_type::_48L);
	cube_reg[CAR_MTQOP].init((uint8_t*)&CAR_MTQOP_D,sizeof(CAR_MTQOP_D),(uint8_t)data_type::_48L);
	cube_reg[CAR_WheelSpeed].init((uint8_t*)&CAR_WheelSpeed_D,sizeof(CAR_WheelSpeed_D),(uint8_t)data_type::_48L);
	cube_reg[CAR_Trigger_ADCSLoop ].init((uint8_t*)&CAR_Trigger_ADCSLoop_D,sizeof(CAR_Trigger_ADCSLoop_D),(uint8_t)data_type::_0UL);
	cube_reg[CAR_Trigger_ADCSLoopSimSen].init((uint8_t*)&CAR_Trigger_ADCSLoopSimSen_D,sizeof(CAR_Trigger_ADCSLoopSimSen_D),(uint8_t)data_type::_1016UL);
	cube_reg[CAR_SetMTQConfig].init((uint8_t*)&CAR_SetMTQConfig_D,sizeof(CAR_SetMTQConfig_D),(uint8_t)data_type::_24UL);
	cube_reg[CAR_Set_Wheel_Config].init((uint8_t*)&CAR_Set_Wheel_Config_D,sizeof(CAR_Set_Wheel_Config_D),(uint8_t)data_type::_32UL);
	cube_reg[CAR_Set_RateGyroConfig].init((uint8_t*)&CAR_Set_RateGyroConfig_D,sizeof(CAR_Set_RateGyroConfig_D),(uint8_t)data_type::_80UL);
	cube_reg[CAR_Set_CSS_Config].init((uint8_t*)&CAR_Set_CSS_Config_D,sizeof(CAR_Set_CSS_Config_D),(uint8_t)data_type::_168UL);
	cube_reg[CAR_Set_CubeSensConfig].init((uint8_t*)&CAR_Set_CubeSensConfig_D,sizeof(CAR_Set_CubeSensConfig_D),(uint8_t)data_type::_896UL);
	cube_reg[CAR_Set_MTMConfig].init((uint8_t*)&CAR_Set_MTMConfig_D,sizeof(CAR_Set_MTMConfig_D),(uint8_t)data_type::_240L);
	cube_reg[CAR_Set_EstPara].init((uint8_t*)&CAR_Set_EstPara_D,sizeof(CAR_Set_EstPara_D),(uint8_t)data_type::_248UL);
	cube_reg[CAR_Set_Aug_SGP4_Para].init((uint8_t*)&CAR_Set_Aug_SGP4_Para_D,sizeof(CAR_Set_Aug_SGP4_Para_D),(uint8_t)data_type::_240UL);
	cube_reg[CAR_Set_ADCS_Config].init((uint8_t*)&CAR_Set_ADCS_Config_D,sizeof(CAR_Set_ADCS_Config_D),(uint8_t)data_type::_1384UL);
	cube_reg[CAR_ASGP4_RunMode].init((uint8_t*)&CAR_ASGP4_RunMode_D,sizeof(CAR_ASGP4_RunMode_D),(uint8_t)data_type::_8UL);
	cube_reg[CAR_ASGP4_Trigger].init((uint8_t*)&CAR_ASGP4_Trigger_D,sizeof(CAR_ASGP4_Trigger_D),(uint8_t)data_type::_0UL);
	cube_reg[CAR_FormatSDcard].init((uint8_t*)&CAR_FormatSDcard_D,sizeof(CAR_FormatSDcard_D),(uint8_t)data_type::_8UL);
	cube_reg[CAR_Set_I_PointingRef_Vector].init((uint8_t*)&CAR_Set_I_PointingRef_Vector_D,sizeof(CAR_Set_I_PointingRef_Vector_D),(uint8_t)data_type::_48L);
	cube_reg[CAR_Set_DetumbCtrl_Para].init((uint8_t*)&CAR_Set_DetumbCtrl_Para_D,sizeof(CAR_Set_DetumbCtrl_Para_D),(uint8_t)data_type::_112UL);
	cube_reg[CAR_Set_YWheel_Ctrl_Para].init((uint8_t*)&CAR_Set_YWheel_Ctrl_Para_D,sizeof(CAR_Set_YWheel_Ctrl_Para_D),(uint8_t)data_type::_160F);
	cube_reg[CAR_Set_RW_Ctrl_Para].init((uint8_t*)&CAR_Set_RW_Ctrl_Para_D,sizeof(CAR_Set_RW_Ctrl_Para_D),(uint8_t)data_type::_104UL);
	cube_reg[CAR_Set_I_Matrix].init((uint8_t*)&CAR_Set_I_Matrix_D,sizeof(CAR_Set_I_Matrix_D),(uint8_t)data_type::_192F);
	cube_reg[CAR_Set_SGP4OrbitPara].init((uint8_t*)&CAR_Set_SGP4OrbitPara_D,sizeof(CAR_Set_SGP4OrbitPara_D),(uint8_t)data_type::_512D);
	cube_reg[CAR_Set_Mode_MTMOp].init((uint8_t*)&CAR_Set_Mode_MTMOp_D,sizeof(CAR_Set_Mode_MTMOp_D),(uint8_t)data_type::_8UL);
	cube_reg[CAR_ConvertJPG].init((uint8_t*)&CAR_ConvertJPG_D,sizeof(CAR_ConvertJPG_D),(uint8_t)data_type::_24UL);
	cube_reg[CAR_Save_Config].init((uint8_t*)&CAR_Save_Config_D,sizeof(CAR_Save_Config_D),(uint8_t)data_type::_0UL);
	cube_reg[CAR_Save_OrbitPara].init((uint8_t*)&CAR_Save_OrbitPara_D,sizeof(CAR_Save_OrbitPara_D),(uint8_t)data_type::_0UL);
	cube_reg[CAR_Save_Image].init((uint8_t*)&CAR_Save_Image_D,sizeof(CAR_Save_Image_D),(uint8_t)data_type::_16UL);
	cube_reg[CAR_SetBootIndex].init((uint8_t*)&CAR_SetBootIndex_D,sizeof(CAR_SetBootIndex_D),(uint8_t)data_type::_8UL);
	cube_reg[CAR_RunSelectedProg].init((uint8_t*)&CAR_RunSelectedProg_D,sizeof(CAR_RunSelectedProg_D),(uint8_t)data_type::_0UL);
	cube_reg[CAR_ReadProgInfor].init((uint8_t*)&CAR_ReadProgInfor_D,sizeof(CAR_ReadProgInfor_D),(uint8_t)data_type::_8UL);
	cube_reg[CAR_CopyProg_to_InFlash].init((uint8_t*)&CAR_CopyProg_to_InFlash_D,sizeof(CAR_CopyProg_to_InFlash_D),(uint8_t)data_type::_16UL);
	cube_reg[CAR_Set_SD_Log1_Config].init((uint8_t*)&CAR_Set_SD_Log1_Config_D,sizeof(CAR_Set_SD_Log1_Config_D),(uint8_t)data_type::_104UL);
	cube_reg[CAR_Set_SD_Log2_Config].init((uint8_t*)&CAR_Set_SD_Log2_Config_D,sizeof(CAR_Set_SD_Log2_Config_D),(uint8_t)data_type::_104UL);
	cube_reg[CAR_Set_UART_Log_Config].init((uint8_t*)&CAR_Set_UART_Log_Config_D,sizeof(CAR_Set_UART_Log_Config_D),(uint8_t)data_type::_96UL);
	cube_reg[CAR_Erase_File].init((uint8_t*)&CAR_Erase_File_D,sizeof(CAR_Erase_File_D),(uint8_t)data_type::_24UL);
	cube_reg[CAR_Load_File_DL_Block].init((uint8_t*)&CAR_Load_File_DL_Block_D,sizeof(CAR_Load_File_DL_Block_D),(uint8_t)data_type::_64UL);
	cube_reg[CAR_AdFile_List_ReadP].init((uint8_t*)&CAR_AdFile_List_ReadP_D,sizeof(CAR_AdFile_List_ReadP_D),(uint8_t)data_type::_0UL);
	cube_reg[CAR_Init_File_Upload].init((uint8_t*)&CAR_Init_File_Upload_D,sizeof(CAR_Init_File_Upload_D),(uint8_t)data_type::_16UL);
	cube_reg[CAR_Fille_upload_Packet].init((uint8_t*)&CAR_Fille_upload_Packet_D,sizeof(CAR_Fille_upload_Packet_D),(uint8_t)data_type::_176UL);
	cube_reg[CAR_Final_Upload_Block].init((uint8_t*)&CAR_Final_Upload_Block_D,sizeof(CAR_Final_Upload_Block_D),(uint8_t)data_type::_56UL);
	cube_reg[CAR_Reset_Upload_Block].init((uint8_t*)&CAR_Reset_Upload_Block_D,sizeof(CAR_Reset_Upload_Block_D),(uint8_t)data_type::_0UL);
	cube_reg[CAR_ResetFile_List_ReadP].init((uint8_t*)&CAR_ResetFile_List_ReadP_D,sizeof(CAR_ResetFile_List_ReadP_D),(uint8_t)data_type::_0UL);
	cube_reg[CAR_Init_Download_Burst].init((uint8_t*)&CAR_Init_Download_Burst_D,sizeof(CAR_Init_Download_Burst_D),(uint8_t)data_type::_16UL);
	cube_reg[CAR_Set_HoleMap1].init((uint8_t*)&CAR_Set_HoleMap1_D,sizeof(CAR_Set_HoleMap1_D),(uint8_t)data_type::_128UL);
	cube_reg[CAR_Set_HoleMap2].init((uint8_t*)&CAR_Set_HoleMap2_D,sizeof(CAR_Set_HoleMap2_D),(uint8_t)data_type::_128UL);
	cube_reg[CAR_Set_HoleMap3].init((uint8_t*)&CAR_Set_HoleMap3_D,sizeof(CAR_Set_HoleMap3_D),(uint8_t)data_type::_128UL);
	cube_reg[CAR_Set_HoleMap4].init((uint8_t*)&CAR_Set_HoleMap4_D,sizeof(CAR_Set_HoleMap4_D),(uint8_t)data_type::_128UL);
	cube_reg[CAR_Set_HoleMap5].init((uint8_t*)&CAR_Set_HoleMap5_D,sizeof(CAR_Set_HoleMap5_D),(uint8_t)data_type::_128UL);
	cube_reg[CAR_Set_HoleMap6].init((uint8_t*)&CAR_Set_HoleMap6_D,sizeof(CAR_Set_HoleMap6_D),(uint8_t)data_type::_128UL);
	cube_reg[CAR_Set_HoleMap7].init((uint8_t*)&CAR_Set_HoleMap7_D,sizeof(CAR_Set_HoleMap7_D),(uint8_t)data_type::_128UL);
	cube_reg[CAR_Set_HoleMap8].init((uint8_t*)&CAR_Set_HoleMap8_D,sizeof(CAR_Set_HoleMap8_D),(uint8_t)data_type::_128UL);

}
#endif // _ADCS_G4_H
