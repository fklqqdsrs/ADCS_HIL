//! @file csp_ctp_task.c
//! @brief C Source File Template

// --------------------------------------------------------------------------------------------------- //
// COMPANY NAME : IOT POW.
//
// INTRODUCTION : CSP time task.
//
// AUTHOR(S) : Pongpot Chaiboonuang
// CONTACT E-MAIL ADDRESS : pongpot.c@gmail.com
//
// REFERENCE DOCUMENT :
// N/A
//
// NOTE :
// N/A
//
// --------------------------------------------------------------------------------------------------- //

// ---------- I M P O R T A N T   N O T E ------------------------------------------------------------ //
//
// --------------------------------------------------------------------------------------------------- //
// N/A

#include "app_modules_config.h"
#if ENABLE_TIME_CTP

// ---------- (1) LOCAL PREDEFINE -------------------------------------------------------------------- //
#define __THIS_FILE_NAME__		"csp_ctp_task.c"

// ---------- (2) STANDARD LIBRARY INCLUDE ----------------------------------------------------------- //
#include <errno.h>
#include <kservice.h>

#include <csp/arch/csp_thread.h>
#include <csp/arch/csp_malloc.h>
#include <csp/arch/csp_time.h>
#include <csp_port.h>

// ---------- (3) USER INCLUDE ----------------------------------------------------------------------- //
#include <csp_ctp_server.h>
#include <csp_ctp_protocol.h>
#include <csp_thread_protocol.h>

// ---------- (4) EXTERN OBJECT ---------------------------------------------------------------------- //
// N/A

// ---------- (5) INTERFACE FUNCTION ---------------------------------------------------------- //
// N/A

// ---------- (6) DATA ------------------------------------------------------------------------ //
// N/A

// ---------- (7) PRIVATE PROGRAMMING DEFINE -------------------------------------------------------- //
// N/A

// ---------- (8) PRIVATE TYPEDEF -------------------------------------------------------------------- //
typedef struct {
	uint8_t port;
	uint32_t broadcast_period;
	csp_thread_handle_t thread;
} csp_ctp_server_context_t;

typedef struct {
	uint8_t node;
	uint32_t update_period;
	csp_thread_handle_t thread;
} csp_ctp_client_context_t;


// ---------- (9) PRIVATE FUNCTION ------------------------------------------------------------------- //
CSP_DEFINE_TASK(csp_ctp_server_task);
CSP_DEFINE_TASK(csp_ctp_client_task);

// ---------- (10) PRIVATE DATA ----------------------------------------------------------------------- //
static csp_ctp_server_context_t def_time_server_ctx = {.port = CSP_CTP_PORT, .broadcast_period = CSP_CTP_SERVER_BROADCAST_PERIOD};
static csp_ctp_client_context_t def_time_client_ctx = {.node = CSP_CTP_CLIENT_SERVER_ADDR, .update_period = CSP_CTP_CLIENT_UPDATE_PERIOD};

// ---------- (11) PRIVATE MACRO DEFINITION ---------------------------------------------------------- //
TSC_THREAD_REGISTER(CSP_CTP_SERVER_THREAD_NAME, csp_ctp_server_task, 0, &def_time_server_ctx, CSP_CTP_SERVER_THREAD_STACK, CSP_CTP_SERVER_THREAD_PRIORITY, 0, 0, 0, CSP_CTP_SERVER_THREAD_AUTO_START);
TSC_THREAD_REGISTER(CSP_CTP_CLIENT_THREAD_NAME, csp_ctp_client_task, 0, &def_time_client_ctx, CSP_CTP_CLIENT_THREAD_STACK, CSP_CTP_CLIENT_THREAD_PRIORITY, 0, 0, 0, CSP_CTP_CLIENT_THREAD_AUTO_START);

// ---------- (12) SOURCE FILE IMPLEMENTATION -------------------------------------------------------- //
/**
 * @brief CSP Time Server Task.
 *
 * This task acts as a CSP time server that handles incoming connections
 * and processes packets on the CSP_CTP_PORT. It uses the CSP CTP service
 * to handle time-related requests from clients.
 *
 * The task:
 * - Creates a CSP socket
 * - Binds to the CSP_CTP_PORT
 * - Accepts incoming connections
 * - Processes packets and dispatches them to `csp_ctp_service_handler()`
 * - Closes the connection after handling
 *
 * @param[in] param Pointer to a `csp_ctp_server_context_t` structure (unused in this function).
 *
 * @return int Task return code (typically ignored).
 *
 * @note This task runs indefinitely inside a while loop.
 */
/* Time Server task - handles requests from clients */
CSP_DEFINE_TASK(csp_ctp_server_task) {
	csp_ctp_server_context_t *csx = param;
	static uint32_t last_broadcast;

	csp_log_info("Time Server task started");

	/* Create socket with no specific socket options, e.g. accepts CRC32, HMAC, XTEA, etc. if enabled during compilation */
	csp_socket_t *sock = csp_socket(CSP_SO_NONE);

	/* Bind socket to all ports, e.g. all incoming connections will be handled here */
	if(csp_bind(sock, csx->port) == CSP_ERR_USED){
		csp_close(sock);
		sock = csp_port_get_socket(csx->port);
	}
	else {
		/* Create a backlog connections, i.e. up to CSP_CTP_CON_MAX new connections can be queued */
		csp_listen(sock, CSP_CTP_CON_MAX);
	}

	/* Wait for connections and then process packets on the connection */
	while (1) {
		thread_wd_kick_self();
		/* Broadcast time */
		if(((csp_get_ms() - last_broadcast) > csx->broadcast_period) && (csx->broadcast_period)){
			struct csp_ctp_message msg;
			csp_timestamp_t now;

			csp_clock_get_time(&now);
			msg.set_time = now;
			csp_ctp_set_time(255, 1000, &msg);
			last_broadcast = csp_get_ms();
		}

		/* Wait for a new connection, 1000 mS timeout */
		csp_conn_t *conn;
		if ((conn = csp_accept(sock, 1000)) == NULL) {
			/* timeout */
			continue;
		}

		/* Read packets on connection, timout is 100 mS */
		csp_packet_t *packet;
		while ((packet = csp_read(conn, 100)) != NULL) {
			if (csp_conn_dport(conn) == csx->port) {
				/* Call the CSP ctp time service handler, get time, set time, etc. */
				csp_ctp_service_handler(conn, packet);
			}
			else {
				break;
			}
		}

		/* Close current connection */
		csp_close(conn);

	}

	return CSP_TASK_RETURN;

}
/* End of Time Server task */

/**
 * @brief CSP Time Client Task.
 *
 * This task periodically sends time synchronization requests to a remote CSP Time Server
 * and updates the local system clock if a valid response is received.
 *
 * The task:
 * - Delays startup for 5 seconds
 * - Sends `csp_ctp_get_time()` requests to the specified node
 * - Validates and applies the received time via `csp_clock_set_time()`
 * - Waits for `update_period` before the next request
 *
 * @param[in] param Pointer to a `csp_ctp_client_context_t` structure containing:
 *   - `node` (destination node address)
 *   - `update_period` (interval between synchronization attempts in milliseconds)
 *
 * @return int Task return code (usually ignored).
 *
 * @note Uses cooperative delay via `csp_sleep_until_ms()` to reduce CPU usage.
 * @note Runs indefinitely in a loop.
 */
/* Time Client task - request time from Time Server */
CSP_DEFINE_TASK(csp_ctp_client_task) {
	csp_ctp_client_context_t *ccx = param;
	static long unsigned int last_update;

	csp_log_info("Time Client task started");

	/* Delay for first start */
	csp_sleep_ms(5000);
	last_update = csp_get_ms();

	/* Request time from time server periodically*/
	while (1) {
		/* Request time */
		struct csp_ctp_message msg;

		if(csp_ctp_get_time(ccx->node, 1000, &msg) != CSP_ERR_NONE) {
			csp_sleep_until_ms(&last_update, 5000);
			continue;
		}

		csp_timestamp_t curren_time = msg.get_time;

		if(csp_clock_set_time(&curren_time) != CSP_ERR_NONE) {
			csp_sleep_until_ms(&last_update, 5000);
			continue;
		}

		csp_sleep_until_ms(&last_update, ccx->update_period);
	}

	return CSP_TASK_RETURN;
}
/* End of Time Client task */

/**
 * @brief Initialize the CSP Time Server.
 *
 * Allocates memory and starts the CSP time server task thread.
 *
 * @param[in] server_port       The CSP port to use for the time server (currently unused).
 * @param[in] broadcast_period  The time interval for broadcasting (currently unused).
 *
 * @return int
 * - `CSP_ERR_NONE` on successful initialization.
 * - `CSP_ERR_NOMEM` if memory allocation or thread creation fails.
 *
 * @note Uses dynamic memory allocation (`csp_calloc`) and must be freed if not used.
 * @note The parameters are stored but not actively used by the current task implementation.
 */
int csp_ctp_server_init(uint8_t server_port, uint32_t broadcast_period) {

	csp_ctp_server_context_t *csx = csp_calloc(1, sizeof(* csx));
	if (csx == NULL) {
		csp_log_error("%s: Error allocating context, errno: %s", __FUNCTION__, strerror(errno));
		return CSP_ERR_NOMEM;
	}

	csx->port = server_port;
	csx->broadcast_period = broadcast_period;

	if (csp_thread_create(csp_ctp_server_task, "TimeServer", 512, csx, osPriorityAboveNormal, &csx->thread) != CSP_ERR_NONE) {
		csp_log_error("%s: csp_thread_create() failed to create TimeServer thread, errno: %s", __FUNCTION__, strerror(errno));
		csp_free(csx);
		return CSP_ERR_NOMEM;
	}

	return CSP_ERR_NONE;
}

/**
 * @brief Initialize the CSP Time Client.
 *
 * Allocates memory and starts a thread that periodically synchronizes time from a remote CSP Time Server.
 *
 * @param[in] node           The CSP node address of the Time Server to query.
 * @param[in] update_period  Time interval (in milliseconds) between synchronization attempts.
 *
 * @return int
 * - `CSP_ERR_NONE` on successful initialization.
 * - `CSP_ERR_NOMEM` if memory allocation or thread creation fails.
 *
 * @note Uses dynamic memory allocation (`csp_calloc`) to store client context.
 *       This context must persist for the lifetime of the client task.
 *
 */
int csp_ctp_client_init(uint8_t node, uint32_t update_period) {

	csp_ctp_client_context_t *ccx = csp_calloc(1, sizeof(* ccx));
	if (ccx == NULL) {
		csp_log_error("%s: Error allocating context, errno: %s", __FUNCTION__, strerror(errno));
		return CSP_ERR_NOMEM;
	}

	ccx->node = node;
	ccx->update_period = update_period;

	if (csp_thread_create(csp_ctp_client_task, "TimeClient", 512, ccx, osPriorityAboveNormal, &ccx->thread) != CSP_ERR_NONE) {
		csp_log_error("%s: csp_thread_create() failed to create TimeClient thread, errno: %s", __FUNCTION__, strerror(errno));
		csp_free(ccx);
		return CSP_ERR_NOMEM;
	}

	return CSP_ERR_NONE;
}

#ifdef TSC_USING_FINSH
static void __csp_ctp_server_init(int argc, char **argv) {
	uint8_t port = CSP_CTP_PORT;
	uint32_t broadcast_period = CSP_CTP_SERVER_BROADCAST_PERIOD;

	if (argc > 2) {
		broadcast_period = atol(argv[2]);

		if((broadcast_period != 0) && (broadcast_period < 1000))
			goto S_HELP;
	}

	if (argc > 1) {
		port = atoi(argv[1]);

		if (port > CSP_ID_PORT_MAX)
			goto S_HELP;
	}

	csp_ctp_server_init(port, broadcast_period);
	return;
S_HELP:
	tsc_kprintf("Please input: time_server [port] [broadcast_period]\r\n");
	tsc_kprintf("[port] must be in 0 - %d range.\r\n", CSP_ID_PORT_MAX);
	tsc_kprintf("[broadcast_period] 0 = disable or more than 1000 mS \r\n");;

}


static void __csp_ctp_client_init(int argc, char **argv) {
	uint8_t node = 0;
	uint32_t update_period = CSP_CTP_CLIENT_UPDATE_PERIOD;

	if (argc > 2) {
		update_period = atol(argv[2]);

		if(update_period < 1000)
			goto C_HELP;
	}

	if (argc > 1) {
		node = atoi(argv[1]);

		if (node > CSP_ID_HOST_MAX)
			goto C_HELP;
	}
	else {
		goto C_HELP;
	}

	csp_ctp_client_init(node, update_period);
	return;
C_HELP:
	tsc_kprintf("Please input: time_client <node> [update_period]\r\n");
	tsc_kprintf("<node> must be in 0 - %d range.\r\n", CSP_ID_HOST_MAX);
	tsc_kprintf("[update_period] must more than 1000 mS \r\n");
}

/* -------------------------------------------------------------------------- */
/* Command Registration (CLI binding)                                         */
/* -------------------------------------------------------------------------- */
TSC_CMD_EXPORT_ALIAS(__csp_ctp_server_init, time_server, CSP Time Server task.);
TSC_CMD_EXPORT_ALIAS(__csp_ctp_client_init, time_client, CSP Time Client task.);
#endif

#endif

// ---------- (13) END OF SOURCE FILE IMPLEMENTATION ------------------------------------------------- //
// SOURCE FORMAT REVISION B
