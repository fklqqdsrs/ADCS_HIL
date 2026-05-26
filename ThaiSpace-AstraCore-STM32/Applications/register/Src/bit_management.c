#include "app_modules_config.h"
#if CSP_ENABLE_REGISTER

/*
 * bit_management.c
 *
 *  Created on: Sep 23, 2024
 *      Author: thaib
 */

#include "bit_management.h"

void set_bits(uint64_t *data, uint64_t value, int start_bit, int bit_length) {
	uint64_t mask = ((1ULL << bit_length) - 1) << start_bit;
	*data = (*data & ~mask) | ((value & ((1ULL << bit_length) - 1)) << start_bit);
}

uint64_t get_bits(uint64_t data, uint8_t bit_start, uint8_t bit_length) {
	uint64_t mask = (1ULL << bit_length) - 1;
	return (data >> bit_start) & mask;
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

void pack_AddWriteRequest(uint8_t **buffer, uint8_t action, uint8_t device_id, uint8_t register_id, uint16_t sub_register, uint8_t type_register,
		uint8_t access, uint64_t value) {
	uint64_t packed_data = 0;
	size_t value_size = get_value_size(type_register);

	if (value_size == 0) {
		DEBUG_PRINT_BIT("[PACK] Invalid type_register\n");
		return;
	}

	*buffer = malloc(5 + value_size);
	if (*buffer == NULL) {
		DEBUG_PRINT_BIT("[PACK] Memory allocation failed\n");
		return;
	}

	set_bits((uint64_t*) &packed_data, action, 60, 4);
	set_bits((uint64_t*) &packed_data, device_id, 55, 5);
	set_bits((uint64_t*) &packed_data, register_id, 47, 8);
	set_bits((uint64_t*) &packed_data, sub_register, 31, 16);
	set_bits((uint64_t*) &packed_data, type_register, 27, 4);
	set_bits((uint64_t*) &packed_data, access, 24, 3);

	(*buffer)[0] = (packed_data >> 56) & 0xFF;
	(*buffer)[1] = (packed_data >> 48) & 0xFF;
	(*buffer)[2] = (packed_data >> 40) & 0xFF;
	(*buffer)[3] = (packed_data >> 32) & 0xFF;
	(*buffer)[4] = (packed_data >> 24) & 0xFF;

	memcpy(*buffer + 5, &value, value_size);

}

void unpack_AddWriteRequest(uint8_t *buffer, uint8_t *action, uint8_t *device_id, uint8_t *register_id, uint16_t *sub_register,
		uint8_t *type_register, uint8_t *access, void *value) { // <--- เปลี่ยน uint64_t* เป็น void*
	uint64_t packed_data = 0;

	packed_data |= (uint64_t) buffer[0] << 56;
	packed_data |= (uint64_t) buffer[1] << 48;
	packed_data |= (uint64_t) buffer[2] << 40;
	packed_data |= (uint64_t) buffer[3] << 32;
	packed_data |= (uint64_t) buffer[4] << 24;

	*action = get_bits(packed_data, 60, 4);
	*device_id = get_bits(packed_data, 55, 5);
	*register_id = get_bits(packed_data, 47, 8);
	*sub_register = get_bits(packed_data, 31, 16);
	*type_register = get_bits(packed_data, 27, 4);
	*access = get_bits(packed_data, 24, 3);

	size_t value_size = get_value_size(*type_register);
	if (value_size > 0 && value != NULL) {
		memcpy(value, buffer + 5, value_size);
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

void pack_ReadRequest(uint8_t *buffer, uint8_t action, uint8_t device_id, uint8_t register_id, uint16_t sub_register, uint8_t type_register,
		uint8_t access) {
	uint64_t packed_data = 0;

	set_bits(&packed_data, action, 60, 4);        // Action: 4 bits starting at bit 60
	set_bits(&packed_data, device_id, 55, 5);     // Device ID: 5 bits starting at bit 55
	set_bits(&packed_data, register_id, 47, 8);   // Register ID: 8 bits starting at bit 47
	set_bits(&packed_data, sub_register, 31, 16); // Sub Register: 16 bits starting at bit 31
	set_bits(&packed_data, type_register, 27, 4); // Type Register: 4 bits starting at bit 27
	set_bits(&packed_data, access, 24, 3);        // Access: 3 bits starting at bit 24

	buffer[0] = (packed_data >> 56) & 0xFF;
	buffer[1] = (packed_data >> 48) & 0xFF;
	buffer[2] = (packed_data >> 40) & 0xFF;
	buffer[3] = (packed_data >> 32) & 0xFF;
	buffer[4] = (packed_data >> 24) & 0xFF;
}

void unpack_ReadRequest(uint8_t *buffer, uint8_t *action, uint8_t *device_id, uint8_t *register_id, uint16_t *sub_register, uint8_t *type_register,
		uint8_t *access) {
	uint64_t packed_data = 0;

	packed_data |= (uint64_t) buffer[0] << 56;
	packed_data |= (uint64_t) buffer[1] << 48;
	packed_data |= (uint64_t) buffer[2] << 40;
	packed_data |= (uint64_t) buffer[3] << 32;
	packed_data |= (uint64_t) buffer[4] << 24;

	*action = get_bits(packed_data, 60, 4);
	*device_id = get_bits(packed_data, 55, 5);
	*register_id = get_bits(packed_data, 47, 8);
	*sub_register = get_bits(packed_data, 31, 16);
	*type_register = get_bits(packed_data, 27, 4);
	*access = get_bits(packed_data, 24, 3);
}

void pack_ReadResponse(uint8_t *buffer, uint8_t action, uint8_t flags, uint8_t register_id, uint16_t sub_register, const void *value_ptr,
		size_t value_size) {
	uint8_t packed_data = 0;
	set_bits_uint8(&packed_data, action, 4, 4);
	set_bits_uint8(&packed_data, flags, 0, 4);

	buffer[0] = packed_data;
	buffer[1] = register_id;
	buffer[2] = (uint8_t) ((sub_register >> 8) & 0xFF);
	buffer[3] = (uint8_t) (sub_register & 0xFF);

	if (value_ptr && value_size > 0) {
		memcpy(buffer + 4, value_ptr, value_size);
	}
}

void unpack_ReadResponse(uint8_t *buffer, uint8_t *action, uint8_t *flags, uint8_t *register_id, uint16_t *sub_register, void *outValue,
		size_t value_size) {
	uint8_t packed_data = buffer[0];
	if (action) *action = get_bits_uint8(packed_data, 4, 4);
	if (flags) *flags = get_bits_uint8(packed_data, 0, 4);

	if (register_id) *register_id = buffer[1];
	if (sub_register) *sub_register = ((uint16_t) buffer[2] << 8) | buffer[3];

	if (value_size > 0 && outValue != NULL) {
		memcpy(outValue, buffer + 4, value_size);
	}
}



#endif /* CSP_ENABLE_REGISTER */
