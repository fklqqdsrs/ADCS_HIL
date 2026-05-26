#include "app_modules_config.h"

#if CSP_ENABLE_REGISTER

/*
 * csp_register_service_handler.c
 *
 *  Created on: Sep 19, 2024
 *      Author: thaib
 */

#include "csp_register_service.h"
#include "register_management.h"

#define TIMEOUT_CSP_REG 1000

bool generic_add_register(uint8_t device_id, uint8_t register_id, uint16_t sub_register, uint8_t type_register, RegisterValue *value, uint8_t access) {
	switch (type_register) {
	case U8:
		return add_register(device_id, register_id, sub_register, U8, &value->u8, access);
	case I8:
		return add_register(device_id, register_id, sub_register, I8, &value->i8, access);
	case U16:
		return add_register(device_id, register_id, sub_register, U16, &value->u16, access);
	case I16:
		return add_register(device_id, register_id, sub_register, I16, &value->i16, access);
	case U32:
		return add_register(device_id, register_id, sub_register, U32, &value->u32, access);
	case I32:
		return add_register(device_id, register_id, sub_register, I32, &value->i32, access);
	case FLOAT_TYPE:
		return add_register(device_id, register_id, sub_register, FLOAT_TYPE, &value->f, access);
	case DOUBLE_TYPE:
		return add_register(device_id, register_id, sub_register, DOUBLE_TYPE, &value->d, access);
	default:
		csp_log_error("Unsupported type_register in ADD!\n")
		;
		return false;
	}
}

bool generic_set_register(uint8_t device_id, uint8_t register_id, uint16_t sub_register, uint8_t type_register, RegisterValue *value) {
	switch (type_register) {
	case U8:
		return set_register(device_id, register_id, sub_register, &value->u8);
	case I8:
		return set_register(device_id, register_id, sub_register, &value->i8);
	case U16:
		return set_register(device_id, register_id, sub_register, &value->u16);
	case I16:
		return set_register(device_id, register_id, sub_register, &value->i16);
	case U32:
		return set_register(device_id, register_id, sub_register, &value->u32);
	case I32:
		return set_register(device_id, register_id, sub_register, &value->i32);
	case FLOAT_TYPE:
		return set_register(device_id, register_id, sub_register, &value->f);
	case DOUBLE_TYPE:
		return set_register(device_id, register_id, sub_register, &value->d);
	default:
		csp_log_error("Unsupported type_register in WRITE!\n")
		;
		return false;
	}
}

void service_register(csp_conn_t *conn, csp_packet_t *packet) {
	uint8_t action = packet->data[0] >> 4;
	uint8_t device_id, type_register, access, flags = FLAGS_NOT_FOUND;
	uint8_t register_id;
	uint16_t sub_register;
	RegisterValue outValue;

	if (action == ADD_REGISTER || action == WRITE_REGISTER) {
		unpack_AddWriteRequest(packet->data, &action, &device_id, &register_id, &sub_register, &type_register, &access, (void*) &outValue);
		size_t value_size = get_value_size(type_register);
		if (value_size == 0) {
			csp_log_error("Invalid type_register size\r\n");
			goto cleanup;
		}

		if (action == ADD_REGISTER) {
			if (generic_add_register(device_id, register_id, sub_register, type_register, &outValue, access)) {
				flags = FLAGS_SUCCESS;
			} else {
				csp_log_error("Failed to add register\n");
				flags = FLAGS_BUSY;
			}
		} else if (action == WRITE_REGISTER) {
			if (generic_set_register(device_id, register_id, sub_register, type_register, &outValue)) {
				flags = FLAGS_SUCCESS;
			} else {
				csp_log_error("Failed to set register value\n");
				flags = FLAGS_BUSY;
			}
		}

		csp_packet_t *AddWriteRequest_packet = csp_buffer_get(4);
		if (!AddWriteRequest_packet) {
			csp_log_error("AddWriteRequest_packet Failed to get buffer for response\n");
			goto cleanup;
		}

		pack_AddWrite_respond(AddWriteRequest_packet->data, action, flags);
		AddWriteRequest_packet->data[1] = register_id;
		AddWriteRequest_packet->data[2] = (uint8_t) ((sub_register >> 8) & 0xFF);
		AddWriteRequest_packet->data[3] = (uint8_t) (sub_register & 0xFF);
		AddWriteRequest_packet->length = 4;
		if (!csp_send(conn, AddWriteRequest_packet, TIMEOUT_CSP_REG)) {
			csp_log_error("Failed to send response\r\n");
			csp_buffer_free(AddWriteRequest_packet);
		}

	} else if (action == READ_REGISTER) {

		unpack_ReadRequest(packet->data, &action, &device_id, &register_id, &sub_register, &type_register, &access);

		size_t value_size = get_value_size(type_register);
		if (value_size == 0) {
			csp_log_error("Invalid type_register\r\n");
			goto cleanup;
		}

		if (get_register(device_id, register_id, sub_register, &outValue) == CSP_ERR_NONE) {
			flags = FLAGS_SUCCESS;
		} else {
			csp_log_error("Failed to get register value\n");
			flags = FLAGS_NOT_FOUND;
			value_size = 0;
		}

		csp_packet_t *response_packet = csp_buffer_get(4 + value_size);
		if (!response_packet) {
			csp_log_error("response_packet Failed to get buffer for response\n");
			goto cleanup;
		}

		pack_ReadResponse(response_packet->data, action, flags, register_id, sub_register, &outValue, value_size);
		response_packet->length = 4 + value_size;

		if (!csp_send(conn, response_packet, TIMEOUT_CSP_REG)) {
			csp_log_error("Failed to send response\r\n");
			csp_buffer_free(response_packet);
		}

	} else if (action == REMOVE_REGISTER) {
		unpack_ReadRequest(packet->data, &action, &device_id, &register_id, &sub_register, &type_register, &access);

		if (remove_register(device_id, register_id, sub_register)) {
			flags = FLAGS_SUCCESS;
		} else {
			flags = FLAGS_NOT_FOUND;
		}

		csp_packet_t *resp = csp_buffer_get(4);
		if (!resp) {
			csp_log_error("Remove response buffer alloc failed\n");
			goto cleanup;
		}
		pack_AddWrite_respond(resp->data, action, flags);
		resp->data[1] = register_id;
		resp->data[2] = (uint8_t) ((sub_register >> 8) & 0xFF);
		resp->data[3] = (uint8_t) (sub_register & 0xFF);
		resp->length = 4;
		if (!csp_send(conn, resp, TIMEOUT_CSP_REG)) {
			csp_log_error("Failed to send remove response\r\n");
			csp_buffer_free(resp);
		}

	} else if (action == GET_REGISTER_COUNT) {
		unpack_ReadRequest(packet->data, &action, &device_id, &register_id, &sub_register, &type_register, &access);

		uint32_t used = (uint32_t) count_used_registers();
		flags = FLAGS_SUCCESS;

		csp_packet_t *resp = csp_buffer_get(4 + sizeof(uint32_t));
		if (!resp) {
			csp_log_error("Count response buffer alloc failed\n");
			goto cleanup;
		}
		pack_ReadResponse(resp->data, action, flags, 0, 0, &used, sizeof(uint32_t));
		resp->length = 4 + sizeof(uint32_t);
		if (!csp_send(conn, resp, TIMEOUT_CSP_REG)) {
			csp_log_error("Failed to send count response\r\n");
			csp_buffer_free(resp);
		}

	} else if (action == GET_STATUS_REGISTER) {
		unpack_ReadRequest(packet->data, &action, &device_id, &register_id, &sub_register, &type_register, &access);

		uint32_t status[2];
		status[0] = (uint32_t) count_used_registers();
		status[1] = (uint32_t) count_available_slots();
		flags = FLAGS_SUCCESS;

		csp_packet_t *resp = csp_buffer_get(4 + sizeof(status));
		if (!resp) {
			csp_log_error("Status response buffer alloc failed\n");
			goto cleanup;
		}
		pack_ReadResponse(resp->data, action, flags, 0, 0, status, sizeof(status));
		resp->length = 4 + sizeof(status);
		if (!csp_send(conn, resp, TIMEOUT_CSP_REG)) {
			csp_log_error("Failed to send status response\r\n");
			csp_buffer_free(resp);
		}

	} else {
		csp_log_error("Unsupported action received!\n");
		goto cleanup;
	}
	cleanup: csp_buffer_free(packet);
}

#endif /* CSP_ENABLE_REGISTER */
