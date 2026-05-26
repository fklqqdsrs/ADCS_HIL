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

#include <csp/csp.h>

#include <stdio.h>

#include <csp/csp_cmp.h>
#include <csp/csp_endian.h>
#include <csp/arch/csp_time.h>

#include <kservice.h>

#ifdef TSC_USING_FINSH
#include "finsh.h"
#endif

int csp_ping(uint8_t node, uint32_t timeout, unsigned int size, uint8_t conn_options) {

	unsigned int i;
	uint32_t start, time, status = 0;

	/* Counter */
	start = csp_get_ms();

	/* Open connection */
	csp_conn_t * conn = csp_connect(CSP_PRIO_NORM, node, CSP_PING, timeout, conn_options);
	if (conn == NULL)
		return -1;

	/* Prepare data */
	csp_packet_t * packet = csp_buffer_get(size);
	if (packet == NULL)
		goto out;

	/* Set data to increasing numbers */
	packet->length = size;
	for (i = 0; i < size; i++)
		packet->data[i] = i;

	/* Try to send frame */
	if (!csp_send(conn, packet, 0))
		goto out;

	/* Read incoming frame */
	packet = csp_read(conn, timeout);
	if (packet == NULL)
		goto out;

	/* Ensure that the data was actually echoed */
	for (i = 0; i < size; i++) {
		if (packet->data[i] != i % (0xff + 1)) {
			goto out;
		}
	}
	status = 1;

out:
	/* Clean up */
	csp_buffer_free(packet);
	csp_close(conn);

	/* We have a reply */
	time = (csp_get_ms() - start);

	if (status) {
		return time;
	}

	return -1;
}

#ifdef TSC_USING_FINSH
static void __csp_ping(int argc, char **argv) {
	int count=5, timeout=1000, size=1;

	if (argc > 1) {
		int node = atoi(argv[1]);
		if ((node >= 0) && (node <= 255)) {
			tsc_kprintf("CSP Ping node:%d ", node);

			if(argc > 2){
				int num = atoi(argv[2]);
				if(num > 0){
					count = num;
					tsc_kprintf("count:%d ", count);
				}
				else{
					tsc_kprintf("[count] must more than 0 \r\n");
					return;
				}
			}

			if(argc > 3){
				int num = atoi(argv[3]);
				if(num > 0){
					timeout = num;
					tsc_kprintf("timeout:%d ", timeout);
				}
				else{
					tsc_kprintf("[timeout] must more than 0 \r\n");
					return;
				}
			}

			if(argc > 4){
				int num = atoi(argv[4]);
				if(num > 0){
					size = num;
					tsc_kprintf("size:%d ", size);
				}
				else{
					tsc_kprintf("[size] must more than 0\r\n");
					return;
				}
			}

			tsc_kprintf("\r\n");
			for (int i = 0; i < count; i++) {
				/* Send ping to server */
				int result = csp_ping(node, timeout, size, CSP_O_NONE);
				tsc_kprintf("Ping address: %u, result %d [mS]\r\n", node, result);
			}

		} else {
			tsc_kprintf("<node> must be in 0 - 255 range.\r\n");
		}
	} else {
		tsc_kprintf("Please input: csp_ping <node> [count] [timeout] [size]\r\n");
	}
}
TSC_CMD_EXPORT_ALIAS(__csp_ping, csp_ping, CSP Ping to client with reply.);
#endif

void csp_ping_noreply(uint8_t node) {

	/* Prepare data */
	csp_packet_t * packet = csp_buffer_get(1);
	if (packet == NULL)
		return;

	/* Open connection */
	csp_conn_t * conn = csp_connect(CSP_PRIO_NORM, node, CSP_PING, 0, 0);
	if (conn == NULL) {
		csp_buffer_free(packet);
		return;
	}

	packet->data[0] = 0x55;
	packet->length = 1;

	printf("Ping ignore reply node %u.\r\n", (unsigned int) node);

	/* Try to send frame */
	if (!csp_send(conn, packet, 0))
		csp_buffer_free(packet);

	csp_close(conn);

}

#ifdef TSC_USING_FINSH
static void __csp_ping_noreply(int argc, char **argv) {

	if (argc == 2) {
		int node = atoi(argv[1]);
		if ((node >= 0) && (node <= 255)) {
			tsc_kprintf("CSP Ping node:%d \r\n", node);
			csp_ping_noreply(node);
		} else {
			tsc_kprintf("<node> must be in 0 - 255 range.\r\n");
		}
	} else {
		tsc_kprintf("Please input: csp_ping_noreply <node> \r\n");
	}
}
TSC_CMD_EXPORT_ALIAS(__csp_ping_noreply, csp_ping_noreply, CSP Ping to client with no reply.);
#endif

void csp_reboot(uint8_t node) {
	uint32_t magic_word = csp_hton32(CSP_REBOOT_MAGIC);
	csp_transaction(CSP_PRIO_NORM, node, CSP_REBOOT, 0, &magic_word, sizeof(magic_word), NULL, 0);
}

#ifdef TSC_USING_FINSH
static void __csp_reboot(int argc, char **argv) {

	if (argc == 2) {
		int node = atoi(argv[1]);
		if ((node >= 0) && (node <= 255)) {
			tsc_kprintf("CSP reboot node:%d \r\n", node);
			csp_reboot(node);
		} else {
			tsc_kprintf("<node> must be in 0 - 255 range.\r\n");
		}
	} else {
		tsc_kprintf("Please input: csp_reboot <node> \r\n");
	}
}
TSC_CMD_EXPORT_ALIAS(__csp_reboot, csp_reboot, CSP client reboot.);
#endif

void csp_shutdown(uint8_t node) {
	uint32_t magic_word = csp_hton32(CSP_REBOOT_SHUTDOWN_MAGIC);
	csp_transaction(CSP_PRIO_NORM, node, CSP_REBOOT, 0, &magic_word, sizeof(magic_word), NULL, 0);
}

#ifdef TSC_USING_FINSH
static void __csp_shutdown(int argc, char **argv) {

	if (argc == 2) {
		int node = atoi(argv[1]);
		if ((node >= 0) && (node <= 255)) {
			tsc_kprintf("CSP shutdown node:%d \r\n", node);
			csp_shutdown(node);
		} else {
			tsc_kprintf("<node> must be in 0 - 255 range.\r\n");
		}
	} else {
		tsc_kprintf("Please input: csp_shutdown <node> \r\n");
	}
}
TSC_CMD_EXPORT_ALIAS(__csp_shutdown, csp_shutdown, CSP client shutdown.);
#endif

void csp_ps(uint8_t node, uint32_t timeout) {

	/* Open connection */
	csp_conn_t * conn = csp_connect(CSP_PRIO_NORM, node, CSP_PS, 0, 0);
	if (conn == NULL) {
		return;
	}

	/* Prepare data */
	csp_packet_t * packet = csp_buffer_get(95);

	/* Check malloc */
	if (packet == NULL) {
		goto out;
	}

	packet->data[0] = 0x55;
	packet->length = 1;

	printf("PS node %u: \r\n", node);
	printf("Name\t\tStatus\tPri\tStack\tNum\r\n");

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
		printf("%s", packet->data);

		/* Each packet from csp_read must to be freed by user */
		csp_buffer_free(packet);
	}

	printf("\r\n");

	/* Clean up */
out:
	csp_buffer_free(packet);
	csp_close(conn);

}

#ifdef TSC_USING_FINSH
static void __csp_ps(int argc, char **argv) {

	if (argc > 1) {
		int node = atoi(argv[1]);
		if ((node >= 0) && (node <= 255)) {
			if (argc == 2) {
				tsc_kprintf("CSP PS node:%d \r\n", node);
				csp_ps(node, 1000);
				return;
			}
			if (argc == 3) {
				int timeout = atoi(argv[2]);
				if ((timeout >= 0) && (timeout <= 5000)) {
					tsc_kprintf("CSP PS node:%d, timeout:%d \r\n", node,
							timeout);
					csp_ps(node, timeout);
					return;
				}
			}
		}

		tsc_kprintf("<node> must be in 0 - 255 range.\r\n");
		tsc_kprintf("[timeout] must be in 0 - 5000 ms range.\r\n");
	} else {
		tsc_kprintf("Please input: csp_ps <node> [timeout]\r\n");
	}
}
TSC_CMD_EXPORT_ALIAS(__csp_ps, csp_ps, CSP client PS.);
#endif

void csp_interface(uint8_t node, uint32_t timeout) {

	/* Open connection */
	csp_conn_t * conn = csp_connect(CSP_PRIO_NORM, node, CSP_INTERFACE, 0, 0);
	if (conn == NULL) {
		return;
	}

	/* Prepare data */
	csp_packet_t * packet = csp_buffer_get(95);

	/* Check malloc */
	if (packet == NULL) {
		goto out;
	}

	packet->data[0] = 0x66;
	packet->length = 1;

	printf("Interface node %u: \r\n", node);

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
		printf("%s", packet->data);

		/* Each packet from csp_read must to be freed by user */
		csp_buffer_free(packet);
	}

	printf("\r\n");

	/* Clean up */
out:
	csp_buffer_free(packet);
	csp_close(conn);

}

#ifdef TSC_USING_FINSH
static void __csp_interface(int argc, char **argv) {

	if (argc > 1) {
		int node = atoi(argv[1]);
		if ((node >= 0) && (node <= 255)) {
			if (argc == 2) {
				tsc_kprintf("CSP interface node:%d \r\n", node);
				csp_interface(node, 1000);
				return;
			}
			if (argc == 3) {
				int timeout = atoi(argv[2]);
				if ((timeout >= 0) && (timeout <= 5000)) {
					tsc_kprintf("CSP interface node:%d, timeout:%d \r\n", node,
							timeout);
					csp_interface(node, timeout);
					return;
				}
			}
		}

		tsc_kprintf("<node> must be in 0 - 255 range.\r\n");
		tsc_kprintf("[timeout] must be in 0 - 5000 ms range.\r\n");
	} else {
		tsc_kprintf("Please input: csp_interface <node> [timeout]\r\n");
	}
}
TSC_CMD_EXPORT_ALIAS(__csp_interface, csp_interface, CSP client interfaces.);
#endif

int csp_get_memfree(uint8_t node, uint32_t timeout, uint32_t * size) {

	int status = csp_transaction(CSP_PRIO_NORM, node, CSP_MEMFREE, timeout, NULL, 0, size, sizeof(*size));
	if (status == sizeof(*size)) {
		*size = csp_ntoh32(*size);
		return CSP_ERR_NONE;
	}
	*size = 0;
	return CSP_ERR_TIMEDOUT;

}

void csp_memfree(uint8_t node, uint32_t timeout) {

	uint32_t memfree;
        int err = csp_get_memfree(node, timeout, &memfree);
	if (err == CSP_ERR_NONE) {
		printf("Free Memory at node %u is %"PRIu32" bytes\r\n", node, memfree);
	} else {
		printf("Network error\r\n");
	}

}

#ifdef TSC_USING_FINSH
static void __csp_memfree(int argc, char **argv) {

	if (argc > 1) {
		int node = atoi(argv[1]);
		if ((node >= 0) && (node <= 255)) {
			if (argc == 2) {
				tsc_kprintf("CSP memfree node:%d \r\n", node);
				csp_memfree(node, 1000);
				return;
			}
			if (argc == 3) {
				int timeout = atoi(argv[2]);
				if ((timeout >= 0) && (timeout <= 5000)) {
					tsc_kprintf("CSP memfree node:%d, timeout:%d \r\n", node,
							timeout);
					csp_memfree(node, timeout);
					return;
				}
			}
		}

		tsc_kprintf("<node> must be in 0 - 255 range.\r\n");
		tsc_kprintf("[timeout] must be in 0 - 5000 ms range.\r\n");
	} else {
		tsc_kprintf("Please input: csp_memfree <node> [timeout]\r\n");
	}
}
TSC_CMD_EXPORT_ALIAS(__csp_memfree, csp_memfree, CSP client memfree.);
#endif

int csp_get_buf_free(uint8_t node, uint32_t timeout, uint32_t * size) {

	int status = csp_transaction(CSP_PRIO_NORM, node, CSP_BUF_FREE, timeout, NULL, 0, size, sizeof(*size));
	if (status == sizeof(*size)) {
		*size = csp_ntoh32(*size);
		return CSP_ERR_NONE;
	}
	*size = 0;
	return CSP_ERR_TIMEDOUT;

}

void csp_buf_free(uint8_t node, uint32_t timeout) {

	uint32_t size;
	int err = csp_get_buf_free(node, timeout, &size);
	if (err == CSP_ERR_NONE) {
		printf("Free buffers at node %u is %"PRIu32"\r\n", node, size);
	} else {
		printf("Network error\r\n");
	}

}

#ifdef TSC_USING_FINSH
static void __csp_buf_free(int argc, char **argv) {

	if (argc > 1) {
		int node = atoi(argv[1]);
		if ((node >= 0) && (node <= 255)) {
			if (argc == 2) {
				tsc_kprintf("CSP buf_free node:%d \r\n", node);
				csp_buf_free(node, 1000);
				return;
			}
			if (argc == 3) {
				int timeout = atoi(argv[2]);
				if ((timeout >= 0) && (timeout <= 5000)) {
					tsc_kprintf("CSP buf_free node:%d, timeout:%d \r\n", node,
							timeout);
					csp_buf_free(node, timeout);
					return;
				}
			}
		}

		tsc_kprintf("<node> must be in 0 - 255 range.\r\n");
		tsc_kprintf("[timeout] must be in 0 - 5000 ms range.\r\n");
	} else {
		tsc_kprintf("Please input: csp_buf_free <node> [timeout]\r\n");
	}
}
TSC_CMD_EXPORT_ALIAS(__csp_buf_free, csp_buf_free, CSP client buf_free.);
#endif

int csp_get_uptime(uint8_t node, uint32_t timeout, uint32_t * uptime) {

	int status = csp_transaction(CSP_PRIO_NORM, node, CSP_UPTIME, timeout, NULL, 0, uptime, sizeof(*uptime));
	if (status == sizeof(*uptime)) {
		*uptime = csp_ntoh32(*uptime);
		return CSP_ERR_NONE;
	}
	*uptime = 0;
	return CSP_ERR_TIMEDOUT;
}

void csp_uptime(uint8_t node, uint32_t timeout) {

	uint32_t uptime;
	int err = csp_get_uptime(node, timeout, &uptime);
	if (err == CSP_ERR_NONE) {
		printf("Uptime of node %u is %"PRIu32" s\r\n", node, uptime);
	} else {
		printf("Network error\r\n");
	}

}

#ifdef TSC_USING_FINSH
static void __csp_uptime(int argc, char **argv) {

	if (argc > 1) {
		int node = atoi(argv[1]);
		if ((node >= 0) && (node <= 255)) {
			if (argc == 2) {
				tsc_kprintf("CSP uptime node:%d \r\n", node);
				csp_uptime(node, 1000);
				return;
			}
			if (argc == 3) {
				int timeout = atoi(argv[2]);
				if ((timeout >= 0) && (timeout <= 5000)) {
					tsc_kprintf("CSP uptime node:%d, timeout:%d \r\n", node, timeout);
					csp_uptime(node, timeout);
					return;
				}
			}
		}

		tsc_kprintf("<node> must be in 0 - 255 range.\r\n");
		tsc_kprintf("[timeout] must be in 0 - 5000 ms range.\r\n");
	} else {
		tsc_kprintf("Please input: csp_uptime <node> [timeout]\r\n");
	}
}
TSC_CMD_EXPORT_ALIAS(__csp_uptime, csp_uptime, CSP client uptime.);
#endif

int csp_cmp(uint8_t node, uint32_t timeout, uint8_t code, int msg_size, struct csp_cmp_message * msg) {
	msg->type = CSP_CMP_REQUEST;
	msg->code = code;
	int status = csp_transaction(CSP_PRIO_NORM, node, CSP_CMP, timeout, msg, msg_size, msg, msg_size);
	if (status == 0) {
		return CSP_ERR_TIMEDOUT;
	}

	return CSP_ERR_NONE;
}

int csp_set_cmp_clock(uint8_t node, uint32_t timeout, uint32_t *tv_sec, uint32_t *tv_nsec){
	struct csp_cmp_message msg;

	msg.clock.tv_sec = csp_hton32(*tv_sec);
	msg.clock.tv_nsec = csp_hton32(*tv_nsec);

	int status = csp_cmp_clock(node, timeout, &msg);

	*tv_sec = csp_ntoh32(msg.clock.tv_sec);
	*tv_nsec = csp_ntoh32(msg.clock.tv_nsec);

	return status;
}


#ifdef TSC_USING_FINSH
static void __csp_cmp_clock(int argc, char **argv) {
	int timeout = 1000, node;
	uint32_t sec, nsec;

	if (argc > 3) {
		node = atoi(argv[1]);
		sec = atol(argv[2]);
		nsec = atol(argv[3]);
		if ((node >= 0) && (node <= 255) && (sec >= 0) && (nsec >= 0)) {
			if (argc > 4) {
				int buff = atoi(argv[4]);
				if ((buff >= 0) && (buff <= 5000)) {
					timeout = buff;
				}
			}

			tsc_kprintf("CSP set_cmp_time node:%d sec:%d nsec:%d timeout:%d \r\n", node, sec, nsec, timeout);
			if(!csp_set_cmp_clock(node, timeout, &sec, &nsec)){
				tsc_kprintf("CSP response set_cmp_time node:%d sec:%d nsec:%d \r\n", node, sec, nsec);
			}else{
				tsc_kprintf("CSP response set_cmp_time error \r\n");
			}
			return;
		}

		tsc_kprintf("<node> must be in 0 - 255 range.\r\n");
		tsc_kprintf("<sec> must more than 0 \r\n");
		tsc_kprintf("<nsec> must more than 0 \r\n");
		tsc_kprintf("[timeout] must be in 0 - 5000 ms range.\r\n");
	} else {
		tsc_kprintf("Please input: csp_set_cmp_time <node> <sec> <nsec> [timeout]\r\n");
	}
}
TSC_CMD_EXPORT_ALIAS(__csp_cmp_clock, csp_cmp_clock, CSP cmp set clock.);
#endif

