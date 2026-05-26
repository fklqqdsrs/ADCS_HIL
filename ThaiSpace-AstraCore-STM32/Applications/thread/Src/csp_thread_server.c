#include "app_modules_config.h"
#if CSP_ENABLE_THREAD

//! @file csp_thread_task.c
//! @brief C Source File Template

// --------------------------------------------------------------------------------------------------- //
// COMPANY NAME : IOT POW.
//
// INTRODUCTION : CSP thread task.
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

// ---------- (1) LOCAL PREDEFINE -------------------------------------------------------------------- //
#define __THIS_FILE_NAME__		"csp_thread_task.c"

// ---------- (2) STANDARD LIBRARY INCLUDE ----------------------------------------------------------- //
#include <kservice.h>
#include <errno.h>

#include <csp/arch/csp_thread.h>
#include <csp/arch/csp_malloc.h>
#include <csp_port.h>

// ---------- (3) USER INCLUDE ----------------------------------------------------------------------- //
#include <csp_thread_server.h>
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
	csp_thread_handle_t thread;
} csp_thread_server_context_t;

// ---------- (9) PRIVATE FUNCTION ------------------------------------------------------------------- //
CSP_DEFINE_TASK(csp_thread_server_task);

// ---------- (10) PRIVATE DATA ----------------------------------------------------------------------- //
static csp_thread_server_context_t def_thread_ctx = {.port = CSP_THREAD_MANAGER_PORT};

// ---------- (11) PRIVATE MACRO DEFINITION ---------------------------------------------------------- //
TSC_THREAD_REGISTER(CSP_THREAD_SERVER_NAME, csp_thread_server_task, 0, &def_thread_ctx, CSP_THREAD_SERVER_STACK, CSP_THREAD_SERVER_PRIORITY, 0, 0, 0, CSP_THREAD_SERVER_AUTO_START);

// ---------- (12) SOURCE FILE IMPLEMENTATION -------------------------------------------------------- //
/**
 * @brief CSP Thread Server Task.
 *
 * This task acts as a CSP thread server that handles incoming connections
 * and processes packets on the CSP_THREAD_PORT. It uses the CSP thread service
 * to handle time-related requests from clients.
 *
 * The task:
 * - Creates a CSP socket
 * - Binds to the CSP_THREAD_PORT
 * - Accepts incoming connections
 * - Processes packets and dispatches them to `csp_thread_service_handler()`
 * - Closes the connection after handling
 *
 * @param[in] param Pointer to a `csp_thread_server_context_t` structure.
 *
 * @return int Task return code (typically ignored).
 *
 * @note This task runs indefinitely inside a while loop.
 */
/* Thread Server task - handles requests from clients */
CSP_DEFINE_TASK(csp_thread_server_task) {
	csp_thread_server_context_t *csx = param;

	csp_log_info("Thread Server task started");

	/* Create socket with no specific socket options, e.g. accepts CRC32, HMAC, XTEA, etc. if enabled during compilation */
	csp_socket_t *sock = csp_socket(CSP_SO_NONE);

	/* Bind socket to all ports, e.g. all incoming connections will be handled here */
	if(csp_bind(sock, csx->port) == CSP_ERR_USED){
		csp_close(sock);
		sock = csp_port_get_socket(csx->port);
	}
	else {
		/* Create a backlog connections, i.e. up to CSP_THREAD_MANAGER_CON_MAX new connections can be queued */
		csp_listen(sock, CSP_THREAD_MANAGER_CON_MAX);
	}

	/* Wait for connections and then process packets on the connection */
	while (1) {
		/* Wait for a new connection, 1000 mS timeout */
		  thread_wd_kick_self();
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
				csp_thread_service_handler(conn, packet);
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
/* End of Thread Server task */

/**
 * @brief Initialize the CSP Thread Server.
 *
 * Allocates memory and starts the CSP thread server task thread.
 *
 * @param[in] server_port       The CSP port to use for the thread server.
 *
 * @return int
 * - `CSP_ERR_NONE` on successful initialization.
 * - `CSP_ERR_NOMEM` if memory allocation or thread creation fails.
 *
 * @note Uses dynamic memory allocation (`csp_calloc`) and must be freed if not used.
 * @note The parameters are stored but not actively used by the current task implementation.
 */
int csp_thread_server_init(uint8_t server_port) {

	csp_thread_server_context_t *csx = csp_calloc(1, sizeof(* csx));
	if (csx == NULL) {
		csp_log_error("%s: Error allocating context, errno: %s", __FUNCTION__, strerror(errno));
		return CSP_ERR_NOMEM;
	}

	csx->port = server_port;

	if (csp_thread_create(csp_thread_server_task, "ThreadServer", 512, csx, osPriorityAboveNormal, &csx->thread) != CSP_ERR_NONE) {
		csp_log_error("%s: csp_thread_create() failed to create ThreadServer thread, errno: %s", __FUNCTION__, strerror(errno));
		csp_free(csx);
		return CSP_ERR_NOMEM;
	}

	return CSP_ERR_NONE;
}


#ifdef TSC_USING_FINSH
static void __csp_thread_server_init(int argc, char **argv) {
	uint8_t port = CSP_THREAD_MANAGER_PORT;

	if (argc > 1) {
		port = atoi(argv[1]);

		if (port > CSP_ID_PORT_MAX)
			goto S_HELP;
	}

	csp_thread_server_init(port);
	return;
S_HELP:
	tsc_kprintf("Please input: thread_server [port]\r\n");
	tsc_kprintf("[port] must be in 0 - %d range.\r\n", CSP_ID_PORT_MAX);

}

/* -------------------------------------------------------------------------- */
/* Command Registration (CLI binding)                                         */
/* -------------------------------------------------------------------------- */
TSC_CMD_EXPORT_ALIAS(__csp_thread_server_init, thread_server, CSP Thread Server task.);

#endif

// ---------- (13) END OF SOURCE FILE IMPLEMENTATION ------------------------------------------------- //
// SOURCE FORMAT REVISION B


#endif /* CSP_ENABLE_THREAD */
