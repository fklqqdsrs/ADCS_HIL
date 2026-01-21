#include "cli_csp.h"

// Function to convert string to RegisterType
RegisterType get_register_type(const char *type_str)
{
    if (strcmp(type_str, "U8") == 0)
        return U8;
    if (strcmp(type_str, "I8") == 0)
        return I8;
    if (strcmp(type_str, "U16") == 0)
        return U16;
    if (strcmp(type_str, "I16") == 0)
        return I16;
    if (strcmp(type_str, "U32") == 0)
        return U32;
    if (strcmp(type_str, "I32") == 0)
        return I32;
    if (strcmp(type_str, "FLOAT") == 0)
        return FLOAT_TYPE;
    if (strcmp(type_str, "DOUBLE") == 0)
        return DOUBLE_TYPE;
    return INVALID_TYPE;
}

// Function to convert string to AccessMode
AccessMode get_access_mode(const char *access_mode_str)
{
    if (strcmp(access_mode_str, "READ_ONLY") == 0)
        return READ_ONLY;
    if (strcmp(access_mode_str, "WRITE_ONLY") == 0)
        return WRITE_ONLY;
    if (strcmp(access_mode_str, "READ_WRITE") == 0)
        return READ_WRITE;
    return 0;
}

// Function to parse the value based on the type_register
void *parse_value(RegisterType type_register, const char *value_str)
{
    static uint8_t u8_value;
    static int8_t i8_value;
    static uint16_t u16_value;
    static int16_t i16_value;
    static uint32_t u32_value;
    static int32_t i32_value;
    static float float_value;
    static double double_value;

    switch (type_register)
    {
    case U8:
        u8_value = (uint8_t)strtoul(value_str, NULL, 0);
        return &u8_value;
    case I8:
        i8_value = (int8_t)strtol(value_str, NULL, 0);
        return &i8_value;
    case U16:
        u16_value = (uint16_t)strtoul(value_str, NULL, 0);
        return &u16_value;
    case I16:
        i16_value = (int16_t)strtol(value_str, NULL, 0);
        return &i16_value;
    case U32:
        u32_value = (uint32_t)strtoul(value_str, NULL, 0);
        return &u32_value;
    case I32:
        i32_value = (int32_t)strtol(value_str, NULL, 0);
        return &i32_value;
    case FLOAT_TYPE:
        float_value = strtof(value_str, NULL);
        return &float_value;
    case DOUBLE_TYPE:
        double_value = strtod(value_str, NULL);
        return &double_value;
    default:
        return NULL;
    }
}

void *allocate_memory_for_value(uint8_t type_register)
{
    switch (type_register)
    {
    case U8:
        return malloc(sizeof(uint8_t));
    case I8:
        return malloc(sizeof(int8_t));
    case U16:
        return malloc(sizeof(uint16_t));
    case I16:
        return malloc(sizeof(int16_t));
    case U32:
        return malloc(sizeof(uint32_t));
    case I32:
        return malloc(sizeof(int32_t));
    case FLOAT_TYPE:
        return malloc(sizeof(float));
    case DOUBLE_TYPE:
        return malloc(sizeof(double));
    default:
        return NULL;
    }
}

void print_value(uint8_t type_register, void *value_ptr)
{
    switch (type_register)
    {
    case U8:
        printf("Value: %u\n", *(uint8_t *)value_ptr);
        break;
    case I8:
        printf("Value: %d\n", *(int8_t *)value_ptr);
        break;
    case U16:
        printf("Value: %u\n", *(uint16_t *)value_ptr);
        break;
    case I16:
        printf("Value: %d\n", *(int16_t *)value_ptr);
        break;
    case U32:
        printf("Value: %lu\n", *(uint32_t *)value_ptr);
        break;
    case I32:
        printf("Value: %ld\n", *(int32_t *)value_ptr);
        break;
    case FLOAT_TYPE:
        printf("Value: %f\n", *(float *)value_ptr);
        break;
    case DOUBLE_TYPE:
        printf("Value: %lf\n", *(double *)value_ptr);
        break;
    default:
        printf("Unknown type_register\n");
        break;
    }
}

void csp_add_register_command(const char *args) {
    unsigned int device_id, register_id, sub_register;
    uint8_t type_register = INVALID_TYPE, access_mode = 0;
    uint32_t timeout;
    void *value_ptr = NULL;

    char type_str[16], access_mode_str[16], value_str[16];

    char *args_copy = strdup(args);
    char *token = strtok(args_copy, " ");

    if (token != NULL) device_id = (unsigned int)atoi(token);
    token = strtok(NULL, " ");
    if (token != NULL) register_id = (unsigned int)atoi(token);
    token = strtok(NULL, " ");
    if (token != NULL) sub_register = (unsigned int)atoi(token);
    token = strtok(NULL, " ");
    if (token != NULL) strncpy(type_str, token, sizeof(type_str) - 1);
    token = strtok(NULL, " ");
    if (token != NULL) strncpy(value_str, token, sizeof(value_str) - 1);
    token = strtok(NULL, " ");
    if (token != NULL) strncpy(access_mode_str, token, sizeof(access_mode_str) - 1);
    token = strtok(NULL, " ");
    if (token != NULL) timeout = (uint32_t)strtoul(token, NULL, 10);

    if (token != NULL) {
        type_register = get_register_type(type_str);
        if (type_register == INVALID_TYPE) {
            printf("Invalid type_register: %s\n", type_str);
            free(args_copy);
            return;
        }

        access_mode = get_access_mode(access_mode_str);
        if (access_mode == 0) {
            printf("Invalid access_mode: %s\n", access_mode_str);
            free(args_copy);
            return;
        }

        value_ptr = parse_value(type_register, value_str);
        if (value_ptr == NULL) {
            printf("Failed to parse value: %s\n", value_str);
            free(args_copy);
            return;
        }

        int result = add_register_csp(device_id, register_id, sub_register, type_register, value_ptr, access_mode, timeout);

        if (result >= 0) {
            printf("Register successfully added.\n");
            printf("Device ID: %u\n", device_id);
            printf("Register ID: %u\n", register_id);
            printf("Sub Register: %u\n", sub_register);
            printf("Type Register: %s\n", type_str);
            printf("Value: %s\n", value_str);
            printf("Access Mode: %s\n", access_mode_str);
            printf("Timeout: %lu ms\n", timeout);
        } else {
            printf("Failed to add reg. Error code: %d\n", result);
        }
    } else {
        printf("Usage: add_reg <device_id> <register_id> <sub_register> <type_register> <value> <access_mode> <timeout>\n");
    }

    free(args_copy);
}

void csp_set_register_command(const char *args) {
    unsigned int device_id, register_id, sub_register;
    uint8_t type_register = INVALID_TYPE;
    uint32_t timeout;
    void *value_ptr = NULL;

    char type_str[16], value_str[16];

    char *args_copy = strdup(args);
    char *token = strtok(args_copy, " ");

    if (token != NULL) device_id = (unsigned int)atoi(token);
    token = strtok(NULL, " ");
    if (token != NULL) register_id = (unsigned int)atoi(token);
    token = strtok(NULL, " ");
    if (token != NULL) sub_register = (unsigned int)atoi(token);
    token = strtok(NULL, " ");
    if (token != NULL) strncpy(type_str, token, sizeof(type_str) - 1);
    token = strtok(NULL, " ");
    if (token != NULL) strncpy(value_str, token, sizeof(value_str) - 1);
    token = strtok(NULL, " ");
    if (token != NULL) timeout = (uint32_t)strtoul(token, NULL, 10);

    if (token != NULL) {
        type_register = get_register_type(type_str);
        if (type_register == INVALID_TYPE) {
            printf("Invalid type_register: %s\n", type_str);
            free(args_copy);
            return;
        }

        value_ptr = parse_value(type_register, value_str);
        if (value_ptr == NULL) {
            printf("Failed to parse value: %s\n", value_str);
            free(args_copy);
            return;
        }

        int result = set_register_csp(device_id, register_id, sub_register, type_register, value_ptr, timeout);

        if (result  >=  0) {
            printf("Register successfully set.\n");
            printf("Device ID: %u\n", device_id);
            printf("Register ID: %u\n", register_id);
            printf("Sub Register: %u\n", sub_register);
            printf("Type Register: %s\n", type_str);
            printf("Value: %s\n", value_str);
            printf("Timeout: %lu ms\n", timeout);
        } else {
            printf("Failed to set reg. Error code: %d\n", result);
        }
    } else {
        printf("Usage: set_reg <device_id> <register_id> <sub_register> <type_register> <value> <timeout>\n");
    }
    free(args_copy);
}

void csp_get_register_command(const char *args) {
    unsigned int device_id, register_id, sub_register;
    uint8_t type_register = INVALID_TYPE;
    uint32_t timeout;
    void *out_value_ptr = NULL;

    char type_str[16];

    char *args_copy = strdup(args);
    char *token = strtok(args_copy, " ");

    if (token != NULL) device_id = (unsigned int)atoi(token);
    token = strtok(NULL, " ");
    if (token != NULL) register_id = (unsigned int)atoi(token);
    token = strtok(NULL, " ");
    if (token != NULL) sub_register = (unsigned int)atoi(token);
    token = strtok(NULL, " ");
    if (token != NULL) strncpy(type_str, token, sizeof(type_str) - 1);
    token = strtok(NULL, " ");
    if (token != NULL) timeout = (uint32_t)strtoul(token, NULL, 10);

    if (token != NULL) {
        type_register = get_register_type(type_str);
        if (type_register == INVALID_TYPE) {
            printf("Invalid type_register: %s\n", type_str);
            free(args_copy);
            return;
        }

        out_value_ptr = allocate_memory_for_value(type_register);
        if (out_value_ptr == NULL) {
            printf("Failed to allocate memory for value.\n");
            free(args_copy);
            return;
        }

        int result = get_register_csp(device_id, register_id, sub_register, type_register, out_value_ptr, timeout);

        if (result >= 0) {
            printf("Register value successfully retrieved.\n");
            printf("Device ID: %u\n", device_id);
            printf("Register ID: %u\n", register_id);
            printf("Sub Register: %u\n", sub_register);
            printf("Type Register: %s\n", type_str);
            printf("Timeout: %lu ms\n", timeout);
            print_value(type_register, out_value_ptr);
        } else {
            printf("Failed to get reg %d\n", result);
        }
    } else {
        printf("Usage: get_reg <device_id> <register_id> <sub_register> <type_register> <timeout>\n");
    }
    free(args_copy);
}
