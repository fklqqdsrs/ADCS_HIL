# CSP Register ID Add and Write Action
## Request Format
| Field           | Size        | Description                          |
|-----------------|-------------|--------------------------------------|
| **Action**      | 4 bits      | Action identifier                    |
| **Device ID**   | 5 bits      | Identifier of the device             |
| **Register ID** | 8 bits      | Identifier of the register           |
| **Sub Register**| 8 bits      | Identifier of the sub-register       |
| **Type Register**| 4 bits     | Type of register (data type)         |
| **Access**      | 3 bits      | Access mode (e.g., read, write)      |
| **Value**       | 8 to 64 bits| Value to be written to the register  |
- **Total Size (excluding value):** 4 bytes.

## Response Format
| Field      | Size   | Description           |
|------------|--------|-----------------------|
| **Action** | 4 bits | Action identifier     |
| **Flags**  | 4 bits | Status or flag bits   |
- **Total Size:** 1 byte.



# CSP Register ID Read Action
## Request Format
| Field           | Size    | Description                          |
|-----------------|---------|--------------------------------------|
| **Action**      | 4 bits  | Action identifier                    |
| **Device ID**   | 5 bits  | Identifier of the device             |
| **Register ID** | 8 bits  | Identifier of the register           |
| **Sub Register**| 8 bits  | Identifier of the sub-register       |
| **Type Register**| 4 bits | Type of register (data type)         |
| **Access**      | 3 bits  | Access mode                          |
- **Total Size:** 4 bytes.

## Response Format
| Field      | Size        | Description                          |
|------------|-------------|--------------------------------------|
| **Action** | 4 bits      | Action identifier                    |
| **Flags**  | 4 bits      | Status or flag bits                  |
| **Value**  | 8 to 64 bits| Value read from the register         |
- **Total Size (excluding value):** 1 byte.



# Function Documentation

This document provides an overview of the functions defined in the provided header files: `bit_management.h`, `csp_register.h`, and `csp_register_service.h`. Each function is described with its purpose, parameters, process, and usage.



# Function Documentation for `csp_register_service.h`

This document provides a detailed overview of the key functions defined in `csp_register_service.h`, including descriptions, parameters, processes, and usage examples.

## 1. `bool generic_add_register(uint8_t device_id, uint8_t register_id, uint8_t sub_register, uint8_t type_register, RegisterValue *value, uint8_t access);`
- **Description**: Adds a register with the specified parameters to the system.
- **Parameters**:
  - `uint8_t device_id`: The ID of the device.
  - `uint8_t register_id`: The ID of the register.
  - `uint8_t sub_register`: The ID of the sub-register.
  - `uint8_t type_register`: The type of the register.
  - `RegisterValue *value`: Pointer to the initial value of the register.
  - `uint8_t access`: Access mode for the register.
- **Process**:
  - Validates inputs, allocates memory if needed, and registers the new entry.
- **Usage**:
  ```c
  RegisterValue value = { .u8 = 10 }; // Example value structure
  bool success = generic_add_register(1, 1, 1, U8, &value, READ_WRITE);
  ```

## 2. `bool generic_set_register(uint8_t device_id, uint8_t register_id, uint8_t sub_register, uint8_t type_register, RegisterValue *value);`
- **Description**: Sets the value of an existing register.
- **Parameters**:
  - `uint8_t device_id`: The ID of the device.
  - `uint8_t register_id`: The ID of the register.
  - `uint8_t sub_register`: The ID of the sub-register.
  - `uint8_t type_register`: The type of the register.
  - `RegisterValue *value`: Pointer to the new value.
- **Process**:
  - Finds the register and updates its value accordingly.
- **Usage**:
  ```c
  RegisterValue value = { .u8 = 20 }; // Example value
  bool updated = generic_set_register(1, 1, 1, U8, &value);
  ```

## 3. `void service_register_demo(csp_conn_t *conn, csp_packet_t *packet);`
- **Description**: A demo function for handling register service requests via CSP (CubeSat Space Protocol).
- **Parameters**:
  - `csp_conn_t *conn`: Pointer to the CSP connection.
  - `csp_packet_t *packet`: Pointer to the CSP packet containing the request data.
- **Process**:
  - Handles incoming register commands and demonstrates the register service capabilities.
- **Usage**:
  ```c
  // Example usage would be within a CSP task or service handler context
  service_register_demo(conn, packet);
  ```

## 4. `int convert_and_get_size(void *value, uint8_t type_register, uint64_t *packed_value, size_t *value_size);`
- **Description**: Converts a value to a packed format and retrieves its size.
- **Parameters**:
  - `void *value`: Pointer to the input value.
  - `uint8_t type_register`: The type of the register.
  - `uint64_t *packed_value`: Pointer to store the packed value.
  - `size_t *value_size`: Pointer to store the size of the value.
- **Process**:
  - Converts the value based on its type and calculates the size needed for transmission or storage.
- **Usage**:
  ```c
  uint8_t value = 5;
  uint64_t packed;
  size_t size;
  int status = convert_and_get_size(&value, U8, &packed, &size);
  ```

## 5. `int add_register_csp(unsigned int device_id, unsigned int register_id, unsigned int sub_register, uint8_t type_register, void *value, uint8_t access_mode, uint16_t timeout);`
- **Description**: Adds a register via a CSP command with a specified timeout.
- **Parameters**:
  - `unsigned int device_id`: The ID of the device.
  - `unsigned int register_id`: The ID of the register.
  - `unsigned int sub_register`: The ID of the sub-register.
  - `uint8_t type_register`: The type of the register.
  - `void *value`: Pointer to the initial value of the register.
  - `uint8_t access_mode`: Access mode for the register.
  - `uint16_t timeout`: Timeout value for the operation.
- **Process**:
  - Sends a CSP packet to add the register, waits for confirmation within the timeout period.
- **Usage**:
  ```c
  uint8_t initial_value = 10;
  int status = add_register_csp(1, 2, 3, U8, &initial_value, READ_WRITE, 1000);
  ```

## 6. `int set_register_csp(unsigned int device_id, unsigned int register_id, unsigned int sub_register, uint8_t type_register, void *value, uint16_t timeout);`
- **Description**: Sets a register's value via a CSP command with a specified timeout.
- **Parameters**:
  - `unsigned int device_id`: The ID of the device.
  - `unsigned int register_id`: The ID of the register.
  - `unsigned int sub_register`: The ID of the sub-register.
  - `uint8_t type_register`: The type of the register.
  - `void *value`: Pointer to the new value.
  - `uint16_t timeout`: Timeout value for the operation.
- **Process**:
  - Sends a CSP packet to set the register value, handling confirmation within the timeout.
- **Usage**:
  ```c
  uint8_t new_value = 20;
  int status = set_register_csp(1, 2, 3, U8, &new_value, 1000);
  ```

## 7. `int get_register_csp(unsigned int device_id, unsigned int register_id, unsigned int sub_register, uint8_t type_register, void *outValue, uint16_t timeout);`
- **Description**: Retrieves the value of a register via a CSP command with a specified timeout.
- **Parameters**:
  - `unsigned int device_id`: The ID of the device.
  - `unsigned int register_id`: The ID of the register.
  - `unsigned int sub_register`: The ID of the sub-register.
  - `uint8_t type_register`: The type of the register.
  - `void *outValue`: Pointer to store the retrieved value.
  - `uint16_t timeout`: Timeout value for the operation.
- **Process**:
  - Sends a CSP request to get the register value, handling data retrieval within the timeout.
- **Usage**:
  ```c
  uint8_t value;
  int status = get_register_csp(1, 2, 3, U8, &value, 1000);
  printf("Register value: %d", value);
  ```

This documentation provides an overview of the functions found in `csp_register_service.h`. Refer to the header file for additional details and context.



# Function Documentation for `csp_register.h`

This document provides a detailed overview of the key functions defined in `csp_register.h`, including descriptions, parameters, processes, and usage examples.

## 1. `size_t hash_function(unsigned int device_id, unsigned int register_id, unsigned int sub_register);`
- **Description**: Computes a hash value based on the device ID, register ID, and sub-register. This hash is typically used to locate or manage registers within a data structure.
- **Parameters**:
  - `unsigned int device_id`: The ID of the device.
  - `unsigned int register_id`: The ID of the register.
  - `unsigned int sub_register`: The ID of the sub-register.
- **Process**:
  - Combines the input parameters in a way that produces a unique hash value.
- **Usage**:
  ```c
  size_t hash = hash_function(1, 2, 3);
  printf("Hash: %zu", hash);
  ```

## 2. `map_node_t* find_register_node(unsigned int device_id, unsigned int register_id, unsigned int sub_register);`
- **Description**: Finds and returns a pointer to the register node that matches the specified device, register, and sub-register IDs.
- **Parameters**:
  - `unsigned int device_id`: The ID of the device.
  - `unsigned int register_id`: The ID of the register.
  - `unsigned int sub_register`: The ID of the sub-register.
- **Process**:
  - Searches through the register map using the IDs to locate the corresponding node.
- **Usage**:
  ```c
  map_node_t *node = find_register_node(1, 2, 3);
  if (node) {
      // Process the node
  }
  ```

## 3. `int8_t initializeRegister(size_t size);`
- **Description**: Initializes the register management system with a specified size, usually the number of registers.
- **Parameters**:
  - `size_t size`: The number of registers to initialize.
- **Process**:
  - Allocates and sets up the necessary data structures for register management.
- **Usage**:
  ```c
  int8_t status = initializeRegister(100);
  if (status == 0) {
      // Initialization successful
  }
  ```

## 4. `bool add_register(unsigned int device_id, unsigned int register_id, unsigned int sub_register, RegisterType type, void *value, AccessMode access_mode);`
- **Description**: Adds a new register to the system with the specified parameters.
- **Parameters**:
  - `unsigned int device_id`: The ID of the device.
  - `unsigned int register_id`: The ID of the register.
  - `unsigned int sub_register`: The ID of the sub-register.
  - `RegisterType type`: The type of the register (e.g., U8, I16).
  - `void *value`: Pointer to the initial value of the register.
  - `AccessMode access_mode`: Access mode for the register (read, write).
- **Process**:
  - Allocates space for the register and adds it to the register map.
- **Usage**:
  ```c
  uint8_t initial_value = 10;
  bool success = add_register(1, 2, 3, U8, &initial_value, READ_WRITE);
  ```

## 5. `bool set_register(unsigned int device_id, unsigned int register_id, unsigned int sub_register, void *value);`
- **Description**: Sets the value of an existing register.
- **Parameters**:
  - `unsigned int device_id`: The ID of the device.
  - `unsigned int register_id`: The ID of the register.
  - `unsigned int sub_register`: The ID of the sub-register.
  - `void *value`: Pointer to the new value.
- **Process**:
  - Finds the register node and updates its value.
- **Usage**:
  ```c
  uint8_t new_value = 20;
  bool updated = set_register(1, 2, 3, &new_value);
  ```

## 6. `void get_register(unsigned int device_id, unsigned int register_id, unsigned int sub_register, void *outValue);`
- **Description**: Retrieves the value of the specified register.
- **Parameters**:
  - `unsigned int device_id`: The ID of the device.
  - `unsigned int register_id`: The ID of the register.
  - `unsigned int sub_register`: The ID of the sub-register.
  - `void *outValue`: Pointer to store the retrieved value.
- **Process**:
  - Locates the register node and copies its value to `outValue`.
- **Usage**:
  ```c
  uint8_t value;
  get_register(1, 2, 3, &value);
  printf("Register value: %d", value);
  ```

## 7. `int8_t แ(unsigned int device_id, unsigned int register_id, unsigned int sub_register, void *outValue, uint8_t *type_register);`
- **Description**: Retrieves the value and type of the specified register.
- **Parameters**:
  - `unsigned int device_id`: The ID of the device.
  - `unsigned int register_id`: The ID of the register.
  - `unsigned int sub_register`: The ID of the sub-register.
  - `void *outValue`: Pointer to store the retrieved value.
  - `uint8_t *type_register`: Pointer to store the type of the register.
- **Process**:
  - Finds the register node, retrieves its value, and returns the register type.
- **Usage**:
  ```c
  uint8_t value;
  uint8_t type;
  int8_t status = get_register_t(1, 2, 3, &value, &type);
  ```

## 8. `size_t count_used_registers();`
- **Description**: Returns the number of registers currently in use.
- **Process**:
  - Counts the number of occupied slots in the register map.
- **Usage**:
  ```c
  size_t used = count_used_registers();
  printf("Used registers: %zu", used);
  ```

## 9. `size_t count_available_slots();`
- **Description**: Returns the number of available slots for registers.
- **Process**:
  - Calculates the number of free slots in the register map.
- **Usage**:
  ```c
  size_t available = count_available_slots();
  printf("Available slots: %zu", available);
  ```

This documentation provides a comprehensive overview of the functions found in `csp_register.h`. Refer to the header file for additional details and context.


## bit_management.h

### 1. `void set_bits(uint32_t *data, uint32_t value, uint8_t start, uint8_t length);`
- **Description**: Sets a specific range of bits within a 32-bit data variable to a given value.
- **Parameters**:
  - `uint32_t *data`: Pointer to the data where bits will be set.
  - `uint32_t value`: Value to set within the specified bit range.
  - `uint8_t start`: The starting bit position.
  - `uint8_t length`: The length of the bit range to set.
  
#### **Process**:
1. The function first masks the bits to be set in the target data.
2. It shifts the value to align with the target bit positions.
3. The original bits are cleared, and the new bits are set using bitwise OR.

#### **Usage**:
```c
uint32_t data = 0x00;
set_bits(&data, 0xF, 4, 4);  // Sets bits 4-7 of data to 0xF
printf("Data after setting bits: 0x%X", data);
```

### 2. `uint32_t get_bits(uint32_t data, uint8_t start, uint8_t length);`
- **Description**: Extracts a specific range of bits from a 32-bit data variable.
- **Parameters**:
  - `uint32_t data`: The data from which bits will be extracted.
  - `uint8_t start`: The starting bit position.
  - `uint8_t length`: The length of the bit range to extract.
  
#### **Process**:
1. The function shifts the desired bit range to the least significant bit position.
2. It masks the bits to isolate the range.

#### **Usage**:
```c
uint32_t data = 0xFF00;
uint32_t result = get_bits(data, 8, 8);  // Extracts bits 8-15
printf("Extracted bits: 0x%X", result);
```

### 3. `size_t get_value_size(uint8_t type_register);`
- **Description**: Returns the size of a value based on its register type.
- **Parameters**:
  - `uint8_t type_register`: The type of the register, which determines the size.
  
#### **Process**:
1. The function uses a switch case or lookup mechanism to determine the size based on `type_register`.

#### **Usage**:
```c
uint8_t type = U8;
size_t size = get_value_size(type);  // Returns size of U8 type
printf("Size of U8 type: %zu", size);
```