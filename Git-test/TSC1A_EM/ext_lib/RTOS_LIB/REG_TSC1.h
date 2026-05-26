#ifndef _REG_TSC1_H
#define _REG_TSC1_H
#define MAX_REG_TSC1 112
#include <stdint.h>
#include "DEV_TYPE_TSC1.h"
#include "REG_MAP/KMUTNB_ADCS_REG.h"
KMUTNB_ADCS_REG REG_TSC1[MAX_REG_TSC1];

enum class reg_REG_TSC1: uint16_t
{
    angle_cube_mag = 0,
    angle_cube_mtq1 = 1,
    angle_cube_mtq2 = 2,
    angle_cube_mtq3 = 3,
    angle_cube_nadir = 4,
    angle_cube_rwa1 = 5,
    angle_cube_rwa2 = 6,
    angle_cube_rwa3 = 7,
    angle_cube_sun = 8,
    angle_obc_imu1 = 9,
    angle_obc_imu2 = 10,
    angle_obc_mtq1 = 11,
    angle_obc_mtq2 = 12,
    angle_obc_mtq3 = 13,
    angle_obc_rwa1 = 14,
    angle_obc_rwa2 = 15,
    angle_obc_rwa3 = 16,
    angle_obc_rwa4 = 17,
    mode_acs = 18,
    mode_acs_delay_ms = 19,
    mode_ads = 20,
    mode_ads_delay_ms = 21,
    mode_can = 22,
    mode_can_delay_ms = 23,
    mode_cube_acs = 24,
    mode_cube_acs_delay_ms = 25,
    mode_cube_ads = 26,
    mode_cube_ads_delay_ms = 27,
    mode_fss = 28,
    mode_fss_delay_ms = 29,
    mode_gps = 30,
    mode_gps_delay_ms = 31,
    mode_i2c = 32,
    mode_i2c_delay_ms = 33,
    mode_imu = 34,
    mode_imu_delay_ms = 35,
    mode_mem = 36,
    mode_mem_delay_ms = 37,
    mode_mtm = 38,
    mode_mtm_delay_ms = 39,
    mode_mtq = 40,
    mode_mtq_delay_ms = 41,
    mode_obc_mtq1 = 42,
    mode_obc_mtq1_delay_ms = 43,
    mode_obc_mtq2 = 44,
    mode_obc_mtq2_delay_ms = 45,
    mode_obc_mtq3 = 46,
    mode_obc_mtq3_delay_ms = 47,
    mode_obc_rwa1 = 48,
    mode_obc_rwa1_delay_ms = 49,
    mode_obc_rwa2 = 50,
    mode_obc_rwa2_delay_ms = 51,
    mode_obc_rwa3 = 52,
    mode_obc_rwa3_delay_ms = 53,
    mode_obc_rwa4 = 54,
    mode_obc_rwa4_delay_ms = 55,
    mode_run = 56,
    mode_run_delay_ms = 57,
    mode_rwa = 58,
    mode_rwa_delay_ms = 59,
    mode_slave = 60,
    mode_slave_delay_ms = 61,
    mode_stt = 62,
    mode_stt_delay_ms = 63,
    mode_uart = 64,
    mode_uart_delay_ms = 65,
    sen_acs = 66,
    sen_ads = 67,
    sen_can = 68,
    sen_cube_acs = 69,
    sen_cube_ads = 70,
    sen_cube_est_quat = 71,
    sen_cube_est_rate = 72,
    sen_cube_gyro = 73,
    sen_cube_mag = 74,
    sen_cube_mtq1 = 75,
    sen_cube_mtq2 = 76,
    sen_cube_mtq3 = 77,
    sen_cube_nadir = 78,
    sen_cube_posvel_ecef = 79,
    sen_cube_rwa1 = 80,
    sen_cube_rwa2 = 81,
    sen_cube_rwa3 = 82,
    sen_cube_sun = 83,
    sen_i2c = 84,
    sen_mem = 85,
    sen_model_igrf = 86,
    sen_model_posvel_ecef = 87,
    sen_model_sun = 88,
    sen_mtq = 89,
    sen_nothing = 90,
    sen_obc_current = 91,
    sen_obc_imu1 = 92,
    sen_obc_imu2 = 93,
    sen_obc_mag1 = 94,
    sen_obc_mag2 = 95,
    sen_obc_mtq1 = 96,
    sen_obc_mtq2 = 97,
    sen_obc_mtq3 = 98,
    sen_obc_rwa1 = 99,
    sen_obc_rwa2 = 100,
    sen_obc_rwa3 = 101,
    sen_obc_rwa4 = 102,
    sen_obc_temp1 = 103,
    sen_run = 104,
    sen_rwa = 105,
    sen_sat_inertia = 106,
    sen_slave = 107,
    sen_uart = 108,
    sen_unix_time = 109,
	ping_stat = 110,
	ls_stat = 111,
};

// Function to set up KM registers
void reg_REG_TSC1_setup()
{
    REG_TSC1[(int)reg_REG_TSC1::angle_cube_mag].init((uint8_t*)&rom_data.angle_cube_mag, sizeof(angle_array_t<1>) * 1);
    REG_TSC1[(int)reg_REG_TSC1::angle_cube_mtq1].init((uint8_t*)&rom_data.angle_cube_mtq1, sizeof(angle_array_t<1>) * 1);
    REG_TSC1[(int)reg_REG_TSC1::angle_cube_mtq2].init((uint8_t*)&rom_data.angle_cube_mtq2, sizeof(angle_array_t<1>) * 1);
    REG_TSC1[(int)reg_REG_TSC1::angle_cube_mtq3].init((uint8_t*)&rom_data.angle_cube_mtq3, sizeof(angle_array_t<1>) * 1);
    REG_TSC1[(int)reg_REG_TSC1::angle_cube_nadir].init((uint8_t*)&rom_data.angle_cube_nadir, sizeof(angle_array_t<1>) * 1);
    REG_TSC1[(int)reg_REG_TSC1::angle_cube_rwa1].init((uint8_t*)&rom_data.angle_cube_rwa1, sizeof(angle_array_t<1>) * 1);
    REG_TSC1[(int)reg_REG_TSC1::angle_cube_rwa2].init((uint8_t*)&rom_data.angle_cube_rwa2, sizeof(angle_array_t<1>) * 1);
    REG_TSC1[(int)reg_REG_TSC1::angle_cube_rwa3].init((uint8_t*)&rom_data.angle_cube_rwa3, sizeof(angle_array_t<1>) * 1);
    REG_TSC1[(int)reg_REG_TSC1::angle_cube_sun].init((uint8_t*)&rom_data.angle_cube_sun, sizeof(angle_array_t<1>) * 1);
    REG_TSC1[(int)reg_REG_TSC1::angle_obc_imu1].init((uint8_t*)&rom_data.angle_obc_imu1, sizeof(angle_array_t<1>) * 1);
    REG_TSC1[(int)reg_REG_TSC1::angle_obc_imu2].init((uint8_t*)&rom_data.angle_obc_imu2, sizeof(angle_array_t<1>) * 1);
    REG_TSC1[(int)reg_REG_TSC1::angle_obc_mtq1].init((uint8_t*)&rom_data.angle_obc_mtq1, sizeof(angle_array_t<1>) * 1);
    REG_TSC1[(int)reg_REG_TSC1::angle_obc_mtq2].init((uint8_t*)&rom_data.angle_obc_mtq2, sizeof(angle_array_t<1>) * 1);
    REG_TSC1[(int)reg_REG_TSC1::angle_obc_mtq3].init((uint8_t*)&rom_data.angle_obc_mtq3, sizeof(angle_array_t<1>) * 1);
    REG_TSC1[(int)reg_REG_TSC1::angle_obc_rwa1].init((uint8_t*)&rom_data.angle_obc_rwa1, sizeof(angle_array_t<1>) * 1);
    REG_TSC1[(int)reg_REG_TSC1::angle_obc_rwa2].init((uint8_t*)&rom_data.angle_obc_rwa2, sizeof(angle_array_t<1>) * 1);
    REG_TSC1[(int)reg_REG_TSC1::angle_obc_rwa3].init((uint8_t*)&rom_data.angle_obc_rwa3, sizeof(angle_array_t<1>) * 1);
    REG_TSC1[(int)reg_REG_TSC1::angle_obc_rwa4].init((uint8_t*)&rom_data.angle_obc_rwa4, sizeof(angle_array_t<1>) * 1);
    REG_TSC1[(int)reg_REG_TSC1::mode_acs].init((uint8_t*)&ram_data.mode_acs, sizeof(uint8_t) * 1);
    REG_TSC1[(int)reg_REG_TSC1::mode_acs_delay_ms].init((uint8_t*)&ram_data.mode_acs_delay_ms, sizeof(uint32_t) * 1);
    REG_TSC1[(int)reg_REG_TSC1::mode_ads].init((uint8_t*)&ram_data.mode_ads, sizeof(uint8_t) * 1);
    REG_TSC1[(int)reg_REG_TSC1::mode_ads_delay_ms].init((uint8_t*)&ram_data.mode_ads_delay_ms, sizeof(uint32_t) * 1);
    REG_TSC1[(int)reg_REG_TSC1::mode_can].init((uint8_t*)&ram_data.mode_can, sizeof(uint8_t) * 1);
    REG_TSC1[(int)reg_REG_TSC1::mode_can_delay_ms].init((uint8_t*)&ram_data.mode_can_delay_ms, sizeof(uint32_t) * 1);
    REG_TSC1[(int)reg_REG_TSC1::mode_cube_acs].init((uint8_t*)&ram_data.mode_cube_acs, sizeof(uint8_t) * 1);
    REG_TSC1[(int)reg_REG_TSC1::mode_cube_acs_delay_ms].init((uint8_t*)&ram_data.mode_cube_acs_delay_ms, sizeof(uint32_t) * 1);
    REG_TSC1[(int)reg_REG_TSC1::mode_cube_ads].init((uint8_t*)&ram_data.mode_cube_ads, sizeof(uint8_t) * 1);
    REG_TSC1[(int)reg_REG_TSC1::mode_cube_ads_delay_ms].init((uint8_t*)&ram_data.mode_cube_ads_delay_ms, sizeof(uint32_t) * 1);
    REG_TSC1[(int)reg_REG_TSC1::mode_fss].init((uint8_t*)&ram_data.mode_fss, sizeof(uint8_t) * 1);
    REG_TSC1[(int)reg_REG_TSC1::mode_fss_delay_ms].init((uint8_t*)&ram_data.mode_fss_delay_ms, sizeof(uint32_t) * 1);
    REG_TSC1[(int)reg_REG_TSC1::mode_gps].init((uint8_t*)&ram_data.mode_gps, sizeof(uint8_t) * 1);
    REG_TSC1[(int)reg_REG_TSC1::mode_gps_delay_ms].init((uint8_t*)&ram_data.mode_gps_delay_ms, sizeof(uint32_t) * 1);
    REG_TSC1[(int)reg_REG_TSC1::mode_i2c].init((uint8_t*)&ram_data.mode_i2c, sizeof(uint8_t) * 1);
    REG_TSC1[(int)reg_REG_TSC1::mode_i2c_delay_ms].init((uint8_t*)&ram_data.mode_i2c_delay_ms, sizeof(uint32_t) * 1);
    REG_TSC1[(int)reg_REG_TSC1::mode_imu].init((uint8_t*)&ram_data.mode_imu, sizeof(uint8_t) * 1);
    REG_TSC1[(int)reg_REG_TSC1::mode_imu_delay_ms].init((uint8_t*)&ram_data.mode_imu_delay_ms, sizeof(uint32_t) * 1);
    REG_TSC1[(int)reg_REG_TSC1::mode_mem].init((uint8_t*)&ram_data.mode_mem, sizeof(uint8_t) * 1);
    REG_TSC1[(int)reg_REG_TSC1::mode_mem_delay_ms].init((uint8_t*)&ram_data.mode_mem_delay_ms, sizeof(uint32_t) * 1);
    REG_TSC1[(int)reg_REG_TSC1::mode_mtm].init((uint8_t*)&ram_data.mode_mtm, sizeof(uint8_t) * 1);
    REG_TSC1[(int)reg_REG_TSC1::mode_mtm_delay_ms].init((uint8_t*)&ram_data.mode_mtm_delay_ms, sizeof(uint32_t) * 1);
    REG_TSC1[(int)reg_REG_TSC1::mode_mtq].init((uint8_t*)&ram_data.mode_mtq, sizeof(uint8_t) * 1);
    REG_TSC1[(int)reg_REG_TSC1::mode_mtq_delay_ms].init((uint8_t*)&ram_data.mode_mtq_delay_ms, sizeof(uint32_t) * 1);
    REG_TSC1[(int)reg_REG_TSC1::mode_obc_mtq1].init((uint8_t*)&ram_data.mode_obc_mtq1, sizeof(uint8_t) * 1);
    REG_TSC1[(int)reg_REG_TSC1::mode_obc_mtq1_delay_ms].init((uint8_t*)&ram_data.mode_obc_mtq1_delay_ms, sizeof(uint32_t) * 1);
    REG_TSC1[(int)reg_REG_TSC1::mode_obc_mtq2].init((uint8_t*)&ram_data.mode_obc_mtq2, sizeof(uint8_t) * 1);
    REG_TSC1[(int)reg_REG_TSC1::mode_obc_mtq2_delay_ms].init((uint8_t*)&ram_data.mode_obc_mtq2_delay_ms, sizeof(uint32_t) * 1);
    REG_TSC1[(int)reg_REG_TSC1::mode_obc_mtq3].init((uint8_t*)&ram_data.mode_obc_mtq3, sizeof(uint8_t) * 1);
    REG_TSC1[(int)reg_REG_TSC1::mode_obc_mtq3_delay_ms].init((uint8_t*)&ram_data.mode_obc_mtq3_delay_ms, sizeof(uint32_t) * 1);
    REG_TSC1[(int)reg_REG_TSC1::mode_obc_rwa1].init((uint8_t*)&ram_data.mode_obc_rwa1, sizeof(uint8_t) * 1);
    REG_TSC1[(int)reg_REG_TSC1::mode_obc_rwa1_delay_ms].init((uint8_t*)&ram_data.mode_obc_rwa1_delay_ms, sizeof(uint32_t) * 1);
    REG_TSC1[(int)reg_REG_TSC1::mode_obc_rwa2].init((uint8_t*)&ram_data.mode_obc_rwa2, sizeof(uint8_t) * 1);
    REG_TSC1[(int)reg_REG_TSC1::mode_obc_rwa2_delay_ms].init((uint8_t*)&ram_data.mode_obc_rwa2_delay_ms, sizeof(uint32_t) * 1);
    REG_TSC1[(int)reg_REG_TSC1::mode_obc_rwa3].init((uint8_t*)&ram_data.mode_obc_rwa3, sizeof(uint8_t) * 1);
    REG_TSC1[(int)reg_REG_TSC1::mode_obc_rwa3_delay_ms].init((uint8_t*)&ram_data.mode_obc_rwa3_delay_ms, sizeof(uint32_t) * 1);
    REG_TSC1[(int)reg_REG_TSC1::mode_obc_rwa4].init((uint8_t*)&ram_data.mode_obc_rwa4, sizeof(uint8_t) * 1);
    REG_TSC1[(int)reg_REG_TSC1::mode_obc_rwa4_delay_ms].init((uint8_t*)&ram_data.mode_obc_rwa4_delay_ms, sizeof(uint32_t) * 1);
    REG_TSC1[(int)reg_REG_TSC1::mode_run].init((uint8_t*)&ram_data.mode_run, sizeof(uint8_t) * 1);
    REG_TSC1[(int)reg_REG_TSC1::mode_run_delay_ms].init((uint8_t*)&ram_data.mode_run_delay_ms, sizeof(uint32_t) * 1);
    REG_TSC1[(int)reg_REG_TSC1::mode_rwa].init((uint8_t*)&ram_data.mode_rwa, sizeof(uint8_t) * 1);
    REG_TSC1[(int)reg_REG_TSC1::mode_rwa_delay_ms].init((uint8_t*)&ram_data.mode_rwa_delay_ms, sizeof(uint32_t) * 1);
    REG_TSC1[(int)reg_REG_TSC1::mode_slave].init((uint8_t*)&ram_data.mode_slave, sizeof(uint8_t) * 1);
    REG_TSC1[(int)reg_REG_TSC1::mode_slave_delay_ms].init((uint8_t*)&ram_data.mode_slave_delay_ms, sizeof(uint32_t) * 1);
    REG_TSC1[(int)reg_REG_TSC1::mode_stt].init((uint8_t*)&ram_data.mode_stt, sizeof(uint8_t) * 1);
    REG_TSC1[(int)reg_REG_TSC1::mode_stt_delay_ms].init((uint8_t*)&ram_data.mode_stt_delay_ms, sizeof(uint32_t) * 1);
    REG_TSC1[(int)reg_REG_TSC1::mode_uart].init((uint8_t*)&ram_data.mode_uart, sizeof(uint8_t) * 1);
    REG_TSC1[(int)reg_REG_TSC1::mode_uart_delay_ms].init((uint8_t*)&ram_data.mode_uart_delay_ms, sizeof(uint32_t) * 1);
    REG_TSC1[(int)reg_REG_TSC1::sen_acs].init((uint8_t*)&ram_data.sen_acs, sizeof(uint8_t) * 1);
    REG_TSC1[(int)reg_REG_TSC1::sen_ads].init((uint8_t*)&ram_data.sen_ads, sizeof(uint8_t) * 1);
    REG_TSC1[(int)reg_REG_TSC1::sen_can].init((uint8_t*)&ram_data.sen_can, sizeof(uint8_t) * 1);
    REG_TSC1[(int)reg_REG_TSC1::sen_cube_acs].init((uint8_t*)&ram_data.sen_cube_acs, sizeof(uint8_t) * 1);
    REG_TSC1[(int)reg_REG_TSC1::sen_cube_ads].init((uint8_t*)&ram_data.sen_cube_ads, sizeof(uint8_t) * 1);
    REG_TSC1[(int)reg_REG_TSC1::sen_cube_est_quat].init((uint8_t*)&ram_data.sen_cube_est_quat[0], sizeof(int16_t) * 3);
    REG_TSC1[(int)reg_REG_TSC1::sen_cube_est_rate].init((uint8_t*)&ram_data.sen_cube_est_rate[0], sizeof(int16_t) * 3);
    REG_TSC1[(int)reg_REG_TSC1::sen_cube_gyro].init((uint8_t*)&ram_data.sen_cube_gyro[0], sizeof(int16_t) * 3);
    REG_TSC1[(int)reg_REG_TSC1::sen_cube_mag].init((uint8_t*)&ram_data.sen_cube_mag[0], sizeof(int16_t) * 3);
    REG_TSC1[(int)reg_REG_TSC1::sen_cube_mtq1].init((uint8_t*)&ram_data.sen_cube_mtq1, sizeof(int16_t) * 1);
    REG_TSC1[(int)reg_REG_TSC1::sen_cube_mtq2].init((uint8_t*)&ram_data.sen_cube_mtq2, sizeof(int16_t) * 1);
    REG_TSC1[(int)reg_REG_TSC1::sen_cube_mtq3].init((uint8_t*)&ram_data.sen_cube_mtq3, sizeof(int16_t) * 1);
    REG_TSC1[(int)reg_REG_TSC1::sen_cube_nadir].init((uint8_t*)&ram_data.sen_cube_nadir[0], sizeof(int16_t) * 3);
    REG_TSC1[(int)reg_REG_TSC1::sen_cube_posvel_ecef].init((uint8_t*)&ram_data.sen_cube_posvel_ecef[0], sizeof(int16_t) * 3);
    REG_TSC1[(int)reg_REG_TSC1::sen_cube_rwa1].init((uint8_t*)&ram_data.sen_cube_rwa1, sizeof(int16_t) * 1);
    REG_TSC1[(int)reg_REG_TSC1::sen_cube_rwa2].init((uint8_t*)&ram_data.sen_cube_rwa2, sizeof(int16_t) * 1);
    REG_TSC1[(int)reg_REG_TSC1::sen_cube_rwa3].init((uint8_t*)&ram_data.sen_cube_rwa3, sizeof(int16_t) * 1);
    REG_TSC1[(int)reg_REG_TSC1::sen_cube_sun].init((uint8_t*)&ram_data.sen_cube_sun[0], sizeof(int16_t) * 3);
    REG_TSC1[(int)reg_REG_TSC1::sen_i2c].init((uint8_t*)&ram_data.sen_i2c, sizeof(uint8_t) * 1);
    REG_TSC1[(int)reg_REG_TSC1::sen_mem].init((uint8_t*)&ram_data.sen_mem, sizeof(uint8_t) * 1);
    REG_TSC1[(int)reg_REG_TSC1::sen_model_igrf].init((uint8_t*)&ram_data.sen_model_igrf[0], sizeof(int16_t) * 3);
    REG_TSC1[(int)reg_REG_TSC1::sen_model_posvel_ecef].init((uint8_t*)&ram_data.sen_model_posvel_ecef[0], sizeof(int16_t) * 3);
    REG_TSC1[(int)reg_REG_TSC1::sen_model_sun].init((uint8_t*)&ram_data.sen_model_sun[0], sizeof(int16_t) * 3);
    REG_TSC1[(int)reg_REG_TSC1::sen_mtq].init((uint8_t*)&ram_data.sen_mtq, sizeof(uint8_t) * 1);
    REG_TSC1[(int)reg_REG_TSC1::sen_nothing].init((uint8_t*)&ram_data.sen_nothing, sizeof(uint8_t) * 1);
    REG_TSC1[(int)reg_REG_TSC1::sen_obc_current].init((uint8_t*)&ram_data.sen_obc_current, sizeof(int16_t) * 1);
    REG_TSC1[(int)reg_REG_TSC1::sen_obc_imu1].init((uint8_t*)&ram_data.sen_obc_imu1[0], sizeof(int16_t) * 3);
    REG_TSC1[(int)reg_REG_TSC1::sen_obc_imu2].init((uint8_t*)&ram_data.sen_obc_imu2[0], sizeof(int16_t) * 3);
    REG_TSC1[(int)reg_REG_TSC1::sen_obc_mag1].init((uint8_t*)&ram_data.sen_obc_mag1[0], sizeof(int16_t) * 3);
    REG_TSC1[(int)reg_REG_TSC1::sen_obc_mag2].init((uint8_t*)&ram_data.sen_obc_mag2[0], sizeof(int16_t) * 3);
    REG_TSC1[(int)reg_REG_TSC1::sen_obc_mtq1].init((uint8_t*)&ram_data.sen_obc_mtq1, sizeof(int16_t) * 1);
    REG_TSC1[(int)reg_REG_TSC1::sen_obc_mtq2].init((uint8_t*)&ram_data.sen_obc_mtq2, sizeof(int16_t) * 1);
    REG_TSC1[(int)reg_REG_TSC1::sen_obc_mtq3].init((uint8_t*)&ram_data.sen_obc_mtq3, sizeof(int16_t) * 1);
    REG_TSC1[(int)reg_REG_TSC1::sen_obc_rwa1].init((uint8_t*)&ram_data.sen_obc_rwa1, sizeof(int16_t) * 1);
    REG_TSC1[(int)reg_REG_TSC1::sen_obc_rwa2].init((uint8_t*)&ram_data.sen_obc_rwa2, sizeof(int16_t) * 1);
    REG_TSC1[(int)reg_REG_TSC1::sen_obc_rwa3].init((uint8_t*)&ram_data.sen_obc_rwa3, sizeof(int16_t) * 1);
    REG_TSC1[(int)reg_REG_TSC1::sen_obc_rwa4].init((uint8_t*)&ram_data.sen_obc_rwa4, sizeof(int16_t) * 1);
    REG_TSC1[(int)reg_REG_TSC1::sen_obc_temp1].init((uint8_t*)&ram_data.sen_obc_temp1, sizeof(int16_t) * 1);
    REG_TSC1[(int)reg_REG_TSC1::sen_run].init((uint8_t*)&ram_data.sen_run, sizeof(uint8_t) * 1);
    REG_TSC1[(int)reg_REG_TSC1::sen_rwa].init((uint8_t*)&ram_data.sen_rwa, sizeof(uint8_t) * 1);
    REG_TSC1[(int)reg_REG_TSC1::sen_sat_inertia].init((uint8_t*)&rom_data.sen_sat_inertia[0], sizeof(float) * 6);
    REG_TSC1[(int)reg_REG_TSC1::sen_slave].init((uint8_t*)&ram_data.sen_slave, sizeof(uint8_t) * 1);
    REG_TSC1[(int)reg_REG_TSC1::sen_uart].init((uint8_t*)&ram_data.sen_uart, sizeof(uint8_t) * 1);
    REG_TSC1[(int)reg_REG_TSC1::sen_unix_time].init((uint8_t*)&rom_data.sen_unix_time, sizeof(uint32_t) * 1);
    REG_TSC1[(int)reg_REG_TSC1::ping_stat].init((uint8_t*)&ram_data.ping_stat, sizeof(uint8_t) * 1);
    REG_TSC1[(int)reg_REG_TSC1::ls_stat].init((uint8_t*)&ram_data.ls_stat, sizeof(uint8_t) * 1);
}

#endif // _REG_TSC1_H
