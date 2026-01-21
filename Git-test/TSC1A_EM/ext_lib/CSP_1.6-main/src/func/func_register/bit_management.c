/*
 * csp_register_service.c
 *
 *  Created on: Sep 23, 2024
 *      Author: thaib
 */

#ifndef SRC_REGISTER_BIT_MANA_C_
#define SRC_REGISTER_BIT_MANA_C_

#include <src/func/func_register/bit_management.h>


void set_bits(uint32_t *data, uint32_t value, uint8_t start, uint8_t length) {
	uint32_t mask = ((1U << length) - 1) << start;
	*data = (*data & ~mask) | ((value << start) & mask);
}

uint32_t get_bits(uint32_t data, uint8_t start, uint8_t length) {
	uint32_t mask = (1U << length) - 1;
	return (data >> start) & mask;
}

void set_bits_uint8(uint8_t *data, uint8_t value, uint8_t start, uint8_t length) {
    uint8_t mask = ((1U << length) - 1) << start;
    *data = (*data & ~mask) | ((value << start) & mask);
}
uint8_t get_bits_uint8(uint8_t data, uint8_t start, uint8_t length) {
    uint8_t mask = (1U << length) - 1;
    return (data >> start) & mask;
}


size_t get_value_size(uint8_t type_register) {
	switch (type_register) {
	case 1:
		return sizeof(uint8_t);    // U8
	case 2:
		return sizeof(int8_t);     // I8
	case 3:
		return sizeof(uint16_t);   // U16
	case 4:
		return sizeof(int16_t);    // I16
	case 5:
		return sizeof(uint32_t);   // U32
	case 6:
		return sizeof(int32_t);    // I32
	case 7:
		return sizeof(float);      // FLOAT
	case 8:
		return sizeof(double);     // DOUBLE
	default:
		return 0;
	}
}

void pack_AddWriteRequest(uint8_t **buffer, uint8_t action, uint8_t device_id, uint8_t register_id, uint8_t sub_register, uint8_t type_register, uint8_t access, uint64_t value) {
	uint32_t packed_data = 0;
	size_t value_size = get_value_size(type_register);
	if (value_size == 0) {
		printf("Invalid type_register\n");
		return;
	}

	*buffer = malloc(4 + value_size);
	if (*buffer == NULL) {
		printf("Memory allocation failed\n");
		return;
	}

	set_bits((uint32_t*) &packed_data, action, 28, 4);         // Action: 4 bits starting at bit 28
	set_bits((uint32_t*) &packed_data, device_id, 23, 5);      // Device ID: 5 bits starting at bit 23
	set_bits((uint32_t*) &packed_data, register_id, 15, 8);    // Register ID: 8 bits starting at bit 15
	set_bits((uint32_t*) &packed_data, sub_register, 7, 8);    // Sub Register: 8 bits starting at bit 7
	set_bits((uint32_t*) &packed_data, type_register, 3, 4);   // Type Register: 4 bits starting at bit 3
	set_bits((uint32_t*) &packed_data, access, 0, 3);          // Access: 3 bits starting at bit 0

	(*buffer)[0] = (packed_data >> 24) & 0xFF;
	(*buffer)[1] = (packed_data >> 16) & 0xFF;
	(*buffer)[2] = (packed_data >> 8) & 0xFF;
	(*buffer)[3] = packed_data & 0xFF;

	memcpy(*buffer + 4, &value, value_size);
}

void unpack_AddWriteRequest(uint8_t *buffer, uint8_t *action, uint8_t *device_id, uint8_t *register_id, uint8_t *sub_register, uint8_t *type_register, uint8_t *access, uint64_t *value) {
	uint32_t packed_data = 0;

	packed_data |= (uint64_t) buffer[0] << 24;
	packed_data |= (uint64_t) buffer[1] << 16;
	packed_data |= (uint64_t) buffer[2] << 8;
	packed_data |= (uint64_t) buffer[3];

	*action = get_bits(packed_data, 28, 4);
	*device_id = get_bits(packed_data, 23, 5);
	*register_id = get_bits(packed_data, 15, 8);
	*sub_register = get_bits(packed_data, 7, 8);
	*type_register = get_bits(packed_data, 3, 4);
	*access = get_bits(packed_data, 0, 3);

	size_t value_size = get_value_size(*type_register);
	if (value_size > 0) {
		*value = 0;
		memcpy(value, buffer + 4, value_size);
	}
}

void pack_AddWrite_respond(uint8_t *buffer, uint8_t action, uint8_t flags) {
    *buffer = 0;
    set_bits_uint8(buffer, action, 4, 4);
    set_bits_uint8(buffer, flags, 0, 4);
}

void unpack_AddWrite_respond(uint8_t buffer, uint8_t *action, uint8_t *flags) {
    *action = get_bits_uint8(buffer, 4, 4);
    *flags = get_bits_uint8(buffer, 0, 4);
}

void pack_ReadRequest(uint8_t *buffer, uint8_t action, uint8_t device_id, uint8_t register_id, uint8_t sub_register, uint8_t type_register, uint8_t access) {
	uint32_t packed_data = 0;

	set_bits(&packed_data, action, 28, 4);        // Action: 4 bits starting at bit 28
	set_bits(&packed_data, device_id, 23, 5);     // Device ID: 5 bits starting at bit 23
	set_bits(&packed_data, register_id, 15, 8);   // Register ID: 8 bits starting at bit 15
	set_bits(&packed_data, sub_register, 7, 8);   // Sub Register: 8 bits starting at bit 7
	set_bits(&packed_data, type_register, 3, 4);  // Type Register: 4 bits starting at bit 3
	set_bits(&packed_data, access, 0, 3);         // Access: 3 bits starting at bit 0

	buffer[0] = (packed_data >> 24) & 0xFF;
	buffer[1] = (packed_data >> 16) & 0xFF;
	buffer[2] = (packed_data >> 8) & 0xFF;
	buffer[3] = packed_data & 0xFF;
}

void unpack_ReadRequest(uint8_t *buffer, uint8_t *action, uint8_t *device_id, uint8_t *register_id, uint8_t *sub_register, uint8_t *type_register, uint8_t *access) {
	uint32_t packed_data = 0;

	packed_data |= (uint32_t) buffer[0] << 24;
	packed_data |= (uint32_t) buffer[1] << 16;
	packed_data |= (uint32_t) buffer[2] << 8;
	packed_data |= (uint32_t) buffer[3];

	*action = get_bits(packed_data, 28, 4);
	*device_id = get_bits(packed_data, 23, 5);
	*register_id = get_bits(packed_data, 15, 8);
	*sub_register = get_bits(packed_data, 7, 8);
	*type_register = get_bits(packed_data, 3, 4);
	*access = get_bits(packed_data, 0, 3);
}

void pack_ReadResponse(uint8_t *buffer, uint8_t action, uint8_t flags, uint64_t value, size_t value_size) {
	uint8_t packed_data = 0;
	set_bits_uint8(&packed_data, action, 4, 4);
	set_bits_uint8(&packed_data, flags, 0, 4);

	buffer[0] = packed_data & 0xFF;
	memcpy(buffer + 3, &value, value_size);
}

void unpack_ReadResponse(uint8_t *buffer, uint8_t *action, uint8_t *flags, uint64_t *value, size_t value_size) {
	uint8_t packed_data = buffer[0];
	*action = get_bits_uint8(packed_data, 4, 4);
	*flags = get_bits_uint8(packed_data, 0, 4);
	*value = 0;
	memcpy(value, buffer + 3, value_size);
}


#endif /* SRC_REGISTER_BIT_MANA_C_ */
