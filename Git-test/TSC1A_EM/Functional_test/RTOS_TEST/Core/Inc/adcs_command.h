/*
 * adcs_command.h
 *
 *  Created on: Feb 17, 2025
 *      Author: Chinnapop
 */

#ifndef INC_ADCS_COMMAND_H_
#define INC_ADCS_COMMAND_H_

/*
 * csp_register_service_handler.c
 *
 *  Created on: Sep 19, 2024
 *      Author: thaib
 */
#include "RTOS_LIB/TASK_TSC1.h"
#include <src/func/func_register/csp_register_service.h>
//#include "csp_register.h"

#define TIMEOUT_CSP_REG 1000
#define GET_BUFFER_SIZE 50


typedef enum{
	Nothing,
	mtq1,
	mtq2,
	mtq3,
	rwa1,
	rwa2,
	rwa3,
	rwa4,
	memory,
	imu,
	gps,
	mtm,
	fss,
	stt,

}csp_reg_command;


uint8_t get_buffer[GET_BUFFER_SIZE];

void mutex_csp_init()
{
    if (csp_mutex_create(&register_mutex) == CSP_MUTEX_OK)
    {
       // printf("Mutex created successfully.\n");
    }

}

void pack_adcs_ReadResponse(uint8_t *buffer, uint8_t action, uint8_t flags, uint8_t* value, size_t value_size) {
	uint8_t packed_data = 0;
	set_bits_uint8(&packed_data, action, 4, 4);
	set_bits_uint8(&packed_data, flags, 0, 4);

	buffer[0] = packed_data & 0xFF;
	memcpy(buffer + 3, value, value_size);
}


bool get_adcs_register(unsigned int register_id, unsigned int sub_register)
{

	if((register_id > 0 )&&(register_id < 14))
	{
		switch(register_id)
		{
			case mtq1:
				mtq_reg[MTQ1][sub_register].get_bytes(get_buffer);
				break;;
			case mtq2:
				mtq_reg[MTQ2][sub_register].get_bytes(get_buffer);
				break;
			case mtq3:
				mtq_reg[MTQ3][sub_register].get_bytes(get_buffer);
				break;
			case rwa1:
				whl500_reg[WHEEL1][sub_register].get_bytes(get_buffer);
				break;
			case rwa2:
				whl500_reg[WHEEL2][sub_register].get_bytes(get_buffer);
				break;
			case rwa3:
				whl500_reg[WHEEL3][sub_register].get_bytes(get_buffer);
				break;
			case rwa4:
				whl500_reg[WHEEL4][sub_register].get_bytes(get_buffer);
				break;
			case memory:
				REG_TSC1[sub_register].get_bytes(get_buffer);
				break;
			case imu:
				imu_reg[sub_register].get_bytes(get_buffer);
				break;
			case gps:
				oem719_reg[sub_register].get_bytes(get_buffer);
				break;
			case mtm:
				cubemag_reg[sub_register].get_bytes(get_buffer);
				break;
			case fss:
				fss_reg[id].get_bytes(get_buffer);
				//cubemag_reg[sub_register].get_bytes(get_buffer);
				break;
		}

	}

	csp_log_error("There is no device with this ID\n");
	return false;

}

uint8_t get_adcs_register_len(unsigned int register_id, unsigned int sub_register)
{

	if((register_id > 0 )&&(register_id < 12))
	{
		switch(register_id)
		{
			case mtq1:
				return mtq_reg[MTQ1][sub_register].get_len();

			case mtq2:
				return mtq_reg[MTQ2][sub_register].get_len();

			case mtq3:
				return mtq_reg[MTQ3][sub_register].get_len();

			case rwa1:
				return whl500_reg[WHEEL1][sub_register].get_len();

			case rwa2:
				return whl500_reg[WHEEL2][sub_register].get_len();

			case rwa3:
				return whl500_reg[WHEEL3][sub_register].get_len();

			case rwa4:
				return whl500_reg[WHEEL4][sub_register].get_len();

			case memory:
				return REG_TSC1[sub_register].get_len();

			case imu:
				return imu_reg[sub_register].get_len();

			case gps:
				return oem719_reg[sub_register].get_len();

			case mtm:
				return 4;
				//return cubemag_reg[sub_register].get_len();
			case fss:
				return fss_reg[sub_register].get_len();


		}

	}

	return false;
}


uint8_t access_checking(unsigned int register_id)
{
	if((register_id > 0) && (register_id <9))
	{
		return READ_ONLY;
	}
	else
	{
		return READ_WRITE;
	}
	return NONE_MODE;

}

int8_t set_adcs_register(unsigned int device_id, unsigned int register_id, unsigned int sub_register, void *value)
{
 //   csp_mutex_lock(&register_mutex, CSP_MAX_TIMEOUT);

//    if (register_id > 0)
//    {
//        if (access_checking(register_id) == READ_ONLY)
//        {
//            printf("Cannot set value. Register is read-only.\n");
//            csp_mutex_unlock(&register_mutex);
//            return -1;
//        }
    if (device_id == 12)
    {
           switch(register_id )
           {
            	case(mtq1):
					mtq_reg[MTQ1][PWM].set_bytes((uint8_t*)value,sizeof(int16_t),LittleEndian);
            	    break;

            	case(mtq2):
					mtq_reg[MTQ2][PWM].set_bytes((uint8_t*)value,sizeof(int16_t),LittleEndian);
            		break;
            	case(mtq3):
					mtq_reg[MTQ3][PWM].set_bytes((uint8_t*)value,sizeof(int16_t),LittleEndian);

            	case(rwa1):
					whl500_reg[WHEEL1][sub_register].set_bytes((uint8_t*)value, 4,LittleEndian);

            	case(rwa2):
					whl500_reg[WHEEL2][sub_register].set_bytes((uint8_t*)value, 4,LittleEndian);

            	case(rwa3):
					whl500_reg[WHEEL3][mrpm_target].set_bytes((uint8_t*)value, 4,LittleEndian);

            	case(rwa4):
					whl500_reg[WHEEL4][sub_register].set_bytes((uint8_t*)value, 4,LittleEndian);
            	case(memory):
            		REG_TSC1[sub_register].set_bytes((uint8_t*)value,REG_TSC1[sub_register].get_len(),LittleEndian);

           }
  //         csp_mutex_unlock(&register_mutex);
           return 1;
    }

//        else
//        {
//            printf("Unknown register type.\n");
//            csp_mutex_unlock(&register_mutex);
//            return -2;
//        }
//    }
//    else
//    {
//        printf("Register not found. Please add it first.\n");
// //       csp_mutex_unlock(&register_mutex);
//        return -3;
//    }

  //  csp_mutex_unlock(&register_mutex);
//    return CSP_ERR_NONE;
}


void service_adcs_register(csp_conn_t *conn, csp_packet_t *packet) {
	uint8_t action = packet->data[0] >> 4;
//	uint8_t action ;
	if (action == ADD_REGISTER || action == WRITE_REGISTER) {
		uint8_t device_id, type_register, access, flags;
		uint8_t register_id, sub_register;
		RegisterValue outValue;

		unpack_AddWriteRequest(packet->data, &action, &device_id, &register_id, &sub_register, &type_register, &access, (uint64_t*) &outValue);
		size_t value_size = get_value_size(type_register);
		if (value_size == 0) {
			csp_log_error("Invalid type_register size\r\n");
			goto cleanup;
		}

		if (action == ADD_REGISTER) {

			csp_log_error("Must not add registers\n");

			goto cleanup;
		} else if (action == WRITE_REGISTER) {

			if (!set_adcs_register(device_id, register_id, sub_register, &outValue)) {
				csp_log_error("Failed to set register\n");
				flags = FLAGS_BUSY;
			}
			flags = FLAGS_SUCCESS;
		}

		csp_packet_t *AddWriteRequest_packet = (csp_packet_t*)csp_buffer_get(3);
		if (!AddWriteRequest_packet) {
			csp_log_error("AddWriteRequest_packet Failed to get buffer for response\n");
			goto cleanup;
		}

		pack_AddWrite_respond(AddWriteRequest_packet->data, action, flags);

        AddWriteRequest_packet->data[1] = register_id;
        AddWriteRequest_packet->data[2]= sub_register;
        AddWriteRequest_packet->length = 3;
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
		if (get_adcs_register(register_id, sub_register) == 0) {
			csp_log_error("Failed to get register value\n");
			flags = FLAGS_NOT_FOUND;
			//packed_value = 0;
		}
		size_t value_size = get_adcs_register_len(register_id, sub_register);
//		if (convert_and_get_size(&outValue, type_register, &packed_value, &value_size) != 0) {
//			csp_log_error("Invalid type_register\r\n");
//			goto cleanup;
//		}

		csp_packet_t *response_packet = (csp_packet_t*)csp_buffer_get(1 + value_size);
		if (!response_packet) {
			csp_log_error("response_packet Failed to get buffer for response\n");
			goto cleanup;
		}
		flags = FLAGS_SUCCESS;
		pack_adcs_ReadResponse(response_packet->data, action, flags, get_buffer, value_size);
        response_packet ->data [1]= register_id;
        response_packet ->data [2]= sub_register;
        response_packet->length = 3 + value_size;


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

//void service_register_demo(csp_conn_t *conn, csp_packet_t *packet) {
//    uint8_t action = packet->data[0] >> 4;
//
//    if (action == ADD_REGISTER || action == WRITE_REGISTER) {
//        uint8_t device_id, type_register, access, flags;
//        uint8_t register_id, sub_register;
//        RegisterValue outValue;
//
//        unpack_AddWriteRequest(packet->data, &action, &device_id, &register_id, &sub_register, &type_register, &access, (void*) &outValue);
//        size_t value_size = get_value_size(type_register);
//        if (value_size == 0) {
//            csp_log_error("Invalid type_register size\r\n");
//            goto cleanup;
//        }
//
//        if (action == ADD_REGISTER) {
//            if (!generic_add_register(device_id, register_id, sub_register, type_register, &outValue, access)) {
//                csp_log_error("Failed to add register\n");
//                flags = FLAGS_BUSY;
//            }
//            flags = FLAGS_SUCCESS;
//        } else if (action == WRITE_REGISTER) {
//            if (!generic_set_register(device_id, register_id, sub_register, type_register, &outValue)) {
//                csp_log_error("Failed to set register\n");
//                flags = FLAGS_BUSY;
//            }
//            flags = FLAGS_SUCCESS;
//        }
//
//        csp_packet_t *AddWriteRequest_packet = csp_buffer_get(3);
//        if (!AddWriteRequest_packet) {
//            csp_log_error("AddWriteRequest_packet Failed to get buffer for response\n");
//            goto cleanup;
//        }
//
//        pack_AddWrite_respond(AddWriteRequest_packet->data, action, flags);
//        AddWriteRequest_packet->data[1] = register_id;
//        AddWriteRequest_packet->data[2]= sub_register;
//        AddWriteRequest_packet->length = 3;
//        if (!csp_send(conn, AddWriteRequest_packet, TIMEOUT_CSP_REG)) {
//            csp_log_error("Failed to send response\r\n");
//            csp_buffer_free(AddWriteRequest_packet);
//        }
//
//    } else if (action == READ_REGISTER) {
//        uint8_t device_id, type_register, access ,flags;
//        uint8_t register_id, sub_register;
//        RegisterValue outValue;
//        uint64_t packed_value;
//
//        unpack_ReadRequest(packet->data, &action, &device_id, &register_id, &sub_register, &type_register, &access);
//        flags = FLAGS_SUCCESS;
//        if (get_register_t(device_id, register_id, sub_register, &outValue, &type_register) != 0) {
//            csp_log_error("Failed to get register value\n");
//            flags = FLAGS_NOT_FOUND;
//            packed_value = 0;
//        }
//        size_t value_size = get_value_size(type_register);
//        if (convert_and_get_size(&outValue, type_register, &packed_value, &value_size) != 0) {
//            csp_log_error("Invalid type_register\r\n");
//            goto cleanup;
//        }
//
//        csp_packet_t *response_packet = csp_buffer_get(1 + value_size);
//        if (!response_packet) {
//            csp_log_error("response_packet Failed to get buffer for response\n");
//            goto cleanup;
//        }
//
//        pack_ReadResponse(response_packet->data, action, flags, packed_value, value_size);
//        response_packet ->data [1]= register_id;
//        response_packet ->data [2]= sub_register;
//        response_packet->length = 3 + value_size;
//
//        if (!csp_send(conn, response_packet, TIMEOUT_CSP_REG)) {
//            csp_log_error("Failed to send response\r\n");
//            csp_buffer_free(response_packet);
//        }
//
//    } else {
//        csp_log_error("Unsupported action received!\n");
//        goto cleanup;
//    }
//    cleanup: csp_buffer_free(packet);
//}


#endif /* INC_ADCS_COMMAND_H_ */
