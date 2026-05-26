/*
 * csp_register_service_handler.c
 *
 *  Created on: Sep 19, 2024
 *      Author: thaib
 */

#include <src/func/func_register/csp_register_service.h>
//#include "csp_register.h"

#define TIMEOUT_CSP_REG 1000

bool generic_add_register(uint8_t device_id, uint8_t register_id, uint8_t sub_register, uint8_t type_register, RegisterValue *value, uint8_t access) {
	switch (type_register) {
//	case U8:
//		return add_register(device_id, register_id, sub_register, U8, &value->u8, access);
	case I8:
		return add_register(device_id, register_id, sub_register, I8, &value->i8, access);
//	case U16:
//		return add_register(device_id, register_id, sub_register, U16, &value->u16, access);
	case I16:
		return add_register(device_id, register_id, sub_register, I16, &value->i16, access);
//	case U32:
//		return add_register(device_id, register_id, sub_register, U32, &value->u32, access);
	case I32:
		return add_register(device_id, register_id, sub_register, I32, &value->i32, access);
	case FLOAT_TYPE :
		return add_register(device_id, register_id, sub_register, FLOAT_TYPE  , &value->f, access);
	case DOUBLE_TYPE:
		return add_register(device_id, register_id, sub_register, DOUBLE_TYPE, &value->d, access);
	default:
		csp_log_error("Unsupported type_register in ADD!\n")
		;
		return false;
	}
}



bool generic_set_register(uint8_t device_id, uint8_t register_id, uint8_t sub_register, uint8_t type_register, RegisterValue *value) {
	switch (type_register) {
//	case U8:
//		return set_register(device_id, register_id, sub_register, &value->u8);
	case I8:
		return set_register(device_id, register_id, sub_register, &value->i8);
//	case U16:
//		return set_register(device_id, register_id, sub_register, &value->u16);
	case I16:
		return set_register(device_id, register_id, sub_register, &value->i16);
//	case U32:
//		return set_register(device_id, register_id, sub_register, &value->u32);
	case I32:
		return set_register(device_id, register_id, sub_register, &value->i32);
	case FLOAT_TYPE :
		return set_register(device_id, register_id, sub_register, &value->f);
	case DOUBLE_TYPE:
		return set_register(device_id, register_id, sub_register, &value->d);
	default:
		csp_log_error("Unsupported type_register in WRITE!\n")
		;
		return false;
	}
}


void service_register_demo(csp_conn_t *conn, csp_packet_t *packet) {
	uint8_t action = packet->data[0] >> 4;

	if (action == ADD_REGISTER || action == WRITE_REGISTER) {
		uint8_t device_id, type_register, access, flags;
		uint8_t register_id, sub_register;
		RegisterValue outValue;

		unpack_AddWriteRequest(packet->data, &action, &device_id, &register_id, &sub_register, &type_register, &access, (void*) &outValue);
		size_t value_size = get_value_size(type_register);
		if (value_size == 0) {
			csp_log_error("Invalid type_register size\r\n");
			goto cleanup;
		}

		if (action == ADD_REGISTER) {
			if (!generic_add_register(device_id, register_id, sub_register, type_register, &outValue, access)) {
				csp_log_error("Failed to add register\n");
				flags = FLAGS_BUSY;
			}
			flags = FLAGS_SUCCESS;
		} else if (action == WRITE_REGISTER) {
			if (!generic_set_register(device_id, register_id, sub_register, type_register, &outValue)) {
				csp_log_error("Failed to set register\n");
				flags = FLAGS_BUSY;
			}
			flags = FLAGS_SUCCESS;
		}

		csp_packet_t *AddWriteRequest_packet = csp_buffer_get(1);
		if (!AddWriteRequest_packet) {
			csp_log_error("AddWriteRequest_packet Failed to get buffer for response\n");
			goto cleanup;
		}

		pack_AddWrite_respond(AddWriteRequest_packet->data, action, flags);
		AddWriteRequest_packet->length = 1;
		if (!csp_send(conn, AddWriteRequest_packet, TIMEOUT_CSP_REG)) {
			csp_log_error("Failed to send response\r\n");
			csp_buffer_free(AddWriteRequest_packet);
		}

	} else if (action == READ_REGISTER) {
		uint8_t device_id, type_register, access ,flags;
		uint8_t register_id, sub_register;
		RegisterValue outValue;
		uint64_t packed_value;

		unpack_ReadRequest(packet->data, &action, &device_id, &register_id, &sub_register, &type_register, &access);
		flags = FLAGS_SUCCESS;
		if (get_register_t(device_id, register_id, sub_register, &outValue, &type_register) != 0) {
			csp_log_error("Failed to get register value\n");
			flags = FLAGS_NOT_FOUND;
			packed_value = 0;
		}
		size_t value_size = get_value_size(type_register);
		if (convert_and_get_size(&outValue, type_register, &packed_value, &value_size) != 0) {
			csp_log_error("Invalid type_register\r\n");
			goto cleanup;
		}

		csp_packet_t *response_packet = csp_buffer_get(1 + value_size);
		if (!response_packet) {
			csp_log_error("response_packet Failed to get buffer for response\n");
			goto cleanup;
		}

		pack_ReadResponse(response_packet->data, action, flags, packed_value, value_size);
		response_packet->length = 1 + value_size;

		if (!csp_send(conn, response_packet, TIMEOUT_CSP_REG)) {
			csp_log_error("Failed to send response\r\n");
			csp_buffer_free(response_packet);
		}

	} else {
		csp_log_error("Unsupported action received!\n");
		goto cleanup;
	}
	cleanup: csp_buffer_free(packet);
}
