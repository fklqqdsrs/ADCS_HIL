#ifndef _4NRW_REG_H
#define _4NRW_REG_H
#include "REG_MAP/NEWC_REG.h"
#include "newc_NRW_REG.h"
#include "REG_MAP/REG_ADCS_MAIN/ADCS_MAIN_REG.h"
#define MAX_NRW_REG_NUM 17
#define MAX_NRW_DEVICE 4

nreg_stuct_t RW4_REG[MAX_NRW_DEVICE][MAX_NRW_REG_NUM];

void NRW4_REG_setup_reg()
{
	for (int i = 0; i < MAX_NRW_DEVICE; i++)
	{
		//nreg_init(&RW4_REG[i][DEVICE_ID], (uint8_t *)&DEVICE_ID_D, sizeof(DEVICE_ID_D), 1, 0);
		nreg_init(&RW4_REG[i][DEVICE_STATE], (uint8_t *)&DEVICE_RW_STATE_D[i], sizeof(DEVICE_RW_STATE_D[i]), 0, 1);
		nreg_init(&RW4_REG[i][DEVICE_FAULT], (uint8_t *)&DEVICE_RW_FAULT_D[i], sizeof(DEVICE_RW_FAULT_D[i]), 0, 1);
		nreg_init(&RW4_REG[i][DEVICE_AXIS], (uint8_t *)&DEVICE_RW_AXIS_D[i], sizeof(DEVICE_RW_AXIS_D[i]), 1, 1);
		nreg_init(&RW4_REG[i][CMD_SPEED_RPM], (uint8_t *)&CMD_RWA_RPM_D[i], sizeof(CMD_RWA_RPM_D[i]), 0, 0);
		nreg_init(&RW4_REG[i][CMD_RAMP_ENA], (uint8_t *)&CMD_RWA_RAMP_ENA_D[i], sizeof(CMD_RWA_RAMP_ENA_D[i]), 1, 0);
		nreg_init(&RW4_REG[i][CMD_RAMP_RATE], (uint8_t *)&CMD_RWA_RAMP_RATE_D[i], sizeof(CMD_RWA_RAMP_RATE_D[i]), 1, 0);
		nreg_init(&RW4_REG[i][CMD_RW_MODE], (uint8_t *)&CMD_RWA_RUNMODE_D[i], sizeof(CMD_RWA_RUNMODE_D[i]), 0, 0);
		//nreg_init(&RW4_REG[i][CMD_RECOVERY_MODE], (uint8_t *)&CMD_RECOVERY_MODE_D, sizeof(CMD_RECOVERY_MODE_D), 1, 0);
		//nreg_init(&RW4_REG[i][CMD_LOG_MODE], (uint8_t *)&CMD_LOG_MODE_D, sizeof(CMD_LOG_MODE_D), 1, 0);
		//nreg_init(&RW4_REG[i][CMD_LOG_TIME], (uint8_t *)&CMD_LOG_TIME_D, sizeof(CMD_LOG_TIME_D), 1, 0);
		//nreg_init(&RW4_REG[i][CMD_OFFSET_RPM], (uint8_t *)&CMD_OFFSET_RPM_D, sizeof(CMD_OFFSET_RPM_D), 1, 0);
		nreg_init(&RW4_REG[i][CMD_SAVE_CONFIG], (uint8_t *)&CMD_RWA_SAVE_CONFIG_D[i], sizeof(CMD_RWA_SAVE_CONFIG_D[i]), 0, 0);
		nreg_init(&RW4_REG[i][SEN_V_BUS], (uint8_t *)&SEN_RWA_VBUS_D[i], sizeof(SEN_RWA_VBUS_D[i]), 0, 1);
		nreg_init(&RW4_REG[i][SEN_TEMP_C], (uint8_t *)&SEN_RWA_TEMP_C_D[i], sizeof(SEN_RWA_TEMP_C_D[i]), 0, 1);
		nreg_init(&RW4_REG[i][SEN_SPEED_RPM], (uint8_t *)&SEN_RWA_RPM_D[i], sizeof(SEN_RWA_RPM_D[i]), 0, 1);
		nreg_init(&RW4_REG[i][EST_POWER_W], (uint8_t *)&EST_RWA_POWER_W_D[i], sizeof(EST_RWA_POWER_W_D[i]), 0, 1);
	}
}
#endif // _NRW_REG[i]_H
