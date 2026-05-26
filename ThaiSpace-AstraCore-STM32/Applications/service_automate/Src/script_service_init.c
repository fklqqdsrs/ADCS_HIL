#include "app_modules_config.h"
#if ENABLE_SCRIPT_MANAGER

/**
 * @file script_service_init.c
 * @author chatu
 * @brief Initialization for the script service, including CSP listener.
 */

#include <config_core.h>
#include "script_service_init.h"
#include "script_manager.h"
#include "script_scheduler.h"

#include <stdio.h>
#include <errno.h>

#include <kservice.h>
#include <csp/csp.h>
#include <csp/arch/csp_thread.h>
#include <csp_port.h>

#include <thread_manager_core.h>

/* ---------- Config macro (ปรับได้ตาม project) ---------- */
#ifndef SCRIPT_SVC_SERVER_NAME
#define SCRIPT_SVC_SERVER_NAME        "SCRIPT_SRV"
#endif

/* Increased stack size from 2048 to 4096 for safer CSP operations */
#ifndef SCRIPT_SVC_SERVER_STACK
#define SCRIPT_SVC_SERVER_STACK       4096
#endif

#ifndef SCRIPT_SVC_SERVER_PRIORITY
#define SCRIPT_SVC_SERVER_PRIORITY    osPriorityAboveNormal
#endif

#ifndef SCRIPT_SVC_CON_BACKLOG
#define SCRIPT_SVC_CON_BACKLOG        5
#endif

/* Configurable timeout values (can be overridden at build time) */
#ifndef SCRIPT_SVC_ACCEPT_TIMEOUT_MS
#define SCRIPT_SVC_ACCEPT_TIMEOUT_MS  1000
#endif

#ifndef SCRIPT_SVC_READ_TIMEOUT_MS
#define SCRIPT_SVC_READ_TIMEOUT_MS    1000
#endif

#ifndef SCRIPT_SVC_SERVER_AUTO_START
#define SCRIPT_SVC_SERVER_AUTO_START  1
#endif

/* ---------- Context ---------- */
typedef struct
{
    uint8_t port;
    csp_thread_handle_t thread;
} scriptsvc_ctx_t;

static scriptsvc_ctx_t g_scriptsvc_ctx = {
    .port = SCRIPT_SERVICE_PORT,
};

/* ---------- Task proto ---------- */
CSP_DEFINE_TASK(script_service_task);

/* ---------- Thread auto-register ---------- */
TSC_THREAD_REGISTER(
    SCRIPT_SVC_SERVER_NAME,      /* name */
    script_service_task,         /* entry */
    0,                           /* user arg (unused) */
    &g_scriptsvc_ctx,            /* param (ctx) */
    SCRIPT_SVC_SERVER_STACK,     /* stack size */
    SCRIPT_SVC_SERVER_PRIORITY,  /* priority */
    0, 0, 0,                     /* reserved */
    SCRIPT_SVC_SERVER_AUTO_START /* auto start */
);

/* ---------- Task impl ---------- */
CSP_DEFINE_TASK(script_service_task)
{
    scriptsvc_ctx_t *ctx = (scriptsvc_ctx_t *)param;
    csp_log_info("Script Service started, port=%u", ctx->port);

    /* เตรียม socket */
    csp_socket_t *sock = csp_socket(CSP_SO_NONE);
    if (!sock)
    {
        csp_log_error("SCRIPT: socket() fail");
        return CSP_TASK_RETURN;
    }

    /* bind แบบ reuse ได้ ถ้ามีคน bind ไว้ก่อน */
    if (csp_bind(sock, ctx->port) == CSP_ERR_USED)
    {
        csp_close(sock);
        sock = csp_port_get_socket(ctx->port);
        if (!sock)
        {
            csp_log_error("SCRIPT: port %u already used but no socket", ctx->port);
            return CSP_TASK_RETURN;
        }
    }
    else
    {
        csp_listen(sock, SCRIPT_SVC_CON_BACKLOG);
    }

    while (1)
    {

        thread_wd_kick_self();

        csp_conn_t *conn = csp_accept(sock, SCRIPT_SVC_ACCEPT_TIMEOUT_MS);
        if (conn == NULL)
        {
            continue;
        }

        csp_packet_t *packet = csp_read(conn, SCRIPT_SVC_READ_TIMEOUT_MS);
        if (packet != NULL)
        {
            char *data = (char *)packet->data;

            if (packet->length > 0 && data[packet->length - 1] == '\0')
            {
                script_process_remote_command(conn, data);
            }
            else
            {
                csp_log_warn("SCRIPT: received non-terminated cmd, len=%u",
                             packet->length);
            }

            csp_buffer_free(packet);
        }

        csp_close(conn);
    }

    return CSP_TASK_RETURN;
}

int script_service_init(void)
{
    (void)script_scheduler_init();

//    g_scriptsvc_ctx.port = SCRIPT_SERVICE_PORT;
//
//    int rc = csp_thread_create(script_service_task,
//                               SCRIPT_SVC_SERVER_NAME,
//                               SCRIPT_SVC_SERVER_STACK,
//                               &g_scriptsvc_ctx,
//                               SCRIPT_SVC_SERVER_PRIORITY,
//                               &g_scriptsvc_ctx.thread);
//    if (rc != CSP_ERR_NONE)
//    {
//        csp_log_error("SCRIPT: thread_create fail: %d", rc);
//        return rc;
//    }

    return CSP_ERR_NONE;
}


#endif /* ENABLE_SCRIPT_MANAGER */
