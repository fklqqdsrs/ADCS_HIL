/*
 * bit_management.h
 *
 * Created on: Sep 23, 2024
 * Author: thaib
 */

#ifndef SRC_REGISTER_BIT_MANAGEMENT_H_
#define SRC_REGISTER_BIT_MANAGEMENT_H_

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

#define DEBUG_BIT_MANA 1
#if DEBUG_BIT_MANA
#define DEBUG_PRINT_BIT(fmt, ...) \
    printf(fmt, ##__VA_ARGS__)
#else
#define DEBUG_PRINT_BIT(fmt, ...) \
    do                               \
    {                                \
    } while (0)
#endif

void set_bits(uint64_t *data, uint64_t value, int start_bit, int bit_length)  ;
uint64_t get_bits(uint64_t data, uint8_t bit_start, uint8_t bit_length) ;
void set_bits_uint8(uint8_t *data, uint8_t value, uint8_t start, uint8_t length);
uint8_t get_bits_uint8(uint8_t data, uint8_t start, uint8_t length) ;
size_t get_value_size(uint8_t type_register) ;
void pack_AddWriteRequest(uint8_t **buffer, uint8_t action, uint8_t device_id, uint8_t register_id, uint16_t sub_register, uint8_t type_register, uint8_t access, uint64_t value) ;
void unpack_AddWriteRequest(uint8_t *buffer, uint8_t *action, uint8_t *device_id, uint8_t *register_id, uint16_t *sub_register, uint8_t *type_register, uint8_t *access, void *value) ;
void pack_AddWrite_respond(uint8_t *buffer, uint8_t action, uint8_t flags) ;
void unpack_AddWrite_respond(uint8_t buffer, uint8_t *action, uint8_t *flags);
void pack_ReadRequest(uint8_t *buffer, uint8_t action, uint8_t device_id, uint8_t register_id, uint16_t sub_register, uint8_t type_register, uint8_t access) ;
void unpack_ReadRequest(uint8_t *buffer, uint8_t *action, uint8_t *device_id, uint8_t *register_id, uint16_t *sub_register, uint8_t *type_register, uint8_t *access) ;

void pack_ReadResponse(uint8_t *buffer, uint8_t action, uint8_t flags, uint8_t register_id, uint16_t sub_register, const void *value_ptr, size_t value_size);
void unpack_ReadResponse(uint8_t *buffer, uint8_t *action, uint8_t *flags, uint8_t *register_id, uint16_t *sub_register, void *outValue, size_t value_size);


#ifdef __cplusplus
}
#endif

#endif /* SRC_REGISTER_BIT_MANAGEMENT_H_ */