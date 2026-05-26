#include "app_modules_config.h"
#if CSP_ENABLE_DATA_MON

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
#include <csp_data_mon_protocol.h>
#include <data_monitor_core.h>


extern uint8_t data_mon_count;
extern data_monitor_t *h_data_mon[CSP_DATA_MONITOR_MAX_TASK];

static int do_data_mon_start(struct csp_data_mon_message *cdm) {
	char *dm_name = cdm->start.name;

	if(tsc_strnlen(dm_name, CSP_DATA_MONITOR_NAME_LENGTH) == 0)
		return CSP_ERR_INVAL;

	data_monitor_t *dm = NULL;
	if(data_monitor_get(dm_name, &dm)) {
		tsc_snprintf(cdm->start.status, sizeof(cdm->start.status), "Error: data monitor '%s' not found.\r\n", dm_name);
		return CSP_ERR_NONE;
	}

    int res = data_monitor_start(dm);
    tsc_kprintf("Data monitor: %s, start: %s\r\n", dm_name, res == 0 ? "Success" : "Fail");
    tsc_snprintf(cdm->start.status, sizeof(cdm->start.status), "Data monitor: %s, start: %s\r\n", dm_name, res == 0 ? "Success" : "Fail");

	return CSP_ERR_NONE;
}

static int do_data_mon_stop(struct csp_data_mon_message *cdm) {
	char *dm_name = cdm->stop.name;

	if(tsc_strnlen(dm_name, CSP_DATA_MONITOR_NAME_LENGTH) == 0)
		return CSP_ERR_INVAL;

	data_monitor_t *dm = NULL;
	if(data_monitor_get(dm_name, &dm)) {
		tsc_snprintf(cdm->stop.status, sizeof(cdm->stop.status), "Error: data monitor '%s' not found.\r\n", dm_name);
		return CSP_ERR_NONE;
	}

    int res = data_monitor_stop(dm);
    tsc_kprintf("Data monitor: %s, stop: %s\r\n", dm_name, res == 0 ? "Success" : "Fail");
    tsc_snprintf(cdm->stop.status, sizeof(cdm->stop.status), "Data monitor: %s, stop: %s\r\n", dm_name, res == 0 ? "Success" : "Fail");

	return CSP_ERR_NONE;
}

static int do_data_mon_get_data(struct csp_data_mon_message *cdm) {
	char *dm_name = cdm->get_data.dm_name;
	char *dmc_name = cdm->get_data.dmc_name;

	if(strlen(dm_name) == 0 || strlen(dmc_name) == 0)
		return CSP_ERR_INVAL;

	data_monitor_t *dm = NULL;
	if(data_monitor_get(dm_name, &dm)) {
		tsc_kprintf("Data Monitor name = %s not found! \r\n", dm_name);
		return CSP_ERR_INVAL;
	}

	data_monitor_config_t *dmc = NULL;
	if(data_monitor_config_get(dm, dmc_name, &dmc)) {
		tsc_kprintf("Data Monitor Config name = %s not found! \r\n", dmc_name);
		return CSP_ERR_INVAL;
	}

	tsc_memcpy(&cdm->get_data.storage, &dmc->storage, sizeof(triple_storage_t));
	cdm->get_data.type = dmc->type;

	return CSP_ERR_NONE;
}

static int do_data_mon_set_data(struct csp_data_mon_message *cdm) {
	char *dm_name = cdm->set_data.dm_name;
	char *dmc_name = cdm->set_data.dmc_name;

	if (tsc_strnlen(dm_name, CSP_DATA_MONITOR_NAME_LENGTH) == 0 ||
		tsc_strnlen(dmc_name, CSP_DATA_MONITOR_NAME_LENGTH) == 0)
		return CSP_ERR_INVAL;

	data_monitor_t *dm = NULL;
	if (data_monitor_get(dm_name, &dm)) {
		tsc_snprintf(cdm->set_data.status, sizeof(cdm->set_data.status),
		             "Error: data monitor '%s' not found.\r\n", dm_name);
		return CSP_ERR_NONE;
	}

	data_monitor_config_t *dmc = NULL;
	if (data_monitor_config_get(dm, dmc_name, &dmc)) {
		tsc_snprintf(cdm->set_data.status, sizeof(cdm->set_data.status),
		             "Error: config '%s' not found.\r\n", dmc_name);
		return CSP_ERR_NONE;
	}

	if (cdm->set_data.type != dmc->type) {
		tsc_snprintf(cdm->set_data.status, sizeof(cdm->set_data.status),
		             "Error: type mismatch (req=%d, cfg=%d).\r\n",
		             cdm->set_data.type, dmc->type);
		return CSP_ERR_NONE;
	}

	size_t len = cdm->set_data.len;
	if (len == 0 || len > DATA_MONITOR_MAX_DATA_BYTES) {
		tsc_snprintf(cdm->set_data.status, sizeof(cdm->set_data.status),
		             "Error: bad length %u.\r\n", (unsigned int)len);
		return CSP_ERR_NONE;
	}

	int rc = data_monitor_set_value(dm, dmc, cdm->set_data.raw, len);
	if (rc == DATA_MON_ERROR_OK) {
		tsc_snprintf(cdm->set_data.status, sizeof(cdm->set_data.status),
		             "OK: value updated.\r\n");
	} else {
		tsc_snprintf(cdm->set_data.status, sizeof(cdm->set_data.status),
		             "Error: set failed, code=%d.\r\n", rc);
	}

	return CSP_ERR_NONE;
}

static int do_data_mon_list(csp_conn_t * conn, csp_packet_t * packet) {
	csp_buffer_free(packet);
	packet = NULL;

	char * monlist = csp_malloc(512);
	int monlen = 0;
	/* Check for malloc fail */
	if (monlist == NULL) {
		/* Allocate packet buffer, if need be */
		if (packet == NULL)
			packet = csp_buffer_get(256);
		if (packet == NULL)
			return CSP_ERR_NOMEM;
		/* Send out the data */
		strcpy((char *)packet->data, "Not enough memory");
		packet->length = strlen((char *)packet->data);
		/* Break and let the default handling send packet */
		return CSP_ERR_NONE;
	}

	monlen = tsc_sprintf(monlist, "***** Data Monitor List *****\r\n");
	for(int i=0; i<data_mon_count; ++i) {
		monlen += tsc_sprintf(monlist + monlen, "[No.%d] = %s\r\n", i+1, h_data_mon[i]->name);
	}

	/* Retrieve the data monitor list */
	do {
		if(!monlen)
			break;

		/* Split the potentially very long string into packets */
		int i = 0;
		while (i < monlen) {

			/* Allocate packet buffer, if need be */
			if (packet == NULL)
				packet = csp_buffer_get(256);
			if (packet == NULL)
				break;

			/* Calculate length, either full MTU or the remainder */
			packet->length = (monlen - i > CSP_DATA_MONITOR_RPS_MTU) ? CSP_DATA_MONITOR_RPS_MTU : (monlen - i);

			/* Send out the data */
			memcpy(packet->data, &monlist[i], packet->length);
			i += packet->length;
			if (!csp_send(conn, packet, 0))
				csp_buffer_free(packet);

			/* Clear the packet reference when sent */
			packet = NULL;

		}

	} while (0);

	csp_free(monlist);
	return CSP_ERR_NONE;
}


static int data_mon_status(csp_conn_t * conn, data_monitor_t * dm) {
	csp_packet_t * packet=NULL;
	char * mon_data = csp_malloc(512);
	int monlen = 0;
	/* Check for malloc fail */
	if (mon_data == NULL) {
		/* Allocate packet buffer, if need be */
		if (packet == NULL)
			packet = csp_buffer_get(256);
		if (packet == NULL)
			return CSP_ERR_NOMEM;
		/* Send out the data */
		strcpy((char *)packet->data, "Not enough memory");
		packet->length = strlen((char *)packet->data);
		/* Break and let the default handling send packet */
		return CSP_ERR_NONE;
	}
	monlen = tsc_sprintf(mon_data, "***** Data Monitor Status *****\r\n");
	monlen += tsc_sprintf(mon_data + monlen, "Data Monitor Name = %s\r\n", dm->name);
	monlen += tsc_sprintf(mon_data + monlen, "Update period = %d\r\n", dm->period_ms);
	monlen += tsc_sprintf(mon_data + monlen, "Running = %s\r\n\r\n", dm->running ? "Yes" : "No");
	monlen += tsc_sprintf(mon_data + monlen, "| %-16s | %-5s | %-14s | %-14s | %-14s | %-14s | %-20s |\r\n", "Name", "Type", "DefVal", "Min", "Max", "Current Val", "Error");
	monlen += tsc_sprintf(mon_data + monlen, "|------------------|-------|----------------|----------------|----------------|----------------|----------------------|\r\n");

	/* Retrieve the data monitor config list */
	data_monitor_config_t *dmc = dm->config_first;
	int dmc_num = dm->monitor_count;
	do {
		if(!monlen)
			break;

		/* Split the potentially very long string into packets */
		int i = 0;
		while (i < monlen) {

			/* Allocate packet buffer, if need be */
			if (packet == NULL)
				packet = csp_buffer_get(256);
			if (packet == NULL)
				break;

			/* Calculate length, either full MTU or the remainder */
			packet->length = (monlen - i > CSP_DATA_MONITOR_RPS_MTU) ? CSP_DATA_MONITOR_RPS_MTU : (monlen - i);

			/* Send out the data */
			tsc_memcpy(packet->data, &mon_data[i], packet->length);
			i += packet->length;
			if (!csp_send(conn, packet, 0))
				csp_buffer_free(packet);

			/* Clear the packet reference when sent */
			packet = NULL;
		}

		if(!dmc)
			break;

		char buff[5][32];
		uint8_t out_data[DATA_MONITOR_MAX_DATA_BYTES];
		size_t out_len;
    	config_val_str(dmc->config_range->type, dmc->config_range->value, buff[0]);
    	config_val_str(dmc->config_range->type, dmc->config_range->min, buff[1]);
    	config_val_str(dmc->config_range->type, dmc->config_range->max, buff[2]);
    	if(data_monitor_get_data(dm, dmc->name, out_data, &out_len)) {
    		tsc_sprintf(buff[3], "Error Recovery");
    	}
    	else {
    		if(out_len)
    			config_val_str(dmc->type, out_data, buff[3]);
    		else
    			tsc_sprintf(buff[3], "Error Length");
    	}

		switch (dmc->error_code) {
		case DATA_MON_ERROR_READ:
			tsc_sprintf(buff[4], "Read Error");
			break;
		case DATA_MON_ERROR_TYPE:
			tsc_sprintf(buff[4], "Type Error");
			break;
		case DATA_MON_ERROR_LENGTH:
			tsc_sprintf(buff[4], "Length Error");
			break;
		case DATA_MON_ERROR_MAX:
			tsc_sprintf(buff[4], "Over Limit");
			break;
		case DATA_MON_ERROR_MIN:
			tsc_sprintf(buff[4], "Under Limit");
			break;
		case DATA_MON_ERROR_OK:
			tsc_sprintf(buff[4], "None");
			break;
		default:
			tsc_sprintf(buff[4], "Unknown");
		}

		monlen = tsc_snprintf(mon_data, CSP_DATA_MONITOR_RPS_MTU, "| %-16s |   %-1d   | %-14s | %-14s | %-14s | %-14s | %-20s |\r\n",
                	dmc->name,
					dmc->type,
					buff[0],
					buff[1],
					buff[2],
					buff[3],
					buff[4]);

        dmc = dmc->config_next;

        if(!dmc) monlen += tsc_sprintf(mon_data + monlen, "\r\n************* End *************\r\n");

	} while (dmc_num--);

	csp_free(mon_data);
	return CSP_ERR_NONE;
}

static int do_data_mon_status(csp_conn_t *conn, csp_packet_t *packet) {
	struct csp_data_mon_message *cdm = (struct csp_data_mon_message*) packet->data;

	data_monitor_t *dm = NULL;
	if(!data_monitor_get(cdm->status.name, &dm)) {
		csp_buffer_free(packet);
		data_mon_status(conn, dm);
		return CSP_ERR_NONE;
	}

	csp_buffer_free(packet);
	return CSP_ERR_INVAL;
}

static int data_mon_config_status(csp_conn_t * conn, data_monitor_t * dm, data_monitor_config_t * dmc) {
	csp_packet_t * packet=NULL;
	char * mon_data = csp_malloc(512);
	int monlen = 0;
	/* Check for malloc fail */
	if (mon_data == NULL) {
		/* Allocate packet buffer, if need be */
		if (packet == NULL)
			packet = csp_buffer_get(256);
		if (packet == NULL)
			return CSP_ERR_NOMEM;
		/* Send out the data */
		strcpy((char *)packet->data, "Not enough memory");
		packet->length = strlen((char *)packet->data);
		/* Break and let the default handling send packet */
		return CSP_ERR_NONE;
	}

	char buff[5][32];
	uint8_t out_data[DATA_MONITOR_MAX_DATA_BYTES];
	size_t out_len;

	config_val_str(dmc->config_range->type, dmc->config_range->value, buff[0]);
	config_val_str(dmc->config_range->type, dmc->config_range->min, buff[1]);
	config_val_str(dmc->config_range->type, dmc->config_range->max, buff[2]);
	if (data_monitor_get_data(dm, dmc->name, out_data, &out_len)) {
		tsc_sprintf(buff[3], "Error Recovery");
	} else {
		if (out_len)
			config_val_str(dmc->type, out_data, buff[3]);
		else
			tsc_sprintf(buff[3], "Error Length");
	}

	switch (dmc->error_code) {
	case DATA_MON_ERROR_READ:
		tsc_sprintf(buff[4], "Read Error");
		break;
	case DATA_MON_ERROR_TYPE:
		tsc_sprintf(buff[4], "Type Error");
		break;
	case DATA_MON_ERROR_LENGTH:
		tsc_sprintf(buff[4], "Length Error");
		break;
	case DATA_MON_ERROR_MAX:
		tsc_sprintf(buff[4], "Over Limit");
		break;
	case DATA_MON_ERROR_MIN:
		tsc_sprintf(buff[4], "Under Limit");
		break;
	case DATA_MON_ERROR_OK:
		tsc_sprintf(buff[4], "None");
		break;
	default:
		tsc_sprintf(buff[4], "Unknown");
	}


	monlen = tsc_sprintf(mon_data, "***** Data Config Status *****\r\n");
	monlen += tsc_sprintf(mon_data + monlen, "Config Name = %s\r\n", dmc->name);
	monlen += tsc_sprintf(mon_data + monlen, "Config Type = %d\r\n", dmc->type);
	monlen += tsc_sprintf(mon_data + monlen, "Config DefVal = %s\r\n", buff[0]);
	monlen += tsc_sprintf(mon_data + monlen, "Config Min = %s\r\n", buff[1]);
	monlen += tsc_sprintf(mon_data + monlen, "Config Max = %s\r\n", buff[2]);
	monlen += tsc_sprintf(mon_data + monlen, "Config Val = %s\r\n", buff[3]);
	monlen += tsc_sprintf(mon_data + monlen, "Config Error = %s\r\n", buff[4]);
    monlen += tsc_sprintf(mon_data + monlen, "\r\n************* End *************\r\n");

	/* Send the data monitor config */
	if (!monlen)
		return CSP_ERR_INVAL;

	/* Split the potentially very long string into packets */
	int i = 0;
	while (i < monlen) {

		/* Allocate packet buffer, if need be */
		if (packet == NULL)
			packet = csp_buffer_get(256);
		if (packet == NULL)
			break;

		/* Calculate length, either full MTU or the remainder */
		packet->length = (monlen - i > CSP_DATA_MONITOR_RPS_MTU) ? CSP_DATA_MONITOR_RPS_MTU : (monlen - i);

		/* Send out the data */
		tsc_memcpy(packet->data, &mon_data[i], packet->length);
		i += packet->length;
		if (!csp_send(conn, packet, 0))
			csp_buffer_free(packet);

		/* Clear the packet reference when sent */
		packet = NULL;
	}


	csp_buffer_free(packet);
	csp_free(mon_data);

	return CSP_ERR_NONE;
}

static int do_data_mon_config_status(csp_conn_t *conn, csp_packet_t *packet) {
	struct csp_data_mon_message *cdm = (struct csp_data_mon_message*) packet->data;
	char * dm_name = cdm->config_status.dm_name;
	char * dmc_name = cdm->config_status.dmc_name;

	if(!dm_name)
		return CSP_ERR_INVAL;

	data_monitor_t *dm = NULL;
	if(data_monitor_get(dm_name, &dm)) {
		tsc_kprintf("Data Monitor name = %s not found! \r\n", dm_name);
		return CSP_ERR_INVAL;
	}

	data_monitor_config_t *dmc = NULL;
	if(data_monitor_config_get(dm, dmc_name, &dmc)) {
		tsc_kprintf("Data Monitor Config name = %s not found! \r\n", dmc_name);
		return CSP_ERR_INVAL;
	}

	csp_buffer_free(packet);
	packet = NULL;
	data_mon_config_status(conn, dm, dmc);

	return CSP_ERR_NONE;
}


/* CSP Data Monitor Protocol handler */
static int csp_data_mon_handler(csp_conn_t * conn, csp_packet_t * packet) {

	int ret = CSP_ERR_INVAL;
	struct csp_data_mon_message * cdm = (struct csp_data_mon_message *) packet->data;

	/* Ignore everything but requests */
	if (cdm->type != CSP_DATA_MON_REQUEST)
		return ret;

	switch (cdm->code) {
		case CSP_DATA_MON_START:
			ret = do_data_mon_start(cdm);
			packet->length = CSP_DATA_MON_SIZE(start);
			break;

		case CSP_DATA_MON_STOP:
			ret = do_data_mon_stop(cdm);
			packet->length = CSP_DATA_MON_SIZE(stop);
			break;

		case CSP_DATA_MON_GET_DATA:
			ret = do_data_mon_get_data(cdm);
			packet->length = CSP_DATA_MON_SIZE(get_data);;
			break;

		case CSP_DATA_MON_LIST:
			ret = do_data_mon_list(conn, packet);
			packet->length = 0;
			break;

		case CSP_DATA_MON_STATUS:
			ret = do_data_mon_status(conn, packet);
			packet->length = 0;
			break;

		case CSP_DATA_MON_CONFIG_STATUS:
			ret = do_data_mon_config_status(conn, packet);
			packet->length = 0;
			break;

		case CSP_DATA_MON_SET_DATA:
			ret = do_data_mon_set_data(cdm);
			packet->length = CSP_DATA_MON_SIZE(set_data);
			break;
	}

	cdm->type = CSP_DATA_MON_REPLY;

	return ret;
}

void csp_data_mon_service_handler(csp_conn_t * conn, csp_packet_t * packet) {

	switch (csp_conn_dport(conn)) {

	case CSP_DATA_MONITOR_PORT:
		/* Pass to CSP data monitor handler */
		if (csp_data_mon_handler(conn, packet) != CSP_ERR_NONE) {
			csp_buffer_free(packet);
			return;
		}
		break;

	default:
		csp_buffer_free(packet);
		return;
	}

	if ((packet != NULL) && (packet->length))  {
		if (!csp_send(conn, packet, 0))
			csp_buffer_free(packet);
	}
}

int csp_data_mon(uint8_t node, uint32_t timeout, uint8_t code, int msg_size, struct csp_data_mon_message * msg) {
	msg->type = CSP_DATA_MON_REQUEST;
	msg->code = code;
	int status = csp_transaction(CSP_PRIO_NORM, node, CSP_DATA_MONITOR_PORT, timeout, msg, msg_size, msg, msg_size);
	if (status == 0) {
		return CSP_ERR_TIMEDOUT;
	}

	return CSP_ERR_NONE;
}


void csp_data_mon_list(uint8_t node, uint32_t timeout) {

	/* Open connection */
	csp_conn_t * conn = csp_connect(CSP_PRIO_NORM, node, CSP_DATA_MONITOR_PORT, 0, 0);
	if (conn == NULL) {
		return;
	}

	/* Prepare data */
	csp_packet_t * packet = csp_buffer_get(256);

	/* Check malloc */
	if (packet == NULL) {
		goto out;
	}

	struct csp_data_mon_message * msg = (struct csp_data_mon_message *) packet->data;
	packet->length = 2;
	msg->type = CSP_DATA_MON_REQUEST;
	msg->code = CSP_DATA_MON_LIST;

	tsc_kprintf("Registered data monitor node %u: \r\n", node);

	/* Try to send frame */
	if (!csp_send(conn, packet, 0)) {
		csp_buffer_free(packet);
		goto out;
	}

	while(1) {
		/* Read incoming frame */
		packet = csp_read(conn, timeout);
		if (packet == NULL) {
			break;
		}

		/* We have a reply, ensure data is 0 (zero) termianted */
		unsigned int length = (packet->length < csp_buffer_data_size()) ? packet->length : (csp_buffer_data_size() - 1);
		packet->data[length] = 0;
		tsc_kprintf("%s", packet->data);

		/* Each packet from csp_read must to be freed by user */
		csp_buffer_free(packet);
	}

	tsc_kprintf("\r\n");

	/* Clean up */
out:
	csp_close(conn);
}

void csp_data_mon_status(uint8_t node, uint32_t timeout, const char *dm_name) {

	/* Open connection */
	csp_conn_t * conn = csp_connect(CSP_PRIO_NORM, node, CSP_DATA_MONITOR_PORT, 0, 0);
	if (conn == NULL) {
		return;
	}

	/* Prepare data */
	csp_packet_t * packet = csp_buffer_get(256);

	/* Check malloc */
	if (packet == NULL) {
		goto out;
	}

	tsc_memset(packet->data, 0, CSP_DATA_MON_SIZE(status));
	struct csp_data_mon_message * msg = (struct csp_data_mon_message *) packet->data;
	packet->length = CSP_DATA_MON_SIZE(status);
	msg->type = CSP_DATA_MON_REQUEST;
	msg->code = CSP_DATA_MON_STATUS;
	if(strnlen(dm_name, CSP_DATA_MONITOR_NAME_LENGTH) && dm_name)
		tsc_strncpy(msg->status.name, dm_name, CSP_DATA_MONITOR_NAME_LENGTH);

	tsc_kprintf("Data monitoring status node %u: \r\n", node);

	/* Try to send frame */
	if (!csp_send(conn, packet, 0)) {
		csp_buffer_free(packet);
		goto out;
	}

	while(1) {
		/* Read incoming frame */
		packet = csp_read(conn, timeout);
		if (packet == NULL) {
			break;
		}

		/* We have a reply, ensure data is 0 (zero) termianted */
		unsigned int length = (packet->length < csp_buffer_data_size()) ? packet->length : (csp_buffer_data_size() - 1);
		packet->data[length] = 0;
		tsc_kprintf("%s", packet->data);

		/* Each packet from csp_read must to be freed by user */
		csp_buffer_free(packet);
	}

	tsc_kprintf("\r\n");

	/* Clean up */
out:
	csp_close(conn);
}

void csp_data_mon_config_status(uint8_t node, uint32_t timeout, const char *dm_name, const char *dmc_name) {

	/* Open connection */
	csp_conn_t * conn = csp_connect(CSP_PRIO_NORM, node, CSP_DATA_MONITOR_PORT, 0, 0);
	if (conn == NULL) {
		return;
	}

	/* Prepare data */
	csp_packet_t * packet = csp_buffer_get(256);

	/* Check malloc */
	if (packet == NULL) {
		goto out;
	}

	tsc_memset(packet->data, 0, CSP_DATA_MON_SIZE(config_status));
	struct csp_data_mon_message * msg = (struct csp_data_mon_message *) packet->data;
	packet->length = CSP_DATA_MON_SIZE(config_status);
	msg->type = CSP_DATA_MON_REQUEST;
	msg->code = CSP_DATA_MON_CONFIG_STATUS;
	if(strnlen(dm_name, CSP_DATA_MONITOR_NAME_LENGTH) && dm_name)
		tsc_strncpy(msg->config_status.dm_name, dm_name, CSP_DATA_MONITOR_NAME_LENGTH);
	if(strnlen(dmc_name, CSP_DATA_MONITOR_NAME_LENGTH) && dmc_name)
		tsc_strncpy(msg->config_status.dmc_name, dmc_name, CSP_DATA_MONITOR_NAME_LENGTH);

	/* Try to send frame */
	if (!csp_send(conn, packet, 0)) {
		csp_buffer_free(packet);
		goto out;
	}

	while(1) {

		/* Read incoming frame */
		packet = csp_read(conn, timeout);
		if (packet == NULL) {
			break;
		}

		/* We have a reply, ensure data is 0 (zero) termianted */
		unsigned int length = (packet->length < csp_buffer_data_size()) ? packet->length : (csp_buffer_data_size() - 1);
		packet->data[length] = 0;
		tsc_kprintf("%s", packet->data);

		/* Each packet from csp_read must to be freed by user */
		csp_buffer_free(packet);
	}

	tsc_kprintf("\r\n");

	/* Clean up */
out:
	csp_close(conn);
}


#endif /* CSP_ENABLE_DATA_MON */
