#include "app_modules_config.h"
#if CSP_ENABLE_LOG

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

#include <kservice.h>
#include <csp/arch/csp_malloc.h>
#include <csp_log_protocol.h>

#include <tsc_log_core.h>

static int do_log_be_level(struct csp_log_message *clm) {
	const char *name = clm->be_level.be_name;
	uint32_t level = clm->be_level.level;

	if (strlen(name) == 0) return CSP_ERR_INVAL;

	if ((level <= LOG_FILTER_LVL_ALL) && (level >= 0)) {
		tsc_log_be_lvl_filter_set(name, level);
	} else {
		tsc_kprintf("Please input correct level (0-%d).\n", LOG_FILTER_LVL_ALL);
		return CSP_ERR_INVAL;
	}

	return CSP_ERR_NONE;
}

static int do_log_tag_level(struct csp_log_message *clm) {
	const char *name = clm->tag_level.tag_name;
	uint32_t level = clm->tag_level.level;

	if (strlen(name) == 0) return CSP_ERR_INVAL;

	if ((level <= LOG_FILTER_LVL_ALL) && (level >= 0)) {
		tsc_log_tag_lvl_filter_set(name, level);
	} else {
		tsc_kprintf("Please input correct level (0-%d).\n", LOG_FILTER_LVL_ALL);
		return CSP_ERR_INVAL;
	}

	return CSP_ERR_NONE;
}

static int do_log_level(struct csp_log_message *clm) {
	uint32_t level = clm->level;

	if ((level <= LOG_FILTER_LVL_ALL) && (level >= 0)) {
		tsc_log_global_filter_lvl_set(level);
	} else {
		tsc_kprintf("Please input correct level (0-%d).\n", LOG_FILTER_LVL_ALL);
		return CSP_ERR_INVAL;
	}

	return CSP_ERR_NONE;
}

static int do_log_tag(struct csp_log_message *clm) {
	const char *name = clm->tag;

	if (tsc_strlen(name) <= TSC_LOG_FILTER_TAG_MAX_LEN) {
		tsc_log_global_filter_tag_set(name);
	} else {
		tsc_kprintf("The tag length is too long. Max is %d.\n", TSC_LOG_FILTER_TAG_MAX_LEN);
		return CSP_ERR_INVAL;
	}

	return CSP_ERR_NONE;
}

static int do_log_keyword(struct csp_log_message *clm) {
	const char *name = clm->keyword;

	if (tsc_strlen(name) <= TSC_LOG_FILTER_KW_MAX_LEN) {
		tsc_log_global_filter_kw_set(name);
	} else {
		tsc_kprintf("The keyword length is too long. Max is %d.\n", TSC_LOG_FILTER_KW_MAX_LEN);
		return CSP_ERR_INVAL;
	}

	return CSP_ERR_NONE;
}

static int do_log_filter(csp_conn_t *conn, csp_packet_t *packet) {
	struct csp_log_message *clm = (struct csp_log_message*) packet->data;
	int log_len = 0;
#ifndef TSC_LOG_USING_SYSLOG
	const char *lvl_name[] = { "Assert ", "Error  ", "Error  ", "Error  ", "Warning", "Info   ", "Info   ", "Debug  " };
#endif
	const char *tag = tsc_log_global_filter_tag_get(), *kw = tsc_log_global_filter_kw_get();
	tsc_slist_t *node;
	tsc_log_tag_lvl_filter_t tag_lvl = NULL;

	char *log_data = csp_malloc(512);
	/* Check for malloc fail */
	if (log_data == NULL) {
		/* Send out the data */
		strcpy((char*) clm->filter, "Not enough memory");
		packet->length = CSP_LOG_SIZE(filter);

		if (!csp_send(conn, packet, 0)) csp_buffer_free(packet);
		/* Break and let the default handling send packet */
		return CSP_ERR_NONE;
	}

	log_len = tsc_sprintf(log_data, "--------------------------------------\n");
	log_len += tsc_sprintf(log_data + log_len, "tsc_log global filter:\n");

#ifndef TSC_LOG_USING_SYSLOG
	log_len += tsc_sprintf(log_data + log_len, "level   : %s\n", lvl_name[tsc_log_global_filter_lvl_get()]);
#else
	log_len += tsc_sprintf(log_data + log_len, "level   : %d\n", tsc_log_global_filter_lvl_get());
#endif

	log_len += tsc_sprintf(log_data + log_len, "tag     : %s\n", tsc_strlen(tag) == 0 ? "NULL" : tag);
	log_len += tsc_sprintf(log_data + log_len, "keyword : %s\n", tsc_strlen(kw) == 0 ? "NULL" : kw);

	log_len += tsc_sprintf(log_data + log_len, "--------------------------------------\n");
	log_len += tsc_sprintf(log_data + log_len, "tsc_log tag's level filter:\n");
	if (tsc_slist_isempty(tsc_log_tag_lvl_list_get())) log_len += tsc_sprintf(log_data + log_len, "settings not found\n");

	/* Split the potentially very long string into packets */
	int i = 0;
	while (i < log_len) {

		/* Allocate packet buffer, if need be */
		if (packet == NULL) {
			packet = csp_buffer_get(CSP_LOG_MANAGER_RPS_MTU);
			clm = (struct csp_log_message*) packet->data;
		}
		if (packet == NULL) break;

		/* Calculate length, either full MTU or the remainder */
		int data_len = (log_len - i > sizeof(clm->filter) - 1) ? sizeof(clm->filter) - 1 : (log_len - i);
		packet->length = CSP_LOG_SIZE(filter);

		/* Send out the data */
		memset(clm->filter, 0, sizeof(clm->filter));
		memcpy(clm->filter, &log_data[i], data_len);
		clm->filter[data_len] = 0;
		i += data_len;
		if (!csp_send(conn, packet, 0)) csp_buffer_free(packet);

		/* Clear the packet reference when sent */
		packet = NULL;
	}

	/* Retrieve the log filter list */
	if (tsc_slist_isempty(tsc_log_tag_lvl_list_get()) == 0) {
		/* show the tag level list */
		for (node = tsc_slist_first(tsc_log_tag_lvl_list_get()); node; node = tsc_slist_next(node)) {
			tag_lvl = tsc_slist_entry(node, struct tsc_log_tag_lvl_filter, list);
			log_len = tsc_sprintf(log_data, "%-*.s: ", TSC_LOG_FILTER_TAG_MAX_LEN, tag_lvl->tag);

#ifndef TSC_LOG_USING_SYSLOG
			log_len += tsc_sprintf(log_data + log_len, "%s\n", lvl_name[tag_lvl->level]);
#else
			log_len += tsc_sprintf(log_data + log_len, "%d\n", tag_lvl->level);
#endif
			int i = 0;
			while (i < log_len) {

				/* Allocate packet buffer, if need be */
				if (packet == NULL) {
					packet = csp_buffer_get(CSP_LOG_MANAGER_RPS_MTU);
					clm = (struct csp_log_message*) packet->data;
				}
				if (packet == NULL) break;

				/* Calculate length, either full MTU or the remainder */
				int data_len = (log_len - i > sizeof(clm->filter) - 1) ? sizeof(clm->filter) - 1 : (log_len - i);
				packet->length = CSP_LOG_SIZE(filter);

				/* Send out the data */
				memset(clm->filter, 0, sizeof(clm->filter));
				memcpy(clm->filter, &log_data[i], data_len);
				clm->filter[data_len] = 0;
				i += data_len;
				if (!csp_send(conn, packet, 0)) csp_buffer_free(packet);

				/* Clear the packet reference when sent */
				packet = NULL;
			}
		}
	}

	csp_buffer_free(packet);
	csp_free(log_data);
	packet->length = 0;

	return CSP_ERR_NONE;
}

/* CSP Log Management Protocol handler */
static int csp_log_handler(csp_conn_t *conn, csp_packet_t *packet) {

	int ret = CSP_ERR_INVAL;
	struct csp_log_message *clm = (struct csp_log_message*) packet->data;

	/* Ignore everything but requests */
	if (clm->type != CSP_LOG_REQUEST) return ret;

	switch (clm->code) {
	case CSP_LOG_BE_LEVEL:
		ret = do_log_be_level(clm);
		packet->length = CSP_LOG_SIZE(be_level);
		break;

	case CSP_LOG_TAG_LEVEL:
		ret = do_log_tag_level(clm);
		packet->length = CSP_LOG_SIZE(tag_level);
		break;

	case CSP_LOG_LEVEL:
		ret = do_log_level(clm);
		packet->length = CSP_LOG_SIZE(level);
		break;

	case CSP_LOG_TAG:
		ret = do_log_tag(clm);
		packet->length = CSP_LOG_SIZE(tag);
		break;

	case CSP_LOG_KEYWORD:
		ret = do_log_keyword(clm);
		packet->length = CSP_LOG_SIZE(keyword);
		break;

	case CSP_LOG_FILTER:
		ret = do_log_filter(conn, packet);
		packet->length = 0;
		break;
	}

	clm->type = CSP_LOG_REPLY;

	return ret;
}

void csp_log_service_handler(csp_conn_t *conn, csp_packet_t *packet) {

	switch (csp_conn_dport(conn)) {

	case CSP_LOG_MANAGER_PORT:
		/* Pass to CSP thread handler */
		if (csp_log_handler(conn, packet) != CSP_ERR_NONE) {
			csp_buffer_free(packet);
			return;
		}
		break;

	default:
		csp_buffer_free(packet);
		return;
	}

	if ((packet != NULL) && (packet->length)) {
		if (!csp_send(conn, packet, 0)) csp_buffer_free(packet);
	}

}

int csp_log(uint8_t node, uint32_t timeout, uint8_t code, int msg_size, struct csp_log_message *msg) {
	msg->type = CSP_LOG_REQUEST;
	msg->code = code;
	int status = csp_transaction(CSP_PRIO_NORM, node, CSP_LOG_MANAGER_PORT, timeout, msg, msg_size, msg, msg_size);
	if (status == 0) {
		return CSP_ERR_TIMEDOUT;
	}

	return CSP_ERR_NONE;
}

void csp_log_filter(uint8_t node, uint32_t timeout) {

	/* Open connection */
	csp_conn_t *conn = csp_connect(CSP_PRIO_NORM, node, CSP_LOG_MANAGER_PORT, 0, 0);
	if (conn == NULL) {
		return;
	}

	/* Prepare data */
	csp_packet_t *packet = csp_buffer_get(95);

	/* Check malloc */
	if (packet == NULL) {
		goto out;
	}

	struct csp_log_message *msg = (struct csp_log_message*) packet->data;
	packet->length = 2;
	msg->type = CSP_LOG_REQUEST;
	msg->code = CSP_LOG_FILTER;

	tsc_kprintf("Log filter info on node %u: \r\n", node);

	/* Try to send frame */
	if (!csp_send(conn, packet, 0)) {
		goto out;
	}

	while (1) {

		/* Read incoming frame */
		packet = csp_read(conn, timeout);
		if (packet == NULL) {
			break;
		}

		/* We have a reply */
		struct csp_log_message *clm = (struct csp_log_message*) packet->data;
		tsc_kprintf("%s", clm->filter);

		/* Each packet from csp_read must to be freed by user */
		csp_buffer_free(packet);
	}

	tsc_kprintf("\r\n");

	/* Clean up */
	out: csp_buffer_free(packet);
	csp_close(conn);

}


#endif /* CSP_ENABLE_LOG */
