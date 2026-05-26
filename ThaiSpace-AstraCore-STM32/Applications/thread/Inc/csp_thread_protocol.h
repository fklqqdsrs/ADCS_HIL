/*
Cubesat Space Protocol - A small network-layer protocol designed for Cubesats
Copyright (C) 2012 Gomspace ApS (http://www.gomspace.com)
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

#ifndef _CSP_THREAD_MANAGER_H_
#define _CSP_THREAD_MANAGER_H_

/**
   @file
   CSP Thread manager protocol.
*/

#include <thread_manager_core.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   CSP Thread type.
   @{
*/
/**
   CSP Thread request.
*/
#define CSP_THREAD_REQUEST 0x00
/**
   CSP Thread reply.
*/
#define CSP_THREAD_REPLY   0xff
/**@}*/

/**
   CSP Thread requests.
   @{
*/
/**
   CSP Thread request codes.
*/
/**
   Start thread.
*/
#define CSP_THREAD_START 		1
/**
   Stop thread.
*/
#define CSP_THREAD_STOP 		2
/**
   Restart thread.
*/
#define CSP_THREAD_RESTART 		3
/**
   Get thread status.
*/
#define CSP_THREAD_STATUS 		4
/**
   Get thread status.
*/
#define CSP_THREAD_STATUS_ALL	5
/**
   Config thread.
*/
#define CSP_THREAD_CONFIG 		6
/**
   List all registered thread.
*/
#define CSP_THREAD_LIST			7
/**@}*/

/**
   CSP time status - max data length.
*/
#define CSP_CTP_TIME_STR_MAX_LEN  	26
#define CSP_CTP_MAIN_SOURCE_MAX_LEN  6

/**
   CSP thread protocol description.
*/
struct csp_thread_message {
        //! CSP thread request type.
        uint8_t type;
        //! CSP thread request code.
        uint8_t code;
	union {
		struct {
			char name[CSP_THREAD_MANAGER_NAME_LENGTH];
			char status[CSP_THREAD_MANAGER_RPS_MTU - CSP_THREAD_MANAGER_NAME_LENGTH];
		} start, stop, restart;
        thread_status_runtime_t status;
		thread_config_runtime_t config;
		struct {
			uint16_t length;
			char data[CSP_THREAD_MANAGER_RPS_MTU];
		} status_all;
	};
} __attribute__ ((packed));

/**
   Macro for calculating total size of csp thread message.
*/
#define CSP_THREAD_SIZE(_memb) (sizeof(((struct csp_thread_message *)0)->type) + sizeof(((struct csp_thread_message *)0)->code) + sizeof(((struct csp_thread_message *)0)->_memb))

/**
   Generic send csp thread message request.
   @param[in] node address of subsystem.
   @param[in] timeout timeout in mS to wait for reply..
   @param[in] code request code.
   @param[in] msg_size size of \a msg.
   @param[in,out] msg data.
   @return #CSP_ERR_NONE on success, otherwise an error code.
*/
int csp_thread(uint8_t node, uint32_t timeout, uint8_t code, int msg_size, struct csp_thread_message *msg);

/**
   Macro for defining time handling function.
*/
#define CSP_THREAD_MESSAGE(_code, _memb) \
static inline int csp_thread_##_memb(uint8_t node, uint32_t timeout, struct csp_thread_message *msg) { \
	return csp_thread(node, timeout, _code, CSP_THREAD_SIZE(_memb), msg); \
}

CSP_THREAD_MESSAGE(CSP_THREAD_START, start)
CSP_THREAD_MESSAGE(CSP_THREAD_STOP, stop)
CSP_THREAD_MESSAGE(CSP_THREAD_RESTART, restart)
CSP_THREAD_MESSAGE(CSP_THREAD_STATUS, status)
CSP_THREAD_MESSAGE(CSP_THREAD_CONFIG, config)

void csp_thread_status_all(uint8_t node, uint32_t timeout);
void csp_thread_list(uint8_t node, uint32_t timeout);

/**
   Handle CSP thread service request.
   If the given packet is a service-request (the destination port matches one of CSP service ports #csp_service_port_t),
   the packet will be processed by the specific CSP service handler.
   The packet will either process it or free it, so this function is typically called in the last "default" clause of
   a switch/case statement in a CSP listener task.
   In order to listen to csp service ports, bind your listener to the specific services ports #csp_service_port_t or
   use #CSP_ANY to all ports.
   @param[in] conn connection
   @param[in] packet first packet, obtained by using csp_read()
*/
void csp_thread_service_handler(csp_conn_t * conn, csp_packet_t * packet);

#ifdef __cplusplus
}
#endif
#endif
