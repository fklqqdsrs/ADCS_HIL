/*
 * script_manager.h
 *
 *  Created on: Aug 5, 2024
 *      Author: chaturong
 */

#ifndef SCRIPT_MANAGER_H_
#define SCRIPT_MANAGER_H_

#include <csp/csp.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Sends a command to a remote node and waits for a response.
 * 
 * @param node The destination CSP address.
 * @param command The command string to send (e.g., "RUN script.txt").
 * @return int 0 on success, -1 on failure.
 */
int script_send_remote_command(int node, const char *command);

/**
 * @brief Processes a command string received from a remote node via CSP.
 * This function will parse the command and execute it, then send a response.
 * 
 * @param conn The active CSP connection to send the response back on.
 * @param remote_command The full command string from the CSP packet.
 */
void script_process_remote_command(csp_conn_t *conn, const char *remote_command);

/**
 * @brief Executes a script file locally.
 * @param filename The name of the script file to run.
 */
void script_run_local(const char *filename);

/**
 * @brief Starts a looping execution of a script file locally.
 * @param args A string containing the filename and optional delay.
 */
void script_loop_local(const char *args);

/**
 * @brief Stops the currently running local script loop.
 * @param args Not used.
 */
void script_stop_local(const char *args);

#ifdef __cplusplus
}
#endif

#endif /* SCRIPT_MANAGER_H_ */
