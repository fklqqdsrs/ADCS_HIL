#include "app_modules_config.h"
#if CSP_ENABLE_ROUTE

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

#include <config_core.h>
#include <kservice.h>
#include <csp/csp.h>
#include <csp/arch/csp_malloc.h>
#include <csp_route_core.h>

extern char rtable_data[256];

static int do_route_load(struct csp_route_message *crm) {
	if (csp_rtable_save(rtable_data, sizeof(rtable_data)) != CSP_ERR_NONE) return CSP_ERR_INVAL;

	crm->load = csp_rtable_load(rtable_data);
	return CSP_ERR_NONE;
}

static int do_route_check(struct csp_route_message *crm) {
	if (csp_rtable_save(rtable_data, sizeof(rtable_data)) != CSP_ERR_NONE) return CSP_ERR_INVAL;

	crm->check = csp_rtable_check(rtable_data);
	return CSP_ERR_NONE;
}

static int do_route_add(struct csp_route_message *crm) {
	csp_iface_t *ifc = csp_iflist_get_by_name(crm->add.iface_name);

	if (!ifc) return CSP_ERR_INVAL;

	crm->add.error = csp_rtable_set(crm->add.address, crm->add.netmask, ifc, crm->add.via);
	return CSP_ERR_NONE;
}

static int do_route_save(struct csp_route_message *crm) {
	if (csp_rtable_save(rtable_data, sizeof(rtable_data)) != CSP_ERR_NONE) return CSP_ERR_INVAL;

	 if (!config_save_profile()) return CSP_ERR_BUSY;

	return CSP_ERR_NONE;
}

static int do_route_clear(struct csp_route_message *crm) {
	csp_rtable_clear();
	return CSP_ERR_NONE;
}

static int do_route_status(struct csp_route_message *crm) {
	if (csp_rtable_save(rtable_data, sizeof(rtable_data)) != CSP_ERR_NONE) return CSP_ERR_INVAL;

	tsc_strncpy(crm->status.data, rtable_data, sizeof(crm->status.data));

	return CSP_ERR_NONE;
}

/* CSP Route Management Protocol handler */
static int csp_route_handler(csp_conn_t *conn, csp_packet_t *packet) {

	int ret = CSP_ERR_INVAL;
	struct csp_route_message *crm = (struct csp_route_message*) packet->data;

	/* Ignore everything but requests */
	if (crm->type != CSP_ROUTE_REQUEST) return ret;

	switch (crm->code) {
	case CSP_ROUTE_LOAD:
		ret = do_route_load(crm);
		packet->length = CSP_ROUTE_SIZE(load);
		break;

	case CSP_ROUTE_CHECK:
		ret = do_route_check(crm);
		packet->length = CSP_ROUTE_SIZE(check);
		break;

	case CSP_ROUTE_ADD:
		ret = do_route_add(crm);
		packet->length = CSP_ROUTE_SIZE(add);
		break;

	case CSP_ROUTE_SAVE:
		ret = do_route_save(crm);
		packet->length = CSP_ROUTE_SIZE(save);
		break;

	case CSP_ROUTE_CLEAR:
		ret = do_route_clear(crm);
		packet->length = CSP_ROUTE_SIZE(clear);
		break;

	case CSP_ROUTE_STATUS:
		ret = do_route_status(crm);
		packet->length = CSP_ROUTE_SIZE(status);
		break;
	}

	crm->type = CSP_ROUTE_REPLY;

	return ret;
}

void csp_route_service_handler(csp_conn_t *conn, csp_packet_t *packet) {

	switch (csp_conn_dport(conn)) {

	case CSP_ROUTE_MANAGER_PORT:
		/* Pass to CSP thread handler */
		if (csp_route_handler(conn, packet) != CSP_ERR_NONE) {
			csp_buffer_free(packet);
			return;
		}
		break;

	default:
		csp_buffer_free(packet);
		return;
	}

	if (packet != NULL) {
		if (!csp_send(conn, packet, 0)) csp_buffer_free(packet);
	}

}

int csp_route(uint8_t node, uint32_t timeout, uint8_t code, int msg_size, struct csp_route_message *msg) {
	msg->type = CSP_ROUTE_REQUEST;
	msg->code = code;
	int status = csp_transaction(CSP_PRIO_NORM, node, CSP_ROUTE_MANAGER_PORT, timeout, msg, msg_size, msg, msg_size);
	if (status == 0) {
		return CSP_ERR_TIMEDOUT;
	}

	return CSP_ERR_NONE;
}


#endif /* CSP_ENABLE_ROUTE */
