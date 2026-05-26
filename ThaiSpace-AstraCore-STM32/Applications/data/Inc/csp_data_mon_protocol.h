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

#ifndef CSP_DATA_MON_PROTOCOL_H
#define CSP_DATA_MON_PROTOCOL_H

/**
   @file
   CSP Data monitor protocol.
*/

#ifdef __cplusplus
extern "C" {
#endif

#include "data_monitor_core.h"

/**
   CSP Data Mon type.
   @{
*/
/**
   CSP Data Mon request.
*/
#define CSP_DATA_MON_REQUEST 0x00
/**
   CSP Data Mon reply.
*/
#define CSP_DATA_MON_REPLY   0xff
/**@}*/

/**
   CSP Data Mon requests.
   @{
*/
/**
   CSP Data Mon request codes.
*/
/**
   Start data monitor.
*/
#define CSP_DATA_MON_START 			1
/**
   Stop data monitor.
*/
#define CSP_DATA_MON_STOP 			2
/**
   Get data monitor.
*/
#define CSP_DATA_MON_GET_DATA 		3
/**
   List all registered data monitor.
*/
#define CSP_DATA_MON_LIST			4
/**
   Get data monitor status.
*/
#define CSP_DATA_MON_STATUS 		5
/**
   Get data monitor status.
*/
#define CSP_DATA_MON_CONFIG_STATUS	6

/**
   Set data monitor value (single config entry).
*/
#define CSP_DATA_MON_SET_DATA		7

/**@}*/


/**
   CSP data monitor protocol description.
*/
struct csp_data_mon_message {
        //! CSP data monitor request type.
        uint8_t type;
        //! CSP data monitor request code.
        uint8_t code;
	union {
		struct {
			char name[CSP_DATA_MONITOR_NAME_LENGTH];
			char status[CSP_DATA_MONITOR_RPS_MTU - CSP_DATA_MONITOR_NAME_LENGTH];
		} start, stop, status;
		struct {
			char dm_name[CSP_DATA_MONITOR_NAME_LENGTH];
			char dmc_name[CSP_DATA_MONITOR_NAME_LENGTH];
			cfg_type_t type;
			triple_storage_t storage;
		} get_data;
		struct {
			char dm_name[CSP_DATA_MONITOR_NAME_LENGTH];
			char dmc_name[CSP_DATA_MONITOR_NAME_LENGTH];
			cfg_type_t type;
			uint8_t len;
			uint8_t raw[DATA_MONITOR_MAX_DATA_BYTES];
			char status[CSP_DATA_MONITOR_RPS_MTU - (CSP_DATA_MONITOR_NAME_LENGTH*2) - 2 - DATA_MONITOR_MAX_DATA_BYTES];
		} set_data;
		struct {
			char data[CSP_DATA_MONITOR_RPS_MTU];
		} list;
		struct {
			char dm_name[CSP_DATA_MONITOR_NAME_LENGTH];
			char dmc_name[CSP_DATA_MONITOR_NAME_LENGTH];
			char status[CSP_DATA_MONITOR_RPS_MTU - (CSP_DATA_MONITOR_NAME_LENGTH*2)];
		} config_status;
	};
} __attribute__ ((packed));

/**
   Macro for calculating total size of csp data monitor message.
*/
#define CSP_DATA_MON_SIZE(_memb) (sizeof(((struct csp_data_mon_message *)0)->type) + sizeof(((struct csp_data_mon_message *)0)->code) + sizeof(((struct csp_data_mon_message *)0)->_memb))

/**
   Generic send csp data monitor message request.
   @param[in] node address of subsystem.
   @param[in] timeout timeout in mS to wait for reply..
   @param[in] code request code.
   @param[in] msg_size size of \a msg.
   @param[in,out] msg data.
   @return #CSP_ERR_NONE on success, otherwise an error code.
*/
int csp_data_mon(uint8_t node, uint32_t timeout, uint8_t code, int msg_size, struct csp_data_mon_message *msg);

/**
   Macro for defining time handling function.
*/
#define CSP_DATA_MON_MESSAGE(_code, _memb) \
static inline int csp_data_mon_##_memb(uint8_t node, uint32_t timeout, struct csp_data_mon_message *msg) { \
	return csp_data_mon(node, timeout, _code, CSP_DATA_MON_SIZE(_memb), msg); \
}

CSP_DATA_MON_MESSAGE(CSP_DATA_MON_START, start)
CSP_DATA_MON_MESSAGE(CSP_DATA_MON_STOP, stop)
CSP_DATA_MON_MESSAGE(CSP_DATA_MON_GET_DATA, get_data)
CSP_DATA_MON_MESSAGE(CSP_DATA_MON_SET_DATA, set_data)

void csp_data_mon_list(uint8_t node, uint32_t timeout);
void csp_data_mon_status(uint8_t node, uint32_t timeout, const char *dm_name);
void csp_data_mon_config_status(uint8_t node, uint32_t timeout, const char *dm_name, const char *dmc_name);

/**
   Handle CSP data monitor service request.
   If the given packet is a service-request (the destination port matches one of CSP service ports #csp_service_port_t),
   the packet will be processed by the specific CSP service handler.
   The packet will either process it or free it, so this function is typically called in the last "default" clause of
   a switch/case statement in a CSP listener task.
   In order to listen to csp service ports, bind your listener to the specific services ports #csp_service_port_t or
   use #CSP_ANY to all ports.
   @param[in] conn connection
   @param[in] packet first packet, obtained by using csp_read()
*/
void csp_data_mon_service_handler(csp_conn_t * conn, csp_packet_t * packet);

#ifdef __cplusplus
}
#endif
#endif
