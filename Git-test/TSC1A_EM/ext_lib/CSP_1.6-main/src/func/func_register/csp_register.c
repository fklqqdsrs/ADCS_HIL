#include <src/func/func_register/csp_register.h>

csp_mutex_t register_mutex;
RegisterMap *csp_register_map = NULL;

size_t hash_function(unsigned int device_id, unsigned int register_id, unsigned int sub_register)
{
    return ((device_id * 31 + register_id * 17 + sub_register) % csp_register_map->size);
}

map_node_t *find_register_node(unsigned int device_id, unsigned int register_id, unsigned int sub_register)
{
    if (csp_register_map == NULL)
    {
        printf("RegisterMap is not initialized\\n");
        return NULL;
    }

    size_t hash = hash_function(device_id, register_id, sub_register);
    map_node_t *node = csp_register_map->buckets[hash];
    while (node)
    {
        RegisterKey *node_key = (RegisterKey *)node->key;
        if (node_key->device_id == device_id && node_key->register_id == register_id && node_key->sub_register == sub_register)
        {
            return node;
        }
        node = node->next;
    }
    return NULL;
}


int8_t initializeRegister(size_t size)
{
    if (size == 0)
    {
        printf("Invalid size for RegisterMap initialization\\n");
        return -1;
    }

    csp_register_map = (RegisterMap *)malloc(sizeof(RegisterMap));
    if (csp_register_map == NULL)
    {
        printf("Failed to create RegisterMap\\n");
        return -2;
    }

    csp_register_map->buckets = (map_node_t **)calloc(size, sizeof(map_node_t *));
    csp_register_map->size = size;

    if (csp_mutex_create(&register_mutex) == CSP_MUTEX_OK)
    {
       // printf("Mutex created successfully.\n");
    }
    else
    {
        printf("Failed to create mutex.\n");
        return -1;
    }

    return 0;
}

// Function definitions for adding values based on type
void add_uint8(void *value, void **node_value)
{
    *node_value = malloc(sizeof(uint8_t));
    **(uint8_t **)node_value = *(uint8_t *)value;
}

void add_int8(void *value, void **node_value)
{
    *node_value = malloc(sizeof(int8_t));
    **(int8_t **)node_value = *(int8_t *)value;
}

void add_uint16(void *value, void **node_value)
{
    *node_value = malloc(sizeof(uint16_t));
    **(uint16_t **)node_value = *(uint16_t *)value;
}

void add_int16(void *value, void **node_value)
{
    *node_value = malloc(sizeof(int16_t));
    **(int16_t **)node_value = *(int16_t *)value;
}

void add_uint32(void *value, void **node_value)
{
    *node_value = malloc(sizeof(uint32_t));
    **(uint32_t **)node_value = *(uint32_t *)value;
}

void add_int32(void *value, void **node_value)
{
    *node_value = malloc(sizeof(int32_t));
    **(int32_t **)node_value = *(int32_t *)value;
}

void add_float(void *value, void **node_value)
{
    *node_value = malloc(sizeof(float));
    **(float **)node_value = *(float *)value;
}

void add_double(void *value, void **node_value)
{
    *node_value = malloc(sizeof(double));
    **(double **)node_value = *(double *)value;
}

// Function pointer table for adding values
typedef void (*add_func_t)(void *, void **);
add_func_t add_functions[] = {
    NULL,       // INVALID_TYPE
    add_uint8,  // U8
    add_int8,   // I8
    add_uint16, // U16
    add_int16,  // I16
    add_uint32, // U32
    add_int32,  // I32
    add_float,  // FLOAT
    add_double  // DOUBLE
};

// Function definitions for setting values based on type
void set_uint8(void *value, void *node_value)
{
    *(uint8_t *)node_value = *(uint8_t *)value;
}

void set_int8(void *value, void *node_value)
{
    *(int8_t *)node_value = *(int8_t *)value;
}

void set_uint16(void *value, void *node_value)
{
    *(uint16_t *)node_value = *(uint16_t *)value;
}

void set_int16(void *value, void *node_value)
{
    *(int16_t *)node_value = *(int16_t *)value;
}

void set_uint32(void *value, void *node_value)
{
    *(uint32_t *)node_value = *(uint32_t *)value;
}

void set_int32(void *value, void *node_value)
{
    *(int32_t *)node_value = *(int32_t *)value;
}

void set_float(void *value, void *node_value)
{
    *(float *)node_value = *(float *)value;
}

void set_double(void *value, void *node_value)
{
    *(double *)node_value = *(double *)value;
}

// Function pointer table for setting values
typedef void (*set_func_t)(void *, void *);
set_func_t set_functions[] = {
    NULL,       // INVALID_TYPE
    set_uint8,  // U8
    set_int8,   // I8
    set_uint16, // U16
    set_int16,  // I16
    set_uint32, // U32
    set_int32,  // I32
    set_float,  // FLOAT
    set_double  // DOUBLE
};

// Function definitions for getting values based on type
void get_uint8(void *node_value, void *outValue)
{
    *(uint8_t *)outValue = *(uint8_t *)node_value; 
}

void get_int8(void *node_value, void *outValue)
{
    *(int8_t *)outValue = *(int8_t *)node_value;
}

void get_uint16(void *node_value, void *outValue)
{
    *(uint16_t *)outValue = *(uint16_t *)node_value;
}

void get_int16(void *node_value, void *outValue)
{
    *(int16_t *)outValue = *(int16_t *)node_value;
}

void get_uint32(void *node_value, void *outValue)
{
    *(uint32_t *)outValue = *(uint32_t *)node_value;
}

void get_int32(void *node_value, void *outValue)
{
    *(int32_t *)outValue = *(int32_t *)node_value;
}

void get_float(void *node_value, void *outValue)
{
    *(float *)outValue = *(float *)node_value;
}

void get_double(void *node_value, void *outValue)
{
    *(double *)outValue = *(double *)node_value;
}

// Function pointer table for getting values
typedef void (*get_func_t)(void *, void *);
get_func_t get_functions[] = {
    NULL,       // INVALID_TYPE
    get_uint8,  // U8
    get_int8,   // I8
    get_uint16, // U16
    get_int16,  // I16
    get_uint32, // U32
    get_int32,  // I32
    get_float,  // FLOAT
    get_double  // DOUBLE
};
bool add_register(unsigned int device_id, unsigned int register_id, unsigned int sub_register, RegisterType type, void *value, AccessMode access_mode)
{
    if (csp_register_map == NULL)
    {
        printf("RegisterMap is not initialized\n");
        return false;
    }
    csp_mutex_lock(&register_mutex, CSP_MAX_TIMEOUT);
    RegisterKey *key = (RegisterKey *)malloc(sizeof(RegisterKey));
    if (key == NULL)

    {
        csp_mutex_unlock(&register_mutex);
        return false;
    }

    key->device_id = device_id;
    key->register_id = register_id & 0xFF;
    key->sub_register = sub_register & 0xFF;

    size_t hash = hash_function(device_id, register_id, sub_register);
    map_node_t *new_node = (map_node_t *)malloc(sizeof(map_node_t));
    if (new_node == NULL)
    {
        free(key);
        csp_mutex_unlock(&register_mutex);
        return false;
    }
    new_node->hash = hash;
    new_node->key = key;

    if (type > 0 && type < sizeof(add_functions) / sizeof(add_func_t) && add_functions[type])
    {
        add_functions[type](value, &new_node->value);
    }
    else
    {
        new_node->value = NULL;
        printf("Unknown type for adding register\n");
        free(new_node);
        free(key);
        csp_mutex_unlock(&register_mutex);
        return false;
    }

    new_node->type = type;
    new_node->access_mode = access_mode;
    new_node->next = csp_register_map->buckets[hash];
    csp_register_map->buckets[hash] = new_node;

    csp_mutex_unlock(&register_mutex);
    return true;
}

int8_t set_register(unsigned int device_id, unsigned int register_id, unsigned int sub_register, void *value)
{
    csp_mutex_lock(&register_mutex, CSP_MAX_TIMEOUT);
    map_node_t *node = find_register_node(device_id, register_id, sub_register);
    if (node)
    {
        if ( node->access_mode == READ_ONLY)
        {
            printf("Cannot set value. Register is read-only.\n");
            csp_mutex_unlock(&register_mutex);
            return -1;
        }
        else if (node->type > 0 && node->type < sizeof(set_functions) / sizeof(set_func_t) && set_functions[node->type])
        {
            set_functions[node->type](value, node->value);
        }
        else
        {
            printf("Unknown register type.\n");
            csp_mutex_unlock(&register_mutex);
            return -2;
        }
    }
    else
    {
        printf("Register not found. Please add it first.\n");
        csp_mutex_unlock(&register_mutex);
        return -3;
    }

    csp_mutex_unlock(&register_mutex);
    return CSP_ERR_NONE;
}


int8_t set_register_myself(unsigned int device_id, unsigned int register_id, unsigned int sub_register, void *value)
{
    csp_mutex_lock(&register_mutex, CSP_MAX_TIMEOUT);
    map_node_t *node = find_register_node(device_id, register_id, sub_register);
    if (node)
    {
        if (device_id != csp_get_address()  && node->access_mode == READ_ONLY)
        {
            printf("Cannot set value. Register is read-only.\n");
            csp_mutex_unlock(&register_mutex);
            return -1;
        }
        else if (node->type > 0 && node->type < sizeof(set_functions) / sizeof(set_func_t) && set_functions[node->type])
        {
            set_functions[node->type](value, node->value);
        }
        else
        {
            printf("Unknown register type.\n");
            csp_mutex_unlock(&register_mutex);
            return -2;
        }
    }
    else
    {
        printf("Register not found. Please add it first.\n");
        csp_mutex_unlock(&register_mutex);
        return -3;
    }

    csp_mutex_unlock(&register_mutex);
    return CSP_ERR_NONE;
}


int8_t get_register(unsigned int device_id, unsigned int register_id, unsigned int sub_register, void *outValue)
{
    csp_mutex_lock(&register_mutex, CSP_MAX_TIMEOUT);
    map_node_t *node = find_register_node(device_id, register_id, sub_register);
    if (node)
    {
        if (device_id != csp_get_address() && node->access_mode == WRITE_ONLY)
        {
            printf("Cannot read value. Register is write-only.\\n");
            return -1;
        }
        else if (node->type > 0 && node->type < sizeof(get_functions) / sizeof(get_func_t) && get_functions[node->type])
        {
            get_functions[node->type](node->value, outValue);

        }
        else
        {
            printf("Unknown register type.\\n");
            return -2;
        }
    }
    else
    {
        printf("Register not found.\\n");
        return -3;
    }
    csp_mutex_unlock(&register_mutex);
    return CSP_ERR_NONE;
}

int8_t get_register_t(unsigned int device_id, unsigned int register_id, unsigned int sub_register, void *outValue, uint8_t *type_register)
{
    csp_mutex_lock(&register_mutex, CSP_MAX_TIMEOUT);
    map_node_t *node = find_register_node(device_id, register_id, sub_register);
    if (node)
    {
        if (node->access_mode == WRITE_ONLY)
        {
            printf("Cannot read value. Register is write-only.\\n");
            csp_mutex_unlock(&register_mutex);
            return -1;
        }
        *type_register = node->type;

        // Use function pointer table to get the value based on the type
        if (node->type > 0 && node->type < sizeof(get_functions) / sizeof(get_func_t) && get_functions[node->type])
        {
            get_functions[node->type](node->value, outValue);
        }
        else
        {
            printf("Unknown register type.\\n");
            csp_mutex_unlock(&register_mutex);
            return -2;
        }
    }
    else
    {
        printf("Register not found.\\n");
        csp_mutex_unlock(&register_mutex);
        return -3;
    }

    csp_mutex_unlock(&register_mutex);
    return CSP_ERR_NONE;
}

size_t count_used_registers()
{
    csp_mutex_lock(&register_mutex, CSP_MAX_TIMEOUT);

    if (csp_register_map == NULL)
    {
        csp_mutex_unlock(&register_mutex);
        return 0;
    }

    size_t count = 0;
    // Traverse all buckets to count the nodes
    for (size_t i = 0; i < csp_register_map->size; ++i)
    {
        map_node_t *node = csp_register_map->buckets[i];
        while (node)
        {
            count++;
            node = node->next;
        }
    }

    csp_mutex_unlock(&register_mutex);
    return count;
}

size_t count_available_slots()
{
    csp_mutex_lock(&register_mutex, CSP_MAX_TIMEOUT);

    if (csp_register_map == NULL)
    {
        csp_mutex_unlock(&register_mutex);
        return 0;
    }

    size_t available_slots = 0;
    for (size_t i = 0; i < csp_register_map->size; ++i)
    {
        if (csp_register_map->buckets[i] == NULL)
        {
            available_slots++;
        }
    }

    csp_mutex_unlock(&register_mutex);
    return available_slots;
}
