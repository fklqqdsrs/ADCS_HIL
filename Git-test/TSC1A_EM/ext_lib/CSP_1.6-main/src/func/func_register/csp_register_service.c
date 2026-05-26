#include <src/func/func_register/csp_register_service.h>

int convert_and_get_size(void *value, uint8_t type_register, uint64_t *packed_value, size_t *value_size) {
	switch (type_register) {
	case 1: // U8
		*packed_value = *(uint8_t*) value;
		*value_size = sizeof(uint8_t);
		break;
	case 2: // I8
		*packed_value = *(int8_t*) value;
		*value_size = sizeof(int8_t);
		break;
	case 3: // U16
		*packed_value = *(uint16_t*) value;
		*value_size = sizeof(uint16_t);
		break;
	case 4: // I16
		*packed_value = *(int16_t*) value;
		*value_size = sizeof(int16_t);
		break;
	case 5: // U32
		*packed_value = *(uint32_t*) value;
		*value_size = sizeof(uint32_t);
		break;
	case 6: // I32
		*packed_value = *(int32_t*) value;
		*value_size = sizeof(int32_t);
		break;
	case 7: // FLOAT
		memcpy(packed_value, value, sizeof(float));
		*value_size = sizeof(float);
		break;
	case 8: // DOUBLE
		memcpy(packed_value, value, sizeof(double));
		*value_size = sizeof(double);
		break;
	default:
		return -1;
	}
	return 0;
}

int add_register_csp(unsigned int device_id, unsigned int register_id, unsigned int sub_register, uint8_t type_register,
		void *value, uint8_t access_mode, uint16_t timeout) { 
	uint32_t start_time = csp_get_ms();
	if (device_id == csp_get_address()) {
		if (add_register(device_id, register_id, sub_register, type_register, value, access_mode) == true) {
			uint32_t end_time = csp_get_ms();
			return (int8_t) (end_time - start_time);
		}
		return CSP_ERR_ALREADY;
	}

	csp_conn_t *conn = csp_connect(CSP_PRIO_NORM, device_id, PORT_REGISTER, timeout, CSP_O_NONE);
	if (conn == NULL) {
		csp_log_error("Connection failed\r\n");
		return CSP_ERR_NOMEM;
	}

	uint64_t packed_value = 0;
	size_t value_size = 0;

	if (convert_and_get_size(value, type_register, &packed_value, &value_size) != 0) {
		csp_log_error("Invalid type_register\r\n");
		csp_close(conn);
		return CSP_ERR_INVAL;
	}

	uint8_t *buffer = NULL;
	pack_AddWriteRequest(&buffer, ADD_REGISTER, device_id, register_id, sub_register, type_register, access_mode,
			packed_value);
	if (buffer == NULL) {
		csp_log_error("Failed to pack data\r\n");
		csp_close(conn);
		return CSP_ERR_INVAL;
	}

	size_t packet_size = 4 + value_size;
	csp_packet_t *packet = csp_buffer_get(packet_size);
	if (packet == NULL) {
		csp_log_error("Failed to get CSP buffer\r\n");
		free(buffer);
		csp_close(conn);
		return CSP_ERR_BUSY;
	}

	memcpy(packet->data, buffer, packet_size);
	packet->length = packet_size;
	free(buffer);

	if (!csp_send(conn, packet, timeout)) {
		csp_log_error("Send failed\r\n");
		csp_buffer_free(packet);
		csp_close(conn);
		return CSP_ERR_TX;
	}

	while ((packet = csp_read(conn, timeout)) != NULL) {
		uint8_t action, flags;
		unpack_AddWrite_respond((uint8_t) packet->data[0], &action, &flags);
		csp_buffer_free(packet);
		break;
	}

	csp_close(conn);
	uint32_t end_time = csp_get_ms();
	return (int8_t) (end_time - start_time);
}

int set_register_csp(unsigned int device_id, unsigned int register_id, unsigned int sub_register, uint8_t type_register,
		void *value, uint16_t timeout) {
	uint32_t start_time = csp_get_ms();
	if (device_id == csp_get_address()) {
		if (set_register_myself(device_id, register_id, sub_register, value) == CSP_ERR_NONE) {
			uint32_t end_time = csp_get_ms();
			return (int8_t) (end_time - start_time);
		}
		return CSP_ERR_ALREADY;

	}

	csp_conn_t *conn = csp_connect(CSP_PRIO_NORM, device_id, PORT_REGISTER, timeout, CSP_O_NONE);
	if (conn == NULL) {
		csp_log_error("Connection failed\r\n");
		return CSP_ERR_NOMEM;
	}

	uint64_t packed_value = 0;
	size_t value_size = 0;
	if (convert_and_get_size(value, type_register, &packed_value, &value_size) != 0) {
		csp_log_error("Invalid type_register\r\n");
		csp_close(conn);
		return CSP_ERR_INVAL;
	}

	uint8_t access_mode = R_ENFORCE;
	uint8_t *buffer = NULL;

	pack_AddWriteRequest(&buffer, WRITE_REGISTER, device_id, register_id, sub_register, type_register, access_mode,
			packed_value);
	if (buffer == NULL) {
		csp_log_error("Failed to pack data\r\n");
		csp_close(conn);
		return CSP_ERR_INVAL;
	}
	size_t packet_size = 4 + value_size;
	csp_packet_t *packet = csp_buffer_get(packet_size);
	if (packet == NULL) {
		csp_log_error("Failed to get CSP buffer\r\n");
		free(buffer);
		csp_close(conn);
		return CSP_ERR_BUSY;
	}

	memcpy(packet->data, buffer, packet_size);
	packet->length = packet_size;
	free(buffer);
	if (!csp_send(conn, packet, timeout)) {
		csp_log_error("Send failed\r\n");
		csp_buffer_free(packet);
		csp_close(conn);
		return CSP_ERR_TX;
	}

	while ((packet = csp_read(conn, timeout)) != NULL) {
		uint8_t action, flags;
		unpack_AddWrite_respond((uint8_t) packet->data[0], &action, &flags);
		csp_buffer_free(packet);
		break;
	}

	csp_close(conn);
	uint32_t end_time = csp_get_ms();
	return (int8_t) (end_time - start_time);
}

int get_register_csp(unsigned int device_id, unsigned int register_id, unsigned int sub_register, uint8_t type_register,
		void *outValue, uint16_t timeout) {

	uint32_t start_time = csp_get_ms();

	if (device_id == csp_get_address()) {
		if (get_register(device_id, register_id, sub_register, outValue) == CSP_ERR_NONE) {
			uint32_t end_time = csp_get_ms();
			return (int8_t) (end_time - start_time);
		}
		return CSP_ERR_ALREADY;
	}
	csp_conn_t *conn = csp_connect(CSP_PRIO_NORM, device_id, PORT_REGISTER, timeout, CSP_O_NONE);
	if (conn == NULL) {
		csp_log_error("Connection failed\r\n");
		return CSP_ERR_NOMEM;
	}

	uint8_t buffer[4];
	pack_ReadRequest(buffer, READ_REGISTER, device_id, register_id, sub_register, type_register, R_ENFORCE);
	size_t packet_size = sizeof(buffer);
	csp_packet_t *packet = csp_buffer_get(packet_size);
	if (packet == NULL) {
		csp_log_error("Failed to get CSP buffer\r\n");
		csp_close(conn);
		return CSP_ERR_BUSY;
	}

	memcpy(packet->data, buffer, packet_size);
	packet->length = packet_size;
	if (!csp_send(conn, packet, timeout)) {
		csp_log_error("Send failed\r\n");
		csp_buffer_free(packet);
		csp_close(conn);
		return CSP_ERR_TX;
	}

	while ((packet = csp_read(conn, timeout)) != NULL) {
		uint8_t action, flags;
		size_t value_size = get_value_size(type_register);

		unpack_ReadResponse((uint8_t*) packet->data, &action, &flags, NULL, value_size);

		if (value_size > 0 && flags == FLAGS_SUCCESS) {
			memcpy(outValue, packet->data + 1, value_size);
		} else {
			csp_log_error("Unsupported flags:%u\n", flags);
			csp_buffer_free(packet);
			csp_close(conn);
			return CSP_ERR_INVAL;
		}

		csp_buffer_free(packet);
		break;
	}
	csp_close(conn);
	uint32_t end_time = csp_get_ms();
	return (int8_t) (end_time - start_time);
}

