#include "app_modules_config.h"
#if CSP_ENABLE_THREAD

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
#include <csp_thread_protocol.h>
#include <thread_manager_core.h>


extern uint32_t thread_count;
extern managed_thread_t thread_table[];

static int do_thread_start(struct csp_thread_message *ctm) {
	int res = CSP_ERR_NONE;
	char *thread_name = ctm->start.name;

	if(thread_name == NULL)
		return CSP_ERR_INVAL;

    if (!is_thread_exist(thread_name))
    {
        tsc_kprintf("Error: thread '%s' not found.\r\n", thread_name);
        tsc_snprintf(ctm->start.status, sizeof(ctm->start.status), "Error: thread '%s' not found.\r\n", thread_name);
        return CSP_ERR_INVAL;
    }

    res = thread_start(thread_name);
    tsc_kprintf("Thread: %s, start: %s\r\n", thread_name, res == 0 ? "OK" : "Fail");
    tsc_snprintf(ctm->start.status, sizeof(ctm->start.status), "Thread start: %s\r\n", res == 0 ? "OK" : "Fail");

	return res;
}

static int do_thread_stop(struct csp_thread_message *ctm) {
	int res = CSP_ERR_NONE;
	char *thread_name = ctm->stop.name;

	if(thread_name == NULL)
		return CSP_ERR_INVAL;

    if (!is_thread_exist(thread_name))
    {
        tsc_kprintf("Error: thread '%s' not found.\r\n", thread_name);
        tsc_snprintf(ctm->stop.status, sizeof(ctm->stop.status), "Error: thread '%s' not found.\r\n", thread_name);
        return CSP_ERR_INVAL;
    }

    res = thread_stop(thread_name);
    tsc_kprintf("Thread: %s, stop: %s\r\n", thread_name, res == 0 ? "OK" : "Fail");
    tsc_snprintf(ctm->stop.status, sizeof(ctm->stop.status), "Thread stop: %s\r\n", res == 0 ? "OK" : "Fail");

	return res;
}

static int do_thread_restart(struct csp_thread_message *ctm) {
	int res = CSP_ERR_NONE;
	char *thread_name = ctm->restart.name;

	if(thread_name == NULL)
		return CSP_ERR_INVAL;

    if (!is_thread_exist(thread_name))
    {
        tsc_kprintf("Error: thread '%s' not found.\r\n", thread_name);
        tsc_snprintf(ctm->restart.status, sizeof(ctm->restart.status), "Error: thread '%s' not found.\r\n", thread_name);
        return CSP_ERR_INVAL;
    }

    res = thread_restart(thread_name);
    tsc_kprintf("Thread: %s, restart: %s\r\n", thread_name, res == 0 ? "OK" : "Fail");
    tsc_snprintf(ctm->restart.status, sizeof(ctm->restart.status), "Thread restart: %s\r\n", res == 0 ? "OK" : "Fail");

	return res;
}

static int do_thread_status(struct csp_thread_message *ctm) {
	char *thread_name = ctm->status.name;

	if(thread_name == NULL)
		return CSP_ERR_INVAL;

	managed_thread_t *thread_ptr =  thread_get_manage_thread(thread_name);

	if(thread_ptr == NULL)
		return CSP_ERR_INVAL;

	/* Copy thread status */
	tsc_strncpy(ctm->status.name, thread_name, CSP_THREAD_MANAGER_NAME_LENGTH);
	ctm->status.stack_size = thread_ptr->attr.stack_size;
	ctm->status.priority = thread_ptr->attr.priority;

	ctm->status.last_heartbeat = thread_ptr->last_heartbeat;
	ctm->status.retry_count = thread_ptr->retry_count;
	ctm->status.enabled = thread_ptr->enabled;

	ctm->status.watchdog_timeout_ms = thread_ptr->watchdog_timeout_ms;
	ctm->status.max_retry_limit = thread_ptr->max_retry_limit;
	ctm->status.watchdog_enabled = thread_ptr->watchdog_enabled;
	ctm->status.autorun = thread_ptr->autorun;

	return CSP_ERR_NONE;
}

static int do_thread_status_all(csp_conn_t * conn, csp_packet_t * packet) {

	char * thlist = csp_malloc(384);
	int thlen = 0;
	/* Check for malloc fail */
	if (thlist == NULL) {
		/* Send out the data */
		strcpy((char *)packet->data, "Not enough memory");
		packet->length = strlen((char *)packet->data);
		/* Break and let the default handling send packet */
		return CSP_ERR_NOMEM;
	}

	thlen = tsc_sprintf(thlist, "| %-16s | %-6s | %-21s | %-14s | %-10s | %-8s | %-9s | %-7s | %-7s |\r\n", "Name", "State", "Retry", "Last Kick (ms)", "Stack", "Priority", "Watchdog", "Timeout", "Autorun");
	thlen += tsc_sprintf(thlist + thlen, "|------------------|--------|-----------------------|----------------|------------|----------|-----------|---------|---------|\r\n");

	packet->length = thlen;

	/* Retrieve the thread list */
	int thread_num = 0;
	do {
		if(!thlen)
			break;

		/* Split the potentially very long string into packets */
		int i = 0;
		while (i < thlen) {

			/* Allocate packet buffer, if need be */
			if (packet == NULL)
				packet = csp_buffer_get(CSP_THREAD_MANAGER_RPS_MTU);
			if (packet == NULL)
				break;

			/* Calculate length, either full MTU or the remainder */
			packet->length = (thlen - i > CSP_THREAD_MANAGER_RPS_MTU) ? CSP_THREAD_MANAGER_RPS_MTU : (thlen - i);

			/* Send out the data */
			memcpy(packet->data, &thlist[i], packet->length);
			i += packet->length;
			if (!csp_send(conn, packet, 0))
				csp_buffer_free(packet);

			/* Clear the packet reference when sent */
			packet = NULL;

		}

		if(thread_num >= thread_count)
			break;

        managed_thread_t *t = &thread_table[thread_num];
        thlen = tsc_snprintf(thlist, CSP_THREAD_MANAGER_RPS_MTU, "| %-16s | %-6s | %10lu/%-10lu | %-14lu | %-10u | %-8u | %-9s | %-7lu | %-7s |\r\n",
                      t->name,
                      t->enabled ? "ON" : "OFF",
                      (unsigned long)t->retry_count,
					  (unsigned long)t->max_retry_limit,
                      (unsigned long)t->last_heartbeat,
                      (unsigned int)t->attr.stack_size,
                      (unsigned int)t->attr.priority,
                      t->watchdog_enabled ? "ENABLED" : "DISABLED",
                      t->watchdog_enabled ? (unsigned long)t->watchdog_timeout_ms : 0,
        			  t->autorun ? "Yes" : "NO");
	} while (thread_num++ < thread_count);

	csp_buffer_free(packet);
	csp_free(thlist);

	return CSP_ERR_NONE;
}

static int do_thread_config(struct csp_thread_message *ctm) {
	char *thread_name = ctm->config.name;

	if(thread_name == NULL)
		return CSP_ERR_INVAL;

	managed_thread_t *thread_ptr =  thread_get_manage_thread(thread_name);

	if(thread_ptr == NULL)
		return CSP_ERR_INVAL;

	/* Copy thread status */
	thread_ptr->attr.stack_size = ctm->config.stack_size;
	thread_ptr->attr.priority = ctm->config.priority;

	thread_ptr->watchdog_timeout_ms = ctm->config.watchdog_timeout_ms;
	thread_ptr->max_retry_limit = ctm->config.max_retry_limit;
	thread_ptr->watchdog_enabled = ctm->config.watchdog_enabled;

	return CSP_ERR_NONE;
}


static int do_thread_list(csp_conn_t * conn, csp_packet_t * packet) {
	char * thlist = csp_malloc((CSP_THREAD_MANAGER_NAME_LENGTH + 2) * CSP_THREAD_MANAGER_MAX_NUM);
	int thlen = 0;
	/* Check for malloc fail */
	if (thlist == NULL) {
		/* Send out the data */
		strcpy((char *)packet->data, "Not enough memory");
		packet->length = strlen((char *)packet->data);
		/* Break and let the default handling send packet */
		return CSP_ERR_NOMEM;
	}

    for (uint32_t i = 0; i < thread_count; i++)
    {
    	thlen += tsc_sprintf(thlist + thlen, "%s\r\n", thread_table[i].name);
    }
	packet->length = thlen;

	/* Retrieve the thread list */
	do {
		if(!thlen)
			break;

		/* Split the potentially very long string into packets */
		int i = 0;
		while (i < thlen) {

			/* Allocate packet buffer, if need be */
			if (packet == NULL)
				packet = csp_buffer_get(CSP_THREAD_MANAGER_RPS_MTU);
			if (packet == NULL)
				break;

			/* Calculate length, either full MTU or the remainder */
			packet->length = (thlen - i > CSP_THREAD_MANAGER_RPS_MTU) ? CSP_THREAD_MANAGER_RPS_MTU : (thlen - i);

			/* Send out the data */
			memcpy(packet->data, &thlist[i], packet->length);
			i += packet->length;
			if (!csp_send(conn, packet, 0))
				csp_buffer_free(packet);

			/* Clear the packet reference when sent */
			packet = NULL;

		}

	} while (0);

	csp_buffer_free(packet);
	csp_free(thlist);

	return CSP_ERR_NONE;
}

/* CSP Thread Management Protocol handler */
static int csp_thread_handler(csp_conn_t * conn, csp_packet_t * packet) {

	int ret = CSP_ERR_INVAL;
	struct csp_thread_message * ctm = (struct csp_thread_message *) packet->data;

	/* Ignore everything but requests */
	if (ctm->type != CSP_THREAD_REQUEST)
		return ret;

	switch (ctm->code) {
		case CSP_THREAD_START:
			ret = do_thread_start(ctm);
			packet->length = CSP_THREAD_SIZE(start);
			break;

		case CSP_THREAD_STOP:
			ret = do_thread_stop(ctm);
			packet->length = CSP_THREAD_SIZE(stop);
			break;

		case CSP_THREAD_RESTART:
			ret = do_thread_restart(ctm);
			packet->length = CSP_THREAD_SIZE(restart);
			break;

		case CSP_THREAD_STATUS:
			ret = do_thread_status(ctm);
			packet->length = CSP_THREAD_SIZE(status);
			break;

		case CSP_THREAD_STATUS_ALL:
			ret = do_thread_status_all(conn, packet);
			packet->length = 0;
			break;

		case CSP_THREAD_CONFIG:
			ret = do_thread_config(ctm);
			packet->length = CSP_THREAD_SIZE(config);
			break;

		case CSP_THREAD_LIST:
			ret = do_thread_list(conn, packet);
			packet->length = 0;
			break;
	}

	ctm->type = CSP_THREAD_REPLY;

	return ret;
}

void csp_thread_service_handler(csp_conn_t * conn, csp_packet_t * packet) {

	switch (csp_conn_dport(conn)) {

	case CSP_THREAD_MANAGER_PORT:
		/* Pass to CSP thread handler */
		if (csp_thread_handler(conn, packet) != CSP_ERR_NONE) {
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

int csp_thread(uint8_t node, uint32_t timeout, uint8_t code, int msg_size, struct csp_thread_message * msg) {
	msg->type = CSP_THREAD_REQUEST;
	msg->code = code;
	int status = csp_transaction(CSP_PRIO_NORM, node, CSP_THREAD_MANAGER_PORT, timeout, msg, msg_size, msg, msg_size);
	if (status == 0) {
		return CSP_ERR_TIMEDOUT;
	}

	return CSP_ERR_NONE;
}

void csp_thread_status_all(uint8_t node, uint32_t timeout) {

	/* Open connection */
	csp_conn_t * conn = csp_connect(CSP_PRIO_NORM, node, CSP_THREAD_MANAGER_PORT, 0, 0);
	if (conn == NULL) {
		return;
	}

	/* Prepare data */
	csp_packet_t * packet = csp_buffer_get(256);

	/* Check malloc */
	if (packet == NULL) {
		goto out;
	}

	struct csp_thread_message * msg = (struct csp_thread_message *) packet->data;
	packet->length = CSP_THREAD_SIZE(status_all);
	msg->type = CSP_THREAD_REQUEST;
	msg->code = CSP_THREAD_STATUS_ALL;

	tsc_kprintf("Thread status all node %u: \r\n", node);

	/* Try to send frame */
	if (!csp_send(conn, packet, 0)) {
		goto out;
	}

	while(1) {

		/* Read incoming frame */
		packet = csp_read(conn, timeout);
		if (packet == NULL) {
			break;
		}

		/* We have a reply, ensure data is 0 (zero) termianted */
		const unsigned int length = (packet->length < csp_buffer_data_size()) ? packet->length : (csp_buffer_data_size() - 1);
		packet->data[length] = 0;
		tsc_kprintf("%s", packet->data);

		/* Each packet from csp_read must to be freed by user */
		csp_buffer_free(packet);
	}

	tsc_kprintf("\r\n");

	/* Clean up */
out:
	csp_buffer_free(packet);
	csp_close(conn);

}

void csp_thread_list(uint8_t node, uint32_t timeout) {

	/* Open connection */
	csp_conn_t * conn = csp_connect(CSP_PRIO_NORM, node, CSP_THREAD_MANAGER_PORT, 0, 0);
	if (conn == NULL) {
		return;
	}

	/* Prepare data */
	csp_packet_t * packet = csp_buffer_get(95);

	/* Check malloc */
	if (packet == NULL) {
		goto out;
	}

	struct csp_thread_message * msg = (struct csp_thread_message *) packet->data;
	packet->length = 2;
	msg->type = CSP_THREAD_REQUEST;
	msg->code = CSP_THREAD_LIST;

	tsc_kprintf("Registered threads node %u: \r\n", node);

	/* Try to send frame */
	if (!csp_send(conn, packet, 0)) {
		goto out;
	}

	while(1) {

		/* Read incoming frame */
		packet = csp_read(conn, timeout);
		if (packet == NULL) {
			break;
		}

		/* We have a reply, ensure data is 0 (zero) termianted */
		const unsigned int length = (packet->length < csp_buffer_data_size()) ? packet->length : (csp_buffer_data_size() - 1);
		packet->data[length] = 0;
		tsc_kprintf("%s", packet->data);

		/* Each packet from csp_read must to be freed by user */
		csp_buffer_free(packet);
	}

	tsc_kprintf("\r\n");

	/* Clean up */
out:
	csp_buffer_free(packet);
	csp_close(conn);

}


#endif /* CSP_ENABLE_THREAD */
