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

#ifndef _CSP_ROUTE_MANAGER_H_
#define _CSP_ROUTE_MANAGER_H_

/**
   @file
   CSP route protocol.
*/

#ifdef __cplusplus
extern "C" {
#endif

/**
   CSP Route type.
   @{
*/
/**
   CSP Route request.
*/
#define CSP_ROUTE_REQUEST 0x00
/**
   CSP Route reply.
*/
#define CSP_ROUTE_REPLY   0xff
/**@}*/

/**
   CSP Route requests.
   @{
*/
/**
   CSP Route request codes.
*/
/**
   Load route table.
*/
#define CSP_ROUTE_LOAD 			1
/**
   Check route table.
*/
#define CSP_ROUTE_CHECK 		2
/**
   Set new route.
*/
#define CSP_ROUTE_ADD 			3
/**
   Save route table.
*/
#define CSP_ROUTE_SAVE 			4
/**
   Clear Route table.
*/
#define CSP_ROUTE_CLEAR			5
/**
   Show route table.
*/
#define CSP_ROUTE_STATUS 		6
/**@}*/

/**
   CSP Route protocol description.
*/
struct csp_route_message {
        //! CSP Route request type.
        uint8_t type;
        //! CSP Route request code.
        uint8_t code;
	union {
        int load, check, save, clear;
		struct {
        	uint8_t address;
        	uint8_t netmask;
        	uint8_t via;
        	char iface_name[CSP_IFLIST_NAME_MAX + 1];

        	int error;
		} add;
		struct {
			char data[CSP_ROUTE_MANAGER_RPS_MTU];
		} status, response;
	};
} __attribute__ ((packed));

/**
   Macro for calculating total size of csp Route message.
*/
#define CSP_ROUTE_SIZE(_memb) (sizeof(((struct csp_route_message *)0)->type) + sizeof(((struct csp_route_message *)0)->code) + sizeof(((struct csp_route_message *)0)->_memb))

/**
   Generic send csp Route message request.
   @param[in] node address of subsystem.
   @param[in] timeout timeout in mS to wait for reply..
   @param[in] code request code.
   @param[in] msg_size size of \a msg.
   @param[in,out] msg data.
   @return #CSP_ERR_NONE on success, otherwise an error code.
*/
int csp_route(uint8_t node, uint32_t timeout, uint8_t code, int msg_size, struct csp_route_message *msg);

/**
   Macro for defining time handling function.
*/
#define CSP_ROUTE_MESSAGE(_code, _memb) \
static inline int csp_route_##_memb(uint8_t node, uint32_t timeout, struct csp_route_message *msg) { \
	return csp_route(node, timeout, _code, CSP_ROUTE_SIZE(_memb), msg); \
}

CSP_ROUTE_MESSAGE(CSP_ROUTE_LOAD, load)
CSP_ROUTE_MESSAGE(CSP_ROUTE_CHECK, check)
CSP_ROUTE_MESSAGE(CSP_ROUTE_ADD, add)
CSP_ROUTE_MESSAGE(CSP_ROUTE_SAVE, save)
CSP_ROUTE_MESSAGE(CSP_ROUTE_CLEAR, clear)
CSP_ROUTE_MESSAGE(CSP_ROUTE_STATUS, status)

//void csp_route_status_all(uint8_t node, uint32_t timeout);
//void csp_route_list(uint8_t node, uint32_t timeout);

/**
   Handle CSP Route service request.
   If the given packet is a service-request (the destination port matches one of CSP service ports #csp_service_port_t),
   the packet will be processed by the specific CSP service handler.
   The packet will either process it or free it, so this function is typically called in the last "default" clause of
   a switch/case statement in a CSP listener task.
   In order to listen to csp service ports, bind your listener to the specific services ports #csp_service_port_t or
   use #CSP_ANY to all ports.
   @param[in] conn connection
   @param[in] packet first packet, obtained by using csp_read()
*/
void csp_route_service_handler(csp_conn_t * conn, csp_packet_t * packet);

#ifdef __cplusplus
}
#endif
#endif
