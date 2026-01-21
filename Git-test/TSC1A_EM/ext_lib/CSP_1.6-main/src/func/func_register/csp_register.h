#ifndef CSP_REGISTER_ASSOCIATIVE_H_
#define CSP_REGISTER_ASSOCIATIVE_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stddef.h>



#ifdef __cplusplus
extern "C" {
#endif

#include <include/csp/csp.h>
#include <include/csp/csp_crc32.h>
#include <include/csp/arch/csp_semaphore.h>



typedef enum {
    INVALID_TYPE = 0,
    u8 = 1,
    I8 = 2,
    u16 = 3,
    I16 = 4,
    u32 = 5,
    I32 = 6,
    FLOAT_TYPE  = 7,
    DOUBLE_TYPE = 8
} RegisterType;


typedef enum {
    NONE_MODE = 0,
    READ_ONLY = 1,
    WRITE_ONLY = 2,
    READ_WRITE = 3
} AccessMode;

typedef struct map_node_t map_node_t;


struct map_node_t {
    size_t hash;
    void *key;
    void *value;
    RegisterType type;
    AccessMode access_mode;
    map_node_t *next;
};

typedef struct {
    map_node_t **buckets;
    size_t size;
} RegisterMap;

typedef struct {
    unsigned int device_id : 5;     // 5 บิต device_id
    unsigned int register_id : 8;   // 8 บิต register_id
    unsigned int sub_register : 8;  // 8 บิต sub_register
} RegisterKey;

extern csp_mutex_t register_mutex; 
extern RegisterMap *csp_register_map;

/**
 * The hash_function calculates a hash value based on the device_id, register_id, and sub_register
 * values.
 * 
 * @param device_id The `device_id` parameter represents the unique identifier of a device.
 * @param register_id The `register_id` parameter represents the ID of a register within a device.
 * @param sub_register The `sub_register` parameter represents a specific sub-register within a
 * register identified by `register_id` on a device with the ID `device_id`.
 * 
 * @return the hash value calculated using the formula ((device_id * 31 + register_id * 17 +
 * sub_register) % csp_register_map->size).
 */
size_t hash_function(unsigned int device_id, unsigned int register_id, unsigned int sub_register);


/**
 * The function `find_register_node` searches for a specific register node in a map based on device ID,
 * register ID, and sub-register.
 * 
 * @param device_id The `device_id` parameter represents the unique identifier of a device in the
 * system.
 * @param register_id The `register_id` parameter in the `find_register_node` function represents the
 * unique identifier for a specific register within a device. It is used to search for a specific node
 * in the register map based on the combination of `device_id`, `register_id`, and `sub_register`.
 * @param sub_register The `sub_register` parameter in the `find_register_node` function represents a
 * specific sub-register within a register identified by `device_id` and `register_id`. This function
 * is designed to search for a node in a register map based on these parameters.
 * 
 * @return The function `find_register_node` returns a pointer to a `map_node_t` structure, which
 * represents a node in a hash map. If a node with the specified `device_id`, `register_id`, and
 * `sub_register` is found in the hash map, that node is returned. Otherwise, `NULL` is returned.
 */
map_node_t* find_register_node(unsigned int device_id, unsigned int register_id, unsigned int sub_register);


/**
 * The function `initializeRegister` initializes a RegisterMap with a specified size and returns an
 * error code if unsuccessful.
 * 
 * @param size The `size` parameter in the `initializeRegister` function represents the number of
 * buckets to be allocated for the register map. This size determines the initial capacity of the
 * register map data structure.
 * 
 * @return The `initializeRegister` function returns an `int8_t` value. If the initialization is
 * successful, it returns 0. If the size is 0, it returns -1 indicating an invalid size. If memory
 * allocation for `RegisterMap` fails, it returns -2.
 */
int8_t initializeRegister(size_t size);


/**
 * The function `add_register` adds a new register entry to a register map with specified device ID,
 * register ID, sub-register, type, value, and access mode.
 * 
 * @param device_id The `device_id` parameter in the `add_register` function represents the unique
 * identifier of the device for which the register is being added. It helps in organizing and accessing
 * registers specific to different devices in the system.
 * @param register_id The `register_id` parameter in the `add_register` function represents the ID of
 * the register being added to the register map. It is an unsigned integer value used to uniquely
 * identify the register within the system.
 * @param sub_register The `sub_register` parameter in the `add_register` function represents a
 * sub-register identifier within a specific register of a device. It is used to uniquely identify a
 * specific sub-register within a register.
 * @param type The `type` parameter in the `add_register` function represents the type of the register
 * being added. It is used to determine how to handle the `value` parameter that is passed to the
 * function. The function checks if the `type` is within a valid range and then calls the appropriate
 * function
 * @param value The `value` parameter in the `add_register` function is a pointer to the data that you
 * want to associate with the register being added. The function will use this data based on the
 * specified `type` to store it in the register node. The `type` parameter determines how the `value
 * @param access_mode The `access_mode` parameter in the `add_register` function represents the access
 * mode for the register being added. It is of type `uint8_t`, which typically means an unsigned 8-bit
 * integer. The `access_mode` parameter is used to specify the access permissions or mode for the
 * register
 * 
 * @return The function `add_register` returns a boolean value (`true` or `false`) indicating whether
 * the operation of adding a register was successful or not.
 */
bool add_register(unsigned int device_id, unsigned int register_id, unsigned int sub_register, RegisterType type, void *value, AccessMode access_mode);

/**
 * The function `set_register` is responsible for setting a value in a specified register, considering
 * its access mode and type.
 * 
 * @param device_id The `device_id` parameter represents the unique identifier of the device for which
 * you want to set a register value.
 * @param register_id The `register_id` parameter in the `set_register` function is an unsigned integer
 * that represents the identifier of a specific register within a device. It is used to locate the
 * register node in the data structure and perform operations on it, such as setting a new value.
 * @param sub_register The `sub_register` parameter in the `set_register` function refers to a specific
 * sub-register within a register identified by `device_id` and `register_id`. It is used to locate a
 * particular sub-register within a register for setting its value.
 * @param value The `value` parameter in the `set_register` function is a pointer to the value that you
 * want to set for a specific register identified by `device_id`, `register_id`, and `sub_register`.
 * This value can be of any data type depending on the register type and the specific implementation of
 * 
 * @return The function `set_register` returns a boolean value - `true` if the register was
 * successfully set, and `false` otherwise.
 */
int8_t  set_register(unsigned int device_id, unsigned int register_id, unsigned int sub_register, void *value);


int8_t set_register_myself(unsigned int device_id, unsigned int register_id, unsigned int sub_register, void *value);

/**
 * The function `get_register` retrieves the value of a specified register from a device, handling
 * different access modes and register types.
 * 
 * @param device_id The `device_id` parameter is an unsigned integer that represents the ID of the
 * device from which you want to retrieve a register.
 * @param register_id The `register_id` parameter in the `get_register` function is used to specify the
 * identifier of the register you want to access within a specific device. It helps in identifying the
 * particular register for which you want to retrieve the value.
 * @param sub_register The `sub_register` parameter in the `get_register` function is used to specify a
 * sub-register within a register. It helps in identifying a specific part or field within a register
 * that you want to access or modify. This parameter allows for more granular control and manipulation
 * of data within a register.
 * @param outValue The `outValue` parameter in the `get_register` function is a pointer to a memory
 * location where the value read from the specified register will be stored. The function will populate
 * this memory location with the value read from the register.
 * 
 * @return The function `get_register` does not return any value. It is a void function, so it does not
 * have a return statement that would provide a specific value.
 */
int8_t get_register(unsigned int device_id, unsigned int register_id, unsigned int sub_register, void *outValue);


/**
 * The function `get_register_t` reads a value from a specified register based on the device ID,
 * register ID, and sub-register, using a function pointer table to handle different data types.
 * 
 * @param device_id The `device_id` parameter represents the identifier of the device from which you
 * want to retrieve a register value.
 * @param register_id The `register_id` parameter in the `get_register_t` function is used to identify
 * a specific register within a device. It is an unsigned integer that helps locate the register in the
 * system.
 * @param sub_register The `sub_register` parameter in the `get_register_t` function represents a
 * specific sub-register within a register identified by `device_id` and `register_id`. It is used to
 * access a particular field or subset of data within the register.
 * @param outValue The `outValue` parameter is a pointer to a memory location where the value read from
 * the register will be stored. The function `get_register_t` will populate this memory location with
 * the value read from the specified register.
 * @param type_register The `type_register` parameter in the `get_register_t` function is a pointer to
 * a `uint8_t` variable. This parameter is used to store the type of the register being accessed, which
 * is retrieved from the `map_node_t` structure. The type information is then used to determine
 * 
 * @return The function `get_register_t` returns an `int8_t` value. If the function execution is
 * successful, it returns 0. If there are any errors encountered during the execution, it returns -1.
 */
int8_t get_register_t(unsigned int device_id, unsigned int register_id, unsigned int sub_register, void *outValue, uint8_t *type_register);


/**
 * The function `count_used_registers` counts the number of nodes in a register map while ensuring
 * thread safety by locking and unlocking a mutex.
 * 
 * @return The function `count_used_registers` returns the number of used registers by counting the
 * nodes in the register map data structure. If successful, it returns the count of used registers as a
 * `size_t` value. If there are any issues with locking the mutex or if the register map is NULL, it
 * returns 0.
 */
size_t count_used_registers();


/**
 * The function `count_available_slots` counts the number of available slots in a register map after
 * locking a mutex.
 * 
 * @return The function `count_available_slots` returns the number of available slots in the
 * `csp_register_map` data structure after checking for mutex locking and the existence of the map
 * itself.
 */
size_t count_available_slots();

#ifdef __cplusplus
}
#endif

#endif /* CSP_REGISTER_ASSOCIATIVE_H_ */
