#include "app_modules_config.h"
#if CSP_ENABLE_REGISTER

// register_service_init.c
// Created on: Jul 23, 2025
// Author: Chaturong

#include <config_core.h>
#include <stdio.h>
#include <errno.h>
#include <csp/csp.h>
#include <csp/arch/csp_thread.h>
#include <csp/arch/csp_malloc.h>
#include <csp_port.h>
#include <thread_manager_core.h>

#include "csp_register_service.h" // service_register()
#include "register_management.h"  // initializeRegister()
#include "register_protocol.h"

// ---------- Context ----------
typedef struct {
	uint8_t port;
	csp_thread_handle_t thread;
} regsvc_ctx_t;

static regsvc_ctx_t g_regsvc_ctx = { .port = REGISTER_PORT_DEFAULT };

// ---------- Task proto ----------
CSP_DEFINE_TASK(register_sv_task);

// ---------- Thread auto-register ----------
TSC_THREAD_REGISTER(REG_SVC_SERVER_NAME, register_sv_task, 0, /* user arg */
&g_regsvc_ctx, /* param to task */
REG_SVC_SERVER_STACK, REG_SVC_SERVER_PRIORITY, 0, 0, 0, REG_SVC_SERVER_AUTO_START);

// ---------- Task impl ----------
CSP_DEFINE_TASK(register_sv_task) {
	regsvc_ctx_t *ctx = (regsvc_ctx_t*) param;
	csp_log_info("Register Service started, port=%u", ctx->port);

	int rc = initializeRegister(MAX_REGISTERS);
	if (rc != CSP_ERR_NONE) {
		csp_log_error("REG: initializeRegister fail: %d", rc);
		return CSP_TASK_RETURN;
	}

	csp_socket_t *sock = csp_socket(CSP_SO_NONE);
	if (!sock) {
		csp_log_error("REG: socket() fail");
		return CSP_TASK_RETURN;
	}

	if (csp_bind(sock, ctx->port) == CSP_ERR_USED) {
		csp_close(sock);
		sock = csp_port_get_socket(ctx->port);
	} else {
		csp_listen(sock, REG_SVC_CON_BACKLOG);
	}

	while (1) {
		thread_wd_kick_self();
		csp_conn_t *conn = csp_accept(sock, REG_SVC_ACCEPT_TIMEOUT_MS);
		if (conn == NULL) {
			continue;
		}
		csp_packet_t *packet;
		while ((packet = csp_read(conn, REG_SVC_READ_TIMEOUT_MS)) != NULL) {
			if (csp_conn_dport(conn) == ctx->port) {
				service_register(conn, packet);
			} else {
				break;
			}
		}
		csp_close(conn);
	}

	return CSP_TASK_RETURN;
}

int register_service_init(uint8_t server_port) {
	g_regsvc_ctx.port = server_port;

	int rc = csp_thread_create(register_sv_task,
	REG_SVC_SERVER_NAME,
	REG_SVC_SERVER_STACK, &g_regsvc_ctx,
	REG_SVC_SERVER_PRIORITY, &g_regsvc_ctx.thread);
	if (rc != CSP_ERR_NONE) {
		csp_log_error("REG: thread_create fail: %d", rc);
		return rc;
	}

	return CSP_ERR_NONE;
}

#ifdef TSC_USING_FINSH
static void __regsvc_cli(int argc, char **argv) {
	uint8_t port = REGISTER_PORT_DEFAULT;
	if (argc > 1) {
		port = (uint8_t) atoi(argv[1]);
		if (port > CSP_ID_PORT_MAX) {
			tsc_kprintf("Usage: reg_server [port]\r\n");
			tsc_kprintf("[port] 0..%d\r\n", CSP_ID_PORT_MAX);
			return;
		}
	}
	int rc = register_service_init(port);
	if (rc == CSP_ERR_NONE) {
		tsc_kprintf("Register Service init OK (port=%u)\r\n", port);
	} else {
		tsc_kprintf("Register Service init FAIL: %d\r\n", rc);
	}
}
TSC_CMD_EXPORT_ALIAS(__regsvc_cli, reg_server, CSP Register Service init / start);
#endif


#endif /* CSP_ENABLE_REGISTER */
