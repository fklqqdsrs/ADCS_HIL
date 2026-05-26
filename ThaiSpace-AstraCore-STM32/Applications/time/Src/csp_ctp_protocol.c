/*
Cubesat Space Protocol - A small network-layer protocol designed for Cubesats
Copyright (C) 2012 GomSpace ApS (http://www.gomspace.com)
Copyright (C) 2012 AAUSAT3 Project (http://aausat3.space.aau.dk)

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "app_modules_config.h"
#if ENABLE_TIME_CTP

#include <csp_ctp_protocol.h>
#include <time_manager_core.h>

static int do_ctp_get(struct csp_ctp_message *ctp) {

	csp_timestamp_t clock;

	int res = CSP_ERR_NONE;

	csp_clock_get_time(&clock);

	ctp->get_time.tv_sec = clock.tv_sec;
	ctp->get_time.tv_nsec = clock.tv_nsec;

	return res;
}

static int do_ctp_set(struct csp_ctp_message *ctp) {

	csp_timestamp_t clock;

	clock.tv_sec = ctp->set_time.tv_sec;
	clock.tv_nsec = ctp->set_time.tv_nsec;

	int res = CSP_ERR_NONE;
	if (clock.tv_sec != 0) {
		// set time
		res = csp_clock_set_time(&clock);
		if (res != CSP_ERR_NONE) {
			csp_log_warn("csp_clock_set_time(sec=%"PRIu32", nsec=%"PRIu32") failed, error: %d", clock.tv_sec, clock.tv_nsec, res);
		}
	}

	csp_clock_get_time(&clock);

	ctp->set_time.tv_sec = clock.tv_sec;
	ctp->set_time.tv_nsec = clock.tv_nsec;

	return res;
}

static int do_ctp_status(struct csp_ctp_message *ctp) {

	/* Convert current time to string */
	time_t now = time(NULL);
	snprintf(ctp->status.time_str, CSP_CTP_TIME_STR_MAX_LEN, "%.*s", 25, ctime(&now));
	ctp->status.time_str[CSP_CTP_TIME_STR_MAX_LEN-1] = '\0';

	if(time_manager_is_pps_syn())
		snprintf(ctp->status.main_source, CSP_CTP_MAIN_SOURCE_MAX_LEN, "PPS");
	else if(time_manager_is_rtc_syn())
		snprintf(ctp->status.main_source, CSP_CTP_MAIN_SOURCE_MAX_LEN, "RTC");
	else
		snprintf(ctp->status.main_source, CSP_CTP_MAIN_SOURCE_MAX_LEN, "ERR");
	ctp->status.main_source[CSP_CTP_MAIN_SOURCE_MAX_LEN-1] = '\0';

	/* Copy PPS time */
	now = 0;
	if(time_manager_get_pps_time(&now))
		ctp->status.pps_time.tv_sec = 0;
	else
		ctp->status.pps_time.tv_sec = now;

	/* Copy RTC time */
	now = 0;
	if(time_manager_get_rtc_time(&now))
		ctp->status.rtc_time.tv_sec = 0;
	else
		ctp->status.rtc_time.tv_sec = now;

	return CSP_ERR_NONE;
}

/* CSP Management Protocol handler */
static int csp_ctp_handler(csp_conn_t * conn, csp_packet_t * packet) {

	int ret = CSP_ERR_INVAL;
	struct csp_ctp_message * ctp = (struct csp_ctp_message *) packet->data;

	/* Ignore everything but requests */
	if (ctp->type != CSP_CTP_REQUEST)
		return ret;

	switch (ctp->code) {
		case CSP_CTP_GET_TIME:
			ret = do_ctp_get(ctp);
			packet->length = CTP_SIZE(get_time);
			break;

		case CSP_CTP_SET_TIME:
			ret = do_ctp_set(ctp);
			packet->length = CTP_SIZE(set_time);
			break;

		case CSP_CTP_STATUS:
			ret = do_ctp_status(ctp);
			packet->length = CTP_SIZE(status);
			break;
	}

	ctp->type = CSP_CTP_REPLY;

	return ret;
}

void csp_ctp_service_handler(csp_conn_t * conn, csp_packet_t * packet) {

	switch (csp_conn_dport(conn)) {

	case CSP_CTP_PORT:
		/* Pass to CMP handler */
		if (csp_ctp_handler(conn, packet) != CSP_ERR_NONE) {
			csp_buffer_free(packet);
			return;
		}
		break;

	default:
		csp_buffer_free(packet);
		return;
	}

	if (packet != NULL) {
		if (!csp_send(conn, packet, 0))
			csp_buffer_free(packet);
	}
}

int csp_ctp(uint8_t node, uint32_t timeout, uint8_t code, int msg_size, struct csp_ctp_message * msg) {
	msg->type = CSP_CTP_REQUEST;
	msg->code = code;
	int status = csp_transaction(CSP_PRIO_NORM, node, CSP_CTP_PORT, timeout, msg, msg_size, msg, msg_size);
	if (status == 0) {
		return CSP_ERR_TIMEDOUT;
	}

	return CSP_ERR_NONE;
}

#endif
