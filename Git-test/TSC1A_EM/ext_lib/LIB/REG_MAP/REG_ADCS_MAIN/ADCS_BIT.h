#ifndef _ADCS_BIT_H
#define _ADCS_BIT_H
#include "REG_MAP/NEW_BIT_REG.h"
#include "REG_MAP/KMUTNB_ADCS_REG.h"
#include "REG_MAP/REG_ADCS_MAIN/ADCS_MAIN_REG.h"
NEW_BIT_REG ADCS_BIT[17];
char SIM_INERTIA_SAT_note_t[]			 = "nan\r\n";
char SIM_ACS_MODE_note_t[]			 = "nan\r\n";
char SIM_ADS_MODE_note_t[]			 = "nan\r\n";
char SIM_MODE_DIF_MS_note_t[]			 = "nan\r\n";
char SIM_MAG1_NT_note_t[]			 = "nan\r\n";
char SIM_GYRO1_DEG_note_t[]			 = "nan\r\n";
char SIM_SUN_VEC_note_t[]			 = "nan\r\n";
char SIM_Q_ECEF2B_note_t[]			 = "nan\r\n";
char SIM_B_RATES_note_t[]			 = "nan\r\n";
char SIM_P_ECEF_note_t[]			 = "nan\r\n";
char SIM_V_ECEF_note_t[]			 = "nan\r\n";
char SIM_PID_CONS_note_t[]			 = "nan\r\n";
char SIM_RWA_RPM_note_t[]			 = "nan\r\n";
char SIM_MTQ_CURRENT_note_t[]			 = "nan\r\n";
char SIM_Q_ECI2B_note_t[]			 = "nan\r\n";
char SIM_T_CMD_note_t[]			 = "nan\r\n";
char SIM_Q_ERR_note_t[]			 = "nan\r\n";
enum	ADCS_BIT
{
 SIM_INERTIA_SAT=	0x0,
 SIM_ACS_MODE=	0x1,
 SIM_ADS_MODE=	0x2,
 SIM_MODE_DIF_MS=	0x3,
 SIM_MAG1_NT=	0x4,
 SIM_GYRO1_DEG=	0x5,
 SIM_SUN_VEC=	0x6,
 SIM_Q_ECEF2B=	0x7,
 SIM_B_RATES=	0x8,
 SIM_P_ECEF=	0x9,
 SIM_V_ECEF=	0xA,
 SIM_PID_CONS=	0xB,
 SIM_RWA_RPM=	0xC,
 SIM_MTQ_CURRENT=	0xD,
 SIM_Q_ECI2B=	0xE,
 SIM_T_CMD=	0xF,
 SIM_Q_ERR=	0x10,
};
void ADCS_BIT_setup(){
	ADCS_BIT[SIM_INERTIA_SAT].init_bit(&adcs_reg[SIM_SETUP_STATE],0,0,bit_len_union_t::_0bit,24,SIM_INERTIA_SAT_note_t,sizeof(SIM_INERTIA_SAT_note_t));
	ADCS_BIT[SIM_ACS_MODE].init_bit(&adcs_reg[SIM_SAT_STATE],0,0,bit_len_union_t::_8bit,1,SIM_ACS_MODE_note_t,sizeof(SIM_ACS_MODE_note_t));
	ADCS_BIT[SIM_ADS_MODE].init_bit(&adcs_reg[SIM_SAT_STATE],1,0,bit_len_union_t::_8bit,1,SIM_ADS_MODE_note_t,sizeof(SIM_ADS_MODE_note_t));
	ADCS_BIT[SIM_MODE_DIF_MS].init_bit(&adcs_reg[SIM_SAT_STATE],2,0,bit_len_union_t::_0bit,2,SIM_MODE_DIF_MS_note_t,sizeof(SIM_MODE_DIF_MS_note_t));
	ADCS_BIT[SIM_MAG1_NT].init_bit(&adcs_reg[SIM_SAT_STATE],4,0,bit_len_union_t::_0bit,12,SIM_MAG1_NT_note_t,sizeof(SIM_MAG1_NT_note_t));
	ADCS_BIT[SIM_GYRO1_DEG].init_bit(&adcs_reg[SIM_SAT_STATE],16,0,bit_len_union_t::_0bit,12,SIM_GYRO1_DEG_note_t,sizeof(SIM_GYRO1_DEG_note_t));
	ADCS_BIT[SIM_SUN_VEC].init_bit(&adcs_reg[SIM_SAT_STATE],28,0,bit_len_union_t::_0bit,12,SIM_SUN_VEC_note_t,sizeof(SIM_SUN_VEC_note_t));
	ADCS_BIT[SIM_Q_ECEF2B].init_bit(&adcs_reg[SIM_SAT_STATE],40,0,bit_len_union_t::_0bit,16,SIM_Q_ECEF2B_note_t,sizeof(SIM_Q_ECEF2B_note_t));
	ADCS_BIT[SIM_B_RATES].init_bit(&adcs_reg[SIM_SAT_STATE],56,0,bit_len_union_t::_0bit,12,SIM_B_RATES_note_t,sizeof(SIM_B_RATES_note_t));
	ADCS_BIT[SIM_P_ECEF].init_bit(&adcs_reg[SIM_SAT_STATE],68,0,bit_len_union_t::_0bit,12,SIM_P_ECEF_note_t,sizeof(SIM_P_ECEF_note_t));
	ADCS_BIT[SIM_V_ECEF].init_bit(&adcs_reg[SIM_SAT_STATE],80,0,bit_len_union_t::_0bit,12,SIM_V_ECEF_note_t,sizeof(SIM_V_ECEF_note_t));
	ADCS_BIT[SIM_PID_CONS].init_bit(&adcs_reg[SIM_SAT_STATE],92,0,bit_len_union_t::_0bit,12,SIM_PID_CONS_note_t,sizeof(SIM_PID_CONS_note_t));
	ADCS_BIT[SIM_RWA_RPM].init_bit(&adcs_reg[SIM_OUT_STATE],0,0,bit_len_union_t::_0bit,8,SIM_RWA_RPM_note_t,sizeof(SIM_RWA_RPM_note_t));
	ADCS_BIT[SIM_MTQ_CURRENT].init_bit(&adcs_reg[SIM_OUT_STATE],8,0,bit_len_union_t::_0bit,6,SIM_MTQ_CURRENT_note_t,sizeof(SIM_MTQ_CURRENT_note_t));
	ADCS_BIT[SIM_Q_ECI2B].init_bit(&adcs_reg[SIM_OUT_STATE],14,0,bit_len_union_t::_0bit,16,SIM_Q_ECI2B_note_t,sizeof(SIM_Q_ECI2B_note_t));
	ADCS_BIT[SIM_T_CMD].init_bit(&adcs_reg[SIM_OUT_STATE],30,0,bit_len_union_t::_0bit,12,SIM_T_CMD_note_t,sizeof(SIM_T_CMD_note_t));
	ADCS_BIT[SIM_Q_ERR].init_bit(&adcs_reg[SIM_OUT_STATE],42,0,bit_len_union_t::_0bit,16,SIM_Q_ERR_note_t,sizeof(SIM_Q_ERR_note_t));

}
void ADCS_BIT_link_get(){
	ADCS_BIT[SIM_INERTIA_SAT].get_data(adcs_reg[FIX_INERTIA_SAT].get_header());
	ADCS_BIT[SIM_ACS_MODE].get_data(adcs_reg[CMD_ACS_MODE].get_header());
	ADCS_BIT[SIM_ADS_MODE].get_data(adcs_reg[CMD_ADS_MODE].get_header());
	ADCS_BIT[SIM_MODE_DIF_MS].get_data(adcs_reg[CMD_MODE_DIF_MS].get_header());
	ADCS_BIT[SIM_MAG1_NT].get_data(adcs_reg[SEN_MAG1_NT].get_header());
	ADCS_BIT[SIM_GYRO1_DEG].get_data(adcs_reg[SEN_GYRO1_DEG].get_header());
	ADCS_BIT[SIM_SUN_VEC].get_data(adcs_reg[SEN_SUN_VEC].get_header());
	ADCS_BIT[SIM_Q_ECEF2B].get_data(adcs_reg[EST_Q_ECEF2B].get_header());
	ADCS_BIT[SIM_B_RATES].get_data(adcs_reg[EST_B_RATES].get_header());
	ADCS_BIT[SIM_P_ECEF].get_data(adcs_reg[EST_P_ECEF].get_header());
	ADCS_BIT[SIM_V_ECEF].get_data(adcs_reg[EST_V_ECEF].get_header());
	ADCS_BIT[SIM_PID_CONS].get_data(adcs_reg[CMD_PID_CONS].get_header());

}
void ADCS_BIT_link_set(){
	ADCS_BIT[SIM_RWA_RPM].update_data(adcs_reg[SEN_RWA_RPM].get_header());
	ADCS_BIT[SIM_MTQ_CURRENT].update_data(adcs_reg[SEN_MTQ_CURRENT].get_header());
	ADCS_BIT[SIM_Q_ECI2B].update_data(adcs_reg[EST_Q_ECI2B].get_header());
	ADCS_BIT[SIM_T_CMD].update_data(adcs_reg[EST_T_CMD].get_header());
	ADCS_BIT[SIM_Q_ERR].update_data(adcs_reg[EST_Q_ERR].get_header());

}
#endif // _ADCS_BIT_H
