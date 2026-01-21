/*
 * Func_ADCS.h
 *
 *  Created on: Aug 23, 2024
 *      Author: thaib
 */

#ifndef SRC_FUNC_ADCS_H_
#define SRC_FUNC_ADCS_H_

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <include/csp/csp.h>
#include <include/csp/csp_buffer.h>

#ifdef __cplusplus
extern "C" {
#endif

#define ADDRESS_ADCS 3
#define PORT_ADCS 15
#define respond_success 0x55

#define MODULE_REACT_VIEW  0x20
#define MODULE_IMU  0x21 
#define MODULE_GPS  0x22 
#define MODULE_MTM  0x23

#define NULL_REGISTER 0x00
#define READ_REGISTER_ADCS 0x01
#define WRITE_REGISTER_ADCS 0x02

//view demo
#define MS_RPM_VIEW_REGISTER 0x01
#define SEC_RPM_VIEW_REGISTER 0x02

#define SUB_REG_VIEW01 0x01
#define SUB_REG_VIEW02 0x02
#define SUB_REG_VIEW03 0x03
#define SUB_REG_VIEW04 0x04

//imu (demo)
#define X_IMU_REGISTER 0x01
#define Y_IMU_REGISTER 0x02
#define Z_IMU_REGISTER 0x03
#define ALL_IMU_REGISTER 0x04

#define SUB_REG_IMU 0x01


//gps (demo)
#define LAT_GPS_REGISTER 0x01
#define LONG_GPS_REGISTER 0x02
#define MSL_GPS_REGISTER 0x03
#define ALL_GPS_REGISTER 0x04

#define SUB_REG_GPS 0x01


//mtm (demo)
#define X_MTM_REGISTER 0x01
#define Y_MTM_REGISTER 0x02
#define Z_MTM_REGISTER 0x03
#define ALL_MTM_REGISTER 0x04

#define SUB_REG_MTM 0x01


typedef struct
{
    uint8_t module_t;
    uint8_t action_t;
    uint8_t register_t;
    uint8_t register_sub_id_t;
} __attribute__((packed)) register_pack;

typedef struct
{
    float x_imu;
    float y_imu;
    float z_imu;
} __attribute__((packed)) axial_imu;

typedef struct
{
    double lat_gps;
    double long_gps;
    double msl_gps;
} __attribute__((packed)) gps_data_t;

typedef struct
{
    float x_mtm;
    float y_mtm;
    float z_mtm;
} __attribute__((packed)) axial_mtm;


int8_t set_ms_rpm_react_view(uint8_t view, uint32_t rpm_view, uint32_t timeout);
uint32_t get_ms_rpm_react_view(uint8_t view, uint32_t timeout);
int8_t get_imu(axial_imu *imu_data, uint32_t timeout);
int8_t get_gps(gps_data_t *gps_data_s, uint32_t timeout);
int8_t get_mtm(axial_mtm *axial_mtm_s, uint32_t timeout);
void service_adcs_demo(csp_conn_t *conn, csp_packet_t *packet) ;
#ifdef __cplusplus
 }
#endif
#endif /* SRC_FUNC_ADCS_H_ */
