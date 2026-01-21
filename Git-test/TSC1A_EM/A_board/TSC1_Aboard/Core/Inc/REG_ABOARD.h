#ifndef _REG_ABOARD_H
#define _REG_ABOARD_H
#define MAX_REG_ABOARD 87
#include <stdint.h>
#include "DEV_TYPE_ABOARD.h"
#include "REG_MAP/KMUTNB_ADCS_REG.h"
KMUTNB_ADCS_REG REG_ABOARD[MAX_REG_ABOARD];

enum class reg_REG_ABOARD: uint16_t
{
    sen_nothing = 0,
    TLM_COUNTER = 1,
    ERR_FLG_MAIN = 2,
    mode_acs = 3,
    mode_ads = 4,
    mode_mtq1 = 5,
    mode_rwa1 = 6,
    mode_uart = 7,
    mode_run = 8,
    SEN_RESET_ROUND = 9,
    cmd_mtq1 = 10,
    cmd_mtq2 = 11,
    cmd_mtq3 = 12,
    cmd_rwa1 = 13,
    cmd_rwa2 = 14,
    cmd_rwa3 = 15,
    cmd_rwa4 = 16,
    sen_acs = 17,
    sen_ads = 18,
    sen_gps_data = 19,
    sen_gyro1 = 20,
    sen_mag1 = 21,
    sen_mag2 = 22,
    sen_model_igrf = 23,
    sen_model_posvel_ecef = 24,
    sen_model_sun = 25,
    sen_mtq1 = 26,
    sen_mtq2 = 27,
    sen_mtq3 = 28,
    sen_obc_current = 29,
    sen_obc_imu1 = 30,
    sen_obc_imu2 = 31,
    sen_obc_mag1 = 32,
    sen_obc_mag2 = 33,
    sen_obc_temp1 = 34,
    sen_run = 35,
    sen_rwa1 = 36,
    sen_rwa2 = 37,
    sen_rwa3 = 38,
    sen_rwa4 = 39,
    sen_sat_inertia = 40,
    sen_star_pu = 41,
    sen_sun1 = 42,
    sen_sun2 = 43,
    sen_sun3 = 44,
    sen_sun4 = 45,
    sen_uart = 46,
    sen_unix_time = 47,
    EST_Q_ECEF2B = 48,
    EST_B_RATES = 49,
    SEN_NADIR_VEC = 50,
    SEN_B_RATES = 51,
    EST_ATT_NED = 52,
    EST_Q_ORF2B = 53,
    EST_Q_ERR = 54,
    EST_T_CMD = 55,
    EST_Q_ECI2B = 56,
    FIX_MEAN_ANAMOLY = 57,
    FIX_MEAN_MOTION = 58,
    FIX_ARG_PERIGEE = 59,
    FIX_RAAN = 60,
    FIX_ECCENTRICITY = 61,
    FIX_INCLINATION = 62,
    mode_acs_delay_ms = 78,
    mode_ads_delay_ms = 79,
    mode_mtq1_delay_ms = 80,
    mode_run_delay_ms = 81,
    mode_rwa1_delay_ms = 82,
    mode_rwa2_delay_ms = 83,
    mode_rwa3_delay_ms = 84,
    mode_rwa4_delay_ms = 85,
    mode_uart_delay_ms = 86,
    angle_mag1 = 63,
    angle_mag2 = 64,
    angle_mtq1 = 65,
    angle_mtq2 = 66,
    angle_mtq3 = 67,
    angle_obc_imu1 = 68,
    angle_obc_imu2 = 69,
    angle_rwa1 = 70,
    angle_rwa2 = 71,
    angle_rwa3 = 72,
    angle_rwa4 = 73,
    angle_sun1 = 74,
    angle_sun2 = 75,
    angle_sun3 = 76,
    angle_sun4 = 77,
};

// Function to set up KM registers
void reg_REG_ABOARD_setup()
{
    REG_ABOARD[(int)reg_REG_ABOARD::sen_nothing].init((uint8_t*)&ram_data.sen_nothing, sizeof(uint8_t) * 1);
    REG_ABOARD[(int)reg_REG_ABOARD::TLM_COUNTER].init((uint8_t*)&ram_data.TLM_COUNTER, sizeof(uint32_t) * 1);
    REG_ABOARD[(int)reg_REG_ABOARD::ERR_FLG_MAIN].init((uint8_t*)&ram_data.ERR_FLG_MAIN, sizeof(uint8_t) * 1);
    REG_ABOARD[(int)reg_REG_ABOARD::mode_acs].init((uint8_t*)&ram_data.mode_acs, sizeof(uint8_t) * 1);
    REG_ABOARD[(int)reg_REG_ABOARD::mode_ads].init((uint8_t*)&ram_data.mode_ads, sizeof(uint8_t) * 1);
    REG_ABOARD[(int)reg_REG_ABOARD::mode_mtq1].init((uint8_t*)&ram_data.mode_mtq1, sizeof(uint8_t) * 1);
    REG_ABOARD[(int)reg_REG_ABOARD::mode_rwa1].init((uint8_t*)&ram_data.mode_rwa1, sizeof(uint8_t) * 1);
    REG_ABOARD[(int)reg_REG_ABOARD::mode_uart].init((uint8_t*)&ram_data.mode_uart, sizeof(uint8_t) * 1);
    REG_ABOARD[(int)reg_REG_ABOARD::mode_run].init((uint8_t*)&ram_data.mode_run, sizeof(uint8_t) * 1);
    REG_ABOARD[(int)reg_REG_ABOARD::SEN_RESET_ROUND].init((uint8_t*)&ram_data.SEN_RESET_ROUND, sizeof(uint32_t) * 1);
    REG_ABOARD[(int)reg_REG_ABOARD::cmd_mtq1].init((uint8_t*)&ram_data.cmd_mtq1, sizeof(int16_t) * 1);
    REG_ABOARD[(int)reg_REG_ABOARD::cmd_mtq2].init((uint8_t*)&ram_data.cmd_mtq2, sizeof(int16_t) * 1);
    REG_ABOARD[(int)reg_REG_ABOARD::cmd_mtq3].init((uint8_t*)&ram_data.cmd_mtq3, sizeof(int16_t) * 1);
    REG_ABOARD[(int)reg_REG_ABOARD::cmd_rwa1].init((uint8_t*)&ram_data.cmd_rwa1, sizeof(int16_t) * 1);
    REG_ABOARD[(int)reg_REG_ABOARD::cmd_rwa2].init((uint8_t*)&ram_data.cmd_rwa2, sizeof(int16_t) * 1);
    REG_ABOARD[(int)reg_REG_ABOARD::cmd_rwa3].init((uint8_t*)&ram_data.cmd_rwa3, sizeof(int16_t) * 1);
    REG_ABOARD[(int)reg_REG_ABOARD::cmd_rwa4].init((uint8_t*)&ram_data.cmd_rwa4, sizeof(int16_t) * 1);
    REG_ABOARD[(int)reg_REG_ABOARD::sen_acs].init((uint8_t*)&ram_data.sen_acs, sizeof(uint8_t) * 1);
    REG_ABOARD[(int)reg_REG_ABOARD::sen_ads].init((uint8_t*)&ram_data.sen_ads, sizeof(uint8_t) * 1);
    REG_ABOARD[(int)reg_REG_ABOARD::sen_gps_data].init((uint8_t*)&ram_data.sen_gps_data[0], sizeof(int16_t) * 6);
    REG_ABOARD[(int)reg_REG_ABOARD::sen_gyro1].init((uint8_t*)&ram_data.sen_gyro1[0], sizeof(int16_t) * 3);
    REG_ABOARD[(int)reg_REG_ABOARD::sen_mag1].init((uint8_t*)&ram_data.sen_mag1[0], sizeof(int16_t) * 3);
    REG_ABOARD[(int)reg_REG_ABOARD::sen_mag2].init((uint8_t*)&ram_data.sen_mag2[0], sizeof(int16_t) * 3);
    REG_ABOARD[(int)reg_REG_ABOARD::sen_model_igrf].init((uint8_t*)&ram_data.sen_model_igrf[0], sizeof(int16_t) * 3);
    REG_ABOARD[(int)reg_REG_ABOARD::sen_model_posvel_ecef].init((uint8_t*)&ram_data.sen_model_posvel_ecef[0], sizeof(int16_t) * 6);
    REG_ABOARD[(int)reg_REG_ABOARD::sen_model_sun].init((uint8_t*)&ram_data.sen_model_sun[0], sizeof(int16_t) * 3);
    REG_ABOARD[(int)reg_REG_ABOARD::sen_mtq1].init((uint8_t*)&ram_data.sen_mtq1, sizeof(int16_t) * 1);
    REG_ABOARD[(int)reg_REG_ABOARD::sen_mtq2].init((uint8_t*)&ram_data.sen_mtq2, sizeof(int16_t) * 1);
    REG_ABOARD[(int)reg_REG_ABOARD::sen_mtq3].init((uint8_t*)&ram_data.sen_mtq3, sizeof(int16_t) * 1);
    REG_ABOARD[(int)reg_REG_ABOARD::sen_obc_current].init((uint8_t*)&ram_data.sen_obc_current, sizeof(int16_t) * 1);
    REG_ABOARD[(int)reg_REG_ABOARD::sen_obc_imu1].init((uint8_t*)&ram_data.sen_obc_imu1[0], sizeof(int16_t) * 3);
    REG_ABOARD[(int)reg_REG_ABOARD::sen_obc_imu2].init((uint8_t*)&ram_data.sen_obc_imu2[0], sizeof(int16_t) * 3);
    REG_ABOARD[(int)reg_REG_ABOARD::sen_obc_mag1].init((uint8_t*)&ram_data.sen_obc_mag1[0], sizeof(int16_t) * 3);
    REG_ABOARD[(int)reg_REG_ABOARD::sen_obc_mag2].init((uint8_t*)&ram_data.sen_obc_mag2[0], sizeof(int16_t) * 3);
    REG_ABOARD[(int)reg_REG_ABOARD::sen_obc_temp1].init((uint8_t*)&ram_data.sen_obc_temp1, sizeof(int16_t) * 1);
    REG_ABOARD[(int)reg_REG_ABOARD::sen_run].init((uint8_t*)&ram_data.sen_run, sizeof(uint8_t) * 1);
    REG_ABOARD[(int)reg_REG_ABOARD::sen_rwa1].init((uint8_t*)&ram_data.sen_rwa1, sizeof(int16_t) * 1);
    REG_ABOARD[(int)reg_REG_ABOARD::sen_rwa2].init((uint8_t*)&ram_data.sen_rwa2, sizeof(int16_t) * 1);
    REG_ABOARD[(int)reg_REG_ABOARD::sen_rwa3].init((uint8_t*)&ram_data.sen_rwa3, sizeof(int16_t) * 1);
    REG_ABOARD[(int)reg_REG_ABOARD::sen_rwa4].init((uint8_t*)&ram_data.sen_rwa4, sizeof(int16_t) * 1);
    REG_ABOARD[(int)reg_REG_ABOARD::sen_sat_inertia].init((uint8_t*)&rom_data.sen_sat_inertia[0], sizeof(float) * 6);
    REG_ABOARD[(int)reg_REG_ABOARD::sen_star_pu].init((uint8_t*)&ram_data.sen_star_pu[0], sizeof(int16_t) * 3);
    REG_ABOARD[(int)reg_REG_ABOARD::sen_sun1].init((uint8_t*)&ram_data.sen_sun1[0], sizeof(int16_t) * 3);
    REG_ABOARD[(int)reg_REG_ABOARD::sen_sun2].init((uint8_t*)&ram_data.sen_sun2[0], sizeof(int16_t) * 3);
    REG_ABOARD[(int)reg_REG_ABOARD::sen_sun3].init((uint8_t*)&ram_data.sen_sun3[0], sizeof(int16_t) * 3);
    REG_ABOARD[(int)reg_REG_ABOARD::sen_sun4].init((uint8_t*)&ram_data.sen_sun4[0], sizeof(int16_t) * 3);
    REG_ABOARD[(int)reg_REG_ABOARD::sen_uart].init((uint8_t*)&ram_data.sen_uart, sizeof(uint8_t) * 1);
    REG_ABOARD[(int)reg_REG_ABOARD::sen_unix_time].init((uint8_t*)&rom_data.sen_unix_time, sizeof(uint32_t) * 1);
    REG_ABOARD[(int)reg_REG_ABOARD::EST_Q_ECEF2B].init((uint8_t*)&ram_data.EST_Q_ECEF2B[0], sizeof(float) * 4);
    REG_ABOARD[(int)reg_REG_ABOARD::EST_B_RATES].init((uint8_t*)&ram_data.EST_B_RATES[0], sizeof(float) * 3);
    REG_ABOARD[(int)reg_REG_ABOARD::SEN_NADIR_VEC].init((uint8_t*)&ram_data.SEN_NADIR_VEC[0], sizeof(float) * 3);
    REG_ABOARD[(int)reg_REG_ABOARD::SEN_B_RATES].init((uint8_t*)&ram_data.SEN_B_RATES[0], sizeof(float) * 3);
    REG_ABOARD[(int)reg_REG_ABOARD::EST_ATT_NED].init((uint8_t*)&ram_data.EST_ATT_NED[0], sizeof(float) * 3);
    REG_ABOARD[(int)reg_REG_ABOARD::EST_Q_ORF2B].init((uint8_t*)&ram_data.EST_Q_ORF2B[0], sizeof(float) * 5);
    REG_ABOARD[(int)reg_REG_ABOARD::EST_Q_ERR].init((uint8_t*)&ram_data.EST_Q_ERR[0], sizeof(float) * 4);
    REG_ABOARD[(int)reg_REG_ABOARD::EST_T_CMD].init((uint8_t*)&ram_data.EST_T_CMD[0], sizeof(float) * 3);
    REG_ABOARD[(int)reg_REG_ABOARD::EST_Q_ECI2B].init((uint8_t*)&ram_data.EST_Q_ECI2B[0], sizeof(float) * 4);
    REG_ABOARD[(int)reg_REG_ABOARD::FIX_MEAN_ANAMOLY].init((uint8_t*)&rom_data.FIX_MEAN_ANAMOLY, sizeof(float) * 1);
    REG_ABOARD[(int)reg_REG_ABOARD::FIX_MEAN_MOTION].init((uint8_t*)&rom_data.FIX_MEAN_MOTION, sizeof(float) * 1);
    REG_ABOARD[(int)reg_REG_ABOARD::FIX_ARG_PERIGEE].init((uint8_t*)&rom_data.FIX_ARG_PERIGEE, sizeof(float) * 1);
    REG_ABOARD[(int)reg_REG_ABOARD::FIX_RAAN].init((uint8_t*)&rom_data.FIX_RAAN, sizeof(float) * 1);
    REG_ABOARD[(int)reg_REG_ABOARD::FIX_ECCENTRICITY].init((uint8_t*)&rom_data.FIX_ECCENTRICITY, sizeof(float) * 1);
    REG_ABOARD[(int)reg_REG_ABOARD::FIX_INCLINATION].init((uint8_t*)&rom_data.FIX_INCLINATION, sizeof(float) * 1);
    REG_ABOARD[(int)reg_REG_ABOARD::mode_acs_delay_ms].init((uint8_t*)&ram_data.mode_acs_delay_ms, sizeof(uint32_t) * 1);
    REG_ABOARD[(int)reg_REG_ABOARD::mode_ads_delay_ms].init((uint8_t*)&ram_data.mode_ads_delay_ms, sizeof(uint32_t) * 1);
    REG_ABOARD[(int)reg_REG_ABOARD::mode_mtq1_delay_ms].init((uint8_t*)&ram_data.mode_mtq1_delay_ms, sizeof(uint32_t) * 1);
    REG_ABOARD[(int)reg_REG_ABOARD::mode_run_delay_ms].init((uint8_t*)&ram_data.mode_run_delay_ms, sizeof(uint32_t) * 1);
    REG_ABOARD[(int)reg_REG_ABOARD::mode_rwa1_delay_ms].init((uint8_t*)&ram_data.mode_rwa1_delay_ms, sizeof(uint32_t) * 1);
    REG_ABOARD[(int)reg_REG_ABOARD::mode_rwa2_delay_ms].init((uint8_t*)&ram_data.mode_rwa2_delay_ms, sizeof(uint32_t) * 1);
    REG_ABOARD[(int)reg_REG_ABOARD::mode_rwa3_delay_ms].init((uint8_t*)&ram_data.mode_rwa3_delay_ms, sizeof(uint32_t) * 1);
    REG_ABOARD[(int)reg_REG_ABOARD::mode_rwa4_delay_ms].init((uint8_t*)&ram_data.mode_rwa4_delay_ms, sizeof(uint32_t) * 1);
    REG_ABOARD[(int)reg_REG_ABOARD::mode_uart_delay_ms].init((uint8_t*)&ram_data.mode_uart_delay_ms, sizeof(uint32_t) * 1);
    REG_ABOARD[(int)reg_REG_ABOARD::angle_mag1].init((uint8_t*)&rom_data.angle_mag1, sizeof(angle_array_t<1>) * 1);
    REG_ABOARD[(int)reg_REG_ABOARD::angle_mag2].init((uint8_t*)&rom_data.angle_mag2, sizeof(angle_array_t<1>) * 1);
    REG_ABOARD[(int)reg_REG_ABOARD::angle_mtq1].init((uint8_t*)&rom_data.angle_mtq1, sizeof(angle_array_t<1>) * 1);
    REG_ABOARD[(int)reg_REG_ABOARD::angle_mtq2].init((uint8_t*)&rom_data.angle_mtq2, sizeof(angle_array_t<1>) * 1);
    REG_ABOARD[(int)reg_REG_ABOARD::angle_mtq3].init((uint8_t*)&rom_data.angle_mtq3, sizeof(angle_array_t<1>) * 1);
    REG_ABOARD[(int)reg_REG_ABOARD::angle_obc_imu1].init((uint8_t*)&rom_data.angle_obc_imu1, sizeof(angle_array_t<1>) * 1);
    REG_ABOARD[(int)reg_REG_ABOARD::angle_obc_imu2].init((uint8_t*)&rom_data.angle_obc_imu2, sizeof(angle_array_t<1>) * 1);
    REG_ABOARD[(int)reg_REG_ABOARD::angle_rwa1].init((uint8_t*)&rom_data.angle_rwa1, sizeof(angle_array_t<1>) * 1);
    REG_ABOARD[(int)reg_REG_ABOARD::angle_rwa2].init((uint8_t*)&rom_data.angle_rwa2, sizeof(angle_array_t<1>) * 1);
    REG_ABOARD[(int)reg_REG_ABOARD::angle_rwa3].init((uint8_t*)&rom_data.angle_rwa3, sizeof(angle_array_t<1>) * 1);
    REG_ABOARD[(int)reg_REG_ABOARD::angle_rwa4].init((uint8_t*)&rom_data.angle_rwa4, sizeof(angle_array_t<1>) * 1);
    REG_ABOARD[(int)reg_REG_ABOARD::angle_sun1].init((uint8_t*)&rom_data.angle_sun1, sizeof(angle_array_t<1>) * 1);
    REG_ABOARD[(int)reg_REG_ABOARD::angle_sun2].init((uint8_t*)&rom_data.angle_sun2, sizeof(angle_array_t<1>) * 1);
    REG_ABOARD[(int)reg_REG_ABOARD::angle_sun3].init((uint8_t*)&rom_data.angle_sun3, sizeof(angle_array_t<1>) * 1);
    REG_ABOARD[(int)reg_REG_ABOARD::angle_sun4].init((uint8_t*)&rom_data.angle_sun4, sizeof(angle_array_t<1>) * 1);
}

#endif // _REG_ABOARD_H
