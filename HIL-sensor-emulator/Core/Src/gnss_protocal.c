/*
 * gnss_protocal.c
 *
 *  Created on: Jan 21, 2026
 *      Author: Chinnapop
 */

#include "gnss_protocal.h"


gps_time_t unix_to_gps(uint32_t unix, uint8_t leap_sec)
{

	gps_time_t gps_time;
    int64_t sec = ((int64_t)unix - (int64_t)UNIX_GPS_EPOCH_OFFSET) + (int64_t)leap_sec;


    gps_time.gps_week = (uint16_t)(sec / (int64_t)SECONDS_IN_WEEK);

    int64_t seconds_into_week = sec % (int64_t)SECONDS_IN_WEEK;
    gps_time.gps_msec = (uint32_t)(seconds_into_week * 1000LL);

    return gps_time;
}

uint8_t msg_default_congfig(bestXYZ_format_t *msg)
{
     /*header config*/
		msg->header.sync[0] = 0xAA;
		msg->header.sync[1] = 0x44;
		msg->header.sync[2] = 0x12;
		msg->header.header_length =  0x1C;
		msg->header.msg_id[0] = 0xF1;
		msg->header.msg_id[1] = 0x00;
		msg->header.msg_type = 0x00;
		msg->header.port_addr = 0xC0;
		msg->header.msg_length = 0x70;
		msg->header.seq = 0x10;
		msg->header.idle_time = 100;
		msg->header.time_stat = 160;
		msg->header.receiver_stat = 0x100;
		msg->header.receiver_version = 65535;
		/*message config*/
		msg->ecef.psol_stat = 0;
		msg->ecef.pos_type = 0x8;
		msg->ecef.pos_sd[0] = 10.245;
		msg->ecef.pos_sd[1] = 13.291;
		msg->ecef.pos_sd[2] = 16.64;
		msg->ecef.vsol_stat = 0;
		msg->ecef.vel_type = 0x8;
		msg->ecef.vel_sd[0] = 0.245;
		msg->ecef.vel_sd[1] = 3.291;
		msg->ecef.vel_sd[2] = 1.64;
		msg->ecef.stn_id[0] = 0x00;
		msg->ecef.stn_id[1] = 0x11;
		msg->ecef.stn_id[2] = 0xFC;
		msg->ecef.stn_id[3] = 0x12;
		msg->ecef.diff_age = 10;
		msg->ecef.sol_age = 230;
		msg->ecef.num_sat_track = 12;
		msg->ecef.num_sat_sol = 10;
		msg->ecef.num_sat_l1 = 4;
		msg->ecef.num_sat_multi_freq = 7;
		msg->ecef.ext_sol_stat = 0x20;
		msg->ecef.galileo_beidou = 0x1;
		msg->ecef.gps_glonass = 0x20;
		msg->ecef.crc = 0xAAF311C7;







}
