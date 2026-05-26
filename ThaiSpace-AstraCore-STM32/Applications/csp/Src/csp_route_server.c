#define __THIS_FILE_NAME__   "csp_route_server.c"

/* ---------- Standard / 3rd-party includes ---------- */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <kservice.h>

#include <app_modules_config.h>

#include <csp/csp.h>
#include <csp/arch/csp_system.h>
#include <csp/arch/csp_thread.h>
#include <csp/csp_debug.h>
#include <csp/csp_error.h>
#include <csp/csp_types.h>
#include <csp_port.h>                    /* csp_port_get_socket */

/* Interfaces present in your project */
#if ENABLE_HW_UART
#include <csp/drivers/usart.h>           /* KISS over UART */
#endif

/* SocketCAN is POSIX-only: do NOT include on STM32 unless explicitly enabled */
#if ENABLE_HW_CAN
#include <csp/drivers/can_socketcan.h>
#endif

#if ENABLE_HW_I2C
#include <csp/drivers/i2c.h>
#endif

/* ---------- User includes ---------- */
#include <csp_route_server.h>
#include <thread_manager_core.h>
#include <config_core.h>
#include <csp_route_core.h>

/* ---------- Forward declares ---------- */
CSP_DEFINE_TASK(csp_default_server);

/* ---------- Globals / module-scope data ---------- */
static csp_debug_level_t debug_level = CSP_INFO;

/* device-name strings used by CSP drivers (const for safety) */
static const char *kiss_device = NULL;
static const char *can_device = NULL;
static const char *i2c_device = NULL;

static bool g_promisc = false;
/* Make route-table buffer global (no static) so other C files (e.g. csp_route.c) can link */
char rtable_data[256] = { 0 };

/* ---------- Framework registration ---------- */
TSC_THREAD_REGISTER(CSP_DEFAULT_SERVER_NAME, csp_default_server, 0, NULL, CSP_DEFAULT_SERVER_STACK, CSP_DEFAULT_SERVER_PRIORITY, 0, 0, 0,
		CSP_DEFAULT_SERVER_AUTO_START);

/* Expose route_table string to config system (default empty) */
TSC_CONFIG_ADD_STRING(route_table, CFG_PERM_RW, CFG_ACCESS_MENU, rtable_data, /*default*/);

/* ---------- Helpers ---------- */


/* ---------- Public: init & start (use csp_conf_get_defaults) ---------- */

static int add_all_enabled_ifaces(void) {
    int err = CSP_ERR_NONE;
    csp_iface_t *ok_iface = NULL;
    /* --- KISS over UART --- */
#if ENABLE_HW_UART
    if (kiss_device != NULL) {
        const csp_usart_conf_t uconf = {
            .device        = kiss_device,
            .baudrate      = 921600,
            .databits      = 8,
            .stopbits      = 1,
            .paritysetting = 0,
            .checkparity   = 0
        };

        csp_log_info("Adding KISS iface on '%s'", kiss_device);
        err = csp_usart_open_and_add_kiss_interface(&uconf,
                                                    CSP_IF_KISS_DEFAULT_NAME,
                                                    &ok_iface);
        if (err != CSP_ERR_NONE) {
            csp_log_error("Add KISS iface failed: %d", err);
            return err;
        }
    }
#endif

    /* --- CAN interface --- */
#if ENABLE_HW_CAN
    if (can_device != NULL) {

        csp_log_info("Adding CAN (socketcan) iface on '%s'", can_device);
        err = csp_can_socketcan_open_and_add_interface(can_device,
                                                       CSP_IF_CAN_DEFAULT_NAME,
                                                       1000000,     /* bitrate */
                                                       g_promisc,  /* promiscuous สำหรับโหนด router */
                                                       &ok_iface);
        if (err != CSP_ERR_NONE) {
            csp_log_error("Add CAN iface failed: %d", err);
            return err;
        }
    }
    if (can_device == NULL) {
        csp_log_warn("CAN device set but no CAN driver enabled; define CSP_USE_SOCKETCAN or add MCU CAN driver.");
    }
#endif

    /* --- I2C interface --- */
#if ENABLE_HW_I2C
    if (i2c_device != NULL) {

        int e2 = csp_i2c_open_and_add_interface(i2c_device,
                                                CSP_IF_I2C_DEFAULT_NAME,
                                                256,
                                                &ok_iface);
        if (e2 != CSP_ERR_NONE) {
            csp_log_error("failed to add I2C interface [%s], error: %d", i2c_device, e2);
            return e2;
        }
    }
#endif

    return CSP_ERR_NONE;
}

/* ---------- System reset hook ---------- */
static int system_reset(void) {
	NVIC_SystemReset();
	return CSP_ERR_NONE;
}

/* ---------- CSP server task ---------- */
CSP_DEFINE_TASK(csp_default_server) {
	static unsigned int server_received = 0U;
	csp_log_info("Server task started");

	csp_socket_t *sock = csp_socket(CSP_SO_NONE);

	if (csp_bind(sock, CSP_ANY) == CSP_ERR_USED) {
		csp_close(sock);
		sock = csp_port_get_socket(CSP_ANY);
	} else {
		(void) csp_listen(sock, 10);
	}

	for (;;) {
		thread_wd_kick_self();
		csp_conn_t *conn = csp_accept(sock, 10000); /* 10 s */
		if (conn == NULL) {
			continue;
		}

		csp_packet_t *packet;
		while ((packet = csp_read(conn, 50)) != NULL) {
			switch (csp_conn_dport(conn)) {
			case CSP_DEFAULT_PORT:
				csp_log_info("Packet on CSP_DEFAULT_PORT: %s", (char* )packet->data)
				;
				csp_buffer_free(packet);
				++server_received;
				break;

			case CSP_ROUTE_MANAGER_PORT:
				csp_route_service_handler(conn, packet);
				break;

			default:
				csp_service_handler(conn, packet);
				break;
			}
		}
		csp_close(conn);
	}
	/* not reached */
	/* return CSP_TASK_RETURN; */
}

/* ---------- Public: init & start ---------- */
int csp_task_init(void) {
    for (csp_debug_level_t i = CSP_ERROR; i <= CSP_LOCK; ++i) {
        csp_debug_set_level(i, (i <= debug_level));
    }
    csp_log_info("Initialising CSP (with csp_conf_get_defaults)");

    csp_sys_set_reboot(system_reset);
    csp_sys_set_shutdown(system_reset);
#if ENABLE_HW_UART
    kiss_device = TSC_CSP_UART1_NAME;
#endif
#if ENABLE_HW_CAN
    can_device = TSC_CSP_CAN1_NAME;
#endif
#if ENABLE_HW_I2C
    i2c_device = TSC_CSP_I2C1_NAME;
#endif

    csp_conf_t csp_conf;
    csp_conf_get_defaults(&csp_conf);
    csp_conf.address  = 11;
    csp_conf.conn_max = 30;
    csp_conf.buffers = 20;

    int err = csp_init(&csp_conf);
    if (err != CSP_ERR_NONE) {
        csp_log_error("csp_init() failed: %d", err);
        return CSP_ERR_INVAL;
    }

    csp_route_start_task(1024, osPriorityHigh);

    err = add_all_enabled_ifaces();
    if (err != CSP_ERR_NONE) {
        return err;
    }

    const char *rtable = NULL;

    rtable = "0/0CAN";


    if ((rtable != NULL) && (rtable[0] != '\0')) {
        int n_loaded = csp_rtable_load(rtable);
        if (n_loaded < 1) {
            csp_log_error("csp_rtable_load(%s) failed, error: %d", rtable, n_loaded);
            return CSP_ERR_INVAL;
        }
    }

    tsc_kprintf("Connection table\r\n");
    csp_conn_print_table();

    tsc_kprintf("Interfaces\r\n");
    csp_route_print_interfaces();

    tsc_kprintf("Route table\r\n");
    csp_route_print_table();

    return CSP_ERR_NONE;
}

void csp_task_start(void) {
	thread_start(CSP_DEFAULT_SERVER_NAME);
}

/* ---------- CLI: send ---------- */
static void csp_send_cmd(int argc, char **argv) {
	if (argc < 5) {
		tsc_kprintf("Usage: csp_send <dst_addr> <dst_port> <UDP|RDP> <message>\r\n");
		return;
	}

	const uint16_t dst = (uint16_t) atoi(argv[1]);
	const uint16_t dport = (uint16_t) atoi(argv[2]);
	const char *proto = argv[3];
	const char *msg = argv[4];

	const int prio = CSP_PRIO_NORM;
	const int timeout = 1000;
	int flags = 0;

	if (strcasecmp(proto, "RDP") == 0) {
		flags |= CSP_O_RDP;
	}

	csp_conn_t *conn = csp_connect(prio, dst, dport, timeout, flags);
	if (conn == NULL) {
		tsc_kprintf("CSP connect failed!\r\n");
		return;
	}

	const size_t len = strlen(msg) + 1U;
	csp_packet_t *packet = csp_buffer_get(len);
	if (packet == NULL) {
		tsc_kprintf("CSP buffer alloc failed!\r\n");
		csp_close(conn);
		return;
	}

	(void) strcpy((char*) packet->data, msg);
	packet->length = (uint16_t) len;

	if (!csp_send(conn, packet, timeout)) {
		tsc_kprintf("CSP send failed!\r\n");
		csp_buffer_free(packet);
	} else {
		tsc_kprintf("CSP send OK to %u:%u (%s)\r\n", (unsigned) dst, (unsigned) dport, proto);
	}

	csp_close(conn);
}

/* ---------- CLI: read ---------- */
static void csp_read_cmd(int argc, char **argv) {
	if (argc < 3) {
		tsc_kprintf("Usage: csp_read <listen_port> <timeout_sec>\r\n");
		return;
	}

	const uint16_t port = (uint16_t) atoi(argv[1]);
	const int timeout = atoi(argv[2]) * 1000;

	csp_socket_t *sock = csp_socket(CSP_SO_NONE);
	if (sock == NULL) {
		tsc_kprintf("CSP socket create failed!\r\n");
		return;
	}

	if (csp_bind(sock, port) != CSP_ERR_NONE) {
		tsc_kprintf("CSP bind failed!\r\n");
		csp_close(sock);
		return;
	}

	if (csp_listen(sock, 5) != CSP_ERR_NONE) {
		tsc_kprintf("CSP listen failed!\r\n");
		csp_close(sock);
		return;
	}

	tsc_kprintf("CSP server listening on port %u for %d sec\r\n", (unsigned) port, (timeout / 1000));

	const uint32_t start = osKernelGetTickCount();
	while ((osKernelGetTickCount() - start) < (uint32_t) timeout) {
		csp_conn_t *conn = csp_accept(sock, 1000);
		if (conn == NULL) {
			continue;
		}

		csp_packet_t *packet;
		while ((packet = csp_read(conn, 100)) != NULL) {
			tsc_kprintf("Received from %u:%u -> %s\r\n", (unsigned) csp_conn_src(conn), (unsigned) csp_conn_sport(conn), (char*) packet->data);
			csp_buffer_free(packet);
		}

		csp_close(conn);
	}

	csp_close(sock);
	tsc_kprintf("CSP server closed\r\n");
}

/* -------------------------------------------------------------------------- */
/* Command Registration (CLI binding)                                         */
/* -------------------------------------------------------------------------- */
#ifdef TSC_USING_FINSH
TSC_CMD_EXPORT(csp_send_cmd, "CSP send <dst_addr> <dst_port> <UDP|RDP> <message>");
TSC_CMD_EXPORT(csp_read_cmd, "CSP read <listen_port> <timeout_sec>");
#endif
