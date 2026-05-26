#ifndef _CUBE_BIT_H
#define _CUBE_BIT_H
#include "REG_MAP/BIT_REG.h"
#include "REG_MAP/KMUTNB_ADCS_REG.h"
#include "REG_MAP/CUBE_ADCS/CUBE_REG.h"
BIT_REG cube_bit_reg[935];
char Node_type_note_t[]			 = "Node type\r\n";
char Interface_Version_note_t[]			 = "Interface version\r\n";
char Firmware_vr_Major_note_t[]			 = "Firmware version (Major)\r\n";
char Firmware_vr_Minor_note_t[]			 = "Firmware version (Minor)\r\n";
char Runtime_s_note_t[]			 = "Runtime (seconds)\r\n";
char Runtime_ms_note_t[]			 = "Runtime (milliseconds)\r\n";
char Cause_of_MCU_Reset_note_t[]			 = "Cause of MCU reset.\r\n";
char Boot_Cause_note_t[]			 = "Cause of last reboot.\r\n";
char Boot_Counter_note_t[]			 = "Number of times CubeComputer has booted\r\n";
char Boot_Program_Index_note_t[]			 = "Location of program that is currently running.\r\n";
char Firmware2_vr_Major_note_t[]			 = "Firmware version (Major)\r\n";
char Firmware2_vr_Minor_note_t[]			 = "Firmware version (Minor)\r\n";
char Prog_Index_note_t[]			 = "Program Index\r\n";
char Boot_Status_note_t[]			 = "Boot Status\r\n";
char GET_Cache_Enable_state_note_t[]			 = "Cache Enable state\r\n";
char EstMode_note_t[]			 = "Attitude Estimation Mode\r\n";
char CtrlMode_note_t[]			 = "Control Mode\r\n";
char AdcsRunMode_note_t[]			 = "AdcsRunMode\r\n";
char ASGP4Mode_note_t[]			 = "ASGP4Mode\r\n";
char CubeCtrlSignalEnabled_note_t[]			 = "CubeControl Signal Enabled\r\n";
char CubeCtrlMotorEnabled_note_t[]			 = "CubeControl Motor Enabled\r\n";
char CubeSense1Enabled_note_t[]			 = "CubeSense1 Enabled\r\n";
char CubeSense2Enabled_note_t[]			 = "CubeSense2 Enabled\r\n";
char CubeWheel1Enabled_note_t[]			 = "CubeWheel1 Enabled\r\n";
char CubeWheel2Enabled_note_t[]			 = "CubeWheel2 Enabled\r\n";
char CubeWheel3Enabled_note_t[]			 = "CubeWheel3 Enabled\r\n";
char CubeStarEnabled_note_t[]			 = "CubeStarEnabled\r\n";
char GPSReceiverEnabled_note_t[]			 = "GPS Receiver Enabled\r\n";
char GPSLNAPowerEnabled_note_t[]			 = "GPS LNA Power Enabled\r\n";
char MotorDriverEnabled_note_t[]			 = "Motor Driver Enabled\r\n";
char SunAboveLocalHorizon_note_t[]			 = "Sun is Above Local Horizon\r\n";
char CubeSense1CommuErr_note_t[]			 = "CubeSense1 Communications Error\r\n";
char CubeSense2CommuErr_note_t[]			 = "CubeSense2 Communications Error\r\n";
char CubeCtrlSignalCommuErr_note_t[]			 = "CubeControl Signal Communications Error\r\n";
char CubeCtrlMotorCommuErr_note_t[]			 = "CubeControl Motor Communications Error\r\n";
char CubeWheel1CommuErr_note_t[]			 = "CubeWheel1 Communications Error\r\n";
char CubeWheel2CommuErr_note_t[]			 = "CubeWheel2 Communications Error\r\n";
char CubeWheel3CommuErr_note_t[]			 = "CubeWheel3 Communications Error\r\n";
char CubeStarCommuErr_note_t[]			 = "CubeStar Communications Error\r\n";
char MTMRangeErr_note_t[]			 = "Magnetometer Range Error\r\n";
char Cam1SRAMOVCDetected_note_t[]			 = "Cam1 SRAM Overcurrent Detected\r\n";
char Cam13V3OVCDetected_note_t[]			 = "Cam1 3V3 Overcurrent Detected\r\n";
char Cam1SensorBusyErr_note_t[]			 = "Cam1 Sensor Busy Error\r\n";
char Cam1SensorDetectionErr_note_t[]			 = "Cam1 Sensor Detection Error\r\n";
char SunSensorRangeErr_note_t[]			 = "Sun Sensor Range Error\r\n";
char Cam2SRAMOVCDetected_note_t[]			 = "Cam2 SRAM Overcurrent Detected\r\n";
char Cam23V3OVCDetected_note_t[]			 = "Cam2 3V3 Overcurrent Detected\r\n";
char Cam2SensorBusyErr_note_t[]			 = "Cam2 Sensor Busy Error\r\n";
char Cam2SensorDetectionErr_note_t[]			 = "Cam2 Sensor Detection Error\r\n";
char NadirSensorRangeErr_note_t[]			 = "Nadir Sensor Range Error\r\n";
char RateSensorRangeErr_note_t[]			 = "Rate Sensor Range Error\r\n";
char WheelSpeedRangeErr_note_t[]			 = "Wheel Speed Range Error\r\n";
char CSSErr_note_t[]			 = "Coarse Sun Sensor Error\r\n";
char STMatchErr_note_t[]			 = "StarTracker Match Error\r\n";
char ST_OVCDetected_note_t[]			 = "Star Tracker Overcurrent Detected\r\n";
char Progress_Percentage_note_t[]			 = "Progress %\r\n";
char Conversion_Result_note_t[]			 = "JPG Conversion Result.\r\n";
char Output_File_CT_note_t[]			 = "Output File Counter\r\n";
char GET_SRAM_Scrub_Size_note_t[]			 = "SRAM_Scrub_Size\r\n";
char ADCSConfigLoadErr_note_t[]			 = "ADCS Config Load Error\r\n";
char OrbitParaLoadErr_note_t[]			 = "Orbit Parameter Load Error\r\n";
char SystemConfigLoadErr_note_t[]			 = "System Configuration Load Error\r\n";
char SDcardInitErr_note_t[]			 = "SD card Initialization error\r\n";
char SDcardreadErr_note_t[]			 = "SD card read error\r\n";
char SDcardwriteErr_note_t[]			 = "SD card write error\r\n";
char GET_MTQ1config_note_t[]			 = "MTQ 1 config\r\n";
char GET_MTQ2config_note_t[]			 = "MTQ 2 config\r\n";
char GET_MTQ3config_note_t[]			 = "MTQ 3 config\r\n";
char GET_RW1_Config_note_t[]			 = "Get RW1 Configuration\r\n";
char GET_RW2_Config_note_t[]			 = "Get RW2 Configuration\r\n";
char GET_RW3_Config_note_t[]			 = "Get RW3 Configuration\r\n";
char GET_RW4_Config_note_t[]			 = "Get RW4 Configuration\r\n";
char GET_Gyro1Config_note_t[]			 = "Gyro1 Configuration\r\n";
char GET_Gyro2Config_note_t[]			 = "Gyro2 Configuration\r\n";
char GET_Gyro3Config_note_t[]			 = "Gyro3 Configuration\r\n";
char GET_XRateSensorOffset_note_t[]			 = "X-Rate Sensor Offset\r\n";
char GET_YRateSensorOffset_note_t[]			 = "Y-Rate Sensor Offset\r\n";
char GET_ZRateSensorOffset_note_t[]			 = "Z-Rate Sensor Offset\r\n";
char GET_RateSensorMult_note_t[]			 = "RateSensorMult\r\n";
char GET_CSS1_Config_note_t[]			 = "CSS1 Configuration.\r\n";
char GET_CSS2_Config_note_t[]			 = "CSS2 Configuration.\r\n";
char GET_CSS3_Config_note_t[]			 = "CSS3 Configuration.\r\n";
char GET_CSS4_Config_note_t[]			 = "CSS4 Configuration.\r\n";
char GET_CSS5_Config_note_t[]			 = "CSS5 Configuration.\r\n";
char GET_CSS6_Config_note_t[]			 = "CSS6 Configuration.\r\n";
char GET_CSS7_Config_note_t[]			 = "CSS7 Configuration.\r\n";
char GET_CSS8_Config_note_t[]			 = "CSS8 Configuration.\r\n";
char GET_CSS9_Config_note_t[]			 = "CSS9 Configuration.\r\n";
char GET_CSS10_Config_note_t[]			 = "CSS10 Configuration.\r\n";
char GET_CSS1_Rela_Scale_note_t[]			 = "CSS1 Relative Scaling Factor\r\n";
char GET_CSS2_Rela_Scale_note_t[]			 = "CSS2 Relative Scaling Factor\r\n";
char GET_CSS3_Rela_Scale_note_t[]			 = "CSS3 Relative Scaling Factor\r\n";
char GET_CSS4_Rela_Scale_note_t[]			 = "CSS4 Relative Scaling Factor\r\n";
char GET_CSS5_Rela_Scale_note_t[]			 = "CSS5 Relative Scaling Factor\r\n";
char GET_CSS6_Rela_Scale_note_t[]			 = "CSS6 Relative Scaling Factor\r\n";
char GET_CSS7_Rela_Scale_note_t[]			 = "CSS7 Relative Scaling Factor\r\n";
char GET_CSS8_Rela_Scale_note_t[]			 = "CSS8 Relative Scaling Factor\r\n";
char GET_CSS9_Rela_Scale_note_t[]			 = "CSS9 Relative Scaling Factor\r\n";
char GET_CSS10_Rela_Scale_note_t[]			 = "CSS10 Relative Scaling Factor\r\n";
char GET_CSS_Threshold_note_t[]			 = "Relative Scaling Factor\r\n";
char GET_CurrentUnixTime_note_t[]			 = "Current Unix Time\r\n";
char GET_Milliseconds_note_t[]			 = "Milliseconds\r\n";
char Time_of_Event_note_t[]			 = "Time of Event\r\n";
char Event_ID_note_t[]			 = "Event ID\r\n";
char Event_Parameter_note_t[]			 = "Event Parameter\r\n";
char SRAM1_latchups_note_t[]			 = "The number of SRAM1 latchups detected\r\n";
char SRAM2_latchups_note_t[]			 = "The number of SRAM2 latchups detected\r\n";
char Single_SRAM_upsets_note_t[]			 = "The number of single SRAM upsets (per byte) detected\r\n";
char Double_SRAM_upsets_note_t[]			 = "The number of double SRAM upsets (per byte) detected\r\n";
char Multi_SRAM_upsets_note_t[]			 = "The number of multiple SRAM upsets (per byte) detected\r\n";
char TC_CT_note_t[]			 = "No. of telecommands received\r\n";
char TLM_CT_note_t[]			 = "No. of telemetry requests received\r\n";
char TC_Buff_Overrun_note_t[]			 = "TC buffer was overrun while receiving a telecommand\r\n";
char UART_Protocol_Error_note_t[]			 = "UART protocol error occurred\r\n";
char UART_Incomplete_Msg_note_t[]			 = "UART start-of-msg identifier was received without a preceding end-of-msg\r\n";
char I2C_TLM_Error_note_t[]			 = "Number of data clocked out was more than telemetry package\r\n";
char I2C_TC_Buff_Error_note_t[]			 = "Telecommand sent exceeds buffer size\r\n";
char CAN_TC_Buff_Error_note_t[]			 = "Telecommand sent exceeds buffer size\r\n";
char GET_Save_Now_note_t[]			 = "Save current Unix time to flash memory\r\n";
char GET_Save_On_Update_note_t[]			 = "Save Unix time to flash memory whenever there is a cmd to update the Unix time\r\n";
char GET_Save_Periodic_note_t[]			 = "Save Unix time to flash memory periodically\r\n";
char GET_Period_note_t[]			 = "Interval at which to save Unix time to flash memory.\r\n";
char Roll_note_t[]			 = "Roll\r\n";
char Pitch_note_t[]			 = "Pitch\r\n";
char Yaw_note_t[]			 = "Yaw\r\n";
char EstAngularRateX_note_t[]			 = "Fine Estimated X Angular Rate\r\n";
char EstAngularRateY_note_t[]			 = "Fine Estimated Y Angular Rate\r\n";
char EstAngularRateZ_note_t[]			 = "Fine Estimated Z Angular Rate\r\n";
char ECI_Pos_X_note_t[]			 = "ECI referenced X coordinate\r\n";
char ECI_Pos_Y_note_t[]			 = "ECI referenced Y coordinate\r\n";
char ECI_Pos_Z_note_t[]			 = "ECI referenced Z coordinate\r\n";
char ECI_Velocity_X_note_t[]			 = "ECI referenced X velocity\r\n";
char ECI_Velocity_Y_note_t[]			 = "ECI referenced Y velocity\r\n";
char ECI_Velocity_Z_note_t[]			 = "ECI referenced Z velocity\r\n";
char Latitude_note_t[]			 = "Latitude\r\n";
char Longitude_note_t[]			 = "Longitude\r\n";
char Altitude_note_t[]			 = "Altitude\r\n";
char MagFieldVectorX_note_t[]			 = "Magnetic Field Vector X\r\n";
char MagFieldVectorY_note_t[]			 = "Magnetic Field Vector Y\r\n";
char MagFieldVectorZ_note_t[]			 = "Magnetic Field Vector Z\r\n";
char CSSVectorX_note_t[]			 = "CoarseSun Vector X\r\n";
char CSSVectorY_note_t[]			 = "CoarseSun Vector Y\r\n";
char CSSVectorZ_note_t[]			 = "CoarseSun Vector Z\r\n";
char FSSVectorX_note_t[]			 = "FineSun Vector X\r\n";
char FSSVectorY_note_t[]			 = "FineSun Vector Y\r\n";
char FSSVectorZ_note_t[]			 = "FineSun Vector Z\r\n";
char NadirVectorX_note_t[]			 = "Nadir Vector X\r\n";
char NadirVectorY_note_t[]			 = "Nadir Vector Y\r\n";
char NadirVectorZ_note_t[]			 = "Nadir Vector Z\r\n";
char RateSensorRatesX_note_t[]			 = "Rate Sensor Rates X\r\n";
char RateSensorRatesY_note_t[]			 = "Rate Sensor Rates Y\r\n";
char RateSensorRatesZ_note_t[]			 = "Rate Sensor Rates Z\r\n";
char CurrWheelSpeedX_note_t[]			 = "Current Wheel Speed X\r\n";
char CurrWheelSpeedY_note_t[]			 = "Current Wheel Speed Y\r\n";
char CurrWheelSpeedZ_note_t[]			 = "Current Wheel Speed Z\r\n";
char CurrMTQCommandX_note_t[]			 = "Current MTQ Command X\r\n";
char CurrMTQCommandY_note_t[]			 = "Current MTQ Command Y\r\n";
char CurrMTQCommandZ_note_t[]			 = "Current MTQ Command Z\r\n";
char CurrWheelSpeedCommandX_note_t[]			 = "Current Wheel Speed Command X\r\n";
char CurrWheelSpeedCommandY_note_t[]			 = "Current Wheel Speed Command Y\r\n";
char CurrWheelSpeedCommandZ_note_t[]			 = "Current Wheel Speed Command Z\r\n";
char IGRFVectorX_note_t[]			 = "IGRF Modelled Mag Field Vector X\r\n";
char IGRFVectorY_note_t[]			 = "IGRF Modelled Mag Field VectorY\r\n";
char IGRFVectorZ_note_t[]			 = "IGRF Modelled Mag Field VectorZ\r\n";
char Modelled_Sun_VectorX_note_t[]			 = "Modelled Sun Vector X\r\n";
char Modelled_Sun_VectorY_note_t[]			 = "Modelled Sun Vector Y\r\n";
char Modelled_Sun_VectorZ_note_t[]			 = "Modelled Sun Vector Z\r\n";
char EstGyroBiasX_note_t[]			 = "Estimated Gyro Bias X\r\n";
char EstGyroBiasY_note_t[]			 = "Estimated Gyro Bias Y\r\n";
char EstGyroBiasZ_note_t[]			 = "Estimated Gyro Bias Z\r\n";
char EstInnovationX_note_t[]			 = "Estimation Innovation X\r\n";
char EstInnovationY_note_t[]			 = "Estimation Innovation Y\r\n";
char EstInnovationZ_note_t[]			 = "Estimation Innovation Z\r\n";
char Q_Err1_note_t[]			 = "Quaternion Error Vector Q1\r\n";
char Q_Err2_note_t[]			 = "Quaternion Error Vector Q2\r\n";
char Q_Err3_note_t[]			 = "Quaternion Error Vector Q3\r\n";
char Q1_Covariance_note_t[]			 = "Quaternion Covariance � Q1 RMS\r\n";
char Q2_Covariance_note_t[]			 = "Quaternion Covariance � Q2 RMS\r\n";
char Q3_Covariance_note_t[]			 = "Quaternion Covariance � Q3 RMS\r\n";
char Angular_Rate_Covar_X_note_t[]			 = "X Angular Rate Covariance\r\n";
char Angular_Rate_Covar_Y_note_t[]			 = "Y Angular Rate Covariance\r\n";
char Angular_Rate_Covar_Z_note_t[]			 = "Z Angular Rate Covariance\r\n";
char Cam2centroidX_note_t[]			 = "Cam2 centroid X\r\n";
char Cam2centroidY_note_t[]			 = "Cam2 centroid Y\r\n";
char Cam2CapStatus_note_t[]			 = "Cam2 Capture status\r\n";
char Cam2DetectResult_note_t[]			 = "Cam2 Detection result\r\n";
char Cam1centroidX_note_t[]			 = "Cam1 centroid X\r\n";
char Cam1centroidY_note_t[]			 = "Cam1 centroid Y\r\n";
char Cam1CapStatus_note_t[]			 = "Cam1 Capture status\r\n";
char Cam1DetectResult_note_t[]			 = "Cam1 Detection result\r\n";
char RAW_CSS1_note_t[]			 = "CSS1\r\n";
char RAW_CSS2_note_t[]			 = "CSS2\r\n";
char RAW_CSS3_note_t[]			 = "CSS3\r\n";
char RAW_CSS4_note_t[]			 = "CSS4\r\n";
char RAW_CSS5_note_t[]			 = "CSS5\r\n";
char RAW_CSS6_note_t[]			 = "CSS6\r\n";
char RAW_CSS7_note_t[]			 = "CSS7\r\n";
char RAW_CSS8_note_t[]			 = "CSS8\r\n";
char RAW_CSS9_note_t[]			 = "CSS9\r\n";
char RAW_CSS10_note_t[]			 = "CSS10\r\n";
char RAW_Mag_X_note_t[]			 = "RAW Mag X\r\n";
char RAW_Mag_Y_note_t[]			 = "RAW Mag Y\r\n";
char RAW_Mag_Z_note_t[]			 = "RAW Mag Z\r\n";
char CS1_3V3_Current_note_t[]			 = "CubeSense1 3V3 Current.\r\n";
char CS1_SRAM_Current_note_t[]			 = "CubeSense1 Cam SRAM Current.\r\n";
char CubeCtrl3V3Current_note_t[]			 = "CubeControl 3V3 Current\r\n";
char CubeCtrl5Current_note_t[]			 = "CubeControl 5 Current\r\n";
char CubeCtrlVbatCurrent_note_t[]			 = "CubeControl Vbat Current\r\n";
char Wheel1Current_note_t[]			 = "Wheel1 Currents\r\n";
char Wheel2Current_note_t[]			 = "Wheel2 Currents\r\n";
char Wheel3Current_note_t[]			 = "Wheel3 Currents\r\n";
char MCU_Temp_note_t[]			 = "MCU Temperature\r\n";
char MTM_Temp_note_t[]			 = "Magnetometer Temperature\r\n";
char RedundMTM_Temp_note_t[]			 = "Redundant Magnetometer Temperature.\r\n";
char RateSensor_TempX_note_t[]			 = "X-Rate sensor Temperature\r\n";
char RateSensor_TempY_note_t[]			 = "Y-Rate sensor Temperature\r\n";
char RateSensor_TempZ_note_t[]			 = "Z-Rate sensor Temperature\r\n";
char GPS_Solution_Status_note_t[]			 = "Gps Solution Status\r\n";
char Num_of_tracked_GPS_Sat_note_t[]			 = "Number of tracked GPS satellites\r\n";
char NumofGPSSat_UsedIn_Solution_note_t[]			 = "Number of GPS satellites used in solution\r\n";
char CTforXYZLof_from_GPS_note_t[]			 = "Counter for XYZ Lof from GPS\r\n";
char CTforRANGElog_from_GPS_note_t[]			 = "Counter for RANGE log from GPS\r\n";
char Resp_Msg_for_GPSLogSetup_note_t[]			 = "Response Message for GPS log setup\r\n";
char GPS_Ref_Week_note_t[]			 = "GPS Reference Week\r\n";
char GPS_Time_Millisec_note_t[]			 = "GPS Time Milliseconds\r\n";
char GPS_ECEF_PosX_note_t[]			 = "GPS ECEF Position X\r\n";
char GPS_ECEF_VeloX_note_t[]			 = "GPS ECEF Velocity X\r\n";
char GPS_ECEF_PosY_note_t[]			 = "GPS ECEF Position Y\r\n";
char GPS_ECEF_VeloY_note_t[]			 = "GPS ECEF Velocity Y\r\n";
char GPS_ECEF_PosZ_note_t[]			 = "GPS ECEF Position Z\r\n";
char GPS_ECEF_VeloZ_note_t[]			 = "GPS ECEF Velocity Z\r\n";
char TimetoPerform_ADCSUpdate_note_t[]			 = "Time to perform complete ADCS Update function.\r\n";
char TimetoPerform_SA_Commu_note_t[]			 = "Time to perform Sensor/actuator communications.\r\n";
char TimetoExecute_SGP4_Propagator_note_t[]			 = "Time to execute SGP4 propagator.\r\n";
char TimetoExecute_IGRF_Model_note_t[]			 = "Time to execute IGRF computation.\r\n";
char GET_CubeCtrlSignalPowerSelect_note_t[]			 = "CubeControl Signal Power Selection\r\n";
char GET_CubeCtrlMotorPowerSelect_note_t[]			 = "CubeControl Motor Power Selection\r\n";
char GET_CubeSense1PowerSelect_note_t[]			 = "CubeSense1 Power Selection\r\n";
char GET_CubeSense2PowerSelect_note_t[]			 = "CubeSense2 Power Selection\r\n";
char GET_CubeStarPowerSelect_note_t[]			 = "CubeStarPower Power Selection\r\n";
char GET_CubeWheel1PowerSelect_note_t[]			 = "CubeWheel1Power Power Selection\r\n";
char GET_CubeWheel2PowerSelect_note_t[]			 = "CubeWheel2Power Power Selection\r\n";
char GET_CubeWheel3PowerSelect_note_t[]			 = "CubeWheel3Power Power Selection\r\n";
char GET_MotorPower_note_t[]			 = "Motor Power\r\n";
char GET_GPSPower_note_t[]			 = "GPS Power\r\n";
char CubeStarCurrent_note_t[]			 = "CubeStar Current\r\n";
char MTQCurrent_note_t[]			 = "Magnetorquer Current\r\n";
char CubeStarMCU_Temp_note_t[]			 = "CubeStar MCU temperature\r\n";
char GET_Cmd_Roll_note_t[]			 = "Commanded roll angle\r\n";
char GET_Cmd_Pitch_note_t[]			 = "Commanded pitch angle.\r\n";
char GET_Cmd_Yaw_note_t[]			 = "Commanded yaw angle\r\n";
char FineEstXAngularRate_note_t[]			 = "Estimated X Angular Rate\r\n";
char FineEstYAngularRate_note_t[]			 = "Estimated Y Angular Rate\r\n";
char FineEstZAngularRate_note_t[]			 = "Estimated Z Angular Rate\r\n";
char GET_Cam1_Mounting_Trans_Alpha_note_t[]			 = "Cam1 Sensor Mounting Transform Alpha Angle\r\n";
char GET_Cam1_Mounting_Trans_Beta_note_t[]			 = "Cam1 Sensor Mounting Transform Beta Angle\r\n";
char GET_Cam1_Mounting_Trans_Gamma_note_t[]			 = "Cam1 Sensor Mounting Transform Gamma Angle\r\n";
char GET_Cam1_DetectThreshold_note_t[]			 = "Cam1 detection threshold\r\n";
char GET_Cam1_AutoAdjust_note_t[]			 = "Cam1 sensor auto adjust mode\r\n";
char GET_Cam1_Exposure_Time_note_t[]			 = "Cam1 sensor exposure time\r\n";
char GET_Cam1_Boresight_X_note_t[]			 = "X Pixel location of Cam1 boresight.\r\n";
char GET_Cam1_Boresight_Y_note_t[]			 = "Y Pixel location of Cam1 boresight\r\n";
char GET_Cam2_Mounting_Trans_Alpha_note_t[]			 = "Cam2 Sensor Mounting Transform Alpha Angle\r\n";
char GET_Cam2_Mounting_Trans_Beta_note_t[]			 = "Cam2 Sensor Mounting Transform Beta Angle\r\n";
char GET_Cam2_Mounting_Trans_Gamma_note_t[]			 = "Cam2 Sensor Mounting Transform Gamma Angle\r\n";
char GET_Cam2_DetectThreshold_note_t[]			 = "Cam2 detection threshold\r\n";
char GET_Cam2_AutoAdjust_note_t[]			 = "Cam2 sensor auto adjust mode\r\n";
char GET_Cam2_Exposure_Time_note_t[]			 = "Cam2 sensor exposure time\r\n";
char GET_Cam2_Boresight_X_note_t[]			 = "X Pixel location of Cam2 boresight.\r\n";
char GET_Cam2_Boresight_Y_note_t[]			 = "Y Pixel location of Cam2 boresight\r\n";
char GET_Nadir_Max_Devia_Percent_note_t[]			 = "Percentage of measured angular radius as edge�s maximum allowable deviation\r\n";
char GET_Nadir_Max_BadEdges_note_t[]			 = "Maximum amount of edges allowed outside maximum deviation\r\n";
char GET_Nadir_Max_Radius_note_t[]			 = "Nadir Max Radius\r\n";
char GET_Nadir_Min_Radius_note_t[]			 = "Nadir Min Radius\r\n";
char GET_Cam1_MinX_Area1_note_t[]			 = "Cam 1 Minimum X of area 1\r\n";
char GET_Cam1_MaxX_Area1_note_t[]			 = "Cam 1 Maximum X of area 1\r\n";
char GET_Cam1_MinY_Area1_note_t[]			 = "Cam 1 Minimum Y of area 1\r\n";
char GET_Cam1_MaxY_Area1_note_t[]			 = "Cam 1 Maximum Y of area 1\r\n";
char GET_Cam1_MinX_Area2_note_t[]			 = "Cam 1 Minimum X of area 2\r\n";
char GET_Cam1_MaxX_Area2_note_t[]			 = "Cam 1 Maximum X of area 2\r\n";
char GET_Cam1_MinY_Area2_note_t[]			 = "Cam 1 Minimum Y of area 2\r\n";
char GET_Cam1_MaxY_Area2_note_t[]			 = "Cam 1 Maximum Y of area 2\r\n";
char GET_Cam1_MinX_Area3_note_t[]			 = "Cam 1 Minimum X of area 3\r\n";
char GET_Cam1_MaxX_Area3_note_t[]			 = "Cam 1 Maximum X of area 3\r\n";
char GET_Cam1_MinY_Area3_note_t[]			 = "Cam 1 Minimum Y of area 3\r\n";
char GET_Cam1_MaxY_Area3_note_t[]			 = "Cam 1 Maximum Y of area 3\r\n";
char GET_Cam1_MinX_Area4_note_t[]			 = "Cam 1 Minimum X of area 4\r\n";
char GET_Cam1_MaxX_Area4_note_t[]			 = "Cam 1 Maximum X of area 4\r\n";
char GET_Cam1_MinY_Area4_note_t[]			 = "Cam 1 Minimum Y of area 4\r\n";
char GET_Cam1_MaxY_Area4_note_t[]			 = "Cam 1 Maximum Y of area 4\r\n";
char GET_Cam1_MinX_Area5_note_t[]			 = "Cam 1 Minimum X of area 5\r\n";
char GET_Cam1_MaxX_Area5_note_t[]			 = "Cam 1 Maximum X of area 5\r\n";
char GET_Cam1_MinY_Area5_note_t[]			 = "Cam 1 Minimum Y of area 5\r\n";
char GET_Cam1_MaxY_Area5_note_t[]			 = "Cam 1 Maximum Y of area 5\r\n";
char GET_Cam2_MinX_Area1_note_t[]			 = "Cam 2 Minimum X of area 1\r\n";
char GET_Cam2_MaxX_Area1_note_t[]			 = "Cam 2 Maximum X of area 1\r\n";
char GET_Cam2_MinY_Area1_note_t[]			 = "Cam 2 Minimum Y of area 1\r\n";
char GET_Cam2_MaxY_Area1_note_t[]			 = "Cam 2 Maximum Y of area 1\r\n";
char GET_Cam2_MinX_Area2_note_t[]			 = "Cam 2 Minimum X of area 2\r\n";
char GET_Cam2_MaxX_Area2_note_t[]			 = "Cam 2 Maximum X of area 2\r\n";
char GET_Cam2_MinY_Area2_note_t[]			 = "Cam 2 Minimum Y of area 2\r\n";
char GET_Cam2_MaxY_Area2_note_t[]			 = "Cam 2 Maximum Y of area 2\r\n";
char GET_Cam2_MinX_Area3_note_t[]			 = "Cam 2 Minimum X of area 3\r\n";
char GET_Cam2_MaxX_Area3_note_t[]			 = "Cam 2 Maximum X of area 3\r\n";
char GET_Cam2_MinY_Area3_note_t[]			 = "Cam 2 Minimum Y of area 3\r\n";
char GET_Cam2_MaxY_Area3_note_t[]			 = "Cam 2 Maximum Y of area 3\r\n";
char GET_Cam2_MinX_Area4_note_t[]			 = "Cam 2 Minimum X of area 4\r\n";
char GET_Cam2_MaxX_Area4_note_t[]			 = "Cam 2 Maximum X of area 4\r\n";
char GET_Cam2_MinY_Area4_note_t[]			 = "Cam 2 Minimum Y of area 4\r\n";
char GET_Cam2_MaxY_Area4_note_t[]			 = "Cam 2 Maximum Y of area 4\r\n";
char GET_Cam2_MinX_Area5_note_t[]			 = "Cam 2 Minimum X of area 5\r\n";
char GET_Cam2_MaxX_Area5_note_t[]			 = "Cam 2 Maximum X of area 5\r\n";
char GET_Cam2_MinY_Area5_note_t[]			 = "Cam 2 Minimum Y of area 5\r\n";
char GET_Cam2_MaxY_Area5_note_t[]			 = "Cam 2 Maximum Y of area 5\r\n";
char GET_MTMMountAlpha_note_t[]			 = "Magnetometer Mounting Transform Alpha Angle\r\n";
char GET_MTMMountBeta_note_t[]			 = "Magnetometer Mounting Transform Beta Angle\r\n";
char GET_MTMMountGamma_note_t[]			 = "Magnetometer Mounting Transform Gamma Angle\r\n";
char GET_MTM_Ch1Offset_note_t[]			 = "Magnetometer Channel 1 Offset\r\n";
char GET_MTM_Ch2Offset_note_t[]			 = "Magnetometer Channel 2 Offset\r\n";
char GET_MTM_Ch3Offset_note_t[]			 = "Magnetometer Channel 3 Offset\r\n";
char GET_MTM_S11_note_t[]			 = "Magnetometer Sensitivity Matrix S11\r\n";
char GET_MTM_S22_note_t[]			 = "Magnetometer Sensitivity Matrix S22\r\n";
char GET_MTM_S33_note_t[]			 = "Magnetometer Sensitivity Matrix S33\r\n";
char GET_MTM_S12_note_t[]			 = "Magnetometer Sensitivity Matrix S12\r\n";
char GET_MTM_S13_note_t[]			 = "Magnetometer Sensitivity Matrix S13\r\n";
char GET_MTM_S21_note_t[]			 = "Magnetometer Sensitivity Matrix S21\r\n";
char GET_MTM_S23_note_t[]			 = "Magnetometer Sensitivity Matrix S23\r\n";
char GET_MTM_S31_note_t[]			 = "Magnetometer Sensitivity Matrix S31\r\n";
char GET_MTM_S32_note_t[]			 = "Magnetometer Sensitivity Matrix S32\r\n";
char GET_Inclination_note_t[]			 = "Inclination\r\n";
char GET_Eccentricity_note_t[]			 = "Eccentricity\r\n";
char GET_RightAscensionOfTheAscendingNode_note_t[]			 = "Right-ascension of the Ascending Node\r\n";
char GET_ArgumentOfPerigee_note_t[]			 = "Argument of Perigee\r\n";
char GET_BStarDragTerm_note_t[]			 = "B-Star drag term\r\n";
char GET_MeanMotion_note_t[]			 = "Mean Motion\r\n";
char GET_MeanAnomaly_note_t[]			 = "Mean Anomaly\r\n";
char GET_Epoch_note_t[]			 = "Epoch\r\n";
char GET_Detumb_SpinGain_note_t[]			 = "Detumbling Spin Gain (Ks)\r\n";
char GET_Detumb_DampGain_note_t[]			 = "Detumbling Damping Gain (Kd)\r\n";
char GET_Ref_SpinRate_note_t[]			 = "Reference spin rate (wy-ref)\r\n";
char GET_FastBdot_DetumbGain_note_t[]			 = "Fast Bdot Detumbling Gain (Kdf)\r\n";
char GET_Ymoment_CtrlGain_note_t[]			 = "Y-Momentum Control Gain (Kh)\r\n";
char GET_Ymoment_NDampGain_note_t[]			 = "Y-momentum Nutation Damping Gain (Kn)\r\n";
char GET_Ymoment_PGain_note_t[]			 = "Y-momentum Proportional Gain (Kp1)\r\n";
char GET_Ymoment_DGain_note_t[]			 = "Y-momentum Derivative Gain (Kd1)\r\n";
char GET_RefWheel_Moment_note_t[]			 = "Reference Wheel Momentum (H-ref)\r\n";
char Raw_RateX_note_t[]			 = "Raw RateX A/D value\r\n";
char Raw_RateY_note_t[]			 = "Raw RateY A/D value\r\n";
char Raw_RateZ_note_t[]			 = "Raw RateZ A/D value\r\n";
char GET_Rwheel_PGain_note_t[]			 = "Rwheel Proportional Gain (Kp2)\r\n";
char GET_Rwheel_DGain_note_t[]			 = "Rwheel Derivative Gain (Kd2)\r\n";
char GET_YWheelBiasMoment_note_t[]			 = "Y-Wheel Bias Momentum (Hbias)\r\n";
char GET_SunPointFacet_note_t[]			 = "Satellite body axis that will align with sun vector\r\n";
char GET_AutoCtrl_TransitDueWheelErr_note_t[]			 = "Enable/disable automatic transition from wheelctrl modes to Y-Thomson mode in case of wheel error\r\n";
char Est_Q1_note_t[]			 = "Estimated Quaternion Q1\r\n";
char Est_Q2_note_t[]			 = "Estimated Quaternion Q2\r\n";
char Est_Q3_note_t[]			 = "Estimated Quaternion Q3\r\n";
char ECEF_Pos_X_note_t[]			 = "ECEF PositionX\r\n";
char ECEF_Pos_Y_note_t[]			 = "ECEF PositionY\r\n";
char ECEF_Pos_Z_note_t[]			 = "ECEF PositionZ\r\n";
char TimeSince_IterationStart_note_t[]			 = "Time since the start of the current loop iteration\r\n";
char Current_ExePoint_note_t[]			 = "Indicates which part of the loop is currently executing\r\n";
char GET_M_Of_IXX_note_t[]			 = "Moment Of Inertia � Ixx\r\n";
char GET_M_Of_IYY_note_t[]			 = "Moment Of Inertia � Iyy\r\n";
char GET_M_Of_IZZ_note_t[]			 = "Moment Of Inertia � Izz\r\n";
char GET_M_Of_IXY_note_t[]			 = "Product Of Inertia � Ixy\r\n";
char GET_M_Of_IXZ_note_t[]			 = "Product Of Inertia � Ixz\r\n";
char GET_M_Of_IYZ_note_t[]			 = "Product Of Inertia � Iyz\r\n";
char GET_MTMRateFilterSysNoise_note_t[]			 = "Magnetometer Rate Filter System Noise\r\n";
char GET_EKFSysNoise_note_t[]			 = "EKF System Noise\r\n";
char GET_CSSMeasureNoise_note_t[]			 = "CSS Measurement Noise\r\n";
char GET_FSSMeasureNoise_note_t[]			 = "Sun Sensor Measurement Noise\r\n";
char GET_NadirSensMeasureNoise_note_t[]			 = "Nadir Sensor Measurement Noise\r\n";
char GET_MTMMeasureNoise_note_t[]			 = "Magnetometer Measurement Noise\r\n";
char GET_STMeasureNoise_note_t[]			 = "Star Tracker Measurement Noise\r\n";
char GET_UseFSS_note_t[]			 = "Use Sun Sensor\r\n";
char GET_UseNadirSens_note_t[]			 = "Use Nadir Sensor\r\n";
char GET_UseCSS_note_t[]			 = "Use CSS\r\n";
char GET_UseST_note_t[]			 = "Use Star Tracker\r\n";
char GET_NadirSensterminatortest_note_t[]			 = "Nadir sensor terminator test\r\n";
char GET_AutoMTMRecovery_note_t[]			 = "Automatic Magnetometer Recovery\r\n";
char GET_MTMMode_note_t[]			 = "Magnetometer Mode\r\n";
char GET_MTMSelectForRAW_MTM_TLM_note_t[]			 = "Magnetometer Selection for RAW MTM TLM\r\n";
char GET_AutoEstTransitDueToRateSensErr_note_t[]			 = "Automatic Estimation Transition due to Rate Sensor Errors\r\n";
char GET_Cam1Cam2SampPeriod_note_t[]			 = "Cam1 and Cam2 Sampling Period\r\n";
char OrbitPara_Are_Invalid_note_t[]			 = "Orbit Parameters are not in allowed bounds\r\n";
char Config_Is_Invalid_note_t[]			 = "Magnetorquer Configuration or CSS in invalid\r\n";
char CtrlMode_ChangeisntAllowed_note_t[]			 = "Attempt was made to select control mode without appropriate estimator, or command to set Y-momentummode while not in steady-state Y-Thomson\r\n";
char Est_ChangeisntAllowed_note_t[]			 = "Attempt was made to change to an estimation mode that would be inappropriate for the current control mode\r\n";
char CurrMTMSampMode_note_t[]			 = "Current magnetometer sampling mode.\r\n";
char ModelAndMeaMF_DiffersInSize_note_t[]			 = "Modelled and measured magnetic field differs in size by more than 5000 nT\r\n";
char Node_Recovery_Err_note_t[]			 = "Failed to Recover an ADCS Node by successive resets\r\n";
char CS1_Runtime_Err_note_t[]			 = "Runtime error occurred with the CubeSense1\r\n";
char CS2_Runtime_Err_note_t[]			 = "Runtime error occurred with the CubeSense2\r\n";
char CubeCtrl_Signal_Runtime_Err_note_t[]			 = "Runtime error occurred with the CubeControl Signal MCU\r\n";
char CubeCtrl_Motor_Runtime_Err_note_t[]			 = "Runtime error occurred with the CubeControl Motor MCU\r\n";
char CubeW1_Runtime_Err_note_t[]			 = "Runtime error occurred with the CubeWheel1\r\n";
char CubeW2_Runtime_Err_note_t[]			 = "Runtime error occurred with the CubeWheel2\r\n";
char CubeW3_Runtime_Err_note_t[]			 = "Runtime error occurred with the CubeWheel3\r\n";
char CubeStar_Runtime_Err_note_t[]			 = "Runtime error occurred with the CubeStar\r\n";
char MTM_Err_note_t[]			 = "Magnetometer failure occurred\r\n";
char RateSensor_Failure_note_t[]			 = "Rate sensor failure occurred\r\n";
char GET_ACP_Type_note_t[]			 = "ACP Type\r\n";
char GET_Special_Ctrl_Select_note_t[]			 = "Special Control Selection.\r\n";
char GET_CubeCtrlSig_Ver_note_t[]			 = "CubeControl Signal Version\r\n";
char GET_CubeCtrlMotor_Ver_note_t[]			 = "CubeControl Motor Version\r\n";
char GET_CS1_Ver_note_t[]			 = "CubeSense1 Version\r\n";
char GET_CS2_Ver_note_t[]			 = "CubeSense2 Version\r\n";
char GET_CS1_CamType_note_t[]			 = "CubeSense1 Camera Type.\r\n";
char GET_CS2_CamType_note_t[]			 = "CubeSense2 Camera Type\r\n";
char GET_CubeStar_Ver_note_t[]			 = "CubeStar Version\r\n";
char GET_GPS_Type_note_t[]			 = "GPS Type\r\n";
char GET_Redun_MTM_Includ_note_t[]			 = "Redundant MTM Included\r\n";
char GET_MTQX_MaxDipole_note_t[]			 = "Magnetorquer-X Max Dipole\r\n";
char GET_MTQY_MaxDipole_note_t[]			 = "Magnetorquer-Y Max Dipole\r\n";
char GET_MTQZ_MaxDipole_note_t[]			 = "Magnetorquer-Z Max Dipole\r\n";
char GET_MTQ_Ontime_Res_note_t[]			 = "Magnetorquer On-time Resolution\r\n";
char GET_MTQ_Max_OnTime_note_t[]			 = "Magnetorquer Maximum Ontime\r\n";
char GET_RWX_MaxTorque_note_t[]			 = "RW-X Maximum Torque\r\n";
char GET_RWY_MaxTorque_note_t[]			 = "RW-Y Maximum Torque\r\n";
char GET_RWZ_MaxTorque_note_t[]			 = "RW-Z Maximum Torque\r\n";
char GET_RWX_MaxMomentum_note_t[]			 = "RW-X Maximum Momentum\r\n";
char GET_RWY_MaxMomentum_note_t[]			 = "RW-Y Maximum Momentum\r\n";
char GET_RWZ_MaxMomentum_note_t[]			 = "RW-Z Maximum Momentum\r\n";
char GET_RWX_Inertia_note_t[]			 = "RW-X Inertia.\r\n";
char GET_RWY_Inertia_note_t[]			 = "RW-Y Inertia.\r\n";
char GET_RWZ_Inertia_note_t[]			 = "RW-Z Inertia.\r\n";
char GET_RW_Torque_Increment_note_t[]			 = "RW Torque Increment\r\n";
char GET_Prim_MTMXBias_d1_note_t[]			 = "Primary MTM X-Bias d1\r\n";
char GET_Prim_MTMYBias_d1_note_t[]			 = "Primary MTM Y-Bias d1\r\n";
char GET_Prim_MTMZBias_d1_note_t[]			 = "Primary MTM Z-Bias d1\r\n";
char GET_Prim_MTMXBias_d2_note_t[]			 = "Primary MTM X-Bias d1\r\n";
char GET_Prim_MTMYBias_d2_note_t[]			 = "Primary MTM Y-Bias d1\r\n";
char GET_Prim_MTMZBias_d2_note_t[]			 = "Primary MTM Z-Bias d1\r\n";
char GET_Prim_MTMXSens_s1_note_t[]			 = "Primary MTM X-Sens s1\r\n";
char GET_Prim_MTMYSens_s1_note_t[]			 = "Primary MTM Y-Sens s1\r\n";
char GET_Prim_MTMZSens_s1_note_t[]			 = "Primary MTM Z-Sens s1\r\n";
char GET_Prim_MTMXSens_s2_note_t[]			 = "Primary MTM X-Sens s2\r\n";
char GET_Prim_MTMYSens_s2_note_t[]			 = "Primary MTM Y-Sens s2\r\n";
char GET_Prim_MTMZSens_s2_note_t[]			 = "Primary MTM Z-Sens s2\r\n";
char GET_Redun_MTMXBias_d1_note_t[]			 = "Redundant MTM X-Bias d1\r\n";
char GET_Redun_MTMYBias_d1_note_t[]			 = "Redundant MTM Y-Bias d1\r\n";
char GET_Redun_MTMZBias_d1_note_t[]			 = "Redundant MTM Z-Bias d1\r\n";
char GET_Redun_MTMXBias_d2_note_t[]			 = "Redundant MTM X-Bias d1\r\n";
char GET_Redun_MTMYBias_d2_note_t[]			 = "Redundant MTM Y-Bias d1\r\n";
char GET_Redun_MTMZBias_d2_note_t[]			 = "Redundant MTM Z-Bias d1\r\n";
char GET_Redun_MTMXSens_s1_note_t[]			 = "Redundant MTM X-Sens s1\r\n";
char GET_Redun_MTMYSens_s1_note_t[]			 = "RedundantMTM Y-Sens s1\r\n";
char GET_Redun_MTMZSens_s1_note_t[]			 = "Redundant MTM Z-Sens s1\r\n";
char GET_Redun_MTMXSens_s2_note_t[]			 = "Redundant MTM X-Sens s2\r\n";
char GET_Redun_MTMYSens_s2_note_t[]			 = "Redundant MTM Y-Sens s2\r\n";
char GET_Redun_MTMZSens_s2_note_t[]			 = "Redundant MTM Z-Sens s2\r\n";
char GET_CCSignal_En_Port_note_t[]			 = "CC Signal Enable GPIO port\r\n";
char GET_CCSignal_En_Pin_note_t[]			 = "CC Signal Enable GPIO port pin\r\n";
char GET_CCMotor_En_Port_note_t[]			 = "CC Motor Enable GPIO port\r\n";
char GET_CCMotor_En_Pin_note_t[]			 = "CC Motor Enable GPIO port pin\r\n";
char GET_CCCommon_En_Port_note_t[]			 = "CC Common Enable GPIO port\r\n";
char GET_CCCommon_En_Pin_note_t[]			 = "CC Common Enable GPIO port pin\r\n";
char GET_CS1_En_Port_note_t[]			 = "CubeSense1 Enable GPIO port.\r\n";
char GET_CS1_En_Pin_note_t[]			 = "CubeSense1 Enable GPIO port pin.\r\n";
char GET_CS2_En_Port_note_t[]			 = "CubeSense2 Enable GPIO port.\r\n";
char GET_CS2_En_Pin_note_t[]			 = "CubeSense2 Enable GPIO port pin.\r\n";
char GET_CubeStar_En_Port_note_t[]			 = "CubeStar Enable GPIO port\r\n";
char GET_CubeStar_En_Pin_note_t[]			 = "CubeStar Enable GPIO port pin.\r\n";
char GET_CW1_En_Port_note_t[]			 = "CubeWheel1 Enable GPIO port\r\n";
char GET_CW1_En_Pin_note_t[]			 = "CubeWheel1 Enable GPIO port pin.\r\n";
char GET_CW2_En_Port_note_t[]			 = "CubeWheel2 Enable GPIO port\r\n";
char GET_CW2_En_Pin_note_t[]			 = "CubeWheel2 Enable GPIO port pin.\r\n";
char GET_CW3_En_Port_note_t[]			 = "CubeWheel3 Enable GPIO port\r\n";
char GET_CW3_En_Pin_note_t[]			 = "CubeWheel3 Enable GPIO port pin.\r\n";
char GET_Incl_Coeff_note_t[]			 = "Inclination filter coefficient\r\n";
char GET_Raan_Coeff_note_t[]			 = "RAAN filter coefficient.\r\n";
char GET_Ecc_Coeff_note_t[]			 = "Eccentricity filter coefficient\r\n";
char GET_Aop_Coeff_note_t[]			 = "Argument of perigee filter coefficient\r\n";
char GET_Time_Coeff_note_t[]			 = "Time filter coefficient\r\n";
char GET_Pos_Coeff_note_t[]			 = "Position filter coefficient\r\n";
char GET_Max_Pos_Err_note_t[]			 = "Maximum position error for asgp4 to continue working\r\n";
char GET_ASGP4_Filter_note_t[]			 = "The type of filter asgp4 is using\r\n";
char GET_xp_Coeff_note_t[]			 = "Polar coefficient xp\r\n";
char GET_yp_Coeff_note_t[]			 = "Polar coefficient yp\r\n";
char GET_GPS_RollOver_note_t[]			 = "GPS roll over number\r\n";
char GET_Pos_sd_note_t[]			 = "Maximum position standard deviation for asgp4 to operate\r\n";
char GET_Velo_sd_note_t[]			 = "Maximum velocity standard deviation for asgp4 to operate\r\n";
char GET_Min_satellites_note_t[]			 = "Minimum satellites for asgp4 to operate\r\n";
char GET_Time_Gain_note_t[]			 = "Time offset compensation gain\r\n";
char GET_Max_Lag_note_t[]			 = "Maximum lagged timestamp measurements to incorporate\r\n";
char GET_Min_Samples_note_t[]			 = "Minimum samples to use for asgp4 process\r\n";
char ASGP4_Complete_note_t[]			 = "Is ASGP4 process complete\r\n";
char ASGP4_Error_note_t[]			 = "The error state that the asgp4 module is in\r\n";
char ASGP4_Epoch_note_t[]			 = "Epoch from asgp4\r\n";
char ASGP4_Inclination_note_t[]			 = "Inclination from asgp4\r\n";
char ASGP4_RAAN_note_t[]			 = "RAAN from asgp4\r\n";
char ASGP4_ECC_note_t[]			 = "Eccentricity from asgp4\r\n";
char ASGP4_AOP_note_t[]			 = "AOP from asgp4\r\n";
char ASGP4_MA_note_t[]			 = "MA from asgp4\r\n";
char ASGP4_MM_note_t[]			 = "MM from asgp4\r\n";
char ASGP4_Bstar_note_t[]			 = "Bstar from asgp4\r\n";
char CS2_3V3_Current_note_t[]			 = "CubeSense2 3V3 Current.\r\n";
char CS2_SRAM_Current_note_t[]			 = "CubeSense2 Cam SRAM Current.\r\n";
char Percentage_Complete_note_t[]			 = "Current progress of operation.\r\n";
char Img_Cap_Status_note_t[]			 = "Current status of operation\r\n";
char Format_Busy_note_t[]			 = "Busy formatting SD card\r\n";
char Erase_All_Busy_note_t[]			 = "Busy formatting erasing all SD files\r\n";
char GET_Log1_Selection_note_t[]			 = "Log1 Selection\r\n";
char GET_Log1_Period_note_t[]			 = "Log1 period\r\n";
char GET_Log1_Destination_note_t[]			 = "log1 SD card to use to store log file\r\n";
char GET_Log2_Selection_note_t[]			 = "Log2 Selection\r\n";
char GET_Log2_Period_note_t[]			 = "Log2 period\r\n";
char GET_Log2_Destination_note_t[]			 = "log2 SD card to use to store log file\r\n";
char GET_LogUART_Selection_note_t[]			 = "LogUART Selection\r\n";
char GET_LogUART_Period_note_t[]			 = "LogUART period\r\n";
char GET_Inertial_RefX_note_t[]			 = "Inertial Reference X\r\n";
char GET_Inertial_RefY_note_t[]			 = "Inertial Reference Y\r\n";
char GET_Inertial_RefZ_note_t[]			 = "Inertial Reference Z\r\n";
char LastTCID_note_t[]			 = "Last TC ID\r\n";
char ProcessedFlag_note_t[]			 = "Processed flag\r\n";
char TCErrStatus_note_t[]			 = "TC error status\r\n";
char TCParaErrIndex_note_t[]			 = "TC parameter error index\r\n";
char FileDLBuff_Packet_Buff_note_t[]			 = "Packet counter of this file download packet\r\n";
char FileDLBuff_FileBytes_note_t[]			 = "File 20-byte packet\r\n";
char DL_Block_Ready_note_t[]			 = "Download Block Ready\r\n";
char DL_Block_ParaError_note_t[]			 = "The combination of message length and hole map resulted in invalid array lengths\r\n";
char DL_Block_CRC16_Checksum_note_t[]			 = "Block CRC16 Checksum\r\n";
char DL_Block_Length_note_t[]			 = "Block length\r\n";
char File_Infor_Type_note_t[]			 = "File Information Type\r\n";
char File_Infor_Busy_Updating_note_t[]			 = "Busy updating\r\n";
char File_Infor_Ctr_note_t[]			 = "File Information Counter\r\n";
char File_Infor_Size_note_t[]			 = "File Information Size\r\n";
char File_Infor_DateTime_note_t[]			 = "File Date and Time (in MS-DOS format)\r\n";
char File_Infor_CRC16_Checksum_note_t[]			 = "File CRC16 Checksum\r\n";
char Init_Upload_Complete_note_t[]			 = "Busy with file initialization (flash erase, etc.)\r\n";
char FinalUL_Block_Busy_note_t[]			 = "Busy with block finalization\r\n";
char FinalUL_Block_Error_note_t[]			 = "Error in block finalization\r\n";
char FileUL_Block_CRC16_Checksum_note_t[]			 = "File upload Block CRC16 Checksum\r\n";
char GET_FileUL_HoleMap_1_note_t[]			 = "File Upload Hole Map 1\r\n";
char GET_FileUL_HoleMap_2_note_t[]			 = "File Upload Hole Map 2\r\n";
char GET_FileUL_HoleMap_3_note_t[]			 = "File Upload Hole Map 3\r\n";
char GET_FileUL_HoleMap_4_note_t[]			 = "File Upload Hole Map 4\r\n";
char GET_FileUL_HoleMap_5_note_t[]			 = "File Upload Hole Map 5\r\n";
char GET_FileUL_HoleMap_6_note_t[]			 = "File Upload Hole Map 6\r\n";
char GET_FileUL_HoleMap_7_note_t[]			 = "File Upload Hole Map 7\r\n";
char GET_FileUL_HoleMap_8_note_t[]			 = "File Upload Hole Map 8\r\n";
char Reset_Magic_number_note_t[]			 = "Magic number to make sure it is a valid reset command. Should equal 0x5A\r\n";
char SET_CurrentUnixTime_note_t[]			 = "Set Current Unix Time\r\n";
char SET_Milliseconds_note_t[]			 = "Set Milliseconds\r\n";
char SET_Cache_Enable_state_note_t[]			 = "Cache Enable state\r\n";
char ResetLogPointer_note_t[]			 = "Reset pointer to log buffer\r\n";
char AdvanceLogPointer_note_t[]			 = "Advance pointer to log buffer\r\n";
char ResetBootReg_note_t[]			 = "Reset Boot Registers\r\n";
char DeployMTMTimeout_note_t[]			 = "Deploy Timeout\r\n";
char SET_SRAM_Scrub_Size_note_t[]			 = "SRAM_Scrub_Size\r\n";
char SET_Save_Now_note_t[]			 = "Save current Unix time to flash memory\r\n";
char SET_Save_On_Update_note_t[]			 = "Save Unix time to flash memory whenever there is a cmd to update the Unix time\r\n";
char SET_Save_Periodic_note_t[]			 = "Save Unix time to flash memory periodically\r\n";
char SET_Period_note_t[]			 = "Interval at which to save Unix time to flash memory.\r\n";
char ADCSRunMode_bit_note_t[]			 = "Set ADCS Run mode\r\n";
char SET_CubeCtrlSignalPowerSelect_note_t[]			 = "Set CubeControl Signal Power Selection\r\n";
char SET_CubeCtrlMotorPowerSelect_note_t[]			 = "Set CubeControl Motor Power Selection\r\n";
char SET_CubeSense1PowerSelect_note_t[]			 = "Set CubeSense1 Power Selection\r\n";
char SET_CubeSense2PowerSelect_note_t[]			 = "Set CubeSense2 Power Selection\r\n";
char SET_CubeStarPowerSelect_note_t[]			 = "Set CubeStarPower Power Selection\r\n";
char SET_CubeWheel1PowerSelect_note_t[]			 = "Set CubeWheel1Power Power Selection\r\n";
char SET_CubeWheel2PowerSelect_note_t[]			 = "Set CubeWheel2Power Power Selection\r\n";
char SET_CubeWheel3PowerSelect_note_t[]			 = "Set CubeWheel3Power Power Selection\r\n";
char SET_MotorPower_note_t[]			 = "Set Motor Power\r\n";
char SET_GPSPower_note_t[]			 = "Set GPS Power\r\n";
char Clear_Err_Flag_note_t[]			 = "Clear ADCS error flags\r\n";
char Clear_HK_Err_Flag_note_t[]			 = "Clear HK Error flags\r\n";
char SET_CtrlMode_note_t[]			 = "Set Control Mode\r\n";
char CtrlModeTimeout_note_t[]			 = "Control Mode Timeout\r\n";
char AttEstMode_bit_note_t[]			 = "Set Attitude Estimation Mode\r\n";
char SET_Cmd_Roll_note_t[]			 = "Commanded roll angle\r\n";
char SET_Cmd_Pitch_note_t[]			 = "Commanded pitch angle.\r\n";
char SET_Cmd_Yaw_note_t[]			 = "Commanded yaw angle\r\n";
char CommandMTQX_note_t[]			 = "Set Commanded X Magnetorquer duty cycle\r\n";
char CommandMTQY_note_t[]			 = "Set Commanded Y Magnetorquer duty cycle\r\n";
char CommandMTQZ_note_t[]			 = "Set Commanded Z Magnetorquer duty cycle\r\n";
char CommandWheelXSpeed_note_t[]			 = "Set Commanded X wheel speed\r\n";
char CommandWheelYSpeed_note_t[]			 = "Set Commanded Y wheel speed\r\n";
char CommandWheelZSpeed_note_t[]			 = "Set Commanded Z wheel speed\r\n";
char Trig_ADCSLoop_note_t[]			 = "Trigger ADCS to perform one iteration of the control loop\r\n";
char Iter_Unix_Time_note_t[]			 = "Unix time for iteration\r\n";
char Iter_CSSRaw_1_note_t[]			 = "CSS1 raw measurement\r\n";
char Iter_CSSRaw_2_note_t[]			 = "CSS2 raw measurement\r\n";
char Iter_CSSRaw_3_note_t[]			 = "CSS3 raw measurement\r\n";
char Iter_CSSRaw_4_note_t[]			 = "CSS4 raw measurement\r\n";
char Iter_CSSRaw_5_note_t[]			 = "CSS5 raw measurement\r\n";
char Iter_CSSRaw_6_note_t[]			 = "CSS6 raw measurement\r\n";
char Iter_CSSRaw_7_note_t[]			 = "CSS7 raw measurement\r\n";
char Iter_CSSRaw_8_note_t[]			 = "CSS8 raw measurement\r\n";
char Iter_CSSRaw_9_note_t[]			 = "CSS9 raw measurement\r\n";
char Iter_CSSRaw_10_note_t[]			 = "CSS10 raw measurement\r\n";
char Iter_Cam1Raw_X_note_t[]			 = "Cam1 sensor raw X angle\r\n";
char Iter_Cam1Raw_Y_note_t[]			 = "Cam1 sensor raw Y angle\r\n";
char Iter_Cam1Busy_note_t[]			 = "Cam1 sensor capture status\r\n";
char Iter_Cam1Result_note_t[]			 = "Cam1 sensor detection result\r\n";
char Iter_Cam2Raw_X_note_t[]			 = "Cam2 sensor raw X angle\r\n";
char Iter_Cam2Raw_Y_note_t[]			 = "Cam2 sensor raw Y angle\r\n";
char Iter_Cam2Busy_note_t[]			 = "Cam2 sensor capture status\r\n";
char Iter_Cam2Result_note_t[]			 = "Cam2 sensor detection result\r\n";
char Iter_MagRaw_X_note_t[]			 = "Raw magnetometer X measurement\r\n";
char Iter_MagRaw_Y_note_t[]			 = "Raw magnetometer Y measurement\r\n";
char Iter_MagRaw_Z_note_t[]			 = "Raw magnetometer Z measurement\r\n";
char Iter_RateRaw_X_note_t[]			 = "Raw X rate sensor measurement\r\n";
char Iter_RateRaw_Y_note_t[]			 = "Raw Y rate sensor measurement\r\n";
char Iter_RateRaw_Z_note_t[]			 = "Raw Z rate sensor measurement\r\n";
char Iter_WheelRaw_X_note_t[]			 = "Raw X wheel speed measurement\r\n";
char Iter_WheelRaw_Y_note_t[]			 = "Raw Y wheel speed measurement\r\n";
char Iter_WheelRaw_Z_note_t[]			 = "Raw Z wheel speed measurement\r\n";
char Iter_Star1Camera_X_note_t[]			 = "Star1 camera X-vector\r\n";
char Iter_Star1Camera_Y_note_t[]			 = "Star1 camera Y-vector\r\n";
char Iter_Star1Camera_Z_note_t[]			 = "Star1 camera Z-vector\r\n";
char Iter_Star1Inertial_X_note_t[]			 = "Star1 inertial X-vector\r\n";
char Iter_Star1Inertial_Y_note_t[]			 = "Star1 inertial Y-vector\r\n";
char Iter_Star1Inertial_Z_note_t[]			 = "Star1 inertial Z-vector\r\n";
char Iter_Star2Camera_X_note_t[]			 = "Star2 camera X-vector\r\n";
char Iter_Star2Camera_Y_note_t[]			 = "Star2 camera Y-vector\r\n";
char Iter_Star2Camera_Z_note_t[]			 = "Star2 camera Z-vector\r\n";
char Iter_Star2Inertial_X_note_t[]			 = "Star2 inertial X-vector\r\n";
char Iter_Star2Inertial_Y_note_t[]			 = "Star2 inertial Y-vector\r\n";
char Iter_Star2Inertial_Z_note_t[]			 = "Star2 inertial Z-vector\r\n";
char Iter_Star3Camera_X_note_t[]			 = "Star3 camera X-vector\r\n";
char Iter_Star3Camera_Y_note_t[]			 = "Star3 camera Y-vector\r\n";
char Iter_Star3Camera_Z_note_t[]			 = "Star3 camera Z-vector\r\n";
char Iter_Star3Inertial_X_note_t[]			 = "Star3 inertial X-vector\r\n";
char Iter_Star3Inertial_Y_note_t[]			 = "Star3 inertial Y-vector\r\n";
char Iter_Star3Inertial_Z_note_t[]			 = "Star3 inertial Z-vector\r\n";
char Iter_Gps_Solution_Status_note_t[]			 = "GPS Solution Status\r\n";
char Iter_GPS_Ref_Week_note_t[]			 = "GPS Reference Week\r\n";
char Iter_GPS_Time_Millisec_note_t[]			 = "GPS Time Milliseconds\r\n";
char Iter_ECEF_Pos_X_note_t[]			 = "ECEF Position X\r\n";
char Iter_ECEF_Velo_X_note_t[]			 = "ECEF Velocity X\r\n";
char Iter_ECEF_Pos_Y_note_t[]			 = "ECEF Position Y\r\n";
char Iter_ECEF_Velo_Y_note_t[]			 = "ECEF Velocity Y\r\n";
char Iter_ECEF_Pos_Z_note_t[]			 = "ECEF Position Z\r\n";
char Iter_ECEF_Velo_Z_note_t[]			 = "ECEF Velocity Z\r\n";
char Iter_XposStandard_Devia_note_t[]			 = "X-pos Standard Deviation\r\n";
char Iter_YposStandard_Devia_note_t[]			 = "Y-pos Standard Deviation\r\n";
char Iter_ZposStandard_Devia_note_t[]			 = "Z-pos Standard Deviation\r\n";
char Iter_XvelStandard_Devia_note_t[]			 = "X-vel Standard Deviation\r\n";
char Iter_YvelStandard_Devia_note_t[]			 = "Y-vel Standard Deviation\r\n";
char Iter_ZvelStandard_Devia_note_t[]			 = "Z-vel Standard Deviation\r\n";
char SET_MTQ1config_note_t[]			 = "Set MTQ 1 Configuration\r\n";
char SET_MTQ2config_note_t[]			 = "Set MTQ 2 Configuration\r\n";
char SET_MTQ3config_note_t[]			 = "Set MTQ 3 Configuration\r\n";
char SET_RW1_Config_note_t[]			 = "Set RW1 Configuration\r\n";
char SET_RW2_Config_note_t[]			 = "Set RW2 Configuration\r\n";
char SET_RW3_Config_note_t[]			 = "Set RW3 Configuration\r\n";
char SET_RW4_Config_note_t[]			 = "Set RW4 Configuration\r\n";
char SET_Gyro1Config_note_t[]			 = "Set Gyro1 Configuration\r\n";
char SET_Gyro2Config_note_t[]			 = "Set Gyro2 Configuration\r\n";
char SET_Gyro3Config_note_t[]			 = "Set Gyro3 Configuration\r\n";
char SET_XRateSensorOffset_note_t[]			 = "Set X-Rate Sensor Offset\r\n";
char SET_YRateSensorOffset_note_t[]			 = "Set Y-Rate Sensor Offset\r\n";
char SET_ZRateSensorOffset_note_t[]			 = "Set Z-Rate Sensor Offset\r\n";
char SET_RateSensorMult_note_t[]			 = "Set RateSensorMult\r\n";
char SET_CSS1_Config_note_t[]			 = "CSS1 Configuration.\r\n";
char SET_CSS2_Config_note_t[]			 = "CSS2 Configuration.\r\n";
char SET_CSS3_Config_note_t[]			 = "CSS3 Configuration.\r\n";
char SET_CSS4_Config_note_t[]			 = "CSS4 Configuration.\r\n";
char SET_CSS5_Config_note_t[]			 = "CSS5 Configuration.\r\n";
char SET_CSS6_Config_note_t[]			 = "CSS6 Configuration.\r\n";
char SET_CSS7_Config_note_t[]			 = "CSS7 Configuration.\r\n";
char SET_CSS8_Config_note_t[]			 = "CSS8 Configuration.\r\n";
char SET_CSS9_Config_note_t[]			 = "CSS9 Configuration.\r\n";
char SET_CSS10_Config_note_t[]			 = "CSS10 Configuration.\r\n";
char SET_CSS1_Rela_Scale_note_t[]			 = "CSS1 Relative Scaling Factor\r\n";
char SET_CSS2_Rela_Scale_note_t[]			 = "CSS2 Relative Scaling Factor\r\n";
char SET_CSS3_Rela_Scale_note_t[]			 = "CSS3 Relative Scaling Factor\r\n";
char SET_CSS4_Rela_Scale_note_t[]			 = "CSS4 Relative Scaling Factor\r\n";
char SET_CSS5_Rela_Scale_note_t[]			 = "CSS5 Relative Scaling Factor\r\n";
char SET_CSS6_Rela_Scale_note_t[]			 = "CSS6 Relative Scaling Factor\r\n";
char SET_CSS7_Rela_Scale_note_t[]			 = "CSS7 Relative Scaling Factor\r\n";
char SET_CSS8_Rela_Scale_note_t[]			 = "CSS8 Relative Scaling Factor\r\n";
char SET_CSS9_Rela_Scale_note_t[]			 = "CSS9 Relative Scaling Factor\r\n";
char SET_CSS10_Rela_Scale_note_t[]			 = "CSS10 Relative Scaling Factor\r\n";
char SET_CSS_Threshold_note_t[]			 = "Relative Scaling Factor\r\n";
char SET_Cam1_Mounting_Trans_Alpha_note_t[]			 = "Cam1 Sensor Mounting Transform Alpha Angle\r\n";
char SET_Cam1_Mounting_Trans_Beta_note_t[]			 = "Cam1 Sensor Mounting Transform Beta Angle\r\n";
char SET_Cam1_Mounting_Trans_Gamma_note_t[]			 = "Cam1 Sensor Mounting Transform Gamma Angle\r\n";
char SET_Cam1_DetectThreshold_note_t[]			 = "Cam1 detection threshold\r\n";
char SET_Cam1_AutoAdjust_note_t[]			 = "Cam1 sensor auto adjust mode\r\n";
char SET_Cam1_Exposure_Time_note_t[]			 = "Cam1 sensor exposure time\r\n";
char SET_Cam1_Boresight_X_note_t[]			 = "X Pixel location of Cam1 boresight.\r\n";
char SET_Cam1_Boresight_Y_note_t[]			 = "Y Pixel location of Cam1 boresight\r\n";
char SET_Cam2_Mounting_Trans_Alpha_note_t[]			 = "Cam2 Sensor Mounting Transform Alpha Angle\r\n";
char SET_Cam2_Mounting_Trans_Beta_note_t[]			 = "Cam2 Sensor Mounting Transform Beta Angle\r\n";
char SET_Cam2_Mounting_Trans_Gamma_note_t[]			 = "Cam2 Sensor Mounting Transform Gamma Angle\r\n";
char SET_Cam2_DetectThreshold_note_t[]			 = "Cam2 detection threshold\r\n";
char SET_Cam2_AutoAdjust_note_t[]			 = "Cam2 sensor auto adjust mode\r\n";
char SET_Cam2_Exposure_Time_note_t[]			 = "Cam2 sensor exposure time\r\n";
char SET_Cam2_Boresight_X_note_t[]			 = "X Pixel location of Cam2 boresight.\r\n";
char SET_Cam2_Boresight_Y_note_t[]			 = "Y Pixel location of Cam2 boresight\r\n";
char SET_Nadir_Max_Devia_Percent_note_t[]			 = "Percentage of measured angular radius as edge�s maximum allowable deviation\r\n";
char SET_Nadir_Max_BadEdges_note_t[]			 = "Maximum amount of edges allowed outside maximum deviation\r\n";
char SET_Nadir_Max_Radius_note_t[]			 = "Nadir Max Radius\r\n";
char SET_Nadir_Min_Radius_note_t[]			 = "Nadir Min Radius\r\n";
char SET_Cam1_MinX_Area1_note_t[]			 = "Cam 1 Minimum X of area 1\r\n";
char SET_Cam1_MaxX_Area1_note_t[]			 = "Cam 1 Maximum X of area 1\r\n";
char SET_Cam1_MinY_Area1_note_t[]			 = "Cam 1 Minimum Y of area 1\r\n";
char SET_Cam1_MaxY_Area1_note_t[]			 = "Cam 1 Maximum Y of area 1\r\n";
char SET_Cam1_MinX_Area2_note_t[]			 = "Cam 1 Minimum X of area 2\r\n";
char SET_Cam1_MaxX_Area2_note_t[]			 = "Cam 1 Maximum X of area 2\r\n";
char SET_Cam1_MinY_Area2_note_t[]			 = "Cam 1 Minimum Y of area 2\r\n";
char SET_Cam1_MaxY_Area2_note_t[]			 = "Cam 1 Maximum Y of area 2\r\n";
char SET_Cam1_MinX_Area3_note_t[]			 = "Cam 1 Minimum X of area 3\r\n";
char SET_Cam1_MaxX_Area3_note_t[]			 = "Cam 1 Maximum X of area 3\r\n";
char SET_Cam1_MinY_Area3_note_t[]			 = "Cam 1 Minimum Y of area 3\r\n";
char SET_Cam1_MaxY_Area3_note_t[]			 = "Cam 1 Maximum Y of area 3\r\n";
char SET_Cam1_MinX_Area4_note_t[]			 = "Cam 1 Minimum X of area 4\r\n";
char SET_Cam1_MaxX_Area4_note_t[]			 = "Cam 1 Maximum X of area 4\r\n";
char SET_Cam1_MinY_Area4_note_t[]			 = "Cam 1 Minimum Y of area 4\r\n";
char SET_Cam1_MaxY_Area4_note_t[]			 = "Cam 1 Maximum Y of area 4\r\n";
char SET_Cam1_MinX_Area5_note_t[]			 = "Cam 1 Minimum X of area 5\r\n";
char SET_Cam1_MaxX_Area5_note_t[]			 = "Cam 1 Maximum X of area 5\r\n";
char SET_Cam1_MinY_Area5_note_t[]			 = "Cam 1 Minimum Y of area 5\r\n";
char SET_Cam1_MaxY_Area5_note_t[]			 = "Cam 1 Maximum Y of area 5\r\n";
char SET_Cam2_MinX_Area1_note_t[]			 = "Cam 2 Minimum X of area 1\r\n";
char SET_Cam2_MaxX_Area1_note_t[]			 = "Cam 2 Maximum X of area 1\r\n";
char SET_Cam2_MinY_Area1_note_t[]			 = "Cam 2 Minimum Y of area 1\r\n";
char SET_Cam2_MaxY_Area1_note_t[]			 = "Cam 2 Maximum Y of area 1\r\n";
char SET_Cam2_MinX_Area2_note_t[]			 = "Cam 2 Minimum X of area 2\r\n";
char SET_Cam2_MaxX_Area2_note_t[]			 = "Cam 2 Maximum X of area 2\r\n";
char SET_Cam2_MinY_Area2_note_t[]			 = "Cam 2 Minimum Y of area 2\r\n";
char SET_Cam2_MaxY_Area2_note_t[]			 = "Cam 2 Maximum Y of area 2\r\n";
char SET_Cam2_MinX_Area3_note_t[]			 = "Cam 2 Minimum X of area 3\r\n";
char SET_Cam2_MaxX_Area3_note_t[]			 = "Cam 2 Maximum X of area 3\r\n";
char SET_Cam2_MinY_Area3_note_t[]			 = "Cam 2 Minimum Y of area 3\r\n";
char SET_Cam2_MaxY_Area3_note_t[]			 = "Cam 2 Maximum Y of area 3\r\n";
char SET_Cam2_MinX_Area4_note_t[]			 = "Cam 2 Minimum X of area 4\r\n";
char SET_Cam2_MaxX_Area4_note_t[]			 = "Cam 2 Maximum X of area 4\r\n";
char SET_Cam2_MinY_Area4_note_t[]			 = "Cam 2 Minimum Y of area 4\r\n";
char SET_Cam2_MaxY_Area4_note_t[]			 = "Cam 2 Maximum Y of area 4\r\n";
char SET_Cam2_MinX_Area5_note_t[]			 = "Cam 2 Minimum X of area 5\r\n";
char SET_Cam2_MaxX_Area5_note_t[]			 = "Cam 2 Maximum X of area 5\r\n";
char SET_Cam2_MinY_Area5_note_t[]			 = "Cam 2 Minimum Y of area 5\r\n";
char SET_Cam2_MaxY_Area5_note_t[]			 = "Cam 2 Maximum Y of area 5\r\n";
char SET_MTMMountAlpha_note_t[]			 = "Set Magnetometer Mounting Transform Alpha Angle\r\n";
char SET_MTMMountBeta_note_t[]			 = "Set Magnetometer Mounting Transform Beta Angle\r\n";
char SET_MTMMountGamma_note_t[]			 = "Set Magnetometer Mounting Transform Gamma Angle\r\n";
char SET_MTM_Ch1Offset_note_t[]			 = "Set Magnetometer Channel 1 Offset\r\n";
char SET_MTM_Ch2Offset_note_t[]			 = "Set Magnetometer Channel 2 Offset\r\n";
char SET_MTM_Ch3Offset_note_t[]			 = "Set Magnetometer Channel 3 Offset\r\n";
char SET_MTM_S11_note_t[]			 = "Set Magnetometer Sensitivity Matrix S11\r\n";
char SET_MTM_S22_note_t[]			 = "Set Magnetometer Sensitivity Matrix S22\r\n";
char SET_MTM_S33_note_t[]			 = "Set Magnetometer Sensitivity Matrix S33\r\n";
char SET_MTM_S12_note_t[]			 = "Set Magnetometer Sensitivity Matrix S12\r\n";
char SET_MTM_S13_note_t[]			 = "Set Magnetometer Sensitivity Matrix S13\r\n";
char SET_MTM_S21_note_t[]			 = "Set Magnetometer Sensitivity Matrix S21\r\n";
char SET_MTM_S23_note_t[]			 = "Set Magnetometer Sensitivity Matrix S23\r\n";
char SET_MTM_S31_note_t[]			 = "Set Magnetometer Sensitivity Matrix S31\r\n";
char SET_MTM_S32_note_t[]			 = "Set Magnetometer Sensitivity Matrix S32\r\n";
char SET_MTMRateFilterSysNoise_note_t[]			 = "Magnetometer Rate Filter System Noise\r\n";
char SET_EKFSysNoise_note_t[]			 = "EKF System Noise\r\n";
char SET_CSSMeasureNoise_note_t[]			 = "CSS Measurement Noise\r\n";
char SET_FSSMeasureNoise_note_t[]			 = "Sun Sensor Measurement Noise\r\n";
char SET_NadirSensMeasureNoise_note_t[]			 = "Nadir Sensor Measurement Noise\r\n";
char SET_MTMMeasureNoise_note_t[]			 = "Magnetometer Measurement Noise\r\n";
char SET_STMeasureNoise_note_t[]			 = "Star Tracker Measurement Noise\r\n";
char SET_UseFSS_note_t[]			 = "Use Sun Sensor\r\n";
char SET_UseNadirSens_note_t[]			 = "Use Nadir Sensor\r\n";
char SET_UseCSS_note_t[]			 = "Use CSS\r\n";
char SET_UseST_note_t[]			 = "Use Star Tracker\r\n";
char SET_NadirSensterminatortest_note_t[]			 = "Nadir sensor terminator test\r\n";
char SET_AutoMTMRecovery_note_t[]			 = "Automatic Magnetometer Recovery\r\n";
char SET_MTMMode_note_t[]			 = "Magnetometer Mode\r\n";
char SET_MTMSelectForRAW_MTM_TLM_note_t[]			 = "Magnetometer Selection for RAW MTM TLM\r\n";
char SET_AutoEstTransitDueToRateSensErr_note_t[]			 = "Automatic Estimation Transition due to Rate Sensor Errors\r\n";
char SET_Cam1Cam2SampPeriod_note_t[]			 = "Cam1 and Cam2 Sampling Period\r\n";
char SET_Incl_Coeff_note_t[]			 = "Inclination filter coefficient\r\n";
char SET_Raan_Coeff_note_t[]			 = "RAAN filter coefficient.\r\n";
char SET_Ecc_Coeff_note_t[]			 = "Eccentricity filter coefficient\r\n";
char SET_Aop_Coeff_note_t[]			 = "Argument of perigee filter coefficient\r\n";
char SET_Time_Coeff_note_t[]			 = "Time filter coefficient\r\n";
char SET_Pos_Coeff_note_t[]			 = "Position filter coefficient\r\n";
char SET_Max_Pos_Err_note_t[]			 = "Maximum position error for asgp4 to continue working\r\n";
char SET_ASGP4_Filter_note_t[]			 = "The type of filter asgp4 is using\r\n";
char SET_xp_Coeff_note_t[]			 = "Polar coefficient xp\r\n";
char SET_yp_Coeff_note_t[]			 = "Polar coefficient yp\r\n";
char SET_GPS_RollOver_note_t[]			 = "GPS roll over number\r\n";
char SET_Pos_sd_note_t[]			 = "Maximum position standard deviation for asgp4 to operate\r\n";
char SET_Velo_sd_note_t[]			 = "Maximum velocity standard deviation for asgp4 to operate\r\n";
char SET_Min_satellites_note_t[]			 = "Minimum satellites for asgp4 to operate\r\n";
char SET_Time_Gain_note_t[]			 = "Time offset compensation gain\r\n";
char SET_Max_Lag_note_t[]			 = "Maximum lagged timestamp measurements to incorporate\r\n";
char SET_ACP_Type_note_t[]			 = "ACP Type\r\n";
char SET_Special_Ctrl_Select_note_t[]			 = "Special Control Selection.\r\n";
char SET_CubeCtrlSig_Ver_note_t[]			 = "CubeControl Signal Version\r\n";
char SET_CubeCtrlMotor_Ver_note_t[]			 = "CubeControl Motor Version\r\n";
char SET_CS1_Ver_note_t[]			 = "CubeSense1 Version\r\n";
char SET_CS2_Ver_note_t[]			 = "CubeSense2 Version\r\n";
char SET_CS1_CamType_note_t[]			 = "CubeSense1 Camera Type.\r\n";
char SET_CS2_CamType_note_t[]			 = "CubeSense2 Camera Type\r\n";
char SET_CubeStar_Ver_note_t[]			 = "CubeStar Version\r\n";
char SET_GPS_Type_note_t[]			 = "GPS Type\r\n";
char SET_Redun_MTM_Includ_note_t[]			 = "Redundant MTM Included\r\n";
char SET_MTQX_MaxDipole_note_t[]			 = "Magnetorquer-X Max Dipole\r\n";
char SET_MTQY_MaxDipole_note_t[]			 = "Magnetorquer-Y Max Dipole\r\n";
char SET_MTQZ_MaxDipole_note_t[]			 = "Magnetorquer-Z Max Dipole\r\n";
char SET_MTQ_Ontime_Res_note_t[]			 = "Magnetorquer On-time Resolution\r\n";
char SET_MTQ_Max_OnTime_note_t[]			 = "Magnetorquer Maximum Ontime\r\n";
char SET_RWX_MaxTorque_note_t[]			 = "RW-X Maximum Torque\r\n";
char SET_RWY_MaxTorque_note_t[]			 = "RW-Y Maximum Torque\r\n";
char SET_RWZ_MaxTorque_note_t[]			 = "RW-Z Maximum Torque\r\n";
char SET_RWX_MaxMomentum_note_t[]			 = "RW-X Maximum Momentum\r\n";
char SET_RWY_MaxMomentum_note_t[]			 = "RW-Y Maximum Momentum\r\n";
char SET_RWZ_MaxMomentum_note_t[]			 = "RW-Z Maximum Momentum\r\n";
char SET_RWX_Inertia_note_t[]			 = "RW-X Inertia.\r\n";
char SET_RWY_Inertia_note_t[]			 = "RW-Y Inertia.\r\n";
char SET_RWZ_Inertia_note_t[]			 = "RW-Z Inertia.\r\n";
char SET_RW_Torque_Increment_note_t[]			 = "RW Torque Increment\r\n";
char SET_Prim_MTMXBias_d1_note_t[]			 = "Primary MTM X-Bias d1\r\n";
char SET_Prim_MTMYBias_d1_note_t[]			 = "Primary MTM Y-Bias d1\r\n";
char SET_Prim_MTMZBias_d1_note_t[]			 = "Primary MTM Z-Bias d1\r\n";
char SET_Prim_MTMXBias_d2_note_t[]			 = "Primary MTM X-Bias d1\r\n";
char SET_Prim_MTMYBias_d2_note_t[]			 = "Primary MTM Y-Bias d1\r\n";
char SET_Prim_MTMZBias_d2_note_t[]			 = "Primary MTM Z-Bias d1\r\n";
char SET_Prim_MTMXSens_s1_note_t[]			 = "Primary MTM X-Sens s1\r\n";
char SET_Prim_MTMYSens_s1_note_t[]			 = "Primary MTM Y-Sens s1\r\n";
char SET_Prim_MTMZSens_s1_note_t[]			 = "Primary MTM Z-Sens s1\r\n";
char SET_Prim_MTMXSens_s2_note_t[]			 = "Primary MTM X-Sens s2\r\n";
char SET_Prim_MTMYSens_s2_note_t[]			 = "Primary MTM Y-Sens s2\r\n";
char SET_Prim_MTMZSens_s2_note_t[]			 = "Primary MTM Z-Sens s2\r\n";
char SET_Redun_MTMXBias_d1_note_t[]			 = "Redundant MTM X-Bias d1\r\n";
char SET_Redun_MTMYBias_d1_note_t[]			 = "Redundant MTM Y-Bias d1\r\n";
char SET_Redun_MTMZBias_d1_note_t[]			 = "Redundant MTM Z-Bias d1\r\n";
char SET_Redun_MTMXBias_d2_note_t[]			 = "Redundant MTM X-Bias d1\r\n";
char SET_Redun_MTMYBias_d2_note_t[]			 = "Redundant MTM Y-Bias d1\r\n";
char SET_Redun_MTMZBias_d2_note_t[]			 = "Redundant MTM Z-Bias d1\r\n";
char SET_Redun_MTMXSens_s1_note_t[]			 = "Redundant MTM X-Sens s1\r\n";
char SET_Redun_MTMYSens_s1_note_t[]			 = "RedundantMTM Y-Sens s1\r\n";
char SET_Redun_MTMZSens_s1_note_t[]			 = "Redundant MTM Z-Sens s1\r\n";
char SET_Redun_MTMXSens_s2_note_t[]			 = "Redundant MTM X-Sens s2\r\n";
char SET_Redun_MTMYSens_s2_note_t[]			 = "Redundant MTM Y-Sens s2\r\n";
char SET_Redun_MTMZSens_s2_note_t[]			 = "Redundant MTM Z-Sens s2\r\n";
char SET_CCSignal_En_Port_note_t[]			 = "CC Signal Enable GPIO port\r\n";
char SET_CCSignal_En_Pin_note_t[]			 = "CC Signal Enable GPIO port pin\r\n";
char SET_CCMotor_En_Port_note_t[]			 = "CC Motor Enable GPIO port\r\n";
char SET_CCMotor_En_Pin_note_t[]			 = "CC Motor Enable GPIO port pin\r\n";
char SET_CCCommon_En_Port_note_t[]			 = "CC Common Enable GPIO port\r\n";
char SET_CCCommon_En_Pin_note_t[]			 = "CC Common Enable GPIO port pin\r\n";
char SET_CS1_En_Port_note_t[]			 = "CubeSense1 Enable GPIO port.\r\n";
char SET_CS1_En_Pin_note_t[]			 = "CubeSense1 Enable GPIO port pin.\r\n";
char SET_CS2_En_Port_note_t[]			 = "CubeSense2 Enable GPIO port.\r\n";
char SET_CS2_En_Pin_note_t[]			 = "CubeSense2 Enable GPIO port pin.\r\n";
char SET_CubeStar_En_Port_note_t[]			 = "CubeStar Enable GPIO port\r\n";
char SET_CubeStar_En_Pin_note_t[]			 = "CubeStar Enable GPIO port pin.\r\n";
char SET_CW1_En_Port_note_t[]			 = "CubeWheel1 Enable GPIO port\r\n";
char SET_CW1_En_Pin_note_t[]			 = "CubeWheel1 Enable GPIO port pin.\r\n";
char SET_CW2_En_Port_note_t[]			 = "CubeWheel2 Enable GPIO port\r\n";
char SET_CW2_En_Pin_note_t[]			 = "CubeWheel2 Enable GPIO port pin.\r\n";
char SET_CW3_En_Port_note_t[]			 = "CubeWheel3 Enable GPIO port\r\n";
char SET_CW3_En_Pin_note_t[]			 = "CubeWheel3 Enable GPIO port pin.\r\n";
char SET_ASGP4_RunMode_note_t[]			 = "Set ASGP4 enabled state\r\n";
char Trig_ASGP4_note_t[]			 = "Trigger a start of the ASGP4 process\r\n";
char Format_SDcard_MagicNum_note_t[]			 = "Magic number to prevent against accidental format\r\n";
char SET_Inertial_RefX_note_t[]			 = "Inertial Reference X\r\n";
char SET_Inertial_RefY_note_t[]			 = "Inertial Reference Y\r\n";
char SET_Inertial_RefZ_note_t[]			 = "Inertial Reference Z\r\n";
char SET_Detumb_SpinGain_note_t[]			 = "Detumbling Spin Gain (Ks)\r\n";
char SET_Detumb_DampGain_note_t[]			 = "Detumbling Damping Gain (Kd)\r\n";
char SET_Ref_SpinRate_note_t[]			 = "Reference spin rate (wy-ref)\r\n";
char SET_FastBdot_DetumbGain_note_t[]			 = "Fast Bdot Detumbling Gain (Kdf)\r\n";
char SET_Ymoment_CtrlGain_note_t[]			 = "Y-Momentum Control Gain (Kh)\r\n";
char SET_Ymoment_NDampGain_note_t[]			 = "Y-momentum Nutation Damping Gain (Kn)\r\n";
char SET_Ymoment_PGain_note_t[]			 = "Y-momentum Proportional Gain (Kp1)\r\n";
char SET_Ymoment_DGain_note_t[]			 = "Y-momentum Derivative Gain (Kd1)\r\n";
char SET_RefWheel_Moment_note_t[]			 = "Reference Wheel Momentum (H-ref)\r\n";
char SET_Rwheel_PGain_note_t[]			 = "Rwheel Proportional Gain (Kp2)\r\n";
char SET_Rwheel_DGain_note_t[]			 = "Rwheel Derivative Gain (Kd2)\r\n";
char SET_YWheelBiasMoment_note_t[]			 = "Y-Wheel Bias Momentum (Hbias)\r\n";
char SET_SunPointFacet_note_t[]			 = "Satellite body axis that will align with sun vector\r\n";
char SET_AutoCtrl_TransitDueWheelErr_note_t[]			 = "Enable/disable automatic transition from wheelctrl modes to Y-Thomson mode in case of wheel error\r\n";
char SET_M_Of_IXX_note_t[]			 = "Moment Of Inertia � Ixx\r\n";
char SET_M_Of_IYY_note_t[]			 = "Moment Of Inertia � Iyy\r\n";
char SET_M_Of_IZZ_note_t[]			 = "Moment Of Inertia � Izz\r\n";
char SET_M_Of_IXY_note_t[]			 = "Product Of Inertia � Ixy\r\n";
char SET_M_Of_IXZ_note_t[]			 = "Product Of Inertia � Ixz\r\n";
char SET_M_Of_IYZ_note_t[]			 = "Product Of Inertia � Iyz\r\n";
char SET_Inclination_note_t[]			 = "Set Inclination\r\n";
char SET_Eccentricity_note_t[]			 = "Set Eccentricity\r\n";
char SET_RightAscensionOfTheAscendingNode_note_t[]			 = "Set Right-ascension of the Ascending Node\r\n";
char SET_ArgumentOfPerigee_note_t[]			 = "Set Argument of Perigee\r\n";
char SET_BStarDragTerm_note_t[]			 = "Set B-Star drag term\r\n";
char SET_MeanMotion_note_t[]			 = "Set Mean Motion\r\n";
char SET_MeanAnomaly_note_t[]			 = "Set Mean Anomaly\r\n";
char SET_Epoch_note_t[]			 = "Set Epoch\r\n";
char SET_MTM_Mode_note_t[]			 = "Mode describing which magnetometer is used for estimation and control\r\n";
char Source_File_CT_note_t[]			 = "Source File Counter\r\n";
char QualityFactor_note_t[]			 = "Quality Factor\r\n";
char WhiteBalance_note_t[]			 = "White Balance\r\n";
char Save_Config_bit_note_t[]			 = "Save current configuration to flash memory\r\n";
char Save_OrbitPara_bit_note_t[]			 = "Save current orbit parameters to flash memory\r\n";
char Camera_Select_note_t[]			 = "Camera Selection\r\n";
char Image_Size_note_t[]			 = "Image size selection.\r\n";
char Boot_Prog_Index_note_t[]			 = "Boot Program Index\r\n";
char RunSelectedProg_note_t[]			 = "Run Selected Program\r\n";
char ReadProgInfor_note_t[]			 = "Read Program Index\r\n";
char Source_Prog_Index_note_t[]			 = "Source Program Index\r\n";
char Bootloader_overwrite_flag_note_t[]			 = "Bootloader overwrite flag\r\n";
char SET_Log1_Selection_note_t[]			 = "Log1 Selection\r\n";
char SET_Log1_Period_note_t[]			 = "Log1 period\r\n";
char SET_Log1_Destination_note_t[]			 = "log1 SD card to use to store log file\r\n";
char SET_Log2_Selection_note_t[]			 = "Log2 Selection\r\n";
char SET_Log2_Period_note_t[]			 = "Log2 period\r\n";
char SET_Log2_Destination_note_t[]			 = "log2 SD card to use to store log file\r\n";
char SET_LogUART_Selection_note_t[]			 = "LogUART Selection\r\n";
char SET_LogUART_Period_note_t[]			 = "LogUART period\r\n";
char Erase_File_Type_note_t[]			 = "File Type\r\n";
char File_Counter_note_t[]			 = "File Counter\r\n";
char Erase_All_note_t[]			 = "Erase All\r\n";
char DL_File_type_note_t[]			 = "Download File Type\r\n";
char DL_Counter_note_t[]			 = "Download Counter\r\n";
char DL_offset_note_t[]			 = "Download offset\r\n";
char DL_BlockLength_note_t[]			 = "Download Block length\r\n";
char AdFile_List_ReadP_note_t[]			 = "Advance File List Read Pointer\r\n";
char InitFile_Upload_Desti_note_t[]			 = "Destination\r\n";
char InitFile_Upload_BlockSize_note_t[]			 = "Block Size\r\n";
char Packet_Number_note_t[]			 = "File Upload Packet Number\r\n";
char FileUpload_Bytes_note_t[]			 = "File Upload Packet File Bytes\r\n";
char FinalUpload_Desti_note_t[]			 = "Finalize Upload Block Destination\r\n";
char FinalUpload_Offset_note_t[]			 = "Finalize Upload Block offset\r\n";
char FinalUpload_BlockSize_note_t[]			 = "Finalize Upload Block lenght of block\r\n";
char Reset_Upload_Block_note_t[]			 = "Reset HoleMap for Upload Block\r\n";
char ResetFile_List_ReadP_note_t[]			 = "Reset File List Read Pointer\r\n";
char InitDLBurst_Msg_Length_note_t[]			 = "Message Length\r\n";
char InitDLBurst_Ignore_HoleMap_note_t[]			 = "Ignore Hole Map\r\n";
char SET_FileUL_HoleMap_1_note_t[]			 = "File Upload Hole Map 1\r\n";
char SET_FileUL_HoleMap_2_note_t[]			 = "File Upload Hole Map 2\r\n";
char SET_FileUL_HoleMap_3_note_t[]			 = "File Upload Hole Map 3\r\n";
char SET_FileUL_HoleMap_4_note_t[]			 = "File Upload Hole Map 4\r\n";
char SET_FileUL_HoleMap_5_note_t[]			 = "File Upload Hole Map 5\r\n";
char SET_FileUL_HoleMap_6_note_t[]			 = "File Upload Hole Map 6\r\n";
char SET_FileUL_HoleMap_7_note_t[]			 = "File Upload Hole Map 7\r\n";
char SET_FileUL_HoleMap_8_note_t[]			 = "File Upload Hole Map 8\r\n";
enum	cube_bit_reg
{
 Node_type=	0x0,
 Interface_Version=	0x1,
 Firmware_vr_Major=	0x2,
 Firmware_vr_Minor=	0x3,
 Runtime_s=	0x4,
 Runtime_ms=	0x5,
 Cause_of_MCU_Reset=	0x6,
 Boot_Cause=	0x7,
 Boot_Counter=	0x8,
 Boot_Program_Index=	0x9,
 Firmware2_vr_Major=	0xA,
 Firmware2_vr_Minor=	0xB,
 Prog_Index=	0xC,
 Boot_Status=	0xD,
 GET_Cache_Enable_state=	0xE,
 EstMode=	0xF,
 CtrlMode=	0x10,
 AdcsRunMode=	0x11,
 ASGP4Mode=	0x12,
 CubeCtrlSignalEnabled=	0x13,
 CubeCtrlMotorEnabled=	0x14,
 CubeSense1Enabled=	0x15,
 CubeSense2Enabled=	0x16,
 CubeWheel1Enabled=	0x17,
 CubeWheel2Enabled=	0x18,
 CubeWheel3Enabled=	0x19,
 CubeStarEnabled=	0x1A,
 GPSReceiverEnabled=	0x1B,
 GPSLNAPowerEnabled=	0x1C,
 MotorDriverEnabled=	0x1D,
 SunAboveLocalHorizon=	0x1E,
 CubeSense1CommuErr=	0x1F,
 CubeSense2CommuErr=	0x20,
 CubeCtrlSignalCommuErr=	0x21,
 CubeCtrlMotorCommuErr=	0x22,
 CubeWheel1CommuErr=	0x23,
 CubeWheel2CommuErr=	0x24,
 CubeWheel3CommuErr=	0x25,
 CubeStarCommuErr=	0x26,
 MTMRangeErr=	0x27,
 Cam1SRAMOVCDetected=	0x28,
 Cam13V3OVCDetected=	0x29,
 Cam1SensorBusyErr=	0x2A,
 Cam1SensorDetectionErr=	0x2B,
 SunSensorRangeErr=	0x2C,
 Cam2SRAMOVCDetected=	0x2D,
 Cam23V3OVCDetected=	0x2E,
 Cam2SensorBusyErr=	0x2F,
 Cam2SensorDetectionErr=	0x30,
 NadirSensorRangeErr=	0x31,
 RateSensorRangeErr=	0x32,
 WheelSpeedRangeErr=	0x33,
 CSSErr=	0x34,
 STMatchErr=	0x35,
 ST_OVCDetected=	0x36,
 Progress_Percentage=	0x37,
 Conversion_Result=	0x38,
 Output_File_CT=	0x39,
 GET_SRAM_Scrub_Size=	0x3A,
 ADCSConfigLoadErr=	0x3B,
 OrbitParaLoadErr=	0x3C,
 SystemConfigLoadErr=	0x3D,
 SDcardInitErr=	0x3E,
 SDcardreadErr=	0x3F,
 SDcardwriteErr=	0x40,
 GET_MTQ1config=	0x41,
 GET_MTQ2config=	0x42,
 GET_MTQ3config=	0x43,
 GET_RW1_Config=	0x44,
 GET_RW2_Config=	0x45,
 GET_RW3_Config=	0x46,
 GET_RW4_Config=	0x47,
 GET_Gyro1Config=	0x48,
 GET_Gyro2Config=	0x49,
 GET_Gyro3Config=	0x4A,
 GET_XRateSensorOffset=	0x4B,
 GET_YRateSensorOffset=	0x4C,
 GET_ZRateSensorOffset=	0x4D,
 GET_RateSensorMult=	0x4E,
 GET_CSS1_Config=	0x4F,
 GET_CSS2_Config=	0x50,
 GET_CSS3_Config=	0x51,
 GET_CSS4_Config=	0x52,
 GET_CSS5_Config=	0x53,
 GET_CSS6_Config=	0x54,
 GET_CSS7_Config=	0x55,
 GET_CSS8_Config=	0x56,
 GET_CSS9_Config=	0x57,
 GET_CSS10_Config=	0x58,
 GET_CSS1_Rela_Scale=	0x59,
 GET_CSS2_Rela_Scale=	0x5A,
 GET_CSS3_Rela_Scale=	0x5B,
 GET_CSS4_Rela_Scale=	0x5C,
 GET_CSS5_Rela_Scale=	0x5D,
 GET_CSS6_Rela_Scale=	0x5E,
 GET_CSS7_Rela_Scale=	0x5F,
 GET_CSS8_Rela_Scale=	0x60,
 GET_CSS9_Rela_Scale=	0x61,
 GET_CSS10_Rela_Scale=	0x62,
 GET_CSS_Threshold=	0x63,
 GET_CurrentUnixTime=	0x64,
 GET_Milliseconds=	0x65,
 Time_of_Event=	0x66,
 Event_ID=	0x67,
 Event_Parameter=	0x68,
 SRAM1_latchups=	0x69,
 SRAM2_latchups=	0x6A,
 Single_SRAM_upsets=	0x6B,
 Double_SRAM_upsets=	0x6C,
 Multi_SRAM_upsets=	0x6D,
 TC_CT=	0x6E,
 TLM_CT=	0x6F,
 TC_Buff_Overrun=	0x70,
 UART_Protocol_Error=	0x71,
 UART_Incomplete_Msg=	0x72,
 I2C_TLM_Error=	0x73,
 I2C_TC_Buff_Error=	0x74,
 CAN_TC_Buff_Error=	0x75,
 GET_Save_Now=	0x76,
 GET_Save_On_Update =	0x77,
 GET_Save_Periodic=	0x78,
 GET_Period=	0x79,
 Roll=	0x7A,
 Pitch=	0x7B,
 Yaw=	0x7C,
 EstAngularRateX=	0x7D,
 EstAngularRateY=	0x7E,
 EstAngularRateZ=	0x7F,
 ECI_Pos_X=	0x80,
 ECI_Pos_Y=	0x81,
 ECI_Pos_Z=	0x82,
 ECI_Velocity_X=	0x83,
 ECI_Velocity_Y=	0x84,
 ECI_Velocity_Z=	0x85,
 Latitude=	0x86,
 Longitude=	0x87,
 Altitude=	0x88,
 MagFieldVectorX=	0x89,
 MagFieldVectorY=	0x8A,
 MagFieldVectorZ=	0x8B,
 CSSVectorX=	0x8C,
 CSSVectorY=	0x8D,
 CSSVectorZ=	0x8E,
 FSSVectorX=	0x8F,
 FSSVectorY=	0x90,
 FSSVectorZ=	0x91,
 NadirVectorX=	0x92,
 NadirVectorY=	0x93,
 NadirVectorZ=	0x94,
 RateSensorRatesX=	0x95,
 RateSensorRatesY=	0x96,
 RateSensorRatesZ=	0x97,
 CurrWheelSpeedX=	0x98,
 CurrWheelSpeedY=	0x99,
 CurrWheelSpeedZ=	0x9A,
 CurrMTQCommandX=	0x9B,
 CurrMTQCommandY=	0x9C,
 CurrMTQCommandZ=	0x9D,
 CurrWheelSpeedCommandX=	0x9E,
 CurrWheelSpeedCommandY=	0x9F,
 CurrWheelSpeedCommandZ=	0xA0,
 IGRFVectorX=	0xA1,
 IGRFVectorY=	0xA2,
 IGRFVectorZ=	0xA3,
 Modelled_Sun_VectorX=	0xA4,
 Modelled_Sun_VectorY=	0xA5,
 Modelled_Sun_VectorZ=	0xA6,
 EstGyroBiasX=	0xA7,
 EstGyroBiasY=	0xA8,
 EstGyroBiasZ=	0xA9,
 EstInnovationX=	0xAA,
 EstInnovationY=	0xAB,
 EstInnovationZ=	0xAC,
 Q_Err1=	0xAD,
 Q_Err2=	0xAE,
 Q_Err3=	0xAF,
 Q1_Covariance=	0xB0,
 Q2_Covariance=	0xB1,
 Q3_Covariance=	0xB2,
 Angular_Rate_Covar_X=	0xB3,
 Angular_Rate_Covar_Y=	0xB4,
 Angular_Rate_Covar_Z=	0xB5,
 Cam2centroidX=	0xB6,
 Cam2centroidY=	0xB7,
 Cam2CapStatus=	0xB8,
 Cam2DetectResult=	0xB9,
 Cam1centroidX=	0xBA,
 Cam1centroidY=	0xBB,
 Cam1CapStatus=	0xBC,
 Cam1DetectResult=	0xBD,
 RAW_CSS1=	0xBE,
 RAW_CSS2=	0xBF,
 RAW_CSS3=	0xC0,
 RAW_CSS4=	0xC1,
 RAW_CSS5=	0xC2,
 RAW_CSS6=	0xC3,
 RAW_CSS7=	0xC4,
 RAW_CSS8=	0xC5,
 RAW_CSS9=	0xC6,
 RAW_CSS10=	0xC7,
 RAW_Mag_X=	0xC8,
 RAW_Mag_Y=	0xC9,
 RAW_Mag_Z=	0xCA,
 CS1_3V3_Current=	0xCB,
 CS1_SRAM_Current=	0xCC,
 CubeCtrl3V3Current=	0xCD,
 CubeCtrl5Current=	0xCE,
 CubeCtrlVbatCurrent=	0xCF,
 Wheel1Current=	0xD0,
 Wheel2Current=	0xD1,
 Wheel3Current=	0xD2,
 MCU_Temp=	0xD3,
 MTM_Temp=	0xD4,
 RedundMTM_Temp=	0xD5,
 RateSensor_TempX=	0xD6,
 RateSensor_TempY=	0xD7,
 RateSensor_TempZ=	0xD8,
 GPS_Solution_Status=	0xD9,
 Num_of_tracked_GPS_Sat=	0xDA,
 NumofGPSSat_UsedIn_Solution=	0xDB,
 CTforXYZLof_from_GPS=	0xDC,
 CTforRANGElog_from_GPS=	0xDD,
 Resp_Msg_for_GPSLogSetup=	0xDE,
 GPS_Ref_Week=	0xDF,
 GPS_Time_Millisec=	0xE0,
 GPS_ECEF_PosX=	0xE1,
 GPS_ECEF_VeloX=	0xE2,
 GPS_ECEF_PosY=	0xE3,
 GPS_ECEF_VeloY=	0xE4,
 GPS_ECEF_PosZ=	0xE5,
 GPS_ECEF_VeloZ=	0xE6,
 TimetoPerform_ADCSUpdate=	0xE7,
 TimetoPerform_SA_Commu=	0xE8,
 TimetoExecute_SGP4_Propagator=	0xE9,
 TimetoExecute_IGRF_Model=	0xEA,
 GET_CubeCtrlSignalPowerSelect=	0xEB,
 GET_CubeCtrlMotorPowerSelect=	0xEC,
 GET_CubeSense1PowerSelect=	0xED,
 GET_CubeSense2PowerSelect=	0xEE,
 GET_CubeStarPowerSelect=	0xEF,
 GET_CubeWheel1PowerSelect=	0xF0,
 GET_CubeWheel2PowerSelect=	0xF1,
 GET_CubeWheel3PowerSelect=	0xF2,
 GET_MotorPower=	0xF3,
 GET_GPSPower=	0xF4,
 CubeStarCurrent=	0xF5,
 MTQCurrent=	0xF6,
 CubeStarMCU_Temp=	0xF7,
 GET_Cmd_Roll=	0xF8,
 GET_Cmd_Pitch=	0xF9,
 GET_Cmd_Yaw=	0xFA,
 FineEstXAngularRate=	0xFB,
 FineEstYAngularRate=	0xFC,
 FineEstZAngularRate=	0xFD,
 GET_Cam1_Mounting_Trans_Alpha =	0xFE,
 GET_Cam1_Mounting_Trans_Beta =	0xFF,
 GET_Cam1_Mounting_Trans_Gamma=	0x100,
 GET_Cam1_DetectThreshold=	0x101,
 GET_Cam1_AutoAdjust=	0x102,
 GET_Cam1_Exposure_Time=	0x103,
 GET_Cam1_Boresight_X=	0x104,
 GET_Cam1_Boresight_Y=	0x105,
 GET_Cam2_Mounting_Trans_Alpha =	0x106,
 GET_Cam2_Mounting_Trans_Beta =	0x107,
 GET_Cam2_Mounting_Trans_Gamma=	0x108,
 GET_Cam2_DetectThreshold=	0x109,
 GET_Cam2_AutoAdjust=	0x10A,
 GET_Cam2_Exposure_Time=	0x10B,
 GET_Cam2_Boresight_X=	0x10C,
 GET_Cam2_Boresight_Y=	0x10D,
 GET_Nadir_Max_Devia_Percent=	0x10E,
 GET_Nadir_Max_BadEdges=	0x10F,
 GET_Nadir_Max_Radius=	0x110,
 GET_Nadir_Min_Radius=	0x111,
 GET_Cam1_MinX_Area1=	0x112,
 GET_Cam1_MaxX_Area1=	0x113,
 GET_Cam1_MinY_Area1=	0x114,
 GET_Cam1_MaxY_Area1=	0x115,
 GET_Cam1_MinX_Area2=	0x116,
 GET_Cam1_MaxX_Area2=	0x117,
 GET_Cam1_MinY_Area2=	0x118,
 GET_Cam1_MaxY_Area2=	0x119,
 GET_Cam1_MinX_Area3=	0x11A,
 GET_Cam1_MaxX_Area3=	0x11B,
 GET_Cam1_MinY_Area3=	0x11C,
 GET_Cam1_MaxY_Area3=	0x11D,
 GET_Cam1_MinX_Area4=	0x11E,
 GET_Cam1_MaxX_Area4=	0x11F,
 GET_Cam1_MinY_Area4=	0x120,
 GET_Cam1_MaxY_Area4=	0x121,
 GET_Cam1_MinX_Area5=	0x122,
 GET_Cam1_MaxX_Area5=	0x123,
 GET_Cam1_MinY_Area5=	0x124,
 GET_Cam1_MaxY_Area5=	0x125,
 GET_Cam2_MinX_Area1=	0x126,
 GET_Cam2_MaxX_Area1=	0x127,
 GET_Cam2_MinY_Area1=	0x128,
 GET_Cam2_MaxY_Area1=	0x129,
 GET_Cam2_MinX_Area2=	0x12A,
 GET_Cam2_MaxX_Area2=	0x12B,
 GET_Cam2_MinY_Area2=	0x12C,
 GET_Cam2_MaxY_Area2=	0x12D,
 GET_Cam2_MinX_Area3=	0x12E,
 GET_Cam2_MaxX_Area3=	0x12F,
 GET_Cam2_MinY_Area3=	0x130,
 GET_Cam2_MaxY_Area3=	0x131,
 GET_Cam2_MinX_Area4=	0x132,
 GET_Cam2_MaxX_Area4=	0x133,
 GET_Cam2_MinY_Area4=	0x134,
 GET_Cam2_MaxY_Area4=	0x135,
 GET_Cam2_MinX_Area5=	0x136,
 GET_Cam2_MaxX_Area5=	0x137,
 GET_Cam2_MinY_Area5=	0x138,
 GET_Cam2_MaxY_Area5=	0x139,
 GET_MTMMountAlpha=	0x13A,
 GET_MTMMountBeta=	0x13B,
 GET_MTMMountGamma=	0x13C,
 GET_MTM_Ch1Offset=	0x13D,
 GET_MTM_Ch2Offset=	0x13E,
 GET_MTM_Ch3Offset=	0x13F,
 GET_MTM_S11=	0x140,
 GET_MTM_S22=	0x141,
 GET_MTM_S33=	0x142,
 GET_MTM_S12=	0x143,
 GET_MTM_S13=	0x144,
 GET_MTM_S21=	0x145,
 GET_MTM_S23=	0x146,
 GET_MTM_S31=	0x147,
 GET_MTM_S32=	0x148,
 GET_Inclination=	0x149,
 GET_Eccentricity=	0x14A,
 GET_RightAscensionOfTheAscendingNode=	0x14B,
 GET_ArgumentOfPerigee=	0x14C,
 GET_BStarDragTerm=	0x14D,
 GET_MeanMotion=	0x14E,
 GET_MeanAnomaly=	0x14F,
 GET_Epoch=	0x150,
 GET_Detumb_SpinGain=	0x151,
 GET_Detumb_DampGain=	0x152,
 GET_Ref_SpinRate=	0x153,
 GET_FastBdot_DetumbGain=	0x154,
 GET_Ymoment_CtrlGain=	0x155,
 GET_Ymoment_NDampGain=	0x156,
 GET_Ymoment_PGain=	0x157,
 GET_Ymoment_DGain=	0x158,
 GET_RefWheel_Moment=	0x159,
 Raw_RateX=	0x15A,
 Raw_RateY=	0x15B,
 Raw_RateZ=	0x15C,
 GET_Rwheel_PGain=	0x15D,
 GET_Rwheel_DGain=	0x15E,
 GET_YWheelBiasMoment=	0x15F,
 GET_SunPointFacet=	0x160,
 GET_AutoCtrl_TransitDueWheelErr=	0x161,
 Est_Q1=	0x162,
 Est_Q2=	0x163,
 Est_Q3=	0x164,
 ECEF_Pos_X=	0x165,
 ECEF_Pos_Y=	0x166,
 ECEF_Pos_Z=	0x167,
 TimeSince_IterationStart=	0x168,
 Current_ExePoint=	0x169,
 GET_M_Of_IXX=	0x16A,
 GET_M_Of_IYY=	0x16B,
 GET_M_Of_IZZ=	0x16C,
 GET_M_Of_IXY=	0x16D,
 GET_M_Of_IXZ=	0x16E,
 GET_M_Of_IYZ=	0x16F,
 GET_MTMRateFilterSysNoise=	0x170,
 GET_EKFSysNoise=	0x171,
 GET_CSSMeasureNoise=	0x172,
 GET_FSSMeasureNoise=	0x173,
 GET_NadirSensMeasureNoise=	0x174,
 GET_MTMMeasureNoise=	0x175,
 GET_STMeasureNoise=	0x176,
 GET_UseFSS=	0x177,
 GET_UseNadirSens=	0x178,
 GET_UseCSS=	0x179,
 GET_UseST=	0x17A,
 GET_NadirSensterminatortest=	0x17B,
 GET_AutoMTMRecovery=	0x17C,
 GET_MTMMode=	0x17D,
 GET_MTMSelectForRAW_MTM_TLM=	0x17E,
 GET_AutoEstTransitDueToRateSensErr=	0x17F,
 GET_Cam1Cam2SampPeriod=	0x180,
 OrbitPara_Are_Invalid=	0x181,
 Config_Is_Invalid=	0x182,
 CtrlMode_ChangeisntAllowed=	0x183,
 Est_ChangeisntAllowed=	0x184,
 CurrMTMSampMode=	0x185,
 ModelAndMeaMF_DiffersInSize=	0x186,
 Node_Recovery_Err=	0x187,
 CS1_Runtime_Err=	0x188,
 CS2_Runtime_Err=	0x189,
 CubeCtrl_Signal_Runtime_Err=	0x18A,
 CubeCtrl_Motor_Runtime_Err=	0x18B,
 CubeW1_Runtime_Err=	0x18C,
 CubeW2_Runtime_Err=	0x18D,
 CubeW3_Runtime_Err=	0x18E,
 CubeStar_Runtime_Err=	0x18F,
 MTM_Err=	0x190,
 RateSensor_Failure=	0x191,
 GET_ACP_Type=	0x192,
 GET_Special_Ctrl_Select=	0x193,
 GET_CubeCtrlSig_Ver=	0x194,
 GET_CubeCtrlMotor_Ver=	0x195,
 GET_CS1_Ver=	0x196,
 GET_CS2_Ver=	0x197,
 GET_CS1_CamType=	0x198,
 GET_CS2_CamType=	0x199,
 GET_CubeStar_Ver=	0x19A,
 GET_GPS_Type=	0x19B,
 GET_Redun_MTM_Includ=	0x19C,
 GET_MTQX_MaxDipole=	0x19D,
 GET_MTQY_MaxDipole=	0x19E,
 GET_MTQZ_MaxDipole=	0x19F,
 GET_MTQ_Ontime_Res=	0x1A0,
 GET_MTQ_Max_OnTime=	0x1A1,
 GET_RWX_MaxTorque=	0x1A2,
 GET_RWY_MaxTorque=	0x1A3,
 GET_RWZ_MaxTorque=	0x1A4,
 GET_RWX_MaxMomentum=	0x1A5,
 GET_RWY_MaxMomentum=	0x1A6,
 GET_RWZ_MaxMomentum=	0x1A7,
 GET_RWX_Inertia=	0x1A8,
 GET_RWY_Inertia=	0x1A9,
 GET_RWZ_Inertia=	0x1AA,
 GET_RW_Torque_Increment=	0x1AB,
 GET_Prim_MTMXBias_d1=	0x1AC,
 GET_Prim_MTMYBias_d1=	0x1AD,
 GET_Prim_MTMZBias_d1=	0x1AE,
 GET_Prim_MTMXBias_d2=	0x1AF,
 GET_Prim_MTMYBias_d2=	0x1B0,
 GET_Prim_MTMZBias_d2=	0x1B1,
 GET_Prim_MTMXSens_s1=	0x1B2,
 GET_Prim_MTMYSens_s1=	0x1B3,
 GET_Prim_MTMZSens_s1=	0x1B4,
 GET_Prim_MTMXSens_s2=	0x1B5,
 GET_Prim_MTMYSens_s2=	0x1B6,
 GET_Prim_MTMZSens_s2=	0x1B7,
 GET_Redun_MTMXBias_d1=	0x1B8,
 GET_Redun_MTMYBias_d1=	0x1B9,
 GET_Redun_MTMZBias_d1=	0x1BA,
 GET_Redun_MTMXBias_d2=	0x1BB,
 GET_Redun_MTMYBias_d2=	0x1BC,
 GET_Redun_MTMZBias_d2=	0x1BD,
 GET_Redun_MTMXSens_s1=	0x1BE,
 GET_Redun_MTMYSens_s1=	0x1BF,
 GET_Redun_MTMZSens_s1=	0x1C0,
 GET_Redun_MTMXSens_s2=	0x1C1,
 GET_Redun_MTMYSens_s2=	0x1C2,
 GET_Redun_MTMZSens_s2=	0x1C3,
 GET_CCSignal_En_Port=	0x1C4,
 GET_CCSignal_En_Pin=	0x1C5,
 GET_CCMotor_En_Port=	0x1C6,
 GET_CCMotor_En_Pin=	0x1C7,
 GET_CCCommon_En_Port=	0x1C8,
 GET_CCCommon_En_Pin=	0x1C9,
 GET_CS1_En_Port=	0x1CA,
 GET_CS1_En_Pin=	0x1CB,
 GET_CS2_En_Port=	0x1CC,
 GET_CS2_En_Pin=	0x1CD,
 GET_CubeStar_En_Port=	0x1CE,
 GET_CubeStar_En_Pin=	0x1CF,
 GET_CW1_En_Port=	0x1D0,
 GET_CW1_En_Pin=	0x1D1,
 GET_CW2_En_Port=	0x1D2,
 GET_CW2_En_Pin=	0x1D3,
 GET_CW3_En_Port=	0x1D4,
 GET_CW3_En_Pin=	0x1D5,
 GET_Incl_Coeff=	0x1D6,
 GET_Raan_Coeff=	0x1D7,
 GET_Ecc_Coeff=	0x1D8,
 GET_Aop_Coeff=	0x1D9,
 GET_Time_Coeff=	0x1DA,
 GET_Pos_Coeff=	0x1DB,
 GET_Max_Pos_Err=	0x1DC,
 GET_ASGP4_Filter=	0x1DD,
 GET_xp_Coeff=	0x1DE,
 GET_yp_Coeff=	0x1DF,
 GET_GPS_RollOver=	0x1E0,
 GET_Pos_sd=	0x1E1,
 GET_Velo_sd =	0x1E2,
 GET_Min_satellites=	0x1E3,
 GET_Time_Gain=	0x1E4,
 GET_Max_Lag=	0x1E5,
 GET_Min_Samples=	0x1E6,
 ASGP4_Complete=	0x1E7,
 ASGP4_Error=	0x1E8,
 ASGP4_Epoch=	0x1E9,
 ASGP4_Inclination=	0x1EA,
 ASGP4_RAAN=	0x1EB,
 ASGP4_ECC=	0x1EC,
 ASGP4_AOP=	0x1ED,
 ASGP4_MA=	0x1EE,
 ASGP4_MM=	0x1EF,
 ASGP4_Bstar=	0x1F0,
 CS2_3V3_Current=	0x1F1,
 CS2_SRAM_Current=	0x1F2,
 Percentage_Complete=	0x1F3,
 Img_Cap_Status=	0x1F4,
 Format_Busy=	0x1F5,
 Erase_All_Busy=	0x1F6,
 GET_Log1_Selection=	0x1F7,
 GET_Log1_Period =	0x1F8,
 GET_Log1_Destination=	0x1F9,
 GET_Log2_Selection=	0x1FA,
 GET_Log2_Period =	0x1FB,
 GET_Log2_Destination=	0x1FC,
 GET_LogUART_Selection=	0x1FD,
 GET_LogUART_Period =	0x1FE,
 GET_Inertial_RefX=	0x1FF,
 GET_Inertial_RefY=	0x200,
 GET_Inertial_RefZ=	0x201,
 LastTCID=	0x202,
 ProcessedFlag=	0x203,
 TCErrStatus=	0x204,
 TCParaErrIndex=	0x205,
 FileDLBuff_Packet_Buff=	0x206,
 FileDLBuff_FileBytes=	0x207,
 DL_Block_Ready=	0x208,
 DL_Block_ParaError=	0x209,
 DL_Block_CRC16_Checksum=	0x20A,
 DL_Block_Length=	0x20B,
 File_Infor_Type=	0x20C,
 File_Infor_Busy_Updating=	0x20D,
 File_Infor_Ctr=	0x20E,
 File_Infor_Size=	0x20F,
 File_Infor_DateTime=	0x210,
 File_Infor_CRC16_Checksum=	0x211,
 Init_Upload_Complete=	0x212,
 FinalUL_Block_Busy=	0x213,
 FinalUL_Block_Error=	0x214,
 FileUL_Block_CRC16_Checksum=	0x215,
 GET_FileUL_HoleMap_1=	0x216,
 GET_FileUL_HoleMap_2=	0x217,
 GET_FileUL_HoleMap_3=	0x218,
 GET_FileUL_HoleMap_4=	0x219,
 GET_FileUL_HoleMap_5=	0x21A,
 GET_FileUL_HoleMap_6=	0x21B,
 GET_FileUL_HoleMap_7=	0x21C,
 GET_FileUL_HoleMap_8=	0x21D,
 Reset_Magic_number=	0x21E,
 SET_CurrentUnixTime=	0x21F,
 SET_Milliseconds=	0x220,
 SET_Cache_Enable_state=	0x221,
 ResetLogPointer=	0x222,
 AdvanceLogPointer=	0x223,
 ResetBootReg=	0x224,
 DeployMTMTimeout=	0x225,
 SET_SRAM_Scrub_Size=	0x226,
 SET_Save_Now=	0x227,
 SET_Save_On_Update =	0x228,
 SET_Save_Periodic=	0x229,
 SET_Period=	0x22A,
 ADCSRunMode_bit=	0x22B,
 SET_CubeCtrlSignalPowerSelect=	0x22C,
 SET_CubeCtrlMotorPowerSelect=	0x22D,
 SET_CubeSense1PowerSelect=	0x22E,
 SET_CubeSense2PowerSelect=	0x22F,
 SET_CubeStarPowerSelect=	0x230,
 SET_CubeWheel1PowerSelect=	0x231,
 SET_CubeWheel2PowerSelect=	0x232,
 SET_CubeWheel3PowerSelect=	0x233,
 SET_MotorPower=	0x234,
 SET_GPSPower=	0x235,
 Clear_Err_Flag=	0x236,
 Clear_HK_Err_Flag=	0x237,
 SET_CtrlMode=	0x238,
 CtrlModeTimeout=	0x239,
 AttEstMode_bit=	0x23A,
 SET_Cmd_Roll=	0x23B,
 SET_Cmd_Pitch=	0x23C,
 SET_Cmd_Yaw=	0x23D,
 CommandMTQX=	0x23E,
 CommandMTQY=	0x23F,
 CommandMTQZ=	0x240,
 CommandWheelXSpeed=	0x241,
 CommandWheelYSpeed=	0x242,
 CommandWheelZSpeed=	0x243,
 Trig_ADCSLoop=	0x244,
 Iter_Unix_Time=	0x245,
 Iter_CSSRaw_1=	0x246,
 Iter_CSSRaw_2=	0x247,
 Iter_CSSRaw_3=	0x248,
 Iter_CSSRaw_4=	0x249,
 Iter_CSSRaw_5=	0x24A,
 Iter_CSSRaw_6=	0x24B,
 Iter_CSSRaw_7=	0x24C,
 Iter_CSSRaw_8=	0x24D,
 Iter_CSSRaw_9=	0x24E,
 Iter_CSSRaw_10=	0x24F,
 Iter_Cam1Raw_X=	0x250,
 Iter_Cam1Raw_Y=	0x251,
 Iter_Cam1Busy=	0x252,
 Iter_Cam1Result=	0x253,
 Iter_Cam2Raw_X=	0x254,
 Iter_Cam2Raw_Y=	0x255,
 Iter_Cam2Busy=	0x256,
 Iter_Cam2Result=	0x257,
 Iter_MagRaw_X=	0x258,
 Iter_MagRaw_Y=	0x259,
 Iter_MagRaw_Z=	0x25A,
 Iter_RateRaw_X=	0x25B,
 Iter_RateRaw_Y=	0x25C,
 Iter_RateRaw_Z=	0x25D,
 Iter_WheelRaw_X=	0x25E,
 Iter_WheelRaw_Y=	0x25F,
 Iter_WheelRaw_Z=	0x260,
 Iter_Star1Camera_X =	0x261,
 Iter_Star1Camera_Y=	0x262,
 Iter_Star1Camera_Z=	0x263,
 Iter_Star1Inertial_X=	0x264,
 Iter_Star1Inertial_Y=	0x265,
 Iter_Star1Inertial_Z=	0x266,
 Iter_Star2Camera_X =	0x267,
 Iter_Star2Camera_Y=	0x268,
 Iter_Star2Camera_Z=	0x269,
 Iter_Star2Inertial_X=	0x26A,
 Iter_Star2Inertial_Y=	0x26B,
 Iter_Star2Inertial_Z=	0x26C,
 Iter_Star3Camera_X =	0x26D,
 Iter_Star3Camera_Y=	0x26E,
 Iter_Star3Camera_Z=	0x26F,
 Iter_Star3Inertial_X=	0x270,
 Iter_Star3Inertial_Y=	0x271,
 Iter_Star3Inertial_Z=	0x272,
 Iter_Gps_Solution_Status=	0x273,
 Iter_GPS_Ref_Week=	0x274,
 Iter_GPS_Time_Millisec=	0x275,
 Iter_ECEF_Pos_X=	0x276,
 Iter_ECEF_Velo_X=	0x277,
 Iter_ECEF_Pos_Y=	0x278,
 Iter_ECEF_Velo_Y=	0x279,
 Iter_ECEF_Pos_Z=	0x27A,
 Iter_ECEF_Velo_Z=	0x27B,
 Iter_XposStandard_Devia=	0x27C,
 Iter_YposStandard_Devia=	0x27D,
 Iter_ZposStandard_Devia=	0x27E,
 Iter_XvelStandard_Devia=	0x27F,
 Iter_YvelStandard_Devia=	0x280,
 Iter_ZvelStandard_Devia=	0x281,
 SET_MTQ1config=	0x282,
 SET_MTQ2config=	0x283,
 SET_MTQ3config=	0x284,
 SET_RW1_Config=	0x285,
 SET_RW2_Config=	0x286,
 SET_RW3_Config=	0x287,
 SET_RW4_Config=	0x288,
 SET_Gyro1Config=	0x289,
 SET_Gyro2Config=	0x28A,
 SET_Gyro3Config=	0x28B,
 SET_XRateSensorOffset=	0x28C,
 SET_YRateSensorOffset=	0x28D,
 SET_ZRateSensorOffset=	0x28E,
 SET_RateSensorMult=	0x28F,
 SET_CSS1_Config=	0x290,
 SET_CSS2_Config=	0x291,
 SET_CSS3_Config=	0x292,
 SET_CSS4_Config=	0x293,
 SET_CSS5_Config=	0x294,
 SET_CSS6_Config=	0x295,
 SET_CSS7_Config=	0x296,
 SET_CSS8_Config=	0x297,
 SET_CSS9_Config=	0x298,
 SET_CSS10_Config=	0x299,
 SET_CSS1_Rela_Scale=	0x29A,
 SET_CSS2_Rela_Scale=	0x29B,
 SET_CSS3_Rela_Scale=	0x29C,
 SET_CSS4_Rela_Scale=	0x29D,
 SET_CSS5_Rela_Scale=	0x29E,
 SET_CSS6_Rela_Scale=	0x29F,
 SET_CSS7_Rela_Scale=	0x2A0,
 SET_CSS8_Rela_Scale=	0x2A1,
 SET_CSS9_Rela_Scale=	0x2A2,
 SET_CSS10_Rela_Scale=	0x2A3,
 SET_CSS_Threshold=	0x2A4,
 SET_Cam1_Mounting_Trans_Alpha =	0x2A5,
 SET_Cam1_Mounting_Trans_Beta =	0x2A6,
 SET_Cam1_Mounting_Trans_Gamma=	0x2A7,
 SET_Cam1_DetectThreshold=	0x2A8,
 SET_Cam1_AutoAdjust=	0x2A9,
 SET_Cam1_Exposure_Time=	0x2AA,
 SET_Cam1_Boresight_X=	0x2AB,
 SET_Cam1_Boresight_Y=	0x2AC,
 SET_Cam2_Mounting_Trans_Alpha =	0x2AD,
 SET_Cam2_Mounting_Trans_Beta =	0x2AE,
 SET_Cam2_Mounting_Trans_Gamma=	0x2AF,
 SET_Cam2_DetectThreshold=	0x2B0,
 SET_Cam2_AutoAdjust=	0x2B1,
 SET_Cam2_Exposure_Time=	0x2B2,
 SET_Cam2_Boresight_X=	0x2B3,
 SET_Cam2_Boresight_Y=	0x2B4,
 SET_Nadir_Max_Devia_Percent=	0x2B5,
 SET_Nadir_Max_BadEdges=	0x2B6,
 SET_Nadir_Max_Radius=	0x2B7,
 SET_Nadir_Min_Radius=	0x2B8,
 SET_Cam1_MinX_Area1=	0x2B9,
 SET_Cam1_MaxX_Area1=	0x2BA,
 SET_Cam1_MinY_Area1=	0x2BB,
 SET_Cam1_MaxY_Area1=	0x2BC,
 SET_Cam1_MinX_Area2=	0x2BD,
 SET_Cam1_MaxX_Area2=	0x2BE,
 SET_Cam1_MinY_Area2=	0x2BF,
 SET_Cam1_MaxY_Area2=	0x2C0,
 SET_Cam1_MinX_Area3=	0x2C1,
 SET_Cam1_MaxX_Area3=	0x2C2,
 SET_Cam1_MinY_Area3=	0x2C3,
 SET_Cam1_MaxY_Area3=	0x2C4,
 SET_Cam1_MinX_Area4=	0x2C5,
 SET_Cam1_MaxX_Area4=	0x2C6,
 SET_Cam1_MinY_Area4=	0x2C7,
 SET_Cam1_MaxY_Area4=	0x2C8,
 SET_Cam1_MinX_Area5=	0x2C9,
 SET_Cam1_MaxX_Area5=	0x2CA,
 SET_Cam1_MinY_Area5=	0x2CB,
 SET_Cam1_MaxY_Area5=	0x2CC,
 SET_Cam2_MinX_Area1=	0x2CD,
 SET_Cam2_MaxX_Area1=	0x2CE,
 SET_Cam2_MinY_Area1=	0x2CF,
 SET_Cam2_MaxY_Area1=	0x2D0,
 SET_Cam2_MinX_Area2=	0x2D1,
 SET_Cam2_MaxX_Area2=	0x2D2,
 SET_Cam2_MinY_Area2=	0x2D3,
 SET_Cam2_MaxY_Area2=	0x2D4,
 SET_Cam2_MinX_Area3=	0x2D5,
 SET_Cam2_MaxX_Area3=	0x2D6,
 SET_Cam2_MinY_Area3=	0x2D7,
 SET_Cam2_MaxY_Area3=	0x2D8,
 SET_Cam2_MinX_Area4=	0x2D9,
 SET_Cam2_MaxX_Area4=	0x2DA,
 SET_Cam2_MinY_Area4=	0x2DB,
 SET_Cam2_MaxY_Area4=	0x2DC,
 SET_Cam2_MinX_Area5=	0x2DD,
 SET_Cam2_MaxX_Area5=	0x2DE,
 SET_Cam2_MinY_Area5=	0x2DF,
 SET_Cam2_MaxY_Area5=	0x2E0,
 SET_MTMMountAlpha=	0x2E1,
 SET_MTMMountBeta=	0x2E2,
 SET_MTMMountGamma=	0x2E3,
 SET_MTM_Ch1Offset=	0x2E4,
 SET_MTM_Ch2Offset=	0x2E5,
 SET_MTM_Ch3Offset=	0x2E6,
 SET_MTM_S11=	0x2E7,
 SET_MTM_S22=	0x2E8,
 SET_MTM_S33=	0x2E9,
 SET_MTM_S12=	0x2EA,
 SET_MTM_S13=	0x2EB,
 SET_MTM_S21=	0x2EC,
 SET_MTM_S23=	0x2ED,
 SET_MTM_S31=	0x2EE,
 SET_MTM_S32=	0x2EF,
 SET_MTMRateFilterSysNoise=	0x2F0,
 SET_EKFSysNoise=	0x2F1,
 SET_CSSMeasureNoise=	0x2F2,
 SET_FSSMeasureNoise=	0x2F3,
 SET_NadirSensMeasureNoise=	0x2F4,
 SET_MTMMeasureNoise=	0x2F5,
 SET_STMeasureNoise=	0x2F6,
 SET_UseFSS=	0x2F7,
 SET_UseNadirSens=	0x2F8,
 SET_UseCSS=	0x2F9,
 SET_UseST=	0x2FA,
 SET_NadirSensterminatortest=	0x2FB,
 SET_AutoMTMRecovery=	0x2FC,
 SET_MTMMode=	0x2FD,
 SET_MTMSelectForRAW_MTM_TLM=	0x2FE,
 SET_AutoEstTransitDueToRateSensErr=	0x2FF,
 SET_Cam1Cam2SampPeriod=	0x300,
 SET_Incl_Coeff=	0x301,
 SET_Raan_Coeff=	0x302,
 SET_Ecc_Coeff=	0x303,
 SET_Aop_Coeff=	0x304,
 SET_Time_Coeff=	0x305,
 SET_Pos_Coeff=	0x306,
 SET_Max_Pos_Err=	0x307,
 SET_ASGP4_Filter=	0x308,
 SET_xp_Coeff=	0x309,
 SET_yp_Coeff=	0x30A,
 SET_GPS_RollOver=	0x30B,
 SET_Pos_sd=	0x30C,
 SET_Velo_sd =	0x30D,
 SET_Min_satellites=	0x30E,
 SET_Time_Gain=	0x30F,
 SET_Max_Lag=	0x310,
 SET_ACP_Type=	0x311,
 SET_Special_Ctrl_Select=	0x312,
 SET_CubeCtrlSig_Ver=	0x313,
 SET_CubeCtrlMotor_Ver=	0x314,
 SET_CS1_Ver=	0x315,
 SET_CS2_Ver=	0x316,
 SET_CS1_CamType=	0x317,
 SET_CS2_CamType=	0x318,
 SET_CubeStar_Ver=	0x319,
 SET_GPS_Type=	0x31A,
 SET_Redun_MTM_Includ=	0x31B,
 SET_MTQX_MaxDipole=	0x31C,
 SET_MTQY_MaxDipole=	0x31D,
 SET_MTQZ_MaxDipole=	0x31E,
 SET_MTQ_Ontime_Res=	0x31F,
 SET_MTQ_Max_OnTime=	0x320,
 SET_RWX_MaxTorque=	0x321,
 SET_RWY_MaxTorque=	0x322,
 SET_RWZ_MaxTorque=	0x323,
 SET_RWX_MaxMomentum=	0x324,
 SET_RWY_MaxMomentum=	0x325,
 SET_RWZ_MaxMomentum=	0x326,
 SET_RWX_Inertia=	0x327,
 SET_RWY_Inertia=	0x328,
 SET_RWZ_Inertia=	0x329,
 SET_RW_Torque_Increment=	0x32A,
 SET_Prim_MTMXBias_d1=	0x32B,
 SET_Prim_MTMYBias_d1=	0x32C,
 SET_Prim_MTMZBias_d1=	0x32D,
 SET_Prim_MTMXBias_d2=	0x32E,
 SET_Prim_MTMYBias_d2=	0x32F,
 SET_Prim_MTMZBias_d2=	0x330,
 SET_Prim_MTMXSens_s1=	0x331,
 SET_Prim_MTMYSens_s1=	0x332,
 SET_Prim_MTMZSens_s1=	0x333,
 SET_Prim_MTMXSens_s2=	0x334,
 SET_Prim_MTMYSens_s2=	0x335,
 SET_Prim_MTMZSens_s2=	0x336,
 SET_Redun_MTMXBias_d1=	0x337,
 SET_Redun_MTMYBias_d1=	0x338,
 SET_Redun_MTMZBias_d1=	0x339,
 SET_Redun_MTMXBias_d2=	0x33A,
 SET_Redun_MTMYBias_d2=	0x33B,
 SET_Redun_MTMZBias_d2=	0x33C,
 SET_Redun_MTMXSens_s1=	0x33D,
 SET_Redun_MTMYSens_s1=	0x33E,
 SET_Redun_MTMZSens_s1=	0x33F,
 SET_Redun_MTMXSens_s2=	0x340,
 SET_Redun_MTMYSens_s2=	0x341,
 SET_Redun_MTMZSens_s2=	0x342,
 SET_CCSignal_En_Port=	0x343,
 SET_CCSignal_En_Pin=	0x344,
 SET_CCMotor_En_Port=	0x345,
 SET_CCMotor_En_Pin=	0x346,
 SET_CCCommon_En_Port=	0x347,
 SET_CCCommon_En_Pin=	0x348,
 SET_CS1_En_Port=	0x349,
 SET_CS1_En_Pin=	0x34A,
 SET_CS2_En_Port=	0x34B,
 SET_CS2_En_Pin=	0x34C,
 SET_CubeStar_En_Port=	0x34D,
 SET_CubeStar_En_Pin=	0x34E,
 SET_CW1_En_Port=	0x34F,
 SET_CW1_En_Pin=	0x350,
 SET_CW2_En_Port=	0x351,
 SET_CW2_En_Pin=	0x352,
 SET_CW3_En_Port=	0x353,
 SET_CW3_En_Pin=	0x354,
 SET_ASGP4_RunMode=	0x355,
 Trig_ASGP4=	0x356,
 Format_SDcard_MagicNum=	0x357,
 SET_Inertial_RefX=	0x358,
 SET_Inertial_RefY=	0x359,
 SET_Inertial_RefZ=	0x35A,
 SET_Detumb_SpinGain=	0x35B,
 SET_Detumb_DampGain=	0x35C,
 SET_Ref_SpinRate=	0x35D,
 SET_FastBdot_DetumbGain=	0x35E,
 SET_Ymoment_CtrlGain=	0x35F,
 SET_Ymoment_NDampGain=	0x360,
 SET_Ymoment_PGain=	0x361,
 SET_Ymoment_DGain=	0x362,
 SET_RefWheel_Moment=	0x363,
 SET_Rwheel_PGain=	0x364,
 SET_Rwheel_DGain=	0x365,
 SET_YWheelBiasMoment=	0x366,
 SET_SunPointFacet=	0x367,
 SET_AutoCtrl_TransitDueWheelErr=	0x368,
 SET_M_Of_IXX=	0x369,
 SET_M_Of_IYY=	0x36A,
 SET_M_Of_IZZ=	0x36B,
 SET_M_Of_IXY=	0x36C,
 SET_M_Of_IXZ=	0x36D,
 SET_M_Of_IYZ=	0x36E,
 SET_Inclination=	0x36F,
 SET_Eccentricity=	0x370,
 SET_RightAscensionOfTheAscendingNode=	0x371,
 SET_ArgumentOfPerigee=	0x372,
 SET_BStarDragTerm=	0x373,
 SET_MeanMotion=	0x374,
 SET_MeanAnomaly=	0x375,
 SET_Epoch=	0x376,
 SET_MTM_Mode=	0x377,
 Source_File_CT=	0x378,
 QualityFactor=	0x379,
 WhiteBalance=	0x37A,
 Save_Config_bit=	0x37B,
 Save_OrbitPara_bit=	0x37C,
 Camera_Select=	0x37D,
 Image_Size=	0x37E,
 Boot_Prog_Index=	0x37F,
 RunSelectedProg=	0x380,
 ReadProgInfor=	0x381,
 Source_Prog_Index=	0x382,
 Bootloader_overwrite_flag=	0x383,
 SET_Log1_Selection=	0x384,
 SET_Log1_Period =	0x385,
 SET_Log1_Destination=	0x386,
 SET_Log2_Selection=	0x387,
 SET_Log2_Period =	0x388,
 SET_Log2_Destination=	0x389,
 SET_LogUART_Selection=	0x38A,
 SET_LogUART_Period =	0x38B,
 Erase_File_Type=	0x38C,
 File_Counter=	0x38D,
 Erase_All=	0x38E,
 DL_File_type=	0x38F,
 DL_Counter=	0x390,
 DL_offset=	0x391,
 DL_BlockLength=	0x392,
 AdFile_List_ReadP=	0x393,
 InitFile_Upload_Desti=	0x394,
 InitFile_Upload_BlockSize=	0x395,
 Packet_Number=	0x396,
 FileUpload_Bytes=	0x397,
 FinalUpload_Desti=	0x398,
 FinalUpload_Offset=	0x399,
 FinalUpload_BlockSize=	0x39A,
 Reset_Upload_Block=	0x39B,
 ResetFile_List_ReadP=	0x39C,
 InitDLBurst_Msg_Length=	0x39D,
 InitDLBurst_Ignore_HoleMap=	0x39E,
 SET_FileUL_HoleMap_1=	0x39F,
 SET_FileUL_HoleMap_2=	0x3A0,
 SET_FileUL_HoleMap_3=	0x3A1,
 SET_FileUL_HoleMap_4=	0x3A2,
 SET_FileUL_HoleMap_5=	0x3A3,
 SET_FileUL_HoleMap_6=	0x3A4,
 SET_FileUL_HoleMap_7=	0x3A5,
 SET_FileUL_HoleMap_8=	0x3A6,
};
void cube_bit_reg_setup(){
	cube_bit_reg[Node_type].init_bit(&cube_reg[CAR_Identi],0,0,bits_len_union_t::_8bits_,_8UL,Node_type_note_t,sizeof(Node_type_note_t));
	cube_bit_reg[Interface_Version].init_bit(&cube_reg[CAR_Identi],1,0,bits_len_union_t::_8bits_,_8UL,Interface_Version_note_t,sizeof(Interface_Version_note_t));
	cube_bit_reg[Firmware_vr_Major].init_bit(&cube_reg[CAR_Identi],2,0,bits_len_union_t::_8bits_,_8UL,Firmware_vr_Major_note_t,sizeof(Firmware_vr_Major_note_t));
	cube_bit_reg[Firmware_vr_Minor].init_bit(&cube_reg[CAR_Identi],3,0,bits_len_union_t::_8bits_,_8UL,Firmware_vr_Minor_note_t,sizeof(Firmware_vr_Minor_note_t));
	cube_bit_reg[Runtime_s].init_bit(&cube_reg[CAR_Identi],4,0,bits_len_union_t::_16bits_,_16UL,Runtime_s_note_t,sizeof(Runtime_s_note_t));
	cube_bit_reg[Runtime_ms].init_bit(&cube_reg[CAR_Identi],6,0,bits_len_union_t::_16bits_,_16UL,Runtime_ms_note_t,sizeof(Runtime_ms_note_t));
	cube_bit_reg[Cause_of_MCU_Reset].init_bit(&cube_reg[CAR_BootandRun_Status],0,0,bits_len_union_t::_4bits_,_8UL,Cause_of_MCU_Reset_note_t,sizeof(Cause_of_MCU_Reset_note_t));
	cube_bit_reg[Boot_Cause].init_bit(&cube_reg[CAR_BootandRun_Status],0,4,bits_len_union_t::_4bits_,_8UL,Boot_Cause_note_t,sizeof(Boot_Cause_note_t));
	cube_bit_reg[Boot_Counter].init_bit(&cube_reg[CAR_BootandRun_Status],1,0,bits_len_union_t::_16bits_,_16UL,Boot_Counter_note_t,sizeof(Boot_Counter_note_t));
	cube_bit_reg[Boot_Program_Index].init_bit(&cube_reg[CAR_BootandRun_Status],3,0,bits_len_union_t::_8bits_,_8UL,Boot_Program_Index_note_t,sizeof(Boot_Program_Index_note_t));
	cube_bit_reg[Firmware2_vr_Major].init_bit(&cube_reg[CAR_BootandRun_Status],4,0,bits_len_union_t::_8bits_,_8UL,Firmware2_vr_Major_note_t,sizeof(Firmware2_vr_Major_note_t));
	cube_bit_reg[Firmware2_vr_Minor].init_bit(&cube_reg[CAR_BootandRun_Status],5,0,bits_len_union_t::_8bits_,_8UL,Firmware2_vr_Minor_note_t,sizeof(Firmware2_vr_Minor_note_t));
	cube_bit_reg[Prog_Index].init_bit(&cube_reg[CAR_BootIndex_Status],0,0,bits_len_union_t::_8bits_,_8UL,Prog_Index_note_t,sizeof(Prog_Index_note_t));
	cube_bit_reg[Boot_Status].init_bit(&cube_reg[CAR_BootIndex_Status],1,0,bits_len_union_t::_8bits_,_8UL,Boot_Status_note_t,sizeof(Boot_Status_note_t));
	cube_bit_reg[GET_Cache_Enable_state].init_bit(&cube_reg[CAR_Get_CacheEnabled_state],0,0,bits_len_union_t::_1bits_,_8UL,GET_Cache_Enable_state_note_t,sizeof(GET_Cache_Enable_state_note_t));
	cube_bit_reg[EstMode].init_bit(&cube_reg[CAR_CurrADCSState],0,0,bits_len_union_t::_4bits_,_8UL,EstMode_note_t,sizeof(EstMode_note_t));
	cube_bit_reg[CtrlMode].init_bit(&cube_reg[CAR_CurrADCSState],0,4,bits_len_union_t::_4bits_,_8UL,CtrlMode_note_t,sizeof(CtrlMode_note_t));
	cube_bit_reg[AdcsRunMode].init_bit(&cube_reg[CAR_CurrADCSState],1,0,bits_len_union_t::_2bits_,_8UL,AdcsRunMode_note_t,sizeof(AdcsRunMode_note_t));
	cube_bit_reg[ASGP4Mode].init_bit(&cube_reg[CAR_CurrADCSState],1,2,bits_len_union_t::_2bits_,_8UL,ASGP4Mode_note_t,sizeof(ASGP4Mode_note_t));
	cube_bit_reg[CubeCtrlSignalEnabled].init_bit(&cube_reg[CAR_CurrADCSState],1,4,bits_len_union_t::_1bits_,_8UL,CubeCtrlSignalEnabled_note_t,sizeof(CubeCtrlSignalEnabled_note_t));
	cube_bit_reg[CubeCtrlMotorEnabled].init_bit(&cube_reg[CAR_CurrADCSState],1,5,bits_len_union_t::_1bits_,_8UL,CubeCtrlMotorEnabled_note_t,sizeof(CubeCtrlMotorEnabled_note_t));
	cube_bit_reg[CubeSense1Enabled].init_bit(&cube_reg[CAR_CurrADCSState],1,6,bits_len_union_t::_1bits_,_8UL,CubeSense1Enabled_note_t,sizeof(CubeSense1Enabled_note_t));
	cube_bit_reg[CubeSense2Enabled].init_bit(&cube_reg[CAR_CurrADCSState],1,7,bits_len_union_t::_1bits_,_8UL,CubeSense2Enabled_note_t,sizeof(CubeSense2Enabled_note_t));
	cube_bit_reg[CubeWheel1Enabled].init_bit(&cube_reg[CAR_CurrADCSState],2,0,bits_len_union_t::_1bits_,_8UL,CubeWheel1Enabled_note_t,sizeof(CubeWheel1Enabled_note_t));
	cube_bit_reg[CubeWheel2Enabled].init_bit(&cube_reg[CAR_CurrADCSState],2,1,bits_len_union_t::_1bits_,_8UL,CubeWheel2Enabled_note_t,sizeof(CubeWheel2Enabled_note_t));
	cube_bit_reg[CubeWheel3Enabled].init_bit(&cube_reg[CAR_CurrADCSState],2,2,bits_len_union_t::_1bits_,_8UL,CubeWheel3Enabled_note_t,sizeof(CubeWheel3Enabled_note_t));
	cube_bit_reg[CubeStarEnabled].init_bit(&cube_reg[CAR_CurrADCSState],2,3,bits_len_union_t::_1bits_,_8UL,CubeStarEnabled_note_t,sizeof(CubeStarEnabled_note_t));
	cube_bit_reg[GPSReceiverEnabled].init_bit(&cube_reg[CAR_CurrADCSState],2,4,bits_len_union_t::_1bits_,_8UL,GPSReceiverEnabled_note_t,sizeof(GPSReceiverEnabled_note_t));
	cube_bit_reg[GPSLNAPowerEnabled].init_bit(&cube_reg[CAR_CurrADCSState],2,5,bits_len_union_t::_1bits_,_8UL,GPSLNAPowerEnabled_note_t,sizeof(GPSLNAPowerEnabled_note_t));
	cube_bit_reg[MotorDriverEnabled].init_bit(&cube_reg[CAR_CurrADCSState],2,6,bits_len_union_t::_1bits_,_8UL,MotorDriverEnabled_note_t,sizeof(MotorDriverEnabled_note_t));
	cube_bit_reg[SunAboveLocalHorizon].init_bit(&cube_reg[CAR_CurrADCSState],2,7,bits_len_union_t::_1bits_,_8UL,SunAboveLocalHorizon_note_t,sizeof(SunAboveLocalHorizon_note_t));
	cube_bit_reg[CubeSense1CommuErr].init_bit(&cube_reg[CAR_CurrADCSState],3,0,bits_len_union_t::_1bits_,_8UL,CubeSense1CommuErr_note_t,sizeof(CubeSense1CommuErr_note_t));
	cube_bit_reg[CubeSense2CommuErr].init_bit(&cube_reg[CAR_CurrADCSState],3,1,bits_len_union_t::_1bits_,_8UL,CubeSense2CommuErr_note_t,sizeof(CubeSense2CommuErr_note_t));
	cube_bit_reg[CubeCtrlSignalCommuErr].init_bit(&cube_reg[CAR_CurrADCSState],3,2,bits_len_union_t::_1bits_,_8UL,CubeCtrlSignalCommuErr_note_t,sizeof(CubeCtrlSignalCommuErr_note_t));
	cube_bit_reg[CubeCtrlMotorCommuErr].init_bit(&cube_reg[CAR_CurrADCSState],3,3,bits_len_union_t::_1bits_,_8UL,CubeCtrlMotorCommuErr_note_t,sizeof(CubeCtrlMotorCommuErr_note_t));
	cube_bit_reg[CubeWheel1CommuErr].init_bit(&cube_reg[CAR_CurrADCSState],3,4,bits_len_union_t::_1bits_,_8UL,CubeWheel1CommuErr_note_t,sizeof(CubeWheel1CommuErr_note_t));
	cube_bit_reg[CubeWheel2CommuErr].init_bit(&cube_reg[CAR_CurrADCSState],3,5,bits_len_union_t::_1bits_,_8UL,CubeWheel2CommuErr_note_t,sizeof(CubeWheel2CommuErr_note_t));
	cube_bit_reg[CubeWheel3CommuErr].init_bit(&cube_reg[CAR_CurrADCSState],3,6,bits_len_union_t::_1bits_,_8UL,CubeWheel3CommuErr_note_t,sizeof(CubeWheel3CommuErr_note_t));
	cube_bit_reg[CubeStarCommuErr].init_bit(&cube_reg[CAR_CurrADCSState],3,7,bits_len_union_t::_1bits_,_8UL,CubeStarCommuErr_note_t,sizeof(CubeStarCommuErr_note_t));
	cube_bit_reg[MTMRangeErr].init_bit(&cube_reg[CAR_CurrADCSState],4,0,bits_len_union_t::_1bits_,_8UL,MTMRangeErr_note_t,sizeof(MTMRangeErr_note_t));
	cube_bit_reg[Cam1SRAMOVCDetected].init_bit(&cube_reg[CAR_CurrADCSState],4,1,bits_len_union_t::_1bits_,_8UL,Cam1SRAMOVCDetected_note_t,sizeof(Cam1SRAMOVCDetected_note_t));
	cube_bit_reg[Cam13V3OVCDetected].init_bit(&cube_reg[CAR_CurrADCSState],4,2,bits_len_union_t::_1bits_,_8UL,Cam13V3OVCDetected_note_t,sizeof(Cam13V3OVCDetected_note_t));
	cube_bit_reg[Cam1SensorBusyErr].init_bit(&cube_reg[CAR_CurrADCSState],4,3,bits_len_union_t::_1bits_,_8UL,Cam1SensorBusyErr_note_t,sizeof(Cam1SensorBusyErr_note_t));
	cube_bit_reg[Cam1SensorDetectionErr].init_bit(&cube_reg[CAR_CurrADCSState],4,4,bits_len_union_t::_1bits_,_8UL,Cam1SensorDetectionErr_note_t,sizeof(Cam1SensorDetectionErr_note_t));
	cube_bit_reg[SunSensorRangeErr].init_bit(&cube_reg[CAR_CurrADCSState],4,5,bits_len_union_t::_1bits_,_8UL,SunSensorRangeErr_note_t,sizeof(SunSensorRangeErr_note_t));
	cube_bit_reg[Cam2SRAMOVCDetected].init_bit(&cube_reg[CAR_CurrADCSState],4,6,bits_len_union_t::_1bits_,_8UL,Cam2SRAMOVCDetected_note_t,sizeof(Cam2SRAMOVCDetected_note_t));
	cube_bit_reg[Cam23V3OVCDetected].init_bit(&cube_reg[CAR_CurrADCSState],4,7,bits_len_union_t::_1bits_,_8UL,Cam23V3OVCDetected_note_t,sizeof(Cam23V3OVCDetected_note_t));
	cube_bit_reg[Cam2SensorBusyErr].init_bit(&cube_reg[CAR_CurrADCSState],5,0,bits_len_union_t::_1bits_,_8UL,Cam2SensorBusyErr_note_t,sizeof(Cam2SensorBusyErr_note_t));
	cube_bit_reg[Cam2SensorDetectionErr].init_bit(&cube_reg[CAR_CurrADCSState],5,1,bits_len_union_t::_1bits_,_8UL,Cam2SensorDetectionErr_note_t,sizeof(Cam2SensorDetectionErr_note_t));
	cube_bit_reg[NadirSensorRangeErr].init_bit(&cube_reg[CAR_CurrADCSState],5,2,bits_len_union_t::_1bits_,_8UL,NadirSensorRangeErr_note_t,sizeof(NadirSensorRangeErr_note_t));
	cube_bit_reg[RateSensorRangeErr].init_bit(&cube_reg[CAR_CurrADCSState],5,3,bits_len_union_t::_1bits_,_8UL,RateSensorRangeErr_note_t,sizeof(RateSensorRangeErr_note_t));
	cube_bit_reg[WheelSpeedRangeErr].init_bit(&cube_reg[CAR_CurrADCSState],5,4,bits_len_union_t::_1bits_,_8UL,WheelSpeedRangeErr_note_t,sizeof(WheelSpeedRangeErr_note_t));
	cube_bit_reg[CSSErr].init_bit(&cube_reg[CAR_CurrADCSState],5,5,bits_len_union_t::_1bits_,_8UL,CSSErr_note_t,sizeof(CSSErr_note_t));
	cube_bit_reg[STMatchErr].init_bit(&cube_reg[CAR_CurrADCSState],5,6,bits_len_union_t::_1bits_,_8UL,STMatchErr_note_t,sizeof(STMatchErr_note_t));
	cube_bit_reg[ST_OVCDetected].init_bit(&cube_reg[CAR_CurrADCSState],5,7,bits_len_union_t::_1bits_,_8UL,ST_OVCDetected_note_t,sizeof(ST_OVCDetected_note_t));
	cube_bit_reg[Progress_Percentage].init_bit(&cube_reg[CAR_JPGConverProg],0,0,bits_len_union_t::_8bits_,_8UL,Progress_Percentage_note_t,sizeof(Progress_Percentage_note_t));
	cube_bit_reg[Conversion_Result].init_bit(&cube_reg[CAR_JPGConverProg],1,0,bits_len_union_t::_8bits_,_8UL,Conversion_Result_note_t,sizeof(Conversion_Result_note_t));
	cube_bit_reg[Output_File_CT].init_bit(&cube_reg[CAR_JPGConverProg],2,0,bits_len_union_t::_8bits_,_8UL,Output_File_CT_note_t,sizeof(Output_File_CT_note_t));
	cube_bit_reg[GET_SRAM_Scrub_Size].init_bit(&cube_reg[CAR_Get_SRAM_Scrub_Para],0,0,bits_len_union_t::_16bits_,_16UL,GET_SRAM_Scrub_Size_note_t,sizeof(GET_SRAM_Scrub_Size_note_t));
	cube_bit_reg[ADCSConfigLoadErr].init_bit(&cube_reg[CAR_CubeACPState],0,0,bits_len_union_t::_1bits_,_8UL,ADCSConfigLoadErr_note_t,sizeof(ADCSConfigLoadErr_note_t));
	cube_bit_reg[OrbitParaLoadErr].init_bit(&cube_reg[CAR_CubeACPState],0,1,bits_len_union_t::_1bits_,_8UL,OrbitParaLoadErr_note_t,sizeof(OrbitParaLoadErr_note_t));
	cube_bit_reg[SystemConfigLoadErr].init_bit(&cube_reg[CAR_CubeACPState],0,2,bits_len_union_t::_1bits_,_8UL,SystemConfigLoadErr_note_t,sizeof(SystemConfigLoadErr_note_t));
	cube_bit_reg[SDcardInitErr].init_bit(&cube_reg[CAR_CubeACPState],0,3,bits_len_union_t::_1bits_,_8UL,SDcardInitErr_note_t,sizeof(SDcardInitErr_note_t));
	cube_bit_reg[SDcardreadErr].init_bit(&cube_reg[CAR_CubeACPState],0,4,bits_len_union_t::_1bits_,_8UL,SDcardreadErr_note_t,sizeof(SDcardreadErr_note_t));
	cube_bit_reg[SDcardwriteErr].init_bit(&cube_reg[CAR_CubeACPState],0,5,bits_len_union_t::_1bits_,_8UL,SDcardwriteErr_note_t,sizeof(SDcardwriteErr_note_t));
	cube_bit_reg[GET_MTQ1config].init_bit(&cube_reg[CAR_GetMTQConfig],0,0,bits_len_union_t::_8bits_,_8UL,GET_MTQ1config_note_t,sizeof(GET_MTQ1config_note_t));
	cube_bit_reg[GET_MTQ2config].init_bit(&cube_reg[CAR_GetMTQConfig],1,0,bits_len_union_t::_8bits_,_8UL,GET_MTQ2config_note_t,sizeof(GET_MTQ2config_note_t));
	cube_bit_reg[GET_MTQ3config].init_bit(&cube_reg[CAR_GetMTQConfig],2,0,bits_len_union_t::_8bits_,_8UL,GET_MTQ3config_note_t,sizeof(GET_MTQ3config_note_t));
	cube_bit_reg[GET_RW1_Config].init_bit(&cube_reg[CAR_Get_Wheel_Config],0,0,bits_len_union_t::_8bits_,_8UL,GET_RW1_Config_note_t,sizeof(GET_RW1_Config_note_t));
	cube_bit_reg[GET_RW2_Config].init_bit(&cube_reg[CAR_Get_Wheel_Config],1,0,bits_len_union_t::_8bits_,_8UL,GET_RW2_Config_note_t,sizeof(GET_RW2_Config_note_t));
	cube_bit_reg[GET_RW3_Config].init_bit(&cube_reg[CAR_Get_Wheel_Config],2,0,bits_len_union_t::_8bits_,_8UL,GET_RW3_Config_note_t,sizeof(GET_RW3_Config_note_t));
	cube_bit_reg[GET_RW4_Config].init_bit(&cube_reg[CAR_Get_Wheel_Config],3,0,bits_len_union_t::_8bits_,_8UL,GET_RW4_Config_note_t,sizeof(GET_RW4_Config_note_t));
	cube_bit_reg[GET_Gyro1Config].init_bit(&cube_reg[CAR_Get_RateGyroConfig],0,0,bits_len_union_t::_8bits_,_8UL,GET_Gyro1Config_note_t,sizeof(GET_Gyro1Config_note_t));
	cube_bit_reg[GET_Gyro2Config].init_bit(&cube_reg[CAR_Get_RateGyroConfig],1,0,bits_len_union_t::_8bits_,_8UL,GET_Gyro2Config_note_t,sizeof(GET_Gyro2Config_note_t));
	cube_bit_reg[GET_Gyro3Config].init_bit(&cube_reg[CAR_Get_RateGyroConfig],2,0,bits_len_union_t::_8bits_,_8UL,GET_Gyro3Config_note_t,sizeof(GET_Gyro3Config_note_t));
	cube_bit_reg[GET_XRateSensorOffset].init_bit(&cube_reg[CAR_Get_RateGyroConfig],3,0,bits_len_union_t::_16bits_,_16L,GET_XRateSensorOffset_note_t,sizeof(GET_XRateSensorOffset_note_t));
	cube_bit_reg[GET_YRateSensorOffset].init_bit(&cube_reg[CAR_Get_RateGyroConfig],5,0,bits_len_union_t::_16bits_,_16L,GET_YRateSensorOffset_note_t,sizeof(GET_YRateSensorOffset_note_t));
	cube_bit_reg[GET_ZRateSensorOffset].init_bit(&cube_reg[CAR_Get_RateGyroConfig],7,0,bits_len_union_t::_16bits_,_16L,GET_ZRateSensorOffset_note_t,sizeof(GET_ZRateSensorOffset_note_t));
	cube_bit_reg[GET_RateSensorMult].init_bit(&cube_reg[CAR_Get_RateGyroConfig],9,0,bits_len_union_t::_8bits_,_8UL,GET_RateSensorMult_note_t,sizeof(GET_RateSensorMult_note_t));
	cube_bit_reg[GET_CSS1_Config].init_bit(&cube_reg[CAR_Get_CSS_Config],0,0,bits_len_union_t::_8bits_,_8UL,GET_CSS1_Config_note_t,sizeof(GET_CSS1_Config_note_t));
	cube_bit_reg[GET_CSS2_Config].init_bit(&cube_reg[CAR_Get_CSS_Config],1,0,bits_len_union_t::_8bits_,_8UL,GET_CSS2_Config_note_t,sizeof(GET_CSS2_Config_note_t));
	cube_bit_reg[GET_CSS3_Config].init_bit(&cube_reg[CAR_Get_CSS_Config],2,0,bits_len_union_t::_8bits_,_8UL,GET_CSS3_Config_note_t,sizeof(GET_CSS3_Config_note_t));
	cube_bit_reg[GET_CSS4_Config].init_bit(&cube_reg[CAR_Get_CSS_Config],3,0,bits_len_union_t::_8bits_,_8UL,GET_CSS4_Config_note_t,sizeof(GET_CSS4_Config_note_t));
	cube_bit_reg[GET_CSS5_Config].init_bit(&cube_reg[CAR_Get_CSS_Config],4,0,bits_len_union_t::_8bits_,_8UL,GET_CSS5_Config_note_t,sizeof(GET_CSS5_Config_note_t));
	cube_bit_reg[GET_CSS6_Config].init_bit(&cube_reg[CAR_Get_CSS_Config],5,0,bits_len_union_t::_8bits_,_8UL,GET_CSS6_Config_note_t,sizeof(GET_CSS6_Config_note_t));
	cube_bit_reg[GET_CSS7_Config].init_bit(&cube_reg[CAR_Get_CSS_Config],6,0,bits_len_union_t::_8bits_,_8UL,GET_CSS7_Config_note_t,sizeof(GET_CSS7_Config_note_t));
	cube_bit_reg[GET_CSS8_Config].init_bit(&cube_reg[CAR_Get_CSS_Config],7,0,bits_len_union_t::_8bits_,_8UL,GET_CSS8_Config_note_t,sizeof(GET_CSS8_Config_note_t));
	cube_bit_reg[GET_CSS9_Config].init_bit(&cube_reg[CAR_Get_CSS_Config],8,0,bits_len_union_t::_8bits_,_8UL,GET_CSS9_Config_note_t,sizeof(GET_CSS9_Config_note_t));
	cube_bit_reg[GET_CSS10_Config].init_bit(&cube_reg[CAR_Get_CSS_Config],9,0,bits_len_union_t::_8bits_,_8UL,GET_CSS10_Config_note_t,sizeof(GET_CSS10_Config_note_t));
	cube_bit_reg[GET_CSS1_Rela_Scale].init_bit(&cube_reg[CAR_Get_CSS_Config],10,0,bits_len_union_t::_8bits_,_8UL,GET_CSS1_Rela_Scale_note_t,sizeof(GET_CSS1_Rela_Scale_note_t));
	cube_bit_reg[GET_CSS2_Rela_Scale].init_bit(&cube_reg[CAR_Get_CSS_Config],11,0,bits_len_union_t::_8bits_,_8UL,GET_CSS2_Rela_Scale_note_t,sizeof(GET_CSS2_Rela_Scale_note_t));
	cube_bit_reg[GET_CSS3_Rela_Scale].init_bit(&cube_reg[CAR_Get_CSS_Config],12,0,bits_len_union_t::_8bits_,_8UL,GET_CSS3_Rela_Scale_note_t,sizeof(GET_CSS3_Rela_Scale_note_t));
	cube_bit_reg[GET_CSS4_Rela_Scale].init_bit(&cube_reg[CAR_Get_CSS_Config],13,0,bits_len_union_t::_8bits_,_8UL,GET_CSS4_Rela_Scale_note_t,sizeof(GET_CSS4_Rela_Scale_note_t));
	cube_bit_reg[GET_CSS5_Rela_Scale].init_bit(&cube_reg[CAR_Get_CSS_Config],14,0,bits_len_union_t::_8bits_,_8UL,GET_CSS5_Rela_Scale_note_t,sizeof(GET_CSS5_Rela_Scale_note_t));
	cube_bit_reg[GET_CSS6_Rela_Scale].init_bit(&cube_reg[CAR_Get_CSS_Config],15,0,bits_len_union_t::_8bits_,_8UL,GET_CSS6_Rela_Scale_note_t,sizeof(GET_CSS6_Rela_Scale_note_t));
	cube_bit_reg[GET_CSS7_Rela_Scale].init_bit(&cube_reg[CAR_Get_CSS_Config],16,0,bits_len_union_t::_8bits_,_8UL,GET_CSS7_Rela_Scale_note_t,sizeof(GET_CSS7_Rela_Scale_note_t));
	cube_bit_reg[GET_CSS8_Rela_Scale].init_bit(&cube_reg[CAR_Get_CSS_Config],17,0,bits_len_union_t::_8bits_,_8UL,GET_CSS8_Rela_Scale_note_t,sizeof(GET_CSS8_Rela_Scale_note_t));
	cube_bit_reg[GET_CSS9_Rela_Scale].init_bit(&cube_reg[CAR_Get_CSS_Config],18,0,bits_len_union_t::_8bits_,_8UL,GET_CSS9_Rela_Scale_note_t,sizeof(GET_CSS9_Rela_Scale_note_t));
	cube_bit_reg[GET_CSS10_Rela_Scale].init_bit(&cube_reg[CAR_Get_CSS_Config],19,0,bits_len_union_t::_8bits_,_8UL,GET_CSS10_Rela_Scale_note_t,sizeof(GET_CSS10_Rela_Scale_note_t));
	cube_bit_reg[GET_CSS_Threshold].init_bit(&cube_reg[CAR_Get_CSS_Config],20,0,bits_len_union_t::_8bits_,_8UL,GET_CSS_Threshold_note_t,sizeof(GET_CSS_Threshold_note_t));
	cube_bit_reg[GET_CurrentUnixTime].init_bit(&cube_reg[CAR_Get_UnixTime],0,0,bits_len_union_t::_32bits_,_32UL,GET_CurrentUnixTime_note_t,sizeof(GET_CurrentUnixTime_note_t));
	cube_bit_reg[GET_Milliseconds].init_bit(&cube_reg[CAR_Get_UnixTime],4,0,bits_len_union_t::_16bits_,_16UL,GET_Milliseconds_note_t,sizeof(GET_Milliseconds_note_t));
	cube_bit_reg[Time_of_Event].init_bit(&cube_reg[CAR_LastLogged_Event],0,0,bits_len_union_t::_32bits_,_32UL,Time_of_Event_note_t,sizeof(Time_of_Event_note_t));
	cube_bit_reg[Event_ID].init_bit(&cube_reg[CAR_LastLogged_Event],4,0,bits_len_union_t::_8bits_,_16UL,Event_ID_note_t,sizeof(Event_ID_note_t));
	cube_bit_reg[Event_Parameter].init_bit(&cube_reg[CAR_LastLogged_Event],5,0,bits_len_union_t::_8bits_,_16UL,Event_Parameter_note_t,sizeof(Event_Parameter_note_t));
	cube_bit_reg[SRAM1_latchups].init_bit(&cube_reg[CAR_SRAMLatchup_CT],0,0,bits_len_union_t::_16bits_,_16UL,SRAM1_latchups_note_t,sizeof(SRAM1_latchups_note_t));
	cube_bit_reg[SRAM2_latchups].init_bit(&cube_reg[CAR_SRAMLatchup_CT],2,0,bits_len_union_t::_16bits_,_16UL,SRAM2_latchups_note_t,sizeof(SRAM2_latchups_note_t));
	cube_bit_reg[Single_SRAM_upsets].init_bit(&cube_reg[CAR_EDAC_Error_CT],0,0,bits_len_union_t::_16bits_,_16UL,Single_SRAM_upsets_note_t,sizeof(Single_SRAM_upsets_note_t));
	cube_bit_reg[Double_SRAM_upsets].init_bit(&cube_reg[CAR_EDAC_Error_CT],2,0,bits_len_union_t::_16bits_,_16UL,Double_SRAM_upsets_note_t,sizeof(Double_SRAM_upsets_note_t));
	cube_bit_reg[Multi_SRAM_upsets].init_bit(&cube_reg[CAR_EDAC_Error_CT],4,0,bits_len_union_t::_16bits_,_16UL,Multi_SRAM_upsets_note_t,sizeof(Multi_SRAM_upsets_note_t));
	cube_bit_reg[TC_CT].init_bit(&cube_reg[CAR_Commu_Status],0,0,bits_len_union_t::_16bits_,_16UL,TC_CT_note_t,sizeof(TC_CT_note_t));
	cube_bit_reg[TLM_CT].init_bit(&cube_reg[CAR_Commu_Status],2,0,bits_len_union_t::_16bits_,_16UL,TLM_CT_note_t,sizeof(TLM_CT_note_t));
	cube_bit_reg[TC_Buff_Overrun].init_bit(&cube_reg[CAR_Commu_Status],4,0,bits_len_union_t::_1bits_,_8UL,TC_Buff_Overrun_note_t,sizeof(TC_Buff_Overrun_note_t));
	cube_bit_reg[UART_Protocol_Error].init_bit(&cube_reg[CAR_Commu_Status],4,1,bits_len_union_t::_1bits_,_8UL,UART_Protocol_Error_note_t,sizeof(UART_Protocol_Error_note_t));
	cube_bit_reg[UART_Incomplete_Msg].init_bit(&cube_reg[CAR_Commu_Status],4,2,bits_len_union_t::_1bits_,_8UL,UART_Incomplete_Msg_note_t,sizeof(UART_Incomplete_Msg_note_t));
	cube_bit_reg[I2C_TLM_Error].init_bit(&cube_reg[CAR_Commu_Status],4,3,bits_len_union_t::_1bits_,_8UL,I2C_TLM_Error_note_t,sizeof(I2C_TLM_Error_note_t));
	cube_bit_reg[I2C_TC_Buff_Error].init_bit(&cube_reg[CAR_Commu_Status],4,4,bits_len_union_t::_1bits_,_8UL,I2C_TC_Buff_Error_note_t,sizeof(I2C_TC_Buff_Error_note_t));
	cube_bit_reg[CAN_TC_Buff_Error].init_bit(&cube_reg[CAR_Commu_Status],4,5,bits_len_union_t::_1bits_,_8UL,CAN_TC_Buff_Error_note_t,sizeof(CAN_TC_Buff_Error_note_t));
	cube_bit_reg[GET_Save_Now].init_bit(&cube_reg[CAR_Get_UnixTime_SavetoFlash],0,0,bits_len_union_t::_1bits_,_8UL,GET_Save_Now_note_t,sizeof(GET_Save_Now_note_t));
	cube_bit_reg[GET_Save_On_Update ].init_bit(&cube_reg[CAR_Get_UnixTime_SavetoFlash],0,1,bits_len_union_t::_1bits_,_8UL,GET_Save_On_Update_note_t,sizeof(GET_Save_On_Update_note_t));
	cube_bit_reg[GET_Save_Periodic].init_bit(&cube_reg[CAR_Get_UnixTime_SavetoFlash],0,2,bits_len_union_t::_1bits_,_8UL,GET_Save_Periodic_note_t,sizeof(GET_Save_Periodic_note_t));
	cube_bit_reg[GET_Period].init_bit(&cube_reg[CAR_Get_UnixTime_SavetoFlash],1,0,bits_len_union_t::_8bits_,_8UL,GET_Period_note_t,sizeof(GET_Period_note_t));
	cube_bit_reg[Roll].init_bit(&cube_reg[CAR_EstAttitudeAngles],0,0,bits_len_union_t::_16bits_,_16L,Roll_note_t,sizeof(Roll_note_t));
	cube_bit_reg[Pitch].init_bit(&cube_reg[CAR_EstAttitudeAngles],2,0,bits_len_union_t::_16bits_,_16L,Pitch_note_t,sizeof(Pitch_note_t));
	cube_bit_reg[Yaw].init_bit(&cube_reg[CAR_EstAttitudeAngles],4,0,bits_len_union_t::_16bits_,_16L,Yaw_note_t,sizeof(Yaw_note_t));
	cube_bit_reg[EstAngularRateX].init_bit(&cube_reg[CAR_EstAngRates],0,0,bits_len_union_t::_16bits_,_16L,EstAngularRateX_note_t,sizeof(EstAngularRateX_note_t));
	cube_bit_reg[EstAngularRateY].init_bit(&cube_reg[CAR_EstAngRates],2,0,bits_len_union_t::_16bits_,_16L,EstAngularRateY_note_t,sizeof(EstAngularRateY_note_t));
	cube_bit_reg[EstAngularRateZ].init_bit(&cube_reg[CAR_EstAngRates],4,0,bits_len_union_t::_16bits_,_16L,EstAngularRateZ_note_t,sizeof(EstAngularRateZ_note_t));
	cube_bit_reg[ECI_Pos_X].init_bit(&cube_reg[CAR_SatllitePosECI],0,0,bits_len_union_t::_16bits_,_16L,ECI_Pos_X_note_t,sizeof(ECI_Pos_X_note_t));
	cube_bit_reg[ECI_Pos_Y].init_bit(&cube_reg[CAR_SatllitePosECI],2,0,bits_len_union_t::_16bits_,_16L,ECI_Pos_Y_note_t,sizeof(ECI_Pos_Y_note_t));
	cube_bit_reg[ECI_Pos_Z].init_bit(&cube_reg[CAR_SatllitePosECI],4,0,bits_len_union_t::_16bits_,_16L,ECI_Pos_Z_note_t,sizeof(ECI_Pos_Z_note_t));
	cube_bit_reg[ECI_Velocity_X].init_bit(&cube_reg[CAR_SatelliteVelECI],0,0,bits_len_union_t::_16bits_,_16L,ECI_Velocity_X_note_t,sizeof(ECI_Velocity_X_note_t));
	cube_bit_reg[ECI_Velocity_Y].init_bit(&cube_reg[CAR_SatelliteVelECI],2,0,bits_len_union_t::_16bits_,_16L,ECI_Velocity_Y_note_t,sizeof(ECI_Velocity_Y_note_t));
	cube_bit_reg[ECI_Velocity_Z].init_bit(&cube_reg[CAR_SatelliteVelECI],4,0,bits_len_union_t::_16bits_,_16L,ECI_Velocity_Z_note_t,sizeof(ECI_Velocity_Z_note_t));
	cube_bit_reg[Latitude].init_bit(&cube_reg[CAR_SatellitePosLLH],0,0,bits_len_union_t::_16bits_,_16L,Latitude_note_t,sizeof(Latitude_note_t));
	cube_bit_reg[Longitude].init_bit(&cube_reg[CAR_SatellitePosLLH],2,0,bits_len_union_t::_16bits_,_16L,Longitude_note_t,sizeof(Longitude_note_t));
	cube_bit_reg[Altitude].init_bit(&cube_reg[CAR_SatellitePosLLH],4,0,bits_len_union_t::_16bits_,_16L,Altitude_note_t,sizeof(Altitude_note_t));
	cube_bit_reg[MagFieldVectorX].init_bit(&cube_reg[CAR_MagneticFieldVector],0,0,bits_len_union_t::_16bits_,_16L,MagFieldVectorX_note_t,sizeof(MagFieldVectorX_note_t));
	cube_bit_reg[MagFieldVectorY].init_bit(&cube_reg[CAR_MagneticFieldVector],2,0,bits_len_union_t::_16bits_,_16L,MagFieldVectorY_note_t,sizeof(MagFieldVectorY_note_t));
	cube_bit_reg[MagFieldVectorZ].init_bit(&cube_reg[CAR_MagneticFieldVector],4,0,bits_len_union_t::_16bits_,_16L,MagFieldVectorZ_note_t,sizeof(MagFieldVectorZ_note_t));
	cube_bit_reg[CSSVectorX].init_bit(&cube_reg[CAR_CSSVector],0,0,bits_len_union_t::_16bits_,_16L,CSSVectorX_note_t,sizeof(CSSVectorX_note_t));
	cube_bit_reg[CSSVectorY].init_bit(&cube_reg[CAR_CSSVector],2,0,bits_len_union_t::_16bits_,_16L,CSSVectorY_note_t,sizeof(CSSVectorY_note_t));
	cube_bit_reg[CSSVectorZ].init_bit(&cube_reg[CAR_CSSVector],4,0,bits_len_union_t::_16bits_,_16L,CSSVectorZ_note_t,sizeof(CSSVectorZ_note_t));
	cube_bit_reg[FSSVectorX].init_bit(&cube_reg[CAR_FSSVector],0,0,bits_len_union_t::_16bits_,_16L,FSSVectorX_note_t,sizeof(FSSVectorX_note_t));
	cube_bit_reg[FSSVectorY].init_bit(&cube_reg[CAR_FSSVector],2,0,bits_len_union_t::_16bits_,_16L,FSSVectorY_note_t,sizeof(FSSVectorY_note_t));
	cube_bit_reg[FSSVectorZ].init_bit(&cube_reg[CAR_FSSVector],4,0,bits_len_union_t::_16bits_,_16L,FSSVectorZ_note_t,sizeof(FSSVectorZ_note_t));
	cube_bit_reg[NadirVectorX].init_bit(&cube_reg[CAR_NadirVector],0,0,bits_len_union_t::_16bits_,_16L,NadirVectorX_note_t,sizeof(NadirVectorX_note_t));
	cube_bit_reg[NadirVectorY].init_bit(&cube_reg[CAR_NadirVector],2,0,bits_len_union_t::_16bits_,_16L,NadirVectorY_note_t,sizeof(NadirVectorY_note_t));
	cube_bit_reg[NadirVectorZ].init_bit(&cube_reg[CAR_NadirVector],4,0,bits_len_union_t::_16bits_,_16L,NadirVectorZ_note_t,sizeof(NadirVectorZ_note_t));
	cube_bit_reg[RateSensorRatesX].init_bit(&cube_reg[CAR_RateSensorRates],0,0,bits_len_union_t::_16bits_,_16L,RateSensorRatesX_note_t,sizeof(RateSensorRatesX_note_t));
	cube_bit_reg[RateSensorRatesY].init_bit(&cube_reg[CAR_RateSensorRates],2,0,bits_len_union_t::_16bits_,_16L,RateSensorRatesY_note_t,sizeof(RateSensorRatesY_note_t));
	cube_bit_reg[RateSensorRatesZ].init_bit(&cube_reg[CAR_RateSensorRates],4,0,bits_len_union_t::_16bits_,_16L,RateSensorRatesZ_note_t,sizeof(RateSensorRatesZ_note_t));
	cube_bit_reg[CurrWheelSpeedX].init_bit(&cube_reg[CAR_CurrWheelSpeed],0,0,bits_len_union_t::_16bits_,_16L,CurrWheelSpeedX_note_t,sizeof(CurrWheelSpeedX_note_t));
	cube_bit_reg[CurrWheelSpeedY].init_bit(&cube_reg[CAR_CurrWheelSpeed],2,0,bits_len_union_t::_16bits_,_16L,CurrWheelSpeedY_note_t,sizeof(CurrWheelSpeedY_note_t));
	cube_bit_reg[CurrWheelSpeedZ].init_bit(&cube_reg[CAR_CurrWheelSpeed],4,0,bits_len_union_t::_16bits_,_16L,CurrWheelSpeedZ_note_t,sizeof(CurrWheelSpeedZ_note_t));
	cube_bit_reg[CurrMTQCommandX].init_bit(&cube_reg[CAR_CurrMTQCommand],0,0,bits_len_union_t::_16bits_,_16L,CurrMTQCommandX_note_t,sizeof(CurrMTQCommandX_note_t));
	cube_bit_reg[CurrMTQCommandY].init_bit(&cube_reg[CAR_CurrMTQCommand],2,0,bits_len_union_t::_16bits_,_16L,CurrMTQCommandY_note_t,sizeof(CurrMTQCommandY_note_t));
	cube_bit_reg[CurrMTQCommandZ].init_bit(&cube_reg[CAR_CurrMTQCommand],4,0,bits_len_union_t::_16bits_,_16L,CurrMTQCommandZ_note_t,sizeof(CurrMTQCommandZ_note_t));
	cube_bit_reg[CurrWheelSpeedCommandX].init_bit(&cube_reg[CAR_CurrWheelSpeedCommand],0,0,bits_len_union_t::_16bits_,_16L,CurrWheelSpeedCommandX_note_t,sizeof(CurrWheelSpeedCommandX_note_t));
	cube_bit_reg[CurrWheelSpeedCommandY].init_bit(&cube_reg[CAR_CurrWheelSpeedCommand],2,0,bits_len_union_t::_16bits_,_16L,CurrWheelSpeedCommandY_note_t,sizeof(CurrWheelSpeedCommandY_note_t));
	cube_bit_reg[CurrWheelSpeedCommandZ].init_bit(&cube_reg[CAR_CurrWheelSpeedCommand],4,0,bits_len_union_t::_16bits_,_16L,CurrWheelSpeedCommandZ_note_t,sizeof(CurrWheelSpeedCommandZ_note_t));
	cube_bit_reg[IGRFVectorX].init_bit(&cube_reg[CAR_IGRFModelMagFieldVector],0,0,bits_len_union_t::_16bits_,_16L,IGRFVectorX_note_t,sizeof(IGRFVectorX_note_t));
	cube_bit_reg[IGRFVectorY].init_bit(&cube_reg[CAR_IGRFModelMagFieldVector],2,0,bits_len_union_t::_16bits_,_16L,IGRFVectorY_note_t,sizeof(IGRFVectorY_note_t));
	cube_bit_reg[IGRFVectorZ].init_bit(&cube_reg[CAR_IGRFModelMagFieldVector],4,0,bits_len_union_t::_16bits_,_16L,IGRFVectorZ_note_t,sizeof(IGRFVectorZ_note_t));
	cube_bit_reg[Modelled_Sun_VectorX].init_bit(&cube_reg[CAR_Modelled_Sun_Vector],0,0,bits_len_union_t::_16bits_,_16L,Modelled_Sun_VectorX_note_t,sizeof(Modelled_Sun_VectorX_note_t));
	cube_bit_reg[Modelled_Sun_VectorY].init_bit(&cube_reg[CAR_Modelled_Sun_Vector],2,0,bits_len_union_t::_16bits_,_16L,Modelled_Sun_VectorY_note_t,sizeof(Modelled_Sun_VectorY_note_t));
	cube_bit_reg[Modelled_Sun_VectorZ].init_bit(&cube_reg[CAR_Modelled_Sun_Vector],4,0,bits_len_union_t::_16bits_,_16L,Modelled_Sun_VectorZ_note_t,sizeof(Modelled_Sun_VectorZ_note_t));
	cube_bit_reg[EstGyroBiasX].init_bit(&cube_reg[CAR_EstGyroBias],0,0,bits_len_union_t::_16bits_,_16L,EstGyroBiasX_note_t,sizeof(EstGyroBiasX_note_t));
	cube_bit_reg[EstGyroBiasY].init_bit(&cube_reg[CAR_EstGyroBias],2,0,bits_len_union_t::_16bits_,_16L,EstGyroBiasY_note_t,sizeof(EstGyroBiasY_note_t));
	cube_bit_reg[EstGyroBiasZ].init_bit(&cube_reg[CAR_EstGyroBias],4,0,bits_len_union_t::_16bits_,_16L,EstGyroBiasZ_note_t,sizeof(EstGyroBiasZ_note_t));
	cube_bit_reg[EstInnovationX].init_bit(&cube_reg[CAR_EstInnovation],0,0,bits_len_union_t::_16bits_,_16L,EstInnovationX_note_t,sizeof(EstInnovationX_note_t));
	cube_bit_reg[EstInnovationY].init_bit(&cube_reg[CAR_EstInnovation],2,0,bits_len_union_t::_16bits_,_16L,EstInnovationY_note_t,sizeof(EstInnovationY_note_t));
	cube_bit_reg[EstInnovationZ].init_bit(&cube_reg[CAR_EstInnovation],4,0,bits_len_union_t::_16bits_,_16L,EstInnovationZ_note_t,sizeof(EstInnovationZ_note_t));
	cube_bit_reg[Q_Err1].init_bit(&cube_reg[CAR_QuatErrorVector],0,0,bits_len_union_t::_16bits_,_16L,Q_Err1_note_t,sizeof(Q_Err1_note_t));
	cube_bit_reg[Q_Err2].init_bit(&cube_reg[CAR_QuatErrorVector],2,0,bits_len_union_t::_16bits_,_16L,Q_Err2_note_t,sizeof(Q_Err2_note_t));
	cube_bit_reg[Q_Err3].init_bit(&cube_reg[CAR_QuatErrorVector],4,0,bits_len_union_t::_16bits_,_16L,Q_Err3_note_t,sizeof(Q_Err3_note_t));
	cube_bit_reg[Q1_Covariance].init_bit(&cube_reg[CAR_QuatCovariance],0,0,bits_len_union_t::_16bits_,_16L,Q1_Covariance_note_t,sizeof(Q1_Covariance_note_t));
	cube_bit_reg[Q2_Covariance].init_bit(&cube_reg[CAR_QuatCovariance],2,0,bits_len_union_t::_16bits_,_16L,Q2_Covariance_note_t,sizeof(Q2_Covariance_note_t));
	cube_bit_reg[Q3_Covariance].init_bit(&cube_reg[CAR_QuatCovariance],4,0,bits_len_union_t::_16bits_,_16L,Q3_Covariance_note_t,sizeof(Q3_Covariance_note_t));
	cube_bit_reg[Angular_Rate_Covar_X].init_bit(&cube_reg[CAR_AngRateCovariance],0,0,bits_len_union_t::_16bits_,_16L,Angular_Rate_Covar_X_note_t,sizeof(Angular_Rate_Covar_X_note_t));
	cube_bit_reg[Angular_Rate_Covar_Y].init_bit(&cube_reg[CAR_AngRateCovariance],2,0,bits_len_union_t::_16bits_,_16L,Angular_Rate_Covar_Y_note_t,sizeof(Angular_Rate_Covar_Y_note_t));
	cube_bit_reg[Angular_Rate_Covar_Z].init_bit(&cube_reg[CAR_AngRateCovariance],4,0,bits_len_union_t::_16bits_,_16L,Angular_Rate_Covar_Z_note_t,sizeof(Angular_Rate_Covar_Z_note_t));
	cube_bit_reg[Cam2centroidX].init_bit(&cube_reg[CAR_RawCam2Sensor],0,0,bits_len_union_t::_16bits_,_16L,Cam2centroidX_note_t,sizeof(Cam2centroidX_note_t));
	cube_bit_reg[Cam2centroidY].init_bit(&cube_reg[CAR_RawCam2Sensor],2,0,bits_len_union_t::_16bits_,_16L,Cam2centroidY_note_t,sizeof(Cam2centroidY_note_t));
	cube_bit_reg[Cam2CapStatus].init_bit(&cube_reg[CAR_RawCam2Sensor],4,0,bits_len_union_t::_8bits_,_8UL,Cam2CapStatus_note_t,sizeof(Cam2CapStatus_note_t));
	cube_bit_reg[Cam2DetectResult].init_bit(&cube_reg[CAR_RawCam2Sensor],5,0,bits_len_union_t::_8bits_,_8UL,Cam2DetectResult_note_t,sizeof(Cam2DetectResult_note_t));
	cube_bit_reg[Cam1centroidX].init_bit(&cube_reg[CAR_RawCam1Sensor],0,0,bits_len_union_t::_16bits_,_16L,Cam1centroidX_note_t,sizeof(Cam1centroidX_note_t));
	cube_bit_reg[Cam1centroidY].init_bit(&cube_reg[CAR_RawCam1Sensor],2,0,bits_len_union_t::_16bits_,_16L,Cam1centroidY_note_t,sizeof(Cam1centroidY_note_t));
	cube_bit_reg[Cam1CapStatus].init_bit(&cube_reg[CAR_RawCam1Sensor],4,0,bits_len_union_t::_8bits_,_8UL,Cam1CapStatus_note_t,sizeof(Cam1CapStatus_note_t));
	cube_bit_reg[Cam1DetectResult].init_bit(&cube_reg[CAR_RawCam1Sensor],5,0,bits_len_union_t::_8bits_,_8UL,Cam1DetectResult_note_t,sizeof(Cam1DetectResult_note_t));
	cube_bit_reg[RAW_CSS1].init_bit(&cube_reg[CAR_RawCSS1to6],0,0,bits_len_union_t::_8bits_,_8UL,RAW_CSS1_note_t,sizeof(RAW_CSS1_note_t));
	cube_bit_reg[RAW_CSS2].init_bit(&cube_reg[CAR_RawCSS1to6],1,0,bits_len_union_t::_8bits_,_8UL,RAW_CSS2_note_t,sizeof(RAW_CSS2_note_t));
	cube_bit_reg[RAW_CSS3].init_bit(&cube_reg[CAR_RawCSS1to6],2,0,bits_len_union_t::_8bits_,_8UL,RAW_CSS3_note_t,sizeof(RAW_CSS3_note_t));
	cube_bit_reg[RAW_CSS4].init_bit(&cube_reg[CAR_RawCSS1to6],3,0,bits_len_union_t::_8bits_,_8UL,RAW_CSS4_note_t,sizeof(RAW_CSS4_note_t));
	cube_bit_reg[RAW_CSS5].init_bit(&cube_reg[CAR_RawCSS1to6],4,0,bits_len_union_t::_8bits_,_8UL,RAW_CSS5_note_t,sizeof(RAW_CSS5_note_t));
	cube_bit_reg[RAW_CSS6].init_bit(&cube_reg[CAR_RawCSS1to6],5,0,bits_len_union_t::_8bits_,_8UL,RAW_CSS6_note_t,sizeof(RAW_CSS6_note_t));
	cube_bit_reg[RAW_CSS7].init_bit(&cube_reg[CAR_RawCSS7to10],0,0,bits_len_union_t::_8bits_,_8UL,RAW_CSS7_note_t,sizeof(RAW_CSS7_note_t));
	cube_bit_reg[RAW_CSS8].init_bit(&cube_reg[CAR_RawCSS7to10],1,0,bits_len_union_t::_8bits_,_8UL,RAW_CSS8_note_t,sizeof(RAW_CSS8_note_t));
	cube_bit_reg[RAW_CSS9].init_bit(&cube_reg[CAR_RawCSS7to10],2,0,bits_len_union_t::_8bits_,_8UL,RAW_CSS9_note_t,sizeof(RAW_CSS9_note_t));
	cube_bit_reg[RAW_CSS10].init_bit(&cube_reg[CAR_RawCSS7to10],3,0,bits_len_union_t::_8bits_,_8UL,RAW_CSS10_note_t,sizeof(RAW_CSS10_note_t));
	cube_bit_reg[RAW_Mag_X].init_bit(&cube_reg[CAR_RawMTM],0,0,bits_len_union_t::_16bits_,_16L,RAW_Mag_X_note_t,sizeof(RAW_Mag_X_note_t));
	cube_bit_reg[RAW_Mag_Y].init_bit(&cube_reg[CAR_RawMTM],2,0,bits_len_union_t::_16bits_,_16L,RAW_Mag_Y_note_t,sizeof(RAW_Mag_Y_note_t));
	cube_bit_reg[RAW_Mag_Z].init_bit(&cube_reg[CAR_RawMTM],4,0,bits_len_union_t::_16bits_,_16L,RAW_Mag_Z_note_t,sizeof(RAW_Mag_Z_note_t));
	cube_bit_reg[CS1_3V3_Current].init_bit(&cube_reg[CAR_CubeSense1CurrentMea],0,0,bits_len_union_t::_16bits_,_16L,CS1_3V3_Current_note_t,sizeof(CS1_3V3_Current_note_t));
	cube_bit_reg[CS1_SRAM_Current].init_bit(&cube_reg[CAR_CubeSense1CurrentMea],2,0,bits_len_union_t::_16bits_,_16L,CS1_SRAM_Current_note_t,sizeof(CS1_SRAM_Current_note_t));
	cube_bit_reg[CubeCtrl3V3Current].init_bit(&cube_reg[CAR_CubeControlCurrent],0,0,bits_len_union_t::_16bits_,_16UL,CubeCtrl3V3Current_note_t,sizeof(CubeCtrl3V3Current_note_t));
	cube_bit_reg[CubeCtrl5Current].init_bit(&cube_reg[CAR_CubeControlCurrent],2,0,bits_len_union_t::_16bits_,_16UL,CubeCtrl5Current_note_t,sizeof(CubeCtrl5Current_note_t));
	cube_bit_reg[CubeCtrlVbatCurrent].init_bit(&cube_reg[CAR_CubeControlCurrent],4,0,bits_len_union_t::_16bits_,_16UL,CubeCtrlVbatCurrent_note_t,sizeof(CubeCtrlVbatCurrent_note_t));
	cube_bit_reg[Wheel1Current].init_bit(&cube_reg[CAR_WheelCurrents],0,0,bits_len_union_t::_16bits_,_16L,Wheel1Current_note_t,sizeof(Wheel1Current_note_t));
	cube_bit_reg[Wheel2Current].init_bit(&cube_reg[CAR_WheelCurrents],2,0,bits_len_union_t::_16bits_,_16L,Wheel2Current_note_t,sizeof(Wheel2Current_note_t));
	cube_bit_reg[Wheel3Current].init_bit(&cube_reg[CAR_WheelCurrents],4,0,bits_len_union_t::_16bits_,_16L,Wheel3Current_note_t,sizeof(Wheel3Current_note_t));
	cube_bit_reg[MCU_Temp].init_bit(&cube_reg[CAR_CubeADCS_Temp],0,0,bits_len_union_t::_16bits_,_16L,MCU_Temp_note_t,sizeof(MCU_Temp_note_t));
	cube_bit_reg[MTM_Temp].init_bit(&cube_reg[CAR_CubeADCS_Temp],2,0,bits_len_union_t::_16bits_,_16L,MTM_Temp_note_t,sizeof(MTM_Temp_note_t));
	cube_bit_reg[RedundMTM_Temp].init_bit(&cube_reg[CAR_CubeADCS_Temp],4,0,bits_len_union_t::_16bits_,_16L,RedundMTM_Temp_note_t,sizeof(RedundMTM_Temp_note_t));
	cube_bit_reg[RateSensor_TempX].init_bit(&cube_reg[CAR_RateSens_Temp],0,0,bits_len_union_t::_16bits_,_16L,RateSensor_TempX_note_t,sizeof(RateSensor_TempX_note_t));
	cube_bit_reg[RateSensor_TempY].init_bit(&cube_reg[CAR_RateSens_Temp],2,0,bits_len_union_t::_16bits_,_16L,RateSensor_TempY_note_t,sizeof(RateSensor_TempY_note_t));
	cube_bit_reg[RateSensor_TempZ].init_bit(&cube_reg[CAR_RateSens_Temp],4,0,bits_len_union_t::_16bits_,_16L,RateSensor_TempZ_note_t,sizeof(RateSensor_TempZ_note_t));
	cube_bit_reg[GPS_Solution_Status].init_bit(&cube_reg[CAR_Raw_GPS_Status],0,0,bits_len_union_t::_8bits_,_8UL,GPS_Solution_Status_note_t,sizeof(GPS_Solution_Status_note_t));
	cube_bit_reg[Num_of_tracked_GPS_Sat].init_bit(&cube_reg[CAR_Raw_GPS_Status],1,0,bits_len_union_t::_8bits_,_8UL,Num_of_tracked_GPS_Sat_note_t,sizeof(Num_of_tracked_GPS_Sat_note_t));
	cube_bit_reg[NumofGPSSat_UsedIn_Solution].init_bit(&cube_reg[CAR_Raw_GPS_Status],2,0,bits_len_union_t::_8bits_,_8UL,NumofGPSSat_UsedIn_Solution_note_t,sizeof(NumofGPSSat_UsedIn_Solution_note_t));
	cube_bit_reg[CTforXYZLof_from_GPS].init_bit(&cube_reg[CAR_Raw_GPS_Status],3,0,bits_len_union_t::_8bits_,_8UL,CTforXYZLof_from_GPS_note_t,sizeof(CTforXYZLof_from_GPS_note_t));
	cube_bit_reg[CTforRANGElog_from_GPS].init_bit(&cube_reg[CAR_Raw_GPS_Status],4,0,bits_len_union_t::_8bits_,_8UL,CTforRANGElog_from_GPS_note_t,sizeof(CTforRANGElog_from_GPS_note_t));
	cube_bit_reg[Resp_Msg_for_GPSLogSetup].init_bit(&cube_reg[CAR_Raw_GPS_Status],5,0,bits_len_union_t::_8bits_,_8UL,Resp_Msg_for_GPSLogSetup_note_t,sizeof(Resp_Msg_for_GPSLogSetup_note_t));
	cube_bit_reg[GPS_Ref_Week].init_bit(&cube_reg[CAR_Raw_GPS_Time],0,0,bits_len_union_t::_16bits_,_16UL,GPS_Ref_Week_note_t,sizeof(GPS_Ref_Week_note_t));
	cube_bit_reg[GPS_Time_Millisec].init_bit(&cube_reg[CAR_Raw_GPS_Time],2,0,bits_len_union_t::_32bits_,_32UL,GPS_Time_Millisec_note_t,sizeof(GPS_Time_Millisec_note_t));
	cube_bit_reg[GPS_ECEF_PosX].init_bit(&cube_reg[CAR_RawGPSX],0,0,bits_len_union_t::_32bits_,_16L,GPS_ECEF_PosX_note_t,sizeof(GPS_ECEF_PosX_note_t));
	cube_bit_reg[GPS_ECEF_VeloX].init_bit(&cube_reg[CAR_RawGPSX],4,0,bits_len_union_t::_16bits_,_16L,GPS_ECEF_VeloX_note_t,sizeof(GPS_ECEF_VeloX_note_t));
	cube_bit_reg[GPS_ECEF_PosY].init_bit(&cube_reg[CAR_RawGPSY],0,0,bits_len_union_t::_32bits_,_16L,GPS_ECEF_PosY_note_t,sizeof(GPS_ECEF_PosY_note_t));
	cube_bit_reg[GPS_ECEF_VeloY].init_bit(&cube_reg[CAR_RawGPSY],4,0,bits_len_union_t::_16bits_,_16L,GPS_ECEF_VeloY_note_t,sizeof(GPS_ECEF_VeloY_note_t));
	cube_bit_reg[GPS_ECEF_PosZ].init_bit(&cube_reg[CAR_RawGPSZ],0,0,bits_len_union_t::_32bits_,_16L,GPS_ECEF_PosZ_note_t,sizeof(GPS_ECEF_PosZ_note_t));
	cube_bit_reg[GPS_ECEF_VeloZ].init_bit(&cube_reg[CAR_RawGPSZ],4,0,bits_len_union_t::_16bits_,_16L,GPS_ECEF_VeloZ_note_t,sizeof(GPS_ECEF_VeloZ_note_t));
	cube_bit_reg[TimetoPerform_ADCSUpdate].init_bit(&cube_reg[CAR_ADCS_Exe_time],0,0,bits_len_union_t::_16bits_,_16L,TimetoPerform_ADCSUpdate_note_t,sizeof(TimetoPerform_ADCSUpdate_note_t));
	cube_bit_reg[TimetoPerform_SA_Commu].init_bit(&cube_reg[CAR_ADCS_Exe_time],2,0,bits_len_union_t::_16bits_,_16L,TimetoPerform_SA_Commu_note_t,sizeof(TimetoPerform_SA_Commu_note_t));
	cube_bit_reg[TimetoExecute_SGP4_Propagator].init_bit(&cube_reg[CAR_ADCS_Exe_time],4,0,bits_len_union_t::_16bits_,_16L,TimetoExecute_SGP4_Propagator_note_t,sizeof(TimetoExecute_SGP4_Propagator_note_t));
	cube_bit_reg[TimetoExecute_IGRF_Model].init_bit(&cube_reg[CAR_ADCS_Exe_time],6,0,bits_len_union_t::_16bits_,_16L,TimetoExecute_IGRF_Model_note_t,sizeof(TimetoExecute_IGRF_Model_note_t));
	cube_bit_reg[GET_CubeCtrlSignalPowerSelect].init_bit(&cube_reg[CAR_Get_ADCSPowerCtrl],0,0,bits_len_union_t::_2bits_,_8UL,GET_CubeCtrlSignalPowerSelect_note_t,sizeof(GET_CubeCtrlSignalPowerSelect_note_t));
	cube_bit_reg[GET_CubeCtrlMotorPowerSelect].init_bit(&cube_reg[CAR_Get_ADCSPowerCtrl],0,2,bits_len_union_t::_2bits_,_8UL,GET_CubeCtrlMotorPowerSelect_note_t,sizeof(GET_CubeCtrlMotorPowerSelect_note_t));
	cube_bit_reg[GET_CubeSense1PowerSelect].init_bit(&cube_reg[CAR_Get_ADCSPowerCtrl],0,4,bits_len_union_t::_2bits_,_8UL,GET_CubeSense1PowerSelect_note_t,sizeof(GET_CubeSense1PowerSelect_note_t));
	cube_bit_reg[GET_CubeSense2PowerSelect].init_bit(&cube_reg[CAR_Get_ADCSPowerCtrl],0,6,bits_len_union_t::_2bits_,_8UL,GET_CubeSense2PowerSelect_note_t,sizeof(GET_CubeSense2PowerSelect_note_t));
	cube_bit_reg[GET_CubeStarPowerSelect].init_bit(&cube_reg[CAR_Get_ADCSPowerCtrl],1,0,bits_len_union_t::_2bits_,_8UL,GET_CubeStarPowerSelect_note_t,sizeof(GET_CubeStarPowerSelect_note_t));
	cube_bit_reg[GET_CubeWheel1PowerSelect].init_bit(&cube_reg[CAR_Get_ADCSPowerCtrl],1,2,bits_len_union_t::_2bits_,_8UL,GET_CubeWheel1PowerSelect_note_t,sizeof(GET_CubeWheel1PowerSelect_note_t));
	cube_bit_reg[GET_CubeWheel2PowerSelect].init_bit(&cube_reg[CAR_Get_ADCSPowerCtrl],1,4,bits_len_union_t::_2bits_,_8UL,GET_CubeWheel2PowerSelect_note_t,sizeof(GET_CubeWheel2PowerSelect_note_t));
	cube_bit_reg[GET_CubeWheel3PowerSelect].init_bit(&cube_reg[CAR_Get_ADCSPowerCtrl],1,6,bits_len_union_t::_2bits_,_8UL,GET_CubeWheel3PowerSelect_note_t,sizeof(GET_CubeWheel3PowerSelect_note_t));
	cube_bit_reg[GET_MotorPower].init_bit(&cube_reg[CAR_Get_ADCSPowerCtrl],2,0,bits_len_union_t::_2bits_,_8UL,GET_MotorPower_note_t,sizeof(GET_MotorPower_note_t));
	cube_bit_reg[GET_GPSPower].init_bit(&cube_reg[CAR_Get_ADCSPowerCtrl],2,2,bits_len_union_t::_2bits_,_8UL,GET_GPSPower_note_t,sizeof(GET_GPSPower_note_t));
	cube_bit_reg[CubeStarCurrent].init_bit(&cube_reg[CAR_ADCSMiscCurrents],0,0,bits_len_union_t::_16bits_,_16UL,CubeStarCurrent_note_t,sizeof(CubeStarCurrent_note_t));
	cube_bit_reg[MTQCurrent].init_bit(&cube_reg[CAR_ADCSMiscCurrents],2,0,bits_len_union_t::_16bits_,_16UL,MTQCurrent_note_t,sizeof(MTQCurrent_note_t));
	cube_bit_reg[CubeStarMCU_Temp].init_bit(&cube_reg[CAR_ADCSMiscCurrents],4,0,bits_len_union_t::_16bits_,_16L,CubeStarMCU_Temp_note_t,sizeof(CubeStarMCU_Temp_note_t));
	cube_bit_reg[GET_Cmd_Roll].init_bit(&cube_reg[CAR_Get_CmdAttAngle],0,0,bits_len_union_t::_16bits_,_16L,GET_Cmd_Roll_note_t,sizeof(GET_Cmd_Roll_note_t));
	cube_bit_reg[GET_Cmd_Pitch].init_bit(&cube_reg[CAR_Get_CmdAttAngle],2,0,bits_len_union_t::_16bits_,_16L,GET_Cmd_Pitch_note_t,sizeof(GET_Cmd_Pitch_note_t));
	cube_bit_reg[GET_Cmd_Yaw].init_bit(&cube_reg[CAR_Get_CmdAttAngle],4,0,bits_len_union_t::_16bits_,_16L,GET_Cmd_Yaw_note_t,sizeof(GET_Cmd_Yaw_note_t));
	cube_bit_reg[FineEstXAngularRate].init_bit(&cube_reg[CAR_FineEstAngRates],0,0,bits_len_union_t::_16bits_,_16L,FineEstXAngularRate_note_t,sizeof(FineEstXAngularRate_note_t));
	cube_bit_reg[FineEstYAngularRate].init_bit(&cube_reg[CAR_FineEstAngRates],2,0,bits_len_union_t::_16bits_,_16L,FineEstYAngularRate_note_t,sizeof(FineEstYAngularRate_note_t));
	cube_bit_reg[FineEstZAngularRate].init_bit(&cube_reg[CAR_FineEstAngRates],4,0,bits_len_union_t::_16bits_,_16L,FineEstZAngularRate_note_t,sizeof(FineEstZAngularRate_note_t));
	cube_bit_reg[GET_Cam1_Mounting_Trans_Alpha ].init_bit(&cube_reg[CAR_Get_CubeSensConfig],0,0,bits_len_union_t::_16bits_,_16L,GET_Cam1_Mounting_Trans_Alpha_note_t,sizeof(GET_Cam1_Mounting_Trans_Alpha_note_t));
	cube_bit_reg[GET_Cam1_Mounting_Trans_Beta ].init_bit(&cube_reg[CAR_Get_CubeSensConfig],2,0,bits_len_union_t::_16bits_,_16L,GET_Cam1_Mounting_Trans_Beta_note_t,sizeof(GET_Cam1_Mounting_Trans_Beta_note_t));
	cube_bit_reg[GET_Cam1_Mounting_Trans_Gamma].init_bit(&cube_reg[CAR_Get_CubeSensConfig],4,0,bits_len_union_t::_16bits_,_16L,GET_Cam1_Mounting_Trans_Gamma_note_t,sizeof(GET_Cam1_Mounting_Trans_Gamma_note_t));
	cube_bit_reg[GET_Cam1_DetectThreshold].init_bit(&cube_reg[CAR_Get_CubeSensConfig],6,0,bits_len_union_t::_8bits_,_8UL,GET_Cam1_DetectThreshold_note_t,sizeof(GET_Cam1_DetectThreshold_note_t));
	cube_bit_reg[GET_Cam1_AutoAdjust].init_bit(&cube_reg[CAR_Get_CubeSensConfig],7,0,bits_len_union_t::_1bits_,_8UL,GET_Cam1_AutoAdjust_note_t,sizeof(GET_Cam1_AutoAdjust_note_t));
	cube_bit_reg[GET_Cam1_Exposure_Time].init_bit(&cube_reg[CAR_Get_CubeSensConfig],8,0,bits_len_union_t::_16bits_,_16UL,GET_Cam1_Exposure_Time_note_t,sizeof(GET_Cam1_Exposure_Time_note_t));
	cube_bit_reg[GET_Cam1_Boresight_X].init_bit(&cube_reg[CAR_Get_CubeSensConfig],10,0,bits_len_union_t::_16bits_,_16UL,GET_Cam1_Boresight_X_note_t,sizeof(GET_Cam1_Boresight_X_note_t));
	cube_bit_reg[GET_Cam1_Boresight_Y].init_bit(&cube_reg[CAR_Get_CubeSensConfig],12,0,bits_len_union_t::_16bits_,_16UL,GET_Cam1_Boresight_Y_note_t,sizeof(GET_Cam1_Boresight_Y_note_t));
	cube_bit_reg[GET_Cam2_Mounting_Trans_Alpha ].init_bit(&cube_reg[CAR_Get_CubeSensConfig],14,0,bits_len_union_t::_16bits_,_16L,GET_Cam2_Mounting_Trans_Alpha_note_t,sizeof(GET_Cam2_Mounting_Trans_Alpha_note_t));
	cube_bit_reg[GET_Cam2_Mounting_Trans_Beta ].init_bit(&cube_reg[CAR_Get_CubeSensConfig],16,0,bits_len_union_t::_16bits_,_16L,GET_Cam2_Mounting_Trans_Beta_note_t,sizeof(GET_Cam2_Mounting_Trans_Beta_note_t));
	cube_bit_reg[GET_Cam2_Mounting_Trans_Gamma].init_bit(&cube_reg[CAR_Get_CubeSensConfig],18,0,bits_len_union_t::_16bits_,_16L,GET_Cam2_Mounting_Trans_Gamma_note_t,sizeof(GET_Cam2_Mounting_Trans_Gamma_note_t));
	cube_bit_reg[GET_Cam2_DetectThreshold].init_bit(&cube_reg[CAR_Get_CubeSensConfig],20,0,bits_len_union_t::_8bits_,_8UL,GET_Cam2_DetectThreshold_note_t,sizeof(GET_Cam2_DetectThreshold_note_t));
	cube_bit_reg[GET_Cam2_AutoAdjust].init_bit(&cube_reg[CAR_Get_CubeSensConfig],21,0,bits_len_union_t::_1bits_,_8UL,GET_Cam2_AutoAdjust_note_t,sizeof(GET_Cam2_AutoAdjust_note_t));
	cube_bit_reg[GET_Cam2_Exposure_Time].init_bit(&cube_reg[CAR_Get_CubeSensConfig],22,0,bits_len_union_t::_16bits_,_16UL,GET_Cam2_Exposure_Time_note_t,sizeof(GET_Cam2_Exposure_Time_note_t));
	cube_bit_reg[GET_Cam2_Boresight_X].init_bit(&cube_reg[CAR_Get_CubeSensConfig],24,0,bits_len_union_t::_16bits_,_16UL,GET_Cam2_Boresight_X_note_t,sizeof(GET_Cam2_Boresight_X_note_t));
	cube_bit_reg[GET_Cam2_Boresight_Y].init_bit(&cube_reg[CAR_Get_CubeSensConfig],26,0,bits_len_union_t::_16bits_,_16UL,GET_Cam2_Boresight_Y_note_t,sizeof(GET_Cam2_Boresight_Y_note_t));
	cube_bit_reg[GET_Nadir_Max_Devia_Percent].init_bit(&cube_reg[CAR_Get_CubeSensConfig],28,0,bits_len_union_t::_8bits_,_8UL,GET_Nadir_Max_Devia_Percent_note_t,sizeof(GET_Nadir_Max_Devia_Percent_note_t));
	cube_bit_reg[GET_Nadir_Max_BadEdges].init_bit(&cube_reg[CAR_Get_CubeSensConfig],29,0,bits_len_union_t::_8bits_,_8UL,GET_Nadir_Max_BadEdges_note_t,sizeof(GET_Nadir_Max_BadEdges_note_t));
	cube_bit_reg[GET_Nadir_Max_Radius].init_bit(&cube_reg[CAR_Get_CubeSensConfig],30,0,bits_len_union_t::_8bits_,_8UL,GET_Nadir_Max_Radius_note_t,sizeof(GET_Nadir_Max_Radius_note_t));
	cube_bit_reg[GET_Nadir_Min_Radius].init_bit(&cube_reg[CAR_Get_CubeSensConfig],31,0,bits_len_union_t::_8bits_,_8UL,GET_Nadir_Min_Radius_note_t,sizeof(GET_Nadir_Min_Radius_note_t));
	cube_bit_reg[GET_Cam1_MinX_Area1].init_bit(&cube_reg[CAR_Get_CubeSensConfig],32,0,bits_len_union_t::_16bits_,_16UL,GET_Cam1_MinX_Area1_note_t,sizeof(GET_Cam1_MinX_Area1_note_t));
	cube_bit_reg[GET_Cam1_MaxX_Area1].init_bit(&cube_reg[CAR_Get_CubeSensConfig],34,0,bits_len_union_t::_16bits_,_16UL,GET_Cam1_MaxX_Area1_note_t,sizeof(GET_Cam1_MaxX_Area1_note_t));
	cube_bit_reg[GET_Cam1_MinY_Area1].init_bit(&cube_reg[CAR_Get_CubeSensConfig],36,0,bits_len_union_t::_16bits_,_16UL,GET_Cam1_MinY_Area1_note_t,sizeof(GET_Cam1_MinY_Area1_note_t));
	cube_bit_reg[GET_Cam1_MaxY_Area1].init_bit(&cube_reg[CAR_Get_CubeSensConfig],38,0,bits_len_union_t::_16bits_,_16UL,GET_Cam1_MaxY_Area1_note_t,sizeof(GET_Cam1_MaxY_Area1_note_t));
	cube_bit_reg[GET_Cam1_MinX_Area2].init_bit(&cube_reg[CAR_Get_CubeSensConfig],40,0,bits_len_union_t::_16bits_,_16UL,GET_Cam1_MinX_Area2_note_t,sizeof(GET_Cam1_MinX_Area2_note_t));
	cube_bit_reg[GET_Cam1_MaxX_Area2].init_bit(&cube_reg[CAR_Get_CubeSensConfig],42,0,bits_len_union_t::_16bits_,_16UL,GET_Cam1_MaxX_Area2_note_t,sizeof(GET_Cam1_MaxX_Area2_note_t));
	cube_bit_reg[GET_Cam1_MinY_Area2].init_bit(&cube_reg[CAR_Get_CubeSensConfig],44,0,bits_len_union_t::_16bits_,_16UL,GET_Cam1_MinY_Area2_note_t,sizeof(GET_Cam1_MinY_Area2_note_t));
	cube_bit_reg[GET_Cam1_MaxY_Area2].init_bit(&cube_reg[CAR_Get_CubeSensConfig],46,0,bits_len_union_t::_16bits_,_16UL,GET_Cam1_MaxY_Area2_note_t,sizeof(GET_Cam1_MaxY_Area2_note_t));
	cube_bit_reg[GET_Cam1_MinX_Area3].init_bit(&cube_reg[CAR_Get_CubeSensConfig],48,0,bits_len_union_t::_16bits_,_16UL,GET_Cam1_MinX_Area3_note_t,sizeof(GET_Cam1_MinX_Area3_note_t));
	cube_bit_reg[GET_Cam1_MaxX_Area3].init_bit(&cube_reg[CAR_Get_CubeSensConfig],50,0,bits_len_union_t::_16bits_,_16UL,GET_Cam1_MaxX_Area3_note_t,sizeof(GET_Cam1_MaxX_Area3_note_t));
	cube_bit_reg[GET_Cam1_MinY_Area3].init_bit(&cube_reg[CAR_Get_CubeSensConfig],52,0,bits_len_union_t::_16bits_,_16UL,GET_Cam1_MinY_Area3_note_t,sizeof(GET_Cam1_MinY_Area3_note_t));
	cube_bit_reg[GET_Cam1_MaxY_Area3].init_bit(&cube_reg[CAR_Get_CubeSensConfig],54,0,bits_len_union_t::_16bits_,_16UL,GET_Cam1_MaxY_Area3_note_t,sizeof(GET_Cam1_MaxY_Area3_note_t));
	cube_bit_reg[GET_Cam1_MinX_Area4].init_bit(&cube_reg[CAR_Get_CubeSensConfig],56,0,bits_len_union_t::_16bits_,_16UL,GET_Cam1_MinX_Area4_note_t,sizeof(GET_Cam1_MinX_Area4_note_t));
	cube_bit_reg[GET_Cam1_MaxX_Area4].init_bit(&cube_reg[CAR_Get_CubeSensConfig],58,0,bits_len_union_t::_16bits_,_16UL,GET_Cam1_MaxX_Area4_note_t,sizeof(GET_Cam1_MaxX_Area4_note_t));
	cube_bit_reg[GET_Cam1_MinY_Area4].init_bit(&cube_reg[CAR_Get_CubeSensConfig],60,0,bits_len_union_t::_16bits_,_16UL,GET_Cam1_MinY_Area4_note_t,sizeof(GET_Cam1_MinY_Area4_note_t));
	cube_bit_reg[GET_Cam1_MaxY_Area4].init_bit(&cube_reg[CAR_Get_CubeSensConfig],62,0,bits_len_union_t::_16bits_,_16UL,GET_Cam1_MaxY_Area4_note_t,sizeof(GET_Cam1_MaxY_Area4_note_t));
	cube_bit_reg[GET_Cam1_MinX_Area5].init_bit(&cube_reg[CAR_Get_CubeSensConfig],64,0,bits_len_union_t::_16bits_,_16UL,GET_Cam1_MinX_Area5_note_t,sizeof(GET_Cam1_MinX_Area5_note_t));
	cube_bit_reg[GET_Cam1_MaxX_Area5].init_bit(&cube_reg[CAR_Get_CubeSensConfig],66,0,bits_len_union_t::_16bits_,_16UL,GET_Cam1_MaxX_Area5_note_t,sizeof(GET_Cam1_MaxX_Area5_note_t));
	cube_bit_reg[GET_Cam1_MinY_Area5].init_bit(&cube_reg[CAR_Get_CubeSensConfig],68,0,bits_len_union_t::_16bits_,_16UL,GET_Cam1_MinY_Area5_note_t,sizeof(GET_Cam1_MinY_Area5_note_t));
	cube_bit_reg[GET_Cam1_MaxY_Area5].init_bit(&cube_reg[CAR_Get_CubeSensConfig],70,0,bits_len_union_t::_16bits_,_16UL,GET_Cam1_MaxY_Area5_note_t,sizeof(GET_Cam1_MaxY_Area5_note_t));
	cube_bit_reg[GET_Cam2_MinX_Area1].init_bit(&cube_reg[CAR_Get_CubeSensConfig],72,0,bits_len_union_t::_16bits_,_16UL,GET_Cam2_MinX_Area1_note_t,sizeof(GET_Cam2_MinX_Area1_note_t));
	cube_bit_reg[GET_Cam2_MaxX_Area1].init_bit(&cube_reg[CAR_Get_CubeSensConfig],74,0,bits_len_union_t::_16bits_,_16UL,GET_Cam2_MaxX_Area1_note_t,sizeof(GET_Cam2_MaxX_Area1_note_t));
	cube_bit_reg[GET_Cam2_MinY_Area1].init_bit(&cube_reg[CAR_Get_CubeSensConfig],76,0,bits_len_union_t::_16bits_,_16UL,GET_Cam2_MinY_Area1_note_t,sizeof(GET_Cam2_MinY_Area1_note_t));
	cube_bit_reg[GET_Cam2_MaxY_Area1].init_bit(&cube_reg[CAR_Get_CubeSensConfig],78,0,bits_len_union_t::_16bits_,_16UL,GET_Cam2_MaxY_Area1_note_t,sizeof(GET_Cam2_MaxY_Area1_note_t));
	cube_bit_reg[GET_Cam2_MinX_Area2].init_bit(&cube_reg[CAR_Get_CubeSensConfig],80,0,bits_len_union_t::_16bits_,_16UL,GET_Cam2_MinX_Area2_note_t,sizeof(GET_Cam2_MinX_Area2_note_t));
	cube_bit_reg[GET_Cam2_MaxX_Area2].init_bit(&cube_reg[CAR_Get_CubeSensConfig],82,0,bits_len_union_t::_16bits_,_16UL,GET_Cam2_MaxX_Area2_note_t,sizeof(GET_Cam2_MaxX_Area2_note_t));
	cube_bit_reg[GET_Cam2_MinY_Area2].init_bit(&cube_reg[CAR_Get_CubeSensConfig],84,0,bits_len_union_t::_16bits_,_16UL,GET_Cam2_MinY_Area2_note_t,sizeof(GET_Cam2_MinY_Area2_note_t));
	cube_bit_reg[GET_Cam2_MaxY_Area2].init_bit(&cube_reg[CAR_Get_CubeSensConfig],86,0,bits_len_union_t::_16bits_,_16UL,GET_Cam2_MaxY_Area2_note_t,sizeof(GET_Cam2_MaxY_Area2_note_t));
	cube_bit_reg[GET_Cam2_MinX_Area3].init_bit(&cube_reg[CAR_Get_CubeSensConfig],88,0,bits_len_union_t::_16bits_,_16UL,GET_Cam2_MinX_Area3_note_t,sizeof(GET_Cam2_MinX_Area3_note_t));
	cube_bit_reg[GET_Cam2_MaxX_Area3].init_bit(&cube_reg[CAR_Get_CubeSensConfig],90,0,bits_len_union_t::_16bits_,_16UL,GET_Cam2_MaxX_Area3_note_t,sizeof(GET_Cam2_MaxX_Area3_note_t));
	cube_bit_reg[GET_Cam2_MinY_Area3].init_bit(&cube_reg[CAR_Get_CubeSensConfig],92,0,bits_len_union_t::_16bits_,_16UL,GET_Cam2_MinY_Area3_note_t,sizeof(GET_Cam2_MinY_Area3_note_t));
	cube_bit_reg[GET_Cam2_MaxY_Area3].init_bit(&cube_reg[CAR_Get_CubeSensConfig],94,0,bits_len_union_t::_16bits_,_16UL,GET_Cam2_MaxY_Area3_note_t,sizeof(GET_Cam2_MaxY_Area3_note_t));
	cube_bit_reg[GET_Cam2_MinX_Area4].init_bit(&cube_reg[CAR_Get_CubeSensConfig],96,0,bits_len_union_t::_16bits_,_16UL,GET_Cam2_MinX_Area4_note_t,sizeof(GET_Cam2_MinX_Area4_note_t));
	cube_bit_reg[GET_Cam2_MaxX_Area4].init_bit(&cube_reg[CAR_Get_CubeSensConfig],98,0,bits_len_union_t::_16bits_,_16UL,GET_Cam2_MaxX_Area4_note_t,sizeof(GET_Cam2_MaxX_Area4_note_t));
	cube_bit_reg[GET_Cam2_MinY_Area4].init_bit(&cube_reg[CAR_Get_CubeSensConfig],100,0,bits_len_union_t::_16bits_,_16UL,GET_Cam2_MinY_Area4_note_t,sizeof(GET_Cam2_MinY_Area4_note_t));
	cube_bit_reg[GET_Cam2_MaxY_Area4].init_bit(&cube_reg[CAR_Get_CubeSensConfig],102,0,bits_len_union_t::_16bits_,_16UL,GET_Cam2_MaxY_Area4_note_t,sizeof(GET_Cam2_MaxY_Area4_note_t));
	cube_bit_reg[GET_Cam2_MinX_Area5].init_bit(&cube_reg[CAR_Get_CubeSensConfig],104,0,bits_len_union_t::_16bits_,_16UL,GET_Cam2_MinX_Area5_note_t,sizeof(GET_Cam2_MinX_Area5_note_t));
	cube_bit_reg[GET_Cam2_MaxX_Area5].init_bit(&cube_reg[CAR_Get_CubeSensConfig],106,0,bits_len_union_t::_16bits_,_16UL,GET_Cam2_MaxX_Area5_note_t,sizeof(GET_Cam2_MaxX_Area5_note_t));
	cube_bit_reg[GET_Cam2_MinY_Area5].init_bit(&cube_reg[CAR_Get_CubeSensConfig],108,0,bits_len_union_t::_16bits_,_16UL,GET_Cam2_MinY_Area5_note_t,sizeof(GET_Cam2_MinY_Area5_note_t));
	cube_bit_reg[GET_Cam2_MaxY_Area5].init_bit(&cube_reg[CAR_Get_CubeSensConfig],110,0,bits_len_union_t::_16bits_,_16UL,GET_Cam2_MaxY_Area5_note_t,sizeof(GET_Cam2_MaxY_Area5_note_t));
	cube_bit_reg[GET_MTMMountAlpha].init_bit(&cube_reg[CAR_Get_MTMConfig],0,0,bits_len_union_t::_16bits_,_16L,GET_MTMMountAlpha_note_t,sizeof(GET_MTMMountAlpha_note_t));
	cube_bit_reg[GET_MTMMountBeta].init_bit(&cube_reg[CAR_Get_MTMConfig],2,0,bits_len_union_t::_16bits_,_16L,GET_MTMMountBeta_note_t,sizeof(GET_MTMMountBeta_note_t));
	cube_bit_reg[GET_MTMMountGamma].init_bit(&cube_reg[CAR_Get_MTMConfig],4,0,bits_len_union_t::_16bits_,_16L,GET_MTMMountGamma_note_t,sizeof(GET_MTMMountGamma_note_t));
	cube_bit_reg[GET_MTM_Ch1Offset].init_bit(&cube_reg[CAR_Get_MTMConfig],6,0,bits_len_union_t::_16bits_,_16L,GET_MTM_Ch1Offset_note_t,sizeof(GET_MTM_Ch1Offset_note_t));
	cube_bit_reg[GET_MTM_Ch2Offset].init_bit(&cube_reg[CAR_Get_MTMConfig],8,0,bits_len_union_t::_16bits_,_16L,GET_MTM_Ch2Offset_note_t,sizeof(GET_MTM_Ch2Offset_note_t));
	cube_bit_reg[GET_MTM_Ch3Offset].init_bit(&cube_reg[CAR_Get_MTMConfig],10,0,bits_len_union_t::_16bits_,_16L,GET_MTM_Ch3Offset_note_t,sizeof(GET_MTM_Ch3Offset_note_t));
	cube_bit_reg[GET_MTM_S11].init_bit(&cube_reg[CAR_Get_MTMConfig],12,0,bits_len_union_t::_16bits_,_16L,GET_MTM_S11_note_t,sizeof(GET_MTM_S11_note_t));
	cube_bit_reg[GET_MTM_S22].init_bit(&cube_reg[CAR_Get_MTMConfig],14,0,bits_len_union_t::_16bits_,_16L,GET_MTM_S22_note_t,sizeof(GET_MTM_S22_note_t));
	cube_bit_reg[GET_MTM_S33].init_bit(&cube_reg[CAR_Get_MTMConfig],16,0,bits_len_union_t::_16bits_,_16L,GET_MTM_S33_note_t,sizeof(GET_MTM_S33_note_t));
	cube_bit_reg[GET_MTM_S12].init_bit(&cube_reg[CAR_Get_MTMConfig],18,0,bits_len_union_t::_16bits_,_16L,GET_MTM_S12_note_t,sizeof(GET_MTM_S12_note_t));
	cube_bit_reg[GET_MTM_S13].init_bit(&cube_reg[CAR_Get_MTMConfig],20,0,bits_len_union_t::_16bits_,_16L,GET_MTM_S13_note_t,sizeof(GET_MTM_S13_note_t));
	cube_bit_reg[GET_MTM_S21].init_bit(&cube_reg[CAR_Get_MTMConfig],22,0,bits_len_union_t::_16bits_,_16L,GET_MTM_S21_note_t,sizeof(GET_MTM_S21_note_t));
	cube_bit_reg[GET_MTM_S23].init_bit(&cube_reg[CAR_Get_MTMConfig],24,0,bits_len_union_t::_16bits_,_16L,GET_MTM_S23_note_t,sizeof(GET_MTM_S23_note_t));
	cube_bit_reg[GET_MTM_S31].init_bit(&cube_reg[CAR_Get_MTMConfig],26,0,bits_len_union_t::_16bits_,_16L,GET_MTM_S31_note_t,sizeof(GET_MTM_S31_note_t));
	cube_bit_reg[GET_MTM_S32].init_bit(&cube_reg[CAR_Get_MTMConfig],28,0,bits_len_union_t::_16bits_,_16L,GET_MTM_S32_note_t,sizeof(GET_MTM_S32_note_t));
	cube_bit_reg[GET_Inclination].init_bit(&cube_reg[CAR_Get_SGP4OrbitPara],0,0,bits_len_union_t::_64bits_,_64L,GET_Inclination_note_t,sizeof(GET_Inclination_note_t));
	cube_bit_reg[GET_Eccentricity].init_bit(&cube_reg[CAR_Get_SGP4OrbitPara],8,0,bits_len_union_t::_64bits_,_64L,GET_Eccentricity_note_t,sizeof(GET_Eccentricity_note_t));
	cube_bit_reg[GET_RightAscensionOfTheAscendingNode].init_bit(&cube_reg[CAR_Get_SGP4OrbitPara],16,0,bits_len_union_t::_64bits_,_64L,GET_RightAscensionOfTheAscendingNode_note_t,sizeof(GET_RightAscensionOfTheAscendingNode_note_t));
	cube_bit_reg[GET_ArgumentOfPerigee].init_bit(&cube_reg[CAR_Get_SGP4OrbitPara],24,0,bits_len_union_t::_64bits_,_64L,GET_ArgumentOfPerigee_note_t,sizeof(GET_ArgumentOfPerigee_note_t));
	cube_bit_reg[GET_BStarDragTerm].init_bit(&cube_reg[CAR_Get_SGP4OrbitPara],32,0,bits_len_union_t::_64bits_,_64L,GET_BStarDragTerm_note_t,sizeof(GET_BStarDragTerm_note_t));
	cube_bit_reg[GET_MeanMotion].init_bit(&cube_reg[CAR_Get_SGP4OrbitPara],40,0,bits_len_union_t::_64bits_,_64L,GET_MeanMotion_note_t,sizeof(GET_MeanMotion_note_t));
	cube_bit_reg[GET_MeanAnomaly].init_bit(&cube_reg[CAR_Get_SGP4OrbitPara],48,0,bits_len_union_t::_64bits_,_64L,GET_MeanAnomaly_note_t,sizeof(GET_MeanAnomaly_note_t));
	cube_bit_reg[GET_Epoch].init_bit(&cube_reg[CAR_Get_SGP4OrbitPara],56,0,bits_len_union_t::_64bits_,_64L,GET_Epoch_note_t,sizeof(GET_Epoch_note_t));
	cube_bit_reg[GET_Detumb_SpinGain].init_bit(&cube_reg[CAR_Get_DetumbCtrl_Para],0,0,bits_len_union_t::_32bits_,_32F,GET_Detumb_SpinGain_note_t,sizeof(GET_Detumb_SpinGain_note_t));
	cube_bit_reg[GET_Detumb_DampGain].init_bit(&cube_reg[CAR_Get_DetumbCtrl_Para],4,0,bits_len_union_t::_32bits_,_32F,GET_Detumb_DampGain_note_t,sizeof(GET_Detumb_DampGain_note_t));
	cube_bit_reg[GET_Ref_SpinRate].init_bit(&cube_reg[CAR_Get_DetumbCtrl_Para],8,0,bits_len_union_t::_16bits_,_16L,GET_Ref_SpinRate_note_t,sizeof(GET_Ref_SpinRate_note_t));
	cube_bit_reg[GET_FastBdot_DetumbGain].init_bit(&cube_reg[CAR_Get_DetumbCtrl_Para],10,0,bits_len_union_t::_32bits_,_32F,GET_FastBdot_DetumbGain_note_t,sizeof(GET_FastBdot_DetumbGain_note_t));
	cube_bit_reg[GET_Ymoment_CtrlGain].init_bit(&cube_reg[CAR_Get_YWheel_Ctrl_Para],0,0,bits_len_union_t::_32bits_,_16UL,GET_Ymoment_CtrlGain_note_t,sizeof(GET_Ymoment_CtrlGain_note_t));
	cube_bit_reg[GET_Ymoment_NDampGain].init_bit(&cube_reg[CAR_Get_YWheel_Ctrl_Para],4,0,bits_len_union_t::_32bits_,_32F,GET_Ymoment_NDampGain_note_t,sizeof(GET_Ymoment_NDampGain_note_t));
	cube_bit_reg[GET_Ymoment_PGain].init_bit(&cube_reg[CAR_Get_YWheel_Ctrl_Para],8,0,bits_len_union_t::_32bits_,_32F,GET_Ymoment_PGain_note_t,sizeof(GET_Ymoment_PGain_note_t));
	cube_bit_reg[GET_Ymoment_DGain].init_bit(&cube_reg[CAR_Get_YWheel_Ctrl_Para],12,0,bits_len_union_t::_32bits_,_32F,GET_Ymoment_DGain_note_t,sizeof(GET_Ymoment_DGain_note_t));
	cube_bit_reg[GET_RefWheel_Moment].init_bit(&cube_reg[CAR_Get_YWheel_Ctrl_Para],16,0,bits_len_union_t::_32bits_,_32F,GET_RefWheel_Moment_note_t,sizeof(GET_RefWheel_Moment_note_t));
	cube_bit_reg[Raw_RateX].init_bit(&cube_reg[CAR_RawRate_Sensor],0,0,bits_len_union_t::_16bits_,_16L,Raw_RateX_note_t,sizeof(Raw_RateX_note_t));
	cube_bit_reg[Raw_RateY].init_bit(&cube_reg[CAR_RawRate_Sensor],2,0,bits_len_union_t::_16bits_,_16L,Raw_RateY_note_t,sizeof(Raw_RateY_note_t));
	cube_bit_reg[Raw_RateZ].init_bit(&cube_reg[CAR_RawRate_Sensor],4,0,bits_len_union_t::_16bits_,_16L,Raw_RateZ_note_t,sizeof(Raw_RateZ_note_t));
	cube_bit_reg[GET_Rwheel_PGain].init_bit(&cube_reg[CAR_Get_RW_Ctrl_Para],0,0,bits_len_union_t::_32bits_,_32F,GET_Rwheel_PGain_note_t,sizeof(GET_Rwheel_PGain_note_t));
	cube_bit_reg[GET_Rwheel_DGain].init_bit(&cube_reg[CAR_Get_RW_Ctrl_Para],4,0,bits_len_union_t::_32bits_,_32F,GET_Rwheel_DGain_note_t,sizeof(GET_Rwheel_DGain_note_t));
	cube_bit_reg[GET_YWheelBiasMoment].init_bit(&cube_reg[CAR_Get_RW_Ctrl_Para],8,0,bits_len_union_t::_32bits_,_32F,GET_YWheelBiasMoment_note_t,sizeof(GET_YWheelBiasMoment_note_t));
	cube_bit_reg[GET_SunPointFacet].init_bit(&cube_reg[CAR_Get_RW_Ctrl_Para],12,0,bits_len_union_t::_7bits_,_8UL,GET_SunPointFacet_note_t,sizeof(GET_SunPointFacet_note_t));
	cube_bit_reg[GET_AutoCtrl_TransitDueWheelErr].init_bit(&cube_reg[CAR_Get_RW_Ctrl_Para],12,7,bits_len_union_t::_1bits_,_8UL,GET_AutoCtrl_TransitDueWheelErr_note_t,sizeof(GET_AutoCtrl_TransitDueWheelErr_note_t));
	cube_bit_reg[Est_Q1].init_bit(&cube_reg[CAR_EstQuaternion],0,0,bits_len_union_t::_16bits_,_16L,Est_Q1_note_t,sizeof(Est_Q1_note_t));
	cube_bit_reg[Est_Q2].init_bit(&cube_reg[CAR_EstQuaternion],2,0,bits_len_union_t::_16bits_,_16L,Est_Q2_note_t,sizeof(Est_Q2_note_t));
	cube_bit_reg[Est_Q3].init_bit(&cube_reg[CAR_EstQuaternion],4,0,bits_len_union_t::_16bits_,_16L,Est_Q3_note_t,sizeof(Est_Q3_note_t));
	cube_bit_reg[ECEF_Pos_X].init_bit(&cube_reg[CAR_ECEF_Position],0,0,bits_len_union_t::_16bits_,_16L,ECEF_Pos_X_note_t,sizeof(ECEF_Pos_X_note_t));
	cube_bit_reg[ECEF_Pos_Y].init_bit(&cube_reg[CAR_ECEF_Position],2,0,bits_len_union_t::_16bits_,_16L,ECEF_Pos_Y_note_t,sizeof(ECEF_Pos_Y_note_t));
	cube_bit_reg[ECEF_Pos_Z].init_bit(&cube_reg[CAR_ECEF_Position],4,0,bits_len_union_t::_16bits_,_16L,ECEF_Pos_Z_note_t,sizeof(ECEF_Pos_Z_note_t));
	cube_bit_reg[TimeSince_IterationStart].init_bit(&cube_reg[CAR_ACP_Exe_State],0,0,bits_len_union_t::_16bits_,_16L,TimeSince_IterationStart_note_t,sizeof(TimeSince_IterationStart_note_t));
	cube_bit_reg[Current_ExePoint].init_bit(&cube_reg[CAR_ACP_Exe_State],2,0,bits_len_union_t::_8bits_,_8UL,Current_ExePoint_note_t,sizeof(Current_ExePoint_note_t));
	cube_bit_reg[GET_M_Of_IXX].init_bit(&cube_reg[CAR_Get_I_Matrix],0,0,bits_len_union_t::_32bits_,_32F,GET_M_Of_IXX_note_t,sizeof(GET_M_Of_IXX_note_t));
	cube_bit_reg[GET_M_Of_IYY].init_bit(&cube_reg[CAR_Get_I_Matrix],4,0,bits_len_union_t::_32bits_,_32F,GET_M_Of_IYY_note_t,sizeof(GET_M_Of_IYY_note_t));
	cube_bit_reg[GET_M_Of_IZZ].init_bit(&cube_reg[CAR_Get_I_Matrix],8,0,bits_len_union_t::_32bits_,_32F,GET_M_Of_IZZ_note_t,sizeof(GET_M_Of_IZZ_note_t));
	cube_bit_reg[GET_M_Of_IXY].init_bit(&cube_reg[CAR_Get_I_Matrix],12,0,bits_len_union_t::_32bits_,_32F,GET_M_Of_IXY_note_t,sizeof(GET_M_Of_IXY_note_t));
	cube_bit_reg[GET_M_Of_IXZ].init_bit(&cube_reg[CAR_Get_I_Matrix],16,0,bits_len_union_t::_32bits_,_32F,GET_M_Of_IXZ_note_t,sizeof(GET_M_Of_IXZ_note_t));
	cube_bit_reg[GET_M_Of_IYZ].init_bit(&cube_reg[CAR_Get_I_Matrix],20,0,bits_len_union_t::_32bits_,_32F,GET_M_Of_IYZ_note_t,sizeof(GET_M_Of_IYZ_note_t));
	cube_bit_reg[GET_MTMRateFilterSysNoise].init_bit(&cube_reg[CAR_Get_EstPara],0,0,bits_len_union_t::_32bits_,_32F,GET_MTMRateFilterSysNoise_note_t,sizeof(GET_MTMRateFilterSysNoise_note_t));
	cube_bit_reg[GET_EKFSysNoise].init_bit(&cube_reg[CAR_Get_EstPara],4,0,bits_len_union_t::_32bits_,_32F,GET_EKFSysNoise_note_t,sizeof(GET_EKFSysNoise_note_t));
	cube_bit_reg[GET_CSSMeasureNoise].init_bit(&cube_reg[CAR_Get_EstPara],8,0,bits_len_union_t::_32bits_,_32F,GET_CSSMeasureNoise_note_t,sizeof(GET_CSSMeasureNoise_note_t));
	cube_bit_reg[GET_FSSMeasureNoise].init_bit(&cube_reg[CAR_Get_EstPara],12,0,bits_len_union_t::_32bits_,_32F,GET_FSSMeasureNoise_note_t,sizeof(GET_FSSMeasureNoise_note_t));
	cube_bit_reg[GET_NadirSensMeasureNoise].init_bit(&cube_reg[CAR_Get_EstPara],16,0,bits_len_union_t::_32bits_,_32F,GET_NadirSensMeasureNoise_note_t,sizeof(GET_NadirSensMeasureNoise_note_t));
	cube_bit_reg[GET_MTMMeasureNoise].init_bit(&cube_reg[CAR_Get_EstPara],20,0,bits_len_union_t::_32bits_,_32F,GET_MTMMeasureNoise_note_t,sizeof(GET_MTMMeasureNoise_note_t));
	cube_bit_reg[GET_STMeasureNoise].init_bit(&cube_reg[CAR_Get_EstPara],24,0,bits_len_union_t::_32bits_,_32F,GET_STMeasureNoise_note_t,sizeof(GET_STMeasureNoise_note_t));
	cube_bit_reg[GET_UseFSS].init_bit(&cube_reg[CAR_Get_EstPara],28,0,bits_len_union_t::_1bits_,_8UL,GET_UseFSS_note_t,sizeof(GET_UseFSS_note_t));
	cube_bit_reg[GET_UseNadirSens].init_bit(&cube_reg[CAR_Get_EstPara],28,1,bits_len_union_t::_1bits_,_8UL,GET_UseNadirSens_note_t,sizeof(GET_UseNadirSens_note_t));
	cube_bit_reg[GET_UseCSS].init_bit(&cube_reg[CAR_Get_EstPara],28,2,bits_len_union_t::_1bits_,_8UL,GET_UseCSS_note_t,sizeof(GET_UseCSS_note_t));
	cube_bit_reg[GET_UseST].init_bit(&cube_reg[CAR_Get_EstPara],28,3,bits_len_union_t::_1bits_,_8UL,GET_UseST_note_t,sizeof(GET_UseST_note_t));
	cube_bit_reg[GET_NadirSensterminatortest].init_bit(&cube_reg[CAR_Get_EstPara],28,4,bits_len_union_t::_1bits_,_8UL,GET_NadirSensterminatortest_note_t,sizeof(GET_NadirSensterminatortest_note_t));
	cube_bit_reg[GET_AutoMTMRecovery].init_bit(&cube_reg[CAR_Get_EstPara],28,5,bits_len_union_t::_1bits_,_8UL,GET_AutoMTMRecovery_note_t,sizeof(GET_AutoMTMRecovery_note_t));
	cube_bit_reg[GET_MTMMode].init_bit(&cube_reg[CAR_Get_EstPara],28,6,bits_len_union_t::_2bits_,_8UL,GET_MTMMode_note_t,sizeof(GET_MTMMode_note_t));
	cube_bit_reg[GET_MTMSelectForRAW_MTM_TLM].init_bit(&cube_reg[CAR_Get_EstPara],29,0,bits_len_union_t::_2bits_,_8UL,GET_MTMSelectForRAW_MTM_TLM_note_t,sizeof(GET_MTMSelectForRAW_MTM_TLM_note_t));
	cube_bit_reg[GET_AutoEstTransitDueToRateSensErr].init_bit(&cube_reg[CAR_Get_EstPara],29,2,bits_len_union_t::_1bits_,_8UL,GET_AutoEstTransitDueToRateSensErr_note_t,sizeof(GET_AutoEstTransitDueToRateSensErr_note_t));
	cube_bit_reg[GET_Cam1Cam2SampPeriod].init_bit(&cube_reg[CAR_Get_EstPara],30,0,bits_len_union_t::_8bits_,_8UL,GET_Cam1Cam2SampPeriod_note_t,sizeof(GET_Cam1Cam2SampPeriod_note_t));
	cube_bit_reg[OrbitPara_Are_Invalid].init_bit(&cube_reg[CAR_CurrADCSState2],0,0,bits_len_union_t::_1bits_,_8UL,OrbitPara_Are_Invalid_note_t,sizeof(OrbitPara_Are_Invalid_note_t));
	cube_bit_reg[Config_Is_Invalid].init_bit(&cube_reg[CAR_CurrADCSState2],0,1,bits_len_union_t::_1bits_,_8UL,Config_Is_Invalid_note_t,sizeof(Config_Is_Invalid_note_t));
	cube_bit_reg[CtrlMode_ChangeisntAllowed].init_bit(&cube_reg[CAR_CurrADCSState2],0,2,bits_len_union_t::_1bits_,_8UL,CtrlMode_ChangeisntAllowed_note_t,sizeof(CtrlMode_ChangeisntAllowed_note_t));
	cube_bit_reg[Est_ChangeisntAllowed].init_bit(&cube_reg[CAR_CurrADCSState2],0,3,bits_len_union_t::_1bits_,_8UL,Est_ChangeisntAllowed_note_t,sizeof(Est_ChangeisntAllowed_note_t));
	cube_bit_reg[CurrMTMSampMode].init_bit(&cube_reg[CAR_CurrADCSState2],0,4,bits_len_union_t::_2bits_,_8UL,CurrMTMSampMode_note_t,sizeof(CurrMTMSampMode_note_t));
	cube_bit_reg[ModelAndMeaMF_DiffersInSize].init_bit(&cube_reg[CAR_CurrADCSState2],0,6,bits_len_union_t::_1bits_,_8UL,ModelAndMeaMF_DiffersInSize_note_t,sizeof(ModelAndMeaMF_DiffersInSize_note_t));
	cube_bit_reg[Node_Recovery_Err].init_bit(&cube_reg[CAR_CurrADCSState2],0,7,bits_len_union_t::_1bits_,_8UL,Node_Recovery_Err_note_t,sizeof(Node_Recovery_Err_note_t));
	cube_bit_reg[CS1_Runtime_Err].init_bit(&cube_reg[CAR_CurrADCSState2],1,0,bits_len_union_t::_1bits_,_8UL,CS1_Runtime_Err_note_t,sizeof(CS1_Runtime_Err_note_t));
	cube_bit_reg[CS2_Runtime_Err].init_bit(&cube_reg[CAR_CurrADCSState2],1,1,bits_len_union_t::_1bits_,_8UL,CS2_Runtime_Err_note_t,sizeof(CS2_Runtime_Err_note_t));
	cube_bit_reg[CubeCtrl_Signal_Runtime_Err].init_bit(&cube_reg[CAR_CurrADCSState2],1,2,bits_len_union_t::_1bits_,_8UL,CubeCtrl_Signal_Runtime_Err_note_t,sizeof(CubeCtrl_Signal_Runtime_Err_note_t));
	cube_bit_reg[CubeCtrl_Motor_Runtime_Err].init_bit(&cube_reg[CAR_CurrADCSState2],1,3,bits_len_union_t::_1bits_,_8UL,CubeCtrl_Motor_Runtime_Err_note_t,sizeof(CubeCtrl_Motor_Runtime_Err_note_t));
	cube_bit_reg[CubeW1_Runtime_Err].init_bit(&cube_reg[CAR_CurrADCSState2],1,4,bits_len_union_t::_1bits_,_8UL,CubeW1_Runtime_Err_note_t,sizeof(CubeW1_Runtime_Err_note_t));
	cube_bit_reg[CubeW2_Runtime_Err].init_bit(&cube_reg[CAR_CurrADCSState2],1,5,bits_len_union_t::_1bits_,_8UL,CubeW2_Runtime_Err_note_t,sizeof(CubeW2_Runtime_Err_note_t));
	cube_bit_reg[CubeW3_Runtime_Err].init_bit(&cube_reg[CAR_CurrADCSState2],1,6,bits_len_union_t::_1bits_,_8UL,CubeW3_Runtime_Err_note_t,sizeof(CubeW3_Runtime_Err_note_t));
	cube_bit_reg[CubeStar_Runtime_Err].init_bit(&cube_reg[CAR_CurrADCSState2],1,7,bits_len_union_t::_1bits_,_8UL,CubeStar_Runtime_Err_note_t,sizeof(CubeStar_Runtime_Err_note_t));
	cube_bit_reg[MTM_Err].init_bit(&cube_reg[CAR_CurrADCSState2],2,0,bits_len_union_t::_1bits_,_8UL,MTM_Err_note_t,sizeof(MTM_Err_note_t));
	cube_bit_reg[RateSensor_Failure].init_bit(&cube_reg[CAR_CurrADCSState2],2,1,bits_len_union_t::_1bits_,_8UL,RateSensor_Failure_note_t,sizeof(RateSensor_Failure_note_t));
	cube_bit_reg[GET_ACP_Type].init_bit(&cube_reg[CAR_Get_ADCS_Config],0,0,bits_len_union_t::_4bits_,_8UL,GET_ACP_Type_note_t,sizeof(GET_ACP_Type_note_t));
	cube_bit_reg[GET_Special_Ctrl_Select].init_bit(&cube_reg[CAR_Get_ADCS_Config],0,4,bits_len_union_t::_4bits_,_8UL,GET_Special_Ctrl_Select_note_t,sizeof(GET_Special_Ctrl_Select_note_t));
	cube_bit_reg[GET_CubeCtrlSig_Ver].init_bit(&cube_reg[CAR_Get_ADCS_Config],1,0,bits_len_union_t::_8bits_,_8UL,GET_CubeCtrlSig_Ver_note_t,sizeof(GET_CubeCtrlSig_Ver_note_t));
	cube_bit_reg[GET_CubeCtrlMotor_Ver].init_bit(&cube_reg[CAR_Get_ADCS_Config],2,0,bits_len_union_t::_8bits_,_8UL,GET_CubeCtrlMotor_Ver_note_t,sizeof(GET_CubeCtrlMotor_Ver_note_t));
	cube_bit_reg[GET_CS1_Ver].init_bit(&cube_reg[CAR_Get_ADCS_Config],3,0,bits_len_union_t::_8bits_,_8UL,GET_CS1_Ver_note_t,sizeof(GET_CS1_Ver_note_t));
	cube_bit_reg[GET_CS2_Ver].init_bit(&cube_reg[CAR_Get_ADCS_Config],4,0,bits_len_union_t::_8bits_,_8UL,GET_CS2_Ver_note_t,sizeof(GET_CS2_Ver_note_t));
	cube_bit_reg[GET_CS1_CamType].init_bit(&cube_reg[CAR_Get_ADCS_Config],5,0,bits_len_union_t::_4bits_,_8UL,GET_CS1_CamType_note_t,sizeof(GET_CS1_CamType_note_t));
	cube_bit_reg[GET_CS2_CamType].init_bit(&cube_reg[CAR_Get_ADCS_Config],5,4,bits_len_union_t::_4bits_,_8UL,GET_CS2_CamType_note_t,sizeof(GET_CS2_CamType_note_t));
	cube_bit_reg[GET_CubeStar_Ver].init_bit(&cube_reg[CAR_Get_ADCS_Config],6,0,bits_len_union_t::_8bits_,_8UL,GET_CubeStar_Ver_note_t,sizeof(GET_CubeStar_Ver_note_t));
	cube_bit_reg[GET_GPS_Type].init_bit(&cube_reg[CAR_Get_ADCS_Config],7,0,bits_len_union_t::_4bits_,_8UL,GET_GPS_Type_note_t,sizeof(GET_GPS_Type_note_t));
	cube_bit_reg[GET_Redun_MTM_Includ].init_bit(&cube_reg[CAR_Get_ADCS_Config],7,4,bits_len_union_t::_1bits_,_8UL,GET_Redun_MTM_Includ_note_t,sizeof(GET_Redun_MTM_Includ_note_t));
	cube_bit_reg[GET_MTQX_MaxDipole].init_bit(&cube_reg[CAR_Get_ADCS_Config],8,0,bits_len_union_t::_32bits_,_32F,GET_MTQX_MaxDipole_note_t,sizeof(GET_MTQX_MaxDipole_note_t));
	cube_bit_reg[GET_MTQY_MaxDipole].init_bit(&cube_reg[CAR_Get_ADCS_Config],12,0,bits_len_union_t::_32bits_,_32F,GET_MTQY_MaxDipole_note_t,sizeof(GET_MTQY_MaxDipole_note_t));
	cube_bit_reg[GET_MTQZ_MaxDipole].init_bit(&cube_reg[CAR_Get_ADCS_Config],16,0,bits_len_union_t::_32bits_,_32F,GET_MTQZ_MaxDipole_note_t,sizeof(GET_MTQZ_MaxDipole_note_t));
	cube_bit_reg[GET_MTQ_Ontime_Res].init_bit(&cube_reg[CAR_Get_ADCS_Config],20,0,bits_len_union_t::_32bits_,_32F,GET_MTQ_Ontime_Res_note_t,sizeof(GET_MTQ_Ontime_Res_note_t));
	cube_bit_reg[GET_MTQ_Max_OnTime].init_bit(&cube_reg[CAR_Get_ADCS_Config],24,0,bits_len_union_t::_32bits_,_32F,GET_MTQ_Max_OnTime_note_t,sizeof(GET_MTQ_Max_OnTime_note_t));
	cube_bit_reg[GET_RWX_MaxTorque].init_bit(&cube_reg[CAR_Get_ADCS_Config],28,0,bits_len_union_t::_32bits_,_32F,GET_RWX_MaxTorque_note_t,sizeof(GET_RWX_MaxTorque_note_t));
	cube_bit_reg[GET_RWY_MaxTorque].init_bit(&cube_reg[CAR_Get_ADCS_Config],32,0,bits_len_union_t::_32bits_,_32F,GET_RWY_MaxTorque_note_t,sizeof(GET_RWY_MaxTorque_note_t));
	cube_bit_reg[GET_RWZ_MaxTorque].init_bit(&cube_reg[CAR_Get_ADCS_Config],36,0,bits_len_union_t::_32bits_,_32F,GET_RWZ_MaxTorque_note_t,sizeof(GET_RWZ_MaxTorque_note_t));
	cube_bit_reg[GET_RWX_MaxMomentum].init_bit(&cube_reg[CAR_Get_ADCS_Config],40,0,bits_len_union_t::_32bits_,_32F,GET_RWX_MaxMomentum_note_t,sizeof(GET_RWX_MaxMomentum_note_t));
	cube_bit_reg[GET_RWY_MaxMomentum].init_bit(&cube_reg[CAR_Get_ADCS_Config],44,0,bits_len_union_t::_32bits_,_32F,GET_RWY_MaxMomentum_note_t,sizeof(GET_RWY_MaxMomentum_note_t));
	cube_bit_reg[GET_RWZ_MaxMomentum].init_bit(&cube_reg[CAR_Get_ADCS_Config],48,0,bits_len_union_t::_32bits_,_32F,GET_RWZ_MaxMomentum_note_t,sizeof(GET_RWZ_MaxMomentum_note_t));
	cube_bit_reg[GET_RWX_Inertia].init_bit(&cube_reg[CAR_Get_ADCS_Config],52,0,bits_len_union_t::_32bits_,_32F,GET_RWX_Inertia_note_t,sizeof(GET_RWX_Inertia_note_t));
	cube_bit_reg[GET_RWY_Inertia].init_bit(&cube_reg[CAR_Get_ADCS_Config],56,0,bits_len_union_t::_32bits_,_32F,GET_RWY_Inertia_note_t,sizeof(GET_RWY_Inertia_note_t));
	cube_bit_reg[GET_RWZ_Inertia].init_bit(&cube_reg[CAR_Get_ADCS_Config],60,0,bits_len_union_t::_32bits_,_32F,GET_RWZ_Inertia_note_t,sizeof(GET_RWZ_Inertia_note_t));
	cube_bit_reg[GET_RW_Torque_Increment].init_bit(&cube_reg[CAR_Get_ADCS_Config],64,0,bits_len_union_t::_32bits_,_32F,GET_RW_Torque_Increment_note_t,sizeof(GET_RW_Torque_Increment_note_t));
	cube_bit_reg[GET_Prim_MTMXBias_d1].init_bit(&cube_reg[CAR_Get_ADCS_Config],68,0,bits_len_union_t::_32bits_,_32F,GET_Prim_MTMXBias_d1_note_t,sizeof(GET_Prim_MTMXBias_d1_note_t));
	cube_bit_reg[GET_Prim_MTMYBias_d1].init_bit(&cube_reg[CAR_Get_ADCS_Config],72,0,bits_len_union_t::_32bits_,_32F,GET_Prim_MTMYBias_d1_note_t,sizeof(GET_Prim_MTMYBias_d1_note_t));
	cube_bit_reg[GET_Prim_MTMZBias_d1].init_bit(&cube_reg[CAR_Get_ADCS_Config],76,0,bits_len_union_t::_32bits_,_32F,GET_Prim_MTMZBias_d1_note_t,sizeof(GET_Prim_MTMZBias_d1_note_t));
	cube_bit_reg[GET_Prim_MTMXBias_d2].init_bit(&cube_reg[CAR_Get_ADCS_Config],80,0,bits_len_union_t::_32bits_,_32F,GET_Prim_MTMXBias_d2_note_t,sizeof(GET_Prim_MTMXBias_d2_note_t));
	cube_bit_reg[GET_Prim_MTMYBias_d2].init_bit(&cube_reg[CAR_Get_ADCS_Config],84,0,bits_len_union_t::_32bits_,_32F,GET_Prim_MTMYBias_d2_note_t,sizeof(GET_Prim_MTMYBias_d2_note_t));
	cube_bit_reg[GET_Prim_MTMZBias_d2].init_bit(&cube_reg[CAR_Get_ADCS_Config],88,0,bits_len_union_t::_32bits_,_32F,GET_Prim_MTMZBias_d2_note_t,sizeof(GET_Prim_MTMZBias_d2_note_t));
	cube_bit_reg[GET_Prim_MTMXSens_s1].init_bit(&cube_reg[CAR_Get_ADCS_Config],92,0,bits_len_union_t::_32bits_,_32F,GET_Prim_MTMXSens_s1_note_t,sizeof(GET_Prim_MTMXSens_s1_note_t));
	cube_bit_reg[GET_Prim_MTMYSens_s1].init_bit(&cube_reg[CAR_Get_ADCS_Config],96,0,bits_len_union_t::_32bits_,_32F,GET_Prim_MTMYSens_s1_note_t,sizeof(GET_Prim_MTMYSens_s1_note_t));
	cube_bit_reg[GET_Prim_MTMZSens_s1].init_bit(&cube_reg[CAR_Get_ADCS_Config],100,0,bits_len_union_t::_32bits_,_32F,GET_Prim_MTMZSens_s1_note_t,sizeof(GET_Prim_MTMZSens_s1_note_t));
	cube_bit_reg[GET_Prim_MTMXSens_s2].init_bit(&cube_reg[CAR_Get_ADCS_Config],104,0,bits_len_union_t::_32bits_,_32F,GET_Prim_MTMXSens_s2_note_t,sizeof(GET_Prim_MTMXSens_s2_note_t));
	cube_bit_reg[GET_Prim_MTMYSens_s2].init_bit(&cube_reg[CAR_Get_ADCS_Config],108,0,bits_len_union_t::_32bits_,_32F,GET_Prim_MTMYSens_s2_note_t,sizeof(GET_Prim_MTMYSens_s2_note_t));
	cube_bit_reg[GET_Prim_MTMZSens_s2].init_bit(&cube_reg[CAR_Get_ADCS_Config],112,0,bits_len_union_t::_32bits_,_32F,GET_Prim_MTMZSens_s2_note_t,sizeof(GET_Prim_MTMZSens_s2_note_t));
	cube_bit_reg[GET_Redun_MTMXBias_d1].init_bit(&cube_reg[CAR_Get_ADCS_Config],116,0,bits_len_union_t::_32bits_,_32F,GET_Redun_MTMXBias_d1_note_t,sizeof(GET_Redun_MTMXBias_d1_note_t));
	cube_bit_reg[GET_Redun_MTMYBias_d1].init_bit(&cube_reg[CAR_Get_ADCS_Config],120,0,bits_len_union_t::_32bits_,_32F,GET_Redun_MTMYBias_d1_note_t,sizeof(GET_Redun_MTMYBias_d1_note_t));
	cube_bit_reg[GET_Redun_MTMZBias_d1].init_bit(&cube_reg[CAR_Get_ADCS_Config],124,0,bits_len_union_t::_32bits_,_32F,GET_Redun_MTMZBias_d1_note_t,sizeof(GET_Redun_MTMZBias_d1_note_t));
	cube_bit_reg[GET_Redun_MTMXBias_d2].init_bit(&cube_reg[CAR_Get_ADCS_Config],128,0,bits_len_union_t::_32bits_,_32F,GET_Redun_MTMXBias_d2_note_t,sizeof(GET_Redun_MTMXBias_d2_note_t));
	cube_bit_reg[GET_Redun_MTMYBias_d2].init_bit(&cube_reg[CAR_Get_ADCS_Config],132,0,bits_len_union_t::_32bits_,_32F,GET_Redun_MTMYBias_d2_note_t,sizeof(GET_Redun_MTMYBias_d2_note_t));
	cube_bit_reg[GET_Redun_MTMZBias_d2].init_bit(&cube_reg[CAR_Get_ADCS_Config],136,0,bits_len_union_t::_32bits_,_32F,GET_Redun_MTMZBias_d2_note_t,sizeof(GET_Redun_MTMZBias_d2_note_t));
	cube_bit_reg[GET_Redun_MTMXSens_s1].init_bit(&cube_reg[CAR_Get_ADCS_Config],140,0,bits_len_union_t::_32bits_,_32F,GET_Redun_MTMXSens_s1_note_t,sizeof(GET_Redun_MTMXSens_s1_note_t));
	cube_bit_reg[GET_Redun_MTMYSens_s1].init_bit(&cube_reg[CAR_Get_ADCS_Config],144,0,bits_len_union_t::_32bits_,_32F,GET_Redun_MTMYSens_s1_note_t,sizeof(GET_Redun_MTMYSens_s1_note_t));
	cube_bit_reg[GET_Redun_MTMZSens_s1].init_bit(&cube_reg[CAR_Get_ADCS_Config],148,0,bits_len_union_t::_32bits_,_32F,GET_Redun_MTMZSens_s1_note_t,sizeof(GET_Redun_MTMZSens_s1_note_t));
	cube_bit_reg[GET_Redun_MTMXSens_s2].init_bit(&cube_reg[CAR_Get_ADCS_Config],152,0,bits_len_union_t::_32bits_,_32F,GET_Redun_MTMXSens_s2_note_t,sizeof(GET_Redun_MTMXSens_s2_note_t));
	cube_bit_reg[GET_Redun_MTMYSens_s2].init_bit(&cube_reg[CAR_Get_ADCS_Config],156,0,bits_len_union_t::_32bits_,_32F,GET_Redun_MTMYSens_s2_note_t,sizeof(GET_Redun_MTMYSens_s2_note_t));
	cube_bit_reg[GET_Redun_MTMZSens_s2].init_bit(&cube_reg[CAR_Get_ADCS_Config],160,0,bits_len_union_t::_32bits_,_32F,GET_Redun_MTMZSens_s2_note_t,sizeof(GET_Redun_MTMZSens_s2_note_t));
	cube_bit_reg[GET_CCSignal_En_Port].init_bit(&cube_reg[CAR_Get_ADCS_Config],164,0,bits_len_union_t::_4bits_,_8UL,GET_CCSignal_En_Port_note_t,sizeof(GET_CCSignal_En_Port_note_t));
	cube_bit_reg[GET_CCSignal_En_Pin].init_bit(&cube_reg[CAR_Get_ADCS_Config],164,4,bits_len_union_t::_4bits_,_8UL,GET_CCSignal_En_Pin_note_t,sizeof(GET_CCSignal_En_Pin_note_t));
	cube_bit_reg[GET_CCMotor_En_Port].init_bit(&cube_reg[CAR_Get_ADCS_Config],165,0,bits_len_union_t::_4bits_,_8UL,GET_CCMotor_En_Port_note_t,sizeof(GET_CCMotor_En_Port_note_t));
	cube_bit_reg[GET_CCMotor_En_Pin].init_bit(&cube_reg[CAR_Get_ADCS_Config],165,4,bits_len_union_t::_4bits_,_8UL,GET_CCMotor_En_Pin_note_t,sizeof(GET_CCMotor_En_Pin_note_t));
	cube_bit_reg[GET_CCCommon_En_Port].init_bit(&cube_reg[CAR_Get_ADCS_Config],166,0,bits_len_union_t::_4bits_,_8UL,GET_CCCommon_En_Port_note_t,sizeof(GET_CCCommon_En_Port_note_t));
	cube_bit_reg[GET_CCCommon_En_Pin].init_bit(&cube_reg[CAR_Get_ADCS_Config],166,4,bits_len_union_t::_4bits_,_8UL,GET_CCCommon_En_Pin_note_t,sizeof(GET_CCCommon_En_Pin_note_t));
	cube_bit_reg[GET_CS1_En_Port].init_bit(&cube_reg[CAR_Get_ADCS_Config],167,0,bits_len_union_t::_4bits_,_8UL,GET_CS1_En_Port_note_t,sizeof(GET_CS1_En_Port_note_t));
	cube_bit_reg[GET_CS1_En_Pin].init_bit(&cube_reg[CAR_Get_ADCS_Config],167,4,bits_len_union_t::_4bits_,_8UL,GET_CS1_En_Pin_note_t,sizeof(GET_CS1_En_Pin_note_t));
	cube_bit_reg[GET_CS2_En_Port].init_bit(&cube_reg[CAR_Get_ADCS_Config],168,0,bits_len_union_t::_4bits_,_8UL,GET_CS2_En_Port_note_t,sizeof(GET_CS2_En_Port_note_t));
	cube_bit_reg[GET_CS2_En_Pin].init_bit(&cube_reg[CAR_Get_ADCS_Config],168,4,bits_len_union_t::_4bits_,_8UL,GET_CS2_En_Pin_note_t,sizeof(GET_CS2_En_Pin_note_t));
	cube_bit_reg[GET_CubeStar_En_Port].init_bit(&cube_reg[CAR_Get_ADCS_Config],169,0,bits_len_union_t::_4bits_,_8UL,GET_CubeStar_En_Port_note_t,sizeof(GET_CubeStar_En_Port_note_t));
	cube_bit_reg[GET_CubeStar_En_Pin].init_bit(&cube_reg[CAR_Get_ADCS_Config],169,4,bits_len_union_t::_4bits_,_8UL,GET_CubeStar_En_Pin_note_t,sizeof(GET_CubeStar_En_Pin_note_t));
	cube_bit_reg[GET_CW1_En_Port].init_bit(&cube_reg[CAR_Get_ADCS_Config],170,0,bits_len_union_t::_4bits_,_8UL,GET_CW1_En_Port_note_t,sizeof(GET_CW1_En_Port_note_t));
	cube_bit_reg[GET_CW1_En_Pin].init_bit(&cube_reg[CAR_Get_ADCS_Config],170,4,bits_len_union_t::_4bits_,_8UL,GET_CW1_En_Pin_note_t,sizeof(GET_CW1_En_Pin_note_t));
	cube_bit_reg[GET_CW2_En_Port].init_bit(&cube_reg[CAR_Get_ADCS_Config],171,0,bits_len_union_t::_4bits_,_8UL,GET_CW2_En_Port_note_t,sizeof(GET_CW2_En_Port_note_t));
	cube_bit_reg[GET_CW2_En_Pin].init_bit(&cube_reg[CAR_Get_ADCS_Config],171,4,bits_len_union_t::_4bits_,_8UL,GET_CW2_En_Pin_note_t,sizeof(GET_CW2_En_Pin_note_t));
	cube_bit_reg[GET_CW3_En_Port].init_bit(&cube_reg[CAR_Get_ADCS_Config],172,0,bits_len_union_t::_4bits_,_8UL,GET_CW3_En_Port_note_t,sizeof(GET_CW3_En_Port_note_t));
	cube_bit_reg[GET_CW3_En_Pin].init_bit(&cube_reg[CAR_Get_ADCS_Config],172,4,bits_len_union_t::_4bits_,_8UL,GET_CW3_En_Pin_note_t,sizeof(GET_CW3_En_Pin_note_t));
	cube_bit_reg[GET_Incl_Coeff].init_bit(&cube_reg[CAR_Get_Aug_SGP4_Para],0,0,bits_len_union_t::_16bits_,_16UL,GET_Incl_Coeff_note_t,sizeof(GET_Incl_Coeff_note_t));
	cube_bit_reg[GET_Raan_Coeff].init_bit(&cube_reg[CAR_Get_Aug_SGP4_Para],2,0,bits_len_union_t::_16bits_,_16UL,GET_Raan_Coeff_note_t,sizeof(GET_Raan_Coeff_note_t));
	cube_bit_reg[GET_Ecc_Coeff].init_bit(&cube_reg[CAR_Get_Aug_SGP4_Para],4,0,bits_len_union_t::_16bits_,_16UL,GET_Ecc_Coeff_note_t,sizeof(GET_Ecc_Coeff_note_t));
	cube_bit_reg[GET_Aop_Coeff].init_bit(&cube_reg[CAR_Get_Aug_SGP4_Para],6,0,bits_len_union_t::_16bits_,_16UL,GET_Aop_Coeff_note_t,sizeof(GET_Aop_Coeff_note_t));
	cube_bit_reg[GET_Time_Coeff].init_bit(&cube_reg[CAR_Get_Aug_SGP4_Para],8,0,bits_len_union_t::_16bits_,_16UL,GET_Time_Coeff_note_t,sizeof(GET_Time_Coeff_note_t));
	cube_bit_reg[GET_Pos_Coeff].init_bit(&cube_reg[CAR_Get_Aug_SGP4_Para],10,0,bits_len_union_t::_16bits_,_16UL,GET_Pos_Coeff_note_t,sizeof(GET_Pos_Coeff_note_t));
	cube_bit_reg[GET_Max_Pos_Err].init_bit(&cube_reg[CAR_Get_Aug_SGP4_Para],12,0,bits_len_union_t::_8bits_,_8UL,GET_Max_Pos_Err_note_t,sizeof(GET_Max_Pos_Err_note_t));
	cube_bit_reg[GET_ASGP4_Filter].init_bit(&cube_reg[CAR_Get_Aug_SGP4_Para],13,0,bits_len_union_t::_8bits_,_32L,GET_ASGP4_Filter_note_t,sizeof(GET_ASGP4_Filter_note_t));
	cube_bit_reg[GET_xp_Coeff].init_bit(&cube_reg[CAR_Get_Aug_SGP4_Para],14,0,bits_len_union_t::_32bits_,_32L,GET_xp_Coeff_note_t,sizeof(GET_xp_Coeff_note_t));
	cube_bit_reg[GET_yp_Coeff].init_bit(&cube_reg[CAR_Get_Aug_SGP4_Para],18,0,bits_len_union_t::_32bits_,_8UL,GET_yp_Coeff_note_t,sizeof(GET_yp_Coeff_note_t));
	cube_bit_reg[GET_GPS_RollOver].init_bit(&cube_reg[CAR_Get_Aug_SGP4_Para],22,0,bits_len_union_t::_8bits_,_8UL,GET_GPS_RollOver_note_t,sizeof(GET_GPS_RollOver_note_t));
	cube_bit_reg[GET_Pos_sd].init_bit(&cube_reg[CAR_Get_Aug_SGP4_Para],23,0,bits_len_union_t::_8bits_,_8UL,GET_Pos_sd_note_t,sizeof(GET_Pos_sd_note_t));
	cube_bit_reg[GET_Velo_sd ].init_bit(&cube_reg[CAR_Get_Aug_SGP4_Para],24,0,bits_len_union_t::_8bits_,_8UL,GET_Velo_sd_note_t,sizeof(GET_Velo_sd_note_t));
	cube_bit_reg[GET_Min_satellites].init_bit(&cube_reg[CAR_Get_Aug_SGP4_Para],25,0,bits_len_union_t::_8bits_,_8UL,GET_Min_satellites_note_t,sizeof(GET_Min_satellites_note_t));
	cube_bit_reg[GET_Time_Gain].init_bit(&cube_reg[CAR_Get_Aug_SGP4_Para],26,0,bits_len_union_t::_8bits_,_8UL,GET_Time_Gain_note_t,sizeof(GET_Time_Gain_note_t));
	cube_bit_reg[GET_Max_Lag].init_bit(&cube_reg[CAR_Get_Aug_SGP4_Para],27,0,bits_len_union_t::_8bits_,_8UL,GET_Max_Lag_note_t,sizeof(GET_Max_Lag_note_t));
	cube_bit_reg[GET_Min_Samples].init_bit(&cube_reg[CAR_GET_ASPG4_TLE],28,0,bits_len_union_t::_16bits_,_16UL,GET_Min_Samples_note_t,sizeof(GET_Min_Samples_note_t));
	cube_bit_reg[ASGP4_Complete].init_bit(&cube_reg[CAR_GET_ASPG4_TLE],0,0,bits_len_union_t::_1bits_,_8UL,ASGP4_Complete_note_t,sizeof(ASGP4_Complete_note_t));
	cube_bit_reg[ASGP4_Error].init_bit(&cube_reg[CAR_GET_ASPG4_TLE],0,1,bits_len_union_t::_7bits_,_8UL,ASGP4_Error_note_t,sizeof(ASGP4_Error_note_t));
	cube_bit_reg[ASGP4_Epoch].init_bit(&cube_reg[CAR_GET_ASPG4_TLE],1,0,bits_len_union_t::_32bits_,_32F,ASGP4_Epoch_note_t,sizeof(ASGP4_Epoch_note_t));
	cube_bit_reg[ASGP4_Inclination].init_bit(&cube_reg[CAR_GET_ASPG4_TLE],5,0,bits_len_union_t::_32bits_,_32F,ASGP4_Inclination_note_t,sizeof(ASGP4_Inclination_note_t));
	cube_bit_reg[ASGP4_RAAN].init_bit(&cube_reg[CAR_GET_ASPG4_TLE],9,0,bits_len_union_t::_32bits_,_32F,ASGP4_RAAN_note_t,sizeof(ASGP4_RAAN_note_t));
	cube_bit_reg[ASGP4_ECC].init_bit(&cube_reg[CAR_GET_ASPG4_TLE],13,0,bits_len_union_t::_32bits_,_32F,ASGP4_ECC_note_t,sizeof(ASGP4_ECC_note_t));
	cube_bit_reg[ASGP4_AOP].init_bit(&cube_reg[CAR_GET_ASPG4_TLE],17,0,bits_len_union_t::_32bits_,_32F,ASGP4_AOP_note_t,sizeof(ASGP4_AOP_note_t));
	cube_bit_reg[ASGP4_MA].init_bit(&cube_reg[CAR_GET_ASPG4_TLE],21,0,bits_len_union_t::_32bits_,_32F,ASGP4_MA_note_t,sizeof(ASGP4_MA_note_t));
	cube_bit_reg[ASGP4_MM].init_bit(&cube_reg[CAR_GET_ASPG4_TLE],25,0,bits_len_union_t::_32bits_,_32F,ASGP4_MM_note_t,sizeof(ASGP4_MM_note_t));
	cube_bit_reg[ASGP4_Bstar].init_bit(&cube_reg[CAR_GET_ASPG4_TLE],29,0,bits_len_union_t::_32bits_,_32F,ASGP4_Bstar_note_t,sizeof(ASGP4_Bstar_note_t));
	cube_bit_reg[CS2_3V3_Current].init_bit(&cube_reg[CAR_CubeSense2CurrentMea],0,0,bits_len_union_t::_16bits_,_16L,CS2_3V3_Current_note_t,sizeof(CS2_3V3_Current_note_t));
	cube_bit_reg[CS2_SRAM_Current].init_bit(&cube_reg[CAR_CubeSense2CurrentMea],2,0,bits_len_union_t::_16bits_,_16L,CS2_SRAM_Current_note_t,sizeof(CS2_SRAM_Current_note_t));
	cube_bit_reg[Percentage_Complete].init_bit(&cube_reg[CAR_StatusImgCapSaveOp],0,0,bits_len_union_t::_8bits_,_8UL,Percentage_Complete_note_t,sizeof(Percentage_Complete_note_t));
	cube_bit_reg[Img_Cap_Status].init_bit(&cube_reg[CAR_StatusImgCapSaveOp],1,0,bits_len_union_t::_8bits_,_8UL,Img_Cap_Status_note_t,sizeof(Img_Cap_Status_note_t));
	cube_bit_reg[Format_Busy].init_bit(&cube_reg[CAR_SDcardFormat_EraseProgress],0,0,bits_len_union_t::_1bits_,_8UL,Format_Busy_note_t,sizeof(Format_Busy_note_t));
	cube_bit_reg[Erase_All_Busy].init_bit(&cube_reg[CAR_SDcardFormat_EraseProgress],0,1,bits_len_union_t::_1bits_,_8UL,Erase_All_Busy_note_t,sizeof(Erase_All_Busy_note_t));
	cube_bit_reg[GET_Log1_Selection].init_bit(&cube_reg[CAR_Get_SD_Log1_Config],0,0,bits_len_union_t::_80bits_,_80UL,GET_Log1_Selection_note_t,sizeof(GET_Log1_Selection_note_t));
	cube_bit_reg[GET_Log1_Period ].init_bit(&cube_reg[CAR_Get_SD_Log1_Config],10,0,bits_len_union_t::_16bits_,_16UL,GET_Log1_Period_note_t,sizeof(GET_Log1_Period_note_t));
	cube_bit_reg[GET_Log1_Destination].init_bit(&cube_reg[CAR_Get_SD_Log1_Config],12,0,bits_len_union_t::_8bits_,_8UL,GET_Log1_Destination_note_t,sizeof(GET_Log1_Destination_note_t));
	cube_bit_reg[GET_Log2_Selection].init_bit(&cube_reg[CAR_Get_SD_Log2_Config],0,0,bits_len_union_t::_80bits_,_80UL,GET_Log2_Selection_note_t,sizeof(GET_Log2_Selection_note_t));
	cube_bit_reg[GET_Log2_Period ].init_bit(&cube_reg[CAR_Get_SD_Log2_Config],10,0,bits_len_union_t::_16bits_,_16UL,GET_Log2_Period_note_t,sizeof(GET_Log2_Period_note_t));
	cube_bit_reg[GET_Log2_Destination].init_bit(&cube_reg[CAR_Get_SD_Log2_Config],12,0,bits_len_union_t::_8bits_,_8UL,GET_Log2_Destination_note_t,sizeof(GET_Log2_Destination_note_t));
	cube_bit_reg[GET_LogUART_Selection].init_bit(&cube_reg[CAR_Get_UART_Log_Config],0,0,bits_len_union_t::_80bits_,_80UL,GET_LogUART_Selection_note_t,sizeof(GET_LogUART_Selection_note_t));
	cube_bit_reg[GET_LogUART_Period ].init_bit(&cube_reg[CAR_Get_UART_Log_Config],10,0,bits_len_union_t::_16bits_,_16UL,GET_LogUART_Period_note_t,sizeof(GET_LogUART_Period_note_t));
	cube_bit_reg[GET_Inertial_RefX].init_bit(&cube_reg[CAR_Get_I_PointingRef_Vector],0,0,bits_len_union_t::_16bits_,_16L,GET_Inertial_RefX_note_t,sizeof(GET_Inertial_RefX_note_t));
	cube_bit_reg[GET_Inertial_RefY].init_bit(&cube_reg[CAR_Get_I_PointingRef_Vector],2,0,bits_len_union_t::_16bits_,_16L,GET_Inertial_RefY_note_t,sizeof(GET_Inertial_RefY_note_t));
	cube_bit_reg[GET_Inertial_RefZ].init_bit(&cube_reg[CAR_Get_I_PointingRef_Vector],4,0,bits_len_union_t::_16bits_,_16L,GET_Inertial_RefZ_note_t,sizeof(GET_Inertial_RefZ_note_t));
	cube_bit_reg[LastTCID].init_bit(&cube_reg[CAR_TLC_ACK],0,0,bits_len_union_t::_8bits_,_8UL,LastTCID_note_t,sizeof(LastTCID_note_t));
	cube_bit_reg[ProcessedFlag].init_bit(&cube_reg[CAR_TLC_ACK],1,0,bits_len_union_t::_1bits_,_8UL,ProcessedFlag_note_t,sizeof(ProcessedFlag_note_t));
	cube_bit_reg[TCErrStatus].init_bit(&cube_reg[CAR_TLC_ACK],2,0,bits_len_union_t::_8bits_,_8UL,TCErrStatus_note_t,sizeof(TCErrStatus_note_t));
	cube_bit_reg[TCParaErrIndex].init_bit(&cube_reg[CAR_TLC_ACK],3,0,bits_len_union_t::_8bits_,_8UL,TCParaErrIndex_note_t,sizeof(TCParaErrIndex_note_t));
	cube_bit_reg[FileDLBuff_Packet_Buff].init_bit(&cube_reg[CAR_FileDLBuff_with_FileContents],0,0,bits_len_union_t::_16bits_,_16UL,FileDLBuff_Packet_Buff_note_t,sizeof(FileDLBuff_Packet_Buff_note_t));
	cube_bit_reg[FileDLBuff_FileBytes].init_bit(&cube_reg[CAR_FileDLBuff_with_FileContents],2,0,bits_len_union_t::_160bits_,_160UL,FileDLBuff_FileBytes_note_t,sizeof(FileDLBuff_FileBytes_note_t));
	cube_bit_reg[DL_Block_Ready].init_bit(&cube_reg[CAR_DL_Block_Ready],0,0,bits_len_union_t::_1bits_,_8UL,DL_Block_Ready_note_t,sizeof(DL_Block_Ready_note_t));
	cube_bit_reg[DL_Block_ParaError].init_bit(&cube_reg[CAR_DL_Block_Ready],0,1,bits_len_union_t::_1bits_,_8UL,DL_Block_ParaError_note_t,sizeof(DL_Block_ParaError_note_t));
	cube_bit_reg[DL_Block_CRC16_Checksum].init_bit(&cube_reg[CAR_DL_Block_Ready],1,0,bits_len_union_t::_16bits_,_16UL,DL_Block_CRC16_Checksum_note_t,sizeof(DL_Block_CRC16_Checksum_note_t));
	cube_bit_reg[DL_Block_Length].init_bit(&cube_reg[CAR_DL_Block_Ready],3,0,bits_len_union_t::_16bits_,_16UL,DL_Block_Length_note_t,sizeof(DL_Block_Length_note_t));
	cube_bit_reg[File_Infor_Type].init_bit(&cube_reg[CAR_File_Infor],0,0,bits_len_union_t::_4bits_,_8UL,File_Infor_Type_note_t,sizeof(File_Infor_Type_note_t));
	cube_bit_reg[File_Infor_Busy_Updating].init_bit(&cube_reg[CAR_File_Infor],0,4,bits_len_union_t::_1bits_,_8UL,File_Infor_Busy_Updating_note_t,sizeof(File_Infor_Busy_Updating_note_t));
	cube_bit_reg[File_Infor_Ctr].init_bit(&cube_reg[CAR_File_Infor],1,0,bits_len_union_t::_8bits_,_8UL,File_Infor_Ctr_note_t,sizeof(File_Infor_Ctr_note_t));
	cube_bit_reg[File_Infor_Size].init_bit(&cube_reg[CAR_File_Infor],2,0,bits_len_union_t::_32bits_,_32UL,File_Infor_Size_note_t,sizeof(File_Infor_Size_note_t));
	cube_bit_reg[File_Infor_DateTime].init_bit(&cube_reg[CAR_File_Infor],6,0,bits_len_union_t::_32bits_,_32UL,File_Infor_DateTime_note_t,sizeof(File_Infor_DateTime_note_t));
	cube_bit_reg[File_Infor_CRC16_Checksum].init_bit(&cube_reg[CAR_File_Infor],10,0,bits_len_union_t::_16bits_,_16UL,File_Infor_CRC16_Checksum_note_t,sizeof(File_Infor_CRC16_Checksum_note_t));
	cube_bit_reg[Init_Upload_Complete].init_bit(&cube_reg[CAR_Init_upload_Complete],0,0,bits_len_union_t::_1bits_,_8UL,Init_Upload_Complete_note_t,sizeof(Init_Upload_Complete_note_t));
	cube_bit_reg[FinalUL_Block_Busy].init_bit(&cube_reg[CAR_Upload_Block_Complete],0,0,bits_len_union_t::_1bits_,_8UL,FinalUL_Block_Busy_note_t,sizeof(FinalUL_Block_Busy_note_t));
	cube_bit_reg[FinalUL_Block_Error].init_bit(&cube_reg[CAR_Upload_Block_Complete],0,1,bits_len_union_t::_1bits_,_8UL,FinalUL_Block_Error_note_t,sizeof(FinalUL_Block_Error_note_t));
	cube_bit_reg[FileUL_Block_CRC16_Checksum].init_bit(&cube_reg[CAR_Block_Checksum],0,0,bits_len_union_t::_16bits_,_16UL,FileUL_Block_CRC16_Checksum_note_t,sizeof(FileUL_Block_CRC16_Checksum_note_t));
	cube_bit_reg[GET_FileUL_HoleMap_1].init_bit(&cube_reg[CAR_Get_HoleMap1],0,0,bits_len_union_t::_128bits_,_128UL,GET_FileUL_HoleMap_1_note_t,sizeof(GET_FileUL_HoleMap_1_note_t));
	cube_bit_reg[GET_FileUL_HoleMap_2].init_bit(&cube_reg[CAR_Get_HoleMap2],0,0,bits_len_union_t::_128bits_,_128UL,GET_FileUL_HoleMap_2_note_t,sizeof(GET_FileUL_HoleMap_2_note_t));
	cube_bit_reg[GET_FileUL_HoleMap_3].init_bit(&cube_reg[CAR_Get_HoleMap3],0,0,bits_len_union_t::_128bits_,_128UL,GET_FileUL_HoleMap_3_note_t,sizeof(GET_FileUL_HoleMap_3_note_t));
	cube_bit_reg[GET_FileUL_HoleMap_4].init_bit(&cube_reg[CAR_Get_HoleMap4],0,0,bits_len_union_t::_128bits_,_128UL,GET_FileUL_HoleMap_4_note_t,sizeof(GET_FileUL_HoleMap_4_note_t));
	cube_bit_reg[GET_FileUL_HoleMap_5].init_bit(&cube_reg[CAR_Get_HoleMap5],0,0,bits_len_union_t::_128bits_,_128UL,GET_FileUL_HoleMap_5_note_t,sizeof(GET_FileUL_HoleMap_5_note_t));
	cube_bit_reg[GET_FileUL_HoleMap_6].init_bit(&cube_reg[CAR_Get_HoleMap6],0,0,bits_len_union_t::_128bits_,_128UL,GET_FileUL_HoleMap_6_note_t,sizeof(GET_FileUL_HoleMap_6_note_t));
	cube_bit_reg[GET_FileUL_HoleMap_7].init_bit(&cube_reg[CAR_Get_HoleMap7],0,0,bits_len_union_t::_128bits_,_128UL,GET_FileUL_HoleMap_7_note_t,sizeof(GET_FileUL_HoleMap_7_note_t));
	cube_bit_reg[GET_FileUL_HoleMap_8].init_bit(&cube_reg[CAR_Get_HoleMap8],0,0,bits_len_union_t::_128bits_,_128UL,GET_FileUL_HoleMap_8_note_t,sizeof(GET_FileUL_HoleMap_8_note_t));
	cube_bit_reg[Reset_Magic_number].init_bit(&cube_reg[CAR_Reset],0,0,bits_len_union_t::_8bits_,_8UL,Reset_Magic_number_note_t,sizeof(Reset_Magic_number_note_t));
	cube_bit_reg[SET_CurrentUnixTime].init_bit(&cube_reg[CAR_Set_UnixTime],0,0,bits_len_union_t::_32bits_,_32UL,SET_CurrentUnixTime_note_t,sizeof(SET_CurrentUnixTime_note_t));
	cube_bit_reg[SET_Milliseconds].init_bit(&cube_reg[CAR_Set_UnixTime],4,0,bits_len_union_t::_16bits_,_16UL,SET_Milliseconds_note_t,sizeof(SET_Milliseconds_note_t));
	cube_bit_reg[SET_Cache_Enable_state].init_bit(&cube_reg[CAR_Set_CacheEnabled_state],0,0,bits_len_union_t::_1bits_,_8UL,SET_Cache_Enable_state_note_t,sizeof(SET_Cache_Enable_state_note_t));
	cube_bit_reg[ResetLogPointer].init_bit(&cube_reg[CAR_ResetLogPointer],0,0,bits_len_union_t::_0bits_,_0UL,ResetLogPointer_note_t,sizeof(ResetLogPointer_note_t));
	cube_bit_reg[AdvanceLogPointer].init_bit(&cube_reg[CAR_AdvanceLogPointer],0,0,bits_len_union_t::_0bits_,_0UL,AdvanceLogPointer_note_t,sizeof(AdvanceLogPointer_note_t));
	cube_bit_reg[ResetBootReg].init_bit(&cube_reg[CAR_ResetBootReg],0,0,bits_len_union_t::_0bits_,_0UL,ResetBootReg_note_t,sizeof(ResetBootReg_note_t));
	cube_bit_reg[DeployMTMTimeout].init_bit(&cube_reg[CAR_DeployMTM],0,0,bits_len_union_t::_8bits_,_8UL,DeployMTMTimeout_note_t,sizeof(DeployMTMTimeout_note_t));
	cube_bit_reg[SET_SRAM_Scrub_Size].init_bit(&cube_reg[CAR_Set_SRAM_Scrub_Para],0,0,bits_len_union_t::_16bits_,_16UL,SET_SRAM_Scrub_Size_note_t,sizeof(SET_SRAM_Scrub_Size_note_t));
	cube_bit_reg[SET_Save_Now].init_bit(&cube_reg[CAR_Set_UnixTime_SavetoFlash],0,0,bits_len_union_t::_1bits_,_8UL,SET_Save_Now_note_t,sizeof(SET_Save_Now_note_t));
	cube_bit_reg[SET_Save_On_Update ].init_bit(&cube_reg[CAR_Set_UnixTime_SavetoFlash],0,1,bits_len_union_t::_1bits_,_8UL,SET_Save_On_Update_note_t,sizeof(SET_Save_On_Update_note_t));
	cube_bit_reg[SET_Save_Periodic].init_bit(&cube_reg[CAR_Set_UnixTime_SavetoFlash],0,2,bits_len_union_t::_1bits_,_8UL,SET_Save_Periodic_note_t,sizeof(SET_Save_Periodic_note_t));
	cube_bit_reg[SET_Period].init_bit(&cube_reg[CAR_Set_UnixTime_SavetoFlash],1,0,bits_len_union_t::_8bits_,_8UL,SET_Period_note_t,sizeof(SET_Period_note_t));
	cube_bit_reg[ADCSRunMode_bit].init_bit(&cube_reg[CAR_ADCSRunMode],0,0,bits_len_union_t::_8bits_,_8UL,ADCSRunMode_bit_note_t,sizeof(ADCSRunMode_bit_note_t));
	cube_bit_reg[SET_CubeCtrlSignalPowerSelect].init_bit(&cube_reg[CAR_Set_ADCSPowerCtrl],0,0,bits_len_union_t::_2bits_,_8UL,SET_CubeCtrlSignalPowerSelect_note_t,sizeof(SET_CubeCtrlSignalPowerSelect_note_t));
	cube_bit_reg[SET_CubeCtrlMotorPowerSelect].init_bit(&cube_reg[CAR_Set_ADCSPowerCtrl],0,2,bits_len_union_t::_2bits_,_8UL,SET_CubeCtrlMotorPowerSelect_note_t,sizeof(SET_CubeCtrlMotorPowerSelect_note_t));
	cube_bit_reg[SET_CubeSense1PowerSelect].init_bit(&cube_reg[CAR_Set_ADCSPowerCtrl],0,4,bits_len_union_t::_2bits_,_8UL,SET_CubeSense1PowerSelect_note_t,sizeof(SET_CubeSense1PowerSelect_note_t));
	cube_bit_reg[SET_CubeSense2PowerSelect].init_bit(&cube_reg[CAR_Set_ADCSPowerCtrl],0,6,bits_len_union_t::_2bits_,_8UL,SET_CubeSense2PowerSelect_note_t,sizeof(SET_CubeSense2PowerSelect_note_t));
	cube_bit_reg[SET_CubeStarPowerSelect].init_bit(&cube_reg[CAR_Set_ADCSPowerCtrl],1,0,bits_len_union_t::_2bits_,_8UL,SET_CubeStarPowerSelect_note_t,sizeof(SET_CubeStarPowerSelect_note_t));
	cube_bit_reg[SET_CubeWheel1PowerSelect].init_bit(&cube_reg[CAR_Set_ADCSPowerCtrl],1,2,bits_len_union_t::_2bits_,_8UL,SET_CubeWheel1PowerSelect_note_t,sizeof(SET_CubeWheel1PowerSelect_note_t));
	cube_bit_reg[SET_CubeWheel2PowerSelect].init_bit(&cube_reg[CAR_Set_ADCSPowerCtrl],1,4,bits_len_union_t::_2bits_,_8UL,SET_CubeWheel2PowerSelect_note_t,sizeof(SET_CubeWheel2PowerSelect_note_t));
	cube_bit_reg[SET_CubeWheel3PowerSelect].init_bit(&cube_reg[CAR_Set_ADCSPowerCtrl],1,6,bits_len_union_t::_2bits_,_8UL,SET_CubeWheel3PowerSelect_note_t,sizeof(SET_CubeWheel3PowerSelect_note_t));
	cube_bit_reg[SET_MotorPower].init_bit(&cube_reg[CAR_Set_ADCSPowerCtrl],2,0,bits_len_union_t::_2bits_,_8UL,SET_MotorPower_note_t,sizeof(SET_MotorPower_note_t));
	cube_bit_reg[SET_GPSPower].init_bit(&cube_reg[CAR_Set_ADCSPowerCtrl],2,2,bits_len_union_t::_2bits_,_8UL,SET_GPSPower_note_t,sizeof(SET_GPSPower_note_t));
	cube_bit_reg[Clear_Err_Flag].init_bit(&cube_reg[CAR_AcpClearErrors],0,0,bits_len_union_t::_1bits_,_8UL,Clear_Err_Flag_note_t,sizeof(Clear_Err_Flag_note_t));
	cube_bit_reg[Clear_HK_Err_Flag].init_bit(&cube_reg[CAR_AcpClearErrors],0,1,bits_len_union_t::_1bits_,_8UL,Clear_HK_Err_Flag_note_t,sizeof(Clear_HK_Err_Flag_note_t));
	cube_bit_reg[SET_CtrlMode].init_bit(&cube_reg[CAR_AttCtrlMode],0,0,bits_len_union_t::_8bits_,_8UL,SET_CtrlMode_note_t,sizeof(SET_CtrlMode_note_t));
	cube_bit_reg[CtrlModeTimeout].init_bit(&cube_reg[CAR_AttCtrlMode],1,0,bits_len_union_t::_16bits_,_8UL,CtrlModeTimeout_note_t,sizeof(CtrlModeTimeout_note_t));
	cube_bit_reg[AttEstMode_bit].init_bit(&cube_reg[CAR_AttEstMode],0,0,bits_len_union_t::_8bits_,_8UL,AttEstMode_bit_note_t,sizeof(AttEstMode_bit_note_t));
	cube_bit_reg[SET_Cmd_Roll].init_bit(&cube_reg[CAR_Set_CmdAttAngle],0,0,bits_len_union_t::_16bits_,_16L,SET_Cmd_Roll_note_t,sizeof(SET_Cmd_Roll_note_t));
	cube_bit_reg[SET_Cmd_Pitch].init_bit(&cube_reg[CAR_Set_CmdAttAngle],2,0,bits_len_union_t::_16bits_,_16L,SET_Cmd_Pitch_note_t,sizeof(SET_Cmd_Pitch_note_t));
	cube_bit_reg[SET_Cmd_Yaw].init_bit(&cube_reg[CAR_Set_CmdAttAngle],4,0,bits_len_union_t::_16bits_,_16L,SET_Cmd_Yaw_note_t,sizeof(SET_Cmd_Yaw_note_t));
	cube_bit_reg[CommandMTQX].init_bit(&cube_reg[CAR_MTQOP],0,0,bits_len_union_t::_16bits_,_16L,CommandMTQX_note_t,sizeof(CommandMTQX_note_t));
	cube_bit_reg[CommandMTQY].init_bit(&cube_reg[CAR_MTQOP],2,0,bits_len_union_t::_16bits_,_16L,CommandMTQY_note_t,sizeof(CommandMTQY_note_t));
	cube_bit_reg[CommandMTQZ].init_bit(&cube_reg[CAR_MTQOP],4,0,bits_len_union_t::_16bits_,_16L,CommandMTQZ_note_t,sizeof(CommandMTQZ_note_t));
	cube_bit_reg[CommandWheelXSpeed].init_bit(&cube_reg[CAR_WheelSpeed],0,0,bits_len_union_t::_16bits_,_16L,CommandWheelXSpeed_note_t,sizeof(CommandWheelXSpeed_note_t));
	cube_bit_reg[CommandWheelYSpeed].init_bit(&cube_reg[CAR_WheelSpeed],2,0,bits_len_union_t::_16bits_,_16L,CommandWheelYSpeed_note_t,sizeof(CommandWheelYSpeed_note_t));
	cube_bit_reg[CommandWheelZSpeed].init_bit(&cube_reg[CAR_WheelSpeed],4,0,bits_len_union_t::_16bits_,_16L,CommandWheelZSpeed_note_t,sizeof(CommandWheelZSpeed_note_t));
	cube_bit_reg[Trig_ADCSLoop].init_bit(&cube_reg[CAR_Trigger_ADCSLoop ],0,0,bits_len_union_t::_0bits_,_0UL,Trig_ADCSLoop_note_t,sizeof(Trig_ADCSLoop_note_t));
	cube_bit_reg[Iter_Unix_Time].init_bit(&cube_reg[CAR_Trigger_ADCSLoopSimSen],0,0,bits_len_union_t::_32bits_,_32UL,Iter_Unix_Time_note_t,sizeof(Iter_Unix_Time_note_t));
	cube_bit_reg[Iter_CSSRaw_1].init_bit(&cube_reg[CAR_Trigger_ADCSLoopSimSen],4,0,bits_len_union_t::_16bits_,_16UL,Iter_CSSRaw_1_note_t,sizeof(Iter_CSSRaw_1_note_t));
	cube_bit_reg[Iter_CSSRaw_2].init_bit(&cube_reg[CAR_Trigger_ADCSLoopSimSen],6,0,bits_len_union_t::_16bits_,_16UL,Iter_CSSRaw_2_note_t,sizeof(Iter_CSSRaw_2_note_t));
	cube_bit_reg[Iter_CSSRaw_3].init_bit(&cube_reg[CAR_Trigger_ADCSLoopSimSen],8,0,bits_len_union_t::_16bits_,_16UL,Iter_CSSRaw_3_note_t,sizeof(Iter_CSSRaw_3_note_t));
	cube_bit_reg[Iter_CSSRaw_4].init_bit(&cube_reg[CAR_Trigger_ADCSLoopSimSen],10,0,bits_len_union_t::_16bits_,_16UL,Iter_CSSRaw_4_note_t,sizeof(Iter_CSSRaw_4_note_t));
	cube_bit_reg[Iter_CSSRaw_5].init_bit(&cube_reg[CAR_Trigger_ADCSLoopSimSen],12,0,bits_len_union_t::_16bits_,_16UL,Iter_CSSRaw_5_note_t,sizeof(Iter_CSSRaw_5_note_t));
	cube_bit_reg[Iter_CSSRaw_6].init_bit(&cube_reg[CAR_Trigger_ADCSLoopSimSen],14,0,bits_len_union_t::_16bits_,_16UL,Iter_CSSRaw_6_note_t,sizeof(Iter_CSSRaw_6_note_t));
	cube_bit_reg[Iter_CSSRaw_7].init_bit(&cube_reg[CAR_Trigger_ADCSLoopSimSen],16,0,bits_len_union_t::_16bits_,_16UL,Iter_CSSRaw_7_note_t,sizeof(Iter_CSSRaw_7_note_t));
	cube_bit_reg[Iter_CSSRaw_8].init_bit(&cube_reg[CAR_Trigger_ADCSLoopSimSen],18,0,bits_len_union_t::_16bits_,_16UL,Iter_CSSRaw_8_note_t,sizeof(Iter_CSSRaw_8_note_t));
	cube_bit_reg[Iter_CSSRaw_9].init_bit(&cube_reg[CAR_Trigger_ADCSLoopSimSen],20,0,bits_len_union_t::_16bits_,_16UL,Iter_CSSRaw_9_note_t,sizeof(Iter_CSSRaw_9_note_t));
	cube_bit_reg[Iter_CSSRaw_10].init_bit(&cube_reg[CAR_Trigger_ADCSLoopSimSen],22,0,bits_len_union_t::_16bits_,_16UL,Iter_CSSRaw_10_note_t,sizeof(Iter_CSSRaw_10_note_t));
	cube_bit_reg[Iter_Cam1Raw_X].init_bit(&cube_reg[CAR_Trigger_ADCSLoopSimSen],24,0,bits_len_union_t::_16bits_,_16L,Iter_Cam1Raw_X_note_t,sizeof(Iter_Cam1Raw_X_note_t));
	cube_bit_reg[Iter_Cam1Raw_Y].init_bit(&cube_reg[CAR_Trigger_ADCSLoopSimSen],26,0,bits_len_union_t::_16bits_,_16L,Iter_Cam1Raw_Y_note_t,sizeof(Iter_Cam1Raw_Y_note_t));
	cube_bit_reg[Iter_Cam1Busy].init_bit(&cube_reg[CAR_Trigger_ADCSLoopSimSen],28,0,bits_len_union_t::_8bits_,_8UL,Iter_Cam1Busy_note_t,sizeof(Iter_Cam1Busy_note_t));
	cube_bit_reg[Iter_Cam1Result].init_bit(&cube_reg[CAR_Trigger_ADCSLoopSimSen],29,0,bits_len_union_t::_8bits_,_8UL,Iter_Cam1Result_note_t,sizeof(Iter_Cam1Result_note_t));
	cube_bit_reg[Iter_Cam2Raw_X].init_bit(&cube_reg[CAR_Trigger_ADCSLoopSimSen],30,0,bits_len_union_t::_16bits_,_16L,Iter_Cam2Raw_X_note_t,sizeof(Iter_Cam2Raw_X_note_t));
	cube_bit_reg[Iter_Cam2Raw_Y].init_bit(&cube_reg[CAR_Trigger_ADCSLoopSimSen],32,0,bits_len_union_t::_16bits_,_16L,Iter_Cam2Raw_Y_note_t,sizeof(Iter_Cam2Raw_Y_note_t));
	cube_bit_reg[Iter_Cam2Busy].init_bit(&cube_reg[CAR_Trigger_ADCSLoopSimSen],34,0,bits_len_union_t::_8bits_,_8UL,Iter_Cam2Busy_note_t,sizeof(Iter_Cam2Busy_note_t));
	cube_bit_reg[Iter_Cam2Result].init_bit(&cube_reg[CAR_Trigger_ADCSLoopSimSen],35,0,bits_len_union_t::_8bits_,_8UL,Iter_Cam2Result_note_t,sizeof(Iter_Cam2Result_note_t));
	cube_bit_reg[Iter_MagRaw_X].init_bit(&cube_reg[CAR_Trigger_ADCSLoopSimSen],36,0,bits_len_union_t::_16bits_,_16L,Iter_MagRaw_X_note_t,sizeof(Iter_MagRaw_X_note_t));
	cube_bit_reg[Iter_MagRaw_Y].init_bit(&cube_reg[CAR_Trigger_ADCSLoopSimSen],38,0,bits_len_union_t::_16bits_,_16L,Iter_MagRaw_Y_note_t,sizeof(Iter_MagRaw_Y_note_t));
	cube_bit_reg[Iter_MagRaw_Z].init_bit(&cube_reg[CAR_Trigger_ADCSLoopSimSen],40,0,bits_len_union_t::_16bits_,_16L,Iter_MagRaw_Z_note_t,sizeof(Iter_MagRaw_Z_note_t));
	cube_bit_reg[Iter_RateRaw_X].init_bit(&cube_reg[CAR_Trigger_ADCSLoopSimSen],42,0,bits_len_union_t::_32bits_,_32L,Iter_RateRaw_X_note_t,sizeof(Iter_RateRaw_X_note_t));
	cube_bit_reg[Iter_RateRaw_Y].init_bit(&cube_reg[CAR_Trigger_ADCSLoopSimSen],46,0,bits_len_union_t::_32bits_,_32L,Iter_RateRaw_Y_note_t,sizeof(Iter_RateRaw_Y_note_t));
	cube_bit_reg[Iter_RateRaw_Z].init_bit(&cube_reg[CAR_Trigger_ADCSLoopSimSen],50,0,bits_len_union_t::_32bits_,_32L,Iter_RateRaw_Z_note_t,sizeof(Iter_RateRaw_Z_note_t));
	cube_bit_reg[Iter_WheelRaw_X].init_bit(&cube_reg[CAR_Trigger_ADCSLoopSimSen],54,0,bits_len_union_t::_16bits_,_16L,Iter_WheelRaw_X_note_t,sizeof(Iter_WheelRaw_X_note_t));
	cube_bit_reg[Iter_WheelRaw_Y].init_bit(&cube_reg[CAR_Trigger_ADCSLoopSimSen],56,0,bits_len_union_t::_16bits_,_16L,Iter_WheelRaw_Y_note_t,sizeof(Iter_WheelRaw_Y_note_t));
	cube_bit_reg[Iter_WheelRaw_Z].init_bit(&cube_reg[CAR_Trigger_ADCSLoopSimSen],58,0,bits_len_union_t::_16bits_,_16L,Iter_WheelRaw_Z_note_t,sizeof(Iter_WheelRaw_Z_note_t));
	cube_bit_reg[Iter_Star1Camera_X ].init_bit(&cube_reg[CAR_Trigger_ADCSLoopSimSen],60,0,bits_len_union_t::_16bits_,_16L,Iter_Star1Camera_X_note_t,sizeof(Iter_Star1Camera_X_note_t));
	cube_bit_reg[Iter_Star1Camera_Y].init_bit(&cube_reg[CAR_Trigger_ADCSLoopSimSen],62,0,bits_len_union_t::_16bits_,_16L,Iter_Star1Camera_Y_note_t,sizeof(Iter_Star1Camera_Y_note_t));
	cube_bit_reg[Iter_Star1Camera_Z].init_bit(&cube_reg[CAR_Trigger_ADCSLoopSimSen],64,0,bits_len_union_t::_16bits_,_16L,Iter_Star1Camera_Z_note_t,sizeof(Iter_Star1Camera_Z_note_t));
	cube_bit_reg[Iter_Star1Inertial_X].init_bit(&cube_reg[CAR_Trigger_ADCSLoopSimSen],66,0,bits_len_union_t::_16bits_,_16L,Iter_Star1Inertial_X_note_t,sizeof(Iter_Star1Inertial_X_note_t));
	cube_bit_reg[Iter_Star1Inertial_Y].init_bit(&cube_reg[CAR_Trigger_ADCSLoopSimSen],68,0,bits_len_union_t::_16bits_,_16L,Iter_Star1Inertial_Y_note_t,sizeof(Iter_Star1Inertial_Y_note_t));
	cube_bit_reg[Iter_Star1Inertial_Z].init_bit(&cube_reg[CAR_Trigger_ADCSLoopSimSen],70,0,bits_len_union_t::_16bits_,_16L,Iter_Star1Inertial_Z_note_t,sizeof(Iter_Star1Inertial_Z_note_t));
	cube_bit_reg[Iter_Star2Camera_X ].init_bit(&cube_reg[CAR_Trigger_ADCSLoopSimSen],72,0,bits_len_union_t::_16bits_,_16L,Iter_Star2Camera_X_note_t,sizeof(Iter_Star2Camera_X_note_t));
	cube_bit_reg[Iter_Star2Camera_Y].init_bit(&cube_reg[CAR_Trigger_ADCSLoopSimSen],74,0,bits_len_union_t::_16bits_,_16L,Iter_Star2Camera_Y_note_t,sizeof(Iter_Star2Camera_Y_note_t));
	cube_bit_reg[Iter_Star2Camera_Z].init_bit(&cube_reg[CAR_Trigger_ADCSLoopSimSen],76,0,bits_len_union_t::_16bits_,_16L,Iter_Star2Camera_Z_note_t,sizeof(Iter_Star2Camera_Z_note_t));
	cube_bit_reg[Iter_Star2Inertial_X].init_bit(&cube_reg[CAR_Trigger_ADCSLoopSimSen],78,0,bits_len_union_t::_16bits_,_16L,Iter_Star2Inertial_X_note_t,sizeof(Iter_Star2Inertial_X_note_t));
	cube_bit_reg[Iter_Star2Inertial_Y].init_bit(&cube_reg[CAR_Trigger_ADCSLoopSimSen],80,0,bits_len_union_t::_16bits_,_16L,Iter_Star2Inertial_Y_note_t,sizeof(Iter_Star2Inertial_Y_note_t));
	cube_bit_reg[Iter_Star2Inertial_Z].init_bit(&cube_reg[CAR_Trigger_ADCSLoopSimSen],82,0,bits_len_union_t::_16bits_,_16L,Iter_Star2Inertial_Z_note_t,sizeof(Iter_Star2Inertial_Z_note_t));
	cube_bit_reg[Iter_Star3Camera_X ].init_bit(&cube_reg[CAR_Trigger_ADCSLoopSimSen],84,0,bits_len_union_t::_16bits_,_16L,Iter_Star3Camera_X_note_t,sizeof(Iter_Star3Camera_X_note_t));
	cube_bit_reg[Iter_Star3Camera_Y].init_bit(&cube_reg[CAR_Trigger_ADCSLoopSimSen],86,0,bits_len_union_t::_16bits_,_16L,Iter_Star3Camera_Y_note_t,sizeof(Iter_Star3Camera_Y_note_t));
	cube_bit_reg[Iter_Star3Camera_Z].init_bit(&cube_reg[CAR_Trigger_ADCSLoopSimSen],88,0,bits_len_union_t::_16bits_,_16L,Iter_Star3Camera_Z_note_t,sizeof(Iter_Star3Camera_Z_note_t));
	cube_bit_reg[Iter_Star3Inertial_X].init_bit(&cube_reg[CAR_Trigger_ADCSLoopSimSen],90,0,bits_len_union_t::_16bits_,_16L,Iter_Star3Inertial_X_note_t,sizeof(Iter_Star3Inertial_X_note_t));
	cube_bit_reg[Iter_Star3Inertial_Y].init_bit(&cube_reg[CAR_Trigger_ADCSLoopSimSen],92,0,bits_len_union_t::_16bits_,_16L,Iter_Star3Inertial_Y_note_t,sizeof(Iter_Star3Inertial_Y_note_t));
	cube_bit_reg[Iter_Star3Inertial_Z].init_bit(&cube_reg[CAR_Trigger_ADCSLoopSimSen],94,0,bits_len_union_t::_16bits_,_16L,Iter_Star3Inertial_Z_note_t,sizeof(Iter_Star3Inertial_Z_note_t));
	cube_bit_reg[Iter_Gps_Solution_Status].init_bit(&cube_reg[CAR_Trigger_ADCSLoopSimSen],96,0,bits_len_union_t::_8bits_,_8UL,Iter_Gps_Solution_Status_note_t,sizeof(Iter_Gps_Solution_Status_note_t));
	cube_bit_reg[Iter_GPS_Ref_Week].init_bit(&cube_reg[CAR_Trigger_ADCSLoopSimSen],97,0,bits_len_union_t::_16bits_,_16UL,Iter_GPS_Ref_Week_note_t,sizeof(Iter_GPS_Ref_Week_note_t));
	cube_bit_reg[Iter_GPS_Time_Millisec].init_bit(&cube_reg[CAR_Trigger_ADCSLoopSimSen],99,0,bits_len_union_t::_32bits_,_32UL,Iter_GPS_Time_Millisec_note_t,sizeof(Iter_GPS_Time_Millisec_note_t));
	cube_bit_reg[Iter_ECEF_Pos_X].init_bit(&cube_reg[CAR_Trigger_ADCSLoopSimSen],103,0,bits_len_union_t::_32bits_,_32L,Iter_ECEF_Pos_X_note_t,sizeof(Iter_ECEF_Pos_X_note_t));
	cube_bit_reg[Iter_ECEF_Velo_X].init_bit(&cube_reg[CAR_Trigger_ADCSLoopSimSen],107,0,bits_len_union_t::_16bits_,_16L,Iter_ECEF_Velo_X_note_t,sizeof(Iter_ECEF_Velo_X_note_t));
	cube_bit_reg[Iter_ECEF_Pos_Y].init_bit(&cube_reg[CAR_Trigger_ADCSLoopSimSen],109,0,bits_len_union_t::_32bits_,_32L,Iter_ECEF_Pos_Y_note_t,sizeof(Iter_ECEF_Pos_Y_note_t));
	cube_bit_reg[Iter_ECEF_Velo_Y].init_bit(&cube_reg[CAR_Trigger_ADCSLoopSimSen],113,0,bits_len_union_t::_16bits_,_16L,Iter_ECEF_Velo_Y_note_t,sizeof(Iter_ECEF_Velo_Y_note_t));
	cube_bit_reg[Iter_ECEF_Pos_Z].init_bit(&cube_reg[CAR_Trigger_ADCSLoopSimSen],115,0,bits_len_union_t::_32bits_,_32L,Iter_ECEF_Pos_Z_note_t,sizeof(Iter_ECEF_Pos_Z_note_t));
	cube_bit_reg[Iter_ECEF_Velo_Z].init_bit(&cube_reg[CAR_Trigger_ADCSLoopSimSen],119,0,bits_len_union_t::_16bits_,_16L,Iter_ECEF_Velo_Z_note_t,sizeof(Iter_ECEF_Velo_Z_note_t));
	cube_bit_reg[Iter_XposStandard_Devia].init_bit(&cube_reg[CAR_Trigger_ADCSLoopSimSen],121,0,bits_len_union_t::_8bits_,_8UL,Iter_XposStandard_Devia_note_t,sizeof(Iter_XposStandard_Devia_note_t));
	cube_bit_reg[Iter_YposStandard_Devia].init_bit(&cube_reg[CAR_Trigger_ADCSLoopSimSen],122,0,bits_len_union_t::_8bits_,_8UL,Iter_YposStandard_Devia_note_t,sizeof(Iter_YposStandard_Devia_note_t));
	cube_bit_reg[Iter_ZposStandard_Devia].init_bit(&cube_reg[CAR_Trigger_ADCSLoopSimSen],123,0,bits_len_union_t::_8bits_,_8UL,Iter_ZposStandard_Devia_note_t,sizeof(Iter_ZposStandard_Devia_note_t));
	cube_bit_reg[Iter_XvelStandard_Devia].init_bit(&cube_reg[CAR_Trigger_ADCSLoopSimSen],124,0,bits_len_union_t::_8bits_,_8UL,Iter_XvelStandard_Devia_note_t,sizeof(Iter_XvelStandard_Devia_note_t));
	cube_bit_reg[Iter_YvelStandard_Devia].init_bit(&cube_reg[CAR_Trigger_ADCSLoopSimSen],125,0,bits_len_union_t::_8bits_,_8UL,Iter_YvelStandard_Devia_note_t,sizeof(Iter_YvelStandard_Devia_note_t));
	cube_bit_reg[Iter_ZvelStandard_Devia].init_bit(&cube_reg[CAR_Trigger_ADCSLoopSimSen],126,0,bits_len_union_t::_8bits_,_8UL,Iter_ZvelStandard_Devia_note_t,sizeof(Iter_ZvelStandard_Devia_note_t));
	cube_bit_reg[SET_MTQ1config].init_bit(&cube_reg[CAR_SetMTQConfig],0,0,bits_len_union_t::_8bits_,_8UL,SET_MTQ1config_note_t,sizeof(SET_MTQ1config_note_t));
	cube_bit_reg[SET_MTQ2config].init_bit(&cube_reg[CAR_SetMTQConfig],1,0,bits_len_union_t::_8bits_,_8UL,SET_MTQ2config_note_t,sizeof(SET_MTQ2config_note_t));
	cube_bit_reg[SET_MTQ3config].init_bit(&cube_reg[CAR_SetMTQConfig],2,0,bits_len_union_t::_8bits_,_8UL,SET_MTQ3config_note_t,sizeof(SET_MTQ3config_note_t));
	cube_bit_reg[SET_RW1_Config].init_bit(&cube_reg[CAR_Set_Wheel_Config],0,0,bits_len_union_t::_8bits_,_8UL,SET_RW1_Config_note_t,sizeof(SET_RW1_Config_note_t));
	cube_bit_reg[SET_RW2_Config].init_bit(&cube_reg[CAR_Set_Wheel_Config],1,0,bits_len_union_t::_8bits_,_8UL,SET_RW2_Config_note_t,sizeof(SET_RW2_Config_note_t));
	cube_bit_reg[SET_RW3_Config].init_bit(&cube_reg[CAR_Set_Wheel_Config],2,0,bits_len_union_t::_8bits_,_8UL,SET_RW3_Config_note_t,sizeof(SET_RW3_Config_note_t));
	cube_bit_reg[SET_RW4_Config].init_bit(&cube_reg[CAR_Set_Wheel_Config],3,0,bits_len_union_t::_8bits_,_8UL,SET_RW4_Config_note_t,sizeof(SET_RW4_Config_note_t));
	cube_bit_reg[SET_Gyro1Config].init_bit(&cube_reg[CAR_Set_RateGyroConfig],0,0,bits_len_union_t::_8bits_,_8UL,SET_Gyro1Config_note_t,sizeof(SET_Gyro1Config_note_t));
	cube_bit_reg[SET_Gyro2Config].init_bit(&cube_reg[CAR_Set_RateGyroConfig],1,0,bits_len_union_t::_8bits_,_8UL,SET_Gyro2Config_note_t,sizeof(SET_Gyro2Config_note_t));
	cube_bit_reg[SET_Gyro3Config].init_bit(&cube_reg[CAR_Set_RateGyroConfig],2,0,bits_len_union_t::_8bits_,_8UL,SET_Gyro3Config_note_t,sizeof(SET_Gyro3Config_note_t));
	cube_bit_reg[SET_XRateSensorOffset].init_bit(&cube_reg[CAR_Set_RateGyroConfig],3,0,bits_len_union_t::_16bits_,_16L,SET_XRateSensorOffset_note_t,sizeof(SET_XRateSensorOffset_note_t));
	cube_bit_reg[SET_YRateSensorOffset].init_bit(&cube_reg[CAR_Set_RateGyroConfig],5,0,bits_len_union_t::_16bits_,_16L,SET_YRateSensorOffset_note_t,sizeof(SET_YRateSensorOffset_note_t));
	cube_bit_reg[SET_ZRateSensorOffset].init_bit(&cube_reg[CAR_Set_RateGyroConfig],7,0,bits_len_union_t::_16bits_,_16L,SET_ZRateSensorOffset_note_t,sizeof(SET_ZRateSensorOffset_note_t));
	cube_bit_reg[SET_RateSensorMult].init_bit(&cube_reg[CAR_Set_RateGyroConfig],9,0,bits_len_union_t::_8bits_,_8UL,SET_RateSensorMult_note_t,sizeof(SET_RateSensorMult_note_t));
	cube_bit_reg[SET_CSS1_Config].init_bit(&cube_reg[CAR_Set_CSS_Config],0,0,bits_len_union_t::_8bits_,_8UL,SET_CSS1_Config_note_t,sizeof(SET_CSS1_Config_note_t));
	cube_bit_reg[SET_CSS2_Config].init_bit(&cube_reg[CAR_Set_CSS_Config],1,0,bits_len_union_t::_8bits_,_8UL,SET_CSS2_Config_note_t,sizeof(SET_CSS2_Config_note_t));
	cube_bit_reg[SET_CSS3_Config].init_bit(&cube_reg[CAR_Set_CSS_Config],2,0,bits_len_union_t::_8bits_,_8UL,SET_CSS3_Config_note_t,sizeof(SET_CSS3_Config_note_t));
	cube_bit_reg[SET_CSS4_Config].init_bit(&cube_reg[CAR_Set_CSS_Config],3,0,bits_len_union_t::_8bits_,_8UL,SET_CSS4_Config_note_t,sizeof(SET_CSS4_Config_note_t));
	cube_bit_reg[SET_CSS5_Config].init_bit(&cube_reg[CAR_Set_CSS_Config],4,0,bits_len_union_t::_8bits_,_8UL,SET_CSS5_Config_note_t,sizeof(SET_CSS5_Config_note_t));
	cube_bit_reg[SET_CSS6_Config].init_bit(&cube_reg[CAR_Set_CSS_Config],5,0,bits_len_union_t::_8bits_,_8UL,SET_CSS6_Config_note_t,sizeof(SET_CSS6_Config_note_t));
	cube_bit_reg[SET_CSS7_Config].init_bit(&cube_reg[CAR_Set_CSS_Config],6,0,bits_len_union_t::_8bits_,_8UL,SET_CSS7_Config_note_t,sizeof(SET_CSS7_Config_note_t));
	cube_bit_reg[SET_CSS8_Config].init_bit(&cube_reg[CAR_Set_CSS_Config],7,0,bits_len_union_t::_8bits_,_8UL,SET_CSS8_Config_note_t,sizeof(SET_CSS8_Config_note_t));
	cube_bit_reg[SET_CSS9_Config].init_bit(&cube_reg[CAR_Set_CSS_Config],8,0,bits_len_union_t::_8bits_,_8UL,SET_CSS9_Config_note_t,sizeof(SET_CSS9_Config_note_t));
	cube_bit_reg[SET_CSS10_Config].init_bit(&cube_reg[CAR_Set_CSS_Config],9,0,bits_len_union_t::_8bits_,_8UL,SET_CSS10_Config_note_t,sizeof(SET_CSS10_Config_note_t));
	cube_bit_reg[SET_CSS1_Rela_Scale].init_bit(&cube_reg[CAR_Set_CSS_Config],10,0,bits_len_union_t::_8bits_,_8UL,SET_CSS1_Rela_Scale_note_t,sizeof(SET_CSS1_Rela_Scale_note_t));
	cube_bit_reg[SET_CSS2_Rela_Scale].init_bit(&cube_reg[CAR_Set_CSS_Config],11,0,bits_len_union_t::_8bits_,_8UL,SET_CSS2_Rela_Scale_note_t,sizeof(SET_CSS2_Rela_Scale_note_t));
	cube_bit_reg[SET_CSS3_Rela_Scale].init_bit(&cube_reg[CAR_Set_CSS_Config],12,0,bits_len_union_t::_8bits_,_8UL,SET_CSS3_Rela_Scale_note_t,sizeof(SET_CSS3_Rela_Scale_note_t));
	cube_bit_reg[SET_CSS4_Rela_Scale].init_bit(&cube_reg[CAR_Set_CSS_Config],13,0,bits_len_union_t::_8bits_,_8UL,SET_CSS4_Rela_Scale_note_t,sizeof(SET_CSS4_Rela_Scale_note_t));
	cube_bit_reg[SET_CSS5_Rela_Scale].init_bit(&cube_reg[CAR_Set_CSS_Config],14,0,bits_len_union_t::_8bits_,_8UL,SET_CSS5_Rela_Scale_note_t,sizeof(SET_CSS5_Rela_Scale_note_t));
	cube_bit_reg[SET_CSS6_Rela_Scale].init_bit(&cube_reg[CAR_Set_CSS_Config],15,0,bits_len_union_t::_8bits_,_8UL,SET_CSS6_Rela_Scale_note_t,sizeof(SET_CSS6_Rela_Scale_note_t));
	cube_bit_reg[SET_CSS7_Rela_Scale].init_bit(&cube_reg[CAR_Set_CSS_Config],16,0,bits_len_union_t::_8bits_,_8UL,SET_CSS7_Rela_Scale_note_t,sizeof(SET_CSS7_Rela_Scale_note_t));
	cube_bit_reg[SET_CSS8_Rela_Scale].init_bit(&cube_reg[CAR_Set_CSS_Config],17,0,bits_len_union_t::_8bits_,_8UL,SET_CSS8_Rela_Scale_note_t,sizeof(SET_CSS8_Rela_Scale_note_t));
	cube_bit_reg[SET_CSS9_Rela_Scale].init_bit(&cube_reg[CAR_Set_CSS_Config],18,0,bits_len_union_t::_8bits_,_8UL,SET_CSS9_Rela_Scale_note_t,sizeof(SET_CSS9_Rela_Scale_note_t));
	cube_bit_reg[SET_CSS10_Rela_Scale].init_bit(&cube_reg[CAR_Set_CSS_Config],19,0,bits_len_union_t::_8bits_,_8UL,SET_CSS10_Rela_Scale_note_t,sizeof(SET_CSS10_Rela_Scale_note_t));
	cube_bit_reg[SET_CSS_Threshold].init_bit(&cube_reg[CAR_Set_CSS_Config],20,0,bits_len_union_t::_8bits_,_8UL,SET_CSS_Threshold_note_t,sizeof(SET_CSS_Threshold_note_t));
	cube_bit_reg[SET_Cam1_Mounting_Trans_Alpha ].init_bit(&cube_reg[CAR_Set_CubeSensConfig],0,0,bits_len_union_t::_16bits_,_16L,SET_Cam1_Mounting_Trans_Alpha_note_t,sizeof(SET_Cam1_Mounting_Trans_Alpha_note_t));
	cube_bit_reg[SET_Cam1_Mounting_Trans_Beta ].init_bit(&cube_reg[CAR_Set_CubeSensConfig],2,0,bits_len_union_t::_16bits_,_16L,SET_Cam1_Mounting_Trans_Beta_note_t,sizeof(SET_Cam1_Mounting_Trans_Beta_note_t));
	cube_bit_reg[SET_Cam1_Mounting_Trans_Gamma].init_bit(&cube_reg[CAR_Set_CubeSensConfig],4,0,bits_len_union_t::_16bits_,_16L,SET_Cam1_Mounting_Trans_Gamma_note_t,sizeof(SET_Cam1_Mounting_Trans_Gamma_note_t));
	cube_bit_reg[SET_Cam1_DetectThreshold].init_bit(&cube_reg[CAR_Set_CubeSensConfig],6,0,bits_len_union_t::_8bits_,_8UL,SET_Cam1_DetectThreshold_note_t,sizeof(SET_Cam1_DetectThreshold_note_t));
	cube_bit_reg[SET_Cam1_AutoAdjust].init_bit(&cube_reg[CAR_Set_CubeSensConfig],7,0,bits_len_union_t::_1bits_,_8UL,SET_Cam1_AutoAdjust_note_t,sizeof(SET_Cam1_AutoAdjust_note_t));
	cube_bit_reg[SET_Cam1_Exposure_Time].init_bit(&cube_reg[CAR_Set_CubeSensConfig],8,0,bits_len_union_t::_16bits_,_16UL,SET_Cam1_Exposure_Time_note_t,sizeof(SET_Cam1_Exposure_Time_note_t));
	cube_bit_reg[SET_Cam1_Boresight_X].init_bit(&cube_reg[CAR_Set_CubeSensConfig],10,0,bits_len_union_t::_16bits_,_16UL,SET_Cam1_Boresight_X_note_t,sizeof(SET_Cam1_Boresight_X_note_t));
	cube_bit_reg[SET_Cam1_Boresight_Y].init_bit(&cube_reg[CAR_Set_CubeSensConfig],12,0,bits_len_union_t::_16bits_,_16UL,SET_Cam1_Boresight_Y_note_t,sizeof(SET_Cam1_Boresight_Y_note_t));
	cube_bit_reg[SET_Cam2_Mounting_Trans_Alpha ].init_bit(&cube_reg[CAR_Set_CubeSensConfig],14,0,bits_len_union_t::_16bits_,_16L,SET_Cam2_Mounting_Trans_Alpha_note_t,sizeof(SET_Cam2_Mounting_Trans_Alpha_note_t));
	cube_bit_reg[SET_Cam2_Mounting_Trans_Beta ].init_bit(&cube_reg[CAR_Set_CubeSensConfig],16,0,bits_len_union_t::_16bits_,_16L,SET_Cam2_Mounting_Trans_Beta_note_t,sizeof(SET_Cam2_Mounting_Trans_Beta_note_t));
	cube_bit_reg[SET_Cam2_Mounting_Trans_Gamma].init_bit(&cube_reg[CAR_Set_CubeSensConfig],18,0,bits_len_union_t::_16bits_,_16L,SET_Cam2_Mounting_Trans_Gamma_note_t,sizeof(SET_Cam2_Mounting_Trans_Gamma_note_t));
	cube_bit_reg[SET_Cam2_DetectThreshold].init_bit(&cube_reg[CAR_Set_CubeSensConfig],20,0,bits_len_union_t::_8bits_,_8UL,SET_Cam2_DetectThreshold_note_t,sizeof(SET_Cam2_DetectThreshold_note_t));
	cube_bit_reg[SET_Cam2_AutoAdjust].init_bit(&cube_reg[CAR_Set_CubeSensConfig],21,0,bits_len_union_t::_1bits_,_8UL,SET_Cam2_AutoAdjust_note_t,sizeof(SET_Cam2_AutoAdjust_note_t));
	cube_bit_reg[SET_Cam2_Exposure_Time].init_bit(&cube_reg[CAR_Set_CubeSensConfig],22,0,bits_len_union_t::_16bits_,_16UL,SET_Cam2_Exposure_Time_note_t,sizeof(SET_Cam2_Exposure_Time_note_t));
	cube_bit_reg[SET_Cam2_Boresight_X].init_bit(&cube_reg[CAR_Set_CubeSensConfig],24,0,bits_len_union_t::_16bits_,_16UL,SET_Cam2_Boresight_X_note_t,sizeof(SET_Cam2_Boresight_X_note_t));
	cube_bit_reg[SET_Cam2_Boresight_Y].init_bit(&cube_reg[CAR_Set_CubeSensConfig],26,0,bits_len_union_t::_16bits_,_16UL,SET_Cam2_Boresight_Y_note_t,sizeof(SET_Cam2_Boresight_Y_note_t));
	cube_bit_reg[SET_Nadir_Max_Devia_Percent].init_bit(&cube_reg[CAR_Set_CubeSensConfig],28,0,bits_len_union_t::_8bits_,_8UL,SET_Nadir_Max_Devia_Percent_note_t,sizeof(SET_Nadir_Max_Devia_Percent_note_t));
	cube_bit_reg[SET_Nadir_Max_BadEdges].init_bit(&cube_reg[CAR_Set_CubeSensConfig],29,0,bits_len_union_t::_8bits_,_8UL,SET_Nadir_Max_BadEdges_note_t,sizeof(SET_Nadir_Max_BadEdges_note_t));
	cube_bit_reg[SET_Nadir_Max_Radius].init_bit(&cube_reg[CAR_Set_CubeSensConfig],30,0,bits_len_union_t::_8bits_,_8UL,SET_Nadir_Max_Radius_note_t,sizeof(SET_Nadir_Max_Radius_note_t));
	cube_bit_reg[SET_Nadir_Min_Radius].init_bit(&cube_reg[CAR_Set_CubeSensConfig],31,0,bits_len_union_t::_8bits_,_8UL,SET_Nadir_Min_Radius_note_t,sizeof(SET_Nadir_Min_Radius_note_t));
	cube_bit_reg[SET_Cam1_MinX_Area1].init_bit(&cube_reg[CAR_Set_CubeSensConfig],32,0,bits_len_union_t::_16bits_,_16UL,SET_Cam1_MinX_Area1_note_t,sizeof(SET_Cam1_MinX_Area1_note_t));
	cube_bit_reg[SET_Cam1_MaxX_Area1].init_bit(&cube_reg[CAR_Set_CubeSensConfig],34,0,bits_len_union_t::_16bits_,_16UL,SET_Cam1_MaxX_Area1_note_t,sizeof(SET_Cam1_MaxX_Area1_note_t));
	cube_bit_reg[SET_Cam1_MinY_Area1].init_bit(&cube_reg[CAR_Set_CubeSensConfig],36,0,bits_len_union_t::_16bits_,_16UL,SET_Cam1_MinY_Area1_note_t,sizeof(SET_Cam1_MinY_Area1_note_t));
	cube_bit_reg[SET_Cam1_MaxY_Area1].init_bit(&cube_reg[CAR_Set_CubeSensConfig],38,0,bits_len_union_t::_16bits_,_16UL,SET_Cam1_MaxY_Area1_note_t,sizeof(SET_Cam1_MaxY_Area1_note_t));
	cube_bit_reg[SET_Cam1_MinX_Area2].init_bit(&cube_reg[CAR_Set_CubeSensConfig],40,0,bits_len_union_t::_16bits_,_16UL,SET_Cam1_MinX_Area2_note_t,sizeof(SET_Cam1_MinX_Area2_note_t));
	cube_bit_reg[SET_Cam1_MaxX_Area2].init_bit(&cube_reg[CAR_Set_CubeSensConfig],42,0,bits_len_union_t::_16bits_,_16UL,SET_Cam1_MaxX_Area2_note_t,sizeof(SET_Cam1_MaxX_Area2_note_t));
	cube_bit_reg[SET_Cam1_MinY_Area2].init_bit(&cube_reg[CAR_Set_CubeSensConfig],44,0,bits_len_union_t::_16bits_,_16UL,SET_Cam1_MinY_Area2_note_t,sizeof(SET_Cam1_MinY_Area2_note_t));
	cube_bit_reg[SET_Cam1_MaxY_Area2].init_bit(&cube_reg[CAR_Set_CubeSensConfig],46,0,bits_len_union_t::_16bits_,_16UL,SET_Cam1_MaxY_Area2_note_t,sizeof(SET_Cam1_MaxY_Area2_note_t));
	cube_bit_reg[SET_Cam1_MinX_Area3].init_bit(&cube_reg[CAR_Set_CubeSensConfig],48,0,bits_len_union_t::_16bits_,_16UL,SET_Cam1_MinX_Area3_note_t,sizeof(SET_Cam1_MinX_Area3_note_t));
	cube_bit_reg[SET_Cam1_MaxX_Area3].init_bit(&cube_reg[CAR_Set_CubeSensConfig],50,0,bits_len_union_t::_16bits_,_16UL,SET_Cam1_MaxX_Area3_note_t,sizeof(SET_Cam1_MaxX_Area3_note_t));
	cube_bit_reg[SET_Cam1_MinY_Area3].init_bit(&cube_reg[CAR_Set_CubeSensConfig],52,0,bits_len_union_t::_16bits_,_16UL,SET_Cam1_MinY_Area3_note_t,sizeof(SET_Cam1_MinY_Area3_note_t));
	cube_bit_reg[SET_Cam1_MaxY_Area3].init_bit(&cube_reg[CAR_Set_CubeSensConfig],54,0,bits_len_union_t::_16bits_,_16UL,SET_Cam1_MaxY_Area3_note_t,sizeof(SET_Cam1_MaxY_Area3_note_t));
	cube_bit_reg[SET_Cam1_MinX_Area4].init_bit(&cube_reg[CAR_Set_CubeSensConfig],56,0,bits_len_union_t::_16bits_,_16UL,SET_Cam1_MinX_Area4_note_t,sizeof(SET_Cam1_MinX_Area4_note_t));
	cube_bit_reg[SET_Cam1_MaxX_Area4].init_bit(&cube_reg[CAR_Set_CubeSensConfig],58,0,bits_len_union_t::_16bits_,_16UL,SET_Cam1_MaxX_Area4_note_t,sizeof(SET_Cam1_MaxX_Area4_note_t));
	cube_bit_reg[SET_Cam1_MinY_Area4].init_bit(&cube_reg[CAR_Set_CubeSensConfig],60,0,bits_len_union_t::_16bits_,_16UL,SET_Cam1_MinY_Area4_note_t,sizeof(SET_Cam1_MinY_Area4_note_t));
	cube_bit_reg[SET_Cam1_MaxY_Area4].init_bit(&cube_reg[CAR_Set_CubeSensConfig],62,0,bits_len_union_t::_16bits_,_16UL,SET_Cam1_MaxY_Area4_note_t,sizeof(SET_Cam1_MaxY_Area4_note_t));
	cube_bit_reg[SET_Cam1_MinX_Area5].init_bit(&cube_reg[CAR_Set_CubeSensConfig],64,0,bits_len_union_t::_16bits_,_16UL,SET_Cam1_MinX_Area5_note_t,sizeof(SET_Cam1_MinX_Area5_note_t));
	cube_bit_reg[SET_Cam1_MaxX_Area5].init_bit(&cube_reg[CAR_Set_CubeSensConfig],66,0,bits_len_union_t::_16bits_,_16UL,SET_Cam1_MaxX_Area5_note_t,sizeof(SET_Cam1_MaxX_Area5_note_t));
	cube_bit_reg[SET_Cam1_MinY_Area5].init_bit(&cube_reg[CAR_Set_CubeSensConfig],68,0,bits_len_union_t::_16bits_,_16UL,SET_Cam1_MinY_Area5_note_t,sizeof(SET_Cam1_MinY_Area5_note_t));
	cube_bit_reg[SET_Cam1_MaxY_Area5].init_bit(&cube_reg[CAR_Set_CubeSensConfig],70,0,bits_len_union_t::_16bits_,_16UL,SET_Cam1_MaxY_Area5_note_t,sizeof(SET_Cam1_MaxY_Area5_note_t));
	cube_bit_reg[SET_Cam2_MinX_Area1].init_bit(&cube_reg[CAR_Set_CubeSensConfig],72,0,bits_len_union_t::_16bits_,_16UL,SET_Cam2_MinX_Area1_note_t,sizeof(SET_Cam2_MinX_Area1_note_t));
	cube_bit_reg[SET_Cam2_MaxX_Area1].init_bit(&cube_reg[CAR_Set_CubeSensConfig],74,0,bits_len_union_t::_16bits_,_16UL,SET_Cam2_MaxX_Area1_note_t,sizeof(SET_Cam2_MaxX_Area1_note_t));
	cube_bit_reg[SET_Cam2_MinY_Area1].init_bit(&cube_reg[CAR_Set_CubeSensConfig],76,0,bits_len_union_t::_16bits_,_16UL,SET_Cam2_MinY_Area1_note_t,sizeof(SET_Cam2_MinY_Area1_note_t));
	cube_bit_reg[SET_Cam2_MaxY_Area1].init_bit(&cube_reg[CAR_Set_CubeSensConfig],78,0,bits_len_union_t::_16bits_,_16UL,SET_Cam2_MaxY_Area1_note_t,sizeof(SET_Cam2_MaxY_Area1_note_t));
	cube_bit_reg[SET_Cam2_MinX_Area2].init_bit(&cube_reg[CAR_Set_CubeSensConfig],80,0,bits_len_union_t::_16bits_,_16UL,SET_Cam2_MinX_Area2_note_t,sizeof(SET_Cam2_MinX_Area2_note_t));
	cube_bit_reg[SET_Cam2_MaxX_Area2].init_bit(&cube_reg[CAR_Set_CubeSensConfig],82,0,bits_len_union_t::_16bits_,_16UL,SET_Cam2_MaxX_Area2_note_t,sizeof(SET_Cam2_MaxX_Area2_note_t));
	cube_bit_reg[SET_Cam2_MinY_Area2].init_bit(&cube_reg[CAR_Set_CubeSensConfig],84,0,bits_len_union_t::_16bits_,_16UL,SET_Cam2_MinY_Area2_note_t,sizeof(SET_Cam2_MinY_Area2_note_t));
	cube_bit_reg[SET_Cam2_MaxY_Area2].init_bit(&cube_reg[CAR_Set_CubeSensConfig],86,0,bits_len_union_t::_16bits_,_16UL,SET_Cam2_MaxY_Area2_note_t,sizeof(SET_Cam2_MaxY_Area2_note_t));
	cube_bit_reg[SET_Cam2_MinX_Area3].init_bit(&cube_reg[CAR_Set_CubeSensConfig],88,0,bits_len_union_t::_16bits_,_16UL,SET_Cam2_MinX_Area3_note_t,sizeof(SET_Cam2_MinX_Area3_note_t));
	cube_bit_reg[SET_Cam2_MaxX_Area3].init_bit(&cube_reg[CAR_Set_CubeSensConfig],90,0,bits_len_union_t::_16bits_,_16UL,SET_Cam2_MaxX_Area3_note_t,sizeof(SET_Cam2_MaxX_Area3_note_t));
	cube_bit_reg[SET_Cam2_MinY_Area3].init_bit(&cube_reg[CAR_Set_CubeSensConfig],92,0,bits_len_union_t::_16bits_,_16UL,SET_Cam2_MinY_Area3_note_t,sizeof(SET_Cam2_MinY_Area3_note_t));
	cube_bit_reg[SET_Cam2_MaxY_Area3].init_bit(&cube_reg[CAR_Set_CubeSensConfig],94,0,bits_len_union_t::_16bits_,_16UL,SET_Cam2_MaxY_Area3_note_t,sizeof(SET_Cam2_MaxY_Area3_note_t));
	cube_bit_reg[SET_Cam2_MinX_Area4].init_bit(&cube_reg[CAR_Set_CubeSensConfig],96,0,bits_len_union_t::_16bits_,_16UL,SET_Cam2_MinX_Area4_note_t,sizeof(SET_Cam2_MinX_Area4_note_t));
	cube_bit_reg[SET_Cam2_MaxX_Area4].init_bit(&cube_reg[CAR_Set_CubeSensConfig],98,0,bits_len_union_t::_16bits_,_16UL,SET_Cam2_MaxX_Area4_note_t,sizeof(SET_Cam2_MaxX_Area4_note_t));
	cube_bit_reg[SET_Cam2_MinY_Area4].init_bit(&cube_reg[CAR_Set_CubeSensConfig],100,0,bits_len_union_t::_16bits_,_16UL,SET_Cam2_MinY_Area4_note_t,sizeof(SET_Cam2_MinY_Area4_note_t));
	cube_bit_reg[SET_Cam2_MaxY_Area4].init_bit(&cube_reg[CAR_Set_CubeSensConfig],102,0,bits_len_union_t::_16bits_,_16UL,SET_Cam2_MaxY_Area4_note_t,sizeof(SET_Cam2_MaxY_Area4_note_t));
	cube_bit_reg[SET_Cam2_MinX_Area5].init_bit(&cube_reg[CAR_Set_CubeSensConfig],104,0,bits_len_union_t::_16bits_,_16UL,SET_Cam2_MinX_Area5_note_t,sizeof(SET_Cam2_MinX_Area5_note_t));
	cube_bit_reg[SET_Cam2_MaxX_Area5].init_bit(&cube_reg[CAR_Set_CubeSensConfig],106,0,bits_len_union_t::_16bits_,_16UL,SET_Cam2_MaxX_Area5_note_t,sizeof(SET_Cam2_MaxX_Area5_note_t));
	cube_bit_reg[SET_Cam2_MinY_Area5].init_bit(&cube_reg[CAR_Set_CubeSensConfig],108,0,bits_len_union_t::_16bits_,_16UL,SET_Cam2_MinY_Area5_note_t,sizeof(SET_Cam2_MinY_Area5_note_t));
	cube_bit_reg[SET_Cam2_MaxY_Area5].init_bit(&cube_reg[CAR_Set_CubeSensConfig],110,0,bits_len_union_t::_16bits_,_16UL,SET_Cam2_MaxY_Area5_note_t,sizeof(SET_Cam2_MaxY_Area5_note_t));
	cube_bit_reg[SET_MTMMountAlpha].init_bit(&cube_reg[CAR_Set_MTMConfig],0,0,bits_len_union_t::_16bits_,_16L,SET_MTMMountAlpha_note_t,sizeof(SET_MTMMountAlpha_note_t));
	cube_bit_reg[SET_MTMMountBeta].init_bit(&cube_reg[CAR_Set_MTMConfig],2,0,bits_len_union_t::_16bits_,_16L,SET_MTMMountBeta_note_t,sizeof(SET_MTMMountBeta_note_t));
	cube_bit_reg[SET_MTMMountGamma].init_bit(&cube_reg[CAR_Set_MTMConfig],4,0,bits_len_union_t::_16bits_,_16L,SET_MTMMountGamma_note_t,sizeof(SET_MTMMountGamma_note_t));
	cube_bit_reg[SET_MTM_Ch1Offset].init_bit(&cube_reg[CAR_Set_MTMConfig],6,0,bits_len_union_t::_16bits_,_16L,SET_MTM_Ch1Offset_note_t,sizeof(SET_MTM_Ch1Offset_note_t));
	cube_bit_reg[SET_MTM_Ch2Offset].init_bit(&cube_reg[CAR_Set_MTMConfig],8,0,bits_len_union_t::_16bits_,_16L,SET_MTM_Ch2Offset_note_t,sizeof(SET_MTM_Ch2Offset_note_t));
	cube_bit_reg[SET_MTM_Ch3Offset].init_bit(&cube_reg[CAR_Set_MTMConfig],10,0,bits_len_union_t::_16bits_,_16L,SET_MTM_Ch3Offset_note_t,sizeof(SET_MTM_Ch3Offset_note_t));
	cube_bit_reg[SET_MTM_S11].init_bit(&cube_reg[CAR_Set_MTMConfig],12,0,bits_len_union_t::_16bits_,_16L,SET_MTM_S11_note_t,sizeof(SET_MTM_S11_note_t));
	cube_bit_reg[SET_MTM_S22].init_bit(&cube_reg[CAR_Set_MTMConfig],14,0,bits_len_union_t::_16bits_,_16L,SET_MTM_S22_note_t,sizeof(SET_MTM_S22_note_t));
	cube_bit_reg[SET_MTM_S33].init_bit(&cube_reg[CAR_Set_MTMConfig],16,0,bits_len_union_t::_16bits_,_16L,SET_MTM_S33_note_t,sizeof(SET_MTM_S33_note_t));
	cube_bit_reg[SET_MTM_S12].init_bit(&cube_reg[CAR_Set_MTMConfig],18,0,bits_len_union_t::_16bits_,_16L,SET_MTM_S12_note_t,sizeof(SET_MTM_S12_note_t));
	cube_bit_reg[SET_MTM_S13].init_bit(&cube_reg[CAR_Set_MTMConfig],20,0,bits_len_union_t::_16bits_,_16L,SET_MTM_S13_note_t,sizeof(SET_MTM_S13_note_t));
	cube_bit_reg[SET_MTM_S21].init_bit(&cube_reg[CAR_Set_MTMConfig],22,0,bits_len_union_t::_16bits_,_16L,SET_MTM_S21_note_t,sizeof(SET_MTM_S21_note_t));
	cube_bit_reg[SET_MTM_S23].init_bit(&cube_reg[CAR_Set_MTMConfig],24,0,bits_len_union_t::_16bits_,_16L,SET_MTM_S23_note_t,sizeof(SET_MTM_S23_note_t));
	cube_bit_reg[SET_MTM_S31].init_bit(&cube_reg[CAR_Set_MTMConfig],26,0,bits_len_union_t::_16bits_,_16L,SET_MTM_S31_note_t,sizeof(SET_MTM_S31_note_t));
	cube_bit_reg[SET_MTM_S32].init_bit(&cube_reg[CAR_Set_MTMConfig],28,0,bits_len_union_t::_16bits_,_16L,SET_MTM_S32_note_t,sizeof(SET_MTM_S32_note_t));
	cube_bit_reg[SET_MTMRateFilterSysNoise].init_bit(&cube_reg[CAR_Set_EstPara],0,0,bits_len_union_t::_32bits_,_32F,SET_MTMRateFilterSysNoise_note_t,sizeof(SET_MTMRateFilterSysNoise_note_t));
	cube_bit_reg[SET_EKFSysNoise].init_bit(&cube_reg[CAR_Set_EstPara],4,0,bits_len_union_t::_32bits_,_32F,SET_EKFSysNoise_note_t,sizeof(SET_EKFSysNoise_note_t));
	cube_bit_reg[SET_CSSMeasureNoise].init_bit(&cube_reg[CAR_Set_EstPara],8,0,bits_len_union_t::_32bits_,_32F,SET_CSSMeasureNoise_note_t,sizeof(SET_CSSMeasureNoise_note_t));
	cube_bit_reg[SET_FSSMeasureNoise].init_bit(&cube_reg[CAR_Set_EstPara],12,0,bits_len_union_t::_32bits_,_32F,SET_FSSMeasureNoise_note_t,sizeof(SET_FSSMeasureNoise_note_t));
	cube_bit_reg[SET_NadirSensMeasureNoise].init_bit(&cube_reg[CAR_Set_EstPara],16,0,bits_len_union_t::_32bits_,_32F,SET_NadirSensMeasureNoise_note_t,sizeof(SET_NadirSensMeasureNoise_note_t));
	cube_bit_reg[SET_MTMMeasureNoise].init_bit(&cube_reg[CAR_Set_EstPara],20,0,bits_len_union_t::_32bits_,_32F,SET_MTMMeasureNoise_note_t,sizeof(SET_MTMMeasureNoise_note_t));
	cube_bit_reg[SET_STMeasureNoise].init_bit(&cube_reg[CAR_Set_EstPara],24,0,bits_len_union_t::_32bits_,_32F,SET_STMeasureNoise_note_t,sizeof(SET_STMeasureNoise_note_t));
	cube_bit_reg[SET_UseFSS].init_bit(&cube_reg[CAR_Set_EstPara],28,0,bits_len_union_t::_1bits_,_8UL,SET_UseFSS_note_t,sizeof(SET_UseFSS_note_t));
	cube_bit_reg[SET_UseNadirSens].init_bit(&cube_reg[CAR_Set_EstPara],28,1,bits_len_union_t::_1bits_,_8UL,SET_UseNadirSens_note_t,sizeof(SET_UseNadirSens_note_t));
	cube_bit_reg[SET_UseCSS].init_bit(&cube_reg[CAR_Set_EstPara],28,2,bits_len_union_t::_1bits_,_8UL,SET_UseCSS_note_t,sizeof(SET_UseCSS_note_t));
	cube_bit_reg[SET_UseST].init_bit(&cube_reg[CAR_Set_EstPara],28,3,bits_len_union_t::_1bits_,_8UL,SET_UseST_note_t,sizeof(SET_UseST_note_t));
	cube_bit_reg[SET_NadirSensterminatortest].init_bit(&cube_reg[CAR_Set_EstPara],28,4,bits_len_union_t::_1bits_,_8UL,SET_NadirSensterminatortest_note_t,sizeof(SET_NadirSensterminatortest_note_t));
	cube_bit_reg[SET_AutoMTMRecovery].init_bit(&cube_reg[CAR_Set_EstPara],28,5,bits_len_union_t::_1bits_,_8UL,SET_AutoMTMRecovery_note_t,sizeof(SET_AutoMTMRecovery_note_t));
	cube_bit_reg[SET_MTMMode].init_bit(&cube_reg[CAR_Set_EstPara],28,6,bits_len_union_t::_2bits_,_8UL,SET_MTMMode_note_t,sizeof(SET_MTMMode_note_t));
	cube_bit_reg[SET_MTMSelectForRAW_MTM_TLM].init_bit(&cube_reg[CAR_Set_EstPara],29,0,bits_len_union_t::_2bits_,_8UL,SET_MTMSelectForRAW_MTM_TLM_note_t,sizeof(SET_MTMSelectForRAW_MTM_TLM_note_t));
	cube_bit_reg[SET_AutoEstTransitDueToRateSensErr].init_bit(&cube_reg[CAR_Set_EstPara],29,2,bits_len_union_t::_1bits_,_8UL,SET_AutoEstTransitDueToRateSensErr_note_t,sizeof(SET_AutoEstTransitDueToRateSensErr_note_t));
	cube_bit_reg[SET_Cam1Cam2SampPeriod].init_bit(&cube_reg[CAR_Set_EstPara],30,0,bits_len_union_t::_8bits_,_8UL,SET_Cam1Cam2SampPeriod_note_t,sizeof(SET_Cam1Cam2SampPeriod_note_t));
	cube_bit_reg[SET_Incl_Coeff].init_bit(&cube_reg[CAR_Set_Aug_SGP4_Para],0,0,bits_len_union_t::_16bits_,_16UL,SET_Incl_Coeff_note_t,sizeof(SET_Incl_Coeff_note_t));
	cube_bit_reg[SET_Raan_Coeff].init_bit(&cube_reg[CAR_Set_Aug_SGP4_Para],2,0,bits_len_union_t::_16bits_,_16UL,SET_Raan_Coeff_note_t,sizeof(SET_Raan_Coeff_note_t));
	cube_bit_reg[SET_Ecc_Coeff].init_bit(&cube_reg[CAR_Set_Aug_SGP4_Para],4,0,bits_len_union_t::_16bits_,_16UL,SET_Ecc_Coeff_note_t,sizeof(SET_Ecc_Coeff_note_t));
	cube_bit_reg[SET_Aop_Coeff].init_bit(&cube_reg[CAR_Set_Aug_SGP4_Para],6,0,bits_len_union_t::_16bits_,_16UL,SET_Aop_Coeff_note_t,sizeof(SET_Aop_Coeff_note_t));
	cube_bit_reg[SET_Time_Coeff].init_bit(&cube_reg[CAR_Set_Aug_SGP4_Para],8,0,bits_len_union_t::_16bits_,_16UL,SET_Time_Coeff_note_t,sizeof(SET_Time_Coeff_note_t));
	cube_bit_reg[SET_Pos_Coeff].init_bit(&cube_reg[CAR_Set_Aug_SGP4_Para],10,0,bits_len_union_t::_16bits_,_16UL,SET_Pos_Coeff_note_t,sizeof(SET_Pos_Coeff_note_t));
	cube_bit_reg[SET_Max_Pos_Err].init_bit(&cube_reg[CAR_Set_Aug_SGP4_Para],12,0,bits_len_union_t::_8bits_,_8UL,SET_Max_Pos_Err_note_t,sizeof(SET_Max_Pos_Err_note_t));
	cube_bit_reg[SET_ASGP4_Filter].init_bit(&cube_reg[CAR_Set_Aug_SGP4_Para],13,0,bits_len_union_t::_8bits_,_32L,SET_ASGP4_Filter_note_t,sizeof(SET_ASGP4_Filter_note_t));
	cube_bit_reg[SET_xp_Coeff].init_bit(&cube_reg[CAR_Set_Aug_SGP4_Para],14,0,bits_len_union_t::_32bits_,_32L,SET_xp_Coeff_note_t,sizeof(SET_xp_Coeff_note_t));
	cube_bit_reg[SET_yp_Coeff].init_bit(&cube_reg[CAR_Set_Aug_SGP4_Para],18,0,bits_len_union_t::_32bits_,_8UL,SET_yp_Coeff_note_t,sizeof(SET_yp_Coeff_note_t));
	cube_bit_reg[SET_GPS_RollOver].init_bit(&cube_reg[CAR_Set_Aug_SGP4_Para],22,0,bits_len_union_t::_8bits_,_8UL,SET_GPS_RollOver_note_t,sizeof(SET_GPS_RollOver_note_t));
	cube_bit_reg[SET_Pos_sd].init_bit(&cube_reg[CAR_Set_Aug_SGP4_Para],23,0,bits_len_union_t::_8bits_,_8UL,SET_Pos_sd_note_t,sizeof(SET_Pos_sd_note_t));
	cube_bit_reg[SET_Velo_sd ].init_bit(&cube_reg[CAR_Set_Aug_SGP4_Para],24,0,bits_len_union_t::_8bits_,_8UL,SET_Velo_sd_note_t,sizeof(SET_Velo_sd_note_t));
	cube_bit_reg[SET_Min_satellites].init_bit(&cube_reg[CAR_Set_Aug_SGP4_Para],25,0,bits_len_union_t::_8bits_,_8UL,SET_Min_satellites_note_t,sizeof(SET_Min_satellites_note_t));
	cube_bit_reg[SET_Time_Gain].init_bit(&cube_reg[CAR_Set_Aug_SGP4_Para],26,0,bits_len_union_t::_8bits_,_8UL,SET_Time_Gain_note_t,sizeof(SET_Time_Gain_note_t));
	cube_bit_reg[SET_Max_Lag].init_bit(&cube_reg[CAR_Set_Aug_SGP4_Para],27,0,bits_len_union_t::_8bits_,_8UL,SET_Max_Lag_note_t,sizeof(SET_Max_Lag_note_t));
	cube_bit_reg[SET_ACP_Type].init_bit(&cube_reg[CAR_Set_ADCS_Config],0,0,bits_len_union_t::_4bits_,_8UL,SET_ACP_Type_note_t,sizeof(SET_ACP_Type_note_t));
	cube_bit_reg[SET_Special_Ctrl_Select].init_bit(&cube_reg[CAR_Set_ADCS_Config],0,4,bits_len_union_t::_4bits_,_8UL,SET_Special_Ctrl_Select_note_t,sizeof(SET_Special_Ctrl_Select_note_t));
	cube_bit_reg[SET_CubeCtrlSig_Ver].init_bit(&cube_reg[CAR_Set_ADCS_Config],1,0,bits_len_union_t::_8bits_,_8UL,SET_CubeCtrlSig_Ver_note_t,sizeof(SET_CubeCtrlSig_Ver_note_t));
	cube_bit_reg[SET_CubeCtrlMotor_Ver].init_bit(&cube_reg[CAR_Set_ADCS_Config],2,0,bits_len_union_t::_8bits_,_8UL,SET_CubeCtrlMotor_Ver_note_t,sizeof(SET_CubeCtrlMotor_Ver_note_t));
	cube_bit_reg[SET_CS1_Ver].init_bit(&cube_reg[CAR_Set_ADCS_Config],3,0,bits_len_union_t::_8bits_,_8UL,SET_CS1_Ver_note_t,sizeof(SET_CS1_Ver_note_t));
	cube_bit_reg[SET_CS2_Ver].init_bit(&cube_reg[CAR_Set_ADCS_Config],4,0,bits_len_union_t::_8bits_,_8UL,SET_CS2_Ver_note_t,sizeof(SET_CS2_Ver_note_t));
	cube_bit_reg[SET_CS1_CamType].init_bit(&cube_reg[CAR_Set_ADCS_Config],5,0,bits_len_union_t::_4bits_,_8UL,SET_CS1_CamType_note_t,sizeof(SET_CS1_CamType_note_t));
	cube_bit_reg[SET_CS2_CamType].init_bit(&cube_reg[CAR_Set_ADCS_Config],5,4,bits_len_union_t::_4bits_,_8UL,SET_CS2_CamType_note_t,sizeof(SET_CS2_CamType_note_t));
	cube_bit_reg[SET_CubeStar_Ver].init_bit(&cube_reg[CAR_Set_ADCS_Config],6,0,bits_len_union_t::_8bits_,_8UL,SET_CubeStar_Ver_note_t,sizeof(SET_CubeStar_Ver_note_t));
	cube_bit_reg[SET_GPS_Type].init_bit(&cube_reg[CAR_Set_ADCS_Config],7,0,bits_len_union_t::_4bits_,_8UL,SET_GPS_Type_note_t,sizeof(SET_GPS_Type_note_t));
	cube_bit_reg[SET_Redun_MTM_Includ].init_bit(&cube_reg[CAR_Set_ADCS_Config],7,4,bits_len_union_t::_1bits_,_8UL,SET_Redun_MTM_Includ_note_t,sizeof(SET_Redun_MTM_Includ_note_t));
	cube_bit_reg[SET_MTQX_MaxDipole].init_bit(&cube_reg[CAR_Set_ADCS_Config],8,0,bits_len_union_t::_32bits_,_32F,SET_MTQX_MaxDipole_note_t,sizeof(SET_MTQX_MaxDipole_note_t));
	cube_bit_reg[SET_MTQY_MaxDipole].init_bit(&cube_reg[CAR_Set_ADCS_Config],12,0,bits_len_union_t::_32bits_,_32F,SET_MTQY_MaxDipole_note_t,sizeof(SET_MTQY_MaxDipole_note_t));
	cube_bit_reg[SET_MTQZ_MaxDipole].init_bit(&cube_reg[CAR_Set_ADCS_Config],16,0,bits_len_union_t::_32bits_,_32F,SET_MTQZ_MaxDipole_note_t,sizeof(SET_MTQZ_MaxDipole_note_t));
	cube_bit_reg[SET_MTQ_Ontime_Res].init_bit(&cube_reg[CAR_Set_ADCS_Config],20,0,bits_len_union_t::_32bits_,_32F,SET_MTQ_Ontime_Res_note_t,sizeof(SET_MTQ_Ontime_Res_note_t));
	cube_bit_reg[SET_MTQ_Max_OnTime].init_bit(&cube_reg[CAR_Set_ADCS_Config],24,0,bits_len_union_t::_32bits_,_32F,SET_MTQ_Max_OnTime_note_t,sizeof(SET_MTQ_Max_OnTime_note_t));
	cube_bit_reg[SET_RWX_MaxTorque].init_bit(&cube_reg[CAR_Set_ADCS_Config],28,0,bits_len_union_t::_32bits_,_32F,SET_RWX_MaxTorque_note_t,sizeof(SET_RWX_MaxTorque_note_t));
	cube_bit_reg[SET_RWY_MaxTorque].init_bit(&cube_reg[CAR_Set_ADCS_Config],32,0,bits_len_union_t::_32bits_,_32F,SET_RWY_MaxTorque_note_t,sizeof(SET_RWY_MaxTorque_note_t));
	cube_bit_reg[SET_RWZ_MaxTorque].init_bit(&cube_reg[CAR_Set_ADCS_Config],36,0,bits_len_union_t::_32bits_,_32F,SET_RWZ_MaxTorque_note_t,sizeof(SET_RWZ_MaxTorque_note_t));
	cube_bit_reg[SET_RWX_MaxMomentum].init_bit(&cube_reg[CAR_Set_ADCS_Config],40,0,bits_len_union_t::_32bits_,_32F,SET_RWX_MaxMomentum_note_t,sizeof(SET_RWX_MaxMomentum_note_t));
	cube_bit_reg[SET_RWY_MaxMomentum].init_bit(&cube_reg[CAR_Set_ADCS_Config],44,0,bits_len_union_t::_32bits_,_32F,SET_RWY_MaxMomentum_note_t,sizeof(SET_RWY_MaxMomentum_note_t));
	cube_bit_reg[SET_RWZ_MaxMomentum].init_bit(&cube_reg[CAR_Set_ADCS_Config],48,0,bits_len_union_t::_32bits_,_32F,SET_RWZ_MaxMomentum_note_t,sizeof(SET_RWZ_MaxMomentum_note_t));
	cube_bit_reg[SET_RWX_Inertia].init_bit(&cube_reg[CAR_Set_ADCS_Config],52,0,bits_len_union_t::_32bits_,_32F,SET_RWX_Inertia_note_t,sizeof(SET_RWX_Inertia_note_t));
	cube_bit_reg[SET_RWY_Inertia].init_bit(&cube_reg[CAR_Set_ADCS_Config],56,0,bits_len_union_t::_32bits_,_32F,SET_RWY_Inertia_note_t,sizeof(SET_RWY_Inertia_note_t));
	cube_bit_reg[SET_RWZ_Inertia].init_bit(&cube_reg[CAR_Set_ADCS_Config],60,0,bits_len_union_t::_32bits_,_32F,SET_RWZ_Inertia_note_t,sizeof(SET_RWZ_Inertia_note_t));
	cube_bit_reg[SET_RW_Torque_Increment].init_bit(&cube_reg[CAR_Set_ADCS_Config],64,0,bits_len_union_t::_32bits_,_32F,SET_RW_Torque_Increment_note_t,sizeof(SET_RW_Torque_Increment_note_t));
	cube_bit_reg[SET_Prim_MTMXBias_d1].init_bit(&cube_reg[CAR_Set_ADCS_Config],68,0,bits_len_union_t::_32bits_,_32F,SET_Prim_MTMXBias_d1_note_t,sizeof(SET_Prim_MTMXBias_d1_note_t));
	cube_bit_reg[SET_Prim_MTMYBias_d1].init_bit(&cube_reg[CAR_Set_ADCS_Config],72,0,bits_len_union_t::_32bits_,_32F,SET_Prim_MTMYBias_d1_note_t,sizeof(SET_Prim_MTMYBias_d1_note_t));
	cube_bit_reg[SET_Prim_MTMZBias_d1].init_bit(&cube_reg[CAR_Set_ADCS_Config],76,0,bits_len_union_t::_32bits_,_32F,SET_Prim_MTMZBias_d1_note_t,sizeof(SET_Prim_MTMZBias_d1_note_t));
	cube_bit_reg[SET_Prim_MTMXBias_d2].init_bit(&cube_reg[CAR_Set_ADCS_Config],80,0,bits_len_union_t::_32bits_,_32F,SET_Prim_MTMXBias_d2_note_t,sizeof(SET_Prim_MTMXBias_d2_note_t));
	cube_bit_reg[SET_Prim_MTMYBias_d2].init_bit(&cube_reg[CAR_Set_ADCS_Config],84,0,bits_len_union_t::_32bits_,_32F,SET_Prim_MTMYBias_d2_note_t,sizeof(SET_Prim_MTMYBias_d2_note_t));
	cube_bit_reg[SET_Prim_MTMZBias_d2].init_bit(&cube_reg[CAR_Set_ADCS_Config],88,0,bits_len_union_t::_32bits_,_32F,SET_Prim_MTMZBias_d2_note_t,sizeof(SET_Prim_MTMZBias_d2_note_t));
	cube_bit_reg[SET_Prim_MTMXSens_s1].init_bit(&cube_reg[CAR_Set_ADCS_Config],92,0,bits_len_union_t::_32bits_,_32F,SET_Prim_MTMXSens_s1_note_t,sizeof(SET_Prim_MTMXSens_s1_note_t));
	cube_bit_reg[SET_Prim_MTMYSens_s1].init_bit(&cube_reg[CAR_Set_ADCS_Config],96,0,bits_len_union_t::_32bits_,_32F,SET_Prim_MTMYSens_s1_note_t,sizeof(SET_Prim_MTMYSens_s1_note_t));
	cube_bit_reg[SET_Prim_MTMZSens_s1].init_bit(&cube_reg[CAR_Set_ADCS_Config],100,0,bits_len_union_t::_32bits_,_32F,SET_Prim_MTMZSens_s1_note_t,sizeof(SET_Prim_MTMZSens_s1_note_t));
	cube_bit_reg[SET_Prim_MTMXSens_s2].init_bit(&cube_reg[CAR_Set_ADCS_Config],104,0,bits_len_union_t::_32bits_,_32F,SET_Prim_MTMXSens_s2_note_t,sizeof(SET_Prim_MTMXSens_s2_note_t));
	cube_bit_reg[SET_Prim_MTMYSens_s2].init_bit(&cube_reg[CAR_Set_ADCS_Config],108,0,bits_len_union_t::_32bits_,_32F,SET_Prim_MTMYSens_s2_note_t,sizeof(SET_Prim_MTMYSens_s2_note_t));
	cube_bit_reg[SET_Prim_MTMZSens_s2].init_bit(&cube_reg[CAR_Set_ADCS_Config],112,0,bits_len_union_t::_32bits_,_32F,SET_Prim_MTMZSens_s2_note_t,sizeof(SET_Prim_MTMZSens_s2_note_t));
	cube_bit_reg[SET_Redun_MTMXBias_d1].init_bit(&cube_reg[CAR_Set_ADCS_Config],116,0,bits_len_union_t::_32bits_,_32F,SET_Redun_MTMXBias_d1_note_t,sizeof(SET_Redun_MTMXBias_d1_note_t));
	cube_bit_reg[SET_Redun_MTMYBias_d1].init_bit(&cube_reg[CAR_Set_ADCS_Config],120,0,bits_len_union_t::_32bits_,_32F,SET_Redun_MTMYBias_d1_note_t,sizeof(SET_Redun_MTMYBias_d1_note_t));
	cube_bit_reg[SET_Redun_MTMZBias_d1].init_bit(&cube_reg[CAR_Set_ADCS_Config],124,0,bits_len_union_t::_32bits_,_32F,SET_Redun_MTMZBias_d1_note_t,sizeof(SET_Redun_MTMZBias_d1_note_t));
	cube_bit_reg[SET_Redun_MTMXBias_d2].init_bit(&cube_reg[CAR_Set_ADCS_Config],128,0,bits_len_union_t::_32bits_,_32F,SET_Redun_MTMXBias_d2_note_t,sizeof(SET_Redun_MTMXBias_d2_note_t));
	cube_bit_reg[SET_Redun_MTMYBias_d2].init_bit(&cube_reg[CAR_Set_ADCS_Config],132,0,bits_len_union_t::_32bits_,_32F,SET_Redun_MTMYBias_d2_note_t,sizeof(SET_Redun_MTMYBias_d2_note_t));
	cube_bit_reg[SET_Redun_MTMZBias_d2].init_bit(&cube_reg[CAR_Set_ADCS_Config],136,0,bits_len_union_t::_32bits_,_32F,SET_Redun_MTMZBias_d2_note_t,sizeof(SET_Redun_MTMZBias_d2_note_t));
	cube_bit_reg[SET_Redun_MTMXSens_s1].init_bit(&cube_reg[CAR_Set_ADCS_Config],140,0,bits_len_union_t::_32bits_,_32F,SET_Redun_MTMXSens_s1_note_t,sizeof(SET_Redun_MTMXSens_s1_note_t));
	cube_bit_reg[SET_Redun_MTMYSens_s1].init_bit(&cube_reg[CAR_Set_ADCS_Config],144,0,bits_len_union_t::_32bits_,_32F,SET_Redun_MTMYSens_s1_note_t,sizeof(SET_Redun_MTMYSens_s1_note_t));
	cube_bit_reg[SET_Redun_MTMZSens_s1].init_bit(&cube_reg[CAR_Set_ADCS_Config],148,0,bits_len_union_t::_32bits_,_32F,SET_Redun_MTMZSens_s1_note_t,sizeof(SET_Redun_MTMZSens_s1_note_t));
	cube_bit_reg[SET_Redun_MTMXSens_s2].init_bit(&cube_reg[CAR_Set_ADCS_Config],152,0,bits_len_union_t::_32bits_,_32F,SET_Redun_MTMXSens_s2_note_t,sizeof(SET_Redun_MTMXSens_s2_note_t));
	cube_bit_reg[SET_Redun_MTMYSens_s2].init_bit(&cube_reg[CAR_Set_ADCS_Config],156,0,bits_len_union_t::_32bits_,_32F,SET_Redun_MTMYSens_s2_note_t,sizeof(SET_Redun_MTMYSens_s2_note_t));
	cube_bit_reg[SET_Redun_MTMZSens_s2].init_bit(&cube_reg[CAR_Set_ADCS_Config],160,0,bits_len_union_t::_32bits_,_32F,SET_Redun_MTMZSens_s2_note_t,sizeof(SET_Redun_MTMZSens_s2_note_t));
	cube_bit_reg[SET_CCSignal_En_Port].init_bit(&cube_reg[CAR_Set_ADCS_Config],164,0,bits_len_union_t::_4bits_,_8UL,SET_CCSignal_En_Port_note_t,sizeof(SET_CCSignal_En_Port_note_t));
	cube_bit_reg[SET_CCSignal_En_Pin].init_bit(&cube_reg[CAR_Set_ADCS_Config],164,4,bits_len_union_t::_4bits_,_8UL,SET_CCSignal_En_Pin_note_t,sizeof(SET_CCSignal_En_Pin_note_t));
	cube_bit_reg[SET_CCMotor_En_Port].init_bit(&cube_reg[CAR_Set_ADCS_Config],165,0,bits_len_union_t::_4bits_,_8UL,SET_CCMotor_En_Port_note_t,sizeof(SET_CCMotor_En_Port_note_t));
	cube_bit_reg[SET_CCMotor_En_Pin].init_bit(&cube_reg[CAR_Set_ADCS_Config],165,4,bits_len_union_t::_4bits_,_8UL,SET_CCMotor_En_Pin_note_t,sizeof(SET_CCMotor_En_Pin_note_t));
	cube_bit_reg[SET_CCCommon_En_Port].init_bit(&cube_reg[CAR_Set_ADCS_Config],166,0,bits_len_union_t::_4bits_,_8UL,SET_CCCommon_En_Port_note_t,sizeof(SET_CCCommon_En_Port_note_t));
	cube_bit_reg[SET_CCCommon_En_Pin].init_bit(&cube_reg[CAR_Set_ADCS_Config],166,4,bits_len_union_t::_4bits_,_8UL,SET_CCCommon_En_Pin_note_t,sizeof(SET_CCCommon_En_Pin_note_t));
	cube_bit_reg[SET_CS1_En_Port].init_bit(&cube_reg[CAR_Set_ADCS_Config],167,0,bits_len_union_t::_4bits_,_8UL,SET_CS1_En_Port_note_t,sizeof(SET_CS1_En_Port_note_t));
	cube_bit_reg[SET_CS1_En_Pin].init_bit(&cube_reg[CAR_Set_ADCS_Config],167,4,bits_len_union_t::_4bits_,_8UL,SET_CS1_En_Pin_note_t,sizeof(SET_CS1_En_Pin_note_t));
	cube_bit_reg[SET_CS2_En_Port].init_bit(&cube_reg[CAR_Set_ADCS_Config],168,0,bits_len_union_t::_4bits_,_8UL,SET_CS2_En_Port_note_t,sizeof(SET_CS2_En_Port_note_t));
	cube_bit_reg[SET_CS2_En_Pin].init_bit(&cube_reg[CAR_Set_ADCS_Config],168,4,bits_len_union_t::_4bits_,_8UL,SET_CS2_En_Pin_note_t,sizeof(SET_CS2_En_Pin_note_t));
	cube_bit_reg[SET_CubeStar_En_Port].init_bit(&cube_reg[CAR_Set_ADCS_Config],169,0,bits_len_union_t::_4bits_,_8UL,SET_CubeStar_En_Port_note_t,sizeof(SET_CubeStar_En_Port_note_t));
	cube_bit_reg[SET_CubeStar_En_Pin].init_bit(&cube_reg[CAR_Set_ADCS_Config],169,4,bits_len_union_t::_4bits_,_8UL,SET_CubeStar_En_Pin_note_t,sizeof(SET_CubeStar_En_Pin_note_t));
	cube_bit_reg[SET_CW1_En_Port].init_bit(&cube_reg[CAR_Set_ADCS_Config],170,0,bits_len_union_t::_4bits_,_8UL,SET_CW1_En_Port_note_t,sizeof(SET_CW1_En_Port_note_t));
	cube_bit_reg[SET_CW1_En_Pin].init_bit(&cube_reg[CAR_Set_ADCS_Config],170,4,bits_len_union_t::_4bits_,_8UL,SET_CW1_En_Pin_note_t,sizeof(SET_CW1_En_Pin_note_t));
	cube_bit_reg[SET_CW2_En_Port].init_bit(&cube_reg[CAR_Set_ADCS_Config],171,0,bits_len_union_t::_4bits_,_8UL,SET_CW2_En_Port_note_t,sizeof(SET_CW2_En_Port_note_t));
	cube_bit_reg[SET_CW2_En_Pin].init_bit(&cube_reg[CAR_Set_ADCS_Config],171,4,bits_len_union_t::_4bits_,_8UL,SET_CW2_En_Pin_note_t,sizeof(SET_CW2_En_Pin_note_t));
	cube_bit_reg[SET_CW3_En_Port].init_bit(&cube_reg[CAR_Set_ADCS_Config],172,0,bits_len_union_t::_4bits_,_8UL,SET_CW3_En_Port_note_t,sizeof(SET_CW3_En_Port_note_t));
	cube_bit_reg[SET_CW3_En_Pin].init_bit(&cube_reg[CAR_Set_ADCS_Config],172,4,bits_len_union_t::_4bits_,_8UL,SET_CW3_En_Pin_note_t,sizeof(SET_CW3_En_Pin_note_t));
	cube_bit_reg[SET_ASGP4_RunMode].init_bit(&cube_reg[CAR_ASGP4_RunMode],0,0,bits_len_union_t::_8bits_,_8UL,SET_ASGP4_RunMode_note_t,sizeof(SET_ASGP4_RunMode_note_t));
	cube_bit_reg[Trig_ASGP4].init_bit(&cube_reg[CAR_ASGP4_Trigger],0,0,bits_len_union_t::_0bits_,_0UL,Trig_ASGP4_note_t,sizeof(Trig_ASGP4_note_t));
	cube_bit_reg[Format_SDcard_MagicNum].init_bit(&cube_reg[CAR_FormatSDcard],0,0,bits_len_union_t::_8bits_,_8UL,Format_SDcard_MagicNum_note_t,sizeof(Format_SDcard_MagicNum_note_t));
	cube_bit_reg[SET_Inertial_RefX].init_bit(&cube_reg[CAR_Set_I_PointingRef_Vector],0,0,bits_len_union_t::_16bits_,_16L,SET_Inertial_RefX_note_t,sizeof(SET_Inertial_RefX_note_t));
	cube_bit_reg[SET_Inertial_RefY].init_bit(&cube_reg[CAR_Set_I_PointingRef_Vector],2,0,bits_len_union_t::_16bits_,_16L,SET_Inertial_RefY_note_t,sizeof(SET_Inertial_RefY_note_t));
	cube_bit_reg[SET_Inertial_RefZ].init_bit(&cube_reg[CAR_Set_I_PointingRef_Vector],4,0,bits_len_union_t::_16bits_,_16L,SET_Inertial_RefZ_note_t,sizeof(SET_Inertial_RefZ_note_t));
	cube_bit_reg[SET_Detumb_SpinGain].init_bit(&cube_reg[CAR_Set_DetumbCtrl_Para],0,0,bits_len_union_t::_32bits_,_32F,SET_Detumb_SpinGain_note_t,sizeof(SET_Detumb_SpinGain_note_t));
	cube_bit_reg[SET_Detumb_DampGain].init_bit(&cube_reg[CAR_Set_DetumbCtrl_Para],4,0,bits_len_union_t::_32bits_,_32F,SET_Detumb_DampGain_note_t,sizeof(SET_Detumb_DampGain_note_t));
	cube_bit_reg[SET_Ref_SpinRate].init_bit(&cube_reg[CAR_Set_DetumbCtrl_Para],8,0,bits_len_union_t::_16bits_,_16L,SET_Ref_SpinRate_note_t,sizeof(SET_Ref_SpinRate_note_t));
	cube_bit_reg[SET_FastBdot_DetumbGain].init_bit(&cube_reg[CAR_Set_DetumbCtrl_Para],10,0,bits_len_union_t::_32bits_,_32F,SET_FastBdot_DetumbGain_note_t,sizeof(SET_FastBdot_DetumbGain_note_t));
	cube_bit_reg[SET_Ymoment_CtrlGain].init_bit(&cube_reg[CAR_Set_YWheel_Ctrl_Para],0,0,bits_len_union_t::_32bits_,_32F,SET_Ymoment_CtrlGain_note_t,sizeof(SET_Ymoment_CtrlGain_note_t));
	cube_bit_reg[SET_Ymoment_NDampGain].init_bit(&cube_reg[CAR_Set_YWheel_Ctrl_Para],4,0,bits_len_union_t::_32bits_,_32F,SET_Ymoment_NDampGain_note_t,sizeof(SET_Ymoment_NDampGain_note_t));
	cube_bit_reg[SET_Ymoment_PGain].init_bit(&cube_reg[CAR_Set_YWheel_Ctrl_Para],8,0,bits_len_union_t::_32bits_,_32F,SET_Ymoment_PGain_note_t,sizeof(SET_Ymoment_PGain_note_t));
	cube_bit_reg[SET_Ymoment_DGain].init_bit(&cube_reg[CAR_Set_YWheel_Ctrl_Para],12,0,bits_len_union_t::_32bits_,_32F,SET_Ymoment_DGain_note_t,sizeof(SET_Ymoment_DGain_note_t));
	cube_bit_reg[SET_RefWheel_Moment].init_bit(&cube_reg[CAR_Set_YWheel_Ctrl_Para],16,0,bits_len_union_t::_32bits_,_32F,SET_RefWheel_Moment_note_t,sizeof(SET_RefWheel_Moment_note_t));
	cube_bit_reg[SET_Rwheel_PGain].init_bit(&cube_reg[CAR_Set_RW_Ctrl_Para],0,0,bits_len_union_t::_32bits_,_32F,SET_Rwheel_PGain_note_t,sizeof(SET_Rwheel_PGain_note_t));
	cube_bit_reg[SET_Rwheel_DGain].init_bit(&cube_reg[CAR_Set_RW_Ctrl_Para],4,0,bits_len_union_t::_32bits_,_32F,SET_Rwheel_DGain_note_t,sizeof(SET_Rwheel_DGain_note_t));
	cube_bit_reg[SET_YWheelBiasMoment].init_bit(&cube_reg[CAR_Set_RW_Ctrl_Para],8,0,bits_len_union_t::_32bits_,_32F,SET_YWheelBiasMoment_note_t,sizeof(SET_YWheelBiasMoment_note_t));
	cube_bit_reg[SET_SunPointFacet].init_bit(&cube_reg[CAR_Set_RW_Ctrl_Para],12,0,bits_len_union_t::_7bits_,_8UL,SET_SunPointFacet_note_t,sizeof(SET_SunPointFacet_note_t));
	cube_bit_reg[SET_AutoCtrl_TransitDueWheelErr].init_bit(&cube_reg[CAR_Set_RW_Ctrl_Para],12,7,bits_len_union_t::_1bits_,_8UL,SET_AutoCtrl_TransitDueWheelErr_note_t,sizeof(SET_AutoCtrl_TransitDueWheelErr_note_t));
	cube_bit_reg[SET_M_Of_IXX].init_bit(&cube_reg[CAR_Set_I_Matrix],0,0,bits_len_union_t::_32bits_,_32F,SET_M_Of_IXX_note_t,sizeof(SET_M_Of_IXX_note_t));
	cube_bit_reg[SET_M_Of_IYY].init_bit(&cube_reg[CAR_Set_I_Matrix],4,0,bits_len_union_t::_32bits_,_32F,SET_M_Of_IYY_note_t,sizeof(SET_M_Of_IYY_note_t));
	cube_bit_reg[SET_M_Of_IZZ].init_bit(&cube_reg[CAR_Set_I_Matrix],8,0,bits_len_union_t::_32bits_,_32F,SET_M_Of_IZZ_note_t,sizeof(SET_M_Of_IZZ_note_t));
	cube_bit_reg[SET_M_Of_IXY].init_bit(&cube_reg[CAR_Set_I_Matrix],12,0,bits_len_union_t::_32bits_,_32F,SET_M_Of_IXY_note_t,sizeof(SET_M_Of_IXY_note_t));
	cube_bit_reg[SET_M_Of_IXZ].init_bit(&cube_reg[CAR_Set_I_Matrix],16,0,bits_len_union_t::_32bits_,_32F,SET_M_Of_IXZ_note_t,sizeof(SET_M_Of_IXZ_note_t));
	cube_bit_reg[SET_M_Of_IYZ].init_bit(&cube_reg[CAR_Set_I_Matrix],20,0,bits_len_union_t::_32bits_,_32F,SET_M_Of_IYZ_note_t,sizeof(SET_M_Of_IYZ_note_t));
	cube_bit_reg[SET_Inclination].init_bit(&cube_reg[CAR_Set_SGP4OrbitPara],0,0,bits_len_union_t::_64bits_,_64L,SET_Inclination_note_t,sizeof(SET_Inclination_note_t));
	cube_bit_reg[SET_Eccentricity].init_bit(&cube_reg[CAR_Set_SGP4OrbitPara],8,0,bits_len_union_t::_64bits_,_64L,SET_Eccentricity_note_t,sizeof(SET_Eccentricity_note_t));
	cube_bit_reg[SET_RightAscensionOfTheAscendingNode].init_bit(&cube_reg[CAR_Set_SGP4OrbitPara],16,0,bits_len_union_t::_64bits_,_64L,SET_RightAscensionOfTheAscendingNode_note_t,sizeof(SET_RightAscensionOfTheAscendingNode_note_t));
	cube_bit_reg[SET_ArgumentOfPerigee].init_bit(&cube_reg[CAR_Set_SGP4OrbitPara],24,0,bits_len_union_t::_64bits_,_64L,SET_ArgumentOfPerigee_note_t,sizeof(SET_ArgumentOfPerigee_note_t));
	cube_bit_reg[SET_BStarDragTerm].init_bit(&cube_reg[CAR_Set_SGP4OrbitPara],32,0,bits_len_union_t::_64bits_,_64L,SET_BStarDragTerm_note_t,sizeof(SET_BStarDragTerm_note_t));
	cube_bit_reg[SET_MeanMotion].init_bit(&cube_reg[CAR_Set_SGP4OrbitPara],40,0,bits_len_union_t::_64bits_,_64L,SET_MeanMotion_note_t,sizeof(SET_MeanMotion_note_t));
	cube_bit_reg[SET_MeanAnomaly].init_bit(&cube_reg[CAR_Set_SGP4OrbitPara],48,0,bits_len_union_t::_64bits_,_64L,SET_MeanAnomaly_note_t,sizeof(SET_MeanAnomaly_note_t));
	cube_bit_reg[SET_Epoch].init_bit(&cube_reg[CAR_Set_SGP4OrbitPara],56,0,bits_len_union_t::_64bits_,_64L,SET_Epoch_note_t,sizeof(SET_Epoch_note_t));
	cube_bit_reg[SET_MTM_Mode].init_bit(&cube_reg[CAR_Set_Mode_MTMOp],0,0,bits_len_union_t::_8bits_,_8UL,SET_MTM_Mode_note_t,sizeof(SET_MTM_Mode_note_t));
	cube_bit_reg[Source_File_CT].init_bit(&cube_reg[CAR_ConvertJPG],0,0,bits_len_union_t::_8bits_,_8UL,Source_File_CT_note_t,sizeof(Source_File_CT_note_t));
	cube_bit_reg[QualityFactor].init_bit(&cube_reg[CAR_ConvertJPG],1,0,bits_len_union_t::_8bits_,_8UL,QualityFactor_note_t,sizeof(QualityFactor_note_t));
	cube_bit_reg[WhiteBalance].init_bit(&cube_reg[CAR_ConvertJPG],2,0,bits_len_union_t::_8bits_,_8UL,WhiteBalance_note_t,sizeof(WhiteBalance_note_t));
	cube_bit_reg[Save_Config_bit].init_bit(&cube_reg[CAR_Save_Config],0,0,bits_len_union_t::_0bits_,_0UL,Save_Config_bit_note_t,sizeof(Save_Config_bit_note_t));
	cube_bit_reg[Save_OrbitPara_bit].init_bit(&cube_reg[CAR_Save_OrbitPara],0,0,bits_len_union_t::_0bits_,_0UL,Save_OrbitPara_bit_note_t,sizeof(Save_OrbitPara_bit_note_t));
	cube_bit_reg[Camera_Select].init_bit(&cube_reg[CAR_Save_Image ],0,0,bits_len_union_t::_8bits_,_8UL,Camera_Select_note_t,sizeof(Camera_Select_note_t));
	cube_bit_reg[Image_Size].init_bit(&cube_reg[CAR_Save_Image ],1,0,bits_len_union_t::_8bits_,_8UL,Image_Size_note_t,sizeof(Image_Size_note_t));
	cube_bit_reg[Boot_Prog_Index].init_bit(&cube_reg[CAR_SetBootIndex],0,0,bits_len_union_t::_8bits_,_8UL,Boot_Prog_Index_note_t,sizeof(Boot_Prog_Index_note_t));
	cube_bit_reg[RunSelectedProg].init_bit(&cube_reg[CAR_RunSelectedProg],0,0,bits_len_union_t::_0bits_,_0UL,RunSelectedProg_note_t,sizeof(RunSelectedProg_note_t));
	cube_bit_reg[ReadProgInfor].init_bit(&cube_reg[CAR_ReadProgInfor],0,0,bits_len_union_t::_8bits_,_8UL,ReadProgInfor_note_t,sizeof(ReadProgInfor_note_t));
	cube_bit_reg[Source_Prog_Index].init_bit(&cube_reg[CAR_CopyProg_to_InFlash],0,0,bits_len_union_t::_8bits_,_8UL,Source_Prog_Index_note_t,sizeof(Source_Prog_Index_note_t));
	cube_bit_reg[Bootloader_overwrite_flag].init_bit(&cube_reg[CAR_CopyProg_to_InFlash],1,0,bits_len_union_t::_8bits_,_8UL,Bootloader_overwrite_flag_note_t,sizeof(Bootloader_overwrite_flag_note_t));
	cube_bit_reg[SET_Log1_Selection].init_bit(&cube_reg[CAR_Set_SD_Log1_Config],0,0,bits_len_union_t::_80bits_,_80UL,SET_Log1_Selection_note_t,sizeof(SET_Log1_Selection_note_t));
	cube_bit_reg[SET_Log1_Period].init_bit(&cube_reg[CAR_Set_SD_Log1_Config],10,0,bits_len_union_t::_16bits_,_16UL,SET_Log1_Period_note_t,sizeof(SET_Log1_Period_note_t));
	cube_bit_reg[SET_Log1_Destination].init_bit(&cube_reg[CAR_Set_SD_Log1_Config],12,0,bits_len_union_t::_8bits_,_8UL,SET_Log1_Destination_note_t,sizeof(SET_Log1_Destination_note_t));
	cube_bit_reg[SET_Log2_Selection].init_bit(&cube_reg[CAR_Set_SD_Log2_Config],0,0,bits_len_union_t::_80bits_,_80UL,SET_Log2_Selection_note_t,sizeof(SET_Log2_Selection_note_t));
	cube_bit_reg[SET_Log2_Period ].init_bit(&cube_reg[CAR_Set_SD_Log2_Config],10,0,bits_len_union_t::_16bits_,_16UL,SET_Log2_Period_note_t,sizeof(SET_Log2_Period_note_t));
	cube_bit_reg[SET_Log2_Destination].init_bit(&cube_reg[CAR_Set_SD_Log2_Config],12,0,bits_len_union_t::_8bits_,_8UL,SET_Log2_Destination_note_t,sizeof(SET_Log2_Destination_note_t));
	cube_bit_reg[SET_LogUART_Selection].init_bit(&cube_reg[CAR_Set_UART_Log_Config],0,0,bits_len_union_t::_80bits_,_80UL,SET_LogUART_Selection_note_t,sizeof(SET_LogUART_Selection_note_t));
	cube_bit_reg[SET_LogUART_Period ].init_bit(&cube_reg[CAR_Set_UART_Log_Config],10,0,bits_len_union_t::_16bits_,_16UL,SET_LogUART_Period_note_t,sizeof(SET_LogUART_Period_note_t));
	cube_bit_reg[Erase_File_Type].init_bit(&cube_reg[CAR_Erase_File],0,0,bits_len_union_t::_8bits_,_8UL,Erase_File_Type_note_t,sizeof(Erase_File_Type_note_t));
	cube_bit_reg[File_Counter].init_bit(&cube_reg[CAR_Erase_File],1,0,bits_len_union_t::_8bits_,_8UL,File_Counter_note_t,sizeof(File_Counter_note_t));
	cube_bit_reg[Erase_All].init_bit(&cube_reg[CAR_Erase_File],2,0,bits_len_union_t::_1bits_,_8UL,Erase_All_note_t,sizeof(Erase_All_note_t));
	cube_bit_reg[DL_File_type].init_bit(&cube_reg[CAR_Load_File_DL_Block],0,0,bits_len_union_t::_8bits_,_8UL,DL_File_type_note_t,sizeof(DL_File_type_note_t));
	cube_bit_reg[DL_Counter].init_bit(&cube_reg[CAR_Load_File_DL_Block],1,0,bits_len_union_t::_8bits_,_8UL,DL_Counter_note_t,sizeof(DL_Counter_note_t));
	cube_bit_reg[DL_offset].init_bit(&cube_reg[CAR_Load_File_DL_Block],2,0,bits_len_union_t::_32bits_,_32UL,DL_offset_note_t,sizeof(DL_offset_note_t));
	cube_bit_reg[DL_BlockLength].init_bit(&cube_reg[CAR_Load_File_DL_Block],6,0,bits_len_union_t::_16bits_,_16UL,DL_BlockLength_note_t,sizeof(DL_BlockLength_note_t));
	cube_bit_reg[AdFile_List_ReadP].init_bit(&cube_reg[CAR_AdFile_List_ReadP],0,0,bits_len_union_t::_0bits_,_0UL,AdFile_List_ReadP_note_t,sizeof(AdFile_List_ReadP_note_t));
	cube_bit_reg[InitFile_Upload_Desti].init_bit(&cube_reg[CAR_Init_File_Upload],0,0,bits_len_union_t::_8bits_,_8UL,InitFile_Upload_Desti_note_t,sizeof(InitFile_Upload_Desti_note_t));
	cube_bit_reg[InitFile_Upload_BlockSize].init_bit(&cube_reg[CAR_Init_File_Upload],1,0,bits_len_union_t::_8bits_,_8UL,InitFile_Upload_BlockSize_note_t,sizeof(InitFile_Upload_BlockSize_note_t));
	cube_bit_reg[Packet_Number].init_bit(&cube_reg[CAR_Fille_upload_Packet],0,0,bits_len_union_t::_16bits_,_16UL,Packet_Number_note_t,sizeof(Packet_Number_note_t));
	cube_bit_reg[FileUpload_Bytes].init_bit(&cube_reg[CAR_Fille_upload_Packet],2,0,bits_len_union_t::_160bits_,_160UL,FileUpload_Bytes_note_t,sizeof(FileUpload_Bytes_note_t));
	cube_bit_reg[FinalUpload_Desti].init_bit(&cube_reg[CAR_Final_Upload_Block],0,0,bits_len_union_t::_8bits_,_8UL,FinalUpload_Desti_note_t,sizeof(FinalUpload_Desti_note_t));
	cube_bit_reg[FinalUpload_Offset].init_bit(&cube_reg[CAR_Final_Upload_Block],1,0,bits_len_union_t::_32bits_,_32UL,FinalUpload_Offset_note_t,sizeof(FinalUpload_Offset_note_t));
	cube_bit_reg[FinalUpload_BlockSize].init_bit(&cube_reg[CAR_Final_Upload_Block],5,0,bits_len_union_t::_16bits_,_16UL,FinalUpload_BlockSize_note_t,sizeof(FinalUpload_BlockSize_note_t));
	cube_bit_reg[Reset_Upload_Block].init_bit(&cube_reg[CAR_Reset_Upload_Block],0,0,bits_len_union_t::_0bits_,_0UL,Reset_Upload_Block_note_t,sizeof(Reset_Upload_Block_note_t));
	cube_bit_reg[ResetFile_List_ReadP].init_bit(&cube_reg[CAR_ResetFile_List_ReadP],0,0,bits_len_union_t::_0bits_,_0UL,ResetFile_List_ReadP_note_t,sizeof(ResetFile_List_ReadP_note_t));
	cube_bit_reg[InitDLBurst_Msg_Length].init_bit(&cube_reg[CAR_Init_Download_Burst],0,0,bits_len_union_t::_8bits_,_8UL,InitDLBurst_Msg_Length_note_t,sizeof(InitDLBurst_Msg_Length_note_t));
	cube_bit_reg[InitDLBurst_Ignore_HoleMap].init_bit(&cube_reg[CAR_Init_Download_Burst],0,0,bits_len_union_t::_1bits_,_8UL,InitDLBurst_Ignore_HoleMap_note_t,sizeof(InitDLBurst_Ignore_HoleMap_note_t));
	cube_bit_reg[SET_FileUL_HoleMap_1].init_bit(&cube_reg[CAR_Set_HoleMap1],0,0,bits_len_union_t::_128bits_,_128UL,SET_FileUL_HoleMap_1_note_t,sizeof(SET_FileUL_HoleMap_1_note_t));
	cube_bit_reg[SET_FileUL_HoleMap_2].init_bit(&cube_reg[CAR_Set_HoleMap2],0,0,bits_len_union_t::_128bits_,_128UL,SET_FileUL_HoleMap_2_note_t,sizeof(SET_FileUL_HoleMap_2_note_t));
	cube_bit_reg[SET_FileUL_HoleMap_3].init_bit(&cube_reg[CAR_Set_HoleMap3],0,0,bits_len_union_t::_128bits_,_128UL,SET_FileUL_HoleMap_3_note_t,sizeof(SET_FileUL_HoleMap_3_note_t));
	cube_bit_reg[SET_FileUL_HoleMap_4].init_bit(&cube_reg[CAR_Set_HoleMap4],0,0,bits_len_union_t::_128bits_,_128UL,SET_FileUL_HoleMap_4_note_t,sizeof(SET_FileUL_HoleMap_4_note_t));
	cube_bit_reg[SET_FileUL_HoleMap_5].init_bit(&cube_reg[CAR_Set_HoleMap5],0,0,bits_len_union_t::_128bits_,_128UL,SET_FileUL_HoleMap_5_note_t,sizeof(SET_FileUL_HoleMap_5_note_t));
	cube_bit_reg[SET_FileUL_HoleMap_6].init_bit(&cube_reg[CAR_Set_HoleMap6],0,0,bits_len_union_t::_128bits_,_128UL,SET_FileUL_HoleMap_6_note_t,sizeof(SET_FileUL_HoleMap_6_note_t));
	cube_bit_reg[SET_FileUL_HoleMap_7].init_bit(&cube_reg[CAR_Set_HoleMap7],0,0,bits_len_union_t::_128bits_,_128UL,SET_FileUL_HoleMap_7_note_t,sizeof(SET_FileUL_HoleMap_7_note_t));
	cube_bit_reg[SET_FileUL_HoleMap_8].init_bit(&cube_reg[CAR_Set_HoleMap8],0,0,bits_len_union_t::_128bits_,_128UL,SET_FileUL_HoleMap_8_note_t,sizeof(SET_FileUL_HoleMap_8_note_t));

}
#endif // _CUBE_BIT_H
