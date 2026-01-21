#ifndef _ADCS_MAIN_REG_H
#define _ADCS_MAIN_REG_H
#define MAX_ADCS_MAIN_REG 77
#include "REG_MAP/KMUTNB_ADCS_REG.h"
KMUTNB_ADCS_REG adcs_reg[MAX_ADCS_MAIN_REG];

enum	adcs_reg
{
 ADCS_ID=	0x0,
 TLM_COUNTER=	0x1,
 ERR_FLG_MAIN=	0x2,
 ERR_RW_FLG=	0x3,
 ERR_MTQ_FLG=	0x4,
 ACK_DATA=	0x5,
 ACK_TLC=	0x6,
 CMD_SAVE_A_CONFIG=	0x7,
 CMD_RUN_MODE=	0x8,
 SEN_RESET_ROUND=	0x9,
 CMD_RUN_TIMEOUT_S=	0xA,
 CMD_I2C_WATCHDOG_MS=	0xB,
 SEN_RWA_CURRENT=	0xC,
 SEN_MTQ_TEMP=	0xD,
 SEN_BUS_CURRENT=	0xE,
 SEN_P_ECEF=	0xF,
 SEN_V_ECEF=	0x10,
 EST_ATT_NED=	0x11,
 SIM_SAT_STATE=	0x12,
 SIM_OUT_STATE=	0x13,
 SIM_SETUP_STATE=	0x14,
 DEVICE_RW_STATE=	0x15,
 DEVICE_RW_FAULT=	0x16,
 DEVICE_RW_AXIS=	0x17,
 EST_RWA_POWER_W=	0x18,
 CMD_RWA_RUNMODE=	0x19,
 CMD_RWA_RAMP_ENA=	0x1A,
 CMD_RWA_RAMP_RATE=	0x1B,
 CMD_RWA_SAVE_CONFIG=	0x1C,
 SEN_RWA_VBUS=	0x1D,
 SEN_RWA_TEMP_C=	0x1E,
 CMD_CAN_MODE=	0x1F,
 CMD_UART_MODE=	0x20,
 CMD_I2C_MODE=	0x21,
 CMD_SLAVE_MODE=	0x22,
 CMD_MEM_MODE=	0x23,
 CMD_CUBE_ACS_MODE=	0x24,
 CMD_CUBE_ADS_MODE=	0x25,
 SEN_PING_MODE_MS=	0x26,
 FIX_INERTIA_SAT=	0x27,
 FIX_INCLINATION=	0x28,
 FIX_ECCENTRICITY=	0x29,
 FIX_RAAN=	0x2A,
 FIX_ARG_PERIGEE=	0x2B,
 FIX_MEAN_MOTION=	0x2C,
 FIX_MEAN_ANAMOLY=	0x2D,
 SEN_RWA_RPM=	0x2E,
 SEN_MTQ_VOLT=	0x2F,
 SEN_MTQ_CURRENT=	0x30,
 EST_Q_ECI2B=	0x31,
 EST_T_CMD=	0x32,
 EST_Q_ERR=	0x33,
 EST_Q_ORF2B=	0x34,
 EST_GYRO_BIAS=	0x35,
 EST_LATLONALT=	0x36,
 CMD_RWA_RPM=	0x37,
 CMD_MTQ_PWM=	0x38,
 CMD_RWA_MODE=	0x39,
 CMD_MTQ_MODE=	0x3A,
 CMD_ACS_MODE=	0x3B,
 CMD_ADS_MODE=	0x3C,
 CMD_MODE_DIF_MS=	0x3D,
 SEN_MAG1_NT=	0x3E,
 SEN_GYRO1_DEG=	0x3F,
 SEN_SUN_VEC=	0x40,
 EST_Q_ECEF2B=	0x41,
 EST_B_RATES=	0x42,
 EST_P_ECEF=	0x43,
 EST_V_ECEF=	0x44,
 CMD_PID_CONS=	0x45,
 REF_MAG_NT=	0x46,
 REF_SUN_POS=	0x47,
 SEN_MAG2_NT=	0x48,
 SEN_GYRO2_DEG=	0x49,
 SEN_NADIR_VEC=	0x4A,
 SEN_B_RATES=	0x4B,
 FIX_UNIX_TIME=	0x4C,
 CMD_MTQ_CUR = 0x4D,
};
uint8_t ADCS_ID_D			 = 90;
uint32_t TLM_COUNTER_D			 = 100;
uint8_t ERR_FLG_MAIN_D			 = 0;
uint8_t ERR_RW_FLG_D			 = 0;
uint8_t ERR_MTQ_FLG_D			 = 0;
uint8_t ACK_DATA_D			 = 0;
uint8_t ACK_TLC_D			 = 0;
uint8_t CMD_SAVE_A_CONFIG_D			 = 0;
uint8_t CMD_RUN_MODE_D			 = 0;
uint32_t SEN_RESET_ROUND_D			 = 100;
uint32_t CMD_RUN_TIMEOUT_S_D			 = 2000;
uint32_t CMD_I2C_WATCHDOG_MS_D			 = 5000;
int16_t SEN_RWA_CURRENT_D[4]			 = {0,0,0,0};
float SEN_MTQ_TEMP_D[3]			 = {0,0,0};
uint16_t SEN_BUS_CURRENT_D			 = 0;
float SEN_P_ECEF_D[3]			 = {0,0,0};
float SEN_V_ECEF_D[3]			 = {0,0,0};
float EST_ATT_NED_D[3]			 = {0,0,0};
uint8_t SIM_SAT_STATE_D[200]			 = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
uint8_t SIM_OUT_STATE_D[150]			 = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
uint8_t SIM_SETUP_STATE_D[24]			 = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
uint8_t DEVICE_RW_STATE_D[4]			 = {0,0,0,0};
uint8_t DEVICE_RW_FAULT_D[4]			 = {0,0,0,0};
uint8_t DEVICE_RW_AXIS_D[4]			 = {0,0,0,0};
float EST_RWA_POWER_W_D[4]			 = {0,0,0,0};
uint8_t CMD_RWA_RUNMODE_D[4]			 = {0,0,0,0};
uint8_t CMD_RWA_RAMP_ENA_D[4]			 = {0,0,0,0};
int16_t CMD_RWA_RAMP_RATE_D[4]			 = {1700,1700,1700,1700};
uint8_t CMD_RWA_SAVE_CONFIG_D[4]			 = {0,0,0,0};
uint16_t SEN_RWA_VBUS_D[4]			 = {0,0,0,0};
int16_t SEN_RWA_TEMP_C_D[4]			 = {0,0,0,0};
uint8_t CMD_CAN_MODE_D			 = 0;
uint8_t CMD_UART_MODE_D			 = 0;
uint8_t CMD_I2C_MODE_D			 = 0;
uint8_t CMD_SLAVE_MODE_D			 = 0;
uint8_t CMD_MEM_MODE_D			 = 0;
uint8_t CMD_CUBE_ACS_MODE_D			 = 0;
uint8_t CMD_CUBE_ADS_MODE_D			 = 0;
uint16_t SEN_PING_MODE_MS_D[20]			 = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
float FIX_INERTIA_SAT_D[6]			 = {1,1,1,0,0,0};
float FIX_INCLINATION_D			 = 0;
float FIX_ECCENTRICITY_D			 = 0;
float FIX_RAAN_D			 = 0;
float FIX_ARG_PERIGEE_D			 = 0;
float FIX_MEAN_MOTION_D			 = 0;
float FIX_MEAN_ANAMOLY_D			 = 0;
int16_t SEN_RWA_RPM_D[4]			 = {0,0,0,0};
int16_t SEN_MTQ_VOLT_D[3]			 = {0,0,0};
int16_t SEN_MTQ_CURRENT_D[3]			 = {0,0,0};
float EST_Q_ECI2B_D[4]			 = {1,0,0,0};
float EST_T_CMD_D[3]			 = {0,0,0};
float EST_Q_ERR_D[4]			 = {1,0,0,0};
float EST_Q_ORF2B_D[4]			 = {1,0,0,0};
float EST_GYRO_BIAS_D[3]			 = {0,0,0};
float EST_LATLONALT_D[3]			 = {0,0,0};
int16_t CMD_RWA_RPM_D[4]			 = {0,0,0,0};
int16_t CMD_MTQ_PWM_D[3]			 = {0,0,0};
uint8_t CMD_RWA_MODE_D			 = 0;
uint8_t CMD_MTQ_MODE_D			 = 0;
uint8_t CMD_ACS_MODE_D			 = 0;
uint8_t CMD_ADS_MODE_D			 = 0;
uint16_t CMD_MODE_DIF_MS_D			 = 50;
float SEN_MAG1_NT_D[3]			 = {0,0,0};
float SEN_GYRO1_DEG_D[3]			 = {0,0,0};
float SEN_SUN_VEC_D[3]			 = {0,0,0};
float EST_Q_ECEF2B_D[4]			 = {1,0,0,0};
float EST_B_RATES_D[3]			 = {0,0,0};
float EST_P_ECEF_D[3]			 = {0,0,0};
float EST_V_ECEF_D[3]			 = {0,0,0};
float CMD_PID_CONS_D[6]			 = {1,1,1,0,0,0};
float REF_MAG_NT_D[3]			 = {0,0,0};
float REF_SUN_POS_D[3]			 = {0,0,0};
float SEN_MAG2_NT_D[3]			 = {0,0,0};
float SEN_GYRO2_DEG_D[3]			 = {0,0,0};
float SEN_NADIR_VEC_D[3]			 = {0,0,0};
float SEN_B_RATES_D[3]			 = {0,0,0};
uint32_t FIX_UNIX_TIME_D			 = 1715317200;
float CMD_MTQ_CUR_D[3] = {0,0,0};
float SEN_BUS_VOLTAGE_D = 0;
int16_t SEN_MTQA_CURR_D = 0;
#define MAX_REG_FRAM_BYTE (131+8)

void main_setup_reg(){
	adcs_reg[ADCS_ID].init((uint8_t*)&ADCS_ID_D,1,(uint8_t)data_type::_8UL,0);
	adcs_reg[TLM_COUNTER].init((uint8_t*)&TLM_COUNTER_D,4,(uint8_t)data_type::_32UL,1);
	adcs_reg[ERR_FLG_MAIN].init((uint8_t*)&ERR_FLG_MAIN_D,1,(uint8_t)data_type::_8UL,0);
	adcs_reg[ERR_RW_FLG].init((uint8_t*)&ERR_RW_FLG_D,1,(uint8_t)data_type::_8UL,0);
	adcs_reg[ERR_MTQ_FLG].init((uint8_t*)&ERR_MTQ_FLG_D,1,(uint8_t)data_type::_8UL,0);
	adcs_reg[ACK_DATA].init((uint8_t*)&ACK_DATA_D,1,(uint8_t)data_type::_8UL,0);
	adcs_reg[ACK_TLC].init((uint8_t*)&ACK_TLC_D,1,(uint8_t)data_type::_8UL,0);
	adcs_reg[CMD_SAVE_A_CONFIG].init((uint8_t*)&CMD_SAVE_A_CONFIG_D,1,(uint8_t)data_type::_8UL,0);
	adcs_reg[CMD_RUN_MODE].init((uint8_t*)&CMD_RUN_MODE_D,1,(uint8_t)data_type::_8UL,1);
	adcs_reg[SEN_RESET_ROUND].init((uint8_t*)&SEN_RESET_ROUND_D,4,(uint8_t)data_type::_32UL,1);
	adcs_reg[CMD_RUN_TIMEOUT_S].init((uint8_t*)&CMD_RUN_TIMEOUT_S_D,4,(uint8_t)data_type::_32UL,1);
	adcs_reg[CMD_I2C_WATCHDOG_MS].init((uint8_t*)&CMD_I2C_WATCHDOG_MS_D,4,(uint8_t)data_type::_32UL,1);
	adcs_reg[SEN_RWA_CURRENT].init((uint8_t*)&SEN_RWA_CURRENT_D,8,(uint8_t)data_type::_16L,0);
	adcs_reg[SEN_MTQ_TEMP].init((uint8_t*)&SEN_MTQ_TEMP_D,12,(uint8_t)data_type::_32F,0);
	adcs_reg[SEN_BUS_CURRENT].init((uint8_t*)&SEN_BUS_CURRENT_D,2,(uint8_t)data_type::_16UL,0);
	adcs_reg[SEN_P_ECEF].init((uint8_t*)&SEN_P_ECEF_D,12,(uint8_t)data_type::_32F,0);
	adcs_reg[SEN_V_ECEF].init((uint8_t*)&SEN_V_ECEF_D,12,(uint8_t)data_type::_32F,0);
	adcs_reg[EST_ATT_NED].init((uint8_t*)&EST_ATT_NED_D,12,(uint8_t)data_type::_32F,0);
	adcs_reg[SIM_SAT_STATE].init((uint8_t*)&SIM_SAT_STATE_D,200,(uint8_t)data_type::_8UL,0);
	adcs_reg[SIM_OUT_STATE].init((uint8_t*)&SIM_OUT_STATE_D,150,(uint8_t)data_type::_8UL,0);
	adcs_reg[SIM_SETUP_STATE].init((uint8_t*)&SIM_SETUP_STATE_D,100,(uint8_t)data_type::_8UL,0);
	adcs_reg[DEVICE_RW_STATE].init((uint8_t*)&DEVICE_RW_STATE_D,4,(uint8_t)data_type::_8UL,0);
	adcs_reg[DEVICE_RW_FAULT].init((uint8_t*)&DEVICE_RW_FAULT_D,4,(uint8_t)data_type::_8UL,0);
	adcs_reg[DEVICE_RW_AXIS].init((uint8_t*)&DEVICE_RW_AXIS_D,4,(uint8_t)data_type::_8UL,0);
	adcs_reg[EST_RWA_POWER_W].init((uint8_t*)&EST_RWA_POWER_W_D,16,(uint8_t)data_type::_32F,0);
	adcs_reg[CMD_RWA_RUNMODE].init((uint8_t*)&CMD_RWA_RUNMODE_D,4,(uint8_t)data_type::_8UL,1);
	adcs_reg[CMD_RWA_RAMP_ENA].init((uint8_t*)&CMD_RWA_RAMP_ENA_D,4,(uint8_t)data_type::_8UL,1);
	adcs_reg[CMD_RWA_RAMP_RATE].init((uint8_t*)&CMD_RWA_RAMP_RATE_D,8,(uint8_t)data_type::_16L,1);
	adcs_reg[CMD_RWA_SAVE_CONFIG].init((uint8_t*)&CMD_RWA_SAVE_CONFIG_D,4,(uint8_t)data_type::_8UL,0);
	adcs_reg[SEN_RWA_VBUS].init((uint8_t*)&SEN_RWA_VBUS_D,8,(uint8_t)data_type::_16UL,0);
	adcs_reg[SEN_RWA_TEMP_C].init((uint8_t*)&SEN_RWA_TEMP_C_D,8,(uint8_t)data_type::_16L,0);
	adcs_reg[CMD_CAN_MODE].init((uint8_t*)&CMD_CAN_MODE_D,1,(uint8_t)data_type::_8UL,0);
	adcs_reg[CMD_UART_MODE].init((uint8_t*)&CMD_UART_MODE_D,1,(uint8_t)data_type::_8UL,0);
	adcs_reg[CMD_I2C_MODE].init((uint8_t*)&CMD_I2C_MODE_D,1,(uint8_t)data_type::_8UL,0);
	adcs_reg[CMD_SLAVE_MODE].init((uint8_t*)&CMD_SLAVE_MODE_D,1,(uint8_t)data_type::_8UL,0);
	adcs_reg[CMD_MEM_MODE].init((uint8_t*)&CMD_MEM_MODE_D,1,(uint8_t)data_type::_8UL,0);
	adcs_reg[CMD_CUBE_ACS_MODE].init((uint8_t*)&CMD_CUBE_ACS_MODE_D,1,(uint8_t)data_type::_8UL,1);
	adcs_reg[CMD_CUBE_ADS_MODE].init((uint8_t*)&CMD_CUBE_ADS_MODE_D,1,(uint8_t)data_type::_8UL,1);
	adcs_reg[SEN_PING_MODE_MS].init((uint8_t*)&SEN_PING_MODE_MS_D,40,(uint8_t)data_type::_16UL,0);
	adcs_reg[FIX_INERTIA_SAT].init((uint8_t*)&FIX_INERTIA_SAT_D,24,(uint8_t)data_type::_32F,1);
	adcs_reg[FIX_INCLINATION].init((uint8_t*)&FIX_INCLINATION_D,4,(uint8_t)data_type::_32F,1);
	adcs_reg[FIX_ECCENTRICITY].init((uint8_t*)&FIX_ECCENTRICITY_D,4,(uint8_t)data_type::_32F,1);
	adcs_reg[FIX_RAAN].init((uint8_t*)&FIX_RAAN_D,4,(uint8_t)data_type::_32F,1);
	adcs_reg[FIX_ARG_PERIGEE].init((uint8_t*)&FIX_ARG_PERIGEE_D,4,(uint8_t)data_type::_32F,1);
	adcs_reg[FIX_MEAN_MOTION].init((uint8_t*)&FIX_MEAN_MOTION_D,4,(uint8_t)data_type::_32F,1);
	adcs_reg[FIX_MEAN_ANAMOLY].init((uint8_t*)&FIX_MEAN_ANAMOLY_D,4,(uint8_t)data_type::_32F,1);
	adcs_reg[SEN_RWA_RPM].init((uint8_t*)&SEN_RWA_RPM_D,8,(uint8_t)data_type::_16L,0);
	adcs_reg[SEN_MTQ_VOLT].init((uint8_t*)&SEN_MTQ_VOLT_D,6,(uint8_t)data_type::_16L,0);
	adcs_reg[SEN_MTQ_CURRENT].init((uint8_t*)&SEN_MTQ_CURRENT_D,6,(uint8_t)data_type::_16L,0);
	adcs_reg[EST_Q_ECI2B].init((uint8_t*)&EST_Q_ECI2B_D,16,(uint8_t)data_type::_32F,0);
	adcs_reg[EST_T_CMD].init((uint8_t*)&EST_T_CMD_D,12,(uint8_t)data_type::_32F,0);
	adcs_reg[EST_Q_ERR].init((uint8_t*)&EST_Q_ERR_D,16,(uint8_t)data_type::_32F,0);
	adcs_reg[EST_Q_ORF2B].init((uint8_t*)&EST_Q_ORF2B_D,16,(uint8_t)data_type::_32F,0);
	adcs_reg[EST_GYRO_BIAS].init((uint8_t*)&EST_GYRO_BIAS_D,12,(uint8_t)data_type::_32F,0);
	adcs_reg[EST_LATLONALT].init((uint8_t*)&EST_LATLONALT_D,12,(uint8_t)data_type::_32F,0);
	adcs_reg[CMD_RWA_RPM].init((uint8_t*)&CMD_RWA_RPM_D,8,(uint8_t)data_type::_16L,1);
	adcs_reg[CMD_MTQ_PWM].init((uint8_t*)&CMD_MTQ_PWM_D,6,(uint8_t)data_type::_16L,1);
	adcs_reg[CMD_RWA_MODE].init((uint8_t*)&CMD_RWA_MODE_D,1,(uint8_t)data_type::_8UL,1);
	adcs_reg[CMD_MTQ_MODE].init((uint8_t*)&CMD_MTQ_MODE_D,1,(uint8_t)data_type::_8UL,1);
	adcs_reg[CMD_ACS_MODE].init((uint8_t*)&CMD_ACS_MODE_D,1,(uint8_t)data_type::_8UL,1);
	adcs_reg[CMD_ADS_MODE].init((uint8_t*)&CMD_ADS_MODE_D,1,(uint8_t)data_type::_8UL,1);
	adcs_reg[CMD_MODE_DIF_MS].init((uint8_t*)&CMD_MODE_DIF_MS_D,2,(uint8_t)data_type::_16UL,1);
	adcs_reg[SEN_MAG1_NT].init((uint8_t*)&SEN_MAG1_NT_D,12,(uint8_t)data_type::_32F,0);
	adcs_reg[SEN_GYRO1_DEG].init((uint8_t*)&SEN_GYRO1_DEG_D,12,(uint8_t)data_type::_32F,0);
	adcs_reg[SEN_SUN_VEC].init((uint8_t*)&SEN_SUN_VEC_D,12,(uint8_t)data_type::_32F,0);
	adcs_reg[EST_Q_ECEF2B].init((uint8_t*)&EST_Q_ECEF2B_D,16,(uint8_t)data_type::_32F,0);
	adcs_reg[EST_B_RATES].init((uint8_t*)&EST_B_RATES_D,12,(uint8_t)data_type::_32F,0);
	adcs_reg[EST_P_ECEF].init((uint8_t*)&EST_P_ECEF_D,12,(uint8_t)data_type::_32F,0);
	adcs_reg[EST_V_ECEF].init((uint8_t*)&EST_V_ECEF_D,12,(uint8_t)data_type::_32F,0);
	adcs_reg[CMD_PID_CONS].init((uint8_t*)&CMD_PID_CONS_D,24,(uint8_t)data_type::_32F,1);
	adcs_reg[REF_MAG_NT].init((uint8_t*)&REF_MAG_NT_D,12,(uint8_t)data_type::_32F,0);
	adcs_reg[REF_SUN_POS].init((uint8_t*)&REF_SUN_POS_D,12,(uint8_t)data_type::_32F,0);
	adcs_reg[SEN_MAG2_NT].init((uint8_t*)&SEN_MAG2_NT_D,12,(uint8_t)data_type::_32F,0);
	adcs_reg[SEN_GYRO2_DEG].init((uint8_t*)&SEN_GYRO2_DEG_D,12,(uint8_t)data_type::_32F,0);
	adcs_reg[SEN_NADIR_VEC].init((uint8_t*)&SEN_NADIR_VEC_D,12,(uint8_t)data_type::_32F,0);
	adcs_reg[SEN_B_RATES].init((uint8_t*)&SEN_B_RATES_D,12,(uint8_t)data_type::_32F,0);
	adcs_reg[FIX_UNIX_TIME].init((uint8_t*)&FIX_UNIX_TIME_D,4,(uint8_t)data_type::_32UL,1);
	adcs_reg[CMD_MTQ_CUR].init((uint8_t*)&CMD_MTQ_CUR_D,12,(uint8_t)data_type::_32F,0);

}

void setup_init_cond(){
	adcs_reg[TLM_COUNTER].set<uint32_t,1>({100});
	adcs_reg[CMD_RUN_MODE].set<uint8_t,1>({0});
	adcs_reg[SEN_RESET_ROUND].set<uint32_t,1>({100});
	adcs_reg[CMD_RUN_TIMEOUT_S].set<uint32_t,1>({2000});
	adcs_reg[CMD_I2C_WATCHDOG_MS].set<uint32_t,1>({5000});
	adcs_reg[CMD_RWA_RUNMODE].set<uint8_t,4>({0,0,0,0});
	adcs_reg[CMD_RWA_RAMP_ENA].set<uint8_t,4>({0,0,0,0});
	adcs_reg[CMD_RWA_RAMP_RATE].set<int16_t,4>({1700,1700,1700,1700});
	adcs_reg[CMD_CUBE_ACS_MODE].set<uint8_t,1>({0});
	adcs_reg[CMD_CUBE_ADS_MODE].set<uint8_t,1>({0});
	adcs_reg[FIX_INERTIA_SAT].set<float,6>({1,1,1,0,0,0});
	adcs_reg[FIX_INCLINATION].set<float,1>({0});
	adcs_reg[FIX_ECCENTRICITY].set<float,1>({0});
	adcs_reg[FIX_RAAN].set<float,1>({0});
	adcs_reg[FIX_ARG_PERIGEE].set<float,1>({0});
	adcs_reg[FIX_MEAN_MOTION].set<float,1>({0});
	adcs_reg[FIX_MEAN_ANAMOLY].set<float,1>({0});
	adcs_reg[CMD_RWA_RPM].set<int16_t,4>({0,0,0,0});
	adcs_reg[CMD_MTQ_PWM].set<int16_t,3>({0,0,0});
	adcs_reg[CMD_RWA_MODE].set<uint8_t,1>({0});
	adcs_reg[CMD_MTQ_MODE].set<uint8_t,1>({0});
	adcs_reg[CMD_ACS_MODE].set<uint8_t,1>({0});
	adcs_reg[CMD_ADS_MODE].set<uint8_t,1>({0});
	adcs_reg[CMD_MODE_DIF_MS].set<uint16_t,1>({50});
	adcs_reg[CMD_PID_CONS].set<float,6>({1,1,1,0,0,0});
	adcs_reg[FIX_UNIX_TIME].set<uint32_t,1>({1715317200});

}



#endif // _ADCS_G4_H
