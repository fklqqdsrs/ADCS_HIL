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

#ifndef _CSP_CONFIG_PROTOCOL_H_
#define _CSP_CONFIG_PROTOCOL_H_

/**
 @file
 CSP Config Protocol (CCP).
 */

#include <config_storage.h>
#include <csp/csp.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 CSP Config type.
 @{
 */
/**
 CSP Config request.
 */
#define CSP_CCP_REQUEST 0x00
/**
 CSP Config reply.
 */
#define CSP_CCP_REPLY   0xff
/**@}*/

/**
 CSP Config requests.
 @{
 */
/**
 CSP Config request codes.
 */
/**
 Load Config.
 */
#define CSP_CCP_LOAD 1
/**
 Save Config.
 */
#define CSP_CCP_SAVE 2
/**
 Load Default Config.
 */
#define CSP_CCP_LOAD_DFAULT 3
/**
 Get config.
 */
#define CSP_CCP_GET_CONFIG 4
/**
 Remove Config.
 */
#define CSP_CCP_REMOVE_CONFIG 5
/**
 Edit Config value.
 */
#define CSP_CCP_EDIT_VALUE 6
/**
 Edit Config min range.
 */
#define CSP_CCP_EDIT_MIN 7
/**
 Edit Config max range.
 */
#define CSP_CCP_EDIT_MAX 8
/**
 List all config name.
 */
#define CSP_CCP_LIST 9
/**
 Get current access level.
 */
#define CSP_CCP_GET_ACCESS 10
/**
 Login to set access level.
 */
#define CSP_CCP_LOGIN_ACCESS 11
/**
 Logout to set access level to CFG_ACCESS_NONE.
 */
#define CSP_CCP_LOGOUT_ACCESS 12
/**
 Get string.
 */
#define CSP_CCP_GET_STRING 13
/**
 Get default string.
 */
#define CSP_CCP_GET_DEF_STRING 14
/**
 Edit string.
 */
#define CSP_CCP_EDIT_STRING 15
/**
 New config.
 */
#define CSP_CCP_NEW_CONFIG 16
/**@}*/

/**
 CSP Config status - max data length.
 */
#define CSP_CTP_TIME_STR_MAX_LEN  	26
#define CSP_CTP_MAIN_SOURCE_MAX_LEN  6

/**
 CCP config protocol description.
 */
struct csp_ccp_message {
	//! CCP request type.
	uint8_t type;
	//! CCP request code.
	uint8_t code;
	//! CCP result code.
	uint8_t result;
	union {
		uint8_t load, save;
		bool load_default;
		config_entry_bin_t get_config, remove_config, edit_value, edit_min, edit_max, new_config;
		struct {
			cfg_access_level_t access_level;
			char username[CONFIG_NAME_MAX_LEN];
			char password[CONFIG_NAME_MAX_LEN];
			char result[32];
		} get_access, login_access, logout_access;
		struct {
			uint16_t num_entries;
			uint16_t index_start;
			uint16_t index_end;
			char data[CSP_CONFIG_MANAGER_RPS_MTU - 6];
		} list;
		struct {
			char name[CONFIG_NAME_MAX_LEN];
			uint16_t str_ptr, str_len, str_total_len;
			char str[CSP_CONFIG_MANAGER_RPS_MTU - CONFIG_NAME_MAX_LEN - 6];
		} edit_string, get_string;
	};
} __attribute__ ((packed));

/**
 Macro for calculating total size of ccp config message.
 */
#define CCP_SIZE(_memb) (sizeof(((struct csp_ccp_message *)0)->type) + sizeof(((struct csp_ccp_message *)0)->code) + sizeof(((struct csp_ccp_message *)0)->result) + sizeof(((struct csp_ccp_message *)0)->_memb))

/**
 Generic send csp config message request.
 @param[in] node address of subsystem.
 @param[in] timeout timeout in mS to wait for reply..
 @param[in] code request code.
 @param[in] msg_size size of \a msg.
 @param[in,out] msg data.
 @return #CSP_ERR_NONE on success, otherwise an error code.
 */
int csp_ccp(uint8_t node, uint32_t timeout, uint8_t code, int msg_size, struct csp_ccp_message *msg);

/**
 Macro for defining time handling function.
 */
#define CCP_MESSAGE(_code, _memb) \
static inline int csp_ccp_##_memb(uint8_t node, uint32_t timeout, struct csp_ccp_message *msg) { \
	return csp_ccp(node, timeout, _code, CCP_SIZE(_memb), msg); \
}

CCP_MESSAGE(CSP_CCP_LOAD, load)
CCP_MESSAGE(CSP_CCP_SAVE, save)
CCP_MESSAGE(CSP_CCP_LOAD_DFAULT, load_default)
CCP_MESSAGE(CSP_CCP_GET_CONFIG, get_config)
CCP_MESSAGE(CSP_CCP_REMOVE_CONFIG, remove_config)
CCP_MESSAGE(CSP_CCP_EDIT_VALUE, edit_value)
CCP_MESSAGE(CSP_CCP_EDIT_MIN, edit_min)
CCP_MESSAGE(CSP_CCP_EDIT_MAX, edit_max)
CCP_MESSAGE(CSP_CCP_GET_ACCESS, get_access)
CCP_MESSAGE(CSP_CCP_LOGIN_ACCESS, login_access)
CCP_MESSAGE(CSP_CCP_LOGOUT_ACCESS, logout_access)
CCP_MESSAGE(CSP_CCP_NEW_CONFIG, new_config)

void csp_ccp_list(uint8_t node, uint32_t timeout);
int csp_ccp_get_string(uint8_t node, uint32_t timeout, char *name, char *str, int *str_total_len);
int csp_ccp_get_default_string(uint8_t node, uint32_t timeout, char *name, char *str, int *str_total_len);
int csp_ccp_edit_string(uint8_t node, uint32_t timeout, char *name, char *str, int str_total_len);

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
void csp_ccp_service_handler(csp_conn_t *conn, csp_packet_t *packet);

#ifdef __cplusplus
}
#endif
#endif
