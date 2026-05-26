/*
 * csp_register_service.h
 *
 *  Created on: Sep 18, 2024
 *      Author: thaib
 */

#ifndef SRC_REGISTER_CSP_REGISTER_SERVICE_H_
#define SRC_REGISTER_CSP_REGISTER_SERVICE_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <include/csp/arch/csp_time.h>
#include <src/func/func_register/bit_management.h>
#include <src/func/func_register/csp_register.h>


#ifdef __cplusplus
extern "C" {
#endif

#define MAX_REGISTERS 200

//CSP
#define PORT_REGISTER 20

// action  4bit  0..0x0F
#define NONE_REGISTER 0x00
#define ADD_REGISTER 0x01
#define READ_REGISTER 0x02
#define WRITE_REGISTER 0x03
#define REMOVE_REGISTER 0x04
#define GET_REGISTER_COUNT 0x05
#define GET_STATUS_REGISTER 0x06


//flags   4bit 0..0x0F
#define FLAGS_NONE 0x00
#define FLAGS_SUCCESS 0x01
#define FLAGS_BUSY 0x02
#define FLAGS_TIMEOUT 0x03
#define FLAGS_NOT_FOUND 0x04


//access  3bit   0..0x07
#define R_NONE 0x00
#define R_ENFORCE 0x01
#define R_SUCCESS 0x02
#define R_BUSY 0x03
#define R_TIMEOUT 0x04
#define R_NOT_FOUND 0x05


#define ACTION_SIZE     4
#define DEVICE_SIZE     5
#define REGISTER_ID_SIZE 8
#define SUB_REGISTER_SIZE 8
#define TYPE_SIZE       4
#define ACCESS_SIZE     3
#define FLAGS_SIZE      4



typedef union {
	uint8_t u8;
	int8_t i8;
	uint16_t u16;
	int16_t i16;
	uint32_t u32;
	int32_t i32;
	float f;
	double d;
} RegisterValue;


/**
 * The function `generic_add_register` adds a register of a specified type to a device with the given
 * parameters.
 * 
 * @param device_id The `device_id` parameter represents the ID of the device for which the register is
 * being added.
 * @param register_id The `register_id` parameter in the `generic_add_register` function is used to
 * specify the ID of the register to be added for a particular device.
 * @param sub_register The `sub_register` parameter in the `generic_add_register` function represents a
 * specific sub-register within a register. It is used to identify a particular field or subset of data
 * within a larger register. This parameter helps in specifying the exact location or purpose of the
 * data being accessed or modified within the register
 * @param type_register The `type_register` parameter in the `generic_add_register` function is used to
 * specify the data type of the register value being added. It is an enumeration that can take on the
 * following values:
 * @param value The `value` parameter in the `generic_add_register` function is a pointer to a
 * structure of type `RegisterValue`. This structure likely contains different fields for storing
 * values of various data types such as `uint8_t`, `int8_t`, `uint16_t`, `int16_t`,
 * @param access The `access` parameter in the `generic_add_register` function represents the access
 * level for the register being added. It specifies the permissions or restrictions on reading or
 * writing to the register. The access level could be defined as an integer value or using predefined
 * constants to indicate different access levels such as read-only
 * 
 * @return The function `generic_add_register` returns a boolean value based on the result of calling
 * the `add_register` function with the appropriate parameters depending on the `type_register`
 * provided as input.
 */
bool generic_add_register(uint8_t device_id, uint8_t register_id, uint8_t sub_register, uint8_t type_register, RegisterValue *value, uint8_t access) ;



/**
 * The function `generic_set_register` sets a register value based on the type of the register.
 * 
 * @param device_id The `device_id` parameter represents the ID of the device for which the register is
 * being set. It is of type `uint8_t`, which typically means an unsigned 8-bit integer.
 * @param register_id The `register_id` parameter in the `generic_set_register` function is used to
 * specify the ID of the register to be set in the device. It helps identify the specific register
 * within the device's memory map where the value will be written.
 * @param sub_register The `sub_register` parameter in the `generic_set_register` function is used to
 * specify a sub-register within a register. It helps in identifying a specific part or field within a
 * register that needs to be set or modified.
 * @param type_register The `type_register` parameter in the `generic_set_register` function is used to
 * determine the data type of the value being set in the register. It is an enum that specifies the
 * type of the register value. The function then calls the `set_register` function with the appropriate
 * data type based on
 * @param value The `value` parameter in the `generic_set_register` function is a pointer to a
 * structure of type `RegisterValue`. This structure likely contains union members for different data
 * types such as `uint8_t`, `int8_t`, `uint16_t`, `int16_t`, `uint32
 * 
 * @return The function `generic_set_register` returns a boolean value based on the result of calling
 * the `set_register` function with the appropriate type of value based on the `type_register`
 * parameter.
 */
bool generic_set_register(uint8_t device_id, uint8_t register_id, uint8_t sub_register, uint8_t type_register, RegisterValue *value) ;



/**
 * The function `service_register_demo` processes requests related to adding, writing, and reading
 * registers, sending appropriate responses based on the actions performed.
 * 
 * @param conn conn is a pointer to a structure representing a connection in the communication protocol
 * being used in the code.
 * @param packet The `packet` parameter is a pointer to a `csp_packet_t` structure, which likely
 * contains data received over a communication channel. The structure may include information such as
 * the data payload, length, source, destination, and other relevant details of the packet. In the
 * provided code snippet, the
 */
void service_register_demo(csp_conn_t *conn, csp_packet_t *packet);


int convert_and_get_size(void *value, uint8_t type_register, uint64_t *packed_value, size_t *value_size) ;


/**
 * The function `add_register_csp` establishes a connection, packs data, sends it over CSP, receives a
 * response, and unpacks the response.
 * 
 * @param device_id The `device_id` parameter in the `add_register_csp` function represents the ID of
 * the device to which you want to add a register. It is an unsigned integer value.
 * @param register_id The `register_id` parameter in the `add_register_csp` function represents the
 * identifier of the register to which you want to add a value. It is an unsigned integer that helps
 * identify a specific register within the device.
 * @param sub_register The `sub_register` parameter in the `add_register_csp` function represents a
 * specific sub-register within a register identified by `register_id`. It is used to target a
 * particular subset or section of data within the specified register for reading or writing
 * operations.
 * @param type_register The `type_register` parameter in the `add_register_csp` function represents the
 * type of the register being accessed. It is of type `uint8_t` and is used to specify the data type of
 * the value being passed to the register. This parameter helps in converting and packing the value
 * appropriately
 * @param value The `value` parameter in the `add_register_csp` function is a pointer to the data that
 * needs to be written to the specified register. It is a void pointer, which means it can point to
 * data of any type. The function will internally convert and pack this data based on the `
 * @param access_mode The `access_mode` parameter in the `add_register_csp` function specifies the
 * access mode for the register being added. It is of type `uint8_t`, which means it is an 8-bit
 * unsigned integer. The access mode typically defines how the register can be accessed or modified,
 * such
 * @param timeout The `timeout` parameter in the `add_register_csp` function represents the maximum
 * time in milliseconds to wait for a response before giving up on the operation. It is used for
 * setting the timeout value for establishing a connection, sending data, and receiving responses
 * within the specified time limit.
 * 
 * @return The function `add_register_csp` returns an integer value, which could be one of the
 * following:
 * - `CSP_ERR_NOMEM` if the connection failed
 * - `CSP_ERR_INVAL` if there was an invalid type_register or failed to pack data
 * - `CSP_ERR_BUSY` if failed to get a CSP buffer
 * - `CSP_ERR_TX` if the send
 */
int add_register_csp(unsigned int device_id, unsigned int register_id, unsigned int sub_register, uint8_t type_register, void *value, uint8_t access_mode, uint16_t timeout) ;


/**
 * The function `set_register_csp` establishes a connection, packs data, sends a packet, receives a
 * response, and closes the connection for setting a register in a device.
 * 
 * @param device_id The `device_id` parameter in the `set_register_csp` function represents the unique
 * identifier of the device to which you want to set a register. It is an unsigned integer value.
 * @param register_id The `register_id` parameter in the `set_register_csp` function represents the
 * identifier of the register to which you want to set a value. It is an unsigned integer that helps
 * identify a specific register within the device.
 * @param sub_register The `sub_register` parameter in the `set_register_csp` function represents a
 * specific sub-register within the register identified by `register_id`. It is used to target a
 * particular field or subset of data within the register for modification or retrieval.
 * @param type_register The `type_register` parameter in the `set_register_csp` function represents the
 * type of the register being set. It is of type `uint8_t` and is used to specify the data type of the
 * value being set in the register. This parameter helps in converting the value to the appropriate
 * @param value The `value` parameter in the `set_register_csp` function is a pointer to the data that
 * needs to be set in the specified register. It is a void pointer, which means it can point to data of
 * any type. The function will convert this data based on the `type_register`
 * @param timeout The `timeout` parameter in the `set_register_csp` function represents the maximum
 * time in milliseconds to wait for a response before giving up on the operation. It is used to control
 * the duration for which the function will wait for a connection, send data, and receive a response
 * before timing out.
 * 
 * @return The function `set_register_csp` returns an integer value, which could be one of the
 * following based on the execution flow:
 * - `CSP_ERR_NOMEM` if the connection failed and there was no memory available.
 * - `CSP_ERR_INVAL` if there was an invalid type register or failed to pack data.
 * - `CSP_ERR_BUSY` if failed to get a CSP buffer
 */
int set_register_csp(unsigned int device_id, unsigned int register_id, unsigned int sub_register, uint8_t type_register, void *value, uint16_t timeout) ;



/**
 * The function `get_register_csp` establishes a connection to a device, sends a read request for a
 * specific register, receives and processes the response, and returns the time taken for the
 * operation.
 * 
 * @param device_id The `device_id` parameter in the `get_register_csp` function represents the ID of
 * the device from which you want to retrieve a register value. It is an unsigned integer used to
 * identify the specific device in the communication network.
 * @param register_id The `register_id` parameter in the `get_register_csp` function represents the
 * identifier of the register you want to access on a specific device. It is used to specify which
 * register you are targeting for reading its value.
 * @param sub_register The `sub_register` parameter in the `get_register_csp` function represents a
 * specific sub-register within the register identified by `register_id`. It is used to specify a
 * particular subset or section of data within the register for reading or manipulation. This parameter
 * helps in accessing specific parts of a register that
 * @param type_register The `type_register` parameter in the `get_register_csp` function is of type
 * `uint8_t`. It is used to specify the type of register being accessed or read. This parameter helps
 * in determining how to interpret the data received from the register read operation. The function
 * uses this parameter to
 * @param outValue The `outValue` parameter in the `get_register_csp` function is a pointer to a memory
 * location where the function will store the value read from the specified register on the device. The
 * function will copy the value read from the register into the memory location pointed to by
 * `outValue`.
 * @param timeout The `timeout` parameter in the `get_register_csp` function represents the maximum
 * time allowed for the function to wait for a response before timing out. It is specified in
 * milliseconds and determines how long the function will wait for a response from the device before
 * giving up and returning an error code.
 * 
 * @return The function `get_register_csp` returns an integer value, which represents the time taken in
 * milliseconds to complete the operation.
 */
int get_register_csp(unsigned int device_id, unsigned int register_id, unsigned int sub_register, uint8_t type_register, void *outValue, uint16_t timeout) ;

#ifdef __cplusplus
 }
#endif

#endif /* SRC_REGISTER_CSP_REGISTER_SERVICE_H_ */
