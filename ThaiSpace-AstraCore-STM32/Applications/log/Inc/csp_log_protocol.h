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

#ifndef _CSP_LOG_H_
#define _CSP_LOG_H_

/**
   @file
   CSP Log protocol.
*/
#include <csp/csp.h>
#include "tsc_log_def.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
   CSP Log type.
   @{
*/
/**
   CSP Log request.
*/
#define CSP_LOG_REQUEST 0x00
/**
   CSP Log reply.
*/
#define CSP_LOG_REPLY   0xff
/**@}*/

/**
   CSP Log requests.
   @{
*/
/**
   CSP Log request codes.
*/
/**
   Set log backend level.
*/
#define CSP_LOG_BE_LEVEL		1
/**
   Set log tag level.
*/
#define CSP_LOG_TAG_LEVEL 		2
/**
   Set global log level.
*/
#define CSP_LOG_LEVEL	 		3
/**
   Set log tag filter.
*/
#define CSP_LOG_TAG		 		4
/**
   Set log keyword filter.
*/
#define CSP_LOG_KEYWORD			5
/**
   Get log filter status.
*/
#define CSP_LOG_FILTER	 		6
/**@}*/

/**
   CSP log status - max data length.
*/
#define CSP_CTP_TIME_STR_MAX_LEN  	26
#define CSP_CTP_MAIN_SOURCE_MAX_LEN  6

/**
   CSP log protocol description.
*/
struct csp_log_message {
        //! CSP log request type.
        uint8_t type;
        //! CSP log request code.
        uint8_t code;
	union {
		struct {
			char be_name[TSC_NAME_MAX];
			uint32_t level;
		} be_level;
		struct {
			char tag_name[TSC_LOG_FILTER_TAG_MAX_LEN];
			uint32_t level;
		} tag_level;
		uint32_t level;
		char tag[TSC_LOG_FILTER_TAG_MAX_LEN];
		char keyword[TSC_LOG_FILTER_KW_MAX_LEN];
		char filter[CSP_LOG_MANAGER_RPS_MTU];
	};
} __attribute__ ((packed));

/**
   Macro for calculating total size of csp thread message.
*/
#define CSP_LOG_SIZE(_memb) (sizeof(((struct csp_log_message *)0)->type) + sizeof(((struct csp_log_message *)0)->code) + sizeof(((struct csp_log_message *)0)->_memb))

/**
   Generic send csp log message request.
   @param[in] node address of subsystem.
   @param[in] timeout timeout in mS to wait for reply..
   @param[in] code request code.
   @param[in] msg_size size of \a msg.
   @param[in,out] msg data.
   @return #CSP_ERR_NONE on success, otherwise an error code.
*/
int csp_log(uint8_t node, uint32_t timeout, uint8_t code, int msg_size, struct csp_log_message *msg);

/**
   Macro for defining log handling function.
*/
#define CSP_LOG_MESSAGE(_code, _memb) \
static inline int csp_log_##_memb(uint8_t node, uint32_t timeout, struct csp_log_message *msg) { \
	return csp_log(node, timeout, _code, CSP_LOG_SIZE(_memb), msg); \
}

CSP_LOG_MESSAGE(CSP_LOG_BE_LEVEL, be_level)
CSP_LOG_MESSAGE(CSP_LOG_TAG_LEVEL, tag_level)
CSP_LOG_MESSAGE(CSP_LOG_LEVEL, level)
CSP_LOG_MESSAGE(CSP_LOG_TAG, tag)
CSP_LOG_MESSAGE(CSP_LOG_KEYWORD, keyword)

void csp_log_filter(uint8_t node, uint32_t timeout);

/**
   Handle CSP log service request.
   If the given packet is a service-request (the destination port matches one of CSP service ports #csp_service_port_t),
   the packet will be processed by the specific CSP service handler.
   The packet will either process it or free it, so this function is typically called in the last "default" clause of
   a switch/case statement in a CSP listener task.
   In order to listen to csp service ports, bind your listener to the specific services ports #csp_service_port_t or
   use #CSP_ANY to all ports.
   @param[in] conn connection
   @param[in] packet first packet, obtained by using csp_read()
*/
void csp_log_service_handler(csp_conn_t * conn, csp_packet_t * packet);

#ifdef __cplusplus
}
#endif
#endif
