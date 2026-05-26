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

#ifndef _CSP_CTP_H_
#define _CSP_CTP_H_

/**
   @file
   CSP Time Protocol (CTP).
*/

#include <csp/arch/csp_clock.h>
#include <csp/csp.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   CSP Time type.
   @{
*/
/**
   CSP Time request.
*/
#define CSP_CTP_REQUEST 0x00
/**
   CSP Time reply.
*/
#define CSP_CTP_REPLY   0xff
/**@}*/

/**
   CSP Time requests.
   @{
*/
/**
   CSP Time request codes.
*/
/**
   Request time.
*/
#define CSP_CTP_GET_TIME 1
/**
   Set/configure time.
*/
#define CSP_CTP_SET_TIME 2
/**
   Request time status.
*/
#define CSP_CTP_STATUS 3
/**@}*/

/**
   CSP time status - max data length.
*/
#define CSP_CTP_TIME_STR_MAX_LEN  	26
#define CSP_CTP_MAIN_SOURCE_MAX_LEN  6

/**
   CSP time protocol description.
*/
struct csp_ctp_message {
        //! CMP request type.
        uint8_t type;
        //! CMP request code.
        uint8_t code;
	union {
		struct {
			char time_str[CSP_CTP_TIME_STR_MAX_LEN];
			char main_source[CSP_CTP_MAIN_SOURCE_MAX_LEN];
			csp_timestamp_t pps_time, rtc_time;
		} status;
		csp_timestamp_t get_time, set_time;
	};
} __attribute__ ((packed));

/**
   Macro for calculating total size of csp time message.
*/
#define CTP_SIZE(_memb) (sizeof(((struct csp_ctp_message *)0)->type) + sizeof(((struct csp_ctp_message *)0)->code) + sizeof(((struct csp_ctp_message *)0)->_memb))

/**
   Generic send csp time message request.
   @param[in] node address of subsystem.
   @param[in] timeout timeout in mS to wait for reply..
   @param[in] code request code.
   @param[in] msg_size size of \a msg.
   @param[in,out] msg data.
   @return #CSP_ERR_NONE on success, otherwise an error code.
*/
int csp_ctp(uint8_t node, uint32_t timeout, uint8_t code, int msg_size, struct csp_ctp_message *msg);

/**
   Macro for defining time handling function.
*/
#define CTP_MESSAGE(_code, _memb) \
static inline int csp_ctp_##_memb(uint8_t node, uint32_t timeout, struct csp_ctp_message *msg) { \
	return csp_ctp(node, timeout, _code, CTP_SIZE(_memb), msg); \
}

CTP_MESSAGE(CSP_CTP_GET_TIME, get_time)
CTP_MESSAGE(CSP_CTP_SET_TIME, set_time)
CTP_MESSAGE(CSP_CTP_STATUS, status)

/**
   Handle CSP ctp service request.
   If the given packet is a service-request (the destination port matches one of CSP service ports #csp_service_port_t),
   the packet will be processed by the specific CSP service handler.
   The packet will either process it or free it, so this function is typically called in the last "default" clause of
   a switch/case statement in a CSP listener task.
   In order to listen to csp service ports, bind your listener to the specific services ports #csp_service_port_t or
   use #CSP_ANY to all ports.
   @param[in] conn connection
   @param[in] packet first packet, obtained by using csp_read()
*/
void csp_ctp_service_handler(csp_conn_t * conn, csp_packet_t * packet);

#ifdef __cplusplus
}
#endif
#endif
