#ifndef _NRW_REG_H
#define _NRW_REG_H
#include "REG_MAP/NEWC_REG.h"
#define MAX_NRW_REG_NUM 17
nreg_stuct_t NRW_REG[MAX_NRW_REG_NUM];

enum	NRW_REG
{
 DEVICE_ID=	0x1,
 DEVICE_STATE=	0x2,
 DEVICE_FAULT=	0x3,
 DEVICE_AXIS=	0x4,
 CMD_SPEED_RPM=	0x5,
 CMD_RAMP_ENA=	0x6,
 CMD_RAMP_RATE=	0x7,
 CMD_RW_MODE=	0x8,
 CMD_RECOVERY_MODE=	0x9,
 CMD_LOG_MODE=	0xA,
 CMD_LOG_TIME=	0xB,
 CMD_OFFSET_RPM=	0xC,
 CMD_SAVE_CONFIG=	0xD,
 SEN_V_BUS=	0xE,
 SEN_TEMP_C=	0xF,
 SEN_SPEED_RPM=	0x10,
 EST_POWER_W=	0x11,
};
uint8_t DEVICE_ID_D			 = 1;
uint8_t DEVICE_STATE_D			 = 0;
uint16_t DEVICE_FAULT_D			 = 0;
uint8_t DEVICE_AXIS_D			 = 1;
int16_t CMD_SPEED_RPM_D			 = 0;
uint8_t CMD_RAMP_ENA_D			 = 1;
int16_t CMD_RAMP_RATE_D			 = 1000;
uint8_t CMD_RW_MODE_D			 = 0;
uint8_t CMD_RECOVERY_MODE_D			 = 1;
uint8_t CMD_LOG_MODE_D			 = 1;
uint32_t CMD_LOG_TIME_D			 = 1000;
int16_t CMD_OFFSET_RPM_D			 = 600;
uint8_t CMD_SAVE_CONFIG_D			 = 0;
uint16_t SEN_V_BUS_D			 = 0;
int16_t SEN_TEMP_C_D			 = 0;
int16_t SEN_SPEED_RPM_D			 = 0;
float EST_POWER_W_D			 = 0;
uint8_t NRW_REG_buf_flash[13];
void NRW_REG_setup_reg(){
	nreg_init(&NRW_REG[DEVICE_ID],(uint8_t*)&DEVICE_ID_D,sizeof(DEVICE_ID_D),1,0);
	nreg_init(&NRW_REG[DEVICE_STATE],(uint8_t*)&DEVICE_STATE_D,sizeof(DEVICE_STATE_D),0,1);
	nreg_init(&NRW_REG[DEVICE_FAULT],(uint8_t*)&DEVICE_FAULT_D,sizeof(DEVICE_FAULT_D),0,1);
	nreg_init(&NRW_REG[DEVICE_AXIS],(uint8_t*)&DEVICE_AXIS_D,sizeof(DEVICE_AXIS_D),1,1);
	nreg_init(&NRW_REG[CMD_SPEED_RPM],(uint8_t*)&CMD_SPEED_RPM_D,sizeof(CMD_SPEED_RPM_D),0,0);
	nreg_init(&NRW_REG[CMD_RAMP_ENA],(uint8_t*)&CMD_RAMP_ENA_D,sizeof(CMD_RAMP_ENA_D),1,0);
	nreg_init(&NRW_REG[CMD_RAMP_RATE],(uint8_t*)&CMD_RAMP_RATE_D,sizeof(CMD_RAMP_RATE_D),1,0);
	nreg_init(&NRW_REG[CMD_RW_MODE],(uint8_t*)&CMD_RW_MODE_D,sizeof(CMD_RW_MODE_D),0,0);
	nreg_init(&NRW_REG[CMD_RECOVERY_MODE],(uint8_t*)&CMD_RECOVERY_MODE_D,sizeof(CMD_RECOVERY_MODE_D),1,0);
	nreg_init(&NRW_REG[CMD_LOG_MODE],(uint8_t*)&CMD_LOG_MODE_D,sizeof(CMD_LOG_MODE_D),1,0);
	nreg_init(&NRW_REG[CMD_LOG_TIME],(uint8_t*)&CMD_LOG_TIME_D,sizeof(CMD_LOG_TIME_D),1,0);
	nreg_init(&NRW_REG[CMD_OFFSET_RPM],(uint8_t*)&CMD_OFFSET_RPM_D,sizeof(CMD_OFFSET_RPM_D),1,0);
	nreg_init(&NRW_REG[CMD_SAVE_CONFIG],(uint8_t*)&CMD_SAVE_CONFIG_D,sizeof(CMD_SAVE_CONFIG_D),0,0);
	nreg_init(&NRW_REG[SEN_V_BUS],(uint8_t*)&SEN_V_BUS_D,sizeof(SEN_V_BUS_D),0,1);
	nreg_init(&NRW_REG[SEN_TEMP_C],(uint8_t*)&SEN_TEMP_C_D,sizeof(SEN_TEMP_C_D),0,1);
	nreg_init(&NRW_REG[SEN_SPEED_RPM],(uint8_t*)&SEN_SPEED_RPM_D,sizeof(SEN_SPEED_RPM_D),0,1);
	nreg_init(&NRW_REG[EST_POWER_W],(uint8_t*)&EST_POWER_W_D,sizeof(EST_POWER_W_D),0,1);

}
#endif // _NRW_REG_H
