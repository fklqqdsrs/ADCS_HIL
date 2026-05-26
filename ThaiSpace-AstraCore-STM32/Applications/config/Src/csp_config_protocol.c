#include "app_modules_config.h"
#if CSP_ENABLE_CONFIG

/*
Cubesat Space Protocol - A small network-layer protocol designed for Cubesats
Copyright (C) 2012 GomSpace ApS (http://www.gomspace.com)
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
#include "app_modules_config.h"
#include <csp/arch/csp_malloc.h>
#include <csp_config_protocol.h>
#include <kservice.h>

#define CSP_CCP_ERR_NONE				0		/**< No error */
#define CSP_CCP_ERR_LOAD				1		/**< Not enough memory */
#define CSP_CCP_ERR_SAVE				2		/**< Not enough memory */
#define CSP_CCP_ERR_ADD					3		/**< Not enough memory */
#define CSP_CCP_ERR_CONVERT				4		/**< Not enough memory */
#define CSP_CCP_ERR_CONFIG_NOT_FOUND	5		/**< Not enough memory */
#define CSP_CCP_ERR_CONFIG_IS_EXISTS	6		/**< Not enough memory */
#define CSP_CCP_ERR_CONFIG_IS_STATIC	7		/**< Not enough memory */
#define CSP_CCP_ERR_PERMISSION			8
#define CSP_CCP_ERR_ACCESS_LEVEL		9		/**< Not enough memory */
#define CSP_CCP_ERR_TYPE				10		/**< Not enough memory */
#define CSP_CCP_ERR_STRING_LENGTH		11

static int do_ccp_load(struct csp_ccp_message *ccp) {
	if(config_load_profile())
		ccp->result = CSP_CCP_ERR_NONE;
	else
		ccp->result = CSP_CCP_ERR_LOAD;

	return CSP_ERR_NONE;
}

static int do_ccp_save(struct csp_ccp_message *ccp) {
	if(config_save_profile())
		ccp->result = CSP_CCP_ERR_NONE;
	else
		ccp->result = CSP_CCP_ERR_SAVE;

	return CSP_ERR_NONE;
}

static int do_ccp_load_default(struct csp_ccp_message *ccp) {
	config_load_profile_default(ccp->load_default);
	ccp->result = CSP_CCP_ERR_NONE;

	return CSP_ERR_NONE;
}

static int do_ccp_get_config(struct csp_ccp_message *ccp) {
	config_entry_t *e = config_find_entry(ccp->get_config.name);

	if(!e) {
		tsc_kprintf("[CCP Get] Entry not found: %s\r\n", ccp->remove_config.name);
		ccp->result = CSP_CCP_ERR_CONFIG_NOT_FOUND;
		return CSP_ERR_NONE;
	}

    if (e->access > config_get_access_level()) {
    	tsc_kprintf("[CCP Get] Permission denied for entry '%s'.\r\n", ccp->remove_config.name);
        ccp->result = CSP_CCP_ERR_ACCESS_LEVEL;
        return CSP_ERR_NONE;
    }

#if ENABLE_LITTLEFS
	config_entry_to_bin(e, &ccp->get_config);
	ccp->result = CSP_CCP_ERR_NONE;
#else
	ccp->result = CSP_CCP_ERR_CONVERT;
#endif
	return CSP_ERR_NONE;
}

static int do_ccp_remove_config(struct csp_ccp_message *ccp) {
	config_entry_t *e = config_find_entry(ccp->remove_config.name);

    if (!e) {
        tsc_kprintf("[CCP Remove] Entry not found: %s\r\n", ccp->remove_config.name);
		ccp->result = CSP_CCP_ERR_CONFIG_NOT_FOUND;
		return CSP_ERR_NONE;
    }

    if (e->access > config_get_access_level()) {
    	tsc_kprintf("[CCP Remove] Permission denied for entry '%s'.\r\n", ccp->remove_config.name);
        ccp->result = CSP_CCP_ERR_ACCESS_LEVEL;
        return CSP_ERR_NONE;
    }

	if(config_remove_entry(ccp->remove_config.name)) {
		ccp->result = CSP_CCP_ERR_NONE;
	}
	else {
		tsc_kprintf("[CCP Remove] Cann't remove static entry '%s'.\r\n", ccp->remove_config.name);
		ccp->result = CSP_CCP_ERR_CONFIG_IS_STATIC;
	}

	return CSP_ERR_NONE;
}

static int do_ccp_edit_value(struct csp_ccp_message *ccp) {
	config_entry_t *e = config_find_entry(ccp->edit_value.name);
	config_entry_bin_t *e_bin = &ccp->edit_value;

    if (!e) {
        tsc_kprintf("[CCP Edit] Entry not found: %s\r\n", ccp->edit_value.name);
		ccp->result = CSP_CCP_ERR_CONFIG_NOT_FOUND;
		return CSP_ERR_NONE;
    }

    if (e->permission != CFG_PERM_RW) {
    	tsc_kprintf("[CCP Edit] Permission denied for entry '%s' is read-only.\r\n", ccp->edit_value.name);
    	ccp->result = CSP_CCP_ERR_PERMISSION;
    	return CSP_ERR_NONE;
    }

    if (e->access > config_get_access_level()) {
    	tsc_kprintf("[CCP Edit] Access denied for entry '%s'.\r\n", ccp->edit_value.name);
        ccp->result = CSP_CCP_ERR_ACCESS_LEVEL;
        return CSP_ERR_NONE;
    }

    if(e->type == e_bin->type) {
    	config_set_entry_val(e->value, e->type, &e_bin->value);
    	ccp->result = CSP_CCP_ERR_NONE;
    }
    else {
    	ccp->result = CSP_CCP_ERR_TYPE;
    }

    return CSP_ERR_NONE;
}

static int do_ccp_edit_min(struct csp_ccp_message *ccp) {
	config_entry_t *e = config_find_entry(ccp->edit_min.name);
	config_entry_bin_t *e_bin = &ccp->edit_min;

    if (!e) {
        tsc_kprintf("[CCP Edit] Entry not found: %s\r\n", ccp->edit_min.name);
		ccp->result = CSP_CCP_ERR_CONFIG_NOT_FOUND;
		return CSP_ERR_NONE;
    }

    if (e->permission != CFG_PERM_RW) {
    	tsc_kprintf("[CCP Edit] Permission denied for entry '%s' is read-only.\r\n", ccp->edit_min.name);
    	ccp->result = CSP_CCP_ERR_PERMISSION;
    	return CSP_ERR_NONE;
    }

    if (e->access > config_get_access_level()) {
    	tsc_kprintf("[CCP Edit] Access denied for entry '%s'.\r\n", ccp->edit_min.name);
        ccp->result = CSP_CCP_ERR_ACCESS_LEVEL;
        return CSP_ERR_NONE;
    }

    if(e->type == e_bin->type) {
    	config_set_entry_val(e->min, e->type, &e_bin->range.min);
    	ccp->result = CSP_CCP_ERR_NONE;
    }
    else {
    	ccp->result = CSP_CCP_ERR_TYPE;
    	return CSP_ERR_INVAL;
    }

    return CSP_ERR_NONE;
}

static int do_ccp_edit_max(struct csp_ccp_message *ccp) {
	config_entry_t *e = config_find_entry(ccp->edit_max.name);
	config_entry_bin_t *e_bin = &ccp->edit_max;

    if (!e) {
        tsc_kprintf("[CCP Edit] Entry not found: %s\r\n", ccp->edit_max.name);
		ccp->result = CSP_CCP_ERR_CONFIG_NOT_FOUND;
		return CSP_ERR_NONE;
    }

    if (e->permission != CFG_PERM_RW) {
    	tsc_kprintf("[CCP Edit] Permission denied for entry '%s' is read-only.\r\n", ccp->edit_max.name);
    	ccp->result = CSP_CCP_ERR_PERMISSION;
    	return CSP_ERR_NONE;
    }

    if (e->access > config_get_access_level()) {
    	tsc_kprintf("[CCP Edit] Access denied for entry '%s'.\r\n", ccp->edit_max.name);
        ccp->result = CSP_CCP_ERR_ACCESS_LEVEL;
        return CSP_ERR_NONE;
    }

    if(e->type == e_bin->type) {
    	config_set_entry_val(e->max, e->type, &e_bin->range.max);
    	ccp->result = CSP_CCP_ERR_NONE;
    }
    else {
    	ccp->result = CSP_CCP_ERR_TYPE;
    }

    return CSP_ERR_NONE;
}

static int do_ccp_list(csp_conn_t * conn, csp_packet_t * packet) {
	config_profile_t *g_active_profile = config_get_active_profile();
	struct csp_ccp_message * ccp = (struct csp_ccp_message *) packet->data;

	if(!g_active_profile) {
		/* Send out the data */
		strcpy((char *)ccp->list.data, "No active profile!");
		packet->length = CCP_SIZE(list);

		if (!csp_send(conn, packet, 0))
			csp_buffer_free(packet);
		return CSP_ERR_NONE;
	}

	char * conlist = csp_malloc(512);
	/* Check for malloc fail */
	if (conlist == NULL) {
		/* Send out the data */
		strcpy((char *)ccp->list.data, "Not enough memory");
		packet->length = CCP_SIZE(list);

		if (!csp_send(conn, packet, 0))
			csp_buffer_free(packet);
		/* Break and let the default handling send packet */
		return CSP_ERR_NONE;
	}

	/* Retrieve the thread list */
	int config_num = 0;
	do {
		int conlen = 0;

	    for (; config_num < g_active_profile->num_entries; ++config_num) {
	        if((conlen + CONFIG_STRING_SLOT_SIZE + CONFIG_NAME_MAX_LEN + 8) > 512)
	        	break;

	        config_entry_t *e = g_active_profile->entries[config_num];
	        if (e->access > config_get_access_level()) continue;

	        conlen += tsc_sprintf(&conlist[conlen], "%s: ", e->name);
	        conlen += config_val_str(e->type, e->value, &conlist[conlen]);
	        conlen += tsc_sprintf(&conlist[conlen], "\r\n");
	    }

		if(!conlen)
			break;

		/* Split the potentially very long string into packets */
		int i = 0;
		while (i < conlen) {

			/* Allocate packet buffer, if need be */
			if (packet == NULL) {
				packet = csp_buffer_get(CSP_CONFIG_MANAGER_RPS_MTU);
				ccp = (struct csp_ccp_message *) packet->data;
			}
			if (packet == NULL)
				break;

			/* Calculate length, either full MTU or the remainder */
			int data_len = (conlen - i > sizeof(ccp->list.data)-1) ? sizeof(ccp->list.data)-1 : (conlen - i);
			packet->length = CCP_SIZE(list);

			/* Send out the data */
			memset(ccp->list.data, 0, sizeof(ccp->list.data));
			memcpy(ccp->list.data, &conlist[i], data_len);
			ccp->list.data[data_len] = 0;
			i += data_len;
			if (!csp_send(conn, packet, 0))
				csp_buffer_free(packet);

			/* Clear the packet reference when sent */
			packet = NULL;

		}

	} while (1);

	csp_buffer_free(packet);
	csp_free(conlist);
	packet->length = 0;

	return CSP_ERR_NONE;
}

static int do_ccp_get_access(struct csp_ccp_message *ccp) {
	ccp->get_access.access_level = config_get_access_level();
	tsc_strncpy(ccp->get_access.username, config_get_access_user(), CONFIG_NAME_MAX_LEN);

	ccp->result = CSP_CCP_ERR_NONE;
	return CSP_ERR_NONE;
}

static int do_ccp_login_access(struct csp_ccp_message *ccp) {
	char *username = ccp->login_access.username;
	char *password = ccp->login_access.password;

	if(config_login(username, password))
		tsc_sprintf(ccp->login_access.result, "Login successful.");
	else
		tsc_sprintf(ccp->login_access.result, "Invalid credentials!");

	ccp->login_access.access_level = config_get_access_level();
	tsc_strncpy(ccp->login_access.username, config_get_access_user(), CONFIG_NAME_MAX_LEN);

	ccp->result = CSP_CCP_ERR_NONE;
	return CSP_ERR_NONE;
}

static int do_ccp_logout_access(struct csp_ccp_message *ccp) {
	config_logout();
	tsc_sprintf(ccp->login_access.result, "Logout successful.");

	ccp->login_access.access_level = config_get_access_level();
	tsc_strncpy(ccp->login_access.username, config_get_access_user(), CONFIG_NAME_MAX_LEN);

	ccp->result = CSP_CCP_ERR_NONE;
	return CSP_ERR_NONE;
}

static int do_ccp_get_string(csp_conn_t * conn, csp_packet_t * packet) {
	struct csp_ccp_message * ccp = (struct csp_ccp_message *) packet->data;
	config_entry_t *e = config_find_entry(ccp->edit_string.name);

	packet->length = CCP_SIZE(get_string);

    if (!e) {
        tsc_kprintf("[CCP Get] Entry not found: %s\r\n", ccp->get_string.name);
		ccp->result = CSP_CCP_ERR_CONFIG_NOT_FOUND;

		return CSP_ERR_NONE;
    }

    if (e->access > config_get_access_level()) {
    	tsc_kprintf("[CCP Get] Permission denied for entry '%s'.\r\n", ccp->get_string.name);
        ccp->result = CSP_CCP_ERR_ACCESS_LEVEL;

        return CSP_ERR_NONE;
    }

    uint16_t str_len = strlen((char*)e->value);
    char *val = e->value;

	if(str_len == 0) {
		/* Send out the data */
		ccp->result = CSP_CCP_ERR_NONE;
		memset(ccp->get_string.str, 0, sizeof(ccp->get_string.str));
		ccp->get_string.str_ptr = 0;
		ccp->get_string.str_len = 0;
		ccp->get_string.str_total_len = 0;

		return CSP_ERR_NONE;
	}

	/* Split the potentially very long string into packets */
	int i = 0;
	while (i < str_len) {

		/* Allocate packet buffer, if need be */
		if (packet == NULL) {
			packet = csp_buffer_get(CSP_CONFIG_MANAGER_RPS_MTU);
			ccp = (struct csp_ccp_message *) packet->data;
		}
		if (packet == NULL)
			break;

		/* Calculate length, either full MTU or the remainder */
		int data_len = (str_len - i > sizeof(ccp->get_string.str)) ? sizeof(ccp->get_string.str) : (str_len - i);

		ccp->result = CSP_CCP_ERR_NONE;
		ccp->get_string.str_ptr = i;
		ccp->get_string.str_len = data_len;
		ccp->get_string.str_total_len = str_len;

		/* Send out the data */
		memset(ccp->get_string.str, 0, sizeof(ccp->get_string.str));
		memcpy(ccp->get_string.str, &val[i], data_len);

		i += data_len;
		if (!csp_send(conn, packet, 0))
			csp_buffer_free(packet);

		/* Clear the packet reference when sent */
		packet = NULL;
	}

	csp_buffer_free(packet);
	packet->length = 0;

	return CSP_ERR_NONE;
}

static int do_ccp_get_default_string(csp_conn_t * conn, csp_packet_t * packet) {
	struct csp_ccp_message * ccp = (struct csp_ccp_message *) packet->data;
	config_entry_t *e = config_find_entry(ccp->edit_string.name);

	packet->length = CCP_SIZE(get_string);

    if (!e) {
        tsc_kprintf("[CCP Get] Entry not found: %s\r\n", ccp->get_string.name);
		ccp->result = CSP_CCP_ERR_CONFIG_NOT_FOUND;

		return CSP_ERR_NONE;
    }

    if (e->access > config_get_access_level()) {
    	tsc_kprintf("[CCP Get] Permission denied for entry '%s'.\r\n", ccp->get_string.name);
        ccp->result = CSP_CCP_ERR_ACCESS_LEVEL;

        return CSP_ERR_NONE;
    }

    uint16_t str_len = strlen((char*)e->default_value);
    char *val = (char*)e->default_value;

	if(str_len == 0) {
		/* Send out the data */
		ccp->result = CSP_CCP_ERR_NONE;
		memset(ccp->get_string.str, 0, sizeof(ccp->get_string.str));
		ccp->get_string.str_ptr = 0;
		ccp->get_string.str_len = 0;
		ccp->get_string.str_total_len = 0;

		return CSP_ERR_NONE;
	}

	/* Split the potentially very long string into packets */
	int i = 0;
	while (i < str_len) {

		/* Allocate packet buffer, if need be */
		if (packet == NULL) {
			packet = csp_buffer_get(CSP_CONFIG_MANAGER_RPS_MTU);
			ccp = (struct csp_ccp_message *) packet->data;
		}
		if (packet == NULL)
			break;

		/* Calculate length, either full MTU or the remainder */
		int data_len = (str_len - i > sizeof(ccp->get_string.str)) ? sizeof(ccp->get_string.str) : (str_len - i);

		ccp->result = CSP_CCP_ERR_NONE;
		ccp->get_string.str_ptr = i;
		ccp->get_string.str_len = data_len;
		ccp->get_string.str_total_len = str_len;

		/* Send out the data */
		memset(ccp->get_string.str, 0, sizeof(ccp->get_string.str));
		memcpy(ccp->get_string.str, &val[i], data_len);

		i += data_len;
		if (!csp_send(conn, packet, 0))
			csp_buffer_free(packet);

		/* Clear the packet reference when sent */
		packet = NULL;
	}

	csp_buffer_free(packet);
	packet->length = 0;

	return CSP_ERR_NONE;
}


static int do_ccp_edit_string(struct csp_ccp_message *ccp) {
	config_entry_t *e = config_find_entry(ccp->edit_string.name);

    if (!e) {
        tsc_kprintf("[CCP Edit] Entry not found: %s\r\n", ccp->edit_string.name);
		ccp->result = CSP_CCP_ERR_CONFIG_NOT_FOUND;
		return CSP_ERR_INVAL;
    }

    if (e->permission != CFG_PERM_RW) {
    	tsc_kprintf("[CCP Edit] Permission denied for entry '%s' is read-only.\r\n", ccp->edit_string.name);
    	ccp->result = CSP_CCP_ERR_PERMISSION;
    	return CSP_ERR_NONE;
    }

    if (e->access > config_get_access_level()) {
    	tsc_kprintf("[CCP Edit] Access denied for entry '%s'.\r\n", ccp->edit_string.name);
        ccp->result = CSP_CCP_ERR_ACCESS_LEVEL;
        return CSP_ERR_NONE;
    }

    uint16_t str_ptr, str_len, str_total_len;
    str_ptr = ccp->edit_string.str_ptr;
    str_len = ccp->edit_string.str_len;
    str_total_len = ccp->edit_string.str_total_len;

    if(((str_ptr + str_len) > e->size) || ((str_ptr + str_len) > str_total_len)) {
    	ccp->result = CSP_CCP_ERR_STRING_LENGTH;
    	return CSP_ERR_NONE;
    }

    char *val = (char*)e->value;
    tsc_strncpy(&val[str_ptr], ccp->edit_string.str, str_len);

    if((str_ptr + str_len) >= str_total_len) {
        val = (char*)e->default_value;
        if(strlen(val) == 0) {
        	int str_len = (str_total_len < CONFIG_STRING_SLOT_SIZE) ? str_total_len : CONFIG_STRING_SLOT_SIZE;
        	tsc_strncpy((char*)e->default_value, (char*)e->value, str_len);
        }
    }

    ccp->result = CSP_CCP_ERR_NONE;
	return CSP_ERR_NONE;
}

static int do_ccp_new_config(struct csp_ccp_message *ccp) {
	config_entry_t *e = config_find_entry(ccp->new_config.name);

	if(e) {
		tsc_kprintf("[CCP New] Entry already exists: %s\r\n", ccp->new_config.name);
		ccp->result = CSP_CCP_ERR_CONFIG_IS_EXISTS;
		return CSP_ERR_NONE;
	}

	config_entry_t *e_new = tsc_malloc(sizeof(config_entry_t));
#if ENABLE_LITTLEFS
	if(!config_bin_to_entry(&ccp->new_config, e_new, true)) {
		tsc_kprintf("[CCP New] Error to convert entry: %s\r\n", ccp->new_config.name);
		ccp->result = CSP_CCP_ERR_CONVERT;
		tsc_free(e_new);
		return CSP_ERR_NONE;
	}
#else
	ccp->result = CSP_CCP_ERR_CONVERT;
	tsc_free(e_new);
	return CSP_ERR_NONE;
#endif

	e_new->access = config_get_access_level();

	if(!config_add_entry(e_new)) {
		tsc_kprintf("[CCP New] Error to add entry: %s\r\n", ccp->new_config.name);
		ccp->result = CSP_CCP_ERR_ADD;
		return CSP_ERR_NONE;
	}

	ccp->result = CSP_CCP_ERR_NONE;
	return CSP_ERR_NONE;
}


/* CSP Management Protocol handler */
static int csp_ccp_handler(csp_conn_t * conn, csp_packet_t * packet) {

	int ret = CSP_ERR_INVAL;
	struct csp_ccp_message * ccp = (struct csp_ccp_message *) packet->data;

	/* Ignore everything but requests */
	if (ccp->type != CSP_CCP_REQUEST)
		return ret;

	switch (ccp->code) {
		case CSP_CCP_LOAD:
			ret = do_ccp_load(ccp);
			packet->length = CCP_SIZE(load);
			break;

		case CSP_CCP_SAVE:
			ret = do_ccp_save(ccp);
			packet->length = CCP_SIZE(save);
			break;

		case CSP_CCP_LOAD_DFAULT:
			ret = do_ccp_load_default(ccp);
			packet->length = CCP_SIZE(load_default);
			break;

		case CSP_CCP_GET_CONFIG:
			ret = do_ccp_get_config(ccp);
			packet->length = CCP_SIZE(get_config);
			break;

		case CSP_CCP_REMOVE_CONFIG:
			ret = do_ccp_remove_config(ccp);
			packet->length = CCP_SIZE(remove_config);
			break;

		case CSP_CCP_EDIT_VALUE:
			ret = do_ccp_edit_value(ccp);
			packet->length = CCP_SIZE(edit_value);
			break;

		case CSP_CCP_EDIT_MIN:
			ret = do_ccp_edit_min(ccp);
			packet->length = CCP_SIZE(edit_min);
			break;

		case CSP_CCP_EDIT_MAX:
			ret = do_ccp_edit_max(ccp);
			packet->length = CCP_SIZE(edit_max);
			break;

		case CSP_CCP_LIST:
			ret = do_ccp_list(conn, packet);
			packet->length = 0;
			break;

		case CSP_CCP_GET_ACCESS:
			ret = do_ccp_get_access(ccp);
			packet->length = CCP_SIZE(get_access);
			break;

		case CSP_CCP_LOGIN_ACCESS:
			ret = do_ccp_login_access(ccp);
			packet->length = CCP_SIZE(login_access);
			break;

		case CSP_CCP_LOGOUT_ACCESS:
			ret = do_ccp_logout_access(ccp);
			packet->length = CCP_SIZE(logout_access);
			break;

		case CSP_CCP_GET_STRING:
			ret = do_ccp_get_string(conn, packet);
			break;

		case CSP_CCP_GET_DEF_STRING:
			ret = do_ccp_get_default_string(conn, packet);
			break;

		case CSP_CCP_EDIT_STRING:
			ret = do_ccp_edit_string(ccp);
			packet->length = CCP_SIZE(edit_string);
			break;

		case CSP_CCP_NEW_CONFIG:
			ret = do_ccp_new_config(ccp);
			packet->length = CCP_SIZE(new_config);
			break;
	}

	ccp->type = CSP_CCP_REPLY;

	return ret;
}

void csp_ccp_service_handler(csp_conn_t * conn, csp_packet_t * packet) {

	switch (csp_conn_dport(conn)) {

	case CSP_CCP_PORT:
		/* Pass to CMP handler */
		if (csp_ccp_handler(conn, packet) != CSP_ERR_NONE) {
			csp_buffer_free(packet);
			return;
		}
		break;

	default:
		csp_buffer_free(packet);
		return;
	}

	if ((packet != NULL) && (packet->length)) {
		if (!csp_send(conn, packet, 0))
			csp_buffer_free(packet);
	}
}

int csp_ccp(uint8_t node, uint32_t timeout, uint8_t code, int msg_size, struct csp_ccp_message * msg) {
	msg->type = CSP_CCP_REQUEST;
	msg->code = code;
	int status = csp_transaction(CSP_PRIO_NORM, node, CSP_CCP_PORT, timeout, msg, msg_size, msg, msg_size);
	if (status == 0) {
		return CSP_ERR_TIMEDOUT;
	}

	return CSP_ERR_NONE;
}

void csp_ccp_list(uint8_t node, uint32_t timeout) {

	/* Open connection */
	csp_conn_t * conn = csp_connect(CSP_PRIO_NORM, node, CSP_CCP_PORT, 0, 0);
	if (conn == NULL) {
		return;
	}

	/* Prepare data */
	csp_packet_t * packet = csp_buffer_get(95);

	/* Check malloc */
	if (packet == NULL) {
		goto out;
	}

	struct csp_ccp_message * msg = (struct csp_ccp_message *) packet->data;
	packet->length = 2;
	msg->type = CSP_CCP_REQUEST;
	msg->code = CSP_CCP_LIST;

	tsc_kprintf("All configuration on node %u: \r\n", node);

	/* Try to send frame */
	if (!csp_send(conn, packet, 0)) {
		goto out;
	}

	while(1) {

		/* Read incoming frame */
		packet = csp_read(conn, timeout);
		if (packet == NULL) {
			break;
		}

		/* We have a reply */
		struct csp_ccp_message * ccp = (struct csp_ccp_message *) packet->data;
		tsc_kprintf("%s", ccp->list.data);

		/* Each packet from csp_read must to be freed by user */
		csp_buffer_free(packet);
	}

	tsc_kprintf("\r\n");

	/* Clean up */
out:
	csp_buffer_free(packet);
	csp_close(conn);

}

int csp_ccp_get_string(uint8_t node, uint32_t timeout, char* name, char* str, int* str_total_len) {
	int error=0;

	/* Open connection */
	csp_conn_t * conn = csp_connect(CSP_PRIO_NORM, node, CSP_CCP_PORT, 0, 0);
	if (conn == NULL) {
		return CSP_ERR_BUSY;
	}

	/* Prepare data */
	csp_packet_t * packet = csp_buffer_get(CSP_BUFFER_PACKET_OVERHEAD + CSP_CONFIG_MANAGER_RPS_MTU);

	/* Check malloc */
	if (packet == NULL) {
		error = CSP_ERR_NOMEM;
		goto out;
	}

	struct csp_ccp_message * msg = (struct csp_ccp_message *) packet->data;
	packet->length = CCP_SIZE(get_string);
	msg->type = CSP_CCP_REQUEST;
	msg->code = CSP_CCP_GET_STRING;
	tsc_strncpy(msg->get_string.name, name, CONFIG_NAME_MAX_LEN);

	tsc_kprintf("Get config [%s] string on node %u: \r\n", name, node);

	/* Try to send frame */
	if (!csp_send(conn, packet, 0)) {
		error = CSP_ERR_TX;
		csp_buffer_free(packet);
		goto out;
	}

	int str_recv_len = 0, str_recv_total_len=0;

	while(1) {

		/* Read incoming frame */
		packet = csp_read(conn, timeout);
		if (packet == NULL) {
			error = CSP_ERR_TIMEDOUT;
			break;
		}

		/* We have a reply */
		struct csp_ccp_message * ccp = (struct csp_ccp_message *) packet->data;
		error = msg->result;
		if(error) {
			/* Each packet from csp_read must to be freed by user */
			csp_buffer_free(packet);
			goto out;
		}

		tsc_memcpy(&str[ccp->get_string.str_ptr], ccp->get_string.str, ccp->get_string.str_len);
		str_recv_len += ccp->get_string.str_len;
		str_recv_total_len = ccp->get_string.str_total_len;

		/* Each packet from csp_read must to be freed by user */
		csp_buffer_free(packet);

		if(str_recv_len >= str_recv_total_len) {
			if(str_total_len) *str_total_len = str_recv_total_len;
			break;
		}
	}
	str[str_recv_len] = 0;

	/* Clean up */
out:
	csp_close(conn);

	return error;
}

int csp_ccp_get_default_string(uint8_t node, uint32_t timeout, char* name, char* str, int* str_total_len) {
	int error=0;

	/* Open connection */
	csp_conn_t * conn = csp_connect(CSP_PRIO_NORM, node, CSP_CCP_PORT, 0, 0);
	if (conn == NULL) {
		return CSP_ERR_BUSY;
	}

	/* Prepare data */
	csp_packet_t * packet = csp_buffer_get(CSP_BUFFER_PACKET_OVERHEAD + CSP_CONFIG_MANAGER_RPS_MTU);

	/* Check malloc */
	if (packet == NULL) {
		error = CSP_ERR_NOMEM;
		goto out;
	}

	struct csp_ccp_message * msg = (struct csp_ccp_message *) packet->data;
	packet->length = CCP_SIZE(get_string);
	msg->type = CSP_CCP_REQUEST;
	msg->code = CSP_CCP_GET_DEF_STRING;
	tsc_strncpy(msg->get_string.name, name, CONFIG_NAME_MAX_LEN);

	tsc_kprintf("Get config [%s] default string on node %u: \r\n", name, node);

	/* Try to send frame */
	if (!csp_send(conn, packet, 0)) {
		error = CSP_ERR_TX;
		csp_buffer_free(packet);
		goto out;
	}

	int str_recv_len = 0, str_recv_total_len=0;

	while(1) {

		/* Read incoming frame */
		packet = csp_read(conn, timeout);
		if (packet == NULL) {
			error = CSP_ERR_TIMEDOUT;
			break;
		}

		/* We have a reply */
		struct csp_ccp_message * ccp = (struct csp_ccp_message *) packet->data;
		error = msg->result;
		if(error) {
			/* Each packet from csp_read must to be freed by user */
			csp_buffer_free(packet);
			goto out;
		}

		tsc_memcpy(&str[ccp->get_string.str_ptr], ccp->get_string.str, ccp->get_string.str_len);
		str_recv_len += ccp->get_string.str_len;
		str_recv_total_len = ccp->get_string.str_total_len;

		/* Each packet from csp_read must to be freed by user */
		csp_buffer_free(packet);

		if(str_recv_len >= str_recv_total_len) {
			*str_total_len = str_recv_total_len;
			break;
		}
	}
	str[str_recv_len] = 0;

	/* Clean up */
out:
	csp_close(conn);

	return error;
}

int csp_ccp_edit_string(uint8_t node, uint32_t timeout, char* name, char* str, int str_total_len) {
	int error=0;

	/* Open connection */
	csp_conn_t * conn = csp_connect(CSP_PRIO_NORM, node, CSP_CCP_PORT, 0, 0);
	if (conn == NULL) {
		return CSP_ERR_BUSY;
	}

	tsc_kprintf("Edit config [%s] string on node %u: \r\n", name, node);

	/* Split the potentially very long string into packets */
	int i = 0;
	while (i < str_total_len) {
		/* Prepare data */
		csp_packet_t * packet = csp_buffer_get(CSP_BUFFER_PACKET_OVERHEAD + CSP_CONFIG_MANAGER_RPS_MTU);

		/* Check malloc */
		if (packet == NULL) {
			error = CSP_ERR_NOMEM;
			csp_buffer_free(packet);
			goto out;
		}

		struct csp_ccp_message * ccp = (struct csp_ccp_message *) packet->data;
		ccp->type = CSP_CCP_REQUEST;
		ccp->code = CSP_CCP_EDIT_STRING;
		tsc_strncpy(ccp->edit_string.name, name, CONFIG_NAME_MAX_LEN);

		/* Calculate length, either full MTU or the remainder */
		int str_len = (str_total_len - i > sizeof(ccp->edit_string.str)) ? sizeof(ccp->edit_string.str) : (str_total_len - i);
		packet->length = CCP_SIZE(edit_string);
		ccp->result = CSP_CCP_ERR_NONE;
		ccp->edit_string.str_ptr = i;
		ccp->edit_string.str_len = str_len;
		ccp->edit_string.str_total_len = str_total_len;

		/* Send out the data */
		memset(ccp->edit_string.str, 0, sizeof(ccp->edit_string.str));
		memcpy(ccp->edit_string.str, &str[i], str_len);

		i += str_len;

		if (!csp_send(conn, packet, 0)) {
			error = CSP_ERR_TX;
			break;
		}

		/* Read incoming frame */
		packet = csp_read(conn, timeout);
		if (packet == NULL) {
			error = CSP_ERR_TIMEDOUT;
			break;
		}

		struct csp_ccp_message * msg = (struct csp_ccp_message *) packet->data;
		error = msg->result;

		/* Each packet from csp_read must to be freed by user */
		csp_buffer_free(packet);

		if(error) break;
	}

	/* Clean up */
out:
	csp_close(conn);

	return error;
}



#endif /* CSP_ENABLE_CONFIG */
